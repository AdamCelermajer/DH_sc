0045d590 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d594 9c409fe5 ldr r4, [pc, #0x9c]
0045d598 9c309fe5 ldr r3, [pc, #0x9c]
0045d59c 20d04de2 sub sp, sp, #0x20
0045d5a0 04408fe0 add r4, pc, r4
0045d5a4 036094e7 ldr r6, [r4, r3]
0045d5a8 90309fe5 ldr r3, [pc, #0x90]
0045d5ac 01a0a0e1 mov sl, r1
0045d5b0 0280a0e1 mov r8, r2
0045d5b4 037094e7 ldr r7, [r4, r3]
0045d5b8 003096e5 ldr r3, [r6]
0045d5bc 04508de2 add r5, sp, #4
0045d5c0 01a02ae2 eor sl, sl, #1
0045d5c4 1c308de5 str r3, [sp, #0x1c]
0045d5c8 0c9090e5 ldr sb, [r0, #0xc]
0045d5cc 0700a0e1 mov r0, r7
0045d5d0 ac68fbeb bl #0x337888
0045d5d4 68109fe5 ldr r1, [pc, #0x68]
0045d5d8 0d20a0e1 mov r2, sp
0045d5dc 0500a0e1 mov r0, r5
0045d5e0 01108fe0 add r1, pc, r1
0045d5e4 c0dafaeb bl #0x3140ec
0045d5e8 0510a0e1 mov r1, r5
0045d5ec 0700a0e1 mov r0, r7
0045d5f0 2469fbeb bl #0x337a88
0045d5f4 0500a0e1 mov r0, r5
0045d5f8 15ebfaeb bl #0x318254
0045d5fc 44309fe5 ldr r3, [pc, #0x44]
0045d600 44209fe5 ldr r2, [pc, #0x44]
0045d604 0810a0e1 mov r1, r8
0045d608 033094e7 ldr r3, [r4, r3]
0045d60c 022094e7 ldr r2, [r4, r2]
0045d610 00a0c3e5 strb sl, [r3]
0045d614 0c0099e5 ldr r0, [sb, #0xc]
0045d618 a2efffeb bl #0x4594a8
0045d61c 1c209de5 ldr r2, [sp, #0x1c]
0045d620 003096e5 ldr r3, [r6]
0045d624 030052e1 cmp r2, r3
0045d628 0100001a bne #0x45d634
0045d62c 20d08de2 add sp, sp, #0x20
0045d630 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045d634 35c3faeb bl #0x30e310
0045d638 f0745300 ldrsheq r7, [r3], #-0x40
0045d63c ac400000 andeq r4, r0, ip, lsr #1
0045d640 84080000 andeq r0, r0, r4, lsl #17
0045d644 a0fa4600 subeq pc, r6, r0, lsr #21
0045d648 f8440000 strdeq r4, r5, [r0], -r8
0045d64c 44140000 andeq r1, r0, r4, asr #8
