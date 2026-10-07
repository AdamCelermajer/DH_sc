
# _ZN6Arrays15GameOptionTable4readEP11IStreamBase
004bc8e0: push     {r4, r5, r6, r7, r8, lr}
004bc8e4: sub      sp, sp, #8
004bc8e8: mov      r8, r0
004bc8ec: bl       #0x313a90
004bc8f0: ldr      r5, [pc, #0x110]
004bc8f4: mov      r3, #1
004bc8f8: cmp      r3, #0
004bc8fc: str      r0, [sp, #4]
004bc900: str      r3, [sp]
004bc904: add      r5, pc, r5
004bc908: bne      #0x4bc950
004bc90c: add      r3, sp, #4
004bc910: add      r2, r3, #2
004bc914: add      r3, r3, #1
004bc918: ldrb     r0, [r2, #1]
004bc91c: ldrb     r1, [r3, #-1]
004bc920: cmp      r2, r3
004bc924: eor      r1, r0, r1
004bc928: strb     r1, [r3, #-1]
004bc92c: ldrb     r0, [r2, #1]
004bc930: eor      r1, r1, r0
004bc934: strb     r1, [r2, #1]
004bc938: ldrb     r0, [r3, #-1]
004bc93c: sub      r2, r2, #1
004bc940: eor      r1, r1, r0
004bc944: strb     r1, [r3, #-1]
004bc948: add      r3, r3, #1
004bc94c: bhi      #0x4bc918
004bc950: ldr      r6, [pc, #0xb4]
004bc954: bl       #0x4a8dc4
004bc958: ldr      r4, [sp, #4]
004bc95c: ldr      r3, [r5, r6]
004bc960: mov      r1, #1
004bc964: lsl      r0, r4, #5
004bc968: str      r4, [r3]
004bc96c: add      r0, r0, #8
004bc970: bl       #0x31056c
004bc974: mov      r3, #0x20
004bc978: cmp      r4, #0
004bc97c: stm      r0, {r3, r4}
004bc980: add      r3, r0, #8
004bc984: beq      #0x4bc9ac
004bc988: ldr      r1, [pc, #0x80]
004bc98c: mov      r2, #0
004bc990: ldr      r1, [r5, r1]
004bc994: add      r1, r1, #8
004bc998: add      r2, r2, #1
004bc99c: cmp      r2, r4
004bc9a0: str      r1, [r0, #8]
004bc9a4: add      r0, r0, #0x20
004bc9a8: bne      #0x4bc998
004bc9ac: ldr      r2, [r5, r6]
004bc9b0: ldr      r7, [pc, #0x5c]
004bc9b4: ldr      r1, [r2]
004bc9b8: ldr      r2, [r5, r7]
004bc9bc: cmp      r1, #0
004bc9c0: str      r3, [r2]
004bc9c4: beq      #0x4bca00
004bc9c8: mov      r4, #0
004bc9cc: b        #0x4bc9d8
004bc9d0: ldr      r3, [r5, r7]
004bc9d4: ldr      r3, [r3]
004bc9d8: add      r0, r3, r4, lsl #5
004bc9dc: mov      r1, r8
004bc9e0: ldr      r3, [r3, r4, lsl #5]
004bc9e4: mov      lr, pc
004bc9e8: ldr      pc, [r3, #0xc]
004bc9ec: ldr      r3, [r5, r6]
004bc9f0: add      r4, r4, #1
004bc9f4: ldr      r3, [r3]
004bc9f8: cmp      r3, r4
004bc9fc: bhi      #0x4bc9d0
004bca00: add      sp, sp, #8
004bca04: pop      {r4, r5, r6, r7, r8, pc}
004bca08: subeq    r8, sp, ip, lsl #3
004bca0c: andeq    r3, r0, r0, ror #10
004bca10: andeq    r0, r0, r4, ror #22
004bca14: andeq    r1, r0, ip, ror sl

# _Z17UpdateSavedValuesv
0043a8f0: ldr      r3, [pc, #0x2c]
0043a8f4: ldr      r2, [pc, #0x2c]
0043a8f8: push     {r4, lr}
0043a8fc: add      r3, pc, r3
0043a900: ldr      r4, [r3, r2]
0043a904: ldr      r1, [pc, #0x20]
0043a908: mov      r0, r4
0043a90c: add      r1, pc, r1
0043a910: bl       #0x320e44
0043a914: mov      r1, r0
0043a918: mov      r0, r4
0043a91c: pop      {r4, lr}
0043a920: b        #0x31f748

# _ZN11Application17GetDeviceLanguageEv
0031f75c: mvn      r0, #0
0031f760: bx       lr

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

# _ZN9Character27UpdateInventoryLocalizationEv
003b36e4: add      r0, r0, #0x37c
003b36e8: b        #0x3fdfa0

# _ZN15SavegameManager11setLanguageEi
0046d104: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0046d108: mov      r6, r1
0046d10c: ldr      r4, [pc, #0x160]
0046d110: ldr      r1, [pc, #0x160]
0046d114: ldr      r5, [pc, #0x160]
0046d118: add      r4, pc, r4
0046d11c: add      r1, pc, r1
0046d120: mov      r2, r6
0046d124: bl       #0x46d0d0
0046d128: ldr      r3, [r4, r5]
0046d12c: ldr      sl, [r3, #0x38]
0046d130: ldr      r7, [sl, #0x60]
0046d134: add      sl, sl, #0x60
0046d138: cmp      sl, r7
0046d13c: beq      #0x46d170
0046d140: ldr      r8, [r7, #8]
0046d144: ldr      r3, [r8]
0046d148: mov      r0, r8
0046d14c: mov      lr, pc
0046d150: ldr      pc, [r3, #0x28]
0046d154: cmp      r0, #0
0046d158: beq      #0x46d208
0046d15c: mov      r0, r8
0046d160: bl       #0x3b36e4
0046d164: ldr      r7, [r7]
0046d168: cmp      sl, r7
0046d16c: bne      #0x46d140
0046d170: ldr      r3, [r4, r5]
0046d174: mov      sb, #0
0046d178: ldr      sl, [r3, #0x38]
0046d17c: ldr      r7, [sl, #0x14]
0046d180: add      sl, sl, #0xc
0046d184: cmp      sl, r7
0046d188: beq      #0x46d1f0
0046d18c: ldr      r8, [r7, #0x2c]
0046d190: cmp      r8, #0
0046d194: beq      #0x46d1c4
0046d198: ldr      r3, [r8]
0046d19c: mov      r0, r8
0046d1a0: mov      lr, pc
0046d1a4: ldr      pc, [r3, #0x20]
0046d1a8: cmp      r0, #0
0046d1ac: beq      #0x46d1c4
0046d1b0: ldr      r3, [r8, #0xf4]
0046d1b4: cmp      r3, #3
0046d1b8: beq      #0x46d264
0046d1bc: cmp      r3, #0xe
0046d1c0: beq      #0x46d220
0046d1c4: ldr      r2, [r7, #0xc]
0046d1c8: cmp      r2, #0
0046d1cc: bne      #0x46d1d8
0046d1d0: b        #0x46d230
0046d1d4: mov      r2, r3
0046d1d8: ldr      r3, [r2, #8]
0046d1dc: cmp      r3, #0
0046d1e0: bne      #0x46d1d4
0046d1e4: mov      r7, r2
0046d1e8: cmp      sl, r7
0046d1ec: bne      #0x46d18c
0046d1f0: ldr      r3, [r4, r5]
0046d1f4: mov      r1, r6
0046d1f8: mov      r2, #1
0046d1fc: ldr      r0, [r3, #0x34]
0046d200: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0046d204: b        #0x507a94
0046d208: mov      r0, r8
0046d20c: bl       #0x3a30c4
0046d210: cmp      r0, #0
0046d214: bne      #0x46d15c
0046d218: ldr      r7, [r7]
0046d21c: b        #0x46d168
0046d220: strb     sb, [r8, #0x819]
0046d224: ldr      r2, [r7, #0xc]
0046d228: cmp      r2, #0
0046d22c: bne      #0x46d1d8
0046d230: ldr      r3, [r7, #4]
0046d234: ldr      r1, [r3, #0xc]
0046d238: cmp      r7, r1
0046d23c: bne      #0x46d258
0046d240: mov      r7, r3
0046d244: ldr      r3, [r3, #4]
0046d248: ldr      r2, [r3, #0xc]
0046d24c: cmp      r2, r7
0046d250: beq      #0x46d240
0046d254: ldr      r2, [r7, #0xc]
0046d258: cmp      r2, r3
0046d25c: movne    r7, r3
0046d260: b        #0x46d184
0046d264: mov      r0, r8
0046d268: bl       #0x3ebca8
0046d26c: ldr      r3, [r8, #0xf4]
0046d270: b        #0x46d1bc
0046d274: subseq   r7, r2, r8, ror sb
0046d278: subeq    lr, r5, ip, lsr sl
0046d27c: strdeq   r3, r4, [r0], -r4

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

# _ZNK9Character10IsMerchantEv
003a30c4: push     {r4, lr}
003a30c8: bl       #0x3a3054
003a30cc: cmp      r0, #7
003a30d0: movne    r0, #0
003a30d4: moveq    r0, #1
003a30d8: pop      {r4, pc}

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

# _ZN7Structs10GameOption4readEP11IStreamBase
004f213c: push     {r4, r5, lr}
004f2140: mov      r4, r0
004f2144: sub      sp, sp, #0xc
004f2148: mov      r0, r1
004f214c: mov      r5, r1
004f2150: add      r1, r4, #4
004f2154: bl       #0x459090
004f2158: mov      r3, #1
004f215c: cmp      r3, #0
004f2160: str      r3, [sp, #4]
004f2164: bne      #0x4f21a8
004f2168: add      r3, r4, #5
004f216c: add      r2, r4, #6
004f2170: ldrb     r0, [r2, #1]
004f2174: ldrb     r1, [r3, #-1]
004f2178: cmp      r3, r2
004f217c: eor      r1, r0, r1
004f2180: strb     r1, [r3, #-1]
004f2184: ldrb     r0, [r2, #1]
004f2188: eor      r1, r1, r0
004f218c: strb     r1, [r2, #1]
004f2190: ldrb     r0, [r3, #-1]
004f2194: sub      r2, r2, #1
004f2198: eor      r1, r1, r0
004f219c: strb     r1, [r3, #-1]
004f21a0: add      r3, r3, #1
004f21a4: blo      #0x4f2170
004f21a8: mov      r0, r5
004f21ac: add      r1, r4, #8
004f21b0: bl       #0x459090
004f21b4: mov      r3, #1
004f21b8: cmp      r3, #0
004f21bc: str      r3, [sp, #4]
004f21c0: bne      #0x4f2204
004f21c4: add      r3, r4, #9
004f21c8: add      r2, r4, #0xa
004f21cc: ldrb     r0, [r2, #1]
004f21d0: ldrb     r1, [r3, #-1]
004f21d4: cmp      r3, r2
004f21d8: eor      r1, r0, r1
004f21dc: strb     r1, [r3, #-1]
004f21e0: ldrb     r0, [r2, #1]
004f21e4: eor      r1, r1, r0
004f21e8: strb     r1, [r2, #1]
004f21ec: ldrb     r0, [r3, #-1]
004f21f0: sub      r2, r2, #1
004f21f4: eor      r1, r1, r0
004f21f8: strb     r1, [r3, #-1]
004f21fc: add      r3, r3, #1
004f2200: blo      #0x4f21cc
004f2204: mov      r0, r5
004f2208: add      r1, r4, #0xc
004f220c: bl       #0x459090
004f2210: mov      r3, #1
004f2214: cmp      r3, #0
004f2218: str      r3, [sp, #4]
004f221c: bne      #0x4f2260
004f2220: add      r3, r4, #0xd
004f2224: add      r2, r4, #0xe
004f2228: ldrb     r0, [r2, #1]
004f222c: ldrb     r1, [r3, #-1]
004f2230: cmp      r3, r2
004f2234: eor      r1, r0, r1
004f2238: strb     r1, [r3, #-1]
004f223c: ldrb     r0, [r2, #1]
004f2240: eor      r1, r1, r0
004f2244: strb     r1, [r2, #1]
004f2248: ldrb     r0, [r3, #-1]
004f224c: sub      r2, r2, #1
004f2250: eor      r1, r1, r0
004f2254: strb     r1, [r3, #-1]
004f2258: add      r3, r3, #1
004f225c: blo      #0x4f2228
004f2260: mov      r0, r5
004f2264: add      r1, r4, #0x10
004f2268: bl       #0x459090
004f226c: mov      r3, #1
004f2270: cmp      r3, #0
004f2274: str      r3, [sp, #4]
004f2278: bne      #0x4f22bc
004f227c: add      r3, r4, #0x11
004f2280: add      r2, r4, #0x12
004f2284: ldrb     r0, [r2, #1]
004f2288: ldrb     r1, [r3, #-1]
004f228c: cmp      r3, r2
004f2290: eor      r1, r0, r1
004f2294: strb     r1, [r3, #-1]
004f2298: ldrb     r0, [r2, #1]
004f229c: eor      r1, r1, r0
004f22a0: strb     r1, [r2, #1]
004f22a4: ldrb     r0, [r3, #-1]
004f22a8: sub      r2, r2, #1
004f22ac: eor      r1, r1, r0
004f22b0: strb     r1, [r3, #-1]
004f22b4: add      r3, r3, #1
004f22b8: blo      #0x4f2284
004f22bc: mov      r0, r5
004f22c0: add      r1, r4, #0x14
004f22c4: bl       #0x459090
004f22c8: mov      r3, #1
004f22cc: cmp      r3, #0
004f22d0: str      r3, [sp, #4]
004f22d4: bne      #0x4f2318
004f22d8: add      r3, r4, #0x15
004f22dc: add      r2, r4, #0x16
004f22e0: ldrb     r0, [r2, #1]
004f22e4: ldrb     r1, [r3, #-1]
004f22e8: cmp      r3, r2
004f22ec: eor      r1, r0, r1
004f22f0: strb     r1, [r3, #-1]
004f22f4: ldrb     r0, [r2, #1]
004f22f8: eor      r1, r1, r0
004f22fc: strb     r1, [r2, #1]
004f2300: ldrb     r0, [r3, #-1]
004f2304: sub      r2, r2, #1
004f2308: eor      r1, r1, r0
004f230c: strb     r1, [r3, #-1]
004f2310: add      r3, r3, #1
004f2314: blo      #0x4f22e0
004f2318: mov      r0, r5
004f231c: add      r1, r4, #0x18
004f2320: bl       #0x459090
004f2324: mov      r3, #1
004f2328: cmp      r3, #0
004f232c: str      r3, [sp, #4]
004f2330: bne      #0x4f2374
004f2334: add      r3, r4, #0x19
004f2338: add      r2, r4, #0x1a
004f233c: ldrb     r0, [r2, #1]
004f2340: ldrb     r1, [r3, #-1]
004f2344: cmp      r3, r2
004f2348: eor      r1, r0, r1
004f234c: strb     r1, [r3, #-1]
004f2350: ldrb     r0, [r2, #1]
004f2354: eor      r1, r1, r0
004f2358: strb     r1, [r2, #1]
004f235c: ldrb     r0, [r3, #-1]
004f2360: sub      r2, r2, #1
004f2364: eor      r1, r1, r0
004f2368: strb     r1, [r3, #-1]
004f236c: add      r3, r3, #1
004f2370: blo      #0x4f233c
004f2374: mov      r0, r5
004f2378: add      r1, r4, #0x1c
004f237c: bl       #0x459090
004f2380: mov      r3, #1
004f2384: cmp      r3, #0
004f2388: str      r3, [sp, #4]
004f238c: bne      #0x4f23d0
004f2390: add      r3, r4, #0x1e
004f2394: add      r4, r4, #0x1d
004f2398: ldrb     r1, [r3, #1]
004f239c: ldrb     r2, [r4, #-1]
004f23a0: cmp      r4, r3
004f23a4: eor      r2, r1, r2
004f23a8: strb     r2, [r4, #-1]
004f23ac: ldrb     r1, [r3, #1]
004f23b0: eor      r2, r2, r1
004f23b4: strb     r2, [r3, #1]
004f23b8: ldrb     r1, [r4, #-1]
004f23bc: sub      r3, r3, #1
004f23c0: eor      r2, r2, r1
004f23c4: strb     r2, [r4, #-1]
004f23c8: add      r4, r4, #1
004f23cc: blo      #0x4f2398
004f23d0: add      sp, sp, #0xc
004f23d4: pop      {r4, r5, pc}

# _ZN15SavegameManager9setOptionEPKci
0046d0d0: push     {r4, r5, lr}
0046d0d4: sub      sp, sp, #0xc
0046d0d8: add      r3, sp, #8
0046d0dc: str      r1, [r3, #-4]!
0046d0e0: add      r4, r0, #0x10
0046d0e4: mov      r1, r3
0046d0e8: mov      r0, r4
0046d0ec: mov      r5, r2
0046d0f0: bl       #0x46ce64
0046d0f4: cmp      r0, r4
0046d0f8: strne    r5, [r0, #0x2c]
0046d0fc: add      sp, sp, #0xc
0046d100: pop      {r4, r5, pc}

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

# _ZN12StreamReader12readStringExEP11IStreamBasePcy
00317454: push     {r4, r5, r6, lr}
00317458: ldr      ip, [r0]
0031745c: mov      r4, r2
00317460: mov      r5, r3
00317464: mov      lr, pc
00317468: ldr      pc, [ip, #0x18]
0031746c: cmp      r0, r4
00317470: mov      r0, #0
00317474: beq      #0x317480
00317478: and      r0, r0, #1
0031747c: pop      {r4, r5, r6, pc}
00317480: cmp      r1, r5
00317484: moveq    r0, #1
00317488: and      r0, r0, #1
0031748c: pop      {r4, r5, r6, pc}

# _ZNK9Character8IsPlayerEv
003a49f0: push     {r4, r5, r6, lr}
003a49f4: mov      r5, r0
003a49f8: bl       #0x3a3054
003a49fc: cmp      r0, #0
003a4a00: beq      #0x3a4a14
003a4a04: cmp      r0, #1
003a4a08: movne    r0, #0
003a4a0c: moveq    r0, #1
003a4a10: pop      {r4, r5, r6, pc}
003a4a14: ldr      r4, [r5, #0x44]
003a4a18: ldr      r1, [pc, #0x18]
003a4a1c: mov      r0, r4
003a4a20: add      r1, pc, r1
003a4a24: bl       #0x30ebd4
003a4a28: cmp      r4, r0
003a4a2c: movne    r0, #0
003a4a30: moveq    r0, #1
003a4a34: pop      {r4, r5, r6, pc}
003a4a38: subseq   lr, r1, r8, asr #14

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

# _ZN10ItemObject18UpdateLocalizationEv
003ebca8: push     {r4, lr}
003ebcac: add      r0, r0, #0x374
003ebcb0: mov      r1, #0
003ebcb4: bl       #0x3fc61c
003ebcb8: cmp      r0, #0
003ebcbc: beq      #0x3ebcc8
003ebcc0: pop      {r4, lr}
003ebcc4: b        #0x3fc1b4
003ebcc8: pop      {r4, pc}

# _ZN12StreamReader10readStringEP11IStreamBasePcy
00317734: push     {r4, r5, r6, r7, r8, sb, lr}
00317738: sub      sp, sp, #0xc
0031773c: mov      sb, r3
00317740: mov      r6, r1
00317744: mov      r7, r0
00317748: mov      r8, r2
0031774c: bl       #0x313a90
00317750: mov      r3, #1
00317754: cmp      r3, #0
00317758: mov      r4, r0
0031775c: str      r0, [sp, #4]
00317760: str      r3, [sp]
00317764: bne      #0x3177b0
00317768: add      r3, sp, #4
0031776c: add      r2, r3, #2
00317770: add      r3, r3, #1
00317774: ldrb     r0, [r2, #1]
00317778: ldrb     r1, [r3, #-1]
0031777c: cmp      r3, r2
00317780: eor      r1, r0, r1
00317784: strb     r1, [r3, #-1]
00317788: ldrb     r0, [r2, #1]
0031778c: eor      r1, r1, r0
00317790: strb     r1, [r2, #1]
00317794: ldrb     r0, [r3, #-1]
00317798: sub      r2, r2, #1
0031779c: eor      r1, r1, r0
003177a0: strb     r1, [r3, #-1]
003177a4: add      r3, r3, #1
003177a8: blo      #0x317774
003177ac: ldr      r4, [sp, #4]
003177b0: mvn      r0, #0
003177b4: adds     r0, r0, r8
003177b8: mvn      r1, #0
003177bc: adc      r1, r1, sb
003177c0: mov      r3, #0
003177c4: cmp      r3, r1
003177c8: mov      r5, r4
003177cc: beq      #0x317818
003177d0: mov      r0, r7
003177d4: ldr      ip, [r7]
003177d8: mov      r1, r6
003177dc: mov      r2, r5
003177e0: mov      lr, pc
003177e4: ldr      pc, [ip, #0x18]
003177e8: mov      r0, #0
003177ec: cmp      sb, #0
003177f0: strb     r0, [r6, r5]
003177f4: bhi      #0x317810
003177f8: beq      #0x317808
003177fc: and      r0, r0, #1
00317800: add      sp, sp, #0xc
00317804: pop      {r4, r5, r6, r7, r8, sb, pc}
00317808: cmp      r8, r4
0031780c: bls      #0x3177fc
00317810: mov      r0, #1
00317814: b        #0x3177fc
00317818: cmp      r4, r0
0031781c: movhi    r5, r0
00317820: movls    r5, r4
00317824: b        #0x3177d0

# _ZN15SavegameManager12loadSettingsEb
0046e584: push     {r4, r5, r6, r7, r8, lr}
0046e588: ldr      r3, [r0, #4]
0046e58c: ldr      r5, [pc, #0x10c]
0046e590: mov      r4, r0
0046e594: cmp      r3, #0
0046e598: mov      r6, r1
0046e59c: add      r5, pc, r5
0046e5a0: beq      #0x46e5bc
0046e5a4: mov      r0, r3
0046e5a8: ldr      r3, [r3]
0046e5ac: mov      lr, pc
0046e5b0: ldr      pc, [r3, #4]
0046e5b4: mov      r3, #0
0046e5b8: str      r3, [r4, #4]
0046e5bc: mov      r0, r4
0046e5c0: mov      r1, r6
0046e5c4: bl       #0x46e47c
0046e5c8: mov      r1, #0
0046e5cc: mov      r0, #0x3c
0046e5d0: bl       #0x310570
0046e5d4: ldr      r1, [pc, #0xc8]
0046e5d8: mov      r2, #1
0046e5dc: mov      r7, r0
0046e5e0: add      r1, pc, r1
0046e5e4: bl       #0x315ed8
0046e5e8: ldr      r3, [pc, #0xb8]
0046e5ec: str      r7, [r4, #4]
0046e5f0: ldr      r1, [r4, #0x38]
0046e5f4: ldr      r3, [r5, r3]
0046e5f8: ldr      r0, [r3, #0x4c]
0046e5fc: bl       #0x46d104
0046e600: cmp      r6, #0
0046e604: bne      #0x46e660
0046e608: ldr      r3, [r4, #4]
0046e60c: ldr      r0, [r3, #0x1c]
0046e610: cmp      r0, #0
0046e614: beq      #0x46e65c
0046e618: mov      r1, r4
0046e61c: bl       #0x46d8f4
0046e620: ldr      r3, [r4, #4]
0046e624: mov      r1, r4
0046e628: ldr      r0, [r3, #0x1c]
0046e62c: bl       #0x46c778
0046e630: mov      r0, r4
0046e634: bl       #0x46d514
0046e638: cmn      r0, #1
0046e63c: beq      #0x46e688
0046e640: mov      r0, r4
0046e644: bl       #0x46d514
0046e648: mov      r1, r0
0046e64c: mov      r0, r4
0046e650: bl       #0x46d104
0046e654: mov      r3, #1
0046e658: strb     r3, [r4, #0x28]
0046e65c: pop      {r4, r5, r6, r7, r8, pc}
0046e660: ldr      r3, [r4, #4]
0046e664: mov      r1, r4
0046e668: ldr      r0, [r3, #0x1c]
0046e66c: bl       #0x46c7b8
0046e670: ldr      r3, [r4, #0x38]
0046e674: cmn      r3, #1
0046e678: bne      #0x46e65c
0046e67c: mov      r3, #1
0046e680: strb     r3, [r4, #0x37]
0046e684: pop      {r4, r5, r6, r7, r8, pc}
0046e688: mov      r1, r6
0046e68c: mov      r0, r4
0046e690: bl       #0x46d104
0046e694: mov      r3, #1
0046e698: strb     r3, [r4, #0x37]
0046e69c: b        #0x46e654
0046e6a0: ldrsheq  r6, [r2], #-0x44
0046e6a4: strdeq   lr, pc, [r5], #-0xf0
0046e6a8: strdeq   r3, r4, [r0], -r4

# _ZN15SavegameManagerC1Ev
0046cd4c: ldr      r3, [pc, #0x7c]
0046cd50: ldr      r1, [pc, #0x7c]
0046cd54: push     {r4, r5, r6, lr}
0046cd58: add      r3, pc, r3
0046cd5c: ldr      r1, [r3, r1]
0046cd60: mov      r4, r0
0046cd64: mov      r5, #0
0046cd68: mov      r2, r0
0046cd6c: add      r1, r1, #8
0046cd70: mvn      r0, #0
0046cd74: str      r1, [r4]
0046cd78: str      r0, [r4, #8]
0046cd7c: add      r1, r4, #0x40
0046cd80: str      r5, [r4, #4]
0046cd84: str      r5, [r4, #0xc]
0046cd88: str      r5, [r4, #0x14]
0046cd8c: strb     r5, [r2, #0x10]!
0046cd90: str      r2, [r4, #0x1c]
0046cd94: str      r0, [r4, #0x38]
0046cd98: str      r1, [r4, #0x50]
0046cd9c: mov      r0, r1
0046cda0: str      r1, [r4, #0x54]
0046cda4: str      r2, [r4, #0x18]
0046cda8: str      r5, [r4, #0x20]
0046cdac: strb     r5, [r4, #0x28]
0046cdb0: strb     r5, [r4, #0x37]
0046cdb4: strb     r5, [r4, #0x3c]
0046cdb8: mov      r1, #0x10
0046cdbc: bl       #0x31167c
0046cdc0: ldr      r3, [r4, #0x50]
0046cdc4: mov      r0, r4
0046cdc8: strb     r5, [r3]
0046cdcc: pop      {r4, r5, r6, pc}
0046cdd0: subseq   r7, r2, r8, lsr sp
0046cdd4: andeq    r3, r0, r4, lsl #25

# _ZN6Arrays15GameOptionTable9readNamesEP11IStreamBase
004b360c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b3610: mov      r7, r0
004b3614: sub      sp, sp, #0x1c
004b3618: bl       #0x4a8d28
004b361c: mov      r0, r7
004b3620: bl       #0x313a90
004b3624: ldr      r6, [pc, #0x16c]
004b3628: mov      r3, #1
004b362c: cmp      r3, #0
004b3630: add      r6, pc, r6
004b3634: str      r0, [sp, #0x14]
004b3638: str      r3, [sp, #0xc]
004b363c: bne      #0x4b368c
004b3640: add      r3, sp, #0x14
004b3644: add      r2, r3, #2
004b3648: add      r3, r3, #1
004b364c: ldrb     r0, [r2, #1]
004b3650: ldrb     r1, [r3, #-1]
004b3654: cmp      r2, r3
004b3658: mov      r4, r2
004b365c: eor      r1, r0, r1
004b3660: strb     r1, [r3, #-1]
004b3664: ldrb     r0, [r2, #1]
004b3668: eor      r1, r1, r0
004b366c: strb     r1, [r2, #1]
004b3670: ldrb     r0, [r3, #-1]
004b3674: sub      r2, r2, #1
004b3678: eor      r1, r1, r0
004b367c: strb     r1, [r3, #-1]
004b3680: add      r3, r3, #1
004b3684: bhi      #0x4b364c
004b3688: ldr      r0, [sp, #0x14]
004b368c: ldr      r3, [pc, #0x108]
004b3690: ldr      r3, [r6, r3]
004b3694: ldr      r3, [r3]
004b3698: cmp      r3, r0
004b369c: beq      #0x4b36a8
004b36a0: add      sp, sp, #0x1c
004b36a4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b36a8: lsl      r0, r0, #2
004b36ac: mov      r1, #1
004b36b0: bl       #0x31056c
004b36b4: ldr      sb, [pc, #0xe4]
004b36b8: ldr      r2, [sp, #0x14]
004b36bc: ldr      r3, [r6, sb]
004b36c0: cmp      r2, #0
004b36c4: str      r0, [r3]
004b36c8: beq      #0x4b36a0
004b36cc: add      sl, sp, #0x10
004b36d0: mov      r8, #1
004b36d4: add      r1, sl, r8
004b36d8: add      r3, sl, #2
004b36dc: mov      r4, #0
004b36e0: stm      sp, {r1, r3}
004b36e4: mov      r0, r7
004b36e8: mov      r1, sl
004b36ec: bl       #0x3df1a0
004b36f0: cmp      r8, #0
004b36f4: str      r8, [sp, #0xc]
004b36f8: bne      #0x4b373c
004b36fc: ldr      r3, [sp]
004b3700: ldr      r2, [sp, #4]
004b3704: ldrb     r0, [r2, #1]
004b3708: ldrb     r1, [r3, #-1]
004b370c: cmp      r2, r3
004b3710: eor      r1, r0, r1
004b3714: strb     r1, [r3, #-1]
004b3718: ldrb     r0, [r2, #1]
004b371c: eor      r1, r1, r0
004b3720: strb     r1, [r2, #1]
004b3724: ldrb     r0, [r3, #-1]
004b3728: sub      r2, r2, #1
004b372c: eor      r1, r1, r0
004b3730: strb     r1, [r3, #-1]
004b3734: add      r3, r3, #1
004b3738: bhi      #0x4b3704
004b373c: ldr      r0, [sp, #0x10]
004b3740: ldr      r5, [r6, sb]
004b3744: mov      r1, #1
004b3748: add      r0, r0, r1
004b374c: ldr      fp, [r5]
004b3750: bl       #0x31056c
004b3754: str      r0, [fp, r4, lsl #2]
004b3758: ldr      r3, [r5]
004b375c: ldr      r2, [sp, #0x10]
004b3760: mov      r0, r7
004b3764: ldr      r1, [r3, r4, lsl #2]
004b3768: mov      r3, #0
004b376c: bl       #0x317454
004b3770: ldr      r3, [r5]
004b3774: mov      r1, #0
004b3778: ldr      r2, [r3, r4, lsl #2]
004b377c: ldr      r3, [sp, #0x10]
004b3780: add      r4, r4, #1
004b3784: strb     r1, [r2, r3]
004b3788: ldr      r3, [sp, #0x14]
004b378c: cmp      r3, r4
004b3790: bhi      #0x4b36e4
004b3794: b        #0x4b36a0
004b3798: subeq    r1, lr, r0, ror #8
004b379c: andeq    r3, r0, r0, ror #10
004b37a0: andeq    r1, r0, ip, lsr pc

# _ZN11Application16ResetOrientationEi
0031f748: bx       lr

# _ZN8Savegame10_cacheFileEP12StreamBuffer
00315ad0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315ad4: ldr      r3, [r0, #0x1c]
00315ad8: ldr      r6, [pc, #0x3ec]
00315adc: sub      sp, sp, #0x1c
00315ae0: cmp      r3, #0
00315ae4: mov      r4, r0
00315ae8: mov      r5, r1
00315aec: add      r6, pc, r6
00315af0: beq      #0x315b0c
00315af4: mov      r0, r3
00315af8: ldr      r3, [r3]
00315afc: mov      lr, pc
00315b00: ldr      pc, [r3, #4]
00315b04: mov      r3, #0
00315b08: str      r3, [r4, #0x1c]
00315b0c: cmp      r5, #0
00315b10: beq      #0x315e30
00315b14: mov      r0, r5
00315b18: mov      r2, #0
00315b1c: mov      r3, #0
00315b20: ldr      r1, [r5]
00315b24: mov      lr, pc
00315b28: ldr      pc, [r1, #0x20]
00315b2c: mov      r2, #0
00315b30: mov      r3, #0
00315b34: mov      r0, r5
00315b38: ldr      r1, [r5]
00315b3c: mov      lr, pc
00315b40: ldr      pc, [r1, #0x2c]
00315b44: mov      r1, #0
00315b48: mov      r0, #0x30
00315b4c: bl       #0x310570
00315b50: mov      r1, r5
00315b54: mov      r7, r0
00315b58: bl       #0x3172d8
00315b5c: str      r7, [r4, #0x1c]
00315b60: ldrb     r3, [r4, #0x38]
00315b64: cmp      r3, #0
00315b68: bne      #0x315df4
00315b6c: ldr      r3, [r4, #0x1c]
00315b70: cmp      r3, #0
00315b74: beq      #0x315bbc
00315b78: mov      r0, r3
00315b7c: ldr      r3, [r3]
00315b80: mov      lr, pc
00315b84: ldr      pc, [r3, #8]
00315b88: cmp      r1, #0
00315b8c: bne      #0x315dfc
00315b90: cmp      r0, #3
00315b94: bhi      #0x315dfc
00315b98: ldr      r3, [r4, #0x1c]
00315b9c: cmp      r3, #0
00315ba0: beq      #0x315bbc
00315ba4: mov      r0, r3
00315ba8: ldr      r3, [r3]
00315bac: mov      lr, pc
00315bb0: ldr      pc, [r3, #4]
00315bb4: mov      r3, #0
00315bb8: str      r3, [r4, #0x1c]
00315bbc: ldr      r3, [r4, #0x18]
00315bc0: ldr      r0, [r4, #0x14]
00315bc4: mov      r1, #0
00315bc8: ldr      r7, [pc, #0x300]
00315bcc: rsb      r0, r3, r0
00315bd0: add      r0, r0, #5
00315bd4: bl       #0x31056c
00315bd8: ldr      r1, [r4, #0x18]
00315bdc: mov      r5, r0
00315be0: bl       #0x30e520
00315be4: mov      r0, r5
00315be8: bl       #0x30de54
00315bec: ldr      r1, [pc, #0x2e0]
00315bf0: mov      r2, #5
00315bf4: add      r0, r5, r0
00315bf8: add      r1, pc, r1
00315bfc: bl       #0x30e868
00315c00: ldr      r3, [r6, r7]
00315c04: mov      r1, r5
00315c08: mov      r2, #0
00315c0c: ldr      r3, [r3, #0x10]
00315c10: ldr      r3, [r3, #0x34]
00315c14: mov      r0, r3
00315c18: ldr      r3, [r3]
00315c1c: mov      lr, pc
00315c20: ldr      pc, [r3, #0x94]
00315c24: cmp      r5, #0
00315c28: str      r0, [sp, #0x14]
00315c2c: beq      #0x315c3c
00315c30: mov      r0, r5
00315c34: bl       #0x310440
00315c38: ldr      r0, [sp, #0x14]
00315c3c: cmp      r0, #0
00315c40: beq      #0x315c94
00315c44: mov      r1, #0
00315c48: mov      r0, #0x30
00315c4c: bl       #0x310570
00315c50: add      r5, sp, #0x18
00315c54: ldr      r1, [r5, #-4]!
00315c58: mov      r8, r0
00315c5c: bl       #0x3172d8
00315c60: ldr      r3, [r6, r7]
00315c64: str      r8, [r4, #0x1c]
00315c68: mov      r1, r5
00315c6c: ldr      r3, [r3, #0x10]
00315c70: ldr      r3, [r3, #0x34]
00315c74: mov      r0, r3
00315c78: ldr      r3, [r3]
00315c7c: mov      lr, pc
00315c80: ldr      pc, [r3, #0x78]
00315c84: ldr      r0, [r4, #0x1c]
00315c88: bl       #0x313a90
00315c8c: cmn      r0, #1
00315c90: beq      #0x315ea4
00315c94: ldr      r3, [r4, #0x1c]
00315c98: cmp      r3, #0
00315c9c: beq      #0x315df4
00315ca0: mov      r0, r3
00315ca4: ldr      r3, [r3]
00315ca8: mov      lr, pc
00315cac: ldr      pc, [r3, #8]
00315cb0: cmp      r1, #0
00315cb4: bne      #0x315cc0
00315cb8: cmp      r0, #3
00315cbc: bls      #0x315df4
00315cc0: ldr      r1, [r4, #0x1c]
00315cc4: mov      r2, #0
00315cc8: mov      r3, #0
00315ccc: mov      r0, r1
00315cd0: ldr      r1, [r1]
00315cd4: mov      lr, pc
00315cd8: ldr      pc, [r1, #0x20]
00315cdc: ldr      r0, [r4, #0x1c]
00315ce0: bl       #0x313a90
00315ce4: cmp      r0, #0
00315ce8: str      r0, [sp, #4]
00315cec: beq      #0x315df4
00315cf0: mov      r5, #0
00315cf4: add      r7, r4, #0x20
00315cf8: mov      sb, r5
00315cfc: add      r8, sp, #0xc
00315d00: ldr      r3, [r4, #0x1c]
00315d04: mov      r0, r3
00315d08: ldr      r3, [r3]
00315d0c: mov      lr, pc
00315d10: ldr      pc, [r3, #0x24]
00315d14: ldr      r3, [r4, #0x1c]
00315d18: mov      sl, r0
00315d1c: mov      r6, r1
00315d20: mov      r0, r3
00315d24: ldr      r3, [r3]
00315d28: mov      lr, pc
00315d2c: ldr      pc, [r3, #8]
00315d30: cmp      r1, r6
00315d34: bhi      #0x315d44
00315d38: bne      #0x315df4
00315d3c: cmp      r0, sl
00315d40: bls      #0x315df4
00315d44: ldr      r0, [r4, #0x1c]
00315d48: bl       #0x313a90
00315d4c: mov      r2, #4
00315d50: mov      r3, #0
00315d54: mov      r1, r8
00315d58: mov      r6, r0
00315d5c: ldr      r0, [r4, #0x1c]
00315d60: strb     sb, [sp, #0xc]
00315d64: strb     sb, [sp, #0xd]
00315d68: strb     sb, [sp, #0xe]
00315d6c: strb     sb, [sp, #0xf]
00315d70: strb     sb, [sp, #0x10]
00315d74: bl       #0x317454
00315d78: ldr      r3, [r4, #0x1c]
00315d7c: mov      r0, r3
00315d80: ldr      r3, [r3]
00315d84: mov      lr, pc
00315d88: ldr      pc, [r3, #0x24]
00315d8c: mov      sl, r0
00315d90: mov      fp, r1
00315d94: mov      r0, r7
00315d98: mov      r1, r8
00315d9c: bl       #0x314380
00315da0: cmp      r7, r0
00315da4: mov      r1, r8
00315da8: beq      #0x315e10
00315dac: mov      r0, r7
00315db0: bl       #0x315978
00315db4: mov      r1, r8
00315db8: strd     sl, fp, [r0]
00315dbc: mov      r0, r7
00315dc0: bl       #0x315978
00315dc4: str      r6, [r0, #8]
00315dc8: ldr      r1, [r4, #0x1c]
00315dcc: adds     r2, sl, r6
00315dd0: adc      r3, fp, #0
00315dd4: mov      r0, r1
00315dd8: ldr      r1, [r1]
00315ddc: mov      lr, pc
00315de0: ldr      pc, [r1, #0x20]
00315de4: ldr      r3, [sp, #4]
00315de8: add      r5, r5, #1
00315dec: cmp      r5, r3
00315df0: bne      #0x315d00
00315df4: add      sp, sp, #0x1c
00315df8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00315dfc: ldr      r0, [r4, #0x1c]
00315e00: bl       #0x313a90
00315e04: cmn      r0, #1
00315e08: bne      #0x315c94
00315e0c: b        #0x315b98
00315e10: mov      r1, r8
00315e14: bl       #0x315978
00315e18: strd     sl, fp, [r0]
00315e1c: str      sb, [r0, #0x14]
00315e20: str      sb, [r0, #0x10]
00315e24: str      sb, [r0, #0xc]
00315e28: str      r6, [r0, #8]
00315e2c: b        #0x315dc8
00315e30: ldr      r3, [pc, #0x98]
00315e34: ldr      r1, [r4, #0x18]
00315e38: mov      r2, r5
00315e3c: ldr      r7, [r6, r3]
00315e40: ldr      r3, [r7, #0x10]
00315e44: ldr      r3, [r3, #0x34]
00315e48: mov      r0, r3
00315e4c: ldr      r3, [r3]
00315e50: mov      lr, pc
00315e54: ldr      pc, [r3, #0x94]
00315e58: cmp      r0, #0
00315e5c: str      r0, [sp, #0x14]
00315e60: beq      #0x315b60
00315e64: mov      r1, r5
00315e68: mov      r0, #0x30
00315e6c: bl       #0x310570
00315e70: add      r5, sp, #0x18
00315e74: mov      r8, r0
00315e78: ldr      r1, [r5, #-4]!
00315e7c: bl       #0x3172d8
00315e80: str      r8, [r4, #0x1c]
00315e84: ldr      r3, [r7, #0x10]
00315e88: mov      r1, r5
00315e8c: ldr      r3, [r3, #0x34]
00315e90: mov      r0, r3
00315e94: ldr      r3, [r3]
00315e98: mov      lr, pc
00315e9c: ldr      pc, [r3, #0x78]
00315ea0: b        #0x315b60
00315ea4: ldr      r3, [r4, #0x1c]
00315ea8: cmp      r3, #0
00315eac: beq      #0x315df4
00315eb0: mov      r0, r3
00315eb4: ldr      r3, [r3]
00315eb8: mov      lr, pc
00315ebc: ldr      pc, [r3, #4]
00315ec0: mov      r3, #0
00315ec4: str      r3, [r4, #0x1c]
00315ec8: b        #0x315df4
00315ecc: rsbeq    lr, r7, r4, lsr #31
00315ed0: strdeq   r3, r4, [r0], -r4
00315ed4: subseq   r8, sl, r8, ror #18

# _ZNK10GameObject12IsGameObjectEv
00340054: mov      r0, #1
00340058: bx       lr

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

# _ZNK15SavegameManager11getLanguageEv
0046d514: push     {r4, r5, r6, lr}
0046d518: ldr      r4, [pc, #0x4c]
0046d51c: mov      r6, r0
0046d520: ldr      r5, [pc, #0x48]
0046d524: add      r4, pc, r4
0046d528: mov      r1, r4
0046d52c: bl       #0x46d4a8
0046d530: cmp      r0, #0
0046d534: add      r5, pc, r5
0046d538: bne      #0x46d544
0046d53c: ldr      r0, [r6, #0x38]
0046d540: pop      {r4, r5, r6, pc}
0046d544: mov      r0, r6
0046d548: mov      r1, r4
0046d54c: bl       #0x46d474
0046d550: cmn      r0, #1
0046d554: beq      #0x46d55c
0046d558: pop      {r4, r5, r6, pc}
0046d55c: ldr      r3, [pc, #0x10]
0046d560: ldr      r0, [r5, r3]
0046d564: pop      {r4, r5, r6, lr}
0046d568: b        #0x31f75c
0046d56c: subeq    lr, r5, r4, lsr r6
0046d570: subseq   r7, r2, ip, asr r5
0046d574: strdeq   r3, r4, [r0], -r4
