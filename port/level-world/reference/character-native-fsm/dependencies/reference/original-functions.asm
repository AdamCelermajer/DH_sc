
# _ZNK16CharStateMachine11SM_IsScaredEb
003c034c: cmp      r1, #0
003c0350: push     {r4, lr}
003c0354: beq      #0x3c0364
003c0358: ldr      r0, [r0, #0x2c]
003c035c: ubfx     r0, r0, #2, #1
003c0360: pop      {r4, pc}
003c0364: bl       #0x3c01ac
003c0368: cmp      r0, #8
003c036c: movne    r0, #0
003c0370: moveq    r0, #1
003c0374: pop      {r4, pc}

# _ZN16CharStateMachine15SM_SetStunStateEjbPvb
003c5ffc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c6000: mov      r5, r0
003c6004: sub      sp, sp, #8
003c6008: ldr      r0, [r0, #4]
003c600c: mov      sl, r1
003c6010: mov      r8, r2
003c6014: mov      r6, r3
003c6018: ldrb     r7, [sp, #0x28]
003c601c: bl       #0x3a3158
003c6020: ldr      r4, [pc, #0x104]
003c6024: cmp      r0, #0
003c6028: add      r4, pc, r4
003c602c: beq      #0x3c6038
003c6030: add      sp, sp, #8
003c6034: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c6038: ldr      r0, [r5, #4]
003c603c: bl       #0x3a3228
003c6040: subs     sb, r0, #0
003c6044: blt      #0x3c6030
003c6048: ldr      r3, [pc, #0xe0]
003c604c: ldr      r3, [r4, r3]
003c6050: ldr      r3, [r3]
003c6054: cmp      sb, r3
003c6058: bge      #0x3c6030
003c605c: ldr      ip, [r5, #0x2c]
003c6060: ands     ip, ip, #2
003c6064: beq      #0x3c6100
003c6068: ldr      r3, [pc, #0xc4]
003c606c: ldr      r2, [pc, #0xc4]
003c6070: ldr      r1, [pc, #0xc4]
003c6074: ldr      r3, [r4, r3]
003c6078: ldr      r2, [r4, r2]
003c607c: add      r1, pc, r1
003c6080: ldr      r3, [r3]
003c6084: ldr      r0, [r2, #0x2c]
003c6088: mov      r2, #0xa0
003c608c: mla      sb, r2, sb, r3
003c6090: ldr      r2, [pc, #0xa8]
003c6094: ldr      r4, [sb, #0x8c]
003c6098: add      r2, pc, r2
003c609c: bl       #0x4c4bdc
003c60a0: ands     r0, r0, #0x200
003c60a4: bne      #0x3c60f4
003c60a8: add      r4, r0, r4
003c60ac: cmp      r7, #0
003c60b0: str      r4, [r5, #0x28]
003c60b4: beq      #0x3c60e0
003c60b8: mov      r3, r6
003c60bc: mov      r0, r5
003c60c0: mov      r1, #9
003c60c4: movw     r2, #0xc35c
003c60c8: bl       #0x3c1938
003c60cc: cmp      r8, #0
003c60d0: ldrne    r3, [r5, #0x24]
003c60d4: orrne    r3, r3, #0x800
003c60d8: strne    r3, [r5, #0x24]
003c60dc: b        #0x3c6030
003c60e0: mov      r2, r6
003c60e4: mov      r0, r5
003c60e8: movw     r1, #0xc35c
003c60ec: bl       #0x3c5684
003c60f0: b        #0x3c60cc
003c60f4: ldr      r0, [r5, #4]
003c60f8: bl       #0x3a53e0
003c60fc: b        #0x3c60a8
003c6100: ldr      r0, [r5, #4]
003c6104: mov      r3, #0x2b
003c6108: mov      r1, sl
003c610c: mov      r2, ip
003c6110: add      r0, r0, #0x3b4
003c6114: str      ip, [sp]
003c6118: bl       #0x3dbe24
003c611c: ldr      r3, [r5, #0x2c]
003c6120: orr      r3, r3, #2
003c6124: str      r3, [r5, #0x2c]
003c6128: b        #0x3c6068
003c612c: subseq   lr, ip, r8, ror #20
003c6130: andeq    r2, r0, r0, asr #17
003c6134: andeq    r4, r0, r4, asr #16
003c6138: strdeq   r3, r4, [r0], -r4
003c613c: subeq    lr, pc, ip, lsr fp
003c6140: subeq    lr, pc, r0, lsr fp

# _ZN16CharStateMachine16SM_SetScareStateEjbPvb
003c6144: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c6148: mov      r5, r0
003c614c: sub      sp, sp, #8
003c6150: ldr      r0, [r0, #4]
003c6154: mov      sl, r1
003c6158: mov      r8, r2
003c615c: mov      r6, r3
003c6160: ldrb     r7, [sp, #0x28]
003c6164: bl       #0x3a3158
003c6168: ldr      r4, [pc, #0x104]
003c616c: cmp      r0, #0
003c6170: add      r4, pc, r4
003c6174: beq      #0x3c6180
003c6178: add      sp, sp, #8
003c617c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c6180: ldr      r0, [r5, #4]
003c6184: bl       #0x3a3228
003c6188: subs     sb, r0, #0
003c618c: blt      #0x3c6178
003c6190: ldr      r3, [pc, #0xe0]
003c6194: ldr      r3, [r4, r3]
003c6198: ldr      r3, [r3]
003c619c: cmp      sb, r3
003c61a0: bge      #0x3c6178
003c61a4: ldr      ip, [r5, #0x2c]
003c61a8: ands     ip, ip, #4
003c61ac: beq      #0x3c6248
003c61b0: ldr      r3, [pc, #0xc4]
003c61b4: ldr      r2, [pc, #0xc4]
003c61b8: ldr      r1, [pc, #0xc4]
003c61bc: ldr      r3, [r4, r3]
003c61c0: ldr      r2, [r4, r2]
003c61c4: add      r1, pc, r1
003c61c8: ldr      r3, [r3]
003c61cc: ldr      r0, [r2, #0x2c]
003c61d0: mov      r2, #0xa0
003c61d4: mla      sb, r2, sb, r3
003c61d8: ldr      r2, [pc, #0xa8]
003c61dc: ldr      r4, [sb, #0x7c]
003c61e0: add      r2, pc, r2
003c61e4: bl       #0x4c4bdc
003c61e8: ands     r0, r0, #0x100
003c61ec: bne      #0x3c623c
003c61f0: add      r4, r0, r4
003c61f4: cmp      r7, #0
003c61f8: str      r4, [r5, #0x28]
003c61fc: beq      #0x3c6228
003c6200: mov      r3, r6
003c6204: mov      r0, r5
003c6208: mov      r1, #8
003c620c: movw     r2, #0xc35d
003c6210: bl       #0x3c1938
003c6214: cmp      r8, #0
003c6218: ldrne    r3, [r5, #0x24]
003c621c: orrne    r3, r3, #0x400
003c6220: strne    r3, [r5, #0x24]
003c6224: b        #0x3c6178
003c6228: mov      r2, r6
003c622c: mov      r0, r5
003c6230: movw     r1, #0xc35d
003c6234: bl       #0x3c5684
003c6238: b        #0x3c6214
003c623c: ldr      r0, [r5, #4]
003c6240: bl       #0x3a53e0
003c6244: b        #0x3c61f0
003c6248: ldr      r0, [r5, #4]
003c624c: mov      r3, #0x2c
003c6250: mov      r1, sl
003c6254: mov      r2, ip
003c6258: add      r0, r0, #0x3b4
003c625c: str      ip, [sp]
003c6260: bl       #0x3dbe24
003c6264: ldr      r3, [r5, #0x2c]
003c6268: orr      r3, r3, #4
003c626c: str      r3, [r5, #0x2c]
003c6270: b        #0x3c61b0
003c6274: subseq   lr, ip, r0, lsr #18
003c6278: andeq    r2, r0, r0, asr #17
003c627c: andeq    r4, r0, r4, asr #16
003c6280: strdeq   r3, r4, [r0], -r4
003c6284: strdeq   lr, pc, [pc], #-0x94
003c6288: subeq    lr, pc, r8, ror #19

# _ZNK16CharStateMachine12SM_IsStunnedEb
003c0378: cmp      r1, #0
003c037c: push     {r4, lr}
003c0380: beq      #0x3c0390
003c0384: ldr      r0, [r0, #0x2c]
003c0388: ubfx     r0, r0, #1, #1
003c038c: pop      {r4, pc}
003c0390: bl       #0x3c01ac
003c0394: cmp      r0, #9
003c0398: movne    r0, #0
003c039c: moveq    r0, #1
003c03a0: pop      {r4, pc}

# _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
0037cb24: ldr      r3, [pc, #0x58]
0037cb28: ldr      r2, [pc, #0x58]
0037cb2c: push     {r4, r5, r6, lr}
0037cb30: add      r3, pc, r3
0037cb34: ldr      r5, [r3, r2]
0037cb38: sub      sp, sp, #0x78
0037cb3c: add      r4, sp, #4
0037cb40: ldr      r3, [r5]
0037cb44: str      r3, [sp, #0x74]
0037cb48: ldr      r6, [r0, #0x24]
0037cb4c: mov      r0, r4
0037cb50: bl       #0x37ca9c
0037cb54: mov      r0, r6
0037cb58: mov      r1, r4
0037cb5c: bl       #0x3195c0
0037cb60: mov      r0, r4
0037cb64: bl       #0x3193e8
0037cb68: ldr      r2, [sp, #0x74]
0037cb6c: ldr      r3, [r5]
0037cb70: cmp      r2, r3
0037cb74: bne      #0x37cb80
0037cb78: add      sp, sp, #0x78
0037cb7c: pop      {r4, r5, r6, pc}
0037cb80: bl       #0x30e310
0037cb84: rsbeq    r7, r1, r0, ror #30
0037cb88: andeq    r4, r0, ip, lsr #1

# _ZN6CSIdle16IdleCommonUpdateEiP9CharacterP16CharStateMachine
003c0b78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c0b7c: ldrb     r3, [r1, #0x1b5]
003c0b80: ldr      r5, [pc, #0x2e0]
003c0b84: sub      sp, sp, #0x4c
003c0b88: cmp      r3, #0
003c0b8c: mov      r4, r1
003c0b90: add      r5, pc, r5
003c0b94: bne      #0x3c0bb8
003c0b98: ldr      r3, [r1]
003c0b9c: mov      r0, r1
003c0ba0: mov      lr, pc
003c0ba4: ldr      pc, [r3, #0x28]
003c0ba8: cmp      r0, #0
003c0bac: bne      #0x3c0bcc
003c0bb0: add      sp, sp, #0x4c
003c0bb4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c0bb8: mov      r1, #0
003c0bbc: mov      r0, r4
003c0bc0: mov      r2, r1
003c0bc4: bl       #0x3a4d5c
003c0bc8: b        #0x3c0bb0
003c0bcc: bl       #0x7fd794
003c0bd0: ldrb     r6, [r0, #5]
003c0bd4: cmp      r6, #0
003c0bd8: bne      #0x3c0bb0
003c0bdc: ldr      r2, [pc, #0x288]
003c0be0: ldr      r1, [pc, #0x288]
003c0be4: ldr      r7, [r5, r2]
003c0be8: str      r2, [sp, #0x14]
003c0bec: ldr      r2, [pc, #0x280]
003c0bf0: add      r1, pc, r1
003c0bf4: ldr      r0, [r7, #0x2c]
003c0bf8: add      r2, pc, r2
003c0bfc: bl       #0x4c4bdc
003c0c00: ldr      r3, [r4, #0x55c]
003c0c04: mov      sb, r0
003c0c08: cmp      r0, r3
003c0c0c: bhi      #0x3c0bb0
003c0c10: ldr      r8, [r7, #0x40]
003c0c14: mov      r0, r8
003c0c18: bl       #0x36d7a8
003c0c1c: subs     sl, r0, #0
003c0c20: ble      #0x3c0bb0
003c0c24: ldr      r3, [pc, #0x24c]
003c0c28: add      r2, sp, #0x30
003c0c2c: str      r2, [sp, #0x24]
003c0c30: add      r3, pc, r3
003c0c34: str      r3, [sp, #0x28]
003c0c38: ldr      r3, [pc, #0x23c]
003c0c3c: mov      r7, r5
003c0c40: add      r3, pc, r3
003c0c44: str      r3, [sp, #0x2c]
003c0c48: add      r3, sp, #0x3c
003c0c4c: str      r3, [sp, #0x20]
003c0c50: b        #0x3c0c60
003c0c54: add      r6, r6, #1
003c0c58: cmp      r6, sl
003c0c5c: beq      #0x3c0bb0
003c0c60: mov      r0, r8
003c0c64: mov      r1, r6
003c0c68: mov      r2, #0
003c0c6c: bl       #0x36e744
003c0c70: ldr      r5, [r0, #0x660]
003c0c74: cmp      r5, #0
003c0c78: cmpne    r4, r5
003c0c7c: beq      #0x3c0c54
003c0c80: add      r0, r5, #0x4f0
003c0c84: add      r0, r0, #0xc
003c0c88: bl       #0x3c01ac
003c0c8c: cmp      r0, #3
003c0c90: bne      #0x3c0c54
003c0c94: ldr      r3, [r5, #0x55c]
003c0c98: cmp      sb, r3
003c0c9c: bhi      #0x3c0c54
003c0ca0: ldr      r2, [sp, #0x14]
003c0ca4: ldr      r1, [sp, #0x28]
003c0ca8: ldr      r3, [r7, r2]
003c0cac: ldr      r2, [sp, #0x2c]
003c0cb0: ldr      r0, [r3, #0x2c]
003c0cb4: bl       #0x4c4bdc
003c0cb8: bl       #0x30e964
003c0cbc: str      r0, [sp, #0xc]
003c0cc0: ldr      r1, [r5, #0x160]
003c0cc4: ldr      r0, [r4, #0x160]
003c0cc8: bl       #0x30e3ac
003c0ccc: str      r0, [sp, #0x10]
003c0cd0: ldr      r1, [r5, #0x164]
003c0cd4: ldr      r0, [r4, #0x164]
003c0cd8: bl       #0x30e3ac
003c0cdc: str      r0, [sp, #0x18]
003c0ce0: ldr      r1, [r5, #0x168]
003c0ce4: ldr      r0, [r4, #0x168]
003c0ce8: bl       #0x30e3ac
003c0cec: str      r0, [sp, #0x1c]
003c0cf0: ldr      r0, [sp, #0x10]
003c0cf4: mov      r1, r0
003c0cf8: bl       #0x30ed6c
003c0cfc: mov      fp, r0
003c0d00: ldr      r0, [sp, #0x18]
003c0d04: mov      r1, r0
003c0d08: bl       #0x30ed6c
003c0d0c: mov      r1, r0
003c0d10: mov      r0, fp
003c0d14: bl       #0x30eba4
003c0d18: mov      fp, r0
003c0d1c: ldr      r0, [sp, #0x1c]
003c0d20: mov      r1, r0
003c0d24: bl       #0x30ed6c
003c0d28: mov      r1, r0
003c0d2c: mov      r0, fp
003c0d30: bl       #0x30eba4
003c0d34: mov      fp, r0
003c0d38: ldr      r0, [sp, #0xc]
003c0d3c: mov      r1, r0
003c0d40: bl       #0x30ed6c
003c0d44: mov      r1, fp
003c0d48: bl       #0x30e2f8
003c0d4c: cmp      r0, #0
003c0d50: beq      #0x3c0c54
003c0d54: mov      r0, fp
003c0d58: mov      r1, #0
003c0d5c: bl       #0x30e2f8
003c0d60: cmp      r0, #0
003c0d64: beq      #0x3c0c54
003c0d68: mov      r0, fp
003c0d6c: bl       #0x30e124
003c0d70: mov      fp, r0
003c0d74: mov      r1, fp
003c0d78: ldr      r0, [sp, #0xc]
003c0d7c: bl       #0x30e3ac
003c0d80: mov      r1, #0x3f400000
003c0d84: bl       #0x30ed6c
003c0d88: mov      r1, fp
003c0d8c: bl       #0x30ec94
003c0d90: ldr      r1, [sp, #0x10]
003c0d94: mov      fp, r0
003c0d98: bl       #0x30ed6c
003c0d9c: ldr      r1, [sp, #0x18]
003c0da0: str      r0, [sp, #0xc]
003c0da4: mov      r0, fp
003c0da8: bl       #0x30ed6c
003c0dac: ldr      r1, [sp, #0x1c]
003c0db0: str      r0, [sp, #0x10]
003c0db4: mov      r0, fp
003c0db8: bl       #0x30ed6c
003c0dbc: ldr      r1, [r4, #0x164]
003c0dc0: mov      fp, r0
003c0dc4: ldr      r0, [sp, #0x10]
003c0dc8: bl       #0x30eba4
003c0dcc: ldr      r1, [r4, #0x168]
003c0dd0: mov      r2, r0
003c0dd4: mov      r0, fp
003c0dd8: str      r2, [sp, #4]
003c0ddc: bl       #0x30eba4
003c0de0: ldr      r1, [r4, #0x160]
003c0de4: mov      ip, r0
003c0de8: ldr      r0, [sp, #0xc]
003c0dec: str      ip, [sp, #8]
003c0df0: bl       #0x30eba4
003c0df4: ldr      r3, [r4, #0x378]
003c0df8: ldmib    sp, {r2, ip}
003c0dfc: str      r0, [sp, #0x3c]
003c0e00: ldr      r1, [sp, #0x20]
003c0e04: mov      r0, r3
003c0e08: str      r2, [sp, #0x40]
003c0e0c: str      ip, [sp, #0x44]
003c0e10: bl       #0x4054e4
003c0e14: ldr      r0, [r5, #0x164]
003c0e18: ldr      r1, [sp, #0x10]
003c0e1c: bl       #0x30e3ac
003c0e20: mov      r1, fp
003c0e24: mov      r3, r0
003c0e28: ldr      r0, [r5, #0x168]
003c0e2c: str      r3, [sp, #8]
003c0e30: bl       #0x30e3ac
003c0e34: ldr      r1, [sp, #0xc]
003c0e38: mov      fp, r0
003c0e3c: ldr      r0, [r5, #0x160]
003c0e40: bl       #0x30e3ac
003c0e44: ldr      r2, [r5, #0x378]
003c0e48: ldr      r3, [sp, #8]
003c0e4c: str      r0, [sp, #0x30]
003c0e50: ldr      r1, [sp, #0x24]
003c0e54: mov      r0, r2
003c0e58: str      r3, [sp, #0x34]
003c0e5c: str      fp, [sp, #0x38]
003c0e60: bl       #0x4054e4
003c0e64: b        #0x3c0c54
003c0e68: subseq   r3, sp, r0, lsl #30
003c0e6c: strdeq   r3, r4, [r0], -r4
003c0e70: subseq   r0, r0, r0, ror #22
003c0e74: subseq   r3, r0, r0, lsl #31
003c0e78: subseq   r0, r0, r0, lsr #22
003c0e7c: subseq   r3, r0, r8, asr pc
