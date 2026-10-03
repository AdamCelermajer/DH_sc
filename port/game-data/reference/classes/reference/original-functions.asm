
# _ZN7Structs9ClassFunc4readEP11IStreamBase
004f19dc: push     {r4, r5, lr}
004f19e0: mov      r4, r0
004f19e4: sub      sp, sp, #0xc
004f19e8: mov      r0, r1
004f19ec: mov      r5, r1
004f19f0: add      r1, r4, #4
004f19f4: bl       #0x459090
004f19f8: mov      r3, #1
004f19fc: cmp      r3, #0
004f1a00: str      r3, [sp, #4]
004f1a04: bne      #0x4f1a48
004f1a08: add      r3, r4, #5
004f1a0c: add      r2, r4, #6
004f1a10: ldrb     r0, [r2, #1]
004f1a14: ldrb     r1, [r3, #-1]
004f1a18: cmp      r2, r3
004f1a1c: eor      r1, r0, r1
004f1a20: strb     r1, [r3, #-1]
004f1a24: ldrb     r0, [r2, #1]
004f1a28: eor      r1, r1, r0
004f1a2c: strb     r1, [r2, #1]
004f1a30: ldrb     r0, [r3, #-1]
004f1a34: sub      r2, r2, #1
004f1a38: eor      r1, r1, r0
004f1a3c: strb     r1, [r3, #-1]
004f1a40: add      r3, r3, #1
004f1a44: bhi      #0x4f1a10
004f1a48: mov      r0, r5
004f1a4c: add      r1, r4, #8
004f1a50: bl       #0x459090
004f1a54: mov      r3, #1
004f1a58: cmp      r3, #0
004f1a5c: str      r3, [sp, #4]
004f1a60: bne      #0x4f1aa4
004f1a64: add      r3, r4, #9
004f1a68: add      r2, r4, #0xa
004f1a6c: ldrb     r0, [r2, #1]
004f1a70: ldrb     r1, [r3, #-1]
004f1a74: cmp      r2, r3
004f1a78: eor      r1, r0, r1
004f1a7c: strb     r1, [r3, #-1]
004f1a80: ldrb     r0, [r2, #1]
004f1a84: eor      r1, r1, r0
004f1a88: strb     r1, [r2, #1]
004f1a8c: ldrb     r0, [r3, #-1]
004f1a90: sub      r2, r2, #1
004f1a94: eor      r1, r1, r0
004f1a98: strb     r1, [r3, #-1]
004f1a9c: add      r3, r3, #1
004f1aa0: bhi      #0x4f1a6c
004f1aa4: mov      r0, r5
004f1aa8: add      r1, r4, #0xc
004f1aac: bl       #0x459090
004f1ab0: mov      r3, #1
004f1ab4: cmp      r3, #0
004f1ab8: str      r3, [sp, #4]
004f1abc: bne      #0x4f1b00
004f1ac0: add      r3, r4, #0xd
004f1ac4: add      r2, r4, #0xe
004f1ac8: ldrb     r0, [r2, #1]
004f1acc: ldrb     r1, [r3, #-1]
004f1ad0: cmp      r2, r3
004f1ad4: eor      r1, r0, r1
004f1ad8: strb     r1, [r3, #-1]
004f1adc: ldrb     r0, [r2, #1]
004f1ae0: eor      r1, r1, r0
004f1ae4: strb     r1, [r2, #1]
004f1ae8: ldrb     r0, [r3, #-1]
004f1aec: sub      r2, r2, #1
004f1af0: eor      r1, r1, r0
004f1af4: strb     r1, [r3, #-1]
004f1af8: add      r3, r3, #1
004f1afc: bhi      #0x4f1ac8
004f1b00: mov      r0, r5
004f1b04: add      r1, r4, #0x10
004f1b08: bl       #0x459090
004f1b0c: mov      r3, #1
004f1b10: cmp      r3, #0
004f1b14: str      r3, [sp, #4]
004f1b18: bne      #0x4f1b5c
004f1b1c: add      r3, r4, #0x11
004f1b20: add      r2, r4, #0x12
004f1b24: ldrb     r0, [r2, #1]
004f1b28: ldrb     r1, [r3, #-1]
004f1b2c: cmp      r2, r3
004f1b30: eor      r1, r0, r1
004f1b34: strb     r1, [r3, #-1]
004f1b38: ldrb     r0, [r2, #1]
004f1b3c: eor      r1, r1, r0
004f1b40: strb     r1, [r2, #1]
004f1b44: ldrb     r0, [r3, #-1]
004f1b48: sub      r2, r2, #1
004f1b4c: eor      r1, r1, r0
004f1b50: strb     r1, [r3, #-1]
004f1b54: add      r3, r3, #1
004f1b58: bhi      #0x4f1b24
004f1b5c: mov      r0, r5
004f1b60: add      r1, r4, #0x14
004f1b64: bl       #0x459090
004f1b68: mov      r3, #1
004f1b6c: cmp      r3, #0
004f1b70: str      r3, [sp, #4]
004f1b74: bne      #0x4f1bb8
004f1b78: add      r3, r4, #0x16
004f1b7c: add      r4, r4, #0x15
004f1b80: ldrb     r1, [r3, #1]
004f1b84: ldrb     r2, [r4, #-1]
004f1b88: cmp      r3, r4
004f1b8c: eor      r2, r1, r2
004f1b90: strb     r2, [r4, #-1]
004f1b94: ldrb     r1, [r3, #1]
004f1b98: eor      r2, r2, r1
004f1b9c: strb     r2, [r3, #1]
004f1ba0: ldrb     r1, [r4, #-1]
004f1ba4: sub      r3, r3, #1
004f1ba8: eor      r2, r2, r1
004f1bac: strb     r2, [r4, #-1]
004f1bb0: add      r4, r4, #1
004f1bb4: bhi      #0x4f1b80
004f1bb8: add      sp, sp, #0xc
004f1bbc: pop      {r4, r5, pc}

# _ZN14CharProperties10_LoadClassERN7Structs19CharacterPropertiesEib
003e2e20: ldr      ip, [pc, #0x1e0]
003e2e24: cmp      r2, #0
003e2e28: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003e2e2c: add      ip, pc, ip
003e2e30: mov      r8, r0
003e2e34: mov      sl, r1
003e2e38: mov      sb, r3
003e2e3c: blt      #0x3e2ee0
003e2e40: ldr      r3, [pc, #0x1c4]
003e2e44: ldr      r3, [ip, r3]
003e2e48: ldr      r3, [r3]
003e2e4c: cmp      r2, r3
003e2e50: bge      #0x3e2ee0
003e2e54: ldr      r3, [pc, #0x1b4]
003e2e58: mov      r7, #0xc
003e2e5c: ldr      r3, [ip, r3]
003e2e60: ldr      r3, [r3]
003e2e64: mla      r7, r7, r2, r3
003e2e68: ldr      r2, [r7, #4]
003e2e6c: cmp      r2, #0
003e2e70: beq      #0x3e2ee0
003e2e74: mov      r4, #0
003e2e78: mov      r6, r4
003e2e7c: ldr      r5, [r7, #8]
003e2e80: add      r5, r5, r4
003e2e84: ldr      r3, [r5, #8]
003e2e88: cmp      r3, #9
003e2e8c: addls    pc, pc, r3, lsl #2
003e2e90: b        #0x3e2ed0
003e2e94: b        #0x3e2fdc
003e2e98: b        #0x3e2fb0
003e2e9c: b        #0x3e2f88
003e2ea0: b        #0x3e2ed0
003e2ea4: b        #0x3e2f60
003e2ea8: b        #0x3e2f34
003e2eac: b        #0x3e2ee4
003e2eb0: b        #0x3e2ef8
003e2eb4: b        #0x3e2f20
003e2eb8: b        #0x3e2ebc
003e2ebc: mov      r2, r5
003e2ec0: mov      r0, r8
003e2ec4: mov      r1, sl
003e2ec8: bl       #0x3e2bd0
003e2ecc: ldr      r2, [r7, #4]
003e2ed0: add      r6, r6, #1
003e2ed4: cmp      r2, r6
003e2ed8: add      r4, r4, #0x18
003e2edc: bhi      #0x3e2e7c
003e2ee0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003e2ee4: mov      r0, r8
003e2ee8: mov      r1, sl
003e2eec: mov      r2, r5
003e2ef0: mov      r3, sb
003e2ef4: bl       #0x3e2c10
003e2ef8: mov      r2, r5
003e2efc: mov      r0, r8
003e2f00: mov      r1, sl
003e2f04: bl       #0x3e2bdc
003e2f08: ldr      r2, [r7, #4]
003e2f0c: add      r6, r6, #1
003e2f10: add      r4, r4, #0x18
003e2f14: cmp      r2, r6
003e2f18: bhi      #0x3e2e7c
003e2f1c: b        #0x3e2ee0
003e2f20: mov      r0, r8
003e2f24: mov      r1, sl
003e2f28: mov      r2, r5
003e2f2c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003e2f30: b        #0x3e2bcc
003e2f34: mov      r2, r5
003e2f38: mov      r0, r8
003e2f3c: mov      r1, sl
003e2f40: mov      r3, sb
003e2f44: bl       #0x3e2c78
003e2f48: ldr      r2, [r7, #4]
003e2f4c: add      r6, r6, #1
003e2f50: add      r4, r4, #0x18
003e2f54: cmp      r2, r6
003e2f58: bhi      #0x3e2e7c
003e2f5c: b        #0x3e2ee0
003e2f60: mov      r2, r5
003e2f64: mov      r0, r8
003e2f68: mov      r1, sl
003e2f6c: bl       #0x3e2cc0
003e2f70: ldr      r2, [r7, #4]
003e2f74: add      r6, r6, #1
003e2f78: add      r4, r4, #0x18
003e2f7c: cmp      r2, r6
003e2f80: bhi      #0x3e2e7c
003e2f84: b        #0x3e2ee0
003e2f88: mov      r2, r5
003e2f8c: mov      r0, r8
003e2f90: mov      r1, sl
003e2f94: bl       #0x3e2d04
003e2f98: ldr      r2, [r7, #4]
003e2f9c: add      r6, r6, #1
003e2fa0: add      r4, r4, #0x18
003e2fa4: cmp      r2, r6
003e2fa8: bhi      #0x3e2e7c
003e2fac: b        #0x3e2ee0
003e2fb0: mov      r2, r5
003e2fb4: mov      r0, r8
003e2fb8: mov      r1, sl
003e2fbc: mov      r3, sb
003e2fc0: bl       #0x3e2d4c
003e2fc4: ldr      r2, [r7, #4]
003e2fc8: add      r6, r6, #1
003e2fcc: add      r4, r4, #0x18
003e2fd0: cmp      r2, r6
003e2fd4: bhi      #0x3e2e7c
003e2fd8: b        #0x3e2ee0
003e2fdc: mov      r2, r5
003e2fe0: mov      r0, r8
003e2fe4: mov      r1, sl
003e2fe8: mov      r3, sb
003e2fec: bl       #0x3e3014
003e2ff0: ldr      r2, [r7, #4]
003e2ff4: add      r6, r6, #1
003e2ff8: add      r4, r4, #0x18
003e2ffc: cmp      r2, r6
003e3000: bhi      #0x3e2e7c
003e3004: b        #0x3e2ee0
003e3008: subseq   r1, fp, r4, ror #24
003e300c: andeq    r3, r0, r8, ror #10
003e3010: muleq    r0, r0, r4

# _ZN14CharProperties18_LoadFromCharTableERN7Structs19CharacterPropertiesEi
003df250: ldr      r3, [pc, #0x40]
003df254: subs     ip, r2, #0
003df258: add      r3, pc, r3
003df25c: bxlt     lr
003df260: ldr      r2, [pc, #0x34]
003df264: ldr      r2, [r3, r2]
003df268: ldr      r2, [r2]
003df26c: cmp      ip, r2
003df270: bxge     lr
003df274: ldr      r2, [pc, #0x24]
003df278: add      r0, r1, #4
003df27c: mov      r1, #0x384
003df280: ldr      r3, [r3, r2]
003df284: mov      r2, #0x380
003df288: ldr      r3, [r3]
003df28c: mla      ip, r1, ip, r3
003df290: add      r1, ip, #4
003df294: b        #0x30e868
003df298: subseq   r5, fp, r8, lsr r8
003df29c: andeq    r4, r0, r4, lsl #4
003df2a0: andeq    r2, r0, r0, asr fp

# _ZN14CharProperties7_MinMaxERN7Structs19CharacterPropertiesEPNS0_9ClassFuncE
003e2d04: push     {r4, r5, r6, lr}
003e2d08: mov      r4, r2
003e2d0c: ldr      r2, [r2, #4]
003e2d10: mov      r6, r0
003e2d14: mov      r5, r1
003e2d18: bl       #0x3dedb4
003e2d1c: ldr      r3, [r4, #0xc]
003e2d20: cmp      r0, r3
003e2d24: blt      #0x3e2d38
003e2d28: ldr      r3, [r4, #0x10]
003e2d2c: cmp      r0, r3
003e2d30: movlt    r3, r0
003e2d34: movge    r3, r3
003e2d38: ldr      r2, [r4, #4]
003e2d3c: mov      r0, r6
003e2d40: mov      r1, r5
003e2d44: pop      {r4, r5, r6, lr}
003e2d48: b        #0x3deca0

# _ZN14CharProperties23PROPS_ApplyClassToSheetEiPN7Structs19CharacterPropertiesE
003df314: str      lr, [sp, #-4]!
003df318: ldr      r3, [pc, #0x80]
003df31c: subs     ip, r2, #0
003df320: sub      sp, sp, #0xc
003df324: mov      r2, r1
003df328: add      r3, pc, r3
003df32c: beq      #0x3df344
003df330: mov      r1, ip
003df334: mov      r3, #1
003df338: add      sp, sp, #0xc
003df33c: pop      {lr}
003df340: b        #0x3e2e20
003df344: ldr      r2, [pc, #0x58]
003df348: ldr      r2, [r3, r2]
003df34c: ldr      r2, [r2]
003df350: cmp      r2, #2
003df354: streq    ip, [ip]
003df358: beq      #0x3df364
003df35c: cmp      r2, #1
003df360: beq      #0x3df36c
003df364: add      sp, sp, #0xc
003df368: ldm      sp!, {pc}
003df36c: ldr      r0, [pc, #0x34]
003df370: ldr      r1, [pc, #0x34]
003df374: ldr      r2, [pc, #0x34]
003df378: ldr      r0, [r3, r0]
003df37c: ldr      r3, [pc, #0x30]
003df380: mov      ip, #0x31c
003df384: add      r1, pc, r1
003df388: add      r2, pc, r2
003df38c: add      r3, pc, r3
003df390: add      r0, r0, #0xa8
003df394: str      ip, [sp]
003df398: bl       #0x30e004
003df39c: b        #0x3df364
003df3a0: subseq   r5, fp, r8, ror #14
003df3a4: andeq    r3, r0, r0, asr #19
003df3a8: andeq    r1, r0, r0, asr #19
003df3ac: subeq    pc, sp, r4, asr r0
003df3b0: subeq    r4, lr, r0, asr #22
003df3b4: subeq    r6, lr, r4, lsr #18

# _ZN14CharProperties6_GroupERN7Structs19CharacterPropertiesEPNS0_9ClassFuncEb
003e3014: push     {r4, r5, r6, r7, r8, lr}
003e3018: mov      r4, r2
003e301c: ldr      r2, [r2, #0xc]
003e3020: mov      r7, r0
003e3024: mov      r6, r1
003e3028: cmn      r2, #1
003e302c: mov      r5, r3
003e3030: beq      #0x3e3038
003e3034: bl       #0x3e2e20
003e3038: ldr      r2, [r4, #0x10]
003e303c: cmn      r2, #1
003e3040: beq      #0x3e3054
003e3044: mov      r0, r7
003e3048: mov      r1, r6
003e304c: mov      r3, r5
003e3050: bl       #0x3e2e20
003e3054: ldr      r2, [r4, #0x14]
003e3058: cmn      r2, #1
003e305c: beq      #0x3e3074
003e3060: mov      r0, r7
003e3064: mov      r1, r6
003e3068: mov      r3, r5
003e306c: pop      {r4, r5, r6, r7, r8, lr}
003e3070: b        #0x3e2e20
003e3074: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14CharProperties13_AddOtherPropERN7Structs19CharacterPropertiesEPNS0_9ClassFuncE
003e2cc0: push     {r4, r5, r6, r7, r8, lr}
003e2cc4: mov      r4, r2
003e2cc8: ldr      r2, [r2, #4]
003e2ccc: mov      r5, r0
003e2cd0: mov      r6, r1
003e2cd4: bl       #0x3dedb4
003e2cd8: mov      r1, r6
003e2cdc: mov      r7, r0
003e2ce0: ldr      r2, [r4, #0xc]
003e2ce4: mov      r0, r5
003e2ce8: bl       #0x3dedb4
003e2cec: ldr      r2, [r4, #4]
003e2cf0: add      r3, r0, r7
003e2cf4: mov      r1, r6
003e2cf8: mov      r0, r5
003e2cfc: pop      {r4, r5, r6, r7, r8, lr}
003e2d00: b        #0x3deca0

# _ZN7Structs13ClassFuncList4readEP11IStreamBase
004dcfec: push     {r4, r5, r6, r7, lr}
004dcff0: mov      r5, r0
004dcff4: sub      sp, sp, #0xc
004dcff8: mov      r0, r1
004dcffc: mov      r7, r1
004dd000: ldr      r6, [pc, #0x148]
004dd004: add      r1, r5, #4
004dd008: bl       #0x3df1a0
004dd00c: mov      r3, #1
004dd010: cmp      r3, #0
004dd014: str      r3, [sp, #4]
004dd018: add      r6, pc, r6
004dd01c: bne      #0x4dd060
004dd020: add      r3, r5, #5
004dd024: add      r2, r5, #6
004dd028: ldrb     r0, [r2, #1]
004dd02c: ldrb     r1, [r3, #-1]
004dd030: cmp      r3, r2
004dd034: eor      r1, r0, r1
004dd038: strb     r1, [r3, #-1]
004dd03c: ldrb     r0, [r2, #1]
004dd040: eor      r1, r1, r0
004dd044: strb     r1, [r2, #1]
004dd048: ldrb     r0, [r3, #-1]
004dd04c: sub      r2, r2, #1
004dd050: eor      r1, r1, r0
004dd054: strb     r1, [r3, #-1]
004dd058: add      r3, r3, #1
004dd05c: blo      #0x4dd028
004dd060: ldr      r3, [r5, #8]
004dd064: cmp      r3, #0
004dd068: beq      #0x4dd0b0
004dd06c: ldr      r2, [r3, #-4]
004dd070: mov      r0, #0x18
004dd074: mla      r0, r0, r2, r3
004dd078: cmp      r3, r0
004dd07c: bne      #0x4dd088
004dd080: b        #0x4dd0a8
004dd084: mov      r0, r4
004dd088: sub      r4, r0, #0x18
004dd08c: ldr      r3, [r0, #-0x18]
004dd090: mov      r0, r4
004dd094: mov      lr, pc
004dd098: ldr      pc, [r3]
004dd09c: ldr      r0, [r5, #8]
004dd0a0: cmp      r0, r4
004dd0a4: bne      #0x4dd084
004dd0a8: sub      r0, r0, #8
004dd0ac: bl       #0x310440
004dd0b0: ldr      r4, [r5, #4]
004dd0b4: mov      r1, #1
004dd0b8: add      r0, r4, r4, lsl #1
004dd0bc: add      r0, r0, r1
004dd0c0: lsl      r0, r0, #3
004dd0c4: bl       #0x31056c
004dd0c8: mov      r3, #0x18
004dd0cc: cmp      r4, #0
004dd0d0: stm      r0, {r3, r4}
004dd0d4: add      r3, r0, #8
004dd0d8: beq      #0x4dd100
004dd0dc: ldr      r1, [pc, #0x70]
004dd0e0: mov      r2, #0
004dd0e4: ldr      r1, [r6, r1]
004dd0e8: add      r1, r1, #8
004dd0ec: add      r2, r2, #1
004dd0f0: cmp      r2, r4
004dd0f4: str      r1, [r0, #8]
004dd0f8: add      r0, r0, #0x18
004dd0fc: bne      #0x4dd0ec
004dd100: ldr      r2, [r5, #4]
004dd104: str      r3, [r5, #8]
004dd108: cmp      r2, #0
004dd10c: beq      #0x4dd148
004dd110: mov      r4, #0
004dd114: mov      r6, r4
004dd118: b        #0x4dd120
004dd11c: ldr      r3, [r5, #8]
004dd120: add      r0, r3, r4
004dd124: mov      r1, r7
004dd128: ldr      r3, [r3, r4]
004dd12c: mov      lr, pc
004dd130: ldr      pc, [r3, #0xc]
004dd134: ldr      r3, [r5, #4]
004dd138: add      r6, r6, #1
004dd13c: add      r4, r4, #0x18
004dd140: cmp      r3, r6
004dd144: bhi      #0x4dd11c
004dd148: add      sp, sp, #0xc
004dd14c: pop      {r4, r5, r6, r7, pc}
004dd150: subeq    r7, fp, r8, ror sl
004dd154: ldrdeq   r2, r3, [r0], -r0

# _ZN14CharProperties17_ApplyGroupOnlyIfERN7Structs19CharacterPropertiesEPNS0_9ClassFuncE
003e2bcc: bx       lr

# _ZN14CharProperties12_SetPropertyERN7Structs19CharacterPropertiesEii
003deca0: str      lr, [sp, #-4]!
003deca4: ldr      ip, [pc, #0xe0]
003deca8: cmp      r2, #0
003decac: sub      sp, sp, #0xc
003decb0: add      ip, pc, ip
003decb4: blt      #0x3dece4
003decb8: cmp      r2, #0xdf
003decbc: ble      #0x3ded04
003decc0: ldr      r3, [pc, #0xc8]
003decc4: ldr      r3, [ip, r3]
003decc8: ldr      r3, [r3]
003deccc: cmp      r3, #2
003decd0: beq      #0x3decf8
003decd4: cmp      r3, #1
003decd8: beq      #0x3ded58
003decdc: add      sp, sp, #0xc
003dece0: ldm      sp!, {pc}
003dece4: ldr      r3, [pc, #0xa4]
003dece8: ldr      r3, [ip, r3]
003decec: ldr      r3, [r3]
003decf0: cmp      r3, #2
003decf4: bne      #0x3ded1c
003decf8: mov      r3, #0
003decfc: str      r3, [r3]
003ded00: b        #0x3decdc
003ded04: ldr      r0, [pc, #0x88]
003ded08: ldr      r0, [ip, r0]
003ded0c: ldr      r2, [r0, r2, lsl #2]
003ded10: add      r1, r1, r2
003ded14: str      r3, [r1, #4]
003ded18: b        #0x3decdc
003ded1c: cmp      r3, #1
003ded20: bne      #0x3decdc
003ded24: ldr      r0, [pc, #0x6c]
003ded28: ldr      r1, [pc, #0x6c]
003ded2c: ldr      r2, [pc, #0x6c]
003ded30: ldr      r0, [ip, r0]
003ded34: ldr      r3, [pc, #0x68]
003ded38: movw     ip, #0x113
003ded3c: add      r1, pc, r1
003ded40: add      r2, pc, r2
003ded44: add      r3, pc, r3
003ded48: add      r0, r0, #0xa8
003ded4c: str      ip, [sp]
003ded50: bl       #0x30e004
003ded54: b        #0x3decdc
003ded58: ldr      r0, [pc, #0x38]
003ded5c: ldr      r1, [pc, #0x44]
003ded60: ldr      r2, [pc, #0x44]
003ded64: ldr      r0, [ip, r0]
003ded68: ldr      r3, [pc, #0x40]
003ded6c: mov      ip, #0x114
003ded70: add      r1, pc, r1
003ded74: add      r2, pc, r2
003ded78: add      r3, pc, r3
003ded7c: add      r0, r0, #0xa8
003ded80: str      ip, [sp]
003ded84: bl       #0x30e004
003ded88: b        #0x3decdc
003ded8c: subseq   r5, fp, r0, ror #27
003ded90: andeq    r3, r0, r0, asr #19
003ded94: andeq    r2, r0, r8, lsr #5
003ded98: andeq    r1, r0, r0, asr #19
003ded9c: umaaleq  pc, sp, ip, r6
003deda0: ldrdeq   r6, r7, [lr], #-0xf0
003deda4: subeq    r6, lr, ip, ror #30
003deda8: subeq    pc, sp, r8, ror #12
003dedac: subeq    r6, lr, ip, lsr #31
003dedb0: subeq    r6, lr, r8, lsr pc

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

# _ZN14CharProperties7_SetOIDERN7Structs19CharacterPropertiesEPNS0_9ClassFuncE
003e2bd0: ldr      r3, [r2, #0xc]
003e2bd4: ldr      r2, [r2, #4]
003e2bd8: b        #0x3deca0

# _ZN14CharProperties27_ScaleWithPercentageForBuffERN7Structs19CharacterPropertiesEPNS0_9ClassFuncEb
003e2c10: push     {r4, r5, r6, r7, r8, lr}
003e2c14: mov      r4, r2
003e2c18: ldr      r2, [r2, #0xc]
003e2c1c: mov      r8, r3
003e2c20: mov      r5, r0
003e2c24: mov      r6, r1
003e2c28: bl       #0x3dedb4
003e2c2c: mov      r1, r6
003e2c30: mov      r7, r0
003e2c34: ldr      r2, [r4, #4]
003e2c38: mov      r0, r5
003e2c3c: bl       #0x3dedb4
003e2c40: cmp      r8, #0
003e2c44: beq      #0x3e2c5c
003e2c48: add      r1, r5, #0xa90
003e2c4c: add      r1, r1, #4
003e2c50: mov      r0, r5
003e2c54: ldr      r2, [r4, #4]
003e2c58: bl       #0x3dedb4
003e2c5c: mul      r3, r7, r0
003e2c60: ldr      r2, [r4, #4]
003e2c64: mov      r0, r5
003e2c68: mov      r1, r6
003e2c6c: asr      r3, r3, #8
003e2c70: pop      {r4, r5, r6, r7, r8, lr}
003e2c74: b        #0x3deca0

# _ZN14CharProperties9_AddValueERN7Structs19CharacterPropertiesEPNS0_9ClassFuncE
003e2bdc: push     {r4, r5, r6, lr}
003e2be0: mov      r5, r2
003e2be4: ldr      r2, [r2, #4]
003e2be8: mov      r4, r0
003e2bec: mov      r6, r1
003e2bf0: bl       #0x3dedb4
003e2bf4: ldr      r3, [r5, #0xc]
003e2bf8: ldr      r2, [r5, #4]
003e2bfc: mov      r1, r6
003e2c00: add      r3, r0, r3
003e2c04: mov      r0, r4
003e2c08: pop      {r4, r5, r6, lr}
003e2c0c: b        #0x3deca0

# _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003dedb4: str      lr, [sp, #-4]!
003dedb8: ldr      r3, [pc, #0xf0]
003dedbc: cmp      r2, #0
003dedc0: sub      sp, sp, #0xc
003dedc4: add      r3, pc, r3
003dedc8: blt      #0x3dedfc
003dedcc: cmp      r2, #0xdf
003dedd0: ble      #0x3dee20
003dedd4: ldr      r2, [pc, #0xd8]
003dedd8: ldr      r2, [r3, r2]
003deddc: ldr      r2, [r2]
003dede0: cmp      r2, #2
003dede4: beq      #0x3dee10
003dede8: cmp      r2, #1
003dedec: beq      #0x3dee78
003dedf0: mvn      r0, #0
003dedf4: add      sp, sp, #0xc
003dedf8: ldm      sp!, {pc}
003dedfc: ldr      r2, [pc, #0xb0]
003dee00: ldr      r2, [r3, r2]
003dee04: ldr      r2, [r2]
003dee08: cmp      r2, #2
003dee0c: bne      #0x3dee38
003dee10: mov      r3, #0
003dee14: str      r3, [r3]
003dee18: mvn      r0, #0
003dee1c: b        #0x3dedf4
003dee20: ldr      r0, [pc, #0x90]
003dee24: ldr      r3, [r3, r0]
003dee28: ldr      r3, [r3, r2, lsl #2]
003dee2c: add      r1, r1, r3
003dee30: ldr      r0, [r1, #4]
003dee34: b        #0x3dedf4
003dee38: cmp      r2, #1
003dee3c: bne      #0x3dedf0
003dee40: ldr      r0, [pc, #0x74]
003dee44: ldr      r1, [pc, #0x74]
003dee48: ldr      r2, [pc, #0x74]
003dee4c: ldr      r0, [r3, r0]
003dee50: ldr      r3, [pc, #0x70]
003dee54: movw     ip, #0x103
003dee58: add      r1, pc, r1
003dee5c: add      r0, r0, #0xa8
003dee60: add      r2, pc, r2
003dee64: add      r3, pc, r3
003dee68: str      ip, [sp]
003dee6c: bl       #0x30e004
003dee70: mvn      r0, #0
003dee74: b        #0x3dedf4
003dee78: ldr      r0, [pc, #0x3c]
003dee7c: ldr      r1, [pc, #0x48]
003dee80: ldr      r2, [pc, #0x48]
003dee84: ldr      r0, [r3, r0]
003dee88: ldr      r3, [pc, #0x44]
003dee8c: mov      ip, #0x104
003dee90: add      r1, pc, r1
003dee94: add      r0, r0, #0xa8
003dee98: add      r2, pc, r2
003dee9c: add      r3, pc, r3
003deea0: str      ip, [sp]
003deea4: bl       #0x30e004
003deea8: mvn      r0, #0
003deeac: b        #0x3dedf4
003deeb0: subseq   r5, fp, ip, asr #25
003deeb4: andeq    r3, r0, r0, asr #19
003deeb8: andeq    r2, r0, r8, lsr #5
003deebc: andeq    r1, r0, r0, asr #19
003deec0: subeq    pc, sp, r0, lsl #11
003deec4: strheq   r6, [lr], #-0xe0
003deec8: subeq    r6, lr, ip, asr #28
003deecc: subeq    pc, sp, r8, asr #10
003deed0: subeq    r6, lr, r8, lsl #29
003deed4: subeq    r6, lr, r4, lsl lr

# _ZN14CharProperties20_ScaleWithPercentageERN7Structs19CharacterPropertiesEPNS0_9ClassFuncEb
003e2c78: push     {r4, r5, r6, r7, r8, lr}
003e2c7c: mov      r4, r2
003e2c80: ldr      r2, [r2, #0xc]
003e2c84: mov      r5, r0
003e2c88: mov      r6, r1
003e2c8c: bl       #0x3dedb4
003e2c90: mov      r1, r6
003e2c94: mov      r7, r0
003e2c98: ldr      r2, [r4, #4]
003e2c9c: mov      r0, r5
003e2ca0: bl       #0x3dedb4
003e2ca4: mul      r3, r7, r0
003e2ca8: ldr      r2, [r4, #4]
003e2cac: mov      r1, r6
003e2cb0: mov      r0, r5
003e2cb4: asr      r3, r3, #8
003e2cb8: pop      {r4, r5, r6, r7, r8, lr}
003e2cbc: b        #0x3deca0

# _ZN6Arrays10ClassTable4readEP11IStreamBase
004b4484: push     {r4, r5, r6, r7, r8, sl, lr}
004b4488: sub      sp, sp, #0xc
004b448c: mov      sl, r0
004b4490: bl       #0x313a90
004b4494: ldr      r6, [pc, #0x124]
004b4498: mov      r3, #1
004b449c: cmp      r3, #0
004b44a0: str      r0, [sp, #4]
004b44a4: str      r3, [sp]
004b44a8: add      r6, pc, r6
004b44ac: bne      #0x4b44f4
004b44b0: add      r3, sp, #4
004b44b4: add      r2, r3, #2
004b44b8: add      r3, r3, #1
004b44bc: ldrb     r0, [r2, #1]
004b44c0: ldrb     r1, [r3, #-1]
004b44c4: cmp      r2, r3
004b44c8: eor      r1, r0, r1
004b44cc: strb     r1, [r3, #-1]
004b44d0: ldrb     r0, [r2, #1]
004b44d4: eor      r1, r1, r0
004b44d8: strb     r1, [r2, #1]
004b44dc: ldrb     r0, [r3, #-1]
004b44e0: sub      r2, r2, #1
004b44e4: eor      r1, r1, r0
004b44e8: strb     r1, [r3, #-1]
004b44ec: add      r3, r3, #1
004b44f0: bhi      #0x4b44bc
004b44f4: bl       #0x4a99b8
004b44f8: ldr      r7, [pc, #0xc4]
004b44fc: ldr      r4, [sp, #4]
004b4500: mov      r5, #0xc
004b4504: ldr      r3, [r6, r7]
004b4508: mul      r0, r5, r4
004b450c: str      r4, [r3]
004b4510: add      r0, r0, #8
004b4514: mov      r1, #1
004b4518: bl       #0x31056c
004b451c: cmp      r4, #0
004b4520: str      r5, [r0]
004b4524: str      r4, [r0, #4]
004b4528: add      r3, r0, #8
004b452c: beq      #0x4b455c
004b4530: ldr      r1, [pc, #0x90]
004b4534: mov      r2, #0
004b4538: mov      ip, r2
004b453c: ldr      r1, [r6, r1]
004b4540: add      r1, r1, #8
004b4544: add      r2, r2, #1
004b4548: cmp      r2, r4
004b454c: str      r1, [r0, #8]
004b4550: str      ip, [r0, #0x10]
004b4554: add      r0, r0, #0xc
004b4558: bne      #0x4b4544
004b455c: ldr      r2, [r6, r7]
004b4560: ldr      r8, [pc, #0x64]
004b4564: ldr      r1, [r2]
004b4568: ldr      r2, [r6, r8]
004b456c: cmp      r1, #0
004b4570: str      r3, [r2]
004b4574: beq      #0x4b45b8
004b4578: mov      r4, #0
004b457c: mov      r5, r4
004b4580: b        #0x4b458c
004b4584: ldr      r3, [r6, r8]
004b4588: ldr      r3, [r3]
004b458c: add      r0, r3, r4
004b4590: mov      r1, sl
004b4594: ldr      r3, [r3, r4]
004b4598: mov      lr, pc
004b459c: ldr      pc, [r3, #0xc]
004b45a0: ldr      r3, [r6, r7]
004b45a4: add      r5, r5, #1
004b45a8: add      r4, r4, #0xc
004b45ac: ldr      r3, [r3]
004b45b0: cmp      r3, r5
004b45b4: bhi      #0x4b4584
004b45b8: add      sp, sp, #0xc
004b45bc: pop      {r4, r5, r6, r7, r8, sl, pc}
004b45c0: subeq    r0, lr, r8, ror #11
004b45c4: andeq    r3, r0, r8, ror #10
004b45c8: andeq    r4, r0, r4, asr ip
004b45cc: muleq    r0, r0, r4

# _ZN14CharProperties28_LinearWithPropPlusBaseValueERN7Structs19CharacterPropertiesEPNS0_9ClassFuncEb
003e2d4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2d50: mov      r4, r2
003e2d54: ldr      r6, [r2, #0xc]
003e2d58: sub      sp, sp, #4
003e2d5c: ldr      r2, [r2, #0x10]
003e2d60: mov      sb, r3
003e2d64: mov      r7, r0
003e2d68: mov      r5, r1
003e2d6c: bl       #0x3dedb4
003e2d70: ldr      r8, [pc, #0xa0]
003e2d74: mvn      r3, #0x298
003e2d78: sub      r3, r3, #1
003e2d7c: cmp      r6, r3
003e2d80: add      r8, pc, r8
003e2d84: mov      sl, r0
003e2d88: ldr      fp, [r4, #0x14]
003e2d8c: beq      #0x3e2e00
003e2d90: ldr      r3, [pc, #0x84]
003e2d94: ldr      r3, [r8, r3]
003e2d98: cmp      r5, r3
003e2d9c: beq      #0x3e2df4
003e2da0: cmp      sb, #0
003e2da4: beq      #0x3e2de0
003e2da8: add      r1, r7, #0xa90
003e2dac: add      r1, r1, #4
003e2db0: mov      r0, r7
003e2db4: ldr      r2, [r4, #0x10]
003e2db8: bl       #0x3dedb4
003e2dbc: mov      sl, r0
003e2dc0: asr      r3, sl, #8
003e2dc4: mla      r3, fp, r3, r6
003e2dc8: ldr      r2, [r4, #4]
003e2dcc: mov      r0, r7
003e2dd0: mov      r1, r5
003e2dd4: add      sp, sp, #4
003e2dd8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2ddc: b        #0x3deca0
003e2de0: mov      r0, r7
003e2de4: ldr      r1, [r4, #0x10]
003e2de8: bl       #0x3dfe60
003e2dec: mov      sl, r0
003e2df0: b        #0x3e2dc0
003e2df4: cmp      sb, #0
003e2df8: beq      #0x3e2dc0
003e2dfc: b        #0x3e2da8
003e2e00: mov      r0, r7
003e2e04: mov      r1, r5
003e2e08: ldr      r2, [r4, #4]
003e2e0c: bl       #0x3dedb4
003e2e10: mov      r6, r0
003e2e14: b        #0x3e2d90
003e2e18: subseq   r1, fp, r0, lsl sp
003e2e1c: andeq    r1, r0, ip, asr #32
