
# _ZN14CharProperties14_LoadGearPowerEib
003e32b4: ldr      r3, [pc, #0xa54]
003e32b8: ldr      ip, [pc, #0xa54]
003e32bc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003e32c0: add      r3, pc, r3
003e32c4: ldr      ip, [r3, ip]
003e32c8: mov      r6, #0x28
003e32cc: mov      sl, r2
003e32d0: ldr      r3, [ip]
003e32d4: mov      r7, r0
003e32d8: mla      r6, r6, r1, r3
003e32dc: ldr      r2, [r6, #0xc]
003e32e0: cmp      r2, #0
003e32e4: beq      #0x3e33f8
003e32e8: add      r8, r0, #0x710
003e32ec: mov      r4, #0
003e32f0: ldr      r5, [r6, #0x10]
003e32f4: add      r5, r5, r4, lsl #4
003e32f8: ldr      r3, [r5, #4]
003e32fc: cmp      r3, #0x30
003e3300: addls    pc, pc, r3, lsl #2
003e3304: b        #0x3e33ec
003e3308: b        #0x3e3c70
003e330c: b        #0x3e3c48
003e3310: b        #0x3e3c20
003e3314: b        #0x3e3bf8
003e3318: b        #0x3e3bd0
003e331c: b        #0x3e3ba8
003e3320: b        #0x3e3b80
003e3324: b        #0x3e3b50
003e3328: b        #0x3e3b20
003e332c: b        #0x3e3af0
003e3330: b        #0x3e3ac0
003e3334: b        #0x3e3a90
003e3338: b        #0x3e3a60
003e333c: b        #0x3e3a30
003e3340: b        #0x3e3a00
003e3344: b        #0x3e39d0
003e3348: b        #0x3e39a0
003e334c: b        #0x3e33cc
003e3350: b        #0x3e3970
003e3354: b        #0x3e3940
003e3358: b        #0x3e3918
003e335c: b        #0x3e38f0
003e3360: b        #0x3e38c8
003e3364: b        #0x3e38a0
003e3368: b        #0x3e3878
003e336c: b        #0x3e3850
003e3370: b        #0x3e3828
003e3374: b        #0x3e3800
003e3378: b        #0x3e37bc
003e337c: b        #0x3e3794
003e3380: b        #0x3e376c
003e3384: b        #0x3e3744
003e3388: b        #0x3e371c
003e338c: b        #0x3e36f4
003e3390: b        #0x3e36cc
003e3394: b        #0x3e36a4
003e3398: b        #0x3e362c
003e339c: b        #0x3e3604
003e33a0: b        #0x3e35dc
003e33a4: b        #0x3e35b4
003e33a8: b        #0x3e358c
003e33ac: b        #0x3e3514
003e33b0: b        #0x3e34ec
003e33b4: b        #0x3e34c4
003e33b8: b        #0x3e349c
003e33bc: b        #0x3e3474
003e33c0: b        #0x3e344c
003e33c4: b        #0x3e3424
003e33c8: b        #0x3e33fc
003e33cc: cmp      sl, #0
003e33d0: movne    r2, #0x74
003e33d4: moveq    r2, #0x72
003e33d8: ldr      r3, [r5, #8]
003e33dc: mov      r0, r7
003e33e0: mov      r1, r8
003e33e4: bl       #0x3df140
003e33e8: ldr      r2, [r6, #0xc]
003e33ec: add      r4, r4, #1
003e33f0: cmp      r2, r4
003e33f4: bhi      #0x3e32f0
003e33f8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003e33fc: mov      r2, #0xc4
003e3400: ldr      r3, [r5, #8]
003e3404: mov      r0, r7
003e3408: mov      r1, r8
003e340c: bl       #0x3df140
003e3410: ldr      r2, [r6, #0xc]
003e3414: add      r4, r4, #1
003e3418: cmp      r2, r4
003e341c: bhi      #0x3e32f0
003e3420: b        #0x3e33f8
003e3424: mov      r2, #0xc3
003e3428: ldr      r3, [r5, #8]
003e342c: mov      r0, r7
003e3430: mov      r1, r8
003e3434: bl       #0x3df140
003e3438: ldr      r2, [r6, #0xc]
003e343c: add      r4, r4, #1
003e3440: cmp      r2, r4
003e3444: bhi      #0x3e32f0
003e3448: b        #0x3e33f8
003e344c: mov      r2, #0xa6
003e3450: ldr      r3, [r5, #8]
003e3454: mov      r0, r7
003e3458: mov      r1, r8
003e345c: bl       #0x3df140
003e3460: ldr      r2, [r6, #0xc]
003e3464: add      r4, r4, #1
003e3468: cmp      r2, r4
003e346c: bhi      #0x3e32f0
003e3470: b        #0x3e33f8
003e3474: mov      r2, #0xa9
003e3478: ldr      r3, [r5, #8]
003e347c: mov      r0, r7
003e3480: mov      r1, r8
003e3484: bl       #0x3df140
003e3488: ldr      r2, [r6, #0xc]
003e348c: add      r4, r4, #1
003e3490: cmp      r2, r4
003e3494: bhi      #0x3e32f0
003e3498: b        #0x3e33f8
003e349c: mov      r2, #0xa8
003e34a0: ldr      r3, [r5, #8]
003e34a4: mov      r0, r7
003e34a8: mov      r1, r8
003e34ac: bl       #0x3df140
003e34b0: ldr      r2, [r6, #0xc]
003e34b4: add      r4, r4, #1
003e34b8: cmp      r2, r4
003e34bc: bhi      #0x3e32f0
003e34c0: b        #0x3e33f8
003e34c4: mov      r2, #0xa7
003e34c8: ldr      r3, [r5, #8]
003e34cc: mov      r0, r7
003e34d0: mov      r1, r8
003e34d4: bl       #0x3df140
003e34d8: ldr      r2, [r6, #0xc]
003e34dc: add      r4, r4, #1
003e34e0: cmp      r2, r4
003e34e4: bhi      #0x3e32f0
003e34e8: b        #0x3e33f8
003e34ec: mov      r2, #0xaa
003e34f0: ldr      r3, [r5, #8]
003e34f4: mov      r0, r7
003e34f8: mov      r1, r8
003e34fc: bl       #0x3df140
003e3500: ldr      r2, [r6, #0xc]
003e3504: add      r4, r4, #1
003e3508: cmp      r2, r4
003e350c: bhi      #0x3e32f0
003e3510: b        #0x3e33f8
003e3514: mov      r2, #0xa6
003e3518: ldr      r3, [r5, #8]
003e351c: mov      r0, r7
003e3520: mov      r1, r8
003e3524: bl       #0x3df140
003e3528: mov      r0, r7
003e352c: mov      r1, r8
003e3530: mov      r2, #0xa9
003e3534: ldr      r3, [r5, #8]
003e3538: bl       #0x3df140
003e353c: mov      r0, r7
003e3540: mov      r1, r8
003e3544: mov      r2, #0xa7
003e3548: ldr      r3, [r5, #8]
003e354c: bl       #0x3df140
003e3550: mov      r0, r7
003e3554: mov      r1, r8
003e3558: mov      r2, #0xaa
003e355c: ldr      r3, [r5, #8]
003e3560: bl       #0x3df140
003e3564: mov      r2, #0xa8
003e3568: mov      r0, r7
003e356c: mov      r1, r8
003e3570: ldr      r3, [r5, #8]
003e3574: bl       #0x3df140
003e3578: ldr      r2, [r6, #0xc]
003e357c: add      r4, r4, #1
003e3580: cmp      r2, r4
003e3584: bhi      #0x3e32f0
003e3588: b        #0x3e33f8
003e358c: mov      r2, #0x9e
003e3590: ldr      r3, [r5, #8]
003e3594: mov      r0, r7
003e3598: mov      r1, r8
003e359c: bl       #0x3df140
003e35a0: ldr      r2, [r6, #0xc]
003e35a4: add      r4, r4, #1
003e35a8: cmp      r2, r4
003e35ac: bhi      #0x3e32f0
003e35b0: b        #0x3e33f8
003e35b4: mov      r2, #0xa5
003e35b8: ldr      r3, [r5, #8]
003e35bc: mov      r0, r7
003e35c0: mov      r1, r8
003e35c4: bl       #0x3df140
003e35c8: ldr      r2, [r6, #0xc]
003e35cc: add      r4, r4, #1
003e35d0: cmp      r2, r4
003e35d4: bhi      #0x3e32f0
003e35d8: b        #0x3e33f8
003e35dc: mov      r2, #0x85
003e35e0: ldr      r3, [r5, #8]
003e35e4: mov      r0, r7
003e35e8: mov      r1, r8
003e35ec: bl       #0x3df140
003e35f0: ldr      r2, [r6, #0xc]
003e35f4: add      r4, r4, #1
003e35f8: cmp      r2, r4
003e35fc: bhi      #0x3e32f0
003e3600: b        #0x3e33f8
003e3604: mov      r2, #0x84
003e3608: ldr      r3, [r5, #8]
003e360c: mov      r0, r7
003e3610: mov      r1, r8
003e3614: bl       #0x3df140
003e3618: ldr      r2, [r6, #0xc]
003e361c: add      r4, r4, #1
003e3620: cmp      r2, r4
003e3624: bhi      #0x3e32f0
003e3628: b        #0x3e33f8
003e362c: mov      r2, #0x4a
003e3630: ldr      r3, [r5, #8]
003e3634: mov      r0, r7
003e3638: mov      r1, r8
003e363c: bl       #0x3df140
003e3640: mov      r0, r7
003e3644: mov      r1, r8
003e3648: mov      r2, #0x4d
003e364c: ldr      r3, [r5, #8]
003e3650: bl       #0x3df140
003e3654: mov      r0, r7
003e3658: mov      r1, r8
003e365c: mov      r2, #0x4b
003e3660: ldr      r3, [r5, #8]
003e3664: bl       #0x3df140
003e3668: mov      r0, r7
003e366c: mov      r1, r8
003e3670: mov      r2, #0x4e
003e3674: ldr      r3, [r5, #8]
003e3678: bl       #0x3df140
003e367c: mov      r2, #0x4c
003e3680: mov      r0, r7
003e3684: mov      r1, r8
003e3688: ldr      r3, [r5, #8]
003e368c: bl       #0x3df140
003e3690: ldr      r2, [r6, #0xc]
003e3694: add      r4, r4, #1
003e3698: cmp      r2, r4
003e369c: bhi      #0x3e32f0
003e36a0: b        #0x3e33f8
003e36a4: mov      r2, #0x4c
003e36a8: ldr      r3, [r5, #8]
003e36ac: mov      r0, r7
003e36b0: mov      r1, r8
003e36b4: bl       #0x3df140
003e36b8: ldr      r2, [r6, #0xc]
003e36bc: add      r4, r4, #1
003e36c0: cmp      r2, r4
003e36c4: bhi      #0x3e32f0
003e36c8: b        #0x3e33f8
003e36cc: mov      r2, #0x4e
003e36d0: ldr      r3, [r5, #8]
003e36d4: mov      r0, r7
003e36d8: mov      r1, r8
003e36dc: bl       #0x3df140
003e36e0: ldr      r2, [r6, #0xc]
003e36e4: add      r4, r4, #1
003e36e8: cmp      r2, r4
003e36ec: bhi      #0x3e32f0
003e36f0: b        #0x3e33f8
003e36f4: mov      r2, #0x4b
003e36f8: ldr      r3, [r5, #8]
003e36fc: mov      r0, r7
003e3700: mov      r1, r8
003e3704: bl       #0x3df140
003e3708: ldr      r2, [r6, #0xc]
003e370c: add      r4, r4, #1
003e3710: cmp      r2, r4
003e3714: bhi      #0x3e32f0
003e3718: b        #0x3e33f8
003e371c: mov      r2, #0x4d
003e3720: ldr      r3, [r5, #8]
003e3724: mov      r0, r7
003e3728: mov      r1, r8
003e372c: bl       #0x3df140
003e3730: ldr      r2, [r6, #0xc]
003e3734: add      r4, r4, #1
003e3738: cmp      r2, r4
003e373c: bhi      #0x3e32f0
003e3740: b        #0x3e33f8
003e3744: mov      r2, #0x4a
003e3748: ldr      r3, [r5, #8]
003e374c: mov      r0, r7
003e3750: mov      r1, r8
003e3754: bl       #0x3df140
003e3758: ldr      r2, [r6, #0xc]
003e375c: add      r4, r4, #1
003e3760: cmp      r2, r4
003e3764: bhi      #0x3e32f0
003e3768: b        #0x3e33f8
003e376c: mov      r2, #0x3b
003e3770: ldr      r3, [r5, #8]
003e3774: mov      r0, r7
003e3778: mov      r1, r8
003e377c: bl       #0x3df140
003e3780: ldr      r2, [r6, #0xc]
003e3784: add      r4, r4, #1
003e3788: cmp      r2, r4
003e378c: bhi      #0x3e32f0
003e3790: b        #0x3e33f8
003e3794: mov      r2, #0x32
003e3798: ldr      r3, [r5, #8]
003e379c: mov      r0, r7
003e37a0: mov      r1, r8
003e37a4: bl       #0x3df140
003e37a8: ldr      r2, [r6, #0xc]
003e37ac: add      r4, r4, #1
003e37b0: cmp      r2, r4
003e37b4: bhi      #0x3e32f0
003e37b8: b        #0x3e33f8
003e37bc: cmp      sl, #0
003e37c0: beq      #0x3e3cd4
003e37c4: mov      r2, #0x51
003e37c8: ldr      r3, [r5, #8]
003e37cc: mov      r0, r7
003e37d0: mov      r1, r8
003e37d4: bl       #0x3df140
003e37d8: mov      r2, #0x52
003e37dc: mov      r0, r7
003e37e0: mov      r1, r8
003e37e4: ldr      r3, [r5, #8]
003e37e8: bl       #0x3df140
003e37ec: ldr      r2, [r6, #0xc]
003e37f0: add      r4, r4, #1
003e37f4: cmp      r2, r4
003e37f8: bhi      #0x3e32f0
003e37fc: b        #0x3e33f8
003e3800: mov      r2, #0x47
003e3804: ldr      r3, [r5, #8]
003e3808: mov      r0, r7
003e380c: mov      r1, r8
003e3810: bl       #0x3df140
003e3814: ldr      r2, [r6, #0xc]
003e3818: add      r4, r4, #1
003e381c: cmp      r2, r4
003e3820: bhi      #0x3e32f0
003e3824: b        #0x3e33f8
003e3828: mov      r2, #0x3d
003e382c: ldr      r3, [r5, #8]
003e3830: mov      r0, r7
003e3834: mov      r1, r8
003e3838: bl       #0x3df140
003e383c: ldr      r2, [r6, #0xc]
003e3840: add      r4, r4, #1
003e3844: cmp      r2, r4
003e3848: bhi      #0x3e32f0
003e384c: b        #0x3e33f8
003e3850: mov      r2, #0x3c
003e3854: ldr      r3, [r5, #8]
003e3858: mov      r0, r7
003e385c: mov      r1, r8
003e3860: bl       #0x3df140
003e3864: ldr      r2, [r6, #0xc]
003e3868: add      r4, r4, #1
003e386c: cmp      r2, r4
003e3870: bhi      #0x3e32f0
003e3874: b        #0x3e33f8
003e3878: mov      r2, #0x3f
003e387c: ldr      r3, [r5, #8]
003e3880: mov      r0, r7
003e3884: mov      r1, r8
003e3888: bl       #0x3df140
003e388c: ldr      r2, [r6, #0xc]
003e3890: add      r4, r4, #1
003e3894: cmp      r2, r4
003e3898: bhi      #0x3e32f0
003e389c: b        #0x3e33f8
003e38a0: mov      r2, #0x2d
003e38a4: ldr      r3, [r5, #8]
003e38a8: mov      r0, r7
003e38ac: mov      r1, r8
003e38b0: bl       #0x3df140
003e38b4: ldr      r2, [r6, #0xc]
003e38b8: add      r4, r4, #1
003e38bc: cmp      r2, r4
003e38c0: bhi      #0x3e32f0
003e38c4: b        #0x3e33f8
003e38c8: mov      r2, #0x28
003e38cc: ldr      r3, [r5, #8]
003e38d0: mov      r0, r7
003e38d4: mov      r1, r8
003e38d8: bl       #0x3df140
003e38dc: ldr      r2, [r6, #0xc]
003e38e0: add      r4, r4, #1
003e38e4: cmp      r2, r4
003e38e8: bhi      #0x3e32f0
003e38ec: b        #0x3e33f8
003e38f0: mov      r2, #0x2c
003e38f4: ldr      r3, [r5, #8]
003e38f8: mov      r0, r7
003e38fc: mov      r1, r8
003e3900: bl       #0x3df140
003e3904: ldr      r2, [r6, #0xc]
003e3908: add      r4, r4, #1
003e390c: cmp      r2, r4
003e3910: bhi      #0x3e32f0
003e3914: b        #0x3e33f8
003e3918: mov      r2, #0x27
003e391c: ldr      r3, [r5, #8]
003e3920: mov      r0, r7
003e3924: mov      r1, r8
003e3928: bl       #0x3df140
003e392c: ldr      r2, [r6, #0xc]
003e3930: add      r4, r4, #1
003e3934: cmp      r2, r4
003e3938: bhi      #0x3e32f0
003e393c: b        #0x3e33f8
003e3940: cmp      sl, #0
003e3944: movne    r2, #0x78
003e3948: moveq    r2, #0x76
003e394c: ldr      r3, [r5, #8]
003e3950: mov      r0, r7
003e3954: mov      r1, r8
003e3958: bl       #0x3df140
003e395c: ldr      r2, [r6, #0xc]
003e3960: add      r4, r4, #1
003e3964: cmp      r2, r4
003e3968: bhi      #0x3e32f0
003e396c: b        #0x3e33f8
003e3970: cmp      sl, #0
003e3974: movne    r2, #0x77
003e3978: moveq    r2, #0x75
003e397c: ldr      r3, [r5, #8]
003e3980: mov      r0, r7
003e3984: mov      r1, r8
003e3988: bl       #0x3df140
003e398c: ldr      r2, [r6, #0xc]
003e3990: add      r4, r4, #1
003e3994: cmp      r2, r4
003e3998: bhi      #0x3e32f0
003e399c: b        #0x3e33f8
003e39a0: cmp      sl, #0
003e39a4: movne    r2, #0x73
003e39a8: moveq    r2, #0x71
003e39ac: ldr      r3, [r5, #8]
003e39b0: mov      r0, r7
003e39b4: mov      r1, r8
003e39b8: bl       #0x3df140
003e39bc: ldr      r2, [r6, #0xc]
003e39c0: add      r4, r4, #1
003e39c4: cmp      r2, r4
003e39c8: bhi      #0x3e32f0
003e39cc: b        #0x3e33f8
003e39d0: cmp      sl, #0
003e39d4: movne    r2, #0x70
003e39d8: moveq    r2, #0x6e
003e39dc: ldr      r3, [r5, #8]
003e39e0: mov      r0, r7
003e39e4: mov      r1, r8
003e39e8: bl       #0x3df140
003e39ec: ldr      r2, [r6, #0xc]
003e39f0: add      r4, r4, #1
003e39f4: cmp      r2, r4
003e39f8: bhi      #0x3e32f0
003e39fc: b        #0x3e33f8
003e3a00: cmp      sl, #0
003e3a04: movne    r2, #0x6f
003e3a08: moveq    r2, #0x6d
003e3a0c: ldr      r3, [r5, #8]
003e3a10: mov      r0, r7
003e3a14: mov      r1, r8
003e3a18: bl       #0x3df140
003e3a1c: ldr      r2, [r6, #0xc]
003e3a20: add      r4, r4, #1
003e3a24: cmp      r2, r4
003e3a28: bhi      #0x3e32f0
003e3a2c: b        #0x3e33f8
003e3a30: cmp      sl, #0
003e3a34: movne    r2, #0x6c
003e3a38: moveq    r2, #0x6a
003e3a3c: ldr      r3, [r5, #8]
003e3a40: mov      r0, r7
003e3a44: mov      r1, r8
003e3a48: bl       #0x3df140
003e3a4c: ldr      r2, [r6, #0xc]
003e3a50: add      r4, r4, #1
003e3a54: cmp      r2, r4
003e3a58: bhi      #0x3e32f0
003e3a5c: b        #0x3e33f8
003e3a60: cmp      sl, #0
003e3a64: movne    r2, #0x6b
003e3a68: moveq    r2, #0x69
003e3a6c: ldr      r3, [r5, #8]
003e3a70: mov      r0, r7
003e3a74: mov      r1, r8
003e3a78: bl       #0x3df140
003e3a7c: ldr      r2, [r6, #0xc]
003e3a80: add      r4, r4, #1
003e3a84: cmp      r2, r4
003e3a88: bhi      #0x3e32f0
003e3a8c: b        #0x3e33f8
003e3a90: cmp      sl, #0
003e3a94: movne    r2, #0x68
003e3a98: moveq    r2, #0x66
003e3a9c: ldr      r3, [r5, #8]
003e3aa0: mov      r0, r7
003e3aa4: mov      r1, r8
003e3aa8: bl       #0x3df140
003e3aac: ldr      r2, [r6, #0xc]
003e3ab0: add      r4, r4, #1
003e3ab4: cmp      r2, r4
003e3ab8: bhi      #0x3e32f0
003e3abc: b        #0x3e33f8
003e3ac0: cmp      sl, #0
003e3ac4: movne    r2, #0x67
003e3ac8: moveq    r2, #0x65
003e3acc: ldr      r3, [r5, #8]
003e3ad0: mov      r0, r7
003e3ad4: mov      r1, r8
003e3ad8: bl       #0x3df140
003e3adc: ldr      r2, [r6, #0xc]
003e3ae0: add      r4, r4, #1
003e3ae4: cmp      r2, r4
003e3ae8: bhi      #0x3e32f0
003e3aec: b        #0x3e33f8
003e3af0: cmp      sl, #0
003e3af4: movne    r2, #0x64
003e3af8: moveq    r2, #0x61
003e3afc: ldr      r3, [r5, #8]
003e3b00: mov      r0, r7
003e3b04: mov      r1, r8
003e3b08: bl       #0x3deca0
003e3b0c: ldr      r2, [r6, #0xc]
003e3b10: add      r4, r4, #1
003e3b14: cmp      r2, r4
003e3b18: bhi      #0x3e32f0
003e3b1c: b        #0x3e33f8
003e3b20: cmp      sl, #0
003e3b24: movne    r2, #0x63
003e3b28: moveq    r2, #0x60
003e3b2c: ldr      r3, [r5, #8]
003e3b30: mov      r0, r7
003e3b34: mov      r1, r8
003e3b38: bl       #0x3df140
003e3b3c: ldr      r2, [r6, #0xc]
003e3b40: add      r4, r4, #1
003e3b44: cmp      r2, r4
003e3b48: bhi      #0x3e32f0
003e3b4c: b        #0x3e33f8
003e3b50: cmp      sl, #0
003e3b54: movne    r2, #0x62
003e3b58: moveq    r2, #0x5f
003e3b5c: ldr      r3, [r5, #8]
003e3b60: mov      r0, r7
003e3b64: mov      r1, r8
003e3b68: bl       #0x3df140
003e3b6c: ldr      r2, [r6, #0xc]
003e3b70: add      r4, r4, #1
003e3b74: cmp      r2, r4
003e3b78: bhi      #0x3e32f0
003e3b7c: b        #0x3e33f8
003e3b80: mov      r2, #0x2b
003e3b84: ldr      r3, [r5, #8]
003e3b88: mov      r0, r7
003e3b8c: mov      r1, r8
003e3b90: bl       #0x3df140
003e3b94: ldr      r2, [r6, #0xc]
003e3b98: add      r4, r4, #1
003e3b9c: cmp      r2, r4
003e3ba0: bhi      #0x3e32f0
003e3ba4: b        #0x3e33f8
003e3ba8: mov      r2, #0x26
003e3bac: ldr      r3, [r5, #8]
003e3bb0: mov      r0, r7
003e3bb4: mov      r1, r8
003e3bb8: bl       #0x3df140
003e3bbc: ldr      r2, [r6, #0xc]
003e3bc0: add      r4, r4, #1
003e3bc4: cmp      r2, r4
003e3bc8: bhi      #0x3e32f0
003e3bcc: b        #0x3e33f8
003e3bd0: mov      r2, #0x98
003e3bd4: ldr      r3, [r5, #8]
003e3bd8: mov      r0, r7
003e3bdc: mov      r1, r8
003e3be0: bl       #0x3df140
003e3be4: ldr      r2, [r6, #0xc]
003e3be8: add      r4, r4, #1
003e3bec: cmp      r2, r4
003e3bf0: bhi      #0x3e32f0
003e3bf4: b        #0x3e33f8
003e3bf8: mov      r2, #0x97
003e3bfc: ldr      r3, [r5, #8]
003e3c00: mov      r0, r7
003e3c04: mov      r1, r8
003e3c08: bl       #0x3df140
003e3c0c: ldr      r2, [r6, #0xc]
003e3c10: add      r4, r4, #1
003e3c14: cmp      r2, r4
003e3c18: bhi      #0x3e32f0
003e3c1c: b        #0x3e33f8
003e3c20: mov      r2, #0x96
003e3c24: ldr      r3, [r5, #8]
003e3c28: mov      r0, r7
003e3c2c: mov      r1, r8
003e3c30: bl       #0x3df140
003e3c34: ldr      r2, [r6, #0xc]
003e3c38: add      r4, r4, #1
003e3c3c: cmp      r2, r4
003e3c40: bhi      #0x3e32f0
003e3c44: b        #0x3e33f8
003e3c48: mov      r2, #0x95
003e3c4c: ldr      r3, [r5, #8]
003e3c50: mov      r0, r7
003e3c54: mov      r1, r8
003e3c58: bl       #0x3df140
003e3c5c: ldr      r2, [r6, #0xc]
003e3c60: add      r4, r4, #1
003e3c64: cmp      r2, r4
003e3c68: bhi      #0x3e32f0
003e3c6c: b        #0x3e33f8
003e3c70: mov      r2, #0x95
003e3c74: ldr      r3, [r5, #8]
003e3c78: mov      r0, r7
003e3c7c: mov      r1, r8
003e3c80: bl       #0x3df140
003e3c84: mov      r0, r7
003e3c88: mov      r1, r8
003e3c8c: mov      r2, #0x96
003e3c90: ldr      r3, [r5, #8]
003e3c94: bl       #0x3df140
003e3c98: mov      r0, r7
003e3c9c: mov      r1, r8
003e3ca0: mov      r2, #0x97
003e3ca4: ldr      r3, [r5, #8]
003e3ca8: bl       #0x3df140
003e3cac: mov      r2, #0x98
003e3cb0: mov      r0, r7
003e3cb4: mov      r1, r8
003e3cb8: ldr      r3, [r5, #8]
003e3cbc: bl       #0x3df140
003e3cc0: ldr      r2, [r6, #0xc]
003e3cc4: add      r4, r4, #1
003e3cc8: cmp      r2, r4
003e3ccc: bhi      #0x3e32f0
003e3cd0: b        #0x3e33f8
003e3cd4: mov      r2, #0x4f
003e3cd8: ldr      r3, [r5, #8]
003e3cdc: mov      r0, r7
003e3ce0: mov      r1, r8
003e3ce4: bl       #0x3df140
003e3ce8: mov      r2, #0x50
003e3cec: mov      r0, r7
003e3cf0: mov      r1, r8
003e3cf4: ldr      r3, [r5, #8]
003e3cf8: bl       #0x3df140
003e3cfc: ldr      r2, [r6, #0xc]
003e3d00: add      r4, r4, #1
003e3d04: cmp      r2, r4
003e3d08: bhi      #0x3e32f0
003e3d0c: b        #0x3e33f8
003e3d10: ldrsbeq  r1, [fp], #-0x70
003e3d14: andeq    r3, r0, r8, ror #23

# _ZN12VisualObject14SetModularSkinEii
00470e18: push     {r4, lr}
00470e1c: ldr      r0, [r0, #0x2c]
00470e20: ldr      r4, [pc, #0x2c]
00470e24: cmp      r0, #0
00470e28: cmnne    r1, #1
00470e2c: add      r4, pc, r4
00470e30: bne      #0x470e38
00470e34: pop      {r4, pc}
00470e38: bl       #0x649430
00470e3c: ldr      r3, [pc, #0x14]
00470e40: ldr      r3, [r4, r3]
00470e44: ldr      r3, [r3, #0x10]
00470e48: ldr      r0, [r3, #0x1c]
00470e4c: pop      {r4, lr}
00470e50: b        #0x5890a8
00470e54: subseq   r3, r2, r4, ror #24
00470e58: strdeq   r3, r4, [r0], -r4

# _ZNK13ItemInventory22GetModularCategoryNameEj
003ffd54: cmp      r1, #8
003ffd58: addls    pc, pc, r1, lsl #2
003ffd5c: b        #0x3ffd84
003ffd60: b        #0x3ffda4
003ffd64: b        #0x3ffdb0
003ffd68: b        #0x3ffdbc
003ffd6c: b        #0x3ffdc8
003ffd70: b        #0x3ffd8c
003ffd74: b        #0x3ffd84
003ffd78: b        #0x3ffd84
003ffd7c: b        #0x3ffd84
003ffd80: b        #0x3ffd98
003ffd84: mov      r0, #0
003ffd88: bx       lr
003ffd8c: ldr      r0, [pc, #0x40]
003ffd90: add      r0, pc, r0
003ffd94: bx       lr
003ffd98: ldr      r0, [pc, #0x38]
003ffd9c: add      r0, pc, r0
003ffda0: bx       lr
003ffda4: ldr      r0, [pc, #0x30]
003ffda8: add      r0, pc, r0
003ffdac: bx       lr
003ffdb0: ldr      r0, [pc, #0x28]
003ffdb4: add      r0, pc, r0
003ffdb8: bx       lr
003ffdbc: ldr      r0, [pc, #0x20]
003ffdc0: add      r0, pc, r0
003ffdc4: bx       lr
003ffdc8: ldr      r0, [pc, #0x18]
003ffdcc: add      r0, pc, r0
003ffdd0: bx       lr
003ffdd4: subeq    r7, ip, r0, lsr r7
003ffdd8: subeq    r7, ip, r4, lsr r7
003ffddc: strdeq   r7, r8, [ip], #-0x60
003ffde0: ldrdeq   r7, r8, [ip], #-0x64
003ffde4: subeq    r7, ip, r8, ror #13
003ffde8: subeq    r7, ip, ip, ror #13

# _ZNK13ItemInventory23IsEquipmentSlotLeftHandEj
0040022c: push     {r4, lr}
00400230: ldr      r2, [r0, #0x14]
00400234: mov      r4, r1
00400238: ldr      r3, [pc, #0x80]
0040023c: ldm      r2, {r1, r2}
00400240: add      r3, pc, r3
00400244: sub      sp, sp, #8
00400248: rsb      r2, r1, r2
0040024c: cmp      r4, r2, asr #2
00400250: blo      #0x400278
00400254: ldr      r2, [pc, #0x68]
00400258: ldr      r2, [r3, r2]
0040025c: ldr      r2, [r2]
00400260: cmp      r2, #2
00400264: moveq    r3, #0
00400268: streq    r3, [r3]
0040026c: beq      #0x400278
00400270: cmp      r2, #1
00400274: beq      #0x40028c
00400278: cmp      r4, #2
0040027c: movne    r0, #0
00400280: moveq    r0, #1
00400284: add      sp, sp, #8
00400288: pop      {r4, pc}
0040028c: ldr      r0, [pc, #0x34]
00400290: ldr      r1, [pc, #0x34]
00400294: ldr      r2, [pc, #0x34]
00400298: ldr      r0, [r3, r0]
0040029c: ldr      r3, [pc, #0x30]
004002a0: movw     ip, #0x14a
004002a4: add      r1, pc, r1
004002a8: add      r2, pc, r2
004002ac: add      r3, pc, r3
004002b0: add      r0, r0, #0xa8
004002b4: str      ip, [sp]
004002b8: bl       #0x30e004
004002bc: b        #0x400278
004002c0: subseq   r4, sb, r0, asr r8
004002c4: andeq    r3, r0, r0, asr #19
004002c8: andeq    r1, r0, r0, asr #19
004002cc: subeq    lr, fp, r4, lsr r1
004002d0: subeq    r7, ip, r8, asr #2
004002d4: subeq    r7, ip, ip, lsr #4

# _ZNK12VisualObject18GetModularModuleIdEiPKc
00474568: push     {r4, r5, r6, r7, r8, lr}
0047456c: ldr      r4, [pc, #0xd0]
00474570: ldr      r7, [pc, #0xd0]
00474574: ldr      r1, [pc, #0xd0]
00474578: add      r4, pc, r4
0047457c: ldr      r3, [r4, r7]
00474580: sub      sp, sp, #0x20
00474584: add      r5, sp, #4
00474588: ldr      r3, [r3]
0047458c: mov      r6, r2
00474590: add      r1, pc, r1
00474594: mov      r2, sp
00474598: mov      r8, r0
0047459c: mov      r0, r5
004745a0: str      r3, [sp, #0x1c]
004745a4: bl       #0x3140ec
004745a8: mov      r0, r6
004745ac: bl       #0x30de54
004745b0: mov      r1, r6
004745b4: add      r2, r6, r0
004745b8: mov      r0, r5
004745bc: bl       #0x310804
004745c0: ldr      r1, [pc, #0x88]
004745c4: mov      r0, r5
004745c8: add      r1, pc, r1
004745cc: add      r2, r1, #0xa
004745d0: bl       #0x310804
004745d4: ldr      r0, [r8, #0x2c]
004745d8: cmp      r0, #0
004745dc: mvneq    r6, #0
004745e0: beq      #0x4745f0
004745e4: ldr      r1, [sp, #0x18]
004745e8: bl       #0x649464
004745ec: mov      r6, r0
004745f0: ldr      r0, [sp, #0x18]
004745f4: cmp      r0, r5
004745f8: beq      #0x474618
004745fc: cmp      r0, #0
00474600: beq      #0x474618
00474604: ldr      r1, [sp, #4]
00474608: rsb      r1, r0, r1
0047460c: cmp      r1, #0x80
00474610: bhi      #0x474638
00474614: bl       #0x708f00
00474618: ldr      r3, [r4, r7]
0047461c: ldr      r2, [sp, #0x1c]
00474620: mov      r0, r6
00474624: ldr      r3, [r3]
00474628: cmp      r2, r3
0047462c: bne      #0x474640
00474630: add      sp, sp, #0x20
00474634: pop      {r4, r5, r6, r7, r8, pc}
00474638: bl       #0x310440
0047463c: b        #0x474618
00474640: bl       #0x30e310
00474644: subseq   r0, r2, r8, lsl r5
00474648: andeq    r4, r0, ip, lsr #1
0047464c: strdeq   r2, r3, [r5], #-0xc0
00474650: subeq    sb, r5, r0, lsr r1

# _ZNK12ItemInstance9GetItemIdEv
003f9e00: ldr      r0, [r0, #4]
003f9e04: bx       lr

# _ZN12VisualObject13SetWeaponSkinEPKcii
00473cd8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00473cdc: ldr      r4, [pc, #0x1a0]
00473ce0: ldr      r8, [pc, #0x1a0]
00473ce4: subs     r7, r1, #0
00473ce8: add      r4, pc, r4
00473cec: ldr      r1, [r4, r8]
00473cf0: mov      sb, r2
00473cf4: sub      sp, sp, #0x24
00473cf8: ldr      r2, [r1]
00473cfc: mov      r5, r0
00473d00: mov      fp, r3
00473d04: str      r2, [sp, #0x1c]
00473d08: beq      #0x473e34
00473d0c: ldr      r1, [pc, #0x178]
00473d10: add      r6, sp, #4
00473d14: mov      r2, sp
00473d18: add      r1, pc, r1
00473d1c: mov      r0, r6
00473d20: bl       #0x3140ec
00473d24: mov      r0, r7
00473d28: bl       #0x30de54
00473d2c: mov      r1, r7
00473d30: add      r2, r7, r0
00473d34: mov      r0, r6
00473d38: bl       #0x310804
00473d3c: ldr      r1, [pc, #0x14c]
00473d40: ldr      sl, [pc, #0x14c]
00473d44: mov      r0, r6
00473d48: add      r1, pc, r1
00473d4c: add      r2, r1, #5
00473d50: bl       #0x310804
00473d54: ldr      r3, [r4, sl]
00473d58: ldr      r1, [sp, #0x18]
00473d5c: mov      r2, #1
00473d60: ldr      r0, [r3, #0x10]
00473d64: ldr      r3, [pc, #0x12c]
00473d68: ldr      r0, [r0, #0x10]
00473d6c: ldr      r3, [r4, r3]
00473d70: bl       #0x61bbd4
00473d74: mov      r7, r0
00473d78: ldr      r0, [sp, #0x18]
00473d7c: cmp      r0, r6
00473d80: beq      #0x473da0
00473d84: cmp      r0, #0
00473d88: beq      #0x473da0
00473d8c: ldr      r1, [sp, #4]
00473d90: rsb      r1, r0, r1
00473d94: cmp      r1, #0x80
00473d98: bhi      #0x473e78
00473d9c: bl       #0x708f00
00473da0: cmp      sb, #1
00473da4: beq      #0x473e40
00473da8: ldr      r3, [r5, #0x34]
00473dac: cmp      r3, #0
00473db0: beq      #0x473dd8
00473db4: mov      r0, r3
00473db8: ldr      r3, [r3]
00473dbc: mov      lr, pc
00473dc0: ldr      pc, [r3, #0x68]
00473dc4: ldr      r3, [r5, #0x34]
00473dc8: ldr      r2, [r3]
00473dcc: ldr      r0, [r2, #-0xc]
00473dd0: add      r0, r3, r0
00473dd4: bl       #0x31d584
00473dd8: str      r7, [r5, #0x34]
00473ddc: ldr      r3, [r4, sl]
00473de0: ldr      r2, [pc, #0xb4]
00473de4: ldr      r1, [r5, #8]
00473de8: ldr      r3, [r3, #0x10]
00473dec: add      r2, pc, r2
00473df0: ldr      r2, [r2, fp, lsl #2]
00473df4: ldr      r0, [r3, #0x1c]
00473df8: mov      r3, #0
00473dfc: bl       #0x35a0e4
00473e00: subs     r3, r0, #0
00473e04: beq      #0x473e18
00473e08: ldr      r3, [r3]
00473e0c: mov      r1, r7
00473e10: mov      lr, pc
00473e14: ldr      pc, [r3, #0x5c]
00473e18: ldr      r3, [r4, r8]
00473e1c: ldr      r2, [sp, #0x1c]
00473e20: ldr      r3, [r3]
00473e24: cmp      r2, r3
00473e28: bne      #0x473e80
00473e2c: add      sp, sp, #0x24
00473e30: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00473e34: cmp      sb, #1
00473e38: ldr      sl, [pc, #0x54]
00473e3c: bne      #0x473da8
00473e40: ldr      r3, [r5, #0x30]
00473e44: cmp      r3, #0
00473e48: beq      #0x473e70
00473e4c: mov      r0, r3
00473e50: ldr      r3, [r3]
00473e54: mov      lr, pc
00473e58: ldr      pc, [r3, #0x68]
00473e5c: ldr      r3, [r5, #0x30]
00473e60: ldr      r2, [r3]
00473e64: ldr      r0, [r2, #-0xc]
00473e68: add      r0, r3, r0
00473e6c: bl       #0x31d584
00473e70: str      r7, [r5, #0x30]
00473e74: b        #0x473ddc
00473e78: bl       #0x310440
00473e7c: b        #0x473da0
00473e80: bl       #0x30e310
00473e84: subseq   r0, r2, r8, lsr #27
00473e88: andeq    r4, r0, ip, lsr #1
00473e8c: subeq    sb, r5, r0, lsr #19
00473e90: subeq    r5, r5, r0, lsr #23
00473e94: strdeq   r3, r4, [r0], -r4
00473e98: andeq    r0, r0, ip, lsr #26
00473e9c: subeq    r2, lr, r4, ror #25

# _ZN14CharProperties14_LoadGearStatsEib
003e3154: ldr      r3, [pc, #0x150]
003e3158: ldr      ip, [pc, #0x150]
003e315c: push     {r4, r5, r6, lr}
003e3160: add      r3, pc, r3
003e3164: ldr      ip, [r3, ip]
003e3168: mov      r4, #0xa4
003e316c: mov      r5, r0
003e3170: ldr      r3, [ip]
003e3174: mla      r4, r4, r1, r3
003e3178: ldr      r3, [r4, #0x58]
003e317c: cmp      r3, #0xc
003e3180: addls    pc, pc, r3, lsl #2
003e3184: b        #0x3e32a8
003e3188: b        #0x3e31bc
003e318c: b        #0x3e31f4
003e3190: b        #0x3e31c4
003e3194: b        #0x3e31bc
003e3198: b        #0x3e3224
003e319c: b        #0x3e3224
003e31a0: b        #0x3e3268
003e31a4: b        #0x3e3294
003e31a8: b        #0x3e3294
003e31ac: b        #0x3e3294
003e31b0: b        #0x3e3294
003e31b4: b        #0x3e32a8
003e31b8: b        #0x3e3294
003e31bc: cmp      r2, #0
003e31c0: bne      #0x3e31f4
003e31c4: add      r6, r5, #0x710
003e31c8: ldr      r3, [r4, #0x8c]
003e31cc: mov      r0, r5
003e31d0: mov      r1, r6
003e31d4: mov      r2, #0x4f
003e31d8: bl       #0x3df140
003e31dc: ldr      r3, [r4, #0x90]
003e31e0: mov      r0, r5
003e31e4: mov      r1, r6
003e31e8: mov      r2, #0x50
003e31ec: pop      {r4, r5, r6, lr}
003e31f0: b        #0x3df140
003e31f4: add      r6, r5, #0x710
003e31f8: ldr      r3, [r4, #0x8c]
003e31fc: mov      r0, r5
003e3200: mov      r1, r6
003e3204: mov      r2, #0x51
003e3208: bl       #0x3df140
003e320c: ldr      r3, [r4, #0x90]
003e3210: mov      r0, r5
003e3214: mov      r1, r6
003e3218: mov      r2, #0x52
003e321c: pop      {r4, r5, r6, lr}
003e3220: b        #0x3df140
003e3224: add      r6, r0, #0x710
003e3228: ldr      r3, [r4, #0x8c]
003e322c: mov      r1, r6
003e3230: mov      r2, #0x4f
003e3234: bl       #0x3df140
003e3238: mov      r0, r5
003e323c: mov      r1, r6
003e3240: ldr      r3, [r4, #0x90]
003e3244: mov      r2, #0x50
003e3248: bl       #0x3df140
003e324c: ldr      r3, [r4, #0x64]
003e3250: mov      r0, r5
003e3254: mov      r1, r6
003e3258: lsl      r3, r3, #8
003e325c: mov      r2, #0x61
003e3260: pop      {r4, r5, r6, lr}
003e3264: b        #0x3df140
003e3268: add      r6, r0, #0x710
003e326c: ldr      r3, [r4, #0x8c]
003e3270: mov      r1, r6
003e3274: mov      r2, #0x47
003e3278: bl       #0x3df140
003e327c: ldr      r3, [r4, #0x90]
003e3280: mov      r0, r5
003e3284: mov      r1, r6
003e3288: mov      r2, #0x3d
003e328c: pop      {r4, r5, r6, lr}
003e3290: b        #0x3df140
003e3294: ldr      r3, [r4, #0x8c]
003e3298: add      r1, r0, #0x710
003e329c: mov      r2, #0x47
003e32a0: pop      {r4, r5, r6, lr}
003e32a4: b        #0x3df140
003e32a8: pop      {r4, r5, r6, pc}
003e32ac: subseq   r1, fp, r0, lsr sb
003e32b0: andeq    r2, r0, ip, ror #16

# _ZNK12VisualObject20GetModularCategoryIdEPKc
00470e5c: ldr      r0, [r0, #0x2c]
00470e60: cmp      r0, #0
00470e64: beq      #0x470e6c
00470e68: b        #0x6494e0
00470e6c: mvn      r0, #0
00470e70: bx       lr

# _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
003def34: ldr      r3, [pc, #0x40]
003def38: ldr      r2, [pc, #0x40]
003def3c: push     {r4, r5, r6, r7, r8, lr}
003def40: add      r3, pc, r3
003def44: mov      r8, r0
003def48: ldr      r7, [r3, r2]
003def4c: mov      r6, r1
003def50: mov      r4, #0
003def54: mov      r1, r4
003def58: mov      r0, r8
003def5c: ldr      r5, [r7, r4, lsl #2]
003def60: bl       #0x3def10
003def64: add      r4, r4, #1
003def68: add      r5, r5, #4
003def6c: cmp      r4, #0xe0
003def70: str      r0, [r6, r5]
003def74: bne      #0x3def54
003def78: pop      {r4, r5, r6, r7, r8, pc}
003def7c: subseq   r5, fp, r0, asr fp
003def80: andeq    r2, r0, r8, lsr #5
