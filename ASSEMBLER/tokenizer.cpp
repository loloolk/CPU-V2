#pragma once

#include <iostream>
#include <string>
#include <map>
#include <tuple>
#include "IO.cpp"

enum TOKEN_TYPE {
    TOKEN_TAG, // #
    TOKEN_TILDE, // ~
    TOKEN_DOLLAR, // $

    TOKEN_PLUS, // +
    TOKEN_MINUS, // -
    TOKEN_SHIFT_LEFT, // < 5
    TOKEN_SHIFT_RIGHT, // >

    TOKEN_AND, // &
    TOKEN_OR, // |
    TOKEN_NAND, // !&
    TOKEN_NOR, // !| 10
    TOKEN_XOR, // ^
    TOKEN_NXOR, // !^
    TOKEN_NOT, // !
    
    TOKEN_ID, // alphabetical
    TOKEN_NUMBER, // numerical 15

    TOKEN_COMMA, // ,
    TOKEN_COLON, // :
    TOKEN_SEMICOLON, // ;

    TOKEN_EOF, // end of file
};

class Tokenizer {
    public:
    IO* io;
    std::string line = "";
    int lineNumber = 0;
    int charNumber = 0;

    std::map<std::string, TOKEN_TYPE> keywords = {
        {"#", TOKEN_TAG},
        {"~", TOKEN_TILDE},
        {"$", TOKEN_DOLLAR},

        {"+", TOKEN_PLUS},
        {"-", TOKEN_MINUS},
        {"<", TOKEN_SHIFT_LEFT},
        {">", TOKEN_SHIFT_RIGHT},

        {"&", TOKEN_AND},
        {"|", TOKEN_OR},
        {"^", TOKEN_XOR},
        {"!", TOKEN_NOT},
        
        {"!&", TOKEN_NAND},
        {"!|", TOKEN_NOR},
        {"!^", TOKEN_NXOR},

        {",", TOKEN_COMMA},
        {":", TOKEN_COLON},
        {";", TOKEN_SEMICOLON},
    };
    Tokenizer(IO* io) {
        this->io = io;

        this->line = io->read();
        this->lineNumber = 0;
        this->charNumber = 0;
    }

    std::tuple<TOKEN_TYPE, std::string> nextToken() {
        // Skip whitespace
        while ( charNumber >= line.size() || isspace(line[charNumber]) || line[charNumber] == '\n' || line[charNumber] == '\r') {
            if (charNumber >= line.size() || line[charNumber] == '\n') {
                // Move to the next line
                lineNumber++;
                charNumber = 0;

                if (io->in.peek() == EOF) {
                    return {TOKEN_EOF, "EOF"}; // End of file
                }

                line = io->read();
            }
            else {
                charNumber++;
            }
        }

        if (isalpha(line[charNumber]) || line[charNumber] == '_') {
            std::string id = "";

            while (charNumber < line.size() && (isalpha(line[charNumber]) || line[charNumber] == '_')) {
                id += line[charNumber];
                charNumber++;
            }

            return {TOKEN_ID, id};
        }

        if (isdigit(line[charNumber])) {
            std::string number = "";

            // Hexadecimal number check
            if (line[charNumber] == '0' && (line.size() > 1 && (line[charNumber + 1] == 'x' || line[charNumber + 1] == 'X'))) {
                if (line.size() <= charNumber + 2 || !isxdigit(line[charNumber + 2])) {
                    throw std::runtime_error("Error: Invalid hexadecimal number at line " + std::to_string(lineNumber) + ", character " + std::to_string(charNumber));
                }

                number += "0x"; // Prefix for hexadecimal

                // Hexadecimal number
                charNumber += 2; // Skip "0x"

                while (charNumber < line.size() && isxdigit(line[charNumber])) {
                    number += line[charNumber];
                    charNumber++;
                }

                return {TOKEN_NUMBER, number};
            }

            // Decimal number check 
            else {
                while (charNumber < line.size() && isdigit(line[charNumber])) {
                    number += line[charNumber];
                    charNumber++;
                }
            }

            return {TOKEN_NUMBER, number};
        }
        
        if (keywords.find(std::string(1, line[charNumber])) == keywords.end()) {
            throw std::runtime_error("Error: Unknown token '" + std::string(1, line[charNumber]) + "' at line " + std::to_string(lineNumber) + ", character " + std::to_string(charNumber));
        }

        // If the character is a keyword, return its type
        std::string keyword = std::string(1, line[charNumber]);
        TOKEN_TYPE tokenType = keywords[keyword];
        charNumber++;
        if (tokenType == TOKEN_NOT) {
            // Check for NAND, NOR, NXOR
            if (charNumber < line.size() && line[charNumber] == '&') {
                charNumber++;
                return {TOKEN_NAND, "!&"};
            }
            else if (charNumber < line.size() && line[charNumber] == '|') {
                charNumber++;
                return {TOKEN_NOR, "!|"};
            }
            else if (charNumber < line.size() && line[charNumber] == '^') {
                charNumber++;
                return {TOKEN_NXOR, "!^"};
            }
        }

        return {tokenType, keyword};
    }

    std::tuple<TOKEN_TYPE, std::string> nextTokenLog() {
        std::tuple<TOKEN_TYPE, std::string> token = nextToken();
        
        // std::cout << "Token: " << std::get<0>(token) << ", Value: " << std::get<1>(token) << std::endl;

        return token;
    }

    void reset() {
        io->reset();
        line = io->read();
        lineNumber = 0;
        charNumber = 0;
    }
};