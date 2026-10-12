0045fe2c f0412de9 push {r4, r5, r6, r7, r8, lr}
0045fe30 b0409fe5 ldr r4, [pc, #0xb0]
0045fe34 b0709fe5 ldr r7, [pc, #0xb0]
0045fe38 b0209fe5 ldr r2, [pc, #0xb0]
0045fe3c 04408fe0 add r4, pc, r4
0045fe40 073094e7 ldr r3, [r4, r7]
0045fe44 028094e7 ldr r8, [r4, r2]
0045fe48 20d04de2 sub sp, sp, #0x20
0045fe4c 003093e5 ldr r3, [r3]
0045fe50 04508de2 add r5, sp, #4
0045fe54 1c308de5 str r3, [sp, #0x1c]
0045fe58 0c6090e5 ldr r6, [r0, #0xc]
0045fe5c 0800a0e1 mov r0, r8
0045fe60 885efbeb bl #0x337888
0045fe64 88109fe5 ldr r1, [pc, #0x88]
0045fe68 0d20a0e1 mov r2, sp
0045fe6c 0500a0e1 mov r0, r5
0045fe70 01108fe0 add r1, pc, r1
0045fe74 9cd0faeb bl #0x3140ec
0045fe78 0510a0e1 mov r1, r5
0045fe7c 0800a0e1 mov r0, r8
0045fe80 005ffbeb bl #0x337a88
0045fe84 0500a0e1 mov r0, r5
0045fe88 f1e0faeb bl #0x318254
0045fe8c 0c30d6e5 ldrb r3, [r6, #0xc]
0045fe90 000053e3 cmp r3, #0
0045fe94 0c00001a bne #0x45fecc
0045fe98 58309fe5 ldr r3, [pc, #0x58]
0045fe9c 082096e5 ldr r2, [r6, #8]
0045fea0 101096e5 ldr r1, [r6, #0x10]
0045fea4 033094e7 ldr r3, [r4, r3]
0045fea8 000093e5 ldr r0, [r3]
0045feac 4e28fceb bl #0x369fec
0045feb0 073094e7 ldr r3, [r4, r7]
0045feb4 1c209de5 ldr r2, [sp, #0x1c]
0045feb8 003093e5 ldr r3, [r3]
0045febc 030052e1 cmp r2, r3
0045fec0 0700001a bne #0x45fee4
0045fec4 20d08de2 add sp, sp, #0x20
0045fec8 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045fecc 24309fe5 ldr r3, [pc, #0x24]
0045fed0 081096e5 ldr r1, [r6, #8]
0045fed4 033094e7 ldr r3, [r4, r3]
0045fed8 000093e5 ldr r0, [r3]
0045fedc af28fceb bl #0x36a1a0
0045fee0 f2ffffea b #0x45feb0
0045fee4 09b9faeb bl #0x30e310
0045fee8 544c5300 subseq r4, r3, r4, asr ip
0045feec ac400000 andeq r4, r0, ip, lsr #1
0045fef0 84080000 andeq r0, r0, r4, lsl #17
0045fef4 10d24600 subeq sp, r6, r0, lsl r2
0045fef8 a40d0000 andeq r0, r0, r4, lsr #27
