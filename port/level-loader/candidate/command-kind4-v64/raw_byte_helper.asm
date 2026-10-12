0x4db89c 04e02de5 str lr, [sp, #-4]!
0x4db8a0 0030a0e3 mov r3, #0
0x4db8a4 0cd04de2 sub sp, sp, #0xc
0x4db8a8 00c090e5 ldr ip, [r0]
0x4db8ac 0120a0e3 mov r2, #1
0x4db8b0 0fe0a0e1 mov lr, pc
0x4db8b4 18f09ce5 ldr pc, [ip, #0x18]
0x4db8b8 74309fe5 ldr r3, [pc, #0x74]
0x4db8bc 010050e3 cmp r0, #1
0x4db8c0 03308fe0 add r3, pc, r3
0x4db8c4 0a00000a beq #0x4db8f4
0x4db8c8 68209fe5 ldr r2, [pc, #0x68]
0x4db8cc 022093e7 ldr r2, [r3, r2]
0x4db8d0 002092e5 ldr r2, [r2]
0x4db8d4 020052e3 cmp r2, #2
0x4db8d8 0030a003 moveq r3, #0
0x4db8f4 000051e3 cmp r1, #0
0x4db8f8 fbffff0a beq #0x4db8ec
