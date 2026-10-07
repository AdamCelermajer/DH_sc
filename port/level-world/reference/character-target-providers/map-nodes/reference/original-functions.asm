
# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKi14ObjectListItemENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EESaIS6_EE14_M_create_nodeERKS6_
0033f654: push     {r4, r5, lr}
0033f658: sub      sp, sp, #0xc
0033f65c: add      r0, sp, #8
0033f660: mov      r3, #0x30
0033f664: str      r3, [r0, #-4]!
0033f668: mov      r5, r1
0033f66c: bl       #0x708ec0
0033f670: ldr      r3, [r5]
0033f674: mov      r4, r0
0033f678: add      r0, r0, #0x14
0033f67c: str      r0, [r4, #0x24]
0033f680: str      r3, [r4, #0x10]
0033f684: str      r0, [r4, #0x28]
0033f688: ldr      r2, [r5, #0x14]
0033f68c: ldr      r1, [r5, #0x18]
0033f690: bl       #0x3116e8
0033f694: ldr      r2, [r5, #0x1c]
0033f698: mov      r3, #0
0033f69c: str      r3, [r4, #0xc]
0033f6a0: str      r2, [r4, #0x2c]
0033f6a4: str      r3, [r4, #8]
0033f6a8: mov      r0, r4
0033f6ac: add      sp, sp, #0xc
0033f6b0: pop      {r4, r5, pc}

# _ZNSt4priv10_Rb_globalIbE10_RebalanceEPNS_18_Rb_tree_node_baseERS3_
00313760: mov      r3, #0
00313764: push     {r4, r5, r6}
00313768: strb     r3, [r0]
0031376c: mov      r5, #1
00313770: ldr      ip, [r1]
00313774: cmp      ip, r0
00313778: beq      #0x31378c
0031377c: ldr      r2, [r0, #4]
00313780: ldrb     r4, [r2]
00313784: cmp      r4, #0
00313788: beq      #0x31379c
0031378c: mov      r3, #1
00313790: strb     r3, [ip]
00313794: pop      {r4, r5, r6}
00313798: bx       lr
0031379c: ldr      r6, [r2, #4]
003137a0: ldr      ip, [r6, #8]
003137a4: cmp      r2, ip
003137a8: beq      #0x313864
003137ac: cmp      ip, #0
003137b0: beq      #0x3137e4
003137b4: ldrb     r4, [ip]
003137b8: cmp      r4, #0
003137bc: bne      #0x3137e4
003137c0: strb     r5, [r2]
003137c4: strb     r5, [ip]
003137c8: ldr      r2, [r0, #4]
003137cc: ldr      r2, [r2, #4]
003137d0: strb     r4, [r2]
003137d4: ldr      r2, [r0, #4]
003137d8: ldr      r2, [r2, #4]
003137dc: mov      r0, r2
003137e0: b        #0x313770
003137e4: ldr      ip, [r2, #8]
003137e8: cmp      ip, r0
003137ec: movne    ip, r2
003137f0: movne    r2, r0
003137f4: beq      #0x3138fc
003137f8: strb     r5, [ip]
003137fc: ldr      r0, [r2, #4]
00313800: ldr      r0, [r0, #4]
00313804: strb     r3, [r0]
00313808: ldr      r0, [r2, #4]
0031380c: ldr      r0, [r0, #4]
00313810: ldr      ip, [r0, #0xc]
00313814: ldr      r4, [ip, #8]
00313818: str      r4, [r0, #0xc]
0031381c: ldr      r4, [ip, #8]
00313820: cmp      r4, #0
00313824: strne    r0, [r4, #4]
00313828: ldr      r4, [r0, #4]
0031382c: str      r4, [ip, #4]
00313830: ldr      r4, [r1]
00313834: cmp      r0, r4
00313838: streq    ip, [r1]
0031383c: beq      #0x313854
00313840: ldr      r4, [r0, #4]
00313844: ldr      r6, [r4, #8]
00313848: cmp      r0, r6
0031384c: streq    ip, [r4, #8]
00313850: strne    ip, [r4, #0xc]
00313854: str      r0, [ip, #8]
00313858: str      ip, [r0, #4]
0031385c: mov      r0, r2
00313860: b        #0x313770
00313864: ldr      ip, [r6, #0xc]
00313868: cmp      ip, #0
0031386c: beq      #0x31387c
00313870: ldrb     r4, [ip]
00313874: cmp      r4, #0
00313878: beq      #0x3137c0
0031387c: ldr      ip, [r2, #0xc]
00313880: cmp      ip, r0
00313884: movne    ip, r2
00313888: movne    r2, r0
0031388c: beq      #0x313948
00313890: strb     r5, [ip]
00313894: ldr      r0, [r2, #4]
00313898: ldr      r0, [r0, #4]
0031389c: strb     r3, [r0]
003138a0: ldr      r0, [r2, #4]
003138a4: ldr      r0, [r0, #4]
003138a8: ldr      ip, [r0, #8]
003138ac: ldr      r4, [ip, #0xc]
003138b0: str      r4, [r0, #8]
003138b4: ldr      r4, [ip, #0xc]
003138b8: cmp      r4, #0
003138bc: strne    r0, [r4, #4]
003138c0: ldr      r4, [r0, #4]
003138c4: str      r4, [ip, #4]
003138c8: ldr      r4, [r1]
003138cc: cmp      r0, r4
003138d0: streq    ip, [r1]
003138d4: beq      #0x3138ec
003138d8: ldr      r4, [r0, #4]
003138dc: ldr      r6, [r4, #0xc]
003138e0: cmp      r0, r6
003138e4: streq    ip, [r4, #0xc]
003138e8: strne    ip, [r4, #8]
003138ec: str      r0, [ip, #0xc]
003138f0: str      ip, [r0, #4]
003138f4: mov      r0, r2
003138f8: b        #0x313770
003138fc: ldr      r4, [r0, #0xc]
00313900: str      r4, [r2, #8]
00313904: ldr      r0, [r0, #0xc]
00313908: cmp      r0, #0
0031390c: strne    r2, [r0, #4]
00313910: ldrne    r6, [r2, #4]
00313914: str      r6, [ip, #4]
00313918: ldr      r0, [r1]
0031391c: cmp      r2, r0
00313920: streq    ip, [r1]
00313924: beq      #0x31393c
00313928: ldr      r0, [r2, #4]
0031392c: ldr      r4, [r0, #0xc]
00313930: cmp      r2, r4
00313934: streq    ip, [r0, #0xc]
00313938: strne    ip, [r0, #8]
0031393c: str      r2, [ip, #0xc]
00313940: str      ip, [r2, #4]
00313944: b        #0x3137f8
00313948: ldr      r4, [r0, #8]
0031394c: str      r4, [r2, #0xc]
00313950: ldr      r0, [r0, #8]
00313954: cmp      r0, #0
00313958: strne    r2, [r0, #4]
0031395c: ldrne    r6, [r2, #4]
00313960: str      r6, [ip, #4]
00313964: ldr      r0, [r1]
00313968: cmp      r2, r0
0031396c: streq    ip, [r1]
00313970: beq      #0x313988
00313974: ldr      r0, [r2, #4]
00313978: ldr      r4, [r0, #8]
0031397c: cmp      r2, r4
00313980: streq    ip, [r0, #8]
00313984: strne    ip, [r0, #0xc]
00313988: str      r2, [ip, #8]
0031398c: str      ip, [r2, #4]
00313990: b        #0x313890
