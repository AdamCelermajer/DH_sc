0045c670 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045c674 10429fe5 ldr r4, [pc, #0x210]
0045c678 10529fe5 ldr r5, [pc, #0x210]
0045c67c 10229fe5 ldr r2, [pc, #0x210]
0045c680 04408fe0 add r4, pc, r4
0045c684 053094e7 ldr r3, [r4, r5]
0045c688 028094e7 ldr r8, [r4, r2]
0045c68c 28d04de2 sub sp, sp, #0x28
0045c690 003093e5 ldr r3, [r3]
0045c694 0c608de2 add r6, sp, #0xc
0045c698 24308de5 str r3, [sp, #0x24]
0045c69c 0c7090e5 ldr r7, [r0, #0xc]
0045c6a0 0800a0e1 mov r0, r8
0045c6a4 776cfbeb bl #0x337888
0045c6a8 e8119fe5 ldr r1, [pc, #0x1e8]
0045c6ac 08208de2 add r2, sp, #8
0045c6b0 0600a0e1 mov r0, r6
0045c6b4 01108fe0 add r1, pc, r1
0045c6b8 8bdefaeb bl #0x3140ec
0045c6bc 0610a0e1 mov r1, r6
0045c6c0 0800a0e1 mov r0, r8
0045c6c4 ef6cfbeb bl #0x337a88
0045c6c8 0600a0e1 mov r0, r6
0045c6cc e0eefaeb bl #0x318254
0045c6d0 c4319fe5 ldr r3, [pc, #0x1c4]
0045c6d4 030094e7 ldr r0, [r4, r3]
0045c6d8 ad0bfbeb bl #0x31f594
0045c6dc 008050e2 subs r8, r0, #0
0045c6e0 1100000a beq #0x45c72c
0045c6e4 286198e5 ldr r6, [r8, #0x128]
0045c6e8 000056e3 cmp r6, #0
0045c6ec 0e00000a beq #0x45c72c
0045c6f0 0c0097e5 ldr r0, [r7, #0xc]
0045c6f4 000050e3 cmp r0, #0
0045c6f8 120000da ble #0x45c748
0045c6fc 98c8faeb bl #0x30e964
0045c700 08a096e5 ldr sl, [r6, #8]
0045c704 0010a0e1 mov r1, r0
0045c708 00309ae5 ldr r3, [sl]
0045c70c 0a00a0e1 mov r0, sl
0045c710 0fe0a0e1 mov lr, pc
0045c714 30f193e5 ldr pc, [r3, #0x130]
0045c718 080097e5 ldr r0, [r7, #8]
0045c71c 000050e3 cmp r0, #0
0045c720 180000ca bgt #0x45c788
0045c724 020070e3 cmn r0, #2
0045c728 1e00000a beq #0x45c7a8
0045c72c 053094e7 ldr r3, [r4, r5]
0045c730 24209de5 ldr r2, [sp, #0x24]
0045c734 003093e5 ldr r3, [r3]
0045c738 030052e1 cmp r2, r3
0045c73c 5100001a bne #0x45c888
0045c740 28d08de2 add sp, sp, #0x28
0045c744 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045c748 020070e3 cmn r0, #2
0045c74c f1ffff1a bne #0x45c718
0045c750 08a096e5 ldr sl, [r6, #8]
0045c754 383098e5 ldr r3, [r8, #0x38]
0045c758 00209ae5 ldr r2, [sl]
0045c75c 000053e3 cmp r3, #0
0045c760 309192e5 ldr sb, [r2, #0x130]
0045c764 3100000a beq #0x45c830
0045c768 940293e5 ldr r0, [r3, #0x294]
0045c76c 7cc8faeb bl #0x30e964
0045c770 0010a0e1 mov r1, r0
0045c774 0a00a0e1 mov r0, sl
0045c778 39ff2fe1 blx sb
0045c77c 080097e5 ldr r0, [r7, #8]
0045c780 000050e3 cmp r0, #0
0045c784 e6ffffda ble #0x45c724
0045c788 75c8faeb bl #0x30e964
0045c78c 086096e5 ldr r6, [r6, #8]
0045c790 0010a0e1 mov r1, r0
0045c794 003096e5 ldr r3, [r6]
0045c798 0600a0e1 mov r0, r6
0045c79c 0fe0a0e1 mov lr, pc
0045c7a0 34f193e5 ldr pc, [r3, #0x134]
0045c7a4 e0ffffea b #0x45c72c
0045c7a8 086096e5 ldr r6, [r6, #8]
0045c7ac 383098e5 ldr r3, [r8, #0x38]
0045c7b0 002096e5 ldr r2, [r6]
0045c7b4 000053e3 cmp r3, #0
0045c7b8 347192e5 ldr r7, [r2, #0x134]
0045c7bc 0500000a beq #0x45c7d8
0045c7c0 980293e5 ldr r0, [r3, #0x298]
0045c7c4 66c8faeb bl #0x30e964
0045c7c8 0010a0e1 mov r1, r0
0045c7cc 0600a0e1 mov r0, r6
0045c7d0 37ff2fe1 blx r7
0045c7d4 d4ffffea b #0x45c72c
0045c7d8 c0209fe5 ldr r2, [pc, #0xc0]
0045c7dc 022094e7 ldr r2, [r4, r2]
0045c7e0 002092e5 ldr r2, [r2]
0045c7e4 020052e3 cmp r2, #2
0045c7e8 00308305 streq r3, [r3]
0045c7ec f3ffff0a beq #0x45c7c0
0045c7f0 010052e3 cmp r2, #1
0045c7f4 f1ffff1a bne #0x45c7c0
0045c7f8 a4009fe5 ldr r0, [pc, #0xa4]
0045c7fc a4109fe5 ldr r1, [pc, #0xa4]
0045c800 a4209fe5 ldr r2, [pc, #0xa4]
0045c804 000094e7 ldr r0, [r4, r0]
0045c808 a0309fe5 ldr r3, [pc, #0xa0]
0045c80c 77cfa0e3 mov ip, #0x1dc
0045c810 01108fe0 add r1, pc, r1
0045c814 03308fe0 add r3, pc, r3
0045c818 a80080e2 add r0, r0, #0xa8
0045c81c 02208fe0 add r2, pc, r2
0045c820 00c08de5 str ip, [sp]
0045c824 f6c5faeb bl #0x30e004
0045c828 383098e5 ldr r3, [r8, #0x38]
0045c82c e3ffffea b #0x45c7c0
0045c830 68209fe5 ldr r2, [pc, #0x68]
0045c834 022094e7 ldr r2, [r4, r2]
0045c838 002092e5 ldr r2, [r2]
0045c83c 020052e3 cmp r2, #2
0045c840 00308305 streq r3, [r3]
0045c844 c7ffff0a beq #0x45c768
0045c848 010052e3 cmp r2, #1
0045c84c c5ffff1a bne #0x45c768
0045c850 4c009fe5 ldr r0, [pc, #0x4c]
0045c854 58109fe5 ldr r1, [pc, #0x58]
0045c858 58209fe5 ldr r2, [pc, #0x58]
0045c85c 000094e7 ldr r0, [r4, r0]
0045c860 54309fe5 ldr r3, [pc, #0x54]
0045c864 77cfa0e3 mov ip, #0x1dc
0045c868 01108fe0 add r1, pc, r1
0045c86c 03308fe0 add r3, pc, r3
0045c870 a80080e2 add r0, r0, #0xa8
0045c874 02208fe0 add r2, pc, r2
0045c878 00c08de5 str ip, [sp]
0045c87c e0c5faeb bl #0x30e004
0045c880 383098e5 ldr r3, [r8, #0x38]
0045c884 b7ffffea b #0x45c768
0045c888 a0c6faeb bl #0x30e310
0045c88c 10845300 subseq r8, r3, r0, lsl r4
0045c890 ac400000 andeq r4, r0, ip, lsr #1
0045c894 84080000 andeq r0, r0, r4, lsl #17
0045c898 cc094700 subeq r0, r7, ip, asr #19
0045c89c f4370000 strdeq r3, r4, [r0], -r4
0045c8a0 c0390000 andeq r3, r0, r0, asr #19
0045c8a4 c0190000 andeq r1, r0, r0, asr #19
0045c8a8 c81b4600 subeq r1, r6, r8, asr #23
0045c8ac d4934600 ldrdeq sb, sl, [r6], #-0x34
0045c8b0 44544600 subeq r5, r6, r4, asr #8
0045c8b4 701b4600 subeq r1, r6, r0, ror fp
0045c8b8 7c934600 subeq sb, r6, ip, ror r3
0045c8bc ec534600 subeq r5, r6, ip, ror #7
