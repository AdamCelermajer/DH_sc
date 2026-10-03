
# _ZSt11__push_heapIPN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEiS7_NS6_6_ECompEEvT_T0_SB_T1_T2_.clone.15
00529398: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0052939c: subs     r5, r1, #0
005293a0: mov      r4, r0
005293a4: mov      sb, r2
005293a8: ble      #0x52942c
005293ac: sub      r7, r5, #1
005293b0: asr      r7, r7, #1
005293b4: mov      sl, #0xc
005293b8: mul      r8, sl, r7
005293bc: ldr      r1, [sb, #8]
005293c0: add      r6, r4, r8
005293c4: ldr      r0, [r6, #8]
005293c8: bl       #0x30e2f8
005293cc: mul      r3, sl, r5
005293d0: cmp      r0, #0
005293d4: add      r2, r6, #4
005293d8: add      r1, r4, r3
005293dc: beq      #0x52942c
005293e0: ldr      ip, [r4, r8]
005293e4: cmp      r7, #0
005293e8: sub      r0, r7, #1
005293ec: str      ip, [r4, r3]
005293f0: ldr      r3, [r6, #4]
005293f4: mov      r5, r7
005293f8: asr      r7, r0, #1
005293fc: str      r3, [r1, #4]
00529400: ldr      r3, [r2, #4]
00529404: str      r3, [r1, #8]
00529408: bne      #0x5293b8
0052940c: mov      r3, sb
00529410: ldr      r1, [r3], #4
00529414: str      r1, [r6]
00529418: ldr      r1, [sb, #4]
0052941c: str      r1, [r6, #4]
00529420: ldr      r3, [r3, #4]
00529424: str      r3, [r2, #4]
00529428: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0052942c: mov      r6, #0xc
00529430: mla      r6, r6, r5, r4
00529434: add      r2, r6, #4
00529438: b        #0x52940c

# _ZNSaINSt4priv13_Rb_tree_nodeISt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEEEEE8allocateEjPKv.clone.18
00529734: str      lr, [sp, #-4]!
00529738: sub      sp, sp, #0xc
0052973c: add      r0, sp, #8
00529740: mov      r3, #0x20
00529744: str      r3, [r0, #-4]!
00529748: bl       #0x708ec0
0052974c: add      sp, sp, #0xc
00529750: ldm      sp!, {pc}

# _ZNSaINSt4priv10_List_nodeIPK12PFGInnerEdgeEEE8allocateEjPKv.clone.5
0052aa84: str      lr, [sp, #-4]!
0052aa88: sub      sp, sp, #0xc
0052aa8c: add      r0, sp, #8
0052aa90: mov      r3, #0xc
0052aa94: str      r3, [r0, #-4]!
0052aa98: bl       #0x708ec0
0052aa9c: add      sp, sp, #0xc
0052aaa0: ldm      sp!, {pc}

# _ZNK3sfc4math5graph4EdgeI12PFGInnerNodefE9getToNodeEv
0051bc50: ldr      r0, [r0, #8]
0051bc54: bx       lr

# _ZNK3sfc4math5graph11GraphSparseI12PFGInnerEdgeE8getEdgesEjRSt4listIPKS3_SaIS7_EE
0052ac30: push     {r4, r5, r6, r7, r8, lr}
0052ac34: ldr      r4, [r0, #8]
0052ac38: mov      r6, r2
0052ac3c: add      r3, r0, #4
0052ac40: cmp      r4, #0
0052ac44: beq      #0x52ad04
0052ac48: mov      r0, r3
0052ac4c: b        #0x52ac54
0052ac50: mov      r4, r2
0052ac54: ldr      r2, [r4, #0x10]
0052ac58: cmp      r1, r2
0052ac5c: ldrhi    r2, [r4, #0xc]
0052ac60: ldrls    r2, [r4, #8]
0052ac64: movhi    r4, r0
0052ac68: mov      r0, r4
0052ac6c: cmp      r2, #0
0052ac70: bne      #0x52ac50
0052ac74: cmp      r3, r4
0052ac78: beq      #0x52ad44
0052ac7c: ldr      r2, [r4, #0x10]
0052ac80: cmp      r1, r2
0052ac84: blo      #0x52ad04
0052ac88: cmp      r3, r4
0052ac8c: beq      #0x52ad44
0052ac90: ldr      r3, [r4, #0x14]
0052ac94: ldr      r5, [r3, #0x34]
0052ac98: add      r2, r3, #0x2c
0052ac9c: cmp      r2, r5
0052aca0: beq      #0x52acfc
0052aca4: mov      r0, r6
0052aca8: ldr      r7, [r5, #0x14]
0052acac: bl       #0x52aa84
0052acb0: str      r7, [r0, #8]
0052acb4: ldr      r3, [r6, #4]
0052acb8: str      r6, [r0]
0052acbc: str      r3, [r0, #4]
0052acc0: str      r0, [r3]
0052acc4: str      r0, [r6, #4]
0052acc8: ldr      r2, [r5, #0xc]
0052accc: cmp      r2, #0
0052acd0: beq      #0x52ad0c
0052acd4: mov      r5, r2
0052acd8: b        #0x52ace0
0052acdc: mov      r5, r3
0052ace0: ldr      r3, [r5, #8]
0052ace4: cmp      r3, #0
0052ace8: bne      #0x52acdc
0052acec: ldr      r3, [r4, #0x14]
0052acf0: add      r2, r3, #0x2c
0052acf4: cmp      r2, r5
0052acf8: bne      #0x52aca4
0052acfc: ldr      r0, [r3, #0x3c]
0052ad00: pop      {r4, r5, r6, r7, r8, pc}
0052ad04: mov      r4, r3
0052ad08: b        #0x52ac88
0052ad0c: ldr      r3, [r5, #4]
0052ad10: ldr      r1, [r3, #0xc]
0052ad14: cmp      r5, r1
0052ad18: bne      #0x52ad34
0052ad1c: mov      r5, r3
0052ad20: ldr      r3, [r3, #4]
0052ad24: ldr      r2, [r3, #0xc]
0052ad28: cmp      r2, r5
0052ad2c: beq      #0x52ad1c
0052ad30: ldr      r2, [r5, #0xc]
0052ad34: cmp      r3, r2
0052ad38: movne    r5, r3
0052ad3c: ldr      r3, [r4, #0x14]
0052ad40: b        #0x52acf0
0052ad44: mov      r0, #0
0052ad48: pop      {r4, r5, r6, r7, r8, pc}

# _ZSt10__pop_heapIPN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeES7_NS6_6_ECompEiEvT_SA_SA_T0_T1_PT2_
0052943c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00529440: rsb      r1, r0, r1
00529444: mov      r4, r0
00529448: ldr      ip, [r0], #4
0052944c: mov      lr, r2
00529450: asr      r1, r1, #2
00529454: str      ip, [lr], #4
00529458: ldr      ip, [r4, #4]
0052945c: add      fp, r1, r1, lsl #2
00529460: sub      sp, sp, #0x24
00529464: str      ip, [r2, #4]
00529468: ldr      r2, [r0, #4]
0052946c: add      fp, fp, fp, lsl #4
00529470: str      r2, [lr, #4]
00529474: ldr      r2, [r3, #8]
00529478: add      fp, fp, fp, lsl #8
0052947c: str      r2, [sp, #0xc]
00529480: ldr      ip, [r3]
00529484: add      fp, fp, fp, lsl #16
00529488: str      ip, [sp, #4]
0052948c: ldr      r3, [r3, #4]
00529490: add      fp, r1, fp, lsl #1
00529494: cmp      fp, #2
00529498: str      r3, [sp, #8]
0052949c: movle    r5, #0
005294a0: movle    r2, #2
005294a4: ble      #0x529518
005294a8: mov      sb, #0
005294ac: mov      r5, #2
005294b0: mov      r7, #0xc
005294b4: b        #0x5294bc
005294b8: mov      r5, r2
005294bc: sub      r8, r5, #1
005294c0: mla      r6, r7, r5, r4
005294c4: mla      sl, r7, r8, r4
005294c8: ldr      r0, [r6, #8]
005294cc: ldr      r1, [sl, #8]
005294d0: bl       #0x30e2f8
005294d4: cmp      r0, #0
005294d8: movne    r6, sl
005294dc: mov      r3, r6
005294e0: ldr      r2, [r3], #4
005294e4: mul      sb, r7, sb
005294e8: movne    r5, r8
005294ec: str      r2, [r4, sb]
005294f0: ldr      r0, [r6, #4]
005294f4: add      r1, r4, sb
005294f8: add      r2, r5, #1
005294fc: str      r0, [r1, #4]
00529500: ldr      r3, [r3, #4]
00529504: lsl      r2, r2, #1
00529508: cmp      fp, r2
0052950c: mov      sb, r5
00529510: str      r3, [r1, #8]
00529514: bgt      #0x5294b8
00529518: cmp      fp, r2
0052951c: beq      #0x529554
00529520: ldr      ip, [sp, #4]
00529524: mov      r0, r4
00529528: mov      r1, r5
0052952c: str      ip, [sp, #0x14]
00529530: ldr      ip, [sp, #8]
00529534: add      r2, sp, #0x14
00529538: mov      r3, #0
0052953c: str      ip, [sp, #0x18]
00529540: ldr      ip, [sp, #0xc]
00529544: str      ip, [sp, #0x1c]
00529548: bl       #0x529398
0052954c: add      sp, sp, #0x24
00529550: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00529554: mov      r3, #0xc
00529558: sub      fp, fp, #1
0052955c: mul      r2, r3, fp
00529560: mul      r5, r3, r5
00529564: ldr      r1, [r4, r2]
00529568: add      r2, r4, r2
0052956c: add      r3, r4, r5
00529570: str      r1, [r4, r5]
00529574: ldr      r1, [r2, #4]
00529578: mov      r5, fp
0052957c: str      r1, [r3, #4]
00529580: ldr      r2, [r2, #8]
00529584: str      r2, [r3, #8]
00529588: b        #0x529520

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
0052a06c: push     {r4, r5, r6, lr}
0052a070: subs     r4, r1, #0
0052a074: mov      r6, r0
0052a078: beq      #0x52a0a0
0052a07c: ldr      r1, [r4, #0xc]
0052a080: mov      r0, r6
0052a084: bl       #0x52a06c
0052a088: ldr      r5, [r4, #8]
0052a08c: mov      r0, r4
0052a090: mov      r1, #0x20
0052a094: bl       #0x708f00
0052a098: subs     r4, r5, #0
0052a09c: bne      #0x52a07c
0052a0a0: pop      {r4, r5, r6, pc}

# _ZNK3sfc4math5graph4EdgeI12PFGInnerNodefE11getFromNodeEv
0051bc48: ldr      r0, [r0, #4]
0051bc4c: bx       lr

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

# _ZNSt6vectorIN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeESaIS7_EED1Ev
0052a640: push     {r4, lr}
0052a644: mov      r4, r0
0052a648: ldr      r0, [r0]
0052a64c: cmp      r0, #0
0052a650: beq      #0x52a688
0052a654: ldr      r3, [r4, #8]
0052a658: rsb      r3, r0, r3
0052a65c: asr      r3, r3, #2
0052a660: add      r1, r3, r3, lsl #2
0052a664: add      r1, r1, r1, lsl #4
0052a668: add      r1, r1, r1, lsl #8
0052a66c: add      r1, r1, r1, lsl #16
0052a670: add      r3, r3, r1, lsl #1
0052a674: mov      r1, #0xc
0052a678: mul      r1, r1, r3
0052a67c: cmp      r1, #0x80
0052a680: bhi      #0x52a690
0052a684: bl       #0x708f00
0052a688: mov      r0, r4
0052a68c: pop      {r4, pc}
0052a690: bl       #0x310440
0052a694: mov      r0, r4
0052a698: pop      {r4, pc}

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE13insert_uniqueENS_17_Rb_tree_iteratorISD_SH_EERKSD_
00529a30: push     {r4, r5, r6, r7, r8, sl, lr}
00529a34: ldr      r4, [r2]
00529a38: ldr      r2, [r1, #8]
00529a3c: sub      sp, sp, #0x2c
00529a40: mov      r5, r1
00529a44: cmp      r4, r2
00529a48: mov      r7, r0
00529a4c: mov      r6, r3
00529a50: beq      #0x529bc0
00529a54: cmp      r4, r1
00529a58: beq      #0x529c40
00529a5c: ldrb     r3, [r4]
00529a60: cmp      r3, #0
00529a64: beq      #0x529b54
00529a68: ldr      ip, [r4, #8]
00529a6c: cmp      ip, #0
00529a70: bne      #0x529a7c
00529a74: b        #0x529b74
00529a78: mov      ip, r3
00529a7c: ldr      r3, [ip, #0xc]
00529a80: cmp      r3, #0
00529a84: bne      #0x529a78
00529a88: ldr      r2, [r6]
00529a8c: ldr      r0, [r4, #0x10]
00529a90: cmp      r2, r0
00529a94: movhs    r1, #0
00529a98: movlo    r1, #1
00529a9c: cmp      r1, #0
00529aa0: bne      #0x529b14
00529aa4: ldr      r8, [r4, #0xc]
00529aa8: cmp      r8, #0
00529aac: beq      #0x529ca8
00529ab0: mov      ip, r8
00529ab4: b        #0x529abc
00529ab8: mov      ip, r3
00529abc: ldr      r3, [ip, #8]
00529ac0: cmp      r3, #0
00529ac4: bne      #0x529ab8
00529ac8: cmp      r1, #0
00529acc: bne      #0x529ba4
00529ad0: cmp      r2, r0
00529ad4: bls      #0x529c68
00529ad8: cmp      r5, ip
00529adc: beq      #0x529aec
00529ae0: ldr      r3, [ip, #0x10]
00529ae4: cmp      r2, r3
00529ae8: bhs      #0x529ba4
00529aec: cmp      r8, #0
00529af0: bne      #0x529c20
00529af4: mov      r1, r5
00529af8: mov      r2, r4
00529afc: mov      r3, r6
00529b00: mov      r0, r7
00529b04: str      r8, [sp]
00529b08: str      r4, [sp, #4]
00529b0c: bl       #0x529754
00529b10: b        #0x529b48
00529b14: ldr      r3, [ip, #0x10]
00529b18: cmp      r2, r3
00529b1c: bls      #0x529aa4
00529b20: ldr      lr, [ip, #0xc]
00529b24: cmp      lr, #0
00529b28: beq      #0x529c88
00529b2c: mov      ip, #0
00529b30: mov      r1, r5
00529b34: mov      r2, r4
00529b38: mov      r3, r6
00529b3c: mov      r0, r7
00529b40: stm      sp, {r4, ip}
00529b44: bl       #0x529754
00529b48: mov      r0, r7
00529b4c: add      sp, sp, #0x2c
00529b50: pop      {r4, r5, r6, r7, r8, sl, pc}
00529b54: ldr      r3, [r4, #4]
00529b58: ldr      r3, [r3, #4]
00529b5c: cmp      r4, r3
00529b60: ldreq    ip, [r4, #0xc]
00529b64: beq      #0x529a88
00529b68: ldr      ip, [r4, #8]
00529b6c: cmp      ip, #0
00529b70: bne      #0x529a7c
00529b74: ldr      ip, [r4, #4]
00529b78: ldr      r3, [ip, #8]
00529b7c: cmp      r4, r3
00529b80: beq      #0x529b8c
00529b84: b        #0x529a88
00529b88: mov      ip, r3
00529b8c: ldr      r3, [ip, #4]
00529b90: ldr      r2, [r3, #8]
00529b94: cmp      r2, ip
00529b98: beq      #0x529b88
00529b9c: mov      ip, r3
00529ba0: b        #0x529a88
00529ba4: mov      r1, r5
00529ba8: mov      r2, r6
00529bac: add      r0, sp, #8
00529bb0: bl       #0x5298a8
00529bb4: ldr      r3, [sp, #8]
00529bb8: str      r3, [r7]
00529bbc: b        #0x529b48
00529bc0: ldr      r2, [r1, #0x10]
00529bc4: cmp      r2, #0
00529bc8: beq      #0x529d18
00529bcc: ldr      r2, [r3]
00529bd0: ldr      ip, [r4, #0x10]
00529bd4: cmp      r2, ip
00529bd8: blo      #0x529d30
00529bdc: bls      #0x529c68
00529be0: ldr      lr, [r4, #0xc]
00529be4: cmp      lr, #0
00529be8: beq      #0x529ce0
00529bec: mov      ip, lr
00529bf0: b        #0x529bf8
00529bf4: mov      ip, r3
00529bf8: ldr      r3, [ip, #8]
00529bfc: cmp      r3, #0
00529c00: bne      #0x529bf4
00529c04: cmp      r5, ip
00529c08: beq      #0x529d80
00529c0c: ldr      r3, [ip, #0x10]
00529c10: cmp      r2, r3
00529c14: bhs      #0x529d44
00529c18: cmp      lr, #0
00529c1c: beq      #0x529d60
00529c20: mov      lr, #0
00529c24: mov      r1, r5
00529c28: mov      r2, ip
00529c2c: mov      r3, r6
00529c30: mov      r0, r7
00529c34: stm      sp, {ip, lr}
00529c38: bl       #0x529754
00529c3c: b        #0x529b48
00529c40: ldr      r2, [r4, #0xc]
00529c44: ldr      ip, [r3]
00529c48: ldr      lr, [r2, #0x10]
00529c4c: cmp      lr, ip
00529c50: bhs      #0x529c70
00529c54: mov      ip, #0
00529c58: str      ip, [sp]
00529c5c: str      r4, [sp, #4]
00529c60: bl       #0x529754
00529c64: b        #0x529b48
00529c68: str      r4, [r7]
00529c6c: b        #0x529b48
00529c70: mov      r2, r3
00529c74: add      r0, sp, #0x10
00529c78: bl       #0x5298a8
00529c7c: ldr      r3, [sp, #0x10]
00529c80: str      r3, [r7]
00529c84: b        #0x529b48
00529c88: mov      r1, r5
00529c8c: mov      r2, ip
00529c90: mov      r3, r6
00529c94: mov      r0, r7
00529c98: str      lr, [sp]
00529c9c: str      ip, [sp, #4]
00529ca0: bl       #0x529754
00529ca4: b        #0x529b48
00529ca8: ldr      r3, [r4, #4]
00529cac: ldr      ip, [r3, #0xc]
00529cb0: cmp      r4, ip
00529cb4: movne    ip, r4
00529cb8: bne      #0x529cd0
00529cbc: mov      ip, r3
00529cc0: ldr      r3, [r3, #4]
00529cc4: ldr      sl, [r3, #0xc]
00529cc8: cmp      ip, sl
00529ccc: beq      #0x529cbc
00529cd0: ldr      sl, [ip, #0xc]
00529cd4: cmp      r3, sl
00529cd8: movne    ip, r3
00529cdc: b        #0x529ac8
00529ce0: ldr      r3, [r4, #4]
00529ce4: ldr      r1, [r3, #0xc]
00529ce8: cmp      r4, r1
00529cec: movne    ip, r4
00529cf0: bne      #0x529d08
00529cf4: mov      ip, r3
00529cf8: ldr      r3, [r3, #4]
00529cfc: ldr      r1, [r3, #0xc]
00529d00: cmp      r1, ip
00529d04: beq      #0x529cf4
00529d08: ldr      r1, [ip, #0xc]
00529d0c: cmp      r3, r1
00529d10: movne    ip, r3
00529d14: b        #0x529c04
00529d18: mov      r2, r3
00529d1c: add      r0, sp, #0x20
00529d20: bl       #0x5298a8
00529d24: ldr      r3, [sp, #0x20]
00529d28: str      r3, [r7]
00529d2c: b        #0x529b48
00529d30: mov      ip, #0
00529d34: mov      r2, r4
00529d38: stm      sp, {r4, ip}
00529d3c: bl       #0x529754
00529d40: b        #0x529b48
00529d44: mov      r1, r5
00529d48: mov      r2, r6
00529d4c: add      r0, sp, #0x18
00529d50: bl       #0x5298a8
00529d54: ldr      r3, [sp, #0x18]
00529d58: str      r3, [r7]
00529d5c: b        #0x529b48
00529d60: mov      r1, r5
00529d64: mov      r2, r4
00529d68: mov      r3, r6
00529d6c: mov      r0, r7
00529d70: str      lr, [sp]
00529d74: str      r4, [sp, #4]
00529d78: bl       #0x529754
00529d7c: b        #0x529b48
00529d80: mov      ip, #0
00529d84: mov      r1, r5
00529d88: mov      r2, r4
00529d8c: mov      r3, r6
00529d90: mov      r0, r7
00529d94: str      ip, [sp]
00529d98: str      r4, [sp, #4]
00529d9c: bl       #0x529754
00529da0: b        #0x529b48

# _ZSt15__push_heap_auxIPN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeENS6_6_ECompEiS7_EvT_SA_T0_PT1_PT2_
0052958c: push     {r4, lr}
00529590: rsb      r3, r0, r1
00529594: asr      r3, r3, #2
00529598: ldr      r4, [r1, #-0xc]
0052959c: add      ip, r3, r3, lsl #2
005295a0: sub      r2, r1, #0xc
005295a4: add      r1, ip, ip, lsl #4
005295a8: ldr      lr, [r2, #4]
005295ac: add      r1, r1, r1, lsl #8
005295b0: ldr      ip, [r2, #8]
005295b4: add      r1, r1, r1, lsl #16
005295b8: sub      sp, sp, #0x10
005295bc: add      r3, r3, r1, lsl #1
005295c0: sub      r1, r3, #1
005295c4: add      r2, sp, #4
005295c8: mov      r3, #0
005295cc: str      r4, [sp, #4]
005295d0: str      lr, [sp, #8]
005295d4: str      ip, [sp, #0xc]
005295d8: bl       #0x529398
005295dc: add      sp, sp, #0x10
005295e0: pop      {r4, pc}

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE13insert_uniqueERKSD_
005298a8: push     {r4, r5, r6, lr}
005298ac: ldr      ip, [r1, #4]
005298b0: sub      sp, sp, #0x10
005298b4: mov      r4, r0
005298b8: cmp      ip, #0
005298bc: mov      r3, r2
005298c0: moveq    ip, r1
005298c4: beq      #0x529920
005298c8: ldr      r6, [r2]
005298cc: b        #0x5298d4
005298d0: mov      ip, r2
005298d4: ldr      r0, [ip, #0x10]
005298d8: mov      r5, #1
005298dc: cmp      r0, r6
005298e0: ldrhi    r2, [ip, #8]
005298e4: ldrls    r2, [ip, #0xc]
005298e8: movls    r5, #0
005298ec: cmp      r2, #0
005298f0: bne      #0x5298d0
005298f4: cmp      r5, #0
005298f8: moveq    r5, ip
005298fc: bne      #0x529920
00529900: cmp      r6, r0
00529904: movls    r3, #0
00529908: strls    r5, [r4]
0052990c: strbls   r3, [r4, #4]
00529910: bhi      #0x529988
00529914: mov      r0, r4
00529918: add      sp, sp, #0x10
0052991c: pop      {r4, r5, r6, pc}
00529920: ldr      r2, [r1, #8]
00529924: cmp      ip, r2
00529928: beq      #0x529a08
0052992c: ldrb     r2, [ip]
00529930: cmp      r2, #0
00529934: bne      #0x529948
00529938: ldr      r2, [ip, #4]
0052993c: ldr      r2, [r2, #4]
00529940: cmp      ip, r2
00529944: beq      #0x5299f4
00529948: ldr      r0, [ip, #8]
0052994c: cmp      r0, #0
00529950: bne      #0x52995c
00529954: b        #0x5299b4
00529958: mov      r0, r2
0052995c: ldr      r2, [r0, #0xc]
00529960: cmp      r2, #0
00529964: bne      #0x529958
00529968: ldr      r6, [r3]
0052996c: mov      r5, r0
00529970: ldr      r0, [r0, #0x10]
00529974: cmp      r6, r0
00529978: movls    r3, #0
0052997c: strls    r5, [r4]
00529980: strbls   r3, [r4, #4]
00529984: bls      #0x529914
00529988: mov      r2, ip
0052998c: add      r0, sp, #8
00529990: mov      ip, #0
00529994: str      ip, [sp, #4]
00529998: str      ip, [sp]
0052999c: bl       #0x529754
005299a0: ldr      r3, [sp, #8]
005299a4: mov      r2, #1
005299a8: strb     r2, [r4, #4]
005299ac: str      r3, [r4]
005299b0: b        #0x529914
005299b4: ldr      r2, [ip, #4]
005299b8: ldr      r0, [r2, #8]
005299bc: cmp      ip, r0
005299c0: movne    r5, r2
005299c4: ldrne    r6, [r3]
005299c8: ldrne    r0, [r2, #0x10]
005299cc: beq      #0x5299d8
005299d0: b        #0x529900
005299d4: mov      r2, r5
005299d8: ldr      r5, [r2, #4]
005299dc: ldr      r0, [r5, #8]
005299e0: cmp      r0, r2
005299e4: beq      #0x5299d4
005299e8: ldr      r6, [r3]
005299ec: ldr      r0, [r5, #0x10]
005299f0: b        #0x529900
005299f4: ldr      r2, [ip, #0xc]
005299f8: ldr      r6, [r3]
005299fc: mov      r5, r2
00529a00: ldr      r0, [r2, #0x10]
00529a04: b        #0x529900
00529a08: mov      r2, ip
00529a0c: mov      lr, #0
00529a10: add      r0, sp, #0xc
00529a14: stm      sp, {ip, lr}
00529a18: bl       #0x529754
00529a1c: ldr      r3, [sp, #0xc]
00529a20: mov      r2, #1
00529a24: strb     r2, [r4, #4]
00529a28: str      r3, [r4]
00529a2c: b        #0x529914

# _ZNSaIN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEE11_M_allocateEjRj
0052a0a4: push     {r4, lr}
0052a0a8: movw     r3, #0x5555
0052a0ac: orr      r3, r3, r3, lsl #14
0052a0b0: cmp      r1, r3
0052a0b4: sub      sp, sp, #8
0052a0b8: mov      r4, r2
0052a0bc: bhi      #0x52a114
0052a0c0: cmp      r1, #0
0052a0c4: moveq    r0, r1
0052a0c8: bne      #0x52a0d4
0052a0cc: add      sp, sp, #8
0052a0d0: pop      {r4, pc}
0052a0d4: mov      r0, #0xc
0052a0d8: mul      r0, r0, r1
0052a0dc: cmp      r0, #0x80
0052a0e0: str      r0, [sp, #4]
0052a0e4: bhi      #0x52a10c
0052a0e8: add      r0, sp, #4
0052a0ec: bl       #0x708ec0
0052a0f0: ldr      r2, [sp, #4]
0052a0f4: movw     r3, #0xaaab
0052a0f8: movt     r3, #0xaaaa
0052a0fc: umull    r1, r3, r3, r2
0052a100: lsr      r3, r3, #3
0052a104: str      r3, [r4]
0052a108: b        #0x52a0cc
0052a10c: bl       #0x310454
0052a110: b        #0x52a0f0
0052a114: ldr      r0, [pc, #0xc]
0052a118: add      r0, pc, r0
0052a11c: bl       #0x30e0c4
0052a120: mov      r0, #1
0052a124: bl       #0x30de48
0052a128: eorseq   r4, sb, r8, asr r3

# _ZNSt14priority_queueIN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeESt6vectorIS7_SaIS7_EENS6_6_ECompEE4pushERKS7_
0052a890: push     {r4, r5, r6, r7, lr}
0052a894: ldmib    r0, {r3, r6}
0052a898: sub      sp, sp, #0x14
0052a89c: mov      r4, r0
0052a8a0: cmp      r3, r6
0052a8a4: mov      r5, r1
0052a8a8: beq      #0x52a8f8
0052a8ac: ldr      r2, [r1]
0052a8b0: str      r2, [r3]
0052a8b4: ldr      r2, [r1, #4]
0052a8b8: str      r2, [r3, #4]
0052a8bc: ldr      r2, [r1, #8]
0052a8c0: str      r2, [r3, #8]
0052a8c4: ldr      r6, [r0, #4]
0052a8c8: ldr      r7, [r0]
0052a8cc: add      r6, r6, #0xc
0052a8d0: str      r6, [r0, #4]
0052a8d4: mov      ip, #0
0052a8d8: mov      r0, r7
0052a8dc: mov      r1, r6
0052a8e0: mov      r3, ip
0052a8e4: mov      r2, #0
0052a8e8: str      ip, [sp]
0052a8ec: bl       #0x52958c
0052a8f0: add      sp, sp, #0x14
0052a8f4: pop      {r4, r5, r6, r7, pc}
0052a8f8: ldr      r2, [r0]
0052a8fc: movw     r3, #0x5555
0052a900: orr      r3, r3, r3, lsl #14
0052a904: rsb      r2, r2, r6
0052a908: asr      r2, r2, #2
0052a90c: add      r1, r2, r2, lsl #2
0052a910: add      r1, r1, r1, lsl #4
0052a914: add      r1, r1, r1, lsl #8
0052a918: add      r1, r1, r1, lsl #16
0052a91c: add      r2, r2, r1, lsl #1
0052a920: cmp      r2, #1
0052a924: addhs    r1, r2, r2
0052a928: addlo    r1, r2, #1
0052a92c: cmp      r1, r3
0052a930: bhi      #0x52aa70
0052a934: cmp      r2, r1
0052a938: bhi      #0x52aa70
0052a93c: add      r2, sp, #0x10
0052a940: str      r1, [r2, #-4]!
0052a944: add      r0, r4, #8
0052a948: bl       #0x52a0a4
0052a94c: ldr      r3, [r4]
0052a950: mov      r7, r0
0052a954: rsb      r6, r3, r6
0052a958: asr      r6, r6, #2
0052a95c: add      r2, r6, r6, lsl #2
0052a960: add      r2, r2, r2, lsl #4
0052a964: add      r2, r2, r2, lsl #8
0052a968: add      r2, r2, r2, lsl #16
0052a96c: add      r6, r6, r2, lsl #1
0052a970: cmp      r6, #0
0052a974: movle    r3, r0
0052a978: ble      #0x52a9b4
0052a97c: mov      r1, r6
0052a980: mov      r2, r0
0052a984: ldr      r0, [r3]
0052a988: subs     r1, r1, #1
0052a98c: str      r0, [r2]
0052a990: ldr      r0, [r3, #4]
0052a994: str      r0, [r2, #4]
0052a998: ldr      r0, [r3, #8]
0052a99c: add      r3, r3, #0xc
0052a9a0: str      r0, [r2, #8]
0052a9a4: add      r2, r2, #0xc
0052a9a8: bne      #0x52a984
0052a9ac: mov      r3, #0xc
0052a9b0: mla      r3, r3, r6, r7
0052a9b4: ldr      r2, [r5]
0052a9b8: add      r6, r3, #0xc
0052a9bc: str      r2, [r3]
0052a9c0: ldr      r2, [r5, #4]
0052a9c4: str      r2, [r3, #4]
0052a9c8: ldr      r2, [r5, #8]
0052a9cc: str      r2, [r3, #8]
0052a9d0: ldm      r4, {r0, r3}
0052a9d4: cmp      r3, r0
0052a9d8: beq      #0x52aa18
0052a9dc: sub      r2, r3, #0xc
0052a9e0: rsb      r2, r0, r2
0052a9e4: lsr      r2, r2, #2
0052a9e8: add      r1, r2, r2, lsl #2
0052a9ec: add      r1, r1, r1, lsl #5
0052a9f0: add      r1, r2, r1, lsl #1
0052a9f4: add      r1, r1, r1, lsl #5
0052a9f8: lsl      ip, r1, #0xf
0052a9fc: rsb      r1, r1, ip
0052aa00: add      r2, r2, r1, lsl #1
0052aa04: bic      r2, r2, #0xc0000000
0052aa08: mvn      r1, #0xb
0052aa0c: mul      r2, r1, r2
0052aa10: add      r2, r2, r1
0052aa14: add      r3, r3, r2
0052aa18: cmp      r3, #0
0052aa1c: ldr      r2, [r4, #8]
0052aa20: beq      #0x52aa54
0052aa24: rsb      r3, r3, r2
0052aa28: asr      r3, r3, #2
0052aa2c: add      r1, r3, r3, lsl #2
0052aa30: add      r1, r1, r1, lsl #4
0052aa34: add      r1, r1, r1, lsl #8
0052aa38: add      r1, r1, r1, lsl #16
0052aa3c: add      r3, r3, r1, lsl #1
0052aa40: mov      r1, #0xc
0052aa44: mul      r1, r1, r3
0052aa48: cmp      r1, #0x80
0052aa4c: bhi      #0x52aa7c
0052aa50: bl       #0x708f00
0052aa54: ldr      r3, [sp, #0xc]
0052aa58: mov      r2, #0xc
0052aa5c: str      r7, [r4]
0052aa60: mla      r3, r2, r3, r7
0052aa64: str      r6, [r4, #4]
0052aa68: str      r3, [r4, #8]
0052aa6c: b        #0x52a8d4
0052aa70: movw     r1, #0x5555
0052aa74: orr      r1, r1, r1, lsl #14
0052aa78: b        #0x52a93c
0052aa7c: bl       #0x310440
0052aa80: b        #0x52aa54

# _ZNK3sfc4math5graph4EdgeI12PFGInnerNodefE9getWeightEv
0051c104: ldr      r0, [r0, #0xc]
0051c108: bx       lr

# _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE8findNodeEjRKNS1_5ITestI12PFGInnerEdge12PFGInnerNodeEEjPSt4listIPKS7_SaISE_EE
0052ad4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0052ad50: mov      r5, r0
0052ad54: ldr      r0, [r0, #4]
0052ad58: mov      r6, r2
0052ad5c: sub      sp, sp, #0xd4
0052ad60: ldr      r2, [r0, #8]
0052ad64: ldr      sl, [sp, #0xf8]
0052ad68: str      r3, [sp, #0xc]
0052ad6c: cmp      r2, #0
0052ad70: add      r0, r0, #4
0052ad74: beq      #0x52b510
0052ad78: mov      ip, r0
0052ad7c: b        #0x52ad84
0052ad80: mov      r2, r3
0052ad84: ldr      r3, [r2, #0x10]
0052ad88: cmp      r1, r3
0052ad8c: ldrhi    r3, [r2, #0xc]
0052ad90: ldrls    r3, [r2, #8]
0052ad94: movhi    r2, ip
0052ad98: mov      ip, r2
0052ad9c: cmp      r3, #0
0052ada0: bne      #0x52ad80
0052ada4: cmp      r0, r2
0052ada8: beq      #0x52b53c
0052adac: ldr      r3, [r2, #0x10]
0052adb0: cmp      r1, r3
0052adb4: blo      #0x52b510
0052adb8: cmp      r0, r2
0052adbc: beq      #0x52b53c
0052adc0: ldr      r2, [r2, #0x14]
0052adc4: mov      r8, #0
0052adc8: str      r2, [sp, #0x10]
0052adcc: add      r2, r5, #8
0052add0: str      r2, [sp, #0x1c]
0052add4: strb     r8, [r5, #0x14]
0052add8: ldr      r0, [sp, #0x1c]
0052addc: bl       #0x529f4c
0052ade0: ldr      r3, [sp, #0x10]
0052ade4: str      r8, [r5, #0x18]
0052ade8: str      r8, [r5, #0x1c]
0052adec: cmp      r3, r8
0052adf0: str      r8, [r5, #0x20]
0052adf4: str      r8, [r5, #0x24]
0052adf8: beq      #0x52b3f0
0052adfc: ldr      ip, [sp, #0x1c]
0052ae00: cmp      sl, r8
0052ae04: add      r7, sp, #0xd0
0052ae08: moveq    sl, ip
0052ae0c: strb     r8, [r7, #-0x9c]!
0052ae10: str      sl, [r5, #0x10]
0052ae14: ldr      r2, [sp, #0x10]
0052ae18: add      r4, sp, #0xb0
0052ae1c: str      r8, [sp, #0x44]
0052ae20: str      r4, [sp, #0xb0]
0052ae24: str      r4, [sp, #0xb4]
0052ae28: str      r8, [sp, #0x7c]
0052ae2c: str      r8, [sp, #0x80]
0052ae30: str      r8, [sp, #0x84]
0052ae34: str      r8, [sp, #0x38]
0052ae38: str      r7, [sp, #0x3c]
0052ae3c: str      r7, [sp, #0x40]
0052ae40: ldr      r3, [r2]
0052ae44: mov      r0, r2
0052ae48: mov      lr, pc
0052ae4c: ldr      pc, [r3]
0052ae50: ldr      ip, [sp, #0x38]
0052ae54: mov      lr, r0
0052ae58: cmp      ip, r8
0052ae5c: moveq    ip, r7
0052ae60: beq      #0x52aea4
0052ae64: mov      r2, r7
0052ae68: b        #0x52ae70
0052ae6c: mov      ip, r3
0052ae70: ldr      r3, [ip, #0x10]
0052ae74: cmp      lr, r3
0052ae78: ldrhi    r3, [ip, #0xc]
0052ae7c: ldrls    r3, [ip, #8]
0052ae80: movhi    ip, r2
0052ae84: mov      r2, ip
0052ae88: cmp      r3, #0
0052ae8c: bne      #0x52ae6c
0052ae90: cmp      ip, r7
0052ae94: beq      #0x52aea4
0052ae98: ldr      r3, [ip, #0x10]
0052ae9c: cmp      lr, r3
0052aea0: bhs      #0x52aed8
0052aea4: mov      r8, #0
0052aea8: str      ip, [sp, #0xcc]
0052aeac: add      r0, sp, #0xc8
0052aeb0: mov      ip, #0
0052aeb4: mov      r1, r7
0052aeb8: add      r2, sp, #0xcc
0052aebc: add      r3, sp, #0x6c
0052aec0: str      ip, [sp, #0x70]
0052aec4: str      lr, [sp, #0x6c]
0052aec8: str      r8, [sp, #0x78]
0052aecc: str      r8, [sp, #0x74]
0052aed0: bl       #0x529a30
0052aed4: ldr      ip, [sp, #0xc8]
0052aed8: mov      sl, #0
0052aedc: mov      r3, #0
0052aee0: str      r3, [ip, #0x14]
0052aee4: str      sl, [ip, #0x1c]
0052aee8: str      sl, [ip, #0x18]
0052aeec: ldr      sb, [sp, #0x10]
0052aef0: add      r3, sp, #0x7c
0052aef4: add      ip, sp, #0x98
0052aef8: add      r2, sp, #0x8c
0052aefc: str      r3, [sp, #0x14]
0052af00: add      r8, sp, #0xa4
0052af04: str      ip, [sp, #0x20]
0052af08: str      r2, [sp, #0x18]
0052af0c: ldr      r3, [r6]
0052af10: mov      r0, r6
0052af14: mov      r1, sb
0052af18: mov      lr, pc
0052af1c: ldr      pc, [r3]
0052af20: cmp      r0, #0
0052af24: beq      #0x52b124
0052af28: mov      r0, r6
0052af2c: ldr      r3, [r6]
0052af30: mov      r1, sb
0052af34: mov      lr, pc
0052af38: ldr      pc, [r3]
0052af3c: cmp      r0, #0
0052af40: strb     r0, [r5, #0x14]
0052af44: beq      #0x52b474
0052af48: add      r3, sp, #0xc0
0052af4c: add      ip, sp, #0xc4
0052af50: add      r2, sp, #0x5c
0052af54: str      r3, [sp, #0xc]
0052af58: str      ip, [sp, #0x18]
0052af5c: str      r2, [sp, #0x20]
0052af60: add      r3, sp, #0xb8
0052af64: add      ip, sp, #0xbc
0052af68: add      r2, sp, #0x4c
0052af6c: mov      sl, #0
0052af70: mov      fp, #0
0052af74: str      r3, [sp, #0x24]
0052af78: str      ip, [sp, #0x28]
0052af7c: str      r2, [sp, #0x2c]
0052af80: mov      r6, r5
0052af84: mov      r8, r4
0052af88: ldr      r3, [sb]
0052af8c: mov      r0, sb
0052af90: mov      lr, pc
0052af94: ldr      pc, [r3]
0052af98: ldr      ip, [sp, #0x10]
0052af9c: mov      r4, r0
0052afa0: ldr      r3, [ip]
0052afa4: mov      r0, ip
0052afa8: mov      lr, pc
0052afac: ldr      pc, [r3]
0052afb0: cmp      r4, r0
0052afb4: beq      #0x52b3c4
0052afb8: ldr      r3, [sb]
0052afbc: mov      r0, sb
0052afc0: ldr      r5, [r6, #0x10]
0052afc4: mov      lr, pc
0052afc8: ldr      pc, [r3]
0052afcc: ldr      r4, [sp, #0x38]
0052afd0: mov      ip, r0
0052afd4: cmp      r4, #0
0052afd8: moveq    r4, r7
0052afdc: beq      #0x52b020
0052afe0: mov      r2, r7
0052afe4: b        #0x52afec
0052afe8: mov      r4, r3
0052afec: ldr      r3, [r4, #0x10]
0052aff0: cmp      ip, r3
0052aff4: ldrhi    r3, [r4, #0xc]
0052aff8: ldrls    r3, [r4, #8]
0052affc: movhi    r4, r2
0052b000: mov      r2, r4
0052b004: cmp      r3, #0
0052b008: bne      #0x52afe8
0052b00c: cmp      r4, r7
0052b010: beq      #0x52b020
0052b014: ldr      r3, [r4, #0x10]
0052b018: cmp      ip, r3
0052b01c: bhs      #0x52b04c
0052b020: ldr      r0, [sp, #0xc]
0052b024: mov      r1, r7
0052b028: ldr      r2, [sp, #0x18]
0052b02c: ldr      r3, [sp, #0x20]
0052b030: str      r4, [sp, #0xc4]
0052b034: str      ip, [sp, #0x5c]
0052b038: str      fp, [sp, #0x60]
0052b03c: str      sl, [sp, #0x64]
0052b040: str      sl, [sp, #0x68]
0052b044: bl       #0x529a30
0052b048: ldr      r4, [sp, #0xc0]
0052b04c: mov      r0, r5
0052b050: ldr      r5, [r5]
0052b054: bl       #0x52aa84
0052b058: ldr      r2, [r4, #0x14]
0052b05c: mov      r3, r0
0052b060: mov      r0, sb
0052b064: str      r2, [r3, #8]
0052b068: ldr      r2, [r5, #4]
0052b06c: str      r5, [r3]
0052b070: str      r2, [r3, #4]
0052b074: str      r3, [r2]
0052b078: str      r3, [r5, #4]
0052b07c: ldr      r3, [sb]
0052b080: mov      lr, pc
0052b084: ldr      pc, [r3]
0052b088: ldr      ip, [sp, #0x38]
0052b08c: mov      lr, r0
0052b090: cmp      ip, #0
0052b094: moveq    ip, r7
0052b098: beq      #0x52b0dc
0052b09c: mov      r2, r7
0052b0a0: b        #0x52b0a8
0052b0a4: mov      ip, r3
0052b0a8: ldr      r3, [ip, #0x10]
0052b0ac: cmp      lr, r3
0052b0b0: ldrhi    r3, [ip, #0xc]
0052b0b4: ldrls    r3, [ip, #8]
0052b0b8: movhi    ip, r2
0052b0bc: mov      r2, ip
0052b0c0: cmp      r3, #0
0052b0c4: bne      #0x52b0a4
0052b0c8: cmp      ip, r7
0052b0cc: beq      #0x52b0dc
0052b0d0: ldr      r3, [ip, #0x10]
0052b0d4: cmp      lr, r3
0052b0d8: bhs      #0x52b108
0052b0dc: ldr      r0, [sp, #0x24]
0052b0e0: mov      r1, r7
0052b0e4: ldr      r2, [sp, #0x28]
0052b0e8: ldr      r3, [sp, #0x2c]
0052b0ec: str      ip, [sp, #0xbc]
0052b0f0: str      lr, [sp, #0x4c]
0052b0f4: str      fp, [sp, #0x50]
0052b0f8: str      sl, [sp, #0x54]
0052b0fc: str      sl, [sp, #0x58]
0052b100: bl       #0x529a30
0052b104: ldr      ip, [sp, #0xb8]
0052b108: ldr      r3, [ip, #0x14]
0052b10c: mov      r0, r3
0052b110: ldr      r3, [r3]
0052b114: mov      lr, pc
0052b118: ldr      pc, [r3, #4]
0052b11c: mov      sb, r0
0052b120: b        #0x52af88
0052b124: ldr      r2, [sp, #0xc]
0052b128: cmp      r2, #0
0052b12c: beq      #0x52af28
0052b130: ldr      r3, [r5, #0x18]
0052b134: mov      r0, sb
0052b138: ldr      fp, [r5, #4]
0052b13c: add      r3, r3, #1
0052b140: str      r3, [r5, #0x18]
0052b144: ldr      r3, [sb]
0052b148: mov      lr, pc
0052b14c: ldr      pc, [r3]
0052b150: mov      r2, r4
0052b154: mov      r1, r0
0052b158: mov      r0, fp
0052b15c: bl       #0x52ac30
0052b160: ldr      r2, [sp, #0xb0]
0052b164: cmp      r2, r4
0052b168: mov      r3, r2
0052b16c: beq      #0x52b268
0052b170: ldr      r3, [r3]
0052b174: cmp      r3, r4
0052b178: bne      #0x52b170
0052b17c: ldr      r3, [r5, #0x1c]
0052b180: add      r3, r3, #1
0052b184: str      r3, [r5, #0x1c]
0052b188: ldr      r3, [r2, #8]
0052b18c: ldr      r2, [r6]
0052b190: mov      r0, r3
0052b194: ldr      r3, [r3]
0052b198: ldr      fp, [r2]
0052b19c: mov      lr, pc
0052b1a0: ldr      pc, [r3, #0xc]
0052b1a4: mov      r1, r0
0052b1a8: mov      r0, r6
0052b1ac: blx      fp
0052b1b0: cmp      r0, #0
0052b1b4: beq      #0x52b31c
0052b1b8: ldr      r3, [sp, #0xb0]
0052b1bc: ldr      r2, [r6]
0052b1c0: ldr      r3, [r3, #8]
0052b1c4: ldr      fp, [r2]
0052b1c8: mov      r0, r3
0052b1cc: ldr      r3, [r3]
0052b1d0: mov      lr, pc
0052b1d4: ldr      pc, [r3, #0xc]
0052b1d8: mov      r1, r0
0052b1dc: mov      r0, r6
0052b1e0: blx      fp
0052b1e4: ldr      r2, [r5, #0x20]
0052b1e8: ldr      r3, [sp, #0xb0]
0052b1ec: add      r2, r2, #1
0052b1f0: str      r2, [r5, #0x20]
0052b1f4: ldr      fp, [r3, #8]
0052b1f8: ldr      r3, [fp]
0052b1fc: mov      r0, fp
0052b200: mov      lr, pc
0052b204: ldr      pc, [r3, #0x10]
0052b208: mov      r1, sl
0052b20c: bl       #0x30eba4
0052b210: mov      r1, #0
0052b214: str      fp, [sp, #0xa4]
0052b218: str      r0, [sp, #0xa8]
0052b21c: bl       #0x30eba4
0052b220: mov      r1, r7
0052b224: str      r0, [sp, #0xac]
0052b228: mov      r2, r8
0052b22c: mov      r0, r5
0052b230: bl       #0x529da4
0052b234: cmp      r0, #0
0052b238: bne      #0x52b374
0052b23c: ldr      r0, [sp, #0xb0]
0052b240: mov      r1, #0xc
0052b244: ldr      r3, [r0]
0052b248: ldr      r2, [r0, #4]
0052b24c: str      r3, [r2]
0052b250: str      r2, [r3, #4]
0052b254: bl       #0x708f00
0052b258: ldr      r2, [sp, #0xb0]
0052b25c: cmp      r2, r4
0052b260: mov      r3, r2
0052b264: bne      #0x52b170
0052b268: ldr      r2, [sp, #0xc]
0052b26c: subs     r2, r2, #1
0052b270: str      r2, [sp, #0xc]
0052b274: beq      #0x52af28
0052b278: ldr      r2, [sp, #0x7c]
0052b27c: ldr      r3, [sp, #0x80]
0052b280: rsb      r3, r2, r3
0052b284: asr      r3, r3, #2
0052b288: add      r1, r3, r3, lsl #2
0052b28c: add      r1, r1, r1, lsl #4
0052b290: add      r1, r1, r1, lsl #8
0052b294: add      r1, r1, r1, lsl #16
0052b298: add      r3, r3, r1, lsl #1
0052b29c: cmp      r3, #0
0052b2a0: beq      #0x52af28
0052b2a4: ldr      r3, [r2]
0052b2a8: mov      r0, r3
0052b2ac: ldr      r3, [r3]
0052b2b0: mov      lr, pc
0052b2b4: ldr      pc, [r3, #0xc]
0052b2b8: ldr      r3, [sp, #0x80]
0052b2bc: mov      sb, r0
0052b2c0: ldr      r0, [sp, #0x7c]
0052b2c4: ldr      r2, [r3, #-0xc]
0052b2c8: sub      r3, r3, #0xc
0052b2cc: ldr      sl, [r0, #4]
0052b2d0: str      r2, [sp, #0x8c]
0052b2d4: ldr      r2, [r3, #4]
0052b2d8: mov      r1, r3
0052b2dc: str      r2, [sp, #0x90]
0052b2e0: ldr      ip, [r3, #8]
0052b2e4: mov      r2, r3
0052b2e8: ldr      r3, [sp, #0x18]
0052b2ec: str      ip, [sp, #0x94]
0052b2f0: mov      ip, #0
0052b2f4: strb     ip, [sp]
0052b2f8: mov      ip, #0
0052b2fc: str      ip, [sp, #4]
0052b300: bl       #0x52943c
0052b304: ldr      r3, [sp, #0x80]
0052b308: cmp      sb, #0
0052b30c: sub      r3, r3, #0xc
0052b310: str      r3, [sp, #0x80]
0052b314: bne      #0x52af0c
0052b318: b        #0x52af28
0052b31c: ldr      r2, [sp, #0xb0]
0052b320: ldr      r3, [r6]
0052b324: mov      r0, r6
0052b328: ldr      r1, [r2, #8]
0052b32c: mov      lr, pc
0052b330: ldr      pc, [r3, #4]
0052b334: cmp      r0, #0
0052b338: beq      #0x52b23c
0052b33c: ldr      r3, [sp, #0xb0]
0052b340: ldr      r2, [r6]
0052b344: ldr      r3, [r3, #8]
0052b348: ldr      fp, [r2, #8]
0052b34c: mov      r0, r3
0052b350: ldr      r3, [r3]
0052b354: mov      lr, pc
0052b358: ldr      pc, [r3, #0xc]
0052b35c: mov      r1, r0
0052b360: mov      r0, r6
0052b364: blx      fp
0052b368: cmp      r0, #0
0052b36c: beq      #0x52b23c
0052b370: b        #0x52b1b8
0052b374: ldr      r3, [sp, #0xb0]
0052b378: ldr      r2, [r6]
0052b37c: ldr      r3, [r3, #8]
0052b380: ldr      fp, [r2]
0052b384: mov      r0, r3
0052b388: ldr      r3, [r3]
0052b38c: mov      lr, pc
0052b390: ldr      pc, [r3, #0xc]
0052b394: mov      r1, r0
0052b398: mov      r0, r6
0052b39c: blx      fp
0052b3a0: cmp      r0, #0
0052b3a4: bne      #0x52b3fc
0052b3a8: ldr      r3, [r5, #0x24]
0052b3ac: ldr      r0, [sp, #0x14]
0052b3b0: mov      r1, r8
0052b3b4: add      r3, r3, #1
0052b3b8: str      r3, [r5, #0x24]
0052b3bc: bl       #0x52a890
0052b3c0: b        #0x52b23c
0052b3c4: mov      r5, r6
0052b3c8: mov      r4, r8
0052b3cc: ldr      r3, [sp, #0x44]
0052b3d0: cmp      r3, #0
0052b3d4: bne      #0x52b518
0052b3d8: ldr      r0, [sp, #0x14]
0052b3dc: bl       #0x52a640
0052b3e0: mov      r0, r4
0052b3e4: bl       #0x529f4c
0052b3e8: ldr      r2, [sp, #0x1c]
0052b3ec: str      r2, [r5, #0x10]
0052b3f0: ldrb     r0, [r5, #0x14]
0052b3f4: add      sp, sp, #0xd4
0052b3f8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0052b3fc: ldr      r0, [sp, #0x7c]
0052b400: ldr      r3, [sp, #0x80]
0052b404: cmp      r0, r3
0052b408: beq      #0x52b464
0052b40c: ldr      sl, [sp, #0x20]
0052b410: ldr      r2, [r3, #-0xc]
0052b414: sub      r3, r3, #0xc
0052b418: mov      r1, r3
0052b41c: str      r2, [sp, #0x98]
0052b420: ldr      r2, [r3, #4]
0052b424: str      r2, [sp, #0x9c]
0052b428: ldr      ip, [r3, #8]
0052b42c: mov      r2, r3
0052b430: mov      r3, sl
0052b434: str      ip, [sp, #0xa0]
0052b438: mov      ip, #0
0052b43c: strb     ip, [sp]
0052b440: mov      ip, #0
0052b444: str      ip, [sp, #4]
0052b448: bl       #0x52943c
0052b44c: ldr      r3, [sp, #0x80]
0052b450: ldr      r0, [sp, #0x7c]
0052b454: sub      r3, r3, #0xc
0052b458: cmp      r3, r0
0052b45c: str      r3, [sp, #0x80]
0052b460: bne      #0x52b410
0052b464: ldr      r0, [sp, #0x14]
0052b468: mov      r1, r8
0052b46c: bl       #0x52a890
0052b470: b        #0x52b268
0052b474: ldr      r6, [sp, #0x3c]
0052b478: cmp      r6, r7
0052b47c: beq      #0x52b3cc
0052b480: ldr      r3, [r6, #0x14]
0052b484: cmp      r3, #0
0052b488: beq      #0x52b4b8
0052b48c: ldr      r3, [r5, #0x10]
0052b490: mov      r0, r3
0052b494: ldr      r8, [r3]
0052b498: bl       #0x52aa84
0052b49c: ldr      r3, [r6, #0x14]
0052b4a0: str      r3, [r0, #8]
0052b4a4: ldr      r3, [r8, #4]
0052b4a8: str      r8, [r0]
0052b4ac: str      r3, [r0, #4]
0052b4b0: str      r0, [r3]
0052b4b4: str      r0, [r8, #4]
0052b4b8: ldr      r2, [r6, #0xc]
0052b4bc: cmp      r2, #0
0052b4c0: beq      #0x52b4dc
0052b4c4: mov      r6, r2
0052b4c8: ldr      r3, [r6, #8]
0052b4cc: cmp      r3, #0
0052b4d0: beq      #0x52b478
0052b4d4: mov      r6, r3
0052b4d8: b        #0x52b4c8
0052b4dc: ldr      r3, [r6, #4]
0052b4e0: ldr      r1, [r3, #0xc]
0052b4e4: cmp      r1, r6
0052b4e8: bne      #0x52b504
0052b4ec: mov      r6, r3
0052b4f0: ldr      r3, [r3, #4]
0052b4f4: ldr      r2, [r3, #0xc]
0052b4f8: cmp      r2, r6
0052b4fc: beq      #0x52b4ec
0052b500: ldr      r2, [r6, #0xc]
0052b504: cmp      r3, r2
0052b508: movne    r6, r3
0052b50c: b        #0x52b478
0052b510: mov      r2, r0
0052b514: b        #0x52adb8
0052b518: mov      r0, r7
0052b51c: ldr      r1, [sp, #0x38]
0052b520: bl       #0x52a06c
0052b524: mov      r3, #0
0052b528: str      r7, [sp, #0x40]
0052b52c: str      r3, [sp, #0x44]
0052b530: str      r7, [sp, #0x3c]
0052b534: str      r3, [sp, #0x38]
0052b538: b        #0x52b3d8
0052b53c: mov      r4, #0
0052b540: strb     r4, [r5, #0x14]
0052b544: add      r0, r5, #8
0052b548: bl       #0x529f4c
0052b54c: str      r4, [r5, #0x24]
0052b550: str      r4, [r5, #0x18]
0052b554: str      r4, [r5, #0x1c]
0052b558: str      r4, [r5, #0x20]
0052b55c: b        #0x52b3f0

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE7_InEdgeEENS_10_Select1stISD_EENS_11_MapTraitsTISD_EESaISD_EE9_M_insertEPNS_18_Rb_tree_node_baseERKSD_SL_SL_
00529754: cmp      r1, r2
00529758: push     {r4, r5, r6, r7, r8, lr}
0052975c: mov      r4, r1
00529760: mov      r7, r2
00529764: mov      r5, r0
00529768: mov      r8, r3
0052976c: beq      #0x52984c
00529770: ldr      r3, [sp, #0x1c]
00529774: cmp      r3, #0
00529778: beq      #0x5297ec
0052977c: mov      r0, r4
00529780: bl       #0x529734
00529784: ldr      r2, [r8]
00529788: mov      r3, #0
0052978c: mov      r6, r0
00529790: str      r2, [r0, #0x10]
00529794: ldr      r2, [r8, #4]
00529798: str      r2, [r0, #0x14]
0052979c: ldr      r2, [r8, #8]
005297a0: str      r2, [r0, #0x18]
005297a4: ldr      r2, [r8, #0xc]
005297a8: str      r3, [r0, #0xc]
005297ac: str      r3, [r0, #8]
005297b0: str      r2, [r0, #0x1c]
005297b4: str      r0, [r7, #0xc]
005297b8: ldr      r3, [r4, #0xc]
005297bc: cmp      r7, r3
005297c0: beq      #0x529844
005297c4: mov      r0, r6
005297c8: str      r7, [r6, #4]
005297cc: add      r1, r4, #4
005297d0: bl       #0x313760
005297d4: ldr      r3, [r4, #0x10]
005297d8: mov      r0, r5
005297dc: add      r3, r3, #1
005297e0: str      r3, [r4, #0x10]
005297e4: str      r6, [r5]
005297e8: pop      {r4, r5, r6, r7, r8, pc}
005297ec: ldr      r3, [sp, #0x18]
005297f0: cmp      r3, #0
005297f4: beq      #0x529894
005297f8: mov      r0, r4
005297fc: bl       #0x529734
00529800: ldr      r2, [r8]
00529804: mov      r3, #0
00529808: mov      r6, r0
0052980c: str      r2, [r0, #0x10]
00529810: ldr      r2, [r8, #4]
00529814: str      r2, [r0, #0x14]
00529818: ldr      r2, [r8, #8]
0052981c: str      r2, [r0, #0x18]
00529820: ldr      r2, [r8, #0xc]
00529824: str      r3, [r0, #0xc]
00529828: str      r3, [r0, #8]
0052982c: str      r2, [r0, #0x1c]
00529830: str      r0, [r7, #8]
00529834: ldr      r3, [r4, #8]
00529838: cmp      r7, r3
0052983c: streq    r0, [r4, #8]
00529840: b        #0x5297c4
00529844: str      r6, [r4, #0xc]
00529848: b        #0x5297c4
0052984c: mov      r0, r1
00529850: bl       #0x529734
00529854: ldr      r2, [r8]
00529858: mov      r3, #0
0052985c: mov      r6, r0
00529860: str      r2, [r0, #0x10]
00529864: ldr      r2, [r8, #4]
00529868: str      r2, [r0, #0x14]
0052986c: ldr      r2, [r8, #8]
00529870: str      r2, [r0, #0x18]
00529874: ldr      r2, [r8, #0xc]
00529878: str      r3, [r0, #0xc]
0052987c: str      r3, [r0, #8]
00529880: str      r2, [r0, #0x1c]
00529884: str      r0, [r4, #8]
00529888: str      r0, [r4, #4]
0052988c: str      r0, [r4, #0xc]
00529890: b        #0x5297c4
00529894: ldr      r2, [r8]
00529898: ldr      r3, [r7, #0x10]
0052989c: cmp      r2, r3
005298a0: bhs      #0x52977c
005298a4: b        #0x5297f8

# _ZN3sfc4math5graph9AlgoAStarI13PFGInnerGraph21DiabloIPhoneHeuristicE9_markNodeERSt3mapIjNS5_7_InEdgeESt4lessIjESaISt4pairIKjS7_EEERS7_
00529da4: push     {r4, r5, r6, lr}
00529da8: ldr      r3, [r2]
00529dac: sub      sp, sp, #0x18
00529db0: mov      r6, r1
00529db4: mov      r0, r3
00529db8: ldr      r3, [r3]
00529dbc: mov      r5, r2
00529dc0: mov      lr, pc
00529dc4: ldr      pc, [r3, #0xc]
00529dc8: ldr      r3, [r0]
00529dcc: mov      r4, r0
00529dd0: mov      lr, pc
00529dd4: ldr      pc, [r3]
00529dd8: ldr      r3, [r6, #4]
00529ddc: cmp      r3, #0
00529de0: beq      #0x529f04
00529de4: mov      r1, r6
00529de8: b        #0x529df0
00529dec: mov      r3, r2
00529df0: ldr      r2, [r3, #0x10]
00529df4: cmp      r0, r2
00529df8: ldrhi    r2, [r3, #0xc]
00529dfc: ldrls    r2, [r3, #8]
00529e00: movhi    r3, r1
00529e04: mov      r1, r3
00529e08: cmp      r2, #0
00529e0c: bne      #0x529dec
00529e10: cmp      r6, r3
00529e14: beq      #0x529e44
00529e18: ldr      r2, [r3, #0x10]
00529e1c: cmp      r0, r2
00529e20: blo      #0x529f04
00529e24: cmp      r6, r3
00529e28: beq      #0x529e44
00529e2c: ldr      r0, [r3, #0x18]
00529e30: ldr      r1, [r5, #4]
00529e34: bl       #0x30e9ac
00529e38: cmp      r0, #0
00529e3c: movne    r0, #0
00529e40: bne      #0x529efc
00529e44: mov      r0, r4
00529e48: ldr      r3, [r4]
00529e4c: mov      lr, pc
00529e50: ldr      pc, [r3]
00529e54: ldr      ip, [r6, #4]
00529e58: mov      r4, r0
00529e5c: cmp      ip, #0
00529e60: movne    r2, r6
00529e64: bne      #0x529e70
00529e68: b        #0x529f0c
00529e6c: mov      ip, r3
00529e70: ldr      r3, [ip, #0x10]
00529e74: cmp      r4, r3
00529e78: ldrhi    r3, [ip, #0xc]
00529e7c: ldrls    r3, [ip, #8]
00529e80: movhi    ip, r2
00529e84: mov      r2, ip
00529e88: cmp      r3, #0
00529e8c: bne      #0x529e6c
00529e90: cmp      r6, ip
00529e94: beq      #0x529ea8
00529e98: ldr      r2, [ip, #0x10]
00529e9c: mov      r3, ip
00529ea0: cmp      r4, r2
00529ea4: bhs      #0x529edc
00529ea8: mov      lr, #0
00529eac: mov      r3, sp
00529eb0: str      r4, [sp]
00529eb4: mov      r1, r6
00529eb8: add      r0, sp, #0x10
00529ebc: add      r2, sp, #0x14
00529ec0: mov      r4, #0
00529ec4: str      r4, [sp, #4]
00529ec8: str      lr, [sp, #0xc]
00529ecc: str      ip, [sp, #0x14]
00529ed0: str      lr, [sp, #8]
00529ed4: bl       #0x529a30
00529ed8: ldr      r3, [sp, #0x10]
00529edc: mov      r2, r5
00529ee0: ldr      r1, [r2], #4
00529ee4: mov      r0, #1
00529ee8: str      r1, [r3, #0x14]
00529eec: ldr      r1, [r5, #4]
00529ef0: str      r1, [r3, #0x18]
00529ef4: ldr      r2, [r2, #4]
00529ef8: str      r2, [r3, #0x1c]
00529efc: add      sp, sp, #0x18
00529f00: pop      {r4, r5, r6, pc}
00529f04: mov      r3, r6
00529f08: b        #0x529e24
00529f0c: mov      ip, r6
00529f10: b        #0x529e90

# _ZNSt4priv10_List_baseIPK12PFGInnerEdgeSaIS3_EE5clearEv
00529f4c: push     {r4, r5, r6, lr}
00529f50: mov      r5, r0
00529f54: ldr      r0, [r0]
00529f58: cmp      r0, r5
00529f5c: bne      #0x529f68
00529f60: b        #0x529f80
00529f64: mov      r0, r4
00529f68: ldr      r4, [r0]
00529f6c: mov      r1, #0xc
00529f70: bl       #0x708f00
00529f74: cmp      r4, r5
00529f78: bne      #0x529f64
00529f7c: mov      r0, r5
00529f80: str      r0, [r5, #4]
00529f84: str      r0, [r5]
00529f88: pop      {r4, r5, r6, pc}
