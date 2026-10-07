
# _ZN6Random9GetRandomEib.clone.1
00401afc: push     {r4, lr}
00401b00: ldr      r4, [pc, #0x7c]
00401b04: cmp      r0, #0
00401b08: add      r4, pc, r4
00401b0c: beq      #0x401b6c
00401b10: ldr      r2, [pc, #0x70]
00401b14: mov      r1, r0
00401b18: movw     r0, #0xe6ab
00401b1c: ldr      r2, [r4, r2]
00401b20: movw     r3, #0xdb17
00401b24: movt     r3, #0x2b52
00401b28: ldr      lr, [r2]
00401b2c: movw     ip, #0xf26b
00401b30: movt     ip, #0xda
00401b34: mul      r0, r0, lr
00401b38: add      r0, r0, #0x2b000
00401b3c: add      r0, r0, #0x3fc
00401b40: add      r0, r0, #1
00401b44: umull    lr, r3, r3, r0
00401b48: rsb      lr, r3, r0
00401b4c: add      r3, r3, lr, lsr #1
00401b50: lsr      r3, r3, #0x17
00401b54: mls      r3, ip, r3, r0
00401b58: mov      r0, r3
00401b5c: str      r3, [r2]
00401b60: bl       #0x30eb2c
00401b64: eor      r0, r1, r1, asr #31
00401b68: sub      r0, r0, r1, asr #31
00401b6c: ldr      r3, [pc, #0x18]
00401b70: ldr      r3, [r4, r3]
00401b74: ldr      r2, [r3]
00401b78: add      r2, r2, #1
00401b7c: str      r2, [r3]
00401b80: pop      {r4, pc}
00401b84: subseq   r2, sb, r8, lsl #31
00401b88: muleq    r0, r4, ip
00401b8c: andeq    r1, r0, r8, lsl #1

# _ZNK9Character16GetCharSkillListEv
003bc5fc: ldr      r3, [pc, #0x20]
003bc600: ldr      r2, [pc, #0x20]
003bc604: push     {r4, lr}
003bc608: add      r3, pc, r3
003bc60c: ldr      r2, [r3, r2]
003bc610: ldr      r4, [r2]
003bc614: bl       #0x3bc5c0
003bc618: mov      r3, #0xc
003bc61c: mla      r0, r3, r0, r4
003bc620: pop      {r4, pc}
003bc624: subseq   r8, sp, r8, lsl #9
003bc628: andeq    r1, r0, r8, asr #3

# _ZN12ItemInstance6SetQtyEi
003fa0e4: push     {r4, r5, lr}
003fa0e8: ldr      r3, [pc, #0x74]
003fa0ec: subs     r5, r1, #0
003fa0f0: sub      sp, sp, #0xc
003fa0f4: mov      r4, r0
003fa0f8: add      r3, pc, r3
003fa0fc: blt      #0x3fa10c
003fa100: strh     r5, [r4, #0x50]
003fa104: add      sp, sp, #0xc
003fa108: pop      {r4, r5, pc}
003fa10c: ldr      r2, [pc, #0x54]
003fa110: ldr      r2, [r3, r2]
003fa114: ldr      r2, [r2]
003fa118: cmp      r2, #2
003fa11c: moveq    r3, #0
003fa120: streq    r3, [r3]
003fa124: beq      #0x3fa100
003fa128: cmp      r2, #1
003fa12c: bne      #0x3fa100
003fa130: ldr      r0, [pc, #0x34]
003fa134: ldr      r1, [pc, #0x34]
003fa138: ldr      r2, [pc, #0x34]
003fa13c: ldr      r0, [r3, r0]
003fa140: ldr      r3, [pc, #0x30]
003fa144: movw     ip, #0x223
003fa148: add      r1, pc, r1
003fa14c: add      r2, pc, r2
003fa150: add      r3, pc, r3
003fa154: add      r0, r0, #0xa8
003fa158: str      ip, [sp]
003fa15c: bl       #0x30e004
003fa160: b        #0x3fa100

# _ZNK9Character16IsSkillAvailableEi
003bca50: push     {r4, r5, r6, lr}
003bca54: mov      r5, r1
003bca58: mov      r6, r0
003bca5c: bl       #0x3bd120
003bca60: mov      r1, r5
003bca64: mov      r4, r0
003bca68: mov      r0, r6
003bca6c: bl       #0x3bc784
003bca70: ldr      r0, [r0, #0x20]
003bca74: cmp      r4, r0
003bca78: movlt    r0, #0
003bca7c: movge    r0, #1
003bca80: pop      {r4, r5, r6, pc}

# _ZNK13ItemInventory16IsItemEquippableEj
003fdeec: push     {r4, r5, lr}
003fdef0: mov      r4, r0
003fdef4: ldr      r2, [r0, #8]
003fdef8: ldr      r0, [r0, #0xc]
003fdefc: ldr      r3, [pc, #0x84]
003fdf00: sub      sp, sp, #0xc
003fdf04: rsb      r0, r2, r0
003fdf08: cmp      r1, r0, asr #2
003fdf0c: mov      r5, r1
003fdf10: add      r3, pc, r3
003fdf14: blo      #0x3fdf3c
003fdf18: ldr      r1, [pc, #0x6c]
003fdf1c: ldr      r1, [r3, r1]
003fdf20: ldr      r1, [r1]
003fdf24: cmp      r1, #2
003fdf28: moveq    r3, #0
003fdf2c: streq    r3, [r3]
003fdf30: beq      #0x3fdf3c
003fdf34: cmp      r1, #1
003fdf38: beq      #0x3fdf50
003fdf3c: ldr      r3, [r2, r5, lsl #2]
003fdf40: ldr      r0, [r3]
003fdf44: add      sp, sp, #0xc
003fdf48: pop      {r4, r5, lr}
003fdf4c: b        #0x3f9e68
003fdf50: ldr      r0, [pc, #0x38]
003fdf54: ldr      r1, [pc, #0x38]
003fdf58: ldr      r2, [pc, #0x38]
003fdf5c: ldr      r0, [r3, r0]
003fdf60: ldr      r3, [pc, #0x34]
003fdf64: add      r2, pc, r2
003fdf68: movw     ip, #0x271
003fdf6c: add      r1, pc, r1
003fdf70: add      r0, r0, #0xa8
003fdf74: add      r3, pc, r3
003fdf78: str      ip, [sp]
003fdf7c: bl       #0x30e004
003fdf80: ldr      r2, [r4, #8]
003fdf84: b        #0x3fdf3c
003fdf88: subseq   r6, sb, r0, lsl #23
003fdf8c: andeq    r3, r0, r0, asr #19
003fdf90: andeq    r1, r0, r0, asr #19
003fdf94: subeq    r0, ip, ip, ror #8
003fdf98: subeq    sb, ip, r4, lsr #8
003fdf9c: subeq    sb, ip, r4, lsr r4

# _ZN13ItemInventory18_AddLootItemPowersEPKN7Structs9LootEntryEP12ItemInstanceiii
00403310: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00403314: ldr      r6, [pc, #0x62c]
00403318: ldr      ip, [pc, #0x62c]
0040331c: sub      sp, sp, #0xb4
00403320: add      r6, pc, r6
00403324: str      ip, [sp, #0x20]
00403328: ldr      ip, [r6, ip]
0040332c: mov      r4, r0
00403330: ldr      r0, [r0, #8]
00403334: str      r1, [sp, #0xc]
00403338: ldr      r1, [ip]
0040333c: cmn      r0, #1
00403340: mov      r7, r2
00403344: str      r1, [sp, #0xac]
00403348: mov      r5, r3
0040334c: beq      #0x403854
00403350: cmp      r0, #0
00403354: blt      #0x403610
00403358: ldr      r3, [pc, #0x5f0]
0040335c: ldr      r3, [r6, r3]
00403360: ldr      r3, [r3]
00403364: cmp      r0, r3
00403368: bge      #0x403610
0040336c: cmn      r5, #1
00403370: strne    r5, [sp, #0x18]
00403374: beq      #0x403930
00403378: ldr      r3, [pc, #0x5d4]
0040337c: ldr      r0, [pc, #0x5d4]
00403380: ldr      r8, [pc, #0x5d4]
00403384: ldr      r3, [r6, r3]
00403388: ldr      r5, [r6, r0]
0040338c: str      r0, [sp, #0x14]
00403390: ldr      r3, [r3]
00403394: ldr      r2, [r4, #8]
00403398: add      r8, pc, r8
0040339c: add      r4, sp, #0x94
004033a0: mov      r0, r5
004033a4: mov      r7, #0xc
004033a8: mla      r7, r7, r2, r3
004033ac: bl       #0x337888
004033b0: add      r2, sp, #0x60
004033b4: mov      r0, r4
004033b8: mov      r1, r8
004033bc: bl       #0x3140ec
004033c0: mov      r1, r4
004033c4: mov      r0, r5
004033c8: bl       #0x337a88
004033cc: mov      r0, r4
004033d0: bl       #0x318254
004033d4: add      r4, sp, #0x7c
004033d8: mov      r0, r5
004033dc: bl       #0x337888
004033e0: add      r2, sp, #0x5c
004033e4: mov      r1, r8
004033e8: mov      r0, r4
004033ec: bl       #0x3140ec
004033f0: mov      r1, r4
004033f4: mov      r0, r5
004033f8: bl       #0x337a88
004033fc: mov      r0, r4
00403400: bl       #0x318254
00403404: ldr      r3, [r7, #4]
00403408: ldr      r1, [sp, #0x18]
0040340c: cmp      r1, r3
00403410: bhs      #0x403874
00403414: ldr      r3, [pc, #0x544]
00403418: mov      fp, #0
0040341c: add      r4, sp, #0xb0
00403420: add      r3, pc, r3
00403424: str      r3, [sp, #0x24]
00403428: ldr      r3, [pc, #0x534]
0040342c: strb     fp, [r4, #-0x80]!
00403430: str      fp, [sp, #0x34]
00403434: add      r3, pc, r3
00403438: str      r3, [sp, #0x28]
0040343c: ldr      r3, [pc, #0x524]
00403440: str      r4, [sp, #0x38]
00403444: str      r4, [sp, #0x3c]
00403448: add      r3, pc, r3
0040344c: str      r3, [sp, #0x2c]
00403450: ldr      r3, [pc, #0x514]
00403454: str      fp, [sp, #0x40]
00403458: add      r3, pc, r3
0040345c: str      r3, [sp, #0x1c]
00403460: ldr      r3, [pc, #0x508]
00403464: str      r3, [sp, #0x10]
00403468: ldr      r0, [sp, #0xc]
0040346c: bl       #0x3f9e80
00403470: ldr      r2, [sp, #0x18]
00403474: cmp      r2, r0
00403478: bls      #0x4036ac
0040347c: mov      r0, r7
00403480: bl       #0x401f38
00403484: ldr      r3, [r7, #8]
00403488: mov      r8, #0xc
0040348c: mla      r8, r8, r0, r3
00403490: ldr      r3, [r8, #4]
00403494: cmp      r3, #0
00403498: blt      #0x4034b0
0040349c: ldr      r2, [pc, #0x4d0]
004034a0: ldr      r2, [r6, r2]
004034a4: ldr      r2, [r2]
004034a8: cmp      r3, r2
004034ac: blt      #0x4034d4
004034b0: ldr      ip, [sp, #0x10]
004034b4: ldr      r2, [r6, ip]
004034b8: ldr      r2, [r2]
004034bc: cmp      r2, #2
004034c0: moveq    r2, #0
004034c4: streq    r2, [r2]
004034c8: beq      #0x4034d4
004034cc: cmp      r2, #1
004034d0: beq      #0x403904
004034d4: ldr      r2, [pc, #0x49c]
004034d8: mov      r5, #0x28
004034dc: ldr      r2, [r6, r2]
004034e0: ldr      r2, [r2]
004034e4: mla      r5, r5, r3, r2
004034e8: ldr      r3, [r5, #0x24]
004034ec: cmp      r3, #0
004034f0: blt      #0x403508
004034f4: ldr      r2, [pc, #0x480]
004034f8: ldr      r2, [r6, r2]
004034fc: ldr      r2, [r2]
00403500: cmp      r3, r2
00403504: blt      #0x40352c
00403508: ldr      r0, [sp, #0x10]
0040350c: ldr      r2, [r6, r0]
00403510: ldr      r2, [r2]
00403514: cmp      r2, #2
00403518: moveq    r2, #0
0040351c: streq    r2, [r2]
00403520: beq      #0x40352c
00403524: cmp      r2, #1
00403528: beq      #0x4038cc
0040352c: ldr      r2, [pc, #0x44c]
00403530: mov      r1, #0xc
00403534: ldr      r2, [r6, r2]
00403538: ldr      r2, [r2]
0040353c: mla      r3, r1, r3, r2
00403540: ldr      sl, [r3, #4]
00403544: cmp      sl, #0
00403548: beq      #0x4035bc
0040354c: ldr      sb, [r3, #8]
00403550: ldr      lr, [sp, #0x34]
00403554: mov      ip, #0
00403558: cmp      lr, #0
0040355c: beq      #0x4038bc
00403560: ldr      r0, [sb, ip, lsl #2]
00403564: mov      r3, lr
00403568: mov      r1, r4
0040356c: b        #0x403578
00403570: mov      r1, r3
00403574: mov      r3, r2
00403578: ldr      r2, [r3, #0x10]
0040357c: cmp      r2, r0
00403580: ldrlt    r2, [r3, #0xc]
00403584: ldrge    r2, [r3, #8]
00403588: movlt    r3, r1
0040358c: cmp      r2, #0
00403590: bne      #0x403570
00403594: cmp      r3, r4
00403598: beq      #0x4038bc
0040359c: ldr      r2, [r3, #0x10]
004035a0: cmp      r2, r0
004035a4: movgt    r3, r4
004035a8: cmp      r3, r4
004035ac: bne      #0x403668
004035b0: add      ip, ip, #1
004035b4: cmp      ip, sl
004035b8: bne      #0x403558
004035bc: ldr      r1, [r8, #4]
004035c0: ldr      r0, [sp, #0xc]
004035c4: ldr      r2, [sp, #0xd8]
004035c8: bl       #0x3fbc60
004035cc: ldr      r3, [r5, #0xc]
004035d0: cmp      r3, #0
004035d4: movne    r8, #0
004035d8: addne    sl, sp, #0x50
004035dc: beq      #0x403608
004035e0: ldr      r2, [r5, #0x10]
004035e4: mov      r0, sl
004035e8: mov      r1, r4
004035ec: add      r2, r2, r8, lsl #4
004035f0: add      r2, r2, #4
004035f4: bl       #0x4022c4
004035f8: ldr      r3, [r5, #0xc]
004035fc: add      r8, r8, #1
00403600: cmp      r3, r8
00403604: bhi      #0x4035e0
00403608: mov      fp, #0
0040360c: b        #0x403468
00403610: ldr      r3, [pc, #0x358]
00403614: ldr      r3, [r6, r3]
00403618: ldr      r3, [r3]
0040361c: cmp      r3, #2
00403620: moveq    r3, #0
00403624: streq    r3, [r3]
00403628: beq      #0x40336c
0040362c: cmp      r3, #1
00403630: bne      #0x40336c
00403634: ldr      r0, [pc, #0x348]
00403638: ldr      r1, [pc, #0x348]
0040363c: ldr      r2, [pc, #0x348]
00403640: ldr      r0, [r6, r0]
00403644: ldr      r3, [pc, #0x344]
00403648: mov      ip, #0x198
0040364c: add      r1, pc, r1
00403650: add      r2, pc, r2
00403654: add      r3, pc, r3
00403658: add      r0, r0, #0xa8
0040365c: str      ip, [sp]
00403660: bl       #0x30e004
00403664: b        #0x40336c
00403668: ldr      r1, [sp, #0x14]
0040366c: add      r5, sp, #0x64
00403670: add      fp, fp, #1
00403674: ldr      r8, [r6, r1]
00403678: mov      r0, r8
0040367c: bl       #0x337888
00403680: add      r2, sp, #0x58
00403684: ldr      r1, [sp, #0x1c]
00403688: mov      r0, r5
0040368c: bl       #0x3140ec
00403690: mov      r1, r5
00403694: mov      r0, r8
00403698: bl       #0x337a88
0040369c: mov      r0, r5
004036a0: bl       #0x318254
004036a4: cmp      fp, #9
004036a8: bls      #0x403468
004036ac: ldr      r0, [sp, #0xc]
004036b0: bl       #0x3f9e80
004036b4: ldr      r3, [sp, #0x18]
004036b8: cmp      r3, r0
004036bc: bls      #0x403834
004036c0: ldr      ip, [r7, #4]
004036c4: cmp      ip, #0
004036c8: str      ip, [sp, #0x10]
004036cc: beq      #0x403834
004036d0: ldr      r2, [pc, #0x2a0]
004036d4: ldr      r3, [pc, #0x2a4]
004036d8: mov      sl, #0
004036dc: ldr      r2, [r6, r2]
004036e0: ldr      r3, [r6, r3]
004036e4: add      r0, sp, #0x48
004036e8: str      r2, [sp, #0x14]
004036ec: str      r3, [sp, #0x1c]
004036f0: mov      r8, sl
004036f4: str      r0, [sp, #0x24]
004036f8: str      r6, [sp, #0x28]
004036fc: ldr      r2, [r7, #8]
00403700: ldr      r1, [sp, #0x14]
00403704: mov      r0, #0x28
00403708: add      r2, r2, sl
0040370c: ldr      r6, [r1]
00403710: ldr      r1, [r2, #4]
00403714: ldr      ip, [sp, #0x1c]
00403718: mla      r6, r0, r1, r6
0040371c: ldr      r3, [ip]
00403720: ldr      r2, [r6, #0x24]
00403724: mov      ip, #0xc
00403728: mla      r3, ip, r2, r3
0040372c: ldr      sb, [r3, #4]
00403730: cmp      sb, #0
00403734: beq      #0x4037a8
00403738: ldr      fp, [r3, #8]
0040373c: ldr      r5, [sp, #0x34]
00403740: mov      lr, #0
00403744: cmp      r5, #0
00403748: beq      #0x4038c4
0040374c: ldr      ip, [fp, lr, lsl #2]
00403750: mov      r3, r5
00403754: mov      r0, r4
00403758: b        #0x403764
0040375c: mov      r0, r3
00403760: mov      r3, r2
00403764: ldr      r2, [r3, #0x10]
00403768: cmp      ip, r2
0040376c: ldrgt    r2, [r3, #0xc]
00403770: ldrle    r2, [r3, #8]
00403774: movgt    r3, r0
00403778: cmp      r2, #0
0040377c: bne      #0x40375c
00403780: cmp      r3, r4
00403784: beq      #0x4038c4
00403788: ldr      r2, [r3, #0x10]
0040378c: cmp      ip, r2
00403790: movlt    r3, r4
00403794: cmp      r3, r4
00403798: bne      #0x40381c
0040379c: add      lr, lr, #1
004037a0: cmp      lr, sb
004037a4: bne      #0x403744
004037a8: ldr      r2, [sp, #0xd8]
004037ac: ldr      r0, [sp, #0xc]
004037b0: bl       #0x3fbc60
004037b4: ldr      r0, [sp, #0xc]
004037b8: bl       #0x3f9e80
004037bc: ldr      r3, [sp, #0x18]
004037c0: cmp      r3, r0
004037c4: bls      #0x403830
004037c8: ldr      r3, [r6, #0xc]
004037cc: cmp      r3, #0
004037d0: ldreq    r0, [r7, #4]
004037d4: streq    r0, [sp, #0x10]
004037d8: beq      #0x40381c
004037dc: mov      sb, sl
004037e0: ldr      sl, [sp, #0x24]
004037e4: mov      r5, #0
004037e8: ldr      r2, [r6, #0x10]
004037ec: mov      r0, sl
004037f0: mov      r1, r4
004037f4: add      r2, r2, r5, lsl #4
004037f8: add      r2, r2, #4
004037fc: bl       #0x4022c4
00403800: ldr      r3, [r6, #0xc]
00403804: add      r5, r5, #1
00403808: cmp      r3, r5
0040380c: bhi      #0x4037e8
00403810: ldr      r1, [r7, #4]
00403814: mov      sl, sb
00403818: str      r1, [sp, #0x10]
0040381c: ldr      r2, [sp, #0x10]
00403820: add      r8, r8, #1
00403824: add      sl, sl, #0xc
00403828: cmp      r2, r8
0040382c: bhi      #0x4036fc
00403830: ldr      r6, [sp, #0x28]
00403834: ldr      r0, [sp, #0xc]
00403838: bl       #0x3f9e80
0040383c: ldr      r3, [sp, #0x40]
00403840: cmp      r3, #0
00403844: beq      #0x403854
00403848: mov      r0, r4
0040384c: ldr      r1, [sp, #0x34]
00403850: bl       #0x402444
00403854: ldr      ip, [sp, #0x20]
00403858: ldr      r2, [sp, #0xac]
0040385c: ldr      r3, [r6, ip]
00403860: ldr      r3, [r3]
00403864: cmp      r2, r3
00403868: bne      #0x403944
0040386c: add      sp, sp, #0xb4
00403870: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00403874: cmp      r3, #0
00403878: beq      #0x403854
0040387c: ldr      r8, [sp, #0xc]
00403880: ldr      sl, [sp, #0xd8]
00403884: mov      r4, #0
00403888: mov      r5, r4
0040388c: ldr      r3, [r7, #8]
00403890: mov      r0, r8
00403894: mov      r2, sl
00403898: add      r3, r3, r4
0040389c: ldr      r1, [r3, #4]
004038a0: bl       #0x3fbc60
004038a4: ldr      r3, [r7, #4]
004038a8: add      r5, r5, #1
004038ac: add      r4, r4, #0xc
004038b0: cmp      r3, r5
004038b4: bhi      #0x40388c
004038b8: b        #0x403854
004038bc: mov      r3, r4
004038c0: b        #0x4035a8
004038c4: mov      r3, r4
004038c8: b        #0x403794
004038cc: ldr      r0, [pc, #0xb0]
004038d0: ldr      r1, [pc, #0xbc]
004038d4: ldr      r2, [pc, #0xbc]
004038d8: ldr      r0, [r6, r0]
004038dc: ldr      r3, [pc, #0xb8]
004038e0: movw     ip, #0x1be
004038e4: add      r1, pc, r1
004038e8: add      r3, pc, r3
004038ec: add      r0, r0, #0xa8
004038f0: add      r2, pc, r2
004038f4: str      ip, [sp]
004038f8: bl       #0x30e004
004038fc: ldr      r3, [r5, #0x24]
00403900: b        #0x40352c
00403904: ldr      r0, [pc, #0x78]
00403908: ldr      r3, [sp, #0x2c]
0040390c: movw     ip, #0x1bb
00403910: ldr      r0, [r6, r0]
00403914: ldr      r1, [sp, #0x24]
00403918: ldr      r2, [sp, #0x28]
0040391c: add      r0, r0, #0xa8
00403920: str      ip, [sp]
00403924: bl       #0x30e004
00403928: ldr      r3, [r8, #4]
0040392c: b        #0x4034d4
00403930: asr      r1, r7, #8
00403934: ldr      r0, [r4, #0x10]
00403938: bl       #0x401b90
0040393c: str      r0, [sp, #0x18]
00403940: b        #0x403378
00403944: bl       #0x30e310
00403948: subseq   r1, sb, r0, ror r7
0040394c: andeq    r4, r0, ip, lsr #1
00403950: andeq    r0, r0, r8, asr #22
00403954: andeq    r0, r0, r8, lsr #15
00403958: andeq    r0, r0, r4, lsl #17
0040395c: umaaleq  r4, ip, r0, r4
00403960: strheq   sl, [fp], #-0xf8
00403964: subeq    r4, ip, r4, ror r4
00403968: subeq    r4, ip, r8, lsr r1
0040396c: ldrdeq   r4, r5, [ip], #-0x30
00403970: andeq    r3, r0, r0, asr #19
00403974: andeq    r1, r0, r8, lsl #3
00403978: andeq    r3, r0, r8, ror #23
0040397c: andeq    r1, r0, ip, lsl ip
00403980: andeq    r3, r0, r8, lsr #32
00403984: andeq    r1, r0, r0, asr #19
00403988: subeq    sl, fp, ip, lsl #27
0040398c: strdeq   r4, r5, [ip], #-0x18
00403990: subeq    r3, ip, ip, lsr #30
00403994: strdeq   sl, fp, [fp], #-0xa4
00403998: subeq    r4, ip, r8
0040399c: umaaleq  r3, ip, r8, ip

# _ZN9Character13EquipItemAutoEj
003a9fa8: push     {r4, r5, r6, lr}
003a9fac: mov      r4, r0
003a9fb0: add      r0, r0, #0x37c
003a9fb4: bl       #0x400c84
003a9fb8: mov      r5, r0
003a9fbc: add      r0, r4, #0x560
003a9fc0: bl       #0x3e08a8
003a9fc4: mov      r0, r4
003a9fc8: bl       #0x3a999c
003a9fcc: mov      r0, r4
003a9fd0: bl       #0x3bd140
003a9fd4: mov      r0, r5
003a9fd8: pop      {r4, r5, r6, pc}

# _ZN12ItemInstanceC1Eij
003fc26c: push     {r4, r5, r6, r7, r8, lr}
003fc270: ldr      r5, [pc, #0x148]
003fc274: ldr      r3, [pc, #0x148]
003fc278: mov      r4, r0
003fc27c: add      r5, pc, r5
003fc280: ldr      r3, [r5, r3]
003fc284: mov      r6, r1
003fc288: add      r1, r0, #8
003fc28c: add      r3, r3, #8
003fc290: str      r3, [r0]
003fc294: sub      sp, sp, #8
003fc298: mov      r0, r1
003fc29c: str      r1, [r4, #0x18]
003fc2a0: str      r1, [r4, #0x1c]
003fc2a4: str      r6, [r4, #4]
003fc2a8: mov      r1, #0x10
003fc2ac: mov      r8, r2
003fc2b0: bl       #0x31167c
003fc2b4: ldr      r2, [r4, #0x18]
003fc2b8: mov      r7, #0
003fc2bc: add      r3, r4, #0x20
003fc2c0: strb     r7, [r2]
003fc2c4: mov      r0, r3
003fc2c8: str      r3, [r4, #0x30]
003fc2cc: str      r3, [r4, #0x34]
003fc2d0: mov      r1, #0x10
003fc2d4: bl       #0x31167c
003fc2d8: ldr      r2, [r4, #0x30]
003fc2dc: add      r3, r4, #0x38
003fc2e0: mov      r0, r3
003fc2e4: strb     r7, [r2]
003fc2e8: mov      r1, #0x10
003fc2ec: str      r3, [r4, #0x48]
003fc2f0: str      r3, [r4, #0x4c]
003fc2f4: bl       #0x31167c
003fc2f8: ldr      r3, [r4, #0x48]
003fc2fc: cmp      r6, r7
003fc300: strb     r7, [r3]
003fc304: mov      r3, #1
003fc308: strb     r3, [r4, #0x68]
003fc30c: mvn      r3, #0
003fc310: strh     r8, [r4, #0x50]
003fc314: strb     r7, [r4, #0x69]
003fc318: str      r7, [r4, #0x54]
003fc31c: strh     r3, [r4, #0x58]
003fc320: str      r7, [r4, #0x5c]
003fc324: str      r7, [r4, #0x60]
003fc328: str      r7, [r4, #0x64]
003fc32c: blt      #0x3fc344
003fc330: ldr      r3, [pc, #0x90]
003fc334: ldr      r3, [r5, r3]
003fc338: ldr      r3, [r3]
003fc33c: cmp      r3, r7
003fc340: bne      #0x3fc368
003fc344: ldr      r3, [pc, #0x80]
003fc348: ldr      r3, [r5, r3]
003fc34c: ldr      r3, [r3]
003fc350: cmp      r3, #2
003fc354: moveq    r3, #0
003fc358: streq    r3, [r3]
003fc35c: beq      #0x3fc368
003fc360: cmp      r3, #1
003fc364: beq      #0x3fc38c
003fc368: mov      r0, r4
003fc36c: bl       #0x3fb754
003fc370: mov      r0, r4
003fc374: bl       #0x3fb290
003fc378: mov      r0, r4
003fc37c: bl       #0x3facdc
003fc380: mov      r0, r4
003fc384: add      sp, sp, #8
003fc388: pop      {r4, r5, r6, r7, r8, pc}
003fc38c: ldr      r0, [pc, #0x3c]
003fc390: ldr      r1, [pc, #0x3c]
003fc394: ldr      r2, [pc, #0x3c]
003fc398: ldr      r0, [r5, r0]
003fc39c: ldr      r3, [pc, #0x38]
003fc3a0: mov      ip, #0x54
003fc3a4: add      r1, pc, r1
003fc3a8: add      r2, pc, r2
003fc3ac: add      r3, pc, r3
003fc3b0: add      r0, r0, #0xa8
003fc3b4: str      ip, [sp]
003fc3b8: bl       #0x30e004
003fc3bc: b        #0x3fc368
003fc3c0: subseq   r8, sb, r4, lsl r8
003fc3c4: andeq    r4, r0, r0, lsl #16
003fc3c8: andeq    r0, r0, r0, ror #26
003fc3cc: andeq    r3, r0, r0, asr #19
003fc3d0: andeq    r1, r0, r0, asr #19
003fc3d4: subeq    r2, ip, r4, lsr r0
003fc3d8: umaaleq  sl, ip, r0, lr
003fc3dc: umaaleq  sl, ip, ip, fp

# _ZN13ItemInventory14_GetRandomItemERKN7Structs17ItemListEntryListE
00401dbc: push     {r4, r5, lr}
00401dc0: ldr      ip, [r0, #4]
00401dc4: ldr      r4, [pc, #0x148]
00401dc8: sub      sp, sp, #0xc
00401dcc: cmp      ip, #0
00401dd0: mov      r5, r0
00401dd4: add      r4, pc, r4
00401dd8: beq      #0x401e08
00401ddc: mov      r3, #0
00401de0: ldr      r2, [r0, #8]
00401de4: mov      r0, r3
00401de8: ldrsh    r1, [r2, #8]
00401dec: add      r3, r3, #1
00401df0: cmp      r3, ip
00401df4: add      r0, r0, r1
00401df8: add      r2, r2, #0xc
00401dfc: bne      #0x401de8
00401e00: cmp      r0, #0
00401e04: bne      #0x401e30
00401e08: ldr      r3, [pc, #0x108]
00401e0c: ldr      r3, [r4, r3]
00401e10: ldr      r3, [r3]
00401e14: cmp      r3, #2
00401e18: beq      #0x401f08
00401e1c: cmp      r3, #1
00401e20: beq      #0x401ed0
00401e24: mov      r0, #0
00401e28: add      sp, sp, #0xc
00401e2c: pop      {r4, r5, pc}
00401e30: bl       #0x401afc
00401e34: ldr      ip, [r5, #4]
00401e38: cmp      ip, #0
00401e3c: beq      #0x401e7c
00401e40: ldr      r1, [r5, #8]
00401e44: mov      r3, r0
00401e48: ldrsh    r2, [r1, #8]
00401e4c: cmp      r0, r2
00401e50: movhs    r0, #0
00401e54: bhs      #0x401e6c
00401e58: b        #0x401e24
00401e5c: ldrsh    r2, [r1, #0x14]
00401e60: add      r1, r1, #0xc
00401e64: cmp      r2, r3
00401e68: bhi      #0x401e28
00401e6c: add      r0, r0, #1
00401e70: cmp      r0, ip
00401e74: rsb      r3, r2, r3
00401e78: bne      #0x401e5c
00401e7c: ldr      r3, [pc, #0x94]
00401e80: ldr      r3, [r4, r3]
00401e84: ldr      r3, [r3]
00401e88: cmp      r3, #2
00401e8c: beq      #0x401f08
00401e90: cmp      r3, #1
00401e94: bne      #0x401e24
00401e98: ldr      r0, [pc, #0x7c]
00401e9c: ldr      r1, [pc, #0x7c]
00401ea0: ldr      r2, [pc, #0x7c]
00401ea4: ldr      r0, [r4, r0]
00401ea8: ldr      r3, [pc, #0x78]
00401eac: mov      ip, #0x18c
00401eb0: add      r1, pc, r1
00401eb4: add      r0, r0, #0xa8
00401eb8: add      r2, pc, r2
00401ebc: add      r3, pc, r3
00401ec0: str      ip, [sp]
00401ec4: bl       #0x30e004
00401ec8: mov      r0, #0
00401ecc: b        #0x401e28
00401ed0: ldr      r0, [pc, #0x44]
00401ed4: ldr      r1, [pc, #0x50]
00401ed8: ldr      r2, [pc, #0x50]
00401edc: ldr      r0, [r4, r0]
00401ee0: ldr      r3, [pc, #0x4c]
00401ee4: movw     ip, #0x17e
00401ee8: add      r1, pc, r1
00401eec: add      r0, r0, #0xa8
00401ef0: add      r2, pc, r2
00401ef4: add      r3, pc, r3
00401ef8: str      ip, [sp]
00401efc: bl       #0x30e004
00401f00: mov      r0, #0
00401f04: b        #0x401e28
00401f08: mov      r0, #0
00401f0c: str      r0, [r0]
00401f10: b        #0x401e28
00401f14: ldrheq   r2, [sb], #-0xcc
00401f18: andeq    r3, r0, r0, asr #19
00401f1c: andeq    r1, r0, r0, asr #19
00401f20: subeq    ip, fp, r8, lsr #10
00401f24: strheq   r5, [ip], #-0x70
00401f28: subeq    r5, ip, r4, asr #13
00401f2c: strdeq   ip, sp, [fp], #-0x40
00401f30: subeq    r5, ip, r8, asr #14
00401f34: subeq    r5, ip, ip, lsl #13

# _ZN13ItemInventory16_AddItemInstanceEP12ItemInstancebb
003ff5d4: push     {r4, r5, r6, r7, r8, sl, lr}
003ff5d8: mov      r4, r0
003ff5dc: ldrsb    r0, [r0, #0x2c]
003ff5e0: ldr      r6, [pc, #0x260]
003ff5e4: sub      sp, sp, #0xc
003ff5e8: cmp      r0, #0
003ff5ec: add      r6, pc, r6
003ff5f0: mov      r5, r1
003ff5f4: mov      r8, r2
003ff5f8: mov      r7, r3
003ff5fc: beq      #0x3ff6f0
003ff600: ldr      r3, [r4, #0x24]
003ff604: cmp      r3, #0
003ff608: beq      #0x3ff81c
003ff60c: cmp      r7, #0
003ff610: bne      #0x3ff724
003ff614: mov      r0, r5
003ff618: bl       #0x3f9e58
003ff61c: cmp      r0, #0
003ff620: beq      #0x3ff674
003ff624: cmp      r8, #0
003ff628: bne      #0x3ff674
003ff62c: ldr      r7, [r4, #8]
003ff630: ldr      r2, [r4, #0xc]
003ff634: cmp      r7, r2
003ff638: beq      #0x3ff674
003ff63c: ldr      sl, [r7]
003ff640: mov      r1, r8
003ff644: mov      r0, r4
003ff648: ldr      r3, [sl]
003ff64c: cmp      r3, #0
003ff650: beq      #0x3ff664
003ff654: bl       #0x3fdaf0
003ff658: cmp      r0, #0
003ff65c: beq      #0x3ff75c
003ff660: ldr      r2, [r4, #0xc]
003ff664: add      r7, r7, #4
003ff668: cmp      r7, r2
003ff66c: addne    r8, r8, #1
003ff670: bne      #0x3ff63c
003ff674: mov      r1, #0
003ff678: mov      r0, #8
003ff67c: bl       #0x310570
003ff680: mvn      r3, #0
003ff684: str      r5, [r0]
003ff688: strb     r3, [r0, #5]
003ff68c: strb     r3, [r0, #4]
003ff690: ldr      r1, [r4, #0xc]
003ff694: ldr      r3, [r4, #0x10]
003ff698: str      r0, [sp, #4]
003ff69c: cmp      r1, r3
003ff6a0: beq      #0x3ff838
003ff6a4: str      r0, [r1]
003ff6a8: ldr      r3, [r4, #0xc]
003ff6ac: add      r3, r3, #4
003ff6b0: str      r3, [r4, #0xc]
003ff6b4: ldr      r3, [pc, #0x190]
003ff6b8: mov      r0, r4
003ff6bc: ldr      r3, [r6, r3]
003ff6c0: ldr      r5, [r3]
003ff6c4: bl       #0x3fe330
003ff6c8: cmp      r0, #0
003ff6cc: bne      #0x3ff7a8
003ff6d0: ldr      r3, [r4, #8]
003ff6d4: ldr      r8, [r4, #0xc]
003ff6d8: rsb      r8, r3, r8
003ff6dc: asr      r8, r8, #2
003ff6e0: sub      r8, r8, #1
003ff6e4: mov      r0, r8
003ff6e8: add      sp, sp, #0xc
003ff6ec: pop      {r4, r5, r6, r7, r8, sl, pc}
003ff6f0: mov      r0, r1
003ff6f4: bl       #0x3f9e08
003ff6f8: ldr      r3, [r0, #0x58]
003ff6fc: cmp      r3, #0xe
003ff700: bne      #0x3ff600
003ff704: cmp      r5, #0
003ff708: beq      #0x3ff754
003ff70c: mov      r0, r5
003ff710: ldr      r3, [r5]
003ff714: mov      lr, pc
003ff718: ldr      pc, [r3, #4]
003ff71c: mvn      r8, #0
003ff720: b        #0x3ff6e4
003ff724: mov      r0, r5
003ff728: bl       #0x3f9e08
003ff72c: ldr      r3, [r0, #0x58]
003ff730: cmp      r3, #0xd
003ff734: bne      #0x3ff614
003ff738: mov      r0, r4
003ff73c: ldr      r1, [r5, #0x54]
003ff740: bl       #0x3fe164
003ff744: mov      r0, r5
003ff748: ldr      r3, [r5]
003ff74c: mov      lr, pc
003ff750: ldr      pc, [r3, #4]
003ff754: mvn      r8, #0
003ff758: b        #0x3ff6e4
003ff75c: ldr      r0, [sl]
003ff760: mov      r1, r5
003ff764: bl       #0x3f9d78
003ff768: cmp      r0, #0
003ff76c: beq      #0x3ff660
003ff770: mov      r0, r5
003ff774: ldrsh    r6, [r5, #0x50]
003ff778: bl       #0x3f9e08
003ff77c: ldr      r3, [r0, #0x58]
003ff780: cmp      r3, #0xe
003ff784: beq      #0x3ff7fc
003ff788: ldr      r0, [sl]
003ff78c: mov      r1, r6
003ff790: bl       #0x3fa17c
003ff794: mov      r0, r5
003ff798: ldr      r3, [r5]
003ff79c: mov      lr, pc
003ff7a0: ldr      pc, [r3, #4]
003ff7a4: b        #0x3ff6e4
003ff7a8: ldr      r3, [r4, #4]
003ff7ac: mov      r0, r3
003ff7b0: ldr      r3, [r3]
003ff7b4: mov      lr, pc
003ff7b8: ldr      pc, [r3, #0x28]
003ff7bc: cmp      r0, #0
003ff7c0: beq      #0x3ff6d0
003ff7c4: ldr      r3, [pc, #0x84]
003ff7c8: ldr      r1, [r4, #4]
003ff7cc: ldr      r3, [r6, r3]
003ff7d0: ldr      r0, [r3, #0x40]
003ff7d4: bl       #0x36effc
003ff7d8: cmp      r0, #0
003ff7dc: beq      #0x3ff6d0
003ff7e0: ldr      r0, [pc, #0x6c]
003ff7e4: add      r0, pc, r0
003ff7e8: bl       #0x3a3f70
003ff7ec: mov      r1, r0
003ff7f0: mov      r0, r5
003ff7f4: bl       #0x3813b8
003ff7f8: b        #0x3ff6d0
003ff7fc: ldr      r0, [sl]
003ff800: ldrsb    r1, [r4, #0x2c]
003ff804: ldrsh    r3, [r0, #0x50]
003ff808: rsb      r1, r3, r1
003ff80c: cmp      r1, r6
003ff810: movge    r1, r6
003ff814: biclt    r1, r1, r1, asr #31
003ff818: b        #0x3ff790
003ff81c: mov      r0, r5
003ff820: bl       #0x3f9e08
003ff824: ldr      r3, [r0, #0x58]
003ff828: cmp      r3, #0xe
003ff82c: streq    r5, [r4, #0x24]
003ff830: bne      #0x3ff60c
003ff834: b        #0x3ff614
003ff838: add      r0, r4, #8
003ff83c: add      r2, sp, #4
003ff840: bl       #0x3fef4c
003ff844: b        #0x3ff6b4
003ff848: subseq   r5, sb, r4, lsr #9
003ff84c: andeq    r1, r0, r0, ror sp
003ff850: strdeq   r3, r4, [r0], -r4
003ff854: subeq    r7, ip, ip, ror ip
