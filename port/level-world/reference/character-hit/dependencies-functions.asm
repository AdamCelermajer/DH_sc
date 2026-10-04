
# _ZN10ObjectBase9GetHandleEv
0033dd2c: ldr      r3, [pc, #0x34]
0033dd30: ldr      r2, [pc, #0x34]
0033dd34: push     {r4, lr}
0033dd38: add      r3, pc, r3
0033dd3c: ldr      r2, [r3, r2]
0033dd40: ldr      ip, [r1, #0x2c]
0033dd44: mov      r4, r0
0033dd48: ldr      lr, [r2, #0x38]
0033dd4c: mov      r2, #0xc
0033dd50: ldr      r3, [lr, #0x78]
0033dd54: str      r3, [ip, #8]
0033dd58: ldr      r1, [r1, #0x2c]
0033dd5c: bl       #0x30df38
0033dd60: mov      r0, r4
0033dd64: pop      {r4, pc}
0033dd68: rsbeq    r6, r5, r8, asr sp
0033dd6c: strdeq   r3, r4, [r0], -r4

# _ZN9Character6HitForEjP10GameObject
003a8bc4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a8bc8: ldr      r4, [pc, #0x64c]
003a8bcc: ldr      r6, [pc, #0x64c]
003a8bd0: mov      fp, r1
003a8bd4: add      r4, pc, r4
003a8bd8: ldr      ip, [r4, r6]
003a8bdc: sub      sp, sp, #0x6c
003a8be0: ldr      r3, [r0]
003a8be4: ldr      r1, [ip]
003a8be8: mov      r5, r0
003a8bec: mov      r7, r2
003a8bf0: str      r1, [sp, #0x64]
003a8bf4: mov      lr, pc
003a8bf8: ldr      pc, [r3, #0x34]
003a8bfc: subs     sl, r0, #0
003a8c00: beq      #0x3a8c20
003a8c04: ldr      r3, [r4, r6]
003a8c08: ldr      r2, [sp, #0x64]
003a8c0c: ldr      r3, [r3]
003a8c10: cmp      r2, r3
003a8c14: bne      #0x3a9218
003a8c18: add      sp, sp, #0x6c
003a8c1c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a8c20: ldr      r0, [pc, #0x5fc]
003a8c24: mov      r1, sl
003a8c28: mov      r2, #1
003a8c2c: ldr      r3, [r4, r0]
003a8c30: str      r0, [sp, #8]
003a8c34: add      r8, sp, #0x4c
003a8c38: ldr      r0, [r3, #0x40]
003a8c3c: movw     r3, #0x1088
003a8c40: ldr      r3, [r5, r3]
003a8c44: str      r3, [sp, #0x1c]
003a8c48: movw     r3, #0x1090
003a8c4c: ldr      r3, [r5, r3]
003a8c50: str      r3, [sp, #0x18]
003a8c54: bl       #0x36e478
003a8c58: ldr      r2, [pc, #0x5c8]
003a8c5c: str      r2, [sp, #0xc]
003a8c60: ldr      sb, [r4, r2]
003a8c64: ldr      r0, [r0, #0x660]
003a8c68: str      r0, [sp, #0x14]
003a8c6c: mov      r0, sb
003a8c70: bl       #0x337888
003a8c74: ldr      r1, [pc, #0x5b0]
003a8c78: add      r2, sp, #0x30
003a8c7c: mov      r0, r8
003a8c80: add      r1, pc, r1
003a8c84: bl       #0x3140ec
003a8c88: mov      r0, sb
003a8c8c: mov      r1, r8
003a8c90: bl       #0x337a88
003a8c94: cmp      r0, #0
003a8c98: bne      #0x3a9014
003a8c9c: mov      r0, r8
003a8ca0: bl       #0x318254
003a8ca4: bl       #0x7fd794
003a8ca8: ldrb     r3, [r0, #5]
003a8cac: cmp      r3, #0
003a8cb0: beq      #0x3a8f30
003a8cb4: ldr      r0, [sp, #8]
003a8cb8: ldr      r3, [r4, r0]
003a8cbc: ldr      r3, [r3, #0x40]
003a8cc0: cmp      r3, #0
003a8cc4: asreq    r2, fp, #8
003a8cc8: streq    r2, [sp, #0x10]
003a8ccc: rsbeq    r2, fp, #0
003a8cd0: beq      #0x3a8d00
003a8cd4: ldr      r3, [r3, #0x714]
003a8cd8: cmp      r3, #0
003a8cdc: beq      #0x3a8f54
003a8ce0: cmp      r3, #5
003a8ce4: asreq    r0, fp, #8
003a8ce8: streq    r0, [sp, #0x10]
003a8cec: rsbeq    r2, fp, #0
003a8cf0: beq      #0x3a8d00
003a8cf4: mov      r0, #0
003a8cf8: str      r0, [sp, #0x10]
003a8cfc: mov      r2, r0
003a8d00: add      sl, r5, #0x560
003a8d04: mov      r0, sl
003a8d08: mov      r1, #0x24
003a8d0c: ldr      r8, [pc, #0x51c]
003a8d10: bl       #0x3e0708
003a8d14: ldr      r2, [sp, #8]
003a8d18: add      r8, pc, r8
003a8d1c: mov      r1, r8
003a8d20: ldr      r0, [r4, r2]
003a8d24: bl       #0x320e14
003a8d28: cmp      r0, #0
003a8d2c: beq      #0x3a8fcc
003a8d30: ldr      r3, [r5]
003a8d34: mov      r0, r5
003a8d38: mov      lr, pc
003a8d3c: ldr      pc, [r3, #0x28]
003a8d40: cmp      r0, #0
003a8d44: bne      #0x3a9038
003a8d48: mov      r0, sl
003a8d4c: mov      r1, #0x24
003a8d50: mov      r2, #0
003a8d54: bl       #0x3e07a0
003a8d58: movw     r3, #0x1088
003a8d5c: ldr      r3, [r5, r3]
003a8d60: cmp      r3, #0
003a8d64: movgt    r0, #0
003a8d68: strgt    r0, [sp, #0xc]
003a8d6c: ble      #0x3a90e8
003a8d70: ldr      r3, [r5]
003a8d74: mov      r0, r5
003a8d78: mov      lr, pc
003a8d7c: ldr      pc, [r3, #0x28]
003a8d80: cmp      r0, #0
003a8d84: beq      #0x3a8f64
003a8d88: movw     r3, #0x1090
003a8d8c: ldr      r3, [r5, r3]
003a8d90: movw     r2, #0x1088
003a8d94: ldr      r2, [r5, r2]
003a8d98: add      r3, r3, r3, lsr #31
003a8d9c: cmp      r2, r3, asr #1
003a8da0: bgt      #0x3a8f64
003a8da4: bl       #0x7fd794
003a8da8: ldrb     r3, [r0, #5]
003a8dac: cmp      r3, #0
003a8db0: beq      #0x3a9060
003a8db4: movw     r3, #0x1448
003a8db8: ldrb     r2, [r5, r3]
003a8dbc: cmp      r2, #0
003a8dc0: beq      #0x3a8e54
003a8dc4: ldr      r0, [sp, #0x14]
003a8dc8: mov      r8, #0
003a8dcc: strb     r8, [r5, r3]
003a8dd0: cmp      r5, r0
003a8dd4: beq      #0x3a91a4
003a8dd8: ldr      r3, [pc, #0x454]
003a8ddc: ldr      r2, [r4, r3]
003a8de0: ldr      r3, [pc, #0x450]
003a8de4: ldr      sl, [r2]
003a8de8: ldr      r3, [r4, r3]
003a8dec: cmp      sl, #0
003a8df0: ldr      r3, [r3]
003a8df4: str      r3, [sp, #0x14]
003a8df8: beq      #0x3a919c
003a8dfc: ldr      r3, [pc, #0x438]
003a8e00: ldr      fp, [pc, #0x438]
003a8e04: ldr      r3, [r4, r3]
003a8e08: add      fp, pc, fp
003a8e0c: ldr      sb, [r3]
003a8e10: b        #0x3a8e20
003a8e14: add      r8, r8, #1
003a8e18: cmp      r8, sl
003a8e1c: beq      #0x3a919c
003a8e20: mov      r0, fp
003a8e24: ldr      r1, [sb, r8, lsl #2]
003a8e28: bl       #0x30e31c
003a8e2c: cmp      r0, #0
003a8e30: bne      #0x3a8e14
003a8e34: mov      r1, r8
003a8e38: mov      ip, #0
003a8e3c: mov      r2, ip
003a8e40: ldr      r0, [sp, #0x14]
003a8e44: mov      r3, ip
003a8e48: str      ip, [sp]
003a8e4c: str      ip, [sp, #4]
003a8e50: bl       #0x36b80c
003a8e54: add      r8, sp, #0x20
003a8e58: mov      r1, r7
003a8e5c: mov      r0, r8
003a8e60: bl       #0x33dd2c
003a8e64: mov      r0, r8
003a8e68: bl       #0x33ff54
003a8e6c: ldr      r3, [pc, #0x3d0]
003a8e70: subs     r8, r0, #0
003a8e74: ldr      r3, [r4, r3]
003a8e78: ldr      r7, [r3]
003a8e7c: beq      #0x3a8c04
003a8e80: ldr      r3, [r8]
003a8e84: mov      lr, pc
003a8e88: ldr      pc, [r3, #0x28]
003a8e8c: cmp      r0, #0
003a8e90: beq      #0x3a8c04
003a8e94: cmp      r5, r8
003a8e98: beq      #0x3a8c04
003a8e9c: ldr      r2, [sp, #8]
003a8ea0: mov      r1, r8
003a8ea4: ldr      r3, [r4, r2]
003a8ea8: ldr      r0, [r3, #0x40]
003a8eac: bl       #0x36effc
003a8eb0: cmp      r0, #0
003a8eb4: beq      #0x3a8c04
003a8eb8: ldr      r3, [sp, #0x10]
003a8ebc: cmp      r3, #0xc7
003a8ec0: bgt      #0x3a9138
003a8ec4: ldr      r0, [sp, #0x10]
003a8ec8: cmp      r0, #0x95
003a8ecc: bgt      #0x3a9150
003a8ed0: ldr      r2, [sp, #0x10]
003a8ed4: cmp      r2, #0x63
003a8ed8: bgt      #0x3a9168
003a8edc: ldr      r3, [sp, #0x10]
003a8ee0: cmp      r3, #0x31
003a8ee4: bgt      #0x3a9180
003a8ee8: ldr      r0, [sp, #0x1c]
003a8eec: ldr      r2, [sp, #0x18]
003a8ef0: cmp      r0, r2
003a8ef4: bne      #0x3a8c04
003a8ef8: ldr      r3, [sp, #0xc]
003a8efc: cmp      r3, #0
003a8f00: beq      #0x3a8c04
003a8f04: mov      r0, r5
003a8f08: bl       #0x3a3158
003a8f0c: cmp      r0, #0
003a8f10: beq      #0x3a9204
003a8f14: ldr      r0, [pc, #0x32c]
003a8f18: add      r0, pc, r0
003a8f1c: bl       #0x3a3f70
003a8f20: mov      r1, r0
003a8f24: mov      r0, r7
003a8f28: bl       #0x3813b8
003a8f2c: b        #0x3a8c04
003a8f30: ldr      r2, [sp, #0x14]
003a8f34: cmp      r2, #0
003a8f38: beq      #0x3a8cf4
003a8f3c: ldr      r3, [r2]
003a8f40: mov      r0, r2
003a8f44: mov      lr, pc
003a8f48: ldr      pc, [r3, #0x34]
003a8f4c: cmp      r0, #0
003a8f50: bne      #0x3a8cf4
003a8f54: asr      r3, fp, #8
003a8f58: str      r3, [sp, #0x10]
003a8f5c: rsb      r2, fp, #0
003a8f60: b        #0x3a8d00
003a8f64: ldr      r3, [r5]
003a8f68: mov      r0, r5
003a8f6c: mov      lr, pc
003a8f70: ldr      pc, [r3, #0x28]
003a8f74: cmp      r0, #0
003a8f78: beq      #0x3a8e54
003a8f7c: movw     r8, #0x1448
003a8f80: ldrb     r3, [r5, r8]
003a8f84: cmp      r3, #0
003a8f88: bne      #0x3a8e54
003a8f8c: movw     r3, #0x1088
003a8f90: ldr      r0, [r5, r3]
003a8f94: bl       #0x30e964
003a8f98: movw     r3, #0x1090
003a8f9c: mov      sl, r0
003a8fa0: ldr      r0, [r5, r3]
003a8fa4: bl       #0x30e964
003a8fa8: mov      r1, #0x3f400000
003a8fac: bl       #0x30ed6c
003a8fb0: mov      r1, r0
003a8fb4: mov      r0, sl
003a8fb8: bl       #0x30e4b4
003a8fbc: cmp      r0, #0
003a8fc0: movne    r3, #1
003a8fc4: strbne   r3, [r5, r8]
003a8fc8: b        #0x3a8e54
003a8fcc: ldr      r3, [sp, #0xc]
003a8fd0: add      sb, sp, #0x34
003a8fd4: ldr      fp, [r4, r3]
003a8fd8: mov      r0, fp
003a8fdc: bl       #0x337888
003a8fe0: mov      r1, r8
003a8fe4: add      r2, sp, #0x2c
003a8fe8: mov      r0, sb
003a8fec: bl       #0x3140ec
003a8ff0: mov      r1, sb
003a8ff4: mov      r0, fp
003a8ff8: bl       #0x337a88
003a8ffc: mov      r8, r0
003a9000: mov      r0, sb
003a9004: bl       #0x318254
003a9008: cmp      r8, #0
003a900c: beq      #0x3a8d58
003a9010: b        #0x3a8d30
003a9014: mov      r0, r5
003a9018: bl       #0x3a3064
003a901c: cmp      r0, #0
003a9020: beq      #0x3a8c9c
003a9024: mov      r0, r8
003a9028: str      sl, [sp, #0x10]
003a902c: bl       #0x318254
003a9030: ldr      r2, [sp, #0x10]
003a9034: b        #0x3a8d00
003a9038: ldr      r3, [r5, #0x418]
003a903c: cmp      r3, #0
003a9040: beq      #0x3a8d58
003a9044: mov      r0, r3
003a9048: ldr      r3, [r3]
003a904c: mov      lr, pc
003a9050: ldr      pc, [r3, #0x28]
003a9054: cmp      r0, #0
003a9058: bne      #0x3a8d58
003a905c: b        #0x3a8d48
003a9060: mov      r0, r5
003a9064: bl       #0x3bb8e4
003a9068: subs     r8, r0, #0
003a906c: bne      #0x3a8db4
003a9070: ldr      r0, [sp, #8]
003a9074: ldr      r3, [r4, r0]
003a9078: ldr      r3, [r3, #0x4c]
003a907c: ldrb     r3, [r3, #0x2d]
003a9080: cmp      r3, #0
003a9084: beq      #0x3a8db4
003a9088: ldr      r3, [pc, #0x1bc]
003a908c: ldr      r1, [pc, #0x1bc]
003a9090: mov      r2, #1
003a9094: ldr      sl, [r4, r3]
003a9098: add      r1, pc, r1
003a909c: mov      r0, sl
003a90a0: bl       #0x4591f0
003a90a4: cmn      r0, #1
003a90a8: mov      r1, r0
003a90ac: beq      #0x3a90c0
003a90b0: mov      r0, sl
003a90b4: mov      r3, r8
003a90b8: mvn      r2, #0
003a90bc: bl       #0x4605c0
003a90c0: ldr      r2, [sp, #8]
003a90c4: mov      r1, #0
003a90c8: ldr      r3, [r4, r2]
003a90cc: ldr      r2, [r3, #0x4c]
003a90d0: ldr      r3, [pc, #0x17c]
003a90d4: strb     r1, [r2, #0x2d]
003a90d8: ldr      r3, [r4, r3]
003a90dc: ldr      r0, [r3]
003a90e0: bl       #0x317e98
003a90e4: b        #0x3a8db4
003a90e8: mov      r0, sl
003a90ec: mov      r1, #0x24
003a90f0: mov      r2, #0
003a90f4: bl       #0x3e07a0
003a90f8: mov      r2, #0
003a90fc: ldr      r0, [r5, #0x378]
003a9100: mov      r1, r7
003a9104: bl       #0x40570c
003a9108: ldr      r3, [r5]
003a910c: mov      r0, r5
003a9110: mov      lr, pc
003a9114: ldr      pc, [r3, #0x54]
003a9118: cmp      r0, #0
003a911c: moveq    r3, #3
003a9120: streq    r3, [r5, #0x11c]
003a9124: movne    r2, #1
003a9128: moveq    r3, #1
003a912c: strne    r2, [sp, #0xc]
003a9130: streq    r3, [sp, #0xc]
003a9134: b        #0x3a8d70
003a9138: ldr      r0, [pc, #0x118]
003a913c: add      r0, pc, r0
003a9140: bl       #0x3a3f70
003a9144: mov      r1, r0
003a9148: mov      r0, r7
003a914c: bl       #0x3813b8
003a9150: ldr      r0, [pc, #0x104]
003a9154: add      r0, pc, r0
003a9158: bl       #0x3a3f70
003a915c: mov      r1, r0
003a9160: mov      r0, r7
003a9164: bl       #0x3813b8
003a9168: ldr      r0, [pc, #0xf0]
003a916c: add      r0, pc, r0
003a9170: bl       #0x3a3f70
003a9174: mov      r1, r0
003a9178: mov      r0, r7
003a917c: bl       #0x3813b8
003a9180: ldr      r0, [pc, #0xdc]
003a9184: add      r0, pc, r0
003a9188: bl       #0x3a3f70
003a918c: mov      r1, r0
003a9190: mov      r0, r7
003a9194: bl       #0x3813b8
003a9198: b        #0x3a8ee8
003a919c: mvn      r1, #0
003a91a0: b        #0x3a8e38
003a91a4: ldr      r3, [pc, #0x88]
003a91a8: ldr      r2, [r4, r3]
003a91ac: ldr      r3, [pc, #0x84]
003a91b0: ldr      sl, [r2]
003a91b4: ldr      r3, [r4, r3]
003a91b8: cmp      sl, r8
003a91bc: ldr      r3, [r3]
003a91c0: str      r3, [sp, #0x14]
003a91c4: beq      #0x3a919c
003a91c8: ldr      r3, [pc, #0x6c]
003a91cc: ldr      sb, [pc, #0x94]
003a91d0: ldr      r3, [r4, r3]
003a91d4: add      sb, pc, sb
003a91d8: ldr      fp, [r3]
003a91dc: b        #0x3a91ec
003a91e0: add      r8, r8, #1
003a91e4: cmp      r8, sl
003a91e8: beq      #0x3a919c
003a91ec: mov      r0, sb
003a91f0: ldr      r1, [fp, r8, lsl #2]
003a91f4: bl       #0x30e31c
003a91f8: cmp      r0, #0
003a91fc: bne      #0x3a91e0
003a9200: b        #0x3a8e34
003a9204: mov      r0, r5
003a9208: bl       #0x3a3144
003a920c: cmp      r0, #0
003a9210: beq      #0x3a8c04
003a9214: b        #0x3a8f14
003a9218: bl       #0x30e310
003a921c: ldrheq   fp, [lr], #-0xec
003a9220: andeq    r4, r0, ip, lsr #1
003a9224: strdeq   r3, r4, [r0], -r4
003a9228: andeq    r0, r0, r4, lsl #17
003a922c: subseq   sl, r1, r8, lsl #15
003a9230: subseq   sl, r1, r0, lsl #14
003a9234: andeq    r3, r0, r8, lsr sp
003a9238: andeq    r0, r0, r4, lsr #27
003a923c: andeq    r3, r0, r8, lsr #19
003a9240: subseq   sl, r1, r0, asr r6
003a9244: andeq    r1, r0, r0, ror sp

# _ZN12ObjectHandlecvP9CharacterEv
0033ff54: push     {r4, lr}
0033ff58: mov      r1, #0
0033ff5c: bl       #0x33fdc0
0033ff60: subs     r4, r0, #0
0033ff64: bne      #0x33ff70
0033ff68: mov      r0, #0
0033ff6c: pop      {r4, pc}
0033ff70: ldr      r3, [r4]
0033ff74: mov      lr, pc
0033ff78: ldr      pc, [r3, #0x24]
0033ff7c: cmp      r0, #0
0033ff80: beq      #0x33ff68
0033ff84: mov      r0, r4
0033ff88: pop      {r4, pc}

# _ZN14CharProperties9PROPS_SetEii
003e07a0: push     {r4, r5, r6, lr}
003e07a4: mov      r6, r2
003e07a8: mov      r4, r0
003e07ac: mov      r5, r1
003e07b0: bl       #0x3deed8
003e07b4: tst      r0, #0x20
003e07b8: bne      #0x3e07e4
003e07bc: tst      r0, #8
003e07c0: bne      #0x3e07c8
003e07c4: pop      {r4, r5, r6, pc}
003e07c8: add      r1, r4, #0xa90
003e07cc: mov      r0, r4
003e07d0: add      r1, r1, #4
003e07d4: mov      r2, r5
003e07d8: mov      r3, r6
003e07dc: pop      {r4, r5, r6, lr}
003e07e0: b        #0x3deca0
003e07e4: mov      r0, r4
003e07e8: add      r1, r4, #0x38c
003e07ec: mov      r3, r6
003e07f0: mov      r2, r5
003e07f4: bl       #0x3deca0
003e07f8: mov      r0, r4
003e07fc: mov      r1, r5
003e0800: pop      {r4, r5, r6, lr}
003e0804: b        #0x3dfe60

# _ZNK14CharProperties8_GetTypeEi
003deed8: ldr      r3, [pc, #0x28]
003deedc: mov      r2, r1
003deee0: ldr      r1, [pc, #0x24]
003deee4: push     {r4, lr}
003deee8: add      r3, pc, r3
003deeec: ldr      ip, [r3, r1]
003deef0: ldr      r1, [ip]
003deef4: add      r1, r1, #0x384
003deef8: bl       #0x3dedb4
003deefc: cmn      r0, #1
003def00: moveq    r0, #0x10
003def04: pop      {r4, pc}
003def08: subseq   r5, fp, r8, lsr #23
003def0c: andeq    r2, r0, r0, asr fp

# _ZN12v2Controller8Cmd_KillEP10GameObjectb
0040570c: push     {r4, lr}
00405710: ldr      r3, [r0, #4]
00405714: mov      r0, r3
00405718: ldr      r3, [r3]
0040571c: mov      lr, pc
00405720: ldr      pc, [r3, #0x58]
00405724: pop      {r4, pc}

# _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
0033fc88: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0033fc8c: ldr      r5, [pc, #0x124]
0033fc90: ldr      sb, [pc, #0x124]
0033fc94: ldr      r4, [r0, #4]
0033fc98: add      r5, pc, r5
0033fc9c: ldr      r3, [r5, sb]
0033fca0: sub      sp, sp, #0x48
0033fca4: cmp      r4, #0
0033fca8: ldr      r3, [r3]
0033fcac: mov      r8, r0
0033fcb0: str      r3, [sp, #0x44]
0033fcb4: beq      #0x33fda8
0033fcb8: ldr      r7, [r1]
0033fcbc: mov      r2, r0
0033fcc0: b        #0x33fccc
0033fcc4: mov      r2, r4
0033fcc8: mov      r4, r3
0033fccc: ldr      r3, [r4, #0x10]
0033fcd0: cmp      r3, r7
0033fcd4: ldrlt    r3, [r4, #0xc]
0033fcd8: ldrge    r3, [r4, #8]
0033fcdc: movlt    r4, r2
0033fce0: cmp      r3, #0
0033fce4: bne      #0x33fcc4
0033fce8: cmp      r8, r4
0033fcec: beq      #0x33fd20
0033fcf0: ldr      r3, [r4, #0x10]
0033fcf4: mov      r0, r4
0033fcf8: cmp      r3, r7
0033fcfc: bgt      #0x33fd20
0033fd00: ldr      r3, [r5, sb]
0033fd04: ldr      r2, [sp, #0x44]
0033fd08: add      r0, r0, #0x14
0033fd0c: ldr      r3, [r3]
0033fd10: cmp      r2, r3
0033fd14: bne      #0x33fdb4
0033fd18: add      sp, sp, #0x48
0033fd1c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0033fd20: add      r6, sp, #0x28
0033fd24: mov      r0, r6
0033fd28: mov      r1, #0x10
0033fd2c: str      r6, [sp, #0x38]
0033fd30: str      r6, [sp, #0x3c]
0033fd34: bl       #0x31167c
0033fd38: ldr      r2, [sp, #0x38]
0033fd3c: mov      r3, #0
0033fd40: add      sl, sp, #0x48
0033fd44: strb     r3, [r2]
0033fd48: str      r7, [sl, #-0x40]!
0033fd4c: add      r7, sl, #4
0033fd50: mov      r0, r7
0033fd54: ldr      r1, [sp, #0x3c]
0033fd58: ldr      r2, [sp, #0x38]
0033fd5c: str      r3, [sp, #0x40]
0033fd60: str      r7, [sp, #0x1c]
0033fd64: str      r7, [sp, #0x20]
0033fd68: bl       #0x3116e8
0033fd6c: ldr      ip, [sp, #0x40]
0033fd70: mov      r1, r8
0033fd74: mov      r3, sl
0033fd78: mov      r2, sp
0033fd7c: add      r0, sp, #4
0033fd80: str      ip, [sp, #0x24]
0033fd84: str      r4, [sp]
0033fd88: bl       #0x33f914
0033fd8c: ldr      r4, [sp, #4]
0033fd90: mov      r0, r7
0033fd94: bl       #0x3139ac
0033fd98: mov      r0, r6
0033fd9c: bl       #0x3139ac
0033fda0: mov      r0, r4
0033fda4: b        #0x33fd00
0033fda8: ldr      r7, [r1]
0033fdac: mov      r4, r0
0033fdb0: b        #0x33fce8
0033fdb4: bl       #0x30e310

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

# _ZN14CharProperties9PROPS_AddEii
003e0708: push     {r4, r5, r6, r7, r8, lr}
003e070c: mov      r7, r2
003e0710: mov      r4, r0
003e0714: mov      r5, r1
003e0718: bl       #0x3deed8
003e071c: tst      r0, #0x20
003e0720: bne      #0x3e0760
003e0724: tst      r0, #8
003e0728: bne      #0x3e0730
003e072c: pop      {r4, r5, r6, r7, r8, pc}
003e0730: add      r6, r4, #0xa90
003e0734: add      r6, r6, #4
003e0738: mov      r1, r6
003e073c: mov      r2, r5
003e0740: mov      r0, r4
003e0744: bl       #0x3dedb4
003e0748: mov      r1, r6
003e074c: add      r3, r0, r7
003e0750: mov      r2, r5
003e0754: mov      r0, r4
003e0758: pop      {r4, r5, r6, r7, r8, lr}
003e075c: b        #0x3deca0
003e0760: add      r6, r4, #0x38c
003e0764: mov      r1, r6
003e0768: mov      r2, r5
003e076c: mov      r0, r4
003e0770: bl       #0x3dedb4
003e0774: mov      r1, r6
003e0778: add      r3, r0, r7
003e077c: mov      r2, r5
003e0780: mov      r0, r4
003e0784: bl       #0x3deca0
003e0788: mov      r0, r4
003e078c: mov      r1, r5
003e0790: pop      {r4, r5, r6, r7, r8, lr}
003e0794: b        #0x3dfe60

# _ZN12ObjectHandle9GetObjectEb
0033fdc0: push     {r4, r5, r6, r7, r8, lr}
0033fdc4: ldr      r4, [r0]
0033fdc8: ldr      r5, [pc, #0xc0]
0033fdcc: sub      sp, sp, #8
0033fdd0: cmp      r4, #0
0033fdd4: mov      r6, r0
0033fdd8: mov      r7, r1
0033fddc: add      r5, pc, r5
0033fde0: beq      #0x33fe20
0033fde4: ldr      r3, [pc, #0xa8]
0033fde8: ldr      r4, [r0, #4]
0033fdec: ldr      r3, [r5, r3]
0033fdf0: cmp      r4, #0
0033fdf4: ldr      r0, [r3, #0x38]
0033fdf8: ldr      r8, [r0, #0x78]
0033fdfc: beq      #0x33fe0c
0033fe00: ldr      r3, [r6, #8]
0033fe04: cmp      r3, r8
0033fe08: beq      #0x33fe20
0033fe0c: add      r0, r0, #0xc
0033fe10: mov      r1, r6
0033fe14: bl       #0x33fc88
0033fe18: ldr      r4, [r0, #0x18]
0033fe1c: stmib    r6, {r4, r8}
0033fe20: cmp      r7, #0
0033fe24: beq      #0x33fe30
0033fe28: cmp      r4, #0
0033fe2c: beq      #0x33fe3c
0033fe30: mov      r0, r4
0033fe34: add      sp, sp, #8
0033fe38: pop      {r4, r5, r6, r7, r8, pc}
0033fe3c: ldr      r3, [pc, #0x54]
0033fe40: ldr      r3, [r5, r3]
0033fe44: ldr      r3, [r3]
0033fe48: cmp      r3, #2
0033fe4c: streq    r4, [r4]
0033fe50: beq      #0x33fe30
0033fe54: cmp      r3, #1
0033fe58: bne      #0x33fe30
0033fe5c: ldr      r0, [pc, #0x38]
0033fe60: ldr      r1, [pc, #0x38]
0033fe64: ldr      r2, [pc, #0x38]
0033fe68: ldr      r0, [r5, r0]
0033fe6c: ldr      r3, [pc, #0x34]
0033fe70: mov      ip, #0x31
0033fe74: add      r1, pc, r1
0033fe78: add      r2, pc, r2
0033fe7c: add      r3, pc, r3
0033fe80: add      r0, r0, #0xa8
0033fe84: str      ip, [sp]
0033fe88: bl       #0x30e004
0033fe8c: b        #0x33fe30
0033fe90: strhteq  r4, [r5], #-0xc4
0033fe94: strdeq   r3, r4, [r0], -r4
0033fe98: andeq    r3, r0, r0, asr #19
0033fe9c: andeq    r1, r0, r0, asr #19
0033fea0: subseq   lr, r7, r4, ror #10

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

# _ZN14CharProperties14RecalcPropertyEi
003dfe60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dfe64: sub      sp, sp, #0x54
003dfe68: mov      r6, r0
003dfe6c: mov      r7, r1
003dfe70: bl       #0x3deed8
003dfe74: tst      r0, #4
003dfe78: bne      #0x3e0110
003dfe7c: tst      r0, #2
003dfe80: bne      #0x3e02bc
003dfe84: tst      r0, #1
003dfe88: beq      #0x3e0080
003dfe8c: add      r2, r6, #0xa90
003dfe90: add      r2, r2, #4
003dfe94: str      r2, [sp, #4]
003dfe98: ldr      r3, [r6, #0xe20]
003dfe9c: add      sb, r6, #0xe10
003dfea0: add      sb, sb, #8
003dfea4: str      r3, [sp, #0xc]
003dfea8: ldr      r2, [sp, #0xc]
003dfeac: add      ip, sp, #0x20
003dfeb0: str      ip, [sp, #8]
003dfeb4: cmp      r2, sb
003dfeb8: add      r4, sp, #0x10
003dfebc: mov      r8, r6
003dfec0: beq      #0x3e0018
003dfec4: ldrb     r3, [sb]
003dfec8: cmp      r3, #0
003dfecc: bne      #0x3dfee4
003dfed0: ldr      r3, [sb, #4]
003dfed4: ldr      r3, [r3, #4]
003dfed8: cmp      r3, sb
003dfedc: ldreq    ip, [sb, #0xc]
003dfee0: beq      #0x3dff04
003dfee4: ldr      ip, [sb, #8]
003dfee8: cmp      ip, #0
003dfeec: bne      #0x3dfef8
003dfef0: b        #0x3e00b4
003dfef4: mov      ip, r3
003dfef8: ldr      r3, [ip, #0xc]
003dfefc: cmp      r3, #0
003dff00: bne      #0x3dfef4
003dff04: ldr      lr, [sp, #8]
003dff08: add      r5, ip, #0x34
003dff0c: ldm      r5, {r0, r1, r2, r3}
003dff10: stm      lr, {r0, r1, r2, r3}
003dff14: add      r0, ip, #0x44
003dff18: ldr      r1, [sp, #8]
003dff1c: bl       #0x3de870
003dff20: subs     sl, r0, #0
003dff24: beq      #0x3dffc8
003dff28: mov      fp, #0
003dff2c: mov      r6, fp
003dff30: ldm      r5, {r0, r1, r2, r3}
003dff34: stm      r4, {r0, r1, r2, r3}
003dff38: mov      r1, r6
003dff3c: mov      r0, r4
003dff40: bl       #0x3de8b4
003dff44: ldm      r5, {r0, r1, r2, r3}
003dff48: stm      r4, {r0, r1, r2, r3}
003dff4c: mov      r1, r6
003dff50: mov      r0, r4
003dff54: bl       #0x3de8b4
003dff58: ldr      r3, [sp, #0x10]
003dff5c: mov      r2, r7
003dff60: mov      r0, r8
003dff64: ldr      r1, [r3]
003dff68: bl       #0x3df114
003dff6c: cmp      r0, #0
003dff70: beq      #0x3dffb4
003dff74: ldm      r5, {r0, r1, r2, r3}
003dff78: stm      r4, {r0, r1, r2, r3}
003dff7c: mov      r1, r6
003dff80: mov      r0, r4
003dff84: bl       #0x3de8b4
003dff88: ldr      r3, [sp, #0x10]
003dff8c: mov      r2, r7
003dff90: mov      r0, r8
003dff94: ldr      r1, [r3]
003dff98: bl       #0x3dedb4
003dff9c: ldr      r1, [sp, #4]
003dffa0: mov      r3, r0
003dffa4: mov      r2, r7
003dffa8: mov      r0, r8
003dffac: bl       #0x3deca0
003dffb0: mov      fp, #1
003dffb4: add      r6, r6, #1
003dffb8: cmp      r6, sl
003dffbc: bne      #0x3dff30
003dffc0: cmp      fp, #0
003dffc4: bne      #0x3e0474
003dffc8: ldrb     r3, [sb]
003dffcc: cmp      r3, #0
003dffd0: bne      #0x3dffe8
003dffd4: ldr      r3, [sb, #4]
003dffd8: ldr      r3, [r3, #4]
003dffdc: cmp      r3, sb
003dffe0: ldreq    r3, [sb, #0xc]
003dffe4: beq      #0x3e0008
003dffe8: ldr      r3, [sb, #8]
003dffec: cmp      r3, #0
003dfff0: bne      #0x3dfffc
003dfff4: b        #0x3e00e4
003dfff8: mov      r3, r2
003dfffc: ldr      r2, [r3, #0xc]
003e0000: cmp      r2, #0
003e0004: bne      #0x3dfff8
003e0008: mov      sb, r3
003e000c: ldr      r2, [sp, #0xc]
003e0010: cmp      r2, sb
003e0014: bne      #0x3dfec4
003e0018: add      r4, r8, #0x710
003e001c: mov      r0, r8
003e0020: mov      r1, r4
003e0024: mov      r2, r7
003e0028: bl       #0x3df114
003e002c: cmp      r0, #0
003e0030: mov      r6, r8
003e0034: bne      #0x3e047c
003e0038: add      r4, r6, #0x38c
003e003c: mov      r0, r6
003e0040: mov      r1, r4
003e0044: mov      r2, r7
003e0048: bl       #0x3df114
003e004c: cmp      r0, #0
003e0050: bne      #0x3e047c
003e0054: add      r4, r6, #8
003e0058: mov      r0, r6
003e005c: mov      r1, r4
003e0060: mov      r2, r7
003e0064: bl       #0x3df114
003e0068: cmp      r0, #0
003e006c: bne      #0x3e047c
003e0070: mov      r0, r6
003e0074: mov      r1, r7
003e0078: bl       #0x3def10
003e007c: b        #0x3e048c
003e0080: tst      r0, #0x20
003e0084: bne      #0x3e0550
003e0088: tst      r0, #0x10
003e008c: addeq    ip, r6, #0xa90
003e0090: addeq    ip, ip, #4
003e0094: streq    ip, [sp, #4]
003e0098: bne      #0x3e0438
003e009c: mov      r0, r6
003e00a0: ldr      r1, [sp, #4]
003e00a4: mov      r2, r7
003e00a8: bl       #0x3dedb4
003e00ac: add      sp, sp, #0x54
003e00b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e00b4: ldr      ip, [sb, #4]
003e00b8: ldr      r3, [ip, #8]
003e00bc: cmp      r3, sb
003e00c0: beq      #0x3e00cc
003e00c4: b        #0x3dff04
003e00c8: mov      ip, r3
003e00cc: ldr      r3, [ip, #4]
003e00d0: ldr      r2, [r3, #8]
003e00d4: cmp      r2, ip
003e00d8: beq      #0x3e00c8
003e00dc: mov      ip, r3
003e00e0: b        #0x3dff04
003e00e4: ldr      r3, [sb, #4]
003e00e8: ldr      r2, [r3, #8]
003e00ec: cmp      sb, r2
003e00f0: bne      #0x3e0008
003e00f4: mov      r2, r3
003e00f8: ldr      r3, [r3, #4]
003e00fc: ldr      r1, [r3, #8]
003e0100: cmp      r1, r2
003e0104: beq      #0x3e00f4
003e0108: mov      sb, r3
003e010c: b        #0x3e000c
003e0110: add      r2, r6, #0xa90
003e0114: add      r2, r2, #4
003e0118: mov      r1, r7
003e011c: mov      r0, r6
003e0120: str      r2, [sp, #4]
003e0124: bl       #0x3def10
003e0128: add      r4, r6, #8
003e012c: mov      r3, r0
003e0130: ldr      r1, [sp, #4]
003e0134: mov      r0, r6
003e0138: mov      r2, r7
003e013c: bl       #0x3deca0
003e0140: mov      r0, r6
003e0144: mov      r1, r4
003e0148: mov      r2, r7
003e014c: bl       #0x3df114
003e0150: cmp      r0, #0
003e0154: bne      #0x3e0528
003e0158: add      r4, r6, #0x38c
003e015c: mov      r0, r6
003e0160: mov      r1, r4
003e0164: mov      r2, r7
003e0168: bl       #0x3df114
003e016c: cmp      r0, #0
003e0170: bne      #0x3e0500
003e0174: add      r4, r6, #0x710
003e0178: mov      r0, r6
003e017c: mov      r1, r4
003e0180: mov      r2, r7
003e0184: bl       #0x3df114
003e0188: cmp      r0, #0
003e018c: bne      #0x3e04d8
003e0190: add      r3, r6, #0xe10
003e0194: add      r3, r3, #8
003e0198: str      r3, [sp, #8]
003e019c: ldr      sb, [r6, #0xe20]
003e01a0: add      fp, sp, #0x40
003e01a4: add      r4, sp, #0x10
003e01a8: ldr      ip, [sp, #8]
003e01ac: cmp      sb, ip
003e01b0: beq      #0x3e009c
003e01b4: add      r8, sb, #0x34
003e01b8: ldm      r8, {r0, r1, r2, r3}
003e01bc: stm      fp, {r0, r1, r2, r3}
003e01c0: add      r0, sb, #0x44
003e01c4: mov      r1, fp
003e01c8: bl       #0x3de870
003e01cc: subs     sl, r0, #0
003e01d0: beq      #0x3e0260
003e01d4: mov      r5, #0
003e01d8: b        #0x3e01e8
003e01dc: add      r5, r5, #1
003e01e0: cmp      r5, sl
003e01e4: beq      #0x3e0260
003e01e8: ldm      r8, {r0, r1, r2, r3}
003e01ec: stm      r4, {r0, r1, r2, r3}
003e01f0: mov      r1, r5
003e01f4: mov      r0, r4
003e01f8: bl       #0x3de8b4
003e01fc: ldr      r3, [sp, #0x10]
003e0200: mov      r2, r7
003e0204: mov      r0, r6
003e0208: ldr      r1, [r3]
003e020c: bl       #0x3df114
003e0210: cmp      r0, #0
003e0214: beq      #0x3e01dc
003e0218: ldm      r8, {r0, r1, r2, r3}
003e021c: stm      r4, {r0, r1, r2, r3}
003e0220: mov      r1, r5
003e0224: mov      r0, r4
003e0228: bl       #0x3de8b4
003e022c: ldr      r3, [sp, #0x10]
003e0230: mov      r2, r7
003e0234: mov      r0, r6
003e0238: ldr      r1, [r3]
003e023c: bl       #0x3dedb4
003e0240: add      r5, r5, #1
003e0244: mov      r3, r0
003e0248: ldr      r1, [sp, #4]
003e024c: mov      r0, r6
003e0250: mov      r2, r7
003e0254: bl       #0x3df140
003e0258: cmp      r5, sl
003e025c: bne      #0x3e01e8
003e0260: ldr      r2, [sb, #0xc]
003e0264: cmp      r2, #0
003e0268: bne      #0x3e0274
003e026c: b        #0x3e0288
003e0270: mov      r2, r3
003e0274: ldr      r3, [r2, #8]
003e0278: cmp      r3, #0
003e027c: bne      #0x3e0270
003e0280: mov      sb, r2
003e0284: b        #0x3e01a8
003e0288: ldr      r3, [sb, #4]
003e028c: ldr      r1, [r3, #0xc]
003e0290: cmp      sb, r1
003e0294: bne      #0x3e02b0
003e0298: mov      sb, r3
003e029c: ldr      r3, [r3, #4]
003e02a0: ldr      r2, [r3, #0xc]
003e02a4: cmp      sb, r2
003e02a8: beq      #0x3e0298
003e02ac: ldr      r2, [sb, #0xc]
003e02b0: cmp      r3, r2
003e02b4: movne    sb, r3
003e02b8: b        #0x3e01a8
003e02bc: add      r4, r6, #8
003e02c0: mov      r1, r4
003e02c4: mov      r0, r6
003e02c8: mov      r2, r7
003e02cc: bl       #0x3df114
003e02d0: cmp      r0, #0
003e02d4: movne    r1, r4
003e02d8: bne      #0x3e043c
003e02dc: add      r4, r6, #0x38c
003e02e0: mov      r0, r6
003e02e4: mov      r1, r4
003e02e8: mov      r2, r7
003e02ec: bl       #0x3df114
003e02f0: cmp      r0, #0
003e02f4: bne      #0x3e05a8
003e02f8: add      r4, r6, #0x710
003e02fc: mov      r0, r6
003e0300: mov      r1, r4
003e0304: mov      r2, r7
003e0308: bl       #0x3df114
003e030c: cmp      r0, #0
003e0310: bne      #0x3e05f8
003e0314: add      lr, r6, #0xe10
003e0318: add      r2, r6, #0xa90
003e031c: add      lr, lr, #8
003e0320: add      r2, r2, #4
003e0324: str      lr, [sp, #0xc]
003e0328: str      r2, [sp, #4]
003e032c: add      r3, sp, #0x30
003e0330: ldr      sl, [r6, #0xe20]
003e0334: add      r4, sp, #0x10
003e0338: str      r3, [sp, #8]
003e033c: mov      r8, r6
003e0340: ldr      lr, [sp, #0xc]
003e0344: cmp      lr, sl
003e0348: beq      #0x3e05e4
003e034c: ldr      ip, [sp, #8]
003e0350: add      r5, sl, #0x34
003e0354: ldm      r5, {r0, r1, r2, r3}
003e0358: stm      ip, {r0, r1, r2, r3}
003e035c: add      r0, sl, #0x44
003e0360: ldr      r1, [sp, #8]
003e0364: bl       #0x3de870
003e0368: subs     sb, r0, #0
003e036c: beq      #0x3e0410
003e0370: mov      r6, #0
003e0374: mov      fp, r6
003e0378: ldm      r5, {r0, r1, r2, r3}
003e037c: stm      r4, {r0, r1, r2, r3}
003e0380: mov      r1, r6
003e0384: mov      r0, r4
003e0388: bl       #0x3de8b4
003e038c: ldm      r5, {r0, r1, r2, r3}
003e0390: stm      r4, {r0, r1, r2, r3}
003e0394: mov      r1, r6
003e0398: mov      r0, r4
003e039c: bl       #0x3de8b4
003e03a0: ldr      r3, [sp, #0x10]
003e03a4: mov      r2, r7
003e03a8: mov      r0, r8
003e03ac: ldr      r1, [r3]
003e03b0: bl       #0x3df114
003e03b4: cmp      r0, #0
003e03b8: beq      #0x3e03fc
003e03bc: ldm      r5, {r0, r1, r2, r3}
003e03c0: stm      r4, {r0, r1, r2, r3}
003e03c4: mov      r1, r6
003e03c8: mov      r0, r4
003e03cc: bl       #0x3de8b4
003e03d0: ldr      r3, [sp, #0x10]
003e03d4: mov      r2, r7
003e03d8: mov      r0, r8
003e03dc: ldr      r1, [r3]
003e03e0: bl       #0x3dedb4
003e03e4: ldr      r1, [sp, #4]
003e03e8: mov      r3, r0
003e03ec: mov      r2, r7
003e03f0: mov      r0, r8
003e03f4: bl       #0x3deca0
003e03f8: mov      fp, #1
003e03fc: add      r6, r6, #1
003e0400: cmp      r6, sb
003e0404: bne      #0x3e0378
003e0408: cmp      fp, #0
003e040c: bne      #0x3e0474
003e0410: ldr      r2, [sl, #0xc]
003e0414: cmp      r2, #0
003e0418: beq      #0x3e04a4
003e041c: mov      sl, r2
003e0420: b        #0x3e0428
003e0424: mov      sl, r3
003e0428: ldr      r3, [sl, #8]
003e042c: cmp      r3, #0
003e0430: bne      #0x3e0424
003e0434: b        #0x3e0340
003e0438: add      r1, r6, #8
003e043c: mov      r2, r7
003e0440: add      lr, r6, #0xa90
003e0444: mov      r0, r6
003e0448: str      lr, [sp, #4]
003e044c: bl       #0x3dedb4
003e0450: ldr      r2, [sp, #4]
003e0454: mov      r3, r0
003e0458: add      r2, r2, #4
003e045c: str      r2, [sp, #4]
003e0460: mov      r1, r2
003e0464: mov      r0, r6
003e0468: mov      r2, r7
003e046c: bl       #0x3deca0
003e0470: b        #0x3e009c
003e0474: mov      r6, r8
003e0478: b        #0x3e009c
003e047c: mov      r1, r4
003e0480: mov      r0, r6
003e0484: mov      r2, r7
003e0488: bl       #0x3dedb4
003e048c: mov      r3, r0
003e0490: ldr      r1, [sp, #4]
003e0494: mov      r0, r6
003e0498: mov      r2, r7
003e049c: bl       #0x3deca0
003e04a0: b        #0x3e009c
003e04a4: ldr      r3, [sl, #4]
003e04a8: ldr      r1, [r3, #0xc]
003e04ac: cmp      sl, r1
003e04b0: bne      #0x3e04cc
003e04b4: mov      sl, r3
003e04b8: ldr      r3, [r3, #4]
003e04bc: ldr      r2, [r3, #0xc]
003e04c0: cmp      r2, sl
003e04c4: beq      #0x3e04b4
003e04c8: ldr      r2, [sl, #0xc]
003e04cc: cmp      r3, r2
003e04d0: movne    sl, r3
003e04d4: b        #0x3e0340
003e04d8: mov      r1, r4
003e04dc: mov      r2, r7
003e04e0: mov      r0, r6
003e04e4: bl       #0x3dedb4
003e04e8: ldr      r1, [sp, #4]
003e04ec: mov      r3, r0
003e04f0: mov      r2, r7
003e04f4: mov      r0, r6
003e04f8: bl       #0x3df140
003e04fc: b        #0x3e0190
003e0500: mov      r1, r4
003e0504: mov      r2, r7
003e0508: mov      r0, r6
003e050c: bl       #0x3dedb4
003e0510: ldr      r1, [sp, #4]
003e0514: mov      r3, r0
003e0518: mov      r2, r7
003e051c: mov      r0, r6
003e0520: bl       #0x3df140
003e0524: b        #0x3e0174
003e0528: mov      r1, r4
003e052c: mov      r2, r7
003e0530: mov      r0, r6
003e0534: bl       #0x3dedb4
003e0538: ldr      r1, [sp, #4]
003e053c: mov      r3, r0
003e0540: mov      r2, r7
003e0544: mov      r0, r6
003e0548: bl       #0x3df140
003e054c: b        #0x3e0158
003e0550: add      r3, r6, #0xa90
003e0554: add      r3, r3, #4
003e0558: add      r1, r6, #8
003e055c: mov      r2, r7
003e0560: mov      r0, r6
003e0564: str      r3, [sp, #4]
003e0568: bl       #0x3dedb4
003e056c: ldr      r1, [sp, #4]
003e0570: mov      r3, r0
003e0574: mov      r2, r7
003e0578: mov      r0, r6
003e057c: bl       #0x3deca0
003e0580: add      r1, r6, #0x38c
003e0584: mov      r2, r7
003e0588: mov      r0, r6
003e058c: bl       #0x3dedb4
003e0590: ldr      r1, [sp, #4]
003e0594: mov      r3, r0
003e0598: mov      r2, r7
003e059c: mov      r0, r6
003e05a0: bl       #0x3df140
003e05a4: b        #0x3e009c
003e05a8: add      r3, r6, #0xa90
003e05ac: mov      r1, r4
003e05b0: mov      r2, r7
003e05b4: mov      r0, r6
003e05b8: str      r3, [sp, #4]
003e05bc: bl       #0x3dedb4
003e05c0: ldr      ip, [sp, #4]
003e05c4: mov      r3, r0
003e05c8: mov      r2, r7
003e05cc: add      ip, ip, #4
003e05d0: mov      r0, r6
003e05d4: mov      r1, ip
003e05d8: str      ip, [sp, #4]
003e05dc: bl       #0x3deca0
003e05e0: b        #0x3e009c
003e05e4: mov      r0, r8
003e05e8: mov      r1, r7
003e05ec: mov      r6, r8
003e05f0: bl       #0x3def10
003e05f4: b        #0x3e048c
003e05f8: mov      r2, r7
003e05fc: mov      r1, r4
003e0600: mov      r0, r6
003e0604: bl       #0x3dedb4
003e0608: add      r2, r6, #0xa90
003e060c: mov      r3, r0
003e0610: b        #0x3e0458
