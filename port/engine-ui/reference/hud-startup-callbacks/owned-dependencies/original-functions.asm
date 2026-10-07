
# _ZN15SavegameManager13__loadOptionsEP11IStreamBasePv
0046d8f4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0046d8f8: ldr      sl, [pc, #0xb0]
0046d8fc: ldr      sb, [pc, #0xb0]
0046d900: sub      sp, sp, #0x90
0046d904: add      sl, pc, sl
0046d908: ldr      r3, [sl, sb]
0046d90c: add      r7, r1, #0x10
0046d910: add      r1, sp, #8
0046d914: ldr      r3, [r3]
0046d918: mov      r5, r0
0046d91c: str      r3, [sp, #0x8c]
0046d920: bl       #0x3df1a0
0046d924: ldr      r3, [sp, #8]
0046d928: cmp      r3, #0
0046d92c: beq      #0x46d990
0046d930: mov      r4, #0
0046d934: add      r6, sp, #0xc
0046d938: add      r8, sp, #4
0046d93c: b        #0x46d974
0046d940: mov      r0, r5
0046d944: mov      r1, r8
0046d948: bl       #0x459090
0046d94c: mov      r0, r7
0046d950: mov      r1, r6
0046d954: bl       #0x46d784
0046d958: cmp      r7, r0
0046d95c: ldrne    r3, [sp, #4]
0046d960: add      r4, r4, #1
0046d964: strne    r3, [r0, #0x2c]
0046d968: ldr      r3, [sp, #8]
0046d96c: cmp      r3, r4
0046d970: bls      #0x46d990
0046d974: mov      r0, r5
0046d978: mov      r1, r6
0046d97c: mov      r2, #0x80
0046d980: mov      r3, #0
0046d984: bl       #0x317734
0046d988: cmp      r0, #0
0046d98c: bne      #0x46d940
0046d990: ldr      r3, [sl, sb]
0046d994: ldr      r2, [sp, #0x8c]
0046d998: ldr      r3, [r3]
0046d99c: cmp      r2, r3
0046d9a0: bne      #0x46d9ac
0046d9a4: add      sp, sp, #0x90
0046d9a8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0046d9ac: bl       #0x30e310
0046d9b0: subseq   r7, r2, ip, lsl #3
0046d9b4: andeq    r4, r0, ip, lsr #1

# _ZN15SavegameManager28__loadLanguageAndOrientationEP11IStreamBasePv
0046c7b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046c7bc: ldr      r2, [pc, #0x1ec]
0046c7c0: ldr      r3, [pc, #0x1ec]
0046c7c4: sub      sp, sp, #0xa4
0046c7c8: add      r2, pc, r2
0046c7cc: str      r3, [sp, #0xc]
0046c7d0: ldr      r3, [r2, r3]
0046c7d4: subs     r6, r0, #0
0046c7d8: str      r2, [sp, #4]
0046c7dc: ldr      r3, [r3]
0046c7e0: mov      sb, r1
0046c7e4: str      r3, [sp, #0x9c]
0046c7e8: beq      #0x46c8cc
0046c7ec: add      r1, sp, #0x18
0046c7f0: bl       #0x3df1a0
0046c7f4: ldr      r3, [sp, #0x18]
0046c7f8: cmp      r3, #0
0046c7fc: beq      #0x46c8cc
0046c800: ldr      r3, [pc, #0x1b0]
0046c804: ldr      r7, [pc, #0x1b0]
0046c808: mov      r4, #0
0046c80c: add      r3, pc, r3
0046c810: add      r7, pc, r7
0046c814: str      r3, [sp, #8]
0046c818: mov      fp, r4
0046c81c: mov      r8, r4
0046c820: add      r5, sp, #0x1c
0046c824: add      sl, sp, #0x14
0046c828: b        #0x46c850
0046c82c: ldr      r3, [sp, #0x14]
0046c830: mov      r8, #1
0046c834: str      r3, [sb, #0x38]
0046c838: cmp      fp, #0
0046c83c: bne      #0x46c8cc
0046c840: ldr      r3, [sp, #0x18]
0046c844: add      r4, r4, #1
0046c848: cmp      r3, r4
0046c84c: bls      #0x46c8cc
0046c850: mov      r0, r6
0046c854: mov      r1, r5
0046c858: mov      r2, #0x80
0046c85c: mov      r3, #0
0046c860: bl       #0x317734
0046c864: cmp      r0, #0
0046c868: beq      #0x46c8cc
0046c86c: mov      r0, r6
0046c870: mov      r1, sl
0046c874: bl       #0x459090
0046c878: mov      r0, r7
0046c87c: mov      r1, r5
0046c880: bl       #0x30e31c
0046c884: cmp      r0, #0
0046c888: beq      #0x46c82c
0046c88c: ldr      r0, [sp, #8]
0046c890: mov      r1, r5
0046c894: bl       #0x30e31c
0046c898: cmp      r0, #0
0046c89c: bne      #0x46c8b4
0046c8a0: ldr      r3, [sp, #0x14]
0046c8a4: mov      fp, #1
0046c8a8: subs     r3, r3, #0
0046c8ac: movne    r3, #1
0046c8b0: strb     r3, [sb, #0x3c]
0046c8b4: cmp      r8, #0
0046c8b8: bne      #0x46c838
0046c8bc: ldr      r3, [sp, #0x18]
0046c8c0: add      r4, r4, #1
0046c8c4: cmp      r3, r4
0046c8c8: bhi      #0x46c850
0046c8cc: ldr      r3, [pc, #0xec]
0046c8d0: ldr      ip, [sp, #4]
0046c8d4: ldr      r3, [ip, r3]
0046c8d8: ldrb     r3, [r3]
0046c8dc: cmp      r3, #0
0046c8e0: bne      #0x46c958
0046c8e4: ldr      r3, [pc, #0xd8]
0046c8e8: ldr      r1, [sp, #4]
0046c8ec: ldr      r3, [r1, r3]
0046c8f0: ldrb     r3, [r3]
0046c8f4: cmp      r3, #0
0046c8f8: beq      #0x46c928
0046c8fc: mov      r3, #4
0046c900: str      r3, [sb, #0x38]
0046c904: ldr      r2, [sp, #0xc]
0046c908: ldr      ip, [sp, #4]
0046c90c: ldr      r3, [ip, r2]
0046c910: ldr      r2, [sp, #0x9c]
0046c914: ldr      r3, [r3]
0046c918: cmp      r2, r3
0046c91c: bne      #0x46c9ac
0046c920: add      sp, sp, #0xa4
0046c924: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046c928: bl       #0x531ab0
0046c92c: cmp      r0, #7
0046c930: addls    pc, pc, r0, lsl #2
0046c934: b        #0x46c964
0046c938: b        #0x46c964
0046c93c: b        #0x46c988
0046c940: b        #0x46c994
0046c944: b        #0x46c9a0
0046c948: b        #0x46c970
0046c94c: b        #0x46c8fc
0046c950: b        #0x46c958
0046c954: b        #0x46c97c
0046c958: mov      r3, #5
0046c95c: str      r3, [sb, #0x38]
0046c960: b        #0x46c904
0046c964: mov      r3, #0
0046c968: str      r3, [sb, #0x38]
0046c96c: b        #0x46c904
0046c970: mov      r3, #3
0046c974: str      r3, [sb, #0x38]
0046c978: b        #0x46c904
0046c97c: mov      r3, #6
0046c980: str      r3, [sb, #0x38]
0046c984: b        #0x46c904
0046c988: mov      r3, #2
0046c98c: str      r3, [sb, #0x38]
0046c990: b        #0x46c904
0046c994: mov      r3, #1
0046c998: str      r3, [sb, #0x38]
0046c99c: b        #0x46c904
0046c9a0: mov      r3, #7
0046c9a4: str      r3, [sb, #0x38]
0046c9a8: b        #0x46c904
0046c9ac: bl       #0x30e310
0046c9b0: subseq   r8, r2, r8, asr #5
0046c9b4: andeq    r4, r0, ip, lsr #1
0046c9b8: subeq    r5, r5, r4, asr #14
0046c9bc: subeq    pc, r5, r8, asr #6
0046c9c0: andeq    r3, r0, ip, lsr #31
0046c9c4: andeq    r4, r0, r4, lsl r7

# _ZNK15SavegameManager9hasOptionEPKc
0046d4a8: push     {r4, lr}
0046d4ac: sub      sp, sp, #8
0046d4b0: add      r3, sp, #8
0046d4b4: str      r1, [r3, #-4]!
0046d4b8: add      r4, r0, #0x10
0046d4bc: mov      r1, r3
0046d4c0: mov      r0, r4
0046d4c4: bl       #0x46ce64
0046d4c8: subs     r0, r4, r0
0046d4cc: movne    r0, #1
0046d4d0: add      sp, sp, #8
0046d4d4: pop      {r4, pc}

# _ZNK15SavegameManager9getOptionEPKc
0046d474: push     {r4, lr}
0046d478: sub      sp, sp, #8
0046d47c: add      r3, sp, #8
0046d480: str      r1, [r3, #-4]!
0046d484: add      r4, r0, #0x10
0046d488: mov      r1, r3
0046d48c: mov      r0, r4
0046d490: bl       #0x46ce64
0046d494: cmp      r0, r4
0046d498: mvneq    r0, #0
0046d49c: ldrne    r0, [r0, #0x2c]
0046d4a0: add      sp, sp, #8
0046d4a4: pop      {r4, pc}

# _ZN8SavegameC1EPKcb
00315ed8: ldr      r3, [pc, #0x60]
00315edc: ldr      ip, [pc, #0x60]
00315ee0: push     {r4, r5, lr}
00315ee4: add      r3, pc, r3
00315ee8: ldr      ip, [r3, ip]
00315eec: sub      sp, sp, #0xc
00315ef0: mov      r4, r0
00315ef4: add      ip, ip, #8
00315ef8: mov      r5, r2
00315efc: str      ip, [r0], #4
00315f00: add      r2, sp, #4
00315f04: bl       #0x3140ec
00315f08: mov      r1, #0
00315f0c: mov      r3, r4
00315f10: str      r1, [r4, #0x1c]
00315f14: str      r1, [r4, #0x24]
00315f18: strb     r1, [r3, #0x20]!
00315f1c: mov      r0, r4
00315f20: str      r3, [r4, #0x2c]
00315f24: strb     r5, [r4, #0x38]
00315f28: str      r3, [r4, #0x28]
00315f2c: str      r1, [r4, #0x30]
00315f30: bl       #0x315ad0
00315f34: mov      r0, r4
00315f38: add      sp, sp, #0xc
00315f3c: pop      {r4, r5, pc}
00315f40: rsbeq    lr, r7, ip, lsr #23
00315f44: strheq   r0, [r0], -r8

# _ZN11Application16ResetOrientationEi
0031f748: bx       lr

# _Z19NativeGetPlayerCharib
0043c388: ldr      r3, [pc, #0x6c]
0043c38c: ldr      r2, [pc, #0x6c]
0043c390: push     {r4, r5, r6, lr}
0043c394: add      r3, pc, r3
0043c398: ldr      r2, [r3, r2]
0043c39c: subs     r4, r0, #0
0043c3a0: mov      r5, r1
0043c3a4: ldr      r6, [r2, #0x40]
0043c3a8: blt      #0x43c3bc
0043c3ac: mov      r0, r6
0043c3b0: bl       #0x36d7a8
0043c3b4: cmp      r4, r0
0043c3b8: blt      #0x43c3c4
0043c3bc: mov      r0, #0
0043c3c0: pop      {r4, r5, r6, pc}
0043c3c4: cmp      r5, #0
0043c3c8: bne      #0x43c3e4
0043c3cc: mov      r0, r6
0043c3d0: mov      r1, r4
0043c3d4: mov      r2, r5
0043c3d8: bl       #0x36e478
0043c3dc: ldr      r0, [r0, #0x660]
0043c3e0: pop      {r4, r5, r6, pc}
0043c3e4: mov      r0, r6
0043c3e8: mov      r1, r4
0043c3ec: mov      r2, #0
0043c3f0: bl       #0x36e2ac
0043c3f4: ldr      r0, [r0, #0x660]
0043c3f8: pop      {r4, r5, r6, pc}
0043c3fc: ldrsheq  r8, [r5], #-0x6c
0043c400: strdeq   r3, r4, [r0], -r4

# _ZN15SavegameManager13_initSettingsEb
0046e47c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046e480: ldr      r3, [r0, #0x20]
0046e484: ldr      r8, [pc, #0xe8]
0046e488: sub      sp, sp, #0x14
0046e48c: cmp      r3, #0
0046e490: mov      fp, r0
0046e494: mov      r4, r1
0046e498: add      r8, pc, r8
0046e49c: bne      #0x46e54c
0046e4a0: cmp      r4, #0
0046e4a4: bne      #0x46e528
0046e4a8: ldr      r1, [pc, #0xc8]
0046e4ac: ldr      r3, [r8, r1]
0046e4b0: str      r1, [sp, #4]
0046e4b4: ldr      r3, [r3]
0046e4b8: cmp      r3, #0
0046e4bc: beq      #0x46e528
0046e4c0: ldr      r3, [pc, #0xb4]
0046e4c4: add      r7, fp, #0x10
0046e4c8: add      r6, sp, #0xc
0046e4cc: ldr      sb, [r8, r3]
0046e4d0: ldr      r3, [pc, #0xa8]
0046e4d4: ldr      sl, [r8, r3]
0046e4d8: ldr      r3, [sl]
0046e4dc: mov      r1, r6
0046e4e0: mov      r0, r7
0046e4e4: ldr      r3, [r3, r4, lsl #2]
0046e4e8: ldr      r5, [sb]
0046e4ec: str      r3, [sp, #0xc]
0046e4f0: bl       #0x46e338
0046e4f4: add      r5, r5, r4, lsl #5
0046e4f8: str      r5, [r0]
0046e4fc: mov      r1, r6
0046e500: mov      r0, r7
0046e504: bl       #0x46e338
0046e508: ldr      r1, [sp, #4]
0046e50c: ldr      r2, [r5, #4]
0046e510: add      r4, r4, #1
0046e514: ldr      r3, [r8, r1]
0046e518: str      r2, [r0, #4]
0046e51c: ldr      r3, [r3]
0046e520: cmp      r3, r4
0046e524: bhi      #0x46e4d8
0046e528: mov      r3, #0
0046e52c: mov      r2, #1
0046e530: add      r3, r3, #1
0046e534: cmp      r3, #0xe
0046e538: strb     r2, [fp, #0x29]
0046e53c: add      fp, fp, #1
0046e540: bne      #0x46e530
0046e544: add      sp, sp, #0x14
0046e548: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046e54c: add      r5, r0, #0x10
0046e550: mov      r0, r5
0046e554: ldr      r1, [fp, #0x14]
0046e558: bl       #0x46cbd8
0046e55c: mov      r3, #0
0046e560: str      r5, [fp, #0x1c]
0046e564: str      r3, [fp, #0x20]
0046e568: str      r5, [fp, #0x18]
0046e56c: str      r3, [fp, #0x14]
0046e570: b        #0x46e4a0
0046e574: ldrsheq  r6, [r2], #-0x58
0046e578: andeq    r3, r0, r0, ror #10
0046e57c: andeq    r1, r0, ip, ror sl
0046e580: andeq    r1, r0, ip, lsr pc

# _ZN15SavegameManager15__loadTutorialsEP11IStreamBasePv
0046c778: push     {r4, lr}
0046c77c: mov      r2, #0xe
0046c780: ldr      ip, [r0]
0046c784: mov      r3, #0
0046c788: add      r1, r1, #0x29
0046c78c: mov      lr, pc
0046c790: ldr      pc, [ip, #0x18]
0046c794: pop      {r4, pc}

# _ZNK13ItemInventory13GetNumPotionsEv
003fc690: ldr      r0, [r0, #0x24]
003fc694: cmp      r0, #0
003fc698: ldrshne  r0, [r0, #0x50]
003fc69c: bx       lr
