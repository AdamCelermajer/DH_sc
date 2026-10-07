# _ZN12TriggerPlate6UpdateEv
0039a668 push     {r4, r5, r6, r7, r8, sb, sl, lr}
0039a66c ldrb     r3, [r0, #0x779]
0039a670 ldr      r5, [pc, #0x3b4]
0039a674 sub      sp, sp, #0x48
0039a678 cmp      r3, #0
0039a67c mov      r4, r0
0039a680 add      r5, pc, r5
0039a684 beq      #0x39a6f4
0039a688 ldrb     r3, [r0, #0x778]
0039a68c cmp      r3, #0
0039a690 bne      #0x39a6fc
0039a694 mov      r0, r4
0039a698 bl       #0x3980a8
0039a69c ldr      r3, [r4]
0039a6a0 mov      r0, r4
0039a6a4 mov      lr, pc
0039a6a8 ldr      pc, [r3, #0xe8]
0039a6ac cmp      r0, #2
0039a6b0 beq      #0x39a83c
0039a6b4 bl       #0x7fd794
0039a6b8 ldrb     r3, [r0, #5]
0039a6bc cmp      r3, #0
0039a6c0 bne      #0x39a714
0039a6c4 mov      r0, r4
0039a6c8 bl       #0x39a5ec
0039a6cc mov      r6, r0
0039a6d0 str      r0, [r4, #0x3c0]
0039a6d4 ldr      r3, [r4, #0x734]
0039a6d8 cmp      r3, r6
0039a6dc ble      #0x39a734
0039a6e0 mov      r0, r4
0039a6e4 bl       #0x3987fc
0039a6e8 ldr      r3, [r4, #0x3b8]
0039a6ec cmp      r3, #0
0039a6f0 ble      #0x39a734
0039a6f4 add      sp, sp, #0x48
0039a6f8 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0039a6fc ldr      r3, [r0]
0039a700 mov      lr, pc
0039a704 ldr      pc, [r3, #0xec]
0039a708 cmp      r0, #5
0039a70c bne      #0x39a694
0039a710 b        #0x39a6f4
0039a714 ldr      r3, [r4]
0039a718 mov      r0, r4
0039a71c mov      lr, pc
0039a720 ldr      pc, [r3, #0x54]
0039a724 cmp      r0, #0
0039a728 ldrne    r6, [r4, #0x3c0]
0039a72c bne      #0x39a6d4
0039a730 b        #0x39a6c4
0039a734 mov      r0, r4
0039a738 bl       #0x3987ac
0039a73c cmp      r0, #0
0039a740 beq      #0x39a830
0039a744 ldr      r3, [r4, #0x774]
0039a748 cmp      r3, r6
0039a74c beq      #0x39a830
0039a750 ldr      r2, [r4, #0x734]
0039a754 cmp      r2, r6
0039a758 ble      #0x39a854
0039a75c cmp      r3, r2
0039a760 blt      #0x39a82c
0039a764 ldr      r3, [r4, #0x730]
0039a768 cmn      r3, #1
0039a76c beq      #0x39a7d0
0039a770 ldr      r2, [pc, #0x2b8]
0039a774 ldr      r3, [r4]
0039a778 mov      r0, r4
0039a77c ldr      r2, [r5, r2]
0039a780 ldr      sl, [r2]
0039a784 mov      lr, pc
0039a788 ldr      pc, [r3, #0xe4]
0039a78c ldr      lr, [r4, #0x168]
0039a790 ldr      r7, [r4, #0x164]
0039a794 ldr      r8, [r4, #0x160]
0039a798 mov      ip, #0xbf000000
0039a79c add      ip, ip, #0x800000
0039a7a0 mov      r1, r0
0039a7a4 str      lr, [sp, #0x44]
0039a7a8 mov      r0, sl
0039a7ac mov      lr, #1
0039a7b0 add      r2, sp, #0x3c
0039a7b4 mov      r3, #0
0039a7b8 str      r8, [sp, #0x3c]
0039a7bc str      r7, [sp, #0x40]
0039a7c0 str      lr, [sp]
0039a7c4 str      ip, [sp, #8]
0039a7c8 str      ip, [sp, #4]
0039a7cc bl       #0x36b5d8
0039a7d0 ldr      r3, [r4, #0x2d8]
0039a7d4 cmp      r3, #0
0039a7d8 beq      #0x39a804
0039a7dc ldr      ip, [r3, #0x38]
0039a7e0 ldr      r1, [pc, #0x24c]
0039a7e4 mov      r3, #0
0039a7e8 mov      r0, ip
0039a7ec mov      r2, r3
0039a7f0 ldr      ip, [ip]
0039a7f4 add      r1, pc, r1
0039a7f8 str      r3, [sp]
0039a7fc mov      lr, pc
0039a800 ldr      pc, [ip, #0x20]
0039a804 ldr      r1, [r4, #0x770]
0039a808 cmn      r1, #1
0039a80c beq      #0x39a824
0039a810 ldr      r3, [pc, #0x220]
0039a814 ldr      r2, [r4, #0x64]
0039a818 ldr      r0, [r5, r3]
0039a81c mov      r3, #0
0039a820 bl       #0x4605c0
0039a824 mov      r3, #0
0039a828 strb     r3, [r4, #0x778]
0039a82c str      r6, [r4, #0x774]
0039a830 mov      r0, r4
0039a834 bl       #0x38b8b8
0039a838 b        #0x39a6f4
0039a83c ldr      r3, [pc, #0x1f8]
0039a840 ldr      r3, [r5, r3]
0039a844 ldr      r0, [r3, #0x40]
0039a848 bl       #0x36d7a8
0039a84c str      r0, [r4, #0x734]
0039a850 b        #0x39a6b4
0039a854 cmp      r3, r2
0039a858 bge      #0x39a82c
0039a85c ldrb     r3, [r4, #0x778]
0039a860 cmp      r3, #0
0039a864 bne      #0x39a82c
0039a868 ldr      r7, [pc, #0x1cc]
0039a86c mov      r0, r4
0039a870 bl       #0x398794
0039a874 ldr      r0, [r5, r7]
0039a878 bl       #0x31f594
0039a87c subs     r8, r0, #0
0039a880 beq      #0x39a9d8
0039a884 ldr      r3, [r4]
0039a888 mov      r0, r4
0039a88c mov      lr, pc
0039a890 ldr      pc, [r3, #0xd8]
0039a894 ldr      r3, [r5, r7]
0039a898 ldr      r1, [pc, #0x1a0]
0039a89c ldr      r2, [pc, #0x1a0]
0039a8a0 mov      sl, r0
0039a8a4 add      r1, pc, r1
0039a8a8 add      r2, pc, r2
0039a8ac ldr      r0, [r3, #0x2c]
0039a8b0 ldr      r7, [r4, #0x64]
0039a8b4 bl       #0x4c4bdc
0039a8b8 ldr      r3, [pc, #0x188]
0039a8bc add      r1, sp, #0x48
0039a8c0 str      r0, [sp, #0x18]
0039a8c4 ldr      r3, [r5, r3]
0039a8c8 mov      r0, r8
0039a8cc mov      r8, #0
0039a8d0 add      r3, r3, #8
0039a8d4 str      r3, [r1, #-0x34]!
0039a8d8 mvn      r3, #0
0039a8dc str      r3, [sp, #0x28]
0039a8e0 str      r7, [sp, #0x20]
0039a8e4 str      sl, [sp, #0x2c]
0039a8e8 str      r8, [sp, #0x1c]
0039a8ec strb     r8, [sp, #0x24]
0039a8f0 strb     r8, [sp, #0x25]
0039a8f4 bl       #0x339090
0039a8f8 ldr      r3, [pc, #0x14c]
0039a8fc ldr      r2, [r4, #0x730]
0039a900 ldr      r3, [r5, r3]
0039a904 cmn      r2, #1
0039a908 add      r3, r3, #8
0039a90c str      r3, [sp, #0x14]
0039a910 beq      #0x39a974
0039a914 ldr      r2, [pc, #0x114]
0039a918 ldr      r3, [r4]
0039a91c mov      r0, r4
0039a920 ldr      r2, [r5, r2]
0039a924 ldr      sb, [r2]
0039a928 mov      lr, pc
0039a92c ldr      pc, [r3, #0xe0]
0039a930 ldr      lr, [r4, #0x164]
0039a934 ldr      r7, [r4, #0x168]
0039a938 ldr      sl, [r4, #0x160]
0039a93c mov      ip, #0xbf000000
0039a940 add      ip, ip, #0x800000
0039a944 mov      r1, r0
0039a948 str      lr, [sp, #0x34]
0039a94c mov      r0, sb
0039a950 mov      lr, #1
0039a954 mov      r3, r8
0039a958 add      r2, sp, #0x30
0039a95c str      sl, [sp, #0x30]
0039a960 str      r7, [sp, #0x38]
0039a964 str      lr, [sp]
0039a968 str      ip, [sp, #8]
0039a96c str      ip, [sp, #4]
0039a970 bl       #0x36b5d8
0039a974 ldr      r3, [r4, #0x2d8]
0039a978 cmp      r3, #0
0039a97c beq      #0x39a9a8
0039a980 ldr      ip, [r3, #0x38]
0039a984 ldr      r1, [pc, #0xc4]
0039a988 mov      r3, #0
0039a98c mov      r0, ip
0039a990 mov      r2, r3
0039a994 ldr      ip, [ip]
0039a998 add      r1, pc, r1
0039a99c str      r3, [sp]
0039a9a0 mov      lr, pc
0039a9a4 ldr      pc, [ip, #0x20]
0039a9a8 ldr      r1, [r4, #0x76c]
0039a9ac cmn      r1, #1
0039a9b0 beq      #0x39a9c8
0039a9b4 ldr      r3, [pc, #0x7c]
0039a9b8 ldr      r2, [r4, #0x64]
0039a9bc ldr      r0, [r5, r3]
0039a9c0 mov      r3, #0
0039a9c4 bl       #0x4605c0
0039a9c8 mov      r3, #1
0039a9cc strb     r3, [r4, #0x778]
0039a9d0 str      r6, [r4, #0x774]
0039a9d4 b        #0x39a830
0039a9d8 ldr      r3, [pc, #0x74]
0039a9dc ldr      r3, [r5, r3]
0039a9e0 ldr      r3, [r3]
0039a9e4 cmp      r3, #2
0039a9e8 streq    r8, [r8]
0039a9ec beq      #0x39a884
0039a9f0 cmp      r3, #1
0039a9f4 bne      #0x39a884
0039a9f8 ldr      r0, [pc, #0x58]
0039a9fc ldr      r1, [pc, #0x58]
0039aa00 ldr      r2, [pc, #0x58]
0039aa04 ldr      r0, [r5, r0]
0039aa08 ldr      r3, [pc, #0x54]
0039aa0c mov      ip, #0xe6
0039aa10 add      r1, pc, r1
0039aa14 add      r2, pc, r2
0039aa18 add      r3, pc, r3
0039aa1c add      r0, r0, #0xa8
0039aa20 str      ip, [sp]
0039aa24 bl       #0x30e004
0039aa28 b        #0x39a884
0039aa2c subseq   sl, pc, r0, lsl r4
0039aa30 andeq    r0, r0, r4, lsr #27
0039aa34 subseq   r8, r2, r4, lsr #7
0039aa38 andeq    r1, r0, r0, lsr #20
0039aa3c strdeq   r3, r4, [r0], -r4
0039aa40 subseq   r8, r2, r4, asr #1
0039aa44 subseq   r5, r2, r0, asr ip
0039aa48 andeq    r0, r0, r4, lsr #19
0039aa4c strheq   r0, [r0], -r0
0039aa50 subseq   r8, r2, r0, asr #2
0039aa54 andeq    r3, r0, r0, asr #19
0039aa58 andeq    r1, r0, r0, asr #19
0039aa5c subseq   r3, r2, r8, asr #19
0039aa60 subseq   r4, r7, r4, asr #30
