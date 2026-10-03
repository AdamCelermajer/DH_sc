
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

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE11getInstanceEv
00610988: push     {r4, r5, r6, lr}
0061098c: ldr      r4, [pc, #0x70]
00610990: ldr      r3, [pc, #0x70]
00610994: add      r4, pc, r4
00610998: ldr      r6, [r4, r3]
0061099c: ldr      r3, [r6]
006109a0: tst      r3, #1
006109a4: beq      #0x6109b4
006109a8: ldr      r5, [pc, #0x5c]
006109ac: ldr      r0, [r4, r5]
006109b0: pop      {r4, r5, r6, pc}
006109b4: mov      r0, r6
006109b8: bl       #0x30e76c
006109bc: cmp      r0, #0
006109c0: beq      #0x6109a8
006109c4: ldr      r3, [pc, #0x44]
006109c8: ldr      r5, [pc, #0x3c]
006109cc: mov      r0, r6
006109d0: ldr      r3, [r4, r3]
006109d4: ldr      r6, [r4, r5]
006109d8: add      r3, r3, #8
006109dc: str      r3, [r6]
006109e0: bl       #0x30ea3c
006109e4: ldr      r3, [pc, #0x28]
006109e8: mov      r0, r6
006109ec: ldr      r1, [r4, r3]
006109f0: ldr      r3, [pc, #0x20]
006109f4: ldr      r2, [r4, r3]
006109f8: bl       #0x30e304
006109fc: ldr      r0, [r4, r5]
00610a00: pop      {r4, r5, r6, pc}
00610a04: ldrshteq r4, [r8], -ip
00610a08: andeq    r3, r0, r0, asr fp
00610a0c: andeq    r1, r0, r8, ror #28
00610a10: andeq    r3, r0, ip, asr #23
00610a14: ldrdeq   r2, r3, [r0], -r0
00610a18: muleq    r0, r0, r8

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

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core10quaternionENS1_25CSceneNodeQuaternionMixinIfEEEEE11getInstanceEv
0060ff20: push     {r4, r5, r6, lr}
0060ff24: ldr      r4, [pc, #0x70]
0060ff28: ldr      r3, [pc, #0x70]
0060ff2c: add      r4, pc, r4
0060ff30: ldr      r6, [r4, r3]
0060ff34: ldr      r3, [r6]
0060ff38: tst      r3, #1
0060ff3c: beq      #0x60ff4c
0060ff40: ldr      r5, [pc, #0x5c]
0060ff44: ldr      r0, [r4, r5]
0060ff48: pop      {r4, r5, r6, pc}
0060ff4c: mov      r0, r6
0060ff50: bl       #0x30e76c
0060ff54: cmp      r0, #0
0060ff58: beq      #0x60ff40
0060ff5c: ldr      r3, [pc, #0x44]
0060ff60: ldr      r5, [pc, #0x3c]
0060ff64: mov      r0, r6
0060ff68: ldr      r3, [r4, r3]
0060ff6c: ldr      r6, [r4, r5]
0060ff70: add      r3, r3, #8
0060ff74: str      r3, [r6]
0060ff78: bl       #0x30ea3c
0060ff7c: ldr      r3, [pc, #0x28]
0060ff80: mov      r0, r6
0060ff84: ldr      r1, [r4, r3]
0060ff88: ldr      r3, [pc, #0x20]
0060ff8c: ldr      r2, [r4, r3]
0060ff90: bl       #0x30e304
0060ff94: ldr      r0, [r4, r5]
0060ff98: pop      {r4, r5, r6, pc}
0060ff9c: eorseq   r4, r8, r4, ror #22
0060ffa0: andeq    r4, r0, ip, lsl #11
0060ffa4: andeq    r3, r0, r8, lsl r2
0060ffa8: andeq    r1, r0, r8, asr #4
0060ffac: andeq    r4, r0, r0, lsr r6
0060ffb0: muleq    r0, r0, r8

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

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE10applyValueEPvSB_PNS1_15CApplicatorInfoE
0062393c: push     {r4, lr}
00623940: mov      r0, r2
00623944: ldr      r3, [r2]
00623948: mov      lr, pc
0062394c: ldr      pc, [r3, #0x94]
00623950: pop      {r4, pc}

# _ZNK6glitch7collada18SAnimationAccessor9getOutputEi
00669e24: ldm      r0, {r2, r3}
00669e28: mov      r0, #0x1c
00669e2c: ldr      r2, [r2, #8]
00669e30: mla      r2, r0, r1, r2
00669e34: ldr      r2, [r2, #0x18]
00669e38: add      r3, r3, r2, lsl #3
00669e3c: add      r0, r3, #4
00669e40: bx       lr

# _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIfLi3ENS1_18SSceneNodePositionELin1EfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
006286cc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
006286d0: mov      r7, r1
006286d4: sub      sp, sp, #8
006286d8: mov      r1, #0
006286dc: mov      r4, r3
006286e0: ldr      r6, [sp, #0x28]
006286e4: bl       #0x669e24
006286e8: mov      r1, r4
006286ec: mov      r8, r0
006286f0: mov      r0, #0x3f800000
006286f4: bl       #0x30e3ac
006286f8: str      r4, [sp, #4]
006286fc: str      r0, [sp]
00628700: mov      r3, #0xc
00628704: mul      r7, r3, r7
00628708: ldr      r3, [r8, #4]
0062870c: mov      r5, r0
00628710: ldr      r1, [r3, r7]
00628714: add      r7, r3, r7
00628718: bl       #0x30ed6c
0062871c: mov      r1, #0
00628720: bl       #0x30eba4
00628724: ldr      r1, [r7, #4]
00628728: mov      r8, r0
0062872c: mov      r0, r5
00628730: bl       #0x30ed6c
00628734: mov      r1, #0
00628738: bl       #0x30eba4
0062873c: add      r7, r7, #4
00628740: ldr      r1, [r7, #4]
00628744: mov      sl, r0
00628748: mov      r0, r5
0062874c: bl       #0x30ed6c
00628750: mov      r1, #0
00628754: bl       #0x30eba4
00628758: add      r5, r7, #4
0062875c: add      sb, r5, #4
00628760: ldr      r1, [sb, #4]
00628764: mov      r7, r0
00628768: mov      r0, r4
0062876c: bl       #0x30ed6c
00628770: mov      r1, r0
00628774: mov      r0, sl
00628778: bl       #0x30eba4
0062877c: add      sb, sb, #4
00628780: ldr      r1, [sb, #4]
00628784: mov      sl, r0
00628788: mov      r0, r4
0062878c: bl       #0x30ed6c
00628790: mov      r1, r7
00628794: bl       #0x30eba4
00628798: ldr      r1, [r5, #4]
0062879c: mov      r7, r0
006287a0: mov      r0, r4
006287a4: bl       #0x30ed6c
006287a8: mov      r1, r0
006287ac: mov      r0, r8
006287b0: bl       #0x30eba4
006287b4: mov      r3, r6
006287b8: str      r0, [r3], #4
006287bc: str      sl, [r6, #4]
006287c0: str      r7, [r3, #4]
006287c4: add      sp, sp, #8
006287c8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch4core10quaternion5slerpES1_S1_f
00612d00: sub      sp, sp, #0x10
00612d04: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00612d08: sub      sp, sp, #0x1c
00612d0c: add      ip, sp, #0x44
00612d10: stm      ip, {r1, r2, r3}
00612d14: ldr      fp, [sp, #0x54]
00612d18: ldr      sb, [sp, #0x44]
00612d1c: ldr      r3, [sp, #0x58]
00612d20: mov      r1, fp
00612d24: mov      r4, r0
00612d28: mov      r0, sb
00612d2c: str      r3, [sp, #0xc]
00612d30: bl       #0x30ed6c
00612d34: ldr      sl, [sp, #0x48]
00612d38: mov      r5, r0
00612d3c: ldr      r1, [sp, #0xc]
00612d40: mov      r0, sl
00612d44: bl       #0x30ed6c
00612d48: ldr      r3, [sp, #0x5c]
00612d4c: mov      r1, r0
00612d50: mov      r0, r5
00612d54: str      r3, [sp, #8]
00612d58: bl       #0x30eba4
00612d5c: ldr      r8, [sp, #0x4c]
00612d60: mov      r5, r0
00612d64: ldr      r1, [sp, #8]
00612d68: mov      r0, r8
00612d6c: bl       #0x30ed6c
00612d70: ldr      r3, [sp, #0x60]
00612d74: mov      r1, r0
00612d78: mov      r0, r5
00612d7c: str      r3, [sp, #4]
00612d80: bl       #0x30eba4
00612d84: ldr      r7, [sp, #0x50]
00612d88: mov      r5, r0
00612d8c: ldr      r1, [sp, #4]
00612d90: mov      r0, r7
00612d94: bl       #0x30ed6c
00612d98: mov      r1, r0
00612d9c: mov      r0, r5
00612da0: bl       #0x30eba4
00612da4: mov      r1, #0
00612da8: mov      r6, r0
00612dac: bl       #0x30e70c
00612db0: cmp      r0, #0
00612db4: addne    r6, r6, #0x80000000
00612db8: mov      r1, #0x3f800000
00612dbc: mov      r0, r6
00612dc0: addne    sb, sb, #0x80000000
00612dc4: addne    sl, sl, #0x80000000
00612dc8: addne    r8, r8, #0x80000000
00612dcc: addne    r7, r7, #0x80000000
00612dd0: bl       #0x30eba4
00612dd4: movw     r1, #0xcccd
00612dd8: movt     r1, #0x3d4c
00612ddc: bl       #0x30e2f8
00612de0: cmp      r0, #0
00612de4: ldr      r5, [sp, #0x64]
00612de8: beq      #0x612f10
00612dec: mov      r1, r6
00612df0: mov      r0, #0x3f800000
00612df4: bl       #0x30e3ac
00612df8: movw     r1, #0xcccd
00612dfc: movt     r1, #0x3d4c
00612e00: bl       #0x30e4b4
00612e04: cmp      r0, #0
00612e08: beq      #0x61300c
00612e0c: mov      r0, r6
00612e10: bl       #0x30e3dc
00612e14: str      r0, [sp, #0x10]
00612e18: bl       #0x30eb08
00612e1c: mov      r1, r0
00612e20: mov      r0, #0x3f800000
00612e24: bl       #0x30ec94
00612e28: mov      r1, r5
00612e2c: str      r0, [sp, #0x14]
00612e30: mov      r0, #0x3f800000
00612e34: bl       #0x30e3ac
00612e38: mov      r1, r0
00612e3c: ldr      r0, [sp, #0x10]
00612e40: bl       #0x30ed6c
00612e44: bl       #0x30eb08
00612e48: ldr      r1, [sp, #0x14]
00612e4c: bl       #0x30ed6c
00612e50: mov      r1, r5
00612e54: mov      r6, r0
00612e58: ldr      r0, [sp, #0x10]
00612e5c: bl       #0x30ed6c
00612e60: bl       #0x30eb08
00612e64: ldr      r1, [sp, #0x14]
00612e68: bl       #0x30ed6c
00612e6c: mov      r1, sb
00612e70: mov      r5, r0
00612e74: mov      r0, r6
00612e78: bl       #0x30ed6c
00612e7c: mov      r1, fp
00612e80: mov      sb, r0
00612e84: mov      r0, r5
00612e88: bl       #0x30ed6c
00612e8c: mov      r1, r0
00612e90: mov      r0, sb
00612e94: bl       #0x30eba4
00612e98: mov      r1, sl
00612e9c: str      r0, [r4]
00612ea0: mov      r0, r6
00612ea4: bl       #0x30ed6c
00612ea8: ldr      r1, [sp, #0xc]
00612eac: mov      sl, r0
00612eb0: mov      r0, r5
00612eb4: bl       #0x30ed6c
00612eb8: mov      r1, r0
00612ebc: mov      r0, sl
00612ec0: bl       #0x30eba4
00612ec4: mov      r1, r8
00612ec8: str      r0, [r4, #4]
00612ecc: mov      r0, r6
00612ed0: bl       #0x30ed6c
00612ed4: ldr      r1, [sp, #8]
00612ed8: mov      r8, r0
00612edc: mov      r0, r5
00612ee0: bl       #0x30ed6c
00612ee4: mov      r1, r0
00612ee8: mov      r0, r8
00612eec: bl       #0x30eba4
00612ef0: mov      r1, r7
00612ef4: str      r0, [r4, #8]
00612ef8: mov      r0, r6
00612efc: bl       #0x30ed6c
00612f00: ldr      r1, [sp, #4]
00612f04: mov      r6, r0
00612f08: mov      r0, r5
00612f0c: b        #0x612fe4
00612f10: mov      r1, r5
00612f14: mov      r0, #0x3f000000
00612f18: bl       #0x30e3ac
00612f1c: movw     r1, #0xfdb
00612f20: movt     r1, #0x4049
00612f24: bl       #0x30ed6c
00612f28: bl       #0x30eb08
00612f2c: movw     r1, #0xfdb
00612f30: mov      r6, r0
00612f34: movt     r1, #0x4049
00612f38: mov      r0, r5
00612f3c: bl       #0x30ed6c
00612f40: bl       #0x30eb08
00612f44: mov      r1, sb
00612f48: mov      r5, r0
00612f4c: mov      r0, r6
00612f50: bl       #0x30ed6c
00612f54: mov      r1, r5
00612f58: mov      fp, r0
00612f5c: add      r0, sl, #0x80000000
00612f60: bl       #0x30ed6c
00612f64: mov      r1, r0
00612f68: mov      r0, fp
00612f6c: bl       #0x30eba4
00612f70: mov      r1, sl
00612f74: str      r0, [r4]
00612f78: mov      r0, r6
00612f7c: bl       #0x30ed6c
00612f80: mov      r1, sb
00612f84: mov      sl, r0
00612f88: mov      r0, r5
00612f8c: bl       #0x30ed6c
00612f90: mov      r1, r0
00612f94: mov      r0, sl
00612f98: bl       #0x30eba4
00612f9c: mov      r1, r8
00612fa0: str      r0, [r4, #4]
00612fa4: mov      r0, r6
00612fa8: bl       #0x30ed6c
00612fac: mov      r1, r5
00612fb0: mov      sl, r0
00612fb4: add      r0, r7, #0x80000000
00612fb8: bl       #0x30ed6c
00612fbc: mov      r1, r0
00612fc0: mov      r0, sl
00612fc4: bl       #0x30eba4
00612fc8: mov      r1, r7
00612fcc: str      r0, [r4, #8]
00612fd0: mov      r0, r6
00612fd4: bl       #0x30ed6c
00612fd8: mov      r1, r8
00612fdc: mov      r6, r0
00612fe0: mov      r0, r5
00612fe4: bl       #0x30ed6c
00612fe8: mov      r1, r0
00612fec: mov      r0, r6
00612ff0: bl       #0x30eba4
00612ff4: str      r0, [r4, #0xc]
00612ff8: mov      r0, r4
00612ffc: add      sp, sp, #0x1c
00613000: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00613004: add      sp, sp, #0x10
00613008: bx       lr
0061300c: mov      r1, r5
00613010: mov      r0, #0x3f800000
00613014: bl       #0x30e3ac
00613018: mov      r1, sb
0061301c: mov      r6, r0
00613020: bl       #0x30ed6c
00613024: mov      r1, fp
00613028: mov      sb, r0
0061302c: mov      r0, r5
00613030: bl       #0x30ed6c
00613034: mov      r1, r0
00613038: mov      r0, sb
0061303c: bl       #0x30eba4
00613040: mov      r1, sl
00613044: str      r0, [r4]
00613048: mov      r0, r6
0061304c: bl       #0x30ed6c
00613050: ldr      r1, [sp, #0xc]
00613054: mov      sl, r0
00613058: mov      r0, r5
0061305c: bl       #0x30ed6c
00613060: mov      r1, r0
00613064: mov      r0, sl
00613068: bl       #0x30eba4
0061306c: mov      r1, r8
00613070: str      r0, [r4, #4]
00613074: mov      r0, r6
00613078: bl       #0x30ed6c
0061307c: ldr      r1, [sp, #8]
00613080: mov      r8, r0
00613084: mov      r0, r5
00613088: bl       #0x30ed6c
0061308c: mov      r1, r0
00613090: mov      r0, r8
00613094: bl       #0x30eba4
00613098: mov      r1, r7
0061309c: str      r0, [r4, #8]
006130a0: mov      r0, r6
006130a4: bl       #0x30ed6c
006130a8: ldr      r1, [sp, #4]
006130ac: mov      r6, r0
006130b0: mov      r0, r5
006130b4: bl       #0x30ed6c
006130b8: mov      r1, r0
006130bc: mov      r0, r6
006130c0: bl       #0x30eba4
006130c4: str      r0, [r4, #0xc]
006130c8: mov      r0, r4
006130cc: bl       #0x35c8f0
006130d0: b        #0x612ff8

# _ZN6glitch7collada15animation_track8CBlenderINS_4core10quaternionELi1ES4_E17getBlendedValueExEPvPfiS6_
006130d4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006130d8: mov      ip, #0
006130dc: sub      sp, sp, #0x3c
006130e0: subs     r6, r2, #0
006130e4: mov      r2, #0x3f800000
006130e8: mov      r8, r0
006130ec: str      r2, [sp, #0x34]
006130f0: mov      r5, r1
006130f4: mov      sb, r3
006130f8: str      ip, [sp, #0x28]
006130fc: str      ip, [sp, #0x2c]
00613100: str      ip, [sp, #0x30]
00613104: ble      #0x61328c
00613108: mov      r1, ip
0061310c: ldr      r0, [r5]
00613110: bl       #0x30df8c
00613114: cmp      r0, #0
00613118: moveq    r3, #0
0061311c: moveq    r7, r5
00613120: moveq    r4, r3
00613124: beq      #0x613224
00613128: mov      r7, r5
0061312c: mov      r4, #0
00613130: b        #0x613144
00613134: ldr      r0, [r7, #4]!
00613138: bl       #0x30df8c
0061313c: cmp      r0, #0
00613140: beq      #0x613220
00613144: add      r4, r4, #1
00613148: cmp      r4, r6
0061314c: mov      r1, #0
00613150: bne      #0x613134
00613154: add      r4, r6, #1
00613158: mov      sl, #0
0061315c: cmp      r6, r4
00613160: ble      #0x6131f8
00613164: add      r3, sp, #4
00613168: add      r5, r5, r4, lsl #2
0061316c: add      r8, r8, r4, lsl #4
00613170: add      fp, sp, #0x28
00613174: str      r3, [sp, #0x24]
00613178: b        #0x61318c
0061317c: cmp      r4, r6
00613180: add      r5, r5, #4
00613184: add      r8, r8, #0x10
00613188: beq      #0x6131f8
0061318c: ldr      r7, [r5]
00613190: mov      r1, #0
00613194: add      r4, r4, #1
00613198: mov      r0, r7
0061319c: bl       #0x30df8c
006131a0: cmp      r0, #0
006131a4: bne      #0x61317c
006131a8: mov      r0, sl
006131ac: mov      r1, r7
006131b0: bl       #0x30eba4
006131b4: ldr      ip, [sp, #0x24]
006131b8: mov      sl, r0
006131bc: ldm      r8, {r0, r1, r2, r3}
006131c0: stm      ip, {r0, r1, r2, r3}
006131c4: mov      r1, sl
006131c8: mov      r0, r7
006131cc: bl       #0x30ec94
006131d0: ldm      fp, {r1, r2, r3}
006131d4: ldr      ip, [sp, #0x34]
006131d8: str      r0, [sp, #0x14]
006131dc: mov      r0, fp
006131e0: str      ip, [sp]
006131e4: bl       #0x612d00
006131e8: cmp      r4, r6
006131ec: add      r5, r5, #4
006131f0: add      r8, r8, #0x10
006131f4: bne      #0x61318c
006131f8: ldr      r1, [sp, #0x2c]
006131fc: ldr      r3, [sp, #0x30]
00613200: ldr      r2, [sp, #0x34]
00613204: ldr      r0, [sp, #0x28]
00613208: str      r1, [sb, #4]
0061320c: str      r2, [sb, #0xc]
00613210: str      r0, [sb]
00613214: str      r3, [sb, #8]
00613218: add      sp, sp, #0x3c
0061321c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00613220: lsl      r3, r4, #4
00613224: ldr      sl, [r7]
00613228: add      r2, r8, r3
0061322c: ldr      r7, [r8, r3]
00613230: ldr      fp, [r2, #0xc]
00613234: ldr      r3, [r2, #4]
00613238: ldr      r2, [r2, #8]
0061323c: mov      r0, sl
00613240: mov      r1, #0x3f800000
00613244: str      r3, [sp, #0x2c]
00613248: str      r2, [sp, #0x30]
0061324c: str      r2, [sp, #0x1c]
00613250: str      r3, [sp, #0x20]
00613254: str      r7, [sp, #0x28]
00613258: str      fp, [sp, #0x34]
0061325c: bl       #0x30df8c
00613260: cmp      r0, #0
00613264: ldr      r2, [sp, #0x1c]
00613268: ldr      r3, [sp, #0x20]
0061326c: beq      #0x613284
00613270: str      fp, [sb, #0xc]
00613274: str      r7, [sb]
00613278: str      r3, [sb, #4]
0061327c: str      r2, [sb, #8]
00613280: b        #0x613218
00613284: add      r4, r4, #1
00613288: b        #0x61315c
0061328c: mov      r4, #1
00613290: b        #0x613158

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE16getKeyBasedValueERKNS0_18SAnimationAccessorEiifPv
00628834: mov      r0, r1
00628838: ldr      ip, [sp, #4]
0062883c: mov      r1, r2
00628840: mov      r2, r3
00628844: ldr      r3, [sp]
00628848: str      ip, [sp]
0062884c: b        #0x6286cc
