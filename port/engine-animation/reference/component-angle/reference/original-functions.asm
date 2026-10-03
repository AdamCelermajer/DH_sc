
# _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061f94c: push     {r4, lr}
0061f950: sub      sp, sp, #0x18
0061f954: mov      ip, #0
0061f958: add      r4, sp, #8
0061f95c: str      ip, [sp, #0x10]
0061f960: str      ip, [sp, #8]
0061f964: str      ip, [sp, #0xc]
0061f968: str      r4, [sp]
0061f96c: bl       #0x61f898
0061f970: ldr      r0, [sp, #0x20]
0061f974: mov      r2, r4
0061f978: ldr      r1, [sp, #0x14]
0061f97c: bl       #0x60cdbc
0061f980: add      sp, sp, #0x18
0061f984: pop      {r4, pc}

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

# _ZN6glitch4core10quaternion13fromAngleAxisEfRKNS0_8vector3dIfEE
0060cdbc: push     {r4, r5, r6, r7, r8, lr}
0060cdc0: mov      r4, r0
0060cdc4: mov      r0, r1
0060cdc8: mov      r1, #0x3f000000
0060cdcc: mov      r6, r2
0060cdd0: bl       #0x30ed6c
0060cdd4: mov      r7, r0
0060cdd8: bl       #0x30eb08
0060cddc: mov      r5, r0
0060cde0: mov      r0, r7
0060cde4: bl       #0x30e754
0060cde8: str      r0, [r4, #0xc]
0060cdec: ldr      r0, [r6]
0060cdf0: mov      r1, r5
0060cdf4: bl       #0x30ed6c
0060cdf8: str      r0, [r4]
0060cdfc: ldr      r0, [r6, #4]
0060ce00: mov      r1, r5
0060ce04: bl       #0x30ed6c
0060ce08: str      r0, [r4, #4]
0060ce0c: ldr      r0, [r6, #8]
0060ce10: mov      r1, r5
0060ce14: bl       #0x30ed6c
0060ce18: str      r0, [r4, #8]
0060ce1c: mov      r0, r4
0060ce20: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
0061f6f4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061f6f8: sub      sp, sp, #0x98
0061f6fc: add      r7, sp, #0x88
0061f700: mov      r4, #0
0061f704: mov      r8, r3
0061f708: mov      r6, r0
0061f70c: mov      sb, r1
0061f710: add      sl, sp, #0x78
0061f714: mov      r1, r2
0061f718: mov      r2, r7
0061f71c: ldr      r5, [sp, #0xbc]
0061f720: str      r4, [sp, #0x88]
0061f724: str      r4, [sp, #0x8c]
0061f728: str      r4, [sp, #0x90]
0061f72c: str      r4, [sp, #0x78]
0061f730: str      r4, [sp, #0x7c]
0061f734: str      r4, [sp, #0x80]
0061f738: str      r4, [sp, #0x68]
0061f73c: str      r4, [sp, #0x6c]
0061f740: str      r4, [sp, #0x70]
0061f744: bl       #0x61f51c
0061f748: mov      r1, r8
0061f74c: mov      r0, r6
0061f750: mov      r2, sl
0061f754: add      r8, sp, #0x68
0061f758: bl       #0x61f51c
0061f75c: mov      r0, r6
0061f760: mov      r1, sb
0061f764: add      r6, sp, #0x58
0061f768: mov      r2, r8
0061f76c: bl       #0x61f51c
0061f770: mov      r3, #0x3f800000
0061f774: mov      r2, r7
0061f778: ldr      r1, [sp, #0x94]
0061f77c: add      r7, sp, #0x48
0061f780: mov      r0, r6
0061f784: str      r3, [sp, #0x34]
0061f788: str      r3, [sp, #0x64]
0061f78c: str      r3, [sp, #0x54]
0061f790: str      r3, [sp, #0x44]
0061f794: str      r4, [sp, #0x30]
0061f798: str      r4, [sp, #0x58]
0061f79c: str      r4, [sp, #0x5c]
0061f7a0: str      r4, [sp, #0x60]
0061f7a4: str      r4, [sp, #0x48]
0061f7a8: str      r4, [sp, #0x4c]
0061f7ac: str      r4, [sp, #0x50]
0061f7b0: str      r4, [sp, #0x38]
0061f7b4: str      r4, [sp, #0x3c]
0061f7b8: str      r4, [sp, #0x40]
0061f7bc: str      r4, [sp, #0x28]
0061f7c0: str      r4, [sp, #0x2c]
0061f7c4: bl       #0x60cdbc
0061f7c8: mov      r2, sl
0061f7cc: ldr      r1, [sp, #0x84]
0061f7d0: mov      r0, r7
0061f7d4: bl       #0x60cdbc
0061f7d8: ldm      r7, {r0, r1, r2, r3}
0061f7dc: add      ip, sp, #4
0061f7e0: stm      ip, {r0, r1, r2, r3}
0061f7e4: ldr      ip, [sp, #0xb8]
0061f7e8: ldm      r6, {r1, r2, r3}
0061f7ec: add      r4, sp, #0x38
0061f7f0: str      ip, [sp, #0x14]
0061f7f4: ldr      ip, [sp, #0x64]
0061f7f8: add      r6, sp, #0x28
0061f7fc: mov      r0, r4
0061f800: str      ip, [sp]
0061f804: bl       #0x612d00
0061f808: mov      r2, r8
0061f80c: ldr      r1, [sp, #0x74]
0061f810: mov      r0, r6
0061f814: bl       #0x60cdbc
0061f818: ldr      lr, [sp, #0x28]
0061f81c: ldr      ip, [sp, #0x2c]
0061f820: ldr      r3, [sp, #0x30]
0061f824: add      lr, lr, #0x80000000
0061f828: add      ip, ip, #0x80000000
0061f82c: add      r3, r3, #0x80000000
0061f830: mov      r1, r6
0061f834: mov      r2, r4
0061f838: add      r0, sp, #0x18
0061f83c: str      r3, [sp, #0x30]
0061f840: str      lr, [sp, #0x28]
0061f844: str      ip, [sp, #0x2c]
0061f848: bl       #0x60dd34
0061f84c: ldr      r1, [sp, #0x1c]
0061f850: ldr      r3, [sp, #0x20]
0061f854: ldr      r2, [sp, #0x24]
0061f858: ldr      r0, [sp, #0x18]
0061f85c: str      r1, [r5, #4]
0061f860: str      r2, [r5, #0xc]
0061f864: str      r0, [r5]
0061f868: str      r3, [r5, #8]
0061f86c: add      sp, sp, #0x98
0061f870: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE13retrieveValueEPvSA_
0061cbf4: push     {r4, lr}
0061cbf8: ldr      r3, [r1]
0061cbfc: mov      r0, r1
0061cc00: mov      r4, r2
0061cc04: mov      lr, pc
0061cc08: ldr      pc, [r3, #0x98]
0061cc0c: ldr      r3, [r0]
0061cc10: str      r3, [r4]
0061cc14: ldr      r3, [r0, #4]
0061cc18: str      r3, [r4, #4]
0061cc1c: ldr      r3, [r0, #8]
0061cc20: str      r3, [r4, #8]
0061cc24: ldr      r3, [r0, #0xc]
0061cc28: str      r3, [r4, #0xc]
0061cc2c: pop      {r4, pc}

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

# _ZNK6glitch7collada18SAnimationAccessor9getOutputEi
00669e24: ldm      r0, {r2, r3}
00669e28: mov      r0, #0x1c
00669e2c: ldr      r2, [r2, #8]
00669e30: mla      r2, r0, r1, r2
00669e34: ldr      r2, [r2, #0x18]
00669e38: add      r3, r3, r2, lsl #3
00669e3c: add      r0, r3, #4
00669e40: bx       lr

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_30CSceneNodeQuaternionAngleMixinIfEEEEE11getInstanceEv
006100dc: push     {r4, r5, r6, lr}
006100e0: ldr      r4, [pc, #0x70]
006100e4: ldr      r3, [pc, #0x70]
006100e8: add      r4, pc, r4
006100ec: ldr      r6, [r4, r3]
006100f0: ldr      r3, [r6]
006100f4: tst      r3, #1
006100f8: beq      #0x610108
006100fc: ldr      r5, [pc, #0x5c]
00610100: ldr      r0, [r4, r5]
00610104: pop      {r4, r5, r6, pc}
00610108: mov      r0, r6
0061010c: bl       #0x30e76c
00610110: cmp      r0, #0
00610114: beq      #0x6100fc
00610118: ldr      r3, [pc, #0x44]
0061011c: ldr      r5, [pc, #0x3c]
00610120: mov      r0, r6
00610124: ldr      r3, [r4, r3]
00610128: ldr      r6, [r4, r5]
0061012c: add      r3, r3, #8
00610130: str      r3, [r6]
00610134: bl       #0x30ea3c
00610138: ldr      r3, [pc, #0x28]
0061013c: mov      r0, r6
00610140: ldr      r1, [r4, r3]
00610144: ldr      r3, [pc, #0x20]
00610148: ldr      r2, [r4, r3]
0061014c: bl       #0x30e304
00610150: ldr      r0, [r4, r5]
00610154: pop      {r4, r5, r6, pc}
00610158: eorseq   r4, r8, r8, lsr #19
0061015c: ldrdeq   r1, r2, [r0], -r8
00610160: andeq    r4, r0, r4, lsr #3
00610164: andeq    r2, r0, r4, asr r7
00610168: andeq    r2, r0, r4, ror #4
0061016c: muleq    r0, r0, r8

# _ZN6glitch7collada15animation_track12CInterpreterINS1_30CSceneNodeQuaternionAngleMixinIfEEfLi4ENS1_17SUseDefaultValuesILi3EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061f898: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061f89c: mov      r4, r1
0061f8a0: mov      r1, #0
0061f8a4: mov      r5, r2
0061f8a8: mov      r8, r3
0061f8ac: mov      r7, r0
0061f8b0: ldr      r6, [sp, #0x20]
0061f8b4: bl       #0x669e24
0061f8b8: ldr      sb, [r0, #4]
0061f8bc: mov      r0, r7
0061f8c0: bl       #0x669e54
0061f8c4: cmp      r0, #0
0061f8c8: beq      #0x61f91c
0061f8cc: mov      sl, #0
0061f8d0: mov      r0, r7
0061f8d4: bl       #0x669e68
0061f8d8: ldr      r3, [r0, sl]
0061f8dc: str      r3, [r6, sl]
0061f8e0: add      sl, sl, #4
0061f8e4: cmp      sl, #0xc
0061f8e8: bne      #0x61f8d0
0061f8ec: ldr      r4, [sb, r4, lsl #2]
0061f8f0: ldr      r0, [sb, r5, lsl #2]
0061f8f4: mov      r1, r4
0061f8f8: bl       #0x30e3ac
0061f8fc: mov      r1, r0
0061f900: mov      r0, r8
0061f904: bl       #0x30ed6c
0061f908: mov      r1, r0
0061f90c: mov      r0, r4
0061f910: bl       #0x30eba4
0061f914: str      r0, [r6, #0xc]
0061f918: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061f91c: ldr      r4, [sb, r4, lsl #2]
0061f920: ldr      r0, [sb, r5, lsl #2]
0061f924: mov      r1, r4
0061f928: bl       #0x30e3ac
0061f92c: mov      r1, r0
0061f930: mov      r0, r8
0061f934: bl       #0x30ed6c
0061f938: mov      r1, r0
0061f93c: mov      r0, r4
0061f940: bl       #0x30eba4
0061f944: str      r0, [r6]
0061f948: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada18SAnimationAccessor15getDefaultValueEv
00669e68: ldr      r3, [r0]
00669e6c: ldr      r3, [r3, #0x18]
00669e70: ldr      r0, [r3, #8]
00669e74: bx       lr

# _ZN6glitch7collada15CResFileManager15postLoadProcessEPNS0_8CResFileEPNS_2io9IReadFileE
00658c90: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00658c94: ldr      r3, [pc, #0x7dc]
00658c98: ldr      r4, [pc, #0x7dc]
00658c9c: sub      sp, sp, #0x84
00658ca0: add      r3, pc, r3
00658ca4: str      r3, [sp, #0x14]
00658ca8: ldr      r5, [pc, #0x7d0]
00658cac: ldr      r3, [r3, r4]
00658cb0: str      r4, [sp, #0x28]
00658cb4: str      r5, [sp, #0x30]
00658cb8: mov      sl, r1
00658cbc: ldr      lr, [r3]
00658cc0: ldr      r4, [sp, #0x14]
00658cc4: ldr      r1, [r1, #4]
00658cc8: ldr      ip, [sl, #0x24]
00658ccc: ldr      r3, [r4, r5]
00658cd0: str      lr, [sp, #0x7c]
00658cd4: ldr      r4, [ip, #0x20]
00658cd8: cmp      r1, #0
00658cdc: addne    r1, r1, #1
00658ce0: str      r3, [sp, #0x44]
00658ce4: str      r2, [sp, #0x18]
00658ce8: str      sl, [sp, #0x40]
00658cec: strne    r1, [sl, #4]
00658cf0: ldr      r3, [r4, #0x10]
00658cf4: mov      fp, r0
00658cf8: cmp      r3, #0
00658cfc: bne      #0x65938c
00658d00: add      r5, sp, #0x40
00658d04: mov      r0, r5
00658d08: bl       #0x60e2a8
00658d0c: ldr      r1, [pc, #0x770]
00658d10: add      r1, pc, r1
00658d14: bl       #0x30e31c
00658d18: cmp      r0, #0
00658d1c: bne      #0x6593dc
00658d20: ldr      r3, [sl, #0x4c]
00658d24: cmp      r3, #0
00658d28: streq    r3, [r4, #4]
00658d2c: bne      #0x6593a0
00658d30: ldr      r8, [r4, #0x24]
00658d34: cmp      r8, #0
00658d38: ble      #0x658d60
00658d3c: mov      r6, #0
00658d40: ldr      r7, [r4, #0x28]
00658d44: add      r7, r7, r6, lsl #5
00658d48: mov      r0, r7
00658d4c: bl       #0x611ae0
00658d50: add      r6, r6, #1
00658d54: cmp      r6, r8
00658d58: str      r0, [r7, #0x14]
00658d5c: bne      #0x658d40
00658d60: add      lr, sp, #0x64
00658d64: str      lr, [sp, #0x1c]
00658d68: mov      r0, lr
00658d6c: mov      r1, #0x10
00658d70: ldr      sb, [r4, #0x4c]
00658d74: str      lr, [sp, #0x74]
00658d78: str      lr, [sp, #0x78]
00658d7c: bl       #0x3209a8
00658d80: ldr      r3, [sp, #0x74]
00658d84: mov      r2, #0
00658d88: add      r6, sp, #0x4c
00658d8c: strb     r2, [r3]
00658d90: ldr      r3, [fp, #0x20]
00658d94: add      r2, sl, #0xc
00658d98: mov      r0, r6
00658d9c: ldr      r3, [r3, #0x34]
00658da0: mov      r1, r3
00658da4: ldr      r3, [r3]
00658da8: mov      lr, pc
00658dac: ldr      pc, [r3, #0x38]
00658db0: ldr      r0, [sp, #0x1c]
00658db4: ldr      r1, [sp, #0x60]
00658db8: ldr      r2, [sp, #0x5c]
00658dbc: bl       #0x320b88
00658dc0: ldr      r0, [sp, #0x60]
00658dc4: cmp      r0, r6
00658dc8: beq      #0x658dd8
00658dcc: cmp      r0, #0
00658dd0: beq      #0x658dd8
00658dd4: bl       #0x310450
00658dd8: ldr      r3, [fp, #0x20]
00658ddc: ldr      r0, [r3, #0x34]
00658de0: str      r0, [sp, #0x24]
00658de4: ldr      r3, [r3, #0x10]
00658de8: cmp      r0, #0
00658dec: ldr      r3, [r3, #0xe0]
00658df0: str      r3, [sp, #0x20]
00658df4: ldrne    r3, [r0, #4]
00658df8: addne    r3, r3, #1
00658dfc: strne    r3, [r0, #4]
00658e00: cmp      sb, #0
00658e04: ble      #0x658ee4
00658e08: mov      r7, #0
00658e0c: add      r1, sp, #0x38
00658e10: str      r5, [sp, #0x34]
00658e14: mov      r8, r7
00658e18: str      r1, [sp, #0x2c]
00658e1c: mov      r5, r4
00658e20: b        #0x658e34
00658e24: add      r8, r8, #1
00658e28: cmp      r8, sb
00658e2c: add      r7, r7, #0x14
00658e30: beq      #0x658edc
00658e34: ldr      r4, [r5, #0x50]
00658e38: add      r4, r4, r7
00658e3c: ldr      r3, [r4, #0xc]
00658e40: cmp      r3, #0
00658e44: bne      #0x658e24
00658e48: bl       #0x60adc0
00658e4c: mov      r6, r0
00658e50: mov      r0, #3
00658e54: bl       #0x60ad80
00658e58: ldr      r0, [fp, #0x24]
00658e5c: ldr      lr, [sp, #0x18]
00658e60: ldr      r3, [sp, #0x1c]
00658e64: ldr      ip, [r0]
00658e68: mov      r1, r0
00658e6c: ldr      r0, [sp, #0x20]
00658e70: mov      r2, sl
00658e74: str      lr, [sp]
00658e78: stmib    sp, {r0, r4}
00658e7c: ldr      r0, [sp, #0x2c]
00658e80: mov      lr, pc
00658e84: ldr      pc, [ip, #8]
00658e88: mov      r0, r6
00658e8c: bl       #0x60ad80
00658e90: ldr      r3, [sp, #0x38]
00658e94: cmp      r3, #0
00658e98: beq      #0x658e24
00658e9c: ldr      r2, [r3, #4]
00658ea0: add      r2, r2, #1
00658ea4: str      r2, [r3, #4]
00658ea8: ldr      r0, [r4, #0x10]
00658eac: str      r3, [r4, #0x10]
00658eb0: cmp      r0, #0
00658eb4: beq      #0x658ebc
00658eb8: bl       #0x31d584
00658ebc: ldr      r0, [sp, #0x38]
00658ec0: cmp      r0, #0
00658ec4: beq      #0x658e24
00658ec8: add      r8, r8, #1
00658ecc: bl       #0x31d584
00658ed0: cmp      r8, sb
00658ed4: add      r7, r7, #0x14
00658ed8: bne      #0x658e34
00658edc: mov      r4, r5
00658ee0: ldr      r5, [sp, #0x34]
00658ee4: ldr      r3, [r4, #0x5c]
00658ee8: cmp      r3, #0
00658eec: ble      #0x658fc8
00658ef0: ldr      r2, [pc, #0x590]
00658ef4: mov      r7, #0
00658ef8: add      r1, sp, #0x38
00658efc: add      r2, pc, r2
00658f00: mov      r8, r7
00658f04: str      r2, [sp, #0x2c]
00658f08: str      r1, [sp, #0x18]
00658f0c: mov      fp, r7
00658f10: mov      sb, #0x14
00658f14: mov      sl, r3
00658f18: str      r5, [sp, #0x20]
00658f1c: ldr      r5, [r4, #0x60]
00658f20: ldr      r2, [r4, #0x54]
00658f24: add      r5, r5, r7
00658f28: ldr      r3, [r5, #0x18]
00658f2c: cmp      r3, r2
00658f30: strgt    fp, [r5, #0x18]
00658f34: bgt      #0x658fb4
00658f38: ldr      r1, [r5, #0x10]
00658f3c: cmp      r1, #0
00658f40: ble      #0x658f9c
00658f44: mov      r3, #0
00658f48: mov      r2, r3
00658f4c: ldr      ip, [r5, #0x14]
00658f50: add      ip, ip, r3
00658f54: ldr      r0, [ip, #8]
00658f58: cmp      r0, #0xa
00658f5c: bls      #0x658f88
00658f60: cmp      r0, #0xe
00658f64: bhi      #0x658f88
00658f68: ldr      r0, [ip, #0x14]
00658f6c: ldr      r0, [r0]
00658f70: ldr      ip, [r0]
00658f74: cmn      ip, #1
00658f78: ldrne    lr, [r4, #0x50]
00658f7c: streq    fp, [r0]
00658f80: mlane    ip, sb, ip, lr
00658f84: strne    ip, [r0]
00658f88: add      r2, r2, #1
00658f8c: cmp      r2, r1
00658f90: add      r3, r3, #0x18
00658f94: bne      #0x658f4c
00658f98: ldr      r3, [r5, #0x18]
00658f9c: cmn      r3, #1
00658fa0: beq      #0x659264
00658fa4: ldr      r2, [r4, #0x58]
00658fa8: mov      lr, #0x74
00658fac: mla      r3, lr, r3, r2
00658fb0: str      r3, [r5, #0x18]
00658fb4: add      r8, r8, #1
00658fb8: cmp      r8, sl
00658fbc: add      r7, r7, #0x24
00658fc0: bne      #0x658f1c
00658fc4: ldr      r5, [sp, #0x20]
00658fc8: ldr      r7, [r4, #0x54]
00658fcc: cmp      r7, #0
00658fd0: ble      #0x6591cc
00658fd4: mov      lr, #0
00658fd8: mov      r6, lr
00658fdc: mov      ip, lr
00658fe0: mov      r0, #0x14
00658fe4: ldr      r3, [r4, #0x58]
00658fe8: add      r3, r3, lr
00658fec: cmn      r3, #8
00658ff0: beq      #0x659060
00658ff4: ldr      r8, [r3, #0x10]
00658ff8: cmp      r8, #0
00658ffc: ble      #0x659060
00659000: mov      r2, #0
00659004: mov      r1, r2
00659008: ldr      sb, [r3, #0x14]
0065900c: add      sb, sb, r2
00659010: ldr      sl, [sb, #4]
00659014: cmp      sl, #0xa
00659018: bls      #0x659050
0065901c: cmp      sl, #0xe
00659020: bhi      #0x659050
00659024: ldr      sl, [sb, #0x14]
00659028: ldr      sb, [sl]
0065902c: ldr      sl, [sb]
00659030: cmn      sl, #1
00659034: streq    ip, [sb]
00659038: beq      #0x659050
0065903c: ldr      fp, [r4, #0x4c]
00659040: cmp      sl, fp
00659044: ldrlt    fp, [r4, #0x50]
00659048: mlalt    sl, r0, sl, fp
0065904c: strlt    sl, [sb]
00659050: add      r1, r1, #1
00659054: cmp      r1, r8
00659058: add      r2, r2, #0x18
0065905c: bne      #0x659008
00659060: cmn      r3, #0x20
00659064: beq      #0x6590d4
00659068: ldr      r8, [r3, #0x28]
0065906c: cmp      r8, #0
00659070: ble      #0x6590d4
00659074: mov      r2, #0
00659078: mov      r1, r2
0065907c: ldr      sb, [r3, #0x2c]
00659080: add      sb, sb, r2
00659084: ldr      sl, [sb, #4]
00659088: cmp      sl, #0xa
0065908c: bls      #0x6590c4
00659090: cmp      sl, #0xe
00659094: bhi      #0x6590c4
00659098: ldr      sl, [sb, #0x14]
0065909c: ldr      sb, [sl]
006590a0: ldr      sl, [sb]
006590a4: cmn      sl, #1
006590a8: streq    ip, [sb]
006590ac: beq      #0x6590c4
006590b0: ldr      fp, [r4, #0x4c]
006590b4: cmp      sl, fp
006590b8: ldrlt    fp, [r4, #0x50]
006590bc: mlalt    sl, r0, sl, fp
006590c0: strlt    sl, [sb]
006590c4: add      r1, r1, #1
006590c8: cmp      r1, r8
006590cc: add      r2, r2, #0x18
006590d0: bne      #0x65907c
006590d4: cmn      r3, #0x3c
006590d8: beq      #0x659148
006590dc: ldr      r8, [r3, #0x44]
006590e0: cmp      r8, #0
006590e4: ble      #0x659148
006590e8: mov      r2, #0
006590ec: mov      r1, r2
006590f0: ldr      sb, [r3, #0x48]
006590f4: add      sb, sb, r2
006590f8: ldr      sl, [sb, #4]
006590fc: cmp      sl, #0xa
00659100: bls      #0x659138
00659104: cmp      sl, #0xe
00659108: bhi      #0x659138
0065910c: ldr      sl, [sb, #0x14]
00659110: ldr      sb, [sl]
00659114: ldr      sl, [sb]
00659118: cmn      sl, #1
0065911c: streq    ip, [sb]
00659120: beq      #0x659138
00659124: ldr      fp, [r4, #0x4c]
00659128: cmp      sl, fp
0065912c: ldrlt    fp, [r4, #0x50]
00659130: mlalt    sl, r0, sl, fp
00659134: strlt    sl, [sb]
00659138: add      r1, r1, #1
0065913c: cmp      r1, r8
00659140: add      r2, r2, #0x18
00659144: bne      #0x6590f0
00659148: cmn      r3, #0x58
0065914c: beq      #0x6591bc
00659150: ldr      r8, [r3, #0x60]
00659154: cmp      r8, #0
00659158: ble      #0x6591bc
0065915c: mov      r2, #0
00659160: mov      r1, r2
00659164: ldr      sb, [r3, #0x64]
00659168: add      sb, sb, r2
0065916c: ldr      sl, [sb, #4]
00659170: cmp      sl, #0xa
00659174: bls      #0x6591ac
00659178: cmp      sl, #0xe
0065917c: bhi      #0x6591ac
00659180: ldr      sl, [sb, #0x14]
00659184: ldr      sb, [sl]
00659188: ldr      sl, [sb]
0065918c: cmn      sl, #1
00659190: streq    ip, [sb]
00659194: beq      #0x6591ac
00659198: ldr      fp, [r4, #0x4c]
0065919c: cmp      sl, fp
006591a0: ldrlt    fp, [r4, #0x50]
006591a4: mlalt    sl, r0, sl, fp
006591a8: strlt    sl, [sb]
006591ac: add      r1, r1, #1
006591b0: cmp      r1, r8
006591b4: add      r2, r2, #0x18
006591b8: bne      #0x659164
006591bc: add      r6, r6, #1
006591c0: cmp      r6, r7
006591c4: add      lr, lr, #0x74
006591c8: bne      #0x658fe4
006591cc: ldr      r3, [sp, #0x40]
006591d0: ldr      r2, [r3, #0x24]
006591d4: ldr      r2, [r2, #0x20]
006591d8: ldr      fp, [r2, #0x70]
006591dc: cmp      fp, #0
006591e0: ble      #0x6592b8
006591e4: mov      r8, #0
006591e8: b        #0x6591f8
006591ec: add      r8, r8, #1
006591f0: cmp      r8, fp
006591f4: beq      #0x6592b4
006591f8: mov      r0, r5
006591fc: mov      r1, r8
00659200: bl       #0x60e434
00659204: ldr      r3, [r0]
00659208: cmp      r3, #1
0065920c: bne      #0x6591ec
00659210: ldr      sb, [r0, #8]
00659214: ldr      sl, [sb, #0x10]
00659218: cmp      sl, #0
0065921c: ble      #0x6591ec
00659220: mov      r6, #0
00659224: b        #0x659234
00659228: add      r6, r6, #1
0065922c: cmp      r6, sl
00659230: beq      #0x6591ec
00659234: ldr      r3, [sp, #0x40]
00659238: ldr      r7, [sb, #0x14]
0065923c: ldr      r3, [r3, #0x24]
00659240: ldr      r1, [r7, r6, lsl #2]
00659244: ldr      r3, [r3, #0x20]
00659248: ldr      r3, [r3, #0x68]
0065924c: cmp      r1, r3
00659250: bhi      #0x659228
00659254: mov      r0, r5
00659258: bl       #0x60e41c
0065925c: str      r0, [r7, r6, lsl #2]
00659260: b        #0x659228
00659264: ldr      r1, [r5, #8]
00659268: cmp      r1, #0
0065926c: streq    r1, [r5, #0x18]
00659270: beq      #0x658fb4
00659274: ldr      ip, [sp, #0x14]
00659278: ldr      r3, [sp, #0x30]
0065927c: ldr      r0, [sp, #0x18]
00659280: ldr      r2, [ip, r3]
00659284: bl       #0x60f25c
00659288: ldr      r6, [sp, #0x38]
0065928c: cmp      r6, #0
00659290: beq      #0x65945c
00659294: ldr      r1, [r5, #0xc]
00659298: ldr      r0, [sp, #0x18]
0065929c: add      r1, r1, #1
006592a0: bl       #0x61b0ac
006592a4: str      r0, [r5, #0x18]
006592a8: ldr      r0, [sp, #0x18]
006592ac: bl       #0x619474
006592b0: b        #0x658fb4
006592b4: ldr      r3, [sp, #0x40]
006592b8: ldr      r3, [r3, #0x24]
006592bc: ldr      r3, [r3, #0x20]
006592c0: ldr      r7, [r3, #0x78]
006592c4: cmp      r7, #0
006592c8: ble      #0x659324
006592cc: mov      r6, #0
006592d0: b        #0x6592e0
006592d4: add      r6, r6, #1
006592d8: cmp      r6, r7
006592dc: beq      #0x659324
006592e0: mov      r0, r5
006592e4: mov      r1, r6
006592e8: bl       #0x60e468
006592ec: ldr      r3, [r0, #0x50]
006592f0: cmp      r3, #2
006592f4: bne      #0x6592d4
006592f8: ldr      r8, [r0, #0x54]
006592fc: mov      r0, r5
00659300: add      r6, r6, #1
00659304: ldr      r1, [r8]
00659308: add      r1, r1, #1
0065930c: bl       #0x61c290
00659310: ldr      r3, [r0, #0x44]
00659314: cmp      r6, r7
00659318: ldr      r3, [r3, #4]
0065931c: str      r3, [r8, #4]
00659320: bne      #0x6592e0
00659324: ldr      r0, [sp, #0x24]
00659328: mov      r3, #1
0065932c: str      r3, [r4, #0x10]
00659330: cmp      r0, #0
00659334: beq      #0x659340
00659338: ldr      r0, [sp, #0x24]
0065933c: bl       #0x31d584
00659340: ldr      r0, [sp, #0x78]
00659344: ldr      r1, [sp, #0x1c]
00659348: cmp      r0, r1
0065934c: beq      #0x65935c
00659350: cmp      r0, #0
00659354: beq      #0x65935c
00659358: bl       #0x310450
0065935c: mov      r0, r5
00659360: bl       #0x619474
00659364: ldr      r2, [sp, #0x28]
00659368: ldr      r4, [sp, #0x14]
0065936c: mov      r0, #0
00659370: ldr      r3, [r4, r2]
00659374: ldr      r2, [sp, #0x7c]
00659378: ldr      r3, [r3]
0065937c: cmp      r2, r3
00659380: bne      #0x659474
00659384: add      sp, sp, #0x84
00659388: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065938c: mov      r1, sl
00659390: ldr      r2, [sp, #0x18]
00659394: bl       #0x658564
00659398: add      r5, sp, #0x40
0065939c: b        #0x65935c
006593a0: ldr      ip, [sp, #0x18]
006593a4: ldr      r3, [ip]
006593a8: mov      r0, ip
006593ac: mov      lr, pc
006593b0: ldr      pc, [r3, #0x2c]
006593b4: mov      r1, #0
006593b8: mov      r6, r0
006593bc: mov      r0, #0x18
006593c0: bl       #0x5341ac
006593c4: mov      r1, r6
006593c8: mov      r7, r0
006593cc: add      r2, sp, #0x48
006593d0: bl       #0x32603c
006593d4: str      r7, [r4, #4]
006593d8: b        #0x658d30
006593dc: ldr      r0, [pc, #0xa8]
006593e0: mov      r1, #2
006593e4: add      r0, pc, r0
006593e8: bl       #0x60aca0
006593ec: ldr      r0, [pc, #0x9c]
006593f0: mov      r1, #2
006593f4: add      r0, pc, r0
006593f8: bl       #0x60aca0
006593fc: mov      r1, #2
00659400: ldr      r0, [sl, #0x20]
00659404: bl       #0x60aca0
00659408: mov      r0, r5
0065940c: bl       #0x60e2a8
00659410: mov      r1, #2
00659414: bl       #0x60aca0
00659418: ldr      r0, [pc, #0x74]
0065941c: mov      r1, #2
00659420: add      r0, pc, r0
00659424: bl       #0x60aca0
00659428: ldr      r0, [pc, #0x68]
0065942c: mov      r1, #2
00659430: add      r0, pc, r0
00659434: bl       #0x60aca0
00659438: ldr      r0, [pc, #0x5c]
0065943c: mov      r1, #2
00659440: add      r0, pc, r0
00659444: bl       #0x60aca0
00659448: ldr      r3, [sl, #0x4c]
0065944c: cmp      r3, #0
00659450: streq    r3, [r4, #4]
00659454: beq      #0x658d30
00659458: b        #0x6593a0
0065945c: mov      r0, #3
00659460: ldr      r1, [sp, #0x2c]
00659464: ldr      r2, [r5, #8]
00659468: bl       #0x60b034
0065946c: str      r6, [r5, #0x18]
00659470: b        #0x6592a8
00659474: bl       #0x30e310
00659478: ldrshteq fp, [r3], -r0
0065947c: andeq    r4, r0, ip, lsr #1
00659480: andeq    r4, r0, r0, lsl r7
00659484: eoreq    ip, r8, r0, lsl #19
00659488: mlaeq    r8, r4, r8, ip

# _ZN6glitch7collada15animation_track12CInterpreterINS1_30CSceneNodeQuaternionAngleMixinIfEEfLi4ENS1_17SUseDefaultValuesILi3EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061f51c: push     {r4, r5, r6, r7, r8, lr}
0061f520: mov      r4, r1
0061f524: mov      r1, #0
0061f528: mov      r6, r2
0061f52c: mov      r5, r0
0061f530: bl       #0x669e24
0061f534: ldr      r7, [r0, #4]
0061f538: mov      r0, r5
0061f53c: bl       #0x669e54
0061f540: cmp      r0, #0
0061f544: bne      #0x61f554
0061f548: ldr      r3, [r7, r4, lsl #2]
0061f54c: str      r3, [r6]
0061f550: pop      {r4, r5, r6, r7, r8, pc}
0061f554: mov      r0, r5
0061f558: bl       #0x669e68
0061f55c: cmp      r0, #0
0061f560: beq      #0x61f548
0061f564: mov      r0, r5
0061f568: bl       #0x669e68
0061f56c: ldr      r2, [r0]
0061f570: mov      r3, r6
0061f574: str      r2, [r3], #4
0061f578: ldr      r2, [r0, #4]
0061f57c: str      r2, [r6, #4]
0061f580: ldr      r2, [r0, #8]
0061f584: str      r2, [r3, #4]
0061f588: ldr      r2, [r7, r4, lsl #2]
0061f58c: str      r2, [r3, #8]
0061f590: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada18SAnimationAccessor11getAnimatorEv
0066a004: ldr      r3, [r0]
0066a008: ldr      r0, [r3, #0x14]
0066a00c: bx       lr

# _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
0061f5e4: push     {r4, r5, r6, r7, r8, sl, lr}
0061f5e8: sub      sp, sp, #0x54
0061f5ec: add      r6, sp, #0x40
0061f5f0: mov      r4, #0
0061f5f4: mov      r7, r0
0061f5f8: mov      r8, r1
0061f5fc: add      r5, sp, #0x30
0061f600: mov      r1, r2
0061f604: mov      r2, r6
0061f608: mov      sl, r3
0061f60c: str      r4, [sp, #0x40]
0061f610: str      r4, [sp, #0x44]
0061f614: str      r4, [sp, #0x48]
0061f618: str      r4, [sp, #0x30]
0061f61c: str      r4, [sp, #0x34]
0061f620: str      r4, [sp, #0x38]
0061f624: bl       #0x61f51c
0061f628: mov      r0, r7
0061f62c: mov      r1, r8
0061f630: mov      r2, r5
0061f634: add      r7, sp, #0x20
0061f638: bl       #0x61f51c
0061f63c: mov      r3, #0x3f800000
0061f640: mov      r2, r6
0061f644: ldr      r1, [sp, #0x4c]
0061f648: add      r6, sp, #0x10
0061f64c: mov      r0, r7
0061f650: str      r3, [sp, #0x1c]
0061f654: str      r3, [sp, #0x2c]
0061f658: str      r4, [sp, #0x18]
0061f65c: str      r4, [sp, #0x20]
0061f660: str      r4, [sp, #0x24]
0061f664: str      r4, [sp, #0x28]
0061f668: str      r4, [sp, #0x10]
0061f66c: str      r4, [sp, #0x14]
0061f670: bl       #0x60cdbc
0061f674: mov      r2, r5
0061f678: ldr      r1, [sp, #0x3c]
0061f67c: mov      r0, r6
0061f680: bl       #0x60cdbc
0061f684: ldr      lr, [sp, #0x10]
0061f688: ldr      ip, [sp, #0x14]
0061f68c: ldr      r3, [sp, #0x18]
0061f690: add      lr, lr, #0x80000000
0061f694: add      ip, ip, #0x80000000
0061f698: add      r3, r3, #0x80000000
0061f69c: mov      r1, r6
0061f6a0: mov      r2, r7
0061f6a4: mov      r0, sp
0061f6a8: str      r3, [sp, #0x18]
0061f6ac: str      lr, [sp, #0x10]
0061f6b0: str      ip, [sp, #0x14]
0061f6b4: bl       #0x60dd34
0061f6b8: ldr      r1, [sp, #4]
0061f6bc: ldr      r3, [sp, #8]
0061f6c0: ldr      r2, [sp, #0xc]
0061f6c4: ldr      r0, [sp]
0061f6c8: str      r1, [sl, #4]
0061f6cc: str      r2, [sl, #0xc]
0061f6d0: str      r0, [sl]
0061f6d4: str      r3, [sl, #8]
0061f6d8: add      sp, sp, #0x54
0061f6dc: pop      {r4, r5, r6, r7, r8, sl, pc}

# _ZNK6glitch7collada18SAnimationAccessor15hasDefaultValueEv
00669e54: ldr      r3, [r0]
00669e58: ldr      r0, [r3, #0x18]
00669e5c: subs     r0, r0, #0
00669e60: movne    r0, #1
00669e64: bx       lr

# _ZN6glitch7collada15animation_track27CInterpreterQuaternionAngleINS1_30CSceneNodeQuaternionAngleMixinIfEEfE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061f594: push     {r4, r5, lr}
0061f598: sub      sp, sp, #0x14
0061f59c: mov      r3, #0
0061f5a0: mov      r5, r2
0061f5a4: mov      r2, sp
0061f5a8: str      r3, [sp, #8]
0061f5ac: str      r3, [sp]
0061f5b0: str      r3, [sp, #4]
0061f5b4: bl       #0x61f51c
0061f5b8: mov      r0, r5
0061f5bc: mov      r2, sp
0061f5c0: ldr      r1, [sp, #0xc]
0061f5c4: mov      r4, sp
0061f5c8: bl       #0x60cdbc
0061f5cc: add      sp, sp, #0x14
0061f5d0: pop      {r4, r5, pc}
