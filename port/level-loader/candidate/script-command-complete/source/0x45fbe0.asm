0045fbe0 f0402de9 push {r4, r5, r6, r7, lr}
0045fbe4 80409fe5 ldr r4, [pc, #0x80]
0045fbe8 80309fe5 ldr r3, [pc, #0x80]
0045fbec 24d04de2 sub sp, sp, #0x24
0045fbf0 04408fe0 add r4, pc, r4
0045fbf4 036094e7 ldr r6, [r4, r3]
0045fbf8 74309fe5 ldr r3, [pc, #0x74]
0045fbfc 04508de2 add r5, sp, #4
0045fc00 037094e7 ldr r7, [r4, r3]
0045fc04 003096e5 ldr r3, [r6]
0045fc08 0700a0e1 mov r0, r7
0045fc0c 1c308de5 str r3, [sp, #0x1c]
0045fc10 1c5ffbeb bl #0x337888
0045fc14 5c109fe5 ldr r1, [pc, #0x5c]
0045fc18 0d20a0e1 mov r2, sp
0045fc1c 0500a0e1 mov r0, r5
0045fc20 01108fe0 add r1, pc, r1
0045fc24 30d1faeb bl #0x3140ec
0045fc28 0510a0e1 mov r1, r5
0045fc2c 0700a0e1 mov r0, r7
0045fc30 945ffbeb bl #0x337a88
0045fc34 0500a0e1 mov r0, r5
0045fc38 85e1faeb bl #0x318254
0045fc3c 38309fe5 ldr r3, [pc, #0x38]
0045fc40 0010a0e3 mov r1, #0
0045fc44 033094e7 ldr r3, [r4, r3]
0045fc48 000093e5 ldr r0, [r3]
0045fc4c f030fceb bl #0x36c014
0045fc50 1c209de5 ldr r2, [sp, #0x1c]
0045fc54 003096e5 ldr r3, [r6]
0045fc58 030052e1 cmp r2, r3
0045fc5c 0100001a bne #0x45fc68
0045fc60 24d08de2 add sp, sp, #0x24
0045fc64 f080bde8 pop {r4, r5, r6, r7, pc}
0045fc68 a8b9faeb bl #0x30e310
0045fc6c a04e5300 subseq r4, r3, r0, lsr #29
0045fc70 ac400000 andeq r4, r0, ip, lsr #1
0045fc74 84080000 andeq r0, r0, r4, lsl #17
0045fc78 60d44600 subeq sp, r6, r0, ror #8
0045fc7c a40d0000 andeq r0, r0, r4, lsr #27
