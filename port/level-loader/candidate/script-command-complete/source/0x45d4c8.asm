0045d4c8 f0452de9 push {r4, r5, r6, r7, r8, sl, lr}
0045d4cc a4409fe5 ldr r4, [pc, #0xa4]
0045d4d0 a4609fe5 ldr r6, [pc, #0xa4]
0045d4d4 a4109fe5 ldr r1, [pc, #0xa4]
0045d4d8 04408fe0 add r4, pc, r4
0045d4dc 063094e7 ldr r3, [r4, r6]
0045d4e0 018094e7 ldr r8, [r4, r1]
0045d4e4 24d04de2 sub sp, sp, #0x24
0045d4e8 003093e5 ldr r3, [r3]
0045d4ec 02a0a0e1 mov sl, r2
0045d4f0 04508de2 add r5, sp, #4
0045d4f4 1c308de5 str r3, [sp, #0x1c]
0045d4f8 0c7090e5 ldr r7, [r0, #0xc]
0045d4fc 0800a0e1 mov r0, r8
0045d500 e068fbeb bl #0x337888
0045d504 78109fe5 ldr r1, [pc, #0x78]
0045d508 0d20a0e1 mov r2, sp
0045d50c 0500a0e1 mov r0, r5
0045d510 01108fe0 add r1, pc, r1
0045d514 f4dafaeb bl #0x3140ec
0045d518 0510a0e1 mov r1, r5
0045d51c 0800a0e1 mov r0, r8
0045d520 5869fbeb bl #0x337a88
0045d524 0500a0e1 mov r0, r5
0045d528 49ebfaeb bl #0x318254
0045d52c 0830d7e5 ldrb r3, [r7, #8]
0045d530 100097e5 ldr r0, [r7, #0x10]
0045d534 000053e3 cmp r3, #0
0045d538 0a00001a bne #0x45d568
0045d53c 44309fe5 ldr r3, [pc, #0x44]
0045d540 032094e7 ldr r2, [r4, r3]
0045d544 0a10a0e1 mov r1, sl
0045d548 d6efffeb bl #0x4594a8
0045d54c 063094e7 ldr r3, [r4, r6]
0045d550 1c209de5 ldr r2, [sp, #0x1c]
0045d554 003093e5 ldr r3, [r3]
0045d558 030052e1 cmp r2, r3
0045d55c 0400001a bne #0x45d574
0045d560 24d08de2 add sp, sp, #0x24
0045d564 f085bde8 pop {r4, r5, r6, r7, r8, sl, pc}
0045d568 1c309fe5 ldr r3, [pc, #0x1c]
0045d56c 032094e7 ldr r2, [r4, r3]
0045d570 f3ffffea b #0x45d544
0045d574 65c3faeb bl #0x30e310
0045d578 b8755300 ldrheq r7, [r3], #-0x58
0045d57c ac400000 andeq r4, r0, ip, lsr #1
0045d580 84080000 andeq r0, r0, r4, lsl #17
0045d584 70fb4600 subeq pc, r6, r0, ror fp
0045d588 b0370000 strheq r3, [r0], -r0
0045d58c 680c0000 andeq r0, r0, r8, ror #24
