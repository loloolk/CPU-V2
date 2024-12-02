#include "CPU.cpp"

uint16_t program[] = {
    0x0,
    0x8200,
    0x64,
    0x207,
    0x8201,
    0x2,
    0x3c00,
    0x1000,
    0x1208,
    0x3e00,
    0x9800,
    0x18,
    0x242,
    0xe82,
    0x9c10,
    0xffff,
    0xa52,
    0x1280,
    0x9800,
    0xe,
    0x4a41,
    0x1,
    0x9400,
    0x6
};

int main() {
    CPU cpu;
    cpu.loadProgram(program, sizeof(program) / sizeof(program[0]));
    cpu.run();
    return 0;
}