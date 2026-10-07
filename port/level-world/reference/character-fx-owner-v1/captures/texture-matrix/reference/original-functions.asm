
# _ZNK6glitch7collada18SAnimationAccessor7getTypeEi
00669e10: ldr      r3, [r0]
00669e14: ldr      r3, [r3, #0x10]
00669e18: add      r3, r3, r1, lsl #4
00669e1c: ldr      r0, [r3, #8]
00669e20: bx       lr

# _ZNK6glitch7collada18SAnimationAccessor16getChannelsCountEv
00669e78: ldr      r3, [r0]
00669e7c: ldr      r0, [r3, #0xc]
00669e80: bx       lr

# _ZN6glitch4core21buildTextureTransformIfEENS0_8CMatrix4IT_EES3_RKNS0_8vector2dIS3_EES8_S8_
006e377c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
006e3780: mov      r4, r0
006e3784: mov      r0, r1
006e3788: mov      r5, r2
006e378c: mov      sb, r3
006e3790: mov      r6, r1
006e3794: bl       #0x30e754
006e3798: mov      r8, r0
006e379c: mov      r0, r6
006e37a0: bl       #0x30eb08
006e37a4: ldr      r7, [sp, #0x28]
006e37a8: mov      r3, #0
006e37ac: strb     r3, [r4, #0x40]
006e37b0: mov      sl, r0
006e37b4: mov      r1, r8
006e37b8: ldr      r0, [r7]
006e37bc: bl       #0x30ed6c
006e37c0: str      r0, [r4]
006e37c4: ldr      r0, [r7, #4]
006e37c8: mov      r1, sl
006e37cc: bl       #0x30ed6c
006e37d0: mov      r6, #0
006e37d4: str      r0, [r4, #4]
006e37d8: str      r6, [r4, #8]
006e37dc: str      r6, [r4, #0xc]
006e37e0: ldr      r1, [r7]
006e37e4: add      r0, sl, #0x80000000
006e37e8: bl       #0x30ed6c
006e37ec: str      r0, [r4, #0x10]
006e37f0: ldr      r0, [r7, #4]
006e37f4: mov      r1, r8
006e37f8: bl       #0x30ed6c
006e37fc: str      r6, [r4, #0x18]
006e3800: str      r0, [r4, #0x14]
006e3804: str      r6, [r4, #0x1c]
006e3808: ldr      fp, [r5]
006e380c: mov      r0, r8
006e3810: mov      r1, fp
006e3814: bl       #0x30ed6c
006e3818: mov      r1, r0
006e381c: mov      r0, fp
006e3820: bl       #0x30e3ac
006e3824: ldr      r1, [r5, #4]
006e3828: mov      fp, r0
006e382c: mov      r0, sl
006e3830: bl       #0x30ed6c
006e3834: mov      r1, r0
006e3838: mov      r0, fp
006e383c: bl       #0x30eba4
006e3840: ldr      r1, [r7]
006e3844: bl       #0x30ed6c
006e3848: ldr      r1, [sb]
006e384c: bl       #0x30eba4
006e3850: str      r0, [r4, #0x20]
006e3854: ldr      r1, [r5]
006e3858: mov      r0, sl
006e385c: bl       #0x30ed6c
006e3860: ldr      r5, [r5, #4]
006e3864: mov      r1, r0
006e3868: mov      r0, r5
006e386c: bl       #0x30e3ac
006e3870: mov      r1, r5
006e3874: mov      sl, r0
006e3878: mov      r0, r8
006e387c: bl       #0x30ed6c
006e3880: mov      r1, r0
006e3884: mov      r0, sl
006e3888: bl       #0x30e3ac
006e388c: ldr      r1, [r7, #4]
006e3890: bl       #0x30ed6c
006e3894: ldr      r1, [sb, #4]
006e3898: bl       #0x30eba4
006e389c: mov      r3, #0x3f800000
006e38a0: str      r0, [r4, #0x24]
006e38a4: str      r3, [r4, #0x28]
006e38a8: mov      r0, r4
006e38ac: str      r6, [r4, #0x2c]
006e38b0: str      r6, [r4, #0x38]
006e38b4: str      r3, [r4, #0x3c]
006e38b8: str      r6, [r4, #0x30]
006e38bc: str      r6, [r4, #0x34]
006e38c0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
