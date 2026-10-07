
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

# _ZN13ItemInventory16SwapEquipmentSetEv
003fc6c8: ldrsb    r2, [r0, #0x2e]
003fc6cc: add      r2, r2, #1
003fc6d0: lsr      r3, r2, #0x1f
003fc6d4: add      r2, r2, r3
003fc6d8: and      r2, r2, #1
003fc6dc: rsb      r3, r3, r2
003fc6e0: strb     r3, [r0, #0x2e]
003fc6e4: bx       lr

# _ZN9Character26INV_CheckItemsRequirementsEv
003a9d10: push     {r4, r5, r6, r7, r8, lr}
003a9d14: add      r5, r0, #0x37c
003a9d18: mov      r6, r0
003a9d1c: mov      r0, r5
003a9d20: bl       #0x3ffd20
003a9d24: subs     r7, r0, #0
003a9d28: beq      #0x3a9d8c
003a9d2c: mov      r4, #0
003a9d30: mov      r8, r4
003a9d34: b        #0x3a9d44
003a9d38: add      r4, r4, #1
003a9d3c: cmp      r4, r7
003a9d40: beq      #0x3a9d84
003a9d44: mov      r1, r4
003a9d48: mov      r0, r5
003a9d4c: bl       #0x3ffe3c
003a9d50: mov      r1, r0
003a9d54: mov      r0, r6
003a9d58: bl       #0x3a4930
003a9d5c: cmp      r0, #0
003a9d60: bne      #0x3a9d38
003a9d64: mov      r1, r4
003a9d68: mov      r0, r5
003a9d6c: mvn      r2, #0
003a9d70: add      r4, r4, #1
003a9d74: bl       #0x40050c
003a9d78: cmp      r4, r7
003a9d7c: mov      r8, #1
003a9d80: bne      #0x3a9d44
003a9d84: cmp      r8, #0
003a9d88: bne      #0x3a9d90
003a9d8c: pop      {r4, r5, r6, r7, r8, pc}
003a9d90: add      r0, r6, #0x560
003a9d94: bl       #0x3e08a8
003a9d98: mov      r0, r6
003a9d9c: bl       #0x3a9d10
003a9da0: mov      r0, r6
003a9da4: bl       #0x3a999c
003a9da8: mov      r0, r6
003a9dac: pop      {r4, r5, r6, r7, r8, lr}
003a9db0: b        #0x3bd140
