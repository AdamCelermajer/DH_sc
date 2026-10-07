
SOURCE 0x46932c _ZN14PlayerSavegame16__LoadPropertiesEP11IStreamBasePv
0046932c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469330 ldr sb, [r1, #0x10]
00469334 ldr r5, [pc, #0xfc]
00469338 sub sp, sp, #0x14
0046933c cmp sb, #0
00469340 mov r6, r1
00469344 mov r4, r0
00469348 add r5, pc, r5
0046934c beq #0x4693d8
00469350 mov r0, r4
00469354 add r1, sp, #0xc
00469358 bl #0x38b758
0046935c ldr r3, [sp, #0xc]
00469360 add r8, sb, #0x560
00469364 cmp r3, #0xe0
00469368 beq #0x469374
0046936c add sp, sp, #0x14
00469370 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469374 ldr fp, [pc, #0xc0]
00469378 add sb, sb, #0x8e0
0046937c add sb, sb, #0xc
00469380 mov r7, #0
00469384 add sl, sp, #8
00469388 mov r0, r4
0046938c mov r1, sl
00469390 bl #0x38b758
00469394 mov r1, r7
00469398 mov r0, r8
0046939c bl #0x3deed8
004693a0 tst r0, #0x20
004693a4 ldrne r3, [r5, fp]
004693a8 ldrne r2, [sp, #8]
004693ac ldrne r3, [r3, r7, lsl #2]
004693b0 add r7, r7, #1
004693b4 addne r3, sb, r3
004693b8 strne r2, [r3, #4]
004693bc ldr r3, [sp, #0xc]
004693c0 cmp r3, r7
004693c4 bgt #0x469388
004693c8 mov r0, r4
004693cc add r1, r6, #0x194
004693d0 bl #0x33e040
004693d4 b #0x46936c
004693d8 ldr r3, [pc, #0x60]
004693dc ldr r3, [r5, r3]
004693e0 ldr r3, [r3]
004693e4 cmp r3, #2
004693e8 streq sb, [sb]
004693ec beq #0x46936c
004693f0 cmp r3, #1
004693f4 bne #0x46936c
004693f8 ldr r0, [pc, #0x44]
004693fc ldr r1, [pc, #0x44]
00469400 ldr r2, [pc, #0x44]
00469404 ldr r0, [r5, r0]
00469408 ldr r3, [pc, #0x40]
0046940c movw ip, #0x31a
00469410 add r1, pc, r1
00469414 add r0, r0, #0xa8
00469418 add r2, pc, r2
0046941c add r3, pc, r3
00469420 str ip, [sp]
00469424 bl #0x30e004
00469428 ldr sb, [r6, #0x10]
0046942c cmp sb, #0
00469430 bne #0x469350
00469434 b #0x46936c
00469438 subseq fp, r2, r8, asr #14
0046943c andeq r2, r0, r8, lsr #5
00469440 andeq r3, r0, r0, asr #19
00469444 andeq r1, r0, r0, asr #19
00469448 subeq r4, r5, r8, asr #31
0046944c subeq r4, r6, r0, lsl #1
00469450 subeq r4, r6, ip, lsl #1

SOURCE 0x33665c _ZN12StreamReader7writeAsIiEEvP11IStreamBaseT_
0033665c str lr, [sp, #-4]!
00336660 sub sp, sp, #0x14
00336664 add r3, sp, #0x10
00336668 str r1, [r3, #-4]!
0033666c mov r1, r3
00336670 ldr ip, [r0]
00336674 mov r3, #0
00336678 mov r2, #4
0033667c mov lr, pc
00336680 ldr pc, [ip, #0x1c]
00336684 ldr r3, [pc, #0x74]
00336688 cmp r0, #4
0033668c add r3, pc, r3
00336690 beq #0x3366c0
00336694 ldr r2, [pc, #0x68]
00336698 ldr r2, [r3, r2]
0033669c ldr r2, [r2]
003366a0 cmp r2, #2
003366a4 moveq r3, #0
003366a8 streq r3, [r3]
003366ac beq #0x3366b8
003366b0 cmp r2, #1
003366b4 beq #0x3366cc
003366b8 add sp, sp, #0x14
003366bc ldm sp!, {pc}
003366c0 cmp r1, #0
003366c4 beq #0x3366b8
003366c8 b #0x336694
003366cc ldr r0, [pc, #0x34]
003366d0 ldr r1, [pc, #0x34]
003366d4 ldr r2, [pc, #0x34]
003366d8 ldr r0, [r3, r0]
003366dc ldr r3, [pc, #0x30]
003366e0 mov ip, #0x74
003366e4 add r1, pc, r1
003366e8 add r2, pc, r2
003366ec add r3, pc, r3
003366f0 add r0, r0, #0xa8
003366f4 str ip, [sp]
003366f8 bl #0x30e004
003366fc b #0x3366b8
00336700 rsbeq lr, r5, r4, lsl #8
00336704 andeq r3, r0, r0, asr #19
00336708 andeq r1, r0, r0, asr #19
0033670c ldrsheq r7, [r8], #-0xc4
00336710 ldrheq r7, [r8], #-0xd8
00336714 subseq sb, r8, r4, asr r6

SOURCE 0x467040 _ZNK14PlayerSavegame16SG_GetLevelStateEii
00467040 push {r4, r5, r6, r7, lr}
00467044 ldr r4, [pc, #0xec]
00467048 subs r5, r1, #0
0046704c sub sp, sp, #0xc
00467050 mov r6, r0
00467054 add r4, pc, r4
00467058 mov r7, r2
0046705c blt #0x4670ac
00467060 ldr r3, [pc, #0xd4]
00467064 ldr r3, [r4, r3]
00467068 ldr r3, [r3]
0046706c cmp r5, r3
00467070 blt #0x467098
00467074 ldr r3, [pc, #0xc4]
00467078 ldr r3, [r4, r3]
0046707c ldr r3, [r3]
00467080 cmp r3, #2
00467084 moveq r3, #0
00467088 streq r3, [r3]
0046708c beq #0x467098
00467090 cmp r3, #1
00467094 beq #0x467104
00467098 add r7, r7, #0x1a
0046709c ldr r3, [r6, r7, lsl #2]
004670a0 ldr r0, [r3, r5, lsl #2]
004670a4 add sp, sp, #0xc
004670a8 pop {r4, r5, r6, r7, pc}
004670ac ldr r3, [pc, #0x8c]
004670b0 ldr r3, [r4, r3]
004670b4 ldr r3, [r3]
004670b8 cmp r3, #2
004670bc moveq r3, #0
004670c0 streq r3, [r3]
004670c4 beq #0x467060
004670c8 cmp r3, #1
004670cc bne #0x467060
004670d0 ldr r0, [pc, #0x6c]
004670d4 ldr r1, [pc, #0x6c]
004670d8 ldr r2, [pc, #0x6c]
004670dc ldr r0, [r4, r0]
004670e0 ldr r3, [pc, #0x68]
004670e4 mov ip, #0x51
004670e8 add r1, pc, r1
004670ec add r2, pc, r2
004670f0 add r3, pc, r3
004670f4 add r0, r0, #0xa8
004670f8 str ip, [sp]
004670fc bl #0x30e004
00467100 b #0x467060
00467104 ldr r0, [pc, #0x38]
00467108 ldr r1, [pc, #0x44]
0046710c ldr r2, [pc, #0x44]
00467110 ldr r0, [r4, r0]
00467114 ldr r3, [pc, #0x40]
00467118 mov ip, #0x52
0046711c add r1, pc, r1
00467120 add r2, pc, r2
00467124 add r3, pc, r3
00467128 add r0, r0, #0xa8
0046712c str ip, [sp]
00467130 bl #0x30e004
00467134 b #0x467098
00467138 subseq sp, r2, ip, lsr sl
0046713c andeq r1, r0, r0, asr #17
00467140 andeq r3, r0, r0, asr #19
00467144 andeq r1, r0, r0, asr #19
00467148 strdeq r7, r8, [r5], #-0x20
0046714c subeq r6, r6, r4, lsl #5
00467150 umaaleq r6, r6, r0, r1
00467154 strheq r7, [r5], #-0x2c
00467158 subeq r6, r6, r0, ror #4
0046715c subeq r6, r6, ip, asr r1

SOURCE 0x46c6fc _ZN13QuestSavegame10SaveQuestsEP11IStreamBase
0046c6fc push {r4, r5, r6, lr}
0046c700 mov r4, r1
0046c704 mov r5, r0
0046c708 mov r2, r4
0046c70c mov r1, #0
0046c710 bl #0x46c658
0046c714 mov r0, r5
0046c718 mov r2, r4
0046c71c mov r1, #1
0046c720 bl #0x46c658
0046c724 mov r0, r5
0046c728 mov r2, r4
0046c72c mov r1, #2
0046c730 pop {r4, r5, r6, lr}
0046c734 b #0x46c658

SOURCE 0x46c584 _ZN13QuestSavegame9PackQuestEiiP11IStreamBase
0046c584 push {r4, r5, r6, r7, r8, sl, lr}
0046c588 ldr r4, [pc, #0xc0]
0046c58c ldr r6, [pc, #0xc0]
0046c590 sub sp, sp, #0x2c
0046c594 add r4, pc, r4
0046c598 ldr ip, [r4, r6]
0046c59c add r5, sp, #0xc
0046c5a0 mov r7, r0
0046c5a4 ldr ip, [ip]
0046c5a8 mov r0, r5
0046c5ac mov r8, r3
0046c5b0 mov sl, r2
0046c5b4 str ip, [sp, #0x24]
0046c5b8 str r1, [sp, #4]
0046c5bc str r5, [sp, #0x1c]
0046c5c0 str r5, [sp, #0x20]
0046c5c4 bl #0x46be68
0046c5c8 mov r3, #0xc
0046c5cc mla r7, r3, sl, r7
0046c5d0 ldr r3, [sp, #0x1c]
0046c5d4 mov r2, #0
0046c5d8 add r1, sp, #0x28
0046c5dc strb r2, [r3]
0046c5e0 ldr r2, [r1, #-0x24]!
0046c5e4 ldr r3, [r7, #4]
0046c5e8 mov r0, r8
0046c5ec ldr r7, [r3, r2, lsl #2]
0046c5f0 bl #0x38b808
0046c5f4 mov r0, r7
0046c5f8 mov r1, r8
0046c5fc bl #0x47f734
0046c600 ldr r0, [sp, #0x20]
0046c604 cmp r0, r5
0046c608 beq #0x46c628
0046c60c cmp r0, #0
0046c610 beq #0x46c628
0046c614 ldr r1, [sp, #0xc]
0046c618 rsb r1, r0, r1
0046c61c cmp r1, #0x80
0046c620 bhi #0x46c644
0046c624 bl #0x708f00
0046c628 ldr r3, [r4, r6]
0046c62c ldr r2, [sp, #0x24]
0046c630 ldr r3, [r3]
0046c634 cmp r2, r3
0046c638 bne #0x46c64c
0046c63c add sp, sp, #0x2c
0046c640 pop {r4, r5, r6, r7, r8, sl, pc}
0046c644 bl #0x310440
0046c648 b #0x46c628
0046c64c bl #0x30e310
0046c650 ldrsheq r8, [r2], #-0x4c
0046c654 andeq r4, r0, ip, lsr #1

SOURCE 0x466d10 _ZNK14PlayerSavegame17SG_GetMapLocStateEii
00466d10 push {r4, r5, r6, r7, lr}
00466d14 ldr r4, [pc, #0xec]
00466d18 subs r5, r1, #0
00466d1c sub sp, sp, #0xc
00466d20 mov r6, r0
00466d24 add r4, pc, r4
00466d28 mov r7, r2
00466d2c blt #0x466d7c
00466d30 ldr r3, [pc, #0xd4]
00466d34 ldr r3, [r4, r3]
00466d38 ldr r3, [r3]
00466d3c cmp r5, r3
00466d40 blt #0x466d68
00466d44 ldr r3, [pc, #0xc4]
00466d48 ldr r3, [r4, r3]
00466d4c ldr r3, [r3]
00466d50 cmp r3, #2
00466d54 moveq r3, #0
00466d58 streq r3, [r3]
00466d5c beq #0x466d68
00466d60 cmp r3, #1
00466d64 beq #0x466dd4
00466d68 add r6, r6, r7, lsl #2
00466d6c ldr r3, [r6, #0x74]
00466d70 ldr r0, [r3, r5, lsl #2]
00466d74 add sp, sp, #0xc
00466d78 pop {r4, r5, r6, r7, pc}
00466d7c ldr r3, [pc, #0x8c]
00466d80 ldr r3, [r4, r3]
00466d84 ldr r3, [r3]
00466d88 cmp r3, #2
00466d8c moveq r3, #0
00466d90 streq r3, [r3]
00466d94 beq #0x466d30
00466d98 cmp r3, #1
00466d9c bne #0x466d30
00466da0 ldr r0, [pc, #0x6c]
00466da4 ldr r1, [pc, #0x6c]
00466da8 ldr r2, [pc, #0x6c]
00466dac ldr r0, [r4, r0]
00466db0 ldr r3, [pc, #0x68]
00466db4 mov ip, #0x73
00466db8 add r1, pc, r1
00466dbc add r2, pc, r2
00466dc0 add r3, pc, r3
00466dc4 add r0, r0, #0xa8
00466dc8 str ip, [sp]
00466dcc bl #0x30e004
00466dd0 b #0x466d30
00466dd4 ldr r0, [pc, #0x38]
00466dd8 ldr r1, [pc, #0x44]
00466ddc ldr r2, [pc, #0x44]
00466de0 ldr r0, [r4, r0]
00466de4 ldr r3, [pc, #0x40]
00466de8 mov ip, #0x74
00466dec add r1, pc, r1
00466df0 add r2, pc, r2
00466df4 add r3, pc, r3
00466df8 add r0, r0, #0xa8
00466dfc str ip, [sp]
00466e00 bl #0x30e004
00466e04 b #0x466d68
00466e08 subseq sp, r2, ip, ror #26
00466e0c andeq r2, r0, r4, ror r2
00466e10 andeq r3, r0, r0, asr #19
00466e14 andeq r1, r0, r0, asr #19
00466e18 subeq r7, r5, r0, lsr #12
00466e1c subeq r6, r6, ip, lsr r5
00466e20 subeq r6, r6, r0, asr #9
00466e24 subeq r7, r5, ip, ror #11
00466e28 subeq r6, r6, r8, lsl r5
00466e2c subeq r6, r6, ip, lsl #9

SOURCE 0x47f734 _ZN5Quest14_saveQuestDataEP11IStreamBase
0047f734 push {r4, r5, r6, lr}
0047f738 mov r5, r0
0047f73c mov r4, r1
0047f740 mov r0, r1
0047f744 ldr r1, [r5]
0047f748 bl #0x33665c
0047f74c ldr r3, [r5, #0x18]
0047f750 mov r1, r4
0047f754 mov r0, r3
0047f758 ldr r3, [r3]
0047f75c mov lr, pc
0047f760 ldr pc, [r3, #0x2c]
0047f764 ldr r3, [r5, #0x1c]
0047f768 mov r1, r4
0047f76c mov r0, r3
0047f770 ldr r3, [r3]
0047f774 mov lr, pc
0047f778 ldr pc, [r3, #0x2c]
0047f77c add r0, r5, #0x2c
0047f780 mov r1, r4
0047f784 pop {r4, r5, r6, lr}
0047f788 b #0x47a76c

SOURCE 0x46c658 _ZN13QuestSavegame10PackQuestsEiP11IStreamBase
0046c658 push {r4, r5, r6, r7, lr}
0046c65c mov r6, r0
0046c660 mov r0, #0xc
0046c664 mla r0, r0, r1, r6
0046c668 sub sp, sp, #0xc
0046c66c ldmib r0, {r3, ip}
0046c670 mov r7, r1
0046c674 add r1, sp, #8
0046c678 rsb r3, r3, ip
0046c67c asr r3, r3, #2
0046c680 str r3, [r1, #-4]!
0046c684 mov r0, r2
0046c688 mov r5, r2
0046c68c bl #0x461770
0046c690 ldr r3, [sp, #4]
0046c694 cmp r3, #0
0046c698 beq #0x46c6c4
0046c69c mov r4, #0
0046c6a0 mov r1, r4
0046c6a4 mov r3, r5
0046c6a8 mov r0, r6
0046c6ac mov r2, r7
0046c6b0 bl #0x46c584
0046c6b4 ldr r3, [sp, #4]
0046c6b8 add r4, r4, #1
0046c6bc cmp r3, r4
0046c6c0 bhi #0x46c6a0
0046c6c4 add r1, r6, r7, lsl #2
0046c6c8 mov r0, r5
0046c6cc add r1, r1, #0x2c
0046c6d0 bl #0x38b808
0046c6d4 add r1, r7, #0xe
0046c6d8 add r1, r6, r1, lsl #2
0046c6dc mov r0, r5
0046c6e0 add r6, r6, r7, lsl #2
0046c6e4 bl #0x38b808
0046c6e8 mov r0, r5
0046c6ec add r1, r6, #0x44
0046c6f0 bl #0x38b808
0046c6f4 add sp, sp, #0xc
0046c6f8 pop {r4, r5, r6, r7, pc}

SOURCE 0x4699a0 _ZNKSt6bitsetILj64EE17_M_copy_to_stringIcSt11char_traitsIcESaIcEEEvRSbIT_T0_T1_E
004699a0 push {r4, r5, r6, r7, r8, lr}
004699a4 ldr r6, [pc, #0x164]
004699a8 ldr r7, [pc, #0x164]
004699ac mov r5, r1
004699b0 add r6, pc, r6
004699b4 ldr r1, [r6, r7]
004699b8 ldr r3, [r5, #0x14]
004699bc ldr r2, [r5, #0x10]
004699c0 ldr r1, [r1]
004699c4 sub sp, sp, #0x20
004699c8 rsb r2, r3, r2
004699cc cmp r2, #0x3f
004699d0 mov r4, r0
004699d4 str r1, [sp, #0x1c]
004699d8 bhi #0x469aa0
004699dc cmp r3, r5
004699e0 beq #0x4699f8
004699e4 ldr r1, [r5]
004699e8 rsb r1, r3, r1
004699ec sub r1, r1, #1
004699f0 cmp r1, #0x40
004699f4 bhi #0x469ae0
004699f8 add r8, sp, #4
004699fc mov r0, r8
00469a00 mov r1, #0x41
00469a04 str r8, [sp, #0x14]
00469a08 str r8, [sp, #0x18]
00469a0c bl #0x31167c
00469a10 ldr r2, [sp, #0x18]
00469a14 mov r3, #0
00469a18 mov r1, #0x30
00469a1c add r0, r2, #0x40
00469a20 strb r1, [r2, r3]
00469a24 add r3, r3, #1
00469a28 cmp r3, #0x40
00469a2c bne #0x469a20
00469a30 mov r3, #0
00469a34 str r0, [sp, #0x14]
00469a38 mov r1, r8
00469a3c mov r0, r5
00469a40 strb r3, [r2, #0x40]
00469a44 bl #0x433880
00469a48 mov r0, r8
00469a4c bl #0x3139ac
00469a50 mov r3, #0
00469a54 mov r0, #1
00469a58 mov ip, #0x31
00469a5c lsr r2, r3, #5
00469a60 ldr r2, [r4, r2, lsl #2]
00469a64 and r1, r3, #0x1f
00469a68 ands r2, r2, r0, lsl r1
00469a6c ldrne r1, [r5, #0x14]
00469a70 rsbne r2, r3, #0x3f
00469a74 add r3, r3, #1
00469a78 strbne ip, [r1, r2]
00469a7c cmp r3, #0x40
00469a80 bne #0x469a5c
00469a84 ldr r3, [r6, r7]
00469a88 ldr r2, [sp, #0x1c]
00469a8c ldr r3, [r3]
00469a90 cmp r2, r3
00469a94 bne #0x469b0c
00469a98 add sp, sp, #0x20
00469a9c pop {r4, r5, r6, r7, r8, pc}
00469aa0 mov r0, r3
00469aa4 mov r1, #0x30
00469aa8 mov r2, #0x40
00469aac bl #0x30e460
00469ab0 ldr r2, [r5, #0x14]
00469ab4 ldr r3, [r5, #0x10]
00469ab8 add r1, r2, #0x40
00469abc cmp r1, r3
00469ac0 beq #0x469a50
00469ac4 ldrb r0, [r3]
00469ac8 rsb r3, r3, r1
00469acc strb r0, [r2, #0x40]
00469ad0 ldr r2, [r5, #0x10]
00469ad4 add r3, r2, r3
00469ad8 str r3, [r5, #0x10]
00469adc b #0x469a50
00469ae0 mov r0, r3
00469ae4 mov r1, #0x30
00469ae8 bl #0x30e460
00469aec ldr r1, [r5, #0x10]
00469af0 ldr r3, [r5, #0x14]
00469af4 mov r0, r5
00469af8 mov r2, #0x30
00469afc rsb r1, r3, r1
00469b00 rsb r1, r1, #0x40
00469b04 bl #0x32a388
00469b08 b #0x469a50
00469b0c bl #0x30e310
00469b10 subseq fp, r2, r0, ror #1
00469b14 andeq r4, r0, ip, lsr #1

SOURCE 0x47a76c _ZN13ObjectiveList9_saveDataEP11IStreamBase
0047a76c str r4, [sp, #-4]!
0047a770 mov ip, #1
0047a774 mov r4, #0x2c
0047a778 sub sp, sp, #0xc
0047a77c mov r3, r1
0047a780 mov r2, ip
0047a784 mov r1, r4
0047a788 stm sp, {r4, ip}
0047a78c add sp, sp, #0xc
0047a790 ldm sp!, {r4}
0047a794 b #0x47a6f8
