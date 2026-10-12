0x459090 04e02de5 str lr, [sp, #-4]!
0x459094 0030a0e3 mov r3, #0
0x459098 0cd04de2 sub sp, sp, #0xc
0x45909c 00c090e5 ldr ip, [r0]
0x4590a0 0420a0e3 mov r2, #4
0x4590a4 0fe0a0e1 mov lr, pc
0x4590a8 18f09ce5 ldr pc, [ip, #0x18]
0x4590ac 74309fe5 ldr r3, [pc, #0x74]
0x4590b0 040050e3 cmp r0, #4
0x4590b4 03308fe0 add r3, pc, r3
0x4590b8 0a00000a beq #0x4590e8
0x4590bc 68209fe5 ldr r2, [pc, #0x68]
0x4590c0 022093e7 ldr r2, [r3, r2]
0x4590c4 002092e5 ldr r2, [r2]
0x4590c8 020052e3 cmp r2, #2
0x4590cc 0030a003 moveq r3, #0
0x4590e8 000051e3 cmp r1, #0
0x4590ec fbffff0a beq #0x4590e0
