# _ZNK9Character13GetAnimStanceEv 3a53e0 212
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
003a5424: bl       #0x4c4bdc ; _ZNK15PyDataConstants11getConstantEPKcS1_
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8 ; _ZNK13ItemInventory8HasStaffEv
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080 ; _ZNK13ItemInventory6HasBowEv
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c ; _ZNK13ItemInventory14IsDualWieldingEv
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0 ; _ZNK13ItemInventory12HasTwoHanderEb
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c ; _ZNK13ItemInventory17HasMainHandWeaponEv
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404