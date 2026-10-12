0x4db94c 04e02de5 str lr, [sp, #-4]!
0x4db950 0030a0e3 mov r3, #0
0x4db954 0cd04de2 sub sp, sp, #0xc
0x4db958 00c090e5 ldr ip, [r0]
0x4db95c 0420a0e3 mov r2, #4
0x4db960 0fe0a0e1 mov lr, pc
0x4db964 18f09ce5 ldr pc, [ip, #0x18]
0x4db968 74309fe5 ldr r3, [pc, #0x74]
0x4db96c 040050e3 cmp r0, #4
0x4db970 03308fe0 add r3, pc, r3
0x4db974 0a00000a beq #0x4db9a4
0x4db978 68209fe5 ldr r2, [pc, #0x68]
0x4db97c 022093e7 ldr r2, [r3, r2]
0x4db980 002092e5 ldr r2, [r2]
0x4db984 020052e3 cmp r2, #2
0x4db988 0030a003 moveq r3, #0
0x4db98c 00308305 streq r3, [r3]
0x4db990 0100000a beq #0x4db99c
0x4db994 010052e3 cmp r2, #1
0x4db998 0400000a beq #0x4db9b0
0x4db99c 0cd08de2 add sp, sp, #0xc
0x4db9a0 0080bde8 ldm sp!, {pc}
0x4db9a4 000051e3 cmp r1, #0
0x4db9a8 fbffff0a beq #0x4db99c
0x4db9ac f1ffffea b #0x4db978
0x4db9b0 34009fe5 ldr r0, [pc, #0x34]
0x4db9b4 34109fe5 ldr r1, [pc, #0x34]
0x4db9b8 34209fe5 ldr r2, [pc, #0x34]
0x4db9bc 000093e7 ldr r0, [r3, r0]
0x4db9c0 30309fe5 ldr r3, [pc, #0x30]
0x4db9c4 50c0a0e3 mov ip, #0x50
0x4db9c8 01108fe0 add r1, pc, r1
0x4db9cc 02208fe0 add r2, pc, r2
0x4db9d0 03308fe0 add r3, pc, r3
0x4db9d4 a80080e2 add r0, r0, #0xa8
0x4db9d8 00c08de5 str ip, [sp]
0x4db9dc 88c9f8eb bl #0x30e004
0x4db9e0 edffffea b #0x4db99c
