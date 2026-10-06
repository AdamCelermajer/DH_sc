local source effect defaults 1360
006324c4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006324c8 mov r7, r0
006324cc ldr r0, [r0]
006324d0 mov r4, r1
006324d4 sub sp, sp, #0x6c
006324d8 ldrh r1, [r0, #0xe]
006324dc str r3, [sp, #0xc]
006324e0 ldr r3, [r2, #0x10]
006324e4 cmp r4, r1
006324e8 ldrlo sb, [r0, #0x20]
006324ec movhs sb, #0
006324f0 ldr r3, [r3]
006324f4 addlo sb, sb, r4, lsl #4
006324f8 ldr r8, [sb, #8]
006324fc ldr r5, [pc, #0x4e8]
00632500 mov r6, r2
00632504 cmp r3, r8
00632508 add r5, pc, r5
0063250c bhs #0x63253c
00632510 ldr r3, [sb]
00632514 ldr r1, [pc, #0x4d4]
00632518 ldr r2, [r0, #8]
0063251c cmp r3, #0
00632520 mov r0, #3
00632524 addne r3, r3, #4
00632528 add r1, pc, r1
0063252c bl #0x60b034
00632530 mov r0, #0
00632534 add sp, sp, #0x6c
00632538 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0063253c ldr r2, [pc, #0x4b0]
00632540 ldrb fp, [sb, #6]
00632544 ldr r3, [r6, #4]
00632548 add r2, pc, r2
0063254c ldr ip, [r2, fp, lsl #2]
00632550 mov r2, #1
00632554 mov sl, r3
00632558 ands ip, ip, r2, lsl r3
0063255c bne #0x6325cc
00632560 ldr r7, [sb]
00632564 ldr r5, [r0, #8]
00632568 cmp r7, #0
0063256c addne r7, r7, #4
00632570 cmp fp, #0xff
00632574 beq #0x632688
00632578 mov r0, #0
0063257c bl #0x5e80b4
00632580 ldr sl, [r6, #4]
00632584 ldr r4, [r0, fp, lsl #2]
00632588 ldr r1, [pc, #0x468]
0063258c mov r2, #0x58
00632590 add r0, sp, #0x10
00632594 add r1, pc, r1
00632598 bl #0x30e868
0063259c add r3, sp, #0x68
006325a0 add sl, r3, sl, lsl #2
006325a4 ldr r1, [pc, #0x450]
006325a8 ldr ip, [sl, #-0x58]
006325ac mov r0, #3
006325b0 mov r2, r5
006325b4 add r1, pc, r1
006325b8 mov r3, r7
006325bc stm sp, {r4, ip}
006325c0 bl #0x60b034
006325c4 mov r0, #0
006325c8 b #0x632534
006325cc sub fp, fp, #9
006325d0 cmp fp, #9
006325d4 addls pc, pc, fp, lsl #2
006325d8 b #0x632694
006325dc b #0x632680
006325e0 b #0x632680
006325e4 b #0x6326e8
006325e8 b #0x6327b8
006325ec b #0x632840
006325f0 b #0x6328c8
006325f4 b #0x632950
006325f8 b #0x632694
006325fc b #0x632694
00632600 b #0x632604
00632604 cmp r8, #0
00632608 beq #0x632680
0063260c mov sl, #0
00632610 mov fp, r7
00632614 mov r5, sl
00632618 mov r7, r4
0063261c add sb, sp, #0x10
00632620 ldr r4, [sp, #0xc]
00632624 b #0x632664
00632628 ldrsb r2, [r3]
0063262c cmp r2, #0x23
00632630 beq #0x6329d8
00632634 cmp r4, #0
00632638 beq #0x632654
0063263c mov r0, r4
00632640 mov r1, fp
00632644 mov r2, r7
00632648 mov r3, r5
0063264c str sb, [sp]
00632650 bl #0x65b08c
00632654 add r5, r5, #1
00632658 cmp r5, r8
0063265c add sl, sl, #4
00632660 beq #0x632680
00632664 ldr r3, [r6, #0x14]
00632668 add r3, r3, sl
0063266c ldr r3, [r3]
00632670 str r3, [sp, #0x10]
00632674 ldr r2, [r3, #-4]
00632678 cmp r2, #0
0063267c bne #0x632628
00632680 mov r0, #1
00632684 b #0x632534
00632688 ldr r4, [pc, #0x370]
0063268c add r4, pc, r4
00632690 b #0x632588
00632694 ldr r2, [pc, #0x368]
00632698 ldr lr, [pc, #0x368]
0063269c ldr ip, [pc, #0x368]
006326a0 ldr r2, [r5, r2]
006326a4 add r1, r3, #1
006326a8 ldr ip, [r5, ip]
006326ac ldr r5, [r5, lr]
006326b0 ldr lr, [r2, r1, lsl #2]
006326b4 ldrb ip, [ip, r1]
006326b8 ldr r2, [pc, #0x350]
006326bc ldrb r1, [r5, lr]
006326c0 add r2, pc, r2
006326c4 mul ip, ip, r1
006326c8 add r2, r2, #0x4c
006326cc ldr r2, [r2, r3, lsl #2]
006326d0 mov r1, r4
006326d4 ldr r3, [r6, #0x14]
006326d8 str ip, [sp]
006326dc bl #0x5d5860
006326e0 mov r0, #1
006326e4 b #0x632534
006326e8 add sl, sp, #0x10
006326ec mov r0, sl
006326f0 bl #0x631c14
006326f4 ldr r2, [pc, #0x308]
006326f8 ldr r3, [r6, #4]
006326fc ldr fp, [sb, #8]
00632700 ldr r1, [r5, r2]
00632704 ldr r2, [pc, #0x2fc]
00632708 add r3, r3, #1
0063270c ldr r1, [r1, r3, lsl #2]
00632710 ldr r0, [r5, r2]
00632714 ldr r2, [pc, #0x2f0]
00632718 cmp fp, #0
0063271c ldr r2, [r5, r2]
00632720 ldrb r2, [r2, r3]
00632724 ldrb r3, [r0, r1]
00632728 mul r3, r2, r3
0063272c beq #0x632680
00632730 mov r8, #0
00632734 str r7, [sp, #0xc]
00632738 mov r5, r8
0063273c mov r7, r4
00632740 mov sb, r8
00632744 mov r4, r3
00632748 b #0x63275c
0063274c add r5, r5, #1
00632750 cmp r5, fp
00632754 add r8, r8, r4
00632758 beq #0x632680
0063275c ldr lr, [r6, #0x14]
00632760 mov ip, sl
00632764 strb sb, [sp, #0x50]
00632768 add lr, lr, r8
0063276c ldm lr!, {r0, r1, r2, r3}
00632770 stm ip!, {r0, r1, r2, r3}
00632774 ldm lr!, {r0, r1, r2, r3}
00632778 stm ip!, {r0, r1, r2, r3}
0063277c ldm lr!, {r0, r1, r2, r3}
00632780 stm ip!, {r0, r1, r2, r3}
00632784 ldm lr, {r0, r1, r2, r3}
00632788 stm ip, {r0, r1, r2, r3}
0063278c mov r0, sl
00632790 bl #0x5ba19c
00632794 cmp r0, #0
00632798 bne #0x63274c
0063279c ldr r3, [sp, #0xc]
006327a0 mov r2, r5
006327a4 mov r1, r7
006327a8 ldr r0, [r3]
006327ac mov r3, sl
006327b0 bl #0x5d4518
006327b4 b #0x63274c
006327b8 cmp r4, r1
006327bc ldrlo r3, [r0, #0x20]
006327c0 movhs r3, #0
006327c4 ldr r5, [r6, #0x14]
006327c8 addlo r3, r3, r4, lsl #4
006327cc ldr r8, [r3, #8]
006327d0 cmp r8, #0
006327d4 beq #0x632680
006327d8 mov r6, #0
006327dc add sl, sp, #0x10
006327e0 ldr r0, [r5, r6, lsl #2]
006327e4 mov r2, r6
006327e8 mov r1, r4
006327ec ldr r0, [r0]
006327f0 mov r3, sl
006327f4 add r6, r6, #1
006327f8 cmp r0, #0
006327fc beq #0x632830
00632800 ldr r0, [r0, #0x10]
00632804 cmp r0, #0
00632808 str r0, [sp, #0x10]
0063280c ldrne ip, [r0, #4]
00632810 addne ip, ip, #1
00632814 strne ip, [r0, #4]
00632818 ldr r0, [r7]
0063281c bl #0x5d691c
00632820 ldr r0, [sp, #0x10]
00632824 cmp r0, #0
00632828 beq #0x632830
0063282c bl #0x31d584
00632830 cmp r6, r8
00632834 bne #0x6327e0
00632838 mov r0, #1
0063283c b #0x632534
00632840 cmp r4, r1
00632844 ldrlo r3, [r0, #0x20]
00632848 movhs r3, #0
0063284c ldr r5, [r6, #0x14]
00632850 addlo r3, r3, r4, lsl #4
00632854 ldr r8, [r3, #8]
00632858 cmp r8, #0
0063285c beq #0x632680
00632860 mov r6, #0
00632864 add sl, sp, #0x10
00632868 ldr r0, [r5, r6, lsl #2]
0063286c mov r2, r6
00632870 mov r1, r4
00632874 ldr r0, [r0]
00632878 mov r3, sl
0063287c add r6, r6, #1
00632880 cmp r0, #0
00632884 beq #0x6328b8
00632888 ldr r0, [r0, #0x10]
0063288c cmp r0, #0
00632890 str r0, [sp, #0x10]
00632894 ldrne ip, [r0, #4]
00632898 addne ip, ip, #1
0063289c strne ip, [r0, #4]
006328a0 ldr r0, [r7]
006328a4 bl #0x5d691c
006328a8 ldr r0, [sp, #0x10]
006328ac cmp r0, #0
006328b0 beq #0x6328b8
006328b4 bl #0x31d584
006328b8 cmp r6, r8
006328bc bne #0x632868
006328c0 mov r0, #1
006328c4 b #0x632534
006328c8 cmp r4, r1
006328cc ldrlo r3, [r0, #0x20]
006328d0 movhs r3, #0
006328d4 ldr r5, [r6, #0x14]
006328d8 addlo r3, r3, r4, lsl #4
006328dc ldr r8, [r3, #8]
006328e0 cmp r8, #0
006328e4 beq #0x632680
006328e8 mov r6, #0
006328ec add sl, sp, #0x10
006328f0 ldr r0, [r5, r6, lsl #2]
006328f4 mov r2, r6
006328f8 mov r1, r4
006328fc ldr r0, [r0]
00632900 mov r3, sl
00632904 add r6, r6, #1
00632908 cmp r0, #0
0063290c beq #0x632940
00632910 ldr r0, [r0, #0x10]
00632914 cmp r0, #0
00632918 str r0, [sp, #0x10]
0063291c ldrne ip, [r0, #4]
00632920 addne ip, ip, #1
00632924 strne ip, [r0, #4]
00632928 ldr r0, [r7]
0063292c bl #0x5d691c
00632930 ldr r0, [sp, #0x10]
00632934 cmp r0, #0
00632938 beq #0x632940
0063293c bl #0x31d584
00632940 cmp r6, r8
00632944 bne #0x6328f0
00632948 mov r0, #1
0063294c b #0x632534
00632950 cmp r4, r1
00632954 ldrlo r3, [r0, #0x20]
00632958 movhs r3, #0
0063295c ldr r5, [r6, #0x14]
00632960 addlo r3, r3, r4, lsl #4
00632964 ldr r8, [r3, #8]
00632968 cmp r8, #0
0063296c beq #0x632680
00632970 mov r6, #0
00632974 add sl, sp, #0x10
00632978 ldr r0, [r5, r6, lsl #2]
0063297c mov r2, r6
00632980 mov r1, r4
00632984 ldr r0, [r0]
00632988 mov r3, sl
0063298c add r6, r6, #1
00632990 cmp r0, #0
00632994 beq #0x6329c8
00632998 ldr r0, [r0, #0x10]
0063299c cmp r0, #0
006329a0 str r0, [sp, #0x10]
006329a4 ldrne ip, [r0, #4]
006329a8 addne ip, ip, #1
006329ac strne ip, [r0, #4]
006329b0 ldr r0, [r7]
006329b4 bl #0x5d691c
006329b8 ldr r0, [sp, #0x10]
006329bc cmp r0, #0
006329c0 beq #0x6329c8
006329c4 bl #0x31d584
006329c8 cmp r6, r8
006329cc bne #0x632978
006329d0 mov r0, #1
006329d4 b #0x632534
006329d8 ldrsb r3, [r3, #1]
006329dc cmp r3, #0
006329e0 bne #0x632634
006329e4 mov r0, #1
006329e8 b #0x632534
006329ec eorseq r2, r6, r8, lsl #11
006329f0 eoreq r2, fp, r0, asr #20
006329f4 eoreq r2, fp, r4, lsl #18
006329f8 ldrsbteq r4, [r2], -r4
006329fc eoreq r2, fp, r4, ror #19
00632a00 .byte 0xd4, 0x3d, 0x29, 0x00
00632a04 andeq r3, r0, ip, lsr #23
00632a08 andeq r1, r0, r0, asr #11
00632a0c andeq r2, r0, ip, lsr #23
00632a10 eoreq r2, fp, ip, lsl #15
