#include <iostream>
#include <string>
#include <map>
#include "IO.cpp"

struct Opcode {
    uint16_t opcode;
    uint16_t inputs;
    uint16_t offset;
};

class Assembler {
    public:
    IO* io;

    std::map<std::string, uint16_t> tags;
    std::map<std::string, Opcode> opcodes;
    std::map<std::string, uint16_t> registers;

    Assembler(IO* io) {
        this->io = io;
         
        this->tags = {};
        this->opcodes = {
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
        this->registers = {
            {"a", 0x0},
            {"b", 0x1},
            {"c", 0x2},
            {"d", 0x3},
            {"e", 0x4},
            {"f", 0x5},
            {"bp", 0x6},
            {"sp", 0x7}
        };
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
        if (line[0] == '#') {
            return io->in.peek() != EOF;
        }
        if (line == "") {
            return io->in.peek() != EOF;
        }

        int i = 0;
        std::string op = getChunk(line, i);

        bool immediate = op[op.size() - 1] == 'i';

        if (immediate) {
            command |= 1 << 15;

            op = op.substr(0, op.size() - 1);
        }
        command |= opcodes[op].opcode << 10;

        uint16_t src;
        if (opcodes[op].inputs & 0b100) {
            std::string src1 = getChunk(line, i);
            src = handleSrc(src1, immediate);
            if (!immediate) {
                command |= src << 7;
            }
        }

        if (opcodes[op].inputs & 0b10) {
            std::string src2 = getChunk(line, i);
            command |= handleSrc(src2) << 4;
        }

        if (opcodes[op].inputs & 0b1) {
            std::string dest = getChunk(line, i);
            command |= handleSrc(dest) << 1;
        }

        io->write(command);

        if (opcodes[op].inputs & 0b100 && immediate) {
            io->write(src);
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