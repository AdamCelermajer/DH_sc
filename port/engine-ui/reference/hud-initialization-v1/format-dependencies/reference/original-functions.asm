
# _ZN13StringManager7parseExERSsPKcRK7VarArgs
00509aec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00509af0: ldr      r6, [pc, #0x930]
00509af4: ldr      sl, [pc, #0x930]
00509af8: subs     r4, r2, #0
00509afc: add      r6, pc, r6
00509b00: ldr      r2, [r6, sl]
00509b04: sub      sp, sp, #0x9c
00509b08: str      r0, [sp, #0x10]
00509b0c: ldr      r2, [r2]
00509b10: mov      r8, r1
00509b14: mov      sb, r3
00509b18: str      r2, [sp, #0x94]
00509b1c: beq      #0x509b2c
00509b20: ldrsb    r3, [r4]
00509b24: cmp      r3, #0
00509b28: bne      #0x509b50
00509b2c: mov      fp, #0
00509b30: ldr      r3, [r6, sl]
00509b34: ldr      r2, [sp, #0x94]
00509b38: mov      r0, fp
00509b3c: ldr      r3, [r3]
00509b40: cmp      r2, r3
00509b44: bne      #0x50a424
00509b48: add      sp, sp, #0x9c
00509b4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00509b50: ldr      r2, [pc, #0x8d8]
00509b54: ldr      r5, [pc, #0x8d8]
00509b58: ldr      r7, [r6, r2]
00509b5c: str      r2, [sp, #0x14]
00509b60: ldr      r2, [pc, #0x8d0]
00509b64: add      r5, pc, r5
00509b68: mov      r1, r5
00509b6c: add      r2, pc, r2
00509b70: ldr      r0, [r7, #0x2c]
00509b74: bl       #0x4c4bdc
00509b78: mov      r1, r0
00509b7c: ldr      r0, [sp, #0x10]
00509b80: bl       #0x508edc
00509b84: ldr      r2, [pc, #0x8b0]
00509b88: str      r0, [sp, #0x2c]
00509b8c: mov      r1, r5
00509b90: add      r2, pc, r2
00509b94: ldr      r0, [r7, #0x2c]
00509b98: bl       #0x4c4bdc
00509b9c: mov      r1, r0
00509ba0: ldr      r0, [sp, #0x10]
00509ba4: bl       #0x508edc
00509ba8: ldr      r2, [pc, #0x890]
00509bac: str      r0, [sp, #0x28]
00509bb0: mov      r1, r5
00509bb4: add      r2, pc, r2
00509bb8: ldr      r0, [r7, #0x2c]
00509bbc: bl       #0x4c4bdc
00509bc0: mov      r1, r0
00509bc4: ldr      r0, [sp, #0x10]
00509bc8: bl       #0x508edc
00509bcc: bl       #0x30e094
00509bd0: str      r0, [sp, #0x20]
00509bd4: ldrb     r3, [r4]
00509bd8: cmp      r3, #0
00509bdc: moveq    fp, r3
00509be0: beq      #0x509e14
00509be4: ldr      r2, [pc, #0x858]
00509be8: ldr      r7, [pc, #0x858]
00509bec: mov      fp, #0
00509bf0: add      r2, pc, r2
00509bf4: str      r2, [sp, #0x24]
00509bf8: ldr      r2, [pc, #0x84c]
00509bfc: str      r7, [sp, #0x34]
00509c00: add      r4, r4, #1
00509c04: add      r2, pc, r2
00509c08: str      r2, [sp, #0x3c]
00509c0c: ldr      r2, [pc, #0x83c]
00509c10: mov      r7, fp
00509c14: mov      r5, fp
00509c18: add      r2, pc, r2
00509c1c: str      r2, [sp, #0x38]
00509c20: ldr      r2, [pc, #0x82c]
00509c24: str      sl, [sp, #0x1c]
00509c28: add      r2, pc, r2
00509c2c: str      r2, [sp, #0x30]
00509c30: b        #0x509c64
00509c34: sxtb     r3, r3
00509c38: cmp      r3, #0x5e
00509c3c: moveq    r7, #1
00509c40: beq      #0x509c58
00509c44: cmp      r3, #0x7c
00509c48: beq      #0x50a1d8
00509c4c: mov      r0, r8
00509c50: mov      r2, r4
00509c54: bl       #0x310804
00509c58: ldrb     r3, [r4], #1
00509c5c: cmp      r3, #0
00509c60: beq      #0x509e10
00509c64: cmp      r7, #0
00509c68: sub      r1, r4, #1
00509c6c: beq      #0x509c34
00509c70: sxtb     r2, r3
00509c74: sub      r3, r2, #0x23
00509c78: cmp      r3, #0x53
00509c7c: addls    pc, pc, r3, lsl #2
00509c80: b        #0x509e00
00509c84: b        #0x50a1c4
00509c88: b        #0x509e00
00509c8c: b        #0x509e00
00509c90: b        #0x509e00
00509c94: b        #0x509e00
00509c98: b        #0x509e00
00509c9c: b        #0x509e00
00509ca0: b        #0x50a1c4
00509ca4: b        #0x509e00
00509ca8: b        #0x509e00
00509cac: b        #0x509e00
00509cb0: b        #0x509e00
00509cb4: b        #0x509e00
00509cb8: b        #0x509e00
00509cbc: b        #0x509e00
00509cc0: b        #0x509e00
00509cc4: b        #0x509e00
00509cc8: b        #0x509e00
00509ccc: b        #0x509e00
00509cd0: b        #0x509e00
00509cd4: b        #0x509e00
00509cd8: b        #0x509e00
00509cdc: b        #0x509e00
00509ce0: b        #0x509e00
00509ce4: b        #0x509e00
00509ce8: b        #0x509e00
00509cec: b        #0x509e00
00509cf0: b        #0x509e00
00509cf4: b        #0x509e00
00509cf8: b        #0x509e00
00509cfc: b        #0x509e00
00509d00: b        #0x509e00
00509d04: b        #0x509e00
00509d08: b        #0x509e00
00509d0c: b        #0x509e00
00509d10: b        #0x509e00
00509d14: b        #0x509e00
00509d18: b        #0x509e00
00509d1c: b        #0x509e00
00509d20: b        #0x509e00
00509d24: b        #0x509e00
00509d28: b        #0x509e00
00509d2c: b        #0x509e00
00509d30: b        #0x509e00
00509d34: b        #0x509e00
00509d38: b        #0x509e00
00509d3c: b        #0x509e00
00509d40: b        #0x509e00
00509d44: b        #0x509e00
00509d48: b        #0x509e00
00509d4c: b        #0x509e00
00509d50: b        #0x509e00
00509d54: b        #0x509e00
00509d58: b        #0x509e00
00509d5c: b        #0x509e00
00509d60: b        #0x509e00
00509d64: b        #0x509e00
00509d68: b        #0x509e00
00509d6c: b        #0x509e00
00509d70: b        #0x50a1c4
00509d74: b        #0x509e00
00509d78: b        #0x509e00
00509d7c: b        #0x509e00
00509d80: b        #0x509e00
00509d84: b        #0x509e00
00509d88: b        #0x50a0e8
00509d8c: b        #0x509e00
00509d90: b        #0x509f28
00509d94: b        #0x509f28
00509d98: b        #0x509f28
00509d9c: b        #0x509f28
00509da0: b        #0x509e00
00509da4: b        #0x50a0e8
00509da8: b        #0x509e00
00509dac: b        #0x509f28
00509db0: b        #0x509dd4
00509db4: b        #0x509e00
00509db8: b        #0x50a0e8
00509dbc: b        #0x509e00
00509dc0: b        #0x509e00
00509dc4: b        #0x509ed4
00509dc8: b        #0x509eb8
00509dcc: b        #0x509e00
00509dd0: b        #0x509e7c
00509dd4: add      r7, sp, #0x68
00509dd8: mov      r1, #0x20
00509ddc: ldr      r2, [sp, #0x24]
00509de0: mov      r0, r7
00509de4: bl       #0x30e244
00509de8: mov      r0, r7
00509dec: bl       #0x30de54
00509df0: mov      r1, r7
00509df4: add      r2, r7, r0
00509df8: mov      r0, r8
00509dfc: bl       #0x310804
00509e00: ldrb     r3, [r4], #1
00509e04: mov      r7, #0
00509e08: cmp      r3, #0
00509e0c: bne      #0x509c64
00509e10: ldr      sl, [sp, #0x1c]
00509e14: ldr      r3, [r8, #0x14]
00509e18: ldr      r0, [r8, #0x10]
00509e1c: mov      r1, #0
00509e20: rsb      r0, r3, r0
00509e24: add      r0, r0, #0x80
00509e28: bl       #0x31056c
00509e2c: mov      r4, r0
00509e30: ldr      r0, [sp, #0x10]
00509e34: ldr      r5, [r8, #0x14]
00509e38: bl       #0x50750c
00509e3c: mov      r1, r4
00509e40: mov      r3, r0
00509e44: mvn      r2, #0
00509e48: mov      r0, r5
00509e4c: bl       #0x752a18
00509e50: mov      r0, r4
00509e54: bl       #0x30de54
00509e58: mov      r1, r4
00509e5c: add      r2, r4, r0
00509e60: mov      r0, r8
00509e64: bl       #0x3109e0
00509e68: cmp      r4, #0
00509e6c: beq      #0x509b30
00509e70: mov      r0, r4
00509e74: bl       #0x310440
00509e78: b        #0x509b30
00509e7c: ldr      ip, [sp, #0x14]
00509e80: add      r7, sp, #0x88
00509e84: mov      r1, r7
00509e88: mov      r2, #0xa
00509e8c: mov      r3, #1
00509e90: ldr      r0, [r6, ip]
00509e94: bl       #0x31f6b0
00509e98: mov      r0, r7
00509e9c: bl       #0x30de54
00509ea0: mov      r1, r7
00509ea4: add      r2, r7, r0
00509ea8: mov      r0, r8
00509eac: bl       #0x310804
00509eb0: mov      r7, #0
00509eb4: b        #0x509c58
00509eb8: ldr      lr, [sp, #0x14]
00509ebc: add      r7, sp, #0x48
00509ec0: mov      r1, r7
00509ec4: mov      r2, #0x20
00509ec8: ldr      r0, [r6, lr]
00509ecc: bl       #0x3206d0
00509ed0: b        #0x509e98
00509ed4: ldmib    sb, {r1, r3}
00509ed8: rsb      r3, r1, r3
00509edc: asr      r3, r3, #2
00509ee0: add      r2, r3, r3, lsl #2
00509ee4: add      r2, r2, r2, lsl #4
00509ee8: add      r2, r2, r2, lsl #8
00509eec: add      r2, r2, r2, lsl #16
00509ef0: add      r2, r3, r2, lsl #1
00509ef4: cmp      r2, r5
00509ef8: bls      #0x509e00
00509efc: mov      r3, #0xc
00509f00: mla      r1, r3, r5, r1
00509f04: add      r5, r5, #1
00509f08: ldr      r1, [r1, #8]
00509f0c: cmp      r1, #0
00509f10: moveq    r7, r1
00509f14: beq      #0x509c58
00509f18: mov      r0, r8
00509f1c: bl       #0x3f1b80
00509f20: mov      r7, #0
00509f24: b        #0x509c58
00509f28: ldr      r3, [sb, #4]
00509f2c: ldr      r1, [sb, #8]
00509f30: rsb      r1, r3, r1
00509f34: asr      r1, r1, #2
00509f38: add      r0, r1, r1, lsl #2
00509f3c: add      r0, r0, r0, lsl #4
00509f40: add      r0, r0, r0, lsl #8
00509f44: add      r0, r0, r0, lsl #16
00509f48: add      r0, r1, r0, lsl #1
00509f4c: cmp      r0, r5
00509f50: bls      #0x509e00
00509f54: cmp      r2, #0x66
00509f58: beq      #0x50a300
00509f5c: cmp      r2, #0x68
00509f60: beq      #0x50a3a0
00509f64: cmp      r2, #0x69
00509f68: beq      #0x50a234
00509f6c: cmp      r2, #0x67
00509f70: beq      #0x50a3e0
00509f74: cmp      r2, #0x6d
00509f78: beq      #0x50a274
00509f7c: add      r5, r5, #1
00509f80: mov      r1, #0
00509f84: ldr      r0, [sp, #0x18]
00509f88: bl       #0x30e70c
00509f8c: cmp      r0, #0
00509f90: movweq   r1, #0xd70a
00509f94: movwne   r1, #0xd70a
00509f98: ldr      r0, [sp, #0x18]
00509f9c: movteq   r1, #0x3ba3
00509fa0: movtne   r1, #0xbba3
00509fa4: bl       #0x30eba4
00509fa8: str      r0, [sp, #0x18]
00509fac: add      r1, sp, #0x44
00509fb0: ldr      r0, [sp, #0x18]
00509fb4: bl       #0x30ea30
00509fb8: mov      r1, #0
00509fbc: mov      r7, r0
00509fc0: ldr      r0, [sp, #0x18]
00509fc4: bl       #0x30e70c
00509fc8: cmp      r0, #0
00509fcc: movweq   r1, #0xd70a
00509fd0: movwne   r1, #0xd70a
00509fd4: movteq   r1, #0x3ba3
00509fd8: movtne   r1, #0xbba3
00509fdc: mov      r0, r7
00509fe0: bl       #0x30e3ac
00509fe4: mov      sl, r0
00509fe8: ldr      r0, [sp, #0x44]
00509fec: bl       #0x30e4cc
00509ff0: ldr      r2, [sp, #0x20]
00509ff4: cmp      r2, r0
00509ff8: bgt      #0x50a2c0
00509ffc: movw     r3, #0xde83
0050a000: movt     r3, #0x431b
0050a004: smull    r7, r3, r3, r0
0050a008: asr      r1, r0, #0x1f
0050a00c: mov      r2, #0xf4000
0050a010: rsb      r3, r1, r3, asr #18
0050a014: add      r2, r2, #0x240
0050a018: mls      r2, r2, r3, r0
0050a01c: movw     lr, #0x4dd3
0050a020: movt     lr, #0x1062
0050a024: smull    r7, ip, lr, r0
0050a028: smull    r7, lr, lr, r2
0050a02c: rsb      ip, r1, ip, asr #6
0050a030: asr      r2, r2, #0x1f
0050a034: mov      r1, #0x3e8
0050a038: cmp      r3, #0
0050a03c: mls      ip, r1, ip, r0
0050a040: rsb      lr, r2, lr, asr #6
0050a044: bne      #0x50a348
0050a048: cmp      lr, #0
0050a04c: beq      #0x50a32c
0050a050: mov      r3, lr
0050a054: ldr      lr, [sp, #0x34]
0050a058: str      ip, [sp, #4]
0050a05c: ldr      ip, [sp, #0x28]
0050a060: add      r7, sp, #0x68
0050a064: mov      r0, r7
0050a068: mov      r1, #0x20
0050a06c: add      r2, pc, lr
0050a070: str      ip, [sp]
0050a074: bl       #0x30e244
0050a078: mov      r0, r7
0050a07c: bl       #0x30de54
0050a080: mov      r1, r7
0050a084: add      r2, r7, r0
0050a088: mov      r0, r8
0050a08c: bl       #0x310804
0050a090: bic      sl, sl, #0x80000000
0050a094: movw     r1, #0xb717
0050a098: mov      r0, sl
0050a09c: movt     r1, #0x38d1
0050a0a0: bl       #0x30e70c
0050a0a4: cmp      r0, #0
0050a0a8: bne      #0x509e00
0050a0ac: mov      r0, r8
0050a0b0: ldr      r1, [sp, #0x2c]
0050a0b4: bl       #0x3f1b80
0050a0b8: ldrsb    r3, [r4, #-1]
0050a0bc: cmp      r3, #0x6d
0050a0c0: beq      #0x50a400
0050a0c4: mov      r0, sl
0050a0c8: bl       #0x30e8a4
0050a0cc: ldr      r2, [sp, #0x38]
0050a0d0: strd     r0, r1, [sp]
0050a0d4: mov      r1, #0x10
0050a0d8: mov      r0, r7
0050a0dc: bl       #0x30e244
0050a0e0: add      r1, r7, #2
0050a0e4: b        #0x509f18
0050a0e8: ldr      r3, [sb, #4]
0050a0ec: ldr      r1, [sb, #8]
0050a0f0: rsb      r1, r3, r1
0050a0f4: asr      r1, r1, #2
0050a0f8: add      r0, r1, r1, lsl #2
0050a0fc: add      r0, r0, r0, lsl #4
0050a100: add      r0, r0, r0, lsl #8
0050a104: add      r0, r0, r0, lsl #16
0050a108: add      r0, r1, r0, lsl #1
0050a10c: cmp      r0, r5
0050a110: bls      #0x509e00
0050a114: cmp      r2, #0x64
0050a118: beq      #0x50a318
0050a11c: cmp      r2, #0x6b
0050a120: beq      #0x50a3c0
0050a124: cmp      r2, #0x70
0050a128: beq      #0x50a254
0050a12c: ldr      r0, [sp, #0x18]
0050a130: bl       #0x30e4cc
0050a134: ldr      r2, [sp, #0x20]
0050a138: add      r5, r5, #1
0050a13c: cmp      r2, r0
0050a140: bgt      #0x50a2e0
0050a144: movw     r3, #0xde83
0050a148: movt     r3, #0x431b
0050a14c: smull    r7, r3, r3, r0
0050a150: asr      r1, r0, #0x1f
0050a154: mov      r2, #0xf4000
0050a158: rsb      r3, r1, r3, asr #18
0050a15c: add      r2, r2, #0x240
0050a160: mls      r2, r2, r3, r0
0050a164: movw     lr, #0x4dd3
0050a168: movt     lr, #0x1062
0050a16c: smull    r7, ip, lr, r0
0050a170: smull    r7, lr, lr, r2
0050a174: rsb      ip, r1, ip, asr #6
0050a178: asr      r2, r2, #0x1f
0050a17c: mov      r1, #0x3e8
0050a180: cmp      r3, #0
0050a184: mls      ip, r1, ip, r0
0050a188: rsb      lr, r2, lr, asr #6
0050a18c: bne      #0x50a374
0050a190: cmp      lr, #0
0050a194: beq      #0x50a214
0050a198: ldr      r2, [pc, #0x2b8]
0050a19c: mov      r3, lr
0050a1a0: ldr      lr, [sp, #0x28]
0050a1a4: add      r7, sp, #0x68
0050a1a8: add      r2, pc, r2
0050a1ac: mov      r0, r7
0050a1b0: mov      r1, #0x20
0050a1b4: str      ip, [sp, #4]
0050a1b8: str      lr, [sp]
0050a1bc: bl       #0x30e244
0050a1c0: b        #0x509e98
0050a1c4: mov      r0, r8
0050a1c8: mov      r2, r4
0050a1cc: bl       #0x310804
0050a1d0: mov      r7, #0
0050a1d4: b        #0x509c58
0050a1d8: ldr      r2, [pc, #0x27c]
0050a1dc: add      sl, sp, #0x68
0050a1e0: mov      r1, #0x20
0050a1e4: add      r2, pc, r2
0050a1e8: mov      r3, #0x11
0050a1ec: mov      r0, sl
0050a1f0: bl       #0x30e244
0050a1f4: mov      r0, sl
0050a1f8: bl       #0x30de54
0050a1fc: mov      r1, sl
0050a200: add      r2, sl, r0
0050a204: mov      r0, r8
0050a208: bl       #0x310804
0050a20c: mov      fp, #1
0050a210: b        #0x509c58
0050a214: ldr      r2, [pc, #0x244]
0050a218: add      r7, sp, #0x68
0050a21c: mov      r3, ip
0050a220: add      r2, pc, r2
0050a224: mov      r0, r7
0050a228: mov      r1, #0x20
0050a22c: bl       #0x30e244
0050a230: b        #0x509e98
0050a234: mov      r2, #0xc
0050a238: mul      r2, r2, r5
0050a23c: mov      r1, #0x40000000
0050a240: ldr      r0, [r3, r2]
0050a244: add      r1, r1, #0xa00000
0050a248: bl       #0x30ec94
0050a24c: str      r0, [sp, #0x18]
0050a250: b        #0x509f7c
0050a254: mov      r2, #0xc
0050a258: mul      r2, r2, r5
0050a25c: mov      r1, #0x42000000
0050a260: ldr      r0, [r3, r2]
0050a264: add      r1, r1, #0xc80000
0050a268: bl       #0x30ed6c
0050a26c: str      r0, [sp, #0x18]
0050a270: b        #0x50a12c
0050a274: mov      r2, #0xc
0050a278: mul      r2, r2, r5
0050a27c: mov      r1, #0x42000000
0050a280: add      r1, r1, #0xc80000
0050a284: ldr      r0, [r3, r2]
0050a288: bl       #0x30ec94
0050a28c: mov      r1, #0
0050a290: mov      r7, r0
0050a294: bl       #0x30e70c
0050a298: cmp      r0, #0
0050a29c: movwne   r0, #0xcccd
0050a2a0: add      r5, r5, #1
0050a2a4: movtne   r0, #0xbd4c
0050a2a8: movweq   r0, #0xcccd
0050a2ac: movteq   r0, #0x3d4c
0050a2b0: mov      r1, r7
0050a2b4: bl       #0x30eba4
0050a2b8: str      r0, [sp, #0x18]
0050a2bc: b        #0x509fac
0050a2c0: ldr      r2, [pc, #0x19c]
0050a2c4: add      r7, sp, #0x68
0050a2c8: mov      r3, r0
0050a2cc: add      r2, pc, r2
0050a2d0: mov      r0, r7
0050a2d4: mov      r1, #0x20
0050a2d8: bl       #0x30e244
0050a2dc: b        #0x50a078
0050a2e0: ldr      r2, [pc, #0x180]
0050a2e4: add      r7, sp, #0x68
0050a2e8: mov      r3, r0
0050a2ec: add      r2, pc, r2
0050a2f0: mov      r0, r7
0050a2f4: mov      r1, #0x20
0050a2f8: bl       #0x30e244
0050a2fc: b        #0x509e98
0050a300: mov      r2, #0xc
0050a304: mul      r2, r2, r5
0050a308: add      r5, r5, #1
0050a30c: ldr      r2, [r3, r2]
0050a310: str      r2, [sp, #0x18]
0050a314: b        #0x509f80
0050a318: mov      r2, #0xc
0050a31c: mul      r2, r2, r5
0050a320: ldr      r2, [r3, r2]
0050a324: str      r2, [sp, #0x18]
0050a328: b        #0x50a12c
0050a32c: add      r7, sp, #0x68
0050a330: mov      r3, ip
0050a334: mov      r0, r7
0050a338: mov      r1, #0x20
0050a33c: ldr      r2, [sp, #0x30]
0050a340: bl       #0x30e244
0050a344: b        #0x50a078
0050a348: ldr      r2, [pc, #0x11c]
0050a34c: str      ip, [sp, #0xc]
0050a350: ldr      ip, [sp, #0x28]
0050a354: add      r7, sp, #0x68
0050a358: add      r2, pc, r2
0050a35c: mov      r0, r7
0050a360: mov      r1, #0x20
0050a364: stm      sp, {ip, lr}
0050a368: str      ip, [sp, #8]
0050a36c: bl       #0x30e244
0050a370: b        #0x50a078
0050a374: ldr      r2, [pc, #0xf4]
0050a378: str      ip, [sp, #0xc]
0050a37c: ldr      ip, [sp, #0x28]
0050a380: add      r7, sp, #0x68
0050a384: add      r2, pc, r2
0050a388: mov      r0, r7
0050a38c: mov      r1, #0x20
0050a390: stm      sp, {ip, lr}
0050a394: str      ip, [sp, #8]
0050a398: bl       #0x30e244
0050a39c: b        #0x509e98
0050a3a0: mov      r2, #0xc
0050a3a4: mul      r2, r2, r5
0050a3a8: mov      r1, #0x42000000
0050a3ac: ldr      r0, [r3, r2]
0050a3b0: add      r1, r1, #0xc80000
0050a3b4: bl       #0x30ed6c
0050a3b8: str      r0, [sp, #0x18]
0050a3bc: b        #0x509f7c
0050a3c0: mov      r2, #0xc
0050a3c4: mul      r2, r2, r5
0050a3c8: mov      r1, #0x44000000
0050a3cc: ldr      r0, [r3, r2]
0050a3d0: add      r1, r1, #0x7a0000
0050a3d4: bl       #0x30ec94
0050a3d8: str      r0, [sp, #0x18]
0050a3dc: b        #0x50a12c
0050a3e0: mov      r2, #0xc
0050a3e4: mul      r2, r2, r5
0050a3e8: mov      r1, #0x44000000
0050a3ec: ldr      r0, [r3, r2]
0050a3f0: add      r1, r1, #0x7a0000
0050a3f4: bl       #0x30ec94
0050a3f8: str      r0, [sp, #0x18]
0050a3fc: b        #0x509f7c
0050a400: mov      r0, sl
0050a404: bl       #0x30e8a4
0050a408: ldr      r2, [sp, #0x3c]
0050a40c: strd     r0, r1, [sp]
0050a410: mov      r1, #0x10
0050a414: mov      r0, r7
0050a418: bl       #0x30e244
0050a41c: add      r1, r7, #2
0050a420: b        #0x509f18
0050a424: bl       #0x30e310
0050a428: umaaleq  sl, r8, r4, pc
0050a42c: andeq    r4, r0, ip, lsr #1
0050a430: strdeq   r3, r4, [r0], -r4
0050a434: eorseq   r5, fp, r4, asr #1
0050a438: eorseq   r2, sp, r4, lsr r2
0050a43c: eorseq   r2, sp, r0, lsr r2
0050a440: eorseq   r2, sp, ip, lsr #4
0050a444: eorseq   r1, ip, r0, lsl #28
0050a448: eorseq   r1, sp, ip, lsr #27
0050a44c: eorseq   r2, sp, ip, lsr #4
0050a450: eorseq   r2, sp, r0, lsl r2
0050a454: eorseq   r8, fp, r8, lsl #5
0050a458: eorseq   r1, sp, r0, ror ip
0050a45c: eorseq   r1, sp, r4, lsr sl
0050a460: mlaseq   fp, r0, ip, r7
0050a464: eorseq   r7, fp, r4, ror #23
0050a468: eorseq   r7, fp, r4, asr #23
0050a46c: ldrhteq  r1, [sp], -r0
0050a470: eorseq   r1, sp, r4, lsl #21

# _ZNK13StringManager9getStringEi
00508edc: cmp      r1, #0
00508ee0: blt      #0x508eec
00508ee4: ldr      r2, [r0, #4]
00508ee8: b        #0x508e1c
00508eec: mov      r0, #0
00508ef0: bx       lr
