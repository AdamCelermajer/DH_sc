
# _ZN11POCharacterC2EP13PhysicalWorldP10GameObjectbsttb.clone.2
003b4010: push     {r4, r5, r6, r7, lr}
003b4014: sub      sp, sp, #0x24
003b4018: ldrh     r5, [sp, #0x38]
003b401c: ldrh     lr, [sp, #0x3c]
003b4020: ldrb     r6, [sp, #0x40]
003b4024: mov      ip, #0
003b4028: str      r3, [sp, #0xc]
003b402c: mov      r7, #1
003b4030: mov      r3, ip
003b4034: ldr      r4, [pc, #0x44]
003b4038: str      r5, [sp, #0x10]
003b403c: str      lr, [sp, #0x14]
003b4040: mov      r5, r0
003b4044: str      ip, [sp, #4]
003b4048: str      ip, [sp, #0x18]
003b404c: str      r7, [sp]
003b4050: str      r6, [sp, #8]
003b4054: bl       #0x46f2f0
003b4058: ldr      r3, [pc, #0x24]
003b405c: add      r4, pc, r4
003b4060: mov      r0, r5
003b4064: ldr      r3, [r4, r3]
003b4068: add      r3, r3, #8
003b406c: str      r3, [r5]
003b4070: bl       #0x46eb20
003b4074: mov      r0, r5
003b4078: add      sp, sp, #0x24
003b407c: pop      {r4, r5, r6, r7, pc}
003b4080: subseq   r0, lr, r4, lsr sl
003b4084: andeq    r1, r0, ip, lsr #4

# _ZN10GameObject14UpdatePFObjectEv
00393ea0: push     {r4, r5, r6, r7, lr}
00393ea4: ldr      r3, [r0, #0x1c8]
00393ea8: ldr      r5, [pc, #0xd8]
00393eac: sub      sp, sp, #0xc
00393eb0: cmp      r3, #0
00393eb4: mov      r4, r0
00393eb8: add      r5, pc, r5
00393ebc: beq      #0x393ee8
00393ec0: ldr      r3, [r0]
00393ec4: mov      lr, pc
00393ec8: ldr      pc, [r3, #0xb4]
00393ecc: cmp      r0, #0
00393ed0: bne      #0x393f38
00393ed4: ldr      r0, [r4, #0x2dc]
00393ed8: cmp      r0, #0
00393edc: beq      #0x393ef0
00393ee0: bl       #0x46e750
00393ee4: str      r0, [r4, #0x1d0]
00393ee8: add      sp, sp, #0xc
00393eec: pop      {r4, r5, r6, r7, pc}
00393ef0: ldr      r1, [r4, #0x144]
00393ef4: ldr      r0, [r4, #0x150]
00393ef8: bl       #0x30e3ac
00393efc: ldr      r1, [r4, #0x148]
00393f00: mov      r5, r0
00393f04: ldr      r0, [r4, #0x154]
00393f08: bl       #0x30e3ac
00393f0c: mov      r6, r0
00393f10: mov      r1, r6
00393f14: mov      r0, r5
00393f18: bl       #0x30e70c
00393f1c: cmp      r0, #0
00393f20: movne    r5, r6
00393f24: mov      r0, r5
00393f28: mov      r1, #0x3f000000
00393f2c: bl       #0x30ed6c
00393f30: str      r0, [r4, #0x1d0]
00393f34: b        #0x393ee8
00393f38: ldr      r3, [r4]
00393f3c: mov      r0, r4
00393f40: ldr      r7, [r4, #0x2dc]
00393f44: mov      lr, pc
00393f48: ldr      pc, [r3, #0xb8]
00393f4c: ldr      r3, [r4]
00393f50: mov      r6, r0
00393f54: mov      r0, r4
00393f58: mov      lr, pc
00393f5c: ldr      pc, [r3, #0xbc]
00393f60: ldr      r3, [pc, #0x24]
00393f64: subs     r7, r7, #0
00393f68: movne    r7, #1
00393f6c: str      r0, [sp]
00393f70: mov      r2, r7
00393f74: ldr      r0, [r5, r3]
00393f78: add      r1, r4, #0x1c8
00393f7c: mov      r3, r6
00393f80: bl       #0x528234
00393f84: b        #0x393ed4

# _ZNK9Character11GetCharTypeEv
003a3054: push     {r4, lr}
003a3058: bl       #0x3a3024
003a305c: ldr      r0, [r0, #0x38]
003a3060: pop      {r4, pc}

# _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
00394bf8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00394bfc: ldr      r4, [pc, #0x120]
00394c00: ldr      r7, [pc, #0x120]
00394c04: ldr      ip, [pc, #0x120]
00394c08: add      r4, pc, r4
00394c0c: ldr      r3, [r4, r7]
00394c10: ldr      r8, [r4, ip]
00394c14: sub      sp, sp, #0x20
00394c18: ldr      r3, [r3]
00394c1c: add      r5, sp, #4
00394c20: mov      sl, r0
00394c24: mov      r0, r8
00394c28: str      r3, [sp, #0x1c]
00394c2c: mov      sb, r2
00394c30: mov      r6, r1
00394c34: bl       #0x337888
00394c38: mov      r0, r5
00394c3c: mov      r1, #0xd
00394c40: str      r5, [sp, #0x14]
00394c44: str      r5, [sp, #0x18]
00394c48: bl       #0x31167c
00394c4c: ldr      r1, [pc, #0xdc]
00394c50: mov      r2, #0xc
00394c54: ldr      r0, [sp, #0x18]
00394c58: add      r1, pc, r1
00394c5c: bl       #0x30e868
00394c60: add      r3, r0, #0xc
00394c64: str      r3, [sp, #0x14]
00394c68: mov      r3, #0
00394c6c: strb     r3, [r0, #0xc]
00394c70: mov      r1, r5
00394c74: mov      r0, r8
00394c78: bl       #0x337a88
00394c7c: mov      r8, r0
00394c80: mov      r0, r5
00394c84: bl       #0x3139ac
00394c88: cmp      r8, #0
00394c8c: beq      #0x394cc4
00394c90: cmp      r6, #0
00394c94: beq      #0x394ca8
00394c98: mov      r0, r6
00394c9c: ldr      r3, [r6]
00394ca0: mov      lr, pc
00394ca4: ldr      pc, [r3, #4]
00394ca8: ldr      r3, [r4, r7]
00394cac: ldr      r2, [sp, #0x1c]
00394cb0: ldr      r3, [r3]
00394cb4: cmp      r2, r3
00394cb8: bne      #0x394d20
00394cbc: add      sp, sp, #0x20
00394cc0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00394cc4: ldr      r3, [sl, #0x2dc]
00394cc8: cmp      r3, r6
00394ccc: beq      #0x394d00
00394cd0: cmp      r3, #0
00394cd4: beq      #0x394cec
00394cd8: mov      r0, r3
00394cdc: ldr      r3, [r3]
00394ce0: mov      lr, pc
00394ce4: ldr      pc, [r3, #4]
00394ce8: str      r8, [sl, #0x2dc]
00394cec: cmp      r6, #0
00394cf0: str      r6, [sl, #0x2dc]
00394cf4: beq      #0x394d00
00394cf8: cmp      sb, #0
00394cfc: bne      #0x394d0c
00394d00: mov      r0, sl
00394d04: bl       #0x393ea0
00394d08: b        #0x394ca8
00394d0c: mov      r0, r6
00394d10: bl       #0x46eb20
00394d14: mov      r0, sl
00394d18: bl       #0x393ea0
00394d1c: b        #0x394ca8
00394d20: bl       #0x30e310
00394d24: subseq   pc, pc, r8, lsl #29
00394d28: andeq    r4, r0, ip, lsr #1
00394d2c: andeq    r0, r0, r4, lsl #17
00394d30: subseq   sp, r2, r8, lsr #24

# _ZNK9Character8IsFaerieEv
003a3094: push     {r4, lr}
003a3098: bl       #0x3a3054
003a309c: cmp      r0, #3
003a30a0: movne    r0, #0
003a30a4: moveq    r0, #1
003a30a8: pop      {r4, pc}

# _ZNK9Character14IsInvisibleManEv
003a30dc: push     {r4, lr}
003a30e0: bl       #0x3a3054
003a30e4: cmp      r0, #9
003a30e8: movne    r0, #0
003a30ec: moveq    r0, #1
003a30f0: pop      {r4, pc}

# _ZNK9Character9IsCleanerEv
003a30f4: push     {r4, lr}
003a30f8: bl       #0x3a3054
003a30fc: cmp      r0, #8
003a3100: movne    r0, #0
003a3104: moveq    r0, #1
003a3108: pop      {r4, pc}

# _ZN14PhysicalObject5_initEP10b2ShapeDefffbb
0046edf0: push     {r4, r5, r6, r7, lr}
0046edf4: sub      sp, sp, #0x34
0046edf8: ldrb     lr, [sp, #0x4c]
0046edfc: mov      r4, #0
0046ee00: mov      r6, r1
0046ee04: mov      r5, r0
0046ee08: mov      ip, #1
0046ee0c: ldr      r0, [r0, #4]
0046ee10: add      r1, sp, #4
0046ee14: str      r4, [sp, #8]
0046ee18: str      r4, [sp, #0xc]
0046ee1c: str      r4, [sp, #4]
0046ee20: str      r4, [sp, #0x10]
0046ee24: str      r4, [sp, #0x20]
0046ee28: str      r4, [sp, #0x24]
0046ee2c: str      r4, [sp, #0x28]
0046ee30: ldrb     r7, [sp, #0x48]
0046ee34: str      r3, [sp, #0x1c]
0046ee38: strb     lr, [sp, #0x2f]
0046ee3c: strb     ip, [sp, #0x2e]
0046ee40: str      r2, [sp, #0x18]
0046ee44: strb     ip, [sp, #0x2c]
0046ee48: str      r5, [sp, #0x14]
0046ee4c: strb     ip, [sp, #0x2d]
0046ee50: bl       #0x34bcf0
0046ee54: mov      r3, #0x3f800000
0046ee58: str      r0, [r5, #0x14]
0046ee5c: str      r5, [r0, #0x90]
0046ee60: str      r3, [r6, #0xc]
0046ee64: ldrb     r3, [r6, #0x18]
0046ee68: cmp      r7, #0
0046ee6c: str      r4, [r6, #0x10]
0046ee70: movweq   r4, #0xd70a
0046ee74: movteq   r4, #0x4133
0046ee78: cmp      r3, #0
0046ee7c: str      r5, [r6, #8]
0046ee80: str      r4, [r6, #0x14]
0046ee84: bne      #0x46eea4
0046ee88: mov      r1, r6
0046ee8c: mov      r0, r5
0046ee90: mov      r2, #1
0046ee94: bl       #0x46edb8
0046ee98: str      r0, [r5, #0x18]
0046ee9c: add      sp, sp, #0x34
0046eea0: pop      {r4, r5, r6, r7, pc}
0046eea4: mov      r1, r6
0046eea8: mov      r0, r5
0046eeac: mov      r2, #1
0046eeb0: bl       #0x46edb8
0046eeb4: str      r0, [r5, #0x1c]
0046eeb8: b        #0x46ee9c

# _ZN7b2World10CreateBodyEPK9b2BodyDef
007e7ef8: push     {r4, r5, r6, r7, r8, lr}
007e7efc: mov      r3, #0x19000
007e7f00: add      r3, r3, #0x1d4
007e7f04: ldrb     r6, [r0, r3]
007e7f08: mov      r4, r0
007e7f0c: mov      r7, r1
007e7f10: cmp      r6, #0
007e7f14: movne    r5, #0
007e7f18: bne      #0x7e7f74
007e7f1c: mov      r1, #0x94
007e7f20: bl       #0x7e90bc
007e7f24: mov      r2, r4
007e7f28: mov      r1, r7
007e7f2c: mov      r5, r0
007e7f30: bl       #0x7e2218
007e7f34: mov      r3, #0x19000
007e7f38: str      r6, [r5, #0x5c]
007e7f3c: add      r3, r3, #0x230
007e7f40: ldr      r2, [r4, r3]
007e7f44: str      r2, [r5, #0x60]
007e7f48: ldr      r3, [r4, r3]
007e7f4c: mov      r2, #0x19000
007e7f50: add      r2, r2, #0x230
007e7f54: cmp      r3, #0
007e7f58: strne    r5, [r3, #0x5c]
007e7f5c: mov      r3, #0x19000
007e7f60: str      r5, [r4, r2]
007e7f64: add      r3, r3, #0x23c
007e7f68: ldr      r2, [r4, r3]
007e7f6c: add      r2, r2, #1
007e7f70: str      r2, [r4, r3]
007e7f74: mov      r0, r5
007e7f78: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14PhysicalObject9_addShapeEP10b2ShapeDefb
0046edb8: push     {r4, r5, r6, lr}
0046edbc: mov      r4, r0
0046edc0: ldr      r0, [r0, #0x14]
0046edc4: mov      r5, r2
0046edc8: bl       #0x7e1dc8
0046edcc: subs     r6, r0, #0
0046edd0: beq      #0x46ede8
0046edd4: cmp      r5, #0
0046edd8: str      r4, [r6, #0x2c]
0046eddc: beq      #0x46ede8
0046ede0: ldr      r0, [r4, #0x14]
0046ede4: bl       #0x7e1818
0046ede8: mov      r0, r6
0046edec: pop      {r4, r5, r6, pc}

# _ZNK9Character10IsMerchantEv
003a30c4: push     {r4, lr}
003a30c8: bl       #0x3a3054
003a30cc: cmp      r0, #7
003a30d0: movne    r0, #0
003a30d4: moveq    r0, #1
003a30d8: pop      {r4, pc}

# _ZNK9Character9IsMonsterEv
003a3064: push     {r4, lr}
003a3068: bl       #0x3a3054
003a306c: cmp      r0, #4
003a3070: movne    r0, #0
003a3074: moveq    r0, #1
003a3078: pop      {r4, pc}

# _ZNK9Character10IsFollowerEv
003a307c: push     {r4, lr}
003a3080: bl       #0x3a3054
003a3084: cmp      r0, #2
003a3088: movne    r0, #0
003a308c: moveq    r0, #1
003a3090: pop      {r4, pc}

# _ZNK9Character10IsSummonedEv
003a30ac: push     {r4, lr}
003a30b0: bl       #0x3a3054
003a30b4: cmp      r0, #5
003a30b8: movne    r0, #0
003a30bc: moveq    r0, #1
003a30c0: pop      {r4, pc}

# _ZNK9Character9GetCharAIEv
003a3024: ldr      r3, [pc, #0x20]
003a3028: ldr      r2, [pc, #0x20]
003a302c: push     {r4, lr}
003a3030: add      r3, pc, r3
003a3034: ldr      r2, [r3, r2]
003a3038: ldr      r4, [r2]
003a303c: bl       #0x3a2fec
003a3040: mov      r3, #0x44
003a3044: mla      r0, r3, r0, r4
003a3048: pop      {r4, pc}
003a304c: subseq   r1, pc, r0, ror #20
003a3050: andeq    r0, r0, r8, asr r7

# _ZN6b2BodyC1EPK9b2BodyDefP7b2World
007e2218: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e221c: mov      r3, #0
007e2220: strh     r3, [r0]
007e2224: ldrb     r3, [r1, #0x2b]
007e2228: mov      r4, r0
007e222c: mov      r5, r1
007e2230: cmp      r3, #0
007e2234: movne    r3, #0x20
007e2238: strhne   r3, [r0]
007e223c: ldrb     r3, [r1, #0x2a]
007e2240: mov      sb, #0x3f800000
007e2244: cmp      r3, #0
007e2248: ldrhne   r3, [r0]
007e224c: orrne    r3, r3, #0x40
007e2250: strhne   r3, [r0]
007e2254: ldrb     r3, [r1, #0x28]
007e2258: cmp      r3, #0
007e225c: ldrhne   r3, [r0]
007e2260: orrne    r3, r3, #0x10
007e2264: strhne   r3, [r0]
007e2268: ldrb     r3, [r1, #0x29]
007e226c: str      r2, [r0, #0x58]
007e2270: cmp      r3, #0
007e2274: ldrhne   r3, [r0]
007e2278: orrne    r3, r3, #8
007e227c: strhne   r3, [r0]
007e2280: ldr      r3, [r1, #0x14]
007e2284: str      r3, [r0, #4]
007e2288: ldr      r3, [r1, #0x18]
007e228c: str      r3, [r0, #8]
007e2290: ldr      r7, [r1, #0x1c]
007e2294: mov      r0, r7
007e2298: bl       #0x30e754
007e229c: mov      r6, r0
007e22a0: mov      r0, r7
007e22a4: bl       #0x30eb08
007e22a8: add      sl, r0, #0x80000000
007e22ac: str      r6, [r4, #0xc]
007e22b0: str      sl, [r4, #0x14]
007e22b4: str      r0, [r4, #0x10]
007e22b8: str      r6, [r4, #0x18]
007e22bc: ldr      r3, [r5, #4]
007e22c0: mov      r7, r0
007e22c4: mov      r1, r6
007e22c8: str      r3, [r4, #0x1c]
007e22cc: ldr      r3, [r5, #8]
007e22d0: str      sb, [r4, #0x3c]
007e22d4: ldr      r8, [r4, #0x1c]
007e22d8: str      r3, [r4, #0x20]
007e22dc: ldr      r3, [r5, #0x1c]
007e22e0: mov      r0, r8
007e22e4: str      r3, [r4, #0x34]
007e22e8: str      r3, [r4, #0x38]
007e22ec: bl       #0x30ed6c
007e22f0: mov      r1, sl
007e22f4: mov      fp, r0
007e22f8: ldr      r0, [r4, #0x20]
007e22fc: bl       #0x30ed6c
007e2300: mov      r1, r0
007e2304: mov      r0, fp
007e2308: bl       #0x30eba4
007e230c: mov      r1, r7
007e2310: mov      sl, r0
007e2314: mov      r0, r8
007e2318: bl       #0x30ed6c
007e231c: mov      r1, r6
007e2320: mov      r7, r0
007e2324: ldr      r0, [r4, #0x20]
007e2328: bl       #0x30ed6c
007e232c: mov      r1, r0
007e2330: mov      r0, r7
007e2334: bl       #0x30eba4
007e2338: ldr      r1, [r4, #4]
007e233c: mov      r7, r0
007e2340: mov      r0, sl
007e2344: bl       #0x30eba4
007e2348: ldr      r1, [r4, #8]
007e234c: mov      r6, r0
007e2350: mov      r0, r7
007e2354: bl       #0x30eba4
007e2358: str      r6, [r4, #0x2c]
007e235c: str      r0, [r4, #0x30]
007e2360: ldr      r1, [r4, #0x2c]
007e2364: ldr      r2, [r4, #0x30]
007e2368: mov      r3, #0
007e236c: str      r1, [r4, #0x24]
007e2370: str      r2, [r4, #0x28]
007e2374: str      r3, [r4, #0x60]
007e2378: str      r3, [r4, #0x6c]
007e237c: str      r3, [r4, #0x70]
007e2380: str      r3, [r4, #0x5c]
007e2384: ldr      r2, [r5, #0x20]
007e2388: mov      r3, #0
007e238c: mov      r1, r3
007e2390: str      r2, [r4, #0x84]
007e2394: ldr      r2, [r5, #0x24]
007e2398: str      r3, [r4, #0x4c]
007e239c: str      r3, [r4, #0x50]
007e23a0: str      r2, [r4, #0x88]
007e23a4: str      r3, [r4, #0x54]
007e23a8: str      r3, [r4, #0x40]
007e23ac: str      r3, [r4, #0x44]
007e23b0: str      r3, [r4, #0x48]
007e23b4: str      r3, [r4, #0x8c]
007e23b8: str      r3, [r4, #0x78]
007e23bc: str      r3, [r4, #0x7c]
007e23c0: str      r3, [r4, #0x80]
007e23c4: ldr      r6, [r5]
007e23c8: str      r6, [r4, #0x74]
007e23cc: mov      r0, r6
007e23d0: bl       #0x30e2f8
007e23d4: cmp      r0, #0
007e23d8: beq      #0x7e23ec
007e23dc: mov      r0, sb
007e23e0: mov      r1, r6
007e23e4: bl       #0x30ec94
007e23e8: str      r0, [r4, #0x78]
007e23ec: ldrh     r3, [r4]
007e23f0: mov      r1, #0
007e23f4: tst      r3, #0x40
007e23f8: ldreq    r6, [r5, #0xc]
007e23fc: ldrne    r6, [r4, #0x7c]
007e2400: streq    r6, [r4, #0x7c]
007e2404: mov      r0, r6
007e2408: bl       #0x30e2f8
007e240c: cmp      r0, #0
007e2410: beq      #0x7e2424
007e2414: mov      r1, r6
007e2418: mov      r0, #0x3f800000
007e241c: bl       #0x30ec94
007e2420: str      r0, [r4, #0x80]
007e2424: ldr      r0, [r4, #0x78]
007e2428: mov      r1, #0
007e242c: bl       #0x30df8c
007e2430: cmp      r0, #0
007e2434: beq      #0x7e2454
007e2438: ldr      r0, [r4, #0x80]
007e243c: mov      r1, #0
007e2440: bl       #0x30df8c
007e2444: cmp      r0, #0
007e2448: movne    r2, #0
007e244c: strhne   r2, [r4, #2]
007e2450: bne      #0x7e245c
007e2454: mov      r3, #1
007e2458: strh     r3, [r4, #2]
007e245c: ldr      r2, [r5, #0x10]
007e2460: mov      r3, #0
007e2464: str      r3, [r4, #0x68]
007e2468: str      r2, [r4, #0x90]
007e246c: str      r3, [r4, #0x64]
007e2470: mov      r0, r4
007e2474: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti
0046f2f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046f2f4: ldr      r5, [pc, #0x360]
0046f2f8: ldr      r4, [pc, #0x360]
0046f2fc: sub      sp, sp, #0xb4
0046f300: add      r5, pc, r5
0046f304: str      r4, [sp, #0x10]
0046f308: ldr      lr, [r5, r4]
0046f30c: ldr      ip, [pc, #0x350]
0046f310: ldr      r4, [pc, #0x350]
0046f314: ldrb     sb, [sp, #0xd8]
0046f318: ldr      ip, [r5, ip]
0046f31c: ldr      r4, [r5, r4]
0046f320: ldr      lr, [lr]
0046f324: mov      r7, #0
0046f328: str      r4, [sp, #0xc]
0046f32c: add      ip, ip, #8
0046f330: mov      r4, r0
0046f334: mov      sl, #0
0046f338: str      r1, [r0, #4]
0046f33c: str      ip, [r0]
0046f340: str      r2, [r4, #8]
0046f344: str      sl, [r0, #0xc]
0046f348: strb     sb, [r0, #0x10]
0046f34c: str      r7, [r0, #0x14]
0046f350: str      r7, [r0, #0x18]
0046f354: str      r7, [r0, #0x1c]
0046f358: strb     r7, [r0, #0x26]
0046f35c: strb     r7, [r0, #0x27]
0046f360: ldrb     ip, [sp, #0xdc]
0046f364: mov      r6, r2
0046f368: str      lr, [sp, #0xac]
0046f36c: ldrh     r2, [sp, #0xe8]
0046f370: ldrb     lr, [sp, #0xe0]
0046f374: str      r3, [sp, #0x1c]
0046f378: ldrh     r3, [sp, #0xec]
0046f37c: ldr      r0, [sp, #0xc]
0046f380: str      ip, [sp, #0x20]
0046f384: str      lr, [sp, #0x24]
0046f388: str      r3, [sp, #0x18]
0046f38c: ldrsh    r8, [sp, #0xe4]
0046f390: str      r2, [sp, #0x14]
0046f394: bl       #0x337888
0046f398: ldr      r1, [pc, #0x2cc]
0046f39c: add      fp, sp, #0x94
0046f3a0: add      r2, sp, #0x90
0046f3a4: add      r1, pc, r1
0046f3a8: mov      r0, fp
0046f3ac: bl       #0x3140ec
0046f3b0: mov      r1, fp
0046f3b4: ldr      r0, [sp, #0xc]
0046f3b8: bl       #0x337a88
0046f3bc: mov      r3, r0
0046f3c0: mov      r0, fp
0046f3c4: str      r3, [sp, #8]
0046f3c8: bl       #0x3139ac
0046f3cc: ldr      r3, [sp, #8]
0046f3d0: mvn      r2, #0x298
0046f3d4: sub      r2, r2, #1
0046f3d8: cmp      r3, r7
0046f3dc: movne    r8, r2
0046f3e0: cmp      r6, r7
0046f3e4: beq      #0x46f558
0046f3e8: cmp      sb, r7
0046f3ec: bne      #0x46f57c
0046f3f0: ldr      r3, [pc, #0x278]
0046f3f4: mov      r2, #1
0046f3f8: ldr      r1, [r6, #0x12c]
0046f3fc: ldr      r3, [r5, r3]
0046f400: ldr      r0, [r6, #0x138]
0046f404: str      r2, [sp, #0x30]
0046f408: add      ip, r3, #8
0046f40c: movw     r3, #0xcccd
0046f410: movt     r3, #0x3e4c
0046f414: strh     r2, [sp, #0x46]
0046f418: mvn      r2, #0
0046f41c: str      ip, [sp, #0x2c]
0046f420: strh     r2, [sp, #0x48]
0046f424: str      r3, [sp, #0x38]
0046f428: str      sl, [sp, #0x40]
0046f42c: str      sb, [sp, #0x8c]
0046f430: str      sb, [sp, #0x34]
0046f434: str      sl, [sp, #0x3c]
0046f438: strh     sb, [sp, #0x4a]
0046f43c: strb     sb, [sp, #0x44]
0046f440: bl       #0x30e3ac
0046f444: movw     r1, #0xd70a
0046f448: movt     r1, #0x3c23
0046f44c: bl       #0x30ed6c
0046f450: ldr      r1, [r6, #0x130]
0046f454: mov      sb, r0
0046f458: ldr      r0, [r6, #0x13c]
0046f45c: bl       #0x30e3ac
0046f460: movw     r1, #0xd70a
0046f464: movt     r1, #0x3c23
0046f468: bl       #0x30ed6c
0046f46c: movw     r1, #0xd70a
0046f470: mov      r7, r0
0046f474: movt     r1, #0x3c23
0046f478: ldr      r0, [r6, #0x160]
0046f47c: bl       #0x30ed6c
0046f480: movw     r1, #0xd70a
0046f484: movt     r1, #0x3c23
0046f488: mov      sl, r0
0046f48c: ldr      r0, [r6, #0x164]
0046f490: bl       #0x30ed6c
0046f494: mov      r1, #0x3f000000
0046f498: mov      r6, r0
0046f49c: mov      r0, sb
0046f4a0: bl       #0x30ed6c
0046f4a4: mov      r1, #0x3f000000
0046f4a8: mov      r3, r0
0046f4ac: mov      r0, r7
0046f4b0: str      r3, [sp, #8]
0046f4b4: bl       #0x30ed6c
0046f4b8: ldr      r3, [sp, #8]
0046f4bc: add      fp, sp, #0x2c
0046f4c0: mov      r2, r0
0046f4c4: mov      r1, r3
0046f4c8: mov      r0, fp
0046f4cc: bl       #0x7e44b8
0046f4d0: mov      r1, r7
0046f4d4: mov      r0, sb
0046f4d8: bl       #0x30e70c
0046f4dc: cmp      r0, #0
0046f4e0: moveq    r7, sb
0046f4e4: mov      r0, r7
0046f4e8: mov      r1, #0x3f000000
0046f4ec: bl       #0x30ed6c
0046f4f0: ldr      r3, [pc, #0x17c]
0046f4f4: str      r0, [r4, #0xc]
0046f4f8: mov      ip, fp
0046f4fc: ldr      r3, [r5, r3]
0046f500: add      r3, r3, #8
0046f504: str      r3, [sp, #0x2c]
0046f508: strh     r8, [r4, #0x24]
0046f50c: ldr      lr, [sp, #0x14]
0046f510: mov      r1, ip
0046f514: mov      r3, r6
0046f518: strh     lr, [r4, #0x20]
0046f51c: ldr      r2, [sp, #0x18]
0046f520: mov      r0, r4
0046f524: strh     r2, [r4, #0x22]
0046f528: ldr      lr, [sp, #0x20]
0046f52c: strh     r8, [ip, #0x1e]
0046f530: mov      r2, sl
0046f534: strb     lr, [ip, #0x18]
0046f538: ldr      lr, [sp, #0x14]
0046f53c: strh     lr, [ip, #0x1a]
0046f540: ldr      lr, [sp, #0x18]
0046f544: strh     lr, [ip, #0x1c]
0046f548: ldr      ip, [sp, #0x1c]
0046f54c: ldr      lr, [sp, #0x24]
0046f550: stm      sp, {ip, lr}
0046f554: bl       #0x46edf0
0046f558: ldr      ip, [sp, #0x10]
0046f55c: ldr      r2, [sp, #0xac]
0046f560: mov      r0, r4
0046f564: ldr      r3, [r5, ip]
0046f568: ldr      r3, [r3]
0046f56c: cmp      r2, r3
0046f570: bne      #0x46f658
0046f574: add      sp, sp, #0xb4
0046f578: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f57c: movw     r3, #0xcccd
0046f580: ldr      r1, [r6, #0x12c]
0046f584: ldr      r0, [r6, #0x138]
0046f588: movt     r3, #0x3e4c
0046f58c: mov      ip, #1
0046f590: mvn      lr, #0
0046f594: str      r3, [sp, #0x38]
0046f598: strh     ip, [sp, #0x46]
0046f59c: strh     lr, [sp, #0x48]
0046f5a0: strb     r7, [sp, #0x44]
0046f5a4: str      r7, [sp, #0x30]
0046f5a8: str      sl, [sp, #0x50]
0046f5ac: str      r7, [sp, #0x34]
0046f5b0: str      sl, [sp, #0x3c]
0046f5b4: str      sl, [sp, #0x40]
0046f5b8: strh     r7, [sp, #0x4a]
0046f5bc: str      sl, [sp, #0x4c]
0046f5c0: bl       #0x30e3ac
0046f5c4: movw     r1, #0xd70a
0046f5c8: movt     r1, #0x3c23
0046f5cc: bl       #0x30ed6c
0046f5d0: ldr      r1, [r6, #0x130]
0046f5d4: mov      sb, r0
0046f5d8: ldr      r0, [r6, #0x13c]
0046f5dc: bl       #0x30e3ac
0046f5e0: movw     r1, #0xd70a
0046f5e4: movt     r1, #0x3c23
0046f5e8: bl       #0x30ed6c
0046f5ec: movw     r1, #0xd70a
0046f5f0: mov      r7, r0
0046f5f4: movt     r1, #0x3c23
0046f5f8: ldr      r0, [r6, #0x160]
0046f5fc: bl       #0x30ed6c
0046f600: movw     r1, #0xd70a
0046f604: movt     r1, #0x3c23
0046f608: mov      sl, r0
0046f60c: ldr      r0, [r6, #0x164]
0046f610: bl       #0x30ed6c
0046f614: mov      r1, r7
0046f618: mov      r6, r0
0046f61c: mov      r0, sb
0046f620: bl       #0x30e70c
0046f624: cmp      r0, #0
0046f628: moveq    r7, sb
0046f62c: mov      r0, r7
0046f630: mov      r1, #0x3f000000
0046f634: bl       #0x30ed6c
0046f638: ldr      r3, [pc, #0x34]
0046f63c: add      ip, sp, #0xb0
0046f640: str      r0, [r4, #0xc]
0046f644: ldr      r3, [r5, r3]
0046f648: str      r0, [sp, #0x54]
0046f64c: add      r3, r3, #8
0046f650: str      r3, [ip, #-0x84]!
0046f654: b        #0x46f508
0046f658: bl       #0x30e310

# _ZN6b2Body7SetMassEPK10b2MassData
007e1b28: push     {r4, r5, r6, r7, r8, lr}
007e1b2c: ldr      r2, [r0, #0x58]
007e1b30: mov      r3, #0x19000
007e1b34: add      r3, r3, #0x1d4
007e1b38: ldrb     r3, [r2, r3]
007e1b3c: mov      r4, r0
007e1b40: mov      r5, r1
007e1b44: cmp      r3, #0
007e1b48: bne      #0x7e1ce8
007e1b4c: mov      r1, #0
007e1b50: str      r1, [r0, #0x78]
007e1b54: str      r1, [r0, #0x7c]
007e1b58: str      r1, [r0, #0x80]
007e1b5c: ldr      r6, [r5]
007e1b60: str      r6, [r0, #0x74]
007e1b64: mov      r0, r6
007e1b68: bl       #0x30e2f8
007e1b6c: cmp      r0, #0
007e1b70: beq      #0x7e1b84
007e1b74: mov      r1, r6
007e1b78: mov      r0, #0x3f800000
007e1b7c: bl       #0x30ec94
007e1b80: str      r0, [r4, #0x78]
007e1b84: ldrh     r3, [r4]
007e1b88: mov      r1, #0
007e1b8c: tst      r3, #0x40
007e1b90: ldreq    r6, [r5, #0xc]
007e1b94: ldrne    r6, [r4, #0x7c]
007e1b98: streq    r6, [r4, #0x7c]
007e1b9c: mov      r0, r6
007e1ba0: bl       #0x30e2f8
007e1ba4: cmp      r0, #0
007e1ba8: bne      #0x7e1d10
007e1bac: ldr      r3, [r5, #4]
007e1bb0: ldr      r1, [r4, #0xc]
007e1bb4: str      r3, [r4, #0x1c]
007e1bb8: ldr      r3, [r5, #8]
007e1bbc: ldr      r6, [r4, #0x1c]
007e1bc0: str      r3, [r4, #0x20]
007e1bc4: mov      r0, r6
007e1bc8: bl       #0x30ed6c
007e1bcc: ldr      r5, [r4, #0x20]
007e1bd0: mov      r7, r0
007e1bd4: ldr      r1, [r4, #0x14]
007e1bd8: mov      r0, r5
007e1bdc: bl       #0x30ed6c
007e1be0: mov      r1, r0
007e1be4: mov      r0, r7
007e1be8: bl       #0x30eba4
007e1bec: ldr      r1, [r4, #0x10]
007e1bf0: mov      r7, r0
007e1bf4: mov      r0, r6
007e1bf8: bl       #0x30ed6c
007e1bfc: ldr      r1, [r4, #0x18]
007e1c00: mov      r6, r0
007e1c04: mov      r0, r5
007e1c08: bl       #0x30ed6c
007e1c0c: mov      r1, r0
007e1c10: mov      r0, r6
007e1c14: bl       #0x30eba4
007e1c18: ldr      r1, [r4, #4]
007e1c1c: mov      r6, r0
007e1c20: mov      r0, r7
007e1c24: bl       #0x30eba4
007e1c28: ldr      r1, [r4, #8]
007e1c2c: mov      r5, r0
007e1c30: mov      r0, r6
007e1c34: bl       #0x30eba4
007e1c38: str      r5, [r4, #0x2c]
007e1c3c: str      r0, [r4, #0x30]
007e1c40: ldr      r5, [r4, #0x64]
007e1c44: ldr      r2, [r4, #0x2c]
007e1c48: ldr      r3, [r4, #0x30]
007e1c4c: cmp      r5, #0
007e1c50: str      r2, [r4, #0x24]
007e1c54: str      r3, [r4, #0x28]
007e1c58: beq      #0x7e1c80
007e1c5c: add      r6, r4, #0x1c
007e1c60: ldr      r3, [r5]
007e1c64: mov      r0, r5
007e1c68: mov      r1, r6
007e1c6c: mov      lr, pc
007e1c70: ldr      pc, [r3, #0x1c]
007e1c74: ldr      r5, [r5, #8]
007e1c78: cmp      r5, #0
007e1c7c: bne      #0x7e1c60
007e1c80: ldr      r0, [r4, #0x78]
007e1c84: mov      r1, #0
007e1c88: bl       #0x30df8c
007e1c8c: cmp      r0, #0
007e1c90: ldrh     r5, [r4, #2]
007e1c94: bne      #0x7e1cec
007e1c98: mov      r3, #1
007e1c9c: strh     r3, [r4, #2]
007e1ca0: mov      r3, #1
007e1ca4: sxth     r5, r5
007e1ca8: cmp      r5, r3
007e1cac: beq      #0x7e1ce8
007e1cb0: ldr      r5, [r4, #0x64]
007e1cb4: cmp      r5, #0
007e1cb8: beq      #0x7e1ce8
007e1cbc: mov      r6, #0x19000
007e1cc0: add      r6, r6, #0x1d8
007e1cc4: add      r7, r4, #4
007e1cc8: ldr      r3, [r4, #0x58]
007e1ccc: mov      r0, r5
007e1cd0: mov      r2, r7
007e1cd4: ldr      r1, [r3, r6]
007e1cd8: bl       #0x7e61b0
007e1cdc: ldr      r5, [r5, #8]
007e1ce0: cmp      r5, #0
007e1ce4: bne      #0x7e1cc8
007e1ce8: pop      {r4, r5, r6, r7, r8, pc}
007e1cec: ldr      r0, [r4, #0x80]
007e1cf0: mov      r1, #0
007e1cf4: bl       #0x30df8c
007e1cf8: cmp      r0, #0
007e1cfc: movne    r3, #0
007e1d00: strhne   r3, [r4, #2]
007e1d04: movne    r3, #0
007e1d08: bne      #0x7e1ca4
007e1d0c: b        #0x7e1c98
007e1d10: mov      r1, r6
007e1d14: mov      r0, #0x3f800000
007e1d18: bl       #0x30ec94
007e1d1c: str      r0, [r4, #0x80]
007e1d20: b        #0x7e1bac

# _ZN13PhysicalWorld10createBodyEP9b2BodyDef
0034bcf0: subs     r3, r1, #0
0034bcf4: beq      #0x34bd00
0034bcf8: ldr      r0, [r0, #0x10]
0034bcfc: b        #0x7e7ef8
0034bd00: mov      r0, r3
0034bd04: bx       lr

# _ZN14PhysicalObject3pinEv
0046eb20: str      lr, [sp, #-4]!
0046eb24: ldrb     r3, [r0, #0x27]
0046eb28: sub      sp, sp, #0x14
0046eb2c: cmp      r3, #0
0046eb30: bne      #0x46eb68
0046eb34: ldr      r2, [r0, #0x14]
0046eb38: mov      r3, #1
0046eb3c: strb     r3, [r0, #0x27]
0046eb40: ldr      r1, [r2, #0x1c]
0046eb44: mov      r0, r2
0046eb48: mov      r3, #0
0046eb4c: str      r1, [sp, #4]
0046eb50: ldr      r2, [r2, #0x20]
0046eb54: mov      r1, sp
0046eb58: str      r3, [sp, #0xc]
0046eb5c: str      r2, [sp, #8]
0046eb60: str      r3, [sp]
0046eb64: bl       #0x7e1b28
0046eb68: add      sp, sp, #0x14
0046eb6c: ldm      sp!, {pc}

# _ZN9Character18InitPhysicalObjectEv
003b4088: push     {r4, r5, r6, r7, r8, sl, lr}
003b408c: sub      sp, sp, #0x24
003b4090: mov      r5, r0
003b4094: bl       #0x3a30dc
003b4098: ldr      r4, [pc, #0x320]
003b409c: subs     r6, r0, #0
003b40a0: add      r4, pc, r4
003b40a4: bne      #0x3b41a8
003b40a8: ldrb     r7, [r5, #0x84]
003b40ac: cmp      r7, #0
003b40b0: bne      #0x3b412c
003b40b4: mov      r0, r5
003b40b8: bl       #0x3a310c
003b40bc: subs     r6, r0, #0
003b40c0: beq      #0x3b41f0
003b40c4: ldr      r3, [pc, #0x2f8]
003b40c8: mov      r1, r7
003b40cc: mov      r0, #0x28
003b40d0: ldr      r3, [r4, r3]
003b40d4: ldr      r8, [r3, #0x44]
003b40d8: bl       #0x310570
003b40dc: mov      ip, #0x400
003b40e0: mvn      r3, #3
003b40e4: str      ip, [sp]
003b40e8: mov      r1, r8
003b40ec: movw     ip, #0xd1f
003b40f0: mov      r2, r5
003b40f4: mov      r6, r0
003b40f8: str      ip, [sp, #4]
003b40fc: str      r7, [sp, #8]
003b4100: bl       #0x3b4010
003b4104: ldr      r3, [pc, #0x2bc]
003b4108: ldr      r3, [r4, r3]
003b410c: mov      r0, r5
003b4110: mov      r1, r6
003b4114: add      r3, r3, #8
003b4118: mov      r2, #1
003b411c: str      r3, [r6]
003b4120: add      sp, sp, #0x24
003b4124: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4128: b        #0x394bf8
003b412c: ldr      r3, [pc, #0x290]
003b4130: mov      r1, r6
003b4134: mov      r0, #0x28
003b4138: ldr      r3, [r4, r3]
003b413c: mov      r7, #1
003b4140: ldr      sl, [r3, #0x44]
003b4144: bl       #0x310570
003b4148: mov      ip, #2
003b414c: mov      r1, sl
003b4150: mov      r2, r5
003b4154: mov      r3, r7
003b4158: str      ip, [sp, #0x10]
003b415c: movw     ip, #0xffff
003b4160: mov      r8, r0
003b4164: str      r6, [sp, #0xc]
003b4168: str      ip, [sp, #0x14]
003b416c: str      r6, [sp]
003b4170: str      r6, [sp, #4]
003b4174: str      r6, [sp, #8]
003b4178: str      r7, [sp, #0x18]
003b417c: bl       #0x46f2f0
003b4180: ldr      r3, [pc, #0x244]
003b4184: mov      r0, r5
003b4188: mov      r1, r8
003b418c: ldr      r3, [r4, r3]
003b4190: mov      r2, r7
003b4194: add      r3, r3, #8
003b4198: str      r3, [r8]
003b419c: add      sp, sp, #0x24
003b41a0: pop      {r4, r5, r6, r7, r8, sl, lr}
003b41a4: b        #0x394bf8
003b41a8: ldr      r3, [pc, #0x214]
003b41ac: mov      r1, #0
003b41b0: mov      r0, #0x28
003b41b4: ldr      r3, [r4, r3]
003b41b8: ldr      r7, [r3, #0x44]
003b41bc: bl       #0x310570
003b41c0: mov      ip, #0
003b41c4: mov      r3, ip
003b41c8: mov      lr, #0x100
003b41cc: mov      r1, r7
003b41d0: mov      r2, r5
003b41d4: mov      r6, r0
003b41d8: str      lr, [sp]
003b41dc: str      ip, [sp, #4]
003b41e0: str      ip, [sp, #8]
003b41e4: bl       #0x3b4010
003b41e8: ldr      r3, [pc, #0x1e0]
003b41ec: b        #0x3b4108
003b41f0: ldr      r3, [r5]
003b41f4: mov      r0, r5
003b41f8: mov      lr, pc
003b41fc: ldr      pc, [r3, #0x28]
003b4200: cmp      r0, #0
003b4204: bne      #0x3b4264
003b4208: mov      r0, r5
003b420c: bl       #0x3a307c
003b4210: cmp      r0, #0
003b4214: beq      #0x3b42d0
003b4218: ldr      r3, [pc, #0x1a4]
003b421c: mov      r1, #0
003b4220: mov      r0, #0x28
003b4224: ldr      r3, [r4, r3]
003b4228: ldr      r7, [r3, #0x44]
003b422c: bl       #0x310570
003b4230: mov      ip, #8
003b4234: str      ip, [sp]
003b4238: movw     ip, #0xd3b
003b423c: mvn      r3, #1
003b4240: str      ip, [sp, #4]
003b4244: mov      r1, r7
003b4248: mov      ip, #0
003b424c: mov      r2, r5
003b4250: mov      r6, r0
003b4254: str      ip, [sp, #8]
003b4258: bl       #0x3b4010
003b425c: ldr      r3, [pc, #0x170]
003b4260: b        #0x3b4108
003b4264: ldr      r3, [pc, #0x158]
003b4268: mov      r1, r6
003b426c: mov      r0, #0x28
003b4270: ldr      r3, [r4, r3]
003b4274: mov      r7, #1
003b4278: ldr      r8, [r3, #0x44]
003b427c: bl       #0x310570
003b4280: mov      ip, #4
003b4284: mov      r1, r8
003b4288: mov      r2, r5
003b428c: str      ip, [sp]
003b4290: mvn      r3, #0
003b4294: movw     ip, #0xd7f
003b4298: mov      r6, r0
003b429c: str      ip, [sp, #4]
003b42a0: str      r7, [sp, #8]
003b42a4: bl       #0x3b4010
003b42a8: ldr      r3, [pc, #0x128]
003b42ac: mov      r0, r5
003b42b0: mov      r1, r6
003b42b4: ldr      r3, [r4, r3]
003b42b8: mov      r2, r7
003b42bc: add      r3, r3, #8
003b42c0: str      r3, [r6]
003b42c4: add      sp, sp, #0x24
003b42c8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b42cc: b        #0x394bf8
003b42d0: mov      r0, r5
003b42d4: bl       #0x3a30ac
003b42d8: subs     r6, r0, #0
003b42dc: bne      #0x3b4218
003b42e0: mov      r0, r5
003b42e4: bl       #0x3a3094
003b42e8: subs     r7, r0, #0
003b42ec: beq      #0x3b4354
003b42f0: ldr      r3, [pc, #0xcc]
003b42f4: mov      r1, r6
003b42f8: mov      r0, #0x28
003b42fc: ldr      r3, [r4, r3]
003b4300: ldr      r8, [r3, #0x44]
003b4304: bl       #0x310570
003b4308: mov      r1, r8
003b430c: mov      r3, r6
003b4310: mov      r2, r5
003b4314: mov      ip, #0x100
003b4318: mov      r7, r0
003b431c: str      ip, [sp]
003b4320: str      r6, [sp, #4]
003b4324: str      r6, [sp, #8]
003b4328: bl       #0x3b4010
003b432c: ldr      r3, [pc, #0x9c]
003b4330: mov      r0, r5
003b4334: mov      r1, r7
003b4338: ldr      r3, [r4, r3]
003b433c: mov      r2, #1
003b4340: add      r3, r3, #8
003b4344: str      r3, [r7]
003b4348: add      sp, sp, #0x24
003b434c: pop      {r4, r5, r6, r7, r8, sl, lr}
003b4350: b        #0x394bf8
003b4354: mov      r0, r5
003b4358: bl       #0x3a3064
003b435c: subs     r1, r0, #0
003b4360: beq      #0x3b43ac
003b4364: ldr      r3, [pc, #0x58]
003b4368: mov      r1, r7
003b436c: mov      r0, #0x28
003b4370: ldr      r3, [r4, r3]
003b4374: ldr      r8, [r3, #0x44]
003b4378: bl       #0x310570
003b437c: mov      ip, #0x10
003b4380: mov      r3, #2
003b4384: str      ip, [sp]
003b4388: mov      r1, r8
003b438c: movw     ip, #0xd3f
003b4390: mov      r2, r5
003b4394: mov      r6, r0
003b4398: str      ip, [sp, #4]
003b439c: str      r7, [sp, #8]
003b43a0: bl       #0x3b4010
003b43a4: ldr      r3, [pc, #0x30]
003b43a8: b        #0x3b4108
003b43ac: mov      r0, r5
003b43b0: mov      r2, r1
003b43b4: add      sp, sp, #0x24
003b43b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003b43bc: b        #0x394bf8
003b43c0: ldrsheq  r0, [lr], #-0x90
003b43c4: strdeq   r3, r4, [r0], -r4
003b43c8: andeq    r3, r0, ip, asr pc
003b43cc: andeq    r2, r0, r8, lsl r4
003b43d0: andeq    r0, r0, ip, asr #26
003b43d4: andeq    r3, r0, r4, lsr #4
003b43d8: andeq    r1, r0, ip, asr sp
003b43dc: andeq    r1, r0, r0, lsr #5

# _ZNK9Character5IsNPCEv
003a310c: push     {r4, lr}
003a3110: mov      r4, r0
003a3114: bl       #0x3a3054
003a3118: cmp      r0, #6
003a311c: beq      #0x3a3130
003a3120: mov      r0, r4
003a3124: bl       #0x3a30c4
003a3128: cmp      r0, #0
003a312c: beq      #0x3a3138
003a3130: mov      r0, #1
003a3134: pop      {r4, pc}
003a3138: mov      r0, r4
003a313c: pop      {r4, lr}
003a3140: b        #0x3a30f4

# _ZN12b2PolygonDef8SetAsBoxEff
007e44b8: add      ip, r1, #0x80000000
007e44bc: add      r3, r2, #0x80000000
007e44c0: str      r4, [sp, #-4]!
007e44c4: mov      r4, #4
007e44c8: str      r2, [r0, #0x3c]
007e44cc: str      r4, [r0, #0x60]
007e44d0: str      r3, [r0, #0x2c]
007e44d4: str      r1, [r0, #0x30]
007e44d8: str      ip, [r0, #0x38]
007e44dc: str      ip, [r0, #0x20]
007e44e0: str      r3, [r0, #0x24]
007e44e4: str      r1, [r0, #0x28]
007e44e8: str      r2, [r0, #0x34]
007e44ec: ldm      sp!, {r4}
007e44f0: bx       lr
