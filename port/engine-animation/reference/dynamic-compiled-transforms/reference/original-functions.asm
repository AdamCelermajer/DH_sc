
# _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKNS0_8SChannelEPPv
0061c6bc: str      lr, [sp, #-4]!
0061c6c0: mov      ip, r1
0061c6c4: ldr      lr, [ip, #8]
0061c6c8: sub      sp, sp, #0xc
0061c6cc: ldr      r1, [r1, #4]
0061c6d0: mov      r3, r2
0061c6d4: add      ip, ip, #0xc
0061c6d8: mov      r2, lr
0061c6dc: str      ip, [sp]
0061c6e0: bl       #0x61c2f8
0061c6e4: add      sp, sp, #0xc
0061c6e8: ldm      sp!, {pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIfEEfLi4ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
00613294: push     {r4, r5, r6, lr}
00613298: mov      r4, r1
0061329c: sub      sp, sp, #8
006132a0: mov      r1, #0
006132a4: mov      r5, r3
006132a8: bl       #0x669e24
006132ac: mov      r1, r5
006132b0: mov      r6, r0
006132b4: mov      r0, #0x3f800000
006132b8: bl       #0x30e3ac
006132bc: str      r5, [sp, #4]
006132c0: str      r0, [sp]
006132c4: ldr      r0, [r6, #4]
006132c8: ldr      r3, [sp, #0x18]
006132cc: mov      r1, sp
006132d0: add      r0, r0, r4, lsl #4
006132d4: mov      r2, #2
006132d8: bl       #0x6130d4
006132dc: add      sp, sp, #8
006132e0: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada16CColladaDatabase12getAnimationEi
0060e35c: ldr      r3, [r0]
0060e360: ldr      r3, [r3, #0x24]
0060e364: ldr      r3, [r3, #0x20]
0060e368: ldr      r0, [r3, #0x28]
0060e36c: add      r0, r0, r1, lsl #5
0060e370: bx       lr

# _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryEj
0062fcf8: push     {r4, r5, lr}
0062fcfc: mov      r4, r0
0062fd00: ldr      r3, [r0, #0x24]
0062fd04: ldr      r0, [r0, #0x28]
0062fd08: ldr      r2, [pc, #0xb4]
0062fd0c: sub      sp, sp, #0x1c
0062fd10: rsb      r0, r3, r0
0062fd14: cmp      r1, r0, asr #3
0062fd18: mov      r5, r1
0062fd1c: add      r2, pc, r2
0062fd20: blo      #0x62fd64
0062fd24: ldr      r3, [pc, #0x9c]
0062fd28: ldr      ip, [r4, #0x68]
0062fd2c: ldr      r1, [r4, #0x6c]
0062fd30: ldr      r3, [r2, r3]
0062fd34: mov      r2, #0
0062fd38: str      r2, [r4, #0x68]
0062fd3c: str      r3, [r4, #0x6c]
0062fd40: add      r0, sp, #8
0062fd44: str      r3, [sp, #0x14]
0062fd48: str      ip, [sp, #8]
0062fd4c: str      r1, [sp, #0xc]
0062fd50: str      r2, [sp, #0x10]
0062fd54: bl       #0x619474
0062fd58: add      r0, sp, #0x10
0062fd5c: bl       #0x619474
0062fd60: ldr      r3, [r4, #0x24]
0062fd64: ldr      r1, [r3, r5, lsl #3]
0062fd68: add      r5, r3, r5, lsl #3
0062fd6c: str      r1, [sp]
0062fd70: ldr      r3, [r5, #4]
0062fd74: cmp      r1, #0
0062fd78: str      r3, [sp, #4]
0062fd7c: beq      #0x62fd94
0062fd80: ldr      r3, [r1, #4]
0062fd84: cmp      r3, #0
0062fd88: addne    r3, r3, #1
0062fd8c: strne    r3, [r1, #4]
0062fd90: ldrne    r1, [sp]
0062fd94: ldr      r0, [sp, #4]
0062fd98: ldr      r3, [r4, #0x6c]
0062fd9c: ldr      r2, [r4, #0x68]
0062fda0: str      r0, [r4, #0x6c]
0062fda4: str      r1, [r4, #0x68]
0062fda8: mov      r0, sp
0062fdac: stm      sp, {r2, r3}
0062fdb0: bl       #0x619474
0062fdb4: mov      r3, #1
0062fdb8: strb     r3, [r4, #0x70]
0062fdbc: add      sp, sp, #0x1c
0062fdc0: pop      {r4, r5, pc}
0062fdc4: eorseq   r4, r6, r4, ror sp
0062fdc8: andeq    r4, r0, r0, lsl r7

# _ZN6glitch7collada18ISceneNodeAnimatorC1Ev
00669828: push     {r4, r5, r6, lr}
0066982c: ldr      r5, [pc, #0xb0]
00669830: ldr      r3, [pc, #0xb0]
00669834: ldr      r2, [pc, #0xb0]
00669838: add      r5, pc, r5
0066983c: ldr      r6, [r5, r3]
00669840: ldr      r2, [r5, r2]
00669844: ldr      r3, [pc, #0xa4]
00669848: ldr      r1, [r6, #8]
0066984c: mov      ip, #1
00669850: add      r2, r2, #8
00669854: str      ip, [r0, #0x28]
00669858: str      r1, [r0]
0066985c: str      r2, [r0, #0x24]
00669860: ldr      r3, [r5, r3]
00669864: ldr      r2, [r1, #-0xc]
00669868: ldr      r1, [r6, #0xc]
0066986c: add      r3, r3, #8
00669870: mov      r4, r0
00669874: str      r1, [r0, r2]
00669878: str      r3, [r0, #4]
0066987c: bl       #0x6a118c
00669880: ldr      r3, [r6, #4]
00669884: ldr      r2, [pc, #0x68]
00669888: ldr      r1, [pc, #0x68]
0066988c: str      r3, [r4]
00669890: ldr      r2, [r5, r2]
00669894: ldr      r1, [r5, r1]
00669898: ldr      r6, [r6, #0x10]
0066989c: ldr      r5, [r3, #-0xc]
006698a0: add      r0, r2, #0x9c
006698a4: mov      r3, #0
006698a8: add      ip, r2, #0xc
006698ac: add      r2, r2, #0xb8
006698b0: str      r6, [r4, r5]
006698b4: str      r0, [r4, #4]
006698b8: str      ip, [r4]
006698bc: str      r2, [r4, #0x24]
006698c0: str      r1, [r4, #0x1c]
006698c4: str      r3, [r4, #0x20]
006698c8: str      r3, [r4, #8]
006698cc: str      r3, [r4, #0xc]
006698d0: str      r3, [r4, #0x10]
006698d4: str      r3, [r4, #0x14]
006698d8: str      r3, [r4, #0x18]
006698dc: mov      r0, r4
006698e0: pop      {r4, r5, r6, pc}
006698e4: eorseq   fp, r2, r8, asr r2
006698e8: andeq    r2, r0, r0, lsr #29
006698ec: andeq    r2, r0, r4, asr #22
006698f0: andeq    r2, r0, ip, asr #14
006698f4: andeq    r4, r0, r4, asr #2
006698f8: andeq    r4, r0, ip, lsr #10

# _ZN6glitch7collada19CTimelineControllerC1Ev
00666e40: ldr      r1, [pc, #0xa4]
00666e44: ldr      r3, [pc, #0xa4]
00666e48: ldr      r2, [pc, #0xa4]
00666e4c: add      r1, pc, r1
00666e50: push     {r4, r5, r6, r7, r8}
00666e54: ldr      r4, [r1, r3]
00666e58: ldr      r2, [r1, r2]
00666e5c: mov      r5, #1
00666e60: ldr      ip, [r4, #8]
00666e64: add      r2, r2, #8
00666e68: str      r2, [r0, #0x40]
00666e6c: str      ip, [r0]
00666e70: str      r5, [r0, #0x44]
00666e74: ldr      r7, [ip, #-0xc]
00666e78: ldr      r6, [r4, #4]
00666e7c: ldr      r8, [r4, #0xc]
00666e80: ldr      ip, [pc, #0x70]
00666e84: mov      r2, #0
00666e88: str      r8, [r0, r7]
00666e8c: ldr      ip, [r1, ip]
00666e90: str      r6, [r0]
00666e94: str      r2, [r0, #4]
00666e98: ldr      r7, [r6, #-0xc]
00666e9c: ldr      r8, [r4, #0x10]
00666ea0: add      r6, ip, #0x70
00666ea4: add      ip, ip, #0xc
00666ea8: str      r8, [r0, r7]
00666eac: mov      r4, #0
00666eb0: str      ip, [r0]
00666eb4: mov      ip, #0x3f800000
00666eb8: strb     r2, [r0, #0x3d]
00666ebc: str      r6, [r0, #0x40]
00666ec0: strb     r5, [r0, #0x18]
00666ec4: str      r4, [r0, #0x2c]
00666ec8: str      ip, [r0, #0x30]
00666ecc: str      r2, [r0, #8]
00666ed0: str      r4, [r0, #0x20]
00666ed4: str      r4, [r0, #0x24]
00666ed8: str      r2, [r0, #0x34]
00666edc: str      r2, [r0, #0x38]
00666ee0: strb     r2, [r0, #0x3c]
00666ee4: pop      {r4, r5, r6, r7, r8}
00666ee8: bx       lr
00666eec: eorseq   sp, r2, r4, asr #24
00666ef0: andeq    r2, r0, r0, ror #28
00666ef4: andeq    r2, r0, r4, asr #22
00666ef8: andeq    r2, r0, r4, lsl pc

# _ZN6glitch7collada20CDynamicAnimationSet26setDefaultAnimationLibraryERKNS0_16CColladaDatabaseE
0062fc90: push     {r4, lr}
0062fc94: ldr      r3, [r1]
0062fc98: ldr      r2, [r1, #4]
0062fc9c: sub      sp, sp, #8
0062fca0: cmp      r3, #0
0062fca4: mov      r4, r0
0062fca8: str      r2, [sp, #4]
0062fcac: str      r3, [sp]
0062fcb0: beq      #0x62fcc8
0062fcb4: ldr      r2, [r3, #4]
0062fcb8: cmp      r2, #0
0062fcbc: addne    r2, r2, #1
0062fcc0: strne    r2, [r3, #4]
0062fcc4: ldrne    r3, [sp]
0062fcc8: ldr      r0, [sp, #4]
0062fccc: ldr      r1, [r4, #0x68]
0062fcd0: ldr      r2, [r4, #0x6c]
0062fcd4: str      r3, [r4, #0x68]
0062fcd8: str      r0, [r4, #0x6c]
0062fcdc: mov      r0, sp
0062fce0: stm      sp, {r1, r2}
0062fce4: bl       #0x619474
0062fce8: mov      r3, #1
0062fcec: strb     r3, [r4, #0x70]
0062fcf0: add      sp, sp, #8
0062fcf4: pop      {r4, pc}

# _ZN14AnimSetManager15AddTemplateAnimEii
00476398: push     {r4, r5, r6, r7, r8, lr}
0047639c: sub      sp, sp, #0x10
004763a0: mov      r5, r0
004763a4: str      r1, [sp, #4]
004763a8: mov      r7, r2
004763ac: bl       #0x475404
004763b0: ldr      r6, [pc, #0xa0]
004763b4: cmp      r0, #0
004763b8: addne    r5, r5, #4
004763bc: add      r6, pc, r6
004763c0: addne    r4, sp, #4
004763c4: bne      #0x476408
004763c8: ldr      r3, [sp, #4]
004763cc: cmp      r3, #0
004763d0: blt      #0x476450
004763d4: add      r5, r5, #4
004763d8: add      r4, sp, #4
004763dc: mov      r1, r4
004763e0: mov      r0, r5
004763e4: bl       #0x476058
004763e8: mov      r8, r0
004763ec: bl       #0x364ca0
004763f0: ldr      r3, [pc, #0x64]
004763f4: ldr      r3, [r6, r3]
004763f8: ldrb     r3, [r3]
004763fc: cmp      r3, #0
00476400: movne    r3, #1
00476404: strbne   r3, [r8, #0x3c]
00476408: mov      r1, r4
0047640c: mov      r0, r5
00476410: bl       #0x476058
00476414: mov      r1, r7
00476418: mov      r5, r0
0047641c: bl       #0x3659ec
00476420: ldr      r3, [pc, #0x38]
00476424: ldr      r5, [r5, #0x20]
00476428: add      r4, sp, #8
0047642c: ldr      r1, [r0, #0x14]
00476430: ldr      r2, [r6, r3]
00476434: mov      r0, r4
00476438: bl       #0x60f25c
0047643c: mov      r0, r5
00476440: mov      r1, r4
00476444: bl       #0x62fc90
00476448: mov      r0, r4
0047644c: bl       #0x619474
00476450: add      sp, sp, #0x10
00476454: pop      {r4, r5, r6, r7, r8, pc}
00476458: ldrsbeq  lr, [r1], #-0x64
0047645c: andeq    r4, r0, r8, lsr #9
00476460: andeq    r4, r0, r0, lsl r7

# _ZNK6glitch7collada18SAnimationAccessor8getValueEiPvRib
0066a1a8: push     {r4, r5, r6, r7, r8, lr}
0066a1ac: sub      sp, sp, #8
0066a1b0: mov      r7, r1
0066a1b4: mov      r6, r2
0066a1b8: mov      r5, r3
0066a1bc: mov      r8, r0
0066a1c0: ldrb     r4, [sp, #0x20]
0066a1c4: bl       #0x66a004
0066a1c8: mov      r1, r8
0066a1cc: ldr      ip, [r0]
0066a1d0: mov      r2, r7
0066a1d4: mov      r3, r6
0066a1d8: str      r5, [sp]
0066a1dc: str      r4, [sp, #4]
0066a1e0: mov      lr, pc
0066a1e4: ldr      pc, [ip, #0x68]
0066a1e8: add      sp, sp, #8
0066a1ec: pop      {r4, r5, r6, r7, r8, pc}

# _ZN11AnimatorSetC1ERKN5boost13intrusive_ptrI12AnimationSetEE
003676b8: push     {r4, r5, r6, lr}
003676bc: ldr      r5, [pc, #0xbc]
003676c0: ldr      r3, [pc, #0xbc]
003676c4: mov      r2, #1
003676c8: add      r5, pc, r5
003676cc: ldr      r3, [r5, r3]
003676d0: str      r2, [r0, #0xa0]
003676d4: sub      sp, sp, #8
003676d8: add      r3, r3, #8
003676dc: str      r3, [r0, #0x9c]
003676e0: ldr      r3, [r1]
003676e4: mov      r6, r1
003676e8: ldr      r1, [pc, #0x98]
003676ec: ldr      r3, [r3, #0x20]
003676f0: mov      r4, r0
003676f4: ldr      r1, [r5, r1]
003676f8: cmp      r3, #0
003676fc: str      r3, [sp, #4]
00367700: ldrne    r2, [r3, #4]
00367704: add      r1, r1, #4
00367708: addne    r2, r2, #1
0036770c: strne    r2, [r3, #4]
00367710: add      r2, sp, #4
00367714: bl       #0x660ce4
00367718: ldr      r0, [sp, #4]
0036771c: cmp      r0, #0
00367720: beq      #0x367728
00367724: bl       #0x31d584
00367728: ldr      r3, [pc, #0x5c]
0036772c: add      r0, r4, #0x58
00367730: mov      r1, r4
00367734: ldr      r3, [r5, r3]
00367738: add      r2, r3, #0xa4
0036773c: add      ip, r3, #0xc
00367740: add      r3, r3, #0xc0
00367744: str      r2, [r4, #4]
00367748: str      r3, [r4, #0x9c]
0036774c: str      ip, [r4]
00367750: bl       #0x364398
00367754: ldr      r3, [r6]
00367758: mov      r0, r4
0036775c: cmp      r3, #0
00367760: str      r3, [r4, #0x94]
00367764: ldrne    r2, [r3, #4]
00367768: addne    r2, r2, #1
0036776c: strne    r2, [r3, #4]
00367770: mov      r3, #0
00367774: str      r3, [r4, #0x98]
00367778: add      sp, sp, #8
0036777c: pop      {r4, r5, r6, pc}
00367780: rsbeq    sp, r2, r8, asr #7
00367784: andeq    r2, r0, r4, asr #22
00367788: ldrdeq   r1, r2, [r0], -r4
0036778c: andeq    r1, r0, ip, asr #20

# _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
00628850: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00628854: mov      r7, r1
00628858: sub      sp, sp, #8
0062885c: mov      r1, #0
00628860: mov      r4, r3
00628864: ldr      r6, [sp, #0x28]
00628868: bl       #0x669e24
0062886c: mov      r1, r4
00628870: mov      r8, r0
00628874: mov      r0, #0x3f800000
00628878: bl       #0x30e3ac
0062887c: str      r4, [sp, #4]
00628880: str      r0, [sp]
00628884: mov      r3, #0xc
00628888: mul      r7, r3, r7
0062888c: ldr      r3, [r8, #4]
00628890: mov      r5, r0
00628894: ldr      r1, [r3, r7]
00628898: add      r7, r3, r7
0062889c: bl       #0x30ed6c
006288a0: mov      r1, #0
006288a4: bl       #0x30eba4
006288a8: ldr      r1, [r7, #4]
006288ac: mov      r8, r0
006288b0: mov      r0, r5
006288b4: bl       #0x30ed6c
006288b8: mov      r1, #0
006288bc: bl       #0x30eba4
006288c0: add      r7, r7, #4
006288c4: ldr      r1, [r7, #4]
006288c8: mov      sl, r0
006288cc: mov      r0, r5
006288d0: bl       #0x30ed6c
006288d4: mov      r1, #0
006288d8: bl       #0x30eba4
006288dc: add      r5, r7, #4
006288e0: add      sb, r5, #4
006288e4: ldr      r1, [sb, #4]
006288e8: mov      r7, r0
006288ec: mov      r0, r4
006288f0: bl       #0x30ed6c
006288f4: mov      r1, r0
006288f8: mov      r0, sl
006288fc: bl       #0x30eba4
00628900: add      sb, sb, #4
00628904: ldr      r1, [sb, #4]
00628908: mov      sl, r0
0062890c: mov      r0, r4
00628910: bl       #0x30ed6c
00628914: mov      r1, r7
00628918: bl       #0x30eba4
0062891c: ldr      r1, [r5, #4]
00628920: mov      r7, r0
00628924: mov      r0, r4
00628928: bl       #0x30ed6c
0062892c: mov      r1, r0
00628930: mov      r0, r8
00628934: bl       #0x30eba4
00628938: mov      r3, r6
0062893c: str      r0, [r3], #4
00628940: str      sl, [r6, #4]
00628944: str      r7, [r3, #4]
00628948: add      sp, sp, #8
0062894c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch7collada21CSceneNodeAnimatorSetC2Ev
00662c90: push     {r4, r5, r6, lr}
00662c94: mov      r6, r1
00662c98: ldr      r5, [pc, #0x68]
00662c9c: add      r1, r1, #4
00662ca0: mov      r4, r0
00662ca4: bl       #0x6698fc
00662ca8: ldr      r2, [r6]
00662cac: ldr      r3, [pc, #0x58]
00662cb0: add      r5, pc, r5
00662cb4: str      r2, [r4]
00662cb8: ldr      r3, [r5, r3]
00662cbc: ldr      r1, [r2, #-0xc]
00662cc0: ldr      r0, [r6, #0x1c]
00662cc4: add      r2, r3, #0xa4
00662cc8: mov      r3, #0
00662ccc: str      r0, [r4, r1]
00662cd0: str      r2, [r4, #4]
00662cd4: str      r3, [r4, #0x54]
00662cd8: str      r3, [r4, #0x24]
00662cdc: str      r3, [r4, #0x28]
00662ce0: str      r3, [r4, #0x2c]
00662ce4: str      r3, [r4, #0x30]
00662ce8: str      r3, [r4, #0x34]
00662cec: str      r3, [r4, #0x38]
00662cf0: str      r3, [r4, #0x3c]
00662cf4: str      r3, [r4, #0x40]
00662cf8: str      r3, [r4, #0x44]
00662cfc: str      r3, [r4, #0x48]
00662d00: mov      r0, r4
00662d04: pop      {r4, r5, r6, pc}
00662d08: eorseq   r1, r3, r0, ror #27
00662d0c: andeq    r2, r0, r0, lsl #12

# _ZN6glitch7collada13CAnimationSet19addAnimationLibraryEPKc
006600fc: push     {r4, r5, r6, r7, r8, lr}
00660100: ldr      r4, [pc, #0xbc]
00660104: ldr      r6, [pc, #0xbc]
00660108: mov      r2, #0
0066010c: add      r4, pc, r4
00660110: ldr      r7, [r4, r6]
00660114: sub      sp, sp, #8
00660118: mov      r5, r0
0066011c: mov      r3, r2
00660120: ldr      r0, [r7]
00660124: mov      r8, r1
00660128: bl       #0x65ac5c
0066012c: cmp      r0, #0
00660130: beq      #0x66019c
00660134: ldr      r2, [pc, #0x90]
00660138: ldr      r3, [r7]
0066013c: mov      r1, #0
00660140: ldr      r2, [r4, r2]
00660144: ldrb     r8, [r3, #0x28]
00660148: strb     r1, [r3, #0x28]
0066014c: stm      sp, {r0, r2}
00660150: ldr      r3, [r0, #4]
00660154: mov      r7, sp
00660158: cmp      r3, r1
0066015c: addne    r3, r3, #1
00660160: strne    r3, [r0, #4]
00660164: ldr      r3, [r5]
00660168: mov      r0, r5
0066016c: mov      r1, sp
00660170: mov      lr, pc
00660174: ldr      pc, [r3, #0x2c]
00660178: mov      r5, r0
0066017c: mov      r0, sp
00660180: bl       #0x619474
00660184: ldr      r3, [r4, r6]
00660188: ldr      r3, [r3]
0066018c: strb     r8, [r3, #0x28]
00660190: mov      r0, r5
00660194: add      sp, sp, #8
00660198: pop      {r4, r5, r6, r7, r8, pc}
0066019c: ldr      r0, [pc, #0x2c]
006601a0: mov      r1, r8
006601a4: add      r0, pc, r0
006601a8: bl       #0x30de84
006601ac: ldr      r3, [r5, #0x24]
006601b0: ldr      r5, [r5, #0x28]
006601b4: rsb      r5, r3, r5
006601b8: asr      r5, r5, #3
006601bc: sub      r5, r5, #1
006601c0: b        #0x660190
006601c4: eorseq   r4, r3, r4, lsl #19
006601c8: andeq    r4, r0, r8, asr #8
006601cc: andeq    r4, r0, r0, lsl r7

# _ZN6glitch7collada21CSceneNodeAnimatorSet19setCurrentAnimationEi
0065f8c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0065f8cc: mov      r4, r0
0065f8d0: mov      r5, r1
0065f8d4: bl       #0x65f0fc
0065f8d8: ldr      r3, [r4, #0x24]
0065f8dc: str      r0, [r4, #0x14]
0065f8e0: mov      r1, r5
0065f8e4: ldr      r2, [r3, #0x3c]
0065f8e8: mov      r0, r3
0065f8ec: str      r5, [r4, #0x50]
0065f8f0: mul      r3, r2, r5
0065f8f4: str      r3, [r4, #0x4c]
0065f8f8: bl       #0x65f0b4
0065f8fc: bl       #0x60e334
0065f900: ldr      r3, [r4]
0065f904: mov      r6, r0
0065f908: mov      r0, r4
0065f90c: mov      lr, pc
0065f910: ldr      pc, [r3, #0x44]
0065f914: cmp      r0, #0
0065f918: beq      #0x65f9fc
0065f91c: ldr      r8, [r6]
0065f920: cmp      r8, #0
0065f924: beq      #0x65f968
0065f928: ldr      r3, [r4]
0065f92c: mov      r0, r4
0065f930: mov      lr, pc
0065f934: ldr      pc, [r3, #0x44]
0065f938: str      r6, [r0, #0x34]
0065f93c: ldr      r2, [r6]
0065f940: cmp      r2, #0
0065f944: moveq    r1, #1
0065f948: streq    r1, [r0, #0x14]
0065f94c: streq    r2, [r0, #0x10]
0065f950: beq      #0x65f9d4
0065f954: ldr      r3, [r0]
0065f958: mov      r1, #0
0065f95c: mov      lr, pc
0065f960: ldr      pc, [r3, #0x10]
0065f964: b        #0x65f9d4
0065f968: ldr      r3, [r4]
0065f96c: mov      r0, r4
0065f970: mov      lr, pc
0065f974: ldr      pc, [r3, #0x44]
0065f978: mov      r7, #1
0065f97c: str      r8, [r0, #0x10]
0065f980: str      r8, [r0, #0x34]
0065f984: str      r7, [r0, #0x14]
0065f988: ldr      r3, [r4]
0065f98c: mov      r0, r4
0065f990: mov      lr, pc
0065f994: ldr      pc, [r3, #0x44]
0065f998: ldr      r3, [r0]
0065f99c: mov      r8, r0
0065f9a0: mov      r1, r5
0065f9a4: mov      r0, r4
0065f9a8: ldr      r6, [r3, #0x50]
0065f9ac: bl       #0x65f104
0065f9b0: mov      r1, r5
0065f9b4: mov      sl, r0
0065f9b8: mov      r0, r4
0065f9bc: bl       #0x65f10c
0065f9c0: mov      r1, sl
0065f9c4: mov      r2, r0
0065f9c8: mov      r3, r7
0065f9cc: mov      r0, r8
0065f9d0: blx      r6
0065f9d4: mov      r1, r5
0065f9d8: ldr      r0, [r4, #0x24]
0065f9dc: bl       #0x65f0b4
0065f9e0: ldr      r3, [r0]
0065f9e4: mov      r0, r4
0065f9e8: ldr      r3, [r3, #0x24]
0065f9ec: ldr      r3, [r3, #0x20]
0065f9f0: ldr      r1, [r3, #0x2c]
0065f9f4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0065f9f8: b        #0x60fab8
0065f9fc: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch7collada13CAnimationSet15CompileInternalEv
006605ac: push     {r4, r5, r6, lr}
006605b0: ldr      r3, [r0, #0x24]
006605b4: ldr      r1, [r0, #0x28]
006605b8: add      r6, r0, #0x40
006605bc: mov      r4, r0
006605c0: rsb      r1, r3, r1
006605c4: sub      sp, sp, #0x10
006605c8: mov      r0, r6
006605cc: asr      r1, r1, #3
006605d0: bl       #0x65fe8c
006605d4: ldr      r3, [r4, #0x24]
006605d8: ldr      r1, [r4, #0x28]
006605dc: add      r2, sp, #0x10
006605e0: mov      r5, #0
006605e4: rsb      r1, r3, r1
006605e8: str      r5, [r2, #-4]!
006605ec: mov      r0, r6
006605f0: asr      r1, r1, #3
006605f4: bl       #0x660568
006605f8: ldr      r3, [r4, #0x24]
006605fc: ldr      r1, [r4, #0x28]
00660600: add      r6, r4, #0x4c
00660604: mov      r0, r6
00660608: rsb      r1, r3, r1
0066060c: asr      r1, r1, #3
00660610: bl       #0x65fe8c
00660614: ldr      r3, [r4, #0x24]
00660618: ldr      r1, [r4, #0x28]
0066061c: add      r2, sp, #0x10
00660620: str      r5, [r2, #-8]!
00660624: rsb      r1, r3, r1
00660628: mov      r0, r6
0066062c: asr      r1, r1, #3
00660630: bl       #0x660568
00660634: ldr      r3, [r4, #0x24]
00660638: ldr      r1, [r4, #0x28]
0066063c: add      r6, r4, #0x58
00660640: mov      r0, r6
00660644: rsb      r1, r3, r1
00660648: asr      r1, r1, #3
0066064c: bl       #0x65fe8c
00660650: ldr      r3, [r4, #0x24]
00660654: ldr      r1, [r4, #0x28]
00660658: add      r2, sp, #0x10
0066065c: str      r5, [r2, #-0xc]!
00660660: rsb      r1, r3, r1
00660664: mov      r0, r6
00660668: asr      r1, r1, #3
0066066c: bl       #0x660568
00660670: ldr      r2, [r4, #0x28]
00660674: ldr      r3, [r4, #0x24]
00660678: rsb      r3, r3, r2
0066067c: lsrs     r3, r3, #3
00660680: beq      #0x660708
00660684: mvn      ip, #0x80000000
00660688: mov      r0, #0x80000000
0066068c: ldr      r3, [r4, #0x40]
00660690: str      ip, [r3, r5, lsl #2]
00660694: ldr      r3, [r4, #0x4c]
00660698: str      r0, [r3, r5, lsl #2]
0066069c: ldr      r2, [r4, #0x24]
006606a0: ldr      r3, [r4, #0x40]
006606a4: ldr      r2, [r2, r5, lsl #3]
006606a8: ldr      r2, [r2, #0x24]
006606ac: ldr      r2, [r2, #0x20]
006606b0: ldr      r2, [r2, #0x1c]
006606b4: str      r2, [r3, r5, lsl #2]
006606b8: ldr      r2, [r4, #0x24]
006606bc: ldr      r3, [r4, #0x4c]
006606c0: ldr      r2, [r2, r5, lsl #3]
006606c4: ldr      r2, [r2, #0x24]
006606c8: ldr      r2, [r2, #0x20]
006606cc: ldr      r2, [r2, #0x20]
006606d0: str      r2, [r3, r5, lsl #2]
006606d4: ldr      r1, [r4, #0x4c]
006606d8: ldr      r2, [r4, #0x40]
006606dc: ldr      r3, [r4, #0x58]
006606e0: ldr      r1, [r1, r5, lsl #2]
006606e4: ldr      r2, [r2, r5, lsl #2]
006606e8: rsb      r2, r2, r1
006606ec: str      r2, [r3, r5, lsl #2]
006606f0: ldr      r2, [r4, #0x28]
006606f4: ldr      r3, [r4, #0x24]
006606f8: add      r5, r5, #1
006606fc: rsb      r3, r3, r2
00660700: cmp      r5, r3, asr #3
00660704: blo      #0x66068c
00660708: add      sp, sp, #0x10
0066070c: pop      {r4, r5, r6, pc}

# _ZN11AnimatorSet12SetCallbacksEPFvPN6glitch5scene19ITimelineControllerEPvES4_PFvRKNS0_7collada15STriggeredEventES4_ES4_
00367510: push     {r4, r5, r6, r7, r8, lr}
00367514: mov      r4, r0
00367518: ldr      r0, [r0, #0x18]
0036751c: ldr      ip, [sp, #0x18]
00367520: str      r3, [r4, #0x1c]
00367524: cmp      r0, #0
00367528: str      ip, [r4, #0x20]
0036752c: strne    ip, [r0, #0xc]
00367530: strne    r3, [r0, #8]
00367534: ldr      r3, [r4]
00367538: mov      r0, r4
0036753c: mov      r7, r1
00367540: mov      r6, r2
00367544: mov      lr, pc
00367548: ldr      pc, [r3, #0x44]
0036754c: ldr      r5, [pc, #0x2c]
00367550: cmp      r0, #0
00367554: add      r5, pc, r5
00367558: beq      #0x36756c
0036755c: ldr      r3, [pc, #0x20]
00367560: str      r4, [r0, #0xc]
00367564: ldr      r3, [r5, r3]
00367568: str      r3, [r0, #8]
0036756c: add      r0, r4, #0x58
00367570: mov      r1, r7
00367574: mov      r2, r6
00367578: pop      {r4, r5, r6, r7, r8, lr}
0036757c: b        #0x364400
00367580: rsbeq    sp, r2, ip, lsr r5
00367584: strheq   r0, [r0], -r0

# _ZN12AnimationSet23_UpdateAnimationIndicesEv
00364af4: push     {r4, r5, r6, lr}
00364af8: ldr      r4, [r0, #0x10]
00364afc: mov      r5, r0
00364b00: add      r6, r0, #8
00364b04: cmp      r6, r4
00364b08: beq      #0x364b48
00364b0c: ldr      r0, [r5, #0x20]
00364b10: add      r1, r4, #0x2c
00364b14: bl       #0x62dbb8
00364b18: ldr      r3, [r4, #0xc]
00364b1c: str      r0, [r4, #0x34]
00364b20: cmp      r3, #0
00364b24: bne      #0x364b30
00364b28: b        #0x364b4c
00364b2c: mov      r3, r2
00364b30: ldr      r2, [r3, #8]
00364b34: cmp      r2, #0
00364b38: bne      #0x364b2c
00364b3c: mov      r4, r3
00364b40: cmp      r6, r4
00364b44: bne      #0x364b0c
00364b48: pop      {r4, r5, r6, pc}
00364b4c: ldr      r2, [r4, #4]
00364b50: ldr      r1, [r2, #0xc]
00364b54: cmp      r4, r1
00364b58: bne      #0x364b74
00364b5c: mov      r4, r2
00364b60: ldr      r2, [r2, #4]
00364b64: ldr      r3, [r2, #0xc]
00364b68: cmp      r3, r4
00364b6c: beq      #0x364b5c
00364b70: ldr      r3, [r4, #0xc]
00364b74: cmp      r3, r2
00364b78: movne    r4, r2
00364b7c: b        #0x364b04

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIiLi1000EEEbRKNS_3res6vectorIiEEiRii
0066a368: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a36c: mov      r0, r2
0066a370: sub      sp, sp, #0xc
0066a374: mov      r4, r1
0066a378: mov      fp, r3
0066a37c: bl       #0x30e964
0066a380: ldr      r5, [r4]
0066a384: ldr      r8, [sp, #0x30]
0066a388: ldr      r7, [r4, #4]
0066a38c: sub      r5, r5, #1
0066a390: bic      r8, r8, r8, asr #31
0066a394: cmp      r8, r5
0066a398: movge    r8, r5
0066a39c: mov      r6, r0
0066a3a0: ldr      r0, [r7, r8, lsl #2]
0066a3a4: bl       #0x30e964
0066a3a8: mov      r1, r6
0066a3ac: bl       #0x30e2f8
0066a3b0: cmp      r0, #0
0066a3b4: mov      sl, #0
0066a3b8: movne    sl, #1
0066a3bc: uxtb     sl, sl
0066a3c0: cmp      sl, #0
0066a3c4: lsl      r2, r8, #2
0066a3c8: beq      #0x66a488
0066a3cc: cmp      r8, #0
0066a3d0: subgt    r8, r8, #1
0066a3d4: ble      #0x66a488
0066a3d8: cmp      r8, r5
0066a3dc: bge      #0x66a58c
0066a3e0: ldr      r0, [r7, r8, lsl #2]
0066a3e4: bl       #0x30e964
0066a3e8: mov      r1, r0
0066a3ec: mov      r0, r6
0066a3f0: bl       #0x30e70c
0066a3f4: cmp      r0, #0
0066a3f8: mov      sl, #0
0066a3fc: movne    sl, #1
0066a400: lsl      r3, r8, #2
0066a404: uxtb     sl, sl
0066a408: mov      sb, r8
0066a40c: cmp      sl, #0
0066a410: beq      #0x66a564
0066a414: cmp      r5, #0
0066a418: ble      #0x66a450
0066a41c: mov      sl, #1
0066a420: add      r8, sl, r5
0066a424: asr      r8, r8, #1
0066a428: ldr      r0, [r7, r8, lsl #2]
0066a42c: bl       #0x30e964
0066a430: mov      r1, r0
0066a434: mov      r0, r6
0066a438: bl       #0x30e70c
0066a43c: cmp      r0, #0
0066a440: subne    r5, r8, #1
0066a444: addeq    sl, r8, #1
0066a448: cmp      sl, r5
0066a44c: ble      #0x66a420
0066a450: str      r5, [fp]
0066a454: ldr      r3, [r4, #4]
0066a458: ldr      r0, [r3, r5, lsl #2]
0066a45c: bl       #0x30e964
0066a460: mov      r1, r0
0066a464: mov      r0, r6
0066a468: bl       #0x30df8c
0066a46c: cmp      r0, #0
0066a470: bne      #0x66a55c
0066a474: ldr      r0, [r4]
0066a478: sub      r0, r0, #1
0066a47c: subs     r0, r0, r5
0066a480: movne    r0, #1
0066a484: b        #0x66a554
0066a488: cmp      r5, r8
0066a48c: ble      #0x66a510
0066a490: add      sb, r8, #1
0066a494: ldr      r0, [r7, sb, lsl #2]
0066a498: str      r2, [sp]
0066a49c: bl       #0x30e964
0066a4a0: mov      r1, r6
0066a4a4: str      r0, [sp, #4]
0066a4a8: bl       #0x30e70c
0066a4ac: ldr      r2, [sp]
0066a4b0: lsl      r3, sb, #2
0066a4b4: cmp      r0, #0
0066a4b8: mov      r1, r3
0066a4bc: moveq    sb, r8
0066a4c0: moveq    r3, r2
0066a4c4: beq      #0x66a40c
0066a4c8: cmp      r5, sb
0066a4cc: ble      #0x66a518
0066a4d0: add      r8, sb, #1
0066a4d4: ldr      r0, [r7, r8, lsl #2]
0066a4d8: str      r3, [sp]
0066a4dc: bl       #0x30e964
0066a4e0: mov      r1, r6
0066a4e4: bl       #0x30e70c
0066a4e8: subs     sl, r0, #0
0066a4ec: bne      #0x66a3d8
0066a4f0: ldr      r1, [sp, #4]
0066a4f4: mov      r0, r6
0066a4f8: bl       #0x30e70c
0066a4fc: cmp      r0, #0
0066a500: movne    sl, #1
0066a504: ldr      r3, [sp]
0066a508: uxtb     sl, sl
0066a50c: b        #0x66a40c
0066a510: mov      sb, r8
0066a514: lsl      r1, r8, #2
0066a518: mov      r3, r1
0066a51c: mov      r8, sb
0066a520: str      r8, [fp]
0066a524: ldr      r2, [r4, #4]
0066a528: ldr      r0, [r2, r3]
0066a52c: bl       #0x30e964
0066a530: mov      r1, r0
0066a534: mov      r0, r6
0066a538: bl       #0x30df8c
0066a53c: cmp      r0, #0
0066a540: bne      #0x66a55c
0066a544: ldr      r0, [r4]
0066a548: sub      r0, r0, #1
0066a54c: subs     r0, r0, r8
0066a550: movne    r0, #1
0066a554: add      sp, sp, #0xc
0066a558: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a55c: mov      r0, #0
0066a560: b        #0x66a554
0066a564: add      r2, sb, #1
0066a568: ldr      r0, [r7, r2, lsl #2]
0066a56c: str      r3, [sp]
0066a570: bl       #0x30e964
0066a574: mov      r1, r6
0066a578: bl       #0x30e70c
0066a57c: cmp      r0, #0
0066a580: ldr      r3, [sp]
0066a584: bne      #0x66a414
0066a588: b        #0x66a51c
0066a58c: lsl      r3, r8, #2
0066a590: b        #0x66a520

# _ZN6glitch7collada18getStringsInternalEPNS0_18ISceneNodeAnimator20E_INTERPOLATION_MODEE
00667c38: ldr      r0, [pc, #4]
00667c3c: add      r0, pc, r0
00667c40: bx       lr
00667c44: eorseq   r3, r3, r4, lsr #24

# _ZNSt6vectorIN6glitch7collada16CColladaDatabaseENS0_4core10SAllocatorIS2_LNS0_6memory13E_MEMORY_HINTE0EEEE9push_backERKS2_
0062ecac: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0062ecb0: ldmib    r0, {r3, r6}
0062ecb4: mov      r4, r0
0062ecb8: mov      r5, r1
0062ecbc: cmp      r3, r6
0062ecc0: beq      #0x62ecfc
0062ecc4: ldr      r2, [r1]
0062ecc8: str      r2, [r3]
0062eccc: ldr      r1, [r1, #4]
0062ecd0: cmp      r2, #0
0062ecd4: str      r1, [r3, #4]
0062ecd8: beq      #0x62ecec
0062ecdc: ldr      r3, [r2, #4]
0062ece0: cmp      r3, #0
0062ece4: addne    r3, r3, #1
0062ece8: strne    r3, [r2, #4]
0062ecec: ldr      r3, [r4, #4]
0062ecf0: add      r3, r3, #8
0062ecf4: str      r3, [r4, #4]
0062ecf8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0062ecfc: ldr      r3, [r0]
0062ed00: rsb      r3, r3, r6
0062ed04: asr      r3, r3, #3
0062ed08: cmp      r3, #1
0062ed0c: addhs    r7, r3, r3
0062ed10: addlo    r7, r3, #1
0062ed14: cmn      r7, #0xe0000001
0062ed18: bls      #0x62edfc
0062ed1c: mvn      r7, #7
0062ed20: mov      r0, r7
0062ed24: mov      r1, #0
0062ed28: bl       #0x310568
0062ed2c: ldr      lr, [r4]
0062ed30: mov      r8, r0
0062ed34: rsb      sl, lr, r6
0062ed38: asr      sl, sl, #3
0062ed3c: cmp      sl, #0
0062ed40: movle    sl, r0
0062ed44: ble      #0x62ed90
0062ed48: mov      r0, sl
0062ed4c: mov      ip, #0
0062ed50: mov      r1, lr
0062ed54: ldr      r3, [r1, ip]!
0062ed58: mov      r2, r8
0062ed5c: str      r3, [r2, ip]!
0062ed60: ldr      r1, [r1, #4]
0062ed64: cmp      r3, #0
0062ed68: add      ip, ip, #8
0062ed6c: str      r1, [r2, #4]
0062ed70: beq      #0x62ed84
0062ed74: ldr      r2, [r3, #4]
0062ed78: cmp      r2, #0
0062ed7c: addne    r2, r2, #1
0062ed80: strne    r2, [r3, #4]
0062ed84: subs     r0, r0, #1
0062ed88: bne      #0x62ed50
0062ed8c: add      sl, r8, sl, lsl #3
0062ed90: ldr      r3, [r5]
0062ed94: str      r3, [sl]
0062ed98: ldr      r2, [r5, #4]
0062ed9c: cmp      r3, #0
0062eda0: str      r2, [sl, #4]
0062eda4: beq      #0x62edb8
0062eda8: ldr      r2, [r3, #4]
0062edac: cmp      r2, #0
0062edb0: addne    r2, r2, #1
0062edb4: strne    r2, [r3, #4]
0062edb8: ldr      r5, [r4, #4]
0062edbc: ldr      r6, [r4]
0062edc0: add      sl, sl, #8
0062edc4: cmp      r5, r6
0062edc8: beq      #0x62ede4
0062edcc: sub      r5, r5, #8
0062edd0: mov      r0, r5
0062edd4: bl       #0x619474
0062edd8: cmp      r6, r5
0062eddc: bne      #0x62edcc
0062ede0: ldr      r6, [r4]
0062ede4: mov      r0, r6
0062ede8: add      r7, r8, r7
0062edec: bl       #0x310450
0062edf0: str      r7, [r4, #8]
0062edf4: stm      r4, {r8, sl}
0062edf8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0062edfc: cmp      r3, r7
0062ee00: lslls    r7, r7, #3
0062ee04: bls      #0x62ed20
0062ee08: b        #0x62ed1c

# _ZNK6glitch7collada16CColladaDatabase14getVisualSceneEi
0060e54c: ldr      r3, [r0]
0060e550: ldr      r3, [r3, #0x24]
0060e554: ldr      r3, [r3, #0x20]
0060e558: ldr      r2, [r3, #0x98]
0060e55c: cmp      r2, #0
0060e560: ldrgt    r0, [r3, #0x9c]
0060e564: movle    r0, #0
0060e568: addgt    r0, r0, r1, lsl #4
0060e56c: bx       lr

# _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationValueEiiPv
0065f7b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065f7b8: mov      r4, r0
0065f7bc: mov      sb, r2
0065f7c0: ldr      r2, [r4, #0xc]
0065f7c4: sub      sp, sp, #0x34
0065f7c8: mov      r5, r1
0065f7cc: ldr      r0, [r0, #0x24]
0065f7d0: ldr      r1, [r4, #0x50]
0065f7d4: mov      sl, r3
0065f7d8: str      r2, [sp, #0xc]
0065f7dc: bl       #0x65f0b4
0065f7e0: ldr      r6, [r4, #0x4c]
0065f7e4: ldr      r3, [r4, #0x24]
0065f7e8: mov      r1, #0xc
0065f7ec: ldr      r2, [r0]
0065f7f0: add      r6, r5, r6
0065f7f4: mul      r6, r1, r6
0065f7f8: ldr      r7, [r3, #0x30]
0065f7fc: ldr      r2, [r2, #0x24]
0065f800: add      r8, r7, r6
0065f804: ldr      r1, [r8, #4]
0065f808: ldr      r2, [r2, #0x20]
0065f80c: cmp      r1, #0
0065f810: ldr      fp, [r2, #0x14]
0065f814: beq      #0x65f844
0065f818: ldr      r3, [r3, #0x18]
0065f81c: ldr      r3, [r3, r5, lsl #2]
0065f820: mov      r0, r3
0065f824: ldr      r3, [r3]
0065f828: str      r1, [sp, #8]
0065f82c: mov      lr, pc
0065f830: ldr      pc, [r3, #8]
0065f834: ldr      r1, [sp, #8]
0065f838: mov      r2, r0
0065f83c: mov      r0, sl
0065f840: bl       #0x30e868
0065f844: ldr      r3, [r7, r6]
0065f848: cmp      r3, #2
0065f84c: bne      #0x65f8a4
0065f850: mov      r3, #0
0065f854: mov      r1, sb
0065f858: mov      r0, r4
0065f85c: strb     r3, [sp, #0x21]
0065f860: bl       #0x65f364
0065f864: ldr      r3, [r8, #8]
0065f868: str      r0, [sp, #0x28]
0065f86c: ldr      r2, [sp, #0xc]
0065f870: str      r3, [sp, #0x24]
0065f874: add      r3, sp, #0x14
0065f878: str      r3, [sp, #0x2c]
0065f87c: ldr      r3, [r4, #0x40]
0065f880: cmp      fp, #0
0065f884: mov      r1, sb
0065f888: addeq    r3, r3, r5, lsl #2
0065f88c: add      r0, sp, #0x24
0065f890: subs     ip, r2, #1
0065f894: movne    ip, #1
0065f898: mov      r2, sl
0065f89c: str      ip, [sp]
0065f8a0: bl       #0x66a1a8
0065f8a4: add      sp, sp, #0x34
0065f8a8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN14AnimSetManager7AddAnimEii
0047653c: push     {r4, r5, r6, r7, r8, sl, lr}
00476540: ldr      r4, [pc, #0x124]
00476544: ldr      r5, [pc, #0x124]
00476548: sub      sp, sp, #0x2c
0047654c: add      r4, pc, r4
00476550: ldr      r3, [r4, r5]
00476554: mov      r7, r0
00476558: str      r1, [sp, #4]
0047655c: ldr      r3, [r3]
00476560: mov      r8, r2
00476564: str      r3, [sp, #0x24]
00476568: bl       #0x475404
0047656c: cmp      r0, #0
00476570: addne    r7, r7, #4
00476574: addne    r6, sp, #4
00476578: bne      #0x4765bc
0047657c: ldr      r3, [sp, #4]
00476580: cmp      r3, #0
00476584: blt      #0x4765d4
00476588: add      r7, r7, #4
0047658c: add      r6, sp, #4
00476590: mov      r1, r6
00476594: mov      r0, r7
00476598: bl       #0x476058
0047659c: mov      sl, r0
004765a0: bl       #0x364ca0
004765a4: ldr      r3, [pc, #0xc8]
004765a8: ldr      r3, [r4, r3]
004765ac: ldrb     r3, [r3]
004765b0: cmp      r3, #0
004765b4: movne    r3, #1
004765b8: strbne   r3, [sl, #0x3c]
004765bc: mov      r0, r7
004765c0: mov      r1, r6
004765c4: bl       #0x476058
004765c8: ldrb     r3, [r0, #0x3c]
004765cc: cmp      r3, #0
004765d0: beq      #0x4765f0
004765d4: ldr      r3, [r4, r5]
004765d8: ldr      r2, [sp, #0x24]
004765dc: ldr      r3, [r3]
004765e0: cmp      r2, r3
004765e4: bne      #0x476668
004765e8: add      sp, sp, #0x2c
004765ec: pop      {r4, r5, r6, r7, r8, sl, pc}
004765f0: mov      r1, r8
004765f4: bl       #0x3659ec
004765f8: ldr      r3, [pc, #0x78]
004765fc: add      r6, sp, #0xc
00476600: ldr      r7, [r4, r3]
00476604: mov      r0, r7
00476608: bl       #0x337888
0047660c: ldr      r1, [pc, #0x68]
00476610: mov      r0, r6
00476614: str      r6, [sp, #0x1c]
00476618: add      r1, pc, r1
0047661c: add      r2, r1, #0x17
00476620: str      r6, [sp, #0x20]
00476624: bl       #0x3116e8
00476628: mov      r0, r7
0047662c: mov      r1, r6
00476630: bl       #0x337a88
00476634: ldr      r0, [sp, #0x20]
00476638: cmp      r0, r6
0047663c: beq      #0x4765d4
00476640: cmp      r0, #0
00476644: beq      #0x4765d4
00476648: ldr      r1, [sp, #0xc]
0047664c: rsb      r1, r0, r1
00476650: cmp      r1, #0x80
00476654: bhi      #0x476660
00476658: bl       #0x708f00
0047665c: b        #0x4765d4
00476660: bl       #0x310440
00476664: b        #0x4765d4
00476668: bl       #0x30e310
0047666c: subseq   lr, r1, r4, asr #10
00476670: andeq    r4, r0, ip, lsr #1
00476674: andeq    r4, r0, r8, lsr #9
00476678: andeq    r0, r0, r4, lsl #17
0047667c: subeq    r7, r5, r0, lsr r2

# _ZN12AnimationSet13CreateAnimSetEv
00364ca0: push     {r4, r5, r6, lr}
00364ca4: mov      r4, r0
00364ca8: ldr      r0, [r0, #0x20]
00364cac: cmp      r0, #0
00364cb0: beq      #0x364cc0
00364cb4: bl       #0x31d584
00364cb8: mov      r3, #0
00364cbc: str      r3, [r4, #0x20]
00364cc0: mov      r6, #1
00364cc4: mov      r1, #0
00364cc8: str      r6, [r4, #0x2c]
00364ccc: mov      r0, #0x80
00364cd0: bl       #0x310570
00364cd4: mov      r5, r0
00364cd8: bl       #0x3648c4
00364cdc: str      r5, [r4, #0x20]
00364ce0: mov      r0, r5
00364ce4: mov      r1, r6
00364ce8: ldr      r3, [r5]
00364cec: mov      lr, pc
00364cf0: ldr      pc, [r3, #0x1c]
00364cf4: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada20CDynamicAnimationSetC1Ev
003648c4: ldr      r1, [pc, #0xa4]
003648c8: ldr      r2, [pc, #0xa4]
003648cc: ldr      r3, [pc, #0xa4]
003648d0: add      r1, pc, r1
003648d4: ldr      r2, [r1, r2]
003648d8: push     {r4, r5}
003648dc: ldr      r4, [r1, r3]
003648e0: add      r5, r2, #8
003648e4: mov      ip, #1
003648e8: mov      r2, #0
003648ec: str      r2, [r0, #0x7c]
003648f0: str      r5, [r0]
003648f4: str      r4, [r0, #0x6c]
003648f8: strb     ip, [r0, #0x70]
003648fc: str      ip, [r0, #4]
00364900: str      r2, [r0, #8]
00364904: str      r2, [r0, #0xc]
00364908: str      r2, [r0, #0x10]
0036490c: str      r2, [r0, #0x14]
00364910: str      r2, [r0, #0x18]
00364914: str      r2, [r0, #0x1c]
00364918: str      r2, [r0, #0x20]
0036491c: str      r2, [r0, #0x24]
00364920: str      r2, [r0, #0x28]
00364924: str      r2, [r0, #0x2c]
00364928: str      r2, [r0, #0x30]
0036492c: str      r2, [r0, #0x34]
00364930: str      r2, [r0, #0x38]
00364934: str      r2, [r0, #0x40]
00364938: str      r2, [r0, #0x44]
0036493c: str      r2, [r0, #0x48]
00364940: str      r2, [r0, #0x4c]
00364944: str      r2, [r0, #0x50]
00364948: str      r2, [r0, #0x54]
0036494c: str      r2, [r0, #0x58]
00364950: str      r2, [r0, #0x5c]
00364954: str      r2, [r0, #0x60]
00364958: str      r2, [r0, #0x64]
0036495c: str      r2, [r0, #0x68]
00364960: str      r2, [r0, #0x74]
00364964: str      r2, [r0, #0x78]
00364968: pop      {r4, r5}
0036496c: bx       lr
00364970: rsbeq    r0, r3, r0, asr #3
00364974: andeq    r3, r0, r4, asr #15
00364978: andeq    r4, r0, r0, lsl r7

# _ZNK6glitch7collada16CColladaDatabase21getBlendableAnimationEPKNS0_8SChannelE
0061c1e0: subs     r2, r1, #0
0061c1e4: beq      #0x61c1f4
0061c1e8: ldrb     r3, [r2, #0xc]
0061c1ec: ldmib    r2, {r1, r2}
0061c1f0: b        #0x61c0c8
0061c1f4: mov      r0, r2
0061c1f8: bx       lr

# _ZN11AnimatorSetC2ERKN5boost13intrusive_ptrI12AnimationSetEE
00367790: push     {r4, r5, r6, r7, lr}
00367794: ldr      r3, [r2]
00367798: sub      sp, sp, #0xc
0036779c: mov      r7, r2
003677a0: ldr      r3, [r3, #0x20]
003677a4: mov      r6, r1
003677a8: add      r1, r1, #4
003677ac: cmp      r3, #0
003677b0: str      r3, [sp, #4]
003677b4: ldrne    r2, [r3, #4]
003677b8: mov      r4, r0
003677bc: ldr      r5, [pc, #0x7c]
003677c0: addne    r2, r2, #1
003677c4: strne    r2, [r3, #4]
003677c8: add      r2, sp, #4
003677cc: bl       #0x660ce4
003677d0: ldr      r0, [sp, #4]
003677d4: add      r5, pc, r5
003677d8: cmp      r0, #0
003677dc: beq      #0x3677e4
003677e0: bl       #0x31d584
003677e4: ldr      r2, [r6]
003677e8: ldr      r3, [pc, #0x54]
003677ec: add      r0, r4, #0x58
003677f0: str      r2, [r4]
003677f4: ldr      r3, [r5, r3]
003677f8: ldr      r2, [r2, #-0xc]
003677fc: ldr      ip, [r6, #0x24]
00367800: add      r3, r3, #0xa4
00367804: mov      r1, r4
00367808: str      ip, [r4, r2]
0036780c: str      r3, [r4, #4]
00367810: bl       #0x364398
00367814: ldr      r3, [r7]
00367818: mov      r0, r4
0036781c: cmp      r3, #0
00367820: str      r3, [r4, #0x94]
00367824: ldrne    r2, [r3, #4]
00367828: addne    r2, r2, #1
0036782c: strne    r2, [r3, #4]
00367830: mov      r3, #0
00367834: str      r3, [r4, #0x98]
00367838: add      sp, sp, #0xc
0036783c: pop      {r4, r5, r6, r7, pc}
00367840: strhteq  sp, [r2], #-0x2c
00367844: andeq    r1, r0, ip, asr #20

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoItLi30EEEbRKNS_3res6vectorIiEEiRii
0066b130: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b134: sub      sp, sp, #0x24
0066b138: str      r0, [sp, #0x18]
0066b13c: mov      r0, r2
0066b140: str      r2, [sp, #0xc]
0066b144: mov      r4, r1
0066b148: str      r3, [sp, #0x10]
0066b14c: bl       #0x30e964
0066b150: movw     r1, #0x5555
0066b154: movt     r1, #0x4205
0066b158: str      r0, [sp, #0x14]
0066b15c: bl       #0x30ec94
0066b160: ldr      r6, [r4]
0066b164: ldr      r5, [sp, #0x48]
0066b168: ldr      r8, [r4, #4]
0066b16c: sub      r6, r6, #1
0066b170: bic      r5, r5, r5, asr #31
0066b174: cmp      r5, r6
0066b178: movge    r5, r6
0066b17c: lsl      r3, r5, #1
0066b180: str      r3, [sp, #8]
0066b184: mov      r7, r0
0066b188: ldrh     r0, [r8, r3]
0066b18c: bl       #0x30e964
0066b190: mov      r1, r7
0066b194: bl       #0x30e2f8
0066b198: cmp      r0, #0
0066b19c: mov      sl, #0
0066b1a0: movne    sl, #1
0066b1a4: uxtb     sl, sl
0066b1a8: cmp      sl, #0
0066b1ac: beq      #0x66b218
0066b1b0: cmp      r5, #0
0066b1b4: subgt    r5, r5, #1
0066b1b8: lslgt    r3, r5, #1
0066b1bc: ble      #0x66b218
0066b1c0: cmp      r5, r6
0066b1c4: bge      #0x66b320
0066b1c8: ldrh     r0, [r8, r3]
0066b1cc: mov      sb, r3
0066b1d0: bl       #0x30e964
0066b1d4: mov      r1, r0
0066b1d8: mov      r0, r7
0066b1dc: bl       #0x30e70c
0066b1e0: cmp      r0, #0
0066b1e4: mov      sl, #0
0066b1e8: movne    sl, #1
0066b1ec: uxtb     sl, sl
0066b1f0: mov      fp, r5
0066b1f4: cmp      sl, #0
0066b1f8: beq      #0x66b2f4
0066b1fc: ldr      r0, [sp, #0x18]
0066b200: ldr      r2, [sp, #0xc]
0066b204: ldr      r3, [sp, #0x10]
0066b208: mov      r1, r4
0066b20c: add      sp, sp, #0x24
0066b210: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b214: b        #0x66abb8
0066b218: cmp      r6, r5
0066b21c: ble      #0x66b294
0066b220: add      fp, r5, #1
0066b224: lsl      sb, fp, #1
0066b228: ldrh     r0, [r8, sb]
0066b22c: bl       #0x30e964
0066b230: mov      r1, r7
0066b234: str      r0, [sp, #0x1c]
0066b238: bl       #0x30e70c
0066b23c: cmp      r0, #0
0066b240: mov      r3, sb
0066b244: beq      #0x66b314
0066b248: cmp      r6, fp
0066b24c: ble      #0x66b29c
0066b250: add      r5, fp, #1
0066b254: lsl      r3, r5, #1
0066b258: ldrh     r0, [r8, r3]
0066b25c: str      r3, [sp, #4]
0066b260: bl       #0x30e964
0066b264: mov      r1, r7
0066b268: bl       #0x30e70c
0066b26c: subs     sl, r0, #0
0066b270: ldr      r3, [sp, #4]
0066b274: bne      #0x66b1c0
0066b278: ldr      r1, [sp, #0x1c]
0066b27c: mov      r0, r7
0066b280: bl       #0x30e70c
0066b284: cmp      r0, #0
0066b288: movne    sl, #1
0066b28c: uxtb     sl, sl
0066b290: b        #0x66b1f4
0066b294: mov      fp, r5
0066b298: lsl      r3, r5, #1
0066b29c: mov      sb, r3
0066b2a0: mov      r5, fp
0066b2a4: ldr      r3, [sp, #0x10]
0066b2a8: str      r5, [r3]
0066b2ac: ldr      r3, [r4, #4]
0066b2b0: ldrh     r0, [r3, sb]
0066b2b4: bl       #0x30e964
0066b2b8: movw     r1, #0x5555
0066b2bc: movt     r1, #0x4205
0066b2c0: bl       #0x30ed6c
0066b2c4: mov      r1, r0
0066b2c8: ldr      r0, [sp, #0x14]
0066b2cc: bl       #0x30df8c
0066b2d0: cmp      r0, #0
0066b2d4: movne    r0, #0
0066b2d8: bne      #0x66b2ec
0066b2dc: ldr      r0, [r4]
0066b2e0: sub      r0, r0, #1
0066b2e4: subs     r0, r5, r0
0066b2e8: movne    r0, #1
0066b2ec: add      sp, sp, #0x24
0066b2f0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066b2f4: add      r8, r8, fp, lsl #1
0066b2f8: ldrh     r0, [r8, #2]
0066b2fc: bl       #0x30e964
0066b300: mov      r1, r7
0066b304: bl       #0x30e70c
0066b308: cmp      r0, #0
0066b30c: bne      #0x66b1fc
0066b310: b        #0x66b2a0
0066b314: ldr      sb, [sp, #8]
0066b318: mov      fp, r5
0066b31c: b        #0x66b1f4
0066b320: mov      sb, r3
0066b324: b        #0x66b2a4

# _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIiLi1000EEEbiRKNS_3res6vectorIiEEiRiRfi
0066a5e0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a5e4: sub      sp, sp, #0x14
0066a5e8: ldr      r8, [sp, #0x38]
0066a5ec: ldr      ip, [sp, #0x40]
0066a5f0: mov      r5, r2
0066a5f4: mov      r6, r1
0066a5f8: mov      r2, r3
0066a5fc: mov      r1, r5
0066a600: mov      r4, r3
0066a604: mov      r3, r8
0066a608: mov      r7, r0
0066a60c: str      ip, [sp]
0066a610: ldr      sl, [sp, #0x3c]
0066a614: bl       #0x66a368
0066a618: mov      r1, r6
0066a61c: mov      sb, r0
0066a620: mov      r0, r7
0066a624: bl       #0x669e9c
0066a628: cmp      r0, #0
0066a62c: moveq    r6, #0
0066a630: andne    r6, sb, #1
0066a634: cmp      r6, #0
0066a638: beq      #0x66a6ac
0066a63c: ldr      fp, [r8]
0066a640: ldr      sb, [r5, #4]
0066a644: mov      r5, #0
0066a648: mov      r7, #0x3f800000
0066a64c: ldr      r0, [sb, fp, lsl #2]
0066a650: bl       #0x30e964
0066a654: bl       #0x30e4cc
0066a658: mov      r8, r0
0066a65c: rsb      r0, r0, r4
0066a660: bl       #0x30e964
0066a664: add      fp, fp, #1
0066a668: mov      r4, r0
0066a66c: ldr      r0, [sb, fp, lsl #2]
0066a670: bl       #0x30e964
0066a674: bl       #0x30e4cc
0066a678: rsb      r0, r8, r0
0066a67c: bl       #0x30e964
0066a680: mov      r1, r0
0066a684: mov      r0, r4
0066a688: bl       #0x30ec94
0066a68c: mov      r1, r5
0066a690: str      r0, [sl]
0066a694: mov      r4, r0
0066a698: bl       #0x30e70c
0066a69c: cmp      r0, #0
0066a6a0: movne    r4, r5
0066a6a4: beq      #0x66a6b8
0066a6a8: str      r4, [sl]
0066a6ac: mov      r0, r6
0066a6b0: add      sp, sp, #0x14
0066a6b4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a6b8: mov      r0, r4
0066a6bc: mov      r1, r7
0066a6c0: bl       #0x30e70c
0066a6c4: cmp      r0, #0
0066a6c8: moveq    r4, r7
0066a6cc: b        #0x66a6a8

# _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIhLi30EEEbiRKNS_3res6vectorIiEEiRiRfi
0066aab4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0066aab8: sub      sp, sp, #0x10
0066aabc: ldr      r8, [sp, #0x30]
0066aac0: ldr      ip, [sp, #0x38]
0066aac4: mov      r5, r2
0066aac8: mov      r6, r1
0066aacc: mov      r2, r3
0066aad0: mov      r1, r5
0066aad4: mov      r4, r3
0066aad8: mov      r3, r8
0066aadc: mov      r7, r0
0066aae0: str      ip, [sp]
0066aae4: ldr      sl, [sp, #0x34]
0066aae8: bl       #0x66a894
0066aaec: mov      r1, r6
0066aaf0: mov      sb, r0
0066aaf4: mov      r0, r7
0066aaf8: bl       #0x669e9c
0066aafc: cmp      r0, #0
0066ab00: moveq    r6, #0
0066ab04: andne    r6, sb, #1
0066ab08: cmp      r6, #0
0066ab0c: beq      #0x66ab94
0066ab10: ldr      r3, [r8]
0066ab14: ldr      r5, [r5, #4]
0066ab18: mov      r8, #0
0066ab1c: mov      sb, #0x3f800000
0066ab20: ldrb     r0, [r5, r3]!
0066ab24: bl       #0x30e964
0066ab28: movw     r1, #0x5555
0066ab2c: movt     r1, #0x4205
0066ab30: bl       #0x30ed6c
0066ab34: bl       #0x30e4cc
0066ab38: mov      r7, r0
0066ab3c: rsb      r0, r0, r4
0066ab40: bl       #0x30e964
0066ab44: mov      r4, r0
0066ab48: ldrb     r0, [r5, #1]
0066ab4c: bl       #0x30e964
0066ab50: movw     r1, #0x5555
0066ab54: movt     r1, #0x4205
0066ab58: bl       #0x30ed6c
0066ab5c: bl       #0x30e4cc
0066ab60: rsb      r0, r7, r0
0066ab64: bl       #0x30e964
0066ab68: mov      r1, r0
0066ab6c: mov      r0, r4
0066ab70: bl       #0x30ec94
0066ab74: mov      r1, r8
0066ab78: str      r0, [sl]
0066ab7c: mov      r4, r0
0066ab80: bl       #0x30e70c
0066ab84: cmp      r0, #0
0066ab88: movne    r4, r8
0066ab8c: beq      #0x66aba0
0066ab90: str      r4, [sl]
0066ab94: mov      r0, r6
0066ab98: add      sp, sp, #0x10
0066ab9c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0066aba0: mov      r0, r4
0066aba4: mov      r1, sb
0066aba8: bl       #0x30e70c
0066abac: cmp      r0, #0
0066abb0: moveq    r4, sb
0066abb4: b        #0x66ab90

# _ZN6glitch7collada20CDynamicAnimationSet27addAnimationLibraryBindingsERKNS0_16CColladaDatabaseE
0062ee9c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062eea0: mov      r4, r0
0062eea4: mov      sl, r1
0062eea8: sub      sp, sp, #0x1c
0062eeac: add      r0, r0, #0x24
0062eeb0: bl       #0x62ecac
0062eeb4: ldr      r3, [sl]
0062eeb8: ldr      r1, [r4, #0x44]
0062eebc: ldr      r2, [r4, #0x48]
0062eec0: ldr      r3, [r3, #0x24]
0062eec4: cmp      r1, r2
0062eec8: ldr      r3, [r3, #0x20]
0062eecc: ldr      r3, [r3, #0x1c]
0062eed0: str      r3, [sp, #0x14]
0062eed4: beq      #0x62f09c
0062eed8: str      r3, [r1]
0062eedc: ldr      r3, [r4, #0x44]
0062eee0: add      r3, r3, #4
0062eee4: str      r3, [r4, #0x44]
0062eee8: ldr      r3, [sl]
0062eeec: ldr      r1, [r4, #0x50]
0062eef0: ldr      r2, [r4, #0x54]
0062eef4: ldr      r3, [r3, #0x24]
0062eef8: cmp      r1, r2
0062eefc: ldr      r3, [r3, #0x20]
0062ef00: ldr      r3, [r3, #0x20]
0062ef04: str      r3, [sp, #0x10]
0062ef08: beq      #0x62f0ac
0062ef0c: str      r3, [r1]
0062ef10: ldr      r3, [r4, #0x50]
0062ef14: add      r3, r3, #4
0062ef18: str      r3, [r4, #0x50]
0062ef1c: ldr      r3, [sl]
0062ef20: ldr      r2, [r4, #0x60]
0062ef24: ldr      r1, [r4, #0x5c]
0062ef28: ldr      r3, [r3, #0x24]
0062ef2c: cmp      r1, r2
0062ef30: ldr      r3, [r3, #0x20]
0062ef34: ldr      r2, [r3, #0x1c]
0062ef38: ldr      r3, [r3, #0x20]
0062ef3c: rsb      r3, r2, r3
0062ef40: str      r3, [sp, #0xc]
0062ef44: beq      #0x62f0bc
0062ef48: str      r3, [r1]
0062ef4c: ldr      r3, [r4, #0x5c]
0062ef50: add      r3, r3, #4
0062ef54: str      r3, [r4, #0x5c]
0062ef58: ldr      r5, [r4, #0x34]
0062ef5c: ldr      r3, [r4, #0x30]
0062ef60: ldr      r7, [r4, #0x3c]
0062ef64: add      r8, r4, #0x30
0062ef68: rsb      r3, r3, r5
0062ef6c: asr      r3, r3, #2
0062ef70: mov      r0, r8
0062ef74: add      r5, r3, r3, lsl #2
0062ef78: mov      r6, #0
0062ef7c: add      r5, r5, r5, lsl #4
0062ef80: add      r5, r5, r5, lsl #8
0062ef84: add      r5, r5, r5, lsl #16
0062ef88: add      r5, r3, r5, lsl #1
0062ef8c: add      r7, r5, r7
0062ef90: mov      r1, r7
0062ef94: bl       #0x62e5e0
0062ef98: mov      r0, r8
0062ef9c: mov      r1, r7
0062efa0: mov      r2, sp
0062efa4: str      r6, [sp]
0062efa8: str      r6, [sp, #4]
0062efac: str      r6, [sp, #8]
0062efb0: bl       #0x62ec50
0062efb4: ldr      r3, [r4, #0x3c]
0062efb8: cmp      r3, r6
0062efbc: beq      #0x62f094
0062efc0: mov      r3, #0xc
0062efc4: mul      r5, r3, r5
0062efc8: add      fp, r4, #0x68
0062efcc: mov      sb, #2
0062efd0: b        #0x62f004
0062efd4: ldr      r2, [r4, #0x30]
0062efd8: ldr      r1, [r4, #0x74]
0062efdc: add      r2, r2, r5
0062efe0: add      r1, r1, r7
0062efe4: add      r2, r2, #4
0062efe8: bl       #0x61c6bc
0062efec: cmp      r0, #0
0062eff0: beq      #0x62f074
0062eff4: ldr      r3, [r4, #0x3c]
0062eff8: add      r5, r5, #0xc
0062effc: cmp      r3, r6
0062f000: bls      #0x62f094
0062f004: ldr      r1, [r4, #0x74]
0062f008: lsl      r7, r6, #4
0062f00c: mov      r0, sl
0062f010: add      r1, r1, r7
0062f014: bl       #0x61c1e0
0062f018: ldr      r2, [r4, #0x30]
0062f01c: ldr      r1, [r4, #0x74]
0062f020: mov      r8, r0
0062f024: add      r2, r2, r5
0062f028: add      r2, r2, #4
0062f02c: mov      r0, sl
0062f030: add      r1, r1, r7
0062f034: bl       #0x61c6bc
0062f038: ldr      r3, [r4, #0x30]
0062f03c: cmp      r8, #0
0062f040: moveq    r2, #1
0062f044: streq    r2, [r3, r5]
0062f048: strne    sb, [r3, r5]
0062f04c: ldr      r3, [r4, #0x30]
0062f050: cmp      r0, #0
0062f054: add      r6, r6, #1
0062f058: add      r3, r3, r5
0062f05c: str      r8, [r3, #8]
0062f060: bne      #0x62eff4
0062f064: ldr      r3, [r4, #0x68]
0062f068: mov      r0, fp
0062f06c: cmp      r3, #0
0062f070: bne      #0x62efd4
0062f074: ldr      r3, [r4, #0x30]
0062f078: mov      r2, #0
0062f07c: add      r3, r3, r5
0062f080: str      r2, [r3, #4]
0062f084: ldr      r3, [r4, #0x3c]
0062f088: add      r5, r5, #0xc
0062f08c: cmp      r3, r6
0062f090: bhi      #0x62f004
0062f094: add      sp, sp, #0x1c
0062f098: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062f09c: add      r0, r4, #0x40
0062f0a0: add      r2, sp, #0x14
0062f0a4: bl       #0x62ee0c
0062f0a8: b        #0x62eee8
0062f0ac: add      r0, r4, #0x4c
0062f0b0: add      r2, sp, #0x10
0062f0b4: bl       #0x62ee0c
0062f0b8: b        #0x62ef1c
0062f0bc: add      r0, r4, #0x58
0062f0c0: add      r2, sp, #0xc
0062f0c4: bl       #0x62ee0c
0062f0c8: b        #0x62ef58

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiiRiRfi
0066b814: push     {r4, r5, r6, r7, lr}
0066b818: sub      sp, sp, #0x14
0066b81c: mov      r5, r2
0066b820: mov      r4, r3
0066b824: mov      r6, r0
0066b828: mov      r7, r1
0066b82c: bl       #0x669eec
0066b830: ldr      ip, [sp, #0x28]
0066b834: mov      r2, r0
0066b838: mov      r1, r7
0066b83c: str      ip, [sp, #4]
0066b840: ldr      ip, [sp, #0x2c]
0066b844: mov      r0, r6
0066b848: mov      r3, r5
0066b84c: str      r4, [sp]
0066b850: str      ip, [sp, #8]
0066b854: bl       #0x66b65c
0066b858: add      sp, sp, #0x14
0066b85c: pop      {r4, r5, r6, r7, pc}

# _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExItLi30EEEbiRKNS_3res6vectorIiEEiRiRfi
0066b550: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0066b554: sub      sp, sp, #0x10
0066b558: ldr      r8, [sp, #0x30]
0066b55c: ldr      ip, [sp, #0x38]
0066b560: mov      r5, r2
0066b564: mov      r6, r1
0066b568: mov      r2, r3
0066b56c: mov      r1, r5
0066b570: mov      r4, r3
0066b574: mov      r3, r8
0066b578: mov      r7, r0
0066b57c: str      ip, [sp]
0066b580: ldr      sl, [sp, #0x34]
0066b584: bl       #0x66b130
0066b588: mov      r1, r6
0066b58c: mov      sb, r0
0066b590: mov      r0, r7
0066b594: bl       #0x669e9c
0066b598: cmp      r0, #0
0066b59c: moveq    r6, #0
0066b5a0: andne    r6, sb, #1
0066b5a4: cmp      r6, #0
0066b5a8: beq      #0x66b638
0066b5ac: ldr      r8, [r8]
0066b5b0: ldr      r7, [r5, #4]
0066b5b4: mov      sb, #0
0066b5b8: lsl      r3, r8, #1
0066b5bc: ldrh     r0, [r7, r3]
0066b5c0: bl       #0x30e964
0066b5c4: movw     r1, #0x5555
0066b5c8: movt     r1, #0x4205
0066b5cc: bl       #0x30ed6c
0066b5d0: bl       #0x30e4cc
0066b5d4: mov      r5, r0
0066b5d8: rsb      r0, r0, r4
0066b5dc: bl       #0x30e964
0066b5e0: add      r7, r7, r8, lsl #1
0066b5e4: mov      r4, r0
0066b5e8: ldrh     r0, [r7, #2]
0066b5ec: bl       #0x30e964
0066b5f0: movw     r1, #0x5555
0066b5f4: movt     r1, #0x4205
0066b5f8: bl       #0x30ed6c
0066b5fc: bl       #0x30e4cc
0066b600: rsb      r0, r5, r0
0066b604: bl       #0x30e964
0066b608: mov      r1, r0
0066b60c: mov      r0, r4
0066b610: bl       #0x30ec94
0066b614: mov      r1, sb
0066b618: str      r0, [sl]
0066b61c: mov      r4, r0
0066b620: bl       #0x30e70c
0066b624: cmp      r0, #0
0066b628: mov      r5, #0x3f800000
0066b62c: movne    r4, sb
0066b630: beq      #0x66b644
0066b634: str      r4, [sl]
0066b638: mov      r0, r6
0066b63c: add      sp, sp, #0x10
0066b640: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0066b644: mov      r0, r4
0066b648: mov      r1, r5
0066b64c: bl       #0x30e70c
0066b650: cmp      r0, #0
0066b654: moveq    r4, r5
0066b658: b        #0x66b634

# _ZNK6glitch7collada18ISceneNodeAnimator19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
00667cb4: bx       lr

# _ZN6glitch7collada21IAnimationSetTemplate10setUnAddedEv
00667320: ldmib    r0, {r2, r3}
00667324: rsb      r3, r2, r3
00667328: lsrs     r3, r3, #2
0066732c: bxeq     lr
00667330: mov      r3, #0
00667334: mov      r1, r3
00667338: ldr      r2, [r2, r3, lsl #2]
0066733c: strb     r1, [r2]
00667340: ldr      r2, [r0, #4]
00667344: ldr      r2, [r2, r3, lsl #2]
00667348: add      r3, r3, #1
0066734c: str      r1, [r2, #0xc]
00667350: ldmib    r0, {r2, ip}
00667354: rsb      ip, r2, ip
00667358: cmp      r3, ip, asr #2
0066735c: blo      #0x667338
00667360: bx       lr

# _ZN6glitch7collada21IAnimationSetTemplate11addChannelsEPSt6vectorIPKNS0_8SChannelENS_4core10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEEPS2_IPKNS0_17CAnimationTrackExENS7_ISF_LS9_0EEEE
00667384: push     {r4, r5, r6, r7, r8, lr}
00667388: mov      r5, r0
0066738c: ldr      r3, [r0, #4]
00667390: ldr      r0, [r0, #8]
00667394: rsb      r2, r3, r0
00667398: lsrs     r2, r2, #2
0066739c: beq      #0x667488
006673a0: mov      r4, #0
006673a4: mov      r7, #1
006673a8: b        #0x6673bc
006673ac: add      r4, r4, #1
006673b0: rsb      r2, r3, r0
006673b4: cmp      r4, r2, asr #2
006673b8: bhs      #0x667488
006673bc: ldr      r2, [r3, r4, lsl #2]
006673c0: lsl      r6, r4, #2
006673c4: ldrb     r1, [r2]
006673c8: cmp      r1, #0
006673cc: bne      #0x6673ac
006673d0: mov      r0, #0x10
006673d4: bl       #0x5341ac
006673d8: ldr      r3, [r5, #4]
006673dc: mov      r8, r0
006673e0: ldr      r3, [r3, r4, lsl #2]
006673e4: ldr      r3, [r3, #4]
006673e8: str      r3, [r0, #8]
006673ec: ldr      r3, [r5, #4]
006673f0: ldr      r3, [r3, r4, lsl #2]
006673f4: ldr      r3, [r3, #8]
006673f8: mov      r0, r3
006673fc: ldr      r3, [r3]
00667400: mov      lr, pc
00667404: ldr      pc, [r3, #0x54]
00667408: str      r0, [r8, #4]
0066740c: ldr      r3, [r5, #4]
00667410: ldr      r3, [r3, r4, lsl #2]
00667414: str      r8, [r3, #0xc]
00667418: ldr      r3, [r8, #8]
0066741c: sub      r3, r3, #1
00667420: cmp      r3, #0xc
00667424: addls    pc, pc, r3, lsl #2
00667428: b        #0x667464
0066742c: b        #0x66756c
00667430: b        #0x667550
00667434: b        #0x667534
00667438: b        #0x667518
0066743c: b        #0x6674fc
00667440: b        #0x6674e0
00667444: b        #0x6674e0
00667448: b        #0x6674e0
0066744c: b        #0x6674e0
00667450: b        #0x6674c4
00667454: b        #0x6674a8
00667458: b        #0x66748c
0066745c: b        #0x667460
00667460: bl       #0x610ebc
00667464: ldr      r3, [r5, #4]
00667468: ldr      r3, [r3, r6]
0066746c: strb     r7, [r3]
00667470: ldr      r3, [r5, #4]
00667474: ldr      r0, [r5, #8]
00667478: add      r4, r4, #1
0066747c: rsb      r2, r3, r0
00667480: cmp      r4, r2, asr #2
00667484: blo      #0x6673bc
00667488: pop      {r4, r5, r6, r7, r8, pc}
0066748c: bl       #0x610d00
00667490: ldr      r3, [r5, #4]
00667494: ldr      r3, [r3, r6]
00667498: strb     r7, [r3]
0066749c: ldr      r3, [r5, #4]
006674a0: ldr      r0, [r5, #8]
006674a4: b        #0x667478
006674a8: bl       #0x610b44
006674ac: ldr      r3, [r5, #4]
006674b0: ldr      r3, [r3, r6]
006674b4: strb     r7, [r3]
006674b8: ldr      r3, [r5, #4]
006674bc: ldr      r0, [r5, #8]
006674c0: b        #0x667478
006674c4: bl       #0x610988
006674c8: ldr      r3, [r5, #4]
006674cc: ldr      r3, [r3, r6]
006674d0: strb     r7, [r3]
006674d4: ldr      r3, [r5, #4]
006674d8: ldr      r0, [r5, #8]
006674dc: b        #0x667478
006674e0: bl       #0x6100dc
006674e4: ldr      r3, [r5, #4]
006674e8: ldr      r3, [r3, r6]
006674ec: strb     r7, [r3]
006674f0: ldr      r3, [r5, #4]
006674f4: ldr      r0, [r5, #8]
006674f8: b        #0x667478
006674fc: bl       #0x60ff20
00667500: ldr      r3, [r5, #4]
00667504: ldr      r3, [r3, r6]
00667508: strb     r7, [r3]
0066750c: ldr      r3, [r5, #4]
00667510: ldr      r0, [r5, #8]
00667514: b        #0x667478
00667518: bl       #0x6107cc
0066751c: ldr      r3, [r5, #4]
00667520: ldr      r3, [r3, r6]
00667524: strb     r7, [r3]
00667528: ldr      r3, [r5, #4]
0066752c: ldr      r0, [r5, #8]
00667530: b        #0x667478
00667534: bl       #0x610610
00667538: ldr      r3, [r5, #4]
0066753c: ldr      r3, [r3, r6]
00667540: strb     r7, [r3]
00667544: ldr      r3, [r5, #4]
00667548: ldr      r0, [r5, #8]
0066754c: b        #0x667478
00667550: bl       #0x610454
00667554: ldr      r3, [r5, #4]
00667558: ldr      r3, [r3, r6]
0066755c: strb     r7, [r3]
00667560: ldr      r3, [r5, #4]
00667564: ldr      r0, [r5, #8]
00667568: b        #0x667478
0066756c: bl       #0x610298
00667570: ldr      r3, [r5, #4]
00667574: ldr      r3, [r3, r6]
00667578: strb     r7, [r3]
0066757c: ldr      r3, [r5, #4]
00667580: ldr      r0, [r5, #8]
00667584: b        #0x667478

# _ZN15AnimatorBlender8SetScaleEf
003666d8: push     {r4, r5, r6, r7, r8, lr}
003666dc: ldr      r3, [r0, #0x28]
003666e0: ldr      r6, [r0, #0x2c]
003666e4: mov      r5, r0
003666e8: mov      r7, r1
003666ec: rsb      r6, r3, r6
003666f0: asrs     r6, r6, #2
003666f4: beq      #0x36673c
003666f8: mov      r4, #0
003666fc: b        #0x366704
00366700: ldr      r3, [r5, #0x28]
00366704: ldr      r3, [r3, r4, lsl #2]
00366708: add      r4, r4, #1
0036670c: mov      r0, r3
00366710: ldr      r3, [r3]
00366714: mov      lr, pc
00366718: ldr      pc, [r3, #0x44]
0036671c: subs     r3, r0, #0
00366720: mov      r1, r7
00366724: beq      #0x366734
00366728: ldr      r3, [r3]
0036672c: mov      lr, pc
00366730: ldr      pc, [r3, #0x48]
00366734: cmp      r4, r6
00366738: bne      #0x366700
0036673c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14AnimSetManager11GetAnimatorEi
004762c4: push     {r4, r5, r6, lr}
004762c8: sub      sp, sp, #0x10
004762cc: str      r1, [sp, #4]
004762d0: mov      r5, r0
004762d4: bl       #0x475404
004762d8: subs     r4, r0, #0
004762dc: bne      #0x4762ec
004762e0: mov      r0, r4
004762e4: add      sp, sp, #0x10
004762e8: pop      {r4, r5, r6, pc}
004762ec: add      r0, r5, #4
004762f0: add      r1, sp, #4
004762f4: bl       #0x476058
004762f8: ldr      r3, [r0, #0x20]
004762fc: mov      r5, r0
00476300: ldrb     r2, [r3, #0x70]
00476304: cmp      r2, #0
00476308: bne      #0x476384
0047630c: add      r6, sp, #0x10
00476310: str      r5, [r6, #-4]!
00476314: ldr      r3, [r5, #4]
00476318: mov      r1, #0
0047631c: mov      r0, #0xa4
00476320: add      r3, r3, #1
00476324: str      r3, [r5, #4]
00476328: bl       #0x310570
0047632c: mov      r1, r6
00476330: mov      r4, r0
00476334: bl       #0x3676b8
00476338: ldr      r0, [sp, #0xc]
0047633c: cmp      r0, #0
00476340: beq      #0x476348
00476344: bl       #0x31d584
00476348: ldr      r3, [r4]
0047634c: mov      r0, r4
00476350: mov      lr, pc
00476354: ldr      pc, [r3, #0x44]
00476358: mov      r6, r0
0047635c: mov      r0, r5
00476360: bl       #0x3649bc
00476364: cmp      r6, #0
00476368: beq      #0x4762e0
0047636c: mov      r0, r6
00476370: ldr      r3, [r6]
00476374: mov      r1, #0
00476378: mov      lr, pc
0047637c: ldr      pc, [r3, #0x40]
00476380: b        #0x4762e0
00476384: mov      r0, r3
00476388: ldr      r3, [r3]
0047638c: mov      lr, pc
00476390: ldr      pc, [r3, #0x38]
00476394: b        #0x47630c

# _ZNK6glitch7collada35CAnimationSetTransformationTemplate15getDefaultValueEPKNS0_8SChannelEPPv
006e23d0: push     {r4, r5, r6, r7, r8, lr}
006e23d4: ldr      r4, [r0, #4]
006e23d8: ldr      r3, [r0, #8]
006e23dc: mov      r5, r0
006e23e0: mov      r7, r1
006e23e4: cmp      r4, r3
006e23e8: mov      r6, r2
006e23ec: bne      #0x6e2400
006e23f0: b        #0x6e242c
006e23f4: ldr      r3, [r5, #8]
006e23f8: cmp      r4, r3
006e23fc: beq      #0x6e242c
006e2400: ldr      r3, [r4]
006e2404: mov      r1, r7
006e2408: mov      r2, r6
006e240c: ldr      r0, [r3, #8]
006e2410: add      r4, r4, #4
006e2414: add      r0, r0, #0x14c
006e2418: bl       #0x61c6bc
006e241c: cmp      r0, #0
006e2420: beq      #0x6e23f4
006e2424: mov      r0, #1
006e2428: pop      {r4, r5, r6, r7, r8, pc}
006e242c: mov      r0, #0
006e2430: pop      {r4, r5, r6, r7, r8, pc}

# _ZN24BlendedAnimSetControllerC2EP13RootSceneNodei
00476b4c: push     {r4, r5, r6, r7, r8, sl, lr}
00476b50: mov      r5, r2
00476b54: sub      sp, sp, #0x14
00476b58: mov      r2, #1
00476b5c: ldr      r7, [pc, #0x258]
00476b60: mov      r4, r0
00476b64: bl       #0x474e44
00476b68: ldr      r3, [pc, #0x250]
00476b6c: add      r7, pc, r7
00476b70: ldr      r2, [pc, #0x24c]
00476b74: ldr      r3, [r7, r3]
00476b78: mov      r8, #0
00476b7c: ldr      r6, [r7, r2]
00476b80: add      r3, r3, #8
00476b84: str      r3, [r4]
00476b88: mov      r3, #1
00476b8c: strb     r3, [r4, #0x10]
00476b90: mov      r1, r5
00476b94: str      r5, [r4, #8]
00476b98: mov      r0, r6
00476b9c: str      r8, [r4, #0xc]
00476ba0: str      r8, [r4, #0x14]
00476ba4: bl       #0x4762c4
00476ba8: mov      r5, r0
00476bac: ldr      r1, [r4, #8]
00476bb0: mov      r0, r6
00476bb4: bl       #0x4762c4
00476bb8: subs     r3, r5, r8
00476bbc: movne    r3, #1
00476bc0: subs     sl, r0, r8
00476bc4: movne    sl, #1
00476bc8: tst      sl, r3
00476bcc: mov      r6, r0
00476bd0: bne      #0x476c10
00476bd4: cmp      r3, #0
00476bd8: beq      #0x476bec
00476bdc: ldr      r3, [r5]
00476be0: ldr      r0, [r3, #-0xc]
00476be4: add      r0, r5, r0
00476be8: bl       #0x31d584
00476bec: cmp      sl, #0
00476bf0: beq      #0x476c04
00476bf4: ldr      r3, [r6]
00476bf8: ldr      r0, [r3, #-0xc]
00476bfc: add      r0, r6, r0
00476c00: bl       #0x31d584
00476c04: mov      r0, r4
00476c08: add      sp, sp, #0x14
00476c0c: pop      {r4, r5, r6, r7, r8, sl, pc}
00476c10: mov      r0, r5
00476c14: bl       #0x65f11c
00476c18: cmp      r0, r8
00476c1c: ble      #0x476d48
00476c20: mov      r1, #0
00476c24: mov      r0, #0xd0
00476c28: bl       #0x5341ac
00476c2c: mov      r7, r0
00476c30: bl       #0x367018
00476c34: mov      r3, #1
00476c38: str      r5, [sp, #0xc]
00476c3c: strb     r3, [r7, #0x24]
00476c40: ldr      r3, [sp, #0xc]
00476c44: add      r8, r7, #0x28
00476c48: ldr      r2, [r3]
00476c4c: ldr      r2, [r2, #-0xc]
00476c50: add      r3, r3, r2
00476c54: ldr      r2, [r3, #4]
00476c58: add      r2, r2, #1
00476c5c: str      r2, [r3, #4]
00476c60: ldr      r1, [r7, #0x2c]
00476c64: ldr      r3, [r7, #0x30]
00476c68: cmp      r1, r3
00476c6c: beq      #0x476d9c
00476c70: ldr      r3, [sp, #0xc]
00476c74: str      r3, [r1]
00476c78: ldr      r3, [r7, #0x2c]
00476c7c: add      r3, r3, #4
00476c80: str      r3, [r7, #0x2c]
00476c84: mov      r3, #1
00476c88: str      r6, [sp, #0xc]
00476c8c: strb     r3, [r7, #0x24]
00476c90: ldr      r3, [sp, #0xc]
00476c94: ldr      r2, [r3]
00476c98: ldr      r2, [r2, #-0xc]
00476c9c: add      r3, r3, r2
00476ca0: ldr      r2, [r3, #4]
00476ca4: add      r2, r2, #1
00476ca8: str      r2, [r3, #4]
00476cac: ldr      r1, [r7, #0x2c]
00476cb0: ldr      r3, [r7, #0x30]
00476cb4: cmp      r1, r3
00476cb8: beq      #0x476dac
00476cbc: ldr      r3, [sp, #0xc]
00476cc0: str      r3, [r1]
00476cc4: ldr      r3, [r7, #0x2c]
00476cc8: add      r3, r3, #4
00476ccc: str      r3, [r7, #0x2c]
00476cd0: mov      r0, r7
00476cd4: ldr      r3, [r7]
00476cd8: mov      r1, #0
00476cdc: mov      lr, pc
00476ce0: ldr      pc, [r3, #0x88]
00476ce4: ldr      r3, [r7, #0x34]
00476ce8: mov      r2, #0x3f800000
00476cec: mov      r1, r7
00476cf0: str      r2, [r3]
00476cf4: ldr      r3, [r7, #0x34]
00476cf8: mov      r2, #0
00476cfc: str      r2, [r3, #4]
00476d00: ldr      r3, [r4, #4]
00476d04: mov      r0, r3
00476d08: ldr      r3, [r3]
00476d0c: mov      lr, pc
00476d10: ldr      pc, [r3, #0x6c]
00476d14: ldr      r3, [r5]
00476d18: ldr      r0, [r3, #-0xc]
00476d1c: add      r0, r5, r0
00476d20: bl       #0x31d584
00476d24: ldr      r3, [r6]
00476d28: ldr      r0, [r3, #-0xc]
00476d2c: add      r0, r6, r0
00476d30: bl       #0x31d584
00476d34: ldr      r3, [r7]
00476d38: ldr      r0, [r3, #-0xc]
00476d3c: add      r0, r7, r0
00476d40: bl       #0x31d584
00476d44: b        #0x476c04
00476d48: ldr      r3, [pc, #0x78]
00476d4c: ldr      r3, [r7, r3]
00476d50: ldr      r3, [r3]
00476d54: cmp      r3, #2
00476d58: streq    r8, [r8]
00476d5c: beq      #0x476c20
00476d60: cmp      r3, #1
00476d64: bne      #0x476c20
00476d68: ldr      r0, [pc, #0x5c]
00476d6c: ldr      r1, [pc, #0x5c]
00476d70: ldr      r2, [pc, #0x5c]
00476d74: ldr      r0, [r7, r0]
00476d78: ldr      r3, [pc, #0x58]
00476d7c: mov      ip, #0x45
00476d80: add      r1, pc, r1
00476d84: add      r2, pc, r2
00476d88: add      r3, pc, r3
00476d8c: add      r0, r0, #0xa8
00476d90: str      ip, [sp]
00476d94: bl       #0x30e004
00476d98: b        #0x476c20
00476d9c: mov      r0, r8
00476da0: add      r2, sp, #0xc
00476da4: bl       #0x476a9c
00476da8: b        #0x476c84
00476dac: mov      r0, r8
00476db0: add      r2, sp, #0xc
00476db4: bl       #0x476a9c
00476db8: b        #0x476cd0
00476dbc: subseq   sp, r1, r4, lsr #30
00476dc0: andeq    r2, r0, r8, lsr #7
00476dc4: andeq    r4, r0, r8, lsr r8
00476dc8: andeq    r3, r0, r0, asr #19
00476dcc: andeq    r1, r0, r0, asr #19
00476dd0: subeq    r7, r4, r8, asr r6
00476dd4: ldrdeq   r6, r7, [r5], #-0xac
00476dd8: strdeq   r6, r7, [r5], #-0xa8

# _ZN6glitch7collada6detail27constructCompatibilityTableEv
00670a60: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00670a64: ldr      r4, [pc, #0x288]
00670a68: add      r4, pc, r4
00670a6c: ldr      r5, [r4]
00670a70: ands     r5, r5, #1
00670a74: beq      #0x670cb0
00670a78: ldr      r6, [pc, #0x278]
00670a7c: mov      r5, #1
00670a80: mov      r7, #0
00670a84: add      r6, pc, r6
00670a88: mov      r0, r6
00670a8c: mov      r8, r5
00670a90: mov      r3, #0
00670a94: add      r4, r7, r7, lsl #1
00670a98: add      r2, r4, r3, lsr #5
00670a9c: and      ip, r3, #0x1f
00670aa0: add      r2, r6, r2, lsl #2
00670aa4: ldr      r1, [r2, #4]
00670aa8: add      r3, r3, #1
00670aac: cmp      r3, #0x5c
00670ab0: bic      r1, r1, r5, lsl ip
00670ab4: str      r1, [r2, #4]
00670ab8: bne      #0x670a98
00670abc: add      r3, r4, r7, lsr #5
00670ac0: and      r1, r7, #0x1f
00670ac4: add      r3, r0, r3, lsl #2
00670ac8: ldr      r2, [r3, #4]
00670acc: add      r7, r7, #1
00670ad0: cmp      r7, #0x5c
00670ad4: orr      r2, r2, r8, lsl r1
00670ad8: str      r2, [r3, #4]
00670adc: bne      #0x670a90
00670ae0: ldr      r7, [r0, #0x450]
00670ae4: ldr      r2, [r0, #0x3d8]
00670ae8: ldr      r6, [r0, #0x420]
00670aec: orr      r7, r7, #0x7800000
00670af0: str      r7, [r0, #0x450]
00670af4: ldr      r7, [r0, #0x10c]
00670af8: orr      r2, r2, #0x3c0000
00670afc: ldr      sb, [r0, #0x42c]
00670b00: orr      r3, r7, #0x3a00000
00670b04: ldr      r7, [r0, #0x118]
00670b08: ldr      r8, [r0, #0x438]
00670b0c: ldr      sl, [r0, #0x444]
00670b10: orr      r7, r7, #0x3600000
00670b14: str      r7, [r0, #0x118]
00670b18: ldr      r7, [r0, #0x130]
00670b1c: ldr      r5, [r0, #0x3e4]
00670b20: ldr      ip, [r0, #0x3f0]
00670b24: orr      r7, r7, #0x1e00000
00670b28: str      r7, [r0, #0x130]
00670b2c: ldr      r7, [r0, #0x100]
00670b30: ldr      r4, [r0, #0x3fc]
00670b34: ldr      r1, [r0, #0x408]
00670b38: orr      r7, r7, #0x3c00000
00670b3c: ldr      fp, [r0, #0x124]
00670b40: str      r7, [r0, #0x100]
00670b44: str      r2, [r0, #0x3d8]
00670b48: ldr      r7, [r0, #0x3a8]
00670b4c: ldr      r2, [r0, #0x7c]
00670b50: orr      sb, sb, #0xe800000
00670b54: orr      sl, sl, #0xb800000
00670b58: orr      r8, r8, #0xd800000
00670b5c: orr      r6, r6, #0xf000000
00670b60: orr      r5, r5, #0x3a0000
00670b64: orr      r4, r4, #0x2e0000
00670b68: orr      ip, ip, #0x360000
00670b6c: orr      r1, r1, #0x1e0000
00670b70: orr      fp, fp, #0x2e00000
00670b74: orr      r7, r7, #0x1d000
00670b78: orr      r2, r2, #0x3800
00670b7c: str      r7, [r0, #0x3a8]
00670b80: str      sb, [r0, #0x42c]
00670b84: str      sl, [r0, #0x444]
00670b88: str      r8, [r0, #0x438]
00670b8c: str      r6, [r0, #0x420]
00670b90: str      r5, [r0, #0x3e4]
00670b94: str      r4, [r0, #0x3fc]
00670b98: str      ip, [r0, #0x3f0]
00670b9c: str      r1, [r0, #0x408]
00670ba0: str      r3, [r0, #0x10c]
00670ba4: str      fp, [r0, #0x124]
00670ba8: ldr      r7, [r0, #0x39c]
00670bac: ldr      sl, [r0, #0x3b4]
00670bb0: ldr      sb, [r0, #0x3c0]
00670bb4: ldr      r8, [r0, #0x3cc]
00670bb8: ldr      r1, [r0, #0x360]
00670bbc: ldr      r6, [r0, #0x36c]
00670bc0: ldr      r4, [r0, #0x378]
00670bc4: ldr      r5, [r0, #0x384]
00670bc8: ldr      ip, [r0, #0x390]
00670bcc: ldr      fp, [r0, #0x88]
00670bd0: str      r2, [r0, #0x7c]
00670bd4: ldr      r2, [r0, #0xa0]
00670bd8: ldr      r3, [r0, #0x94]
00670bdc: orr      sb, sb, #0x17000
00670be0: orr      r2, r2, #0x1c00
00670be4: str      r2, [r0, #0xa0]
00670be8: ldr      r2, [r0, #0x10]
00670bec: orr      r3, r3, #0x2c00
00670bf0: str      r3, [r0, #0x94]
00670bf4: orr      r2, r2, #0x1c
00670bf8: ldr      r3, [r0, #0x1c]
00670bfc: str      r2, [r0, #0x10]
00670c00: ldr      r2, [r0, #0x34]
00670c04: orr      r3, r3, #0x1a
00670c08: str      r3, [r0, #0x1c]
00670c0c: orr      r2, r2, #0xe
00670c10: ldr      r3, [r0, #0x28]
00670c14: str      r2, [r0, #0x34]
00670c18: ldr      r2, [r0, #0x4c]
00670c1c: orr      r3, r3, #0x16
00670c20: str      r3, [r0, #0x28]
00670c24: orr      r2, r2, #0x3a0
00670c28: ldr      r3, [r0, #0x64]
00670c2c: str      r2, [r0, #0x4c]
00670c30: ldr      r2, [r0, #0x58]
00670c34: orr      r3, r3, #0x2e0
00670c38: str      r3, [r0, #0x64]
00670c3c: orr      r2, r2, #0x360
00670c40: ldr      r3, [r0, #0x70]
00670c44: str      r2, [r0, #0x58]
00670c48: ldr      r2, [r0, #0x40]
00670c4c: orr      r3, r3, #0x1e0
00670c50: str      r3, [r0, #0x70]
00670c54: orr      sl, sl, #0x1b000
00670c58: orr      r8, r8, #0xf000
00670c5c: orr      r7, r7, #0x1e000
00670c60: orr      r6, r6, #0xe80
00670c64: orr      r5, r5, #0xb80
00670c68: orr      r4, r4, #0xd80
00670c6c: orr      ip, ip, #0x780
00670c70: orr      r1, r1, #0xf00
00670c74: orr      fp, fp, #0x3400
00670c78: orr      r3, r2, #0x3c0
00670c7c: str      sb, [r0, #0x3c0]
00670c80: str      sl, [r0, #0x3b4]
00670c84: str      r8, [r0, #0x3cc]
00670c88: str      r7, [r0, #0x39c]
00670c8c: str      r6, [r0, #0x36c]
00670c90: str      r5, [r0, #0x384]
00670c94: str      r4, [r0, #0x378]
00670c98: str      ip, [r0, #0x390]
00670c9c: str      r1, [r0, #0x360]
00670ca0: str      fp, [r0, #0x88]
00670ca4: str      r3, [r0, #0x40]
00670ca8: add      r0, r0, #4
00670cac: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00670cb0: mov      r0, r4
00670cb4: bl       #0x30e76c
00670cb8: cmp      r0, #0
00670cbc: beq      #0x670a78
00670cc0: add      r4, r4, #4
00670cc4: add      r2, r4, #0x450
00670cc8: mov      r3, r4
00670ccc: str      r5, [r3], #4
00670cd0: str      r5, [r4, #4]
00670cd4: add      r4, r4, #0xc
00670cd8: cmp      r4, r2
00670cdc: str      r5, [r3, #4]
00670ce0: bne      #0x670cc8
00670ce4: ldr      r0, [pc, #0x10]
00670ce8: add      r0, pc, r0
00670cec: bl       #0x30ea3c
00670cf0: b        #0x670a78
00670cf4: eorseq   r6, r8, r0, lsr #13
00670cf8: eorseq   r6, r8, r4, lsl #13
00670cfc: eorseq   r6, r8, r0, lsr #8

# _ZNK6glitch7collada16CColladaDatabase7getNodeEPKcRNS0_5SNodeE
0061c214: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c218: mov      r7, r0
0061c21c: ldr      r0, [r2]
0061c220: mov      r4, r2
0061c224: mov      r8, r1
0061c228: bl       #0x30e31c
0061c22c: cmp      r0, #0
0061c230: bne      #0x61c23c
0061c234: mov      r0, r4
0061c238: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c23c: ldr      sl, [r4, #0x38]
0061c240: cmp      sl, #0
0061c244: ble      #0x61c288
0061c248: mov      r5, #0
0061c24c: mov      r6, r5
0061c250: b        #0x61c260
0061c254: cmp      r6, sl
0061c258: add      r5, r5, #0x50
0061c25c: beq      #0x61c288
0061c260: ldr      r2, [r4, #0x3c]
0061c264: mov      r0, r7
0061c268: mov      r1, r8
0061c26c: add      r2, r2, r5
0061c270: bl       #0x61c214
0061c274: cmp      r0, #0
0061c278: add      r6, r6, #1
0061c27c: beq      #0x61c254
0061c280: mov      r4, r0
0061c284: b        #0x61c234
0061c288: mov      r4, #0
0061c28c: b        #0x61c234

# _ZN12AnimationSet13LoadAnimationEi
003659ec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003659f0: ldr      r4, [pc, #0x2fc]
003659f4: ldr      r5, [pc, #0x2fc]
003659f8: mov      r7, r0
003659fc: add      r4, pc, r4
00365a00: ldr      r3, [r4, r5]
00365a04: ldr      r0, [pc, #0x2f0]
00365a08: sub      sp, sp, #0xc4
00365a0c: ldr      r3, [r3]
00365a10: add      r0, pc, r0
00365a14: str      r1, [sp, #4]
00365a18: str      r3, [sp, #0xbc]
00365a1c: bl       #0x3136b4
00365a20: ldr      r3, [sp, #4]
00365a24: cmp      r3, #0
00365a28: blt      #0x365a40
00365a2c: ldr      r2, [pc, #0x2cc]
00365a30: ldr      r2, [r4, r2]
00365a34: ldr      r2, [r2]
00365a38: cmp      r3, r2
00365a3c: blt      #0x365a74
00365a40: ldr      r3, [pc, #0x2bc]
00365a44: ldr      r7, [r4, r3]
00365a48: ldr      r0, [pc, #0x2b8]
00365a4c: add      r0, pc, r0
00365a50: bl       #0x3136b8
00365a54: ldr      r3, [r4, r5]
00365a58: ldr      r2, [sp, #0xbc]
00365a5c: mov      r0, r7
00365a60: ldr      r3, [r3]
00365a64: cmp      r2, r3
00365a68: bne      #0x365cf0
00365a6c: add      sp, sp, #0xc4
00365a70: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00365a74: ldr      r2, [pc, #0x290]
00365a78: ldr      r1, [pc, #0x290]
00365a7c: add      r6, sp, #0x8c
00365a80: ldr      r2, [r4, r2]
00365a84: add      r1, pc, r1
00365a88: mov      r0, r6
00365a8c: ldr      ip, [r2]
00365a90: mov      sl, #0xc
00365a94: add      r2, r1, #7
00365a98: mla      sl, sl, r3, ip
00365a9c: str      r6, [sp, #0x9c]
00365aa0: str      r6, [sp, #0xa0]
00365aa4: bl       #0x3116e8
00365aa8: ldr      r2, [pc, #0x264]
00365aac: mvn      r3, #0
00365ab0: mov      r8, #0
00365ab4: ldr      r2, [r4, r2]
00365ab8: str      r3, [sp, #0xb0]
00365abc: str      r3, [sp, #0xac]
00365ac0: str      r2, [sp, #0xa8]
00365ac4: str      r8, [sp, #0xa4]
00365ac8: str      r8, [sp, #0xb4]
00365acc: str      r8, [sp, #0xb8]
00365ad0: ldr      sb, [sl, #8]
00365ad4: add      sl, sp, #0x1c
00365ad8: mov      r0, sb
00365adc: bl       #0x30de54
00365ae0: mov      r1, sb
00365ae4: add      r2, sb, r0
00365ae8: mov      r0, r6
00365aec: bl       #0x3109e0
00365af0: mov      r2, r8
00365af4: mov      r0, sl
00365af8: ldr      r1, [sp, #0xa0]
00365afc: bl       #0x60f25c
00365b00: ldr      r3, [sp, #0x1c]
00365b04: ldr      r2, [sp, #0x20]
00365b08: cmp      r3, r8
00365b0c: str      r2, [sp, #0x18]
00365b10: str      r3, [sp, #0x14]
00365b14: beq      #0x365b2c
00365b18: ldr      r2, [r3, #4]
00365b1c: cmp      r2, r8
00365b20: addne    r2, r2, #1
00365b24: strne    r2, [r3, #4]
00365b28: ldrne    r3, [sp, #0x14]
00365b2c: ldr      r2, [sp, #0x18]
00365b30: ldr      r1, [sp, #0xa4]
00365b34: str      r3, [sp, #0xa4]
00365b38: ldr      r3, [sp, #0xa8]
00365b3c: add      r0, sp, #0x14
00365b40: str      r1, [sp, #0x14]
00365b44: str      r3, [sp, #0x18]
00365b48: str      r2, [sp, #0xa8]
00365b4c: mov      r8, #0
00365b50: bl       #0x619474
00365b54: mov      r0, sl
00365b58: bl       #0x619474
00365b5c: str      r8, [sp, #0xb0]
00365b60: bl       #0x60b0cc
00365b64: ldr      r3, [r7, #0x20]
00365b68: str      r0, [sp, #0xb4]
00365b6c: str      r8, [sp, #0xb8]
00365b70: cmp      r3, r8
00365b74: beq      #0x365b9c
00365b78: ldrb     r2, [r3, #0x70]
00365b7c: cmp      r2, r8
00365b80: beq      #0x365cd4
00365b84: mov      r0, r3
00365b88: add      r1, r6, #0x18
00365b8c: ldr      r3, [r3]
00365b90: mov      lr, pc
00365b94: ldr      pc, [r3, #0x2c]
00365b98: str      r0, [sp, #0xac]
00365b9c: ldr      r3, [sp, #4]
00365ba0: add      sl, sp, #0xc0
00365ba4: str      r3, [sl, #-0x68]!
00365ba8: add      r3, sl, #4
00365bac: mov      r0, r3
00365bb0: ldr      r2, [sp, #0x9c]
00365bb4: ldr      r1, [sp, #0xa0]
00365bb8: str      r3, [sp, #0x6c]
00365bbc: str      r3, [sp, #0x70]
00365bc0: bl       #0x3116e8
00365bc4: ldr      r3, [sp, #0xa4]
00365bc8: ldr      r2, [sp, #0xa8]
00365bcc: cmp      r3, #0
00365bd0: str      r2, [sp, #0x78]
00365bd4: str      r3, [sp, #0x74]
00365bd8: beq      #0x365bec
00365bdc: ldr      r2, [r3, #4]
00365be0: cmp      r2, #0
00365be4: addne    r2, r2, #1
00365be8: strne    r2, [r3, #4]
00365bec: ldr      r3, [sp, #0x58]
00365bf0: add      r8, sp, #0xc0
00365bf4: ldr      ip, [sp, #0xac]
00365bf8: ldr      lr, [sp, #0xb0]
00365bfc: ldr      sb, [sp, #0xb4]
00365c00: ldr      fp, [sp, #0xb8]
00365c04: str      r3, [r8, #-0x9c]!
00365c08: add      r3, r8, #4
00365c0c: mov      r0, r3
00365c10: ldr      r2, [sp, #0x6c]
00365c14: ldr      r1, [sp, #0x70]
00365c18: str      r3, [sp, #0x38]
00365c1c: str      r3, [sp, #0x3c]
00365c20: str      ip, [sp, #0x7c]
00365c24: str      lr, [sp, #0x80]
00365c28: str      sb, [sp, #0x84]
00365c2c: str      fp, [sp, #0x88]
00365c30: bl       #0x3116e8
00365c34: ldr      r3, [sp, #0x74]
00365c38: ldr      r2, [sp, #0x78]
00365c3c: cmp      r3, #0
00365c40: str      r2, [sp, #0x44]
00365c44: str      r3, [sp, #0x40]
00365c48: beq      #0x365c5c
00365c4c: ldr      r2, [r3, #4]
00365c50: cmp      r2, #0
00365c54: addne    r2, r2, #1
00365c58: strne    r2, [r3, #4]
00365c5c: ldr      r3, [sp, #0x7c]
00365c60: add      r7, r7, #8
00365c64: mov      r2, r8
00365c68: str      r3, [sp, #0x48]
00365c6c: ldr      r3, [sp, #0x80]
00365c70: mov      r1, r7
00365c74: add      r0, sp, #0xc
00365c78: str      r3, [sp, #0x4c]
00365c7c: ldr      r3, [sp, #0x84]
00365c80: str      r3, [sp, #0x50]
00365c84: ldr      r3, [sp, #0x88]
00365c88: str      r3, [sp, #0x54]
00365c8c: bl       #0x365340
00365c90: add      r0, r8, #0x1c
00365c94: bl       #0x619474
00365c98: add      r0, r8, #4
00365c9c: bl       #0x3139ac
00365ca0: add      r0, sl, #0x1c
00365ca4: bl       #0x619474
00365ca8: add      r0, sl, #4
00365cac: bl       #0x3139ac
00365cb0: add      r1, sp, #4
00365cb4: mov      r0, r7
00365cb8: bl       #0x36583c
00365cbc: mov      r7, r0
00365cc0: add      r0, r6, #0x18
00365cc4: bl       #0x619474
00365cc8: mov      r0, r6
00365ccc: bl       #0x3139ac
00365cd0: b        #0x365a48
00365cd4: mov      r0, r3
00365cd8: bl       #0x65f044
00365cdc: str      r0, [sp, #0xac]
00365ce0: add      r1, r6, #0x18
00365ce4: ldr      r0, [r7, #0x20]
00365ce8: bl       #0x62ee9c
00365cec: b        #0x365b9c
00365cf0: bl       #0x30e310
00365cf4: mlseq    r2, r4, r0, pc
00365cf8: andeq    r4, r0, ip, lsr #1
00365cfc: ldrheq   fp, [r5], #-0x30
00365d00: andeq    r2, r0, r8, lsr r2
00365d04: strheq   r4, [r0], -ip
00365d08: subseq   fp, r5, r4, ror r3
00365d0c: andeq    r0, r0, r0, ror lr
00365d10: subseq   r4, r7, ip, lsl ip
00365d14: andeq    r4, r0, r0, lsl r7

# _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvRib
006e2c40: push     {r4, r5, r6, r7, r8, lr}
006e2c44: sub      sp, sp, #0x10
006e2c48: ldr      r4, [sp, #0x28]
006e2c4c: mov      lr, #0
006e2c50: add      ip, sp, #0x10
006e2c54: ldr      r7, [r4]
006e2c58: mov      r6, r1
006e2c5c: str      lr, [ip, #-4]!
006e2c60: mov      r5, r0
006e2c64: mov      r8, r3
006e2c68: mov      r1, lr
006e2c6c: mov      r3, ip
006e2c70: mov      r0, r6
006e2c74: add      ip, sp, #8
006e2c78: str      r7, [sp, #4]
006e2c7c: str      ip, [sp]
006e2c80: ldrb     r7, [sp, #0x2c]
006e2c84: bl       #0x66b814
006e2c88: tst      r0, r7
006e2c8c: bne      #0x6e2cbc
006e2c90: mov      r0, r5
006e2c94: mov      r1, r6
006e2c98: mov      r3, r8
006e2c9c: ldr      ip, [r5]
006e2ca0: ldr      r2, [sp, #0xc]
006e2ca4: mov      lr, pc
006e2ca8: ldr      pc, [ip, #0x28]
006e2cac: ldr      r3, [sp, #0xc]
006e2cb0: str      r3, [r4]
006e2cb4: add      sp, sp, #0x10
006e2cb8: pop      {r4, r5, r6, r7, r8, pc}
006e2cbc: ldr      r3, [sp, #8]
006e2cc0: ldr      r2, [sp, #0xc]
006e2cc4: str      r8, [sp, #4]
006e2cc8: str      r3, [sp]
006e2ccc: mov      r0, r5
006e2cd0: mov      r1, r6
006e2cd4: ldr      ip, [r5]
006e2cd8: add      r3, r2, #1
006e2cdc: mov      lr, pc
006e2ce0: ldr      pc, [ip, #0x20]
006e2ce4: b        #0x6e2cac

# _ZN6glitch7collada13CAnimationSet12addAnimationEPKNS0_10SAnimationE
006601fc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00660200: ldr      r3, [r0, #0xc]
00660204: ldr      fp, [r0, #0x10]
00660208: ldr      r2, [pc, #0x28c]
0066020c: sub      sp, sp, #0x14
00660210: rsb      fp, r3, fp
00660214: str      r1, [sp, #0xc]
00660218: asrs     fp, fp, #2
0066021c: add      r2, pc, r2
00660220: mov      r5, r0
00660224: ldr      r4, [r1, #0x10]
00660228: beq      #0x660330
0066022c: ldr      r1, [pc, #0x26c]
00660230: mov      r6, #0
00660234: mov      sl, #0xc
00660238: ldr      sb, [r2, r1]
0066023c: ldr      r2, [pc, #0x260]
00660240: mov      r8, #1
00660244: lsl      r7, r6, #2
00660248: add      r2, pc, r2
0066024c: str      r2, [sp, #8]
00660250: ldr      r1, [r3, r6, lsl #2]
00660254: ldr      r3, [r4, #8]
00660258: ldr      r2, [sb]
0066025c: ldr      r1, [r1, #8]
00660260: cmp      r3, #0x5b
00660264: mla      r2, sl, r1, r2
00660268: bhi      #0x660300
0066026c: lsr      r1, r3, #5
00660270: ldr      r2, [r2, r1, lsl #2]
00660274: and      r3, r3, #0x1f
00660278: ands     r2, r2, r8, lsl r3
0066027c: beq      #0x6602d0
00660280: ldr      r3, [r5, #0xc]
00660284: ldr      r1, [r4, #4]
00660288: ldr      r7, [r3, r7]
0066028c: ldr      r0, [r7, #4]
00660290: bl       #0x30e31c
00660294: cmp      r0, #0
00660298: bne      #0x6602d0
0066029c: ldr      r3, [r4, #8]
006602a0: cmp      r3, #0xe
006602a4: beq      #0x66031c
006602a8: cmp      r3, #0x56
006602ac: beq      #0x6602bc
006602b0: mov      r0, r6
006602b4: add      sp, sp, #0x14
006602b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006602bc: ldr      r0, [r7, #0xc]
006602c0: ldr      r1, [r4, #0xc]
006602c4: bl       #0x30e31c
006602c8: cmp      r0, #0
006602cc: beq      #0x6602b0
006602d0: add      r6, r6, #1
006602d4: cmp      r6, fp
006602d8: beq      #0x660330
006602dc: ldr      r3, [r5, #0xc]
006602e0: ldr      r2, [sb]
006602e4: lsl      r7, r6, #2
006602e8: ldr      r1, [r3, r6, lsl #2]
006602ec: ldr      r3, [r4, #8]
006602f0: ldr      r1, [r1, #8]
006602f4: cmp      r3, #0x5b
006602f8: mla      r2, sl, r1, r2
006602fc: bls      #0x66026c
00660300: ldr      r0, [sp, #8]
00660304: str      r2, [sp, #4]
00660308: str      r3, [sp]
0066030c: bl       #0x708eb0
00660310: ldr      r3, [sp]
00660314: ldr      r2, [sp, #4]
00660318: b        #0x66026c
0066031c: ldrb     r2, [r7, #0xc]
00660320: ldrb     r3, [r4, #0xc]
00660324: cmp      r2, r3
00660328: bne      #0x6602d0
0066032c: b        #0x6602b0
00660330: ldr      r0, [sp, #0xc]
00660334: bl       #0x611ae0
00660338: subs     r6, r0, #0
0066033c: mvneq    r0, #0
00660340: beq      #0x6602b4
00660344: ldr      sl, [r5, #0x10]
00660348: ldr      r3, [r5, #0x14]
0066034c: cmp      sl, r3
00660350: beq      #0x66039c
00660354: str      r4, [sl]
00660358: ldr      r3, [r5, #0x10]
0066035c: add      r3, r3, #4
00660360: str      r3, [r5, #0x10]
00660364: ldr      r8, [r5, #0x1c]
00660368: ldr      r3, [r5, #0x20]
0066036c: cmp      r8, r3
00660370: beq      #0x66041c
00660374: str      r6, [r8]
00660378: ldr      r3, [r5, #0x1c]
0066037c: add      r3, r3, #4
00660380: str      r3, [r5, #0x1c]
00660384: ldr      r3, [r5, #0xc]
00660388: ldr      r0, [r5, #0x10]
0066038c: rsb      r0, r3, r0
00660390: asr      r0, r0, #2
00660394: sub      r0, r0, #1
00660398: b        #0x6602b4
0066039c: ldr      r3, [r5, #0xc]
006603a0: rsb      r3, r3, sl
006603a4: asr      r3, r3, #2
006603a8: cmp      r3, #1
006603ac: addhs    r8, r3, r3
006603b0: addlo    r8, r3, #1
006603b4: cmn      r8, #0xc0000001
006603b8: bhi      #0x660414
006603bc: cmp      r3, r8
006603c0: bhi      #0x660414
006603c4: lsl      r8, r8, #2
006603c8: mov      r1, #0
006603cc: mov      r0, r8
006603d0: bl       #0x310568
006603d4: ldr      r1, [r5, #0xc]
006603d8: mov      r7, r0
006603dc: subs     sl, sl, r1
006603e0: moveq    sl, r0
006603e4: beq      #0x6603f4
006603e8: mov      r2, sl
006603ec: bl       #0x30df38
006603f0: add      sl, r0, sl
006603f4: str      r4, [sl], #4
006603f8: ldr      r0, [r5, #0xc]
006603fc: add      r8, r7, r8
00660400: bl       #0x310450
00660404: str      sl, [r5, #0x10]
00660408: str      r8, [r5, #0x14]
0066040c: str      r7, [r5, #0xc]
00660410: b        #0x660364
00660414: mvn      r8, #0xc0000000
00660418: b        #0x6603c4
0066041c: ldr      r3, [r5, #0x18]
00660420: rsb      r3, r3, r8
00660424: asr      r3, r3, #2
00660428: cmp      r3, #1
0066042c: addhs    r7, r3, r3
00660430: addlo    r7, r3, #1
00660434: cmn      r7, #0xc0000001
00660438: bhi      #0x660494
0066043c: cmp      r3, r7
00660440: bhi      #0x660494
00660444: lsl      r7, r7, #2
00660448: mov      r1, #0
0066044c: mov      r0, r7
00660450: bl       #0x310568
00660454: ldr      r1, [r5, #0x18]
00660458: mov      r4, r0
0066045c: subs     r8, r8, r1
00660460: moveq    r8, r0
00660464: beq      #0x660474
00660468: mov      r2, r8
0066046c: bl       #0x30df38
00660470: add      r8, r0, r8
00660474: str      r6, [r8], #4
00660478: ldr      r0, [r5, #0x18]
0066047c: add      r7, r4, r7
00660480: bl       #0x310450
00660484: str      r8, [r5, #0x1c]
00660488: str      r7, [r5, #0x20]
0066048c: str      r4, [r5, #0x18]
00660490: b        #0x660384
00660494: mvn      r7, #0xc0000000
00660498: b        #0x660444
0066049c: eorseq   r4, r3, r4, ror r8
006604a0: andeq    r4, r0, ip, asr #10
006604a4: eoreq    r1, r6, r0, lsl #21

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIhLi30EEEbRKNS_3res6vectorIiEEiRii
0066a894: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a898: sub      sp, sp, #0x1c
0066a89c: str      r0, [sp, #0x10]
0066a8a0: mov      r0, r2
0066a8a4: str      r2, [sp, #4]
0066a8a8: mov      r4, r1
0066a8ac: str      r3, [sp, #8]
0066a8b0: bl       #0x30e964
0066a8b4: movw     r1, #0x5555
0066a8b8: movt     r1, #0x4205
0066a8bc: str      r0, [sp, #0xc]
0066a8c0: bl       #0x30ec94
0066a8c4: ldr      r6, [r4]
0066a8c8: ldr      r5, [sp, #0x40]
0066a8cc: ldr      r8, [r4, #4]
0066a8d0: sub      r6, r6, #1
0066a8d4: bic      r5, r5, r5, asr #31
0066a8d8: cmp      r5, r6
0066a8dc: movge    r5, r6
0066a8e0: mov      r7, r0
0066a8e4: ldrb     r0, [r8, r5]
0066a8e8: bl       #0x30e964
0066a8ec: mov      r1, r7
0066a8f0: bl       #0x30e2f8
0066a8f4: cmp      r0, #0
0066a8f8: mov      sl, #0
0066a8fc: movne    sl, #1
0066a900: uxtb     sl, sl
0066a904: cmp      sl, #0
0066a908: beq      #0x66a96c
0066a90c: cmp      r5, #0
0066a910: subgt    r5, r5, #1
0066a914: ble      #0x66a96c
0066a918: cmp      r5, r6
0066a91c: bge      #0x66aa60
0066a920: ldrb     r0, [r8, r5]
0066a924: bl       #0x30e964
0066a928: mov      r1, r0
0066a92c: mov      r0, r7
0066a930: bl       #0x30e70c
0066a934: cmp      r0, #0
0066a938: mov      sl, #0
0066a93c: movne    sl, #1
0066a940: mov      fp, r5
0066a944: uxtb     sl, sl
0066a948: mov      sb, r5
0066a94c: cmp      sl, #0
0066a950: beq      #0x66aa3c
0066a954: ldr      r0, [sp, #0x10]
0066a958: ldmib    sp, {r2, r3}
0066a95c: mov      r1, r4
0066a960: add      sp, sp, #0x1c
0066a964: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a968: b        #0x66a6d0
0066a96c: cmp      r6, r5
0066a970: bgt      #0x66a9d0
0066a974: mov      sb, r5
0066a978: mov      fp, sb
0066a97c: mov      r5, sb
0066a980: ldr      r3, [sp, #8]
0066a984: str      r5, [r3]
0066a988: ldr      r3, [r4, #4]
0066a98c: ldrb     r0, [r3, fp]
0066a990: bl       #0x30e964
0066a994: movw     r1, #0x5555
0066a998: movt     r1, #0x4205
0066a99c: bl       #0x30ed6c
0066a9a0: mov      r1, r0
0066a9a4: ldr      r0, [sp, #0xc]
0066a9a8: bl       #0x30df8c
0066a9ac: cmp      r0, #0
0066a9b0: movne    r0, #0
0066a9b4: bne      #0x66a9c8
0066a9b8: ldr      r0, [r4]
0066a9bc: sub      r0, r0, #1
0066a9c0: subs     r0, r5, r0
0066a9c4: movne    r0, #1
0066a9c8: add      sp, sp, #0x1c
0066a9cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a9d0: add      sb, r5, #1
0066a9d4: ldrb     r0, [r8, sb]
0066a9d8: bl       #0x30e964
0066a9dc: mov      r1, r7
0066a9e0: str      r0, [sp, #0x14]
0066a9e4: bl       #0x30e70c
0066a9e8: cmp      r0, #0
0066a9ec: mov      fp, sb
0066a9f0: moveq    fp, r5
0066a9f4: moveq    sb, fp
0066a9f8: beq      #0x66a94c
0066a9fc: cmp      r6, sb
0066aa00: ble      #0x66a978
0066aa04: add      r5, sb, #1
0066aa08: ldrb     r0, [r8, r5]
0066aa0c: bl       #0x30e964
0066aa10: mov      r1, r7
0066aa14: bl       #0x30e70c
0066aa18: subs     sl, r0, #0
0066aa1c: bne      #0x66a918
0066aa20: ldr      r1, [sp, #0x14]
0066aa24: mov      r0, r7
0066aa28: bl       #0x30e70c
0066aa2c: cmp      r0, #0
0066aa30: movne    sl, #1
0066aa34: uxtb     sl, sl
0066aa38: b        #0x66a94c
0066aa3c: add      r8, r8, sb
0066aa40: ldrb     r0, [r8, #1]
0066aa44: bl       #0x30e964
0066aa48: mov      r1, r7
0066aa4c: bl       #0x30e70c
0066aa50: cmp      r0, #0
0066aa54: moveq    r5, sb
0066aa58: beq      #0x66a980
0066aa5c: b        #0x66a954
0066aa60: mov      fp, r5
0066aa64: b        #0x66a980

# _ZN11AnimatorSet19setCurrentAnimationEi
00367420: push     {r4, r5, r6, lr}
00367424: mov      r4, r0
00367428: ldr      r0, [r0, #0x94]
0036742c: mov      r5, r1
00367430: bl       #0x364bf4
00367434: ldr      r3, [r0, #0x20]
00367438: cmn      r3, #1
0036743c: beq      #0x367480
00367440: ldr      r2, [r0, #0x24]
00367444: ldr      r3, [r0, #0x2c]
00367448: mov      r1, r5
0036744c: add      r2, r2, #1
00367450: add      r3, r3, #1
00367454: str      r2, [r0, #0x24]
00367458: str      r3, [r0, #0x2c]
0036745c: ldr      r3, [r4, #0x98]
00367460: str      r0, [r4, #0x98]
00367464: mov      r0, r4
00367468: cmp      r3, #0
0036746c: ldrne    r2, [r3, #0x24]
00367470: subne    r2, r2, #1
00367474: strne    r2, [r3, #0x24]
00367478: pop      {r4, r5, r6, lr}
0036747c: b        #0x65f8c8
00367480: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada20CDynamicAnimationSet16getDatabaseIndexERNS0_16CColladaDatabaseE
0062dbb8: ldr      r2, [r0, #0x28]
0062dbbc: ldr      r3, [r0, #0x24]
0062dbc0: rsb      r2, r3, r2
0062dbc4: asrs     r2, r2, #3
0062dbc8: beq      #0x62dc00
0062dbcc: ldr      ip, [r1]
0062dbd0: ldr      r1, [r3]
0062dbd4: cmp      r1, ip
0062dbd8: moveq    r0, #0
0062dbdc: bxeq     lr
0062dbe0: mov      r0, #0
0062dbe4: b        #0x62dbf4
0062dbe8: ldr      r1, [r3, r0, lsl #3]
0062dbec: cmp      r1, ip
0062dbf0: beq      #0x62dc08
0062dbf4: add      r0, r0, #1
0062dbf8: cmp      r0, r2
0062dbfc: bne      #0x62dbe8
0062dc00: mvn      r0, #0
0062dc04: bx       lr
0062dc08: bx       lr

# _ZN6glitch7collada21CSceneNodeAnimatorSet4initERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
00660af4: push     {r4, r5, r6, r7, lr}
00660af8: ldr      r3, [r1]
00660afc: mov      r4, r0
00660b00: sub      sp, sp, #0x14
00660b04: cmp      r3, #0
00660b08: ldrne    r2, [r3, #4]
00660b0c: addne    r2, r2, #1
00660b10: strne    r2, [r3, #4]
00660b14: ldr      r0, [r0, #0x24]
00660b18: str      r3, [r4, #0x24]
00660b1c: cmp      r0, #0
00660b20: beq      #0x660b2c
00660b24: bl       #0x31d584
00660b28: ldr      r3, [r4, #0x24]
00660b2c: mov      r0, r3
00660b30: bl       #0x65f058
00660b34: add      r7, r4, #0x28
00660b38: mov      r5, r0
00660b3c: mov      r1, r0
00660b40: mov      r0, r7
00660b44: bl       #0x65fcc4
00660b48: mov      r6, #0
00660b4c: add      r2, sp, #0x10
00660b50: str      r6, [r2, #-4]!
00660b54: mov      r0, r7
00660b58: mov      r1, r5
00660b5c: bl       #0x65ec40
00660b60: cmp      r5, r6
00660b64: ble      #0x660b80
00660b68: mov      r2, r6
00660b6c: ldr      r3, [r4, #0x28]
00660b70: str      r2, [r3, r6, lsl #2]
00660b74: add      r6, r6, #1
00660b78: cmp      r6, r5
00660b7c: bne      #0x660b6c
00660b80: add      r7, r4, #0x34
00660b84: mov      r0, r7
00660b88: mov      r1, r5
00660b8c: bl       #0x65fda8
00660b90: mov      r6, #0
00660b94: add      r2, sp, #0x10
00660b98: str      r6, [r2, #-8]!
00660b9c: mov      r0, r7
00660ba0: mov      r1, r5
00660ba4: add      r7, r4, #0x40
00660ba8: bl       #0x65ed54
00660bac: mov      r0, r7
00660bb0: mov      r1, r5
00660bb4: bl       #0x65fe8c
00660bb8: add      r2, sp, #0x10
00660bbc: str      r6, [r2, #-0xc]!
00660bc0: mov      r0, r7
00660bc4: mov      r1, r5
00660bc8: bl       #0x660568
00660bcc: mov      r1, r6
00660bd0: mov      r0, #0x48
00660bd4: bl       #0x5341ac
00660bd8: mov      r5, r0
00660bdc: bl       #0x666e40
00660be0: mov      r0, r4
00660be4: mov      r1, r5
00660be8: ldr      r3, [r4]
00660bec: mov      lr, pc
00660bf0: ldr      pc, [r3, #0x3c]
00660bf4: mov      r0, r4
00660bf8: ldr      r3, [r4]
00660bfc: mov      r1, r6
00660c00: mov      lr, pc
00660c04: ldr      pc, [r3, #0x88]
00660c08: ldr      r3, [r5]
00660c0c: ldr      r0, [r3, #-0xc]
00660c10: add      r0, r5, r0
00660c14: bl       #0x31d584
00660c18: add      sp, sp, #0x14
00660c1c: pop      {r4, r5, r6, r7, pc}

# _ZNK6glitch7collada16CColladaDatabase7getNodeEPKc
0061c290: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c294: mov      r8, r1
0061c298: mov      r1, #0
0061c29c: mov      r7, r0
0061c2a0: bl       #0x60e54c
0061c2a4: subs     r6, r0, #0
0061c2a8: bne      #0x61c2b4
0061c2ac: mov      r0, #0
0061c2b0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c2b4: ldr      sl, [r6, #8]
0061c2b8: cmp      sl, #0
0061c2bc: ble      #0x61c2ac
0061c2c0: mov      r4, #0
0061c2c4: mov      r5, r4
0061c2c8: ldr      r2, [r6, #0xc]
0061c2cc: mov      r0, r7
0061c2d0: mov      r1, r8
0061c2d4: add      r2, r2, r4
0061c2d8: bl       #0x61c214
0061c2dc: cmp      r0, #0
0061c2e0: add      r5, r5, #1
0061c2e4: bne      #0x61c2b0
0061c2e8: cmp      r5, sl
0061c2ec: add      r4, r4, #0x50
0061c2f0: bne      #0x61c2c8
0061c2f4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch7collada20CDynamicAnimationSet11clearTracksEv
0062f54c: push     {r4, r5, lr}
0062f550: ldr      r1, [r0, #0x78]
0062f554: ldr      lr, [r0, #0x74]
0062f558: sub      sp, sp, #0x24
0062f55c: mov      ip, #0
0062f560: mov      r4, r0
0062f564: add      r0, sp, #4
0062f568: str      ip, [r0], #4
0062f56c: rsb      r2, lr, r1
0062f570: str      ip, [r0], #4
0062f574: asrs     r2, r2, #4
0062f578: str      ip, [r0]
0062f57c: str      ip, [sp]
0062f580: beq      #0x62f5e4
0062f584: cmp      r1, lr
0062f588: ldr      r3, [r4, #0x18]
0062f58c: ldr      r1, [r4, #0x1c]
0062f590: mov      r2, #0
0062f594: strne    lr, [r4, #0x78]
0062f598: str      r2, [sp, #0x1c]
0062f59c: rsb      r2, r3, r1
0062f5a0: asrs     r2, r2, #2
0062f5a4: beq      #0x62f60c
0062f5a8: cmp      r1, r3
0062f5ac: strne    r3, [r4, #0x1c]
0062f5b0: mov      r5, #0
0062f5b4: add      r0, r4, #0x30
0062f5b8: mov      r1, r5
0062f5bc: add      r2, sp, #0x10
0062f5c0: str      r5, [sp, #0x10]
0062f5c4: str      r5, [sp, #0x14]
0062f5c8: str      r5, [sp, #0x18]
0062f5cc: bl       #0x62ec50
0062f5d0: mov      r3, #1
0062f5d4: strb     r3, [r4, #0x70]
0062f5d8: str      r5, [r4, #0x3c]
0062f5dc: add      sp, sp, #0x24
0062f5e0: pop      {r4, r5, pc}
0062f5e4: mov      r3, sp
0062f5e8: add      r0, r4, #0x74
0062f5ec: bl       #0x62e854
0062f5f0: ldr      r1, [r4, #0x1c]
0062f5f4: ldr      r3, [r4, #0x18]
0062f5f8: mov      r2, #0
0062f5fc: str      r2, [sp, #0x1c]
0062f600: rsb      r2, r3, r1
0062f604: asrs     r2, r2, #2
0062f608: bne      #0x62f5a8
0062f60c: add      r0, r4, #0x18
0062f610: add      r3, sp, #0x1c
0062f614: bl       #0x62f300
0062f618: b        #0x62f5b0

# _ZN6glitch7collada18ISceneNodeAnimator21deserializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
00667cb8: bx       lr

# _ZN11AnimatorSet19SetCurrentAnimationEi
003674ac: push     {r4, r5, r6, lr}
003674b0: mov      r5, r0
003674b4: ldr      r0, [r0, #0x94]
003674b8: bl       #0x3660f4
003674bc: mov      r4, r0
003674c0: ldr      r0, [r0, #0x20]
003674c4: cmn      r0, #1
003674c8: beq      #0x36750c
003674cc: ldr      r2, [r4, #0x24]
003674d0: ldr      r3, [r4, #0x2c]
003674d4: mov      r0, r5
003674d8: add      r2, r2, #1
003674dc: add      r3, r3, #1
003674e0: str      r2, [r4, #0x24]
003674e4: str      r3, [r4, #0x2c]
003674e8: ldr      r3, [r5, #0x98]
003674ec: str      r4, [r5, #0x98]
003674f0: cmp      r3, #0
003674f4: ldrne    r2, [r3, #0x24]
003674f8: subne    r2, r2, #1
003674fc: strne    r2, [r3, #0x24]
00367500: ldr      r1, [r4, #0x20]
00367504: bl       #0x65f8c8
00367508: ldr      r0, [r4, #0x20]
0036750c: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiRKNS_3res6vectorIiEEiRiRfi
0066b65c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b660: ldr      ip, [r0, #8]
0066b664: sub      sp, sp, #0x14
0066b668: mov      r4, r0
0066b66c: ldrb     r8, [ip, #0xd]
0066b670: mov      sb, r1
0066b674: mov      sl, r2
0066b678: cmp      r8, #0
0066b67c: mov      r7, r3
0066b680: ldr      r6, [sp, #0x38]
0066b684: ldr      r5, [sp, #0x3c]
0066b688: ldr      fp, [sp, #0x40]
0066b68c: beq      #0x66b6e8
0066b690: ldr      r3, [ip, #4]
0066b694: cmp      r3, r7
0066b698: beq      #0x66b6c4
0066b69c: str      r7, [ip, #4]
0066b6a0: mov      r1, #0
0066b6a4: bl       #0x669e84
0066b6a8: cmp      r0, #3
0066b6ac: beq      #0x66b7bc
0066b6b0: cmp      r0, #4
0066b6b4: beq      #0x66b78c
0066b6b8: cmp      r0, #1
0066b6bc: beq      #0x66b734
0066b6c0: ldr      ip, [r4, #8]
0066b6c4: ldr      r3, [ip]
0066b6c8: str      r3, [r5]
0066b6cc: ldr      r3, [r4, #8]
0066b6d0: ldr      r3, [r3, #8]
0066b6d4: str      r3, [r6]
0066b6d8: ldr      r3, [r4, #8]
0066b6dc: ldrb     r0, [r3, #0xc]
0066b6e0: add      sp, sp, #0x14
0066b6e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066b6e8: mov      r1, r8
0066b6ec: bl       #0x669e84
0066b6f0: cmp      r0, #3
0066b6f4: beq      #0x66b7ec
0066b6f8: cmp      r0, #4
0066b6fc: beq      #0x66b764
0066b700: cmp      r0, #1
0066b704: movne    r0, r8
0066b708: bne      #0x66b6e0
0066b70c: mov      r0, r4
0066b710: mov      r1, sb
0066b714: mov      r2, sl
0066b718: mov      r3, r7
0066b71c: str      r6, [sp, #0x38]
0066b720: str      r5, [sp, #0x3c]
0066b724: str      fp, [sp, #0x40]
0066b728: add      sp, sp, #0x14
0066b72c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b730: b        #0x66aab4
0066b734: ldr      r8, [r4, #8]
0066b738: mov      r1, sb
0066b73c: mov      r2, sl
0066b740: add      ip, r8, #8
0066b744: mov      r3, r7
0066b748: mov      r0, r4
0066b74c: str      ip, [sp]
0066b750: stmib    sp, {r8, fp}
0066b754: bl       #0x66aab4
0066b758: strb     r0, [r8, #0xc]
0066b75c: ldr      ip, [r4, #8]
0066b760: b        #0x66b6c4
0066b764: mov      r0, r4
0066b768: mov      r1, sb
0066b76c: mov      r2, sl
0066b770: mov      r3, r7
0066b774: str      r6, [sp, #0x38]
0066b778: str      r5, [sp, #0x3c]
0066b77c: str      fp, [sp, #0x40]
0066b780: add      sp, sp, #0x14
0066b784: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b788: b        #0x66a5e0
0066b78c: ldr      r8, [r4, #8]
0066b790: mov      r1, sb
0066b794: mov      r2, sl
0066b798: add      ip, r8, #8
0066b79c: mov      r3, r7
0066b7a0: mov      r0, r4
0066b7a4: str      ip, [sp]
0066b7a8: stmib    sp, {r8, fp}
0066b7ac: bl       #0x66a5e0
0066b7b0: strb     r0, [r8, #0xc]
0066b7b4: ldr      ip, [r4, #8]
0066b7b8: b        #0x66b6c4
0066b7bc: ldr      r8, [r4, #8]
0066b7c0: mov      r1, sb
0066b7c4: mov      r2, sl
0066b7c8: add      ip, r8, #8
0066b7cc: mov      r3, r7
0066b7d0: mov      r0, r4
0066b7d4: str      ip, [sp]
0066b7d8: stmib    sp, {r8, fp}
0066b7dc: bl       #0x66b550
0066b7e0: strb     r0, [r8, #0xc]
0066b7e4: ldr      ip, [r4, #8]
0066b7e8: b        #0x66b6c4
0066b7ec: mov      r0, r4
0066b7f0: mov      r1, sb
0066b7f4: mov      r2, sl
0066b7f8: mov      r3, r7
0066b7fc: str      r6, [sp, #0x38]
0066b800: str      r5, [sp, #0x3c]
0066b804: str      fp, [sp, #0x40]
0066b808: add      sp, sp, #0x14
0066b80c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b810: b        #0x66b550

# _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKcNS0_8SChannel4TypeEPPvS6_
0061c2f8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061c2fc: ldr      r4, [pc, #0x3a4]
0061c300: ldr      r5, [pc, #0x3a4]
0061c304: mov      r7, r3
0061c308: add      r4, pc, r4
0061c30c: ldr      ip, [r4, r5]
0061c310: sub      sp, sp, #0x7c
0061c314: mov      r6, r1
0061c318: ldr      r3, [ip]
0061c31c: ldr      r8, [sp, #0xa0]
0061c320: str      r3, [sp, #0x74]
0061c324: cmp      r2, #0x5b
0061c328: addls    pc, pc, r2, lsl #2
0061c32c: b        #0x61c5d4
0061c330: b        #0x61c5e0
0061c334: b        #0x61c660
0061c338: b        #0x61c660
0061c33c: b        #0x61c660
0061c340: b        #0x61c660
0061c344: b        #0x61c670
0061c348: b        #0x61c670
0061c34c: b        #0x61c670
0061c350: b        #0x61c670
0061c354: b        #0x61c670
0061c358: b        #0x61c548
0061c35c: b        #0x61c548
0061c360: b        #0x61c5e8
0061c364: b        #0x61c600
0061c368: b        #0x61c61c
0061c36c: b        #0x61c524
0061c370: b        #0x61c564
0061c374: b        #0x61c524
0061c378: b        #0x61c524
0061c37c: b        #0x61c524
0061c380: b        #0x61c644
0061c384: b        #0x61c524
0061c388: b        #0x61c524
0061c38c: b        #0x61c524
0061c390: b        #0x61c524
0061c394: b        #0x61c524
0061c398: b        #0x61c524
0061c39c: b        #0x61c5d4
0061c3a0: b        #0x61c5d4
0061c3a4: b        #0x61c5d4
0061c3a8: b        #0x61c5d4
0061c3ac: b        #0x61c5d4
0061c3b0: b        #0x61c5d4
0061c3b4: b        #0x61c5d4
0061c3b8: b        #0x61c5d4
0061c3bc: b        #0x61c5d4
0061c3c0: b        #0x61c5d4
0061c3c4: b        #0x61c5d4
0061c3c8: b        #0x61c5d4
0061c3cc: b        #0x61c5d4
0061c3d0: b        #0x61c5d4
0061c3d4: b        #0x61c5d4
0061c3d8: b        #0x61c5d4
0061c3dc: b        #0x61c5d4
0061c3e0: b        #0x61c5d4
0061c3e4: b        #0x61c5d4
0061c3e8: b        #0x61c5d4
0061c3ec: b        #0x61c5d4
0061c3f0: b        #0x61c5d4
0061c3f4: b        #0x61c5d4
0061c3f8: b        #0x61c5d4
0061c3fc: b        #0x61c5d4
0061c400: b        #0x61c5d4
0061c404: b        #0x61c5d4
0061c408: b        #0x61c5d4
0061c40c: b        #0x61c5d4
0061c410: b        #0x61c5d4
0061c414: b        #0x61c5d4
0061c418: b        #0x61c5d4
0061c41c: b        #0x61c5d4
0061c420: b        #0x61c5d4
0061c424: b        #0x61c5d4
0061c428: b        #0x61c5d4
0061c42c: b        #0x61c5d4
0061c430: b        #0x61c5d4
0061c434: b        #0x61c5d4
0061c438: b        #0x61c5d4
0061c43c: b        #0x61c5d4
0061c440: b        #0x61c5d4
0061c444: b        #0x61c5d4
0061c448: b        #0x61c5d4
0061c44c: b        #0x61c524
0061c450: b        #0x61c524
0061c454: b        #0x61c524
0061c458: b        #0x61c524
0061c45c: b        #0x61c524
0061c460: b        #0x61c524
0061c464: b        #0x61c524
0061c468: b        #0x61c524
0061c46c: b        #0x61c524
0061c470: b        #0x61c524
0061c474: b        #0x61c524
0061c478: b        #0x61c524
0061c47c: b        #0x61c524
0061c480: b        #0x61c524
0061c484: b        #0x61c524
0061c488: b        #0x61c580
0061c48c: b        #0x61c524
0061c490: b        #0x61c524
0061c494: b        #0x61c524
0061c498: b        #0x61c524
0061c49c: b        #0x61c524
0061c4a0: ldr      r1, [pc, #0x208]
0061c4a4: add      sb, sp, #0x5c
0061c4a8: add      r8, sp, #0x44
0061c4ac: add      r1, pc, r1
0061c4b0: add      r2, sp, #0x10
0061c4b4: mov      r0, sb
0061c4b8: bl       #0x32603c
0061c4bc: mov      r2, r6
0061c4c0: mov      r0, r8
0061c4c4: mov      r1, sb
0061c4c8: bl       #0x56d8b0
0061c4cc: ldr      r1, [pc, #0x1e0]
0061c4d0: add      r6, sp, #0x2c
0061c4d4: add      r2, sp, #0xc
0061c4d8: add      r1, pc, r1
0061c4dc: add      sl, sp, #0x14
0061c4e0: mov      r0, r6
0061c4e4: bl       #0x32603c
0061c4e8: mov      r2, r6
0061c4ec: mov      r0, sl
0061c4f0: mov      r1, r8
0061c4f4: bl       #0x34e324
0061c4f8: mov      r1, #2
0061c4fc: ldr      r0, [sp, #0x28]
0061c500: bl       #0x60aca0
0061c504: mov      r0, sl
0061c508: bl       #0x60f494
0061c50c: mov      r0, r6
0061c510: bl       #0x60f494
0061c514: mov      r0, r8
0061c518: bl       #0x60f494
0061c51c: mov      r0, sb
0061c520: bl       #0x60f494
0061c524: mov      r0, #0
0061c528: str      r0, [r7]
0061c52c: ldr      r3, [r4, r5]
0061c530: ldr      r2, [sp, #0x74]
0061c534: ldr      r3, [r3]
0061c538: cmp      r2, r3
0061c53c: bne      #0x61c6a4
0061c540: add      sp, sp, #0x7c
0061c544: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061c548: bl       #0x61c290
0061c54c: cmp      r0, #0
0061c550: addne    r0, r0, #0x28
0061c554: strne    r0, [r7]
0061c558: movne    r0, #1
0061c55c: bne      #0x61c52c
0061c560: b        #0x61c524
0061c564: bl       #0x61b1ec
0061c568: cmp      r0, #0
0061c56c: beq      #0x61c524
0061c570: add      r0, r0, #0xc
0061c574: str      r0, [r7]
0061c578: mov      r0, #1
0061c57c: b        #0x61c52c
0061c580: bl       #0x61ac28
0061c584: subs     sb, r0, #0
0061c588: beq      #0x61c4a0
0061c58c: ldr      fp, [sb, #0x10]
0061c590: cmp      fp, #0
0061c594: ble      #0x61c524
0061c598: ldr      r3, [pc, #0x118]
0061c59c: mov      r6, #0
0061c5a0: mov      sl, r6
0061c5a4: add      r3, pc, r3
0061c5a8: ldr      r2, [sb, #0x14]
0061c5ac: ldr      r1, [r8]
0061c5b0: add      r2, r2, r6
0061c5b4: ldr      r2, [r2, #4]
0061c5b8: cmp      r1, r2
0061c5bc: beq      #0x61c68c
0061c5c0: add      sl, sl, #1
0061c5c4: cmp      sl, fp
0061c5c8: add      r6, r6, #0x18
0061c5cc: bne      #0x61c5a8
0061c5d0: b        #0x61c524
0061c5d4: mov      r3, #0
0061c5d8: str      r3, [r7]
0061c5dc: b        #0x61c524
0061c5e0: bl       #0x61c290
0061c5e4: b        #0x61c524
0061c5e8: bl       #0x61c290
0061c5ec: cmp      r0, #0
0061c5f0: addne    r0, r0, #0x2c
0061c5f4: strne    r0, [r7]
0061c5f8: movne    r0, #1
0061c5fc: b        #0x61c52c
0061c600: bl       #0x61c290
0061c604: cmp      r0, #0
0061c608: addne    r0, r0, #0x30
0061c60c: strne    r0, [r7]
0061c610: movne    r0, #1
0061c614: bne      #0x61c52c
0061c618: b        #0x61c524
0061c61c: bl       #0x61a9a0
0061c620: cmp      r0, #0
0061c624: beq      #0x61c524
0061c628: ldr      r3, [r0, #8]
0061c62c: ldrb     r2, [r8]
0061c630: mov      r0, #1
0061c634: ldr      r3, [r3, #0x1c]
0061c638: add      r3, r3, r2, lsl #2
0061c63c: str      r3, [r7]
0061c640: b        #0x61c52c
0061c644: bl       #0x61c290
0061c648: cmp      r0, #0
0061c64c: addne    r0, r0, #0x34
0061c650: strne    r0, [r7]
0061c654: movne    r0, #1
0061c658: bne      #0x61c52c
0061c65c: b        #0x61c524
0061c660: bl       #0x61c290
0061c664: cmp      r0, #0
0061c668: bne      #0x61c570
0061c66c: b        #0x61c524
0061c670: bl       #0x61c290
0061c674: cmp      r0, #0
0061c678: addne    r0, r0, #0x18
0061c67c: strne    r0, [r7]
0061c680: movne    r0, #1
0061c684: bne      #0x61c52c
0061c688: b        #0x61c524
0061c68c: mov      r0, r3
0061c690: mov      r1, #1
0061c694: str      r3, [sp, #4]
0061c698: bl       #0x60aca0
0061c69c: ldr      r3, [sp, #4]
0061c6a0: b        #0x61c5c0
0061c6a4: bl       #0x30e310
0061c6a8: eorseq   r8, r7, r8, lsl #15
0061c6ac: andeq    r4, r0, ip, lsr #1
0061c6b0: eoreq    r8, ip, r4, lsl #18
0061c6b4: eoreq    r8, ip, r8, ror #17
0061c6b8: eoreq    r8, ip, ip, lsr #16

# _ZNK6glitch7collada16CColladaDatabase12getAnimationEPKcNS0_8SChannel4TypeEh
0061c0c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c0cc: mov      r4, r0
0061c0d0: ldr      r0, [r0]
0061c0d4: mov      r6, r2
0061c0d8: mov      sb, r3
0061c0dc: ldr      r2, [r0, #0x24]
0061c0e0: mov      r5, r1
0061c0e4: ldr      r3, [r2, #0x20]
0061c0e8: ldr      sl, [r3, #0x24]
0061c0ec: cmp      sl, #0
0061c0f0: ble      #0x61c1ac
0061c0f4: mov      r7, #0
0061c0f8: b        #0x61c12c
0061c0fc: cmp      r6, #1
0061c100: blo      #0x61c114
0061c104: cmp      r6, #4
0061c108: bls      #0x61c190
0061c10c: cmp      r6, #5
0061c110: beq      #0x61c1b8
0061c114: ldr      r2, [r3, #8]
0061c118: cmp      r2, r6
0061c11c: beq      #0x61c1d0
0061c120: add      r7, r7, #1
0061c124: cmp      r7, sl
0061c128: beq      #0x61c1ac
0061c12c: mov      r0, r4
0061c130: mov      r1, r7
0061c134: bl       #0x60e35c
0061c138: cmp      r6, #9
0061c13c: mov      r8, r0
0061c140: ldr      r3, [r0, #0x10]
0061c144: beq      #0x61c1b8
0061c148: bls      #0x61c0fc
0061c14c: cmp      r6, #0x57
0061c150: blo      #0x61c114
0061c154: cmp      r6, #0x5b
0061c158: bls      #0x61c164
0061c15c: cmp      r6, #0x100
0061c160: bne      #0x61c114
0061c164: ldr      r2, [r3, #8]
0061c168: sub      r2, r2, #0x57
0061c16c: cmp      r2, #4
0061c170: bhi      #0x61c120
0061c174: ldr      r0, [r3, #4]
0061c178: mov      r1, r5
0061c17c: bl       #0x30e31c
0061c180: cmp      r0, #0
0061c184: bne      #0x61c120
0061c188: mov      r0, r8
0061c18c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c190: ldr      r2, [r3, #8]
0061c194: sub      r2, r2, #1
0061c198: cmp      r2, #3
0061c19c: bls      #0x61c174
0061c1a0: add      r7, r7, #1
0061c1a4: cmp      r7, sl
0061c1a8: bne      #0x61c12c
0061c1ac: mov      r8, #0
0061c1b0: mov      r0, r8
0061c1b4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c1b8: ldr      r2, [r3, #8]
0061c1bc: cmp      r2, #5
0061c1c0: beq      #0x61c174
0061c1c4: cmp      r2, #9
0061c1c8: bne      #0x61c120
0061c1cc: b        #0x61c174
0061c1d0: ldrb     r2, [r3, #0xc]
0061c1d4: cmp      r2, sb
0061c1d8: bne      #0x61c120
0061c1dc: b        #0x61c174

# _ZN6glitch7collada18ISceneNodeAnimatorC2Ev
006698fc: push     {r4, r5, r6, r7, r8, lr}
00669900: add      r7, r1, #4
00669904: ldr      r5, [pc, #0xa4]
00669908: ldr      r2, [r7, #4]
0066990c: ldr      r3, [pc, #0xa0]
00669910: add      r5, pc, r5
00669914: str      r2, [r0]
00669918: ldr      r3, [r5, r3]
0066991c: mov      r6, r1
00669920: ldr      r2, [r2, #-0xc]
00669924: ldr      r1, [r7, #8]
00669928: add      r3, r3, #8
0066992c: mov      r4, r0
00669930: str      r1, [r0, r2]
00669934: str      r3, [r0, #4]
00669938: bl       #0x6a118c
0066993c: ldr      r1, [r6, #4]
00669940: ldr      r2, [pc, #0x70]
00669944: mov      r3, #0
00669948: str      r1, [r4]
0066994c: ldr      r2, [r5, r2]
00669950: ldr      r0, [r1, #-0xc]
00669954: ldr      ip, [r7, #0xc]
00669958: add      r2, r2, #0x68
0066995c: ldr      r1, [pc, #0x58]
00669960: str      ip, [r4, r0]
00669964: stmib    r4, {r2, r3}
00669968: ldr      r0, [r6]
0066996c: ldr      r2, [pc, #0x4c]
00669970: ldr      r1, [r5, r1]
00669974: str      r0, [r4]
00669978: ldr      ip, [r0, #-0xc]
0066997c: ldr      r2, [r5, r2]
00669980: ldr      r5, [r6, #0x14]
00669984: add      r1, r1, #0x9c
00669988: mov      r0, r4
0066998c: str      r5, [r4, ip]
00669990: str      r1, [r4, #4]
00669994: str      r2, [r4, #0x1c]
00669998: str      r3, [r4, #0x20]
0066999c: str      r3, [r4, #0xc]
006699a0: str      r3, [r4, #0x10]
006699a4: str      r3, [r4, #0x14]
006699a8: str      r3, [r4, #0x18]
006699ac: pop      {r4, r5, r6, r7, r8, pc}
006699b0: eorseq   fp, r2, r0, lsl #3
006699b4: andeq    r2, r0, ip, asr #14
006699b8: andeq    r2, r0, r8, lsl #6
006699bc: andeq    r4, r0, r4, asr #2
006699c0: andeq    r4, r0, ip, lsr #10

# _ZN6glitch7collada13CAnimationSet19addAnimationLibraryERKNS0_16CColladaDatabaseE
006601d4: push     {r4, lr}
006601d8: mov      r4, r0
006601dc: add      r0, r0, #0x24
006601e0: bl       #0x62ecac
006601e4: ldr      r3, [r4, #0x24]
006601e8: ldr      r0, [r4, #0x28]
006601ec: rsb      r0, r3, r0
006601f0: asr      r0, r0, #3
006601f4: sub      r0, r0, #1
006601f8: pop      {r4, pc}

# _ZN24BlendedAnimSetController8PlayClipEjbij
0047680c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00476810: mov      r4, r1
00476814: ldr      r1, [sp, #0x28]
00476818: mov      sl, r2
0047681c: mov      r7, r0
00476820: bl       #0x4748b8
00476824: subs     r6, r0, #0
00476828: beq      #0x476900
0047682c: ldr      r1, [r7, #0x14]
00476830: bl       #0x36679c
00476834: cmn      r4, #1
00476838: beq      #0x476900
0047683c: ldr      r2, [r6, #0x70]
00476840: ldr      r3, [r6, #0x28]
00476844: ldr      r5, [r3, r2, lsl #2]
00476848: cmp      r5, #0
0047684c: beq      #0x476908
00476850: ldr      r3, [r5]
00476854: mov      r0, r5
00476858: mov      lr, pc
0047685c: ldr      pc, [r3, #0x44]
00476860: mov      r8, r0
00476864: mov      r0, r5
00476868: bl       #0x65f114
0047686c: mov      sb, r0
00476870: mov      r0, r6
00476874: bl       #0x369160
00476878: mov      r1, r4
0047687c: mov      fp, r0
00476880: mov      r0, r5
00476884: bl       #0x3674ac
00476888: cmn      r0, #1
0047688c: mov      r4, r0
00476890: beq      #0x476900
00476894: ldr      r3, [r8, #0x34]
00476898: cmp      r3, #0
0047689c: beq      #0x4768b4
004768a0: mov      r0, r5
004768a4: ldr      r3, [r5]
004768a8: ldr      r1, [r7, #0xc]
004768ac: mov      lr, pc
004768b0: ldr      pc, [r3, #0x30]
004768b4: cmp      sb, r4
004768b8: beq      #0x476920
004768bc: mov      r1, sl
004768c0: mov      r0, r8
004768c4: ldr      r3, [r8]
004768c8: mov      lr, pc
004768cc: ldr      pc, [r3, #0x40]
004768d0: ldr      r3, [r8]
004768d4: mov      r0, r8
004768d8: mov      r1, #0x3f800000
004768dc: mov      lr, pc
004768e0: ldr      pc, [r3, #0x48]
004768e4: ldrb     r1, [r7, #0x10]
004768e8: ldr      r0, [r7, #4]
004768ec: bl       #0x35d624
004768f0: mov      r0, r6
004768f4: bl       #0x366740
004768f8: mov      r0, #1
004768fc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476900: mov      r0, #0
00476904: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476908: mov      r0, r5
0047690c: bl       #0x65f114
00476910: mov      r0, r6
00476914: bl       #0x369160
00476918: mov      r0, r5
0047691c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00476920: ldr      r3, [r8]
00476924: mov      r0, r8
00476928: mov      lr, pc
0047692c: ldr      pc, [r3, #0x44]
00476930: cmp      r0, #0
00476934: bne      #0x4768bc
00476938: cmp      fp, #0
0047693c: ldr      r3, [r8]
00476940: ldr      r1, [r8, #0x10]
00476944: ldrne    fp, [fp, #0x10]
00476948: ldr      r3, [r3, #0xc]
0047694c: mov      r0, r8
00476950: add      r1, fp, r1
00476954: blx      r3
00476958: b        #0x4768bc

# _ZN6glitch7collada20CDynamicAnimationSet12addAnimationEPKNS0_10SAnimationE
0062f354: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062f358: ldr      r2, [r0, #0x74]
0062f35c: ldr      fp, [r0, #0x78]
0062f360: mov      r4, r0
0062f364: ldr      r0, [pc, #0x1d4]
0062f368: sub      sp, sp, #0x2c
0062f36c: rsb      fp, r2, fp
0062f370: str      r1, [sp, #0x14]
0062f374: asrs     fp, fp, #4
0062f378: add      r0, pc, r0
0062f37c: ldr      r7, [r1, #0x10]
0062f380: beq      #0x62f484
0062f384: ldr      r3, [r7, #8]
0062f388: cmp      r3, #0
0062f38c: blt      #0x62f47c
0062f390: ldr      r1, [pc, #0x1ac]
0062f394: mov      r5, #0
0062f398: mov      sl, #0xc
0062f39c: ldr      sb, [r0, r1]
0062f3a0: ldr      r1, [pc, #0x1a0]
0062f3a4: mov      r8, #1
0062f3a8: add      r1, pc, r1
0062f3ac: str      r1, [sp, #0x10]
0062f3b0: lsl      r6, r5, #4
0062f3b4: add      r2, r2, r6
0062f3b8: ldr      r1, [r2, #8]
0062f3bc: ldr      r2, [sb]
0062f3c0: cmp      r3, #0x5b
0062f3c4: mla      r2, sl, r1, r2
0062f3c8: bls      #0x62f3e4
0062f3cc: ldr      r0, [sp, #0x10]
0062f3d0: str      r2, [sp, #8]
0062f3d4: str      r3, [sp, #0xc]
0062f3d8: bl       #0x708eb0
0062f3dc: ldr      r3, [sp, #0xc]
0062f3e0: ldr      r2, [sp, #8]
0062f3e4: lsr      r1, r3, #5
0062f3e8: ldr      r2, [r2, r1, lsl #2]
0062f3ec: and      r3, r3, #0x1f
0062f3f0: ands     r2, r2, r8, lsl r3
0062f3f4: beq      #0x62f448
0062f3f8: ldr      r3, [r4, #0x74]
0062f3fc: ldr      r1, [r7, #4]
0062f400: add      r6, r3, r6
0062f404: ldr      r0, [r6, #4]
0062f408: bl       #0x30e31c
0062f40c: cmp      r0, #0
0062f410: bne      #0x62f448
0062f414: ldr      r3, [r7, #8]
0062f418: cmp      r3, #0xe
0062f41c: beq      #0x62f468
0062f420: cmp      r3, #0x56
0062f424: beq      #0x62f434
0062f428: mov      r0, r5
0062f42c: add      sp, sp, #0x2c
0062f430: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062f434: ldr      r0, [r6, #0xc]
0062f438: ldr      r1, [r7, #0xc]
0062f43c: bl       #0x30e31c
0062f440: cmp      r0, #0
0062f444: beq      #0x62f428
0062f448: add      r5, r5, #1
0062f44c: cmp      r5, fp
0062f450: beq      #0x62f484
0062f454: ldr      r3, [r7, #8]
0062f458: cmp      r3, #0
0062f45c: blt      #0x62f47c
0062f460: ldr      r2, [r4, #0x74]
0062f464: b        #0x62f3b0
0062f468: ldrb     r2, [r6, #0xc]
0062f46c: ldrb     r3, [r7, #0xc]
0062f470: cmp      r2, r3
0062f474: bne      #0x62f448
0062f478: b        #0x62f428
0062f47c: mvn      r0, #0
0062f480: b        #0x62f42c
0062f484: ldr      r0, [sp, #0x14]
0062f488: bl       #0x611ae0
0062f48c: cmp      r0, #0
0062f490: beq      #0x62f47c
0062f494: ldr      ip, [r4, #0x78]
0062f498: ldr      r3, [r4, #0x7c]
0062f49c: ldr      r1, [sp, #0x14]
0062f4a0: str      r0, [sp, #0x1c]
0062f4a4: cmp      ip, r3
0062f4a8: ldr      r2, [r1, #0x10]
0062f4ac: beq      #0x62f500
0062f4b0: ldm      r2, {r0, r1, r2, r3}
0062f4b4: stm      ip, {r0, r1, r2, r3}
0062f4b8: ldr      r3, [r4, #0x78]
0062f4bc: add      r3, r3, #0x10
0062f4c0: str      r3, [r4, #0x78]
0062f4c4: ldr      r1, [r4, #0x1c]
0062f4c8: ldr      r3, [r4, #0x20]
0062f4cc: cmp      r1, r3
0062f4d0: beq      #0x62f520
0062f4d4: ldr      r3, [sp, #0x1c]
0062f4d8: str      r3, [r1]
0062f4dc: ldr      r3, [r4, #0x1c]
0062f4e0: add      r3, r3, #4
0062f4e4: str      r3, [r4, #0x1c]
0062f4e8: ldr      r3, [r4, #0x74]
0062f4ec: ldr      r0, [r4, #0x78]
0062f4f0: rsb      r0, r3, r0
0062f4f4: asr      r0, r0, #4
0062f4f8: sub      r0, r0, #1
0062f4fc: b        #0x62f42c
0062f500: mov      r1, ip
0062f504: add      r0, r4, #0x74
0062f508: mov      ip, #1
0062f50c: add      r3, sp, #0x24
0062f510: str      ip, [sp, #4]
0062f514: str      ip, [sp]
0062f518: bl       #0x62e710
0062f51c: b        #0x62f4c4
0062f520: mov      ip, #1
0062f524: add      r0, r4, #0x18
0062f528: add      r2, sp, #0x1c
0062f52c: add      r3, sp, #0x20
0062f530: str      ip, [sp, #4]
0062f534: str      ip, [sp]
0062f538: bl       #0x62f240
0062f53c: b        #0x62f4e8
0062f540: eorseq   r5, r6, r8, lsl r7
0062f544: andeq    r4, r0, ip, asr #10
0062f548: eoreq    r2, sb, r0, lsr #18

# _ZN6glitch7collada20CDynamicAnimationSet7compileEv
0062f61c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062f620: ldrb     r3, [r0, #0x70]
0062f624: sub      sp, sp, #0x1c
0062f628: mov      r4, r0
0062f62c: cmp      r3, #0
0062f630: bne      #0x62f63c
0062f634: add      sp, sp, #0x1c
0062f638: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062f63c: bl       #0x62f54c
0062f640: ldr      r3, [r4, #0x24]
0062f644: ldr      r2, [r4, #0x28]
0062f648: rsb      r1, r3, r2
0062f64c: lsrs     r1, r1, #3
0062f650: movne    r7, #0
0062f654: beq      #0x62f980
0062f658: ldr      r1, [r3, r7, lsl #3]
0062f65c: add      r6, r3, r7, lsl #3
0062f660: ldr      r1, [r1, #0x24]
0062f664: ldr      r1, [r1, #0x20]
0062f668: ldr      r1, [r1, #0x24]
0062f66c: cmp      r1, #0
0062f670: ble      #0x62f6bc
0062f674: mov      r5, #0
0062f678: mov      r1, r5
0062f67c: mov      r0, r6
0062f680: bl       #0x60e35c
0062f684: ldr      r3, [r4]
0062f688: mov      r1, r0
0062f68c: mov      r0, r4
0062f690: mov      lr, pc
0062f694: ldr      pc, [r3, #0xc]
0062f698: ldr      r3, [r6]
0062f69c: add      r5, r5, #1
0062f6a0: ldr      r3, [r3, #0x24]
0062f6a4: ldr      r3, [r3, #0x20]
0062f6a8: ldr      r3, [r3, #0x24]
0062f6ac: cmp      r5, r3
0062f6b0: blt      #0x62f678
0062f6b4: ldr      r3, [r4, #0x24]
0062f6b8: ldr      r2, [r4, #0x28]
0062f6bc: rsb      r1, r3, r2
0062f6c0: add      r7, r7, #1
0062f6c4: asr      r1, r1, #3
0062f6c8: cmp      r7, r1
0062f6cc: blo      #0x62f658
0062f6d0: cmp      r1, #0
0062f6d4: beq      #0x62f980
0062f6d8: ldr      r0, [r4, #0x78]
0062f6dc: ldr      r1, [r4, #0x74]
0062f6e0: mov      r7, #0
0062f6e4: add      sl, sp, #0x14
0062f6e8: rsb      r0, r1, r0
0062f6ec: asrs     ip, r0, #4
0062f6f0: add      r6, r3, r7, lsl #3
0062f6f4: beq      #0x62f738
0062f6f8: mov      r5, #0
0062f6fc: lsl      r8, r5, #4
0062f700: add      r1, r1, r8
0062f704: mov      r0, r6
0062f708: bl       #0x61c1e0
0062f70c: cmp      r0, #0
0062f710: beq      #0x62f8cc
0062f714: ldr      r0, [r4, #0x78]
0062f718: ldr      r1, [r4, #0x74]
0062f71c: add      r5, r5, #1
0062f720: rsb      r0, r1, r0
0062f724: asr      ip, r0, #4
0062f728: cmp      r5, ip
0062f72c: blo      #0x62f6fc
0062f730: ldr      r3, [r4, #0x24]
0062f734: ldr      r2, [r4, #0x28]
0062f738: add      r7, r7, #1
0062f73c: rsb      lr, r3, r2
0062f740: cmp      r7, lr, asr #3
0062f744: blo      #0x62f6ec
0062f748: rsb      r3, r3, r2
0062f74c: asr      r5, r3, #3
0062f750: mul      r5, r5, ip
0062f754: add      r6, r4, #0x30
0062f758: str      ip, [r4, #0x3c]
0062f75c: mov      r0, r6
0062f760: mov      r1, r5
0062f764: bl       #0x62e5e0
0062f768: mov      sl, #0
0062f76c: mov      r1, r5
0062f770: add      r2, sp, #8
0062f774: mov      r0, r6
0062f778: str      sl, [sp, #8]
0062f77c: str      sl, [sp, #0xc]
0062f780: str      sl, [sp, #0x10]
0062f784: bl       #0x62ec50
0062f788: ldr      r3, [r4, #0x24]
0062f78c: ldr      r2, [r4, #0x28]
0062f790: rsb      r1, r3, r2
0062f794: lsrs     r1, r1, #3
0062f798: beq      #0x62f8b8
0062f79c: add      r0, r4, #0x68
0062f7a0: ldr      r1, [r4, #0x3c]
0062f7a4: mov      fp, #2
0062f7a8: stm      sp, {r0, sl}
0062f7ac: ldr      ip, [sp, #4]
0062f7b0: cmp      r1, #0
0062f7b4: add      sb, r3, ip, lsl #3
0062f7b8: beq      #0x62f89c
0062f7bc: mov      r0, #0xc
0062f7c0: mul      r5, r0, sl
0062f7c4: mov      r6, #0
0062f7c8: b        #0x62f800
0062f7cc: ldr      r2, [r4, #0x30]
0062f7d0: ldr      r1, [r4, #0x74]
0062f7d4: add      r2, r2, r5
0062f7d8: add      r1, r1, r7
0062f7dc: add      r2, r2, #4
0062f7e0: bl       #0x61c6bc
0062f7e4: cmp      r0, #0
0062f7e8: beq      #0x62f870
0062f7ec: ldr      r1, [r4, #0x3c]
0062f7f0: add      sl, sl, #1
0062f7f4: add      r5, r5, #0xc
0062f7f8: cmp      r1, r6
0062f7fc: bls      #0x62f894
0062f800: ldr      r1, [r4, #0x74]
0062f804: lsl      r7, r6, #4
0062f808: mov      r0, sb
0062f80c: add      r1, r1, r7
0062f810: bl       #0x61c1e0
0062f814: ldr      r2, [r4, #0x30]
0062f818: ldr      r1, [r4, #0x74]
0062f81c: mov      r8, r0
0062f820: add      r2, r2, r5
0062f824: add      r2, r2, #4
0062f828: mov      r0, sb
0062f82c: add      r1, r1, r7
0062f830: bl       #0x61c6bc
0062f834: ldr      r3, [r4, #0x30]
0062f838: cmp      r8, #0
0062f83c: moveq    r2, #1
0062f840: streq    r2, [r3, r5]
0062f844: strne    fp, [r3, r5]
0062f848: ldr      r3, [r4, #0x30]
0062f84c: cmp      r0, #0
0062f850: add      r6, r6, #1
0062f854: add      r3, r3, r5
0062f858: str      r8, [r3, #8]
0062f85c: bne      #0x62f7ec
0062f860: ldr      r3, [r4, #0x68]
0062f864: ldr      r0, [sp]
0062f868: cmp      r3, #0
0062f86c: bne      #0x62f7cc
0062f870: ldr      r3, [r4, #0x30]
0062f874: mov      ip, #0
0062f878: add      sl, sl, #1
0062f87c: add      r3, r3, r5
0062f880: str      ip, [r3, #4]
0062f884: ldr      r1, [r4, #0x3c]
0062f888: add      r5, r5, #0xc
0062f88c: cmp      r1, r6
0062f890: bhi      #0x62f800
0062f894: ldr      r3, [r4, #0x24]
0062f898: ldr      r2, [r4, #0x28]
0062f89c: ldr      r0, [sp, #4]
0062f8a0: add      r0, r0, #1
0062f8a4: str      r0, [sp, #4]
0062f8a8: ldr      ip, [sp, #4]
0062f8ac: rsb      r0, r3, r2
0062f8b0: cmp      ip, r0, asr #3
0062f8b4: blo      #0x62f7ac
0062f8b8: mov      r0, r4
0062f8bc: bl       #0x6605ac
0062f8c0: mov      r3, #0
0062f8c4: strb     r3, [r4, #0x70]
0062f8c8: b        #0x62f634
0062f8cc: ldr      r1, [r4, #0x74]
0062f8d0: mov      r0, r6
0062f8d4: mov      r2, sl
0062f8d8: add      r1, r1, r8
0062f8dc: bl       #0x61c6bc
0062f8e0: cmp      r0, #0
0062f8e4: bne      #0x62f714
0062f8e8: ldr      r3, [r4, #8]
0062f8ec: cmp      r3, #0
0062f8f0: bne      #0x62f714
0062f8f4: ldr      lr, [r4, #0x74]
0062f8f8: ldr      r3, [r4, #0x78]
0062f8fc: add      lr, lr, r8
0062f900: add      ip, lr, #0x10
0062f904: cmp      ip, r3
0062f908: beq      #0x62f93c
0062f90c: rsb      r8, ip, r3
0062f910: asr      r8, r8, #4
0062f914: cmp      r8, #0
0062f918: bgt      #0x62f924
0062f91c: b        #0x62f93c
0062f920: add      ip, ip, #0x10
0062f924: subs     r8, r8, #1
0062f928: ldm      ip, {r0, r1, r2, r3}
0062f92c: stm      lr, {r0, r1, r2, r3}
0062f930: mov      lr, ip
0062f934: bne      #0x62f920
0062f938: ldr      r3, [r4, #0x78]
0062f93c: ldr      r0, [r4, #0x18]
0062f940: ldr      ip, [r4, #0x1c]
0062f944: sub      r3, r3, #0x10
0062f948: add      r0, r0, r5, lsl #2
0062f94c: add      r1, r0, #4
0062f950: cmp      r1, ip
0062f954: str      r3, [r4, #0x78]
0062f958: beq      #0x62f970
0062f95c: subs     r2, ip, r1
0062f960: moveq    r1, ip
0062f964: beq      #0x62f970
0062f968: bl       #0x30df38
0062f96c: ldr      r1, [r4, #0x1c]
0062f970: sub      r1, r1, #4
0062f974: str      r1, [r4, #0x1c]
0062f978: sub      r5, r5, #1
0062f97c: b        #0x62f714
0062f980: ldr      r1, [r4, #0x74]
0062f984: ldr      ip, [r4, #0x78]
0062f988: rsb      ip, r1, ip
0062f98c: asr      ip, ip, #4
0062f990: b        #0x62f748

# _ZN6glitch7collada13CAnimationSet19setMismatchBehaviorENS1_19E_MISMATCH_BEHAVIORE
0062db98: str      r1, [r0, #8]
0062db9c: bx       lr

# _ZN6glitch7collada20CDynamicAnimationSet19addAnimationLibraryERKNS0_16CColladaDatabaseE
0062e8a8: push     {r4, lr}
0062e8ac: mov      r4, r0
0062e8b0: bl       #0x6601d4
0062e8b4: mov      r3, #1
0062e8b8: strb     r3, [r4, #0x70]
0062e8bc: pop      {r4, pc}

# _ZN6glitch7collada35CAnimationSetTransformationTemplate16isAnimationExistEPKNS0_8SChannelE
006e22cc: push     {r4, r5, r6, r7, r8, lr}
006e22d0: ldr      r3, [r0, #4]
006e22d4: ldr      r2, [r0, #8]
006e22d8: mov      r5, r0
006e22dc: mov      r6, r1
006e22e0: rsb      r2, r3, r2
006e22e4: lsrs     r2, r2, #2
006e22e8: beq      #0x6e2394
006e22ec: mov      r4, #0
006e22f0: mov      r8, #1
006e22f4: b        #0x6e234c
006e22f8: ldr      r3, [r6, #8]
006e22fc: cmp      r3, #0xd
006e2300: bhi      #0x6e237c
006e2304: lsl      r3, r8, r3
006e2308: tst      r3, #0x3c00
006e230c: mov      r0, #1
006e2310: bne      #0x6e23b8
006e2314: tst      r3, #0x3e0
006e2318: bne      #0x6e239c
006e231c: tst      r3, #0x1e
006e2320: beq      #0x6e237c
006e2324: ldr      r3, [r5, #4]
006e2328: ldr      r2, [r3, r7]
006e232c: ldr      r0, [r2, #4]
006e2330: cmp      r0, #1
006e2334: beq      #0x6e23b0
006e2338: ldr      r2, [r5, #8]
006e233c: add      r4, r4, #1
006e2340: rsb      r2, r3, r2
006e2344: cmp      r4, r2, asr #2
006e2348: bhs      #0x6e2394
006e234c: ldr      r3, [r3, r4, lsl #2]
006e2350: lsl      r7, r4, #2
006e2354: ldr      r3, [r3, #8]
006e2358: mov      r0, r3
006e235c: ldr      r3, [r3]
006e2360: mov      lr, pc
006e2364: ldr      pc, [r3, #0x54]
006e2368: mov      r1, r0
006e236c: ldr      r0, [r6, #4]
006e2370: bl       #0x30e31c
006e2374: cmp      r0, #0
006e2378: beq      #0x6e22f8
006e237c: ldr      r3, [r5, #4]
006e2380: ldr      r2, [r5, #8]
006e2384: add      r4, r4, #1
006e2388: rsb      r2, r3, r2
006e238c: cmp      r4, r2, asr #2
006e2390: blo      #0x6e234c
006e2394: mov      r0, #0
006e2398: pop      {r4, r5, r6, r7, r8, pc}
006e239c: ldr      r3, [r5, #4]
006e23a0: ldr      r2, [r3, r7]
006e23a4: ldr      r1, [r2, #4]
006e23a8: cmp      r1, #5
006e23ac: bne      #0x6e2338
006e23b0: strb     r0, [r2]
006e23b4: pop      {r4, r5, r6, r7, r8, pc}
006e23b8: ldr      r3, [r5, #4]
006e23bc: ldr      r2, [r3, r7]
006e23c0: ldr      r1, [r2, #4]
006e23c4: cmp      r1, #0xa
006e23c8: bne      #0x6e2338
006e23cc: b        #0x6e23b0

# _ZN6glitch7collada21CSceneNodeAnimatorSetC1ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
00660c20: push     {r4, r5, r6, lr}
00660c24: ldr      r5, [pc, #0xa8]
00660c28: ldr      r2, [pc, #0xa8]
00660c2c: ldr      r3, [pc, #0xa8]
00660c30: add      r5, pc, r5
00660c34: ldr      r2, [r5, r2]
00660c38: ldr      r3, [r5, r3]
00660c3c: mov      ip, #1
00660c40: add      r2, r2, #8
00660c44: mov      r6, r1
00660c48: str      ip, [r0, #0x5c]
00660c4c: add      r1, r3, #4
00660c50: str      r2, [r0, #0x58]
00660c54: mov      r4, r0
00660c58: bl       #0x6698fc
00660c5c: ldr      r3, [pc, #0x7c]
00660c60: mov      r0, r4
00660c64: ldr      r3, [r5, r3]
00660c68: add      r2, r3, #0xa4
00660c6c: add      r1, r3, #0xc
00660c70: add      r3, r3, #0xc0
00660c74: stm      r4, {r1, r2}
00660c78: str      r3, [r4, #0x58]
00660c7c: ldr      r3, [r6]
00660c80: mov      r1, r6
00660c84: cmp      r3, #0
00660c88: str      r3, [r4, #0x24]
00660c8c: ldrne    r2, [r3, #4]
00660c90: addne    r2, r2, #1
00660c94: strne    r2, [r3, #4]
00660c98: mov      r3, #0
00660c9c: str      r3, [r4, #0x54]
00660ca0: str      r3, [r4, #0x28]
00660ca4: str      r3, [r4, #0x2c]
00660ca8: str      r3, [r4, #0x30]
00660cac: str      r3, [r4, #0x34]
00660cb0: str      r3, [r4, #0x38]
00660cb4: str      r3, [r4, #0x3c]
00660cb8: str      r3, [r4, #0x40]
00660cbc: str      r3, [r4, #0x44]
00660cc0: str      r3, [r4, #0x48]
00660cc4: str      r3, [r4, #0x4c]
00660cc8: bl       #0x660af4
00660ccc: mov      r0, r4
00660cd0: pop      {r4, r5, r6, pc}
00660cd4: eorseq   r3, r3, r0, ror #28
00660cd8: andeq    r2, r0, r4, asr #22
00660cdc: andeq    r1, r0, r4, asr #10
00660ce0: andeq    r2, r0, r0, lsl #12

# _ZN6glitch7collada21CSceneNodeAnimatorSetC2ERKN5boost13intrusive_ptrINS0_13CAnimationSetEEE
00660ce4: push     {r4, r5, r6, r7, r8, lr}
00660ce8: mov      r5, r1
00660cec: ldr      r4, [pc, #0x90]
00660cf0: add      r1, r1, #4
00660cf4: mov      r6, r0
00660cf8: mov      r7, r2
00660cfc: bl       #0x6698fc
00660d00: ldr      r2, [r5]
00660d04: ldr      r3, [pc, #0x7c]
00660d08: add      r4, pc, r4
00660d0c: str      r2, [r6]
00660d10: ldr      r3, [r4, r3]
00660d14: ldr      r2, [r2, #-0xc]
00660d18: ldr      r1, [r5, #0x1c]
00660d1c: add      r3, r3, #0xa4
00660d20: mov      r0, r6
00660d24: str      r1, [r6, r2]
00660d28: str      r3, [r6, #4]
00660d2c: ldr      r3, [r7]
00660d30: mov      r1, r7
00660d34: cmp      r3, #0
00660d38: str      r3, [r6, #0x24]
00660d3c: ldrne    r2, [r3, #4]
00660d40: addne    r2, r2, #1
00660d44: strne    r2, [r3, #4]
00660d48: mov      r3, #0
00660d4c: str      r3, [r6, #0x54]
00660d50: str      r3, [r6, #0x28]
00660d54: str      r3, [r6, #0x2c]
00660d58: str      r3, [r6, #0x30]
00660d5c: str      r3, [r6, #0x34]
00660d60: str      r3, [r6, #0x38]
00660d64: str      r3, [r6, #0x3c]
00660d68: str      r3, [r6, #0x40]
00660d6c: str      r3, [r6, #0x44]
00660d70: str      r3, [r6, #0x48]
00660d74: str      r3, [r6, #0x4c]
00660d78: bl       #0x660af4
00660d7c: mov      r0, r6
00660d80: pop      {r4, r5, r6, r7, r8, pc}
00660d84: eorseq   r3, r3, r8, lsl #27
00660d88: andeq    r2, r0, r0, lsl #12

# _ZN14AnimApplicatorC1EPN6glitch7collada18ISceneNodeAnimatorE
00364398: push     {r4, r5}
0036439c: ldr      r4, [pc, #0x54]
003643a0: ldr      r5, [pc, #0x54]
003643a4: mov      ip, #0
003643a8: add      r4, pc, r4
003643ac: ldr      r5, [r4, r5]
003643b0: mov      r2, #0
003643b4: str      r2, [r0, #0x38]
003643b8: add      r5, r5, #8
003643bc: str      r5, [r0]
003643c0: str      r1, [r0, #4]
003643c4: str      ip, [r0, #0x2c]
003643c8: str      r2, [r0, #8]
003643cc: str      r2, [r0, #0x10]
003643d0: str      r2, [r0, #0x14]
003643d4: str      ip, [r0, #0x18]
003643d8: str      ip, [r0, #0x1c]
003643dc: str      ip, [r0, #0x20]
003643e0: str      ip, [r0, #0x24]
003643e4: str      ip, [r0, #0x28]
003643e8: strb     r2, [r0, #0x30]
003643ec: str      r2, [r0, #0x34]
003643f0: pop      {r4, r5}
003643f4: bx       lr
003643f8: rsbeq    r0, r3, r8, ror #13
003643fc: andeq    r2, r0, r4, lsl #22
