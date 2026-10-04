
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

# _ZN15VoxSoundManager16SetInitialVolumeEfff
00369c2c: ldr      ip, [pc, #0x150]
00369c30: push     {r4, r5, r6, r7, r8, sl, lr}
00369c34: mov      r4, r0
00369c38: ldr      r0, [pc, #0x148]
00369c3c: add      ip, pc, ip
00369c40: mov      sl, r3
00369c44: ldr      r0, [ip, r0]
00369c48: sub      sp, sp, #0xc
00369c4c: mov      r6, r2
00369c50: ldrb     r3, [r0]
00369c54: mov      r8, r1
00369c58: cmp      r3, #0
00369c5c: bne      #0x369d4c
00369c60: ldrb     r3, [r4, #0x20]
00369c64: cmp      r3, #0
00369c68: beq      #0x369c74
00369c6c: add      sp, sp, #0xc
00369c70: pop      {r4, r5, r6, r7, r8, sl, pc}
00369c74: ldr      r1, [pc, #0x110]
00369c78: add      r5, sp, #8
00369c7c: str      r3, [r5, #-4]!
00369c80: add      r7, r4, #0x64
00369c84: mov      r2, r5
00369c88: add      r1, pc, r1
00369c8c: mov      r0, r7
00369c90: bl       #0x88cc14
00369c94: mov      r1, #0x42000000
00369c98: mov      r0, r8
00369c9c: add      r1, r1, #0xc80000
00369ca0: bl       #0x30ec94
00369ca4: ldr      r8, [r4]
00369ca8: movw     r3, #0xcccd
00369cac: movt     r3, #0x3d4c
00369cb0: mov      r2, r0
00369cb4: ldr      r1, [sp, #4]
00369cb8: mov      r0, r8
00369cbc: bl       #0x8617fc
00369cc0: ldr      r1, [pc, #0xc8]
00369cc4: mov      r2, r5
00369cc8: mov      r0, r7
00369ccc: add      r1, pc, r1
00369cd0: bl       #0x88cc14
00369cd4: mov      r1, #0x42000000
00369cd8: mov      r0, r6
00369cdc: add      r1, r1, #0xc80000
00369ce0: bl       #0x30ec94
00369ce4: ldr      r6, [r4]
00369ce8: movw     r3, #0xcccd
00369cec: movt     r3, #0x3d4c
00369cf0: mov      r2, r0
00369cf4: ldr      r1, [sp, #4]
00369cf8: mov      r0, r6
00369cfc: bl       #0x8617fc
00369d00: ldr      r1, [pc, #0x8c]
00369d04: mov      r2, r5
00369d08: mov      r0, r7
00369d0c: add      r1, pc, r1
00369d10: bl       #0x88cc14
00369d14: mov      r1, #0x42000000
00369d18: add      r1, r1, #0xc80000
00369d1c: mov      r0, sl
00369d20: bl       #0x30ec94
00369d24: ldr      r5, [r4]
00369d28: movw     r3, #0xcccd
00369d2c: mov      r2, r0
00369d30: movt     r3, #0x3d4c
00369d34: mov      r0, r5
00369d38: ldr      r1, [sp, #4]
00369d3c: bl       #0x8617fc
00369d40: mov      r3, #1
00369d44: strb     r3, [r4, #0x20]
00369d48: b        #0x369c6c
00369d4c: ldr      r3, [pc, #0x44]
00369d50: mov      r2, #1
00369d54: ldr      r5, [ip, r3]
00369d58: ldr      r3, [pc, #0x3c]
00369d5c: str      r6, [r5]
00369d60: ldr      r3, [ip, r3]
00369d64: str      r1, [r3]
00369d68: ldr      r0, [r4, #0x24]
00369d6c: bl       #0x53177c
00369d70: ldr      r0, [r4, #0x24]
00369d74: ldr      r1, [r5]
00369d78: mov      r2, #2
00369d7c: bl       #0x53177c
00369d80: b        #0x369c6c
00369d84: rsbeq    sl, r2, r4, asr lr
00369d88: andeq    r3, r0, r0, lsr fp
00369d8c: subseq   r7, r5, r0, ror #5
00369d90: ldrheq   r7, [r5], #-0x24
00369d94: subseq   r5, r5, ip, ror #21
00369d98: andeq    r0, r0, r0, lsl #13
00369d9c: andeq    r0, r0, r8, lsl #24

# _ZN11Application14GetSavedOptionEPKc
00320e44: push     {r4, r5, r6, lr}
00320e48: ldr      r4, [r0, #0x4c]
00320e4c: mov      r5, r1
00320e50: mov      r0, r4
00320e54: bl       #0x46d4a8
00320e58: cmp      r0, #0
00320e5c: bne      #0x320e64
00320e60: pop      {r4, r5, r6, pc}
00320e64: mov      r0, r4
00320e68: mov      r1, r5
00320e6c: pop      {r4, r5, r6, lr}
00320e70: b        #0x46d474

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

# _ZN6Device17IsHighPerformanceEv
0038174c: b        #0x3816a8

# _ZN7gameswf8as_value8set_boolEb
00797230: push     {r4, r5, r6, lr}
00797234: mov      r4, r0
00797238: mov      r5, r1
0079723c: bl       #0x797124
00797240: mov      r3, #1
00797244: strb     r5, [r4, #4]
00797248: strb     r3, [r4, #1]
0079724c: pop      {r4, r5, r6, pc}

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
