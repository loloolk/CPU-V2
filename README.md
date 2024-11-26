# CPU

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](https://opensource.org/licenses/MIT)

## Description:
A 16-bit CPU, made with logisim evolution. This contains the CPU's Circuitry in `CPU`, an assembler for the CPU (from assembly to machine code) in `Assembler` and a Virtual Machine for the CPU in `VM`.

---

## Quick Start (CPU Simulation):
1) Download [logisim evolution](https://sourceforge.net/projects/logisimevolution/)
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
- `AND(i)` `[Register/Value]` `[Register]` `[Register]` : Bitwise AND
- `NAND(i)` `[Register/Value]` `[Register]` `[Register]` : Bitwise NAND
- `OR(i)` `[Register/Value]` `[Register]` `[Register]` : Bitwise OR
- `NOR(i)` `[Register/Value]` `[Register]` `[Register]` : Bitwise NOR
- `XOR(i)` `[Register/Value]` `[Register]` `[Register]` : Bitwise XOR
- `XNOR(i)` `[Register/Value]` `[Register]` `[Register]` : Bitwise XNOR
- `ADD(i)` `[Register/Value]` `[Register]` `[Register]` : Add a register/value to a register
- `SUB(i)` `[Register/Value]` `[Register]` `[Register]` : Subtract a register/value from a register **__*__**
- `SHL(i)` `[Register/Value]` `[Register]` `[Register]` : Shift left
- `SHR(i)` `[Register/Value]` `[Register]` `[Register]` : Shift right
- `CMP(i)` `[Register/Value]` `[Register]` `[Register]` : Compare two registers
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

**__*NOTE__**: subtracks the register value from the immediate value, not the other way around

---
## Example Programs:
<details>
<summary>Fibonacci Sequence</summary>

`input.txt`:
```assembly
MOVi 0 a
MOVi 1 b

#loop
ADD a b c
PUSH c
MOV b a
MOV c b
JMPi loop
```

`output.txt (+ formatting)`:
```
0000 8400 0000 8402 0001 1414 7900 0480
0502 a800 0005 0000 0000 0000 0000 0000
```
</details>

<details>
<summary>Prime Number Finder</summary>

`input.txt`:
```assembly
#start
MOVi 100 a
MOV a sp
MOVi 2 b

#checkcondition
PUSH a
SHR a a
CMP a b
POP a
JLTi end

MOV b c
SHL c c

#loop
WRTi 65535 c
ADD b c c
CMP c a
JLTi loop

ADDi 1 b b
JMPi checkcondition

#end
```

`output.txt (+ formatting)`:
```
0000 8400 0064 040e 8402 0002 7800 2000
2410 7c00 b000 0018 0484 1d04 b820 ffff
14a4 2500 b000 000e 9412 0001 a800 0006
```
</details>


## License
This project is licensed under the [MIT License](LICENSE).