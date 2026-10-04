
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

# _ZNK6glitch7collada15animation_track8CFloatEx16getKeyBasedValueERKNS0_18SAnimationAccessorEiPv
006e32cc: push     {r4, r5, r6, lr}
006e32d0: mov      r0, r1
006e32d4: mov      r1, #0
006e32d8: mov      r5, r3
006e32dc: mov      r4, r2
006e32e0: bl       #0x669e24
006e32e4: ldr      r3, [r0, #4]
006e32e8: ldr      r3, [r3, r4, lsl #2]
006e32ec: str      r3, [r5]
006e32f0: pop      {r4, r5, r6, pc}

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

# _ZNK6glitch7collada15animation_track8CFloatEx10applyValueEPvS3_PNS1_15CApplicatorInfoE
006e3258: push     {r4, r5, r6, lr}
006e325c: ldr      r3, [r0]
006e3260: mov      r5, r2
006e3264: mov      r4, r1
006e3268: mov      lr, pc
006e326c: ldr      pc, [r3, #8]
006e3270: mov      r3, r0
006e3274: mov      r1, r4
006e3278: mov      r0, r5
006e327c: mov      r2, r3
006e3280: pop      {r4, r5, r6, lr}
006e3284: b        #0x30e868

# _ZNK6glitch7collada15animation_track8CFloatEx16getIdentityValueEPv
006e3118: mov      r3, #0
006e311c: str      r3, [r1]
006e3120: bx       lr

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

# _ZNK6glitch7collada15animation_track8CFloatEx15getBlendedValueEPvPfiS3_
006e3124: push     {r4, r5, r6, r7, r8, sb, sl, lr}
006e3128: ldr      r4, [sp, #0x20]
006e312c: mov      r0, #0
006e3130: subs     r6, r3, #0
006e3134: mov      r5, r1
006e3138: mov      sb, r2
006e313c: str      r0, [r4]
006e3140: ble      #0x6e3180
006e3144: mov      r7, #0
006e3148: mov      sl, r0
006e314c: mov      r8, r7
006e3150: ldr      r1, [sb, r7]
006e3154: ldr      r0, [r5, r7]
006e3158: bl       #0x30ed6c
006e315c: mov      r1, r0
006e3160: mov      r0, sl
006e3164: bl       #0x30eba4
006e3168: add      r8, r8, #1
006e316c: cmp      r8, r6
006e3170: mov      sl, r0
006e3174: str      r0, [r4]
006e3178: add      r7, r7, #4
006e317c: bne      #0x6e3150
006e3180: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
006e32f4: push     {r4, r5, r6, r7, r8, lr}
006e32f8: mov      r4, r1
006e32fc: mov      r1, #0
006e3300: mov      r7, r3
006e3304: mov      r5, r2
006e3308: bl       #0x669e24
006e330c: ldr      r6, [r0, #4]
006e3310: lsl      r4, r4, #2
006e3314: ldr      r5, [r6, r5, lsl #2]
006e3318: ldr      r0, [r6, r7, lsl #2]
006e331c: mov      r1, r5
006e3320: bl       #0x30e3ac
006e3324: mov      r1, r0
006e3328: ldr      r0, [sp, #0x18]
006e332c: bl       #0x30ed6c
006e3330: mov      r1, r0
006e3334: mov      r0, r5
006e3338: bl       #0x30eba4
006e333c: ldr      r1, [r6, r4]
006e3340: bl       #0x30e3ac
006e3344: ldr      r3, [sp, #0x1c]
006e3348: str      r0, [r3]
006e334c: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada15animation_track8CFloatEx12getValueSizeEv
006e3110: mov      r0, #4
006e3114: bx       lr

# _ZN6glitch7collada16CColladaDatabase19getAnimationTrackExEPKNS0_10SAnimationE
00611ae0: ldr      r3, [pc, #0x61c]
00611ae4: push     {r4, r5, r6, lr}
00611ae8: subs     r4, r0, #0
00611aec: add      r3, pc, r3
00611af0: beq      #0x611c80
00611af4: ldr      r2, [r4, #0x10]
00611af8: ldr      r2, [r2, #8]
00611afc: sub      r2, r2, #1
00611b00: cmp      r2, #0x5a
00611b04: addls    pc, pc, r2, lsl #2
00611b08: b        #0x611c80
00611b0c: b        #0x611cc4
00611b10: b        #0x611ce8
00611b14: b        #0x611d0c
00611b18: b        #0x611d30
00611b1c: b        #0x611d54
00611b20: b        #0x611d78
00611b24: b        #0x611d78
00611b28: b        #0x611d78
00611b2c: b        #0x611d78
00611b30: b        #0x611d9c
00611b34: b        #0x611dc0
00611b38: b        #0x611de4
00611b3c: b        #0x611e08
00611b40: b        #0x611e2c
00611b44: b        #0x611c80
00611b48: b        #0x611e44
00611b4c: b        #0x611c80
00611b50: b        #0x611c80
00611b54: b        #0x611c80
00611b58: b        #0x611e4c
00611b5c: b        #0x611c80
00611b60: b        #0x611c80
00611b64: b        #0x611c80
00611b68: b        #0x611c80
00611b6c: b        #0x611c80
00611b70: b        #0x611c80
00611b74: b        #0x611c80
00611b78: b        #0x611e58
00611b7c: b        #0x611e58
00611b80: b        #0x611e58
00611b84: b        #0x611e58
00611b88: b        #0x611e58
00611b8c: b        #0x611e64
00611b90: b        #0x611e58
00611b94: b        #0x611e58
00611b98: b        #0x611e58
00611b9c: b        #0x611e58
00611ba0: b        #0x611e58
00611ba4: b        #0x611e64
00611ba8: b        #0x611e58
00611bac: b        #0x611e58
00611bb0: b        #0x611e58
00611bb4: b        #0x611e58
00611bb8: b        #0x611e58
00611bbc: b        #0x611e58
00611bc0: b        #0x611e58
00611bc4: b        #0x611e58
00611bc8: b        #0x611e58
00611bcc: b        #0x611e58
00611bd0: b        #0x611e58
00611bd4: b        #0x611e58
00611bd8: b        #0x611e58
00611bdc: b        #0x611e58
00611be0: b        #0x611e58
00611be4: b        #0x611e58
00611be8: b        #0x611e58
00611bec: b        #0x611e64
00611bf0: b        #0x611e64
00611bf4: b        #0x611e58
00611bf8: b        #0x611e58
00611bfc: b        #0x611e58
00611c00: b        #0x611e58
00611c04: b        #0x611e58
00611c08: b        #0x611e58
00611c0c: b        #0x611e58
00611c10: b        #0x611e58
00611c14: b        #0x611e64
00611c18: b        #0x611e58
00611c1c: b        #0x611e58
00611c20: b        #0x611e58
00611c24: b        #0x611c80
00611c28: b        #0x611c80
00611c2c: b        #0x611c80
00611c30: b        #0x611c80
00611c34: b        #0x611c80
00611c38: b        #0x611c80
00611c3c: b        #0x611c80
00611c40: b        #0x611c80
00611c44: b        #0x611c80
00611c48: b        #0x611c80
00611c4c: b        #0x611c80
00611c50: b        #0x611c80
00611c54: b        #0x611c80
00611c58: b        #0x611c80
00611c5c: b        #0x611c80
00611c60: b        #0x611c88
00611c64: b        #0x611e38
00611c68: b        #0x611e38
00611c6c: b        #0x611e38
00611c70: b        #0x611e38
00611c74: b        #0x611e38
00611c78: cmp      r3, #2
00611c7c: beq      #0x611f6c
00611c80: mov      r0, #0
00611c84: pop      {r4, r5, r6, pc}
00611c88: ldr      r2, [r4, #8]
00611c8c: ldr      r3, [r2, #0x10]
00611c90: cmp      r3, #1
00611c94: beq      #0x611e70
00611c98: cmp      r3, #6
00611c9c: bne      #0x611c80
00611ca0: ldr      r3, [r2, #0x14]
00611ca4: sub      r3, r3, #1
00611ca8: cmp      r3, #3
00611cac: addls    pc, pc, r3, lsl #2
00611cb0: b        #0x611c80
00611cb4: b        #0x611f9c
00611cb8: b        #0x611f94
00611cbc: b        #0x611f8c
00611cc0: b        #0x611f84
00611cc4: ldr      r3, [r4, #0x1c]
00611cc8: cmp      r3, #0
00611ccc: beq      #0x611ef4
00611cd0: ldr      r3, [r3]
00611cd4: cmp      r3, #1
00611cd8: beq      #0x612034
00611cdc: bhs      #0x611eec
00611ce0: pop      {r4, r5, r6, lr}
00611ce4: b        #0x6103c0
00611ce8: ldr      r3, [r4, #0x1c]
00611cec: cmp      r3, #0
00611cf0: beq      #0x611f54
00611cf4: ldr      r3, [r3]
00611cf8: cmp      r3, #1
00611cfc: beq      #0x612024
00611d00: bhs      #0x611f4c
00611d04: pop      {r4, r5, r6, lr}
00611d08: b        #0x61057c
00611d0c: ldr      r3, [r4, #0x1c]
00611d10: cmp      r3, #0
00611d14: beq      #0x611f04
00611d18: ldr      r3, [r3]
00611d1c: cmp      r3, #1
00611d20: beq      #0x61201c
00611d24: bhs      #0x611efc
00611d28: pop      {r4, r5, r6, lr}
00611d2c: b        #0x610738
00611d30: ldr      r3, [r4, #0x1c]
00611d34: cmp      r3, #0
00611d38: beq      #0x611f64
00611d3c: ldr      r3, [r3]
00611d40: cmp      r3, #1
00611d44: beq      #0x61204c
00611d48: bhs      #0x611f5c
00611d4c: pop      {r4, r5, r6, lr}
00611d50: b        #0x6108f4
00611d54: ldr      r3, [r4, #0x1c]
00611d58: cmp      r3, #0
00611d5c: beq      #0x611f6c
00611d60: ldr      r3, [r3]
00611d64: cmp      r3, #1
00611d68: beq      #0x612064
00611d6c: bhs      #0x611c78
00611d70: pop      {r4, r5, r6, lr}
00611d74: b        #0x610048
00611d78: ldr      r3, [r4, #0x1c]
00611d7c: cmp      r3, #0
00611d80: beq      #0x611f44
00611d84: ldr      r3, [r3]
00611d88: cmp      r3, #1
00611d8c: beq      #0x61205c
00611d90: bhs      #0x611f3c
00611d94: pop      {r4, r5, r6, lr}
00611d98: b        #0x610204
00611d9c: ldr      r3, [r4, #0x1c]
00611da0: cmp      r3, #0
00611da4: beq      #0x611f34
00611da8: ldr      r3, [r3]
00611dac: cmp      r3, #1
00611db0: beq      #0x612054
00611db4: bhs      #0x611f2c
00611db8: pop      {r4, r5, r6, lr}
00611dbc: b        #0x610ab0
00611dc0: ldr      r3, [r4, #0x1c]
00611dc4: cmp      r3, #0
00611dc8: beq      #0x611f24
00611dcc: ldr      r3, [r3]
00611dd0: cmp      r3, #1
00611dd4: beq      #0x61202c
00611dd8: bhs      #0x611f1c
00611ddc: pop      {r4, r5, r6, lr}
00611de0: b        #0x610c6c
00611de4: ldr      r3, [r4, #0x1c]
00611de8: cmp      r3, #0
00611dec: beq      #0x611f14
00611df0: ldr      r3, [r3]
00611df4: cmp      r3, #1
00611df8: beq      #0x612044
00611dfc: bhs      #0x611f0c
00611e00: pop      {r4, r5, r6, lr}
00611e04: b        #0x610e28
00611e08: ldr      r3, [r4, #0x1c]
00611e0c: cmp      r3, #0
00611e10: beq      #0x611f7c
00611e14: ldr      r3, [r3]
00611e18: cmp      r3, #1
00611e1c: beq      #0x61203c
00611e20: bhs      #0x611f74
00611e24: pop      {r4, r5, r6, lr}
00611e28: b        #0x610fe4
00611e2c: ldr      r2, [pc, #0x2d4]
00611e30: ldr      r0, [r3, r2]
00611e34: pop      {r4, r5, r6, pc}
00611e38: ldr      r2, [pc, #0x2cc]
00611e3c: ldr      r0, [r3, r2]
00611e40: pop      {r4, r5, r6, pc}
00611e44: pop      {r4, r5, r6, lr}
00611e48: b        #0x611078
00611e4c: ldr      r2, [pc, #0x2bc]
00611e50: ldr      r0, [r3, r2]
00611e54: pop      {r4, r5, r6, pc}
00611e58: ldr      r2, [pc, #0x2b4]
00611e5c: ldr      r0, [r3, r2]
00611e60: pop      {r4, r5, r6, pc}
00611e64: ldr      r2, [pc, #0x2ac]
00611e68: ldr      r0, [r3, r2]
00611e6c: pop      {r4, r5, r6, pc}
00611e70: ldr      r3, [r2, #0x14]
00611e74: cmp      r3, #3
00611e78: beq      #0x61206c
00611e7c: cmp      r3, #4
00611e80: beq      #0x611ff4
00611e84: cmp      r3, #1
00611e88: bne      #0x611c80
00611e8c: ldr      r3, [r4, #0x18]
00611e90: ldr      r2, [r3, #4]
00611e94: cmp      r2, #1
00611e98: ble      #0x611c80
00611e9c: ldr      r3, [r3]
00611ea0: sub      r3, r3, #1
00611ea4: cmp      r3, #0xe
00611ea8: addls    pc, pc, r3, lsl #2
00611eac: b        #0x611c80
00611eb0: b        #0x612004
00611eb4: b        #0x611ffc
00611eb8: b        #0x611c80
00611ebc: b        #0x612014
00611ec0: b        #0x611c80
00611ec4: b        #0x611c80
00611ec8: b        #0x611c80
00611ecc: b        #0x61200c
00611ed0: b        #0x611c80
00611ed4: b        #0x611c80
00611ed8: b        #0x611c80
00611edc: b        #0x611c80
00611ee0: b        #0x611c80
00611ee4: b        #0x611c80
00611ee8: b        #0x611ff4
00611eec: cmp      r3, #2
00611ef0: bne      #0x611c80
00611ef4: pop      {r4, r5, r6, lr}
00611ef8: b        #0x610298
00611efc: cmp      r3, #2
00611f00: bne      #0x611c80
00611f04: pop      {r4, r5, r6, lr}
00611f08: b        #0x610610
00611f0c: cmp      r3, #2
00611f10: bne      #0x611c80
00611f14: pop      {r4, r5, r6, lr}
00611f18: b        #0x610d00
00611f1c: cmp      r3, #2
00611f20: bne      #0x611c80
00611f24: pop      {r4, r5, r6, lr}
00611f28: b        #0x610b44
00611f2c: cmp      r3, #2
00611f30: bne      #0x611c80
00611f34: pop      {r4, r5, r6, lr}
00611f38: b        #0x610988
00611f3c: cmp      r3, #2
00611f40: bne      #0x611c80
00611f44: pop      {r4, r5, r6, lr}
00611f48: b        #0x6100dc
00611f4c: cmp      r3, #2
00611f50: bne      #0x611c80
00611f54: pop      {r4, r5, r6, lr}
00611f58: b        #0x610454
00611f5c: cmp      r3, #2
00611f60: bne      #0x611c80
00611f64: pop      {r4, r5, r6, lr}
00611f68: b        #0x6107cc
00611f6c: pop      {r4, r5, r6, lr}
00611f70: b        #0x60ff20
00611f74: cmp      r3, #2
00611f78: bne      #0x611c80
00611f7c: pop      {r4, r5, r6, lr}
00611f80: b        #0x610ebc
00611f84: pop      {r4, r5, r6, lr}
00611f88: b        #0x6116d4
00611f8c: pop      {r4, r5, r6, lr}
00611f90: b        #0x611640
00611f94: pop      {r4, r5, r6, lr}
00611f98: b        #0x6115ac
00611f9c: ldr      r5, [pc, #0x178]
00611fa0: add      r5, pc, r5
00611fa4: ldr      r3, [r5, #0xc]
00611fa8: tst      r3, #1
00611fac: beq      #0x612074
00611fb0: ldr      r3, [r4, #0x18]
00611fb4: ldm      r3, {r1, r2}
00611fb8: sub      r3, r1, #1
00611fbc: cmp      r3, #7
00611fc0: movhi    r1, #0
00611fc4: bhi      #0x611fd4
00611fc8: ldr      r1, [pc, #0x150]
00611fcc: add      r1, pc, r1
00611fd0: ldr      r1, [r1, r3, lsl #2]
00611fd4: ldr      r3, [pc, #0x148]
00611fd8: sub      r2, r2, #1
00611fdc: add      r2, r2, r2, lsl #2
00611fe0: add      r2, r2, r1
00611fe4: add      r3, pc, r3
00611fe8: add      r3, r3, r2, lsl #2
00611fec: ldr      r0, [r3, #0x10]
00611ff0: pop      {r4, r5, r6, pc}
00611ff4: pop      {r4, r5, r6, lr}
00611ff8: b        #0x611a4c
00611ffc: pop      {r4, r5, r6, lr}
00612000: b        #0x6117fc
00612004: pop      {r4, r5, r6, lr}
00612008: b        #0x611768
0061200c: pop      {r4, r5, r6, lr}
00612010: b        #0x611924
00612014: pop      {r4, r5, r6, lr}
00612018: b        #0x611890
0061201c: pop      {r4, r5, r6, lr}
00612020: b        #0x6106a4
00612024: pop      {r4, r5, r6, lr}
00612028: b        #0x6104e8
0061202c: pop      {r4, r5, r6, lr}
00612030: b        #0x610bd8
00612034: pop      {r4, r5, r6, lr}
00612038: b        #0x61032c
0061203c: pop      {r4, r5, r6, lr}
00612040: b        #0x610f50
00612044: pop      {r4, r5, r6, lr}
00612048: b        #0x610d94
0061204c: pop      {r4, r5, r6, lr}
00612050: b        #0x610860
00612054: pop      {r4, r5, r6, lr}
00612058: b        #0x610a1c
0061205c: pop      {r4, r5, r6, lr}
00612060: b        #0x610170
00612064: pop      {r4, r5, r6, lr}
00612068: b        #0x60ffb4
0061206c: pop      {r4, r5, r6, lr}
00612070: b        #0x6119b8
00612074: add      r6, r5, #0xc
00612078: mov      r0, r6
0061207c: bl       #0x30e76c
00612080: cmp      r0, #0
00612084: beq      #0x611fb0
00612088: bl       #0x61110c
0061208c: str      r0, [r5, #0x10]
00612090: bl       #0x61110c
00612094: str      r0, [r5, #0x14]
00612098: bl       #0x6115ac
0061209c: str      r0, [r5, #0x24]
006120a0: bl       #0x6111a0
006120a4: str      r0, [r5, #0x28]
006120a8: bl       #0x611234
006120ac: str      r0, [r5, #0x2c]
006120b0: bl       #0x611640
006120b4: str      r0, [r5, #0x38]
006120b8: bl       #0x6112c8
006120bc: str      r0, [r5, #0x3c]
006120c0: bl       #0x6112c8
006120c4: str      r0, [r5, #0x40]
006120c8: bl       #0x6112c8
006120cc: str      r0, [r5, #0x44]
006120d0: bl       #0x6116d4
006120d4: str      r0, [r5, #0x4c]
006120d8: bl       #0x61135c
006120dc: str      r0, [r5, #0x50]
006120e0: bl       #0x6113f0
006120e4: str      r0, [r5, #0x54]
006120e8: bl       #0x611484
006120ec: str      r0, [r5, #0x58]
006120f0: bl       #0x611518
006120f4: str      r0, [r5, #0x5c]
006120f8: mov      r0, r6
006120fc: bl       #0x30ea3c
00612100: b        #0x611fb0
00612104: eorseq   r2, r8, r4, lsr #31
00612108: andeq    r0, r0, r4, lsr #30
0061210c: andeq    r2, r0, r4, lsl #27
00612110: andeq    r4, r0, r0, lsl #1
00612114: strheq   r2, [r0], -r0
00612118: ldrdeq   r4, r5, [r0], -r0
0061211c: eorseq   r4, lr, r4, ror sp
00612120: mlaeq    sp, r8, sp, r2
00612124: eorseq   r4, lr, r0, lsr sp

# _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
006e3374: push     {r4, r5, r6, lr}
006e3378: mov      r4, r1
006e337c: mov      r1, #0
006e3380: mov      r5, r2
006e3384: mov      r6, r3
006e3388: bl       #0x669e24
006e338c: ldr      r3, [r0, #4]
006e3390: ldr      r4, [r3, r4, lsl #2]
006e3394: ldr      r0, [r3, r5, lsl #2]
006e3398: mov      r1, r4
006e339c: bl       #0x30e3ac
006e33a0: mov      r1, r0
006e33a4: mov      r0, r6
006e33a8: bl       #0x30ed6c
006e33ac: mov      r1, r0
006e33b0: mov      r0, r4
006e33b4: bl       #0x30eba4
006e33b8: ldr      r3, [sp, #0x10]
006e33bc: str      r0, [r3]
006e33c0: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada18SAnimationAccessor9getOutputEi
00669e24: ldm      r0, {r2, r3}
00669e28: mov      r0, #0x1c
00669e2c: ldr      r2, [r2, #8]
00669e30: mla      r2, r0, r1, r2
00669e34: ldr      r2, [r2, #0x18]
00669e38: add      r3, r3, r2, lsl #3
00669e3c: add      r0, r3, #4
00669e40: bx       lr

# _ZN6glitch7collada33CGlitchNewParticleSystemSceneNode26getParticleSystemParameterEPKc
0063f07c: push     {r4, r5, lr}
0063f080: ldr      r3, [r0, #0x178]
0063f084: sub      sp, sp, #0x14
0063f088: ldr      r2, [r3]
0063f08c: ldr      r5, [r2, #-0xc]
0063f090: add      r5, r3, r5
0063f094: mov      r0, r5
0063f098: bl       #0x63b44c
0063f09c: ldr      ip, [r5, #0x34]
0063f0a0: add      r1, r5, #0x30
0063f0a4: mov      r4, r0
0063f0a8: cmp      ip, #0
0063f0ac: moveq    ip, r1
0063f0b0: beq      #0x63f0e0
0063f0b4: mov      r2, r1
0063f0b8: b        #0x63f0c0
0063f0bc: mov      ip, r3
0063f0c0: ldr      r3, [ip, #0x10]
0063f0c4: cmp      r4, r3
0063f0c8: ldrhi    r3, [ip, #0xc]
0063f0cc: ldrls    r3, [ip, #8]
0063f0d0: movhi    ip, r2
0063f0d4: mov      r2, ip
0063f0d8: cmp      r3, #0
0063f0dc: bne      #0x63f0bc
0063f0e0: cmp      r1, ip
0063f0e4: beq      #0x63f0f8
0063f0e8: ldr      r2, [ip, #0x10]
0063f0ec: mov      r3, ip
0063f0f0: cmp      r4, r2
0063f0f4: bhs      #0x63f118
0063f0f8: mov      r3, sp
0063f0fc: mov      lr, #0
0063f100: add      r0, sp, #8
0063f104: add      r2, sp, #0xc
0063f108: stm      sp, {r4, lr}
0063f10c: str      ip, [sp, #0xc]
0063f110: bl       #0x63aa74
0063f114: ldr      r3, [sp, #8]
0063f118: ldr      r0, [r3, #0x14]
0063f11c: add      sp, sp, #0x14
0063f120: pop      {r4, r5, pc}

# _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE7reserveEj
0063a714: push     {r4, r5, r6, lr}
0063a718: mov      r4, r0
0063a71c: ldr      r2, [r0]
0063a720: ldr      r0, [r0, #8]
0063a724: sub      sp, sp, #8
0063a728: str      r1, [sp, #4]
0063a72c: rsb      r0, r2, r0
0063a730: cmp      r1, r0, asr #2
0063a734: bls      #0x63a784
0063a738: cmn      r1, #0xc0000001
0063a73c: bhi      #0x63a78c
0063a740: ldr      r3, [r4, #4]
0063a744: cmp      r2, #0
0063a748: rsb      r5, r2, r3
0063a74c: asr      r5, r5, #2
0063a750: beq      #0x63a7a0
0063a754: mov      r0, r4
0063a758: add      r1, sp, #4
0063a75c: bl       #0x63a6d8
0063a760: mov      r6, r0
0063a764: ldr      r0, [r4]
0063a768: bl       #0x310450
0063a76c: ldr      r3, [sp, #4]
0063a770: add      r5, r6, r5, lsl #2
0063a774: str      r5, [r4, #4]
0063a778: add      r3, r6, r3, lsl #2
0063a77c: str      r3, [r4, #8]
0063a780: str      r6, [r4]
0063a784: add      sp, sp, #8
0063a788: pop      {r4, r5, r6, pc}
0063a78c: ldr      r0, [pc, #0x24]
0063a790: add      r0, pc, r0
0063a794: bl       #0x708e40
0063a798: ldr      r2, [r4]
0063a79c: b        #0x63a740
0063a7a0: ldr      r0, [sp, #4]
0063a7a4: mov      r1, r2
0063a7a8: lsl      r0, r0, #2
0063a7ac: bl       #0x310568
0063a7b0: mov      r6, r0
0063a7b4: b        #0x63a76c

# _ZNK6glitch7collada15animation_track8CFloatEx13getAddedValueEPvPfiS3_
006e3184: push     {r4, r5, r6, r7, r8, sb, sl, lr}
006e3188: ldr      r4, [sp, #0x20]
006e318c: mov      r0, #0
006e3190: subs     r6, r3, #0
006e3194: mov      r5, r1
006e3198: mov      sb, r2
006e319c: str      r0, [r4]
006e31a0: ble      #0x6e31e0
006e31a4: mov      r7, #0
006e31a8: mov      sl, r0
006e31ac: mov      r8, r7
006e31b0: ldr      r1, [sb, r7]
006e31b4: ldr      r0, [r5, r7]
006e31b8: bl       #0x30ed6c
006e31bc: mov      r1, r0
006e31c0: mov      r0, sl
006e31c4: bl       #0x30eba4
006e31c8: add      r8, r8, #1
006e31cc: cmp      r8, r6
006e31d0: mov      sl, r0
006e31d4: str      r0, [r4]
006e31d8: add      r7, r7, #4
006e31dc: bne      #0x6e31b0
006e31e0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

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

# _ZN6glitch7collada24CParticleSystemSceneNodeC1ERKNS0_16CColladaDatabaseERNS0_8SEmitterEPNS_3res6vectorINS7_6StringEEEPNS0_14CRootSceneNodeE
0064ed94: push     {r4, r5, r6, r7, lr}
0064ed98: ldr      r5, [pc, #0x94]
0064ed9c: ldr      ip, [pc, #0x94]
0064eda0: ldr      lr, [pc, #0x94]
0064eda4: add      r5, pc, r5
0064eda8: ldr      ip, [r5, ip]
0064edac: ldr      lr, [r5, lr]
0064edb0: mov      r7, #1
0064edb4: ldr      r6, [ip, #0x24]
0064edb8: add      lr, lr, #8
0064edbc: str      r7, [r0, #0x194]
0064edc0: str      lr, [r0, #0x190]
0064edc4: str      r6, [r0]
0064edc8: ldr      r6, [r6, #-0xc]
0064edcc: ldr      r7, [ip, #0x28]
0064edd0: sub      sp, sp, #0xc
0064edd4: mov      lr, r1
0064edd8: str      r7, [r0, r6]
0064eddc: add      r1, ip, #4
0064ede0: ldr      ip, [sp, #0x20]
0064ede4: mov      r6, r2
0064ede8: mov      r2, lr
0064edec: mov      r4, r0
0064edf0: str      ip, [sp]
0064edf4: bl       #0x667a98
0064edf8: ldr      r3, [pc, #0x40]
0064edfc: str      r6, [r4, #0x18c]
0064ee00: mov      r0, r4
0064ee04: ldr      r3, [r5, r3]
0064ee08: mov      r1, #2
0064ee0c: add      r2, r3, #0x138
0064ee10: add      r3, r3, #0x1c
0064ee14: str      r3, [r4]
0064ee18: str      r2, [r4, #0x190]
0064ee1c: ldr      r3, [r6]
0064ee20: str      r3, [r4, #0x130]
0064ee24: bl       #0x59719c
0064ee28: mov      r0, r4
0064ee2c: add      sp, sp, #0xc
0064ee30: pop      {r4, r5, r6, r7, pc}
0064ee34: eorseq   r5, r4, ip, ror #25
0064ee38: strheq   r1, [r0], -ip
0064ee3c: andeq    r2, r0, r4, asr #22
0064ee40: andeq    r2, r0, ip, asr #26

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

# _ZN6glitch7collada15animation_track8CFloatEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
006e3288: push     {r4, r5, r6, lr}
006e328c: mov      r4, r1
006e3290: mov      r1, #0
006e3294: mov      r5, r2
006e3298: mov      r6, r3
006e329c: bl       #0x669e24
006e32a0: ldr      r3, [r0, #4]
006e32a4: ldr      r1, [r3, r4, lsl #2]
006e32a8: ldr      r0, [r3, r5, lsl #2]
006e32ac: bl       #0x30e3ac
006e32b0: str      r0, [r6]
006e32b4: pop      {r4, r5, r6, pc}

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

# _ZNSt4priv8_Rb_treeIjSt4lessIjESt4pairIKjPvENS_10_Select1stIS6_EENS_11_MapTraitsTIS6_EEN6glitch4core10SAllocatorIS6_LNSB_6memory13E_MEMORY_HINTE0EEEE13insert_uniqueENS_17_Rb_tree_iteratorIS6_SA_EERKS6_
0063aa74: push     {r4, r5, r6, r7, r8, sl, lr}
0063aa78: ldr      r4, [r2]
0063aa7c: ldr      r2, [r1, #8]
0063aa80: sub      sp, sp, #0x2c
0063aa84: mov      r5, r1
0063aa88: cmp      r4, r2
0063aa8c: mov      r7, r0
0063aa90: mov      r6, r3
0063aa94: beq      #0x63ac04
0063aa98: cmp      r4, r1
0063aa9c: beq      #0x63ac84
0063aaa0: ldrb     r3, [r4]
0063aaa4: cmp      r3, #0
0063aaa8: beq      #0x63ab98
0063aaac: ldr      ip, [r4, #8]
0063aab0: cmp      ip, #0
0063aab4: bne      #0x63aac0
0063aab8: b        #0x63abb8
0063aabc: mov      ip, r3
0063aac0: ldr      r3, [ip, #0xc]
0063aac4: cmp      r3, #0
0063aac8: bne      #0x63aabc
0063aacc: ldr      r2, [r6]
0063aad0: ldr      r0, [r4, #0x10]
0063aad4: cmp      r2, r0
0063aad8: movhs    r1, #0
0063aadc: movlo    r1, #1
0063aae0: cmp      r1, #0
0063aae4: bne      #0x63ab58
0063aae8: ldr      r8, [r4, #0xc]
0063aaec: cmp      r8, #0
0063aaf0: beq      #0x63acec
0063aaf4: mov      ip, r8
0063aaf8: b        #0x63ab00
0063aafc: mov      ip, r3
0063ab00: ldr      r3, [ip, #8]
0063ab04: cmp      r3, #0
0063ab08: bne      #0x63aafc
0063ab0c: cmp      r1, #0
0063ab10: bne      #0x63abe8
0063ab14: cmp      r2, r0
0063ab18: bls      #0x63acac
0063ab1c: cmp      r5, ip
0063ab20: beq      #0x63ab30
0063ab24: ldr      r3, [ip, #0x10]
0063ab28: cmp      r2, r3
0063ab2c: bhs      #0x63abe8
0063ab30: cmp      r8, #0
0063ab34: bne      #0x63ac64
0063ab38: mov      r1, r5
0063ab3c: mov      r2, r4
0063ab40: mov      r3, r6
0063ab44: mov      r0, r7
0063ab48: str      r8, [sp]
0063ab4c: str      r4, [sp, #4]
0063ab50: bl       #0x63a7bc
0063ab54: b        #0x63ab8c
0063ab58: ldr      r3, [ip, #0x10]
0063ab5c: cmp      r2, r3
0063ab60: bls      #0x63aae8
0063ab64: ldr      lr, [ip, #0xc]
0063ab68: cmp      lr, #0
0063ab6c: beq      #0x63accc
0063ab70: mov      ip, #0
0063ab74: mov      r1, r5
0063ab78: mov      r2, r4
0063ab7c: mov      r3, r6
0063ab80: mov      r0, r7
0063ab84: stm      sp, {r4, ip}
0063ab88: bl       #0x63a7bc
0063ab8c: mov      r0, r7
0063ab90: add      sp, sp, #0x2c
0063ab94: pop      {r4, r5, r6, r7, r8, sl, pc}
0063ab98: ldr      r3, [r4, #4]
0063ab9c: ldr      r3, [r3, #4]
0063aba0: cmp      r4, r3
0063aba4: ldreq    ip, [r4, #0xc]
0063aba8: beq      #0x63aacc
0063abac: ldr      ip, [r4, #8]
0063abb0: cmp      ip, #0
0063abb4: bne      #0x63aac0
0063abb8: ldr      ip, [r4, #4]
0063abbc: ldr      r3, [ip, #8]
0063abc0: cmp      r4, r3
0063abc4: beq      #0x63abd0
0063abc8: b        #0x63aacc
0063abcc: mov      ip, r3
0063abd0: ldr      r3, [ip, #4]
0063abd4: ldr      r2, [r3, #8]
0063abd8: cmp      r2, ip
0063abdc: beq      #0x63abcc
0063abe0: mov      ip, r3
0063abe4: b        #0x63aacc
0063abe8: mov      r1, r5
0063abec: mov      r2, r6
0063abf0: add      r0, sp, #8
0063abf4: bl       #0x63a8ec
0063abf8: ldr      r3, [sp, #8]
0063abfc: str      r3, [r7]
0063ac00: b        #0x63ab8c
0063ac04: ldr      r2, [r1, #0x10]
0063ac08: cmp      r2, #0
0063ac0c: beq      #0x63ad5c
0063ac10: ldr      r2, [r3]
0063ac14: ldr      ip, [r4, #0x10]
0063ac18: cmp      r2, ip
0063ac1c: blo      #0x63ad74
0063ac20: bls      #0x63acac
0063ac24: ldr      lr, [r4, #0xc]
0063ac28: cmp      lr, #0
0063ac2c: beq      #0x63ad24
0063ac30: mov      ip, lr
0063ac34: b        #0x63ac3c
0063ac38: mov      ip, r3
0063ac3c: ldr      r3, [ip, #8]
0063ac40: cmp      r3, #0
0063ac44: bne      #0x63ac38
0063ac48: cmp      r5, ip
0063ac4c: beq      #0x63adc4
0063ac50: ldr      r3, [ip, #0x10]
0063ac54: cmp      r2, r3
0063ac58: bhs      #0x63ad88
0063ac5c: cmp      lr, #0
0063ac60: beq      #0x63ada4
0063ac64: mov      lr, #0
0063ac68: mov      r1, r5
0063ac6c: mov      r2, ip
0063ac70: mov      r3, r6
0063ac74: mov      r0, r7
0063ac78: stm      sp, {ip, lr}
0063ac7c: bl       #0x63a7bc
0063ac80: b        #0x63ab8c
0063ac84: ldr      r2, [r4, #0xc]
0063ac88: ldr      ip, [r3]
0063ac8c: ldr      lr, [r2, #0x10]
0063ac90: cmp      lr, ip
0063ac94: bhs      #0x63acb4
0063ac98: mov      ip, #0
0063ac9c: str      ip, [sp]
0063aca0: str      r4, [sp, #4]
0063aca4: bl       #0x63a7bc
0063aca8: b        #0x63ab8c
0063acac: str      r4, [r7]
0063acb0: b        #0x63ab8c
0063acb4: mov      r2, r3
0063acb8: add      r0, sp, #0x10
0063acbc: bl       #0x63a8ec
0063acc0: ldr      r3, [sp, #0x10]
0063acc4: str      r3, [r7]
0063acc8: b        #0x63ab8c
0063accc: mov      r1, r5
0063acd0: mov      r2, ip
0063acd4: mov      r3, r6
0063acd8: mov      r0, r7
0063acdc: str      lr, [sp]
0063ace0: str      ip, [sp, #4]
0063ace4: bl       #0x63a7bc
0063ace8: b        #0x63ab8c
0063acec: ldr      r3, [r4, #4]
0063acf0: ldr      ip, [r3, #0xc]
0063acf4: cmp      r4, ip
0063acf8: movne    ip, r4
0063acfc: bne      #0x63ad14
0063ad00: mov      ip, r3
0063ad04: ldr      r3, [r3, #4]
0063ad08: ldr      sl, [r3, #0xc]
0063ad0c: cmp      ip, sl
0063ad10: beq      #0x63ad00
0063ad14: ldr      sl, [ip, #0xc]
0063ad18: cmp      r3, sl
0063ad1c: movne    ip, r3
0063ad20: b        #0x63ab0c
0063ad24: ldr      r3, [r4, #4]
0063ad28: ldr      r1, [r3, #0xc]
0063ad2c: cmp      r4, r1
0063ad30: movne    ip, r4
0063ad34: bne      #0x63ad4c
0063ad38: mov      ip, r3
0063ad3c: ldr      r3, [r3, #4]
0063ad40: ldr      r1, [r3, #0xc]
0063ad44: cmp      r1, ip
0063ad48: beq      #0x63ad38
0063ad4c: ldr      r1, [ip, #0xc]
0063ad50: cmp      r3, r1
0063ad54: movne    ip, r3
0063ad58: b        #0x63ac48
0063ad5c: mov      r2, r3
0063ad60: add      r0, sp, #0x20
0063ad64: bl       #0x63a8ec
0063ad68: ldr      r3, [sp, #0x20]
0063ad6c: str      r3, [r7]
0063ad70: b        #0x63ab8c
0063ad74: mov      ip, #0
0063ad78: mov      r2, r4
0063ad7c: stm      sp, {r4, ip}
0063ad80: bl       #0x63a7bc
0063ad84: b        #0x63ab8c
0063ad88: mov      r1, r5
0063ad8c: mov      r2, r6
0063ad90: add      r0, sp, #0x18
0063ad94: bl       #0x63a8ec
0063ad98: ldr      r3, [sp, #0x18]
0063ad9c: str      r3, [r7]
0063ada0: b        #0x63ab8c
0063ada4: mov      r1, r5
0063ada8: mov      r2, r4
0063adac: mov      r3, r6
0063adb0: mov      r0, r7
0063adb4: str      lr, [sp]
0063adb8: str      r4, [sp, #4]
0063adbc: bl       #0x63a7bc
0063adc0: b        #0x63ab8c
0063adc4: mov      ip, #0
0063adc8: mov      r1, r5
0063adcc: mov      r2, r4
0063add0: mov      r3, r6
0063add4: mov      r0, r7
0063add8: str      ip, [sp]
0063addc: str      r4, [sp, #4]
0063ade0: bl       #0x63a7bc
0063ade4: b        #0x63ab8c

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

# _ZN6glitch7collada14CRootSceneNode17getParticleSystemEPKc
0065b4e4: push     {r4, r5, r6, lr}
0065b4e8: mov      r5, r0
0065b4ec: mov      r6, r1
0065b4f0: ldr      r4, [r5, #0x158]!
0065b4f4: b        #0x65b520
0065b4f8: ldr      r3, [r4, #8]
0065b4fc: mov      r0, r3
0065b500: ldr      r3, [r3]
0065b504: mov      lr, pc
0065b508: ldr      pc, [r3, #0x54]
0065b50c: mov      r1, r6
0065b510: bl       #0x30e6e8
0065b514: cmp      r0, #0
0065b518: beq      #0x65b530
0065b51c: ldr      r4, [r4]
0065b520: cmp      r5, r4
0065b524: bne      #0x65b4f8
0065b528: mov      r0, #0
0065b52c: pop      {r4, r5, r6, pc}
0065b530: ldr      r0, [r4, #8]
0065b534: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada18ISceneNodeAnimator9forceBindEv
00667f18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00667f1c: ldr      r6, [r0, #0x10]
00667f20: ldr      sl, [pc, #0xeec]
00667f24: sub      sp, sp, #0x24
00667f28: cmp      r6, #0
00667f2c: mov      r4, r0
00667f30: add      sl, pc, sl
00667f34: beq      #0x6681e4
00667f38: ldr      r3, [r0]
00667f3c: mov      lr, pc
00667f40: ldr      pc, [r3, #0x70]
00667f44: subs     r7, r0, #0
00667f48: ble      #0x6681e4
00667f4c: ldr      r3, [pc, #0xec4]
00667f50: ldr      fp, [pc, #0xec4]
00667f54: mov      r5, #0
00667f58: add      r3, pc, r3
00667f5c: str      r3, [sp]
00667f60: ldr      r3, [pc, #0xeb8]
00667f64: add      r3, pc, r3
00667f68: str      r3, [sp, #4]
00667f6c: ldr      r3, [pc, #0xeb0]
00667f70: add      r3, pc, r3
00667f74: str      r3, [sp, #8]
00667f78: ldr      r3, [pc, #0xea8]
00667f7c: add      r3, pc, r3
00667f80: str      r3, [sp, #0xc]
00667f84: mov      r1, r5
00667f88: ldr      r3, [r4]
00667f8c: mov      r0, r4
00667f90: mov      lr, pc
00667f94: ldr      pc, [r3, #0x6c]
00667f98: ldr      r3, [r4]
00667f9c: mov      sb, r0
00667fa0: mov      r1, r5
00667fa4: mov      r0, r4
00667fa8: mov      lr, pc
00667fac: ldr      pc, [r3, #0x54]
00667fb0: ldr      r3, [r0, #8]
00667fb4: sub      r3, r3, #1
00667fb8: cmp      r3, #0x5a
00667fbc: addls    pc, pc, r3, lsl #2
00667fc0: b        #0x6681ec
00667fc4: b        #0x66820c
00667fc8: b        #0x66820c
00667fcc: b        #0x66820c
00667fd0: b        #0x66820c
00667fd4: b        #0x66820c
00667fd8: b        #0x6681ec
00667fdc: b        #0x6681ec
00667fe0: b        #0x6681ec
00667fe4: b        #0x66820c
00667fe8: b        #0x66820c
00667fec: b        #0x66820c
00667ff0: b        #0x66820c
00667ff4: b        #0x66820c
00667ff8: b        #0x668258
00667ffc: b        #0x6682ac
00668000: b        #0x6682cc
00668004: b        #0x6681ec
00668008: b        #0x6681ec
0066800c: b        #0x6681ec
00668010: b        #0x66820c
00668014: b        #0x6681ec
00668018: b        #0x6681ec
0066801c: b        #0x6681ec
00668020: b        #0x6681ec
00668024: b        #0x6681ec
00668028: b        #0x66830c
0066802c: b        #0x6681ec
00668030: b        #0x668354
00668034: b        #0x66839c
00668038: b        #0x6683e4
0066803c: b        #0x66842c
00668040: b        #0x668474
00668044: b        #0x6684bc
00668048: b        #0x668504
0066804c: b        #0x66854c
00668050: b        #0x668594
00668054: b        #0x6685dc
00668058: b        #0x668624
0066805c: b        #0x66866c
00668060: b        #0x6686b4
00668064: b        #0x6686fc
00668068: b        #0x668744
0066806c: b        #0x66878c
00668070: b        #0x6687d4
00668074: b        #0x66881c
00668078: b        #0x668864
0066807c: b        #0x6688ac
00668080: b        #0x6688f4
00668084: b        #0x66893c
00668088: b        #0x668984
0066808c: b        #0x6689cc
00668090: b        #0x668a14
00668094: b        #0x668a5c
00668098: b        #0x668aac
0066809c: b        #0x668af4
006680a0: b        #0x668b3c
006680a4: b        #0x668b8c
006680a8: b        #0x668bd4
006680ac: b        #0x668c1c
006680b0: b        #0x668c64
006680b4: b        #0x668cac
006680b8: b        #0x668cf4
006680bc: b        #0x668d3c
006680c0: b        #0x668d84
006680c4: b        #0x668dcc
006680c8: b        #0x668ecc
006680cc: b        #0x668f1c
006680d0: b        #0x668f64
006680d4: b        #0x668fac
006680d8: b        #0x668ff0
006680dc: b        #0x6681ec
006680e0: b        #0x6681ec
006680e4: b        #0x6681ec
006680e8: b        #0x6681ec
006680ec: b        #0x6681ec
006680f0: b        #0x6681ec
006680f4: b        #0x6681ec
006680f8: b        #0x6681ec
006680fc: b        #0x6681ec
00668100: b        #0x6681ec
00668104: b        #0x6681ec
00668108: b        #0x6681ec
0066810c: b        #0x6681ec
00668110: b        #0x6681ec
00668114: b        #0x6681ec
00668118: b        #0x668130
0066811c: b        #0x668130
00668120: b        #0x668130
00668124: b        #0x668130
00668128: b        #0x668130
0066812c: b        #0x668130
00668130: add      r8, sp, #0x1c
00668134: mov      r2, sb
00668138: mov      r0, r8
0066813c: mov      r1, r6
00668140: mov      r3, #0
00668144: bl       #0x65ca30
00668148: ldr      r2, [sp, #0x1c]
0066814c: cmp      r2, #0
00668150: beq      #0x6694a4
00668154: mov      r1, r5
00668158: ldr      r3, [r4]
0066815c: mov      r0, r4
00668160: mov      lr, pc
00668164: ldr      pc, [r3, #0x54]
00668168: ldr      r3, [pc, #0xcbc]
0066816c: mov      r2, #0
00668170: mvn      r1, #0
00668174: ldr      r3, [sl, r3]
00668178: str      r2, [sp, #0x14]
0066817c: str      r1, [sp, #0x18]
00668180: add      r3, r3, #8
00668184: str      r3, [sp, #0x10]
00668188: ldr      r3, [r0, #0xc]
0066818c: str      r3, [sp, #0x14]
00668190: ldr      r3, [sp, #0x1c]
00668194: ldr      r1, [r0, #0xc]
00668198: ldr      r0, [r3, #4]
0066819c: bl       #0x5d308c
006681a0: str      r0, [sp, #0x18]
006681a4: add      r3, sp, #0x10
006681a8: mov      r0, r4
006681ac: ldr      ip, [r4]
006681b0: mov      r1, r5
006681b4: ldr      r2, [sp, #0x1c]
006681b8: mov      lr, pc
006681bc: ldr      pc, [ip, #0x68]
006681c0: ldr      r3, [pc, #0xc68]
006681c4: mov      r0, r8
006681c8: ldr      r3, [sl, r3]
006681cc: add      r3, r3, #8
006681d0: str      r3, [sp, #0x10]
006681d4: bl       #0x310be8
006681d8: add      r5, r5, #1
006681dc: cmp      r5, r7
006681e0: bne      #0x667f84
006681e4: add      sp, sp, #0x24
006681e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006681ec: mov      r2, #0
006681f0: ldr      ip, [r4]
006681f4: mov      r0, r4
006681f8: mov      r1, r5
006681fc: mov      r3, r2
00668200: mov      lr, pc
00668204: ldr      pc, [ip, #0x68]
00668208: b        #0x6681d8
0066820c: mov      r1, sb
00668210: mov      r0, r6
00668214: bl       #0x5985e4
00668218: mov      r8, r0
0066821c: ldr      ip, [r4]
00668220: mov      r0, r4
00668224: mov      r1, r5
00668228: mov      r2, r8
0066822c: mov      r3, #0
00668230: mov      lr, pc
00668234: ldr      pc, [ip, #0x68]
00668238: cmp      r8, #0
0066823c: beq      #0x6681d8
00668240: mov      r0, r8
00668244: ldr      r3, [r8]
00668248: mov      r1, r4
0066824c: mov      lr, pc
00668250: ldr      pc, [r3, #0x78]
00668254: b        #0x6681d8
00668258: mov      r1, sb
0066825c: mov      r0, r6
00668260: bl       #0x65b5b0
00668264: subs     r8, r0, #0
00668268: beq      #0x669484
0066826c: mov      r1, r5
00668270: ldr      r3, [r4]
00668274: mov      r0, r4
00668278: mov      lr, pc
0066827c: ldr      pc, [r3, #0x54]
00668280: ldr      r3, [r8, #0x24]
00668284: ldrb     r2, [r0, #0xc]
00668288: ldr      ip, [r4]
0066828c: mov      r0, r4
00668290: add      r2, r3, r2, lsl #3
00668294: add      r2, r2, #0xc
00668298: mov      r1, r5
0066829c: mov      r3, #0
006682a0: mov      lr, pc
006682a4: ldr      pc, [ip, #0x68]
006682a8: b        #0x6681d8
006682ac: ldr      ip, [r4]
006682b0: mov      r0, r4
006682b4: mov      r1, r5
006682b8: mov      r2, r6
006682bc: mov      r3, #0
006682c0: mov      lr, pc
006682c4: ldr      pc, [ip, #0x68]
006682c8: b        #0x6681d8
006682cc: mov      r1, sb
006682d0: mov      r0, r6
006682d4: bl       #0x65b5f4
006682d8: subs     r2, r0, #0
006682dc: ldreq    ip, [r4]
006682e0: moveq    r0, r4
006682e4: moveq    r1, r5
006682e8: moveq    r3, r2
006682ec: ldrne    r2, [r2, #0x134]
006682f0: ldrne    ip, [r4]
006682f4: movne    r0, r4
006682f8: movne    r1, r5
006682fc: movne    r3, #0
00668300: mov      lr, pc
00668304: ldr      pc, [ip, #0x68]
00668308: b        #0x6681d8
0066830c: add      r8, sp, #0x1c
00668310: mov      r3, #0
00668314: mov      r2, sb
00668318: mov      r0, r8
0066831c: mov      r1, r6
00668320: bl       #0x65ca30
00668324: ldr      r2, [sp, #0x1c]
00668328: mov      r0, r4
0066832c: ldr      ip, [r4]
00668330: cmp      r2, #0
00668334: mov      r1, r5
00668338: moveq    r3, r2
0066833c: movne    r3, #0
00668340: mov      lr, pc
00668344: ldr      pc, [ip, #0x68]
00668348: mov      r0, r8
0066834c: bl       #0x310be8
00668350: b        #0x6681d8
00668354: mov      r1, sb
00668358: mov      r0, r6
0066835c: bl       #0x65b4e4
00668360: subs     r2, r0, #0
00668364: beq      #0x669468
00668368: ldr      r1, [pc, #0xac4]
0066836c: ldr      ip, [r4]
00668370: ldr      r3, [r2]
00668374: add      r1, pc, r1
00668378: ldr      r8, [ip, #0x68]
0066837c: mov      lr, pc
00668380: ldr      pc, [r3, #0xfc]
00668384: mov      r1, r5
00668388: mov      r2, r0
0066838c: mov      r3, #0
00668390: mov      r0, r4
00668394: blx      r8
00668398: b        #0x6681d8
0066839c: mov      r1, sb
006683a0: mov      r0, r6
006683a4: bl       #0x65b4e4
006683a8: subs     r2, r0, #0
006683ac: beq      #0x66944c
006683b0: ldr      r1, [pc, #0xa80]
006683b4: ldr      ip, [r4]
006683b8: ldr      r3, [r2]
006683bc: add      r1, pc, r1
006683c0: ldr      r8, [ip, #0x68]
006683c4: mov      lr, pc
006683c8: ldr      pc, [r3, #0xfc]
006683cc: mov      r1, r5
006683d0: mov      r2, r0
006683d4: mov      r3, #0
006683d8: mov      r0, r4
006683dc: blx      r8
006683e0: b        #0x6681d8
006683e4: mov      r1, sb
006683e8: mov      r0, r6
006683ec: bl       #0x65b4e4
006683f0: subs     r2, r0, #0
006683f4: beq      #0x669430
006683f8: ldr      r1, [pc, #0xa3c]
006683fc: ldr      ip, [r4]
00668400: ldr      r3, [r2]
00668404: add      r1, pc, r1
00668408: ldr      r8, [ip, #0x68]
0066840c: mov      lr, pc
00668410: ldr      pc, [r3, #0xfc]
00668414: mov      r1, r5
00668418: mov      r2, r0
0066841c: mov      r3, #0
00668420: mov      r0, r4
00668424: blx      r8
00668428: b        #0x6681d8
0066842c: mov      r1, sb
00668430: mov      r0, r6
00668434: bl       #0x65b4e4
00668438: subs     r2, r0, #0
0066843c: beq      #0x669414
00668440: ldr      r1, [pc, #0x9f8]
00668444: ldr      ip, [r4]
00668448: ldr      r3, [r2]
0066844c: add      r1, pc, r1
00668450: ldr      r8, [ip, #0x68]
00668454: mov      lr, pc
00668458: ldr      pc, [r3, #0xfc]
0066845c: mov      r1, r5
00668460: mov      r2, r0
00668464: mov      r3, #0
00668468: mov      r0, r4
0066846c: blx      r8
00668470: b        #0x6681d8
00668474: mov      r1, sb
00668478: mov      r0, r6
0066847c: bl       #0x65b4e4
00668480: subs     r2, r0, #0
00668484: beq      #0x6693f8
00668488: ldr      r1, [pc, #0x9b4]
0066848c: ldr      ip, [r4]
00668490: ldr      r3, [r2]
00668494: add      r1, pc, r1
00668498: ldr      r8, [ip, #0x68]
0066849c: mov      lr, pc
006684a0: ldr      pc, [r3, #0xfc]
006684a4: mov      r1, r5
006684a8: mov      r2, r0
006684ac: mov      r3, #0
006684b0: mov      r0, r4
006684b4: blx      r8
006684b8: b        #0x6681d8
006684bc: mov      r1, sb
006684c0: mov      r0, r6
006684c4: bl       #0x65b4e4
006684c8: subs     r2, r0, #0
006684cc: beq      #0x6693dc
006684d0: ldr      r1, [pc, #0x970]
006684d4: ldr      ip, [r4]
006684d8: ldr      r3, [r2]
006684dc: add      r1, pc, r1
006684e0: ldr      r8, [ip, #0x68]
006684e4: mov      lr, pc
006684e8: ldr      pc, [r3, #0xfc]
006684ec: mov      r1, r5
006684f0: mov      r2, r0
006684f4: mov      r3, #0
006684f8: mov      r0, r4
006684fc: blx      r8
00668500: b        #0x6681d8
00668504: mov      r1, sb
00668508: mov      r0, r6
0066850c: bl       #0x65b4e4
00668510: subs     r2, r0, #0
00668514: beq      #0x6693c0
00668518: ldr      r1, [pc, #0x92c]
0066851c: ldr      ip, [r4]
00668520: ldr      r3, [r2]
00668524: add      r1, pc, r1
00668528: ldr      r8, [ip, #0x68]
0066852c: mov      lr, pc
00668530: ldr      pc, [r3, #0xfc]
00668534: mov      r1, r5
00668538: mov      r2, r0
0066853c: mov      r3, #0
00668540: mov      r0, r4
00668544: blx      r8
00668548: b        #0x6681d8
0066854c: mov      r1, sb
00668550: mov      r0, r6
00668554: bl       #0x65b4e4
00668558: subs     r2, r0, #0
0066855c: beq      #0x6693a4
00668560: ldr      r1, [pc, #0x8e8]
00668564: ldr      ip, [r4]
00668568: ldr      r3, [r2]
0066856c: add      r1, pc, r1
00668570: ldr      r8, [ip, #0x68]
00668574: mov      lr, pc
00668578: ldr      pc, [r3, #0xfc]
0066857c: mov      r1, r5
00668580: mov      r2, r0
00668584: mov      r3, #0
00668588: mov      r0, r4
0066858c: blx      r8
00668590: b        #0x6681d8
00668594: mov      r1, sb
00668598: mov      r0, r6
0066859c: bl       #0x65b4e4
006685a0: subs     r2, r0, #0
006685a4: beq      #0x669388
006685a8: ldr      r1, [pc, #0x8a4]
006685ac: ldr      ip, [r4]
006685b0: ldr      r3, [r2]
006685b4: add      r1, pc, r1
006685b8: ldr      r8, [ip, #0x68]
006685bc: mov      lr, pc
006685c0: ldr      pc, [r3, #0xfc]
006685c4: mov      r1, r5
006685c8: mov      r2, r0
006685cc: mov      r3, #0
006685d0: mov      r0, r4
006685d4: blx      r8
006685d8: b        #0x6681d8
006685dc: mov      r1, sb
006685e0: mov      r0, r6
006685e4: bl       #0x65b4e4
006685e8: subs     r2, r0, #0
006685ec: beq      #0x66936c
006685f0: ldr      r1, [pc, #0x860]
006685f4: ldr      ip, [r4]
006685f8: ldr      r3, [r2]
006685fc: add      r1, pc, r1
00668600: ldr      r8, [ip, #0x68]
00668604: mov      lr, pc
00668608: ldr      pc, [r3, #0xfc]
0066860c: mov      r1, r5
00668610: mov      r2, r0
00668614: mov      r3, #0
00668618: mov      r0, r4
0066861c: blx      r8
00668620: b        #0x6681d8
00668624: mov      r1, sb
00668628: mov      r0, r6
0066862c: bl       #0x65b4e4
00668630: subs     r2, r0, #0
00668634: beq      #0x669350
00668638: ldr      r1, [pc, #0x81c]
0066863c: ldr      ip, [r4]
00668640: ldr      r3, [r2]
00668644: add      r1, pc, r1
00668648: ldr      r8, [ip, #0x68]
0066864c: mov      lr, pc
00668650: ldr      pc, [r3, #0xfc]
00668654: mov      r1, r5
00668658: mov      r2, r0
0066865c: mov      r3, #0
00668660: mov      r0, r4
00668664: blx      r8
00668668: b        #0x6681d8
0066866c: mov      r1, sb
00668670: mov      r0, r6
00668674: bl       #0x65b4e4
00668678: subs     r2, r0, #0
0066867c: beq      #0x669334
00668680: ldr      r1, [pc, #0x7d8]
00668684: ldr      ip, [r4]
00668688: ldr      r3, [r2]
0066868c: add      r1, pc, r1
00668690: ldr      r8, [ip, #0x68]
00668694: mov      lr, pc
00668698: ldr      pc, [r3, #0xfc]
0066869c: mov      r1, r5
006686a0: mov      r2, r0
006686a4: mov      r3, #0
006686a8: mov      r0, r4
006686ac: blx      r8
006686b0: b        #0x6681d8
006686b4: mov      r1, sb
006686b8: mov      r0, r6
006686bc: bl       #0x65b4e4
006686c0: subs     r2, r0, #0
006686c4: beq      #0x669318
006686c8: ldr      r1, [pc, #0x794]
006686cc: ldr      ip, [r4]
006686d0: ldr      r3, [r2]
006686d4: add      r1, pc, r1
006686d8: ldr      r8, [ip, #0x68]
006686dc: mov      lr, pc
006686e0: ldr      pc, [r3, #0xfc]
006686e4: mov      r1, r5
006686e8: mov      r2, r0
006686ec: mov      r3, #0
006686f0: mov      r0, r4
006686f4: blx      r8
006686f8: b        #0x6681d8
006686fc: mov      r1, sb
00668700: mov      r0, r6
00668704: bl       #0x65b4e4
00668708: subs     r2, r0, #0
0066870c: beq      #0x6692fc
00668710: ldr      r1, [pc, #0x750]
00668714: ldr      ip, [r4]
00668718: ldr      r3, [r2]
0066871c: add      r1, pc, r1
00668720: ldr      r8, [ip, #0x68]
00668724: mov      lr, pc
00668728: ldr      pc, [r3, #0xfc]
0066872c: mov      r1, r5
00668730: mov      r2, r0
00668734: mov      r3, #0
00668738: mov      r0, r4
0066873c: blx      r8
00668740: b        #0x6681d8
00668744: mov      r1, sb
00668748: mov      r0, r6
0066874c: bl       #0x65b4e4
00668750: subs     r2, r0, #0
00668754: beq      #0x6692e0
00668758: ldr      r1, [pc, #0x70c]
0066875c: ldr      ip, [r4]
00668760: ldr      r3, [r2]
00668764: add      r1, pc, r1
00668768: ldr      r8, [ip, #0x68]
0066876c: mov      lr, pc
00668770: ldr      pc, [r3, #0xfc]
00668774: mov      r1, r5
00668778: mov      r2, r0
0066877c: mov      r3, #0
00668780: mov      r0, r4
00668784: blx      r8
00668788: b        #0x6681d8
0066878c: mov      r1, sb
00668790: mov      r0, r6
00668794: bl       #0x65b4e4
00668798: subs     r2, r0, #0
0066879c: beq      #0x6692c4
006687a0: ldr      r1, [pc, #0x6c8]
006687a4: ldr      ip, [r4]
006687a8: ldr      r3, [r2]
006687ac: add      r1, pc, r1
006687b0: ldr      r8, [ip, #0x68]
006687b4: mov      lr, pc
006687b8: ldr      pc, [r3, #0xfc]
006687bc: mov      r1, r5
006687c0: mov      r2, r0
006687c4: mov      r3, #0
006687c8: mov      r0, r4
006687cc: blx      r8
006687d0: b        #0x6681d8
006687d4: mov      r1, sb
006687d8: mov      r0, r6
006687dc: bl       #0x65b4e4
006687e0: subs     r2, r0, #0
006687e4: beq      #0x6692a8
006687e8: ldr      r1, [pc, #0x684]
006687ec: ldr      ip, [r4]
006687f0: ldr      r3, [r2]
006687f4: add      r1, pc, r1
006687f8: ldr      r8, [ip, #0x68]
006687fc: mov      lr, pc
00668800: ldr      pc, [r3, #0xfc]
00668804: mov      r1, r5
00668808: mov      r2, r0
0066880c: mov      r3, #0
00668810: mov      r0, r4
00668814: blx      r8
00668818: b        #0x6681d8
0066881c: mov      r1, sb
00668820: mov      r0, r6
00668824: bl       #0x65b4e4
00668828: subs     r2, r0, #0
0066882c: beq      #0x66928c
00668830: ldr      r1, [pc, #0x640]
00668834: ldr      ip, [r4]
00668838: ldr      r3, [r2]
0066883c: add      r1, pc, r1
00668840: ldr      r8, [ip, #0x68]
00668844: mov      lr, pc
00668848: ldr      pc, [r3, #0xfc]
0066884c: mov      r1, r5
00668850: mov      r2, r0
00668854: mov      r3, #0
00668858: mov      r0, r4
0066885c: blx      r8
00668860: b        #0x6681d8
00668864: mov      r1, sb
00668868: mov      r0, r6
0066886c: bl       #0x65b4e4
00668870: subs     r2, r0, #0
00668874: beq      #0x669270
00668878: ldr      r1, [pc, #0x5fc]
0066887c: ldr      ip, [r4]
00668880: ldr      r3, [r2]
00668884: add      r1, pc, r1
00668888: ldr      r8, [ip, #0x68]
0066888c: mov      lr, pc
00668890: ldr      pc, [r3, #0xfc]
00668894: mov      r1, r5
00668898: mov      r2, r0
0066889c: mov      r3, #0
006688a0: mov      r0, r4
006688a4: blx      r8
006688a8: b        #0x6681d8
006688ac: mov      r1, sb
006688b0: mov      r0, r6
006688b4: bl       #0x65b4e4
006688b8: subs     r2, r0, #0
006688bc: beq      #0x669254
006688c0: ldr      r1, [pc, #0x5b8]
006688c4: ldr      ip, [r4]
006688c8: ldr      r3, [r2]
006688cc: add      r1, pc, r1
006688d0: ldr      r8, [ip, #0x68]
006688d4: mov      lr, pc
006688d8: ldr      pc, [r3, #0xfc]
006688dc: mov      r1, r5
006688e0: mov      r2, r0
006688e4: mov      r3, #0
006688e8: mov      r0, r4
006688ec: blx      r8
006688f0: b        #0x6681d8
006688f4: mov      r1, sb
006688f8: mov      r0, r6
006688fc: bl       #0x65b4e4
00668900: subs     r2, r0, #0
00668904: beq      #0x669238
00668908: ldr      r1, [pc, #0x574]
0066890c: ldr      ip, [r4]
00668910: ldr      r3, [r2]
00668914: add      r1, pc, r1
00668918: ldr      r8, [ip, #0x68]
0066891c: mov      lr, pc
00668920: ldr      pc, [r3, #0xfc]
00668924: mov      r1, r5
00668928: mov      r2, r0
0066892c: mov      r3, #0
00668930: mov      r0, r4
00668934: blx      r8
00668938: b        #0x6681d8
0066893c: mov      r1, sb
00668940: mov      r0, r6
00668944: bl       #0x65b4e4
00668948: subs     r2, r0, #0
0066894c: beq      #0x66921c
00668950: ldr      r1, [pc, #0x530]
00668954: ldr      ip, [r4]
00668958: ldr      r3, [r2]
0066895c: add      r1, pc, r1
00668960: ldr      r8, [ip, #0x68]
00668964: mov      lr, pc
00668968: ldr      pc, [r3, #0xfc]
0066896c: mov      r1, r5
00668970: mov      r2, r0
00668974: mov      r3, #0
00668978: mov      r0, r4
0066897c: blx      r8
00668980: b        #0x6681d8
00668984: mov      r1, sb
00668988: mov      r0, r6
0066898c: bl       #0x65b4e4
00668990: subs     r2, r0, #0
00668994: beq      #0x669200
00668998: ldr      r1, [pc, #0x4ec]
0066899c: ldr      ip, [r4]
006689a0: ldr      r3, [r2]
006689a4: add      r1, pc, r1
006689a8: ldr      r8, [ip, #0x68]
006689ac: mov      lr, pc
006689b0: ldr      pc, [r3, #0xfc]
006689b4: mov      r1, r5
006689b8: mov      r2, r0
006689bc: mov      r3, #0
006689c0: mov      r0, r4
006689c4: blx      r8
006689c8: b        #0x6681d8
006689cc: mov      r1, sb
006689d0: mov      r0, r6
006689d4: bl       #0x65b4e4
006689d8: subs     r2, r0, #0
006689dc: beq      #0x6691e4
006689e0: ldr      r1, [pc, #0x4a8]
006689e4: ldr      ip, [r4]
006689e8: ldr      r3, [r2]
006689ec: add      r1, pc, r1
006689f0: ldr      r8, [ip, #0x68]
006689f4: mov      lr, pc
006689f8: ldr      pc, [r3, #0xfc]
006689fc: mov      r1, r5
00668a00: mov      r2, r0
00668a04: mov      r3, #0
00668a08: mov      r0, r4
00668a0c: blx      r8
00668a10: b        #0x6681d8
00668a14: mov      r1, sb
00668a18: mov      r0, r6
00668a1c: bl       #0x65b4e4
00668a20: subs     r2, r0, #0
00668a24: beq      #0x6691c8
00668a28: ldr      r1, [pc, #0x464]
00668a2c: ldr      ip, [r4]
00668a30: ldr      r3, [r2]
00668a34: add      r1, pc, r1
00668a38: ldr      r8, [ip, #0x68]
00668a3c: mov      lr, pc
00668a40: ldr      pc, [r3, #0xfc]
00668a44: mov      r1, r5
00668a48: mov      r2, r0
00668a4c: mov      r3, #0
00668a50: mov      r0, r4
00668a54: blx      r8
00668a58: b        #0x6681d8
00668a5c: mov      r1, sb
00668a60: mov      r0, r6
00668a64: bl       #0x65b4e4
00668a68: subs     r2, r0, #0
00668a6c: beq      #0x66951c
00668a70: ldrb     r8, [r2, #0x13c]
00668a74: cmp      r8, #0
00668a78: bne      #0x6681d8
00668a7c: ldr      ip, [r4]
00668a80: ldr      r3, [r2]
00668a84: ldr      r1, [sp, #0xc]
00668a88: ldr      sb, [ip, #0x68]
00668a8c: mov      lr, pc
00668a90: ldr      pc, [r3, #0xfc]
00668a94: mov      r1, r5
00668a98: mov      r2, r0
00668a9c: mov      r3, r8
00668aa0: mov      r0, r4
00668aa4: blx      sb
00668aa8: b        #0x6681d8
00668aac: mov      r1, sb
00668ab0: mov      r0, r6
00668ab4: bl       #0x65b4e4
00668ab8: subs     r2, r0, #0
00668abc: beq      #0x669040
00668ac0: ldr      r1, [pc, #0x3d0]
00668ac4: ldr      ip, [r4]
00668ac8: ldr      r3, [r2]
00668acc: add      r1, pc, r1
00668ad0: ldr      r8, [ip, #0x68]
00668ad4: mov      lr, pc
00668ad8: ldr      pc, [r3, #0xfc]
00668adc: mov      r1, r5
00668ae0: mov      r2, r0
00668ae4: mov      r3, #0
00668ae8: mov      r0, r4
00668aec: blx      r8
00668af0: b        #0x6681d8
00668af4: mov      r1, sb
00668af8: mov      r0, r6
00668afc: bl       #0x65b4e4
00668b00: subs     r2, r0, #0
00668b04: beq      #0x6691ac
00668b08: ldr      r1, [pc, #0x38c]
00668b0c: ldr      ip, [r4]
00668b10: ldr      r3, [r2]
00668b14: add      r1, pc, r1
00668b18: ldr      r8, [ip, #0x68]
00668b1c: mov      lr, pc
00668b20: ldr      pc, [r3, #0xfc]
00668b24: mov      r1, r5
00668b28: mov      r2, r0
00668b2c: mov      r3, #0
00668b30: mov      r0, r4
00668b34: blx      r8
00668b38: b        #0x6681d8
00668b3c: mov      r1, sb
00668b40: mov      r0, r6
00668b44: bl       #0x65b4e4
00668b48: subs     r2, r0, #0
00668b4c: beq      #0x669500
00668b50: ldrb     r8, [r2, #0x13d]
00668b54: cmp      r8, #0
00668b58: bne      #0x6681d8
00668b5c: ldr      ip, [r4]
00668b60: ldr      r3, [r2]
00668b64: ldr      r1, [sp, #8]
00668b68: ldr      sb, [ip, #0x68]
00668b6c: mov      lr, pc
00668b70: ldr      pc, [r3, #0xfc]
00668b74: mov      r1, r5
00668b78: mov      r2, r0
00668b7c: mov      r3, r8
00668b80: mov      r0, r4
00668b84: blx      sb
00668b88: b        #0x6681d8
00668b8c: mov      r1, sb
00668b90: mov      r0, r6
00668b94: bl       #0x65b4e4
00668b98: subs     r2, r0, #0
00668b9c: beq      #0x6690b0
00668ba0: ldr      r1, [pc, #0x2f8]
00668ba4: ldr      ip, [r4]
00668ba8: ldr      r3, [r2]
00668bac: add      r1, pc, r1
00668bb0: ldr      r8, [ip, #0x68]
00668bb4: mov      lr, pc
00668bb8: ldr      pc, [r3, #0xfc]
00668bbc: mov      r1, r5
00668bc0: mov      r2, r0
00668bc4: mov      r3, #0
00668bc8: mov      r0, r4
00668bcc: blx      r8
00668bd0: b        #0x6681d8
00668bd4: mov      r1, sb
00668bd8: mov      r0, r6
00668bdc: bl       #0x65b4e4
00668be0: subs     r2, r0, #0
00668be4: beq      #0x6690e8
00668be8: ldr      r1, [pc, #0x2b4]
00668bec: ldr      ip, [r4]
00668bf0: ldr      r3, [r2]
00668bf4: add      r1, pc, r1
00668bf8: ldr      r8, [ip, #0x68]
00668bfc: mov      lr, pc
00668c00: ldr      pc, [r3, #0xfc]
00668c04: mov      r1, r5
00668c08: mov      r2, r0
00668c0c: mov      r3, #0
00668c10: mov      r0, r4
00668c14: blx      r8
00668c18: b        #0x6681d8
00668c1c: mov      r1, sb
00668c20: mov      r0, r6
00668c24: bl       #0x65b4e4
00668c28: subs     r2, r0, #0
00668c2c: beq      #0x6690cc
00668c30: ldr      r1, [pc, #0x270]
00668c34: ldr      ip, [r4]
00668c38: ldr      r3, [r2]
00668c3c: add      r1, pc, r1
00668c40: ldr      r8, [ip, #0x68]
00668c44: mov      lr, pc
00668c48: ldr      pc, [r3, #0xfc]
00668c4c: mov      r1, r5
00668c50: mov      r2, r0
00668c54: mov      r3, #0
00668c58: mov      r0, r4
00668c5c: blx      r8
00668c60: b        #0x6681d8
00668c64: mov      r1, sb
00668c68: mov      r0, r6
00668c6c: bl       #0x65b4e4
00668c70: subs     r2, r0, #0
00668c74: beq      #0x669158
00668c78: ldr      r1, [pc, #0x22c]
00668c7c: ldr      ip, [r4]
00668c80: ldr      r3, [r2]
00668c84: add      r1, pc, r1
00668c88: ldr      r8, [ip, #0x68]
00668c8c: mov      lr, pc
00668c90: ldr      pc, [r3, #0xfc]
00668c94: mov      r1, r5
00668c98: mov      r2, r0
00668c9c: mov      r3, #0
00668ca0: mov      r0, r4
00668ca4: blx      r8
00668ca8: b        #0x6681d8
00668cac: mov      r1, sb
00668cb0: mov      r0, r6
00668cb4: bl       #0x65b4e4
00668cb8: subs     r2, r0, #0
00668cbc: beq      #0x66913c
00668cc0: ldr      r1, [pc, #0x1e8]
00668cc4: ldr      ip, [r4]
00668cc8: ldr      r3, [r2]
00668ccc: add      r1, pc, r1
00668cd0: ldr      r8, [ip, #0x68]
00668cd4: mov      lr, pc
00668cd8: ldr      pc, [r3, #0xfc]
00668cdc: mov      r1, r5
00668ce0: mov      r2, r0
00668ce4: mov      r3, #0
00668ce8: mov      r0, r4
00668cec: blx      r8
00668cf0: b        #0x6681d8
00668cf4: mov      r1, sb
00668cf8: mov      r0, r6
00668cfc: bl       #0x65b4e4
00668d00: subs     r2, r0, #0
00668d04: beq      #0x669120
00668d08: ldr      r1, [pc, #0x1a4]
00668d0c: ldr      ip, [r4]
00668d10: ldr      r3, [r2]
00668d14: add      r1, pc, r1
00668d18: ldr      r8, [ip, #0x68]
00668d1c: mov      lr, pc
00668d20: ldr      pc, [r3, #0xfc]
00668d24: mov      r1, r5
00668d28: mov      r2, r0
00668d2c: mov      r3, #0
00668d30: mov      r0, r4
00668d34: blx      r8
00668d38: b        #0x6681d8
00668d3c: mov      r1, sb
00668d40: mov      r0, r6
00668d44: bl       #0x65b4e4
00668d48: subs     r2, r0, #0
00668d4c: beq      #0x669104
00668d50: ldr      r1, [pc, #0x160]
00668d54: ldr      ip, [r4]
00668d58: ldr      r3, [r2]
00668d5c: add      r1, pc, r1
00668d60: ldr      r8, [ip, #0x68]
00668d64: mov      lr, pc
00668d68: ldr      pc, [r3, #0xfc]
00668d6c: mov      r1, r5
00668d70: mov      r2, r0
00668d74: mov      r3, #0
00668d78: mov      r0, r4
00668d7c: blx      r8
00668d80: b        #0x6681d8
00668d84: mov      r1, sb
00668d88: mov      r0, r6
00668d8c: bl       #0x65b4e4
00668d90: subs     r2, r0, #0
00668d94: beq      #0x669190
00668d98: ldr      r1, [pc, #0x11c]
00668d9c: ldr      ip, [r4]
00668da0: ldr      r3, [r2]
00668da4: add      r1, pc, r1
00668da8: ldr      r8, [ip, #0x68]
00668dac: mov      lr, pc
00668db0: ldr      pc, [r3, #0xfc]
00668db4: mov      r1, r5
00668db8: mov      r2, r0
00668dbc: mov      r3, #0
00668dc0: mov      r0, r4
00668dc4: blx      r8
00668dc8: b        #0x6681d8
00668dcc: mov      r1, sb
00668dd0: mov      r0, r6
00668dd4: bl       #0x65b4e4
00668dd8: subs     r2, r0, #0
00668ddc: beq      #0x669174
00668de0: ldr      r1, [pc, #0xd8]
00668de4: ldr      ip, [r4]
00668de8: ldr      r3, [r2]
00668dec: add      r1, pc, r1
00668df0: ldr      r8, [ip, #0x68]
00668df4: mov      lr, pc
00668df8: ldr      pc, [r3, #0xfc]
00668dfc: mov      r1, r5
00668e00: mov      r2, r0
00668e04: mov      r3, #0
00668e08: mov      r0, r4
00668e0c: blx      r8
00668e10: b        #0x6681d8
00668e14: eorseq   ip, r2, r0, ror #22
00668e18: eoreq    sp, r7, r8, ror r2
00668e1c: mlaeq    r7, r8, r2, ip
00668e20: eoreq    sp, r7, r4, asr #4
00668e24: eoreq    sp, r7, r8, lsl r2
00668e28: eoreq    sp, r7, r4, ror #3
00668e2c: andeq    r0, r0, r0, lsl #20
00668e30: andeq    r1, r0, ip, asr r2
00668e34: eoreq    sp, r7, r4, asr #32
00668e38: eoreq    sp, r7, ip, lsr r0
00668e3c: eoreq    sp, r7, r4
00668e40: eoreq    r0, r7, r4, lsr #31
00668e44: eoreq    ip, r7, ip, lsl lr
00668e48: eoreq    ip, r7, ip, asr #26
00668e4c: eoreq    ip, r7, r4, lsl sp
00668e50: eoreq    ip, r7, r4, asr sp
00668e54: eoreq    ip, r7, ip, lsl sp
00668e58: eoreq    ip, r7, r4, ror #25
00668e5c: eoreq    ip, r7, ip, lsr #25
00668e60: eoreq    ip, r7, ip, ror ip
00668e64: eoreq    ip, r7, r4, asr #24
00668e68: eoreq    ip, r7, ip, lsr #26

# _ZNSt6vectorIPN6glitch7collada15particle_system15CForceSceneNodeENS0_4core10SAllocatorIS4_LNS0_6memory13E_MEMORY_HINTE0EEEE6resizeEjRKS4_
0063db08: push     {r4, r5}
0063db0c: ldr      r4, [r0, #4]
0063db10: ldr      r5, [r0]
0063db14: mov      r3, r2
0063db18: rsb      r2, r5, r4
0063db1c: asr      r2, r2, #2
0063db20: cmp      r1, r2
0063db24: bhs      #0x63db3c
0063db28: add      r5, r5, r1, lsl #2
0063db2c: cmp      r5, r4
0063db30: strne    r5, [r0, #4]
0063db34: pop      {r4, r5}
0063db38: bx       lr
0063db3c: rsb      r2, r2, r1
0063db40: mov      r1, r4
0063db44: pop      {r4, r5}
0063db48: b        #0x63d998

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

# _ZN6glitch7collada24IParticleSystemSceneNodeC2ERKNS0_16CColladaDatabaseEPNS_3res6vectorINS5_6StringEEEPNS0_14CRootSceneNodeE
00667a98: push     {r4, r5, r6, r7, r8, lr}
00667a9c: sub      sp, sp, #0x30
00667aa0: add      r4, sp, #8
00667aa4: mov      ip, #0
00667aa8: mov      lr, #0x3f800000
00667aac: mov      r6, r2
00667ab0: str      r4, [sp]
00667ab4: mvn      r2, #0
00667ab8: add      r4, sp, #0x18
00667abc: mov      r5, r1
00667ac0: mov      r7, r3
00667ac4: add      r1, r1, #4
00667ac8: add      r3, sp, #0x24
00667acc: str      r4, [sp, #4]
00667ad0: str      ip, [sp, #0x10]
00667ad4: mov      r4, r0
00667ad8: str      lr, [sp, #0x20]
00667adc: str      ip, [sp, #0x24]
00667ae0: str      ip, [sp, #0x28]
00667ae4: str      ip, [sp, #0x2c]
00667ae8: str      ip, [sp, #8]
00667aec: str      ip, [sp, #0xc]
00667af0: str      lr, [sp, #0x14]
00667af4: str      lr, [sp, #0x18]
00667af8: str      lr, [sp, #0x1c]
00667afc: ldr      r8, [sp, #0x48]
00667b00: bl       #0x5990c0
00667b04: ldr      r3, [r6]
00667b08: ldr      r1, [pc, #0xbc]
00667b0c: str      r3, [r4, #0x134]
00667b10: ldr      r2, [r6, #4]
00667b14: cmp      r3, #0
00667b18: add      r1, pc, r1
00667b1c: str      r2, [r4, #0x138]
00667b20: beq      #0x667b34
00667b24: ldr      r2, [r3, #4]
00667b28: cmp      r2, #0
00667b2c: addne    r2, r2, #1
00667b30: strne    r2, [r3, #4]
00667b34: ldr      r2, [pc, #0x94]
00667b38: mov      r3, #0
00667b3c: mov      r0, r8
00667b40: ldr      r2, [r1, r2]
00667b44: mov      r1, r4
00667b48: add      r2, r2, #4
00667b4c: str      r2, [r4, #0x130]
00667b50: ldr      r2, [r5]
00667b54: str      r2, [r4]
00667b58: ldr      ip, [r5, #0x10]
00667b5c: ldr      r2, [r2, #-0x1c]
00667b60: str      ip, [r4, r2]
00667b64: ldr      r2, [r4]
00667b68: ldr      ip, [r5, #0x14]
00667b6c: ldr      r2, [r2, #-0xc]
00667b70: str      ip, [r4, r2]
00667b74: strb     r3, [r4, #0x170]
00667b78: strb     r3, [r4, #0x13c]
00667b7c: strb     r3, [r4, #0x13d]
00667b80: strb     r3, [r4, #0x13e]
00667b84: strb     r3, [r4, #0x13f]
00667b88: str      r3, [r4, #0x140]
00667b8c: str      r3, [r4, #0x148]
00667b90: str      r3, [r4, #0x158]
00667b94: str      r3, [r4, #0x15c]
00667b98: str      r3, [r4, #0x160]
00667b9c: str      r3, [r4, #0x164]
00667ba0: str      r3, [r4, #0x168]
00667ba4: str      r3, [r4, #0x16c]
00667ba8: str      r7, [r4, #0x150]
00667bac: str      r8, [r4, #0x154]
00667bb0: bl       #0x65b210
00667bb4: mov      r0, r4
00667bb8: mov      r1, #2
00667bbc: bl       #0x59719c
00667bc0: mov      r0, r4
00667bc4: add      sp, sp, #0x30
00667bc8: pop      {r4, r5, r6, r7, r8, pc}
00667bcc: eorseq   ip, r2, r8, ror pc
00667bd0: strheq   r1, [r0], -r4

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
