
# _ZN6CharAI12OnTargetDiedEv
003d1f60: push     {r4, r5, r6, r7, r8, lr}
003d1f64: ldr      r4, [pc, #0x98]
003d1f68: ldr      r6, [pc, #0x98]
003d1f6c: ldr      r2, [pc, #0x98]
003d1f70: add      r4, pc, r4
003d1f74: ldr      r3, [r4, r6]
003d1f78: ldr      r8, [r4, r2]
003d1f7c: sub      sp, sp, #0x20
003d1f80: ldr      r3, [r3]
003d1f84: mov      r7, r0
003d1f88: mov      r0, r8
003d1f8c: str      r3, [sp, #0x1c]
003d1f90: bl       #0x337888
003d1f94: ldr      r1, [pc, #0x74]
003d1f98: add      r5, sp, #4
003d1f9c: mov      r2, sp
003d1fa0: add      r1, pc, r1
003d1fa4: mov      r0, r5
003d1fa8: bl       #0x3140ec
003d1fac: mov      r1, r5
003d1fb0: mov      r0, r8
003d1fb4: bl       #0x337a88
003d1fb8: mov      r0, r5
003d1fbc: bl       #0x3139ac
003d1fc0: ldr      r3, [r7, #0x1c]
003d1fc4: mov      r2, #0
003d1fc8: strb     r2, [r7, #0x78]
003d1fcc: cmp      r3, r2
003d1fd0: beq      #0x3d1fe4
003d1fd4: mov      r0, r3
003d1fd8: ldr      r3, [r3]
003d1fdc: mov      lr, pc
003d1fe0: ldr      pc, [r3, #0x40]
003d1fe4: ldr      r3, [r4, r6]
003d1fe8: ldr      r2, [sp, #0x1c]
003d1fec: ldr      r3, [r3]
003d1ff0: cmp      r2, r3
003d1ff4: bne      #0x3d2000
003d1ff8: add      sp, sp, #0x20
003d1ffc: pop      {r4, r5, r6, r7, r8, pc}
003d2000: bl       #0x30e310
003d2004: subseq   r2, ip, r0, lsr #22
003d2008: andeq    r4, r0, ip, lsr #1
003d200c: andeq    r0, r0, r4, lsl #17
003d2010: subeq    r3, pc, r8, asr #10

# _ZN11AISExternal18OnTargetOutOfSightEv
003dce14: ldr      r1, [pc, #4]
003dce18: add      r1, pc, r1
003dce1c: b        #0x37c514
003dce20: subeq    r8, lr, r8, lsr #25

# _ZN11AISExternal15OnTargetInSightEv
003dce04: ldr      r1, [pc, #4]
003dce08: add      r1, pc, r1
003dce0c: b        #0x37c514
003dce10: subeq    r8, lr, r8, lsr #25

# _ZN9Character5CleanEv
003a66f8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003a66fc: ldr      r8, [pc, #0x110]
003a6700: ldr      sb, [pc, #0x110]
003a6704: add      sl, r0, #0x1480
003a6708: add      r8, pc, r8
003a670c: ldr      r6, [r8, sb]
003a6710: mov      r5, r0
003a6714: add      r1, sl, #4
003a6718: mov      r0, r6
003a671c: bl       #0x494978
003a6720: add      r1, sl, #0xc
003a6724: mov      r0, r6
003a6728: movw     r7, #0x1494
003a672c: bl       #0x494978
003a6730: ldr      r1, [r5, r7]
003a6734: cmp      r1, #0
003a6738: beq      #0x3a677c
003a673c: mov      r4, #0
003a6740: b        #0x3a6748
003a6744: ldr      r1, [r5, r7]
003a6748: add      r1, r1, r4
003a674c: mov      r0, r6
003a6750: add      r4, r4, #4
003a6754: bl       #0x494978
003a6758: cmp      r4, #0x24
003a675c: bne      #0x3a6744
003a6760: movw     r4, #0x1494
003a6764: ldr      r0, [r5, r4]
003a6768: cmp      r0, #0
003a676c: beq      #0x3a677c
003a6770: bl       #0x310440
003a6774: mov      r3, #0
003a6778: str      r3, [r5, r4]
003a677c: ldr      r0, [r8, sb]
003a6780: add      r1, sl, #0x1c
003a6784: bl       #0x494978
003a6788: mov      r0, r5
003a678c: bl       #0x3a4068
003a6790: movw     r3, #0x14e0
003a6794: ldr      r0, [r5, r3]
003a6798: cmp      r0, #0
003a679c: beq      #0x3a67a4
003a67a0: bl       #0x310440
003a67a4: ldrb     r3, [r5, #0x4f8]
003a67a8: cmp      r3, #0
003a67ac: bne      #0x3a6804
003a67b0: movw     r4, #0x14e8
003a67b4: ldr      r3, [r5, r4]
003a67b8: cmp      r3, #0
003a67bc: beq      #0x3a67d8
003a67c0: mov      r0, r3
003a67c4: ldr      r3, [r3]
003a67c8: mov      lr, pc
003a67cc: ldr      pc, [r3, #4]
003a67d0: mov      r3, #0
003a67d4: str      r3, [r5, r4]
003a67d8: movw     r4, #0x14ec
003a67dc: ldr      r3, [r5, r4]
003a67e0: cmp      r3, #0
003a67e4: beq      #0x3a6800
003a67e8: mov      r0, r3
003a67ec: ldr      r3, [r3]
003a67f0: mov      lr, pc
003a67f4: ldr      pc, [r3, #0x1c]
003a67f8: mov      r3, #0
003a67fc: str      r3, [r5, r4]
003a6800: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003a6804: add      r0, r5, #0x490
003a6808: add      r0, r0, #0xc
003a680c: bl       #0x3c91c8
003a6810: b        #0x3a67b0
003a6814: subseq   lr, lr, r8, lsl #7
003a6818: andeq    r1, r0, r8, lsl #22

# _ZN11AISExternal21OnTargetInRangedRangeEv
003dcdcc: ldr      r3, [r0, #0xb8]
003dcdd0: tst      r3, #8
003dcdd4: bxeq     lr
003dcdd8: ldr      r1, [pc, #4]
003dcddc: add      r1, pc, r1
003dcde0: b        #0x37c514
003dcde4: subeq    r8, lr, r4, lsr #25

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

# _ZN11AISExternal14OnEnemySpottedEP9Character
003dd2f4: push     {r4, r5, r6, lr}
003dd2f8: sub      sp, sp, #8
003dd2fc: mov      r5, r0
003dd300: mov      r6, r1
003dd304: mov      r0, sp
003dd308: bl       #0x3192b4
003dd30c: mov      r0, sp
003dd310: mov      r1, r6
003dd314: bl       #0x386f28
003dd318: ldr      r1, [pc, #0x20]
003dd31c: mov      r0, r5
003dd320: mov      r2, sp
003dd324: add      r1, pc, r1
003dd328: bl       #0x37c41c
003dd32c: mov      r0, sp
003dd330: mov      r4, sp
003dd334: bl       #0x319228
003dd338: add      sp, sp, #8
003dd33c: pop      {r4, r5, r6, pc}
003dd340: subeq    r8, lr, ip, lsr #16

# _ZN11AISExternal18OnTargetOutOfRangeEv
003dcde8: ldr      r3, [r0, #0xb8]
003dcdec: tst      r3, #4
003dcdf0: bxeq     lr
003dcdf4: ldr      r1, [pc, #4]
003dcdf8: add      r1, pc, r1
003dcdfc: b        #0x37c514
003dce00: subeq    r8, lr, r0, lsr #25

# _ZN11AISExternal20OnTargetInMeleeRangeEv
003dcd94: ldr      r3, [r0, #0xb8]
003dcd98: tst      r3, #0x20
003dcd9c: bxeq     lr
003dcda0: ldr      r1, [pc, #4]
003dcda4: add      r1, pc, r1
003dcda8: b        #0x37c514
003dcdac: subeq    r8, lr, ip, lsr #25

# _ZN11AISExternal12OnTargetDiedEv
003dce24: ldr      r1, [pc, #4]
003dce28: add      r1, pc, r1
003dce2c: b        #0x37c514
003dce30: strheq   r4, [lr], #-0xd8

# _ZN11AISExternal20OnTargetInCloseRangeEv
003dcdb0: ldr      r3, [r0, #0xb8]
003dcdb4: tst      r3, #0x10
003dcdb8: bxeq     lr
003dcdbc: ldr      r1, [pc, #4]
003dcdc0: add      r1, pc, r1
003dcdc4: b        #0x37c514
003dcdc8: subeq    r8, lr, r8, lsr #25
