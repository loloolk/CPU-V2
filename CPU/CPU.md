Fixed Length Instruction Encoding
1 - Immediate SRC1, 1 - Immediate SRC2, 1 - Inverter, 4 - OPCODE, 3 - SRC1, 3 - SRC2, 3 - DST

Reg Codes:
0-7: a, b, c, d, e, f, bp, sp
COMPARATOR BITS:
less, equal, greater, not equal

OPCODES:

1. 0x00 - noop - No Operation
2. 0x01 - mov - Move data from SRC 1 to DST
3. 0x11 (INV) - Not SCR 1 and store in DST
4. 0x02 - And SCR 1 & SRC 2 and store in DST
5. 0x12 (INV) - Nand SCR 1 & SRC 2 and store in DST
6. 0x03 - Or SCR 1 & SRC 2 and store in DST
7. 0x13 (INV) - Nor SCR 1 & SRC 2 and store in DST
8. 0x04 - Xor SCR 1 & SRC 2 and store in DST
9. 0x14 (INV) - Xnor SCR 1 & SRC 2 and store in DST
10. 0x05 - Add SCR 1 & SRC 2 and store in DST
11. 0x06 - Sub SCR 1 & SRC 2 and store in DST
12. 0x07 - Shl SCR 1 and store in DST
13. 0x08 - Shr SCR 1 and store in DST
14. 0x09 - cmp - Compare SCR 1 and SRC 2 (result store in compare register)
15. 0x0a - jmp - Jump to address defined in SRC 1
16. 0x0b - je - Jump to address defined in SRC 1 if equality bit is set in compare register
17. 0x1b (INV) - jne - Jump to address defined in SRC 1 if equality bit is not set in compare register
18. 0x0c - jl - Jump to address defined in SRC 1 if less-than bit is set in compare register
19. 0x1c (INV) - jge - Jump to address defined in SRC 1 if less-than bit is not set in compare register
20. 0x0d - jg - Jump to address defined in SRC 1 if greater-than bit is set in compare register
21. 0x1d (INV) - jle - Jump to address defined in SRC 1 if greater-than bit is not set in compare register
22. 0x0e - wrt - Write the memory at address SRC 2, the data in SRC 1
23. 0x1e (INV) - push - Write mem at address in sp, the data in SRC 1, increment sp by 1
24. 0x0f - read - Read mem at address SRC 2, store in DST
25. 0x1f (INV) - pop - Decrement sp by 1, read mem at address in sp, store in DST. 

NOTE: Inverter bit can still be active on addition and subtraction operations, in which case it will invert the result.

Future Possible Improvements:
Floating Point Operations
Visual Interface
Signed support
Peripheral interface
