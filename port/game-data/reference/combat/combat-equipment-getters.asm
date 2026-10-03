
# _ZN13ItemInventory15GetEquippedItemEj
003ffe3c: push     {r4, r5, r6, lr}
003ffe40: ldr      r3, [r0, #0x14]
003ffe44: mov      r5, r0
003ffe48: mov      r4, r1
003ffe4c: ldm      r3, {r2, r3}
003ffe50: rsb      r3, r2, r3
003ffe54: cmp      r1, r3, asr #2
003ffe58: blo      #0x3ffe64
003ffe5c: mov      r0, #0
003ffe60: pop      {r4, r5, r6, pc}
003ffe64: bl       #0x3fc6a8
003ffe68: mov      r3, #0xc
003ffe6c: mul      r3, r3, r0
003ffe70: ldr      r2, [r5, #0x14]
003ffe74: ldr      r3, [r2, r3]
003ffe78: ldr      r3, [r3, r4, lsl #2]
003ffe7c: cmp      r3, #0
003ffe80: beq      #0x3ffe5c
003ffe84: ldr      r0, [r3]
003ffe88: pop      {r4, r5, r6, pc}

# _ZNK13ItemInventory9HasShieldEv
00400110: push     {r4, lr}
00400114: mov      r1, #2
00400118: mov      r4, r0
0040011c: bl       #0x3fc6a8
00400120: mov      r3, #0xc
00400124: mul      r3, r3, r0
00400128: ldr      r2, [r4, #0x14]
0040012c: ldr      r3, [r2, r3]
00400130: ldr      r0, [r3, #8]
00400134: cmp      r0, #0
00400138: beq      #0x400154
0040013c: ldr      r0, [r0]
00400140: bl       #0x3f9e08
00400144: ldr      r0, [r0, #0x58]
00400148: cmp      r0, #6
0040014c: movne    r0, #0
00400150: moveq    r0, #1
00400154: pop      {r4, pc}

# _ZNK13ItemInventory14IsDualWieldingEv
0040019c: b        #0x400158

# _ZNK13ItemInventory12HasTwoHanderEb
004001a0: push     {r4, r5, r6, lr}
004001a4: mov      r5, r1
004001a8: mov      r1, #1
004001ac: mov      r4, r0
004001b0: bl       #0x3fc6a8
004001b4: mov      r3, #0xc
004001b8: mul      r3, r3, r0
004001bc: ldr      r2, [r4, #0x14]
004001c0: ldr      r3, [r2, r3]
004001c4: ldr      r0, [r3, #4]
004001c8: cmp      r0, #0
004001cc: beq      #0x400218
004001d0: ldr      r0, [r0]
004001d4: bl       #0x3f9e08
004001d8: ldr      r3, [r0, #0x58]
004001dc: ldr      r2, [r4, #4]
004001e0: ldr      r0, [r0, #0x68]
004001e4: sub      r3, r3, #4
004001e8: cmp      r3, #1
004001ec: bls      #0x40021c
004001f0: cmp      r5, #0
004001f4: bne      #0x40021c
004001f8: cmn      r0, #4
004001fc: beq      #0x400208
00400200: mov      r0, r5
00400204: pop      {r4, r5, r6, pc}
00400208: movw     r3, #0x1324
0040020c: ldr      r0, [r2, r3]
00400210: rsbs     r0, r0, #1
00400214: movlo    r0, #0
00400218: pop      {r4, r5, r6, pc}
0040021c: cmn      r0, #4
00400220: movne    r0, #0
00400224: moveq    r0, #1
00400228: pop      {r4, r5, r6, pc}

# _ZNK12ItemInstance7GetItemEv
003f9e08: ldr      r3, [pc, #0x1c]
003f9e0c: ldr      r1, [pc, #0x1c]
003f9e10: ldr      r2, [r0, #4]
003f9e14: add      r3, pc, r3
003f9e18: ldr      r1, [r3, r1]
003f9e1c: mov      r0, #0xa4
003f9e20: ldr      r3, [r1]
003f9e24: mla      r0, r0, r2, r3
003f9e28: bx       lr
003f9e2c: subseq   sl, sb, ip, ror ip
003f9e30: andeq    r2, r0, ip, ror #16
