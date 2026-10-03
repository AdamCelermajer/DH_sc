
# _ZNSt4priv11_Deque_baseIN14ObjectSearcher10TargetInfoESaIS2_EE17_M_initialize_mapEj.clone.4
004a2240: mov      r3, #8
004a2244: push     {r4, r5, r6, lr}
004a2248: mov      r1, r3
004a224c: mov      r4, r0
004a2250: str      r3, [r0, #0x24]
004a2254: mov      r2, #0
004a2258: add      r0, r0, #0x20
004a225c: bl       #0x4a21e0
004a2260: mov      r5, r0
004a2264: str      r0, [r4, #0x20]
004a2268: mov      r0, r4
004a226c: ldr      r6, [r0, #0x24]!
004a2270: bl       #0x4a1e0c
004a2274: sub      r6, r6, #1
004a2278: lsr      r6, r6, #1
004a227c: str      r0, [r5, r6, lsl #2]
004a2280: add      r3, r5, r6, lsl #2
004a2284: str      r3, [r4, #0xc]
004a2288: ldr      r2, [r5, r6, lsl #2]
004a228c: str      r3, [r4, #0x1c]
004a2290: add      r3, r2, #0x78
004a2294: stmib    r4, {r2, r3}
004a2298: ldr      r3, [r5, r6, lsl #2]
004a229c: str      r2, [r4]
004a22a0: add      r2, r3, #0x78
004a22a4: str      r3, [r4, #0x10]
004a22a8: str      r2, [r4, #0x18]
004a22ac: str      r3, [r4, #0x14]
004a22b0: pop      {r4, r5, r6, pc}

# _ZN14ObjectSearcher10TargetList6SearchEffRNS_11IObjectListE
004a3428: push     {r4, r5, r6, r7, r8, lr}
004a342c: sub      sp, sp, #0x18
004a3430: add      r5, sp, #0xc
004a3434: mov      ip, #0
004a3438: mov      r4, r0
004a343c: mov      r7, r1
004a3440: ldr      r0, [r0, #0x2c]
004a3444: mov      r1, r5
004a3448: mov      r6, r2
004a344c: mov      r8, r3
004a3450: str      ip, [sp, #0x14]
004a3454: str      ip, [sp, #0xc]
004a3458: str      ip, [sp, #0x10]
004a345c: bl       #0x393ae4
004a3460: ldr      r0, [r4, #0x2c]
004a3464: bl       #0x3935dc
004a3468: mov      r2, r7
004a346c: mov      r1, r0
004a3470: mov      r3, r5
004a3474: mov      r0, r4
004a3478: str      r6, [sp]
004a347c: str      r8, [sp, #4]
004a3480: bl       #0x4a2f34
004a3484: add      sp, sp, #0x18
004a3488: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14ObjectSearcher10TargetList6SearchEff
003d0020: ldr      ip, [pc, #0x4c]
003d0024: push     {r4, lr}
003d0028: ldr      lr, [pc, #0x48]
003d002c: add      ip, pc, ip
003d0030: ldr      r3, [pc, #0x44]
003d0034: ldr      lr, [ip, lr]
003d0038: sub      sp, sp, #0x18
003d003c: ldr      r3, [ip, r3]
003d0040: ldr      r4, [lr, #0x38]
003d0044: add      r3, r3, #8
003d0048: add      lr, r4, #0x80
003d004c: stmib    sp, {r3, lr}
003d0050: ldr      r4, [r4, #0x80]
003d0054: add      r3, sp, #4
003d0058: str      lr, [sp, #0x10]
003d005c: mov      lr, #0
003d0060: str      r4, [sp, #0xc]
003d0064: str      lr, [sp, #0x14]
003d0068: bl       #0x4a3428
003d006c: add      sp, sp, #0x18
003d0070: pop      {r4, pc}
003d0074: subseq   r4, ip, r4, ror #20
003d0078: strdeq   r3, r4, [r0], -r4
003d007c: andeq    r3, r0, r4, ror #10

# _ZNK13ItemInventory14CanMeleeAttackEv
003ffd38: push     {r4, lr}
003ffd3c: ldr      r3, [r0]
003ffd40: mov      lr, pc
003ffd44: ldr      pc, [r3, #8]
003ffd48: eor      r0, r0, #1
003ffd4c: uxtb     r0, r0
003ffd50: pop      {r4, pc}

# _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a191c: cmp      r1, #0
004a1920: push     {r4, lr}
004a1924: mov      r4, r0
004a1928: beq      #0x4a194c
004a192c: str      r1, [r0, #0x2c]
004a1930: ldr      r3, [r1]
004a1934: mov      r0, r1
004a1938: mov      lr, pc
004a193c: ldr      pc, [r3, #0x24]
004a1940: cmp      r0, #0
004a1944: ldrne    r3, [r4, #0x2c]
004a1948: strne    r3, [r4, #0x30]
004a194c: pop      {r4, pc}

# _ZNK6CharAI10AI_IsEnemyEPK10GameObject
003d574c: push     {r4, r5, r6, r7, r8, lr}
003d5750: ldr      r4, [pc, #0x2fc]
003d5754: subs     r7, r1, #0
003d5758: sub      sp, sp, #0x18
003d575c: mov      r5, r0
003d5760: add      r4, pc, r4
003d5764: beq      #0x3d59d0
003d5768: add      r6, sp, #0xc
003d576c: mov      r0, r6
003d5770: mov      r1, r7
003d5774: bl       #0x33dd70
003d5778: mov      r0, r6
003d577c: mov      r1, #0
003d5780: bl       #0x33ff8c
003d5784: subs     r6, r0, #0
003d5788: bne      #0x3d57bc
003d578c: cmp      r7, #0
003d5790: beq      #0x3d57b0
003d5794: ldr      r3, [r7]
003d5798: mov      r0, r7
003d579c: ldr      r1, [r5, #4]
003d57a0: mov      lr, pc
003d57a4: ldr      pc, [r3, #0x88]
003d57a8: cmp      r0, #0
003d57ac: bne      #0x3d58e4
003d57b0: mov      r0, #0
003d57b4: add      sp, sp, #0x18
003d57b8: pop      {r4, r5, r6, r7, r8, pc}
003d57bc: ldr      r8, [r6, #0xf4]
003d57c0: cmp      r8, #0
003d57c4: bne      #0x3d578c
003d57c8: bl       #0x3a3180
003d57cc: cmp      r0, #0
003d57d0: blt      #0x3d597c
003d57d4: mov      r0, r6
003d57d8: bl       #0x3a3180
003d57dc: ldr      r7, [pc, #0x274]
003d57e0: ldr      r3, [r4, r7]
003d57e4: ldr      r3, [r3]
003d57e8: cmp      r0, r3
003d57ec: blt      #0x3d5814
003d57f0: ldr      r3, [pc, #0x264]
003d57f4: ldr      r3, [r4, r3]
003d57f8: ldr      r3, [r3]
003d57fc: cmp      r3, #2
003d5800: moveq    r3, #0
003d5804: streq    r3, [r3]
003d5808: beq      #0x3d5814
003d580c: cmp      r3, #1
003d5810: beq      #0x3d5a20
003d5814: ldr      r0, [r5, #4]
003d5818: bl       #0x3a3180
003d581c: cmp      r0, #0
003d5820: blt      #0x3d5924
003d5824: ldr      r0, [r5, #4]
003d5828: bl       #0x3a3180
003d582c: ldr      r3, [r4, r7]
003d5830: ldr      r3, [r3]
003d5834: cmp      r0, r3
003d5838: blt      #0x3d5860
003d583c: ldr      r3, [pc, #0x218]
003d5840: ldr      r3, [r4, r3]
003d5844: ldr      r3, [r3]
003d5848: cmp      r3, #2
003d584c: moveq    r3, #0
003d5850: streq    r3, [r3]
003d5854: beq      #0x3d5860
003d5858: cmp      r3, #1
003d585c: beq      #0x3d59ec
003d5860: ldr      r3, [r5, #4]
003d5864: mov      r0, r3
003d5868: ldr      r3, [r3]
003d586c: mov      lr, pc
003d5870: ldr      pc, [r3, #0x28]
003d5874: cmp      r0, #0
003d5878: bne      #0x3d5908
003d587c: ldr      r3, [pc, #0x1dc]
003d5880: ldr      r0, [r5, #4]
003d5884: ldr      r3, [r4, r3]
003d5888: ldr      r4, [r3]
003d588c: bl       #0x3a3180
003d5890: mov      r3, #0xc
003d5894: mla      r4, r3, r0, r4
003d5898: mov      r0, r6
003d589c: bl       #0x3a3180
003d58a0: ldr      ip, [r4, #4]
003d58a4: cmp      ip, #0
003d58a8: beq      #0x3d57b0
003d58ac: ldr      r2, [r4, #8]
003d58b0: ldr      r3, [r2, #4]
003d58b4: cmp      r0, r3
003d58b8: movne    r3, #0
003d58bc: bne      #0x3d58d0
003d58c0: b        #0x3d59e0
003d58c4: ldr      r1, [r2, #4]
003d58c8: cmp      r0, r1
003d58cc: beq      #0x3d59e0
003d58d0: add      r3, r3, #1
003d58d4: cmp      r3, ip
003d58d8: add      r2, r2, #0xc
003d58dc: bne      #0x3d58c4
003d58e0: b        #0x3d57b0
003d58e4: mov      r0, r7
003d58e8: ldr      r1, [r5, #4]
003d58ec: ldr      r3, [r7]
003d58f0: mov      lr, pc
003d58f4: ldr      pc, [r3, #0x90]
003d58f8: cmp      r0, #8
003d58fc: movne    r0, #0
003d5900: moveq    r0, #1
003d5904: b        #0x3d57b4
003d5908: ldr      r3, [r6]
003d590c: mov      r0, r6
003d5910: mov      lr, pc
003d5914: ldr      pc, [r3, #0x28]
003d5918: cmp      r0, #0
003d591c: bne      #0x3d57b0
003d5920: b        #0x3d587c
003d5924: ldr      r3, [pc, #0x130]
003d5928: ldr      r3, [r4, r3]
003d592c: ldr      r3, [r3]
003d5930: cmp      r3, #2
003d5934: moveq    r3, #0
003d5938: streq    r3, [r3]
003d593c: beq      #0x3d5824
003d5940: cmp      r3, #1
003d5944: bne      #0x3d5824
003d5948: ldr      r0, [pc, #0x114]
003d594c: ldr      r1, [pc, #0x114]
003d5950: ldr      r2, [pc, #0x114]
003d5954: ldr      r0, [r4, r0]
003d5958: ldr      r3, [pc, #0x110]
003d595c: movw     ip, #0x109
003d5960: add      r1, pc, r1
003d5964: add      r2, pc, r2
003d5968: add      r3, pc, r3
003d596c: add      r0, r0, #0xa8
003d5970: str      ip, [sp]
003d5974: bl       #0x30e004
003d5978: b        #0x3d5824
003d597c: ldr      r3, [pc, #0xd8]
003d5980: ldr      r3, [r4, r3]
003d5984: ldr      r3, [r3]
003d5988: cmp      r3, #2
003d598c: streq    r8, [r8]
003d5990: beq      #0x3d57d4
003d5994: cmp      r3, #1
003d5998: bne      #0x3d57d4
003d599c: ldr      r0, [pc, #0xc0]
003d59a0: ldr      r1, [pc, #0xcc]
003d59a4: ldr      r2, [pc, #0xcc]
003d59a8: ldr      r0, [r4, r0]
003d59ac: ldr      r3, [pc, #0xc8]
003d59b0: movw     ip, #0x107
003d59b4: add      r1, pc, r1
003d59b8: add      r2, pc, r2
003d59bc: add      r3, pc, r3
003d59c0: add      r0, r0, #0xa8
003d59c4: str      ip, [sp]
003d59c8: bl       #0x30e004
003d59cc: b        #0x3d57d4
003d59d0: ldr      r7, [r0, #0x40]
003d59d4: cmp      r7, #0
003d59d8: beq      #0x3d57b0
003d59dc: b        #0x3d5768
003d59e0: ldr      r0, [r2, #8]
003d59e4: lsr      r0, r0, #0x1f
003d59e8: b        #0x3d57b4
003d59ec: ldr      r0, [pc, #0x70]
003d59f0: ldr      r1, [pc, #0x88]
003d59f4: ldr      r2, [pc, #0x88]
003d59f8: ldr      r0, [r4, r0]
003d59fc: ldr      r3, [pc, #0x84]
003d5a00: movw     ip, #0x10a
003d5a04: add      r1, pc, r1
003d5a08: add      r2, pc, r2
003d5a0c: add      r3, pc, r3
003d5a10: add      r0, r0, #0xa8
003d5a14: str      ip, [sp]
003d5a18: bl       #0x30e004
003d5a1c: b        #0x3d5860
003d5a20: ldr      r0, [pc, #0x3c]
003d5a24: ldr      r1, [pc, #0x60]
003d5a28: ldr      r2, [pc, #0x60]
003d5a2c: ldr      r0, [r4, r0]
003d5a30: ldr      r3, [pc, #0x5c]
003d5a34: mov      ip, #0x108
003d5a38: add      r1, pc, r1
003d5a3c: add      r2, pc, r2
003d5a40: add      r3, pc, r3
003d5a44: add      r0, r0, #0xa8
003d5a48: str      ip, [sp]
003d5a4c: bl       #0x30e004
003d5a50: b        #0x3d5814
003d5a54: subseq   pc, fp, r0, lsr r3
003d5a58: andeq    r2, r0, r4, asr #4
003d5a5c: andeq    r3, r0, r0, asr #19
003d5a60: andeq    r4, r0, ip, lsr #12
003d5a64: andeq    r1, r0, r0, asr #19
003d5a68: subeq    r8, lr, r8, ror sl
003d5a6c: subeq    pc, lr, r4, lsl sp
003d5a70: subeq    pc, lr, r8, asr ip
003d5a74: subeq    r8, lr, r4, lsr #20
003d5a78: subeq    pc, lr, r0, ror #24
003d5a7c: subeq    pc, lr, r4, lsl #24
003d5a80: ldrdeq   r8, sb, [lr], #-0x94
003d5a84: umaaleq  pc, lr, r0, ip
003d5a88: strheq   pc, [lr], #-0xb4
003d5a8c: subeq    r8, lr, r0, lsr #19

# _ZN6CharAI12AI_SetTargetEP10GameObjectb
003d6890: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr      r5, [pc, #0x204]
003d6898: ldr      r7, [pc, #0x204]
003d689c: sub      sp, sp, #0x78
003d68a0: add      r5, pc, r5
003d68a4: ldr      r3, [r5, r7]
003d68a8: mov      r4, r0
003d68ac: cmp      r2, #0
003d68b0: ldr      r3, [r3]
003d68b4: mov      r6, r1
003d68b8: str      r1, [r4, #0x3c]
003d68bc: str      r3, [sp, #0x74]
003d68c0: bne      #0x3d6a20
003d68c4: ldr      r3, [r0, #0x40]
003d68c8: ldr      sb, [pc, #0x1d8]
003d68cc: add      r8, sp, #0x5c
003d68d0: cmp      r3, r1
003d68d4: ldrne    r1, [r0, #4]
003d68d8: ldr      sl, [r5, sb]
003d68dc: movwne   r3, #0x14d0
003d68e0: strhne   r2, [r1, r3]
003d68e4: mov      r0, sl
003d68e8: bl       #0x337888
003d68ec: ldr      r1, [pc, #0x1b8]
003d68f0: add      r2, sp, #0x10
003d68f4: mov      r0, r8
003d68f8: add      r1, pc, r1
003d68fc: bl       #0x3140ec
003d6900: mov      r0, sl
003d6904: mov      r1, r8
003d6908: bl       #0x337a88
003d690c: mov      sl, r0
003d6910: ldr      r0, [sp, #0x70]
003d6914: cmp      r0, r8
003d6918: beq      #0x3d6938
003d691c: cmp      r0, #0
003d6920: beq      #0x3d6938
003d6924: ldr      r1, [sp, #0x5c]
003d6928: rsb      r1, r0, r1
003d692c: cmp      r1, #0x80
003d6930: bhi      #0x3d6a4c
003d6934: bl       #0x708f00
003d6938: cmp      sl, #0
003d693c: beq      #0x3d69a4
003d6940: ldr      r3, [r4, #0x40]
003d6944: cmp      r3, r6
003d6948: beq      #0x3d69a4
003d694c: subs     r3, r3, #0
003d6950: movne    r3, #1
003d6954: subs     r2, r6, #0
003d6958: movne    r2, #1
003d695c: tst      r2, r3
003d6960: bne      #0x3d6a28
003d6964: cmp      r3, #0
003d6968: beq      #0x3d6a18
003d696c: ldr      sl, [r5, sb]
003d6970: add      r8, sp, #0x2c
003d6974: mov      r0, sl
003d6978: bl       #0x337888
003d697c: ldr      r1, [pc, #0x12c]
003d6980: add      r2, sp, #8
003d6984: mov      r0, r8
003d6988: add      r1, pc, r1
003d698c: bl       #0x3140ec
003d6990: mov      r0, sl
003d6994: mov      r1, r8
003d6998: bl       #0x337a88
003d699c: mov      r0, r8
003d69a0: bl       #0x318254
003d69a4: cmp      r6, #0
003d69a8: str      r6, [r4, #0x40]
003d69ac: beq      #0x3d69fc
003d69b0: ldr      r0, [r4, #4]
003d69b4: bl       #0x3a2fec
003d69b8: ldr      r2, [r4, #0x44]
003d69bc: ldr      r3, [r4, #0x40]
003d69c0: cmp      r3, r2
003d69c4: movne    r2, #0
003d69c8: strbne   r2, [r4, #0x4c]
003d69cc: movne    r2, r3
003d69d0: str      r2, [r4, #0x44]
003d69d4: mov      r0, r3
003d69d8: ldr      r3, [r3]
003d69dc: mov      lr, pc
003d69e0: ldr      pc, [r3, #0x34]
003d69e4: eor      r0, r0, #1
003d69e8: strb     r0, [r4, #0x48]
003d69ec: ldr      r1, [r4, #0x40]
003d69f0: mov      r0, r4
003d69f4: bl       #0x3d4ed8
003d69f8: strb     r0, [r4, #0x49]
003d69fc: ldr      r3, [r5, r7]
003d6a00: ldr      r2, [sp, #0x74]
003d6a04: ldr      r3, [r3]
003d6a08: cmp      r2, r3
003d6a0c: bne      #0x3d6a9c
003d6a10: add      sp, sp, #0x78
003d6a14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp      r2, #0
003d6a1c: bne      #0x3d6a5c
003d6a20: str      r6, [r4, #0x40]
003d6a24: b        #0x3d69fc
003d6a28: ldr      sl, [r5, sb]
003d6a2c: add      r8, sp, #0x44
003d6a30: mov      r0, sl
003d6a34: bl       #0x337888
003d6a38: ldr      r1, [pc, #0x74]
003d6a3c: add      r2, sp, #0xc
003d6a40: mov      r0, r8
003d6a44: add      r1, pc, r1
003d6a48: b        #0x3d698c
003d6a4c: bl       #0x310440
003d6a50: cmp      sl, #0
003d6a54: beq      #0x3d69a4
003d6a58: b        #0x3d6940
003d6a5c: ldr      sl, [r5, sb]
003d6a60: add      r8, sp, #0x14
003d6a64: mov      r0, sl
003d6a68: bl       #0x337888
003d6a6c: ldr      r1, [pc, #0x44]
003d6a70: add      r2, sp, #4
003d6a74: mov      r0, r8
003d6a78: add      r1, pc, r1
003d6a7c: bl       #0x3140ec
003d6a80: mov      r1, r8
003d6a84: mov      r0, sl
003d6a88: bl       #0x337a88
003d6a8c: mov      r0, r8
003d6a90: bl       #0x318254
003d6a94: str      r6, [r4, #0x40]
003d6a98: b        #0x3d69b0
003d6a9c: bl       #0x30e310
003d6aa0: ldrsheq  lr, [fp], #-0x10
003d6aa4: andeq    r4, r0, ip, lsr #1
003d6aa8: andeq    r0, r0, r4, lsl #17
003d6aac: subeq    lr, lr, r0, ror #27
003d6ab0: subeq    lr, lr, r8, ror #26
003d6ab4: subeq    lr, lr, ip, lsr #25
003d6ab8: subeq    lr, lr, r8, ror ip

# _ZNK6CharAI12AI_IsInRangeEPK10GameObject
003d6604: push     {r4, r5, r6, r7, r8, sl, lr}
003d6608: ldr      r4, [pc, #0x1d0]
003d660c: ldr      r5, [pc, #0x1d0]
003d6610: sub      sp, sp, #0x4c
003d6614: add      r4, pc, r4
003d6618: ldr      r3, [r4, r5]
003d661c: subs     r7, r1, #0
003d6620: mov      r6, r0
003d6624: ldr      r3, [r3]
003d6628: str      r3, [sp, #0x44]
003d662c: beq      #0x3d6794
003d6630: ldr      r3, [r6, #4]
003d6634: add      r1, sp, #8
003d6638: add      r2, sp, #4
003d663c: mov      r0, r3
003d6640: ldr      ip, [r3]
003d6644: mov      r3, sp
003d6648: mov      lr, pc
003d664c: ldr      pc, [ip, #0x128]
003d6650: cmp      r0, #0
003d6654: bne      #0x3d6678
003d6658: mov      r0, #0
003d665c: ldr      r3, [r4, r5]
003d6660: ldr      r2, [sp, #0x44]
003d6664: ldr      r3, [r3]
003d6668: cmp      r2, r3
003d666c: bne      #0x3d67dc
003d6670: add      sp, sp, #0x4c
003d6674: pop      {r4, r5, r6, r7, r8, sl, pc}
003d6678: ldr      r0, [r6, #4]
003d667c: bl       #0x3935dc
003d6680: mov      r6, r0
003d6684: mov      r0, r7
003d6688: bl       #0x3935dc
003d668c: mov      r7, r0
003d6690: ldr      r1, [r0]
003d6694: ldr      r0, [r6]
003d6698: bl       #0x30e3ac
003d669c: ldr      r1, [r7, #4]
003d66a0: mov      sl, r0
003d66a4: ldr      r0, [r6, #4]
003d66a8: bl       #0x30e3ac
003d66ac: ldr      r1, [r7, #8]
003d66b0: mov      r8, r0
003d66b4: ldr      r0, [r6, #8]
003d66b8: bl       #0x30e3ac
003d66bc: mov      r1, sl
003d66c0: mov      r7, r0
003d66c4: mov      r0, sl
003d66c8: bl       #0x30ed6c
003d66cc: mov      r1, r8
003d66d0: mov      r6, r0
003d66d4: mov      r0, r8
003d66d8: bl       #0x30ed6c
003d66dc: mov      r1, r0
003d66e0: mov      r0, r6
003d66e4: bl       #0x30eba4
003d66e8: mov      r1, r7
003d66ec: mov      r6, r0
003d66f0: mov      r0, r7
003d66f4: bl       #0x30ed6c
003d66f8: mov      r1, r0
003d66fc: mov      r0, r6
003d6700: bl       #0x30eba4
003d6704: ldr      r3, [pc, #0xdc]
003d6708: mov      r8, r0
003d670c: add      r6, sp, #0x2c
003d6710: ldr      r7, [r4, r3]
003d6714: mov      r0, r7
003d6718: bl       #0x337888
003d671c: ldr      r1, [pc, #0xc8]
003d6720: add      r2, sp, #0x10
003d6724: mov      r0, r6
003d6728: add      r1, pc, r1
003d672c: bl       #0x3140ec
003d6730: mov      r1, r6
003d6734: mov      r0, r7
003d6738: bl       #0x337a88
003d673c: mov      sl, r0
003d6740: mov      r0, r6
003d6744: bl       #0x318254
003d6748: cmp      sl, #0
003d674c: bne      #0x3d67a4
003d6750: ldr      r0, [sp, #8]
003d6754: mul      r0, r0, r0
003d6758: bl       #0x30e964
003d675c: mov      r1, r8
003d6760: bl       #0x30e9ac
003d6764: cmp      r0, #0
003d6768: beq      #0x3d6658
003d676c: ldr      r0, [sp, #4]
003d6770: mov      r6, #0
003d6774: mul      r0, r0, r0
003d6778: bl       #0x30e964
003d677c: mov      r1, r8
003d6780: bl       #0x30e4b4
003d6784: cmp      r0, #0
003d6788: movne    r6, #1
003d678c: uxtb     r0, r6
003d6790: b        #0x3d665c
003d6794: ldr      r7, [r0, #0x40]
003d6798: cmp      r7, #0
003d679c: beq      #0x3d6658
003d67a0: b        #0x3d6630
003d67a4: mov      r0, r7
003d67a8: bl       #0x337888
003d67ac: ldr      r1, [pc, #0x3c]
003d67b0: add      r6, sp, #0x14
003d67b4: add      r2, sp, #0xc
003d67b8: add      r1, pc, r1
003d67bc: mov      r0, r6
003d67c0: bl       #0x3140ec
003d67c4: mov      r0, r7
003d67c8: mov      r1, r6
003d67cc: bl       #0x337a88
003d67d0: mov      r0, r6
003d67d4: bl       #0x318254
003d67d8: b        #0x3d6750
003d67dc: bl       #0x30e310
003d67e0: subseq   lr, fp, ip, ror r4
003d67e4: andeq    r4, r0, ip, lsr #1
003d67e8: andeq    r0, r0, r4, lsl #17
003d67ec: strheq   lr, [lr], #-0xf0
003d67f0: subeq    lr, lr, r8, lsr pc

# _ZNK6CharAI17AI_IsInMeleeRangeEPK10GameObject
003d6188: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d618c: ldr      r4, [pc, #0x230]
003d6190: ldr      r5, [pc, #0x230]
003d6194: sub      sp, sp, #0x4c
003d6198: add      r4, pc, r4
003d619c: ldr      r3, [r4, r5]
003d61a0: subs     r6, r1, #0
003d61a4: mov      r8, r0
003d61a8: ldr      r3, [r3]
003d61ac: str      r3, [sp, #0x44]
003d61b0: beq      #0x3d6368
003d61b4: mov      r1, r6
003d61b8: mov      r0, sp
003d61bc: bl       #0x33dd70
003d61c0: mov      r0, sp
003d61c4: mov      r1, #0
003d61c8: bl       #0x33ff8c
003d61cc: subs     sl, r0, #0
003d61d0: mov      r7, sp
003d61d4: bne      #0x3d6200
003d61d8: mov      r0, r8
003d61dc: mov      r1, r6
003d61e0: bl       #0x3d4f98
003d61e4: ldr      r3, [r4, r5]
003d61e8: ldr      r2, [sp, #0x44]
003d61ec: ldr      r3, [r3]
003d61f0: cmp      r2, r3
003d61f4: bne      #0x3d63c0
003d61f8: add      sp, sp, #0x4c
003d61fc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d6200: ldr      r3, [sl, #0xf4]
003d6204: cmp      r3, #0
003d6208: bne      #0x3d61d8
003d620c: ldr      r3, [sl]
003d6210: ldr      r1, [r8, #4]
003d6214: mov      lr, pc
003d6218: ldr      pc, [r3, #0x90]
003d621c: cmp      r0, #8
003d6220: bne      #0x3d61d8
003d6224: ldr      r0, [r8, #4]
003d6228: bl       #0x3935dc
003d622c: mov      r7, r0
003d6230: mov      r0, r6
003d6234: bl       #0x3935dc
003d6238: mov      r6, r0
003d623c: ldr      r1, [r0]
003d6240: ldr      r0, [r7]
003d6244: bl       #0x30e3ac
003d6248: ldr      r1, [r6, #4]
003d624c: mov      fp, r0
003d6250: ldr      r0, [r7, #4]
003d6254: bl       #0x30e3ac
003d6258: ldr      r1, [r6, #8]
003d625c: mov      sb, r0
003d6260: ldr      r0, [r7, #8]
003d6264: bl       #0x30e3ac
003d6268: mov      r1, fp
003d626c: mov      r7, r0
003d6270: mov      r0, fp
003d6274: bl       #0x30ed6c
003d6278: mov      r1, sb
003d627c: mov      r6, r0
003d6280: mov      r0, sb
003d6284: bl       #0x30ed6c
003d6288: mov      r1, r0
003d628c: mov      r0, r6
003d6290: bl       #0x30eba4
003d6294: mov      r1, r7
003d6298: mov      r6, r0
003d629c: mov      r0, r7
003d62a0: bl       #0x30ed6c
003d62a4: mov      r1, r0
003d62a8: mov      r0, r6
003d62ac: bl       #0x30eba4
003d62b0: mov      r7, r0
003d62b4: mov      r0, r8
003d62b8: bl       #0x3d4c34
003d62bc: mov      r6, r0
003d62c0: add      r0, sl, #0x3c8
003d62c4: bl       #0x3d4c34
003d62c8: mov      r1, r0
003d62cc: mov      r0, r6
003d62d0: bl       #0x30eba4
003d62d4: ldr      sb, [pc, #0xf0]
003d62d8: mov      r6, r0
003d62dc: add      r8, sp, #0x2c
003d62e0: ldr      sl, [r4, sb]
003d62e4: mov      r0, sl
003d62e8: bl       #0x337888
003d62ec: ldr      r1, [pc, #0xdc]
003d62f0: add      r2, sp, #0x10
003d62f4: mov      r0, r8
003d62f8: add      r1, pc, r1
003d62fc: bl       #0x3140ec
003d6300: mov      r0, sl
003d6304: mov      r1, r8
003d6308: bl       #0x337a88
003d630c: mov      sl, r0
003d6310: ldr      r0, [sp, #0x40]
003d6314: cmp      r0, r8
003d6318: beq      #0x3d6338
003d631c: cmp      r0, #0
003d6320: beq      #0x3d6338
003d6324: ldr      r1, [sp, #0x2c]
003d6328: rsb      r1, r0, r1
003d632c: cmp      r1, #0x80
003d6330: bhi      #0x3d63b8
003d6334: bl       #0x708f00
003d6338: cmp      sl, #0
003d633c: bne      #0x3d637c
003d6340: mov      r1, r6
003d6344: mov      r0, r6
003d6348: bl       #0x30ed6c
003d634c: mov      r1, r7
003d6350: bl       #0x30e2f8
003d6354: cmp      r0, #0
003d6358: mov      r0, #0
003d635c: movne    r0, #1
003d6360: uxtb     r0, r0
003d6364: b        #0x3d61e4
003d6368: ldr      r6, [r0, #0x40]
003d636c: cmp      r6, #0
003d6370: moveq    r0, r6
003d6374: beq      #0x3d61e4
003d6378: b        #0x3d61b4
003d637c: ldr      sl, [r4, sb]
003d6380: add      r8, sp, #0x14
003d6384: mov      r0, sl
003d6388: bl       #0x337888
003d638c: ldr      r1, [pc, #0x40]
003d6390: add      r2, sp, #0xc
003d6394: mov      r0, r8
003d6398: add      r1, pc, r1
003d639c: bl       #0x3140ec
003d63a0: mov      r0, sl
003d63a4: mov      r1, r8
003d63a8: bl       #0x337a88
003d63ac: mov      r0, r8
003d63b0: bl       #0x318254
003d63b4: b        #0x3d6340
003d63b8: bl       #0x310440
003d63bc: b        #0x3d6338
003d63c0: bl       #0x30e310
003d63c4: ldrsheq  lr, [fp], #-0x88
003d63c8: andeq    r4, r0, ip, lsr #1
003d63cc: andeq    r0, r0, r4, lsl #17
003d63d0: subeq    pc, lr, r0, ror #7
003d63d4: subeq    pc, lr, r8, asr r3
