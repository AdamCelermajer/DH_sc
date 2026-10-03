
# _ZN7PODecorC1EP13PhysicalWorldP10GameObjectb.clone.2
00388a2c: push     {r4, r5, lr}
00388a30: mov      lr, #1
00388a34: sub      sp, sp, #0x24
00388a38: mov      r5, #2
00388a3c: mov      ip, #0
00388a40: mov      r3, lr
00388a44: str      r5, [sp, #0x10]
00388a48: ldr      r4, [pc, #0x40]
00388a4c: movw     r5, #0xffff
00388a50: str      r5, [sp, #0x14]
00388a54: str      ip, [sp, #0xc]
00388a58: mov      r5, r0
00388a5c: str      ip, [sp]
00388a60: str      ip, [sp, #4]
00388a64: str      ip, [sp, #8]
00388a68: str      lr, [sp, #0x18]
00388a6c: bl       #0x46f2f0
00388a70: ldr      r3, [pc, #0x1c]
00388a74: add      r4, pc, r4
00388a78: mov      r0, r5
00388a7c: ldr      r3, [r4, r3]
00388a80: add      r3, r3, #8
00388a84: str      r3, [r5]
00388a88: add      sp, sp, #0x24
00388a8c: pop      {r4, r5, pc}
00388a90: rsbeq    ip, r0, ip, lsl r0
00388a94: andeq    r2, r0, r8, lsl r4

# _ZN7PODecorC1EP13PhysicalWorldP10GameObjectb.clone.2
0039fc2c: push     {r4, r5, lr}
0039fc30: mov      lr, #1
0039fc34: sub      sp, sp, #0x24
0039fc38: mov      r5, #2
0039fc3c: mov      ip, #0
0039fc40: mov      r3, lr
0039fc44: str      r5, [sp, #0x10]
0039fc48: ldr      r4, [pc, #0x40]
0039fc4c: movw     r5, #0xffff
0039fc50: str      r5, [sp, #0x14]
0039fc54: str      ip, [sp, #0xc]
0039fc58: mov      r5, r0
0039fc5c: str      ip, [sp]
0039fc60: str      ip, [sp, #4]
0039fc64: str      ip, [sp, #8]
0039fc68: str      lr, [sp, #0x18]
0039fc6c: bl       #0x46f2f0
0039fc70: ldr      r3, [pc, #0x1c]
0039fc74: add      r4, pc, r4
0039fc78: mov      r0, r5
0039fc7c: ldr      r3, [r4, r3]
0039fc80: add      r3, r3, #8
0039fc84: str      r3, [r5]
0039fc88: add      sp, sp, #0x24
0039fc8c: pop      {r4, r5, pc}
0039fc90: subseq   r4, pc, ip, lsl lr
0039fc94: andeq    r2, r0, r8, lsl r4

# _ZN10GameObject17SetPhysicalObjectEP14PhysicalObjectb
00394bf8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00394bfc: ldr      r4, [pc, #0x120]
00394c00: ldr      r7, [pc, #0x120]
00394c04: ldr      ip, [pc, #0x120]
00394c08: add      r4, pc, r4
00394c0c: ldr      r3, [r4, r7]
00394c10: ldr      r8, [r4, ip]
00394c14: sub      sp, sp, #0x20
00394c18: ldr      r3, [r3]
00394c1c: add      r5, sp, #4
00394c20: mov      sl, r0
00394c24: mov      r0, r8
00394c28: str      r3, [sp, #0x1c]
00394c2c: mov      sb, r2
00394c30: mov      r6, r1
00394c34: bl       #0x337888
00394c38: mov      r0, r5
00394c3c: mov      r1, #0xd
00394c40: str      r5, [sp, #0x14]
00394c44: str      r5, [sp, #0x18]
00394c48: bl       #0x31167c
00394c4c: ldr      r1, [pc, #0xdc]
00394c50: mov      r2, #0xc
00394c54: ldr      r0, [sp, #0x18]
00394c58: add      r1, pc, r1
00394c5c: bl       #0x30e868
00394c60: add      r3, r0, #0xc
00394c64: str      r3, [sp, #0x14]
00394c68: mov      r3, #0
00394c6c: strb     r3, [r0, #0xc]
00394c70: mov      r1, r5
00394c74: mov      r0, r8
00394c78: bl       #0x337a88
00394c7c: mov      r8, r0
00394c80: mov      r0, r5
00394c84: bl       #0x3139ac
00394c88: cmp      r8, #0
00394c8c: beq      #0x394cc4
00394c90: cmp      r6, #0
00394c94: beq      #0x394ca8
00394c98: mov      r0, r6
00394c9c: ldr      r3, [r6]
00394ca0: mov      lr, pc
00394ca4: ldr      pc, [r3, #4]
00394ca8: ldr      r3, [r4, r7]
00394cac: ldr      r2, [sp, #0x1c]
00394cb0: ldr      r3, [r3]
00394cb4: cmp      r2, r3
00394cb8: bne      #0x394d20
00394cbc: add      sp, sp, #0x20
00394cc0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00394cc4: ldr      r3, [sl, #0x2dc]
00394cc8: cmp      r3, r6
00394ccc: beq      #0x394d00
00394cd0: cmp      r3, #0
00394cd4: beq      #0x394cec
00394cd8: mov      r0, r3
00394cdc: ldr      r3, [r3]
00394ce0: mov      lr, pc
00394ce4: ldr      pc, [r3, #4]
00394ce8: str      r8, [sl, #0x2dc]
00394cec: cmp      r6, #0
00394cf0: str      r6, [sl, #0x2dc]
00394cf4: beq      #0x394d00
00394cf8: cmp      sb, #0
00394cfc: bne      #0x394d0c
00394d00: mov      r0, sl
00394d04: bl       #0x393ea0
00394d08: b        #0x394ca8
00394d0c: mov      r0, r6
00394d10: bl       #0x46eb20
00394d14: mov      r0, sl
00394d18: bl       #0x393ea0
00394d1c: b        #0x394ca8
00394d20: bl       #0x30e310
00394d24: subseq   pc, pc, r8, lsl #29
00394d28: andeq    r4, r0, ip, lsr #1
00394d2c: andeq    r0, r0, r4, lsl #17
00394d30: subseq   sp, r2, r8, lsr #24

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

# _ZN14PhysicalObject5_initEP10b2ShapeDefffbb
0046edf0: push     {r4, r5, r6, r7, lr}
0046edf4: sub      sp, sp, #0x34
0046edf8: ldrb     lr, [sp, #0x4c]
0046edfc: mov      r4, #0
0046ee00: mov      r6, r1
0046ee04: mov      r5, r0
0046ee08: mov      ip, #1
0046ee0c: ldr      r0, [r0, #4]
0046ee10: add      r1, sp, #4
0046ee14: str      r4, [sp, #8]
0046ee18: str      r4, [sp, #0xc]
0046ee1c: str      r4, [sp, #4]
0046ee20: str      r4, [sp, #0x10]
0046ee24: str      r4, [sp, #0x20]
0046ee28: str      r4, [sp, #0x24]
0046ee2c: str      r4, [sp, #0x28]
0046ee30: ldrb     r7, [sp, #0x48]
0046ee34: str      r3, [sp, #0x1c]
0046ee38: strb     lr, [sp, #0x2f]
0046ee3c: strb     ip, [sp, #0x2e]
0046ee40: str      r2, [sp, #0x18]
0046ee44: strb     ip, [sp, #0x2c]
0046ee48: str      r5, [sp, #0x14]
0046ee4c: strb     ip, [sp, #0x2d]
0046ee50: bl       #0x34bcf0
0046ee54: mov      r3, #0x3f800000
0046ee58: str      r0, [r5, #0x14]
0046ee5c: str      r5, [r0, #0x90]
0046ee60: str      r3, [r6, #0xc]
0046ee64: ldrb     r3, [r6, #0x18]
0046ee68: cmp      r7, #0
0046ee6c: str      r4, [r6, #0x10]
0046ee70: movweq   r4, #0xd70a
0046ee74: movteq   r4, #0x4133
0046ee78: cmp      r3, #0
0046ee7c: str      r5, [r6, #8]
0046ee80: str      r4, [r6, #0x14]
0046ee84: bne      #0x46eea4
0046ee88: mov      r1, r6
0046ee8c: mov      r0, r5
0046ee90: mov      r2, #1
0046ee94: bl       #0x46edb8
0046ee98: str      r0, [r5, #0x18]
0046ee9c: add      sp, sp, #0x34
0046eea0: pop      {r4, r5, r6, r7, pc}
0046eea4: mov      r1, r6
0046eea8: mov      r0, r5
0046eeac: mov      r2, #1
0046eeb0: bl       #0x46edb8
0046eeb4: str      r0, [r5, #0x1c]
0046eeb8: b        #0x46ee9c

# _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeERKSsb
00352b74: push     {r4, r5, r6, r7, r8, lr}
00352b78: subs     r4, r1, #0
00352b7c: mov      r8, r0
00352b80: mov      r5, r2
00352b84: mov      r7, r3
00352b88: beq      #0x352c04
00352b8c: cmp      r3, #0
00352b90: bne      #0x352c0c
00352b94: ldr      r3, [r4]
00352b98: mov      r0, r4
00352b9c: mov      lr, pc
00352ba0: ldr      pc, [r3, #0x24]
00352ba4: ldr      r1, [r5, #0x14]
00352ba8: bl       #0x30e31c
00352bac: cmp      r0, #0
00352bb0: beq      #0x352c04
00352bb4: mov      r0, r4
00352bb8: bl       #0x5971c8
00352bbc: mov      r6, r0
00352bc0: ldr      r4, [r6, #4]!
00352bc4: cmp      r4, r6
00352bc8: moveq    r4, #0
00352bcc: beq      #0x352c04
00352bd0: cmp      r4, #0
00352bd4: moveq    r1, r4
00352bd8: subne    r1, r4, #4
00352bdc: mov      r0, r8
00352be0: mov      r2, r5
00352be4: mov      r3, r7
00352be8: bl       #0x352b74
00352bec: ldr      r4, [r4]
00352bf0: cmp      r6, r4
00352bf4: beq      #0x352c00
00352bf8: cmp      r0, #0
00352bfc: beq      #0x352bd0
00352c00: mov      r4, r0
00352c04: mov      r0, r4
00352c08: pop      {r4, r5, r6, r7, r8, pc}
00352c0c: ldr      r3, [r4]
00352c10: mov      r0, r4
00352c14: mov      lr, pc
00352c18: ldr      pc, [r3, #0x24]
00352c1c: ldr      r1, [r5, #0x14]
00352c20: ldr      r2, [r5, #0x10]
00352c24: rsb      r2, r1, r2
00352c28: bl       #0x30ec7c
00352c2c: cmp      r0, #0
00352c30: bne      #0x352bb4
00352c34: mov      r0, r4
00352c38: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14PhysicalObject9_addShapeEP10b2ShapeDefb
0046edb8: push     {r4, r5, r6, lr}
0046edbc: mov      r4, r0
0046edc0: ldr      r0, [r0, #0x14]
0046edc4: mov      r5, r2
0046edc8: bl       #0x7e1dc8
0046edcc: subs     r6, r0, #0
0046edd0: beq      #0x46ede8
0046edd4: cmp      r5, #0
0046edd8: str      r4, [r6, #0x2c]
0046eddc: beq      #0x46ede8
0046ede0: ldr      r0, [r4, #0x14]
0046ede4: bl       #0x7e1818
0046ede8: mov      r0, r6
0046edec: pop      {r4, r5, r6, pc}

# _ZN12VisualObjectC2EP10GameObjectRKSsS3_
00472c5c: push     {r4, r5, r6, r7, r8, sl, lr}
00472c60: ldr      r6, [pc, #0x234]
00472c64: ldr      r5, [pc, #0x234]
00472c68: mov      ip, #0xbf000000
00472c6c: add      r6, pc, r6
00472c70: ldr      r5, [r6, r5]
00472c74: mov      lr, #0
00472c78: add      ip, ip, #0x800000
00472c7c: mov      r7, r1
00472c80: add      r1, r5, #8
00472c84: mov      r5, #0
00472c88: mov      r8, r3
00472c8c: sub      sp, sp, #0xc
00472c90: str      r1, [r0]
00472c94: str      lr, [r0, #0x24]
00472c98: str      ip, [r0, #0x74]
00472c9c: str      lr, [r0, #0x10]
00472ca0: str      lr, [r0, #0x14]
00472ca4: str      lr, [r0, #0x18]
00472ca8: str      lr, [r0, #0x1c]
00472cac: str      lr, [r0, #0x20]
00472cb0: str      ip, [r0, #0x58]
00472cb4: str      ip, [r0, #0x5c]
00472cb8: str      ip, [r0, #0x60]
00472cbc: str      ip, [r0, #0x64]
00472cc0: str      ip, [r0, #0x68]
00472cc4: str      r7, [r0, #4]
00472cc8: str      r5, [r0, #8]
00472ccc: str      r5, [r0, #0xc]
00472cd0: strb     r5, [r0, #0x28]
00472cd4: str      r5, [r0, #0x2c]
00472cd8: str      r5, [r0, #0x30]
00472cdc: str      r5, [r0, #0x34]
00472ce0: str      r5, [r0, #0x38]
00472ce4: strb     r5, [r0, #0x3c]
00472ce8: str      r5, [r0, #0x40]
00472cec: str      r5, [r0, #0x44]
00472cf0: str      r5, [r0, #0x48]
00472cf4: str      r5, [r0, #0x4c]
00472cf8: str      r5, [r0, #0x50]
00472cfc: str      r5, [r0, #0x54]
00472d00: strb     r5, [r0, #0x6c]
00472d04: strb     r5, [r0, #0x7c]
00472d08: strb     r5, [r0, #0x7d]
00472d0c: strb     r5, [r0, #0x7e]
00472d10: strb     r5, [r0, #0x7f]
00472d14: str      r5, [r0, #0x80]
00472d18: str      r5, [r0, #0x84]
00472d1c: str      r5, [r0, #0x88]
00472d20: str      r5, [r0, #0x8c]
00472d24: str      r5, [r0, #0x90]
00472d28: str      r5, [r0, #0x94]
00472d2c: str      r5, [r0, #0x9c]
00472d30: str      r5, [r0, #0xa0]
00472d34: str      r5, [r0, #0xa4]
00472d38: strb     r5, [r0, #0xa9]
00472d3c: mov      sl, r2
00472d40: mov      r4, r0
00472d44: bl       #0x50a564
00472d48: ldr      ip, [r8, #0x10]
00472d4c: ldr      r2, [r8, #0x14]
00472d50: ldr      r1, [sl, #0x14]
00472d54: mov      r3, r5
00472d58: cmp      ip, r2
00472d5c: moveq    r2, r5
00472d60: mvn      ip, #0x80000000
00472d64: str      ip, [sp]
00472d68: bl       #0x50a504
00472d6c: cmp      r0, r5
00472d70: str      r0, [r4, #8]
00472d74: beq      #0x472e90
00472d78: mov      r1, r7
00472d7c: mov      r0, r4
00472d80: bl       #0x47295c
00472d84: ldr      r0, [r4, #8]
00472d88: bl       #0x35c854
00472d8c: mov      r0, r4
00472d90: bl       #0x4718f0
00472d94: ldr      r3, [pc, #0x108]
00472d98: ldr      r1, [r4, #8]
00472d9c: ldr      r5, [r6, r3]
00472da0: ldr      r3, [r5, #0x10]
00472da4: ldr      r3, [r3, #0x1c]
00472da8: ldr      r3, [r3, #4]
00472dac: mov      r0, r3
00472db0: ldr      r3, [r3]
00472db4: mov      lr, pc
00472db8: ldr      pc, [r3, #0x5c]
00472dbc: ldr      r3, [r5, #0x10]
00472dc0: ldr      r0, [r3, #0x1c]
00472dc4: bl       #0x350ee0
00472dc8: ldr      r3, [r5, #0x10]
00472dcc: ldr      r2, [pc, #0xd4]
00472dd0: ldr      r1, [r4, #8]
00472dd4: ldr      r0, [r3, #0x1c]
00472dd8: add      r2, pc, r2
00472ddc: mov      r3, #1
00472de0: bl       #0x35a0e4
00472de4: subs     r2, r0, #0
00472de8: beq      #0x472e58
00472dec: mov      r3, #1
00472df0: strb     r3, [r4, #0x28]
00472df4: ldr      r3, [r5, #0x10]
00472df8: movw     r1, #0x6164
00472dfc: movt     r1, #0x6d65
00472e00: ldr      r3, [r3, #0x1c]
00472e04: mov      r0, r3
00472e08: ldr      r3, [r3]
00472e0c: mov      lr, pc
00472e10: ldr      pc, [r3, #0x1c]
00472e14: cmp      r0, #0
00472e18: str      r0, [r4, #0xc]
00472e1c: beq      #0x472e3c
00472e20: ldr      r3, [r0]
00472e24: ldr      r3, [r3, #-0xc]
00472e28: add      r0, r0, r3
00472e2c: ldr      r3, [r0, #4]
00472e30: add      r3, r3, #1
00472e34: str      r3, [r0, #4]
00472e38: ldr      r0, [r4, #0xc]
00472e3c: mov      r1, #0
00472e40: strb     r1, [r0, #0x138]
00472e44: ldr      r3, [r4, #0xc]
00472e48: mov      r0, r3
00472e4c: ldr      r3, [r3]
00472e50: mov      lr, pc
00472e54: ldr      pc, [r3, #0x48]
00472e58: mov      r0, r4
00472e5c: bl       #0x47211c
00472e60: mov      r0, r4
00472e64: bl       #0x470a54
00472e68: mov      r1, #0
00472e6c: mov      r0, #8
00472e70: bl       #0x310570
00472e74: ldr      r1, [r4, #8]
00472e78: mov      r5, r0
00472e7c: mov      r2, #0
00472e80: bl       #0x474d30
00472e84: mov      r0, r4
00472e88: mov      r1, r5
00472e8c: bl       #0x470a84
00472e90: mov      r0, r4
00472e94: add      sp, sp, #0xc
00472e98: pop      {r4, r5, r6, r7, r8, sl, pc}
00472e9c: subseq   r1, r2, r4, lsr #28
00472ea0: muleq    r0, r4, lr
00472ea4: strdeq   r3, r4, [r0], -r4
00472ea8: ldrdeq   sl, fp, [r5], #-0x80

# _ZN13PhysicalWorld4loadEffff
0034c048: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0034c04c: ldr      r5, [pc, #0xf8]
0034c050: mov      sb, r3
0034c054: ldr      r3, [pc, #0xf4]
0034c058: add      r5, pc, r5
0034c05c: sub      sp, sp, #0x38
0034c060: ldr      r6, [r5, r3]
0034c064: mov      r4, r0
0034c068: mov      sl, r1
0034c06c: ldr      r3, [r6]
0034c070: mov      r8, r2
0034c074: add      r7, sp, #0x1c
0034c078: str      r3, [sp, #0x34]
0034c07c: bl       #0x34be0c
0034c080: ldr      r3, [pc, #0xcc]
0034c084: ldr      r5, [r5, r3]
0034c088: mov      r0, r5
0034c08c: bl       #0x337888
0034c090: ldr      r1, [pc, #0xc0]
0034c094: add      r2, sp, #0x18
0034c098: mov      r0, r7
0034c09c: add      r1, pc, r1
0034c0a0: bl       #0x3140ec
0034c0a4: mov      r1, r7
0034c0a8: mov      r0, r5
0034c0ac: bl       #0x337a88
0034c0b0: mov      r0, r7
0034c0b4: bl       #0x3139ac
0034c0b8: ldr      r2, [sp, #0x58]
0034c0bc: mov      r0, #0x19000
0034c0c0: mov      r3, #0
0034c0c4: mov      r1, #0
0034c0c8: add      r0, r0, #0x278
0034c0cc: str      r2, [sp, #0xc]
0034c0d0: str      r3, [sp, #0x14]
0034c0d4: str      r3, [sp, #0x10]
0034c0d8: str      sl, [sp]
0034c0dc: str      r8, [sp, #4]
0034c0e0: str      sb, [sp, #8]
0034c0e4: bl       #0x310570
0034c0e8: add      r2, sp, #0x10
0034c0ec: mov      r3, #1
0034c0f0: mov      r5, r0
0034c0f4: mov      r1, sp
0034c0f8: bl       #0x7e80b8
0034c0fc: mov      r0, r5
0034c100: mov      r1, r4
0034c104: str      r5, [r4, #0x10]
0034c108: bl       #0x7e65e4
0034c10c: ldr      r0, [r4, #0x10]
0034c110: add      r1, r4, #4
0034c114: bl       #0x7e65f4
0034c118: ldr      r0, [r4, #0x10]
0034c11c: add      r1, r4, #8
0034c120: bl       #0x7e6604
0034c124: add      r1, r4, #0xc
0034c128: ldr      r0, [r4, #0x10]
0034c12c: bl       #0x7e65d4
0034c130: ldr      r2, [sp, #0x34]
0034c134: ldr      r3, [r6]
0034c138: cmp      r2, r3
0034c13c: bne      #0x34c148
0034c140: add      sp, sp, #0x38
0034c144: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0034c148: bl       #0x30e310
0034c14c: rsbeq    r8, r4, r8, lsr sl
0034c150: andeq    r4, r0, ip, lsr #1
0034c154: andeq    r0, r0, r4, lsl #17

# _ZN7Point3DIfE9transformERKN6glitch4core8CMatrix4IfEE
00312da8: push     {r4, lr}
00312dac: ldr      r3, [r0, #8]
00312db0: ldr      r2, [r0, #4]
00312db4: ldr      ip, [r0]
00312db8: sub      sp, sp, #0x10
00312dbc: mov      r4, r0
00312dc0: str      r3, [sp, #8]
00312dc4: mov      r0, r1
00312dc8: mov      r3, #0x3f800000
00312dcc: mov      r1, sp
00312dd0: str      r2, [sp, #4]
00312dd4: str      r3, [sp, #0xc]
00312dd8: str      ip, [sp]
00312ddc: bl       #0x312bf8
00312de0: ldr      r2, [sp, #8]
00312de4: ldr      r3, [sp]
00312de8: ldr      r1, [sp, #4]
00312dec: str      r2, [r4, #8]
00312df0: str      r3, [r4]
00312df4: str      r1, [r4, #4]
00312df8: add      sp, sp, #0x10
00312dfc: pop      {r4, pc}

# _ZN9Container9InitFinalEv
0039fc98: push     {r4, r5, r6, lr}
0039fc9c: mov      r4, r0
0039fca0: bl       #0x38bd64
0039fca4: ldr      r3, [r4, #0x274]
0039fca8: ldr      r5, [pc, #0x80]
0039fcac: cmp      r0, r3
0039fcb0: add      r5, pc, r5
0039fcb4: blt      #0x39fcbc
0039fcb8: pop      {r4, r5, r6, pc}
0039fcbc: mov      r0, r4
0039fcc0: bl       #0x38cd48
0039fcc4: ldr      r3, [r4, #0x394]
0039fcc8: sub      r3, r3, #3
0039fccc: cmp      r3, #1
0039fcd0: bls      #0x39fce4
0039fcd4: ldr      r3, [r4]
0039fcd8: mov      r0, r4
0039fcdc: mov      lr, pc
0039fce0: ldr      pc, [r3, #0x2c]
0039fce4: mov      r0, r4
0039fce8: bl       #0x38ab60
0039fcec: cmp      r0, #0
0039fcf0: beq      #0x39fcb8
0039fcf4: ldr      r3, [pc, #0x38]
0039fcf8: mov      r1, #0
0039fcfc: mov      r0, #0x28
0039fd00: ldr      r3, [r5, r3]
0039fd04: ldr      r6, [r3, #0x44]
0039fd08: bl       #0x310570
0039fd0c: mov      r1, r6
0039fd10: mov      r2, r4
0039fd14: mov      r5, r0
0039fd18: bl       #0x39fc2c
0039fd1c: mov      r0, r4
0039fd20: mov      r1, r5
0039fd24: mov      r2, #0
0039fd28: pop      {r4, r5, r6, lr}
0039fd2c: b        #0x394bf8
0039fd30: subseq   r4, pc, r0, ror #27
0039fd34: strdeq   r3, r4, [r0], -r4

# _ZN12VisualObject12ApplyMeshBoxEv
00470a54: push     {r4, lr}
00470a58: ldr      r3, [r0, #4]
00470a5c: mov      r1, r0
00470a60: cmp      r3, #0
00470a64: beq      #0x470a80
00470a68: mov      r0, r3
00470a6c: ldrb     r2, [r1, #0x28]
00470a70: ldr      r3, [r3]
00470a74: add      r1, r1, #0x10
00470a78: mov      lr, pc
00470a7c: ldr      pc, [r3, #0x9c]
00470a80: pop      {r4, pc}

# _ZN13AnimatedDecor8InitPostEv
00389128: push     {r4, r5, r6, r7, r8, lr}
0038912c: mov      r3, #1
00389130: strb     r3, [r0, #0x10c]
00389134: sub      sp, sp, #8
00389138: mov      r4, r0
0038913c: bl       #0x388a98
00389140: mov      r0, r4
00389144: bl       #0x38ab60
00389148: ldr      r5, [pc, #0x1d8]
0038914c: subs     r1, r0, #0
00389150: add      r5, pc, r5
00389154: beq      #0x3892bc
00389158: ldr      r8, [r4, #0x2d8]
0038915c: cmp      r8, #0
00389160: beq      #0x38923c
00389164: ldr      r7, [r4, #0x390]
00389168: ldr      r3, [r4, #0x38c]
0038916c: cmp      r3, r7
00389170: beq      #0x389308
00389174: ldr      r1, [pc, #0x1b0]
00389178: mov      r0, r7
0038917c: add      r1, pc, r1
00389180: bl       #0x30e6e8
00389184: subs     r6, r0, #0
00389188: beq      #0x389244
0038918c: ldr      r3, [r8, #0x38]
00389190: mov      r1, r7
00389194: mov      r2, #0
00389198: mov      r0, r3
0038919c: ldr      r3, [r3]
003891a0: mov      lr, pc
003891a4: ldr      pc, [r3, #0x14]
003891a8: cmp      r0, #0
003891ac: bne      #0x3892d0
003891b0: ldr      r3, [r4, #0x2d8]
003891b4: mov      lr, #0
003891b8: mov      r1, lr
003891bc: ldr      ip, [r3, #0x38]
003891c0: mov      r2, #1
003891c4: mov      r3, lr
003891c8: mov      r0, ip
003891cc: ldr      ip, [ip]
003891d0: str      lr, [sp]
003891d4: mov      lr, pc
003891d8: ldr      pc, [ip, #0x1c]
003891dc: ldr      r0, [r4, #0x2d8]
003891e0: bl       #0x470a54
003891e4: ldr      r3, [r4, #0x2d8]
003891e8: ldrb     r3, [r3, #0x28]
003891ec: cmp      r3, #0
003891f0: beq      #0x38922c
003891f4: ldr      r3, [pc, #0x134]
003891f8: mov      r1, #0
003891fc: mov      r0, #0x28
00389200: ldr      r3, [r5, r3]
00389204: ldr      r6, [r3, #0x44]
00389208: bl       #0x310570
0038920c: mov      r1, r6
00389210: mov      r5, r0
00389214: mov      r2, r4
00389218: bl       #0x388a2c
0038921c: mov      r0, r4
00389220: mov      r1, r5
00389224: mov      r2, #0
00389228: bl       #0x394bf8
0038922c: mov      r0, r4
00389230: ldr      r3, [r4]
00389234: mov      lr, pc
00389238: ldr      pc, [r3, #0x2c]
0038923c: add      sp, sp, #8
00389240: pop      {r4, r5, r6, r7, r8, pc}
00389244: ldr      r3, [r8, #0x38]
00389248: mov      r1, r6
0038924c: mov      r0, r3
00389250: ldr      r3, [r3]
00389254: mov      lr, pc
00389258: ldr      pc, [r3, #0x10]
0038925c: sub      r0, r0, #1
00389260: bl       #0x388c58
00389264: ldr      r3, [r4, #0x2d8]
00389268: mov      r1, r0
0038926c: mov      r2, r6
00389270: ldr      ip, [r3, #0x38]
00389274: mov      r3, r6
00389278: mov      r0, ip
0038927c: ldr      ip, [ip]
00389280: str      r6, [sp]
00389284: mov      lr, pc
00389288: ldr      pc, [ip, #0x1c]
0038928c: ldr      r2, [r4, #0x2d8]
00389290: mov      r3, r6
00389294: ldr      ip, [r2, #0x38]
00389298: ldr      r2, [pc, #0x94]
0038929c: mov      r0, ip
003892a0: ldr      r1, [r5, r2]
003892a4: ldr      ip, [ip]
003892a8: mov      r2, r4
003892ac: str      r4, [sp]
003892b0: mov      lr, pc
003892b4: ldr      pc, [ip, #0x2c]
003892b8: b        #0x3891dc
003892bc: mov      r0, r4
003892c0: ldr      r3, [r4]
003892c4: mov      lr, pc
003892c8: ldr      pc, [r3, #0x40]
003892cc: b        #0x38923c
003892d0: ldr      r3, [r4, #0x2d8]
003892d4: mov      r2, #0
003892d8: ldr      r1, [r4, #0x390]
003892dc: ldr      ip, [r3, #0x38]
003892e0: mov      r3, r2
003892e4: mov      r0, ip
003892e8: ldr      ip, [ip]
003892ec: str      r2, [sp]
003892f0: mov      r2, #1
003892f4: mov      lr, pc
003892f8: ldr      pc, [ip, #0x20]
003892fc: cmp      r0, #0
00389300: bne      #0x3891dc
00389304: b        #0x3891b0
00389308: ldr      r1, [pc, #0x28]
0038930c: add      r0, r4, #0x37c
00389310: add      r1, pc, r1
00389314: add      r2, r1, #4
00389318: bl       #0x3109e0
0038931c: ldr      r8, [r4, #0x2d8]
00389320: ldr      r7, [r4, #0x390]
00389324: b        #0x389174
00389328: rsbeq    fp, r0, r0, asr #18
0038932c: subseq   sb, r3, ip, lsr r1
00389330: strdeq   r3, r4, [r0], -r4
00389334: ldrdeq   r3, r4, [r0], -r0
00389338: subseq   r8, r3, r0, lsr #31

# _ZN12VisualObject11CalcMeshBoxEv
0047211c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00472120: ldr      r2, [pc, #0x5d4]
00472124: ldr      r3, [r0, #0xc]
00472128: sub      sp, sp, #0x64
0047212c: add      r2, pc, r2
00472130: cmp      r3, #0
00472134: str      r2, [sp, #8]
00472138: mov      r4, r0
0047213c: beq      #0x472408
00472140: mov      r0, r3
00472144: ldr      r3, [r3]
00472148: mov      lr, pc
0047214c: ldr      pc, [r3, #0x30]
00472150: ldr      r1, [r0]
00472154: mov      r3, r0
00472158: ldr      r2, [r4, #0xc]
0047215c: str      r1, [r4, #0x10]
00472160: ldr      r1, [r0, #4]
00472164: mov      r0, r2
00472168: str      r1, [r4, #0x14]
0047216c: ldr      r3, [r3, #8]
00472170: str      r3, [r4, #0x18]
00472174: ldr      r3, [r2]
00472178: mov      lr, pc
0047217c: ldr      pc, [r3, #0x30]
00472180: ldr      r2, [r0, #0xc]
00472184: mov      r3, r0
00472188: ldr      r0, [r4, #0xc]
0047218c: str      r2, [r4, #0x1c]
00472190: ldr      r2, [r3, #0x10]
00472194: str      r2, [r4, #0x20]
00472198: ldr      r3, [r3, #0x14]
0047219c: str      r3, [r4, #0x24]
004721a0: bl       #0x597290
004721a4: ldr      r3, [r0]
004721a8: mov      lr, pc
004721ac: ldr      pc, [r3, #0x90]
004721b0: mov      r3, r0
004721b4: ldr      r1, [r0]
004721b8: ldr      r0, [r4, #0x10]
004721bc: ldr      r6, [r3, #4]
004721c0: ldr      r5, [r3, #8]
004721c4: bl       #0x30ed6c
004721c8: mov      r1, r6
004721cc: str      r0, [r4, #0x10]
004721d0: ldr      r0, [r4, #0x14]
004721d4: bl       #0x30ed6c
004721d8: mov      r1, r5
004721dc: str      r0, [r4, #0x14]
004721e0: ldr      r0, [r4, #0x18]
004721e4: bl       #0x30ed6c
004721e8: str      r0, [r4, #0x18]
004721ec: ldr      r0, [r4, #0xc]
004721f0: bl       #0x597290
004721f4: ldr      r3, [r0]
004721f8: mov      lr, pc
004721fc: ldr      pc, [r3, #0x90]
00472200: mov      r3, r0
00472204: ldr      r1, [r0]
00472208: ldr      r0, [r4, #0x1c]
0047220c: ldr      r6, [r3, #4]
00472210: ldr      r5, [r3, #8]
00472214: bl       #0x30ed6c
00472218: mov      r1, r6
0047221c: str      r0, [r4, #0x1c]
00472220: ldr      r0, [r4, #0x20]
00472224: bl       #0x30ed6c
00472228: mov      r1, r5
0047222c: str      r0, [r4, #0x20]
00472230: ldr      r0, [r4, #0x24]
00472234: bl       #0x30ed6c
00472238: str      r0, [r4, #0x24]
0047223c: ldr      r3, [r4, #8]
00472240: add      r5, sp, #0x10
00472244: mov      r6, #0
00472248: mov      r0, r3
0047224c: ldr      r3, [r3]
00472250: mov      lr, pc
00472254: ldr      pc, [r3, #0x40]
00472258: mov      r2, #0x41
0047225c: mov      r1, r0
00472260: mov      r0, r5
00472264: strb     r6, [sp, #0x50]
00472268: bl       #0x30e868
0047226c: mov      r3, #0
00472270: mov      r1, r5
00472274: add      r0, r4, #0x10
00472278: str      r3, [sp, #0x48]
0047227c: str      r3, [sp, #0x40]
00472280: str      r3, [sp, #0x44]
00472284: strb     r6, [sp, #0x50]
00472288: bl       #0x312da8
0047228c: mov      r1, r5
00472290: add      r0, r4, #0x1c
00472294: bl       #0x312da8
00472298: ldr      r7, [r4, #0x1c]
0047229c: ldr      r5, [r4, #0x10]
004722a0: mov      r0, r7
004722a4: mov      r1, r5
004722a8: bl       #0x30e70c
004722ac: mov      r1, r5
004722b0: cmp      r0, r6
004722b4: mov      r0, r7
004722b8: movne    fp, r7
004722bc: moveq    fp, r5
004722c0: bl       #0x30e2f8
004722c4: cmp      r0, #0
004722c8: ldr      r6, [r4, #0x20]
004722cc: moveq    r7, r5
004722d0: ldr      r5, [r4, #0x14]
004722d4: mov      r0, r6
004722d8: str      r7, [r4, #0x1c]
004722dc: mov      r1, r5
004722e0: str      fp, [r4, #0x10]
004722e4: bl       #0x30e70c
004722e8: mov      r1, r5
004722ec: cmp      r0, #0
004722f0: mov      r0, r6
004722f4: movne    sb, r6
004722f8: moveq    sb, r5
004722fc: bl       #0x30e2f8
00472300: cmp      r0, #0
00472304: ldr      r8, [r4, #0x18]
00472308: moveq    r6, r5
0047230c: ldr      r5, [r4, #0x24]
00472310: mov      r1, r8
00472314: str      r6, [r4, #0x20]
00472318: mov      r0, r5
0047231c: str      sb, [r4, #0x14]
00472320: bl       #0x30e70c
00472324: mov      r1, r8
00472328: cmp      r0, #0
0047232c: mov      r0, r5
00472330: movne    sl, r5
00472334: moveq    sl, r8
00472338: bl       #0x30e2f8
0047233c: cmp      r0, #0
00472340: moveq    r5, r8
00472344: mov      r1, fp
00472348: str      r5, [r4, #0x24]
0047234c: mov      r0, r7
00472350: str      sl, [r4, #0x18]
00472354: bl       #0x30e3ac
00472358: mov      r1, #0x3f000000
0047235c: bl       #0x30ed6c
00472360: mov      r1, sb
00472364: mov      r7, r0
00472368: mov      r0, r6
0047236c: bl       #0x30e3ac
00472370: mov      r1, #0x3f000000
00472374: bl       #0x30ed6c
00472378: mov      r1, sl
0047237c: mov      r8, r0
00472380: mov      r0, r5
00472384: bl       #0x30e3ac
00472388: mov      r1, #0x3f000000
0047238c: bl       #0x30ed6c
00472390: ldr      ip, [sp, #8]
00472394: ldr      r3, [pc, #0x364]
00472398: mov      r6, r0
0047239c: mov      r1, r7
004723a0: ldr      r5, [ip, r3]
004723a4: ldr      r0, [r5]
004723a8: bl       #0x30e3ac
004723ac: str      r0, [r4, #0x10]
004723b0: ldr      r1, [r5]
004723b4: mov      r0, r7
004723b8: bl       #0x30eba4
004723bc: str      r0, [r4, #0x1c]
004723c0: ldr      r0, [r5, #4]
004723c4: mov      r1, r8
004723c8: bl       #0x30e3ac
004723cc: str      r0, [r4, #0x14]
004723d0: ldr      r1, [r5, #4]
004723d4: mov      r0, r8
004723d8: bl       #0x30eba4
004723dc: str      r0, [r4, #0x20]
004723e0: ldr      r0, [r5, #8]
004723e4: mov      r1, r6
004723e8: bl       #0x30e3ac
004723ec: str      r0, [r4, #0x18]
004723f0: ldr      r1, [r5, #8]
004723f4: mov      r0, r6
004723f8: bl       #0x30eba4
004723fc: str      r0, [r4, #0x24]
00472400: add      sp, sp, #0x64
00472404: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00472408: ldr      ip, [sp, #8]
0047240c: ldr      r0, [pc, #0x2f0]
00472410: mvn      r1, #0x80000000
00472414: sub      r1, r1, #0x800000
00472418: ldr      r5, [ip, r0]
0047241c: mvn      r2, #0x800000
00472420: str      r1, [r4, #0x18]
00472424: str      r1, [r4, #0x10]
00472428: str      r1, [r4, #0x14]
0047242c: str      r2, [r4, #0x24]
00472430: str      r2, [r4, #0x1c]
00472434: str      r2, [r4, #0x20]
00472438: ldr      r2, [r5, #0x10]
0047243c: str      r3, [sp, #0x5c]
00472440: str      r3, [sp, #0x54]
00472444: str      r3, [sp, #0x58]
00472448: ldr      r3, [r2, #0x1c]
0047244c: add      r6, sp, #0x54
00472450: movw     r1, #0x6164
00472454: mov      r0, r3
00472458: ldr      ip, [r3]
0047245c: movt     r1, #0x7365
00472460: ldr      r3, [r4, #8]
00472464: mov      r2, r6
00472468: mov      lr, pc
0047246c: ldr      pc, [ip, #0x20]
00472470: ldr      r0, [sp, #0x54]
00472474: ldr      r3, [sp, #0x58]
00472478: rsb      r3, r0, r3
0047247c: asrs     r3, r3, #2
00472480: str      r3, [sp, #0xc]
00472484: beq      #0x472690
00472488: ldr      r2, [sp, #0xc]
0047248c: cmp      r2, #0
00472490: beq      #0x472680
00472494: mov      r5, #0
00472498: b        #0x4724a0
0047249c: ldr      r0, [sp, #0x54]
004724a0: ldr      r3, [r0, r5, lsl #2]
004724a4: mov      r0, r3
004724a8: ldr      r3, [r3]
004724ac: mov      lr, pc
004724b0: ldr      pc, [r3, #0x30]
004724b4: ldr      r3, [sp, #0x54]
004724b8: ldr      sl, [r0, #8]
004724bc: ldr      fp, [r0]
004724c0: ldr      r3, [r3, r5, lsl #2]
004724c4: ldr      sb, [r0, #4]
004724c8: mov      r0, r3
004724cc: ldr      r3, [r3]
004724d0: mov      lr, pc
004724d4: ldr      pc, [r3, #0x30]
004724d8: ldr      r2, [sp, #0x54]
004724dc: mov      r3, r0
004724e0: ldr      r6, [r0, #0x14]
004724e4: ldr      r8, [r0, #0xc]
004724e8: ldr      r0, [r2, r5, lsl #2]
004724ec: ldr      r7, [r3, #0x10]
004724f0: bl       #0x597290
004724f4: cmp      r0, #0
004724f8: beq      #0x4725bc
004724fc: ldr      r3, [sp, #0x54]
00472500: ldr      r0, [r3, r5, lsl #2]
00472504: bl       #0x597290
00472508: ldr      r3, [r0]
0047250c: mov      lr, pc
00472510: ldr      pc, [r3, #0x90]
00472514: mov      r2, r0
00472518: ldr      r3, [r2, #4]
0047251c: ldr      r2, [r2, #8]
00472520: ldr      r1, [r0]
00472524: mov      r0, fp
00472528: stm      sp, {r2, r3}
0047252c: bl       #0x30ed6c
00472530: ldr      r3, [sp, #4]
00472534: mov      fp, r0
00472538: mov      r0, sb
0047253c: mov      r1, r3
00472540: bl       #0x30ed6c
00472544: ldr      r2, [sp]
00472548: mov      sb, r0
0047254c: mov      r0, sl
00472550: mov      r1, r2
00472554: bl       #0x30ed6c
00472558: ldr      r3, [sp, #0x54]
0047255c: mov      sl, r0
00472560: ldr      r0, [r3, r5, lsl #2]
00472564: bl       #0x597290
00472568: ldr      r3, [r0]
0047256c: mov      lr, pc
00472570: ldr      pc, [r3, #0x90]
00472574: mov      r2, r0
00472578: ldr      r3, [r2, #4]
0047257c: ldr      r2, [r2, #8]
00472580: ldr      r1, [r0]
00472584: mov      r0, r8
00472588: stm      sp, {r2, r3}
0047258c: bl       #0x30ed6c
00472590: ldr      r3, [sp, #4]
00472594: mov      r8, r0
00472598: mov      r0, r7
0047259c: mov      r1, r3
004725a0: bl       #0x30ed6c
004725a4: ldr      r2, [sp]
004725a8: mov      r7, r0
004725ac: mov      r0, r6
004725b0: mov      r1, r2
004725b4: bl       #0x30ed6c
004725b8: mov      r6, r0
004725bc: ldr      r3, [r4, #0x10]
004725c0: mov      r1, fp
004725c4: add      r5, r5, #1
004725c8: mov      r0, r3
004725cc: str      r3, [sp, #4]
004725d0: bl       #0x30e2f8
004725d4: cmp      r0, #0
004725d8: ldr      r3, [sp, #4]
004725dc: movne    r3, fp
004725e0: ldr      fp, [r4, #0x14]
004725e4: str      r3, [r4, #0x10]
004725e8: mov      r1, sb
004725ec: mov      r0, fp
004725f0: bl       #0x30e2f8
004725f4: cmp      r0, #0
004725f8: movne    fp, sb
004725fc: ldr      sb, [r4, #0x18]
00472600: mov      r1, sl
00472604: str      fp, [r4, #0x14]
00472608: mov      r0, sb
0047260c: bl       #0x30e2f8
00472610: cmp      r0, #0
00472614: movne    sb, sl
00472618: ldr      sl, [r4, #0x1c]
0047261c: mov      r1, r8
00472620: str      sb, [r4, #0x18]
00472624: mov      r0, sl
00472628: bl       #0x30e70c
0047262c: cmp      r0, #0
00472630: movne    sl, r8
00472634: ldr      r8, [r4, #0x20]
00472638: mov      r1, r7
0047263c: str      sl, [r4, #0x1c]
00472640: mov      r0, r8
00472644: bl       #0x30e70c
00472648: cmp      r0, #0
0047264c: movne    r8, r7
00472650: ldr      r7, [r4, #0x24]
00472654: str      r8, [r4, #0x20]
00472658: mov      r1, r6
0047265c: mov      r0, r7
00472660: bl       #0x30e70c
00472664: ldr      r3, [sp, #0xc]
00472668: cmp      r0, #0
0047266c: movne    r7, r6
00472670: cmp      r5, r3
00472674: str      r7, [r4, #0x24]
00472678: bne      #0x47249c
0047267c: ldr      r0, [sp, #0x54]
00472680: cmp      r0, #0
00472684: beq      #0x47223c
00472688: bl       #0x310450
0047268c: b        #0x47223c
00472690: ldr      r3, [r5, #0x10]
00472694: movw     r1, #0x6164
00472698: movt     r1, #0x6d65
0047269c: ldr      ip, [r3, #0x1c]
004726a0: mov      r2, r6
004726a4: ldr      r3, [r4, #8]
004726a8: mov      r0, ip
004726ac: ldr      ip, [ip]
004726b0: mov      lr, pc
004726b4: ldr      pc, [ip, #0x20]
004726b8: ldr      r0, [sp, #0x54]
004726bc: ldr      r3, [sp, #0x58]
004726c0: rsb      r3, r0, r3
004726c4: asrs     r3, r3, #2
004726c8: str      r3, [sp, #0xc]
004726cc: bne      #0x472488
004726d0: mov      r3, #0
004726d4: cmp      r0, #0
004726d8: str      r3, [r4, #0x24]
004726dc: str      r3, [r4, #0x10]
004726e0: str      r3, [r4, #0x14]
004726e4: str      r3, [r4, #0x18]
004726e8: str      r3, [r4, #0x1c]
004726ec: str      r3, [r4, #0x20]
004726f0: beq      #0x472400
004726f4: bl       #0x310450
004726f8: b        #0x472400
004726fc: subseq   r2, r2, r4, ror #18
00472700: andeq    r3, r0, ip, lsr #30
00472704: strdeq   r3, r4, [r0], -r4

# _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti
0046f2f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046f2f4: ldr      r5, [pc, #0x360]
0046f2f8: ldr      r4, [pc, #0x360]
0046f2fc: sub      sp, sp, #0xb4
0046f300: add      r5, pc, r5
0046f304: str      r4, [sp, #0x10]
0046f308: ldr      lr, [r5, r4]
0046f30c: ldr      ip, [pc, #0x350]
0046f310: ldr      r4, [pc, #0x350]
0046f314: ldrb     sb, [sp, #0xd8]
0046f318: ldr      ip, [r5, ip]
0046f31c: ldr      r4, [r5, r4]
0046f320: ldr      lr, [lr]
0046f324: mov      r7, #0
0046f328: str      r4, [sp, #0xc]
0046f32c: add      ip, ip, #8
0046f330: mov      r4, r0
0046f334: mov      sl, #0
0046f338: str      r1, [r0, #4]
0046f33c: str      ip, [r0]
0046f340: str      r2, [r4, #8]
0046f344: str      sl, [r0, #0xc]
0046f348: strb     sb, [r0, #0x10]
0046f34c: str      r7, [r0, #0x14]
0046f350: str      r7, [r0, #0x18]
0046f354: str      r7, [r0, #0x1c]
0046f358: strb     r7, [r0, #0x26]
0046f35c: strb     r7, [r0, #0x27]
0046f360: ldrb     ip, [sp, #0xdc]
0046f364: mov      r6, r2
0046f368: str      lr, [sp, #0xac]
0046f36c: ldrh     r2, [sp, #0xe8]
0046f370: ldrb     lr, [sp, #0xe0]
0046f374: str      r3, [sp, #0x1c]
0046f378: ldrh     r3, [sp, #0xec]
0046f37c: ldr      r0, [sp, #0xc]
0046f380: str      ip, [sp, #0x20]
0046f384: str      lr, [sp, #0x24]
0046f388: str      r3, [sp, #0x18]
0046f38c: ldrsh    r8, [sp, #0xe4]
0046f390: str      r2, [sp, #0x14]
0046f394: bl       #0x337888
0046f398: ldr      r1, [pc, #0x2cc]
0046f39c: add      fp, sp, #0x94
0046f3a0: add      r2, sp, #0x90
0046f3a4: add      r1, pc, r1
0046f3a8: mov      r0, fp
0046f3ac: bl       #0x3140ec
0046f3b0: mov      r1, fp
0046f3b4: ldr      r0, [sp, #0xc]
0046f3b8: bl       #0x337a88
0046f3bc: mov      r3, r0
0046f3c0: mov      r0, fp
0046f3c4: str      r3, [sp, #8]
0046f3c8: bl       #0x3139ac
0046f3cc: ldr      r3, [sp, #8]
0046f3d0: mvn      r2, #0x298
0046f3d4: sub      r2, r2, #1
0046f3d8: cmp      r3, r7
0046f3dc: movne    r8, r2
0046f3e0: cmp      r6, r7
0046f3e4: beq      #0x46f558
0046f3e8: cmp      sb, r7
0046f3ec: bne      #0x46f57c
0046f3f0: ldr      r3, [pc, #0x278]
0046f3f4: mov      r2, #1
0046f3f8: ldr      r1, [r6, #0x12c]
0046f3fc: ldr      r3, [r5, r3]
0046f400: ldr      r0, [r6, #0x138]
0046f404: str      r2, [sp, #0x30]
0046f408: add      ip, r3, #8
0046f40c: movw     r3, #0xcccd
0046f410: movt     r3, #0x3e4c
0046f414: strh     r2, [sp, #0x46]
0046f418: mvn      r2, #0
0046f41c: str      ip, [sp, #0x2c]
0046f420: strh     r2, [sp, #0x48]
0046f424: str      r3, [sp, #0x38]
0046f428: str      sl, [sp, #0x40]
0046f42c: str      sb, [sp, #0x8c]
0046f430: str      sb, [sp, #0x34]
0046f434: str      sl, [sp, #0x3c]
0046f438: strh     sb, [sp, #0x4a]
0046f43c: strb     sb, [sp, #0x44]
0046f440: bl       #0x30e3ac
0046f444: movw     r1, #0xd70a
0046f448: movt     r1, #0x3c23
0046f44c: bl       #0x30ed6c
0046f450: ldr      r1, [r6, #0x130]
0046f454: mov      sb, r0
0046f458: ldr      r0, [r6, #0x13c]
0046f45c: bl       #0x30e3ac
0046f460: movw     r1, #0xd70a
0046f464: movt     r1, #0x3c23
0046f468: bl       #0x30ed6c
0046f46c: movw     r1, #0xd70a
0046f470: mov      r7, r0
0046f474: movt     r1, #0x3c23
0046f478: ldr      r0, [r6, #0x160]
0046f47c: bl       #0x30ed6c
0046f480: movw     r1, #0xd70a
0046f484: movt     r1, #0x3c23
0046f488: mov      sl, r0
0046f48c: ldr      r0, [r6, #0x164]
0046f490: bl       #0x30ed6c
0046f494: mov      r1, #0x3f000000
0046f498: mov      r6, r0
0046f49c: mov      r0, sb
0046f4a0: bl       #0x30ed6c
0046f4a4: mov      r1, #0x3f000000
0046f4a8: mov      r3, r0
0046f4ac: mov      r0, r7
0046f4b0: str      r3, [sp, #8]
0046f4b4: bl       #0x30ed6c
0046f4b8: ldr      r3, [sp, #8]
0046f4bc: add      fp, sp, #0x2c
0046f4c0: mov      r2, r0
0046f4c4: mov      r1, r3
0046f4c8: mov      r0, fp
0046f4cc: bl       #0x7e44b8
0046f4d0: mov      r1, r7
0046f4d4: mov      r0, sb
0046f4d8: bl       #0x30e70c
0046f4dc: cmp      r0, #0
0046f4e0: moveq    r7, sb
0046f4e4: mov      r0, r7
0046f4e8: mov      r1, #0x3f000000
0046f4ec: bl       #0x30ed6c
0046f4f0: ldr      r3, [pc, #0x17c]
0046f4f4: str      r0, [r4, #0xc]
0046f4f8: mov      ip, fp
0046f4fc: ldr      r3, [r5, r3]
0046f500: add      r3, r3, #8
0046f504: str      r3, [sp, #0x2c]
0046f508: strh     r8, [r4, #0x24]
0046f50c: ldr      lr, [sp, #0x14]
0046f510: mov      r1, ip
0046f514: mov      r3, r6
0046f518: strh     lr, [r4, #0x20]
0046f51c: ldr      r2, [sp, #0x18]
0046f520: mov      r0, r4
0046f524: strh     r2, [r4, #0x22]
0046f528: ldr      lr, [sp, #0x20]
0046f52c: strh     r8, [ip, #0x1e]
0046f530: mov      r2, sl
0046f534: strb     lr, [ip, #0x18]
0046f538: ldr      lr, [sp, #0x14]
0046f53c: strh     lr, [ip, #0x1a]
0046f540: ldr      lr, [sp, #0x18]
0046f544: strh     lr, [ip, #0x1c]
0046f548: ldr      ip, [sp, #0x1c]
0046f54c: ldr      lr, [sp, #0x24]
0046f550: stm      sp, {ip, lr}
0046f554: bl       #0x46edf0
0046f558: ldr      ip, [sp, #0x10]
0046f55c: ldr      r2, [sp, #0xac]
0046f560: mov      r0, r4
0046f564: ldr      r3, [r5, ip]
0046f568: ldr      r3, [r3]
0046f56c: cmp      r2, r3
0046f570: bne      #0x46f658
0046f574: add      sp, sp, #0xb4
0046f578: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f57c: movw     r3, #0xcccd
0046f580: ldr      r1, [r6, #0x12c]
0046f584: ldr      r0, [r6, #0x138]
0046f588: movt     r3, #0x3e4c
0046f58c: mov      ip, #1
0046f590: mvn      lr, #0
0046f594: str      r3, [sp, #0x38]
0046f598: strh     ip, [sp, #0x46]
0046f59c: strh     lr, [sp, #0x48]
0046f5a0: strb     r7, [sp, #0x44]
0046f5a4: str      r7, [sp, #0x30]
0046f5a8: str      sl, [sp, #0x50]
0046f5ac: str      r7, [sp, #0x34]
0046f5b0: str      sl, [sp, #0x3c]
0046f5b4: str      sl, [sp, #0x40]
0046f5b8: strh     r7, [sp, #0x4a]
0046f5bc: str      sl, [sp, #0x4c]
0046f5c0: bl       #0x30e3ac
0046f5c4: movw     r1, #0xd70a
0046f5c8: movt     r1, #0x3c23
0046f5cc: bl       #0x30ed6c
0046f5d0: ldr      r1, [r6, #0x130]
0046f5d4: mov      sb, r0
0046f5d8: ldr      r0, [r6, #0x13c]
0046f5dc: bl       #0x30e3ac
0046f5e0: movw     r1, #0xd70a
0046f5e4: movt     r1, #0x3c23
0046f5e8: bl       #0x30ed6c
0046f5ec: movw     r1, #0xd70a
0046f5f0: mov      r7, r0
0046f5f4: movt     r1, #0x3c23
0046f5f8: ldr      r0, [r6, #0x160]
0046f5fc: bl       #0x30ed6c
0046f600: movw     r1, #0xd70a
0046f604: movt     r1, #0x3c23
0046f608: mov      sl, r0
0046f60c: ldr      r0, [r6, #0x164]
0046f610: bl       #0x30ed6c
0046f614: mov      r1, r7
0046f618: mov      r6, r0
0046f61c: mov      r0, sb
0046f620: bl       #0x30e70c
0046f624: cmp      r0, #0
0046f628: moveq    r7, sb
0046f62c: mov      r0, r7
0046f630: mov      r1, #0x3f000000
0046f634: bl       #0x30ed6c
0046f638: ldr      r3, [pc, #0x34]
0046f63c: add      ip, sp, #0xb0
0046f640: str      r0, [r4, #0xc]
0046f644: ldr      r3, [r5, r3]
0046f648: str      r0, [sp, #0x54]
0046f64c: add      r3, r3, #8
0046f650: str      r3, [ip, #-0x84]!
0046f654: b        #0x46f508
0046f658: bl       #0x30e310

# _ZN10GameObject15SetRelativeAABBERK4aabbIfEb
0038b110: push     {r4, r5, r6, r7, r8, lr}
0038b114: ldr      r5, [r1]
0038b118: mov      r3, r1
0038b11c: mov      r4, r0
0038b120: str      r5, [r0, #0x144]
0038b124: ldr      r7, [r1, #4]
0038b128: mov      r1, r5
0038b12c: str      r7, [r0, #0x148]
0038b130: ldr      r2, [r3, #8]
0038b134: str      r2, [r0, #0x14c]
0038b138: ldr      r0, [r3, #0xc]
0038b13c: str      r0, [r4, #0x150]
0038b140: ldr      r6, [r3, #0x10]
0038b144: str      r6, [r4, #0x154]
0038b148: ldr      r3, [r3, #0x14]
0038b14c: str      r3, [r4, #0x158]
0038b150: bl       #0x30e3ac
0038b154: mov      r1, #0
0038b158: mov      r8, r0
0038b15c: bl       #0x30df8c
0038b160: cmp      r0, #0
0038b164: beq      #0x38b188
0038b168: mov      r1, r7
0038b16c: mov      r0, r6
0038b170: bl       #0x30e3ac
0038b174: mov      r1, #0
0038b178: bl       #0x30df8c
0038b17c: cmp      r0, #0
0038b180: movne    r3, #1
0038b184: strbne   r3, [r4, #0x2f9]
0038b188: mov      r1, #0x41000000
0038b18c: mov      r0, r8
0038b190: add      r1, r1, #0x200000
0038b194: bl       #0x30e70c
0038b198: cmp      r0, #0
0038b19c: beq      #0x38b1c8
0038b1a0: mov      r1, #0x40000000
0038b1a4: add      r1, r1, #0xa00000
0038b1a8: mov      r0, r5
0038b1ac: bl       #0x30e3ac
0038b1b0: mov      r1, #0x40000000
0038b1b4: str      r0, [r4, #0x144]
0038b1b8: add      r1, r1, #0xa00000
0038b1bc: ldr      r0, [r4, #0x150]
0038b1c0: bl       #0x30eba4
0038b1c4: str      r0, [r4, #0x150]
0038b1c8: ldr      r5, [r4, #0x148]
0038b1cc: ldr      r0, [r4, #0x154]
0038b1d0: mov      r1, r5
0038b1d4: bl       #0x30e3ac
0038b1d8: mov      r1, #0x41000000
0038b1dc: add      r1, r1, #0x200000
0038b1e0: bl       #0x30e70c
0038b1e4: cmp      r0, #0
0038b1e8: beq      #0x38b214
0038b1ec: mov      r1, #0x40000000
0038b1f0: add      r1, r1, #0xa00000
0038b1f4: mov      r0, r5
0038b1f8: bl       #0x30e3ac
0038b1fc: mov      r1, #0x40000000
0038b200: str      r0, [r4, #0x148]
0038b204: add      r1, r1, #0xa00000
0038b208: ldr      r0, [r4, #0x154]
0038b20c: bl       #0x30eba4
0038b210: str      r0, [r4, #0x154]
0038b214: mov      r0, r4
0038b218: bl       #0x38aac8
0038b21c: mov      r0, r4
0038b220: pop      {r4, r5, r6, r7, r8, lr}
0038b224: b        #0x393ea0

# _ZN14PhysicalObject3pinEv
0046eb20: str      lr, [sp, #-4]!
0046eb24: ldrb     r3, [r0, #0x27]
0046eb28: sub      sp, sp, #0x14
0046eb2c: cmp      r3, #0
0046eb30: bne      #0x46eb68
0046eb34: ldr      r2, [r0, #0x14]
0046eb38: mov      r3, #1
0046eb3c: strb     r3, [r0, #0x27]
0046eb40: ldr      r1, [r2, #0x1c]
0046eb44: mov      r0, r2
0046eb48: mov      r3, #0
0046eb4c: str      r1, [sp, #4]
0046eb50: ldr      r2, [r2, #0x20]
0046eb54: mov      r1, sp
0046eb58: str      r3, [sp, #0xc]
0046eb5c: str      r2, [sp, #8]
0046eb60: str      r3, [sp]
0046eb64: bl       #0x7e1b28
0046eb68: add      sp, sp, #0x14
0046eb6c: ldm      sp!, {pc}

# _ZN10GameObject18UpdateAbsoluteAABBEv
0038aac8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0038aacc: mov      r4, r0
0038aad0: ldr      sl, [r4, #0x148]
0038aad4: ldr      r0, [r0, #0x144]
0038aad8: ldr      r8, [r4, #0x14c]
0038aadc: ldr      r7, [r4, #0x150]
0038aae0: ldr      r6, [r4, #0x154]
0038aae4: ldr      r5, [r4, #0x158]
0038aae8: ldr      r1, [r4, #0x160]
0038aaec: str      r0, [r4, #0x12c]
0038aaf0: str      sl, [r4, #0x130]
0038aaf4: str      r8, [r4, #0x134]
0038aaf8: str      r7, [r4, #0x138]
0038aafc: str      r6, [r4, #0x13c]
0038ab00: str      r5, [r4, #0x140]
0038ab04: bl       #0x30eba4
0038ab08: ldr      r1, [r4, #0x164]
0038ab0c: str      r0, [r4, #0x12c]
0038ab10: mov      r0, sl
0038ab14: bl       #0x30eba4
0038ab18: ldr      r1, [r4, #0x168]
0038ab1c: str      r0, [r4, #0x130]
0038ab20: mov      r0, r8
0038ab24: bl       #0x30eba4
0038ab28: ldr      r1, [r4, #0x160]
0038ab2c: str      r0, [r4, #0x134]
0038ab30: mov      r0, r7
0038ab34: bl       #0x30eba4
0038ab38: ldr      r1, [r4, #0x164]
0038ab3c: str      r0, [r4, #0x138]
0038ab40: mov      r0, r6
0038ab44: bl       #0x30eba4
0038ab48: ldr      r1, [r4, #0x168]
0038ab4c: str      r0, [r4, #0x13c]
0038ab50: mov      r0, r5
0038ab54: bl       #0x30eba4
0038ab58: str      r0, [r4, #0x140]
0038ab5c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN5Decor8InitPostEv
00388a98: push     {r4, r5, r6, lr}
00388a9c: mov      r4, r0
00388aa0: bl       #0x38be5c
00388aa4: ldr      r0, [r4, #0x2d8]
00388aa8: ldr      r5, [pc, #0x68]
00388aac: cmp      r0, #0
00388ab0: add      r5, pc, r5
00388ab4: beq      #0x388b14
00388ab8: bl       #0x470a54
00388abc: ldr      r3, [r4, #0x2d8]
00388ac0: ldrb     r3, [r3, #0x28]
00388ac4: cmp      r3, #0
00388ac8: bne      #0x388ad8
00388acc: mov      r0, r4
00388ad0: pop      {r4, r5, r6, lr}
00388ad4: b        #0x388730
00388ad8: ldr      r3, [pc, #0x3c]
00388adc: mov      r1, #0
00388ae0: mov      r0, #0x28
00388ae4: ldr      r3, [r5, r3]
00388ae8: ldr      r6, [r3, #0x44]
00388aec: bl       #0x310570
00388af0: mov      r1, r6
00388af4: mov      r5, r0
00388af8: mov      r2, r4
00388afc: bl       #0x388a2c
00388b00: mov      r0, r4
00388b04: mov      r1, r5
00388b08: mov      r2, #0
00388b0c: bl       #0x394bf8
00388b10: b        #0x388acc
00388b14: pop      {r4, r5, r6, pc}
00388b18: rsbeq    fp, r0, r0, ror #31
00388b1c: strdeq   r3, r4, [r0], -r4

# _ZNK6glitch4core8CMatrix4IfE21multiplyWith1x4MatrixEPf
00312bf8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00312bfc: ldr      sl, [r1]
00312c00: mov      r4, r0
00312c04: ldr      r8, [r1, #4]
00312c08: mov      r5, r1
00312c0c: ldr      r1, [r0]
00312c10: mov      r0, sl
00312c14: bl       #0x30ed6c
00312c18: ldr      r1, [r4, #0x10]
00312c1c: mov      r6, r0
00312c20: mov      r0, r8
00312c24: bl       #0x30ed6c
00312c28: mov      r1, r0
00312c2c: mov      r0, r6
00312c30: bl       #0x30eba4
00312c34: ldr      r7, [r5, #8]
00312c38: ldr      r1, [r4, #0x20]
00312c3c: mov      sb, r0
00312c40: mov      r0, r7
00312c44: bl       #0x30ed6c
00312c48: mov      r1, r0
00312c4c: mov      r0, sb
00312c50: bl       #0x30eba4
00312c54: ldr      r6, [r5, #0xc]
00312c58: ldr      r1, [r4, #0x30]
00312c5c: mov      sb, r0
00312c60: mov      r0, r6
00312c64: bl       #0x30ed6c
00312c68: mov      r1, r0
00312c6c: mov      r0, sb
00312c70: bl       #0x30eba4
00312c74: str      r0, [r5]
00312c78: ldr      r1, [r4, #4]
00312c7c: mov      r0, sl
00312c80: bl       #0x30ed6c
00312c84: ldr      r1, [r4, #0x14]
00312c88: mov      sb, r0
00312c8c: mov      r0, r8
00312c90: bl       #0x30ed6c
00312c94: mov      r1, r0
00312c98: mov      r0, sb
00312c9c: bl       #0x30eba4
00312ca0: ldr      r1, [r4, #0x24]
00312ca4: mov      sb, r0
00312ca8: mov      r0, r7
00312cac: bl       #0x30ed6c
00312cb0: mov      r1, r0
00312cb4: mov      r0, sb
00312cb8: bl       #0x30eba4
00312cbc: ldr      r1, [r4, #0x34]
00312cc0: mov      sb, r0
00312cc4: mov      r0, r6
00312cc8: bl       #0x30ed6c
00312ccc: mov      r1, r0
00312cd0: mov      r0, sb
00312cd4: bl       #0x30eba4
00312cd8: str      r0, [r5, #4]
00312cdc: ldr      r1, [r4, #8]
00312ce0: mov      r0, sl
00312ce4: bl       #0x30ed6c
00312ce8: ldr      r1, [r4, #0x18]
00312cec: mov      sb, r0
00312cf0: mov      r0, r8
00312cf4: bl       #0x30ed6c
00312cf8: mov      r1, r0
00312cfc: mov      r0, sb
00312d00: bl       #0x30eba4
00312d04: ldr      r1, [r4, #0x28]
00312d08: mov      sb, r0
00312d0c: mov      r0, r7
00312d10: bl       #0x30ed6c
00312d14: mov      r1, r0
00312d18: mov      r0, sb
00312d1c: bl       #0x30eba4
00312d20: ldr      r1, [r4, #0x38]
00312d24: mov      sb, r0
00312d28: mov      r0, r6
00312d2c: bl       #0x30ed6c
00312d30: mov      r1, r0
00312d34: mov      r0, sb
00312d38: bl       #0x30eba4
00312d3c: str      r0, [r5, #8]
00312d40: ldr      r1, [r4, #0xc]
00312d44: mov      r0, sl
00312d48: bl       #0x30ed6c
00312d4c: ldr      r1, [r4, #0x1c]
00312d50: mov      sl, r0
00312d54: mov      r0, r8
00312d58: bl       #0x30ed6c
00312d5c: mov      r1, r0
00312d60: mov      r0, sl
00312d64: bl       #0x30eba4
00312d68: ldr      r1, [r4, #0x2c]
00312d6c: mov      r8, r0
00312d70: mov      r0, r7
00312d74: bl       #0x30ed6c
00312d78: mov      r1, r0
00312d7c: mov      r0, r8
00312d80: bl       #0x30eba4
00312d84: ldr      r1, [r4, #0x3c]
00312d88: mov      r7, r0
00312d8c: mov      r0, r6
00312d90: bl       #0x30ed6c
00312d94: mov      r1, r0
00312d98: mov      r0, r7
00312d9c: bl       #0x30eba4
00312da0: str      r0, [r5, #0xc]
00312da4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
0035a0e4: push     {r4, r5, r6, r7, r8, sl, lr}
0035a0e8: ldr      r4, [pc, #0x90]
0035a0ec: ldr      r6, [pc, #0x90]
0035a0f0: cmp      r2, #0
0035a0f4: cmpne    r1, #0
0035a0f8: add      r4, pc, r4
0035a0fc: ldr      ip, [r4, r6]
0035a100: mov      r5, r1
0035a104: sub      sp, sp, #0x24
0035a108: ldr      ip, [ip]
0035a10c: moveq    r1, #0
0035a110: movne    r1, #1
0035a114: mov      sl, r0
0035a118: mov      r8, r3
0035a11c: str      ip, [sp, #0x1c]
0035a120: moveq    r5, r1
0035a124: beq      #0x35a15c
0035a128: add      r7, sp, #4
0035a12c: mov      r1, r2
0035a130: mov      r0, r7
0035a134: mov      r2, sp
0035a138: bl       #0x3140ec
0035a13c: mov      r1, r5
0035a140: mov      r0, sl
0035a144: mov      r2, r7
0035a148: mov      r3, r8
0035a14c: bl       #0x352b74
0035a150: mov      r5, r0
0035a154: mov      r0, r7
0035a158: bl       #0x318254
0035a15c: ldr      r3, [r4, r6]
0035a160: ldr      r2, [sp, #0x1c]
0035a164: mov      r0, r5
0035a168: ldr      r3, [r3]
0035a16c: cmp      r2, r3
0035a170: bne      #0x35a17c
0035a174: add      sp, sp, #0x24
0035a178: pop      {r4, r5, r6, r7, r8, sl, pc}
0035a17c: bl       #0x30e310
0035a180: mlseq    r3, r8, sb, sl
0035a184: andeq    r4, r0, ip, lsr #1
