
# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIfNS1_6CMixinIfLi1ENS1_17SMaterialSetParamINS1_15SAnimationTypesIffEEEELin1EfEEEEE15getBlendedValueEPvPfiSC_
00620134: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00620138: cmp      r3, #1
0062013c: mov      r4, r3
00620140: mov      r5, r1
00620144: mov      r8, r2
00620148: ldr      sl, [sp, #0x20]
0062014c: beq      #0x620194
00620150: cmp      r3, #0
00620154: moveq    r7, #0
00620158: beq      #0x62018c
0062015c: mov      r7, #0
00620160: mov      r6, #0
00620164: ldr      r1, [r8, r6]
00620168: ldr      r0, [r5, r6]
0062016c: bl       #0x30ed6c
00620170: mov      r1, r0
00620174: mov      r0, r7
00620178: bl       #0x30eba4
0062017c: subs     r4, r4, #1
00620180: mov      r7, r0
00620184: add      r6, r6, #4
00620188: bne      #0x620164
0062018c: str      r7, [sl]
00620190: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00620194: ldr      r3, [r1]
00620198: str      r3, [sl]
0062019c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_20CSceneNodeScaleMixinIfEEEEE15getBlendedValueEPvPfiSB_
006275fc: cmp      r3, #1
00627600: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00627604: mov      r4, r3
00627608: mov      fp, r2
0062760c: beq      #0x6276b8
00627610: cmp      r3, #0
00627614: moveq    r8, #0
00627618: moveq    sb, r8
0062761c: moveq    sl, r8
00627620: beq      #0x6276a0
00627624: mov      r8, #0
00627628: mov      r5, r1
0062762c: mov      r7, #0
00627630: mov      sb, r8
00627634: mov      sl, r8
00627638: ldr      r6, [fp, r7]
0062763c: ldr      r1, [r5]
00627640: add      r7, r7, #4
00627644: mov      r0, r6
00627648: bl       #0x30ed6c
0062764c: mov      r1, r0
00627650: mov      r0, r8
00627654: bl       #0x30eba4
00627658: ldr      r1, [r5, #4]
0062765c: mov      r8, r0
00627660: mov      r0, r6
00627664: bl       #0x30ed6c
00627668: mov      r1, r0
0062766c: mov      r0, sb
00627670: bl       #0x30eba4
00627674: ldr      r1, [r5, #8]
00627678: mov      sb, r0
0062767c: mov      r0, r6
00627680: bl       #0x30ed6c
00627684: mov      r1, r0
00627688: mov      r0, sl
0062768c: bl       #0x30eba4
00627690: subs     r4, r4, #1
00627694: mov      sl, r0
00627698: add      r5, r5, #0xc
0062769c: bne      #0x627638
006276a0: ldr      r3, [sp, #0x28]
006276a4: str      r8, [r3], #4
006276a8: ldr      r2, [sp, #0x28]
006276ac: str      sb, [r2, #4]
006276b0: str      sl, [r3, #4]
006276b4: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
006276b8: mov      r2, r1
006276bc: ldr      r0, [r2], #4
006276c0: ldr      r3, [sp, #0x28]
006276c4: str      r0, [r3], #4
006276c8: ldr      r1, [r1, #4]
006276cc: ldr      r0, [sp, #0x28]
006276d0: str      r1, [r0, #4]
006276d4: ldr      r2, [r2, #4]
006276d8: str      r2, [r3, #4]
006276dc: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExINS_4core8vector3dIfEENS1_23CSceneNodePositionMixinIfEEEEE15getBlendedValueEPvPfiSB_
00626b4c: cmp      r3, #1
00626b50: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00626b54: mov      r4, r3
00626b58: mov      fp, r2
00626b5c: beq      #0x626c08
00626b60: cmp      r3, #0
00626b64: moveq    r8, #0
00626b68: moveq    sb, r8
00626b6c: moveq    sl, r8
00626b70: beq      #0x626bf0
00626b74: mov      r8, #0
00626b78: mov      r5, r1
00626b7c: mov      r7, #0
00626b80: mov      sb, r8
00626b84: mov      sl, r8
00626b88: ldr      r6, [fp, r7]
00626b8c: ldr      r1, [r5]
00626b90: add      r7, r7, #4
00626b94: mov      r0, r6
00626b98: bl       #0x30ed6c
00626b9c: mov      r1, r0
00626ba0: mov      r0, r8
00626ba4: bl       #0x30eba4
00626ba8: ldr      r1, [r5, #4]
00626bac: mov      r8, r0
00626bb0: mov      r0, r6
00626bb4: bl       #0x30ed6c
00626bb8: mov      r1, r0
00626bbc: mov      r0, sb
00626bc0: bl       #0x30eba4
00626bc4: ldr      r1, [r5, #8]
00626bc8: mov      sb, r0
00626bcc: mov      r0, r6
00626bd0: bl       #0x30ed6c
00626bd4: mov      r1, r0
00626bd8: mov      r0, sl
00626bdc: bl       #0x30eba4
00626be0: subs     r4, r4, #1
00626be4: mov      sl, r0
00626be8: add      r5, r5, #0xc
00626bec: bne      #0x626b88
00626bf0: ldr      r3, [sp, #0x28]
00626bf4: str      r8, [r3], #4
00626bf8: ldr      r2, [sp, #0x28]
00626bfc: str      sb, [r2, #4]
00626c00: str      sl, [r3, #4]
00626c04: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00626c08: mov      r2, r1
00626c0c: ldr      r0, [r2], #4
00626c10: ldr      r3, [sp, #0x28]
00626c14: str      r0, [r3], #4
00626c18: ldr      r1, [r1, #4]
00626c1c: ldr      r0, [sp, #0x28]
00626c20: str      r1, [r0, #4]
00626c24: ldr      r2, [r2, #4]
00626c28: str      r2, [r3, #4]
00626c2c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

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

# _ZN6glitch4core10quaternion9normalizeEv
0035c8f0: push     {r4, r5, r6, r7, r8, lr}
0035c8f4: mov      r4, r0
0035c8f8: ldr      r0, [r0]
0035c8fc: ldr      r7, [r4, #4]
0035c900: ldr      r6, [r4, #8]
0035c904: mov      r1, r0
0035c908: bl       #0x30ed6c
0035c90c: mov      r1, r7
0035c910: mov      r5, r0
0035c914: mov      r0, r7
0035c918: bl       #0x30ed6c
0035c91c: mov      r1, r0
0035c920: mov      r0, r5
0035c924: bl       #0x30eba4
0035c928: mov      r1, r6
0035c92c: mov      r5, r0
0035c930: mov      r0, r6
0035c934: bl       #0x30ed6c
0035c938: mov      r1, r0
0035c93c: mov      r0, r5
0035c940: bl       #0x30eba4
0035c944: ldr      r6, [r4, #0xc]
0035c948: mov      r5, r0
0035c94c: mov      r1, r6
0035c950: mov      r0, r6
0035c954: bl       #0x30ed6c
0035c958: mov      r1, r0
0035c95c: mov      r0, r5
0035c960: bl       #0x30eba4
0035c964: mov      r1, #0x3f800000
0035c968: mov      r5, r0
0035c96c: bl       #0x30df8c
0035c970: cmp      r0, #0
0035c974: bne      #0x35c9d0
0035c978: mov      r0, r5
0035c97c: bl       #0x30e124
0035c980: mov      r1, r0
0035c984: mov      r0, #0x3f800000
0035c988: bl       #0x30ec94
0035c98c: mov      r5, r0
0035c990: mov      r1, r0
0035c994: ldr      r0, [r4]
0035c998: bl       #0x30ed6c
0035c99c: mov      r1, r5
0035c9a0: str      r0, [r4]
0035c9a4: ldr      r0, [r4, #4]
0035c9a8: bl       #0x30ed6c
0035c9ac: mov      r1, r5
0035c9b0: str      r0, [r4, #4]
0035c9b4: ldr      r0, [r4, #8]
0035c9b8: bl       #0x30ed6c
0035c9bc: mov      r1, r5
0035c9c0: str      r0, [r4, #8]
0035c9c4: ldr      r0, [r4, #0xc]
0035c9c8: bl       #0x30ed6c
0035c9cc: str      r0, [r4, #0xc]
0035c9d0: mov      r0, r4
0035c9d4: pop      {r4, r5, r6, r7, r8, pc}

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
