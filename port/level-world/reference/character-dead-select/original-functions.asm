
# _ZN16CharStateMachine15SM_SetDeadStateEbPvb
003c58c8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c58cc: mov      r4, r0
003c58d0: sub      sp, sp, #4
003c58d4: ldr      r0, [r0, #4]
003c58d8: mov      r5, r1
003c58dc: mov      sb, r2
003c58e0: mov      r7, r3
003c58e4: bl       #0x3a3228
003c58e8: ldr      r6, [pc, #0x190]
003c58ec: cmp      r0, #0
003c58f0: add      r6, pc, r6
003c58f4: blt      #0x3c59d8
003c58f8: ldr      r3, [pc, #0x184]
003c58fc: ldr      r3, [r6, r3]
003c5900: ldr      r3, [r3]
003c5904: cmp      r0, r3
003c5908: bge      #0x3c59d8
003c590c: ldr      r3, [pc, #0x174]
003c5910: ldrb     r2, [r4, #0x3f]
003c5914: mov      r8, #0xa0
003c5918: ldr      r3, [r6, r3]
003c591c: cmp      r2, #0
003c5920: ldr      r3, [r3]
003c5924: mla      r8, r8, r0, r3
003c5928: beq      #0x3c5a10
003c592c: ldr      sl, [pc, #0x158]
003c5930: ldr      r1, [pc, #0x158]
003c5934: ldr      r2, [pc, #0x158]
003c5938: ldr      r3, [r6, sl]
003c593c: add      r1, pc, r1
003c5940: add      r2, pc, r2
003c5944: ldr      r0, [r3, #0x2c]
003c5948: ldr      fp, [r8, #0x10]
003c594c: bl       #0x4c4bdc
003c5950: ands     r0, r0, #0x20000
003c5954: bne      #0x3c5a3c
003c5958: ldrb     r3, [r4, #0x3f]
003c595c: add      fp, r0, fp
003c5960: str      fp, [r4, #0x28]
003c5964: cmp      r3, #0
003c5968: beq      #0x3c59e0
003c596c: ldr      r3, [r6, sl]
003c5970: ldr      r1, [pc, #0x120]
003c5974: ldr      r2, [pc, #0x120]
003c5978: ldr      r0, [r3, #0x2c]
003c597c: add      r1, pc, r1
003c5980: add      r2, pc, r2
003c5984: ldr      r8, [r8, #0x18]
003c5988: bl       #0x4c4bdc
003c598c: ands     r0, r0, #0x40000
003c5990: bne      #0x3c5a58
003c5994: add      r6, r0, r8
003c5998: mov      sl, #0
003c599c: str      r6, [r4, #0x38]
003c59a0: strb     r5, [r4, #0x3e]
003c59a4: strb     sl, [r4, #0x3f]
003c59a8: mov      r0, r4
003c59ac: bl       #0x3c01ec
003c59b0: cmp      r0, sl
003c59b4: strne    sl, [r4, #0x20]
003c59b8: cmp      r7, #0
003c59bc: bne      #0x3c5a64
003c59c0: mov      r0, r4
003c59c4: mov      r2, sb
003c59c8: movw     r1, #0xc358
003c59cc: add      sp, sp, #4
003c59d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c59d4: b        #0x3c5684
003c59d8: add      sp, sp, #4
003c59dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c59e0: ldr      r3, [r6, sl]
003c59e4: ldr      r1, [pc, #0xb4]
003c59e8: ldr      r2, [pc, #0xb4]
003c59ec: ldr      r0, [r3, #0x2c]
003c59f0: add      r1, pc, r1
003c59f4: add      r2, pc, r2
003c59f8: ldr      r6, [r8, #0x14]
003c59fc: bl       #0x4c4bdc
003c5a00: ands     r0, r0, #0x10000
003c5a04: bne      #0x3c5a48
003c5a08: add      r6, r0, r6
003c5a0c: b        #0x3c5998
003c5a10: ldr      sl, [pc, #0x74]
003c5a14: ldr      r1, [pc, #0x8c]
003c5a18: ldr      r2, [pc, #0x8c]
003c5a1c: ldr      r3, [r6, sl]
003c5a20: add      r1, pc, r1
003c5a24: add      r2, pc, r2
003c5a28: ldr      r0, [r3, #0x2c]
003c5a2c: ldr      fp, [r8, #0x1c]
003c5a30: bl       #0x4c4bdc
003c5a34: ands     r0, r0, #0x8000
003c5a38: beq      #0x3c5958
003c5a3c: ldr      r0, [r4, #4]
003c5a40: bl       #0x3a53e0
003c5a44: b        #0x3c5958
003c5a48: ldr      r0, [r4, #4]
003c5a4c: bl       #0x3a53e0
003c5a50: add      r6, r0, r6
003c5a54: b        #0x3c5998
003c5a58: ldr      r0, [r4, #4]
003c5a5c: bl       #0x3a53e0
003c5a60: b        #0x3c5994
003c5a64: mov      r0, r4
003c5a68: mov      r3, sb
003c5a6c: mov      r1, #0xc
003c5a70: movw     r2, #0xc358
003c5a74: add      sp, sp, #4
003c5a78: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c5a7c: b        #0x3c1938
003c5a80: subseq   pc, ip, r0, lsr #3
003c5a84: andeq    r2, r0, r0, asr #17
003c5a88: andeq    r4, r0, r4, asr #16
003c5a8c: strdeq   r3, r4, [r0], -r4
003c5a90: subeq    pc, pc, ip, ror r2
003c5a94: subeq    pc, pc, r8, lsl #5
003c5a98: subeq    pc, pc, ip, lsr r2
003c5a9c: subeq    pc, pc, r8, asr #4
003c5aa0: subeq    pc, pc, r8, asr #3

# _ZN16CharStateMachine9_SetStateEiiPv
003c1938: push     {r4, r5, r6, r7, r8, sl, lr}
003c193c: ldr      ip, [r0, #0x20]
003c1940: sub      sp, sp, #0x14
003c1944: mov      r4, r0
003c1948: cmp      ip, #0
003c194c: mvneq    r7, #0
003c1950: mov      r5, r1
003c1954: mov      r8, r2
003c1958: mov      sl, r3
003c195c: moveq    r6, r7
003c1960: beq      #0x3c1990
003c1964: ldr      r3, [ip, #4]
003c1968: ldr      r6, [ip]
003c196c: ldr      r2, [r0, #4]
003c1970: ldr      ip, [r3]
003c1974: mov      r0, r3
003c1978: str      r1, [sp]
003c197c: mov      r3, r4
003c1980: mov      r1, r6
003c1984: mov      lr, pc
003c1988: ldr      pc, [ip, #0x10]
003c198c: mov      r7, r6
003c1990: mov      r0, r4
003c1994: mov      r1, r5
003c1998: bl       #0x3c0084
003c199c: cmp      r0, #0
003c19a0: streq    r0, [r4, #0x20]
003c19a4: bne      #0x3c19c0
003c19a8: ldr      r0, [r4, #4]
003c19ac: mov      r2, r7
003c19b0: mov      r1, #0x1d
003c19b4: add      sp, sp, #0x14
003c19b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003c19bc: b        #0x3a4d5c
003c19c0: mov      r1, r5
003c19c4: mov      r0, r4
003c19c8: bl       #0x3c184c
003c19cc: cmp      r6, r5
003c19d0: movne    r3, #0
003c19d4: str      r0, [r4, #0x20]
003c19d8: strne    r3, [r4, #0x60]
003c19dc: ldm      r0, {r1, r3}
003c19e0: ldr      r2, [r4, #4]
003c19e4: ldr      ip, [r3]
003c19e8: mov      r0, r3
003c19ec: stm      sp, {r6, r8, sl}
003c19f0: mov      r3, r4
003c19f4: mov      lr, pc
003c19f8: ldr      pc, [ip, #0xc]
003c19fc: b        #0x3c19a8

# _ZNK16CharStateMachine21SM_IsAwaitingToReviveEv
003c01ec: push     {r4, lr}
003c01f0: mov      r4, r0
003c01f4: bl       #0x3c01ac
003c01f8: cmp      r0, #0
003c01fc: bne      #0x3c0208
003c0200: mov      r0, #1
003c0204: pop      {r4, pc}
003c0208: mov      r0, r4
003c020c: bl       #0x3c01ac
003c0210: cmp      r0, #0x11
003c0214: beq      #0x3c0200
003c0218: mov      r0, r4
003c021c: bl       #0x3c01ac
003c0220: cmp      r0, #0x10
003c0224: movne    r0, #0
003c0228: moveq    r0, #1
003c022c: pop      {r4, pc}

# _ZN16CharStateMachine15RaiseStateEventEiPv
003c5684: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c5688: ldr      r5, [pc, #0x1e0]
003c568c: ldr      r7, [pc, #0x1e0]
003c5690: mov      r6, r1
003c5694: add      r5, pc, r5
003c5698: ldr      r1, [r5, r7]
003c569c: sub      sp, sp, #0x30
003c56a0: sub      r3, r6, #0x2a
003c56a4: ldr      r1, [r1]
003c56a8: mov      r4, r0
003c56ac: mov      r8, r2
003c56b0: str      r1, [sp, #0x2c]
003c56b4: cmp      r3, #6
003c56b8: addls    pc, pc, r3, lsl #2
003c56bc: b        #0x3c5700
003c56c0: b        #0x3c584c
003c56c4: b        #0x3c583c
003c56c8: b        #0x3c582c
003c56cc: b        #0x3c5700
003c56d0: b        #0x3c5700
003c56d4: b        #0x3c5700
003c56d8: b        #0x3c56dc
003c56dc: mov      r1, #0
003c56e0: bl       #0x3c0260
003c56e4: cmp      r0, #0
003c56e8: beq      #0x3c5700
003c56ec: ldr      r3, [r4, #4]
003c56f0: ldr      r0, [r3, #0x2dc]
003c56f4: cmp      r0, #0
003c56f8: beq      #0x3c5700
003c56fc: bl       #0x46eb20
003c5700: ldr      r3, [r4, #0x20]
003c5704: cmp      r3, #0
003c5708: beq      #0x3c574c
003c570c: ldm      r3, {r1, r3}
003c5710: ldr      r2, [r4, #4]
003c5714: ldr      ip, [r3]
003c5718: mov      r0, r3
003c571c: str      r6, [sp]
003c5720: mov      r3, r4
003c5724: str      r8, [sp, #4]
003c5728: mov      lr, pc
003c572c: ldr      pc, [ip, #0x18]
003c5730: ldr      r3, [r4, #0x20]
003c5734: mov      r0, r4
003c5738: mov      r2, r6
003c573c: ldr      r1, [r3]
003c5740: bl       #0x3c00e0
003c5744: cmp      r0, #0
003c5748: bne      #0x3c5768
003c574c: ldr      r3, [r5, r7]
003c5750: ldr      r2, [sp, #0x2c]
003c5754: ldr      r3, [r3]
003c5758: cmp      r2, r3
003c575c: bne      #0x3c586c
003c5760: add      sp, sp, #0x30
003c5764: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c5768: ldr      r3, [pc, #0x108]
003c576c: add      sl, sp, #0x14
003c5770: ldr      sb, [r5, r3]
003c5774: mov      r0, sb
003c5778: bl       #0x337888
003c577c: ldr      r1, [pc, #0xf8]
003c5780: add      r2, sp, #0x10
003c5784: mov      r0, sl
003c5788: add      r1, pc, r1
003c578c: bl       #0x3140ec
003c5790: mov      r1, sl
003c5794: mov      r0, sb
003c5798: bl       #0x337a88
003c579c: mov      r0, sl
003c57a0: bl       #0x318254
003c57a4: ldr      r3, [r4, #0x20]
003c57a8: mov      r0, r4
003c57ac: mov      r2, r6
003c57b0: ldr      r1, [r3]
003c57b4: bl       #0x3c1694
003c57b8: ldr      r1, [r0, #8]
003c57bc: str      r1, [sp, #0xc]
003c57c0: ldr      r3, [r0]
003c57c4: cmp      r3, #0
003c57c8: beq      #0x3c585c
003c57cc: ldr      r3, [r0, #4]
003c57d0: ldr      r2, [r4, #4]
003c57d4: tst      r3, #1
003c57d8: ldrne    r1, [r0]
003c57dc: ldrne    ip, [r2, r3, asr #1]
003c57e0: addne    r0, r2, r3, asr #1
003c57e4: ldreq    ip, [r0]
003c57e8: addeq    r0, r2, r3, asr #1
003c57ec: ldr      r3, [r4, #0x20]
003c57f0: add      r2, sp, #0xc
003c57f4: ldrne    ip, [ip, r1]
003c57f8: ldr      r3, [r3]
003c57fc: mov      r1, r6
003c5800: str      r2, [sp]
003c5804: mov      r2, r8
003c5808: blx      ip
003c580c: cmp      r0, #0
003c5810: beq      #0x3c574c
003c5814: ldr      r1, [sp, #0xc]
003c5818: mov      r0, r4
003c581c: mov      r2, r6
003c5820: mov      r3, r8
003c5824: bl       #0x3c1938
003c5828: b        #0x3c574c
003c582c: ldr      r3, [r0, #0x2c]
003c5830: bic      r3, r3, #4
003c5834: str      r3, [r0, #0x2c]
003c5838: b        #0x3c5700
003c583c: ldr      r3, [r0, #0x2c]
003c5840: bic      r3, r3, #2
003c5844: str      r3, [r0, #0x2c]
003c5848: b        #0x3c5700
003c584c: ldr      r3, [r0, #0x2c]
003c5850: bic      r3, r3, #1
003c5854: str      r3, [r0, #0x2c]
003c5858: b        #0x3c5700
003c585c: ldr      r3, [r0, #4]
003c5860: tst      r3, #1
003c5864: beq      #0x3c5818
003c5868: b        #0x3c57cc
003c586c: bl       #0x30e310
003c5870: ldrsheq  pc, [ip], #-0x3c
003c5874: andeq    r4, r0, ip, lsr #1
003c5878: andeq    r0, r0, r4, lsl #17
003c587c: subeq    pc, pc, r8, lsr #15

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZNK9Character13GetAnimStanceEv
003a53e0: push     {r4, r5, r6, lr}
003a53e4: ldr      r3, [r0]
003a53e8: mov      r4, r0
003a53ec: mov      lr, pc
003a53f0: ldr      pc, [r3, #0x28]
003a53f4: ldr      r5, [pc, #0xa8]
003a53f8: cmp      r0, #0
003a53fc: add      r5, pc, r5
003a5400: bne      #0x3a5438
003a5404: mov      r4, #0
003a5408: ldr      r3, [pc, #0x98]
003a540c: ldr      r1, [pc, #0x98]
003a5410: ldr      r2, [pc, #0x98]
003a5414: ldr      r3, [r5, r3]
003a5418: add      r1, pc, r1
003a541c: add      r2, pc, r2
003a5420: ldr      r0, [r3, #0x2c]
003a5424: bl       #0x4c4bdc
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404

# _ZNK9Character18GetCharAnimTableIdEv
003a3228: mov      r3, #0x1000
003a322c: ldr      r0, [r0, r3]
003a3230: ldr      r3, [pc, #0x24]
003a3234: cmp      r0, #0
003a3238: add      r3, pc, r3
003a323c: blt      #0x3a3254
003a3240: ldr      r2, [pc, #0x18]
003a3244: ldr      r3, [r3, r2]
003a3248: ldr      r3, [r3]
003a324c: cmp      r0, r3
003a3250: bxlt     lr
003a3254: mov      r0, #0x11
003a3258: bx       lr
003a325c: subseq   r1, pc, r8, asr r8
003a3260: andeq    r2, r0, r0, asr #17
