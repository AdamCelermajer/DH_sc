
# _ZNSaINSt4priv13_Rb_tree_nodeISt4pairIKiP9CharacterEEEE8allocateEjPKv.clone.8
003aa75c: str      lr, [sp, #-4]!
003aa760: sub      sp, sp, #0xc
003aa764: add      r0, sp, #8
003aa768: mov      r3, #0x18
003aa76c: str      r3, [r0, #-4]!
003aa770: bl       #0x708ec0
003aa774: add      sp, sp, #0xc
003aa778: ldm      sp!, {pc}

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiP9CharacterENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS7_SF_SF_.clone.2
003aa77c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003aa780: ldr      r4, [pc, #0x124]
003aa784: ldr      r5, [pc, #0x124]
003aa788: mov      r7, r1
003aa78c: add      r4, pc, r4
003aa790: ldr      r1, [r4, r5]
003aa794: mov      r6, r0
003aa798: mov      sl, r2
003aa79c: cmp      r7, r1
003aa7a0: beq      #0x3aa860
003aa7a4: ldr      r2, [sp, #0x20]
003aa7a8: cmp      r2, #0
003aa7ac: beq      #0x3aa818
003aa7b0: ldr      sb, [r4, r5]
003aa7b4: mov      r0, sb
003aa7b8: bl       #0x3aa75c
003aa7bc: ldr      r2, [sl]
003aa7c0: mov      r3, #0
003aa7c4: mov      r8, r0
003aa7c8: str      r2, [r0, #0x10]
003aa7cc: ldr      r2, [sl, #4]
003aa7d0: str      r3, [r0, #0xc]
003aa7d4: str      r3, [r0, #8]
003aa7d8: str      r2, [r0, #0x14]
003aa7dc: str      r0, [r7, #0xc]
003aa7e0: ldr      r3, [sb, #0xc]
003aa7e4: cmp      r7, r3
003aa7e8: streq    r0, [sb, #0xc]
003aa7ec: ldr      r4, [r4, r5]
003aa7f0: mov      r0, r8
003aa7f4: str      r7, [r8, #4]
003aa7f8: add      r1, r4, #4
003aa7fc: bl       #0x313760
003aa800: ldr      r3, [r4, #0x10]
003aa804: mov      r0, r6
003aa808: add      r3, r3, #1
003aa80c: str      r3, [r4, #0x10]
003aa810: str      r8, [r6]
003aa814: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003aa818: cmp      r3, #0
003aa81c: beq      #0x3aa898
003aa820: ldr      sb, [r4, r5]
003aa824: mov      r0, sb
003aa828: bl       #0x3aa75c
003aa82c: ldr      r2, [sl]
003aa830: mov      r3, #0
003aa834: mov      r8, r0
003aa838: str      r2, [r0, #0x10]
003aa83c: ldr      r2, [sl, #4]
003aa840: str      r3, [r0, #0xc]
003aa844: str      r3, [r0, #8]
003aa848: str      r2, [r0, #0x14]
003aa84c: str      r0, [r7, #8]
003aa850: ldr      r3, [sb, #8]
003aa854: cmp      r7, r3
003aa858: streq    r0, [sb, #8]
003aa85c: b        #0x3aa7ec
003aa860: mov      r0, r7
003aa864: bl       #0x3aa75c
003aa868: ldr      r2, [sl]
003aa86c: mov      r3, #0
003aa870: mov      r8, r0
003aa874: str      r2, [r0, #0x10]
003aa878: ldr      r2, [sl, #4]
003aa87c: str      r3, [r0, #0xc]
003aa880: str      r3, [r0, #8]
003aa884: str      r2, [r0, #0x14]
003aa888: str      r0, [r7, #8]
003aa88c: str      r0, [r7, #4]
003aa890: str      r0, [r7, #0xc]
003aa894: b        #0x3aa7ec
003aa898: ldr      r2, [sl]
003aa89c: ldr      r3, [r7, #0x10]
003aa8a0: cmp      r2, r3
003aa8a4: blt      #0x3aa820
003aa8a8: b        #0x3aa7b0
003aa8ac: subseq   sl, lr, r4, lsl #6
003aa8b0: andeq    r1, r0, r4, lsr r1

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiP9CharacterENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueERKS7_.clone.7
003aa8b4: ldr      r3, [pc, #0x170]
003aa8b8: ldr      ip, [pc, #0x170]
003aa8bc: push     {r4, r5, r6, r7, lr}
003aa8c0: add      r3, pc, r3
003aa8c4: mov      r4, r0
003aa8c8: ldr      r0, [r3, ip]
003aa8cc: mov      r2, r1
003aa8d0: sub      sp, sp, #0x14
003aa8d4: ldr      r1, [r0, #4]
003aa8d8: cmp      r1, #0
003aa8dc: moveq    r1, r0
003aa8e0: beq      #0x3aa93c
003aa8e4: ldr      r7, [r2]
003aa8e8: b        #0x3aa8f0
003aa8ec: mov      r1, r0
003aa8f0: ldr      r5, [r1, #0x10]
003aa8f4: mov      r6, #1
003aa8f8: cmp      r5, r7
003aa8fc: ldrgt    r0, [r1, #8]
003aa900: ldrle    r0, [r1, #0xc]
003aa904: movle    r6, #0
003aa908: cmp      r0, #0
003aa90c: bne      #0x3aa8ec
003aa910: cmp      r6, #0
003aa914: moveq    r3, r1
003aa918: bne      #0x3aa93c
003aa91c: cmp      r7, r5
003aa920: strle    r3, [r4]
003aa924: movle    r3, #0
003aa928: strble   r3, [r4, #4]
003aa92c: bgt      #0x3aa9a4
003aa930: mov      r0, r4
003aa934: add      sp, sp, #0x14
003aa938: pop      {r4, r5, r6, r7, pc}
003aa93c: ldr      r3, [r3, ip]
003aa940: ldr      r3, [r3, #8]
003aa944: cmp      r3, r1
003aa948: beq      #0x3aaa04
003aa94c: ldrb     r3, [r1]
003aa950: cmp      r3, #0
003aa954: bne      #0x3aa968
003aa958: ldr      r3, [r1, #4]
003aa95c: ldr      r3, [r3, #4]
003aa960: cmp      r1, r3
003aa964: beq      #0x3aa9f4
003aa968: ldr      r3, [r1, #8]
003aa96c: cmp      r3, #0
003aa970: beq      #0x3aa9cc
003aa974: b        #0x3aa97c
003aa978: mov      r3, r0
003aa97c: ldr      r0, [r3, #0xc]
003aa980: cmp      r0, #0
003aa984: bne      #0x3aa978
003aa988: ldr      r5, [r3, #0x10]
003aa98c: ldr      r7, [r2]
003aa990: cmp      r7, r5
003aa994: strle    r3, [r4]
003aa998: movle    r3, #0
003aa99c: strble   r3, [r4, #4]
003aa9a0: ble      #0x3aa930
003aa9a4: mov      ip, #0
003aa9a8: mov      r3, ip
003aa9ac: add      r0, sp, #0xc
003aa9b0: str      ip, [sp]
003aa9b4: bl       #0x3aa77c
003aa9b8: ldr      r3, [sp, #0xc]
003aa9bc: mov      r2, #1
003aa9c0: strb     r2, [r4, #4]
003aa9c4: str      r3, [r4]
003aa9c8: b        #0x3aa930
003aa9cc: ldr      r3, [r1, #4]
003aa9d0: ldr      r0, [r3, #8]
003aa9d4: cmp      r1, r0
003aa9d8: bne      #0x3aa988
003aa9dc: mov      r0, r3
003aa9e0: ldr      r3, [r3, #4]
003aa9e4: ldr      ip, [r3, #8]
003aa9e8: cmp      ip, r0
003aa9ec: beq      #0x3aa9dc
003aa9f0: b        #0x3aa988
003aa9f4: ldr      r3, [r1, #0xc]
003aa9f8: ldr      r7, [r2]
003aa9fc: ldr      r5, [r3, #0x10]
003aaa00: b        #0x3aa91c
003aaa04: mov      r3, r1
003aaa08: mov      ip, #0
003aaa0c: add      r0, sp, #8
003aaa10: str      ip, [sp]
003aaa14: bl       #0x3aa77c
003aaa18: ldr      r3, [sp, #8]
003aaa1c: mov      r2, #1
003aaa20: strb     r2, [r4, #4]
003aaa24: str      r3, [r4]
003aaa28: b        #0x3aa930
003aaa2c: ldrsbeq  sl, [lr], #-0x10
003aaa30: andeq    r1, r0, r4, lsr r1

# _GLOBAL__I_.._.._sources_Game_Objects_Characters_Character.cpp
003aaa34: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aaa38: ldr      r4, [pc, #0x848]
003aaa3c: mov      r3, #0x3f000000
003aaa40: sub      sp, sp, #0x1c
003aaa44: add      r4, pc, r4
003aaa48: str      r3, [r4, #0x10]
003aaa4c: str      r3, [r4, #8]
003aaa50: str      r3, [r4, #0xc]
003aaa54: bl       #0x809a38
003aaa58: ldr      r5, [pc, #0x82c]
003aaa5c: strb     r0, [r4, #0x14]
003aaa60: ldr      r3, [pc, #0x828]
003aaa64: ldr      r0, [pc, #0x828]
003aaa68: add      r5, pc, r5
003aaa6c: ldr      r1, [r5, r3]
003aaa70: add      r0, pc, r0
003aaa74: bl       #0x80a2e8
003aaa78: strb     r0, [r4, #0x15]
003aaa7c: ldr      r3, [pc, #0x814]
003aaa80: ldr      r0, [pc, #0x814]
003aaa84: ldr      r7, [pc, #0x814]
003aaa88: ldr      r1, [r5, r3]
003aaa8c: add      r0, pc, r0
003aaa90: bl       #0x80a2e8
003aaa94: ldr      r3, [pc, #0x808]
003aaa98: strb     r0, [r4, #0x16]
003aaa9c: ldr      r0, [pc, #0x804]
003aaaa0: ldr      r1, [r5, r3]
003aaaa4: add      r0, pc, r0
003aaaa8: bl       #0x80a2e8
003aaaac: ldr      r3, [pc, #0x7f8]
003aaab0: strb     r0, [r4, #0x17]
003aaab4: ldr      r0, [pc, #0x7f4]
003aaab8: ldr      r1, [r5, r3]
003aaabc: add      r0, pc, r0
003aaac0: bl       #0x80a2e8
003aaac4: ldr      r3, [pc, #0x7e8]
003aaac8: strb     r0, [r4, #0x18]
003aaacc: ldr      r0, [pc, #0x7e4]
003aaad0: ldr      r1, [r5, r3]
003aaad4: add      r0, pc, r0
003aaad8: bl       #0x80a2e8
003aaadc: ldr      r3, [pc, #0x7d8]
003aaae0: strb     r0, [r4, #0x19]
003aaae4: ldr      r0, [pc, #0x7d4]
003aaae8: ldr      r1, [r5, r3]
003aaaec: add      r0, pc, r0
003aaaf0: bl       #0x80a2e8
003aaaf4: ldr      r3, [pc, #0x7c8]
003aaaf8: strb     r0, [r4, #0x1a]
003aaafc: ldr      r0, [pc, #0x7c4]
003aab00: ldr      r1, [r5, r3]
003aab04: add      r0, pc, r0
003aab08: bl       #0x80a2e8
003aab0c: ldr      r3, [pc, #0x7b8]
003aab10: strb     r0, [r4, #0x1b]
003aab14: ldr      r0, [pc, #0x7b4]
003aab18: ldr      r1, [r5, r3]
003aab1c: add      r0, pc, r0
003aab20: bl       #0x80a2e8
003aab24: ldr      r3, [pc, #0x7a8]
003aab28: strb     r0, [r4, #0x1c]
003aab2c: ldr      r0, [pc, #0x7a4]
003aab30: ldr      r1, [r5, r3]
003aab34: add      r0, pc, r0
003aab38: bl       #0x80a2e8
003aab3c: ldr      r3, [pc, #0x798]
003aab40: strb     r0, [r4, #0x1d]
003aab44: ldr      r0, [pc, #0x794]
003aab48: ldr      r1, [r5, r3]
003aab4c: add      r0, pc, r0
003aab50: bl       #0x80a2e8
003aab54: ldr      r3, [pc, #0x788]
003aab58: strb     r0, [r4, #0x1e]
003aab5c: ldr      r0, [pc, #0x784]
003aab60: ldr      r1, [r5, r3]
003aab64: add      r0, pc, r0
003aab68: bl       #0x80a2e8
003aab6c: ldr      r3, [pc, #0x778]
003aab70: strb     r0, [r4, #0x1f]
003aab74: ldr      r0, [pc, #0x774]
003aab78: ldr      r1, [r5, r3]
003aab7c: add      r0, pc, r0
003aab80: bl       #0x80a2e8
003aab84: ldr      r3, [pc, #0x768]
003aab88: strb     r0, [r4, #0x20]
003aab8c: ldr      r0, [pc, #0x764]
003aab90: ldr      r1, [r5, r3]
003aab94: add      r0, pc, r0
003aab98: bl       #0x80a2e8
003aab9c: strb     r0, [r4, #0x21]
003aaba0: bl       #0x8099a0
003aaba4: ldr      r3, [pc, #0x750]
003aaba8: ldr      r6, [r5, r7]
003aabac: ldr      r2, [pc, #0x74c]
003aabb0: ldr      r3, [r5, r3]
003aabb4: strb     r0, [r4, #0x22]
003aabb8: mov      r4, #0
003aabbc: ldr      r1, [r5, r2]
003aabc0: mov      r0, r3
003aabc4: mov      r2, r6
003aabc8: str      r4, [r3, #4]
003aabcc: strb     r4, [r3]
003aabd0: str      r3, [r3, #8]
003aabd4: str      r3, [r3, #0xc]
003aabd8: str      r4, [r3, #0x10]
003aabdc: bl       #0x30e304
003aabe0: ldr      r3, [pc, #0x71c]
003aabe4: ldr      r1, [pc, #0x71c]
003aabe8: mov      r2, r6
003aabec: ldr      r3, [r5, r3]
003aabf0: ldr      r1, [r5, r1]
003aabf4: mov      r0, r3
003aabf8: str      r4, [r3, #0x10]
003aabfc: str      r4, [r3, #4]
003aac00: strb     r4, [r3]
003aac04: str      r3, [r3, #8]
003aac08: str      r3, [r3, #0xc]
003aac0c: bl       #0x30e304
003aac10: ldr      r3, [pc, #0x6f4]
003aac14: ldr      r3, [r5, r3]
003aac18: ldr      r2, [r3]
003aac1c: tst      r2, #1
003aac20: beq      #0x3ab228
003aac24: ldr      r3, [pc, #0x6e4]
003aac28: ldr      r3, [r5, r3]
003aac2c: ldr      r2, [r3]
003aac30: tst      r2, #1
003aac34: beq      #0x3ab1f8
003aac38: ldr      r3, [pc, #0x6d4]
003aac3c: ldr      r3, [r5, r3]
003aac40: ldr      r6, [r3]
003aac44: ands     r6, r6, #1
003aac48: bne      #0x3aad3c
003aac4c: ldr      r1, [pc, #0x6c4]
003aac50: ldr      r2, [pc, #0x6c4]
003aac54: add      sb, sp, #0x14
003aac58: ldr      sl, [r5, r1]
003aac5c: ldr      r2, [r5, r2]
003aac60: str      r1, [sp, #0xc]
003aac64: mov      r4, sl
003aac68: mov      r1, #1
003aac6c: add      r2, r2, #8
003aac70: str      r1, [r3]
003aac74: str      r2, [r4], #4
003aac78: add      sl, sl, #0xa4
003aac7c: mov      r8, #8
003aac80: mov      fp, #0x70
003aac84: mov      r1, r8
003aac88: str      r6, [r4]
003aac8c: str      r6, [r4, #4]
003aac90: str      r6, [r4, #8]
003aac94: str      r6, [r4, #0xc]
003aac98: str      r6, [r4, #0x10]
003aac9c: str      r6, [r4, #0x14]
003aaca0: str      r6, [r4, #0x18]
003aaca4: str      r6, [r4, #0x1c]
003aaca8: str      r6, [r4, #0x20]
003aacac: str      r8, [r4, #0x24]
003aacb0: add      r0, r4, #0x20
003aacb4: mov      r2, r6
003aacb8: bl       #0x329510
003aacbc: ldr      r2, [r4, #0x24]
003aacc0: str      r0, [r4, #0x20]
003aacc4: mov      r3, r0
003aacc8: sub      r2, r2, #1
003aaccc: lsr      r2, r2, #1
003aacd0: mov      r0, sb
003aacd4: stmib    sp, {r2, r3}
003aacd8: str      fp, [sp, #0x14]
003aacdc: bl       #0x708ec0
003aace0: ldmib    sp, {r2, r3}
003aace4: add      r1, r3, r2, lsl #2
003aace8: str      r0, [r3, r2, lsl #2]
003aacec: str      r1, [r4, #0xc]
003aacf0: ldr      r0, [r3, r2, lsl #2]
003aacf4: str      r1, [r4, #0x1c]
003aacf8: add      r1, r0, #0x70
003aacfc: stmib    r4, {r0, r1}
003aad00: ldr      r3, [r3, r2, lsl #2]
003aad04: str      r0, [r4]
003aad08: add      r2, r3, #0x70
003aad0c: str      r2, [r4, #0x18]
003aad10: str      r3, [r4, #0x10]
003aad14: str      r3, [r4, #0x14]
003aad18: add      r4, r4, #0x28
003aad1c: cmp      r4, sl
003aad20: bne      #0x3aac84
003aad24: ldr      r2, [sp, #0xc]
003aad28: ldr      r3, [pc, #0x5f0]
003aad2c: ldr      r0, [r5, r2]
003aad30: ldr      r1, [r5, r3]
003aad34: ldr      r2, [r5, r7]
003aad38: bl       #0x30e304
003aad3c: ldr      r3, [pc, #0x5e0]
003aad40: ldr      ip, [r5, r3]
003aad44: ldr      r3, [ip]
003aad48: ands     r3, r3, #1
003aad4c: bne      #0x3aae1c
003aad50: ldr      r6, [pc, #0x5d0]
003aad54: ldr      r2, [pc, #0x5d0]
003aad58: mov      lr, #8
003aad5c: ldr      r6, [r5, r6]
003aad60: ldr      r4, [r5, r2]
003aad64: mov      r8, #1
003aad68: add      r6, r6, lr
003aad6c: str      r8, [ip]
003aad70: mov      r1, lr
003aad74: mov      r2, r3
003aad78: str      lr, [r4, #0x28]
003aad7c: str      r6, [r4]
003aad80: str      r3, [r4, #4]
003aad84: str      r3, [r4, #8]
003aad88: str      r3, [r4, #0xc]
003aad8c: str      r3, [r4, #0x10]
003aad90: str      r3, [r4, #0x14]
003aad94: str      r3, [r4, #0x18]
003aad98: str      r3, [r4, #0x1c]
003aad9c: str      r3, [r4, #0x20]
003aada0: str      r3, [r4, #0x24]
003aada4: add      r0, r4, #0x24
003aada8: bl       #0x329570
003aadac: mov      r3, #0x70
003aadb0: mov      r6, r0
003aadb4: add      r0, sp, #0x18
003aadb8: ldr      r8, [r4, #0x28]
003aadbc: str      r3, [r0, #-4]!
003aadc0: str      r6, [r4, #0x24]
003aadc4: bl       #0x708ec0
003aadc8: sub      r8, r8, #1
003aadcc: lsr      r8, r8, #1
003aadd0: str      r0, [r6, r8, lsl #2]
003aadd4: add      r3, r6, r8, lsl #2
003aadd8: str      r3, [r4, #0x10]
003aaddc: ldr      ip, [r6, r8, lsl #2]
003aade0: str      r3, [r4, #0x20]
003aade4: ldr      r2, [pc, #0x544]
003aade8: add      r3, ip, #0x70
003aadec: str      r3, [r4, #0xc]
003aadf0: str      ip, [r4, #8]
003aadf4: ldr      r3, [r6, r8, lsl #2]
003aadf8: ldr      r1, [r5, r2]
003aadfc: mov      r0, r4
003aae00: add      lr, r3, #0x70
003aae04: ldr      r2, [r5, r7]
003aae08: str      lr, [r4, #0x1c]
003aae0c: str      ip, [r4, #4]
003aae10: str      r3, [r4, #0x14]
003aae14: str      r3, [r4, #0x18]
003aae18: bl       #0x30e304
003aae1c: ldr      r3, [pc, #0x510]
003aae20: ldr      ip, [r5, r3]
003aae24: ldr      r3, [ip]
003aae28: ands     r3, r3, #1
003aae2c: bne      #0x3aaefc
003aae30: ldr      r6, [pc, #0x500]
003aae34: ldr      r2, [pc, #0x500]
003aae38: mov      lr, #8
003aae3c: ldr      r6, [r5, r6]
003aae40: ldr      r4, [r5, r2]
003aae44: mov      r8, #1
003aae48: add      r6, r6, lr
003aae4c: str      r8, [ip]
003aae50: mov      r1, lr
003aae54: mov      r2, r3
003aae58: str      lr, [r4, #0x28]
003aae5c: str      r6, [r4]
003aae60: str      r3, [r4, #4]
003aae64: str      r3, [r4, #8]
003aae68: str      r3, [r4, #0xc]
003aae6c: str      r3, [r4, #0x10]
003aae70: str      r3, [r4, #0x14]
003aae74: str      r3, [r4, #0x18]
003aae78: str      r3, [r4, #0x1c]
003aae7c: str      r3, [r4, #0x20]
003aae80: str      r3, [r4, #0x24]
003aae84: add      r0, r4, #0x24
003aae88: bl       #0x3295d0
003aae8c: mov      r3, #0x80
003aae90: mov      r6, r0
003aae94: add      r0, sp, #0x18
003aae98: ldr      r8, [r4, #0x28]
003aae9c: str      r3, [r0, #-4]!
003aaea0: str      r6, [r4, #0x24]
003aaea4: bl       #0x708ec0
003aaea8: sub      r8, r8, #1
003aaeac: lsr      r8, r8, #1
003aaeb0: str      r0, [r6, r8, lsl #2]
003aaeb4: add      r3, r6, r8, lsl #2
003aaeb8: str      r3, [r4, #0x10]
003aaebc: ldr      ip, [r6, r8, lsl #2]
003aaec0: str      r3, [r4, #0x20]
003aaec4: ldr      r2, [pc, #0x474]
003aaec8: add      r3, ip, #0x80
003aaecc: str      r3, [r4, #0xc]
003aaed0: str      ip, [r4, #8]
003aaed4: ldr      r3, [r6, r8, lsl #2]
003aaed8: ldr      r1, [r5, r2]
003aaedc: mov      r0, r4
003aaee0: add      lr, r3, #0x80
003aaee4: ldr      r2, [r5, r7]
003aaee8: str      lr, [r4, #0x1c]
003aaeec: str      ip, [r4, #4]
003aaef0: str      r3, [r4, #0x14]
003aaef4: str      r3, [r4, #0x18]
003aaef8: bl       #0x30e304
003aaefc: ldr      r3, [pc, #0x440]
003aaf00: ldr      ip, [r5, r3]
003aaf04: ldr      r3, [ip]
003aaf08: ands     r3, r3, #1
003aaf0c: bne      #0x3aafdc
003aaf10: ldr      r6, [pc, #0x430]
003aaf14: ldr      r2, [pc, #0x430]
003aaf18: mov      lr, #8
003aaf1c: ldr      r6, [r5, r6]
003aaf20: ldr      r4, [r5, r2]
003aaf24: mov      r8, #1
003aaf28: add      r6, r6, lr
003aaf2c: str      r8, [ip]
003aaf30: mov      r1, lr
003aaf34: mov      r2, r3
003aaf38: str      lr, [r4, #0x28]
003aaf3c: str      r6, [r4]
003aaf40: str      r3, [r4, #4]
003aaf44: str      r3, [r4, #8]
003aaf48: str      r3, [r4, #0xc]
003aaf4c: str      r3, [r4, #0x10]
003aaf50: str      r3, [r4, #0x14]
003aaf54: str      r3, [r4, #0x18]
003aaf58: str      r3, [r4, #0x1c]
003aaf5c: str      r3, [r4, #0x20]
003aaf60: str      r3, [r4, #0x24]
003aaf64: add      r0, r4, #0x24
003aaf68: bl       #0x329368
003aaf6c: mov      r3, #0x4c
003aaf70: mov      r6, r0
003aaf74: add      r0, sp, #0x18
003aaf78: ldr      r8, [r4, #0x28]
003aaf7c: str      r3, [r0, #-4]!
003aaf80: str      r6, [r4, #0x24]
003aaf84: bl       #0x708ec0
003aaf88: sub      r8, r8, #1
003aaf8c: lsr      r8, r8, #1
003aaf90: str      r0, [r6, r8, lsl #2]
003aaf94: add      r3, r6, r8, lsl #2
003aaf98: str      r3, [r4, #0x10]
003aaf9c: ldr      ip, [r6, r8, lsl #2]
003aafa0: str      r3, [r4, #0x20]
003aafa4: ldr      r2, [pc, #0x3a4]
003aafa8: add      r3, ip, #0x4c
003aafac: str      r3, [r4, #0xc]
003aafb0: str      ip, [r4, #8]
003aafb4: ldr      r3, [r6, r8, lsl #2]
003aafb8: ldr      r1, [r5, r2]
003aafbc: mov      r0, r4
003aafc0: add      lr, r3, #0x4c
003aafc4: ldr      r2, [r5, r7]
003aafc8: str      lr, [r4, #0x1c]
003aafcc: str      ip, [r4, #4]
003aafd0: str      r3, [r4, #0x14]
003aafd4: str      r3, [r4, #0x18]
003aafd8: bl       #0x30e304
003aafdc: ldr      r3, [pc, #0x370]
003aafe0: ldr      ip, [r5, r3]
003aafe4: ldr      r3, [ip]
003aafe8: ands     r3, r3, #1
003aafec: bne      #0x3ab0bc
003aaff0: ldr      r6, [pc, #0x360]
003aaff4: ldr      r2, [pc, #0x360]
003aaff8: mov      lr, #8
003aaffc: ldr      r6, [r5, r6]
003ab000: ldr      r4, [r5, r2]
003ab004: mov      r8, #1
003ab008: add      r6, r6, lr
003ab00c: str      r8, [ip]
003ab010: mov      r1, lr
003ab014: mov      r2, r3
003ab018: str      lr, [r4, #0x28]
003ab01c: str      r6, [r4]
003ab020: str      r3, [r4, #4]
003ab024: str      r3, [r4, #8]
003ab028: str      r3, [r4, #0xc]
003ab02c: str      r3, [r4, #0x10]
003ab030: str      r3, [r4, #0x14]
003ab034: str      r3, [r4, #0x18]
003ab038: str      r3, [r4, #0x1c]
003ab03c: str      r3, [r4, #0x20]
003ab040: str      r3, [r4, #0x24]
003ab044: add      r0, r4, #0x24
003ab048: bl       #0x3293c8
003ab04c: mov      r3, #0x68
003ab050: mov      r6, r0
003ab054: add      r0, sp, #0x18
003ab058: ldr      r8, [r4, #0x28]
003ab05c: str      r3, [r0, #-4]!
003ab060: str      r6, [r4, #0x24]
003ab064: bl       #0x708ec0
003ab068: sub      r8, r8, #1
003ab06c: lsr      r8, r8, #1
003ab070: str      r0, [r6, r8, lsl #2]
003ab074: add      r3, r6, r8, lsl #2
003ab078: str      r3, [r4, #0x10]
003ab07c: ldr      ip, [r6, r8, lsl #2]
003ab080: str      r3, [r4, #0x20]
003ab084: ldr      r2, [pc, #0x2d4]
003ab088: add      r3, ip, #0x68
003ab08c: str      r3, [r4, #0xc]
003ab090: str      ip, [r4, #8]
003ab094: ldr      r3, [r6, r8, lsl #2]
003ab098: ldr      r1, [r5, r2]
003ab09c: mov      r0, r4
003ab0a0: add      lr, r3, #0x68
003ab0a4: ldr      r2, [r5, r7]
003ab0a8: str      lr, [r4, #0x1c]
003ab0ac: str      ip, [r4, #4]
003ab0b0: str      r3, [r4, #0x14]
003ab0b4: str      r3, [r4, #0x18]
003ab0b8: bl       #0x30e304
003ab0bc: ldr      r3, [pc, #0x2a0]
003ab0c0: ldr      ip, [r5, r3]
003ab0c4: ldr      r3, [ip]
003ab0c8: ands     r3, r3, #1
003ab0cc: bne      #0x3ab19c
003ab0d0: ldr      r6, [pc, #0x290]
003ab0d4: ldr      r2, [pc, #0x290]
003ab0d8: mov      lr, #8
003ab0dc: ldr      r6, [r5, r6]
003ab0e0: ldr      r4, [r5, r2]
003ab0e4: mov      r8, #1
003ab0e8: add      r6, r6, lr
003ab0ec: str      r8, [ip]
003ab0f0: mov      r1, lr
003ab0f4: mov      r2, r3
003ab0f8: str      lr, [r4, #0x28]
003ab0fc: str      r6, [r4]
003ab100: str      r3, [r4, #4]
003ab104: str      r3, [r4, #8]
003ab108: str      r3, [r4, #0xc]
003ab10c: str      r3, [r4, #0x10]
003ab110: str      r3, [r4, #0x14]
003ab114: str      r3, [r4, #0x18]
003ab118: str      r3, [r4, #0x1c]
003ab11c: str      r3, [r4, #0x20]
003ab120: str      r3, [r4, #0x24]
003ab124: add      r0, r4, #0x24
003ab128: bl       #0x329428
003ab12c: mov      r3, #0x78
003ab130: mov      r6, r0
003ab134: add      r0, sp, #0x18
003ab138: ldr      r8, [r4, #0x28]
003ab13c: str      r3, [r0, #-4]!
003ab140: str      r6, [r4, #0x24]
003ab144: bl       #0x708ec0
003ab148: sub      r8, r8, #1
003ab14c: lsr      r8, r8, #1
003ab150: str      r0, [r6, r8, lsl #2]
003ab154: add      r3, r6, r8, lsl #2
003ab158: str      r3, [r4, #0x10]
003ab15c: ldr      ip, [r6, r8, lsl #2]
003ab160: str      r3, [r4, #0x20]
003ab164: ldr      r2, [pc, #0x204]
003ab168: add      r3, ip, #0x78
003ab16c: str      r3, [r4, #0xc]
003ab170: str      ip, [r4, #8]
003ab174: ldr      r3, [r6, r8, lsl #2]
003ab178: ldr      r1, [r5, r2]
003ab17c: mov      r0, r4
003ab180: add      lr, r3, #0x78
003ab184: ldr      r2, [r5, r7]
003ab188: str      lr, [r4, #0x1c]
003ab18c: str      ip, [r4, #4]
003ab190: str      r3, [r4, #0x14]
003ab194: str      r3, [r4, #0x18]
003ab198: bl       #0x30e304
003ab19c: ldr      r3, [pc, #0x1d0]
003ab1a0: ldr      r3, [r5, r3]
003ab1a4: ldr      r2, [r3]
003ab1a8: tst      r2, #1
003ab1ac: beq      #0x3ab258
003ab1b0: ldr      r3, [pc, #0x1c0]
003ab1b4: ldr      r3, [r5, r3]
003ab1b8: ldr      r2, [r3]
003ab1bc: tst      r2, #1
003ab1c0: bne      #0x3ab1f0
003ab1c4: mov      r2, #1
003ab1c8: str      r2, [r3]
003ab1cc: ldr      r3, [pc, #0x1a8]
003ab1d0: ldr      r4, [r5, r3]
003ab1d4: mov      r0, r4
003ab1d8: bl       #0x522e2c
003ab1dc: ldr      r3, [pc, #0x19c]
003ab1e0: ldr      r2, [r5, r7]
003ab1e4: mov      r0, r4
003ab1e8: ldr      r1, [r5, r3]
003ab1ec: bl       #0x30e304
003ab1f0: add      sp, sp, #0x1c
003ab1f4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ab1f8: mov      r2, #1
003ab1fc: str      r2, [r3]
003ab200: ldr      r3, [pc, #0x17c]
003ab204: ldr      r4, [r5, r3]
003ab208: mov      r0, r4
003ab20c: bl       #0x32d79c
003ab210: ldr      r3, [pc, #0x170]
003ab214: mov      r0, r4
003ab218: ldr      r2, [r5, r7]
003ab21c: ldr      r1, [r5, r3]
003ab220: bl       #0x30e304
003ab224: b        #0x3aac38
003ab228: mov      r2, #1
003ab22c: str      r2, [r3]
003ab230: ldr      r3, [pc, #0x154]
003ab234: ldr      r4, [r5, r3]
003ab238: mov      r0, r4
003ab23c: bl       #0x3790a8
003ab240: ldr      r3, [pc, #0x148]
003ab244: mov      r0, r4
003ab248: mov      r2, r6
003ab24c: ldr      r1, [r5, r3]
003ab250: bl       #0x30e304
003ab254: b        #0x3aac24
003ab258: mov      r2, #1
003ab25c: str      r2, [r3]
003ab260: ldr      r3, [pc, #0x12c]
003ab264: ldr      r4, [r5, r3]
003ab268: mov      r0, r4
003ab26c: bl       #0x4932c4
003ab270: ldr      r3, [pc, #0x120]
003ab274: mov      r0, r4
003ab278: ldr      r2, [r5, r7]
003ab27c: ldr      r1, [r5, r3]
003ab280: bl       #0x30e304
003ab284: b        #0x3ab1b0
003ab288: ldrheq   r7, [pc], #-0xe4
003ab28c: subseq   sl, lr, r8, lsr #32
003ab290: andeq    r2, r0, r4, lsr #30
003ab294: ldrheq   r4, [r1], #-0x40
003ab298: andeq    r1, r0, r8, asr #16
003ab29c: subseq   r4, r1, ip, ror r4
003ab2a0: muleq    r0, r0, r8
003ab2a4: strheq   r3, [r0], -r0
003ab2a8: subseq   r4, r1, ip, asr #8
003ab2ac: andeq    r0, r0, r4, lsl #13
003ab2b0: subseq   r4, r1, r4, lsr #8
003ab2b4: andeq    r0, r0, ip, asr lr
003ab2b8: ldrsheq  r4, [r1], #-0x3c
003ab2bc: andeq    r2, r0, r8, ror #12
003ab2c0: ldrsbeq  r4, [r1], #-0x34
003ab2c4: andeq    r0, r0, r0, lsr #13
003ab2c8: subseq   r4, r1, r4, lsr #7
003ab2cc: andeq    r0, r0, r4, asr sl
003ab2d0: subseq   r4, r1, ip, ror r3
003ab2d4: andeq    r3, r0, r4, asr #22
003ab2d8: subseq   r4, r1, r4, asr r3
003ab2dc: andeq    r1, r0, r0, lsr #29
003ab2e0: subseq   r4, r1, ip, lsr #6
003ab2e4: andeq    r1, r0, r8, lsl r6
003ab2e8: subseq   r4, r1, r4, lsl #6
003ab2ec: andeq    r2, r0, r4, lsr r8
003ab2f0: ldrsbeq  r4, [r1], #-0x2c
003ab2f4: strdeq   r3, r4, [r0], -r0

# _ZN9Character6UpdateEv
003abe98: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003abe9c: ldr      r5, [pc, #0xe54]
003abea0: ldr      r6, [pc, #0xe54]
003abea4: mov      r4, r0
003abea8: add      r5, pc, r5
003abeac: ldr      r3, [r5, r6]
003abeb0: ldr      r0, [pc, #0xe48]
003abeb4: ldr      r7, [pc, #0xe48]
003abeb8: ldr      r3, [r3]
003abebc: sub      sp, sp, #0x9c
003abec0: add      r0, pc, r0
003abec4: str      r3, [sp, #0x94]
003abec8: bl       #0x3136b4
003abecc: ldr      sl, [r5, r7]
003abed0: add      r8, sp, #0x7c
003abed4: mov      r0, sl
003abed8: bl       #0x337888
003abedc: ldr      r1, [pc, #0xe24]
003abee0: add      r2, sp, #0x48
003abee4: mov      r0, r8
003abee8: add      r1, pc, r1
003abeec: bl       #0x3140ec
003abef0: mov      r0, sl
003abef4: mov      r1, r8
003abef8: bl       #0x337a88
003abefc: mov      sl, r0
003abf00: mov      r0, r8
003abf04: bl       #0x318254
003abf08: cmp      sl, #0
003abf0c: bne      #0x3ac300
003abf10: ldr      sl, [r5, r7]
003abf14: add      r8, sp, #0x64
003abf18: mov      r0, sl
003abf1c: bl       #0x337888
003abf20: ldr      r1, [pc, #0xde4]
003abf24: add      r2, sp, #0x44
003abf28: mov      r0, r8
003abf2c: add      r1, pc, r1
003abf30: bl       #0x3140ec
003abf34: mov      r0, sl
003abf38: mov      r1, r8
003abf3c: bl       #0x337a88
003abf40: mov      sl, r0
003abf44: mov      r0, r8
003abf48: bl       #0x318254
003abf4c: cmp      sl, #0
003abf50: bne      #0x3ac2d8
003abf54: ldr      r3, [r4]
003abf58: mov      r0, r4
003abf5c: mov      lr, pc
003abf60: ldr      pc, [r3, #0x148]
003abf64: cmp      r0, #0
003abf68: beq      #0x3ac204
003abf6c: ldr      r0, [pc, #0xd9c]
003abf70: add      r8, r4, #0x4f0
003abf74: add      r8, r8, #0xc
003abf78: add      r0, pc, r0
003abf7c: bl       #0x3136b4
003abf80: mov      r0, r8
003abf84: bl       #0x3c01ac
003abf88: cmp      r0, #0
003abf8c: bne      #0x3ac348
003abf90: ldr      sl, [pc, #0xd7c]
003abf94: add      r2, r4, #0x3c8
003abf98: str      r2, [sp, #8]
003abf9c: ldr      r0, [pc, #0xd74]
003abfa0: add      sb, sp, #0x4c
003abfa4: add      r0, pc, r0
003abfa8: bl       #0x3136b8
003abfac: ldr      fp, [r5, sl]
003abfb0: ldr      r3, [fp, #0x38]
003abfb4: ldr      r2, [r3, #0x5c]
003abfb8: add      r2, r2, #1
003abfbc: str      r2, [r3, #0x5c]
003abfc0: ldr      r3, [r4, #0x378]
003abfc4: mov      r0, r3
003abfc8: ldr      r3, [r3]
003abfcc: mov      lr, pc
003abfd0: ldr      pc, [r3, #8]
003abfd4: ldr      r7, [r5, r7]
003abfd8: mov      r0, r7
003abfdc: bl       #0x337888
003abfe0: ldr      r1, [pc, #0xd34]
003abfe4: add      r2, sp, #0x40
003abfe8: mov      r0, sb
003abfec: add      r1, pc, r1
003abff0: bl       #0x3140ec
003abff4: mov      r0, r7
003abff8: mov      r1, sb
003abffc: bl       #0x337a88
003ac000: mov      r7, r0
003ac004: mov      r0, sb
003ac008: bl       #0x318254
003ac00c: cmp      r7, #0
003ac010: bne      #0x3ac618
003ac014: movw     r3, #0x14e0
003ac018: ldr      r0, [r4, r3]
003ac01c: cmp      r0, #0
003ac020: beq      #0x3ac028
003ac024: bl       #0x317ae4
003ac028: add      r0, r4, #0x3b4
003ac02c: bl       #0x3db640
003ac030: ldr      r0, [sp, #8]
003ac034: bl       #0x3cfbf4
003ac038: mov      r0, r8
003ac03c: bl       #0x3c628c
003ac040: add      r0, r4, #0x490
003ac044: add      r0, r0, #0xc
003ac048: bl       #0x3caf3c
003ac04c: movw     r7, #0x14fc
003ac050: mov      r0, r4
003ac054: bl       #0x38cbe8
003ac058: ldr      r8, [r4, r7]
003ac05c: mov      r1, #0
003ac060: mov      r0, r8
003ac064: bl       #0x30e2f8
003ac068: cmp      r0, #0
003ac06c: bne      #0x3ac894
003ac070: ldr      r3, [pc, #0xca8]
003ac074: ldr      r3, [r5, r3]
003ac078: ldrb     r3, [r3]
003ac07c: cmp      r3, #0
003ac080: bne      #0x3ac22c
003ac084: ldr      r3, [pc, #0xc98]
003ac088: ldr      r3, [r5, r3]
003ac08c: ldrb     r3, [r3]
003ac090: cmp      r3, #0
003ac094: bne      #0x3ac22c
003ac098: ldr      r3, [r4]
003ac09c: mov      r0, r4
003ac0a0: mov      lr, pc
003ac0a4: ldr      pc, [r3, #0x28]
003ac0a8: cmp      r0, #0
003ac0ac: beq      #0x3ac27c
003ac0b0: mov      r0, r4
003ac0b4: bl       #0x3abb9c
003ac0b8: mov      r0, r4
003ac0bc: bl       #0x3a469c
003ac0c0: ldr      r3, [r5, sl]
003ac0c4: ldr      r3, [r3, #0x74]
003ac0c8: tst      r3, #7
003ac0cc: beq      #0x3ac888
003ac0d0: mov      r3, #0x1480
003ac0d4: ldrb     r3, [r4, r3]
003ac0d8: cmp      r3, #0
003ac0dc: beq      #0x3ac740
003ac0e0: movw     r3, #0x1494
003ac0e4: ldr      r3, [r4, r3]
003ac0e8: cmp      r3, #0
003ac0ec: beq      #0x3ac10c
003ac0f0: movw     r2, #0x1498
003ac0f4: ldr      r2, [r4, r2]
003ac0f8: cmp      r2, #0
003ac0fc: blt      #0x3ac10c
003ac100: ldr      r0, [r3, r2, lsl #2]
003ac104: mov      r1, #0
003ac108: bl       #0x492ef0
003ac10c: movw     r3, #0x149c
003ac110: ldr      r0, [r4, r3]
003ac114: cmp      r0, #0
003ac118: beq      #0x3ac124
003ac11c: mov      r1, #0
003ac120: bl       #0x492ef0
003ac124: bl       #0x3a42f4
003ac128: cmp      r0, #0
003ac12c: beq      #0x3ac148
003ac130: movw     r3, #0x14a0
003ac134: ldr      r0, [r4, r3]
003ac138: cmp      r0, #0
003ac13c: beq      #0x3ac148
003ac140: mov      r1, #1
003ac144: bl       #0x492ef0
003ac148: ldr      r3, [pc, #0xbd8]
003ac14c: ldr      r0, [r5, r3]
003ac150: bl       #0x455c54
003ac154: cmp      r0, #0
003ac158: bne      #0x3ac1f4
003ac15c: mov      r7, #0x1500
003ac160: ldr      r1, [r4, r7]
003ac164: cmn      r1, #1
003ac168: beq      #0x3ac17c
003ac16c: mov      r0, r4
003ac170: bl       #0x3aef68
003ac174: mvn      r3, #0
003ac178: str      r3, [r4, r7]
003ac17c: movw     r7, #0x1504
003ac180: ldr      r3, [r4, r7]
003ac184: cmn      r3, #1
003ac188: beq      #0x3ac1f4
003ac18c: ldr      sb, [r5, sl]
003ac190: mov      r1, r4
003ac194: ldr      r0, [sb, #0x40]
003ac198: bl       #0x36effc
003ac19c: cmp      r0, #0
003ac1a0: beq      #0x3ac1f4
003ac1a4: bl       #0x413e90
003ac1a8: ldr      r1, [pc, #0xb7c]
003ac1ac: ldr      sl, [r4, r7]
003ac1b0: add      r1, pc, r1
003ac1b4: bl       #0x414678
003ac1b8: ldr      r1, [pc, #0xb70]
003ac1bc: ldr      r2, [pc, #0xb70]
003ac1c0: mov      r8, r0
003ac1c4: add      r1, pc, r1
003ac1c8: add      r2, pc, r2
003ac1cc: ldr      r0, [sb, #0x2c]
003ac1d0: bl       #0x4c4bdc
003ac1d4: asr      sl, sl, #8
003ac1d8: mov      r3, r0
003ac1dc: mov      r1, sl
003ac1e0: mov      r0, r4
003ac1e4: mov      r2, r8
003ac1e8: bl       #0x3af0c8
003ac1ec: mvn      r3, #0
003ac1f0: str      r3, [r4, r7]
003ac1f4: ldr      r0, [pc, #0xb3c]
003ac1f8: add      r0, pc, r0
003ac1fc: bl       #0x3136b8
003ac200: b        #0x3ac210
003ac204: ldr      r0, [pc, #0xb30]
003ac208: add      r0, pc, r0
003ac20c: bl       #0x3136b8
003ac210: ldr      r3, [r5, r6]
003ac214: ldr      r2, [sp, #0x94]
003ac218: ldr      r3, [r3]
003ac21c: cmp      r2, r3
003ac220: bne      #0x3aca98
003ac224: add      sp, sp, #0x9c
003ac228: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ac22c: mov      r0, r4
003ac230: bl       #0x3a3094
003ac234: cmp      r0, #0
003ac238: beq      #0x3ac098
003ac23c: ldr      r3, [pc, #0xafc]
003ac240: ldr      r1, [r4, #0x164]
003ac244: ldr      r0, [r4, #0x160]
003ac248: ldr      r3, [r5, r3]
003ac24c: ldr      r2, [r4, #0x168]
003ac250: str      r1, [r3, #4]
003ac254: mov      r1, #0
003ac258: str      r0, [r3]
003ac25c: str      r1, [r3, #0xc]
003ac260: str      r2, [r3, #8]
003ac264: ldr      r3, [r4]
003ac268: mov      r0, r4
003ac26c: mov      lr, pc
003ac270: ldr      pc, [r3, #0x28]
003ac274: cmp      r0, #0
003ac278: bne      #0x3ac0b0
003ac27c: mov      r0, r4
003ac280: bl       #0x3a30c4
003ac284: cmp      r0, #0
003ac288: bne      #0x3ac574
003ac28c: movw     r3, #0x14e4
003ac290: ldrb     r3, [r4, r3]
003ac294: cmp      r3, #0
003ac298: bne      #0x3ac900
003ac29c: bl       #0x7fd794
003ac2a0: ldrb     r3, [r0, #5]
003ac2a4: cmp      r3, #0
003ac2a8: beq      #0x3ac0c0
003ac2ac: ldrb     r3, [r4, #0x118]
003ac2b0: cmp      r3, #0
003ac2b4: beq      #0x3ac0c0
003ac2b8: ldr      r3, [r4, #0x520]
003ac2bc: tst      r3, #0x100
003ac2c0: beq      #0x3ac0c0
003ac2c4: ldr      r3, [r4, #0x110]
003ac2c8: cmn      r3, #1
003ac2cc: moveq    r3, #0
003ac2d0: strbeq   r3, [r4, #0x118]
003ac2d4: b        #0x3ac0c0
003ac2d8: ldr      r3, [r4]
003ac2dc: mov      r0, r4
003ac2e0: mov      lr, pc
003ac2e4: ldr      pc, [r3, #0x28]
003ac2e8: cmp      r0, #0
003ac2ec: beq      #0x3abf54
003ac2f0: add      r0, r4, #0x37c
003ac2f4: mov      r1, #0x32
003ac2f8: bl       #0x3ffc40
003ac2fc: b        #0x3abf54
003ac300: ldr      sl, [pc, #0xa0c]
003ac304: mov      r1, #0
003ac308: mov      r2, r1
003ac30c: ldr      r3, [r5, sl]
003ac310: ldr      r0, [r3, #0x40]
003ac314: bl       #0x36e744
003ac318: ldr      r3, [r0, #0x660]
003ac31c: cmp      r4, r3
003ac320: bne      #0x3abf10
003ac324: movw     r3, #0x1088
003ac328: ldr      r2, [r4, r3]
003ac32c: cmp      r2, #0
003ac330: ble      #0x3ac940
003ac334: sub      r2, r2, #0xc8
003ac338: add      r0, r4, #0x560
003ac33c: mov      r1, #0x24
003ac340: bl       #0x3e07a0
003ac344: b        #0x3abf10
003ac348: mov      r0, r8
003ac34c: bl       #0x3c01ac
003ac350: cmp      r0, #0xc
003ac354: beq      #0x3ac608
003ac358: mov      r0, r8
003ac35c: bl       #0x3c01ac
003ac360: cmp      r0, #2
003ac364: beq      #0x3abf90
003ac368: mov      r3, #0x1480
003ac36c: ldrb     r3, [r4, r3]
003ac370: cmp      r3, #0
003ac374: bne      #0x3ac608
003ac378: ldr      r3, [r4, #0x3e4]
003ac37c: cmp      r3, #0
003ac380: bne      #0x3abf90
003ac384: ldr      sl, [pc, #0x988]
003ac388: ldr      r0, [r5, sl]
003ac38c: bl       #0x31f594
003ac390: ldr      r3, [r0, #0x3c]
003ac394: cmp      r3, #0x1d
003ac398: beq      #0x3aca48
003ac39c: ldr      sb, [pc, #0x9a0]
003ac3a0: ldr      r3, [r5, sb]
003ac3a4: ldr      r3, [r3, #0x10]
003ac3a8: cmp      r3, #8
003ac3ac: movls    r3, #0
003ac3b0: movhi    r3, #1
003ac3b4: cmp      r3, #0
003ac3b8: beq      #0x3ac3f4
003ac3bc: ldr      r3, [r5, sb]
003ac3c0: ldr      fp, [r3, #8]
003ac3c4: cmp      fp, r3
003ac3c8: beq      #0x3ac3f4
003ac3cc: ldr      r3, [fp, #0x14]
003ac3d0: mov      r1, #0
003ac3d4: mov      r2, #1
003ac3d8: ldr      r0, [r3, #0x378]
003ac3dc: bl       #0x40570c
003ac3e0: add      r1, sp, #0x98
003ac3e4: ldr      r0, [fp, #0x14]
003ac3e8: mov      r2, #1
003ac3ec: str      fp, [r1, #-0x5c]!
003ac3f0: bl       #0x3a7b24
003ac3f4: add      r3, r4, #0x3c8
003ac3f8: mov      r0, r3
003ac3fc: mov      r1, #1
003ac400: str      r3, [sp, #8]
003ac404: bl       #0x3cf3a4
003ac408: cmp      r0, #0
003ac40c: beq      #0x3abf9c
003ac410: mov      r0, r4
003ac414: bl       #0x3a3064
003ac418: cmp      r0, #0
003ac41c: beq      #0x3abf9c
003ac420: mov      r0, r4
003ac424: bl       #0x3a3144
003ac428: cmp      r0, #0
003ac42c: bne      #0x3abf9c
003ac430: mov      r0, r4
003ac434: bl       #0x3a3158
003ac438: cmp      r0, #0
003ac43c: bne      #0x3abf9c
003ac440: bl       #0x7fd794
003ac444: ldrb     r3, [r0, #5]
003ac448: cmp      r3, #0
003ac44c: bne      #0x3abf9c
003ac450: ldrb     r3, [r4, #0x3ec]
003ac454: cmp      r3, #0
003ac458: beq      #0x3abf9c
003ac45c: bl       #0x60b0cc
003ac460: ldr      r2, [r5, sb]
003ac464: ldr      ip, [r2, #4]
003ac468: cmp      ip, #0
003ac46c: moveq    ip, r2
003ac470: bne      #0x3ac480
003ac474: b        #0x3ac4b4
003ac478: mov      r2, ip
003ac47c: mov      ip, r3
003ac480: ldr      r3, [ip, #0x10]
003ac484: cmp      r0, r3
003ac488: ldrgt    r3, [ip, #0xc]
003ac48c: ldrle    r3, [ip, #8]
003ac490: movgt    ip, r2
003ac494: cmp      r3, #0
003ac498: bne      #0x3ac478
003ac49c: ldr      r3, [r5, sb]
003ac4a0: cmp      ip, r3
003ac4a4: beq      #0x3ac4b4
003ac4a8: ldr      r3, [ip, #0x10]
003ac4ac: cmp      r0, r3
003ac4b0: bge      #0x3ac56c
003ac4b4: ldr      r3, [r5, sb]
003ac4b8: mov      fp, #0
003ac4bc: str      r0, [sp, #0x30]
003ac4c0: ldr      lr, [r3, #8]
003ac4c4: str      fp, [sp, #0x34]
003ac4c8: cmp      lr, ip
003ac4cc: beq      #0x3acae4
003ac4d0: cmp      ip, r3
003ac4d4: beq      #0x3acc50
003ac4d8: ldrb     r3, [ip]
003ac4dc: cmp      r3, #0
003ac4e0: bne      #0x3ac4f8
003ac4e4: ldr      r3, [ip, #4]
003ac4e8: ldr      r3, [r3, #4]
003ac4ec: cmp      r3, ip
003ac4f0: ldreq    lr, [ip, #0xc]
003ac4f4: beq      #0x3ac518
003ac4f8: ldr      lr, [ip, #8]
003ac4fc: cmp      lr, #0
003ac500: bne      #0x3ac50c
003ac504: b        #0x3acc7c
003ac508: mov      lr, r3
003ac50c: ldr      r3, [lr, #0xc]
003ac510: cmp      r3, #0
003ac514: bne      #0x3ac508
003ac518: ldr      r2, [ip, #0x10]
003ac51c: cmp      r0, r2
003ac520: movge    fp, #0
003ac524: movlt    fp, #1
003ac528: cmp      fp, #0
003ac52c: str      r2, [sp, #0xc]
003ac530: beq      #0x3aca9c
003ac534: ldr      r3, [lr, #0x10]
003ac538: cmp      r0, r3
003ac53c: ble      #0x3aca9c
003ac540: ldr      r3, [lr, #0xc]
003ac544: cmp      r3, #0
003ac548: movne    r1, ip
003ac54c: beq      #0x3acbf8
003ac550: mov      ip, #0
003ac554: add      r0, sp, #0x38
003ac558: add      r2, sp, #0x30
003ac55c: mov      r3, r1
003ac560: str      ip, [sp]
003ac564: bl       #0x3aa77c
003ac568: ldr      ip, [sp, #0x38]
003ac56c: str      r4, [ip, #0x14]
003ac570: b        #0x3abf9c
003ac574: ldrb     r3, [r4, #0x8a]
003ac578: cmp      r3, #0
003ac57c: beq      #0x3ac28c
003ac580: ldrb     r3, [r4, #0x2fa]
003ac584: cmp      r3, #0
003ac588: bne      #0x3ac5e0
003ac58c: movw     r7, #0x1484
003ac590: ldr      r3, [r4, r7]
003ac594: cmp      r3, #0
003ac598: bne      #0x3ac0c0
003ac59c: ldr      r3, [pc, #0x7a4]
003ac5a0: mov      r1, #0x8b
003ac5a4: mov      r2, r4
003ac5a8: ldr      r0, [r5, r3]
003ac5ac: bl       #0x495430
003ac5b0: cmp      r0, #0
003ac5b4: str      r0, [r4, r7]
003ac5b8: beq      #0x3ac0c0
003ac5bc: bl       #0x49267c
003ac5c0: ldr      r3, [r0]
003ac5c4: mov      lr, pc
003ac5c8: ldr      pc, [r3, #0x44]
003ac5cc: mov      r1, #1
003ac5d0: ldr      r3, [r0]
003ac5d4: mov      lr, pc
003ac5d8: ldr      pc, [r3, #0x40]
003ac5dc: b        #0x3ac0c0
003ac5e0: movw     r3, #0x1484
003ac5e4: ldr      r3, [r4, r3]
003ac5e8: cmp      r3, #0
003ac5ec: beq      #0x3ac0c0
003ac5f0: ldr      r3, [pc, #0x750]
003ac5f4: add      r1, r4, #0x1480
003ac5f8: add      r1, r1, #4
003ac5fc: ldr      r0, [r5, r3]
003ac600: bl       #0x494978
003ac604: b        #0x3ac0c0
003ac608: add      r3, r4, #0x3c8
003ac60c: ldr      sl, [pc, #0x700]
003ac610: str      r3, [sp, #8]
003ac614: b        #0x3abf9c
003ac618: bl       #0x7fd794
003ac61c: ldrb     r3, [r0, #5]
003ac620: cmp      r3, #0
003ac624: beq      #0x3ac014
003ac628: ldr      r3, [r4]
003ac62c: mov      r0, r4
003ac630: mov      lr, pc
003ac634: ldr      pc, [r3, #0x28]
003ac638: cmp      r0, #0
003ac63c: beq      #0x3ac014
003ac640: ldr      r0, [fp, #0x40]
003ac644: mov      r1, r4
003ac648: mov      r2, #0
003ac64c: bl       #0x36eea8
003ac650: ldrb     r3, [r0, #0x66c]
003ac654: cmp      r3, #0
003ac658: beq      #0x3ac014
003ac65c: mov      r1, r4
003ac660: ldr      r0, [fp, #0x40]
003ac664: mov      r2, #0
003ac668: bl       #0x36eea8
003ac66c: ldr      r1, [r0, #0x678]
003ac670: cmp      r1, #0
003ac674: ble      #0x3ac014
003ac678: ldr      r0, [fp, #0x40]
003ac67c: sub      r1, r1, #1
003ac680: mov      r2, #0
003ac684: bl       #0x36e744
003ac688: ldr      r7, [r0, #0x660]
003ac68c: cmp      r7, #0
003ac690: beq      #0x3ac014
003ac694: ldr      sb, [pc, #0x6b0]
003ac698: mov      r0, r4
003ac69c: add      sb, pc, sb
003ac6a0: ldr      r2, [sb, #0x24]
003ac6a4: ldr      r3, [sb, #0x28]
003ac6a8: add      r2, r2, #1
003ac6ac: add      r3, r3, #1
003ac6b0: str      r2, [sb, #0x24]
003ac6b4: str      r3, [sb, #0x28]
003ac6b8: bl       #0x3bd110
003ac6bc: cmp      r0, #0x45
003ac6c0: ble      #0x3aca18
003ac6c4: movw     r3, #0x14a4
003ac6c8: ldr      r0, [r4, r3]
003ac6cc: cmp      r0, #0
003ac6d0: beq      #0x3ac6f4
003ac6d4: movw     r3, #0x14ac
003ac6d8: ldrb     r3, [r4, r3]
003ac6dc: cmp      r3, #0
003ac6e0: beq      #0x3ac978
003ac6e4: movw     r3, #0x14a8
003ac6e8: ldrsb    r3, [r4, r3]
003ac6ec: cmp      r3, #8
003ac6f0: beq      #0x3ac990
003ac6f4: add      fp, r7, #0x160
003ac6f8: add      sb, r4, #0x160
003ac6fc: mov      r1, sb
003ac700: mov      r0, fp
003ac704: bl       #0x3a4118
003ac708: mov      r1, #0x43000000
003ac70c: add      r1, r1, #0x960000
003ac710: bl       #0x30e2f8
003ac714: cmp      r0, #0
003ac718: bne      #0x3aca70
003ac71c: ldr      r3, [pc, #0x62c]
003ac720: add      r3, pc, r3
003ac724: ldr      r2, [r3, #0x28]
003ac728: cmp      r2, #0x3e8
003ac72c: bgt      #0x3aca80
003ac730: ldr      r0, [r4, #0x378]
003ac734: mov      r1, #0
003ac738: bl       #0x4053d0
003ac73c: b        #0x3ac014
003ac740: ldr      r3, [r5, sl]
003ac744: mov      r1, r4
003ac748: ldr      r0, [r3, #0x40]
003ac74c: bl       #0x36effc
003ac750: cmp      r0, #0
003ac754: beq      #0x3ac0e0
003ac758: movw     r3, #0x1494
003ac75c: ldr      r3, [r4, r3]
003ac760: cmp      r3, #0
003ac764: beq      #0x3ac820
003ac768: ldr      r7, [r4, #0x40c]
003ac76c: cmp      r7, #0
003ac770: beq      #0x3ac8b4
003ac774: cmp      r4, r7
003ac778: beq      #0x3ac8b4
003ac77c: movw     r3, #0x149c
003ac780: ldr      r0, [r4, r3]
003ac784: cmp      r0, #0
003ac788: beq      #0x3ac794
003ac78c: mov      r1, #0
003ac790: bl       #0x492ef0
003ac794: ldr      r3, [r7]
003ac798: mov      r0, r7
003ac79c: mov      r1, r4
003ac7a0: mov      lr, pc
003ac7a4: ldr      pc, [r3, #0x90]
003ac7a8: subs     r8, r0, #0
003ac7ac: blt      #0x3ac820
003ac7b0: cmp      r8, #1
003ac7b4: beq      #0x3ac930
003ac7b8: movw     sb, #0x1494
003ac7bc: ldr      r3, [r4, sb]
003ac7c0: ldr      r0, [r3, r8, lsl #2]
003ac7c4: cmp      r0, #0
003ac7c8: beq      #0x3ac820
003ac7cc: str      r7, [r0, #0x28]
003ac7d0: mov      r1, #1
003ac7d4: bl       #0x492aa0
003ac7d8: movw     r3, #0x1498
003ac7dc: ldr      r3, [r4, r3]
003ac7e0: cmp      r8, r3
003ac7e4: beq      #0x3ac820
003ac7e8: cmp      r3, #0
003ac7ec: blt      #0x3ac808
003ac7f0: ldr      r2, [r4, sb]
003ac7f4: ldr      r0, [r2, r3, lsl #2]
003ac7f8: cmp      r0, #0
003ac7fc: beq      #0x3ac80c
003ac800: mov      r1, #0
003ac804: bl       #0x492ef0
003ac808: ldr      r2, [r4, sb]
003ac80c: ldr      r0, [r2, r8, lsl #2]
003ac810: mov      r1, #1
003ac814: bl       #0x492ef0
003ac818: movw     r3, #0x1498
003ac81c: str      r8, [r4, r3]
003ac820: movw     r3, #0x149c
003ac824: ldr      r0, [r4, r3]
003ac828: cmp      r0, #0
003ac82c: beq      #0x3ac124
003ac830: ldr      r3, [r4, #0x378]
003ac834: ldrb     r2, [r3, #9]
003ac838: cmp      r2, #0
003ac83c: bne      #0x3ac860
003ac840: ldr      r2, [pc, #0x50c]
003ac844: ldr      r2, [r5, r2]
003ac848: ldrb     r2, [r2]
003ac84c: cmp      r2, #0
003ac850: bne      #0x3ac11c
003ac854: ldrb     r3, [r3, #8]
003ac858: cmp      r3, #0
003ac85c: bne      #0x3ac11c
003ac860: ldrb     r3, [r4, #0x1b5]
003ac864: cmp      r3, #0
003ac868: beq      #0x3ac11c
003ac86c: ldr      r0, [r5, sl]
003ac870: bl       #0x320e74
003ac874: cmp      r0, #0
003ac878: beq      #0x3ac124
003ac87c: movw     r3, #0x149c
003ac880: ldr      r0, [r4, r3]
003ac884: b        #0x3ac11c
003ac888: mov      r0, r4
003ac88c: bl       #0x3a4470
003ac890: b        #0x3ac0d0
003ac894: ldr      r0, [r5, sl]
003ac898: bl       #0x31f66c
003ac89c: bl       #0x30e2e0
003ac8a0: mov      r1, r0
003ac8a4: mov      r0, r8
003ac8a8: bl       #0x30e3ac
003ac8ac: str      r0, [r4, r7]
003ac8b0: b        #0x3ac070
003ac8b4: movw     r2, #0x14a4
003ac8b8: ldr      r7, [r4, r2]
003ac8bc: cmp      r7, #0
003ac8c0: bne      #0x3ac8f4
003ac8c4: movw     r7, #0x1498
003ac8c8: ldr      r2, [r4, r7]
003ac8cc: cmp      r2, #0
003ac8d0: blt      #0x3ac820
003ac8d4: ldr      r0, [r3, r2, lsl #2]
003ac8d8: cmp      r0, #0
003ac8dc: beq      #0x3ac820
003ac8e0: mov      r1, #0
003ac8e4: bl       #0x492ef0
003ac8e8: mvn      r3, #0
003ac8ec: str      r3, [r4, r7]
003ac8f0: b        #0x3ac820
003ac8f4: cmp      r4, r7
003ac8f8: beq      #0x3ac8c4
003ac8fc: b        #0x3ac77c
003ac900: bl       #0x7fd794
003ac904: ldrb     r3, [r0, #5]
003ac908: cmp      r3, #0
003ac90c: beq      #0x3ac29c
003ac910: movw     r2, #0x14e5
003ac914: ldrb     r3, [r4, r2]
003ac918: cmp      r3, #0
003ac91c: moveq    r1, #1
003ac920: strbeq   r1, [r4, r2]
003ac924: strbeq   r3, [r4, #0x118]
003ac928: beq      #0x3ac0c0
003ac92c: b        #0x3ac29c
003ac930: mov      r0, r7
003ac934: mov      r1, r4
003ac938: bl       #0x3ec048
003ac93c: b        #0x3ac7b8
003ac940: mov      r1, #0x24
003ac944: add      r0, r4, #0x560
003ac948: mov      r2, #0
003ac94c: bl       #0x3e07a0
003ac950: ldr      r3, [r4]
003ac954: mov      r0, r4
003ac958: mov      lr, pc
003ac95c: ldr      pc, [r3, #0x34]
003ac960: subs     r1, r0, #0
003ac964: bne      #0x3abf10
003ac968: ldr      r0, [r4, #0x378]
003ac96c: mov      r2, r1
003ac970: bl       #0x40570c
003ac974: b        #0x3abf10
003ac978: movw     r3, #0x14a8
003ac97c: ldrsb    r3, [r4, r3]
003ac980: cmp      r3, #1
003ac984: beq      #0x3ac9c8
003ac988: cmn      r3, #2
003ac98c: beq      #0x3ac9c8
003ac990: add      sb, r4, #0x160
003ac994: mov      r1, sb
003ac998: add      r0, r0, #0x160
003ac99c: bl       #0x3a4118
003ac9a0: mov      r1, #0x43000000
003ac9a4: add      r1, r1, #0x960000
003ac9a8: bl       #0x30e2f8
003ac9ac: subs     r1, r0, #0
003ac9b0: beq      #0x3aca64
003ac9b4: movw     r3, #0x14a4
003ac9b8: ldr      r1, [r4, r3]
003ac9bc: ldr      r0, [r4, #0x378]
003ac9c0: bl       #0x4053d0
003ac9c4: b        #0x3ac014
003ac9c8: ldr      r3, [r0, #0x3bc]
003ac9cc: cmp      r4, r3
003ac9d0: bne      #0x3ac6f4
003ac9d4: add      r0, r4, #0x37c
003ac9d8: bl       #0x3fe330
003ac9dc: cmp      r0, #0
003ac9e0: bne      #0x3ac6f4
003ac9e4: add      fp, r7, #0x160
003ac9e8: add      sb, r4, #0x160
003ac9ec: mov      r1, sb
003ac9f0: mov      r0, fp
003ac9f4: bl       #0x3a4118
003ac9f8: mov      r1, #0x43000000
003ac9fc: add      r1, r1, #0xfa0000
003aca00: bl       #0x30e70c
003aca04: cmp      r0, #0
003aca08: beq      #0x3ac6fc
003aca0c: movw     r3, #0x14a4
003aca10: ldr      r0, [r4, r3]
003aca14: b        #0x3ac994
003aca18: mov      r0, r4
003aca1c: bl       #0x3a5990
003aca20: cmp      r0, #0
003aca24: beq      #0x3ac6c4
003aca28: ldr      r3, [sb, #0x24]
003aca2c: cmp      r3, #0x64
003aca30: ble      #0x3ac6c4
003aca34: ldr      r0, [r4, #0x378]
003aca38: bl       #0x4056b0
003aca3c: mov      r3, #0
003aca40: str      r3, [sb, #0x24]
003aca44: b        #0x3ac014
003aca48: ldr      sb, [pc, #0x2f4]
003aca4c: ldr      r3, [r5, sb]
003aca50: ldr      r3, [r3, #0x10]
003aca54: cmp      r3, #0x18
003aca58: movls    r3, #0
003aca5c: movhi    r3, #1
003aca60: b        #0x3ac3b4
003aca64: ldr      r0, [r4, #0x378]
003aca68: bl       #0x4057fc
003aca6c: b        #0x3ac014
003aca70: mov      r1, r7
003aca74: ldr      r0, [r4, #0x378]
003aca78: bl       #0x4053d0
003aca7c: b        #0x3ac014
003aca80: str      r0, [r3, #0x28]
003aca84: ldr      r3, [r4]
003aca88: mov      r0, r4
003aca8c: mov      lr, pc
003aca90: ldr      pc, [r3, #0x138]
003aca94: b        #0x3ac730
003aca98: bl       #0x30e310
003aca9c: ldr      r3, [ip, #0xc]
003acaa0: cmp      r3, #0
003acaa4: beq      #0x3acb7c
003acaa8: mov      r1, r3
003acaac: b        #0x3acab4
003acab0: mov      r1, r2
003acab4: ldr      r2, [r1, #8]
003acab8: cmp      r2, #0
003acabc: bne      #0x3acab0
003acac0: cmp      fp, #0
003acac4: beq      #0x3acb28
003acac8: add      r0, sp, #0x10
003acacc: add      r1, sp, #0x30
003acad0: bl       #0x3aa8b4
003acad4: ldr      r3, [sp, #0x10]
003acad8: str      r3, [sp, #0x38]
003acadc: ldr      ip, [sp, #0x38]
003acae0: b        #0x3ac56c
003acae4: ldr      r3, [r3, #0x10]
003acae8: cmp      r3, fp
003acaec: beq      #0x3acc34
003acaf0: ldr      r3, [ip, #0x10]
003acaf4: cmp      r0, r3
003acaf8: blt      #0x3acc14
003acafc: ble      #0x3acb70
003acb00: ldr      r3, [ip, #0xc]
003acb04: cmp      r3, #0
003acb08: beq      #0x3acbac
003acb0c: mov      r1, r3
003acb10: b        #0x3acb18
003acb14: mov      r1, r2
003acb18: ldr      r2, [r1, #8]
003acb1c: cmp      r2, #0
003acb20: bne      #0x3acb14
003acb24: b        #0x3acbd8
003acb28: ldr      r2, [sp, #0xc]
003acb2c: cmp      r0, r2
003acb30: ble      #0x3acb70
003acb34: ldr      r2, [r5, sb]
003acb38: cmp      r1, r2
003acb3c: beq      #0x3acb4c
003acb40: ldr      r2, [r1, #0x10]
003acb44: cmp      r0, r2
003acb48: bge      #0x3acac8
003acb4c: cmp      r3, #0
003acb50: bne      #0x3ac550
003acb54: mov      r1, ip
003acb58: add      r0, sp, #0x38
003acb5c: add      r2, sp, #0x30
003acb60: str      ip, [sp]
003acb64: bl       #0x3aa77c
003acb68: ldr      ip, [sp, #0x38]
003acb6c: b        #0x3ac56c
003acb70: str      ip, [sp, #0x38]
003acb74: ldr      ip, [sp, #0x38]
003acb78: b        #0x3ac56c
003acb7c: ldr      r1, [ip, #4]
003acb80: mov      r2, ip
003acb84: b        #0x3acb90
003acb88: mov      r2, r1
003acb8c: ldr      r1, [r1, #4]
003acb90: ldr      lr, [r1, #0xc]
003acb94: cmp      lr, r2
003acb98: beq      #0x3acb88
003acb9c: ldr      lr, [r2, #0xc]
003acba0: cmp      r1, lr
003acba4: moveq    r1, r2
003acba8: b        #0x3acac0
003acbac: ldr      r1, [ip, #4]
003acbb0: mov      r2, ip
003acbb4: ldr      ip, [r1, #0xc]
003acbb8: cmp      ip, r2
003acbbc: bne      #0x3acbcc
003acbc0: mov      r2, r1
003acbc4: ldr      r1, [r1, #4]
003acbc8: b        #0x3acbb4
003acbcc: ldr      ip, [r2, #0xc]
003acbd0: cmp      r1, ip
003acbd4: moveq    r1, r2
003acbd8: ldr      r2, [r5, sb]
003acbdc: cmp      r1, r2
003acbe0: beq      #0x3accd8
003acbe4: ldr      r2, [r1, #0x10]
003acbe8: cmp      r0, r2
003acbec: bge      #0x3accbc
003acbf0: cmp      r3, #0
003acbf4: bne      #0x3ac550
003acbf8: mov      r1, lr
003acbfc: add      r0, sp, #0x38
003acc00: add      r2, sp, #0x30
003acc04: str      lr, [sp]
003acc08: bl       #0x3aa77c
003acc0c: ldr      ip, [sp, #0x38]
003acc10: b        #0x3ac56c
003acc14: mov      r1, ip
003acc18: mov      r3, ip
003acc1c: add      r0, sp, #0x38
003acc20: add      r2, sp, #0x30
003acc24: str      fp, [sp]
003acc28: bl       #0x3aa77c
003acc2c: ldr      ip, [sp, #0x38]
003acc30: b        #0x3ac56c
003acc34: add      r0, sp, #0x28
003acc38: add      r1, sp, #0x30
003acc3c: bl       #0x3aa8b4
003acc40: ldr      r3, [sp, #0x28]
003acc44: str      r3, [sp, #0x38]
003acc48: ldr      ip, [sp, #0x38]
003acc4c: b        #0x3ac56c
003acc50: ldr      r1, [ip, #0xc]
003acc54: ldr      r3, [r1, #0x10]
003acc58: cmp      r0, r3
003acc5c: ble      #0x3acca0
003acc60: mov      r3, fp
003acc64: add      r0, sp, #0x38
003acc68: add      r2, sp, #0x30
003acc6c: str      ip, [sp]
003acc70: bl       #0x3aa77c
003acc74: ldr      ip, [sp, #0x38]
003acc78: b        #0x3ac56c
003acc7c: ldr      lr, [ip, #4]
003acc80: mov      r3, ip
003acc84: b        #0x3acc90
003acc88: mov      r3, lr
003acc8c: ldr      lr, [lr, #4]
003acc90: ldr      r2, [lr, #8]
003acc94: cmp      r2, r3
003acc98: beq      #0x3acc88
003acc9c: b        #0x3ac518
003acca0: add      r0, sp, #0x18
003acca4: add      r1, sp, #0x30
003acca8: bl       #0x3aa8b4
003accac: ldr      r3, [sp, #0x18]
003accb0: str      r3, [sp, #0x38]
003accb4: ldr      ip, [sp, #0x38]
003accb8: b        #0x3ac56c
003accbc: add      r0, sp, #0x20
003accc0: add      r1, sp, #0x30
003accc4: bl       #0x3aa8b4
003accc8: ldr      r3, [sp, #0x20]
003acccc: str      r3, [sp, #0x38]
003accd0: ldr      ip, [sp, #0x38]
003accd4: b        #0x3ac56c
003accd8: mov      r1, lr
003accdc: add      r0, sp, #0x38
003acce0: add      r2, sp, #0x30
003acce4: mov      r3, #0
003acce8: str      lr, [sp]
003accec: bl       #0x3aa77c
003accf0: ldr      ip, [sp, #0x38]
003accf4: b        #0x3ac56c
003accf8: subseq   r8, lr, r8, ror #23
003accfc: andeq    r4, r0, ip, lsr #1
003acd00: ldrsbeq  r7, [r1], #-0x70
003acd04: andeq    r0, r0, r4, lsl #17
003acd08: subseq   r7, r1, r0, asr #15
003acd0c: subseq   r4, r1, ip, lsl #9
003acd10: subseq   r7, r1, r0, asr #14
003acd14: strdeq   r3, r4, [r0], -r4
003acd18: subseq   r7, r1, r4, lsl r7
003acd1c: subseq   r7, r1, ip, ror #13
003acd20: andeq    r4, r0, r8, lsr #9
003acd24: andeq    r2, r0, r4, ror r4
003acd28: andeq    r1, r0, r0, lsr #20
003acd2c: subseq   r7, r1, r0, lsr r5
003acd30: subseq   r7, r1, ip, lsr #10
003acd34: subseq   r7, r1, r0, asr #10

# _ZNSt3mapIiP9CharacterSt4lessIiESaISt4pairIKiS1_EEED1Ev
003a7968: push     {r4, lr}
003a796c: ldr      r3, [r0, #0x10]
003a7970: mov      r4, r0
003a7974: cmp      r3, #0
003a7978: beq      #0x3a7994
003a797c: ldr      r1, [r0, #4]
003a7980: bl       #0x3a7930
003a7984: mov      r3, #0
003a7988: str      r3, [r4, #0x10]
003a798c: stmib    r4, {r3, r4}
003a7990: str      r4, [r4, #0xc]
003a7994: mov      r0, r4
003a7998: pop      {r4, pc}

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

# _ZN9Character19UnLoadScriptProcessENSt4priv17_Rb_tree_iteratorISt4pairIKiPS_ENS0_11_MapTraitsTIS5_EEEEb
003a7b24: push     {r4, r5, r6, r7, r8, lr}
003a7b28: ldr      r4, [pc, #0x104]
003a7b2c: ldr      r5, [pc, #0x104]
003a7b30: ldr      r3, [r1]
003a7b34: add      r4, pc, r4
003a7b38: ldr      ip, [r4, r5]
003a7b3c: sub      sp, sp, #0x10
003a7b40: mov      r6, r0
003a7b44: cmp      r3, ip
003a7b48: mov      r7, r2
003a7b4c: beq      #0x3a7ba4
003a7b50: ldr      r2, [r4, r5]
003a7b54: cmp      r3, r2
003a7b58: beq      #0x3a7b90
003a7b5c: mov      r0, r3
003a7b60: add      r1, r2, #4
003a7b64: add      r3, r2, #0xc
003a7b68: add      r2, r2, #8
003a7b6c: bl       #0x336004
003a7b70: cmp      r0, #0
003a7b74: beq      #0x3a7b80
003a7b78: mov      r1, #0x18
003a7b7c: bl       #0x708f00
003a7b80: ldr      r3, [r4, r5]
003a7b84: ldr      r2, [r3, #0x10]
003a7b88: sub      r2, r2, #1
003a7b8c: str      r2, [r3, #0x10]
003a7b90: add      r0, r6, #0x3c8
003a7b94: mov      r1, r7
003a7b98: add      sp, sp, #0x10
003a7b9c: pop      {r4, r5, r6, r7, r8, lr}
003a7ba0: b        #0x3cc9dc
003a7ba4: ldr      r3, [r3, #8]
003a7ba8: ldr      r2, [sp, #8]
003a7bac: str      r0, [sp, #0xc]
003a7bb0: cmp      r3, ip
003a7bb4: str      r2, [sp]
003a7bb8: str      r0, [sp, #4]
003a7bbc: beq      #0x3a7bf8
003a7bc0: ldr      r2, [r3, #0x14]
003a7bc4: cmp      r6, r2
003a7bc8: beq      #0x3a7bf8
003a7bcc: ldr      r2, [r3, #0xc]
003a7bd0: cmp      r2, #0
003a7bd4: bne      #0x3a7be0
003a7bd8: b        #0x3a7c00
003a7bdc: mov      r2, r3
003a7be0: ldr      r3, [r2, #8]
003a7be4: cmp      r3, #0
003a7be8: bne      #0x3a7bdc
003a7bec: mov      r3, r2
003a7bf0: cmp      r3, ip
003a7bf4: bne      #0x3a7bc0
003a7bf8: str      r3, [r1]
003a7bfc: b        #0x3a7b50
003a7c00: ldr      r0, [r3, #4]
003a7c04: ldr      r8, [r0, #0xc]
003a7c08: cmp      r3, r8
003a7c0c: bne      #0x3a7c28
003a7c10: mov      r3, r0
003a7c14: ldr      r0, [r0, #4]
003a7c18: ldr      r2, [r0, #0xc]
003a7c1c: cmp      r2, r3
003a7c20: beq      #0x3a7c10
003a7c24: ldr      r2, [r3, #0xc]
003a7c28: cmp      r0, r2
003a7c2c: movne    r3, r0
003a7c30: b        #0x3a7bf0
003a7c34: subseq   ip, lr, ip, asr pc
003a7c38: andeq    r1, r0, r4, lsr r1

# _ZN12v2Controller8Cmd_KillEP10GameObjectb
0040570c: push     {r4, lr}
00405710: ldr      r3, [r0, #4]
00405714: mov      r0, r3
00405718: ldr      r3, [r3]
0040571c: mov      lr, pc
00405720: ldr      pc, [r3, #0x58]
00405724: pop      {r4, pc}

# _ZNSt4priv10_Rb_globalIbE20_Rebalance_for_eraseEPNS_18_Rb_tree_node_baseERS3_S4_S4_
00336004: push     {r4, r5, r6, r7}
00336008: ldr      ip, [r0, #8]
0033600c: cmp      ip, #0
00336010: ldreq    ip, [r0, #0xc]
00336014: beq      #0x3363ac
00336018: ldr      r5, [r0, #0xc]
0033601c: cmp      r5, #0
00336020: bne      #0x33602c
00336024: b        #0x3363ac
00336028: mov      r5, r4
0033602c: ldr      r4, [r5, #8]
00336030: cmp      r4, #0
00336034: bne      #0x336028
00336038: cmp      r0, r5
0033603c: ldr      r4, [r5, #0xc]
00336040: beq      #0x3363b4
00336044: str      r5, [ip, #4]
00336048: ldr      r3, [r0, #8]
0033604c: str      r3, [r5, #8]
00336050: ldr      ip, [r0, #0xc]
00336054: cmp      ip, r5
00336058: beq      #0x336084
0033605c: ldr      ip, [r5, #4]
00336060: cmp      r4, #0
00336064: strne    ip, [r4, #4]
00336068: ldrne    r3, [r5, #4]
0033606c: moveq    r3, ip
00336070: str      r4, [r3, #8]
00336074: ldr      r3, [r0, #0xc]
00336078: str      r3, [r5, #0xc]
0033607c: ldr      r3, [r0, #0xc]
00336080: str      r5, [r3, #4]
00336084: ldr      r3, [r1]
00336088: cmp      r3, r0
0033608c: streq    r5, [r1]
00336090: beq      #0x3360a8
00336094: ldr      r3, [r0, #4]
00336098: ldr      r2, [r3, #8]
0033609c: cmp      r2, r0
003360a0: streq    r5, [r3, #8]
003360a4: strne    r5, [r3, #0xc]
003360a8: ldr      r2, [r0, #4]
003360ac: ldrb     r3, [r5]
003360b0: str      r2, [r5, #4]
003360b4: ldrb     r2, [r0]
003360b8: strb     r2, [r5]
003360bc: strb     r3, [r0]
003360c0: mov      r5, r0
003360c4: cmp      r3, #0
003360c8: movne    r6, #0
003360cc: movne    r7, #1
003360d0: beq      #0x3360fc
003360d4: ldr      r3, [r1]
003360d8: cmp      r3, r4
003360dc: beq      #0x3363a0
003360e0: cmp      r4, #0
003360e4: beq      #0x336108
003360e8: ldrb     r3, [r4]
003360ec: cmp      r3, #0
003360f0: bne      #0x336108
003360f4: mov      r3, #1
003360f8: strb     r3, [r4]
003360fc: mov      r0, r5
00336100: pop      {r4, r5, r6, r7}
00336104: bx       lr
00336108: ldr      r3, [ip, #8]
0033610c: cmp      r3, r4
00336110: beq      #0x3361bc
00336114: ldrb     r0, [r3]
00336118: cmp      r0, #0
0033611c: bne      #0x336178
00336120: strb     r7, [r3]
00336124: ldr      r2, [ip, #8]
00336128: strb     r0, [ip]
0033612c: ldr      r3, [r2, #0xc]
00336130: str      r3, [ip, #8]
00336134: ldr      r3, [r2, #0xc]
00336138: cmp      r3, #0
0033613c: strne    ip, [r3, #4]
00336140: ldr      r3, [ip, #4]
00336144: str      r3, [r2, #4]
00336148: ldr      r3, [r1]
0033614c: cmp      ip, r3
00336150: streq    r2, [r1]
00336154: beq      #0x33616c
00336158: ldr      r3, [ip, #4]
0033615c: ldr      r0, [r3, #0xc]
00336160: cmp      ip, r0
00336164: streq    r2, [r3, #0xc]
00336168: strne    r2, [r3, #8]
0033616c: str      ip, [r2, #0xc]
00336170: ldr      r3, [ip, #8]
00336174: str      r2, [ip, #4]
00336178: ldr      r2, [r3, #0xc]
0033617c: cmp      r2, #0
00336180: beq      #0x336190
00336184: ldrb     r0, [r2]
00336188: cmp      r0, #0
0033618c: beq      #0x3362c0
00336190: ldr      r2, [r3, #8]
00336194: cmp      r2, #0
00336198: beq      #0x3361a8
0033619c: ldrb     r2, [r2]
003361a0: cmp      r2, #0
003361a4: beq      #0x336338
003361a8: strb     r6, [r3]
003361ac: ldr      r3, [ip, #4]
003361b0: mov      r4, ip
003361b4: mov      ip, r3
003361b8: b        #0x3360d4
003361bc: ldr      r3, [ip, #0xc]
003361c0: ldrb     r0, [r3]
003361c4: cmp      r0, #0
003361c8: bne      #0x336224
003361cc: strb     r7, [r3]
003361d0: ldr      r2, [ip, #0xc]
003361d4: strb     r0, [ip]
003361d8: ldr      r3, [r2, #8]
003361dc: str      r3, [ip, #0xc]
003361e0: ldr      r3, [r2, #8]
003361e4: cmp      r3, #0
003361e8: strne    ip, [r3, #4]
003361ec: ldr      r3, [ip, #4]
003361f0: str      r3, [r2, #4]
003361f4: ldr      r3, [r1]
003361f8: cmp      ip, r3
003361fc: streq    r2, [r1]
00336200: beq      #0x336218
00336204: ldr      r3, [ip, #4]
00336208: ldr      r0, [r3, #8]
0033620c: cmp      ip, r0
00336210: streq    r2, [r3, #8]
00336214: strne    r2, [r3, #0xc]
00336218: str      ip, [r2, #8]
0033621c: ldr      r3, [ip, #0xc]
00336220: str      r2, [ip, #4]
00336224: ldr      r2, [r3, #8]
00336228: cmp      r2, #0
0033622c: beq      #0x33623c
00336230: ldrb     r0, [r2]
00336234: cmp      r0, #0
00336238: beq      #0x33643c
0033623c: ldr      r2, [r3, #0xc]
00336240: cmp      r2, #0
00336244: beq      #0x3361a8
00336248: ldrb     r2, [r2]
0033624c: cmp      r2, #0
00336250: bne      #0x3361a8
00336254: ldrb     r0, [ip]
00336258: mov      r2, #1
0033625c: strb     r0, [r3]
00336260: strb     r2, [ip]
00336264: ldr      r3, [r3, #0xc]
00336268: cmp      r3, #0
0033626c: strbne   r2, [r3]
00336270: ldr      r3, [ip, #0xc]
00336274: ldr      r2, [r3, #8]
00336278: str      r2, [ip, #0xc]
0033627c: ldr      r2, [r3, #8]
00336280: cmp      r2, #0
00336284: strne    ip, [r2, #4]
00336288: ldr      r2, [ip, #4]
0033628c: str      r2, [r3, #4]
00336290: ldr      r2, [r1]
00336294: cmp      ip, r2
00336298: streq    r3, [r1]
0033629c: beq      #0x3362b4
003362a0: ldr      r2, [ip, #4]
003362a4: ldr      r1, [r2, #8]
003362a8: cmp      ip, r1
003362ac: streq    r3, [r2, #8]
003362b0: strne    r3, [r2, #0xc]
003362b4: str      ip, [r3, #8]
003362b8: str      r3, [ip, #4]
003362bc: b        #0x3363a0
003362c0: ldr      r0, [r3, #8]
003362c4: cmp      r0, #0
003362c8: beq      #0x3362d8
003362cc: ldrb     r0, [r0]
003362d0: cmp      r0, #0
003362d4: beq      #0x336338
003362d8: mov      r0, #1
003362dc: strb     r0, [r2]
003362e0: ldr      r2, [r3, #0xc]
003362e4: mov      r0, #0
003362e8: strb     r0, [r3]
003362ec: ldr      r0, [r2, #8]
003362f0: str      r0, [r3, #0xc]
003362f4: ldr      r0, [r2, #8]
003362f8: cmp      r0, #0
003362fc: strne    r3, [r0, #4]
00336300: ldr      r0, [r3, #4]
00336304: str      r0, [r2, #4]
00336308: ldr      r0, [r1]
0033630c: cmp      r3, r0
00336310: streq    r2, [r1]
00336314: beq      #0x33632c
00336318: ldr      r0, [r3, #4]
0033631c: ldr      r6, [r0, #8]
00336320: cmp      r3, r6
00336324: streq    r2, [r0, #8]
00336328: strne    r2, [r0, #0xc]
0033632c: str      r3, [r2, #8]
00336330: str      r2, [r3, #4]
00336334: ldr      r3, [ip, #8]
00336338: ldrb     r0, [ip]
0033633c: mov      r2, #1
00336340: strb     r0, [r3]
00336344: strb     r2, [ip]
00336348: ldr      r3, [r3, #8]
0033634c: cmp      r3, #0
00336350: strbne   r2, [r3]
00336354: ldr      r3, [ip, #8]
00336358: ldr      r2, [r3, #0xc]
0033635c: str      r2, [ip, #8]
00336360: ldr      r2, [r3, #0xc]
00336364: cmp      r2, #0
00336368: strne    ip, [r2, #4]
0033636c: ldr      r2, [ip, #4]
00336370: str      r2, [r3, #4]
00336374: ldr      r2, [r1]
00336378: cmp      ip, r2
0033637c: streq    r3, [r1]
00336380: beq      #0x336398
00336384: ldr      r2, [ip, #4]
00336388: ldr      r1, [r2, #0xc]
0033638c: cmp      ip, r1
00336390: streq    r3, [r2, #0xc]
00336394: strne    r3, [r2, #8]
00336398: str      ip, [r3, #0xc]
0033639c: str      r3, [ip, #4]
003363a0: cmp      r4, #0
003363a4: beq      #0x3360fc
003363a8: b        #0x3360f4
003363ac: mov      r4, ip
003363b0: mov      r5, r0
003363b4: ldr      ip, [r5, #4]
003363b8: cmp      r4, #0
003363bc: strne    ip, [r4, #4]
003363c0: ldr      r6, [r1]
003363c4: cmp      r6, r0
003363c8: streq    r4, [r1]
003363cc: beq      #0x3363e4
003363d0: ldr      r6, [r0, #4]
003363d4: ldr      r7, [r6, #8]
003363d8: cmp      r7, r0
003363dc: streq    r4, [r6, #8]
003363e0: strne    r4, [r6, #0xc]
003363e4: ldr      r6, [r2]
003363e8: cmp      r6, r0
003363ec: beq      #0x3364b8
003363f0: ldr      r2, [r3]
003363f4: cmp      r2, r0
003363f8: ldrbne   r3, [r5]
003363fc: bne      #0x3360c4
00336400: ldr      r2, [r0, #8]
00336404: cmp      r2, #0
00336408: ldreq    r2, [r0, #4]
0033640c: streq    r2, [r3]
00336410: ldrbeq   r3, [r5]
00336414: beq      #0x3360c4
00336418: mov      r0, r4
0033641c: b        #0x336424
00336420: mov      r0, r2
00336424: ldr      r2, [r0, #0xc]
00336428: cmp      r2, #0
0033642c: bne      #0x336420
00336430: str      r0, [r3]
00336434: ldrb     r3, [r5]
00336438: b        #0x3360c4
0033643c: ldr      r0, [r3, #0xc]
00336440: cmp      r0, #0
00336444: beq      #0x336454
00336448: ldrb     r0, [r0]
0033644c: cmp      r0, #0
00336450: beq      #0x336254
00336454: mov      r0, #1
00336458: strb     r0, [r2]
0033645c: ldr      r2, [r3, #8]
00336460: mov      r0, #0
00336464: strb     r0, [r3]
00336468: ldr      r0, [r2, #0xc]
0033646c: str      r0, [r3, #8]
00336470: ldr      r0, [r2, #0xc]
00336474: cmp      r0, #0
00336478: strne    r3, [r0, #4]
0033647c: ldr      r0, [r3, #4]
00336480: str      r0, [r2, #4]
00336484: ldr      r0, [r1]
00336488: cmp      r3, r0
0033648c: streq    r2, [r1]
00336490: beq      #0x3364a8
00336494: ldr      r0, [r3, #4]
00336498: ldr      r6, [r0, #0xc]
0033649c: cmp      r3, r6
003364a0: streq    r2, [r0, #0xc]
003364a4: strne    r2, [r0, #8]
003364a8: str      r3, [r2, #0xc]
003364ac: str      r2, [r3, #4]
003364b0: ldr      r3, [ip, #0xc]
003364b4: b        #0x336254
003364b8: ldr      r6, [r0, #0xc]
003364bc: cmp      r6, #0
003364c0: ldreq    r6, [r0, #4]
003364c4: streq    r6, [r2]
003364c8: beq      #0x3363f0
003364cc: mov      r7, r4
003364d0: b        #0x3364d8
003364d4: mov      r7, r6
003364d8: ldr      r6, [r7, #8]
003364dc: cmp      r6, #0
003364e0: bne      #0x3364d4
003364e4: str      r7, [r2]
003364e8: b        #0x3363f0

# _ZN6CharAI21AIUnLoadScriptProcessEb
003cc9dc: push     {r4, r5, r6, r7, r8, lr}
003cc9e0: ldr      r3, [r0, #0x20]
003cc9e4: cmp      r1, #0
003cc9e8: movne    r2, #1
003cc9ec: ldrbeq   r2, [r0, #0x24]
003cc9f0: cmp      r3, #0
003cc9f4: mov      r5, r0
003cc9f8: beq      #0x3cca04
003cc9fc: cmp      r2, #0
003cca00: bne      #0x3cca08
003cca04: pop      {r4, r5, r6, r7, r8, pc}
003cca08: bl       #0x3d8ae0
003cca0c: mov      r0, r5
003cca10: bl       #0x3d8a98
003cca14: ldr      r4, [r5, #0xb4]
003cca18: ldr      r6, [r5, #0xb8]
003cca1c: cmp      r4, r6
003cca20: beq      #0x3cca64
003cca24: mov      r7, #0
003cca28: ldr      r3, [r4]
003cca2c: cmp      r3, #0
003cca30: beq      #0x3cca48
003cca34: mov      r0, r3
003cca38: ldr      r3, [r3]
003cca3c: mov      lr, pc
003cca40: ldr      pc, [r3, #4]
003cca44: str      r7, [r4]
003cca48: add      r4, r4, #4
003cca4c: cmp      r6, r4
003cca50: bne      #0x3cca28
003cca54: ldr      r3, [r5, #0xb4]
003cca58: ldr      r2, [r5, #0xb8]
003cca5c: cmp      r3, r2
003cca60: strne    r3, [r5, #0xb8]
003cca64: ldr      r4, [r5, #0xc0]
003cca68: ldr      r6, [r5, #0xc4]
003cca6c: cmp      r4, r6
003cca70: beq      #0x3ccab4
003cca74: mov      r7, #0
003cca78: ldr      r3, [r4]
003cca7c: cmp      r3, #0
003cca80: beq      #0x3cca98
003cca84: mov      r0, r3
003cca88: ldr      r3, [r3]
003cca8c: mov      lr, pc
003cca90: ldr      pc, [r3, #4]
003cca94: str      r7, [r4]
003cca98: add      r4, r4, #4
003cca9c: cmp      r6, r4
003ccaa0: bne      #0x3cca78
003ccaa4: ldr      r3, [r5, #0xc0]
003ccaa8: ldr      r2, [r5, #0xc4]
003ccaac: cmp      r3, r2
003ccab0: strne    r3, [r5, #0xc4]
003ccab4: ldr      r3, [r5, #0x20]
003ccab8: cmp      r3, #0
003ccabc: beq      #0x3ccad8
003ccac0: mov      r0, r3
003ccac4: ldr      r3, [r3]
003ccac8: mov      lr, pc
003ccacc: ldr      pc, [r3, #4]
003ccad0: mov      r3, #0
003ccad4: str      r3, [r5, #0x20]
003ccad8: mov      r3, #0
003ccadc: str      r3, [r5, #0x30]
003ccae0: str      r3, [r5, #0x20]
003ccae4: str      r3, [r5, #0x1c]
003ccae8: str      r3, [r5, #0x28]
003ccaec: strb     r3, [r5, #0x2c]
003ccaf0: pop      {r4, r5, r6, r7, r8, pc}

# _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiP9CharacterENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
003a7930: push     {r4, r5, r6, lr}
003a7934: subs     r4, r1, #0
003a7938: mov      r6, r0
003a793c: beq      #0x3a7964
003a7940: ldr      r1, [r4, #0xc]
003a7944: mov      r0, r6
003a7948: bl       #0x3a7930
003a794c: ldr      r5, [r4, #8]
003a7950: mov      r0, r4
003a7954: mov      r1, #0x18
003a7958: bl       #0x708f00
003a795c: subs     r4, r5, #0
003a7960: bne      #0x3a7940
003a7964: pop      {r4, r5, r6, pc}
