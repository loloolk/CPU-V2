#include "assembler.cpp"

int main() {
    IO* io = new IO();
    Assembler assembler = Assembler(io);
    
    assembler.assemble();
    
    return 0;
}