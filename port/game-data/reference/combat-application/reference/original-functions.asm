
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

# _ZN9Character13F_ApplyResultERKNS_12AttackResultEPS_S3_b
003b10b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b10b8: ldr      r7, [pc, #0xcb4]
003b10bc: ldr      sb, [pc, #0xcb4]
003b10c0: mov      r4, r0
003b10c4: add      r7, pc, r7
003b10c8: ldr      r0, [r7, sb]
003b10cc: mov      r5, r2
003b10d0: sub      sp, sp, #0x15c
003b10d4: ldr      r2, [r0]
003b10d8: mov      r8, r3
003b10dc: mov      r6, r1
003b10e0: str      r2, [sp, #0x154]
003b10e4: bl       #0x7fd794
003b10e8: ldrb     r3, [r0, #5]
003b10ec: cmp      r3, #0
003b10f0: bne      #0x3b1440
003b10f4: ldrb     r3, [r4, #0x18]
003b10f8: ldr      fp, [pc, #0xc7c]
003b10fc: add      r8, sp, #0x13c
003b1100: tst      r3, #3
003b1104: movw     r3, #0x14d0
003b1108: ldrheq   r2, [r6, r3]
003b110c: ldr      sl, [r7, fp]
003b1110: movne    r2, #0
003b1114: addeq    r2, r2, #1
003b1118: strh     r2, [r6, r3]
003b111c: mov      r0, sl
003b1120: bl       #0x337888
003b1124: ldr      r1, [pc, #0xc54]
003b1128: add      r2, sp, #0x48
003b112c: mov      r0, r8
003b1130: add      r1, pc, r1
003b1134: bl       #0x3140ec
003b1138: mov      r0, sl
003b113c: mov      r1, r8
003b1140: bl       #0x337a88
003b1144: cmp      r0, #0
003b1148: beq      #0x3b14cc
003b114c: mov      r0, r8
003b1150: bl       #0x3139ac
003b1154: ldr      r1, [r4, #0x10]
003b1158: mov      r0, r6
003b115c: bl       #0x3bdca4
003b1160: mov      r0, r6
003b1164: ldr      r1, [r4, #0x14]
003b1168: bl       #0x3bdbb8
003b116c: ldr      r3, [r5]
003b1170: mov      r0, r5
003b1174: mov      lr, pc
003b1178: ldr      pc, [r3, #0x34]
003b117c: cmp      r0, #0
003b1180: beq      #0x3b1204
003b1184: mov      r0, r5
003b1188: bl       #0x3bc6b8
003b118c: mov      r0, r4
003b1190: mov      r1, r6
003b1194: mov      r2, r5
003b1198: bl       #0x3af77c
003b119c: mov      r0, r4
003b11a0: mov      r1, r6
003b11a4: mov      r2, r5
003b11a8: bl       #0x3afee0
003b11ac: ldr      r3, [r4, #0x1c]
003b11b0: tst      r3, #0x20000000
003b11b4: beq      #0x3b1788
003b11b8: ldr      r3, [r5]
003b11bc: mov      r0, r5
003b11c0: mov      lr, pc
003b11c4: ldr      pc, [r3, #0x28]
003b11c8: cmp      r0, #0
003b11cc: bne      #0x3b1758
003b11d0: ldr      r3, [r6]
003b11d4: mov      r0, r6
003b11d8: mov      lr, pc
003b11dc: ldr      pc, [r3, #0x28]
003b11e0: cmp      r0, #0
003b11e4: bne      #0x3b13e0
003b11e8: ldr      r3, [r7, sb]
003b11ec: ldr      r2, [sp, #0x154]
003b11f0: ldr      r3, [r3]
003b11f4: cmp      r2, r3
003b11f8: bne      #0x3b1d70
003b11fc: add      sp, sp, #0x15c
003b1200: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b1204: ldr      r1, [r4, #8]
003b1208: cmp      r1, #0
003b120c: ble      #0x3b122c
003b1210: ldr      r2, [r4, #0xc]
003b1214: cmp      r2, #0
003b1218: ble      #0x3b122c
003b121c: asr      r1, r1, #8
003b1220: add      r0, r5, #0x560
003b1224: ldr      r3, [r4, #4]
003b1228: bl       #0x3e2720
003b122c: ldr      r2, [r4, #0x1c]
003b1230: ldr      r3, [r5]
003b1234: mov      r0, r5
003b1238: tst      r2, #0x18000000
003b123c: moveq    ip, #0
003b1240: movne    ip, #1
003b1244: str      ip, [sp, #0xc]
003b1248: mov      lr, pc
003b124c: ldr      pc, [r3, #0x28]
003b1250: cmp      r0, #0
003b1254: bne      #0x3b17ec
003b1258: ldrb     r3, [r4, #0x18]
003b125c: tst      r3, #2
003b1260: bne      #0x3b1834
003b1264: tst      r3, #4
003b1268: bne      #0x3b18a4
003b126c: tst      r3, #0x10
003b1270: bne      #0x3b1914
003b1274: tst      r3, #0x80
003b1278: bne      #0x3b196c
003b127c: tst      r3, #0x40
003b1280: beq      #0x3b1304
003b1284: ldr      r3, [r4, #0x1c]
003b1288: add      r1, r6, #0xff0
003b128c: add      r1, r1, #4
003b1290: tst      r3, #0x1000
003b1294: movne    r2, #0xb9
003b1298: moveq    r2, #0x8c
003b129c: add      r0, r6, #0x560
003b12a0: bl       #0x3dedb4
003b12a4: mov      r1, r0
003b12a8: add      r0, r5, #0x4f0
003b12ac: mov      r3, r6
003b12b0: mov      ip, #0
003b12b4: asr      r1, r1, #8
003b12b8: mov      r2, #1
003b12bc: add      r0, r0, #0xc
003b12c0: str      ip, [sp]
003b12c4: bl       #0x3c5ffc
003b12c8: ldr      sl, [r7, fp]
003b12cc: add      r8, sp, #0x7c
003b12d0: mov      r0, sl
003b12d4: bl       #0x337888
003b12d8: ldr      r1, [pc, #0xaa4]
003b12dc: add      r2, sp, #0x28
003b12e0: mov      r0, r8
003b12e4: add      r1, pc, r1
003b12e8: bl       #0x3140ec
003b12ec: mov      r1, r8
003b12f0: mov      r0, sl
003b12f4: bl       #0x337a88
003b12f8: mov      r0, r8
003b12fc: bl       #0x3139ac
003b1300: ldrb     r3, [r4, #0x18]
003b1304: tst      r3, #0x20
003b1308: beq      #0x3b136c
003b130c: ldr      r3, [r4, #0x1c]
003b1310: add      r1, r6, #0xff0
003b1314: add      r1, r1, #4
003b1318: tst      r3, #0x4000
003b131c: movne    r2, #0xbb
003b1320: moveq    r2, #0x8f
003b1324: add      r0, r6, #0x560
003b1328: bl       #0x3dedb4
003b132c: asrs     r1, r0, #8
003b1330: bne      #0x3b19e0
003b1334: ldr      sl, [r7, fp]
003b1338: add      r8, sp, #0x64
003b133c: mov      r0, sl
003b1340: bl       #0x337888
003b1344: ldr      r1, [pc, #0xa3c]
003b1348: add      r2, sp, #0x24
003b134c: mov      r0, r8
003b1350: add      r1, pc, r1
003b1354: bl       #0x3140ec
003b1358: mov      r0, sl
003b135c: mov      r1, r8
003b1360: bl       #0x337a88
003b1364: mov      r0, r8
003b1368: bl       #0x3139ac
003b136c: ldrb     r3, [r4, #0x19]
003b1370: tst      r3, #1
003b1374: beq      #0x3b1184
003b1378: ldr      r3, [r4, #0x1c]
003b137c: add      r1, r6, #0xff0
003b1380: add      r1, r1, #4
003b1384: tst      r3, #0x10000
003b1388: movne    r2, #0xbd
003b138c: moveq    r2, #0x92
003b1390: add      r0, r6, #0x560
003b1394: bl       #0x3dedb4
003b1398: asr      r1, r0, #8
003b139c: add      r0, r5, #0x560
003b13a0: bl       #0x3e2a5c
003b13a4: ldr      sl, [r7, fp]
003b13a8: add      r8, sp, #0x4c
003b13ac: mov      r0, sl
003b13b0: bl       #0x337888
003b13b4: ldr      r1, [pc, #0x9d0]
003b13b8: add      r2, sp, #0x20
003b13bc: mov      r0, r8
003b13c0: add      r1, pc, r1
003b13c4: bl       #0x3140ec
003b13c8: mov      r0, sl
003b13cc: mov      r1, r8
003b13d0: bl       #0x337a88
003b13d4: mov      r0, r8
003b13d8: bl       #0x3139ac
003b13dc: b        #0x3b1184
003b13e0: ldr      r3, [pc, #0x9a8]
003b13e4: mov      r1, r6
003b13e8: mov      r2, #0
003b13ec: ldr      r3, [r7, r3]
003b13f0: ldr      r0, [r3, #0x40]
003b13f4: bl       #0x36eea8
003b13f8: ldrh     r3, [r4, #0x18]
003b13fc: ldr      r6, [r0, #0x670]
003b1400: tst      r3, #0x160
003b1404: bne      #0x3b1a4c
003b1408: ldr      r8, [pc, #0x984]
003b140c: mov      r0, r5
003b1410: ldr      r3, [r5]
003b1414: mov      lr, pc
003b1418: ldr      pc, [r3, #0x34]
003b141c: cmp      r0, #0
003b1420: bne      #0x3b1a38
003b1424: ldr      r2, [r4]
003b1428: ldr      r0, [r7, r8]
003b142c: mov      r3, r6
003b1430: asr      r2, r2, #8
003b1434: mov      r1, #1
003b1438: bl       #0x3790e0
003b143c: b        #0x3b11e8
003b1440: ldr      r3, [r6]
003b1444: mov      r0, r6
003b1448: mov      lr, pc
003b144c: ldr      pc, [r3, #0x54]
003b1450: cmp      r0, #0
003b1454: bne      #0x3b17c4
003b1458: cmp      r8, #0
003b145c: bne      #0x3b10f4
003b1460: cmp      r5, #0
003b1464: beq      #0x3b1aa8
003b1468: ldr      sl, [r5, #0x108]
003b146c: ldr      r8, [r6, #0x108]
003b1470: lsr      r3, sl, #0x1f
003b1474: orrs     r3, r3, r8, lsr #31
003b1478: beq      #0x3b14a0
003b147c: ldr      r3, [pc, #0x914]
003b1480: ldr      r3, [r7, r3]
003b1484: ldr      r3, [r3]
003b1488: cmp      r3, #2
003b148c: moveq    r3, #0
003b1490: streq    r3, [r3]
003b1494: beq      #0x3b14a0
003b1498: cmp      r3, #1
003b149c: beq      #0x3b1d08
003b14a0: bl       #0x80b1bc
003b14a4: mov      r1, sl
003b14a8: mov      fp, r0
003b14ac: mov      r2, r4
003b14b0: mov      r0, r8
003b14b4: mov      r3, #1
003b14b8: bl       #0x3af330
003b14bc: mov      r1, r0
003b14c0: mov      r0, fp
003b14c4: bl       #0x80e2a4
003b14c8: b        #0x3b10f4
003b14cc: ldr      r3, [pc, #0x8c8]
003b14d0: add      ip, sp, #0x124
003b14d4: mov      r0, sl
003b14d8: add      r3, pc, r3
003b14dc: str      ip, [sp, #0xc]
003b14e0: str      r3, [sp, #8]
003b14e4: bl       #0x337888
003b14e8: ldr      r3, [sp, #8]
003b14ec: add      r2, sp, #0x44
003b14f0: ldr      r0, [sp, #0xc]
003b14f4: mov      r1, r3
003b14f8: bl       #0x3140ec
003b14fc: mov      r0, sl
003b1500: ldr      r1, [sp, #0xc]
003b1504: bl       #0x337a88
003b1508: cmp      r0, #0
003b150c: ldr      r3, [sp, #8]
003b1510: beq      #0x3b17d0
003b1514: ldr      r3, [r5]
003b1518: mov      r0, r5
003b151c: mov      lr, pc
003b1520: ldr      pc, [r3, #0x28]
003b1524: cmp      r0, #0
003b1528: bne      #0x3b1a2c
003b152c: movw     r3, #0x14f0
003b1530: ldrb     r3, [r5, r3]
003b1534: cmp      r3, #0
003b1538: bne      #0x3b1a2c
003b153c: ldr      r0, [sp, #0xc]
003b1540: bl       #0x3139ac
003b1544: mov      r0, r8
003b1548: bl       #0x3139ac
003b154c: ldr      r3, [r4, #0x1c]
003b1550: tst      r3, #0x400000
003b1554: bne      #0x3b1a00
003b1558: ldr      r8, [r4]
003b155c: cmp      r8, #0
003b1560: ble      #0x3b1154
003b1564: ldr      r2, [pc, #0x824]
003b1568: ldr      r3, [r7, r2]
003b156c: str      r2, [sp, #0xc]
003b1570: ldr      r3, [r3, #0x40]
003b1574: ldr      sl, [r3, #0x6c4]
003b1578: cmp      sl, #1
003b157c: ble      #0x3b15c4
003b1580: mov      r0, r6
003b1584: bl       #0x3a3064
003b1588: cmp      r0, #0
003b158c: beq      #0x3b1ad8
003b1590: sub      r0, sl, #1
003b1594: bl       #0x30e964
003b1598: ldr      r3, [pc, #0x800]
003b159c: ldr      r3, [r7, r3]
003b15a0: ldr      r3, [r3]
003b15a4: ldr      r1, [r3, #0x34]
003b15a8: bl       #0x30ed6c
003b15ac: mov      r1, #0x3f800000
003b15b0: bl       #0x30eba4
003b15b4: bl       #0x30e4cc
003b15b8: ldr      r8, [r4]
003b15bc: mul      r8, r8, r0
003b15c0: str      r8, [r4]
003b15c4: mov      r0, r6
003b15c8: bl       #0x3bd394
003b15cc: mov      sl, r0
003b15d0: ldr      r0, [r4]
003b15d4: bl       #0x30e964
003b15d8: mov      r1, #0x3b800000
003b15dc: bl       #0x30ed6c
003b15e0: mov      r1, r0
003b15e4: mov      r0, sl
003b15e8: bl       #0x30ed6c
003b15ec: add      sl, r5, #0x3c8
003b15f0: mov      r2, r0
003b15f4: mov      r1, r6
003b15f8: mov      r0, sl
003b15fc: bl       #0x3d7c68
003b1600: mov      r1, #0
003b1604: bl       #0x30e2f8
003b1608: cmp      r0, #0
003b160c: bne      #0x3b1a64
003b1610: ldrb     r3, [r4, #0x18]
003b1614: ldr      r2, [r5, #0x110]
003b1618: and      r3, r3, #0x80
003b161c: uxtb     r3, r3
003b1620: cmp      r3, #0
003b1624: ldrne    r3, [r4, #0x1c]
003b1628: ubfxne   r3, r3, #0x14, #1
003b162c: cmn      r2, #1
003b1630: strb     r3, [r5, #0x53b]
003b1634: beq      #0x3b1b2c
003b1638: ldr      r3, [r4, #0x1c]
003b163c: tst      r3, #0x200000
003b1640: beq      #0x3b1680
003b1644: ldr      r8, [r4, #0x24]
003b1648: cmn      r8, #1
003b164c: addne    r8, r8, #0x7c
003b1650: beq      #0x3b1cd0
003b1654: mov      r0, r5
003b1658: bl       #0x3935dc
003b165c: ldr      r3, [pc, #0x740]
003b1660: mov      ip, #0
003b1664: mov      r2, r0
003b1668: mov      r1, r8
003b166c: ldr      r0, [r7, r3]
003b1670: add      r3, r5, #0x16c
003b1674: str      ip, [sp, #4]
003b1678: str      ip, [sp]
003b167c: bl       #0x495888
003b1680: ldr      r3, [r5]
003b1684: mov      r0, r5
003b1688: mov      lr, pc
003b168c: ldr      pc, [r3, #0x34]
003b1690: cmp      r0, #0
003b1694: beq      #0x3b16b4
003b1698: ldrb     r3, [r4, #0x18]
003b169c: ldrb     r2, [r4, #0x19]
003b16a0: and      r3, r3, #0xbf
003b16a4: bfc      r2, #0, #1
003b16a8: bfc      r3, #5, #1
003b16ac: strb     r2, [r4, #0x19]
003b16b0: strb     r3, [r4, #0x18]
003b16b4: ldrb     r3, [r4, #0x18]
003b16b8: tst      r3, #8
003b16bc: beq      #0x3b1154
003b16c0: ldr      r3, [r6]
003b16c4: mov      r0, r6
003b16c8: mov      lr, pc
003b16cc: ldr      pc, [r3, #0x28]
003b16d0: cmp      r0, #0
003b16d4: beq      #0x3b1154
003b16d8: ldr      r3, [sp, #0xc]
003b16dc: ldr      r0, [r7, r3]
003b16e0: bl       #0x31f594
003b16e4: cmp      r0, #0
003b16e8: beq      #0x3b1154
003b16ec: ldr      r8, [r0, #0x128]
003b16f0: cmp      r8, #0
003b16f4: beq      #0x3b1154
003b16f8: mov      r0, r8
003b16fc: mov      r1, r6
003b1700: bl       #0x40f980
003b1704: cmp      r0, #0
003b1708: beq      #0x3b1154
003b170c: mov      r3, #0
003b1710: mov      r0, r6
003b1714: add      r1, sp, #0x14
003b1718: str      r3, [sp, #0x1c]
003b171c: str      r3, [sp, #0x14]
003b1720: str      r3, [sp, #0x18]
003b1724: bl       #0x393ae4
003b1728: ldr      r3, [pc, #0x678]
003b172c: ldr      r1, [r8, #0x80]
003b1730: mov      ip, #0x1c
003b1734: ldr      r3, [r7, r3]
003b1738: mov      r0, r8
003b173c: mov      r2, #0
003b1740: ldr      lr, [r3]
003b1744: mov      r3, #1
003b1748: mla      r1, ip, r1, lr
003b174c: ldr      r1, [r1, #0xc]
003b1750: bl       #0x40f904
003b1754: b        #0x3b1154
003b1758: ldr      r3, [pc, #0x630]
003b175c: mov      r1, r5
003b1760: mov      r2, #0
003b1764: ldr      r3, [r7, r3]
003b1768: ldr      r8, [pc, #0x624]
003b176c: ldr      r0, [r3, #0x40]
003b1770: bl       #0x36eea8
003b1774: mov      r1, #3
003b1778: ldr      r2, [r0, #0x670]
003b177c: ldr      r0, [r7, r8]
003b1780: bl       #0x3790ec
003b1784: b        #0x3b11d0
003b1788: add      r0, r6, #0x3c8
003b178c: mov      r1, r6
003b1790: mov      r2, r5
003b1794: mov      r3, r4
003b1798: ldr      ip, [r6, #0x3c8]
003b179c: mov      lr, pc
003b17a0: ldr      pc, [ip, #0xb4]
003b17a4: ldr      ip, [r5, #0x3c8]
003b17a8: add      r0, r5, #0x3c8
003b17ac: mov      r1, r6
003b17b0: mov      r2, r5
003b17b4: mov      r3, r4
003b17b8: mov      lr, pc
003b17bc: ldr      pc, [ip, #0xb4]
003b17c0: b        #0x3b11b8
003b17c4: cmp      r8, #0
003b17c8: beq      #0x3b11e8
003b17cc: b        #0x3b10f4
003b17d0: mov      r1, r3
003b17d4: ldr      r3, [pc, #0x5b4]
003b17d8: ldr      r0, [r7, r3]
003b17dc: bl       #0x320e14
003b17e0: cmp      r0, #0
003b17e4: beq      #0x3b152c
003b17e8: b        #0x3b1514
003b17ec: add      r0, r5, #0x4f0
003b17f0: add      r0, r0, #0xc
003b17f4: mov      r1, #0
003b17f8: bl       #0x3c0260
003b17fc: cmp      r0, #0
003b1800: beq      #0x3b1258
003b1804: ldrb     r3, [r4, #0x18]
003b1808: tst      r3, #0x16
003b180c: bne      #0x3b125c
003b1810: ldr      r2, [r4]
003b1814: cmp      r2, #0
003b1818: orrle    r3, r3, #2
003b181c: orrgt    r3, r3, #0x10
003b1820: strble   r3, [r4, #0x18]
003b1824: uxtble   r3, r3
003b1828: strbgt   r3, [r4, #0x18]
003b182c: tst      r3, #2
003b1830: beq      #0x3b1264
003b1834: add      r0, r5, #0x4f0
003b1838: add      r0, r0, #0xc
003b183c: mov      r1, r6
003b1840: mov      r2, #0
003b1844: bl       #0x3c5b3c
003b1848: ldr      r3, [r5]
003b184c: mov      r0, r5
003b1850: mov      lr, pc
003b1854: ldr      pc, [r3, #0x28]
003b1858: cmp      r0, #0
003b185c: bne      #0x3b1bf0
003b1860: ldr      sl, [r7, fp]
003b1864: add      r8, sp, #0xdc
003b1868: mov      r0, sl
003b186c: bl       #0x337888
003b1870: ldr      r1, [pc, #0x534]
003b1874: add      r2, sp, #0x38
003b1878: mov      r0, r8
003b187c: add      r1, pc, r1
003b1880: bl       #0x3140ec
003b1884: mov      r1, r8
003b1888: mov      r0, sl
003b188c: bl       #0x337a88
003b1890: mov      r0, r8
003b1894: bl       #0x3139ac
003b1898: ldrb     r3, [r4, #0x18]
003b189c: tst      r3, #4
003b18a0: beq      #0x3b126c
003b18a4: add      r0, r5, #0x4f0
003b18a8: add      r0, r0, #0xc
003b18ac: mov      r1, r6
003b18b0: mov      r2, #0
003b18b4: bl       #0x3c5c60
003b18b8: ldr      r3, [r5]
003b18bc: mov      r0, r5
003b18c0: mov      lr, pc
003b18c4: ldr      pc, [r3, #0x28]
003b18c8: cmp      r0, #0
003b18cc: bne      #0x3b1b80
003b18d0: ldr      sl, [r7, fp]
003b18d4: add      r8, sp, #0xc4
003b18d8: mov      r0, sl
003b18dc: bl       #0x337888
003b18e0: ldr      r1, [pc, #0x4c8]
003b18e4: add      r2, sp, #0x34
003b18e8: mov      r0, r8
003b18ec: add      r1, pc, r1
003b18f0: bl       #0x3140ec
003b18f4: mov      r1, r8
003b18f8: mov      r0, sl
003b18fc: bl       #0x337a88
003b1900: mov      r0, r8
003b1904: bl       #0x3139ac
003b1908: ldrb     r3, [r4, #0x18]
003b190c: tst      r3, #0x10
003b1910: beq      #0x3b1274
003b1914: add      r0, r5, #0x4f0
003b1918: mov      r1, r6
003b191c: ldr      r2, [sp, #0xc]
003b1920: add      r0, r0, #0xc
003b1924: bl       #0x3c5d84
003b1928: ldr      sl, [r7, fp]
003b192c: add      r8, sp, #0xac
003b1930: mov      r0, sl
003b1934: bl       #0x337888
003b1938: ldr      r1, [pc, #0x474]
003b193c: add      r2, sp, #0x30
003b1940: mov      r0, r8
003b1944: add      r1, pc, r1
003b1948: bl       #0x3140ec
003b194c: mov      r1, r8
003b1950: mov      r0, sl
003b1954: bl       #0x337a88
003b1958: mov      r0, r8
003b195c: bl       #0x3139ac
003b1960: ldrb     r3, [r4, #0x18]
003b1964: tst      r3, #0x80
003b1968: beq      #0x3b127c
003b196c: ldr      r1, [r4, #0x1c]
003b1970: add      r0, r5, #0x4f0
003b1974: add      r0, r0, #0xc
003b1978: ubfx     r1, r1, #0x14, #1
003b197c: mov      r2, r6
003b1980: ldr      r3, [sp, #0xc]
003b1984: bl       #0x3c5ea0
003b1988: ldr      r3, [r5]
003b198c: mov      r0, r5
003b1990: mov      lr, pc
003b1994: ldr      pc, [r3, #0x28]
003b1998: cmp      r0, #0
003b199c: bne      #0x3b1c60
003b19a0: ldr      sl, [r7, fp]
003b19a4: add      r8, sp, #0x94
003b19a8: mov      r0, sl
003b19ac: bl       #0x337888
003b19b0: ldr      r1, [pc, #0x400]
003b19b4: add      r2, sp, #0x2c
003b19b8: mov      r0, r8
003b19bc: add      r1, pc, r1
003b19c0: bl       #0x3140ec
003b19c4: mov      r1, r8
003b19c8: mov      r0, sl
003b19cc: bl       #0x337a88
003b19d0: mov      r0, r8
003b19d4: bl       #0x3139ac
003b19d8: ldrb     r3, [r4, #0x18]
003b19dc: b        #0x3b127c
003b19e0: ldr      ip, [sp, #0xc]
003b19e4: add      r0, r5, #0x4f0
003b19e8: add      r0, r0, #0xc
003b19ec: mov      r2, #1
003b19f0: mov      r3, r6
003b19f4: str      ip, [sp]
003b19f8: bl       #0x3c6144
003b19fc: b        #0x3b1334
003b1a00: ldr      r3, [r6, #0x39c]
003b1a04: ldr      r2, [r4]
003b1a08: add      r0, r6, #0x37c
003b1a0c: lsl      r3, r3, #8
003b1a10: cmp      r3, r2
003b1a14: movge    r3, r2
003b1a18: asr      r1, r3, #8
003b1a1c: str      r3, [r4]
003b1a20: rsb      r1, r1, #0
003b1a24: bl       #0x3fe164
003b1a28: b        #0x3b1558
003b1a2c: ldr      r0, [sp, #0xc]
003b1a30: bl       #0x3139ac
003b1a34: b        #0x3b114c
003b1a38: ldr      r0, [r7, r8]
003b1a3c: mov      r1, #0
003b1a40: mov      r2, r6
003b1a44: bl       #0x3790ec
003b1a48: b        #0x3b1424
003b1a4c: ldr      r8, [pc, #0x340]
003b1a50: mov      r1, #2
003b1a54: mov      r2, r6
003b1a58: ldr      r0, [r7, r8]
003b1a5c: bl       #0x3790ec
003b1a60: b        #0x3b140c
003b1a64: ldr      r3, [r7, fp]
003b1a68: add      sl, sp, #0x10c
003b1a6c: mov      r0, r3
003b1a70: str      r3, [sp, #8]
003b1a74: bl       #0x337888
003b1a78: ldr      r1, [pc, #0x33c]
003b1a7c: add      r2, sp, #0x40
003b1a80: mov      r0, sl
003b1a84: add      r1, pc, r1
003b1a88: bl       #0x3140ec
003b1a8c: ldr      r3, [sp, #8]
003b1a90: mov      r1, sl
003b1a94: mov      r0, r3
003b1a98: bl       #0x337a88
003b1a9c: mov      r0, sl
003b1aa0: bl       #0x3139ac
003b1aa4: b        #0x3b1610
003b1aa8: ldr      r3, [pc, #0x2e8]
003b1aac: ldr      r3, [r7, r3]
003b1ab0: ldr      r3, [r3]
003b1ab4: cmp      r3, #2
003b1ab8: streq    r5, [r5]
003b1abc: beq      #0x3b1ac8
003b1ac0: cmp      r3, #1
003b1ac4: beq      #0x3b1d3c
003b1ac8: ldr      r8, [r6, #0x108]
003b1acc: mov      r3, #1
003b1ad0: mvn      sl, #0
003b1ad4: b        #0x3b1474
003b1ad8: ldr      r3, [r6]
003b1adc: mov      r0, r6
003b1ae0: mov      lr, pc
003b1ae4: ldr      pc, [r3, #0x28]
003b1ae8: cmp      r0, #0
003b1aec: beq      #0x3b15c4
003b1af0: sub      r0, sl, #1
003b1af4: bl       #0x30e964
003b1af8: ldr      r3, [pc, #0x2a0]
003b1afc: ldr      r3, [r7, r3]
003b1b00: ldr      r3, [r3]
003b1b04: ldr      r1, [r3, #0x38]
003b1b08: bl       #0x30ed6c
003b1b0c: mov      r1, #0x3f800000
003b1b10: bl       #0x30eba4
003b1b14: bl       #0x30e4cc
003b1b18: mov      r1, r0
003b1b1c: mov      r0, r8
003b1b20: bl       #0x30e2a4
003b1b24: mov      r8, r0
003b1b28: b        #0x3b15c4
003b1b2c: ldr      r3, [r7, fp]
003b1b30: add      sl, sp, #0xf4
003b1b34: mov      r0, r3
003b1b38: str      r3, [sp, #8]
003b1b3c: bl       #0x337888
003b1b40: ldr      r1, [pc, #0x278]
003b1b44: add      r2, sp, #0x3c
003b1b48: mov      r0, sl
003b1b4c: add      r1, pc, r1
003b1b50: bl       #0x3140ec
003b1b54: ldr      r3, [sp, #8]
003b1b58: mov      r1, sl
003b1b5c: mov      r0, r3
003b1b60: bl       #0x337a88
003b1b64: mov      r0, sl
003b1b68: bl       #0x3139ac
003b1b6c: mov      r0, r5
003b1b70: mov      r1, r8
003b1b74: mov      r2, r6
003b1b78: bl       #0x3a8bc4
003b1b7c: b        #0x3b1638
003b1b80: add      r8, r5, #0x560
003b1b84: mov      r0, r8
003b1b88: mov      r1, #0xd6
003b1b8c: mov      r2, #1
003b1b90: bl       #0x3e0798
003b1b94: ldr      r3, [pc, #0x228]
003b1b98: mov      r0, r8
003b1b9c: mov      r1, #0xd6
003b1ba0: ldr      r3, [r7, r3]
003b1ba4: mov      r2, #0
003b1ba8: ldr      r8, [r3]
003b1bac: bl       #0x3df6e0
003b1bb0: cmp      r0, #0x1f4
003b1bb4: blt      #0x3b18d0
003b1bb8: ldr      r3, [pc, #0x1d0]
003b1bbc: mov      r1, r5
003b1bc0: ldr      r3, [r7, r3]
003b1bc4: ldr      r0, [r3, #0x40]
003b1bc8: bl       #0x36effc
003b1bcc: cmp      r0, #0
003b1bd0: beq      #0x3b18d0
003b1bd4: ldr      r0, [pc, #0x1ec]
003b1bd8: add      r0, pc, r0
003b1bdc: bl       #0x3a3f70
003b1be0: mov      r1, r0
003b1be4: mov      r0, r8
003b1be8: bl       #0x3813b8
003b1bec: b        #0x3b18d0
003b1bf0: add      r8, r5, #0x560
003b1bf4: mov      r0, r8
003b1bf8: mov      r1, #0xd7
003b1bfc: mov      r2, #1
003b1c00: bl       #0x3e0798
003b1c04: ldr      r3, [pc, #0x1b8]
003b1c08: mov      r0, r8
003b1c0c: mov      r1, #0xd7
003b1c10: ldr      r3, [r7, r3]
003b1c14: mov      r2, #0
003b1c18: ldr      r8, [r3]
003b1c1c: bl       #0x3df6e0
003b1c20: cmp      r0, #0x1f4
003b1c24: blt      #0x3b1860
003b1c28: ldr      r3, [pc, #0x160]
003b1c2c: mov      r1, r5
003b1c30: ldr      r3, [r7, r3]
003b1c34: ldr      r0, [r3, #0x40]
003b1c38: bl       #0x36effc
003b1c3c: cmp      r0, #0
003b1c40: beq      #0x3b1860
003b1c44: ldr      r0, [pc, #0x180]
003b1c48: add      r0, pc, r0
003b1c4c: bl       #0x3a3f70
003b1c50: mov      r1, r0
003b1c54: mov      r0, r8
003b1c58: bl       #0x3813b8
003b1c5c: b        #0x3b1860
003b1c60: add      r8, r5, #0x560
003b1c64: mov      r0, r8
003b1c68: mov      r1, #0xde
003b1c6c: mov      r2, #1
003b1c70: bl       #0x3e0798
003b1c74: ldr      r3, [pc, #0x148]
003b1c78: mov      r0, r8
003b1c7c: mov      r1, #0xde
003b1c80: ldr      r3, [r7, r3]
003b1c84: mov      r2, #0
003b1c88: ldr      r8, [r3]
003b1c8c: bl       #0x3df6e0
003b1c90: cmp      r0, #0x31
003b1c94: ble      #0x3b19a0
003b1c98: ldr      r3, [pc, #0xf0]
003b1c9c: mov      r1, r5
003b1ca0: ldr      r3, [r7, r3]
003b1ca4: ldr      r0, [r3, #0x40]
003b1ca8: bl       #0x36effc
003b1cac: cmp      r0, #0
003b1cb0: beq      #0x3b19a0
003b1cb4: ldr      r0, [pc, #0x114]
003b1cb8: add      r0, pc, r0
003b1cbc: bl       #0x3a3f70
003b1cc0: mov      r1, r0
003b1cc4: mov      r0, r8
003b1cc8: bl       #0x3813b8
003b1ccc: b        #0x3b19a0
003b1cd0: ldr      r3, [r5]
003b1cd4: mov      r0, r5
003b1cd8: mov      lr, pc
003b1cdc: ldr      pc, [r3, #0x34]
003b1ce0: cmp      r0, #0
003b1ce4: beq      #0x3b1cf8
003b1ce8: mov      r0, r5
003b1cec: bl       #0x3a3368
003b1cf0: mov      r8, r0
003b1cf4: b        #0x3b1654
003b1cf8: mov      r0, r5
003b1cfc: bl       #0x3a33d0
003b1d00: mov      r8, r0
003b1d04: b        #0x3b1654
003b1d08: ldr      r0, [pc, #0xc4]
003b1d0c: ldr      r1, [pc, #0xc4]
003b1d10: ldr      r2, [pc, #0xc4]
003b1d14: ldr      r0, [r7, r0]
003b1d18: ldr      r3, [pc, #0xc0]
003b1d1c: movw     ip, #0x2d2
003b1d20: add      r1, pc, r1
003b1d24: add      r2, pc, r2
003b1d28: add      r3, pc, r3
003b1d2c: add      r0, r0, #0xa8
003b1d30: str      ip, [sp]
003b1d34: bl       #0x30e004
003b1d38: b        #0x3b14a0
003b1d3c: ldr      r0, [pc, #0x90]
003b1d40: ldr      r1, [pc, #0x9c]
003b1d44: ldr      r2, [pc, #0x9c]
003b1d48: ldr      r0, [r7, r0]
003b1d4c: ldr      r3, [pc, #0x98]
003b1d50: mov      ip, #0x2c8
003b1d54: add      r1, pc, r1
003b1d58: add      r2, pc, r2
003b1d5c: add      r3, pc, r3
003b1d60: add      r0, r0, #0xa8
003b1d64: str      ip, [sp]
003b1d68: bl       #0x30e004
003b1d6c: b        #0x3b1ac8
003b1d70: bl       #0x30e310
003b1d74: subseq   r3, lr, ip, asr #19
003b1d78: andeq    r4, r0, ip, lsr #1
003b1d7c: andeq    r0, r0, r4, lsl #17
003b1d80: subseq   r2, r1, r0, ror fp
003b1d84: ldrsbeq  r2, [r1], #-0x94
003b1d88: subseq   r2, r1, r8, ror #18
003b1d8c: ldrsheq  r2, [r1], #-0x88
003b1d90: strdeq   r3, r4, [r0], -r4
003b1d94: andeq    r2, r0, r4, lsl r7
003b1d98: andeq    r3, r0, r0, asr #19
003b1d9c: ldrsbeq  r2, [r1], #-0x78
003b1da0: andeq    r3, r0, r8, asr #5
003b1da4: andeq    r1, r0, r8, lsl #22
003b1da8: ldrdeq   r3, r4, [r0], -r4
003b1dac: subseq   r2, r1, ip, lsr r4
003b1db0: subseq   r2, r1, ip, asr #7
003b1db4: subseq   r2, r1, r4, ror r3
003b1db8: ldrsheq  r2, [r1], #-0x2c
003b1dbc: subseq   r2, r1, ip, asr r2
003b1dc0: subseq   r2, r1, ip, ror #2
003b1dc4: andeq    r1, r0, r0, ror sp
003b1dc8: subseq   r2, r1, r0, lsr r1
003b1dcc: ldrheq   r2, [r1], #-0
003b1dd0: subseq   r2, r1, r0, rrx
003b1dd4: andeq    r1, r0, r0, asr #19
003b1dd8: ldrheq   ip, [r0], #-0x68
003b1ddc: subseq   r1, r1, r4, asr #30
003b1de0: subseq   r1, r1, r8, ror #29
003b1de4: subseq   ip, r0, r4, lsl #13
003b1de8: subseq   r1, r1, r0, lsr #29
003b1dec: ldrheq   r1, [r1], #-0xe4

# _ZN9Character9Ctrl_KillEP10GameObjectb
003ad528: push     {r4, r5, r6, lr}
003ad52c: ldr      r3, [r0]
003ad530: mov      r4, r0
003ad534: mov      r5, r1
003ad538: mov      r6, r2
003ad53c: mov      lr, pc
003ad540: ldr      pc, [r3, #0x34]
003ad544: cmp      r0, #0
003ad548: beq      #0x3ad550
003ad54c: pop      {r4, r5, r6, pc}
003ad550: mov      r2, r6
003ad554: mov      r1, r5
003ad558: mov      r0, r4
003ad55c: bl       #0x3a5b18
003ad560: mov      r0, r4
003ad564: mov      r2, r5
003ad568: mov      r1, #2
003ad56c: pop      {r4, r5, r6, lr}
003ad570: b        #0x3a4d5c

# _ZN9Character7RegenMPEi
003bdbb8: push     {r4, r5, r6, r7, r8, sl, lr}
003bdbbc: ldr      r5, [pc, #0xd0]
003bdbc0: ldr      r8, [pc, #0xd0]
003bdbc4: add      r6, r0, #0xff0
003bdbc8: add      r5, pc, r5
003bdbcc: ldr      r3, [r5, r8]
003bdbd0: add      r6, r6, #4
003bdbd4: add      r7, r0, #0x560
003bdbd8: ldr      r3, [r3]
003bdbdc: mov      r4, r1
003bdbe0: sub      sp, sp, #0x24
003bdbe4: mov      r2, #0x29
003bdbe8: mov      r1, r6
003bdbec: mov      r0, r7
003bdbf0: str      r3, [sp, #0x1c]
003bdbf4: bl       #0x3dedb4
003bdbf8: mov      sl, r0
003bdbfc: mov      r1, r6
003bdc00: mov      r0, r7
003bdc04: mov      r2, #0x2b
003bdc08: bl       #0x3dedb4
003bdc0c: cmp      r4, #0
003bdc10: movlt    r4, r0
003bdc14: add      r3, r4, sl
003bdc18: cmp      r3, r0
003bdc1c: rsbgt    r4, sl, r0
003bdc20: cmp      r4, #0
003bdc24: ble      #0x3bdc74
003bdc28: ldr      r3, [pc, #0x6c]
003bdc2c: add      r6, sp, #4
003bdc30: ldr      sl, [r5, r3]
003bdc34: mov      r0, sl
003bdc38: bl       #0x337888
003bdc3c: ldr      r1, [pc, #0x5c]
003bdc40: mov      r2, sp
003bdc44: mov      r0, r6
003bdc48: add      r1, pc, r1
003bdc4c: bl       #0x3140ec
003bdc50: mov      r1, r6
003bdc54: mov      r0, sl
003bdc58: bl       #0x337a88
003bdc5c: mov      r0, r6
003bdc60: bl       #0x318254
003bdc64: mov      r0, r7
003bdc68: mov      r2, r4
003bdc6c: mov      r1, #0x29
003bdc70: bl       #0x3e0708
003bdc74: ldr      r3, [r5, r8]
003bdc78: ldr      r2, [sp, #0x1c]
003bdc7c: ldr      r3, [r3]
003bdc80: cmp      r2, r3
003bdc84: bne      #0x3bdc90
003bdc88: add      sp, sp, #0x24
003bdc8c: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdc90: bl       #0x30e310
003bdc94: subseq   r6, sp, r8, asr #29
003bdc98: andeq    r4, r0, ip, lsr #1
003bdc9c: andeq    r0, r0, r4, lsl #17
003bdca0: subseq   r6, r0, r8, asr #24

# _ZN9Character4KillEP10GameObjectb
003a5b18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a5b1c: sub      sp, sp, #0xb4
003a5b20: ldr      r3, [r0]
003a5b24: mov      r6, r2
003a5b28: mov      r4, r0
003a5b2c: mov      r7, r1
003a5b30: mov      lr, pc
003a5b34: ldr      pc, [r3, #0x34]
003a5b38: ldr      r5, [pc, #0x664]
003a5b3c: subs     r2, r0, #0
003a5b40: add      r5, pc, r5
003a5b44: beq      #0x3a5b50
003a5b48: add      sp, sp, #0xb4
003a5b4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a5b50: movw     r3, #0x1449
003a5b54: mov      sl, #1
003a5b58: add      r8, r4, #0x560
003a5b5c: strb     sl, [r4, r3]
003a5b60: mov      r0, r8
003a5b64: mov      r1, #0x24
003a5b68: bl       #0x3e07a0
003a5b6c: ldr      r3, [r4]
003a5b70: mov      r0, r4
003a5b74: mov      lr, pc
003a5b78: ldr      pc, [r3, #0x28]
003a5b7c: cmp      r0, #0
003a5b80: beq      #0x3a5be0
003a5b84: cmp      r6, #0
003a5b88: bne      #0x3a5be0
003a5b8c: ldr      sb, [pc, #0x614]
003a5b90: mov      r2, sl
003a5b94: mov      r0, r8
003a5b98: mov      r1, #0x19
003a5b9c: bl       #0x3e0798
003a5ba0: ldr      r3, [r5, sb]
003a5ba4: mov      r1, r4
003a5ba8: ldr      r0, [r3, #0x40]
003a5bac: bl       #0x36effc
003a5bb0: cmp      r0, #0
003a5bb4: bne      #0x3a602c
003a5bb8: bl       #0x7fd794
003a5bbc: ldrb     r3, [r0, #5]
003a5bc0: cmp      r3, #0
003a5bc4: beq      #0x3a5b48
003a5bc8: ldr      r3, [r5, sb]
003a5bcc: mov      r1, #0
003a5bd0: mov      r2, #1
003a5bd4: ldr      r0, [r3, #0x40]
003a5bd8: bl       #0x36e478
003a5bdc: b        #0x3a5b48
003a5be0: ldr      sb, [pc, #0x5c0]
003a5be4: ldr      r0, [r5, sb]
003a5be8: bl       #0x31f594
003a5bec: ldr      r3, [r0, #0x150]
003a5bf0: cmp      r3, #0
003a5bf4: beq      #0x3a601c
003a5bf8: cmp      r7, #0
003a5bfc: beq      #0x3a609c
003a5c00: cmp      r6, #0
003a5c04: bne      #0x3a5b48
003a5c08: movw     r3, #0x144c
003a5c0c: str      r7, [r4, r3]
003a5c10: ldr      r3, [pc, #0x594]
003a5c14: ldr      r2, [pc, #0x594]
003a5c18: cmp      r7, r4
003a5c1c: movne    fp, #0
003a5c20: moveq    fp, #1
003a5c24: add      r3, pc, r3
003a5c28: str      r3, [sp, #0x24]
003a5c2c: ldr      r3, [pc, #0x580]
003a5c30: add      r8, r4, #0x3c8
003a5c34: str      r2, [sp, #0x14]
003a5c38: add      r3, pc, r3
003a5c3c: str      r3, [sp, #0x20]
003a5c40: ldr      r3, [pc, #0x570]
003a5c44: mov      sl, r7
003a5c48: add      r3, pc, r3
003a5c4c: str      r3, [sp, #0x1c]
003a5c50: ldr      r3, [pc, #0x564]
003a5c54: add      r3, pc, r3
003a5c58: str      r3, [sp, #0x18]
003a5c5c: mov      r0, r8
003a5c60: bl       #0x3d4a10
003a5c64: cmp      r6, r0
003a5c68: bge      #0x3a5de8
003a5c6c: mov      ip, #0
003a5c70: str      ip, [sp, #0xac]
003a5c74: mov      r0, r8
003a5c78: mov      ip, #0
003a5c7c: mov      r1, r6
003a5c80: add      r2, sp, #0xac
003a5c84: add      r3, sp, #0xa8
003a5c88: str      ip, [sp, #0xa8]
003a5c8c: bl       #0x3d72d0
003a5c90: ldr      r7, [sp, #0xac]
003a5c94: cmp      r7, #0
003a5c98: beq      #0x3a5d40
003a5c9c: movw     r3, #0x14d4
003a5ca0: ldr      r0, [r7, r3]
003a5ca4: cmp      r0, #0
003a5ca8: strne    r0, [sp, #0xac]
003a5cac: beq      #0x3a5de0
003a5cb0: mov      r1, #4
003a5cb4: mov      r2, r4
003a5cb8: bl       #0x3a4d5c
003a5cbc: ldr      r3, [sp, #0xac]
003a5cc0: cmp      r3, #0
003a5cc4: beq      #0x3a5d40
003a5cc8: mov      r0, r3
003a5ccc: ldr      r3, [r3]
003a5cd0: mov      lr, pc
003a5cd4: ldr      pc, [r3, #0x28]
003a5cd8: cmp      r0, #0
003a5cdc: beq      #0x3a5d40
003a5ce0: ldr      r0, [sp, #0xac]
003a5ce4: movw     r3, #0x14a4
003a5ce8: mov      r1, #0x17
003a5cec: ldr      r2, [r0, r3]
003a5cf0: cmp      r4, r2
003a5cf4: moveq    r2, #0
003a5cf8: streq    r2, [r0, r3]
003a5cfc: ldreq    r0, [sp, #0xac]
003a5d00: mov      r2, #1
003a5d04: add      r0, r0, #0x560
003a5d08: bl       #0x3e0798
003a5d0c: ldr      r0, [sp, #0xac]
003a5d10: mov      r1, #0x18
003a5d14: mov      r2, #1
003a5d18: add      r0, r0, #0x560
003a5d1c: bl       #0x3e0798
003a5d20: ldr      r3, [r5, sb]
003a5d24: cmp      sl, r7
003a5d28: ldr      r1, [sp, #0xac]
003a5d2c: ldr      r0, [r3, #0x40]
003a5d30: moveq    fp, #1
003a5d34: bl       #0x36effc
003a5d38: cmp      r0, #0
003a5d3c: bne      #0x3a5d48
003a5d40: add      r6, r6, #1
003a5d44: b        #0x3a5c5c
003a5d48: ldr      r2, [sp, #0x14]
003a5d4c: ldr      r0, [sp, #0xac]
003a5d50: mov      r1, #0x17
003a5d54: ldr      r3, [r5, r2]
003a5d58: add      r0, r0, #0x560
003a5d5c: mov      r2, #0
003a5d60: ldr      r7, [r3]
003a5d64: bl       #0x3df6e0
003a5d68: cmp      r0, #0x64
003a5d6c: beq      #0x3a60fc
003a5d70: ldr      r0, [sp, #0xac]
003a5d74: mov      r1, #0x17
003a5d78: mov      r2, #0
003a5d7c: add      r0, r0, #0x560
003a5d80: bl       #0x3df6e0
003a5d84: cmp      r0, #0x1f4
003a5d88: beq      #0x3a6118
003a5d8c: ldr      r0, [sp, #0xac]
003a5d90: mov      r1, #0x17
003a5d94: mov      r2, #0
003a5d98: add      r0, r0, #0x560
003a5d9c: bl       #0x3df6e0
003a5da0: cmp      r0, #0x3e8
003a5da4: beq      #0x3a60a8
003a5da8: ldr      r0, [sp, #0xac]
003a5dac: mov      r1, #0x17
003a5db0: mov      r2, #0
003a5db4: add      r0, r0, #0x560
003a5db8: bl       #0x3df6e0
003a5dbc: cmp      r0, #0x7d0
003a5dc0: bne      #0x3a5d40
003a5dc4: ldr      r0, [sp, #0x24]
003a5dc8: bl       #0x3a3f70
003a5dcc: mov      r1, r0
003a5dd0: mov      r0, r7
003a5dd4: bl       #0x3813b8
003a5dd8: add      r6, r6, #1
003a5ddc: b        #0x3a5c5c
003a5de0: mov      r0, r7
003a5de4: b        #0x3a5cb0
003a5de8: cmp      fp, #0
003a5dec: mov      r7, sl
003a5df0: beq      #0x3a5e1c
003a5df4: movw     r8, #0x144c
003a5df8: ldr      r3, [r4, r8]
003a5dfc: mov      r0, r3
003a5e00: ldr      r3, [r3]
003a5e04: mov      lr, pc
003a5e08: ldr      pc, [r3, #0x24]
003a5e0c: cmp      r0, #0
003a5e10: bne      #0x3a60c4
003a5e14: mov      r1, r4
003a5e18: bl       #0x3bf828
003a5e1c: ldr      r3, [r4]
003a5e20: mov      r0, r4
003a5e24: mov      lr, pc
003a5e28: ldr      pc, [r3, #0x54]
003a5e2c: cmp      r0, #0
003a5e30: bne      #0x3a5b48
003a5e34: movw     r3, #0x14e4
003a5e38: ldrb     r3, [r4, r3]
003a5e3c: cmp      r3, #0
003a5e40: bne      #0x3a5b48
003a5e44: ldr      r0, [r5, sb]
003a5e48: bl       #0x31f594
003a5e4c: cmp      r0, #0
003a5e50: str      r0, [sp, #0x14]
003a5e54: beq      #0x3a6150
003a5e58: ldr      r8, [pc, #0x360]
003a5e5c: movw     r3, #0x13c8
003a5e60: ldr      sl, [r5, sb]
003a5e64: ldr      r2, [pc, #0x358]
003a5e68: ldrsh    ip, [r4, r3]
003a5e6c: add      r8, pc, r8
003a5e70: ldr      r0, [sl, #0x2c]
003a5e74: add      r2, pc, r2
003a5e78: mov      r1, r8
003a5e7c: ldr      fp, [r4, #0x64]
003a5e80: str      r3, [sp, #0x10]
003a5e84: str      ip, [sp, #0xc]
003a5e88: bl       #0x4c4bdc
003a5e8c: ldr      r2, [pc, #0x334]
003a5e90: ldr      ip, [sp, #0xc]
003a5e94: mov      r6, #0
003a5e98: ldr      r2, [r5, r2]
003a5e9c: mvn      sb, #0
003a5ea0: str      r0, [sp, #0x84]
003a5ea4: add      r2, r2, #8
003a5ea8: ldr      r0, [sp, #0x14]
003a5eac: add      r1, sp, #0x80
003a5eb0: str      fp, [sp, #0x8c]
003a5eb4: ldr      fp, [pc, #0x310]
003a5eb8: str      r2, [sp, #0x80]
003a5ebc: str      ip, [sp, #0x98]
003a5ec0: str      r7, [sp, #0x88]
003a5ec4: strb     r6, [sp, #0x90]
003a5ec8: strb     r6, [sp, #0x91]
003a5ecc: str      sb, [sp, #0x94]
003a5ed0: bl       #0x339090
003a5ed4: ldr      r3, [sp, #0x10]
003a5ed8: ldr      fp, [r5, fp]
003a5edc: ldr      r2, [pc, #0x2ec]
003a5ee0: ldrsh    r3, [r4, r3]
003a5ee4: add      fp, fp, #8
003a5ee8: str      fp, [sp, #0x80]
003a5eec: str      r3, [sp, #0x18]
003a5ef0: ldr      ip, [r4, #0x64]
003a5ef4: add      r2, pc, r2
003a5ef8: mov      r1, r8
003a5efc: ldr      r0, [sl, #0x2c]
003a5f00: str      ip, [sp, #0xc]
003a5f04: bl       #0x4c4bdc
003a5f08: ldr      r3, [pc, #0x2c4]
003a5f0c: ldr      ip, [sp, #0xc]
003a5f10: str      r0, [sp, #0x68]
003a5f14: ldr      r3, [r5, r3]
003a5f18: add      r1, sp, #0x64
003a5f1c: ldr      r0, [sp, #0x14]
003a5f20: add      r3, r3, #8
003a5f24: str      r3, [sp, #0x64]
003a5f28: ldr      r3, [sp, #0x18]
003a5f2c: str      ip, [sp, #0x70]
003a5f30: str      r7, [sp, #0x6c]
003a5f34: str      r3, [sp, #0x7c]
003a5f38: strb     r6, [sp, #0x74]
003a5f3c: strb     r6, [sp, #0x75]
003a5f40: str      sb, [sp, #0x78]
003a5f44: bl       #0x339090
003a5f48: movw     r2, #0x13ca
003a5f4c: ldrsh    ip, [r4, r2]
003a5f50: str      fp, [sp, #0x64]
003a5f54: cmp      ip, sb
003a5f58: beq      #0x3a5b48
003a5f5c: ldr      r2, [pc, #0x274]
003a5f60: ldr      r3, [r4, #0x64]
003a5f64: mov      r1, r8
003a5f68: ldr      r0, [sl, #0x2c]
003a5f6c: add      r2, pc, r2
003a5f70: str      r3, [sp, #0x18]
003a5f74: str      ip, [sp, #0xc]
003a5f78: bl       #0x4c4bdc
003a5f7c: ldr      r3, [pc, #0x258]
003a5f80: ldr      r2, [sp, #0x18]
003a5f84: ldr      ip, [sp, #0xc]
003a5f88: ldr      r3, [r5, r3]
003a5f8c: str      r0, [sp, #0x4c]
003a5f90: add      r1, sp, #0x48
003a5f94: ldr      r0, [sp, #0x14]
003a5f98: add      r3, r3, #8
003a5f9c: str      r2, [sp, #0x54]
003a5fa0: str      r3, [sp, #0x48]
003a5fa4: str      ip, [sp, #0x60]
003a5fa8: str      r7, [sp, #0x50]
003a5fac: strb     r6, [sp, #0x58]
003a5fb0: strb     r6, [sp, #0x59]
003a5fb4: str      sb, [sp, #0x5c]
003a5fb8: bl       #0x339090
003a5fbc: ldr      r2, [pc, #0x21c]
003a5fc0: str      fp, [sp, #0x48]
003a5fc4: mov      r1, r8
003a5fc8: movw     r3, #0x13ca
003a5fcc: ldr      r0, [sl, #0x2c]
003a5fd0: add      r2, pc, r2
003a5fd4: ldrsh    r8, [r4, r3]
003a5fd8: ldr      r4, [r4, #0x64]
003a5fdc: bl       #0x4c4bdc
003a5fe0: ldr      r3, [pc, #0x1fc]
003a5fe4: str      r0, [sp, #0x30]
003a5fe8: add      r1, sp, #0x2c
003a5fec: ldr      r3, [r5, r3]
003a5ff0: ldr      r0, [sp, #0x14]
003a5ff4: str      r7, [sp, #0x34]
003a5ff8: add      r3, r3, #8
003a5ffc: str      r4, [sp, #0x38]
003a6000: strb     r6, [sp, #0x3d]
003a6004: str      sb, [sp, #0x40]
003a6008: str      r3, [sp, #0x2c]
003a600c: str      r8, [sp, #0x44]
003a6010: strb     r6, [sp, #0x3c]
003a6014: bl       #0x339090
003a6018: b        #0x3a5b48
003a601c: mov      r0, r4
003a6020: mov      r1, r7
003a6024: bl       #0x3a5ae4
003a6028: b        #0x3a5bf8
003a602c: ldr      r3, [pc, #0x17c]
003a6030: mov      r0, r8
003a6034: mov      r1, #0x19
003a6038: ldr      r3, [r5, r3]
003a603c: mov      r2, r6
003a6040: ldr      r4, [r3]
003a6044: bl       #0x3df6e0
003a6048: cmp      r0, #0xa
003a604c: beq      #0x3a60e0
003a6050: mov      r0, r8
003a6054: mov      r1, #0x19
003a6058: mov      r2, r6
003a605c: bl       #0x3df6e0
003a6060: cmp      r0, #0x32
003a6064: beq      #0x3a6134
003a6068: mov      r0, r8
003a606c: mov      r2, r6
003a6070: mov      r1, #0x19
003a6074: bl       #0x3df6e0
003a6078: cmp      r0, #0x64
003a607c: bne      #0x3a5bb8
003a6080: ldr      r0, [pc, #0x160]
003a6084: add      r0, pc, r0
003a6088: bl       #0x3a3f70
003a608c: mov      r1, r0
003a6090: mov      r0, r4
003a6094: bl       #0x3813b8
003a6098: b        #0x3a5bb8
003a609c: cmp      r6, #0
003a60a0: bne      #0x3a5b48
003a60a4: b        #0x3a5e1c
003a60a8: ldr      r0, [sp, #0x20]
003a60ac: bl       #0x3a3f70
003a60b0: mov      r1, r0
003a60b4: mov      r0, r7
003a60b8: bl       #0x3813b8
003a60bc: add      r6, r6, #1
003a60c0: b        #0x3a5c5c
003a60c4: add      r6, sp, #0x9c
003a60c8: mov      r0, r6
003a60cc: ldr      r1, [r4, r8]
003a60d0: bl       #0x33dd2c
003a60d4: mov      r0, r6
003a60d8: bl       #0x33ff54
003a60dc: b        #0x3a5e14
003a60e0: ldr      r0, [pc, #0x104]
003a60e4: add      r0, pc, r0
003a60e8: bl       #0x3a3f70
003a60ec: mov      r1, r0
003a60f0: mov      r0, r4
003a60f4: bl       #0x3813b8
003a60f8: b        #0x3a5bb8
003a60fc: ldr      r0, [sp, #0x18]
003a6100: bl       #0x3a3f70
003a6104: mov      r1, r0
003a6108: mov      r0, r7
003a610c: bl       #0x3813b8
003a6110: add      r6, r6, #1
003a6114: b        #0x3a5c5c
003a6118: ldr      r0, [sp, #0x1c]
003a611c: bl       #0x3a3f70
003a6120: mov      r1, r0
003a6124: mov      r0, r7
003a6128: bl       #0x3813b8
003a612c: add      r6, r6, #1
003a6130: b        #0x3a5c5c
003a6134: ldr      r0, [pc, #0xb4]
003a6138: add      r0, pc, r0
003a613c: bl       #0x3a3f70
003a6140: mov      r1, r0
003a6144: mov      r0, r4
003a6148: bl       #0x3813b8
003a614c: b        #0x3a5bb8
003a6150: ldr      r3, [pc, #0x9c]
003a6154: ldr      r3, [r5, r3]
003a6158: ldr      r3, [r3]
003a615c: cmp      r3, #2
003a6160: streq    r0, [r0]
003a6164: beq      #0x3a5e58
003a6168: cmp      r3, #1
003a616c: bne      #0x3a5e58
003a6170: ldr      r0, [pc, #0x80]
003a6174: ldr      r1, [pc, #0x80]
003a6178: ldr      r2, [pc, #0x80]
003a617c: ldr      r0, [r5, r0]
003a6180: ldr      r3, [pc, #0x7c]
003a6184: movw     ip, #0x2d3
003a6188: add      r1, pc, r1
003a618c: add      r2, pc, r2
003a6190: add      r3, pc, r3
003a6194: add      r0, r0, #0xa8
003a6198: str      ip, [sp]
003a619c: bl       #0x30e004
003a61a0: b        #0x3a5e58
003a61a4: subseq   lr, lr, r0, asr pc
003a61a8: strdeq   r3, r4, [r0], -r4
003a61ac: subseq   sp, r1, ip, lsr r7
003a61b0: andeq    r1, r0, r0, ror sp
003a61b4: subseq   sp, r1, r8, lsl r7
003a61b8: ldrsheq  sp, [r1], #-0x68
003a61bc: ldrsbeq  sp, [r1], #-0x6c
003a61c0: ldrsheq  ip, [r1], #-0xac
003a61c4: ldrsheq  sp, [r1], #-0x4c
003a61c8: ldrdeq   r3, r4, [r0], -r4
003a61cc: strheq   r0, [r0], -r0
003a61d0: subseq   sp, r1, ip, lsl #9
003a61d4: strheq   r4, [r0], -r4
003a61d8: subseq   sp, r1, r4, lsr #8
003a61dc: andeq    r2, r0, r0, ror #29
003a61e0: ldrsbeq  sp, [r1], #-0x38
003a61e4: andeq    r4, r0, r4, lsr #11

# _ZNK9Character27GetEffectiveThreatPerDamageEv
003bd394: add      r1, r0, #0xff0
003bd398: push     {r4, lr}
003bd39c: add      r1, r1, #4
003bd3a0: mov      r2, #0xcc
003bd3a4: add      r0, r0, #0x560
003bd3a8: bl       #0x3dedb4
003bd3ac: bl       #0x30e964
003bd3b0: mov      r1, #0x3b800000
003bd3b4: bl       #0x30ed6c
003bd3b8: pop      {r4, pc}

# _ZN9Character7RegenHPEi
003bdca4: push     {r4, r5, r6, r7, r8, sl, lr}
003bdca8: ldr      r5, [pc, #0xd0]
003bdcac: ldr      r8, [pc, #0xd0]
003bdcb0: add      r6, r0, #0xff0
003bdcb4: add      r5, pc, r5
003bdcb8: ldr      r3, [r5, r8]
003bdcbc: add      r6, r6, #4
003bdcc0: add      r7, r0, #0x560
003bdcc4: ldr      r3, [r3]
003bdcc8: mov      r4, r1
003bdccc: sub      sp, sp, #0x24
003bdcd0: mov      r2, #0x24
003bdcd4: mov      r1, r6
003bdcd8: mov      r0, r7
003bdcdc: str      r3, [sp, #0x1c]
003bdce0: bl       #0x3dedb4
003bdce4: mov      sl, r0
003bdce8: mov      r1, r6
003bdcec: mov      r0, r7
003bdcf0: mov      r2, #0x26
003bdcf4: bl       #0x3dedb4
003bdcf8: cmp      r4, #0
003bdcfc: movlt    r4, r0
003bdd00: add      r3, r4, sl
003bdd04: cmp      r3, r0
003bdd08: rsbgt    r4, sl, r0
003bdd0c: cmp      r4, #0
003bdd10: ble      #0x3bdd60
003bdd14: ldr      r3, [pc, #0x6c]
003bdd18: add      r6, sp, #4
003bdd1c: ldr      sl, [r5, r3]
003bdd20: mov      r0, sl
003bdd24: bl       #0x337888
003bdd28: ldr      r1, [pc, #0x5c]
003bdd2c: mov      r2, sp
003bdd30: mov      r0, r6
003bdd34: add      r1, pc, r1
003bdd38: bl       #0x3140ec
003bdd3c: mov      r1, r6
003bdd40: mov      r0, sl
003bdd44: bl       #0x337a88
003bdd48: mov      r0, r6
003bdd4c: bl       #0x318254
003bdd50: mov      r0, r7
003bdd54: mov      r2, r4
003bdd58: mov      r1, #0x24
003bdd5c: bl       #0x3e0708
003bdd60: ldr      r3, [r5, r8]
003bdd64: ldr      r2, [sp, #0x1c]
003bdd68: ldr      r3, [r3]
003bdd6c: cmp      r2, r3
003bdd70: bne      #0x3bdd7c
003bdd74: add      sp, sp, #0x24
003bdd78: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdd7c: bl       #0x30e310
003bdd80: ldrsbeq  r6, [sp], #-0xdc
003bdd84: andeq    r4, r0, ip, lsr #1
003bdd88: andeq    r0, r0, r4, lsl #17
003bdd8c: subseq   r6, r0, ip, asr fp
