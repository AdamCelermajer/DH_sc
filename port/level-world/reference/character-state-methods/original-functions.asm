
# _ZN8CSLimbus7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c2e58: push     {r4, r5, r6, r7, r8, lr}
003c2e5c: ldr      r4, [pc, #0x104]
003c2e60: ldr      r6, [pc, #0x104]
003c2e64: ldr      r1, [pc, #0x104]
003c2e68: add      r4, pc, r4
003c2e6c: ldr      r3, [r4, r6]
003c2e70: ldr      r8, [r4, r1]
003c2e74: sub      sp, sp, #0x28
003c2e78: ldr      r3, [r3]
003c2e7c: mov      r0, r8
003c2e80: mov      r5, r2
003c2e84: str      r3, [sp, #0x24]
003c2e88: bl       #0x337888
003c2e8c: ldr      r1, [pc, #0xe0]
003c2e90: add      r7, sp, #0xc
003c2e94: add      r2, sp, #8
003c2e98: add      r1, pc, r1
003c2e9c: mov      r0, r7
003c2ea0: bl       #0x3140ec
003c2ea4: mov      r1, r7
003c2ea8: mov      r0, r8
003c2eac: bl       #0x337a88
003c2eb0: mov      r0, r7
003c2eb4: bl       #0x318254
003c2eb8: mov      r1, #0
003c2ebc: ldr      r3, [r5]
003c2ec0: str      r1, [r5, #0x520]
003c2ec4: mov      r0, r5
003c2ec8: mov      lr, pc
003c2ecc: ldr      pc, [r3, #0x40]
003c2ed0: ldrb     r3, [r5, #0x530]
003c2ed4: cmp      r3, #0
003c2ed8: bne      #0x3c2f00
003c2edc: add      r0, r5, #0x3c8
003c2ee0: bl       #0x3d5fa8
003c2ee4: ldr      r3, [r4, r6]
003c2ee8: ldr      r2, [sp, #0x24]
003c2eec: ldr      r3, [r3]
003c2ef0: cmp      r2, r3
003c2ef4: bne      #0x3c2f64
003c2ef8: add      sp, sp, #0x28
003c2efc: pop      {r4, r5, r6, r7, r8, pc}
003c2f00: mov      r0, r5
003c2f04: bl       #0x3a4bac
003c2f08: cmp      r0, #0
003c2f0c: ble      #0x3c2edc
003c2f10: bl       #0x7fd794
003c2f14: ldrb     r3, [r0, #5]
003c2f18: cmp      r3, #0
003c2f1c: bne      #0x3c2f48
003c2f20: mov      r0, r5
003c2f24: bl       #0x3a4bac
003c2f28: mov      ip, #0
003c2f2c: mov      r1, r0
003c2f30: mov      r2, ip
003c2f34: add      r0, r5, #0x3b4
003c2f38: mov      r3, #0x2f
003c2f3c: str      ip, [sp]
003c2f40: bl       #0x3dbe24
003c2f44: b        #0x3c2edc
003c2f48: ldr      r3, [pc, #0x28]
003c2f4c: ldr      r3, [r4, r3]
003c2f50: ldr      r0, [r3, #0x40]
003c2f54: bl       #0x36f074
003c2f58: cmp      r0, #0
003c2f5c: beq      #0x3c2edc
003c2f60: b        #0x3c2f20
003c2f64: bl       #0x30e310
003c2f68: subseq   r1, sp, r8, lsr #24
003c2f6c: andeq    r4, r0, ip, lsr #1
003c2f70: andeq    r0, r0, r4, lsl #17
003c2f74: ldrheq   r1, [r0], #-0xf8
003c2f78: strdeq   r3, r4, [r0], -r4

# _ZN8CSLimbus6OnBlurEiP9CharacterP16CharStateMachinei
003c2be4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c2be8: ldr      r5, [pc, #0x13c]
003c2bec: ldr      r7, [pc, #0x13c]
003c2bf0: ldr      r1, [pc, #0x13c]
003c2bf4: add      r5, pc, r5
003c2bf8: ldr      r3, [r5, r7]
003c2bfc: ldr      r8, [r5, r1]
003c2c00: sub      sp, sp, #0x20
003c2c04: ldr      r3, [r3]
003c2c08: mov      r0, r8
003c2c0c: mov      r4, r2
003c2c10: str      r3, [sp, #0x1c]
003c2c14: bl       #0x337888
003c2c18: ldr      r1, [pc, #0x118]
003c2c1c: add      r6, sp, #4
003c2c20: mov      r2, sp
003c2c24: mov      r0, r6
003c2c28: add      r1, pc, r1
003c2c2c: bl       #0x3140ec
003c2c30: mov      r1, r6
003c2c34: mov      r0, r8
003c2c38: bl       #0x337a88
003c2c3c: mov      r0, r6
003c2c40: bl       #0x318254
003c2c44: ldr      r3, [r4, #0x378]
003c2c48: mov      r6, #0
003c2c4c: mov      r0, r4
003c2c50: strb     r6, [r3, #8]
003c2c54: ldr      r3, [r4]
003c2c58: mov      r1, #1
003c2c5c: add      r8, r4, #0x1440
003c2c60: mov      lr, pc
003c2c64: ldr      pc, [r3, #0x40]
003c2c68: mov      r2, #1
003c2c6c: add      r1, r8, #0x10
003c2c70: mov      r0, r4
003c2c74: bl       #0x393db4
003c2c78: mov      r0, r4
003c2c7c: add      r1, r8, #0x1c
003c2c80: bl       #0x3938a0
003c2c84: mov      r0, r4
003c2c88: mov      r1, r6
003c2c8c: mov      r2, r6
003c2c90: bl       #0x3a59ac
003c2c94: ldr      r3, [r4, #0x400]
003c2c98: cmp      r3, #3
003c2c9c: beq      #0x3c2cbc
003c2ca0: ldr      r3, [r5, r7]
003c2ca4: ldr      r2, [sp, #0x1c]
003c2ca8: ldr      r3, [r3]
003c2cac: cmp      r2, r3
003c2cb0: bne      #0x3c2d28
003c2cb4: add      sp, sp, #0x20
003c2cb8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c2cbc: ldr      sl, [r4, #0x3fc]
003c2cc0: ldr      r3, [sl, #0x18]
003c2cc4: ldr      sb, [sl, #0x1c]
003c2cc8: rsb      sb, r3, sb
003c2ccc: asrs     sb, sb, #2
003c2cd0: beq      #0x3c2d1c
003c2cd4: mov      r8, #1
003c2cd8: b        #0x3c2ce0
003c2cdc: ldr      r3, [sl, #0x18]
003c2ce0: ldr      r0, [r3, r6, lsl #2]
003c2ce4: cmp      r0, r4
003c2ce8: beq      #0x3c2d00
003c2cec: add      r0, r0, #0x4f0
003c2cf0: add      r0, r0, #0xc
003c2cf4: bl       #0x3c01c0
003c2cf8: cmp      r0, #0
003c2cfc: movne    r8, #0
003c2d00: add      r6, r6, #1
003c2d04: cmp      r6, sb
003c2d08: bne      #0x3c2cdc
003c2d0c: cmp      r8, #0
003c2d10: moveq    r3, #2
003c2d14: streq    r3, [sl, #0x24]
003c2d18: beq      #0x3c2ca0
003c2d1c: mov      r3, #0
003c2d20: str      r3, [sl, #0x24]
003c2d24: b        #0x3c2ca0
003c2d28: bl       #0x30e310

# _ZN8CSLimbus8OnUpdateEiP9CharacterP16CharStateMachine
003bffe4: bx       lr

# _ZN8CSLimbus7OnEventEiP9CharacterP16CharStateMachineiPv
003bffe8: bx       lr

# _ZN7CSSpawn7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c35ec: push     {r4, r5, r6, r7, r8, sl, lr}
003c35f0: ldr      r5, [pc, #0x178]
003c35f4: ldr      r8, [pc, #0x178]
003c35f8: ldr      r1, [pc, #0x178]
003c35fc: add      r5, pc, r5
003c3600: ldr      r3, [r5, r8]
003c3604: ldr      r6, [r5, r1]
003c3608: sub      sp, sp, #0x44
003c360c: ldr      r3, [r3]
003c3610: mov      r0, r6
003c3614: mov      r4, r2
003c3618: str      r3, [sp, #0x3c]
003c361c: bl       #0x337888
003c3620: ldr      r1, [pc, #0x154]
003c3624: add      r7, sp, #0x24
003c3628: add      r2, sp, #8
003c362c: add      r1, pc, r1
003c3630: mov      r0, r7
003c3634: bl       #0x3140ec
003c3638: mov      r1, r7
003c363c: mov      r0, r6
003c3640: bl       #0x337a88
003c3644: mov      r0, r7
003c3648: bl       #0x318254
003c364c: mov      r0, r6
003c3650: bl       #0x337888
003c3654: ldr      r1, [pc, #0x124]
003c3658: add      r7, sp, #0xc
003c365c: add      r2, sp, #4
003c3660: mov      r0, r7
003c3664: add      r1, pc, r1
003c3668: bl       #0x3140ec
003c366c: mov      r1, r7
003c3670: mov      r0, r6
003c3674: bl       #0x337a88
003c3678: mov      r0, r7
003c367c: bl       #0x318254
003c3680: ldr      r3, [sp, #0x60]
003c3684: mov      r0, r4
003c3688: add      r6, r4, #0x490
003c368c: cmp      r3, #0x11
003c3690: movw     r3, #0x241
003c3694: ldreq    r7, [r4, #0x520]
003c3698: str      r3, [r4, #0x520]
003c369c: ldr      r3, [pc, #0xe0]
003c36a0: movne    r7, #0
003c36a4: ubfxeq   r7, r7, #0xd, #1
003c36a8: ldr      r3, [r5, r3]
003c36ac: add      r6, r6, #0xc
003c36b0: ldr      sl, [r3]
003c36b4: bl       #0x3a3228
003c36b8: ldr      r3, [pc, #0xc8]
003c36bc: ldr      r1, [pc, #0xc8]
003c36c0: ldr      r2, [r5, r3]
003c36c4: mov      r3, #0xa0
003c36c8: mla      r3, r3, r0, sl
003c36cc: ldr      r0, [r2, #0x2c]
003c36d0: ldr      r2, [pc, #0xb8]
003c36d4: add      r1, pc, r1
003c36d8: ldr      sl, [r3, #0x80]
003c36dc: add      r2, pc, r2
003c36e0: bl       #0x4c4bdc
003c36e4: ands     r0, r0, #1
003c36e8: bne      #0x3c3760
003c36ec: add      r1, r0, sl
003c36f0: mov      r0, r6
003c36f4: bl       #0x3cacb0
003c36f8: add      r6, r4, #0x3c8
003c36fc: mov      r1, #0
003c3700: mov      r2, r1
003c3704: mov      r0, r6
003c3708: bl       #0x3d6890
003c370c: mov      r0, r6
003c3710: bl       #0x3d49c4
003c3714: mov      r0, r4
003c3718: bl       #0x3bc6b8
003c371c: cmp      r7, #0
003c3720: ldrne    r3, [r4, #0x520]
003c3724: ldr      r0, [r4, #0x2d8]
003c3728: orrne    r3, r3, #0x2000
003c372c: strne    r3, [r4, #0x520]
003c3730: cmp      r0, #0
003c3734: beq      #0x3c3744
003c3738: mov      r3, #0x1440
003c373c: ldr      r1, [r4, r3]
003c3740: bl       #0x470ce4
003c3744: ldr      r3, [r5, r8]
003c3748: ldr      r2, [sp, #0x3c]
003c374c: ldr      r3, [r3]
003c3750: cmp      r2, r3
003c3754: bne      #0x3c376c
003c3758: add      sp, sp, #0x44
003c375c: pop      {r4, r5, r6, r7, r8, sl, pc}
003c3760: mov      r0, r4
003c3764: bl       #0x3a53e0
003c3768: b        #0x3c36ec
003c376c: bl       #0x30e310

# _ZN7CSSpawn6OnBlurEiP9CharacterP16CharStateMachinei
003c2f7c: push     {r4, r5, r6, r7, r8, lr}
003c2f80: ldr      r4, [pc, #0x88]
003c2f84: ldr      r6, [pc, #0x88]
003c2f88: ldr      r1, [pc, #0x88]
003c2f8c: add      r4, pc, r4
003c2f90: ldr      r3, [r4, r6]
003c2f94: ldr      r7, [r4, r1]
003c2f98: sub      sp, sp, #0x20
003c2f9c: ldr      r3, [r3]
003c2fa0: mov      r0, r7
003c2fa4: mov      r8, r2
003c2fa8: str      r3, [sp, #0x1c]
003c2fac: bl       #0x337888
003c2fb0: ldr      r1, [pc, #0x64]
003c2fb4: add      r5, sp, #4
003c2fb8: mov      r2, sp
003c2fbc: add      r1, pc, r1
003c2fc0: mov      r0, r5
003c2fc4: bl       #0x3140ec
003c2fc8: mov      r1, r5
003c2fcc: mov      r0, r7
003c2fd0: bl       #0x337a88
003c2fd4: mov      r0, r5
003c2fd8: bl       #0x318254
003c2fdc: ldr      r3, [r8, #0x520]
003c2fe0: tst      r3, #0x2000
003c2fe4: bne      #0x3c2ff0
003c2fe8: mov      r0, r8
003c2fec: bl       #0x3b4088
003c2ff0: ldr      r3, [r4, r6]
003c2ff4: ldr      r2, [sp, #0x1c]
003c2ff8: ldr      r3, [r3]
003c2ffc: cmp      r2, r3
003c3000: bne      #0x3c300c
003c3004: add      sp, sp, #0x20
003c3008: pop      {r4, r5, r6, r7, r8, pc}
003c300c: bl       #0x30e310
003c3010: subseq   r1, sp, r4, lsl #22
003c3014: andeq    r4, r0, ip, lsr #1
003c3018: andeq    r0, r0, r4, lsl #17

# _ZN7CSSpawn8OnUpdateEiP9CharacterP16CharStateMachine
003bfff0: bx       lr

# _ZN7CSSpawn7OnEventEiP9CharacterP16CharStateMachineiPv
003c0b04: push     {r4, lr}
003c0b08: ldr      r3, [sp, #8]
003c0b0c: mov      r4, r2
003c0b10: ldr      r0, [sp, #0xc]
003c0b14: cmp      r3, #0x28
003c0b18: beq      #0x3c0b20
003c0b1c: pop      {r4, pc}
003c0b20: ldr      r1, [pc, #0x24]
003c0b24: add      r1, pc, r1
003c0b28: bl       #0x30e31c
003c0b2c: cmp      r0, #0
003c0b30: bne      #0x3c0b1c
003c0b34: ldr      r3, [r4, #0x520]
003c0b38: mov      r0, r4
003c0b3c: orr      r3, r3, #0x2000
003c0b40: str      r3, [r4, #0x520]
003c0b44: pop      {r4, lr}
003c0b48: b        #0x3b4088
003c0b4c: subseq   r4, r0, r4, asr #32

# _ZN9CSDespawn7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c32fc: ldr      r3, [pc, #0xd0]
003c3300: ldr      r1, [pc, #0xd0]
003c3304: push     {r4, r5, r6, r7, lr}
003c3308: add      r3, pc, r3
003c330c: ldr      r7, [r3, r1]
003c3310: ldr      r1, [pc, #0xc4]
003c3314: mov      r4, r2
003c3318: ldr      r2, [r7]
003c331c: ldr      r5, [r3, r1]
003c3320: sub      sp, sp, #0x44
003c3324: str      r2, [sp, #0x3c]
003c3328: mov      r0, r5
003c332c: bl       #0x337888
003c3330: ldr      r1, [pc, #0xa8]
003c3334: add      r6, sp, #0x24
003c3338: add      r2, sp, #8
003c333c: mov      r0, r6
003c3340: add      r1, pc, r1
003c3344: bl       #0x3140ec
003c3348: mov      r1, r6
003c334c: mov      r0, r5
003c3350: bl       #0x337a88
003c3354: mov      r0, r6
003c3358: bl       #0x318254
003c335c: mov      r0, r5
003c3360: bl       #0x337888
003c3364: ldr      r1, [pc, #0x78]
003c3368: add      r6, sp, #0xc
003c336c: add      r2, sp, #4
003c3370: add      r1, pc, r1
003c3374: mov      r0, r6
003c3378: bl       #0x3140ec
003c337c: mov      r1, r6
003c3380: mov      r0, r5
003c3384: bl       #0x337a88
003c3388: mov      r0, r6
003c338c: bl       #0x318254
003c3390: ldr      r3, [r4, #0x534]
003c3394: mov      r2, #0x200
003c3398: add      r0, r4, #0x4f0
003c339c: str      r2, [r4, #0x520]
003c33a0: str      r3, [r4, #0x524]
003c33a4: add      r0, r0, #0xc
003c33a8: mvn      r1, #0
003c33ac: bl       #0x3c0b50
003c33b0: mov      r0, r4
003c33b4: bl       #0x3bc6b8
003c33b8: ldr      r2, [sp, #0x3c]
003c33bc: ldr      r3, [r7]
003c33c0: cmp      r2, r3
003c33c4: bne      #0x3c33d0
003c33c8: add      sp, sp, #0x44
003c33cc: pop      {r4, r5, r6, r7, pc}
003c33d0: bl       #0x30e310
003c33d4: subseq   r1, sp, r8, lsl #15
003c33d8: andeq    r4, r0, ip, lsr #1
003c33dc: andeq    r0, r0, r4, lsl #17
003c33e0: subseq   r1, r0, r0, lsl fp
003c33e4: ldrsheq  r1, [r0], #-0xa8

# _ZN9CSDespawn6OnBlurEiP9CharacterP16CharStateMachinei
003c3794: push     {r4, r5, r6, r7, r8, lr}
003c3798: ldr      r5, [pc, #0x17c]
003c379c: ldr      r7, [pc, #0x17c]
003c37a0: ldr      r1, [pc, #0x17c]
003c37a4: add      r5, pc, r5
003c37a8: ldr      r3, [r5, r7]
003c37ac: ldr      r8, [r5, r1]
003c37b0: sub      sp, sp, #0x28
003c37b4: ldr      r3, [r3]
003c37b8: mov      r0, r8
003c37bc: mov      r4, r2
003c37c0: str      r3, [sp, #0x24]
003c37c4: bl       #0x337888
003c37c8: ldr      r1, [pc, #0x158]
003c37cc: add      r6, sp, #0xc
003c37d0: add      r2, sp, #8
003c37d4: add      r1, pc, r1
003c37d8: mov      r0, r6
003c37dc: bl       #0x3140ec
003c37e0: mov      r1, r6
003c37e4: mov      r0, r8
003c37e8: bl       #0x337a88
003c37ec: mov      r0, r6
003c37f0: bl       #0x318254
003c37f4: ldr      r3, [r4, #0x378]
003c37f8: mov      r2, #0
003c37fc: mov      r0, r4
003c3800: strb     r2, [r3, #8]
003c3804: mov      r3, #1
003c3808: strb     r3, [r4, #0x530]
003c380c: bl       #0x3a5248
003c3810: cmp      r0, #0
003c3814: beq      #0x3c3884
003c3818: bl       #0x7fd794
003c381c: ldrb     r3, [r0, #5]
003c3820: add      r6, r4, #0x490
003c3824: add      r6, r6, #0xc
003c3828: cmp      r3, #0
003c382c: movne    r3, #0
003c3830: mov      r8, #0
003c3834: strbne   r3, [r4, #0x118]
003c3838: mov      r0, r6
003c383c: str      r8, [r4, #0x520]
003c3840: bl       #0x3c91c8
003c3844: mov      r0, r6
003c3848: bl       #0x3ca52c
003c384c: ldr      r3, [pc, #0xd8]
003c3850: add      r1, sp, #0x28
003c3854: mov      r2, r8
003c3858: ldr      r3, [r5, r3]
003c385c: mov      r0, r4
003c3860: str      r3, [r1, #-0x24]!
003c3864: bl       #0x3a7b24
003c3868: ldr      r3, [r5, r7]
003c386c: ldr      r2, [sp, #0x24]
003c3870: ldr      r3, [r3]
003c3874: cmp      r2, r3
003c3878: bne      #0x3c3918
003c387c: add      sp, sp, #0x28
003c3880: pop      {r4, r5, r6, r7, r8, pc}
003c3884: bl       #0x7fd794
003c3888: ldrb     r3, [r0, #5]
003c388c: cmp      r3, #0
003c3890: bne      #0x3c38e8
003c3894: ldr      r3, [pc, #0x94]
003c3898: mov      r1, #0
003c389c: mov      r2, #1
003c38a0: ldr      r3, [r5, r3]
003c38a4: ldr      r0, [r3, #0x40]
003c38a8: bl       #0x36e478
003c38ac: ldr      r1, [r0, #0x660]
003c38b0: cmp      r1, #0
003c38b4: beq      #0x3c38cc
003c38b8: movw     r3, #0x14a4
003c38bc: ldr      r2, [r1, r3]
003c38c0: cmp      r4, r2
003c38c4: moveq    r2, #0
003c38c8: streq    r2, [r1, r3]
003c38cc: movw     r3, #0x14e4
003c38d0: ldrb     r3, [r4, r3]
003c38d4: cmp      r3, #0
003c38d8: beq      #0x3c3904
003c38dc: mov      r0, r4
003c38e0: bl       #0x33ddb4
003c38e4: b        #0x3c3818
003c38e8: ldr      r3, [r4]
003c38ec: mov      r0, r4
003c38f0: mov      lr, pc
003c38f4: ldr      pc, [r3, #0x28]
003c38f8: cmp      r0, #0
003c38fc: bne      #0x3c3818
003c3900: b        #0x3c3894
003c3904: mov      r0, r4
003c3908: bl       #0x3a30ac
003c390c: cmp      r0, #0
003c3910: beq      #0x3c3818
003c3914: b        #0x3c38dc
003c3918: bl       #0x30e310
003c391c: subseq   r1, sp, ip, ror #5
003c3920: andeq    r4, r0, ip, lsr #1
003c3924: andeq    r0, r0, r4, lsl #17
003c3928: subseq   r1, r0, ip, ror r6
003c392c: andeq    r1, r0, r4, lsr r1
003c3930: strdeq   r3, r4, [r0], -r4

# _ZN9CSDespawn8OnUpdateEiP9CharacterP16CharStateMachine
003bfff8: bx       lr

# _ZN9CSDespawn7OnEventEiP9CharacterP16CharStateMachineiPv
003bfffc: bx       lr

# _ZN7CSSkill7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c4480: push     {r4, r5, r6, r7, r8, lr}
003c4484: ldr      r5, [pc, #0x128]
003c4488: ldr      r7, [pc, #0x128]
003c448c: ldr      r1, [pc, #0x128]
003c4490: add      r5, pc, r5
003c4494: ldr      r3, [r5, r7]
003c4498: ldr      r8, [r5, r1]
003c449c: sub      sp, sp, #0x20
003c44a0: ldr      r3, [r3]
003c44a4: mov      r0, r8
003c44a8: mov      r4, r2
003c44ac: str      r3, [sp, #0x1c]
003c44b0: bl       #0x337888
003c44b4: ldr      r1, [pc, #0x104]
003c44b8: add      r6, sp, #4
003c44bc: mov      r2, sp
003c44c0: add      r1, pc, r1
003c44c4: mov      r0, r6
003c44c8: bl       #0x3140ec
003c44cc: mov      r1, r6
003c44d0: mov      r0, r8
003c44d4: bl       #0x337a88
003c44d8: mov      r0, r6
003c44dc: bl       #0x318254
003c44e0: ldr      r3, [r4, #0x528]
003c44e4: movw     r2, #0x6341
003c44e8: str      r2, [r4, #0x520]
003c44ec: bic      r3, r3, #0x140
003c44f0: str      r3, [r4, #0x528]
003c44f4: mov      r2, #0
003c44f8: mov      r0, r4
003c44fc: mov      r1, #0x1e
003c4500: bl       #0x3a4d5c
003c4504: add      r0, r4, #0x4f0
003c4508: add      r0, r0, #0xc
003c450c: mvn      r1, #0
003c4510: bl       #0x3c0b50
003c4514: add      r0, r4, #0x490
003c4518: add      r0, r0, #0xc
003c451c: mov      r1, #0x3f800000
003c4520: bl       #0x3c93fc
003c4524: mov      r3, #0
003c4528: strb     r3, [r4, #0x412]
003c452c: mov      r0, r4
003c4530: bl       #0x3bc6b8
003c4534: ldrb     r3, [r4, #0x554]
003c4538: ldr      r0, [r4, #0x2dc]
003c453c: cmp      r3, #0
003c4540: ldrne    r3, [r4, #0x528]
003c4544: orrne    r3, r3, #0x100
003c4548: strne    r3, [r4, #0x528]
003c454c: cmp      r0, #0
003c4550: beq      #0x3c4558
003c4554: bl       #0x46eae0
003c4558: mov      r0, r4
003c455c: bl       #0x3a3064
003c4560: cmp      r0, #0
003c4564: bne      #0x3c4584
003c4568: ldr      r3, [r5, r7]
003c456c: ldr      r2, [sp, #0x1c]
003c4570: ldr      r3, [r3]
003c4574: cmp      r2, r3
003c4578: bne      #0x3c45b0
003c457c: add      sp, sp, #0x20
003c4580: pop      {r4, r5, r6, r7, r8, pc}
003c4584: mov      r0, r4
003c4588: bl       #0x3a3144
003c458c: cmp      r0, #0
003c4590: bne      #0x3c4568
003c4594: mov      r0, r4
003c4598: bl       #0x3a3158
003c459c: cmp      r0, #0
003c45a0: ldreq    r3, [r4, #0x520]
003c45a4: orreq    r3, r3, #0x10000
003c45a8: streq    r3, [r4, #0x520]
003c45ac: b        #0x3c4568
003c45b0: bl       #0x30e310
003c45b4: subseq   r0, sp, r0, lsl #12
003c45b8: andeq    r4, r0, ip, lsr #1
003c45bc: andeq    r0, r0, r4, lsl #17

# _ZN7CSSkill6OnBlurEiP9CharacterP16CharStateMachinei
003c434c: push     {r4, r5, r6, r7, r8, lr}
003c4350: ldr      r5, [pc, #0x118]
003c4354: ldr      r6, [pc, #0x118]
003c4358: ldr      r1, [pc, #0x118]
003c435c: add      r5, pc, r5
003c4360: ldr      r3, [r5, r6]
003c4364: ldr      r8, [r5, r1]
003c4368: sub      sp, sp, #0x28
003c436c: ldr      r3, [r3]
003c4370: mov      r0, r8
003c4374: mov      r4, r2
003c4378: str      r3, [sp, #0x24]
003c437c: bl       #0x337888
003c4380: ldr      r1, [pc, #0xf4]
003c4384: add      r7, sp, #0xc
003c4388: add      r2, sp, #8
003c438c: add      r1, pc, r1
003c4390: mov      r0, r7
003c4394: bl       #0x3140ec
003c4398: mov      r1, r7
003c439c: mov      r0, r8
003c43a0: bl       #0x337a88
003c43a4: mov      r0, r7
003c43a8: bl       #0x318254
003c43ac: add      r0, r4, #0x3c8
003c43b0: bl       #0x3d49c4
003c43b4: mov      r0, r4
003c43b8: bl       #0x3938f8
003c43bc: mov      r0, r4
003c43c0: mov      r1, #0x1f
003c43c4: mov      r2, #0
003c43c8: bl       #0x3a4d5c
003c43cc: ldr      r3, [r4, #0x528]
003c43d0: tst      r3, #0x100
003c43d4: bne      #0x3c4414
003c43d8: ldr      r0, [r4, #0x2dc]
003c43dc: cmp      r0, #0
003c43e0: beq      #0x3c43e8
003c43e4: bl       #0x46eb20
003c43e8: mov      r0, r4
003c43ec: bl       #0x3a3064
003c43f0: cmp      r0, #0
003c43f4: bne      #0x3c4440
003c43f8: ldr      r3, [r5, r6]
003c43fc: ldr      r2, [sp, #0x24]
003c4400: ldr      r3, [r3]
003c4404: cmp      r2, r3
003c4408: bne      #0x3c446c
003c440c: add      sp, sp, #0x28
003c4410: pop      {r4, r5, r6, r7, r8, pc}
003c4414: mov      ip, #0
003c4418: mov      r2, ip
003c441c: mov      r1, #0xa
003c4420: mov      r3, #0x30
003c4424: add      r0, r4, #0x3b4
003c4428: str      ip, [sp]
003c442c: bl       #0x3dbe24
003c4430: mov      r0, r4
003c4434: bl       #0x3a3064
003c4438: cmp      r0, #0
003c443c: beq      #0x3c43f8
003c4440: mov      r0, r4
003c4444: bl       #0x3a3144
003c4448: cmp      r0, #0
003c444c: bne      #0x3c43f8
003c4450: mov      r0, r4
003c4454: bl       #0x3a3158
003c4458: cmp      r0, #0
003c445c: ldreq    r3, [r4, #0x520]
003c4460: biceq    r3, r3, #0x10000
003c4464: streq    r3, [r4, #0x520]
003c4468: b        #0x3c43f8
003c446c: bl       #0x30e310
003c4470: subseq   r0, sp, r4, lsr r7
003c4474: andeq    r4, r0, ip, lsr #1
003c4478: andeq    r0, r0, r4, lsl #17
003c447c: subseq   r0, r0, r4, asr #21

# _ZN7CSSkill8OnUpdateEiP9CharacterP16CharStateMachine
003c0018: bx       lr

# _ZN7CSSkill7OnEventEiP9CharacterP16CharStateMachineiPv
003c0ac8: push     {r4, lr}
003c0acc: ldr      r3, [sp, #8]
003c0ad0: mov      r4, r2
003c0ad4: cmp      r3, #0x28
003c0ad8: bne      #0x3c0afc
003c0adc: ldr      r1, [pc, #0x1c]
003c0ae0: ldr      r0, [sp, #0xc]
003c0ae4: add      r1, pc, r1
003c0ae8: bl       #0x30e31c
003c0aec: cmp      r0, #0
003c0af0: ldreq    r3, [r4, #0x520]
003c0af4: orreq    r3, r3, #0x8000
003c0af8: streq    r3, [r4, #0x520]
003c0afc: pop      {r4, pc}
003c0b00: subseq   r4, r0, r4, ror r0

# _ZN6CSCast7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c39d0: ldr      r3, [pc, #0xbc]
003c39d4: ldr      r1, [pc, #0xbc]
003c39d8: push     {r4, r5, r6, r7, lr}
003c39dc: add      r3, pc, r3
003c39e0: ldr      r6, [r3, r1]
003c39e4: ldr      r1, [pc, #0xb0]
003c39e8: mov      r4, r2
003c39ec: ldr      r2, [r6]
003c39f0: ldr      r7, [r3, r1]
003c39f4: sub      sp, sp, #0x24
003c39f8: str      r2, [sp, #0x1c]
003c39fc: mov      r0, r7
003c3a00: bl       #0x337888
003c3a04: ldr      r1, [pc, #0x94]
003c3a08: add      r5, sp, #4
003c3a0c: mov      r2, sp
003c3a10: add      r1, pc, r1
003c3a14: mov      r0, r5
003c3a18: bl       #0x3140ec
003c3a1c: mov      r1, r5
003c3a20: mov      r0, r7
003c3a24: bl       #0x337a88
003c3a28: mov      r0, r5
003c3a2c: bl       #0x318254
003c3a30: movw     r3, #0x6301
003c3a34: mov      r2, #0
003c3a38: str      r3, [r4, #0x520]
003c3a3c: mov      r0, r4
003c3a40: mov      r1, #0x20
003c3a44: bl       #0x3a4d5c
003c3a48: add      r0, r4, #0x4f0
003c3a4c: add      r0, r0, #0xc
003c3a50: mvn      r1, #0
003c3a54: bl       #0x3c0b50
003c3a58: add      r0, r4, #0x490
003c3a5c: add      r0, r0, #0xc
003c3a60: mov      r1, #0x3f800000
003c3a64: bl       #0x3c93fc
003c3a68: mov      r3, #0
003c3a6c: strb     r3, [r4, #0x412]
003c3a70: mov      r0, r4
003c3a74: bl       #0x3bc6b8
003c3a78: ldr      r2, [sp, #0x1c]
003c3a7c: ldr      r3, [r6]
003c3a80: cmp      r2, r3
003c3a84: bne      #0x3c3a90
003c3a88: add      sp, sp, #0x24
003c3a8c: pop      {r4, r5, r6, r7, pc}
003c3a90: bl       #0x30e310
003c3a94: ldrheq   r1, [sp], #-4
003c3a98: andeq    r4, r0, ip, lsr #1
003c3a9c: andeq    r0, r0, r4, lsl #17
003c3aa0: subseq   r1, r0, r0, asr #8

# _ZN6CSCast6OnBlurEiP9CharacterP16CharStateMachinei
003c3934: ldr      r3, [pc, #0x84]
003c3938: ldr      r1, [pc, #0x84]
003c393c: push     {r4, r5, r6, r7, lr}
003c3940: add      r3, pc, r3
003c3944: ldr      r5, [r3, r1]
003c3948: ldr      r1, [pc, #0x78]
003c394c: mov      r7, r2
003c3950: ldr      r2, [r5]
003c3954: ldr      r6, [r3, r1]
003c3958: sub      sp, sp, #0x24
003c395c: str      r2, [sp, #0x1c]
003c3960: mov      r0, r6
003c3964: bl       #0x337888
003c3968: ldr      r1, [pc, #0x5c]
003c396c: add      r4, sp, #4
003c3970: mov      r2, sp
003c3974: add      r1, pc, r1
003c3978: mov      r0, r4
003c397c: bl       #0x3140ec
003c3980: mov      r1, r4
003c3984: mov      r0, r6
003c3988: bl       #0x337a88
003c398c: mov      r0, r4
003c3990: bl       #0x318254
003c3994: mov      r2, #0
003c3998: mov      r0, r7
003c399c: mov      r1, #0x21
003c39a0: bl       #0x3a4d5c
003c39a4: ldr      r2, [sp, #0x1c]
003c39a8: ldr      r3, [r5]
003c39ac: cmp      r2, r3
003c39b0: bne      #0x3c39bc
003c39b4: add      sp, sp, #0x24
003c39b8: pop      {r4, r5, r6, r7, pc}
003c39bc: bl       #0x30e310
003c39c0: subseq   r1, sp, r0, asr r1
003c39c4: andeq    r4, r0, ip, lsr #1
003c39c8: andeq    r0, r0, r4, lsl #17
003c39cc: ldrsbeq  r1, [r0], #-0x4c

# _ZN6CSCast8OnUpdateEiP9CharacterP16CharStateMachine
003c0020: bx       lr

# _ZN6CSCast7OnEventEiP9CharacterP16CharStateMachineiPv
003c0024: bx       lr

# _ZN8CSScared7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c45c4: push     {r4, r5, r6, r7, r8, lr}
003c45c8: ldr      r4, [pc, #0x198]
003c45cc: ldr      r7, [pc, #0x198]
003c45d0: ldr      r1, [pc, #0x198]
003c45d4: add      r4, pc, r4
003c45d8: ldr      r3, [r4, r7]
003c45dc: ldr      r8, [r4, r1]
003c45e0: sub      sp, sp, #0x30
003c45e4: ldr      r3, [r3]
003c45e8: mov      r0, r8
003c45ec: mov      r5, r2
003c45f0: str      r3, [sp, #0x2c]
003c45f4: bl       #0x337888
003c45f8: ldr      r1, [pc, #0x174]
003c45fc: add      r6, sp, #0x14
003c4600: add      r2, sp, #0x10
003c4604: mov      r0, r6
003c4608: add      r1, pc, r1
003c460c: bl       #0x3140ec
003c4610: mov      r1, r6
003c4614: mov      r0, r8
003c4618: bl       #0x337a88
003c461c: mov      r0, r6
003c4620: bl       #0x318254
003c4624: mov      r3, #0x2240
003c4628: str      r3, [r5, #0x520]
003c462c: ldr      r3, [pc, #0x144]
003c4630: mov      r0, r5
003c4634: add      r6, r5, #0x490
003c4638: ldr      r3, [r4, r3]
003c463c: add      r6, r6, #0xc
003c4640: ldr      r8, [r3]
003c4644: bl       #0x3a3228
003c4648: ldr      r3, [pc, #0x12c]
003c464c: ldr      r1, [pc, #0x12c]
003c4650: ldr      r2, [r4, r3]
003c4654: mov      r3, #0xa0
003c4658: mla      r3, r3, r0, r8
003c465c: ldr      r0, [r2, #0x2c]
003c4660: ldr      r2, [pc, #0x11c]
003c4664: add      r1, pc, r1
003c4668: ldr      r8, [r3, #0x7c]
003c466c: add      r2, pc, r2
003c4670: bl       #0x4c4bdc
003c4674: ands     r0, r0, #0x100
003c4678: bne      #0x3c4758
003c467c: add      r1, r0, r8
003c4680: mov      r0, r6
003c4684: bl       #0x3cacb0
003c4688: mov      r3, #0
003c468c: movw     r0, #0x270e
003c4690: str      r3, [sp, #0xc]
003c4694: str      r3, [sp, #4]
003c4698: str      r3, [sp, #8]
003c469c: bl       #0x3c26a0
003c46a0: bl       #0x30e964
003c46a4: movw     r1, #0xb717
003c46a8: movt     r1, #0x38d1
003c46ac: bl       #0x30ed6c
003c46b0: movw     r1, #0xb717
003c46b4: movt     r1, #0x3951
003c46b8: bl       #0x30eba4
003c46bc: str      r0, [sp, #4]
003c46c0: movw     r0, #0x270e
003c46c4: bl       #0x3c26a0
003c46c8: bl       #0x30e964
003c46cc: movw     r1, #0xb717
003c46d0: movt     r1, #0x38d1
003c46d4: bl       #0x30ed6c
003c46d8: movw     r1, #0xb717
003c46dc: movt     r1, #0x3951
003c46e0: bl       #0x30eba4
003c46e4: str      r0, [sp, #8]
003c46e8: mov      r0, #0x64
003c46ec: bl       #0x3c26a0
003c46f0: cmp      r0, #0x31
003c46f4: ldrle    r3, [sp, #4]
003c46f8: mov      r0, #0x64
003c46fc: addle    r3, r3, #0x80000000
003c4700: strle    r3, [sp, #4]
003c4704: bl       #0x3c26a0
003c4708: cmp      r0, #0x31
003c470c: ldrle    r3, [sp, #8]
003c4710: add      r1, sp, #4
003c4714: addle    r3, r3, #0x80000000
003c4718: strle    r3, [sp, #8]
003c471c: ldr      r0, [r5, #0x378]
003c4720: bl       #0x405374
003c4724: mov      r0, r5
003c4728: bl       #0x3bc6b8
003c472c: ldr      r0, [r5, #0x2dc]
003c4730: cmp      r0, #0
003c4734: beq      #0x3c473c
003c4738: bl       #0x46eae0
003c473c: ldr      r3, [r4, r7]
003c4740: ldr      r2, [sp, #0x2c]
003c4744: ldr      r3, [r3]
003c4748: cmp      r2, r3
003c474c: bne      #0x3c4764
003c4750: add      sp, sp, #0x30
003c4754: pop      {r4, r5, r6, r7, r8, pc}
003c4758: mov      r0, r5
003c475c: bl       #0x3a53e0
003c4760: b        #0x3c467c
003c4764: bl       #0x30e310
003c4768: ldrheq   r0, [sp], #-0x4c
003c476c: andeq    r4, r0, ip, lsr #1
003c4770: andeq    r0, r0, r4, lsl #17
003c4774: subseq   r0, r0, r8, asr #16
003c4778: andeq    r4, r0, r4, asr #16
003c477c: strdeq   r3, r4, [r0], -r4
003c4780: subseq   r0, r0, r4, asr r5
003c4784: subseq   r0, r0, ip, asr r5

# _ZN8CSScared6OnBlurEiP9CharacterP16CharStateMachinei
003c4834: push     {r4, r5, r6, r7, r8, lr}
003c4838: ldr      r4, [pc, #0x90]
003c483c: ldr      r6, [pc, #0x90]
003c4840: ldr      r1, [pc, #0x90]
003c4844: add      r4, pc, r4
003c4848: ldr      r3, [r4, r6]
003c484c: ldr      r8, [r4, r1]
003c4850: sub      sp, sp, #0x20
003c4854: ldr      r3, [r3]
003c4858: mov      r0, r8
003c485c: mov      r7, r2
003c4860: str      r3, [sp, #0x1c]
003c4864: bl       #0x337888
003c4868: ldr      r1, [pc, #0x6c]
003c486c: add      r5, sp, #4
003c4870: mov      r2, sp
003c4874: add      r1, pc, r1
003c4878: mov      r0, r5
003c487c: bl       #0x3140ec
003c4880: mov      r1, r5
003c4884: mov      r0, r8
003c4888: bl       #0x337a88
003c488c: mov      r0, r5
003c4890: bl       #0x318254
003c4894: ldr      r0, [r7, #0x378]
003c4898: mov      r1, #0
003c489c: bl       #0x4053d0
003c48a0: ldr      r0, [r7, #0x2dc]
003c48a4: cmp      r0, #0
003c48a8: beq      #0x3c48b0
003c48ac: bl       #0x46eb20
003c48b0: ldr      r3, [r4, r6]
003c48b4: ldr      r2, [sp, #0x1c]
003c48b8: ldr      r3, [r3]
003c48bc: cmp      r2, r3
003c48c0: bne      #0x3c48cc
003c48c4: add      sp, sp, #0x20
003c48c8: pop      {r4, r5, r6, r7, r8, pc}
003c48cc: bl       #0x30e310
003c48d0: subseq   r0, sp, ip, asr #4
003c48d4: andeq    r4, r0, ip, lsr #1
003c48d8: andeq    r0, r0, r4, lsl #17
003c48dc: ldrsbeq  r0, [r0], #-0x5c

# _ZN8CSScared8OnUpdateEiP9CharacterP16CharStateMachine
003c4788: push     {r4, r5, r6, r7, r8, sl, lr}
003c478c: ldr      r4, [pc, #0x90]
003c4790: ldr      r5, [pc, #0x90]
003c4794: ldr      r8, [r2, #0x528]
003c4798: add      r4, pc, r4
003c479c: ldr      r3, [r4, r5]
003c47a0: sub      sp, sp, #0x24
003c47a4: ands     r8, r8, #4
003c47a8: ldr      r3, [r3]
003c47ac: mov      r6, r2
003c47b0: str      r3, [sp, #0x1c]
003c47b4: bne      #0x3c4804
003c47b8: ldr      r3, [pc, #0x6c]
003c47bc: add      r7, sp, #4
003c47c0: ldr      sl, [r4, r3]
003c47c4: mov      r0, sl
003c47c8: bl       #0x337888
003c47cc: ldr      r1, [pc, #0x5c]
003c47d0: mov      r2, sp
003c47d4: mov      r0, r7
003c47d8: add      r1, pc, r1
003c47dc: bl       #0x3140ec
003c47e0: mov      r1, r7
003c47e4: mov      r0, sl
003c47e8: bl       #0x337a88
003c47ec: mov      r0, r7
003c47f0: bl       #0x318254
003c47f4: add      r0, r6, #0x490
003c47f8: add      r0, r0, #0xc
003c47fc: mov      r1, r8
003c4800: bl       #0x3c948c
003c4804: ldr      r3, [r4, r5]
003c4808: ldr      r2, [sp, #0x1c]
003c480c: ldr      r3, [r3]
003c4810: cmp      r2, r3
003c4814: bne      #0x3c4820
003c4818: add      sp, sp, #0x24
003c481c: pop      {r4, r5, r6, r7, r8, sl, pc}
003c4820: bl       #0x30e310
003c4824: ldrsheq  r0, [sp], #-0x28
003c4828: andeq    r4, r0, ip, lsr #1
003c482c: andeq    r0, r0, r4, lsl #17
003c4830: ldrsheq  r0, [r0], #-0x60

# _ZN8CSScared7OnEventEiP9CharacterP16CharStateMachineiPv
003c2b28: push     {r4, lr}
003c2b2c: sub      sp, sp, #0x10
003c2b30: ldr      r3, [sp, #0x18]
003c2b34: mov      r4, r2
003c2b38: cmp      r3, #0x23
003c2b3c: bne      #0x3c2bdc
003c2b40: mov      r3, #0
003c2b44: movw     r0, #0x270e
003c2b48: str      r3, [sp, #0xc]
003c2b4c: str      r3, [sp, #4]
003c2b50: str      r3, [sp, #8]
003c2b54: bl       #0x3c26a0
003c2b58: bl       #0x30e964
003c2b5c: movw     r1, #0xb717
003c2b60: movt     r1, #0x38d1
003c2b64: bl       #0x30ed6c
003c2b68: movw     r1, #0xb717
003c2b6c: movt     r1, #0x3951
003c2b70: bl       #0x30eba4
003c2b74: str      r0, [sp, #4]
003c2b78: movw     r0, #0x270e
003c2b7c: bl       #0x3c26a0
003c2b80: bl       #0x30e964
003c2b84: movw     r1, #0xb717
003c2b88: movt     r1, #0x38d1
003c2b8c: bl       #0x30ed6c
003c2b90: movw     r1, #0xb717
003c2b94: movt     r1, #0x3951
003c2b98: bl       #0x30eba4
003c2b9c: str      r0, [sp, #8]
003c2ba0: mov      r0, #0x64
003c2ba4: bl       #0x3c26a0
003c2ba8: cmp      r0, #0x31
003c2bac: ldrle    r3, [sp, #4]
003c2bb0: mov      r0, #0x64
003c2bb4: addle    r3, r3, #0x80000000
003c2bb8: strle    r3, [sp, #4]
003c2bbc: bl       #0x3c26a0
003c2bc0: cmp      r0, #0x31
003c2bc4: ldrle    r3, [sp, #8]
003c2bc8: add      r1, sp, #4
003c2bcc: addle    r3, r3, #0x80000000
003c2bd0: strle    r3, [sp, #8]
003c2bd4: ldr      r0, [r4, #0x378]
003c2bd8: bl       #0x405374
003c2bdc: add      sp, sp, #0x10
003c2be0: pop      {r4, pc}

# _ZN9CSStunned7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3cc0: push     {r4, r5, r6, r7, r8, lr}
003c3cc4: ldr      r4, [pc, #0x150]
003c3cc8: ldr      r8, [pc, #0x150]
003c3ccc: ldr      r1, [pc, #0x150]
003c3cd0: add      r4, pc, r4
003c3cd4: ldr      r3, [r4, r8]
003c3cd8: ldr      r6, [r4, r1]
003c3cdc: sub      sp, sp, #0x40
003c3ce0: ldr      r3, [r3]
003c3ce4: mov      r0, r6
003c3ce8: mov      r5, r2
003c3cec: str      r3, [sp, #0x3c]
003c3cf0: bl       #0x337888
003c3cf4: ldr      r1, [pc, #0x12c]
003c3cf8: add      r7, sp, #0x24
003c3cfc: add      r2, sp, #8
003c3d00: mov      r0, r7
003c3d04: add      r1, pc, r1
003c3d08: bl       #0x3140ec
003c3d0c: mov      r1, r7
003c3d10: mov      r0, r6
003c3d14: bl       #0x337a88
003c3d18: mov      r0, r7
003c3d1c: bl       #0x318254
003c3d20: mov      r0, r6
003c3d24: bl       #0x337888
003c3d28: ldr      r1, [pc, #0xfc]
003c3d2c: add      r7, sp, #0xc
003c3d30: add      r2, sp, #4
003c3d34: add      r1, pc, r1
003c3d38: mov      r0, r7
003c3d3c: bl       #0x3140ec
003c3d40: mov      r1, r7
003c3d44: mov      r0, r6
003c3d48: bl       #0x337a88
003c3d4c: mov      r0, r7
003c3d50: bl       #0x318254
003c3d54: movw     r3, #0x2202
003c3d58: str      r3, [r5, #0x520]
003c3d5c: ldr      r3, [pc, #0xcc]
003c3d60: mov      r0, r5
003c3d64: add      r6, r5, #0x490
003c3d68: ldr      r3, [r4, r3]
003c3d6c: add      r6, r6, #0xc
003c3d70: ldr      r7, [r3]
003c3d74: bl       #0x3a3228
003c3d78: ldr      r3, [pc, #0xb4]
003c3d7c: ldr      r1, [pc, #0xb4]
003c3d80: ldr      r2, [r4, r3]
003c3d84: mov      r3, #0xa0
003c3d88: mla      r3, r3, r0, r7
003c3d8c: ldr      r0, [r2, #0x2c]
003c3d90: ldr      r2, [pc, #0xa4]
003c3d94: add      r1, pc, r1
003c3d98: ldr      r7, [r3, #0x8c]
003c3d9c: add      r2, pc, r2
003c3da0: bl       #0x4c4bdc
003c3da4: ands     r0, r0, #0x200
003c3da8: bne      #0x3c3e0c
003c3dac: add      r1, r0, r7
003c3db0: mov      r0, r6
003c3db4: bl       #0x3cacb0
003c3db8: ldr      r3, [r5]
003c3dbc: mov      r0, r5
003c3dc0: mov      lr, pc
003c3dc4: ldr      pc, [r3, #0x28]
003c3dc8: cmp      r0, #0
003c3dcc: ldrne    r3, [r5, #0x378]
003c3dd0: movne    r2, #1
003c3dd4: mov      r0, r5
003c3dd8: strbne   r2, [r3, #8]
003c3ddc: bl       #0x3bc6b8
003c3de0: ldr      r0, [r5, #0x2dc]
003c3de4: cmp      r0, #0
003c3de8: beq      #0x3c3df0
003c3dec: bl       #0x46eae0
003c3df0: ldr      r3, [r4, r8]
003c3df4: ldr      r2, [sp, #0x3c]
003c3df8: ldr      r3, [r3]
003c3dfc: cmp      r2, r3
003c3e00: bne      #0x3c3e18
003c3e04: add      sp, sp, #0x40
003c3e08: pop      {r4, r5, r6, r7, r8, pc}
003c3e0c: mov      r0, r5
003c3e10: bl       #0x3a53e0
003c3e14: b        #0x3c3dac
003c3e18: bl       #0x30e310
003c3e1c: subseq   r0, sp, r0, asr #27
003c3e20: andeq    r4, r0, ip, lsr #1
003c3e24: andeq    r0, r0, r4, lsl #17
003c3e28: subseq   r1, r0, ip, asr #2
003c3e2c: subseq   r1, r0, ip, ror r1
003c3e30: andeq    r4, r0, r4, asr #16
003c3e34: strdeq   r3, r4, [r0], -r4
003c3e38: subseq   r0, r0, r4, lsr #28
003c3e3c: subseq   r0, r0, ip, lsr #28

# _ZN9CSStunned6OnBlurEiP9CharacterP16CharStateMachinei
003c3b4c: push     {r4, r5, r6, r7, r8, lr}
003c3b50: ldr      r4, [pc, #0x90]
003c3b54: ldr      r6, [pc, #0x90]
003c3b58: ldr      r1, [pc, #0x90]
003c3b5c: add      r4, pc, r4
003c3b60: ldr      r3, [r4, r6]
003c3b64: ldr      r8, [r4, r1]
003c3b68: sub      sp, sp, #0x20
003c3b6c: ldr      r3, [r3]
003c3b70: mov      r0, r8
003c3b74: mov      r7, r2
003c3b78: str      r3, [sp, #0x1c]
003c3b7c: bl       #0x337888
003c3b80: ldr      r1, [pc, #0x6c]
003c3b84: add      r5, sp, #4
003c3b88: mov      r2, sp
003c3b8c: add      r1, pc, r1
003c3b90: mov      r0, r5
003c3b94: bl       #0x3140ec
003c3b98: mov      r1, r5
003c3b9c: mov      r0, r8
003c3ba0: bl       #0x337a88
003c3ba4: mov      r0, r5
003c3ba8: bl       #0x318254
003c3bac: ldr      r3, [r7, #0x378]
003c3bb0: mov      r2, #0
003c3bb4: strb     r2, [r3, #8]
003c3bb8: ldr      r0, [r7, #0x2dc]
003c3bbc: cmp      r0, r2
003c3bc0: beq      #0x3c3bc8
003c3bc4: bl       #0x46eb20
003c3bc8: ldr      r3, [r4, r6]
003c3bcc: ldr      r2, [sp, #0x1c]
003c3bd0: ldr      r3, [r3]
003c3bd4: cmp      r2, r3
003c3bd8: bne      #0x3c3be4
003c3bdc: add      sp, sp, #0x20
003c3be0: pop      {r4, r5, r6, r7, r8, pc}
003c3be4: bl       #0x30e310
003c3be8: subseq   r0, sp, r4, lsr pc
003c3bec: andeq    r4, r0, ip, lsr #1
003c3bf0: andeq    r0, r0, r4, lsl #17
003c3bf4: subseq   r1, r0, r4, asr #5

# _ZN9CSStunned8OnUpdateEiP9CharacterP16CharStateMachine
003c549c: push     {r4, r5, r6, r7, r8, sl, lr}
003c54a0: ldr      r4, [pc, #0x90]
003c54a4: ldr      r5, [pc, #0x90]
003c54a8: ldr      r8, [r2, #0x528]
003c54ac: add      r4, pc, r4
003c54b0: ldr      r3, [r4, r5]
003c54b4: sub      sp, sp, #0x24
003c54b8: ands     r8, r8, #2
003c54bc: ldr      r3, [r3]
003c54c0: mov      r6, r2
003c54c4: str      r3, [sp, #0x1c]
003c54c8: bne      #0x3c5518
003c54cc: ldr      r3, [pc, #0x6c]
003c54d0: add      r7, sp, #4
003c54d4: ldr      sl, [r4, r3]
003c54d8: mov      r0, sl
003c54dc: bl       #0x337888
003c54e0: ldr      r1, [pc, #0x5c]
003c54e4: mov      r2, sp
003c54e8: mov      r0, r7
003c54ec: add      r1, pc, r1
003c54f0: bl       #0x3140ec
003c54f4: mov      r1, r7
003c54f8: mov      r0, sl
003c54fc: bl       #0x337a88
003c5500: mov      r0, r7
003c5504: bl       #0x318254
003c5508: add      r0, r6, #0x4f0
003c550c: add      r0, r0, #0xc
003c5510: mov      r1, r8
003c5514: bl       #0x3c1a00
003c5518: ldr      r3, [r4, r5]
003c551c: ldr      r2, [sp, #0x1c]
003c5520: ldr      r3, [r3]
003c5524: cmp      r2, r3
003c5528: bne      #0x3c5534
003c552c: add      sp, sp, #0x24
003c5530: pop      {r4, r5, r6, r7, r8, sl, pc}
003c5534: bl       #0x30e310
003c5538: subseq   pc, ip, r4, ror #11
003c553c: andeq    r4, r0, ip, lsr #1
003c5540: andeq    r0, r0, r4, lsl #17
003c5544: subeq    pc, pc, r4, asr #19

# _ZN9CSStunned7OnEventEiP9CharacterP16CharStateMachineiPv
003c0030: bx       lr

# _ZN13CSKnockedBack7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c4a48: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c4a4c: ldr      r5, [pc, #0x13c]
003c4a50: ldr      r8, [pc, #0x13c]
003c4a54: ldr      r0, [pc, #0x13c]
003c4a58: add      r5, pc, r5
003c4a5c: ldr      r1, [r5, r8]
003c4a60: ldr      r6, [r5, r0]
003c4a64: mov      r7, r3
003c4a68: ldr      r3, [r1]
003c4a6c: sub      sp, sp, #0x48
003c4a70: mov      r0, r6
003c4a74: str      r3, [sp, #0x44]
003c4a78: mov      r4, r2
003c4a7c: ldr      sb, [sp, #0x70]
003c4a80: bl       #0x337888
003c4a84: ldr      r1, [pc, #0x110]
003c4a88: add      sl, sp, #0x2c
003c4a8c: add      r2, sp, #0x10
003c4a90: add      r1, pc, r1
003c4a94: mov      r0, sl
003c4a98: bl       #0x3140ec
003c4a9c: mov      r1, sl
003c4aa0: mov      r0, r6
003c4aa4: bl       #0x337a88
003c4aa8: mov      r0, sl
003c4aac: bl       #0x318254
003c4ab0: mov      r0, r6
003c4ab4: bl       #0x337888
003c4ab8: ldr      r1, [pc, #0xe0]
003c4abc: add      sl, sp, #0x14
003c4ac0: add      r2, sp, #0xc
003c4ac4: add      r1, pc, r1
003c4ac8: mov      r0, sl
003c4acc: bl       #0x3140ec
003c4ad0: mov      r1, sl
003c4ad4: mov      r0, r6
003c4ad8: bl       #0x337a88
003c4adc: mov      r0, sl
003c4ae0: bl       #0x318254
003c4ae4: movw     r3, #0x2341
003c4ae8: add      r0, r4, #0x4f0
003c4aec: str      r3, [r4, #0x520]
003c4af0: add      r0, r0, #0xc
003c4af4: mvn      r1, #0
003c4af8: bl       #0x3c0b50
003c4afc: ldr      r3, [r7, #0x2c]
003c4b00: tst      r3, #8
003c4b04: beq      #0x3c4b30
003c4b08: ldr      r0, [r4, #0x2dc]
003c4b0c: cmp      r0, #0
003c4b10: beq      #0x3c4b30
003c4b14: mov      ip, #0
003c4b18: mov      r3, #3
003c4b1c: mov      r1, ip
003c4b20: movw     r2, #0x51c
003c4b24: str      ip, [sp]
003c4b28: bl       #0x46ece8
003c4b2c: ldr      r3, [r7, #0x2c]
003c4b30: tst      r3, #0x10
003c4b34: movne    r3, #0x20
003c4b38: biceq    r3, r3, #0x20
003c4b3c: str      r3, [r7, #0x2c]
003c4b40: ldr      r3, [r4, #0x378]
003c4b44: mov      r2, #1
003c4b48: mov      r1, sb
003c4b4c: strb     r2, [r3, #8]
003c4b50: mov      r0, r4
003c4b54: bl       #0x393d48
003c4b58: mov      r0, r4
003c4b5c: bl       #0x3bc6b8
003c4b60: ldr      r0, [r4, #0x2dc]
003c4b64: cmp      r0, #0
003c4b68: beq      #0x3c4b70
003c4b6c: bl       #0x46eae0
003c4b70: ldr      r3, [r5, r8]
003c4b74: ldr      r2, [sp, #0x44]
003c4b78: ldr      r3, [r3]
003c4b7c: cmp      r2, r3
003c4b80: bne      #0x3c4b8c
003c4b84: add      sp, sp, #0x48
003c4b88: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c4b8c: bl       #0x30e310
003c4b90: subseq   r0, sp, r8, lsr r0
003c4b94: andeq    r4, r0, ip, lsr #1
003c4b98: andeq    r0, r0, r4, lsl #17
003c4b9c: subseq   r0, r0, r0, asr #7
003c4ba0: subseq   r0, r0, ip, lsl r4

# _ZN13CSKnockedBack6OnBlurEiP9CharacterP16CharStateMachinei
003c48e0: push     {r4, r5, r6, r7, r8, lr}
003c48e4: ldr      r4, [pc, #0xa0]
003c48e8: ldr      r7, [pc, #0xa0]
003c48ec: ldr      r1, [pc, #0xa0]
003c48f0: add      r4, pc, r4
003c48f4: ldr      r3, [r4, r7]
003c48f8: ldr      r8, [r4, r1]
003c48fc: sub      sp, sp, #0x20
003c4900: ldr      r3, [r3]
003c4904: mov      r0, r8
003c4908: mov      r6, r2
003c490c: str      r3, [sp, #0x1c]
003c4910: bl       #0x337888
003c4914: ldr      r1, [pc, #0x7c]
003c4918: add      r5, sp, #4
003c491c: mov      r2, sp
003c4920: add      r1, pc, r1
003c4924: mov      r0, r5
003c4928: bl       #0x3140ec
003c492c: mov      r1, r5
003c4930: mov      r0, r8
003c4934: bl       #0x337a88
003c4938: mov      r0, r5
003c493c: bl       #0x318254
003c4940: ldr      r3, [r6, #0x378]
003c4944: mov      r2, #0
003c4948: strb     r2, [r3, #8]
003c494c: ldr      r0, [r6, #0x2dc]
003c4950: cmp      r0, r2
003c4954: beq      #0x3c496c
003c4958: bl       #0x46ec6c
003c495c: ldr      r0, [r6, #0x2dc]
003c4960: cmp      r0, #0
003c4964: beq      #0x3c496c
003c4968: bl       #0x46eb20
003c496c: ldr      r3, [r4, r7]
003c4970: ldr      r2, [sp, #0x1c]
003c4974: ldr      r3, [r3]
003c4978: cmp      r2, r3
003c497c: bne      #0x3c4988
003c4980: add      sp, sp, #0x20
003c4984: pop      {r4, r5, r6, r7, r8, pc}
003c4988: bl       #0x30e310
003c498c: subseq   r0, sp, r0, lsr #3
003c4990: andeq    r4, r0, ip, lsr #1
003c4994: andeq    r0, r0, r4, lsl #17
003c4998: subseq   r0, r0, r0, lsr r5

# _ZN13CSKnockedBack8OnUpdateEiP9CharacterP16CharStateMachine
003c0038: bx       lr

# _ZN13CSKnockedBack7OnEventEiP9CharacterP16CharStateMachineiPv
003c5ab0: str      lr, [sp, #-4]!
003c5ab4: sub      sp, sp, #0xc
003c5ab8: ldr      r3, [sp, #0x10]
003c5abc: cmp      r3, #0x23
003c5ac0: beq      #0x3c5aec
003c5ac4: cmp      r3, #0x27
003c5ac8: beq      #0x3c5ad4
003c5acc: add      sp, sp, #0xc
003c5ad0: ldm      sp!, {pc}
003c5ad4: ldr      r0, [r2, #0x2dc]
003c5ad8: cmp      r0, #0
003c5adc: beq      #0x3c5acc
003c5ae0: add      sp, sp, #0xc
003c5ae4: pop      {lr}
003c5ae8: b        #0x46ec6c
003c5aec: ldr      r3, [r2]
003c5af0: mov      r0, r2
003c5af4: str      r2, [sp, #4]
003c5af8: mov      lr, pc
003c5afc: ldr      pc, [r3, #0x34]
003c5b00: cmp      r0, #0
003c5b04: ldr      r2, [sp, #4]
003c5b08: beq      #0x3c5acc
003c5b0c: add      r0, r2, #0x490
003c5b10: add      r0, r0, #0xc
003c5b14: bl       #0x3c9924
003c5b18: ldr      r2, [sp, #4]
003c5b1c: mov      r1, #1
003c5b20: mov      r3, r1
003c5b24: add      r0, r2, #0x4f0
003c5b28: add      r0, r0, #0xc
003c5b2c: mov      r2, #0
003c5b30: add      sp, sp, #0xc
003c5b34: pop      {lr}
003c5b38: b        #0x3c58c8

# _ZN9CSInjured7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c33e8: ldr      r3, [pc, #0xc8]
003c33ec: ldr      r1, [pc, #0xc8]
003c33f0: push     {r4, r5, r6, r7, lr}
003c33f4: add      r3, pc, r3
003c33f8: ldr      r7, [r3, r1]
003c33fc: ldr      r1, [pc, #0xbc]
003c3400: mov      r5, r2
003c3404: ldr      r2, [r7]
003c3408: ldr      r4, [r3, r1]
003c340c: sub      sp, sp, #0x44
003c3410: str      r2, [sp, #0x3c]
003c3414: mov      r0, r4
003c3418: bl       #0x337888
003c341c: ldr      r1, [pc, #0xa0]
003c3420: add      r6, sp, #0x24
003c3424: add      r2, sp, #8
003c3428: mov      r0, r6
003c342c: add      r1, pc, r1
003c3430: bl       #0x3140ec
003c3434: mov      r1, r6
003c3438: mov      r0, r4
003c343c: bl       #0x337a88
003c3440: mov      r0, r6
003c3444: bl       #0x318254
003c3448: mov      r0, r4
003c344c: bl       #0x337888
003c3450: ldr      r1, [pc, #0x70]
003c3454: add      r6, sp, #0xc
003c3458: add      r2, sp, #4
003c345c: add      r1, pc, r1
003c3460: mov      r0, r6
003c3464: bl       #0x3140ec
003c3468: mov      r1, r6
003c346c: mov      r0, r4
003c3470: bl       #0x337a88
003c3474: mov      r0, r6
003c3478: bl       #0x318254
003c347c: movw     r3, #0x2b41
003c3480: add      r0, r5, #0x4f0
003c3484: str      r3, [r5, #0x520]
003c3488: add      r0, r0, #0xc
003c348c: mvn      r1, #0
003c3490: bl       #0x3c0b50
003c3494: mov      r0, r5
003c3498: bl       #0x3bc6b8
003c349c: ldr      r2, [sp, #0x3c]
003c34a0: ldr      r3, [r7]
003c34a4: cmp      r2, r3
003c34a8: bne      #0x3c34b4
003c34ac: add      sp, sp, #0x44
003c34b0: pop      {r4, r5, r6, r7, pc}
003c34b4: bl       #0x30e310

# _ZN9CSInjured6OnBlurEiP9CharacterP16CharStateMachinei
003c4ba4: ldr      r3, [pc, #0x80]
003c4ba8: ldr      r1, [pc, #0x80]
003c4bac: push     {r4, r5, r6, r7, lr}
003c4bb0: add      r3, pc, r3
003c4bb4: ldr      r5, [r3, r1]
003c4bb8: ldr      r1, [pc, #0x74]
003c4bbc: mov      r6, r2
003c4bc0: ldr      r2, [r5]
003c4bc4: ldr      r7, [r3, r1]
003c4bc8: sub      sp, sp, #0x24
003c4bcc: str      r2, [sp, #0x1c]
003c4bd0: mov      r0, r7
003c4bd4: bl       #0x337888
003c4bd8: ldr      r1, [pc, #0x58]
003c4bdc: add      r4, sp, #4
003c4be0: mov      r2, sp
003c4be4: add      r1, pc, r1
003c4be8: mov      r0, r4
003c4bec: bl       #0x3140ec
003c4bf0: mov      r1, r4
003c4bf4: mov      r0, r7
003c4bf8: bl       #0x337a88
003c4bfc: mov      r0, r4
003c4c00: bl       #0x318254
003c4c04: ldr      r1, [r6, #0x408]
003c4c08: ldr      r0, [r6, #0x378]
003c4c0c: bl       #0x4052bc
003c4c10: ldr      r2, [sp, #0x1c]
003c4c14: ldr      r3, [r5]
003c4c18: cmp      r2, r3
003c4c1c: bne      #0x3c4c28
003c4c20: add      sp, sp, #0x24
003c4c24: pop      {r4, r5, r6, r7, pc}
003c4c28: bl       #0x30e310
003c4c2c: subseq   pc, ip, r0, ror #29
003c4c30: andeq    r4, r0, ip, lsr #1
003c4c34: andeq    r0, r0, r4, lsl #17
003c4c38: subseq   r0, r0, ip, ror #4

# _ZN9CSInjured8OnUpdateEiP9CharacterP16CharStateMachine
003c0040: bx       lr

# _ZN9CSInjured7OnEventEiP9CharacterP16CharStateMachineiPv
003c0044: bx       lr

# _ZN10CSInteract7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c5548: push     {r4, r5, r6, r7, r8, sl, lr}
003c554c: ldr      r4, [pc, #0x11c]
003c5550: ldr      r8, [pc, #0x11c]
003c5554: ldr      r1, [pc, #0x11c]
003c5558: add      r4, pc, r4
003c555c: ldr      r3, [r4, r8]
003c5560: ldr      r6, [r4, r1]
003c5564: sub      sp, sp, #0x44
003c5568: ldr      r3, [r3]
003c556c: mov      r0, r6
003c5570: mov      r5, r2
003c5574: str      r3, [sp, #0x3c]
003c5578: ldr      sl, [sp, #0x68]
003c557c: bl       #0x337888
003c5580: ldr      r1, [pc, #0xf4]
003c5584: add      r7, sp, #0x24
003c5588: add      r2, sp, #8
003c558c: add      r1, pc, r1
003c5590: mov      r0, r7
003c5594: bl       #0x3140ec
003c5598: mov      r1, r7
003c559c: mov      r0, r6
003c55a0: bl       #0x337a88
003c55a4: mov      r0, r7
003c55a8: bl       #0x318254
003c55ac: mov      r0, r6
003c55b0: bl       #0x337888
003c55b4: ldr      r1, [pc, #0xc4]
003c55b8: add      r7, sp, #0xc
003c55bc: add      r2, sp, #4
003c55c0: add      r1, pc, r1
003c55c4: mov      r0, r7
003c55c8: bl       #0x3140ec
003c55cc: mov      r1, r7
003c55d0: mov      r0, r6
003c55d4: bl       #0x337a88
003c55d8: mov      r0, r7
003c55dc: bl       #0x318254
003c55e0: mov      r0, r5
003c55e4: bl       #0x3a3064
003c55e8: cmp      r0, #0
003c55ec: bne      #0x3c5658
003c55f0: movw     r3, #0x63c1
003c55f4: add      r0, r5, #0x4f0
003c55f8: str      r3, [r5, #0x520]
003c55fc: add      r0, r0, #0xc
003c5600: mvn      r1, #0
003c5604: bl       #0x3c0b50
003c5608: mov      r0, r5
003c560c: bl       #0x3a316c
003c5610: cmp      r0, #0
003c5614: beq      #0x3c563c
003c5618: mov      r0, r5
003c561c: bl       #0x3bc6b8
003c5620: ldr      r3, [r4, r8]
003c5624: ldr      r2, [sp, #0x3c]
003c5628: ldr      r3, [r3]
003c562c: cmp      r2, r3
003c5630: bne      #0x3c566c
003c5634: add      sp, sp, #0x44
003c5638: pop      {r4, r5, r6, r7, r8, sl, pc}
003c563c: ldrb     r3, [r5, #0x548]
003c5640: cmp      r3, #0
003c5644: beq      #0x3c5618
003c5648: mov      r1, sl
003c564c: ldr      r0, [r5, #0x378]
003c5650: bl       #0x4052bc
003c5654: b        #0x3c5618
003c5658: add      r0, r5, #0x4f0
003c565c: add      r0, r0, #0xc
003c5660: mov      r1, #0
003c5664: bl       #0x3c1a00
003c5668: b        #0x3c5620
003c566c: bl       #0x30e310
003c5670: subseq   pc, ip, r8, lsr r5
003c5674: andeq    r4, r0, ip, lsr #1
003c5678: andeq    r0, r0, r4, lsl #17
003c567c: subeq    pc, pc, r4, asr #17
003c5680: subeq    pc, pc, r8, asr sb

# _ZN10CSInteract6OnBlurEiP9CharacterP16CharStateMachinei
003c4f8c: push     {r4, r5, r6, r7, r8, sl, lr}
003c4f90: ldr      r4, [pc, #0x10c]
003c4f94: ldr      r7, [pc, #0x10c]
003c4f98: ldr      r1, [pc, #0x10c]
003c4f9c: add      r4, pc, r4
003c4fa0: ldr      r3, [r4, r7]
003c4fa4: ldr      r8, [r4, r1]
003c4fa8: sub      sp, sp, #0x24
003c4fac: ldr      r3, [r3]
003c4fb0: mov      r0, r8
003c4fb4: mov      r5, r2
003c4fb8: str      r3, [sp, #0x1c]
003c4fbc: ldr      sl, [sp, #0x40]
003c4fc0: bl       #0x337888
003c4fc4: ldr      r1, [pc, #0xe4]
003c4fc8: add      r6, sp, #4
003c4fcc: mov      r2, sp
003c4fd0: add      r1, pc, r1
003c4fd4: mov      r0, r6
003c4fd8: bl       #0x3140ec
003c4fdc: mov      r1, r6
003c4fe0: mov      r0, r8
003c4fe4: bl       #0x337a88
003c4fe8: mov      r0, r6
003c4fec: bl       #0x318254
003c4ff0: mov      r0, r5
003c4ff4: bl       #0x3a316c
003c4ff8: cmp      r0, #0
003c4ffc: beq      #0x3c5054
003c5000: ldr      r3, [r5, #0x544]
003c5004: sub      r3, r3, #4
003c5008: cmp      r3, #1
003c500c: bhi      #0x3c5038
003c5010: cmp      sl, #0x13
003c5014: bls      #0x3c5078
003c5018: ldr      r0, [r5, #0x54c]
003c501c: cmp      r0, #0
003c5020: beq      #0x3c5038
003c5024: ldr      r3, [r0, #0xf4]
003c5028: cmp      r3, #6
003c502c: beq      #0x3c508c
003c5030: mov      r3, #0
003c5034: str      r3, [r5, #0x54c]
003c5038: ldr      r3, [r4, r7]
003c503c: ldr      r2, [sp, #0x1c]
003c5040: ldr      r3, [r3]
003c5044: cmp      r2, r3
003c5048: bne      #0x3c50a0
003c504c: add      sp, sp, #0x24
003c5050: pop      {r4, r5, r6, r7, r8, sl, pc}
003c5054: mov      r0, r5
003c5058: bl       #0x3a310c
003c505c: cmp      r0, #0
003c5060: beq      #0x3c5000
003c5064: add      r1, r5, #0x1440
003c5068: add      r1, r1, #0x1c
003c506c: mov      r0, r5
003c5070: bl       #0x3938a0
003c5074: b        #0x3c5000
003c5078: mov      r3, #1
003c507c: lsl      sl, r3, sl
003c5080: tst      sl, #0xc2000
003c5084: bne      #0x3c5038
003c5088: b        #0x3c5018
003c508c: ldr      r3, [r0, #0x390]
003c5090: cmp      r3, r5
003c5094: bne      #0x3c5030
003c5098: bl       #0x3ee5a8
003c509c: b        #0x3c5030
003c50a0: bl       #0x30e310
003c50a4: ldrsheq  pc, [ip], #-0xa4
003c50a8: andeq    r4, r0, ip, lsr #1
003c50ac: andeq    r0, r0, r4, lsl #17
003c50b0: subeq    pc, pc, r0, lsl #29

# _ZN10CSInteract8OnUpdateEiP9CharacterP16CharStateMachine
003c0080: bx       lr

# _ZN10CSInteract7OnEventEiP9CharacterP16CharStateMachineiPv
003c1558: push     {r4, r5, r6, lr}
003c155c: ldr      r3, [sp, #0x10]
003c1560: mov      r4, r2
003c1564: ldr      r5, [sp, #0x14]
003c1568: cmp      r3, #0x28
003c156c: beq      #0x3c1574
003c1570: pop      {r4, r5, r6, pc}
003c1574: ldr      r6, [r2, #0x54c]
003c1578: cmp      r6, #0
003c157c: beq      #0x3c1570
003c1580: ldr      r1, [pc, #0x70]
003c1584: mov      r0, r5
003c1588: add      r1, pc, r1
003c158c: bl       #0x30e31c
003c1590: cmp      r0, #0
003c1594: bne      #0x3c15b8
003c1598: ldr      r3, [r6, #0xf4]
003c159c: cmp      r3, #6
003c15a0: bne      #0x3c15ac
003c15a4: mov      r0, r6
003c15a8: bl       #0x3ee468
003c15ac: mov      r3, #0
003c15b0: str      r3, [r4, #0x54c]
003c15b4: pop      {r4, r5, r6, pc}
003c15b8: ldr      r1, [pc, #0x3c]
003c15bc: mov      r0, r5
003c15c0: add      r1, pc, r1
003c15c4: bl       #0x30e31c
003c15c8: cmp      r0, #0
003c15cc: bne      #0x3c1570
003c15d0: ldr      r3, [r6, #0xf4]
003c15d4: cmp      r3, #6
003c15d8: bne      #0x3c1570
003c15dc: mov      r0, r6
003c15e0: mov      r1, r4
003c15e4: bl       #0x3ee5fc
003c15e8: cmp      r0, #0
003c15ec: mvneq    r3, #0
003c15f0: streq    r3, [r4, #0x544]
003c15f4: b        #0x3c1570
003c15f8: subseq   r3, r0, r0, asr r6
003c15fc: subseq   r3, r0, r0, lsr #12

# _ZN6CSAnim7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c324c: ldr      r3, [pc, #0x98]
003c3250: ldr      r1, [pc, #0x98]
003c3254: push     {r4, r5, r6, r7, lr}
003c3258: add      r3, pc, r3
003c325c: ldr      r6, [r3, r1]
003c3260: ldr      r1, [pc, #0x8c]
003c3264: mov      r4, r2
003c3268: ldr      r2, [r6]
003c326c: ldr      r7, [r3, r1]
003c3270: sub      sp, sp, #0x24
003c3274: str      r2, [sp, #0x1c]
003c3278: mov      r0, r7
003c327c: bl       #0x337888
003c3280: ldr      r1, [pc, #0x70]
003c3284: add      r5, sp, #4
003c3288: mov      r2, sp
003c328c: add      r1, pc, r1
003c3290: mov      r0, r5
003c3294: bl       #0x3140ec
003c3298: mov      r1, r5
003c329c: mov      r0, r7
003c32a0: bl       #0x337a88
003c32a4: mov      r0, r5
003c32a8: bl       #0x318254
003c32ac: ldrb     r3, [r4, #0x541]
003c32b0: add      r0, r4, #0x4f0
003c32b4: add      r0, r0, #0xc
003c32b8: cmp      r3, #0
003c32bc: movne    r3, #0x2240
003c32c0: moveq    r3, #0x2340
003c32c4: str      r3, [r4, #0x520]
003c32c8: mvn      r1, #0
003c32cc: bl       #0x3c0b50
003c32d0: ldr      r2, [sp, #0x1c]
003c32d4: ldr      r3, [r6]
003c32d8: cmp      r2, r3
003c32dc: bne      #0x3c32e8
003c32e0: add      sp, sp, #0x24
003c32e4: pop      {r4, r5, r6, r7, pc}
003c32e8: bl       #0x30e310
003c32ec: subseq   r1, sp, r8, lsr r8
003c32f0: andeq    r4, r0, ip, lsr #1
003c32f4: andeq    r0, r0, r4, lsl #17
003c32f8: subseq   r1, r0, r4, asr #23

# _ZN6CSAnim6OnBlurEiP9CharacterP16CharStateMachinei
003c2dd0: ldr      r3, [pc, #0x70]
003c2dd4: ldr      r2, [pc, #0x70]
003c2dd8: push     {r4, r5, r6, lr}
003c2ddc: add      r3, pc, r3
003c2de0: ldr      r5, [r3, r2]
003c2de4: ldr      r2, [pc, #0x64]
003c2de8: sub      sp, sp, #0x20
003c2dec: add      r4, sp, #4
003c2df0: ldr      r6, [r3, r2]
003c2df4: ldr      r3, [r5]
003c2df8: mov      r0, r6
003c2dfc: str      r3, [sp, #0x1c]
003c2e00: bl       #0x337888
003c2e04: ldr      r1, [pc, #0x48]
003c2e08: mov      r2, sp
003c2e0c: mov      r0, r4
003c2e10: add      r1, pc, r1
003c2e14: bl       #0x3140ec
003c2e18: mov      r1, r4
003c2e1c: mov      r0, r6
003c2e20: bl       #0x337a88
003c2e24: mov      r0, r4
003c2e28: bl       #0x318254
003c2e2c: ldr      r2, [sp, #0x1c]
003c2e30: ldr      r3, [r5]
003c2e34: cmp      r2, r3
003c2e38: bne      #0x3c2e44
003c2e3c: add      sp, sp, #0x20
003c2e40: pop      {r4, r5, r6, pc}
003c2e44: bl       #0x30e310
003c2e48: ldrheq   r1, [sp], #-0xc4
003c2e4c: andeq    r4, r0, ip, lsr #1
003c2e50: andeq    r0, r0, r4, lsl #17
003c2e54: subseq   r2, r0, r0, asr #32

# _ZN6CSAnim8OnUpdateEiP9CharacterP16CharStateMachine
003c1a3c: mov      r3, #0x1480
003c1a40: ldrb     r1, [r2, r3]
003c1a44: cmp      r1, #0
003c1a48: bxne     lr
003c1a4c: ldrb     r3, [r2, #0x541]
003c1a50: cmp      r3, #0
003c1a54: bxeq     lr
003c1a58: add      r0, r2, #0x4f0
003c1a5c: add      r0, r0, #0xc
003c1a60: b        #0x3c1a00

# _ZN6CSAnim7OnEventEiP9CharacterP16CharStateMachineiPv
003c1a14: ldr      r3, [sp]
003c1a18: cmp      r3, #0x22
003c1a1c: bxne     lr
003c1a20: ldrb     r3, [r2, #0x540]
003c1a24: cmp      r3, #0
003c1a28: bxeq     lr
003c1a2c: add      r0, r2, #0x4f0
003c1a30: add      r0, r0, #0xc
003c1a34: mov      r1, #0
003c1a38: b        #0x3c1a00

# _ZN10CSReviving7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c5294: push     {r4, r5, r6, r7, r8, lr}
003c5298: ldr      r5, [pc, #0x12c]
003c529c: ldr      r7, [pc, #0x12c]
003c52a0: ldr      r1, [pc, #0x12c]
003c52a4: add      r5, pc, r5
003c52a8: ldr      r3, [r5, r7]
003c52ac: ldr      r8, [r5, r1]
003c52b0: sub      sp, sp, #0x30
003c52b4: ldr      r3, [r3]
003c52b8: mov      r0, r8
003c52bc: mov      r4, r2
003c52c0: str      r3, [sp, #0x2c]
003c52c4: bl       #0x337888
003c52c8: ldr      r1, [pc, #0x108]
003c52cc: add      r6, sp, #0x14
003c52d0: add      r2, sp, #0x10
003c52d4: mov      r0, r6
003c52d8: add      r1, pc, r1
003c52dc: bl       #0x3140ec
003c52e0: mov      r1, r6
003c52e4: mov      r0, r8
003c52e8: bl       #0x337a88
003c52ec: mov      r0, r6
003c52f0: bl       #0x318254
003c52f4: movw     r3, #0x2341
003c52f8: str      r3, [r4, #0x520]
003c52fc: mov      r0, r4
003c5300: bl       #0x3bc6b8
003c5304: ldr      r3, [r4, #0x378]
003c5308: mov      r2, #1
003c530c: add      r6, sp, #4
003c5310: strb     r2, [r3, #8]
003c5314: ldr      r1, [r4, #0x408]
003c5318: mov      r0, r6
003c531c: bl       #0x33dd2c
003c5320: mov      r0, r6
003c5324: bl       #0x33ff54
003c5328: add      r6, r4, #0x3c8
003c532c: mov      r1, #0
003c5330: mov      r2, r1
003c5334: str      r0, [r4, #0x558]
003c5338: mov      r0, r6
003c533c: bl       #0x3d6890
003c5340: mov      r0, r6
003c5344: bl       #0x3d49c4
003c5348: ldr      r3, [pc, #0x8c]
003c534c: mov      r0, r4
003c5350: add      r6, r4, #0x4f0
003c5354: ldr      r3, [r5, r3]
003c5358: add      r6, r6, #0xc
003c535c: ldr      r8, [r3]
003c5360: bl       #0x3a3228
003c5364: ldr      r3, [pc, #0x74]
003c5368: ldr      r1, [pc, #0x74]
003c536c: ldr      r2, [r5, r3]
003c5370: mov      r3, #0xa0
003c5374: mla      r3, r3, r0, r8
003c5378: ldr      r0, [r2, #0x2c]
003c537c: ldr      r2, [pc, #0x64]
003c5380: add      r1, pc, r1
003c5384: ldr      r8, [r3, #0x6c]
003c5388: add      r2, pc, r2
003c538c: bl       #0x4c4bdc
003c5390: ands     r0, r0, #0x80000
003c5394: beq      #0x3c53a0
003c5398: mov      r0, r4
003c539c: bl       #0x3a53e0
003c53a0: add      r1, r0, r8
003c53a4: mov      r0, r6
003c53a8: bl       #0x3c0b50
003c53ac: ldr      r3, [r5, r7]
003c53b0: ldr      r2, [sp, #0x2c]
003c53b4: ldr      r3, [r3]
003c53b8: cmp      r2, r3
003c53bc: bne      #0x3c53c8
003c53c0: add      sp, sp, #0x30
003c53c4: pop      {r4, r5, r6, r7, r8, pc}
003c53c8: bl       #0x30e310
003c53cc: subseq   pc, ip, ip, ror #15
003c53d0: andeq    r4, r0, ip, lsr #1
003c53d4: andeq    r0, r0, r4, lsl #17
003c53d8: subeq    pc, pc, r8, ror fp
003c53dc: andeq    r4, r0, r4, asr #16
003c53e0: strdeq   r3, r4, [r0], -r4
003c53e4: subeq    pc, pc, r8, lsr r8
003c53e8: subeq    pc, pc, r0, asr #16

# _ZN10CSReviving6OnBlurEiP9CharacterP16CharStateMachinei
003c6724: ldr      r3, [pc, #0x98]
003c6728: ldr      r1, [pc, #0x98]
003c672c: push     {r4, r5, r6, r7, lr}
003c6730: add      r3, pc, r3
003c6734: ldr      r5, [r3, r1]
003c6738: ldr      r1, [pc, #0x8c]
003c673c: mov      r6, r2
003c6740: ldr      r2, [r5]
003c6744: ldr      r7, [r3, r1]
003c6748: sub      sp, sp, #0x24
003c674c: str      r2, [sp, #0x1c]
003c6750: mov      r0, r7
003c6754: bl       #0x337888
003c6758: ldr      r1, [pc, #0x70]
003c675c: add      r4, sp, #4
003c6760: mov      r2, sp
003c6764: add      r1, pc, r1
003c6768: mov      r0, r4
003c676c: bl       #0x3140ec
003c6770: mov      r1, r4
003c6774: mov      r0, r7
003c6778: bl       #0x337a88
003c677c: mov      r0, r4
003c6780: bl       #0x318254
003c6784: ldr      r0, [r6, #0x558]
003c6788: mov      r1, #0
003c678c: mov      r2, r1
003c6790: add      r0, r0, #0x4f0
003c6794: add      r0, r0, #0xc
003c6798: bl       #0x3c5880
003c679c: ldr      r3, [r6, #0x378]
003c67a0: mov      r2, #0
003c67a4: strb     r2, [r3, #8]
003c67a8: ldr      r2, [sp, #0x1c]
003c67ac: ldr      r3, [r5]
003c67b0: cmp      r2, r3
003c67b4: bne      #0x3c67c0
003c67b8: add      sp, sp, #0x24
003c67bc: pop      {r4, r5, r6, r7, pc}
003c67c0: bl       #0x30e310
003c67c4: subseq   lr, ip, r0, ror #6
003c67c8: andeq    r4, r0, ip, lsr #1
003c67cc: andeq    r0, r0, r4, lsl #17
003c67d0: subeq    lr, pc, ip, ror #13

# _ZN10CSReviving8OnUpdateEiP9CharacterP16CharStateMachine
003c0058: bx       lr

# _ZN10CSReviving7OnEventEiP9CharacterP16CharStateMachineiPv
003c005c: bx       lr

# _ZN9CSRevived7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c34cc: push     {r4, r5, r6, r7, r8, lr}
003c34d0: ldr      r4, [pc, #0xf4]
003c34d4: ldr      r7, [pc, #0xf4]
003c34d8: ldr      r1, [pc, #0xf4]
003c34dc: add      r4, pc, r4
003c34e0: ldr      r3, [r4, r7]
003c34e4: ldr      r8, [r4, r1]
003c34e8: sub      sp, sp, #0x20
003c34ec: ldr      r3, [r3]
003c34f0: mov      r0, r8
003c34f4: mov      r5, r2
003c34f8: str      r3, [sp, #0x1c]
003c34fc: bl       #0x337888
003c3500: ldr      r1, [pc, #0xd0]
003c3504: add      r6, sp, #4
003c3508: mov      r2, sp
003c350c: mov      r0, r6
003c3510: add      r1, pc, r1
003c3514: bl       #0x3140ec
003c3518: mov      r1, r6
003c351c: mov      r0, r8
003c3520: bl       #0x337a88
003c3524: mov      r0, r6
003c3528: bl       #0x318254
003c352c: movw     r3, #0x2341
003c3530: str      r3, [r5, #0x520]
003c3534: mov      r0, r5
003c3538: bl       #0x3bc6b8
003c353c: ldr      r3, [pc, #0x98]
003c3540: mov      r0, r5
003c3544: add      r6, r5, #0x4f0
003c3548: ldr      r3, [r4, r3]
003c354c: add      r6, r6, #0xc
003c3550: ldr      r8, [r3]
003c3554: bl       #0x3a3228
003c3558: ldr      r3, [pc, #0x80]
003c355c: ldr      r1, [pc, #0x80]
003c3560: ldr      r2, [r4, r3]
003c3564: mov      r3, #0xa0
003c3568: mla      r3, r3, r0, r8
003c356c: ldr      r0, [r2, #0x2c]
003c3570: ldr      r2, [pc, #0x70]
003c3574: add      r1, pc, r1
003c3578: ldr      r8, [r3, #0x68]
003c357c: add      r2, pc, r2
003c3580: bl       #0x4c4bdc
003c3584: ands     r0, r0, #0x100000
003c3588: beq      #0x3c3594
003c358c: mov      r0, r5
003c3590: bl       #0x3a53e0
003c3594: add      r1, r0, r8
003c3598: mov      r0, r6
003c359c: bl       #0x3c0b50
003c35a0: ldr      r2, [r5, #0x378]
003c35a4: ldr      r3, [r4, r7]
003c35a8: mov      r1, #1
003c35ac: strb     r1, [r2, #8]
003c35b0: ldr      r2, [sp, #0x1c]
003c35b4: ldr      r3, [r3]
003c35b8: cmp      r2, r3
003c35bc: bne      #0x3c35c8
003c35c0: add      sp, sp, #0x20
003c35c4: pop      {r4, r5, r6, r7, r8, pc}
003c35c8: bl       #0x30e310
003c35cc: ldrheq   r1, [sp], #-0x54
003c35d0: andeq    r4, r0, ip, lsr #1
003c35d4: andeq    r0, r0, r4, lsl #17
003c35d8: subseq   r1, r0, r0, asr #18
003c35dc: andeq    r4, r0, r4, asr #16
003c35e0: strdeq   r3, r4, [r0], -r4
003c35e4: subseq   r1, r0, r4, asr #12
003c35e8: subseq   r1, r0, ip, asr #12

# _ZN9CSRevived6OnBlurEiP9CharacterP16CharStateMachinei
003c53ec: ldr      r3, [pc, #0x98]
003c53f0: ldr      r1, [pc, #0x98]
003c53f4: push     {r4, r5, r6, r7, lr}
003c53f8: add      r3, pc, r3
003c53fc: ldr      r6, [r3, r1]
003c5400: ldr      r1, [pc, #0x8c]
003c5404: mov      r4, r2
003c5408: ldr      r2, [r6]
003c540c: ldr      r7, [r3, r1]
003c5410: sub      sp, sp, #0x24
003c5414: str      r2, [sp, #0x1c]
003c5418: mov      r0, r7
003c541c: bl       #0x337888
003c5420: ldr      r1, [pc, #0x70]
003c5424: add      r5, sp, #4
003c5428: mov      r2, sp
003c542c: add      r1, pc, r1
003c5430: mov      r0, r5
003c5434: bl       #0x3140ec
003c5438: mov      r1, r5
003c543c: mov      r0, r7
003c5440: bl       #0x337a88
003c5444: mov      r0, r5
003c5448: bl       #0x318254
003c544c: mov      r0, r4
003c5450: mov      r1, #0
003c5454: mov      r2, #1
003c5458: bl       #0x3a59ac
003c545c: ldr      r3, [r4, #0x378]
003c5460: mov      r2, #0
003c5464: mov      r0, r4
003c5468: strb     r2, [r3, #8]
003c546c: bl       #0x3a41a0
003c5470: ldr      r2, [sp, #0x1c]
003c5474: ldr      r3, [r6]
003c5478: cmp      r2, r3
003c547c: bne      #0x3c5488
003c5480: add      sp, sp, #0x24
003c5484: pop      {r4, r5, r6, r7, pc}
003c5488: bl       #0x30e310

# _ZN9CSRevived8OnUpdateEiP9CharacterP16CharStateMachine
003c0064: bx       lr

# _ZN9CSRevived7OnEventEiP9CharacterP16CharStateMachineiPv
003c0068: bx       lr

# _ZN10CSPreSpawn7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c688c: push     {r4, r5, r6, r7, r8, sl, lr}
003c6890: ldr      r5, [pc, #0x168]
003c6894: ldr      r7, [pc, #0x168]
003c6898: ldr      r1, [pc, #0x168]
003c689c: add      r5, pc, r5
003c68a0: ldr      r3, [r5, r7]
003c68a4: ldr      r8, [r5, r1]
003c68a8: sub      sp, sp, #0x24
003c68ac: ldr      r3, [r3]
003c68b0: mov      r0, r8
003c68b4: mov      r4, r2
003c68b8: str      r3, [sp, #0x1c]
003c68bc: bl       #0x337888
003c68c0: ldr      r1, [pc, #0x144]
003c68c4: add      r6, sp, #4
003c68c8: mov      r2, sp
003c68cc: mov      r0, r6
003c68d0: add      r1, pc, r1
003c68d4: bl       #0x3140ec
003c68d8: mov      r1, r6
003c68dc: mov      r0, r8
003c68e0: bl       #0x337a88
003c68e4: mov      r0, r6
003c68e8: bl       #0x318254
003c68ec: mov      r3, #0x1300
003c68f0: str      r3, [r4, #0x520]
003c68f4: ldr      r3, [pc, #0x114]
003c68f8: mov      r0, r4
003c68fc: mov      r6, #0xa0
003c6900: ldr      r8, [r5, r3]
003c6904: ldr      sl, [r8]
003c6908: bl       #0x3a3228
003c690c: mla      r0, r6, r0, sl
003c6910: ldr      r3, [r0, #0x64]
003c6914: cmn      r3, #1
003c6918: beq      #0x3c6990
003c691c: mov      r0, r4
003c6920: ldr      r8, [r8]
003c6924: bl       #0x3a3228
003c6928: mla      r6, r6, r0, r8
003c692c: add      r0, r4, #0x490
003c6930: add      r0, r0, #0xc
003c6934: ldr      r1, [r6, #0x64]
003c6938: bl       #0x3cacb0
003c693c: mov      r1, #0
003c6940: mov      r2, r1
003c6944: mov      r0, r4
003c6948: bl       #0x394bf8
003c694c: movw     r3, #0x13e4
003c6950: ldrb     r1, [r4, r3]
003c6954: cmp      r1, #0
003c6958: bne      #0x3c696c
003c695c: ldr      r3, [r4]
003c6960: mov      r0, r4
003c6964: mov      lr, pc
003c6968: ldr      pc, [r3, #0x40]
003c696c: mov      r0, r4
003c6970: bl       #0x3949b0
003c6974: ldr      r3, [r5, r7]
003c6978: ldr      r2, [sp, #0x1c]
003c697c: ldr      r3, [r3]
003c6980: cmp      r2, r3
003c6984: bne      #0x3c69fc
003c6988: add      sp, sp, #0x24
003c698c: pop      {r4, r5, r6, r7, r8, sl, pc}
003c6990: mov      r0, r4
003c6994: ldr      r8, [r8]
003c6998: bl       #0x3a3228
003c699c: ldr      r3, [pc, #0x70]
003c69a0: ldr      r1, [pc, #0x70]
003c69a4: ldr      r2, [r5, r3]
003c69a8: mla      r3, r6, r0, r8
003c69ac: ldr      r0, [r2, #0x2c]
003c69b0: ldr      r2, [pc, #0x64]
003c69b4: add      r1, pc, r1
003c69b8: ldr      r6, [r3, #0x80]
003c69bc: add      r2, pc, r2
003c69c0: bl       #0x4c4bdc
003c69c4: add      r8, r4, #0x490
003c69c8: ands     r0, r0, #1
003c69cc: add      r8, r8, #0xc
003c69d0: bne      #0x3c69f0
003c69d4: add      r1, r0, r6
003c69d8: mov      r0, r8
003c69dc: bl       #0x3cacb0
003c69e0: mov      r0, r8
003c69e4: mov      r1, #0
003c69e8: bl       #0x3c93fc
003c69ec: b        #0x3c693c
003c69f0: mov      r0, r4
003c69f4: bl       #0x3a53e0
003c69f8: b        #0x3c69d4
003c69fc: bl       #0x30e310
003c6a00: ldrsheq  lr, [ip], #-0x14
003c6a04: andeq    r4, r0, ip, lsr #1
003c6a08: andeq    r0, r0, r4, lsl #17
003c6a0c: subeq    lr, pc, r0, lsl #11
003c6a10: andeq    r4, r0, r4, asr #16
003c6a14: strdeq   r3, r4, [r0], -r4
003c6a18: subeq    lr, pc, r4, lsl #4
003c6a1c: subeq    lr, pc, ip, lsl #4

# _ZN10CSPreSpawn6OnBlurEiP9CharacterP16CharStateMachinei
003c67d4: ldr      r3, [pc, #0xa0]
003c67d8: ldr      r1, [pc, #0xa0]
003c67dc: push     {r4, r5, r6, r7, lr}
003c67e0: add      r3, pc, r3
003c67e4: ldr      r6, [r3, r1]
003c67e8: ldr      r1, [pc, #0x94]
003c67ec: mov      r4, r2
003c67f0: ldr      r2, [r6]
003c67f4: ldr      r7, [r3, r1]
003c67f8: sub      sp, sp, #0x24
003c67fc: str      r2, [sp, #0x1c]
003c6800: mov      r0, r7
003c6804: bl       #0x337888
003c6808: ldr      r1, [pc, #0x78]
003c680c: add      r5, sp, #4
003c6810: mov      r2, sp
003c6814: add      r1, pc, r1
003c6818: mov      r0, r5
003c681c: bl       #0x3140ec
003c6820: mov      r1, r5
003c6824: mov      r0, r7
003c6828: bl       #0x337a88
003c682c: mov      r0, r5
003c6830: bl       #0x318254
003c6834: ldr      r3, [r4]
003c6838: mov      r1, #1
003c683c: mov      r0, r4
003c6840: mov      lr, pc
003c6844: ldr      pc, [r3, #0x40]
003c6848: mov      r1, #0
003c684c: mov      r2, r1
003c6850: mov      r0, r4
003c6854: bl       #0x3a59ac
003c6858: mov      r0, r4
003c685c: bl       #0x394a3c
003c6860: ldr      r2, [sp, #0x1c]
003c6864: ldr      r3, [r6]
003c6868: cmp      r2, r3
003c686c: bne      #0x3c6878
003c6870: add      sp, sp, #0x24
003c6874: pop      {r4, r5, r6, r7, pc}
003c6878: bl       #0x30e310
003c687c: ldrheq   lr, [ip], #-0x20
003c6880: andeq    r4, r0, ip, lsr #1
003c6884: andeq    r0, r0, r4, lsl #17
003c6888: subeq    lr, pc, ip, lsr r6

# _ZN10CSPreSpawn8OnUpdateEiP9CharacterP16CharStateMachine
003c0070: bx       lr

# _ZN10CSPreSpawn7OnEventEiP9CharacterP16CharStateMachineiPv
003c2810: push     {r4, r5, r6, lr}
003c2814: sub      sp, sp, #0x10
003c2818: ldr      r6, [sp, #0x20]
003c281c: ldr      r4, [pc, #0x108]
003c2820: mov      r3, r1
003c2824: cmp      r6, #9
003c2828: add      r4, pc, r4
003c282c: mov      r5, r2
003c2830: ldr      r0, [sp, #0x24]
003c2834: beq      #0x3c2874
003c2838: cmp      r6, #0x28
003c283c: beq      #0x3c2848
003c2840: add      sp, sp, #0x10
003c2844: pop      {r4, r5, r6, pc}
003c2848: ldr      r1, [pc, #0xe0]
003c284c: add      r1, pc, r1
003c2850: bl       #0x30e31c
003c2854: cmp      r0, #0
003c2858: bne      #0x3c2840
003c285c: ldr      r3, [r5, #0x520]
003c2860: mov      r0, r5
003c2864: orr      r3, r3, #0x2000
003c2868: str      r3, [r5, #0x520]
003c286c: bl       #0x3b4088
003c2870: b        #0x3c2840
003c2874: add      ip, sp, #0x10
003c2878: mov      r2, #1
003c287c: str      r2, [ip, #-4]!
003c2880: mov      r1, r6
003c2884: mov      r2, r0
003c2888: mov      r0, r5
003c288c: str      ip, [sp]
003c2890: bl       #0x3ad2e4
003c2894: cmp      r0, #0
003c2898: beq      #0x3c2840
003c289c: ldr      r1, [sp, #0xc]
003c28a0: cmp      r1, #1
003c28a4: beq      #0x3c2900
003c28a8: ldr      r3, [pc, #0x84]
003c28ac: ldr      r3, [r4, r3]
003c28b0: ldr      r3, [r3]
003c28b4: cmp      r3, #2
003c28b8: moveq    r3, #0
003c28bc: streq    r3, [r3]
003c28c0: beq      #0x3c2840
003c28c4: cmp      r3, #1
003c28c8: bne      #0x3c2840
003c28cc: ldr      r0, [pc, #0x64]
003c28d0: ldr      r1, [pc, #0x64]
003c28d4: ldr      r2, [pc, #0x64]
003c28d8: ldr      r0, [r4, r0]
003c28dc: ldr      r3, [pc, #0x60]
003c28e0: mov      ip, #0x72
003c28e4: add      r1, pc, r1
003c28e8: add      r2, pc, r2
003c28ec: add      r3, pc, r3
003c28f0: add      r0, r0, #0xa8
003c28f4: str      ip, [sp]
003c28f8: bl       #0x30e004
003c28fc: b        #0x3c2840
003c2900: add      r0, r5, #0x4f0
003c2904: add      r0, r0, #0xc
003c2908: mov      r2, #0
003c290c: bl       #0x3c2734
003c2910: ldr      r3, [r5, #0x400]
003c2914: cmp      r3, #3
003c2918: bne      #0x3c2840
003c291c: ldr      r3, [r5, #0x3fc]
003c2920: mov      r2, #0
003c2924: str      r2, [r3, #0x24]
003c2928: b        #0x3c2840
003c292c: subseq   r2, sp, r8, ror #4
003c2930: subseq   r2, r0, ip, lsl r3
003c2934: andeq    r3, r0, r0, asr #19
003c2938: andeq    r1, r0, r0, asr #19
003c293c: strdeq   fp, ip, [pc], #-0xa4
003c2940: subseq   r2, r0, r8, lsr #9
003c2944: ldrsheq  r2, [r0], #-0x4c

# _ZN13CSLiftingIdle7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3140: push     {r4, r5, r6, r7, r8, lr}
003c3144: ldr      r4, [pc, #0xe0]
003c3148: ldr      r7, [pc, #0xe0]
003c314c: ldr      r1, [pc, #0xe0]
003c3150: add      r4, pc, r4
003c3154: ldr      r3, [r4, r7]
003c3158: ldr      r8, [r4, r1]
003c315c: sub      sp, sp, #0x20
003c3160: ldr      r3, [r3]
003c3164: mov      r0, r8
003c3168: mov      r5, r2
003c316c: str      r3, [sp, #0x1c]
003c3170: bl       #0x337888
003c3174: ldr      r1, [pc, #0xbc]
003c3178: add      r6, sp, #4
003c317c: mov      r2, sp
003c3180: mov      r0, r6
003c3184: add      r1, pc, r1
003c3188: bl       #0x3140ec
003c318c: mov      r1, r6
003c3190: mov      r0, r8
003c3194: bl       #0x337a88
003c3198: mov      r0, r6
003c319c: bl       #0x318254
003c31a0: mov      r3, #0x2380
003c31a4: str      r3, [r5, #0x520]
003c31a8: ldr      r3, [pc, #0x8c]
003c31ac: mov      r0, r5
003c31b0: add      r6, r5, #0x490
003c31b4: ldr      r3, [r4, r3]
003c31b8: add      r6, r6, #0xc
003c31bc: ldr      r8, [r3]
003c31c0: bl       #0x3a3228
003c31c4: ldr      r3, [pc, #0x74]
003c31c8: ldr      r1, [pc, #0x74]
003c31cc: ldr      r2, [r4, r3]
003c31d0: mov      r3, #0xa0
003c31d4: mla      r3, r3, r0, r8
003c31d8: ldr      r0, [r2, #0x2c]
003c31dc: ldr      r2, [pc, #0x64]
003c31e0: add      r1, pc, r1
003c31e4: ldr      r8, [r3, #0x50]
003c31e8: add      r2, pc, r2
003c31ec: bl       #0x4c4bdc
003c31f0: ands     r0, r0, #0x1000000
003c31f4: beq      #0x3c3200
003c31f8: mov      r0, r5
003c31fc: bl       #0x3a53e0
003c3200: add      r1, r0, r8
003c3204: mov      r0, r6
003c3208: bl       #0x3cacb0
003c320c: ldr      r3, [r4, r7]
003c3210: ldr      r2, [sp, #0x1c]
003c3214: ldr      r3, [r3]
003c3218: cmp      r2, r3
003c321c: bne      #0x3c3228
003c3220: add      sp, sp, #0x20
003c3224: pop      {r4, r5, r6, r7, r8, pc}
003c3228: bl       #0x30e310
003c322c: subseq   r1, sp, r0, asr #18
003c3230: andeq    r4, r0, ip, lsr #1
003c3234: andeq    r0, r0, r4, lsl #17
003c3238: subseq   r1, r0, ip, asr #25
003c323c: andeq    r4, r0, r4, asr #16
003c3240: strdeq   r3, r4, [r0], -r4
003c3244: ldrsbeq  r1, [r0], #-0x98
003c3248: subseq   r1, r0, r0, ror #19

# _ZN13CSLiftingIdle6OnBlurEiP9CharacterP16CharStateMachinei
003c50b4: push     {r4, r5, r6, r7, r8, sl, lr}
003c50b8: ldr      r4, [pc, #0xc8]
003c50bc: ldr      r6, [pc, #0xc8]
003c50c0: ldr      r1, [pc, #0xc8]
003c50c4: add      r4, pc, r4
003c50c8: ldr      r3, [r4, r6]
003c50cc: ldr      r7, [r4, r1]
003c50d0: sub      sp, sp, #0x24
003c50d4: ldr      r3, [r3]
003c50d8: mov      r0, r7
003c50dc: mov      sl, r2
003c50e0: str      r3, [sp, #0x1c]
003c50e4: ldr      r8, [sp, #0x40]
003c50e8: bl       #0x337888
003c50ec: ldr      r1, [pc, #0xa0]
003c50f0: add      r5, sp, #4
003c50f4: mov      r2, sp
003c50f8: add      r1, pc, r1
003c50fc: mov      r0, r5
003c5100: bl       #0x3140ec
003c5104: mov      r1, r5
003c5108: mov      r0, r7
003c510c: bl       #0x337a88
003c5110: mov      r0, r5
003c5114: bl       #0x318254
003c5118: cmp      r8, #0x13
003c511c: bhi      #0x3c514c
003c5120: mov      r3, #1
003c5124: lsl      r8, r3, r8
003c5128: tst      r8, #0xc2000
003c512c: beq      #0x3c514c
003c5130: ldr      r3, [r4, r6]
003c5134: ldr      r2, [sp, #0x1c]
003c5138: ldr      r3, [r3]
003c513c: cmp      r2, r3
003c5140: bne      #0x3c5184
003c5144: add      sp, sp, #0x24
003c5148: pop      {r4, r5, r6, r7, r8, sl, pc}
003c514c: ldr      r0, [sl, #0x54c]
003c5150: cmp      r0, #0
003c5154: beq      #0x3c5130
003c5158: ldr      r3, [r0, #0xf4]
003c515c: cmp      r3, #6
003c5160: beq      #0x3c5170
003c5164: mov      r3, #0
003c5168: str      r3, [sl, #0x54c]
003c516c: b        #0x3c5130
003c5170: ldr      r3, [r0, #0x390]
003c5174: cmp      r3, sl
003c5178: bne      #0x3c5164
003c517c: bl       #0x3ee5a8
003c5180: b        #0x3c5164
003c5184: bl       #0x30e310
003c5188: subseq   pc, ip, ip, asr #19
003c518c: andeq    r4, r0, ip, lsr #1
003c5190: andeq    r0, r0, r4, lsl #17
003c5194: subeq    pc, pc, r8, asr sp

# _ZN13CSLiftingIdle8OnUpdateEiP9CharacterP16CharStateMachine
003c0e90: mov      r0, r1
003c0e94: mov      r1, r2
003c0e98: mov      r2, r3
003c0e9c: b        #0x3c0b78

# _ZN13CSLiftingIdle7OnEventEiP9CharacterP16CharStateMachineiPv
003c6608: ldr      r3, [sp]
003c660c: cmp      r3, #6
003c6610: bxne     lr
003c6614: ldr      r3, [r2, #0x54c]
003c6618: cmp      r3, #0
003c661c: bxeq     lr
003c6620: add      r0, r2, #0x4f0
003c6624: mov      ip, #1
003c6628: add      r0, r0, #0xc
003c662c: mov      r1, #5
003c6630: mov      r2, #0
003c6634: str      ip, [sp]
003c6638: b        #0x3c64ac

# _ZN13CSLiftingMove7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3e40: push     {r4, r5, r6, r7, r8, lr}
003c3e44: ldr      r4, [pc, #0x108]
003c3e48: ldr      r7, [pc, #0x108]
003c3e4c: ldr      r1, [pc, #0x108]
003c3e50: add      r4, pc, r4
003c3e54: ldr      r3, [r4, r7]
003c3e58: ldr      r8, [r4, r1]
003c3e5c: sub      sp, sp, #0x20
003c3e60: ldr      r3, [r3]
003c3e64: mov      r0, r8
003c3e68: mov      r5, r2
003c3e6c: str      r3, [sp, #0x1c]
003c3e70: bl       #0x337888
003c3e74: ldr      r1, [pc, #0xe4]
003c3e78: add      r6, sp, #4
003c3e7c: mov      r2, sp
003c3e80: mov      r0, r6
003c3e84: add      r1, pc, r1
003c3e88: bl       #0x3140ec
003c3e8c: mov      r1, r6
003c3e90: mov      r0, r8
003c3e94: bl       #0x337a88
003c3e98: mov      r0, r6
003c3e9c: bl       #0x318254
003c3ea0: movw     r3, #0x23c1
003c3ea4: str      r3, [r5, #0x520]
003c3ea8: ldr      r3, [pc, #0xb4]
003c3eac: mov      r0, r5
003c3eb0: add      r6, r5, #0x490
003c3eb4: ldr      r3, [r4, r3]
003c3eb8: add      r6, r6, #0xc
003c3ebc: ldr      r8, [r3]
003c3ec0: bl       #0x3a3228
003c3ec4: ldr      r3, [pc, #0x9c]
003c3ec8: ldr      r1, [pc, #0x9c]
003c3ecc: ldr      r2, [r4, r3]
003c3ed0: mov      r3, #0xa0
003c3ed4: mla      r3, r3, r0, r8
003c3ed8: ldr      r0, [r2, #0x2c]
003c3edc: ldr      r2, [pc, #0x8c]
003c3ee0: add      r1, pc, r1
003c3ee4: ldr      r8, [r3, #0x54]
003c3ee8: add      r2, pc, r2
003c3eec: bl       #0x4c4bdc
003c3ef0: ands     r0, r0, #0x1000000
003c3ef4: bne      #0x3c3f44
003c3ef8: add      r1, r0, r8
003c3efc: mov      r0, r6
003c3f00: bl       #0x3cacb0
003c3f04: add      r0, r5, #0x560
003c3f08: bl       #0x3de6c4
003c3f0c: mov      r1, r0
003c3f10: mov      r0, r6
003c3f14: bl       #0x3c93fc
003c3f18: ldr      r0, [r5, #0x2dc]
003c3f1c: cmp      r0, #0
003c3f20: beq      #0x3c3f28
003c3f24: bl       #0x46eae0
003c3f28: ldr      r3, [r4, r7]
003c3f2c: ldr      r2, [sp, #0x1c]
003c3f30: ldr      r3, [r3]
003c3f34: cmp      r2, r3
003c3f38: bne      #0x3c3f50
003c3f3c: add      sp, sp, #0x20
003c3f40: pop      {r4, r5, r6, r7, r8, pc}
003c3f44: mov      r0, r5
003c3f48: bl       #0x3a53e0
003c3f4c: b        #0x3c3ef8
003c3f50: bl       #0x30e310
003c3f54: subseq   r0, sp, r0, asr #24
003c3f58: andeq    r4, r0, ip, lsr #1
003c3f5c: andeq    r0, r0, r4, lsl #17
003c3f60: subseq   r0, r0, ip, asr #31
003c3f64: andeq    r4, r0, r4, asr #16
003c3f68: strdeq   r3, r4, [r0], -r4
003c3f6c: ldrsbeq  r0, [r0], #-0xc8
003c3f70: subseq   r0, r0, r0, ror #25

# _ZN13CSLiftingMove6OnBlurEiP9CharacterP16CharStateMachinei
003c5198: push     {r4, r5, r6, r7, r8, sl, lr}
003c519c: ldr      r4, [pc, #0xe0]
003c51a0: ldr      r7, [pc, #0xe0]
003c51a4: ldr      r1, [pc, #0xe0]
003c51a8: add      r4, pc, r4
003c51ac: ldr      r3, [r4, r7]
003c51b0: ldr      r8, [r4, r1]
003c51b4: sub      sp, sp, #0x24
003c51b8: ldr      r3, [r3]
003c51bc: mov      r0, r8
003c51c0: mov      r5, r2
003c51c4: str      r3, [sp, #0x1c]
003c51c8: ldr      sl, [sp, #0x40]
003c51cc: bl       #0x337888
003c51d0: ldr      r1, [pc, #0xb8]
003c51d4: add      r6, sp, #4
003c51d8: mov      r2, sp
003c51dc: add      r1, pc, r1
003c51e0: mov      r0, r6
003c51e4: bl       #0x3140ec
003c51e8: mov      r1, r6
003c51ec: mov      r0, r8
003c51f0: bl       #0x337a88
003c51f4: mov      r0, r6
003c51f8: bl       #0x318254
003c51fc: mov      r0, r5
003c5200: bl       #0x3938f8
003c5204: ldr      r0, [r5, #0x2dc]
003c5208: cmp      r0, #0
003c520c: beq      #0x3c5214
003c5210: bl       #0x46eb20
003c5214: cmp      sl, #0x13
003c5218: bhi      #0x3c5248
003c521c: mov      r3, #1
003c5220: lsl      sl, r3, sl
003c5224: tst      sl, #0xc2000
003c5228: beq      #0x3c5248
003c522c: ldr      r3, [r4, r7]
003c5230: ldr      r2, [sp, #0x1c]
003c5234: ldr      r3, [r3]
003c5238: cmp      r2, r3
003c523c: bne      #0x3c5280
003c5240: add      sp, sp, #0x24
003c5244: pop      {r4, r5, r6, r7, r8, sl, pc}
003c5248: ldr      r0, [r5, #0x54c]
003c524c: cmp      r0, #0
003c5250: beq      #0x3c522c
003c5254: ldr      r3, [r0, #0xf4]
003c5258: cmp      r3, #6
003c525c: beq      #0x3c526c
003c5260: mov      r3, #0
003c5264: str      r3, [r5, #0x54c]
003c5268: b        #0x3c522c
003c526c: ldr      r3, [r0, #0x390]
003c5270: cmp      r3, r5
003c5274: bne      #0x3c5260
003c5278: bl       #0x3ee5a8
003c527c: b        #0x3c5260
003c5280: bl       #0x30e310
003c5284: subseq   pc, ip, r8, ror #17
003c5288: andeq    r4, r0, ip, lsr #1
003c528c: andeq    r0, r0, r4, lsl #17
003c5290: subeq    pc, pc, r4, ror ip

# _ZN13CSLiftingMove8OnUpdateEiP9CharacterP16CharStateMachine
003c0ea0: str      lr, [sp, #-4]!
003c0ea4: ldrb     r3, [r2, #0x1b5]
003c0ea8: sub      sp, sp, #0xc
003c0eac: cmp      r3, #0
003c0eb0: bne      #0x3c0ecc
003c0eb4: mov      r0, r2
003c0eb8: mov      r1, #0x3f
003c0ebc: mov      r2, #0
003c0ec0: add      sp, sp, #0xc
003c0ec4: pop      {lr}
003c0ec8: b        #0x3a4d5c
003c0ecc: ldr      r3, [r2, #0x408]
003c0ed0: cmp      r3, #0
003c0ed4: beq      #0x3c0ee0
003c0ed8: add      sp, sp, #0xc
003c0edc: ldm      sp!, {pc}
003c0ee0: ldr      r3, [r2]
003c0ee4: mov      r0, r2
003c0ee8: str      r2, [sp, #4]
003c0eec: mov      lr, pc
003c0ef0: ldr      pc, [r3, #0x28]
003c0ef4: cmp      r0, #0
003c0ef8: ldr      r2, [sp, #4]
003c0efc: bne      #0x3c0ed8
003c0f00: mov      r0, r2
003c0f04: bl       #0x39361c
003c0f08: cmp      r0, #0
003c0f0c: ldr      r2, [sp, #4]
003c0f10: beq      #0x3c0ed8
003c0f14: b        #0x3c0eb4

# _ZN13CSLiftingMove7OnEventEiP9CharacterP16CharStateMachineiPv
003c663c: ldr      r3, [sp]
003c6640: cmp      r3, #6
003c6644: bxne     lr
003c6648: ldr      r3, [r2, #0x54c]
003c664c: cmp      r3, #0
003c6650: bxeq     lr
003c6654: add      r0, r2, #0x4f0
003c6658: mov      ip, #1
003c665c: add      r0, r0, #0xc
003c6660: mov      r1, #5
003c6664: mov      r2, #0
003c6668: str      ip, [sp]
003c666c: b        #0x3c64ac
