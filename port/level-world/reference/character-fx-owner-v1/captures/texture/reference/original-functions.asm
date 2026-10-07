
# _ZNK6glitch7collada15animation_track19CTextureTransformEx16getIdentityValueEPv
006e3400: mov      r3, #0
006e3404: mov      r2, #0x3f800000
006e3408: str      r3, [r1]
006e340c: str      r2, [r1, #0xc]
006e3410: str      r2, [r1, #0x10]
006e3414: str      r3, [r1, #8]
006e3418: str      r3, [r1, #4]
006e341c: bx       lr

# _ZN6glitch7collada15animation_track19CTextureTransformEx10getValueExERKNS0_18SAnimationAccessorEiiPvRib
006e3b48: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006e3b4c: sub      sp, sp, #0x34
006e3b50: str      r3, [sp, #0x10]
006e3b54: mov      sl, r1
006e3b58: mov      r8, r2
006e3b5c: mov      r5, r0
006e3b60: ldrb     r6, [sp, #0x5c]
006e3b64: bl       #0x669e68
006e3b68: ldr      lr, [sp, #0x10]
006e3b6c: mov      ip, r0
006e3b70: ldm      ip!, {r0, r1, r2, r3}
006e3b74: stm      lr!, {r0, r1, r2, r3}
006e3b78: ldr      r2, [ip]
006e3b7c: mov      r0, r5
006e3b80: str      r2, [lr]
006e3b84: bl       #0x669e78
006e3b88: subs     sb, r0, #0
006e3b8c: ble      #0x6e3c78
006e3b90: mov      r4, #0
006e3b94: add      r2, sp, #0x24
006e3b98: add      r3, sp, #0x20
006e3b9c: add      ip, sp, #0x2c
006e3ba0: add      fp, sp, #0x28
006e3ba4: str      r2, [sp, #0x14]
006e3ba8: str      r3, [sp, #0x1c]
006e3bac: str      ip, [sp, #0x18]
006e3bb0: mov      r7, r4
006e3bb4: ldr      lr, [sp, #0x14]
006e3bb8: mov      r1, r4
006e3bbc: mov      r2, r8
006e3bc0: mov      r3, fp
006e3bc4: mov      r0, r5
006e3bc8: str      lr, [sp]
006e3bcc: str      r7, [sp, #0x28]
006e3bd0: bl       #0x66af34
006e3bd4: tst      r0, r6
006e3bd8: moveq    r6, #0
006e3bdc: movne    r6, #1
006e3be0: mov      r1, r4
006e3be4: mov      r2, sl
006e3be8: ldr      r3, [sp, #0x1c]
006e3bec: mov      r0, r5
006e3bf0: str      r7, [sp, #0x20]
006e3bf4: bl       #0x66b0f4
006e3bf8: cmp      r6, #0
006e3bfc: mov      r1, r4
006e3c00: mov      r0, r5
006e3c04: beq      #0x6e3cf0
006e3c08: ldr      ip, [sp, #0x28]
006e3c0c: ldr      r2, [sp, #0x20]
006e3c10: mov      r3, ip
006e3c14: add      ip, ip, #1
006e3c18: str      ip, [sp]
006e3c1c: ldr      ip, [sp, #0x24]
006e3c20: str      ip, [sp, #4]
006e3c24: ldr      ip, [sp, #0x18]
006e3c28: str      ip, [sp, #8]
006e3c2c: bl       #0x6e3ac4
006e3c30: mov      r1, r4
006e3c34: mov      r0, r5
006e3c38: bl       #0x669e10
006e3c3c: sub      r0, r0, #0x57
006e3c40: cmp      r0, #4
006e3c44: addls    pc, pc, r0, lsl #2
006e3c48: b        #0x6e3c6c
006e3c4c: b        #0x6e3cd4
006e3c50: b        #0x6e3cb8
006e3c54: b        #0x6e3c60
006e3c58: b        #0x6e3c9c
006e3c5c: b        #0x6e3c80
006e3c60: ldr      r3, [sp, #0x2c]
006e3c64: ldr      r2, [sp, #0x10]
006e3c68: str      r3, [r2, #8]
006e3c6c: add      r4, r4, #1
006e3c70: cmp      r4, sb
006e3c74: bne      #0x6e3bb4
006e3c78: add      sp, sp, #0x34
006e3c7c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006e3c80: ldr      r3, [sp, #0x2c]
006e3c84: ldr      ip, [sp, #0x10]
006e3c88: add      r4, r4, #1
006e3c8c: cmp      r4, sb
006e3c90: str      r3, [ip, #0x10]
006e3c94: bne      #0x6e3bb4
006e3c98: b        #0x6e3c78
006e3c9c: ldr      r3, [sp, #0x2c]
006e3ca0: ldr      r2, [sp, #0x10]
006e3ca4: add      r4, r4, #1
006e3ca8: cmp      r4, sb
006e3cac: str      r3, [r2, #0xc]
006e3cb0: bne      #0x6e3bb4
006e3cb4: b        #0x6e3c78
006e3cb8: ldr      r3, [sp, #0x2c]
006e3cbc: ldr      ip, [sp, #0x10]
006e3cc0: add      r4, r4, #1
006e3cc4: cmp      r4, sb
006e3cc8: str      r3, [ip, #4]
006e3ccc: bne      #0x6e3bb4
006e3cd0: b        #0x6e3c78
006e3cd4: ldr      r3, [sp, #0x2c]
006e3cd8: ldr      r2, [sp, #0x10]
006e3cdc: add      r4, r4, #1
006e3ce0: cmp      r4, sb
006e3ce4: str      r3, [r2]
006e3ce8: bne      #0x6e3bb4
006e3cec: b        #0x6e3c78
006e3cf0: ldr      lr, [sp, #0x18]
006e3cf4: ldr      r2, [sp, #0x20]
006e3cf8: ldr      r3, [sp, #0x28]
006e3cfc: str      lr, [sp]
006e3d00: bl       #0x6e3b1c
006e3d04: b        #0x6e3c30

# _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiiifPv
006e3ac4: push     {r4, r5, r6, lr}
006e3ac8: mov      r4, r2
006e3acc: mov      r6, r3
006e3ad0: bl       #0x669e24
006e3ad4: ldr      r5, [r0, #4]
006e3ad8: ldr      r3, [sp, #0x10]
006e3adc: lsl      r4, r4, #2
006e3ae0: ldr      r6, [r5, r6, lsl #2]
006e3ae4: ldr      r0, [r5, r3, lsl #2]
006e3ae8: mov      r1, r6
006e3aec: bl       #0x30e3ac
006e3af0: mov      r1, r0
006e3af4: ldr      r0, [sp, #0x14]
006e3af8: bl       #0x30ed6c
006e3afc: mov      r1, r0
006e3b00: mov      r0, r6
006e3b04: bl       #0x30eba4
006e3b08: ldr      r1, [r5, r4]
006e3b0c: bl       #0x30e3ac
006e3b10: ldr      r3, [sp, #0x18]
006e3b14: str      r0, [r3]
006e3b18: pop      {r4, r5, r6, pc}

# _ZN6glitch5video6detail19IMaterialParametersINS0_9CMaterialENS_24ISharedMemoryBlockHeaderIS3_EEE12setParameterEtjRKNS_4core8CMatrix4IfEE
005cb4dc: push     {r4, lr}
005cb4e0: ldr      ip, [r0, #4]
005cb4e4: ldrh     r4, [ip, #0xe]
005cb4e8: cmp      r4, r1
005cb4ec: bls      #0x5cb508
005cb4f0: ldr      ip, [ip, #0x20]
005cb4f4: adds     ip, ip, r1, lsl #4
005cb4f8: beq      #0x5cb508
005cb4fc: ldrb     r4, [ip, #6]
005cb500: cmp      r4, #0xb
005cb504: beq      #0x5cb510
005cb508: mov      r0, #0
005cb50c: pop      {r4, pc}
005cb510: ldr      r1, [ip, #8]
005cb514: cmp      r2, r1
005cb518: bhs      #0x5cb508
005cb51c: mvn      r1, #0
005cb520: str      r1, [r0, #0xc]
005cb524: str      r1, [r0, #0x10]
005cb528: ldr      ip, [ip, #0xc]
005cb52c: add      r0, r0, #0x20
005cb530: mov      r1, r3
005cb534: add      r2, ip, r2, lsl #2
005cb538: add      r0, r0, r2
005cb53c: mov      r2, #0
005cb540: bl       #0x5badb8
005cb544: mov      r0, #1
005cb548: pop      {r4, pc}

# _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiiPv
006e3b1c: push     {r4, r5, r6, lr}
006e3b20: mov      r4, r2
006e3b24: mov      r5, r3
006e3b28: bl       #0x669e24
006e3b2c: ldr      r3, [r0, #4]
006e3b30: ldr      r1, [r3, r4, lsl #2]
006e3b34: ldr      r0, [r3, r5, lsl #2]
006e3b38: bl       #0x30e3ac
006e3b3c: ldr      r3, [sp, #0x10]
006e3b40: str      r0, [r3]
006e3b44: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada15animation_track19CTextureTransformEx12applyValueExEPvRNS2_5SDataEPNS1_15CApplicatorInfoE
006e38c4: push     {r4, r5, r6, r7, r8, sl, lr}
006e38c8: mov      r4, r1
006e38cc: mov      r1, #0x43000000
006e38d0: sub      sp, sp, #0x6c
006e38d4: mov      r5, r0
006e38d8: add      r1, r1, #0x340000
006e38dc: ldr      r0, [r4, #8]
006e38e0: mov      r8, r2
006e38e4: bl       #0x30ec94
006e38e8: movw     r1, #0xfe9
006e38ec: movt     r1, #0x4049
006e38f0: bl       #0x30ed6c
006e38f4: ldr      lr, [r4, #0xc]
006e38f8: ldr      r6, [r4, #4]
006e38fc: ldr      r7, [r4]
006e3900: ldr      sl, [r4, #0x10]
006e3904: add      r4, sp, #0xc
006e3908: mov      ip, #0x3f000000
006e390c: mov      r1, r0
006e3910: add      r2, sp, #0x60
006e3914: add      r3, sp, #0x58
006e3918: mov      r0, r4
006e391c: str      lr, [sp, #0x50]
006e3920: add      lr, sp, #0x50
006e3924: str      ip, [sp, #0x64]
006e3928: str      r7, [sp, #0x58]
006e392c: str      r6, [sp, #0x5c]
006e3930: str      sl, [sp, #0x54]
006e3934: str      lr, [sp]
006e3938: str      ip, [sp, #0x60]
006e393c: bl       #0x6e377c
006e3940: mov      r0, r5
006e3944: ldrh     r1, [r8, #8]
006e3948: mov      r3, r4
006e394c: mov      r2, #0
006e3950: bl       #0x5cb4dc
006e3954: add      sp, sp, #0x6c
006e3958: pop      {r4, r5, r6, r7, r8, sl, pc}

# _ZN6glitch7collada15animation_track19CTextureTransformEx18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
006e3d30: push     {r4, r5, r6, lr}
006e3d34: mov      r4, r2
006e3d38: mov      r5, r3
006e3d3c: bl       #0x669e24
006e3d40: ldr      r3, [r0, #4]
006e3d44: ldr      r4, [r3, r4, lsl #2]
006e3d48: ldr      r0, [r3, r5, lsl #2]
006e3d4c: mov      r1, r4
006e3d50: bl       #0x30e3ac
006e3d54: mov      r1, r0
006e3d58: ldr      r0, [sp, #0x10]
006e3d5c: bl       #0x30ed6c
006e3d60: mov      r1, r0
006e3d64: mov      r0, r4
006e3d68: bl       #0x30eba4
006e3d6c: ldr      r3, [sp, #0x14]
006e3d70: str      r0, [r3]
006e3d74: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada15animation_track19CTextureTransformEx10getValueExERKNS0_18SAnimationAccessorEiPvRib
006e3d78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006e3d7c: sub      sp, sp, #0x24
006e3d80: str      r2, [sp, #8]
006e3d84: mov      r7, r1
006e3d88: mov      r5, r0
006e3d8c: ldrb     r6, [sp, #0x48]
006e3d90: bl       #0x669e68
006e3d94: ldr      lr, [sp, #8]
006e3d98: mov      ip, r0
006e3d9c: ldm      ip!, {r0, r1, r2, r3}
006e3da0: stm      lr!, {r0, r1, r2, r3}
006e3da4: ldr      r2, [ip]
006e3da8: mov      r0, r5
006e3dac: str      r2, [lr]
006e3db0: bl       #0x669e78
006e3db4: subs     r8, r0, #0
006e3db8: ble      #0x6e3e6c
006e3dbc: add      r2, sp, #0x1c
006e3dc0: mov      r4, #0
006e3dc4: add      sl, sp, #0x18
006e3dc8: add      sb, sp, #0x14
006e3dcc: str      r2, [sp, #0xc]
006e3dd0: mov      r1, r4
006e3dd4: mov      ip, #0
006e3dd8: mov      r2, r7
006e3ddc: mov      r3, sl
006e3de0: mov      r0, r5
006e3de4: str      ip, [sp, #0x18]
006e3de8: str      sb, [sp]
006e3dec: bl       #0x66af34
006e3df0: tst      r0, r6
006e3df4: mov      r1, r4
006e3df8: mov      r0, r5
006e3dfc: moveq    r6, #0
006e3e00: movne    r6, #1
006e3e04: beq      #0x6e3ee4
006e3e08: ldr      r2, [sp, #0x18]
006e3e0c: ldr      ip, [sp, #0x14]
006e3e10: ldr      lr, [sp, #0xc]
006e3e14: add      r3, r2, #1
006e3e18: str      ip, [sp]
006e3e1c: str      lr, [sp, #4]
006e3e20: bl       #0x6e3d30
006e3e24: mov      r1, r4
006e3e28: mov      r0, r5
006e3e2c: bl       #0x669e10
006e3e30: sub      r0, r0, #0x57
006e3e34: cmp      r0, #4
006e3e38: addls    pc, pc, r0, lsl #2
006e3e3c: b        #0x6e3e60
006e3e40: b        #0x6e3ec8
006e3e44: b        #0x6e3eac
006e3e48: b        #0x6e3e54
006e3e4c: b        #0x6e3e90
006e3e50: b        #0x6e3e74
006e3e54: ldr      r3, [sp, #0x1c]
006e3e58: ldr      r2, [sp, #8]
006e3e5c: str      r3, [r2, #8]
006e3e60: add      r4, r4, #1
006e3e64: cmp      r4, r8
006e3e68: bne      #0x6e3dd0
006e3e6c: add      sp, sp, #0x24
006e3e70: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006e3e74: ldr      r3, [sp, #0x1c]
006e3e78: ldr      ip, [sp, #8]
006e3e7c: add      r4, r4, #1
006e3e80: cmp      r4, r8
006e3e84: str      r3, [ip, #0x10]
006e3e88: bne      #0x6e3dd0
006e3e8c: b        #0x6e3e6c
006e3e90: ldr      r3, [sp, #0x1c]
006e3e94: ldr      r2, [sp, #8]
006e3e98: add      r4, r4, #1
006e3e9c: cmp      r4, r8
006e3ea0: str      r3, [r2, #0xc]
006e3ea4: bne      #0x6e3dd0
006e3ea8: b        #0x6e3e6c
006e3eac: ldr      r3, [sp, #0x1c]
006e3eb0: ldr      ip, [sp, #8]
006e3eb4: add      r4, r4, #1
006e3eb8: cmp      r4, r8
006e3ebc: str      r3, [ip, #4]
006e3ec0: bne      #0x6e3dd0
006e3ec4: b        #0x6e3e6c
006e3ec8: ldr      r3, [sp, #0x1c]
006e3ecc: ldr      r2, [sp, #8]
006e3ed0: add      r4, r4, #1
006e3ed4: cmp      r4, r8
006e3ed8: str      r3, [r2]
006e3edc: bne      #0x6e3dd0
006e3ee0: b        #0x6e3e6c
006e3ee4: ldr      fp, [sp, #0x18]
006e3ee8: bl       #0x669e24
006e3eec: ldr      r3, [r0, #4]
006e3ef0: ldr      r3, [r3, fp, lsl #2]
006e3ef4: str      r3, [sp, #0x1c]
006e3ef8: b        #0x6e3e24

# _ZNK6glitch7collada15animation_track19CTextureTransformEx15getBlendedValueEPvPfiS3_
006e3420: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006e3424: cmp      r3, #0
006e3428: sub      sp, sp, #0x24
006e342c: str      r3, [sp, #0x1c]
006e3430: str      r1, [sp, #4]
006e3434: str      r2, [sp, #0x18]
006e3438: ble      #0x6e3570
006e343c: mov      r2, #0
006e3440: mov      r3, #0x3f800000
006e3444: mov      r6, #0
006e3448: str      r2, [sp, #8]
006e344c: str      r2, [sp, #0x10]
006e3450: str      r3, [sp, #0xc]
006e3454: str      r2, [sp, #0x14]
006e3458: mov      r7, r6
006e345c: ldr      r1, [sp, #0x18]
006e3460: ldr      r2, [sp, #4]
006e3464: ldr      r5, [r1, r7, lsl #2]
006e3468: ldr      r1, [r2, r6]
006e346c: add      r4, r2, r6
006e3470: mov      r0, r5
006e3474: str      r3, [sp]
006e3478: bl       #0x30ed6c
006e347c: ldr      r1, [sp, #4]
006e3480: mov      fp, r0
006e3484: add      r7, r7, #1
006e3488: str      r0, [r1, r6]
006e348c: ldr      r1, [r4, #4]
006e3490: mov      r0, r5
006e3494: bl       #0x30ed6c
006e3498: ldr      r1, [r4, #8]
006e349c: mov      sb, r0
006e34a0: str      r0, [r4, #4]
006e34a4: mov      r0, r5
006e34a8: bl       #0x30ed6c
006e34ac: ldr      r1, [r4, #0xc]
006e34b0: mov      sl, r0
006e34b4: str      r0, [r4, #8]
006e34b8: mov      r0, r5
006e34bc: bl       #0x30ed6c
006e34c0: ldr      r1, [r4, #0x10]
006e34c4: mov      r8, r0
006e34c8: str      r0, [r4, #0xc]
006e34cc: mov      r0, r5
006e34d0: bl       #0x30ed6c
006e34d4: str      r0, [r4, #0x10]
006e34d8: mov      r5, r0
006e34dc: mov      r1, fp
006e34e0: ldr      r0, [sp, #0x14]
006e34e4: bl       #0x30eba4
006e34e8: mov      r1, sb
006e34ec: str      r0, [sp, #0x14]
006e34f0: ldr      r0, [sp, #0x10]
006e34f4: bl       #0x30eba4
006e34f8: mov      r1, sl
006e34fc: str      r0, [sp, #0x10]
006e3500: ldr      r0, [sp, #8]
006e3504: bl       #0x30eba4
006e3508: mov      r1, r8
006e350c: str      r0, [sp, #8]
006e3510: ldr      r0, [sp, #0xc]
006e3514: bl       #0x30eba4
006e3518: ldr      r3, [sp]
006e351c: str      r0, [sp, #0xc]
006e3520: mov      r1, r5
006e3524: mov      r0, r3
006e3528: bl       #0x30eba4
006e352c: ldr      r2, [sp, #0x1c]
006e3530: mov      r3, r0
006e3534: add      r6, r6, #0x14
006e3538: cmp      r7, r2
006e353c: bne      #0x6e345c
006e3540: ldr      r1, [sp, #0x14]
006e3544: ldr      r2, [sp, #0x48]
006e3548: str      r1, [r2]
006e354c: str      r3, [r2, #0x10]
006e3550: ldr      r3, [sp, #0xc]
006e3554: str      r3, [r2, #0xc]
006e3558: ldr      r1, [sp, #8]
006e355c: str      r1, [r2, #8]
006e3560: ldr      r3, [sp, #0x10]
006e3564: str      r3, [r2, #4]
006e3568: add      sp, sp, #0x24
006e356c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006e3570: mov      r1, #0
006e3574: mov      r3, #0x3f800000
006e3578: str      r1, [sp, #8]
006e357c: str      r1, [sp, #0x10]
006e3580: str      r3, [sp, #0xc]
006e3584: str      r1, [sp, #0x14]
006e3588: b        #0x6e3540

# _ZN6glitch7collada15animation_track19CTextureTransformEx12applyValueExERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoERib
006e3efc: push     {r4, r5, r6, r7, lr}
006e3f00: sub      sp, sp, #0x24
006e3f04: ldrb     r7, [sp, #0x3c]
006e3f08: add      r4, sp, #0xc
006e3f0c: mov      ip, #0
006e3f10: mov      lr, #0x3f800000
006e3f14: mov      r6, r2
006e3f18: mov      r5, r3
006e3f1c: mov      r2, r4
006e3f20: ldr      r3, [sp, #0x38]
006e3f24: str      ip, [sp, #0x14]
006e3f28: str      lr, [sp, #0x1c]
006e3f2c: str      r7, [sp]
006e3f30: str      ip, [sp, #0xc]
006e3f34: str      ip, [sp, #0x10]
006e3f38: str      lr, [sp, #0x18]
006e3f3c: bl       #0x6e3d78
006e3f40: mov      r0, r6
006e3f44: mov      r1, r4
006e3f48: mov      r2, r5
006e3f4c: bl       #0x6e38c4
006e3f50: add      sp, sp, #0x24
006e3f54: pop      {r4, r5, r6, r7, pc}
