
# _ZN10AISDefault11OnEndOfAnimEv
003dbeec: bx       lr

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

# _ZN12CharAnimator12ANIM_SetStepEjj
003c946c: ldrb     r3, [r0, #0x48]
003c9470: cmp      r3, #0
003c9474: moveq    r3, #0xc
003c9478: mlaeq    r0, r3, r2, r0
003c947c: streq    r1, [r0, #0x10]
003c9480: bx       lr

# _ZN6CharAI11OnPreAttackEi
003d0ed4: push     {r4, r5, r6, lr}
003d0ed8: mov      r4, r0
003d0edc: ldr      r0, [r0, #4]
003d0ee0: mov      r5, r1
003d0ee4: mov      r1, #0
003d0ee8: add      r0, r0, #0x3c8
003d0eec: bl       #0x3d67f4
003d0ef0: cmp      r0, #0
003d0ef4: beq      #0x3d0f18
003d0ef8: ldr      r3, [r4, #0x1c]
003d0efc: cmp      r3, #0
003d0f00: beq      #0x3d0f18
003d0f04: mov      r0, r3
003d0f08: mov      r1, r5
003d0f0c: ldr      r3, [r3]
003d0f10: mov      lr, pc
003d0f14: ldr      pc, [r3, #0xa4]
003d0f18: pop      {r4, r5, r6, pc}

# _ZNK6CharAI17AI_GetMeleeRadiusEv
003d4c34: push     {r4, r5, lr}
003d4c38: mov      r5, r0
003d4c3c: ldr      r0, [r0, #4]
003d4c40: sub      sp, sp, #0xc
003d4c44: add      r1, sp, #8
003d4c48: mov      r3, #0
003d4c4c: str      r3, [r1, #-4]!
003d4c50: add      r0, r0, #0x37c
003d4c54: ldr      r4, [pc, #0x38]
003d4c58: bl       #0x3fff30
003d4c5c: ldr      r3, [pc, #0x34]
003d4c60: add      r4, pc, r4
003d4c64: ldr      r0, [r5, #4]
003d4c68: ldr      r3, [r4, r3]
003d4c6c: ldr      r4, [r3]
003d4c70: bl       #0x3a2fec
003d4c74: mov      r3, #0x44
003d4c78: mla      r4, r3, r0, r4
003d4c7c: ldr      r0, [sp, #4]
003d4c80: bl       #0x30e964
003d4c84: ldr      r1, [r4, #0x20]
003d4c88: bl       #0x30eba4
003d4c8c: add      sp, sp, #0xc
003d4c90: pop      {r4, r5, pc}
003d4c94: subseq   pc, fp, r0, lsr lr
003d4c98: andeq    r0, r0, r8, asr r7

# _ZN12CharAnimator17ANIM_SkipNextStepEj
003c9444: ldrb     r3, [r0, #0x48]
003c9448: cmp      r3, #0
003c944c: moveq    r3, #0xc
003c9450: mlaeq    r0, r3, r1, r0
003c9454: ldreq    r3, [r0, #0x10]
003c9458: addeq    r3, r3, #1
003c945c: streq    r3, [r0, #0x10]
003c9460: bx       lr

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

# _ZN11AISExternal11OnEndOfAnimEv
003dccd0: push     {r4, lr}
003dccd4: mov      r4, r0
003dccd8: bl       #0x3dbeec
003dccdc: ldr      r1, [pc, #0xc]
003dcce0: mov      r0, r4
003dcce4: add      r1, pc, r1
003dcce8: pop      {r4, lr}
003dccec: b        #0x37c514
003dccf0: subeq    r8, lr, r4, asr #25

# _ZN6CharAI11OnEndOfAnimEv
003d0ce8: push     {r4, lr}
003d0cec: ldr      r3, [r0, #0x1c]
003d0cf0: cmp      r3, #0
003d0cf4: beq      #0x3d0d08
003d0cf8: mov      r0, r3
003d0cfc: ldr      r3, [r3]
003d0d00: mov      lr, pc
003d0d04: ldr      pc, [r3, #0x98]
003d0d08: pop      {r4, pc}

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
