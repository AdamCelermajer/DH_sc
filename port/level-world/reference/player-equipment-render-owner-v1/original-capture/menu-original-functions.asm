# NativeEquipSkill _Z16NativeEquipSkillRKN7gameswf7fn_callE
0043d63c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0043d640: ldr      r3, [r0, #0xc]
0043d644: mov      r4, r0
0043d648: ldr      r0, [r0, #0x14]
0043d64c: ldr      r3, [r3]
0043d650: mov      r5, #0xc
0043d654: mla      r0, r5, r0, r3
0043d658: bl       #0x797a54
0043d65c: ldr      r3, [r4, #0xc]
0043d660: mov      r8, r0
0043d664: ldr      r0, [r4, #0x14]
0043d668: ldr      r3, [r3]
0043d66c: mov      sb, r1
0043d670: sub      r0, r0, #1
0043d674: mla      r0, r5, r0, r3
0043d678: bl       #0x797a54
0043d67c: ldr      r3, [r4, #0xc]
0043d680: mov      r6, r0
0043d684: ldr      r0, [r4, #0x14]
0043d688: ldr      r3, [r3]
0043d68c: mov      r7, r1
0043d690: sub      r0, r0, #2
0043d694: mla      r0, r5, r0, r3
0043d698: bl       #0x797a54
0043d69c: bl       #0x30ea24
0043d6a0: mov      r1, #0
0043d6a4: bl       #0x43c388
0043d6a8: subs     r4, r0, #0
0043d6ac: beq      #0x43d6d4
0043d6b0: mov      r1, r7
0043d6b4: mov      r0, r6
0043d6b8: bl       #0x30ea24
0043d6bc: mov      r5, r0
0043d6c0: mov      r1, r5
0043d6c4: mov      r0, r4
0043d6c8: bl       #0x3bca84
0043d6cc: cmp      r0, #0
0043d6d0: bne      #0x43d6d8
0043d6d4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0043d6d8: mov      r1, sb
0043d6dc: mov      r0, r8
0043d6e0: bl       #0x30ea24
0043d6e4: mov      r2, r5
0043d6e8: mov      r1, r0
0043d6ec: mov      r0, r4
0043d6f0: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0043d6f4: b        #0x3bbe54
# NativeGetPlayerStats _Z20NativeGetPlayerStatsRKN7gameswf7fn_callE
00446afc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00446b00: ldr      r1, [pc, #0x134]
00446b04: ldr      r2, [pc, #0x134]
00446b08: sub      sp, sp, #0x8d0
00446b0c: sub      sp, sp, #4
00446b10: add      r1, pc, r1
00446b14: str      r1, [sp]
00446b18: ldr      r1, [r1, r2]
00446b1c: str      r2, [sp, #8]
00446b20: ldr      r2, [r0, #0xc]
00446b24: ldr      r1, [r1]
00446b28: ldr      r3, [r0, #0x14]
00446b2c: mov      sb, r0
00446b30: str      r1, [sp, #0x8cc]
00446b34: ldr      r2, [r2]
00446b38: mov      r1, #0xc
00446b3c: mla      r3, r1, r3, r2
00446b40: ldrsb    r2, [r3, #1]
00446b44: cmp      r2, #5
00446b48: ldreq    r0, [r3, #4]
00446b4c: movne    r0, #0
00446b50: bl       #0x439cb4
00446b54: ldr      r3, [sb, #0xc]
00446b58: ldr      r2, [sb, #0x14]
00446b5c: mov      r4, r0
00446b60: ldr      r3, [r3]
00446b64: sub      r2, r2, #1
00446b68: mov      r0, #0xc
00446b6c: mla      r0, r0, r2, r3
00446b70: bl       #0x797a54
00446b74: bl       #0x30ea24
00446b78: mov      r1, #0
00446b7c: bl       #0x43c388
00446b80: subs     r3, r0, #0
00446b84: str      r3, [sp, #4]
00446b88: beq      #0x44935c
00446b8c: bl       #0x3bb7fc
00446b90: sub      r0, r0, #0x120
00446b94: sub      r0, r0, #2
00446b98: cmp      r0, #0x25
00446b9c: addls    pc, pc, r0, lsl #2
00446ba0: b        #0x4493a0
00446ba4: b        #0x449390
00446ba8: b        #0x449390
00446bac: b        #0x449390
00446bb0: b        #0x4493a0
00446bb4: b        #0x4493a0
00446bb8: b        #0x4493a0
00446bbc: b        #0x4493a0
00446bc0: b        #0x4493a0
00446bc4: b        #0x4493a0
00446bc8: b        #0x4493a0
00446bcc: b        #0x4493a0
00446bd0: b        #0x4493a0
00446bd4: b        #0x4493a0
00446bd8: b        #0x4493a0
00446bdc: b        #0x4493a0
00446be0: b        #0x4493a0
00446be4: b        #0x4493a0
00446be8: b        #0x4493a0
00446bec: b        #0x4493a0
00446bf0: b        #0x4493a0
00446bf4: b        #0x4493a0
00446bf8: b        #0x4493a0
00446bfc: b        #0x4493a0
00446c00: b        #0x4493a0
00446c04: b        #0x4493a0
00446c08: b        #0x4493a0
00446c0c: b        #0x4493a0
00446c10: b        #0x4493a0
00446c14: b        #0x4493a0
00446c18: b        #0x4493a0
00446c1c: b        #0x4493a0
00446c20: b        #0x4493a0
00446c24: b        #0x4493a0
00446c28: b        #0x4493a0
00446c2c: b        #0x4493a0
00446c30: b        #0x446cb0
00446c34: b        #0x446cb0
00446c38: b        #0x446cb0
00446c3c: subseq   sp, r4, r0, lsl #31
00446c40: andeq    r4, r0, ip, lsr #1
00446c44: subeq    r4, r8, r8, ror #31
00446c48: subeq    r2, ip, r0, asr #9
00446c4c: subeq    r5, r8, r4, ror #5
00446c50: strheq   pc, [r7], #-0x8c
00446c54: ldrdeq   r5, r6, [r8], #-0x1c
00446c58: subeq    r5, r8, r4, asr r1
00446c5c: ldrdeq   r5, r6, [r8], #-4
00446c60: subeq    r5, r8, r8, asr #32
00446c64: subeq    r4, r8, r0, asr #31
00446c68: subeq    r4, r8, r0, asr #30
00446c6c: strheq   r4, [r8], #-0xe8
00446c70: subeq    r4, r8, ip, lsr #28
00446c74: subeq    r4, r8, r4, lsr #27
00446c78: subeq    r4, r8, r8, lsr #26
00446c7c: subeq    r4, r8, ip, lsr #25
00446c80: subeq    r4, r8, ip, lsr #24
00446c84: strheq   r4, [r8], #-0xb0
00446c88: subeq    r4, r8, r4, lsr fp
00446c8c: subeq    r4, r8, r4, asr #21
00446c90: subeq    r4, r8, r0, lsr sl
00446c94: strheq   r4, [r8], #-0x94
00446c98: subeq    r4, r8, r8, lsr sb
00446c9c: strheq   r4, [r8], #-0x8c
00446ca0: subeq    r4, r8, ip, lsr r8
00446ca4: subeq    r4, r8, r8, asr #15
00446ca8: subeq    r4, r8, r4, asr r7
00446cac: ldrdeq   r4, r5, [r8], #-0x68
00446cb0: mov      sl, #0
00446cb4: mov      fp, #0x40000000
00446cb8: ldr      r1, [sp, #4]
00446cbc: mov      r2, #0
00446cc0: add      r8, sp, #0x8b0
00446cc4: add      r5, r1, #0x560
00446cc8: mov      r0, r5
00446ccc: mov      r1, #0x4f
00446cd0: bl       #0x3df6e0
00446cd4: mov      r1, #0
00446cd8: mov      r6, r0
00446cdc: mov      r0, r5
00446ce0: bl       #0x3df8ac
00446ce4: mov      r2, #0
00446ce8: add      r6, r6, r0, asr #8
00446cec: mov      r1, #0x50
00446cf0: mov      r0, r5
00446cf4: str      r6, [sp, #0x1c]
00446cf8: bl       #0x3df6e0
00446cfc: mov      r1, #0
00446d00: mov      r6, r0
00446d04: mov      r0, r5
00446d08: bl       #0x3df8ac
00446d0c: mov      r2, #0
00446d10: add      r6, r6, r0, asr #8
00446d14: mov      r1, #0x51
00446d18: mov      r0, r5
00446d1c: str      r6, [sp, #0x18]
00446d20: bl       #0x3df6e0
00446d24: mov      r1, #1
00446d28: mov      r6, r0
00446d2c: mov      r0, r5
00446d30: bl       #0x3df8ac
00446d34: mov      r2, #0
00446d38: add      r6, r6, r0, asr #8
00446d3c: mov      r1, #0x52
00446d40: mov      r0, r5
00446d44: str      r6, [sp, #0x14]
00446d48: bl       #0x3df6e0
00446d4c: mov      r1, #1
00446d50: mov      r6, r0
00446d54: mov      r0, r5
00446d58: bl       #0x3df8ac
00446d5c: mov      r2, #0
00446d60: add      r6, r6, r0, asr #8
00446d64: mov      r1, #0x32
00446d68: mov      r0, r5
00446d6c: str      r6, [sp, #0x10]
00446d70: bl       #0x3df6e0
00446d74: mov      r1, #0
00446d78: mov      r6, r0
00446d7c: mov      r0, r5
00446d80: bl       #0x3df81c
00446d84: ldr      r1, [pc, #-0x148]
00446d88: ldr      r3, [r4]
00446d8c: add      r6, r6, r0, asr #8
00446d90: add      r8, r8, #8
00446d94: str      r6, [sp, #0xc]
00446d98: add      r1, pc, r1
00446d9c: mov      r0, r8
00446da0: ldr      r7, [r3, #0x1c]
00446da4: bl       #0x413a7c
00446da8: ldr      r0, [sp, #4]
00446dac: bl       #0x3bb7e8
00446db0: add      r6, sp, #0x360
00446db4: sub      r6, r6, #0xc
00446db8: mov      r3, #0
00446dbc: mov      r1, r0
00446dc0: mov      r0, r6
00446dc4: strb     r3, [sp, #0x355]
00446dc8: strb     r3, [sp, #0x354]
00446dcc: bl       #0x797350
00446dd0: mov      r2, r6
00446dd4: mov      r1, r8
00446dd8: mov      r0, r4
00446ddc: blx      r7
00446de0: mov      r0, r6
00446de4: bl       #0x797124
00446de8: ldrb     r2, [sp, #0x8b8]
00446dec: sxtb     r3, r2
00446df0: cmn      r3, #1
00446df4: beq      #0x4497e0
00446df8: ldr      r1, [pc, #-0x1b8]
00446dfc: ldr      r3, [r4]
00446e00: add      r8, sp, #0x8a0
00446e04: add      r8, r8, #4
00446e08: add      r1, pc, r1
00446e0c: mov      r0, r8
00446e10: ldr      r7, [r3, #0x1c]
00446e14: bl       #0x413a7c
00446e18: ldr      r0, [sp, #4]
00446e1c: bl       #0x3a56a4
00446e20: add      r6, sp, #0x350
00446e24: sub      r6, r6, #8
00446e28: mov      r3, #0
00446e2c: mov      r1, r0
00446e30: mov      r0, r6
00446e34: strb     r3, [sp, #0x349]
00446e38: strb     r3, [sp, #0x348]
00446e3c: bl       #0x797350
00446e40: mov      r1, r8
00446e44: mov      r2, r6
00446e48: mov      r0, r4
00446e4c: blx      r7
00446e50: mov      r0, r6
00446e54: bl       #0x797124
00446e58: ldrb     r1, [sp, #0x8a4]
00446e5c: sxtb     r3, r1
00446e60: cmn      r3, #1
00446e64: beq      #0x4497d0
00446e68: ldr      r1, [pc, #-0x224]
00446e6c: ldr      r3, [r4]
00446e70: add      r8, sp, #0x890
00446e74: add      r1, pc, r1
00446e78: mov      r0, r8
00446e7c: ldr      r6, [r3, #0x1c]
00446e80: bl       #0x413a7c
00446e84: mov      r2, #0
00446e88: strb     r2, [sp, #0x33c]
00446e8c: mov      r3, #0xa9000000
00446e90: mov      r2, #2
00446e94: strb     r2, [sp, #0x33d]
00446e98: asr      r3, r3, #0x14
00446e9c: add      r2, sp, #0x8d0
00446ea0: strd     sl, fp, [r2, r3]
00446ea4: ldr      r3, [sp, #0x360]
00446ea8: add      r7, sp, #0x340
00446eac: sub      r7, r7, #4
00446eb0: str      r3, [sp, #0x340]
00446eb4: ldr      r3, [sp, #0x364]
00446eb8: mov      r1, r8
00446ebc: mov      r2, r7
00446ec0: str      r3, [sp, #0x344]
00446ec4: mov      r0, r4
00446ec8: blx      r6
00446ecc: mov      r0, r7
00446ed0: bl       #0x797124
00446ed4: ldrb     r1, [sp, #0x890]
00446ed8: sxtb     r3, r1
00446edc: cmn      r3, #1
00446ee0: beq      #0x4497c0
00446ee4: ldr      r1, [pc, #-0x29c]
00446ee8: ldr      r3, [r4]
00446eec: add      r7, sp, #0x870
00446ef0: add      r7, r7, #0xc
00446ef4: add      r1, pc, r1
00446ef8: mov      r0, r7
00446efc: ldr      r8, [r3, #0x1c]
00446f00: bl       #0x413a7c
00446f04: mov      r2, #0
00446f08: mov      r1, #0x13
00446f0c: mov      r0, r5
00446f10: bl       #0x3df6e0
00446f14: mov      r3, #0
00446f18: strb     r3, [sp, #0x330]
00446f1c: mov      r3, #2
00446f20: strb     r3, [sp, #0x331]
00446f24: bl       #0x30ed30
00446f28: mov      r3, #0xa9000000
00446f2c: asr      r3, r3, #0x14
00446f30: add      r2, sp, #0x8d0
00446f34: strd     r0, r1, [r2, r3]
00446f38: ldr      r3, [sp, #0x360]
00446f3c: add      r6, sp, #0x330
00446f40: mov      r1, r7
00446f44: str      r3, [sp, #0x334]
00446f48: ldr      r3, [sp, #0x364]
00446f4c: mov      r2, r6
00446f50: mov      r0, r4
00446f54: str      r3, [r6, #8]
00446f58: blx      r8
00446f5c: mov      r0, r6
00446f60: bl       #0x797124
00446f64: ldrb     r1, [sp, #0x87c]
00446f68: sxtb     r3, r1
00446f6c: cmn      r3, #1
00446f70: beq      #0x4497b0
00446f74: ldr      r1, [pc, #-0x328]
00446f78: ldr      r3, [r4]
00446f7c: add      r8, sp, #0x860
00446f80: add      r8, r8, #8
00446f84: add      r1, pc, r1
00446f88: mov      r0, r8
00446f8c: ldr      sl, [r3, #0x1c]
00446f90: bl       #0x413a7c
00446f94: mov      r2, #0
00446f98: mov      r1, #0x24
00446f9c: mov      r0, r5
00446fa0: bl       #0x3df6e0
00446fa4: mov      r3, #0
00446fa8: strb     r3, [sp, #0x324]
00446fac: mov      r3, #2
00446fb0: strb     r3, [sp, #0x325]
00446fb4: bl       #0x30ed30
00446fb8: mov      r3, #0xa9000000
00446fbc: asr      r3, r3, #0x14
00446fc0: add      r2, sp, #0x8d0
00446fc4: strd     r0, r1, [r2, r3]
00446fc8: ldr      r3, [sp, #0x360]
00446fcc: sub      r7, r6, #0xc
00446fd0: mov      r1, r8
00446fd4: str      r3, [sp, #0x328]
00446fd8: ldr      r3, [sp, #0x364]
00446fdc: mov      r2, r7
00446fe0: mov      r0, r4
00446fe4: str      r3, [r6, #-4]
00446fe8: blx      sl
00446fec: mov      r0, r7
00446ff0: bl       #0x797124
00446ff4: ldrb     r1, [sp, #0x868]
00446ff8: sxtb     r3, r1
00446ffc: cmn      r3, #1
00447000: beq      #0x4497a0
00447004: ldr      r1, [pc, #-0x3b4]
00447008: ldr      r3, [r4]
0044700c: add      r7, sp, #0x850
00447010: add      r7, r7, #4
00447014: add      r1, pc, r1
00447018: mov      r0, r7
0044701c: ldr      r8, [r3, #0x1c]
00447020: bl       #0x413a7c
00447024: mov      r2, #0
00447028: mov      r1, #0x25
0044702c: mov      r0, r5
00447030: bl       #0x3df6e0
00447034: mov      r3, #0
00447038: strb     r3, [sp, #0x318]
0044703c: mov      r3, #2
00447040: strb     r3, [sp, #0x319]
00447044: bl       #0x30ed30
00447048: mov      r3, #0xa9000000
0044704c: asr      r3, r3, #0x14
00447050: add      r2, sp, #0x8d0
00447054: strd     r0, r1, [r2, r3]
00447058: ldr      r2, [sp, #0x364]
0044705c: add      r3, sp, #0x320
00447060: sub      r6, r3, #8
00447064: str      r2, [r3]
00447068: ldr      r3, [sp, #0x360]
0044706c: mov      r1, r7
00447070: mov      r2, r6
00447074: str      r3, [sp, #0x31c]
00447078: mov      r0, r4
0044707c: blx      r8
00447080: mov      r0, r6
00447084: bl       #0x797124
00447088: ldrb     r1, [sp, #0x854]
0044708c: sxtb     r3, r1
00447090: cmn      r3, #1
00447094: beq      #0x449790
00447098: ldr      r1, [pc, #-0x444]
0044709c: ldr      r3, [r4]
004470a0: add      r7, sp, #0x840
004470a4: add      r1, pc, r1
004470a8: mov      r0, r7
004470ac: ldr      r8, [r3, #0x1c]
004470b0: bl       #0x413a7c
004470b4: mov      r2, #0
004470b8: mov      r1, #0x26
004470bc: mov      r0, r5
004470c0: bl       #0x3df6e0
004470c4: mov      r3, #0
004470c8: strb     r3, [sp, #0x30c]
004470cc: mov      r3, #2
004470d0: strb     r3, [sp, #0x30d]
004470d4: bl       #0x30ed30
004470d8: mov      r3, #0xa9000000
004470dc: asr      r3, r3, #0x14
004470e0: add      r2, sp, #0x8d0
004470e4: strd     r0, r1, [r2, r3]
004470e8: ldr      r3, [sp, #0x360]
004470ec: add      r6, sp, #0x310
004470f0: sub      r6, r6, #4
004470f4: str      r3, [sp, #0x310]
004470f8: ldr      r3, [sp, #0x364]
004470fc: mov      r1, r7
00447100: mov      r2, r6
00447104: str      r3, [sp, #0x314]
00447108: mov      r0, r4
0044710c: blx      r8
00447110: mov      r0, r6
00447114: bl       #0x797124
00447118: ldrb     r1, [sp, #0x840]
0044711c: sxtb     r3, r1
00447120: cmn      r3, #1
00447124: beq      #0x449780
00447128: ldr      r1, [pc, #-0x4d0]
0044712c: ldr      r3, [r4]
00447130: add      r7, sp, #0x820
00447134: add      r7, r7, #0xc
00447138: add      r1, pc, r1
0044713c: mov      r0, r7
00447140: ldr      r8, [r3, #0x1c]
00447144: bl       #0x413a7c
00447148: mov      r2, #0
0044714c: mov      r1, #0x29
00447150: mov      r0, r5
00447154: bl       #0x3df6e0
00447158: mov      r3, #0
0044715c: strb     r3, [sp, #0x300]
00447160: mov      r3, #2
00447164: strb     r3, [sp, #0x301]
00447168: bl       #0x30ed30
0044716c: mov      r3, #0xa9000000
00447170: asr      r3, r3, #0x14
00447174: add      r2, sp, #0x8d0
00447178: strd     r0, r1, [r2, r3]
0044717c: ldr      r3, [sp, #0x360]
00447180: add      r6, sp, #0x300
00447184: mov      r1, r7
00447188: str      r3, [sp, #0x304]
0044718c: ldr      r3, [sp, #0x364]
00447190: mov      r2, r6
00447194: mov      r0, r4
00447198: str      r3, [r6, #8]
0044719c: blx      r8
004471a0: mov      r0, r6
004471a4: bl       #0x797124
004471a8: ldrb     r1, [sp, #0x82c]
004471ac: sxtb     r3, r1
004471b0: cmn      r3, #1
004471b4: beq      #0x449770
004471b8: ldr      r1, [pc, #-0x55c]
004471bc: ldr      r3, [r4]
004471c0: add      r8, sp, #0x810
004471c4: add      r8, r8, #8
004471c8: add      r1, pc, r1
004471cc: mov      r0, r8
004471d0: ldr      sl, [r3, #0x1c]
004471d4: bl       #0x413a7c
004471d8: mov      r2, #0
004471dc: mov      r1, #0x2a
004471e0: mov      r0, r5
004471e4: bl       #0x3df6e0
004471e8: mov      r3, #0
004471ec: strb     r3, [sp, #0x2f4]
004471f0: mov      r3, #2
004471f4: strb     r3, [sp, #0x2f5]
004471f8: bl       #0x30ed30
004471fc: mov      r3, #0xa9000000
00447200: asr      r3, r3, #0x14
00447204: add      r2, sp, #0x8d0
00447208: strd     r0, r1, [r2, r3]
0044720c: ldr      r3, [sp, #0x360]
00447210: sub      r7, r6, #0xc
00447214: mov      r1, r8
00447218: str      r3, [sp, #0x2f8]
0044721c: ldr      r3, [sp, #0x364]
00447220: mov      r2, r7
00447224: mov      r0, r4
00447228: str      r3, [r6, #-4]
0044722c: blx      sl
00447230: mov      r0, r7
00447234: bl       #0x797124
00447238: ldrb     r1, [sp, #0x818]
0044723c: sxtb     r3, r1
00447240: cmn      r3, #1
00447244: beq      #0x449760
00447248: ldr      r1, [pc, #-0x5e8]
0044724c: ldr      r3, [r4]
00447250: add      r7, sp, #0x800
00447254: add      r7, r7, #4
00447258: add      r1, pc, r1
0044725c: mov      r0, r7
00447260: ldr      r8, [r3, #0x1c]
00447264: bl       #0x413a7c
00447268: mov      r2, #0
0044726c: mov      r1, #0x2b
00447270: mov      r0, r5
00447274: bl       #0x3df6e0
00447278: mov      r3, #0
0044727c: strb     r3, [sp, #0x2e8]
00447280: mov      r3, #2
00447284: strb     r3, [sp, #0x2e9]
00447288: bl       #0x30ed30
0044728c: mov      r3, #0xa9000000
00447290: asr      r3, r3, #0x14
00447294: add      r2, sp, #0x8d0
00447298: strd     r0, r1, [r2, r3]
0044729c: ldr      r2, [sp, #0x364]
004472a0: add      r3, sp, #0x2f0
004472a4: sub      r6, r3, #8
004472a8: str      r2, [r3]
004472ac: ldr      r3, [sp, #0x360]
004472b0: mov      r1, r7
004472b4: mov      r2, r6
004472b8: str      r3, [sp, #0x2ec]
004472bc: mov      r0, r4
004472c0: blx      r8
004472c4: mov      r0, r6
004472c8: bl       #0x797124
004472cc: ldrb     r1, [sp, #0x804]
004472d0: sxtb     r3, r1
004472d4: cmn      r3, #1
004472d8: beq      #0x449750
004472dc: ldr      r1, [pc, #-0x678]
004472e0: ldr      r3, [r4]
004472e4: add      r7, sp, #0x7f0
004472e8: add      r1, pc, r1
004472ec: mov      r0, r7
004472f0: ldr      r8, [r3, #0x1c]
004472f4: bl       #0x413a7c
004472f8: mov      r2, #0
004472fc: mov      r1, #0x21
00447300: mov      r0, r5
00447304: bl       #0x3df6e0
00447308: mov      r3, #0
0044730c: strb     r3, [sp, #0x2dc]
00447310: mov      r3, #2
00447314: strb     r3, [sp, #0x2dd]
00447318: bl       #0x30ed30
0044731c: mov      r3, #0xa9000000
00447320: asr      r3, r3, #0x14
00447324: add      r2, sp, #0x8d0
00447328: strd     r0, r1, [r2, r3]
0044732c: ldr      r3, [sp, #0x360]
00447330: add      r6, sp, #0x2e0
00447334: sub      r6, r6, #4
00447338: str      r3, [sp, #0x2e0]
0044733c: ldr      r3, [sp, #0x364]
00447340: mov      r1, r7
00447344: mov      r2, r6
00447348: str      r3, [sp, #0x2e4]
0044734c: mov      r0, r4
00447350: blx      r8
00447354: mov      r0, r6
00447358: bl       #0x797124
0044735c: ldrb     r1, [sp, #0x7f0]
00447360: sxtb     r3, r1
00447364: cmn      r3, #1
00447368: beq      #0x449740
0044736c: ldr      r1, [pc, #-0x704]
00447370: ldr      r3, [r4]
00447374: add      r7, sp, #0x7d0
00447378: add      r7, r7, #0xc
0044737c: add      r1, pc, r1
00447380: mov      r0, r7
00447384: ldr      r8, [r3, #0x1c]
00447388: bl       #0x413a7c
0044738c: mov      r2, #0
00447390: mov      r1, #0x22
00447394: mov      r0, r5
00447398: bl       #0x3df6e0
0044739c: mov      r3, #0
004473a0: strb     r3, [sp, #0x2d0]
004473a4: mov      r3, #2
004473a8: strb     r3, [sp, #0x2d1]
004473ac: bl       #0x30ed30
004473b0: mov      r3, #0xa9000000
004473b4: asr      r3, r3, #0x14
004473b8: add      r2, sp, #0x8d0
004473bc: strd     r0, r1, [r2, r3]
004473c0: ldr      r3, [sp, #0x360]
004473c4: add      r6, sp, #0x2d0
004473c8: mov      r1, r7
004473cc: str      r3, [sp, #0x2d4]
004473d0: ldr      r3, [sp, #0x364]
004473d4: mov      r2, r6
004473d8: mov      r0, r4
004473dc: str      r3, [r6, #8]
004473e0: blx      r8
004473e4: mov      r0, r6
004473e8: bl       #0x797124
004473ec: ldrb     r1, [sp, #0x7dc]
004473f0: sxtb     r3, r1
004473f4: cmn      r3, #1
004473f8: beq      #0x449730
004473fc: ldr      r1, [pc, #-0x790]
00447400: ldr      r3, [r4]
00447404: add      r8, sp, #0x7c0
00447408: add      r8, r8, #8
0044740c: add      r1, pc, r1
00447410: mov      r0, r8
00447414: ldr      sl, [r3, #0x1c]
00447418: bl       #0x413a7c
0044741c: mov      r2, #0
00447420: mov      r1, #0x95
00447424: mov      r0, r5
00447428: bl       #0x3df6e0
0044742c: mov      r3, #0
00447430: strb     r3, [sp, #0x2c4]
00447434: mov      r3, #2
00447438: strb     r3, [sp, #0x2c5]
0044743c: bl       #0x30ed30
00447440: mov      r3, #0xa9000000
00447444: asr      r3, r3, #0x14
00447448: add      r2, sp, #0x8d0
0044744c: strd     r0, r1, [r2, r3]
00447450: ldr      r3, [sp, #0x360]
00447454: sub      r7, r6, #0xc
00447458: mov      r1, r8
0044745c: str      r3, [sp, #0x2c8]
00447460: ldr      r3, [sp, #0x364]
00447464: mov      r2, r7
00447468: mov      r0, r4
0044746c: str      r3, [r6, #-4]
00447470: blx      sl
00447474: mov      r0, r7
00447478: bl       #0x797124
0044747c: ldrb     r3, [sp, #0x7c8]
00447480: cmp      r3, #0xff
00447484: beq      #0x449720
00447488: ldr      r1, [pc, #-0x818]
0044748c: ldr      r3, [r4]
00447490: add      r7, sp, #0x7b0
00447494: add      r7, r7, #4
00447498: add      r1, pc, r1
0044749c: mov      r0, r7
004474a0: ldr      r8, [r3, #0x1c]
004474a4: bl       #0x413a7c
004474a8: mov      r2, #0
004474ac: mov      r1, #0x96
004474b0: mov      r0, r5
004474b4: bl       #0x3df6e0
004474b8: mov      r3, #0
004474bc: strb     r3, [sp, #0x2b8]
004474c0: mov      r3, #2
004474c4: strb     r3, [sp, #0x2b9]
004474c8: bl       #0x30ed30
004474cc: mov      r3, #0xa9000000
004474d0: asr      r3, r3, #0x14
004474d4: add      r2, sp, #0x8d0
004474d8: strd     r0, r1, [r2, r3]
004474dc: ldr      r2, [sp, #0x364]
004474e0: add      r3, sp, #0x2c0
004474e4: sub      r6, r3, #8
004474e8: str      r2, [r3]
004474ec: ldr      r3, [sp, #0x360]
004474f0: mov      r1, r7
004474f4: mov      r2, r6
004474f8: str      r3, [sp, #0x2bc]
004474fc: mov      r0, r4
00447500: blx      r8
00447504: mov      r0, r6
00447508: bl       #0x797124
0044750c: ldrb     r3, [sp, #0x7b4]
00447510: cmp      r3, #0xff
00447514: beq      #0x449710
00447518: ldr      r1, [pc, #-0x8a4]
0044751c: ldr      r3, [r4]
00447520: add      r7, sp, #0x7a0
00447524: add      r1, pc, r1
00447528: mov      r0, r7
0044752c: ldr      r8, [r3, #0x1c]
00447530: bl       #0x413a7c
00447534: mov      r2, #0
00447538: mov      r1, #0x97
0044753c: mov      r0, r5
00447540: bl       #0x3df6e0
00447544: mov      r3, #0
00447548: strb     r3, [sp, #0x2ac]
0044754c: mov      r3, #2
00447550: strb     r3, [sp, #0x2ad]
00447554: bl       #0x30ed30
00447558: mov      r3, #0xa9000000
0044755c: asr      r3, r3, #0x14
00447560: add      r2, sp, #0x8d0
00447564: strd     r0, r1, [r2, r3]
00447568: ldr      r3, [sp, #0x360]
0044756c: add      r6, sp, #0x2b0
00447570: sub      r6, r6, #4
00447574: str      r3, [sp, #0x2b0]
00447578: ldr      r3, [sp, #0x364]
0044757c: mov      r1, r7
00447580: mov      r2, r6
00447584: str      r3, [sp, #0x2b4]
00447588: mov      r0, r4
0044758c: blx      r8
00447590: mov      r0, r6
00447594: bl       #0x797124
00447598: ldrb     r3, [sp, #0x7a0]
0044759c: cmp      r3, #0xff
004475a0: beq      #0x449700
004475a4: ldr      r1, [pc, #-0x92c]
004475a8: ldr      r3, [r4]
004475ac: add      r7, sp, #0x780
004475b0: add      r7, r7, #0xc
004475b4: add      r1, pc, r1
004475b8: mov      r0, r7
004475bc: ldr      r8, [r3, #0x1c]
004475c0: bl       #0x413a7c
004475c4: mov      r2, #0
004475c8: mov      r1, #0x98
004475cc: mov      r0, r5
004475d0: bl       #0x3df6e0
004475d4: mov      r3, #0
004475d8: strb     r3, [sp, #0x2a0]
004475dc: mov      r3, #2
004475e0: strb     r3, [sp, #0x2a1]
004475e4: bl       #0x30ed30
004475e8: mov      r3, #0xa9000000
004475ec: asr      r3, r3, #0x14
004475f0: add      r2, sp, #0x8d0
004475f4: strd     r0, r1, [r2, r3]
004475f8: ldr      r3, [sp, #0x360]
004475fc: add      r6, sp, #0x2a0
00447600: mov      r1, r7
00447604: str      r3, [sp, #0x2a4]
00447608: ldr      r3, [sp, #0x364]
0044760c: mov      r2, r6
00447610: mov      r0, r4
00447614: str      r3, [r6, #8]
00447618: blx      r8
0044761c: mov      r0, r6
00447620: bl       #0x797124
00447624: ldrb     r3, [sp, #0x78c]
00447628: cmp      r3, #0xff
0044762c: beq      #0x4496f0
00447630: ldr      r1, [pc, #-0x9b4]
00447634: ldr      r3, [r4]
00447638: add      r8, sp, #0x770
0044763c: add      r8, r8, #8
00447640: add      r1, pc, r1
00447644: mov      r0, r8
00447648: ldr      sl, [r3, #0x1c]
0044764c: bl       #0x413a7c
00447650: mov      r2, #0
00447654: mov      r1, #0x94
00447658: mov      r0, r5
0044765c: bl       #0x3df6e0
00447660: mov      r3, #0
00447664: strb     r3, [sp, #0x294]
00447668: mov      r3, #2
0044766c: strb     r3, [sp, #0x295]
00447670: bl       #0x30ed30
00447674: mov      r3, #0xa9000000
00447678: asr      r3, r3, #0x14
0044767c: add      r2, sp, #0x8d0
00447680: strd     r0, r1, [r2, r3]
00447684: ldr      r3, [sp, #0x360]
00447688: sub      r7, r6, #0xc
0044768c: mov      r1, r8
00447690: str      r3, [sp, #0x298]
00447694: ldr      r3, [sp, #0x364]
00447698: mov      r2, r7
0044769c: mov      r0, r4
004476a0: str      r3, [r6, #-4]
004476a4: blx      sl
004476a8: mov      r0, r7
004476ac: bl       #0x797124
004476b0: ldrb     r3, [sp, #0x778]
004476b4: cmp      r3, #0xff
004476b8: beq      #0x4496e0
004476bc: ldr      r1, [pc, #-0xa3c]
004476c0: ldr      r3, [r4]
004476c4: add      r8, sp, #0x760
004476c8: add      r8, r8, #4
004476cc: add      r1, pc, r1
004476d0: mov      r0, r8
004476d4: ldr      r6, [r3, #0x1c]
004476d8: bl       #0x413a7c
004476dc: mov      r3, #0
004476e0: ldr      r0, [sp, #0xc]
004476e4: strb     r3, [sp, #0x288]
004476e8: mov      r3, #2
004476ec: strb     r3, [sp, #0x289]
004476f0: bl       #0x30ed30
004476f4: mov      r3, #0xa9000000
004476f8: asr      r3, r3, #0x14
004476fc: add      r2, sp, #0x8d0
00447700: strd     r0, r1, [r2, r3]
00447704: ldr      r2, [sp, #0x364]
00447708: add      r3, sp, #0x290
0044770c: sub      r7, r3, #8
00447710: str      r2, [r3]
00447714: ldr      r3, [sp, #0x360]
00447718: mov      r1, r8
0044771c: mov      r2, r7
00447720: str      r3, [sp, #0x28c]
00447724: mov      r0, r4
00447728: blx      r6
0044772c: mov      r0, r7
00447730: bl       #0x797124
00447734: ldrb     r3, [sp, #0x764]
00447738: cmp      r3, #0xff
0044773c: beq      #0x4496d0
00447740: ldr      r1, [pc, #-0xabc]
00447744: ldr      r3, [r4]
00447748: add      r7, sp, #0x750
0044774c: add      r1, pc, r1
00447750: mov      r0, r7
00447754: ldr      r8, [r3, #0x1c]
00447758: bl       #0x413a7c
0044775c: mov      r2, #0
00447760: mov      r1, #0x3f
00447764: mov      r0, r5
00447768: bl       #0x3df6e0
0044776c: mov      r1, #0
00447770: mov      r6, r0
00447774: mov      r0, r5
00447778: bl       #0x3df7c4
0044777c: mov      r3, #0
00447780: add      r0, r6, r0, asr #8
00447784: strb     r3, [sp, #0x27c]
00447788: mov      r3, #2
0044778c: strb     r3, [sp, #0x27d]
00447790: bl       #0x30ed30
00447794: mov      r3, #0xa9000000
00447798: asr      r3, r3, #0x14
0044779c: add      r2, sp, #0x8d0
004477a0: strd     r0, r1, [r2, r3]
004477a4: ldr      r3, [sp, #0x360]
004477a8: add      r6, sp, #0x280
004477ac: sub      r6, r6, #4
004477b0: str      r3, [sp, #0x280]
004477b4: ldr      r3, [sp, #0x364]
004477b8: mov      r1, r7
004477bc: mov      r2, r6
004477c0: str      r3, [sp, #0x284]
004477c4: mov      r0, r4
004477c8: blx      r8
004477cc: mov      r0, r6
004477d0: bl       #0x797124
004477d4: ldrb     r3, [sp, #0x750]
004477d8: cmp      r3, #0xff
004477dc: beq      #0x4496c0
004477e0: ldr      r1, [pc, #-0xb58]
004477e4: ldr      r3, [r4]
004477e8: add      r7, sp, #0x730
004477ec: add      r7, r7, #0xc
004477f0: add      r1, pc, r1
004477f4: mov      r0, r7
004477f8: ldr      r8, [r3, #0x1c]
004477fc: bl       #0x413a7c
00447800: mov      r2, #0
00447804: mov      r1, #0x3b
00447808: mov      r0, r5
0044780c: bl       #0x3df6e0
00447810: mov      r3, #0
00447814: strb     r3, [sp, #0x270]
00447818: mov      r3, #2
0044781c: strb     r3, [sp, #0x271]
00447820: bl       #0x30ed30
00447824: mov      r3, #0xa9000000
00447828: asr      r3, r3, #0x14
0044782c: add      r2, sp, #0x8d0
00447830: strd     r0, r1, [r2, r3]
00447834: ldr      r3, [sp, #0x360]
00447838: add      r6, sp, #0x270
0044783c: mov      r1, r7
00447840: str      r3, [sp, #0x274]
00447844: ldr      r3, [sp, #0x364]
00447848: mov      r2, r6
0044784c: mov      r0, r4
00447850: str      r3, [r6, #8]
00447854: blx      r8
00447858: mov      r0, r6
0044785c: bl       #0x797124
00447860: ldrb     r3, [sp, #0x73c]
00447864: cmp      r3, #0xff
00447868: beq      #0x4496b0
0044786c: ldr      r1, [pc, #-0xbe0]
00447870: ldr      r3, [r4]
00447874: add      r8, sp, #0x720
00447878: add      r8, r8, #8
0044787c: add      r1, pc, r1
00447880: mov      r0, r8
00447884: ldr      sl, [r3, #0x1c]
00447888: bl       #0x413a7c
0044788c: mov      r2, #0
00447890: mov      r1, #0x3c
00447894: mov      r0, r5
00447898: bl       #0x3df6e0
0044789c: mov      r3, #0
004478a0: strb     r3, [sp, #0x264]
004478a4: mov      r3, #2
004478a8: strb     r3, [sp, #0x265]
004478ac: bl       #0x30ed30
004478b0: mov      r3, #0xa9000000
004478b4: asr      r3, r3, #0x14
004478b8: add      r2, sp, #0x8d0
004478bc: strd     r0, r1, [r2, r3]
004478c0: ldr      r3, [sp, #0x360]
004478c4: sub      r7, r6, #0xc
004478c8: mov      r1, r8
004478cc: str      r3, [sp, #0x268]
004478d0: ldr      r3, [sp, #0x364]
004478d4: mov      r2, r7
004478d8: mov      r0, r4
004478dc: str      r3, [r6, #-4]
004478e0: blx      sl
004478e4: mov      r0, r7
004478e8: bl       #0x797124
004478ec: ldrb     r3, [sp, #0x728]
004478f0: cmp      r3, #0xff
004478f4: beq      #0x4496a0
004478f8: ldr      r1, [pc, #-0xc68]
004478fc: ldr      r3, [r4]
00447900: add      r7, sp, #0x710
00447904: add      r7, r7, #4
00447908: add      r1, pc, r1
0044790c: mov      r0, r7
00447910: ldr      r8, [r3, #0x1c]
00447914: bl       #0x413a7c
00447918: mov      r2, #0
0044791c: mov      r1, #0x3d
00447920: mov      r0, r5
00447924: bl       #0x3df6e0
00447928: mov      r3, #0
0044792c: strb     r3, [sp, #0x258]
00447930: mov      r3, #2
00447934: strb     r3, [sp, #0x259]
00447938: bl       #0x30ed30
0044793c: mov      r3, #0xa9000000
00447940: asr      r3, r3, #0x14
00447944: add      r2, sp, #0x8d0
00447948: strd     r0, r1, [r2, r3]
0044794c: ldr      r2, [sp, #0x364]
00447950: add      r3, sp, #0x260
00447954: sub      r6, r3, #8
00447958: str      r2, [r3]
0044795c: ldr      r3, [sp, #0x360]
00447960: mov      r1, r7
00447964: mov      r2, r6
00447968: str      r3, [sp, #0x25c]
0044796c: mov      r0, r4
00447970: blx      r8
00447974: mov      r0, r6
00447978: bl       #0x797124
0044797c: ldrb     r3, [sp, #0x714]
00447980: cmp      r3, #0xff
00447984: beq      #0x449690
00447988: ldr      r1, [pc, #-0xcf4]
0044798c: ldr      r3, [r4]
00447990: add      r7, sp, #0x700
00447994: add      r1, pc, r1
00447998: mov      r0, r7
0044799c: ldr      r8, [r3, #0x1c]
004479a0: bl       #0x413a7c
004479a4: mov      r2, #0
004479a8: mov      r1, #0x4a
004479ac: mov      r0, r5
004479b0: bl       #0x3df6e0
004479b4: mov      r3, #0
004479b8: strb     r3, [sp, #0x24c]
004479bc: mov      r3, #2
004479c0: strb     r3, [sp, #0x24d]
004479c4: bl       #0x30ed30
004479c8: mov      r3, #0xa9000000
004479cc: asr      r3, r3, #0x14
004479d0: add      r2, sp, #0x8d0
004479d4: strd     r0, r1, [r2, r3]
004479d8: ldr      r3, [sp, #0x360]
004479dc: add      r6, sp, #0x250
004479e0: sub      r6, r6, #4
004479e4: str      r3, [sp, #0x250]
004479e8: ldr      r3, [sp, #0x364]
004479ec: mov      r1, r7
004479f0: mov      r2, r6
004479f4: str      r3, [sp, #0x254]
004479f8: mov      r0, r4
004479fc: blx      r8
00447a00: mov      r0, r6
00447a04: bl       #0x797124
00447a08: ldrb     r3, [sp, #0x700]
00447a0c: cmp      r3, #0xff
00447a10: beq      #0x449680
00447a14: ldr      r1, [pc, #-0xd7c]
00447a18: ldr      r3, [r4]
00447a1c: add      r7, sp, #0x6e0
00447a20: add      r7, r7, #0xc
00447a24: add      r1, pc, r1
00447a28: mov      r0, r7
00447a2c: ldr      r8, [r3, #0x1c]
00447a30: bl       #0x413a7c
00447a34: mov      r2, #0
00447a38: mov      r1, #0x4d
00447a3c: mov      r0, r5
00447a40: bl       #0x3df6e0
00447a44: mov      r3, #0
00447a48: strb     r3, [sp, #0x240]
00447a4c: mov      r3, #2
00447a50: strb     r3, [sp, #0x241]
00447a54: bl       #0x30ed30
00447a58: mov      r3, #0xa9000000
00447a5c: asr      r3, r3, #0x14
00447a60: add      r2, sp, #0x8d0
00447a64: strd     r0, r1, [r2, r3]
00447a68: ldr      r3, [sp, #0x360]
00447a6c: add      r6, sp, #0x240
00447a70: mov      r1, r7
00447a74: str      r3, [sp, #0x244]
00447a78: ldr      r3, [sp, #0x364]
00447a7c: mov      r2, r6
00447a80: mov      r0, r4
00447a84: str      r3, [r6, #8]
00447a88: blx      r8
00447a8c: mov      r0, r6
00447a90: bl       #0x797124
00447a94: ldrb     r3, [sp, #0x6ec]
00447a98: cmp      r3, #0xff
00447a9c: beq      #0x449670
00447aa0: ldr      r1, [pc, #-0xe04]
00447aa4: ldr      r3, [r4]
00447aa8: add      r8, sp, #0x6d0
00447aac: add      r8, r8, #8
00447ab0: add      r1, pc, r1
00447ab4: mov      r0, r8
00447ab8: ldr      sl, [r3, #0x1c]
00447abc: bl       #0x413a7c
00447ac0: mov      r2, #0
00447ac4: mov      r1, #0x4b
00447ac8: mov      r0, r5
00447acc: bl       #0x3df6e0
00447ad0: mov      r3, #0
00447ad4: strb     r3, [sp, #0x234]
00447ad8: mov      r3, #2
00447adc: strb     r3, [sp, #0x235]
00447ae0: bl       #0x30ed30
00447ae4: mov      r3, #0xa9000000
00447ae8: asr      r3, r3, #0x14
00447aec: add      r2, sp, #0x8d0
00447af0: strd     r0, r1, [r2, r3]
00447af4: ldr      r3, [sp, #0x360]
00447af8: sub      r7, r6, #0xc
00447afc: mov      r1, r8
00447b00: str      r3, [sp, #0x238]
00447b04: ldr      r3, [sp, #0x364]
00447b08: mov      r2, r7
00447b0c: mov      r0, r4
00447b10: str      r3, [r6, #-4]
00447b14: blx      sl
00447b18: mov      r0, r7
00447b1c: bl       #0x797124
00447b20: ldrb     r3, [sp, #0x6d8]
00447b24: cmp      r3, #0xff
00447b28: beq      #0x449660
00447b2c: ldr      r1, [pc, #-0xe8c]
00447b30: ldr      r3, [r4]
00447b34: add      r7, sp, #0x6c0
00447b38: add      r7, r7, #4
00447b3c: add      r1, pc, r1
00447b40: mov      r0, r7
00447b44: ldr      r8, [r3, #0x1c]
00447b48: bl       #0x413a7c
00447b4c: mov      r2, #0
00447b50: mov      r1, #0x4e
00447b54: mov      r0, r5
00447b58: bl       #0x3df6e0
00447b5c: mov      r3, #0
00447b60: strb     r3, [sp, #0x228]
00447b64: mov      r3, #2
00447b68: strb     r3, [sp, #0x229]
00447b6c: bl       #0x30ed30
00447b70: mov      r3, #0xa9000000
00447b74: asr      r3, r3, #0x14
00447b78: add      r2, sp, #0x8d0
00447b7c: strd     r0, r1, [r2, r3]
00447b80: ldr      r2, [sp, #0x364]
00447b84: add      r3, sp, #0x230
00447b88: sub      r6, r3, #8
00447b8c: str      r2, [r3]
00447b90: ldr      r3, [sp, #0x360]
00447b94: mov      r1, r7
00447b98: mov      r2, r6
00447b9c: str      r3, [sp, #0x22c]
00447ba0: mov      r0, r4
00447ba4: blx      r8
00447ba8: mov      r0, r6
00447bac: bl       #0x797124
00447bb0: ldrb     r3, [sp, #0x6c4]
00447bb4: cmp      r3, #0xff
00447bb8: beq      #0x449650
00447bbc: ldr      r1, [pc, #-0xf18]
00447bc0: ldr      r3, [r4]
00447bc4: add      r7, sp, #0x6b0
00447bc8: add      r1, pc, r1
00447bcc: mov      r0, r7
00447bd0: ldr      r8, [r3, #0x1c]
00447bd4: bl       #0x413a7c
00447bd8: mov      r2, #0
00447bdc: mov      r1, #0x4c
00447be0: mov      r0, r5
00447be4: bl       #0x3df6e0
00447be8: mov      r3, #0
00447bec: strb     r3, [sp, #0x21c]
00447bf0: mov      r3, #2
00447bf4: strb     r3, [sp, #0x21d]
00447bf8: bl       #0x30ed30
00447bfc: mov      r3, #0xa9000000
00447c00: asr      r3, r3, #0x14
00447c04: add      r2, sp, #0x8d0
00447c08: strd     r0, r1, [r2, r3]
00447c0c: ldr      r3, [sp, #0x360]
00447c10: add      r6, sp, #0x220
00447c14: sub      r6, r6, #4
00447c18: str      r3, [sp, #0x220]
00447c1c: ldr      r3, [sp, #0x364]
00447c20: mov      r1, r7
00447c24: mov      r2, r6
00447c28: str      r3, [sp, #0x224]
00447c2c: mov      r0, r4
00447c30: blx      r8
00447c34: mov      r0, r6
00447c38: bl       #0x797124
00447c3c: ldrb     r3, [sp, #0x6b0]
00447c40: cmp      r3, #0xff
00447c44: beq      #0x449640
00447c48: ldr      r1, [pc, #0xf90]
00447c4c: ldr      r3, [r4]
00447c50: add      r8, sp, #0x690
00447c54: add      r8, r8, #0xc
00447c58: add      r1, pc, r1
00447c5c: mov      r0, r8
00447c60: ldr      r7, [r3, #0x1c]
00447c64: bl       #0x413a7c
00447c68: mov      r3, #0
00447c6c: ldr      r0, [sp, #0x1c]
00447c70: strb     r3, [sp, #0x210]
00447c74: mov      r3, #2
00447c78: strb     r3, [sp, #0x211]
00447c7c: bl       #0x30ed30
00447c80: mov      r3, #0xa9000000
00447c84: asr      r3, r3, #0x14
00447c88: add      r2, sp, #0x8d0
00447c8c: strd     r0, r1, [r2, r3]
00447c90: ldr      r3, [sp, #0x360]
00447c94: add      r6, sp, #0x210
00447c98: mov      r1, r8
00447c9c: str      r3, [sp, #0x214]
00447ca0: ldr      r3, [sp, #0x364]
00447ca4: mov      r2, r6
00447ca8: mov      r0, r4
00447cac: str      r3, [r6, #8]
00447cb0: blx      r7
00447cb4: mov      r0, r6
00447cb8: bl       #0x797124
00447cbc: ldrb     r3, [sp, #0x69c]
00447cc0: cmp      r3, #0xff
00447cc4: beq      #0x449630
00447cc8: ldr      r1, [pc, #0xf14]
00447ccc: ldr      r3, [r4]
00447cd0: add      sl, sp, #0x680
00447cd4: add      sl, sl, #8
00447cd8: add      r1, pc, r1
00447cdc: mov      r0, sl
00447ce0: ldr      r8, [r3, #0x1c]
00447ce4: bl       #0x413a7c
00447ce8: mov      r3, #0
00447cec: ldr      r0, [sp, #0x18]
00447cf0: strb     r3, [sp, #0x204]
00447cf4: mov      r3, #2
00447cf8: strb     r3, [sp, #0x205]
00447cfc: bl       #0x30ed30
00447d00: mov      r3, #0xa9000000
00447d04: asr      r3, r3, #0x14
00447d08: add      r2, sp, #0x8d0
00447d0c: strd     r0, r1, [r2, r3]
00447d10: ldr      r3, [sp, #0x360]
00447d14: sub      r7, r6, #0xc
00447d18: mov      r1, sl
00447d1c: str      r3, [sp, #0x208]
00447d20: ldr      r3, [sp, #0x364]
00447d24: mov      r2, r7
00447d28: mov      r0, r4
00447d2c: str      r3, [r6, #-4]
00447d30: blx      r8
00447d34: mov      r0, r7
00447d38: bl       #0x797124
00447d3c: ldrb     r3, [sp, #0x688]
00447d40: cmp      r3, #0xff
00447d44: beq      #0x449620
00447d48: ldr      r1, [pc, #0xe98]
00447d4c: ldr      r3, [r4]
00447d50: add      r7, sp, #0x670
00447d54: add      r7, r7, #4
00447d58: add      r1, pc, r1
00447d5c: mov      r0, r7
00447d60: ldr      r8, [r3, #0x1c]
00447d64: bl       #0x413a7c
00447d68: mov      r2, #0
00447d6c: mov      r1, #0x5f
00447d70: mov      r0, r5
00447d74: bl       #0x3df6e0
00447d78: mov      r3, #0
00447d7c: strb     r3, [sp, #0x1f8]
00447d80: mov      r3, #2
00447d84: strb     r3, [sp, #0x1f9]
00447d88: bl       #0x30ed30
00447d8c: mov      r3, #0xa9000000
00447d90: asr      r3, r3, #0x14
00447d94: add      r2, sp, #0x8d0
00447d98: strd     r0, r1, [r2, r3]
00447d9c: ldr      r2, [sp, #0x364]
00447da0: add      r3, sp, #0x200
00447da4: sub      r6, r3, #8
00447da8: str      r2, [r3]
00447dac: ldr      r3, [sp, #0x360]
00447db0: mov      r1, r7
00447db4: mov      r2, r6
00447db8: str      r3, [sp, #0x1fc]
00447dbc: mov      r0, r4
00447dc0: blx      r8
00447dc4: mov      r0, r6
00447dc8: bl       #0x797124
00447dcc: ldrb     r3, [sp, #0x674]
00447dd0: cmp      r3, #0xff
00447dd4: beq      #0x449610
00447dd8: ldr      r1, [pc, #0xe0c]
00447ddc: ldr      r3, [r4]
00447de0: add      r7, sp, #0x660
00447de4: add      r1, pc, r1
00447de8: mov      r0, r7
00447dec: ldr      r8, [r3, #0x1c]
00447df0: bl       #0x413a7c
00447df4: mov      r2, #0
00447df8: mov      r1, #0x60
00447dfc: mov      r0, r5
00447e00: bl       #0x3df6e0
00447e04: mov      r3, #0
00447e08: strb     r3, [sp, #0x1ec]
00447e0c: mov      r3, #2
00447e10: strb     r3, [sp, #0x1ed]
00447e14: bl       #0x30ed30
00447e18: mov      r3, #0xa9000000
00447e1c: asr      r3, r3, #0x14
00447e20: add      r2, sp, #0x8d0
00447e24: strd     r0, r1, [r2, r3]
00447e28: ldr      r3, [sp, #0x360]
00447e2c: add      r6, sp, #0x1f0
00447e30: sub      r6, r6, #4
00447e34: str      r3, [sp, #0x1f0]
00447e38: ldr      r3, [sp, #0x364]
00447e3c: mov      r1, r7
00447e40: mov      r2, r6
00447e44: str      r3, [sp, #0x1f4]
00447e48: mov      r0, r4
00447e4c: blx      r8
00447e50: mov      r0, r6
00447e54: bl       #0x797124
00447e58: ldrb     r3, [sp, #0x660]
00447e5c: cmp      r3, #0xff
00447e60: beq      #0x449600
00447e64: ldr      r1, [pc, #0xd84]
00447e68: ldr      r3, [r4]
00447e6c: add      r7, sp, #0x640
00447e70: add      r7, r7, #0xc
00447e74: add      r1, pc, r1
00447e78: mov      r0, r7
00447e7c: ldr      r8, [r3, #0x1c]
00447e80: bl       #0x413a7c
00447e84: mov      r2, #0
00447e88: mov      r1, #0x61
00447e8c: mov      r0, r5
00447e90: bl       #0x3df6e0
00447e94: mov      r3, #0
00447e98: strb     r3, [sp, #0x1e0]
00447e9c: mov      r3, #2
00447ea0: strb     r3, [sp, #0x1e1]
00447ea4: bl       #0x30ed30
00447ea8: mov      r3, #0xa9000000
00447eac: asr      r3, r3, #0x14
00447eb0: add      r2, sp, #0x8d0
00447eb4: strd     r0, r1, [r2, r3]
00447eb8: ldr      r3, [sp, #0x360]
00447ebc: add      r6, sp, #0x1e0
00447ec0: mov      r1, r7
00447ec4: str      r3, [sp, #0x1e4]
00447ec8: ldr      r3, [sp, #0x364]
00447ecc: mov      r2, r6
00447ed0: mov      r0, r4
00447ed4: str      r3, [r6, #8]
00447ed8: blx      r8
00447edc: mov      r0, r6
00447ee0: bl       #0x797124
00447ee4: ldrb     r3, [sp, #0x64c]
00447ee8: cmp      r3, #0xff
00447eec: beq      #0x4495f0
00447ef0: ldr      r1, [pc, #0xcfc]
00447ef4: ldr      r3, [r4]
00447ef8: add      sl, sp, #0x630
00447efc: add      sl, sl, #8
00447f00: add      r1, pc, r1
00447f04: mov      r0, sl
00447f08: ldr      r8, [r3, #0x1c]
00447f0c: bl       #0x413a7c
00447f10: mov      r3, #0
00447f14: ldr      r0, [sp, #0x14]
00447f18: strb     r3, [sp, #0x1d4]
00447f1c: mov      r3, #2
00447f20: strb     r3, [sp, #0x1d5]
00447f24: bl       #0x30ed30
00447f28: mov      r3, #0xa9000000
00447f2c: asr      r3, r3, #0x14
00447f30: add      r2, sp, #0x8d0
00447f34: strd     r0, r1, [r2, r3]
00447f38: ldr      r3, [sp, #0x360]
00447f3c: sub      r7, r6, #0xc
00447f40: mov      r1, sl
00447f44: str      r3, [sp, #0x1d8]
00447f48: ldr      r3, [sp, #0x364]
00447f4c: mov      r2, r7
00447f50: mov      r0, r4
00447f54: str      r3, [r6, #-4]
00447f58: blx      r8
00447f5c: mov      r0, r7
00447f60: bl       #0x797124
00447f64: ldrb     r3, [sp, #0x638]
00447f68: cmp      r3, #0xff
00447f6c: beq      #0x4495e0
00447f70: ldr      r1, [pc, #0xc80]
00447f74: ldr      r3, [r4]
00447f78: add      r8, sp, #0x620
00447f7c: add      r8, r8, #4
00447f80: add      r1, pc, r1
00447f84: mov      r0, r8
00447f88: ldr      r7, [r3, #0x1c]
00447f8c: bl       #0x413a7c
00447f90: mov      r3, #0
00447f94: ldr      r0, [sp, #0x10]
00447f98: strb     r3, [sp, #0x1c8]
00447f9c: mov      r3, #2
00447fa0: strb     r3, [sp, #0x1c9]
00447fa4: bl       #0x30ed30
00447fa8: mov      r3, #0xa9000000
00447fac: asr      r3, r3, #0x14
00447fb0: add      r2, sp, #0x8d0
00447fb4: strd     r0, r1, [r2, r3]
00447fb8: ldr      r2, [sp, #0x364]
00447fbc: add      r3, sp, #0x1d0
00447fc0: sub      r6, r3, #8
00447fc4: str      r2, [r3]
00447fc8: ldr      r3, [sp, #0x360]
00447fcc: mov      r1, r8
00447fd0: mov      r2, r6
00447fd4: str      r3, [sp, #0x1cc]
00447fd8: mov      r0, r4
00447fdc: blx      r7
00447fe0: mov      r0, r6
00447fe4: bl       #0x797124
00447fe8: ldrb     r3, [sp, #0x624]
00447fec: cmp      r3, #0xff
00447ff0: beq      #0x4495d0
00447ff4: ldr      r1, [pc, #0xc00]
00447ff8: ldr      r3, [r4]
00447ffc: add      r7, sp, #0x610
00448000: add      r1, pc, r1
00448004: mov      r0, r7
00448008: ldr      r8, [r3, #0x1c]
0044800c: bl       #0x413a7c
00448010: mov      r2, #0
00448014: mov      r1, #0x62
00448018: mov      r0, r5
0044801c: bl       #0x3df6e0
00448020: mov      r3, #0
00448024: strb     r3, [sp, #0x1bc]
00448028: mov      r3, #2
0044802c: strb     r3, [sp, #0x1bd]
00448030: bl       #0x30ed30
00448034: mov      r3, #0xa9000000
00448038: asr      r3, r3, #0x14
0044803c: add      r2, sp, #0x8d0
00448040: strd     r0, r1, [r2, r3]
00448044: ldr      r3, [sp, #0x360]
00448048: add      r6, sp, #0x1c0
0044804c: sub      r6, r6, #4
00448050: str      r3, [sp, #0x1c0]
00448054: ldr      r3, [sp, #0x364]
00448058: mov      r1, r7
0044805c: mov      r2, r6
00448060: str      r3, [sp, #0x1c4]
00448064: mov      r0, r4
00448068: blx      r8
0044806c: mov      r0, r6
00448070: bl       #0x797124
00448074: ldrb     r3, [sp, #0x610]
00448078: cmp      r3, #0xff
0044807c: beq      #0x4495c0
00448080: ldr      r1, [pc, #0xb78]
00448084: ldr      r3, [r4]
00448088: add      r7, sp, #0x5f0
0044808c: add      r7, r7, #0xc
00448090: add      r1, pc, r1
00448094: mov      r0, r7
00448098: ldr      r8, [r3, #0x1c]
0044809c: bl       #0x413a7c
004480a0: mov      r2, #0
004480a4: mov      r1, #0x63
004480a8: mov      r0, r5
004480ac: bl       #0x3df6e0
004480b0: mov      r3, #0
004480b4: strb     r3, [sp, #0x1b0]
004480b8: mov      r3, #2
004480bc: strb     r3, [sp, #0x1b1]
004480c0: bl       #0x30ed30
004480c4: mov      r3, #0xa9000000
004480c8: asr      r3, r3, #0x14
004480cc: add      r2, sp, #0x8d0
004480d0: strd     r0, r1, [r2, r3]
004480d4: ldr      r3, [sp, #0x360]
004480d8: add      r6, sp, #0x1b0
004480dc: mov      r1, r7
004480e0: str      r3, [sp, #0x1b4]
004480e4: ldr      r3, [sp, #0x364]
004480e8: mov      r2, r6
004480ec: mov      r0, r4
004480f0: str      r3, [r6, #8]
004480f4: blx      r8
004480f8: mov      r0, r6
004480fc: bl       #0x797124
00448100: ldrb     r3, [sp, #0x5fc]
00448104: cmp      r3, #0xff
00448108: beq      #0x4495b0
0044810c: ldr      r1, [pc, #0xaf0]
00448110: ldr      r3, [r4]
00448114: add      r8, sp, #0x5e0
00448118: add      r8, r8, #8
0044811c: add      r1, pc, r1
00448120: mov      r0, r8
00448124: ldr      sl, [r3, #0x1c]
00448128: bl       #0x413a7c
0044812c: mov      r2, #0
00448130: mov      r1, #0x64
00448134: mov      r0, r5
00448138: bl       #0x3df6e0
0044813c: mov      r3, #0
00448140: strb     r3, [sp, #0x1a4]
00448144: mov      r3, #2
00448148: strb     r3, [sp, #0x1a5]
0044814c: bl       #0x30ed30
00448150: mov      r3, #0xa9000000
00448154: asr      r3, r3, #0x14
00448158: add      r2, sp, #0x8d0
0044815c: strd     r0, r1, [r2, r3]
00448160: ldr      r3, [sp, #0x360]
00448164: sub      r7, r6, #0xc
00448168: mov      r1, r8
0044816c: str      r3, [sp, #0x1a8]
00448170: ldr      r3, [sp, #0x364]
00448174: mov      r2, r7
00448178: mov      r0, r4
0044817c: str      r3, [r6, #-4]
00448180: blx      sl
00448184: mov      r0, r7
00448188: bl       #0x797124
0044818c: ldrb     r3, [sp, #0x5e8]
00448190: cmp      r3, #0xff
00448194: beq      #0x4495a0
00448198: ldr      r1, [pc, #0xa68]
0044819c: ldr      r3, [r4]
004481a0: add      r7, sp, #0x5d0
004481a4: add      r7, r7, #4
004481a8: add      r1, pc, r1
004481ac: mov      r0, r7
004481b0: ldr      r8, [r3, #0x1c]
004481b4: bl       #0x413a7c
004481b8: mov      r2, #0
004481bc: mov      r1, #0x65
004481c0: mov      r0, r5
004481c4: bl       #0x3df6e0
004481c8: mov      r3, #0
004481cc: strb     r3, [sp, #0x198]
004481d0: mov      r3, #2
004481d4: strb     r3, [sp, #0x199]
004481d8: bl       #0x30ed30
004481dc: mov      r3, #0xa9000000
004481e0: asr      r3, r3, #0x14
004481e4: add      r2, sp, #0x8d0
004481e8: strd     r0, r1, [r2, r3]
004481ec: ldr      r2, [sp, #0x364]
004481f0: add      r3, sp, #0x1a0
004481f4: sub      r6, r3, #8
004481f8: str      r2, [r3]
004481fc: ldr      r3, [sp, #0x360]
00448200: mov      r1, r7
00448204: mov      r2, r6
00448208: str      r3, [sp, #0x19c]
0044820c: mov      r0, r4
00448210: blx      r8
00448214: mov      r0, r6
00448218: bl       #0x797124
0044821c: ldrb     r3, [sp, #0x5d4]
00448220: cmp      r3, #0xff
00448224: beq      #0x449590
00448228: ldr      r1, [pc, #0x9dc]
0044822c: ldr      r3, [r4]
00448230: add      r7, sp, #0x5c0
00448234: add      r1, pc, r1
00448238: mov      r0, r7
0044823c: ldr      r8, [r3, #0x1c]
00448240: bl       #0x413a7c
00448244: mov      r2, #0
00448248: mov      r1, #0x66
0044824c: mov      r0, r5
00448250: bl       #0x3df6e0
00448254: mov      r3, #0
00448258: strb     r3, [sp, #0x18c]
0044825c: mov      r3, #2
00448260: strb     r3, [sp, #0x18d]
00448264: bl       #0x30ed30
00448268: mov      r3, #0xa9000000
0044826c: asr      r3, r3, #0x14
00448270: add      r2, sp, #0x8d0
00448274: strd     r0, r1, [r2, r3]
00448278: ldr      r3, [sp, #0x360]
0044827c: add      r6, sp, #0x190
00448280: sub      r6, r6, #4
00448284: str      r3, [sp, #0x190]
00448288: ldr      r3, [sp, #0x364]
0044828c: mov      r1, r7
00448290: mov      r2, r6
00448294: str      r3, [sp, #0x194]
00448298: mov      r0, r4
0044829c: blx      r8
004482a0: mov      r0, r6
004482a4: bl       #0x797124
004482a8: ldrb     r3, [sp, #0x5c0]
004482ac: cmp      r3, #0xff
004482b0: beq      #0x449580
004482b4: ldr      r1, [pc, #0x954]
004482b8: ldr      r3, [r4]
004482bc: add      r7, sp, #0x5a0
004482c0: add      r7, r7, #0xc
004482c4: add      r1, pc, r1
004482c8: mov      r0, r7
004482cc: ldr      r8, [r3, #0x1c]
004482d0: bl       #0x413a7c
004482d4: mov      r2, #0
004482d8: mov      r1, #0x69
004482dc: mov      r0, r5
004482e0: bl       #0x3df6e0
004482e4: mov      r3, #0
004482e8: strb     r3, [sp, #0x180]
004482ec: mov      r3, #2
004482f0: strb     r3, [sp, #0x181]
004482f4: bl       #0x30ed30
004482f8: mov      r3, #0xa9000000
004482fc: asr      r3, r3, #0x14
00448300: add      r2, sp, #0x8d0
00448304: strd     r0, r1, [r2, r3]
00448308: ldr      r3, [sp, #0x360]
0044830c: add      r6, sp, #0x180
00448310: mov      r1, r7
00448314: str      r3, [sp, #0x184]
00448318: ldr      r3, [sp, #0x364]
0044831c: mov      r2, r6
00448320: mov      r0, r4
00448324: str      r3, [r6, #8]
00448328: blx      r8
0044832c: mov      r0, r6
00448330: bl       #0x797124
00448334: ldrb     r3, [sp, #0x5ac]
00448338: cmp      r3, #0xff
0044833c: beq      #0x449570
00448340: ldr      r1, [pc, #0x8cc]
00448344: ldr      r3, [r4]
00448348: add      r8, sp, #0x590
0044834c: add      r8, r8, #8
00448350: add      r1, pc, r1
00448354: mov      r0, r8
00448358: ldr      sl, [r3, #0x1c]
0044835c: bl       #0x413a7c
00448360: mov      r2, #0
00448364: mov      r1, #0x6a
00448368: mov      r0, r5
0044836c: bl       #0x3df6e0
00448370: mov      r3, #0
00448374: strb     r3, [sp, #0x174]
00448378: mov      r3, #2
0044837c: strb     r3, [sp, #0x175]
00448380: bl       #0x30ed30
00448384: mov      r3, #0xa9000000
00448388: asr      r3, r3, #0x14
0044838c: add      r2, sp, #0x8d0
00448390: strd     r0, r1, [r2, r3]
00448394: ldr      r3, [sp, #0x360]
00448398: sub      r7, r6, #0xc
0044839c: mov      r1, r8
004483a0: str      r3, [sp, #0x178]
004483a4: ldr      r3, [sp, #0x364]
004483a8: mov      r2, r7
004483ac: mov      r0, r4
004483b0: str      r3, [r6, #-4]
004483b4: blx      sl
004483b8: mov      r0, r7
004483bc: bl       #0x797124
004483c0: ldrb     r3, [sp, #0x598]
004483c4: cmp      r3, #0xff
004483c8: beq      #0x449560
004483cc: ldr      r1, [pc, #0x844]
004483d0: ldr      r3, [r4]
004483d4: add      r7, sp, #0x580
004483d8: add      r7, r7, #4
004483dc: add      r1, pc, r1
004483e0: mov      r0, r7
004483e4: ldr      r8, [r3, #0x1c]
004483e8: bl       #0x413a7c
004483ec: mov      r2, #0
004483f0: mov      r1, #0x6d
004483f4: mov      r0, r5
004483f8: bl       #0x3df6e0
004483fc: mov      r3, #0
00448400: strb     r3, [sp, #0x168]
00448404: mov      r3, #2
00448408: strb     r3, [sp, #0x169]
0044840c: bl       #0x30ed30
00448410: mov      r3, #0xa9000000
00448414: asr      r3, r3, #0x14
00448418: add      r2, sp, #0x8d0
0044841c: strd     r0, r1, [r2, r3]
00448420: ldr      r2, [sp, #0x364]
00448424: add      r3, sp, #0x170
00448428: sub      r6, r3, #8
0044842c: str      r2, [r3]
00448430: ldr      r3, [sp, #0x360]
00448434: mov      r1, r7
00448438: mov      r2, r6
0044843c: str      r3, [sp, #0x16c]
00448440: mov      r0, r4
00448444: blx      r8
00448448: mov      r0, r6
0044844c: bl       #0x797124
00448450: ldrb     r3, [sp, #0x584]
00448454: cmp      r3, #0xff
00448458: beq      #0x449550
0044845c: ldr      r1, [pc, #0x7b8]
00448460: ldr      r3, [r4]
00448464: add      r7, sp, #0x570
00448468: add      r1, pc, r1
0044846c: mov      r0, r7
00448470: ldr      r8, [r3, #0x1c]
00448474: bl       #0x413a7c
00448478: mov      r2, #0
0044847c: mov      r1, #0x6e
00448480: mov      r0, r5
00448484: bl       #0x3df6e0
00448488: mov      r3, #0
0044848c: strb     r3, [sp, #0x15c]
00448490: mov      r3, #2
00448494: strb     r3, [sp, #0x15d]
00448498: bl       #0x30ed30
0044849c: mov      r3, #0xa9000000
004484a0: asr      r3, r3, #0x14
004484a4: add      r2, sp, #0x8d0
004484a8: strd     r0, r1, [r2, r3]
004484ac: ldr      r3, [sp, #0x360]
004484b0: add      r6, sp, #0x160
004484b4: sub      r6, r6, #4
004484b8: str      r3, [sp, #0x160]
004484bc: ldr      r3, [sp, #0x364]
004484c0: mov      r1, r7
004484c4: mov      r2, r6
004484c8: str      r3, [sp, #0x164]
004484cc: mov      r0, r4
004484d0: blx      r8
004484d4: mov      r0, r6
004484d8: bl       #0x797124
004484dc: ldrb     r3, [sp, #0x570]
004484e0: cmp      r3, #0xff
004484e4: beq      #0x449540
004484e8: ldr      r1, [pc, #0x730]
004484ec: ldr      r3, [r4]
004484f0: add      r7, sp, #0x550
004484f4: add      r7, r7, #0xc
004484f8: add      r1, pc, r1
004484fc: mov      r0, r7
00448500: ldr      r8, [r3, #0x1c]
00448504: bl       #0x413a7c
00448508: mov      r2, #0
0044850c: mov      r1, #0x75
00448510: mov      r0, r5
00448514: bl       #0x3df6e0
00448518: mov      r3, #0
0044851c: strb     r3, [sp, #0x150]
00448520: mov      r3, #2
00448524: strb     r3, [sp, #0x151]
00448528: bl       #0x30ed30
0044852c: mov      r3, #0xa9000000
00448530: asr      r3, r3, #0x14
00448534: add      r2, sp, #0x8d0
00448538: strd     r0, r1, [r2, r3]
0044853c: ldr      r3, [sp, #0x360]
00448540: add      r6, sp, #0x150
00448544: mov      r1, r7
00448548: str      r3, [sp, #0x154]
0044854c: ldr      r3, [sp, #0x364]
00448550: mov      r2, r6
00448554: mov      r0, r4
00448558: str      r3, [r6, #8]
0044855c: blx      r8
00448560: mov      r0, r6
00448564: bl       #0x797124
00448568: ldrb     r3, [sp, #0x55c]
0044856c: cmp      r3, #0xff
00448570: beq      #0x449530
00448574: ldr      r1, [pc, #0x6a8]
00448578: ldr      r3, [r4]
0044857c: add      r8, sp, #0x540
00448580: add      r8, r8, #8
00448584: add      r1, pc, r1
00448588: mov      r0, r8
0044858c: ldr      sl, [r3, #0x1c]
00448590: bl       #0x413a7c
00448594: mov      r2, #0
00448598: mov      r1, #0x76
0044859c: mov      r0, r5
004485a0: bl       #0x3df6e0
004485a4: mov      r3, #0
004485a8: strb     r3, [sp, #0x144]
004485ac: mov      r3, #2
004485b0: strb     r3, [sp, #0x145]
004485b4: bl       #0x30ed30
004485b8: mov      r3, #0xa9000000
004485bc: asr      r3, r3, #0x14
004485c0: add      r2, sp, #0x8d0
004485c4: strd     r0, r1, [r2, r3]
004485c8: ldr      r3, [sp, #0x360]
004485cc: sub      r7, r6, #0xc
004485d0: mov      r1, r8
004485d4: str      r3, [sp, #0x148]
004485d8: ldr      r3, [sp, #0x364]
004485dc: mov      r2, r7
004485e0: mov      r0, r4
004485e4: str      r3, [r6, #-4]
004485e8: blx      sl
004485ec: mov      r0, r7
004485f0: bl       #0x797124
004485f4: ldrb     r3, [sp, #0x548]
004485f8: cmp      r3, #0xff
004485fc: beq      #0x449520
00448600: ldr      r1, [pc, #0x620]
00448604: ldr      r3, [r4]
00448608: add      r7, sp, #0x530
0044860c: add      r7, r7, #4
00448610: add      r1, pc, r1
00448614: mov      r0, r7
00448618: ldr      r8, [r3, #0x1c]
0044861c: bl       #0x413a7c
00448620: mov      r2, #0
00448624: mov      r1, #0x71
00448628: mov      r0, r5
0044862c: bl       #0x3df6e0
00448630: mov      r3, #0
00448634: strb     r3, [sp, #0x138]
00448638: mov      r3, #2
0044863c: strb     r3, [sp, #0x139]
00448640: bl       #0x30ed30
00448644: mov      r3, #0xa9000000
00448648: asr      r3, r3, #0x14
0044864c: add      r2, sp, #0x8d0
00448650: strd     r0, r1, [r2, r3]
00448654: ldr      r2, [sp, #0x364]
00448658: add      r3, sp, #0x140
0044865c: sub      r6, r3, #8
00448660: str      r2, [r3]
00448664: ldr      r3, [sp, #0x360]
00448668: mov      r1, r7
0044866c: mov      r2, r6
00448670: str      r3, [sp, #0x13c]
00448674: mov      r0, r4
00448678: blx      r8
0044867c: mov      r0, r6
00448680: bl       #0x797124
00448684: ldrb     r3, [sp, #0x534]
00448688: cmp      r3, #0xff
0044868c: beq      #0x449510
00448690: ldr      r1, [pc, #0x594]
00448694: ldr      r3, [r4]
00448698: add      r7, sp, #0x520
0044869c: add      r1, pc, r1
004486a0: mov      r0, r7
004486a4: ldr      r8, [r3, #0x1c]
004486a8: bl       #0x413a7c
004486ac: mov      r2, #0
004486b0: mov      r1, #0x72
004486b4: mov      r0, r5
004486b8: bl       #0x3df6e0
004486bc: mov      r3, #0
004486c0: strb     r3, [sp, #0x12c]
004486c4: mov      r3, #2
004486c8: strb     r3, [sp, #0x12d]
004486cc: bl       #0x30ed30
004486d0: mov      r3, #0xa9000000
004486d4: asr      r3, r3, #0x14
004486d8: add      r2, sp, #0x8d0
004486dc: strd     r0, r1, [r2, r3]
004486e0: ldr      r3, [sp, #0x360]
004486e4: add      r6, sp, #0x130
004486e8: sub      r6, r6, #4
004486ec: str      r3, [sp, #0x130]
004486f0: ldr      r3, [sp, #0x364]
004486f4: mov      r1, r7
004486f8: mov      r2, r6
004486fc: str      r3, [sp, #0x134]
00448700: mov      r0, r4
00448704: blx      r8
00448708: mov      r0, r6
0044870c: bl       #0x797124
00448710: ldrb     r3, [sp, #0x520]
00448714: cmp      r3, #0xff
00448718: beq      #0x449500
0044871c: ldr      r2, [sp, #4]
00448720: ldr      r1, [pc, #0x508]
00448724: ldr      r3, [r4]
00448728: add      sl, sp, #0x500
0044872c: add      sl, sl, #0xc
00448730: add      r6, r2, #0x37c
00448734: add      r1, pc, r1
00448738: mov      r0, sl
0044873c: ldr      r8, [r3, #0x1c]
00448740: bl       #0x413a7c
00448744: mov      r1, #0
00448748: mov      r0, r6
0044874c: bl       #0x4001a0
00448750: mov      r3, #0
00448754: add      r7, sp, #0x120
00448758: strb     r3, [sp, #0x120]
0044875c: mov      r3, #1
00448760: strb     r3, [sp, #0x121]
00448764: strb     r0, [sp, #0x124]
00448768: mov      r1, sl
0044876c: mov      r2, r7
00448770: mov      r0, r4
00448774: blx      r8
00448778: mov      r0, r7
0044877c: bl       #0x797124
00448780: ldrb     r3, [sp, #0x50c]
00448784: cmp      r3, #0xff
00448788: beq      #0x4494f0
0044878c: ldr      r1, [pc, #0x4a0]
00448790: ldr      r3, [r4]
00448794: add      sl, sp, #0x4f0
00448798: add      sl, sl, #8
0044879c: add      r1, pc, r1
004487a0: mov      r0, sl
004487a4: ldr      r8, [r3, #0x1c]
004487a8: bl       #0x413a7c
004487ac: mov      r0, r6
004487b0: bl       #0x400158
004487b4: mov      r3, #0
004487b8: sub      r7, r7, #0xc
004487bc: strb     r3, [sp, #0x114]
004487c0: mov      r3, #1
004487c4: strb     r3, [sp, #0x115]
004487c8: strb     r0, [sp, #0x118]
004487cc: mov      r1, sl
004487d0: mov      r2, r7
004487d4: mov      r0, r4
004487d8: blx      r8
004487dc: mov      r0, r7
004487e0: bl       #0x797124
004487e4: ldrb     r3, [sp, #0x4f8]
004487e8: cmp      r3, #0xff
004487ec: beq      #0x4494e0
004487f0: ldr      r1, [pc, #0x440]
004487f4: ldr      r3, [r4]
004487f8: add      sl, sp, #0x4e0
004487fc: add      sl, sl, #4
00448800: add      r1, pc, r1
00448804: mov      r0, sl
00448808: ldr      r8, [r3, #0x1c]
0044880c: bl       #0x413a7c
00448810: mov      r0, r6
00448814: bl       #0x4000c8
00448818: add      r7, sp, #0x110
0044881c: mov      r3, #0
00448820: sub      r7, r7, #8
00448824: strb     r3, [sp, #0x108]
00448828: mov      r3, #1
0044882c: strb     r3, [sp, #0x109]
00448830: strb     r0, [sp, #0x10c]
00448834: mov      r1, sl
00448838: mov      r2, r7
0044883c: mov      r0, r4
00448840: blx      r8
00448844: mov      r0, r7
00448848: bl       #0x797124
0044884c: ldrb     r3, [sp, #0x4e4]
00448850: cmp      r3, #0xff
00448854: beq      #0x4494d0
00448858: ldr      r1, [pc, #0x3dc]
0044885c: ldr      r3, [r4]
00448860: add      r8, sp, #0x4d0
00448864: add      r1, pc, r1
00448868: mov      r0, r8
0044886c: ldr      r7, [r3, #0x1c]
00448870: bl       #0x413a7c
00448874: mov      r0, r6
00448878: bl       #0x400080
0044887c: add      r6, sp, #0x8d0
00448880: strb     r0, [r6, #-0x7d0]!
00448884: mov      r3, #0
00448888: sub      r6, r6, #4
0044888c: strb     r3, [sp, #0xfc]
00448890: mov      r3, #1
00448894: strb     r3, [sp, #0xfd]
00448898: mov      r1, r8
0044889c: mov      r2, r6
004488a0: mov      r0, r4
004488a4: blx      r7
004488a8: mov      r0, r6
004488ac: bl       #0x797124
004488b0: ldrb     r3, [sp, #0x4d0]
004488b4: cmp      r3, #0xff
004488b8: beq      #0x4494c0
004488bc: ldr      r1, [pc, #0x37c]
004488c0: ldr      r3, [r4]
004488c4: add      r7, sp, #0x4c0
004488c8: sub      r7, r7, #4
004488cc: add      r1, pc, r1
004488d0: mov      r0, r7
004488d4: ldr      r8, [r3, #0x1c]
004488d8: bl       #0x413a7c
004488dc: mov      r2, #0
004488e0: mov      r1, #0xcf
004488e4: mov      r0, r5
004488e8: bl       #0x3df6e0
004488ec: mov      r3, #0
004488f0: strb     r3, [sp, #0xf0]
004488f4: mov      r3, #2
004488f8: strb     r3, [sp, #0xf1]
004488fc: bl       #0x30ed30
00448900: mov      r3, #0xa9000000
00448904: asr      r3, r3, #0x14
00448908: add      r2, sp, #0x8d0
0044890c: strd     r0, r1, [r2, r3]
00448910: ldr      r3, [sp, #0x360]
00448914: add      r6, sp, #0xf0
00448918: mov      r1, r7
0044891c: str      r3, [sp, #0xf4]
00448920: ldr      r3, [sp, #0x364]
00448924: mov      r2, r6
00448928: mov      r0, r4
0044892c: str      r3, [r6, #8]
00448930: blx      r8
00448934: mov      r0, r6
00448938: bl       #0x797124
0044893c: ldrb     r3, [sp, #0x4bc]
00448940: cmp      r3, #0xff
00448944: beq      #0x4494b0
00448948: ldr      r1, [pc, #0x2f4]
0044894c: ldr      r3, [r4]
00448950: add      r8, sp, #0x4b0
00448954: sub      r8, r8, #8
00448958: add      r1, pc, r1
0044895c: mov      r0, r8
00448960: ldr      sl, [r3, #0x1c]
00448964: bl       #0x413a7c
00448968: mov      r2, #0
0044896c: mov      r1, #0x47
00448970: mov      r0, r5
00448974: bl       #0x3df6e0
00448978: mov      r3, #0
0044897c: strb     r3, [sp, #0xe4]
00448980: mov      r3, #2
00448984: strb     r3, [sp, #0xe5]
00448988: bl       #0x30ed30
0044898c: mov      r3, #0xa9000000
00448990: asr      r3, r3, #0x14
00448994: add      r2, sp, #0x8d0
00448998: strd     r0, r1, [r2, r3]
0044899c: ldr      r3, [sp, #0x360]
004489a0: sub      r7, r6, #0xc
004489a4: mov      r1, r8
004489a8: str      r3, [sp, #0xe8]
004489ac: ldr      r3, [sp, #0x364]
004489b0: mov      r2, r7
004489b4: mov      r0, r4
004489b8: str      r3, [r6, #-4]
004489bc: blx      sl
004489c0: mov      r0, r7
004489c4: bl       #0x797124
004489c8: ldrb     r3, [sp, #0x4a8]
004489cc: cmp      r3, #0xff
004489d0: beq      #0x4494a0
004489d4: ldr      r1, [pc, #0x26c]
004489d8: ldr      r3, [r4]
004489dc: add      r7, sp, #0x4a0
004489e0: sub      r7, r7, #0xc
004489e4: add      r1, pc, r1
004489e8: mov      r0, r7
004489ec: ldr      r8, [r3, #0x1c]
004489f0: bl       #0x413a7c
004489f4: mov      r2, #0
004489f8: mov      r1, #0xa4
004489fc: mov      r0, r5
00448a00: bl       #0x3df6e0
00448a04: mov      r3, #0
00448a08: strb     r3, [sp, #0xd8]
00448a0c: mov      r3, #2
00448a10: strb     r3, [sp, #0xd9]
00448a14: bl       #0x30ed30
00448a18: mov      r3, #0xa9000000
00448a1c: asr      r3, r3, #0x14
00448a20: add      r2, sp, #0x8d0
00448a24: strd     r0, r1, [r2, r3]
00448a28: ldr      r2, [sp, #0x364]
00448a2c: add      r3, sp, #0xe0
00448a30: sub      r6, r3, #8
00448a34: str      r2, [r3]
00448a38: ldr      r3, [sp, #0x360]
00448a3c: mov      r1, r7
00448a40: mov      r2, r6
00448a44: str      r3, [sp, #0xdc]
00448a48: mov      r0, r4
00448a4c: blx      r8
00448a50: mov      r0, r6
00448a54: bl       #0x797124
00448a58: ldrb     r3, [sp, #0x494]
00448a5c: cmp      r3, #0xff
00448a60: beq      #0x449490
00448a64: ldr      r1, [pc, #0x1e0]
00448a68: ldr      r3, [r4]
00448a6c: add      r7, sp, #0x480
00448a70: add      r1, pc, r1
00448a74: mov      r0, r7
00448a78: ldr      r8, [r3, #0x1c]
00448a7c: bl       #0x413a7c
00448a80: mov      r2, #0
00448a84: mov      r1, #0xd1
00448a88: mov      r0, r5
00448a8c: bl       #0x3df6e0
00448a90: mov      r3, #0
00448a94: strb     r3, [sp, #0xcc]
00448a98: mov      r3, #2
00448a9c: strb     r3, [sp, #0xcd]
00448aa0: bl       #0x30ed30
00448aa4: mov      r3, #0xa9000000
00448aa8: asr      r3, r3, #0x14
00448aac: add      r2, sp, #0x8d0
00448ab0: strd     r0, r1, [r2, r3]
00448ab4: ldr      r3, [sp, #0x360]
00448ab8: add      r6, sp, #0xd0
00448abc: sub      r6, r6, #4
00448ac0: str      r3, [sp, #0xd0]
00448ac4: ldr      r3, [sp, #0x364]
00448ac8: mov      r1, r7
00448acc: mov      r2, r6
00448ad0: str      r3, [sp, #0xd4]
00448ad4: mov      r0, r4
00448ad8: blx      r8
00448adc: mov      r0, r6
00448ae0: bl       #0x797124
00448ae4: ldrb     r3, [sp, #0x480]
00448ae8: cmp      r3, #0xff
00448aec: beq      #0x449480
00448af0: ldr      r1, [pc, #0x158]
00448af4: ldr      r3, [r4]
00448af8: add      r7, sp, #0x470
00448afc: sub      r7, r7, #4
00448b00: add      r1, pc, r1
00448b04: mov      r0, r7
00448b08: ldr      r8, [r3, #0x1c]
00448b0c: bl       #0x413a7c
00448b10: mov      r2, #0
00448b14: mov      r1, #0xa5
00448b18: mov      r0, r5
00448b1c: bl       #0x3df6e0
00448b20: mov      r3, #0
00448b24: strb     r3, [sp, #0xc0]
00448b28: mov      r3, #2
00448b2c: strb     r3, [sp, #0xc1]
00448b30: bl       #0x30ed30
00448b34: mov      r3, #0xa9000000
00448b38: asr      r3, r3, #0x14
00448b3c: add      r2, sp, #0x8d0
00448b40: strd     r0, r1, [r2, r3]
00448b44: ldr      r3, [sp, #0x360]
00448b48: add      r6, sp, #0xc0
00448b4c: mov      r1, r7
00448b50: str      r3, [sp, #0xc4]
00448b54: ldr      r3, [sp, #0x364]
00448b58: mov      r2, r6
00448b5c: mov      r0, r4
00448b60: str      r3, [r6, #8]
00448b64: blx      r8
00448b68: mov      r0, r6
00448b6c: bl       #0x797124
00448b70: ldrb     r3, [sp, #0x46c]
00448b74: cmp      r3, #0xff
00448b78: beq      #0x449470
00448b7c: ldr      r1, [pc, #0xd0]
00448b80: ldr      r3, [r4]
00448b84: add      r8, sp, #0x460
00448b88: sub      r8, r8, #8
00448b8c: add      r1, pc, r1
00448b90: mov      r0, r8
00448b94: ldr      sl, [r3, #0x1c]
00448b98: bl       #0x413a7c
00448b9c: mov      r2, #0
00448ba0: mov      r1, #0xd0
00448ba4: mov      r0, r5
00448ba8: bl       #0x3df6e0
00448bac: mov      r3, #0
00448bb0: strb     r3, [sp, #0xb4]
00448bb4: mov      r3, #2
00448bb8: strb     r3, [sp, #0xb5]
00448bbc: bl       #0x30ed30
00448bc0: mov      r3, #0xa9000000
00448bc4: asr      r3, r3, #0x14
00448bc8: add      r2, sp, #0x8d0
00448bcc: strd     r0, r1, [r2, r3]
00448bd0: ldr      r3, [sp, #0x360]
00448bd4: sub      r7, r6, #0xc
00448bd8: mov      r1, r8
00448bdc: b        #0x448c88
00448be0: subeq    r4, r8, r0, ror #12
00448be4: strdeq   r4, r5, [r8], #-0x58
00448be8: umaaleq  r4, r8, r0, r5
00448bec: subeq    r4, r8, r4, lsr #10
00448bf0: strheq   r4, [r8], #-0x44
00448bf4: subeq    r4, r8, r8, asr #8
00448bf8: subeq    r4, r8, r0, ror #7
00448bfc: subeq    r4, r8, r8, ror r3
00448c00: subeq    r4, r8, r8, lsl #6
00448c04: umaaleq  r4, r8, ip, r2
00448c08: subeq    r4, r8, r0, lsr r2
00448c0c: subeq    r4, r8, r4, asr #3
00448c10: subeq    r4, r8, r4, asr r1
00448c14: subeq    r4, r8, r8, ror #1
00448c18: subeq    r4, r8, ip, ror r0
00448c1c: subeq    r4, r8, r0, lsl r0
00448c20: subeq    r3, r8, r0, lsr #31
00448c24: subeq    r3, r8, r4, lsr pc
00448c28: subeq    r3, r8, r8, asr #29
00448c2c: subeq    r3, r8, ip, asr lr
00448c30: subeq    r3, r8, r4, ror #27
00448c34: umaaleq  r3, r8, r4, sp
00448c38: subeq    r3, r8, r8, asr #26
00448c3c: subeq    fp, r7, ip, lsr #24
00448c40: subeq    r3, r8, ip, lsl #25
00448c44: subeq    r3, r8, r0, lsr #24
00448c48: subeq    r3, r8, r4, lsr #23
00448c4c: subeq    r3, r8, r0, lsr fp
00448c50: subeq    r3, r8, r0, asr #21
00448c54: subeq    r3, r8, ip, asr #20
00448c58: subeq    r3, r8, r4, lsr sb
00448c5c: subeq    r3, r8, r0, asr #17
00448c60: subeq    r3, r8, r0, asr r8
00448c64: subeq    r3, r8, r4, ror #15
00448c68: subeq    r3, r8, r0, ror r7
00448c6c: subeq    r3, r8, r4, lsl #14
00448c70: subeq    r3, r8, r4, lsl #13
00448c74: subeq    r3, r8, r8, lsl #12
00448c78: subeq    r3, r8, ip, lsl #11
00448c7c: subeq    r3, r8, r0, lsl r5
00448c80: subeq    r3, r8, r0, lsr #9
00448c84: subeq    r3, r8, r4, lsr r4
00448c88: str      r3, [sp, #0xb8]
00448c8c: ldr      r3, [sp, #0x364]
00448c90: mov      r2, r7
00448c94: mov      r0, r4
00448c98: str      r3, [r6, #-4]
00448c9c: blx      sl
00448ca0: mov      r0, r7
00448ca4: bl       #0x797124
00448ca8: ldrb     r3, [sp, #0x458]
00448cac: cmp      r3, #0xff
00448cb0: beq      #0x449460
00448cb4: ldr      r1, [pc, #-0x64]
00448cb8: ldr      r3, [r4]
00448cbc: add      r7, sp, #0x450
00448cc0: sub      r7, r7, #0xc
00448cc4: add      r1, pc, r1
00448cc8: mov      r0, r7
00448ccc: ldr      r8, [r3, #0x1c]
00448cd0: bl       #0x413a7c
00448cd4: mov      r2, #0
00448cd8: mov      r1, #0xa6
00448cdc: mov      r0, r5
00448ce0: bl       #0x3df6e0
00448ce4: mov      r3, #0
00448ce8: strb     r3, [sp, #0xa8]
00448cec: mov      r3, #2
00448cf0: strb     r3, [sp, #0xa9]
00448cf4: bl       #0x30ed30
00448cf8: mov      r3, #0xa9000000
00448cfc: asr      r3, r3, #0x14
00448d00: add      r2, sp, #0x8d0
00448d04: strd     r0, r1, [r2, r3]
00448d08: ldr      r2, [sp, #0x364]
00448d0c: add      r3, sp, #0xb0
00448d10: sub      r6, r3, #8
00448d14: str      r2, [r3]
00448d18: ldr      r3, [sp, #0x360]
00448d1c: mov      r1, r7
00448d20: mov      r2, r6
00448d24: str      r3, [sp, #0xac]
00448d28: mov      r0, r4
00448d2c: blx      r8
00448d30: mov      r0, r6
00448d34: bl       #0x797124
00448d38: ldrb     r3, [sp, #0x444]
00448d3c: cmp      r3, #0xff
00448d40: beq      #0x449450
00448d44: ldr      r1, [pc, #-0xf0]
00448d48: ldr      r3, [r4]
00448d4c: add      r7, sp, #0x430
00448d50: add      r1, pc, r1
00448d54: mov      r0, r7
00448d58: ldr      r8, [r3, #0x1c]
00448d5c: bl       #0x413a7c
00448d60: mov      r2, #0
00448d64: mov      r1, #0xa9
00448d68: mov      r0, r5
00448d6c: bl       #0x3df6e0
00448d70: mov      r3, #0
00448d74: strb     r3, [sp, #0x9c]
00448d78: mov      r3, #2
00448d7c: strb     r3, [sp, #0x9d]
00448d80: bl       #0x30ed30
00448d84: mov      r3, #0xa9000000
00448d88: asr      r3, r3, #0x14
00448d8c: add      r2, sp, #0x8d0
00448d90: strd     r0, r1, [r2, r3]
00448d94: ldr      r3, [sp, #0x360]
00448d98: add      r6, sp, #0xa0
00448d9c: sub      r6, r6, #4
00448da0: str      r3, [sp, #0xa0]
00448da4: ldr      r3, [sp, #0x364]
00448da8: mov      r1, r7
00448dac: mov      r2, r6
00448db0: str      r3, [sp, #0xa4]
00448db4: mov      r0, r4
00448db8: blx      r8
00448dbc: mov      r0, r6
00448dc0: bl       #0x797124
00448dc4: ldrb     r3, [sp, #0x430]
00448dc8: cmp      r3, #0xff
00448dcc: beq      #0x449440
00448dd0: ldr      r1, [pc, #-0x178]
00448dd4: ldr      r3, [r4]
00448dd8: add      r7, sp, #0x420
00448ddc: sub      r7, r7, #4
00448de0: add      r1, pc, r1
00448de4: mov      r0, r7
00448de8: ldr      r8, [r3, #0x1c]
00448dec: bl       #0x413a7c
00448df0: mov      r2, #0
00448df4: mov      r1, #0xa7
00448df8: mov      r0, r5
00448dfc: bl       #0x3df6e0
00448e00: mov      r3, #0
00448e04: strb     r3, [sp, #0x90]
00448e08: mov      r3, #2
00448e0c: strb     r3, [sp, #0x91]
00448e10: bl       #0x30ed30
00448e14: mov      r3, #0xa9000000
00448e18: asr      r3, r3, #0x14
00448e1c: add      r2, sp, #0x8d0
00448e20: strd     r0, r1, [r2, r3]
00448e24: ldr      r3, [sp, #0x360]
00448e28: add      r6, sp, #0x90
00448e2c: mov      r1, r7
00448e30: str      r3, [sp, #0x94]
00448e34: ldr      r3, [sp, #0x364]
00448e38: mov      r2, r6
00448e3c: mov      r0, r4
00448e40: str      r3, [r6, #8]
00448e44: blx      r8
00448e48: mov      r0, r6
00448e4c: bl       #0x797124
00448e50: ldrb     r3, [sp, #0x41c]
00448e54: cmp      r3, #0xff
00448e58: beq      #0x449430
00448e5c: ldr      r1, [pc, #-0x200]
00448e60: ldr      r3, [r4]
00448e64: add      r8, sp, #0x410
00448e68: sub      r8, r8, #8
00448e6c: add      r1, pc, r1
00448e70: mov      r0, r8
00448e74: ldr      sl, [r3, #0x1c]
00448e78: bl       #0x413a7c
00448e7c: mov      r2, #0
00448e80: mov      r1, #0xaa
00448e84: mov      r0, r5
00448e88: bl       #0x3df6e0
00448e8c: mov      r3, #0
00448e90: strb     r3, [sp, #0x84]
00448e94: mov      r3, #2
00448e98: strb     r3, [sp, #0x85]
00448e9c: bl       #0x30ed30
00448ea0: mov      r3, #0xa9000000
00448ea4: asr      r3, r3, #0x14
00448ea8: add      r2, sp, #0x8d0
00448eac: strd     r0, r1, [r2, r3]
00448eb0: ldr      r3, [sp, #0x360]
00448eb4: sub      r7, r6, #0xc
00448eb8: mov      r1, r8
00448ebc: str      r3, [sp, #0x88]
00448ec0: ldr      r3, [sp, #0x364]
00448ec4: mov      r2, r7
00448ec8: mov      r0, r4
00448ecc: str      r3, [r6, #-4]
00448ed0: blx      sl
00448ed4: mov      r0, r7
00448ed8: bl       #0x797124
00448edc: ldrb     r3, [sp, #0x408]
00448ee0: cmp      r3, #0xff
00448ee4: beq      #0x449420
00448ee8: ldr      r1, [pc, #-0x288]
00448eec: ldr      r3, [r4]
00448ef0: add      r7, sp, #0x400
00448ef4: sub      r7, r7, #0xc
00448ef8: add      r1, pc, r1
00448efc: mov      r0, r7
00448f00: ldr      r8, [r3, #0x1c]
00448f04: bl       #0x413a7c
00448f08: mov      r2, #0
00448f0c: mov      r1, #0xa8
00448f10: mov      r0, r5
00448f14: bl       #0x3df6e0
00448f18: mov      r3, #0
00448f1c: strb     r3, [sp, #0x78]
00448f20: mov      r3, #2
00448f24: strb     r3, [sp, #0x79]
00448f28: bl       #0x30ed30
00448f2c: mov      r3, #0xa9000000
00448f30: asr      r3, r3, #0x14
00448f34: add      r2, sp, #0x8d0
00448f38: strd     r0, r1, [r2, r3]
00448f3c: ldr      r2, [sp, #0x364]
00448f40: add      r3, sp, #0x80
00448f44: sub      r6, r3, #8
00448f48: str      r2, [r3]
00448f4c: ldr      r3, [sp, #0x360]
00448f50: mov      r1, r7
00448f54: mov      r2, r6
00448f58: str      r3, [sp, #0x7c]
00448f5c: mov      r0, r4
00448f60: blx      r8
00448f64: mov      r0, r6
00448f68: bl       #0x797124
00448f6c: ldrb     r3, [sp, #0x3f4]
00448f70: cmp      r3, #0xff
00448f74: beq      #0x449410
00448f78: ldr      r1, [pc, #-0x314]
00448f7c: ldr      r3, [r4]
00448f80: add      r7, sp, #0x3e0
00448f84: add      r1, pc, r1
00448f88: mov      r0, r7
00448f8c: ldr      r8, [r3, #0x1c]
00448f90: bl       #0x413a7c
00448f94: mov      r2, #0
00448f98: mov      r1, #0x27
00448f9c: mov      r0, r5
00448fa0: bl       #0x3df6e0
00448fa4: mov      r3, #0
00448fa8: strb     r3, [sp, #0x6c]
00448fac: mov      r3, #2
00448fb0: strb     r3, [sp, #0x6d]
00448fb4: bl       #0x30ed30
00448fb8: mov      r3, #0xa9000000
00448fbc: asr      r3, r3, #0x14
00448fc0: add      r2, sp, #0x8d0
00448fc4: strd     r0, r1, [r2, r3]
00448fc8: ldr      r3, [sp, #0x360]
00448fcc: add      r6, sp, #0x70
00448fd0: sub      r6, r6, #4
00448fd4: str      r3, [sp, #0x70]
00448fd8: ldr      r3, [sp, #0x364]
00448fdc: mov      r1, r7
00448fe0: mov      r2, r6
00448fe4: str      r3, [sp, #0x74]
00448fe8: mov      r0, r4
00448fec: blx      r8
00448ff0: mov      r0, r6
00448ff4: bl       #0x797124
00448ff8: ldrb     r3, [sp, #0x3e0]
00448ffc: cmp      r3, #0xff
00449000: beq      #0x449400
00449004: ldr      r1, [pc, #-0x39c]
00449008: ldr      r3, [r4]
0044900c: add      r7, sp, #0x3d0
00449010: sub      r7, r7, #4
00449014: add      r1, pc, r1
00449018: mov      r0, r7
0044901c: ldr      r8, [r3, #0x1c]
00449020: bl       #0x413a7c
00449024: mov      r2, #0
00449028: mov      r1, #0x2c
0044902c: mov      r0, r5
00449030: bl       #0x3df6e0
00449034: mov      r3, #0
00449038: strb     r3, [sp, #0x60]
0044903c: mov      r3, #2
00449040: strb     r3, [sp, #0x61]
00449044: bl       #0x30ed30
00449048: mov      r3, #0xa9000000
0044904c: asr      r3, r3, #0x14
00449050: add      r2, sp, #0x8d0
00449054: strd     r0, r1, [r2, r3]
00449058: ldr      r3, [sp, #0x360]
0044905c: add      r6, sp, #0x60
00449060: mov      r1, r7
00449064: str      r3, [sp, #0x64]
00449068: ldr      r3, [sp, #0x364]
0044906c: mov      r2, r6
00449070: mov      r0, r4
00449074: str      r3, [r6, #8]
00449078: blx      r8
0044907c: mov      r0, r6
00449080: bl       #0x797124
00449084: ldrb     r3, [sp, #0x3cc]
00449088: cmp      r3, #0xff
0044908c: beq      #0x4493f0
00449090: ldr      r1, [pc, #-0x424]
00449094: ldr      r3, [r4]
00449098: add      r8, sp, #0x3c0
0044909c: sub      r8, r8, #8
004490a0: add      r1, pc, r1
004490a4: mov      r0, r8
004490a8: ldr      sl, [r3, #0x1c]
004490ac: bl       #0x413a7c
004490b0: mov      r2, #0
004490b4: mov      r1, #0x84
004490b8: mov      r0, r5
004490bc: bl       #0x3df6e0
004490c0: mov      r3, #0
004490c4: strb     r3, [sp, #0x54]
004490c8: mov      r3, #2
004490cc: strb     r3, [sp, #0x55]
004490d0: bl       #0x30ed30
004490d4: mov      r3, #0xa9000000
004490d8: asr      r3, r3, #0x14
004490dc: add      r2, sp, #0x8d0
004490e0: strd     r0, r1, [r2, r3]
004490e4: ldr      r3, [sp, #0x360]
004490e8: sub      r7, r6, #0xc
004490ec: mov      r1, r8
004490f0: str      r3, [sp, #0x58]
004490f4: ldr      r3, [sp, #0x364]
004490f8: mov      r2, r7
004490fc: mov      r0, r4
00449100: str      r3, [r6, #-4]
00449104: blx      sl
00449108: mov      r0, r7
0044910c: bl       #0x797124
00449110: ldrb     r3, [sp, #0x3b8]
00449114: cmp      r3, #0xff
00449118: beq      #0x4493e0
0044911c: ldr      r1, [pc, #-0x4ac]
00449120: ldr      r3, [r4]
00449124: add      r7, sp, #0x3b0
00449128: sub      r7, r7, #0xc
0044912c: add      r1, pc, r1
00449130: mov      r0, r7
00449134: ldr      r8, [r3, #0x1c]
00449138: bl       #0x413a7c
0044913c: mov      r2, #0
00449140: mov      r1, #0x85
00449144: mov      r0, r5
00449148: bl       #0x3df6e0
0044914c: mov      r3, #0
00449150: strb     r3, [sp, #0x48]
00449154: mov      r3, #2
00449158: strb     r3, [sp, #0x49]
0044915c: bl       #0x30ed30
00449160: mov      r3, #0xa9000000
00449164: asr      r3, r3, #0x14
00449168: add      r2, sp, #0x8d0
0044916c: strd     r0, r1, [r2, r3]
00449170: ldr      r2, [sp, #0x364]
00449174: add      r3, sp, #0x50
00449178: sub      r6, r3, #8
0044917c: str      r2, [r3]
00449180: ldr      r3, [sp, #0x360]
00449184: mov      r1, r7
00449188: mov      r2, r6
0044918c: str      r3, [sp, #0x4c]
00449190: mov      r0, r4
00449194: blx      r8
00449198: mov      r0, r6
0044919c: bl       #0x797124
004491a0: ldrb     r3, [sp, #0x3a4]
004491a4: cmp      r3, #0xff
004491a8: beq      #0x4493d0
004491ac: ldr      r1, [pc, #-0x538]
004491b0: ldr      r3, [r4]
004491b4: add      r7, sp, #0x390
004491b8: add      r1, pc, r1
004491bc: mov      r0, r7
004491c0: ldr      r8, [r3, #0x1c]
004491c4: bl       #0x413a7c
004491c8: mov      r2, #0
004491cc: mov      r1, #0xc3
004491d0: mov      r0, r5
004491d4: bl       #0x3df6e0
004491d8: mov      r3, #0
004491dc: strb     r3, [sp, #0x3c]
004491e0: mov      r3, #2
004491e4: strb     r3, [sp, #0x3d]
004491e8: bl       #0x30ed30
004491ec: mov      r3, #0xa9000000
004491f0: asr      r3, r3, #0x14
004491f4: add      r2, sp, #0x8d0
004491f8: strd     r0, r1, [r2, r3]
004491fc: ldr      r3, [sp, #0x360]
00449200: add      r6, sp, #0x40
00449204: sub      r6, r6, #4
00449208: str      r3, [sp, #0x40]
0044920c: ldr      r3, [sp, #0x364]
00449210: mov      r1, r7
00449214: mov      r2, r6
00449218: str      r3, [sp, #0x44]
0044921c: mov      r0, r4
00449220: blx      r8
00449224: mov      r0, r6
00449228: bl       #0x797124
0044922c: ldrb     r3, [sp, #0x390]
00449230: cmp      r3, #0xff
00449234: beq      #0x4493c0
00449238: ldr      r1, [pc, #-0x5c0]
0044923c: ldr      r3, [r4]
00449240: add      r7, sp, #0x380
00449244: sub      r7, r7, #4
00449248: add      r1, pc, r1
0044924c: mov      r0, r7
00449250: ldr      r8, [r3, #0x1c]
00449254: bl       #0x413a7c
00449258: mov      r2, #0
0044925c: mov      r1, #0xc4
00449260: mov      r0, r5
00449264: bl       #0x3df6e0
00449268: mov      r3, #0
0044926c: strb     r3, [sp, #0x30]
00449270: mov      r3, #2
00449274: strb     r3, [sp, #0x31]
00449278: bl       #0x30ed30
0044927c: mov      r3, #0xa9000000
00449280: asr      r3, r3, #0x14
00449284: add      r2, sp, #0x8d0
00449288: strd     r0, r1, [r2, r3]
0044928c: ldr      r3, [sp, #0x360]
00449290: add      r6, sp, #0x30
00449294: mov      r1, r7
00449298: str      r3, [sp, #0x34]
0044929c: ldr      r3, [sp, #0x364]
004492a0: mov      r2, r6
004492a4: mov      r0, r4
004492a8: str      r3, [r6, #8]
004492ac: blx      r8
004492b0: mov      r0, r6
004492b4: bl       #0x797124
004492b8: ldrb     r3, [sp, #0x37c]
004492bc: cmp      r3, #0xff
004492c0: beq      #0x4493b0
004492c4: ldr      r1, [pc, #-0x648]
004492c8: ldr      r3, [r4]
004492cc: add      r7, sp, #0x370
004492d0: sub      r7, r7, #8
004492d4: add      r1, pc, r1
004492d8: mov      r0, r7
004492dc: ldr      r8, [r3, #0x1c]
004492e0: bl       #0x413a7c
004492e4: mov      r2, #0
004492e8: mov      r0, r5
004492ec: mov      r1, #0x8a
004492f0: bl       #0x3df6e0
004492f4: mov      r3, #0
004492f8: strb     r3, [sp, #0x24]
004492fc: mov      r3, #2
00449300: strb     r3, [sp, #0x25]
00449304: bl       #0x30ed30
00449308: mov      r3, #0xa9000000
0044930c: asr      r3, r3, #0x14
00449310: add      r2, sp, #0x8d0
00449314: strd     r0, r1, [r2, r3]
00449318: ldr      r3, [sp, #0x360]
0044931c: sub      r5, r6, #0xc
00449320: mov      r1, r7
00449324: str      r3, [sp, #0x28]
00449328: ldr      r3, [sp, #0x364]
0044932c: mov      r2, r5
00449330: mov      r0, r4
00449334: str      r3, [r6, #-4]
00449338: blx      r8
0044933c: mov      r0, r5
00449340: bl       #0x797124
00449344: ldrb     r3, [sp, #0x368]
00449348: cmp      r3, #0xff
0044934c: bne      #0x44935c
00449350: ldr      r0, [sp, #0x374]
00449354: ldr      r1, [sp, #0x370]
00449358: bl       #0x752b38
0044935c: mov      r1, r4
00449360: ldr      r0, [sb]
00449364: bl       #0x797250
00449368: ldr      r2, [sp]
0044936c: ldr      r1, [sp, #8]
00449370: ldr      r3, [r2, r1]
00449374: ldr      r2, [sp, #0x8cc]
00449378: ldr      r3, [r3]
0044937c: cmp      r2, r3
00449380: bne      #0x4497f0
00449384: add      sp, sp, #0xd4
00449388: add      sp, sp, #0x800
0044938c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00449390: mov      fp, #0x40000000
00449394: mov      sl, #0
00449398: add      fp, fp, #0x80000
0044939c: b        #0x446cb8
004493a0: mov      fp, #0x3fc00000
004493a4: mov      sl, #0
004493a8: add      fp, fp, #0x300000
004493ac: b        #0x446cb8
004493b0: ldr      r0, [sp, #0x388]
004493b4: ldr      r1, [sp, #0x384]
004493b8: bl       #0x752b38
004493bc: b        #0x4492c4
004493c0: ldr      r0, [sp, #0x39c]
004493c4: ldr      r1, [sp, #0x398]
004493c8: bl       #0x752b38
004493cc: b        #0x449238
004493d0: ldr      r0, [sp, #0x3b0]
004493d4: ldr      r1, [sp, #0x3ac]
004493d8: bl       #0x752b38
004493dc: b        #0x4491ac
004493e0: ldr      r0, [sp, #0x3c4]
004493e4: ldr      r1, [sp, #0x3c0]
004493e8: bl       #0x752b38
004493ec: b        #0x44911c
004493f0: ldr      r0, [sp, #0x3d8]
004493f4: ldr      r1, [sp, #0x3d4]
004493f8: bl       #0x752b38
004493fc: b        #0x449090
00449400: ldr      r0, [sp, #0x3ec]
00449404: ldr      r1, [sp, #0x3e8]
00449408: bl       #0x752b38
0044940c: b        #0x449004
00449410: ldr      r0, [sp, #0x400]
00449414: ldr      r1, [sp, #0x3fc]
00449418: bl       #0x752b38
0044941c: b        #0x448f78
00449420: ldr      r0, [sp, #0x414]
00449424: ldr      r1, [sp, #0x410]
00449428: bl       #0x752b38
0044942c: b        #0x448ee8
00449430: ldr      r0, [sp, #0x428]
00449434: ldr      r1, [sp, #0x424]
00449438: bl       #0x752b38
0044943c: b        #0x448e5c
00449440: ldr      r0, [sp, #0x43c]
00449444: ldr      r1, [sp, #0x438]
00449448: bl       #0x752b38
0044944c: b        #0x448dd0
00449450: ldr      r0, [sp, #0x450]
00449454: ldr      r1, [sp, #0x44c]
00449458: bl       #0x752b38
0044945c: b        #0x448d44
00449460: ldr      r0, [sp, #0x464]
00449464: ldr      r1, [sp, #0x460]
00449468: bl       #0x752b38
0044946c: b        #0x448cb4
00449470: ldr      r0, [sp, #0x478]
00449474: ldr      r1, [sp, #0x474]
00449478: bl       #0x752b38
0044947c: b        #0x448b7c
00449480: ldr      r0, [sp, #0x48c]
00449484: ldr      r1, [sp, #0x488]
00449488: bl       #0x752b38
0044948c: b        #0x448af0
00449490: ldr      r0, [sp, #0x4a0]
00449494: ldr      r1, [sp, #0x49c]
00449498: bl       #0x752b38
0044949c: b        #0x448a64
004494a0: ldr      r0, [sp, #0x4b4]
004494a4: ldr      r1, [sp, #0x4b0]
004494a8: bl       #0x752b38
004494ac: b        #0x4489d4
004494b0: ldr      r0, [sp, #0x4c8]
004494b4: ldr      r1, [sp, #0x4c4]
004494b8: bl       #0x752b38
004494bc: b        #0x448948
004494c0: ldr      r0, [sp, #0x4dc]
004494c4: ldr      r1, [sp, #0x4d8]
004494c8: bl       #0x752b38
004494cc: b        #0x4488bc
004494d0: ldr      r0, [sp, #0x4f0]
004494d4: ldr      r1, [sp, #0x4ec]
004494d8: bl       #0x752b38
004494dc: b        #0x448858
004494e0: ldr      r0, [sp, #0x504]
004494e4: ldr      r1, [sp, #0x500]
004494e8: bl       #0x752b38
004494ec: b        #0x4487f0
004494f0: ldr      r0, [sp, #0x518]
004494f4: ldr      r1, [sp, #0x514]
004494f8: bl       #0x752b38
004494fc: b        #0x44878c
00449500: ldr      r0, [sp, #0x52c]
00449504: ldr      r1, [sp, #0x528]
00449508: bl       #0x752b38
0044950c: b        #0x44871c
00449510: ldr      r0, [sp, #0x540]
00449514: ldr      r1, [sp, #0x53c]
00449518: bl       #0x752b38
0044951c: b        #0x448690
00449520: ldr      r0, [sp, #0x554]
00449524: ldr      r1, [sp, #0x550]
00449528: bl       #0x752b38
0044952c: b        #0x448600
00449530: ldr      r0, [sp, #0x568]
00449534: ldr      r1, [sp, #0x564]
00449538: bl       #0x752b38
0044953c: b        #0x448574
00449540: ldr      r0, [sp, #0x57c]
00449544: ldr      r1, [sp, #0x578]
00449548: bl       #0x752b38
0044954c: b        #0x4484e8
00449550: ldr      r0, [sp, #0x590]
00449554: ldr      r1, [sp, #0x58c]
00449558: bl       #0x752b38
0044955c: b        #0x44845c
00449560: ldr      r0, [sp, #0x5a4]
00449564: ldr      r1, [sp, #0x5a0]
00449568: bl       #0x752b38
0044956c: b        #0x4483cc
00449570: ldr      r0, [sp, #0x5b8]
00449574: ldr      r1, [sp, #0x5b4]
00449578: bl       #0x752b38
0044957c: b        #0x448340
00449580: ldr      r0, [sp, #0x5cc]
00449584: ldr      r1, [sp, #0x5c8]
00449588: bl       #0x752b38
0044958c: b        #0x4482b4
00449590: ldr      r0, [sp, #0x5e0]
00449594: ldr      r1, [sp, #0x5dc]
00449598: bl       #0x752b38
0044959c: b        #0x448228
004495a0: ldr      r0, [sp, #0x5f4]
004495a4: ldr      r1, [sp, #0x5f0]
004495a8: bl       #0x752b38
004495ac: b        #0x448198
004495b0: ldr      r0, [sp, #0x608]
004495b4: ldr      r1, [sp, #0x604]
004495b8: bl       #0x752b38
004495bc: b        #0x44810c
004495c0: ldr      r0, [sp, #0x61c]
004495c4: ldr      r1, [sp, #0x618]
004495c8: bl       #0x752b38
004495cc: b        #0x448080
004495d0: ldr      r0, [sp, #0x630]
004495d4: ldr      r1, [sp, #0x62c]
004495d8: bl       #0x752b38
004495dc: b        #0x447ff4
004495e0: ldr      r0, [sp, #0x644]
004495e4: ldr      r1, [sp, #0x640]
004495e8: bl       #0x752b38
004495ec: b        #0x447f70
004495f0: ldr      r0, [sp, #0x658]
004495f4: ldr      r1, [sp, #0x654]
004495f8: bl       #0x752b38
004495fc: b        #0x447ef0
00449600: ldr      r0, [sp, #0x66c]
00449604: ldr      r1, [sp, #0x668]
00449608: bl       #0x752b38
0044960c: b        #0x447e64
00449610: ldr      r0, [sp, #0x680]
00449614: ldr      r1, [sp, #0x67c]
00449618: bl       #0x752b38
0044961c: b        #0x447dd8
00449620: ldr      r0, [sp, #0x694]
00449624: ldr      r1, [sp, #0x690]
00449628: bl       #0x752b38
0044962c: b        #0x447d48
00449630: ldr      r0, [sp, #0x6a8]
00449634: ldr      r1, [sp, #0x6a4]
00449638: bl       #0x752b38
0044963c: b        #0x447cc8
00449640: ldr      r0, [sp, #0x6bc]
00449644: ldr      r1, [sp, #0x6b8]
00449648: bl       #0x752b38
0044964c: b        #0x447c48
00449650: ldr      r0, [sp, #0x6d0]
00449654: ldr      r1, [sp, #0x6cc]
00449658: bl       #0x752b38
0044965c: b        #0x447bbc
00449660: ldr      r0, [sp, #0x6e4]
00449664: ldr      r1, [sp, #0x6e0]
00449668: bl       #0x752b38
0044966c: b        #0x447b2c
00449670: ldr      r0, [sp, #0x6f8]
00449674: ldr      r1, [sp, #0x6f4]
00449678: bl       #0x752b38
0044967c: b        #0x447aa0
00449680: ldr      r0, [sp, #0x70c]
00449684: ldr      r1, [sp, #0x708]
00449688: bl       #0x752b38
0044968c: b        #0x447a14
00449690: ldr      r0, [sp, #0x720]
00449694: ldr      r1, [sp, #0x71c]
00449698: bl       #0x752b38
0044969c: b        #0x447988
004496a0: ldr      r0, [sp, #0x734]
004496a4: ldr      r1, [sp, #0x730]
004496a8: bl       #0x752b38
004496ac: b        #0x4478f8
004496b0: ldr      r0, [sp, #0x748]
004496b4: ldr      r1, [sp, #0x744]
004496b8: bl       #0x752b38
004496bc: b        #0x44786c
004496c0: ldr      r0, [sp, #0x75c]
004496c4: ldr      r1, [sp, #0x758]
004496c8: bl       #0x752b38
004496cc: b        #0x4477e0
004496d0: ldr      r0, [sp, #0x770]
004496d4: ldr      r1, [sp, #0x76c]
004496d8: bl       #0x752b38
004496dc: b        #0x447740
004496e0: ldr      r0, [sp, #0x784]
004496e4: ldr      r1, [sp, #0x780]
004496e8: bl       #0x752b38
004496ec: b        #0x4476bc
004496f0: ldr      r0, [sp, #0x798]
004496f4: ldr      r1, [sp, #0x794]
004496f8: bl       #0x752b38
004496fc: b        #0x447630
00449700: ldr      r0, [sp, #0x7ac]
00449704: ldr      r1, [sp, #0x7a8]
00449708: bl       #0x752b38
0044970c: b        #0x4475a4
00449710: ldr      r0, [sp, #0x7c0]
00449714: ldr      r1, [sp, #0x7bc]
00449718: bl       #0x752b38
0044971c: b        #0x447518
00449720: ldr      r0, [sp, #0x7d4]
00449724: ldr      r1, [sp, #0x7d0]
00449728: bl       #0x752b38
0044972c: b        #0x447488
00449730: ldr      r0, [sp, #0x7e8]
00449734: ldr      r1, [sp, #0x7e4]
00449738: bl       #0x752b38
0044973c: b        #0x4473fc
00449740: ldr      r0, [sp, #0x7fc]
00449744: ldr      r1, [sp, #0x7f8]
00449748: bl       #0x752b38
0044974c: b        #0x44736c
00449750: ldr      r0, [sp, #0x810]
00449754: ldr      r1, [sp, #0x80c]
00449758: bl       #0x752b38
0044975c: b        #0x4472dc
00449760: ldr      r0, [sp, #0x824]
00449764: ldr      r1, [sp, #0x820]
00449768: bl       #0x752b38
0044976c: b        #0x447248
00449770: ldr      r0, [sp, #0x838]
00449774: ldr      r1, [sp, #0x834]
00449778: bl       #0x752b38
0044977c: b        #0x4471b8
00449780: ldr      r0, [sp, #0x84c]
00449784: ldr      r1, [sp, #0x848]
00449788: bl       #0x752b38
0044978c: b        #0x447128
00449790: ldr      r0, [sp, #0x860]
00449794: ldr      r1, [sp, #0x85c]
00449798: bl       #0x752b38
0044979c: b        #0x447098
004497a0: ldr      r0, [sp, #0x874]
004497a4: ldr      r1, [sp, #0x870]
004497a8: bl       #0x752b38
004497ac: b        #0x447004
004497b0: ldr      r0, [sp, #0x888]
004497b4: ldr      r1, [sp, #0x884]
004497b8: bl       #0x752b38
004497bc: b        #0x446f74
004497c0: ldr      r0, [sp, #0x89c]
004497c4: ldr      r1, [sp, #0x898]
004497c8: bl       #0x752b38
004497cc: b        #0x446ee4
004497d0: ldr      r0, [sp, #0x8b0]
004497d4: ldr      r1, [sp, #0x8ac]
004497d8: bl       #0x752b38
004497dc: b        #0x446e68
004497e0: ldr      r0, [sp, #0x8c4]
004497e4: ldr      r1, [sp, #0x8c0]
004497e8: bl       #0x752b38
004497ec: b        #0x446df8
004497f0: bl       #0x30e310
# NativeGetSkillDetails _Z21NativeGetSkillDetailsRKN7gameswf7fn_callE
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
# NativeHUDGetActiveFaery _Z23NativeHUDGetActiveFaeryRKN7gameswf7fn_callE
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
# NativeHUDGetIsFaeryUnlocked _Z27NativeHUDGetIsFaeryUnlockedRKN7gameswf7fn_callE
0043ed58: push     {r4, r5, r6, r7, r8, sb, lr}
0043ed5c: ldr      r3, [r0, #0x10]
0043ed60: sub      sp, sp, #0x14
0043ed64: mov      r4, r0
0043ed68: cmp      r3, #2
0043ed6c: beq      #0x43ed78
0043ed70: add      sp, sp, #0x14
0043ed74: pop      {r4, r5, r6, r7, r8, sb, pc}
0043ed78: ldr      r7, [r0, #0xc]
0043ed7c: ldr      r6, [r0, #0x14]
0043ed80: mov      r5, #0xc
0043ed84: ldr      r3, [r7]
0043ed88: mla      r3, r5, r6, r3
0043ed8c: ldrsb    r2, [r3, #1]
0043ed90: cmp      r2, #2
0043ed94: bne      #0x43ed70
0043ed98: ldr      r2, [r3, #8]
0043ed9c: ldr      r3, [r3, #4]
0043eda0: str      r2, [sp, #0xc]
0043eda4: str      r3, [sp, #8]
0043eda8: ldrd     r8, sb, [sp, #8]
0043edac: mov      r0, r8
0043edb0: mov      r2, r8
0043edb4: mov      r1, sb
0043edb8: mov      r3, sb
0043edbc: strd     r8, sb, [sp]
0043edc0: bl       #0x30e2bc
0043edc4: subs     r8, r0, #0
0043edc8: bne      #0x43ed70
0043edcc: ldr      r3, [r7]
0043edd0: sub      r0, r6, #1
0043edd4: mla      r0, r5, r0, r3
0043edd8: bl       #0x439d8c
0043eddc: cmp      r0, #0
0043ede0: beq      #0x43ed70
0043ede4: ldr      r3, [r4, #0xc]
0043ede8: ldr      r0, [r4, #0x14]
0043edec: ldr      r3, [r3]
0043edf0: mla      r0, r5, r0, r3
0043edf4: bl       #0x43a1b8
0043edf8: ldr      r3, [r4, #0xc]
0043edfc: mov      r6, r0
0043ee00: ldr      r0, [r4, #0x14]
0043ee04: ldr      r3, [r3]
0043ee08: sub      r0, r0, #1
0043ee0c: mla      r0, r5, r0, r3
0043ee10: bl       #0x43a1b8
0043ee14: mov      r1, r8
0043ee18: bl       #0x43c388
0043ee1c: cmp      r0, #0
0043ee20: beq      #0x43ed70
0043ee24: mov      r1, r6
0043ee28: mvn      r2, #0
0043ee2c: bl       #0x3bbb94
0043ee30: mov      r1, r0
0043ee34: ldr      r0, [r4]
0043ee38: bl       #0x797230
0043ee3c: b        #0x43ed70
# NativeHUDSetActiveFaery _Z23NativeHUDSetActiveFaeryRKN7gameswf7fn_callE
0043ee40: push     {r4, r5, r6, r7, r8, sl, fp, lr}
0043ee44: ldr      r3, [r0, #0x10]
0043ee48: ldr      r4, [pc, #0x10c]
0043ee4c: sub      sp, sp, #0x10
0043ee50: cmp      r3, #2
0043ee54: mov      r5, r0
0043ee58: add      r4, pc, r4
0043ee5c: beq      #0x43ee68
0043ee60: add      sp, sp, #0x10
0043ee64: pop      {r4, r5, r6, r7, r8, sl, fp, pc}
0043ee68: ldr      r7, [r0, #0xc]
0043ee6c: ldr      r8, [r0, #0x14]
0043ee70: mov      r6, #0xc
0043ee74: ldr      r3, [r7]
0043ee78: mla      r3, r6, r8, r3
0043ee7c: ldrsb    r2, [r3, #1]
0043ee80: cmp      r2, #2
0043ee84: bne      #0x43ee60
0043ee88: ldr      r2, [r3, #8]
0043ee8c: ldr      r3, [r3, #4]
0043ee90: str      r2, [sp, #0xc]
0043ee94: str      r3, [sp, #8]
0043ee98: ldrd     sl, fp, [sp, #8]
0043ee9c: mov      r0, sl
0043eea0: mov      r2, sl
0043eea4: mov      r1, fp
0043eea8: mov      r3, fp
0043eeac: strd     sl, fp, [sp]
0043eeb0: bl       #0x30e2bc
0043eeb4: subs     sl, r0, #0
0043eeb8: bne      #0x43ee60
0043eebc: ldr      r3, [r7]
0043eec0: sub      r0, r8, #1
0043eec4: mla      r0, r6, r0, r3
0043eec8: bl       #0x439d8c
0043eecc: cmp      r0, #0
0043eed0: beq      #0x43ee60
0043eed4: ldr      r3, [r5, #0xc]
0043eed8: ldr      r0, [r5, #0x14]
0043eedc: ldr      r3, [r3]
0043eee0: mla      r0, r6, r0, r3
0043eee4: bl       #0x43a1b8
0043eee8: ldr      r3, [r5, #0xc]
0043eeec: mov      r7, r0
0043eef0: ldr      r0, [r5, #0x14]
0043eef4: ldr      r3, [r3]
0043eef8: sub      r0, r0, #1
0043eefc: mla      r0, r6, r0, r3
0043ef00: bl       #0x43a1b8
0043ef04: mov      r1, sl
0043ef08: bl       #0x43c388
0043ef0c: cmp      r0, #0
0043ef10: beq      #0x43ee60
0043ef14: mov      r1, r7
0043ef18: bl       #0x3ae99c
0043ef1c: ldr      r3, [pc, #0x3c]
0043ef20: ldr      r4, [r4, r3]
0043ef24: mov      r0, r4
0043ef28: bl       #0x31f594
0043ef2c: cmp      r0, #0
0043ef30: beq      #0x43ef44
0043ef34: mov      r0, r4
0043ef38: bl       #0x31f594
0043ef3c: mov      r1, sl
0043ef40: bl       #0x3f0898
0043ef44: ldr      r4, [r5]
0043ef48: mov      r0, r4
0043ef4c: bl       #0x797124
0043ef50: mov      r3, #0
0043ef54: strb     r3, [r4, #1]
0043ef58: b        #0x43ee60
0043ef5c: subseq   r5, r5, r8, lsr ip
0043ef60: strdeq   r3, r4, [r0], -r4
# NativeInvAutoEquipSlot _Z22NativeInvAutoEquipSlotRKN7gameswf7fn_callE
0043da2c: push     {r4, r5, r6, r7, r8, lr}
0043da30: ldr      r3, [r0, #0x10]
0043da34: mov      r4, r0
0043da38: cmp      r3, #2
0043da3c: beq      #0x43da44
0043da40: pop      {r4, r5, r6, r7, r8, pc}
0043da44: ldr      r3, [r0, #0xc]
0043da48: ldr      r0, [r0, #0x14]
0043da4c: mov      r5, #0xc
0043da50: ldr      r3, [r3]
0043da54: mla      r0, r5, r0, r3
0043da58: bl       #0x797a54
0043da5c: ldr      r3, [r4, #0xc]
0043da60: mov      r6, r0
0043da64: ldr      r0, [r4, #0x14]
0043da68: ldr      r3, [r3]
0043da6c: mov      r7, r1
0043da70: sub      r0, r0, #1
0043da74: mla      r0, r5, r0, r3
0043da78: bl       #0x797a54
0043da7c: bl       #0x30ea24
0043da80: mov      r1, #0
0043da84: bl       #0x43c388
0043da88: subs     r5, r0, #0
0043da8c: beq      #0x43da40
0043da90: mov      r1, r7
0043da94: mov      r0, r6
0043da98: bl       #0x30ea24
0043da9c: mov      r4, r0
0043daa0: add      r0, r5, #0x37c
0043daa4: bl       #0x3ffd20
0043daa8: cmp      r4, #0
0043daac: cmpge    r0, r4
0043dab0: bgt      #0x43dad0
0043dab4: cmn      r4, #1
0043dab8: bne      #0x43da40
0043dabc: mov      r0, r5
0043dac0: ldr      r3, [r5]
0043dac4: mov      lr, pc
0043dac8: ldr      pc, [r3, #0x138]
0043dacc: b        #0x43da40
0043dad0: mov      r0, r5
0043dad4: mov      r1, r4
0043dad8: ldr      r3, [r5]
0043dadc: mov      lr, pc
0043dae0: ldr      pc, [r3, #0x144]
0043dae4: mov      r0, r5
0043dae8: mov      r1, r4
0043daec: ldr      r3, [r5]
0043daf0: mov      lr, pc
0043daf4: ldr      pc, [r3, #0x140]
0043daf8: pop      {r4, r5, r6, r7, r8, pc}
# NativeInvDropItem _Z17NativeInvDropItemRKN7gameswf7fn_callE
0043cfb4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0043cfb8: ldr      r3, [r0, #0xc]
0043cfbc: ldr      r2, [r0, #0x14]
0043cfc0: mov      r0, #0xc
0043cfc4: ldr      r3, [r3]
0043cfc8: sub      sp, sp, #0x4c
0043cfcc: ldr      r4, [pc, #0x198]
0043cfd0: mla      r0, r0, r2, r3
0043cfd4: bl       #0x797a54
0043cfd8: ldr      r3, [pc, #0x190]
0043cfdc: add      r4, pc, r4
0043cfe0: mov      sb, r1
0043cfe4: ldr      r6, [r4, r3]
0043cfe8: mov      r1, #0
0043cfec: mov      r8, r0
0043cff0: mov      r2, r1
0043cff4: ldr      r0, [r6, #0x40]
0043cff8: bl       #0x36e478
0043cffc: ldr      r4, [r0, #0x660]
0043d000: cmp      r4, #0
0043d004: beq      #0x43d074
0043d008: mov      r1, sb
0043d00c: mov      r0, r8
0043d010: bl       #0x30ea24
0043d014: mov      r5, r0
0043d018: bl       #0x7fd794
0043d01c: ldrb     r3, [r0, #5]
0043d020: cmp      r3, #0
0043d024: addeq    r6, r4, #0x37c
0043d028: bne      #0x43d07c
0043d02c: add      r7, sp, #0x10
0043d030: mov      r0, r7
0043d034: bl       #0x3ff200
0043d038: mov      r1, r5
0043d03c: mov      r2, r7
0043d040: mov      r5, #0
0043d044: mov      r3, #1
0043d048: mov      r0, r6
0043d04c: str      r5, [sp]
0043d050: str      r5, [sp, #4]
0043d054: bl       #0x3ff858
0043d058: mov      r0, r7
0043d05c: mov      r1, r4
0043d060: mov      r2, r4
0043d064: mov      r3, r5
0043d068: bl       #0x3ec974
0043d06c: mov      r0, r7
0043d070: bl       #0x3ff460
0043d074: add      sp, sp, #0x4c
0043d078: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0043d07c: ldr      r0, [r6, #0x38]
0043d080: mov      r1, r4
0043d084: bl       #0x3402f4
0043d088: add      r6, r4, #0x37c
0043d08c: mov      r1, r5
0043d090: str      r0, [sp, #0xc]
0043d094: mov      r0, r6
0043d098: bl       #0x3fc61c
0043d09c: mov      fp, r0
0043d0a0: bl       #0x3f9e00
0043d0a4: str      r0, [sp, #8]
0043d0a8: mov      r0, fp
0043d0ac: bl       #0x3f9e80
0043d0b0: lsl      sb, r0, #2
0043d0b4: mov      r8, r0
0043d0b8: mov      r0, sb
0043d0bc: bl       #0x310454
0043d0c0: cmp      r8, #0
0043d0c4: mov      sl, r0
0043d0c8: ble      #0x43d0ec
0043d0cc: mov      r7, #0
0043d0d0: mov      r1, r7
0043d0d4: mov      r0, fp
0043d0d8: bl       #0x3fa038
0043d0dc: str      r0, [sl, r7, lsl #2]
0043d0e0: add      r7, r7, #1
0043d0e4: cmp      r8, r7
0043d0e8: bne      #0x43d0d0
0043d0ec: bl       #0x80b1bc
0043d0f0: mov      r8, r0
0043d0f4: ldr      r0, [pc, #0x78]
0043d0f8: mov      r1, #1
0043d0fc: add      r0, pc, r0
0043d100: bl       #0x80a244
0043d104: mov      r2, #1
0043d108: strb     r2, [r0, #0x50]
0043d10c: ldr      r2, [sp, #8]
0043d110: mov      r3, #0
0043d114: cmp      sl, #0
0043d118: cmpne    sb, #0
0043d11c: str      r2, [r0, #0x54]
0043d120: ldr      r2, [sp, #0xc]
0043d124: mov      r7, r0
0043d128: str      r3, [r0, #0x60]
0043d12c: str      r2, [r0, #0x58]
0043d130: str      r3, [r0, #0x5c]
0043d134: bgt      #0x43d148
0043d138: mov      r0, r8
0043d13c: mov      r1, r7
0043d140: bl       #0x80e2a4
0043d144: b        #0x43d02c
0043d148: str      sb, [r0, #0x64]
0043d14c: mov      r1, #2
0043d150: mov      r0, sb
0043d154: bl       #0x31056c
0043d158: mov      r1, sl
0043d15c: str      r0, [r7, #0x68]
0043d160: mov      r2, sb
0043d164: bl       #0x30e868
0043d168: b        #0x43d138
0043d16c: ldrheq   r7, [r5], #-0xa4
0043d170: strdeq   r3, r4, [r0], -r4
0043d174: subeq    r1, r8, ip, lsl #27
# NativeInvEquipItem _Z18NativeInvEquipItemRKN7gameswf7fn_callE
0043e600: push     {r4, r5, r6, r7, r8, sb, lr}
0043e604: ldr      r3, [r0, #0x10]
0043e608: sub      sp, sp, #0x1c
0043e60c: mov      r4, r0
0043e610: cmp      r3, #3
0043e614: beq      #0x43e620
0043e618: add      sp, sp, #0x1c
0043e61c: pop      {r4, r5, r6, r7, r8, sb, pc}
0043e620: ldr      r7, [r0, #0xc]
0043e624: ldr      r6, [r0, #0x14]
0043e628: mov      r5, #0xc
0043e62c: ldr      r3, [r7]
0043e630: mla      r3, r5, r6, r3
0043e634: ldrsb    r2, [r3, #1]
0043e638: cmp      r2, #2
0043e63c: bne      #0x43e618
0043e640: ldr      r2, [r3, #8]
0043e644: ldr      r3, [r3, #4]
0043e648: str      r2, [sp, #0x14]
0043e64c: str      r3, [sp, #0x10]
0043e650: ldrd     r8, sb, [sp, #0x10]
0043e654: mov      r0, r8
0043e658: mov      r2, r8
0043e65c: mov      r1, sb
0043e660: mov      r3, sb
0043e664: strd     r8, sb, [sp, #8]
0043e668: bl       #0x30e2bc
0043e66c: subs     r8, r0, #0
0043e670: bne      #0x43e618
0043e674: ldr      r3, [r7]
0043e678: sub      r0, r6, #1
0043e67c: mla      r0, r5, r0, r3
0043e680: bl       #0x439d8c
0043e684: cmp      r0, #0
0043e688: beq      #0x43e618
0043e68c: ldr      r3, [r4, #0xc]
0043e690: ldr      r0, [r4, #0x14]
0043e694: ldr      r3, [r3]
0043e698: sub      r0, r0, #2
0043e69c: mla      r0, r5, r0, r3
0043e6a0: bl       #0x439d8c
0043e6a4: cmp      r0, #0
0043e6a8: beq      #0x43e618
0043e6ac: ldr      r3, [r4, #0xc]
0043e6b0: ldr      r0, [r4, #0x14]
0043e6b4: ldr      r3, [r3]
0043e6b8: mla      r0, r5, r0, r3
0043e6bc: bl       #0x43a1b8
0043e6c0: ldr      r3, [r4, #0xc]
0043e6c4: mov      r6, r0
0043e6c8: ldr      r0, [r4, #0x14]
0043e6cc: ldr      r3, [r3]
0043e6d0: sub      r0, r0, #1
0043e6d4: mla      r0, r5, r0, r3
0043e6d8: bl       #0x43a1b8
0043e6dc: ldr      r3, [r4, #0xc]
0043e6e0: mov      r2, r0
0043e6e4: ldr      r0, [r4, #0x14]
0043e6e8: ldr      r3, [r3]
0043e6ec: str      r2, [sp, #4]
0043e6f0: sub      r0, r0, #2
0043e6f4: mla      r0, r5, r0, r3
0043e6f8: bl       #0x43a1b8
0043e6fc: mov      r1, r8
0043e700: bl       #0x43c388
0043e704: subs     r3, r0, #0
0043e708: ldr      r2, [sp, #4]
0043e70c: beq      #0x43e618
0043e710: mov      r1, r6
0043e714: ldr      r3, [r3]
0043e718: mov      lr, pc
0043e71c: ldr      pc, [r3, #0x13c]
0043e720: ldr      r4, [r4]
0043e724: mov      r0, r4
0043e728: bl       #0x797124
0043e72c: strb     r8, [r4, #1]
0043e730: b        #0x43e618
# NativeInvGetEquipedItem _Z23NativeInvGetEquipedItemRKN7gameswf7fn_callE
0043d790: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0043d794: ldr      r4, [pc, #0x26c]
0043d798: ldr      r6, [pc, #0x26c]
0043d79c: ldr      r2, [r0, #0x10]
0043d7a0: add      r4, pc, r4
0043d7a4: ldr      r3, [r4, r6]
0043d7a8: sub      sp, sp, #0xf4
0043d7ac: cmp      r2, #3
0043d7b0: ldr      r3, [r3]
0043d7b4: mov      r5, r0
0043d7b8: str      r3, [sp, #0xec]
0043d7bc: beq      #0x43d7dc
0043d7c0: ldr      r3, [r4, r6]
0043d7c4: ldr      r2, [sp, #0xec]
0043d7c8: ldr      r3, [r3]
0043d7cc: cmp      r2, r3
0043d7d0: bne      #0x43da04
0043d7d4: add      sp, sp, #0xf4
0043d7d8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0043d7dc: ldr      r3, [r0, #0xc]
0043d7e0: ldr      r0, [r0, #0x14]
0043d7e4: mov      r7, #0xc
0043d7e8: ldr      r3, [r3]
0043d7ec: mla      r0, r7, r0, r3
0043d7f0: bl       #0x797a54
0043d7f4: ldr      r3, [r5, #0xc]
0043d7f8: ldr      r2, [r5, #0x14]
0043d7fc: mov      sl, r0
0043d800: ldr      r3, [r3]
0043d804: sub      r2, r2, #1
0043d808: mov      fp, r1
0043d80c: mla      r7, r7, r2, r3
0043d810: ldrsb    r3, [r7, #1]
0043d814: cmp      r3, #5
0043d818: ldreq    r0, [r7, #4]
0043d81c: movne    r0, #0
0043d820: bl       #0x439cb4
0043d824: ldr      r3, [r5, #0xc]
0043d828: ldr      r2, [r5, #0x14]
0043d82c: mov      r7, r0
0043d830: ldr      r3, [r3]
0043d834: sub      r2, r2, #2
0043d838: mov      r0, #0xc
0043d83c: mla      r0, r0, r2, r3
0043d840: bl       #0x797a54
0043d844: bl       #0x30ea24
0043d848: mov      r1, #0
0043d84c: bl       #0x43c388
0043d850: subs     r8, r0, #0
0043d854: beq      #0x43d9cc
0043d858: mov      r0, sl
0043d85c: mov      r1, fp
0043d860: bl       #0x30ea24
0043d864: add      r8, r8, #0x37c
0043d868: mov      sb, r0
0043d86c: mov      r0, r8
0043d870: bl       #0x3ffd20
0043d874: cmp      sb, #0
0043d878: cmpge    r0, sb
0043d87c: movle    sl, #0
0043d880: movgt    sl, #1
0043d884: ble      #0x43d9dc
0043d888: mov      r1, sb
0043d88c: mov      r0, r8
0043d890: bl       #0x3ffe3c
0043d894: subs     r3, r0, #0
0043d898: str      r3, [sp, #4]
0043d89c: beq      #0x43d9cc
0043d8a0: bl       #0x3fa710
0043d8a4: ldr      r1, [pc, #0x164]
0043d8a8: mov      r2, r0
0043d8ac: ldr      r0, [sp, #4]
0043d8b0: add      ip, sp, #0x30
0043d8b4: add      r1, pc, r1
0043d8b8: ldr      r3, [r0, #0x1c]
0043d8bc: mov      r0, ip
0043d8c0: str      ip, [sp]
0043d8c4: bl       #0x30eae4
0043d8c8: ldr      r1, [pc, #0x144]
0043d8cc: ldr      r3, [r7]
0043d8d0: add      fp, sp, #0xd8
0043d8d4: add      r1, pc, r1
0043d8d8: mov      r0, fp
0043d8dc: ldr      sl, [r3, #0x1c]
0043d8e0: bl       #0x413a7c
0043d8e4: ldr      ip, [sp]
0043d8e8: add      sb, sp, #0x24
0043d8ec: mov      r0, sb
0043d8f0: mov      r1, ip
0043d8f4: bl       #0x43a2ac
0043d8f8: mov      r2, sb
0043d8fc: mov      r1, fp
0043d900: mov      r0, r7
0043d904: blx      sl
0043d908: mov      r0, sb
0043d90c: bl       #0x797124
0043d910: mov      r0, fp
0043d914: bl       #0x41fed8
0043d918: ldr      r1, [pc, #0xf8]
0043d91c: ldr      r3, [r7]
0043d920: add      sl, sp, #0xc4
0043d924: add      r1, pc, r1
0043d928: mov      r0, sl
0043d92c: ldr      sb, [r3, #0x1c]
0043d930: bl       #0x413a7c
0043d934: mov      r0, r8
0043d938: ldr      r1, [sp, #4]
0043d93c: bl       #0x3fc63c
0043d940: add      r8, sp, #0x18
0043d944: mov      r1, r0
0043d948: mov      r0, r8
0043d94c: bl       #0x439d50
0043d950: mov      r2, r8
0043d954: mov      r1, sl
0043d958: mov      r0, r7
0043d95c: blx      sb
0043d960: mov      r0, r8
0043d964: bl       #0x797124
0043d968: mov      r0, sl
0043d96c: bl       #0x41fed8
0043d970: ldr      r1, [pc, #0xa4]
0043d974: ldr      r3, [r7]
0043d978: add      r8, sp, #0xb0
0043d97c: add      r1, pc, r1
0043d980: mov      r0, r8
0043d984: ldr      sb, [r3, #0x1c]
0043d988: bl       #0x413a7c
0043d98c: ldr      r0, [sp, #4]
0043d990: bl       #0x3f9e80
0043d994: add      sl, sp, #0xc
0043d998: mov      r1, r0
0043d99c: mov      r0, sl
0043d9a0: bl       #0x439d50
0043d9a4: mov      r1, r8
0043d9a8: mov      r2, sl
0043d9ac: mov      r0, r7
0043d9b0: blx      sb
0043d9b4: mov      r0, sl
0043d9b8: bl       #0x797124
0043d9bc: mov      r0, r8
0043d9c0: bl       #0x41fed8
0043d9c4: mov      r1, #1
0043d9c8: b        #0x43d9d0
0043d9cc: mov      r1, #0
0043d9d0: ldr      r0, [r5]
0043d9d4: bl       #0x797230
0043d9d8: b        #0x43d7c0
0043d9dc: ldr      r3, [pc, #0x3c]
0043d9e0: ldr      r1, [pc, #0x3c]
0043d9e4: ldr      r2, [pc, #0x3c]
0043d9e8: ldr      r3, [r4, r3]
0043d9ec: add      r1, pc, r1
0043d9f0: add      r2, pc, r2
0043d9f4: ldr      r0, [r3, #0x2c]
0043d9f8: bl       #0x4c4bdc
0043d9fc: mov      r1, sl
0043da00: b        #0x43d9d0
0043da04: bl       #0x30e310
0043da08: ldrsheq  r7, [r5], #-0x20
0043da0c: andeq    r4, r0, ip, lsr #1
0043da10: subeq    lr, r8, r4, ror #6
0043da14: subeq    lr, r8, ip, ror #6
0043da18: subeq    lr, r8, ip, lsr #6
0043da1c: subeq    lr, r8, r4, ror #5
0043da20: strdeq   r3, r4, [r0], -r4
0043da24: subeq    sb, r8, ip, lsr sb
0043da28: subeq    ip, sb, r0, lsr #22
# NativeInvGetHasOffHandWeapon _Z28NativeInvGetHasOffHandWeaponRKN7gameswf7fn_callE
0043f044: push     {r4, r5, r6, r7, r8, sb, lr}
0043f048: ldr      r3, [r0, #0x10]
0043f04c: sub      sp, sp, #0x14
0043f050: mov      r4, r0
0043f054: cmp      r3, #1
0043f058: beq      #0x43f064
0043f05c: add      sp, sp, #0x14
0043f060: pop      {r4, r5, r6, r7, r8, sb, pc}
0043f064: ldr      r2, [r0, #0x14]
0043f068: ldr      r6, [r0, #0xc]
0043f06c: mov      r5, #0xc
0043f070: mul      r5, r5, r2
0043f074: ldr      r3, [r6]
0043f078: add      r3, r3, r5
0043f07c: ldrsb    r2, [r3, #1]
0043f080: cmp      r2, #2
0043f084: bne      #0x43f05c
0043f088: ldr      r2, [r3, #8]
0043f08c: ldr      r3, [r3, #4]
0043f090: str      r2, [sp, #0xc]
0043f094: str      r3, [sp, #8]
0043f098: ldrd     r8, sb, [sp, #8]
0043f09c: mov      r0, r8
0043f0a0: mov      r1, sb
0043f0a4: mov      r2, r8
0043f0a8: mov      r3, sb
0043f0ac: strd     r8, sb, [sp]
0043f0b0: bl       #0x30e2bc
0043f0b4: subs     r7, r0, #0
0043f0b8: bne      #0x43f05c
0043f0bc: ldr      r0, [r6]
0043f0c0: add      r0, r0, r5
0043f0c4: bl       #0x43a1b8
0043f0c8: mov      r1, r7
0043f0cc: bl       #0x43c388
0043f0d0: cmp      r0, #0
0043f0d4: beq      #0x43f0e0
0043f0d8: add      r0, r0, #0x37c
0043f0dc: bl       #0x400158
0043f0e0: mov      r1, r0
0043f0e4: ldr      r0, [r4]
0043f0e8: bl       #0x797230
0043f0ec: b        #0x43f05c
# NativeInvGetHasTwoHandedWeapon _Z30NativeInvGetHasTwoHandedWeaponRKN7gameswf7fn_callE
0043f0f0: push     {r4, r5, r6, r7, r8, sb, lr}
0043f0f4: ldr      r3, [r0, #0x10]
0043f0f8: sub      sp, sp, #0x14
0043f0fc: mov      r4, r0
0043f100: cmp      r3, #1
0043f104: beq      #0x43f110
0043f108: add      sp, sp, #0x14
0043f10c: pop      {r4, r5, r6, r7, r8, sb, pc}
0043f110: ldr      r2, [r0, #0x14]
0043f114: ldr      r6, [r0, #0xc]
0043f118: mov      r5, #0xc
0043f11c: mul      r5, r5, r2
0043f120: ldr      r3, [r6]
0043f124: add      r3, r3, r5
0043f128: ldrsb    r2, [r3, #1]
0043f12c: cmp      r2, #2
0043f130: bne      #0x43f108
0043f134: ldr      r2, [r3, #8]
0043f138: ldr      r3, [r3, #4]
0043f13c: str      r2, [sp, #0xc]
0043f140: str      r3, [sp, #8]
0043f144: ldrd     r8, sb, [sp, #8]
0043f148: mov      r0, r8
0043f14c: mov      r1, sb
0043f150: mov      r2, r8
0043f154: mov      r3, sb
0043f158: strd     r8, sb, [sp]
0043f15c: bl       #0x30e2bc
0043f160: subs     r7, r0, #0
0043f164: bne      #0x43f108
0043f168: ldr      r0, [r6]
0043f16c: add      r0, r0, r5
0043f170: bl       #0x43a1b8
0043f174: mov      r1, r7
0043f178: bl       #0x43c388
0043f17c: cmp      r0, #0
0043f180: beq      #0x43f190
0043f184: add      r0, r0, #0x37c
0043f188: mov      r1, r7
0043f18c: bl       #0x4001a0
0043f190: mov      r1, r0
0043f194: ldr      r0, [r4]
0043f198: bl       #0x797230
0043f19c: b        #0x43f108
# NativeInvGetItemDetails _Z23NativeInvGetItemDetailsRKN7gameswf7fn_callE
0044ca5c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044ca60: ldr      r6, [pc, #0xcb0]
0044ca64: ldr      r1, [pc, #0xcb0]
0044ca68: sub      sp, sp, #0x2ac
0044ca6c: add      r6, pc, r6
0044ca70: ldr      r3, [r6, r1]
0044ca74: str      r1, [sp, #0x14]
0044ca78: str      r0, [sp, #0x10]
0044ca7c: ldr      r3, [r3]
0044ca80: ldr      r7, [r0, #0xc]
0044ca84: ldr      r8, [r0, #0x14]
0044ca88: str      r3, [sp, #0x2a4]
0044ca8c: ldr      r3, [r7]
0044ca90: mov      r5, #0xc
0044ca94: mla      r3, r5, r8, r3
0044ca98: ldrsb    r2, [r3, #1]
0044ca9c: cmp      r2, #2
0044caa0: beq      #0x44cac4
0044caa4: ldr      r2, [sp, #0x14]
0044caa8: ldr      r3, [r6, r2]
0044caac: ldr      r2, [sp, #0x2a4]
0044cab0: ldr      r3, [r3]
0044cab4: cmp      r2, r3
0044cab8: bne      #0x44d714
0044cabc: add      sp, sp, #0x2ac
0044cac0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044cac4: ldr      ip, [r3, #8]
0044cac8: ldr      lr, [r3, #4]
0044cacc: mov      r3, #0x94000000
0044cad0: str      ip, [sp, #0xfc]
0044cad4: str      lr, [sp, #0xf8]
0044cad8: asr      r3, r3, #0x16
0044cadc: add      r2, sp, #0x2a8
0044cae0: ldrd     r0, r1, [r2, r3]
0044cae4: mov      r2, r0
0044cae8: mov      r3, r1
0044caec: str      ip, [sp, #0xf0]
0044caf0: str      lr, [sp, #0xec]
0044caf4: bl       #0x30e2bc
0044caf8: subs     r4, r0, #0
0044cafc: bne      #0x44caa4
0044cb00: ldr      r3, [r7]
0044cb04: sub      r2, r8, #1
0044cb08: mla      r2, r5, r2, r3
0044cb0c: ldrsb    r2, [r2, #1]
0044cb10: cmp      r2, #5
0044cb14: bne      #0x44caa4
0044cb18: sub      r0, r8, #2
0044cb1c: mla      r0, r5, r0, r3
0044cb20: bl       #0x439d8c
0044cb24: cmp      r0, #0
0044cb28: beq      #0x44caa4
0044cb2c: ldr      ip, [sp, #0x10]
0044cb30: mov      r7, #0xc
0044cb34: ldr      r3, [ip, #0xc]
0044cb38: ldr      r0, [ip, #0x14]
0044cb3c: ldr      r3, [r3]
0044cb40: mla      r0, r5, r0, r3
0044cb44: bl       #0x797a54
0044cb48: strd     r0, r1, [sp, #0x18]
0044cb4c: ldr      r1, [sp, #0x10]
0044cb50: ldr      r2, [r1, #0xc]
0044cb54: ldr      r3, [r1, #0x14]
0044cb58: ldr      r0, [r2]
0044cb5c: sub      r2, r3, #1
0044cb60: sub      r3, r3, #2
0044cb64: mla      r5, r5, r2, r0
0044cb68: mla      r0, r7, r3, r0
0044cb6c: ldrsb    r2, [r5, #1]
0044cb70: cmp      r2, #5
0044cb74: ldreq    r4, [r5, #4]
0044cb78: bl       #0x797a54
0044cb7c: bl       #0x30ea24
0044cb80: ldr      ip, [sp, #0x10]
0044cb84: mov      r5, r0
0044cb88: ldr      r3, [ip, #0x10]
0044cb8c: cmp      r3, #6
0044cb90: beq      #0x44d534
0044cb94: mov      sl, #0
0044cb98: mov      r8, sl
0044cb9c: mov      r0, r5
0044cba0: mov      r1, #0
0044cba4: bl       #0x43c388
0044cba8: cmp      r0, #0
0044cbac: str      r0, [sp, #0xc]
0044cbb0: beq      #0x44caa4
0044cbb4: cmp      sl, #0
0044cbb8: beq      #0x44d504
0044cbbc: mov      r0, sl
0044cbc0: bl       #0x3a30c4
0044cbc4: cmp      r0, #0
0044cbc8: beq      #0x44d504
0044cbcc: mov      r0, sl
0044cbd0: bl       #0x3a2fcc
0044cbd4: ldr      r3, [pc, #0xb44]
0044cbd8: mov      r5, #0x14
0044cbdc: ldr      r3, [r6, r3]
0044cbe0: ldr      r3, [r3]
0044cbe4: mla      r5, r5, r0, r3
0044cbe8: ldr      r0, [r5, #4]
0044cbec: bl       #0x30e964
0044cbf0: mov      r1, #0x42000000
0044cbf4: add      r1, r1, #0xc80000
0044cbf8: bl       #0x30ec94
0044cbfc: mov      sb, r0
0044cc00: ldr      r0, [r5, #8]
0044cc04: bl       #0x30e964
0044cc08: mov      r1, #0x42000000
0044cc0c: add      r1, r1, #0xc80000
0044cc10: bl       #0x30ec94
0044cc14: mov      r7, r0
0044cc18: ldrd     r0, r1, [sp, #0x18]
0044cc1c: bl       #0x30ea24
0044cc20: cmp      r8, #0
0044cc24: mov      r1, r0
0044cc28: beq      #0x44d520
0044cc2c: cmp      sl, #0
0044cc30: beq      #0x44d520
0044cc34: add      r0, sl, #0x37c
0044cc38: bl       #0x3fc61c
0044cc3c: mov      r5, r0
0044cc40: cmp      r5, #0
0044cc44: beq      #0x44caa4
0044cc48: ldr      ip, [sp, #0xc]
0044cc4c: ldr      r3, [r5, #0x54]
0044cc50: mov      r2, #0xc5
0044cc54: add      r1, ip, #0xff0
0044cc58: add      r0, ip, #0x560
0044cc5c: lsl      r3, r3, #8
0044cc60: add      r1, r1, #4
0044cc64: str      r3, [sp, #8]
0044cc68: bl       #0x3dedb4
0044cc6c: ldr      sl, [pc, #0xab0]
0044cc70: mov      fp, r0
0044cc74: ldr      r1, [pc, #0xaac]
0044cc78: ldr      r2, [r6, sl]
0044cc7c: add      fp, fp, #0x100
0044cc80: add      r1, pc, r1
0044cc84: ldr      r0, [r2, #0x2c]
0044cc88: ldr      r2, [pc, #0xa9c]
0044cc8c: add      r2, pc, r2
0044cc90: bl       #0x4c4bdc
0044cc94: ldr      r3, [sp, #8]
0044cc98: mul      r3, r3, fp
0044cc9c: asr      r3, r3, #8
0044cca0: mul      r3, r0, r3
0044cca4: ldr      r0, [r5, #0x54]
0044cca8: asr      r3, r3, #0x10
0044ccac: str      r3, [sp, #0x34]
0044ccb0: bl       #0x30e964
0044ccb4: mov      r1, sb
0044ccb8: str      r0, [sp, #8]
0044ccbc: bl       #0x30ed6c
0044ccc0: bl       #0x30e4cc
0044ccc4: ldr      r3, [sp, #8]
0044ccc8: mov      fp, r0
0044cccc: mov      r0, r7
0044ccd0: mov      r1, r3
0044ccd4: bl       #0x30ed6c
0044ccd8: bl       #0x30e4cc
0044ccdc: ldrb     r2, [r5, #0x69]
0044cce0: mov      r3, r0
0044cce4: cmp      r2, #0
0044cce8: beq      #0x44ccf8
0044ccec: cmp      r8, #0
0044ccf0: movne    sb, fp
0044ccf4: bne      #0x44cd00
0044ccf8: mov      sb, r3
0044ccfc: mov      r3, fp
0044cd00: add      r1, sp, #0x14c
0044cd04: mov      r0, r1
0044cd08: cmp      r3, #1
0044cd0c: movlt    r3, #1
0044cd10: str      r1, [sp, #0x2c]
0044cd14: mov      r1, #0x10
0044cd18: str      r3, [sp, #0x30]
0044cd1c: str      r0, [sp, #0x15c]
0044cd20: str      r0, [sp, #0x160]
0044cd24: bl       #0x31167c
0044cd28: add      r3, sp, #0x134
0044cd2c: str      r3, [sp, #0x28]
0044cd30: ldr      r3, [sp, #0x15c]
0044cd34: mov      r8, #0
0044cd38: mov      r1, #0x10
0044cd3c: strb     r8, [r3]
0044cd40: ldr      r0, [sp, #0x28]
0044cd44: ldr      r7, [pc, #0x9e4]
0044cd48: cmp      sb, #1
0044cd4c: movlt    sb, #1
0044cd50: str      r0, [sp, #0x144]
0044cd54: str      r0, [sp, #0x148]
0044cd58: bl       #0x31167c
0044cd5c: ldr      r3, [sp, #0x144]
0044cd60: add      r1, sp, #0x11c
0044cd64: str      r1, [sp, #0x24]
0044cd68: strb     r8, [r3]
0044cd6c: ldr      r2, [sp, #0x24]
0044cd70: mov      r0, r1
0044cd74: mov      r1, #0x10
0044cd78: str      r2, [sp, #0x12c]
0044cd7c: str      r2, [sp, #0x130]
0044cd80: bl       #0x31167c
0044cd84: add      r3, sp, #0x104
0044cd88: str      r3, [sp, #0x18]
0044cd8c: mov      r0, r3
0044cd90: ldr      r3, [sp, #0x12c]
0044cd94: mov      r1, #0x10
0044cd98: add      r7, pc, r7
0044cd9c: strb     r8, [r3]
0044cda0: ldr      ip, [sp, #0x18]
0044cda4: str      ip, [sp, #0x114]
0044cda8: str      ip, [sp, #0x118]
0044cdac: bl       #0x31167c
0044cdb0: ldr      r3, [sp, #0x114]
0044cdb4: ldr      sl, [r6, sl]
0044cdb8: mov      r2, r7
0044cdbc: strb     r8, [r3]
0044cdc0: ldr      r1, [sp, #0x2c]
0044cdc4: ldr      r3, [r5, #0x54]
0044cdc8: ldr      r0, [sl, #0x34]
0044cdcc: bl       #0x508ef4
0044cdd0: mov      r2, r7
0044cdd4: ldr      r1, [sp, #0x28]
0044cdd8: mov      r3, sb
0044cddc: ldr      r0, [sl, #0x34]
0044cde0: bl       #0x508ef4
0044cde4: ldr      r1, [sp, #0x34]
0044cde8: mov      r2, r7
0044cdec: ldr      r3, [sp, #0x30]
0044cdf0: cmp      r1, #1
0044cdf4: movlt    r1, #1
0044cdf8: str      r1, [sp, #0x34]
0044cdfc: ldr      r0, [sl, #0x34]
0044ce00: ldr      r1, [sp, #0x24]
0044ce04: bl       #0x508ef4
0044ce08: mov      r2, r7
0044ce0c: ldr      r0, [sl, #0x34]
0044ce10: ldr      r3, [sp, #0x34]
0044ce14: ldr      r1, [sp, #0x18]
0044ce18: bl       #0x508ef4
0044ce1c: ldr      r1, [pc, #0x910]
0044ce20: ldr      r3, [r4]
0044ce24: add      sl, sp, #0x290
0044ce28: add      r1, pc, r1
0044ce2c: mov      r0, sl
0044ce30: ldr      fp, [r3, #0x1c]
0044ce34: bl       #0x413a7c
0044ce38: ldr      r0, [r5, #0x54]
0044ce3c: mov      r3, #2
0044ce40: strb     r3, [sp, #0xe1]
0044ce44: strb     r8, [sp, #0xe0]
0044ce48: bl       #0x30ed30
0044ce4c: mov      r3, #0x94000000
0044ce50: asr      r3, r3, #0x16
0044ce54: add      r2, sp, #0x2a8
0044ce58: strd     r0, r1, [r2, r3]
0044ce5c: ldr      r3, [sp, #0xf8]
0044ce60: add      r7, sp, #0xe0
0044ce64: mov      r1, sl
0044ce68: str      r3, [sp, #0xe4]
0044ce6c: ldr      r3, [sp, #0xfc]
0044ce70: mov      r2, r7
0044ce74: mov      r0, r4
0044ce78: str      r3, [r7, #8]
0044ce7c: blx      fp
0044ce80: mov      r0, r7
0044ce84: bl       #0x797124
0044ce88: ldrb     ip, [sp, #0x290]
0044ce8c: sxtb     r3, ip
0044ce90: cmn      r3, #1
0044ce94: beq      #0x44d684
0044ce98: ldr      r1, [pc, #0x898]
0044ce9c: ldr      r3, [r4]
0044cea0: add      r8, sp, #0x27c
0044cea4: add      r1, pc, r1
0044cea8: mov      r0, r8
0044ceac: ldr      sl, [r3, #0x1c]
0044ceb0: bl       #0x413a7c
0044ceb4: mov      r3, #0
0044ceb8: strb     r3, [sp, #0xd4]
0044cebc: mov      r0, sb
0044cec0: mov      r3, #2
0044cec4: strb     r3, [sp, #0xd5]
0044cec8: bl       #0x30ed30
0044cecc: mov      r3, #0x94000000
0044ced0: asr      r3, r3, #0x16
0044ced4: add      r2, sp, #0x2a8
0044ced8: strd     r0, r1, [r2, r3]
0044cedc: ldr      r3, [sp, #0xf8]
0044cee0: add      r7, sp, #0xd4
0044cee4: mov      r1, r8
0044cee8: str      r3, [sp, #0xd8]
0044ceec: ldr      r3, [sp, #0xfc]
0044cef0: mov      r2, r7
0044cef4: mov      r0, r4
0044cef8: str      r3, [r7, #8]
0044cefc: blx      sl
0044cf00: mov      r0, r7
0044cf04: bl       #0x797124
0044cf08: ldrb     ip, [sp, #0x27c]
0044cf0c: sxtb     r3, ip
0044cf10: cmn      r3, #1
0044cf14: beq      #0x44d674
0044cf18: ldr      r1, [pc, #0x81c]
0044cf1c: ldr      r3, [r4]
0044cf20: add      sl, sp, #0x268
0044cf24: add      r1, pc, r1
0044cf28: mov      r0, sl
0044cf2c: ldr      r8, [r3, #0x1c]
0044cf30: bl       #0x413a7c
0044cf34: mov      r3, #0
0044cf38: ldr      r0, [sp, #0x30]
0044cf3c: strb     r3, [sp, #0xc8]
0044cf40: mov      r3, #2
0044cf44: strb     r3, [sp, #0xc9]
0044cf48: bl       #0x30ed30
0044cf4c: mov      r3, #0x94000000
0044cf50: asr      r3, r3, #0x16
0044cf54: add      r2, sp, #0x2a8
0044cf58: strd     r0, r1, [r2, r3]
0044cf5c: ldr      r3, [sp, #0xf8]
0044cf60: add      r7, sp, #0xc8
0044cf64: mov      r1, sl
0044cf68: str      r3, [sp, #0xcc]
0044cf6c: ldr      r3, [sp, #0xfc]
0044cf70: mov      r2, r7
0044cf74: mov      r0, r4
0044cf78: str      r3, [r7, #8]
0044cf7c: blx      r8
0044cf80: mov      r0, r7
0044cf84: bl       #0x797124
0044cf88: ldrb     ip, [sp, #0x268]
0044cf8c: sxtb     r3, ip
0044cf90: cmn      r3, #1
0044cf94: beq      #0x44d664
0044cf98: ldr      r1, [pc, #0x7a0]
0044cf9c: ldr      r3, [r4]
0044cfa0: add      sl, sp, #0x254
0044cfa4: add      r1, pc, r1
0044cfa8: mov      r0, sl
0044cfac: ldr      r8, [r3, #0x1c]
0044cfb0: bl       #0x413a7c
0044cfb4: mov      r3, #0
0044cfb8: ldr      r0, [sp, #0x34]
0044cfbc: strb     r3, [sp, #0xbc]
0044cfc0: mov      r3, #2
0044cfc4: strb     r3, [sp, #0xbd]
0044cfc8: bl       #0x30ed30
0044cfcc: mov      r3, #0x94000000
0044cfd0: asr      r3, r3, #0x16
0044cfd4: add      r2, sp, #0x2a8
0044cfd8: strd     r0, r1, [r2, r3]
0044cfdc: ldr      r3, [sp, #0xf8]
0044cfe0: add      r7, sp, #0xbc
0044cfe4: mov      r1, sl
0044cfe8: str      r3, [sp, #0xc0]
0044cfec: ldr      r3, [sp, #0xfc]
0044cff0: mov      r2, r7
0044cff4: mov      r0, r4
0044cff8: str      r3, [r7, #8]
0044cffc: blx      r8
0044d000: mov      r0, r7
0044d004: bl       #0x797124
0044d008: ldrb     ip, [sp, #0x254]
0044d00c: sxtb     r3, ip
0044d010: cmn      r3, #1
0044d014: beq      #0x44d654
0044d018: ldr      r1, [pc, #0x724]
0044d01c: ldr      r3, [r4]
0044d020: add      sl, sp, #0x240
0044d024: add      r7, sp, #0xb0
0044d028: add      r1, pc, r1
0044d02c: mov      r0, sl
0044d030: ldr      r8, [r3, #0x1c]
0044d034: bl       #0x413a7c
0044d038: mov      r3, #0
0044d03c: ldr      r1, [sp, #0x160]
0044d040: mov      r0, r7
0044d044: strb     r3, [sp, #0xb1]
0044d048: strb     r3, [sp, #0xb0]
0044d04c: bl       #0x797350
0044d050: mov      r1, sl
0044d054: mov      r2, r7
0044d058: mov      r0, r4
0044d05c: blx      r8
0044d060: mov      r0, r7
0044d064: bl       #0x797124
0044d068: ldrb     r1, [sp, #0x240]
0044d06c: sxtb     r3, r1
0044d070: cmn      r3, #1
0044d074: beq      #0x44d704
0044d078: ldr      r1, [pc, #0x6c8]
0044d07c: ldr      r3, [r4]
0044d080: add      sl, sp, #0x22c
0044d084: add      r7, sp, #0xa4
0044d088: add      r1, pc, r1
0044d08c: mov      r0, sl
0044d090: ldr      r8, [r3, #0x1c]
0044d094: bl       #0x413a7c
0044d098: mov      r3, #0
0044d09c: ldr      r1, [sp, #0x148]
0044d0a0: mov      r0, r7
0044d0a4: strb     r3, [sp, #0xa5]
0044d0a8: strb     r3, [sp, #0xa4]
0044d0ac: bl       #0x797350
0044d0b0: mov      r2, r7
0044d0b4: mov      r1, sl
0044d0b8: mov      r0, r4
0044d0bc: blx      r8
0044d0c0: mov      r0, r7
0044d0c4: bl       #0x797124
0044d0c8: ldrb     r2, [sp, #0x22c]
0044d0cc: sxtb     r3, r2
0044d0d0: cmn      r3, #1
0044d0d4: beq      #0x44d6f4
0044d0d8: ldr      r1, [pc, #0x66c]
0044d0dc: ldr      r3, [r4]
0044d0e0: add      sl, sp, #0x218
0044d0e4: add      r7, sp, #0x98
0044d0e8: add      r1, pc, r1
0044d0ec: mov      r0, sl
0044d0f0: ldr      r8, [r3, #0x1c]
0044d0f4: bl       #0x413a7c
0044d0f8: mov      r3, #0
0044d0fc: ldr      r1, [sp, #0x130]
0044d100: mov      r0, r7
0044d104: strb     r3, [sp, #0x99]
0044d108: strb     r3, [sp, #0x98]
0044d10c: bl       #0x797350
0044d110: mov      r1, sl
0044d114: mov      r2, r7
0044d118: mov      r0, r4
0044d11c: blx      r8
0044d120: mov      r0, r7
0044d124: bl       #0x797124
0044d128: ldrb     ip, [sp, #0x218]
0044d12c: sxtb     r3, ip
0044d130: cmn      r3, #1
0044d134: beq      #0x44d6e4
0044d138: ldr      r1, [pc, #0x610]
0044d13c: ldr      r3, [r4]
0044d140: add      sl, sp, #0x204
0044d144: add      r7, sp, #0x8c
0044d148: add      r1, pc, r1
0044d14c: mov      r0, sl
0044d150: ldr      r8, [r3, #0x1c]
0044d154: bl       #0x413a7c
0044d158: mov      r3, #0
0044d15c: ldr      r1, [sp, #0x118]
0044d160: mov      r0, r7
0044d164: strb     r3, [sp, #0x8d]
0044d168: strb     r3, [sp, #0x8c]
0044d16c: bl       #0x797350
0044d170: mov      r1, sl
0044d174: mov      r2, r7
0044d178: mov      r0, r4
0044d17c: blx      r8
0044d180: mov      r0, r7
0044d184: bl       #0x797124
0044d188: ldrb     r1, [sp, #0x204]
0044d18c: sxtb     r3, r1
0044d190: cmn      r3, #1
0044d194: beq      #0x44d6d4
0044d198: ldr      r1, [pc, #0x5b4]
0044d19c: ldr      r3, [r4]
0044d1a0: add      sl, sp, #0x1f0
0044d1a4: add      r1, pc, r1
0044d1a8: mov      r0, sl
0044d1ac: ldr      r8, [r3, #0x1c]
0044d1b0: bl       #0x413a7c
0044d1b4: ldr      r1, [sp, #0xc]
0044d1b8: mov      r0, r5
0044d1bc: bl       #0x3fa330
0044d1c0: mov      r3, #0
0044d1c4: add      r7, sp, #0x80
0044d1c8: strb     r3, [sp, #0x80]
0044d1cc: mov      r3, #1
0044d1d0: strb     r3, [sp, #0x81]
0044d1d4: mov      r2, r7
0044d1d8: strb     r0, [sp, #0x84]
0044d1dc: mov      r1, sl
0044d1e0: mov      r0, r4
0044d1e4: blx      r8
0044d1e8: mov      r0, r7
0044d1ec: bl       #0x797124
0044d1f0: ldrb     r2, [sp, #0x1f0]
0044d1f4: sxtb     r3, r2
0044d1f8: cmn      r3, #1
0044d1fc: beq      #0x44d6c4
0044d200: ldr      r1, [pc, #0x550]
0044d204: ldr      r3, [r4]
0044d208: add      sl, sp, #0x1dc
0044d20c: add      r7, sp, #0x74
0044d210: add      r1, pc, r1
0044d214: mov      r0, sl
0044d218: ldr      r8, [r3, #0x1c]
0044d21c: bl       #0x413a7c
0044d220: mov      r3, #0
0044d224: ldr      r1, [r5, #0x4c]
0044d228: mov      r0, r7
0044d22c: strb     r3, [sp, #0x75]
0044d230: strb     r3, [sp, #0x74]
0044d234: bl       #0x797350
0044d238: mov      r1, sl
0044d23c: mov      r2, r7
0044d240: mov      r0, r4
0044d244: blx      r8
0044d248: mov      r0, r7
0044d24c: bl       #0x797124
0044d250: ldrb     ip, [sp, #0x1dc]
0044d254: sxtb     r3, ip
0044d258: cmn      r3, #1
0044d25c: beq      #0x44d6b4
0044d260: ldr      r1, [pc, #0x4f4]
0044d264: ldr      r3, [r4]
0044d268: add      sl, sp, #0x1c8
0044d26c: add      r7, sp, #0x68
0044d270: add      r1, pc, r1
0044d274: mov      r0, sl
0044d278: ldr      r8, [r3, #0x1c]
0044d27c: bl       #0x413a7c
0044d280: mov      r3, #0
0044d284: ldr      r1, [r5, #0x34]
0044d288: mov      r0, r7
0044d28c: strb     r3, [sp, #0x69]
0044d290: strb     r3, [sp, #0x68]
0044d294: bl       #0x797350
0044d298: mov      r1, sl
0044d29c: mov      r2, r7
0044d2a0: mov      r0, r4
0044d2a4: blx      r8
0044d2a8: mov      r0, r7
0044d2ac: bl       #0x797124
0044d2b0: ldrb     r1, [sp, #0x1c8]
0044d2b4: sxtb     r3, r1
0044d2b8: cmn      r3, #1
0044d2bc: beq      #0x44d6a4
0044d2c0: ldr      r1, [pc, #0x498]
0044d2c4: ldr      r3, [r4]
0044d2c8: add      sl, sp, #0x1b4
0044d2cc: add      r1, pc, r1
0044d2d0: mov      r0, sl
0044d2d4: ldr      r8, [r3, #0x1c]
0044d2d8: bl       #0x413a7c
0044d2dc: mov      r0, r5
0044d2e0: bl       #0x3f9e80
0044d2e4: mov      r3, #0
0044d2e8: strb     r3, [sp, #0x5c]
0044d2ec: mov      r3, #2
0044d2f0: strb     r3, [sp, #0x5d]
0044d2f4: bl       #0x30ed30
0044d2f8: mov      r3, #0x94000000
0044d2fc: asr      r3, r3, #0x16
0044d300: add      r2, sp, #0x2a8
0044d304: strd     r0, r1, [r2, r3]
0044d308: ldr      r3, [sp, #0xf8]
0044d30c: add      r7, sp, #0x5c
0044d310: mov      r1, sl
0044d314: str      r3, [sp, #0x60]
0044d318: ldr      r3, [sp, #0xfc]
0044d31c: mov      r2, r7
0044d320: mov      r0, r4
0044d324: str      r3, [r7, #8]
0044d328: blx      r8
0044d32c: mov      r0, r7
0044d330: bl       #0x797124
0044d334: ldrb     ip, [sp, #0x1b4]
0044d338: sxtb     r3, ip
0044d33c: cmn      r3, #1
0044d340: beq      #0x44d694
0044d344: ldr      r1, [pc, #0x418]
0044d348: ldr      r3, [r4]
0044d34c: add      sl, sp, #0x1a0
0044d350: add      r1, pc, r1
0044d354: mov      r0, sl
0044d358: ldr      r8, [r3, #0x1c]
0044d35c: bl       #0x413a7c
0044d360: mov      r0, r5
0044d364: bl       #0x3f9e94
0044d368: add      r7, sp, #0x50
0044d36c: mov      r3, #0
0044d370: mov      r1, r0
0044d374: mov      r0, r7
0044d378: strb     r3, [sp, #0x51]
0044d37c: strb     r3, [sp, #0x50]
0044d380: bl       #0x797350
0044d384: mov      r1, sl
0044d388: mov      r2, r7
0044d38c: mov      r0, r4
0044d390: blx      r8
0044d394: mov      r0, r7
0044d398: bl       #0x797124
0044d39c: ldrb     r3, [sp, #0x1a0]
0044d3a0: cmp      r3, #0xff
0044d3a4: beq      #0x44d644
0044d3a8: ldr      r1, [pc, #0x3b8]
0044d3ac: ldr      r3, [r4]
0044d3b0: add      sl, sp, #0x18c
0044d3b4: add      r1, pc, r1
0044d3b8: mov      r0, sl
0044d3bc: ldr      r8, [r3, #0x1c]
0044d3c0: bl       #0x413a7c
0044d3c4: mov      r0, r5
0044d3c8: bl       #0x3f9e58
0044d3cc: mov      r3, #0
0044d3d0: add      r7, sp, #0x44
0044d3d4: strb     r3, [sp, #0x44]
0044d3d8: mov      r3, #1
0044d3dc: strb     r3, [sp, #0x45]
0044d3e0: strb     r0, [sp, #0x48]
0044d3e4: mov      r1, sl
0044d3e8: mov      r2, r7
0044d3ec: mov      r0, r4
0044d3f0: blx      r8
0044d3f4: mov      r0, r7
0044d3f8: bl       #0x797124
0044d3fc: ldrb     r3, [sp, #0x18c]
0044d400: cmp      r3, #0xff
0044d404: beq      #0x44d634
0044d408: ldr      r3, [pc, #0x35c]
0044d40c: mov      r0, r5
0044d410: str      r6, [sp, #0x30]
0044d414: add      r3, pc, r3
0044d418: str      r3, [sp, #0xc]
0044d41c: bl       #0x3f9e80
0044d420: mov      r7, #0
0044d424: cmp      r7, r0
0044d428: add      sl, sp, #0x178
0044d42c: add      sb, sp, #0x164
0044d430: mov      fp, r7
0044d434: add      r8, sp, #0x38
0044d438: bge      #0x44d4b8
0044d43c: mov      r2, r7
0044d440: ldr      r1, [sp, #0xc]
0044d444: mov      r0, sl
0044d448: bl       #0x30eae4
0044d44c: ldr      r3, [r4]
0044d450: mov      r1, sl
0044d454: mov      r0, sb
0044d458: ldr      r6, [r3, #0x1c]
0044d45c: bl       #0x413a7c
0044d460: mov      r1, r7
0044d464: mov      r0, r5
0044d468: bl       #0x3f9f88
0044d46c: mov      r1, r0
0044d470: mov      r0, r8
0044d474: strb     fp, [sp, #0x38]
0044d478: strb     fp, [sp, #0x39]
0044d47c: bl       #0x797350
0044d480: mov      r1, sb
0044d484: mov      r2, r8
0044d488: mov      r0, r4
0044d48c: blx      r6
0044d490: mov      r0, r8
0044d494: bl       #0x797124
0044d498: ldrb     r3, [sp, #0x164]
0044d49c: cmp      r3, #0xff
0044d4a0: beq      #0x44d4f0
0044d4a4: add      r7, r7, #1
0044d4a8: mov      r0, r5
0044d4ac: bl       #0x3f9e80
0044d4b0: cmp      r7, r0
0044d4b4: blt      #0x44d43c
0044d4b8: ldr      r0, [sp, #0x18]
0044d4bc: ldr      r6, [sp, #0x30]
0044d4c0: bl       #0x3139ac
0044d4c4: ldr      r0, [sp, #0x24]
0044d4c8: bl       #0x3139ac
0044d4cc: ldr      r0, [sp, #0x28]
0044d4d0: bl       #0x3139ac
0044d4d4: ldr      r0, [sp, #0x2c]
0044d4d8: bl       #0x3139ac
0044d4dc: ldr      r3, [sp, #0x10]
0044d4e0: mov      r1, r4
0044d4e4: ldr      r0, [r3]
0044d4e8: bl       #0x797250
0044d4ec: b        #0x44caa4
0044d4f0: ldr      r0, [sp, #0x170]
0044d4f4: ldr      r1, [sp, #0x16c]
0044d4f8: bl       #0x752b38
0044d4fc: add      r7, r7, #1
0044d500: b        #0x44d4a8
0044d504: ldrd     r0, r1, [sp, #0x18]
0044d508: bl       #0x30ea24
0044d50c: mov      r7, #0
0044d510: cmp      r8, #0
0044d514: mov      sb, r7
0044d518: mov      r1, r0
0044d51c: bne      #0x44cc2c
0044d520: ldr      r2, [sp, #0xc]
0044d524: add      r0, r2, #0x37c
0044d528: bl       #0x3fc61c
0044d52c: mov      r5, r0
0044d530: b        #0x44cc40
0044d534: ldr      r3, [ip, #0xc]
0044d538: ldr      r2, [ip, #0x14]
0044d53c: ldr      r3, [r3]
0044d540: sub      r1, r2, #3
0044d544: mla      r1, r7, r1, r3
0044d548: ldrsb    r1, [r1, #1]
0044d54c: cmp      r1, #1
0044d550: bne      #0x44cb94
0044d554: sub      r1, r2, #4
0044d558: mla      r1, r7, r1, r3
0044d55c: ldrb     r1, [r1, #1]
0044d560: sub      r1, r1, #3
0044d564: uxtb     r1, r1
0044d568: cmp      r1, #1
0044d56c: bhi      #0x44cb94
0044d570: sub      r2, r2, #5
0044d574: mla      r0, r7, r2, r3
0044d578: bl       #0x439d8c
0044d57c: cmp      r0, #0
0044d580: beq      #0x44cb94
0044d584: ldr      r1, [sp, #0x10]
0044d588: add      sl, sp, #0xec
0044d58c: ldr      r3, [r1, #0xc]
0044d590: ldr      r0, [r1, #0x14]
0044d594: ldr      r3, [r3]
0044d598: sub      r0, r0, #3
0044d59c: mla      r0, r7, r0, r3
0044d5a0: bl       #0x797960
0044d5a4: ldr      r2, [sp, #0x10]
0044d5a8: mov      r8, r0
0044d5ac: ldr      r3, [r2, #0xc]
0044d5b0: ldr      r0, [r2, #0x14]
0044d5b4: ldr      r3, [r3]
0044d5b8: sub      r0, r0, #4
0044d5bc: mla      r0, r7, r0, r3
0044d5c0: bl       #0x796fb4
0044d5c4: ldr      ip, [sp, #0x10]
0044d5c8: mov      sb, r0
0044d5cc: ldr      r3, [ip, #0xc]
0044d5d0: ldr      r0, [ip, #0x14]
0044d5d4: ldr      r3, [r3]
0044d5d8: sub      r0, r0, #5
0044d5dc: mla      r0, r7, r0, r3
0044d5e0: bl       #0x797a54
0044d5e4: ldr      r3, [pc, #0x138]
0044d5e8: ldr      r7, [r6, r3]
0044d5ec: bl       #0x30ea24
0044d5f0: ldr      r1, [r7, #0x38]
0044d5f4: mov      ip, #0
0044d5f8: mov      r3, r0
0044d5fc: mov      r2, sb
0044d600: mov      r0, sl
0044d604: str      ip, [sp, #4]
0044d608: str      ip, [sp]
0044d60c: bl       #0x34aca0
0044d610: mov      r0, sl
0044d614: bl       #0x33ff54
0044d618: subs     sl, r0, #0
0044d61c: beq      #0x44d62c
0044d620: bl       #0x3a30c4
0044d624: cmp      r0, #0
0044d628: bne      #0x44cb9c
0044d62c: mov      sl, #0
0044d630: b        #0x44cb9c
0044d634: ldr      r0, [sp, #0x198]
0044d638: ldr      r1, [sp, #0x194]
0044d63c: bl       #0x752b38
0044d640: b        #0x44d408
0044d644: ldr      r0, [sp, #0x1ac]
0044d648: ldr      r1, [sp, #0x1a8]
0044d64c: bl       #0x752b38
0044d650: b        #0x44d3a8
0044d654: ldr      r0, [sp, #0x260]
0044d658: ldr      r1, [sp, #0x25c]
0044d65c: bl       #0x752b38
0044d660: b        #0x44d018
0044d664: ldr      r0, [sp, #0x274]
0044d668: ldr      r1, [sp, #0x270]
0044d66c: bl       #0x752b38
0044d670: b        #0x44cf98
0044d674: ldr      r0, [sp, #0x288]
0044d678: ldr      r1, [sp, #0x284]
0044d67c: bl       #0x752b38
0044d680: b        #0x44cf18
0044d684: ldr      r0, [sp, #0x29c]
0044d688: ldr      r1, [sp, #0x298]
0044d68c: bl       #0x752b38
0044d690: b        #0x44ce98
0044d694: ldr      r0, [sp, #0x1c0]
0044d698: ldr      r1, [sp, #0x1bc]
0044d69c: bl       #0x752b38
0044d6a0: b        #0x44d344
0044d6a4: ldr      r0, [sp, #0x1d4]
0044d6a8: ldr      r1, [sp, #0x1d0]
0044d6ac: bl       #0x752b38
0044d6b0: b        #0x44d2c0
0044d6b4: ldr      r0, [sp, #0x1e8]
0044d6b8: ldr      r1, [sp, #0x1e4]
0044d6bc: bl       #0x752b38
0044d6c0: b        #0x44d260
0044d6c4: ldr      r0, [sp, #0x1fc]
0044d6c8: ldr      r1, [sp, #0x1f8]
0044d6cc: bl       #0x752b38
0044d6d0: b        #0x44d200
0044d6d4: ldr      r0, [sp, #0x210]
0044d6d8: ldr      r1, [sp, #0x20c]
0044d6dc: bl       #0x752b38
0044d6e0: b        #0x44d198
0044d6e4: ldr      r0, [sp, #0x224]
0044d6e8: ldr      r1, [sp, #0x220]
0044d6ec: bl       #0x752b38
0044d6f0: b        #0x44d138
0044d6f4: ldr      r0, [sp, #0x238]
0044d6f8: ldr      r1, [sp, #0x234]
0044d6fc: bl       #0x752b38
0044d700: b        #0x44d0d8
0044d704: ldr      r0, [sp, #0x24c]
0044d708: ldr      r1, [sp, #0x248]
0044d70c: bl       #0x752b38
0044d710: b        #0x44d078
0044d714: bl       #0x30e310
0044d718: subseq   r8, r4, r4, lsr #32
0044d71c: andeq    r4, r0, ip, lsr #1
0044d720: andeq    r4, r0, r4, ror r1
0044d724: strdeq   r3, r4, [r0], -r4
0044d728: ldrdeq   r4, r5, [r7], #-0xa0
0044d72c: subeq    r6, r7, ip, ror #9
0044d730: subeq    ip, r7, r0, lsr #5
0044d734: subeq    pc, r7, r8, lsl #23
0044d738: subeq    pc, r7, ip, lsl fp
0044d73c: subeq    pc, r7, ip, lsr #21
0044d740: subeq    pc, r7, ip, lsr sl
# NativeInvGetItemsListForSlot _Z28NativeInvGetItemsListForSlotRKN7gameswf7fn_callE
0044bb94: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044bb98: ldr      r4, [pc, #0x844]
0044bb9c: ldr      r5, [pc, #0x844]
0044bba0: ldr      r2, [r0, #0x10]
0044bba4: add      r4, pc, r4
0044bba8: ldr      r3, [r4, r5]
0044bbac: sub      sp, sp, #0x244
0044bbb0: cmp      r2, #3
0044bbb4: ldr      r3, [r3]
0044bbb8: mov      r7, r0
0044bbbc: str      r3, [sp, #0x23c]
0044bbc0: beq      #0x44bbe0
0044bbc4: ldr      r3, [r4, r5]
0044bbc8: ldr      r2, [sp, #0x23c]
0044bbcc: ldr      r3, [r3]
0044bbd0: cmp      r2, r3
0044bbd4: bne      #0x44c3e0
0044bbd8: add      sp, sp, #0x244
0044bbdc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044bbe0: ldr      r6, [r0, #0xc]
0044bbe4: ldr      sl, [r0, #0x14]
0044bbe8: mov      r8, #0xc
0044bbec: ldr      r3, [r6]
0044bbf0: mla      r3, r8, sl, r3
0044bbf4: ldrsb    r2, [r3, #1]
0044bbf8: cmp      r2, #2
0044bbfc: bne      #0x44bbc4
0044bc00: ldr      ip, [r3, #8]
0044bc04: ldr      lr, [r3, #4]
0044bc08: mov      r3, #0x9a000000
0044bc0c: str      ip, [sp, #0xac]
0044bc10: str      lr, [sp, #0xa8]
0044bc14: asr      r3, r3, #0x16
0044bc18: add      r2, sp, #0x240
0044bc1c: ldrd     r0, r1, [r2, r3]
0044bc20: mov      r2, r0
0044bc24: mov      r3, r1
0044bc28: str      ip, [sp, #0xb8]
0044bc2c: str      lr, [sp, #0xb4]
0044bc30: bl       #0x30e2bc
0044bc34: subs     sb, r0, #0
0044bc38: bne      #0x44bbc4
0044bc3c: ldr      r3, [r6]
0044bc40: sub      r2, sl, #1
0044bc44: mla      r2, r8, r2, r3
0044bc48: ldrsb    r2, [r2, #1]
0044bc4c: cmp      r2, #5
0044bc50: bne      #0x44bbc4
0044bc54: sub      r0, sl, #2
0044bc58: mla      r0, r8, r0, r3
0044bc5c: bl       #0x439d8c
0044bc60: cmp      r0, #0
0044bc64: beq      #0x44bbc4
0044bc68: ldr      r3, [r7, #0xc]
0044bc6c: ldr      r0, [r7, #0x14]
0044bc70: ldr      r3, [r3]
0044bc74: mla      r0, r8, r0, r3
0044bc78: bl       #0x43a1b8
0044bc7c: str      r0, [sp, #8]
0044bc80: ldr      r3, [r7, #0xc]
0044bc84: ldr      r2, [r7, #0x14]
0044bc88: ldr      r3, [r3]
0044bc8c: sub      r2, r2, #1
0044bc90: mla      r8, r8, r2, r3
0044bc94: ldrsb    r3, [r8, #1]
0044bc98: cmp      r3, #5
0044bc9c: movne    r0, sb
0044bca0: ldreq    r0, [r8, #4]
0044bca4: bl       #0x439ce8
0044bca8: str      r0, [sp, #0x10]
0044bcac: ldr      r3, [r7, #0xc]
0044bcb0: ldr      r2, [r7, #0x14]
0044bcb4: mov      r0, #0xc
0044bcb8: ldr      r3, [r3]
0044bcbc: sub      r2, r2, #2
0044bcc0: mov      r8, #0
0044bcc4: mla      r0, r0, r2, r3
0044bcc8: bl       #0x43a1b8
0044bccc: mov      r1, r8
0044bcd0: bl       #0x43c388
0044bcd4: cmp      r0, #0
0044bcd8: str      r0, [sp, #0x38]
0044bcdc: str      r8, [sp, #0x9c]
0044bce0: str      r8, [sp, #0xa0]
0044bce4: str      r8, [sp, #0xa4]
0044bce8: beq      #0x44c3d4
0044bcec: ldr      r3, [pc, #0x6f8]
0044bcf0: ldr      r6, [pc, #0x6f8]
0044bcf4: ldr      r2, [pc, #0x6f8]
0044bcf8: ldr      sl, [r4, r3]
0044bcfc: add      r6, pc, r6
0044bd00: add      r2, pc, r2
0044bd04: mov      r1, r6
0044bd08: ldr      r0, [sl, #0x2c]
0044bd0c: str      r3, [sp, #0x34]
0044bd10: bl       #0x4c4bdc
0044bd14: ldr      ip, [sp, #8]
0044bd18: ldr      r2, [pc, #0x6d8]
0044bd1c: mov      r1, r6
0044bd20: cmp      ip, r0
0044bd24: movne    sb, #0
0044bd28: moveq    sb, #1
0044bd2c: add      r2, pc, r2
0044bd30: ldr      r0, [sl, #0x2c]
0044bd34: bl       #0x4c4bdc
0044bd38: ldr      r2, [sp, #8]
0044bd3c: cmp      r2, r0
0044bd40: beq      #0x44c384
0044bd44: ldr      r2, [pc, #0x6b0]
0044bd48: ldr      r0, [sl, #0x2c]
0044bd4c: mov      r1, r6
0044bd50: add      r2, pc, r2
0044bd54: bl       #0x4c4bdc
0044bd58: ldr      r3, [sp, #8]
0044bd5c: cmp      r3, r0
0044bd60: strne    r8, [sp, #0x18]
0044bd64: beq      #0x44c384
0044bd68: ldr      r2, [sp, #0x34]
0044bd6c: ldr      r6, [pc, #0x68c]
0044bd70: ldr      r8, [r4, r2]
0044bd74: ldr      r2, [pc, #0x688]
0044bd78: add      r6, pc, r6
0044bd7c: ldr      r0, [r8, #0x2c]
0044bd80: add      r2, pc, r2
0044bd84: mov      r1, r6
0044bd88: bl       #0x4c4bdc
0044bd8c: ldr      r3, [sp, #8]
0044bd90: cmp      r3, r0
0044bd94: beq      #0x44c378
0044bd98: ldr      r2, [pc, #0x668]
0044bd9c: ldr      r0, [r8, #0x2c]
0044bda0: mov      r1, r6
0044bda4: add      r2, pc, r2
0044bda8: bl       #0x4c4bdc
0044bdac: ldr      ip, [sp, #8]
0044bdb0: cmp      ip, r0
0044bdb4: movne    r2, #0
0044bdb8: strne    r2, [sp, #0xc]
0044bdbc: beq      #0x44c378
0044bdc0: cmp      sb, #0
0044bdc4: beq      #0x44c390
0044bdc8: ldr      ip, [sp, #0x38]
0044bdcc: add      r2, sp, #0x9c
0044bdd0: mov      r1, r2
0044bdd4: add      ip, ip, #0x37c
0044bdd8: mov      r0, ip
0044bddc: str      ip, [sp, #0x30]
0044bde0: str      r2, [sp, #0x40]
0044bde4: bl       #0x3fee50
0044bde8: ldr      r3, [sp, #8]
0044bdec: ldr      r2, [sp, #0x38]
0044bdf0: ldr      r0, [sp, #0x9c]
0044bdf4: ldr      r1, [sp, #0xa0]
0044bdf8: str      r3, [sp, #0xc0]
0044bdfc: str      r2, [sp, #0xbc]
0044be00: bl       #0x43bff4
0044be04: ldr      r6, [sp, #0x9c]
0044be08: ldr      r3, [sp, #0xa0]
0044be0c: cmp      r6, r3
0044be10: beq      #0x44c1c0
0044be14: ldr      r2, [pc, #0x5f0]
0044be18: str      r5, [sp, #0x44]
0044be1c: str      r7, [sp, #0x14]
0044be20: str      r2, [sp, #0x2c]
0044be24: ldr      r2, [pc, #0x5e4]
0044be28: str      r4, [sp, #0x3c]
0044be2c: mov      r5, sb
0044be30: add      r2, pc, r2
0044be34: str      r2, [sp, #0x1c]
0044be38: ldr      r2, [pc, #0x5d4]
0044be3c: add      r2, pc, r2
0044be40: str      r2, [sp, #0x20]
0044be44: ldr      r2, [pc, #0x5cc]
0044be48: add      r2, pc, r2
0044be4c: str      r2, [sp, #0x24]
0044be50: ldr      r2, [pc, #0x5c4]
0044be54: add      r2, pc, r2
0044be58: str      r2, [sp, #0x28]
0044be5c: ldr      r4, [r6]
0044be60: cmp      r4, #0
0044be64: beq      #0x44c1a8
0044be68: mov      r0, r4
0044be6c: bl       #0x3fa710
0044be70: add      sb, sp, #0xc4
0044be74: mov      r2, r0
0044be78: ldr      r3, [r4, #0x1c]
0044be7c: mov      r0, sb
0044be80: ldr      r1, [sp, #0x1c]
0044be84: bl       #0x30eae4
0044be88: ldr      r3, [sp, #0x18]
0044be8c: cmp      r3, #0
0044be90: bne      #0x44c220
0044be94: ldr      ip, [sp, #0xc]
0044be98: cmp      ip, #0
0044be9c: moveq    sl, ip
0044bea0: bne      #0x44c2b0
0044bea4: ldr      r2, [sp, #0x14]
0044bea8: ldr      r7, [r2, #0xc]
0044beac: ldr      r8, [r7, #0x68]
0044beb0: cmp      r8, #0
0044beb4: beq      #0x44bec8
0044beb8: ldr      r0, [r7, #0x64]
0044bebc: ldrb     r3, [r0, #4]
0044bec0: cmp      r3, #0
0044bec4: beq      #0x44c288
0044bec8: mov      r1, #0
0044becc: mov      r0, #0x38
0044bed0: bl       #0x752ba8
0044bed4: mov      r1, r8
0044bed8: mov      r7, r0
0044bedc: bl       #0x76b820
0044bee0: ldr      r3, [r7]
0044bee4: add      fp, sp, #0x228
0044bee8: add      r8, sp, #0x90
0044beec: ldr      r3, [r3, #0x1c]
0044bef0: ldr      r1, [sp, #0x20]
0044bef4: mov      r0, fp
0044bef8: str      r3, [sp, #4]
0044befc: bl       #0x413a7c
0044bf00: mov      r2, #0
0044bf04: mov      r1, sb
0044bf08: mov      r0, r8
0044bf0c: strb     r2, [sp, #0x91]
0044bf10: strb     r2, [sp, #0x90]
0044bf14: bl       #0x797350
0044bf18: ldr      r3, [sp, #4]
0044bf1c: mov      r1, fp
0044bf20: mov      r2, r8
0044bf24: mov      r0, r7
0044bf28: blx      r3
0044bf2c: mov      r0, r8
0044bf30: bl       #0x797124
0044bf34: ldrb     ip, [sp, #0x228]
0044bf38: sxtb     r3, ip
0044bf3c: cmn      r3, #1
0044bf40: beq      #0x44c338
0044bf44: ldr      r3, [r7]
0044bf48: add      fp, sp, #0x214
0044bf4c: ldr      r1, [sp, #0x24]
0044bf50: mov      r0, fp
0044bf54: ldr      sb, [r3, #0x1c]
0044bf58: bl       #0x413a7c
0044bf5c: mov      r3, #0
0044bf60: ldr      r0, [r6, #8]
0044bf64: strb     r3, [sp, #0x84]
0044bf68: mov      r3, #2
0044bf6c: strb     r3, [sp, #0x85]
0044bf70: bl       #0x30ed30
0044bf74: mov      r3, #0x9a000000
0044bf78: asr      r3, r3, #0x16
0044bf7c: add      r2, sp, #0x240
0044bf80: strd     r0, r1, [r2, r3]
0044bf84: ldr      r3, [sp, #0xa8]
0044bf88: add      r8, sp, #0x84
0044bf8c: mov      r1, fp
0044bf90: str      r3, [sp, #0x88]
0044bf94: ldr      r3, [sp, #0xac]
0044bf98: mov      r2, r8
0044bf9c: mov      r0, r7
0044bfa0: str      r3, [r8, #8]
0044bfa4: blx      sb
0044bfa8: mov      r0, r8
0044bfac: bl       #0x797124
0044bfb0: ldrb     ip, [sp, #0x214]
0044bfb4: sxtb     r3, ip
0044bfb8: cmn      r3, #1
0044bfbc: beq      #0x44c328
0044bfc0: ldr      r3, [r7]
0044bfc4: add      fp, sp, #0x200
0044bfc8: mov      r0, fp
0044bfcc: ldr      r1, [sp, #0x28]
0044bfd0: ldr      sb, [r3, #0x1c]
0044bfd4: bl       #0x413a7c
0044bfd8: cmp      r5, #0
0044bfdc: beq      #0x44c1d8
0044bfe0: mov      r3, #1
0044bfe4: mov      r2, #0
0044bfe8: add      r8, sp, #0x78
0044bfec: strb     r2, [sp, #0x78]
0044bff0: mov      r2, #1
0044bff4: strb     r3, [sp, #0x7c]
0044bff8: strb     r2, [sp, #0x79]
0044bffc: mov      r1, fp
0044c000: mov      r2, r8
0044c004: mov      r0, r7
0044c008: blx      sb
0044c00c: mov      r0, r8
0044c010: bl       #0x797124
0044c014: ldrb     r2, [sp, #0x200]
0044c018: sxtb     r3, r2
0044c01c: cmn      r3, #1
0044c020: beq      #0x44c318
0044c024: ldr      ip, [sp, #0x2c]
0044c028: ldr      r3, [r7]
0044c02c: add      fp, sp, #0x1ec
0044c030: mov      r0, fp
0044c034: add      r1, pc, ip
0044c038: ldr      sb, [r3, #0x1c]
0044c03c: bl       #0x413a7c
0044c040: cmp      r5, #0
0044c044: beq      #0x44c1f4
0044c048: mov      r3, #0
0044c04c: mov      r2, #0
0044c050: add      r8, sp, #0x6c
0044c054: strb     r2, [sp, #0x6c]
0044c058: mov      r2, #1
0044c05c: strb     r3, [sp, #0x70]
0044c060: strb     r2, [sp, #0x6d]
0044c064: mov      r1, fp
0044c068: mov      r2, r8
0044c06c: mov      r0, r7
0044c070: blx      sb
0044c074: mov      r0, r8
0044c078: bl       #0x797124
0044c07c: ldrb     r2, [sp, #0x1ec]
0044c080: sxtb     r3, r2
0044c084: cmn      r3, #1
0044c088: beq      #0x44c368
0044c08c: ldr      r1, [pc, #0x38c]
0044c090: ldr      r3, [r7]
0044c094: add      fp, sp, #0x1d8
0044c098: cmp      r5, #0
0044c09c: add      r1, pc, r1
0044c0a0: mov      r0, fp
0044c0a4: movne    sl, #0
0044c0a8: ldr      sb, [r3, #0x1c]
0044c0ac: bl       #0x413a7c
0044c0b0: mov      r3, #0
0044c0b4: add      r8, sp, #0x60
0044c0b8: strb     r3, [sp, #0x60]
0044c0bc: mov      r3, #1
0044c0c0: strb     r3, [sp, #0x61]
0044c0c4: mov      r1, fp
0044c0c8: mov      r2, r8
0044c0cc: strb     sl, [sp, #0x64]
0044c0d0: mov      r0, r7
0044c0d4: blx      sb
0044c0d8: mov      r0, r8
0044c0dc: bl       #0x797124
0044c0e0: ldrb     ip, [sp, #0x1d8]
0044c0e4: sxtb     r3, ip
0044c0e8: cmn      r3, #1
0044c0ec: beq      #0x44c358
0044c0f0: cmp      r5, #0
0044c0f4: beq      #0x44c210
0044c0f8: ldr      r1, [pc, #0x324]
0044c0fc: add      r8, sp, #0xb4
0044c100: ldrsh    r2, [r4, #0x50]
0044c104: add      r1, pc, r1
0044c108: mov      r0, r8
0044c10c: bl       #0x30eae4
0044c110: ldr      r1, [pc, #0x310]
0044c114: ldr      r3, [r7]
0044c118: add      sb, sp, #0x1c4
0044c11c: add      r4, sp, #0x54
0044c120: add      r1, pc, r1
0044c124: mov      r0, sb
0044c128: ldr      sl, [r3, #0x1c]
0044c12c: bl       #0x413a7c
0044c130: mov      r3, #0
0044c134: mov      r1, r8
0044c138: mov      r0, r4
0044c13c: strb     r3, [sp, #0x55]
0044c140: strb     r3, [sp, #0x54]
0044c144: bl       #0x797350
0044c148: mov      r1, sb
0044c14c: mov      r2, r4
0044c150: mov      r0, r7
0044c154: blx      sl
0044c158: mov      r0, r4
0044c15c: bl       #0x797124
0044c160: ldrb     ip, [sp, #0x1c4]
0044c164: sxtb     r3, ip
0044c168: cmn      r3, #1
0044c16c: beq      #0x44c348
0044c170: mov      r3, #0
0044c174: mov      r0, r7
0044c178: add      r4, sp, #0x48
0044c17c: strb     r3, [sp, #0x48]
0044c180: mov      r3, #5
0044c184: strb     r3, [sp, #0x49]
0044c188: str      r7, [sp, #0x4c]
0044c18c: bl       #0x759c64
0044c190: mov      r1, r4
0044c194: ldr      r0, [sp, #0x10]
0044c198: bl       #0x799134
0044c19c: mov      r0, r4
0044c1a0: bl       #0x797124
0044c1a4: ldr      r3, [sp, #0xa0]
0044c1a8: add      r6, r6, #0xc
0044c1ac: cmp      r6, r3
0044c1b0: bne      #0x44be5c
0044c1b4: ldr      r7, [sp, #0x14]
0044c1b8: ldr      r4, [sp, #0x3c]
0044c1bc: ldr      r5, [sp, #0x44]
0044c1c0: ldr      r0, [r7]
0044c1c4: ldr      r1, [sp, #0x10]
0044c1c8: bl       #0x797250
0044c1cc: ldr      r0, [sp, #0x40]
0044c1d0: bl       #0x43f1e0
0044c1d4: b        #0x44bbc4
0044c1d8: mov      r0, r4
0044c1dc: ldr      r1, [sp, #0x38]
0044c1e0: bl       #0x3fa330
0044c1e4: cmp      r0, #0
0044c1e8: moveq    r3, r5
0044c1ec: beq      #0x44bfe4
0044c1f0: b        #0x44bfe0
0044c1f4: ldr      r0, [sp, #0x30]
0044c1f8: ldr      r1, [sp, #8]
0044c1fc: bl       #0x3ffe3c
0044c200: cmp      r4, r0
0044c204: moveq    r3, #1
0044c208: bne      #0x44c048
0044c20c: b        #0x44c04c
0044c210: add      r8, sp, #0xb4
0044c214: mov      r2, #0x20
0044c218: strh     r2, [r8]
0044c21c: b        #0x44c110
0044c220: ldr      r2, [sp, #0x3c]
0044c224: ldr      ip, [sp, #0x34]
0044c228: ldr      r7, [pc, #0x1fc]
0044c22c: ldr      r8, [pc, #0x1fc]
0044c230: ldr      sl, [r2, ip]
0044c234: add      r7, pc, r7
0044c238: add      r8, pc, r8
0044c23c: mov      r1, r7
0044c240: mov      r2, r8
0044c244: ldr      r0, [sl, #0x2c]
0044c248: bl       #0x4c4bdc
0044c24c: mov      r2, r8
0044c250: mov      fp, r0
0044c254: mov      r1, r7
0044c258: ldr      r0, [sl, #0x2c]
0044c25c: bl       #0x4c4bdc
0044c260: ldr      r3, [sp, #8]
0044c264: cmp      r3, r0
0044c268: beq      #0x44c3b8
0044c26c: mov      r1, fp
0044c270: ldr      r0, [sp, #0x30]
0044c274: bl       #0x3ffe3c
0044c278: cmp      r4, r0
0044c27c: movne    sl, #0
0044c280: moveq    sl, #1
0044c284: b        #0x44bea4
0044c288: ldr      r1, [r0]
0044c28c: sub      r1, r1, #1
0044c290: cmp      r1, #0
0044c294: str      r1, [r0]
0044c298: bne      #0x44c2a0
0044c29c: bl       #0x752b38
0044c2a0: mov      r8, #0
0044c2a4: str      r8, [r7, #0x68]
0044c2a8: str      r8, [r7, #0x64]
0044c2ac: b        #0x44bec8
0044c2b0: ldr      r3, [sp, #0x3c]
0044c2b4: ldr      r2, [sp, #0x34]
0044c2b8: ldr      r7, [pc, #0x174]
0044c2bc: ldr      r8, [pc, #0x174]
0044c2c0: ldr      sl, [r3, r2]
0044c2c4: add      r7, pc, r7
0044c2c8: add      r8, pc, r8
0044c2cc: mov      r1, r7
0044c2d0: mov      r2, r8
0044c2d4: ldr      r0, [sl, #0x2c]
0044c2d8: bl       #0x4c4bdc
0044c2dc: mov      r2, r8
0044c2e0: mov      fp, r0
0044c2e4: mov      r1, r7
0044c2e8: ldr      r0, [sl, #0x2c]
0044c2ec: bl       #0x4c4bdc
0044c2f0: ldr      ip, [sp, #8]
0044c2f4: cmp      ip, r0
0044c2f8: bne      #0x44c26c
0044c2fc: ldr      r2, [pc, #0x138]
0044c300: ldr      r0, [sl, #0x2c]
0044c304: mov      r1, r7
0044c308: add      r2, pc, r2
0044c30c: bl       #0x4c4bdc
0044c310: mov      fp, r0
0044c314: b        #0x44c26c
0044c318: ldr      r0, [sp, #0x20c]
0044c31c: ldr      r1, [sp, #0x208]
0044c320: bl       #0x752b38
0044c324: b        #0x44c024
0044c328: ldr      r0, [sp, #0x220]
0044c32c: ldr      r1, [sp, #0x21c]
0044c330: bl       #0x752b38
0044c334: b        #0x44bfc0
0044c338: ldr      r0, [sp, #0x234]
0044c33c: ldr      r1, [sp, #0x230]
0044c340: bl       #0x752b38
0044c344: b        #0x44bf44
0044c348: ldr      r0, [sp, #0x1d0]
0044c34c: ldr      r1, [sp, #0x1cc]
0044c350: bl       #0x752b38
0044c354: b        #0x44c170
0044c358: ldr      r0, [sp, #0x1e4]
0044c35c: ldr      r1, [sp, #0x1e0]
0044c360: bl       #0x752b38
0044c364: b        #0x44c0f0
0044c368: ldr      r0, [sp, #0x1f8]
0044c36c: ldr      r1, [sp, #0x1f4]
0044c370: bl       #0x752b38
0044c374: b        #0x44c08c
0044c378: mov      r3, #1
0044c37c: str      r3, [sp, #0xc]
0044c380: b        #0x44bdc0
0044c384: mov      ip, #1
0044c388: str      ip, [sp, #0x18]
0044c38c: b        #0x44bd68
0044c390: ldr      r3, [sp, #0x38]
0044c394: add      ip, sp, #0x9c
0044c398: ldr      r1, [sp, #8]
0044c39c: add      r3, r3, #0x37c
0044c3a0: mov      r0, r3
0044c3a4: mov      r2, ip
0044c3a8: str      r3, [sp, #0x30]
0044c3ac: str      ip, [sp, #0x40]
0044c3b0: bl       #0x3fed48
0044c3b4: b        #0x44bde8
0044c3b8: ldr      r2, [pc, #0x80]
0044c3bc: ldr      r0, [sl, #0x2c]
0044c3c0: mov      r1, r7
0044c3c4: add      r2, pc, r2
0044c3c8: bl       #0x4c4bdc
0044c3cc: mov      fp, r0
0044c3d0: b        #0x44c26c
0044c3d4: add      r0, sp, #0x9c
0044c3d8: bl       #0x43f1e0
0044c3dc: b        #0x44bbc4
0044c3e0: bl       #0x30e310
0044c3e4: subseq   r8, r4, ip, ror #29
0044c3e8: andeq    r4, r0, ip, lsr #1
0044c3ec: strdeq   r3, r4, [r0], -r4
0044c3f0: subeq    fp, r7, ip, lsr #12
0044c3f4: subeq    lr, r8, r0, lsl r8
0044c3f8: subeq    fp, r7, ip, lsl #12
0044c3fc: strdeq   fp, ip, [r7], #-0x58
0044c400: strheq   fp, [r7], #-0x50
0044c404: ldrdeq   fp, ip, [r7], #-0x58
0044c408: subeq    fp, r7, ip, asr #11
0044c40c: strheq   pc, [r7], #-0xe4
0044c410: subeq    pc, r7, r8, ror #27
0044c414: subeq    pc, r7, r4, lsl #28
0044c418: subeq    pc, r7, r8, lsl #28
0044c41c: subeq    r0, r8, r4, lsl #1
0044c420: subeq    r0, r8, ip, ror r8
0044c424: subeq    r5, r7, ip, lsr #27
0044c428: subeq    pc, r7, r8, ror #27
0044c42c: strdeq   fp, ip, [r7], #-4
0044c430: subeq    fp, r7, r0, lsl #2
0044c434: subeq    fp, r7, r4, rrx
0044c438: umaaleq  fp, r7, r0, r0
0044c43c: subeq    fp, r7, r8, rrx
0044c440: subeq    sl, r7, r4, lsl #31
# NativeInvGetPlayerGold _Z22NativeInvGetPlayerGoldRKN7gameswf7fn_callE
0044a5c4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044a5c8: ldr      r4, [pc, #0x238]
0044a5cc: ldr      r8, [pc, #0x238]
0044a5d0: ldr      r3, [r0, #0xc]
0044a5d4: add      r4, pc, r4
0044a5d8: ldr      r2, [r4, r8]
0044a5dc: sub      sp, sp, #0x74
0044a5e0: mov      r7, r0
0044a5e4: ldr      r2, [r2]
0044a5e8: ldr      r0, [r0, #0x14]
0044a5ec: mov      r6, #0xc
0044a5f0: str      r2, [sp, #0x6c]
0044a5f4: ldr      r3, [r3]
0044a5f8: mla      r0, r6, r0, r3
0044a5fc: bl       #0x797a54
0044a600: bl       #0x30ea24
0044a604: ldr      r3, [r7, #0x10]
0044a608: mov      sb, r0
0044a60c: cmp      r3, #2
0044a610: beq      #0x44a784
0044a614: cmp      r3, #3
0044a618: movne    r5, #0
0044a61c: movne    fp, r5
0044a620: beq      #0x44a7d0
0044a624: add      r6, sp, #0x2c
0044a628: mov      r0, r6
0044a62c: mov      r1, #0x10
0044a630: str      r6, [sp, #0x3c]
0044a634: str      r6, [sp, #0x40]
0044a638: bl       #0x31167c
0044a63c: ldr      r3, [sp, #0x3c]
0044a640: mov      sl, #0
0044a644: mov      r0, sb
0044a648: strb     sl, [r3]
0044a64c: mov      r1, fp
0044a650: bl       #0x43c388
0044a654: subs     sb, r0, #0
0044a658: beq      #0x44a760
0044a65c: ldr      r3, [pc, #0x1ac]
0044a660: ldr      r2, [pc, #0x1ac]
0044a664: mov      r1, r6
0044a668: ldr      r0, [r4, r3]
0044a66c: add      r2, pc, r2
0044a670: ldr      r3, [sb, #0x39c]
0044a674: ldr      r0, [r0, #0x34]
0044a678: bl       #0x508ef4
0044a67c: cmp      r5, sl
0044a680: beq      #0x44a7f4
0044a684: ldr      r3, [r5]
0044a688: ldr      r1, [pc, #0x188]
0044a68c: add      fp, sp, #0x58
0044a690: ldr      r3, [r3, #0x1c]
0044a694: add      r1, pc, r1
0044a698: mov      r0, fp
0044a69c: str      r3, [sp, #4]
0044a6a0: bl       #0x413a7c
0044a6a4: ldr      r0, [sb, #0x39c]
0044a6a8: mov      r2, #2
0044a6ac: strb     sl, [sp, #0x14]
0044a6b0: strb     r2, [sp, #0x15]
0044a6b4: bl       #0x30ed30
0044a6b8: strd     r0, r1, [sp, #0x20]
0044a6bc: ldr      r2, [sp, #0x20]
0044a6c0: add      sl, sp, #0x14
0044a6c4: mov      r1, fp
0044a6c8: str      r2, [sp, #0x18]
0044a6cc: ldr      r2, [sp, #0x24]
0044a6d0: mov      r0, r5
0044a6d4: str      r2, [sl, #8]
0044a6d8: ldr      r3, [sp, #4]
0044a6dc: mov      r2, sl
0044a6e0: blx      r3
0044a6e4: mov      r0, sl
0044a6e8: bl       #0x797124
0044a6ec: ldrsb    r3, [sp, #0x58]
0044a6f0: cmn      r3, #1
0044a6f4: beq      #0x44a7c0
0044a6f8: ldr      r1, [pc, #0x11c]
0044a6fc: ldr      r3, [r5]
0044a700: add      fp, sp, #0x44
0044a704: add      sl, sp, #8
0044a708: add      r1, pc, r1
0044a70c: mov      r0, fp
0044a710: ldr      sb, [r3, #0x1c]
0044a714: bl       #0x413a7c
0044a718: mov      r3, #0
0044a71c: ldr      r1, [sp, #0x40]
0044a720: mov      r0, sl
0044a724: strb     r3, [sp, #9]
0044a728: strb     r3, [sp, #8]
0044a72c: bl       #0x797350
0044a730: mov      r1, fp
0044a734: mov      r2, sl
0044a738: mov      r0, r5
0044a73c: blx      sb
0044a740: mov      r0, sl
0044a744: bl       #0x797124
0044a748: ldrsb    r3, [sp, #0x44]
0044a74c: cmn      r3, #1
0044a750: beq      #0x44a7b0
0044a754: ldr      r0, [r7]
0044a758: mov      r1, r5
0044a75c: bl       #0x797250
0044a760: mov      r0, r6
0044a764: bl       #0x3139ac
0044a768: ldr      r3, [r4, r8]
0044a76c: ldr      r2, [sp, #0x6c]
0044a770: ldr      r3, [r3]
0044a774: cmp      r2, r3
0044a778: bne      #0x44a804
0044a77c: add      sp, sp, #0x74
0044a780: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044a784: ldr      r3, [r7, #0xc]
0044a788: ldr      r2, [r7, #0x14]
0044a78c: ldr      r3, [r3]
0044a790: sub      r2, r2, #1
0044a794: mla      r6, r6, r2, r3
0044a798: ldrsb    r3, [r6, #1]
0044a79c: cmp      r3, #5
0044a7a0: ldreq    r5, [r6, #4]
0044a7a4: movne    r5, #0
0044a7a8: mov      fp, #0
0044a7ac: b        #0x44a624
0044a7b0: ldr      r0, [sp, #0x50]
0044a7b4: ldr      r1, [sp, #0x4c]
0044a7b8: bl       #0x752b38
0044a7bc: b        #0x44a754
0044a7c0: ldr      r0, [sp, #0x64]
0044a7c4: ldr      r1, [sp, #0x60]
0044a7c8: bl       #0x752b38
0044a7cc: b        #0x44a6f8
0044a7d0: ldr      r3, [r7, #0xc]
0044a7d4: ldr      r0, [r7, #0x14]
0044a7d8: mov      r5, #0
0044a7dc: ldr      r3, [r3]
0044a7e0: sub      r0, r0, #2
0044a7e4: mla      r0, r6, r0, r3
0044a7e8: bl       #0x797960
0044a7ec: mov      fp, r0
0044a7f0: b        #0x44a624
0044a7f4: ldr      r0, [r7]
0044a7f8: ldr      r1, [sp, #0x40]
0044a7fc: bl       #0x797350
0044a800: b        #0x44a760
0044a804: bl       #0x30e310
0044a808: ldrheq   sl, [r4], #-0x4c
0044a80c: andeq    r4, r0, ip, lsr #1
0044a810: strdeq   r3, r4, [r0], -r4
0044a814: subeq    lr, r7, ip, asr #19
0044a818: subeq    r2, r8, ip, lsl r1
0044a81c: strheq   r2, [r8], #-0
# NativeInvTransmuteItem _Z22NativeInvTransmuteItemRKN7gameswf7fn_callE
0043f23c: push     {r4, r5, r6, r7, r8, sb, lr}
0043f240: ldr      r3, [r0, #0x10]
0043f244: sub      sp, sp, #0x24
0043f248: mov      r4, r0
0043f24c: cmp      r3, #2
0043f250: beq      #0x43f25c
0043f254: add      sp, sp, #0x24
0043f258: pop      {r4, r5, r6, r7, r8, sb, pc}
0043f25c: ldr      r7, [r0, #0xc]
0043f260: ldr      r6, [r0, #0x14]
0043f264: mov      r5, #0xc
0043f268: ldr      r3, [r7]
0043f26c: mla      r3, r5, r6, r3
0043f270: ldrsb    r2, [r3, #1]
0043f274: cmp      r2, #2
0043f278: bne      #0x43f254
0043f27c: ldr      r2, [r3, #8]
0043f280: ldr      r3, [r3, #4]
0043f284: str      r2, [sp, #0x1c]
0043f288: str      r3, [sp, #0x18]
0043f28c: ldrd     r8, sb, [sp, #0x18]
0043f290: mov      r0, r8
0043f294: mov      r2, r8
0043f298: mov      r1, sb
0043f29c: mov      r3, sb
0043f2a0: strd     r8, sb, [sp, #0x10]
0043f2a4: bl       #0x30e2bc
0043f2a8: subs     r8, r0, #0
0043f2ac: bne      #0x43f254
0043f2b0: ldr      r3, [r7]
0043f2b4: sub      r0, r6, #1
0043f2b8: mla      r0, r5, r0, r3
0043f2bc: bl       #0x439d8c
0043f2c0: cmp      r0, #0
0043f2c4: beq      #0x43f254
0043f2c8: ldr      r3, [r4, #0xc]
0043f2cc: ldr      r0, [r4, #0x14]
0043f2d0: ldr      r3, [r3]
0043f2d4: mla      r0, r5, r0, r3
0043f2d8: bl       #0x43a1b8
0043f2dc: ldr      r3, [r4, #0xc]
0043f2e0: mov      r6, r0
0043f2e4: ldr      r0, [r4, #0x14]
0043f2e8: ldr      r3, [r3]
0043f2ec: sub      r0, r0, #1
0043f2f0: mla      r0, r5, r0, r3
0043f2f4: bl       #0x43a1b8
0043f2f8: mov      r1, r8
0043f2fc: str      r8, [sp, #4]
0043f300: str      r8, [sp, #8]
0043f304: str      r8, [sp, #0xc]
0043f308: bl       #0x43c388
0043f30c: subs     r5, r0, #0
0043f310: beq      #0x43f344
0043f314: mov      r1, r6
0043f318: mov      r2, r8
0043f31c: bl       #0x3a4bec
0043f320: mov      r0, r5
0043f324: bl       #0x3a999c
0043f328: ldr      r4, [r4]
0043f32c: mov      r0, r4
0043f330: bl       #0x797124
0043f334: strb     r8, [r4, #1]
0043f338: add      r0, sp, #4
0043f33c: bl       #0x43f1e0
0043f340: b        #0x43f254
0043f344: add      r0, sp, #4
0043f348: bl       #0x43f1e0
0043f34c: b        #0x43f254
# NativeInvUnequipItem _Z20NativeInvUnequipItemRKN7gameswf7fn_callE
0043ef64: push     {r4, r5, r6, r7, r8, sb, lr}
0043ef68: ldr      r3, [r0, #0x10]
0043ef6c: sub      sp, sp, #0x14
0043ef70: mov      r4, r0
0043ef74: cmp      r3, #2
0043ef78: beq      #0x43ef84
0043ef7c: add      sp, sp, #0x14
0043ef80: pop      {r4, r5, r6, r7, r8, sb, pc}
0043ef84: ldr      r7, [r0, #0xc]
0043ef88: ldr      r6, [r0, #0x14]
0043ef8c: mov      r5, #0xc
0043ef90: ldr      r3, [r7]
0043ef94: mla      r3, r5, r6, r3
0043ef98: ldrsb    r2, [r3, #1]
0043ef9c: cmp      r2, #2
0043efa0: bne      #0x43ef7c
0043efa4: ldr      r2, [r3, #8]
0043efa8: ldr      r3, [r3, #4]
0043efac: str      r2, [sp, #0xc]
0043efb0: str      r3, [sp, #8]
0043efb4: ldrd     r8, sb, [sp, #8]
0043efb8: mov      r0, r8
0043efbc: mov      r2, r8
0043efc0: mov      r1, sb
0043efc4: mov      r3, sb
0043efc8: strd     r8, sb, [sp]
0043efcc: bl       #0x30e2bc
0043efd0: subs     r8, r0, #0
0043efd4: bne      #0x43ef7c
0043efd8: ldr      r3, [r7]
0043efdc: sub      r0, r6, #1
0043efe0: mla      r0, r5, r0, r3
0043efe4: bl       #0x439d8c
0043efe8: cmp      r0, #0
0043efec: beq      #0x43ef7c
0043eff0: ldr      r3, [r4, #0xc]
0043eff4: ldr      r0, [r4, #0x14]
0043eff8: ldr      r3, [r3]
0043effc: mla      r0, r5, r0, r3
0043f000: bl       #0x43a1b8
0043f004: ldr      r3, [r4, #0xc]
0043f008: mov      r6, r0
0043f00c: ldr      r0, [r4, #0x14]
0043f010: ldr      r3, [r3]
0043f014: sub      r0, r0, #1
0043f018: mla      r0, r5, r0, r3
0043f01c: bl       #0x43a1b8
0043f020: mov      r1, r8
0043f024: bl       #0x43c388
0043f028: subs     r3, r0, #0
0043f02c: beq      #0x43ef7c
0043f030: ldr      r3, [r3]
0043f034: mov      r1, r6
0043f038: mov      lr, pc
0043f03c: ldr      pc, [r3, #0x144]
0043f040: b        #0x43ef7c
# NativeSaveGame _Z14NativeSaveGameRKN7gameswf7fn_callE
0043dc08: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0043dc0c: ldr      r3, [r0, #0x10]
0043dc10: ldr      r7, [pc, #0x1f0]
0043dc14: sub      sp, sp, #0x14
0043dc18: cmp      r3, #1
0043dc1c: add      r7, pc, r7
0043dc20: movne    r0, #0
0043dc24: beq      #0x43dde0
0043dc28: mov      r1, #0
0043dc2c: bl       #0x43c388
0043dc30: subs     r5, r0, #0
0043dc34: beq      #0x43dd64
0043dc38: bl       #0x3bc4a8
0043dc3c: ldr      r2, [pc, #0x1c8]
0043dc40: movw     r6, #0x14e8
0043dc44: ldr      r3, [r5, r6]
0043dc48: ldr      r2, [r7, r2]
0043dc4c: cmp      r3, #0
0043dc50: ldr      r2, [r2]
0043dc54: str      r2, [sp, #8]
0043dc58: beq      #0x43de00
0043dc5c: ldr      r3, [r3, #0x84]
0043dc60: cmp      r3, #0
0043dc64: beq      #0x43de00
0043dc68: ldr      r3, [pc, #0x1a0]
0043dc6c: ldr      fp, [pc, #0x1a0]
0043dc70: mov      sl, #0
0043dc74: add      r3, pc, r3
0043dc78: str      r3, [sp, #4]
0043dc7c: ldr      r3, [pc, #0x194]
0043dc80: ldr      sb, [pc, #0x194]
0043dc84: add      fp, pc, fp
0043dc88: add      r3, pc, r3
0043dc8c: str      r3, [sp, #0xc]
0043dc90: mov      r4, sl
0043dc94: mov      r8, #1
0043dc98: b        #0x43dcbc
0043dc9c: ldr      r3, [r5, r6]
0043dca0: add      r4, r4, #1
0043dca4: mov      sl, r4
0043dca8: cmp      r3, #0
0043dcac: beq      #0x43dd28
0043dcb0: ldr      r3, [r3, #0x84]
0043dcb4: cmp      r3, r4
0043dcb8: bls      #0x43dd28
0043dcbc: mov      r1, sl
0043dcc0: mov      r0, r5
0043dcc4: bl       #0x3bbed0
0043dcc8: cmp      r0, #0
0043dccc: moveq    r8, r0
0043dcd0: beq      #0x43dc9c
0043dcd4: mov      r1, sl
0043dcd8: mov      r0, r5
0043dcdc: bl       #0x3bbed0
0043dce0: ldr      r3, [r7, sb]
0043dce4: mov      sl, r0
0043dce8: ldr      r2, [sp, #4]
0043dcec: mov      r1, fp
0043dcf0: ldr      r0, [r3, #0x2c]
0043dcf4: bl       #0x4c4bdc
0043dcf8: cmp      sl, r0
0043dcfc: bne      #0x43dc9c
0043dd00: ldr      r0, [sp, #0xc]
0043dd04: bl       #0x3a3f70
0043dd08: mov      r1, r0
0043dd0c: ldr      r0, [sp, #8]
0043dd10: bl       #0x3813b8
0043dd14: ldr      r3, [r5, r6]
0043dd18: add      r4, r4, #1
0043dd1c: mov      sl, r4
0043dd20: cmp      r3, #0
0043dd24: bne      #0x43dcb0
0043dd28: ldr      r3, [r5]
0043dd2c: mov      r0, r5
0043dd30: mov      lr, pc
0043dd34: ldr      pc, [r3, #0x28]
0043dd38: cmp      r0, #0
0043dd3c: beq      #0x43dd64
0043dd40: cmp      r8, #0
0043dd44: bne      #0x43ddc4
0043dd48: add      r5, r5, #0x560
0043dd4c: mov      r2, #0
0043dd50: mov      r0, r5
0043dd54: mov      r1, #0x9d
0043dd58: bl       #0x3df6e0
0043dd5c: subs     r2, r0, #0
0043dd60: beq      #0x43dd6c
0043dd64: add      sp, sp, #0x14
0043dd68: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0043dd6c: mov      r1, #0x13
0043dd70: mov      r0, r5
0043dd74: bl       #0x3df6e0
0043dd78: ldr      r3, [pc, #0x9c]
0043dd7c: ldr      r1, [pc, #0x9c]
0043dd80: ldr      r2, [pc, #0x9c]
0043dd84: ldr      r3, [r7, r3]
0043dd88: mov      r4, r0
0043dd8c: add      r1, pc, r1
0043dd90: add      r2, pc, r2
0043dd94: ldr      r0, [r3, #0x2c]
0043dd98: bl       #0x4c4bdc
0043dd9c: cmp      r4, r0
0043dda0: bne      #0x43dd64
0043dda4: ldr      r0, [pc, #0x7c]
0043dda8: add      r0, pc, r0
0043ddac: bl       #0x3a3f70
0043ddb0: mov      r1, r0
0043ddb4: ldr      r0, [sp, #8]
0043ddb8: add      sp, sp, #0x14
0043ddbc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0043ddc0: b        #0x3813b8
0043ddc4: ldr      r0, [pc, #0x60]
0043ddc8: add      r0, pc, r0
0043ddcc: bl       #0x3a3f70
0043ddd0: mov      r1, r0
0043ddd4: ldr      r0, [sp, #8]
0043ddd8: bl       #0x3813b8
0043dddc: b        #0x43dd48
0043dde0: ldr      r3, [r0, #0xc]
0043dde4: ldr      r2, [r0, #0x14]
0043dde8: mov      r0, #0xc
0043ddec: ldr      r3, [r3]
0043ddf0: mla      r0, r0, r2, r3
0043ddf4: bl       #0x797a54
0043ddf8: bl       #0x30ea24
0043ddfc: b        #0x43dc28
0043de00: mov      r8, #1
0043de04: b        #0x43dd28
0043de08: subseq   r6, r5, r4, ror lr
0043de0c: andeq    r1, r0, r0, ror sp
0043de10: subeq    r6, r8, r4, ror #24
0043de14: subeq    r3, r8, ip, asr #21
0043de18: subeq    sp, r8, r8, ror #31
0043de1c: strdeq   r3, r4, [r0], -r4
0043de20: subeq    r3, r8, r4, asr #19
0043de24: umaaleq  r6, r8, r8, r7
0043de28: strdeq   sp, lr, [r8], #-0xe8
0043de2c: strheq   sp, [r8], #-0xe8
# NativeSkillGetEquipedSkillsIDs _Z30NativeSkillGetEquipedSkillsIDsRKN7gameswf7fn_callE
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
# NativeSkillsGetSkillPointsLeft _Z30NativeSkillsGetSkillPointsLeftRKN7gameswf7fn_callE
0043ec98: push     {r4, r5, r6, r7, r8, sb, lr}
0043ec9c: ldr      r3, [r0, #0x10]
0043eca0: sub      sp, sp, #0x14
0043eca4: mov      r4, r0
0043eca8: cmp      r3, #1
0043ecac: beq      #0x43ecb8
0043ecb0: add      sp, sp, #0x14
0043ecb4: pop      {r4, r5, r6, r7, r8, sb, pc}
0043ecb8: ldr      r2, [r0, #0x14]
0043ecbc: ldr      r6, [r0, #0xc]
0043ecc0: mov      r5, #0xc
0043ecc4: mul      r5, r5, r2
0043ecc8: ldr      r3, [r6]
0043eccc: add      r3, r3, r5
0043ecd0: ldrsb    r2, [r3, #1]
0043ecd4: cmp      r2, #2
0043ecd8: bne      #0x43ecb0
0043ecdc: ldr      r2, [r3, #8]
0043ece0: ldr      r3, [r3, #4]
0043ece4: str      r2, [sp, #0xc]
0043ece8: str      r3, [sp, #8]
0043ecec: ldrd     r8, sb, [sp, #8]
0043ecf0: mov      r0, r8
0043ecf4: mov      r1, sb
0043ecf8: mov      r2, r8
0043ecfc: mov      r3, sb
0043ed00: strd     r8, sb, [sp]
0043ed04: bl       #0x30e2bc
0043ed08: subs     r7, r0, #0
0043ed0c: bne      #0x43ecb0
0043ed10: ldr      r0, [r6]
0043ed14: add      r0, r0, r5
0043ed18: bl       #0x43a1b8
0043ed1c: mov      r1, r7
0043ed20: bl       #0x43c388
0043ed24: cmp      r0, #0
0043ed28: beq      #0x43ecb0
0043ed2c: mov      r2, r7
0043ed30: mov      r1, #0x9d
0043ed34: add      r0, r0, #0x560
0043ed38: bl       #0x3df6e0
0043ed3c: bl       #0x30ed30
0043ed40: ldr      r4, [r4]
0043ed44: mov      r2, r0
0043ed48: mov      r3, r1
0043ed4c: mov      r0, r4
0043ed50: bl       #0x797488
0043ed54: b        #0x43ecb0
# NativeSkillsTrainSkill _Z22NativeSkillsTrainSkillRKN7gameswf7fn_callE
0043d548: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0043d54c: ldr      r3, [r0, #0xc]
0043d550: mov      r4, r0
0043d554: ldr      r0, [r0, #0x14]
0043d558: ldr      r3, [r3]
0043d55c: mov      r5, #0xc
0043d560: mla      r0, r5, r0, r3
0043d564: bl       #0x797a54
0043d568: ldr      r3, [r4, #0xc]
0043d56c: mov      r8, r0
0043d570: ldr      r0, [r4, #0x14]
0043d574: ldr      r3, [r3]
0043d578: mov      sb, r1
0043d57c: sub      r0, r0, #1
0043d580: mla      r0, r5, r0, r3
0043d584: bl       #0x797a54
0043d588: bl       #0x30ea24
0043d58c: ldr      r3, [r4, #0x10]
0043d590: mov      r7, r0
0043d594: cmp      r3, #3
0043d598: movne    r6, #0
0043d59c: beq      #0x43d61c
0043d5a0: mov      r0, r7
0043d5a4: mov      r1, #0
0043d5a8: bl       #0x43c388
0043d5ac: subs     r5, r0, #0
0043d5b0: beq      #0x43d618
0043d5b4: mov      r1, sb
0043d5b8: mov      r0, r8
0043d5bc: bl       #0x30ea24
0043d5c0: mov      r2, r6
0043d5c4: mov      r1, r0
0043d5c8: mov      r0, r5
0043d5cc: bl       #0x3bcc58
0043d5d0: mov      r1, #0x9d
0043d5d4: mov      r7, r0
0043d5d8: mov      r2, #0
0043d5dc: add      r0, r5, #0x560
0043d5e0: bl       #0x3df6e0
0043d5e4: cmp      r6, #0
0043d5e8: bne      #0x43d608
0043d5ec: bl       #0x30ed30
0043d5f0: ldr      r4, [r4]
0043d5f4: mov      r2, r0
0043d5f8: mov      r3, r1
0043d5fc: mov      r0, r4
0043d600: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0043d604: b        #0x797488
0043d608: ldr      r0, [r4]
0043d60c: mov      r1, r7
0043d610: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
0043d614: b        #0x797230
0043d618: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0043d61c: ldr      r3, [r4, #0xc]
0043d620: ldr      r0, [r4, #0x14]
0043d624: ldr      r3, [r3]
0043d628: sub      r0, r0, #2
0043d62c: mla      r0, r5, r0, r3
0043d630: bl       #0x797960
0043d634: mov      r6, r0
0043d638: b        #0x43d5a0
# NativeStatsAssignPoint _Z22NativeStatsAssignPointRKN7gameswf7fn_callE
0043eb7c: push     {r4, r5, r6, r7, r8, sb, lr}
0043eb80: ldr      r3, [r0, #0x10]
0043eb84: sub      sp, sp, #0x14
0043eb88: mov      r4, r0
0043eb8c: cmp      r3, #2
0043eb90: beq      #0x43eb9c
0043eb94: add      sp, sp, #0x14
0043eb98: pop      {r4, r5, r6, r7, r8, sb, pc}
0043eb9c: ldr      r7, [r0, #0xc]
0043eba0: ldr      r6, [r0, #0x14]
0043eba4: mov      r5, #0xc
0043eba8: ldr      r3, [r7]
0043ebac: mla      r3, r5, r6, r3
0043ebb0: ldrsb    r2, [r3, #1]
0043ebb4: cmp      r2, #2
0043ebb8: bne      #0x43eb94
0043ebbc: ldr      r2, [r3, #8]
0043ebc0: ldr      r3, [r3, #4]
0043ebc4: str      r2, [sp, #0xc]
0043ebc8: str      r3, [sp, #8]
0043ebcc: ldrd     r8, sb, [sp, #8]
0043ebd0: mov      r0, r8
0043ebd4: mov      r2, r8
0043ebd8: mov      r1, sb
0043ebdc: mov      r3, sb
0043ebe0: strd     r8, sb, [sp]
0043ebe4: bl       #0x30e2bc
0043ebe8: subs     r8, r0, #0
0043ebec: bne      #0x43eb94
0043ebf0: ldr      r3, [r7]
0043ebf4: sub      r0, r6, #1
0043ebf8: mla      r0, r5, r0, r3
0043ebfc: bl       #0x439d8c
0043ec00: cmp      r0, #0
0043ec04: beq      #0x43eb94
0043ec08: ldr      r3, [r4, #0xc]
0043ec0c: ldr      r0, [r4, #0x14]
0043ec10: ldr      r3, [r3]
0043ec14: mla      r0, r5, r0, r3
0043ec18: bl       #0x43a1b8
0043ec1c: ldr      r3, [r4, #0xc]
0043ec20: mov      r6, r0
0043ec24: ldr      r0, [r4, #0x14]
0043ec28: ldr      r3, [r3]
0043ec2c: sub      r0, r0, #1
0043ec30: mla      r0, r5, r0, r3
0043ec34: bl       #0x43a1b8
0043ec38: mov      r1, r8
0043ec3c: bl       #0x43c388
0043ec40: cmp      r0, #0
0043ec44: beq      #0x43eb94
0043ec48: cmp      r6, #3
0043ec4c: addls    pc, pc, r6, lsl #2
0043ec50: b        #0x43ec68
0043ec54: b        #0x43ec64
0043ec58: b        #0x43ec90
0043ec5c: b        #0x43ec88
0043ec60: b        #0x43ec80
0043ec64: bl       #0x3bd810
0043ec68: ldr      r4, [r4]
0043ec6c: mov      r0, r4
0043ec70: bl       #0x797124
0043ec74: mov      r3, #0
0043ec78: strb     r3, [r4, #1]
0043ec7c: b        #0x43eb94
0043ec80: bl       #0x3bd4f8
0043ec84: b        #0x43ec68
0043ec88: bl       #0x3bd600
0043ec8c: b        #0x43ec68
0043ec90: bl       #0x3bd708
0043ec94: b        #0x43ec68
# NativeSwapEquipment _Z19NativeSwapEquipmentRKN7gameswf7fn_callE
00442198: push     {r4, r5, r6, r7, lr}
0044219c: ldr      r3, [r0, #0xc]
004421a0: ldr      r2, [r0, #0x14]
004421a4: mov      r0, #0xc
004421a8: ldr      r3, [r3]
004421ac: sub      sp, sp, #0x24
004421b0: ldr      r5, [pc, #0xf0]
004421b4: mla      r0, r0, r2, r3
004421b8: bl       #0x797a54
004421bc: bl       #0x30ea24
004421c0: mov      r1, #0
004421c4: bl       #0x43c388
004421c8: subs     r4, r0, #0
004421cc: add      r5, pc, r5
004421d0: beq      #0x4421fc
004421d4: add      r0, r4, #0x37c
004421d8: bl       #0x3fc6c8
004421dc: add      r0, r4, #0x560
004421e0: bl       #0x3e08a8
004421e4: mov      r0, r4
004421e8: bl       #0x3a9d10
004421ec: mov      r0, r4
004421f0: bl       #0x3a999c
004421f4: mov      r0, r4
004421f8: bl       #0x3bd140
004421fc: ldr      r3, [pc, #0xa8]
00442200: mov      r6, #0
00442204: add      r4, sp, #0xc
00442208: ldr      r3, [r5, r3]
0044220c: ldr      r0, [r3, #0x54]
00442210: bl       #0x42cb8c
00442214: mov      r5, r0
00442218: bl       #0x7a7cac
0044221c: bl       #0x774154
00442220: ldr      r2, [pc, #0x88]
00442224: mov      r1, r0
00442228: mov      r3, r6
0044222c: add      r2, pc, r2
00442230: mov      r0, r5
00442234: str      r6, [sp]
00442238: bl       #0x7abe0c
0044223c: mov      r0, r5
00442240: bl       #0x7a7cac
00442244: bl       #0x774154
00442248: mov      r7, r0
0044224c: bl       #0x42ca8c
00442250: mov      r3, #2
00442254: ldr      r0, [r0, #0x108]
00442258: strb     r3, [sp, #0xd]
0044225c: strb     r6, [sp, #0xc]
00442260: bl       #0x30ed30
00442264: strd     r0, r1, [sp, #0x18]
00442268: ldr      ip, [sp, #0x18]
0044226c: ldr      r2, [pc, #0x40]
00442270: mov      r0, r5
00442274: str      ip, [sp, #0x10]
00442278: ldr      ip, [sp, #0x1c]
0044227c: mov      r1, r7
00442280: add      r2, pc, r2
00442284: str      ip, [r4, #8]
00442288: mov      r3, r4
0044228c: mov      ip, #1
00442290: str      ip, [sp]
00442294: bl       #0x7abe0c
00442298: mov      r0, r4
0044229c: bl       #0x797124
004422a0: add      sp, sp, #0x24
004422a4: pop      {r4, r5, r6, r7, pc}
004422a8: subseq   r2, r5, r4, asr #17
004422ac: strdeq   r3, r4, [r0], -r4
004422b0: subeq    sb, r8, r4, lsl #24
004422b4: strdeq   r7, r8, [r8], #-0xe8
# _ZN9Character15EquipItemToSlotEjj _ZN9Character15EquipItemToSlotEjj
003a9f30: push     {r4, lr}
003a9f34: mov      r4, r0
003a9f38: add      r0, r0, #0x37c
003a9f3c: bl       #0x4009f0
003a9f40: add      r0, r4, #0x560
003a9f44: bl       #0x3e08a8
003a9f48: mov      r0, r4
003a9f4c: bl       #0x3a9d10
003a9f50: mov      r0, r4
003a9f54: bl       #0x3a999c
003a9f58: mov      r0, r4
003a9f5c: pop      {r4, lr}
003a9f60: b        #0x3bd140
# _ZN9Character19UnEquipItemFromSlotEj _ZN9Character19UnEquipItemFromSlotEj
003a9eb0: push     {r4, lr}
003a9eb4: mvn      r2, #0
003a9eb8: mov      r4, r0
003a9ebc: add      r0, r0, #0x37c
003a9ec0: bl       #0x40050c
003a9ec4: add      r0, r4, #0x560
003a9ec8: bl       #0x3e08a8
003a9ecc: mov      r0, r4
003a9ed0: bl       #0x3a9d10
003a9ed4: mov      r0, r4
003a9ed8: bl       #0x3a999c
003a9edc: mov      r0, r4
003a9ee0: pop      {r4, lr}
003a9ee4: b        #0x3bd140
# _ZN9Character10IncStatStrEv _ZN9Character10IncStatStrEv
003bd810: push     {r4, r5, r6, r7, lr}
003bd814: ldr      r4, [pc, #0xe8]
003bd818: ldr      r6, [pc, #0xe8]
003bd81c: add      r5, r0, #0x560
003bd820: add      r4, pc, r4
003bd824: ldr      r3, [r4, r6]
003bd828: sub      sp, sp, #0x44
003bd82c: mov      r7, r0
003bd830: ldr      r3, [r3]
003bd834: mov      r0, r5
003bd838: mov      r1, #0x94
003bd83c: mov      r2, #0
003bd840: str      r3, [sp, #0x3c]
003bd844: bl       #0x3df6e0
003bd848: cmp      r0, #0
003bd84c: ble      #0x3bd8d8
003bd850: mov      r0, r5
003bd854: mov      r1, #0x94
003bd858: mvn      r2, #0
003bd85c: bl       #0x3e0798
003bd860: mov      r2, #1
003bd864: mov      r0, r5
003bd868: mov      r1, #0x95
003bd86c: bl       #0x3e0798
003bd870: movw     r3, #0x13c8
003bd874: ldrsh    r1, [r7, r3]
003bd878: mov      r0, r5
003bd87c: bl       #0x3e087c
003bd880: ldr      r3, [pc, #0x84]
003bd884: add      r5, sp, #0xc
003bd888: ldr      r7, [r4, r3]
003bd88c: mov      r0, r7
003bd890: bl       #0x337888
003bd894: ldr      r1, [pc, #0x74]
003bd898: add      r2, sp, #4
003bd89c: mov      r0, r5
003bd8a0: add      r1, pc, r1
003bd8a4: bl       #0x3140ec
003bd8a8: mov      r1, r5
003bd8ac: mov      r0, r7
003bd8b0: bl       #0x337a88
003bd8b4: mov      r0, r5
003bd8b8: bl       #0x318254
003bd8bc: ldr      r3, [r4, r6]
003bd8c0: ldr      r2, [sp, #0x3c]
003bd8c4: ldr      r3, [r3]
003bd8c8: cmp      r2, r3
003bd8cc: bne      #0x3bd900
003bd8d0: add      sp, sp, #0x44
003bd8d4: pop      {r4, r5, r6, r7, pc}
003bd8d8: ldr      r3, [pc, #0x2c]
003bd8dc: add      r5, sp, #0x24
003bd8e0: ldr      r7, [r4, r3]
003bd8e4: mov      r0, r7
003bd8e8: bl       #0x337888
003bd8ec: ldr      r1, [pc, #0x20]
003bd8f0: add      r2, sp, #8
003bd8f4: mov      r0, r5
003bd8f8: add      r1, pc, r1
003bd8fc: b        #0x3bd8a4
003bd900: bl       #0x30e310
003bd904: subseq   r7, sp, r0, ror r2
003bd908: andeq    r4, r0, ip, lsr #1
003bd90c: andeq    r0, r0, r4, lsl #17
003bd910: ldrsheq  r6, [r0], #-0xf0
# _ZN9Character10IncStatDexEv _ZN9Character10IncStatDexEv
003bd708: push     {r4, r5, r6, r7, lr}
003bd70c: ldr      r4, [pc, #0xe8]
003bd710: ldr      r6, [pc, #0xe8]
003bd714: add      r5, r0, #0x560
003bd718: add      r4, pc, r4
003bd71c: ldr      r3, [r4, r6]
003bd720: sub      sp, sp, #0x44
003bd724: mov      r7, r0
003bd728: ldr      r3, [r3]
003bd72c: mov      r0, r5
003bd730: mov      r1, #0x94
003bd734: mov      r2, #0
003bd738: str      r3, [sp, #0x3c]
003bd73c: bl       #0x3df6e0
003bd740: cmp      r0, #0
003bd744: ble      #0x3bd7d0
003bd748: mov      r0, r5
003bd74c: mov      r1, #0x94
003bd750: mvn      r2, #0
003bd754: bl       #0x3e0798
003bd758: mov      r2, #1
003bd75c: mov      r0, r5
003bd760: mov      r1, #0x96
003bd764: bl       #0x3e0798
003bd768: movw     r3, #0x13c8
003bd76c: ldrsh    r1, [r7, r3]
003bd770: mov      r0, r5
003bd774: bl       #0x3e087c
003bd778: ldr      r3, [pc, #0x84]
003bd77c: add      r5, sp, #0xc
003bd780: ldr      r7, [r4, r3]
003bd784: mov      r0, r7
003bd788: bl       #0x337888
003bd78c: ldr      r1, [pc, #0x74]
003bd790: add      r2, sp, #4
003bd794: mov      r0, r5
003bd798: add      r1, pc, r1
003bd79c: bl       #0x3140ec
003bd7a0: mov      r1, r5
003bd7a4: mov      r0, r7
003bd7a8: bl       #0x337a88
003bd7ac: mov      r0, r5
003bd7b0: bl       #0x318254
003bd7b4: ldr      r3, [r4, r6]
003bd7b8: ldr      r2, [sp, #0x3c]
003bd7bc: ldr      r3, [r3]
003bd7c0: cmp      r2, r3
003bd7c4: bne      #0x3bd7f8
003bd7c8: add      sp, sp, #0x44
003bd7cc: pop      {r4, r5, r6, r7, pc}
003bd7d0: ldr      r3, [pc, #0x2c]
003bd7d4: add      r5, sp, #0x24
003bd7d8: ldr      r7, [r4, r3]
003bd7dc: mov      r0, r7
003bd7e0: bl       #0x337888
003bd7e4: ldr      r1, [pc, #0x20]
003bd7e8: add      r2, sp, #8
003bd7ec: mov      r0, r5
003bd7f0: add      r1, pc, r1
003bd7f4: b        #0x3bd79c
003bd7f8: bl       #0x30e310
003bd7fc: subseq   r7, sp, r8, ror r3
003bd800: andeq    r4, r0, ip, lsr #1
003bd804: andeq    r0, r0, r4, lsl #17
003bd808: ldrsheq  r7, [r0], #-8
003bd80c: subseq   r7, r0, r0, lsr #1
# _ZN9Character10IncStatEndEv _ZN9Character10IncStatEndEv
003bd600: push     {r4, r5, r6, r7, lr}
003bd604: ldr      r4, [pc, #0xe8]
003bd608: ldr      r6, [pc, #0xe8]
003bd60c: add      r5, r0, #0x560
003bd610: add      r4, pc, r4
003bd614: ldr      r3, [r4, r6]
003bd618: sub      sp, sp, #0x44
003bd61c: mov      r7, r0
003bd620: ldr      r3, [r3]
003bd624: mov      r0, r5
003bd628: mov      r1, #0x94
003bd62c: mov      r2, #0
003bd630: str      r3, [sp, #0x3c]
003bd634: bl       #0x3df6e0
003bd638: cmp      r0, #0
003bd63c: ble      #0x3bd6c8
003bd640: mov      r0, r5
003bd644: mov      r1, #0x94
003bd648: mvn      r2, #0
003bd64c: bl       #0x3e0798
003bd650: mov      r2, #1
003bd654: mov      r0, r5
003bd658: mov      r1, #0x97
003bd65c: bl       #0x3e0798
003bd660: movw     r3, #0x13c8
003bd664: ldrsh    r1, [r7, r3]
003bd668: mov      r0, r5
003bd66c: bl       #0x3e087c
003bd670: ldr      r3, [pc, #0x84]
003bd674: add      r5, sp, #0xc
003bd678: ldr      r7, [r4, r3]
003bd67c: mov      r0, r7
003bd680: bl       #0x337888
003bd684: ldr      r1, [pc, #0x74]
003bd688: add      r2, sp, #4
003bd68c: mov      r0, r5
003bd690: add      r1, pc, r1
003bd694: bl       #0x3140ec
003bd698: mov      r1, r5
003bd69c: mov      r0, r7
003bd6a0: bl       #0x337a88
003bd6a4: mov      r0, r5
003bd6a8: bl       #0x318254
003bd6ac: ldr      r3, [r4, r6]
003bd6b0: ldr      r2, [sp, #0x3c]
003bd6b4: ldr      r3, [r3]
003bd6b8: cmp      r2, r3
003bd6bc: bne      #0x3bd6f0
003bd6c0: add      sp, sp, #0x44
003bd6c4: pop      {r4, r5, r6, r7, pc}
003bd6c8: ldr      r3, [pc, #0x2c]
003bd6cc: add      r5, sp, #0x24
003bd6d0: ldr      r7, [r4, r3]
003bd6d4: mov      r0, r7
003bd6d8: bl       #0x337888
003bd6dc: ldr      r1, [pc, #0x20]
003bd6e0: add      r2, sp, #8
003bd6e4: mov      r0, r5
003bd6e8: add      r1, pc, r1
003bd6ec: b        #0x3bd694
003bd6f0: bl       #0x30e310
003bd6f4: subseq   r7, sp, r0, lsl #9
003bd6f8: andeq    r4, r0, ip, lsr #1
003bd6fc: andeq    r0, r0, r4, lsl #17
003bd700: subseq   r7, r0, r0, lsl #4
003bd704: subseq   r7, r0, r8, lsr #3
# _ZN9Character10IncStatNrgEv _ZN9Character10IncStatNrgEv
003bd4f8: push     {r4, r5, r6, r7, lr}
003bd4fc: ldr      r4, [pc, #0xe8]
003bd500: ldr      r6, [pc, #0xe8]
003bd504: add      r5, r0, #0x560
003bd508: add      r4, pc, r4
003bd50c: ldr      r3, [r4, r6]
003bd510: sub      sp, sp, #0x44
003bd514: mov      r7, r0
003bd518: ldr      r3, [r3]
003bd51c: mov      r0, r5
003bd520: mov      r1, #0x94
003bd524: mov      r2, #0
003bd528: str      r3, [sp, #0x3c]
003bd52c: bl       #0x3df6e0
003bd530: cmp      r0, #0
003bd534: ble      #0x3bd5c0
003bd538: mov      r0, r5
003bd53c: mov      r1, #0x94
003bd540: mvn      r2, #0
003bd544: bl       #0x3e0798
003bd548: mov      r2, #1
003bd54c: mov      r0, r5
003bd550: mov      r1, #0x98
003bd554: bl       #0x3e0798
003bd558: movw     r3, #0x13c8
003bd55c: ldrsh    r1, [r7, r3]
003bd560: mov      r0, r5
003bd564: bl       #0x3e087c
003bd568: ldr      r3, [pc, #0x84]
003bd56c: add      r5, sp, #0xc
003bd570: ldr      r7, [r4, r3]
003bd574: mov      r0, r7
003bd578: bl       #0x337888
003bd57c: ldr      r1, [pc, #0x74]
003bd580: add      r2, sp, #4
003bd584: mov      r0, r5
003bd588: add      r1, pc, r1
003bd58c: bl       #0x3140ec
003bd590: mov      r1, r5
003bd594: mov      r0, r7
003bd598: bl       #0x337a88
003bd59c: mov      r0, r5
003bd5a0: bl       #0x318254
003bd5a4: ldr      r3, [r4, r6]
003bd5a8: ldr      r2, [sp, #0x3c]
003bd5ac: ldr      r3, [r3]
003bd5b0: cmp      r2, r3
003bd5b4: bne      #0x3bd5e8
003bd5b8: add      sp, sp, #0x44
003bd5bc: pop      {r4, r5, r6, r7, pc}
003bd5c0: ldr      r3, [pc, #0x2c]
003bd5c4: add      r5, sp, #0x24
003bd5c8: ldr      r7, [r4, r3]
003bd5cc: mov      r0, r7
003bd5d0: bl       #0x337888
003bd5d4: ldr      r1, [pc, #0x20]
003bd5d8: add      r2, sp, #8
003bd5dc: mov      r0, r5
003bd5e0: add      r1, pc, r1
003bd5e4: b        #0x3bd58c
003bd5e8: bl       #0x30e310
003bd5ec: subseq   r7, sp, r8, lsl #11
003bd5f0: andeq    r4, r0, ip, lsr #1
003bd5f4: andeq    r0, r0, r4, lsl #17
003bd5f8: subseq   r7, r0, r8, lsl #6
003bd5fc: ldrheq   r7, [r0], #-0x20
# _ZN14CharProperties20UpdateBasePropertiesEi _ZN14CharProperties20UpdateBasePropertiesEi
003e087c: push     {r4, r5, r6, lr}
003e0880: mov      r4, r0
003e0884: mov      r5, r1
003e0888: bl       #0x3defbc
003e088c: mov      r0, r4
003e0890: mov      r1, r5
003e0894: bl       #0x3df2a4
003e0898: mov      r0, r4
003e089c: mov      r1, #1
003e08a0: pop      {r4, r5, r6, lr}
003e08a4: b        #0x3e0810
# _ZN14CharProperties19ResetBasePropertiesEv _ZN14CharProperties19ResetBasePropertiesEv
003defbc: add      r1, r0, #8
003defc0: b        #0x3def34
# _ZN14CharProperties18LoadBasePropertiesEi _ZN14CharProperties18LoadBasePropertiesEi
003df2a4: mov      r2, r1
003df2a8: add      r1, r0, #8
003df2ac: b        #0x3df250
# _ZN14CharProperties16RecalcPropertiesEb _ZN14CharProperties16RecalcPropertiesEb
003e0810: cmp      r1, #0
003e0814: push     {r4, r5, r6, lr}
003e0818: mov      r4, r0
003e081c: bne      #0x3e0840
003e0820: mov      r5, #0
003e0824: mov      r1, r5
003e0828: mov      r0, r4
003e082c: add      r5, r5, #1
003e0830: bl       #0x3dfe60
003e0834: cmp      r5, #0xe0
003e0838: bne      #0x3e0824
003e083c: pop      {r4, r5, r6, pc}
003e0840: add      r1, r0, #8
003e0844: ldr      r2, [r0, #0x74]
003e0848: mov      r3, #0
003e084c: bl       #0x3e2e20
003e0850: b        #0x3e0820
# _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
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
# _ZN14CharProperties18_LoadFromCharTableERN7Structs19CharacterPropertiesEi _ZN14CharProperties18_LoadFromCharTableERN7Structs19CharacterPropertiesEi
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
# _ZN14CharProperties10_LoadClassERN7Structs19CharacterPropertiesEib _ZN14CharProperties10_LoadClassERN7Structs19CharacterPropertiesEib
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
# _ZNK9Character17IsSkillEquippableEi _ZNK9Character17IsSkillEquippableEi
003bca84: push     {r4, r5, r6, lr}
003bca88: mov      r5, r0
003bca8c: mov      r4, r1
003bca90: bl       #0x3bca50
003bca94: cmp      r0, #0
003bca98: bne      #0x3bcaa4
003bca9c: mov      r0, #0
003bcaa0: pop      {r4, r5, r6, pc}
003bcaa4: mov      r0, r5
003bcaa8: mov      r1, r4
003bcaac: bl       #0x3bc784
003bcab0: ldr      r3, [r0, #0x48]
003bcab4: cmn      r3, #1
003bcab8: beq      #0x3bca9c
003bcabc: mov      r0, r5
003bcac0: mov      r1, r4
003bcac4: bl       #0x3bbed0
003bcac8: cmp      r0, #0
003bcacc: movle    r0, #0
003bcad0: movgt    r0, #1
003bcad4: pop      {r4, r5, r6, pc}
# _ZNK9Character16IsSkillAvailableEi _ZNK9Character16IsSkillAvailableEi
003bca50: push     {r4, r5, r6, lr}
003bca54: mov      r5, r1
003bca58: mov      r6, r0
003bca5c: bl       #0x3bd120
003bca60: mov      r1, r5
003bca64: mov      r4, r0
003bca68: mov      r0, r6
003bca6c: bl       #0x3bc784
003bca70: ldr      r0, [r0, #0x20]
003bca74: cmp      r4, r0
003bca78: movlt    r0, #0
003bca7c: movge    r0, #1
003bca80: pop      {r4, r5, r6, pc}
# _ZNK9Character12GetCharSkillEi _ZNK9Character12GetCharSkillEi
003bc784: push     {r4, r5, r6, lr}
003bc788: sub      sp, sp, #8
003bc78c: mov      r5, r1
003bc790: bl       #0x3bc5c0
003bc794: ldr      r4, [pc, #0xa4]
003bc798: ldr      r3, [pc, #0xa4]
003bc79c: mov      r6, #0xc
003bc7a0: add      r4, pc, r4
003bc7a4: ldr      r3, [r4, r3]
003bc7a8: cmp      r5, #0
003bc7ac: ldr      r3, [r3]
003bc7b0: mla      r6, r6, r0, r3
003bc7b4: blt      #0x3bc7c4
003bc7b8: ldr      r3, [r6, #4]
003bc7bc: cmp      r5, r3
003bc7c0: blt      #0x3bc7e8
003bc7c4: ldr      r3, [pc, #0x7c]
003bc7c8: ldr      r3, [r4, r3]
003bc7cc: ldr      r3, [r3]
003bc7d0: cmp      r3, #2
003bc7d4: moveq    r3, #0
003bc7d8: streq    r3, [r3]
003bc7dc: beq      #0x3bc7e8
003bc7e0: cmp      r3, #1
003bc7e4: beq      #0x3bc80c
003bc7e8: ldr      r3, [pc, #0x5c]
003bc7ec: ldr      r2, [r6, #8]
003bc7f0: mov      r0, #0x4c
003bc7f4: ldr      r3, [r4, r3]
003bc7f8: ldr      r2, [r2, r5, lsl #2]
003bc7fc: ldr      r3, [r3]
003bc800: mla      r0, r0, r2, r3
003bc804: add      sp, sp, #8
003bc808: pop      {r4, r5, r6, pc}
003bc80c: ldr      r0, [pc, #0x3c]
003bc810: ldr      r1, [pc, #0x3c]
003bc814: ldr      r2, [pc, #0x3c]
003bc818: ldr      r0, [r4, r0]
003bc81c: ldr      r3, [pc, #0x38]
003bc820: mov      ip, #0x3d
003bc824: add      r1, pc, r1
003bc828: add      r2, pc, r2
003bc82c: add      r3, pc, r3
003bc830: add      r0, r0, #0xa8
003bc834: str      ip, [sp]
003bc838: bl       #0x30e004
003bc83c: b        #0x3bc7e8
003bc840: ldrsheq  r8, [sp], #-0x20
003bc844: andeq    r1, r0, r8, asr #3
003bc848: andeq    r3, r0, r0, asr #19
003bc84c: andeq    r4, r0, ip, lsl r4
003bc850: andeq    r1, r0, r0, asr #19
003bc854: ldrheq   r1, [r0], #-0xb4
003bc858: subseq   r7, r0, r0, lsr #31
003bc85c: ldrsbeq  r7, [r0], #-0xf4
# _ZNK9Character18GetCharSkillListIdEv _ZNK9Character18GetCharSkillListIdEv
003bc5c0: movw     r3, #0x1068
003bc5c4: ldr      r0, [r0, r3]
003bc5c8: ldr      r3, [pc, #0x24]
003bc5cc: cmp      r0, #0
003bc5d0: add      r3, pc, r3
003bc5d4: blt      #0x3bc5ec
003bc5d8: ldr      r2, [pc, #0x18]
003bc5dc: ldr      r3, [r3, r2]
003bc5e0: ldr      r3, [r3]
003bc5e4: cmp      r0, r3
003bc5e8: bxlt     lr
003bc5ec: mov      r0, #3
003bc5f0: bx       lr
003bc5f4: subseq   r8, sp, r0, asr #9
003bc5f8: andeq    r2, r0, r8, ror sp
# _ZNK14CharProperties20PROPS_GetBonusDamageEb _ZNK14CharProperties20PROPS_GetBonusDamageEb
003df8ac: push     {r4, r5, r6, r7, r8, lr}
003df8b0: mov      r4, r0
003df8b4: ldr      r0, [r0, #4]
003df8b8: cmp      r1, #0
003df8bc: movne    r1, #2
003df8c0: moveq    r1, #1
003df8c4: add      r0, r0, #0x37c
003df8c8: bl       #0x3ffe3c
003df8cc: cmp      r0, #0
003df8d0: beq      #0x3df938
003df8d4: bl       #0x3f9e08
003df8d8: ldr      r2, [r0, #0x94]
003df8dc: cmn      r2, #1
003df8e0: beq      #0x3df938
003df8e4: add      r5, r4, #0xa90
003df8e8: add      r5, r5, #4
003df8ec: add      r2, r2, #0x53
003df8f0: mov      r1, r5
003df8f4: mov      r0, r4
003df8f8: bl       #0x3dedb4
003df8fc: mov      r7, r0
003df900: ldr      r0, [r4, #4]
003df904: mov      r1, #1
003df908: add      r0, r0, #0x37c
003df90c: bl       #0x4001a0
003df910: cmp      r0, #0
003df914: bne      #0x3df940
003df918: ldr      r3, [r4, #4]
003df91c: add      r6, r0, r7
003df920: add      r0, r3, #0x37c
003df924: bl       #0x40019c
003df928: cmp      r0, #0
003df92c: bne      #0x3df968
003df930: add      r0, r6, r0
003df934: pop      {r4, r5, r6, r7, r8, pc}
003df938: mov      r0, #0
003df93c: pop      {r4, r5, r6, r7, r8, pc}
003df940: mov      r1, r5
003df944: mov      r2, #0x5b
003df948: mov      r0, r4
003df94c: bl       #0x3dedb4
003df950: ldr      r3, [r4, #4]
003df954: add      r6, r0, r7
003df958: add      r0, r3, #0x37c
003df95c: bl       #0x40019c
003df960: cmp      r0, #0
003df964: beq      #0x3df930
003df968: mov      r0, r4
003df96c: mov      r1, r5
003df970: mov      r2, #0x5a
003df974: bl       #0x3dedb4
003df978: add      r0, r6, r0
003df97c: pop      {r4, r5, r6, r7, r8, pc}
# _ZNK14CharProperties26PROPS_GetBonusAttackRatingEb _ZNK14CharProperties26PROPS_GetBonusAttackRatingEb
003df81c: push     {r4, r5, r6, lr}
003df820: mov      r4, r0
003df824: ldr      r0, [r0, #4]
003df828: cmp      r1, #0
003df82c: movne    r1, #2
003df830: moveq    r1, #1
003df834: add      r0, r0, #0x37c
003df838: bl       #0x3ffe3c
003df83c: cmp      r0, #0
003df840: beq      #0x3df88c
003df844: bl       #0x3f9e08
003df848: ldr      r2, [r0, #0x94]
003df84c: cmn      r2, #1
003df850: beq      #0x3df88c
003df854: add      r5, r4, #0xa90
003df858: add      r5, r5, #4
003df85c: add      r2, r2, #0x33
003df860: mov      r1, r5
003df864: mov      r0, r4
003df868: bl       #0x3dedb4
003df86c: mov      r6, r0
003df870: ldr      r0, [r4, #4]
003df874: add      r0, r0, #0x37c
003df878: bl       #0x40019c
003df87c: cmp      r0, #0
003df880: bne      #0x3df894
003df884: add      r0, r0, r6
003df888: pop      {r4, r5, r6, pc}
003df88c: mov      r0, #0
003df890: pop      {r4, r5, r6, pc}
003df894: mov      r0, r4
003df898: mov      r1, r5
003df89c: mov      r2, #0x3a
003df8a0: bl       #0x3dedb4
003df8a4: add      r0, r0, r6
003df8a8: pop      {r4, r5, r6, pc}
# _ZNK14CharProperties24PROPS_GetBonusCritRatingEb _ZNK14CharProperties24PROPS_GetBonusCritRatingEb
003df7c4: push     {r4, lr}
003df7c8: mov      r4, r0
003df7cc: ldr      r0, [r0, #4]
003df7d0: cmp      r1, #0
003df7d4: movne    r1, #2
003df7d8: moveq    r1, #1
003df7dc: add      r0, r0, #0x37c
003df7e0: bl       #0x3ffe3c
003df7e4: cmp      r0, #0
003df7e8: beq      #0x3df814
003df7ec: bl       #0x3f9e08
003df7f0: ldr      r2, [r0, #0x94]
003df7f4: cmn      r2, #1
003df7f8: beq      #0x3df814
003df7fc: add      r1, r4, #0xa90
003df800: mov      r0, r4
003df804: add      r1, r1, #4
003df808: add      r2, r2, #0x40
003df80c: pop      {r4, lr}
003df810: b        #0x3dedb4
003df814: mov      r0, #0
003df818: pop      {r4, pc}
