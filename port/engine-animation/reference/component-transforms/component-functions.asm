
# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061ffbc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061ffc0: mov      r4, r1
0061ffc4: mov      r1, #0
0061ffc8: mov      r5, r2
0061ffcc: mov      r8, r3
0061ffd0: mov      r7, r0
0061ffd4: ldr      r6, [sp, #0x20]
0061ffd8: bl       #0x669e24
0061ffdc: ldr      sl, [r0, #4]
0061ffe0: mov      r0, r7
0061ffe4: bl       #0x669e54
0061ffe8: cmp      r0, #0
0061ffec: beq      #0x620040
0061fff0: mov      r0, r7
0061fff4: bl       #0x669e68
0061fff8: ldr      r3, [r0]
0061fffc: mov      r0, r7
00620000: str      r3, [r6]
00620004: bl       #0x669e68
00620008: ldr      r3, [r0, #4]
0062000c: str      r3, [r6, #4]
00620010: ldr      r4, [sl, r4, lsl #2]
00620014: ldr      r0, [sl, r5, lsl #2]
00620018: mov      r1, r4
0062001c: bl       #0x30e3ac
00620020: mov      r1, r0
00620024: mov      r0, r8
00620028: bl       #0x30ed6c
0062002c: mov      r1, r0
00620030: mov      r0, r4
00620034: bl       #0x30eba4
00620038: str      r0, [r6, #8]
0062003c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00620040: ldr      r4, [sl, r4, lsl #2]
00620044: ldr      r0, [sl, r5, lsl #2]
00620048: mov      r1, r4
0062004c: bl       #0x30e3ac
00620050: mov      r1, r0
00620054: mov      r0, r8
00620058: bl       #0x30ed6c
0062005c: mov      r1, r0
00620060: mov      r0, r4
00620064: bl       #0x30eba4
00620068: str      r0, [r6]
0062006c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE12getValueSizeEv
0060ef1c: mov      r0, #0xc
0060ef20: bx       lr

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
0061e334: push     {r4, r5, r6, r7, r8, lr}
0061e338: mov      r4, r1
0061e33c: mov      r1, #0
0061e340: mov      r5, r2
0061e344: mov      r6, r3
0061e348: mov      r7, r0
0061e34c: bl       #0x669e24
0061e350: ldr      r3, [r0, #4]
0061e354: ldr      r1, [r3, r4, lsl #2]
0061e358: ldr      r0, [r3, r5, lsl #2]
0061e35c: bl       #0x30e3ac
0061e360: mov      r4, r0
0061e364: mov      r0, r7
0061e368: bl       #0x669e54
0061e36c: cmp      r0, #0
0061e370: bne      #0x61e37c
0061e374: str      r4, [r6]
0061e378: pop      {r4, r5, r6, r7, r8, pc}
0061e37c: mov      r0, r7
0061e380: bl       #0x669e68
0061e384: ldr      r2, [r0]
0061e388: mov      r3, r6
0061e38c: str      r2, [r3], #4
0061e390: ldr      r2, [r0, #4]
0061e394: str      r2, [r6, #4]
0061e398: str      r4, [r3, #4]
0061e39c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
0061e0ec: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061e0f0: mov      r4, r1
0061e0f4: mov      r1, #0
0061e0f8: mov      sb, r3
0061e0fc: mov      r5, r2
0061e100: mov      r8, r0
0061e104: ldr      sl, [sp, #0x28]
0061e108: ldr      r7, [sp, #0x2c]
0061e10c: bl       #0x669e24
0061e110: ldr      r6, [r0, #4]
0061e114: ldr      fp, [r6, r4, lsl #2]
0061e118: ldr      r0, [r6, r5, lsl #2]
0061e11c: mov      r1, fp
0061e120: bl       #0x30e3ac
0061e124: mov      r1, fp
0061e128: mov      r4, r0
0061e12c: ldr      r0, [r6, sb, lsl #2]
0061e130: bl       #0x30e3ac
0061e134: mov      sb, r0
0061e138: mov      r0, r8
0061e13c: bl       #0x669e54
0061e140: cmp      r0, #0
0061e144: bne      #0x61e174
0061e148: mov      r1, r4
0061e14c: mov      r0, sb
0061e150: bl       #0x30e3ac
0061e154: mov      r1, r0
0061e158: mov      r0, sl
0061e15c: bl       #0x30ed6c
0061e160: mov      r1, r0
0061e164: mov      r0, r4
0061e168: bl       #0x30eba4
0061e16c: str      r0, [r7]
0061e170: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061e174: mov      r0, r8
0061e178: bl       #0x669e68
0061e17c: ldr      r3, [r0]
0061e180: mov      r5, r7
0061e184: mov      r6, r0
0061e188: str      r3, [r5], #4
0061e18c: mov      r1, r4
0061e190: mov      r0, sb
0061e194: bl       #0x30e3ac
0061e198: mov      r1, r0
0061e19c: mov      r0, sl
0061e1a0: bl       #0x30ed6c
0061e1a4: mov      r1, r0
0061e1a8: mov      r0, r4
0061e1ac: bl       #0x30eba4
0061e1b0: str      r0, [r7, #4]
0061e1b4: ldr      r3, [r6, #8]
0061e1b8: str      r3, [r5, #4]
0061e1bc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

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

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE13retrieveValueEPvSD_
0062069c: push     {r4, lr}
006206a0: ldr      r3, [r1]
006206a4: mov      r0, r1
006206a8: mov      r4, r2
006206ac: mov      lr, pc
006206b0: ldr      pc, [r3, #0x90]
006206b4: ldr      r3, [r0]
006206b8: str      r3, [r4]
006206bc: ldr      r3, [r0, #4]
006206c0: str      r3, [r4, #4]
006206c4: ldr      r3, [r0, #8]
006206c8: str      r3, [r4, #8]
006206cc: pop      {r4, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061fa24: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061fa28: mov      r4, r1
0061fa2c: mov      r1, #0
0061fa30: mov      r5, r2
0061fa34: mov      r8, r3
0061fa38: mov      r6, r0
0061fa3c: ldr      r7, [sp, #0x20]
0061fa40: bl       #0x669e24
0061fa44: ldr      sb, [r0, #4]
0061fa48: mov      r0, r6
0061fa4c: bl       #0x669e54
0061fa50: cmp      r0, #0
0061fa54: beq      #0x61faac
0061fa58: ldr      sl, [sb, r4, lsl #2]
0061fa5c: ldr      r0, [sb, r5, lsl #2]
0061fa60: mov      r4, r7
0061fa64: mov      r1, sl
0061fa68: bl       #0x30e3ac
0061fa6c: mov      r1, r0
0061fa70: mov      r0, r8
0061fa74: bl       #0x30ed6c
0061fa78: mov      r1, r0
0061fa7c: mov      r0, sl
0061fa80: bl       #0x30eba4
0061fa84: str      r0, [r4], #4
0061fa88: mov      r0, r6
0061fa8c: bl       #0x669e68
0061fa90: ldr      r3, [r0, #4]
0061fa94: mov      r0, r6
0061fa98: str      r3, [r7, #4]
0061fa9c: bl       #0x669e68
0061faa0: ldr      r3, [r0, #8]
0061faa4: str      r3, [r4, #4]
0061faa8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061faac: ldr      r4, [sb, r4, lsl #2]
0061fab0: ldr      r0, [sb, r5, lsl #2]
0061fab4: mov      r1, r4
0061fab8: bl       #0x30e3ac
0061fabc: mov      r1, r0
0061fac0: mov      r0, r8
0061fac4: bl       #0x30ed6c
0061fac8: mov      r1, r0
0061facc: mov      r0, r4
0061fad0: bl       #0x30eba4
0061fad4: str      r0, [r7]
0061fad8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleYExIfEELi1EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00622c40: push     {r4, lr}
00622c44: mov      r0, r2
00622c48: ldr      r3, [r2]
00622c4c: mov      lr, pc
00622c50: ldr      pc, [r3, #0x94]
00622c54: pop      {r4, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE13retrieveValueEPvSD_
00620460: push     {r4, lr}
00620464: ldr      r3, [r1]
00620468: mov      r0, r1
0062046c: mov      r4, r2
00620470: mov      lr, pc
00620474: ldr      pc, [r3, #0xa0]
00620478: ldr      r3, [r0]
0062047c: str      r3, [r4]
00620480: ldr      r3, [r0, #4]
00620484: str      r3, [r4, #4]
00620488: ldr      r3, [r0, #8]
0062048c: str      r3, [r4, #8]
00620490: pop      {r4, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061df98: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061df9c: mov      r4, r1
0061dfa0: mov      r1, #0
0061dfa4: mov      r5, r2
0061dfa8: mov      r8, r3
0061dfac: mov      r6, r0
0061dfb0: ldr      r7, [sp, #0x20]
0061dfb4: bl       #0x669e24
0061dfb8: ldr      sb, [r0, #4]
0061dfbc: mov      r0, r6
0061dfc0: bl       #0x669e54
0061dfc4: cmp      r0, #0
0061dfc8: beq      #0x61e020
0061dfcc: mov      r0, r6
0061dfd0: bl       #0x669e68
0061dfd4: ldr      r3, [r0]
0061dfd8: mov      sl, r7
0061dfdc: str      r3, [sl], #4
0061dfe0: ldr      r4, [sb, r4, lsl #2]
0061dfe4: ldr      r0, [sb, r5, lsl #2]
0061dfe8: mov      r1, r4
0061dfec: bl       #0x30e3ac
0061dff0: mov      r1, r0
0061dff4: mov      r0, r8
0061dff8: bl       #0x30ed6c
0061dffc: mov      r1, r0
0061e000: mov      r0, r4
0061e004: bl       #0x30eba4
0061e008: str      r0, [r7, #4]
0061e00c: mov      r0, r6
0061e010: bl       #0x669e68
0061e014: ldr      r3, [r0, #8]
0061e018: str      r3, [sl, #4]
0061e01c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061e020: ldr      r4, [sb, r4, lsl #2]
0061e024: ldr      r0, [sb, r5, lsl #2]
0061e028: mov      r1, r4
0061e02c: bl       #0x30e3ac
0061e030: mov      r1, r0
0061e034: mov      r0, r8
0061e038: bl       #0x30ed6c
0061e03c: mov      r1, r0
0061e040: mov      r0, r4
0061e044: bl       #0x30eba4
0061e048: str      r0, [r7]
0061e04c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00622cb8: push     {r4, lr}
00622cbc: mov      r0, r2
00622cc0: ldr      r3, [r2]
00622cc4: mov      lr, pc
00622cc8: ldr      pc, [r3, #0xa4]
00622ccc: pop      {r4, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE12getValueSizeEv
0060efe0: mov      r0, #0xc
0060efe4: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE13retrieveValueEPvSD_
006203c4: push     {r4, lr}
006203c8: ldr      r3, [r1]
006203cc: mov      r0, r1
006203d0: mov      r4, r2
006203d4: mov      lr, pc
006203d8: ldr      pc, [r3, #0xa0]
006203dc: ldr      r3, [r0]
006203e0: str      r3, [r4]
006203e4: ldr      r3, [r0, #4]
006203e8: str      r3, [r4, #4]
006203ec: ldr      r3, [r0, #8]
006203f0: str      r3, [r4, #8]
006203f4: pop      {r4, pc}

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

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
0061dda0: push     {r4, r5, r6, r7, r8, lr}
0061dda4: mov      r4, r1
0061dda8: mov      r1, #0
0061ddac: mov      r5, r2
0061ddb0: mov      r6, r3
0061ddb4: mov      r7, r0
0061ddb8: bl       #0x669e24
0061ddbc: ldr      r3, [r0, #4]
0061ddc0: ldr      r1, [r3, r4, lsl #2]
0061ddc4: ldr      r0, [r3, r5, lsl #2]
0061ddc8: bl       #0x30e3ac
0061ddcc: mov      r4, r0
0061ddd0: mov      r0, r7
0061ddd4: bl       #0x669e54
0061ddd8: cmp      r0, #0
0061dddc: bne      #0x61dde8
0061dde0: str      r4, [r6]
0061dde4: pop      {r4, r5, r6, r7, r8, pc}
0061dde8: mov      r0, r7
0061ddec: bl       #0x669e68
0061ddf0: mov      r3, r6
0061ddf4: str      r4, [r3], #4
0061ddf8: ldr      r2, [r0, #4]
0061ddfc: str      r2, [r6, #4]
0061de00: ldr      r2, [r0, #8]
0061de04: str      r2, [r3, #4]
0061de08: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE13retrieveValueEPvSD_
00620600: push     {r4, lr}
00620604: ldr      r3, [r1]
00620608: mov      r0, r1
0062060c: mov      r4, r2
00620610: mov      lr, pc
00620614: ldr      pc, [r3, #0x90]
00620618: ldr      r3, [r0]
0062061c: str      r3, [r4]
00620620: ldr      r3, [r0, #4]
00620624: str      r3, [r4, #4]
00620628: ldr      r3, [r0, #8]
0062062c: str      r3, [r4, #8]
00620630: pop      {r4, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061ff3c: push     {r4, r5, r6, r7, r8, lr}
0061ff40: mov      r4, r1
0061ff44: mov      r1, #0
0061ff48: mov      r6, r2
0061ff4c: mov      r5, r0
0061ff50: bl       #0x669e24
0061ff54: ldr      r7, [r0, #4]
0061ff58: mov      r0, r5
0061ff5c: bl       #0x669e54
0061ff60: cmp      r0, #0
0061ff64: bne      #0x61ff74
0061ff68: ldr      r3, [r7, r4, lsl #2]
0061ff6c: str      r3, [r6]
0061ff70: pop      {r4, r5, r6, r7, r8, pc}
0061ff74: mov      r0, r5
0061ff78: bl       #0x669e68
0061ff7c: cmp      r0, #0
0061ff80: beq      #0x61ff68
0061ff84: mov      r0, r5
0061ff88: bl       #0x669e68
0061ff8c: ldr      r2, [r0]
0061ff90: mov      r3, r6
0061ff94: str      r2, [r3], #4
0061ff98: ldr      r2, [r0, #4]
0061ff9c: str      r2, [r6, #4]
0061ffa0: ldr      r2, [r7, r4, lsl #2]
0061ffa4: str      r2, [r3, #4]
0061ffa8: pop      {r4, r5, r6, r7, r8, pc}

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

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
00615e6c: push     {r4, r5, r6, r7, r8, lr}
00615e70: mov      r4, r1
00615e74: mov      r1, #0
00615e78: mov      r5, r2
00615e7c: mov      r6, r3
00615e80: mov      r7, r0
00615e84: bl       #0x669e24
00615e88: ldr      r3, [r0, #4]
00615e8c: ldr      r1, [r3, r4, lsl #2]
00615e90: ldr      r0, [r3, r5, lsl #2]
00615e94: bl       #0x30e3ac
00615e98: mov      r4, r0
00615e9c: mov      r0, r7
00615ea0: bl       #0x669e54
00615ea4: cmp      r0, #0
00615ea8: bne      #0x615eb4
00615eac: str      r4, [r6]
00615eb0: pop      {r4, r5, r6, r7, r8, pc}
00615eb4: mov      r0, r7
00615eb8: bl       #0x669e68
00615ebc: ldr      r2, [r0]
00615ec0: mov      r3, r6
00615ec4: str      r2, [r3], #4
00615ec8: ldr      r2, [r0, #4]
00615ecc: str      r2, [r6, #4]
00615ed0: str      r4, [r3, #4]
00615ed4: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061fcf0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061fcf4: mov      r4, r1
0061fcf8: mov      r1, #0
0061fcfc: mov      r5, r2
0061fd00: mov      r8, r3
0061fd04: mov      r6, r0
0061fd08: ldr      r7, [sp, #0x20]
0061fd0c: bl       #0x669e24
0061fd10: ldr      sb, [r0, #4]
0061fd14: mov      r0, r6
0061fd18: bl       #0x669e54
0061fd1c: cmp      r0, #0
0061fd20: beq      #0x61fd78
0061fd24: mov      r0, r6
0061fd28: bl       #0x669e68
0061fd2c: ldr      r3, [r0]
0061fd30: mov      sl, r7
0061fd34: str      r3, [sl], #4
0061fd38: ldr      r4, [sb, r4, lsl #2]
0061fd3c: ldr      r0, [sb, r5, lsl #2]
0061fd40: mov      r1, r4
0061fd44: bl       #0x30e3ac
0061fd48: mov      r1, r0
0061fd4c: mov      r0, r8
0061fd50: bl       #0x30ed6c
0061fd54: mov      r1, r0
0061fd58: mov      r0, r4
0061fd5c: bl       #0x30eba4
0061fd60: str      r0, [r7, #4]
0061fd64: mov      r0, r6
0061fd68: bl       #0x669e68
0061fd6c: ldr      r3, [r0, #8]
0061fd70: str      r3, [sl, #4]
0061fd74: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061fd78: ldr      r4, [sb, r4, lsl #2]
0061fd7c: ldr      r0, [sb, r5, lsl #2]
0061fd80: mov      r1, r4
0061fd84: bl       #0x30e3ac
0061fd88: mov      r1, r0
0061fd8c: mov      r0, r8
0061fd90: bl       #0x30ed6c
0061fd94: mov      r1, r0
0061fd98: mov      r0, r4
0061fd9c: bl       #0x30eba4
0061fda0: str      r0, [r7]
0061fda4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
0061e3b4: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061e3b8: mov      r4, r1
0061e3bc: mov      r1, #0
0061e3c0: mov      r5, r2
0061e3c4: mov      sb, r3
0061e3c8: mov      r8, r0
0061e3cc: ldr      sl, [sp, #0x28]
0061e3d0: ldr      r7, [sp, #0x2c]
0061e3d4: bl       #0x669e24
0061e3d8: ldr      r6, [r0, #4]
0061e3dc: ldr      fp, [r6, r4, lsl #2]
0061e3e0: ldr      r0, [r6, r5, lsl #2]
0061e3e4: mov      r1, fp
0061e3e8: bl       #0x30e3ac
0061e3ec: mov      r1, fp
0061e3f0: mov      r4, r0
0061e3f4: ldr      r0, [r6, sb, lsl #2]
0061e3f8: bl       #0x30e3ac
0061e3fc: mov      r6, r0
0061e400: mov      r0, r8
0061e404: bl       #0x669e54
0061e408: cmp      r0, #0
0061e40c: bne      #0x61e43c
0061e410: mov      r1, r4
0061e414: mov      r0, r6
0061e418: bl       #0x30e3ac
0061e41c: mov      r1, r0
0061e420: mov      r0, sl
0061e424: bl       #0x30ed6c
0061e428: mov      r1, r0
0061e42c: mov      r0, r4
0061e430: bl       #0x30eba4
0061e434: str      r0, [r7]
0061e438: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061e43c: mov      r0, r8
0061e440: bl       #0x669e68
0061e444: ldr      r2, [r0]
0061e448: mov      r5, r7
0061e44c: mov      r3, r0
0061e450: str      r2, [r5], #4
0061e454: ldr      r3, [r3, #4]
0061e458: mov      r1, r4
0061e45c: mov      r0, r6
0061e460: str      r3, [r7, #4]
0061e464: bl       #0x30e3ac
0061e468: mov      r1, r0
0061e46c: mov      r0, sl
0061e470: bl       #0x30ed6c
0061e474: mov      r1, r0
0061e478: mov      r0, r4
0061e47c: bl       #0x30eba4
0061e480: str      r0, [r5, #4]
0061e484: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

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

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
0061de20: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061de24: mov      r4, r1
0061de28: mov      r1, #0
0061de2c: mov      r5, r2
0061de30: mov      sb, r3
0061de34: mov      r8, r0
0061de38: ldr      sl, [sp, #0x28]
0061de3c: ldr      r7, [sp, #0x2c]
0061de40: bl       #0x669e24
0061de44: ldr      r6, [r0, #4]
0061de48: ldr      fp, [r6, r4, lsl #2]
0061de4c: ldr      r0, [r6, r5, lsl #2]
0061de50: mov      r1, fp
0061de54: bl       #0x30e3ac
0061de58: mov      r1, fp
0061de5c: mov      r4, r0
0061de60: ldr      r0, [r6, sb, lsl #2]
0061de64: bl       #0x30e3ac
0061de68: mov      r6, r0
0061de6c: mov      r0, r8
0061de70: bl       #0x669e54
0061de74: cmp      r0, #0
0061de78: bne      #0x61dea8
0061de7c: mov      r1, r4
0061de80: mov      r0, r6
0061de84: bl       #0x30e3ac
0061de88: mov      r1, r0
0061de8c: mov      r0, sl
0061de90: bl       #0x30ed6c
0061de94: mov      r1, r0
0061de98: mov      r0, r4
0061de9c: bl       #0x30eba4
0061dea0: str      r0, [r7]
0061dea4: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061dea8: mov      r0, r8
0061deac: bl       #0x669e68
0061deb0: mov      r1, r4
0061deb4: mov      r5, r0
0061deb8: mov      r0, r6
0061debc: bl       #0x30e3ac
0061dec0: mov      r1, r0
0061dec4: mov      r0, sl
0061dec8: bl       #0x30ed6c
0061decc: mov      r1, r0
0061ded0: mov      r0, r4
0061ded4: bl       #0x30eba4
0061ded8: mov      r3, r7
0061dedc: str      r0, [r3], #4
0061dee0: ldr      r2, [r5, #4]
0061dee4: str      r2, [r7, #4]
0061dee8: ldr      r2, [r5, #8]
0061deec: str      r2, [r3, #4]
0061def0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061e1e4: push     {r4, r5, r6, r7, r8, lr}
0061e1e8: mov      r4, r1
0061e1ec: mov      r1, #0
0061e1f0: mov      r6, r2
0061e1f4: mov      r5, r0
0061e1f8: bl       #0x669e24
0061e1fc: ldr      r7, [r0, #4]
0061e200: mov      r0, r5
0061e204: bl       #0x669e54
0061e208: cmp      r0, #0
0061e20c: bne      #0x61e21c
0061e210: ldr      r3, [r7, r4, lsl #2]
0061e214: str      r3, [r6]
0061e218: pop      {r4, r5, r6, r7, r8, pc}
0061e21c: mov      r0, r5
0061e220: bl       #0x669e68
0061e224: cmp      r0, #0
0061e228: beq      #0x61e210
0061e22c: mov      r0, r5
0061e230: bl       #0x669e68
0061e234: ldr      r2, [r0]
0061e238: mov      r3, r6
0061e23c: str      r2, [r3], #4
0061e240: ldr      r2, [r0, #4]
0061e244: str      r2, [r6, #4]
0061e248: ldr      r2, [r7, r4, lsl #2]
0061e24c: str      r2, [r3, #4]
0061e250: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada6detail27constructCompatibilityTableEv
00670a60: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00670a64: ldr      r4, [pc, #0x288]
00670a68: add      r4, pc, r4
00670a6c: ldr      r5, [r4]
00670a70: ands     r5, r5, #1
00670a74: beq      #0x670cb0
00670a78: ldr      r6, [pc, #0x278]
00670a7c: mov      r5, #1
00670a80: mov      r7, #0
00670a84: add      r6, pc, r6
00670a88: mov      r0, r6
00670a8c: mov      r8, r5
00670a90: mov      r3, #0
00670a94: add      r4, r7, r7, lsl #1
00670a98: add      r2, r4, r3, lsr #5
00670a9c: and      ip, r3, #0x1f
00670aa0: add      r2, r6, r2, lsl #2
00670aa4: ldr      r1, [r2, #4]
00670aa8: add      r3, r3, #1
00670aac: cmp      r3, #0x5c
00670ab0: bic      r1, r1, r5, lsl ip
00670ab4: str      r1, [r2, #4]
00670ab8: bne      #0x670a98
00670abc: add      r3, r4, r7, lsr #5
00670ac0: and      r1, r7, #0x1f
00670ac4: add      r3, r0, r3, lsl #2
00670ac8: ldr      r2, [r3, #4]
00670acc: add      r7, r7, #1
00670ad0: cmp      r7, #0x5c
00670ad4: orr      r2, r2, r8, lsl r1
00670ad8: str      r2, [r3, #4]
00670adc: bne      #0x670a90
00670ae0: ldr      r7, [r0, #0x450]
00670ae4: ldr      r2, [r0, #0x3d8]
00670ae8: ldr      r6, [r0, #0x420]
00670aec: orr      r7, r7, #0x7800000
00670af0: str      r7, [r0, #0x450]
00670af4: ldr      r7, [r0, #0x10c]
00670af8: orr      r2, r2, #0x3c0000
00670afc: ldr      sb, [r0, #0x42c]
00670b00: orr      r3, r7, #0x3a00000
00670b04: ldr      r7, [r0, #0x118]
00670b08: ldr      r8, [r0, #0x438]
00670b0c: ldr      sl, [r0, #0x444]
00670b10: orr      r7, r7, #0x3600000
00670b14: str      r7, [r0, #0x118]
00670b18: ldr      r7, [r0, #0x130]
00670b1c: ldr      r5, [r0, #0x3e4]
00670b20: ldr      ip, [r0, #0x3f0]
00670b24: orr      r7, r7, #0x1e00000
00670b28: str      r7, [r0, #0x130]
00670b2c: ldr      r7, [r0, #0x100]
00670b30: ldr      r4, [r0, #0x3fc]
00670b34: ldr      r1, [r0, #0x408]
00670b38: orr      r7, r7, #0x3c00000
00670b3c: ldr      fp, [r0, #0x124]
00670b40: str      r7, [r0, #0x100]
00670b44: str      r2, [r0, #0x3d8]
00670b48: ldr      r7, [r0, #0x3a8]
00670b4c: ldr      r2, [r0, #0x7c]
00670b50: orr      sb, sb, #0xe800000
00670b54: orr      sl, sl, #0xb800000
00670b58: orr      r8, r8, #0xd800000
00670b5c: orr      r6, r6, #0xf000000
00670b60: orr      r5, r5, #0x3a0000
00670b64: orr      r4, r4, #0x2e0000
00670b68: orr      ip, ip, #0x360000
00670b6c: orr      r1, r1, #0x1e0000
00670b70: orr      fp, fp, #0x2e00000
00670b74: orr      r7, r7, #0x1d000
00670b78: orr      r2, r2, #0x3800
00670b7c: str      r7, [r0, #0x3a8]
00670b80: str      sb, [r0, #0x42c]
00670b84: str      sl, [r0, #0x444]
00670b88: str      r8, [r0, #0x438]
00670b8c: str      r6, [r0, #0x420]
00670b90: str      r5, [r0, #0x3e4]
00670b94: str      r4, [r0, #0x3fc]
00670b98: str      ip, [r0, #0x3f0]
00670b9c: str      r1, [r0, #0x408]
00670ba0: str      r3, [r0, #0x10c]
00670ba4: str      fp, [r0, #0x124]
00670ba8: ldr      r7, [r0, #0x39c]
00670bac: ldr      sl, [r0, #0x3b4]
00670bb0: ldr      sb, [r0, #0x3c0]
00670bb4: ldr      r8, [r0, #0x3cc]
00670bb8: ldr      r1, [r0, #0x360]
00670bbc: ldr      r6, [r0, #0x36c]
00670bc0: ldr      r4, [r0, #0x378]
00670bc4: ldr      r5, [r0, #0x384]
00670bc8: ldr      ip, [r0, #0x390]
00670bcc: ldr      fp, [r0, #0x88]
00670bd0: str      r2, [r0, #0x7c]
00670bd4: ldr      r2, [r0, #0xa0]
00670bd8: ldr      r3, [r0, #0x94]
00670bdc: orr      sb, sb, #0x17000
00670be0: orr      r2, r2, #0x1c00
00670be4: str      r2, [r0, #0xa0]
00670be8: ldr      r2, [r0, #0x10]
00670bec: orr      r3, r3, #0x2c00
00670bf0: str      r3, [r0, #0x94]
00670bf4: orr      r2, r2, #0x1c
00670bf8: ldr      r3, [r0, #0x1c]
00670bfc: str      r2, [r0, #0x10]
00670c00: ldr      r2, [r0, #0x34]
00670c04: orr      r3, r3, #0x1a
00670c08: str      r3, [r0, #0x1c]
00670c0c: orr      r2, r2, #0xe
00670c10: ldr      r3, [r0, #0x28]
00670c14: str      r2, [r0, #0x34]
00670c18: ldr      r2, [r0, #0x4c]
00670c1c: orr      r3, r3, #0x16
00670c20: str      r3, [r0, #0x28]
00670c24: orr      r2, r2, #0x3a0
00670c28: ldr      r3, [r0, #0x64]
00670c2c: str      r2, [r0, #0x4c]
00670c30: ldr      r2, [r0, #0x58]
00670c34: orr      r3, r3, #0x2e0
00670c38: str      r3, [r0, #0x64]
00670c3c: orr      r2, r2, #0x360
00670c40: ldr      r3, [r0, #0x70]
00670c44: str      r2, [r0, #0x58]
00670c48: ldr      r2, [r0, #0x40]
00670c4c: orr      r3, r3, #0x1e0
00670c50: str      r3, [r0, #0x70]
00670c54: orr      sl, sl, #0x1b000
00670c58: orr      r8, r8, #0xf000
00670c5c: orr      r7, r7, #0x1e000
00670c60: orr      r6, r6, #0xe80
00670c64: orr      r5, r5, #0xb80
00670c68: orr      r4, r4, #0xd80
00670c6c: orr      ip, ip, #0x780
00670c70: orr      r1, r1, #0xf00
00670c74: orr      fp, fp, #0x3400
00670c78: orr      r3, r2, #0x3c0
00670c7c: str      sb, [r0, #0x3c0]
00670c80: str      sl, [r0, #0x3b4]
00670c84: str      r8, [r0, #0x3cc]
00670c88: str      r7, [r0, #0x39c]
00670c8c: str      r6, [r0, #0x36c]
00670c90: str      r5, [r0, #0x384]
00670c94: str      r4, [r0, #0x378]
00670c98: str      ip, [r0, #0x390]
00670c9c: str      r1, [r0, #0x360]
00670ca0: str      fp, [r0, #0x88]
00670ca4: str      r3, [r0, #0x40]
00670ca8: add      r0, r0, #4
00670cac: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00670cb0: mov      r0, r4
00670cb4: bl       #0x30e76c
00670cb8: cmp      r0, #0
00670cbc: beq      #0x670a78
00670cc0: add      r4, r4, #4
00670cc4: add      r2, r4, #0x450
00670cc8: mov      r3, r4
00670ccc: str      r5, [r3], #4
00670cd0: str      r5, [r4, #4]
00670cd4: add      r4, r4, #0xc
00670cd8: cmp      r4, r2
00670cdc: str      r5, [r3, #4]
00670ce0: bne      #0x670cc8
00670ce4: ldr      r0, [pc, #0x10]
00670ce8: add      r0, pc, r0
00670cec: bl       #0x30ea3c
00670cf0: b        #0x670a78
00670cf4: eorseq   r6, r8, r0, lsr #13
00670cf8: eorseq   r6, r8, r4, lsl #13
00670cfc: eorseq   r6, r8, r0, lsr #8

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
0061db54: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061db58: mov      r4, r1
0061db5c: mov      r1, #0
0061db60: mov      r5, r2
0061db64: mov      sb, r3
0061db68: mov      r8, r0
0061db6c: ldr      sl, [sp, #0x28]
0061db70: ldr      r7, [sp, #0x2c]
0061db74: bl       #0x669e24
0061db78: ldr      r6, [r0, #4]
0061db7c: ldr      fp, [r6, r4, lsl #2]
0061db80: ldr      r0, [r6, r5, lsl #2]
0061db84: mov      r1, fp
0061db88: bl       #0x30e3ac
0061db8c: mov      r1, fp
0061db90: mov      r4, r0
0061db94: ldr      r0, [r6, sb, lsl #2]
0061db98: bl       #0x30e3ac
0061db9c: mov      r6, r0
0061dba0: mov      r0, r8
0061dba4: bl       #0x669e54
0061dba8: cmp      r0, #0
0061dbac: bne      #0x61dbdc
0061dbb0: mov      r1, r4
0061dbb4: mov      r0, r6
0061dbb8: bl       #0x30e3ac
0061dbbc: mov      r1, r0
0061dbc0: mov      r0, sl
0061dbc4: bl       #0x30ed6c
0061dbc8: mov      r1, r0
0061dbcc: mov      r0, r4
0061dbd0: bl       #0x30eba4
0061dbd4: str      r0, [r7]
0061dbd8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061dbdc: mov      r0, r8
0061dbe0: bl       #0x669e68
0061dbe4: ldr      r2, [r0]
0061dbe8: mov      r5, r7
0061dbec: mov      r3, r0
0061dbf0: str      r2, [r5], #4
0061dbf4: ldr      r3, [r3, #4]
0061dbf8: mov      r1, r4
0061dbfc: mov      r0, r6
0061dc00: str      r3, [r7, #4]
0061dc04: bl       #0x30e3ac
0061dc08: mov      r1, r0
0061dc0c: mov      r0, sl
0061dc10: bl       #0x30ed6c
0061dc14: mov      r1, r0
0061dc18: mov      r0, r4
0061dc1c: bl       #0x30eba4
0061dc20: str      r0, [r5, #4]
0061dc24: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

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

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061dc4c: push     {r4, r5, r6, r7, r8, lr}
0061dc50: mov      r4, r1
0061dc54: mov      r1, #0
0061dc58: mov      r6, r2
0061dc5c: mov      r5, r0
0061dc60: bl       #0x669e24
0061dc64: ldr      r7, [r0, #4]
0061dc68: mov      r0, r5
0061dc6c: bl       #0x669e54
0061dc70: cmp      r0, #0
0061dc74: bne      #0x61dc84
0061dc78: ldr      r3, [r7, r4, lsl #2]
0061dc7c: str      r3, [r6]
0061dc80: pop      {r4, r5, r6, r7, r8, pc}
0061dc84: mov      r0, r5
0061dc88: bl       #0x669e68
0061dc8c: cmp      r0, #0
0061dc90: beq      #0x61dc78
0061dc94: mov      r0, r5
0061dc98: bl       #0x669e68
0061dc9c: ldr      r2, [r7, r4, lsl #2]
0061dca0: mov      r3, r6
0061dca4: str      r2, [r3], #4
0061dca8: ldr      r2, [r0, #4]
0061dcac: str      r2, [r6, #4]
0061dcb0: ldr      r2, [r0, #8]
0061dcb4: str      r2, [r3, #4]
0061dcb8: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionXExIfEELi0EfEEEEE13retrieveValueEPvSD_
006204fc: push     {r4, lr}
00620500: ldr      r3, [r1]
00620504: mov      r0, r1
00620508: mov      r4, r2
0062050c: mov      lr, pc
00620510: ldr      pc, [r3, #0xa0]
00620514: ldr      r3, [r0]
00620518: str      r3, [r4]
0062051c: ldr      r3, [r0, #4]
00620520: str      r3, [r4, #4]
00620524: ldr      r3, [r0, #8]
00620528: str      r3, [r4, #8]
0062052c: pop      {r4, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061fc70: push     {r4, r5, r6, r7, r8, lr}
0061fc74: mov      r4, r1
0061fc78: mov      r1, #0
0061fc7c: mov      r6, r2
0061fc80: mov      r5, r0
0061fc84: bl       #0x669e24
0061fc88: ldr      r7, [r0, #4]
0061fc8c: mov      r0, r5
0061fc90: bl       #0x669e54
0061fc94: cmp      r0, #0
0061fc98: bne      #0x61fca8
0061fc9c: ldr      r3, [r7, r4, lsl #2]
0061fca0: str      r3, [r6]
0061fca4: pop      {r4, r5, r6, r7, r8, pc}
0061fca8: mov      r0, r5
0061fcac: bl       #0x669e68
0061fcb0: cmp      r0, #0
0061fcb4: beq      #0x61fc9c
0061fcb8: mov      r0, r5
0061fcbc: bl       #0x669e68
0061fcc0: ldr      r2, [r0]
0061fcc4: mov      r3, r6
0061fcc8: str      r2, [r3], #4
0061fccc: ldr      r2, [r7, r4, lsl #2]
0061fcd0: str      r2, [r6, #4]
0061fcd4: ldr      r2, [r0, #8]
0061fcd8: str      r2, [r3, #4]
0061fcdc: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
0061fdc4: push     {r4, r5, r6, r7, r8, lr}
0061fdc8: mov      r4, r1
0061fdcc: mov      r1, #0
0061fdd0: mov      r5, r2
0061fdd4: mov      r6, r3
0061fdd8: mov      r7, r0
0061fddc: bl       #0x669e24
0061fde0: ldr      r3, [r0, #4]
0061fde4: ldr      r1, [r3, r4, lsl #2]
0061fde8: ldr      r0, [r3, r5, lsl #2]
0061fdec: bl       #0x30e3ac
0061fdf0: mov      r4, r0
0061fdf4: mov      r0, r7
0061fdf8: bl       #0x669e54
0061fdfc: cmp      r0, #0
0061fe00: bne      #0x61fe0c
0061fe04: str      r4, [r6]
0061fe08: pop      {r4, r5, r6, r7, r8, pc}
0061fe0c: mov      r0, r7
0061fe10: bl       #0x669e68
0061fe14: ldr      r2, [r0]
0061fe18: mov      r3, r6
0061fe1c: str      r2, [r3], #4
0061fe20: str      r4, [r6, #4]
0061fe24: ldr      r2, [r0, #8]
0061fe28: str      r2, [r3, #4]
0061fe2c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleZExIfEEfLi3ENS1_17SUseDefaultValuesILi2EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061e264: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061e268: mov      r4, r1
0061e26c: mov      r1, #0
0061e270: mov      r5, r2
0061e274: mov      r8, r3
0061e278: mov      r7, r0
0061e27c: ldr      r6, [sp, #0x20]
0061e280: bl       #0x669e24
0061e284: ldr      sl, [r0, #4]
0061e288: mov      r0, r7
0061e28c: bl       #0x669e54
0061e290: cmp      r0, #0
0061e294: beq      #0x61e2e8
0061e298: mov      r0, r7
0061e29c: bl       #0x669e68
0061e2a0: ldr      r3, [r0]
0061e2a4: mov      r0, r7
0061e2a8: str      r3, [r6]
0061e2ac: bl       #0x669e68
0061e2b0: ldr      r3, [r0, #4]
0061e2b4: str      r3, [r6, #4]
0061e2b8: ldr      r4, [sl, r4, lsl #2]
0061e2bc: ldr      r0, [sl, r5, lsl #2]
0061e2c0: mov      r1, r4
0061e2c4: bl       #0x30e3ac
0061e2c8: mov      r1, r0
0061e2cc: mov      r0, r8
0061e2d0: bl       #0x30ed6c
0061e2d4: mov      r1, r0
0061e2d8: mov      r0, r4
0061e2dc: bl       #0x30eba4
0061e2e0: str      r0, [r6, #8]
0061e2e4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061e2e8: ldr      r4, [sl, r4, lsl #2]
0061e2ec: ldr      r0, [sl, r5, lsl #2]
0061e2f0: mov      r1, r4
0061e2f4: bl       #0x30e3ac
0061e2f8: mov      r1, r0
0061e2fc: mov      r0, r8
0061e300: bl       #0x30ed6c
0061e304: mov      r1, r0
0061e308: mov      r0, r4
0061e30c: bl       #0x30eba4
0061e310: str      r0, [r6]
0061e314: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
0061fe44: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061fe48: mov      r4, r1
0061fe4c: mov      r1, #0
0061fe50: mov      sb, r3
0061fe54: mov      r5, r2
0061fe58: mov      r8, r0
0061fe5c: ldr      sl, [sp, #0x28]
0061fe60: ldr      r7, [sp, #0x2c]
0061fe64: bl       #0x669e24
0061fe68: ldr      r6, [r0, #4]
0061fe6c: ldr      fp, [r6, r4, lsl #2]
0061fe70: ldr      r0, [r6, r5, lsl #2]
0061fe74: mov      r1, fp
0061fe78: bl       #0x30e3ac
0061fe7c: mov      r1, fp
0061fe80: mov      r4, r0
0061fe84: ldr      r0, [r6, sb, lsl #2]
0061fe88: bl       #0x30e3ac
0061fe8c: mov      sb, r0
0061fe90: mov      r0, r8
0061fe94: bl       #0x669e54
0061fe98: cmp      r0, #0
0061fe9c: bne      #0x61fecc
0061fea0: mov      r1, r4
0061fea4: mov      r0, sb
0061fea8: bl       #0x30e3ac
0061feac: mov      r1, r0
0061feb0: mov      r0, sl
0061feb4: bl       #0x30ed6c
0061feb8: mov      r1, r0
0061febc: mov      r0, r4
0061fec0: bl       #0x30eba4
0061fec4: str      r0, [r7]
0061fec8: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061fecc: mov      r0, r8
0061fed0: bl       #0x669e68
0061fed4: ldr      r3, [r0]
0061fed8: mov      r5, r7
0061fedc: mov      r6, r0
0061fee0: str      r3, [r5], #4
0061fee4: mov      r1, r4
0061fee8: mov      r0, sb
0061feec: bl       #0x30e3ac
0061fef0: mov      r1, r0
0061fef4: mov      r0, sl
0061fef8: bl       #0x30ed6c
0061fefc: mov      r1, r0
0061ff00: mov      r0, r4
0061ff04: bl       #0x30eba4
0061ff08: str      r0, [r7, #4]
0061ff0c: ldr      r3, [r6, #8]
0061ff10: str      r3, [r5, #4]
0061ff14: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleZExIfEELi2EfEEEEE10applyValueEPvSD_PNS1_15CApplicatorInfoE
00622bf8: push     {r4, lr}
00622bfc: mov      r0, r2
00622c00: ldr      r3, [r2]
00622c04: mov      lr, pc
00622c08: ldr      pc, [r3, #0x94]
00622c0c: pop      {r4, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionYExIfEELi1EfEEEEE12getValueSizeEv
0060efc8: mov      r0, #0xc
0060efcc: bx       lr

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
0061fb78: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061fb7c: mov      r4, r1
0061fb80: mov      r1, #0
0061fb84: mov      r5, r2
0061fb88: mov      sb, r3
0061fb8c: mov      r8, r0
0061fb90: ldr      sl, [sp, #0x28]
0061fb94: ldr      r7, [sp, #0x2c]
0061fb98: bl       #0x669e24
0061fb9c: ldr      r6, [r0, #4]
0061fba0: ldr      fp, [r6, r4, lsl #2]
0061fba4: ldr      r0, [r6, r5, lsl #2]
0061fba8: mov      r1, fp
0061fbac: bl       #0x30e3ac
0061fbb0: mov      r1, fp
0061fbb4: mov      r4, r0
0061fbb8: ldr      r0, [r6, sb, lsl #2]
0061fbbc: bl       #0x30e3ac
0061fbc0: mov      r6, r0
0061fbc4: mov      r0, r8
0061fbc8: bl       #0x669e54
0061fbcc: cmp      r0, #0
0061fbd0: bne      #0x61fc00
0061fbd4: mov      r1, r4
0061fbd8: mov      r0, r6
0061fbdc: bl       #0x30e3ac
0061fbe0: mov      r1, r0
0061fbe4: mov      r0, sl
0061fbe8: bl       #0x30ed6c
0061fbec: mov      r1, r0
0061fbf0: mov      r0, r4
0061fbf4: bl       #0x30eba4
0061fbf8: str      r0, [r7]
0061fbfc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061fc00: mov      r0, r8
0061fc04: bl       #0x669e68
0061fc08: mov      r1, r4
0061fc0c: mov      r5, r0
0061fc10: mov      r0, r6
0061fc14: bl       #0x30e3ac
0061fc18: mov      r1, r0
0061fc1c: mov      r0, sl
0061fc20: bl       #0x30ed6c
0061fc24: mov      r1, r0
0061fc28: mov      r0, r4
0061fc2c: bl       #0x30eba4
0061fc30: mov      r3, r7
0061fc34: str      r0, [r3], #4
0061fc38: ldr      r2, [r5, #4]
0061fc3c: str      r2, [r7, #4]
0061fc40: ldr      r2, [r5, #8]
0061fc44: str      r2, [r3, #4]
0061fc48: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_32CSceneNodePositionComponentMixinINS1_21CSceneNodePositionZExIfEELi2EfEEEEE12getValueSizeEv
0060efb0: mov      r0, #0xc
0060efb4: bx       lr

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
0061faf8: push     {r4, r5, r6, r7, r8, lr}
0061fafc: mov      r4, r1
0061fb00: mov      r1, #0
0061fb04: mov      r5, r2
0061fb08: mov      r6, r3
0061fb0c: mov      r7, r0
0061fb10: bl       #0x669e24
0061fb14: ldr      r3, [r0, #4]
0061fb18: ldr      r1, [r3, r4, lsl #2]
0061fb1c: ldr      r0, [r3, r5, lsl #2]
0061fb20: bl       #0x30e3ac
0061fb24: mov      r4, r0
0061fb28: mov      r0, r7
0061fb2c: bl       #0x669e54
0061fb30: cmp      r0, #0
0061fb34: bne      #0x61fb40
0061fb38: str      r4, [r6]
0061fb3c: pop      {r4, r5, r6, r7, r8, pc}
0061fb40: mov      r0, r7
0061fb44: bl       #0x669e68
0061fb48: mov      r3, r6
0061fb4c: str      r4, [r3], #4
0061fb50: ldr      r2, [r0, #4]
0061fb54: str      r2, [r6, #4]
0061fb58: ldr      r2, [r0, #8]
0061fb5c: str      r2, [r3, #4]
0061fb60: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_29CSceneNodeScaleComponentMixinINS1_18CSceneNodeScaleXExIfEELi0EfEEEEE13retrieveValueEPvSD_
00620738: push     {r4, lr}
0062073c: ldr      r3, [r1]
00620740: mov      r0, r1
00620744: mov      r4, r2
00620748: mov      lr, pc
0062074c: ldr      pc, [r3, #0x90]
00620750: ldr      r3, [r0]
00620754: str      r3, [r4]
00620758: ldr      r3, [r0, #4]
0062075c: str      r3, [r4, #4]
00620760: ldr      r3, [r0, #8]
00620764: str      r3, [r4, #8]
00620768: pop      {r4, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_21CSceneNodePositionXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061f9a4: push     {r4, r5, r6, r7, r8, lr}
0061f9a8: mov      r4, r1
0061f9ac: mov      r1, #0
0061f9b0: mov      r6, r2
0061f9b4: mov      r5, r0
0061f9b8: bl       #0x669e24
0061f9bc: ldr      r7, [r0, #4]
0061f9c0: mov      r0, r5
0061f9c4: bl       #0x669e54
0061f9c8: cmp      r0, #0
0061f9cc: bne      #0x61f9dc
0061f9d0: ldr      r3, [r7, r4, lsl #2]
0061f9d4: str      r3, [r6]
0061f9d8: pop      {r4, r5, r6, r7, r8, pc}
0061f9dc: mov      r0, r5
0061f9e0: bl       #0x669e68
0061f9e4: cmp      r0, #0
0061f9e8: beq      #0x61f9d0
0061f9ec: mov      r0, r5
0061f9f0: bl       #0x669e68
0061f9f4: ldr      r2, [r7, r4, lsl #2]
0061f9f8: mov      r3, r6
0061f9fc: str      r2, [r3], #4
0061fa00: ldr      r2, [r0, #4]
0061fa04: str      r2, [r6, #4]
0061fa08: ldr      r2, [r0, #8]
0061fa0c: str      r2, [r3, #4]
0061fa10: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061df18: push     {r4, r5, r6, r7, r8, lr}
0061df1c: mov      r4, r1
0061df20: mov      r1, #0
0061df24: mov      r6, r2
0061df28: mov      r5, r0
0061df2c: bl       #0x669e24
0061df30: ldr      r7, [r0, #4]
0061df34: mov      r0, r5
0061df38: bl       #0x669e54
0061df3c: cmp      r0, #0
0061df40: bne      #0x61df50
0061df44: ldr      r3, [r7, r4, lsl #2]
0061df48: str      r3, [r6]
0061df4c: pop      {r4, r5, r6, r7, r8, pc}
0061df50: mov      r0, r5
0061df54: bl       #0x669e68
0061df58: cmp      r0, #0
0061df5c: beq      #0x61df44
0061df60: mov      r0, r5
0061df64: bl       #0x669e68
0061df68: ldr      r2, [r0]
0061df6c: mov      r3, r6
0061df70: str      r2, [r3], #4
0061df74: ldr      r2, [r7, r4, lsl #2]
0061df78: str      r2, [r6, #4]
0061df7c: ldr      r2, [r0, #8]
0061df80: str      r2, [r3, #4]
0061df84: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleYExIfEEfLi3ENS1_17SUseDefaultValuesILi1EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiPv
0061e06c: push     {r4, r5, r6, r7, r8, lr}
0061e070: mov      r4, r1
0061e074: mov      r1, #0
0061e078: mov      r5, r2
0061e07c: mov      r6, r3
0061e080: mov      r7, r0
0061e084: bl       #0x669e24
0061e088: ldr      r3, [r0, #4]
0061e08c: ldr      r1, [r3, r4, lsl #2]
0061e090: ldr      r0, [r3, r5, lsl #2]
0061e094: bl       #0x30e3ac
0061e098: mov      r4, r0
0061e09c: mov      r0, r7
0061e0a0: bl       #0x669e54
0061e0a4: cmp      r0, #0
0061e0a8: bne      #0x61e0b4
0061e0ac: str      r4, [r6]
0061e0b0: pop      {r4, r5, r6, r7, r8, pc}
0061e0b4: mov      r0, r7
0061e0b8: bl       #0x669e68
0061e0bc: ldr      r2, [r0]
0061e0c0: mov      r3, r6
0061e0c4: str      r2, [r3], #4
0061e0c8: str      r4, [r6, #4]
0061e0cc: ldr      r2, [r0, #8]
0061e0d0: str      r2, [r3, #4]
0061e0d4: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch7collada16CColladaDatabase12getAnimationEPKcNS0_8SChannel4TypeEh
0061c0c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c0cc: mov      r4, r0
0061c0d0: ldr      r0, [r0]
0061c0d4: mov      r6, r2
0061c0d8: mov      sb, r3
0061c0dc: ldr      r2, [r0, #0x24]
0061c0e0: mov      r5, r1
0061c0e4: ldr      r3, [r2, #0x20]
0061c0e8: ldr      sl, [r3, #0x24]
0061c0ec: cmp      sl, #0
0061c0f0: ble      #0x61c1ac
0061c0f4: mov      r7, #0
0061c0f8: b        #0x61c12c
0061c0fc: cmp      r6, #1
0061c100: blo      #0x61c114
0061c104: cmp      r6, #4
0061c108: bls      #0x61c190
0061c10c: cmp      r6, #5
0061c110: beq      #0x61c1b8
0061c114: ldr      r2, [r3, #8]
0061c118: cmp      r2, r6
0061c11c: beq      #0x61c1d0
0061c120: add      r7, r7, #1
0061c124: cmp      r7, sl
0061c128: beq      #0x61c1ac
0061c12c: mov      r0, r4
0061c130: mov      r1, r7
0061c134: bl       #0x60e35c
0061c138: cmp      r6, #9
0061c13c: mov      r8, r0
0061c140: ldr      r3, [r0, #0x10]
0061c144: beq      #0x61c1b8
0061c148: bls      #0x61c0fc
0061c14c: cmp      r6, #0x57
0061c150: blo      #0x61c114
0061c154: cmp      r6, #0x5b
0061c158: bls      #0x61c164
0061c15c: cmp      r6, #0x100
0061c160: bne      #0x61c114
0061c164: ldr      r2, [r3, #8]
0061c168: sub      r2, r2, #0x57
0061c16c: cmp      r2, #4
0061c170: bhi      #0x61c120
0061c174: ldr      r0, [r3, #4]
0061c178: mov      r1, r5
0061c17c: bl       #0x30e31c
0061c180: cmp      r0, #0
0061c184: bne      #0x61c120
0061c188: mov      r0, r8
0061c18c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c190: ldr      r2, [r3, #8]
0061c194: sub      r2, r2, #1
0061c198: cmp      r2, #3
0061c19c: bls      #0x61c174
0061c1a0: add      r7, r7, #1
0061c1a4: cmp      r7, sl
0061c1a8: bne      #0x61c12c
0061c1ac: mov      r8, #0
0061c1b0: mov      r0, r8
0061c1b4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c1b8: ldr      r2, [r3, #8]
0061c1bc: cmp      r2, #5
0061c1c0: beq      #0x61c174
0061c1c4: cmp      r2, #9
0061c1c8: bne      #0x61c120
0061c1cc: b        #0x61c174
0061c1d0: ldrb     r2, [r3, #0xc]
0061c1d4: cmp      r2, sb
0061c1d8: bne      #0x61c120
0061c1dc: b        #0x61c174

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

# _ZN6glitch7collada15animation_track12CInterpreterINS1_18CSceneNodeScaleXExIfEEfLi3ENS1_17SUseDefaultValuesILi0EfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061dccc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061dcd0: mov      r4, r1
0061dcd4: mov      r1, #0
0061dcd8: mov      r5, r2
0061dcdc: mov      r8, r3
0061dce0: mov      r6, r0
0061dce4: ldr      r7, [sp, #0x20]
0061dce8: bl       #0x669e24
0061dcec: ldr      sb, [r0, #4]
0061dcf0: mov      r0, r6
0061dcf4: bl       #0x669e54
0061dcf8: cmp      r0, #0
0061dcfc: beq      #0x61dd54
0061dd00: ldr      sl, [sb, r4, lsl #2]
0061dd04: ldr      r0, [sb, r5, lsl #2]
0061dd08: mov      r4, r7
0061dd0c: mov      r1, sl
0061dd10: bl       #0x30e3ac
0061dd14: mov      r1, r0
0061dd18: mov      r0, r8
0061dd1c: bl       #0x30ed6c
0061dd20: mov      r1, r0
0061dd24: mov      r0, sl
0061dd28: bl       #0x30eba4
0061dd2c: str      r0, [r4], #4
0061dd30: mov      r0, r6
0061dd34: bl       #0x669e68
0061dd38: ldr      r3, [r0, #4]
0061dd3c: mov      r0, r6
0061dd40: str      r3, [r7, #4]
0061dd44: bl       #0x669e68
0061dd48: ldr      r3, [r0, #8]
0061dd4c: str      r3, [r4, #4]
0061dd50: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061dd54: ldr      r4, [sb, r4, lsl #2]
0061dd58: ldr      r0, [sb, r5, lsl #2]
0061dd5c: mov      r1, r4
0061dd60: bl       #0x30e3ac
0061dd64: mov      r1, r0
0061dd68: mov      r0, r8
0061dd6c: bl       #0x30ed6c
0061dd70: mov      r1, r0
0061dd74: mov      r0, r4
0061dd78: bl       #0x30eba4
0061dd7c: str      r0, [r7]
0061dd80: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
