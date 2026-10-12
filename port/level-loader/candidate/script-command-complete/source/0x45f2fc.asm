0045f2fc f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045f300 e4409fe5 ldr r4, [pc, #0xe4]
0045f304 e4809fe5 ldr r8, [pc, #0xe4]
0045f308 e4109fe5 ldr r1, [pc, #0xe4]
0045f30c 04408fe0 add r4, pc, r4
0045f310 083094e7 ldr r3, [r4, r8]
0045f314 01a094e7 ldr sl, [r4, r1]
0045f318 38d04de2 sub sp, sp, #0x38
0045f31c 003093e5 ldr r3, [r3]
0045f320 0290a0e1 mov sb, r2
0045f324 1c508de2 add r5, sp, #0x1c
0045f328 34308de5 str r3, [sp, #0x34]
0045f32c 0c7090e5 ldr r7, [r0, #0xc]
0045f330 0a00a0e1 mov r0, sl
0045f334 5361fbeb bl #0x337888
0045f338 b8109fe5 ldr r1, [pc, #0xb8]
0045f33c 18208de2 add r2, sp, #0x18
0045f340 0500a0e1 mov r0, r5
0045f344 01108fe0 add r1, pc, r1
0045f348 67d3faeb bl #0x3140ec
0045f34c 0510a0e1 mov r1, r5
0045f350 0a00a0e1 mov r0, sl
0045f354 cb61fbeb bl #0x337a88
0045f358 0500a0e1 mov r0, r5
0045f35c bce3faeb bl #0x318254
0045f360 94109fe5 ldr r1, [pc, #0x94]
0045f364 0c608de2 add r6, sp, #0xc
0045f368 142097e5 ldr r2, [r7, #0x14]
0045f36c 011094e7 ldr r1, [r4, r1]
0045f370 0050a0e3 mov r5, #0
0045f374 0930a0e1 mov r3, sb
0045f378 381091e5 ldr r1, [r1, #0x38]
0045f37c 0600a0e1 mov r0, r6
0045f380 00508de5 str r5, [sp]
0045f384 04508de5 str r5, [sp, #4]
0045f388 44aefbeb bl #0x34aca0
0045f38c 0510a0e1 mov r1, r5
0045f390 0600a0e1 mov r0, r6
0045f394 8982fbeb bl #0x33fdc0
0045f398 005050e2 subs r5, r0, #0
0045f39c 0a00000a beq #0x45f3cc
0045f3a0 003095e5 ldr r3, [r5]
0045f3a4 0fe0a0e1 mov lr, pc
0045f3a8 24f093e5 ldr pc, [r3, #0x24]
0045f3ac 000050e3 cmp r0, #0
0045f3b0 0500000a beq #0x45f3cc
0045f3b4 e40395e5 ldr r0, [r5, #0x3e4]
0045f3b8 182097e5 ldr r2, [r7, #0x18]
0045f3bc 0c1097e5 ldr r1, [r7, #0xc]
0045f3c0 000050e3 cmp r0, #0
0045f3c4 0000000a beq #0x45f3cc
0045f3c8 7079fceb bl #0x37d990
0045f3cc 083094e7 ldr r3, [r4, r8]
0045f3d0 34209de5 ldr r2, [sp, #0x34]
0045f3d4 003093e5 ldr r3, [r3]
0045f3d8 030052e1 cmp r2, r3
0045f3dc 0100001a bne #0x45f3e8
0045f3e0 38d08de2 add sp, sp, #0x38
0045f3e4 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045f3e8 c8bbfaeb bl #0x30e310
0045f3ec 84575300 subseq r5, r3, r4, lsl #15
0045f3f0 ac400000 andeq r4, r0, ip, lsr #1
0045f3f4 84080000 andeq r0, r0, r4, lsl #17
0045f3f8 3cdd4600 subeq sp, r6, ip, lsr sp
0045f3fc f4370000 strdeq r3, r4, [r0], -r4
