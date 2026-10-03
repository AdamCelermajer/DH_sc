
# _ZN14ObjectSearcher10TargetList11SetSortTypeEi.clone.1
003d015c: push     {r4, r5, r6, lr}
003d0160: ldr      r2, [r0, #0x10]
003d0164: ldr      r3, [r0]
003d0168: ldr      r5, [pc, #0x34]
003d016c: mov      r4, r0
003d0170: cmp      r2, r3
003d0174: add      r5, pc, r5
003d0178: beq      #0x3d0194
003d017c: mov      r0, r4
003d0180: bl       #0x38fb18
003d0184: ldr      r2, [r4, #0x10]
003d0188: ldr      r3, [r4]
003d018c: cmp      r2, r3
003d0190: bne      #0x3d017c
003d0194: ldr      r3, [pc, #0xc]
003d0198: ldr      r3, [r5, r3]
003d019c: str      r3, [r4, #0x28]
003d01a0: pop      {r4, r5, r6, pc}
003d01a4: subseq   r4, ip, ip, lsl sb
003d01a8: ldrdeq   r1, r2, [r0], -r4

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

# _ZNK16CharStateMachine14SM_IsAttackingEv
003c02d0: push     {r4, lr}
003c02d4: bl       #0x3c01ac
003c02d8: cmp      r0, #5
003c02dc: movne    r0, #0
003c02e0: moveq    r0, #1
003c02e4: pop      {r4, pc}

# _ZN16CharStateMachine17SM_SetAttackStateEPvb
003c6488: cmp      r2, #0
003c648c: bne      #0x3c649c
003c6490: mov      r2, r1
003c6494: movw     r1, #0xc354
003c6498: b        #0x3c5684
003c649c: mov      r3, r1
003c64a0: movw     r2, #0xc354
003c64a4: mov      r1, #5
003c64a8: b        #0x3c1938

# _ZN14ObjectSearcher10TargetList6SearchEff
003d0020: ldr      ip, [pc, #0x4c]
003d0024: push     {r4, lr}
003d0028: ldr      lr, [pc, #0x48]
003d002c: add      ip, pc, ip
003d0030: ldr      r3, [pc, #0x44]
003d0034: ldr      lr, [ip, lr]
003d0038: sub      sp, sp, #0x18
003d003c: ldr      r3, [ip, r3]
003d0040: ldr      r4, [lr, #0x38]
003d0044: add      r3, r3, #8
003d0048: add      lr, r4, #0x80
003d004c: stmib    sp, {r3, lr}
003d0050: ldr      r4, [r4, #0x80]
003d0054: add      r3, sp, #4
003d0058: str      lr, [sp, #0x10]
003d005c: mov      lr, #0
003d0060: str      r4, [sp, #0xc]
003d0064: str      lr, [sp, #0x14]
003d0068: bl       #0x4a3428
003d006c: add      sp, sp, #0x18
003d0070: pop      {r4, pc}
003d0074: subseq   r4, ip, r4, ror #20
003d0078: strdeq   r3, r4, [r0], -r4
003d007c: andeq    r3, r0, r4, ror #10

# _ZN12v2Controller10Cmd_AttackEP10GameObject
00405b04: push     {r4, r5, r6, r7, r8, lr}
00405b08: ldrb     r2, [r0, #9]
00405b0c: ldr      r3, [pc, #0x19c]
00405b10: mov      r4, r0
00405b14: cmp      r2, #0
00405b18: mov      r5, r1
00405b1c: add      r3, pc, r3
00405b20: bne      #0x405b48
00405b24: ldr      r2, [pc, #0x188]
00405b28: ldr      r3, [r3, r2]
00405b2c: ldrb     r3, [r3]
00405b30: cmp      r3, #0
00405b34: beq      #0x405b3c
00405b38: pop      {r4, r5, r6, r7, r8, pc}
00405b3c: ldrb     r3, [r0, #8]
00405b40: cmp      r3, #0
00405b44: bne      #0x405b38
00405b48: bl       #0x7fd794
00405b4c: ldrb     r3, [r0, #5]
00405b50: cmp      r3, #0
00405b54: beq      #0x405c20
00405b58: ldrb     r3, [r4, #0xa]
00405b5c: cmp      r3, #0
00405b60: beq      #0x405c20
00405b64: ldr      r3, [r4, #0xc]
00405b68: cmp      r3, #0
00405b6c: beq      #0x405c20
00405b70: add      r0, r3, #0x3c8
00405b74: mov      r1, r5
00405b78: mov      r2, #1
00405b7c: ldr      r6, [r3, #0x408]
00405b80: ldr      r8, [r3, #0x40c]
00405b84: ldrb     r7, [r3, #0x440]
00405b88: bl       #0x3d01ac
00405b8c: ldr      r0, [r4, #0xc]
00405b90: ldr      r3, [r0, #0x408]
00405b94: cmp      r6, r3
00405b98: beq      #0x405c3c
00405b9c: mov      r1, r8
00405ba0: mov      r2, #1
00405ba4: add      r0, r0, #0x3c8
00405ba8: bl       #0x3d6890
00405bac: ldr      r0, [r4, #0xc]
00405bb0: add      r0, r0, #0x3c8
00405bb4: bl       #0x3d49c4
00405bb8: ldr      r0, [r4, #0xc]
00405bbc: mov      r1, r6
00405bc0: mov      r2, #1
00405bc4: add      r0, r0, #0x3c8
00405bc8: bl       #0x3d6890
00405bcc: ldr      r3, [r4, #0xc]
00405bd0: strb     r7, [r3, #0x440]
00405bd4: bl       #0x80b1bc
00405bd8: cmp      r5, #0
00405bdc: mov      r7, r0
00405be0: ldr      r0, [pc, #0xd0]
00405be4: ldr      r3, [r4, #0xc]
00405be8: ldrne    r6, [r5, #0x108]
00405bec: mov      r1, #1
00405bf0: add      r0, pc, r0
00405bf4: ldrb     r8, [r3, #0x108]
00405bf8: moveq    r6, r5
00405bfc: uxthne   r6, r6
00405c00: bl       #0x80a244
00405c04: mov      r3, #0
00405c08: mov      r1, r0
00405c0c: strb     r8, [r0, #0x54]
00405c10: strb     r3, [r0, #0x50]
00405c14: strh     r6, [r0, #0x52]
00405c18: mov      r0, r7
00405c1c: bl       #0x80e2a4
00405c20: ldr      r3, [r4, #4]
00405c24: mov      r1, r5
00405c28: mov      r0, r3
00405c2c: ldr      r3, [r3]
00405c30: mov      lr, pc
00405c34: ldr      pc, [r3, #0x38]
00405c38: pop      {r4, r5, r6, r7, r8, pc}
00405c3c: ldrb     r3, [r0, #0x440]
00405c40: cmp      r3, r7
00405c44: bne      #0x405b9c
00405c48: add      r0, r0, #0x4f0
00405c4c: add      r0, r0, #0xc
00405c50: bl       #0x3c02d0
00405c54: cmp      r0, #0
00405c58: ldrne    r3, [r4, #0xc]
00405c5c: bne      #0x405c74
00405c60: ldr      r3, [r4, #0xc]
00405c64: ldrb     r2, [r3, #0x440]
00405c68: mov      r0, r3
00405c6c: cmp      r2, #0
00405c70: beq      #0x405b9c
00405c74: add      r0, r3, #0x3c8
00405c78: mov      r1, r8
00405c7c: mov      r2, #1
00405c80: bl       #0x3d6890
00405c84: ldr      r0, [r4, #0xc]
00405c88: add      r0, r0, #0x3c8
00405c8c: bl       #0x3d49c4
00405c90: ldr      r0, [r4, #0xc]
00405c94: mov      r1, r6
00405c98: mov      r2, #1
00405c9c: add      r0, r0, #0x3c8
00405ca0: bl       #0x3d6890
00405ca4: ldr      r3, [r4, #0xc]
00405ca8: strb     r7, [r3, #0x440]
00405cac: b        #0x405c20
00405cb0: subseq   lr, r8, r4, ror pc
00405cb4: andeq    r3, r0, r0, asr r6
00405cb8: subeq    sb, fp, r8, lsl r3

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

# _ZThn884_N9Character11Ctrl_AttackEP10GameObject
003ad874: sub      r0, r0, #0x374
003ad878: b        #0x3ad87c

# _ZN9Character11Ctrl_AttackEP10GameObject
003ad87c: add      r0, r0, #0x3c8
003ad880: mov      r2, #0
003ad884: b        #0x3d01ac

# _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
004a2730: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a2734: ldr      r6, [pc, #0x10c]
004a2738: ldr      r7, [pc, #0x10c]
004a273c: mov      r5, #0
004a2740: add      r6, pc, r6
004a2744: str      r5, [r0]
004a2748: str      r5, [r0, #4]
004a274c: str      r5, [r0, #8]
004a2750: str      r5, [r0, #0xc]
004a2754: str      r5, [r0, #0x10]
004a2758: str      r5, [r0, #0x14]
004a275c: str      r5, [r0, #0x18]
004a2760: str      r5, [r0, #0x1c]
004a2764: str      r5, [r0, #0x20]
004a2768: str      r5, [r0, #0x24]
004a276c: mov      r4, r0
004a2770: mov      r8, r2
004a2774: mov      sl, r3
004a2778: mov      fp, r1
004a277c: ldr      sb, [sp, #0x28]
004a2780: bl       #0x4a2240
004a2784: ldr      r2, [r6, r7]
004a2788: mov      r3, r4
004a278c: str      r8, [r4, #0x34]
004a2790: str      r2, [r4, #0x28]
004a2794: str      sl, [r4, #0x38]
004a2798: str      r5, [r4, #0x2c]
004a279c: str      r5, [r4, #0x30]
004a27a0: str      r5, [r4, #0x40]
004a27a4: strb     r5, [r3, #0x3c]!
004a27a8: ldr      r1, [r4, #0x10]
004a27ac: ldr      r2, [r4]
004a27b0: str      r3, [r4, #0x48]
004a27b4: str      r5, [r4, #0x4c]
004a27b8: cmp      r1, r2
004a27bc: str      r3, [r4, #0x44]
004a27c0: beq      #0x4a27dc
004a27c4: mov      r0, r4
004a27c8: bl       #0x38fb18
004a27cc: ldr      r2, [r4, #0x10]
004a27d0: ldr      r3, [r4]
004a27d4: cmp      r2, r3
004a27d8: bne      #0x4a27c4
004a27dc: cmp      sb, #1
004a27e0: beq      #0x4a2808
004a27e4: cmp      sb, #2
004a27e8: beq      #0x4a2828
004a27ec: ldr      r3, [r6, r7]
004a27f0: mov      r0, r4
004a27f4: mov      r1, fp
004a27f8: str      r3, [r4, #0x28]
004a27fc: bl       #0x4a191c
004a2800: mov      r0, r4
004a2804: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2808: ldr      r3, [pc, #0x40]
004a280c: mov      r0, r4
004a2810: mov      r1, fp
004a2814: ldr      r3, [r6, r3]
004a2818: str      r3, [r4, #0x28]
004a281c: bl       #0x4a191c
004a2820: mov      r0, r4
004a2824: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2828: ldr      r3, [pc, #0x24]
004a282c: mov      r0, r4
004a2830: mov      r1, fp
004a2834: ldr      r3, [r6, r3]
004a2838: str      r3, [r4, #0x28]
004a283c: bl       #0x4a191c
004a2840: mov      r0, r4
004a2844: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2848: subeq    r2, pc, r0, asr r3
004a284c: andeq    r2, r0, r4, ror ip
004a2850: andeq    r4, r0, r4, asr #21
004a2854: ldrdeq   r1, r2, [r0], -r4
