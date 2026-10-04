
# _ZN9Character13_GetStateTimeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b6d6c: mov      r0, r1
003b6d70: ldr      r1, [r2, #0x55c]
003b6d74: b        #0x37cb24

# _ZN16CharStateMachine9_GetStateEi
003c184c: push     {r4, r5, lr}
003c1850: sub      sp, sp, #0xc
003c1854: mov      r5, r0
003c1858: mov      r4, r1
003c185c: bl       #0x3c0084
003c1860: ldr      r3, [pc, #0xb8]
003c1864: cmp      r0, #0
003c1868: add      r3, pc, r3
003c186c: bne      #0x3c1890
003c1870: ldr      r2, [pc, #0xac]
003c1874: ldr      r2, [r3, r2]
003c1878: ldr      r2, [r2]
003c187c: cmp      r2, #2
003c1880: streq    r0, [r0]
003c1884: beq      #0x3c1890
003c1888: cmp      r2, #1
003c188c: beq      #0x3c18ec
003c1890: ldr      r3, [r5, #0xc]
003c1894: add      r5, r5, #8
003c1898: cmp      r3, #0
003c189c: beq      #0x3c18e0
003c18a0: mov      r1, r5
003c18a4: b        #0x3c18ac
003c18a8: mov      r3, r2
003c18ac: ldr      r2, [r3, #0x10]
003c18b0: cmp      r4, r2
003c18b4: ldrgt    r2, [r3, #0xc]
003c18b8: ldrle    r2, [r3, #8]
003c18bc: movgt    r3, r1
003c18c0: mov      r1, r3
003c18c4: cmp      r2, #0
003c18c8: bne      #0x3c18a8
003c18cc: cmp      r5, r3
003c18d0: beq      #0x3c18e0
003c18d4: ldr      r2, [r3, #0x10]
003c18d8: cmp      r4, r2
003c18dc: movge    r5, r3
003c18e0: add      r0, r5, #0x14
003c18e4: add      sp, sp, #0xc
003c18e8: pop      {r4, r5, pc}
003c18ec: ldr      r0, [pc, #0x34]
003c18f0: ldr      r1, [pc, #0x34]
003c18f4: ldr      r2, [pc, #0x34]
003c18f8: ldr      r0, [r3, r0]
003c18fc: ldr      r3, [pc, #0x30]
003c1900: mov      ip, #0x97
003c1904: add      r1, pc, r1
003c1908: add      r2, pc, r2
003c190c: add      r3, pc, r3
003c1910: add      r0, r0, #0xa8
003c1914: str      ip, [sp]
003c1918: bl       #0x30e004
003c191c: b        #0x3c1890
003c1920: subseq   r3, sp, r8, lsr #4
003c1924: andeq    r3, r0, r0, asr #19
003c1928: andeq    r1, r0, r0, asr #19
003c192c: ldrdeq   ip, sp, [pc], #-0xa4
003c1930: subseq   r3, r0, r8, asr #6
003c1934: ldrsbeq  r3, [r0], #-0x2c

# _ZN16CharStateMachine9_SetStateEiiPv
003c1938: push     {r4, r5, r6, r7, r8, sl, lr}
003c193c: ldr      ip, [r0, #0x20]
003c1940: sub      sp, sp, #0x14
003c1944: mov      r4, r0
003c1948: cmp      ip, #0
003c194c: mvneq    r7, #0
003c1950: mov      r5, r1
003c1954: mov      r8, r2
003c1958: mov      sl, r3
003c195c: moveq    r6, r7
003c1960: beq      #0x3c1990
003c1964: ldr      r3, [ip, #4]
003c1968: ldr      r6, [ip]
003c196c: ldr      r2, [r0, #4]
003c1970: ldr      ip, [r3]
003c1974: mov      r0, r3
003c1978: str      r1, [sp]
003c197c: mov      r3, r4
003c1980: mov      r1, r6
003c1984: mov      lr, pc
003c1988: ldr      pc, [ip, #0x10]
003c198c: mov      r7, r6
003c1990: mov      r0, r4
003c1994: mov      r1, r5
003c1998: bl       #0x3c0084
003c199c: cmp      r0, #0
003c19a0: streq    r0, [r4, #0x20]
003c19a4: bne      #0x3c19c0
003c19a8: ldr      r0, [r4, #4]
003c19ac: mov      r2, r7
003c19b0: mov      r1, #0x1d
003c19b4: add      sp, sp, #0x14
003c19b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003c19bc: b        #0x3a4d5c
003c19c0: mov      r1, r5
003c19c4: mov      r0, r4
003c19c8: bl       #0x3c184c
003c19cc: cmp      r6, r5
003c19d0: movne    r3, #0
003c19d4: str      r0, [r4, #0x20]
003c19d8: strne    r3, [r4, #0x60]
003c19dc: ldm      r0, {r1, r3}
003c19e0: ldr      r2, [r4, #4]
003c19e4: ldr      ip, [r3]
003c19e8: mov      r0, r3
003c19ec: stm      sp, {r6, r8, sl}
003c19f0: mov      r3, r4
003c19f4: mov      lr, pc
003c19f8: ldr      pc, [ip, #0xc]
003c19fc: b        #0x3c19a8

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

# _ZN16CharStateMachine12SetCharacterEP9Character
003c1600: push     {r4, r5, lr}
003c1604: ldr      r3, [pc, #0x70]
003c1608: subs     r4, r1, #0
003c160c: sub      sp, sp, #0xc
003c1610: mov      r5, r0
003c1614: add      r3, pc, r3
003c1618: beq      #0x3c1628
003c161c: str      r4, [r5, #4]
003c1620: add      sp, sp, #0xc
003c1624: pop      {r4, r5, pc}
003c1628: ldr      r2, [pc, #0x50]
003c162c: ldr      r2, [r3, r2]
003c1630: ldr      r2, [r2]
003c1634: cmp      r2, #2
003c1638: streq    r4, [r4]
003c163c: beq      #0x3c161c
003c1640: cmp      r2, #1
003c1644: bne      #0x3c161c
003c1648: ldr      r0, [pc, #0x34]
003c164c: ldr      r1, [pc, #0x34]
003c1650: ldr      r2, [pc, #0x34]
003c1654: ldr      r0, [r3, r0]
003c1658: ldr      r3, [pc, #0x30]
003c165c: mov      ip, #0xe1
003c1660: add      r1, pc, r1
003c1664: add      r2, pc, r2
003c1668: add      r3, pc, r3
003c166c: add      r0, r0, #0xa8
003c1670: str      ip, [sp]
003c1674: bl       #0x30e004
003c1678: b        #0x3c161c
003c167c: subseq   r3, sp, ip, ror r4
003c1680: andeq    r3, r0, r0, asr #19
003c1684: andeq    r1, r0, r0, asr #19
003c1688: subeq    ip, pc, r8, ror sp
003c168c: subseq   r0, r3, r4, lsr #20
003c1690: subseq   r3, r0, r0, lsl #11

# _ZN9Character9_GetStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b6d78: add      r0, r2, #0x4f0
003b6d7c: push     {r4, lr}
003b6d80: add      r0, r0, #0xc
003b6d84: mov      r4, r1
003b6d88: bl       #0x3c01ac
003b6d8c: mov      r3, r0
003b6d90: mov      r1, r3
003b6d94: mov      r0, r4
003b6d98: pop      {r4, lr}
003b6d9c: b        #0x37cb24

# _ZNK9Character16GetPreSetAIStateEv
003a5784: push     {r4, lr}
003a5788: movw     r3, #0x13dc
003a578c: ldr      r2, [r0, r3]
003a5790: movw     r3, #0x13e0
003a5794: ldr      r3, [r0, r3]
003a5798: cmp      r2, r3
003a579c: beq      #0x3a57e4
003a57a0: ldr      r1, [pc, #0x44]
003a57a4: add      r4, r0, #0x13c0
003a57a8: add      r4, r4, #0xc
003a57ac: add      r1, pc, r1
003a57b0: mov      r0, r4
003a57b4: bl       #0x3a5720
003a57b8: cmp      r0, #0
003a57bc: bne      #0x3a57c4
003a57c0: pop      {r4, pc}
003a57c4: ldr      r1, [pc, #0x24]
003a57c8: mov      r0, r4
003a57cc: add      r1, pc, r1
003a57d0: bl       #0x3a5720
003a57d4: cmp      r0, #0
003a57d8: bne      #0x3a57e4
003a57dc: mov      r0, #0x11
003a57e0: pop      {r4, pc}
003a57e4: mov      r0, #3
003a57e8: pop      {r4, pc}
003a57ec: subseq   sp, r1, r4, lsl #22
003a57f0: subseq   sp, r1, ip, ror #21

# _ZN16CharStateMachine6UpdateEv
003c628c: push     {r4, r5, r6, lr}
003c6290: mov      r4, r0
003c6294: ldr      r0, [pc, #0xe8]
003c6298: sub      sp, sp, #8
003c629c: ldr      r5, [pc, #0xe4]
003c62a0: add      r0, pc, r0
003c62a4: bl       #0x3136b4
003c62a8: ldr      r3, [pc, #0xdc]
003c62ac: add      r5, pc, r5
003c62b0: ldr      r6, [r4, #0x60]
003c62b4: ldr      r0, [r5, r3]
003c62b8: bl       #0x31f66c
003c62bc: ldr      r3, [r4, #0x2c]
003c62c0: add      r0, r0, r6
003c62c4: str      r0, [r4, #0x60]
003c62c8: tst      r3, #2
003c62cc: bne      #0x3c6314
003c62d0: tst      r3, #4
003c62d4: bne      #0x3c6350
003c62d8: ldr      r3, [r4, #0x20]
003c62dc: cmp      r3, #0
003c62e0: beq      #0x3c6300
003c62e4: ldm      r3, {r1, r2}
003c62e8: mov      r3, r4
003c62ec: mov      r0, r2
003c62f0: ldr      ip, [r2]
003c62f4: ldr      r2, [r4, #4]
003c62f8: mov      lr, pc
003c62fc: ldr      pc, [ip, #0x14]
003c6300: ldr      r0, [pc, #0x88]
003c6304: add      r0, pc, r0
003c6308: add      sp, sp, #8
003c630c: pop      {r4, r5, r6, lr}
003c6310: b        #0x3136b8
003c6314: mov      r0, r4
003c6318: mov      r1, #0
003c631c: bl       #0x3c0378
003c6320: subs     ip, r0, #0
003c6324: bne      #0x3c6344
003c6328: ldr      r2, [r4, #0x24]
003c632c: mov      r3, ip
003c6330: mov      r0, r4
003c6334: ubfx     r2, r2, #0xb, #1
003c6338: mvn      r1, #0
003c633c: str      ip, [sp]
003c6340: bl       #0x3c5ffc
003c6344: ldr      r3, [r4, #0x2c]
003c6348: tst      r3, #4
003c634c: beq      #0x3c62d8
003c6350: mov      r0, r4
003c6354: mov      r1, #0
003c6358: bl       #0x3c034c
003c635c: subs     ip, r0, #0
003c6360: bne      #0x3c62d8
003c6364: ldr      r2, [r4, #0x24]
003c6368: mov      r3, ip
003c636c: mov      r0, r4
003c6370: ubfx     r2, r2, #0xa, #1
003c6374: mvn      r1, #0
003c6378: str      ip, [sp]
003c637c: bl       #0x3c6144
003c6380: b        #0x3c62d8
003c6384: subeq    lr, pc, r0, lsr #25
003c6388: subseq   lr, ip, r4, ror #15
003c638c: strdeq   r3, r4, [r0], -r4
003c6390: subeq    lr, pc, ip, lsr ip

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZN16CharStateMachine13RegisterStateEi
003c7318: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c731c: ldr      r4, [pc, #0x178]
003c7320: ldr      r5, [pc, #0x178]
003c7324: ldr      r3, [r0, #0xc]
003c7328: add      r4, pc, r4
003c732c: ldr      r2, [r4, r5]
003c7330: sub      sp, sp, #0x2c
003c7334: cmp      r3, #0
003c7338: ldr      r2, [r2]
003c733c: mov      r7, r0
003c7340: str      r1, [sp, #4]
003c7344: add      r8, r0, #8
003c7348: str      r2, [sp, #0x24]
003c734c: beq      #0x3c73b4
003c7350: mov      r0, r8
003c7354: b        #0x3c7360
003c7358: mov      r0, r3
003c735c: mov      r3, r2
003c7360: ldr      r2, [r3, #0x10]
003c7364: cmp      r2, r1
003c7368: ldrlt    r2, [r3, #0xc]
003c736c: ldrge    r2, [r3, #8]
003c7370: movlt    r3, r0
003c7374: cmp      r2, #0
003c7378: bne      #0x3c7358
003c737c: cmp      r8, r3
003c7380: beq      #0x3c73c0
003c7384: ldr      r2, [r3, #0x10]
003c7388: cmp      r1, r2
003c738c: blt      #0x3c73b4
003c7390: cmp      r8, r3
003c7394: beq      #0x3c73c0
003c7398: ldr      r3, [r4, r5]
003c739c: ldr      r2, [sp, #0x24]
003c73a0: ldr      r3, [r3]
003c73a4: cmp      r2, r3
003c73a8: bne      #0x3c7498
003c73ac: add      sp, sp, #0x2c
003c73b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c73b4: mov      r3, r8
003c73b8: cmp      r8, r3
003c73bc: bne      #0x3c7398
003c73c0: ldr      sl, [pc, #0xdc]
003c73c4: ldr      r1, [sp, #4]
003c73c8: mov      r3, #0
003c73cc: add      sl, pc, sl
003c73d0: b        #0x3c73e0
003c73d4: add      r3, r3, #1
003c73d8: cmp      r3, #0x14
003c73dc: beq      #0x3c7398
003c73e0: ldr      r2, [sl, r3, lsl #3]
003c73e4: lsl      sb, r3, #3
003c73e8: cmp      r1, r2
003c73ec: bne      #0x3c73d4
003c73f0: add      fp, sp, #4
003c73f4: mov      r1, fp
003c73f8: mov      r0, r8
003c73fc: bl       #0x3c71e0
003c7400: ldr      r3, [sp, #4]
003c7404: mov      r1, fp
003c7408: add      sl, sl, sb
003c740c: str      r3, [r0]
003c7410: mov      r0, r8
003c7414: bl       #0x3c71e0
003c7418: str      r0, [sp]
003c741c: mov      lr, pc
003c7420: ldr      pc, [sl, #4]
003c7424: ldr      r3, [sp]
003c7428: mov      r1, fp
003c742c: add      r6, sp, #0xc
003c7430: str      r0, [r3, #4]
003c7434: mov      r0, r8
003c7438: bl       #0x3c71e0
003c743c: ldr      ip, [r0, #4]
003c7440: ldr      r1, [sp, #4]
003c7444: ldr      r2, [r7, #4]
003c7448: mov      r3, r7
003c744c: mov      r0, ip
003c7450: ldr      ip, [ip]
003c7454: mov      lr, pc
003c7458: ldr      pc, [ip, #8]
003c745c: ldr      r3, [pc, #0x44]
003c7460: ldr      r7, [r4, r3]
003c7464: mov      r0, r7
003c7468: bl       #0x337888
003c746c: ldr      r1, [pc, #0x38]
003c7470: add      r2, sp, #8
003c7474: mov      r0, r6
003c7478: add      r1, pc, r1
003c747c: bl       #0x3140ec
003c7480: mov      r0, r7
003c7484: mov      r1, r6
003c7488: bl       #0x337a88
003c748c: mov      r0, r6
003c7490: bl       #0x318254
003c7494: b        #0x3c7398
003c7498: bl       #0x30e310
003c749c: subseq   sp, ip, r8, ror #14
003c74a0: andeq    r4, r0, ip, lsr #1
003c74a4: subseq   pc, sb, ip, ror #5
003c74a8: andeq    r0, r0, r4, lsl #17
003c74ac: strheq   sp, [pc], #-0xa8

# _ZNK16CharStateMachine9_GetEventEii
003c1694: push     {r4, r5, r6, r7, lr}
003c1698: sub      sp, sp, #0xc
003c169c: mov      r4, r2
003c16a0: mov      r7, r0
003c16a4: mov      r5, r1
003c16a8: bl       #0x3c0084
003c16ac: ldr      r6, [pc, #0x174]
003c16b0: cmp      r0, #0
003c16b4: add      r6, pc, r6
003c16b8: bne      #0x3c16dc
003c16bc: ldr      r3, [pc, #0x168]
003c16c0: ldr      r3, [r6, r3]
003c16c4: ldr      r3, [r3]
003c16c8: cmp      r3, #2
003c16cc: streq    r0, [r0]
003c16d0: beq      #0x3c16dc
003c16d4: cmp      r3, #1
003c16d8: beq      #0x3c17c0
003c16dc: mov      r0, r7
003c16e0: mov      r1, r5
003c16e4: mov      r2, r4
003c16e8: bl       #0x3c00e0
003c16ec: cmp      r0, #0
003c16f0: bne      #0x3c1714
003c16f4: ldr      r3, [pc, #0x130]
003c16f8: ldr      r3, [r6, r3]
003c16fc: ldr      r3, [r3]
003c1700: cmp      r3, #2
003c1704: streq    r0, [r0]
003c1708: beq      #0x3c1714
003c170c: cmp      r3, #1
003c1710: beq      #0x3c17f4
003c1714: ldr      r3, [r7, #0xc]
003c1718: add      r7, r7, #8
003c171c: cmp      r3, #0
003c1720: beq      #0x3c1764
003c1724: mov      r1, r7
003c1728: b        #0x3c1730
003c172c: mov      r3, r2
003c1730: ldr      r2, [r3, #0x10]
003c1734: cmp      r5, r2
003c1738: ldrgt    r2, [r3, #0xc]
003c173c: ldrle    r2, [r3, #8]
003c1740: movgt    r3, r1
003c1744: mov      r1, r3
003c1748: cmp      r2, #0
003c174c: bne      #0x3c172c
003c1750: cmp      r7, r3
003c1754: beq      #0x3c1764
003c1758: ldr      r2, [r3, #0x10]
003c175c: cmp      r5, r2
003c1760: movge    r7, r3
003c1764: ldr      r3, [r7, #0x20]
003c1768: add      r5, r7, #0x1c
003c176c: cmp      r3, #0
003c1770: beq      #0x3c17b4
003c1774: mov      r1, r5
003c1778: b        #0x3c1780
003c177c: mov      r3, r2
003c1780: ldr      r2, [r3, #0x10]
003c1784: cmp      r2, r4
003c1788: ldrlt    r2, [r3, #0xc]
003c178c: ldrge    r2, [r3, #8]
003c1790: movlt    r3, r1
003c1794: mov      r1, r3
003c1798: cmp      r2, #0
003c179c: bne      #0x3c177c
003c17a0: cmp      r5, r3
003c17a4: beq      #0x3c17b4
003c17a8: ldr      r2, [r3, #0x10]
003c17ac: cmp      r2, r4
003c17b0: movle    r5, r3
003c17b4: add      r0, r5, #0x14
003c17b8: add      sp, sp, #0xc
003c17bc: pop      {r4, r5, r6, r7, pc}
003c17c0: ldr      r0, [pc, #0x68]
003c17c4: ldr      r1, [pc, #0x68]
003c17c8: ldr      r2, [pc, #0x68]
003c17cc: ldr      r0, [r6, r0]
003c17d0: ldr      r3, [pc, #0x64]
003c17d4: mov      ip, #0xd0
003c17d8: add      r1, pc, r1
003c17dc: add      r2, pc, r2
003c17e0: add      r3, pc, r3
003c17e4: add      r0, r0, #0xa8
003c17e8: str      ip, [sp]
003c17ec: bl       #0x30e004
003c17f0: b        #0x3c16dc
003c17f4: ldr      r0, [pc, #0x34]
003c17f8: ldr      r1, [pc, #0x40]
003c17fc: ldr      r2, [pc, #0x40]
003c1800: ldr      r0, [r6, r0]
003c1804: ldr      r3, [pc, #0x3c]
003c1808: mov      ip, #0xd1
003c180c: add      r1, pc, r1
003c1810: add      r2, pc, r2
003c1814: add      r3, pc, r3
003c1818: add      r0, r0, #0xa8
003c181c: str      ip, [sp]
003c1820: bl       #0x30e004
003c1824: b        #0x3c1714
003c1828: ldrsbeq  r3, [sp], #-0x3c
003c182c: andeq    r3, r0, r0, asr #19
003c1830: andeq    r1, r0, r0, asr #19
003c1834: subeq    ip, pc, r0, lsl #24
003c1838: subseq   r3, r0, r4, ror r4
003c183c: subseq   r3, r0, r8, lsl #8
003c1840: subeq    ip, pc, ip, asr #23
003c1844: subseq   r3, r0, r8, asr r4
003c1848: ldrsbeq  r3, [r0], #-0x34
