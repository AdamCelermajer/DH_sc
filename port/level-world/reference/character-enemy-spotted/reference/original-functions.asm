
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

# _ZN7Structs14DesignSettings4readEP11IStreamBase
004ee0d0: push     {r4, r5, lr}
004ee0d4: mov      r4, r0
004ee0d8: sub      sp, sp, #0xc
004ee0dc: mov      r0, r1
004ee0e0: mov      r5, r1
004ee0e4: add      r1, r4, #4
004ee0e8: bl       #0x4db94c
004ee0ec: mov      r3, #1
004ee0f0: cmp      r3, #0
004ee0f4: str      r3, [sp, #4]
004ee0f8: bne      #0x4ee13c
004ee0fc: add      r3, r4, #5
004ee100: add      r2, r4, #6
004ee104: ldrb     r0, [r2, #1]
004ee108: ldrb     r1, [r3, #-1]
004ee10c: cmp      r3, r2
004ee110: eor      r1, r0, r1
004ee114: strb     r1, [r3, #-1]
004ee118: ldrb     r0, [r2, #1]
004ee11c: eor      r1, r1, r0
004ee120: strb     r1, [r2, #1]
004ee124: ldrb     r0, [r3, #-1]
004ee128: sub      r2, r2, #1
004ee12c: eor      r1, r1, r0
004ee130: strb     r1, [r3, #-1]
004ee134: add      r3, r3, #1
004ee138: blo      #0x4ee104
004ee13c: mov      r0, r5
004ee140: add      r1, r4, #8
004ee144: bl       #0x4db94c
004ee148: mov      r3, #1
004ee14c: cmp      r3, #0
004ee150: str      r3, [sp, #4]
004ee154: bne      #0x4ee198
004ee158: add      r3, r4, #9
004ee15c: add      r2, r4, #0xa
004ee160: ldrb     r0, [r2, #1]
004ee164: ldrb     r1, [r3, #-1]
004ee168: cmp      r3, r2
004ee16c: eor      r1, r0, r1
004ee170: strb     r1, [r3, #-1]
004ee174: ldrb     r0, [r2, #1]
004ee178: eor      r1, r1, r0
004ee17c: strb     r1, [r2, #1]
004ee180: ldrb     r0, [r3, #-1]
004ee184: sub      r2, r2, #1
004ee188: eor      r1, r1, r0
004ee18c: strb     r1, [r3, #-1]
004ee190: add      r3, r3, #1
004ee194: blo      #0x4ee160
004ee198: mov      r0, r5
004ee19c: add      r1, r4, #0xc
004ee1a0: bl       #0x4db94c
004ee1a4: mov      r3, #1
004ee1a8: cmp      r3, #0
004ee1ac: str      r3, [sp, #4]
004ee1b0: bne      #0x4ee1f4
004ee1b4: add      r3, r4, #0xd
004ee1b8: add      r2, r4, #0xe
004ee1bc: ldrb     r0, [r2, #1]
004ee1c0: ldrb     r1, [r3, #-1]
004ee1c4: cmp      r3, r2
004ee1c8: eor      r1, r0, r1
004ee1cc: strb     r1, [r3, #-1]
004ee1d0: ldrb     r0, [r2, #1]
004ee1d4: eor      r1, r1, r0
004ee1d8: strb     r1, [r2, #1]
004ee1dc: ldrb     r0, [r3, #-1]
004ee1e0: sub      r2, r2, #1
004ee1e4: eor      r1, r1, r0
004ee1e8: strb     r1, [r3, #-1]
004ee1ec: add      r3, r3, #1
004ee1f0: blo      #0x4ee1bc
004ee1f4: mov      r0, r5
004ee1f8: add      r1, r4, #0x10
004ee1fc: bl       #0x4db94c
004ee200: mov      r3, #1
004ee204: cmp      r3, #0
004ee208: str      r3, [sp, #4]
004ee20c: bne      #0x4ee250
004ee210: add      r3, r4, #0x11
004ee214: add      r2, r4, #0x12
004ee218: ldrb     r0, [r2, #1]
004ee21c: ldrb     r1, [r3, #-1]
004ee220: cmp      r3, r2
004ee224: eor      r1, r0, r1
004ee228: strb     r1, [r3, #-1]
004ee22c: ldrb     r0, [r2, #1]
004ee230: eor      r1, r1, r0
004ee234: strb     r1, [r2, #1]
004ee238: ldrb     r0, [r3, #-1]
004ee23c: sub      r2, r2, #1
004ee240: eor      r1, r1, r0
004ee244: strb     r1, [r3, #-1]
004ee248: add      r3, r3, #1
004ee24c: blo      #0x4ee218
004ee250: mov      r0, r5
004ee254: add      r1, r4, #0x14
004ee258: bl       #0x459090
004ee25c: mov      r3, #1
004ee260: cmp      r3, #0
004ee264: str      r3, [sp, #4]
004ee268: bne      #0x4ee2ac
004ee26c: add      r3, r4, #0x15
004ee270: add      r2, r4, #0x16
004ee274: ldrb     r0, [r2, #1]
004ee278: ldrb     r1, [r3, #-1]
004ee27c: cmp      r3, r2
004ee280: eor      r1, r0, r1
004ee284: strb     r1, [r3, #-1]
004ee288: ldrb     r0, [r2, #1]
004ee28c: eor      r1, r1, r0
004ee290: strb     r1, [r2, #1]
004ee294: ldrb     r0, [r3, #-1]
004ee298: sub      r2, r2, #1
004ee29c: eor      r1, r1, r0
004ee2a0: strb     r1, [r3, #-1]
004ee2a4: add      r3, r3, #1
004ee2a8: blo      #0x4ee274
004ee2ac: mov      r0, r5
004ee2b0: add      r1, r4, #0x18
004ee2b4: bl       #0x4db94c
004ee2b8: mov      r3, #1
004ee2bc: cmp      r3, #0
004ee2c0: str      r3, [sp, #4]
004ee2c4: bne      #0x4ee308
004ee2c8: add      r3, r4, #0x19
004ee2cc: add      r2, r4, #0x1a
004ee2d0: ldrb     r0, [r2, #1]
004ee2d4: ldrb     r1, [r3, #-1]
004ee2d8: cmp      r3, r2
004ee2dc: eor      r1, r0, r1
004ee2e0: strb     r1, [r3, #-1]
004ee2e4: ldrb     r0, [r2, #1]
004ee2e8: eor      r1, r1, r0
004ee2ec: strb     r1, [r2, #1]
004ee2f0: ldrb     r0, [r3, #-1]
004ee2f4: sub      r2, r2, #1
004ee2f8: eor      r1, r1, r0
004ee2fc: strb     r1, [r3, #-1]
004ee300: add      r3, r3, #1
004ee304: blo      #0x4ee2d0
004ee308: mov      r0, r5
004ee30c: add      r1, r4, #0x1c
004ee310: bl       #0x4db94c
004ee314: mov      r3, #1
004ee318: cmp      r3, #0
004ee31c: str      r3, [sp, #4]
004ee320: bne      #0x4ee364
004ee324: add      r3, r4, #0x1d
004ee328: add      r2, r4, #0x1e
004ee32c: ldrb     r0, [r2, #1]
004ee330: ldrb     r1, [r3, #-1]
004ee334: cmp      r3, r2
004ee338: eor      r1, r0, r1
004ee33c: strb     r1, [r3, #-1]
004ee340: ldrb     r0, [r2, #1]
004ee344: eor      r1, r1, r0
004ee348: strb     r1, [r2, #1]
004ee34c: ldrb     r0, [r3, #-1]
004ee350: sub      r2, r2, #1
004ee354: eor      r1, r1, r0
004ee358: strb     r1, [r3, #-1]
004ee35c: add      r3, r3, #1
004ee360: blo      #0x4ee32c
004ee364: mov      r0, r5
004ee368: add      r1, r4, #0x20
004ee36c: bl       #0x4db94c
004ee370: mov      r3, #1
004ee374: cmp      r3, #0
004ee378: str      r3, [sp, #4]
004ee37c: bne      #0x4ee3c0
004ee380: add      r3, r4, #0x21
004ee384: add      r2, r4, #0x22
004ee388: ldrb     r0, [r2, #1]
004ee38c: ldrb     r1, [r3, #-1]
004ee390: cmp      r3, r2
004ee394: eor      r1, r0, r1
004ee398: strb     r1, [r3, #-1]
004ee39c: ldrb     r0, [r2, #1]
004ee3a0: eor      r1, r1, r0
004ee3a4: strb     r1, [r2, #1]
004ee3a8: ldrb     r0, [r3, #-1]
004ee3ac: sub      r2, r2, #1
004ee3b0: eor      r1, r1, r0
004ee3b4: strb     r1, [r3, #-1]
004ee3b8: add      r3, r3, #1
004ee3bc: blo      #0x4ee388
004ee3c0: mov      r0, r5
004ee3c4: add      r1, r4, #0x24
004ee3c8: bl       #0x459090
004ee3cc: mov      r3, #1
004ee3d0: cmp      r3, #0
004ee3d4: str      r3, [sp, #4]
004ee3d8: bne      #0x4ee41c
004ee3dc: add      r3, r4, #0x25
004ee3e0: add      r2, r4, #0x26
004ee3e4: ldrb     r0, [r2, #1]
004ee3e8: ldrb     r1, [r3, #-1]
004ee3ec: cmp      r3, r2
004ee3f0: eor      r1, r0, r1
004ee3f4: strb     r1, [r3, #-1]
004ee3f8: ldrb     r0, [r2, #1]
004ee3fc: eor      r1, r1, r0
004ee400: strb     r1, [r2, #1]
004ee404: ldrb     r0, [r3, #-1]
004ee408: sub      r2, r2, #1
004ee40c: eor      r1, r1, r0
004ee410: strb     r1, [r3, #-1]
004ee414: add      r3, r3, #1
004ee418: blo      #0x4ee3e4
004ee41c: mov      r0, r5
004ee420: add      r1, r4, #0x28
004ee424: bl       #0x459090
004ee428: mov      r3, #1
004ee42c: cmp      r3, #0
004ee430: str      r3, [sp, #4]
004ee434: bne      #0x4ee478
004ee438: add      r3, r4, #0x29
004ee43c: add      r2, r4, #0x2a
004ee440: ldrb     r0, [r2, #1]
004ee444: ldrb     r1, [r3, #-1]
004ee448: cmp      r3, r2
004ee44c: eor      r1, r0, r1
004ee450: strb     r1, [r3, #-1]
004ee454: ldrb     r0, [r2, #1]
004ee458: eor      r1, r1, r0
004ee45c: strb     r1, [r2, #1]
004ee460: ldrb     r0, [r3, #-1]
004ee464: sub      r2, r2, #1
004ee468: eor      r1, r1, r0
004ee46c: strb     r1, [r3, #-1]
004ee470: add      r3, r3, #1
004ee474: blo      #0x4ee440
004ee478: mov      r0, r5
004ee47c: add      r1, r4, #0x2c
004ee480: bl       #0x4db94c
004ee484: mov      r3, #1
004ee488: cmp      r3, #0
004ee48c: str      r3, [sp, #4]
004ee490: bne      #0x4ee4d4
004ee494: add      r3, r4, #0x2d
004ee498: add      r2, r4, #0x2e
004ee49c: ldrb     r0, [r2, #1]
004ee4a0: ldrb     r1, [r3, #-1]
004ee4a4: cmp      r3, r2
004ee4a8: eor      r1, r0, r1
004ee4ac: strb     r1, [r3, #-1]
004ee4b0: ldrb     r0, [r2, #1]
004ee4b4: eor      r1, r1, r0
004ee4b8: strb     r1, [r2, #1]
004ee4bc: ldrb     r0, [r3, #-1]
004ee4c0: sub      r2, r2, #1
004ee4c4: eor      r1, r1, r0
004ee4c8: strb     r1, [r3, #-1]
004ee4cc: add      r3, r3, #1
004ee4d0: blo      #0x4ee49c
004ee4d4: mov      r0, r5
004ee4d8: add      r1, r4, #0x30
004ee4dc: bl       #0x4db94c
004ee4e0: mov      r3, #1
004ee4e4: cmp      r3, #0
004ee4e8: str      r3, [sp, #4]
004ee4ec: bne      #0x4ee530
004ee4f0: add      r3, r4, #0x31
004ee4f4: add      r2, r4, #0x32
004ee4f8: ldrb     r0, [r2, #1]
004ee4fc: ldrb     r1, [r3, #-1]
004ee500: cmp      r3, r2
004ee504: eor      r1, r0, r1
004ee508: strb     r1, [r3, #-1]
004ee50c: ldrb     r0, [r2, #1]
004ee510: eor      r1, r1, r0
004ee514: strb     r1, [r2, #1]
004ee518: ldrb     r0, [r3, #-1]
004ee51c: sub      r2, r2, #1
004ee520: eor      r1, r1, r0
004ee524: strb     r1, [r3, #-1]
004ee528: add      r3, r3, #1
004ee52c: blo      #0x4ee4f8
004ee530: mov      r0, r5
004ee534: add      r1, r4, #0x34
004ee538: bl       #0x4db94c
004ee53c: mov      r3, #1
004ee540: cmp      r3, #0
004ee544: str      r3, [sp, #4]
004ee548: bne      #0x4ee58c
004ee54c: add      r3, r4, #0x35
004ee550: add      r2, r4, #0x36
004ee554: ldrb     r0, [r2, #1]
004ee558: ldrb     r1, [r3, #-1]
004ee55c: cmp      r3, r2
004ee560: eor      r1, r0, r1
004ee564: strb     r1, [r3, #-1]
004ee568: ldrb     r0, [r2, #1]
004ee56c: eor      r1, r1, r0
004ee570: strb     r1, [r2, #1]
004ee574: ldrb     r0, [r3, #-1]
004ee578: sub      r2, r2, #1
004ee57c: eor      r1, r1, r0
004ee580: strb     r1, [r3, #-1]
004ee584: add      r3, r3, #1
004ee588: blo      #0x4ee554
004ee58c: mov      r0, r5
004ee590: add      r1, r4, #0x38
004ee594: bl       #0x4db94c
004ee598: mov      r3, #1
004ee59c: cmp      r3, #0
004ee5a0: str      r3, [sp, #4]
004ee5a4: bne      #0x4ee5e8
004ee5a8: add      r3, r4, #0x39
004ee5ac: add      r2, r4, #0x3a
004ee5b0: ldrb     r0, [r2, #1]
004ee5b4: ldrb     r1, [r3, #-1]
004ee5b8: cmp      r3, r2
004ee5bc: eor      r1, r0, r1
004ee5c0: strb     r1, [r3, #-1]
004ee5c4: ldrb     r0, [r2, #1]
004ee5c8: eor      r1, r1, r0
004ee5cc: strb     r1, [r2, #1]
004ee5d0: ldrb     r0, [r3, #-1]
004ee5d4: sub      r2, r2, #1
004ee5d8: eor      r1, r1, r0
004ee5dc: strb     r1, [r3, #-1]
004ee5e0: add      r3, r3, #1
004ee5e4: blo      #0x4ee5b0
004ee5e8: mov      r0, r5
004ee5ec: add      r1, r4, #0x3c
004ee5f0: bl       #0x4db94c
004ee5f4: mov      r3, #1
004ee5f8: cmp      r3, #0
004ee5fc: str      r3, [sp, #4]
004ee600: bne      #0x4ee644
004ee604: add      r3, r4, #0x3d
004ee608: add      r2, r4, #0x3e
004ee60c: ldrb     r0, [r2, #1]
004ee610: ldrb     r1, [r3, #-1]
004ee614: cmp      r3, r2
004ee618: eor      r1, r0, r1
004ee61c: strb     r1, [r3, #-1]
004ee620: ldrb     r0, [r2, #1]
004ee624: eor      r1, r1, r0
004ee628: strb     r1, [r2, #1]
004ee62c: ldrb     r0, [r3, #-1]
004ee630: sub      r2, r2, #1
004ee634: eor      r1, r1, r0
004ee638: strb     r1, [r3, #-1]
004ee63c: add      r3, r3, #1
004ee640: blo      #0x4ee60c
004ee644: mov      r0, r5
004ee648: add      r1, r4, #0x40
004ee64c: bl       #0x4db94c
004ee650: mov      r3, #1
004ee654: cmp      r3, #0
004ee658: str      r3, [sp, #4]
004ee65c: bne      #0x4ee6a0
004ee660: add      r3, r4, #0x41
004ee664: add      r2, r4, #0x42
004ee668: ldrb     r0, [r2, #1]
004ee66c: ldrb     r1, [r3, #-1]
004ee670: cmp      r3, r2
004ee674: eor      r1, r0, r1
004ee678: strb     r1, [r3, #-1]
004ee67c: ldrb     r0, [r2, #1]
004ee680: eor      r1, r1, r0
004ee684: strb     r1, [r2, #1]
004ee688: ldrb     r0, [r3, #-1]
004ee68c: sub      r2, r2, #1
004ee690: eor      r1, r1, r0
004ee694: strb     r1, [r3, #-1]
004ee698: add      r3, r3, #1
004ee69c: blo      #0x4ee668
004ee6a0: mov      r0, r5
004ee6a4: add      r1, r4, #0x44
004ee6a8: bl       #0x4db94c
004ee6ac: mov      r3, #1
004ee6b0: cmp      r3, #0
004ee6b4: str      r3, [sp, #4]
004ee6b8: bne      #0x4ee6fc
004ee6bc: add      r3, r4, #0x45
004ee6c0: add      r2, r4, #0x46
004ee6c4: ldrb     r0, [r2, #1]
004ee6c8: ldrb     r1, [r3, #-1]
004ee6cc: cmp      r3, r2
004ee6d0: eor      r1, r0, r1
004ee6d4: strb     r1, [r3, #-1]
004ee6d8: ldrb     r0, [r2, #1]
004ee6dc: eor      r1, r1, r0
004ee6e0: strb     r1, [r2, #1]
004ee6e4: ldrb     r0, [r3, #-1]
004ee6e8: sub      r2, r2, #1
004ee6ec: eor      r1, r1, r0
004ee6f0: strb     r1, [r3, #-1]
004ee6f4: add      r3, r3, #1
004ee6f8: blo      #0x4ee6c4
004ee6fc: mov      r0, r5
004ee700: add      r1, r4, #0x48
004ee704: bl       #0x4db94c
004ee708: mov      r3, #1
004ee70c: cmp      r3, #0
004ee710: str      r3, [sp, #4]
004ee714: bne      #0x4ee758
004ee718: add      r3, r4, #0x49
004ee71c: add      r2, r4, #0x4a
004ee720: ldrb     r0, [r2, #1]
004ee724: ldrb     r1, [r3, #-1]
004ee728: cmp      r3, r2
004ee72c: eor      r1, r0, r1
004ee730: strb     r1, [r3, #-1]
004ee734: ldrb     r0, [r2, #1]
004ee738: eor      r1, r1, r0
004ee73c: strb     r1, [r2, #1]
004ee740: ldrb     r0, [r3, #-1]
004ee744: sub      r2, r2, #1
004ee748: eor      r1, r1, r0
004ee74c: strb     r1, [r3, #-1]
004ee750: add      r3, r3, #1
004ee754: blo      #0x4ee720
004ee758: mov      r0, r5
004ee75c: add      r1, r4, #0x4c
004ee760: bl       #0x4db94c
004ee764: mov      r3, #1
004ee768: cmp      r3, #0
004ee76c: str      r3, [sp, #4]
004ee770: bne      #0x4ee7b4
004ee774: add      r3, r4, #0x4d
004ee778: add      r2, r4, #0x4e
004ee77c: ldrb     r0, [r2, #1]
004ee780: ldrb     r1, [r3, #-1]
004ee784: cmp      r3, r2
004ee788: eor      r1, r0, r1
004ee78c: strb     r1, [r3, #-1]
004ee790: ldrb     r0, [r2, #1]
004ee794: eor      r1, r1, r0
004ee798: strb     r1, [r2, #1]
004ee79c: ldrb     r0, [r3, #-1]
004ee7a0: sub      r2, r2, #1
004ee7a4: eor      r1, r1, r0
004ee7a8: strb     r1, [r3, #-1]
004ee7ac: add      r3, r3, #1
004ee7b0: blo      #0x4ee77c
004ee7b4: mov      r0, r5
004ee7b8: add      r1, r4, #0x50
004ee7bc: bl       #0x4db94c
004ee7c0: mov      r3, #1
004ee7c4: cmp      r3, #0
004ee7c8: str      r3, [sp, #4]
004ee7cc: bne      #0x4ee810
004ee7d0: add      r3, r4, #0x51
004ee7d4: add      r2, r4, #0x52
004ee7d8: ldrb     r0, [r2, #1]
004ee7dc: ldrb     r1, [r3, #-1]
004ee7e0: cmp      r3, r2
004ee7e4: eor      r1, r0, r1
004ee7e8: strb     r1, [r3, #-1]
004ee7ec: ldrb     r0, [r2, #1]
004ee7f0: eor      r1, r1, r0
004ee7f4: strb     r1, [r2, #1]
004ee7f8: ldrb     r0, [r3, #-1]
004ee7fc: sub      r2, r2, #1
004ee800: eor      r1, r1, r0
004ee804: strb     r1, [r3, #-1]
004ee808: add      r3, r3, #1
004ee80c: blo      #0x4ee7d8
004ee810: mov      r0, r5
004ee814: add      r1, r4, #0x54
004ee818: bl       #0x4db94c
004ee81c: mov      r3, #1
004ee820: cmp      r3, #0
004ee824: str      r3, [sp, #4]
004ee828: bne      #0x4ee86c
004ee82c: add      r3, r4, #0x55
004ee830: add      r2, r4, #0x56
004ee834: ldrb     r0, [r2, #1]
004ee838: ldrb     r1, [r3, #-1]
004ee83c: cmp      r3, r2
004ee840: eor      r1, r0, r1
004ee844: strb     r1, [r3, #-1]
004ee848: ldrb     r0, [r2, #1]
004ee84c: eor      r1, r1, r0
004ee850: strb     r1, [r2, #1]
004ee854: ldrb     r0, [r3, #-1]
004ee858: sub      r2, r2, #1
004ee85c: eor      r1, r1, r0
004ee860: strb     r1, [r3, #-1]
004ee864: add      r3, r3, #1
004ee868: blo      #0x4ee834
004ee86c: mov      r0, r5
004ee870: add      r1, r4, #0x58
004ee874: bl       #0x4db94c
004ee878: mov      r3, #1
004ee87c: cmp      r3, #0
004ee880: str      r3, [sp, #4]
004ee884: bne      #0x4ee8c8
004ee888: add      r3, r4, #0x59
004ee88c: add      r2, r4, #0x5a
004ee890: ldrb     r0, [r2, #1]
004ee894: ldrb     r1, [r3, #-1]
004ee898: cmp      r3, r2
004ee89c: eor      r1, r0, r1
004ee8a0: strb     r1, [r3, #-1]
004ee8a4: ldrb     r0, [r2, #1]
004ee8a8: eor      r1, r1, r0
004ee8ac: strb     r1, [r2, #1]
004ee8b0: ldrb     r0, [r3, #-1]
004ee8b4: sub      r2, r2, #1
004ee8b8: eor      r1, r1, r0
004ee8bc: strb     r1, [r3, #-1]
004ee8c0: add      r3, r3, #1
004ee8c4: blo      #0x4ee890
004ee8c8: mov      r0, r5
004ee8cc: add      r1, r4, #0x5c
004ee8d0: bl       #0x4db94c
004ee8d4: mov      r3, #1
004ee8d8: cmp      r3, #0
004ee8dc: str      r3, [sp, #4]
004ee8e0: bne      #0x4ee924
004ee8e4: add      r3, r4, #0x5d
004ee8e8: add      r2, r4, #0x5e
004ee8ec: ldrb     r0, [r2, #1]
004ee8f0: ldrb     r1, [r3, #-1]
004ee8f4: cmp      r3, r2
004ee8f8: eor      r1, r0, r1
004ee8fc: strb     r1, [r3, #-1]
004ee900: ldrb     r0, [r2, #1]
004ee904: eor      r1, r1, r0
004ee908: strb     r1, [r2, #1]
004ee90c: ldrb     r0, [r3, #-1]
004ee910: sub      r2, r2, #1
004ee914: eor      r1, r1, r0
004ee918: strb     r1, [r3, #-1]
004ee91c: add      r3, r3, #1
004ee920: blo      #0x4ee8ec
004ee924: mov      r0, r5
004ee928: add      r1, r4, #0x60
004ee92c: bl       #0x4db94c
004ee930: mov      r3, #1
004ee934: cmp      r3, #0
004ee938: str      r3, [sp, #4]
004ee93c: bne      #0x4ee980
004ee940: add      r3, r4, #0x61
004ee944: add      r2, r4, #0x62
004ee948: ldrb     r0, [r2, #1]
004ee94c: ldrb     r1, [r3, #-1]
004ee950: cmp      r3, r2
004ee954: eor      r1, r0, r1
004ee958: strb     r1, [r3, #-1]
004ee95c: ldrb     r0, [r2, #1]
004ee960: eor      r1, r1, r0
004ee964: strb     r1, [r2, #1]
004ee968: ldrb     r0, [r3, #-1]
004ee96c: sub      r2, r2, #1
004ee970: eor      r1, r1, r0
004ee974: strb     r1, [r3, #-1]
004ee978: add      r3, r3, #1
004ee97c: blo      #0x4ee948
004ee980: mov      r0, r5
004ee984: add      r1, r4, #0x64
004ee988: bl       #0x459090
004ee98c: mov      r3, #1
004ee990: cmp      r3, #0
004ee994: str      r3, [sp, #4]
004ee998: bne      #0x4ee9dc
004ee99c: add      r3, r4, #0x65
004ee9a0: add      r2, r4, #0x66
004ee9a4: ldrb     r0, [r2, #1]
004ee9a8: ldrb     r1, [r3, #-1]
004ee9ac: cmp      r3, r2
004ee9b0: eor      r1, r0, r1
004ee9b4: strb     r1, [r3, #-1]
004ee9b8: ldrb     r0, [r2, #1]
004ee9bc: eor      r1, r1, r0
004ee9c0: strb     r1, [r2, #1]
004ee9c4: ldrb     r0, [r3, #-1]
004ee9c8: sub      r2, r2, #1
004ee9cc: eor      r1, r1, r0
004ee9d0: strb     r1, [r3, #-1]
004ee9d4: add      r3, r3, #1
004ee9d8: blo      #0x4ee9a4
004ee9dc: mov      r0, r5
004ee9e0: add      r1, r4, #0x68
004ee9e4: bl       #0x459090
004ee9e8: mov      r3, #1
004ee9ec: cmp      r3, #0
004ee9f0: str      r3, [sp, #4]
004ee9f4: bne      #0x4eea38
004ee9f8: add      r3, r4, #0x69
004ee9fc: add      r2, r4, #0x6a
004eea00: ldrb     r0, [r2, #1]
004eea04: ldrb     r1, [r3, #-1]
004eea08: cmp      r3, r2
004eea0c: eor      r1, r0, r1
004eea10: strb     r1, [r3, #-1]
004eea14: ldrb     r0, [r2, #1]
004eea18: eor      r1, r1, r0
004eea1c: strb     r1, [r2, #1]
004eea20: ldrb     r0, [r3, #-1]
004eea24: sub      r2, r2, #1
004eea28: eor      r1, r1, r0
004eea2c: strb     r1, [r3, #-1]
004eea30: add      r3, r3, #1
004eea34: blo      #0x4eea00
004eea38: mov      r0, r5
004eea3c: add      r1, r4, #0x6c
004eea40: bl       #0x459090
004eea44: mov      r3, #1
004eea48: cmp      r3, #0
004eea4c: str      r3, [sp, #4]
004eea50: bne      #0x4eea94
004eea54: add      r3, r4, #0x6d
004eea58: add      r2, r4, #0x6e
004eea5c: ldrb     r0, [r2, #1]
004eea60: ldrb     r1, [r3, #-1]
004eea64: cmp      r3, r2
004eea68: eor      r1, r0, r1
004eea6c: strb     r1, [r3, #-1]
004eea70: ldrb     r0, [r2, #1]
004eea74: eor      r1, r1, r0
004eea78: strb     r1, [r2, #1]
004eea7c: ldrb     r0, [r3, #-1]
004eea80: sub      r2, r2, #1
004eea84: eor      r1, r1, r0
004eea88: strb     r1, [r3, #-1]
004eea8c: add      r3, r3, #1
004eea90: blo      #0x4eea5c
004eea94: mov      r0, r5
004eea98: add      r1, r4, #0x70
004eea9c: bl       #0x459090
004eeaa0: mov      r3, #1
004eeaa4: cmp      r3, #0
004eeaa8: str      r3, [sp, #4]
004eeaac: bne      #0x4eeaf0
004eeab0: add      r3, r4, #0x71
004eeab4: add      r2, r4, #0x72
004eeab8: ldrb     r0, [r2, #1]
004eeabc: ldrb     r1, [r3, #-1]
004eeac0: cmp      r3, r2
004eeac4: eor      r1, r0, r1
004eeac8: strb     r1, [r3, #-1]
004eeacc: ldrb     r0, [r2, #1]
004eead0: eor      r1, r1, r0
004eead4: strb     r1, [r2, #1]
004eead8: ldrb     r0, [r3, #-1]
004eeadc: sub      r2, r2, #1
004eeae0: eor      r1, r1, r0
004eeae4: strb     r1, [r3, #-1]
004eeae8: add      r3, r3, #1
004eeaec: blo      #0x4eeab8
004eeaf0: mov      r0, r5
004eeaf4: add      r1, r4, #0x74
004eeaf8: bl       #0x459090
004eeafc: mov      r3, #1
004eeb00: cmp      r3, #0
004eeb04: str      r3, [sp, #4]
004eeb08: bne      #0x4eeb4c
004eeb0c: add      r3, r4, #0x75
004eeb10: add      r2, r4, #0x76
004eeb14: ldrb     r0, [r2, #1]
004eeb18: ldrb     r1, [r3, #-1]
004eeb1c: cmp      r3, r2
004eeb20: eor      r1, r0, r1
004eeb24: strb     r1, [r3, #-1]
004eeb28: ldrb     r0, [r2, #1]
004eeb2c: eor      r1, r1, r0
004eeb30: strb     r1, [r2, #1]
004eeb34: ldrb     r0, [r3, #-1]
004eeb38: sub      r2, r2, #1
004eeb3c: eor      r1, r1, r0
004eeb40: strb     r1, [r3, #-1]
004eeb44: add      r3, r3, #1
004eeb48: blo      #0x4eeb14
004eeb4c: mov      r0, r5
004eeb50: add      r1, r4, #0x78
004eeb54: bl       #0x459090
004eeb58: mov      r3, #1
004eeb5c: cmp      r3, #0
004eeb60: str      r3, [sp, #4]
004eeb64: bne      #0x4eeba8
004eeb68: add      r3, r4, #0x79
004eeb6c: add      r2, r4, #0x7a
004eeb70: ldrb     r0, [r2, #1]
004eeb74: ldrb     r1, [r3, #-1]
004eeb78: cmp      r3, r2
004eeb7c: eor      r1, r0, r1
004eeb80: strb     r1, [r3, #-1]
004eeb84: ldrb     r0, [r2, #1]
004eeb88: eor      r1, r1, r0
004eeb8c: strb     r1, [r2, #1]
004eeb90: ldrb     r0, [r3, #-1]
004eeb94: sub      r2, r2, #1
004eeb98: eor      r1, r1, r0
004eeb9c: strb     r1, [r3, #-1]
004eeba0: add      r3, r3, #1
004eeba4: blo      #0x4eeb70
004eeba8: mov      r0, r5
004eebac: add      r1, r4, #0x7c
004eebb0: bl       #0x459090
004eebb4: mov      r3, #1
004eebb8: cmp      r3, #0
004eebbc: str      r3, [sp, #4]
004eebc0: bne      #0x4eec04
004eebc4: add      r3, r4, #0x7d
004eebc8: add      r2, r4, #0x7e
004eebcc: ldrb     r0, [r2, #1]
004eebd0: ldrb     r1, [r3, #-1]
004eebd4: cmp      r3, r2
004eebd8: eor      r1, r0, r1
004eebdc: strb     r1, [r3, #-1]
004eebe0: ldrb     r0, [r2, #1]
004eebe4: eor      r1, r1, r0
004eebe8: strb     r1, [r2, #1]
004eebec: ldrb     r0, [r3, #-1]
004eebf0: sub      r2, r2, #1
004eebf4: eor      r1, r1, r0
004eebf8: strb     r1, [r3, #-1]
004eebfc: add      r3, r3, #1
004eec00: blo      #0x4eebcc
004eec04: mov      r0, r5
004eec08: add      r1, r4, #0x80
004eec0c: bl       #0x459090
004eec10: mov      r3, #1
004eec14: cmp      r3, #0
004eec18: str      r3, [sp, #4]
004eec1c: bne      #0x4eec60
004eec20: add      r3, r4, #0x81
004eec24: add      r2, r4, #0x82
004eec28: ldrb     r0, [r2, #1]
004eec2c: ldrb     r1, [r3, #-1]
004eec30: cmp      r3, r2
004eec34: eor      r1, r0, r1
004eec38: strb     r1, [r3, #-1]
004eec3c: ldrb     r0, [r2, #1]
004eec40: eor      r1, r1, r0
004eec44: strb     r1, [r2, #1]
004eec48: ldrb     r0, [r3, #-1]
004eec4c: sub      r2, r2, #1
004eec50: eor      r1, r1, r0
004eec54: strb     r1, [r3, #-1]
004eec58: add      r3, r3, #1
004eec5c: blo      #0x4eec28
004eec60: mov      r0, r5
004eec64: add      r1, r4, #0x84
004eec68: bl       #0x459090
004eec6c: mov      r3, #1
004eec70: cmp      r3, #0
004eec74: str      r3, [sp, #4]
004eec78: bne      #0x4eecbc
004eec7c: add      r3, r4, #0x85
004eec80: add      r2, r4, #0x86
004eec84: ldrb     r0, [r2, #1]
004eec88: ldrb     r1, [r3, #-1]
004eec8c: cmp      r3, r2
004eec90: eor      r1, r0, r1
004eec94: strb     r1, [r3, #-1]
004eec98: ldrb     r0, [r2, #1]
004eec9c: eor      r1, r1, r0
004eeca0: strb     r1, [r2, #1]
004eeca4: ldrb     r0, [r3, #-1]
004eeca8: sub      r2, r2, #1
004eecac: eor      r1, r1, r0
004eecb0: strb     r1, [r3, #-1]
004eecb4: add      r3, r3, #1
004eecb8: blo      #0x4eec84
004eecbc: mov      r0, r5
004eecc0: add      r1, r4, #0x88
004eecc4: bl       #0x459090
004eecc8: mov      r3, #1
004eeccc: cmp      r3, #0
004eecd0: str      r3, [sp, #4]
004eecd4: bne      #0x4eed18
004eecd8: add      r3, r4, #0x89
004eecdc: add      r2, r4, #0x8a
004eece0: ldrb     r0, [r2, #1]
004eece4: ldrb     r1, [r3, #-1]
004eece8: cmp      r3, r2
004eecec: eor      r1, r0, r1
004eecf0: strb     r1, [r3, #-1]
004eecf4: ldrb     r0, [r2, #1]
004eecf8: eor      r1, r1, r0
004eecfc: strb     r1, [r2, #1]
004eed00: ldrb     r0, [r3, #-1]
004eed04: sub      r2, r2, #1
004eed08: eor      r1, r1, r0
004eed0c: strb     r1, [r3, #-1]
004eed10: add      r3, r3, #1
004eed14: blo      #0x4eece0
004eed18: mov      r0, r5
004eed1c: add      r1, r4, #0x8c
004eed20: bl       #0x4db94c
004eed24: mov      r3, #1
004eed28: cmp      r3, #0
004eed2c: str      r3, [sp, #4]
004eed30: bne      #0x4eed74
004eed34: add      r3, r4, #0x8d
004eed38: add      r2, r4, #0x8e
004eed3c: ldrb     r0, [r2, #1]
004eed40: ldrb     r1, [r3, #-1]
004eed44: cmp      r3, r2
004eed48: eor      r1, r0, r1
004eed4c: strb     r1, [r3, #-1]
004eed50: ldrb     r0, [r2, #1]
004eed54: eor      r1, r1, r0
004eed58: strb     r1, [r2, #1]
004eed5c: ldrb     r0, [r3, #-1]
004eed60: sub      r2, r2, #1
004eed64: eor      r1, r1, r0
004eed68: strb     r1, [r3, #-1]
004eed6c: add      r3, r3, #1
004eed70: blo      #0x4eed3c
004eed74: mov      r0, r5
004eed78: add      r1, r4, #0x90
004eed7c: bl       #0x4db94c
004eed80: mov      r3, #1
004eed84: cmp      r3, #0
004eed88: str      r3, [sp, #4]
004eed8c: bne      #0x4eedd0
004eed90: add      r3, r4, #0x91
004eed94: add      r2, r4, #0x92
004eed98: ldrb     r0, [r2, #1]
004eed9c: ldrb     r1, [r3, #-1]
004eeda0: cmp      r2, r3
004eeda4: eor      r1, r0, r1
004eeda8: strb     r1, [r3, #-1]
004eedac: ldrb     r0, [r2, #1]
004eedb0: eor      r1, r1, r0
004eedb4: strb     r1, [r2, #1]
004eedb8: ldrb     r0, [r3, #-1]
004eedbc: sub      r2, r2, #1
004eedc0: eor      r1, r1, r0
004eedc4: strb     r1, [r3, #-1]
004eedc8: add      r3, r3, #1
004eedcc: bhi      #0x4eed98
004eedd0: mov      r0, r5
004eedd4: add      r1, r4, #0x94
004eedd8: bl       #0x4db94c
004eeddc: mov      r3, #1
004eede0: cmp      r3, #0
004eede4: str      r3, [sp, #4]
004eede8: bne      #0x4eee2c
004eedec: add      r3, r4, #0x95
004eedf0: add      r2, r4, #0x96
004eedf4: ldrb     r0, [r2, #1]
004eedf8: ldrb     r1, [r3, #-1]
004eedfc: cmp      r3, r2
004eee00: eor      r1, r0, r1
004eee04: strb     r1, [r3, #-1]
004eee08: ldrb     r0, [r2, #1]
004eee0c: eor      r1, r1, r0
004eee10: strb     r1, [r2, #1]
004eee14: ldrb     r0, [r3, #-1]
004eee18: sub      r2, r2, #1
004eee1c: eor      r1, r1, r0
004eee20: strb     r1, [r3, #-1]
004eee24: add      r3, r3, #1
004eee28: blo      #0x4eedf4
004eee2c: mov      r0, r5
004eee30: add      r1, r4, #0x98
004eee34: bl       #0x4db94c
004eee38: mov      r3, #1
004eee3c: cmp      r3, #0
004eee40: str      r3, [sp, #4]
004eee44: bne      #0x4eee88
004eee48: add      r3, r4, #0x99
004eee4c: add      r2, r4, #0x9a
004eee50: ldrb     r0, [r2, #1]
004eee54: ldrb     r1, [r3, #-1]
004eee58: cmp      r2, r3
004eee5c: eor      r1, r0, r1
004eee60: strb     r1, [r3, #-1]
004eee64: ldrb     r0, [r2, #1]
004eee68: eor      r1, r1, r0
004eee6c: strb     r1, [r2, #1]
004eee70: ldrb     r0, [r3, #-1]
004eee74: sub      r2, r2, #1
004eee78: eor      r1, r1, r0
004eee7c: strb     r1, [r3, #-1]
004eee80: add      r3, r3, #1
004eee84: bhi      #0x4eee50
004eee88: mov      r0, r5
004eee8c: add      r1, r4, #0x9c
004eee90: bl       #0x4db94c
004eee94: mov      r3, #1
004eee98: cmp      r3, #0
004eee9c: str      r3, [sp, #4]
004eeea0: bne      #0x4eeee4
004eeea4: add      r3, r4, #0x9d
004eeea8: add      r2, r4, #0x9e
004eeeac: ldrb     r0, [r2, #1]
004eeeb0: ldrb     r1, [r3, #-1]
004eeeb4: cmp      r2, r3
004eeeb8: eor      r1, r0, r1
004eeebc: strb     r1, [r3, #-1]
004eeec0: ldrb     r0, [r2, #1]
004eeec4: eor      r1, r1, r0
004eeec8: strb     r1, [r2, #1]
004eeecc: ldrb     r0, [r3, #-1]
004eeed0: sub      r2, r2, #1
004eeed4: eor      r1, r1, r0
004eeed8: strb     r1, [r3, #-1]
004eeedc: add      r3, r3, #1
004eeee0: bhi      #0x4eeeac
004eeee4: mov      r0, r5
004eeee8: add      r1, r4, #0xa0
004eeeec: bl       #0x4db94c
004eeef0: mov      r3, #1
004eeef4: cmp      r3, #0
004eeef8: str      r3, [sp, #4]
004eeefc: bne      #0x4eef40
004eef00: add      r3, r4, #0xa1
004eef04: add      r2, r4, #0xa2
004eef08: ldrb     r0, [r2, #1]
004eef0c: ldrb     r1, [r3, #-1]
004eef10: cmp      r2, r3
004eef14: eor      r1, r0, r1
004eef18: strb     r1, [r3, #-1]
004eef1c: ldrb     r0, [r2, #1]
004eef20: eor      r1, r1, r0
004eef24: strb     r1, [r2, #1]
004eef28: ldrb     r0, [r3, #-1]
004eef2c: sub      r2, r2, #1
004eef30: eor      r1, r1, r0
004eef34: strb     r1, [r3, #-1]
004eef38: add      r3, r3, #1
004eef3c: bhi      #0x4eef08
004eef40: mov      r0, r5
004eef44: add      r1, r4, #0xa4
004eef48: bl       #0x4db94c
004eef4c: mov      r3, #1
004eef50: cmp      r3, #0
004eef54: str      r3, [sp, #4]
004eef58: bne      #0x4eef9c
004eef5c: add      r3, r4, #0xa5
004eef60: add      r2, r4, #0xa6
004eef64: ldrb     r0, [r2, #1]
004eef68: ldrb     r1, [r3, #-1]
004eef6c: cmp      r3, r2
004eef70: eor      r1, r0, r1
004eef74: strb     r1, [r3, #-1]
004eef78: ldrb     r0, [r2, #1]
004eef7c: eor      r1, r1, r0
004eef80: strb     r1, [r2, #1]
004eef84: ldrb     r0, [r3, #-1]
004eef88: sub      r2, r2, #1
004eef8c: eor      r1, r1, r0
004eef90: strb     r1, [r3, #-1]
004eef94: add      r3, r3, #1
004eef98: blo      #0x4eef64
004eef9c: mov      r0, r5
004eefa0: add      r1, r4, #0xa8
004eefa4: bl       #0x4db94c
004eefa8: mov      r3, #1
004eefac: cmp      r3, #0
004eefb0: str      r3, [sp, #4]
004eefb4: bne      #0x4eeff8
004eefb8: add      r3, r4, #0xa9
004eefbc: add      r2, r4, #0xaa
004eefc0: ldrb     r0, [r2, #1]
004eefc4: ldrb     r1, [r3, #-1]
004eefc8: cmp      r2, r3
004eefcc: eor      r1, r0, r1
004eefd0: strb     r1, [r3, #-1]
004eefd4: ldrb     r0, [r2, #1]
004eefd8: eor      r1, r1, r0
004eefdc: strb     r1, [r2, #1]
004eefe0: ldrb     r0, [r3, #-1]
004eefe4: sub      r2, r2, #1
004eefe8: eor      r1, r1, r0
004eefec: strb     r1, [r3, #-1]
004eeff0: add      r3, r3, #1
004eeff4: bhi      #0x4eefc0
004eeff8: mov      r0, r5
004eeffc: add      r1, r4, #0xac
004ef000: bl       #0x4db94c
004ef004: mov      r3, #1
004ef008: cmp      r3, #0
004ef00c: str      r3, [sp, #4]
004ef010: bne      #0x4ef054
004ef014: add      r3, r4, #0xae
004ef018: add      r4, r4, #0xad
004ef01c: ldrb     r1, [r3, #1]
004ef020: ldrb     r2, [r4, #-1]
004ef024: cmp      r3, r4
004ef028: eor      r2, r1, r2
004ef02c: strb     r2, [r4, #-1]
004ef030: ldrb     r1, [r3, #1]
004ef034: eor      r2, r2, r1
004ef038: strb     r2, [r3, #1]
004ef03c: ldrb     r1, [r4, #-1]
004ef040: sub      r3, r3, #1
004ef044: eor      r2, r2, r1
004ef048: strb     r2, [r4, #-1]
004ef04c: add      r4, r4, #1
004ef050: bhi      #0x4ef01c
004ef054: add      sp, sp, #0xc
004ef058: pop      {r4, r5, pc}

# _ZN6Arrays19DesignSettingsTable4readEP11IStreamBase
004b3cd0: push     {r4, r5, r6, r7, r8, sl, lr}
004b3cd4: sub      sp, sp, #0xc
004b3cd8: mov      sl, r0
004b3cdc: bl       #0x313a90
004b3ce0: ldr      r6, [pc, #0x120]
004b3ce4: mov      r3, #1
004b3ce8: cmp      r3, #0
004b3cec: str      r0, [sp, #4]
004b3cf0: str      r3, [sp]
004b3cf4: add      r6, pc, r6
004b3cf8: bne      #0x4b3d40
004b3cfc: add      r3, sp, #4
004b3d00: add      r2, r3, #2
004b3d04: add      r3, r3, #1
004b3d08: ldrb     r0, [r2, #1]
004b3d0c: ldrb     r1, [r3, #-1]
004b3d10: cmp      r2, r3
004b3d14: eor      r1, r0, r1
004b3d18: strb     r1, [r3, #-1]
004b3d1c: ldrb     r0, [r2, #1]
004b3d20: eor      r1, r1, r0
004b3d24: strb     r1, [r2, #1]
004b3d28: ldrb     r0, [r3, #-1]
004b3d2c: sub      r2, r2, #1
004b3d30: eor      r1, r1, r0
004b3d34: strb     r1, [r3, #-1]
004b3d38: add      r3, r3, #1
004b3d3c: bhi      #0x4b3d08
004b3d40: bl       #0x4a90b8
004b3d44: ldr      r4, [sp, #4]
004b3d48: ldr      r7, [pc, #0xbc]
004b3d4c: mov      r0, #0x16
004b3d50: mul      r0, r0, r4
004b3d54: ldr      r3, [r6, r7]
004b3d58: add      r0, r0, #1
004b3d5c: lsl      r0, r0, #3
004b3d60: str      r4, [r3]
004b3d64: mov      r1, #1
004b3d68: bl       #0x31056c
004b3d6c: mov      r3, #0xb0
004b3d70: cmp      r4, #0
004b3d74: stm      r0, {r3, r4}
004b3d78: add      r3, r0, #8
004b3d7c: beq      #0x4b3da4
004b3d80: ldr      r1, [pc, #0x88]
004b3d84: mov      r2, #0
004b3d88: ldr      r1, [r6, r1]
004b3d8c: add      r1, r1, #8
004b3d90: add      r2, r2, #1
004b3d94: cmp      r2, r4
004b3d98: str      r1, [r0, #8]
004b3d9c: add      r0, r0, #0xb0
004b3da0: bne      #0x4b3d90
004b3da4: ldr      r2, [r6, r7]
004b3da8: ldr      r8, [pc, #0x64]
004b3dac: ldr      r1, [r2]
004b3db0: ldr      r2, [r6, r8]
004b3db4: cmp      r1, #0
004b3db8: str      r3, [r2]
004b3dbc: beq      #0x4b3e00
004b3dc0: mov      r4, #0
004b3dc4: mov      r5, r4
004b3dc8: b        #0x4b3dd4
004b3dcc: ldr      r3, [r6, r8]
004b3dd0: ldr      r3, [r3]
004b3dd4: add      r0, r3, r4
004b3dd8: mov      r1, sl
004b3ddc: ldr      r3, [r3, r4]
004b3de0: mov      lr, pc
004b3de4: ldr      pc, [r3, #0xc]
004b3de8: ldr      r3, [r6, r7]
004b3dec: add      r5, r5, #1
004b3df0: add      r4, r4, #0xb0
004b3df4: ldr      r3, [r3]
004b3df8: cmp      r3, r5
004b3dfc: bhi      #0x4b3dcc
004b3e00: add      sp, sp, #0xc
004b3e04: pop      {r4, r5, r6, r7, r8, sl, pc}
004b3e08: umaaleq  r0, lr, ip, sp
004b3e0c: andeq    r2, r0, r4, ror #29
004b3e10: andeq    r4, r0, r0, lsl #13
004b3e14: andeq    r3, r0, r8, asr #5

# _ZN7Structs19GetMemberIDByStringINS_14DesignSettingsEEEiPKc
004aedfc: ldr      r3, [pc, #0x4c]
004aee00: ldr      r2, [pc, #0x4c]
004aee04: push     {r4, r5, r6, lr}
004aee08: add      r3, pc, r3
004aee0c: mov      r6, r0
004aee10: ldr      r5, [r3, r2]
004aee14: mov      r4, #0
004aee18: b        #0x4aee2c
004aee1c: add      r4, r4, #1
004aee20: cmp      r4, #0x2b
004aee24: add      r5, r5, #0x18
004aee28: beq      #0x4aee48
004aee2c: ldr      r1, [r5, #0x14]
004aee30: mov      r0, r6
004aee34: bl       #0x30e31c
004aee38: cmp      r0, #0
004aee3c: bne      #0x4aee1c
004aee40: mov      r0, r4
004aee44: pop      {r4, r5, r6, pc}
004aee48: mvn      r0, #0
004aee4c: pop      {r4, r5, r6, pc}
004aee50: subeq    r5, lr, r8, lsl #25
004aee54: muleq    r0, ip, sb

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
