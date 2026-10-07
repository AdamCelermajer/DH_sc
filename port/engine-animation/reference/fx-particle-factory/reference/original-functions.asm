
# _ZN6glitch2ps9PSManager18createPCloudSystemEb
00655314: push     {r4, r5, r6, lr}
00655318: subs     r5, r1, #0
0065531c: bne      #0x655348
00655320: mov      r0, #0x1dc
00655324: bl       #0x5341ac
00655328: mov      r1, r5
0065532c: mov      r4, r0
00655330: mov      r2, #0x180
00655334: bl       #0x30e460
00655338: mov      r0, r4
0065533c: bl       #0x654ec4
00655340: mov      r0, r4
00655344: pop      {r4, r5, r6, pc}
00655348: mov      r1, #0
0065534c: mov      r0, #0x1dc
00655350: bl       #0x5341ac
00655354: mov      r1, #0
00655358: mov      r4, r0
0065535c: mov      r2, #0x180
00655360: bl       #0x30e460
00655364: mov      r0, r4
00655368: bl       #0x654a74
0065536c: mov      r0, r4
00655370: pop      {r4, r5, r6, pc}

# _ZN6glitch2ps16IParticleContextINS0_9SParticleEE10hashStringEPKc
0064d0bc: push     {r4, r5, r6, r7, lr}
0064d0c0: ldr      r4, [pc, #0xf0]
0064d0c4: ldr      r3, [pc, #0xf0]
0064d0c8: ldr      r7, [pc, #0xf0]
0064d0cc: add      r4, pc, r4
0064d0d0: ldr      r5, [r4, r3]
0064d0d4: ldr      r3, [r4, r7]
0064d0d8: sub      sp, sp, #0x24
0064d0dc: ldr      r2, [r5]
0064d0e0: ldr      r3, [r3]
0064d0e4: mov      r6, r1
0064d0e8: tst      r2, #1
0064d0ec: str      r3, [sp, #0x1c]
0064d0f0: beq      #0x64d17c
0064d0f4: add      r5, sp, #4
0064d0f8: mov      r0, r6
0064d0fc: str      r5, [sp, #0x14]
0064d100: str      r5, [sp, #0x18]
0064d104: bl       #0x30de54
0064d108: mov      r1, r6
0064d10c: add      r2, r6, r0
0064d110: mov      r0, r5
0064d114: bl       #0x3116e8
0064d118: ldr      r2, [sp, #0x18]
0064d11c: ldr      r0, [sp, #0x14]
0064d120: cmp      r2, r0
0064d124: moveq    r6, #0
0064d128: beq      #0x64d154
0064d12c: mov      r6, #0
0064d130: ldrsb    r1, [r2], #1
0064d134: movw     r3, #0x79b9
0064d138: movt     r3, #0x9e37
0064d13c: add      r3, r1, r3
0064d140: add      r3, r3, r6, lsl #6
0064d144: add      r3, r3, r6, lsr #2
0064d148: cmp      r2, r0
0064d14c: eor      r6, r6, r3
0064d150: bne      #0x64d130
0064d154: mov      r0, r5
0064d158: bl       #0x3139ac
0064d15c: ldr      r3, [r4, r7]
0064d160: ldr      r2, [sp, #0x1c]
0064d164: mov      r0, r6
0064d168: ldr      r3, [r3]
0064d16c: cmp      r2, r3
0064d170: bne      #0x64d1b4
0064d174: add      sp, sp, #0x24
0064d178: pop      {r4, r5, r6, r7, pc}
0064d17c: mov      r0, r5
0064d180: bl       #0x30e76c
0064d184: cmp      r0, #0
0064d188: beq      #0x64d0f4
0064d18c: mov      r0, r5
0064d190: bl       #0x30ea3c
0064d194: ldr      r3, [pc, #0x28]
0064d198: ldr      r0, [r4, r3]
0064d19c: ldr      r3, [pc, #0x24]
0064d1a0: ldr      r1, [r4, r3]
0064d1a4: ldr      r3, [pc, #0x20]
0064d1a8: ldr      r2, [r4, r3]
0064d1ac: bl       #0x30e304
0064d1b0: b        #0x64d0f4
0064d1b4: bl       #0x30e310
0064d1b8: eorseq   r7, r4, r4, asr #19
0064d1bc: andeq    r0, r0, r0, lsr #21
0064d1c0: andeq    r4, r0, ip, lsr #1
0064d1c4: strheq   r2, [r0], -r4
0064d1c8: andeq    r2, r0, r4, lsr #14
0064d1cc: muleq    r0, r0, r8

# _ZN6glitch2ps16IParticleContextINS0_9SParticleEEC2Ev
0064d1d0: ldr      ip, [pc, #0xac]
0064d1d4: ldr      r2, [pc, #0xac]
0064d1d8: push     {r4, r5, lr}
0064d1dc: add      ip, pc, ip
0064d1e0: ldr      r2, [ip, r2]
0064d1e4: mov      r3, #0
0064d1e8: add      r1, r0, #0x14
0064d1ec: add      lr, r2, #8
0064d1f0: str      lr, [r0]
0064d1f4: str      r3, [r0, #8]
0064d1f8: str      r3, [r0, #0xc]
0064d1fc: str      r3, [r0, #0x10]
0064d200: str      r3, [r0, #0x14]
0064d204: str      r3, [r1, #8]
0064d208: str      r3, [r1, #4]
0064d20c: ldr      r1, [pc, #0x78]
0064d210: mov      r2, #0
0064d214: mov      r5, r0
0064d218: strb     r2, [r0, #0x21]
0064d21c: str      r2, [r0, #0x24]
0064d220: str      r2, [r0, #0x28]
0064d224: str      r2, [r0, #0x2c]
0064d228: str      r2, [r0, #0x34]
0064d22c: strb     r2, [r5, #0x30]!
0064d230: sub      sp, sp, #0x14
0064d234: str      r3, [r0, #0x50]
0064d238: strb     r2, [r0, #0x54]
0064d23c: add      r1, pc, r1
0064d240: str      r5, [r0, #0x38]
0064d244: str      r5, [r0, #0x3c]
0064d248: str      r2, [r0, #0x40]
0064d24c: str      r3, [r0, #0x48]
0064d250: str      r3, [r0, #0x4c]
0064d254: mov      r4, r0
0064d258: bl       #0x64d0bc
0064d25c: add      r3, r4, #0x58
0064d260: str      r0, [sp]
0064d264: mov      r1, r5
0064d268: add      r0, sp, #8
0064d26c: mov      r2, sp
0064d270: str      r3, [sp, #4]
0064d274: bl       #0x63a8ec
0064d278: mov      r0, r4
0064d27c: add      sp, sp, #0x14
0064d280: pop      {r4, r5, pc}
0064d284: ldrhteq  r7, [r4], -r4
0064d288: andeq    r2, r0, r8, lsr #4
0064d28c: mlaeq    sb, r4, lr, r7

# _ZNK6glitch7collada16CColladaDatabase16constructEmitterEPNS0_16SInstanceEmitterEPNS_5video12IVideoDriverEPNS0_14CRootSceneNodeE
0061a888: push     {r4, r5, r6, r7, r8, sl, lr}
0061a88c: mov      r4, r1
0061a890: ldr      r1, [r1, #4]
0061a894: sub      sp, sp, #0xc
0061a898: str      r3, [sp]
0061a89c: add      r1, r1, #1
0061a8a0: add      r3, r4, #0x18
0061a8a4: mov      r5, r0
0061a8a8: mov      r6, r2
0061a8ac: bl       #0x61a854
0061a8b0: subs     sl, r0, #0
0061a8b4: beq      #0x61a904
0061a8b8: ldr      r3, [r4, #0xc]
0061a8bc: cmp      r3, #0
0061a8c0: ble      #0x61a904
0061a8c4: mov      r7, #0
0061a8c8: mov      r8, r7
0061a8cc: ldr      r3, [r4, #0x10]
0061a8d0: mov      r0, r5
0061a8d4: add      r8, r8, #1
0061a8d8: add      r3, r3, r7
0061a8dc: ldr      r1, [r3, #8]
0061a8e0: bl       #0x60e400
0061a8e4: mov      r2, r6
0061a8e8: mov      r1, r0
0061a8ec: mov      r0, sl
0061a8f0: bl       #0x619820
0061a8f4: ldr      r3, [r4, #0xc]
0061a8f8: add      r7, r7, #0x3c
0061a8fc: cmp      r8, r3
0061a900: blt      #0x61a8cc
0061a904: mov      r0, sl
0061a908: add      sp, sp, #0xc
0061a90c: pop      {r4, r5, r6, r7, r8, sl, pc}

# _ZNK6glitch7collada16CColladaDatabase16constructEmitterEPNS0_8SEmitterEPNS_5video12IVideoDriverEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
0060e79c: push     {r4, lr}
0060e7a0: subs     ip, r1, #0
0060e7a4: sub      sp, sp, #8
0060e7a8: mov      r4, r3
0060e7ac: moveq    r0, ip
0060e7b0: beq      #0x60e7d8
0060e7b4: ldr      lr, [r0, #4]
0060e7b8: mov      r1, r0
0060e7bc: mov      r3, ip
0060e7c0: mov      r0, lr
0060e7c4: ldr      ip, [lr]
0060e7c8: ldr      lr, [sp, #0x10]
0060e7cc: stm      sp, {r4, lr}
0060e7d0: mov      lr, pc
0060e7d4: ldr      pc, [ip, #0x68]
0060e7d8: add      sp, sp, #8
0060e7dc: pop      {r4, pc}

# _ZN14ColladaFactory20createParticleSystemERKN6glitch7collada16CColladaDatabaseEPNS0_5video12IVideoDriverEPNS1_8SEmitterEPNS0_3res6vectorINSA_6StringEEEPNS1_14CRootSceneNodeE
003507b8: b        #0x634c64

# _ZNK6glitch7collada16CColladaDatabase10getEmitterEi
0060e468: ldr      r3, [r0]
0060e46c: mov      r0, #0x90
0060e470: ldr      r3, [r3, #0x24]
0060e474: ldr      r3, [r3, #0x20]
0060e478: ldr      r3, [r3, #0x7c]
0060e47c: mla      r0, r0, r1, r3
0060e480: bx       lr

# _ZN6glitch7collada24CParticleSystemSceneNode6attachEPNS_5scene10ISceneNodeE
0064f6f8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0064f6fc: ldr      r3, [r0, #0x150]
0064f700: add      r8, r0, #0x164
0064f704: sub      sp, sp, #8
0064f708: ldr      r5, [r3]
0064f70c: mov      r4, r0
0064f710: mov      r6, r1
0064f714: mov      r0, r8
0064f718: mov      r1, r5
0064f71c: bl       #0x63a714
0064f720: mov      r7, #0
0064f724: add      r2, sp, #8
0064f728: str      r7, [r2, #-4]!
0064f72c: mov      r0, r8
0064f730: mov      r1, r5
0064f734: bl       #0x63db08
0064f738: cmp      r5, r7
0064f73c: ble      #0x64f7e0
0064f740: movw     sb, #0x6164
0064f744: movt     sb, #0x6665
0064f748: ldr      r3, [r4, #0x150]
0064f74c: mov      r0, r6
0064f750: ldr      r3, [r3, #4]
0064f754: ldr      r1, [r3, r7, lsl #2]
0064f758: add      r1, r1, #1
0064f75c: bl       #0x5985e4
0064f760: subs     sl, r0, #0
0064f764: beq      #0x64f7d4
0064f768: ldr      r8, [sl, #0xf4]!
0064f76c: cmp      r8, sl
0064f770: bne      #0x64f784
0064f774: b        #0x64f7d4
0064f778: ldr      r8, [r8]
0064f77c: cmp      sl, r8
0064f780: beq      #0x64f7d4
0064f784: cmp      r8, #0
0064f788: moveq    r3, r8
0064f78c: subne    r3, r8, #4
0064f790: mov      r0, r3
0064f794: ldr      r3, [r3]
0064f798: mov      lr, pc
0064f79c: ldr      pc, [r3, #0xbc]
0064f7a0: cmp      r0, sb
0064f7a4: bne      #0x64f778
0064f7a8: cmp      r8, #0
0064f7ac: moveq    r3, r8
0064f7b0: subne    r3, r8, #4
0064f7b4: mov      r0, r3
0064f7b8: mov      r1, r4
0064f7bc: ldr      r3, [r3]
0064f7c0: mov      lr, pc
0064f7c4: ldr      pc, [r3, #0xf4]
0064f7c8: ldr      r8, [r8]
0064f7cc: cmp      sl, r8
0064f7d0: bne      #0x64f784
0064f7d4: add      r7, r7, #1
0064f7d8: cmp      r7, r5
0064f7dc: bne      #0x64f748
0064f7e0: add      sp, sp, #8
0064f7e4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch2ps12PMotionModelINS0_9SParticleEEC2Ev
006540ec: push     {r4, r5, lr}
006540f0: ldr      r3, [r1]
006540f4: mov      r2, #0
006540f8: sub      sp, sp, #0x44
006540fc: str      r3, [r0]
00654100: ldr      r3, [r3, #-0xc]
00654104: ldr      r1, [r1, #4]
00654108: mov      r4, r0
0065410c: str      r1, [r0, r3]
00654110: ldr      r3, [r0]
00654114: str      r2, [r0, #0xc]
00654118: str      r2, [r0, #4]
0065411c: str      r2, [r0, #8]
00654120: ldr      r5, [r3, #-0xc]
00654124: ldr      r1, [pc, #0xdc]
00654128: add      r5, r0, r5
0065412c: add      r1, pc, r1
00654130: mov      r0, r5
00654134: bl       #0x64d0bc
00654138: add      r2, sp, #0x30
0065413c: add      r3, r4, #4
00654140: str      r0, [sp, #0x30]
00654144: add      r1, r5, #0x30
00654148: add      r0, sp, #0x38
0065414c: str      r3, [sp, #0x34]
00654150: bl       #0x63a8ec
00654154: ldr      r3, [r4]
00654158: ldr      r1, [pc, #0xac]
0065415c: ldr      r5, [r3, #-0xc]
00654160: add      r1, pc, r1
00654164: add      r5, r4, r5
00654168: mov      r0, r5
0065416c: bl       #0x64d0bc
00654170: add      r2, sp, #0x20
00654174: add      r3, r4, #0x10
00654178: str      r0, [sp, #0x20]
0065417c: add      r1, r5, #0x30
00654180: add      r0, sp, #0x28
00654184: str      r3, [sp, #0x24]
00654188: bl       #0x63a8ec
0065418c: ldr      r3, [r4]
00654190: ldr      r1, [pc, #0x78]
00654194: ldr      r5, [r3, #-0xc]
00654198: add      r1, pc, r1
0065419c: add      r5, r4, r5
006541a0: mov      r0, r5
006541a4: bl       #0x64d0bc
006541a8: add      r2, sp, #0x10
006541ac: add      r3, r4, #0x14
006541b0: str      r0, [sp, #0x10]
006541b4: add      r1, r5, #0x30
006541b8: add      r0, sp, #0x18
006541bc: str      r3, [sp, #0x14]
006541c0: bl       #0x63a8ec
006541c4: ldr      r3, [r4]
006541c8: ldr      r1, [pc, #0x44]
006541cc: ldr      r5, [r3, #-0xc]
006541d0: add      r1, pc, r1
006541d4: add      r5, r4, r5
006541d8: mov      r0, r5
006541dc: bl       #0x64d0bc
006541e0: add      r3, r4, #0x18
006541e4: str      r0, [sp]
006541e8: add      r1, r5, #0x30
006541ec: add      r0, sp, #8
006541f0: mov      r2, sp
006541f4: str      r3, [sp, #4]
006541f8: bl       #0x63a8ec
006541fc: mov      r0, r4
00654200: add      sp, sp, #0x44
00654204: pop      {r4, r5, pc}

# _ZN6glitch2ps10PSpinModelINS0_9SParticleEEC2Ev
00654218: push     {r4, r5, lr}
0065421c: ldr      r3, [r1]
00654220: mov      r2, #0
00654224: sub      sp, sp, #0x74
00654228: str      r3, [r0]
0065422c: ldr      r3, [r3, #-0xc]
00654230: ldr      r1, [r1, #4]
00654234: mov      r4, r0
00654238: str      r1, [r0, r3]
0065423c: ldr      r3, [r0]
00654240: str      r2, [r0, #0x1c]
00654244: str      r2, [r0, #0x14]
00654248: str      r2, [r0, #0x18]
0065424c: ldr      r5, [r3, #-0xc]
00654250: ldr      r1, [pc, #0x184]
00654254: add      r5, r0, r5
00654258: add      r1, pc, r1
0065425c: mov      r0, r5
00654260: bl       #0x64d0bc
00654264: add      r2, sp, #0x40
00654268: add      r3, r4, #4
0065426c: str      r0, [sp, #0x40]
00654270: add      r1, r5, #0x30
00654274: add      r0, sp, #0x48
00654278: str      r3, [sp, #0x44]
0065427c: bl       #0x63a8ec
00654280: ldr      r3, [r4]
00654284: ldr      r1, [pc, #0x154]
00654288: ldr      r5, [r3, #-0xc]
0065428c: add      r1, pc, r1
00654290: add      r5, r4, r5
00654294: mov      r0, r5
00654298: bl       #0x64d0bc
0065429c: add      r2, sp, #0x30
006542a0: add      r3, r4, #8
006542a4: str      r0, [sp, #0x30]
006542a8: add      r1, r5, #0x30
006542ac: add      r0, sp, #0x38
006542b0: str      r3, [sp, #0x34]
006542b4: bl       #0x63a8ec
006542b8: ldr      r3, [r4]
006542bc: ldr      r1, [pc, #0x120]
006542c0: ldr      r5, [r3, #-0xc]
006542c4: add      r1, pc, r1
006542c8: add      r5, r4, r5
006542cc: mov      r0, r5
006542d0: bl       #0x64d0bc
006542d4: add      r2, sp, #0x20
006542d8: add      r3, r4, #0xc
006542dc: str      r0, [sp, #0x20]
006542e0: add      r1, r5, #0x30
006542e4: add      r0, sp, #0x28
006542e8: str      r3, [sp, #0x24]
006542ec: bl       #0x63a8ec
006542f0: ldr      r3, [r4]
006542f4: ldr      r1, [pc, #0xec]
006542f8: ldr      r5, [r3, #-0xc]
006542fc: add      r1, pc, r1
00654300: add      r5, r4, r5
00654304: mov      r0, r5
00654308: bl       #0x64d0bc
0065430c: add      r2, sp, #0x10
00654310: add      r3, r4, #0x10
00654314: str      r0, [sp, #0x10]
00654318: add      r1, r5, #0x30
0065431c: add      r0, sp, #0x18
00654320: str      r3, [sp, #0x14]
00654324: bl       #0x63a8ec
00654328: ldr      r3, [r4]
0065432c: ldr      r1, [pc, #0xb8]
00654330: ldr      r5, [r3, #-0xc]
00654334: add      r1, pc, r1
00654338: add      r5, r4, r5
0065433c: mov      r0, r5
00654340: bl       #0x64d0bc
00654344: add      r2, sp, #0x60
00654348: add      r3, r4, #0x14
0065434c: str      r0, [sp, #0x60]
00654350: add      r1, r5, #0x30
00654354: add      r0, sp, #0x68
00654358: str      r3, [sp, #0x64]
0065435c: bl       #0x63a8ec
00654360: ldr      r3, [r4]
00654364: ldr      r1, [pc, #0x84]
00654368: ldr      r5, [r3, #-0xc]
0065436c: add      r1, pc, r1
00654370: add      r5, r4, r5
00654374: mov      r0, r5
00654378: bl       #0x64d0bc
0065437c: mov      r2, sp
00654380: add      r3, r4, #0x20
00654384: str      r0, [sp]
00654388: add      r1, r5, #0x30
0065438c: add      r0, sp, #8
00654390: str      r3, [sp, #4]
00654394: bl       #0x63a8ec
00654398: ldr      r3, [r4]
0065439c: ldr      r1, [pc, #0x50]
006543a0: ldr      r5, [r3, #-0xc]
006543a4: add      r1, pc, r1
006543a8: add      r5, r4, r5
006543ac: mov      r0, r5
006543b0: bl       #0x64d0bc
006543b4: add      r3, r4, #0x24
006543b8: str      r0, [sp, #0x50]
006543bc: add      r1, r5, #0x30
006543c0: add      r0, sp, #0x58
006543c4: add      r2, sp, #0x50
006543c8: str      r3, [sp, #0x54]
006543cc: bl       #0x63a8ec
006543d0: mov      r0, r4
006543d4: add      sp, sp, #0x74
006543d8: pop      {r4, r5, pc}
006543dc: eoreq    r1, sb, r8, rrx
006543e0: eoreq    r1, sb, r4, asr #32
006543e4: eoreq    r1, sb, ip, lsl r0

# _ZZN6glitch2ps9PSManager20createParticleSystemINS0_9SParticleENS0_16PGenerationModelIS3_EENS0_10PSizeModelIS3_EENS0_11PColorModelIS3_EENS0_13PEmitterModelIS3_EENS0_12PMotionModelIS3_EENS0_12PForcesModelIS3_EENS0_10PSpinModelIS3_EENS0_10PLifeModelIS3_EENS0_25PRenderDataBillboardModelIS3_NS0_27PSNullShaderParametersBakerENS0_16PSNullColorBakerIS3_EENS0_20PSGenericNormalBakerIS3_EENS0_22PSGenericPositionBakerIS3_EENS0_23PSGenericTexCoordsBakerIS3_EEEEEEPNS0_15IParticleSystemIT_EEvEN5MixinC1Ev
00654ec4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00654ec8: mov      r4, r0
00654ecc: sub      sp, sp, #0x84
00654ed0: ldr      r7, [pc, #0x410]
00654ed4: add      r0, r0, #0x180
00654ed8: bl       #0x64d1d0
00654edc: ldr      r2, [pc, #0x408]
00654ee0: add      r7, pc, r7
00654ee4: movw     r3, #0xcd15
00654ee8: ldr      r8, [r7, r2]
00654eec: mov      r5, #0
00654ef0: movt     r3, #0x75b
00654ef4: ldmib    r8, {r2, ip}
00654ef8: add      r1, r8, #0xc
00654efc: str      r2, [r4]
00654f00: ldr      r2, [r2, #-0xc]
00654f04: add      r0, r4, #0x10
00654f08: mov      r6, r4
00654f0c: str      ip, [r4, r2]
00654f10: str      r3, [r4, #8]
00654f14: str      r3, [r4, #4]
00654f18: str      r5, [r4, #0xc]
00654f1c: bl       #0x6545d0
00654f20: add      r1, r8, #0x14
00654f24: add      r0, r4, #0x24
00654f28: bl       #0x6543f8
00654f2c: add      r1, r8, #0x1c
00654f30: add      r0, r4, #0x38
00654f34: bl       #0x654690
00654f38: add      r1, r8, #0x24
00654f3c: add      r0, r4, #0x60
00654f40: bl       #0x6548fc
00654f44: add      r1, r8, #0x2c
00654f48: add      r0, r4, #0x78
00654f4c: bl       #0x6540ec
00654f50: ldr      r3, [r8, #0x34]
00654f54: ldr      r2, [r8, #0x38]
00654f58: add      r1, r8, #0x3c
00654f5c: str      r3, [r4, #0x94]
00654f60: ldr      r3, [r3, #-0xc]
00654f64: add      r0, r4, #0xa8
00654f68: add      sl, r4, #0x10c
00654f6c: add      r3, r4, r3
00654f70: str      r2, [r3, #0x94]
00654f74: str      r5, [r4, #0x98]
00654f78: str      r5, [r4, #0x9c]
00654f7c: str      r5, [r4, #0xa0]
00654f80: strb     r5, [r4, #0xa4]
00654f84: bl       #0x654218
00654f88: add      r1, r8, #0x44
00654f8c: add      r0, r4, #0xd0
00654f90: bl       #0x65452c
00654f94: ldr      r3, [r8, #0x4c]
00654f98: ldr      r1, [r8, #0x50]
00654f9c: mov      r2, #0x40
00654fa0: str      r3, [r6, #0xdc]!
00654fa4: ldr      r3, [r3, #-0xc]
00654fa8: mov      r0, sl
00654fac: mov      r8, #1
00654fb0: str      r1, [r6, r3]
00654fb4: mov      r3, #0x104
00654fb8: mov      r1, #0xff
00654fbc: strh     r1, [r4, r3]
00654fc0: movw     r3, #0x106
00654fc4: mov      r1, #6
00654fc8: strh     r1, [r4, r3]
00654fcc: str      r5, [r4, #0xe0]
00654fd0: str      r5, [r4, #0xe4]
00654fd4: str      r5, [r4, #0xec]
00654fd8: str      r5, [r4, #0xf0]
00654fdc: str      r5, [r4, #0xf4]
00654fe0: str      r5, [r4, #0xf8]
00654fe4: str      r5, [r4, #0xfc]
00654fe8: str      r5, [r4, #0x100]
00654fec: str      r5, [r4, #0x108]
00654ff0: strb     r5, [r4, #0x14c]
00654ff4: mov      r1, r5
00654ff8: bl       #0x30e460
00654ffc: ldr      r1, [r4, #0xdc]
00655000: mov      r2, #0xbf000000
00655004: mov      r3, #0x3f800000
00655008: add      r2, r2, #0x800000
0065500c: str      r2, [r4, #0x158]
00655010: str      r3, [r4, #0x164]
00655014: str      r3, [r4, #0x10c]
00655018: str      r3, [r4, #0x120]
0065501c: str      r3, [r4, #0x134]
00655020: str      r3, [r4, #0x148]
00655024: strb     r8, [r4, #0x14c]
00655028: str      r2, [r4, #0x150]
0065502c: str      r2, [r4, #0x154]
00655030: str      r3, [r4, #0x15c]
00655034: str      r3, [r4, #0x160]
00655038: strb     r8, [r4, #0x168]
0065503c: str      r5, [r4, #0x16c]
00655040: str      r5, [r4, #0x170]
00655044: str      r5, [r4, #0x178]
00655048: str      r5, [r4, #0x17c]
0065504c: ldr      sb, [r1, #-0xc]
00655050: ldr      r1, [pc, #0x298]
00655054: add      sb, r6, sb
00655058: add      r1, pc, r1
0065505c: mov      r0, sb
00655060: bl       #0x64d0bc
00655064: add      r2, sp, #0x70
00655068: add      r3, r4, #0x168
0065506c: str      r0, [sp, #0x70]
00655070: add      r1, sb, #0x30
00655074: add      r0, sp, #0x78
00655078: str      r3, [sp, #0x74]
0065507c: bl       #0x63a8ec
00655080: ldr      r3, [r4, #0xdc]
00655084: ldr      r1, [pc, #0x268]
00655088: ldr      sb, [r3, #-0xc]
0065508c: add      r1, pc, r1
00655090: add      sb, r6, sb
00655094: mov      r0, sb
00655098: bl       #0x64d0bc
0065509c: add      r2, sp, #0x60
006550a0: add      r3, r4, #0xe0
006550a4: str      r0, [sp, #0x60]
006550a8: add      r1, sb, #0x30
006550ac: add      r0, sp, #0x68
006550b0: str      r3, [sp, #0x64]
006550b4: bl       #0x63a8ec
006550b8: ldr      r3, [r4, #0xdc]
006550bc: ldr      r1, [pc, #0x234]
006550c0: ldr      sb, [r3, #-0xc]
006550c4: add      r1, pc, r1
006550c8: add      sb, r6, sb
006550cc: mov      r0, sb
006550d0: bl       #0x64d0bc
006550d4: add      r2, sp, #0x50
006550d8: add      r3, r4, #0x17c
006550dc: str      r0, [sp, #0x50]
006550e0: add      r1, sb, #0x30
006550e4: add      r0, sp, #0x58
006550e8: str      r3, [sp, #0x54]
006550ec: bl       #0x63a8ec
006550f0: ldr      r3, [r4, #0xdc]
006550f4: ldr      r1, [pc, #0x200]
006550f8: ldr      sb, [r3, #-0xc]
006550fc: add      r1, pc, r1
00655100: add      sb, r6, sb
00655104: mov      r0, sb
00655108: bl       #0x64d0bc
0065510c: add      r2, sp, #0x40
00655110: add      r3, r4, #0xe4
00655114: str      r0, [sp, #0x40]
00655118: add      r1, sb, #0x30
0065511c: add      r0, sp, #0x48
00655120: str      r3, [sp, #0x44]
00655124: bl       #0x63a8ec
00655128: ldr      r3, [r4, #0xdc]
0065512c: ldr      r1, [pc, #0x1cc]
00655130: ldr      sb, [r3, #-0xc]
00655134: add      r1, pc, r1
00655138: add      sb, r6, sb
0065513c: mov      r0, sb
00655140: bl       #0x64d0bc
00655144: add      r2, sp, #0x30
00655148: add      r3, r4, #0x16c
0065514c: str      r0, [sp, #0x30]
00655150: add      r1, sb, #0x30
00655154: add      r0, sp, #0x38
00655158: str      r3, [sp, #0x34]
0065515c: bl       #0x63a8ec
00655160: ldr      r3, [r4, #0xdc]
00655164: ldr      r1, [pc, #0x198]
00655168: ldr      sb, [r3, #-0xc]
0065516c: add      r1, pc, r1
00655170: add      sb, r6, sb
00655174: mov      r0, sb
00655178: bl       #0x64d0bc
0065517c: add      r2, sp, #0x20
00655180: add      r3, r4, #0x174
00655184: str      r0, [sp, #0x20]
00655188: add      r1, sb, #0x30
0065518c: add      r0, sp, #0x28
00655190: str      r3, [sp, #0x24]
00655194: bl       #0x63a8ec
00655198: ldr      r3, [r4, #0xdc]
0065519c: ldr      r1, [pc, #0x164]
006551a0: ldr      sb, [r3, #-0xc]
006551a4: add      r1, pc, r1
006551a8: add      sb, r6, sb
006551ac: mov      r0, sb
006551b0: bl       #0x64d0bc
006551b4: add      r2, sp, #0x10
006551b8: str      r0, [sp, #0x10]
006551bc: add      r1, sb, #0x30
006551c0: add      r0, sp, #0x18
006551c4: str      sl, [sp, #0x14]
006551c8: bl       #0x63a8ec
006551cc: ldr      r3, [r4, #0xdc]
006551d0: ldr      r1, [pc, #0x134]
006551d4: ldr      sl, [r3, #-0xc]
006551d8: add      r1, pc, r1
006551dc: add      sl, r6, sl
006551e0: mov      r0, sl
006551e4: bl       #0x64d0bc
006551e8: add      r3, r4, #0x150
006551ec: str      r0, [sp]
006551f0: add      r1, sl, #0x30
006551f4: add      r0, sp, #8
006551f8: mov      r2, sp
006551fc: str      r3, [sp, #4]
00655200: bl       #0x63a8ec
00655204: ldr      r2, [r4, #0xdc]
00655208: mov      r3, #0
0065520c: mov      ip, #0x3f000000
00655210: ldr      r2, [r2, #-0xc]
00655214: mov      r1, r5
00655218: mov      r0, r8
0065521c: add      r2, r6, r2
00655220: strb     r5, [r2, #4]
00655224: ldr      r2, [r4, #0xdc]
00655228: ldr      r2, [r2, #-0xc]
0065522c: add      r2, r6, r2
00655230: strb     r5, [r2, #5]
00655234: ldr      r2, [r4, #0xdc]
00655238: ldr      r2, [r2, #-0xc]
0065523c: add      r2, r6, r2
00655240: str      ip, [r2, #8]
00655244: str      r3, [r2, #0x10]
00655248: str      r3, [r2, #0xc]
0065524c: ldr      r2, [r4, #0xdc]
00655250: ldr      r2, [r2, #-0xc]
00655254: add      r2, r6, r2
00655258: str      ip, [r2, #0x18]
0065525c: str      r3, [r2, #0x1c]
00655260: str      r3, [r2, #0x14]
00655264: ldr      r3, [r4, #0xdc]
00655268: ldr      r3, [r3, #-0xc]
0065526c: add      r6, r6, r3
00655270: strb     r5, [r6, #0x20]
00655274: bl       #0x5341ac
00655278: ldr      r3, [pc, #0x90]
0065527c: str      r0, [r4, #0x178]
00655280: mov      r0, r4
00655284: ldr      r3, [r7, r3]
00655288: add      fp, r3, #0x138
0065528c: add      sl, r3, #0xc
00655290: add      r8, r3, #0x1f4
00655294: add      r7, r3, #0x30
00655298: add      r6, r3, #0x4c
0065529c: add      r5, r3, #0x6c
006552a0: add      ip, r3, #0x90
006552a4: add      r1, r3, #0xac
006552a8: add      r2, r3, #0xcc
006552ac: add      sb, r3, #0xf8
006552b0: add      r3, r3, #0x118
006552b4: str      sl, [r4]
006552b8: str      r8, [r4, #0x180]
006552bc: str      r7, [r4, #0x10]
006552c0: str      r6, [r4, #0x24]
006552c4: str      r5, [r4, #0x38]
006552c8: str      ip, [r4, #0x60]
006552cc: str      r1, [r4, #0x78]
006552d0: str      r2, [r4, #0x94]
006552d4: str      sb, [r4, #0xa8]
006552d8: str      r3, [r4, #0xd0]
006552dc: str      fp, [r4, #0xdc]
006552e0: add      sp, sp, #0x84
006552e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006552e8: ldrhteq  pc, [r3], -r0
006552ec: strheq   r0, [r0], -r4
006552f0: eoreq    r0, sb, r8, lsl r5
006552f4: eoreq    r0, sb, ip, asr r0

# _ZN6glitch7collada15CColladaFactory20createParticleSystemERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_8SEmitterEPNS_3res6vectorINSA_6StringEEEPNS0_14CRootSceneNodeE
00634c64: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00634c68: mov      r0, #0x198
00634c6c: sub      sp, sp, #0x6c
00634c70: mov      r8, r1
00634c74: mov      r1, #0
00634c78: mov      r6, r3
00634c7c: mov      r7, r2
00634c80: bl       #0x5341ac
00634c84: ldr      ip, [sp, #0x94]
00634c88: ldr      r3, [sp, #0x90]
00634c8c: mov      r1, r8
00634c90: mov      r2, r6
00634c94: mov      r4, r0
00634c98: str      ip, [sp]
00634c9c: bl       #0x64ed94
00634ca0: ldr      r3, [r6, #0x50]
00634ca4: ldr      r5, [pc, #0xae8]
00634ca8: cmp      r3, #0
00634cac: add      r5, pc, r5
00634cb0: bne      #0x634db4
00634cb4: ldr      r3, [r6, #0x54]
00634cb8: ldr      r3, [r3]
00634cbc: sub      r3, r3, #1
00634cc0: cmp      r3, #6
00634cc4: addls    pc, pc, r3, lsl #2
00634cc8: b        #0x634da8
00634ccc: b        #0x634f4c
00634cd0: b        #0x634ff8
00634cd4: b        #0x634ce8
00634cd8: b        #0x634ce8
00634cdc: b        #0x634da8
00634ce0: b        #0x634da8
00634ce4: b        #0x634e94
00634ce8: ldr      r3, [r4]
00634cec: mov      r2, #1
00634cf0: mov      r1, r7
00634cf4: mov      r0, r4
00634cf8: mov      lr, pc
00634cfc: ldr      pc, [r3, #0xf4]
00634d00: ldr      r3, [r6, #0x54]
00634d04: ldr      r5, [r3, #4]
00634d08: ldr      r2, [r3, #8]
00634d0c: cmp      r5, #0
00634d10: beq      #0x634da8
00634d14: cmp      r2, #2
00634d18: beq      #0x63575c
00634d1c: cmp      r2, #3
00634d20: beq      #0x635734
00634d24: cmp      r2, #1
00634d28: bne      #0x634da8
00634d2c: tst      r5, #2
00634d30: beq      #0x634d68
00634d34: ldr      r0, [r4, #0x178]
00634d38: ldr      ip, [r3, #0xc]
00634d3c: mov      r1, #0
00634d40: ldr      lr, [r0]
00634d44: add      r2, sp, #0x3c
00634d48: ldr      lr, [lr, #-0xc]
00634d4c: str      ip, [sp, #0x3c]
00634d50: ldr      ip, [r3, #0x10]
00634d54: add      r0, r0, lr
00634d58: str      ip, [sp, #0x40]
00634d5c: ldr      r3, [r3, #0x14]
00634d60: str      r3, [sp, #0x44]
00634d64: bl       #0x6309ac
00634d68: tst      r5, #4
00634d6c: beq      #0x634da8
00634d70: ldr      r3, [r6, #0x54]
00634d74: ldr      r0, [r4, #0x178]
00634d78: mov      r1, #1
00634d7c: ldr      ip, [r3, #0x18]
00634d80: ldr      lr, [r0]
00634d84: add      r2, sp, #0x30
00634d88: ldr      lr, [lr, #-0xc]
00634d8c: str      ip, [sp, #0x30]
00634d90: ldr      ip, [r3, #0x1c]
00634d94: add      r0, r0, lr
00634d98: str      ip, [sp, #0x34]
00634d9c: ldr      r3, [r3, #0x20]
00634da0: str      r3, [sp, #0x38]
00634da4: bl       #0x6309ac
00634da8: mov      r0, r4
00634dac: add      sp, sp, #0x6c
00634db0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00634db4: cmp      r3, #2
00634db8: bne      #0x634da8
00634dbc: ldr      r3, [r6, #0x54]
00634dc0: mov      r1, r8
00634dc4: add      r0, sp, #0x50
00634dc8: ldr      r3, [r3, #4]
00634dcc: mov      r2, r7
00634dd0: ldr      r3, [r3, #4]
00634dd4: add      r3, r3, #1
00634dd8: bl       #0x61aaa8
00634ddc: ldr      r5, [sp, #0x50]
00634de0: cmp      r5, #0
00634de4: moveq    r6, r5
00634de8: beq      #0x634e0c
00634dec: ldr      r3, [r5, #4]
00634df0: mov      r6, r5
00634df4: add      r3, r3, #1
00634df8: str      r3, [r5, #4]
00634dfc: ldr      r0, [sp, #0x50]
00634e00: cmp      r0, #0
00634e04: beq      #0x634e0c
00634e08: bl       #0x31d584
00634e0c: mov      r1, r5
00634e10: add      r0, sp, #0x4c
00634e14: mov      r2, #0
00634e18: ldr      r3, [r5]
00634e1c: mov      lr, pc
00634e20: ldr      pc, [r3, #0x14]
00634e24: mov      r1, r7
00634e28: ldr      r3, [r4]
00634e2c: mov      r0, r4
00634e30: mov      r2, #0
00634e34: mov      lr, pc
00634e38: ldr      pc, [r3, #0xf4]
00634e3c: ldr      r2, [sp, #0x4c]
00634e40: ldr      r3, [r4]
00634e44: mov      r0, r4
00634e48: cmp      r2, #0
00634e4c: ldr      r3, [r3, #0x100]
00634e50: str      r2, [sp, #0x48]
00634e54: ldrne    r1, [r2, #4]
00634e58: addne    r1, r1, #1
00634e5c: strne    r1, [r2, #4]
00634e60: add      r1, sp, #0x48
00634e64: blx      r3
00634e68: ldr      r0, [sp, #0x48]
00634e6c: cmp      r0, #0
00634e70: beq      #0x634e78
00634e74: bl       #0x31d584
00634e78: ldr      r0, [sp, #0x4c]
00634e7c: cmp      r0, #0
00634e80: beq      #0x634e88
00634e84: bl       #0x31d584
00634e88: mov      r0, r6
00634e8c: bl       #0x31d584
00634e90: b        #0x634da8
00634e94: mov      r1, #0x60000
00634e98: mov      ip, #0xf
00634e9c: add      r1, r1, #3
00634ea0: mov      r3, #0x3f800000
00634ea4: add      r0, sp, #0x58
00634ea8: mov      r2, r7
00634eac: str      ip, [sp, #4]
00634eb0: str      ip, [sp]
00634eb4: bl       #0x6d7808
00634eb8: ldr      r3, [sp, #0x58]
00634ebc: add      r0, sp, #0x4c
00634ec0: mov      r2, #0
00634ec4: mov      r1, r3
00634ec8: ldr      r3, [r3]
00634ecc: mov      lr, pc
00634ed0: ldr      pc, [r3, #0x14]
00634ed4: mov      r1, r7
00634ed8: ldr      r3, [r4]
00634edc: mov      r0, r4
00634ee0: mov      r2, #0
00634ee4: mov      lr, pc
00634ee8: ldr      pc, [r3, #0xf4]
00634eec: ldr      r2, [sp, #0x4c]
00634ef0: ldr      r3, [r4]
00634ef4: mov      r0, r4
00634ef8: cmp      r2, #0
00634efc: ldr      r3, [r3, #0x100]
00634f00: str      r2, [sp, #0x54]
00634f04: ldrne    r1, [r2, #4]
00634f08: addne    r1, r1, #1
00634f0c: strne    r1, [r2, #4]
00634f10: add      r1, sp, #0x54
00634f14: blx      r3
00634f18: ldr      r0, [sp, #0x54]
00634f1c: cmp      r0, #0
00634f20: beq      #0x634f28
00634f24: bl       #0x31d584
00634f28: ldr      r0, [sp, #0x4c]
00634f2c: cmp      r0, #0
00634f30: beq      #0x634f38
00634f34: bl       #0x31d584
00634f38: ldr      r0, [sp, #0x58]
00634f3c: cmp      r0, #0
00634f40: beq      #0x634da8
00634f44: bl       #0x31d584
00634f48: b        #0x634da8
00634f4c: mov      r1, #0x60000
00634f50: add      r1, r1, #3
00634f54: mov      r3, #0x3f800000
00634f58: add      r0, sp, #0x4c
00634f5c: mov      r2, r7
00634f60: bl       #0x6d8cb4
00634f64: ldr      r3, [sp, #0x4c]
00634f68: add      r0, sp, #0x58
00634f6c: mov      r2, #0
00634f70: mov      r1, r3
00634f74: ldr      r3, [r3]
00634f78: mov      lr, pc
00634f7c: ldr      pc, [r3, #0x14]
00634f80: mov      r1, r7
00634f84: ldr      r3, [r4]
00634f88: mov      r0, r4
00634f8c: mov      r2, #0
00634f90: mov      lr, pc
00634f94: ldr      pc, [r3, #0xf4]
00634f98: ldr      r2, [sp, #0x58]
00634f9c: ldr      r3, [r4]
00634fa0: mov      r0, r4
00634fa4: cmp      r2, #0
00634fa8: ldr      r3, [r3, #0x100]
00634fac: str      r2, [sp, #0x64]
00634fb0: ldrne    r1, [r2, #4]
00634fb4: addne    r1, r1, #1
00634fb8: strne    r1, [r2, #4]
00634fbc: add      r1, sp, #0x64
00634fc0: blx      r3
00634fc4: ldr      r0, [sp, #0x64]
00634fc8: cmp      r0, #0
00634fcc: beq      #0x634fd4
00634fd0: bl       #0x31d584
00634fd4: ldr      r0, [sp, #0x58]
00634fd8: cmp      r0, #0
00634fdc: beq      #0x634fe4
00634fe0: bl       #0x31d584
00634fe4: ldr      r0, [sp, #0x4c]
00634fe8: cmp      r0, #0
00634fec: beq      #0x634da8
00634ff0: bl       #0x31d584
00634ff4: b        #0x634da8
00634ff8: ldr      r3, [pc, #0x798]
00634ffc: mov      r2, #0x48
00635000: str      r2, [sp]
00635004: add      r3, pc, r3
00635008: str      r3, [sp, #4]
0063500c: mov      r3, #0
00635010: str      r3, [sp, #8]
00635014: ldr      ip, [r7]
00635018: mov      r3, #4
0063501c: mov      r2, #1
00635020: add      r0, sp, #0x60
00635024: mov      r1, r7
00635028: mov      lr, pc
0063502c: ldr      pc, [ip, #0x78]
00635030: ldr      r8, [sp, #0x60]
00635034: mov      r1, #0
00635038: mov      r0, #0x38
0063503c: cmp      r8, #0
00635040: ldrne    r3, [r8, #4]
00635044: addne    r3, r3, #1
00635048: strne    r3, [r8, #4]
0063504c: bl       #0x5341ac
00635050: ldr      r2, [pc, #0x744]
00635054: mov      r3, #0
00635058: mov      r1, #0x60000
0063505c: ldr      r2, [r5, r2]
00635060: mov      r6, r0
00635064: str      r3, [r0, #0x10]
00635068: add      r2, r2, #8
0063506c: str      r3, [r0, #4]
00635070: str      r3, [r0, #8]
00635074: str      r3, [r0, #0xc]
00635078: str      r2, [r0]
0063507c: add      r1, r1, #3
00635080: add      r0, r0, #0x14
00635084: bl       #0x5a135c
00635088: cmp      r8, #0
0063508c: str      r8, [r6, #0x18]
00635090: ldrne    r3, [r8, #4]
00635094: mov      r1, #0x24
00635098: mov      r0, #1
0063509c: addne    r3, r3, #1
006350a0: strne    r3, [r8, #4]
006350a4: ldr      r2, [r6, #4]
006350a8: str      r1, [r6, #0x20]
006350ac: mov      r1, #0x18
006350b0: mov      r3, #0
006350b4: add      r2, r2, #1
006350b8: str      r1, [r6, #0x28]
006350bc: cmp      r8, #0
006350c0: mov      r1, #6
006350c4: strb     r3, [r6, #0x34]
006350c8: str      r2, [r6, #4]
006350cc: str      r3, [r6, #0x1c]
006350d0: str      r3, [r6, #0x24]
006350d4: strh     r0, [r6, #0x2c]
006350d8: strh     r1, [r6, #0x2e]
006350dc: str      r3, [r6, #0x30]
006350e0: beq      #0x6350ec
006350e4: mov      r0, r8
006350e8: bl       #0x31d584
006350ec: ldr      r0, [sp, #0x60]
006350f0: cmp      r0, #0
006350f4: beq      #0x6350fc
006350f8: bl       #0x31d584
006350fc: ldr      r2, [r6, #0x14]
00635100: mov      sb, #0
00635104: mov      r8, #1
00635108: add      r5, sp, #0x58
0063510c: str      r2, [sp, #0x14]
00635110: str      r8, [sp, #8]
00635114: str      sb, [sp]
00635118: str      sb, [sp, #4]
0063511c: ldr      ip, [r7]
00635120: mov      r3, #4
00635124: mov      r0, r5
00635128: mov      r1, r7
0063512c: mov      r2, sb
00635130: mov      lr, pc
00635134: ldr      pc, [ip, #0x78]
00635138: mov      r1, r5
0063513c: mvn      r2, #0
00635140: ldr      r0, [sp, #0x14]
00635144: bl       #0x5a15c0
00635148: mov      r3, #0x18
0063514c: mul      r5, r3, r0
00635150: mov      r1, sb
00635154: mov      r0, r5
00635158: ldr      sl, [sp, #0x58]
0063515c: bl       #0x5341a8
00635160: mov      r3, r8
00635164: mov      r2, r0
00635168: mov      r1, r5
0063516c: mov      r0, sl
00635170: bl       #0x5a1cb4
00635174: ldr      ip, [sp, #0x14]
00635178: mov      r1, #4
0063517c: mov      r5, #0
00635180: ldr      r0, [ip, #0x24]
00635184: bl       #0x5a19f0
00635188: ldr      lr, [sp, #0x14]
0063518c: ldr      r1, [sp, #0x14]
00635190: ldr      r2, [sp, #0x14]
00635194: add      lr, lr, #0x24
00635198: str      lr, [sp, #0x18]
0063519c: ldr      r3, [lr, #4]
006351a0: add      r8, r1, #0x14
006351a4: mov      r1, #4
006351a8: add      r3, r0, r3
006351ac: str      r3, [sp, #0x1c]
006351b0: ldr      r0, [r2, #0x14]
006351b4: bl       #0x5a19f0
006351b8: ldr      r1, [r8, #4]
006351bc: mov      ip, #0xbf000000
006351c0: mov      r2, #0x3f000000
006351c4: add      r3, r0, r1
006351c8: str      ip, [r0, r1]
006351cc: str      ip, [r3, #4]
006351d0: str      r5, [r3, #8]
006351d4: ldrh     r0, [r8, #0xe]
006351d8: mov      lr, #6
006351dc: mov      fp, #0xbf000000
006351e0: add      r1, r3, r0
006351e4: str      ip, [r3, r0]
006351e8: str      r2, [r1, #4]
006351ec: str      r5, [r1, #8]
006351f0: ldrh     r0, [r8, #0xe]
006351f4: add      fp, fp, #0x800000
006351f8: add      r1, r3, r0, lsl #1
006351fc: str      r2, [r3, r0, lsl #1]
00635200: str      r2, [r1, #4]
00635204: str      r5, [r1, #8]
00635208: ldrh     r0, [r8, #0xe]
0063520c: add      r0, r0, r0, lsl #1
00635210: add      r1, r3, r0
00635214: str      r2, [r3, r0]
00635218: str      ip, [r1, #4]
0063521c: str      r5, [r1, #8]
00635220: ldrh     r0, [r8, #0xe]
00635224: add      r1, r3, r0, lsl #2
00635228: str      ip, [r3, r0, lsl #2]
0063522c: str      ip, [r1, #4]
00635230: str      r5, [r1, #8]
00635234: ldrh     r0, [r8, #0xe]
00635238: add      r0, r0, r0, lsl #2
0063523c: add      r1, r3, r0
00635240: str      ip, [r3, r0]
00635244: str      r2, [r1, #4]
00635248: str      r5, [r1, #8]
0063524c: ldrh     r0, [r8, #0xe]
00635250: mul      r0, lr, r0
00635254: add      r1, r3, r0
00635258: str      r2, [r3, r0]
0063525c: str      r2, [r1, #4]
00635260: str      r5, [r1, #8]
00635264: ldrh     lr, [r8, #0xe]
00635268: mov      r1, #7
0063526c: ldr      r0, [sp, #0x14]
00635270: mul      lr, r1, lr
00635274: add      sl, r0, #0x34
00635278: add      r0, r3, lr
0063527c: str      r2, [r3, lr]
00635280: str      ip, [r0, #4]
00635284: str      r5, [r0, #8]
00635288: ldrh     lr, [r8, #0xe]
0063528c: mov      r1, #4
00635290: add      r0, r3, lr, lsl #3
00635294: str      ip, [r3, lr, lsl #3]
00635298: str      ip, [r0, #8]
0063529c: str      r5, [r0, #4]
006352a0: ldrh     lr, [r8, #0xe]
006352a4: add      lr, lr, lr, lsl #3
006352a8: add      r0, r3, lr
006352ac: str      ip, [r3, lr]
006352b0: str      r2, [r0, #8]
006352b4: str      r5, [r0, #4]
006352b8: ldrh     lr, [r8, #0xe]
006352bc: mov      r0, #0xa
006352c0: mul      lr, r0, lr
006352c4: add      r0, r3, lr
006352c8: str      r2, [r3, lr]
006352cc: str      r2, [r0, #8]
006352d0: str      r5, [r0, #4]
006352d4: ldrh     lr, [r8, #0xe]
006352d8: mov      r0, #0xb
006352dc: mul      lr, r0, lr
006352e0: add      r0, r3, lr
006352e4: str      r2, [r3, lr]
006352e8: str      ip, [r0, #8]
006352ec: str      r5, [r0, #4]
006352f0: ldrh     lr, [r8, #0xe]
006352f4: mov      r0, #0xc
006352f8: mul      lr, r0, lr
006352fc: add      r0, r3, lr
00635300: str      ip, [r3, lr]
00635304: str      ip, [r0, #8]
00635308: str      r5, [r0, #4]
0063530c: ldrh     lr, [r8, #0xe]
00635310: mov      r0, #0xd
00635314: mul      lr, r0, lr
00635318: add      r0, r3, lr
0063531c: str      ip, [r3, lr]
00635320: str      r2, [r0, #8]
00635324: str      r5, [r0, #4]
00635328: ldrh     lr, [r8, #0xe]
0063532c: mov      r0, #0xe
00635330: mul      lr, r0, lr
00635334: add      r0, r3, lr
00635338: str      r2, [r3, lr]
0063533c: str      r2, [r0, #8]
00635340: str      r5, [r0, #4]
00635344: ldrh     lr, [r8, #0xe]
00635348: mov      r0, #0xf
0063534c: mul      lr, r0, lr
00635350: add      r0, r3, lr
00635354: str      r2, [r3, lr]
00635358: str      ip, [r0, #8]
0063535c: str      r5, [r0, #4]
00635360: ldrh     lr, [r8, #0xe]
00635364: add      r0, r3, lr, lsl #4
00635368: str      r5, [r3, lr, lsl #4]
0063536c: str      ip, [r0, #4]
00635370: str      ip, [r0, #8]
00635374: ldrh     lr, [r8, #0xe]
00635378: add      lr, lr, lr, lsl #4
0063537c: add      r0, r3, lr
00635380: str      r5, [r3, lr]
00635384: str      ip, [r0, #4]
00635388: str      r2, [r0, #8]
0063538c: ldrh     lr, [r8, #0xe]
00635390: mov      r0, #0x12
00635394: mul      lr, r0, lr
00635398: add      r0, r3, lr
0063539c: str      r5, [r3, lr]
006353a0: str      r2, [r0, #8]
006353a4: str      r2, [r0, #4]
006353a8: ldrh     lr, [r8, #0xe]
006353ac: mov      r0, #0x13
006353b0: mul      lr, r0, lr
006353b4: add      r0, r3, lr
006353b8: str      r5, [r3, lr]
006353bc: str      ip, [r0, #8]
006353c0: str      r2, [r0, #4]
006353c4: ldrh     lr, [r8, #0xe]
006353c8: mov      r0, #0x14
006353cc: mul      lr, r0, lr
006353d0: add      r0, r3, lr
006353d4: str      r5, [r3, lr]
006353d8: str      ip, [r0, #8]
006353dc: str      ip, [r0, #4]
006353e0: ldrh     lr, [r8, #0xe]
006353e4: mov      r0, #0x15
006353e8: mul      lr, r0, lr
006353ec: add      r0, r3, lr
006353f0: str      r5, [r3, lr]
006353f4: str      ip, [r0, #4]
006353f8: str      r2, [r0, #8]
006353fc: ldrh     lr, [r8, #0xe]
00635400: mov      r0, #0x16
00635404: mul      lr, r0, lr
00635408: add      r0, r3, lr
0063540c: str      r5, [r3, lr]
00635410: str      r2, [r0, #8]
00635414: str      r2, [r0, #4]
00635418: ldrh     lr, [r8, #0xe]
0063541c: mov      r0, #0x17
00635420: mul      lr, r0, lr
00635424: add      r0, r3, lr
00635428: str      r5, [r3, lr]
0063542c: str      ip, [r0, #8]
00635430: str      r2, [r0, #4]
00635434: ldr      r2, [sp, #0x14]
00635438: ldr      r0, [r2, #0x34]
0063543c: bl       #0x5a19f0
00635440: ldr      r2, [sl, #4]
00635444: mov      ip, #0x3f800000
00635448: mov      r1, r7
0063544c: add      r3, r0, r2
00635450: str      r5, [r0, r2]
00635454: str      ip, [r3, #8]
00635458: str      r5, [r3, #4]
0063545c: ldrh     r0, [sl, #0xe]
00635460: mov      r2, sb
00635464: add      lr, r3, r0
00635468: str      r5, [r3, r0]
0063546c: str      ip, [lr, #8]
00635470: str      r5, [lr, #4]
00635474: ldrh     r7, [sl, #0xe]
00635478: mov      r0, r4
0063547c: add      lr, r3, r7, lsl #1
00635480: str      r5, [r3, r7, lsl #1]
00635484: str      ip, [lr, #8]
00635488: str      r5, [lr, #4]
0063548c: ldrh     r7, [sl, #0xe]
00635490: add      r7, r7, r7, lsl #1
00635494: add      lr, r3, r7
00635498: str      r5, [r3, r7]
0063549c: str      ip, [lr, #8]
006354a0: str      r5, [lr, #4]
006354a4: ldrh     r7, [sl, #0xe]
006354a8: add      lr, r3, r7, lsl #2
006354ac: str      r5, [r3, r7, lsl #2]
006354b0: str      fp, [lr, #8]
006354b4: str      r5, [lr, #4]
006354b8: ldrh     r7, [sl, #0xe]
006354bc: add      r7, r7, r7, lsl #2
006354c0: add      lr, r3, r7
006354c4: str      r5, [r3, r7]
006354c8: str      fp, [lr, #8]
006354cc: str      r5, [lr, #4]
006354d0: ldrh     r7, [sl, #0xe]
006354d4: mov      lr, #6
006354d8: mul      r7, lr, r7
006354dc: add      lr, r3, r7
006354e0: str      r5, [r3, r7]
006354e4: str      fp, [lr, #8]
006354e8: str      r5, [lr, #4]
006354ec: ldrh     r7, [sl, #0xe]
006354f0: mov      lr, #7
006354f4: mul      r7, lr, r7
006354f8: add      lr, r3, r7
006354fc: str      r5, [r3, r7]
00635500: str      fp, [lr, #8]
00635504: str      r5, [lr, #4]
00635508: ldrh     r7, [sl, #0xe]
0063550c: add      lr, r3, r7, lsl #3
00635510: str      r5, [r3, r7, lsl #3]
00635514: str      ip, [lr, #4]
00635518: str      r5, [lr, #8]
0063551c: ldrh     r7, [sl, #0xe]
00635520: add      r7, r7, r7, lsl #3
00635524: add      lr, r3, r7
00635528: str      r5, [r3, r7]
0063552c: str      ip, [lr, #4]
00635530: str      r5, [lr, #8]
00635534: ldrh     r7, [sl, #0xe]
00635538: mov      lr, #0xa
0063553c: mul      r7, lr, r7
00635540: add      lr, r3, r7
00635544: str      r5, [r3, r7]
00635548: str      ip, [lr, #4]
0063554c: str      r5, [lr, #8]
00635550: ldrh     r7, [sl, #0xe]
00635554: mov      lr, #0xb
00635558: mul      r7, lr, r7
0063555c: add      lr, r3, r7
00635560: str      r5, [r3, r7]
00635564: str      ip, [lr, #4]
00635568: str      r5, [lr, #8]
0063556c: ldrh     r7, [sl, #0xe]
00635570: mov      lr, #0xc
00635574: mul      r7, lr, r7
00635578: add      lr, r3, r7
0063557c: str      r5, [r3, r7]
00635580: str      fp, [lr, #4]
00635584: str      r5, [lr, #8]
00635588: ldrh     r7, [sl, #0xe]
0063558c: mov      lr, #0xd
00635590: mul      r7, lr, r7
00635594: add      lr, r3, r7
00635598: str      r5, [r3, r7]
0063559c: str      fp, [lr, #4]
006355a0: str      r5, [lr, #8]
006355a4: ldrh     r7, [sl, #0xe]
006355a8: mov      lr, #0xe
006355ac: mul      r7, lr, r7
006355b0: add      lr, r3, r7
006355b4: str      r5, [r3, r7]
006355b8: str      fp, [lr, #4]
006355bc: str      r5, [lr, #8]
006355c0: ldrh     r7, [sl, #0xe]
006355c4: mov      lr, #0xf
006355c8: mul      r7, lr, r7
006355cc: add      lr, r3, r7
006355d0: str      r5, [r3, r7]
006355d4: str      fp, [lr, #4]
006355d8: str      r5, [lr, #8]
006355dc: ldrh     r7, [sl, #0xe]
006355e0: add      lr, r3, r7, lsl #4
006355e4: str      ip, [r3, r7, lsl #4]
006355e8: str      r5, [lr, #4]
006355ec: str      r5, [lr, #8]
006355f0: ldrh     r7, [sl, #0xe]
006355f4: add      r7, r7, r7, lsl #4
006355f8: add      lr, r3, r7
006355fc: str      ip, [r3, r7]
00635600: str      r5, [lr, #8]
00635604: str      r5, [lr, #4]
00635608: ldrh     r7, [sl, #0xe]
0063560c: mov      lr, #0x12
00635610: mul      r7, lr, r7
00635614: add      lr, r3, r7
00635618: str      ip, [r3, r7]
0063561c: str      r5, [lr, #8]
00635620: str      r5, [lr, #4]
00635624: ldrh     r7, [sl, #0xe]
00635628: mov      lr, #0x13
0063562c: mul      r7, lr, r7
00635630: add      lr, r3, r7
00635634: str      ip, [r3, r7]
00635638: str      r5, [lr, #8]
0063563c: str      r5, [lr, #4]
00635640: ldrh     lr, [sl, #0xe]
00635644: mov      ip, #0x14
00635648: mul      lr, ip, lr
0063564c: add      ip, r3, lr
00635650: str      fp, [r3, lr]
00635654: str      r5, [ip, #8]
00635658: str      r5, [ip, #4]
0063565c: ldrh     lr, [sl, #0xe]
00635660: mov      ip, #0x15
00635664: mul      lr, ip, lr
00635668: add      ip, r3, lr
0063566c: str      fp, [r3, lr]
00635670: str      r5, [ip, #8]
00635674: str      r5, [ip, #4]
00635678: ldrh     lr, [sl, #0xe]
0063567c: mov      ip, #0x16
00635680: mul      lr, ip, lr
00635684: add      ip, r3, lr
00635688: str      fp, [r3, lr]
0063568c: str      r5, [ip, #8]
00635690: str      r5, [ip, #4]
00635694: ldrh     lr, [sl, #0xe]
00635698: mov      ip, #0x17
0063569c: mul      lr, ip, lr
006356a0: add      ip, r3, lr
006356a4: str      fp, [r3, lr]
006356a8: str      r5, [ip, #8]
006356ac: str      r5, [ip, #4]
006356b0: ldr      lr, [sp, #0x14]
006356b4: mov      r3, #0x18
006356b8: str      r3, [lr, #8]
006356bc: ldr      r3, [r4]
006356c0: mov      lr, pc
006356c4: ldr      pc, [r3, #0xf4]
006356c8: ldr      r3, [r4]
006356cc: add      r1, sp, #0x68
006356d0: mov      r0, r4
006356d4: ldr      r3, [r3, #0x100]
006356d8: str      r6, [r1, #-0xc]!
006356dc: ldr      r2, [r6, #4]
006356e0: add      r2, r2, #1
006356e4: str      r2, [r6, #4]
006356e8: blx      r3
006356ec: ldr      r0, [sp, #0x5c]
006356f0: cmp      r0, sb
006356f4: beq      #0x6356fc
006356f8: bl       #0x31d584
006356fc: mov      r0, sl
00635700: bl       #0x62fe18
00635704: mov      r0, r8
00635708: bl       #0x62fe18
0063570c: ldr      ip, [sp, #0x1c]
00635710: cmp      ip, #0
00635714: beq      #0x635720
00635718: ldr      r0, [sp, #0x18]
0063571c: bl       #0x62fe18
00635720: ldr      r0, [sp, #0x58]
00635724: cmp      r0, #0
00635728: beq      #0x634e88
0063572c: bl       #0x31d584
00635730: b        #0x634e88
00635734: ldr      r3, [r4, #0x178]
00635738: mov      r2, #1
0063573c: ldr      r1, [r3]
00635740: ldr      r1, [r1, #-0xc]
00635744: add      r3, r3, r1
00635748: mov      r1, #0
0063574c: strb     r2, [r3, #5]
00635750: strb     r1, [r3, #4]
00635754: strb     r2, [r3, #0x20]
00635758: b        #0x634da8
0063575c: ldr      r0, [r4, #0x178]
00635760: ldr      ip, [r3, #0x18]
00635764: mov      r1, #1
00635768: ldr      lr, [r0]
0063576c: add      r2, sp, #0x24
00635770: ldr      lr, [lr, #-0xc]
00635774: str      ip, [sp, #0x24]
00635778: ldr      ip, [r3, #0x1c]
0063577c: add      r0, r0, lr
00635780: str      ip, [sp, #0x28]
00635784: ldr      r3, [r3, #0x20]
00635788: str      r3, [sp, #0x2c]
0063578c: bl       #0x6309ac
00635790: b        #0x634da8
00635794: eorseq   pc, r5, r4, ror #27
00635798: ldrshteq r6, [r6], -ip
0063579c: andeq    r0, r0, r4, asr ip

# _ZN6glitch7collada24CParticleSystemSceneNode4initEv
0064fd60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0064fd64: ldr      r4, [r0, #0x158]
0064fd68: ldr      r7, [r0, #0x15c]
0064fd6c: ldr      sb, [pc, #0x444]
0064fd70: sub      sp, sp, #0x64
0064fd74: cmp      r4, r7
0064fd78: mov      r6, r0
0064fd7c: add      sb, pc, sb
0064fd80: beq      #0x64fe2c
0064fd84: ldr      r3, [pc, #0x430]
0064fd88: ldr      r1, [pc, #0x430]
0064fd8c: add      r2, r0, #0x134
0064fd90: add      r3, pc, r3
0064fd94: str      r3, [sp, #0xc]
0064fd98: ldr      r3, [pc, #0x424]
0064fd9c: str      r1, [sp, #8]
0064fda0: str      r2, [sp]
0064fda4: add      r3, pc, r3
0064fda8: str      r3, [sp, #0x10]
0064fdac: ldr      r3, [pc, #0x414]
0064fdb0: mov      fp, sb
0064fdb4: add      r3, pc, r3
0064fdb8: str      r3, [sp, #0x18]
0064fdbc: ldr      r3, [pc, #0x408]
0064fdc0: add      r3, pc, r3
0064fdc4: str      r3, [sp, #0x14]
0064fdc8: ldr      r3, [r4]
0064fdcc: mov      r2, #0
0064fdd0: mov      r1, #6
0064fdd4: ldr      r8, [r3, #0x1c]
0064fdd8: ldr      r0, [r3, #4]
0064fddc: cmp      r8, #0
0064fde0: addne    r8, r8, #4
0064fde4: bl       #0x5cef08
0064fde8: ldr      r3, [r4]
0064fdec: mov      r5, r0
0064fdf0: ldr      r3, [r3, #4]
0064fdf4: ldrh     r2, [r3, #0xe]
0064fdf8: cmp      r2, r0
0064fdfc: ldrhi    sl, [r3, #0x20]
0064fe00: ldr      r3, [r6, #0x134]
0064fe04: movls    sl, #0
0064fe08: addhi    sl, sl, r0, lsl #4
0064fe0c: ldr      r2, [r3, #0x24]
0064fe10: ldr      r2, [r2, #0x20]
0064fe14: ldr      r2, [r2, #4]
0064fe18: cmp      r2, #0
0064fe1c: beq      #0x64ff88
0064fe20: add      r4, r4, #4
0064fe24: cmp      r4, r7
0064fe28: bne      #0x64fdc8
0064fe2c: add      r4, sp, #0x58
0064fe30: mov      r0, r4
0064fe34: mov      r1, r6
0064fe38: mov      r2, #0
0064fe3c: ldr      r3, [r6]
0064fe40: mov      lr, pc
0064fe44: ldr      pc, [r3, #0x84]
0064fe48: ldr      r3, [sp, #0x58]
0064fe4c: mov      r1, #6
0064fe50: mov      r2, #0
0064fe54: ldr      r0, [r3, #4]
0064fe58: bl       #0x5cef08
0064fe5c: str      r0, [r6, #0x174]
0064fe60: mov      r0, r4
0064fe64: bl       #0x310be8
0064fe68: ldr      ip, [r6, #0x178]
0064fe6c: add      r4, sp, #0x54
0064fe70: mov      r0, r4
0064fe74: ldr      r3, [ip]
0064fe78: mov      r1, r6
0064fe7c: mov      r2, #0
0064fe80: ldr      r5, [r3, #-0xc]
0064fe84: ldr      r3, [r6]
0064fe88: add      r5, ip, r5
0064fe8c: mov      lr, pc
0064fe90: ldr      pc, [r3, #0x84]
0064fe94: ldr      r1, [pc, #0x334]
0064fe98: mov      r0, r5
0064fe9c: add      r1, pc, r1
0064fea0: bl       #0x64d0bc
0064fea4: ldr      ip, [r5, #0x34]
0064fea8: add      r1, r5, #0x30
0064feac: mov      lr, r0
0064feb0: cmp      ip, #0
0064feb4: moveq    ip, r1
0064feb8: beq      #0x64fee8
0064febc: mov      r2, r1
0064fec0: b        #0x64fec8
0064fec4: mov      ip, r3
0064fec8: ldr      r3, [ip, #0x10]
0064fecc: cmp      lr, r3
0064fed0: ldrhi    r3, [ip, #0xc]
0064fed4: ldrls    r3, [ip, #8]
0064fed8: movhi    ip, r2
0064fedc: mov      r2, ip
0064fee0: cmp      r3, #0
0064fee4: bne      #0x64fec4
0064fee8: cmp      r1, ip
0064feec: beq      #0x64ff60
0064fef0: ldr      r2, [ip, #0x10]
0064fef4: mov      r3, ip
0064fef8: cmp      lr, r2
0064fefc: blo      #0x64ff60
0064ff00: ldr      r2, [r3, #0x14]
0064ff04: cmp      r2, #0
0064ff08: beq      #0x64ff3c
0064ff0c: ldr      r3, [sp, #0x54]
0064ff10: add      r0, sp, #0x60
0064ff14: str      r3, [sp, #0x48]
0064ff18: cmp      r3, #0
0064ff1c: ldrne    r1, [r3]
0064ff20: addne    r1, r1, #1
0064ff24: strne    r1, [r3]
0064ff28: ldrne    r3, [sp, #0x48]
0064ff2c: ldr      r1, [r2]
0064ff30: str      r1, [r0, #-0x18]!
0064ff34: str      r3, [r2]
0064ff38: bl       #0x310be8
0064ff3c: mov      r0, r4
0064ff40: bl       #0x310be8
0064ff44: ldr      r3, [r6, #0x178]
0064ff48: mov      r0, r3
0064ff4c: ldr      r3, [r3]
0064ff50: mov      lr, pc
0064ff54: ldr      pc, [r3, #0xc]
0064ff58: add      sp, sp, #0x64
0064ff5c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0064ff60: add      r3, sp, #0x30
0064ff64: str      lr, [sp, #0x30]
0064ff68: add      r0, sp, #0x40
0064ff6c: mov      lr, #0
0064ff70: add      r2, sp, #0x44
0064ff74: str      lr, [sp, #0x34]
0064ff78: str      ip, [sp, #0x44]
0064ff7c: bl       #0x63aa74
0064ff80: ldr      r3, [sp, #0x40]
0064ff84: b        #0x64ff00
0064ff88: ldr      r2, [r6, #0x138]
0064ff8c: cmp      r3, #0
0064ff90: str      r3, [sp, #0x20]
0064ff94: str      r2, [sp, #0x24]
0064ff98: beq      #0x64ffb0
0064ff9c: ldr      r2, [r3, #4]
0064ffa0: cmp      r2, #0
0064ffa4: addne    r2, r2, #1
0064ffa8: strne    r2, [r3, #4]
0064ffac: ldrne    r3, [r6, #0x134]
0064ffb0: mov      r1, #0
0064ffb4: str      r1, [sp, #0x28]
0064ffb8: ldr      r3, [r3, #0x24]
0064ffbc: ldr      r0, [r3, #0x20]
0064ffc0: ldr      r3, [r0, #0x34]
0064ffc4: cmp      r3, r1
0064ffc8: addeq    r0, r0, #0x18
0064ffcc: streq    r0, [sp, #0x28]
0064ffd0: bne      #0x650170
0064ffd4: ldr      r1, [sp, #8]
0064ffd8: ldmib    r0, {r3, ip}
0064ffdc: ldr      r2, [fp, r1]
0064ffe0: bic      r3, r3, r3, asr #31
0064ffe4: cmp      r3, ip
0064ffe8: strle    r3, [sp, #0x2c]
0064ffec: strgt    ip, [sp, #0x2c]
0064fff0: add      r1, sp, #0x20
0064fff4: ldr      r0, [r2]
0064fff8: mov      r3, #0
0064fffc: add      r2, sp, #0x5c
00650000: str      r1, [sp, #4]
00650004: str      r3, [sp, #0x5c]
00650008: bl       #0x60c59c
0065000c: ldr      r2, [sp, #0x5c]
00650010: cmp      r2, #0
00650014: beq      #0x6500d0
00650018: ldr      r0, [r2, #0x14]
0065001c: ldr      r3, [r6, #0x178]
00650020: ldr      r1, [sp, #0x14]
00650024: ldr      r0, [r0, #0xc]
00650028: ldr      r2, [r3]
0065002c: str      r0, [sp, #0x1c]
00650030: ldr      sb, [r2, #-0xc]
00650034: add      sb, r3, sb
00650038: mov      r0, sb
0065003c: bl       #0x64d0bc
00650040: ldr      ip, [sb, #0x34]
00650044: add      r1, sb, #0x30
00650048: mov      lr, r0
0065004c: cmp      ip, #0
00650050: moveq    ip, r1
00650054: beq      #0x650084
00650058: mov      r2, r1
0065005c: b        #0x650064
00650060: mov      ip, r3
00650064: ldr      r3, [ip, #0x10]
00650068: cmp      lr, r3
0065006c: ldrhi    r3, [ip, #0xc]
00650070: ldrls    r3, [ip, #8]
00650074: movhi    ip, r2
00650078: mov      r2, ip
0065007c: cmp      r3, #0
00650080: bne      #0x650060
00650084: cmp      r1, ip
00650088: beq      #0x65009c
0065008c: ldr      r2, [ip, #0x10]
00650090: mov      r3, ip
00650094: cmp      lr, r2
00650098: bhs      #0x6500c0
0065009c: add      r3, sp, #0x38
006500a0: str      lr, [sp, #0x38]
006500a4: add      r0, sp, #0x4c
006500a8: mov      lr, #0
006500ac: add      r2, sp, #0x50
006500b0: str      lr, [sp, #0x3c]
006500b4: str      ip, [sp, #0x50]
006500b8: bl       #0x63aa74
006500bc: ldr      r3, [sp, #0x4c]
006500c0: ldr      r3, [r3, #0x14]
006500c4: cmp      r3, #0
006500c8: ldrne    r2, [sp, #0x1c]
006500cc: strne    r2, [r3]
006500d0: movw     r3, #0xffff
006500d4: cmp      r5, r3
006500d8: beq      #0x650180
006500dc: ldr      r3, [sl]
006500e0: mov      r2, #0x56
006500e4: ldr      r0, [sp]
006500e8: cmp      r3, #0
006500ec: addne    r3, r3, #4
006500f0: mov      r1, r8
006500f4: bl       #0x61c91c
006500f8: subs     r2, r0, #0
006500fc: beq      #0x650180
00650100: ldr      r3, [r6, #0x178]
00650104: ldr      r1, [sp, #0xc]
00650108: ldr      r0, [r3]
0065010c: ldr      r0, [r0, #-0xc]
00650110: add      r0, r3, r0
00650114: bl       #0x64fcbc
00650118: mov      r1, r8
0065011c: mov      r2, #0x100
00650120: mov      r3, #0xff
00650124: ldr      r0, [sp]
00650128: bl       #0x61c0c8
0065012c: ldr      r3, [r6, #0x178]
00650130: subs     r1, r0, #0
00650134: movne    r1, #1
00650138: strb     r1, [r6, #0x170]
0065013c: ldr      ip, [r3]
00650140: mov      r2, r0
00650144: ldr      r1, [sp, #0x10]
00650148: ldr      r0, [ip, #-0xc]
0065014c: add      r0, r3, r0
00650150: bl       #0x64fcbc
00650154: ldr      r0, [sp, #0x5c]
00650158: cmp      r0, #0
0065015c: beq      #0x650164
00650160: bl       #0x60bbd4
00650164: ldr      r0, [sp, #4]
00650168: bl       #0x619474
0065016c: b        #0x64fe20
00650170: ldr      r0, [sp]
00650174: bl       #0x60e374
00650178: str      r0, [sp, #0x28]
0065017c: b        #0x64ffd4
00650180: mov      r2, #0x19
00650184: ldr      r0, [sp]
00650188: mov      r1, r8
0065018c: mov      r3, #0xff
00650190: bl       #0x61c0c8
00650194: subs     r2, r0, #0
00650198: bne      #0x650100
0065019c: mov      r2, #0x56
006501a0: ldr      r0, [sp]
006501a4: mov      r1, r8
006501a8: ldr      r3, [sp, #0x18]
006501ac: bl       #0x61c91c
006501b0: mov      r2, r0
006501b4: b        #0x650100
006501b8: eorseq   r4, r4, r4, lsl sp
006501bc: eoreq    r5, sb, r8, lsl #7
006501c0: andeq    r0, r0, r4, ror sb
006501c4: eoreq    r5, sb, ip, lsl #7
006501c8: eoreq    r5, sb, r4, asr #6
006501cc: eoreq    r5, sb, r0, lsl r3
006501d0: eoreq    r5, sb, ip, asr #6

# _ZN6glitch2ps11PColorModelINS0_9SParticleEEC2Ev
00654690: push     {r4, r5, lr}
00654694: ldr      r3, [r1]
00654698: mov      r4, r0
0065469c: mov      r2, #0
006546a0: str      r3, [r0]
006546a4: ldr      r0, [r3, #-0xc]
006546a8: ldr      r1, [r1, #4]
006546ac: mov      r3, #0
006546b0: sub      sp, sp, #0x94
006546b4: str      r1, [r4, r0]
006546b8: ldr      r1, [r4]
006546bc: str      r3, [r4, #0xc]
006546c0: str      r2, [r4, #0x1c]
006546c4: str      r3, [r4, #4]
006546c8: str      r3, [r4, #8]
006546cc: str      r2, [r4, #0x10]
006546d0: str      r2, [r4, #0x14]
006546d4: str      r2, [r4, #0x18]
006546d8: ldr      r5, [r1, #-0xc]
006546dc: ldr      r1, [pc, #0x1f4]
006546e0: add      r5, r4, r5
006546e4: add      r1, pc, r1
006546e8: mov      r0, r5
006546ec: bl       #0x64d0bc
006546f0: add      r2, sp, #0x80
006546f4: add      r3, r4, #4
006546f8: str      r0, [sp, #0x80]
006546fc: add      r1, r5, #0x30
00654700: add      r0, sp, #0x88
00654704: str      r3, [sp, #0x84]
00654708: bl       #0x63a8ec
0065470c: ldr      r3, [r4]
00654710: ldr      r1, [pc, #0x1c4]
00654714: ldr      r5, [r3, #-0xc]
00654718: add      r1, pc, r1
0065471c: add      r5, r4, r5
00654720: mov      r0, r5
00654724: bl       #0x64d0bc
00654728: add      r2, sp, #0x70
0065472c: add      r3, r4, #8
00654730: str      r0, [sp, #0x70]
00654734: add      r1, r5, #0x30
00654738: add      r0, sp, #0x78
0065473c: str      r3, [sp, #0x74]
00654740: bl       #0x63a8ec
00654744: ldr      r3, [r4]
00654748: ldr      r1, [pc, #0x190]
0065474c: ldr      r5, [r3, #-0xc]
00654750: add      r1, pc, r1
00654754: add      r5, r4, r5
00654758: mov      r0, r5
0065475c: bl       #0x64d0bc
00654760: add      r2, sp, #0x60
00654764: add      r3, r4, #0xc
00654768: str      r0, [sp, #0x60]
0065476c: add      r1, r5, #0x30
00654770: add      r0, sp, #0x68
00654774: str      r3, [sp, #0x64]
00654778: bl       #0x63a8ec
0065477c: ldr      r3, [r4]
00654780: ldr      r1, [pc, #0x15c]
00654784: ldr      r5, [r3, #-0xc]
00654788: add      r1, pc, r1
0065478c: add      r5, r4, r5
00654790: mov      r0, r5
00654794: bl       #0x64d0bc
00654798: add      r2, sp, #0x50
0065479c: add      r3, r4, #0x10
006547a0: str      r0, [sp, #0x50]
006547a4: add      r1, r5, #0x30
006547a8: add      r0, sp, #0x58
006547ac: str      r3, [sp, #0x54]
006547b0: bl       #0x63a8ec
006547b4: ldr      r3, [r4]
006547b8: ldr      r1, [pc, #0x128]
006547bc: ldr      r5, [r3, #-0xc]
006547c0: add      r1, pc, r1
006547c4: add      r5, r4, r5
006547c8: mov      r0, r5
006547cc: bl       #0x64d0bc
006547d0: add      r2, sp, #0x40
006547d4: add      r3, r4, #0x14
006547d8: str      r0, [sp, #0x40]
006547dc: add      r1, r5, #0x30
006547e0: add      r0, sp, #0x48
006547e4: str      r3, [sp, #0x44]
006547e8: bl       #0x63a8ec
006547ec: ldr      r3, [r4]
006547f0: ldr      r1, [pc, #0xf4]
006547f4: ldr      r5, [r3, #-0xc]
006547f8: add      r1, pc, r1
006547fc: add      r5, r4, r5
00654800: mov      r0, r5
00654804: bl       #0x64d0bc
00654808: add      r2, sp, #0x30
0065480c: add      r3, r4, #0x18
00654810: str      r0, [sp, #0x30]
00654814: add      r1, r5, #0x30
00654818: add      r0, sp, #0x38
0065481c: str      r3, [sp, #0x34]
00654820: bl       #0x63a8ec
00654824: ldr      r3, [r4]
00654828: ldr      r1, [pc, #0xc0]
0065482c: ldr      r5, [r3, #-0xc]
00654830: add      r1, pc, r1
00654834: add      r5, r4, r5
00654838: mov      r0, r5
0065483c: bl       #0x64d0bc
00654840: add      r2, sp, #0x20
00654844: add      r3, r4, #0x1c
00654848: str      r0, [sp, #0x20]
0065484c: add      r1, r5, #0x30
00654850: add      r0, sp, #0x28
00654854: str      r3, [sp, #0x24]
00654858: bl       #0x63a8ec
0065485c: ldr      r3, [r4]
00654860: ldr      r1, [pc, #0x8c]
00654864: ldr      r5, [r3, #-0xc]
00654868: add      r1, pc, r1
0065486c: add      r5, r4, r5
00654870: mov      r0, r5
00654874: bl       #0x64d0bc
00654878: add      r2, sp, #0x10
0065487c: add      r3, r4, #0x20
00654880: str      r0, [sp, #0x10]
00654884: add      r1, r5, #0x30
00654888: add      r0, sp, #0x18
0065488c: str      r3, [sp, #0x14]
00654890: bl       #0x63a8ec
00654894: ldr      r3, [r4]
00654898: ldr      r1, [pc, #0x58]
0065489c: ldr      r5, [r3, #-0xc]
006548a0: add      r1, pc, r1
006548a4: add      r5, r4, r5
006548a8: mov      r0, r5
006548ac: bl       #0x64d0bc
006548b0: add      r3, r4, #0x24
006548b4: str      r0, [sp]
006548b8: add      r1, r5, #0x30
006548bc: add      r0, sp, #8
006548c0: mov      r2, sp
006548c4: str      r3, [sp, #4]
006548c8: bl       #0x63a8ec
006548cc: mov      r0, r4
006548d0: add      sp, sp, #0x94
006548d4: pop      {r4, r5, pc}
006548d8: eoreq    r0, sb, ip, asr #20
006548dc: eoreq    r0, sb, r0, lsl #20
006548e0: eoreq    r0, sb, r0, ror #25
006548e4: eoreq    r0, sb, r0, asr #25
006548e8: mlaeq    sb, r8, ip, r0
006548ec: eoreq    r0, sb, r8, ror ip
006548f0: eoreq    r0, sb, r0, asr ip
006548f4: eoreq    r0, sb, r0, lsr ip
006548f8: eoreq    r0, sb, r0, lsl ip

# _ZN6glitch2ps10PSizeModelINS0_9SParticleEEC2Ev
006543f8: push     {r4, r5, lr}
006543fc: ldr      r3, [r1]
00654400: mov      r2, #0
00654404: sub      sp, sp, #0x44
00654408: str      r3, [r0]
0065440c: ldr      r3, [r3, #-0xc]
00654410: ldr      r1, [r1, #4]
00654414: mov      r4, r0
00654418: str      r1, [r0, r3]
0065441c: ldr      r3, [r0]
00654420: mov      r1, #0x3f800000
00654424: str      r1, [r0, #4]
00654428: str      r2, [r0, #0x10]
0065442c: str      r2, [r0, #8]
00654430: str      r2, [r0, #0xc]
00654434: ldr      r5, [r3, #-0xc]
00654438: ldr      r1, [pc, #0xdc]
0065443c: add      r5, r0, r5
00654440: add      r1, pc, r1
00654444: mov      r0, r5
00654448: bl       #0x64d0bc
0065444c: add      r2, sp, #0x30
00654450: add      r3, r4, #4
00654454: str      r0, [sp, #0x30]
00654458: add      r1, r5, #0x30
0065445c: add      r0, sp, #0x38
00654460: str      r3, [sp, #0x34]
00654464: bl       #0x63a8ec
00654468: ldr      r3, [r4]
0065446c: ldr      r1, [pc, #0xac]
00654470: ldr      r5, [r3, #-0xc]
00654474: add      r1, pc, r1
00654478: add      r5, r4, r5
0065447c: mov      r0, r5
00654480: bl       #0x64d0bc
00654484: add      r2, sp, #0x20
00654488: add      r3, r4, #8
0065448c: str      r0, [sp, #0x20]
00654490: add      r1, r5, #0x30
00654494: add      r0, sp, #0x28
00654498: str      r3, [sp, #0x24]
0065449c: bl       #0x63a8ec
006544a0: ldr      r3, [r4]
006544a4: ldr      r1, [pc, #0x78]
006544a8: ldr      r5, [r3, #-0xc]
006544ac: add      r1, pc, r1
006544b0: add      r5, r4, r5
006544b4: mov      r0, r5
006544b8: bl       #0x64d0bc
006544bc: add      r2, sp, #0x10
006544c0: add      r3, r4, #0xc
006544c4: str      r0, [sp, #0x10]
006544c8: add      r1, r5, #0x30
006544cc: add      r0, sp, #0x18
006544d0: str      r3, [sp, #0x14]
006544d4: bl       #0x63a8ec
006544d8: ldr      r3, [r4]
006544dc: ldr      r1, [pc, #0x44]
006544e0: ldr      r5, [r3, #-0xc]
006544e4: add      r1, pc, r1
006544e8: add      r5, r4, r5
006544ec: mov      r0, r5
006544f0: bl       #0x64d0bc
006544f4: add      r3, r4, #0x10
006544f8: str      r0, [sp]
006544fc: add      r1, r5, #0x30
00654500: add      r0, sp, #8
00654504: mov      r2, sp
00654508: str      r3, [sp, #4]
0065450c: bl       #0x63a8ec
00654510: mov      r0, r4
00654514: add      sp, sp, #0x44
00654518: pop      {r4, r5, pc}

# _ZNK6glitch7collada16CColladaDatabase10getEmitterEPKc
0061a7f4: push     {r4, r5, r6, r7, r8, lr}
0061a7f8: ldr      r3, [r0]
0061a7fc: mov      r7, r1
0061a800: ldr      r3, [r3, #0x24]
0061a804: ldr      r3, [r3, #0x20]
0061a808: ldr      r6, [r3, #0x78]
0061a80c: cmp      r6, #0
0061a810: ble      #0x61a84c
0061a814: ldr      r4, [r3, #0x7c]
0061a818: mov      r5, #0
0061a81c: b        #0x61a82c
0061a820: cmp      r5, r6
0061a824: add      r4, r4, #0x90
0061a828: beq      #0x61a84c
0061a82c: ldr      r0, [r4]
0061a830: mov      r1, r7
0061a834: bl       #0x30e31c
0061a838: cmp      r0, #0
0061a83c: add      r5, r5, #1
0061a840: bne      #0x61a820
0061a844: mov      r0, r4
0061a848: pop      {r4, r5, r6, r7, r8, pc}
0061a84c: mov      r0, #0
0061a850: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch2ps13PEmitterModelINS0_9SParticleEEC2Ev
006548fc: push     {r4, r5, lr}
00654900: ldr      r3, [r1]
00654904: mov      r4, r0
00654908: mov      r2, #0xbf000000
0065490c: str      r3, [r0]
00654910: ldr      ip, [r1, #4]
00654914: ldr      r0, [r3, #-0xc]
00654918: sub      sp, sp, #0x5c
0065491c: mov      r3, #0x3f800000
00654920: str      ip, [r4, r0]
00654924: add      r2, r2, #0x800000
00654928: mov      r1, #0
0065492c: mov      r0, #0x5c
00654930: str      r3, [sp, #8]
00654934: str      r3, [sp]
00654938: str      r3, [sp, #4]
0065493c: str      r2, [sp, #0x14]
00654940: str      r2, [sp, #0xc]
00654944: str      r2, [sp, #0x10]
00654948: bl       #0x5341ac
0065494c: mov      r2, sp
00654950: add      r1, sp, #0xc
00654954: mov      r5, r0
00654958: bl       #0x69b074
0065495c: ldr      r2, [r4]
00654960: mov      r3, #0x40000000
00654964: mov      r1, #0
00654968: str      r1, [r4, #8]
0065496c: str      r3, [r4, #0x14]
00654970: str      r3, [r4, #0xc]
00654974: str      r3, [r4, #0x10]
00654978: str      r5, [r4, #4]
0065497c: ldr      r5, [r2, #-0xc]
00654980: ldr      r1, [pc, #0xdc]
00654984: add      r5, r4, r5
00654988: add      r1, pc, r1
0065498c: mov      r0, r5
00654990: bl       #0x64d0bc
00654994: add      r2, sp, #0x48
00654998: add      r3, r4, #8
0065499c: str      r0, [sp, #0x48]
006549a0: add      r1, r5, #0x30
006549a4: add      r0, sp, #0x50
006549a8: str      r3, [sp, #0x4c]
006549ac: bl       #0x63a8ec
006549b0: ldr      r3, [r4]
006549b4: ldr      r1, [pc, #0xac]
006549b8: ldr      r5, [r3, #-0xc]
006549bc: add      r1, pc, r1
006549c0: add      r5, r4, r5
006549c4: mov      r0, r5
006549c8: bl       #0x64d0bc
006549cc: add      r2, sp, #0x38
006549d0: add      r3, r4, #0xc
006549d4: str      r0, [sp, #0x38]
006549d8: add      r1, r5, #0x30
006549dc: add      r0, sp, #0x40
006549e0: str      r3, [sp, #0x3c]
006549e4: bl       #0x63a8ec
006549e8: ldr      r3, [r4]
006549ec: ldr      r1, [pc, #0x78]
006549f0: ldr      r5, [r3, #-0xc]
006549f4: add      r1, pc, r1
006549f8: add      r5, r4, r5
006549fc: mov      r0, r5
00654a00: bl       #0x64d0bc
00654a04: add      r2, sp, #0x28
00654a08: add      r3, r4, #0x10
00654a0c: str      r0, [sp, #0x28]
00654a10: add      r1, r5, #0x30
00654a14: add      r0, sp, #0x30
00654a18: str      r3, [sp, #0x2c]
00654a1c: bl       #0x63a8ec
00654a20: ldr      r3, [r4]
00654a24: ldr      r1, [pc, #0x44]
00654a28: ldr      r5, [r3, #-0xc]
00654a2c: add      r1, pc, r1
00654a30: add      r5, r4, r5
00654a34: mov      r0, r5
00654a38: bl       #0x64d0bc
00654a3c: add      r3, r4, #0x14
00654a40: str      r0, [sp, #0x18]
00654a44: add      r1, r5, #0x30
00654a48: add      r0, sp, #0x20
00654a4c: add      r2, sp, #0x18
00654a50: str      r3, [sp, #0x1c]
00654a54: bl       #0x63a8ec
00654a58: mov      r0, r4
00654a5c: add      sp, sp, #0x5c
00654a60: pop      {r4, r5, pc}
00654a64: eoreq    r0, sb, r0, lsl #17
00654a68: eoreq    r0, sb, ip, asr r8
00654a6c: eoreq    r6, sb, ip, ror #18
00654a70: eoreq    r6, sb, r4, asr #18

# _ZN6glitch2ps16IParticleContextINS0_9SParticleEE12setParameterIiEEvPKcT_
006501d4: push     {r4, r5, r6, lr}
006501d8: sub      sp, sp, #0x10
006501dc: mov      r6, r0
006501e0: mov      r5, r2
006501e4: bl       #0x64d0bc
006501e8: ldr      ip, [r6, #0x34]
006501ec: add      r1, r6, #0x30
006501f0: mov      r4, r0
006501f4: cmp      ip, #0
006501f8: moveq    ip, r1
006501fc: beq      #0x65022c
00650200: mov      r2, r1
00650204: b        #0x65020c
00650208: mov      ip, r3
0065020c: ldr      r3, [ip, #0x10]
00650210: cmp      r4, r3
00650214: ldrhi    r3, [ip, #0xc]
00650218: ldrls    r3, [ip, #8]
0065021c: movhi    ip, r2
00650220: mov      r2, ip
00650224: cmp      r3, #0
00650228: bne      #0x650208
0065022c: cmp      r1, ip
00650230: beq      #0x650244
00650234: ldr      r2, [ip, #0x10]
00650238: mov      r3, ip
0065023c: cmp      r4, r2
00650240: bhs      #0x650264
00650244: mov      r3, sp
00650248: mov      lr, #0
0065024c: add      r0, sp, #8
00650250: add      r2, sp, #0xc
00650254: stm      sp, {r4, lr}
00650258: str      ip, [sp, #0xc]
0065025c: bl       #0x63aa74
00650260: ldr      r3, [sp, #8]
00650264: ldr      r3, [r3, #0x14]
00650268: cmp      r3, #0
0065026c: strne    r5, [r3]
00650270: add      sp, sp, #0x10
00650274: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada24CParticleSystemSceneNode26getParticleSystemParameterEPKc
00651924: push     {r4, r5, lr}
00651928: ldr      r3, [r0, #0x178]
0065192c: sub      sp, sp, #0x14
00651930: ldr      r2, [r3]
00651934: ldr      r5, [r2, #-0xc]
00651938: add      r5, r3, r5
0065193c: mov      r0, r5
00651940: bl       #0x64d0bc
00651944: ldr      ip, [r5, #0x34]
00651948: add      r1, r5, #0x30
0065194c: mov      r4, r0
00651950: cmp      ip, #0
00651954: moveq    ip, r1
00651958: beq      #0x651988
0065195c: mov      r2, r1
00651960: b        #0x651968
00651964: mov      ip, r3
00651968: ldr      r3, [ip, #0x10]
0065196c: cmp      r4, r3
00651970: ldrhi    r3, [ip, #0xc]
00651974: ldrls    r3, [ip, #8]
00651978: movhi    ip, r2
0065197c: mov      r2, ip
00651980: cmp      r3, #0
00651984: bne      #0x651964
00651988: cmp      r1, ip
0065198c: beq      #0x6519a0
00651990: ldr      r2, [ip, #0x10]
00651994: mov      r3, ip
00651998: cmp      r4, r2
0065199c: bhs      #0x6519c0
006519a0: mov      r3, sp
006519a4: mov      lr, #0
006519a8: add      r0, sp, #8
006519ac: add      r2, sp, #0xc
006519b0: stm      sp, {r4, lr}
006519b4: str      ip, [sp, #0xc]
006519b8: bl       #0x63aa74
006519bc: ldr      r3, [sp, #8]
006519c0: ldr      r0, [r3, #0x14]
006519c4: add      sp, sp, #0x14
006519c8: pop      {r4, r5, pc}

# _ZN6glitch2ps16PGenerationModelINS0_9SParticleEEC2Ev
006545d0: push     {r4, r5, lr}
006545d4: ldr      r3, [r1]
006545d8: mov      r2, #0
006545dc: sub      sp, sp, #0x24
006545e0: str      r3, [r0]
006545e4: ldr      r3, [r3, #-0xc]
006545e8: ldr      r1, [r1, #4]
006545ec: mov      r4, r0
006545f0: str      r1, [r0, r3]
006545f4: ldr      r3, [r0]
006545f8: mov      r1, #0x3f800000
006545fc: str      r1, [r0, #4]
00654600: mov      r1, #1
00654604: str      r1, [r0, #8]
00654608: str      r2, [r0, #0x10]
0065460c: str      r2, [r0, #0xc]
00654610: ldr      r5, [r3, #-0xc]
00654614: ldr      r1, [pc, #0x6c]
00654618: add      r5, r0, r5
0065461c: add      r1, pc, r1
00654620: mov      r0, r5
00654624: bl       #0x64d0bc
00654628: mov      r2, sp
0065462c: add      r3, r4, #4
00654630: str      r0, [sp]
00654634: add      r1, r5, #0x30
00654638: add      r0, sp, #8
0065463c: str      r3, [sp, #4]
00654640: bl       #0x63a8ec
00654644: ldr      r3, [r4]
00654648: ldr      r1, [pc, #0x3c]
0065464c: ldr      r5, [r3, #-0xc]
00654650: add      r1, pc, r1
00654654: add      r5, r4, r5
00654658: mov      r0, r5
0065465c: bl       #0x64d0bc
00654660: add      r3, r4, #8
00654664: str      r0, [sp, #0x10]
00654668: add      r1, r5, #0x30
0065466c: add      r0, sp, #0x18
00654670: add      r2, sp, #0x10
00654674: str      r3, [sp, #0x14]
00654678: bl       #0x63a8ec
0065467c: mov      r0, r4
00654680: add      sp, sp, #0x24
00654684: pop      {r4, r5, pc}
00654688: mlaeq    sb, ip, sp, r0
0065468c: eoreq    r0, sb, r8, lsr #23

# _ZN6glitch7collada24CParticleSystemSceneNode18initParticleSystemEPNS_5video12IVideoDriverEb
00656450: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00656454: mov      r6, r2
00656458: sub      sp, sp, #0xa4
0065645c: mov      r4, r0
00656460: mov      r7, r1
00656464: bl       #0x63b3d0
00656468: mov      r1, r6
0065646c: bl       #0x655314
00656470: str      r0, [r4, #0x178]
00656474: ldr      r3, [r0]
00656478: ldr      r2, [r4, #0x18c]
0065647c: ldr      r1, [pc, #0xd80]
00656480: ldr      r3, [r3, #-0xc]
00656484: ldr      r2, [r2, #8]
00656488: add      r1, pc, r1
0065648c: add      r0, r0, r3
00656490: bl       #0x6501d4
00656494: ldr      r2, [r4, #0x18c]
00656498: ldr      r5, [pc, #0xd68]
0065649c: ldr      r3, [r2, #8]
006564a0: add      r5, pc, r5
006564a4: cmp      r3, #1
006564a8: beq      #0x656e4c
006564ac: cmp      r3, #2
006564b0: beq      #0x656df8
006564b4: cmp      r3, #0
006564b8: beq      #0x656cf0
006564bc: ldr      r3, [r4, #0x178]
006564c0: ldr      r1, [pc, #0xd44]
006564c4: ldr      r2, [r2, #0x18]
006564c8: ldr      r0, [r3]
006564cc: add      r1, pc, r1
006564d0: ldr      r0, [r0, #-0xc]
006564d4: add      r0, r3, r0
006564d8: bl       #0x6501d4
006564dc: ldr      r3, [r4, #0x178]
006564e0: ldr      r2, [r4, #0x18c]
006564e4: ldr      r1, [pc, #0xd24]
006564e8: ldr      r0, [r3]
006564ec: ldr      r2, [r2, #0x20]
006564f0: add      r1, pc, r1
006564f4: ldr      r0, [r0, #-0xc]
006564f8: add      r0, r3, r0
006564fc: bl       #0x650400
00656500: ldr      r3, [r4, #0x178]
00656504: ldr      r2, [r4, #0x18c]
00656508: ldr      r1, [pc, #0xd04]
0065650c: ldr      r0, [r3]
00656510: ldr      r2, [r2, #0x28]
00656514: add      r1, pc, r1
00656518: ldr      r0, [r0, #-0xc]
0065651c: add      r0, r3, r0
00656520: bl       #0x650400
00656524: ldr      r3, [r4, #0x178]
00656528: ldr      r2, [r4, #0x18c]
0065652c: ldr      r1, [pc, #0xce4]
00656530: ldr      r0, [r3]
00656534: ldr      r2, [r2, #0x2c]
00656538: add      r1, pc, r1
0065653c: ldr      r0, [r0, #-0xc]
00656540: add      r0, r3, r0
00656544: bl       #0x650400
00656548: ldr      r3, [r4, #0x178]
0065654c: ldr      r2, [r4, #0x18c]
00656550: ldr      r1, [pc, #0xcc4]
00656554: ldr      r0, [r3]
00656558: ldr      r2, [r2, #0x30]
0065655c: add      r1, pc, r1
00656560: ldr      r0, [r0, #-0xc]
00656564: add      r0, r3, r0
00656568: bl       #0x650400
0065656c: ldr      r3, [r4, #0x178]
00656570: ldr      r2, [r4, #0x18c]
00656574: ldr      r1, [pc, #0xca4]
00656578: ldr      r0, [r3]
0065657c: ldr      r2, [r2, #0x34]
00656580: add      r1, pc, r1
00656584: ldr      r0, [r0, #-0xc]
00656588: add      r0, r3, r0
0065658c: bl       #0x650400
00656590: ldr      r3, [r4, #0x178]
00656594: ldr      r2, [r4, #0x18c]
00656598: ldr      r1, [pc, #0xc84]
0065659c: ldr      r0, [r3]
006565a0: ldr      r2, [r2, #0x38]
006565a4: add      r1, pc, r1
006565a8: ldr      r0, [r0, #-0xc]
006565ac: add      r0, r3, r0
006565b0: bl       #0x650400
006565b4: ldr      r3, [r4, #0x178]
006565b8: ldr      r2, [r4, #0x18c]
006565bc: ldr      r1, [pc, #0xc64]
006565c0: ldr      r0, [r3]
006565c4: ldr      r2, [r2, #0x3c]
006565c8: add      r1, pc, r1
006565cc: ldr      r0, [r0, #-0xc]
006565d0: add      r0, r3, r0
006565d4: bl       #0x650400
006565d8: ldr      r3, [r4, #0x178]
006565dc: ldr      r2, [r4, #0x18c]
006565e0: ldr      r1, [pc, #0xc44]
006565e4: ldr      r0, [r3]
006565e8: ldr      r2, [r2, #0x40]
006565ec: add      r1, pc, r1
006565f0: ldr      r0, [r0, #-0xc]
006565f4: add      r0, r3, r0
006565f8: bl       #0x650400
006565fc: ldr      r3, [r4, #0x178]
00656600: ldr      r2, [r4, #0x18c]
00656604: ldr      r1, [pc, #0xc24]
00656608: ldr      r0, [r3]
0065660c: ldr      r2, [r2, #0x44]
00656610: add      r1, pc, r1
00656614: ldr      r0, [r0, #-0xc]
00656618: add      r0, r3, r0
0065661c: bl       #0x650400
00656620: ldr      r2, [r4, #0x18c]
00656624: ldr      r3, [r2, #0x48]
00656628: cmp      r3, #1
0065662c: beq      #0x656d6c
00656630: cmp      r3, #2
00656634: beq      #0x656dcc
00656638: cmp      r3, #0
0065663c: beq      #0x656cb8
00656640: ldr      r3, [r4, #0x178]
00656644: ldr      r8, [r2, #0x64]
00656648: ldr      r1, [pc, #0xbe4]
0065664c: ldr      r2, [r3]
00656650: add      r1, pc, r1
00656654: ldr      sl, [r2, #-0xc]
00656658: add      sl, r3, sl
0065665c: mov      r0, sl
00656660: bl       #0x64d0bc
00656664: ldr      ip, [sl, #0x34]
00656668: add      r1, sl, #0x30
0065666c: mov      lr, r0
00656670: cmp      ip, #0
00656674: moveq    ip, r1
00656678: beq      #0x6566a8
0065667c: mov      r2, r1
00656680: b        #0x656688
00656684: mov      ip, r3
00656688: ldr      r3, [ip, #0x10]
0065668c: cmp      lr, r3
00656690: ldrhi    r3, [ip, #0xc]
00656694: ldrls    r3, [ip, #8]
00656698: movhi    ip, r2
0065669c: mov      r2, ip
006566a0: cmp      r3, #0
006566a4: bne      #0x656684
006566a8: cmp      r1, ip
006566ac: beq      #0x656ac0
006566b0: ldr      r2, [ip, #0x10]
006566b4: mov      r3, ip
006566b8: cmp      lr, r2
006566bc: blo      #0x656ac0
006566c0: ldr      r3, [r3, #0x14]
006566c4: ldr      r1, [pc, #0xb6c]
006566c8: cmp      r3, #0
006566cc: strne    r8, [r3]
006566d0: ldr      r3, [r4, #0x178]
006566d4: ldr      r2, [r4, #0x18c]
006566d8: add      r1, pc, r1
006566dc: ldr      r0, [r3]
006566e0: ldr      r2, [r2, #0x5c]
006566e4: ldr      r0, [r0, #-0xc]
006566e8: add      r0, r3, r0
006566ec: bl       #0x650400
006566f0: ldr      r3, [r4, #0x178]
006566f4: ldr      r2, [r4, #0x18c]
006566f8: ldr      r1, [pc, #0xb3c]
006566fc: ldr      r0, [r3]
00656700: ldr      r2, [r2, #0x60]
00656704: add      r1, pc, r1
00656708: ldr      r0, [r0, #-0xc]
0065670c: add      r0, r3, r0
00656710: bl       #0x650400
00656714: ldr      r3, [r4, #0x178]
00656718: ldr      r2, [r4, #0x18c]
0065671c: ldr      r1, [pc, #0xb1c]
00656720: ldr      r0, [r3]
00656724: ldr      r2, [r2, #0x68]
00656728: add      r1, pc, r1
0065672c: ldr      r0, [r0, #-0xc]
00656730: add      r0, r3, r0
00656734: bl       #0x650400
00656738: ldr      r3, [r4, #0x178]
0065673c: ldr      r2, [r4, #0x18c]
00656740: ldr      r1, [pc, #0xafc]
00656744: ldr      r0, [r3]
00656748: ldr      r2, [r2, #0x6c]
0065674c: add      r1, pc, r1
00656750: ldr      r0, [r0, #-0xc]
00656754: add      r0, r3, r0
00656758: bl       #0x650400
0065675c: ldr      r3, [r4, #0x178]
00656760: ldr      r2, [r4, #0x18c]
00656764: ldr      r1, [pc, #0xadc]
00656768: ldr      r0, [r3]
0065676c: ldr      r2, [r2, #0x70]
00656770: add      r1, pc, r1
00656774: ldr      r0, [r0, #-0xc]
00656778: add      r0, r3, r0
0065677c: bl       #0x650400
00656780: ldr      r3, [r4, #0x178]
00656784: ldr      r2, [r4, #0x18c]
00656788: ldr      r1, [pc, #0xabc]
0065678c: ldr      r0, [r3]
00656790: ldr      r2, [r2, #0x74]
00656794: add      r1, pc, r1
00656798: ldr      r0, [r0, #-0xc]
0065679c: add      r0, r3, r0
006567a0: bl       #0x650400
006567a4: ldr      r3, [r4, #0x178]
006567a8: ldr      r2, [r4, #0x18c]
006567ac: ldr      r1, [pc, #0xa9c]
006567b0: ldr      r0, [r3]
006567b4: ldr      r2, [r2, #0x78]
006567b8: add      r1, pc, r1
006567bc: ldr      r0, [r0, #-0xc]
006567c0: add      r0, r3, r0
006567c4: bl       #0x650400
006567c8: ldr      r3, [r4, #0x178]
006567cc: ldr      r2, [r4, #0x18c]
006567d0: ldr      r1, [pc, #0xa7c]
006567d4: ldr      r0, [r3]
006567d8: ldr      r2, [r2, #0x7c]
006567dc: add      r1, pc, r1
006567e0: ldr      r0, [r0, #-0xc]
006567e4: add      r0, r3, r0
006567e8: bl       #0x650400
006567ec: ldr      r3, [r4, #0x178]
006567f0: ldr      r2, [r4, #0x18c]
006567f4: ldr      r1, [pc, #0xa5c]
006567f8: ldr      r0, [r3]
006567fc: ldr      r2, [r2, #0x80]
00656800: add      r1, pc, r1
00656804: ldr      r0, [r0, #-0xc]
00656808: add      r0, r3, r0
0065680c: bl       #0x650400
00656810: ldr      r3, [r4, #0x178]
00656814: ldr      r2, [r4, #0x18c]
00656818: ldr      r1, [pc, #0xa3c]
0065681c: ldr      r0, [r3]
00656820: ldr      r2, [r2, #0x84]
00656824: add      r1, pc, r1
00656828: ldr      r0, [r0, #-0xc]
0065682c: add      r0, r3, r0
00656830: bl       #0x650400
00656834: ldr      r3, [r4, #0x178]
00656838: ldr      r2, [r4, #0x18c]
0065683c: ldr      r1, [pc, #0xa1c]
00656840: ldr      r0, [r3]
00656844: ldr      r2, [r2, #0x88]
00656848: add      r1, pc, r1
0065684c: ldr      r0, [r0, #-0xc]
00656850: add      r0, r3, r0
00656854: bl       #0x6501d4
00656858: ldr      r3, [r4, #0x18c]
0065685c: ldr      r2, [r3, #0x88]
00656860: cmp      r2, #1
00656864: beq      #0x656eb0
00656868: ldr      r3, [r4, #0x178]
0065686c: ldr      r1, [pc, #0x9f0]
00656870: mov      r8, #0
00656874: ldr      r0, [r3]
00656878: add      r1, pc, r1
0065687c: add      r2, sp, #0x44
00656880: ldr      r0, [r0, #-0xc]
00656884: str      r8, [sp, #0x44]
00656888: str      r8, [sp, #0x48]
0065688c: add      r0, r3, r0
00656890: str      r8, [sp, #0x4c]
00656894: bl       #0x650340
00656898: ldr      r3, [r4, #0x178]
0065689c: ldr      r1, [pc, #0x9c4]
006568a0: mov      r2, r8
006568a4: ldr      r0, [r3]
006568a8: add      r1, pc, r1
006568ac: ldr      r0, [r0, #-0xc]
006568b0: add      r0, r3, r0
006568b4: bl       #0x650400
006568b8: cmp      r6, #0
006568bc: strb     r6, [r4, #0x17c]
006568c0: str      r7, [r4, #0x180]
006568c4: beq      #0x656b44
006568c8: ldr      r6, [pc, #0x99c]
006568cc: ldr      r3, [r5, r6]
006568d0: ldr      r3, [r3]
006568d4: cmp      r3, #0
006568d8: beq      #0x656f2c
006568dc: ldr      r2, [r3, #4]
006568e0: add      r2, r2, #1
006568e4: str      r2, [r3, #4]
006568e8: ldr      r0, [r4, #0x140]
006568ec: str      r3, [r4, #0x140]
006568f0: cmp      r0, #0
006568f4: beq      #0x6568fc
006568f8: bl       #0x31d584
006568fc: ldr      r7, [pc, #0x96c]
00656900: ldr      r2, [r4, #0x18c]
00656904: ldr      r3, [r5, r7]
00656908: ldr      r2, [r2, #0x18]
0065690c: ldr      r3, [r3]
00656910: cmp      r2, r3
00656914: ble      #0x656ae8
00656918: ldr      r1, [pc, #0x954]
0065691c: ldr      r3, [r5, r1]
00656920: str      r1, [sp, #0x18]
00656924: ldr      r3, [r3]
00656928: cmp      r3, #0
0065692c: beq      #0x65718c
00656930: ldr      r8, [r5, r7]
00656934: mov      r1, #1
00656938: str      r2, [r8]
0065693c: ldr      r2, [r4, #0x140]
00656940: str      r2, [sp, #0x1c]
00656944: ldr      r0, [r2, #0x18]
00656948: bl       #0x5a1adc
0065694c: ldr      r3, [sp, #0x18]
00656950: ldr      r1, [sp, #0x1c]
00656954: ldr      r8, [r8]
00656958: ldr      r2, [r5, r3]
0065695c: ldr      r6, [r1, #0x1c]
00656960: ldr      r3, [r4, #0x140]
00656964: ldr      r2, [r2]
00656968: add      r6, r0, r6
0065696c: cmp      r2, #0
00656970: str      r2, [sp, #0x14]
00656974: movne    r1, r2
00656978: ldrne    r2, [r1, #4]
0065697c: ldr      r3, [r3, #0x20]
00656980: addne    r2, r2, #1
00656984: strne    r2, [r1, #4]
00656988: ldr      r2, [sp, #0x14]
0065698c: lsl      r3, r3, #1
00656990: mul      r8, r8, r3
00656994: ldr      r3, [r2, #0xc]
00656998: cmp      r8, r3
0065699c: bhi      #0x656f0c
006569a0: ldr      r0, [sp, #0x14]
006569a4: mov      r1, #4
006569a8: bl       #0x5a19f0
006569ac: ldr      fp, [r5, r7]
006569b0: ldr      r3, [fp]
006569b4: cmp      r3, #0
006569b8: ble      #0x656a58
006569bc: mov      lr, #0
006569c0: add      r3, r6, #0xa
006569c4: str      r5, [sp, #0x24]
006569c8: add      sb, r6, #2
006569cc: add      sl, r6, #4
006569d0: add      r8, r6, #6
006569d4: add      r7, r6, #8
006569d8: mov      ip, lr
006569dc: mov      r1, lr
006569e0: str      r4, [sp, #0x20]
006569e4: mov      r5, r3
006569e8: ldrh     r4, [r6]
006569ec: uxth     r2, r1
006569f0: mov      r3, r0
006569f4: add      r4, r2, r4
006569f8: strh     r4, [r3, lr]!
006569fc: ldrh     r4, [sb]
00656a00: add      ip, ip, #1
00656a04: add      r1, r1, #4
00656a08: add      r4, r2, r4
00656a0c: strh     r4, [r3, #2]
00656a10: ldrh     r4, [sl]
00656a14: add      lr, lr, #0xc
00656a18: add      r4, r2, r4
00656a1c: strh     r4, [r3, #4]
00656a20: ldrh     r4, [r8]
00656a24: add      r4, r2, r4
00656a28: strh     r4, [r3, #6]
00656a2c: ldrh     r4, [r7]
00656a30: add      r4, r2, r4
00656a34: strh     r4, [r3, #8]
00656a38: ldrh     r4, [r5]
00656a3c: add      r2, r2, r4
00656a40: strh     r2, [r3, #0xa]
00656a44: ldr      r3, [fp]
00656a48: cmp      r3, ip
00656a4c: bgt      #0x6569e8
00656a50: ldr      r4, [sp, #0x20]
00656a54: ldr      r5, [sp, #0x24]
00656a58: cmp      r0, #0
00656a5c: beq      #0x656a84
00656a60: ldr      r1, [sp, #0x14]
00656a64: ldrb     r3, [r1, #0x13]
00656a68: and      r2, r3, #0x1f
00656a6c: cmp      r2, #1
00656a70: bls      #0x656e78
00656a74: sub      r2, r2, #1
00656a78: bic      r3, r3, #0x1f
00656a7c: orr      r3, r2, r3
00656a80: strb     r3, [r1, #0x13]
00656a84: cmp      r6, #0
00656a88: beq      #0x656ab4
00656a8c: ldr      r2, [sp, #0x1c]
00656a90: ldr      r6, [r2, #0x18]
00656a94: ldrb     r3, [r6, #0x13]
00656a98: and      r2, r3, #0x1f
00656a9c: cmp      r2, #1
00656aa0: bls      #0x656e98
00656aa4: sub      r2, r2, #1
00656aa8: bic      r3, r3, #0x1f
00656aac: orr      r3, r2, r3
00656ab0: strb     r3, [r6, #0x13]
00656ab4: ldr      r0, [sp, #0x14]
00656ab8: bl       #0x31d584
00656abc: b        #0x656af0
00656ac0: add      r3, sp, #0x6c
00656ac4: str      lr, [sp, #0x6c]
00656ac8: add      r0, sp, #0x84
00656acc: mov      lr, #0
00656ad0: add      r2, sp, #0x88
00656ad4: str      lr, [sp, #0x70]
00656ad8: str      ip, [sp, #0x88]
00656adc: bl       #0x63aa74
00656ae0: ldr      r3, [sp, #0x84]
00656ae4: b        #0x6566c0
00656ae8: ldr      r3, [pc, #0x784]
00656aec: str      r3, [sp, #0x18]
00656af0: ldr      r1, [sp, #0x18]
00656af4: ldr      r2, [r4, #0x178]
00656af8: ldr      r3, [r5, r1]
00656afc: ldr      r1, [r2]
00656b00: ldr      r3, [r3]
00656b04: ldr      r0, [r1, #-0xc]
00656b08: add      r1, sp, #0x90
00656b0c: cmp      r3, #0
00656b10: str      r3, [sp, #0x90]
00656b14: add      r0, r2, r0
00656b18: ldrne    r2, [r3, #4]
00656b1c: addne    r2, r2, #1
00656b20: strne    r2, [r3, #4]
00656b24: bl       #0x6504a4
00656b28: ldr      r0, [sp, #0x90]
00656b2c: cmp      r0, #0
00656b30: beq      #0x656b38
00656b34: bl       #0x31d584
00656b38: mov      r3, #0x60000
00656b3c: add      r3, r3, #3
00656b40: str      r3, [r4, #0x184]
00656b44: ldr      r3, [r4, #0x178]
00656b48: ldr      r1, [r4, #0x140]
00656b4c: ldr      r2, [r3]
00656b50: ldr      r0, [r2, #-0xc]
00656b54: add      r0, r3, r0
00656b58: bl       #0x650574
00656b5c: ldr      r3, [r4, #0x178]
00656b60: ldr      r1, [pc, #0x710]
00656b64: ldr      r2, [r3]
00656b68: add      r1, pc, r1
00656b6c: ldr      r5, [r2, #-0xc]
00656b70: add      r5, r3, r5
00656b74: mov      r0, r5
00656b78: bl       #0x64d0bc
00656b7c: ldr      ip, [r5, #0x34]
00656b80: add      r1, r5, #0x30
00656b84: mov      lr, r0
00656b88: cmp      ip, #0
00656b8c: moveq    ip, r1
00656b90: beq      #0x656bc0
00656b94: mov      r2, r1
00656b98: b        #0x656ba0
00656b9c: mov      ip, r3
00656ba0: ldr      r3, [ip, #0x10]
00656ba4: cmp      lr, r3
00656ba8: ldrhi    r3, [ip, #0xc]
00656bac: ldrls    r3, [ip, #8]
00656bb0: movhi    ip, r2
00656bb4: mov      r2, ip
00656bb8: cmp      r3, #0
00656bbc: bne      #0x656b9c
00656bc0: cmp      r1, ip
00656bc4: beq      #0x656c90
00656bc8: ldr      r2, [ip, #0x10]
00656bcc: mov      r3, ip
00656bd0: cmp      lr, r2
00656bd4: blo      #0x656c90
00656bd8: ldr      r2, [r3, #0x14]
00656bdc: ldr      r3, [r4, #0x178]
00656be0: ldr      r1, [pc, #0x694]
00656be4: str      r2, [r4, #0x148]
00656be8: ldr      r2, [r3]
00656bec: add      r1, pc, r1
00656bf0: ldr      r6, [r2, #-0xc]
00656bf4: add      r6, r3, r6
00656bf8: mov      r0, r6
00656bfc: bl       #0x64d0bc
00656c00: ldr      ip, [r6, #0x34]
00656c04: add      r1, r6, #0x30
00656c08: mov      r5, r0
00656c0c: cmp      ip, #0
00656c10: moveq    ip, r1
00656c14: beq      #0x656c44
00656c18: mov      r2, r1
00656c1c: b        #0x656c24
00656c20: mov      ip, r3
00656c24: ldr      r3, [ip, #0x10]
00656c28: cmp      r5, r3
00656c2c: ldrhi    r3, [ip, #0xc]
00656c30: ldrls    r3, [ip, #8]
00656c34: movhi    ip, r2
00656c38: mov      r2, ip
00656c3c: cmp      r3, #0
00656c40: bne      #0x656c20
00656c44: cmp      r1, ip
00656c48: beq      #0x656c5c
00656c4c: ldr      r2, [ip, #0x10]
00656c50: mov      r3, ip
00656c54: cmp      r5, r2
00656c58: bhs      #0x656c80
00656c5c: add      r3, sp, #0x5c
00656c60: mov      lr, #0
00656c64: add      r0, sp, #0x74
00656c68: add      r2, sp, #0x78
00656c6c: str      r5, [sp, #0x5c]
00656c70: str      lr, [sp, #0x60]
00656c74: str      ip, [sp, #0x78]
00656c78: bl       #0x63aa74
00656c7c: ldr      r3, [sp, #0x74]
00656c80: ldr      r3, [r3, #0x14]
00656c84: str      r3, [r4, #0x14c]
00656c88: add      sp, sp, #0xa4
00656c8c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00656c90: add      r3, sp, #0x64
00656c94: str      lr, [sp, #0x64]
00656c98: add      r0, sp, #0x7c
00656c9c: mov      lr, #0
00656ca0: add      r2, sp, #0x80
00656ca4: str      lr, [sp, #0x68]
00656ca8: str      ip, [sp, #0x80]
00656cac: bl       #0x63aa74
00656cb0: ldr      r3, [sp, #0x7c]
00656cb4: b        #0x656bd8
00656cb8: ldr      r0, [r4, #0x178]
00656cbc: ldr      r1, [pc, #0x5bc]
00656cc0: mov      r3, #0
00656cc4: ldr      ip, [r0]
00656cc8: add      r2, sp, #0x50
00656ccc: add      r1, pc, r1
00656cd0: ldr      ip, [ip, #-0xc]
00656cd4: str      r3, [sp, #0x58]
00656cd8: str      r3, [sp, #0x50]
00656cdc: add      r0, r0, ip
00656ce0: str      r3, [sp, #0x54]
00656ce4: bl       #0x650340
00656ce8: ldr      r2, [r4, #0x18c]
00656cec: b        #0x656640
00656cf0: ldr      r3, [r4, #0x178]
00656cf4: ldr      r2, [r2, #0xc]
00656cf8: ldr      r1, [pc, #0x584]
00656cfc: ldr      r0, [r3]
00656d00: ldr      r2, [r2]
00656d04: add      r1, pc, r1
00656d08: ldr      r0, [r0, #-0xc]
00656d0c: add      r0, r3, r0
00656d10: bl       #0x650400
00656d14: ldr      r3, [r4, #0x178]
00656d18: ldr      r2, [r4, #0x18c]
00656d1c: ldr      r1, [pc, #0x564]
00656d20: ldr      r0, [r3]
00656d24: ldr      r2, [r2, #0xc]
00656d28: add      r1, pc, r1
00656d2c: ldr      r0, [r0, #-0xc]
00656d30: ldr      r2, [r2, #4]
00656d34: add      r0, r3, r0
00656d38: bl       #0x650400
00656d3c: ldr      r3, [r4, #0x178]
00656d40: ldr      r2, [r4, #0x18c]
00656d44: ldr      r1, [pc, #0x540]
00656d48: ldr      r0, [r3]
00656d4c: ldr      r2, [r2, #0xc]
00656d50: add      r1, pc, r1
00656d54: ldr      r0, [r0, #-0xc]
00656d58: ldr      r2, [r2, #8]
00656d5c: add      r0, r3, r0
00656d60: bl       #0x650400
00656d64: ldr      r2, [r4, #0x18c]
00656d68: b        #0x6564bc
00656d6c: ldr      r0, [r4, #0x178]
00656d70: ldr      r2, [r2, #0x4c]
00656d74: ldr      r1, [pc, #0x514]
00656d78: ldr      ip, [r0]
00656d7c: ldr      r3, [r2, #8]
00656d80: add      r1, pc, r1
00656d84: ldr      ip, [ip, #-0xc]
00656d88: str      r3, [sp]
00656d8c: ldr      r3, [r2, #4]
00656d90: add      r0, r0, ip
00656d94: ldr      r2, [r2]
00656d98: bl       #0x650278
00656d9c: ldr      r3, [r4, #0x178]
00656da0: ldr      r2, [r4, #0x18c]
00656da4: ldr      r1, [pc, #0x4e8]
00656da8: ldr      r0, [r3]
00656dac: ldr      r2, [r2, #0x4c]
00656db0: add      r1, pc, r1
00656db4: ldr      r0, [r0, #-0xc]
00656db8: ldr      r2, [r2, #0xc]
00656dbc: add      r0, r3, r0
00656dc0: bl       #0x650400
00656dc4: ldr      r2, [r4, #0x18c]
00656dc8: b        #0x656640
00656dcc: ldr      r3, [r4, #0x178]
00656dd0: ldr      r2, [r2, #0x4c]
00656dd4: ldr      r1, [pc, #0x4bc]
00656dd8: ldr      r0, [r3]
00656ddc: ldr      r2, [r2, #4]
00656de0: add      r1, pc, r1
00656de4: ldr      r0, [r0, #-0xc]
00656de8: add      r0, r3, r0
00656dec: bl       #0x650400
00656df0: ldr      r2, [r4, #0x18c]
00656df4: b        #0x656640
00656df8: ldr      r3, [r4, #0x178]
00656dfc: ldr      r2, [r2, #0xc]
00656e00: ldr      r1, [pc, #0x494]
00656e04: ldr      r0, [r3]
00656e08: ldr      r2, [r2]
00656e0c: add      r1, pc, r1
00656e10: ldr      r0, [r0, #-0xc]
00656e14: add      r0, r3, r0
00656e18: bl       #0x650400
00656e1c: ldr      r3, [r4, #0x178]
00656e20: ldr      r2, [r4, #0x18c]
00656e24: ldr      r1, [pc, #0x474]
00656e28: ldr      r0, [r3]
00656e2c: ldr      r2, [r2, #0xc]
00656e30: add      r1, pc, r1
00656e34: ldr      r0, [r0, #-0xc]
00656e38: ldr      r2, [r2, #4]
00656e3c: add      r0, r3, r0
00656e40: bl       #0x650400
00656e44: ldr      r2, [r4, #0x18c]
00656e48: b        #0x6564bc
00656e4c: ldr      r3, [r4, #0x178]
00656e50: ldr      r2, [r2, #0xc]
00656e54: ldr      r1, [pc, #0x448]
00656e58: ldr      r0, [r3]
00656e5c: ldr      r2, [r2]
00656e60: add      r1, pc, r1
00656e64: ldr      r0, [r0, #-0xc]
00656e68: add      r0, r3, r0
00656e6c: bl       #0x650400
00656e70: ldr      r2, [r4, #0x18c]
00656e74: b        #0x6564bc
00656e78: ldr      r2, [sp, #0x14]
00656e7c: ldrb     r3, [r2, #0x12]
00656e80: tst      r3, #0x20
00656e84: bne      #0x657178
00656e88: ldr      r1, [sp, #0x14]
00656e8c: mov      r3, #0
00656e90: strb     r3, [r1, #0x13]
00656e94: b        #0x656a84
00656e98: ldrb     r3, [r6, #0x12]
00656e9c: tst      r3, #0x20
00656ea0: bne      #0x657164
00656ea4: mov      r3, #0
00656ea8: strb     r3, [r6, #0x13]
00656eac: b        #0x656ab4
00656eb0: ldr      r0, [r4, #0x178]
00656eb4: ldr      r2, [r3, #0x8c]
00656eb8: ldr      r1, [pc, #0x3e8]
00656ebc: ldr      ip, [r0]
00656ec0: ldr      r3, [r2, #8]
00656ec4: add      r1, pc, r1
00656ec8: ldr      ip, [ip, #-0xc]
00656ecc: str      r3, [sp]
00656ed0: ldr      r3, [r2, #4]
00656ed4: add      r0, r0, ip
00656ed8: ldr      r2, [r2]
00656edc: bl       #0x650278
00656ee0: ldr      r3, [r4, #0x178]
00656ee4: ldr      r2, [r4, #0x18c]
00656ee8: ldr      r1, [pc, #0x3bc]
00656eec: ldr      r0, [r3]
00656ef0: ldr      r2, [r2, #0x8c]
00656ef4: add      r1, pc, r1
00656ef8: ldr      r0, [r0, #-0xc]
00656efc: ldr      r2, [r2, #0xc]
00656f00: add      r0, r3, r0
00656f04: bl       #0x650400
00656f08: b        #0x6568b8
00656f0c: mov      r0, r8
00656f10: bl       #0x64d290
00656f14: mov      r1, r8
00656f18: mov      r2, r0
00656f1c: mov      r3, #1
00656f20: ldr      r0, [sp, #0x14]
00656f24: bl       #0x5a1cb4
00656f28: b        #0x6569a0
00656f2c: ldr      r2, [pc, #0x37c]
00656f30: mov      r1, #0xc
00656f34: str      r1, [sp]
00656f38: add      r2, pc, r2
00656f3c: stmib    sp, {r2, r3}
00656f40: add      fp, sp, #0x9c
00656f44: ldr      ip, [r7]
00656f48: mov      r2, #1
00656f4c: mov      r1, r7
00656f50: mov      r0, fp
00656f54: mov      r3, #4
00656f58: mov      lr, pc
00656f5c: ldr      pc, [ip, #0x78]
00656f60: ldr      r3, [sp, #0x9c]
00656f64: mov      r8, #0
00656f68: mov      r1, r8
00656f6c: cmp      r3, #0
00656f70: str      r3, [sp, #0x2c]
00656f74: ldrne    r2, [r3, #4]
00656f78: mov      r0, #0x38
00656f7c: add      sb, sp, #0x98
00656f80: addne    r2, r2, #1
00656f84: strne    r2, [r3, #4]
00656f88: mov      r2, #4
00656f8c: mov      r3, #6
00656f90: str      r2, [sp, #0x3c]
00656f94: mov      r2, #1
00656f98: strh     r2, [sp, #0x40]
00656f9c: strh     r3, [sp, #0x42]
00656fa0: str      r8, [sp, #0x30]
00656fa4: str      r3, [sp, #0x34]
00656fa8: str      r8, [sp, #0x38]
00656fac: bl       #0x5341ac
00656fb0: ldr      r3, [pc, #0x2fc]
00656fb4: mov      r1, #0x60000
00656fb8: mov      sl, r0
00656fbc: ldr      r3, [r5, r3]
00656fc0: str      r8, [r0, #0x10]
00656fc4: str      r8, [r0, #4]
00656fc8: add      r3, r3, #8
00656fcc: str      r8, [r0, #8]
00656fd0: str      r3, [r0]
00656fd4: str      r8, [r0, #0xc]
00656fd8: add      r1, r1, #3
00656fdc: add      r0, r0, #0x14
00656fe0: bl       #0x5a135c
00656fe4: ldr      r3, [sp, #0x2c]
00656fe8: ldr      r6, [r5, r6]
00656fec: add      r0, sp, #0xa0
00656ff0: cmp      r3, r8
00656ff4: str      r3, [sl, #0x18]
00656ff8: ldrne    r2, [r3, #4]
00656ffc: mov      r8, #0
00657000: addne    r2, r2, #1
00657004: strne    r2, [r3, #4]
00657008: ldr      r3, [sp, #0x30]
0065700c: str      r3, [sl, #0x1c]
00657010: ldr      r3, [sp, #0x34]
00657014: str      r3, [sl, #0x20]
00657018: ldr      r3, [sp, #0x38]
0065701c: str      r3, [sl, #0x24]
00657020: ldr      r3, [sp, #0x3c]
00657024: str      r3, [sl, #0x28]
00657028: ldrh     r3, [sp, #0x40]
0065702c: strh     r3, [sl, #0x2c]
00657030: ldrh     r1, [sp, #0x42]
00657034: str      r8, [sl, #0x30]
00657038: strb     r8, [sl, #0x34]
0065703c: strh     r1, [sl, #0x2e]
00657040: str      sl, [sp, #0x8c]
00657044: ldr      r3, [sl, #4]
00657048: add      r3, r3, #1
0065704c: str      r3, [sl, #4]
00657050: ldr      r2, [r6]
00657054: ldr      r3, [sp, #0x8c]
00657058: str      r2, [r0, #-0x14]!
0065705c: str      r3, [r6]
00657060: bl       #0x637bac
00657064: add      r0, sp, #0x2c
00657068: bl       #0x637b8c
0065706c: mov      r0, fp
00657070: bl       #0x637b8c
00657074: ldr      r2, [r6]
00657078: mov      r3, #4
0065707c: mov      r1, r7
00657080: ldr      sl, [r2, #0x14]
00657084: mov      r2, #1
00657088: str      r8, [sp]
0065708c: str      r8, [sp, #4]
00657090: str      r2, [sp, #8]
00657094: ldr      ip, [r7]
00657098: mov      r2, r8
0065709c: mov      r0, sb
006570a0: mov      lr, pc
006570a4: ldr      pc, [ip, #0x78]
006570a8: mvn      r2, #0
006570ac: mov      r1, sb
006570b0: mov      r0, sl
006570b4: bl       #0x5a15c0
006570b8: lsl      r7, r0, #2
006570bc: mov      r1, r8
006570c0: mov      r0, r7
006570c4: ldr      r8, [sp, #0x98]
006570c8: bl       #0x5341a8
006570cc: mov      r1, r7
006570d0: mov      r2, r0
006570d4: mov      r3, #1
006570d8: mov      r0, r8
006570dc: bl       #0x5a1cb4
006570e0: mov      r1, #4
006570e4: ldr      r0, [sl, #0x24]
006570e8: bl       #0x5a19f0
006570ec: add      r7, sl, #0x24
006570f0: ldr      ip, [r7, #4]
006570f4: mov      r2, #0
006570f8: mov      r1, #0x3f800000
006570fc: add      r3, r0, ip
00657100: str      r2, [r0, ip]
00657104: str      r2, [r3, #4]
00657108: ldrh     r0, [r7, #0xe]
0065710c: add      ip, r3, r0
00657110: str      r2, [r3, r0]
00657114: str      r1, [ip, #4]
00657118: ldrh     r0, [r7, #0xe]
0065711c: add      ip, r3, r0, lsl #1
00657120: str      r1, [r3, r0, lsl #1]
00657124: str      r1, [ip, #4]
00657128: ldrh     r0, [r7, #0xe]
0065712c: add      r0, r0, r0, lsl #1
00657130: add      ip, r3, r0
00657134: str      r1, [r3, r0]
00657138: str      r2, [ip, #4]
0065713c: mov      r3, #4
00657140: str      r3, [sl, #8]
00657144: ldr      r0, [sl, #0x24]
00657148: bl       #0x637a68
0065714c: mov      r0, sb
00657150: bl       #0x637b8c
00657154: ldr      r3, [r6]
00657158: cmp      r3, #0
0065715c: beq      #0x6568e8
00657160: b        #0x6568dc
00657164: ldr      r3, [r6]
00657168: mov      r0, r6
0065716c: mov      lr, pc
00657170: ldr      pc, [r3, #0x18]
00657174: b        #0x656ea4
00657178: ldr      r3, [r2]
0065717c: mov      r0, r2
00657180: mov      lr, pc
00657184: ldr      pc, [r3, #0x18]
00657188: b        #0x656e88
0065718c: ldr      ip, [r4, #0x180]
00657190: mov      r0, #1
00657194: add      r6, sp, #0x94
00657198: mov      r2, r0
0065719c: mov      r1, ip
006571a0: ldr      ip, [ip]
006571a4: str      r3, [sp, #4]
006571a8: str      r3, [sp]
006571ac: str      r0, [sp, #8]
006571b0: mov      r3, #4
006571b4: mov      r0, r6
006571b8: mov      lr, pc
006571bc: ldr      pc, [ip, #0x78]
006571c0: ldr      r3, [sp, #0x94]
006571c4: cmp      r3, #0
006571c8: ldrne    r2, [r3, #4]
006571cc: addne    r2, r2, #1
006571d0: strne    r2, [r3, #4]
006571d4: ldr      r1, [sp, #0x18]
006571d8: ldr      r2, [r5, r1]
006571dc: ldr      r0, [r2]
006571e0: str      r3, [r2]
006571e4: cmp      r0, #0
006571e8: beq      #0x6571f0
006571ec: bl       #0x31d584
006571f0: mov      r0, r6
006571f4: bl       #0x637b8c
006571f8: ldr      r3, [r4, #0x18c]
006571fc: ldr      r2, [r3, #0x18]
00657200: b        #0x656930
00657204: eoreq    lr, r8, r0, lsl #27
00657208: ldrshteq lr, [r3], -r0
0065720c: eoreq    lr, r8, ip, lsr #26
00657210: eoreq    lr, r8, r8, asr #29
00657214: eoreq    lr, r8, ip, lsl #29
00657218: eoreq    lr, r8, r0, ror lr
0065721c: mlaeq    r8, ip, lr, lr
00657220: eoreq    lr, r8, r8, lsl #29
00657224: eoreq    pc, r8, r4, lsl #1
00657228: eoreq    pc, r8, r0, ror r0
0065722c: eoreq    r2, r8, r4, lsl #28
00657230: eoreq    lr, r8, r0, lsr #25
00657234: eoreq    lr, r8, r0, ror #27
00657238: eoreq    lr, r8, r0, ror sp
0065723c: eoreq    lr, r8, r4, asr sp
00657240: eoreq    lr, r8, r8, asr #26
00657244: eoreq    lr, r8, r4, lsr sp
00657248: eoreq    lr, r8, r8, lsr #26
0065724c: eoreq    lr, r8, ip, lsl sp
00657250: eoreq    lr, r8, r8, lsl #22

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjPvENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EEN6glitch4core10SAllocatorIS6_LNSB_6memory13E_MEMORY_HINTE0EEEE13insert_uniqueERKS6_
0063a8ec: push     {r4, r5, r6, lr}
0063a8f0: ldr      ip, [r1, #4]
0063a8f4: sub      sp, sp, #0x10
0063a8f8: mov      r4, r0
0063a8fc: cmp      ip, #0
0063a900: mov      r3, r2
0063a904: moveq    ip, r1
0063a908: beq      #0x63a964
0063a90c: ldr      r6, [r2]
0063a910: b        #0x63a918
0063a914: mov      ip, r2
0063a918: ldr      r0, [ip, #0x10]
0063a91c: mov      r5, #1
0063a920: cmp      r0, r6
0063a924: ldrhi    r2, [ip, #8]
0063a928: ldrls    r2, [ip, #0xc]
0063a92c: movls    r5, #0
0063a930: cmp      r2, #0
0063a934: bne      #0x63a914
0063a938: cmp      r5, #0
0063a93c: moveq    r5, ip
0063a940: bne      #0x63a964
0063a944: cmp      r6, r0
0063a948: movls    r3, #0
0063a94c: strls    r5, [r4]
0063a950: strbls   r3, [r4, #4]
0063a954: bhi      #0x63a9cc
0063a958: mov      r0, r4
0063a95c: add      sp, sp, #0x10
0063a960: pop      {r4, r5, r6, pc}
0063a964: ldr      r2, [r1, #8]
0063a968: cmp      ip, r2
0063a96c: beq      #0x63aa4c
0063a970: ldrb     r2, [ip]
0063a974: cmp      r2, #0
0063a978: bne      #0x63a98c
0063a97c: ldr      r2, [ip, #4]
0063a980: ldr      r2, [r2, #4]
0063a984: cmp      ip, r2
0063a988: beq      #0x63aa38
0063a98c: ldr      r0, [ip, #8]
0063a990: cmp      r0, #0
0063a994: bne      #0x63a9a0
0063a998: b        #0x63a9f8
0063a99c: mov      r0, r2
0063a9a0: ldr      r2, [r0, #0xc]
0063a9a4: cmp      r2, #0
0063a9a8: bne      #0x63a99c
0063a9ac: ldr      r6, [r3]
0063a9b0: mov      r5, r0
0063a9b4: ldr      r0, [r0, #0x10]
0063a9b8: cmp      r6, r0
0063a9bc: movls    r3, #0
0063a9c0: strls    r5, [r4]
0063a9c4: strbls   r3, [r4, #4]
0063a9c8: bls      #0x63a958
0063a9cc: mov      r2, ip
0063a9d0: add      r0, sp, #8
0063a9d4: mov      ip, #0
0063a9d8: str      ip, [sp, #4]
0063a9dc: str      ip, [sp]
0063a9e0: bl       #0x63a7bc
0063a9e4: ldr      r3, [sp, #8]
0063a9e8: mov      r2, #1
0063a9ec: strb     r2, [r4, #4]
0063a9f0: str      r3, [r4]
0063a9f4: b        #0x63a958
0063a9f8: ldr      r2, [ip, #4]
0063a9fc: ldr      r0, [r2, #8]
0063aa00: cmp      ip, r0
0063aa04: movne    r5, r2
0063aa08: ldrne    r6, [r3]
0063aa0c: ldrne    r0, [r2, #0x10]
0063aa10: beq      #0x63aa1c
0063aa14: b        #0x63a944
0063aa18: mov      r2, r5
0063aa1c: ldr      r5, [r2, #4]
0063aa20: ldr      r0, [r5, #8]
0063aa24: cmp      r0, r2
0063aa28: beq      #0x63aa18
0063aa2c: ldr      r6, [r3]
0063aa30: ldr      r0, [r5, #0x10]
0063aa34: b        #0x63a944
0063aa38: ldr      r2, [ip, #0xc]
0063aa3c: ldr      r6, [r3]
0063aa40: mov      r5, r2
0063aa44: ldr      r0, [r2, #0x10]
0063aa48: b        #0x63a944
0063aa4c: mov      r2, ip
0063aa50: mov      lr, #0
0063aa54: add      r0, sp, #0xc
0063aa58: stm      sp, {ip, lr}
0063aa5c: bl       #0x63a7bc
0063aa60: ldr      r3, [sp, #0xc]
0063aa64: mov      r2, #1
0063aa68: strb     r2, [r4, #4]
0063aa6c: str      r3, [r4]
0063aa70: b        #0x63a958

# _ZN6glitch2ps10PLifeModelINS0_9SParticleEEC2Ev
0065452c: push     {r4, r5, lr}
00654530: ldr      r3, [r1]
00654534: sub      sp, sp, #0x24
00654538: mov      r4, r0
0065453c: str      r3, [r0]
00654540: ldr      r2, [r1, #4]
00654544: ldr      r3, [r3, #-0xc]
00654548: ldr      r1, [pc, #0x78]
0065454c: str      r2, [r0, r3]
00654550: ldr      r3, [r0]
00654554: add      r1, pc, r1
00654558: ldr      r5, [r3, #-0xc]
0065455c: add      r5, r0, r5
00654560: mov      r0, r5
00654564: bl       #0x64d0bc
00654568: add      r2, sp, #0x10
0065456c: add      r3, r4, #4
00654570: str      r0, [sp, #0x10]
00654574: add      r1, r5, #0x30
00654578: add      r0, sp, #0x18
0065457c: str      r3, [sp, #0x14]
00654580: bl       #0x63a8ec
00654584: ldr      r3, [r4]
00654588: ldr      r1, [pc, #0x3c]
0065458c: ldr      r5, [r3, #-0xc]
00654590: add      r1, pc, r1
00654594: add      r5, r4, r5
00654598: mov      r0, r5
0065459c: bl       #0x64d0bc
006545a0: add      r3, r4, #8
006545a4: str      r0, [sp]
006545a8: add      r1, r5, #0x30
006545ac: add      r0, sp, #8
006545b0: mov      r2, sp
006545b4: str      r3, [sp, #4]
006545b8: bl       #0x63a8ec
006545bc: mov      r0, r4
006545c0: add      sp, sp, #0x24
006545c4: pop      {r4, r5, pc}
006545c8: eoreq    r0, sb, ip, asr #28
006545cc: eoreq    r0, sb, r8, lsl lr

# _ZN6glitch7collada24CParticleSystemSceneNode19onRegisterSceneNodeEv
0064da1c: push     {r4, r5, r6, r7, lr}
0064da20: ldr      r2, [r0, #0x178]
0064da24: movw     r3, #0x5c29
0064da28: movt     r3, #0xc28f
0064da2c: ldr      r1, [r2]
0064da30: sub      sp, sp, #0x1c
0064da34: mov      r6, r0
0064da38: ldr      r1, [r1, #-0xc]
0064da3c: add      r2, r2, r1
0064da40: ldr      r1, [r2, #0x24]
0064da44: ldr      r2, [r2, #0x28]
0064da48: rsb      r2, r1, r2
0064da4c: asr      r2, r2, #2
0064da50: mul      r3, r3, r2
0064da54: cmp      r3, #0
0064da58: beq      #0x64dab4
0064da5c: ldr      r7, [r0, #0x110]
0064da60: add      r4, sp, #0x14
0064da64: mov      r0, r4
0064da68: ldr      ip, [r7]
0064da6c: mov      r1, r6
0064da70: mov      r2, #0
0064da74: ldr      r3, [r6]
0064da78: ldr      r5, [ip, #0x24]
0064da7c: mov      lr, pc
0064da80: ldr      pc, [r3, #0x84]
0064da84: mov      r2, #8
0064da88: mov      r3, #0
0064da8c: str      r2, [sp]
0064da90: mvn      r2, #0x80000000
0064da94: str      r2, [sp, #8]
0064da98: str      r3, [sp, #4]
0064da9c: mov      r0, r7
0064daa0: mov      r1, r6
0064daa4: mov      r2, r4
0064daa8: blx      r5
0064daac: mov      r0, r4
0064dab0: bl       #0x310be8
0064dab4: mov      r0, #1
0064dab8: add      sp, sp, #0x1c
0064dabc: pop      {r4, r5, r6, r7, pc}

# _ZZN6glitch2ps9PSManager20createParticleSystemINS0_9SParticleENS0_16PGenerationModelIS3_EENS0_10PSizeModelIS3_EENS0_11PColorModelIS3_EENS0_13PEmitterModelIS3_EENS0_12PMotionModelIS3_EENS0_12PForcesModelIS3_EENS0_10PSpinModelIS3_EENS0_10PLifeModelIS3_EENS0_25PRenderDataBillboardModelIS3_NS0_27PSNullShaderParametersBakerENS0_21PSBillboardColorBakerIS3_EENS0_22PSBillboardNormalBakerIS3_EENS0_24PSBillboardPositionBakerIS3_EENS0_25PSBillboardTexCoordsBakerIS3_EEEEEEPNS0_15IParticleSystemIT_EEvEN5MixinC1Ev
00654a74: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00654a78: mov      r4, r0
00654a7c: sub      sp, sp, #0x84
00654a80: ldr      r7, [pc, #0x410]
00654a84: add      r0, r0, #0x180
00654a88: bl       #0x64d1d0
00654a8c: ldr      r2, [pc, #0x408]
00654a90: add      r7, pc, r7
00654a94: movw     r3, #0xcd15
00654a98: ldr      r8, [r7, r2]
00654a9c: mov      r5, #0
00654aa0: movt     r3, #0x75b
00654aa4: ldmib    r8, {r2, ip}
00654aa8: add      r1, r8, #0xc
00654aac: str      r2, [r4]
00654ab0: ldr      r2, [r2, #-0xc]
00654ab4: add      r0, r4, #0x10
00654ab8: mov      r6, r4
00654abc: str      ip, [r4, r2]
00654ac0: str      r3, [r4, #8]
00654ac4: str      r3, [r4, #4]
00654ac8: str      r5, [r4, #0xc]
00654acc: bl       #0x6545d0
00654ad0: add      r1, r8, #0x14
00654ad4: add      r0, r4, #0x24
00654ad8: bl       #0x6543f8
00654adc: add      r1, r8, #0x1c
00654ae0: add      r0, r4, #0x38
00654ae4: bl       #0x654690
00654ae8: add      r1, r8, #0x24
00654aec: add      r0, r4, #0x60
00654af0: bl       #0x6548fc
00654af4: add      r1, r8, #0x2c
00654af8: add      r0, r4, #0x78
00654afc: bl       #0x6540ec
00654b00: ldr      r3, [r8, #0x34]
00654b04: ldr      r2, [r8, #0x38]
00654b08: add      r1, r8, #0x3c
00654b0c: str      r3, [r4, #0x94]
00654b10: ldr      r3, [r3, #-0xc]
00654b14: add      r0, r4, #0xa8
00654b18: add      sl, r4, #0x10c
00654b1c: add      r3, r4, r3
00654b20: str      r2, [r3, #0x94]
00654b24: str      r5, [r4, #0x98]
00654b28: str      r5, [r4, #0x9c]
00654b2c: str      r5, [r4, #0xa0]
00654b30: strb     r5, [r4, #0xa4]
00654b34: bl       #0x654218
00654b38: add      r1, r8, #0x44
00654b3c: add      r0, r4, #0xd0
00654b40: bl       #0x65452c
00654b44: ldr      r3, [r8, #0x4c]
00654b48: ldr      r1, [r8, #0x50]
00654b4c: mov      r2, #0x40
00654b50: str      r3, [r6, #0xdc]!
00654b54: ldr      r3, [r3, #-0xc]
00654b58: mov      r0, sl
00654b5c: mov      r8, #1
00654b60: str      r1, [r6, r3]
00654b64: mov      r3, #0x104
00654b68: mov      r1, #0xff
00654b6c: strh     r1, [r4, r3]
00654b70: movw     r3, #0x106
00654b74: mov      r1, #6
00654b78: strh     r1, [r4, r3]
00654b7c: str      r5, [r4, #0xe0]
00654b80: str      r5, [r4, #0xe4]
00654b84: str      r5, [r4, #0xec]
00654b88: str      r5, [r4, #0xf0]
00654b8c: str      r5, [r4, #0xf4]
00654b90: str      r5, [r4, #0xf8]
00654b94: str      r5, [r4, #0xfc]
00654b98: str      r5, [r4, #0x100]
00654b9c: str      r5, [r4, #0x108]
00654ba0: strb     r5, [r4, #0x14c]
00654ba4: mov      r1, r5
00654ba8: bl       #0x30e460
00654bac: ldr      r1, [r4, #0xdc]
00654bb0: mov      r2, #0xbf000000
00654bb4: mov      r3, #0x3f800000
00654bb8: add      r2, r2, #0x800000
00654bbc: str      r2, [r4, #0x158]
00654bc0: str      r3, [r4, #0x164]
00654bc4: str      r3, [r4, #0x10c]
00654bc8: str      r3, [r4, #0x120]
00654bcc: str      r3, [r4, #0x134]
00654bd0: str      r3, [r4, #0x148]
00654bd4: strb     r8, [r4, #0x14c]
00654bd8: str      r2, [r4, #0x150]
00654bdc: str      r2, [r4, #0x154]
00654be0: str      r3, [r4, #0x15c]
00654be4: str      r3, [r4, #0x160]
00654be8: strb     r8, [r4, #0x168]
00654bec: str      r5, [r4, #0x16c]
00654bf0: str      r5, [r4, #0x170]
00654bf4: str      r5, [r4, #0x178]
00654bf8: str      r5, [r4, #0x17c]
00654bfc: ldr      sb, [r1, #-0xc]
00654c00: ldr      r1, [pc, #0x298]
00654c04: add      sb, r6, sb
00654c08: add      r1, pc, r1
00654c0c: mov      r0, sb
00654c10: bl       #0x64d0bc
00654c14: add      r2, sp, #0x70
00654c18: add      r3, r4, #0x168
00654c1c: str      r0, [sp, #0x70]
00654c20: add      r1, sb, #0x30
00654c24: add      r0, sp, #0x78
00654c28: str      r3, [sp, #0x74]
00654c2c: bl       #0x63a8ec
00654c30: ldr      r3, [r4, #0xdc]
00654c34: ldr      r1, [pc, #0x268]
00654c38: ldr      sb, [r3, #-0xc]
00654c3c: add      r1, pc, r1
00654c40: add      sb, r6, sb
00654c44: mov      r0, sb
00654c48: bl       #0x64d0bc
00654c4c: add      r2, sp, #0x60
00654c50: add      r3, r4, #0xe0
00654c54: str      r0, [sp, #0x60]
00654c58: add      r1, sb, #0x30
00654c5c: add      r0, sp, #0x68
00654c60: str      r3, [sp, #0x64]
00654c64: bl       #0x63a8ec
00654c68: ldr      r3, [r4, #0xdc]
00654c6c: ldr      r1, [pc, #0x234]
00654c70: ldr      sb, [r3, #-0xc]
00654c74: add      r1, pc, r1
00654c78: add      sb, r6, sb
00654c7c: mov      r0, sb
00654c80: bl       #0x64d0bc
00654c84: add      r2, sp, #0x50
00654c88: add      r3, r4, #0x17c
00654c8c: str      r0, [sp, #0x50]
00654c90: add      r1, sb, #0x30
00654c94: add      r0, sp, #0x58
00654c98: str      r3, [sp, #0x54]
00654c9c: bl       #0x63a8ec
00654ca0: ldr      r3, [r4, #0xdc]
00654ca4: ldr      r1, [pc, #0x200]
00654ca8: ldr      sb, [r3, #-0xc]
00654cac: add      r1, pc, r1
00654cb0: add      sb, r6, sb
00654cb4: mov      r0, sb
00654cb8: bl       #0x64d0bc
00654cbc: add      r2, sp, #0x40
00654cc0: add      r3, r4, #0xe4
00654cc4: str      r0, [sp, #0x40]
00654cc8: add      r1, sb, #0x30
00654ccc: add      r0, sp, #0x48
00654cd0: str      r3, [sp, #0x44]
00654cd4: bl       #0x63a8ec
00654cd8: ldr      r3, [r4, #0xdc]
00654cdc: ldr      r1, [pc, #0x1cc]
00654ce0: ldr      sb, [r3, #-0xc]
00654ce4: add      r1, pc, r1
00654ce8: add      sb, r6, sb
00654cec: mov      r0, sb
00654cf0: bl       #0x64d0bc
00654cf4: add      r2, sp, #0x30
00654cf8: add      r3, r4, #0x16c
00654cfc: str      r0, [sp, #0x30]
00654d00: add      r1, sb, #0x30
00654d04: add      r0, sp, #0x38
00654d08: str      r3, [sp, #0x34]
00654d0c: bl       #0x63a8ec
00654d10: ldr      r3, [r4, #0xdc]
00654d14: ldr      r1, [pc, #0x198]
00654d18: ldr      sb, [r3, #-0xc]
00654d1c: add      r1, pc, r1
00654d20: add      sb, r6, sb
00654d24: mov      r0, sb
00654d28: bl       #0x64d0bc
00654d2c: add      r2, sp, #0x20
00654d30: add      r3, r4, #0x174
00654d34: str      r0, [sp, #0x20]
00654d38: add      r1, sb, #0x30
00654d3c: add      r0, sp, #0x28
00654d40: str      r3, [sp, #0x24]
00654d44: bl       #0x63a8ec
00654d48: ldr      r3, [r4, #0xdc]
00654d4c: ldr      r1, [pc, #0x164]
00654d50: ldr      sb, [r3, #-0xc]
00654d54: add      r1, pc, r1
00654d58: add      sb, r6, sb
00654d5c: mov      r0, sb
00654d60: bl       #0x64d0bc
00654d64: add      r2, sp, #0x10
00654d68: str      r0, [sp, #0x10]
00654d6c: add      r1, sb, #0x30
00654d70: add      r0, sp, #0x18
00654d74: str      sl, [sp, #0x14]
00654d78: bl       #0x63a8ec
00654d7c: ldr      r3, [r4, #0xdc]
00654d80: ldr      r1, [pc, #0x134]
00654d84: ldr      sl, [r3, #-0xc]
00654d88: add      r1, pc, r1
00654d8c: add      sl, r6, sl
00654d90: mov      r0, sl
00654d94: bl       #0x64d0bc
00654d98: add      r3, r4, #0x150
00654d9c: str      r0, [sp]
00654da0: add      r1, sl, #0x30
00654da4: add      r0, sp, #8
00654da8: mov      r2, sp
00654dac: str      r3, [sp, #4]
00654db0: bl       #0x63a8ec
00654db4: ldr      r2, [r4, #0xdc]
00654db8: mov      r3, #0
00654dbc: mov      ip, #0x3f000000
00654dc0: ldr      r2, [r2, #-0xc]
00654dc4: mov      r1, r5
00654dc8: mov      r0, r8
00654dcc: add      r2, r6, r2
00654dd0: strb     r5, [r2, #4]
00654dd4: ldr      r2, [r4, #0xdc]
00654dd8: ldr      r2, [r2, #-0xc]
00654ddc: add      r2, r6, r2
00654de0: strb     r5, [r2, #5]
00654de4: ldr      r2, [r4, #0xdc]
00654de8: ldr      r2, [r2, #-0xc]
00654dec: add      r2, r6, r2
00654df0: str      ip, [r2, #8]
00654df4: str      r3, [r2, #0x10]
00654df8: str      r3, [r2, #0xc]
00654dfc: ldr      r2, [r4, #0xdc]
00654e00: ldr      r2, [r2, #-0xc]
00654e04: add      r2, r6, r2
00654e08: str      ip, [r2, #0x18]
00654e0c: str      r3, [r2, #0x1c]
00654e10: str      r3, [r2, #0x14]
00654e14: ldr      r3, [r4, #0xdc]
00654e18: ldr      r3, [r3, #-0xc]
00654e1c: add      r6, r6, r3
00654e20: strb     r5, [r6, #0x20]
00654e24: bl       #0x5341ac
00654e28: ldr      r3, [pc, #0x90]
00654e2c: str      r0, [r4, #0x178]
00654e30: mov      r0, r4
00654e34: ldr      r3, [r7, r3]
00654e38: add      fp, r3, #0x138
00654e3c: add      sl, r3, #0xc
00654e40: add      r8, r3, #0x1f4
00654e44: add      r7, r3, #0x30
00654e48: add      r6, r3, #0x4c
00654e4c: add      r5, r3, #0x6c
00654e50: add      ip, r3, #0x90
00654e54: add      r1, r3, #0xac
00654e58: add      r2, r3, #0xcc
00654e5c: add      sb, r3, #0xf8
00654e60: add      r3, r3, #0x118
00654e64: str      sl, [r4]
00654e68: str      r8, [r4, #0x180]
00654e6c: str      r7, [r4, #0x10]
00654e70: str      r6, [r4, #0x24]
00654e74: str      r5, [r4, #0x38]
00654e78: str      ip, [r4, #0x60]
00654e7c: str      r1, [r4, #0x78]
00654e80: str      r2, [r4, #0x94]
00654e84: str      sb, [r4, #0xa8]
00654e88: str      r3, [r4, #0xd0]
00654e8c: str      fp, [r4, #0xdc]
00654e90: add      sp, sp, #0x84
00654e94: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00654e98: eorseq   r0, r4, r0
00654e9c: andeq    r1, r0, ip, lsr r2
00654ea0: eoreq    r0, sb, r8, ror #18
00654ea4: eoreq    r0, sb, ip, lsr #9
00654ea8: eoreq    r0, sb, ip, lsl #18
00654eac: eoreq    r0, sb, ip, lsr r5
00654eb0: eoreq    r0, sb, ip, lsr #17
00654eb4: eoreq    r0, sb, r4, lsl #17
00654eb8: eoreq    r0, sb, r4, ror #16
00654ebc: eoreq    r0, sb, r0, asr #16
00654ec0: muleq    r0, r4, fp

# _ZN6glitch2ps16IParticleContextINS0_9SParticleEE12setParameterIfEEvPKcT_
00650400: push     {r4, r5, r6, lr}
00650404: sub      sp, sp, #0x10
00650408: mov      r6, r0
0065040c: mov      r5, r2
00650410: bl       #0x64d0bc
00650414: ldr      ip, [r6, #0x34]
00650418: add      r1, r6, #0x30
0065041c: mov      r4, r0
00650420: cmp      ip, #0
00650424: moveq    ip, r1
00650428: beq      #0x650458
0065042c: mov      r2, r1
00650430: b        #0x650438
00650434: mov      ip, r3
00650438: ldr      r3, [ip, #0x10]
0065043c: cmp      r4, r3
00650440: ldrhi    r3, [ip, #0xc]
00650444: ldrls    r3, [ip, #8]
00650448: movhi    ip, r2
0065044c: mov      r2, ip
00650450: cmp      r3, #0
00650454: bne      #0x650434
00650458: cmp      r1, ip
0065045c: beq      #0x650470
00650460: ldr      r2, [ip, #0x10]
00650464: mov      r3, ip
00650468: cmp      r4, r2
0065046c: bhs      #0x650490
00650470: mov      r3, sp
00650474: mov      lr, #0
00650478: add      r0, sp, #8
0065047c: add      r2, sp, #0xc
00650480: stm      sp, {r4, lr}
00650484: str      ip, [sp, #0xc]
00650488: bl       #0x63aa74
0065048c: ldr      r3, [sp, #8]
00650490: ldr      r3, [r3, #0x14]
00650494: cmp      r3, #0
00650498: strne    r5, [r3]
0065049c: add      sp, sp, #0x10
006504a0: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada24CParticleSystemSceneNode9onAnimateEj
0064dac0: push     {r4, r5, r6, lr}
0064dac4: mov      r4, r0
0064dac8: sub      sp, sp, #8
0064dacc: mov      r5, r1
0064dad0: bl       #0x596d6c
0064dad4: ldr      r3, [r4, #0xec]
0064dad8: ldr      r6, [r4, #0x178]
0064dadc: mov      r0, r3
0064dae0: ldr      r3, [r3]
0064dae4: mov      lr, pc
0064dae8: ldr      pc, [r3, #0x38]
0064daec: str      r0, [r6, #0xc]
0064daf0: ldr      r2, [r4, #0x110]
0064daf4: mvn      r3, #0
0064daf8: str      r5, [r4, #0x144]
0064dafc: strb     r3, [sp, #7]
0064db00: strb     r3, [sp, #4]
0064db04: strb     r3, [sp, #5]
0064db08: strb     r3, [sp, #6]
0064db0c: ldr      r3, [r2, #0x14]
0064db10: mov      r0, r3
0064db14: ldr      r3, [r3]
0064db18: mov      lr, pc
0064db1c: ldr      pc, [r3, #0x5c]
0064db20: tst      r0, #7
0064db24: addeq    r5, sp, #4
0064db28: beq      #0x64db68
0064db2c: mov      r0, sp
0064db30: mov      r1, r4
0064db34: mov      r2, #0
0064db38: ldr      r3, [r4]
0064db3c: mov      lr, pc
0064db40: ldr      pc, [r3, #0x84]
0064db44: ldr      r1, [r4, #0x174]
0064db48: add      r5, sp, #4
0064db4c: ldr      r0, [sp]
0064db50: uxth     r1, r1
0064db54: mov      r2, #0
0064db58: mov      r3, r5
0064db5c: bl       #0x5c71a4
0064db60: mov      r0, sp
0064db64: bl       #0x310be8
0064db68: ldr      r0, [r4, #0x144]
0064db6c: bl       #0x30e2e0
0064db70: mov      r1, #0x44000000
0064db74: add      r1, r1, #0x7a0000
0064db78: bl       #0x30ec94
0064db7c: ldr      r6, [r4, #0x178]
0064db80: mov      r1, r0
0064db84: mov      r2, r5
0064db88: mov      r0, r6
0064db8c: ldr      r3, [r6]
0064db90: mov      lr, pc
0064db94: ldr      pc, [r3, #0x10]
0064db98: add      sp, sp, #8
0064db9c: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada24CParticleSystemSceneNodeD1Ev
0064ecac: push     {r4, r5, r6, lr}
0064ecb0: ldr      r5, [pc, #0x54]
0064ecb4: ldr      r3, [pc, #0x54]
0064ecb8: ldr      r2, [r0, #0x178]
0064ecbc: add      r5, pc, r5
0064ecc0: ldr      r3, [r5, r3]
0064ecc4: cmp      r2, #0
0064ecc8: mov      r4, r0
0064eccc: add      r1, r3, #0x138
0064ecd0: add      r3, r3, #0x1c
0064ecd4: str      r3, [r0]
0064ecd8: str      r1, [r0, #0x190]
0064ecdc: beq      #0x64ecf0
0064ece0: mov      r0, r2
0064ece4: ldr      r3, [r2]
0064ece8: mov      lr, pc
0064ecec: ldr      pc, [r3, #8]
0064ecf0: ldr      r1, [pc, #0x1c]
0064ecf4: mov      r0, r4
0064ecf8: ldr      r1, [r5, r1]
0064ecfc: add      r1, r1, #4
0064ed00: bl       #0x6678b8
0064ed04: mov      r0, r4
0064ed08: pop      {r4, r5, r6, pc}
0064ed0c: ldrsbteq r5, [r4], -r4
0064ed10: andeq    r2, r0, ip, asr #26
0064ed14: strheq   r1, [r0], -ip
