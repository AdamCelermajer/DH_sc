
# _ZNK12ItemInstance11IsStackableEv
003f9e58: push     {r4, lr}
003f9e5c: bl       #0x3f9e08
003f9e60: ldrb     r0, [r0, #0x1c]
003f9e64: pop      {r4, pc}

# _ZN12ItemInstance5SplitEi
003fc3e0: push     {r4, r5, r6, lr}
003fc3e4: mov      r6, r1
003fc3e8: mov      r5, r0
003fc3ec: bl       #0x3f9e58
003fc3f0: cmp      r0, #0
003fc3f4: bne      #0x3fc404
003fc3f8: mov      r4, #0
003fc3fc: mov      r0, r4
003fc400: pop      {r4, r5, r6, pc}
003fc404: cmp      r6, #0
003fc408: ble      #0x3fc3f8
003fc40c: ldrsh    r3, [r5, #0x50]
003fc410: cmp      r6, r3
003fc414: bge      #0x3fc3f8
003fc418: rsb      r1, r6, #0
003fc41c: mov      r0, r5
003fc420: bl       #0x3fa17c
003fc424: mov      r1, #0
003fc428: mov      r0, #0x6c
003fc42c: bl       #0x310570
003fc430: mov      r2, r6
003fc434: mov      r4, r0
003fc438: ldr      r1, [r5, #4]
003fc43c: bl       #0x3fc26c
003fc440: mov      r6, #0
003fc444: mov      r0, r4
003fc448: ldr      r1, [r5, #0x54]
003fc44c: bl       #0x3fbc58
003fc450: strb     r6, [r4, #0x68]
003fc454: b        #0x3fc470
003fc458: bl       #0x3fa038
003fc45c: mvn      r2, #0
003fc460: mov      r1, r0
003fc464: mov      r0, r4
003fc468: bl       #0x3fbc60
003fc46c: add      r6, r6, #1
003fc470: mov      r0, r5
003fc474: bl       #0x3f9e80
003fc478: cmp      r6, r0
003fc47c: mov      r1, r6
003fc480: mov      r0, r5
003fc484: blo      #0x3fc458
003fc488: ldrb     r3, [r5, #0x68]
003fc48c: strb     r3, [r4, #0x68]
003fc490: b        #0x3fc3fc

# _ZN13ItemInventory20_HasItemInstanceLikeEPK12ItemInstanceRj
003fe1cc: push     {r4, r5, r6, r7, r8, lr}
003fe1d0: ldr      r4, [r0, #8]
003fe1d4: ldr      r3, [r0, #0xc]
003fe1d8: mov      r6, r0
003fe1dc: mov      r5, r1
003fe1e0: cmp      r3, r4
003fe1e4: mov      r8, r2
003fe1e8: beq      #0x3fe24c
003fe1ec: mov      r7, #0
003fe1f0: ldr      r2, [r4]
003fe1f4: mov      r1, r5
003fe1f8: ldr      ip, [r2]
003fe1fc: cmp      ip, r5
003fe200: mov      r0, ip
003fe204: beq      #0x3fe218
003fe208: bl       #0x3f9d78
003fe20c: cmp      r0, #0
003fe210: bne      #0x3fe22c
003fe214: ldr      r3, [r6, #0xc]
003fe218: add      r4, r4, #4
003fe21c: cmp      r4, r3
003fe220: beq      #0x3fe24c
003fe224: add      r7, r7, #1
003fe228: b        #0x3fe1f0
003fe22c: mov      r0, r6
003fe230: mov      r1, r7
003fe234: bl       #0x3fdaf0
003fe238: cmp      r0, #0
003fe23c: bne      #0x3fe214
003fe240: str      r7, [r8]
003fe244: mov      r0, #1
003fe248: pop      {r4, r5, r6, r7, r8, pc}
003fe24c: mov      r0, #0
003fe250: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14CharProperties21UpdateGearsPropertiesEv
003e08a8: push     {r4, lr}
003e08ac: mov      r4, r0
003e08b0: bl       #0x3defac
003e08b4: mov      r0, r4
003e08b8: bl       #0x3df480
003e08bc: mov      r0, r4
003e08c0: mov      r1, #1
003e08c4: pop      {r4, lr}
003e08c8: b        #0x3e0810

# _ZN13ItemInventory16_DelItemInstanceEP12ItemInstance
003fe7d8: push     {r4, r5, r6, lr}
003fe7dc: ldr      r3, [r0, #0x24]
003fe7e0: ldr      r5, [r0, #8]
003fe7e4: ldr      r2, [r0, #0xc]
003fe7e8: cmp      r3, r1
003fe7ec: moveq    r3, #0
003fe7f0: streq    r3, [r0, #0x24]
003fe7f4: cmp      r5, r2
003fe7f8: mov      r4, r0
003fe7fc: bne      #0x3fe810
003fe800: b        #0x3fe874
003fe804: add      r5, r5, #4
003fe808: cmp      r5, r2
003fe80c: beq      #0x3fe874
003fe810: ldr      r0, [r5]
003fe814: ldr      r3, [r0]
003fe818: cmp      r3, r1
003fe81c: bne      #0x3fe804
003fe820: cmp      r1, #0
003fe824: beq      #0x3fe83c
003fe828: mov      r0, r1
003fe82c: ldr      r3, [r1]
003fe830: mov      lr, pc
003fe834: ldr      pc, [r3, #4]
003fe838: ldr      r0, [r5]
003fe83c: bl       #0x310440
003fe840: ldr      r3, [r4, #0xc]
003fe844: add      r1, r5, #4
003fe848: cmp      r1, r3
003fe84c: beq      #0x3fe868
003fe850: subs     r2, r3, r1
003fe854: moveq    r1, r3
003fe858: beq      #0x3fe868
003fe85c: mov      r0, r5
003fe860: bl       #0x30df38
003fe864: ldr      r1, [r4, #0xc]
003fe868: sub      r1, r1, #4
003fe86c: str      r1, [r4, #0xc]
003fe870: pop      {r4, r5, r6, pc}
003fe874: pop      {r4, r5, r6, pc}

# _ZN9Character12ValidateHPMPEv
003bd140: push     {r4, r5, r6, lr}
003bd144: add      r5, r0, #0xff0
003bd148: add      r4, r0, #0x560
003bd14c: add      r5, r5, #4
003bd150: mov      r1, r5
003bd154: mov      r2, #0x24
003bd158: mov      r0, r4
003bd15c: bl       #0x3dedb4
003bd160: mov      r1, r5
003bd164: mov      r6, r0
003bd168: mov      r2, #0x26
003bd16c: mov      r0, r4
003bd170: bl       #0x3dedb4
003bd174: mov      r1, #0x24
003bd178: cmp      r0, r6
003bd17c: movlt    r2, r0
003bd180: movge    r2, r6
003bd184: mov      r0, r4
003bd188: bl       #0x3e07a0
003bd18c: mov      r1, r5
003bd190: mov      r0, r4
003bd194: mov      r2, #0x29
003bd198: bl       #0x3dedb4
003bd19c: mov      r1, r5
003bd1a0: mov      r6, r0
003bd1a4: mov      r2, #0x2b
003bd1a8: mov      r0, r4
003bd1ac: bl       #0x3dedb4
003bd1b0: mov      r1, #0x29
003bd1b4: cmp      r0, r6
003bd1b8: movlt    r2, r0
003bd1bc: movge    r2, r6
003bd1c0: mov      r0, r4
003bd1c4: pop      {r4, r5, r6, lr}
003bd1c8: b        #0x3e07a0

# _ZNK13ItemInventory14IsItemEquippedEj
003fdaf0: push     {r4, r5, lr}
003fdaf4: mov      r4, r0
003fdaf8: ldr      r2, [r0, #8]
003fdafc: ldr      r0, [r0, #0xc]
003fdb00: ldr      r3, [pc, #0xd8]
003fdb04: sub      sp, sp, #0xc
003fdb08: rsb      r0, r2, r0
003fdb0c: cmp      r1, r0, asr #2
003fdb10: mov      r5, r1
003fdb14: add      r3, pc, r3
003fdb18: blo      #0x3fdb40
003fdb1c: ldr      r1, [pc, #0xc0]
003fdb20: ldr      r1, [r3, r1]
003fdb24: ldr      r1, [r1]
003fdb28: cmp      r1, #2
003fdb2c: moveq    r3, #0
003fdb30: streq    r3, [r3]
003fdb34: beq      #0x3fdb40
003fdb38: cmp      r1, #1
003fdb3c: beq      #0x3fdba8
003fdb40: ldr      r3, [r2, r5, lsl #2]
003fdb44: ldr      r0, [r3]
003fdb48: cmp      r0, #0
003fdb4c: beq      #0x3fdb80
003fdb50: bl       #0x3f9e08
003fdb54: ldr      r1, [r0, #0x68]
003fdb58: cmp      r1, #0
003fdb5c: blt      #0x3fdb88
003fdb60: mov      r0, r4
003fdb64: bl       #0x3fc6a8
003fdb68: ldr      r3, [r4, #8]
003fdb6c: ldr      r3, [r3, r5, lsl #2]
003fdb70: add      r3, r3, r0
003fdb74: ldrsb    r0, [r3, #4]
003fdb78: adds     r0, r0, #1
003fdb7c: movne    r0, #1
003fdb80: add      sp, sp, #0xc
003fdb84: pop      {r4, r5, pc}
003fdb88: cmn      r1, #4
003fdb8c: blt      #0x3fdb60
003fdb90: cmn      r1, #3
003fdb94: movle    r1, #1
003fdb98: ble      #0x3fdb60
003fdb9c: cmn      r1, #2
003fdba0: moveq    r1, #5
003fdba4: b        #0x3fdb60
003fdba8: ldr      r0, [pc, #0x38]
003fdbac: ldr      r1, [pc, #0x38]
003fdbb0: ldr      r2, [pc, #0x38]
003fdbb4: ldr      r0, [r3, r0]
003fdbb8: ldr      r3, [pc, #0x34]
003fdbbc: add      r2, pc, r2
003fdbc0: mov      ip, #0x24c
003fdbc4: add      r1, pc, r1
003fdbc8: add      r0, r0, #0xa8
003fdbcc: add      r3, pc, r3
003fdbd0: str      ip, [sp]
003fdbd4: bl       #0x30e004
003fdbd8: ldr      r2, [r4, #8]
003fdbdc: b        #0x3fdb40
003fdbe0: subseq   r6, sb, ip, ror pc
003fdbe4: andeq    r3, r0, r0, asr #19
003fdbe8: andeq    r1, r0, r0, asr #19
003fdbec: subeq    r0, ip, r4, lsl r8
003fdbf0: subeq    sb, ip, ip, asr #15
003fdbf4: ldrdeq   sb, sl, [ip], #-0x7c

# _ZN9Character13EquipItemAutoEj
003a9fa8: push     {r4, r5, r6, lr}
003a9fac: mov      r4, r0
003a9fb0: add      r0, r0, #0x37c
003a9fb4: bl       #0x400c84
003a9fb8: mov      r5, r0
003a9fbc: add      r0, r4, #0x560
003a9fc0: bl       #0x3e08a8
003a9fc4: mov      r0, r4
003a9fc8: bl       #0x3a999c
003a9fcc: mov      r0, r4
003a9fd0: bl       #0x3bd140
003a9fd4: mov      r0, r5
003a9fd8: pop      {r4, r5, r6, pc}

# _ZNK13ItemInventory18GetCurrentEquipSetEi
003fc6a8: cmp      r1, #0
003fc6ac: blt      #0x3fc6c0
003fc6b0: sub      r1, r1, #1
003fc6b4: cmp      r1, #1
003fc6b8: movhi    r0, #0
003fc6bc: bxhi     lr
003fc6c0: ldrsb    r0, [r0, #0x2e]
003fc6c4: bx       lr

# _ZN13ItemInventory7GetItemEj
003fc61c: ldr      r3, [r0, #8]
003fc620: ldr      r2, [r0, #0xc]
003fc624: rsb      r2, r3, r2
003fc628: cmp      r1, r2, asr #2
003fc62c: ldrlo    r3, [r3, r1, lsl #2]
003fc630: movhs    r0, #0
003fc634: ldrlo    r0, [r3]
003fc638: bx       lr

# _ZN12ItemInstance6AddQtyEi
003fa17c: ldrsh    r3, [r0, #0x50]
003fa180: add      r1, r1, r3
003fa184: b        #0x3fa0e4
