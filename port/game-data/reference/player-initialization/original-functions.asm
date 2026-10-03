
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

# _ZN9Character18SafeGetCharPropsIdEv
003b3d38: push     {r4, r5, r6, r7, r8, sl, lr}
003b3d3c: movw     r6, #0x13c8
003b3d40: ldrsh    r3, [r0, r6]
003b3d44: ldr      r7, [pc, #0x294]
003b3d48: mov      r4, r0
003b3d4c: cmn      r3, #1
003b3d50: add      r7, pc, r7
003b3d54: sub      sp, sp, #0xc
003b3d58: movne    r0, r3
003b3d5c: beq      #0x3b3d68
003b3d60: add      sp, sp, #0xc
003b3d64: pop      {r4, r5, r6, r7, r8, sl, pc}
003b3d68: ldr      r3, [r4]
003b3d6c: mov      lr, pc
003b3d70: ldr      pc, [r3, #0x28]
003b3d74: subs     sl, r0, #0
003b3d78: bne      #0x3b3dcc
003b3d7c: movw     r3, #0x13a8
003b3d80: ldr      r2, [r4, r3]
003b3d84: movw     r3, #0x13ac
003b3d88: ldr      r3, [r4, r3]
003b3d8c: cmp      r2, r3
003b3d90: beq      #0x3b3f20
003b3d94: mov      r0, r4
003b3d98: bl       #0x3b36ec
003b3d9c: cmp      r0, #0
003b3da0: blt      #0x3b3dc4
003b3da4: ldr      r3, [pc, #0x238]
003b3da8: mov      r5, #0xc
003b3dac: ldr      r3, [r7, r3]
003b3db0: ldr      r3, [r3]
003b3db4: mla      r5, r5, r0, r3
003b3db8: ldr      r1, [r5, #4]
003b3dbc: cmp      r1, #0
003b3dc0: bne      #0x3b3e08
003b3dc4: ldrsh    r0, [r4, r6]
003b3dc8: b        #0x3b3d60
003b3dcc: mov      r1, #1
003b3dd0: mov      r0, r4
003b3dd4: bl       #0x3bc4d0
003b3dd8: mov      r0, r4
003b3ddc: bl       #0x3bb7fc
003b3de0: uxth     r0, r0
003b3de4: sxth     r1, r0
003b3de8: cmn      r1, #1
003b3dec: strh     r0, [r4, r6]
003b3df0: beq      #0x3b3ebc
003b3df4: mov      r0, r4
003b3df8: bl       #0x3bb814
003b3dfc: movw     r3, #0x13c8
003b3e00: ldrsh    r0, [r4, r3]
003b3e04: b        #0x3b3d60
003b3e08: ldr      r2, [pc, #0x1d8]
003b3e0c: movw     lr, #0xe6ab
003b3e10: movw     r3, #0xdb17
003b3e14: ldr      r2, [r7, r2]
003b3e18: movt     r3, #0x2b52
003b3e1c: movw     ip, #0xf26b
003b3e20: ldr      r0, [r2]
003b3e24: movt     ip, #0xda
003b3e28: mul      r0, lr, r0
003b3e2c: add      r0, r0, #0x2b000
003b3e30: add      r0, r0, #0x3fc
003b3e34: add      r0, r0, #1
003b3e38: umull    lr, r3, r3, r0
003b3e3c: rsb      lr, r3, r0
003b3e40: add      r3, r3, lr, lsr #1
003b3e44: lsr      r3, r3, #0x17
003b3e48: mls      r3, ip, r3, r0
003b3e4c: str      r3, [r2]
003b3e50: mov      r0, r3
003b3e54: bl       #0x30eb2c
003b3e58: ldr      r3, [pc, #0x18c]
003b3e5c: eor      r6, r1, r1, asr #31
003b3e60: sub      r6, r6, r1, asr #31
003b3e64: ldr      r3, [r7, r3]
003b3e68: ldr      r2, [r3]
003b3e6c: add      r2, r2, #1
003b3e70: str      r2, [r3]
003b3e74: ldr      r3, [r5, #4]
003b3e78: cmp      r3, r6
003b3e7c: bgt      #0x3b3ea0
003b3e80: ldr      r3, [pc, #0x168]
003b3e84: ldr      r3, [r7, r3]
003b3e88: ldr      r3, [r3]
003b3e8c: cmp      r3, #2
003b3e90: streq    sl, [sl]
003b3e94: beq      #0x3b3ea0
003b3e98: cmp      r3, #1
003b3e9c: beq      #0x3b3fac
003b3ea0: ldr      r3, [r5, #8]
003b3ea4: add      r6, r3, r6, lsl #3
003b3ea8: ldrh     r0, [r6, #4]
003b3eac: movw     r3, #0x13c8
003b3eb0: strh     r0, [r4, r3]
003b3eb4: sxth     r0, r0
003b3eb8: b        #0x3b3d60
003b3ebc: ldr      r3, [pc, #0x130]
003b3ec0: ldr      r3, [r7, r3]
003b3ec4: ldr      r6, [r3]
003b3ec8: cmp      r6, #0
003b3ecc: beq      #0x3b3fa0
003b3ed0: ldr      r3, [pc, #0x120]
003b3ed4: ldr      r8, [pc, #0x120]
003b3ed8: mov      r5, #0
003b3edc: ldr      r3, [r7, r3]
003b3ee0: add      r8, pc, r8
003b3ee4: ldr      r7, [r3]
003b3ee8: b        #0x3b3ef8
003b3eec: add      r5, r5, #1
003b3ef0: cmp      r5, r6
003b3ef4: beq      #0x3b3fa0
003b3ef8: ldr      r1, [r7, r5, lsl #2]
003b3efc: mov      r0, r8
003b3f00: bl       #0x30e31c
003b3f04: cmp      r0, #0
003b3f08: bne      #0x3b3eec
003b3f0c: uxth     r5, r5
003b3f10: sxth     r1, r5
003b3f14: movw     r3, #0x13c8
003b3f18: strh     r5, [r4, r3]
003b3f1c: b        #0x3b3df4
003b3f20: movw     r3, #0x13c4
003b3f24: ldr      r8, [r4, r3]
003b3f28: mov      r3, #0x13c0
003b3f2c: ldr      r3, [r4, r3]
003b3f30: cmp      r3, r8
003b3f34: beq      #0x3b3dc4
003b3f38: ldr      r3, [pc, #0xb4]
003b3f3c: ldr      r3, [r7, r3]
003b3f40: ldr      r6, [r3]
003b3f44: cmp      r6, #0
003b3f48: beq      #0x3b3f94
003b3f4c: ldr      r3, [pc, #0xa4]
003b3f50: mov      r5, sl
003b3f54: ldr      r3, [r7, r3]
003b3f58: ldr      r7, [r3]
003b3f5c: b        #0x3b3f6c
003b3f60: add      r5, r5, #1
003b3f64: cmp      r5, r6
003b3f68: beq      #0x3b3f94
003b3f6c: ldr      r1, [r7, r5, lsl #2]
003b3f70: mov      r0, r8
003b3f74: bl       #0x30e31c
003b3f78: cmp      r0, #0
003b3f7c: bne      #0x3b3f60
003b3f80: uxth     r5, r5
003b3f84: sxth     r0, r5
003b3f88: movw     r3, #0x13c8
003b3f8c: strh     r5, [r4, r3]
003b3f90: b        #0x3b3d60
003b3f94: mvn      r0, #0
003b3f98: movw     r5, #0xffff
003b3f9c: b        #0x3b3f88
003b3fa0: mvn      r1, #0
003b3fa4: movw     r5, #0xffff
003b3fa8: b        #0x3b3f14
003b3fac: ldr      r0, [pc, #0x4c]
003b3fb0: ldr      r1, [pc, #0x4c]
003b3fb4: ldr      r2, [pc, #0x4c]
003b3fb8: ldr      r0, [r7, r0]
003b3fbc: ldr      r3, [pc, #0x48]
003b3fc0: mov      ip, #0x2f4
003b3fc4: add      r1, pc, r1
003b3fc8: add      r2, pc, r2
003b3fcc: add      r3, pc, r3
003b3fd0: add      r0, r0, #0xa8
003b3fd4: str      ip, [sp]
003b3fd8: bl       #0x30e004
003b3fdc: b        #0x3b3ea0
003b3fe0: subseq   r0, lr, r0, asr #26
003b3fe4: strheq   r4, [r0], -r8
003b3fe8: muleq    r0, r4, ip
003b3fec: andeq    r1, r0, r8, lsl #1
003b3ff0: andeq    r3, r0, r0, asr #19
003b3ff4: andeq    r4, r0, r4, lsl #4
003b3ff8: andeq    r3, r0, r8, lsl #24
003b3ffc: subseq   pc, r0, r0, lsl #29
003b4000: andeq    r1, r0, r0, asr #19
003b4004: subseq   sl, r0, r4, lsl r4
003b4008: ldrheq   pc, [r0], #-0xd0
003b400c: subseq   pc, r0, r4, ror #27

# _ZN14PlayerSavegame7SG_LoadEi
00465430: push     {r4, r5, r6, lr}
00465434: mov      r5, r0
00465438: mov      r4, r1
0046543c: bl       #0x464f4c
00465440: mov      r0, r5
00465444: mov      r1, r4
00465448: pop      {r4, r5, r6, lr}
0046544c: b        #0x468574

# _ZN14PlayerSavegame19_SetupSavedSectionsEbb
00468630: push     {r4, r5, r6, lr}
00468634: ldr      r4, [pc, #0x210]
00468638: cmp      r1, #0
0046863c: sub      sp, sp, #8
00468640: mov      r5, r0
00468644: add      r4, pc, r4
00468648: mov      r6, r2
0046864c: beq      #0x468660
00468650: cmp      r2, #0
00468654: beq      #0x468768
00468658: add      sp, sp, #8
0046865c: pop      {r4, r5, r6, pc}
00468660: cmp      r2, #0
00468664: bne      #0x468658
00468668: ldr      r3, [pc, #0x1e0]
0046866c: ldr      r1, [pc, #0x1e0]
00468670: ldr      r0, [r0, #8]
00468674: ldr      r2, [r4, r3]
00468678: ldr      r3, [pc, #0x1d8]
0046867c: add      r1, pc, r1
00468680: str      r5, [sp]
00468684: ldr      r3, [r4, r3]
00468688: bl       #0x315904
0046868c: ldr      r3, [pc, #0x1c8]
00468690: ldr      r1, [pc, #0x1c8]
00468694: ldr      r0, [r5, #8]
00468698: ldr      r2, [r4, r3]
0046869c: ldr      r3, [pc, #0x1c0]
004686a0: add      r1, pc, r1
004686a4: str      r5, [sp]
004686a8: ldr      r3, [r4, r3]
004686ac: bl       #0x315904
004686b0: ldr      r3, [pc, #0x1b0]
004686b4: ldr      r1, [pc, #0x1b0]
004686b8: ldr      r0, [r5, #8]
004686bc: ldr      r2, [r4, r3]
004686c0: ldr      r3, [pc, #0x1a8]
004686c4: add      r1, pc, r1
004686c8: str      r5, [sp]
004686cc: ldr      r3, [r4, r3]
004686d0: bl       #0x315904
004686d4: ldr      r3, [pc, #0x198]
004686d8: ldr      r1, [pc, #0x198]
004686dc: ldr      r0, [r5, #8]
004686e0: ldr      r2, [r4, r3]
004686e4: ldr      r3, [pc, #0x190]
004686e8: add      r1, pc, r1
004686ec: str      r5, [sp]
004686f0: ldr      r3, [r4, r3]
004686f4: bl       #0x315904
004686f8: ldr      r3, [pc, #0x180]
004686fc: ldr      r1, [pc, #0x180]
00468700: ldr      r0, [r5, #8]
00468704: ldr      r2, [r4, r3]
00468708: ldr      r3, [pc, #0x178]
0046870c: add      r1, pc, r1
00468710: str      r5, [sp]
00468714: ldr      r3, [r4, r3]
00468718: bl       #0x315904
0046871c: ldr      r3, [pc, #0x168]
00468720: ldr      r1, [pc, #0x168]
00468724: ldr      r0, [r5, #8]
00468728: ldr      r2, [r4, r3]
0046872c: ldr      r3, [pc, #0x160]
00468730: add      r1, pc, r1
00468734: str      r5, [sp]
00468738: ldr      r3, [r4, r3]
0046873c: bl       #0x315904
00468740: ldr      r3, [pc, #0x150]
00468744: ldr      r1, [pc, #0x150]
00468748: ldr      r0, [r5, #8]
0046874c: ldr      r2, [r4, r3]
00468750: ldr      r3, [pc, #0x148]
00468754: add      r1, pc, r1
00468758: str      r5, [sp]
0046875c: ldr      r3, [r4, r3]
00468760: bl       #0x315904
00468764: b        #0x468658
00468768: ldr      r3, [pc, #0xe0]
0046876c: ldr      r1, [pc, #0x130]
00468770: ldr      r0, [r0, #8]
00468774: ldr      r2, [r4, r3]
00468778: add      r1, pc, r1
0046877c: mov      r3, r6
00468780: str      r5, [sp]
00468784: bl       #0x315904
00468788: ldr      r3, [pc, #0xcc]
0046878c: ldr      r1, [pc, #0x114]
00468790: ldr      r0, [r5, #8]
00468794: ldr      r2, [r4, r3]
00468798: add      r1, pc, r1
0046879c: mov      r3, r6
004687a0: str      r5, [sp]
004687a4: bl       #0x315904
004687a8: ldr      r3, [pc, #0xb8]
004687ac: ldr      r1, [pc, #0xf8]
004687b0: ldr      r0, [r5, #8]
004687b4: ldr      r2, [r4, r3]
004687b8: add      r1, pc, r1
004687bc: mov      r3, r6
004687c0: str      r5, [sp]
004687c4: bl       #0x315904
004687c8: ldr      r3, [pc, #0xa4]
004687cc: ldr      r1, [pc, #0xdc]
004687d0: ldr      r0, [r5, #8]
004687d4: ldr      r2, [r4, r3]
004687d8: add      r1, pc, r1
004687dc: mov      r3, r6
004687e0: str      r5, [sp]
004687e4: bl       #0x315904
004687e8: ldr      r3, [pc, #0x90]
004687ec: ldr      r1, [pc, #0xc0]
004687f0: ldr      r0, [r5, #8]
004687f4: ldr      r2, [r4, r3]
004687f8: add      r1, pc, r1
004687fc: mov      r3, r6
00468800: str      r5, [sp]
00468804: bl       #0x315904
00468808: ldr      r3, [pc, #0x7c]
0046880c: ldr      r1, [pc, #0xa4]
00468810: ldr      r0, [r5, #8]
00468814: ldr      r2, [r4, r3]
00468818: add      r1, pc, r1
0046881c: mov      r3, r6
00468820: str      r5, [sp]
00468824: bl       #0x315904
00468828: ldr      r3, [pc, #0x68]
0046882c: ldr      r1, [pc, #0x88]
00468830: ldr      r0, [r5, #8]
00468834: ldr      r2, [r4, r3]
00468838: add      r1, pc, r1
0046883c: mov      r3, r6
00468840: str      r5, [sp]
00468844: bl       #0x315904
00468848: b        #0x468658
0046884c: subseq   ip, r2, ip, asr #8
00468850: andeq    r0, r0, r8, lsr #26
00468854: subeq    r4, r6, r4, lsl #23
00468858: strheq   r1, [r0], -r8
0046885c: andeq    r3, r0, r4, lsr #12
00468860: subeq    r4, r6, r8, ror #22
00468864: andeq    r3, r0, r8, ror r7
00468868: andeq    r3, r0, ip, asr #26
0046886c: subeq    r4, r6, ip, asr #22
00468870: strheq   r2, [r0], -r0
00468874: andeq    r4, r0, r0, lsr r0
00468878: subeq    r4, r6, r0, lsr fp
0046887c: andeq    r2, r0, ip, ror #12
00468880: andeq    r3, r0, r0, lsl #30
00468884: subeq    r4, r6, r4, lsl fp
00468888: andeq    r4, r0, ip, ror r6
0046888c: andeq    r1, r0, ip, lsr r3
00468890: subeq    r4, r6, r0, lsl #22
00468894: andeq    r0, r0, r4, lsr #14
00468898: andeq    r0, r0, ip, lsl #29
0046889c: subeq    r4, r6, r4, lsl #22
004688a0: andeq    r1, r0, r0, ror #24
004688a4: subeq    r4, r6, r8, lsl #21
004688a8: subeq    r4, r6, r0, ror sl
004688ac: subeq    r4, r6, r8, asr sl
004688b0: subeq    r4, r6, r0, asr #20
004688b4: subeq    r4, r6, r8, lsr #20
004688b8: subeq    r4, r6, r8, lsl sl
004688bc: subeq    r4, r6, r0, lsr #20
