0045d868 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d86c d4409fe5 ldr r4, [pc, #0xd4]
0045d870 d4809fe5 ldr r8, [pc, #0xd4]
0045d874 d4109fe5 ldr r1, [pc, #0xd4]
0045d878 04408fe0 add r4, pc, r4
0045d87c 083094e7 ldr r3, [r4, r8]
0045d880 01a094e7 ldr sl, [r4, r1]
0045d884 38d04de2 sub sp, sp, #0x38
0045d888 003093e5 ldr r3, [r3]
0045d88c 0290a0e1 mov sb, r2
0045d890 1c508de2 add r5, sp, #0x1c
0045d894 34308de5 str r3, [sp, #0x34]
0045d898 0c7090e5 ldr r7, [r0, #0xc]
0045d89c 0a00a0e1 mov r0, sl
0045d8a0 f867fbeb bl #0x337888
0045d8a4 a8109fe5 ldr r1, [pc, #0xa8]
0045d8a8 18208de2 add r2, sp, #0x18
0045d8ac 0500a0e1 mov r0, r5
0045d8b0 01108fe0 add r1, pc, r1
0045d8b4 0cdafaeb bl #0x3140ec
0045d8b8 0510a0e1 mov r1, r5
0045d8bc 0a00a0e1 mov r0, sl
0045d8c0 7068fbeb bl #0x337a88
0045d8c4 0500a0e1 mov r0, r5
0045d8c8 61eafaeb bl #0x318254
0045d8cc 84109fe5 ldr r1, [pc, #0x84]
0045d8d0 0c608de2 add r6, sp, #0xc
0045d8d4 102097e5 ldr r2, [r7, #0x10]
0045d8d8 011094e7 ldr r1, [r4, r1]
0045d8dc 0050a0e3 mov r5, #0
0045d8e0 0930a0e1 mov r3, sb
0045d8e4 381091e5 ldr r1, [r1, #0x38]
0045d8e8 0600a0e1 mov r0, r6
0045d8ec 00508de5 str r5, [sp]
0045d8f0 04508de5 str r5, [sp, #4]
0045d8f4 e9b4fbeb bl #0x34aca0
0045d8f8 0600a0e1 mov r0, r6
0045d8fc 0510a0e1 mov r1, r5
0045d900 2e89fbeb bl #0x33fdc0
0045d904 000050e3 cmp r0, #0
0045d908 0a00001a bne #0x45d938
0045d90c 0830d7e5 ldrb r3, [r7, #8]
0045d910 000053e3 cmp r3, #0
0045d914 0130a013 movne r3, #1
0045d918 8430c015 strbne r3, [r0, #0x84]
0045d91c 083094e7 ldr r3, [r4, r8]
0045d920 34209de5 ldr r2, [sp, #0x34]
0045d924 003093e5 ldr r3, [r3]
0045d928 030052e1 cmp r2, r3
0045d92c 0400001a bne #0x45d944
0045d930 38d08de2 add sp, sp, #0x38
0045d934 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045d938 0600a0e1 mov r0, r6
0045d93c 8489fbeb bl #0x33ff54
0045d940 f1ffffea b #0x45d90c
0045d944 71c2faeb bl #0x30e310
0045d948 18725300 subseq r7, r3, r8, lsl r2
0045d94c ac400000 andeq r4, r0, ip, lsr #1
0045d950 84080000 andeq r0, r0, r4, lsl #17
