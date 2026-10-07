
# _ZNK6CharAI11AI_GetAggroEP9Character
003d4ac8: push     {r4, r5, lr}
003d4acc: ldr      r3, [pc, #0xd8]
003d4ad0: subs     r4, r1, #0
003d4ad4: sub      sp, sp, #0xc
003d4ad8: mov      r5, r0
003d4adc: add      r3, pc, r3
003d4ae0: beq      #0x3d4b50
003d4ae4: ldr      r3, [r5, #0x80]
003d4ae8: add      r5, r5, #0x7c
003d4aec: cmp      r3, #0
003d4af0: beq      #0x3d4b48
003d4af4: mov      r1, r5
003d4af8: b        #0x3d4b00
003d4afc: mov      r3, r2
003d4b00: ldr      r2, [r3, #0x10]
003d4b04: cmp      r4, r2
003d4b08: ldrhi    r2, [r3, #0xc]
003d4b0c: ldrls    r2, [r3, #8]
003d4b10: movhi    r3, r1
003d4b14: mov      r1, r3
003d4b18: cmp      r2, #0
003d4b1c: bne      #0x3d4afc
003d4b20: cmp      r5, r3
003d4b24: beq      #0x3d4ba4
003d4b28: ldr      r2, [r3, #0x10]
003d4b2c: cmp      r4, r2
003d4b30: blo      #0x3d4b48
003d4b34: cmp      r5, r3
003d4b38: ldrne    r0, [r3, #0x14]
003d4b3c: beq      #0x3d4ba4
003d4b40: add      sp, sp, #0xc
003d4b44: pop      {r4, r5, pc}
003d4b48: mov      r3, r5
003d4b4c: b        #0x3d4b34
003d4b50: ldr      r2, [pc, #0x58]
003d4b54: ldr      r2, [r3, r2]
003d4b58: ldr      r2, [r2]
003d4b5c: cmp      r2, #2
003d4b60: streq    r4, [r4]
003d4b64: beq      #0x3d4ae4
003d4b68: cmp      r2, #1
003d4b6c: bne      #0x3d4ae4
003d4b70: ldr      r0, [pc, #0x3c]
003d4b74: ldr      r1, [pc, #0x3c]
003d4b78: ldr      r2, [pc, #0x3c]
003d4b7c: ldr      r0, [r3, r0]
003d4b80: ldr      r3, [pc, #0x38]
003d4b84: movw     ip, #0x229
003d4b88: add      r1, pc, r1
003d4b8c: add      r2, pc, r2
003d4b90: add      r3, pc, r3
003d4b94: add      r0, r0, #0xa8
003d4b98: str      ip, [sp]
003d4b9c: bl       #0x30e004
003d4ba0: b        #0x3d4ae4
003d4ba4: mov      r0, #0
003d4ba8: b        #0x3d4b40
003d4bac: ldrheq   pc, [fp], #-0xf4
003d4bb0: andeq    r3, r0, r0, asr #19
003d4bb4: andeq    r1, r0, r0, asr #19
003d4bb8: subeq    sb, lr, r0, asr r8
003d4bbc: ldrsheq  sp, [r1], #-0x4c
003d4bc0: subeq    r0, pc, r0, lsr sl

# _ZN10AISDefault15OnTargetRevivedEv
003dbeb0: bx       lr

# _ZN6CharAI14OnEnemySpottedEP9Character
003d14b4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d14b8: ldr      r4, [pc, #0x1bc]
003d14bc: ldr      r5, [pc, #0x1bc]
003d14c0: ldr      sb, [pc, #0x1bc]
003d14c4: add      r4, pc, r4
003d14c8: ldr      r3, [r4, r5]
003d14cc: ldr      sl, [r4, sb]
003d14d0: sub      sp, sp, #0x40
003d14d4: ldr      r3, [r3]
003d14d8: mov      r7, r0
003d14dc: mov      r0, sl
003d14e0: str      r3, [sp, #0x3c]
003d14e4: mov      r8, r1
003d14e8: bl       #0x337888
003d14ec: ldr      r1, [pc, #0x194]
003d14f0: add      r6, sp, #0x24
003d14f4: add      r2, sp, #8
003d14f8: add      r1, pc, r1
003d14fc: mov      r0, r6
003d1500: bl       #0x3140ec
003d1504: mov      r1, r6
003d1508: mov      r0, sl
003d150c: bl       #0x337a88
003d1510: mov      r0, r6
003d1514: bl       #0x3139ac
003d1518: ldr      r0, [r7, #0x34]
003d151c: cmp      r0, #0
003d1520: beq      #0x3d1530
003d1524: ldr      r1, [r7, #4]
003d1528: mov      r2, r8
003d152c: bl       #0x3d27cc
003d1530: add      r6, r8, #0x4f0
003d1534: add      r6, r6, #0xc
003d1538: mov      r0, r6
003d153c: bl       #0x3c0230
003d1540: cmp      r0, #0
003d1544: beq      #0x3d1564
003d1548: ldr      r3, [r4, r5]
003d154c: ldr      r2, [sp, #0x3c]
003d1550: ldr      r3, [r3]
003d1554: cmp      r2, r3
003d1558: bne      #0x3d1678
003d155c: add      sp, sp, #0x40
003d1560: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d1564: ldr      r0, [r7, #4]
003d1568: add      r0, r0, #0x4f0
003d156c: add      r0, r0, #0xc
003d1570: bl       #0x3c0230
003d1574: cmp      r0, #0
003d1578: bne      #0x3d1548
003d157c: mov      r0, r6
003d1580: bl       #0x3c01c0
003d1584: cmp      r0, #0
003d1588: bne      #0x3d1548
003d158c: ldr      r0, [r7, #4]
003d1590: add      r0, r0, #0x4f0
003d1594: add      r0, r0, #0xc
003d1598: bl       #0x3c01c0
003d159c: cmp      r0, #0
003d15a0: bne      #0x3d1548
003d15a4: mov      r0, r7
003d15a8: bl       #0x3d4bc4
003d15ac: cmp      r0, #0
003d15b0: beq      #0x3d15cc
003d15b4: ldr      r3, [r8]
003d15b8: mov      r0, r8
003d15bc: mov      lr, pc
003d15c0: ldr      pc, [r3, #0x28]
003d15c4: cmp      r0, #0
003d15c8: beq      #0x3d1618
003d15cc: mov      r1, r8
003d15d0: mov      r0, r7
003d15d4: bl       #0x3d4ac8
003d15d8: mov      r1, #0
003d15dc: bl       #0x30df8c
003d15e0: cmp      r0, #0
003d15e4: beq      #0x3d1618
003d15e8: ldr      r3, [pc, #0x9c]
003d15ec: ldr      r0, [r7, #4]
003d15f0: mov      r1, r8
003d15f4: ldr      r3, [r4, r3]
003d15f8: add      r0, r0, #0x3c8
003d15fc: ldr      r3, [r3]
003d1600: ldr      r2, [r3, #0x30]
003d1604: bl       #0x3d7c68
003d1608: mov      r1, #0
003d160c: bl       #0x30e2f8
003d1610: cmp      r0, #0
003d1614: bne      #0x3d163c
003d1618: ldr      r3, [r7, #0x1c]
003d161c: cmp      r3, #0
003d1620: beq      #0x3d1548
003d1624: mov      r0, r3
003d1628: mov      r1, r8
003d162c: ldr      r3, [r3]
003d1630: mov      lr, pc
003d1634: ldr      pc, [r3, #0x34]
003d1638: b        #0x3d1548
003d163c: ldr      sl, [r4, sb]
003d1640: add      r6, sp, #0xc
003d1644: mov      r0, sl
003d1648: bl       #0x337888
003d164c: ldr      r1, [pc, #0x3c]
003d1650: add      r2, sp, #4
003d1654: mov      r0, r6
003d1658: add      r1, pc, r1
003d165c: bl       #0x3140ec
003d1660: mov      r0, sl
003d1664: mov      r1, r6
003d1668: bl       #0x337a88
003d166c: mov      r0, r6
003d1670: bl       #0x3139ac
003d1674: b        #0x3d1618
003d1678: bl       #0x30e310
003d167c: subseq   r3, ip, ip, asr #11
003d1680: andeq    r4, r0, ip, lsr #1
003d1684: andeq    r0, r0, r4, lsl #17
003d1688: strdeq   r3, r4, [pc], #-0xf0
003d168c: andeq    r3, r0, r8, asr #5
003d1690: subeq    r2, pc, r8, lsl #13

# _ZN6CharAI9GroupInfo14OnEnemySpottedEP9CharacterS2_
003d27cc: push     {r4, r5, r6, r7, r8, lr}
003d27d0: ldrb     r3, [r0, #0x29]
003d27d4: mov      r5, r0
003d27d8: mov      r7, r1
003d27dc: cmp      r3, #0
003d27e0: bne      #0x3d2814
003d27e4: ldr      r3, [r1, #0x400]
003d27e8: cmp      r3, #3
003d27ec: addls    pc, pc, r3, lsl #2
003d27f0: b        #0x3d2814
003d27f4: b        #0x3d2998
003d27f8: b        #0x3d2820
003d27fc: b        #0x3d28c4
003d2800: b        #0x3d2928
003d2804: mov      r3, #0
003d2808: str      r3, [r5, #0x24]
003d280c: mov      r3, #1
003d2810: strb     r3, [r5, #0x29]
003d2814: pop      {r4, r5, r6, r7, r8, pc}
003d2818: mov      r3, #1
003d281c: strb     r3, [r5, #0x29]
003d2820: ldr      r3, [r5, #0x18]
003d2824: ldr      r6, [r5, #0x1c]
003d2828: rsb      r6, r3, r6
003d282c: asrs     r6, r6, #2
003d2830: beq      #0x3d288c
003d2834: mov      r4, #0
003d2838: b        #0x3d284c
003d283c: add      r4, r4, #1
003d2840: cmp      r4, r6
003d2844: beq      #0x3d288c
003d2848: ldr      r3, [r5, #0x18]
003d284c: ldr      r0, [r3, r4, lsl #2]
003d2850: add      r0, r0, #0x4f0
003d2854: add      r0, r0, #0xc
003d2858: bl       #0x3c0230
003d285c: cmp      r0, #0
003d2860: beq      #0x3d283c
003d2864: ldr      r3, [r5, #0x18]
003d2868: mov      r1, #1
003d286c: mov      r2, #0
003d2870: ldr      r0, [r3, r4, lsl #2]
003d2874: add      r4, r4, #1
003d2878: add      r0, r0, #0x4f0
003d287c: add      r0, r0, #0xc
003d2880: bl       #0x3c2734
003d2884: cmp      r4, r6
003d2888: bne      #0x3d2848
003d288c: add      r7, r7, #0x4f0
003d2890: add      r7, r7, #0xc
003d2894: mov      r0, r7
003d2898: bl       #0x3c0230
003d289c: cmp      r0, #0
003d28a0: bne      #0x3d2a20
003d28a4: ldr      r2, [r5, #4]
003d28a8: ldr      r3, [r5]
003d28ac: rsb      r3, r3, r2
003d28b0: lsrs     r3, r3, #2
003d28b4: bne      #0x3d2814
003d28b8: mov      r3, #1
003d28bc: strb     r3, [r5, #0x29]
003d28c0: pop      {r4, r5, r6, r7, r8, pc}
003d28c4: ldr      r3, [r0, #0xc]
003d28c8: ldr      r6, [r0, #0x10]
003d28cc: rsb      r6, r3, r6
003d28d0: asrs     r6, r6, #2
003d28d4: beq      #0x3d2818
003d28d8: mov      r4, #0
003d28dc: b        #0x3d28f0
003d28e0: add      r4, r4, #1
003d28e4: cmp      r4, r6
003d28e8: beq      #0x3d2818
003d28ec: ldr      r3, [r5, #0xc]
003d28f0: ldr      r0, [r3, r4, lsl #2]
003d28f4: add      r0, r0, #0x4f0
003d28f8: add      r0, r0, #0xc
003d28fc: bl       #0x3c0230
003d2900: cmp      r0, #0
003d2904: beq      #0x3d28e0
003d2908: ldr      r3, [r5, #0xc]
003d290c: mov      r1, #1
003d2910: mov      r2, #0
003d2914: ldr      r0, [r3, r4, lsl #2]
003d2918: add      r0, r0, #0x4f0
003d291c: add      r0, r0, #0xc
003d2920: bl       #0x3c2734
003d2924: b        #0x3d28e0
003d2928: ldr      r3, [r0, #0x24]
003d292c: cmn      r3, #1
003d2930: bne      #0x3d2814
003d2934: ldr      r3, [r0, #0x18]
003d2938: ldr      r6, [r0, #0x1c]
003d293c: rsb      r6, r3, r6
003d2940: asrs     r6, r6, #2
003d2944: beq      #0x3d2804
003d2948: mov      r4, #0
003d294c: b        #0x3d2960
003d2950: add      r4, r4, #1
003d2954: cmp      r4, r6
003d2958: beq      #0x3d2804
003d295c: ldr      r3, [r5, #0x18]
003d2960: ldr      r0, [r3, r4, lsl #2]
003d2964: add      r0, r0, #0x4f0
003d2968: add      r0, r0, #0xc
003d296c: bl       #0x3c0230
003d2970: cmp      r0, #0
003d2974: beq      #0x3d2950
003d2978: ldr      r3, [r5, #0x18]
003d297c: mov      r1, #1
003d2980: mov      r2, #0
003d2984: ldr      r0, [r3, r4, lsl #2]
003d2988: add      r0, r0, #0x4f0
003d298c: add      r0, r0, #0xc
003d2990: bl       #0x3c2734
003d2994: b        #0x3d2950
003d2998: ldr      r2, [r0, #4]
003d299c: ldr      r3, [r0]
003d29a0: rsb      r3, r3, r2
003d29a4: lsrs     r3, r3, #2
003d29a8: bne      #0x3d2814
003d29ac: ldr      r4, [r0, #0x10]
003d29b0: ldr      r3, [r0, #0xc]
003d29b4: rsb      r4, r3, r4
003d29b8: asrs     r4, r4, #2
003d29bc: bne      #0x3d2814
003d29c0: ldr      r3, [r0, #0x18]
003d29c4: ldr      r6, [r0, #0x1c]
003d29c8: rsb      r6, r3, r6
003d29cc: asrs     r6, r6, #2
003d29d0: bne      #0x3d29e8
003d29d4: b        #0x3d28b8
003d29d8: add      r4, r4, #1
003d29dc: cmp      r4, r6
003d29e0: beq      #0x3d28b8
003d29e4: ldr      r3, [r5, #0x18]
003d29e8: ldr      r0, [r3, r4, lsl #2]
003d29ec: add      r0, r0, #0x4f0
003d29f0: add      r0, r0, #0xc
003d29f4: bl       #0x3c0230
003d29f8: cmp      r0, #0
003d29fc: beq      #0x3d29d8
003d2a00: ldr      r3, [r5, #0x18]
003d2a04: mov      r1, #1
003d2a08: mov      r2, #0
003d2a0c: ldr      r0, [r3, r4, lsl #2]
003d2a10: add      r0, r0, #0x4f0
003d2a14: add      r0, r0, #0xc
003d2a18: bl       #0x3c2734
003d2a1c: b        #0x3d29d8
003d2a20: mov      r0, r7
003d2a24: mov      r1, #1
003d2a28: mov      r2, #0
003d2a2c: bl       #0x3c2734
003d2a30: b        #0x3d28a4

# _ZNK6CharAI13AI_IsInCombatEv
003d4bc4: push     {r4, lr}
003d4bc8: mov      r4, r0
003d4bcc: bl       #0x3d49f0
003d4bd0: cmp      r0, #0
003d4bd4: beq      #0x3d4be0
003d4bd8: mov      r0, #1
003d4bdc: pop      {r4, pc}
003d4be0: mov      r0, r4
003d4be4: bl       #0x3d4a00
003d4be8: cmp      r0, #0
003d4bec: bne      #0x3d4bd8
003d4bf0: ldr      r0, [r4, #4]
003d4bf4: add      r0, r0, #0x4f0
003d4bf8: add      r0, r0, #0xc
003d4bfc: bl       #0x3c02d0
003d4c00: cmp      r0, #0
003d4c04: bne      #0x3d4bd8
003d4c08: ldr      r0, [r4, #4]
003d4c0c: add      r0, r0, #0x4f0
003d4c10: add      r0, r0, #0xc
003d4c14: bl       #0x3c02e8
003d4c18: cmp      r0, #0
003d4c1c: bne      #0x3d4bd8
003d4c20: ldr      r0, [r4, #4]
003d4c24: add      r0, r0, #0x4f0
003d4c28: add      r0, r0, #0xc
003d4c2c: pop      {r4, lr}
003d4c30: b        #0x3c0334

# _ZN10AISMonster15OnTargetRevivedEv
003dd490: bx       lr

# _ZN9Character10RaiseEventEiPv
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c

# _ZN6CharAI12RaiseAIEventEiPv
003cbb34: ldr      r3, [pc, #0x6d4]
003cbb38: push     {r4, r5, r6, r7, r8, lr}
003cbb3c: add      r3, pc, r3
003cbb40: mov      r4, r1
003cbb44: mov      r5, r0
003cbb48: mov      r6, r2
003cbb4c: cmp      r1, #0x3f
003cbb50: addls    pc, pc, r1, lsl #2
003cbb54: b        #0x3cbcfc
003cbb58: b        #0x3cbc58
003cbb5c: b        #0x3cbe90
003cbb60: b        #0x3cbe98
003cbb64: b        #0x3cbe2c
003cbb68: b        #0x3cbcfc
003cbb6c: b        #0x3cbcfc
003cbb70: b        #0x3cbcfc
003cbb74: b        #0x3cbcfc
003cbb78: b        #0x3cbcfc
003cbb7c: b        #0x3cbcfc
003cbb80: b        #0x3cbcfc
003cbb84: b        #0x3cbcfc
003cbb88: b        #0x3cbcfc
003cbb8c: b        #0x3cbcfc
003cbb90: b        #0x3cbcfc
003cbb94: b        #0x3cbcfc
003cbb98: b        #0x3cbcfc
003cbb9c: b        #0x3cbcfc
003cbba0: b        #0x3cbcfc
003cbba4: b        #0x3cbcfc
003cbba8: b        #0x3cbcfc
003cbbac: b        #0x3cbcfc
003cbbb0: b        #0x3cbcfc
003cbbb4: b        #0x3cbcfc
003cbbb8: b        #0x3cbcfc
003cbbbc: b        #0x3cbcfc
003cbbc0: b        #0x3cbcfc
003cbbc4: b        #0x3cbcfc
003cbbc8: b        #0x3cbcfc
003cbbcc: b        #0x3cbcfc
003cbbd0: b        #0x3cbcfc
003cbbd4: b        #0x3cbcfc
003cbbd8: b        #0x3cbcfc
003cbbdc: b        #0x3cbcfc
003cbbe0: b        #0x3cbe40
003cbbe4: b        #0x3cbe64
003cbbe8: b        #0x3cbcfc
003cbbec: b        #0x3cbcfc
003cbbf0: b        #0x3cbcfc
003cbbf4: b        #0x3cbcfc
003cbbf8: b        #0x3cbe80
003cbbfc: b        #0x3cbc78
003cbc00: b        #0x3cbcfc
003cbc04: b        #0x3cbcfc
003cbc08: b        #0x3cbcfc
003cbc0c: b        #0x3cbcfc
003cbc10: b        #0x3cbcfc
003cbc14: b        #0x3cbcfc
003cbc18: b        #0x3cbc5c
003cbc1c: b        #0x3cbc8c
003cbc20: b        #0x3cbc98
003cbc24: b        #0x3cbca4
003cbc28: b        #0x3cbcac
003cbc2c: b        #0x3cbcbc
003cbc30: b        #0x3cbcfc
003cbc34: b        #0x3cbcfc
003cbc38: b        #0x3cbcfc
003cbc3c: b        #0x3cbcfc
003cbc40: b        #0x3cbcfc
003cbc44: b        #0x3cbcfc
003cbc48: b        #0x3cbcfc
003cbc4c: b        #0x3cbcfc
003cbc50: b        #0x3cbcfc
003cbc54: b        #0x3cbcf0
003cbc58: movw     r4, #0xc351
003cbc5c: ldr      r0, [r5, #4]
003cbc60: add      r0, r0, #0x4f0
003cbc64: add      r0, r0, #0xc
003cbc68: mov      r1, r4
003cbc6c: mov      r2, r6
003cbc70: pop      {r4, r5, r6, r7, r8, lr}
003cbc74: b        #0x3c5684
003cbc78: ldr      r3, [r0]
003cbc7c: mov      r1, r2
003cbc80: mov      lr, pc
003cbc84: ldr      pc, [r3, #0x80]
003cbc88: b        #0x3cbc5c
003cbc8c: mov      r3, #0
003cbc90: strb     r3, [r0, #0x18]
003cbc94: pop      {r4, r5, r6, r7, r8, pc}
003cbc98: mov      r3, #1
003cbc9c: strb     r3, [r0, #0x4a]
003cbca0: pop      {r4, r5, r6, r7, r8, pc}
003cbca4: bl       #0x3cb77c
003cbca8: pop      {r4, r5, r6, r7, r8, pc}
003cbcac: ldr      r0, [r0, #4]
003cbcb0: add      r0, r0, #0x560
003cbcb4: bl       #0x3df3f0
003cbcb8: pop      {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr      r3, [r0]
003cbcc0: cmp      r2, #0
003cbcc4: mvneq    r1, #0
003cbcc8: ldr      r4, [r3, #0x90]
003cbccc: beq      #0x3cbce4
003cbcd0: mov      r0, r2
003cbcd4: ldr      r3, [r2]
003cbcd8: mov      lr, pc
003cbcdc: ldr      pc, [r3]
003cbce0: mov      r1, r0
003cbce4: mov      r0, r5
003cbce8: blx      r4
003cbcec: pop      {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr      r0, [r0, #4]
003cbcf4: bl       #0x394a3c
003cbcf8: b        #0x3cbc5c
003cbcfc: ldr      r0, [r0, #4]
003cbd00: ldr      r1, [r0, #0x378]
003cbd04: ldrb     r2, [r1, #9]
003cbd08: cmp      r2, #0
003cbd0c: bne      #0x3cbd30
003cbd10: ldr      r2, [pc, #0x4fc]
003cbd14: ldr      r3, [r3, r2]
003cbd18: ldrb     r3, [r3]
003cbd1c: cmp      r3, #0
003cbd20: bne      #0x3cbc5c
003cbd24: ldrb     r3, [r1, #8]
003cbd28: cmp      r3, #0
003cbd2c: bne      #0x3cbc5c
003cbd30: sub      r3, r4, #4
003cbd34: cmp      r3, #0x3a
003cbd38: addls    pc, pc, r3, lsl #2
003cbd3c: b        #0x3cbc60
003cbd40: b        #0x3cc1f8
003cbd44: b        #0x3cbc60
003cbd48: b        #0x3cbc60
003cbd4c: b        #0x3cc1e0
003cbd50: b        #0x3cc1c8
003cbd54: b        #0x3cc1ac
003cbd58: b        #0x3cc198
003cbd5c: b        #0x3cc184
003cbd60: b        #0x3cc170
003cbd64: b        #0x3cc15c
003cbd68: b        #0x3cc148
003cbd6c: b        #0x3cc134
003cbd70: b        #0x3cc120
003cbd74: b        #0x3cc10c
003cbd78: b        #0x3cc0f8
003cbd7c: b        #0x3cc0e4
003cbd80: b        #0x3cc0d0
003cbd84: b        #0x3cc0bc
003cbd88: b        #0x3cc0a8
003cbd8c: b        #0x3cc094
003cbd90: b        #0x3cc080
003cbd94: b        #0x3cc06c
003cbd98: b        #0x3cbc60
003cbd9c: b        #0x3cbc60
003cbda0: b        #0x3cbc60
003cbda4: b        #0x3cc044
003cbda8: b        #0x3cc038
003cbdac: b        #0x3cc02c
003cbdb0: b        #0x3cc020
003cbdb4: b        #0x3cc014
003cbdb8: b        #0x3cbc60
003cbdbc: b        #0x3cbc60
003cbdc0: b        #0x3cc004
003cbdc4: b        #0x3cbff4
003cbdc8: b        #0x3cbfe4
003cbdcc: b        #0x3cbfd4
003cbdd0: b        #0x3cbc60
003cbdd4: b        #0x3cbc60
003cbdd8: b        #0x3cbfbc
003cbddc: b        #0x3cbfa4
003cbde0: b        #0x3cbf8c
003cbde4: b        #0x3cbc60
003cbde8: b        #0x3cbc60
003cbdec: b        #0x3cbc60
003cbdf0: b        #0x3cbc60
003cbdf4: b        #0x3cbc60
003cbdf8: b        #0x3cbc60
003cbdfc: b        #0x3cbc60
003cbe00: b        #0x3cbc60
003cbe04: b        #0x3cbc60
003cbe08: b        #0x3cbc60
003cbe0c: b        #0x3cbf70
003cbe10: b        #0x3cbf54
003cbe14: b        #0x3cbf38
003cbe18: b        #0x3cbf1c
003cbe1c: b        #0x3cbf00
003cbe20: b        #0x3cbee4
003cbe24: b        #0x3cbec8
003cbe28: b        #0x3cbeac
003cbe2c: mov      r1, r2
003cbe30: ldr      r3, [r5]
003cbe34: mov      lr, pc
003cbe38: ldr      pc, [r3, #0x28]
003cbe3c: pop      {r4, r5, r6, r7, r8, pc}
003cbe40: bl       #0x3d3aec
003cbe44: ldr      r3, [r5]
003cbe48: mov      r7, r0
003cbe4c: mov      r0, r5
003cbe50: mov      lr, pc
003cbe54: ldr      pc, [r3, #0x98]
003cbe58: cmp      r7, #0
003cbe5c: bne      #0x3cbc5c
003cbe60: pop      {r4, r5, r6, r7, r8, pc}
003cbe64: bl       #0x3d3ae4
003cbe68: ldr      r3, [r5]
003cbe6c: mov      r7, r0
003cbe70: mov      r0, r5
003cbe74: mov      lr, pc
003cbe78: ldr      pc, [r3, #0x98]
003cbe7c: b        #0x3cbe58
003cbe80: mov      r1, r2
003cbe84: bl       #0x3d4434
003cbe88: mov      r7, r0
003cbe8c: b        #0x3cbe58
003cbe90: movw     r4, #0xc352
003cbe94: b        #0x3cbc5c
003cbe98: ldr      r3, [r0]
003cbe9c: mov      r1, r2
003cbea0: mov      lr, pc
003cbea4: ldr      pc, [r3, #0x24]
003cbea8: b        #0x3cbc5c
003cbeac: mov      r0, r5
003cbeb0: mov      r1, r6
003cbeb4: ldr      r3, [r5]
003cbeb8: mov      r2, #0
003cbebc: mov      lr, pc
003cbec0: ldr      pc, [r3, #0xc8]
003cbec4: pop      {r4, r5, r6, r7, r8, pc}
003cbec8: mov      r0, r5
003cbecc: mov      r1, r6
003cbed0: ldr      r3, [r5]
003cbed4: mov      r2, #1
003cbed8: mov      lr, pc
003cbedc: ldr      pc, [r3, #0xc8]
003cbee0: pop      {r4, r5, r6, r7, r8, pc}
003cbee4: mov      r0, r5
003cbee8: mov      r1, r6
003cbeec: ldr      r3, [r5]
003cbef0: mov      r2, #0
003cbef4: mov      lr, pc
003cbef8: ldr      pc, [r3, #0xc4]
003cbefc: pop      {r4, r5, r6, r7, r8, pc}
003cbf00: mov      r0, r5
003cbf04: mov      r1, r6
003cbf08: ldr      r3, [r5]
003cbf0c: mov      r2, #1
003cbf10: mov      lr, pc
003cbf14: ldr      pc, [r3, #0xc4]
003cbf18: pop      {r4, r5, r6, r7, r8, pc}
003cbf1c: mov      r0, r5
003cbf20: mov      r1, r6
003cbf24: ldr      r3, [r5]
003cbf28: mov      r2, #0
003cbf2c: mov      lr, pc
003cbf30: ldr      pc, [r3, #0xc0]
003cbf34: pop      {r4, r5, r6, r7, r8, pc}
003cbf38: mov      r0, r5
003cbf3c: mov      r1, r6
003cbf40: ldr      r3, [r5]
003cbf44: mov      r2, #1
003cbf48: mov      lr, pc
003cbf4c: ldr      pc, [r3, #0xc0]
003cbf50: pop      {r4, r5, r6, r7, r8, pc}
003cbf54: mov      r0, r5
003cbf58: mov      r1, r6
003cbf5c: ldr      r3, [r5]
003cbf60: mov      r2, #0
003cbf64: mov      lr, pc
003cbf68: ldr      pc, [r3, #0xbc]
003cbf6c: pop      {r4, r5, r6, r7, r8, pc}
003cbf70: mov      r0, r5
003cbf74: mov      r1, r6
003cbf78: ldr      r3, [r5]
003cbf7c: mov      r2, #1
003cbf80: mov      lr, pc
003cbf84: ldr      pc, [r3, #0xbc]
003cbf88: pop      {r4, r5, r6, r7, r8, pc}
003cbf8c: mov      r0, r5
003cbf90: ldr      r3, [r5]
003cbf94: mov      lr, pc
003cbf98: ldr      pc, [r3, #0x88]
003cbf9c: ldr      r0, [r5, #4]
003cbfa0: b        #0x3cbc60
003cbfa4: mov      r0, r5
003cbfa8: ldr      r3, [r5]
003cbfac: mov      lr, pc
003cbfb0: ldr      pc, [r3, #0x84]
003cbfb4: ldr      r0, [r5, #4]
003cbfb8: b        #0x3cbc60
003cbfbc: mov      r0, r5
003cbfc0: ldr      r3, [r5]
003cbfc4: mov      lr, pc
003cbfc8: ldr      pc, [r3, #0x8c]
003cbfcc: ldr      r0, [r5, #4]
003cbfd0: b        #0x3cbc60
003cbfd4: mov      r0, r5
003cbfd8: bl       #0x3d3ff8
003cbfdc: mov      r7, r0
003cbfe0: b        #0x3cbe58
003cbfe4: mov      r0, r5
003cbfe8: bl       #0x3d4204
003cbfec: mov      r7, r0
003cbff0: b        #0x3cbe58
003cbff4: mov      r0, r5
003cbff8: bl       #0x3d3d30
003cbffc: mov      r7, r0
003cc000: b        #0x3cbe58
003cc004: mov      r0, r5
003cc008: bl       #0x3d3d4c
003cc00c: mov      r7, r0
003cc010: b        #0x3cbe58
003cc014: mov      r0, r5
003cc018: pop      {r4, r5, r6, r7, r8, lr}
003cc01c: b        #0x3d8b28
003cc020: mov      r0, r5
003cc024: pop      {r4, r5, r6, r7, r8, lr}
003cc028: b        #0x3d8038
003cc02c: mov      r0, r5
003cc030: pop      {r4, r5, r6, r7, r8, lr}
003cc034: b        #0x3d8b7c
003cc038: mov      r0, r5
003cc03c: pop      {r4, r5, r6, r7, r8, lr}
003cc040: b        #0x3d808c
003cc044: ldr      r3, [r5]
003cc048: add      r0, r0, #0x4f0
003cc04c: add      r0, r0, #0xc
003cc050: ldr      r4, [r3, #0x20]
003cc054: bl       #0x3c01ac
003cc058: mov      r1, r6
003cc05c: mov      r2, r0
003cc060: mov      r0, r5
003cc064: blx      r4
003cc068: pop      {r4, r5, r6, r7, r8, pc}
003cc06c: mov      r0, r5
003cc070: ldr      r3, [r5]
003cc074: mov      lr, pc
003cc078: ldr      pc, [r3, #0x7c]
003cc07c: pop      {r4, r5, r6, r7, r8, pc}
003cc080: mov      r0, r5
003cc084: ldr      r3, [r5]
003cc088: mov      lr, pc
003cc08c: ldr      pc, [r3, #0x78]
003cc090: pop      {r4, r5, r6, r7, r8, pc}
003cc094: mov      r0, r5
003cc098: ldr      r3, [r5]
003cc09c: mov      lr, pc
003cc0a0: ldr      pc, [r3, #0x74]
003cc0a4: pop      {r4, r5, r6, r7, r8, pc}
003cc0a8: mov      r0, r5
003cc0ac: ldr      r3, [r5]
003cc0b0: mov      lr, pc
003cc0b4: ldr      pc, [r3, #0x70]
003cc0b8: pop      {r4, r5, r6, r7, r8, pc}
003cc0bc: mov      r0, r5
003cc0c0: ldr      r3, [r5]
003cc0c4: mov      lr, pc
003cc0c8: ldr      pc, [r3, #0x6c]
003cc0cc: pop      {r4, r5, r6, r7, r8, pc}
003cc0d0: mov      r0, r5
003cc0d4: ldr      r3, [r5]
003cc0d8: mov      lr, pc
003cc0dc: ldr      pc, [r3, #0x68]
003cc0e0: pop      {r4, r5, r6, r7, r8, pc}
003cc0e4: mov      r0, r5
003cc0e8: ldr      r3, [r5]
003cc0ec: mov      lr, pc
003cc0f0: ldr      pc, [r3, #0x64]
003cc0f4: pop      {r4, r5, r6, r7, r8, pc}
003cc0f8: mov      r0, r5
003cc0fc: ldr      r3, [r5]
003cc100: mov      lr, pc
003cc104: ldr      pc, [r3, #0x60]
003cc108: pop      {r4, r5, r6, r7, r8, pc}
003cc10c: mov      r0, r5
003cc110: ldr      r3, [r5]
003cc114: mov      lr, pc
003cc118: ldr      pc, [r3, #0x5c]
003cc11c: pop      {r4, r5, r6, r7, r8, pc}
003cc120: mov      r0, r5
003cc124: ldr      r3, [r5]
003cc128: mov      lr, pc
003cc12c: ldr      pc, [r3, #0x58]
003cc130: pop      {r4, r5, r6, r7, r8, pc}
003cc134: mov      r0, r5
003cc138: ldr      r3, [r5]
003cc13c: mov      lr, pc
003cc140: ldr      pc, [r3, #0x54]
003cc144: pop      {r4, r5, r6, r7, r8, pc}
003cc148: mov      r0, r5
003cc14c: ldr      r3, [r5]
003cc150: mov      lr, pc
003cc154: ldr      pc, [r3, #0x50]
003cc158: pop      {r4, r5, r6, r7, r8, pc}
003cc15c: mov      r0, r5
003cc160: ldr      r3, [r5]
003cc164: mov      lr, pc
003cc168: ldr      pc, [r3, #0x4c]
003cc16c: pop      {r4, r5, r6, r7, r8, pc}
003cc170: mov      r0, r5
003cc174: ldr      r3, [r5]
003cc178: mov      lr, pc
003cc17c: ldr      pc, [r3, #0x48]
003cc180: pop      {r4, r5, r6, r7, r8, pc}
003cc184: mov      r0, r5
003cc188: ldr      r3, [r5]
003cc18c: mov      lr, pc
003cc190: ldr      pc, [r3, #0x44]
003cc194: pop      {r4, r5, r6, r7, r8, pc}
003cc198: mov      r0, r5
003cc19c: ldr      r3, [r5]
003cc1a0: mov      lr, pc
003cc1a4: ldr      pc, [r3, #0x40]
003cc1a8: pop      {r4, r5, r6, r7, r8, pc}
003cc1ac: mov      r0, r5
003cc1b0: ldr      r3, [r5]
003cc1b4: mov      r1, r6
003cc1b8: mov      lr, pc
003cc1bc: ldr      pc, [r3, #0x34]
003cc1c0: ldr      r0, [r5, #4]
003cc1c4: b        #0x3cbc60
003cc1c8: mov      r0, r5
003cc1cc: mov      r1, r6
003cc1d0: ldr      r3, [r5]
003cc1d4: mov      lr, pc
003cc1d8: ldr      pc, [r3, #0x30]
003cc1dc: pop      {r4, r5, r6, r7, r8, pc}
003cc1e0: mov      r0, r5
003cc1e4: mov      r1, r6
003cc1e8: ldr      r3, [r5]
003cc1ec: mov      lr, pc
003cc1f0: ldr      pc, [r3, #0x2c]
003cc1f4: pop      {r4, r5, r6, r7, r8, pc}
003cc1f8: mov      r0, r5
003cc1fc: mov      r1, r6
003cc200: ldr      r3, [r5]
003cc204: mov      lr, pc
003cc208: ldr      pc, [r3, #0xb0]
003cc20c: pop      {r4, r5, r6, r7, r8, pc}
003cc210: subseq   r8, ip, r4, asr pc
003cc214: andeq    r3, r0, r0, asr r6

# _ZN6CharAI11AI_AddAggroEP9Characterf
003d7c68: push     {r4, r5, r6, r7, lr}
003d7c6c: ldr      r3, [pc, #0x110]
003d7c70: subs     r4, r1, #0
003d7c74: sub      sp, sp, #0xc
003d7c78: mov      r5, r0
003d7c7c: add      r3, pc, r3
003d7c80: mov      r6, r2
003d7c84: beq      #0x3d7d18
003d7c88: ldr      r3, [r5, #0x80]
003d7c8c: add      r0, r5, #0x7c
003d7c90: cmp      r3, #0
003d7c94: beq      #0x3d7d10
003d7c98: mov      r1, r0
003d7c9c: b        #0x3d7ca4
003d7ca0: mov      r3, r2
003d7ca4: ldr      r2, [r3, #0x10]
003d7ca8: cmp      r4, r2
003d7cac: ldrhi    r2, [r3, #0xc]
003d7cb0: ldrls    r2, [r3, #8]
003d7cb4: movhi    r3, r1
003d7cb8: mov      r1, r3
003d7cbc: cmp      r2, #0
003d7cc0: bne      #0x3d7ca0
003d7cc4: cmp      r0, r3
003d7cc8: beq      #0x3d7d6c
003d7ccc: ldr      r2, [r3, #0x10]
003d7cd0: cmp      r4, r2
003d7cd4: blo      #0x3d7d10
003d7cd8: cmp      r0, r3
003d7cdc: beq      #0x3d7d6c
003d7ce0: ldr      r7, [r3, #0x14]
003d7ce4: mov      r0, r6
003d7ce8: mov      r1, r7
003d7cec: bl       #0x30eba4
003d7cf0: mov      r1, r4
003d7cf4: mov      r2, r0
003d7cf8: mov      r0, r5
003d7cfc: bl       #0x3d79ec
003d7d00: mov      r1, r7
003d7d04: bl       #0x30e3ac
003d7d08: add      sp, sp, #0xc
003d7d0c: pop      {r4, r5, r6, r7, pc}
003d7d10: mov      r3, r0
003d7d14: b        #0x3d7cd8
003d7d18: ldr      r2, [pc, #0x68]
003d7d1c: ldr      r2, [r3, r2]
003d7d20: ldr      r2, [r2]
003d7d24: cmp      r2, #2
003d7d28: streq    r4, [r4]
003d7d2c: beq      #0x3d7c88
003d7d30: cmp      r2, #1
003d7d34: bne      #0x3d7c88
003d7d38: ldr      r0, [pc, #0x4c]
003d7d3c: ldr      r1, [pc, #0x4c]
003d7d40: ldr      r2, [pc, #0x4c]
003d7d44: ldr      r0, [r3, r0]
003d7d48: ldr      r3, [pc, #0x48]
003d7d4c: movw     ip, #0x292
003d7d50: add      r1, pc, r1
003d7d54: add      r2, pc, r2
003d7d58: add      r3, pc, r3
003d7d5c: add      r0, r0, #0xa8
003d7d60: str      ip, [sp]
003d7d64: bl       #0x30e004
003d7d68: b        #0x3d7c88
003d7d6c: mov      r0, r5
003d7d70: mov      r1, r4
003d7d74: mov      r2, r6
003d7d78: add      sp, sp, #0xc
003d7d7c: pop      {r4, r5, r6, r7, lr}
003d7d80: b        #0x3d79ec
003d7d84: subseq   ip, fp, r4, lsl lr
003d7d88: andeq    r3, r0, r0, asr #19
003d7d8c: andeq    r1, r0, r0, asr #19
003d7d90: subeq    r6, lr, r8, lsl #13
003d7d94: subseq   sl, r1, r4, lsr r3
003d7d98: subeq    sp, lr, r8, ror #16
