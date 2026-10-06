_ZNK6glitch5video9CMaterialeqERKS1_ 0x3537b0 52
003537b0 push {r4, r5, r6, lr}
003537b4 mov r4, r1
003537b8 mov r6, r0
003537bc bl #0x5c5d34
003537c0 mov r5, r0
003537c4 mov r0, r4
003537c8 bl #0x5c5d34
003537cc mov r1, r5
003537d0 mov r3, r0
003537d4 mov r2, r4
003537d8 mov r0, r6
003537dc pop {r4, r5, r6, lr}
003537e0 b #0x353684
_ZNK6glitch5video9CMaterial6equalsEhRKS1_h 0x353684 300
00353684 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00353688 ldr ip, [r0, #0x10]
0035368c sub sp, sp, #0x14
00353690 mov r6, r0
00353694 lsr ip, ip, r1
00353698 tst ip, #1
0035369c mov r7, r1
003536a0 mov r4, r2
003536a4 mov r5, r3
003536a8 beq #0x3536b0
003536ac bl #0x5c5fd8
003536b0 ldr r2, [r4, #0x10]
003536b4 ldr r3, [r6, #0x18]
003536b8 lsr r2, r2, r5
003536bc tst r2, #1
003536c0 ldr r8, [r3, r7, lsl #2]
003536c4 beq #0x3536d4
003536c8 mov r0, r4
003536cc mov r1, r5
003536d0 bl #0x5c5fd8
003536d4 ldr r3, [r4, #0x18]
003536d8 ldr r3, [r3, r5, lsl #2]
003536dc cmp r8, r3
003536e0 beq #0x3536f0
003536e4 mov r0, #0
003536e8 add sp, sp, #0x14
003536ec pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003536f0 ldr r2, [r6, #4]
003536f4 ldr r1, [r4, #4]
003536f8 mov r3, #0xc
003536fc ldr r2, [r2, #0x18]
00353700 ldr r1, [r1, #0x18]
00353704 mla r2, r3, r7, r2
00353708 mla r3, r3, r5, r1
0035370c ldrb sb, [r2, #4]
00353710 ldrb r1, [r3, #4]
00353714 cmp r1, sb
00353718 bne #0x3536e4
0035371c cmp sb, #0
00353720 beq #0x353794
00353724 ldr r8, [r2, #8]
00353728 ldr sl, [r3, #8]
0035372c ldr r2, [r8, #0x20]
00353730 ldr r3, [sl, #0x20]
00353734 cmp r2, r3
00353738 bne #0x3536e4
0035373c sub r3, sb, #1
00353740 uxtb r3, r3
00353744 mov fp, #0x34
00353748 mla r3, r3, fp, fp
0035374c mov r1, sl
00353750 str r3, [sp, #0xc]
00353754 mov r0, r8
00353758 mov r2, #0x20
0035375c bl #0x30e5e0
00353760 cmp r0, #0
00353764 bne #0x3536e4
00353768 ldr r3, [sp, #0xc]
0035376c add r0, r8, fp
00353770 add r1, sl, fp
00353774 cmp fp, r3
00353778 beq #0x353794
0035377c ldr r2, [r0, #0x20]
00353780 ldr r3, [r1, #0x20]
00353784 add fp, fp, #0x34
00353788 cmp r2, r3
0035378c bne #0x3536e4
00353790 b #0x353758
00353794 mov r0, r6
00353798 mov r1, r7
0035379c mov r2, sb
003537a0 mov r3, r4
003537a4 str r5, [sp]
003537a8 bl #0x5ca72c
003537ac b #0x3536e8
_ZNK6glitch5video9CMaterial18areParametersEqualEhhRKS1_h 0x5ca72c 876
005ca72c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005ca730 ldr r5, [pc, #0x354]
005ca734 sub sp, sp, #0x44
005ca738 cmp r2, #0
005ca73c add r5, pc, r5
005ca740 str r5, [sp, #8]
005ca744 ldrb r4, [sp, #0x68]
005ca748 beq #0x5caa84
005ca74c ldr r7, [r0, #4]
005ca750 sub r2, r2, #1
005ca754 add r0, r0, #0x20
005ca758 str r7, [sp, #0x14]
005ca75c ldr ip, [r3, #4]
005ca760 add r3, r3, #0x20
005ca764 str ip, [sp, #0x10]
005ca768 ldr r6, [r7, #0x18]
005ca76c ldr r7, [sp, #0x10]
005ca770 mov ip, #0xc
005ca774 mla r1, ip, r1, r6
005ca778 ldr r5, [r7, #0x18]
005ca77c mla ip, ip, r4, r5
005ca780 uxtb r4, r2
005ca784 mov r2, #0x34
005ca788 mla r2, r4, r2, r2
005ca78c str r2, [sp, #0x3c]
005ca790 ldr r1, [r1, #8]
005ca794 mov r2, #0
005ca798 str r1, [sp, #0x38]
005ca79c ldr ip, [ip, #8]
005ca7a0 ldr r1, [pc, #0x2e8]
005ca7a4 str r0, [sp, #0x24]
005ca7a8 str ip, [sp, #0x34]
005ca7ac ldr ip, [pc, #0x2e0]
005ca7b0 str r1, [sp, #0x1c]
005ca7b4 str r3, [sp, #0x20]
005ca7b8 str ip, [sp, #0x2c]
005ca7bc str r2, [sp, #0x30]
005ca7c0 ldr r3, [sp, #0x38]
005ca7c4 ldr r5, [sp, #0x30]
005ca7c8 ldr r7, [sp, #0x34]
005ca7cc add r2, r3, r5
005ca7d0 ldr r3, [r2, #0x20]
005ca7d4 ldr r2, [r2, #0x24]
005ca7d8 str r2, [sp, #4]
005ca7dc ldrh ip, [r3, #0x36]
005ca7e0 ldrh r0, [r3, #0x2e]
005ca7e4 ldrh r1, [r3, #0x2c]
005ca7e8 ldrh r3, [r3, #0x34]
005ca7ec add r0, ip, r0
005ca7f0 uxth r0, r0
005ca7f4 rsb r1, r1, r0
005ca7f8 ldr ip, [sp, #4]
005ca7fc rsb r3, r3, r1
005ca800 add r2, r7, r5
005ca804 uxth r3, r3
005ca808 ldr r2, [r2, #0x24]
005ca80c add r3, ip, r3, lsl #1
005ca810 cmp r3, ip
005ca814 str r2, [sp, #0x18]
005ca818 str r3, [sp, #0xc]
005ca81c beq #0x5caa6c
005ca820 mov r4, #0
005ca824 b #0x5ca878
005ca828 ldr r7, [sp, #8]
005ca82c ldr r5, [sp, #0x2c]
005ca830 ldr r0, [r3, #0xc]
005ca834 ldr lr, [sp, #0x20]
005ca838 ldr ip, [r7, r5]
005ca83c ldr r1, [r1, #0xc]
005ca840 ldrb r2, [ip, r2]
005ca844 ldr ip, [sp, #0x24]
005ca848 add r1, lr, r1
005ca84c mul r2, sl, r2
005ca850 add r0, ip, r0
005ca854 bl #0x30e5e0
005ca858 cmp r0, #0
005ca85c bne #0x5caa04
005ca860 ldr r0, [sp, #4]
005ca864 ldr r1, [sp, #0xc]
005ca868 add r4, r4, #2
005ca86c add r3, r0, r4
005ca870 cmp r1, r3
005ca874 beq #0x5caa6c
005ca878 ldr r0, [sp, #4]
005ca87c ldrh r3, [r0, r4]
005ca880 tst r3, #0x8000
005ca884 bne #0x5ca860
005ca888 ldr r1, [sp, #0x18]
005ca88c ldrh r2, [r1, r4]
005ca890 tst r2, #0x8000
005ca894 bne #0x5ca860
005ca898 ldr r5, [sp, #0x14]
005ca89c ldr ip, [sp, #0x10]
005ca8a0 ldrh r1, [r5, #0xe]
005ca8a4 cmp r1, r3
005ca8a8 ldrhi r7, [sp, #0x14]
005ca8ac movls r3, #0
005ca8b0 ldrhi r1, [r7, #0x20]
005ca8b4 addhi r3, r1, r3, lsl #4
005ca8b8 ldrh r1, [ip, #0xe]
005ca8bc cmp r1, r2
005ca8c0 ldrhi r0, [sp, #0x10]
005ca8c4 movls r1, #0
005ca8c8 ldrhi r1, [r0, #0x20]
005ca8cc addhi r1, r1, r2, lsl #4
005ca8d0 ldrb r0, [r1, #6]
005ca8d4 ldrb r2, [r3, #6]
005ca8d8 cmp r2, r0
005ca8dc bne #0x5caa04
005ca8e0 cmp r2, #0xb
005ca8e4 ldr sl, [r3, #8]
005ca8e8 bne #0x5ca828
005ca8ec ldr r8, [r3, #0xc]
005ca8f0 ldr sb, [r1, #0xc]
005ca8f4 ldr r1, [sp, #0x24]
005ca8f8 add r8, r1, r8
005ca8fc add sl, r8, sl, lsl #2
005ca900 cmp r8, sl
005ca904 beq #0x5ca860
005ca908 ldr r5, [sp, #8]
005ca90c ldr r2, [sp, #0x1c]
005ca910 ldr r7, [sp, #0x20]
005ca914 ldr r3, [r5, r2]
005ca918 add sb, r7, sb
005ca91c mov r7, #0
005ca920 ldrb r3, [r3, #0x40]
005ca924 str r3, [sp, #0x28]
005ca928 ldr r6, [r8, r7]
005ca92c cmp r6, #0
005ca930 beq #0x5ca998
005ca934 ldr fp, [sb, r7]
005ca938 cmp fp, #0
005ca93c beq #0x5caa10
005ca940 ldrb r3, [r6, #0x40]
005ca944 cmp r3, #0
005ca948 beq #0x5ca958
005ca94c ldrb r3, [fp, #0x40]
005ca950 cmp r3, #0
005ca954 bne #0x5ca97c
005ca958 mov r5, #0
005ca95c ldr r0, [r6, r5]
005ca960 ldr r1, [fp, r5]
005ca964 bl #0x30df8c
005ca968 cmp r0, #0
005ca96c add r5, r5, #4
005ca970 beq #0x5caa04
005ca974 cmp r5, #0x40
005ca978 bne #0x5ca95c
005ca97c add r7, r7, #4
005ca980 add r3, r8, r7
005ca984 cmp sl, r3
005ca988 beq #0x5ca860
005ca98c ldr r6, [r8, r7]
005ca990 cmp r6, #0
005ca994 bne #0x5ca934
005ca998 ldr r6, [sb, r7]
005ca99c cmp r6, #0
005ca9a0 beq #0x5ca97c
005ca9a4 ldrb r3, [r6, #0x40]
005ca9a8 cmp r3, #0
005ca9ac beq #0x5ca9bc
005ca9b0 ldr r1, [sp, #0x28]
005ca9b4 cmp r1, #0
005ca9b8 bne #0x5ca97c
005ca9bc ldr r3, [sp, #8]
005ca9c0 ldr r2, [sp, #0x1c]
005ca9c4 mov r5, #0
005ca9c8 ldr r0, [r6, r5]
005ca9cc ldr fp, [r3, r2]
005ca9d0 ldr r1, [r5, fp]
005ca9d4 bl #0x30df8c
005ca9d8 cmp r0, #0
005ca9dc add r5, r5, #4
005ca9e0 beq #0x5caa04
005ca9e4 cmp r5, #0x40
005ca9e8 beq #0x5ca97c
005ca9ec ldr r0, [r6, r5]
005ca9f0 ldr r1, [r5, fp]
005ca9f4 bl #0x30df8c
005ca9f8 cmp r0, #0
005ca9fc add r5, r5, #4
005caa00 bne #0x5ca9e4
005caa04 mov r0, #0
005caa08 add sp, sp, #0x44
005caa0c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005caa10 ldrb r3, [r6, #0x40]
005caa14 cmp r3, #0
005caa18 beq #0x5caa28
005caa1c ldr ip, [sp, #0x28]
005caa20 cmp ip, #0
005caa24 bne #0x5ca97c
005caa28 ldr r0, [sp, #8]
005caa2c ldr lr, [sp, #0x1c]
005caa30 mov r5, #0
005caa34 ldr fp, [r0, lr]
005caa38 ldr r0, [r6, r5]
005caa3c ldr r1, [r5, fp]
005caa40 bl #0x30df8c
005caa44 cmp r0, #0
005caa48 add r5, r5, #4
005caa4c beq #0x5caa04
005caa50 cmp r5, #0x40
005caa54 bne #0x5caa38
005caa58 add r7, r7, #4
005caa5c add r3, r8, r7
005caa60 cmp sl, r3
005caa64 bne #0x5ca98c
005caa68 b #0x5ca860
005caa6c ldr r2, [sp, #0x30]
005caa70 ldr r3, [sp, #0x3c]
005caa74 add r2, r2, #0x34
005caa78 cmp r2, r3
005caa7c str r2, [sp, #0x30]
005caa80 bne #0x5ca7c0
005caa84 mov r0, #1
005caa88 b #0x5caa08
005caa8c eorseq sl, ip, r4, asr r3
005caa90 andeq r2, r0, r0, lsr r8
005caa94 andeq r1, r0, r0, asr #11
_ZNK6glitch5video9CMaterialltERKS1_ 0x3537e4 200
003537e4 push {r4, r5, r6, r7, r8, lr}
003537e8 sub sp, sp, #8
003537ec mov r5, r1
003537f0 mov r7, r0
003537f4 bl #0x5c5d34
003537f8 mov r6, r0
003537fc mov r0, r5
00353800 bl #0x5c5d34
00353804 ldr r3, [r7, #0x10]
00353808 mov r4, r0
0035380c lsr r3, r3, r6
00353810 tst r3, #1
00353814 beq #0x353824
00353818 mov r0, r7
0035381c mov r1, r6
00353820 bl #0x5c5fd8
00353824 ldr r2, [r5, #0x10]
00353828 ldr r3, [r7, #0x18]
0035382c lsr r2, r2, r4
00353830 tst r2, #1
00353834 ldr r8, [r3, r6, lsl #2]
00353838 beq #0x353848
0035383c mov r0, r5
00353840 mov r1, r4
00353844 bl #0x5c5fd8
00353848 ldr r3, [r5, #0x18]
0035384c ldr r3, [r3, r4, lsl #2]
00353850 cmp r8, r3
00353854 beq #0x353868
00353858 movhs r0, #0
0035385c movlo r0, #1
00353860 add sp, sp, #8
00353864 pop {r4, r5, r6, r7, r8, pc}
00353868 ldr r2, [r7, #4]
0035386c ldr r1, [r5, #4]
00353870 mov r3, #0xc
00353874 ldr r2, [r2, #0x18]
00353878 ldr r1, [r1, #0x18]
0035387c mla r2, r3, r6, r2
00353880 mla r3, r3, r4, r1
00353884 ldrb r2, [r2, #4]
00353888 ldrb r3, [r3, #4]
0035388c cmp r2, r3
00353890 bne #0x353858
00353894 mov r0, r7
00353898 mov r1, r6
0035389c mov r3, r5
003538a0 str r4, [sp]
003538a4 bl #0x5ca3fc
003538a8 b #0x353860
_ZNK6glitch5video9CMaterial7compareEhhRKS1_h 0x5ca3fc 816
005ca3fc push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005ca400 ldr r5, [pc, #0x318]
005ca404 sub sp, sp, #0x3c
005ca408 cmp r2, #0
005ca40c add r5, pc, r5
005ca410 str r5, [sp, #4]
005ca414 ldrb r4, [sp, #0x60]
005ca418 ldr sl, [r0, #4]
005ca41c ldr r8, [r3, #4]
005ca420 beq #0x5ca67c
005ca424 ldr r6, [sl, #0x18]
005ca428 mov ip, #0xc
005ca42c ldr r5, [r8, #0x18]
005ca430 mla r1, ip, r1, r6
005ca434 mla ip, ip, r4, r5
005ca438 ldr r1, [r1, #8]
005ca43c str r1, [sp, #0x14]
005ca440 ldr ip, [ip, #8]
005ca444 str ip, [sp, #0x10]
005ca448 ldr r4, [ip, #0x20]
005ca44c ldr r1, [r1, #0x20]
005ca450 ldrh r4, [r4, #0x40]
005ca454 ldrh ip, [r1, #0x40]
005ca458 cmp r4, ip
005ca45c bhi #0x5ca5f8
005ca460 blo #0x5ca67c
005ca464 sub r2, r2, #1
005ca468 mov ip, #0x34
005ca46c uxtb r2, r2
005ca470 mla r2, r2, ip, ip
005ca474 add r0, r0, #0x20
005ca478 str r0, [sp, #8]
005ca47c str r2, [sp, #0x2c]
005ca480 str ip, [sp, #0x24]
005ca484 ldr fp, [sp, #0x10]
005ca488 ldr ip, [sp, #0x14]
005ca48c ldr r0, [pc, #0x290]
005ca490 ldr r2, [pc, #0x290]
005ca494 add r3, r3, #0x20
005ca498 str r3, [sp, #0xc]
005ca49c str fp, [sp, #0x18]
005ca4a0 str ip, [sp, #0x1c]
005ca4a4 str r0, [sp, #0x28]
005ca4a8 str r2, [sp, #0x20]
005ca4ac ldrh r0, [r1, #0x36]
005ca4b0 ldrh r2, [r1, #0x2e]
005ca4b4 ldrh r3, [r1, #0x34]
005ca4b8 ldr ip, [sp, #0x1c]
005ca4bc add r2, r0, r2
005ca4c0 ldrh r1, [r1, #0x2c]
005ca4c4 uxth r2, r2
005ca4c8 rsb r7, r3, r2
005ca4cc ldr r6, [ip, #0x24]
005ca4d0 rsb r7, r1, r7
005ca4d4 uxth r7, r7
005ca4d8 ldr r0, [sp, #0x18]
005ca4dc add r7, r6, r7, lsl #1
005ca4e0 cmp r7, r6
005ca4e4 ldr sb, [r0, #0x24]
005ca4e8 moveq r0, #0
005ca4ec beq #0x5ca600
005ca4f0 mov r4, #0
005ca4f4 mov r0, r4
005ca4f8 b #0x5ca550
005ca4fc cmp r0, #0
005ca500 bne #0x5ca540
005ca504 ldrb ip, [r2, #6]
005ca508 cmp ip, #0xb
005ca50c beq #0x5ca688
005ca510 ldr r0, [sp, #0x28]
005ca514 ldr r5, [sp, #4]
005ca518 ldr r1, [r1, #0xc]
005ca51c ldr fp, [sp, #8]
005ca520 ldr lr, [r5, r0]
005ca524 ldr r0, [r2, #0xc]
005ca528 ldrb r2, [lr, ip]
005ca52c ldr ip, [sp, #0xc]
005ca530 add r0, fp, r0
005ca534 mul r2, r3, r2
005ca538 add r1, ip, r1
005ca53c bl #0x30e5e0
005ca540 add r4, r4, #2
005ca544 add r3, r6, r4
005ca548 cmp r7, r3
005ca54c beq #0x5ca600
005ca550 ldrh r3, [r6, r4]
005ca554 tst r3, #0x8000
005ca558 bne #0x5ca540
005ca55c ldrh r1, [sb, r4]
005ca560 tst r1, #0x8000
005ca564 bne #0x5ca540
005ca568 ldrh r2, [sl, #0xe]
005ca56c cmp r2, r3
005ca570 ldrhi r2, [sl, #0x20]
005ca574 movls r2, #0
005ca578 addhi r2, r2, r3, lsl #4
005ca57c ldrh r3, [r8, #0xe]
005ca580 ldrh ip, [r2, #4]
005ca584 cmp r3, r1
005ca588 ldrhi r3, [r8, #0x20]
005ca58c movls r1, #0
005ca590 addhi r1, r3, r1, lsl #4
005ca594 cmp ip, #2
005ca598 ldr r3, [r2, #8]
005ca59c bne #0x5ca4fc
005ca5a0 ldr ip, [r2, #0xc]
005ca5a4 ldr r5, [sp, #8]
005ca5a8 ldr r1, [r1, #0xc]
005ca5ac add r2, r5, ip
005ca5b0 add r3, r2, r3, lsl #2
005ca5b4 cmp r2, r3
005ca5b8 beq #0x5ca540
005ca5bc ldr fp, [sp, #0xc]
005ca5c0 ldr r5, [r5, ip]
005ca5c4 ldr ip, [fp, r1]
005ca5c8 add r1, fp, r1
005ca5cc cmp r5, ip
005ca5d0 blo #0x5ca5f8
005ca5d4 bhi #0x5ca67c
005ca5d8 add r2, r2, #4
005ca5dc cmp r3, r2
005ca5e0 beq #0x5ca540
005ca5e4 ldr ip, [r1, #4]
005ca5e8 ldr r5, [r2]
005ca5ec add r1, r1, #4
005ca5f0 cmp r5, ip
005ca5f4 bhs #0x5ca5d4
005ca5f8 mov r0, #1
005ca5fc b #0x5ca680
005ca600 ldr r1, [sp, #0x1c]
005ca604 ldr r5, [sp, #0x18]
005ca608 ldrb r3, [r1]
005ca60c ldrb r2, [r5]
005ca610 rsb r3, r2, r3
005ca614 cmp r3, #0
005ca618 blt #0x5ca5f8
005ca61c bne #0x5ca67c
005ca620 cmp r0, #0
005ca624 blt #0x5ca5f8
005ca628 bne #0x5ca67c
005ca62c ldr fp, [sp, #0x24]
005ca630 ldr ip, [sp, #0x2c]
005ca634 cmp fp, ip
005ca638 beq #0x5ca67c
005ca63c ldr r5, [sp, #0x24]
005ca640 ldr r3, [sp, #0x14]
005ca644 ldr fp, [sp, #0x10]
005ca648 add r3, r3, r5
005ca64c add fp, fp, r5
005ca650 str r3, [sp, #0x1c]
005ca654 str fp, [sp, #0x18]
005ca658 ldr r1, [r3, #0x20]
005ca65c ldr r3, [fp, #0x20]
005ca660 ldrh r2, [r1, #0x40]
005ca664 ldrh r3, [r3, #0x40]
005ca668 cmp r2, r3
005ca66c blo #0x5ca5f8
005ca670 add r5, r5, #0x34
005ca674 str r5, [sp, #0x24]
005ca678 bls #0x5ca4ac
005ca67c mov r0, #0
005ca680 add sp, sp, #0x3c
005ca684 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005ca688 ldr fp, [r2, #0xc]
005ca68c ldr ip, [sp, #8]
005ca690 ldr r2, [r1, #0xc]
005ca694 add fp, ip, fp
005ca698 add r3, fp, r3, lsl #2
005ca69c cmp fp, r3
005ca6a0 beq #0x5ca540
005ca6a4 add r1, fp, #4
005ca6a8 rsb r3, r1, r3
005ca6ac ldr r1, [sp, #0xc]
005ca6b0 bic r3, r3, #3
005ca6b4 add r3, r3, #4
005ca6b8 add r2, r1, r2
005ca6bc str r4, [sp, #0x30]
005ca6c0 str r7, [sp, #0x34]
005ca6c4 mov r5, r0
005ca6c8 mov r7, r6
005ca6cc mov r4, r3
005ca6d0 mov r6, r2
005ca6d4 ldr r0, [fp, r5]
005ca6d8 ldr r1, [r6, r5]
005ca6dc mov r2, #0x44
005ca6e0 cmp r0, #0
005ca6e4 ldreq ip, [sp, #4]
005ca6e8 ldreq r3, [sp, #0x20]
005ca6ec add r5, r5, #4
005ca6f0 ldreq r0, [ip, r3]
005ca6f4 cmp r1, #0
005ca6f8 ldreq ip, [sp, #4]
005ca6fc ldreq r3, [sp, #0x20]
005ca700 ldreq r1, [ip, r3]
005ca704 bl #0x30e5e0
005ca708 cmp r5, r4
005ca70c bne #0x5ca6d4
005ca710 mov r6, r7
005ca714 ldr r4, [sp, #0x30]
005ca718 ldr r7, [sp, #0x34]
005ca71c b #0x5ca540
005ca720 eorseq sl, ip, r4, lsl #13
005ca724 andeq r1, r0, r0, asr #11
005ca728 andeq r2, r0, r0, lsr r8
