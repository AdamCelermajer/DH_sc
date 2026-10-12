0045e4b8 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045e4bc f0409fe5 ldr r4, [pc, #0xf0]
0045e4c0 f0909fe5 ldr sb, [pc, #0xf0]
0045e4c4 f0109fe5 ldr r1, [pc, #0xf0]
0045e4c8 04408fe0 add r4, pc, r4
0045e4cc 093094e7 ldr r3, [r4, sb]
0045e4d0 016094e7 ldr r6, [r4, r1]
0045e4d4 40d04de2 sub sp, sp, #0x40
0045e4d8 003093e5 ldr r3, [r3]
0045e4dc 02a0a0e1 mov sl, r2
0045e4e0 24508de2 add r5, sp, #0x24
0045e4e4 3c308de5 str r3, [sp, #0x3c]
0045e4e8 0c7090e5 ldr r7, [r0, #0xc]
0045e4ec 0600a0e1 mov r0, r6
0045e4f0 e464fbeb bl #0x337888
0045e4f4 c4109fe5 ldr r1, [pc, #0xc4]
0045e4f8 20208de2 add r2, sp, #0x20
0045e4fc 0500a0e1 mov r0, r5
0045e500 01108fe0 add r1, pc, r1
0045e504 f8d6faeb bl #0x3140ec
0045e508 0510a0e1 mov r1, r5
0045e50c 0600a0e1 mov r0, r6
0045e510 5c65fbeb bl #0x337a88
0045e514 0500a0e1 mov r0, r5
0045e518 4de7faeb bl #0x318254
0045e51c a0309fe5 ldr r3, [pc, #0xa0]
0045e520 14808de2 add r8, sp, #0x14
0045e524 1c2097e5 ldr r2, [r7, #0x1c]
0045e528 036094e7 ldr r6, [r4, r3]
0045e52c 0050a0e3 mov r5, #0
0045e530 0a30a0e1 mov r3, sl
0045e534 381096e5 ldr r1, [r6, #0x38]
0045e538 0800a0e1 mov r0, r8
0045e53c 00508de5 str r5, [sp]
0045e540 04508de5 str r5, [sp, #4]
0045e544 d5b1fbeb bl #0x34aca0
0045e548 0800a0e1 mov r0, r8
0045e54c 8086fbeb bl #0x33ff54
0045e550 381096e5 ldr r1, [r6, #0x38]
0045e554 08608de2 add r6, sp, #8
0045e558 102097e5 ldr r2, [r7, #0x10]
0045e55c 0080a0e1 mov r8, r0
0045e560 0a30a0e1 mov r3, sl
0045e564 0600a0e1 mov r0, r6
0045e568 04508de5 str r5, [sp, #4]
0045e56c 00508de5 str r5, [sp]
0045e570 cab1fbeb bl #0x34aca0
0045e574 0600a0e1 mov r0, r6
0045e578 7586fbeb bl #0x33ff54
0045e57c 050050e1 cmp r0, r5
0045e580 05005811 cmpne r8, r5
0045e584 0010a0e1 mov r1, r0
0045e588 0100000a beq #0x45e594
0045e58c f20f88e2 add r0, r8, #0x3c8
0045e590 fad9fdeb bl #0x3d4d80
0045e594 093094e7 ldr r3, [r4, sb]
0045e598 3c209de5 ldr r2, [sp, #0x3c]
0045e59c 003093e5 ldr r3, [r3]
0045e5a0 030052e1 cmp r2, r3
0045e5a4 0100001a bne #0x45e5b0
0045e5a8 40d08de2 add sp, sp, #0x40
0045e5ac f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045e5b0 56bffaeb bl #0x30e310
0045e5b4 c8655300 subseq r6, r3, r8, asr #11
0045e5b8 ac400000 andeq r4, r0, ip, lsr #1
0045e5bc 84080000 andeq r0, r0, r4, lsl #17
0045e5c0 80eb4600 subeq lr, r6, r0, lsl #23
0045e5c4 f4370000 strdeq r3, r4, [r0], -r4
