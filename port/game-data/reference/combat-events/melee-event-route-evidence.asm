
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

# _ZN6CharAI11OnAnimEventEPKc
003d0cc4: push     {r4, lr}
003d0cc8: ldr      r3, [r0, #0x1c]
003d0ccc: cmp      r3, #0
003d0cd0: beq      #0x3d0ce4
003d0cd4: mov      r0, r3
003d0cd8: ldr      r3, [r3]
003d0cdc: mov      lr, pc
003d0ce0: ldr      pc, [r3, #0x94]
003d0ce4: pop      {r4, pc}

# _ZN6CharAI12_OnAnimEventEPKc
003d4434: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d4438: ldr      r4, [pc, #0x528]
003d443c: ldr      r7, [pc, #0x528]
003d4440: mov      r6, r0
003d4444: add      r4, pc, r4
003d4448: ldr      r3, [r4, r7]
003d444c: ldr      r0, [r0, #4]
003d4450: sub      sp, sp, #0xd4
003d4454: ldr      r3, [r3]
003d4458: add      r0, r0, #0x490
003d445c: add      r0, r0, #0xc
003d4460: mov      r5, r1
003d4464: str      r3, [sp, #0xcc]
003d4468: bl       #0x3c932c
003d446c: mov      r8, r0
003d4470: ldr      r0, [r6, #4]
003d4474: add      r0, r0, #0x490
003d4478: add      r0, r0, #0xc
003d447c: bl       #0x3c934c
003d4480: ldr      r1, [pc, #0x4e8]
003d4484: mov      r0, r5
003d4488: mov      r2, #3
003d448c: add      r1, pc, r1
003d4490: bl       #0x30ec7c
003d4494: cmp      r0, #0
003d4498: beq      #0x3d4634
003d449c: ldr      r1, [pc, #0x4d0]
003d44a0: mov      r0, r5
003d44a4: mov      r2, #3
003d44a8: add      r1, pc, r1
003d44ac: bl       #0x30ec7c
003d44b0: cmp      r0, #0
003d44b4: beq      #0x3d464c
003d44b8: ldr      r1, [pc, #0x4b8]
003d44bc: mov      r0, r5
003d44c0: mov      r2, #3
003d44c4: add      r1, pc, r1
003d44c8: bl       #0x30ec7c
003d44cc: subs     sl, r0, #0
003d44d0: beq      #0x3d45bc
003d44d4: ldr      r1, [pc, #0x4a0]
003d44d8: mov      r0, r5
003d44dc: mov      r2, #4
003d44e0: add      r1, pc, r1
003d44e4: bl       #0x30ec7c
003d44e8: cmp      r0, #0
003d44ec: bne      #0x3d465c
003d44f0: ldr      r3, [pc, #0x488]
003d44f4: add      r5, r5, #4
003d44f8: ldr      r3, [r4, r3]
003d44fc: ldr      sb, [r3]
003d4500: cmp      sb, #0
003d4504: beq      #0x3d459c
003d4508: ldr      r3, [pc, #0x474]
003d450c: mov      r8, r0
003d4510: ldr      r3, [r4, r3]
003d4514: ldr      fp, [r3]
003d4518: b        #0x3d4528
003d451c: add      r8, r8, #1
003d4520: cmp      r8, sb
003d4524: beq      #0x3d459c
003d4528: mov      r0, r5
003d452c: ldr      r1, [fp, r8, lsl #2]
003d4530: bl       #0x30e31c
003d4534: subs     sl, r0, #0
003d4538: bne      #0x3d451c
003d453c: cmn      r8, #1
003d4540: beq      #0x3d459c
003d4544: ldr      r3, [pc, #0x43c]
003d4548: ldr      r0, [r6, #4]
003d454c: ldr      r3, [r4, r3]
003d4550: ldr      sb, [r3]
003d4554: bl       #0x3935dc
003d4558: ldr      lr, [r0, #4]
003d455c: ldr      r5, [r0]
003d4560: ldr      r6, [r0, #8]
003d4564: mov      ip, #0xbf000000
003d4568: add      ip, ip, #0x800000
003d456c: str      lr, [sp, #0x14]
003d4570: mov      r0, sb
003d4574: mov      lr, #1
003d4578: mov      r1, r8
003d457c: mov      r3, sl
003d4580: add      r2, sp, #0x10
003d4584: str      r5, [sp, #0x10]
003d4588: str      r6, [sp, #0x18]
003d458c: str      lr, [sp]
003d4590: str      ip, [sp, #8]
003d4594: str      ip, [sp, #4]
003d4598: bl       #0x36b5d8
003d459c: ldr      r3, [r4, r7]
003d45a0: ldr      r2, [sp, #0xcc]
003d45a4: mov      r0, #1
003d45a8: ldr      r3, [r3]
003d45ac: cmp      r2, r3
003d45b0: bne      #0x3d4964
003d45b4: add      sp, sp, #0xd4
003d45b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d45bc: ldr      r3, [pc, #0x3c8]
003d45c0: add      r5, r5, #3
003d45c4: ldr      r3, [r4, r3]
003d45c8: ldr      sb, [r3]
003d45cc: cmp      sb, #0
003d45d0: beq      #0x3d459c
003d45d4: ldr      r3, [pc, #0x3b4]
003d45d8: ldr      r3, [r4, r3]
003d45dc: ldr      fp, [r3]
003d45e0: b        #0x3d45f0
003d45e4: add      sl, sl, #1
003d45e8: cmp      sl, sb
003d45ec: beq      #0x3d459c
003d45f0: mov      r0, r5
003d45f4: ldr      r1, [fp, sl, lsl #2]
003d45f8: bl       #0x30e31c
003d45fc: subs     r8, r0, #0
003d4600: bne      #0x3d45e4
003d4604: cmn      sl, #1
003d4608: beq      #0x3d459c
003d460c: ldr      r0, [r6, #4]
003d4610: bl       #0x3935dc
003d4614: ldr      r3, [pc, #0x378]
003d4618: mov      r2, r0
003d461c: mov      r1, sl
003d4620: ldr      r0, [r4, r3]
003d4624: mov      r3, r8
003d4628: str      r8, [sp]
003d462c: bl       #0x495d14
003d4630: b        #0x3d459c
003d4634: mov      r0, r6
003d4638: add      r1, r5, #3
003d463c: ldr      r3, [r6]
003d4640: mov      lr, pc
003d4644: ldr      pc, [r3, #0x94]
003d4648: b        #0x3d459c
003d464c: mov      r0, r6
003d4650: add      r1, r5, #3
003d4654: bl       #0x3d3af4
003d4658: b        #0x3d459c
003d465c: ldr      r0, [r6, #4]
003d4660: add      r0, r0, #0x4f0
003d4664: add      r0, r0, #0xc
003d4668: bl       #0x3c01ac
003d466c: sub      r0, r0, #5
003d4670: cmp      r0, #8
003d4674: addls    pc, pc, r0, lsl #2
003d4678: b        #0x3d459c
003d467c: b        #0x3d4750
003d4680: b        #0x3d46f8
003d4684: b        #0x3d46a0
003d4688: b        #0x3d459c
003d468c: b        #0x3d459c
003d4690: b        #0x3d459c
003d4694: b        #0x3d459c
003d4698: b        #0x3d459c
003d469c: b        #0x3d482c
003d46a0: ldr      r1, [pc, #0x2f0]
003d46a4: mov      r0, r5
003d46a8: add      r1, pc, r1
003d46ac: bl       #0x30e31c
003d46b0: cmp      r0, #0
003d46b4: bne      #0x3d459c
003d46b8: ldr      r3, [pc, #0x2dc]
003d46bc: add      r5, sp, #0x3c
003d46c0: ldr      r8, [r4, r3]
003d46c4: mov      r0, r8
003d46c8: bl       #0x337888
003d46cc: add      r1, sp, #0x24
003d46d0: mov      r0, r5
003d46d4: bl       #0x3d43ec
003d46d8: mov      r1, r5
003d46dc: mov      r0, r8
003d46e0: bl       #0x337a88
003d46e4: mov      r0, r5
003d46e8: bl       #0x3139ac
003d46ec: mov      r0, r6
003d46f0: bl       #0x3d8ba4
003d46f4: b        #0x3d459c
003d46f8: ldr      r1, [pc, #0x2a0]
003d46fc: mov      r0, r5
003d4700: add      r1, pc, r1
003d4704: bl       #0x30e31c
003d4708: cmp      r0, #0
003d470c: bne      #0x3d459c
003d4710: ldr      r3, [pc, #0x284]
003d4714: add      r5, sp, #0x54
003d4718: ldr      r8, [r4, r3]
003d471c: mov      r0, r8
003d4720: bl       #0x337888
003d4724: add      r1, sp, #0x28
003d4728: mov      r0, r5
003d472c: bl       #0x3d43ec
003d4730: mov      r1, r5
003d4734: mov      r0, r8
003d4738: bl       #0x337a88
003d473c: mov      r0, r5
003d4740: bl       #0x3139ac
003d4744: mov      r0, r6
003d4748: bl       #0x3d8bf8
003d474c: b        #0x3d459c
003d4750: ldr      r3, [r6, #4]
003d4754: add      r1, sp, #0x20
003d4758: mov      r2, r1
003d475c: mov      r0, r3
003d4760: ldr      ip, [r3]
003d4764: add      r3, sp, #0x1c
003d4768: mov      lr, pc
003d476c: ldr      pc, [ip, #0x128]
003d4770: cmp      r0, #0
003d4774: beq      #0x3d488c
003d4778: ldr      r1, [pc, #0x224]
003d477c: mov      r0, r5
003d4780: add      r1, pc, r1
003d4784: bl       #0x30e31c
003d4788: cmp      r0, #0
003d478c: beq      #0x3d47c0
003d4790: ldr      r1, [pc, #0x210]
003d4794: mov      r0, r5
003d4798: add      r1, pc, r1
003d479c: bl       #0x30e31c
003d47a0: cmp      r0, #0
003d47a4: beq      #0x3d47c0
003d47a8: ldr      r1, [pc, #0x1fc]
003d47ac: mov      r0, r5
003d47b0: add      r1, pc, r1
003d47b4: bl       #0x30e31c
003d47b8: cmp      r0, #0
003d47bc: bne      #0x3d459c
003d47c0: ldr      r3, [pc, #0x1d4]
003d47c4: add      r5, sp, #0xb4
003d47c8: ldr      r8, [r4, r3]
003d47cc: mov      r0, r8
003d47d0: bl       #0x337888
003d47d4: add      r1, sp, #0x38
003d47d8: mov      r0, r5
003d47dc: bl       #0x3d43ec
003d47e0: mov      r1, r5
003d47e4: mov      r0, r8
003d47e8: bl       #0x337a88
003d47ec: mov      r0, r5
003d47f0: bl       #0x3139ac
003d47f4: ldr      r3, [pc, #0x1b4]
003d47f8: mov      ip, #0
003d47fc: ldr      r2, [r6, #4]
003d4800: ldr      r0, [r4, r3]
003d4804: ldr      r3, [pc, #0x1a8]
003d4808: ldr      r1, [sp, #0x1c]
003d480c: str      ip, [sp]
003d4810: ldr      lr, [r4, r3]
003d4814: mov      r3, ip
003d4818: str      ip, [sp, #8]
003d481c: str      lr, [sp, #4]
003d4820: str      ip, [sp, #0xc]
003d4824: bl       #0x3e701c
003d4828: b        #0x3d459c
003d482c: ldr      r1, [pc, #0x184]
003d4830: mov      r0, r5
003d4834: add      r1, pc, r1
003d4838: bl       #0x30e31c
003d483c: cmp      r0, #0
003d4840: bne      #0x3d459c
003d4844: ldr      r3, [pc, #0x150]
003d4848: add      r5, sp, #0x6c
003d484c: ldr      r8, [r4, r3]
003d4850: mov      r0, r8
003d4854: bl       #0x337888
003d4858: add      r1, sp, #0x2c
003d485c: mov      r0, r5
003d4860: bl       #0x3d43ec
003d4864: mov      r1, r5
003d4868: mov      r0, r8
003d486c: bl       #0x337a88
003d4870: mov      r0, r5
003d4874: bl       #0x3139ac
003d4878: mov      r0, r6
003d487c: ldr      r3, [r6]
003d4880: mov      lr, pc
003d4884: ldr      pc, [r3, #0xa0]
003d4888: b        #0x3d459c
003d488c: ldr      r1, [pc, #0x128]
003d4890: mov      r0, r5
003d4894: add      r1, pc, r1
003d4898: bl       #0x30e31c
003d489c: subs     sl, r0, #0
003d48a0: beq      #0x3d4910
003d48a4: ldr      r1, [pc, #0x114]
003d48a8: mov      r0, r5
003d48ac: add      r1, pc, r1
003d48b0: bl       #0x30e31c
003d48b4: cmp      r0, #0
003d48b8: bne      #0x3d459c
003d48bc: ldr      r3, [pc, #0xd8]
003d48c0: add      r5, sp, #0x84
003d48c4: ldr      sl, [r4, r3]
003d48c8: mov      r0, sl
003d48cc: bl       #0x337888
003d48d0: add      r1, sp, #0x30
003d48d4: mov      r0, r5
003d48d8: bl       #0x3d43ec
003d48dc: mov      r1, r5
003d48e0: mov      r0, sl
003d48e4: bl       #0x337a88
003d48e8: mov      r0, r5
003d48ec: bl       #0x3139ac
003d48f0: mov      r0, r6
003d48f4: sub      r2, r8, #1
003d48f8: ldr      ip, [r6]
003d48fc: ldr      r1, [r6, #0x74]
003d4900: mov      r3, #1
003d4904: mov      lr, pc
003d4908: ldr      pc, [ip, #0xa8]
003d490c: b        #0x3d459c
003d4910: ldr      r3, [pc, #0x84]
003d4914: add      r5, sp, #0x9c
003d4918: ldr      sb, [r4, r3]
003d491c: mov      r0, sb
003d4920: bl       #0x337888
003d4924: add      r1, sp, #0x34
003d4928: mov      r0, r5
003d492c: bl       #0x3d43ec
003d4930: mov      r1, r5
003d4934: mov      r0, sb
003d4938: bl       #0x337a88
003d493c: mov      r0, r5
003d4940: bl       #0x3139ac
003d4944: mov      r0, r6
003d4948: sub      r2, r8, #1
003d494c: mov      r3, sl
003d4950: ldr      ip, [r6]
003d4954: ldr      r1, [r6, #0x74]
003d4958: mov      lr, pc
003d495c: ldr      pc, [ip, #0xa8]
003d4960: b        #0x3d459c
003d4964: bl       #0x30e310
003d4968: subseq   r0, ip, ip, asr #12
003d496c: andeq    r4, r0, ip, lsr #1
003d4970: strheq   r1, [pc], #-4
003d4974: subeq    r1, pc, r0, lsr #1
003d4978: subeq    r1, pc, ip, lsl #1
003d497c: subeq    r1, pc, r8, ror r0
003d4980: andeq    r3, r0, r8, lsr sp
003d4984: andeq    r3, r0, r8, lsr #19
003d4988: andeq    r0, r0, r4, lsr #27
003d498c: andeq    r0, r0, r4, asr #13
003d4990: muleq    r0, r4, r2
003d4994: andeq    r1, r0, r8, lsl #22
003d4998: subeq    r0, pc, r8, lsl #30
003d499c: andeq    r0, r0, r4, lsl #17
003d49a0: subeq    r0, pc, r0, lsl #29
003d49a4: subeq    r0, pc, r0, ror #27
003d49a8: ldrdeq   r0, r1, [pc], #-0xd8
003d49ac: ldrdeq   r0, r1, [pc], #-0xd0
003d49b0: andeq    r0, r0, ip, asr #16
003d49b4: andeq    r1, r0, r4, lsr r8
003d49b8: subeq    r0, pc, ip, ror #26
003d49bc: ldrdeq   r0, r1, [pc], #-0xcc
003d49c0: subeq    r0, pc, r4, ror #25

# _ZN9Character13F_MeleeAttackERNS_12AttackResultEPS_S2_bb
003b3368: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b336c: ldr      r4, [pc, #0x240]
003b3370: ldr      ip, [pc, #0x240]
003b3374: subs     r5, r1, #0
003b3378: add      r4, pc, r4
003b337c: ldr      r1, [r4, ip]
003b3380: sub      sp, sp, #0x4c
003b3384: mov      r6, r2
003b3388: str      r3, [sp, #0x14]
003b338c: ldr      r2, [r1]
003b3390: ldrb     r3, [sp, #0x70]
003b3394: str      ip, [sp, #0x18]
003b3398: str      r0, [sp, #0x20]
003b339c: str      r3, [sp, #0x1c]
003b33a0: str      r2, [sp, #0x44]
003b33a4: beq      #0x3b355c
003b33a8: cmp      r6, #0
003b33ac: beq      #0x3b3508
003b33b0: ldr      r3, [pc, #0x204]
003b33b4: add      r7, sp, #0x2c
003b33b8: ldr      r8, [r4, r3]
003b33bc: mov      r0, r8
003b33c0: bl       #0x337888
003b33c4: ldr      r1, [pc, #0x1f4]
003b33c8: add      r2, sp, #0x28
003b33cc: mov      r0, r7
003b33d0: add      r1, pc, r1
003b33d4: bl       #0x3140ec
003b33d8: mov      r1, r7
003b33dc: mov      r0, r8
003b33e0: bl       #0x337a88
003b33e4: mov      r0, r7
003b33e8: bl       #0x3139ac
003b33ec: ldr      ip, [sp, #0x14]
003b33f0: ldr      r2, [sp, #0x1c]
003b33f4: movw     r3, #0xaab5
003b33f8: cmp      ip, #0
003b33fc: movt     r3, #0x22
003b3400: movne    r1, #2
003b3404: moveq    r1, #1
003b3408: movw     r7, #0x554a
003b340c: cmp      r2, #0
003b3410: add      r0, r5, #0x37c
003b3414: movt     r7, #5
003b3418: moveq    r7, r3
003b341c: bl       #0x3ffe3c
003b3420: cmp      r0, #0
003b3424: mvneq    r3, #0
003b3428: streq    r3, [sp, #0x24]
003b342c: beq      #0x3b343c
003b3430: bl       #0x3f9e08
003b3434: ldr      r0, [r0, #0x94]
003b3438: str      r0, [sp, #0x24]
003b343c: ldr      r3, [pc, #0x180]
003b3440: ldr      r3, [r4, r3]
003b3444: ldr      sl, [r3]
003b3448: cmp      sl, #0
003b344c: beq      #0x3b34b0
003b3450: ldr      r3, [pc, #0x170]
003b3454: ldr      fp, [pc, #0x170]
003b3458: mov      r8, #0
003b345c: ldr      r3, [r4, r3]
003b3460: add      fp, pc, fp
003b3464: ldr      sb, [r3]
003b3468: b        #0x3b3478
003b346c: add      r8, r8, #1
003b3470: cmp      r8, sl
003b3474: beq      #0x3b34b0
003b3478: mov      r0, fp
003b347c: ldr      r1, [sb, r8, lsl #2]
003b3480: bl       #0x30e31c
003b3484: cmp      r0, #0
003b3488: bne      #0x3b346c
003b348c: cmn      r8, #1
003b3490: beq      #0x3b34b0
003b3494: ldr      ip, [sp, #0x1c]
003b3498: cmp      ip, #0
003b349c: beq      #0x3b34b0
003b34a0: mov      r2, r0
003b34a4: mov      r1, r8
003b34a8: add      r0, r5, #0x560
003b34ac: bl       #0x3df3b8
003b34b0: ldr      r2, [sp, #0x14]
003b34b4: ldr      ip, [sp, #0x24]
003b34b8: ldr      r0, [sp, #0x20]
003b34bc: cmp      r2, #0
003b34c0: orrne    r7, r7, #0x4000000
003b34c4: str      ip, [sp]
003b34c8: mvn      ip, #0
003b34cc: mov      r2, r6
003b34d0: mov      r3, r7
003b34d4: str      ip, [sp, #4]
003b34d8: mov      r1, r5
003b34dc: mov      ip, #0
003b34e0: str      ip, [sp, #8]
003b34e4: bl       #0x3b2638
003b34e8: ldr      r2, [sp, #0x18]
003b34ec: ldr      r3, [r4, r2]
003b34f0: ldr      r2, [sp, #0x44]
003b34f4: ldr      r3, [r3]
003b34f8: cmp      r2, r3
003b34fc: bne      #0x3b35b0
003b3500: add      sp, sp, #0x4c
003b3504: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b3508: ldr      r3, [pc, #0xc0]
003b350c: ldr      r3, [r4, r3]
003b3510: ldr      r3, [r3]
003b3514: cmp      r3, #2
003b3518: streq    r6, [r6]
003b351c: beq      #0x3b33b0
003b3520: cmp      r3, #1
003b3524: bne      #0x3b33b0
003b3528: ldr      r0, [pc, #0xa4]
003b352c: ldr      r1, [pc, #0xa4]
003b3530: ldr      r2, [pc, #0xa4]
003b3534: ldr      r0, [r4, r0]
003b3538: ldr      r3, [pc, #0xa0]
003b353c: movw     ip, #0x249
003b3540: add      r1, pc, r1
003b3544: add      r2, pc, r2
003b3548: add      r3, pc, r3
003b354c: add      r0, r0, #0xa8
003b3550: str      ip, [sp]
003b3554: bl       #0x30e004
003b3558: b        #0x3b33b0
003b355c: ldr      r3, [pc, #0x6c]
003b3560: ldr      r3, [r4, r3]
003b3564: ldr      r3, [r3]
003b3568: cmp      r3, #2
003b356c: streq    r5, [r5]
003b3570: beq      #0x3b33a8
003b3574: cmp      r3, #1
003b3578: bne      #0x3b33a8
003b357c: ldr      r0, [pc, #0x50]
003b3580: ldr      r1, [pc, #0x5c]
003b3584: ldr      r2, [pc, #0x5c]
003b3588: ldr      r0, [r4, r0]
003b358c: ldr      r3, [pc, #0x58]
003b3590: mov      ip, #0x248
003b3594: add      r1, pc, r1
003b3598: add      r2, pc, r2
003b359c: add      r3, pc, r3
003b35a0: add      r0, r0, #0xa8
003b35a4: str      ip, [sp]
003b35a8: bl       #0x30e004
003b35ac: b        #0x3b33a8
003b35b0: bl       #0x30e310
003b35b4: subseq   r1, lr, r8, lsl r7
003b35b8: andeq    r4, r0, ip, lsr #1
003b35bc: andeq    r0, r0, r4, lsl #17
003b35c0: subseq   r0, r1, r8, ror #17
003b35c4: andeq    r3, r0, r8, ror #10
003b35c8: muleq    r0, r0, sl
003b35cc: ldrsheq  r0, [r1], #-0x80
003b35d0: andeq    r3, r0, r0, asr #19
003b35d4: andeq    r1, r0, r0, asr #19

# _ZN6CharAI23_OnAnimStepBegin_AttackEv
003d4044: push     {r4, r5, r6, lr}
003d4048: ldr      r3, [r0, #4]
003d404c: mov      r4, r0
003d4050: add      r0, r3, #0x490
003d4054: add      r0, r0, #0xc
003d4058: ldr      r5, [r3, #0x4c8]
003d405c: bl       #0x3c932c
003d4060: mov      r6, r0
003d4064: ldr      r0, [r4, #4]
003d4068: add      r0, r0, #0x490
003d406c: add      r0, r0, #0xc
003d4070: bl       #0x3c934c
003d4074: cmp      r5, #0
003d4078: beq      #0x3d40c8
003d407c: cmp      r5, #1
003d4080: beq      #0x3d408c
003d4084: mov      r0, #1
003d4088: pop      {r4, r5, r6, pc}
003d408c: cmp      r6, #0
003d4090: bne      #0x3d40e4
003d4094: ldr      r3, [r4, #4]
003d4098: strb     r5, [r4, #0x79]
003d409c: ldr      r1, [r3, #0x408]
003d40a0: ldr      r0, [r3, #0x378]
003d40a4: bl       #0x4052bc
003d40a8: mov      r0, r4
003d40ac: strb     r6, [r4, #0x7a]
003d40b0: ldr      r3, [r4]
003d40b4: ldr      r1, [r4, #0x74]
003d40b8: mov      lr, pc
003d40bc: ldr      pc, [r3, #0xa4]
003d40c0: mov      r0, #1
003d40c4: pop      {r4, r5, r6, pc}
003d40c8: ldr      r0, [r4, #4]
003d40cc: str      r6, [r4, #0x74]
003d40d0: mov      r2, r6
003d40d4: mov      r1, #0x1a
003d40d8: bl       #0x3a4d5c
003d40dc: mov      r0, #1
003d40e0: pop      {r4, r5, r6, pc}
003d40e4: ldr      r3, [r4, #4]
003d40e8: sub      r0, r0, #1
003d40ec: cmp      r6, r0
003d40f0: movne    r6, #0
003d40f4: moveq    r6, #1
003d40f8: strb     r6, [r4, #0x79]
003d40fc: ldr      r1, [r3, #0x408]
003d4100: ldr      r0, [r3, #0x378]
003d4104: bl       #0x4052bc
003d4108: cmp      r6, #0
003d410c: mov      r3, #0
003d4110: strb     r3, [r4, #0x7a]
003d4114: mov      r0, #1
003d4118: strbne   r5, [r4, #0x7a]
003d411c: pop      {r4, r5, r6, pc}
