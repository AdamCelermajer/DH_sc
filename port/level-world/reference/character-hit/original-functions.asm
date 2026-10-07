
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
