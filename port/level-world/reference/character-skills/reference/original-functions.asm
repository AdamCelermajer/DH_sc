
# _ZN9LuaScript4CallEPKcRN3sfc6script3lua12ReturnValuesE
0037c494: push     {r4, r5, r6, r7, lr}
0037c498: ldr      r3, [r2, #0x24]
0037c49c: mov      r5, r2
0037c4a0: ldr      r4, [pc, #0x64]
0037c4a4: ldr      ip, [r3]
0037c4a8: ldr      r2, [r3, #4]
0037c4ac: sub      sp, sp, #0xc
0037c4b0: mov      r6, r0
0037c4b4: cmp      ip, r2
0037c4b8: add      r4, pc, r4
0037c4bc: mov      r7, r1
0037c4c0: beq      #0x37c4d4
0037c4c4: mov      r0, r3
0037c4c8: mov      r1, ip
0037c4cc: add      r3, sp, #4
0037c4d0: bl       #0x31c3cc
0037c4d4: mov      r1, r7
0037c4d8: mov      r0, r6
0037c4dc: bl       #0x37c314
0037c4e0: mov      r2, r5
0037c4e4: mov      r1, r0
0037c4e8: add      r0, r6, #4
0037c4ec: bl       #0x31ab14
0037c4f0: ldr      r3, [pc, #0x18]
0037c4f4: ldr      r3, [r4, r3]
0037c4f8: ldr      r2, [r3]
0037c4fc: add      r2, r2, #1
0037c500: str      r2, [r3]
0037c504: add      sp, sp, #0xc
0037c508: pop      {r4, r5, r6, r7, pc}

# _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsERNS4_12ReturnValuesE
0037c390: push     {r4, r5, r6, r7, r8, lr}
0037c394: mov      r5, r3
0037c398: ldr      r3, [r3, #0x24]
0037c39c: ldr      r4, [pc, #0x70]
0037c3a0: sub      sp, sp, #8
0037c3a4: ldr      lr, [r3]
0037c3a8: ldr      ip, [r3, #4]
0037c3ac: add      r4, pc, r4
0037c3b0: mov      r6, r0
0037c3b4: cmp      lr, ip
0037c3b8: mov      r7, r1
0037c3bc: mov      r8, r2
0037c3c0: beq      #0x37c3d8
0037c3c4: mov      r0, r3
0037c3c8: mov      r1, lr
0037c3cc: mov      r2, ip
0037c3d0: add      r3, sp, #4
0037c3d4: bl       #0x31c3cc
0037c3d8: mov      r1, r7
0037c3dc: mov      r0, r6
0037c3e0: bl       #0x37c314
0037c3e4: mov      r2, r8
0037c3e8: mov      r1, r0
0037c3ec: mov      r3, r5
0037c3f0: add      r0, r6, #4
0037c3f4: bl       #0x31abe8
0037c3f8: ldr      r3, [pc, #0x18]
0037c3fc: ldr      r3, [r4, r3]
0037c400: ldr      r2, [r3]
0037c404: add      r2, r2, #1
0037c408: str      r2, [r3]
0037c40c: add      sp, sp, #8
0037c410: pop      {r4, r5, r6, r7, r8, pc}
0037c414: rsbeq    r8, r1, r4, ror #13
0037c418: strdeq   r2, r3, [r0], -ip

# _ZN3sfc6script3lua12ReturnValuesC1Ev
0031b434: ldr      r3, [pc, #0x2c]
0031b438: ldr      r2, [pc, #0x2c]
0031b43c: push     {r4, lr}
0031b440: add      r3, pc, r3
0031b444: ldr      r2, [r3, r2]
0031b448: mov      r4, r0
0031b44c: add      r2, r2, #8
0031b450: str      r2, [r0], #4
0031b454: bl       #0x31a804
0031b458: bl       #0x31ce84
0031b45c: str      r0, [r4, #0x24]
0031b460: mov      r0, r4
0031b464: pop      {r4, pc}
0031b468: rsbeq    sb, r7, r0, asr r6
0031b46c: andeq    r1, r0, r4, lsr #1

# _ZN6CharAI13_SkillCleanUpEv
003d8ae0: push     {r4, r5, r6, lr}
003d8ae4: ldr      r3, [r0, #0xb4]
003d8ae8: ldr      r6, [r0, #0xb8]
003d8aec: mov      r5, r0
003d8af0: rsb      r6, r3, r6
003d8af4: asrs     r6, r6, #2
003d8af8: beq      #0x3d8b24
003d8afc: mov      r4, #0
003d8b00: b        #0x3d8b08
003d8b04: ldr      r3, [r5, #0xb4]
003d8b08: ldr      r0, [r3, r4, lsl #2]
003d8b0c: add      r4, r4, #1
003d8b10: cmp      r0, #0
003d8b14: beq      #0x3d8b1c
003d8b18: bl       #0x3daafc
003d8b1c: cmp      r4, r6
003d8b20: bne      #0x3d8b04
003d8b24: pop      {r4, r5, r6, pc}

# _ZN6CharAI13_SpellCleanUpEv
003d8a98: push     {r4, r5, r6, lr}
003d8a9c: ldr      r3, [r0, #0xc0]
003d8aa0: ldr      r6, [r0, #0xc4]
003d8aa4: mov      r5, r0
003d8aa8: rsb      r6, r3, r6
003d8aac: asrs     r6, r6, #2
003d8ab0: beq      #0x3d8adc
003d8ab4: mov      r4, #0
003d8ab8: b        #0x3d8ac0
003d8abc: ldr      r3, [r5, #0xc0]
003d8ac0: ldr      r0, [r3, r4, lsl #2]
003d8ac4: add      r4, r4, #1
003d8ac8: cmp      r0, #0
003d8acc: beq      #0x3d8ad4
003d8ad0: bl       #0x3daafc
003d8ad4: cmp      r4, r6
003d8ad8: bne      #0x3d8abc
003d8adc: pop      {r4, r5, r6, pc}

# _ZN17CharAISkillScriptD0Ev
003cc448: ldr      r3, [pc, #0x2c]
003cc44c: ldr      r2, [pc, #0x2c]
003cc450: push     {r4, lr}
003cc454: add      r3, pc, r3
003cc458: ldr      r2, [r3, r2]
003cc45c: mov      r4, r0
003cc460: add      r2, r2, #8
003cc464: str      r2, [r0], #0xc
003cc468: bl       #0x319228
003cc46c: mov      r0, r4
003cc470: bl       #0x310440
003cc474: mov      r0, r4
003cc478: pop      {r4, pc}
003cc47c: subseq   r8, ip, ip, lsr r6
003cc480: andeq    r0, r0, ip, lsr #23

# _ZN3sfc6script3lua12ReturnValuesD1Ev
0031b398: ldr      r3, [pc, #0x30]
0031b39c: ldr      r2, [pc, #0x30]
0031b3a0: push     {r4, lr}
0031b3a4: add      r3, pc, r3
0031b3a8: ldr      r2, [r3, r2]
0031b3ac: mov      r4, r0
0031b3b0: ldr      r0, [r0, #0x24]
0031b3b4: add      r2, r2, #8
0031b3b8: str      r2, [r4]
0031b3bc: bl       #0x31d194
0031b3c0: add      r0, r4, #4
0031b3c4: bl       #0x31a68c
0031b3c8: mov      r0, r4
0031b3cc: pop      {r4, pc}
0031b3d0: rsbeq    sb, r7, ip, ror #13
0031b3d4: andeq    r1, r0, r4, lsr #1

# _ZNK9Character18GetCharSkillListIdEv
003bc5c0: movw     r3, #0x1068
003bc5c4: ldr      r0, [r0, r3]
003bc5c8: ldr      r3, [pc, #0x24]
003bc5cc: cmp      r0, #0
003bc5d0: add      r3, pc, r3
003bc5d4: blt      #0x3bc5ec
003bc5d8: ldr      r2, [pc, #0x18]
003bc5dc: ldr      r3, [r3, r2]
003bc5e0: ldr      r3, [r3]
003bc5e4: cmp      r0, r3
003bc5e8: bxlt     lr
003bc5ec: mov      r0, #3
003bc5f0: bx       lr
003bc5f4: subseq   r8, sp, r0, asr #9
003bc5f8: andeq    r2, r0, r8, ror sp

# _ZNK9Character18GetCharFaeryListIdEv
003ae5a0: movw     r3, #0x106c
003ae5a4: ldr      r0, [r0, r3]
003ae5a8: ldr      r3, [pc, #0x24]
003ae5ac: cmp      r0, #0
003ae5b0: add      r3, pc, r3
003ae5b4: blt      #0x3ae5cc
003ae5b8: ldr      r2, [pc, #0x18]
003ae5bc: ldr      r3, [r3, r2]
003ae5c0: ldr      r3, [r3]
003ae5c4: cmp      r0, r3
003ae5c8: bxlt     lr
003ae5cc: mov      r0, #0
003ae5d0: bx       lr
003ae5d4: subseq   r6, lr, r0, ror #9
003ae5d8: andeq    r4, r0, r4, lsl r5

# _ZN17CharAISkillScript13OnSkillUpdateEv
003dabd0: push     {r4, r5, r6, r7, lr}
003dabd4: ldr      r4, [pc, #0xb8]
003dabd8: ldr      r7, [pc, #0xb8]
003dabdc: sub      sp, sp, #0x34
003dabe0: add      r4, pc, r4
003dabe4: ldr      r3, [r4, r7]
003dabe8: add      r5, sp, #4
003dabec: mov      r6, r0
003dabf0: ldr      r3, [r3]
003dabf4: mov      r0, r5
003dabf8: str      r3, [sp, #0x2c]
003dabfc: bl       #0x31b434
003dac00: ldr      r3, [r6, #4]
003dac04: ldr      r0, [r3, #0x3e4]
003dac08: cmp      r0, #0
003dac0c: beq      #0x3dac30
003dac10: ldr      r1, [pc, #0x84]
003dac14: mov      r3, r5
003dac18: add      r2, r6, #0xc
003dac1c: add      r1, pc, r1
003dac20: bl       #0x37c390
003dac24: ldr      r3, [sp, #0xc]
003dac28: cmp      r3, #0
003dac2c: beq      #0x3dac54
003dac30: mov      r0, r5
003dac34: bl       #0x31b398
003dac38: ldr      r3, [r4, r7]
003dac3c: ldr      r2, [sp, #0x2c]
003dac40: ldr      r3, [r3]
003dac44: cmp      r2, r3
003dac48: bne      #0x3dac90
003dac4c: add      sp, sp, #0x34
003dac50: pop      {r4, r5, r6, r7, pc}
003dac54: ldr      r0, [sp, #0x28]
003dac58: ldm      r0, {r1, r2}
003dac5c: cmp      r1, r2
003dac60: beq      #0x3dac6c
003dac64: mov      r3, sp
003dac68: bl       #0x31c3cc
003dac6c: ldr      r3, [r6, #4]
003dac70: ldr      r1, [pc, #0x28]
003dac74: mov      r2, r5
003dac78: ldr      r0, [r3, #0x3e4]
003dac7c: add      r1, pc, r1
003dac80: bl       #0x37c494
003dac84: mov      r0, r5
003dac88: bl       #0x31b398
003dac8c: b        #0x3dac38
003dac90: bl       #0x30e310
003dac94: ldrheq   sb, [fp], #-0xe0
003dac98: andeq    r4, r0, ip, lsr #1
003dac9c: subeq    sl, lr, r4, lsr ip
003daca0: subeq    sl, lr, ip, lsr #24

# _ZN17CharAISkillScriptC1EP9CharacterPKcj
003cde2c: push     {r4, r5, r6, r7, r8, sl, lr}
003cde30: ldr      r5, [pc, #0x11c]
003cde34: ldr      ip, [pc, #0x11c]
003cde38: mov      r4, r0
003cde3c: add      r5, pc, r5
003cde40: ldr      ip, [r5, ip]
003cde44: add      r7, r0, #0xc
003cde48: mov      sl, r1
003cde4c: add      ip, ip, #8
003cde50: str      ip, [r0]
003cde54: sub      sp, sp, #0xc
003cde58: stmib    r4, {r1, r2}
003cde5c: mov      r0, r7
003cde60: mov      r8, r3
003cde64: mov      r6, r2
003cde68: bl       #0x3192b4
003cde6c: mvn      r3, #0
003cde70: cmp      sl, #0
003cde74: str      r3, [r4, #0x18]
003cde78: str      r8, [r4, #0x14]
003cde7c: beq      #0x3cdeac
003cde80: cmp      r6, #0
003cde84: beq      #0x3cdf00
003cde88: mov      r1, r6
003cde8c: mov      r0, r7
003cde90: bl       #0x39ec10
003cde94: mov      r0, r7
003cde98: mov      r1, r8
003cde9c: bl       #0x3cdd78
003cdea0: mov      r0, r4
003cdea4: add      sp, sp, #0xc
003cdea8: pop      {r4, r5, r6, r7, r8, sl, pc}
003cdeac: ldr      r3, [pc, #0xa8]
003cdeb0: ldr      r3, [r5, r3]
003cdeb4: ldr      r3, [r3]
003cdeb8: cmp      r3, #2
003cdebc: streq    sl, [sl]
003cdec0: beq      #0x3cde80
003cdec4: cmp      r3, #1
003cdec8: bne      #0x3cde80
003cdecc: ldr      r0, [pc, #0x8c]
003cded0: ldr      r1, [pc, #0x8c]
003cded4: ldr      r2, [pc, #0x8c]
003cded8: ldr      r0, [r5, r0]
003cdedc: ldr      r3, [pc, #0x88]
003cdee0: mov      ip, #0x2e
003cdee4: add      r1, pc, r1
003cdee8: add      r2, pc, r2
003cdeec: add      r3, pc, r3
003cdef0: add      r0, r0, #0xa8
003cdef4: str      ip, [sp]
003cdef8: bl       #0x30e004
003cdefc: b        #0x3cde80
003cdf00: ldr      r3, [pc, #0x54]
003cdf04: ldr      r3, [r5, r3]
003cdf08: ldr      r3, [r3]
003cdf0c: cmp      r3, #2
003cdf10: streq    r6, [r6]
003cdf14: beq      #0x3cde88
003cdf18: cmp      r3, #1
003cdf1c: bne      #0x3cde88
003cdf20: ldr      r0, [pc, #0x38]
003cdf24: ldr      r1, [pc, #0x44]
003cdf28: ldr      r2, [pc, #0x44]
003cdf2c: ldr      r0, [r5, r0]
003cdf30: ldr      r3, [pc, #0x40]
003cdf34: mov      ip, #0x2e
003cdf38: add      r1, pc, r1
003cdf3c: add      r2, pc, r2
003cdf40: add      r3, pc, r3
003cdf44: add      r0, r0, #0xa8
003cdf48: str      ip, [sp]
003cdf4c: bl       #0x30e004
003cdf50: b        #0x3cde88
003cdf54: subseq   r6, ip, r4, asr ip
003cdf58: andeq    r0, r0, ip, lsr #23
003cdf5c: andeq    r3, r0, r0, asr #19
003cdf60: andeq    r1, r0, r0, asr #19
003cdf64: strdeq   r0, r1, [pc], #-0x44
003cdf68: subeq    r7, pc, r0, lsl r4
003cdf6c: subeq    r7, pc, r4, lsl r4
003cdf70: subeq    r0, pc, r0, lsr #9
003cdf74: subseq   r3, r1, ip, lsr #3
003cdf78: subeq    r7, pc, r0, asr #7

# _ZN6CharAI15UpdateAllSkillsEv
003d8894: push     {r4, r5, r6, lr}
003d8898: mov      r5, r0
003d889c: ldr      r0, [r0, #4]
003d88a0: add      r0, r0, #0x4f0
003d88a4: add      r0, r0, #0xc
003d88a8: bl       #0x3c02e8
003d88ac: cmp      r0, #0
003d88b0: beq      #0x3d88b8
003d88b4: pop      {r4, r5, r6, pc}
003d88b8: ldr      r0, [r5, #4]
003d88bc: add      r0, r0, #0x4f0
003d88c0: add      r0, r0, #0xc
003d88c4: bl       #0x3c0334
003d88c8: subs     r4, r0, #0
003d88cc: bne      #0x3d88b4
003d88d0: ldr      r3, [r5, #0xb4]
003d88d4: ldr      r6, [r5, #0xb8]
003d88d8: rsb      r6, r3, r6
003d88dc: asrs     r6, r6, #2
003d88e0: bne      #0x3d88ec
003d88e4: b        #0x3d8908
003d88e8: ldr      r3, [r5, #0xb4]
003d88ec: ldr      r0, [r3, r4, lsl #2]
003d88f0: add      r4, r4, #1
003d88f4: cmp      r0, #0
003d88f8: beq      #0x3d8900
003d88fc: bl       #0x3dabd0
003d8900: cmp      r4, r6
003d8904: bne      #0x3d88e8
003d8908: ldr      r3, [r5, #0xc0]
003d890c: ldr      r6, [r5, #0xc4]
003d8910: rsb      r6, r3, r6
003d8914: asrs     r6, r6, #2
003d8918: beq      #0x3d88b4
003d891c: mov      r4, #0
003d8920: b        #0x3d8928
003d8924: ldr      r3, [r5, #0xc0]
003d8928: ldr      r0, [r3, r4, lsl #2]
003d892c: add      r4, r4, #1
003d8930: cmp      r0, #0
003d8934: beq      #0x3d893c
003d8938: bl       #0x3dabd0
003d893c: cmp      r4, r6
003d8940: bne      #0x3d8924
003d8944: pop      {r4, r5, r6, pc}

# _ZN17CharAISkillScript14OnSkillCleanUpEv
003daafc: push     {r4, r5, r6, r7, lr}
003dab00: ldr      r4, [pc, #0xb8]
003dab04: ldr      r7, [pc, #0xb8]
003dab08: sub      sp, sp, #0x34
003dab0c: add      r4, pc, r4
003dab10: ldr      r3, [r4, r7]
003dab14: add      r5, sp, #4
003dab18: mov      r6, r0
003dab1c: ldr      r3, [r3]
003dab20: mov      r0, r5
003dab24: str      r3, [sp, #0x2c]
003dab28: bl       #0x31b434
003dab2c: ldr      r3, [r6, #4]
003dab30: ldr      r0, [r3, #0x3e4]
003dab34: cmp      r0, #0
003dab38: beq      #0x3dab5c
003dab3c: ldr      r1, [pc, #0x84]
003dab40: mov      r3, r5
003dab44: add      r2, r6, #0xc
003dab48: add      r1, pc, r1
003dab4c: bl       #0x37c390
003dab50: ldr      r3, [sp, #0xc]
003dab54: cmp      r3, #0
003dab58: beq      #0x3dab80
003dab5c: mov      r0, r5
003dab60: bl       #0x31b398
003dab64: ldr      r3, [r4, r7]
003dab68: ldr      r2, [sp, #0x2c]
003dab6c: ldr      r3, [r3]
003dab70: cmp      r2, r3
003dab74: bne      #0x3dabbc
003dab78: add      sp, sp, #0x34
003dab7c: pop      {r4, r5, r6, r7, pc}
003dab80: ldr      r0, [sp, #0x28]
003dab84: ldm      r0, {r1, r2}
003dab88: cmp      r1, r2
003dab8c: beq      #0x3dab98
003dab90: mov      r3, sp
003dab94: bl       #0x31c3cc
003dab98: ldr      r3, [r6, #4]
003dab9c: ldr      r1, [pc, #0x28]
003daba0: mov      r2, r5
003daba4: ldr      r0, [r3, #0x3e4]
003daba8: add      r1, pc, r1
003dabac: bl       #0x37c494
003dabb0: mov      r0, r5
003dabb4: bl       #0x31b398
003dabb8: b        #0x3dab64
003dabbc: bl       #0x30e310
003dabc0: subseq   sb, fp, r4, lsl #31
003dabc4: andeq    r4, r0, ip, lsr #1
003dabc8: subeq    sl, lr, r8, lsl #26
003dabcc: strdeq   sl, fp, [lr], #-0xc0

# _ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE8_M_eraseEPS3_S6_RKSt12__false_type
0031c3cc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0031c3d0: ldr      r4, [r0, #4]
0031c3d4: mov      r5, r0
0031c3d8: mov      r8, r2
0031c3dc: rsb      r3, r2, r4
0031c3e0: asr      r3, r3, #4
0031c3e4: mov      r7, r1
0031c3e8: add      sl, r3, r3, lsl #3
0031c3ec: add      sl, sl, sl, lsl #6
0031c3f0: add      sl, r3, sl, lsl #3
0031c3f4: add      sl, sl, sl, lsl #15
0031c3f8: add      sl, r3, sl, lsl #3
0031c3fc: rsb      sl, sl, #0
0031c400: cmp      sl, #0
0031c404: movle    sl, r1
0031c408: ble      #0x31c438
0031c40c: mov      r6, sl
0031c410: mov      r4, #0
0031c414: add      r0, r7, r4
0031c418: add      r1, r8, r4
0031c41c: bl       #0x31c368
0031c420: subs     r6, r6, #1
0031c424: add      r4, r4, #0x70
0031c428: bne      #0x31c414
0031c42c: mov      r3, #0x70
0031c430: mla      sl, r3, sl, r7
0031c434: ldr      r4, [r5, #4]
0031c438: cmp      sl, r4
0031c43c: beq      #0x31c460
0031c440: mov      r6, sl
0031c444: ldr      r3, [r6]
0031c448: mov      r0, r6
0031c44c: add      r6, r6, #0x70
0031c450: mov      lr, pc
0031c454: ldr      pc, [r3]
0031c458: cmp      r6, r4
0031c45c: bne      #0x31c444
0031c460: str      sl, [r5, #4]
0031c464: mov      r0, r7
0031c468: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6CharAI18SetSkillsAndSpellsEv
003ce044: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ce048: ldr      r6, [pc, #0x714]
003ce04c: ldr      r2, [pc, #0x714]
003ce050: sub      sp, sp, #0xa4
003ce054: add      r6, pc, r6
003ce058: str      r2, [sp, #0x14]
003ce05c: ldr      r2, [r6, r2]
003ce060: ldr      r3, [r0, #0x1c]
003ce064: mov      r4, r0
003ce068: ldr      r2, [r2]
003ce06c: cmp      r3, #0
003ce070: str      r2, [sp, #0x9c]
003ce074: beq      #0x3ce6ac
003ce078: ldr      fp, [pc, #0x6ec]
003ce07c: add      r5, sp, #0x84
003ce080: ldr      r7, [r6, fp]
003ce084: mov      r0, r7
003ce088: bl       #0x337888
003ce08c: ldr      r1, [pc, #0x6dc]
003ce090: add      r2, sp, #0x50
003ce094: mov      r0, r5
003ce098: add      r1, pc, r1
003ce09c: bl       #0x3140ec
003ce0a0: mov      r0, r7
003ce0a4: mov      r1, r5
003ce0a8: bl       #0x337a88
003ce0ac: ldr      r0, [sp, #0x98]
003ce0b0: cmp      r0, r5
003ce0b4: beq      #0x3ce0d4
003ce0b8: cmp      r0, #0
003ce0bc: beq      #0x3ce0d4
003ce0c0: ldr      r1, [sp, #0x84]
003ce0c4: rsb      r1, r0, r1
003ce0c8: cmp      r1, #0x80
003ce0cc: bhi      #0x3ce694
003ce0d0: bl       #0x708f00
003ce0d4: ldr      r3, [r4, #0x1c]
003ce0d8: add      r8, sp, #0x6c
003ce0dc: str      r8, [sp, #0x7c]
003ce0e0: str      r8, [sp, #0x80]
003ce0e4: ldr      r2, [r3, #0x78]
003ce0e8: ldr      r1, [r3, #0x7c]
003ce0ec: mov      r0, r8
003ce0f0: bl       #0x3116e8
003ce0f4: ldr      r1, [pc, #0x678]
003ce0f8: ldr      r0, [r4, #0x1c]
003ce0fc: add      r1, pc, r1
003ce100: add      r0, r0, #0x68
003ce104: add      r2, r1, #0x14
003ce108: bl       #0x3109e0
003ce10c: ldr      r5, [r4, #0xb8]
003ce110: ldr      r3, [r4, #0xb4]
003ce114: rsb      r5, r3, r5
003ce118: asrs     r5, r5, #2
003ce11c: beq      #0x3ce448
003ce120: ldr      r5, [r4, #0xc4]
003ce124: ldr      r3, [r4, #0xc0]
003ce128: rsb      r5, r3, r5
003ce12c: asrs     r5, r5, #2
003ce130: beq      #0x3ce204
003ce134: ldr      r0, [r4, #0x1c]
003ce138: add      r0, r0, #0x68
003ce13c: cmp      r0, r8
003ce140: beq      #0x3ce150
003ce144: ldr      r1, [sp, #0x80]
003ce148: ldr      r2, [sp, #0x7c]
003ce14c: bl       #0x3109e0
003ce150: ldr      r7, [r6, fp]
003ce154: add      r5, sp, #0x54
003ce158: mov      r0, r7
003ce15c: bl       #0x337888
003ce160: ldr      r1, [pc, #0x610]
003ce164: add      r2, sp, #0x4c
003ce168: mov      r0, r5
003ce16c: add      r1, pc, r1
003ce170: bl       #0x3140ec
003ce174: mov      r0, r7
003ce178: mov      r1, r5
003ce17c: bl       #0x337a88
003ce180: ldr      r0, [sp, #0x68]
003ce184: cmp      r0, r5
003ce188: beq      #0x3ce1a8
003ce18c: cmp      r0, #0
003ce190: beq      #0x3ce1a8
003ce194: ldr      r1, [sp, #0x54]
003ce198: rsb      r1, r0, r1
003ce19c: cmp      r1, #0x80
003ce1a0: bhi      #0x3ce69c
003ce1a4: bl       #0x708f00
003ce1a8: ldr      r3, [r4, #0x1c]
003ce1ac: mov      r0, r3
003ce1b0: ldr      r3, [r3]
003ce1b4: mov      lr, pc
003ce1b8: ldr      pc, [r3, #0xcc]
003ce1bc: ldr      r0, [sp, #0x80]
003ce1c0: cmp      r0, r8
003ce1c4: beq      #0x3ce1e4
003ce1c8: cmp      r0, #0
003ce1cc: beq      #0x3ce1e4
003ce1d0: ldr      r1, [sp, #0x6c]
003ce1d4: rsb      r1, r0, r1
003ce1d8: cmp      r1, #0x80
003ce1dc: bhi      #0x3ce6a4
003ce1e0: bl       #0x708f00
003ce1e4: ldr      r2, [sp, #0x14]
003ce1e8: ldr      r3, [r6, r2]
003ce1ec: ldr      r2, [sp, #0x9c]
003ce1f0: ldr      r3, [r3]
003ce1f4: cmp      r2, r3
003ce1f8: bne      #0x3ce760
003ce1fc: add      sp, sp, #0xa4
003ce200: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ce204: add      r2, r4, #0xc0
003ce208: ldr      r0, [r4, #4]
003ce20c: str      r2, [sp, #0x18]
003ce210: bl       #0x3ae5dc
003ce214: add      sb, sp, #0x2c
003ce218: ldr      r1, [r0, #4]
003ce21c: mov      r7, r0
003ce220: ldr      r0, [sp, #0x18]
003ce224: bl       #0x3cd584
003ce228: mov      r0, sb
003ce22c: bl       #0x3192b4
003ce230: ldr      r1, [pc, #0x544]
003ce234: mov      r0, sb
003ce238: add      r1, pc, r1
003ce23c: bl       #0x39ec10
003ce240: mov      r0, sb
003ce244: mvn      r1, #0
003ce248: bl       #0x3cdd78
003ce24c: ldr      r3, [r7, #4]
003ce250: cmp      r3, #0
003ce254: beq      #0x3ce414
003ce258: ldr      r3, [pc, #0x520]
003ce25c: str      r6, [sp, #0x24]
003ce260: mov      sl, r8
003ce264: str      r3, [sp, #0x20]
003ce268: ldr      r3, [pc, #0x514]
003ce26c: add      r3, pc, r3
003ce270: str      r3, [sp, #8]
003ce274: ldr      r3, [pc, #0x50c]
003ce278: add      r3, pc, r3
003ce27c: str      r3, [sp, #0xc]
003ce280: ldr      r3, [pc, #0x504]
003ce284: add      r3, pc, r3
003ce288: str      r3, [sp, #0x10]
003ce28c: ldr      r3, [pc, #0x4fc]
003ce290: add      r3, pc, r3
003ce294: str      r3, [sp, #0x1c]
003ce298: b        #0x3ce3bc
003ce29c: ldr      r0, [r4, #0x1c]
003ce2a0: ldr      r1, [sp, #8]
003ce2a4: bl       #0x37b574
003ce2a8: ldr      r6, [sp, #0x30]
003ce2ac: ldm      r6, {r0, r3}
003ce2b0: rsb      r3, r0, r3
003ce2b4: asr      r3, r3, #4
003ce2b8: add      r2, r3, r3, lsl #3
003ce2bc: add      r2, r2, r2, lsl #6
003ce2c0: add      r2, r3, r2, lsl #3
003ce2c4: add      r2, r2, r2, lsl #15
003ce2c8: add      r2, r3, r2, lsl #3
003ce2cc: cmp      r2, #0
003ce2d0: bne      #0x3ce2e4
003ce2d4: ldr      r2, [sp, #0x20]
003ce2d8: add      r0, pc, r2
003ce2dc: bl       #0x708eb0
003ce2e0: ldr      r0, [r6]
003ce2e4: ldr      r1, [r8, #0x18]
003ce2e8: bl       #0x31c46c
003ce2ec: ldr      r6, [sp, #0x30]
003ce2f0: ldm      r6, {r0, r3}
003ce2f4: rsb      r3, r0, r3
003ce2f8: asr      r3, r3, #4
003ce2fc: add      r2, r3, r3, lsl #3
003ce300: add      r2, r2, r2, lsl #6
003ce304: add      r2, r3, r2, lsl #3
003ce308: add      r2, r2, r2, lsl #15
003ce30c: add      r2, r3, r2, lsl #3
003ce310: rsb      r2, r2, #0
003ce314: cmp      r2, #1
003ce318: bhi      #0x3ce328
003ce31c: ldr      r0, [sp, #0x1c]
003ce320: bl       #0x708eb0
003ce324: ldr      r0, [r6]
003ce328: mov      r1, #0xbf000000
003ce32c: add      r0, r0, #0x70
003ce330: add      r1, r1, #0x800000
003ce334: bl       #0x31b5e8
003ce338: ldr      r0, [r4, #0x1c]
003ce33c: ldr      r1, [sp, #0xc]
003ce340: mov      r2, sb
003ce344: bl       #0x37c41c
003ce348: ldr      r0, [r4, #0x1c]
003ce34c: ldr      r1, [r8, #0x18]
003ce350: bl       #0x37b574
003ce354: cmp      r0, #0
003ce358: beq      #0x3ce420
003ce35c: mov      r1, #0
003ce360: mov      r0, #0x1c
003ce364: bl       #0x310570
003ce368: ldr      r1, [r4, #4]
003ce36c: mvn      r3, #0
003ce370: ldr      r2, [r8, #0x18]
003ce374: mov      r6, r0
003ce378: bl       #0x3cde2c
003ce37c: ldr      r1, [r4, #0xc4]
003ce380: ldr      r3, [r4, #0xc8]
003ce384: str      r6, [sp, #0x3c]
003ce388: cmp      r1, r3
003ce38c: beq      #0x3ce730
003ce390: str      r6, [r1]
003ce394: ldr      r3, [r4, #0xc4]
003ce398: add      r3, r3, #4
003ce39c: str      r3, [r4, #0xc4]
003ce3a0: ldr      r0, [r4, #0x1c]
003ce3a4: ldr      r1, [sp, #0x10]
003ce3a8: bl       #0x37c514
003ce3ac: ldr      r3, [r7, #4]
003ce3b0: add      r5, r5, #1
003ce3b4: cmp      r3, r5
003ce3b8: bls      #0x3ce40c
003ce3bc: ldr      r0, [r4, #4]
003ce3c0: mov      r1, r5
003ce3c4: bl       #0x3aeac0
003ce3c8: ldr      r3, [r0, #0x14]
003ce3cc: mov      r8, r0
003ce3d0: cmp      r3, #0
003ce3d4: bne      #0x3ce29c
003ce3d8: ldr      r1, [r4, #0xc4]
003ce3dc: ldr      r2, [r4, #0xc8]
003ce3e0: str      r3, [sp, #0x34]
003ce3e4: cmp      r1, r2
003ce3e8: beq      #0x3ce700
003ce3ec: str      r3, [r1]
003ce3f0: ldr      r3, [r4, #0xc4]
003ce3f4: add      r5, r5, #1
003ce3f8: add      r3, r3, #4
003ce3fc: str      r3, [r4, #0xc4]
003ce400: ldr      r3, [r7, #4]
003ce404: cmp      r3, r5
003ce408: bhi      #0x3ce3bc
003ce40c: ldr      r6, [sp, #0x24]
003ce410: mov      r8, sl
003ce414: mov      r0, sb
003ce418: bl       #0x319228
003ce41c: b        #0x3ce134
003ce420: ldr      r1, [r4, #0xc4]
003ce424: ldr      r3, [r4, #0xc8]
003ce428: str      r0, [sp, #0x38]
003ce42c: cmp      r1, r3
003ce430: beq      #0x3ce750
003ce434: str      r0, [r1]
003ce438: ldr      r3, [r4, #0xc4]
003ce43c: add      r3, r3, #4
003ce440: str      r3, [r4, #0xc4]
003ce444: b        #0x3ce3a0
003ce448: add      r3, r4, #0xb4
003ce44c: ldr      r0, [r4, #4]
003ce450: str      r3, [sp, #0x18]
003ce454: bl       #0x3bc5fc
003ce458: add      sb, sp, #0x2c
003ce45c: ldr      r1, [r0, #4]
003ce460: mov      r7, r0
003ce464: ldr      r0, [sp, #0x18]
003ce468: bl       #0x3cd584
003ce46c: mov      r0, sb
003ce470: bl       #0x3192b4
003ce474: ldr      r1, [pc, #0x318]
003ce478: mov      r0, sb
003ce47c: add      r1, pc, r1
003ce480: bl       #0x39ec10
003ce484: mov      r0, sb
003ce488: mvn      r1, #0
003ce48c: bl       #0x3cdd78
003ce490: ldr      r3, [r7, #4]
003ce494: cmp      r3, #0
003ce498: beq      #0x3ce660
003ce49c: ldr      r3, [pc, #0x2f4]
003ce4a0: ldr      r2, [pc, #0x2f4]
003ce4a4: str      r6, [sp, #0x24]
003ce4a8: add      r3, pc, r3
003ce4ac: str      r3, [sp, #8]
003ce4b0: ldr      r3, [pc, #0x2e8]
003ce4b4: str      r2, [sp, #0x20]
003ce4b8: mov      sl, r8
003ce4bc: add      r3, pc, r3
003ce4c0: str      r3, [sp, #0xc]
003ce4c4: ldr      r3, [pc, #0x2d8]
003ce4c8: add      r3, pc, r3
003ce4cc: str      r3, [sp, #0x10]
003ce4d0: ldr      r3, [pc, #0x2d0]
003ce4d4: add      r3, pc, r3
003ce4d8: str      r3, [sp, #0x1c]
003ce4dc: b        #0x3ce608
003ce4e0: ldr      r0, [r4, #0x1c]
003ce4e4: ldr      r1, [sp, #8]
003ce4e8: bl       #0x37b574
003ce4ec: ldr      r6, [sp, #0x30]
003ce4f0: ldm      r6, {r0, r3}
003ce4f4: rsb      r3, r0, r3
003ce4f8: asr      r3, r3, #4
003ce4fc: add      r2, r3, r3, lsl #3
003ce500: add      r2, r2, r2, lsl #6
003ce504: add      r2, r3, r2, lsl #3
003ce508: add      r2, r2, r2, lsl #15
003ce50c: add      r2, r3, r2, lsl #3
003ce510: cmp      r2, #0
003ce514: bne      #0x3ce528
003ce518: ldr      r3, [sp, #0x20]
003ce51c: add      r0, pc, r3
003ce520: bl       #0x708eb0
003ce524: ldr      r0, [r6]
003ce528: ldr      r1, [r8, #0x28]
003ce52c: bl       #0x31c46c
003ce530: ldr      r6, [sp, #0x30]
003ce534: ldm      r6, {r2, r3}
003ce538: rsb      r3, r2, r3
003ce53c: asr      r3, r3, #4
003ce540: add      r1, r3, r3, lsl #3
003ce544: add      r1, r1, r1, lsl #6
003ce548: add      r1, r3, r1, lsl #3
003ce54c: add      r1, r1, r1, lsl #15
003ce550: add      r1, r3, r1, lsl #3
003ce554: rsb      r1, r1, #0
003ce558: cmp      r1, #1
003ce55c: bhi      #0x3ce56c
003ce560: ldr      r0, [sp, #0x1c]
003ce564: bl       #0x708eb0
003ce568: ldr      r2, [r6]
003ce56c: mov      r0, r5
003ce570: add      r6, r2, #0x70
003ce574: bl       #0x30e964
003ce578: mov      r1, r0
003ce57c: mov      r0, r6
003ce580: bl       #0x31b5e8
003ce584: ldr      r0, [r4, #0x1c]
003ce588: ldr      r1, [sp, #0xc]
003ce58c: mov      r2, sb
003ce590: bl       #0x37c41c
003ce594: ldr      r0, [r4, #0x1c]
003ce598: ldr      r1, [r8, #0x28]
003ce59c: bl       #0x37b574
003ce5a0: cmp      r0, #0
003ce5a4: beq      #0x3ce66c
003ce5a8: mov      r1, #0
003ce5ac: mov      r0, #0x1c
003ce5b0: bl       #0x310570
003ce5b4: ldr      r1, [r4, #4]
003ce5b8: mov      r3, r5
003ce5bc: ldr      r2, [r8, #0x28]
003ce5c0: mov      r6, r0
003ce5c4: bl       #0x3cde2c
003ce5c8: ldr      r1, [r4, #0xb8]
003ce5cc: ldr      r3, [r4, #0xbc]
003ce5d0: str      r6, [sp, #0x48]
003ce5d4: cmp      r1, r3
003ce5d8: beq      #0x3ce740
003ce5dc: str      r6, [r1]
003ce5e0: ldr      r3, [r4, #0xb8]
003ce5e4: add      r3, r3, #4
003ce5e8: str      r3, [r4, #0xb8]
003ce5ec: ldr      r0, [r4, #0x1c]
003ce5f0: ldr      r1, [sp, #0x10]
003ce5f4: bl       #0x37c514
003ce5f8: ldr      r3, [r7, #4]
003ce5fc: add      r5, r5, #1
003ce600: cmp      r3, r5
003ce604: bls      #0x3ce658
003ce608: ldr      r0, [r4, #4]
003ce60c: mov      r1, r5
003ce610: bl       #0x3bc784
003ce614: ldr      r3, [r0, #0x24]
003ce618: mov      r8, r0
003ce61c: cmp      r3, #0
003ce620: bne      #0x3ce4e0
003ce624: ldr      r1, [r4, #0xb8]
003ce628: ldr      r2, [r4, #0xbc]
003ce62c: str      r3, [sp, #0x40]
003ce630: cmp      r1, r2
003ce634: beq      #0x3ce710
003ce638: str      r3, [r1]
003ce63c: ldr      r3, [r4, #0xb8]
003ce640: add      r5, r5, #1
003ce644: add      r3, r3, #4
003ce648: str      r3, [r4, #0xb8]
003ce64c: ldr      r3, [r7, #4]
003ce650: cmp      r3, r5
003ce654: bhi      #0x3ce608
003ce658: ldr      r6, [sp, #0x24]
003ce65c: mov      r8, sl
003ce660: mov      r0, sb
003ce664: bl       #0x319228
003ce668: b        #0x3ce120
003ce66c: ldr      r1, [r4, #0xb8]
003ce670: ldr      r3, [r4, #0xbc]
003ce674: str      r0, [sp, #0x44]
003ce678: cmp      r1, r3
003ce67c: beq      #0x3ce720
003ce680: str      r0, [r1]
003ce684: ldr      r3, [r4, #0xb8]
003ce688: add      r3, r3, #4
003ce68c: str      r3, [r4, #0xb8]
003ce690: b        #0x3ce5ec
003ce694: bl       #0x310440
003ce698: b        #0x3ce0d4
003ce69c: bl       #0x310440
003ce6a0: b        #0x3ce1a8
003ce6a4: bl       #0x310440
003ce6a8: b        #0x3ce1e4
003ce6ac: ldr      r2, [pc, #0xf8]
003ce6b0: ldr      r2, [r6, r2]
003ce6b4: ldr      r2, [r2]
003ce6b8: cmp      r2, #2
003ce6bc: streq    r3, [r3]
003ce6c0: beq      #0x3ce078
003ce6c4: cmp      r2, #1
003ce6c8: bne      #0x3ce078
003ce6cc: ldr      r0, [pc, #0xdc]
003ce6d0: ldr      r1, [pc, #0xdc]
003ce6d4: ldr      r2, [pc, #0xdc]
003ce6d8: ldr      r0, [r6, r0]
003ce6dc: ldr      r3, [pc, #0xd8]
003ce6e0: mov      ip, #0x294
003ce6e4: add      r1, pc, r1
003ce6e8: add      r2, pc, r2
003ce6ec: add      r3, pc, r3
003ce6f0: add      r0, r0, #0xa8
003ce6f4: str      ip, [sp]
003ce6f8: bl       #0x30e004
003ce6fc: b        #0x3ce078
003ce700: ldr      r0, [sp, #0x18]
003ce704: add      r2, sp, #0x34
003ce708: bl       #0x3cd89c
003ce70c: b        #0x3ce3ac
003ce710: ldr      r0, [sp, #0x18]
003ce714: add      r2, sp, #0x40
003ce718: bl       #0x3cd89c
003ce71c: b        #0x3ce5f8
003ce720: ldr      r0, [sp, #0x18]
003ce724: add      r2, sp, #0x44
003ce728: bl       #0x3cd89c
003ce72c: b        #0x3ce5ec
003ce730: ldr      r0, [sp, #0x18]
003ce734: add      r2, sp, #0x3c
003ce738: bl       #0x3cd89c
003ce73c: b        #0x3ce3a0
003ce740: ldr      r0, [sp, #0x18]
003ce744: add      r2, sp, #0x48
003ce748: bl       #0x3cd89c
003ce74c: b        #0x3ce5ec
003ce750: ldr      r0, [sp, #0x18]
003ce754: add      r2, sp, #0x38
003ce758: bl       #0x3cd89c
003ce75c: b        #0x3ce3a0
003ce760: bl       #0x30e310
003ce764: subseq   r6, ip, ip, lsr sl
003ce768: andeq    r4, r0, ip, lsr #1
003ce76c: andeq    r0, r0, r4, lsl #17
003ce770: subeq    r7, pc, r0, asr #5
003ce774: subeq    r5, pc, ip, ror #10
003ce778: subeq    r7, pc, ip, ror #3
003ce77c: ldrdeq   sp, lr, [pc], #-0x50
003ce780: umaaleq  r0, pc, r0, r1
003ce784: subeq    r6, pc, ip, lsr #31
003ce788: subeq    r7, pc, r8, lsl #2
003ce78c: strdeq   r7, r8, [pc], #-0xc
003ce790: ldrdeq   r0, r1, [pc], #-0x18
003ce794: subeq    sp, pc, ip, lsl #7
003ce798: subeq    r6, pc, r0, ror sp
003ce79c: subeq    pc, lr, ip, asr #30
003ce7a0: subeq    r6, pc, r4, asr #29
003ce7a4: strheq   r6, [pc], #-0xe8
003ce7a8: umaaleq  pc, lr, r4, pc
003ce7ac: andeq    r3, r0, r0, asr #19
003ce7b0: andeq    r1, r0, r0, asr #19
