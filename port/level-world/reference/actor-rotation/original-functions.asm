
# _ZN11Application5GetDtEv
0031f66c: ldr      r0, [r0, #0x8c]
0031f670: bx       lr

# _ZNK9Character28IsUpdatingVisualWithRotationEv
003a2e68: ldr      r0, [r0, #0x520]
003a2e6c: eor      r0, r0, #0x10
003a2e70: ubfx     r0, r0, #4, #1
003a2e74: bx       lr

# _ZN10GameObject14UpdateRotationEv
00393710: push     {r4, r5, r6, r7, r8, lr}
00393714: ldr      r3, [r0]
00393718: mov      r4, r0
0039371c: mov      lr, pc
00393720: ldr      pc, [r3, #0xac]
00393724: mov      r1, #0
00393728: mov      r5, r0
0039372c: bl       #0x30e70c
00393730: ldr      r3, [pc, #0x160]
00393734: cmp      r0, #0
00393738: add      r3, pc, r3
0039373c: beq      #0x39377c
00393740: ldr      r3, [r4, #0x178]
00393744: str      r3, [r4, #0x174]
00393748: ldr      r3, [r4, #0x2d8]
0039374c: cmp      r3, #0
00393750: beq      #0x393778
00393754: ldr      r3, [r4]
00393758: mov      r0, r4
0039375c: mov      lr, pc
00393760: ldr      pc, [r3, #0x70]
00393764: cmp      r0, #0
00393768: beq      #0x393778
0039376c: ldr      r0, [r4, #0x2d8]
00393770: pop      {r4, r5, r6, r7, r8, lr}
00393774: b        #0x472948
00393778: pop      {r4, r5, r6, r7, r8, pc}
0039377c: ldr      r2, [pc, #0x118]
00393780: ldr      r0, [r3, r2]
00393784: bl       #0x31f66c
00393788: movw     r1, #0xfdb
0039378c: mov      r6, r0
00393790: movt     r1, #0x4149
00393794: mov      r0, r5
00393798: bl       #0x30ed6c
0039379c: mov      r5, r0
003937a0: mov      r0, r6
003937a4: bl       #0x30e2e0
003937a8: movw     r1, #0x126f
003937ac: movt     r1, #0x3a83
003937b0: bl       #0x30ed6c
003937b4: mov      r1, r0
003937b8: mov      r0, r5
003937bc: bl       #0x30ed6c
003937c0: ldr      r6, [r4, #0x178]
003937c4: ldr      r7, [r4, #0x174]
003937c8: mov      r8, r0
003937cc: mov      r0, r6
003937d0: mov      r1, r7
003937d4: bl       #0x30e3ac
003937d8: movw     r1, #0xfdb
003937dc: movt     r1, #0x4049
003937e0: mov      r5, r0
003937e4: bl       #0x30e2f8
003937e8: cmp      r0, #0
003937ec: bne      #0x393864
003937f0: movw     r1, #0xfdb
003937f4: mov      r0, r5
003937f8: movt     r1, #0xc049
003937fc: bl       #0x30e70c
00393800: cmp      r0, #0
00393804: beq      #0x39381c
00393808: movw     r1, #0xfdb
0039380c: mov      r0, r5
00393810: movt     r1, #0x40c9
00393814: bl       #0x30eba4
00393818: mov      r5, r0
0039381c: bic      r0, r5, #0x80000000
00393820: mov      r1, r8
00393824: bl       #0x30e70c
00393828: cmp      r0, #0
0039382c: strne    r6, [r4, #0x174]
00393830: bne      #0x393748
00393834: mov      r0, r5
00393838: mov      r1, #0
0039383c: bl       #0x30e70c
00393840: cmp      r0, #0
00393844: beq      #0x39387c
00393848: mov      r0, r7
0039384c: mov      r1, r8
00393850: bl       #0x30e3ac
00393854: mov      r3, #0
00393858: str      r0, [r4, #0x174]
0039385c: str      r3, [r4, #0x17c]
00393860: b        #0x393748
00393864: movw     r1, #0xfdb
00393868: mov      r0, r5
0039386c: movt     r1, #0x40c9
00393870: bl       #0x30e3ac
00393874: mov      r5, r0
00393878: b        #0x39381c
0039387c: mov      r0, r8
00393880: mov      r1, r7
00393884: bl       #0x30eba4
00393888: mov      r3, #1
0039388c: str      r0, [r4, #0x174]
00393890: str      r3, [r4, #0x17c]
00393894: b        #0x393748
00393898: rsbeq    r1, r0, r8, asr r3
0039389c: strdeq   r3, r4, [r0], -r4

# _ZNK14CharProperties22PROPS_GetRotationSpeedEv
003de708: push     {r4, lr}
003de70c: ldr      r0, [r0, #0xb54]
003de710: bl       #0x30e964
003de714: mov      r1, #0x3b800000
003de718: bl       #0x30ed6c
003de71c: movw     r1, #0xd70a
003de720: movt     r1, #0x3c23
003de724: bl       #0x30ed6c
003de728: mov      r1, #0x3f800000
003de72c: bl       #0x30eba4
003de730: mov      r1, #0
003de734: mov      r4, r0
003de738: bl       #0x30e2f8
003de73c: cmp      r0, #0
003de740: moveq    r4, #0
003de744: mov      r0, r4
003de748: pop      {r4, pc}

# _ZNK9Character16GetRotationSpeedEv
003a372c: ldr      r3, [r0, #0x520]
003a3730: tst      r3, #0x20
003a3734: beq      #0x3a3744
003a3738: mov      r0, #0xbf000000
003a373c: add      r0, r0, #0x800000
003a3740: bx       lr
003a3744: add      r0, r0, #0x560
003a3748: b        #0x3de708
