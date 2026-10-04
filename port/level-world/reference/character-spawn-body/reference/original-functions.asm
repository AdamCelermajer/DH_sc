
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

# _ZN9Character14CancelSneakingEv
003bc6b8: push     {r4, r5, r6, lr}
003bc6bc: ldr      r3, [r0]
003bc6c0: mov      r5, r0
003bc6c4: mov      lr, pc
003bc6c8: ldr      pc, [r3, #0x28]
003bc6cc: ldr      r4, [pc, #0xa8]
003bc6d0: cmp      r0, #0
003bc6d4: add      r4, pc, r4
003bc6d8: bne      #0x3bc760
003bc6dc: mov      r0, r5
003bc6e0: bl       #0x3bc690
003bc6e4: cmp      r0, #0
003bc6e8: bne      #0x3bc6f0
003bc6ec: pop      {r4, r5, r6, pc}
003bc6f0: mov      r0, r5
003bc6f4: bl       #0x3bc5fc
003bc6f8: ldr      r2, [r0, #4]
003bc6fc: cmp      r2, #0
003bc700: beq      #0x3bc6ec
003bc704: ldr      r1, [pc, #0x74]
003bc708: ldr      r0, [r0, #8]
003bc70c: mov      r3, #0x4c
003bc710: ldr      ip, [r4, r1]
003bc714: ldr      r1, [r0]
003bc718: ldr      ip, [ip]
003bc71c: mla      r1, r3, r1, ip
003bc720: ldr      r1, [r1, #0x1c]
003bc724: ands     r1, r1, #0x2000000
003bc728: movne    r1, #0
003bc72c: bne      #0x3bc754
003bc730: mov      r4, r3
003bc734: add      r1, r1, #1
003bc738: cmp      r1, r2
003bc73c: beq      #0x3bc6ec
003bc740: ldr      r3, [r0, r1, lsl #2]
003bc744: mla      r3, r4, r3, ip
003bc748: ldr      r3, [r3, #0x1c]
003bc74c: tst      r3, #0x2000000
003bc750: beq      #0x3bc734
003bc754: add      r0, r5, #0x3c8
003bc758: pop      {r4, r5, r6, lr}
003bc75c: b        #0x3d84e0
003bc760: add      r0, r5, #0x560
003bc764: mov      r1, #0x92
003bc768: mov      r2, #0
003bc76c: bl       #0x3e101c
003bc770: mov      r3, #1
003bc774: strb     r3, [r5, #0x415]
003bc778: b        #0x3bc6dc
003bc77c: ldrheq   r8, [sp], #-0x3c
003bc780: andeq    r4, r0, ip, lsl r4

# _ZN12VisualObject11StartFadeInEf
00470ce4: bx       lr

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

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

# _ZN6CharAI12AI_SetTargetEP10GameObjectb
003d6890: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr      r5, [pc, #0x204]
003d6898: ldr      r7, [pc, #0x204]
003d689c: sub      sp, sp, #0x78
003d68a0: add      r5, pc, r5
003d68a4: ldr      r3, [r5, r7]
003d68a8: mov      r4, r0
003d68ac: cmp      r2, #0
003d68b0: ldr      r3, [r3]
003d68b4: mov      r6, r1
003d68b8: str      r1, [r4, #0x3c]
003d68bc: str      r3, [sp, #0x74]
003d68c0: bne      #0x3d6a20
003d68c4: ldr      r3, [r0, #0x40]
003d68c8: ldr      sb, [pc, #0x1d8]
003d68cc: add      r8, sp, #0x5c
003d68d0: cmp      r3, r1
003d68d4: ldrne    r1, [r0, #4]
003d68d8: ldr      sl, [r5, sb]
003d68dc: movwne   r3, #0x14d0
003d68e0: strhne   r2, [r1, r3]
003d68e4: mov      r0, sl
003d68e8: bl       #0x337888
003d68ec: ldr      r1, [pc, #0x1b8]
003d68f0: add      r2, sp, #0x10
003d68f4: mov      r0, r8
003d68f8: add      r1, pc, r1
003d68fc: bl       #0x3140ec
003d6900: mov      r0, sl
003d6904: mov      r1, r8
003d6908: bl       #0x337a88
003d690c: mov      sl, r0
003d6910: ldr      r0, [sp, #0x70]
003d6914: cmp      r0, r8
003d6918: beq      #0x3d6938
003d691c: cmp      r0, #0
003d6920: beq      #0x3d6938
003d6924: ldr      r1, [sp, #0x5c]
003d6928: rsb      r1, r0, r1
003d692c: cmp      r1, #0x80
003d6930: bhi      #0x3d6a4c
003d6934: bl       #0x708f00
003d6938: cmp      sl, #0
003d693c: beq      #0x3d69a4
003d6940: ldr      r3, [r4, #0x40]
003d6944: cmp      r3, r6
003d6948: beq      #0x3d69a4
003d694c: subs     r3, r3, #0
003d6950: movne    r3, #1
003d6954: subs     r2, r6, #0
003d6958: movne    r2, #1
003d695c: tst      r2, r3
003d6960: bne      #0x3d6a28
003d6964: cmp      r3, #0
003d6968: beq      #0x3d6a18
003d696c: ldr      sl, [r5, sb]
003d6970: add      r8, sp, #0x2c
003d6974: mov      r0, sl
003d6978: bl       #0x337888
003d697c: ldr      r1, [pc, #0x12c]
003d6980: add      r2, sp, #8
003d6984: mov      r0, r8
003d6988: add      r1, pc, r1
003d698c: bl       #0x3140ec
003d6990: mov      r0, sl
003d6994: mov      r1, r8
003d6998: bl       #0x337a88
003d699c: mov      r0, r8
003d69a0: bl       #0x318254
003d69a4: cmp      r6, #0
003d69a8: str      r6, [r4, #0x40]
003d69ac: beq      #0x3d69fc
003d69b0: ldr      r0, [r4, #4]
003d69b4: bl       #0x3a2fec
003d69b8: ldr      r2, [r4, #0x44]
003d69bc: ldr      r3, [r4, #0x40]
003d69c0: cmp      r3, r2
003d69c4: movne    r2, #0
003d69c8: strbne   r2, [r4, #0x4c]
003d69cc: movne    r2, r3
003d69d0: str      r2, [r4, #0x44]
003d69d4: mov      r0, r3
003d69d8: ldr      r3, [r3]
003d69dc: mov      lr, pc
003d69e0: ldr      pc, [r3, #0x34]
003d69e4: eor      r0, r0, #1
003d69e8: strb     r0, [r4, #0x48]
003d69ec: ldr      r1, [r4, #0x40]
003d69f0: mov      r0, r4
003d69f4: bl       #0x3d4ed8
003d69f8: strb     r0, [r4, #0x49]
003d69fc: ldr      r3, [r5, r7]
003d6a00: ldr      r2, [sp, #0x74]
003d6a04: ldr      r3, [r3]
003d6a08: cmp      r2, r3
003d6a0c: bne      #0x3d6a9c
003d6a10: add      sp, sp, #0x78
003d6a14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp      r2, #0
003d6a1c: bne      #0x3d6a5c
003d6a20: str      r6, [r4, #0x40]
003d6a24: b        #0x3d69fc
003d6a28: ldr      sl, [r5, sb]
003d6a2c: add      r8, sp, #0x44
003d6a30: mov      r0, sl
003d6a34: bl       #0x337888
003d6a38: ldr      r1, [pc, #0x74]
003d6a3c: add      r2, sp, #0xc
003d6a40: mov      r0, r8
003d6a44: add      r1, pc, r1
003d6a48: b        #0x3d698c
003d6a4c: bl       #0x310440
003d6a50: cmp      sl, #0
003d6a54: beq      #0x3d69a4
003d6a58: b        #0x3d6940
003d6a5c: ldr      sl, [r5, sb]
003d6a60: add      r8, sp, #0x14
003d6a64: mov      r0, sl
003d6a68: bl       #0x337888
003d6a6c: ldr      r1, [pc, #0x44]
003d6a70: add      r2, sp, #4
003d6a74: mov      r0, r8
003d6a78: add      r1, pc, r1
003d6a7c: bl       #0x3140ec
003d6a80: mov      r1, r8
003d6a84: mov      r0, sl
003d6a88: bl       #0x337a88
003d6a8c: mov      r0, r8
003d6a90: bl       #0x318254
003d6a94: str      r6, [r4, #0x40]
003d6a98: b        #0x3d69b0
003d6a9c: bl       #0x30e310
003d6aa0: ldrsheq  lr, [fp], #-0x10
003d6aa4: andeq    r4, r0, ip, lsr #1
003d6aa8: andeq    r0, r0, r4, lsl #17
003d6aac: subeq    lr, lr, r0, ror #27
003d6ab0: subeq    lr, lr, r8, ror #26
003d6ab4: subeq    lr, lr, ip, lsr #25
003d6ab8: subeq    lr, lr, r8, ror ip

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
