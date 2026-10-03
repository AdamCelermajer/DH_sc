
# _ZN11POCharacterC2EP13PhysicalWorldP10GameObjectbsttb.clone.2
003b4010: push     {r4, r5, r6, r7, lr}
003b4014: sub      sp, sp, #0x24
003b4018: ldrh     r5, [sp, #0x38]
003b401c: ldrh     lr, [sp, #0x3c]
003b4020: ldrb     r6, [sp, #0x40]
003b4024: mov      ip, #0
003b4028: str      r3, [sp, #0xc]
003b402c: mov      r7, #1
003b4030: mov      r3, ip
003b4034: ldr      r4, [pc, #0x44]
003b4038: str      r5, [sp, #0x10]
003b403c: str      lr, [sp, #0x14]
003b4040: mov      r5, r0
003b4044: str      ip, [sp, #4]
003b4048: str      ip, [sp, #0x18]
003b404c: str      r7, [sp]
003b4050: str      r6, [sp, #8]
003b4054: bl       #0x46f2f0
003b4058: ldr      r3, [pc, #0x24]
003b405c: add      r4, pc, r4
003b4060: mov      r0, r5
003b4064: ldr      r3, [r4, r3]
003b4068: add      r3, r3, #8
003b406c: str      r3, [r5]
003b4070: bl       #0x46eb20
003b4074: mov      r0, r5
003b4078: add      sp, sp, #0x24
003b407c: pop      {r4, r5, r6, r7, pc}
003b4080: subseq   r0, lr, r4, lsr sl
003b4084: andeq    r1, r0, ip, lsr #4

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

# _ZN16CharStateMachine15RaiseStateEventEiPv
003c5684: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c5688: ldr      r5, [pc, #0x1e0]
003c568c: ldr      r7, [pc, #0x1e0]
003c5690: mov      r6, r1
003c5694: add      r5, pc, r5
003c5698: ldr      r1, [r5, r7]
003c569c: sub      sp, sp, #0x30
003c56a0: sub      r3, r6, #0x2a
003c56a4: ldr      r1, [r1]
003c56a8: mov      r4, r0
003c56ac: mov      r8, r2
003c56b0: str      r1, [sp, #0x2c]
003c56b4: cmp      r3, #6
003c56b8: addls    pc, pc, r3, lsl #2
003c56bc: b        #0x3c5700
003c56c0: b        #0x3c584c
003c56c4: b        #0x3c583c
003c56c8: b        #0x3c582c
003c56cc: b        #0x3c5700
003c56d0: b        #0x3c5700
003c56d4: b        #0x3c5700
003c56d8: b        #0x3c56dc
003c56dc: mov      r1, #0
003c56e0: bl       #0x3c0260
003c56e4: cmp      r0, #0
003c56e8: beq      #0x3c5700
003c56ec: ldr      r3, [r4, #4]
003c56f0: ldr      r0, [r3, #0x2dc]
003c56f4: cmp      r0, #0
003c56f8: beq      #0x3c5700
003c56fc: bl       #0x46eb20
003c5700: ldr      r3, [r4, #0x20]
003c5704: cmp      r3, #0
003c5708: beq      #0x3c574c
003c570c: ldm      r3, {r1, r3}
003c5710: ldr      r2, [r4, #4]
003c5714: ldr      ip, [r3]
003c5718: mov      r0, r3
003c571c: str      r6, [sp]
003c5720: mov      r3, r4
003c5724: str      r8, [sp, #4]
003c5728: mov      lr, pc
003c572c: ldr      pc, [ip, #0x18]
003c5730: ldr      r3, [r4, #0x20]
003c5734: mov      r0, r4
003c5738: mov      r2, r6
003c573c: ldr      r1, [r3]
003c5740: bl       #0x3c00e0
003c5744: cmp      r0, #0
003c5748: bne      #0x3c5768
003c574c: ldr      r3, [r5, r7]
003c5750: ldr      r2, [sp, #0x2c]
003c5754: ldr      r3, [r3]
003c5758: cmp      r2, r3
003c575c: bne      #0x3c586c
003c5760: add      sp, sp, #0x30
003c5764: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c5768: ldr      r3, [pc, #0x108]
003c576c: add      sl, sp, #0x14
003c5770: ldr      sb, [r5, r3]
003c5774: mov      r0, sb
003c5778: bl       #0x337888
003c577c: ldr      r1, [pc, #0xf8]
003c5780: add      r2, sp, #0x10
003c5784: mov      r0, sl
003c5788: add      r1, pc, r1
003c578c: bl       #0x3140ec
003c5790: mov      r1, sl
003c5794: mov      r0, sb
003c5798: bl       #0x337a88
003c579c: mov      r0, sl
003c57a0: bl       #0x318254
003c57a4: ldr      r3, [r4, #0x20]
003c57a8: mov      r0, r4
003c57ac: mov      r2, r6
003c57b0: ldr      r1, [r3]
003c57b4: bl       #0x3c1694
003c57b8: ldr      r1, [r0, #8]
003c57bc: str      r1, [sp, #0xc]
003c57c0: ldr      r3, [r0]
003c57c4: cmp      r3, #0
003c57c8: beq      #0x3c585c
003c57cc: ldr      r3, [r0, #4]
003c57d0: ldr      r2, [r4, #4]
003c57d4: tst      r3, #1
003c57d8: ldrne    r1, [r0]
003c57dc: ldrne    ip, [r2, r3, asr #1]
003c57e0: addne    r0, r2, r3, asr #1
003c57e4: ldreq    ip, [r0]
003c57e8: addeq    r0, r2, r3, asr #1
003c57ec: ldr      r3, [r4, #0x20]
003c57f0: add      r2, sp, #0xc
003c57f4: ldrne    ip, [ip, r1]
003c57f8: ldr      r3, [r3]
003c57fc: mov      r1, r6
003c5800: str      r2, [sp]
003c5804: mov      r2, r8
003c5808: blx      ip
003c580c: cmp      r0, #0
003c5810: beq      #0x3c574c
003c5814: ldr      r1, [sp, #0xc]
003c5818: mov      r0, r4
003c581c: mov      r2, r6
003c5820: mov      r3, r8
003c5824: bl       #0x3c1938
003c5828: b        #0x3c574c
003c582c: ldr      r3, [r0, #0x2c]
003c5830: bic      r3, r3, #4
003c5834: str      r3, [r0, #0x2c]
003c5838: b        #0x3c5700
003c583c: ldr      r3, [r0, #0x2c]
003c5840: bic      r3, r3, #2
003c5844: str      r3, [r0, #0x2c]
003c5848: b        #0x3c5700
003c584c: ldr      r3, [r0, #0x2c]
003c5850: bic      r3, r3, #1
003c5854: str      r3, [r0, #0x2c]
003c5858: b        #0x3c5700
003c585c: ldr      r3, [r0, #4]
003c5860: tst      r3, #1
003c5864: beq      #0x3c5818
003c5868: b        #0x3c57cc
003c586c: bl       #0x30e310
003c5870: ldrsheq  pc, [ip], #-0x3c
003c5874: andeq    r4, r0, ip, lsr #1
003c5878: andeq    r0, r0, r4, lsl #17
003c587c: subeq    pc, pc, r8, lsr #15

# _ZN6CSMove6OnBlurEiP9CharacterP16CharStateMachinei
003c3aa4: push     {r4, r5, r6, r7, r8, lr}
003c3aa8: ldr      r4, [pc, #0x8c]
003c3aac: ldr      r6, [pc, #0x8c]
003c3ab0: ldr      r1, [pc, #0x8c]
003c3ab4: add      r4, pc, r4
003c3ab8: ldr      r3, [r4, r6]
003c3abc: ldr      r8, [r4, r1]
003c3ac0: sub      sp, sp, #0x20
003c3ac4: ldr      r3, [r3]
003c3ac8: mov      r0, r8
003c3acc: mov      r7, r2
003c3ad0: str      r3, [sp, #0x1c]
003c3ad4: bl       #0x337888
003c3ad8: ldr      r1, [pc, #0x68]
003c3adc: add      r5, sp, #4
003c3ae0: mov      r2, sp
003c3ae4: add      r1, pc, r1
003c3ae8: mov      r0, r5
003c3aec: bl       #0x3140ec
003c3af0: mov      r1, r5
003c3af4: mov      r0, r8
003c3af8: bl       #0x337a88
003c3afc: mov      r0, r5
003c3b00: bl       #0x318254
003c3b04: mov      r0, r7
003c3b08: bl       #0x3938f8
003c3b0c: ldr      r0, [r7, #0x2dc]
003c3b10: cmp      r0, #0
003c3b14: beq      #0x3c3b1c
003c3b18: bl       #0x46eb20
003c3b1c: ldr      r3, [r4, r6]
003c3b20: ldr      r2, [sp, #0x1c]
003c3b24: ldr      r3, [r3]
003c3b28: cmp      r2, r3
003c3b2c: bne      #0x3c3b38
003c3b30: add      sp, sp, #0x20
003c3b34: pop      {r4, r5, r6, r7, r8, pc}
003c3b38: bl       #0x30e310
003c3b3c: ldrsbeq  r0, [sp], #-0xfc
003c3b40: andeq    r4, r0, ip, lsr #1
003c3b44: andeq    r0, r0, r4, lsl #17
003c3b48: subseq   r1, r0, ip, ror #6

# _ZN6CSMove7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3bf8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c3bfc: ldr      r4, [pc, #0xac]
003c3c00: ldr      r6, [pc, #0xac]
003c3c04: ldr      ip, [pc, #0xac]
003c3c08: add      r4, pc, r4
003c3c0c: ldr      r1, [r4, r6]
003c3c10: ldr      r7, [r4, ip]
003c3c14: sub      sp, sp, #0x20
003c3c18: ldr      r1, [r1]
003c3c1c: mov      sb, r0
003c3c20: mov      r0, r7
003c3c24: mov      r5, r2
003c3c28: mov      sl, r3
003c3c2c: str      r1, [sp, #0x1c]
003c3c30: bl       #0x337888
003c3c34: ldr      r1, [pc, #0x80]
003c3c38: add      r8, sp, #4
003c3c3c: mov      r2, sp
003c3c40: add      r1, pc, r1
003c3c44: mov      r0, r8
003c3c48: bl       #0x3140ec
003c3c4c: mov      r1, r8
003c3c50: mov      r0, r7
003c3c54: bl       #0x337a88
003c3c58: mov      r0, r8
003c3c5c: bl       #0x318254
003c3c60: movw     r3, #0x23c1
003c3c64: str      r3, [r5, #0x520]
003c3c68: mov      r3, #0
003c3c6c: mov      r0, sb
003c3c70: str      r3, [r5, #0x53c]
003c3c74: mov      r2, sl
003c3c78: mov      r1, r5
003c3c7c: bl       #0x3c0f18
003c3c80: ldr      r0, [r5, #0x2dc]
003c3c84: cmp      r0, #0
003c3c88: beq      #0x3c3c90
003c3c8c: bl       #0x46eae0
003c3c90: ldr      r3, [r4, r6]
003c3c94: ldr      r2, [sp, #0x1c]
003c3c98: ldr      r3, [r3]
003c3c9c: cmp      r2, r3
003c3ca0: bne      #0x3c3cac
003c3ca4: add      sp, sp, #0x20
003c3ca8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c3cac: bl       #0x30e310
003c3cb0: subseq   r0, sp, r8, lsl #29
003c3cb4: andeq    r4, r0, ip, lsr #1
003c3cb8: andeq    r0, r0, r4, lsl #17
003c3cbc: subseq   r1, r0, r0, lsl r2
