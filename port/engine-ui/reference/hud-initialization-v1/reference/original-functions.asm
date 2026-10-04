
# _Z21NativeGetSkillDetailsRKN7gameswf7fn_callE
00445a10: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00445a14: ldr      r1, [pc, #0xc2c]
00445a18: ldr      r2, [pc, #0xc2c]
00445a1c: sub      sp, sp, #0x224
00445a20: add      r1, pc, r1
00445a24: str      r2, [sp, #0x4c]
00445a28: ldr      r2, [r1, r2]
00445a2c: str      r1, [sp, #0x18]
00445a30: str      r0, [sp, #0x20]
00445a34: ldr      r2, [r2]
00445a38: ldr      r3, [r0, #0xc]
00445a3c: ldr      r0, [r0, #0x14]
00445a40: str      r2, [sp, #0x21c]
00445a44: ldr      r3, [r3]
00445a48: mov      r4, #0xc
00445a4c: mla      r0, r4, r0, r3
00445a50: bl       #0x797a54
00445a54: mov      r6, r0
00445a58: ldr      r0, [sp, #0x20]
00445a5c: mov      r7, r1
00445a60: ldr      r3, [r0, #0xc]
00445a64: ldr      r2, [r0, #0x14]
00445a68: ldr      r3, [r3]
00445a6c: sub      r2, r2, #1
00445a70: mla      r4, r4, r2, r3
00445a74: ldrsb    r3, [r4, #1]
00445a78: cmp      r3, #5
00445a7c: ldreq    r0, [r4, #4]
00445a80: movne    r0, #0
00445a84: bl       #0x439cb4
00445a88: ldr      r1, [sp, #0x20]
00445a8c: str      r0, [sp, #0x10]
00445a90: mov      r4, #0xc
00445a94: ldr      r3, [r1, #0xc]
00445a98: ldr      r0, [r1, #0x14]
00445a9c: ldr      r3, [r3]
00445aa0: sub      r0, r0, #2
00445aa4: mla      r0, r4, r0, r3
00445aa8: bl       #0x797a54
00445aac: bl       #0x30ea24
00445ab0: ldr      r2, [sp, #0x20]
00445ab4: mov      r5, r0
00445ab8: ldr      r3, [r2, #0x10]
00445abc: cmp      r3, #4
00445ac0: movne    r1, #0
00445ac4: beq      #0x44658c
00445ac8: mov      r0, r5
00445acc: bl       #0x43c388
00445ad0: cmp      r0, #0
00445ad4: str      r0, [sp, #0x1c]
00445ad8: beq      #0x446104
00445adc: ldr      r2, [sp, #0x18]
00445ae0: ldr      r3, [pc, #0xb68]
00445ae4: add      r1, sp, #0x150
00445ae8: mov      r4, #0
00445aec: ldr      r3, [r2, r3]
00445af0: mov      r0, r1
00445af4: str      r1, [sp, #0x30]
00445af8: add      r3, r3, #8
00445afc: mov      r1, #0x10
00445b00: str      r3, [sp, #0x64]
00445b04: str      r3, [sp, #0x54]
00445b08: str      r0, [sp, #0x160]
00445b0c: str      r0, [sp, #0x164]
00445b10: str      r4, [sp, #0x58]
00445b14: str      r4, [sp, #0x5c]
00445b18: str      r4, [sp, #0x60]
00445b1c: str      r4, [sp, #0x68]
00445b20: str      r4, [sp, #0x6c]
00445b24: str      r4, [sp, #0x70]
00445b28: bl       #0x31167c
00445b2c: ldr      r3, [sp, #0x160]
00445b30: add      r0, sp, #0x138
00445b34: str      r0, [sp, #0x2c]
00445b38: strb     r4, [r3]
00445b3c: ldr      r2, [sp, #0x2c]
00445b40: mov      r1, #0x10
00445b44: ldr      r5, [pc, #0xb08]
00445b48: str      r2, [sp, #0x148]
00445b4c: str      r2, [sp, #0x14c]
00445b50: bl       #0x31167c
00445b54: ldr      r3, [sp, #0x148]
00445b58: mov      r1, r7
00445b5c: mov      r0, r6
00445b60: strb     r4, [r3]
00445b64: add      r3, sp, #0x120
00445b68: str      r3, [sp, #0x44]
00445b6c: bl       #0x30ea24
00445b70: str      r0, [sp, #0x14]
00445b74: ldr      r1, [sp, #0x14]
00445b78: ldr      r0, [sp, #0x1c]
00445b7c: bl       #0x3bbeec
00445b80: add      r5, pc, r5
00445b84: add      r0, sp, #0x108
00445b88: mov      r1, r5
00445b8c: str      r0, [sp, #0x48]
00445b90: add      r2, sp, #0x104
00445b94: ldr      r0, [sp, #0x44]
00445b98: bl       #0x3140ec
00445b9c: mov      r1, r5
00445ba0: add      r2, sp, #0x100
00445ba4: ldr      r0, [sp, #0x48]
00445ba8: bl       #0x3140ec
00445bac: ldr      r1, [sp, #0x14]
00445bb0: ldr      r0, [sp, #0x1c]
00445bb4: bl       #0x3bc784
00445bb8: mov      r5, r0
00445bbc: ldr      r0, [sp, #0x1c]
00445bc0: bl       #0x3bd120
00445bc4: ldr      r3, [r5, #0x20]
00445bc8: cmp      r0, r3
00445bcc: bge      #0x446138
00445bd0: ldr      r1, [pc, #0xa80]
00445bd4: ldr      r2, [sp, #0x18]
00445bd8: str      r1, [sp, #0x40]
00445bdc: ldr      r3, [r2, r1]
00445be0: ldr      r1, [pc, #0xa74]
00445be4: ldr      r2, [pc, #0xa74]
00445be8: ldr      r0, [r3, #0x2c]
00445bec: add      r1, pc, r1
00445bf0: add      r2, pc, r2
00445bf4: ldr      r6, [r3, #0x34]
00445bf8: bl       #0x4c4bdc
00445bfc: mov      r1, r0
00445c00: mov      r0, r6
00445c04: bl       #0x508edc
00445c08: mov      r6, r0
00445c0c: bl       #0x30de54
00445c10: mov      r1, r6
00445c14: add      r2, r6, r0
00445c18: ldr      r0, [sp, #0x44]
00445c1c: bl       #0x3109e0
00445c20: ldr      r3, [sp, #0x60]
00445c24: ldr      r1, [sp, #0x5c]
00445c28: str      r4, [sp, #0x88]
00445c2c: str      r4, [sp, #0x84]
00445c30: cmp      r1, r3
00445c34: mov      r3, #0
00445c38: str      r3, [sp, #0x80]
00445c3c: beq      #0x446630
00445c40: ldr      r0, [sp, #0x80]
00445c44: mov      r3, r1
00445c48: add      r2, sp, #0x84
00445c4c: str      r0, [r3], #4
00445c50: ldr      r0, [r2], #4
00445c54: str      r0, [r1, #4]
00445c58: ldr      r2, [r2]
00445c5c: str      r2, [r3, #4]
00445c60: ldr      r4, [sp, #0x5c]
00445c64: add      r4, r4, #0xc
00445c68: str      r4, [sp, #0x5c]
00445c6c: ldr      r0, [r5, #0x20]
00445c70: bl       #0x30e964
00445c74: str      r0, [r4, #-0xc]
00445c78: ldr      r2, [r5, #0x20]
00445c7c: mov      r3, #0
00445c80: sub      r4, r4, #0xc
00445c84: str      r3, [r4, #8]
00445c88: mov      r8, r3
00445c8c: add      r3, sp, #0x54
00445c90: str      r2, [r4, #4]
00445c94: str      r3, [sp, #0x28]
00445c98: ldr      r0, [sp, #0x18]
00445c9c: ldr      r3, [sp, #0x40]
00445ca0: ldr      r1, [sp, #0x30]
00445ca4: ldr      r2, [sp, #0x134]
00445ca8: ldr      r4, [r0, r3]
00445cac: ldr      r3, [sp, #0x28]
00445cb0: add      r6, sp, #0x208
00445cb4: ldr      r0, [r4, #0x34]
00445cb8: bl       #0x509aec
00445cbc: ldr      r0, [sp, #0x28]
00445cc0: ldr      r1, [sp, #0x2c]
00445cc4: ldr      r2, [sp, #0x11c]
00445cc8: add      r3, r0, #0x10
00445ccc: ldr      r0, [r4, #0x34]
00445cd0: bl       #0x509aec
00445cd4: ldr      r1, [sp, #0x10]
00445cd8: mov      r0, r6
00445cdc: ldr      r3, [r1]
00445ce0: ldr      r1, [pc, #0x97c]
00445ce4: ldr      r7, [r3, #0x1c]
00445ce8: add      r1, pc, r1
00445cec: bl       #0x413a7c
00445cf0: ldr      r1, [r5, #0x40]
00445cf4: cmp      r1, #0
00445cf8: bge      #0x446298
00445cfc: ldr      r1, [pc, #0x964]
00445d00: add      r1, pc, r1
00445d04: add      r4, sp, #0xec
00445d08: mov      r3, #0
00445d0c: mov      r0, r4
00445d10: strb     r3, [sp, #0xed]
00445d14: strb     r3, [sp, #0xec]
00445d18: bl       #0x797350
00445d1c: mov      r2, r4
00445d20: mov      r1, r6
00445d24: ldr      r0, [sp, #0x10]
00445d28: blx      r7
00445d2c: mov      r0, r4
00445d30: bl       #0x797124
00445d34: ldrb     r2, [sp, #0x208]
00445d38: sxtb     r3, r2
00445d3c: cmn      r3, #1
00445d40: beq      #0x4464fc
00445d44: ldr      r0, [sp, #0x10]
00445d48: ldr      r1, [pc, #0x91c]
00445d4c: add      r6, sp, #0x1f4
00445d50: ldr      r3, [r0]
00445d54: add      r1, pc, r1
00445d58: mov      r0, r6
00445d5c: ldr      r7, [r3, #0x1c]
00445d60: bl       #0x413a7c
00445d64: ldr      r1, [r5, #0x34]
00445d68: cmp      r1, #0
00445d6c: bge      #0x4462a8
00445d70: ldr      r1, [pc, #0x8f8]
00445d74: add      r1, pc, r1
00445d78: add      r4, sp, #0xe0
00445d7c: mov      r3, #0
00445d80: mov      r0, r4
00445d84: strb     r3, [sp, #0xe1]
00445d88: strb     r3, [sp, #0xe0]
00445d8c: bl       #0x797350
00445d90: mov      r1, r6
00445d94: mov      r2, r4
00445d98: ldr      r0, [sp, #0x10]
00445d9c: blx      r7
00445da0: mov      r0, r4
00445da4: bl       #0x797124
00445da8: ldrb     r1, [sp, #0x1f4]
00445dac: sxtb     r3, r1
00445db0: cmn      r3, #1
00445db4: beq      #0x44657c
00445db8: ldr      r2, [sp, #0x10]
00445dbc: ldr      r1, [pc, #0x8b0]
00445dc0: add      r6, sp, #0x1e0
00445dc4: ldr      r3, [r2]
00445dc8: add      r4, sp, #0xd4
00445dcc: add      r1, pc, r1
00445dd0: mov      r0, r6
00445dd4: ldr      r7, [r3, #0x1c]
00445dd8: bl       #0x413a7c
00445ddc: mov      r3, #0
00445de0: ldr      r1, [sp, #0x164]
00445de4: mov      r0, r4
00445de8: strb     r3, [sp, #0xd5]
00445dec: strb     r3, [sp, #0xd4]
00445df0: bl       #0x797350
00445df4: mov      r1, r6
00445df8: mov      r2, r4
00445dfc: ldr      r0, [sp, #0x10]
00445e00: blx      r7
00445e04: mov      r0, r4
00445e08: bl       #0x797124
00445e0c: ldrb     r0, [sp, #0x1e0]
00445e10: sxtb     r3, r0
00445e14: cmn      r3, #1
00445e18: beq      #0x44656c
00445e1c: ldr      r1, [sp, #0x10]
00445e20: add      r6, sp, #0x1cc
00445e24: add      r4, sp, #0xc8
00445e28: ldr      r3, [r1]
00445e2c: ldr      r1, [pc, #0x844]
00445e30: mov      r0, r6
00445e34: ldr      r7, [r3, #0x1c]
00445e38: add      r1, pc, r1
00445e3c: bl       #0x413a7c
00445e40: mov      r3, #0
00445e44: ldr      r1, [sp, #0x14c]
00445e48: mov      r0, r4
00445e4c: strb     r3, [sp, #0xc9]
00445e50: strb     r3, [sp, #0xc8]
00445e54: bl       #0x797350
00445e58: mov      r2, r4
00445e5c: mov      r1, r6
00445e60: ldr      r0, [sp, #0x10]
00445e64: blx      r7
00445e68: mov      r0, r4
00445e6c: bl       #0x797124
00445e70: ldrb     r2, [sp, #0x1cc]
00445e74: sxtb     r3, r2
00445e78: cmn      r3, #1
00445e7c: beq      #0x44655c
00445e80: ldr      r0, [sp, #0x10]
00445e84: ldr      r1, [pc, #0x7f0]
00445e88: add      r6, sp, #0x1b8
00445e8c: ldr      r3, [r0]
00445e90: add      r1, pc, r1
00445e94: mov      r0, r6
00445e98: ldr      r7, [r3, #0x1c]
00445e9c: bl       #0x413a7c
00445ea0: ldrb     r3, [r5, #0x2c]
00445ea4: mov      r2, #0
00445ea8: add      r4, sp, #0xbc
00445eac: strb     r2, [sp, #0xbc]
00445eb0: mov      r2, #1
00445eb4: strb     r3, [sp, #0xc0]
00445eb8: mov      r1, r6
00445ebc: strb     r2, [sp, #0xbd]
00445ec0: ldr      r0, [sp, #0x10]
00445ec4: mov      r2, r4
00445ec8: blx      r7
00445ecc: mov      r0, r4
00445ed0: bl       #0x797124
00445ed4: ldrb     r1, [sp, #0x1b8]
00445ed8: sxtb     r3, r1
00445edc: cmn      r3, #1
00445ee0: beq      #0x44654c
00445ee4: ldr      r2, [sp, #0x10]
00445ee8: ldr      r1, [pc, #0x790]
00445eec: add      r6, sp, #0x1a4
00445ef0: ldr      r3, [r2]
00445ef4: add      r4, sp, #0xb0
00445ef8: add      r1, pc, r1
00445efc: mov      r0, r6
00445f00: ldr      r7, [r3, #0x1c]
00445f04: bl       #0x413a7c
00445f08: mov      r3, #0
00445f0c: ldr      r1, [r5, #0x3c]
00445f10: mov      r0, r4
00445f14: strb     r3, [sp, #0xb1]
00445f18: strb     r3, [sp, #0xb0]
00445f1c: bl       #0x797350
00445f20: mov      r1, r6
00445f24: mov      r2, r4
00445f28: ldr      r0, [sp, #0x10]
00445f2c: blx      r7
00445f30: mov      r0, r4
00445f34: bl       #0x797124
00445f38: ldrb     r0, [sp, #0x1a4]
00445f3c: sxtb     r3, r0
00445f40: cmn      r3, #1
00445f44: beq      #0x44653c
00445f48: ldr      r1, [sp, #0x10]
00445f4c: add      r5, sp, #0x190
00445f50: mov      r0, r5
00445f54: ldr      r3, [r1]
00445f58: ldr      r1, [pc, #0x724]
00445f5c: add      r4, sp, #0xa4
00445f60: ldr      r6, [r3, #0x1c]
00445f64: add      r1, pc, r1
00445f68: bl       #0x413a7c
00445f6c: ldr      r1, [sp, #0x14]
00445f70: ldr      r0, [sp, #0x1c]
00445f74: bl       #0x3bbed0
00445f78: mov      r3, #0
00445f7c: strb     r3, [sp, #0xa4]
00445f80: mov      r3, #2
00445f84: strb     r3, [sp, #0xa5]
00445f88: bl       #0x30ed30
00445f8c: mov      r3, #0xb6000000
00445f90: asr      r3, r3, #0x16
00445f94: add      r2, sp, #0x220
00445f98: strd     r0, r1, [r2, r3]
00445f9c: ldr      r3, [sp, #0xf8]
00445fa0: mov      r1, r5
00445fa4: mov      r2, r4
00445fa8: str      r3, [sp, #0xa8]
00445fac: ldr      r3, [sp, #0xfc]
00445fb0: str      r3, [r4, #8]
00445fb4: ldr      r0, [sp, #0x10]
00445fb8: blx      r6
00445fbc: mov      r0, r4
00445fc0: bl       #0x797124
00445fc4: ldrb     r0, [sp, #0x190]
00445fc8: sxtb     r3, r0
00445fcc: cmn      r3, #1
00445fd0: beq      #0x44652c
00445fd4: ldr      r1, [sp, #0x10]
00445fd8: add      r5, sp, #0x17c
00445fdc: mov      r0, r5
00445fe0: ldr      r3, [r1]
00445fe4: ldr      r1, [pc, #0x69c]
00445fe8: add      r4, sp, #0x98
00445fec: ldr      r6, [r3, #0x1c]
00445ff0: add      r1, pc, r1
00445ff4: bl       #0x413a7c
00445ff8: ldr      r1, [sp, #0x14]
00445ffc: ldr      r0, [sp, #0x1c]
00446000: bl       #0x3bbe84
00446004: mov      r3, #0
00446008: strb     r3, [sp, #0x98]
0044600c: mov      r3, #2
00446010: strb     r3, [sp, #0x99]
00446014: bl       #0x30ed30
00446018: mov      r3, #0xb6000000
0044601c: asr      r3, r3, #0x16
00446020: add      r2, sp, #0x220
00446024: strd     r0, r1, [r2, r3]
00446028: ldr      r3, [sp, #0xf8]
0044602c: mov      r1, r5
00446030: mov      r2, r4
00446034: str      r3, [sp, #0x9c]
00446038: ldr      r3, [sp, #0xfc]
0044603c: str      r3, [r4, #8]
00446040: ldr      r0, [sp, #0x10]
00446044: blx      r6
00446048: mov      r0, r4
0044604c: bl       #0x797124
00446050: ldrb     r0, [sp, #0x17c]
00446054: sxtb     r3, r0
00446058: cmn      r3, #1
0044605c: beq      #0x44651c
00446060: ldr      r1, [sp, #0x10]
00446064: add      r6, sp, #0x168
00446068: mov      r0, r6
0044606c: ldr      r3, [r1]
00446070: ldr      r1, [pc, #0x614]
00446074: add      r4, sp, #0x8c
00446078: ldr      r5, [r3, #0x1c]
0044607c: add      r1, pc, r1
00446080: bl       #0x413a7c
00446084: mov      r3, #0
00446088: strb     r3, [sp, #0x8c]
0044608c: mov      r3, #1
00446090: strb     r3, [sp, #0x8d]
00446094: mov      r2, r4
00446098: mov      r1, r6
0044609c: strb     r8, [sp, #0x90]
004460a0: ldr      r0, [sp, #0x10]
004460a4: blx      r5
004460a8: mov      r0, r4
004460ac: bl       #0x797124
004460b0: ldrb     r2, [sp, #0x168]
004460b4: sxtb     r3, r2
004460b8: cmn      r3, #1
004460bc: beq      #0x44650c
004460c0: ldr      r0, [sp, #0x48]
004460c4: bl       #0x3139ac
004460c8: ldr      r0, [sp, #0x44]
004460cc: bl       #0x3139ac
004460d0: ldr      r0, [sp, #0x2c]
004460d4: bl       #0x3139ac
004460d8: ldr      r0, [sp, #0x30]
004460dc: bl       #0x3139ac
004460e0: ldr      r3, [sp, #0x28]
004460e4: add      r0, r3, #0x10
004460e8: ldr      r3, [sp, #0x64]
004460ec: mov      lr, pc
004460f0: ldr      pc, [r3]
004460f4: ldr      r0, [sp, #0x28]
004460f8: ldr      r3, [sp, #0x54]
004460fc: mov      lr, pc
00446100: ldr      pc, [r3]
00446104: ldr      r1, [sp, #0x20]
00446108: ldr      r0, [r1]
0044610c: ldr      r1, [sp, #0x10]
00446110: bl       #0x797250
00446114: ldr      r2, [sp, #0x4c]
00446118: ldr      r0, [sp, #0x18]
0044611c: ldr      r3, [r0, r2]
00446120: ldr      r2, [sp, #0x21c]
00446124: ldr      r3, [r3]
00446128: cmp      r2, r3
0044612c: bne      #0x446644
00446130: add      sp, sp, #0x224
00446134: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00446138: ldr      r0, [sp, #0x1c]
0044613c: ldr      r1, [sp, #0x14]
00446140: bl       #0x3bbed0
00446144: cmp      r0, #0
00446148: ble      #0x4465cc
0044614c: ldrb     r3, [r5, #0x18]
00446150: ldr      r4, [r5, #0x30]
00446154: cmp      r3, #0
00446158: beq      #0x446184
0044615c: cmn      r4, #1
00446160: beq      #0x446490
00446164: mvn      r1, #0
00446168: ldr      r0, [sp, #0x1c]
0044616c: bl       #0x3bb98c
00446170: mov      r1, r0
00446174: ldr      r0, [sp, #0x1c]
00446178: bl       #0x3aeac0
0044617c: ldr      r3, [r0, #8]
00446180: add      r4, r4, r3
00446184: cmp      r4, #0
00446188: blt      #0x446490
0044618c: ldr      r2, [sp, #0x18]
00446190: ldr      r0, [pc, #0x4c0]
00446194: mov      r1, r4
00446198: ldr      r3, [r2, r0]
0044619c: str      r0, [sp, #0x40]
004461a0: ldr      r0, [r3, #0x34]
004461a4: bl       #0x508edc
004461a8: mov      r4, r0
004461ac: bl       #0x30de54
004461b0: add      r2, r4, r0
004461b4: mov      r1, r4
004461b8: ldr      r0, [sp, #0x44]
004461bc: bl       #0x3109e0
004461c0: ldr      r3, [sp, #0x18]
004461c4: ldr      r2, [sp, #0x40]
004461c8: ldr      r4, [pc, #0x4c0]
004461cc: ldr      r7, [r3, r2]
004461d0: ldr      r2, [pc, #0x4bc]
004461d4: add      r4, pc, r4
004461d8: mov      r1, r4
004461dc: add      r2, pc, r2
004461e0: ldr      r0, [r7, #0x2c]
004461e4: bl       #0x4c4bdc
004461e8: mov      r6, r0
004461ec: ldr      r0, [sp, #0x1c]
004461f0: bl       #0x3bb918
004461f4: cmp      r0, #1
004461f8: beq      #0x4465b0
004461fc: ldr      r0, [sp, #0x1c]
00446200: bl       #0x3bb918
00446204: cmp      r0, #2
00446208: beq      #0x446614
0044620c: ldr      r0, [sp, #0x1c]
00446210: ldr      r1, [sp, #0x14]
00446214: bl       #0x3bbed0
00446218: cmp      r6, r0
0044621c: ble      #0x4462c4
00446220: ldr      r0, [sp, #0x1c]
00446224: ldr      r1, [sp, #0x14]
00446228: bl       #0x3bc9ec
0044622c: cmp      r0, #0
00446230: beq      #0x4464a8
00446234: ldrb     r3, [r5, #0x18]
00446238: ldr      r4, [r5, #0x44]
0044623c: cmp      r3, #0
00446240: beq      #0x44626c
00446244: cmn      r4, #1
00446248: beq      #0x4464ec
0044624c: mvn      r1, #0
00446250: ldr      r0, [sp, #0x1c]
00446254: bl       #0x3bb98c
00446258: mov      r1, r0
0044625c: ldr      r0, [sp, #0x1c]
00446260: bl       #0x3aeac0
00446264: ldr      r3, [r0, #8]
00446268: add      r4, r4, r3
0044626c: cmp      r4, #0
00446270: blt      #0x4464ec
00446274: ldr      r1, [sp, #0x18]
00446278: ldr      r0, [sp, #0x40]
0044627c: ldr      r3, [r1, r0]
00446280: mov      r1, r4
00446284: ldr      r0, [r3, #0x34]
00446288: bl       #0x508edc
0044628c: mov      r4, r0
00446290: mov      r0, r4
00446294: b        #0x4462fc
00446298: ldr      r0, [r4, #0x34]
0044629c: bl       #0x508edc
004462a0: mov      r1, r0
004462a4: b        #0x445d04
004462a8: ldr      r0, [sp, #0x18]
004462ac: ldr      r2, [sp, #0x40]
004462b0: ldr      r3, [r0, r2]
004462b4: ldr      r0, [r3, #0x34]
004462b8: bl       #0x508edc
004462bc: mov      r1, r0
004462c0: b        #0x445d78
004462c4: ldr      r1, [sp, #0x18]
004462c8: ldr      r0, [sp, #0x40]
004462cc: ldr      r2, [pc, #0x3c4]
004462d0: ldr      r3, [r1, r0]
004462d4: ldr      r1, [pc, #0x3c0]
004462d8: add      r2, pc, r2
004462dc: ldr      r0, [r3, #0x2c]
004462e0: add      r1, pc, r1
004462e4: ldr      r4, [r3, #0x34]
004462e8: bl       #0x4c4bdc
004462ec: mov      r1, r0
004462f0: mov      r0, r4
004462f4: bl       #0x508edc
004462f8: mov      r4, r0
004462fc: bl       #0x30de54
00446300: mov      r1, r4
00446304: add      r2, r4, r0
00446308: ldr      r0, [sp, #0x48]
0044630c: bl       #0x3109e0
00446310: ldr      r3, [sp, #0x1c]
00446314: add      r2, sp, #0x54
00446318: ldr      r0, [sp, #0x1c]
0044631c: add      r7, sp, #0x74
00446320: str      r2, [sp, #0x28]
00446324: add      r4, r2, #0xc
00446328: ldr      r2, [pc, #0x370]
0044632c: mov      r1, #0
00446330: add      r3, r3, #0x3c8
00446334: add      sb, r7, #4
00446338: str      r1, [sp, #0xc]
0044633c: str      r3, [sp, #0x34]
00446340: add      r0, r0, #0x560
00446344: add      r1, sp, #0xf8
00446348: add      r3, sb, #4
0044634c: str      r0, [sp, #4]
00446350: str      r1, [sp, #0x38]
00446354: mov      r6, #0
00446358: str      r2, [sp, #0x3c]
0044635c: str      r3, [sp, #8]
00446360: ldr      r1, [sp, #0x14]
00446364: ldr      r0, [sp, #0x1c]
00446368: bl       #0x3bbed0
0044636c: ldr      r3, [sp, #0xc]
00446370: ldr      r1, [sp, #0x14]
00446374: add      r2, r3, r0
00446378: ldr      r3, [sp, #0x38]
0044637c: ldr      r0, [sp, #0x34]
00446380: bl       #0x3d7e88
00446384: ldr      r3, [r5, #0xc]
00446388: cmp      r3, #0
0044638c: beq      #0x446470
00446390: ldr      r1, [sp, #0x18]
00446394: ldr      r0, [sp, #0x3c]
00446398: ldr      r3, [sp, #0x28]
0044639c: ldr      r0, [r1, r0]
004463a0: str      r0, [sp]
004463a4: ldr      r0, [sp, #0xc]
004463a8: add      r2, r3, r0, lsl #4
004463ac: add      r2, r2, #4
004463b0: mov      r3, #0
004463b4: str      r2, [sp, #0x24]
004463b8: mov      r8, r3
004463bc: b        #0x446428
004463c0: ldr      r2, [r7]
004463c4: mov      r3, r1
004463c8: str      r2, [r3], #4
004463cc: ldr      r2, [sb]
004463d0: str      r2, [r1, #4]
004463d4: ldr      r0, [sp, #8]
004463d8: ldr      r2, [r0]
004463dc: str      r2, [r3, #4]
004463e0: ldr      fp, [r4, #-4]
004463e4: add      fp, fp, #0xc
004463e8: str      fp, [r4, #-4]
004463ec: mov      r0, sl
004463f0: bl       #0x30e964
004463f4: mov      r1, #0x3b800000
004463f8: bl       #0x30ed6c
004463fc: asr      sl, sl, #8
00446400: sub      r3, fp, #0xc
00446404: mov      r1, #0
00446408: str      r0, [fp, #-0xc]
0044640c: str      sl, [r3, #4]
00446410: str      r1, [r3, #8]
00446414: ldr      r2, [r5, #0xc]
00446418: add      r8, r8, #1
0044641c: mov      r3, r8
00446420: cmp      r2, r8
00446424: bls      #0x446470
00446428: ldr      r2, [r5, #0x10]
0044642c: ldr      r1, [sp]
00446430: ldr      r0, [sp, #4]
00446434: ldr      r2, [r2, r3, lsl #2]
00446438: bl       #0x3dedb4
0044643c: mov      r1, #0
00446440: str      r1, [sp, #0x74]
00446444: str      r6, [sp, #0x78]
00446448: str      r6, [sp, #0x7c]
0044644c: ldmda    r4, {r1, r3}
00446450: mov      sl, r0
00446454: cmp      r1, r3
00446458: bne      #0x4463c0
0044645c: ldr      r0, [sp, #0x24]
00446460: mov      r2, r7
00446464: bl       #0x43f414
00446468: ldr      fp, [r4, #-4]
0044646c: b        #0x4463ec
00446470: ldr      r2, [sp, #0xc]
00446474: add      r4, r4, #0x10
00446478: add      r2, r2, #1
0044647c: cmp      r2, #2
00446480: str      r2, [sp, #0xc]
00446484: bne      #0x446360
00446488: mov      r8, #1
0044648c: b        #0x445c98
00446490: ldr      r2, [pc, #0x20c]
00446494: ldr      r3, [pc, #0x1bc]
00446498: add      r2, pc, r2
0044649c: str      r3, [sp, #0x40]
004464a0: mov      r4, r2
004464a4: b        #0x4461b4
004464a8: ldr      r0, [sp, #0x18]
004464ac: ldr      r2, [sp, #0x40]
004464b0: ldr      r1, [pc, #0x1f0]
004464b4: ldr      r3, [r0, r2]
004464b8: ldr      r2, [pc, #0x1ec]
004464bc: add      r1, pc, r1
004464c0: ldr      r0, [r3, #0x2c]
004464c4: add      r2, pc, r2
004464c8: ldr      r4, [r3, #0x34]
004464cc: bl       #0x4c4bdc
004464d0: mov      r1, r0
004464d4: mov      r0, r4
004464d8: bl       #0x508edc
004464dc: mov      r1, r0
004464e0: ldr      r0, [sp, #0x48]
004464e4: bl       #0x33076c
004464e8: b        #0x446310
004464ec: ldr      r4, [pc, #0x1bc]
004464f0: add      r4, pc, r4
004464f4: mov      r0, r4
004464f8: b        #0x4462fc
004464fc: ldr      r0, [sp, #0x214]
00446500: ldr      r1, [sp, #0x210]
00446504: bl       #0x752b38
00446508: b        #0x445d44
0044650c: ldr      r0, [sp, #0x174]
00446510: ldr      r1, [sp, #0x170]
00446514: bl       #0x752b38
00446518: b        #0x4460c0
0044651c: ldr      r0, [sp, #0x188]
00446520: ldr      r1, [sp, #0x184]
00446524: bl       #0x752b38
00446528: b        #0x446060
0044652c: ldr      r0, [sp, #0x19c]
00446530: ldr      r1, [sp, #0x198]
00446534: bl       #0x752b38
00446538: b        #0x445fd4
0044653c: ldr      r0, [sp, #0x1b0]
00446540: ldr      r1, [sp, #0x1ac]
00446544: bl       #0x752b38
00446548: b        #0x445f48
0044654c: ldr      r0, [sp, #0x1c4]
00446550: ldr      r1, [sp, #0x1c0]
00446554: bl       #0x752b38
00446558: b        #0x445ee4
0044655c: ldr      r0, [sp, #0x1d8]
00446560: ldr      r1, [sp, #0x1d4]
00446564: bl       #0x752b38
00446568: b        #0x445e80
0044656c: ldr      r0, [sp, #0x1ec]
00446570: ldr      r1, [sp, #0x1e8]
00446574: bl       #0x752b38
00446578: b        #0x445e1c
0044657c: ldr      r0, [sp, #0x200]
00446580: ldr      r1, [sp, #0x1fc]
00446584: bl       #0x752b38
00446588: b        #0x445db8
0044658c: ldr      r0, [sp, #0x20]
00446590: ldr      r3, [r0, #0xc]
00446594: ldr      r0, [r0, #0x14]
00446598: ldr      r3, [r3]
0044659c: sub      r0, r0, #3
004465a0: mla      r0, r4, r0, r3
004465a4: bl       #0x797960
004465a8: mov      r1, r0
004465ac: b        #0x445ac8
004465b0: ldr      r2, [pc, #0xfc]
004465b4: ldr      r0, [r7, #0x2c]
004465b8: mov      r1, r4
004465bc: add      r2, pc, r2
004465c0: bl       #0x4c4bdc
004465c4: mov      r6, r0
004465c8: b        #0x44620c
004465cc: ldr      r1, [sp, #0x18]
004465d0: ldr      r0, [pc, #0x80]
004465d4: ldr      r2, [pc, #0xdc]
004465d8: ldr      r3, [r1, r0]
004465dc: ldr      r1, [pc, #0xd8]
004465e0: str      r0, [sp, #0x40]
004465e4: add      r2, pc, r2
004465e8: ldr      r0, [r3, #0x2c]
004465ec: add      r1, pc, r1
004465f0: ldr      r4, [r3, #0x34]
004465f4: bl       #0x4c4bdc
004465f8: mov      r1, r0
004465fc: mov      r0, r4
00446600: bl       #0x508edc
00446604: mov      r1, r0
00446608: ldr      r0, [sp, #0x44]
0044660c: bl       #0x33076c
00446610: b        #0x4461c0
00446614: ldr      r2, [pc, #0xa4]
00446618: ldr      r0, [r7, #0x2c]
0044661c: mov      r1, r4
00446620: add      r2, pc, r2
00446624: bl       #0x4c4bdc
00446628: mov      r6, r0
0044662c: b        #0x44620c
00446630: add      r0, sp, #0x58
00446634: add      r2, sp, #0x80
00446638: bl       #0x43f414
0044663c: ldr      r4, [sp, #0x5c]
00446640: b        #0x445c6c
00446644: bl       #0x30e310
00446648: subseq   pc, r4, r0, ror r0
0044664c: andeq    r4, r0, ip, lsr #1
00446650: andeq    r4, r0, r8, lsl #1
00446654: subeq    r5, r8, r8, lsl #25
00446658: strdeq   r3, r4, [r0], -r4
0044665c: subeq    sb, r7, ip, lsr r0
00446660: subeq    r6, r8, r0, ror #7
00446664: subeq    r6, r8, r8, lsl #7
00446668: subeq    r5, r8, r8, lsl #22
0044666c: subeq    r6, r8, ip, lsr #6
00446670: umaaleq  r5, r8, r4, sl
00446674: subeq    r6, r8, ip, asr #5
00446678: subeq    r6, r8, r0, ror r2
0044667c: subeq    r6, r8, r8, lsr #4
00446680: ldrdeq   r6, r7, [r8], #-0x10
00446684: subeq    r6, r8, r4, ror r1
00446688: strdeq   r6, r7, [r8], #-8
0044668c: subeq    r6, r8, r4, lsl #1
00446690: subeq    fp, r7, ip, ror r5
00446694: subeq    lr, r7, ip, asr #13
00446698: subeq    r5, r8, r8, ror sp
0044669c: subeq    r8, r7, r8, asr #18
004466a0: andeq    r1, r0, ip, asr #32
004466a4: subeq    r5, r8, r0, ror r3
004466a8: subeq    r8, r7, ip, ror #14
004466ac: subeq    r5, r8, ip, asr fp
004466b0: subeq    r5, r8, r8, lsl r3
004466b4: subeq    lr, r7, r4, lsl #6
004466b8: subeq    r5, r8, r4, lsl sl
004466bc: subeq    r8, r7, ip, lsr r6
004466c0: strheq   lr, [r7], #-0x28

# _Z23NativeHUDGetActiveFaeryRKN7gameswf7fn_callE
0044a820: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044a824: ldr      r4, [pc, #0x1ec]
0044a828: ldr      r5, [pc, #0x1ec]
0044a82c: ldr      r3, [r0, #0xc]
0044a830: add      r4, pc, r4
0044a834: ldr      r2, [r4, r5]
0044a838: sub      sp, sp, #0x54
0044a83c: mov      r7, r0
0044a840: ldr      r2, [r2]
0044a844: ldr      r0, [r0, #0x14]
0044a848: mov      r6, #0xc
0044a84c: str      r2, [sp, #0x4c]
0044a850: ldr      r3, [r3]
0044a854: mla      r0, r6, r0, r3
0044a858: bl       #0x797a54
0044a85c: bl       #0x30ea24
0044a860: ldr      r3, [r7, #0x10]
0044a864: cmp      r3, #2
0044a868: beq      #0x44a9a8
0044a86c: mov      r6, #0
0044a870: mov      r1, #0
0044a874: bl       #0x43c388
0044a878: subs     r8, r0, #0
0044a87c: beq      #0x44a98c
0044a880: cmp      r6, #0
0044a884: beq      #0x44a9f0
0044a888: ldr      r1, [pc, #0x190]
0044a88c: ldr      r3, [r6]
0044a890: add      fp, sp, #0x38
0044a894: add      r1, pc, r1
0044a898: mov      r0, fp
0044a89c: ldr      sb, [r3, #0x1c]
0044a8a0: bl       #0x413a7c
0044a8a4: mvn      r1, #0
0044a8a8: mov      r0, r8
0044a8ac: bl       #0x3bb98c
0044a8b0: mov      r3, #0
0044a8b4: strb     r3, [sp, #0xc]
0044a8b8: mov      r3, #2
0044a8bc: strb     r3, [sp, #0xd]
0044a8c0: bl       #0x30ed30
0044a8c4: strd     r0, r1, [sp, #0x18]
0044a8c8: ldr      r3, [sp, #0x18]
0044a8cc: add      sl, sp, #0xc
0044a8d0: mov      r1, fp
0044a8d4: str      r3, [sp, #0x10]
0044a8d8: ldr      r3, [sp, #0x1c]
0044a8dc: mov      r2, sl
0044a8e0: mov      r0, r6
0044a8e4: str      r3, [sl, #8]
0044a8e8: blx      sb
0044a8ec: mov      r0, sl
0044a8f0: bl       #0x797124
0044a8f4: ldrsb    r3, [sp, #0x38]
0044a8f8: cmn      r3, #1
0044a8fc: beq      #0x44a9e0
0044a900: ldr      r1, [pc, #0x11c]
0044a904: ldr      r3, [r6]
0044a908: add      sl, sp, #0x24
0044a90c: add      r1, pc, r1
0044a910: mov      r0, sl
0044a914: ldr      sb, [r3, #0x1c]
0044a918: bl       #0x413a7c
0044a91c: mov      r0, r8
0044a920: mvn      r1, #0
0044a924: bl       #0x3bb98c
0044a928: mvn      r2, #0
0044a92c: mov      r1, r0
0044a930: mov      r0, r8
0044a934: bl       #0x3bbc18
0044a938: mov      r3, #0
0044a93c: cmp      r0, #0
0044a940: movle    r0, #0
0044a944: movgt    r0, #1
0044a948: strb     r3, [sp]
0044a94c: mov      r3, #1
0044a950: strb     r3, [sp, #1]
0044a954: strb     r0, [sp, #4]
0044a958: mov      r1, sl
0044a95c: mov      r2, sp
0044a960: mov      r0, r6
0044a964: blx      sb
0044a968: mov      r0, sp
0044a96c: bl       #0x797124
0044a970: ldrsb    r3, [sp, #0x24]
0044a974: mov      r8, sp
0044a978: cmn      r3, #1
0044a97c: beq      #0x44a9d0
0044a980: ldr      r0, [r7]
0044a984: mov      r1, r6
0044a988: bl       #0x797250
0044a98c: ldr      r3, [r4, r5]
0044a990: ldr      r2, [sp, #0x4c]
0044a994: ldr      r3, [r3]
0044a998: cmp      r2, r3
0044a99c: bne      #0x44aa14
0044a9a0: add      sp, sp, #0x54
0044a9a4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044a9a8: ldr      r3, [r7, #0xc]
0044a9ac: ldr      r2, [r7, #0x14]
0044a9b0: ldr      r3, [r3]
0044a9b4: sub      r2, r2, #1
0044a9b8: mla      r6, r6, r2, r3
0044a9bc: ldrsb    r3, [r6, #1]
0044a9c0: cmp      r3, #5
0044a9c4: ldreq    r6, [r6, #4]
0044a9c8: bne      #0x44a86c
0044a9cc: b        #0x44a870
0044a9d0: ldr      r0, [sp, #0x30]
0044a9d4: ldr      r1, [sp, #0x2c]
0044a9d8: bl       #0x752b38
0044a9dc: b        #0x44a980
0044a9e0: ldr      r0, [sp, #0x44]
0044a9e4: ldr      r1, [sp, #0x40]
0044a9e8: bl       #0x752b38
0044a9ec: b        #0x44a900
0044a9f0: mvn      r1, #0
0044a9f4: bl       #0x3bb98c
0044a9f8: bl       #0x30ed30
0044a9fc: ldr      r6, [r7]
0044aa00: mov      r2, r0
0044aa04: mov      r3, r1
0044aa08: mov      r0, r6
0044aa0c: bl       #0x797488
0044aa10: b        #0x44a98c
0044aa14: bl       #0x30e310
0044aa18: subseq   sl, r4, r0, ror #4
0044aa1c: andeq    r4, r0, ip, lsr #1
0044aa20: subeq    r1, r8, r4, lsr pc
0044aa24: subeq    r1, r8, r4, asr #29

# _Z30NativeSkillGetEquipedSkillsIDsRKN7gameswf7fn_callE
004425a0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004425a4: ldr      r3, [r0, #0x10]
004425a8: sub      sp, sp, #0x1c
004425ac: mov      r4, r0
004425b0: sub      r3, r3, #2
004425b4: cmp      r3, #1
004425b8: bls      #0x4425c4
004425bc: add      sp, sp, #0x1c
004425c0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004425c4: ldr      r3, [r0, #0xc]
004425c8: ldr      r2, [r0, #0x14]
004425cc: mov      r5, #0xc
004425d0: ldr      r3, [r3]
004425d4: mla      r1, r5, r2, r3
004425d8: ldrsb    r1, [r1, #1]
004425dc: cmp      r1, #5
004425e0: bne      #0x4425bc
004425e4: sub      r2, r2, #1
004425e8: mla      r0, r5, r2, r3
004425ec: bl       #0x439d8c
004425f0: cmp      r0, #0
004425f4: beq      #0x4425bc
004425f8: ldr      r3, [r4, #0x10]
004425fc: cmp      r3, #3
00442600: beq      #0x442708
00442604: ldr      r3, [r4, #0xc]
00442608: ldr      r2, [r4, #0x14]
0044260c: ldr      r3, [r3]
00442610: mov      r1, #0xc
00442614: mla      r3, r1, r2, r3
00442618: ldrsb    r2, [r3, #1]
0044261c: cmp      r2, #5
00442620: movne    r0, #0
00442624: ldreq    r0, [r3, #4]
00442628: bl       #0x439ce8
0044262c: ldr      r3, [r4, #0xc]
00442630: mov      sb, r0
00442634: ldr      r0, [r4, #0x14]
00442638: ldr      r3, [r3]
0044263c: mov      r5, #0xc
00442640: sub      r0, r0, #1
00442644: mla      r0, r5, r0, r3
00442648: bl       #0x797a54
0044264c: bl       #0x30ea24
00442650: ldr      r3, [r4, #0x10]
00442654: mov      r6, r0
00442658: cmp      r3, #3
0044265c: movne    r1, #0
00442660: beq      #0x4426e8
00442664: mov      r0, r6
00442668: bl       #0x43c388
0044266c: subs     sl, r0, #0
00442670: beq      #0x4425bc
00442674: mov      r5, #0
00442678: add      r6, sp, #4
0044267c: add      r8, sp, #0x10
00442680: mov      fp, r5
00442684: add      r7, r6, #4
00442688: mov      r1, r5
0044268c: mov      r0, sl
00442690: bl       #0x3bbe68
00442694: mov      r3, #2
00442698: strb     r3, [sp, #5]
0044269c: strb     fp, [sp, #4]
004426a0: bl       #0x30ed30
004426a4: strd     r0, r1, [sp, #0x10]
004426a8: ldr      r3, [r8]
004426ac: ldr      r2, [r8, #4]
004426b0: mov      r1, r6
004426b4: mov      r0, sb
004426b8: str      r3, [r7]
004426bc: str      r2, [r7, #4]
004426c0: bl       #0x799134
004426c4: add      r5, r5, #1
004426c8: mov      r0, r6
004426cc: bl       #0x797124
004426d0: cmp      r5, #3
004426d4: bne      #0x442688
004426d8: ldr      r0, [r4]
004426dc: mov      r1, #1
004426e0: bl       #0x797230
004426e4: b        #0x4425bc
004426e8: ldr      r3, [r4, #0xc]
004426ec: ldr      r0, [r4, #0x14]
004426f0: ldr      r3, [r3]
004426f4: sub      r0, r0, #2
004426f8: mla      r0, r5, r0, r3
004426fc: bl       #0x797960
00442700: mov      r1, r0
00442704: b        #0x442664
00442708: ldr      r3, [r4, #0xc]
0044270c: ldr      r2, [r4, #0x14]
00442710: ldr      r3, [r3]
00442714: sub      r1, r2, #2
00442718: mla      r5, r5, r1, r3
0044271c: ldrb     r1, [r5, #1]
00442720: cmp      r1, #1
00442724: beq      #0x442610
00442728: cmp      r1, #0
0044272c: bne      #0x4425bc
00442730: b        #0x442610
