
# _ZN16MultiMenuManager11LoadSWFFileEPKci
00437d68: ldr      r3, [pc, #0xac]
00437d6c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00437d70: cmp      r2, #2
00437d74: mov      r4, r2
00437d78: ldr      r2, [pc, #0xa0]
00437d7c: add      r3, pc, r3
00437d80: add      r5, r4, #0x4c
00437d84: ldr      r3, [r3, r2]
00437d88: moveq    r2, #1
00437d8c: movne    r2, #0
00437d90: strb     r2, [r3]
00437d94: add      r5, r0, r5, lsl #2
00437d98: ldr      sl, [r5, #4]
00437d9c: mov      r6, r0
00437da0: mov      r7, r1
00437da4: cmp      sl, #0
00437da8: beq      #0x437db4
00437dac: mov      r0, sl
00437db0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00437db4: mov      r1, #8
00437db8: mov      r0, #0x124
00437dbc: bl       #0x310570
00437dc0: mov      r8, r0
00437dc4: bl       #0x7a86e4
00437dc8: str      r8, [r5, #4]
00437dcc: mov      r2, sl
00437dd0: ldr      r3, [r8]
00437dd4: mov      r0, r8
00437dd8: mov      r1, r7
00437ddc: mov      lr, pc
00437de0: ldr      pc, [r3, #8]
00437de4: ldr      r0, [r5, #4]
00437de8: mov      r1, #1
00437dec: bl       #0x7a7cb4
00437df0: mov      r1, #8
00437df4: mov      r0, #0x34
00437df8: bl       #0x310570
00437dfc: add      r4, r6, r4, lsl #2
00437e00: mov      r7, r0
00437e04: ldr      r1, [r5, #4]
00437e08: bl       #0x42ccd0
00437e0c: str      r7, [r4, #0x144]
00437e10: ldr      sl, [r5, #4]
00437e14: mov      r0, sl
00437e18: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00437e1c: subseq   ip, r5, r4, lsl sp
00437e20: strheq   r0, [r0], -ip

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

# _ZN11MenuManager4InitEv
0042f304: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0042f308: ldr      r5, [pc, #0x24c]
0042f30c: ldr      r6, [r0, #0xbc]
0042f310: sub      sp, sp, #0xc
0042f314: mov      r4, r0
0042f318: add      r5, pc, r5
0042f31c: cmp      r6, #0x1a
0042f320: addls    pc, pc, r6, lsl #2
0042f324: b        #0x42f468
0042f328: b        #0x42f394
0042f32c: b        #0x42f488
0042f330: b        #0x42f394
0042f334: b        #0x42f394
0042f338: b        #0x42f4a4
0042f33c: b        #0x42f4f0
0042f340: b        #0x42f468
0042f344: b        #0x42f550
0042f348: b        #0x42f468
0042f34c: b        #0x43089c
0042f350: b        #0x42f468
0042f354: b        #0x4308e8
0042f358: b        #0x42f468
0042f35c: b        #0x4309dc
0042f360: b        #0x4309e8
0042f364: b        #0x42f468
0042f368: b        #0x42f468
0042f36c: b        #0x430a0c
0042f370: b        #0x42f47c
0042f374: b        #0x4309f4
0042f378: b        #0x430a00
0042f37c: b        #0x42f468
0042f380: b        #0x42f468
0042f384: b        #0x4308fc
0042f388: b        #0x4308a8
0042f38c: b        #0x42fafc
0042f390: b        #0x42f504
0042f394: ldr      r2, [r0, #0xf4]
0042f398: add      r7, r6, #0x4c
0042f39c: mov      r3, #0
0042f3a0: add      r2, r2, r7, lsl #2
0042f3a4: str      r3, [r2, #4]
0042f3a8: ldr      r2, [r0, #0xf4]
0042f3ac: ldr      r0, [pc, #0x1ac]
0042f3b0: add      r1, r6, #0x50
0042f3b4: add      r2, r2, r1, lsl #2
0042f3b8: ldr      sl, [r5, r0]
0042f3bc: str      r3, [r2, #4]
0042f3c0: ldr      r8, [pc, #0x19c]
0042f3c4: ldr      r3, [sl, #0x10]
0042f3c8: add      r8, pc, r8
0042f3cc: ldr      r3, [r3, #0x34]
0042f3d0: ldr      sb, [r8, r6, lsl #2]
0042f3d4: mov      r0, r3
0042f3d8: mov      r1, sb
0042f3dc: ldr      r3, [r3]
0042f3e0: mov      lr, pc
0042f3e4: ldr      pc, [r3, #0xa8]
0042f3e8: cmp      r0, #0
0042f3ec: beq      #0x430894
0042f3f0: ldr      r3, [pc, #0x170]
0042f3f4: movw     r2, #0x356
0042f3f8: ldr      r3, [r5, r3]
0042f3fc: ldr      r3, [r3]
0042f400: cmp      r3, r2
0042f404: beq      #0x430a34
0042f408: cmp      r3, #0x3c0
0042f40c: beq      #0x42f438
0042f410: cmp      r3, #0x320
0042f414: beq      #0x430a4c
0042f418: ldr      r0, [sl, #0x4c]
0042f41c: bl       #0x46d514
0042f420: cmp      r0, #5
0042f424: beq      #0x430aa4
0042f428: ldr      r0, [sl, #0x4c]
0042f42c: bl       #0x46d514
0042f430: cmp      r0, #4
0042f434: beq      #0x430abc
0042f438: mov      r1, sb
0042f43c: ldr      r0, [r4, #0xf4]
0042f440: mov      r2, r6
0042f444: bl       #0x437d68
0042f448: cmp      r6, #3
0042f44c: beq      #0x430a1c
0042f450: ldr      r3, [r4, #0xf4]
0042f454: mov      r1, #0x84
0042f458: add      r7, r3, r7, lsl #2
0042f45c: ldr      r0, [r7, #4]
0042f460: bl       #0x7a7c98
0042f464: ldr      r6, [r4, #0xbc]
0042f468: add      r6, r6, #1
0042f46c: str      r6, [r4, #0xbc]
0042f470: mov      r0, #0
0042f474: add      sp, sp, #0xc
0042f478: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0042f47c: bl       #0x433720
0042f480: ldr      r6, [r4, #0xbc]
0042f484: b        #0x42f468
0042f488: ldr      r2, [r0, #0xf4]
0042f48c: mov      r3, #0
0042f490: str      r3, [r2, #0x138]
0042f494: ldr      r2, [r0, #0xf4]
0042f498: str      r3, [r2, #0x148]
0042f49c: ldr      r6, [r0, #0xbc]
0042f4a0: b        #0x42f468
0042f4a4: bl       #0x41edd0
0042f4a8: ldr      r3, [r4, #0xf4]
0042f4ac: ldr      r3, [r3, #0x140]
0042f4b0: str      r3, [r0, #0x57c]
0042f4b4: bl       #0x41d880
0042f4b8: bl       #0x41b11c
0042f4bc: ldr      r3, [r4, #0xf4]
0042f4c0: ldr      r3, [r3, #0x140]
0042f4c4: str      r3, [r0, #0x658]
0042f4c8: bl       #0x419b4c
0042f4cc: bl       #0x413e90
0042f4d0: mov      r5, r0
0042f4d4: mov      r0, r4
0042f4d8: bl       #0x42cb8c
0042f4dc: mov      r1, r0
0042f4e0: mov      r0, r5
0042f4e4: bl       #0x4151d8
0042f4e8: ldr      r6, [r4, #0xbc]
0042f4ec: b        #0x42f468
0042f4f0: ldr      r0, [pc, #0x74]
0042f4f4: add      r0, pc, r0
0042f4f8: bl       #0x3e1374
0042f4fc: ldr      r6, [r4, #0xbc]
0042f500: b        #0x42f468
0042f504: ldr      r3, [pc, #0x54]
0042f508: mov      r2, r0
0042f50c: mov      r1, #5
0042f510: ldr      r5, [r5, r3]
0042f514: mov      r3, #0xa
0042f518: ldr      r0, [r5, #0x14]
0042f51c: bl       #0x338da0
0042f520: mov      r1, #4
0042f524: mov      r2, r4
0042f528: mov      r3, #0xa
0042f52c: ldr      r0, [r5, #0x14]
0042f530: bl       #0x338da0
0042f534: ldr      r0, [r5, #0x14]
0042f538: mov      r2, r4
0042f53c: mov      r1, #7
0042f540: mov      r3, #0xa
0042f544: bl       #0x338da0
0042f548: mov      r0, #1
0042f54c: b        #0x42f474
0042f550: bl       #0x42c35c
0042f554: ldr      r6, [r4, #0xbc]
0042f558: b        #0x42f468
0042f55c: subseq   r5, r6, r8, ror r7
0042f560: strdeq   r3, r4, [r0], -r4

# _ZN7gameswf9tu_stringC1EPKc
00413a7c: mov      r3, #1
00413a80: push     {r4, r5, r6, lr}
00413a84: strb     r3, [r0]
00413a88: subs     r5, r1, #0
00413a8c: mov      r3, #0
00413a90: mov      r4, r0
00413a94: strb     r3, [r0, #1]
00413a98: beq      #0x413ac8
00413a9c: mov      r0, r5
00413aa0: bl       #0x30de54
00413aa4: mov      r1, r0
00413aa8: mov      r0, r4
00413aac: bl       #0x751d14
00413ab0: ldrsb    r3, [r4]
00413ab4: mov      r1, r5
00413ab8: cmn      r3, #1
00413abc: addne    r0, r4, #1
00413ac0: ldreq    r0, [r4, #0xc]
00413ac4: bl       #0x30e520
00413ac8: ldr      r3, [r4, #0x10]
00413acc: mvn      r2, #0
00413ad0: mov      r0, r4
00413ad4: bfi      r3, r2, #0, #0x18
00413ad8: lsr      r2, r3, #0x18
00413adc: bfc      r2, #0, #1
00413ae0: str      r3, [r4, #0x10]
00413ae4: strb     r2, [r4, #0x13]
00413ae8: pop      {r4, r5, r6, pc}
