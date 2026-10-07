# _ZN12CharAnimator12_SetAnimStepEj
003ca79c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ca7a0 ldr      r3, [r0, #0x2c]
003ca7a4 mov      r2, #0xc
003ca7a8 ldr      r5, [pc, #0x374]
003ca7ac mla      r3, r2, r3, r0
003ca7b0 ldr      r2, [pc, #0x370]
003ca7b4 add      r5, pc, r5
003ca7b8 mov      r4, r0
003ca7bc ldr      r2, [r5, r2]
003ca7c0 ldr      r0, [r3, #8]
003ca7c4 mov      ip, #0x14
003ca7c8 ldr      r2, [r2]
003ca7cc sub      sp, sp, #0x24
003ca7d0 mla      r2, ip, r0, r2
003ca7d4 ldr      r0, [r2, #8]
003ca7d8 cmp      r0, r1
003ca7dc bhi      #0x3ca7e8
003ca7e0 add      sp, sp, #0x24
003ca7e4 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ca7e8 ldr      r2, [r2, #0xc]
003ca7ec mov      r6, #0x38
003ca7f0 str      r1, [r3, #0x10]
003ca7f4 mla      r6, r6, r1, r2
003ca7f8 ldr      r0, [r4, #4]
003ca7fc mov      r1, #0x26
003ca800 mov      r2, #0
003ca804 bl       #0x3a4d5c
003ca808 ldr      r3, [r6, #0x28]
003ca80c cmp      r3, #1
003ca810 beq      #0x3caad8
003ca814 ldr      r3, [r6, #0x10]
003ca818 cmn      r3, #1
003ca81c beq      #0x3ca9d8
003ca820 ldr      r3, [pc, #0x304]
003ca824 ldr      r0, [r5, r3]
003ca828 bl       #0x31f594
003ca82c cmp      r0, #0
003ca830 beq      #0x3ca870
003ca834 ldr      r7, [r0, #0x128]
003ca838 cmp      r7, #0
003ca83c beq      #0x3ca870
003ca840 mov      r0, r7
003ca844 ldr      r1, [r4, #4]
003ca848 bl       #0x40f980
003ca84c cmp      r0, #0
003ca850 beq      #0x3ca870
003ca854 ldr      r1, [r6, #0x10]
003ca858 cmn      r1, #1
003ca85c beq      #0x3ca9f4
003ca860 mov      r0, r7
003ca864 mov      r2, #0
003ca868 mov      r3, #1
003ca86c bl       #0x40f904
003ca870 ldrb     r3, [r6, #0x34]
003ca874 strb     r3, [r4, #0x30]
003ca878 ldrb     r3, [r6, #0x34]
003ca87c cmp      r3, #0
003ca880 moveq    r7, #1
003ca884 bne      #0x3caa10
003ca888 ldr      r3, [pc, #0x2a0]
003ca88c ldr      r0, [r4, #4]
003ca890 ldr      sb, [r6, #0x2c]
003ca894 ldr      r3, [r5, r3]
003ca898 ldr      fp, [r3]
003ca89c bl       #0x3935dc
003ca8a0 ldr      lr, [r0]
003ca8a4 ldr      r8, [r0, #4]
003ca8a8 ldr      sl, [r0, #8]
003ca8ac mov      ip, #0xbf000000
003ca8b0 add      ip, ip, #0x800000
003ca8b4 str      lr, [sp, #0x14]
003ca8b8 mov      r0, fp
003ca8bc mov      lr, #1
003ca8c0 mov      r1, sb
003ca8c4 add      r2, sp, #0x14
003ca8c8 mov      r3, #0
003ca8cc str      r8, [sp, #0x18]
003ca8d0 str      sl, [sp, #0x1c]
003ca8d4 str      lr, [sp]
003ca8d8 str      ip, [sp, #8]
003ca8dc str      ip, [sp, #4]
003ca8e0 bl       #0x36b5d8
003ca8e4 cmp      r7, #0
003ca8e8 beq      #0x3ca91c
003ca8ec ldr      r8, [r6, #0x18]
003ca8f0 cmn      r8, #1
003ca8f4 beq      #0x3ca91c
003ca8f8 ldrb     r7, [r6, #4]
003ca8fc cmp      r7, #0
003ca900 beq      #0x3caa94
003ca904 ldr      r3, [pc, #0x228]
003ca908 mov      r1, r8
003ca90c ldr      r2, [r4, #4]
003ca910 ldr      r0, [r5, r3]
003ca914 mov      r3, #0
003ca918 bl       #0x495f04
003ca91c ldr      r2, [r6, #0x30]
003ca920 ldr      r3, [r4, #4]
003ca924 str      r2, [r4, #0x34]
003ca928 ldr      r5, [r3, #0x2d8]
003ca92c cmp      r5, #0
003ca930 beq      #0x3ca9e8
003ca934 ldr      r3, [r6, #8]
003ca938 cmn      r3, #1
003ca93c beq      #0x3ca9e8
003ca940 mov      r3, #0
003ca944 strb     r3, [r4, #0x48]
003ca948 mov      r0, r4
003ca94c bl       #0x3c9b7c
003ca950 ldrb     r3, [r4, #0x54]
003ca954 cmp      r3, #0
003ca958 beq      #0x3caa80
003ca95c ldrb     r1, [r4, #0x49]
003ca960 ldr      r3, [r5, #0x38]
003ca964 ldrb     r2, [r6, #0x1c]
003ca968 cmp      r1, #0
003ca96c beq      #0x3caac4
003ca970 mov      r1, #0
003ca974 str      r1, [r3, #0x14]
003ca978 mov      r1, #0
003ca97c str      r1, [r3, #0xc]
003ca980 strb     r2, [r3, #0x10]
003ca984 ldr      r2, [r5, #0x38]
003ca988 ldr      r1, [r6, #8]
003ca98c mov      r6, #0
003ca990 ldr      r3, [r4, #0x44]
003ca994 ldr      ip, [r2]
003ca998 mov      r0, r2
003ca99c str      r6, [sp]
003ca9a0 mov      r2, r6
003ca9a4 mov      lr, pc
003ca9a8 ldr      pc, [ip, #0x1c]
003ca9ac ldr      r1, [r4, #0x34]
003ca9b0 ldr      r0, [r4, #0x40]
003ca9b4 bl       #0x30ed6c
003ca9b8 ldr      r5, [r5, #0x38]
003ca9bc mov      r1, r0
003ca9c0 mov      r2, r6
003ca9c4 mov      r0, r5
003ca9c8 ldr      r3, [r5]
003ca9cc mov      lr, pc
003ca9d0 ldr      pc, [r3, #0x28]
003ca9d4 b        #0x3ca7e0
003ca9d8 ldr      r3, [r6, #0x20]
003ca9dc cmp      r3, #0
003ca9e0 beq      #0x3ca870
003ca9e4 b        #0x3ca820
003ca9e8 mov      r0, r4
003ca9ec bl       #0x3c9924
003ca9f0 b        #0x3ca7e0
003ca9f4 ldr      r0, [r6, #0x20]
003ca9f8 cmp      r0, #0
003ca9fc beq      #0x3ca870
003caa00 ldr      r8, [r6, #0x24]
003caa04 bl       #0x3ca708
003caa08 ldr      r1, [r8, r0, lsl #2]
003caa0c b        #0x3ca860
003caa10 ldr      r0, [r4, #4]
003caa14 mov      r1, #1
003caa18 add      r0, r0, #0x37c
003caa1c bl       #0x3ffe3c
003caa20 mov      r7, r0
003caa24 ldr      r0, [r4, #4]
003caa28 mov      r1, #2
003caa2c add      r0, r0, #0x37c
003caa30 bl       #0x3ffe3c
003caa34 mov      r1, r7
003caa38 mov      sl, r0
003caa3c mov      r0, r4
003caa40 bl       #0x3c94b8
003caa44 mov      r1, r7
003caa48 eor      r8, r0, #1
003caa4c ldrb     r2, [r6, #4]
003caa50 mov      r0, r4
003caa54 bl       #0x3c955c
003caa58 uxtb     r8, r8
003caa5c eor      r0, r0, #1
003caa60 cmp      r8, #0
003caa64 uxtb     r7, r0
003caa68 bne      #0x3caaf0
003caa6c cmp      r7, #0
003caa70 bne      #0x3cab08
003caa74 cmp      r8, #0
003caa78 beq      #0x3ca8e4
003caa7c b        #0x3ca888
003caa80 ldr      r2, [r5, #0x38]
003caa84 ldrb     r1, [r6, #0x1c]
003caa88 str      r3, [r2, #0xc]
003caa8c strb     r1, [r2, #0x10]
003caa90 b        #0x3ca984
003caa94 ldr      r0, [r4, #4]
003caa98 bl       #0x3935dc
003caa9c ldr      r3, [r4, #4]
003caaa0 mov      r2, r0
003caaa4 ldr      r0, [pc, #0x88]
003caaa8 mov      r1, r8
003caaac add      r3, r3, #0x16c
003caab0 ldr      r0, [r5, r0]
003caab4 str      r7, [sp, #4]
003caab8 str      r7, [sp]
003caabc bl       #0x495888
003caac0 b        #0x3ca91c
003caac4 ldrb     r1, [r4, #0x4a]
003caac8 cmp      r1, #0
003caacc ldreq    r1, [r6, #0xc]
003caad0 beq      #0x3ca974
003caad4 b        #0x3ca970
003caad8 ldr      r2, [r4, #0x2c]
003caadc mov      r0, r4
003caae0 ldr      r1, [r6, #8]
003caae4 add      r2, r2, #1
003caae8 bl       #0x3cab38
003caaec b        #0x3ca7e0
003caaf0 mov      r0, r4
003caaf4 mov      r1, sl
003caaf8 bl       #0x3c94b8
003caafc eor      r0, r0, #1
003cab00 uxtb     r8, r0
003cab04 b        #0x3caa6c
003cab08 mov      r1, sl
003cab0c mov      r0, r4
003cab10 ldrb     r2, [r6, #4]
003cab14 bl       #0x3c955c
003cab18 eor      r0, r0, #1
003cab1c uxtb     r8, r0
003cab20 b        #0x3caa74
003cab24 ldrsbeq  sl, [ip], #-0x2c
003cab28 andeq    r3, r0, ip, ror ip
003cab2c strdeq   r3, r4, [r0], -r4
003cab30 andeq    r0, r0, r4, lsr #27
003cab34 andeq    r1, r0, r8, lsl #22
