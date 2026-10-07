
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

# _ZN6CharAI13AI_ClearAggroEP9Character
003d6d68: push     {r4, r5, lr}
003d6d6c: subs     r4, r1, #0
003d6d70: sub      sp, sp, #0xc
003d6d74: mov      r5, r0
003d6d78: beq      #0x3d6e98
003d6d7c: ldr      r2, [r0, #0x80]
003d6d80: add      r0, r0, #0x7c
003d6d84: cmp      r2, #0
003d6d88: beq      #0x3d6ea0
003d6d8c: mov      ip, r0
003d6d90: mov      r3, r2
003d6d94: b        #0x3d6d9c
003d6d98: mov      r3, r1
003d6d9c: ldr      r1, [r3, #0x10]
003d6da0: cmp      r1, r4
003d6da4: ldrlo    r1, [r3, #0xc]
003d6da8: ldrhs    r1, [r3, #8]
003d6dac: movlo    r3, ip
003d6db0: mov      ip, r3
003d6db4: cmp      r1, #0
003d6db8: bne      #0x3d6d98
003d6dbc: cmp      r0, r3
003d6dc0: beq      #0x3d6e88
003d6dc4: ldr      r1, [r3, #0x10]
003d6dc8: cmp      r1, r4
003d6dcc: bhi      #0x3d6ea0
003d6dd0: cmp      r0, r3
003d6dd4: beq      #0x3d6e88
003d6dd8: cmp      r2, #0
003d6ddc: beq      #0x3d6e20
003d6de0: mov      r1, r0
003d6de4: b        #0x3d6dec
003d6de8: mov      r2, r3
003d6dec: ldr      r3, [r2, #0x10]
003d6df0: cmp      r3, r4
003d6df4: ldrlo    r3, [r2, #0xc]
003d6df8: ldrhs    r3, [r2, #8]
003d6dfc: movlo    r2, r1
003d6e00: mov      r1, r2
003d6e04: cmp      r3, #0
003d6e08: bne      #0x3d6de8
003d6e0c: cmp      r0, r2
003d6e10: beq      #0x3d6e20
003d6e14: ldr      r3, [r2, #0x10]
003d6e18: cmp      r3, r4
003d6e1c: bls      #0x3d6ec0
003d6e20: ldr      r3, [r4, #0x460]
003d6e24: cmp      r3, #0
003d6e28: beq      #0x3d6eb8
003d6e2c: add      r0, r4, #0x450
003d6e30: add      r0, r0, #0xc
003d6e34: ldr      r1, [r5, #4]
003d6e38: mov      ip, r0
003d6e3c: b        #0x3d6e44
003d6e40: mov      r3, r2
003d6e44: ldr      r2, [r3, #0x10]
003d6e48: cmp      r1, r2
003d6e4c: ldrhi    r2, [r3, #0xc]
003d6e50: ldrls    r2, [r3, #8]
003d6e54: movhi    r3, ip
003d6e58: mov      ip, r3
003d6e5c: cmp      r2, #0
003d6e60: bne      #0x3d6e40
003d6e64: cmp      r0, r3
003d6e68: beq      #0x3d6e78
003d6e6c: ldr      r2, [r3, #0x10]
003d6e70: cmp      r1, r2
003d6e74: bhs      #0x3d6ea8
003d6e78: ldr      r3, [r4, #0x3c8]
003d6e7c: add      r0, r4, #0x3c8
003d6e80: mov      lr, pc
003d6e84: ldr      pc, [r3, #0x3c]
003d6e88: ldr      r2, [r5, #4]
003d6e8c: ldr      r3, [r4, #0x408]
003d6e90: cmp      r2, r3
003d6e94: beq      #0x3d6ed0
003d6e98: add      sp, sp, #0xc
003d6e9c: pop      {r4, r5, pc}
003d6ea0: mov      r3, r0
003d6ea4: b        #0x3d6dd0
003d6ea8: add      r1, sp, #8
003d6eac: str      r3, [r1, #-8]!
003d6eb0: mov      r1, sp
003d6eb4: bl       #0x3d5d9c
003d6eb8: ldr      r1, [r5, #4]
003d6ebc: b        #0x3d6e78
003d6ec0: add      r1, sp, #8
003d6ec4: str      r2, [r1, #-4]!
003d6ec8: bl       #0x3d5d9c
003d6ecc: b        #0x3d6e20
003d6ed0: mov      r1, #0
003d6ed4: add      r0, r4, #0x3c8
003d6ed8: mov      r2, r1
003d6edc: bl       #0x3d6890
003d6ee0: ldr      r0, [r4, #0x378]
003d6ee4: bl       #0x40559c
003d6ee8: b        #0x3d6e98

# _ZNK6CharAI21AI_IsTargetACharacterEv
003d5484: push     {r4, lr}
003d5488: ldr      r1, [r0, #0x40]
003d548c: sub      sp, sp, #0x10
003d5490: cmp      r1, #0
003d5494: moveq    r0, r1
003d5498: beq      #0x3d54b8
003d549c: add      r4, sp, #4
003d54a0: mov      r0, r4
003d54a4: bl       #0x33dd2c
003d54a8: mov      r0, r4
003d54ac: bl       #0x33ff54
003d54b0: subs     r0, r0, #0
003d54b4: movne    r0, #1
003d54b8: add      sp, sp, #0x10
003d54bc: pop      {r4, pc}

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

# _ZN6CharAI23AI_GetTargetAsCharacterEv
003d5450: push     {r4, lr}
003d5454: ldr      r1, [r0, #0x40]
003d5458: sub      sp, sp, #0x10
003d545c: cmp      r1, #0
003d5460: moveq    r0, r1
003d5464: beq      #0x3d547c
003d5468: add      r4, sp, #4
003d546c: mov      r0, r4
003d5470: bl       #0x33dd2c
003d5474: mov      r0, r4
003d5478: bl       #0x33ff54
003d547c: add      sp, sp, #0x10
003d5480: pop      {r4, pc}

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
