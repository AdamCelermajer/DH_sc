
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
