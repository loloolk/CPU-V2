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
            
            bool imm = (instruction >> 15) & 0b1;
            bool inv = (instruction >> 14) & 0b1;

            uint16_t opcode = (instruction >> 10) & 0b1111;
            uint16_t src1 = (instruction >> 7) & 0b111;
            uint16_t src2 = (instruction >> 4) & 0b111;
            uint16_t dest = (instruction >> 1) & 0b111;

            if (imm) {
                src1 = ROM[++this->pc];
            }

            switch (opcode) {
                case 0b0000: // NOOP
                    break;
                case 0b0001: // MOV
                    handleMov(src1, dest, inv, imm);
                    break;
                case 0b0010: // AND
                    handleAnd(src1, src2, dest, inv, imm);
                    break;
                case 0b0011: // OR
                    handleOr(src1, src2, dest, inv, imm);
                    break;
                case 0b0100: // XOR
                    handleXor(src1, src2, dest, inv, imm);
                    break;
                case 0b0101: // ADD
                    handleAdd(src1, src2, dest, inv, imm);
                    break;
                case 0b0110: // SUB
                    handleSub(src1, src2, dest, inv, imm);
                    break;
                case 0b0111: // SHL
                    handleShl(src1, dest, inv, imm);
                    break;
                case 0b1000: // SHR
                    handleShr(src1, dest, inv, imm);
                    break;
                case 0b1001: // CMP
                    handleCmp(src1, src2, imm);
                    break;
                case 0b1010: // JMP
                    handleJmp(src1, imm);
                    pc--;
                    break;
                case 0b1011: // JEQ
                    handleJeq(src1, inv, imm);
                    pc--;
                    break;
                case 0b1100: // JL
                    handleJl(src1, inv, imm);
                    pc--;
                    break;
                case 0b1101: // JG
                    handleJg(src1, inv, imm);
                    pc--;
                    break;
                case 0b1110: // WRITE_RAM
                    handleWriteRAM(src1, src2, inv, imm);
                    break;
                case 0b1111: // READ_RAM
                    handleReadRAM(src2, dest, inv, imm);
                    break;
                default:
                    std::cout << "Invalid opcode: " << opcode << std::endl;
                    break;
            }
            pc++;
        }
    }

    void handleMov(uint16_t src, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~src;
            }
            else {
                registers[dest] = ~registers[src];
            }
        }
        else {
            if (imm) {
                registers[dest] = src;
            }
            else {
                registers[dest] = registers[src];
            }
        }
    }
    void handleAnd(uint16_t src1, uint16_t src2, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~(src1 & registers[src2]);
            }
            else {
                registers[dest] = ~(registers[src1] & registers[src2]);
            }
        }
        else {
            if (imm) {
                registers[dest] = src1 & registers[src2];
            }
            else {
                registers[dest] = registers[src1] & registers[src2];
            }
        }
    }
    void handleOr(uint16_t src1, uint16_t src2, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~(src1 | registers[src2]);
            }
            else {
                registers[dest] = ~(registers[src1] | registers[src2]);
            }
        }
        else {
            if (imm) {
                registers[dest] = src1 | registers[src2];
            }
            else {
                registers[dest] = registers[src1] | registers[src2];
            }
        }
    }
    void handleXor(uint16_t src1, uint16_t src2, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~(src1 ^ registers[src2]);
            }
            else {
                registers[dest] = ~(registers[src1] ^ registers[src2]);
            }
        }
        else {
            if (imm) {
                registers[dest] = src1 ^ registers[src2];
            }
            else {
                registers[dest] = registers[src1] ^ registers[src2];
            }
        }
    }
    void handleAdd(uint16_t src1, uint16_t src2, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~(src1 + registers[src2]);
            }
            else {
                registers[dest] = ~(registers[src1] + registers[src2]);
            }
        } 
        else {
            if (imm) {
                registers[dest] = src1 + registers[src2];
            }
            else {
                registers[dest] = registers[src1] + registers[src2];
            }
        }
    }
    void handleSub(uint16_t src1, uint16_t src2, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~(src1 - registers[src2]);
            }
            else {
                registers[dest] = ~(registers[src1] - registers[src2]);
            }
        }
        else {
            if (imm) {
                registers[dest] = src1 - registers[src2];
            }
            else {
                registers[dest] = registers[src1] - registers[src2];
            }
        }
    }
    void handleShl(uint16_t src, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~(src << 1);
            }
            else {
                registers[dest] = ~(registers[src] << 1);
            }
        }
        else {
            if (imm) {
                registers[dest] = src << 1;
            }
            else {
                registers[dest] = registers[src] << 1;
            }
        }
    }
    void handleShr(uint16_t src, uint16_t dest, bool inv, bool imm) {
        if (inv) {
            if (imm) {
                registers[dest] = ~(src >> 1);
            }
            else {
                registers[dest] = ~(registers[src] >> 1);
            }
        }
        else {
            if (imm) {
                registers[dest] = src >> 1;
            }
            else {
                registers[dest] = registers[src] >> 1;
            }
        }
    }
    void handleCmp(uint16_t src1, uint16_t src2, bool imm) {
        if (imm) {
            cmpReg = (src1 < registers[src2]) << 2 | (src1 == registers[src2]) << 1 | (src1 > registers[src2]);
        }
        else {
            cmpReg = (registers[src1] < registers[src2]) << 2 | (registers[src1] == registers[src2]) << 1 | (registers[src1] > registers[src2]);
        }
    }
    void handleJmp(uint16_t src, bool imm) {
        if (imm) {
            pc = src;
        }
        else {
            pc = registers[src];
        }
    }
    void handleJeq(uint16_t src, bool inv, bool imm) {
        if (inv) {
            if (cmpReg != 0b010) {
                if (imm) {
                    pc = src;
                }
                else {
                    pc = registers[src];
                }
            }
        }
        else {
            if (cmpReg == 0b010) {
                if (imm) {
                    pc = src;
                }
                else {
                    pc = registers[src];
                }
            }
        }
    }
    void handleJl(uint16_t src, bool inv, bool imm) {
        if (inv) {
            if (cmpReg != 0b100) {
                if (imm) {
                    pc = src;
                }
                else {
                    pc = registers[src];
                }
            }
        }
        else {
            if (cmpReg == 0b100) {
                if (imm) {
                    pc = src;
                }
                else {
                    pc = registers[src];
                }
            }
        }
    }
    void handleJg(uint16_t src, bool inv, bool imm) {
        if (inv) {
            if (cmpReg != 0b001) {
                if (imm) {
                    pc = src;
                }
                else {
                    pc = registers[src];
                }
            }
        }
        else {
            if (cmpReg == 0b001) {
                if (imm) {
                    pc = src;
                }
                else {
                    pc = registers[src];
                }
            }
        }
    }
    void handleWriteRAM(uint16_t src1, uint16_t src2, bool inv, bool imm) {
        if (inv) { // PUSH
            if (imm) {
                RAM[registers[7]] = src1;
                registers[7]++;
                std::cout << "PUSH: " << src1 << std::endl;
            }
            else {
                RAM[registers[7]] = registers[src1];
                registers[7]++;
                std::cout << "PUSH: " << registers[src1] << std::endl;
            }
        }
        else {
            if (imm) {
                RAM[registers[src2]] = src1;
            }
            else {
                RAM[registers[src2]] = registers[src1];
            }
        }
    }
    void handleReadRAM(uint16_t src2, uint16_t dest, bool inv, bool imm) {
        if (inv) { // POP
            if (imm) {
                registers[7]--;
                registers[dest] = RAM[registers[7]];
            }
            else {
                registers[7]--;
                registers[dest] = RAM[registers[7]];
            }
        }
        else {
            if (imm) {
                registers[dest] = RAM[registers[src2]];
            }
            else {
                registers[dest] = RAM[registers[src2]];
            }
        }
    }
};