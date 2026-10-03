
# _ZN6CharAI13_UpdateTargetEv
003cb908: push     {r4, r5, r6, lr}
003cb90c: mov      r4, r0
003cb910: ldr      r0, [r0, #4]
003cb914: add      r0, r0, #0x4f0
003cb918: add      r0, r0, #0xc
003cb91c: bl       #0x3c0230
003cb920: cmp      r0, #0
003cb924: beq      #0x3cb92c
003cb928: pop      {r4, r5, r6, pc}
003cb92c: ldr      r0, [r4, #4]
003cb930: add      r0, r0, #0x4f0
003cb934: add      r0, r0, #0xc
003cb938: bl       #0x3c01c0
003cb93c: cmp      r0, #0
003cb940: bne      #0x3cb928
003cb944: ldr      r3, [r4, #0x40]
003cb948: cmp      r3, #0
003cb94c: beq      #0x3cb928
003cb950: mov      r0, r3
003cb954: ldr      r1, [r4, #4]
003cb958: ldr      r3, [r3]
003cb95c: mov      lr, pc
003cb960: ldr      pc, [r3, #0x88]
003cb964: subs     r2, r0, #0
003cb968: beq      #0x3cbaa4
003cb96c: ldr      r3, [r4, #0x40]
003cb970: cmp      r3, #0
003cb974: beq      #0x3cb928
003cb978: ldr      r0, [r4, #4]
003cb97c: bl       #0x3a2fec
003cb980: ldr      r3, [r4, #0x40]
003cb984: mov      r0, r3
003cb988: ldr      r3, [r3]
003cb98c: mov      lr, pc
003cb990: ldr      pc, [r3, #0x34]
003cb994: ldrb     r3, [r4, #0x48]
003cb998: eor      r0, r0, #1
003cb99c: uxtb     r5, r0
003cb9a0: cmp      r3, #0
003cb9a4: bne      #0x3cba6c
003cb9a8: cmp      r5, #0
003cb9ac: bne      #0x3cbaf8
003cb9b0: ldr      r1, [r4, #0x40]
003cb9b4: strb     r5, [r4, #0x48]
003cb9b8: cmp      r1, #0
003cb9bc: beq      #0x3cb928
003cb9c0: mov      r0, r4
003cb9c4: bl       #0x3d4ed8
003cb9c8: ldrb     r3, [r4, #0x49]
003cb9cc: mov      r5, r0
003cb9d0: cmp      r3, #0
003cb9d4: bne      #0x3cba88
003cb9d8: cmp      r0, #0
003cb9dc: bne      #0x3cbae4
003cb9e0: ldr      r3, [r4, #0x40]
003cb9e4: strb     r5, [r4, #0x49]
003cb9e8: cmp      r3, #0
003cb9ec: beq      #0x3cb928
003cb9f0: cmp      r5, #0
003cb9f4: beq      #0x3cb928
003cb9f8: mov      r0, r3
003cb9fc: ldr      r1, [r4, #4]
003cba00: ldr      r3, [r3]
003cba04: mov      lr, pc
003cba08: ldr      pc, [r3, #0x88]
003cba0c: cmp      r0, #0
003cba10: beq      #0x3cb928
003cba14: ldr      r3, [r4, #4]
003cba18: mov      r0, r3
003cba1c: ldr      r3, [r3]
003cba20: mov      lr, pc
003cba24: ldr      pc, [r3, #0x124]
003cba28: cmp      r0, #0
003cba2c: beq      #0x3cbabc
003cba30: mov      r0, r4
003cba34: ldr      r1, [r4, #0x40]
003cba38: bl       #0x3d63d8
003cba3c: cmp      r0, #0
003cba40: bne      #0x3cbb0c
003cba44: mov      r0, r4
003cba48: ldr      r1, [r4, #0x40]
003cba4c: bl       #0x3d6604
003cba50: cmp      r0, #0
003cba54: beq      #0x3cbad0
003cba58: ldr      r2, [r4, #0x40]
003cba5c: ldr      r0, [r4, #4]
003cba60: mov      r1, #0xf
003cba64: pop      {r4, r5, r6, lr}
003cba68: b        #0x3a4d5c
003cba6c: cmp      r5, #0
003cba70: bne      #0x3cb9b0
003cba74: ldr      r0, [r4, #4]
003cba78: mov      r1, #0xa
003cba7c: ldr      r2, [r4, #0x40]
003cba80: bl       #0x3a4d5c
003cba84: b        #0x3cb9b0
003cba88: cmp      r0, #0
003cba8c: bne      #0x3cb9e0
003cba90: ldr      r0, [r4, #4]
003cba94: mov      r1, #0xc
003cba98: ldr      r2, [r4, #0x40]
003cba9c: bl       #0x3a4d5c
003cbaa0: b        #0x3cb9e0
003cbaa4: ldr      r0, [r4, #4]
003cbaa8: mov      r1, #0xc
003cbaac: str      r2, [r4, #0x40]
003cbab0: str      r2, [r4, #0x44]
003cbab4: pop      {r4, r5, r6, lr}
003cbab8: b        #0x3a4d5c
003cbabc: mov      r0, r4
003cbac0: ldr      r1, [r4, #0x40]
003cbac4: bl       #0x3d6188
003cbac8: cmp      r0, #0
003cbacc: bne      #0x3cbb20
003cbad0: ldr      r2, [r4, #0x40]
003cbad4: ldr      r0, [r4, #4]
003cbad8: mov      r1, #0xe
003cbadc: pop      {r4, r5, r6, lr}
003cbae0: b        #0x3a4d5c
003cbae4: ldr      r0, [r4, #4]
003cbae8: mov      r1, #0xd
003cbaec: ldr      r2, [r4, #0x40]
003cbaf0: bl       #0x3a4d5c
003cbaf4: b        #0x3cb9e0
003cbaf8: ldr      r0, [r4, #4]
003cbafc: mov      r1, #0xb
003cbb00: ldr      r2, [r4, #0x40]
003cbb04: bl       #0x3a4d5c
003cbb08: b        #0x3cb9b0
003cbb0c: ldr      r2, [r4, #0x40]
003cbb10: ldr      r0, [r4, #4]
003cbb14: mov      r1, #0x10
003cbb18: pop      {r4, r5, r6, lr}
003cbb1c: b        #0x3a4d5c
003cbb20: ldr      r2, [r4, #0x40]
003cbb24: ldr      r0, [r4, #4]
003cbb28: mov      r1, #0x11
003cbb2c: pop      {r4, r5, r6, lr}
003cbb30: b        #0x3a4d5c

# _ZNK9Character11GetCharAIIdEv
003a2fec: ldr      r0, [r0, #0xffc]
003a2ff0: ldr      r3, [pc, #0x24]
003a2ff4: cmp      r0, #0
003a2ff8: add      r3, pc, r3
003a2ffc: blt      #0x3a3014
003a3000: ldr      r2, [pc, #0x18]
003a3004: ldr      r3, [r3, r2]
003a3008: ldr      r3, [r3]
003a300c: cmp      r0, r3
003a3010: bxlt     lr
003a3014: mov      r0, #8
003a3018: bx       lr

# _ZN10AISMonster12OnTargetDiedEv
003dd550: push     {r4, lr}
003dd554: ldr      r3, [r0, #0x98]
003dd558: mov      r4, r0
003dd55c: ldr      r0, [r3, #0x378]
003dd560: bl       #0x40559c
003dd564: ldr      r0, [r4, #0x98]
003dd568: mov      r1, #0
003dd56c: mov      r2, r1
003dd570: add      r0, r0, #0x3c8
003dd574: bl       #0x3d6890
003dd578: ldr      r0, [r4, #0x98]
003dd57c: add      r0, r0, #0x3c8
003dd580: pop      {r4, lr}
003dd584: b        #0x3d49c4

# _ZNK9Character18GetCharAIFactionIdEv
003a3180: ldr      r0, [r0, #0xff8]
003a3184: ldr      r3, [pc, #0x24]
003a3188: cmp      r0, #0
003a318c: add      r3, pc, r3
003a3190: blt      #0x3a31a8
003a3194: ldr      r2, [pc, #0x18]
003a3198: ldr      r3, [r3, r2]
003a319c: ldr      r3, [r3]
003a31a0: cmp      r0, r3
003a31a4: bxlt     lr
003a31a8: mov      r0, #0xa
003a31ac: bx       lr
003a31b0: subseq   r1, pc, r4, lsl #18
003a31b4: andeq    r2, r0, r4, asr #4

# _ZN10AISDefault20OnTargetInMeleeRangeEv
003dc284: push     {r4, r5, r6, lr}
003dc288: mov      r4, r0
003dc28c: ldr      r0, [r0, #0x98]
003dc290: mov      r1, #0
003dc294: add      r0, r0, #0x3c8
003dc298: bl       #0x3d574c
003dc29c: subs     r5, r0, #0
003dc2a0: bne      #0x3dc2e0
003dc2a4: ldr      r0, [r4, #0x98]
003dc2a8: ldr      r1, [r0, #0x408]
003dc2ac: add      r0, r0, #0x3c8
003dc2b0: bl       #0x3d4f98
003dc2b4: cmp      r0, #0
003dc2b8: bne      #0x3dc2c0
003dc2bc: pop      {r4, r5, r6, pc}
003dc2c0: ldr      r3, [r4, #0x98]
003dc2c4: ldr      r0, [r3, #0x378]
003dc2c8: bl       #0x40559c
003dc2cc: ldr      r0, [r4, #0x98]
003dc2d0: mov      r1, r5
003dc2d4: add      r0, r0, #0x3c8
003dc2d8: pop      {r4, r5, r6, lr}
003dc2dc: b        #0x3cff34
003dc2e0: ldr      r3, [r4, #0x98]
003dc2e4: ldr      r0, [r3, #0x378]
003dc2e8: bl       #0x40559c
003dc2ec: ldr      r3, [r4, #0x98]
003dc2f0: ldr      r1, [r3, #0x408]
003dc2f4: ldr      r0, [r3, #0x378]
003dc2f8: pop      {r4, r5, r6, lr}
003dc2fc: b        #0x405b04

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

# _ZN10AISMonster18OnTargetOutOfSightEv
003dd500: push     {r4, r5, r6, lr}
003dd504: ldr      r3, [r0, #0x98]
003dd508: mov      r4, r0
003dd50c: ldr      r0, [r3, #0x408]
003dd510: cmp      r0, #0
003dd514: beq      #0x3dd530
003dd518: ldr      r5, [r3, #0x378]
003dd51c: bl       #0x3935dc
003dd520: mov      r1, r0
003dd524: mov      r0, r5
003dd528: bl       #0x4054e4
003dd52c: ldr      r3, [r4, #0x98]
003dd530: mov      r1, #0
003dd534: add      r0, r3, #0x3c8
003dd538: mov      r2, r1
003dd53c: bl       #0x3d6890
003dd540: ldr      r0, [r4, #0x98]
003dd544: add      r0, r0, #0x3c8
003dd548: pop      {r4, r5, r6, lr}
003dd54c: b        #0x3d49c4

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

# _ZNK6CharAI12AI_IsInSightEf
003d4ea0: push     {r4, r5, r6, lr}
003d4ea4: ldr      r0, [r0, #4]
003d4ea8: mov      r5, r1
003d4eac: bl       #0x3a3024
003d4eb0: ldr      r0, [r0, #0x3c]
003d4eb4: mov      r4, #0
003d4eb8: mov      r1, r0
003d4ebc: bl       #0x30ed6c
003d4ec0: mov      r1, r5
003d4ec4: bl       #0x30e2f8
003d4ec8: cmp      r0, #0
003d4ecc: movne    r4, #1
003d4ed0: and      r0, r4, #1
003d4ed4: pop      {r4, r5, r6, pc}

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

# _ZNK6CharAI12AI_IsInSightEPK10GameObject
003d4ed8: push     {r4, r5, r6, r7, r8, lr}
003d4edc: subs     r5, r1, #0
003d4ee0: mov      r6, r0
003d4ee4: beq      #0x3d4f84
003d4ee8: ldr      r0, [r6, #4]
003d4eec: bl       #0x3935dc
003d4ef0: mov      r4, r0
003d4ef4: mov      r0, r5
003d4ef8: bl       #0x3935dc
003d4efc: mov      r5, r0
003d4f00: ldr      r1, [r0]
003d4f04: ldr      r0, [r4]
003d4f08: bl       #0x30e3ac
003d4f0c: ldr      r1, [r5, #4]
003d4f10: mov      r8, r0
003d4f14: ldr      r0, [r4, #4]
003d4f18: bl       #0x30e3ac
003d4f1c: ldr      r1, [r5, #8]
003d4f20: mov      r7, r0
003d4f24: ldr      r0, [r4, #8]
003d4f28: bl       #0x30e3ac
003d4f2c: mov      r1, r8
003d4f30: mov      r5, r0
003d4f34: mov      r0, r8
003d4f38: bl       #0x30ed6c
003d4f3c: mov      r1, r7
003d4f40: mov      r4, r0
003d4f44: mov      r0, r7
003d4f48: bl       #0x30ed6c
003d4f4c: mov      r1, r0
003d4f50: mov      r0, r4
003d4f54: bl       #0x30eba4
003d4f58: mov      r1, r5
003d4f5c: mov      r4, r0
003d4f60: mov      r0, r5
003d4f64: bl       #0x30ed6c
003d4f68: mov      r1, r0
003d4f6c: mov      r0, r4
003d4f70: bl       #0x30eba4
003d4f74: mov      r1, r0
003d4f78: mov      r0, r6
003d4f7c: pop      {r4, r5, r6, r7, r8, lr}
003d4f80: b        #0x3d4ea0
003d4f84: ldr      r5, [r0, #0x40]
003d4f88: cmp      r5, #0
003d4f8c: bne      #0x3d4ee8
003d4f90: mov      r0, r5
003d4f94: pop      {r4, r5, r6, r7, r8, pc}

# _ZN10AISMonster14OnEnemySpottedEP9Character
003dd4cc: push     {r4, lr}
003dd4d0: mov      r4, r0
003dd4d4: ldr      r0, [r0, #0x98]
003dd4d8: ldr      r2, [r0, #0x408]
003dd4dc: cmp      r2, #0
003dd4e0: beq      #0x3dd4e8
003dd4e4: pop      {r4, pc}
003dd4e8: add      r0, r0, #0x3c8
003dd4ec: bl       #0x3d6890
003dd4f0: ldr      r3, [r4, #0x98]
003dd4f4: mov      r2, #1
003dd4f8: strb     r2, [r3, #0x412]
003dd4fc: pop      {r4, pc}
