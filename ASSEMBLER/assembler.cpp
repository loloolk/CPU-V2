#pragma once

#include <string>
#include <vector>
#include <map>
#include <tuple>
#include "tokenizer.cpp"

#include <iostream>

struct Instruction {
    uint16_t opcode;
    uint16_t inputs;
    uint16_t offset;
};
static std::map<std::string, Instruction> instructions = {
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
    Tokenizer* tokenizer;

    int locationCounter = 1; // Current instruction index

    std::map<std::string, uint16_t> labels;

    Assembler(IO* io) {
        this->io = io;
        this->tokenizer = new Tokenizer(io);
        
        this->locationCounter = 1;

        this->labels = {};
    }
    ~Assembler() {
        delete tokenizer;
    }

    void assemble() {
        std::tuple<TOKEN_TYPE, std::string> token;
        io->write(0);

        // First pass: Collect labels and handle memory offsets
        while (std::get<0>(token = this->tokenizer->nextTokenLog()) != TOKEN_EOF) {

            if (std::get<0>(token) == TOKEN_TAG) {
                // Handle label definition
                token = this->tokenizer->nextTokenLog();
                if (std::get<0>(token) != TOKEN_ID) {
                    throw std::runtime_error("Expected label name after tag at line " + std::to_string(this->tokenizer->lineNumber));
                }

                this->labels[std::get<1>(token)] = this->locationCounter;

                token = this->tokenizer->nextTokenLog();
                if (std::get<0>(token) != TOKEN_COLON) {
                    throw std::runtime_error("Expected colon after label at line " + std::to_string(this->tokenizer->lineNumber));
                }
                continue;
            }
            else if (std::get<0>(token) == TOKEN_TILDE) {
                // handle memory offset
                token = this->tokenizer->nextTokenLog();
                if (std::get<0>(token) != TOKEN_NUMBER) {
                    throw std::runtime_error("Expected number after tilde at line " + std::to_string(this->tokenizer->lineNumber));
                }

                int offset = this->parseNumber(std::get<1>(token));
                this->locationCounter += offset;

                token = this->tokenizer->nextTokenLog();
                if (std::get<0>(token) != TOKEN_SEMICOLON) {
                    throw std::runtime_error("Expected semicolon after tilde at line " + std::to_string(this->tokenizer->lineNumber));
                }

                continue;
            }
        
            if (std::get<0>(token) != TOKEN_ID) {
                throw std::runtime_error("Expected instruction name at line " + std::to_string(this->tokenizer->lineNumber));
            }

            std::string instructionName = std::get<1>(token);
            
            if (instructionName[0] == 'i') {
                // Handle immediate instruction
                instructionName = instructionName.substr(1);
                this->locationCounter++;
            }
            if (instructionName[instructionName.size() - 1] == 'i') {
                // Handle immediate instruction
                instructionName = instructionName.substr(0, instructionName.size() - 1);
                this->locationCounter++;
            }
            if (instructions.find(instructionName) == instructions.end()) {
                throw std::runtime_error("Unknown instruction: " + instructionName + " at line " + std::to_string(this->tokenizer->lineNumber));
            }
            
            Instruction instruction = instructions[instructionName];
            this->locationCounter += instruction.offset;
            
            for (int i = 0; i < 3; i++) {
                if (instruction.inputs & (1 << i)) {
                    token = this->tokenizer->nextTokenLog();
                    if (std::get<0>(token) != TOKEN_ID && std::get<0>(token) != TOKEN_NUMBER) {
                        throw std::runtime_error("Expected input for instruction " + instructionName + " at line " + std::to_string(this->tokenizer->lineNumber));
                    }
                }
            }

            token = this->tokenizer->nextTokenLog();
            if (std::get<0>(token) != TOKEN_SEMICOLON) {
                throw std::runtime_error("Expected semicolon after instruction " + instructionName + " at line " + std::to_string(this->tokenizer->lineNumber));
            }

            // If we reach here, the instruction is valid and we can continue
        }

        std::cout << "First pass complete." << std::endl;
        
        // Reset tokenizer for the second pass
        this->tokenizer->reset();

        // Second pass: Generate machine code
        while (std::get<0>(token = this->tokenizer->nextTokenLog()) != TOKEN_EOF) {

            if (std::get<0>(token) == TOKEN_TAG) {
                // Handle label definition
                token = this->tokenizer->nextTokenLog();
                token = this->tokenizer->nextTokenLog();
                continue;
            }
            else if (std::get<0>(token) == TOKEN_TILDE) {
                // handle memory offset
                token = this->tokenizer->nextTokenLog();

                int offset = this->parseNumber(std::get<1>(token));
                for (int i = 0; i < offset; i++) {
                    this->io->write(0); // Write zeroes for the offset
                }

                token = this->tokenizer->nextTokenLog();
                continue;
            }
        
            // If we reach here, we expect an instruction
            if (std::get<0>(token) != TOKEN_ID) {
                throw std::runtime_error("Expected instruction name at line " + std::to_string(this->tokenizer->lineNumber));
            }
            
            uint16_t command = 0;

            std::string instructionName = std::get<1>(token);

            bool imm_src1 = instructionName[0] == 'i';
            bool imm_src2 = instructionName[instructionName.size() - 1] == 'i';

            // Check if the instruction is immediate
            if (imm_src1) {
                // Handle immediate instruction
                instructionName = instructionName.substr(1);
                command |= 0x1 << 15;
            }
            if (imm_src2) {
                // Handle immediate instruction
                instructionName = instructionName.substr(0, instructionName.size() - 1);
                command |= 0x1 << 14;
            }

            if (instructions.find(instructionName) == instructions.end()) {
                throw std::runtime_error("Unknown instruction: " + instructionName + " at line " + std::to_string(this->tokenizer->lineNumber));
            }

            Instruction instruction = instructions[instructionName];
            command |= instruction.opcode << 9; // Set the opcode

            // Handle inputs
            uint16_t args[3] = {0, 0, 0};  // src1, src2, dest
            int bitShifts[3] = {6, 3, 0};              // bit positions in command
            bool isImmediate[3] = {imm_src1, imm_src2, false};  // only src1/src2 can be immediate

            for (int i = 0; i < 3; i++) {
                if (instruction.inputs & (1 << (2 - i))) {
                    token = tokenizer->nextTokenLog();

                    if (std::get<0>(token) != TOKEN_ID && std::get<0>(token) != TOKEN_NUMBER) {
                        throw std::runtime_error("Expected argument for " + instructionName +
                                                " at line " + std::to_string(tokenizer->lineNumber));
                    }

                    args[i] = handleSrc(token, isImmediate[i]);

                    if (!isImmediate[i]) {
                        command |= args[i] << bitShifts[i];
                    }
                }
            }

            // Write the command to the output
            this->io->write(command);
            // If the instruction has immediate sources, write them as well
            if (instruction.inputs & 0b100 && imm_src1)
                this->io->write(args[0]);
            if (instruction.inputs & 0b10 && imm_src2)
                this->io->write(args[1]);

            token = this->tokenizer->nextTokenLog();
            if (std::get<0>(token) != TOKEN_SEMICOLON) {
                throw std::runtime_error("Expected semicolon after instruction " + instructionName + 
                                        " at line " + std::to_string(this->tokenizer->lineNumber));
            }

            // If we reach here, the instruction is valid and we can continue
            continue;
        }

        std::cout << "Second pass complete." << std::endl;
    }

    uint16_t handleSrc(std::tuple<TOKEN_TYPE, std::string> token, bool immediate = false) {
        if (immediate) {
            if (std::get<0>(token) == TOKEN_NUMBER) {
                return this->parseNumber(std::get<1>(token));
            }
            else if (std::get<0>(token) == TOKEN_ID) {
                if (this->labels.find(std::get<1>(token)) != this->labels.end()) {
                    return this->labels[std::get<1>(token)];
                }
                
                throw std::runtime_error("Error: Unknown label '" + std::get<1>(token) + "' at line " + std::to_string(this->tokenizer->lineNumber));
            }

            throw std::runtime_error("Error: Invalid immediate source at line " + std::to_string(this->tokenizer->lineNumber));
        }
        if (std::get<0>(token) == TOKEN_ID) {
            if (registers.find(std::get<1>(token)) != registers.end()) {
                return registers[std::get<1>(token)];
            }
            else {
                throw std::runtime_error("Error: Unknown register or label '" + std::get<1>(token) + "' at line " + std::to_string(this->tokenizer->lineNumber));
            }
        }

        throw std::runtime_error("Error: Invalid source token at line " + std::to_string(this->tokenizer->lineNumber));
    }

    int parseNumber(std::string number, int minNum = 0, int maxNum = 0xFFFF) {
        // Hexadecimal number
        if (number.size() > 2 && number[0] == '0' && (number[1] == 'x' || number[1] == 'X')) {
            int num = std::stoi(number.substr(2), nullptr, 16);
            if (num < minNum || num > maxNum) {
                throw std::runtime_error("Error: Hexadecimal number out of range at line " + std::to_string(this->tokenizer->lineNumber));
            }
            return num;
        }

        // Decimal number
        else {
            int num = std::stoi(number);
            if (num < minNum || num > maxNum) {
                throw std::runtime_error("Error: Decimal number out of range at line " + std::to_string(this->tokenizer->lineNumber));
            }
            return num;
        }
    }
};