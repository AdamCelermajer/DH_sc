# 0x7ad7e8 _ZN8RenderFX16InvokeASCallbackEPKcS1_PKN7gameswf8as_valueEi
007ad7e8: push {r4, r5, r6, r7, r8, lr}
007ad7ec: ldr r4, [sp, #0x18]
007ad7f0: mov r6, r2
007ad7f4: mov r5, r3
007ad7f8: mov r7, r0
007ad7fc: bl #0x7a9160
007ad800: mov r2, r6
007ad804: mov r1, r0
007ad808: mov r3, r5
007ad80c: mov r0, r7
007ad810: str r4, [sp, #0x18]
007ad814: pop {r4, r5, r6, r7, r8, lr}
007ad818: b #0x7abe0c

# 0x3f6990 _ZN5Level12_LoadProcessEv
003f6990: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f6994: ldr r5, [pc, #0xf84]
003f6998: ldr r3, [pc, #0xf84]
003f699c: ldr r6, [pc, #0xf84]
003f69a0: add r5, pc, r5
003f69a4: ldr r2, [r5, r3]
003f69a8: ldr r3, [r5, r6]
003f69ac: sub sp, sp, #0x4b0
003f69b0: ldrb r2, [r2]
003f69b4: ldr r3, [r3]
003f69b8: sub sp, sp, #0xc
003f69bc: cmp r2, #0
003f69c0: mov r4, r0
003f69c4: str r3, [sp, #0x4b4]
003f69c8: bne #0x3f6aec
003f69cc: ldr r3, [pc, #0xf58]
003f69d0: add r7, sp, #0x490
003f69d4: add r7, r7, #0xc
003f69d8: ldr r8, [r5, r3]
003f69dc: mov r0, r8
003f69e0: bl #0x337888
003f69e4: ldr r1, [pc, #0xf44]
003f69e8: add r2, sp, #0x138
003f69ec: mov r0, r7
003f69f0: add r1, pc, r1
003f69f4: bl #0x3140ec
003f69f8: mov r0, r8
003f69fc: mov r1, r7
003f6a00: bl #0x337a88
003f6a04: mov r8, r0
003f6a08: mov r0, r7
003f6a0c: bl #0x3139ac
003f6a10: cmp r8, #0
003f6a14: bne #0x3f6ad0
003f6a18: ldr r0, [pc, #0xf14]
003f6a1c: ldr r1, [r4, #0x130]
003f6a20: add r0, pc, r0
003f6a24: bl #0x324114
003f6a28: ldr r3, [r4, #0x130]
003f6a2c: cmp r3, #0x25
003f6a30: addls pc, pc, r3, lsl #2
003f6a34: b #0x3f753c
003f6a38: b #0x3f74c8
003f6a3c: b #0x3f753c
003f6a40: b #0x3f745c
003f6a44: b #0x3f7414
003f6a48: b #0x3f73c0
003f6a4c: b #0x3f734c
003f6a50: b #0x3f72d0
003f6a54: b #0x3f7204
003f6a58: b #0x3f7174
003f6a5c: b #0x3f7128
003f6a60: b #0x3f70cc
003f6a64: b #0x3f707c
003f6a68: b #0x3f7a84
003f6a6c: b #0x3f7a34
003f6a70: b #0x3f7880
003f6a74: b #0x3f7828
003f6a78: b #0x3f780c
003f6a7c: b #0x3f77c0
003f6a80: b #0x3f76c8
003f6a84: b #0x3f76ac
003f6a88: b #0x3f7b84
003f6a8c: b #0x3f7b38
003f6a90: b #0x3f7aec
003f6a94: b #0x3f7aa0
003f6a98: b #0x3f7df4
003f6a9c: b #0x3f7da4
003f6aa0: b #0x3f6fb0
003f6aa4: b #0x3f6e90
003f6aa8: b #0x3f6e90
003f6aac: b #0x3f7c3c
003f6ab0: b #0x3f7be8
003f6ab4: b #0x3f7bd0
003f6ab8: b #0x3f7714
003f6abc: b #0x3f7630
003f6ac0: b #0x3f6b34
003f6ac4: b #0x3f753c
003f6ac8: b #0x3f7558
003f6acc: b #0x3f7c98
003f6ad0: ldr r0, [r4, #0x130]
003f6ad4: bl #0x3ef18c
003f6ad8: mov r7, r0
003f6adc: bl #0x42a660
003f6ae0: mov r1, r7
003f6ae4: bl #0x42a08c
003f6ae8: b #0x3f6a18
003f6aec: ldr r7, [pc, #0xf24]
003f6af0: ldr r7, [r5, r7]
003f6af4: mov r0, r7
003f6af8: bl #0x31f594
003f6afc: cmp r0, #0
003f6b00: beq #0x3f69cc
003f6b04: ldr r0, [r7, #0x40]
003f6b08: mov r1, #0
003f6b0c: mov r2, #1
003f6b10: bl #0x36e478
003f6b14: ldr r0, [r0, #0x660]
003f6b18: cmp r0, #0
003f6b1c: beq #0x3f69cc
003f6b20: bl #0x3bba70
003f6b24: bl #0x4364f8
003f6b28: mov r3, #1
003f6b2c: strb r3, [r0, #0x12c]
003f6b30: b #0x3f69cc
003f6b34: bl #0x330740
003f6b38: ldr r1, [pc, #0xdf8]
003f6b3c: add r8, sp, #0x19c
003f6b40: add r2, sp, #0xbc
003f6b44: mov sl, r0
003f6b48: add r1, pc, r1
003f6b4c: mov r0, r8
003f6b50: ldr r7, [pc, #0xec0]
003f6b54: bl #0x3140ec
003f6b58: mov r1, r8
003f6b5c: mov r0, sl
003f6b60: bl #0x337a88
003f6b64: mov r0, r8
003f6b68: bl #0x3139ac
003f6b6c: ldr r3, [r5, r7]
003f6b70: mov r1, #0
003f6b74: mov r2, #1
003f6b78: ldr r0, [r3, #0x40]
003f6b7c: ldr fp, [r4, #0x128]
003f6b80: bl #0x36e478
003f6b84: ldr r8, [r0, #0x660]
003f6b88: cmp r8, #0
003f6b8c: beq #0x3f6bb0
003f6b90: mvn r2, #0
003f6b94: mov r0, r8
003f6b98: ldr r1, [r4, #0x3c]
003f6b9c: bl #0x3bb854
003f6ba0: mov r0, fp
003f6ba4: mov r1, r8
003f6ba8: mov r2, #0
003f6bac: bl #0x4119c4
003f6bb0: bl #0x7fd794
003f6bb4: ldrb r3, [r0, #5]
003f6bb8: cmp r3, #0
003f6bbc: bne #0x3f7f54
003f6bc0: mov r0, fp
003f6bc4: bl #0x40f45c
003f6bc8: ldr sl, [r5, r7]
003f6bcc: mov r2, #0
003f6bd0: mov r1, #0
003f6bd4: ldr r3, [sl, #0x10]
003f6bd8: mov r8, r2
003f6bdc: mov sb, #1
003f6be0: ldr r3, [r3, #0x1c]
003f6be4: mov r0, r3
003f6be8: ldr r3, [r3]
003f6bec: mov lr, pc
003f6bf0: ldr pc, [r3, #0x60]
003f6bf4: mov r0, fp
003f6bf8: ldr r3, [fp]
003f6bfc: mov lr, pc
003f6c00: ldr pc, [r3, #0x10]
003f6c04: b #0x3f6c2c
003f6c08: mov r1, r8
003f6c0c: ldr r0, [sl, #0x40]
003f6c10: mov r2, #1
003f6c14: bl #0x36e478
003f6c18: ldr r3, [r0, #0x660]
003f6c1c: add r8, r8, #1
003f6c20: cmp r3, #0
003f6c24: ldrne r3, [r3, #0x378]
003f6c28: strbne sb, [r3, #8]
003f6c2c: ldr r0, [sl, #0x40]
003f6c30: mov r1, #1
003f6c34: bl #0x36ead0
003f6c38: cmp r8, r0
003f6c3c: blt #0x3f6c08
003f6c40: ldr r3, [r4, #0x128]
003f6c44: add r0, sp, #0x88
003f6c48: sub r0, r0, #0xc
003f6c4c: ldr r1, [r3, #8]
003f6c50: bl #0x597180
003f6c54: ldr r3, [r4, #0x128]
003f6c58: add r8, sp, #0x184
003f6c5c: ldr r0, [r3, #8]
003f6c60: bl #0x597290
003f6c64: mov r1, r0
003f6c68: add r0, sp, #0x78
003f6c6c: sub r0, r0, #8
003f6c70: bl #0x597180
003f6c74: ldr r1, [sp, #0x74]
003f6c78: ldr r0, [sp, #0x80]
003f6c7c: bl #0x30e3ac
003f6c80: ldr r1, [sp, #0x78]
003f6c84: mov fp, r0
003f6c88: ldr r0, [sp, #0x84]
003f6c8c: bl #0x30e3ac
003f6c90: ldr r1, [sp, #0x70]
003f6c94: mov sb, r0
003f6c98: ldr r0, [sp, #0x7c]
003f6c9c: bl #0x30e3ac
003f6ca0: str fp, [r4, #0x1a0]
003f6ca4: str r0, [r4, #0x19c]
003f6ca8: str sb, [r4, #0x1a4]
003f6cac: mov r0, r4
003f6cb0: ldr sb, [sl, #0x58]
003f6cb4: bl #0x3ef4a8
003f6cb8: mov r2, r0
003f6cbc: ldr r3, [r2, #0x1e4]
003f6cc0: ldr r2, [r2, #0x1e8]
003f6cc4: ldr r0, [r0, #0x1e0]
003f6cc8: str r3, [sp, #0x10]
003f6ccc: str r2, [sp, #0x14]
003f6cd0: bl #0x8be2a0
003f6cd4: ldr r3, [sp, #0x10]
003f6cd8: uxtb fp, r0
003f6cdc: mov r0, r3
003f6ce0: bl #0x8be2a0
003f6ce4: ldr r2, [sp, #0x14]
003f6ce8: uxtb r3, r0
003f6cec: str r3, [sp, #0x10]
003f6cf0: mov r0, r2
003f6cf4: bl #0x8be2a0
003f6cf8: mvn r2, #0
003f6cfc: strb r2, [sb, #0xf7]
003f6d00: strb r0, [sb, #0xf6]
003f6d04: ldr r3, [sp, #0x10]
003f6d08: mov r0, r4
003f6d0c: strb fp, [sb, #0xf4]
003f6d10: strb r3, [sb, #0xf5]
003f6d14: bl #0x3ef4a8
003f6d18: ldr r0, [r0, #0x1d8]
003f6d1c: bl #0x30e964
003f6d20: mov fp, r0
003f6d24: mov r0, r4
003f6d28: bl #0x3ef4a8
003f6d2c: ldr r0, [r0, #0x1dc]
003f6d30: bl #0x30e964
003f6d34: str fp, [sb, #0x110]
003f6d38: str r0, [sb, #0x114]
003f6d3c: mov r0, r4
003f6d40: bl #0x3ef4a8
003f6d44: ldr r1, [r0, #0x1f8]
003f6d48: ldr r2, [r0, #0x1fc]
003f6d4c: ldr r3, [r0, #0x200]
003f6d50: str r1, [sb, #0x11c]
003f6d54: str r2, [sb, #0x120]
003f6d58: str r3, [sb, #0x124]
003f6d5c: ldr r3, [sl, #0x10]
003f6d60: mov r0, r4
003f6d64: ldr sb, [r3, #0x1c]
003f6d68: bl #0x3ef4a8
003f6d6c: ldr r1, [r0, #0x1f8]
003f6d70: ldr r2, [r0, #0x1fc]
003f6d74: ldr r3, [r0, #0x200]
003f6d78: str r1, [sb, #0x458]
003f6d7c: str r2, [sb, #0x45c]
003f6d80: str r3, [sb, #0x460]
003f6d84: ldr r3, [sl, #0x10]
003f6d88: ldr fp, [r3, #0x1c]
003f6d8c: bl #0x330740
003f6d90: ldr r1, [pc, #0xba4]
003f6d94: add r2, sp, #0xb8
003f6d98: mov sb, r0
003f6d9c: add r1, pc, r1
003f6da0: mov r0, r8
003f6da4: bl #0x3140ec
003f6da8: mov r1, r8
003f6dac: mov r0, sb
003f6db0: bl #0x337a88
003f6db4: eor r0, r0, #1
003f6db8: strb r0, [fp, #0x430]
003f6dbc: mov r0, r8
003f6dc0: bl #0x3139ac
003f6dc4: ldr r0, [sl, #0x38]
003f6dc8: bl #0x345954
003f6dcc: mov r0, r4
003f6dd0: mov r1, #0
003f6dd4: bl #0x3f2304
003f6dd8: mov r0, r4
003f6ddc: mov r1, #1
003f6de0: mov r2, #4
003f6de4: bl #0x3ef280
003f6de8: mov r0, r4
003f6dec: mov r1, #0
003f6df0: mov r2, #1
003f6df4: bl #0x3ef234
003f6df8: bl #0x7fd794
003f6dfc: ldrb r3, [r0, #5]
003f6e00: cmp r3, #0
003f6e04: bne #0x3f7f30
003f6e08: ldr r0, [r4, #0xec]
003f6e0c: cmp r0, #0
003f6e10: beq #0x3f7f00
003f6e14: ldr r1, [r4, #0x114]
003f6e18: ldr r2, [r4, #0x40]
003f6e1c: ldr r3, [r4, #0x3c]
003f6e20: bl #0x463288
003f6e24: cmp r0, #0
003f6e28: bne #0x3f7f00
003f6e2c: mov r0, r4
003f6e30: bl #0x3ef614
003f6e34: ldr r3, [r5, r7]
003f6e38: mov r8, r0
003f6e3c: mov r1, #0
003f6e40: ldr r0, [r3, #0x40]
003f6e44: mov r2, #1
003f6e48: bl #0x36e478
003f6e4c: cmp r8, #0
003f6e50: ldr r3, [r0, #0x660]
003f6e54: addne r8, r8, #0x160
003f6e58: beq #0x3f81e8
003f6e5c: ldr r3, [r8]
003f6e60: add r1, sp, #0x68
003f6e64: sub r1, r1, #4
003f6e68: str r3, [sp, #0x64]
003f6e6c: ldr r3, [r8, #4]
003f6e70: ldrb r2, [r4, #0xf5]
003f6e74: mov r0, r4
003f6e78: str r3, [sp, #0x68]
003f6e7c: ldr r3, [r8, #8]
003f6e80: str r3, [sp, #0x6c]
003f6e84: bl #0x3f04b4
003f6e88: mov r3, #0
003f6e8c: strb r3, [r4, #0xf5]
003f6e90: ldr r3, [r4, #0x130]
003f6e94: add r3, r3, #1
003f6e98: str r3, [r4, #0x130]
003f6e9c: ldr r3, [r4, #0x130]
003f6ea0: cmp r3, #0x26
003f6ea4: beq #0x3f6f64
003f6ea8: ldr r2, [r4, #0x138]
003f6eac: ldr r3, [r4, #0x134]
003f6eb0: cmp r2, r3
003f6eb4: ldrlt r3, [r4, #0x138]
003f6eb8: ldrge r3, [r4, #0x134]
003f6ebc: str r3, [r4, #0x134]
003f6ec0: ldr r3, [r4, #0x130]
003f6ec4: cmp r3, #0x24
003f6ec8: beq #0x3f6f5c
003f6ecc: ldr r3, [r4, #0x130]
003f6ed0: mov r1, #0x64
003f6ed4: movw r2, #0x1af3
003f6ed8: mul r1, r1, r3
003f6edc: movt r2, #0x6bca
003f6ee0: smull r0, r3, r2, r1
003f6ee4: asr r1, r1, #0x1f
003f6ee8: rsb r3, r1, r3, asr #4
003f6eec: cmp r3, #0x63
003f6ef0: bgt #0x3f6f5c
003f6ef4: str r3, [r4, #0x30]
003f6ef8: bl #0x42ca8c
003f6efc: ldr r1, [pc, #0xa3c]
003f6f00: add r1, pc, r1
003f6f04: bl #0x42d1f0
003f6f08: subs r4, r0, #0
003f6f0c: beq #0x3f6f3c
003f6f10: add r0, r4, #0x48
003f6f14: ldr r7, [r4, #4]
003f6f18: bl #0x386144
003f6f1c: ldr r2, [pc, #0xa20]
003f6f20: mov ip, #0
003f6f24: ldr r1, [r4, #0x4c]
003f6f28: mov r0, r7
003f6f2c: add r2, pc, r2
003f6f30: mov r3, ip
003f6f34: str ip, [sp]
003f6f38: bl #0x7abe0c
003f6f3c: ldr r3, [r5, r6]
003f6f40: ldr r2, [sp, #0x4b4]
003f6f44: ldr r3, [r3]
003f6f48: cmp r2, r3
003f6f4c: bne #0x3f82d4
003f6f50: add sp, sp, #0xbc
003f6f54: add sp, sp, #0x400
003f6f58: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f6f5c: mov r3, #0x64
003f6f60: b #0x3f6ef4
003f6f64: mov r3, #0x64
003f6f68: str r3, [r4, #0x30]
003f6f6c: ldr r3, [pc, #0x9d4]
003f6f70: ldr r3, [r5, r3]
003f6f74: ldrb r3, [r3]
003f6f78: cmp r3, #0
003f6f7c: bne #0x3f6fa4
003f6f80: bl #0x42ca8c
003f6f84: ldr r1, [pc, #0x9c0]
003f6f88: mov r4, r0
003f6f8c: add r1, pc, r1
003f6f90: bl #0x42d1f0
003f6f94: mov r1, r0
003f6f98: mov r0, r4
003f6f9c: bl #0x4317e8
003f6fa0: b #0x3f6ef8
003f6fa4: mov r0, #1
003f6fa8: bl #0x89becc
003f6fac: b #0x3f6f80
003f6fb0: bl #0x42ca8c
003f6fb4: mov r1, #3
003f6fb8: bl #0x431ea4
003f6fbc: ldr r3, [pc, #0x98c]
003f6fc0: mov r1, #0
003f6fc4: mov r2, r1
003f6fc8: ldr r7, [r5, r3]
003f6fcc: mov r3, r1
003f6fd0: mov r0, r7
003f6fd4: bl #0x427c44
003f6fd8: ldr r3, [pc, #0x974]
003f6fdc: mov r1, #0
003f6fe0: mov r2, r1
003f6fe4: ldr r0, [r5, r3]
003f6fe8: mov r3, r1
003f6fec: bl #0x427c44
003f6ff0: mov r1, #0
003f6ff4: mov r2, r1
003f6ff8: mov r3, r1
003f6ffc: mov r0, r7
003f7000: bl #0x427c44
003f7004: ldr r3, [pc, #0x94c]
003f7008: mov r1, #0
003f700c: mov r2, r1
003f7010: ldr r0, [r5, r3]
003f7014: mov r3, r1
003f7018: bl #0x427c44
003f701c: ldr r3, [pc, #0x938]
003f7020: mov r1, #0
003f7024: mov r2, r1
003f7028: ldr r0, [r5, r3]
003f702c: mov r3, r1
003f7030: bl #0x427c44
003f7034: ldr r3, [pc, #0x9dc]
003f7038: ldr r3, [r5, r3]
003f703c: ldr r0, [r3, #0x40]
003f7040: bl #0x36f2bc
003f7044: bl #0x42ca8c
003f7048: bl #0x42cb8c
003f704c: ldr r1, [pc, #0x90c]
003f7050: ldr r2, [pc, #0x90c]
003f7054: mov ip, #0
003f7058: mov r3, ip
003f705c: add r1, pc, r1
003f7060: add r2, pc, r2
003f7064: str ip, [sp]
003f7068: bl #0x7ad7e8
003f706c: ldr r3, [r4, #0x130]
003f7070: add r3, r3, #1
003f7074: str r3, [r4, #0x130]
003f7078: b #0x3f6e9c
003f707c: bl #0x330740
003f7080: ldr r1, [pc, #0x8e0]
003f7084: add r7, sp, #0x31c
003f7088: add r2, sp, #0xfc
003f708c: mov r8, r0
003f7090: add r1, pc, r1
003f7094: mov r0, r7
003f7098: bl #0x3140ec
003f709c: mov r1, r7
003f70a0: mov r0, r8
003f70a4: bl #0x337a88
003f70a8: mov r0, r7
003f70ac: bl #0x3139ac
003f70b0: ldr r3, [pc, #0x978]
003f70b4: ldr r0, [r5, r3]
003f70b8: bl #0x5226cc
003f70bc: ldr r3, [r4, #0x130]
003f70c0: add r3, r3, #1
003f70c4: str r3, [r4, #0x130]
003f70c8: b #0x3f6e9c
003f70cc: bl #0x330740
003f70d0: ldr r1, [pc, #0x894]
003f70d4: add r8, sp, #0x334
003f70d8: add r2, sp, #0x100
003f70dc: mov sl, r0
003f70e0: add r1, pc, r1
003f70e4: mov r0, r8
003f70e8: ldr r7, [pc, #0x928]
003f70ec: bl #0x3140ec
003f70f0: mov r1, r8
003f70f4: mov r0, sl
003f70f8: bl #0x337a88
003f70fc: mov r0, r8
003f7100: bl #0x3139ac
003f7104: ldr r7, [r5, r7]
003f7108: ldr r3, [r7, #0x38]
003f710c: ldr r3, [r3, #0x1c]
003f7110: str r3, [r4, #0x138]
003f7114: ldr r0, [r7, #0x38]
003f7118: bl #0x34552c
003f711c: cmp r0, #0
003f7120: beq #0x3f7114
003f7124: b #0x3f6e90
003f7128: bl #0x330740
003f712c: ldr r1, [pc, #0x83c]
003f7130: add r7, sp, #0x34c
003f7134: add r2, sp, #0x104
003f7138: mov r8, r0
003f713c: add r1, pc, r1
003f7140: mov r0, r7
003f7144: bl #0x3140ec
003f7148: mov r1, r7
003f714c: mov r0, r8
003f7150: bl #0x337a88
003f7154: mov r0, r7
003f7158: bl #0x3139ac
003f715c: mov r0, r4
003f7160: bl #0x3f459c
003f7164: ldr r3, [r4, #0x130]
003f7168: add r3, r3, #1
003f716c: str r3, [r4, #0x130]
003f7170: b #0x3f6e9c
003f7174: bl #0x330740
003f7178: ldr r1, [pc, #0x7f4]
003f717c: add r7, sp, #0x3c4
003f7180: add r2, sp, #0x118
003f7184: mov r8, r0
003f7188: add r1, pc, r1
003f718c: mov r0, r7
003f7190: bl #0x3140ec
003f7194: mov r1, r7
003f7198: mov r0, r8
003f719c: bl #0x337a88
003f71a0: mov r0, r7
003f71a4: bl #0x3139ac
003f71a8: ldr r3, [r4, #0x38]
003f71ac: cmp r3, #0
003f71b0: beq #0x3f8070
003f71b4: ldr r1, [r3, #0x204]
003f71b8: ldr r2, [r3, #0x208]
003f71bc: cmp r1, r2
003f71c0: beq #0x3f71ec
003f71c4: ldr r0, [r3, #0x210]
003f71c8: add r7, r3, #0x204
003f71cc: ldr r3, [pc, #0x844]
003f71d0: ldr r3, [r5, r3]
003f71d4: ldr r8, [r3, #0x38]
003f71d8: bl #0x30e4cc
003f71dc: mov r1, r7
003f71e0: mov r2, r0
003f71e4: mov r0, r8
003f71e8: bl #0x347100
003f71ec: mov r0, r4
003f71f0: bl #0x3f3a58
003f71f4: ldr r3, [r4, #0x130]
003f71f8: add r3, r3, #1
003f71fc: str r3, [r4, #0x130]
003f7200: b #0x3f6e9c
003f7204: ldr r2, [pc, #0x76c]
003f7208: ldr r3, [r4, #0xdc]
003f720c: ldr r7, [r5, r2]
003f7210: ldr r2, [pc, #0x764]
003f7214: str r3, [r7]
003f7218: ldr r8, [r5, r2]
003f721c: ldr r3, [r4, #0xe0]
003f7220: str r3, [r8]
003f7224: ldrb r3, [r4, #0xe8]
003f7228: cmp r3, #0
003f722c: addeq r8, r4, #0xf8
003f7230: bne #0x3f7f7c
003f7234: bl #0x330740
003f7238: ldr r1, [pc, #0x740]
003f723c: add r7, sp, #0x3f4
003f7240: mov sl, r0
003f7244: add r2, sp, #0x120
003f7248: add r1, pc, r1
003f724c: mov r0, r7
003f7250: bl #0x3140ec
003f7254: mov r1, r7
003f7258: mov r0, sl
003f725c: ldr sl, [pc, #0x720]
003f7260: bl #0x337a88
003f7264: mov r0, r7
003f7268: bl #0x3139ac
003f726c: mov r3, #0x1f4
003f7270: str r3, [r4, #0x138]
003f7274: add sl, pc, sl
003f7278: add r7, sp, #0x3dc
003f727c: add sb, sp, #0x11c
003f7280: mov r1, sl
003f7284: mov r2, sb
003f7288: mov r0, r7
003f728c: bl #0x3140ec
003f7290: mov r1, r8
003f7294: mov r2, r7
003f7298: mov r0, r4
003f729c: bl #0x3f3b40
003f72a0: mov fp, r0
003f72a4: mov r0, r7
003f72a8: bl #0x3139ac
003f72ac: cmp fp, #0
003f72b0: beq #0x3f7280
003f72b4: ldr r3, [r4, #0x130]
003f72b8: add r3, r3, #1
003f72bc: str r3, [r4, #0x130]
003f72c0: ldr r3, [r4, #0x13c]
003f72c4: add r3, r3, #1
003f72c8: str r3, [r4, #0x13c]
003f72cc: b #0x3f6e9c
003f72d0: bl #0x330740
003f72d4: ldr r1, [pc, #0x6ac]
003f72d8: add r7, sp, #0x364
003f72dc: add r2, sp, #0x108
003f72e0: mov r8, r0
003f72e4: add r1, pc, r1
003f72e8: mov r0, r7
003f72ec: bl #0x3140ec
003f72f0: mov r1, r7
003f72f4: mov r0, r8
003f72f8: bl #0x337a88
003f72fc: mov r0, r7
003f7300: bl #0x3139ac
003f7304: ldr r3, [pc, #0x70c]
003f7308: ldr r3, [r5, r3]
003f730c: ldr r0, [r3, #0x40]
003f7310: mov r3, #1
003f7314: strb r3, [r0, #0x6c9]
003f7318: bl #0x378fb4
003f731c: mov r1, #0
003f7320: mov r0, #0xc
003f7324: bl #0x310570
003f7328: mov r7, r0
003f732c: bl #0x479d08
003f7330: str r7, [r4, #0x194]
003f7334: mov r0, r7
003f7338: bl #0x479e5c
003f733c: ldr r3, [r4, #0x130]
003f7340: add r3, r3, #1
003f7344: str r3, [r4, #0x130]
003f7348: b #0x3f6e9c
003f734c: bl #0x330740
003f7350: ldr r1, [pc, #0x634]
003f7354: add r7, sp, #0x420
003f7358: add r7, r7, #4
003f735c: add r2, sp, #0x124
003f7360: mov r8, r0
003f7364: add r1, pc, r1
003f7368: mov r0, r7
003f736c: bl #0x3140ec
003f7370: mov r1, r7
003f7374: mov r0, r8
003f7378: bl #0x337a88
003f737c: mov r0, r7
003f7380: bl #0x3139ac
003f7384: ldr r3, [pc, #0x68c]
003f7388: mov ip, #0x44000000
003f738c: mov r1, #0xc4000000
003f7390: ldr r0, [r5, r3]
003f7394: add ip, ip, #0xfa0000
003f7398: add r1, r1, #0xfa0000
003f739c: mov r3, ip
003f73a0: ldr r0, [r0, #0x44]
003f73a4: mov r2, r1
003f73a8: str ip, [sp]
003f73ac: bl #0x34c048
003f73b0: ldr r3, [r4, #0x130]
003f73b4: add r3, r3, #1
003f73b8: str r3, [r4, #0x130]
003f73bc: b #0x3f6e9c
003f73c0: bl #0x330740
003f73c4: ldr r1, [pc, #0x5c4]
003f73c8: add r7, sp, #0x430
003f73cc: add r7, r7, #0xc
003f73d0: add r2, sp, #0x128
003f73d4: mov r8, r0
003f73d8: add r1, pc, r1
003f73dc: mov r0, r7
003f73e0: bl #0x3140ec
003f73e4: mov r1, r7
003f73e8: mov r0, r8
003f73ec: bl #0x337a88
003f73f0: mov r0, r7
003f73f4: bl #0x3139ac
003f73f8: ldr r3, [pc, #0x5e8]
003f73fc: ldr r0, [r5, r3]
003f7400: bl #0x496bd8
003f7404: ldr r3, [r4, #0x130]
003f7408: add r3, r3, #1
003f740c: str r3, [r4, #0x130]
003f7410: b #0x3f6e9c
003f7414: bl #0x330740
003f7418: ldr r1, [pc, #0x574]
003f741c: add r7, sp, #0x460
003f7420: mov r8, r0
003f7424: add r7, r7, #0xc
003f7428: add r2, sp, #0x130
003f742c: add r1, pc, r1
003f7430: mov r0, r7
003f7434: bl #0x3140ec
003f7438: mov r1, r7
003f743c: mov r0, r8
003f7440: bl #0x337a88
003f7444: mov r0, r7
003f7448: bl #0x3139ac
003f744c: ldr r3, [r4, #0x130]
003f7450: add r3, r3, #1
003f7454: str r3, [r4, #0x130]
003f7458: b #0x3f6e9c
003f745c: bl #0x330740
003f7460: ldr r1, [pc, #0x530]
003f7464: add r7, sp, #0x480
003f7468: add r7, r7, #4
003f746c: add r2, sp, #0x134
003f7470: mov r8, r0
003f7474: add r1, pc, r1
003f7478: mov r0, r7
003f747c: bl #0x3140ec
003f7480: mov r1, r7
003f7484: mov r0, r8
003f7488: bl #0x337a88
003f748c: mov r0, r7
003f7490: bl #0x3139ac
003f7494: bl #0x42ca8c
003f7498: mov r1, #2
003f749c: bl #0x42d4bc
003f74a0: bl #0x42ca8c
003f74a4: mov r1, #1
003f74a8: bl #0x42d4bc
003f74ac: bl #0x42ca8c
003f74b0: mov r1, #3
003f74b4: bl #0x42d4bc
003f74b8: ldr r3, [r4, #0x130]
003f74bc: add r3, r3, #1
003f74c0: str r3, [r4, #0x130]
003f74c4: b #0x3f6e9c
003f74c8: ldr r3, [pc, #0x548]
003f74cc: mov r7, #0
003f74d0: ldr r8, [r5, r3]
003f74d4: ldr r3, [pc, #0x4c0]
003f74d8: mov r0, r8
003f74dc: ldr r2, [r5, r3]
003f74e0: ldr r3, [pc, #0x4b8]
003f74e4: str r7, [r2]
003f74e8: ldr r3, [r5, r3]
003f74ec: str r7, [r3]
003f74f0: bl #0x31f55c
003f74f4: ldr r0, [pc, #0x4a8]
003f74f8: add r0, pc, r0
003f74fc: bl #0x31041c
003f7500: ldr r3, [pc, #0x4a0]
003f7504: mov r1, #0x1f4
003f7508: ldr r3, [r5, r3]
003f750c: ldr r0, [r3]
003f7510: bl #0x369990
003f7514: mov r3, #1
003f7518: strb r3, [r8, #0xb4]
003f751c: str r7, [r4, #0x134]
003f7520: str r7, [r4, #0x138]
003f7524: str r7, [r4, #0x13c]
003f7528: ldr r3, [r4, #0x130]
003f752c: strb r7, [r4, #0x144]
003f7530: add r3, r3, #1
003f7534: str r3, [r4, #0x130]
003f7538: b #0x3f6e9c
003f753c: ldr r3, [r4, #0x130]
003f7540: cmp r3, #0x25
003f7544: ldrle r3, [r4, #0x130]
003f7548: movgt r3, #0x26
003f754c: addle r3, r3, #1
003f7550: str r3, [r4, #0x130]
003f7554: b #0x3f6e9c
003f7558: bl #0x330740
003f755c: ldr r1, [pc, #0x448]
003f7560: add r7, sp, #0x16c
003f7564: add r2, sp, #0xb8
003f7568: sub r2, r2, #4
003f756c: mov r8, r0
003f7570: add r1, pc, r1
003f7574: mov r0, r7
003f7578: bl #0x3140ec
003f757c: mov r1, r7
003f7580: mov r0, r8
003f7584: bl #0x337a88
003f7588: mov r0, r7
003f758c: bl #0x3139ac
003f7590: bl #0x7fd794
003f7594: ldrb r3, [r0, #5]
003f7598: cmp r3, #0
003f759c: beq #0x3f6e9c
003f75a0: ldr r7, [pc, #0x470]
003f75a4: mov r1, #0
003f75a8: mov r2, r1
003f75ac: ldr r8, [r5, r7]
003f75b0: ldr r0, [r8, #0x40]
003f75b4: bl #0x36e478
003f75b8: ldrb r3, [r0, #0x545]
003f75bc: cmp r3, #0
003f75c0: bne #0x3f81b4
003f75c4: ldr r3, [r5, r7]
003f75c8: ldr r0, [r3, #0x38]
003f75cc: ldrb r3, [r0, #0x1ac]
003f75d0: cmp r3, #0
003f75d4: bne #0x3f75dc
003f75d8: bl #0x340be0
003f75dc: ldr r7, [r5, r7]
003f75e0: mov r1, #0x3f800000
003f75e4: ldr r0, [r7, #0x38]
003f75e8: bl #0x34a620
003f75ec: mov r1, #0
003f75f0: ldr r0, [r7, #0x40]
003f75f4: mov r2, r1
003f75f8: bl #0x36e478
003f75fc: mov r7, r0
003f7600: bl #0x80f23c
003f7604: cmp r0, #0
003f7608: beq #0x3f6e9c
003f760c: ldr r3, [r7, #0x660]
003f7610: cmp r3, #0
003f7614: beq #0x3f6e9c
003f7618: movw r2, #0x14e8
003f761c: ldr r0, [r3, r2]
003f7620: cmp r0, #0
003f7624: beq #0x3f6e9c
003f7628: bl #0x4679e8
003f762c: b #0x3f6e9c
003f7630: bl #0x330740
003f7634: ldr r1, [pc, #0x374]
003f7638: add r7, sp, #0x1b4
003f763c: add r2, sp, #0xc0
003f7640: mov r8, r0
003f7644: add r1, pc, r1
003f7648: mov r0, r7
003f764c: bl #0x3140ec
003f7650: mov r1, r7
003f7654: mov r0, r8
003f7658: bl #0x337a88
003f765c: mov r0, r7
003f7660: bl #0x3139ac
003f7664: ldrb r3, [r4, #0xf1]
003f7668: cmp r3, #0
003f766c: bne #0x3f7e40
003f7670: ldr r7, [pc, #0x3a0]
003f7674: ldr r3, [r5, r7]
003f7678: mov r2, #1
003f767c: mov r1, #0
003f7680: ldr r0, [r3, #0x40]
003f7684: bl #0x36e478
003f7688: mov r1, #0
003f768c: ldr r0, [r0, #0x660]
003f7690: bl #0x3bb798
003f7694: ldr r3, [r4, #0x130]
003f7698: mov r2, #0
003f769c: strb r2, [r4, #0xf3]
003f76a0: add r3, r3, #1
003f76a4: str r3, [r4, #0x130]
003f76a8: b #0x3f6e9c
003f76ac: bl #0x330740
003f76b0: ldr r1, [pc, #0x2fc]
003f76b4: mov r8, r0
003f76b8: add r7, sp, #0x274
003f76bc: add r2, sp, #0xe0
003f76c0: add r1, pc, r1
003f76c4: b #0x3f7430
003f76c8: bl #0x330740
003f76cc: ldr r1, [pc, #0x2e4]
003f76d0: add r7, sp, #0x28c
003f76d4: add r2, sp, #0xe4
003f76d8: mov r8, r0
003f76dc: add r1, pc, r1
003f76e0: mov r0, r7
003f76e4: bl #0x3140ec
003f76e8: mov r1, r7
003f76ec: mov r0, r8
003f76f0: bl #0x337a88
003f76f4: mov r0, r7
003f76f8: bl #0x3139ac
003f76fc: mov r0, r4
003f7700: bl #0x3eff98
003f7704: ldr r3, [r4, #0x130]
003f7708: add r3, r3, #1
003f770c: str r3, [r4, #0x130]
003f7710: b #0x3f6e9c
003f7714: ldr r3, [pc, #0x2fc]
003f7718: mov r0, r4
003f771c: mov r8, #0x3f800000
003f7720: ldr r7, [r5, r3]
003f7724: ldr r3, [r7, #0x10]
003f7728: ldr fp, [r3, #0x1c]
003f772c: bl #0x3ef4a8
003f7730: ldr sb, [r0, #0x1cc]
003f7734: mov r0, r4
003f7738: bl #0x3ef4a8
003f773c: ldr sl, [r0, #0x1d0]
003f7740: mov r0, r4
003f7744: bl #0x3ef4a8
003f7748: ldr r3, [r0, #0x1d4]
003f774c: add r1, sp, #0x58
003f7750: mov r0, fp
003f7754: sub r1, r1, #4
003f7758: str r3, [sp, #0x5c]
003f775c: str r8, [sp, #0x60]
003f7760: str sb, [sp, #0x54]
003f7764: str sl, [sp, #0x58]
003f7768: bl #0x589508
003f776c: ldr r3, [pc, #0x2bc]
003f7770: mov r1, r8
003f7774: ldr r0, [r7, #0x38]
003f7778: ldr r8, [r5, r3]
003f777c: mov r3, #1
003f7780: strb r3, [r8, #0x94]
003f7784: bl #0x34a620
003f7788: bl #0x3ce9b8
003f778c: ldr r0, [r7, #0x44]
003f7790: bl #0x34bd08
003f7794: mov r3, #0
003f7798: strb r3, [r8, #0x94]
003f779c: ldr r3, [r4, #0x128]
003f77a0: mov r0, r3
003f77a4: ldr r3, [r3]
003f77a8: mov lr, pc
003f77ac: ldr pc, [r3, #0x10]
003f77b0: ldr r3, [r4, #0x130]
003f77b4: add r3, r3, #1
003f77b8: str r3, [r4, #0x130]
003f77bc: b #0x3f6e9c
003f77c0: bl #0x330740
003f77c4: ldr r1, [pc, #0x1f0]
003f77c8: add r7, sp, #0x2a4
003f77cc: add r2, sp, #0xe8
003f77d0: mov r8, r0
003f77d4: add r1, pc, r1
003f77d8: mov r0, r7
003f77dc: bl #0x3140ec
003f77e0: mov r1, r7
003f77e4: mov r0, r8
003f77e8: bl #0x337a88
003f77ec: mov r0, r7
003f77f0: bl #0x3139ac
003f77f4: mov r0, r4
003f77f8: bl #0x3efca4
003f77fc: ldr r3, [r4, #0x130]
003f7800: add r3, r3, #1
003f7804: str r3, [r4, #0x130]
003f7808: b #0x3f6e9c
003f780c: bl #0x330740
003f7810: ldr r1, [pc, #0x1a8]
003f7814: mov r8, r0
003f7818: add r7, sp, #0x2bc
003f781c: add r2, sp, #0xec
003f7820: add r1, pc, r1
003f7824: b #0x3f7430
003f7828: bl #0x330740
003f782c: ldr r1, [pc, #0x190]
003f7830: add r7, sp, #0x2d4
003f7834: add r2, sp, #0xf0
003f7838: mov r8, r0
003f783c: add r1, pc, r1
003f7840: mov r0, r7
003f7844: bl #0x3140ec
003f7848: mov r1, r7
003f784c: mov r0, r8
003f7850: bl #0x337a88
003f7854: mov r0, r7
003f7858: bl #0x3139ac
003f785c: ldr r3, [pc, #0x144]
003f7860: ldr r1, [r4, #0x3c]
003f7864: ldr r3, [r5, r3]
003f7868: ldr r0, [r3]
003f786c: bl #0x3695f4
003f7870: ldr r3, [r4, #0x130]
003f7874: add r3, r3, #1
003f7878: str r3, [r4, #0x130]
003f787c: b #0x3f6e9c
003f7880: bl #0x330740
003f7884: ldr r1, [pc, #0x13c]
003f7888: add r7, sp, #0x2ec
003f788c: add r2, sp, #0xf4
003f7890: mov r8, r0
003f7894: add r1, pc, r1
003f7898: mov r0, r7
003f789c: bl #0x3140ec
003f78a0: mov r1, r7
003f78a4: mov r0, r8
003f78a8: bl #0x337a88
003f78ac: mov r0, r7
003f78b0: bl #0x3139ac
003f78b4: mov r0, r4
003f78b8: bl #0x3f0018
003f78bc: bl #0x7fd794
003f78c0: ldrb r3, [r0, #5]
003f78c4: cmp r3, #0
003f78c8: beq #0x3f6e90
003f78cc: ldr r3, [pc, #0x144]
003f78d0: mov r1, #0
003f78d4: mov r2, r1
003f78d8: ldr r7, [r5, r3]
003f78dc: ldr r0, [r7, #0x40]
003f78e0: bl #0x36e478
003f78e4: ldr r3, [r0]
003f78e8: mov lr, pc
003f78ec: ldr pc, [r3, #0x5c]
003f78f0: cmp r0, #0
003f78f4: bne #0x3f6e90
003f78f8: bl #0x800f8c
003f78fc: ldr r3, [r0]
003f7900: mov lr, pc
003f7904: ldr pc, [r3, #0x3c]
003f7908: bl #0x7fbd74
003f790c: bl #0x7fbe54
003f7910: mov r0, r7
003f7914: mov r1, #3
003f7918: bl #0x32c1f4
003f791c: b #0x3f6e90
003f7920: ldrsheq lr, [sb], #-0
003f7924: strheq r3, [r0], -r0
003f7928: andeq r4, r0, ip, lsr #1
003f792c: andeq r0, r0, r4, lsl #17
003f7930: strheq r0, [sp], #-8
003f7934: subeq r0, sp, r8, lsr #1

# 0x7abe0c _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi
007abe0c: push {r4, r5, r6, r7, r8, sl, lr}
007abe10: ldr r4, [pc, #0x114]
007abe14: ldr r6, [pc, #0x114]
007abe18: subs r5, r1, #0
007abe1c: add r4, pc, r4
007abe20: ldr r1, [r4, r6]
007abe24: mov r7, r3
007abe28: sub sp, sp, #0x24
007abe2c: ldr r3, [r1]
007abe30: mov r8, r2
007abe34: str r3, [sp, #0x1c]
007abe38: beq #0x7abed8
007abe3c: ldr r3, [r5]
007abe40: mov r0, r5
007abe44: mov r1, #2
007abe48: mov lr, pc
007abe4c: ldr pc, [r3, #8]
007abe50: cmp r0, #0
007abe54: movne sl, r5
007abe58: beq #0x7abec4
007abe5c: mov r0, r5
007abe60: bl #0x759c64
007abe64: ldr r3, [sl]
007abe68: mov r0, sl
007abe6c: mov lr, pc
007abe70: ldr pc, [r3, #0x58]
007abe74: ldr ip, [sp, #0x40]
007abe78: mov r1, r0
007abe7c: mov r3, r8
007abe80: add r0, sp, #8
007abe84: mov r2, r5
007abe88: stm sp, {r7, ip}
007abe8c: bl #0x7bbbfc
007abe90: ldrsb r3, [sp, #8]
007abe94: cmn r3, #1
007abe98: beq #0x7abee0
007abe9c: mov r0, r5
007abea0: bl #0x75a240
007abea4: mov r0, #1
007abea8: ldr r3, [r4, r6]
007abeac: ldr r2, [sp, #0x1c]
007abeb0: ldr r3, [r3]
007abeb4: cmp r2, r3
007abeb8: bne #0x7abf28
007abebc: add sp, sp, #0x24
007abec0: pop {r4, r5, r6, r7, r8, sl, pc}
007abec4: add sl, r5, #0x3c
007abec8: mov r0, sl
007abecc: bl #0x438224
007abed0: cmp r0, #0
007abed4: bne #0x7abef0
007abed8: mov r0, #0
007abedc: b #0x7abea8
007abee0: ldr r0, [sp, #0x14]
007abee4: ldr r1, [sp, #0x10]
007abee8: bl #0x752b38
007abeec: b #0x7abe9c
007abef0: mov r0, sl
007abef4: bl #0x438224
007abef8: mov r1, #2
007abefc: ldr r3, [r0]
007abf00: mov lr, pc
007abf04: ldr pc, [r3, #8]
007abf08: cmp r0, #0
007abf0c: beq #0x7abed8
007abf10: mov r0, sl
007abf14: bl #0x438224
007abf18: subs sl, r0, #0
007abf1c: bne #0x7abe5c
007abf20: mov r0, #0
007abf24: b #0x7abea8
007abf28: bl #0x30e310
007abf2c: andseq r8, lr, r4, ror ip
007abf30: andeq r4, r0, ip, lsr #1

# 0x7ac498 _ZN8RenderFX8SetFocusEPKci
007ac498: push {r4, r5, r6, lr}
007ac49c: mov r4, r2
007ac4a0: mov r5, r0
007ac4a4: bl #0x7a9160
007ac4a8: mov r2, r4
007ac4ac: mov r1, r0
007ac4b0: mov r0, r5
007ac4b4: pop {r4, r5, r6, lr}
007ac4b8: b #0x7ac228

# 0x89b25c _ZN13ALicenseCheck7LoadRMSEv
0089b25c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0089b260: ldr r5, [pc, #0x3d0]
0089b264: ldr fp, [pc, #0x3d0]
0089b268: ldr r2, [pc, #0x3d0]
0089b26c: add r5, pc, r5
0089b270: ldr r3, [r5, fp]
0089b274: sub sp, sp, #0x80000
0089b278: sub sp, sp, #0x410
0089b27c: sub sp, sp, #4
0089b280: ldr r3, [r3]
0089b284: ldr r2, [r5, r2]
0089b288: ldr r1, [pc, #0x3b4]
0089b28c: add r4, sp, #0x80000
0089b290: add ip, sp, #0x80000
0089b294: add r4, r4, #0x30c
0089b298: add r1, pc, r1
0089b29c: ldr r2, [r2]
0089b2a0: str r3, [ip, #0x40c]
0089b2a4: mov r0, r4
0089b2a8: bl #0x30eae4
0089b2ac: ldr r1, [pc, #0x394]
0089b2b0: mov r0, r4
0089b2b4: add r1, pc, r1
0089b2b8: bl #0x30e508
0089b2bc: subs r6, r0, #0
0089b2c0: beq #0x89b5b0
0089b2c4: add r4, sp, #0x410
0089b2c8: sub r4, r4, #0x400
0089b2cc: sub r4, r4, #4
0089b2d0: mov r1, #1
0089b2d4: mov r3, r6
0089b2d8: mov r2, #0x80000
0089b2dc: mov r0, r4
0089b2e0: bl #0x30e2ec
0089b2e4: mov r0, r6
0089b2e8: bl #0x30eb14
0089b2ec: ldr r1, [pc, #0x358]
0089b2f0: add sl, sp, #0x80000
0089b2f4: mov r3, #0
0089b2f8: add r1, pc, r1
0089b2fc: add sl, sl, #0x20c
0089b300: add r0, r1, #0x34
0089b304: ldr r2, [r0, r3, lsl #2]
0089b308: ldrb r2, [r1, r2]
0089b30c: strb r2, [sl, r3]
0089b310: add r3, r3, #1
0089b314: cmp r3, #8
0089b318: bne #0x89b304
0089b31c: ldr r1, [pc, #0x32c]
0089b320: mov r3, #0x2000
0089b324: add r3, r3, #0xc
0089b328: ldr sb, [r5, r1]
0089b32c: ldrb r3, [r4, r3]
0089b330: add r1, sp, #0x80000
0089b334: add r1, r1, #0x200
0089b338: mov r2, #0x2040
0089b33c: mov r6, #0
0089b340: add r2, r2, #0x10
0089b344: strb r6, [r1, #0x14]
0089b348: mov r0, sb
0089b34c: ldrb r8, [r4, r2]
0089b350: str r3, [sp]
0089b354: bl #0x30de54
0089b358: subs r7, r0, #0
0089b35c: ldr r3, [sp]
0089b360: ble #0x89b5dc
0089b364: ldr r0, [pc, #0x2e8]
0089b368: mov r2, r6
0089b36c: add r6, sp, #0x80000
0089b370: add r6, r6, #0x10c
0089b374: ldrsb r1, [r2, sb]
0089b378: cmp r1, #0xff
0089b37c: ldrls ip, [r5, r0]
0089b380: ldrls ip, [ip]
0089b384: addls r1, ip, r1, lsl #1
0089b388: ldrshls r1, [r1, #2]
0089b38c: strb r1, [r6, r2]
0089b390: add r2, r2, #1
0089b394: cmp r7, r2
0089b398: bne #0x89b374
0089b39c: mov sb, #0
0089b3a0: mov r0, r6
0089b3a4: mov r1, sl
0089b3a8: mov r2, r6
0089b3ac: strb sb, [r6, r7]
0089b3b0: str r3, [sp]
0089b3b4: bl #0x89a820
0089b3b8: ldr r3, [sp]
0089b3bc: add ip, sp, #0x80000
0089b3c0: add ip, ip, #0x410
0089b3c4: add r2, ip, r3
0089b3c8: add r1, ip, r7
0089b3cc: sub r2, r2, #0x7e000
0089b3d0: add r0, sp, #0x80000
0089b3d4: strb sb, [r1, #-0x304]
0089b3d8: sub r2, r2, #0x384
0089b3dc: add r0, r0, #0x100
0089b3e0: ldrb r2, [r2]
0089b3e4: ldrb r1, [r0, #0xc]
0089b3e8: cmp r1, r2
0089b3ec: addeq r3, r3, r8
0089b3f0: addeq r3, r3, #0x2080
0089b3f4: beq #0x89b410
0089b3f8: b #0x89b5b0
0089b3fc: ldrb r1, [r4, r3]
0089b400: ldrb r2, [r6, sb]
0089b404: add r3, r3, r8
0089b408: cmp r1, r2
0089b40c: bne #0x89b5b0
0089b410: add sb, sb, #1
0089b414: cmp r7, sb
0089b418: bne #0x89b3fc
0089b41c: mov r3, #0x6000
0089b420: add r3, r3, #0xc
0089b424: ldrb r0, [r4, r3]
0089b428: mov r3, #0x6000
0089b42c: add r3, r3, #0x50
0089b430: cmp r0, #0
0089b434: ldrb ip, [r4, r3]
0089b438: beq #0x89b480
0089b43c: mov r3, #0x6000
0089b440: add r3, r3, #0x80
0089b444: ldrsb r3, [r4, r3]
0089b448: cmp r3, #0xf
0089b44c: addle r2, ip, #0x6000
0089b450: addle r2, r2, #0x80
0089b454: movle r3, #0
0089b458: ble #0x89b474
0089b45c: b #0x89b5b0
0089b460: ldrb r1, [r4, r2]
0089b464: add r2, r2, ip
0089b468: lsl r1, r1, #0x18
0089b46c: cmp r1, #0xf000000
0089b470: bgt #0x89b5b0
0089b474: add r3, r3, #1
0089b478: cmp r3, r0
0089b47c: bne #0x89b460
0089b480: ldr r1, [pc, #0x1d0]
0089b484: mov r2, #0xe000
0089b488: mov r3, r2
0089b48c: ldr sb, [r5, r1]
0089b490: add r2, r2, #0xc
0089b494: ldrb r2, [r4, r2]
0089b498: add r3, r3, #0x50
0089b49c: mov r0, sb
0089b4a0: str r2, [sp, #4]
0089b4a4: ldrb r8, [r4, r3]
0089b4a8: bl #0x30de54
0089b4ac: subs r7, r0, #0
0089b4b0: ble #0x89b60c
0089b4b4: add r6, sp, #0x80000
0089b4b8: ldr r1, [pc, #0x194]
0089b4bc: add r6, r6, #0x10
0089b4c0: sub r6, r6, #4
0089b4c4: mov r3, #0
0089b4c8: ldrsb r2, [r3, sb]
0089b4cc: cmp r2, #0xff
0089b4d0: ldrls r0, [r5, r1]
0089b4d4: ldrls r0, [r0]
0089b4d8: addls r2, r0, r2, lsl #1
0089b4dc: ldrshls r2, [r2, #2]
0089b4e0: strb r2, [r6, r3]
0089b4e4: add r3, r3, #1
0089b4e8: cmp r7, r3
0089b4ec: bne #0x89b4c8
0089b4f0: mov r1, sl
0089b4f4: mov sl, #0
0089b4f8: ldr sb, [sp, #4]
0089b4fc: mov r2, r6
0089b500: mov r0, r6
0089b504: strb sl, [r6, r7]
0089b508: bl #0x89a820
0089b50c: add r1, sp, #0x80000
0089b510: add r1, r1, #0x410
0089b514: add r3, r1, sb
0089b518: add r2, r1, r7
0089b51c: sub r3, r3, #0x72000
0089b520: strb sl, [r2, #-0x404]
0089b524: sub r3, r3, #0x304
0089b528: add ip, sp, #0x80000
0089b52c: ldrb r3, [r3]
0089b530: ldrb r2, [ip, #0xc]
0089b534: cmp r2, r3
0089b538: addeq r3, sb, r8
0089b53c: addeq r3, r3, #0xe100
0089b540: beq #0x89b55c
0089b544: b #0x89b5b0
0089b548: ldrb r1, [r4, r3]
0089b54c: ldrb r2, [r6, sl]
0089b550: add r3, r3, r8
0089b554: cmp r1, r2
0089b558: bne #0x89b5b0
0089b55c: add sl, sl, #1
0089b560: cmp r7, sl
0089b564: bne #0x89b548
0089b568: mov r3, #0x32000
0089b56c: mov r0, #0x64000
0089b570: add r3, r3, #0x200
0089b574: add r0, r0, #0x200
0089b578: mov r2, #0
0089b57c: ldrsb r1, [r4, r3]
0089b580: add r3, r3, #1
0089b584: cmp r3, r0
0089b588: add r2, r2, r1
0089b58c: bne #0x89b57c
0089b590: mov r3, #0x64000
0089b594: add r3, r3, #0x280
0089b598: ldrb r3, [r4, r3]
0089b59c: and r2, r2, #0xff
0089b5a0: cmp r2, r3
0089b5a4: movne r0, #0
0089b5a8: moveq r0, #1
0089b5ac: b #0x89b5b4
0089b5b0: mov r0, #0
0089b5b4: ldr r3, [r5, fp]
0089b5b8: add r1, sp, #0x80000
0089b5bc: ldr r2, [r1, #0x40c]
0089b5c0: ldr r3, [r3]
0089b5c4: cmp r2, r3
0089b5c8: bne #0x89b634
0089b5cc: add sp, sp, #0x14
0089b5d0: add sp, sp, #0x400
0089b5d4: add sp, sp, #0x80000
0089b5d8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0089b5dc: add r3, sp, #0x80000
0089b5e0: add r3, r3, #0x10c
0089b5e4: mov r2, r3
0089b5e8: strb r6, [r3, r7]
0089b5ec: mov r0, r3
0089b5f0: mov r1, sl
0089b5f4: bl #0x89a820
0089b5f8: add r2, sp, #0x80000
0089b5fc: add r2, r2, #0x410
0089b600: add r7, r2, r7
0089b604: strb r6, [r7, #-0x304]
0089b608: b #0x89b41c
0089b60c: add r3, sp, #0x80000
0089b610: add r3, r3, #0x10
0089b614: sub r3, r3, #4
0089b618: mov ip, #0
0089b61c: mov r0, r3
0089b620: mov r1, sl
0089b624: mov r2, r3
0089b628: strb ip, [r3, r7]
0089b62c: bl #0x89a820
0089b630: b #0x89b568
0089b634: bl #0x30e310
0089b638: andeq sb, pc, r4, lsr #16
0089b63c: andeq r4, r0, ip, lsr #1
0089b640: strdeq r4, r5, [r0], -r0
0089b644: andeq sb, r7, r8, asr r7
0089b648: andeq r5, r2, ip, ror #9
0089b64c: andeq sb, r7, r0, asr #11
0089b650: andeq r3, r0, r8, lsr #7
0089b654: andeq r0, r0, r8, lsr #12
0089b658: strdeq r1, r2, [r0], -ip

# 0x89becc ALicenseCheck_ValidateLicense
0089becc: b #0x89bdc0

# 0x7ac228 _ZN8RenderFX8SetFocusEPN7gameswf9characterEi
007ac228: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ac22c: ldr r4, [pc, #0x1cc]
007ac230: ldr sl, [pc, #0x1cc]
007ac234: mov r8, r2
007ac238: add r4, pc, r4
007ac23c: ldr r3, [r4, sl]
007ac240: sub sp, sp, #0x134
007ac244: mov r5, r0
007ac248: ldr r2, [r3]
007ac24c: mov r3, #0x28
007ac250: mla r3, r3, r8, r0
007ac254: str r2, [sp, #0x12c]
007ac258: ldr r7, [r3, #0x68]
007ac25c: mov r6, r1
007ac260: cmp r1, r7
007ac264: beq #0x7ac3b8
007ac268: ldr sb, [r0, #0xf8]
007ac26c: ands sb, sb, #0x40
007ac270: bne #0x7ac314
007ac274: cmp r7, #0
007ac278: beq #0x7ac314
007ac27c: ldr r3, [r7]
007ac280: mov r0, r7
007ac284: mov r1, #2
007ac288: mov lr, pc
007ac28c: ldr pc, [r3, #8]
007ac290: cmp r0, #0
007ac294: beq #0x7ac314
007ac298: ldrb r3, [r7, #0xea]
007ac29c: cmp r3, #0
007ac2a0: beq #0x7ac314
007ac2a4: ldr r2, [pc, #0x15c]
007ac2a8: mov r1, r7
007ac2ac: mov r3, sb
007ac2b0: add r2, pc, r2
007ac2b4: mov r0, r5
007ac2b8: bl #0x7aba04
007ac2bc: mov r3, #0
007ac2c0: mov r2, #1
007ac2c4: str sb, [sp, #0x1c]
007ac2c8: str r3, [sp, #0x18]
007ac2cc: str r2, [sp, #0xc]
007ac2d0: str r3, [sp, #0x14]
007ac2d4: str r3, [sp, #0x10]
007ac2d8: str r7, [sp, #4]
007ac2dc: ldr r2, [r7, #0x44]
007ac2e0: mov r0, r5
007ac2e4: add r1, sp, #4
007ac2e8: ldrsb r3, [r2]
007ac2ec: cmn r3, #1
007ac2f0: ldreq r2, [r2, #0xc]
007ac2f4: mov r3, #0
007ac2f8: addne r2, r2, #1
007ac2fc: str r2, [sp, #8]
007ac300: strb r3, [sp, #0x28]
007ac304: strb r3, [sp, #0x29]
007ac308: str r3, [sp, #0x20]
007ac30c: str r8, [sp, #0x24]
007ac310: bl #0x7abf34
007ac314: mov fp, #0x28
007ac318: mul fp, fp, r8
007ac31c: mov r1, r6
007ac320: add fp, fp, #0x68
007ac324: add fp, r5, fp
007ac328: mov r0, fp
007ac32c: bl #0x75518c
007ac330: ldr r3, [r5, #0xf8]
007ac334: ands r3, r3, #0x40
007ac338: bne #0x7ac3b8
007ac33c: cmp r6, #0
007ac340: beq #0x7ac3b8
007ac344: ldr r1, [r6, #0x44]
007ac348: mov r2, #0
007ac34c: str r3, [sp, #0xc]
007ac350: str r2, [sp, #0x18]
007ac354: str r2, [sp, #0x14]
007ac358: str r2, [sp, #0x10]
007ac35c: str r3, [sp, #0x1c]
007ac360: str r6, [sp, #4]
007ac364: ldrsb r3, [r1]
007ac368: mov sb, #0
007ac36c: add r7, sp, #4
007ac370: cmn r3, #1
007ac374: ldreq r1, [r1, #0xc]
007ac378: ldr r3, [r5, #0xfc]
007ac37c: addne r1, r1, #1
007ac380: str r1, [sp, #8]
007ac384: str r8, [sp, #0x24]
007ac388: strb sb, [sp, #0x29]
007ac38c: str sb, [sp, #0x20]
007ac390: strb sb, [sp, #0x28]
007ac394: mov r0, r3
007ac398: mov r1, r7
007ac39c: ldr r3, [r3]
007ac3a0: mov lr, pc
007ac3a4: ldr pc, [r3, #8]
007ac3a8: subs r1, r0, #0
007ac3ac: bne #0x7ac3d4
007ac3b0: mov r0, fp
007ac3b4: bl #0x75518c
007ac3b8: ldr r3, [r4, sl]
007ac3bc: ldr r2, [sp, #0x12c]
007ac3c0: ldr r3, [r3]
007ac3c4: cmp r2, r3
007ac3c8: bne #0x7ac3fc
007ac3cc: add sp, sp, #0x134
007ac3d0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ac3d4: ldr r2, [pc, #0x30]
007ac3d8: mov r1, r6
007ac3dc: mov r3, sb
007ac3e0: add r2, pc, r2
007ac3e4: mov r0, r5
007ac3e8: bl #0x7aba04
007ac3ec: mov r0, r5
007ac3f0: mov r1, r7
007ac3f4: bl #0x7abf34
007ac3f8: b #0x7ac3b8
007ac3fc: bl #0x30e310
007ac400: andseq r8, lr, r8, asr r8
007ac404: andeq r4, r0, ip, lsr #1
007ac408: ldrsheq pc, [r1], -r0
007ac40c: andseq pc, r1, r8, lsr r7

# 0x3850c8 _ZN6GSInit6UpdateEP12StateMachined
003850c8: push {r4, r5, r6, r7, r8, sl, lr}
003850cc: ldr r5, [pc, #0x7a0]
003850d0: ldr r3, [r0, #4]
003850d4: sub sp, sp, #0x24
003850d8: mov r4, r0
003850dc: add r5, pc, r5
003850e0: cmp r3, #0xe
003850e4: addls pc, pc, r3, lsl #2
003850e8: b #0x3853f8
003850ec: b #0x3853e4
003850f0: b #0x3853c0
003850f4: b #0x38535c
003850f8: b #0x3852f8
003850fc: b #0x3852c8
00385100: b #0x385270
00385104: b #0x385254
00385108: b #0x3851b4
0038510c: b #0x3851a8
00385110: b #0x385128
00385114: b #0x385138
00385118: b #0x385170
0038511c: b #0x385544
00385120: b #0x3854e8
00385124: b #0x385400
00385128: ldr r3, [pc, #0x748]
0038512c: ldr r3, [r5, r3]
00385130: ldr r0, [r3]
00385134: bl #0x36c2e4
00385138: ldr r3, [pc, #0x73c]
0038513c: ldr r3, [r5, r3]
00385140: ldr r0, [r3, #0x4c]
00385144: ldr r5, [r3, #0x34]
00385148: bl #0x46d514
0038514c: mov r2, #0
00385150: mov r1, r0
00385154: mov r0, r5
00385158: bl #0x507a94
0038515c: ldr r3, [r4, #4]
00385160: add r3, r3, #1
00385164: str r3, [r4, #4]
00385168: add sp, sp, #0x24
0038516c: pop {r4, r5, r6, r7, r8, sl, pc}
00385170: bl #0x3804c0
00385174: ldr r3, [pc, #0x704]
00385178: ldr r5, [r5, r3]
0038517c: ldr r3, [r5]
00385180: mov r0, r3
00385184: ldr r3, [r3]
00385188: mov lr, pc
0038518c: ldr pc, [r3]
00385190: ldr r0, [r5]
00385194: bl #0x38151c
00385198: ldr r3, [r4, #4]
0038519c: add r3, r3, #1
003851a0: str r3, [r4, #4]
003851a4: b #0x385168
003851a8: mov r3, #9
003851ac: str r3, [r0, #4]
003851b0: b #0x385168
003851b4: ldr r3, [pc, #0x6c0]
003851b8: ldr r2, [r5, r3]
003851bc: ldr r3, [r2, #0x10]
003851c0: ldr r0, [r2, #0x4c]
003851c4: ldr r3, [r3, #0x10]
003851c8: ldr r6, [r3, #0xe0]
003851cc: bl #0x46d514
003851d0: mov r7, r0
003851d4: mov r1, r0
003851d8: ldr r0, [pc, #0x6a4]
003851dc: add r0, pc, r0
003851e0: bl #0x324114
003851e4: cmp r7, #4
003851e8: beq #0x385640
003851ec: cmp r7, #5
003851f0: beq #0x385604
003851f4: ldr r3, [pc, #0x68c]
003851f8: ldr r3, [r5, r3]
003851fc: ldr r3, [r3]
00385200: cmp r3, #0x320
00385204: beq #0x385838
00385208: movw r2, #0x356
0038520c: cmp r3, r2
00385210: beq #0x3857bc
00385214: ldr r2, [pc, #0x670]
00385218: add r5, sp, #0xc
0038521c: mov r1, r6
00385220: add r2, pc, r2
00385224: mov r0, r5
00385228: mov r3, #0
0038522c: bl #0x5ed210
00385230: mov r1, r5
00385234: add r0, r4, #0x10
00385238: bl #0x384df8
0038523c: ldr r0, [sp, #0xc]
00385240: cmp r0, #0
00385244: beq #0x38524c
00385248: bl #0x31d584
0038524c: ldr r3, [r4, #4]
00385250: b #0x3852ec
00385254: ldr r3, [pc, #0x634]
00385258: ldr r3, [r5, r3]
0038525c: ldrb r3, [r3]
00385260: cmp r3, #0
00385264: movne r3, #7
00385268: strne r3, [r0, #4]
0038526c: b #0x385168
00385270: ldr r3, [pc, #0x604]
00385274: ldr r6, [r5, r3]
00385278: ldr r0, [r6, #0x4c]
0038527c: bl #0x46d514
00385280: cmp r0, #4
00385284: bne #0x3855b4
00385288: ldr r3, [pc, #0x5f8]
0038528c: ldr r3, [r5, r3]
00385290: ldr r3, [r3]
00385294: cmp r3, #0x320
00385298: beq #0x3856fc
0038529c: movw r2, #0x356
003852a0: cmp r3, r2
003852a4: beq #0x3856dc
003852a8: ldr r0, [r6, #0x4c]
003852ac: bl #0x46d514
003852b0: mov r1, r0
003852b4: ldr r0, [pc, #0x5d8]
003852b8: add r0, pc, r0
003852bc: bl #0x531bac
003852c0: ldr r3, [r4, #4]
003852c4: b #0x3852ec
003852c8: ldr r3, [pc, #0x5ac]
003852cc: ldr r3, [r5, r3]
003852d0: ldr r0, [r3, #0x4c]
003852d4: ldrb r1, [r0, #0x28]
003852d8: cmp r1, #0
003852dc: movne r3, #4
003852e0: bne #0x3852ec
003852e4: bl #0x46e584
003852e8: ldr r3, [r4, #4]
003852ec: add r3, r3, #1
003852f0: str r3, [r4, #4]
003852f4: b #0x385168
003852f8: ldr r6, [pc, #0x57c]
003852fc: ldr r5, [r5, r6]
00385300: ldr r3, [r5, #0x10]
00385304: ldr r3, [r3, #0x20]
00385308: mov r0, r3
0038530c: ldr r3, [r3]
00385310: mov lr, pc
00385314: ldr pc, [r3, #0xc]
00385318: mov r6, r0
0038531c: b #0x385344
00385320: ldr r3, [r5, #0x10]
00385324: ldr r3, [r3, #0x20]
00385328: mov r0, r3
0038532c: ldr r3, [r3]
00385330: mov lr, pc
00385334: ldr pc, [r3, #0xc]
00385338: rsb r0, r6, r0
0038533c: cmp r0, #0x31
00385340: bgt #0x385168
00385344: ldr r0, [r5, #0x30]
00385348: bl #0x4aa21c
0038534c: cmp r0, #0
00385350: beq #0x385320
00385354: ldr r3, [r4, #4]
00385358: b #0x3852ec
0038535c: ldr r6, [pc, #0x518]
00385360: ldr r5, [r5, r6]
00385364: ldr r3, [r5, #0x10]
00385368: ldr r3, [r3, #0x20]
0038536c: mov r0, r3
00385370: ldr r3, [r3]
00385374: mov lr, pc
00385378: ldr pc, [r3, #0xc]
0038537c: mov r6, r0
00385380: b #0x3853a8
00385384: ldr r3, [r5, #0x10]
00385388: ldr r3, [r3, #0x20]
0038538c: mov r0, r3
00385390: ldr r3, [r3]
00385394: mov lr, pc
00385398: ldr pc, [r3, #0xc]
0038539c: rsb r0, r6, r0
003853a0: cmp r0, #0x31
003853a4: bgt #0x385168
003853a8: ldr r0, [r5, #0x2c]
003853ac: bl #0x4c3708
003853b0: cmp r0, #0
003853b4: beq #0x385384
003853b8: ldr r3, [r4, #4]
003853bc: b #0x3852ec
003853c0: ldr r3, [pc, #0x4b4]
003853c4: mov r1, #1
003853c8: ldr r3, [r5, r3]
003853cc: ldr r0, [r3, #0x4c]
003853d0: bl #0x46e584
003853d4: ldr r3, [r4, #4]
003853d8: add r3, r3, #1
003853dc: str r3, [r4, #4]
003853e0: b #0x385168
003853e4: bl #0x36ca88
003853e8: ldr r3, [r4, #4]
003853ec: add r3, r3, #1
003853f0: str r3, [r4, #4]
003853f4: b #0x385168
003853f8: mvn r0, #0
003853fc: bl #0x30de48
00385400: bl #0x42ca8c
00385404: ldr r6, [pc, #0x470]
00385408: mov sl, r0
0038540c: bl #0x42d73c
00385410: ldr r7, [r5, r6]
00385414: mov r1, #1
00385418: ldr r2, [r7, #0xcc]
0038541c: ldr r3, [r7, #0xd0]
00385420: strb r1, [r7, #0xed]
00385424: cmp r2, r3
00385428: beq #0x38567c
0038542c: mov r0, sp
00385430: mov r1, #0
00385434: bl #0x46403c
00385438: ldr r0, [r7, #0xd0]
0038543c: bl #0x33179c
00385440: cmp r0, #0
00385444: mov r8, sp
00385448: bne #0x3856ac
0038544c: ldr r1, [pc, #0x444]
00385450: mov r0, sl
00385454: ldr r7, [pc, #0x440]
00385458: add r1, pc, r1
0038545c: bl #0x42d1f0
00385460: ldr r3, [r5, r7]
00385464: str r0, [r3, #0xc]
00385468: mov r0, sp
0038546c: bl #0x313f30
00385470: bl #0x320e98
00385474: ldr r6, [r5, r6]
00385478: mov r2, #0
0038547c: strb r2, [r0, #0x26]
00385480: ldr r1, [r5, r7]
00385484: ldr r0, [r6, #0x18]
00385488: bl #0x33a388
0038548c: ldr r1, [pc, #0x40c]
00385490: mov r0, r6
00385494: mov r7, #1
00385498: add r1, pc, r1
0038549c: bl #0x320e44
003854a0: mov r1, r0
003854a4: mov r0, r6
003854a8: bl #0x31f748
003854ac: bl #0x32bd08
003854b0: strb r7, [r0, #0xe]
003854b4: mov r0, r6
003854b8: bl #0x320590
003854bc: cmp r0, #0
003854c0: bne #0x3856c4
003854c4: ldr r3, [pc, #0x3d8]
003854c8: ldr r3, [r5, r3]
003854cc: ldrb r3, [r3]
003854d0: cmp r3, #0
003854d4: beq #0x38524c
003854d8: mov r0, #0
003854dc: bl #0x89becc
003854e0: ldr r3, [r4, #4]
003854e4: b #0x3852ec
003854e8: bl #0x60b0cc
003854ec: ldr r3, [pc, #0x388]
003854f0: ldr r6, [r5, r3]
003854f4: ldr r3, [pc, #0x3ac]
003854f8: ldr r1, [r5, r3]
003854fc: ldr r3, [pc, #0x3a8]
00385500: str r0, [r1]
00385504: ldr r2, [r5, r3]
00385508: ldr r3, [r6, #0x10]
0038550c: mov r1, #0
00385510: str r1, [r2]
00385514: ldr r3, [r3, #0x20]
00385518: mov r0, r3
0038551c: ldr r3, [r3]
00385520: mov lr, pc
00385524: ldr pc, [r3, #0xc]
00385528: mov r3, #0x1e
0038552c: str r0, [r6, #0x70]
00385530: str r3, [r6, #0x6c]
00385534: mov r0, r6
00385538: bl #0x320da4
0038553c: ldr r3, [r4, #4]
00385540: b #0x3852ec
00385544: bl #0x42ca8c
00385548: ldr r6, [pc, #0x32c]
0038554c: mov r7, r0
00385550: ldr r3, [r5, r6]
00385554: ldr r3, [r3, #0x10]
00385558: ldr r3, [r3, #0x20]
0038555c: mov r0, r3
00385560: ldr r3, [r3]
00385564: mov lr, pc
00385568: ldr pc, [r3, #0xc]
0038556c: mov r8, r0
00385570: b #0x38559c
00385574: ldr r3, [r5, r6]
00385578: ldr r3, [r3, #0x10]
0038557c: ldr r3, [r3, #0x20]
00385580: mov r0, r3
00385584: ldr r3, [r3]
00385588: mov lr, pc
0038558c: ldr pc, [r3, #0xc]
00385590: rsb r0, r8, r0
00385594: cmp r0, #0x31
00385598: bgt #0x385168
0038559c: mov r0, r7
003855a0: bl #0x42f304
003855a4: cmp r0, #0
003855a8: beq #0x385574
003855ac: ldr r3, [r4, #4]
003855b0: b #0x3852ec
003855b4: ldr r0, [r6, #0x4c]
003855b8: bl #0x46d514
003855bc: cmp r0, #5
003855c0: beq #0x38573c
003855c4: ldr r3, [pc, #0x2bc]
003855c8: ldr r3, [r5, r3]
003855cc: ldr r3, [r3]
003855d0: cmp r3, #0x320
003855d4: beq #0x38579c
003855d8: movw r2, #0x356
003855dc: cmp r3, r2
003855e0: beq #0x38577c
003855e4: ldr r0, [r6, #0x4c]
003855e8: bl #0x46d514
003855ec: mov r1, r0
003855f0: ldr r0, [pc, #0x2b8]
003855f4: add r0, pc, r0
003855f8: bl #0x531bac
003855fc: ldr r3, [r4, #4]
00385600: b #0x3852ec
00385604: ldr r2, [pc, #0x2a8]
00385608: add r5, sp, #0x18
0038560c: mov r1, r6
00385610: add r2, pc, r2
00385614: mov r0, r5
00385618: mov r3, #0
0038561c: bl #0x5ed210
00385620: mov r1, r5
00385624: add r0, r4, #0x10
00385628: bl #0x384df8
0038562c: ldr r0, [sp, #0x18]
00385630: cmp r0, #0
00385634: beq #0x38524c
00385638: bl #0x31d584
0038563c: b #0x38524c
00385640: ldr r2, [pc, #0x270]
00385644: add r5, sp, #0x1c
00385648: mov r1, r6
0038564c: add r2, pc, r2
00385650: mov r0, r5
00385654: mov r3, #0
00385658: bl #0x5ed210
0038565c: mov r1, r5
00385660: add r0, r4, #0x10
00385664: bl #0x384df8
00385668: ldr r0, [sp, #0x1c]
0038566c: cmp r0, #0
00385670: beq #0x38524c
00385674: bl #0x31d584
00385678: b #0x38524c
0038567c: ldr r3, [r7, #0x4c]
00385680: ldrb r3, [r3, #0x37]
00385684: cmp r3, #0
00385688: beq #0x38571c
0038568c: ldr r1, [pc, #0x228]
00385690: mov r0, sl
00385694: ldr r7, [pc, #0x200]
00385698: add r1, pc, r1
0038569c: bl #0x42d1f0
003856a0: ldr r3, [r5, r7]
003856a4: str r0, [r3, #0xc]
003856a8: b #0x385470
003856ac: ldr r3, [r4, #4]
003856b0: mov r0, sp
003856b4: add r3, r3, #1
003856b8: str r3, [r4, #4]
003856bc: bl #0x313f30
003856c0: b #0x385168
003856c4: ldr r3, [pc, #0x1f4]
003856c8: ldr r3, [r5, r3]
003856cc: mov r0, r3
003856d0: strb r7, [r3, #5]
003856d4: bl #0x52e524
003856d8: b #0x3854c4
003856dc: ldr r0, [r6, #0x4c]
003856e0: bl #0x46d514
003856e4: mov r1, r0
003856e8: ldr r0, [pc, #0x1d4]
003856ec: add r0, pc, r0
003856f0: bl #0x531bac
003856f4: ldr r3, [r4, #4]
003856f8: b #0x3852ec
003856fc: ldr r0, [r6, #0x4c]
00385700: bl #0x46d514
00385704: mov r1, r0
00385708: ldr r0, [pc, #0x1b8]
0038570c: add r0, pc, r0
00385710: bl #0x531bac
00385714: ldr r3, [r4, #4]
00385718: b #0x3852ec
0038571c: ldr r1, [pc, #0x1a8]
00385720: mov r0, sl
00385724: ldr r7, [pc, #0x170]
00385728: add r1, pc, r1
0038572c: bl #0x42d1f0
00385730: ldr r3, [r5, r7]
00385734: str r0, [r3, #0xc]
00385738: b #0x385470
0038573c: ldr r3, [pc, #0x144]
00385740: ldr r3, [r5, r3]
00385744: ldr r3, [r3]
00385748: cmp r3, #0x320
0038574c: beq #0x385818
00385750: movw r2, #0x356
00385754: cmp r3, r2
00385758: beq #0x3857f8
0038575c: ldr r0, [r6, #0x4c]
00385760: bl #0x46d514
00385764: mov r1, r0
00385768: ldr r0, [pc, #0x160]
0038576c: add r0, pc, r0
00385770: bl #0x531bac
00385774: ldr r3, [r4, #4]
00385778: b #0x3852ec
0038577c: ldr r0, [r6, #0x4c]
00385780: bl #0x46d514
00385784: mov r1, r0
00385788: ldr r0, [pc, #0x144]
0038578c: add r0, pc, r0
00385790: bl #0x531bac
00385794: ldr r3, [r4, #4]
00385798: b #0x3852ec
0038579c: ldr r0, [r6, #0x4c]
003857a0: bl #0x46d514
003857a4: mov r1, r0
003857a8: ldr r0, [pc, #0x128]
003857ac: add r0, pc, r0
003857b0: bl #0x531bac
003857b4: ldr r3, [r4, #4]
003857b8: b #0x3852ec
003857bc: ldr r2, [pc, #0x118]
003857c0: add r5, sp, #0x10
003857c4: mov r1, r6
003857c8: add r2, pc, r2
003857cc: mov r0, r5
003857d0: mov r3, #0
003857d4: bl #0x5ed210
003857d8: mov r1, r5
003857dc: add r0, r4, #0x10
003857e0: bl #0x384df8
003857e4: ldr r0, [sp, #0x10]
003857e8: cmp r0, #0
003857ec: beq #0x38524c
003857f0: bl #0x31d584
003857f4: b #0x38524c
003857f8: ldr r0, [r6, #0x4c]
003857fc: bl #0x46d514
00385800: mov r1, r0
00385804: ldr r0, [pc, #0xd4]
00385808: add r0, pc, r0
0038580c: bl #0x531bac
00385810: ldr r3, [r4, #4]
00385814: b #0x3852ec
00385818: ldr r0, [r6, #0x4c]
0038581c: bl #0x46d514
00385820: mov r1, r0
00385824: ldr r0, [pc, #0xb8]
00385828: add r0, pc, r0
0038582c: bl #0x531bac
00385830: ldr r3, [r4, #4]
00385834: b #0x3852ec
00385838: ldr r2, [pc, #0xa8]
0038583c: add r5, sp, #0x14
00385840: mov r1, r6
00385844: add r2, pc, r2
00385848: mov r0, r5
0038584c: mov r3, #0
00385850: bl #0x5ed210
00385854: mov r1, r5
00385858: add r0, r4, #0x10
0038585c: bl #0x384df8
00385860: ldr r0, [sp, #0x14]
00385864: cmp r0, #0
00385868: beq #0x38524c
0038586c: bl #0x31d584
00385870: b #0x38524c
00385874: strhteq pc, [r0], #-0x94
00385878: andeq r0, r0, r4, lsr #27
0038587c: strdeq r3, r4, [r0], -r4
00385880: andeq r1, r0, r0, ror sp
00385884: subseq ip, r3, r4, lsr #25
00385888: andeq r2, r0, r4, asr #11
0038588c: subseq ip, r3, r0, ror #23
00385890: andeq r4, r0, r8, lsr #18

# 0x7a84c4 _ZN8RenderFX10Controller5ResetEv
007a84c4: push {r4, lr}
007a84c8: mov r1, #0
007a84cc: mov r4, r0
007a84d0: add r0, r0, #0x10
007a84d4: bl #0x75518c
007a84d8: add r0, r4, #0x14
007a84dc: mov r1, #0
007a84e0: bl #0x75518c
007a84e4: add r0, r4, #0x18
007a84e8: mov r1, #0
007a84ec: bl #0x75518c
007a84f0: add r0, r4, #0x1c
007a84f4: mov r1, #0
007a84f8: bl #0x75518c
007a84fc: add r0, r4, #0x20
007a8500: mov r1, #0
007a8504: pop {r4, lr}
007a8508: b #0x75518c

# 0x7ab924 _ZN8RenderFX9GotoFrameEPN7gameswf9characterEPKcb
007ab924: push {r4, r5, r6, r7, r8, sl, lr}
007ab928: ldr r4, [pc, #0xcc]
007ab92c: ldr r6, [pc, #0xcc]
007ab930: mov r8, r3
007ab934: add r4, pc, r4
007ab938: ldr r0, [r4, r6]
007ab93c: sub sp, sp, #0x1c
007ab940: subs r5, r1, #0
007ab944: ldr r3, [r0]
007ab948: mov r7, r2
007ab94c: str r3, [sp, #0x14]
007ab950: beq #0x7ab9c8
007ab954: ldr r2, [r5]
007ab958: mov r0, r5
007ab95c: mov r1, #2
007ab960: mov lr, pc
007ab964: ldr pc, [r2, #8]
007ab968: cmp r0, #0
007ab96c: beq #0x7ab9c8
007ab970: ldr r3, [r5]
007ab974: mov r1, r7
007ab978: mov r0, sp
007ab97c: ldr r7, [r3, #0x9c]
007ab980: bl #0x413a7c
007ab984: mov r0, r5
007ab988: mov r1, sp
007ab98c: blx r7
007ab990: ldrsb r3, [sp]
007ab994: mov sl, sp
007ab998: mov r7, r0
007ab99c: cmn r3, #1
007ab9a0: beq #0x7ab9e8
007ab9a4: cmp r7, #0
007ab9a8: beq #0x7ab9c8
007ab9ac: mov r0, r5
007ab9b0: eor r1, r8, #1
007ab9b4: ldr r3, [r5]
007ab9b8: mov lr, pc
007ab9bc: ldr pc, [r3, #0x94]
007ab9c0: mov r0, #1
007ab9c4: b #0x7ab9cc
007ab9c8: mov r0, #0
007ab9cc: ldr r3, [r4, r6]
007ab9d0: ldr r2, [sp, #0x14]
007ab9d4: ldr r3, [r3]
007ab9d8: cmp r2, r3
007ab9dc: bne #0x7ab9f8
007ab9e0: add sp, sp, #0x1c
007ab9e4: pop {r4, r5, r6, r7, r8, sl, pc}
007ab9e8: ldr r0, [sp, #0xc]
007ab9ec: ldr r1, [sp, #8]
007ab9f0: bl #0x752b38
007ab9f4: b #0x7ab9a4
007ab9f8: bl #0x30e310
007ab9fc: andseq sb, lr, ip, asr r1
007aba00: andeq r4, r0, ip, lsr #1

# 0x7aba04 _ZN8RenderFX8PlayAnimEPN7gameswf9characterEPKci
007aba04: mov r3, #1
007aba08: b #0x7ab924

# 0x7ac410 _ZN8RenderFX10ResetFocusEi
007ac410: push {r4, r5, r6, lr}
007ac414: mov r2, r1
007ac418: mov r4, r1
007ac41c: mov r1, #0
007ac420: mov r5, r0
007ac424: bl #0x7ac228
007ac428: mov r0, #0x28
007ac42c: mul r4, r0, r4
007ac430: mov r1, #0
007ac434: add r0, r4, #0x78
007ac438: add r0, r5, r0
007ac43c: pop {r4, r5, r6, lr}
007ac440: b #0x75518c
