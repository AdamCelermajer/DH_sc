
# _ZN6CharAI12OnTargetDiedEv
003d1f60: push     {r4, r5, r6, r7, r8, lr}
003d1f64: ldr      r4, [pc, #0x98]
003d1f68: ldr      r6, [pc, #0x98]
003d1f6c: ldr      r2, [pc, #0x98]
003d1f70: add      r4, pc, r4
003d1f74: ldr      r3, [r4, r6]
003d1f78: ldr      r8, [r4, r2]
003d1f7c: sub      sp, sp, #0x20
003d1f80: ldr      r3, [r3]
003d1f84: mov      r7, r0
003d1f88: mov      r0, r8
003d1f8c: str      r3, [sp, #0x1c]
003d1f90: bl       #0x337888
003d1f94: ldr      r1, [pc, #0x74]
003d1f98: add      r5, sp, #4
003d1f9c: mov      r2, sp
003d1fa0: add      r1, pc, r1
003d1fa4: mov      r0, r5
003d1fa8: bl       #0x3140ec
003d1fac: mov      r1, r5
003d1fb0: mov      r0, r8
003d1fb4: bl       #0x337a88
003d1fb8: mov      r0, r5
003d1fbc: bl       #0x3139ac
003d1fc0: ldr      r3, [r7, #0x1c]
003d1fc4: mov      r2, #0
003d1fc8: strb     r2, [r7, #0x78]
003d1fcc: cmp      r3, r2
003d1fd0: beq      #0x3d1fe4
003d1fd4: mov      r0, r3
003d1fd8: ldr      r3, [r3]
003d1fdc: mov      lr, pc
003d1fe0: ldr      pc, [r3, #0x40]
003d1fe4: ldr      r3, [r4, r6]
003d1fe8: ldr      r2, [sp, #0x1c]
003d1fec: ldr      r3, [r3]
003d1ff0: cmp      r2, r3
003d1ff4: bne      #0x3d2000
003d1ff8: add      sp, sp, #0x20
003d1ffc: pop      {r4, r5, r6, r7, r8, pc}
003d2000: bl       #0x30e310
003d2004: subseq   r2, ip, r0, lsr #22
003d2008: andeq    r4, r0, ip, lsr #1
003d200c: andeq    r0, r0, r4, lsl #17
003d2010: subeq    r3, pc, r8, asr #10

# _ZN6CharAI18OnTargetOutOfSightEv
003d2410: push     {r4, r5, r6, r7, r8, lr}
003d2414: ldr      r4, [pc, #0xcc]
003d2418: ldr      r7, [pc, #0xcc]
003d241c: ldr      r2, [pc, #0xcc]
003d2420: add      r4, pc, r4
003d2424: ldr      r3, [r4, r7]
003d2428: ldr      r8, [r4, r2]
003d242c: sub      sp, sp, #0x20
003d2430: ldr      r3, [r3]
003d2434: mov      r5, r0
003d2438: mov      r0, r8
003d243c: str      r3, [sp, #0x1c]
003d2440: bl       #0x337888
003d2444: ldr      r1, [pc, #0xa8]
003d2448: add      r6, sp, #4
003d244c: mov      r2, sp
003d2450: add      r1, pc, r1
003d2454: mov      r0, r6
003d2458: bl       #0x3140ec
003d245c: mov      r1, r6
003d2460: mov      r0, r8
003d2464: bl       #0x337a88
003d2468: mov      r0, r6
003d246c: bl       #0x3139ac
003d2470: ldr      r0, [r5, #4]
003d2474: add      r0, r0, #0x3c8
003d2478: bl       #0x3d5484
003d247c: cmp      r0, #0
003d2480: bne      #0x3d24c4
003d2484: ldr      r3, [r5, #0x1c]
003d2488: mov      r2, #0
003d248c: strb     r2, [r5, #0x4c]
003d2490: cmp      r3, r2
003d2494: beq      #0x3d24a8
003d2498: mov      r0, r3
003d249c: ldr      r3, [r3]
003d24a0: mov      lr, pc
003d24a4: ldr      pc, [r3, #0x48]
003d24a8: ldr      r3, [r4, r7]
003d24ac: ldr      r2, [sp, #0x1c]
003d24b0: ldr      r3, [r3]
003d24b4: cmp      r2, r3
003d24b8: bne      #0x3d24e4
003d24bc: add      sp, sp, #0x20
003d24c0: pop      {r4, r5, r6, r7, r8, pc}
003d24c4: ldr      r6, [r5, #4]
003d24c8: add      r6, r6, #0x3c8
003d24cc: mov      r0, r6
003d24d0: bl       #0x3d5450
003d24d4: mov      r1, r0
003d24d8: mov      r0, r6
003d24dc: bl       #0x3d6d68
003d24e0: b        #0x3d2484
003d24e4: bl       #0x30e310
003d24e8: subseq   r2, ip, r0, ror r6
003d24ec: andeq    r4, r0, ip, lsr #1
003d24f0: andeq    r0, r0, r4, lsl #17
003d24f4: umaaleq  r3, pc, r8, r0

# _ZN6CharAI15OnTargetInSightEv
003d22e4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d22e8: ldr      r4, [pc, #0x108]
003d22ec: ldr      r6, [pc, #0x108]
003d22f0: ldr      r2, [pc, #0x108]
003d22f4: add      r4, pc, r4
003d22f8: ldr      r3, [r4, r6]
003d22fc: ldr      r8, [r4, r2]
003d2300: sub      sp, sp, #0x40
003d2304: ldr      r3, [r3]
003d2308: mov      r5, r0
003d230c: mov      r0, r8
003d2310: str      r3, [sp, #0x3c]
003d2314: bl       #0x337888
003d2318: ldr      r1, [pc, #0xe4]
003d231c: add      r7, sp, #0x24
003d2320: add      r2, sp, #0x20
003d2324: mov      r0, r7
003d2328: add      r1, pc, r1
003d232c: bl       #0x3140ec
003d2330: mov      r1, r7
003d2334: mov      r0, r8
003d2338: bl       #0x337a88
003d233c: mov      r0, r7
003d2340: bl       #0x3139ac
003d2344: ldr      r3, [pc, #0xbc]
003d2348: ldr      r0, [r5, #4]
003d234c: ldr      r3, [r4, r3]
003d2350: ldr      r7, [r3]
003d2354: bl       #0x3a2fec
003d2358: mov      r3, #0x44
003d235c: mla      r7, r3, r0, r7
003d2360: ldr      r3, [pc, #0xa4]
003d2364: ldr      r0, [r5, #4]
003d2368: ldr      sl, [r7, #0x24]
003d236c: ldr      r3, [r4, r3]
003d2370: ldr      sb, [r3]
003d2374: bl       #0x3935dc
003d2378: ldr      lr, [r0, #4]
003d237c: ldr      r7, [r0]
003d2380: ldr      r8, [r0, #8]
003d2384: mov      ip, #0xbf000000
003d2388: add      ip, ip, #0x800000
003d238c: mov      r3, #0
003d2390: str      lr, [sp, #0x18]
003d2394: mov      r0, sb
003d2398: mov      lr, #1
003d239c: mov      r1, sl
003d23a0: add      r2, sp, #0x14
003d23a4: str      r7, [sp, #0x14]
003d23a8: str      r8, [sp, #0x1c]
003d23ac: str      lr, [sp]
003d23b0: str      ip, [sp, #8]
003d23b4: str      ip, [sp, #4]
003d23b8: bl       #0x36b5d8
003d23bc: ldr      r3, [r5, #0x1c]
003d23c0: cmp      r3, #0
003d23c4: beq      #0x3d23d8
003d23c8: mov      r0, r3
003d23cc: ldr      r3, [r3]
003d23d0: mov      lr, pc
003d23d4: ldr      pc, [r3, #0x4c]
003d23d8: ldr      r3, [r4, r6]
003d23dc: ldr      r2, [sp, #0x3c]
003d23e0: ldr      r3, [r3]
003d23e4: cmp      r2, r3
003d23e8: bne      #0x3d23f4
003d23ec: add      sp, sp, #0x40
003d23f0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d23f4: bl       #0x30e310

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

# _ZN6CharAI21OnTargetInRangedRangeEv
003d1d4c: push     {r4, r5, r6, r7, r8, lr}
003d1d50: ldr      r4, [pc, #0x98]
003d1d54: ldr      r6, [pc, #0x98]
003d1d58: ldr      r2, [pc, #0x98]
003d1d5c: add      r4, pc, r4
003d1d60: ldr      r3, [r4, r6]
003d1d64: ldr      r8, [r4, r2]
003d1d68: sub      sp, sp, #0x20
003d1d6c: ldr      r3, [r3]
003d1d70: mov      r7, r0
003d1d74: mov      r0, r8
003d1d78: str      r3, [sp, #0x1c]
003d1d7c: bl       #0x337888
003d1d80: ldr      r1, [pc, #0x74]
003d1d84: add      r5, sp, #4
003d1d88: mov      r2, sp
003d1d8c: add      r1, pc, r1
003d1d90: mov      r0, r5
003d1d94: bl       #0x3140ec
003d1d98: mov      r1, r5
003d1d9c: mov      r0, r8
003d1da0: bl       #0x337a88
003d1da4: mov      r0, r5
003d1da8: bl       #0x3139ac
003d1dac: ldr      r3, [r7, #0x1c]
003d1db0: mov      r2, #0
003d1db4: strb     r2, [r7, #0x4c]
003d1db8: cmp      r3, r2
003d1dbc: beq      #0x3d1dd0
003d1dc0: mov      r0, r3
003d1dc4: ldr      r3, [r3]
003d1dc8: mov      lr, pc
003d1dcc: ldr      pc, [r3, #0x54]
003d1dd0: ldr      r3, [r4, r6]
003d1dd4: ldr      r2, [sp, #0x1c]
003d1dd8: ldr      r3, [r3]
003d1ddc: cmp      r2, r3
003d1de0: bne      #0x3d1dec
003d1de4: add      sp, sp, #0x20
003d1de8: pop      {r4, r5, r6, r7, r8, pc}
003d1dec: bl       #0x30e310
003d1df0: subseq   r2, ip, r4, lsr sp
003d1df4: andeq    r4, r0, ip, lsr #1
003d1df8: andeq    r0, r0, r4, lsl #17
003d1dfc: subeq    r3, pc, ip, asr r7

# _ZN6CharAI15OnTargetRevivedEv
003d1eb4: push     {r4, r5, r6, r7, r8, lr}
003d1eb8: ldr      r4, [pc, #0x90]
003d1ebc: ldr      r6, [pc, #0x90]
003d1ec0: ldr      r2, [pc, #0x90]
003d1ec4: add      r4, pc, r4
003d1ec8: ldr      r3, [r4, r6]
003d1ecc: ldr      r7, [r4, r2]
003d1ed0: sub      sp, sp, #0x20
003d1ed4: ldr      r3, [r3]
003d1ed8: mov      r8, r0
003d1edc: mov      r0, r7
003d1ee0: str      r3, [sp, #0x1c]
003d1ee4: bl       #0x337888
003d1ee8: ldr      r1, [pc, #0x6c]
003d1eec: add      r5, sp, #4
003d1ef0: mov      r2, sp
003d1ef4: add      r1, pc, r1
003d1ef8: mov      r0, r5
003d1efc: bl       #0x3140ec
003d1f00: mov      r1, r5
003d1f04: mov      r0, r7
003d1f08: bl       #0x337a88
003d1f0c: mov      r0, r5
003d1f10: bl       #0x3139ac
003d1f14: ldr      r3, [r8, #0x1c]
003d1f18: cmp      r3, #0
003d1f1c: beq      #0x3d1f30
003d1f20: mov      r0, r3
003d1f24: ldr      r3, [r3]
003d1f28: mov      lr, pc
003d1f2c: ldr      pc, [r3, #0x44]
003d1f30: ldr      r3, [r4, r6]
003d1f34: ldr      r2, [sp, #0x1c]
003d1f38: ldr      r3, [r3]
003d1f3c: cmp      r2, r3
003d1f40: bne      #0x3d1f4c
003d1f44: add      sp, sp, #0x20
003d1f48: pop      {r4, r5, r6, r7, r8, pc}
003d1f4c: bl       #0x30e310
003d1f50: subseq   r2, ip, ip, asr #23
003d1f54: andeq    r4, r0, ip, lsr #1
003d1f58: andeq    r0, r0, r4, lsl #17
003d1f5c: strdeq   r3, r4, [pc], #-0x54

# _ZN6CharAI20OnTargetInCloseRangeEv
003d1ca0: push     {r4, r5, r6, r7, r8, lr}
003d1ca4: ldr      r4, [pc, #0x90]
003d1ca8: ldr      r6, [pc, #0x90]
003d1cac: ldr      r2, [pc, #0x90]
003d1cb0: add      r4, pc, r4
003d1cb4: ldr      r3, [r4, r6]
003d1cb8: ldr      r7, [r4, r2]
003d1cbc: sub      sp, sp, #0x20
003d1cc0: ldr      r3, [r3]
003d1cc4: mov      r8, r0
003d1cc8: mov      r0, r7
003d1ccc: str      r3, [sp, #0x1c]
003d1cd0: bl       #0x337888
003d1cd4: ldr      r1, [pc, #0x6c]
003d1cd8: add      r5, sp, #4
003d1cdc: mov      r2, sp
003d1ce0: add      r1, pc, r1
003d1ce4: mov      r0, r5
003d1ce8: bl       #0x3140ec
003d1cec: mov      r1, r5
003d1cf0: mov      r0, r7
003d1cf4: bl       #0x337a88
003d1cf8: mov      r0, r5
003d1cfc: bl       #0x3139ac
003d1d00: ldr      r3, [r8, #0x1c]
003d1d04: cmp      r3, #0
003d1d08: beq      #0x3d1d1c
003d1d0c: mov      r0, r3
003d1d10: ldr      r3, [r3]
003d1d14: mov      lr, pc
003d1d18: ldr      pc, [r3, #0x58]
003d1d1c: ldr      r3, [r4, r6]
003d1d20: ldr      r2, [sp, #0x1c]
003d1d24: ldr      r3, [r3]
003d1d28: cmp      r2, r3
003d1d2c: bne      #0x3d1d38
003d1d30: add      sp, sp, #0x20
003d1d34: pop      {r4, r5, r6, r7, r8, pc}
003d1d38: bl       #0x30e310
003d1d3c: subseq   r2, ip, r0, ror #27
003d1d40: andeq    r4, r0, ip, lsr #1
003d1d44: andeq    r0, r0, r4, lsl #17
003d1d48: subeq    r3, pc, r8, lsl #16

# _ZN6CharAI20OnTargetInMeleeRangeEv
003d1bf4: push     {r4, r5, r6, r7, r8, lr}
003d1bf8: ldr      r4, [pc, #0x90]
003d1bfc: ldr      r6, [pc, #0x90]
003d1c00: ldr      r2, [pc, #0x90]
003d1c04: add      r4, pc, r4
003d1c08: ldr      r3, [r4, r6]
003d1c0c: ldr      r7, [r4, r2]
003d1c10: sub      sp, sp, #0x20
003d1c14: ldr      r3, [r3]
003d1c18: mov      r8, r0
003d1c1c: mov      r0, r7
003d1c20: str      r3, [sp, #0x1c]
003d1c24: bl       #0x337888
003d1c28: ldr      r1, [pc, #0x6c]
003d1c2c: add      r5, sp, #4
003d1c30: mov      r2, sp
003d1c34: add      r1, pc, r1
003d1c38: mov      r0, r5
003d1c3c: bl       #0x3140ec
003d1c40: mov      r1, r5
003d1c44: mov      r0, r7
003d1c48: bl       #0x337a88
003d1c4c: mov      r0, r5
003d1c50: bl       #0x3139ac
003d1c54: ldr      r3, [r8, #0x1c]
003d1c58: cmp      r3, #0
003d1c5c: beq      #0x3d1c70
003d1c60: mov      r0, r3
003d1c64: ldr      r3, [r3]
003d1c68: mov      lr, pc
003d1c6c: ldr      pc, [r3, #0x5c]
003d1c70: ldr      r3, [r4, r6]
003d1c74: ldr      r2, [sp, #0x1c]
003d1c78: ldr      r3, [r3]
003d1c7c: cmp      r2, r3
003d1c80: bne      #0x3d1c8c
003d1c84: add      sp, sp, #0x20
003d1c88: pop      {r4, r5, r6, r7, r8, pc}
003d1c8c: bl       #0x30e310
003d1c90: subseq   r2, ip, ip, lsl #29
003d1c94: andeq    r4, r0, ip, lsr #1
003d1c98: andeq    r0, r0, r4, lsl #17
003d1c9c: strheq   r3, [pc], #-0x84

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

# _ZN6CharAI18OnTargetOutOfRangeEv
003d1e00: push     {r4, r5, r6, r7, r8, lr}
003d1e04: ldr      r4, [pc, #0x98]
003d1e08: ldr      r6, [pc, #0x98]
003d1e0c: ldr      r2, [pc, #0x98]
003d1e10: add      r4, pc, r4
003d1e14: ldr      r3, [r4, r6]
003d1e18: ldr      r8, [r4, r2]
003d1e1c: sub      sp, sp, #0x20
003d1e20: ldr      r3, [r3]
003d1e24: mov      r7, r0
003d1e28: mov      r0, r8
003d1e2c: str      r3, [sp, #0x1c]
003d1e30: bl       #0x337888
003d1e34: ldr      r1, [pc, #0x74]
003d1e38: add      r5, sp, #4
003d1e3c: mov      r2, sp
003d1e40: add      r1, pc, r1
003d1e44: mov      r0, r5
003d1e48: bl       #0x3140ec
003d1e4c: mov      r1, r5
003d1e50: mov      r0, r8
003d1e54: bl       #0x337a88
003d1e58: mov      r0, r5
003d1e5c: bl       #0x3139ac
003d1e60: ldr      r3, [r7, #0x1c]
003d1e64: mov      r2, #0
003d1e68: strb     r2, [r7, #0x4c]
003d1e6c: cmp      r3, r2
003d1e70: beq      #0x3d1e84
003d1e74: mov      r0, r3
003d1e78: ldr      r3, [r3]
003d1e7c: mov      lr, pc
003d1e80: ldr      pc, [r3, #0x50]
003d1e84: ldr      r3, [r4, r6]
003d1e88: ldr      r2, [sp, #0x1c]
003d1e8c: ldr      r3, [r3]
003d1e90: cmp      r2, r3
003d1e94: bne      #0x3d1ea0
003d1e98: add      sp, sp, #0x20
003d1e9c: pop      {r4, r5, r6, r7, r8, pc}
003d1ea0: bl       #0x30e310
003d1ea4: subseq   r2, ip, r0, lsl #25
003d1ea8: andeq    r4, r0, ip, lsr #1
003d1eac: andeq    r0, r0, r4, lsl #17
003d1eb0: subeq    r3, pc, r8, lsr #13

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
