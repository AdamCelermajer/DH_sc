0045fc80 f0402de9 push {r4, r5, r6, r7, lr}
0045fc84 80409fe5 ldr r4, [pc, #0x80]
0045fc88 80309fe5 ldr r3, [pc, #0x80]
0045fc8c 24d04de2 sub sp, sp, #0x24
0045fc90 04408fe0 add r4, pc, r4
0045fc94 036094e7 ldr r6, [r4, r3]
0045fc98 74309fe5 ldr r3, [pc, #0x74]
0045fc9c 04508de2 add r5, sp, #4
0045fca0 037094e7 ldr r7, [r4, r3]
0045fca4 003096e5 ldr r3, [r6]
0045fca8 0700a0e1 mov r0, r7
0045fcac 1c308de5 str r3, [sp, #0x1c]
0045fcb0 f45efbeb bl #0x337888
0045fcb4 5c109fe5 ldr r1, [pc, #0x5c]
0045fcb8 0d20a0e1 mov r2, sp
0045fcbc 0500a0e1 mov r0, r5
0045fcc0 01108fe0 add r1, pc, r1
0045fcc4 08d1faeb bl #0x3140ec
0045fcc8 0510a0e1 mov r1, r5
0045fccc 0700a0e1 mov r0, r7
0045fcd0 6c5ffbeb bl #0x337a88
0045fcd4 0500a0e1 mov r0, r5
0045fcd8 5de1faeb bl #0x318254
0045fcdc 38309fe5 ldr r3, [pc, #0x38]
0045fce0 0110a0e3 mov r1, #1
0045fce4 033094e7 ldr r3, [r4, r3]
0045fce8 000093e5 ldr r0, [r3]
0045fcec c830fceb bl #0x36c014
0045fcf0 1c209de5 ldr r2, [sp, #0x1c]
0045fcf4 003096e5 ldr r3, [r6]
0045fcf8 030052e1 cmp r2, r3
0045fcfc 0100001a bne #0x45fd08
0045fd00 24d08de2 add sp, sp, #0x24
0045fd04 f080bde8 pop {r4, r5, r6, r7, pc}
0045fd08 80b9faeb bl #0x30e310
0045fd0c 004e5300 subseq r4, r3, r0, lsl #28
0045fd10 ac400000 andeq r4, r0, ip, lsr #1
0045fd14 84080000 andeq r0, r0, r4, lsl #17
0045fd18 c0d34600 subeq sp, r6, r0, asr #7
0045fd1c a40d0000 andeq r0, r0, r4, lsr #27
