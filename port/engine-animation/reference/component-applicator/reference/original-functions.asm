
# _ZN6glitch5scene10ISceneNode8setScaleERKNS_4core8vector3dIfEE
005970c4: ldr      r3, [r1]
005970c8: ldr      r2, [r0, #0x11c]
005970cc: str      r3, [r0, #0xc8]
005970d0: ldr      r3, [r1, #4]
005970d4: orr      r2, r2, #2
005970d8: str      r3, [r0, #0xcc]
005970dc: ldr      r3, [r1, #8]
005970e0: str      r2, [r0, #0x11c]
005970e4: str      r3, [r0, #0xd0]
005970e8: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE12getValueSizeEv
0060ef1c: mov      r0, #0xc
0060ef20: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE15getBlendedValueEPvPfiSD_
006270a4: cmp      r3, #1
006270a8: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
006270ac: mov      r4, r3
006270b0: mov      fp, r2
006270b4: beq      #0x627160
006270b8: cmp      r3, #0
006270bc: moveq    r8, #0
006270c0: moveq    sb, r8
006270c4: moveq    sl, r8
006270c8: beq      #0x627148
006270cc: mov      r8, #0
006270d0: mov      r5, r1
006270d4: mov      r7, #0
006270d8: mov      sb, r8
006270dc: mov      sl, r8
006270e0: ldr      r6, [fp, r7]
006270e4: ldr      r1, [r5]
006270e8: add      r7, r7, #4
006270ec: mov      r0, r6
006270f0: bl       #0x30ed6c
006270f4: mov      r1, r0
006270f8: mov      r0, r8
006270fc: bl       #0x30eba4
00627100: ldr      r1, [r5, #4]
00627104: mov      r8, r0
00627108: mov      r0, r6
0062710c: bl       #0x30ed6c
00627110: mov      r1, r0
00627114: mov      r0, sb
00627118: bl       #0x30eba4
0062711c: ldr      r1, [r5, #8]
00627120: mov      sb, r0
00627124: mov      r0, r6
00627128: bl       #0x30ed6c
0062712c: mov      r1, r0
00627130: mov      r0, sl
00627134: bl       #0x30eba4
00627138: subs     r4, r4, #1
0062713c: mov      sl, r0
00627140: add      r5, r5, #0xc
00627144: bne      #0x6270e0
00627148: ldr      r3, [sp, #0x28]
0062714c: str      r8, [r3], #4
00627150: ldr      r2, [sp, #0x28]
00627154: str      sb, [r2, #4]
00627158: str      sl, [r3, #4]
0062715c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00627160: mov      r2, r1
00627164: ldr      r0, [r2], #4
00627168: ldr      r3, [sp, #0x28]
0062716c: str      r0, [r3], #4
00627170: ldr      r1, [r1, #4]
00627174: ldr      r0, [sp, #0x28]
00627178: str      r1, [r0, #4]
0062717c: ldr      r2, [r2, #4]
00627180: str      r2, [r3, #4]
00627184: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
0062c944: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062c948: cmp      r2, #1
0062c94c: sub      sp, sp, #0x1c
0062c950: mov      sl, #0
0062c954: mov      r4, r2
0062c958: mov      r5, r1
0062c95c: str      r3, [sp, #4]
0062c960: str      sl, [sp, #0x14]
0062c964: beq      #0x62ca18
0062c968: cmp      r2, #0
0062c96c: moveq    fp, sl
0062c970: moveq    sb, sl
0062c974: beq      #0x62c9f0
0062c978: mov      r6, r0
0062c97c: mov      r8, #0
0062c980: mov      fp, sl
0062c984: mov      sb, sl
0062c988: ldr      r7, [r5, r8]
0062c98c: ldr      r1, [r6]
0062c990: add      r8, r8, #4
0062c994: mov      r0, r7
0062c998: bl       #0x30ed6c
0062c99c: mov      r1, r0
0062c9a0: mov      r0, sl
0062c9a4: bl       #0x30eba4
0062c9a8: ldr      r1, [r6, #4]
0062c9ac: mov      sl, r0
0062c9b0: mov      r0, r7
0062c9b4: bl       #0x30ed6c
0062c9b8: mov      r1, r0
0062c9bc: mov      r0, fp
0062c9c0: bl       #0x30eba4
0062c9c4: ldr      r1, [r6, #8]
0062c9c8: mov      fp, r0
0062c9cc: mov      r0, r7
0062c9d0: bl       #0x30ed6c
0062c9d4: mov      r1, r0
0062c9d8: mov      r0, sb
0062c9dc: bl       #0x30eba4
0062c9e0: subs     r4, r4, #1
0062c9e4: mov      sb, r0
0062c9e8: add      r6, r6, #0xc
0062c9ec: bne      #0x62c988
0062c9f0: add      r1, sp, #0x18
0062c9f4: str      sl, [r1, #-0xc]!
0062c9f8: str      fp, [sp, #0x10]
0062c9fc: str      sb, [r1, #8]
0062ca00: ldr      r0, [sp, #4]
0062ca04: ldr      r3, [r0]
0062ca08: mov      lr, pc
0062ca0c: ldr      pc, [r3, #0x94]
0062ca10: add      sp, sp, #0x1c
0062ca14: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062ca18: mov      r3, r0
0062ca1c: ldr      ip, [r3], #4
0062ca20: ldr      r2, [r0, #4]
0062ca24: add      r1, sp, #0x18
0062ca28: ldr      r3, [r3, #4]
0062ca2c: str      ip, [r1, #-0xc]!
0062ca30: str      r2, [sp, #0x10]
0062ca34: str      r3, [r1, #8]
0062ca38: b        #0x62ca00

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
0062d0b4: mov      r0, r1
0062d0b8: ldr      ip, [sp, #4]
0062d0bc: mov      r1, r2
0062d0c0: mov      r2, r3
0062d0c4: ldr      r3, [sp]
0062d0c8: str      ip, [sp]
0062d0cc: b        #0x62cfbc

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE11getInstanceEv
00610ebc: push     {r4, r5, r6, lr}
00610ec0: ldr      r4, [pc, #0x70]
00610ec4: ldr      r3, [pc, #0x70]
00610ec8: add      r4, pc, r4
00610ecc: ldr      r6, [r4, r3]
00610ed0: ldr      r3, [r6]
00610ed4: tst      r3, #1
00610ed8: beq      #0x610ee8
00610edc: ldr      r5, [pc, #0x5c]
00610ee0: ldr      r0, [r4, r5]
00610ee4: pop      {r4, r5, r6, pc}
00610ee8: mov      r0, r6
00610eec: bl       #0x30e76c
00610ef0: cmp      r0, #0
00610ef4: beq      #0x610edc
00610ef8: ldr      r3, [pc, #0x44]
00610efc: ldr      r5, [pc, #0x3c]
00610f00: mov      r0, r6
00610f04: ldr      r3, [r4, r3]
00610f08: ldr      r6, [r4, r5]
00610f0c: add      r3, r3, #8
00610f10: str      r3, [r6]
00610f14: bl       #0x30ea3c
00610f18: ldr      r3, [pc, #0x28]
00610f1c: mov      r0, r6
00610f20: ldr      r1, [r4, r3]
00610f24: ldr      r3, [pc, #0x20]
00610f28: ldr      r2, [r4, r3]
00610f2c: bl       #0x30e304
00610f30: ldr      r0, [r4, r5]
00610f34: pop      {r4, r5, r6, pc}
00610f38: eorseq   r3, r8, r8, asr #23
00610f3c: andeq    r2, r0, ip, ror #9
00610f40: strheq   r4, [r0], -r8
00610f44: andeq    r1, r0, r8, lsr #29
00610f48: strdeq   r1, r2, [r0], -r8
00610f4c: muleq    r0, r0, r8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00623d90: push     {r4, lr}
00623d94: mov      r0, r2
00623d98: ldr      r3, [r2]
00623d9c: mov      lr, pc
00623da0: ldr      pc, [r3, #0xa4]
00623da4: pop      {r4, pc}

# _ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE
0059712c: ldr      r3, [r1]
00597130: ldr      r2, [r0, #0x11c]
00597134: str      r3, [r0, #0xac]
00597138: ldr      r3, [r1, #4]
0059713c: orr      r2, r2, #8
00597140: str      r3, [r0, #0xb0]
00597144: ldr      r3, [r1, #8]
00597148: str      r2, [r0, #0x11c]
0059714c: str      r3, [r0, #0xb4]
00597150: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
00628e24: mov      r0, r1
00628e28: ldr      ip, [sp, #4]
00628e2c: mov      r1, r2
00628e30: mov      r2, r3
00628e34: ldr      r3, [sp]
00628e38: str      ip, [sp]
00628e3c: b        #0x628d2c

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00622c40: push     {r4, lr}
00622c44: mov      r0, r2
00622c48: ldr      r3, [r2]
00622c4c: mov      lr, pc
00622c50: ldr      pc, [r3, #0x94]
00622c54: pop      {r4, pc}

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
00628d2c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00628d30: cmp      r2, #1
00628d34: sub      sp, sp, #0x1c
00628d38: mov      sl, #0
00628d3c: mov      r4, r2
00628d40: mov      r5, r1
00628d44: str      r3, [sp, #4]
00628d48: str      sl, [sp, #0x14]
00628d4c: beq      #0x628e00
00628d50: cmp      r2, #0
00628d54: moveq    fp, sl
00628d58: moveq    sb, sl
00628d5c: beq      #0x628dd8
00628d60: mov      r6, r0
00628d64: mov      r8, #0
00628d68: mov      fp, sl
00628d6c: mov      sb, sl
00628d70: ldr      r7, [r5, r8]
00628d74: ldr      r1, [r6]
00628d78: add      r8, r8, #4
00628d7c: mov      r0, r7
00628d80: bl       #0x30ed6c
00628d84: mov      r1, r0
00628d88: mov      r0, sl
00628d8c: bl       #0x30eba4
00628d90: ldr      r1, [r6, #4]
00628d94: mov      sl, r0
00628d98: mov      r0, r7
00628d9c: bl       #0x30ed6c
00628da0: mov      r1, r0
00628da4: mov      r0, fp
00628da8: bl       #0x30eba4
00628dac: ldr      r1, [r6, #8]
00628db0: mov      fp, r0
00628db4: mov      r0, r7
00628db8: bl       #0x30ed6c
00628dbc: mov      r1, r0
00628dc0: mov      r0, sb
00628dc4: bl       #0x30eba4
00628dc8: subs     r4, r4, #1
00628dcc: mov      sb, r0
00628dd0: add      r6, r6, #0xc
00628dd4: bne      #0x628d70
00628dd8: add      r1, sp, #0x18
00628ddc: str      sl, [r1, #-0xc]!
00628de0: str      fp, [sp, #0x10]
00628de4: str      sb, [r1, #8]
00628de8: ldr      r0, [sp, #4]
00628dec: ldr      r3, [r0]
00628df0: mov      lr, pc
00628df4: ldr      pc, [r3, #0xa4]
00628df8: add      sp, sp, #0x1c
00628dfc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00628e00: mov      r3, r0
00628e04: ldr      ip, [r3], #4
00628e08: ldr      r2, [r0, #4]
00628e0c: add      r1, sp, #0x18
00628e10: ldr      r3, [r3, #4]
00628e14: str      ip, [r1, #-0xc]!
00628e18: str      r2, [sp, #0x10]
00628e1c: str      r3, [r1, #8]
00628e20: b        #0x628de8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00622cb8: push     {r4, lr}
00622cbc: mov      r0, r2
00622cc0: ldr      r3, [r2]
00622cc4: mov      lr, pc
00622cc8: ldr      pc, [r3, #0xa4]
00622ccc: pop      {r4, pc}

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
0062c2cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062c2d0: cmp      r2, #1
0062c2d4: sub      sp, sp, #0x1c
0062c2d8: mov      sl, #0
0062c2dc: mov      r4, r2
0062c2e0: mov      r5, r1
0062c2e4: str      r3, [sp, #4]
0062c2e8: str      sl, [sp, #0x14]
0062c2ec: beq      #0x62c3a0
0062c2f0: cmp      r2, #0
0062c2f4: moveq    fp, sl
0062c2f8: moveq    sb, sl
0062c2fc: beq      #0x62c378
0062c300: mov      r6, r0
0062c304: mov      r8, #0
0062c308: mov      fp, sl
0062c30c: mov      sb, sl
0062c310: ldr      r7, [r5, r8]
0062c314: ldr      r1, [r6]
0062c318: add      r8, r8, #4
0062c31c: mov      r0, r7
0062c320: bl       #0x30ed6c
0062c324: mov      r1, r0
0062c328: mov      r0, sl
0062c32c: bl       #0x30eba4
0062c330: ldr      r1, [r6, #4]
0062c334: mov      sl, r0
0062c338: mov      r0, r7
0062c33c: bl       #0x30ed6c
0062c340: mov      r1, r0
0062c344: mov      r0, fp
0062c348: bl       #0x30eba4
0062c34c: ldr      r1, [r6, #8]
0062c350: mov      fp, r0
0062c354: mov      r0, r7
0062c358: bl       #0x30ed6c
0062c35c: mov      r1, r0
0062c360: mov      r0, sb
0062c364: bl       #0x30eba4
0062c368: subs     r4, r4, #1
0062c36c: mov      sb, r0
0062c370: add      r6, r6, #0xc
0062c374: bne      #0x62c310
0062c378: add      r1, sp, #0x18
0062c37c: str      sl, [r1, #-0xc]!
0062c380: str      fp, [sp, #0x10]
0062c384: str      sb, [r1, #8]
0062c388: ldr      r0, [sp, #4]
0062c38c: ldr      r3, [r0]
0062c390: mov      lr, pc
0062c394: ldr      pc, [r3, #0x94]
0062c398: add      sp, sp, #0x1c
0062c39c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062c3a0: mov      r3, r0
0062c3a4: ldr      ip, [r3], #4
0062c3a8: ldr      r2, [r0, #4]
0062c3ac: add      r1, sp, #0x18
0062c3b0: ldr      r3, [r3, #4]
0062c3b4: str      ip, [r1, #-0xc]!
0062c3b8: str      r2, [sp, #0x10]
0062c3bc: str      r3, [r1, #8]
0062c3c0: b        #0x62c388

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE12getValueSizeEv
0060efe0: mov      r0, #0xc
0060efe4: bx       lr

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE11getInstanceEv
00610d00: push     {r4, r5, r6, lr}
00610d04: ldr      r4, [pc, #0x70]
00610d08: ldr      r3, [pc, #0x70]
00610d0c: add      r4, pc, r4
00610d10: ldr      r6, [r4, r3]
00610d14: ldr      r3, [r6]
00610d18: tst      r3, #1
00610d1c: beq      #0x610d2c
00610d20: ldr      r5, [pc, #0x5c]
00610d24: ldr      r0, [r4, r5]
00610d28: pop      {r4, r5, r6, pc}
00610d2c: mov      r0, r6
00610d30: bl       #0x30e76c
00610d34: cmp      r0, #0
00610d38: beq      #0x610d20
00610d3c: ldr      r3, [pc, #0x44]
00610d40: ldr      r5, [pc, #0x3c]
00610d44: mov      r0, r6
00610d48: ldr      r3, [r4, r3]
00610d4c: ldr      r6, [r4, r5]
00610d50: add      r3, r3, #8
00610d54: str      r3, [r6]
00610d58: bl       #0x30ea3c
00610d5c: ldr      r3, [pc, #0x28]
00610d60: mov      r0, r6
00610d64: ldr      r1, [r4, r3]
00610d68: ldr      r3, [pc, #0x20]
00610d6c: ldr      r2, [r4, r3]
00610d70: bl       #0x30e304
00610d74: ldr      r0, [r4, r5]
00610d78: pop      {r4, r5, r6, pc}
00610d7c: eorseq   r3, r8, r4, lsl #27
00610d80: muleq    r0, r8, sb
00610d84: andeq    r1, r0, r4, lsr #6
00610d88: andeq    r4, r0, ip, lsr #14
00610d8c: andeq    r1, r0, r0, asr #22
00610d90: muleq    r0, r0, r8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00623d48: push     {r4, lr}
00623d4c: mov      r0, r2
00623d50: ldr      r3, [r2]
00623d54: mov      lr, pc
00623d58: ldr      pc, [r3, #0xa4]
00623d5c: pop      {r4, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE12getValueSizeEv
0060ef34: mov      r0, #0xc
0060ef38: bx       lr

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

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE11getInstanceEv
00610610: push     {r4, r5, r6, lr}
00610614: ldr      r4, [pc, #0x70]
00610618: ldr      r3, [pc, #0x70]
0061061c: add      r4, pc, r4
00610620: ldr      r6, [r4, r3]
00610624: ldr      r3, [r6]
00610628: tst      r3, #1
0061062c: beq      #0x61063c
00610630: ldr      r5, [pc, #0x5c]
00610634: ldr      r0, [r4, r5]
00610638: pop      {r4, r5, r6, pc}
0061063c: mov      r0, r6
00610640: bl       #0x30e76c
00610644: cmp      r0, #0
00610648: beq      #0x610630
0061064c: ldr      r3, [pc, #0x44]
00610650: ldr      r5, [pc, #0x3c]
00610654: mov      r0, r6
00610658: ldr      r3, [r4, r3]
0061065c: ldr      r6, [r4, r5]
00610660: add      r3, r3, #8
00610664: str      r3, [r6]
00610668: bl       #0x30ea3c
0061066c: ldr      r3, [pc, #0x28]
00610670: mov      r0, r6
00610674: ldr      r1, [r4, r3]
00610678: ldr      r3, [pc, #0x20]
0061067c: ldr      r2, [r4, r3]
00610680: bl       #0x30e304
00610684: ldr      r0, [r4, r5]
00610688: pop      {r4, r5, r6, pc}
0061068c: eorseq   r4, r8, r4, ror r4
00610690: muleq    r0, ip, lr
00610694: andeq    r3, r0, r0, asr #4
00610698: andeq    r4, r0, r8, lsr #16
0061069c: andeq    r1, r0, ip, ror r0
006106a0: muleq    r0, r0, r8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
0062c3c4: mov      r0, r1
0062c3c8: ldr      ip, [sp, #4]
0062c3cc: mov      r1, r2
0062c3d0: mov      r2, r3
0062c3d4: ldr      r3, [sp]
0062c3d8: str      ip, [sp]
0062c3dc: b        #0x62c2cc

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

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE11getInstanceEv
00610b44: push     {r4, r5, r6, lr}
00610b48: ldr      r4, [pc, #0x70]
00610b4c: ldr      r3, [pc, #0x70]
00610b50: add      r4, pc, r4
00610b54: ldr      r6, [r4, r3]
00610b58: ldr      r3, [r6]
00610b5c: tst      r3, #1
00610b60: beq      #0x610b70
00610b64: ldr      r5, [pc, #0x5c]
00610b68: ldr      r0, [r4, r5]
00610b6c: pop      {r4, r5, r6, pc}
00610b70: mov      r0, r6
00610b74: bl       #0x30e76c
00610b78: cmp      r0, #0
00610b7c: beq      #0x610b64
00610b80: ldr      r3, [pc, #0x44]
00610b84: ldr      r5, [pc, #0x3c]
00610b88: mov      r0, r6
00610b8c: ldr      r3, [r4, r3]
00610b90: ldr      r6, [r4, r5]
00610b94: add      r3, r3, #8
00610b98: str      r3, [r6]
00610b9c: bl       #0x30ea3c
00610ba0: ldr      r3, [pc, #0x28]
00610ba4: mov      r0, r6
00610ba8: ldr      r1, [r4, r3]
00610bac: ldr      r3, [pc, #0x20]
00610bb0: ldr      r2, [r4, r3]
00610bb4: bl       #0x30e304
00610bb8: ldr      r0, [r4, r5]
00610bbc: pop      {r4, r5, r6, pc}
00610bc0: eorseq   r3, r8, r0, asr #30
00610bc4: andeq    r3, r0, r4, lsl #6
00610bc8: andeq    r3, r0, r4, asr #16
00610bcc: andeq    r1, r0, ip, ror #5
00610bd0: andeq    r3, r0, r0, asr #16
00610bd4: muleq    r0, r0, r8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
00629b14: mov      r0, r1
00629b18: ldr      ip, [sp, #4]
00629b1c: mov      r1, r2
00629b20: mov      r2, r3
00629b24: ldr      r3, [sp]
00629b28: str      ip, [sp]
00629b2c: b        #0x629a1c

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE15getBlendedValueEPvPfiSD_
0062a2e0: cmp      r3, #1
0062a2e4: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062a2e8: mov      r4, r3
0062a2ec: mov      fp, r2
0062a2f0: beq      #0x62a39c
0062a2f4: cmp      r3, #0
0062a2f8: moveq    r8, #0
0062a2fc: moveq    sb, r8
0062a300: moveq    sl, r8
0062a304: beq      #0x62a384
0062a308: mov      r8, #0
0062a30c: mov      r5, r1
0062a310: mov      r7, #0
0062a314: mov      sb, r8
0062a318: mov      sl, r8
0062a31c: ldr      r6, [fp, r7]
0062a320: ldr      r1, [r5]
0062a324: add      r7, r7, #4
0062a328: mov      r0, r6
0062a32c: bl       #0x30ed6c
0062a330: mov      r1, r0
0062a334: mov      r0, r8
0062a338: bl       #0x30eba4
0062a33c: ldr      r1, [r5, #4]
0062a340: mov      r8, r0
0062a344: mov      r0, r6
0062a348: bl       #0x30ed6c
0062a34c: mov      r1, r0
0062a350: mov      r0, sb
0062a354: bl       #0x30eba4
0062a358: ldr      r1, [r5, #8]
0062a35c: mov      sb, r0
0062a360: mov      r0, r6
0062a364: bl       #0x30ed6c
0062a368: mov      r1, r0
0062a36c: mov      r0, sl
0062a370: bl       #0x30eba4
0062a374: subs     r4, r4, #1
0062a378: mov      sl, r0
0062a37c: add      r5, r5, #0xc
0062a380: bne      #0x62a31c
0062a384: ldr      r3, [sp, #0x28]
0062a388: str      r8, [r3], #4
0062a38c: ldr      r2, [sp, #0x28]
0062a390: str      sb, [r2, #4]
0062a394: str      sl, [r3, #4]
0062a398: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062a39c: mov      r2, r1
0062a3a0: ldr      r0, [r2], #4
0062a3a4: ldr      r3, [sp, #0x28]
0062a3a8: str      r0, [r3], #4
0062a3ac: ldr      r1, [r1, #4]
0062a3b0: ldr      r0, [sp, #0x28]
0062a3b4: str      r1, [r0, #4]
0062a3b8: ldr      r2, [r2, #4]
0062a3bc: str      r2, [r3, #4]
0062a3c0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
00629a1c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00629a20: cmp      r2, #1
00629a24: sub      sp, sp, #0x1c
00629a28: mov      sl, #0
00629a2c: mov      r4, r2
00629a30: mov      r5, r1
00629a34: str      r3, [sp, #4]
00629a38: str      sl, [sp, #0x14]
00629a3c: beq      #0x629af0
00629a40: cmp      r2, #0
00629a44: moveq    fp, sl
00629a48: moveq    sb, sl
00629a4c: beq      #0x629ac8
00629a50: mov      r6, r0
00629a54: mov      r8, #0
00629a58: mov      fp, sl
00629a5c: mov      sb, sl
00629a60: ldr      r7, [r5, r8]
00629a64: ldr      r1, [r6]
00629a68: add      r8, r8, #4
00629a6c: mov      r0, r7
00629a70: bl       #0x30ed6c
00629a74: mov      r1, r0
00629a78: mov      r0, sl
00629a7c: bl       #0x30eba4
00629a80: ldr      r1, [r6, #4]
00629a84: mov      sl, r0
00629a88: mov      r0, r7
00629a8c: bl       #0x30ed6c
00629a90: mov      r1, r0
00629a94: mov      r0, fp
00629a98: bl       #0x30eba4
00629a9c: ldr      r1, [r6, #8]
00629aa0: mov      fp, r0
00629aa4: mov      r0, r7
00629aa8: bl       #0x30ed6c
00629aac: mov      r1, r0
00629ab0: mov      r0, sb
00629ab4: bl       #0x30eba4
00629ab8: subs     r4, r4, #1
00629abc: mov      sb, r0
00629ac0: add      r6, r6, #0xc
00629ac4: bne      #0x629a60
00629ac8: add      r1, sp, #0x18
00629acc: str      sl, [r1, #-0xc]!
00629ad0: str      fp, [sp, #0x10]
00629ad4: str      sb, [r1, #8]
00629ad8: ldr      r0, [sp, #4]
00629adc: ldr      r3, [r0]
00629ae0: mov      lr, pc
00629ae4: ldr      pc, [r3, #0xa4]
00629ae8: add      sp, sp, #0x1c
00629aec: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00629af0: mov      r3, r0
00629af4: ldr      ip, [r3], #4
00629af8: ldr      r2, [r0, #4]
00629afc: add      r1, sp, #0x18
00629b00: ldr      r3, [r3, #4]
00629b04: str      ip, [r1, #-0xc]!
00629b08: str      r2, [sp, #0x10]
00629b0c: str      r3, [r1, #8]
00629b10: b        #0x629ad8

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
0062cfbc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062cfc0: cmp      r2, #1
0062cfc4: sub      sp, sp, #0x1c
0062cfc8: mov      sl, #0
0062cfcc: mov      r4, r2
0062cfd0: mov      r5, r1
0062cfd4: str      r3, [sp, #4]
0062cfd8: str      sl, [sp, #0x14]
0062cfdc: beq      #0x62d090
0062cfe0: cmp      r2, #0
0062cfe4: moveq    fp, sl
0062cfe8: moveq    sb, sl
0062cfec: beq      #0x62d068
0062cff0: mov      r6, r0
0062cff4: mov      r8, #0
0062cff8: mov      fp, sl
0062cffc: mov      sb, sl
0062d000: ldr      r7, [r5, r8]
0062d004: ldr      r1, [r6]
0062d008: add      r8, r8, #4
0062d00c: mov      r0, r7
0062d010: bl       #0x30ed6c
0062d014: mov      r1, r0
0062d018: mov      r0, sl
0062d01c: bl       #0x30eba4
0062d020: ldr      r1, [r6, #4]
0062d024: mov      sl, r0
0062d028: mov      r0, r7
0062d02c: bl       #0x30ed6c
0062d030: mov      r1, r0
0062d034: mov      r0, fp
0062d038: bl       #0x30eba4
0062d03c: ldr      r1, [r6, #8]
0062d040: mov      fp, r0
0062d044: mov      r0, r7
0062d048: bl       #0x30ed6c
0062d04c: mov      r1, r0
0062d050: mov      r0, sb
0062d054: bl       #0x30eba4
0062d058: subs     r4, r4, #1
0062d05c: mov      sb, r0
0062d060: add      r6, r6, #0xc
0062d064: bne      #0x62d000
0062d068: add      r1, sp, #0x18
0062d06c: str      sl, [r1, #-0xc]!
0062d070: str      fp, [sp, #0x10]
0062d074: str      sb, [r1, #8]
0062d078: ldr      r0, [sp, #4]
0062d07c: ldr      r3, [r0]
0062d080: mov      lr, pc
0062d084: ldr      pc, [r3, #0x94]
0062d088: add      sp, sp, #0x1c
0062d08c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062d090: mov      r3, r0
0062d094: ldr      ip, [r3], #4
0062d098: ldr      r2, [r0, #4]
0062d09c: add      r1, sp, #0x18
0062d0a0: ldr      r3, [r3, #4]
0062d0a4: str      ip, [r1, #-0xc]!
0062d0a8: str      r2, [sp, #0x10]
0062d0ac: str      r3, [r1, #8]
0062d0b0: b        #0x62d078

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE15getBlendedValueEPvPfiSD_
006278a8: cmp      r3, #1
006278ac: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
006278b0: mov      r4, r3
006278b4: mov      fp, r2
006278b8: beq      #0x627964
006278bc: cmp      r3, #0
006278c0: moveq    r8, #0
006278c4: moveq    sb, r8
006278c8: moveq    sl, r8
006278cc: beq      #0x62794c
006278d0: mov      r8, #0
006278d4: mov      r5, r1
006278d8: mov      r7, #0
006278dc: mov      sb, r8
006278e0: mov      sl, r8
006278e4: ldr      r6, [fp, r7]
006278e8: ldr      r1, [r5]
006278ec: add      r7, r7, #4
006278f0: mov      r0, r6
006278f4: bl       #0x30ed6c
006278f8: mov      r1, r0
006278fc: mov      r0, r8
00627900: bl       #0x30eba4
00627904: ldr      r1, [r5, #4]
00627908: mov      r8, r0
0062790c: mov      r0, r6
00627910: bl       #0x30ed6c
00627914: mov      r1, r0
00627918: mov      r0, sb
0062791c: bl       #0x30eba4
00627920: ldr      r1, [r5, #8]
00627924: mov      sb, r0
00627928: mov      r0, r6
0062792c: bl       #0x30ed6c
00627930: mov      r1, r0
00627934: mov      r0, sl
00627938: bl       #0x30eba4
0062793c: subs     r4, r4, #1
00627940: mov      sl, r0
00627944: add      r5, r5, #0xc
00627948: bne      #0x6278e4
0062794c: ldr      r3, [sp, #0x28]
00627950: str      r8, [r3], #4
00627954: ldr      r2, [sp, #0x28]
00627958: str      sb, [r2, #4]
0062795c: str      sl, [r3, #4]
00627960: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00627964: mov      r2, r1
00627968: ldr      r0, [r2], #4
0062796c: ldr      r3, [sp, #0x28]
00627970: str      r0, [r3], #4
00627974: ldr      r1, [r1, #4]
00627978: ldr      r0, [sp, #0x28]
0062797c: str      r1, [r0, #4]
00627980: ldr      r2, [r2, #4]
00627984: str      r2, [r3, #4]
00627988: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE15getBlendedValueEPvPfiSD_
00627350: cmp      r3, #1
00627354: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00627358: mov      r4, r3
0062735c: mov      fp, r2
00627360: beq      #0x62740c
00627364: cmp      r3, #0
00627368: moveq    r8, #0
0062736c: moveq    sb, r8
00627370: moveq    sl, r8
00627374: beq      #0x6273f4
00627378: mov      r8, #0
0062737c: mov      r5, r1
00627380: mov      r7, #0
00627384: mov      sb, r8
00627388: mov      sl, r8
0062738c: ldr      r6, [fp, r7]
00627390: ldr      r1, [r5]
00627394: add      r7, r7, #4
00627398: mov      r0, r6
0062739c: bl       #0x30ed6c
006273a0: mov      r1, r0
006273a4: mov      r0, r8
006273a8: bl       #0x30eba4
006273ac: ldr      r1, [r5, #4]
006273b0: mov      r8, r0
006273b4: mov      r0, r6
006273b8: bl       #0x30ed6c
006273bc: mov      r1, r0
006273c0: mov      r0, sb
006273c4: bl       #0x30eba4
006273c8: ldr      r1, [r5, #8]
006273cc: mov      sb, r0
006273d0: mov      r0, r6
006273d4: bl       #0x30ed6c
006273d8: mov      r1, r0
006273dc: mov      r0, sl
006273e0: bl       #0x30eba4
006273e4: subs     r4, r4, #1
006273e8: mov      sl, r0
006273ec: add      r5, r5, #0xc
006273f0: bne      #0x62738c
006273f4: ldr      r3, [sp, #0x28]
006273f8: str      r8, [r3], #4
006273fc: ldr      r2, [sp, #0x28]
00627400: str      sb, [r2, #4]
00627404: str      sl, [r3, #4]
00627408: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062740c: mov      r2, r1
00627410: ldr      r0, [r2], #4
00627414: ldr      r3, [sp, #0x28]
00627418: str      r0, [r3], #4
0062741c: ldr      r1, [r1, #4]
00627420: ldr      r0, [sp, #0x28]
00627424: str      r1, [r0, #4]
00627428: ldr      r2, [r2, #4]
0062742c: str      r2, [r3, #4]
00627430: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE11getInstanceEv
006107cc: push     {r4, r5, r6, lr}
006107d0: ldr      r4, [pc, #0x70]
006107d4: ldr      r3, [pc, #0x70]
006107d8: add      r4, pc, r4
006107dc: ldr      r6, [r4, r3]
006107e0: ldr      r3, [r6]
006107e4: tst      r3, #1
006107e8: beq      #0x6107f8
006107ec: ldr      r5, [pc, #0x5c]
006107f0: ldr      r0, [r4, r5]
006107f4: pop      {r4, r5, r6, pc}
006107f8: mov      r0, r6
006107fc: bl       #0x30e76c
00610800: cmp      r0, #0
00610804: beq      #0x6107ec
00610808: ldr      r3, [pc, #0x44]
0061080c: ldr      r5, [pc, #0x3c]
00610810: mov      r0, r6
00610814: ldr      r3, [r4, r3]
00610818: ldr      r6, [r4, r5]
0061081c: add      r3, r3, #8
00610820: str      r3, [r6]
00610824: bl       #0x30ea3c
00610828: ldr      r3, [pc, #0x28]
0061082c: mov      r0, r6
00610830: ldr      r1, [r4, r3]
00610834: ldr      r3, [pc, #0x20]
00610838: ldr      r2, [r4, r3]
0061083c: bl       #0x30e304
00610840: ldr      r0, [r4, r5]
00610844: pop      {r4, r5, r6, pc}
00610848: ldrhteq  r4, [r8], -r8
0061084c: muleq    r0, r0, sb
00610850: andeq    r1, r0, r4, asr #7
00610854: andeq    r3, r0, r8, lsl #31
00610858: andeq    r1, r0, r4, ror #23
0061085c: muleq    r0, r0, r8

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE11getInstanceEv
00610454: push     {r4, r5, r6, lr}
00610458: ldr      r4, [pc, #0x70]
0061045c: ldr      r3, [pc, #0x70]
00610460: add      r4, pc, r4
00610464: ldr      r6, [r4, r3]
00610468: ldr      r3, [r6]
0061046c: tst      r3, #1
00610470: beq      #0x610480
00610474: ldr      r5, [pc, #0x5c]
00610478: ldr      r0, [r4, r5]
0061047c: pop      {r4, r5, r6, pc}
00610480: mov      r0, r6
00610484: bl       #0x30e76c
00610488: cmp      r0, #0
0061048c: beq      #0x610474
00610490: ldr      r3, [pc, #0x44]
00610494: ldr      r5, [pc, #0x3c]
00610498: mov      r0, r6
0061049c: ldr      r3, [r4, r3]
006104a0: ldr      r6, [r4, r5]
006104a4: add      r3, r3, #8
006104a8: str      r3, [r6]
006104ac: bl       #0x30ea3c
006104b0: ldr      r3, [pc, #0x28]
006104b4: mov      r0, r6
006104b8: ldr      r1, [r4, r3]
006104bc: ldr      r3, [pc, #0x20]
006104c0: ldr      r2, [r4, r3]
006104c4: bl       #0x30e304
006104c8: ldr      r0, [r4, r5]
006104cc: pop      {r4, r5, r6, pc}
006104d0: eorseq   r4, r8, r0, lsr r6
006104d4: andeq    r1, r0, ip, asr #26
006104d8: andeq    r4, r0, ip, lsl sb
006104dc: andeq    r1, r0, ip, asr #31
006104e0: andeq    r4, r0, r4, ror #23
006104e4: muleq    r0, r0, r8

# _ZN6glitch7collada15animation_track13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEE19applyBlendedValueExEPvPfiSB_PNS1_15CApplicatorInfoE
006293a4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006293a8: cmp      r2, #1
006293ac: sub      sp, sp, #0x1c
006293b0: mov      sl, #0
006293b4: mov      r4, r2
006293b8: mov      r5, r1
006293bc: str      r3, [sp, #4]
006293c0: str      sl, [sp, #0x14]
006293c4: beq      #0x629478
006293c8: cmp      r2, #0
006293cc: moveq    fp, sl
006293d0: moveq    sb, sl
006293d4: beq      #0x629450
006293d8: mov      r6, r0
006293dc: mov      r8, #0
006293e0: mov      fp, sl
006293e4: mov      sb, sl
006293e8: ldr      r7, [r5, r8]
006293ec: ldr      r1, [r6]
006293f0: add      r8, r8, #4
006293f4: mov      r0, r7
006293f8: bl       #0x30ed6c
006293fc: mov      r1, r0
00629400: mov      r0, sl
00629404: bl       #0x30eba4
00629408: ldr      r1, [r6, #4]
0062940c: mov      sl, r0
00629410: mov      r0, r7
00629414: bl       #0x30ed6c
00629418: mov      r1, r0
0062941c: mov      r0, fp
00629420: bl       #0x30eba4
00629424: ldr      r1, [r6, #8]
00629428: mov      fp, r0
0062942c: mov      r0, r7
00629430: bl       #0x30ed6c
00629434: mov      r1, r0
00629438: mov      r0, sb
0062943c: bl       #0x30eba4
00629440: subs     r4, r4, #1
00629444: mov      sb, r0
00629448: add      r6, r6, #0xc
0062944c: bne      #0x6293e8
00629450: add      r1, sp, #0x18
00629454: str      sl, [r1, #-0xc]!
00629458: str      fp, [sp, #0x10]
0062945c: str      sb, [r1, #8]
00629460: ldr      r0, [sp, #4]
00629464: ldr      r3, [r0]
00629468: mov      lr, pc
0062946c: ldr      pc, [r3, #0xa4]
00629470: add      sp, sp, #0x1c
00629474: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00629478: mov      r3, r0
0062947c: ldr      ip, [r3], #4
00629480: ldr      r2, [r0, #4]
00629484: add      r1, sp, #0x18
00629488: ldr      r3, [r3, #4]
0062948c: str      ip, [r1, #-0xc]!
00629490: str      r2, [sp, #0x10]
00629494: str      r3, [r1, #8]
00629498: b        #0x629460

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00622bf8: push     {r4, lr}
00622bfc: mov      r0, r2
00622c00: ldr      r3, [r2]
00622c04: mov      lr, pc
00622c08: ldr      pc, [r3, #0x94]
00622c0c: pop      {r4, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE15getBlendedValueEPvPfiSD_
00626df8: cmp      r3, #1
00626dfc: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00626e00: mov      r4, r3
00626e04: mov      fp, r2
00626e08: beq      #0x626eb4
00626e0c: cmp      r3, #0
00626e10: moveq    r8, #0
00626e14: moveq    sb, r8
00626e18: moveq    sl, r8
00626e1c: beq      #0x626e9c
00626e20: mov      r8, #0
00626e24: mov      r5, r1
00626e28: mov      r7, #0
00626e2c: mov      sb, r8
00626e30: mov      sl, r8
00626e34: ldr      r6, [fp, r7]
00626e38: ldr      r1, [r5]
00626e3c: add      r7, r7, #4
00626e40: mov      r0, r6
00626e44: bl       #0x30ed6c
00626e48: mov      r1, r0
00626e4c: mov      r0, r8
00626e50: bl       #0x30eba4
00626e54: ldr      r1, [r5, #4]
00626e58: mov      r8, r0
00626e5c: mov      r0, r6
00626e60: bl       #0x30ed6c
00626e64: mov      r1, r0
00626e68: mov      r0, sb
00626e6c: bl       #0x30eba4
00626e70: ldr      r1, [r5, #8]
00626e74: mov      sb, r0
00626e78: mov      r0, r6
00626e7c: bl       #0x30ed6c
00626e80: mov      r1, r0
00626e84: mov      r0, sl
00626e88: bl       #0x30eba4
00626e8c: subs     r4, r4, #1
00626e90: mov      sl, r0
00626e94: add      r5, r5, #0xc
00626e98: bne      #0x626e34
00626e9c: ldr      r3, [sp, #0x28]
00626ea0: str      r8, [r3], #4
00626ea4: ldr      r2, [sp, #0x28]
00626ea8: str      sb, [r2, #4]
00626eac: str      sl, [r3, #4]
00626eb0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00626eb4: mov      r2, r1
00626eb8: ldr      r0, [r2], #4
00626ebc: ldr      r3, [sp, #0x28]
00626ec0: str      r0, [r3], #4
00626ec4: ldr      r1, [r1, #4]
00626ec8: ldr      r0, [sp, #0x28]
00626ecc: str      r1, [r0, #4]
00626ed0: ldr      r2, [r2, #4]
00626ed4: str      r2, [r3, #4]
00626ed8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
0062949c: mov      r0, r1
006294a0: ldr      ip, [sp, #4]
006294a4: mov      r1, r2
006294a8: mov      r2, r3
006294ac: ldr      r3, [sp]
006294b0: str      ip, [sp]
006294b4: b        #0x6293a4

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE12getValueSizeEv
0060efc8: mov      r0, #0xc
0060efcc: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE12getValueSizeEv
0060efb0: mov      r0, #0xc
0060efb4: bx       lr

# _ZN6glitch5scene10ISceneNode11setRotationERKNS_4core10quaternionE
005970f4: ldr      r3, [r1]
005970f8: ldr      r2, [r0, #0x11c]
005970fc: str      r3, [r0, #0xb8]
00597100: ldr      r3, [r1, #4]
00597104: orr      r2, r2, #4
00597108: str      r3, [r0, #0xbc]
0059710c: ldr      r3, [r1, #8]
00597110: str      r3, [r0, #0xc0]
00597114: ldr      r3, [r1, #0xc]
00597118: str      r2, [r0, #0x11c]
0059711c: str      r3, [r0, #0xc4]
00597120: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE17applyBlendedValueEPvPfiSD_PNS1_15CApplicatorInfoE
0062ca3c: mov      r0, r1
0062ca40: ldr      ip, [sp, #4]
0062ca44: mov      r1, r2
0062ca48: mov      r2, r3
0062ca4c: ldr      r3, [sp]
0062ca50: str      ip, [sp]
0062ca54: b        #0x62c944

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE15getBlendedValueEPvPfiSD_
0062a034: cmp      r3, #1
0062a038: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0062a03c: mov      r4, r3
0062a040: mov      fp, r2
0062a044: beq      #0x62a0f0
0062a048: cmp      r3, #0
0062a04c: moveq    r8, #0
0062a050: moveq    sb, r8
0062a054: moveq    sl, r8
0062a058: beq      #0x62a0d8
0062a05c: mov      r8, #0
0062a060: mov      r5, r1
0062a064: mov      r7, #0
0062a068: mov      sb, r8
0062a06c: mov      sl, r8
0062a070: ldr      r6, [fp, r7]
0062a074: ldr      r1, [r5]
0062a078: add      r7, r7, #4
0062a07c: mov      r0, r6
0062a080: bl       #0x30ed6c
0062a084: mov      r1, r0
0062a088: mov      r0, r8
0062a08c: bl       #0x30eba4
0062a090: ldr      r1, [r5, #4]
0062a094: mov      r8, r0
0062a098: mov      r0, r6
0062a09c: bl       #0x30ed6c
0062a0a0: mov      r1, r0
0062a0a4: mov      r0, sb
0062a0a8: bl       #0x30eba4
0062a0ac: ldr      r1, [r5, #8]
0062a0b0: mov      sb, r0
0062a0b4: mov      r0, r6
0062a0b8: bl       #0x30ed6c
0062a0bc: mov      r1, r0
0062a0c0: mov      r0, sl
0062a0c4: bl       #0x30eba4
0062a0c8: subs     r4, r4, #1
0062a0cc: mov      sl, r0
0062a0d0: add      r5, r5, #0xc
0062a0d4: bne      #0x62a070
0062a0d8: ldr      r3, [sp, #0x28]
0062a0dc: str      r8, [r3], #4
0062a0e0: ldr      r2, [sp, #0x28]
0062a0e4: str      sb, [r2, #4]
0062a0e8: str      sl, [r3, #4]
0062a0ec: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0062a0f0: mov      r2, r1
0062a0f4: ldr      r0, [r2], #4
0062a0f8: ldr      r3, [sp, #0x28]
0062a0fc: str      r0, [r3], #4
0062a100: ldr      r1, [r1, #4]
0062a104: ldr      r0, [sp, #0x28]
0062a108: str      r1, [r0, #4]
0062a10c: ldr      r2, [r2, #4]
0062a110: str      r2, [r3, #4]
0062a114: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE12getValueSizeEv
0060ef4c: mov      r0, #0xc
0060ef50: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
006238f4: push     {r4, lr}
006238f8: mov      r0, r2
006238fc: ldr      r3, [r2]
00623900: mov      lr, pc
00623904: ldr      pc, [r3, #0x94]
00623908: pop      {r4, pc}
