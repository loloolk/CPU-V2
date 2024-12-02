#include <stdint.h>
#include <iostream>

class CPU {
    public:
    uint16_t registers[8];
    uint16_t cmpReg;
    uint16_t ROM[1 << 8] = {0};
    uint16_t RAM[1 << 8] = {0};

    uint16_t pc;

    CPU() {
        for (int i = 0; i < 8; i++) {
            registers[i] = 0;
        }
        cmpReg = 0;
        pc = 0;
    }
    ~CPU() {}

    void loadProgram(uint16_t program[], int size) {
        for (int i = 0; i < size; i++) {
            ROM[i] = program[i];
        }
    }

    void run() {
        while (this->pc < (1 << 16)) {
            uint16_t instruction = ROM[pc];
            
            bool imm_src1 = (instruction >> 15) & 0b1;
            bool imm_src2 = (instruction >> 14) & 0b1;
            bool inv = (instruction >> 13) & 0b1;

            uint16_t opcode = (instruction >> 9) & 0b1111;
            uint16_t src1 = (instruction >> 6) & 0b111;
            uint16_t src2 = (instruction >> 3) & 0b111;
            uint16_t dest = instruction & 0b111;

            if (imm_src1) {
                src1 = ROM[++this->pc];
            }
            else {
                src1 = registers[src1];
            }
            if (imm_src2) {
                src2 = ROM[++this->pc];
            }
            else {
                src2 = registers[src2];
            }

            switch (opcode) {
                case 0b0000: // NOOP
                    break;
                case 0b0001: // MOV
                    handleMov(src1, dest, inv);
                    break;
                case 0b0010: // AND
                    handleAnd(src1, src2, dest, inv);
                    break;
                case 0b0011: // OR
                    handleOr(src1, src2, dest, inv);
                    break;
                case 0b0100: // XOR
                    handleXor(src1, src2, dest, inv);
                    break;
                case 0b0101: // ADD
                    handleAdd(src1, src2, dest, inv);
                    break;
                case 0b0110: // SUB
                    handleSub(src1, src2, dest, inv);
                    break;
                case 0b0111: // SHL
                    handleShl(src1, dest, inv);
                    break;
                case 0b1000: // SHR
                    handleShr(src1, dest, inv);
                    break;
                case 0b1001: // CMP
                    handleCmp(src1, src2);
                    break;
                case 0b1010: // JMP
                    handleJmp(src1, inv);
                    pc--;
                    break;
                case 0b1011: // JEQ
                    handleJeq(src1, inv);
                    pc--;
                    break;
                case 0b1100: // JL
                    handleJl(src1, inv);
                    pc--;
                    break;
                case 0b1101: // JG
                    handleJg(src1, inv);
                    pc--;
                    break;
                case 0b1110: // WRITE_RAM
                    handleWriteRAM(src1, src2, inv);
                    break;
                case 0b1111: // READ_RAM
                    handleReadRAM(src2, dest, inv);
                    break;
                default:
                    std::cout << "Invalid opcode: " << opcode << std::endl;
                    break;
            }
            pc++;
        }
    }

    void handleMov(uint16_t src, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~src;
        }
        else {
            registers[dest] = src;
        }
    }
    void handleAnd(uint16_t src1, uint16_t src2, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~(src1 & src2);
        }
        else {
            registers[dest] = src1 & src2;
        }
    }
    void handleOr(uint16_t src1, uint16_t src2, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~(src1 | src2);
        }
        else {
            registers[dest] = src1 | src2;
        }
    }
    void handleXor(uint16_t src1, uint16_t src2, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~(src1 ^ src2);
        }
        else {
            registers[dest] = src1 ^ src2;
        }
    }
    void handleAdd(uint16_t src1, uint16_t src2, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~(src1 + src2);
        }
        else {
            registers[dest] = src1 + src2;
        }
    }
    void handleSub(uint16_t src1, uint16_t src2, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~(src1 - src2);
        }
        else {
            registers[dest] = src1 - src2;
        }
    }
    void handleShl(uint16_t src, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~(src << 1);
        }
        else {
            registers[dest] = src << 1;
        }
    }
    void handleShr(uint16_t src, uint16_t dest, bool inv) {
        if (inv) {
            registers[dest] = ~(src >> 1);
        }
        else {
            registers[dest] = src >> 1;
        }
    }
    void handleCmp(uint16_t src1, uint16_t src2) {
        cmpReg = (src1 < src2) << 2 | (src1 == src2) << 1 | (src1 > src2);
    }
    void handleJmp(uint16_t src, bool inv) {
        if (!inv) {
            pc = src;
        }
    }
    void handleJeq(uint16_t src, bool inv) {
        if (inv) {
            if (!(cmpReg & 0b10)) {
                pc = src;
            }
        }
        else {
            if (cmpReg & 0b10) {
                pc = src;
            }
        }
    }
    void handleJl(uint16_t src, bool inv) {
        if (inv) {
            if (!(cmpReg & 0b100)) {
                pc = src;
            }
        }
        else {
            if (cmpReg & 0b100) {
                pc = src;
            }
        }
    }
    void handleJg(uint16_t src, bool inv) {
        if (inv) {
            if (!(cmpReg & 0b1)) {
                pc = src;
            }
        }
        else {
            if (cmpReg & 0b1) {
                pc = src;
            }
        }
    }
    void handleWriteRAM(uint16_t src1, uint16_t src2, bool inv) {
        if (inv) { // PUSH
            RAM[registers[7]++] = src1;
        }
        else {
            RAM[src2] = src1;
        }
    }
    void handleReadRAM(uint16_t src2, uint16_t dest, bool inv) {
        if (inv) { // POP
            registers[dest] = RAM[--registers[7]];
        }
        else {
            registers[dest] = RAM[src2];
        }
    }
};