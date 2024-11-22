#include "CPU.cpp"

uint16_t program[] = {0x0, 0x8402, 0x1, 0x1414, 0x7900, 0x480, 0x502, 0xa800, 0x3};

int main() {
    CPU cpu;
    cpu.loadProgram(program, sizeof(program) / sizeof(program[0]));
    cpu.run();
    return 0;
}