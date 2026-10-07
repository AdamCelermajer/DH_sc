
# _ZN13DebugSwitches4loadEv
00337888: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0033788c: ldr      r4, [pc, #0x1c8]
00337890: ldr      r3, [pc, #0x1c8]
00337894: ldr      r6, [pc, #0x1c8]
00337898: add      r4, pc, r4
0033789c: ldr      r3, [r4, r3]
003378a0: ldr      r1, [r4, r6]
003378a4: sub      sp, sp, #0x98
003378a8: ldrb     r2, [r3]
003378ac: ldr      r1, [r1]
003378b0: mov      r7, r0
003378b4: cmp      r2, #0
003378b8: str      r1, [sp, #0x94]
003378bc: bne      #0x337a3c
003378c0: mov      r1, #1
003378c4: strb     r1, [r3]
003378c8: ldr      r3, [pc, #0x198]
003378cc: ldr      r3, [r4, r3]
003378d0: ldr      r3, [r3, #0x10]
003378d4: ldr      r5, [r3, #0x34]
003378d8: cmp      r5, #0
003378dc: beq      #0x337924
003378e0: ldr      r1, [pc, #0x184]
003378e4: ldr      r3, [r5]
003378e8: mov      r0, r5
003378ec: add      r1, pc, r1
003378f0: mov      lr, pc
003378f4: ldr      pc, [r3, #0x94]
003378f8: cmp      r0, #0
003378fc: mov      r1, r0
00337900: str      r0, [sp, #4]
00337904: beq      #0x337924
00337908: mov      r0, r7
0033790c: bl       #0x3374e4
00337910: mov      r0, r5
00337914: ldr      r3, [r5]
00337918: add      r1, sp, #4
0033791c: mov      lr, pc
00337920: ldr      pc, [r3, #0x78]
00337924: ldr      r3, [pc, #0x144]
00337928: add      sl, sp, #0x7c
0033792c: add      r8, sp, #0x64
00337930: ldr      r5, [r4, r3]
00337934: add      r7, sp, #0x4c
00337938: add      sb, sp, #0x34
0033793c: mov      r0, r5
00337940: bl       #0x337888
00337944: ldr      r1, [pc, #0x128]
00337948: add      r2, sp, #0x18
0033794c: mov      r0, sl
00337950: add      r1, pc, r1
00337954: bl       #0x3140ec
00337958: mov      r1, sl
0033795c: mov      r2, #0
00337960: mov      r0, r5
00337964: bl       #0x337ddc
00337968: mov      r0, sl
0033796c: bl       #0x318254
00337970: mov      r0, r5
00337974: bl       #0x337888
00337978: ldr      r1, [pc, #0xf8]
0033797c: add      r2, sp, #0x14
00337980: mov      r0, r8
00337984: add      r1, pc, r1
00337988: bl       #0x3140ec
0033798c: mov      r1, r8
00337990: mov      r2, #0
00337994: mov      r0, r5
00337998: bl       #0x337ddc
0033799c: mov      r0, r8
003379a0: bl       #0x318254
003379a4: mov      r0, r5
003379a8: bl       #0x337888
003379ac: ldr      r1, [pc, #0xc8]
003379b0: add      r2, sp, #0x10
003379b4: mov      r0, r7
003379b8: add      r1, pc, r1
003379bc: bl       #0x3140ec
003379c0: mov      r1, r7
003379c4: mov      r2, #0
003379c8: mov      r0, r5
003379cc: bl       #0x337ddc
003379d0: mov      r0, r7
003379d4: bl       #0x318254
003379d8: mov      r0, r5
003379dc: bl       #0x337888
003379e0: ldr      r1, [pc, #0x98]
003379e4: add      r2, sp, #0xc
003379e8: mov      r0, sb
003379ec: add      r1, pc, r1
003379f0: bl       #0x3140ec
003379f4: mov      r1, sb
003379f8: mov      r0, r5
003379fc: bl       #0x337a88
00337a00: mov      r0, sb
00337a04: bl       #0x318254
00337a08: mov      r0, r5
00337a0c: bl       #0x337888
00337a10: ldr      r1, [pc, #0x6c]
00337a14: add      r7, sp, #0x1c
00337a18: add      r2, sp, #8
00337a1c: add      r1, pc, r1
00337a20: mov      r0, r7
00337a24: bl       #0x3140ec
00337a28: mov      r0, r5
00337a2c: mov      r1, r7
00337a30: bl       #0x337a88
00337a34: mov      r0, r7
00337a38: bl       #0x318254
00337a3c: ldr      r3, [r4, r6]
00337a40: ldr      r2, [sp, #0x94]
00337a44: ldr      r3, [r3]
00337a48: cmp      r2, r3
00337a4c: bne      #0x337a58
00337a50: add      sp, sp, #0x98
00337a54: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00337a58: bl       #0x30e310

# _ZN13DebugSwitches13_loadSwitchesEP11IFileStream
003374e4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003374e8: ldr      r4, [pc, #0x36c]
003374ec: ldr      r6, [pc, #0x36c]
003374f0: sub      sp, sp, #0x1dc
003374f4: add      r4, pc, r4
003374f8: ldr      r3, [r4, r6]
003374fc: subs     r5, r1, #0
00337500: mov      r7, r0
00337504: ldr      r3, [r3]
00337508: str      r3, [sp, #0x1d4]
0033750c: beq      #0x3375d0
00337510: ldr      r3, [r5]
00337514: mov      r0, r5
00337518: mov      lr, pc
0033751c: ldr      pc, [r3, #8]
00337520: ldr      r3, [r5]
00337524: mov      r8, r0
00337528: mov      r0, r5
0033752c: mov      sb, r1
00337530: mov      lr, pc
00337534: ldr      pc, [r3, #0x24]
00337538: mov      r2, r8
0033753c: mov      r3, sb
00337540: subs     r2, r2, r0
00337544: sbc      r3, r3, r1
00337548: cmp      r3, #0
0033754c: beq      #0x3375ec
00337550: mov      r0, r5
00337554: bl       #0x3364ec
00337558: movw     r3, #0x5357
0033755c: movt     r3, #0x4442
00337560: cmp      r0, r3
00337564: beq      #0x3375f8
00337568: ldr      r3, [pc, #0x2f4]
0033756c: add      r7, sp, #0x144
00337570: ldr      r8, [r4, r3]
00337574: mov      r0, r8
00337578: bl       #0x337888
0033757c: ldr      r1, [pc, #0x2e4]
00337580: add      r2, sp, #0x2c
00337584: mov      r0, r7
00337588: add      r1, pc, r1
0033758c: bl       #0x3140ec
00337590: mov      r1, r7
00337594: mov      r0, r8
00337598: bl       #0x337a88
0033759c: mov      r0, r7
003375a0: bl       #0x318254
003375a4: ldr      r3, [r5]
003375a8: mov      r0, r5
003375ac: ldr      r7, [r3, #0x20]
003375b0: mov      lr, pc
003375b4: ldr      pc, [r3, #0x24]
003375b8: mvn      r2, #3
003375bc: adds     r2, r2, r0
003375c0: mvn      r3, #0
003375c4: adc      r3, r3, r1
003375c8: mov      r0, r5
003375cc: blx      r7
003375d0: ldr      r3, [r4, r6]
003375d4: ldr      r2, [sp, #0x1d4]
003375d8: ldr      r3, [r3]
003375dc: cmp      r2, r3
003375e0: bne      #0x337858
003375e4: add      sp, sp, #0x1dc
003375e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003375ec: cmp      r2, #0xb
003375f0: bls      #0x3375d0
003375f4: b        #0x337550
003375f8: mov      r0, r5
003375fc: bl       #0x3364ec
00337600: cmp      r0, #0x20000
00337604: bge      #0x3376d8
00337608: cmp      r0, #0x10000
0033760c: bge      #0x337764
00337610: ldr      r3, [pc, #0x24c]
00337614: ldr      r7, [pc, #0x250]
00337618: add      sl, sp, #0x174
0033761c: ldr      r5, [r4, r3]
00337620: add      r7, pc, r7
00337624: add      r8, sp, #0x15c
00337628: mov      r0, r5
0033762c: bl       #0x337888
00337630: add      r2, sp, #0x34
00337634: mov      r1, r7
00337638: mov      r0, sl
0033763c: bl       #0x3140ec
00337640: mov      r1, sl
00337644: mov      r0, r5
00337648: bl       #0x337a88
0033764c: mov      r0, sl
00337650: bl       #0x318254
00337654: mov      r0, r5
00337658: bl       #0x337888
0033765c: add      r2, sp, #0x30
00337660: mov      r1, r7
00337664: mov      r0, r8
00337668: bl       #0x3140ec
0033766c: mov      r1, r8
00337670: mov      r0, r5
00337674: bl       #0x337a88
00337678: mov      r0, r8
0033767c: bl       #0x318254
00337680: ldr      r3, [pc, #0x1e8]
00337684: ldr      r3, [r4, r3]
00337688: ldr      r3, [r3]
0033768c: cmp      r3, #2
00337690: moveq    r3, #0
00337694: streq    r3, [r3]
00337698: beq      #0x3375d0
0033769c: cmp      r3, #1
003376a0: bne      #0x3375d0
003376a4: ldr      r0, [pc, #0x1c8]
003376a8: ldr      r1, [pc, #0x1c8]
003376ac: ldr      r2, [pc, #0x1c8]
003376b0: ldr      r0, [r4, r0]
003376b4: ldr      r3, [pc, #0x1c4]
003376b8: movw     ip, #0x10d
003376bc: add      r1, pc, r1
003376c0: add      r2, pc, r2
003376c4: add      r3, pc, r3
003376c8: add      r0, r0, #0xa8
003376cc: str      ip, [sp]
003376d0: bl       #0x30e004
003376d4: b        #0x3375d0
003376d8: mov      r0, r5
003376dc: bl       #0x3364ec
003376e0: subs     sl, r0, #0
003376e4: ble      #0x337764
003376e8: add      r3, sp, #0x40
003376ec: str      r6, [sp, #0x10]
003376f0: mov      sb, #0
003376f4: add      r8, sp, #0x44
003376f8: add      fp, sp, #0x1bc
003376fc: str      r4, [sp, #0xc]
00337700: mov      r6, r3
00337704: mov      r3, #0
00337708: mov      r2, #0xff
0033770c: mov      r1, r8
00337710: mov      r0, r5
00337714: bl       #0x317734
00337718: mov      r0, r5
0033771c: bl       #0x3365a4
00337720: mov      r1, r8
00337724: mov      r4, r0
00337728: mov      r2, r6
0033772c: mov      r0, fp
00337730: bl       #0x3140ec
00337734: subs     r2, r4, #0
00337738: movne    r2, #1
0033773c: mov      r0, r7
00337740: mov      r1, fp
00337744: bl       #0x337404
00337748: add      sb, sb, #1
0033774c: mov      r0, fp
00337750: bl       #0x318254
00337754: cmp      sb, sl
00337758: bne      #0x337704
0033775c: ldr      r4, [sp, #0xc]
00337760: ldr      r6, [sp, #0x10]
00337764: mov      r0, r5
00337768: bl       #0x3364ec
0033776c: cmp      r0, #0
00337770: str      r0, [sp, #0xc]
00337774: ble      #0x3375d0
00337778: ldr      r3, [pc, #0x104]
0033777c: ldr      r2, [pc, #0xe0]
00337780: mov      sl, r4
00337784: add      r3, pc, r3
00337788: str      r3, [sp, #0x1c]
0033778c: add      r3, sp, #0x3c
00337790: str      r2, [sp, #0x18]
00337794: str      r3, [sp, #0x14]
00337798: add      r2, sp, #0x38
0033779c: add      r3, sp, #0x18c
003377a0: mov      sb, #0
003377a4: add      r8, sp, #0x44
003377a8: add      fp, sp, #0x1a4
003377ac: str      r2, [sp, #0x10]
003377b0: str      r7, [sp, #0x20]
003377b4: str      r6, [sp, #0x24]
003377b8: mov      r4, r3
003377bc: mov      r2, #0xff
003377c0: mov      r1, r8
003377c4: mov      r3, #0
003377c8: mov      r0, r5
003377cc: bl       #0x317734
003377d0: mov      r0, r5
003377d4: bl       #0x3365a4
003377d8: ldr      r3, [sp, #0x18]
003377dc: mov      r7, r0
003377e0: add      sb, sb, #1
003377e4: ldr      r6, [sl, r3]
003377e8: mov      r0, r6
003377ec: bl       #0x337888
003377f0: ldr      r2, [sp, #0x14]
003377f4: ldr      r1, [sp, #0x1c]
003377f8: mov      r0, fp
003377fc: bl       #0x3140ec
00337800: mov      r1, fp
00337804: mov      r0, r6
00337808: bl       #0x337a88
0033780c: mov      r0, fp
00337810: bl       #0x318254
00337814: mov      r1, r8
00337818: ldr      r2, [sp, #0x10]
0033781c: mov      r0, r4
00337820: bl       #0x3140ec
00337824: subs     r2, r7, #0
00337828: movne    r2, #1
0033782c: ldr      r0, [sp, #0x20]
00337830: mov      r1, r4
00337834: bl       #0x337ddc
00337838: mov      r0, r4
0033783c: bl       #0x318254
00337840: ldr      r2, [sp, #0xc]
00337844: cmp      sb, r2
00337848: bne      #0x3377bc
0033784c: mov      r4, sl
00337850: ldr      r6, [sp, #0x24]
00337854: b        #0x3375d0
00337858: bl       #0x30e310
0033785c: mlseq    r5, ip, r5, sp
00337860: andeq    r4, r0, ip, lsr #1
00337864: andeq    r0, r0, r4, lsl #17
00337868: subseq   r8, r8, r0, lsl r8
0033786c: subseq   r8, r8, r8, ror r7
00337870: andeq    r3, r0, r0, asr #19
00337874: andeq    r1, r0, r0, asr #19
00337878: subseq   r6, r8, ip, lsl sp
0033787c: ldrsheq  r8, [r8], #-0x68
00337880: subseq   r8, r8, ip, lsr #14
00337884: subseq   r8, r8, r4, lsl r6

# _ZN13DebugSwitches9SetSwitchERKSsb
00337ddc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00337de0: ldr      r4, [pc, #0xd0]
00337de4: ldr      r6, [pc, #0xd0]
00337de8: sub      sp, sp, #0x20
00337dec: add      r4, pc, r4
00337df0: ldr      r3, [r4, r6]
00337df4: mov      r5, r0
00337df8: mov      r8, r2
00337dfc: ldr      r3, [r3]
00337e00: mov      r7, r1
00337e04: str      r3, [sp, #0x1c]
00337e08: bl       #0x3369a8
00337e0c: cmp      r5, r0
00337e10: beq      #0x337e60
00337e14: mov      r0, r5
00337e18: mov      r1, r7
00337e1c: bl       #0x337288
00337e20: ldrb     r3, [r0]
00337e24: cmp      r3, r8
00337e28: beq      #0x337e44
00337e2c: mov      r1, r7
00337e30: mov      r0, r5
00337e34: bl       #0x337288
00337e38: strb     r8, [r0]
00337e3c: mov      r0, r5
00337e40: bl       #0x337d54
00337e44: ldr      r3, [r4, r6]
00337e48: ldr      r2, [sp, #0x1c]
00337e4c: ldr      r3, [r3]
00337e50: cmp      r2, r3
00337e54: bne      #0x337eb4
00337e58: add      sp, sp, #0x20
00337e5c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00337e60: ldr      r3, [pc, #0x58]
00337e64: add      sl, sp, #4
00337e68: ldr      sb, [r4, r3]
00337e6c: mov      r0, sb
00337e70: bl       #0x337888
00337e74: ldr      r1, [pc, #0x48]
00337e78: mov      r2, sp
00337e7c: mov      r0, sl
00337e80: add      r1, pc, r1
00337e84: bl       #0x3140ec
00337e88: mov      r1, sl
00337e8c: mov      r0, sb
00337e90: bl       #0x337a88
00337e94: mov      r0, sl
00337e98: bl       #0x318254
00337e9c: mov      r0, r5
00337ea0: mov      r1, r7
00337ea4: bl       #0x337288
00337ea8: mov      r3, #0
00337eac: strb     r3, [r0]
00337eb0: b        #0x337e14
00337eb4: bl       #0x30e310
00337eb8: rsbeq    ip, r5, r4, lsr #25
00337ebc: andeq    r4, r0, ip, lsr #1
00337ec0: andeq    r0, r0, r4, lsl #17
00337ec4: subseq   r7, r8, r0, lsl #30

# _ZN13DebugSwitchesC2Ev
00335f94: mov      r2, #0
00335f98: mov      r1, r0
00335f9c: mov      r3, r0
00335fa0: str      r2, [r0, #4]
00335fa4: strb     r2, [r0]
00335fa8: str      r0, [r3, #8]
00335fac: str      r0, [r3, #0xc]
00335fb0: str      r2, [r0, #0x10]
00335fb4: str      r2, [r0, #0x1c]
00335fb8: strb     r2, [r1, #0x18]!
00335fbc: str      r1, [r0, #0x24]
00335fc0: str      r2, [r0, #0x28]
00335fc4: str      r1, [r0, #0x20]
00335fc8: bx       lr

# _ZN13DebugSwitches9GetSwitchERKSs
00337a88: push     {r4, r5, r6, r7, r8, sl, lr}
00337a8c: ldr      r4, [pc, #0xa8]
00337a90: ldr      r5, [pc, #0xa8]
00337a94: sub      sp, sp, #0x24
00337a98: add      r4, pc, r4
00337a9c: ldr      r3, [r4, r5]
00337aa0: mov      r6, r0
00337aa4: mov      r7, r1
00337aa8: ldr      r3, [r3]
00337aac: str      r3, [sp, #0x1c]
00337ab0: bl       #0x3369a8
00337ab4: cmp      r6, r0
00337ab8: beq      #0x337ae8
00337abc: mov      r0, r6
00337ac0: mov      r1, r7
00337ac4: bl       #0x337288
00337ac8: ldr      r3, [r4, r5]
00337acc: ldr      r2, [sp, #0x1c]
00337ad0: ldrb     r0, [r0]
00337ad4: ldr      r3, [r3]
00337ad8: cmp      r2, r3
00337adc: bne      #0x337b38
00337ae0: add      sp, sp, #0x24
00337ae4: pop      {r4, r5, r6, r7, r8, sl, pc}
00337ae8: mov      r1, r7
00337aec: bl       #0x337288
00337af0: mov      r3, #0
00337af4: strb     r3, [r0]
00337af8: ldr      r3, [pc, #0x44]
00337afc: add      r8, sp, #4
00337b00: ldr      sl, [r4, r3]
00337b04: mov      r0, sl
00337b08: bl       #0x337888
00337b0c: ldr      r1, [pc, #0x34]
00337b10: mov      r2, sp
00337b14: mov      r0, r8
00337b18: add      r1, pc, r1
00337b1c: bl       #0x3140ec
00337b20: mov      r0, sl
00337b24: mov      r1, r8
00337b28: bl       #0x337a88
00337b2c: mov      r0, r8
00337b30: bl       #0x318254
00337b34: b        #0x337abc
00337b38: bl       #0x30e310

# _ZN13DebugSwitches9SetModuleERKSsb
00337404: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00337408: ldr      r4, [pc, #0xc4]
0033740c: ldr      r6, [pc, #0xc4]
00337410: add      r7, r0, #0x18
00337414: add      r4, pc, r4
00337418: ldr      r3, [r4, r6]
0033741c: sub      sp, sp, #0x20
00337420: mov      r8, r0
00337424: ldr      r3, [r3]
00337428: mov      r0, r7
0033742c: mov      sb, r2
00337430: mov      sl, r1
00337434: str      r3, [sp, #0x1c]
00337438: bl       #0x3369a8
0033743c: cmp      r0, r7
00337440: mov      r5, r0
00337444: beq      #0x33747c
00337448: ldrb     r3, [r0, #0x28]
0033744c: cmp      r3, sb
00337450: beq      #0x337460
00337454: strb     sb, [r0, #0x28]
00337458: mov      r0, r8
0033745c: bl       #0x337d54
00337460: ldr      r3, [r4, r6]
00337464: ldr      r2, [sp, #0x1c]
00337468: ldr      r3, [r3]
0033746c: cmp      r2, r3
00337470: bne      #0x3374d0
00337474: add      sp, sp, #0x20
00337478: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0033747c: ldr      r3, [pc, #0x58]
00337480: add      r7, sp, #4
00337484: ldr      r8, [r4, r3]
00337488: mov      r0, r8
0033748c: bl       #0x337888
00337490: ldr      r1, [pc, #0x48]
00337494: mov      r2, sp
00337498: mov      r0, r7
0033749c: add      r1, pc, r1
003374a0: bl       #0x3140ec
003374a4: mov      r1, r7
003374a8: mov      r0, r8
003374ac: bl       #0x337a88
003374b0: mov      r0, r7
003374b4: bl       #0x318254
003374b8: mov      r0, r5
003374bc: mov      r1, sl
003374c0: bl       #0x337288
003374c4: mov      r3, #1
003374c8: strb     r3, [r0]
003374cc: b        #0x337460
003374d0: bl       #0x30e310
003374d4: rsbeq    sp, r5, ip, ror r6
003374d8: andeq    r4, r0, ip, lsr #1
003374dc: andeq    r0, r0, r4, lsl #17
003374e0: subseq   r8, r8, r4, ror #17

# _ZN13DebugSwitches9GetModuleERKSs
00337ec8: push     {r4, r5, r6, r7, r8, sl, lr}
00337ecc: ldr      r4, [pc, #0xac]
00337ed0: ldr      r5, [pc, #0xac]
00337ed4: add      r8, r0, #0x18
00337ed8: add      r4, pc, r4
00337edc: ldr      r3, [r4, r5]
00337ee0: sub      sp, sp, #0x24
00337ee4: mov      r0, r8
00337ee8: ldr      r3, [r3]
00337eec: mov      r7, r1
00337ef0: str      r3, [sp, #0x1c]
00337ef4: bl       #0x3369a8
00337ef8: cmp      r0, r8
00337efc: mov      r6, r0
00337f00: ldrbne   r0, [r0, #0x28]
00337f04: beq      #0x337f24
00337f08: ldr      r3, [r4, r5]
00337f0c: ldr      r2, [sp, #0x1c]
00337f10: ldr      r3, [r3]
00337f14: cmp      r2, r3
00337f18: bne      #0x337f7c
00337f1c: add      sp, sp, #0x24
00337f20: pop      {r4, r5, r6, r7, r8, sl, pc}
00337f24: ldr      r3, [pc, #0x5c]
00337f28: add      r8, sp, #4
00337f2c: ldr      sl, [r4, r3]
00337f30: mov      r0, sl
00337f34: bl       #0x337888
00337f38: ldr      r1, [pc, #0x4c]
00337f3c: mov      r2, sp
00337f40: mov      r0, r8
00337f44: add      r1, pc, r1
00337f48: bl       #0x3140ec
00337f4c: mov      r1, r8
00337f50: mov      r0, sl
00337f54: bl       #0x337a88
00337f58: mov      r0, r8
00337f5c: bl       #0x318254
00337f60: mov      r0, r6
00337f64: mov      r1, r7
00337f68: bl       #0x337288
00337f6c: mov      r3, #1
00337f70: strb     r3, [r0]
00337f74: mov      r0, r3
00337f78: b        #0x337f08
00337f7c: bl       #0x30e310
00337f80: strhteq  ip, [r5], #-0xb8
00337f84: andeq    r4, r0, ip, lsr #1
00337f88: andeq    r0, r0, r4, lsl #17
00337f8c: subseq   r7, r8, ip, lsr lr
