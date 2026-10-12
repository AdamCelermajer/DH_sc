0045d2d4 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d2d8 d8409fe5 ldr r4, [pc, #0xd8]
0045d2dc d8609fe5 ldr r6, [pc, #0xd8]
0045d2e0 d8109fe5 ldr r1, [pc, #0xd8]
0045d2e4 04408fe0 add r4, pc, r4
0045d2e8 063094e7 ldr r3, [r4, r6]
0045d2ec 017094e7 ldr r7, [r4, r1]
0045d2f0 38d04de2 sub sp, sp, #0x38
0045d2f4 003093e5 ldr r3, [r3]
0045d2f8 0080a0e1 mov r8, r0
0045d2fc 0700a0e1 mov r0, r7
0045d300 34308de5 str r3, [sp, #0x34]
0045d304 0290a0e1 mov sb, r2
0045d308 0ca098e5 ldr sl, [r8, #0xc]
0045d30c 5d69fbeb bl #0x337888
0045d310 ac109fe5 ldr r1, [pc, #0xac]
0045d314 1c508de2 add r5, sp, #0x1c
0045d318 18208de2 add r2, sp, #0x18
0045d31c 01108fe0 add r1, pc, r1
0045d320 0500a0e1 mov r0, r5
0045d324 70dbfaeb bl #0x3140ec
0045d328 0510a0e1 mov r1, r5
0045d32c 0700a0e1 mov r0, r7
0045d330 d469fbeb bl #0x337a88
0045d334 0500a0e1 mov r0, r5
0045d338 c5ebfaeb bl #0x318254
0045d33c 84309fe5 ldr r3, [pc, #0x84]
0045d340 0c708de2 add r7, sp, #0xc
0045d344 0c209ae5 ldr r2, [sl, #0xc]
0045d348 031094e7 ldr r1, [r4, r3]
0045d34c 0050a0e3 mov r5, #0
0045d350 0930a0e1 mov r3, sb
0045d354 381091e5 ldr r1, [r1, #0x38]
0045d358 0700a0e1 mov r0, r7
0045d35c 00508de5 str r5, [sp]
0045d360 04508de5 str r5, [sp, #4]
0045d364 4db6fbeb bl #0x34aca0
0045d368 0700a0e1 mov r0, r7
0045d36c 0510a0e1 mov r1, r5
0045d370 928afbeb bl #0x33fdc0
0045d374 000050e3 cmp r0, #0
0045d378 0400000a beq #0x45d390
0045d37c f43090e5 ldr r3, [r0, #0xf4]
0045d380 110053e3 cmp r3, #0x11
0045d384 10008805 streq r0, [r8, #0x10]
0045d388 0c54c005 strbeq r5, [r0, #0x40c]
0045d38c 0100000a beq #0x45d398
0045d390 0030a0e3 mov r3, #0
0045d394 103088e5 str r3, [r8, #0x10]
0045d398 063094e7 ldr r3, [r4, r6]
0045d39c 34209de5 ldr r2, [sp, #0x34]
0045d3a0 003093e5 ldr r3, [r3]
0045d3a4 030052e1 cmp r2, r3
0045d3a8 0100001a bne #0x45d3b4
0045d3ac 38d08de2 add sp, sp, #0x38
0045d3b0 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045d3b4 d5c3faeb bl #0x30e310
0045d3b8 ac775300 subseq r7, r3, ip, lsr #15
0045d3bc ac400000 andeq r4, r0, ip, lsr #1
0045d3c0 84080000 andeq r0, r0, r4, lsl #17
0045d3c4 64fd4600 subeq pc, r6, r4, ror #26
0045d3c8 f4370000 strdeq r3, r4, [r0], -r4
