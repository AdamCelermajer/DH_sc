
# _ZN5Level12_LoadProcessEv
003f6990: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f6994: ldr      r5, [pc, #0xf84]
003f6998: ldr      r3, [pc, #0xf84]
003f699c: ldr      r6, [pc, #0xf84]
003f69a0: add      r5, pc, r5
003f69a4: ldr      r2, [r5, r3]
003f69a8: ldr      r3, [r5, r6]
003f69ac: sub      sp, sp, #0x4b0
003f69b0: ldrb     r2, [r2]
003f69b4: ldr      r3, [r3]
003f69b8: sub      sp, sp, #0xc
003f69bc: cmp      r2, #0
003f69c0: mov      r4, r0
003f69c4: str      r3, [sp, #0x4b4]
003f69c8: bne      #0x3f6aec
003f69cc: ldr      r3, [pc, #0xf58]
003f69d0: add      r7, sp, #0x490
003f69d4: add      r7, r7, #0xc
003f69d8: ldr      r8, [r5, r3]
003f69dc: mov      r0, r8
003f69e0: bl       #0x337888
003f69e4: ldr      r1, [pc, #0xf44]
003f69e8: add      r2, sp, #0x138
003f69ec: mov      r0, r7
003f69f0: add      r1, pc, r1
003f69f4: bl       #0x3140ec
003f69f8: mov      r0, r8
003f69fc: mov      r1, r7
003f6a00: bl       #0x337a88
003f6a04: mov      r8, r0
003f6a08: mov      r0, r7
003f6a0c: bl       #0x3139ac
003f6a10: cmp      r8, #0
003f6a14: bne      #0x3f6ad0
003f6a18: ldr      r0, [pc, #0xf14]
003f6a1c: ldr      r1, [r4, #0x130]
003f6a20: add      r0, pc, r0
003f6a24: bl       #0x324114
003f6a28: ldr      r3, [r4, #0x130]
003f6a2c: cmp      r3, #0x25
003f6a30: addls    pc, pc, r3, lsl #2
003f6a34: b        #0x3f753c
003f6a38: b        #0x3f74c8
003f6a3c: b        #0x3f753c
003f6a40: b        #0x3f745c
003f6a44: b        #0x3f7414
003f6a48: b        #0x3f73c0
003f6a4c: b        #0x3f734c
003f6a50: b        #0x3f72d0
003f6a54: b        #0x3f7204
003f6a58: b        #0x3f7174
003f6a5c: b        #0x3f7128
003f6a60: b        #0x3f70cc
003f6a64: b        #0x3f707c
003f6a68: b        #0x3f7a84
003f6a6c: b        #0x3f7a34
003f6a70: b        #0x3f7880
003f6a74: b        #0x3f7828
003f6a78: b        #0x3f780c
003f6a7c: b        #0x3f77c0
003f6a80: b        #0x3f76c8
003f6a84: b        #0x3f76ac
003f6a88: b        #0x3f7b84
003f6a8c: b        #0x3f7b38
003f6a90: b        #0x3f7aec
003f6a94: b        #0x3f7aa0
003f6a98: b        #0x3f7df4
003f6a9c: b        #0x3f7da4
003f6aa0: b        #0x3f6fb0
003f6aa4: b        #0x3f6e90
003f6aa8: b        #0x3f6e90
003f6aac: b        #0x3f7c3c
003f6ab0: b        #0x3f7be8
003f6ab4: b        #0x3f7bd0
003f6ab8: b        #0x3f7714
003f6abc: b        #0x3f7630
003f6ac0: b        #0x3f6b34
003f6ac4: b        #0x3f753c
003f6ac8: b        #0x3f7558
003f6acc: b        #0x3f7c98
003f6ad0: ldr      r0, [r4, #0x130]
003f6ad4: bl       #0x3ef18c
003f6ad8: mov      r7, r0
003f6adc: bl       #0x42a660
003f6ae0: mov      r1, r7
003f6ae4: bl       #0x42a08c
003f6ae8: b        #0x3f6a18
003f6aec: ldr      r7, [pc, #0xf24]
003f6af0: ldr      r7, [r5, r7]
003f6af4: mov      r0, r7
003f6af8: bl       #0x31f594
003f6afc: cmp      r0, #0
003f6b00: beq      #0x3f69cc
003f6b04: ldr      r0, [r7, #0x40]
003f6b08: mov      r1, #0
003f6b0c: mov      r2, #1
003f6b10: bl       #0x36e478
003f6b14: ldr      r0, [r0, #0x660]
003f6b18: cmp      r0, #0
003f6b1c: beq      #0x3f69cc
003f6b20: bl       #0x3bba70
003f6b24: bl       #0x4364f8
003f6b28: mov      r3, #1
003f6b2c: strb     r3, [r0, #0x12c]
003f6b30: b        #0x3f69cc
003f6b34: bl       #0x330740
003f6b38: ldr      r1, [pc, #0xdf8]
003f6b3c: add      r8, sp, #0x19c
003f6b40: add      r2, sp, #0xbc
003f6b44: mov      sl, r0
003f6b48: add      r1, pc, r1
003f6b4c: mov      r0, r8
003f6b50: ldr      r7, [pc, #0xec0]
003f6b54: bl       #0x3140ec
003f6b58: mov      r1, r8
003f6b5c: mov      r0, sl
003f6b60: bl       #0x337a88
003f6b64: mov      r0, r8
003f6b68: bl       #0x3139ac
003f6b6c: ldr      r3, [r5, r7]
003f6b70: mov      r1, #0
003f6b74: mov      r2, #1
003f6b78: ldr      r0, [r3, #0x40]
003f6b7c: ldr      fp, [r4, #0x128]
003f6b80: bl       #0x36e478
003f6b84: ldr      r8, [r0, #0x660]
003f6b88: cmp      r8, #0
003f6b8c: beq      #0x3f6bb0
003f6b90: mvn      r2, #0
003f6b94: mov      r0, r8
003f6b98: ldr      r1, [r4, #0x3c]
003f6b9c: bl       #0x3bb854
003f6ba0: mov      r0, fp
003f6ba4: mov      r1, r8
003f6ba8: mov      r2, #0
003f6bac: bl       #0x4119c4
003f6bb0: bl       #0x7fd794
003f6bb4: ldrb     r3, [r0, #5]
003f6bb8: cmp      r3, #0
003f6bbc: bne      #0x3f7f54
003f6bc0: mov      r0, fp
003f6bc4: bl       #0x40f45c
003f6bc8: ldr      sl, [r5, r7]
003f6bcc: mov      r2, #0
003f6bd0: mov      r1, #0
003f6bd4: ldr      r3, [sl, #0x10]
003f6bd8: mov      r8, r2
003f6bdc: mov      sb, #1
003f6be0: ldr      r3, [r3, #0x1c]
003f6be4: mov      r0, r3
003f6be8: ldr      r3, [r3]
003f6bec: mov      lr, pc
003f6bf0: ldr      pc, [r3, #0x60]
003f6bf4: mov      r0, fp
003f6bf8: ldr      r3, [fp]
003f6bfc: mov      lr, pc
003f6c00: ldr      pc, [r3, #0x10]
003f6c04: b        #0x3f6c2c
003f6c08: mov      r1, r8
003f6c0c: ldr      r0, [sl, #0x40]
003f6c10: mov      r2, #1
003f6c14: bl       #0x36e478
003f6c18: ldr      r3, [r0, #0x660]
003f6c1c: add      r8, r8, #1
003f6c20: cmp      r3, #0
003f6c24: ldrne    r3, [r3, #0x378]
003f6c28: strbne   sb, [r3, #8]
003f6c2c: ldr      r0, [sl, #0x40]
003f6c30: mov      r1, #1
003f6c34: bl       #0x36ead0
003f6c38: cmp      r8, r0
003f6c3c: blt      #0x3f6c08
003f6c40: ldr      r3, [r4, #0x128]
003f6c44: add      r0, sp, #0x88
003f6c48: sub      r0, r0, #0xc
003f6c4c: ldr      r1, [r3, #8]
003f6c50: bl       #0x597180
003f6c54: ldr      r3, [r4, #0x128]
003f6c58: add      r8, sp, #0x184
003f6c5c: ldr      r0, [r3, #8]
003f6c60: bl       #0x597290
003f6c64: mov      r1, r0
003f6c68: add      r0, sp, #0x78
003f6c6c: sub      r0, r0, #8
003f6c70: bl       #0x597180
003f6c74: ldr      r1, [sp, #0x74]
003f6c78: ldr      r0, [sp, #0x80]
003f6c7c: bl       #0x30e3ac
003f6c80: ldr      r1, [sp, #0x78]
003f6c84: mov      fp, r0
003f6c88: ldr      r0, [sp, #0x84]
003f6c8c: bl       #0x30e3ac
003f6c90: ldr      r1, [sp, #0x70]
003f6c94: mov      sb, r0
003f6c98: ldr      r0, [sp, #0x7c]
003f6c9c: bl       #0x30e3ac
003f6ca0: str      fp, [r4, #0x1a0]
003f6ca4: str      r0, [r4, #0x19c]
003f6ca8: str      sb, [r4, #0x1a4]
003f6cac: mov      r0, r4
003f6cb0: ldr      sb, [sl, #0x58]
003f6cb4: bl       #0x3ef4a8
003f6cb8: mov      r2, r0
003f6cbc: ldr      r3, [r2, #0x1e4]
003f6cc0: ldr      r2, [r2, #0x1e8]
003f6cc4: ldr      r0, [r0, #0x1e0]
003f6cc8: str      r3, [sp, #0x10]
003f6ccc: str      r2, [sp, #0x14]
003f6cd0: bl       #0x8be2a0
003f6cd4: ldr      r3, [sp, #0x10]
003f6cd8: uxtb     fp, r0
003f6cdc: mov      r0, r3
003f6ce0: bl       #0x8be2a0
003f6ce4: ldr      r2, [sp, #0x14]
003f6ce8: uxtb     r3, r0
003f6cec: str      r3, [sp, #0x10]
003f6cf0: mov      r0, r2
003f6cf4: bl       #0x8be2a0
003f6cf8: mvn      r2, #0
003f6cfc: strb     r2, [sb, #0xf7]
003f6d00: strb     r0, [sb, #0xf6]
003f6d04: ldr      r3, [sp, #0x10]
003f6d08: mov      r0, r4
003f6d0c: strb     fp, [sb, #0xf4]
003f6d10: strb     r3, [sb, #0xf5]
003f6d14: bl       #0x3ef4a8
003f6d18: ldr      r0, [r0, #0x1d8]
003f6d1c: bl       #0x30e964
003f6d20: mov      fp, r0
003f6d24: mov      r0, r4
003f6d28: bl       #0x3ef4a8
003f6d2c: ldr      r0, [r0, #0x1dc]
003f6d30: bl       #0x30e964
003f6d34: str      fp, [sb, #0x110]
003f6d38: str      r0, [sb, #0x114]
003f6d3c: mov      r0, r4
003f6d40: bl       #0x3ef4a8
003f6d44: ldr      r1, [r0, #0x1f8]
003f6d48: ldr      r2, [r0, #0x1fc]
003f6d4c: ldr      r3, [r0, #0x200]
003f6d50: str      r1, [sb, #0x11c]
003f6d54: str      r2, [sb, #0x120]
003f6d58: str      r3, [sb, #0x124]
003f6d5c: ldr      r3, [sl, #0x10]
003f6d60: mov      r0, r4
003f6d64: ldr      sb, [r3, #0x1c]
003f6d68: bl       #0x3ef4a8
003f6d6c: ldr      r1, [r0, #0x1f8]
003f6d70: ldr      r2, [r0, #0x1fc]
003f6d74: ldr      r3, [r0, #0x200]
003f6d78: str      r1, [sb, #0x458]
003f6d7c: str      r2, [sb, #0x45c]
003f6d80: str      r3, [sb, #0x460]
003f6d84: ldr      r3, [sl, #0x10]
003f6d88: ldr      fp, [r3, #0x1c]
003f6d8c: bl       #0x330740
003f6d90: ldr      r1, [pc, #0xba4]
003f6d94: add      r2, sp, #0xb8
003f6d98: mov      sb, r0
003f6d9c: add      r1, pc, r1
003f6da0: mov      r0, r8
003f6da4: bl       #0x3140ec
003f6da8: mov      r1, r8
003f6dac: mov      r0, sb
003f6db0: bl       #0x337a88
003f6db4: eor      r0, r0, #1
003f6db8: strb     r0, [fp, #0x430]
003f6dbc: mov      r0, r8
003f6dc0: bl       #0x3139ac
003f6dc4: ldr      r0, [sl, #0x38]
003f6dc8: bl       #0x345954
003f6dcc: mov      r0, r4
003f6dd0: mov      r1, #0
003f6dd4: bl       #0x3f2304
003f6dd8: mov      r0, r4
003f6ddc: mov      r1, #1
003f6de0: mov      r2, #4
003f6de4: bl       #0x3ef280
003f6de8: mov      r0, r4
003f6dec: mov      r1, #0
003f6df0: mov      r2, #1
003f6df4: bl       #0x3ef234
003f6df8: bl       #0x7fd794
003f6dfc: ldrb     r3, [r0, #5]
003f6e00: cmp      r3, #0
003f6e04: bne      #0x3f7f30
003f6e08: ldr      r0, [r4, #0xec]
003f6e0c: cmp      r0, #0
003f6e10: beq      #0x3f7f00
003f6e14: ldr      r1, [r4, #0x114]
003f6e18: ldr      r2, [r4, #0x40]
003f6e1c: ldr      r3, [r4, #0x3c]
003f6e20: bl       #0x463288
003f6e24: cmp      r0, #0
003f6e28: bne      #0x3f7f00
003f6e2c: mov      r0, r4
003f6e30: bl       #0x3ef614
003f6e34: ldr      r3, [r5, r7]
003f6e38: mov      r8, r0
003f6e3c: mov      r1, #0
003f6e40: ldr      r0, [r3, #0x40]
003f6e44: mov      r2, #1
003f6e48: bl       #0x36e478
003f6e4c: cmp      r8, #0
003f6e50: ldr      r3, [r0, #0x660]
003f6e54: addne    r8, r8, #0x160
003f6e58: beq      #0x3f81e8
003f6e5c: ldr      r3, [r8]
003f6e60: add      r1, sp, #0x68
003f6e64: sub      r1, r1, #4
003f6e68: str      r3, [sp, #0x64]
003f6e6c: ldr      r3, [r8, #4]
003f6e70: ldrb     r2, [r4, #0xf5]
003f6e74: mov      r0, r4
003f6e78: str      r3, [sp, #0x68]
003f6e7c: ldr      r3, [r8, #8]
003f6e80: str      r3, [sp, #0x6c]
003f6e84: bl       #0x3f04b4
003f6e88: mov      r3, #0
003f6e8c: strb     r3, [r4, #0xf5]
003f6e90: ldr      r3, [r4, #0x130]
003f6e94: add      r3, r3, #1
003f6e98: str      r3, [r4, #0x130]
003f6e9c: ldr      r3, [r4, #0x130]
003f6ea0: cmp      r3, #0x26
003f6ea4: beq      #0x3f6f64
003f6ea8: ldr      r2, [r4, #0x138]
003f6eac: ldr      r3, [r4, #0x134]
003f6eb0: cmp      r2, r3
003f6eb4: ldrlt    r3, [r4, #0x138]
003f6eb8: ldrge    r3, [r4, #0x134]
003f6ebc: str      r3, [r4, #0x134]
003f6ec0: ldr      r3, [r4, #0x130]
003f6ec4: cmp      r3, #0x24
003f6ec8: beq      #0x3f6f5c
003f6ecc: ldr      r3, [r4, #0x130]
003f6ed0: mov      r1, #0x64
003f6ed4: movw     r2, #0x1af3
003f6ed8: mul      r1, r1, r3
003f6edc: movt     r2, #0x6bca
003f6ee0: smull    r0, r3, r2, r1
003f6ee4: asr      r1, r1, #0x1f
003f6ee8: rsb      r3, r1, r3, asr #4
003f6eec: cmp      r3, #0x63
003f6ef0: bgt      #0x3f6f5c
003f6ef4: str      r3, [r4, #0x30]
003f6ef8: bl       #0x42ca8c
003f6efc: ldr      r1, [pc, #0xa3c]
003f6f00: add      r1, pc, r1
003f6f04: bl       #0x42d1f0
003f6f08: subs     r4, r0, #0
003f6f0c: beq      #0x3f6f3c
003f6f10: add      r0, r4, #0x48
003f6f14: ldr      r7, [r4, #4]
003f6f18: bl       #0x386144
003f6f1c: ldr      r2, [pc, #0xa20]
003f6f20: mov      ip, #0
003f6f24: ldr      r1, [r4, #0x4c]
003f6f28: mov      r0, r7
003f6f2c: add      r2, pc, r2
003f6f30: mov      r3, ip
003f6f34: str      ip, [sp]
003f6f38: bl       #0x7abe0c
003f6f3c: ldr      r3, [r5, r6]
003f6f40: ldr      r2, [sp, #0x4b4]
003f6f44: ldr      r3, [r3]
003f6f48: cmp      r2, r3
003f6f4c: bne      #0x3f82d4
003f6f50: add      sp, sp, #0xbc
003f6f54: add      sp, sp, #0x400
003f6f58: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f6f5c: mov      r3, #0x64
003f6f60: b        #0x3f6ef4
003f6f64: mov      r3, #0x64
003f6f68: str      r3, [r4, #0x30]
003f6f6c: ldr      r3, [pc, #0x9d4]
003f6f70: ldr      r3, [r5, r3]
003f6f74: ldrb     r3, [r3]
003f6f78: cmp      r3, #0
003f6f7c: bne      #0x3f6fa4
003f6f80: bl       #0x42ca8c
003f6f84: ldr      r1, [pc, #0x9c0]
003f6f88: mov      r4, r0
003f6f8c: add      r1, pc, r1
003f6f90: bl       #0x42d1f0
003f6f94: mov      r1, r0
003f6f98: mov      r0, r4
003f6f9c: bl       #0x4317e8
003f6fa0: b        #0x3f6ef8
003f6fa4: mov      r0, #1
003f6fa8: bl       #0x89becc
003f6fac: b        #0x3f6f80
003f6fb0: bl       #0x42ca8c
003f6fb4: mov      r1, #3
003f6fb8: bl       #0x431ea4
003f6fbc: ldr      r3, [pc, #0x98c]
003f6fc0: mov      r1, #0
003f6fc4: mov      r2, r1
003f6fc8: ldr      r7, [r5, r3]
003f6fcc: mov      r3, r1
003f6fd0: mov      r0, r7
003f6fd4: bl       #0x427c44
003f6fd8: ldr      r3, [pc, #0x974]
003f6fdc: mov      r1, #0
003f6fe0: mov      r2, r1
003f6fe4: ldr      r0, [r5, r3]
003f6fe8: mov      r3, r1
003f6fec: bl       #0x427c44
003f6ff0: mov      r1, #0
003f6ff4: mov      r2, r1
003f6ff8: mov      r3, r1
003f6ffc: mov      r0, r7
003f7000: bl       #0x427c44
003f7004: ldr      r3, [pc, #0x94c]
003f7008: mov      r1, #0
003f700c: mov      r2, r1
003f7010: ldr      r0, [r5, r3]
003f7014: mov      r3, r1
003f7018: bl       #0x427c44
003f701c: ldr      r3, [pc, #0x938]
003f7020: mov      r1, #0
003f7024: mov      r2, r1
003f7028: ldr      r0, [r5, r3]
003f702c: mov      r3, r1
003f7030: bl       #0x427c44
003f7034: ldr      r3, [pc, #0x9dc]
003f7038: ldr      r3, [r5, r3]
003f703c: ldr      r0, [r3, #0x40]
003f7040: bl       #0x36f2bc
003f7044: bl       #0x42ca8c
003f7048: bl       #0x42cb8c
003f704c: ldr      r1, [pc, #0x90c]
003f7050: ldr      r2, [pc, #0x90c]
003f7054: mov      ip, #0
003f7058: mov      r3, ip
003f705c: add      r1, pc, r1
003f7060: add      r2, pc, r2
003f7064: str      ip, [sp]
003f7068: bl       #0x7ad7e8
003f706c: ldr      r3, [r4, #0x130]
003f7070: add      r3, r3, #1
003f7074: str      r3, [r4, #0x130]
003f7078: b        #0x3f6e9c
003f707c: bl       #0x330740
003f7080: ldr      r1, [pc, #0x8e0]
003f7084: add      r7, sp, #0x31c
003f7088: add      r2, sp, #0xfc
003f708c: mov      r8, r0
003f7090: add      r1, pc, r1
003f7094: mov      r0, r7
003f7098: bl       #0x3140ec
003f709c: mov      r1, r7
003f70a0: mov      r0, r8
003f70a4: bl       #0x337a88
003f70a8: mov      r0, r7
003f70ac: bl       #0x3139ac
003f70b0: ldr      r3, [pc, #0x978]
003f70b4: ldr      r0, [r5, r3]
003f70b8: bl       #0x5226cc
003f70bc: ldr      r3, [r4, #0x130]
003f70c0: add      r3, r3, #1
003f70c4: str      r3, [r4, #0x130]
003f70c8: b        #0x3f6e9c
003f70cc: bl       #0x330740
003f70d0: ldr      r1, [pc, #0x894]
003f70d4: add      r8, sp, #0x334
003f70d8: add      r2, sp, #0x100
003f70dc: mov      sl, r0
003f70e0: add      r1, pc, r1
003f70e4: mov      r0, r8
003f70e8: ldr      r7, [pc, #0x928]
003f70ec: bl       #0x3140ec
003f70f0: mov      r1, r8
003f70f4: mov      r0, sl
003f70f8: bl       #0x337a88
003f70fc: mov      r0, r8
003f7100: bl       #0x3139ac
003f7104: ldr      r7, [r5, r7]
003f7108: ldr      r3, [r7, #0x38]
003f710c: ldr      r3, [r3, #0x1c]
003f7110: str      r3, [r4, #0x138]
003f7114: ldr      r0, [r7, #0x38]
003f7118: bl       #0x34552c
003f711c: cmp      r0, #0
003f7120: beq      #0x3f7114
003f7124: b        #0x3f6e90
003f7128: bl       #0x330740
003f712c: ldr      r1, [pc, #0x83c]
003f7130: add      r7, sp, #0x34c
003f7134: add      r2, sp, #0x104
003f7138: mov      r8, r0
003f713c: add      r1, pc, r1
003f7140: mov      r0, r7
003f7144: bl       #0x3140ec
003f7148: mov      r1, r7
003f714c: mov      r0, r8
003f7150: bl       #0x337a88
003f7154: mov      r0, r7
003f7158: bl       #0x3139ac
003f715c: mov      r0, r4
003f7160: bl       #0x3f459c
003f7164: ldr      r3, [r4, #0x130]
003f7168: add      r3, r3, #1
003f716c: str      r3, [r4, #0x130]
003f7170: b        #0x3f6e9c
003f7174: bl       #0x330740
003f7178: ldr      r1, [pc, #0x7f4]
003f717c: add      r7, sp, #0x3c4
003f7180: add      r2, sp, #0x118
003f7184: mov      r8, r0
003f7188: add      r1, pc, r1
003f718c: mov      r0, r7
003f7190: bl       #0x3140ec
003f7194: mov      r1, r7
003f7198: mov      r0, r8
003f719c: bl       #0x337a88
003f71a0: mov      r0, r7
003f71a4: bl       #0x3139ac
003f71a8: ldr      r3, [r4, #0x38]
003f71ac: cmp      r3, #0
003f71b0: beq      #0x3f8070
003f71b4: ldr      r1, [r3, #0x204]
003f71b8: ldr      r2, [r3, #0x208]
003f71bc: cmp      r1, r2
003f71c0: beq      #0x3f71ec
003f71c4: ldr      r0, [r3, #0x210]
003f71c8: add      r7, r3, #0x204
003f71cc: ldr      r3, [pc, #0x844]
003f71d0: ldr      r3, [r5, r3]
003f71d4: ldr      r8, [r3, #0x38]
003f71d8: bl       #0x30e4cc
003f71dc: mov      r1, r7
003f71e0: mov      r2, r0
003f71e4: mov      r0, r8
003f71e8: bl       #0x347100
003f71ec: mov      r0, r4
003f71f0: bl       #0x3f3a58
003f71f4: ldr      r3, [r4, #0x130]
003f71f8: add      r3, r3, #1
003f71fc: str      r3, [r4, #0x130]
003f7200: b        #0x3f6e9c
003f7204: ldr      r2, [pc, #0x76c]
003f7208: ldr      r3, [r4, #0xdc]
003f720c: ldr      r7, [r5, r2]
003f7210: ldr      r2, [pc, #0x764]
003f7214: str      r3, [r7]
003f7218: ldr      r8, [r5, r2]
003f721c: ldr      r3, [r4, #0xe0]
003f7220: str      r3, [r8]
003f7224: ldrb     r3, [r4, #0xe8]
003f7228: cmp      r3, #0
003f722c: addeq    r8, r4, #0xf8
003f7230: bne      #0x3f7f7c
003f7234: bl       #0x330740
003f7238: ldr      r1, [pc, #0x740]
003f723c: add      r7, sp, #0x3f4
003f7240: mov      sl, r0
003f7244: add      r2, sp, #0x120
003f7248: add      r1, pc, r1
003f724c: mov      r0, r7
003f7250: bl       #0x3140ec
003f7254: mov      r1, r7
003f7258: mov      r0, sl
003f725c: ldr      sl, [pc, #0x720]
003f7260: bl       #0x337a88
003f7264: mov      r0, r7
003f7268: bl       #0x3139ac
003f726c: mov      r3, #0x1f4
003f7270: str      r3, [r4, #0x138]
003f7274: add      sl, pc, sl
003f7278: add      r7, sp, #0x3dc
003f727c: add      sb, sp, #0x11c
003f7280: mov      r1, sl
003f7284: mov      r2, sb
003f7288: mov      r0, r7
003f728c: bl       #0x3140ec
003f7290: mov      r1, r8
003f7294: mov      r2, r7
003f7298: mov      r0, r4
003f729c: bl       #0x3f3b40
003f72a0: mov      fp, r0
003f72a4: mov      r0, r7
003f72a8: bl       #0x3139ac
003f72ac: cmp      fp, #0
003f72b0: beq      #0x3f7280
003f72b4: ldr      r3, [r4, #0x130]
003f72b8: add      r3, r3, #1
003f72bc: str      r3, [r4, #0x130]
003f72c0: ldr      r3, [r4, #0x13c]
003f72c4: add      r3, r3, #1
003f72c8: str      r3, [r4, #0x13c]
003f72cc: b        #0x3f6e9c
003f72d0: bl       #0x330740
003f72d4: ldr      r1, [pc, #0x6ac]
003f72d8: add      r7, sp, #0x364
003f72dc: add      r2, sp, #0x108
003f72e0: mov      r8, r0
003f72e4: add      r1, pc, r1
003f72e8: mov      r0, r7
003f72ec: bl       #0x3140ec
003f72f0: mov      r1, r7
003f72f4: mov      r0, r8
003f72f8: bl       #0x337a88
003f72fc: mov      r0, r7
003f7300: bl       #0x3139ac
003f7304: ldr      r3, [pc, #0x70c]
003f7308: ldr      r3, [r5, r3]
003f730c: ldr      r0, [r3, #0x40]
003f7310: mov      r3, #1
003f7314: strb     r3, [r0, #0x6c9]
003f7318: bl       #0x378fb4
003f731c: mov      r1, #0
003f7320: mov      r0, #0xc
003f7324: bl       #0x310570
003f7328: mov      r7, r0
003f732c: bl       #0x479d08
003f7330: str      r7, [r4, #0x194]
003f7334: mov      r0, r7
003f7338: bl       #0x479e5c
003f733c: ldr      r3, [r4, #0x130]
003f7340: add      r3, r3, #1
003f7344: str      r3, [r4, #0x130]
003f7348: b        #0x3f6e9c
003f734c: bl       #0x330740
003f7350: ldr      r1, [pc, #0x634]
003f7354: add      r7, sp, #0x420
003f7358: add      r7, r7, #4
003f735c: add      r2, sp, #0x124
003f7360: mov      r8, r0
003f7364: add      r1, pc, r1
003f7368: mov      r0, r7
003f736c: bl       #0x3140ec
003f7370: mov      r1, r7
003f7374: mov      r0, r8
003f7378: bl       #0x337a88
003f737c: mov      r0, r7
003f7380: bl       #0x3139ac
003f7384: ldr      r3, [pc, #0x68c]
003f7388: mov      ip, #0x44000000
003f738c: mov      r1, #0xc4000000
003f7390: ldr      r0, [r5, r3]
003f7394: add      ip, ip, #0xfa0000
003f7398: add      r1, r1, #0xfa0000
003f739c: mov      r3, ip
003f73a0: ldr      r0, [r0, #0x44]
003f73a4: mov      r2, r1
003f73a8: str      ip, [sp]
003f73ac: bl       #0x34c048
003f73b0: ldr      r3, [r4, #0x130]
003f73b4: add      r3, r3, #1
003f73b8: str      r3, [r4, #0x130]
003f73bc: b        #0x3f6e9c
003f73c0: bl       #0x330740
003f73c4: ldr      r1, [pc, #0x5c4]
003f73c8: add      r7, sp, #0x430
003f73cc: add      r7, r7, #0xc
003f73d0: add      r2, sp, #0x128
003f73d4: mov      r8, r0
003f73d8: add      r1, pc, r1
003f73dc: mov      r0, r7
003f73e0: bl       #0x3140ec
003f73e4: mov      r1, r7
003f73e8: mov      r0, r8
003f73ec: bl       #0x337a88
003f73f0: mov      r0, r7
003f73f4: bl       #0x3139ac
003f73f8: ldr      r3, [pc, #0x5e8]
003f73fc: ldr      r0, [r5, r3]
003f7400: bl       #0x496bd8
003f7404: ldr      r3, [r4, #0x130]
003f7408: add      r3, r3, #1
003f740c: str      r3, [r4, #0x130]
003f7410: b        #0x3f6e9c
003f7414: bl       #0x330740
003f7418: ldr      r1, [pc, #0x574]
003f741c: add      r7, sp, #0x460
003f7420: mov      r8, r0
003f7424: add      r7, r7, #0xc
003f7428: add      r2, sp, #0x130
003f742c: add      r1, pc, r1
003f7430: mov      r0, r7
003f7434: bl       #0x3140ec
003f7438: mov      r1, r7
003f743c: mov      r0, r8
003f7440: bl       #0x337a88
003f7444: mov      r0, r7
003f7448: bl       #0x3139ac
003f744c: ldr      r3, [r4, #0x130]
003f7450: add      r3, r3, #1
003f7454: str      r3, [r4, #0x130]
003f7458: b        #0x3f6e9c
003f745c: bl       #0x330740
003f7460: ldr      r1, [pc, #0x530]
003f7464: add      r7, sp, #0x480
003f7468: add      r7, r7, #4
003f746c: add      r2, sp, #0x134
003f7470: mov      r8, r0
003f7474: add      r1, pc, r1
003f7478: mov      r0, r7
003f747c: bl       #0x3140ec
003f7480: mov      r1, r7
003f7484: mov      r0, r8
003f7488: bl       #0x337a88
003f748c: mov      r0, r7
003f7490: bl       #0x3139ac
003f7494: bl       #0x42ca8c
003f7498: mov      r1, #2
003f749c: bl       #0x42d4bc
003f74a0: bl       #0x42ca8c
003f74a4: mov      r1, #1
003f74a8: bl       #0x42d4bc
003f74ac: bl       #0x42ca8c
003f74b0: mov      r1, #3
003f74b4: bl       #0x42d4bc
003f74b8: ldr      r3, [r4, #0x130]
003f74bc: add      r3, r3, #1
003f74c0: str      r3, [r4, #0x130]
003f74c4: b        #0x3f6e9c
003f74c8: ldr      r3, [pc, #0x548]
003f74cc: mov      r7, #0
003f74d0: ldr      r8, [r5, r3]
003f74d4: ldr      r3, [pc, #0x4c0]
003f74d8: mov      r0, r8
003f74dc: ldr      r2, [r5, r3]
003f74e0: ldr      r3, [pc, #0x4b8]
003f74e4: str      r7, [r2]
003f74e8: ldr      r3, [r5, r3]
003f74ec: str      r7, [r3]
003f74f0: bl       #0x31f55c
003f74f4: ldr      r0, [pc, #0x4a8]
003f74f8: add      r0, pc, r0
003f74fc: bl       #0x31041c
003f7500: ldr      r3, [pc, #0x4a0]
003f7504: mov      r1, #0x1f4
003f7508: ldr      r3, [r5, r3]
003f750c: ldr      r0, [r3]
003f7510: bl       #0x369990
003f7514: mov      r3, #1
003f7518: strb     r3, [r8, #0xb4]
003f751c: str      r7, [r4, #0x134]
003f7520: str      r7, [r4, #0x138]
003f7524: str      r7, [r4, #0x13c]
003f7528: ldr      r3, [r4, #0x130]
003f752c: strb     r7, [r4, #0x144]
003f7530: add      r3, r3, #1
003f7534: str      r3, [r4, #0x130]
003f7538: b        #0x3f6e9c
003f753c: ldr      r3, [r4, #0x130]
003f7540: cmp      r3, #0x25
003f7544: ldrle    r3, [r4, #0x130]
003f7548: movgt    r3, #0x26
003f754c: addle    r3, r3, #1
003f7550: str      r3, [r4, #0x130]
003f7554: b        #0x3f6e9c
003f7558: bl       #0x330740
003f755c: ldr      r1, [pc, #0x448]
003f7560: add      r7, sp, #0x16c
003f7564: add      r2, sp, #0xb8
003f7568: sub      r2, r2, #4
003f756c: mov      r8, r0
003f7570: add      r1, pc, r1
003f7574: mov      r0, r7
003f7578: bl       #0x3140ec
003f757c: mov      r1, r7
003f7580: mov      r0, r8
003f7584: bl       #0x337a88
003f7588: mov      r0, r7
003f758c: bl       #0x3139ac
003f7590: bl       #0x7fd794
003f7594: ldrb     r3, [r0, #5]
003f7598: cmp      r3, #0
003f759c: beq      #0x3f6e9c
003f75a0: ldr      r7, [pc, #0x470]
003f75a4: mov      r1, #0
003f75a8: mov      r2, r1
003f75ac: ldr      r8, [r5, r7]
003f75b0: ldr      r0, [r8, #0x40]
003f75b4: bl       #0x36e478
003f75b8: ldrb     r3, [r0, #0x545]
003f75bc: cmp      r3, #0
003f75c0: bne      #0x3f81b4
003f75c4: ldr      r3, [r5, r7]
003f75c8: ldr      r0, [r3, #0x38]
003f75cc: ldrb     r3, [r0, #0x1ac]
003f75d0: cmp      r3, #0
003f75d4: bne      #0x3f75dc
003f75d8: bl       #0x340be0
003f75dc: ldr      r7, [r5, r7]
003f75e0: mov      r1, #0x3f800000
003f75e4: ldr      r0, [r7, #0x38]
003f75e8: bl       #0x34a620
003f75ec: mov      r1, #0
003f75f0: ldr      r0, [r7, #0x40]
003f75f4: mov      r2, r1
003f75f8: bl       #0x36e478
003f75fc: mov      r7, r0
003f7600: bl       #0x80f23c
003f7604: cmp      r0, #0
003f7608: beq      #0x3f6e9c
003f760c: ldr      r3, [r7, #0x660]
003f7610: cmp      r3, #0
003f7614: beq      #0x3f6e9c
003f7618: movw     r2, #0x14e8
003f761c: ldr      r0, [r3, r2]
003f7620: cmp      r0, #0
003f7624: beq      #0x3f6e9c
003f7628: bl       #0x4679e8
003f762c: b        #0x3f6e9c
003f7630: bl       #0x330740
003f7634: ldr      r1, [pc, #0x374]
003f7638: add      r7, sp, #0x1b4
003f763c: add      r2, sp, #0xc0
003f7640: mov      r8, r0
003f7644: add      r1, pc, r1
003f7648: mov      r0, r7
003f764c: bl       #0x3140ec
003f7650: mov      r1, r7
003f7654: mov      r0, r8
003f7658: bl       #0x337a88
003f765c: mov      r0, r7
003f7660: bl       #0x3139ac
003f7664: ldrb     r3, [r4, #0xf1]
003f7668: cmp      r3, #0
003f766c: bne      #0x3f7e40
003f7670: ldr      r7, [pc, #0x3a0]
003f7674: ldr      r3, [r5, r7]
003f7678: mov      r2, #1
003f767c: mov      r1, #0
003f7680: ldr      r0, [r3, #0x40]
003f7684: bl       #0x36e478
003f7688: mov      r1, #0
003f768c: ldr      r0, [r0, #0x660]
003f7690: bl       #0x3bb798
003f7694: ldr      r3, [r4, #0x130]
003f7698: mov      r2, #0
003f769c: strb     r2, [r4, #0xf3]
003f76a0: add      r3, r3, #1
003f76a4: str      r3, [r4, #0x130]
003f76a8: b        #0x3f6e9c
003f76ac: bl       #0x330740
003f76b0: ldr      r1, [pc, #0x2fc]
003f76b4: mov      r8, r0
003f76b8: add      r7, sp, #0x274
003f76bc: add      r2, sp, #0xe0
003f76c0: add      r1, pc, r1
003f76c4: b        #0x3f7430
003f76c8: bl       #0x330740
003f76cc: ldr      r1, [pc, #0x2e4]
003f76d0: add      r7, sp, #0x28c
003f76d4: add      r2, sp, #0xe4
003f76d8: mov      r8, r0
003f76dc: add      r1, pc, r1
003f76e0: mov      r0, r7
003f76e4: bl       #0x3140ec
003f76e8: mov      r1, r7
003f76ec: mov      r0, r8
003f76f0: bl       #0x337a88
003f76f4: mov      r0, r7
003f76f8: bl       #0x3139ac
003f76fc: mov      r0, r4
003f7700: bl       #0x3eff98
003f7704: ldr      r3, [r4, #0x130]
003f7708: add      r3, r3, #1
003f770c: str      r3, [r4, #0x130]
003f7710: b        #0x3f6e9c
003f7714: ldr      r3, [pc, #0x2fc]
003f7718: mov      r0, r4
003f771c: mov      r8, #0x3f800000
003f7720: ldr      r7, [r5, r3]
003f7724: ldr      r3, [r7, #0x10]
003f7728: ldr      fp, [r3, #0x1c]
003f772c: bl       #0x3ef4a8
003f7730: ldr      sb, [r0, #0x1cc]
003f7734: mov      r0, r4
003f7738: bl       #0x3ef4a8
003f773c: ldr      sl, [r0, #0x1d0]
003f7740: mov      r0, r4
003f7744: bl       #0x3ef4a8
003f7748: ldr      r3, [r0, #0x1d4]
003f774c: add      r1, sp, #0x58
003f7750: mov      r0, fp
003f7754: sub      r1, r1, #4
003f7758: str      r3, [sp, #0x5c]
003f775c: str      r8, [sp, #0x60]
003f7760: str      sb, [sp, #0x54]
003f7764: str      sl, [sp, #0x58]
003f7768: bl       #0x589508
003f776c: ldr      r3, [pc, #0x2bc]
003f7770: mov      r1, r8
003f7774: ldr      r0, [r7, #0x38]
003f7778: ldr      r8, [r5, r3]
003f777c: mov      r3, #1
003f7780: strb     r3, [r8, #0x94]
003f7784: bl       #0x34a620
003f7788: bl       #0x3ce9b8
003f778c: ldr      r0, [r7, #0x44]
003f7790: bl       #0x34bd08
003f7794: mov      r3, #0
003f7798: strb     r3, [r8, #0x94]
003f779c: ldr      r3, [r4, #0x128]
003f77a0: mov      r0, r3
003f77a4: ldr      r3, [r3]
003f77a8: mov      lr, pc
003f77ac: ldr      pc, [r3, #0x10]
003f77b0: ldr      r3, [r4, #0x130]
003f77b4: add      r3, r3, #1
003f77b8: str      r3, [r4, #0x130]
003f77bc: b        #0x3f6e9c
003f77c0: bl       #0x330740
003f77c4: ldr      r1, [pc, #0x1f0]
003f77c8: add      r7, sp, #0x2a4
003f77cc: add      r2, sp, #0xe8
003f77d0: mov      r8, r0
003f77d4: add      r1, pc, r1
003f77d8: mov      r0, r7
003f77dc: bl       #0x3140ec
003f77e0: mov      r1, r7
003f77e4: mov      r0, r8
003f77e8: bl       #0x337a88
003f77ec: mov      r0, r7
003f77f0: bl       #0x3139ac
003f77f4: mov      r0, r4
003f77f8: bl       #0x3efca4
003f77fc: ldr      r3, [r4, #0x130]
003f7800: add      r3, r3, #1
003f7804: str      r3, [r4, #0x130]
003f7808: b        #0x3f6e9c
003f780c: bl       #0x330740
003f7810: ldr      r1, [pc, #0x1a8]
003f7814: mov      r8, r0
003f7818: add      r7, sp, #0x2bc
003f781c: add      r2, sp, #0xec
003f7820: add      r1, pc, r1
003f7824: b        #0x3f7430
003f7828: bl       #0x330740
003f782c: ldr      r1, [pc, #0x190]
003f7830: add      r7, sp, #0x2d4
003f7834: add      r2, sp, #0xf0
003f7838: mov      r8, r0
003f783c: add      r1, pc, r1
003f7840: mov      r0, r7
003f7844: bl       #0x3140ec
003f7848: mov      r1, r7
003f784c: mov      r0, r8
003f7850: bl       #0x337a88
003f7854: mov      r0, r7
003f7858: bl       #0x3139ac
003f785c: ldr      r3, [pc, #0x144]
003f7860: ldr      r1, [r4, #0x3c]
003f7864: ldr      r3, [r5, r3]
003f7868: ldr      r0, [r3]
003f786c: bl       #0x3695f4
003f7870: ldr      r3, [r4, #0x130]
003f7874: add      r3, r3, #1
003f7878: str      r3, [r4, #0x130]
003f787c: b        #0x3f6e9c
003f7880: bl       #0x330740
003f7884: ldr      r1, [pc, #0x13c]
003f7888: add      r7, sp, #0x2ec
003f788c: add      r2, sp, #0xf4
003f7890: mov      r8, r0
003f7894: add      r1, pc, r1
003f7898: mov      r0, r7
003f789c: bl       #0x3140ec
003f78a0: mov      r1, r7
003f78a4: mov      r0, r8
003f78a8: bl       #0x337a88
003f78ac: mov      r0, r7
003f78b0: bl       #0x3139ac
003f78b4: mov      r0, r4
003f78b8: bl       #0x3f0018
003f78bc: bl       #0x7fd794
003f78c0: ldrb     r3, [r0, #5]
003f78c4: cmp      r3, #0
003f78c8: beq      #0x3f6e90
003f78cc: ldr      r3, [pc, #0x144]
003f78d0: mov      r1, #0
003f78d4: mov      r2, r1
003f78d8: ldr      r7, [r5, r3]
003f78dc: ldr      r0, [r7, #0x40]
003f78e0: bl       #0x36e478
003f78e4: ldr      r3, [r0]
003f78e8: mov      lr, pc
003f78ec: ldr      pc, [r3, #0x5c]
003f78f0: cmp      r0, #0
003f78f4: bne      #0x3f6e90
003f78f8: bl       #0x800f8c
003f78fc: ldr      r3, [r0]
003f7900: mov      lr, pc
003f7904: ldr      pc, [r3, #0x3c]
003f7908: bl       #0x7fbd74
003f790c: bl       #0x7fbe54
003f7910: mov      r0, r7
003f7914: mov      r1, #3
003f7918: bl       #0x32c1f4
003f791c: b        #0x3f6e90
003f7920: ldrsheq  lr, [sb], #-0
003f7924: strheq   r3, [r0], -r0
003f7928: andeq    r4, r0, ip, lsr #1
003f792c: andeq    r0, r0, r4, lsl #17
003f7930: strheq   r0, [sp], #-8
003f7934: subeq    r0, sp, r8, lsr #1

# _ZN5Level15_LoadCharStatesEv
003eff98: ldr      r3, [pc, #0x70]
003eff9c: ldr      r2, [pc, #0x70]
003effa0: push     {r4, r5, r6, lr}
003effa4: add      r3, pc, r3
003effa8: ldr      r2, [r3, r2]
003effac: ldr      r6, [r2, #0x38]
003effb0: ldr      r4, [r6, #0x60]!
003effb4: cmp      r6, r4
003effb8: beq      #0x3efff8
003effbc: ldr      r5, [r4, #8]
003effc0: subs     r0, r5, #0
003effc4: beq      #0x3effec
003effc8: bl       #0x3a5784
003effcc: subs     r3, r0, #0
003effd0: beq      #0x3efffc
003effd4: add      r0, r5, #0x4f0
003effd8: cmp      r3, #0x11
003effdc: add      r0, r0, #0xc
003effe0: mov      r1, #0
003effe4: beq      #0x3efffc
003effe8: bl       #0x3c1a00
003effec: ldr      r4, [r4]
003efff0: cmp      r6, r4
003efff4: bne      #0x3effbc
003efff8: pop      {r4, r5, r6, pc}
003efffc: add      r0, r5, #0x4f0
003f0000: add      r0, r0, #0xc
003f0004: bl       #0x3c1a64
003f0008: ldr      r4, [r4]
003f000c: b        #0x3efff0
003f0010: subseq   r4, sl, ip, ror #21
003f0014: strdeq   r3, r4, [r0], -r4

# _ZN9Character8InitPostEv
003b4d60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4d64: ldr      r5, [pc, #0x828]
003b4d68: ldr      r6, [pc, #0x828]
003b4d6c: movw     r3, #0x1394
003b4d70: add      r5, pc, r5
003b4d74: ldr      r2, [r5, r6]
003b4d78: ldrb     r7, [r0, r3]
003b4d7c: sub      sp, sp, #0xfc
003b4d80: ldr      r2, [r2]
003b4d84: cmp      r7, #0
003b4d88: mov      r4, r0
003b4d8c: str      r2, [sp, #0xf4]
003b4d90: beq      #0x3b4db0
003b4d94: ldr      r3, [r5, r6]
003b4d98: ldr      r2, [sp, #0xf4]
003b4d9c: ldr      r3, [r3]
003b4da0: cmp      r2, r3
003b4da4: bne      #0x3b5590
003b4da8: add      sp, sp, #0xfc
003b4dac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4db0: mov      r2, #1
003b4db4: strb     r2, [r0, r3]
003b4db8: bl       #0x38bd64
003b4dbc: ldr      r3, [r4, #0x274]
003b4dc0: cmp      r0, r3
003b4dc4: bge      #0x3b4d94
003b4dc8: ldr      fp, [pc, #0x7cc]
003b4dcc: add      r8, sp, #0xdc
003b4dd0: ldr      sl, [r5, fp]
003b4dd4: mov      r0, sl
003b4dd8: bl       #0x337888
003b4ddc: ldr      r1, [pc, #0x7bc]
003b4de0: add      r2, sp, #0x48
003b4de4: mov      r0, r8
003b4de8: add      r1, pc, r1
003b4dec: bl       #0x3140ec
003b4df0: mov      r1, r8
003b4df4: mov      r0, sl
003b4df8: bl       #0x337a88
003b4dfc: mov      r0, r8
003b4e00: bl       #0x318254
003b4e04: movw     r3, #0x13fc
003b4e08: ldr      r2, [r4, r3]
003b4e0c: movw     r3, #0x13f8
003b4e10: ldr      r3, [r4, r3]
003b4e14: cmp      r3, r2
003b4e18: beq      #0x3b4e6c
003b4e1c: ldr      r1, [pc, #0x780]
003b4e20: add      r8, sp, #0x24
003b4e24: ldr      r3, [r4, #0x64]
003b4e28: ldr      r1, [r5, r1]
003b4e2c: mov      r0, r8
003b4e30: ldr      r1, [r1, #0x38]
003b4e34: str      r7, [sp]
003b4e38: str      r7, [sp, #4]
003b4e3c: bl       #0x34aca0
003b4e40: mov      r0, r8
003b4e44: mov      r1, r7
003b4e48: bl       #0x33fdc0
003b4e4c: cmp      r0, #0
003b4e50: beq      #0x3b4e6c
003b4e54: mov      r0, r8
003b4e58: bl       #0x33fee4
003b4e5c: subs     r1, r0, #0
003b4e60: beq      #0x3b4e6c
003b4e64: ldr      r0, [r4, #0x378]
003b4e68: bl       #0x405540
003b4e6c: mov      r0, r4
003b4e70: movw     r7, #0x13c8
003b4e74: bl       #0x3b3d38
003b4e78: ldrsh    r3, [r4, r7]
003b4e7c: cmn      r3, #1
003b4e80: movweq   r1, #0xffff
003b4e84: beq      #0x3b4ec4
003b4e88: ldr      sl, [r5, fp]
003b4e8c: add      r8, sp, #0xc4
003b4e90: mov      r0, sl
003b4e94: bl       #0x337888
003b4e98: ldr      r1, [pc, #0x708]
003b4e9c: add      r2, sp, #0x44
003b4ea0: mov      r0, r8
003b4ea4: add      r1, pc, r1
003b4ea8: bl       #0x3140ec
003b4eac: mov      r1, r8
003b4eb0: mov      r0, sl
003b4eb4: bl       #0x337a88
003b4eb8: mov      r0, r8
003b4ebc: bl       #0x318254
003b4ec0: ldrh     r1, [r4, r7]
003b4ec4: add      r7, r4, #0x560
003b4ec8: sxth     r1, r1
003b4ecc: mov      r0, r7
003b4ed0: bl       #0x3df2a4
003b4ed4: mov      r0, r7
003b4ed8: mov      r1, #1
003b4edc: bl       #0x3e0810
003b4ee0: mov      r0, r4
003b4ee4: bl       #0x3a54d4
003b4ee8: subs     r8, r0, #0
003b4eec: beq      #0x3b4f04
003b4ef0: bl       #0x30de54
003b4ef4: mov      r1, r8
003b4ef8: add      r2, r8, r0
003b4efc: add      r0, r4, #0x290
003b4f00: bl       #0x3109e0
003b4f04: ldr      sl, [r5, fp]
003b4f08: add      r8, sp, #0xac
003b4f0c: mov      r0, sl
003b4f10: bl       #0x337888
003b4f14: ldr      r1, [pc, #0x690]
003b4f18: add      r2, sp, #0x40
003b4f1c: mov      r0, r8
003b4f20: add      r1, pc, r1
003b4f24: bl       #0x3140ec
003b4f28: mov      r1, r8
003b4f2c: mov      r0, sl
003b4f30: bl       #0x337a88
003b4f34: mov      r0, r8
003b4f38: bl       #0x318254
003b4f3c: ldr      r0, [r4, #0x59c]
003b4f40: bl       #0x30e964
003b4f44: movw     r1, #0x74bc
003b4f48: movt     r1, #0x3c13
003b4f4c: bl       #0x30ed6c
003b4f50: str      r0, [r4, #0x120]
003b4f54: ldr      r0, [r4, #0x5a0]
003b4f58: bl       #0x30e964
003b4f5c: movw     r1, #0x74bc
003b4f60: movt     r1, #0x3c13
003b4f64: bl       #0x30ed6c
003b4f68: str      r0, [r4, #0x124]
003b4f6c: ldr      r0, [r4, #0x5a4]
003b4f70: bl       #0x30e964
003b4f74: movw     r1, #0xd70a
003b4f78: movt     r1, #0x3c23
003b4f7c: bl       #0x30ed6c
003b4f80: str      r0, [r4, #0x128]
003b4f84: mov      r0, r4
003b4f88: bl       #0x38be5c
003b4f8c: mov      r0, r4
003b4f90: bl       #0x38ab60
003b4f94: subs     r1, r0, #0
003b4f98: beq      #0x3b5488
003b4f9c: mov      r1, #2
003b4fa0: mov      r0, r4
003b4fa4: bl       #0x3bc4d0
003b4fa8: ldr      r3, [pc, #0x600]
003b4fac: mov      r0, r4
003b4fb0: ldr      r3, [r5, r3]
003b4fb4: ldr      r8, [r3]
003b4fb8: bl       #0x3a2fec
003b4fbc: mov      r3, #0x44
003b4fc0: mla      r8, r3, r0, r8
003b4fc4: ldr      r3, [r4]
003b4fc8: mov      r0, r4
003b4fcc: mov      lr, pc
003b4fd0: ldr      pc, [r3, #0x28]
003b4fd4: cmp      r0, #0
003b4fd8: bne      #0x3b4ff4
003b4fdc: ldrb     r3, [r8, #0x10]
003b4fe0: cmp      r3, #0
003b4fe4: beq      #0x3b4ff4
003b4fe8: mov      r3, #1
003b4fec: strb     r3, [r4, #0x3ec]
003b4ff0: b        #0x3b5014
003b4ff4: ldr      r3, [r4]
003b4ff8: mov      r0, r4
003b4ffc: mov      lr, pc
003b5000: ldr      pc, [r3, #0x28]
003b5004: cmp      r0, #0
003b5008: bne      #0x3b54a4
003b500c: add      r0, r4, #0x3c8
003b5010: bl       #0x3cf1f0
003b5014: movw     r3, #0x1488
003b5018: ldr      r2, [pc, #0x594]
003b501c: ldr      r1, [r4, r3]
003b5020: ldr      r3, [r8, #0x30]
003b5024: str      r2, [sp, #0xc]
003b5028: mov      r2, r4
003b502c: add      r1, r1, r3
003b5030: ldr      r3, [sp, #0xc]
003b5034: ldr      sb, [pc, #0x57c]
003b5038: add      sl, sp, #0x94
003b503c: ldr      r0, [r5, r3]
003b5040: bl       #0x495430
003b5044: movw     r3, #0x1484
003b5048: str      r0, [r4, r3]
003b504c: mov      r0, r4
003b5050: bl       #0x3b4738
003b5054: ldr      r8, [r5, fp]
003b5058: add      sb, pc, sb
003b505c: mov      r0, r8
003b5060: bl       #0x337888
003b5064: add      r2, sp, #0x3c
003b5068: mov      r0, sl
003b506c: mov      r1, sb
003b5070: bl       #0x3140ec
003b5074: mov      r1, sl
003b5078: mov      r0, r8
003b507c: bl       #0x337a88
003b5080: mov      r0, sl
003b5084: bl       #0x318254
003b5088: add      r0, r4, #0x490
003b508c: add      r0, r0, #0xc
003b5090: bl       #0x3c9f4c
003b5094: add      sl, sp, #0x7c
003b5098: mov      r0, r8
003b509c: bl       #0x337888
003b50a0: add      r2, sp, #0x38
003b50a4: mov      r0, sl
003b50a8: mov      r1, sb
003b50ac: bl       #0x3140ec
003b50b0: mov      r1, sl
003b50b4: mov      r0, r8
003b50b8: bl       #0x337a88
003b50bc: mov      r0, sl
003b50c0: bl       #0x318254
003b50c4: mov      r0, r4
003b50c8: bl       #0x3b3b00
003b50cc: add      sl, sp, #0x64
003b50d0: mov      r0, r8
003b50d4: bl       #0x337888
003b50d8: add      r2, sp, #0x34
003b50dc: mov      r1, sb
003b50e0: mov      r0, sl
003b50e4: bl       #0x3140ec
003b50e8: mov      r1, sl
003b50ec: mov      r0, r8
003b50f0: bl       #0x337a88
003b50f4: mov      r0, sl
003b50f8: bl       #0x318254
003b50fc: ldr      r3, [r4]
003b5100: mov      r0, r4
003b5104: mov      lr, pc
003b5108: ldr      pc, [r3, #0x28]
003b510c: cmp      r0, #0
003b5110: beq      #0x3b538c
003b5114: ldr      r3, [r4, #0x2d8]
003b5118: cmp      r3, #0
003b511c: beq      #0x3b512c
003b5120: ldr      r0, [r3, #8]
003b5124: mov      r1, #0
003b5128: bl       #0x59719c
003b512c: ldr      r2, [pc, #0x470]
003b5130: mov      r0, r4
003b5134: mov      r1, #4
003b5138: str      r2, [sp, #0x10]
003b513c: bl       #0x3bc4d0
003b5140: ldr      r3, [sp, #0x10]
003b5144: ldr      r0, [r5, r3]
003b5148: bl       #0x31f594
003b514c: cmp      r0, #0
003b5150: beq      #0x3b5160
003b5154: ldr      r1, [r0, #0x118]
003b5158: mov      r0, r4
003b515c: bl       #0x3bb950
003b5160: ldr      r2, [sp, #0x10]
003b5164: mov      r1, r4
003b5168: ldr      r3, [r5, r2]
003b516c: ldr      r0, [r3, #0x40]
003b5170: bl       #0x36effc
003b5174: cmp      r0, #0
003b5178: bne      #0x3b54dc
003b517c: movw     r3, #0x13c8
003b5180: ldrsh    r1, [r4, r3]
003b5184: mov      r0, r7
003b5188: bl       #0x3df2a4
003b518c: mov      r0, r7
003b5190: bl       #0x3df480
003b5194: mov      r0, r7
003b5198: mov      r1, #1
003b519c: bl       #0x3e0810
003b51a0: ldr      r2, [sp, #0x10]
003b51a4: mov      r1, r4
003b51a8: ldr      r3, [r5, r2]
003b51ac: ldr      r0, [r3, #0x40]
003b51b0: bl       #0x36effc
003b51b4: cmp      r0, #0
003b51b8: bne      #0x3b54d0
003b51bc: movw     r3, #0xc9ff
003b51c0: movt     r3, #0x3b9a
003b51c4: str      r3, [r4, #0x3a4]
003b51c8: mov      r2, #0
003b51cc: mov      r0, r7
003b51d0: mov      r1, #0xc2
003b51d4: bl       #0x3df6e0
003b51d8: ldr      r2, [sp, #0xc]
003b51dc: bic      r0, r0, r0, asr #31
003b51e0: strb     r0, [r4, #0x3a8]
003b51e4: ldr      r3, [r5, r2]
003b51e8: ldr      r2, [r3, #0x1c]
003b51ec: ldr      r3, [r3, #0x20]
003b51f0: rsb      r3, r2, r3
003b51f4: asr      r3, r3, #3
003b51f8: add      r2, r3, r3, lsl #2
003b51fc: add      r2, r2, r2, lsl #4
003b5200: add      r2, r2, r2, lsl #8
003b5204: add      r2, r2, r2, lsl #16
003b5208: add      r3, r3, r2, lsl #1
003b520c: cmp      r3, #0
003b5210: beq      #0x3b538c
003b5214: mov      r0, #0x24
003b5218: mov      r1, #0
003b521c: bl       #0x31056c
003b5220: movw     r3, #0x1494
003b5224: str      r0, [r4, r3]
003b5228: ldr      r3, [pc, #0x38c]
003b522c: mov      sb, r0
003b5230: ldr      r3, [r5, r3]
003b5234: ldr      sl, [r3]
003b5238: cmp      sl, #0
003b523c: beq      #0x3b550c
003b5240: ldr      r3, [pc, #0x378]
003b5244: ldr      r2, [pc, #0x378]
003b5248: str      r7, [sp, #0x18]
003b524c: ldr      r3, [r5, r3]
003b5250: add      r2, pc, r2
003b5254: mov      r8, #0
003b5258: ldr      r3, [r3]
003b525c: str      r0, [sp, #0x14]
003b5260: mov      sb, r2
003b5264: mov      r7, r3
003b5268: b        #0x3b5278
003b526c: add      r8, r8, #1
003b5270: cmp      r8, sl
003b5274: beq      #0x3b5504
003b5278: mov      r0, sb
003b527c: ldr      r1, [r7, r8, lsl #2]
003b5280: bl       #0x30e31c
003b5284: cmp      r0, #0
003b5288: bne      #0x3b526c
003b528c: ldr      sb, [sp, #0x14]
003b5290: ldr      r7, [sp, #0x18]
003b5294: mov      r1, r8
003b5298: ldr      r3, [sp, #0xc]
003b529c: str      r7, [sp, #0x14]
003b52a0: str      fp, [sp, #0x18]
003b52a4: ldr      r2, [r5, r3]
003b52a8: ldr      r3, [pc, #0x318]
003b52ac: str      r6, [sp, #0x1c]
003b52b0: mov      r8, #0
003b52b4: movw     sl, #0x1494
003b52b8: mov      r7, r1
003b52bc: mov      r6, r2
003b52c0: mov      fp, r3
003b52c4: b        #0x3b52cc
003b52c8: ldr      sb, [r4, sl]
003b52cc: mov      r0, r6
003b52d0: add      r1, r7, r8
003b52d4: mov      r2, #0
003b52d8: bl       #0x495430
003b52dc: str      r0, [sb, r8, lsl #2]
003b52e0: ldr      r3, [r4, sl]
003b52e4: ldr      r3, [r3, r8, lsl #2]
003b52e8: cmp      r3, #0
003b52ec: beq      #0x3b5350
003b52f0: ldr      r2, [r5, fp]
003b52f4: mov      r0, r3
003b52f8: mov      r1, #0
003b52fc: ldr      lr, [r2]
003b5300: ldr      ip, [r2, #4]
003b5304: ldr      r2, [r2, #8]
003b5308: str      lr, [r3, #0x34]
003b530c: str      ip, [r3, #0x38]
003b5310: str      r2, [r3, #0x3c]
003b5314: bl       #0x492aa0
003b5318: ldr      r3, [r4, sl]
003b531c: mov      r1, #0
003b5320: ldr      r0, [r3, r8, lsl #2]
003b5324: bl       #0x492ef0
003b5328: ldr      r3, [r4, sl]
003b532c: ldr      r0, [r3, r8, lsl #2]
003b5330: bl       #0x49267c
003b5334: ldr      r3, [r0]
003b5338: mov      lr, pc
003b533c: ldr      pc, [r3, #0x44]
003b5340: mov      r1, #1
003b5344: ldr      r3, [r0]
003b5348: mov      lr, pc
003b534c: ldr      pc, [r3, #0x40]
003b5350: add      r8, r8, #1
003b5354: cmp      r8, #9
003b5358: bne      #0x3b52c8
003b535c: ldr      r2, [sp, #0x10]
003b5360: mov      r1, r4
003b5364: ldr      r7, [sp, #0x14]
003b5368: ldr      r3, [r5, r2]
003b536c: ldr      fp, [sp, #0x18]
003b5370: ldr      r6, [sp, #0x1c]
003b5374: ldr      r0, [r3, #0x40]
003b5378: bl       #0x36effc
003b537c: cmp      r0, #0
003b5380: bne      #0x3b5514
003b5384: mov      r0, r4
003b5388: bl       #0x3a41a0
003b538c: add      r8, r4, #0xff0
003b5390: add      r8, r8, #4
003b5394: mov      r1, r8
003b5398: mov      r2, #0xd2
003b539c: mov      r0, r7
003b53a0: bl       #0x3dedb4
003b53a4: bl       #0x30e964
003b53a8: mov      r3, #0x1440
003b53ac: str      r0, [r4, r3]
003b53b0: mov      r2, #0xd3
003b53b4: mov      r1, r8
003b53b8: mov      r0, r7
003b53bc: bl       #0x3dedb4
003b53c0: bl       #0x30e964
003b53c4: movw     r3, #0x1444
003b53c8: str      r0, [r4, r3]
003b53cc: add      r1, r4, #0x160
003b53d0: mov      r0, r4
003b53d4: bl       #0x3a58f4
003b53d8: add      r1, r4, #0x1440
003b53dc: mov      r0, r4
003b53e0: add      r1, r1, #0x10
003b53e4: mov      r2, #1
003b53e8: bl       #0x393db4
003b53ec: ldr      ip, [r4, #0x16c]
003b53f0: ldr      r0, [r4, #0x2d8]
003b53f4: ldr      r1, [r4, #0x170]
003b53f8: ldr      r2, [r4, #0x174]
003b53fc: movw     r3, #0x145c
003b5400: str      ip, [r4, r3]
003b5404: movw     r3, #0x1460
003b5408: str      r1, [r4, r3]
003b540c: cmp      r0, #0
003b5410: movw     r3, #0x1464
003b5414: str      r2, [r4, r3]
003b5418: beq      #0x3b5420
003b541c: bl       #0x470a54
003b5420: mov      r1, #0
003b5424: mov      r2, #1
003b5428: mov      r0, r4
003b542c: bl       #0x3a59ac
003b5430: mov      r0, r4
003b5434: bl       #0x3b3a70
003b5438: ldrb     r1, [r4, #0x3ec]
003b543c: cmp      r1, #0
003b5440: beq      #0x3b54c4
003b5444: mov      r0, r4
003b5448: bl       #0x3d37d0
003b544c: ldr      r7, [r5, fp]
003b5450: add      r4, sp, #0x4c
003b5454: mov      r0, r7
003b5458: bl       #0x337888
003b545c: ldr      r1, [pc, #0x168]
003b5460: add      r2, sp, #0x30
003b5464: mov      r0, r4
003b5468: add      r1, pc, r1
003b546c: bl       #0x3140ec
003b5470: mov      r0, r7
003b5474: mov      r1, r4
003b5478: bl       #0x337a88
003b547c: mov      r0, r4
003b5480: bl       #0x318254
003b5484: b        #0x3b4d94
003b5488: mov      r0, r4
003b548c: ldr      r3, [r4]
003b5490: mov      lr, pc
003b5494: ldr      pc, [r3, #0x40]
003b5498: mov      r0, r4
003b549c: bl       #0x33ddb4
003b54a0: b        #0x3b4d94
003b54a4: ldr      r3, [pc, #0xf8]
003b54a8: mov      r1, r4
003b54ac: ldr      r3, [r5, r3]
003b54b0: ldr      r0, [r3, #0x40]
003b54b4: bl       #0x36effc
003b54b8: cmp      r0, #0
003b54bc: bne      #0x3b500c
003b54c0: b        #0x3b4fe8
003b54c4: add      r0, r4, #0x3c8
003b54c8: bl       #0x3ce7c0
003b54cc: b        #0x3b5444
003b54d0: mov      r0, r4
003b54d4: bl       #0x3b3a90
003b54d8: b        #0x3b51bc
003b54dc: mov      r0, r4
003b54e0: bl       #0x3b395c
003b54e4: mov      r0, r7
003b54e8: bl       #0x3defac
003b54ec: movw     r3, #0x14e8
003b54f0: ldr      r0, [r4, r3]
003b54f4: cmp      r0, #0
003b54f8: beq      #0x3b517c
003b54fc: bl       #0x4679e8
003b5500: b        #0x3b517c
003b5504: ldr      sb, [sp, #0x14]
003b5508: ldr      r7, [sp, #0x18]
003b550c: mvn      r1, #0
003b5510: b        #0x3b5298
003b5514: ldr      r3, [sp, #0xc]
003b5518: mov      r2, #0
003b551c: movw     r8, #0x149c
003b5520: ldr      r0, [r5, r3]
003b5524: ldr      r3, [pc, #0xa4]
003b5528: ldr      r3, [r5, r3]
003b552c: ldr      r3, [r3]
003b5530: ldr      r1, [r3, #0x88]
003b5534: bl       #0x495430
003b5538: cmp      r0, #0
003b553c: str      r0, [r4, r8]
003b5540: beq      #0x3b5384
003b5544: mov      r2, #0
003b5548: str      r2, [r0, #0x3c]
003b554c: str      r2, [r0, #0x34]
003b5550: str      r2, [r0, #0x38]
003b5554: mov      r1, #0
003b5558: bl       #0x492aa0
003b555c: mov      r1, #0
003b5560: ldr      r0, [r4, r8]
003b5564: bl       #0x492ef0
003b5568: ldr      r0, [r4, r8]
003b556c: bl       #0x49267c
003b5570: ldr      r3, [r0]
003b5574: mov      lr, pc
003b5578: ldr      pc, [r3, #0x44]
003b557c: mov      r1, #1
003b5580: ldr      r3, [r0]
003b5584: mov      lr, pc
003b5588: ldr      pc, [r3, #0x40]
003b558c: b        #0x3b5384
003b5590: bl       #0x30e310
003b5594: subseq   pc, sp, r0, lsr #26
003b5598: andeq    r4, r0, ip, lsr #1
003b559c: andeq    r0, r0, r4, lsl #17
003b55a0: subseq   pc, r0, r8, lsr #32
003b55a4: strdeq   r3, r4, [r0], -r4
003b55a8: subseq   lr, r0, ip, ror #30
003b55ac: ldrsheq  lr, [r0], #-0xe0
003b55b0: andeq    r0, r0, r8, asr r7
003b55b4: andeq    r1, r0, r8, lsl #22
003b55b8: ldrheq   lr, [r0], #-0xd8
003b55bc: andeq    r0, r0, r4, asr #13
003b55c0: muleq    r0, r4, r2
003b55c4: subseq   lr, r0, r0, ror #24
003b55c8: andeq    r3, r0, ip, lsr #30
003b55cc: subseq   lr, r0, r8, lsr #19
003b55d0: andeq    r3, r0, r8, asr #5

# _ZN6CharAI17InitScriptProcessEb
003ce7c0: push     {r4, r5, r6, lr}
003ce7c4: mov      r4, r0
003ce7c8: ldr      r0, [r0, #4]
003ce7cc: mov      r5, r1
003ce7d0: bl       #0x3b3a70
003ce7d4: mov      r0, r4
003ce7d8: bl       #0x3ce044
003ce7dc: mov      r0, r4
003ce7e0: bl       #0x3d8894
003ce7e4: ldr      r3, [r4]
003ce7e8: mov      r0, r4
003ce7ec: mov      lr, pc
003ce7f0: ldr      pc, [r3, #0xc]
003ce7f4: cmp      r5, #0
003ce7f8: beq      #0x3ce80c
003ce7fc: mov      r0, r4
003ce800: ldr      r3, [r4]
003ce804: mov      lr, pc
003ce808: ldr      pc, [r3, #0x10]
003ce80c: pop      {r4, r5, r6, pc}
