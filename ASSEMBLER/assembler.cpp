#include <iostream>
#include <string>
#include <map>
#include "IO.cpp"

struct Opcode {
    uint16_t opcode;
    uint16_t inputs;
    uint16_t offset;
};

static std::map<std::string, Opcode> opcodes = {
    // STANDARD OPCODES
    {"NOOP", {0x0 , 0b000, 1}},
    {"MOV",  {0x1 , 0b101, 1}},
    {"NOT",  {0x11, 0b101, 1}},
    {"AND",  {0x2 , 0b111, 1}},
    {"NAND", {0x12, 0b111, 1}},
    {"OR",   {0x3 , 0b111, 1}},
    {"NOR",  {0x13, 0b111, 1}},
    {"XOR",  {0x4 , 0b111, 1}},
    {"NXOR", {0x14, 0b111, 1}},
    {"ADD",  {0x5 , 0b111, 1}},
    {"SUB",  {0x6 , 0b111, 1}},
    {"SHL",  {0x7 , 0b101, 1}},
    {"SHR",  {0x8 , 0b101, 1}},
    {"CMP",  {0x9 , 0b110, 1}},
    {"JMP",  {0xa , 0b100, 1}},
    {"JEQ",  {0xb , 0b100, 1}},
    {"JNE",  {0x1b, 0b100, 1}},
    {"JLT",  {0xc , 0b100, 1}},
    {"JGE",  {0x1c, 0b100, 1}},
    {"JGT",  {0xd , 0b100, 1}},
    {"JLE",  {0x1d, 0b100, 1}},
    {"WRT",  {0xe , 0b110, 1}},
    {"PUSH", {0x1e, 0b100, 1}},
    {"READ", {0xf , 0b011, 1}},
    {"POP",  {0x1f, 0b001, 1}}
};
static std::map<std::string, uint16_t> registers = {
    {"a", 0x0},
    {"b", 0x1},
    {"c", 0x2},
    {"d", 0x3},
    {"e", 0x4},
    {"f", 0x5},
    {"bp", 0x6},
    {"sp", 0x7}
};

class Assembler {
    public:
    IO* io;

    std::map<std::string, uint16_t> tags;

    Assembler(IO* io) {
        this->io = io;
        this->tags = {};
    }

    std::string getChunk(std::string line, int& i) {
        std::string chunk = "";
        for (; i < line.size(); i++) {
            if (line[i] == ' ') {
                break;
            }
            chunk += line[i];
        }
        i++;
        return chunk;
    }

    uint16_t handleSrc(std::string src, bool immediate = false) {
        if (immediate) {
            if (isdigit(src[0])) {
                return std::stoi(src);
            }
            else if (tags.find(src) != tags.end()) {
                return tags[src];
            }

            io->write(0xdead);
            exit(1);
        }
        if (registers.find(src) != registers.end()) {
            return registers[src];
        }

        io->write(0xdead);
        exit(1);
    }

    bool assembleLine() {
        uint16_t command = 0;

        std::string line = io->read();
        if (line == "") {
            return io->in.peek() != EOF;
        }
        if (line[0] == '#') {
            return io->in.peek() != EOF;
        }
        if (line[0] == '~') {
            line = line.substr(1);
            for (int i = 0; i < std::stoi(line); i++) {
                io->write(0);
            }
            return io->in.peek() != EOF;
        }

        int i = 0;
        std::string op = getChunk(line, i);

        bool imm_src1 = op[0] == 'i';
        bool imm_src2 = op[op.size() - 1] == 'i';

        if (imm_src1) {
            op = op.substr(1);
            command |= 0x1 << 15;
        }
        if (imm_src2) {
            op = op.substr(0, op.size() - 1);
            command |= 0x1 << 14;
        }
        command |= opcodes[op].opcode << 9;

        uint16_t src1;
        if (opcodes[op].inputs & 0b100) {
            std::string src = getChunk(line, i);
            src1 = handleSrc(src, imm_src1);
            if (!imm_src1) {
                command |= src1 << 6;
            }
        }

        uint16_t src2;
        if (opcodes[op].inputs & 0b10) {
            std::string src = getChunk(line, i);
            src2 = handleSrc(src, imm_src2);
            if (!imm_src2) {
                command |= src2 << 3;
            }
        }

        if (opcodes[op].inputs & 0b1) {
            std::string dest = getChunk(line, i);
            command |= handleSrc(dest);
        }

        io->write(command);

        if (opcodes[op].inputs & 0b100 && imm_src1) {
            io->write(src1);
        }
        if (opcodes[op].inputs & 0b10 && imm_src2) {
            io->write(src2);
        }

        return io->in.peek() != EOF;
    }
    void getTags() {
        int PC = 1;

        while (io->in.peek() != EOF) {
            int i = 0;
                
            std::string line = io->read();
            if (line == "") {
                continue;
            }

            std::string code = getChunk(line, i);
            if (line[0] != '#') {
                if (code[0] == '~') {
                    code = code.substr(1);
                    PC += std::stoi(code);
                    continue;
                }

                if (code[0] == 'i') {
                    PC++;
                    code = code.substr(1);
                }
                if (code[code.size() - 1] == 'i') {
                    PC++;
                    code = code.substr(0, code.size() - 1);
                }

                PC += opcodes[code].offset;
                continue;
            }

            tags[line.substr(1)] = PC;
        }

        io->in.clear();
        io->in.seekg(0);
    }

    void assemble() {
        getTags();
        
        io->write(0);
        while (assembleLine()) {}
    }
};