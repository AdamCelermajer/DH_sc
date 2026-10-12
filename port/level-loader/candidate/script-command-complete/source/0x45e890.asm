0045e890 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045e894 50419fe5 ldr r4, [pc, #0x150]
0045e898 50519fe5 ldr r5, [pc, #0x150]
0045e89c 44d04de2 sub sp, sp, #0x44
0045e8a0 04408fe0 add r4, pc, r4
0045e8a4 053094e7 ldr r3, [r4, r5]
0045e8a8 007051e2 subs r7, r1, #0
0045e8ac 0060a0e1 mov r6, r0
0045e8b0 003093e5 ldr r3, [r3]
0045e8b4 0290a0e1 mov sb, r2
0045e8b8 3c308de5 str r3, [sp, #0x3c]
0045e8bc 0600000a beq #0x45e8dc
0045e8c0 053094e7 ldr r3, [r4, r5]
0045e8c4 3c209de5 ldr r2, [sp, #0x3c]
0045e8c8 003093e5 ldr r3, [r3]
0045e8cc 030052e1 cmp r2, r3
0045e8d0 4400001a bne #0x45e9e8
0045e8d4 44d08de2 add sp, sp, #0x44
0045e8d8 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045e8dc 10319fe5 ldr r3, [pc, #0x110]
0045e8e0 0c8090e5 ldr r8, [r0, #0xc]
0045e8e4 24a08de2 add sl, sp, #0x24
0045e8e8 03b094e7 ldr fp, [r4, r3]
0045e8ec 10308de2 add r3, sp, #0x10
0045e8f0 08308de5 str r3, [sp, #8]
0045e8f4 103080e2 add r3, r0, #0x10
0045e8f8 0b00a0e1 mov r0, fp
0045e8fc 0c308de5 str r3, [sp, #0xc]
0045e900 e063fbeb bl #0x337888
0045e904 ec109fe5 ldr r1, [pc, #0xec]
0045e908 20208de2 add r2, sp, #0x20
0045e90c 0a00a0e1 mov r0, sl
0045e910 01108fe0 add r1, pc, r1
0045e914 f4d5faeb bl #0x3140ec
0045e918 0a10a0e1 mov r1, sl
0045e91c 0b00a0e1 mov r0, fp
0045e920 5864fbeb bl #0x337a88
0045e924 0a00a0e1 mov r0, sl
0045e928 49e6faeb bl #0x318254
0045e92c c8109fe5 ldr r1, [pc, #0xc8]
0045e930 182098e5 ldr r2, [r8, #0x18]
0045e934 0930a0e1 mov r3, sb
0045e938 011094e7 ldr r1, [r4, r1]
0045e93c 08009de5 ldr r0, [sp, #8]
0045e940 381091e5 ldr r1, [r1, #0x38]
0045e944 00708de5 str r7, [sp]
0045e948 04708de5 str r7, [sp, #4]
0045e94c d3b0fbeb bl #0x34aca0
0045e950 08309de5 ldr r3, [sp, #8]
0045e954 10209de5 ldr r2, [sp, #0x10]
0045e958 081093e5 ldr r1, [r3, #8]
0045e95c 14309de5 ldr r3, [sp, #0x14]
0045e960 102086e5 str r2, [r6, #0x10]
0045e964 181086e5 str r1, [r6, #0x18]
0045e968 143086e5 str r3, [r6, #0x14]
0045e96c 0c009de5 ldr r0, [sp, #0xc]
0045e970 0710a0e1 mov r1, r7
0045e974 1185fbeb bl #0x33fdc0
0045e978 000050e3 cmp r0, #0
0045e97c cfffff0a beq #0x45e8c0
0045e980 0c009de5 ldr r0, [sp, #0xc]
0045e984 7285fbeb bl #0x33ff54
0045e988 000050e3 cmp r0, #0
0045e98c 10a098e5 ldr sl, [r8, #0x10]
0045e990 caffff0a beq #0x45e8c0
0045e994 64309fe5 ldr r3, [pc, #0x64]
0045e998 02108ae2 add r1, sl, #2
0045e99c 1460a0e3 mov r6, #0x14
0045e9a0 03c094e7 ldr ip, [r4, r3]
0045e9a4 089098e5 ldr sb, [r8, #8]
0045e9a8 08a08ae2 add sl, sl, #8
0045e9ac 00309ce5 ldr r3, [ip]
0045e9b0 4f0e80e2 add r0, r0, #0x4f0
0045e9b4 0c0080e2 add r0, r0, #0xc
0045e9b8 963123e0 mla r3, r6, r1, r3
0045e9bc 0720a0e1 mov r2, r7
0045e9c0 0ce093e5 ldr lr, [r3, #0xc]
0045e9c4 0130a0e3 mov r3, #1
0045e9c8 08908ee5 str sb, [lr, #8]
0045e9cc 00c09ce5 ldr ip, [ip]
0045e9d0 0ce098e5 ldr lr, [r8, #0xc]
0045e9d4 96ca26e0 mla r6, r6, sl, ip
0045e9d8 0cc096e5 ldr ip, [r6, #0xc]
0045e9dc 08e08ce5 str lr, [ip, #8]
0045e9e0 308cfdeb bl #0x3c1aa8
0045e9e4 b5ffffea b #0x45e8c0
0045e9e8 48befaeb bl #0x30e310
0045e9ec f0615300 ldrsheq r6, [r3], #-0x10
0045e9f0 ac400000 andeq r4, r0, ip, lsr #1
0045e9f4 84080000 andeq r0, r0, r4, lsl #17
0045e9f8 70e74600 subeq lr, r6, r0, ror r7
0045e9fc f4370000 strdeq r3, r4, [r0], -r4
0045ea00 7c3c0000 andeq r3, r0, ip, ror ip
