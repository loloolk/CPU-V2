# CPU

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

Please Note: The Virtual Machine is still in development. Expect bugs and incomplete features.

## Description:
A 16-bit CPU, made with logisim evolution. This contains the CPU's Circuitry in `CPU`, an assembler for the CPU (from assembly to machine code) in `Assembler` and a Virtual Machine for the CPU in `VM`.

---

## Quick Start (CPU Simulation):
1) Download [logisim evolution](https://github.com/logisim-evolution/logisim-evolution)
2) Open `CPU/CPU.circ` in logisim evolution
3) Open `Assembler/input.txt` and put in your code
4) Run the `Assembler/main.cpp` file
5) Copy the `Assembler/outut.txt` into the Program Input chip
6) Run the CPU

---

## ASSEMBLER INSTRUCTIONS:
Registers: a, b, c, d, e, f, bp, sp

- `NOOP` : No operation
- `(i)MOV` `[Register/Value]` `[Register]` : Move a register/value to a register
- `(i)NOT` `[Register/Value]` `[Register]` : Negate a register
- `(i)AND(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise AND
- `(i)NAND(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise NAND
- `(i)OR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise OR
- `(i)NOR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise NOR
- `(i)XOR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise XOR
- `(i)XNOR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise XNOR
- `(i)ADD(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Add a register/value to a register
- `(i)SUB(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Subtract a register/value from a register **__*__**
- `(i)SHL(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Shift left
- `(i)SHR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Shift right
- `(i)CMP(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Compare two registers
- `(i)JMP` `[Register/Value]` : Jump to a register/value's address
- `(i)JEQ` `[Register/Value]` : Jump if the equality flag is set in the previous CMP instruction
- `(i)JNE` `[Register/Value]` : Jump if the equality flag is not set in the previous CMP instruction
- `(i)JGT` `[Register/Value]` : Jump if the greater flag is set in the previous CMP instruction
- `(i)JGE` `[Register/Value]` : Jump if the greater or equal flag is set in the previous CMP instruction
- `(i)JLT` `[Register/Value]` : Jump if the less flag is set in the previous CMP instruction
- `(i)JLE` `[Register/Value]` : Jump if the less or equal flag is set in the previous CMP instruction
- `(i)WRT(i)` `[Register/Value]` `[Register/Value]` : Write a register/value to the memory
- `(i)PUSH` `[Register/Value]` : Push a register/value to the stack
- `READ(i)` `[Register/Value]` `[Register]` : Read a value from the memory address to a register
- `POP` `[Register]` : Pop a value from the stack to a register

**__NOTE:__** Subtracting computes SRC1 - SRC2, not SRC2 - SRC1

---
## Example Programs:
<details>
<summary>Fibonacci Sequence</summary>

`input.txt`:
```assembly
iMOV 1, b
ADDi sp 10 sp

#start
ADD a b c
PUSH c
MOV b a
MOV c b
iJMP start
```

`output.txt (+ formatting)`:
```
0000 8201 0001 4bc7 000a 0a0a 3c80 0240
0281 9400 0005 0000 0000 0000 0000 0000
```
</details>

<details>
<summary>Prime Number Finder</summary>

`input.txt`:
```assembly
#start:
    iMOV 100 a;
    iMOV 210 sp;
    iMOV 2 b;

#removemultiples:
    ADDi c 100 c;
    iWRT 0xffff c;
    SUBi c 100 c;
    ADD b c c;
CMP c a;
iJLT removemultiples;

#nextnumber:
    ADDi b 101 b;
    READ b c;
    SUBi b 100 b;
CMPi c 0xffff;
iJEQ nextnumber;

#checkcondition:
    PUSH a;
    SHR a a;
    CMP a b;
    POP a;
iJLT 300;

MOV b c;
SHL c c;
iJMP removemultiples;
```

`output.txt (+ formatting)`:
```
0000 8200 0064 8207 00d2 8201 0002 4a82
0064 9c10 ffff 4c82 0064 0a52 1280 9800
0007 4a41 0065 1e0a 4c41 0064 5280 ffff
9600 0011 3c00 1000 1208 3e00 9800 012c
0242 0e82 9400 0007 0000 0000 0000 0000

0000 8200 0064 8207 00d2 8201 0002 4a82
0064 9c10 ffff 4c82 0064 0a52 1280 9800
0006 4a41 0065 1e0a 4c41 0064 5280 ffff
9600 0010 3c00 1000 1208 3e00 9800 012c
0242 0e82 9400 0006 0000 0000 0000 0000
```
</details>


## License
This project is licensed under the [MIT License](LICENSE).
