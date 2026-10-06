# _ZN13PlayerManager18ReviveLocalPlayersEb
0037565c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00375660 mov      r3, #0
00375664 sub      sp, sp, #0x64
00375668 str      r0, [sp, #0x20]
0037566c strb     r3, [r0, #0x711]
00375670 str      r3, [r0, #0x714]
00375674 str      r1, [sp, #0x2c]
00375678 bl       #0x7fd794
0037567c ldrb     r3, [r0, #5]
00375680 ldr      fp, [pc, #0x44c]
00375684 cmp      r3, #0
00375688 add      fp, pc, fp
0037568c beq      #0x375abc
00375690 ldr      r0, [pc, #0x440]
00375694 str      r0, [sp, #0x10]
00375698 ldr      r0, [sp, #0x10]
0037569c ldr      r3, [pc, #0x438]
003756a0 ldr      r1, [pc, #0x438]
003756a4 ldr      sl, [fp, r0]
003756a8 str      r3, [sp, #0x28]
003756ac ldr      r3, [fp, r3]
003756b0 add      r1, pc, r1
003756b4 mov      r0, sl
003756b8 ldr      r4, [r3]
003756bc bl       #0x320e44
003756c0 bl       #0x30e964
003756c4 mov      r1, #1
003756c8 mov      r2, r0
003756cc mov      r0, r4
003756d0 bl       #0x369da0
003756d4 ldr      r3, [pc, #0x408]
003756d8 add      r2, sp, #0x5c
003756dc ldr      r0, [sl, #0x40]
003756e0 add      r3, pc, r3
003756e4 str      r3, [sp, #0x14]
003756e8 ldr      r3, [pc, #0x3f8]
003756ec mov      r1, #0
003756f0 str      r2, [sp, #0x24]
003756f4 add      r3, pc, r3
003756f8 str      r3, [sp, #0x18]
003756fc str      fp, [sp, #0x1c]
00375700 bl       #0x36ead0
00375704 mov      r8, #0
00375708 cmp      r8, r0
0037570c add      r5, sp, #0x48
00375710 bge      #0x3758e8
00375714 ldr      r0, [sl, #0x40]
00375718 mov      r1, r8
0037571c mov      r2, #0
00375720 bl       #0x36e478
00375724 ldr      r6, [r0, #0x660]
00375728 cmp      r6, #0
0037572c beq      #0x3758d0
00375730 add      r0, r6, #0x560
00375734 bl       #0x3e0af8
00375738 ldr      r3, [sl, #0x38]
0037573c add      r4, sp, #0x54
00375740 str      r4, [sp, #0x54]
00375744 str      r4, [sp, #0x58]
00375748 ldr      r7, [r3, #0x14]
0037574c add      sb, r3, #0xc
00375750 mov      fp, #0xc
00375754 cmp      sb, r7
00375758 beq      #0x3757ec
0037575c ldr      r1, [r7, #0x2c]
00375760 cmp      r1, #0
00375764 beq      #0x3757c0
00375768 mov      r0, r5
0037576c bl       #0x33dd2c
00375770 mov      r0, r5
00375774 bl       #0x33ff54
00375778 subs     r3, r0, #0
0037577c beq      #0x3757c0
00375780 str      r3, [sp, #0xc]
00375784 bl       #0x3a3094
00375788 cmp      r0, #0
0037578c ldr      r3, [sp, #0xc]
00375790 beq      #0x375930
00375794 ldr      r0, [sp, #0x24]
00375798 str      r3, [sp, #0xc]
0037579c str      fp, [sp, #0x5c]
003757a0 bl       #0x708ec0
003757a4 ldr      r3, [sp, #0xc]
003757a8 str      r3, [r0, #8]
003757ac ldr      r3, [sp, #0x58]
003757b0 str      r4, [r0]
003757b4 str      r3, [r0, #4]
003757b8 str      r0, [r3]
003757bc str      r0, [sp, #0x58]
003757c0 ldr      r3, [r7, #0xc]
003757c4 cmp      r3, #0
003757c8 bne      #0x3757d4
003757cc b        #0x375948
003757d0 mov      r3, r2
003757d4 ldr      r2, [r3, #8]
003757d8 cmp      r2, #0
003757dc bne      #0x3757d0
003757e0 mov      r7, r3
003757e4 cmp      sb, r7
003757e8 bne      #0x37575c
003757ec bl       #0x7fd794
003757f0 ldrb     r3, [r0, #5]
003757f4 cmp      r3, #0
003757f8 bne      #0x375a1c
003757fc movw     r3, #0x1474
00375800 ldr      r3, [r6, r3]
00375804 ldr      r0, [sp, #0x1c]
00375808 ldr      r2, [pc, #0x2dc]
0037580c str      r3, [sp, #0x3c]
00375810 movw     r3, #0x1478
00375814 ldr      r3, [r6, r3]
00375818 add      sb, sp, #0x3c
0037581c ldr      r1, [r0, r2]
00375820 str      r3, [sp, #0x40]
00375824 movw     r3, #0x147c
00375828 ldr      r3, [r6, r3]
0037582c mov      r0, sb
00375830 str      r3, [sp, #0x44]
00375834 bl       #0x312b6c
00375838 cmp      r0, #0
0037583c beq      #0x37597c
00375840 movw     r3, #0x14a4
00375844 mov      r7, #0
00375848 str      r7, [r6, r3]
0037584c mov      r2, #1
00375850 mov      r0, r6
00375854 mov      r1, r7
00375858 bl       #0x3a59ac
0037585c add      r0, r6, #0x4f0
00375860 mov      r1, r7
00375864 add      r0, r0, #0xc
00375868 add      r6, r6, #0x37c
0037586c bl       #0x3c1a00
00375870 mov      r0, r6
00375874 bl       #0x3fc690
00375878 ldr      r2, [sp, #0x10]
0037587c ldr      r3, [sp, #0x1c]
00375880 mov      sb, r0
00375884 ldr      r1, [sp, #0x14]
00375888 ldr      r7, [r3, r2]
0037588c ldr      r2, [sp, #0x18]
00375890 ldr      r0, [r7, #0x2c]
00375894 bl       #0x4c4bdc
00375898 cmp      sb, r0
0037589c blt      #0x3759fc
003758a0 ldr      r0, [sp, #0x54]
003758a4 cmp      r0, r4
003758a8 bne      #0x3758b4
003758ac b        #0x3758c8
003758b0 mov      r0, r6
003758b4 ldr      r6, [r0]
003758b8 mov      r1, #0xc
003758bc bl       #0x708f00
003758c0 cmp      r6, r4
003758c4 bne      #0x3758b0
003758c8 str      r4, [sp, #0x58]
003758cc str      r4, [sp, #0x54]
003758d0 ldr      r0, [sl, #0x40]
003758d4 mov      r1, #0
003758d8 bl       #0x36ead0
003758dc add      r8, r8, #1
003758e0 cmp      r8, r0
003758e4 blt      #0x375714
003758e8 ldr      r0, [sp, #0x28]
003758ec ldr      fp, [sp, #0x1c]
003758f0 mov      r1, #2
003758f4 ldr      r4, [fp, r0]
003758f8 ldr      r0, [r4]
003758fc bl       #0x3698f0
00375900 mov      r0, sl
00375904 ldr      r4, [r4]
00375908 bl       #0x31f594
0037590c mov      ip, #0x3e8
00375910 ldr      r1, [r0, #0x11c]
00375914 mov      r2, #1
00375918 mov      r0, r4
0037591c mov      r3, #0
00375920 str      ip, [sp]
00375924 bl       #0x36bd78
00375928 add      sp, sp, #0x64
0037592c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00375930 ldr      r2, [r3, #0x418]
00375934 cmp      r6, r2
00375938 beq      #0x375794
0037593c ldr      r3, [r7, #0xc]
00375940 cmp      r3, #0
00375944 bne      #0x3757d4
00375948 ldr      r2, [r7, #4]
0037594c ldr      r1, [r2, #0xc]
00375950 cmp      r7, r1
00375954 bne      #0x375970
00375958 mov      r7, r2
0037595c ldr      r2, [r2, #4]
00375960 ldr      r3, [r2, #0xc]
00375964 cmp      r3, r7
00375968 beq      #0x375958
0037596c ldr      r3, [r7, #0xc]
00375970 cmp      r3, r2
00375974 movne    r7, r2
00375978 b        #0x375754
0037597c mov      r2, #1
00375980 mov      r0, r6
00375984 mov      r1, sb
00375988 bl       #0x393db4
0037598c mov      r1, sb
00375990 mov      r0, r6
00375994 bl       #0x393ae4
00375998 mov      r0, r6
0037599c bl       #0x3935dc
003759a0 ldr      r1, [r0]
003759a4 mov      r7, r0
003759a8 ldr      r0, [sp, #0x3c]
003759ac bl       #0x30eba4
003759b0 str      r0, [sp, #0x3c]
003759b4 ldr      r1, [r7, #4]
003759b8 ldr      r0, [sp, #0x40]
003759bc bl       #0x30eba4
003759c0 str      r0, [sp, #0x40]
003759c4 ldr      r1, [r7, #8]
003759c8 ldr      r0, [sp, #0x44]
003759cc bl       #0x30eba4
003759d0 ldr      r7, [sp, #0x54]
003759d4 str      r0, [sp, #0x44]
003759d8 b        #0x3759f0
003759dc ldr      r0, [r7, #8]
003759e0 mov      r1, sb
003759e4 mov      r2, #1
003759e8 bl       #0x393db4
003759ec ldr      r7, [r7]
003759f0 cmp      r7, r4
003759f4 bne      #0x3759dc
003759f8 b        #0x375840
003759fc ldr      r1, [sp, #0x14]
00375a00 ldr      r0, [r7, #0x2c]
00375a04 ldr      r2, [sp, #0x18]
00375a08 bl       #0x4c4bdc
00375a0c mov      r1, r0
00375a10 mov      r0, r6
00375a14 bl       #0x3ffc40
00375a18 b        #0x3758a0
00375a1c ldr      r0, [sp, #0x20]
00375a20 bl       #0x36f074
00375a24 cmp      r0, #0
00375a28 beq      #0x375a38
00375a2c ldr      r3, [sp, #0x2c]
00375a30 cmp      r3, #0
00375a34 bne      #0x3757fc
00375a38 ldr      r0, [sp, #0x20]
00375a3c bl       #0x36e09c
00375a40 ldr      r3, [r0]
00375a44 mov      r7, r0
00375a48 mov      lr, pc
00375a4c ldr      pc, [r3, #0x5c]
00375a50 cmp      r0, #0
00375a54 beq      #0x375840
00375a58 ldr      r3, [r7, #0x660]
00375a5c cmp      r3, #0
00375a60 beq      #0x375840
00375a64 ldr      r2, [r3, #0x160]
00375a68 ldr      r0, [sp, #0x1c]
00375a6c ldr      r1, [pc, #0x78]
00375a70 str      r2, [sp, #0x30]
00375a74 ldr      r2, [r3, #0x164]
00375a78 add      r7, sp, #0x30
00375a7c ldr      r1, [r0, r1]
00375a80 str      r2, [sp, #0x34]
00375a84 ldr      r3, [r3, #0x168]
00375a88 mov      r0, r7
00375a8c str      r3, [sp, #0x38]
00375a90 bl       #0x312b6c
00375a94 cmp      r0, #0
00375a98 bne      #0x375840
00375a9c mov      r0, r6
00375aa0 mov      r1, r7
00375aa4 mov      r2, #1
00375aa8 bl       #0x393db4
00375aac mov      r0, r6
00375ab0 mov      r1, r7
00375ab4 bl       #0x393ae4
00375ab8 b        #0x375840
00375abc ldr      r2, [pc, #0x14]
00375ac0 ldr      r0, [fp, r2]
00375ac4 str      r2, [sp, #0x10]
00375ac8 bl       #0x31f594
00375acc bl       #0x3f0428
00375ad0 b        #0x375698
00375ad4 rsbeq    pc, r1, r8, lsl #8
00375ad8 strdeq   r3, r4, [r0], -r4
00375adc andeq    r0, r0, r4, lsr #27
00375ae0 subseq   fp, r4, r0, lsr #18
00375ae4 subseq   ip, r4, r0, ror r0
00375ae8 subseq   ip, r4, r4, ror #2
00375aec andeq    r3, r0, ip, lsr #30
