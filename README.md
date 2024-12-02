# CPU

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Description:
A 16-bit CPU, made with logisim evolution. This contains the CPU's Circuitry in `CPU`, an assembler for the CPU (from assembly to machine code) in `Assembler` and a Virtual Machine for the CPU in `VM`.

---

## Quick Start (CPU Simulation):
1) Download [logisim evolution](https://github.com/logisim-evolution/logisim-evolution)
2) Open `CPU/CPU.circ` in logisim evolution
3) Open `Assembler/input.txt` and put in your code
4) Run the `Assembler/main.c` file
5) Copy the `Assembler/outut.txt` into the Program Input chip
6) Run the CPU

---

## ASSEMBLER INSTRUCTIONS:
Registers: a, b, c, d, e, f, bp, sp

- `NOOP` : No operation
- `MOV(i)` `[Register/Value]` `[Register]` : Move a register/value to a register
- `NOT(i)` `[Register/Value]` `[Register]` : Negate a register
- `AND(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise AND
- `NAND(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise NAND
- `OR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise OR
- `NOR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise NOR
- `XOR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise XOR
- `XNOR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Bitwise XNOR
- `ADD(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Add a register/value to a register
- `SUB(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Subtract a register/value from a register **__*__**
- `SHL(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Shift left
- `SHR(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Shift right
- `CMP(i)` `[Register/Value]` `[Register/Value]` `[Register]` : Compare two registers
- `JMP(i)` `[Register/Value]` : Jump to a register/value
- `JEQ(i)` `[Register/Value]` : Jump if the equality flag is set in the previous CMP instruction
- `JNE(i)` `[Register/Value]` : Jump if the equality flag is not set in the previous CMP instruction
- `JGT(i)` `[Register/Value]` : Jump if the greater flag is set in the previous CMP instruction
- `JGE(i)` `[Register/Value]` : Jump if the greater or equal flag is set in the previous CMP instruction
- `JLT(i)` `[Register/Value]` : Jump if the less flag is set in the previous CMP instruction
- `JLE(i)` `[Register/Value]` : Jump if the less or equal flag is set in the previous CMP instruction
- `WRT(i)` `[Register/Value]` `[Register]` : Write a register/value to the memory
- `PUSH(i)` `[Register/Value]` : Push a register/value to the stack
- `READ(i)` `[Register]` : Read a value from the memory to a register
- `POP(i)` `[Register]` : Pop a value from the stack to a register

**__NOTE:__** Subtracting computes SRC1 - SRC2, not SRC2 - SRC1

---
## Example Programs:
<details>
<summary>Fibonacci Sequence</summary>

`input.txt`:
```assembly
iMOV 1, b

#start
ADD a b c
PUSH c
MOV b a
MOV c b
iJMP start
```

`output.txt (+ formatting)`:
```
0000 8201 0001 0a0a 3c80 0240 0281 9400
0003 0000 0000 0000 0000 0000 0000 0000
```
</details>

<details>
<summary>Prime Number Finder</summary>

`input.txt`:
```assembly
#start
iMOV 100 a
MOV a sp
iMOV 2 b

#checkcondition
PUSH a
SHR a a
CMP a b
POP a
iJLT end

MOV b c
SHL c c

#loop
iWRT 65535 c
ADD b c c
CMP c a
iJLT loop

ADDi b 1 b
iJMP checkcondition

#end
```

`output.txt (+ formatting)`:
```
0000 8200 0064 0207 8201 0002 3c00 1000
1208 3e00 9800 0018 0242 0e82 9c10 ffff
0a52 1280 9800 000e 4a41 0001 9400 0006
```
</details>


## License
This project is licensed under the [MIT License](LICENSE).