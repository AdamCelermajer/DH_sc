0045e374 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045e378 24419fe5 ldr r4, [pc, #0x124]
0045e37c 24919fe5 ldr sb, [pc, #0x124]
0045e380 24119fe5 ldr r1, [pc, #0x124]
0045e384 04408fe0 add r4, pc, r4
0045e388 093094e7 ldr r3, [r4, sb]
0045e38c 016094e7 ldr r6, [r4, r1]
0045e390 40d04de2 sub sp, sp, #0x40
0045e394 003093e5 ldr r3, [r3]
0045e398 02a0a0e1 mov sl, r2
0045e39c 24508de2 add r5, sp, #0x24
0045e3a0 3c308de5 str r3, [sp, #0x3c]
0045e3a4 0c7090e5 ldr r7, [r0, #0xc]
0045e3a8 0600a0e1 mov r0, r6
0045e3ac 3565fbeb bl #0x337888
0045e3b0 f8109fe5 ldr r1, [pc, #0xf8]
0045e3b4 20208de2 add r2, sp, #0x20
0045e3b8 0500a0e1 mov r0, r5
0045e3bc 01108fe0 add r1, pc, r1
0045e3c0 ec809fe5 ldr r8, [pc, #0xec]
0045e3c4 48d7faeb bl #0x3140ec
0045e3c8 0510a0e1 mov r1, r5
0045e3cc 0600a0e1 mov r0, r6
0045e3d0 ac65fbeb bl #0x337a88
0045e3d4 0500a0e1 mov r0, r5
0045e3d8 9de7faeb bl #0x318254
0045e3dc 083094e7 ldr r3, [r4, r8]
0045e3e0 14608de2 add r6, sp, #0x14
0045e3e4 0c2097e5 ldr r2, [r7, #0xc]
0045e3e8 381093e5 ldr r1, [r3, #0x38]
0045e3ec 0050a0e3 mov r5, #0
0045e3f0 0a30a0e1 mov r3, sl
0045e3f4 0600a0e1 mov r0, r6
0045e3f8 00508de5 str r5, [sp]
0045e3fc 04508de5 str r5, [sp, #4]
0045e400 26b2fbeb bl #0x34aca0
0045e404 0510a0e1 mov r1, r5
0045e408 0600a0e1 mov r0, r6
0045e40c 6b86fbeb bl #0x33fdc0
0045e410 005050e2 subs r5, r0, #0
0045e414 1900001a bne #0x45e480
0045e418 083094e7 ldr r3, [r4, r8]
0045e41c 08608de2 add r6, sp, #8
0045e420 142097e5 ldr r2, [r7, #0x14]
0045e424 381093e5 ldr r1, [r3, #0x38]
0045e428 0070a0e3 mov r7, #0
0045e42c 0a30a0e1 mov r3, sl
0045e430 0600a0e1 mov r0, r6
0045e434 00708de5 str r7, [sp]
0045e438 04708de5 str r7, [sp, #4]
0045e43c 17b2fbeb bl #0x34aca0
0045e440 0710a0e1 mov r1, r7
0045e444 0600a0e1 mov r0, r6
0045e448 5c86fbeb bl #0x33fdc0
0045e44c 001050e2 subs r1, r0, #0
0045e450 0e00001a bne #0x45e490
0045e454 000055e3 cmp r5, #0
0045e458 0100000a beq #0x45e464
0045e45c 0500a0e1 mov r0, r5
0045e460 9f1dfdeb bl #0x3a5ae4
0045e464 093094e7 ldr r3, [r4, sb]
0045e468 3c209de5 ldr r2, [sp, #0x3c]
0045e46c 003093e5 ldr r3, [r3]
0045e470 030052e1 cmp r2, r3
0045e474 0900001a bne #0x45e4a0
0045e478 40d08de2 add sp, sp, #0x40
0045e47c f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045e480 0600a0e1 mov r0, r6
0045e484 b286fbeb bl #0x33ff54
0045e488 0050a0e1 mov r5, r0
0045e48c e1ffffea b #0x45e418
0045e490 0600a0e1 mov r0, r6
0045e494 9286fbeb bl #0x33fee4
0045e498 0010a0e1 mov r1, r0
0045e49c ecffffea b #0x45e454
0045e4a0 9abffaeb bl #0x30e310
0045e4a4 0c675300 subseq r6, r3, ip, lsl #14
0045e4a8 ac400000 andeq r4, r0, ip, lsr #1
0045e4ac 84080000 andeq r0, r0, r4, lsl #17
0045e4b0 c4ec4600 subeq lr, r6, r4, asr #25
0045e4b4 f4370000 strdeq r3, r4, [r0], -r4
