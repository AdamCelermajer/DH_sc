
# _ZN6CharAI16AI_DoMeleeAttackEP10GameObjectb
003d01ac: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d01b0: ldr      r4, [pc, #0x57c]
003d01b4: ldr      r5, [pc, #0x57c]
003d01b8: mov      r6, r0
003d01bc: add      r4, pc, r4
003d01c0: ldr      r0, [r4, r5]
003d01c4: mov      sl, r1
003d01c8: ldr      r3, [r6, #4]
003d01cc: ldr      r1, [r0]
003d01d0: sub      sp, sp, #0x114
003d01d4: mov      r0, r3
003d01d8: str      r1, [sp, #0x10c]
003d01dc: ldr      r3, [r3]
003d01e0: mov      r7, r2
003d01e4: mov      lr, pc
003d01e8: ldr      pc, [r3, #0x34]
003d01ec: cmp      r0, #0
003d01f0: bne      #0x3d0204
003d01f4: ldr      r3, [r6, #4]
003d01f8: ldr      r2, [r3, #0x528]
003d01fc: tst      r2, #1
003d0200: beq      #0x3d0220
003d0204: ldr      r3, [r4, r5]
003d0208: ldr      r2, [sp, #0x10c]
003d020c: ldr      r3, [r3]
003d0210: cmp      r2, r3
003d0214: bne      #0x3d0730
003d0218: add      sp, sp, #0x114
003d021c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d0220: mov      r0, r3
003d0224: ldr      r3, [r3]
003d0228: mov      lr, pc
003d022c: ldr      pc, [r3, #0x124]
003d0230: cmp      r0, #0
003d0234: bne      #0x3d0370
003d0238: ldr      r0, [r6, #4]
003d023c: add      r0, r0, #0x4f0
003d0240: add      r0, r0, #0xc
003d0244: bl       #0x3c02d0
003d0248: subs     r8, r0, #0
003d024c: beq      #0x3d0384
003d0250: ldrb     r7, [r6, #0x79]
003d0254: cmp      r7, #0
003d0258: bne      #0x3d0204
003d025c: ldr      fp, [pc, #0x4d8]
003d0260: add      sl, sp, #0xf4
003d0264: add      r8, sp, #0x10
003d0268: ldr      sb, [r4, fp]
003d026c: mov      r0, sb
003d0270: bl       #0x337888
003d0274: ldr      r1, [pc, #0x4c4]
003d0278: add      r2, sp, #0x78
003d027c: mov      r0, sl
003d0280: add      r1, pc, r1
003d0284: bl       #0x3140ec
003d0288: mov      r1, sl
003d028c: mov      r0, sb
003d0290: bl       #0x337a88
003d0294: mov      r0, sl
003d0298: bl       #0x318254
003d029c: mov      ip, #1
003d02a0: ldr      r1, [r6, #4]
003d02a4: strb     ip, [r6, #0x78]
003d02a8: mov      r3, r7
003d02ac: mov      r2, ip
003d02b0: mov      r0, r8
003d02b4: str      ip, [sp]
003d02b8: bl       #0x4a2730
003d02bc: ldr      r3, [r6, #4]
003d02c0: ldrb     r1, [r3, #0x1b5]
003d02c4: cmp      r1, #0
003d02c8: bne      #0x3d0438
003d02cc: ldr      r3, [r6, #0x40]
003d02d0: cmp      r3, #0
003d02d4: beq      #0x3d02e8
003d02d8: mov      r0, r6
003d02dc: bl       #0x3d67f4
003d02e0: cmp      r0, #0
003d02e4: bne      #0x3d064c
003d02e8: mov      r0, r8
003d02ec: bl       #0x3d015c
003d02f0: movw     r2, #0xfdb
003d02f4: mov      r0, r8
003d02f8: mov      r1, #0
003d02fc: movt     r2, #0x40c9
003d0300: bl       #0x3d0020
003d0304: ldr      r3, [sp, #0x10]
003d0308: ldr      r2, [sp, #0x20]
003d030c: cmp      r2, r3
003d0310: beq      #0x3d0364
003d0314: ldr      r1, [r3]
003d0318: mov      r2, #0
003d031c: mov      r0, r6
003d0320: bl       #0x3d6890
003d0324: ldr      sl, [r4, fp]
003d0328: add      r7, sp, #0xc4
003d032c: mov      r0, sl
003d0330: bl       #0x337888
003d0334: ldr      r1, [pc, #0x408]
003d0338: add      r2, sp, #0x70
003d033c: mov      r0, r7
003d0340: add      r1, pc, r1
003d0344: bl       #0x3140ec
003d0348: mov      r0, sl
003d034c: mov      r1, r7
003d0350: bl       #0x337a88
003d0354: cmp      r0, #0
003d0358: bne      #0x3d0480
003d035c: mov      r0, r7
003d0360: bl       #0x318254
003d0364: mov      r0, r8
003d0368: bl       #0x38d18c
003d036c: b        #0x3d0204
003d0370: mov      r0, r6
003d0374: mov      r1, sl
003d0378: mov      r2, r7
003d037c: bl       #0x3d076c
003d0380: b        #0x3d0204
003d0384: ldr      fp, [pc, #0x3b0]
003d0388: add      sb, sp, #0xac
003d038c: ldr      r3, [r4, fp]
003d0390: mov      r0, r3
003d0394: str      r3, [sp, #0xc]
003d0398: bl       #0x337888
003d039c: ldr      r1, [pc, #0x3a4]
003d03a0: add      r2, sp, #0x6c
003d03a4: mov      r0, sb
003d03a8: add      r1, pc, r1
003d03ac: bl       #0x3140ec
003d03b0: ldr      r3, [sp, #0xc]
003d03b4: mov      r1, sb
003d03b8: mov      r0, r3
003d03bc: bl       #0x337a88
003d03c0: mov      r0, sb
003d03c4: bl       #0x318254
003d03c8: cmp      sl, #0
003d03cc: strb     r8, [r6, #0x78]
003d03d0: beq      #0x3d056c
003d03d4: mov      r1, sl
003d03d8: mov      r0, r6
003d03dc: mov      r2, r7
003d03e0: bl       #0x3d6890
003d03e4: ldr      r3, [r6, #0x40]
003d03e8: cmp      r3, #0
003d03ec: beq      #0x3d04d0
003d03f0: cmp      r7, #0
003d03f4: bne      #0x3d0204
003d03f8: ldr      r3, [r6, #0x40]
003d03fc: cmp      r3, #0
003d0400: moveq    r1, r7
003d0404: beq      #0x3d0420
003d0408: mov      r1, r7
003d040c: mov      r0, r6
003d0410: bl       #0x3d6188
003d0414: cmp      r0, #0
003d0418: beq      #0x3d0204
003d041c: ldr      r1, [r6, #0x40]
003d0420: ldr      r0, [r6, #4]
003d0424: mov      r2, #0
003d0428: add      r0, r0, #0x4f0
003d042c: add      r0, r0, #0xc
003d0430: bl       #0x3c6488
003d0434: b        #0x3d0204
003d0438: ldr      r3, [pc, #0x30c]
003d043c: ldr      r1, [pc, #0x30c]
003d0440: ldr      r2, [pc, #0x30c]
003d0444: ldr      r3, [r4, r3]
003d0448: add      r1, pc, r1
003d044c: add      r2, pc, r2
003d0450: ldr      r0, [r3, #0x2c]
003d0454: bl       #0x4c4bdc
003d0458: asr      r0, r0, #1
003d045c: bl       #0x30e964
003d0460: movw     r1, #0xfa35
003d0464: movt     r1, #0x3c8e
003d0468: bl       #0x30ed6c
003d046c: mov      r1, #0
003d0470: mov      r2, r0
003d0474: mov      r0, r8
003d0478: bl       #0x3d0020
003d047c: b        #0x3d0304
003d0480: ldr      r3, [r6, #4]
003d0484: mov      r0, r3
003d0488: ldr      r3, [r3]
003d048c: mov      lr, pc
003d0490: ldr      pc, [r3, #0x28]
003d0494: cmp      r0, #0
003d0498: beq      #0x3d035c
003d049c: mov      r0, r7
003d04a0: bl       #0x318254
003d04a4: ldr      r3, [sp, #0x10]
003d04a8: ldr      r2, [sp, #0x20]
003d04ac: cmp      r2, r3
003d04b0: beq      #0x3d0364
003d04b4: mov      r0, r8
003d04b8: bl       #0x38fb18
003d04bc: ldr      r3, [sp, #0x10]
003d04c0: ldr      r2, [sp, #0x20]
003d04c4: cmp      r2, r3
003d04c8: bne      #0x3d04b4
003d04cc: b        #0x3d0364
003d04d0: ldr      r3, [r6, #4]
003d04d4: movw     r2, #0x14a8
003d04d8: ldrsb    r2, [r3, r2]
003d04dc: cmp      r2, #8
003d04e0: bne      #0x3d03f0
003d04e4: ldrb     r2, [r3, #0x1b5]
003d04e8: cmp      r2, #0
003d04ec: bne      #0x3d03f0
003d04f0: mov      r0, r3
003d04f4: ldr      r3, [r3]
003d04f8: mov      lr, pc
003d04fc: ldr      pc, [r3, #0x28]
003d0500: cmp      r0, #0
003d0504: beq      #0x3d0540
003d0508: ldr      sl, [r4, fp]
003d050c: add      r8, sp, #0x7c
003d0510: mov      r0, sl
003d0514: bl       #0x337888
003d0518: ldr      r1, [pc, #0x238]
003d051c: add      r2, sp, #0x64
003d0520: mov      r0, r8
003d0524: add      r1, pc, r1
003d0528: bl       #0x3140ec
003d052c: mov      r0, sl
003d0530: mov      r1, r8
003d0534: bl       #0x337a88
003d0538: mov      r0, r8
003d053c: bl       #0x318254
003d0540: ldr      r2, [r6, #4]
003d0544: mov      r3, #1
003d0548: strb     r3, [r6, #0x4a]
003d054c: movw     r3, #0x14a4
003d0550: ldr      r1, [r2, r3]
003d0554: mov      r0, r6
003d0558: mov      r2, #0
003d055c: bl       #0x3d6890
003d0560: mov      r0, r6
003d0564: bl       #0x3d49c4
003d0568: b        #0x3d03f0
003d056c: mov      ip, #1
003d0570: add      r8, sp, #0x10
003d0574: ldr      r1, [r6, #4]
003d0578: mov      r3, sl
003d057c: mov      r2, ip
003d0580: mov      r0, r8
003d0584: str      ip, [sp]
003d0588: bl       #0x4a2730
003d058c: ldr      r3, [r6, #4]
003d0590: ldrb     r3, [r3, #0x1b5]
003d0594: cmp      r3, #0
003d0598: beq      #0x3d06c0
003d059c: ldr      r3, [pc, #0x1a8]
003d05a0: ldr      r1, [pc, #0x1b4]
003d05a4: ldr      r2, [pc, #0x1b4]
003d05a8: ldr      r3, [r4, r3]
003d05ac: add      r1, pc, r1
003d05b0: add      r2, pc, r2
003d05b4: ldr      r0, [r3, #0x2c]
003d05b8: bl       #0x4c4bdc
003d05bc: asr      r0, r0, #1
003d05c0: bl       #0x30e964
003d05c4: movw     r1, #0xfa35
003d05c8: movt     r1, #0x3c8e
003d05cc: bl       #0x30ed6c
003d05d0: mov      r1, #0
003d05d4: mov      r2, r0
003d05d8: mov      r0, r8
003d05dc: bl       #0x3d0020
003d05e0: ldr      r3, [sp, #0x10]
003d05e4: ldr      r2, [sp, #0x20]
003d05e8: cmp      r2, r3
003d05ec: beq      #0x3d0640
003d05f0: ldr      r1, [r3]
003d05f4: mov      r2, #0
003d05f8: mov      r0, r6
003d05fc: bl       #0x3d6890
003d0600: ldr      sb, [r4, fp]
003d0604: add      sl, sp, #0x94
003d0608: mov      r0, sb
003d060c: bl       #0x337888
003d0610: ldr      r1, [pc, #0x14c]
003d0614: add      r2, sp, #0x68
003d0618: mov      r0, sl
003d061c: add      r1, pc, r1
003d0620: bl       #0x3140ec
003d0624: mov      r0, sb
003d0628: mov      r1, sl
003d062c: bl       #0x337a88
003d0630: cmp      r0, #0
003d0634: bne      #0x3d06e0
003d0638: mov      r0, sl
003d063c: bl       #0x318254
003d0640: mov      r0, r8
003d0644: bl       #0x38d18c
003d0648: b        #0x3d03e4
003d064c: ldr      r3, [r6, #0x40]
003d0650: mov      r0, r3
003d0654: ldr      r3, [r3]
003d0658: mov      lr, pc
003d065c: ldr      pc, [r3, #0x34]
003d0660: cmp      r0, #0
003d0664: bne      #0x3d02e8
003d0668: ldr      r3, [r6, #4]
003d066c: mov      r0, r3
003d0670: ldr      r3, [r3]
003d0674: mov      lr, pc
003d0678: ldr      pc, [r3, #0x28]
003d067c: cmp      r0, #0
003d0680: beq      #0x3d0304
003d0684: ldr      sl, [r4, fp]
003d0688: add      r7, sp, #0xdc
003d068c: mov      r0, sl
003d0690: bl       #0x337888
003d0694: ldr      r1, [pc, #0xcc]
003d0698: add      r2, sp, #0x74
003d069c: mov      r0, r7
003d06a0: add      r1, pc, r1
003d06a4: bl       #0x3140ec
003d06a8: mov      r0, sl
003d06ac: mov      r1, r7
003d06b0: bl       #0x337a88
003d06b4: mov      r0, r7
003d06b8: bl       #0x318254
003d06bc: b        #0x3d0304
003d06c0: mov      r0, r8
003d06c4: bl       #0x3d015c
003d06c8: movw     r2, #0xfdb
003d06cc: mov      r0, r8
003d06d0: mov      r1, #0
003d06d4: movt     r2, #0x40c9
003d06d8: bl       #0x3d0020
003d06dc: b        #0x3d05e0
003d06e0: ldr      r3, [r6, #4]
003d06e4: mov      r0, r3
003d06e8: ldr      r3, [r3]
003d06ec: mov      lr, pc
003d06f0: ldr      pc, [r3, #0x28]
003d06f4: cmp      r0, #0
003d06f8: beq      #0x3d0638
003d06fc: mov      r0, sl
003d0700: bl       #0x318254
003d0704: ldr      r3, [sp, #0x10]
003d0708: ldr      r2, [sp, #0x20]
003d070c: cmp      r2, r3
003d0710: beq      #0x3d0640
003d0714: mov      r0, r8
003d0718: bl       #0x38fb18
003d071c: ldr      r3, [sp, #0x10]
003d0720: ldr      r2, [sp, #0x20]
003d0724: cmp      r2, r3
003d0728: bne      #0x3d0714
003d072c: b        #0x3d0640
003d0730: bl       #0x30e310
003d0734: ldrsbeq  r4, [ip], #-0x84
003d0738: andeq    r4, r0, ip, lsr #1
003d073c: andeq    r0, r0, r4, lsl #17
003d0740: subeq    r5, pc, r0, lsl r2
003d0744: subeq    r5, pc, r0, lsl #3
003d0748: subeq    r5, pc, r8, ror #1
003d074c: strdeq   r3, r4, [r0], -r4
003d0750: subeq    r1, pc, r8, lsl #6
003d0754: subeq    r5, pc, ip, asr r0
003d0758: umaaleq  r4, pc, ip, pc
003d075c: subeq    r1, pc, r4, lsr #3
003d0760: strdeq   r4, r5, [pc], #-0xe8
003d0764: subeq    r4, pc, r4, lsr #29
003d0768: subeq    r4, pc, r0, lsr #28

# _ZN6CharAI13AI_BeginSpellEb
003d81c0: push     {r4, r5, r6, r7, r8, lr}
003d81c4: mov      r4, r0
003d81c8: mov      r6, r1
003d81cc: ldr      r0, [r0, #4]
003d81d0: mvn      r1, #0
003d81d4: bl       #0x3bb98c
003d81d8: mov      r1, r0
003d81dc: mov      r5, r0
003d81e0: ldr      r0, [r4, #4]
003d81e4: bl       #0x3aeac0
003d81e8: ldr      r7, [r0, #0x1c]
003d81ec: cmp      r7, #1
003d81f0: beq      #0x3d82a4
003d81f4: mov      r0, r4
003d81f8: bl       #0x3d80b4
003d81fc: cmp      r0, #0
003d8200: bne      #0x3d8208
003d8204: pop      {r4, r5, r6, r7, r8, pc}
003d8208: ldr      r7, [r4, #4]
003d820c: mov      r8, #0
003d8210: mvn      r1, #0
003d8214: strb     r8, [r4, #0xd0]
003d8218: strb     r8, [r4, #0xd1]
003d821c: mov      r0, r7
003d8220: bl       #0x3bb98c
003d8224: mov      r1, r0
003d8228: add      r0, r7, #0x4f0
003d822c: mov      r3, r8
003d8230: mov      r2, r8
003d8234: add      r0, r0, #0xc
003d8238: bl       #0x3c6394
003d823c: bl       #0x7fd794
003d8240: ldrb     r3, [r0, #5]
003d8244: cmp      r3, r8
003d8248: beq      #0x3d8290
003d824c: cmp      r6, r8
003d8250: bne      #0x3d8290
003d8254: bl       #0x80b1bc
003d8258: mov      r6, r0
003d825c: ldr      r0, [pc, #0xc0]
003d8260: ldr      r3, [r4, #4]
003d8264: mov      r1, #1
003d8268: add      r0, pc, r0
003d826c: ldrb     r7, [r3, #0x108]
003d8270: bl       #0x80a244
003d8274: mov      r3, #3
003d8278: mov      r1, r0
003d827c: strb     r7, [r0, #0x54]
003d8280: strb     r3, [r0, #0x50]
003d8284: strh     r5, [r0, #0x52]
003d8288: mov      r0, r6
003d828c: bl       #0x80e2a4
003d8290: ldr      r0, [r4, #4]
003d8294: add      r0, r0, #0x4f0
003d8298: add      r0, r0, #0xc
003d829c: pop      {r4, r5, r6, r7, r8, lr}
003d82a0: b        #0x3c0334
003d82a4: mov      r0, r4
003d82a8: bl       #0x3d7d9c
003d82ac: cmp      r0, #0
003d82b0: beq      #0x3d81f4
003d82b4: ldr      r3, [r4, #0xc0]
003d82b8: ldr      r0, [r3, r5, lsl #2]
003d82bc: bl       #0x3da8b8
003d82c0: bl       #0x7fd794
003d82c4: ldrb     r3, [r0, #5]
003d82c8: cmp      r3, #0
003d82cc: beq      #0x3d831c
003d82d0: cmp      r6, #0
003d82d4: bne      #0x3d831c
003d82d8: bl       #0x80b1bc
003d82dc: mov      r6, r0
003d82e0: ldr      r0, [pc, #0x40]
003d82e4: ldr      r3, [r4, #4]
003d82e8: mov      r1, r7
003d82ec: add      r0, pc, r0
003d82f0: ldrb     r4, [r3, #0x108]
003d82f4: bl       #0x80a244
003d82f8: mov      r3, #3
003d82fc: mov      r1, r0
003d8300: strb     r4, [r0, #0x54]
003d8304: strb     r3, [r0, #0x50]
003d8308: strh     r5, [r0, #0x52]
003d830c: mov      r0, r6
003d8310: bl       #0x80e2a4
003d8314: mov      r0, r7
003d8318: pop      {r4, r5, r6, r7, r8, pc}
003d831c: mov      r0, #1
003d8320: b        #0x3d8204
003d8324: subeq    r6, lr, r0, lsr #25
003d8328: subeq    r6, lr, ip, lsl ip

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

# _ZN10AISDefault11OnPreAttackEi
003dbef8: bx       lr

# _ZN6CharAIC1Ev
003ced50: ldr      r3, [pc, #0x14c]
003ced54: ldr      r2, [pc, #0x14c]
003ced58: push     {r4, r5, lr}
003ced5c: add      r3, pc, r3
003ced60: ldr      r2, [r3, r2]
003ced64: mov      r4, r0
003ced68: mov      r1, #0
003ced6c: add      r2, r2, #8
003ced70: str      r2, [r4]
003ced74: ldr      r2, [pc, #0x130]
003ced78: mov      r0, #1
003ced7c: mvn      ip, #0
003ced80: mov      r5, r4
003ced84: strb     r0, [r4, #0x55]
003ced88: str      r1, [r4, #8]
003ced8c: str      r1, [r4, #0xc]
003ced90: strb     r1, [r4, #0x18]
003ced94: str      r1, [r4, #0x1c]
003ced98: str      r1, [r4, #0x20]
003ced9c: strb     r1, [r4, #0x24]
003ceda0: str      r1, [r4, #0x28]
003ceda4: strb     r1, [r4, #0x2c]
003ceda8: str      r1, [r4, #0x30]
003cedac: str      r1, [r4, #0x34]
003cedb0: str      r1, [r4, #0x3c]
003cedb4: str      r1, [r4, #0x40]
003cedb8: str      r1, [r4, #0x44]
003cedbc: strb     r1, [r4, #0x49]
003cedc0: strb     r0, [r4, #0x4a]
003cedc4: strb     r0, [r4, #0x4b]
003cedc8: strb     r1, [r4, #0x4c]
003cedcc: strb     r0, [r4, #0x4d]
003cedd0: str      r1, [r4, #0x50]
003cedd4: strb     r0, [r4, #0x54]
003cedd8: str      r1, [r4, #0x58]
003ceddc: mov      r0, r4
003cede0: str      r1, [r4, #0x60]
003cede4: str      ip, [r4, #0x10]
003cede8: str      ip, [r4, #0x14]
003cedec: str      ip, [r4, #0x38]
003cedf0: strb     r1, [r5, #0x5c]!
003cedf4: str      r5, [r4, #0x68]
003cedf8: str      r5, [r4, #0x64]
003cedfc: str      r1, [r4, #0x6c]
003cee00: str      r1, [r4, #0x80]
003cee04: strb     r1, [r0, #0x7c]!
003cee08: ldr      r5, [r3, r2]
003cee0c: mov      r2, r4
003cee10: str      r0, [r4, #0x88]
003cee14: str      r0, [r4, #0x84]
003cee18: str      r1, [r4, #0x8c]
003cee1c: str      r1, [r4, #0x98]
003cee20: add      r0, r4, #0xac
003cee24: strb     r1, [r2, #0x94]!
003cee28: str      r2, [r4, #0xa0]
003cee2c: str      r0, [r4, #0xb0]
003cee30: str      ip, [r4, #0xcc]
003cee34: strb     r1, [r4, #0xd1]
003cee38: str      r2, [r4, #0x9c]
003cee3c: str      r1, [r4, #0xa4]
003cee40: str      r0, [r4, #0xac]
003cee44: str      r1, [r4, #0xb4]
003cee48: str      r1, [r4, #0xb8]
003cee4c: str      r1, [r4, #0xbc]
003cee50: str      r1, [r4, #0xc0]
003cee54: str      r1, [r4, #0xc4]
003cee58: str      r1, [r4, #0xc8]
003cee5c: strb     r1, [r4, #0xd0]
003cee60: ldr      r1, [r5, #0x18]
003cee64: ldr      r2, [r5, #0x10]
003cee68: sub      sp, sp, #0xc
003cee6c: sub      r3, r1, #4
003cee70: cmp      r2, r3
003cee74: str      r4, [sp, #4]
003cee78: beq      #0x3cee98
003cee7c: str      r4, [r2]
003cee80: ldr      r3, [r5, #0x10]
003cee84: add      r3, r3, #4
003cee88: str      r3, [r5, #0x10]
003cee8c: mov      r0, r4
003cee90: add      sp, sp, #0xc
003cee94: pop      {r4, r5, pc}
003cee98: add      r0, sp, #4
003cee9c: bl       #0x3ce810
003ceea0: b        #0x3cee8c
003ceea4: subseq   r5, ip, r4, lsr sp
003ceea8: andeq    r4, r0, ip, asr #12
003ceeac: andeq    r4, r0, ip, lsr #19

# _Z19NativeLockCharacterRKN7gameswf7fn_callE
0043eac0: push     {r4, r5, r6, r7, r8, sb, lr}
0043eac4: ldr      r5, [r0, #0x10]
0043eac8: sub      sp, sp, #0x14
0043eacc: mov      r4, r0
0043ead0: cmp      r5, #1
0043ead4: beq      #0x43eae0
0043ead8: add      sp, sp, #0x14
0043eadc: pop      {r4, r5, r6, r7, r8, sb, pc}
0043eae0: ldr      r2, [r0, #0x14]
0043eae4: ldr      r7, [r0, #0xc]
0043eae8: mov      r6, #0xc
0043eaec: mul      r6, r6, r2
0043eaf0: ldr      r3, [r7]
0043eaf4: add      r3, r3, r6
0043eaf8: ldrsb    r2, [r3, #1]
0043eafc: cmp      r2, #2
0043eb00: bne      #0x43ead8
0043eb04: ldr      r2, [r3, #8]
0043eb08: ldr      r3, [r3, #4]
0043eb0c: str      r2, [sp, #0xc]
0043eb10: str      r3, [sp, #8]
0043eb14: ldrd     r8, sb, [sp, #8]
0043eb18: mov      r0, r8
0043eb1c: mov      r2, r8
0043eb20: mov      r1, sb
0043eb24: mov      r3, sb
0043eb28: strd     r8, sb, [sp]
0043eb2c: bl       #0x30e2bc
0043eb30: subs     r8, r0, #0
0043eb34: bne      #0x43ead8
0043eb38: ldr      r0, [r7]
0043eb3c: add      r0, r0, r6
0043eb40: bl       #0x43a1b8
0043eb44: mov      r1, r8
0043eb48: bl       #0x43c388
0043eb4c: subs     r6, r0, #0
0043eb50: beq      #0x43ead8
0043eb54: ldr      r0, [r6, #0x378]
0043eb58: bl       #0x40559c
0043eb5c: ldr      r3, [r6, #0x378]
0043eb60: mov      r0, r5
0043eb64: strb     r5, [r3, #8]
0043eb68: bl       #0x439c68
0043eb6c: ldr      r0, [r4]
0043eb70: mov      r1, r8
0043eb74: bl       #0x797230
0043eb78: b        #0x43ead8

# _ZN20Script_LockCharacter7ExecuteEbi
0045dda0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0045dda4: ldr      r4, [pc, #0x108]
0045dda8: ldr      r5, [pc, #0x108]
0045ddac: sub      sp, sp, #0x38
0045ddb0: add      r4, pc, r4
0045ddb4: ldr      r3, [r4, r5]
0045ddb8: subs     r6, r1, #0
0045ddbc: mov      sl, r2
0045ddc0: ldr      r3, [r3]
0045ddc4: str      r3, [sp, #0x34]
0045ddc8: beq      #0x45dde8
0045ddcc: ldr      r3, [r4, r5]
0045ddd0: ldr      r2, [sp, #0x34]
0045ddd4: ldr      r3, [r3]
0045ddd8: cmp      r2, r3
0045dddc: bne      #0x45deb0
0045dde0: add      sp, sp, #0x38
0045dde4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0045dde8: ldr      r3, [pc, #0xcc]
0045ddec: ldr      sb, [r0, #0xc]
0045ddf0: add      r7, sp, #0x1c
0045ddf4: ldr      r8, [r4, r3]
0045ddf8: mov      r0, r8
0045ddfc: bl       #0x337888
0045de00: ldr      r1, [pc, #0xb8]
0045de04: add      r2, sp, #0x18
0045de08: mov      r0, r7
0045de0c: add      r1, pc, r1
0045de10: bl       #0x3140ec
0045de14: mov      r1, r7
0045de18: mov      r0, r8
0045de1c: bl       #0x337a88
0045de20: mov      r0, r7
0045de24: bl       #0x318254
0045de28: ldr      r8, [sb, #0xc]
0045de2c: ldr      r1, [pc, #0x90]
0045de30: mov      r0, r8
0045de34: add      r1, pc, r1
0045de38: bl       #0x30e6e8
0045de3c: cmp      r0, #0
0045de40: bne      #0x45de58
0045de44: ldr      r3, [pc, #0x7c]
0045de48: mov      r2, #1
0045de4c: ldr      r3, [r4, r3]
0045de50: strb     r2, [r3]
0045de54: b        #0x45ddcc
0045de58: ldr      r3, [pc, #0x6c]
0045de5c: add      r7, sp, #0xc
0045de60: mov      r2, r8
0045de64: ldr      r1, [r4, r3]
0045de68: mov      r0, r7
0045de6c: mov      r3, sl
0045de70: ldr      r1, [r1, #0x38]
0045de74: str      r6, [sp]
0045de78: str      r6, [sp, #4]
0045de7c: bl       #0x34aca0
0045de80: mov      r0, r7
0045de84: mov      r1, r6
0045de88: bl       #0x33fdc0
0045de8c: cmp      r0, #0
0045de90: beq      #0x45ddcc
0045de94: mov      r0, r7
0045de98: bl       #0x33ff54
0045de9c: cmp      r0, #0
0045dea0: ldrne    r3, [r0, #0x378]
0045dea4: movne    r2, #1
0045dea8: strbne   r2, [r3, #8]
0045deac: b        #0x45ddcc
0045deb0: bl       #0x30e310
0045deb4: subseq   r6, r3, r0, ror #25
0045deb8: andeq    r4, r0, ip, lsr #1
0045debc: andeq    r0, r0, r4, lsl #17
0045dec0: subeq    pc, r6, r4, ror r2
0045dec4: subeq    fp, r6, r4, lsl r1
0045dec8: andeq    r3, r0, r0, asr r6
0045decc: strdeq   r3, r4, [r0], -r4

# _ZN6CharAI13AI_BeginSkillEj
003d86bc: push     {r4, r5, r6, r7, r8, sl, lr}
003d86c0: mov      r4, r0
003d86c4: sub      sp, sp, #0xc
003d86c8: ldr      r0, [r0, #4]
003d86cc: mov      r7, r1
003d86d0: bl       #0x3bc784
003d86d4: ldr      r6, [r0, #0x48]
003d86d8: ldr      r5, [pc, #0x170]
003d86dc: mov      r8, r0
003d86e0: cmp      r6, #1
003d86e4: add      r5, pc, r5
003d86e8: beq      #0x3d876c
003d86ec: mov      r0, r4
003d86f0: mov      r1, r7
003d86f4: bl       #0x3d8358
003d86f8: cmp      r0, #0
003d86fc: bne      #0x3d8708
003d8700: add      sp, sp, #0xc
003d8704: pop      {r4, r5, r6, r7, r8, sl, pc}
003d8708: ldr      r0, [r4, #4]
003d870c: mov      r6, #0
003d8710: str      r7, [r4, #0xcc]
003d8714: strb     r6, [r4, #0xd0]
003d8718: strb     r6, [r4, #0xd1]
003d871c: add      r0, r0, #0x4f0
003d8720: ldrb     r2, [r8, #8]
003d8724: mov      r1, r7
003d8728: add      r0, r0, #0xc
003d872c: mov      r3, r6
003d8730: str      r6, [sp]
003d8734: bl       #0x3c6670
003d8738: ldr      r3, [r4, #4]
003d873c: mov      r0, r3
003d8740: ldr      r3, [r3]
003d8744: mov      lr, pc
003d8748: ldr      pc, [r3, #0x28]
003d874c: cmp      r0, r6
003d8750: bne      #0x3d8794
003d8754: ldr      r0, [r4, #4]
003d8758: add      r0, r0, #0x4f0
003d875c: add      r0, r0, #0xc
003d8760: add      sp, sp, #0xc
003d8764: pop      {r4, r5, r6, r7, r8, sl, lr}
003d8768: b        #0x3c02e8
003d876c: mov      r0, r4
003d8770: mov      r1, r7
003d8774: bl       #0x3d85d4
003d8778: cmp      r0, #0
003d877c: beq      #0x3d86ec
003d8780: ldr      r3, [r4, #0xb4]
003d8784: ldr      r0, [r3, r7, lsl #2]
003d8788: bl       #0x3da8b8
003d878c: mov      r0, r6
003d8790: b        #0x3d8700
003d8794: ldr      r0, [r4, #4]
003d8798: mov      r1, #0xd8
003d879c: mov      r2, #1
003d87a0: add      r0, r0, #0x560
003d87a4: bl       #0x3e0798
003d87a8: ldr      r3, [pc, #0xa4]
003d87ac: ldr      r0, [r4, #4]
003d87b0: mov      r1, #0xd8
003d87b4: ldr      r3, [r5, r3]
003d87b8: add      r0, r0, #0x560
003d87bc: mov      r2, r6
003d87c0: ldr      r7, [r3]
003d87c4: bl       #0x3df6e0
003d87c8: cmp      r0, #0xc7
003d87cc: ble      #0x3d8754
003d87d0: ldr      r3, [pc, #0x80]
003d87d4: ldr      r1, [r4, #4]
003d87d8: ldr      r3, [r5, r3]
003d87dc: ldr      r0, [r3, #0x40]
003d87e0: bl       #0x36effc
003d87e4: cmp      r0, r6
003d87e8: beq      #0x3d8754
003d87ec: ldr      r3, [pc, #0x68]
003d87f0: ldr      r3, [r5, r3]
003d87f4: ldr      sl, [r3]
003d87f8: cmp      sl, r6
003d87fc: beq      #0x3d8848
003d8800: ldr      r3, [pc, #0x58]
003d8804: ldr      r8, [pc, #0x58]
003d8808: ldr      r3, [r5, r3]
003d880c: add      r8, pc, r8
003d8810: ldr      r5, [r3]
003d8814: b        #0x3d8824
003d8818: add      r6, r6, #1
003d881c: cmp      r6, sl
003d8820: beq      #0x3d8848
003d8824: ldr      r1, [r5, r6, lsl #2]
003d8828: mov      r0, r8
003d882c: bl       #0x30e31c
003d8830: cmp      r0, #0
003d8834: bne      #0x3d8818
003d8838: mov      r1, r6
003d883c: mov      r0, r7
003d8840: bl       #0x3813b8
003d8844: b        #0x3d8754
003d8848: mvn      r1, #0
003d884c: b        #0x3d883c
003d8850: subseq   ip, fp, ip, lsr #7
003d8854: andeq    r1, r0, r0, ror sp
003d8858: strdeq   r3, r4, [r0], -r4
003d885c: strdeq   r0, r1, [r0], -ip
003d8860: andeq    r1, r0, ip, lsr #32
003d8864: strheq   ip, [lr], #-0xfc

# _ZN6CharAI11AI_EndSpellEb
003d7f60: push     {r4, r5, r6, lr}
003d7f64: mov      r4, r0
003d7f68: ldr      r0, [r0, #4]
003d7f6c: mov      r6, r1
003d7f70: add      r0, r0, #0x4f0
003d7f74: add      r0, r0, #0xc
003d7f78: bl       #0x3c0334
003d7f7c: cmp      r0, #0
003d7f80: bne      #0x3d7f88
003d7f84: pop      {r4, r5, r6, pc}
003d7f88: ldr      r5, [r4, #4]
003d7f8c: mvn      r1, #0
003d7f90: mov      r0, r5
003d7f94: bl       #0x3bb98c
003d7f98: mov      r1, r0
003d7f9c: mov      r0, r5
003d7fa0: bl       #0x3aeac0
003d7fa4: ldr      r3, [r0, #0x1c]
003d7fa8: cmp      r3, #2
003d7fac: bne      #0x3d7f84
003d7fb0: ldrb     r3, [r4, #0xd0]
003d7fb4: cmp      r3, #0
003d7fb8: moveq    r3, #1
003d7fbc: strbeq   r3, [r4, #0xd1]
003d7fc0: bne      #0x3d801c
003d7fc4: bl       #0x7fd794
003d7fc8: ldrb     r3, [r0, #5]
003d7fcc: cmp      r3, #0
003d7fd0: beq      #0x3d7f84
003d7fd4: cmp      r6, #0
003d7fd8: bne      #0x3d7f84
003d7fdc: bl       #0x80b1bc
003d7fe0: mov      r5, r0
003d7fe4: ldr      r0, [pc, #0x48]
003d7fe8: ldr      r3, [r4, #4]
003d7fec: mov      r1, #1
003d7ff0: add      r0, pc, r0
003d7ff4: ldrb     r4, [r3, #0x108]
003d7ff8: bl       #0x80a244
003d7ffc: mov      r3, #4
003d8000: mov      r1, r0
003d8004: strb     r4, [r0, #0x54]
003d8008: strb     r3, [r0, #0x50]
003d800c: strh     r6, [r0, #0x52]
003d8010: mov      r0, r5
003d8014: pop      {r4, r5, r6, lr}
003d8018: b        #0x80e2a4
003d801c: ldr      r0, [r4, #4]
003d8020: mov      r1, #1
003d8024: add      r0, r0, #0x490
003d8028: add      r0, r0, #0xc
003d802c: bl       #0x3c948c
003d8030: b        #0x3d7fc4
003d8034: subeq    r6, lr, r8, lsl pc

# _ZN10AISDefault11OnEndOfAnimEv
003dbeec: bx       lr

# _ZN16Script_MoveActor7ExecuteEbi
0045f0a8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045f0ac: ldr      r4, [pc, #0x234]
0045f0b0: ldr      fp, [pc, #0x234]
0045f0b4: ldr      ip, [pc, #0x234]
0045f0b8: add      r4, pc, r4
0045f0bc: ldr      r3, [r4, fp]
0045f0c0: ldr      r8, [r4, ip]
0045f0c4: sub      sp, sp, #0x4c
0045f0c8: ldr      r3, [r3]
0045f0cc: str      r1, [sp, #0xc]
0045f0d0: mov      r6, r0
0045f0d4: str      r3, [sp, #0x44]
0045f0d8: mov      r0, r8
0045f0dc: mov      sb, r2
0045f0e0: ldr      r7, [r6, #0xc]
0045f0e4: bl       #0x337888
0045f0e8: ldr      r1, [pc, #0x204]
0045f0ec: add      r5, sp, #0x2c
0045f0f0: add      r2, sp, #0x28
0045f0f4: add      r1, pc, r1
0045f0f8: mov      r0, r5
0045f0fc: ldr      sl, [pc, #0x1f4]
0045f100: bl       #0x3140ec
0045f104: mov      r1, r5
0045f108: mov      r0, r8
0045f10c: bl       #0x337a88
0045f110: mov      r0, r5
0045f114: bl       #0x318254
0045f118: ldr      r3, [r4, sl]
0045f11c: add      r8, sp, #0x1c
0045f120: ldr      r2, [r7, #0x18]
0045f124: ldr      r1, [r3, #0x38]
0045f128: mov      r5, #0
0045f12c: mov      r3, sb
0045f130: mov      r0, r8
0045f134: str      r5, [sp]
0045f138: str      r5, [sp, #4]
0045f13c: bl       #0x34aca0
0045f140: mov      r1, r5
0045f144: mov      r0, r8
0045f148: bl       #0x33fdc0
0045f14c: subs     r5, r0, #0
0045f150: bne      #0x45f2b0
0045f154: ldr      r3, [r4, sl]
0045f158: add      r8, sp, #0x10
0045f15c: ldr      r2, [r7, #0xc]
0045f160: ldr      r1, [r3, #0x38]
0045f164: mov      sl, #0
0045f168: mov      r3, sb
0045f16c: mov      r0, r8
0045f170: str      sl, [sp]
0045f174: str      sl, [sp, #4]
0045f178: bl       #0x34aca0
0045f17c: mov      r1, sl
0045f180: mov      r0, r8
0045f184: bl       #0x33fdc0
0045f188: cmp      r0, #0
0045f18c: moveq    sl, r0
0045f190: bne      #0x45f290
0045f194: ldrb     r3, [r7, #0x1c]
0045f198: cmp      r5, #0
0045f19c: str      r5, [r6, #0x14]
0045f1a0: strb     r3, [r6, #0x10]
0045f1a4: ldrbne   r3, [r5, #0x84]
0045f1a8: moveq    r3, r5
0045f1ac: cmp      r0, #0
0045f1b0: strb     r3, [r6, #0x18]
0045f1b4: strbeq   r0, [r6, #0x10]
0045f1b8: beq      #0x45f244
0045f1bc: ldrb     r3, [r7, #0x10]
0045f1c0: cmp      r3, #0
0045f1c4: bne      #0x45f2c0
0045f1c8: ldr      r2, [sp, #0xc]
0045f1cc: ldr      r3, [r5, #0x378]
0045f1d0: cmp      r2, #0
0045f1d4: mov      r2, #1
0045f1d8: strb     r2, [r3, #9]
0045f1dc: bne      #0x45f274
0045f1e0: ldrb     r3, [r6, #0x18]
0045f1e4: cmp      r3, #0
0045f1e8: beq      #0x45f210
0045f1ec: ldrb     r3, [r7, #0x1c]
0045f1f0: cmp      r3, #0
0045f1f4: beq      #0x45f238
0045f1f8: ldr      r1, [sp, #0xc]
0045f1fc: mov      r0, r5
0045f200: mov      r2, r1
0045f204: bl       #0x394bf8
0045f208: ldr      r3, [sp, #0xc]
0045f20c: strb     r3, [r5, #0x84]
0045f210: ldr      r0, [r5, #0x378]
0045f214: mov      r1, sl
0045f218: bl       #0x405540
0045f21c: mov      r2, r5
0045f220: ldr      r3, [r2, #0x200]!
0045f224: cmp      r3, r2
0045f228: beq      #0x45f26c
0045f22c: ldr      r3, [r3]
0045f230: cmp      r2, r3
0045f234: bne      #0x45f22c
0045f238: ldr      r3, [r5, #0x378]
0045f23c: mov      r2, #0
0045f240: strb     r2, [r3, #9]
0045f244: mov      r0, r8
0045f248: mov      r1, #0
0045f24c: bl       #0x33fdc0
0045f250: ldr      r3, [r4, fp]
0045f254: ldr      r2, [sp, #0x44]
0045f258: ldr      r3, [r3]
0045f25c: cmp      r2, r3
0045f260: bne      #0x45f2e4
0045f264: add      sp, sp, #0x4c
0045f268: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045f26c: mov      r3, #0
0045f270: strb     r3, [r6, #0x10]
0045f274: add      r1, sl, #0x160
0045f278: ldr      r0, [r5, #0x378]
0045f27c: bl       #0x405318
0045f280: ldr      r3, [r5, #0x378]
0045f284: mov      r2, #0
0045f288: strb     r2, [r3, #9]
0045f28c: b        #0x45f244
0045f290: mov      r0, r8
0045f294: bl       #0x33fee4
0045f298: cmp      r0, #0
0045f29c: cmpne    r5, #0
0045f2a0: mov      sl, r0
0045f2a4: moveq    r0, #0
0045f2a8: movne    r0, #1
0045f2ac: b        #0x45f194
0045f2b0: mov      r0, r8
0045f2b4: bl       #0x33ff54
0045f2b8: mov      r5, r0
0045f2bc: b        #0x45f154
0045f2c0: mov      r0, r5
0045f2c4: bl       #0x3949b0
0045f2c8: ldr      r2, [sp, #0xc]
0045f2cc: ldr      r3, [r5, #0x378]
0045f2d0: cmp      r2, #0
0045f2d4: mov      r2, #1
0045f2d8: strb     r2, [r3, #9]
0045f2dc: beq      #0x45f1e0
0045f2e0: b        #0x45f274
0045f2e4: bl       #0x30e310
0045f2e8: ldrsbeq  r5, [r3], #-0x98
0045f2ec: andeq    r4, r0, ip, lsr #1
0045f2f0: andeq    r0, r0, r4, lsl #17
0045f2f4: subeq    sp, r6, ip, lsl #31
0045f2f8: strdeq   r3, r4, [r0], -r4

# _ZN9Character22UpdateObjectOfInterestEv
003abb9c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003abba0: movw     r5, #0x14a4
003abba4: ldr      r3, [r0, r5]
003abba8: ldr      r6, [pc, #0x2cc]
003abbac: mov      r1, #0
003abbb0: cmp      r3, #0
003abbb4: movw     r2, #0x14ad
003abbb8: strb     r1, [r0, r2]
003abbbc: sub      sp, sp, #0x74
003abbc0: mov      r4, r0
003abbc4: add      r6, pc, r6
003abbc8: beq      #0x3abbf4
003abbcc: mov      r0, r3
003abbd0: mov      r1, r4
003abbd4: ldr      r3, [r3]
003abbd8: mov      lr, pc
003abbdc: ldr      pc, [r3, #0x8c]
003abbe0: cmp      r0, #0
003abbe4: bne      #0x3abc28
003abbe8: mov      r2, #0
003abbec: movw     r3, #0x14a4
003abbf0: str      r2, [r4, r3]
003abbf4: ldr      r7, [pc, #0x284]
003abbf8: movw     sl, #0x14aa
003abbfc: ldrh     r5, [r4, sl]
003abc00: ldr      r0, [r6, r7]
003abc04: bl       #0x31f66c
003abc08: rsb      r5, r0, r5
003abc0c: uxth     r5, r5
003abc10: sxth     r3, r5
003abc14: cmp      r3, #0
003abc18: strh     r5, [r4, sl]
003abc1c: ble      #0x3abc48
003abc20: add      sp, sp, #0x74
003abc24: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003abc28: ldr      r3, [r4, r5]
003abc2c: ldrb     r3, [r3, #0x81]
003abc30: cmp      r3, #0
003abc34: beq      #0x3abbf4
003abc38: mov      r2, #0
003abc3c: movw     r3, #0x14a4
003abc40: str      r2, [r4, r3]
003abc44: b        #0x3abbf4
003abc48: mvn      r0, #0
003abc4c: movw     r3, #0x14a8
003abc50: strb     r0, [r4, r3]
003abc54: mov      r3, #0x1f4
003abc58: strh     r3, [r4, sl]
003abc5c: mov      r2, #0
003abc60: movw     r3, #0x14ac
003abc64: strb     r2, [r4, r3]
003abc68: mov      r8, #1
003abc6c: movw     r1, #0x14a4
003abc70: add      r5, sp, #8
003abc74: ldr      fp, [r4, r1]
003abc78: mov      r3, r2
003abc7c: str      r2, [r4, r1]
003abc80: mov      r0, r5
003abc84: mov      r2, r8
003abc88: mov      r1, r4
003abc8c: str      r8, [sp]
003abc90: bl       #0x4a2730
003abc94: mov      r3, #0x59
003abc98: str      r3, [sp, #0x3c]
003abc9c: ldr      r2, [sp, #0x18]
003abca0: ldr      r3, [sp, #8]
003abca4: str      r8, [sp, #0x40]
003abca8: cmp      r2, r3
003abcac: beq      #0x3abcc8
003abcb0: mov      r0, r5
003abcb4: bl       #0x38fb18
003abcb8: ldr      r3, [sp, #8]
003abcbc: ldr      r2, [sp, #0x18]
003abcc0: cmp      r2, r3
003abcc4: bne      #0x3abcb0
003abcc8: ldr      r3, [pc, #0x1b4]
003abccc: mov      r0, r4
003abcd0: ldr      r3, [r6, r3]
003abcd4: str      r3, [sp, #0x30]
003abcd8: bl       #0x3935dc
003abcdc: ldr      r7, [r6, r7]
003abce0: ldr      r1, [pc, #0x1a0]
003abce4: ldr      r2, [pc, #0x1a0]
003abce8: mov      r8, r0
003abcec: add      r1, pc, r1
003abcf0: add      r2, pc, r2
003abcf4: ldr      r0, [r7, #0x2c]
003abcf8: bl       #0x4c4bdc
003abcfc: bl       #0x30e964
003abd00: ldr      r3, [pc, #0x188]
003abd04: ldr      lr, [r7, #0x38]
003abd08: mov      r2, r0
003abd0c: ldr      r3, [r6, r3]
003abd10: add      ip, lr, #0x80
003abd14: str      ip, [sp, #0x60]
003abd18: add      r3, r3, #8
003abd1c: str      r3, [sp, #0x5c]
003abd20: ldr      lr, [lr, #0x80]
003abd24: movw     r3, #0xfdb
003abd28: str      ip, [sp, #0x68]
003abd2c: mov      ip, #0
003abd30: movt     r3, #0x40c9
003abd34: str      ip, [sp, #0x6c]
003abd38: mov      r1, r8
003abd3c: add      ip, sp, #0x5c
003abd40: mov      r0, r5
003abd44: str      lr, [sp, #0x64]
003abd48: str      ip, [sp]
003abd4c: bl       #0x4a33c8
003abd50: ldr      r3, [pc, #0x13c]
003abd54: ldr      r7, [sp, #8]
003abd58: ldr      r2, [sp, #0x18]
003abd5c: ldr      r3, [r6, r3]
003abd60: cmp      r2, r7
003abd64: add      r3, r3, #8
003abd68: str      r3, [sp, #0x5c]
003abd6c: movwne   r6, #0x14a4
003abd70: movwne   sb, #0x14a8
003abd74: movwne   sl, #0x14ac
003abd78: bne      #0x3abde8
003abd7c: b        #0x3abe68
003abd80: str      r3, [r4, r6]
003abd84: ldr      r3, [r7]
003abd88: mov      r1, r4
003abd8c: mov      r0, r3
003abd90: ldr      r3, [r3]
003abd94: mov      lr, pc
003abd98: ldr      pc, [r3, #0x90]
003abd9c: uxtb     r0, r0
003abda0: strb     r0, [r4, sb]
003abda4: ldr      r3, [r7, #0xc]
003abda8: sxtb     r0, r0
003abdac: and      r3, r3, #1
003abdb0: cmp      r3, #0
003abdb4: strb     r3, [r4, sl]
003abdb8: bne      #0x3abe74
003abdbc: cmp      r0, #0
003abdc0: cmpne    r0, #2
003abdc4: beq      #0x3abe74
003abdc8: cmp      r0, #1
003abdcc: beq      #0x3abe34
003abdd0: mov      r0, r5
003abdd4: bl       #0x38fb18
003abdd8: ldr      r7, [sp, #8]
003abddc: ldr      r3, [sp, #0x18]
003abde0: cmp      r3, r7
003abde4: beq      #0x3abe68
003abde8: ldr      r3, [r7]
003abdec: movw     r8, #0x14a4
003abdf0: cmp      r4, r3
003abdf4: beq      #0x3abdd0
003abdf8: ldr      r2, [r4, r6]
003abdfc: cmp      r2, #0
003abe00: beq      #0x3abd80
003abe04: ldr      r2, [r7, #0xc]
003abe08: tst      r2, #1
003abe0c: bne      #0x3abd80
003abe10: mov      r0, r3
003abe14: mov      r1, r4
003abe18: ldr      r3, [r3]
003abe1c: mov      lr, pc
003abe20: ldr      pc, [r3, #0x90]
003abe24: cmp      r0, #1
003abe28: bne      #0x3abdd0
003abe2c: ldr      r3, [r7]
003abe30: b        #0x3abd80
003abe34: ldr      r3, [r4, r6]
003abe38: ldr      r2, [r3, #0x3bc]
003abe3c: cmp      r4, r2
003abe40: bne      #0x3abdd0
003abe44: cmp      r3, #0
003abe48: beq      #0x3abe5c
003abe4c: cmp      fp, r3
003abe50: movne    r2, #1
003abe54: movwne   r3, #0x14ad
003abe58: strbne   r2, [r4, r3]
003abe5c: mov      r0, r5
003abe60: bl       #0x38d18c
003abe64: b        #0x3abc20
003abe68: movw     r3, #0x14a4
003abe6c: ldr      r3, [r4, r3]
003abe70: b        #0x3abe44
003abe74: ldr      r3, [r4, r8]
003abe78: b        #0x3abe44
003abe7c: subseq   r8, lr, ip, asr #29
003abe80: strdeq   r3, r4, [r0], -r4
003abe84: ldrdeq   r1, r2, [r0], -r4
003abe88: subseq   r5, r1, r4, ror #20

# _ZN14v2ControllableC2Ev
00404db8: ldr      r3, [pc, #0x1c]
00404dbc: ldr      r2, [pc, #0x1c]
00404dc0: mov      ip, #0
00404dc4: add      r3, pc, r3
00404dc8: ldr      r2, [r3, r2]
00404dcc: str      ip, [r0, #4]
00404dd0: add      r2, r2, #8
00404dd4: str      r2, [r0]
00404dd8: bx       lr
00404ddc: subseq   pc, r8, ip, asr #25
00404de0: strdeq   r2, r3, [r0], -r0

# _ZN6CharAI6UpdateEv
003cfbf4: push     {r4, r5, r6, lr}
003cfbf8: mov      r5, r0
003cfbfc: ldr      r0, [pc, #0x140]
003cfc00: ldr      r4, [pc, #0x140]
003cfc04: add      r0, pc, r0
003cfc08: bl       #0x3136b4
003cfc0c: ldrb     r3, [r5, #0x18]
003cfc10: add      r4, pc, r4
003cfc14: cmp      r3, #0
003cfc18: bne      #0x3cfc44
003cfc1c: ldr      r6, [r5, #4]
003cfc20: ldr      r3, [r6, #0x378]
003cfc24: ldrb     r2, [r3, #9]
003cfc28: cmp      r2, #0
003cfc2c: bne      #0x3cfc60
003cfc30: ldr      r2, [pc, #0x114]
003cfc34: ldr      r2, [r4, r2]
003cfc38: ldrb     r2, [r2]
003cfc3c: cmp      r2, #0
003cfc40: beq      #0x3cfc54
003cfc44: ldr      r0, [pc, #0x104]
003cfc48: add      r0, pc, r0
003cfc4c: pop      {r4, r5, r6, lr}
003cfc50: b        #0x3136b8
003cfc54: ldrb     r3, [r3, #8]
003cfc58: cmp      r3, #0
003cfc5c: bne      #0x3cfc44
003cfc60: ldr      r3, [r6, #0x520]
003cfc64: tst      r3, #0x100
003cfc68: beq      #0x3cfc44
003cfc6c: ldr      r3, [r6]
003cfc70: mov      r0, r6
003cfc74: mov      lr, pc
003cfc78: ldr      pc, [r3, #0xc4]
003cfc7c: cmp      r0, #0
003cfc80: beq      #0x3cfc90
003cfc84: ldrb     r3, [r6, #0x2ee]
003cfc88: cmp      r3, #0
003cfc8c: bne      #0x3cfd34
003cfc90: ldr      r6, [pc, #0xbc]
003cfc94: ldr      r3, [r5, #4]
003cfc98: mov      r2, #1
003cfc9c: add      r6, pc, r6
003cfca0: ldr      r4, [pc, #0xb0]
003cfca4: strb     r2, [r3, #0x88]
003cfca8: mov      r0, r6
003cfcac: bl       #0x3136b4
003cfcb0: mov      r0, r5
003cfcb4: bl       #0x3cb908
003cfcb8: add      r4, pc, r4
003cfcbc: mov      r0, r6
003cfcc0: ldr      r6, [pc, #0x94]
003cfcc4: bl       #0x3136b8
003cfcc8: mov      r0, r4
003cfccc: bl       #0x3136b4
003cfcd0: mov      r0, r5
003cfcd4: bl       #0x3cc5a4
003cfcd8: add      r6, pc, r6
003cfcdc: mov      r0, r4
003cfce0: ldr      r4, [pc, #0x78]
003cfce4: bl       #0x3136b8
003cfce8: mov      r0, r6
003cfcec: bl       #0x3136b4
003cfcf0: mov      r0, r5
003cfcf4: bl       #0x3cf3f0
003cfcf8: add      r4, pc, r4
003cfcfc: mov      r0, r6
003cfd00: bl       #0x3136b8
003cfd04: mov      r0, r4
003cfd08: bl       #0x3136b4
003cfd0c: mov      r0, r5
003cfd10: ldr      r3, [r5]
003cfd14: mov      lr, pc
003cfd18: ldr      pc, [r3, #0x18]
003cfd1c: mov      r0, r4
003cfd20: bl       #0x3136b8
003cfd24: ldr      r0, [pc, #0x38]
003cfd28: add      r0, pc, r0
003cfd2c: pop      {r4, r5, r6, lr}
003cfd30: b        #0x3136b8
003cfd34: ldrb     r3, [r6, #0x2f0]
003cfd38: cmp      r3, #0
003cfd3c: beq      #0x3cfc44
003cfd40: b        #0x3cfc90
003cfd44: strdeq   r5, r6, [pc], #-0x7c
003cfd48: subseq   r4, ip, r0, lsl #29
003cfd4c: andeq    r3, r0, r0, asr r6
003cfd50: strheq   r5, [pc], #-0x78
003cfd54: subeq    r5, pc, ip, ror r7
003cfd58: subeq    r5, pc, r8, ror r7
003cfd5c: subeq    r5, pc, r0, ror r7
003cfd60: subeq    r5, pc, r8, ror #14
003cfd64: ldrdeq   r5, r6, [pc], #-0x68

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

# _ZN6CharAI16AI_DoRangeAttackEP10GameObjectb
003d076c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d0770: ldr      r4, [pc, #0x3dc]
003d0774: ldr      r5, [pc, #0x3dc]
003d0778: mov      r6, r0
003d077c: add      r4, pc, r4
003d0780: ldr      r0, [r4, r5]
003d0784: mov      r7, r1
003d0788: ldr      r3, [r6, #4]
003d078c: ldr      r1, [r0]
003d0790: sub      sp, sp, #0xc4
003d0794: mov      r0, r3
003d0798: str      r1, [sp, #0xbc]
003d079c: ldr      r3, [r3]
003d07a0: mov      r8, r2
003d07a4: mov      lr, pc
003d07a8: ldr      pc, [r3, #0x34]
003d07ac: cmp      r0, #0
003d07b0: bne      #0x3d07c4
003d07b4: ldr      r3, [r6, #4]
003d07b8: ldr      r2, [r3, #0x528]
003d07bc: tst      r2, #1
003d07c0: beq      #0x3d07e0
003d07c4: ldr      r3, [r4, r5]
003d07c8: ldr      r2, [sp, #0xbc]
003d07cc: ldr      r3, [r3]
003d07d0: cmp      r2, r3
003d07d4: bne      #0x3d0b50
003d07d8: add      sp, sp, #0xc4
003d07dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d07e0: mov      r0, r3
003d07e4: ldr      ip, [r3]
003d07e8: add      r1, sp, #0x64
003d07ec: add      r2, sp, #0x60
003d07f0: add      r3, sp, #0x5c
003d07f4: mov      lr, pc
003d07f8: ldr      pc, [ip, #0x128]
003d07fc: cmp      r0, #0
003d0800: beq      #0x3d0910
003d0804: ldr      r0, [r6, #4]
003d0808: add      r0, r0, #0x4f0
003d080c: add      r0, r0, #0xc
003d0810: bl       #0x3c02d0
003d0814: subs     sl, r0, #0
003d0818: beq      #0x3d0924
003d081c: ldrb     r7, [r6, #0x79]
003d0820: cmp      r7, #0
003d0824: bne      #0x3d07c4
003d0828: ldr      fp, [pc, #0x32c]
003d082c: add      r8, sp, #0xa4
003d0830: add      sl, sp, #8
003d0834: ldr      sb, [r4, fp]
003d0838: mov      r0, sb
003d083c: bl       #0x337888
003d0840: ldr      r1, [pc, #0x318]
003d0844: add      r2, sp, #0x70
003d0848: mov      r0, r8
003d084c: add      r1, pc, r1
003d0850: bl       #0x3140ec
003d0854: mov      r1, r8
003d0858: mov      r0, sb
003d085c: bl       #0x337a88
003d0860: mov      r0, r8
003d0864: bl       #0x318254
003d0868: mov      ip, #1
003d086c: ldr      r1, [r6, #4]
003d0870: mov      r3, r7
003d0874: mov      r2, ip
003d0878: mov      r0, sl
003d087c: str      ip, [sp]
003d0880: bl       #0x4a2730
003d0884: ldr      r3, [r6, #4]
003d0888: ldrb     r1, [r3, #0x1b5]
003d088c: cmp      r1, #0
003d0890: bne      #0x3d099c
003d0894: ldr      r3, [r6, #0x40]
003d0898: cmp      r3, #0
003d089c: beq      #0x3d08b0
003d08a0: mov      r0, r6
003d08a4: bl       #0x3d67f4
003d08a8: cmp      r0, #0
003d08ac: bne      #0x3d0ab4
003d08b0: mov      r0, sl
003d08b4: bl       #0x3d015c
003d08b8: ldr      r0, [sp, #0x60]
003d08bc: bl       #0x30e964
003d08c0: movw     r2, #0xfdb
003d08c4: mov      r1, r0
003d08c8: movt     r2, #0x40c9
003d08cc: mov      r0, sl
003d08d0: bl       #0x3d0020
003d08d4: ldr      r3, [sp, #8]
003d08d8: ldr      r2, [sp, #0x18]
003d08dc: cmp      r2, r3
003d08e0: beq      #0x3d0904
003d08e4: ldr      r1, [r3]
003d08e8: mov      r0, r6
003d08ec: mov      r2, #0
003d08f0: bl       #0x3d6890
003d08f4: ldr      r3, [sp, #8]
003d08f8: ldr      r0, [r6, #4]
003d08fc: ldr      r1, [r3]
003d0900: bl       #0x393d48
003d0904: mov      r0, sl
003d0908: bl       #0x38d18c
003d090c: b        #0x3d07c4
003d0910: mov      r0, r6
003d0914: mov      r1, r7
003d0918: mov      r2, r8
003d091c: bl       #0x3d01ac
003d0920: b        #0x3d07c4
003d0924: ldr      r3, [pc, #0x230]
003d0928: add      sb, sp, #0x74
003d092c: ldr      fp, [r4, r3]
003d0930: mov      r0, fp
003d0934: bl       #0x337888
003d0938: ldr      r1, [pc, #0x224]
003d093c: add      r2, sp, #0x68
003d0940: mov      r0, sb
003d0944: add      r1, pc, r1
003d0948: bl       #0x3140ec
003d094c: mov      r1, sb
003d0950: mov      r0, fp
003d0954: bl       #0x337a88
003d0958: mov      r0, sb
003d095c: bl       #0x318254
003d0960: cmp      r7, #0
003d0964: strb     sl, [r6, #0x78]
003d0968: beq      #0x3d09f4
003d096c: mov      r1, r7
003d0970: ldr      r0, [r6, #4]
003d0974: bl       #0x393d48
003d0978: cmp      r8, #0
003d097c: bne      #0x3d07c4
003d0980: ldr      r0, [r6, #4]
003d0984: mov      r1, r8
003d0988: mov      r2, r8
003d098c: add      r0, r0, #0x4f0
003d0990: add      r0, r0, #0xc
003d0994: bl       #0x3c6488
003d0998: b        #0x3d07c4
003d099c: ldr      r3, [pc, #0x1c4]
003d09a0: ldr      r1, [pc, #0x1c4]
003d09a4: ldr      r2, [pc, #0x1c4]
003d09a8: ldr      r3, [r4, r3]
003d09ac: add      r1, pc, r1
003d09b0: add      r2, pc, r2
003d09b4: ldr      r0, [r3, #0x2c]
003d09b8: bl       #0x4c4bdc
003d09bc: mov      r8, r0
003d09c0: ldr      r0, [sp, #0x60]
003d09c4: bl       #0x30e964
003d09c8: mov      r7, r0
003d09cc: asr      r0, r8, #1
003d09d0: bl       #0x30e964
003d09d4: movw     r1, #0xfa35
003d09d8: movt     r1, #0x3c8e
003d09dc: bl       #0x30ed6c
003d09e0: mov      r1, r7
003d09e4: mov      r2, r0
003d09e8: mov      r0, sl
003d09ec: bl       #0x3d0020
003d09f0: b        #0x3d08d4
003d09f4: mov      ip, #1
003d09f8: add      sl, sp, #8
003d09fc: ldr      r1, [r6, #4]
003d0a00: mov      r3, r7
003d0a04: mov      r2, ip
003d0a08: mov      r0, sl
003d0a0c: str      ip, [sp]
003d0a10: bl       #0x4a2730
003d0a14: ldr      r3, [r6, #4]
003d0a18: ldrb     r3, [r3, #0x1b5]
003d0a1c: cmp      r3, #0
003d0a20: beq      #0x3d0b28
003d0a24: ldr      r3, [pc, #0x13c]
003d0a28: ldr      r1, [pc, #0x144]
003d0a2c: ldr      r2, [pc, #0x144]
003d0a30: ldr      r3, [r4, r3]
003d0a34: add      r1, pc, r1
003d0a38: add      r2, pc, r2
003d0a3c: ldr      r0, [r3, #0x2c]
003d0a40: bl       #0x4c4bdc
003d0a44: mov      sb, r0
003d0a48: ldr      r0, [sp, #0x60]
003d0a4c: bl       #0x30e964
003d0a50: mov      r7, r0
003d0a54: asr      r0, sb, #1
003d0a58: bl       #0x30e964
003d0a5c: movw     r1, #0xfa35
003d0a60: movt     r1, #0x3c8e
003d0a64: bl       #0x30ed6c
003d0a68: mov      r1, r7
003d0a6c: mov      r2, r0
003d0a70: mov      r0, sl
003d0a74: bl       #0x3d0020
003d0a78: ldr      r3, [sp, #8]
003d0a7c: ldr      r2, [sp, #0x18]
003d0a80: cmp      r2, r3
003d0a84: beq      #0x3d0aa8
003d0a88: ldr      r1, [r3]
003d0a8c: mov      r0, r6
003d0a90: mov      r2, #0
003d0a94: bl       #0x3d6890
003d0a98: ldr      r3, [sp, #8]
003d0a9c: ldr      r0, [r6, #4]
003d0aa0: ldr      r1, [r3]
003d0aa4: bl       #0x393d48
003d0aa8: mov      r0, sl
003d0aac: bl       #0x38d18c
003d0ab0: b        #0x3d0978
003d0ab4: ldr      r3, [r6, #0x40]
003d0ab8: mov      r0, r3
003d0abc: ldr      r3, [r3]
003d0ac0: mov      lr, pc
003d0ac4: ldr      pc, [r3, #0x34]
003d0ac8: cmp      r0, #0
003d0acc: bne      #0x3d08b0
003d0ad0: ldr      r3, [r6, #4]
003d0ad4: mov      r0, r3
003d0ad8: ldr      r3, [r3]
003d0adc: mov      lr, pc
003d0ae0: ldr      pc, [r3, #0x28]
003d0ae4: cmp      r0, #0
003d0ae8: beq      #0x3d08d4
003d0aec: ldr      r8, [r4, fp]
003d0af0: add      r7, sp, #0x8c
003d0af4: mov      r0, r8
003d0af8: bl       #0x337888
003d0afc: ldr      r1, [pc, #0x78]
003d0b00: add      r2, sp, #0x6c
003d0b04: mov      r0, r7
003d0b08: add      r1, pc, r1
003d0b0c: bl       #0x3140ec
003d0b10: mov      r0, r8
003d0b14: mov      r1, r7
003d0b18: bl       #0x337a88
003d0b1c: mov      r0, r7
003d0b20: bl       #0x318254
003d0b24: b        #0x3d08d4
003d0b28: mov      r0, sl
003d0b2c: bl       #0x3d015c
003d0b30: ldr      r0, [sp, #0x60]
003d0b34: bl       #0x30e964
003d0b38: movw     r2, #0xfdb
003d0b3c: mov      r1, r0
003d0b40: movt     r2, #0x40c9
003d0b44: mov      r0, sl
003d0b48: bl       #0x3d0020
003d0b4c: b        #0x3d0a78
003d0b50: bl       #0x30e310
003d0b54: subseq   r4, ip, r4, lsl r3
003d0b58: andeq    r4, r0, ip, lsr #1
003d0b5c: andeq    r0, r0, r4, lsl #17
003d0b60: subeq    r4, pc, r4, asr #24
003d0b64: subeq    r4, pc, ip, asr #22
003d0b68: strdeq   r3, r4, [r0], -r4
003d0b6c: subeq    r0, pc, r4, lsr #27
003d0b70: strdeq   r4, r5, [pc], #-0xa8
003d0b74: subeq    r0, pc, ip, lsl sp
003d0b78: subeq    r4, pc, r0, ror sl
003d0b7c: strheq   r4, [pc], #-0x98

# _ZN14v2Controllable13SetControllerEP12v2Controller
00404e10: push     {r4, r5, r6, lr}
00404e14: ldr      r3, [r0, #4]
00404e18: mov      r4, r0
00404e1c: mov      r5, r1
00404e20: cmp      r3, r1
00404e24: beq      #0x404e4c
00404e28: cmp      r3, #0
00404e2c: beq      #0x404e48
00404e30: mov      r0, r3
00404e34: ldr      r3, [r3]
00404e38: mov      lr, pc
00404e3c: ldr      pc, [r3, #4]
00404e40: mov      r3, #0
00404e44: str      r3, [r4, #4]
00404e48: str      r5, [r4, #4]
00404e4c: pop      {r4, r5, r6, pc}

# _ZN6CharAI11OnPreAttackEi
003d0ed4: push     {r4, r5, r6, lr}
003d0ed8: mov      r4, r0
003d0edc: ldr      r0, [r0, #4]
003d0ee0: mov      r5, r1
003d0ee4: mov      r1, #0
003d0ee8: add      r0, r0, #0x3c8
003d0eec: bl       #0x3d67f4
003d0ef0: cmp      r0, #0
003d0ef4: beq      #0x3d0f18
003d0ef8: ldr      r3, [r4, #0x1c]
003d0efc: cmp      r3, #0
003d0f00: beq      #0x3d0f18
003d0f04: mov      r0, r3
003d0f08: mov      r1, r5
003d0f0c: ldr      r3, [r3]
003d0f10: mov      lr, pc
003d0f14: ldr      pc, [r3, #0xa4]
003d0f18: pop      {r4, r5, r6, pc}

# _ZN6CharAIC2Ev
003cebf0: ldr      r3, [pc, #0x14c]
003cebf4: ldr      r2, [pc, #0x14c]
003cebf8: push     {r4, r5, lr}
003cebfc: add      r3, pc, r3
003cec00: ldr      r2, [r3, r2]
003cec04: mov      r4, r0
003cec08: mov      r1, #0
003cec0c: add      r2, r2, #8
003cec10: str      r2, [r4]
003cec14: ldr      r2, [pc, #0x130]
003cec18: mov      r0, #1
003cec1c: mvn      ip, #0
003cec20: mov      r5, r4
003cec24: strb     r0, [r4, #0x55]
003cec28: str      r1, [r4, #8]
003cec2c: str      r1, [r4, #0xc]
003cec30: strb     r1, [r4, #0x18]
003cec34: str      r1, [r4, #0x1c]
003cec38: str      r1, [r4, #0x20]
003cec3c: strb     r1, [r4, #0x24]
003cec40: str      r1, [r4, #0x28]
003cec44: strb     r1, [r4, #0x2c]
003cec48: str      r1, [r4, #0x30]
003cec4c: str      r1, [r4, #0x34]
003cec50: str      r1, [r4, #0x3c]
003cec54: str      r1, [r4, #0x40]
003cec58: str      r1, [r4, #0x44]
003cec5c: strb     r1, [r4, #0x49]
003cec60: strb     r0, [r4, #0x4a]
003cec64: strb     r0, [r4, #0x4b]
003cec68: strb     r1, [r4, #0x4c]
003cec6c: strb     r0, [r4, #0x4d]
003cec70: str      r1, [r4, #0x50]
003cec74: strb     r0, [r4, #0x54]
003cec78: str      r1, [r4, #0x58]
003cec7c: mov      r0, r4
003cec80: str      r1, [r4, #0x60]
003cec84: str      ip, [r4, #0x10]
003cec88: str      ip, [r4, #0x14]
003cec8c: str      ip, [r4, #0x38]
003cec90: strb     r1, [r5, #0x5c]!
003cec94: str      r5, [r4, #0x68]
003cec98: str      r5, [r4, #0x64]
003cec9c: str      r1, [r4, #0x6c]
003ceca0: str      r1, [r4, #0x80]
003ceca4: strb     r1, [r0, #0x7c]!
003ceca8: ldr      r5, [r3, r2]
003cecac: mov      r2, r4
003cecb0: str      r0, [r4, #0x88]
003cecb4: str      r0, [r4, #0x84]
003cecb8: str      r1, [r4, #0x8c]
003cecbc: str      r1, [r4, #0x98]
003cecc0: add      r0, r4, #0xac
003cecc4: strb     r1, [r2, #0x94]!
003cecc8: str      r2, [r4, #0xa0]
003ceccc: str      r0, [r4, #0xb0]
003cecd0: str      ip, [r4, #0xcc]
003cecd4: strb     r1, [r4, #0xd1]
003cecd8: str      r2, [r4, #0x9c]
003cecdc: str      r1, [r4, #0xa4]
003cece0: str      r0, [r4, #0xac]
003cece4: str      r1, [r4, #0xb4]
003cece8: str      r1, [r4, #0xb8]
003cecec: str      r1, [r4, #0xbc]
003cecf0: str      r1, [r4, #0xc0]
003cecf4: str      r1, [r4, #0xc4]
003cecf8: str      r1, [r4, #0xc8]
003cecfc: strb     r1, [r4, #0xd0]
003ced00: ldr      r1, [r5, #0x18]
003ced04: ldr      r2, [r5, #0x10]
003ced08: sub      sp, sp, #0xc
003ced0c: sub      r3, r1, #4
003ced10: cmp      r2, r3
003ced14: str      r4, [sp, #4]
003ced18: beq      #0x3ced38
003ced1c: str      r4, [r2]
003ced20: ldr      r3, [r5, #0x10]
003ced24: add      r3, r3, #4
003ced28: str      r3, [r5, #0x10]
003ced2c: mov      r0, r4
003ced30: add      sp, sp, #0xc
003ced34: pop      {r4, r5, pc}
003ced38: add      r0, sp, #4
003ced3c: bl       #0x3ce810
003ced40: b        #0x3ced2c

# _ZN13ScriptManager5FlushEv
0045a2ac: push     {r4, r5, r6, lr}
0045a2b0: mvn      r2, #0
0045a2b4: mov      r4, #0
0045a2b8: str      r2, [r0]
0045a2bc: strb     r4, [r0, #0x30]
0045a2c0: str      r4, [r0, #4]
0045a2c4: str      r4, [r0, #8]
0045a2c8: ldr      r5, [pc, #0x14]
0045a2cc: bl       #0x45a1c8
0045a2d0: ldr      r3, [pc, #0x10]
0045a2d4: add      r5, pc, r5
0045a2d8: ldr      r3, [r5, r3]
0045a2dc: strb     r4, [r3]
0045a2e0: pop      {r4, r5, r6, pc}
0045a2e4: ldrheq   sl, [r3], #-0x7c
0045a2e8: andeq    r3, r0, r0, asr r6

# _ZN11HUDControls6UpdateEv
0041a780: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0041a784: ldr      r4, [pc, #0x25c]
0041a788: ldr      r3, [pc, #0x25c]
0041a78c: sub      sp, sp, #0x20
0041a790: add      r4, pc, r4
0041a794: ldr      r3, [r4, r3]
0041a798: mov      r5, r0
0041a79c: ldrb     r3, [r3, #0x30]
0041a7a0: cmp      r3, #0
0041a7a4: beq      #0x41a910
0041a7a8: ldr      r3, [pc, #0x240]
0041a7ac: mov      r2, #1
0041a7b0: ldr      r3, [r4, r3]
0041a7b4: strb     r2, [r3]
0041a7b8: ldr      r6, [pc, #0x234]
0041a7bc: mov      r3, #0
0041a7c0: strb     r3, [r5, #0x84]
0041a7c4: ldr      r0, [r4, r6]
0041a7c8: bl       #0x31f594
0041a7cc: cmp      r0, #0
0041a7d0: beq      #0x41a7e0
0041a7d4: ldrb     r3, [r0, #0x198]
0041a7d8: cmp      r3, #0
0041a7dc: beq      #0x41a920
0041a7e0: ldr      r3, [r5, #0x658]
0041a7e4: cmp      r3, #0
0041a7e8: beq      #0x41a908
0041a7ec: ldrb     r3, [r5, #8]
0041a7f0: cmp      r3, #0
0041a7f4: beq      #0x41a940
0041a7f8: ldr      r3, [r4, r6]
0041a7fc: mov      r1, #0
0041a800: mov      r2, r1
0041a804: ldr      r0, [r3, #0x40]
0041a808: bl       #0x36e478
0041a80c: ldr      r6, [r0, #0x660]
0041a810: cmp      r6, #0
0041a814: beq      #0x41a908
0041a818: ldrb     r3, [r5, #9]
0041a81c: cmp      r3, #0
0041a820: beq      #0x41a840
0041a824: movw     r3, #0x14a4
0041a828: ldr      r1, [r6, r3]
0041a82c: cmp      r1, #0
0041a830: beq      #0x41a9d8
0041a834: ldr      r0, [r6, #0x378]
0041a838: mov      r1, #0
0041a83c: bl       #0x4057fc
0041a840: ldrb     r3, [r5, #0xa]
0041a844: cmp      r3, #0
0041a848: bne      #0x41a978
0041a84c: ldrb     r7, [r5, #9]
0041a850: cmp      r7, #0
0041a854: bne      #0x41a908
0041a858: ldr      r0, [r5, #0x7c]
0041a85c: cmp      r0, #0
0041a860: ble      #0x41a908
0041a864: ldr      r5, [r5, #0x80]
0041a868: cmp      r5, #0
0041a86c: ble      #0x41a908
0041a870: bl       #0x30e964
0041a874: mov      r8, r0
0041a878: mov      r0, r5
0041a87c: bl       #0x30e964
0041a880: ldr      r2, [pc, #0x170]
0041a884: mov      r3, #0
0041a888: str      r0, [sp, #0x1c]
0041a88c: add      r1, sp, #0x18
0041a890: ldr      r0, [r4, r2]
0041a894: mov      r2, sp
0041a898: str      r3, [sp, #8]
0041a89c: str      r8, [sp, #0x18]
0041a8a0: str      r3, [sp]
0041a8a4: str      r3, [sp, #4]
0041a8a8: bl       #0x525884
0041a8ac: cmp      r0, #0
0041a8b0: mov      r5, sp
0041a8b4: beq      #0x41a908
0041a8b8: movw     r4, #0x149c
0041a8bc: ldr      r0, [r6, r4]
0041a8c0: ldr      r1, [sp]
0041a8c4: ldr      r2, [sp, #4]
0041a8c8: cmp      r0, #0
0041a8cc: ldr      r3, [sp, #8]
0041a8d0: beq      #0x41a8fc
0041a8d4: str      r1, [r0, #0x34]
0041a8d8: str      r2, [r0, #0x38]
0041a8dc: str      r3, [r0, #0x3c]
0041a8e0: mov      r1, r7
0041a8e4: bl       #0x492aa0
0041a8e8: ldr      r0, [r6, r4]
0041a8ec: cmp      r0, #0
0041a8f0: beq      #0x41a8fc
0041a8f4: mov      r1, #1
0041a8f8: bl       #0x492ef0
0041a8fc: ldr      r0, [r6, #0x378]
0041a900: mov      r1, sp
0041a904: bl       #0x4054e4
0041a908: add      sp, sp, #0x20
0041a90c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0041a910: ldr      r2, [pc, #0xd8]
0041a914: ldr      r2, [r4, r2]
0041a918: strb     r3, [r2]
0041a91c: b        #0x41a7b8
0041a920: ldrb     r2, [r5, #9]
0041a924: cmp      r2, #0
0041a928: strbne   r3, [r5, #9]
0041a92c: ldrb     r3, [r5, #0xa]
0041a930: cmp      r3, #0
0041a934: movne    r3, #0
0041a938: strbne   r3, [r5, #0xa]
0041a93c: b        #0x41a908
0041a940: mov      r0, r5
0041a944: bl       #0x419b4c
0041a948: mvn      r3, #0
0041a94c: str      r3, [r5, #0x80]
0041a950: str      r3, [r5, #0x7c]
0041a954: ldr      r3, [r4, r6]
0041a958: mov      r1, #0
0041a95c: mov      r2, r1
0041a960: ldr      r0, [r3, #0x40]
0041a964: bl       #0x36e478
0041a968: ldr      r6, [r0, #0x660]
0041a96c: cmp      r6, #0
0041a970: bne      #0x41a818
0041a974: b        #0x41a908
0041a978: mov      r0, r6
0041a97c: bl       #0x3ad430
0041a980: cmp      r0, #0
0041a984: beq      #0x41a84c
0041a988: ldr      r7, [r5, #0x668]
0041a98c: ldr      r1, [r5, #0x660]
0041a990: ldr      sb, [r6, #0x378]
0041a994: mov      r0, r7
0041a998: bl       #0x30ed6c
0041a99c: ldr      r1, [r5, #0x664]
0041a9a0: mov      sl, r0
0041a9a4: mov      r0, r7
0041a9a8: bl       #0x30ed6c
0041a9ac: mov      r1, r7
0041a9b0: mov      r8, r0
0041a9b4: ldr      r0, [r5, #0x65c]
0041a9b8: bl       #0x30ed6c
0041a9bc: add      r1, sp, #0xc
0041a9c0: str      r0, [sp, #0xc]
0041a9c4: mov      r0, sb
0041a9c8: str      sl, [sp, #0x10]
0041a9cc: str      r8, [sp, #0x14]
0041a9d0: bl       #0x405374
0041a9d4: b        #0x41a84c
0041a9d8: strb     r1, [r6, #0x413]
0041a9dc: ldr      r0, [r6, #0x378]
0041a9e0: bl       #0x405b04
0041a9e4: b        #0x41a840
0041a9e8: subseq   sl, r7, r0, lsl #6
0041a9ec: andeq    r1, r0, r0, lsr #20
0041a9f0: strdeq   r3, r4, [r0], -r8
0041a9f4: strdeq   r3, r4, [r0], -r4
0041a9f8: andeq    r1, r0, r4, lsl #4

# _ZN6CharAI12SetCharacterEP9Character
003cb7c0: push     {r4, r5, lr}
003cb7c4: ldr      r3, [pc, #0x70]
003cb7c8: subs     r4, r1, #0
003cb7cc: sub      sp, sp, #0xc
003cb7d0: mov      r5, r0
003cb7d4: add      r3, pc, r3
003cb7d8: beq      #0x3cb7e8
003cb7dc: str      r4, [r5, #4]
003cb7e0: add      sp, sp, #0xc
003cb7e4: pop      {r4, r5, pc}
003cb7e8: ldr      r2, [pc, #0x50]
003cb7ec: ldr      r2, [r3, r2]
003cb7f0: ldr      r2, [r2]
003cb7f4: cmp      r2, #2
003cb7f8: streq    r4, [r4]
003cb7fc: beq      #0x3cb7dc
003cb800: cmp      r2, #1
003cb804: bne      #0x3cb7dc
003cb808: ldr      r0, [pc, #0x34]
003cb80c: ldr      r1, [pc, #0x34]
003cb810: ldr      r2, [pc, #0x34]
003cb814: ldr      r0, [r3, r0]
003cb818: ldr      r3, [pc, #0x30]
003cb81c: movw     ip, #0x1c7
003cb820: add      r1, pc, r1
003cb824: add      r2, pc, r2
003cb828: add      r3, pc, r3
003cb82c: add      r0, r0, #0xa8
003cb830: str      ip, [sp]
003cb834: bl       #0x30e004
003cb838: b        #0x3cb7dc
003cb83c: ldrheq   sb, [ip], #-0x2c
003cb840: andeq    r3, r0, r0, asr #19
003cb844: andeq    r1, r0, r0, asr #19
003cb848: strheq   r2, [pc], #-0xb8
003cb84c: subseq   r6, r2, r4, ror #16
003cb850: umaaleq  sb, pc, r0, sb

# _ZN6CharAI12AI_SetTargetEP10GameObjectb
003d6890: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr      r5, [pc, #0x204]
003d6898: ldr      r7, [pc, #0x204]
003d689c: sub      sp, sp, #0x78
003d68a0: add      r5, pc, r5
003d68a4: ldr      r3, [r5, r7]
003d68a8: mov      r4, r0
003d68ac: cmp      r2, #0
003d68b0: ldr      r3, [r3]
003d68b4: mov      r6, r1
003d68b8: str      r1, [r4, #0x3c]
003d68bc: str      r3, [sp, #0x74]
003d68c0: bne      #0x3d6a20
003d68c4: ldr      r3, [r0, #0x40]
003d68c8: ldr      sb, [pc, #0x1d8]
003d68cc: add      r8, sp, #0x5c
003d68d0: cmp      r3, r1
003d68d4: ldrne    r1, [r0, #4]
003d68d8: ldr      sl, [r5, sb]
003d68dc: movwne   r3, #0x14d0
003d68e0: strhne   r2, [r1, r3]
003d68e4: mov      r0, sl
003d68e8: bl       #0x337888
003d68ec: ldr      r1, [pc, #0x1b8]
003d68f0: add      r2, sp, #0x10
003d68f4: mov      r0, r8
003d68f8: add      r1, pc, r1
003d68fc: bl       #0x3140ec
003d6900: mov      r0, sl
003d6904: mov      r1, r8
003d6908: bl       #0x337a88
003d690c: mov      sl, r0
003d6910: ldr      r0, [sp, #0x70]
003d6914: cmp      r0, r8
003d6918: beq      #0x3d6938
003d691c: cmp      r0, #0
003d6920: beq      #0x3d6938
003d6924: ldr      r1, [sp, #0x5c]
003d6928: rsb      r1, r0, r1
003d692c: cmp      r1, #0x80
003d6930: bhi      #0x3d6a4c
003d6934: bl       #0x708f00
003d6938: cmp      sl, #0
003d693c: beq      #0x3d69a4
003d6940: ldr      r3, [r4, #0x40]
003d6944: cmp      r3, r6
003d6948: beq      #0x3d69a4
003d694c: subs     r3, r3, #0
003d6950: movne    r3, #1
003d6954: subs     r2, r6, #0
003d6958: movne    r2, #1
003d695c: tst      r2, r3
003d6960: bne      #0x3d6a28
003d6964: cmp      r3, #0
003d6968: beq      #0x3d6a18
003d696c: ldr      sl, [r5, sb]
003d6970: add      r8, sp, #0x2c
003d6974: mov      r0, sl
003d6978: bl       #0x337888
003d697c: ldr      r1, [pc, #0x12c]
003d6980: add      r2, sp, #8
003d6984: mov      r0, r8
003d6988: add      r1, pc, r1
003d698c: bl       #0x3140ec
003d6990: mov      r0, sl
003d6994: mov      r1, r8
003d6998: bl       #0x337a88
003d699c: mov      r0, r8
003d69a0: bl       #0x318254
003d69a4: cmp      r6, #0
003d69a8: str      r6, [r4, #0x40]
003d69ac: beq      #0x3d69fc
003d69b0: ldr      r0, [r4, #4]
003d69b4: bl       #0x3a2fec
003d69b8: ldr      r2, [r4, #0x44]
003d69bc: ldr      r3, [r4, #0x40]
003d69c0: cmp      r3, r2
003d69c4: movne    r2, #0
003d69c8: strbne   r2, [r4, #0x4c]
003d69cc: movne    r2, r3
003d69d0: str      r2, [r4, #0x44]
003d69d4: mov      r0, r3
003d69d8: ldr      r3, [r3]
003d69dc: mov      lr, pc
003d69e0: ldr      pc, [r3, #0x34]
003d69e4: eor      r0, r0, #1
003d69e8: strb     r0, [r4, #0x48]
003d69ec: ldr      r1, [r4, #0x40]
003d69f0: mov      r0, r4
003d69f4: bl       #0x3d4ed8
003d69f8: strb     r0, [r4, #0x49]
003d69fc: ldr      r3, [r5, r7]
003d6a00: ldr      r2, [sp, #0x74]
003d6a04: ldr      r3, [r3]
003d6a08: cmp      r2, r3
003d6a0c: bne      #0x3d6a9c
003d6a10: add      sp, sp, #0x78
003d6a14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp      r2, #0
003d6a1c: bne      #0x3d6a5c
003d6a20: str      r6, [r4, #0x40]
003d6a24: b        #0x3d69fc
003d6a28: ldr      sl, [r5, sb]
003d6a2c: add      r8, sp, #0x44
003d6a30: mov      r0, sl
003d6a34: bl       #0x337888
003d6a38: ldr      r1, [pc, #0x74]
003d6a3c: add      r2, sp, #0xc
003d6a40: mov      r0, r8
003d6a44: add      r1, pc, r1
003d6a48: b        #0x3d698c
003d6a4c: bl       #0x310440
003d6a50: cmp      sl, #0
003d6a54: beq      #0x3d69a4
003d6a58: b        #0x3d6940
003d6a5c: ldr      sl, [r5, sb]
003d6a60: add      r8, sp, #0x14
003d6a64: mov      r0, sl
003d6a68: bl       #0x337888
003d6a6c: ldr      r1, [pc, #0x44]
003d6a70: add      r2, sp, #4
003d6a74: mov      r0, r8
003d6a78: add      r1, pc, r1
003d6a7c: bl       #0x3140ec
003d6a80: mov      r1, r8
003d6a84: mov      r0, sl
003d6a88: bl       #0x337a88
003d6a8c: mov      r0, r8
003d6a90: bl       #0x318254
003d6a94: str      r6, [r4, #0x40]
003d6a98: b        #0x3d69b0
003d6a9c: bl       #0x30e310
003d6aa0: ldrsheq  lr, [fp], #-0x10
003d6aa4: andeq    r4, r0, ip, lsr #1
003d6aa8: andeq    r0, r0, r4, lsl #17
003d6aac: subeq    lr, lr, r0, ror #27
003d6ab0: subeq    lr, lr, r8, ror #26
003d6ab4: subeq    lr, lr, ip, lsr #25
003d6ab8: subeq    lr, lr, r8, ror ip

# _ZN6CharAI11OnEndOfAnimEv
003d0ce8: push     {r4, lr}
003d0cec: ldr      r3, [r0, #0x1c]
003d0cf0: cmp      r3, #0
003d0cf4: beq      #0x3d0d08
003d0cf8: mov      r0, r3
003d0cfc: ldr      r3, [r3]
003d0d00: mov      lr, pc
003d0d04: ldr      pc, [r3, #0x98]
003d0d08: pop      {r4, pc}

# _ZN22Script_UnlockCharacter7ExecuteEbi
0045dc80: push     {r4, r5, r6, r7, r8, sl, lr}
0045dc84: ldr      r4, [pc, #0xf8]
0045dc88: ldr      r5, [pc, #0xf8]
0045dc8c: ldr      r1, [pc, #0xf8]
0045dc90: add      r4, pc, r4
0045dc94: ldr      r3, [r4, r5]
0045dc98: ldr      r7, [r4, r1]
0045dc9c: sub      sp, sp, #0x3c
0045dca0: ldr      r3, [r3]
0045dca4: mov      r8, r2
0045dca8: add      r6, sp, #0x1c
0045dcac: str      r3, [sp, #0x34]
0045dcb0: ldr      sl, [r0, #0xc]
0045dcb4: mov      r0, r7
0045dcb8: bl       #0x337888
0045dcbc: ldr      r1, [pc, #0xcc]
0045dcc0: add      r2, sp, #0x18
0045dcc4: mov      r0, r6
0045dcc8: add      r1, pc, r1
0045dccc: bl       #0x3140ec
0045dcd0: mov      r1, r6
0045dcd4: mov      r0, r7
0045dcd8: bl       #0x337a88
0045dcdc: mov      r0, r6
0045dce0: bl       #0x318254
0045dce4: ldr      r6, [sl, #0xc]
0045dce8: ldr      r1, [pc, #0xa4]
0045dcec: mov      r0, r6
0045dcf0: add      r1, pc, r1
0045dcf4: bl       #0x30e31c
0045dcf8: cmp      r0, #0
0045dcfc: bne      #0x45dd28
0045dd00: ldr      r3, [pc, #0x90]
0045dd04: ldr      r3, [r4, r3]
0045dd08: strb     r0, [r3]
0045dd0c: ldr      r3, [r4, r5]
0045dd10: ldr      r2, [sp, #0x34]
0045dd14: ldr      r3, [r3]
0045dd18: cmp      r2, r3
0045dd1c: bne      #0x45dd80
0045dd20: add      sp, sp, #0x3c
0045dd24: pop      {r4, r5, r6, r7, r8, sl, pc}
0045dd28: ldr      r3, [pc, #0x6c]
0045dd2c: add      r7, sp, #0xc
0045dd30: mov      r2, r6
0045dd34: ldr      r1, [r4, r3]
0045dd38: mov      r6, #0
0045dd3c: mov      r3, r8
0045dd40: ldr      r1, [r1, #0x38]
0045dd44: mov      r0, r7
0045dd48: str      r6, [sp]
0045dd4c: str      r6, [sp, #4]
0045dd50: bl       #0x34aca0
0045dd54: mov      r0, r7
0045dd58: mov      r1, r6
0045dd5c: bl       #0x33fdc0
0045dd60: cmp      r0, r6
0045dd64: beq      #0x45dd0c
0045dd68: mov      r0, r7
0045dd6c: bl       #0x33ff54
0045dd70: cmp      r0, #0
0045dd74: ldrne    r3, [r0, #0x378]
0045dd78: strbne   r6, [r3, #8]
0045dd7c: b        #0x45dd0c
0045dd80: bl       #0x30e310
0045dd84: subseq   r6, r3, r0, lsl #28
0045dd88: andeq    r4, r0, ip, lsr #1
0045dd8c: andeq    r0, r0, r4, lsl #17
0045dd90: strheq   pc, [r6], #-0x38
0045dd94: subeq    fp, r6, r8, asr r2
0045dd98: andeq    r3, r0, r0, asr r6
0045dd9c: strdeq   r3, r4, [r0], -r4

# _ZN6CharAI12RaiseAIEventEiPv
003cbb34: ldr      r3, [pc, #0x6d4]
003cbb38: push     {r4, r5, r6, r7, r8, lr}
003cbb3c: add      r3, pc, r3
003cbb40: mov      r4, r1
003cbb44: mov      r5, r0
003cbb48: mov      r6, r2
003cbb4c: cmp      r1, #0x3f
003cbb50: addls    pc, pc, r1, lsl #2
003cbb54: b        #0x3cbcfc
003cbb58: b        #0x3cbc58
003cbb5c: b        #0x3cbe90
003cbb60: b        #0x3cbe98
003cbb64: b        #0x3cbe2c
003cbb68: b        #0x3cbcfc
003cbb6c: b        #0x3cbcfc
003cbb70: b        #0x3cbcfc
003cbb74: b        #0x3cbcfc
003cbb78: b        #0x3cbcfc
003cbb7c: b        #0x3cbcfc
003cbb80: b        #0x3cbcfc
003cbb84: b        #0x3cbcfc
003cbb88: b        #0x3cbcfc
003cbb8c: b        #0x3cbcfc
003cbb90: b        #0x3cbcfc
003cbb94: b        #0x3cbcfc
003cbb98: b        #0x3cbcfc
003cbb9c: b        #0x3cbcfc
003cbba0: b        #0x3cbcfc
003cbba4: b        #0x3cbcfc
003cbba8: b        #0x3cbcfc
003cbbac: b        #0x3cbcfc
003cbbb0: b        #0x3cbcfc
003cbbb4: b        #0x3cbcfc
003cbbb8: b        #0x3cbcfc
003cbbbc: b        #0x3cbcfc
003cbbc0: b        #0x3cbcfc
003cbbc4: b        #0x3cbcfc
003cbbc8: b        #0x3cbcfc
003cbbcc: b        #0x3cbcfc
003cbbd0: b        #0x3cbcfc
003cbbd4: b        #0x3cbcfc
003cbbd8: b        #0x3cbcfc
003cbbdc: b        #0x3cbcfc
003cbbe0: b        #0x3cbe40
003cbbe4: b        #0x3cbe64
003cbbe8: b        #0x3cbcfc
003cbbec: b        #0x3cbcfc
003cbbf0: b        #0x3cbcfc
003cbbf4: b        #0x3cbcfc
003cbbf8: b        #0x3cbe80
003cbbfc: b        #0x3cbc78
003cbc00: b        #0x3cbcfc
003cbc04: b        #0x3cbcfc
003cbc08: b        #0x3cbcfc
003cbc0c: b        #0x3cbcfc
003cbc10: b        #0x3cbcfc
003cbc14: b        #0x3cbcfc
003cbc18: b        #0x3cbc5c
003cbc1c: b        #0x3cbc8c
003cbc20: b        #0x3cbc98
003cbc24: b        #0x3cbca4
003cbc28: b        #0x3cbcac
003cbc2c: b        #0x3cbcbc
003cbc30: b        #0x3cbcfc
003cbc34: b        #0x3cbcfc
003cbc38: b        #0x3cbcfc
003cbc3c: b        #0x3cbcfc
003cbc40: b        #0x3cbcfc
003cbc44: b        #0x3cbcfc
003cbc48: b        #0x3cbcfc
003cbc4c: b        #0x3cbcfc
003cbc50: b        #0x3cbcfc
003cbc54: b        #0x3cbcf0
003cbc58: movw     r4, #0xc351
003cbc5c: ldr      r0, [r5, #4]
003cbc60: add      r0, r0, #0x4f0
003cbc64: add      r0, r0, #0xc
003cbc68: mov      r1, r4
003cbc6c: mov      r2, r6
003cbc70: pop      {r4, r5, r6, r7, r8, lr}
003cbc74: b        #0x3c5684
003cbc78: ldr      r3, [r0]
003cbc7c: mov      r1, r2
003cbc80: mov      lr, pc
003cbc84: ldr      pc, [r3, #0x80]
003cbc88: b        #0x3cbc5c
003cbc8c: mov      r3, #0
003cbc90: strb     r3, [r0, #0x18]
003cbc94: pop      {r4, r5, r6, r7, r8, pc}
003cbc98: mov      r3, #1
003cbc9c: strb     r3, [r0, #0x4a]
003cbca0: pop      {r4, r5, r6, r7, r8, pc}
003cbca4: bl       #0x3cb77c
003cbca8: pop      {r4, r5, r6, r7, r8, pc}
003cbcac: ldr      r0, [r0, #4]
003cbcb0: add      r0, r0, #0x560
003cbcb4: bl       #0x3df3f0
003cbcb8: pop      {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr      r3, [r0]
003cbcc0: cmp      r2, #0
003cbcc4: mvneq    r1, #0
003cbcc8: ldr      r4, [r3, #0x90]
003cbccc: beq      #0x3cbce4
003cbcd0: mov      r0, r2
003cbcd4: ldr      r3, [r2]
003cbcd8: mov      lr, pc
003cbcdc: ldr      pc, [r3]
003cbce0: mov      r1, r0
003cbce4: mov      r0, r5
003cbce8: blx      r4
003cbcec: pop      {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr      r0, [r0, #4]
003cbcf4: bl       #0x394a3c
003cbcf8: b        #0x3cbc5c
003cbcfc: ldr      r0, [r0, #4]
003cbd00: ldr      r1, [r0, #0x378]
003cbd04: ldrb     r2, [r1, #9]
003cbd08: cmp      r2, #0
003cbd0c: bne      #0x3cbd30
003cbd10: ldr      r2, [pc, #0x4fc]
003cbd14: ldr      r3, [r3, r2]
003cbd18: ldrb     r3, [r3]
003cbd1c: cmp      r3, #0
003cbd20: bne      #0x3cbc5c
003cbd24: ldrb     r3, [r1, #8]
003cbd28: cmp      r3, #0
003cbd2c: bne      #0x3cbc5c
003cbd30: sub      r3, r4, #4
003cbd34: cmp      r3, #0x3a
003cbd38: addls    pc, pc, r3, lsl #2
003cbd3c: b        #0x3cbc60
003cbd40: b        #0x3cc1f8
003cbd44: b        #0x3cbc60
003cbd48: b        #0x3cbc60
003cbd4c: b        #0x3cc1e0
003cbd50: b        #0x3cc1c8
003cbd54: b        #0x3cc1ac
003cbd58: b        #0x3cc198
003cbd5c: b        #0x3cc184
003cbd60: b        #0x3cc170
003cbd64: b        #0x3cc15c
003cbd68: b        #0x3cc148
003cbd6c: b        #0x3cc134
003cbd70: b        #0x3cc120
003cbd74: b        #0x3cc10c
003cbd78: b        #0x3cc0f8
003cbd7c: b        #0x3cc0e4
003cbd80: b        #0x3cc0d0
003cbd84: b        #0x3cc0bc
003cbd88: b        #0x3cc0a8
003cbd8c: b        #0x3cc094
003cbd90: b        #0x3cc080
003cbd94: b        #0x3cc06c
003cbd98: b        #0x3cbc60
003cbd9c: b        #0x3cbc60
003cbda0: b        #0x3cbc60
003cbda4: b        #0x3cc044
003cbda8: b        #0x3cc038
003cbdac: b        #0x3cc02c
003cbdb0: b        #0x3cc020
003cbdb4: b        #0x3cc014
003cbdb8: b        #0x3cbc60
003cbdbc: b        #0x3cbc60
003cbdc0: b        #0x3cc004
003cbdc4: b        #0x3cbff4
003cbdc8: b        #0x3cbfe4
003cbdcc: b        #0x3cbfd4
003cbdd0: b        #0x3cbc60
003cbdd4: b        #0x3cbc60
003cbdd8: b        #0x3cbfbc
003cbddc: b        #0x3cbfa4
003cbde0: b        #0x3cbf8c
003cbde4: b        #0x3cbc60
003cbde8: b        #0x3cbc60
003cbdec: b        #0x3cbc60
003cbdf0: b        #0x3cbc60
003cbdf4: b        #0x3cbc60
003cbdf8: b        #0x3cbc60
003cbdfc: b        #0x3cbc60
003cbe00: b        #0x3cbc60
003cbe04: b        #0x3cbc60
003cbe08: b        #0x3cbc60
003cbe0c: b        #0x3cbf70
003cbe10: b        #0x3cbf54
003cbe14: b        #0x3cbf38
003cbe18: b        #0x3cbf1c
003cbe1c: b        #0x3cbf00
003cbe20: b        #0x3cbee4
003cbe24: b        #0x3cbec8
003cbe28: b        #0x3cbeac
003cbe2c: mov      r1, r2
003cbe30: ldr      r3, [r5]
003cbe34: mov      lr, pc
003cbe38: ldr      pc, [r3, #0x28]
003cbe3c: pop      {r4, r5, r6, r7, r8, pc}
003cbe40: bl       #0x3d3aec
003cbe44: ldr      r3, [r5]
003cbe48: mov      r7, r0
003cbe4c: mov      r0, r5
003cbe50: mov      lr, pc
003cbe54: ldr      pc, [r3, #0x98]
003cbe58: cmp      r7, #0
003cbe5c: bne      #0x3cbc5c
003cbe60: pop      {r4, r5, r6, r7, r8, pc}
003cbe64: bl       #0x3d3ae4
003cbe68: ldr      r3, [r5]
003cbe6c: mov      r7, r0
003cbe70: mov      r0, r5
003cbe74: mov      lr, pc
003cbe78: ldr      pc, [r3, #0x98]
003cbe7c: b        #0x3cbe58
003cbe80: mov      r1, r2
003cbe84: bl       #0x3d4434
003cbe88: mov      r7, r0
003cbe8c: b        #0x3cbe58
003cbe90: movw     r4, #0xc352
003cbe94: b        #0x3cbc5c
003cbe98: ldr      r3, [r0]
003cbe9c: mov      r1, r2
003cbea0: mov      lr, pc
003cbea4: ldr      pc, [r3, #0x24]
003cbea8: b        #0x3cbc5c
003cbeac: mov      r0, r5
003cbeb0: mov      r1, r6
003cbeb4: ldr      r3, [r5]
003cbeb8: mov      r2, #0
003cbebc: mov      lr, pc
003cbec0: ldr      pc, [r3, #0xc8]
003cbec4: pop      {r4, r5, r6, r7, r8, pc}
003cbec8: mov      r0, r5
003cbecc: mov      r1, r6
003cbed0: ldr      r3, [r5]
003cbed4: mov      r2, #1
003cbed8: mov      lr, pc
003cbedc: ldr      pc, [r3, #0xc8]
003cbee0: pop      {r4, r5, r6, r7, r8, pc}
003cbee4: mov      r0, r5
003cbee8: mov      r1, r6
003cbeec: ldr      r3, [r5]
003cbef0: mov      r2, #0
003cbef4: mov      lr, pc
003cbef8: ldr      pc, [r3, #0xc4]
003cbefc: pop      {r4, r5, r6, r7, r8, pc}
003cbf00: mov      r0, r5
003cbf04: mov      r1, r6
003cbf08: ldr      r3, [r5]
003cbf0c: mov      r2, #1
003cbf10: mov      lr, pc
003cbf14: ldr      pc, [r3, #0xc4]
003cbf18: pop      {r4, r5, r6, r7, r8, pc}
003cbf1c: mov      r0, r5
003cbf20: mov      r1, r6
003cbf24: ldr      r3, [r5]
003cbf28: mov      r2, #0
003cbf2c: mov      lr, pc
003cbf30: ldr      pc, [r3, #0xc0]
003cbf34: pop      {r4, r5, r6, r7, r8, pc}
003cbf38: mov      r0, r5
003cbf3c: mov      r1, r6
003cbf40: ldr      r3, [r5]
003cbf44: mov      r2, #1
003cbf48: mov      lr, pc
003cbf4c: ldr      pc, [r3, #0xc0]
003cbf50: pop      {r4, r5, r6, r7, r8, pc}
003cbf54: mov      r0, r5
003cbf58: mov      r1, r6
003cbf5c: ldr      r3, [r5]
003cbf60: mov      r2, #0
003cbf64: mov      lr, pc
003cbf68: ldr      pc, [r3, #0xbc]
003cbf6c: pop      {r4, r5, r6, r7, r8, pc}
003cbf70: mov      r0, r5
003cbf74: mov      r1, r6
003cbf78: ldr      r3, [r5]
003cbf7c: mov      r2, #1
003cbf80: mov      lr, pc
003cbf84: ldr      pc, [r3, #0xbc]
003cbf88: pop      {r4, r5, r6, r7, r8, pc}
003cbf8c: mov      r0, r5
003cbf90: ldr      r3, [r5]
003cbf94: mov      lr, pc
003cbf98: ldr      pc, [r3, #0x88]
003cbf9c: ldr      r0, [r5, #4]
003cbfa0: b        #0x3cbc60
003cbfa4: mov      r0, r5
003cbfa8: ldr      r3, [r5]
003cbfac: mov      lr, pc
003cbfb0: ldr      pc, [r3, #0x84]
003cbfb4: ldr      r0, [r5, #4]
003cbfb8: b        #0x3cbc60
003cbfbc: mov      r0, r5
003cbfc0: ldr      r3, [r5]
003cbfc4: mov      lr, pc
003cbfc8: ldr      pc, [r3, #0x8c]
003cbfcc: ldr      r0, [r5, #4]
003cbfd0: b        #0x3cbc60
003cbfd4: mov      r0, r5
003cbfd8: bl       #0x3d3ff8
003cbfdc: mov      r7, r0
003cbfe0: b        #0x3cbe58
003cbfe4: mov      r0, r5
003cbfe8: bl       #0x3d4204
003cbfec: mov      r7, r0
003cbff0: b        #0x3cbe58
003cbff4: mov      r0, r5
003cbff8: bl       #0x3d3d30
003cbffc: mov      r7, r0
003cc000: b        #0x3cbe58
003cc004: mov      r0, r5
003cc008: bl       #0x3d3d4c
003cc00c: mov      r7, r0
003cc010: b        #0x3cbe58
003cc014: mov      r0, r5
003cc018: pop      {r4, r5, r6, r7, r8, lr}
003cc01c: b        #0x3d8b28
003cc020: mov      r0, r5
003cc024: pop      {r4, r5, r6, r7, r8, lr}
003cc028: b        #0x3d8038
003cc02c: mov      r0, r5
003cc030: pop      {r4, r5, r6, r7, r8, lr}
003cc034: b        #0x3d8b7c
003cc038: mov      r0, r5
003cc03c: pop      {r4, r5, r6, r7, r8, lr}
003cc040: b        #0x3d808c
003cc044: ldr      r3, [r5]
003cc048: add      r0, r0, #0x4f0
003cc04c: add      r0, r0, #0xc
003cc050: ldr      r4, [r3, #0x20]
003cc054: bl       #0x3c01ac
003cc058: mov      r1, r6
003cc05c: mov      r2, r0
003cc060: mov      r0, r5
003cc064: blx      r4
003cc068: pop      {r4, r5, r6, r7, r8, pc}
003cc06c: mov      r0, r5
003cc070: ldr      r3, [r5]
003cc074: mov      lr, pc
003cc078: ldr      pc, [r3, #0x7c]
003cc07c: pop      {r4, r5, r6, r7, r8, pc}
003cc080: mov      r0, r5
003cc084: ldr      r3, [r5]
003cc088: mov      lr, pc
003cc08c: ldr      pc, [r3, #0x78]
003cc090: pop      {r4, r5, r6, r7, r8, pc}
003cc094: mov      r0, r5
003cc098: ldr      r3, [r5]
003cc09c: mov      lr, pc
003cc0a0: ldr      pc, [r3, #0x74]
003cc0a4: pop      {r4, r5, r6, r7, r8, pc}
003cc0a8: mov      r0, r5
003cc0ac: ldr      r3, [r5]
003cc0b0: mov      lr, pc
003cc0b4: ldr      pc, [r3, #0x70]
003cc0b8: pop      {r4, r5, r6, r7, r8, pc}
003cc0bc: mov      r0, r5
003cc0c0: ldr      r3, [r5]
003cc0c4: mov      lr, pc
003cc0c8: ldr      pc, [r3, #0x6c]
003cc0cc: pop      {r4, r5, r6, r7, r8, pc}
003cc0d0: mov      r0, r5
003cc0d4: ldr      r3, [r5]
003cc0d8: mov      lr, pc
003cc0dc: ldr      pc, [r3, #0x68]
003cc0e0: pop      {r4, r5, r6, r7, r8, pc}
003cc0e4: mov      r0, r5
003cc0e8: ldr      r3, [r5]
003cc0ec: mov      lr, pc
003cc0f0: ldr      pc, [r3, #0x64]
003cc0f4: pop      {r4, r5, r6, r7, r8, pc}
003cc0f8: mov      r0, r5
003cc0fc: ldr      r3, [r5]
003cc100: mov      lr, pc
003cc104: ldr      pc, [r3, #0x60]
003cc108: pop      {r4, r5, r6, r7, r8, pc}
003cc10c: mov      r0, r5
003cc110: ldr      r3, [r5]
003cc114: mov      lr, pc
003cc118: ldr      pc, [r3, #0x5c]
003cc11c: pop      {r4, r5, r6, r7, r8, pc}
003cc120: mov      r0, r5
003cc124: ldr      r3, [r5]
003cc128: mov      lr, pc
003cc12c: ldr      pc, [r3, #0x58]
003cc130: pop      {r4, r5, r6, r7, r8, pc}
003cc134: mov      r0, r5
003cc138: ldr      r3, [r5]
003cc13c: mov      lr, pc
003cc140: ldr      pc, [r3, #0x54]
003cc144: pop      {r4, r5, r6, r7, r8, pc}
003cc148: mov      r0, r5
003cc14c: ldr      r3, [r5]
003cc150: mov      lr, pc
003cc154: ldr      pc, [r3, #0x50]
003cc158: pop      {r4, r5, r6, r7, r8, pc}
003cc15c: mov      r0, r5
003cc160: ldr      r3, [r5]
003cc164: mov      lr, pc
003cc168: ldr      pc, [r3, #0x4c]
003cc16c: pop      {r4, r5, r6, r7, r8, pc}
003cc170: mov      r0, r5
003cc174: ldr      r3, [r5]
003cc178: mov      lr, pc
003cc17c: ldr      pc, [r3, #0x48]
003cc180: pop      {r4, r5, r6, r7, r8, pc}
003cc184: mov      r0, r5
003cc188: ldr      r3, [r5]
003cc18c: mov      lr, pc
003cc190: ldr      pc, [r3, #0x44]
003cc194: pop      {r4, r5, r6, r7, r8, pc}
003cc198: mov      r0, r5
003cc19c: ldr      r3, [r5]
003cc1a0: mov      lr, pc
003cc1a4: ldr      pc, [r3, #0x40]
003cc1a8: pop      {r4, r5, r6, r7, r8, pc}
003cc1ac: mov      r0, r5
003cc1b0: ldr      r3, [r5]
003cc1b4: mov      r1, r6
003cc1b8: mov      lr, pc
003cc1bc: ldr      pc, [r3, #0x34]
003cc1c0: ldr      r0, [r5, #4]
003cc1c4: b        #0x3cbc60
003cc1c8: mov      r0, r5
003cc1cc: mov      r1, r6
003cc1d0: ldr      r3, [r5]
003cc1d4: mov      lr, pc
003cc1d8: ldr      pc, [r3, #0x30]
003cc1dc: pop      {r4, r5, r6, r7, r8, pc}
003cc1e0: mov      r0, r5
003cc1e4: mov      r1, r6
003cc1e8: ldr      r3, [r5]
003cc1ec: mov      lr, pc
003cc1f0: ldr      pc, [r3, #0x2c]
003cc1f4: pop      {r4, r5, r6, r7, r8, pc}
003cc1f8: mov      r0, r5
003cc1fc: mov      r1, r6
003cc200: ldr      r3, [r5]
003cc204: mov      lr, pc
003cc208: ldr      pc, [r3, #0xb0]
003cc20c: pop      {r4, r5, r6, r7, r8, pc}
003cc210: subseq   r8, ip, r4, asr pc
003cc214: andeq    r3, r0, r0, asr r6

# _ZN6CharAI21AI_PauseTargetSeekingEj
003d53ec: str      lr, [sp, #-4]!
003d53f0: ldr      r3, [r0, #4]
003d53f4: mov      ip, #0
003d53f8: strb     ip, [r0, #0x4a]
003d53fc: sub      sp, sp, #0xc
003d5400: add      r0, r3, #0x3b4
003d5404: mov      r2, ip
003d5408: mov      r3, #0x32
003d540c: str      ip, [sp]
003d5410: bl       #0x3dbe24
003d5414: add      sp, sp, #0xc
003d5418: ldm      sp!, {pc}

# _ZN6CharAI11AI_EndSkillEj
003d8474: push     {r4, r5, r6, lr}
003d8478: mov      r4, r0
003d847c: ldr      r0, [r0, #4]
003d8480: mov      r5, r1
003d8484: add      r0, r0, #0x4f0
003d8488: add      r0, r0, #0xc
003d848c: bl       #0x3c02e8
003d8490: cmp      r0, #0
003d8494: bne      #0x3d849c
003d8498: pop      {r4, r5, r6, pc}
003d849c: mov      r1, r5
003d84a0: ldr      r0, [r4, #4]
003d84a4: bl       #0x3bc784
003d84a8: ldr      r3, [r0, #0x48]
003d84ac: cmp      r3, #2
003d84b0: bne      #0x3d8498
003d84b4: ldrb     r3, [r4, #0xd0]
003d84b8: cmp      r3, #0
003d84bc: moveq    r3, #1
003d84c0: strbeq   r3, [r4, #0xd1]
003d84c4: beq      #0x3d8498
003d84c8: ldr      r0, [r4, #4]
003d84cc: mov      r1, #1
003d84d0: add      r0, r0, #0x490
003d84d4: add      r0, r0, #0xc
003d84d8: pop      {r4, r5, r6, lr}
003d84dc: b        #0x3c948c

# _ZN9CharacterC1EN10ObjectBase6GO_IDSE
003aa1b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8: add      ip, r0, #0x374
003aa1bc: sub      sp, sp, #0x3c
003aa1c0: mov      r4, r0
003aa1c4: str      ip, [sp, #0xc]
003aa1c8: bl       #0x38c398
003aa1cc: ldr      ip, [sp, #0xc]
003aa1d0: add      r5, r4, #0x4f0
003aa1d4: add      r5, r5, #0xc
003aa1d8: mov      r0, ip
003aa1dc: bl       #0x404db8
003aa1e0: add      r0, r4, #0x3b4
003aa1e4: str      r0, [sp, #0x20]
003aa1e8: add      r0, r4, #0x37c
003aa1ec: bl       #0x3ff330
003aa1f0: add      r2, r4, #0x490
003aa1f4: add      r1, r4, #0x3c8
003aa1f8: add      r2, r2, #0xc
003aa1fc: ldr      r0, [sp, #0x20]
003aa200: str      r1, [sp, #0x1c]
003aa204: str      r2, [sp, #0x14]
003aa208: bl       #0x3dbb0c
003aa20c: ldr      r0, [sp, #0x1c]
003aa210: bl       #0x3cebf0
003aa214: ldr      r0, [sp, #0x14]
003aa218: bl       #0x3c8ff4
003aa21c: add      r3, r4, #0x560
003aa220: mov      r0, r5
003aa224: str      r3, [sp, #0x18]
003aa228: ldr      sb, [pc, #0x50c]
003aa22c: bl       #0x3c1b58
003aa230: ldr      r0, [sp, #0x18]
003aa234: bl       #0x3df084
003aa238: ldr      lr, [pc, #0x500]
003aa23c: add      sb, pc, sb
003aa240: mov      r8, #0
003aa244: ldr      lr, [sb, lr]
003aa248: mov      fp, #1
003aa24c: mvn      r6, #0
003aa250: add      sl, lr, #0x324
003aa254: str      sl, [sp, #0x34]
003aa258: add      sl, lr, #0x180
003aa25c: str      sl, [sp, #0x10]
003aa260: add      sl, lr, #0x1f4
003aa264: str      sl, [sp, #0x24]
003aa268: add      sl, lr, #0x220
003aa26c: str      sl, [sp, #0x28]
003aa270: add      sl, lr, #0x230
003aa274: str      sl, [sp, #0x2c]
003aa278: add      r0, lr, #8
003aa27c: add      r1, lr, #0x15c
003aa280: add      r2, lr, #0x168
003aa284: add      sl, lr, #0x304
003aa288: str      sl, [sp, #0x30]
003aa28c: stm      r4, {r0, r1}
003aa290: str      r2, [r4, #0x24]
003aa294: ldr      r0, [sp, #0x10]
003aa298: add      lr, lr, #0x314
003aa29c: add      r7, r4, #0x1380
003aa2a0: str      r0, [r4, #0x374]
003aa2a4: ldr      r1, [sp, #0x24]
003aa2a8: add      r3, r7, #0x18
003aa2ac: movw     sl, #0x13a8
003aa2b0: str      r1, [r4, #0x37c]
003aa2b4: ldr      r2, [sp, #0x28]
003aa2b8: add      r7, r7, #0x30
003aa2bc: str      r2, [r4, #0x3b4]
003aa2c0: ldr      r0, [sp, #0x2c]
003aa2c4: str      r0, [r4, #0x3c8]
003aa2c8: ldr      r1, [sp, #0x30]
003aa2cc: str      lr, [r4, #0x4fc]
003aa2d0: mov      r0, r3
003aa2d4: str      r1, [r4, #0x49c]
003aa2d8: ldr      r2, [sp, #0x34]
003aa2dc: mov      r1, #0x10
003aa2e0: str      r2, [r4, #0x560]
003aa2e4: movw     r2, #0x1394
003aa2e8: strb     r8, [r4, r2]
003aa2ec: movw     r2, #0x1395
003aa2f0: strb     r8, [r4, r2]
003aa2f4: movw     r2, #0x1396
003aa2f8: strb     fp, [r4, r2]
003aa2fc: movw     r2, #0x1397
003aa300: strb     r6, [r4, r2]
003aa304: movw     r2, #0x13ac
003aa308: str      r3, [r4, r2]
003aa30c: str      r3, [r4, sl]
003aa310: bl       #0x31167c
003aa314: ldr      r3, [r4, sl]
003aa318: mov      sl, #0x13c0
003aa31c: mov      r0, r7
003aa320: strb     r8, [r3]
003aa324: movw     r3, #0x13c4
003aa328: str      r7, [r4, r3]
003aa32c: mov      r1, #0x10
003aa330: str      r7, [r4, sl]
003aa334: bl       #0x31167c
003aa338: ldr      r2, [r4, sl]
003aa33c: add      r7, r4, sl
003aa340: add      r3, r7, #0xc
003aa344: strb     r8, [r2]
003aa348: movw     r2, #0x13c8
003aa34c: strh     r6, [r4, r2]
003aa350: movw     r2, #0x13ca
003aa354: strh     r6, [r4, r2]
003aa358: movw     sl, #0x13dc
003aa35c: movw     r2, #0x13e0
003aa360: str      r3, [r4, r2]
003aa364: mov      r0, r3
003aa368: str      r3, [r4, sl]
003aa36c: mov      r1, #0x10
003aa370: bl       #0x31167c
003aa374: ldr      r3, [r4, sl]
003aa378: add      r7, r7, #0x28
003aa37c: movw     sl, #0x13f8
003aa380: strb     r8, [r3]
003aa384: movw     r3, #0x13e4
003aa388: strb     fp, [r4, r3]
003aa38c: movw     r3, #0x13fc
003aa390: str      r7, [r4, r3]
003aa394: mov      r0, r7
003aa398: str      r7, [r4, sl]
003aa39c: mov      r1, #0x10
003aa3a0: bl       #0x31167c
003aa3a4: ldr      r3, [r4, sl]
003aa3a8: add      r7, r4, #0x1400
003aa3ac: movw     sl, #0x1410
003aa3b0: strb     r8, [r3]
003aa3b4: movw     r3, #0x1414
003aa3b8: str      r7, [r4, r3]
003aa3bc: mov      r0, r7
003aa3c0: str      r7, [r4, sl]
003aa3c4: mov      r1, #0x10
003aa3c8: bl       #0x31167c
003aa3cc: ldr      r3, [r4, sl]
003aa3d0: add      r7, r7, #0x18
003aa3d4: movw     sl, #0x1428
003aa3d8: strb     r8, [r3]
003aa3dc: movw     r3, #0x142c
003aa3e0: str      r7, [r4, r3]
003aa3e4: mov      r0, r7
003aa3e8: str      r7, [r4, sl]
003aa3ec: mov      r1, #0x10
003aa3f0: bl       #0x31167c
003aa3f4: ldr      r2, [r4, sl]
003aa3f8: mov      r3, #0
003aa3fc: mov      r1, #0xbf000000
003aa400: strb     r8, [r2]
003aa404: movw     r2, #0x14a8
003aa408: strb     r6, [r4, r2]
003aa40c: movw     r2, #0x1430
003aa410: strb     fp, [r4, r2]
003aa414: movw     r2, #0x1434
003aa418: str      r8, [r4, r2]
003aa41c: movw     r2, #0x1438
003aa420: str      r8, [r4, r2]
003aa424: movw     r2, #0x1448
003aa428: strb     fp, [r4, r2]
003aa42c: movw     r2, #0x1449
003aa430: strb     r8, [r4, r2]
003aa434: movw     r2, #0x144c
003aa438: str      r8, [r4, r2]
003aa43c: movw     r2, #0x1450
003aa440: str      r3, [r4, r2]
003aa444: movw     r2, #0x1454
003aa448: str      r3, [r4, r2]
003aa44c: movw     r2, #0x1458
003aa450: str      r3, [r4, r2]
003aa454: movw     r2, #0x145c
003aa458: str      r3, [r4, r2]
003aa45c: movw     r2, #0x1460
003aa460: str      r3, [r4, r2]
003aa464: movw     r2, #0x1464
003aa468: str      r3, [r4, r2]
003aa46c: movw     r2, #0x1468
003aa470: str      r3, [r4, r2]
003aa474: movw     r2, #0x146c
003aa478: str      r3, [r4, r2]
003aa47c: movw     r2, #0x1470
003aa480: str      r3, [r4, r2]
003aa484: movw     r2, #0x1474
003aa488: str      r3, [r4, r2]
003aa48c: movw     r2, #0x1478
003aa490: str      r3, [r4, r2]
003aa494: movw     r2, #0x147c
003aa498: str      r3, [r4, r2]
003aa49c: mov      r2, #0x1480
003aa4a0: strb     r8, [r4, r2]
003aa4a4: movw     r2, #0x1481
003aa4a8: strb     r8, [r4, r2]
003aa4ac: movw     r2, #0x1484
003aa4b0: str      r8, [r4, r2]
003aa4b4: movw     r2, #0x1488
003aa4b8: str      r8, [r4, r2]
003aa4bc: movw     r2, #0x148c
003aa4c0: str      r8, [r4, r2]
003aa4c4: movw     r2, #0x1490
003aa4c8: str      r8, [r4, r2]
003aa4cc: movw     r2, #0x1494
003aa4d0: str      r8, [r4, r2]
003aa4d4: movw     r2, #0x1498
003aa4d8: str      r6, [r4, r2]
003aa4dc: movw     r2, #0x149c
003aa4e0: str      r8, [r4, r2]
003aa4e4: movw     r2, #0x14a0
003aa4e8: str      r8, [r4, r2]
003aa4ec: movw     r2, #0x14a4
003aa4f0: str      r8, [r4, r2]
003aa4f4: movw     r2, #0x14aa
003aa4f8: strh     r8, [r4, r2]
003aa4fc: movw     r2, #0x14ac
003aa500: strb     r8, [r4, r2]
003aa504: movw     r2, #0x14d8
003aa508: str      r3, [r4, r2]
003aa50c: add      r1, r1, #0x800000
003aa510: movw     r2, #0x14fc
003aa514: str      r1, [r4, r2]
003aa518: movw     r2, #0x1504
003aa51c: str      r6, [r4, r2]
003aa520: movw     r2, #0x14ad
003aa524: strb     r8, [r4, r2]
003aa528: movw     r2, #0x14b0
003aa52c: str      r3, [r4, r2]
003aa530: movw     r2, #0x14b4
003aa534: str      r3, [r4, r2]
003aa538: movw     r2, #0x14b8
003aa53c: str      r3, [r4, r2]
003aa540: movw     r2, #0x14bc
003aa544: str      r3, [r4, r2]
003aa548: mov      r2, #0x14c0
003aa54c: str      r3, [r4, r2]
003aa550: movw     r2, #0x14c4
003aa554: str      r3, [r4, r2]
003aa558: movw     r3, #0x14c8
003aa55c: strb     r8, [r4, r3]
003aa560: movw     r3, #0x14ca
003aa564: strh     r6, [r4, r3]
003aa568: movw     r3, #0x14cc
003aa56c: str      r8, [r4, r3]
003aa570: movw     r3, #0x14d0
003aa574: strh     r8, [r4, r3]
003aa578: movw     r3, #0x14d4
003aa57c: str      r8, [r4, r3]
003aa580: movw     r3, #0x14dc
003aa584: strb     r8, [r4, r3]
003aa588: movw     r3, #0x14e4
003aa58c: strb     r8, [r4, r3]
003aa590: movw     r3, #0x14e5
003aa594: strb     r8, [r4, r3]
003aa598: movw     r3, #0x14e8
003aa59c: str      r8, [r4, r3]
003aa5a0: movw     r3, #0x14ec
003aa5a4: str      r8, [r4, r3]
003aa5a8: add      r7, r4, #0x1500
003aa5ac: movw     r3, #0x14f0
003aa5b0: add      r0, r4, #0x1a40
003aa5b4: strb     r8, [r4, r3]
003aa5b8: add      r0, r0, #8
003aa5bc: mov      r3, #0x1500
003aa5c0: add      r7, r7, #8
003aa5c4: str      r6, [r4, r3]
003aa5c8: str      r0, [sp, #0x10]
003aa5cc: mov      r0, r7
003aa5d0: bl       #0x3a6a24
003aa5d4: ldr      r0, [sp, #0x10]
003aa5d8: bl       #0x3a6a24
003aa5dc: add      r0, r4, #0x304
003aa5e0: mov      r1, r4
003aa5e4: strb     fp, [r4, #0x28]
003aa5e8: bl       #0x4a191c
003aa5ec: strb     fp, [r4, #0x1c4]
003aa5f0: strb     fp, [r4, #0x85]
003aa5f4: mov      r0, #0x10
003aa5f8: mov      r1, r8
003aa5fc: bl       #0x310570
003aa600: ldr      r3, [pc, #0x13c]
003aa604: ldr      ip, [sp, #0xc]
003aa608: mov      r6, r0
003aa60c: ldr      r3, [sb, r3]
003aa610: cmp      ip, r8
003aa614: strb     r8, [r6, #0xa]
003aa618: add      r3, r3, #8
003aa61c: str      r8, [r0, #0xc]
003aa620: stm      r0, {r3, ip}
003aa624: strb     r8, [r6, #8]
003aa628: strb     r8, [r6, #9]
003aa62c: beq      #0x3aa6e0
003aa630: mov      r0, ip
003aa634: mov      r1, r6
003aa638: bl       #0x404e10
003aa63c: ldr      r3, [r4, #0x378]
003aa640: ldr      r0, [sp, #0x20]
003aa644: mov      r1, r4
003aa648: str      r4, [r3, #0xc]
003aa64c: bl       #0x3db480
003aa650: ldr      r0, [sp, #0x1c]
003aa654: mov      r1, r4
003aa658: bl       #0x3cb7c0
003aa65c: ldr      r0, [sp, #0x14]
003aa660: mov      r1, r4
003aa664: bl       #0x3c9890
003aa668: mov      r0, r5
003aa66c: mov      r1, r4
003aa670: bl       #0x3c1600
003aa674: ldr      r0, [sp, #0x18]
003aa678: mov      r1, r4
003aa67c: bl       #0x3dec0c
003aa680: mov      r6, #0
003aa684: str      r4, [r4, #0x380]
003aa688: mov      r1, r6
003aa68c: mov      r0, r5
003aa690: add      r6, r6, #1
003aa694: bl       #0x3c7318
003aa698: cmp      r6, #0x14
003aa69c: bne      #0x3aa688
003aa6a0: mov      r1, #0
003aa6a4: movw     r2, #0x14e0
003aa6a8: str      r1, [r4, r2]
003aa6ac: mvn      r3, #0
003aa6b0: movw     r2, #0x14f4
003aa6b4: str      r3, [r4, r2]
003aa6b8: str      r7, [r4, #0x100]
003aa6bc: ldr      sl, [sp, #0x10]
003aa6c0: movw     r2, #0x14f8
003aa6c4: mov      r0, r4
003aa6c8: str      sl, [r4, #0x104]
003aa6cc: str      r3, [r4, r2]
003aa6d0: mov      r3, #1
003aa6d4: strb     r3, [r4, #0xf8]
003aa6d8: add      sp, sp, #0x3c
003aa6dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0: ldr      r3, [pc, #0x60]
003aa6e4: ldr      r3, [sb, r3]
003aa6e8: ldr      r3, [r3]
003aa6ec: cmp      r3, #2
003aa6f0: streq    ip, [r4, #0x374]
003aa6f4: beq      #0x3aa630
003aa6f8: cmp      r3, #1
003aa6fc: bne      #0x3aa630
003aa700: ldr      r0, [pc, #0x44]
003aa704: ldr      r1, [pc, #0x44]
003aa708: ldr      r2, [pc, #0x44]
003aa70c: ldr      r0, [sb, r0]
003aa710: ldr      r3, [pc, #0x40]
003aa714: mov      lr, #0x44
003aa718: add      r1, pc, r1
003aa71c: add      r0, r0, #0xa8
003aa720: add      r2, pc, r2
003aa724: add      r3, pc, r3
003aa728: str      ip, [sp, #0xc]
003aa72c: str      lr, [sp]
003aa730: bl       #0x30e004
003aa734: ldr      ip, [sp, #0xc]
003aa738: b        #0x3aa630
003aa73c: subseq   sl, lr, r4, asr r8
003aa740: andeq    r2, r0, r8, lsl #28
003aa744: andeq    r2, r0, r4, lsr #21
003aa748: andeq    r3, r0, r0, asr #19
003aa74c: andeq    r1, r0, r0, asr #19
003aa750: subseq   r3, r1, r0, asr #25
003aa754: subseq   r8, r1, r0, lsr #27
003aa758: subseq   r8, r1, ip, lsr #27

# _ZN16Script_LookActor7ExecuteEbi
0045ec50: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045ec54: ldr      r4, [pc, #0x24c]
0045ec58: ldr      fp, [pc, #0x24c]
0045ec5c: ldr      r1, [pc, #0x24c]
0045ec60: add      r4, pc, r4
0045ec64: ldr      r3, [r4, fp]
0045ec68: ldr      r7, [r4, r1]
0045ec6c: sub      sp, sp, #0x54
0045ec70: ldr      r3, [r3]
0045ec74: mov      sb, r2
0045ec78: add      r5, sp, #0x34
0045ec7c: str      r3, [sp, #0x4c]
0045ec80: ldr      r6, [r0, #0xc]
0045ec84: mov      r0, r7
0045ec88: bl       #0x337888
0045ec8c: ldr      r1, [pc, #0x220]
0045ec90: add      r2, sp, #0x30
0045ec94: mov      r0, r5
0045ec98: add      r1, pc, r1
0045ec9c: ldr      sl, [pc, #0x214]
0045eca0: bl       #0x3140ec
0045eca4: mov      r1, r5
0045eca8: mov      r0, r7
0045ecac: bl       #0x337a88
0045ecb0: mov      r0, r5
0045ecb4: bl       #0x318254
0045ecb8: ldr      r3, [r4, sl]
0045ecbc: add      r8, sp, #0x24
0045ecc0: ldr      r2, [r6, #0x14]
0045ecc4: ldr      r1, [r3, #0x38]
0045ecc8: mov      r5, #0
0045eccc: mov      r3, sb
0045ecd0: mov      r0, r8
0045ecd4: str      r5, [sp]
0045ecd8: str      r5, [sp, #4]
0045ecdc: bl       #0x34aca0
0045ece0: mov      r0, r8
0045ece4: mov      r1, r5
0045ece8: bl       #0x33fdc0
0045ecec: subs     r7, r0, #0
0045ecf0: bne      #0x45ee2c
0045ecf4: add      r5, sp, #0x18
0045ecf8: mov      r1, #0
0045ecfc: mov      r0, r5
0045ed00: bl       #0x33f524
0045ed04: ldr      r8, [r6, #0xc]
0045ed08: ldr      r1, [pc, #0x1ac]
0045ed0c: mov      r0, r8
0045ed10: add      r1, pc, r1
0045ed14: bl       #0x30e31c
0045ed18: cmp      r0, #0
0045ed1c: bne      #0x45edf4
0045ed20: str      r0, [sp]
0045ed24: ldr      r3, [r4, sl]
0045ed28: ldr      ip, [r6, #0x14]
0045ed2c: add      r6, sp, #8
0045ed30: ldr      r1, [r3, #0x38]
0045ed34: mov      r2, r8
0045ed38: mov      r3, sb
0045ed3c: mov      r0, r6
0045ed40: str      ip, [sp, #4]
0045ed44: bl       #0x34aca0
0045ed48: ldr      r3, [r6, #8]
0045ed4c: mov      r0, r5
0045ed50: mov      r1, #0
0045ed54: str      r3, [r5, #8]
0045ed58: ldr      r3, [sp, #8]
0045ed5c: str      r3, [sp, #0x18]
0045ed60: ldr      r3, [sp, #0xc]
0045ed64: str      r3, [sp, #0x1c]
0045ed68: bl       #0x33fdc0
0045ed6c: subs     r6, r0, #0
0045ed70: bne      #0x45ee1c
0045ed74: cmp      r7, #0
0045ed78: beq      #0x45edcc
0045ed7c: mov      r0, r5
0045ed80: mov      r1, #0
0045ed84: bl       #0x33fdc0
0045ed88: cmp      r0, #0
0045ed8c: beq      #0x45edcc
0045ed90: ldr      r3, [r7]
0045ed94: mov      r0, r7
0045ed98: mov      lr, pc
0045ed9c: ldr      pc, [r3, #0x28]
0045eda0: cmp      r0, #0
0045eda4: bne      #0x45ee3c
0045eda8: ldr      r3, [r7, #0x378]
0045edac: mov      r2, #1
0045edb0: mov      r1, r6
0045edb4: strb     r2, [r3, #9]
0045edb8: ldr      r0, [r7, #0x378]
0045edbc: bl       #0x4052bc
0045edc0: ldr      r3, [r7, #0x378]
0045edc4: mov      r2, #0
0045edc8: strb     r2, [r3, #9]
0045edcc: mov      r0, r5
0045edd0: mov      r1, #0
0045edd4: bl       #0x33fdc0
0045edd8: ldr      r3, [r4, fp]
0045eddc: ldr      r2, [sp, #0x4c]
0045ede0: ldr      r3, [r3]
0045ede4: cmp      r2, r3
0045ede8: bne      #0x45eea4
0045edec: add      sp, sp, #0x54
0045edf0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045edf4: ldr      r3, [r4, sl]
0045edf8: mov      ip, #0
0045edfc: add      r6, sp, #8
0045ee00: ldr      r1, [r3, #0x38]
0045ee04: mov      r2, r8
0045ee08: mov      r3, sb
0045ee0c: mov      r0, r6
0045ee10: str      ip, [sp, #4]
0045ee14: str      ip, [sp]
0045ee18: b        #0x45ed44
0045ee1c: mov      r0, r5
0045ee20: bl       #0x33fee4
0045ee24: mov      r6, r0
0045ee28: b        #0x45ed74
0045ee2c: mov      r0, r8
0045ee30: bl       #0x33ff54
0045ee34: mov      r7, r0
0045ee38: b        #0x45ecf4
0045ee3c: ldr      sl, [r4, sl]
0045ee40: mov      r0, sl
0045ee44: bl       #0x31f594
0045ee48: cmp      r0, #0
0045ee4c: beq      #0x45eda8
0045ee50: mov      r8, #1
0045ee54: ldr      r0, [sl, #0x40]
0045ee58: mov      r1, r8
0045ee5c: mov      r2, #0
0045ee60: bl       #0x36e744
0045ee64: ldr      sb, [r0, #0x660]
0045ee68: cmp      sb, #0
0045ee6c: beq      #0x45ee94
0045ee70: ldr      r3, [sb, #0x378]
0045ee74: mov      r2, #1
0045ee78: mov      r1, r6
0045ee7c: strb     r2, [r3, #9]
0045ee80: ldr      r0, [sb, #0x378]
0045ee84: bl       #0x4052bc
0045ee88: ldr      r3, [sb, #0x378]
0045ee8c: mov      r2, #0
0045ee90: strb     r2, [r3, #9]
0045ee94: add      r8, r8, #1
0045ee98: cmp      r8, #4
0045ee9c: bne      #0x45ee54
0045eea0: b        #0x45eda8
0045eea4: bl       #0x30e310
0045eea8: subseq   r5, r3, r0, lsr lr
0045eeac: andeq    r4, r0, ip, lsr #1
0045eeb0: andeq    r0, r0, r4, lsl #17
0045eeb4: subeq    lr, r6, r8, ror #7
0045eeb8: strdeq   r3, r4, [r0], -r4
0045eebc: ldrdeq   r1, r2, [r6], #-0x60

# _ZN6CharAI21_ClearNonStickyTargetEv
003d8d70: push     {r4, lr}
003d8d74: ldrb     r1, [r0, #0x4b]
003d8d78: mov      r4, r0
003d8d7c: cmp      r1, #0
003d8d80: beq      #0x3d8d88
003d8d84: pop      {r4, pc}
003d8d88: mov      r2, r1
003d8d8c: bl       #0x3d6890
003d8d90: mov      r0, r4
003d8d94: pop      {r4, lr}
003d8d98: b        #0x3d49c4

# _ZN15v2HudControllerC1EP14v2Controllable
00408930: push     {r4, r5, lr}
00408934: ldr      r5, [pc, #0xa8]
00408938: ldr      r3, [pc, #0xa8]
0040893c: mov      r2, #0
00408940: add      r5, pc, r5
00408944: ldr      r3, [r5, r3]
00408948: cmp      r1, #0
0040894c: sub      sp, sp, #0xc
00408950: add      r3, r3, #8
00408954: mov      r4, r0
00408958: str      r3, [r0]
0040895c: str      r2, [r0, #0xc]
00408960: str      r1, [r0, #4]
00408964: strb     r2, [r0, #8]
00408968: strb     r2, [r0, #9]
0040896c: strb     r2, [r0, #0xa]
00408970: beq      #0x408990
00408974: ldr      r3, [pc, #0x70]
00408978: mov      r0, r4
0040897c: ldr      r3, [r5, r3]
00408980: add      r3, r3, #8
00408984: str      r3, [r4]
00408988: add      sp, sp, #0xc
0040898c: pop      {r4, r5, pc}
00408990: ldr      r3, [pc, #0x58]
00408994: ldr      r3, [r5, r3]
00408998: ldr      r3, [r3]
0040899c: cmp      r3, #2
004089a0: streq    r1, [r1]
004089a4: beq      #0x408974
004089a8: cmp      r3, #1
004089ac: bne      #0x408974
004089b0: ldr      r0, [pc, #0x3c]
004089b4: ldr      r1, [pc, #0x3c]
004089b8: ldr      r2, [pc, #0x3c]
004089bc: ldr      r0, [r5, r0]
004089c0: ldr      r3, [pc, #0x38]
004089c4: mov      ip, #0x44
004089c8: add      r1, pc, r1
004089cc: add      r2, pc, r2
004089d0: add      r3, pc, r3
004089d4: add      r0, r0, #0xa8
004089d8: str      ip, [sp]
004089dc: bl       #0x30e004
004089e0: b        #0x408974
004089e4: subseq   ip, r8, r0, asr r1
004089e8: andeq    r2, r0, r4, lsr #21
004089ec: andeq    r1, r0, r8, asr #31
004089f0: andeq    r3, r0, r0, asr #19
004089f4: andeq    r1, r0, r0, asr #19
004089f8: subeq    r5, fp, r0, lsl sl
004089fc: strdeq   sl, fp, [fp], #-0xa4
00408a00: subeq    pc, fp, r8

# _ZN9Character10Ctrl_ClickERK7Point3DIfEb
003addc8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003addcc: ldr      r6, [pc, #0x674]
003addd0: ldr      sl, [pc, #0x674]
003addd4: sub      sp, sp, #0xbc
003addd8: add      r6, pc, r6
003adddc: ldr      r3, [r6, sl]
003adde0: mov      r7, r1
003adde4: str      r2, [sp, #0x18]
003adde8: ldr      r3, [r3]
003addec: mov      r8, r0
003addf0: str      r3, [sp, #0xb4]
003addf4: bl       #0x3ad430
003addf8: cmp      r0, #0
003addfc: bne      #0x3ade1c
003ade00: ldr      r3, [r6, sl]
003ade04: ldr      r2, [sp, #0xb4]
003ade08: ldr      r3, [r3]
003ade0c: cmp      r2, r3
003ade10: bne      #0x3ae444
003ade14: add      sp, sp, #0xbc
003ade18: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ade1c: ldr      r1, [pc, #0x62c]
003ade20: add      r4, r8, #0x4f0
003ade24: add      r4, r4, #0xc
003ade28: ldr      r5, [r6, r1]
003ade2c: str      r1, [sp, #0x20]
003ade30: mov      r0, r5
003ade34: bl       #0x320e74
003ade38: str      r0, [sp, #0x14]
003ade3c: mov      r0, r4
003ade40: bl       #0x3c0334
003ade44: cmp      r0, #0
003ade48: beq      #0x3adecc
003ade4c: ldr      r2, [sp, #0x18]
003ade50: cmp      r2, #0
003ade54: beq      #0x3ade74
003ade58: movw     r3, #0x14c8
003ade5c: mov      r2, #0
003ade60: strb     r2, [r8, r3]
003ade64: mvn      r1, #0
003ade68: movw     r3, #0x14ca
003ade6c: strh     r1, [r8, r3]
003ade70: b        #0x3ade00
003ade74: mov      r2, #1
003ade78: movw     r3, #0x14c8
003ade7c: strb     r2, [r8, r3]
003ade80: ldr      r1, [r7]
003ade84: movw     r3, #0x14b0
003ade88: movw     r0, #0x14bc
003ade8c: str      r1, [r8, r3]
003ade90: ldr      r2, [r7, #4]
003ade94: movw     r3, #0x14b4
003ade98: str      r2, [r8, r3]
003ade9c: ldr      r3, [r7, #8]
003adea0: str      r1, [r8, r0]
003adea4: mov      r1, #0x14c0
003adea8: str      r2, [r8, r1]
003adeac: movw     r2, #0x14c4
003adeb0: str      r3, [r8, r2]
003adeb4: mvn      r1, #0
003adeb8: movw     r2, #0x14ca
003adebc: strh     r1, [r8, r2]
003adec0: movw     r2, #0x14b8
003adec4: str      r3, [r8, r2]
003adec8: b        #0x3ade00
003adecc: mov      r0, r4
003aded0: bl       #0x3c02e8
003aded4: cmp      r0, #0
003aded8: bne      #0x3ade4c
003adedc: movw     r3, #0x14c8
003adee0: strb     r0, [r8, r3]
003adee4: mvn      r1, #0
003adee8: movw     r3, #0x14ca
003adeec: strh     r1, [r8, r3]
003adef0: ldr      sb, [r5, #0x38]
003adef4: add      r2, r8, #0x3c8
003adef8: str      r0, [sp, #0x24]
003adefc: str      r2, [sp, #0x1c]
003adf00: ldr      r5, [sb, #0x60]!
003adf04: ldr      r1, [pc, #0x548]
003adf08: movw     r3, #0x23f0
003adf0c: movt     r3, #0x4974
003adf10: cmp      r5, sb
003adf14: str      r3, [sp, #0x30]
003adf18: str      r1, [sp, #0x34]
003adf1c: str      r6, [sp, #0x28]
003adf20: str      sl, [sp, #0x2c]
003adf24: beq      #0x3ae034
003adf28: ldr      r4, [r5, #8]
003adf2c: mov      r0, r4
003adf30: bl       #0x3935dc
003adf34: ldr      r2, [r0, #8]
003adf38: cmp      r4, r8
003adf3c: str      r2, [sp, #8]
003adf40: ldr      r6, [r0]
003adf44: ldr      r0, [r0, #4]
003adf48: ldr      sl, [r7]
003adf4c: ldr      fp, [r7, #4]
003adf50: str      r0, [sp, #0x10]
003adf54: ldr      r3, [r7, #8]
003adf58: str      r3, [sp, #0xc]
003adf5c: beq      #0x3ae028
003adf60: ldr      r3, [r4]
003adf64: mov      r0, r4
003adf68: mov      r1, r8
003adf6c: mov      lr, pc
003adf70: ldr      pc, [r3, #0x88]
003adf74: cmp      r0, #0
003adf78: beq      #0x3ae028
003adf7c: ldr      r1, [sp, #0x14]
003adf80: cmp      r1, #0
003adf84: bne      #0x3ae09c
003adf88: ldr      r0, [sp, #0x1c]
003adf8c: mov      r1, r4
003adf90: bl       #0x3d5a98
003adf94: cmp      r0, #0
003adf98: bne      #0x3ae028
003adf9c: ldr      r1, [sp, #8]
003adfa0: ldr      r0, [sp, #0xc]
003adfa4: bl       #0x30e3ac
003adfa8: ldr      r1, [sp, #0x10]
003adfac: mov      r3, r0
003adfb0: mov      r0, fp
003adfb4: str      r3, [sp]
003adfb8: bl       #0x30e3ac
003adfbc: mov      r1, r6
003adfc0: mov      fp, r0
003adfc4: mov      r0, sl
003adfc8: bl       #0x30e3ac
003adfcc: mov      r1, r0
003adfd0: bl       #0x30ed6c
003adfd4: mov      r1, fp
003adfd8: mov      r6, r0
003adfdc: mov      r0, fp
003adfe0: bl       #0x30ed6c
003adfe4: mov      r1, r0
003adfe8: mov      r0, r6
003adfec: bl       #0x30eba4
003adff0: ldr      r3, [sp]
003adff4: mov      r6, r0
003adff8: mov      r1, r3
003adffc: mov      r0, r3
003ae000: bl       #0x30ed6c
003ae004: mov      r1, r0
003ae008: mov      r0, r6
003ae00c: bl       #0x30eba4
003ae010: mov      r6, r0
003ae014: mov      r1, r6
003ae018: ldr      r0, [sp, #0x30]
003ae01c: bl       #0x30e2f8
003ae020: cmp      r0, #0
003ae024: bne      #0x3ae190
003ae028: ldr      r5, [r5]
003ae02c: cmp      r5, sb
003ae030: bne      #0x3adf28
003ae034: add      r2, sp, #0x24
003ae038: ldm      r2, {r2, r6, sl}
003ae03c: cmp      r2, #0
003ae040: beq      #0x3ae0b8
003ae044: ldr      r3, [pc, #0x40c]
003ae048: add      r4, sp, #0x9c
003ae04c: ldr      r5, [r6, r3]
003ae050: mov      r0, r5
003ae054: bl       #0x337888
003ae058: ldr      r1, [pc, #0x3fc]
003ae05c: add      r2, sp, #0x50
003ae060: mov      r0, r4
003ae064: add      r1, pc, r1
003ae068: bl       #0x3140ec
003ae06c: mov      r1, r4
003ae070: mov      r0, r5
003ae074: bl       #0x337a88
003ae078: mov      r0, r4
003ae07c: bl       #0x3139ac
003ae080: mov      r3, #1
003ae084: strb     r3, [r8, #0x413]
003ae088: ldr      r0, [sp, #0x1c]
003ae08c: ldr      r1, [sp, #0x24]
003ae090: mov      r2, #0
003ae094: bl       #0x3d6890
003ae098: b        #0x3ade00
003ae09c: ldr      r0, [sp, #0x1c]
003ae0a0: mov      r1, r4
003ae0a4: bl       #0x3d574c
003ae0a8: cmp      r0, #0
003ae0ac: bne      #0x3adf88
003ae0b0: ldr      r5, [r5]
003ae0b4: b        #0x3ae02c
003ae0b8: ldr      r3, [sp, #0x18]
003ae0bc: cmp      r3, #0
003ae0c0: beq      #0x3ae1d8
003ae0c4: ldr      r1, [sp, #0x14]
003ae0c8: cmp      r1, #0
003ae0cc: bne      #0x3ae1d8
003ae0d0: ldr      r2, [sp, #0x20]
003ae0d4: str      r1, [sp, #0xc]
003ae0d8: movw     r1, #0x23f0
003ae0dc: ldr      r3, [r6, r2]
003ae0e0: ldr      r2, [pc, #0x378]
003ae0e4: movt     r1, #0x4974
003ae0e8: ldr      r3, [r3, #0x38]
003ae0ec: add      r2, pc, r2
003ae0f0: str      r2, [sp, #8]
003ae0f4: ldr      r2, [pc, #0x358]
003ae0f8: str      r1, [sp, #0x10]
003ae0fc: ldr      r4, [r3, #0x14]
003ae100: add      fp, r3, #0xc
003ae104: add      sb, sp, #0x38
003ae108: str      r2, [sp, #0x24]
003ae10c: cmp      r4, fp
003ae110: beq      #0x3ae2dc
003ae114: ldr      r1, [r4, #0x2c]
003ae118: cmp      r1, #0
003ae11c: beq      #0x3ae16c
003ae120: mov      r0, sb
003ae124: bl       #0x33dd2c
003ae128: mov      r0, sb
003ae12c: bl       #0x33fee4
003ae130: subs     r5, r0, #0
003ae134: beq      #0x3ae16c
003ae138: cmp      r8, r5
003ae13c: beq      #0x3ae16c
003ae140: ldr      r3, [r5]
003ae144: mov      r1, r8
003ae148: mov      lr, pc
003ae14c: ldr      pc, [r3, #0x88]
003ae150: cmp      r0, #0
003ae154: beq      #0x3ae16c
003ae158: ldr      r0, [r5, #0x5c]
003ae15c: ldr      r1, [sp, #8]
003ae160: bl       #0x30e31c
003ae164: cmp      r0, #0
003ae168: bne      #0x3ae338
003ae16c: ldr      r2, [r4, #0xc]
003ae170: cmp      r2, #0
003ae174: beq      #0x3ae290
003ae178: mov      r4, r2
003ae17c: ldr      r3, [r4, #8]
003ae180: cmp      r3, #0
003ae184: beq      #0x3ae10c
003ae188: mov      r4, r3
003ae18c: b        #0x3ae17c
003ae190: mov      r1, r4
003ae194: ldr      r0, [sp, #0x1c]
003ae198: bl       #0x3d574c
003ae19c: ldr      r2, [sp, #0x34]
003ae1a0: ldr      r1, [sp, #0x28]
003ae1a4: cmp      r0, #0
003ae1a8: mov      r0, r4
003ae1ac: ldr      r3, [r1, r2]
003ae1b0: mov      r1, r7
003ae1b4: ldr      r3, [r3]
003ae1b8: ldrne    r2, [r3, #0x2c]
003ae1bc: ldreq    r2, [r3, #0x50]
003ae1c0: bl       #0x38ac0c
003ae1c4: cmp      r0, #0
003ae1c8: strne    r6, [sp, #0x30]
003ae1cc: strne    r4, [sp, #0x24]
003ae1d0: ldr      r5, [r5]
003ae1d4: b        #0x3ae02c
003ae1d8: ldr      sb, [pc, #0x278]
003ae1dc: add      r4, sp, #0x6c
003ae1e0: ldr      r5, [r6, sb]
003ae1e4: mov      r0, r5
003ae1e8: bl       #0x337888
003ae1ec: ldr      r1, [pc, #0x270]
003ae1f0: add      r2, sp, #0x48
003ae1f4: mov      r0, r4
003ae1f8: add      r1, pc, r1
003ae1fc: bl       #0x3140ec
003ae200: mov      r0, r5
003ae204: mov      r1, r4
003ae208: bl       #0x337a88
003ae20c: cmp      r0, #0
003ae210: beq      #0x3ae2c4
003ae214: mov      r0, r4
003ae218: bl       #0x3139ac
003ae21c: ldr      r5, [r6, sb]
003ae220: add      r4, sp, #0x54
003ae224: mov      r0, r5
003ae228: bl       #0x337888
003ae22c: ldr      r1, [pc, #0x234]
003ae230: add      r2, sp, #0x44
003ae234: mov      r0, r4
003ae238: add      r1, pc, r1
003ae23c: bl       #0x3140ec
003ae240: mov      r1, r4
003ae244: mov      r0, r5
003ae248: bl       #0x337a88
003ae24c: mov      r0, r4
003ae250: bl       #0x3139ac
003ae254: mov      r1, #0
003ae258: mov      r2, r1
003ae25c: ldr      r0, [sp, #0x1c]
003ae260: bl       #0x3d6890
003ae264: ldr      r0, [sp, #0x1c]
003ae268: bl       #0x3d49c4
003ae26c: ldr      r2, [sp, #0x18]
003ae270: cmp      r2, #0
003ae274: bne      #0x3ae42c
003ae278: mov      r0, r8
003ae27c: mov      r1, r7
003ae280: ldr      r3, [r8]
003ae284: mov      lr, pc
003ae288: ldr      pc, [r3, #0xe4]
003ae28c: b        #0x3ade00
003ae290: ldr      r3, [r4, #4]
003ae294: ldr      r1, [r3, #0xc]
003ae298: cmp      r4, r1
003ae29c: bne      #0x3ae2b8
003ae2a0: mov      r4, r3
003ae2a4: ldr      r3, [r3, #4]
003ae2a8: ldr      r2, [r3, #0xc]
003ae2ac: cmp      r2, r4
003ae2b0: beq      #0x3ae2a0
003ae2b4: ldr      r2, [r4, #0xc]
003ae2b8: cmp      r2, r3
003ae2bc: movne    r4, r3
003ae2c0: b        #0x3ae10c
003ae2c4: ldr      r1, [sp, #0x14]
003ae2c8: cmp      r1, #0
003ae2cc: beq      #0x3ae214
003ae2d0: mov      r0, r4
003ae2d4: bl       #0x3139ac
003ae2d8: b        #0x3ade00
003ae2dc: ldr      r3, [sp, #0xc]
003ae2e0: cmp      r3, #0
003ae2e4: beq      #0x3ae1d8
003ae2e8: ldr      r3, [pc, #0x168]
003ae2ec: add      r4, sp, #0x84
003ae2f0: ldr      r5, [r6, r3]
003ae2f4: mov      r0, r5
003ae2f8: bl       #0x337888
003ae2fc: ldr      r1, [pc, #0x168]
003ae300: add      r2, sp, #0x4c
003ae304: mov      r0, r4
003ae308: add      r1, pc, r1
003ae30c: bl       #0x3140ec
003ae310: mov      r1, r4
003ae314: mov      r0, r5
003ae318: bl       #0x337a88
003ae31c: mov      r0, r4
003ae320: bl       #0x3139ac
003ae324: ldr      r0, [sp, #0x1c]
003ae328: ldr      r1, [sp, #0xc]
003ae32c: mov      r2, #0
003ae330: bl       #0x3d6890
003ae334: b        #0x3ade00
003ae338: mov      r0, r5
003ae33c: bl       #0x3935dc
003ae340: ldr      r1, [r0]
003ae344: mov      r3, r0
003ae348: ldr      r0, [r7]
003ae34c: str      r3, [sp]
003ae350: bl       #0x30e3ac
003ae354: ldr      r3, [sp]
003ae358: mov      r2, r0
003ae35c: ldr      r0, [r7, #4]
003ae360: ldr      r1, [r3, #4]
003ae364: str      r2, [sp, #4]
003ae368: bl       #0x30e3ac
003ae36c: ldr      r3, [sp]
003ae370: mov      ip, r0
003ae374: ldr      r0, [r7, #8]
003ae378: ldr      r1, [r3, #8]
003ae37c: str      ip, [sp]
003ae380: bl       #0x30e3ac
003ae384: ldr      r2, [sp, #4]
003ae388: str      r0, [sp, #0x20]
003ae38c: mov      r1, r2
003ae390: mov      r0, r2
003ae394: bl       #0x30ed6c
003ae398: ldr      ip, [sp]
003ae39c: mov      r3, r0
003ae3a0: str      r3, [sp]
003ae3a4: mov      r1, ip
003ae3a8: mov      r0, ip
003ae3ac: bl       #0x30ed6c
003ae3b0: ldr      r3, [sp]
003ae3b4: mov      r1, r0
003ae3b8: mov      r0, r3
003ae3bc: bl       #0x30eba4
003ae3c0: mov      r3, r0
003ae3c4: ldr      r0, [sp, #0x20]
003ae3c8: str      r3, [sp]
003ae3cc: mov      r1, r0
003ae3d0: bl       #0x30ed6c
003ae3d4: ldr      r3, [sp]
003ae3d8: mov      r1, r0
003ae3dc: mov      r0, r3
003ae3e0: bl       #0x30eba4
003ae3e4: str      r0, [sp, #0x20]
003ae3e8: ldr      r1, [sp, #0x20]
003ae3ec: ldr      r0, [sp, #0x10]
003ae3f0: bl       #0x30e2f8
003ae3f4: cmp      r0, #0
003ae3f8: beq      #0x3ae16c
003ae3fc: ldr      r1, [sp, #0x24]
003ae400: mov      r0, r5
003ae404: ldr      r3, [r6, r1]
003ae408: mov      r1, r7
003ae40c: ldr      r3, [r3]
003ae410: ldr      r2, [r3, #0x54]
003ae414: bl       #0x38ac0c
003ae418: cmp      r0, #0
003ae41c: ldrne    r2, [sp, #0x20]
003ae420: strne    r5, [sp, #0xc]
003ae424: strne    r2, [sp, #0x10]
003ae428: b        #0x3ae16c
003ae42c: mov      r0, r8
003ae430: mov      r1, r7
003ae434: ldr      r3, [r8]
003ae438: mov      lr, pc
003ae43c: ldr      pc, [r3, #0xec]
003ae440: b        #0x3ade00
003ae444: bl       #0x30e310
003ae448: ldrheq   r6, [lr], #-0xc8
003ae44c: andeq    r4, r0, ip, lsr #1
003ae450: strdeq   r3, r4, [r0], -r4
003ae454: andeq    r3, r0, r8, asr #5
003ae458: andeq    r0, r0, r4, lsl #17
003ae45c: subseq   r5, r1, r4, asr r7
003ae460: subseq   r2, r1, ip, asr #7
003ae464: ldrsbeq  r0, [r1], #-0xf0
003ae468: subseq   r5, r1, r0, lsl #11
003ae46c: ldrheq   r5, [r1], #-0x40

# _ZN16Script_KillActor7ExecuteEbi
0045ea04: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0045ea08: ldr      r4, [pc, #0xf0]
0045ea0c: ldr      r6, [pc, #0xf0]
0045ea10: ldr      r1, [pc, #0xf0]
0045ea14: add      r4, pc, r4
0045ea18: ldr      r3, [r4, r6]
0045ea1c: ldr      r8, [r4, r1]
0045ea20: sub      sp, sp, #0x38
0045ea24: ldr      r3, [r3]
0045ea28: mov      sb, r2
0045ea2c: add      r7, sp, #0x1c
0045ea30: str      r3, [sp, #0x34]
0045ea34: ldr      sl, [r0, #0xc]
0045ea38: mov      r0, r8
0045ea3c: bl       #0x337888
0045ea40: ldr      r1, [pc, #0xc4]
0045ea44: add      r2, sp, #0x18
0045ea48: mov      r0, r7
0045ea4c: add      r1, pc, r1
0045ea50: bl       #0x3140ec
0045ea54: mov      r1, r7
0045ea58: mov      r0, r8
0045ea5c: bl       #0x337a88
0045ea60: mov      r0, r7
0045ea64: bl       #0x318254
0045ea68: ldr      r1, [pc, #0xa0]
0045ea6c: add      r5, sp, #0xc
0045ea70: ldr      r2, [sl, #0xc]
0045ea74: ldr      r1, [r4, r1]
0045ea78: mov      r7, #0
0045ea7c: mov      r3, sb
0045ea80: ldr      r1, [r1, #0x38]
0045ea84: mov      r0, r5
0045ea88: str      r7, [sp]
0045ea8c: str      r7, [sp, #4]
0045ea90: bl       #0x34aca0
0045ea94: mov      r0, r5
0045ea98: mov      r1, r7
0045ea9c: bl       #0x33fdc0
0045eaa0: cmp      r0, r7
0045eaa4: bne      #0x45eac4
0045eaa8: ldr      r3, [r4, r6]
0045eaac: ldr      r2, [sp, #0x34]
0045eab0: ldr      r3, [r3]
0045eab4: cmp      r2, r3
0045eab8: bne      #0x45eafc
0045eabc: add      sp, sp, #0x38
0045eac0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0045eac4: mov      r0, r5
0045eac8: bl       #0x33ff54
0045eacc: subs     r5, r0, #0
0045ead0: beq      #0x45eaa8
0045ead4: ldr      r3, [r5, #0x378]
0045ead8: mov      r2, #1
0045eadc: mov      r1, r7
0045eae0: strb     r2, [r3, #9]
0045eae4: ldr      r0, [r5, #0x378]
0045eae8: mov      r2, r7
0045eaec: bl       #0x40570c
0045eaf0: ldr      r3, [r5, #0x378]
0045eaf4: strb     r7, [r3, #9]
0045eaf8: b        #0x45eaa8
0045eafc: bl       #0x30e310
0045eb00: subseq   r6, r3, ip, ror r0
0045eb04: andeq    r4, r0, ip, lsr #1
0045eb08: andeq    r0, r0, r4, lsl #17
0045eb0c: subeq    lr, r6, r4, lsr r6
0045eb10: strdeq   r3, r4, [r0], -r4
