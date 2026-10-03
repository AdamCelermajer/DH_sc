
# _ZNK9Character14CanRangeAttackEv
003a4d3c: movw     r3, #0x1078
003a4d40: ldr      r3, [r0, r3]
003a4d44: cmn      r3, #1
003a4d48: beq      #0x3a4d54
003a4d4c: mov      r0, #1
003a4d50: bx       lr
003a4d54: add      r0, r0, #0x37c
003a4d58: b        #0x400014

# _ZNK13ItemInventory15HasRangedWeaponEv
003fffa4: push     {r4, r5, r6, lr}
003fffa8: mov      r1, #1
003fffac: mov      r4, r0
003fffb0: bl       #0x3fc6a8
003fffb4: mov      r5, #0xc
003fffb8: mul      r5, r5, r0
003fffbc: ldr      r3, [r4, #0x14]
003fffc0: ldr      r3, [r3, r5]
003fffc4: ldr      r0, [r3, #4]
003fffc8: cmp      r0, #0
003fffcc: beq      #0x400008
003fffd0: ldr      r0, [r0]
003fffd4: bl       #0x3f9e08
003fffd8: ldr      r3, [r0, #0x58]
003fffdc: cmp      r3, #4
003fffe0: beq      #0x40000c
003fffe4: ldr      r3, [r4, #0x14]
003fffe8: ldr      r3, [r3, r5]
003fffec: ldr      r3, [r3, #4]
003ffff0: ldr      r0, [r3]
003ffff4: bl       #0x3f9e08
003ffff8: ldr      r0, [r0, #0x58]
003ffffc: cmp      r0, #5
00400000: movne    r0, #0
00400004: moveq    r0, #1
00400008: pop      {r4, r5, r6, pc}
0040000c: mov      r0, #1
00400010: pop      {r4, r5, r6, pc}

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

# _ZNK9Character13GetAnimStanceEv
003a53e0: push     {r4, r5, r6, lr}
003a53e4: ldr      r3, [r0]
003a53e8: mov      r4, r0
003a53ec: mov      lr, pc
003a53f0: ldr      pc, [r3, #0x28]
003a53f4: ldr      r5, [pc, #0xa8]
003a53f8: cmp      r0, #0
003a53fc: add      r5, pc, r5
003a5400: bne      #0x3a5438
003a5404: mov      r4, #0
003a5408: ldr      r3, [pc, #0x98]
003a540c: ldr      r1, [pc, #0x98]
003a5410: ldr      r2, [pc, #0x98]
003a5414: ldr      r3, [r5, r3]
003a5418: add      r1, pc, r1
003a541c: add      r2, pc, r2
003a5420: ldr      r0, [r3, #0x2c]
003a5424: bl       #0x4c4bdc
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404

# _ZNK13ItemInventory18GetCurrentEquipSetEi
003fc6a8: cmp      r1, #0
003fc6ac: blt      #0x3fc6c0
003fc6b0: sub      r1, r1, #1
003fc6b4: cmp      r1, #1
003fc6b8: movhi    r0, #0
003fc6bc: bxhi     lr
003fc6c0: ldrsb    r0, [r0, #0x2e]
003fc6c4: bx       lr

# _ZNK9Character14HasComboAttackEv
003a346c: push     {r4, r5, r6, lr}
003a3470: ldr      r4, [pc, #0x68]
003a3474: ldr      r3, [pc, #0x68]
003a3478: add      r4, pc, r4
003a347c: ldr      r3, [r4, r3]
003a3480: ldr      r5, [r3]
003a3484: bl       #0x3a3228
003a3488: mov      r3, #0xa0
003a348c: mla      r5, r3, r0, r5
003a3490: ldr      r3, [r5, #4]
003a3494: cmp      r3, #0
003a3498: blt      #0x3a34d8
003a349c: ldr      r2, [pc, #0x44]
003a34a0: ldr      r2, [r4, r2]
003a34a4: ldr      r2, [r2]
003a34a8: cmp      r3, r2
003a34ac: bge      #0x3a34d8
003a34b0: ldr      r2, [pc, #0x34]
003a34b4: mov      r1, #0x14
003a34b8: ldr      r2, [r4, r2]
003a34bc: ldr      r2, [r2]
003a34c0: mla      r3, r1, r3, r2
003a34c4: ldr      r0, [r3, #0x10]
003a34c8: cmp      r0, #1
003a34cc: movne    r0, #0
003a34d0: moveq    r0, #1
003a34d4: pop      {r4, r5, r6, pc}
003a34d8: mov      r0, #0
003a34dc: pop      {r4, r5, r6, pc}
003a34e0: subseq   r1, pc, r8, lsl r6
003a34e4: andeq    r4, r0, r4, asr #16
003a34e8: andeq    r2, r0, r8, asr #20
003a34ec: andeq    r3, r0, ip, ror ip

# _ZNK9Character18GetCharAnimTableIdEv
003a3228: mov      r3, #0x1000
003a322c: ldr      r0, [r0, r3]
003a3230: ldr      r3, [pc, #0x24]
003a3234: cmp      r0, #0
003a3238: add      r3, pc, r3
003a323c: blt      #0x3a3254
003a3240: ldr      r2, [pc, #0x18]
003a3244: ldr      r3, [r3, r2]
003a3248: ldr      r3, [r3]
003a324c: cmp      r0, r3
003a3250: bxlt     lr
003a3254: mov      r0, #0x11
003a3258: bx       lr
003a325c: subseq   r1, pc, r8, asr r8
003a3260: andeq    r2, r0, r0, asr #17

# _ZNK13ItemInventory14CanRangeAttackEv
00400014: b        #0x3fffa4
