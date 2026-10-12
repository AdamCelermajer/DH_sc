0045d650 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045d654 ec409fe5 ldr r4, [pc, #0xec]
0045d658 ec509fe5 ldr r5, [pc, #0xec]
0045d65c ecc09fe5 ldr ip, [pc, #0xec]
0045d660 04408fe0 add r4, pc, r4
0045d664 053094e7 ldr r3, [r4, r5]
0045d668 0ca094e7 ldr sl, [r4, ip]
0045d66c 3cd04de2 sub sp, sp, #0x3c
0045d670 003093e5 ldr r3, [r3]
0045d674 0080a0e1 mov r8, r0
0045d678 0a00a0e1 mov r0, sl
0045d67c 34308de5 str r3, [sp, #0x34]
0045d680 0290a0e1 mov sb, r2
0045d684 01b0a0e1 mov fp, r1
0045d688 0c7098e5 ldr r7, [r8, #0xc]
0045d68c 7d68fbeb bl #0x337888
0045d690 bc109fe5 ldr r1, [pc, #0xbc]
0045d694 1c608de2 add r6, sp, #0x1c
0045d698 18208de2 add r2, sp, #0x18
0045d69c 01108fe0 add r1, pc, r1
0045d6a0 0600a0e1 mov r0, r6
0045d6a4 90dafaeb bl #0x3140ec
0045d6a8 0610a0e1 mov r1, r6
0045d6ac 0a00a0e1 mov r0, sl
0045d6b0 f468fbeb bl #0x337a88
0045d6b4 0600a0e1 mov r0, r6
0045d6b8 e5eafaeb bl #0x318254
0045d6bc 94309fe5 ldr r3, [pc, #0x94]
0045d6c0 0ca08de2 add sl, sp, #0xc
0045d6c4 0c2097e5 ldr r2, [r7, #0xc]
0045d6c8 031094e7 ldr r1, [r4, r3]
0045d6cc 0060a0e3 mov r6, #0
0045d6d0 0930a0e1 mov r3, sb
0045d6d4 381091e5 ldr r1, [r1, #0x38]
0045d6d8 0a00a0e1 mov r0, sl
0045d6dc 00608de5 str r6, [sp]
0045d6e0 04608de5 str r6, [sp, #4]
0045d6e4 6db5fbeb bl #0x34aca0
0045d6e8 0a00a0e1 mov r0, sl
0045d6ec 0610a0e1 mov r1, r6
0045d6f0 b289fbeb bl #0x33fdc0
0045d6f4 003050e2 subs r3, r0, #0
0045d6f8 0200000a beq #0x45d708
0045d6fc f42093e5 ldr r2, [r3, #0xf4]
0045d700 020052e3 cmp r2, #2
0045d704 0800000a beq #0x45d72c
0045d708 0030a0e3 mov r3, #0
0045d70c 103088e5 str r3, [r8, #0x10]
0045d710 053094e7 ldr r3, [r4, r5]
0045d714 34209de5 ldr r2, [sp, #0x34]
0045d718 003093e5 ldr r3, [r3]
0045d71c 030052e1 cmp r2, r3
0045d720 0700001a bne #0x45d744
0045d724 3cd08de2 add sp, sp, #0x3c
0045d728 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045d72c 103088e5 str r3, [r8, #0x10]
0045d730 1030d7e5 ldrb r3, [r7, #0x10]
0045d734 0b10a0e1 mov r1, fp
0045d738 1430c8e5 strb r3, [r8, #0x14]
0045d73c be27feeb bl #0x3e763c
0045d740 f2ffffea b #0x45d710
0045d744 f1c2faeb bl #0x30e310
0045d748 30745300 subseq r7, r3, r0, lsr r4
0045d74c ac400000 andeq r4, r0, ip, lsr #1
0045d750 84080000 andeq r0, r0, r4, lsl #17
0045d754 e4f94600 subeq pc, r6, r4, ror #19
0045d758 f4370000 strdeq r3, r4, [r0], -r4
