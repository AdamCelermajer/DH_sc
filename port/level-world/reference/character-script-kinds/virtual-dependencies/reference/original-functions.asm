
# _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE
0037c41c: ldr      r3, [pc, #0x68]
0037c420: ldr      ip, [pc, #0x68]
0037c424: push     {r4, r5, r6, r7, r8, lr}
0037c428: add      r3, pc, r3
0037c42c: ldr      r5, [r3, ip]
0037c430: sub      sp, sp, #0x30
0037c434: mov      r8, r1
0037c438: ldr      r1, [r5]
0037c43c: add      r4, sp, #4
0037c440: mov      r6, r0
0037c444: mov      r7, r2
0037c448: mov      r0, r4
0037c44c: str      r1, [sp, #0x2c]
0037c450: bl       #0x31b434
0037c454: mov      r2, r7
0037c458: mov      r3, r4
0037c45c: mov      r0, r6
0037c460: mov      r1, r8
0037c464: bl       #0x37c390
0037c468: mov      r0, r4
0037c46c: bl       #0x31b398
0037c470: ldr      r2, [sp, #0x2c]
0037c474: ldr      r3, [r5]
0037c478: cmp      r2, r3
0037c47c: bne      #0x37c488
0037c480: add      sp, sp, #0x30
0037c484: pop      {r4, r5, r6, r7, r8, pc}
0037c488: bl       #0x30e310
0037c48c: rsbeq    r8, r1, r8, ror #12
0037c490: andeq    r4, r0, ip, lsr #1

# _ZN3sfc6script3lua9Arguments12pushUserDataEPNS1_8UserDataE
00386f28: ldr      r3, [pc, #0x58]
00386f2c: ldr      r2, [pc, #0x58]
00386f30: push     {r4, r5, r6, lr}
00386f34: add      r3, pc, r3
00386f38: ldr      r5, [r3, r2]
00386f3c: sub      sp, sp, #0x78
00386f40: add      r4, sp, #4
00386f44: ldr      r3, [r5]
00386f48: str      r3, [sp, #0x74]
00386f4c: ldr      r6, [r0, #4]
00386f50: mov      r0, r4
00386f54: bl       #0x37c978
00386f58: mov      r0, r6
00386f5c: mov      r1, r4
00386f60: bl       #0x3195c0
00386f64: mov      r0, r4
00386f68: bl       #0x3193e8
00386f6c: ldr      r2, [sp, #0x74]
00386f70: ldr      r3, [r5]
00386f74: cmp      r2, r3
00386f78: bne      #0x386f84
00386f7c: add      sp, sp, #0x78
00386f80: pop      {r4, r5, r6, pc}
00386f84: bl       #0x30e310
00386f88: rsbeq    sp, r0, ip, asr fp
00386f8c: andeq    r4, r0, ip, lsr #1

# _ZN9LuaScript4CallEPKc
0037c514: ldr      r3, [pc, #0x60]
0037c518: ldr      r2, [pc, #0x60]
0037c51c: push     {r4, r5, r6, r7, lr}
0037c520: add      r3, pc, r3
0037c524: ldr      r5, [r3, r2]
0037c528: sub      sp, sp, #0x34
0037c52c: add      r4, sp, #4
0037c530: ldr      r2, [r5]
0037c534: mov      r6, r0
0037c538: mov      r7, r1
0037c53c: mov      r0, r4
0037c540: str      r2, [sp, #0x2c]
0037c544: bl       #0x31b434
0037c548: mov      r2, r4
0037c54c: mov      r0, r6
0037c550: mov      r1, r7
0037c554: bl       #0x37c494
0037c558: mov      r0, r4
0037c55c: bl       #0x31b398
0037c560: ldr      r2, [sp, #0x2c]
0037c564: ldr      r3, [r5]
0037c568: cmp      r2, r3
0037c56c: bne      #0x37c578
0037c570: add      sp, sp, #0x34
0037c574: pop      {r4, r5, r6, r7, pc}
0037c578: bl       #0x30e310
0037c57c: rsbeq    r8, r1, r0, ror r5
0037c580: andeq    r4, r0, ip, lsr #1

# _ZNK9LuaScript11IsInVFTableEPKc
0037c2a0: push     {r4, lr}
0037c2a4: mov      r4, r0
0037c2a8: mov      r0, r1
0037c2ac: bl       #0x37c164
0037c2b0: ldr      r3, [r4, #0x38]
0037c2b4: add      r4, r4, #0x34
0037c2b8: cmp      r3, #0
0037c2bc: beq      #0x37c300
0037c2c0: mov      r1, r4
0037c2c4: b        #0x37c2cc
0037c2c8: mov      r3, r2
0037c2cc: ldr      r2, [r3, #0x10]
0037c2d0: cmp      r0, r2
0037c2d4: ldrhi    r2, [r3, #0xc]
0037c2d8: ldrls    r2, [r3, #8]
0037c2dc: movhi    r3, r1
0037c2e0: mov      r1, r3
0037c2e4: cmp      r2, #0
0037c2e8: bne      #0x37c2c8
0037c2ec: cmp      r4, r3
0037c2f0: beq      #0x37c300
0037c2f4: ldr      r2, [r3, #0x10]
0037c2f8: cmp      r0, r2
0037c2fc: bhs      #0x37c308
0037c300: mov      r0, #0
0037c304: pop      {r4, pc}
0037c308: subs     r0, r3, r4
0037c30c: movne    r0, #1
0037c310: pop      {r4, pc}

# _ZN12CharAIScript19CallStateConditionsEv
003d8ea0: ldr      r3, [r0, #0xb4]
003d8ea4: cmp      r3, #0
003d8ea8: bxeq     lr
003d8eac: ldr      r1, [r3, #0x2c]
003d8eb0: b        #0x37c514

# _ZN12CharAIScript15CallStateUpdateEv
003d8eb4: ldr      r3, [r0, #0xb4]
003d8eb8: cmp      r3, #0
003d8ebc: bxeq     lr
003d8ec0: ldr      r1, [r3, #0x14]
003d8ec4: b        #0x37c514
