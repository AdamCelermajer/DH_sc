
# _ZNSs19_M_range_initializeEPKcS0_
003116e8: push     {r4, r5, r6, r7, r8, lr}
003116ec: rsb      r5, r1, r2
003116f0: mov      r4, r1
003116f4: mov      r7, r2
003116f8: add      r1, r5, #1
003116fc: mov      r6, r0
00311700: bl       #0x31167c
00311704: cmp      r7, r4
00311708: ldr      r0, [r6, #0x14]
0031170c: beq      #0x311720
00311710: mov      r1, r4
00311714: mov      r2, r5
00311718: bl       #0x30e868
0031171c: add      r0, r0, r5
00311720: mov      r3, #0
00311724: str      r0, [r6, #0x10]
00311728: strb     r3, [r0]
0031172c: pop      {r4, r5, r6, r7, r8, pc}

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE13insert_uniqueERKS6_
0033f78c: push     {r4, r5, r6, lr}
0033f790: ldr      ip, [r1, #4]
0033f794: sub      sp, sp, #0x10
0033f798: mov      r4, r0
0033f79c: cmp      ip, #0
0033f7a0: mov      r3, r2
0033f7a4: moveq    ip, r1
0033f7a8: beq      #0x33f804
0033f7ac: ldr      r6, [r2]
0033f7b0: b        #0x33f7b8
0033f7b4: mov      ip, r2
0033f7b8: ldr      r0, [ip, #0x10]
0033f7bc: mov      r5, #1
0033f7c0: cmp      r0, r6
0033f7c4: ldrgt    r2, [ip, #8]
0033f7c8: ldrle    r2, [ip, #0xc]
0033f7cc: movle    r5, #0
0033f7d0: cmp      r2, #0
0033f7d4: bne      #0x33f7b4
0033f7d8: cmp      r5, #0
0033f7dc: moveq    r5, ip
0033f7e0: bne      #0x33f804
0033f7e4: cmp      r6, r0
0033f7e8: movle    r3, #0
0033f7ec: strle    r5, [r4]
0033f7f0: strble   r3, [r4, #4]
0033f7f4: bgt      #0x33f86c
0033f7f8: mov      r0, r4
0033f7fc: add      sp, sp, #0x10
0033f800: pop      {r4, r5, r6, pc}
0033f804: ldr      r2, [r1, #8]
0033f808: cmp      ip, r2
0033f80c: beq      #0x33f8ec
0033f810: ldrb     r2, [ip]
0033f814: cmp      r2, #0
0033f818: bne      #0x33f82c
0033f81c: ldr      r2, [ip, #4]
0033f820: ldr      r2, [r2, #4]
0033f824: cmp      ip, r2
0033f828: beq      #0x33f8d8
0033f82c: ldr      r0, [ip, #8]
0033f830: cmp      r0, #0
0033f834: bne      #0x33f840
0033f838: b        #0x33f898
0033f83c: mov      r0, r2
0033f840: ldr      r2, [r0, #0xc]
0033f844: cmp      r2, #0
0033f848: bne      #0x33f83c
0033f84c: ldr      r6, [r3]
0033f850: mov      r5, r0
0033f854: ldr      r0, [r0, #0x10]
0033f858: cmp      r6, r0
0033f85c: movle    r3, #0
0033f860: strle    r5, [r4]
0033f864: strble   r3, [r4, #4]
0033f868: ble      #0x33f7f8
0033f86c: mov      r2, ip
0033f870: add      r0, sp, #8
0033f874: mov      ip, #0
0033f878: str      ip, [sp, #4]
0033f87c: str      ip, [sp]
0033f880: bl       #0x33f6b4
0033f884: ldr      r3, [sp, #8]
0033f888: mov      r2, #1
0033f88c: strb     r2, [r4, #4]
0033f890: str      r3, [r4]
0033f894: b        #0x33f7f8
0033f898: ldr      r2, [ip, #4]
0033f89c: ldr      r0, [r2, #8]
0033f8a0: cmp      ip, r0
0033f8a4: movne    r5, r2
0033f8a8: ldrne    r6, [r3]
0033f8ac: ldrne    r0, [r2, #0x10]
0033f8b0: beq      #0x33f8bc
0033f8b4: b        #0x33f7e4
0033f8b8: mov      r2, r5
0033f8bc: ldr      r5, [r2, #4]
0033f8c0: ldr      r0, [r5, #8]
0033f8c4: cmp      r0, r2
0033f8c8: beq      #0x33f8b8
0033f8cc: ldr      r6, [r3]
0033f8d0: ldr      r0, [r5, #0x10]
0033f8d4: b        #0x33f7e4
0033f8d8: ldr      r2, [ip, #0xc]
0033f8dc: ldr      r6, [r3]
0033f8e0: mov      r5, r2
0033f8e4: ldr      r0, [r2, #0x10]
0033f8e8: b        #0x33f7e4
0033f8ec: mov      r2, ip
0033f8f0: mov      lr, #0
0033f8f4: add      r0, sp, #0xc
0033f8f8: stm      sp, {ip, lr}
0033f8fc: bl       #0x33f6b4
0033f900: ldr      r3, [sp, #0xc]
0033f904: mov      r2, #1
0033f908: strb     r2, [r4, #4]
0033f90c: str      r3, [r4]
0033f910: b        #0x33f7f8

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS6_SE_SE_
0033f6b4: cmp      r1, r2
0033f6b8: push     {r4, r5, r6, r7, r8, lr}
0033f6bc: mov      r4, r1
0033f6c0: mov      r5, r2
0033f6c4: mov      r6, r0
0033f6c8: beq      #0x33f758
0033f6cc: ldr      r2, [sp, #0x1c]
0033f6d0: cmp      r2, #0
0033f6d4: beq      #0x33f720
0033f6d8: mov      r1, r3
0033f6dc: mov      r0, r4
0033f6e0: bl       #0x33f654
0033f6e4: str      r0, [r5, #0xc]
0033f6e8: ldr      r3, [r4, #0xc]
0033f6ec: mov      r7, r0
0033f6f0: cmp      r5, r3
0033f6f4: beq      #0x33f750
0033f6f8: mov      r0, r7
0033f6fc: str      r5, [r7, #4]
0033f700: add      r1, r4, #4
0033f704: bl       #0x313760
0033f708: ldr      r3, [r4, #0x10]
0033f70c: mov      r0, r6
0033f710: add      r3, r3, #1
0033f714: str      r3, [r4, #0x10]
0033f718: str      r7, [r6]
0033f71c: pop      {r4, r5, r6, r7, r8, pc}
0033f720: ldr      r2, [sp, #0x18]
0033f724: cmp      r2, #0
0033f728: beq      #0x33f778
0033f72c: mov      r1, r3
0033f730: mov      r0, r4
0033f734: bl       #0x33f654
0033f738: str      r0, [r5, #8]
0033f73c: ldr      r3, [r4, #8]
0033f740: mov      r7, r0
0033f744: cmp      r5, r3
0033f748: streq    r0, [r4, #8]
0033f74c: b        #0x33f6f8
0033f750: str      r7, [r4, #0xc]
0033f754: b        #0x33f6f8
0033f758: mov      r1, r3
0033f75c: mov      r0, r4
0033f760: bl       #0x33f654
0033f764: mov      r7, r0
0033f768: str      r0, [r4, #8]
0033f76c: str      r0, [r4, #4]
0033f770: str      r0, [r4, #0xc]
0033f774: b        #0x33f6f8
0033f778: ldr      r1, [r3]
0033f77c: ldr      r2, [r5, #0x10]
0033f780: cmp      r1, r2
0033f784: bge      #0x33f6d8
0033f788: b        #0x33f72c

# _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003139ac: ldr      r3, [r0, #0x14]
003139b0: cmp      r3, r0
003139b4: bxeq     lr
003139b8: cmp      r3, #0
003139bc: bxeq     lr
003139c0: ldr      r1, [r0]
003139c4: rsb      r1, r3, r1
003139c8: cmp      r1, #0x80
003139cc: bhi      #0x3139d8
003139d0: mov      r0, r3
003139d4: b        #0x708f00
003139d8: mov      r0, r3
003139dc: b        #0x310440

# _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0031167c: push     {r4, lr}
00311680: cmp      r1, #0
00311684: sub      sp, sp, #8
00311688: mov      r4, r0
0031168c: beq      #0x3116c8
00311690: cmp      r1, #0x10
00311694: bls      #0x3116c0
00311698: cmp      r1, #0x80
0031169c: str      r1, [sp, #4]
003116a0: bhi      #0x3116d8
003116a4: add      r0, sp, #4
003116a8: bl       #0x708ec0
003116ac: ldr      r3, [sp, #4]
003116b0: str      r0, [r4, #0x14]
003116b4: str      r0, [r4, #0x10]
003116b8: add      r0, r0, r3
003116bc: str      r0, [r4]
003116c0: add      sp, sp, #8
003116c4: pop      {r4, pc}
003116c8: ldr      r0, [pc, #0x14]
003116cc: add      r0, pc, r0
003116d0: bl       #0x708e40
003116d4: b        #0x3116c0
003116d8: mov      r0, r1
003116dc: bl       #0x310454
003116e0: b        #0x3116ac
003116e4: subseq   ip, sl, ip, lsl #27
