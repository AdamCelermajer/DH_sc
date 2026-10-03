
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

# _ZNK6glitch7collada18ISceneNodeAnimator19serializeAttributesEPNS_2io11IAttributesEPNS2_26SAttributeReadWriteOptionsE
00667cb4: bx       lr

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
