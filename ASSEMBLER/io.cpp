#include <cstdint>
#include <fstream>
#include <string>

class IO {
    public:
    std::ifstream in;
    std::ofstream out;

    IO() {
        in.open("input.txt");
        out.open("output.txt");
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

    void write(uint16_t val) {
        out << std::hex << val << std::endl;
    }
};