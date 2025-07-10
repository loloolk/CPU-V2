#include <cstdint>
#include <fstream>
#include <string>

class IO {
    public:
    std::ifstream in;
    std::ofstream out;

    IO(std::string inputFile = "input.txt", std::string outputFile = "output.txt") {
        in.open(inputFile);
        if (!in.is_open()) {
            throw std::runtime_error("Could not open input file: " + inputFile);
        }
        out.open(outputFile);
        if (!out.is_open()) {
            throw std::runtime_error("Could not open output file: " + outputFile);
        }
    }
    ~IO() {
        in.close();
        out.close();
    }

    std::string read() {
        std::string line;
        std::getline(in, line);
        
        return line;
    }

    void write(std::string val) {
        out << val << std::endl;
    }
    void write(uint16_t val) {
        out << std::hex << val << std::endl;
    }

    void reset() {
        in.clear();
        in.seekg(0, std::ios::beg);
    }
};