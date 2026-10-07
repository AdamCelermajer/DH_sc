
# _ZNK13ItemInventory6HasBowEv
00400080: push     {r4, lr}
00400084: mov      r1, #1
00400088: mov      r4, r0
0040008c: bl       #0x3fc6a8
00400090: mov      r3, #0xc
00400094: mul      r3, r3, r0
00400098: ldr      r2, [r4, #0x14]
0040009c: ldr      r3, [r2, r3]
004000a0: ldr      r0, [r3, #4]
004000a4: cmp      r0, #0
004000a8: beq      #0x4000c4
004000ac: ldr      r0, [r0]
004000b0: bl       #0x3f9e08
004000b4: ldr      r0, [r0, #0x94]
004000b8: cmp      r0, #4
004000bc: movne    r0, #0
004000c0: moveq    r0, #1
004000c4: pop      {r4, pc}

# _ZNK13ItemInventory17HasMainHandWeaponEv
003ffe8c: push     {r4, lr}
003ffe90: mov      r1, #1
003ffe94: mov      r4, r0
003ffe98: bl       #0x3fc6a8
003ffe9c: mov      r3, #0xc
003ffea0: mul      r3, r3, r0
003ffea4: ldr      r2, [r4, #0x14]
003ffea8: ldr      r3, [r2, r3]
003ffeac: ldr      r0, [r3, #4]
003ffeb0: subs     r0, r0, #0
003ffeb4: movne    r0, #1
003ffeb8: pop      {r4, pc}

# _ZNK9Character7HasManaEi
003bd40c: push     {r4, r5, r6, lr}
003bd410: sub      sp, sp, #8
003bd414: mov      r4, r0
003bd418: mov      r5, r1
003bd41c: bl       #0x7fd794
003bd420: ldrb     r3, [r0, #5]
003bd424: ldr      r6, [pc, #0xb4]
003bd428: cmp      r3, #0
003bd42c: add      r6, pc, r6
003bd430: bne      #0x3bd464
003bd434: cmp      r5, #0
003bd438: blt      #0x3bd488
003bd43c: add      r1, r4, #0xff0
003bd440: add      r1, r1, #4
003bd444: add      r0, r4, #0x560
003bd448: mov      r2, #0x29
003bd44c: bl       #0x3dedb4
003bd450: cmp      r5, r0
003bd454: movgt    r0, #0
003bd458: movle    r0, #1
003bd45c: add      sp, sp, #8
003bd460: pop      {r4, r5, r6, pc}
003bd464: ldr      r3, [r4]
003bd468: mov      r0, r4
003bd46c: mov      lr, pc
003bd470: ldr      pc, [r3, #0x54]
003bd474: cmp      r0, #0
003bd478: movne    r0, #1
003bd47c: bne      #0x3bd45c
003bd480: cmp      r5, #0
003bd484: bge      #0x3bd43c
003bd488: ldr      r3, [pc, #0x54]
003bd48c: ldr      r3, [r6, r3]
003bd490: ldr      r3, [r3]
003bd494: cmp      r3, #2
003bd498: moveq    r3, #0
003bd49c: streq    r3, [r3]
003bd4a0: beq      #0x3bd43c
003bd4a4: cmp      r3, #1
003bd4a8: bne      #0x3bd43c
003bd4ac: ldr      r0, [pc, #0x34]
003bd4b0: ldr      r1, [pc, #0x34]
003bd4b4: ldr      r2, [pc, #0x34]
003bd4b8: ldr      r0, [r6, r0]
003bd4bc: ldr      r3, [pc, #0x30]
003bd4c0: mov      ip, #0x91
003bd4c4: add      r1, pc, r1
003bd4c8: add      r2, pc, r2
003bd4cc: add      r3, pc, r3
003bd4d0: add      r0, r0, #0xa8
003bd4d4: str      ip, [sp]
003bd4d8: bl       #0x30e004
003bd4dc: b        #0x3bd43c
003bd4e0: subseq   r7, sp, r4, ror #12
003bd4e4: andeq    r3, r0, r0, asr #19
003bd4e8: andeq    r1, r0, r0, asr #19
003bd4ec: subseq   r0, r0, r4, lsl pc
003bd4f0: subseq   r7, r0, r8, lsr #8
003bd4f4: subseq   r7, r0, r4, lsr r4

# _ZNK13ItemInventory9HasShieldEv
00400110: push     {r4, lr}
00400114: mov      r1, #2
00400118: mov      r4, r0
0040011c: bl       #0x3fc6a8
00400120: mov      r3, #0xc
00400124: mul      r3, r3, r0
00400128: ldr      r2, [r4, #0x14]
0040012c: ldr      r3, [r2, r3]
00400130: ldr      r0, [r3, #8]
00400134: cmp      r0, #0
00400138: beq      #0x400154
0040013c: ldr      r0, [r0]
00400140: bl       #0x3f9e08
00400144: ldr      r0, [r0, #0x58]
00400148: cmp      r0, #6
0040014c: movne    r0, #0
00400150: moveq    r0, #1
00400154: pop      {r4, pc}

# _ZN10GameObject21_TargetListSearchRectERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00392b80: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00392b84: ldr      r7, [r0, #4]
00392b88: mov      r6, r2
00392b8c: ldr      r4, [pc, #0x758]
00392b90: ldm      r7, {r1, r3}
00392b94: add      r4, pc, r4
00392b98: sub      sp, sp, #0x3c
00392b9c: rsb      r3, r1, r3
00392ba0: asr      r3, r3, #4
00392ba4: mov      r5, r0
00392ba8: add      r2, r3, r3, lsl #3
00392bac: add      r2, r2, r2, lsl #6
00392bb0: add      r2, r3, r2, lsl #3
00392bb4: add      r2, r2, r2, lsl #15
00392bb8: add      r3, r3, r2, lsl #3
00392bbc: rsb      r3, r3, #0
00392bc0: cmp      r3, #1
00392bc4: bls      #0x392bdc
00392bc8: cmp      r3, #0
00392bcc: beq      #0x392be4
00392bd0: ldr      r3, [r1, #4]
00392bd4: cmp      r3, #3
00392bd8: beq      #0x392c00
00392bdc: add      sp, sp, #0x3c
00392be0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00392be4: ldr      r0, [pc, #0x704]
00392be8: add      r0, pc, r0
00392bec: bl       #0x708eb0
00392bf0: ldr      r1, [r7]
00392bf4: ldr      r3, [r1, #4]
00392bf8: cmp      r3, #3
00392bfc: bne      #0x392bdc
00392c00: ldr      r7, [r5, #4]
00392c04: ldm      r7, {r2, r3}
00392c08: rsb      r3, r2, r3
00392c0c: asr      r3, r3, #4
00392c10: add      r1, r3, r3, lsl #3
00392c14: add      r1, r1, r1, lsl #6
00392c18: add      r1, r3, r1, lsl #3
00392c1c: add      r1, r1, r1, lsl #15
00392c20: add      r3, r3, r1, lsl #3
00392c24: rsb      r3, r3, #0
00392c28: cmp      r3, #1
00392c2c: bhi      #0x392c40
00392c30: ldr      r0, [pc, #0x6bc]
00392c34: add      r0, pc, r0
00392c38: bl       #0x708eb0
00392c3c: ldr      r2, [r7]
00392c40: ldr      r7, [r2, #0x74]
00392c44: cmp      r7, #3
00392c48: bne      #0x392bdc
00392c4c: ldr      r2, [r5, #4]
00392c50: ldr      r3, [r6, #0x168]
00392c54: ldr      r0, [r6, #0x160]
00392c58: ldr      r1, [r6, #0x164]
00392c5c: ldm      r2, {r8, sl}
00392c60: str      r0, [sp, #0x2c]
00392c64: str      r1, [sp, #0x30]
00392c68: str      r3, [sp, #0x34]
00392c6c: ldr      r3, [r2]
00392c70: ldr      r2, [r2, #4]
00392c74: rsb      r3, r3, r2
00392c78: asr      r3, r3, #4
00392c7c: add      r2, r3, r3, lsl #3
00392c80: add      r2, r2, r2, lsl #6
00392c84: add      r2, r3, r2, lsl #3
00392c88: add      r2, r2, r2, lsl #15
00392c8c: add      r3, r3, r2, lsl #3
00392c90: rsb      r3, r3, #0
00392c94: cmp      r3, #2
00392c98: bhi      #0x392e4c
00392c9c: rsb      r8, r8, sl
00392ca0: asr      r8, r8, #4
00392ca4: add      r7, r8, r8, lsl #3
00392ca8: add      r7, r7, r7, lsl #6
00392cac: add      r7, r8, r7, lsl #3
00392cb0: add      r7, r7, r7, lsl #15
00392cb4: add      r7, r8, r7, lsl #3
00392cb8: mvn      r7, r7
00392cbc: cmp      r7, r3
00392cc0: blo      #0x392d88
00392cc4: ldr      r3, [r6, #0x338]
00392cc8: ands     r3, r3, #0x80
00392ccc: beq      #0x392d3c
00392cd0: ldr      r2, [pc, #0x620]
00392cd4: ldr      ip, [pc, #0x620]
00392cd8: mov      r3, #0
00392cdc: ldr      r2, [r4, r2]
00392ce0: ldr      ip, [r4, ip]
00392ce4: mov      r1, r3
00392ce8: ldr      r2, [r2, #0x40]
00392cec: add      ip, ip, #8
00392cf0: mov      r0, r5
00392cf4: str      ip, [sp, #0x18]
00392cf8: str      r2, [sp, #0x1c]
00392cfc: str      r3, [sp, #0x20]
00392d00: bl       #0x37baf8
00392d04: bl       #0x31bbf0
00392d08: mov      r1, #1
00392d0c: mov      r4, r0
00392d10: mov      r0, r5
00392d14: bl       #0x37baf8
00392d18: bl       #0x31bbf0
00392d1c: add      ip, sp, #0x18
00392d20: mov      r3, r0
00392d24: mov      r2, r4
00392d28: add      r0, r6, #0x304
00392d2c: add      r1, sp, #0x2c
00392d30: str      ip, [sp]
00392d34: bl       #0x4a2e70
00392d38: b        #0x392bdc
00392d3c: ldr      r2, [r6, #0x33c]
00392d40: cmp      r2, #2
00392d44: beq      #0x392ee0
00392d48: ldr      r0, [pc, #0x5a8]
00392d4c: ldr      r2, [pc, #0x5ac]
00392d50: mov      r1, r3
00392d54: ldr      ip, [r4, r0]
00392d58: ldr      r2, [r4, r2]
00392d5c: mov      r0, r5
00392d60: ldr      ip, [ip, #0x38]
00392d64: add      r2, r2, #8
00392d68: str      r2, [sp, #0x18]
00392d6c: add      r2, ip, #0x80
00392d70: str      r2, [sp, #0x1c]
00392d74: ldr      ip, [ip, #0x80]
00392d78: str      r2, [sp, #0x24]
00392d7c: str      r3, [sp, #0x28]
00392d80: str      ip, [sp, #0x20]
00392d84: b        #0x392d00
00392d88: mov      r0, r5
00392d8c: mov      r1, r7
00392d90: bl       #0x37baf8
00392d94: ldr      r8, [r0, #4]
00392d98: cmp      r8, #1
00392d9c: beq      #0x392f74
00392da0: ldr      r3, [r5, #4]
00392da4: ldm      r3, {r2, r3}
00392da8: rsb      r3, r2, r3
00392dac: asr      r3, r3, #4
00392db0: add      r2, r3, r3, lsl #3
00392db4: add      r2, r2, r2, lsl #6
00392db8: add      r2, r3, r2, lsl #3
00392dbc: add      r2, r2, r2, lsl #15
00392dc0: add      r3, r3, r2, lsl #3
00392dc4: rsb      r3, r3, #0
00392dc8: cmp      r7, r3
00392dcc: bhs      #0x392cc4
00392dd0: mov      r0, r5
00392dd4: mov      r1, r7
00392dd8: bl       #0x37baf8
00392ddc: ldr      r3, [r0, #4]
00392de0: cmp      r3, #4
00392de4: bne      #0x392cc4
00392de8: mov      r1, r7
00392dec: mov      r0, r5
00392df0: bl       #0x37baf8
00392df4: bl       #0x31c49c
00392df8: add      r6, r6, #0x304
00392dfc: mov      r1, r0
00392e00: mov      r0, r6
00392e04: bl       #0x38f72c
00392e08: mov      r1, #0
00392e0c: mov      r4, r0
00392e10: mov      r0, r5
00392e14: bl       #0x37baf8
00392e18: bl       #0x31bbf0
00392e1c: mov      r1, #1
00392e20: mov      r7, r0
00392e24: mov      r0, r5
00392e28: bl       #0x37baf8
00392e2c: bl       #0x31bbf0
00392e30: mov      r2, r7
00392e34: mov      r3, r0
00392e38: add      r1, sp, #0x2c
00392e3c: mov      r0, r6
00392e40: str      r4, [sp]
00392e44: bl       #0x4a2e70
00392e48: b        #0x392bdc
00392e4c: mov      r0, r5
00392e50: mov      r1, #2
00392e54: bl       #0x37baf8
00392e58: ldr      r3, [r0, #4]
00392e5c: cmp      r3, #7
00392e60: beq      #0x392f1c
00392e64: ldr      r3, [r5, #4]
00392e68: ldr      r2, [r3, #4]
00392e6c: ldr      r3, [r3]
00392e70: rsb      r3, r3, r2
00392e74: asr      r3, r3, #4
00392e78: add      r2, r3, r3, lsl #3
00392e7c: add      r2, r2, r2, lsl #6
00392e80: add      r2, r3, r2, lsl #3
00392e84: add      r2, r2, r2, lsl #15
00392e88: add      r3, r3, r2, lsl #3
00392e8c: rsb      r3, r3, #0
00392e90: cmp      r3, #4
00392e94: bls      #0x392c9c
00392e98: mov      r1, #2
00392e9c: mov      r0, r5
00392ea0: bl       #0x37baf8
00392ea4: ldr      r1, [r0, #4]
00392ea8: cmp      r1, #3
00392eac: beq      #0x392fc4
00392eb0: ldr      r3, [r5, #4]
00392eb4: ldr      r2, [r3, #4]
00392eb8: ldr      r3, [r3]
00392ebc: rsb      r3, r3, r2
00392ec0: asr      r3, r3, #4
00392ec4: add      r2, r3, r3, lsl #3
00392ec8: add      r2, r2, r2, lsl #6
00392ecc: add      r2, r3, r2, lsl #3
00392ed0: add      r2, r2, r2, lsl #15
00392ed4: add      r3, r3, r2, lsl #3
00392ed8: rsb      r3, r3, #0
00392edc: b        #0x392c9c
00392ee0: mov      r1, r3
00392ee4: ldr      r3, [pc, #0x40c]
00392ee8: ldr      r2, [pc, #0x414]
00392eec: mov      r0, r5
00392ef0: ldr      r3, [r4, r3]
00392ef4: ldr      r2, [r4, r2]
00392ef8: ldr      ip, [r3, #0x38]
00392efc: add      r2, r2, #8
00392f00: str      r2, [sp, #0x18]
00392f04: add      r3, ip, #0x60
00392f08: str      r3, [sp, #0x1c]
00392f0c: ldr      r2, [ip, #0x60]
00392f10: str      r3, [sp, #0x24]
00392f14: str      r2, [sp, #0x20]
00392f18: b        #0x392d00
00392f1c: mov      r1, #2
00392f20: mov      r0, r5
00392f24: bl       #0x37baf8
00392f28: bl       #0x31b5a0
00392f2c: ldr      r2, [r0, #0x160]
00392f30: ldr      r3, [r5, #4]
00392f34: str      r2, [sp, #0x2c]
00392f38: ldr      r2, [r0, #0x164]
00392f3c: str      r2, [sp, #0x30]
00392f40: ldr      r2, [r0, #0x168]
00392f44: str      r2, [sp, #0x34]
00392f48: ldr      r2, [r3, #4]
00392f4c: ldr      r3, [r3]
00392f50: rsb      r3, r3, r2
00392f54: asr      r3, r3, #4
00392f58: add      r2, r3, r3, lsl #3
00392f5c: add      r2, r2, r2, lsl #6
00392f60: add      r2, r3, r2, lsl #3
00392f64: add      r2, r2, r2, lsl #15
00392f68: add      r3, r3, r2, lsl #3
00392f6c: rsb      r3, r3, #0
00392f70: b        #0x392cbc
00392f74: mov      r1, r7
00392f78: mov      r0, r5
00392f7c: bl       #0x37baf8
00392f80: bl       #0x31bc80
00392f84: cmp      r0, #0
00392f88: beq      #0x392da0
00392f8c: ldr      r1, [pc, #0x374]
00392f90: add      r6, r6, #0x304
00392f94: mov      r0, r6
00392f98: add      r1, pc, r1
00392f9c: bl       #0x38f72c
00392fa0: mov      r1, #0
00392fa4: mov      r4, r0
00392fa8: mov      r0, r5
00392fac: bl       #0x37baf8
00392fb0: bl       #0x31bbf0
00392fb4: mov      r1, r8
00392fb8: mov      r7, r0
00392fbc: mov      r0, r5
00392fc0: b        #0x392e28
00392fc4: mov      r0, r5
00392fc8: bl       #0x37baf8
00392fcc: ldr      r3, [r0, #4]
00392fd0: cmp      r3, #3
00392fd4: bne      #0x392eb0
00392fd8: mov      r0, r5
00392fdc: mov      r1, #4
00392fe0: bl       #0x37baf8
00392fe4: ldr      r3, [r0, #4]
00392fe8: cmp      r3, #3
00392fec: beq      #0x393014
00392ff0: ldr      r2, [r5, #4]
00392ff4: movw     r3, #0x6db7
00392ff8: movt     r3, #0xb6db
00392ffc: ldr      r1, [r2, #4]
00393000: ldr      r2, [r2]
00393004: rsb      r2, r2, r1
00393008: asr      r2, r2, #4
0039300c: mul      r3, r3, r2
00393010: b        #0x392c9c
00393014: ldr      r2, [r5, #4]
00393018: movw     r3, #0x6db7
0039301c: movt     r3, #0xb6db
00393020: ldm      r2, {r1, r2}
00393024: rsb      r2, r1, r2
00393028: asr      r2, r2, #4
0039302c: mul      r3, r3, r2
00393030: cmp      r3, #5
00393034: bhi      #0x393090
00393038: mov      r1, #2
0039303c: mov      r0, r5
00393040: bl       #0x37baf8
00393044: bl       #0x31bbf0
00393048: mov      r1, #3
0039304c: mov      r8, r0
00393050: mov      r0, r5
00393054: bl       #0x37baf8
00393058: bl       #0x31bbf0
0039305c: mov      r1, #4
00393060: mov      r7, r0
00393064: mov      r0, r5
00393068: bl       #0x37baf8
0039306c: bl       #0x31bbf0
00393070: ldr      r3, [r5, #4]
00393074: str      r7, [sp, #0x30]
00393078: str      r8, [sp, #0x2c]
0039307c: str      r0, [sp, #0x34]
00393080: ldr      r2, [r3, #4]
00393084: mov      r7, #6
00393088: ldr      r3, [r3]
0039308c: b        #0x392f50
00393090: mov      r0, r5
00393094: mov      r1, #5
00393098: bl       #0x37baf8
0039309c: ldr      r3, [r0, #4]
003930a0: cmp      r3, #1
003930a4: bne      #0x393038
003930a8: mov      r1, #5
003930ac: mov      r0, r5
003930b0: bl       #0x37baf8
003930b4: bl       #0x31bc80
003930b8: cmp      r0, #0
003930bc: beq      #0x393038
003930c0: mov      r3, #0
003930c4: mov      r0, r6
003930c8: add      r1, sp, #0x18
003930cc: str      r3, [sp, #0x20]
003930d0: str      r3, [sp, #0x18]
003930d4: str      r3, [sp, #0x1c]
003930d8: bl       #0x393ae4
003930dc: ldr      r3, [pc, #0x228]
003930e0: ldr      ip, [r6, #0x160]
003930e4: ldr      r2, [r6, #0x164]
003930e8: ldr      r7, [r4, r3]
003930ec: ldr      r3, [r6, #0x168]
003930f0: mov      r1, #2
003930f4: mov      r0, r5
003930f8: str      r3, [sp, #0x34]
003930fc: ldr      r3, [r7, #4]
00393100: str      ip, [sp, #0x2c]
00393104: str      r2, [sp, #0x30]
00393108: str      r3, [sp, #0xc]
0039310c: ldr      r3, [sp, #0x1c]
00393110: ldr      fp, [r7, #8]
00393114: ldr      sl, [r7]
00393118: str      r3, [sp, #0x10]
0039311c: ldr      r3, [sp, #0x18]
00393120: ldr      sb, [sp, #0x20]
00393124: str      r3, [sp, #0x14]
00393128: bl       #0x37baf8
0039312c: bl       #0x31bbf0
00393130: mov      r1, sb
00393134: mov      r8, r0
00393138: ldr      r0, [sp, #0xc]
0039313c: bl       #0x30ed6c
00393140: ldr      r1, [sp, #0x10]
00393144: mov      r3, r0
00393148: mov      r0, fp
0039314c: str      r3, [sp, #8]
00393150: bl       #0x30ed6c
00393154: ldr      r3, [sp, #8]
00393158: mov      r1, r0
0039315c: mov      r0, r3
00393160: bl       #0x30e3ac
00393164: mov      r1, r0
00393168: mov      r0, r8
0039316c: bl       #0x30ed6c
00393170: mov      r1, r0
00393174: ldr      r0, [sp, #0x2c]
00393178: bl       #0x30eba4
0039317c: ldr      r1, [sp, #0x14]
00393180: str      r0, [sp, #0x2c]
00393184: mov      r0, fp
00393188: bl       #0x30ed6c
0039318c: mov      r1, sl
00393190: mov      fp, r0
00393194: mov      r0, sb
00393198: bl       #0x30ed6c
0039319c: mov      r1, r0
003931a0: mov      r0, fp
003931a4: bl       #0x30e3ac
003931a8: mov      r1, r0
003931ac: mov      r0, r8
003931b0: bl       #0x30ed6c
003931b4: mov      r1, r0
003931b8: ldr      r0, [sp, #0x30]
003931bc: bl       #0x30eba4
003931c0: mov      r1, sl
003931c4: str      r0, [sp, #0x30]
003931c8: ldr      r0, [sp, #0x10]
003931cc: bl       #0x30ed6c
003931d0: ldr      r1, [sp, #0x14]
003931d4: mov      sl, r0
003931d8: ldr      r0, [sp, #0xc]
003931dc: bl       #0x30ed6c
003931e0: mov      r1, r0
003931e4: mov      r0, sl
003931e8: bl       #0x30e3ac
003931ec: mov      r1, r0
003931f0: mov      r0, r8
003931f4: bl       #0x30ed6c
003931f8: mov      r1, r0
003931fc: ldr      r0, [sp, #0x34]
00393200: bl       #0x30eba4
00393204: mov      r1, #3
00393208: str      r0, [sp, #0x34]
0039320c: mov      r0, r5
00393210: bl       #0x37baf8
00393214: bl       #0x31bbf0
00393218: ldr      r1, [sp, #0x1c]
0039321c: mov      r8, r0
00393220: bl       #0x30ed6c
00393224: ldr      r1, [sp, #0x20]
00393228: mov      sb, r0
0039322c: mov      r0, r8
00393230: bl       #0x30ed6c
00393234: ldr      r1, [sp, #0x18]
00393238: mov      sl, r0
0039323c: mov      r0, r8
00393240: bl       #0x30ed6c
00393244: mov      r1, r0
00393248: ldr      r0, [sp, #0x2c]
0039324c: bl       #0x30eba4
00393250: mov      r1, sb
00393254: str      r0, [sp, #0x2c]
00393258: ldr      r0, [sp, #0x30]
0039325c: bl       #0x30eba4
00393260: mov      r1, sl
00393264: str      r0, [sp, #0x30]
00393268: ldr      r0, [sp, #0x34]
0039326c: bl       #0x30eba4
00393270: mov      r1, #4
00393274: str      r0, [sp, #0x34]
00393278: mov      r0, r5
0039327c: bl       #0x37baf8
00393280: bl       #0x31bbf0
00393284: ldr      r1, [r7, #4]
00393288: mov      r8, r0
0039328c: bl       #0x30ed6c
00393290: ldr      r1, [r7, #8]
00393294: mov      sb, r0
00393298: mov      r0, r8
0039329c: bl       #0x30ed6c
003932a0: ldr      r1, [r7]
003932a4: mov      sl, r0
003932a8: mov      r0, r8
003932ac: bl       #0x30ed6c
003932b0: mov      r1, r0
003932b4: ldr      r0, [sp, #0x2c]
003932b8: bl       #0x30eba4
003932bc: mov      r1, sb
003932c0: str      r0, [sp, #0x2c]
003932c4: ldr      r0, [sp, #0x30]
003932c8: bl       #0x30eba4
003932cc: mov      r1, sl
003932d0: str      r0, [sp, #0x30]
003932d4: ldr      r0, [sp, #0x34]
003932d8: bl       #0x30eba4
003932dc: mov      r7, #6
003932e0: ldr      r3, [r5, #4]
003932e4: str      r0, [sp, #0x34]
003932e8: b        #0x392f48

# _ZNK13ItemInventory14IsDualWieldingEv
0040019c: b        #0x400158

# _ZNK13ItemInventory12HasTwoHanderEb
004001a0: push     {r4, r5, r6, lr}
004001a4: mov      r5, r1
004001a8: mov      r1, #1
004001ac: mov      r4, r0
004001b0: bl       #0x3fc6a8
004001b4: mov      r3, #0xc
004001b8: mul      r3, r3, r0
004001bc: ldr      r2, [r4, #0x14]
004001c0: ldr      r3, [r2, r3]
004001c4: ldr      r0, [r3, #4]
004001c8: cmp      r0, #0
004001cc: beq      #0x400218
004001d0: ldr      r0, [r0]
004001d4: bl       #0x3f9e08
004001d8: ldr      r3, [r0, #0x58]
004001dc: ldr      r2, [r4, #4]
004001e0: ldr      r0, [r0, #0x68]
004001e4: sub      r3, r3, #4
004001e8: cmp      r3, #1
004001ec: bls      #0x40021c
004001f0: cmp      r5, #0
004001f4: bne      #0x40021c
004001f8: cmn      r0, #4
004001fc: beq      #0x400208
00400200: mov      r0, r5
00400204: pop      {r4, r5, r6, pc}
00400208: movw     r3, #0x1324
0040020c: ldr      r0, [r2, r3]
00400210: rsbs     r0, r0, #1
00400214: movlo    r0, #0
00400218: pop      {r4, r5, r6, pc}
0040021c: cmn      r0, #4
00400220: movne    r0, #0
00400224: moveq    r0, #1
00400228: pop      {r4, r5, r6, pc}

# _ZN10GameObject17_TargetListBackupERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390bd4: str      lr, [sp, #-4]!
00390bd8: ldr      r3, [r0, #4]
00390bdc: sub      sp, sp, #0xc
00390be0: ldm      r3, {r0, r1}
00390be4: rsb      r3, r0, r1
00390be8: asr      r3, r3, #4
00390bec: add      r1, r3, r3, lsl #3
00390bf0: add      r1, r1, r1, lsl #6
00390bf4: add      r1, r3, r1, lsl #3
00390bf8: add      r1, r1, r1, lsl #15
00390bfc: add      r3, r3, r1, lsl #3
00390c00: cmp      r3, #0
00390c04: bne      #0x390c20
00390c08: ldr      r1, [pc, #0x3c]
00390c0c: add      r0, r2, #0x304
00390c10: add      r1, pc, r1
00390c14: add      sp, sp, #0xc
00390c18: pop      {lr}
00390c1c: b        #0x4a36b0
00390c20: ldr      r3, [r0, #4]
00390c24: cmp      r3, #4
00390c28: bne      #0x390c08
00390c2c: str      r2, [sp, #4]
00390c30: bl       #0x31c49c
00390c34: ldr      r2, [sp, #4]
00390c38: mov      r1, r0
00390c3c: add      r0, r2, #0x304
00390c40: add      sp, sp, #0xc
00390c44: pop      {lr}
00390c48: b        #0x4a36b0
00390c4c: ldrheq   pc, [r2], #-0xc0

# _ZN10GameObject17_GetTargetListTopERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038eb68: push     {r4, r5, r6, lr}
0038eb6c: ldr      r3, [r2, #0x314]
0038eb70: ldr      r4, [r2, #0x304]
0038eb74: mov      r5, r1
0038eb78: cmp      r3, r4
0038eb7c: beq      #0x38ebd4
0038eb80: ldr      r1, [r4]
0038eb84: mov      r0, r5
0038eb88: bl       #0x37c9f8
0038eb8c: mov      r0, r5
0038eb90: ldr      r1, [r4, #4]
0038eb94: bl       #0x37ccbc
0038eb98: movw     r1, #0x2ee0
0038eb9c: ldr      r0, [r4, #8]
0038eba0: movt     r1, #0x4265
0038eba4: bl       #0x30ed6c
0038eba8: mov      r1, r0
0038ebac: mov      r0, r5
0038ebb0: bl       #0x37ccbc
0038ebb4: ldr      r1, [r4, #0xc]
0038ebb8: mov      r0, r5
0038ebbc: and      r1, r1, #1
0038ebc0: bl       #0x37c7e4
0038ebc4: ldr      r1, [r4, #0x10]
0038ebc8: mov      r0, r5
0038ebcc: pop      {r4, r5, r6, lr}
0038ebd0: b        #0x37ccbc
0038ebd4: mov      r0, r1
0038ebd8: mov      r1, #0
0038ebdc: pop      {r4, r5, r6, lr}
0038ebe0: b        #0x38eb00

# _ZN9Character7_LookAtERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b8ed8: push     {r4, lr}
003b8edc: ldr      r3, [r0, #4]
003b8ee0: ldm      r3, {r0, r1}
003b8ee4: rsb      r3, r0, r1
003b8ee8: asr      r3, r3, #4
003b8eec: add      r1, r3, r3, lsl #3
003b8ef0: add      r1, r1, r1, lsl #6
003b8ef4: add      r1, r3, r1, lsl #3
003b8ef8: add      r1, r1, r1, lsl #15
003b8efc: add      r3, r3, r1, lsl #3
003b8f00: cmp      r3, #0
003b8f04: bne      #0x3b8f0c
003b8f08: pop      {r4, pc}
003b8f0c: ldr      r3, [r0, #4]
003b8f10: cmp      r3, #2
003b8f14: beq      #0x3b8f20
003b8f18: cmp      r3, #7
003b8f1c: bne      #0x3b8f08
003b8f20: ldr      r4, [r2, #0x378]
003b8f24: bl       #0x31b5a0
003b8f28: mov      r1, r0
003b8f2c: mov      r0, r4
003b8f30: pop      {r4, lr}
003b8f34: b        #0x4052bc

# _ZN9Character7UseManaEi
003bdef4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003bdef8: ldr      r4, [pc, #0x1a4]
003bdefc: ldr      r5, [pc, #0x1a4]
003bdf00: sub      sp, sp, #0x48
003bdf04: add      r4, pc, r4
003bdf08: ldr      r3, [r4, r5]
003bdf0c: mov      r6, r0
003bdf10: mov      r7, r1
003bdf14: ldr      r3, [r3]
003bdf18: str      r3, [sp, #0x44]
003bdf1c: bl       #0x7fd794
003bdf20: ldrb     r3, [r0, #5]
003bdf24: cmp      r3, #0
003bdf28: bne      #0x3bdf74
003bdf2c: cmp      r7, #0
003bdf30: blt      #0x3bdf94
003bdf34: ldr      sb, [pc, #0x170]
003bdf38: ldr      r3, [pc, #0x170]
003bdf3c: add      sb, pc, sb
003bdf40: ldr      r0, [r4, r3]
003bdf44: mov      r1, sb
003bdf48: bl       #0x320e14
003bdf4c: cmp      r0, #0
003bdf50: beq      #0x3bdfec
003bdf54: mov      r0, #1
003bdf58: ldr      r3, [r4, r5]
003bdf5c: ldr      r2, [sp, #0x44]
003bdf60: ldr      r3, [r3]
003bdf64: cmp      r2, r3
003bdf68: bne      #0x3be0a0
003bdf6c: add      sp, sp, #0x48
003bdf70: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003bdf74: ldr      r3, [r6]
003bdf78: mov      r0, r6
003bdf7c: mov      lr, pc
003bdf80: ldr      pc, [r3, #0x54]
003bdf84: cmp      r0, #0
003bdf88: bne      #0x3bdf54
003bdf8c: cmp      r7, #0
003bdf90: bge      #0x3bdf34
003bdf94: ldr      r3, [pc, #0x118]
003bdf98: ldr      r3, [r4, r3]
003bdf9c: ldr      r3, [r3]
003bdfa0: cmp      r3, #2
003bdfa4: moveq    r3, #0
003bdfa8: streq    r3, [r3]
003bdfac: beq      #0x3bdf34
003bdfb0: cmp      r3, #1
003bdfb4: bne      #0x3bdf34
003bdfb8: ldr      r0, [pc, #0xf8]
003bdfbc: ldr      r1, [pc, #0xf8]
003bdfc0: ldr      r2, [pc, #0xf8]
003bdfc4: ldr      r0, [r4, r0]
003bdfc8: ldr      r3, [pc, #0xf4]
003bdfcc: mov      ip, #0x9f
003bdfd0: add      r1, pc, r1
003bdfd4: add      r2, pc, r2
003bdfd8: add      r3, pc, r3
003bdfdc: add      r0, r0, #0xa8
003bdfe0: str      ip, [sp]
003bdfe4: bl       #0x30e004
003bdfe8: b        #0x3bdf34
003bdfec: ldr      r3, [pc, #0xd4]
003bdff0: add      r8, sp, #0x2c
003bdff4: ldr      sl, [r4, r3]
003bdff8: mov      r0, sl
003bdffc: bl       #0x337888
003be000: mov      r1, sb
003be004: add      r2, sp, #0x10
003be008: mov      r0, r8
003be00c: bl       #0x3140ec
003be010: mov      r1, r8
003be014: mov      r0, sl
003be018: bl       #0x337a88
003be01c: mov      sb, r0
003be020: mov      r0, r8
003be024: bl       #0x318254
003be028: cmp      sb, #0
003be02c: bne      #0x3bdf54
003be030: movw     r3, #0x14f0
003be034: ldrb     r3, [r6, r3]
003be038: cmp      r3, #0
003be03c: bne      #0x3bdf54
003be040: mov      r0, r6
003be044: mov      r1, r7
003be048: bl       #0x3bd40c
003be04c: cmp      r0, #0
003be050: beq      #0x3bdf58
003be054: rsb      r2, r7, #0
003be058: add      r0, r6, #0x560
003be05c: mov      r1, #0x29
003be060: bl       #0x3e0708
003be064: mov      r0, sl
003be068: bl       #0x337888
003be06c: ldr      r1, [pc, #0x58]
003be070: add      r6, sp, #0x14
003be074: add      r2, sp, #0xc
003be078: add      r1, pc, r1
003be07c: mov      r0, r6
003be080: bl       #0x3140ec
003be084: mov      r1, r6
003be088: mov      r0, sl
003be08c: bl       #0x337a88
003be090: mov      r0, r6
003be094: bl       #0x318254
003be098: mov      r0, #1
003be09c: b        #0x3bdf58
003be0a0: bl       #0x30e310
003be0a4: subseq   r6, sp, ip, lsl #23
003be0a8: andeq    r4, r0, ip, lsr #1
003be0ac: subseq   r6, r0, r4, lsr sl
003be0b0: strdeq   r3, r4, [r0], -r4
003be0b4: andeq    r3, r0, r0, asr #19
003be0b8: andeq    r1, r0, r0, asr #19
003be0bc: subseq   r0, r0, r8, lsl #8
003be0c0: subseq   r6, r0, ip, lsl sb
003be0c4: subseq   r6, r0, r8, lsr #18
003be0c8: andeq    r0, r0, r4, lsl #17
003be0cc: subseq   r6, r0, r8, lsl r8

# _ZN10GameObject14_PopTargetListERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038fbb8: push     {r4, lr}
0038fbbc: sub      sp, sp, #0x10
0038fbc0: mov      lr, r2
0038fbc4: mov      ip, sp
0038fbc8: add      r4, r2, #0x304
0038fbcc: ldm      r4, {r0, r1, r2, r3}
0038fbd0: stm      ip, {r0, r1, r2, r3}
0038fbd4: add      r0, lr, #0x314
0038fbd8: mov      r1, sp
0038fbdc: bl       #0x38d610
0038fbe0: cmp      r0, #0
0038fbe4: beq      #0x38fbf0
0038fbe8: mov      r0, r4
0038fbec: bl       #0x38fb18
0038fbf0: add      sp, sp, #0x10
0038fbf4: pop      {r4, pc}

# _ZNK9Character13GetAnimStanceEv
003a53e0: push     {r4, r5, r6, lr}
003a53e4: ldr      r3, [r0]
003a53e8: mov      r4, r0
003a53ec: mov      lr, pc
003a53f0: ldr      pc, [r3, #0x28]
003a53f4: ldr      r5, [pc, #0xa8]
003a53f8: cmp      r0, #0
003a53fc: add      r5, pc, r5
003a5400: bne      #0x3a5438
003a5404: mov      r4, #0
003a5408: ldr      r3, [pc, #0x98]
003a540c: ldr      r1, [pc, #0x98]
003a5410: ldr      r2, [pc, #0x98]
003a5414: ldr      r3, [r5, r3]
003a5418: add      r1, pc, r1
003a541c: add      r2, pc, r2
003a5420: ldr      r0, [r3, #0x2c]
003a5424: bl       #0x4c4bdc
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404

# _ZN9Character10_HasShieldERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b6fa4: push     {r4, lr}
003b6fa8: add      r0, r2, #0x37c
003b6fac: mov      r4, r1
003b6fb0: bl       #0x400110
003b6fb4: mov      r1, r0
003b6fb8: mov      r0, r4
003b6fbc: pop      {r4, lr}
003b6fc0: b        #0x37c7e4

# _ZN10GameObject21_SetTargetListSortingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390548: push     {r4, r5, r6, r7, r8, lr}
0039054c: ldr      r3, [r0, #4]
00390550: mov      r5, r2
00390554: ldr      r4, [pc, #0xb8]
00390558: ldm      r3, {r1, r2}
0039055c: add      r4, pc, r4
00390560: rsb      r3, r1, r2
00390564: asr      r3, r3, #4
00390568: add      r2, r3, r3, lsl #3
0039056c: add      r2, r2, r2, lsl #6
00390570: add      r2, r3, r2, lsl #3
00390574: add      r2, r2, r2, lsl #15
00390578: add      r3, r3, r2, lsl #3
0039057c: cmp      r3, #0
00390580: bne      #0x390588
00390584: pop      {r4, r5, r6, r7, r8, pc}
00390588: ldr      r3, [r1, #4]
0039058c: cmp      r3, #3
00390590: bne      #0x390584
00390594: mov      r1, #0
00390598: bl       #0x37baf8
0039059c: bl       #0x31bbf0
003905a0: bl       #0x30e4cc
003905a4: ldr      r2, [r5, #0x314]
003905a8: ldr      r3, [r5, #0x304]
003905ac: mov      r7, r0
003905b0: cmp      r2, r3
003905b4: beq      #0x3905d4
003905b8: add      r6, r5, #0x304
003905bc: mov      r0, r6
003905c0: bl       #0x38fb18
003905c4: ldr      r2, [r5, #0x314]
003905c8: ldr      r3, [r5, #0x304]
003905cc: cmp      r2, r3
003905d0: bne      #0x3905bc
003905d4: cmp      r7, #1
003905d8: beq      #0x390604
003905dc: cmp      r7, #2
003905e0: beq      #0x3905f4
003905e4: ldr      r3, [pc, #0x2c]
003905e8: ldr      r3, [r4, r3]
003905ec: str      r3, [r5, #0x32c]
003905f0: b        #0x390584
003905f4: ldr      r3, [pc, #0x20]
003905f8: ldr      r3, [r4, r3]
003905fc: str      r3, [r5, #0x32c]
00390600: pop      {r4, r5, r6, r7, r8, pc}
00390604: ldr      r3, [pc, #0x14]
00390608: ldr      r3, [r4, r3]
0039060c: str      r3, [r5, #0x32c]
00390610: pop      {r4, r5, r6, r7, r8, pc}
00390614: rsbeq    r4, r0, r4, lsr r5
00390618: andeq    r2, r0, r4, ror ip
0039061c: ldrdeq   r1, r2, [r0], -r4
00390620: andeq    r4, r0, r4, asr #21

# _ZN10GameObject26_SetTargetListObjectFilterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390624: str      lr, [sp, #-4]!
00390628: ldr      r3, [r0, #4]
0039062c: sub      sp, sp, #0xc
00390630: ldr      r1, [r3, #4]
00390634: ldr      ip, [r3]
00390638: rsb      r3, ip, r1
0039063c: asr      r3, r3, #4
00390640: add      r1, r3, r3, lsl #3
00390644: add      r1, r1, r1, lsl #6
00390648: add      r1, r3, r1, lsl #3
0039064c: add      r1, r1, r1, lsl #15
00390650: add      r3, r3, r1, lsl #3
00390654: cmp      r3, #0
00390658: bne      #0x390664
0039065c: add      sp, sp, #0xc
00390660: ldm      sp!, {pc}
00390664: ldr      r3, [ip, #4]
00390668: cmp      r3, #3
0039066c: bne      #0x39065c
00390670: mov      r1, #0
00390674: str      r2, [sp, #4]
00390678: bl       #0x37baf8
0039067c: bl       #0x31bbf0
00390680: bl       #0x30e4cc
00390684: ldr      r2, [sp, #4]
00390688: str      r0, [r2, #0x33c]
0039068c: b        #0x39065c

# _ZN10GameObject18_IsTargetListEmptyERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038e970: ldr      r3, [r2, #0x304]
0038e974: ldr      r2, [r2, #0x314]
0038e978: mov      r0, r1
0038e97c: cmp      r2, r3
0038e980: movne    r1, #0
0038e984: moveq    r1, #1
0038e988: b        #0x37c7e4

# _ZNK13ItemInventory8HasStaffEv
004000c8: push     {r4, lr}
004000cc: mov      r1, #1
004000d0: mov      r4, r0
004000d4: bl       #0x3fc6a8
004000d8: mov      r3, #0xc
004000dc: mul      r3, r3, r0
004000e0: ldr      r2, [r4, #0x14]
004000e4: ldr      r3, [r2, r3]
004000e8: ldr      r0, [r3, #4]
004000ec: cmp      r0, #0
004000f0: beq      #0x40010c
004000f4: ldr      r0, [r0]
004000f8: bl       #0x3f9e08
004000fc: ldr      r0, [r0, #0x94]
00400100: cmp      r0, #5
00400104: movne    r0, #0
00400108: moveq    r0, #1
0040010c: pop      {r4, pc}

# _ZN10GameObject17_TargetListSearchERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038fcf0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038fcf4: ldr      r3, [r0, #4]
0038fcf8: mov      r6, r2
0038fcfc: ldr      r4, [pc, #0x6ec]
0038fd00: ldm      r3, {r1, r2}
0038fd04: add      r4, pc, r4
0038fd08: sub      sp, sp, #0x6c
0038fd0c: rsb      r2, r1, r2
0038fd10: asr      r2, r2, #4
0038fd14: mov      r5, r0
0038fd18: add      r7, r2, r2, lsl #3
0038fd1c: add      r7, r7, r7, lsl #6
0038fd20: add      r7, r2, r7, lsl #3
0038fd24: add      r7, r7, r7, lsl #15
0038fd28: add      r7, r2, r7, lsl #3
0038fd2c: rsb      r7, r7, #0
0038fd30: cmp      r7, #0
0038fd34: bne      #0x38fd40
0038fd38: add      sp, sp, #0x6c
0038fd3c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038fd40: ldr      r2, [r1, #4]
0038fd44: cmp      r2, #3
0038fd48: bne      #0x38fd38
0038fd4c: cmp      r7, #1
0038fd50: bls      #0x38fd94
0038fd54: mov      r1, #1
0038fd58: bl       #0x37baf8
0038fd5c: ldr      r3, [r0, #4]
0038fd60: cmp      r3, #3
0038fd64: bne      #0x38fd38
0038fd68: ldr      r3, [r5, #4]
0038fd6c: ldr      r1, [r3, #4]
0038fd70: ldr      r2, [r3]
0038fd74: rsb      r2, r2, r1
0038fd78: asr      r2, r2, #4
0038fd7c: add      r1, r2, r2, lsl #3
0038fd80: add      r1, r1, r1, lsl #6
0038fd84: add      r1, r2, r1, lsl #3
0038fd88: add      r1, r1, r1, lsl #15
0038fd8c: add      r2, r2, r1, lsl #3
0038fd90: rsb      r7, r2, #0
0038fd94: ldr      r1, [r6, #0x164]
0038fd98: ldr      r2, [r6, #0x168]
0038fd9c: ldr      r0, [r6, #0x160]
0038fda0: str      r1, [sp, #0x60]
0038fda4: str      r2, [sp, #0x64]
0038fda8: str      r0, [sp, #0x5c]
0038fdac: ldr      r1, [r3, #4]
0038fdb0: ldr      r2, [r3]
0038fdb4: rsb      r2, r2, r1
0038fdb8: asr      r2, r2, #4
0038fdbc: add      r1, r2, r2, lsl #3
0038fdc0: add      r1, r1, r1, lsl #6
0038fdc4: add      r1, r2, r1, lsl #3
0038fdc8: add      r1, r1, r1, lsl #15
0038fdcc: add      r2, r2, r1, lsl #3
0038fdd0: rsb      r2, r2, #0
0038fdd4: cmp      r2, #2
0038fdd8: bhi      #0x38ff0c
0038fddc: sub      r7, r7, #1
0038fde0: ldr      r2, [pc, #0x60c]
0038fde4: ldr      ip, [pc, #0x60c]
0038fde8: ldr      r0, [pc, #0x60c]
0038fdec: ldr      r1, [r4, r2]
0038fdf0: ldr      ip, [r4, ip]
0038fdf4: ldr      r0, [r4, r0]
0038fdf8: ldr      r2, [r1, #0x38]
0038fdfc: ldr      sl, [r1, #0x40]
0038fe00: ldr      r1, [pc, #0x5f8]
0038fe04: add      r8, r2, #0x60
0038fe08: add      ip, ip, #8
0038fe0c: add      r0, r0, #8
0038fe10: mov      lr, #0
0038fe14: str      r0, [sp, #0x34]
0038fe18: str      ip, [sp, #0x44]
0038fe1c: str      sl, [sp, #0x48]
0038fe20: str      lr, [sp, #0x4c]
0038fe24: str      r8, [sp, #0x38]
0038fe28: ldr      r1, [r4, r1]
0038fe2c: ldr      ip, [r2, #0x60]
0038fe30: add      r0, r2, #0x80
0038fe34: add      r1, r1, #8
0038fe38: str      ip, [sp, #0x3c]
0038fe3c: str      r8, [sp, #0x40]
0038fe40: str      r1, [sp, #0x20]
0038fe44: str      r0, [sp, #0x24]
0038fe48: ldr      r2, [r2, #0x80]
0038fe4c: str      r0, [sp, #0x2c]
0038fe50: str      lr, [sp, #0x30]
0038fe54: str      r2, [sp, #0x28]
0038fe58: ldm      r3, {r2, r3}
0038fe5c: rsb      r3, r2, r3
0038fe60: asr      r3, r3, #4
0038fe64: add      r2, r3, r3, lsl #3
0038fe68: add      r2, r2, r2, lsl #6
0038fe6c: add      r2, r3, r2, lsl #3
0038fe70: add      r2, r2, r2, lsl #15
0038fe74: add      r3, r3, r2, lsl #3
0038fe78: rsb      r3, r3, #0
0038fe7c: cmp      r7, r3
0038fe80: blo      #0x39001c
0038fe84: ldr      r2, [r6, #0x338]
0038fe88: tst      r2, #0x80
0038fe8c: addne    r6, r6, #0x304
0038fe90: addne    r4, sp, #0x44
0038fe94: beq      #0x38fef4
0038fe98: cmp      r3, #1
0038fe9c: bls      #0x3900b0
0038fea0: mov      r1, #0
0038fea4: mov      r0, r5
0038fea8: bl       #0x37baf8
0038feac: bl       #0x31bbf0
0038feb0: mov      r1, #1
0038feb4: mov      r7, r0
0038feb8: mov      r0, r5
0038febc: bl       #0x37baf8
0038fec0: bl       #0x31bbf0
0038fec4: movw     r1, #0xfa35
0038fec8: movt     r1, #0x3c8e
0038fecc: bl       #0x30ed6c
0038fed0: mov      r1, #0x3f000000
0038fed4: bl       #0x30ed6c
0038fed8: add      r1, sp, #0x5c
0038fedc: mov      r3, r0
0038fee0: mov      r2, r7
0038fee4: mov      r0, r6
0038fee8: str      r4, [sp]
0038feec: bl       #0x4a33c8
0038fef0: b        #0x38fd38
0038fef4: ldr      r2, [r6, #0x33c]
0038fef8: add      r6, r6, #0x304
0038fefc: cmp      r2, #2
0038ff00: addne    r4, sp, #0x20
0038ff04: addeq    r4, sp, #0x34
0038ff08: b        #0x38fe98
0038ff0c: mov      r0, r5
0038ff10: mov      r1, #2
0038ff14: bl       #0x37baf8
0038ff18: ldr      r3, [r0, #4]
0038ff1c: cmp      r3, #7
0038ff20: beq      #0x39033c
0038ff24: ldr      r3, [r5, #4]
0038ff28: ldr      r1, [r3, #4]
0038ff2c: ldr      r2, [r3]
0038ff30: rsb      r2, r2, r1
0038ff34: asr      r2, r2, #4
0038ff38: add      r1, r2, r2, lsl #3
0038ff3c: add      r1, r1, r1, lsl #6
0038ff40: add      r1, r2, r1, lsl #3
0038ff44: add      r1, r1, r1, lsl #15
0038ff48: add      r2, r2, r1, lsl #3
0038ff4c: rsb      r2, r2, #0
0038ff50: cmp      r2, #4
0038ff54: bls      #0x38fddc
0038ff58: mov      r1, #2
0038ff5c: mov      r0, r5
0038ff60: bl       #0x37baf8
0038ff64: ldr      r1, [r0, #4]
0038ff68: cmp      r1, #3
0038ff6c: beq      #0x38ff78
0038ff70: ldr      r3, [r5, #4]
0038ff74: b        #0x38fddc
0038ff78: mov      r0, r5
0038ff7c: bl       #0x37baf8
0038ff80: ldr      r3, [r0, #4]
0038ff84: cmp      r3, #3
0038ff88: bne      #0x38ff70
0038ff8c: mov      r0, r5
0038ff90: mov      r1, #4
0038ff94: bl       #0x37baf8
0038ff98: ldr      r0, [r0, #4]
0038ff9c: cmp      r0, #3
0038ffa0: str      r0, [sp, #0x1c]
0038ffa4: bne      #0x38ff70
0038ffa8: ldr      r2, [r5, #4]
0038ffac: movw     r3, #0x6db7
0038ffb0: movt     r3, #0xb6db
0038ffb4: ldm      r2, {r1, r2}
0038ffb8: rsb      r2, r1, r2
0038ffbc: asr      r2, r2, #4
0038ffc0: mul      r3, r3, r2
0038ffc4: cmp      r3, #5
0038ffc8: bhi      #0x3900e0
0038ffcc: mov      r1, #2
0038ffd0: mov      r0, r5
0038ffd4: bl       #0x37baf8
0038ffd8: bl       #0x31bbf0
0038ffdc: mov      r1, #3
0038ffe0: mov      r8, r0
0038ffe4: mov      r0, r5
0038ffe8: bl       #0x37baf8
0038ffec: bl       #0x31bbf0
0038fff0: mov      r1, #4
0038fff4: mov      r7, r0
0038fff8: mov      r0, r5
0038fffc: bl       #0x37baf8
00390000: bl       #0x31bbf0
00390004: str      r7, [sp, #0x60]
00390008: str      r8, [sp, #0x5c]
0039000c: str      r0, [sp, #0x64]
00390010: mov      r7, #6
00390014: ldr      r3, [r5, #4]
00390018: b        #0x38fde0
0039001c: mov      r0, r5
00390020: mov      r1, r7
00390024: bl       #0x37baf8
00390028: ldr      r3, [r0, #4]
0039002c: cmp      r3, #1
00390030: beq      #0x3903c4
00390034: ldr      r2, [r5, #4]
00390038: ldr      r3, [r2]
0039003c: ldr      r2, [r2, #4]
00390040: rsb      r3, r3, r2
00390044: asr      r3, r3, #4
00390048: add      r2, r3, r3, lsl #3
0039004c: add      r2, r2, r2, lsl #6
00390050: add      r2, r3, r2, lsl #3
00390054: add      r2, r2, r2, lsl #15
00390058: add      r3, r3, r2, lsl #3
0039005c: rsb      r3, r3, #0
00390060: cmp      r7, r3
00390064: bhs      #0x38fe84
00390068: mov      r0, r5
0039006c: mov      r1, r7
00390070: bl       #0x37baf8
00390074: ldr      r3, [r0, #4]
00390078: cmp      r3, #4
0039007c: beq      #0x390370
00390080: ldr      r3, [r5, #4]
00390084: ldr      r2, [r3, #4]
00390088: ldr      r3, [r3]
0039008c: rsb      r3, r3, r2
00390090: asr      r3, r3, #4
00390094: add      r2, r3, r3, lsl #3
00390098: add      r2, r2, r2, lsl #6
0039009c: add      r2, r3, r2, lsl #3
003900a0: add      r2, r2, r2, lsl #15
003900a4: add      r3, r3, r2, lsl #3
003900a8: rsb      r3, r3, #0
003900ac: b        #0x38fe84
003900b0: mov      r1, #0
003900b4: mov      r0, r5
003900b8: bl       #0x37baf8
003900bc: bl       #0x31bbf0
003900c0: movw     r3, #0xfdb
003900c4: mov      r2, r0
003900c8: add      r1, sp, #0x5c
003900cc: mov      r0, r6
003900d0: movt     r3, #0x4049
003900d4: str      r4, [sp]
003900d8: bl       #0x4a33c8
003900dc: b        #0x38fd38
003900e0: mov      r0, r5
003900e4: mov      r1, #5
003900e8: bl       #0x37baf8
003900ec: ldr      r3, [r0, #4]
003900f0: cmp      r3, #1
003900f4: bne      #0x38ffcc
003900f8: mov      r1, #5
003900fc: mov      r0, r5
00390100: bl       #0x37baf8
00390104: bl       #0x31bc80
00390108: cmp      r0, #0
0039010c: beq      #0x38ffcc
00390110: mov      r3, #0
00390114: mov      r0, r6
00390118: add      r1, sp, #0x50
0039011c: str      r3, [sp, #0x58]
00390120: str      r3, [sp, #0x50]
00390124: str      r3, [sp, #0x54]
00390128: bl       #0x393ae4
0039012c: ldr      r3, [pc, #0x2d0]
00390130: ldr      ip, [r6, #0x160]
00390134: ldr      r2, [r6, #0x164]
00390138: ldr      r7, [r4, r3]
0039013c: ldr      r3, [r6, #0x168]
00390140: mov      r1, #2
00390144: mov      r0, r5
00390148: str      r3, [sp, #0x64]
0039014c: ldr      r3, [r7, #4]
00390150: str      ip, [sp, #0x5c]
00390154: str      r2, [sp, #0x60]
00390158: str      r3, [sp, #0x10]
0039015c: ldr      r3, [sp, #0x54]
00390160: ldr      fp, [r7, #8]
00390164: ldr      sl, [r7]
00390168: str      r3, [sp, #0x14]
0039016c: ldr      r3, [sp, #0x50]
00390170: ldr      sb, [sp, #0x58]
00390174: str      r3, [sp, #0x18]
00390178: bl       #0x37baf8
0039017c: bl       #0x31bbf0
00390180: mov      r1, sb
00390184: mov      r8, r0
00390188: ldr      r0, [sp, #0x10]
0039018c: bl       #0x30ed6c
00390190: ldr      r1, [sp, #0x14]
00390194: mov      r3, r0
00390198: mov      r0, fp
0039019c: str      r3, [sp, #0xc]
003901a0: bl       #0x30ed6c
003901a4: ldr      r3, [sp, #0xc]
003901a8: mov      r1, r0
003901ac: mov      r0, r3
003901b0: bl       #0x30e3ac
003901b4: mov      r1, r0
003901b8: mov      r0, r8
003901bc: bl       #0x30ed6c
003901c0: mov      r1, r0
003901c4: ldr      r0, [sp, #0x5c]
003901c8: bl       #0x30eba4
003901cc: ldr      r1, [sp, #0x18]
003901d0: str      r0, [sp, #0x5c]
003901d4: mov      r0, fp
003901d8: bl       #0x30ed6c
003901dc: mov      r1, sl
003901e0: mov      fp, r0
003901e4: mov      r0, sb
003901e8: bl       #0x30ed6c
003901ec: mov      r1, r0
003901f0: mov      r0, fp
003901f4: bl       #0x30e3ac
003901f8: mov      r1, r0
003901fc: mov      r0, r8
00390200: bl       #0x30ed6c
00390204: mov      r1, r0
00390208: ldr      r0, [sp, #0x60]
0039020c: bl       #0x30eba4
00390210: mov      r1, sl
00390214: str      r0, [sp, #0x60]
00390218: ldr      r0, [sp, #0x14]
0039021c: bl       #0x30ed6c
00390220: ldr      r1, [sp, #0x18]
00390224: mov      sl, r0
00390228: ldr      r0, [sp, #0x10]
0039022c: bl       #0x30ed6c
00390230: mov      r1, r0
00390234: mov      r0, sl
00390238: bl       #0x30e3ac
0039023c: mov      r1, r0
00390240: mov      r0, r8
00390244: bl       #0x30ed6c
00390248: mov      r1, r0
0039024c: ldr      r0, [sp, #0x64]
00390250: bl       #0x30eba4
00390254: ldr      r1, [sp, #0x1c]
00390258: str      r0, [sp, #0x64]
0039025c: mov      r0, r5
00390260: bl       #0x37baf8
00390264: bl       #0x31bbf0
00390268: ldr      r1, [sp, #0x54]
0039026c: mov      r8, r0
00390270: bl       #0x30ed6c
00390274: ldr      r1, [sp, #0x58]
00390278: mov      sb, r0
0039027c: mov      r0, r8
00390280: bl       #0x30ed6c
00390284: ldr      r1, [sp, #0x50]
00390288: mov      sl, r0
0039028c: mov      r0, r8
00390290: bl       #0x30ed6c
00390294: mov      r1, r0
00390298: ldr      r0, [sp, #0x5c]
0039029c: bl       #0x30eba4
003902a0: mov      r1, sb
003902a4: str      r0, [sp, #0x5c]
003902a8: ldr      r0, [sp, #0x60]
003902ac: bl       #0x30eba4
003902b0: mov      r1, sl
003902b4: str      r0, [sp, #0x60]
003902b8: ldr      r0, [sp, #0x64]
003902bc: bl       #0x30eba4
003902c0: mov      r1, #4
003902c4: str      r0, [sp, #0x64]
003902c8: mov      r0, r5
003902cc: bl       #0x37baf8
003902d0: bl       #0x31bbf0
003902d4: ldr      r1, [r7, #4]
003902d8: mov      r8, r0
003902dc: bl       #0x30ed6c
003902e0: ldr      r1, [r7, #8]
003902e4: mov      sb, r0
003902e8: mov      r0, r8
003902ec: bl       #0x30ed6c
003902f0: ldr      r1, [r7]
003902f4: mov      sl, r0
003902f8: mov      r0, r8
003902fc: bl       #0x30ed6c
00390300: mov      r1, r0
00390304: ldr      r0, [sp, #0x5c]
00390308: bl       #0x30eba4
0039030c: mov      r1, sb
00390310: str      r0, [sp, #0x5c]
00390314: ldr      r0, [sp, #0x60]
00390318: bl       #0x30eba4
0039031c: mov      r1, sl
00390320: str      r0, [sp, #0x60]
00390324: ldr      r0, [sp, #0x64]
00390328: bl       #0x30eba4
0039032c: mov      r7, #6
00390330: ldr      r3, [r5, #4]
00390334: str      r0, [sp, #0x64]
00390338: b        #0x38fde0
0039033c: mov      r1, #2
00390340: mov      r0, r5
00390344: bl       #0x37baf8
00390348: bl       #0x31b5a0
0039034c: ldr      r2, [r0, #0x160]
00390350: ldr      r3, [r5, #4]
00390354: mov      r7, #3
00390358: str      r2, [sp, #0x5c]
0039035c: ldr      r2, [r0, #0x164]
00390360: str      r2, [sp, #0x60]
00390364: ldr      r2, [r0, #0x168]
00390368: str      r2, [sp, #0x64]
0039036c: b        #0x38fde0
00390370: mov      r1, r7
00390374: mov      r0, r5
00390378: bl       #0x37baf8
0039037c: bl       #0x31c49c
00390380: add      r6, r6, #0x304
00390384: mov      r1, r0
00390388: mov      r0, r6
0039038c: bl       #0x38f72c
00390390: ldr      r3, [r5, #4]
00390394: mov      r4, r0
00390398: ldr      r2, [r3, #4]
0039039c: ldr      r3, [r3]
003903a0: rsb      r3, r3, r2
003903a4: asr      r3, r3, #4
003903a8: add      r2, r3, r3, lsl #3
003903ac: add      r2, r2, r2, lsl #6
003903b0: add      r2, r3, r2, lsl #3
003903b4: add      r2, r2, r2, lsl #15
003903b8: add      r3, r3, r2, lsl #3
003903bc: rsb      r3, r3, #0
003903c0: b        #0x38fe98
003903c4: mov      r1, r7
003903c8: mov      r0, r5
003903cc: bl       #0x37baf8
003903d0: bl       #0x31bc80
003903d4: cmp      r0, #0
003903d8: beq      #0x390034
003903dc: ldr      r1, [pc, #0x24]
003903e0: add      r6, r6, #0x304
003903e4: mov      r0, r6
003903e8: add      r1, pc, r1
003903ec: b        #0x39038c
003903f0: rsbeq    r4, r0, ip, lsl #27
003903f4: strdeq   r3, r4, [r0], -r4
003903f8: andeq    r2, r0, r8, lsl #17
003903fc: andeq    r2, r0, r0, lsr r6
00390400: andeq    r3, r0, r4, ror #10
00390404: andeq    r4, r0, r0, asr #6
00390408: ldrsbeq  r0, [r3], #-0x48

# _ZN9Character8_UseManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b7864: push     {r4, lr}
003b7868: ldr      r3, [r0, #4]
003b786c: mov      r4, r1
003b7870: sub      sp, sp, #8
003b7874: ldr      r1, [r3, #4]
003b7878: ldr      ip, [r3]
003b787c: rsb      r3, ip, r1
003b7880: asr      r3, r3, #4
003b7884: add      r1, r3, r3, lsl #3
003b7888: add      r1, r1, r1, lsl #6
003b788c: add      r1, r3, r1, lsl #3
003b7890: add      r1, r1, r1, lsl #15
003b7894: add      r3, r3, r1, lsl #3
003b7898: cmp      r3, #0
003b789c: bne      #0x3b78a8
003b78a0: add      sp, sp, #8
003b78a4: pop      {r4, pc}
003b78a8: ldr      r3, [ip, #4]
003b78ac: cmp      r3, #3
003b78b0: bne      #0x3b78a0
003b78b4: mov      r1, #0
003b78b8: str      r2, [sp, #4]
003b78bc: bl       #0x37baf8
003b78c0: bl       #0x31bbf0
003b78c4: bl       #0x30e4cc
003b78c8: ldr      r2, [sp, #4]
003b78cc: mov      r1, r0
003b78d0: mov      r0, r2
003b78d4: bl       #0x3bdef4
003b78d8: mov      r1, r0
003b78dc: mov      r0, r4
003b78e0: add      sp, sp, #8
003b78e4: pop      {r4, lr}
003b78e8: b        #0x37c7e4

# _ZN14ObjectSearcher10TargetList13BackupResultsEPKc
004a36b0: push     {r4, r5, r6, r7, r8, sl, lr}
004a36b4: ldr      r3, [pc, #0xfc]
004a36b8: sub      sp, sp, #0x24
004a36bc: subs     r2, r1, #0
004a36c0: str      r1, [sp, #0xc]
004a36c4: mov      r8, r0
004a36c8: add      r3, pc, r3
004a36cc: beq      #0x4a3764
004a36d0: add      r1, sp, #0xc
004a36d4: add      r0, r8, #0x3c
004a36d8: bl       #0x38f53c
004a36dc: ldr      r4, [r8]
004a36e0: mov      r5, r0
004a36e4: ldr      r7, [r8, #0x10]
004a36e8: ldr      sl, [r8, #0xc]
004a36ec: ldr      r6, [r8, #8]
004a36f0: bl       #0x4a19f8
004a36f4: ldm      r8, {r0, r1, r2, r3}
004a36f8: add      ip, sp, #0x10
004a36fc: stm      ip, {r0, r1, r2, r3}
004a3700: mov      r1, ip
004a3704: add      r0, r8, #0x10
004a3708: bl       #0x38d610
004a370c: mov      r1, r0
004a3710: add      r0, r5, #4
004a3714: bl       #0x4a2024
004a3718: cmp      r7, r4
004a371c: bne      #0x4a372c
004a3720: b        #0x4a3750
004a3724: cmp      r4, r7
004a3728: beq      #0x4a3750
004a372c: ldr      r1, [r4], #0x14
004a3730: mov      r0, r5
004a3734: bl       #0x4a20f0
004a3738: cmp      r4, r6
004a373c: bne      #0x4a3724
004a3740: ldr      r4, [sl, #4]!
004a3744: cmp      r7, r4
004a3748: add      r6, r4, #0x78
004a374c: bne      #0x4a372c
004a3750: ldmib    r5, {r2, r3}
004a3754: str      r2, [r5, #0x10]
004a3758: str      r3, [r5, #0x14]
004a375c: add      sp, sp, #0x24
004a3760: pop      {r4, r5, r6, r7, r8, sl, pc}
004a3764: ldr      r1, [pc, #0x50]
004a3768: ldr      r1, [r3, r1]
004a376c: ldr      r1, [r1]
004a3770: cmp      r1, #2
004a3774: streq    r2, [r2]
004a3778: beq      #0x4a36d0
004a377c: cmp      r1, #1
004a3780: bne      #0x4a36d0
004a3784: ldr      r0, [pc, #0x34]
004a3788: ldr      r1, [pc, #0x34]
004a378c: ldr      r2, [pc, #0x34]
004a3790: ldr      r0, [r3, r0]
004a3794: ldr      r3, [pc, #0x30]
004a3798: movw     ip, #0x195
004a379c: add      r1, pc, r1
004a37a0: add      r2, pc, r2
004a37a4: add      r3, pc, r3
004a37a8: add      r0, r0, #0xa8
004a37ac: str      ip, [sp]
004a37b0: bl       #0x30e004
004a37b4: b        #0x4a36d0
004a37b8: subeq    r1, pc, r8, asr #7
004a37bc: andeq    r3, r0, r0, asr #19
004a37c0: andeq    r1, r0, r0, asr #19
004a37c4: subeq    sl, r1, ip, lsr ip
004a37c8: strheq   pc, [r1], #-0x30
004a37cc: strheq   r1, [r3], #-0xe4

# _ZN10GameObject29_SetTargetListCharacterFilterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
00390690: str      lr, [sp, #-4]!
00390694: ldr      r3, [r0, #4]
00390698: sub      sp, sp, #0xc
0039069c: ldr      r1, [r3, #4]
003906a0: ldr      ip, [r3]
003906a4: rsb      r3, ip, r1
003906a8: asr      r3, r3, #4
003906ac: add      r1, r3, r3, lsl #3
003906b0: add      r1, r1, r1, lsl #6
003906b4: add      r1, r3, r1, lsl #3
003906b8: add      r1, r1, r1, lsl #15
003906bc: add      r3, r3, r1, lsl #3
003906c0: cmp      r3, #0
003906c4: bne      #0x3906d0
003906c8: add      sp, sp, #0xc
003906cc: ldm      sp!, {pc}
003906d0: ldr      r3, [ip, #4]
003906d4: cmp      r3, #3
003906d8: bne      #0x3906c8
003906dc: mov      r1, #0
003906e0: str      r2, [sp, #4]
003906e4: bl       #0x37baf8
003906e8: bl       #0x31bbf0
003906ec: bl       #0x30e4cc
003906f0: ldr      r2, [sp, #4]
003906f4: str      r0, [r2, #0x338]
003906f8: b        #0x3906c8

# _ZN9Character8_HasManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b78ec: push     {r4, lr}
003b78f0: ldr      r3, [r0, #4]
003b78f4: mov      r4, r1
003b78f8: sub      sp, sp, #8
003b78fc: ldr      r1, [r3, #4]
003b7900: ldr      ip, [r3]
003b7904: rsb      r3, ip, r1
003b7908: asr      r3, r3, #4
003b790c: add      r1, r3, r3, lsl #3
003b7910: add      r1, r1, r1, lsl #6
003b7914: add      r1, r3, r1, lsl #3
003b7918: add      r1, r1, r1, lsl #15
003b791c: add      r3, r3, r1, lsl #3
003b7920: cmp      r3, #0
003b7924: bne      #0x3b7930
003b7928: add      sp, sp, #8
003b792c: pop      {r4, pc}
003b7930: ldr      r3, [ip, #4]
003b7934: cmp      r3, #3
003b7938: bne      #0x3b7928
003b793c: mov      r1, #0
003b7940: str      r2, [sp, #4]
003b7944: bl       #0x37baf8
003b7948: bl       #0x31bbf0
003b794c: bl       #0x30e4cc
003b7950: ldr      r2, [sp, #4]
003b7954: mov      r1, r0
003b7958: mov      r0, r2
003b795c: bl       #0x3bd40c
003b7960: mov      r1, r0
003b7964: mov      r0, r4
003b7968: add      sp, sp, #8
003b796c: pop      {r4, lr}
003b7970: b        #0x37c7e4
