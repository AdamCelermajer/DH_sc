0045d3cc f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d3d0 dc409fe5 ldr r4, [pc, #0xdc]
0045d3d4 dc509fe5 ldr r5, [pc, #0xdc]
0045d3d8 dc109fe5 ldr r1, [pc, #0xdc]
0045d3dc 04408fe0 add r4, pc, r4
0045d3e0 053094e7 ldr r3, [r4, r5]
0045d3e4 017094e7 ldr r7, [r4, r1]
0045d3e8 38d04de2 sub sp, sp, #0x38
0045d3ec 003093e5 ldr r3, [r3]
0045d3f0 0080a0e1 mov r8, r0
0045d3f4 0700a0e1 mov r0, r7
0045d3f8 34308de5 str r3, [sp, #0x34]
0045d3fc 0290a0e1 mov sb, r2
0045d400 0ca098e5 ldr sl, [r8, #0xc]
0045d404 1f69fbeb bl #0x337888
0045d408 b0109fe5 ldr r1, [pc, #0xb0]
0045d40c 1c608de2 add r6, sp, #0x1c
0045d410 18208de2 add r2, sp, #0x18
0045d414 01108fe0 add r1, pc, r1
0045d418 0600a0e1 mov r0, r6
0045d41c 32dbfaeb bl #0x3140ec
0045d420 0610a0e1 mov r1, r6
0045d424 0700a0e1 mov r0, r7
0045d428 9669fbeb bl #0x337a88
0045d42c 0600a0e1 mov r0, r6
0045d430 87ebfaeb bl #0x318254
0045d434 88309fe5 ldr r3, [pc, #0x88]
0045d438 0c708de2 add r7, sp, #0xc
0045d43c 0c209ae5 ldr r2, [sl, #0xc]
0045d440 031094e7 ldr r1, [r4, r3]
0045d444 0060a0e3 mov r6, #0
0045d448 0930a0e1 mov r3, sb
0045d44c 381091e5 ldr r1, [r1, #0x38]
0045d450 0700a0e1 mov r0, r7
0045d454 00608de5 str r6, [sp]
0045d458 04608de5 str r6, [sp, #4]
0045d45c 0fb6fbeb bl #0x34aca0
0045d460 0700a0e1 mov r0, r7
0045d464 0610a0e1 mov r1, r6
0045d468 548afbeb bl #0x33fdc0
0045d46c 000050e3 cmp r0, #0
0045d470 0500000a beq #0x45d48c
0045d474 f43090e5 ldr r3, [r0, #0xf4]
0045d478 110053e3 cmp r3, #0x11
0045d47c 0130a003 moveq r3, #1
0045d480 10008805 streq r0, [r8, #0x10]
0045d484 0c34c005 strbeq r3, [r0, #0x40c]
0045d488 0100000a beq #0x45d494
0045d48c 0030a0e3 mov r3, #0
0045d490 103088e5 str r3, [r8, #0x10]
0045d494 053094e7 ldr r3, [r4, r5]
0045d498 34209de5 ldr r2, [sp, #0x34]
0045d49c 003093e5 ldr r3, [r3]
0045d4a0 030052e1 cmp r2, r3
0045d4a4 0100001a bne #0x45d4b0
0045d4a8 38d08de2 add sp, sp, #0x38
0045d4ac f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045d4b0 96c3faeb bl #0x30e310
0045d4b4 b4765300 ldrheq r7, [r3], #-0x64
0045d4b8 ac400000 andeq r4, r0, ip, lsr #1
0045d4bc 84080000 andeq r0, r0, r4, lsl #17
0045d4c0 6cfc4600 subeq pc, r6, ip, ror #24
0045d4c4 f4370000 strdeq r3, r4, [r0], -r4
