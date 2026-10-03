
# _ZN6b2Body8SetXFormERK6b2Vec2f
007e164c: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
007e1650: ldr      r8, [r0, #0x58]
007e1654: mov      r3, #0x19000
007e1658: add      r3, r3, #0x1d4
007e165c: ldrb     r3, [r8, r3]
007e1660: mov      r4, r0
007e1664: mov      r5, r1
007e1668: cmp      r3, #0
007e166c: mov      r6, r2
007e1670: bne      #0x7e169c
007e1674: ldrh     r3, [r0]
007e1678: tst      r3, #2
007e167c: beq      #0x7e16a4
007e1680: mov      r0, #0
007e1684: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e1688: ldr      r8, [r4, #0x58]
007e168c: mov      r3, #0x19000
007e1690: add      r3, r3, #0x1d8
007e1694: ldr      r0, [r8, r3]
007e1698: bl       #0x7e2ac8
007e169c: mov      r0, #1
007e16a0: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
007e16a4: mov      r0, r2
007e16a8: bl       #0x30e754
007e16ac: mov      r7, r0
007e16b0: mov      r0, r6
007e16b4: bl       #0x30eb08
007e16b8: add      r2, r0, #0x80000000
007e16bc: str      r2, [r4, #0x14]
007e16c0: str      r7, [r4, #0xc]
007e16c4: str      r0, [r4, #0x10]
007e16c8: str      r7, [r4, #0x18]
007e16cc: ldr      r3, [r5]
007e16d0: ldr      sb, [r4, #0x1c]
007e16d4: mov      sl, r0
007e16d8: str      r3, [r4, #4]
007e16dc: ldr      r3, [r5, #4]
007e16e0: mov      r1, r7
007e16e4: mov      r0, sb
007e16e8: str      r3, [r4, #8]
007e16ec: mov      fp, r2
007e16f0: bl       #0x30ed6c
007e16f4: mov      r1, fp
007e16f8: mov      r5, r0
007e16fc: ldr      r0, [r4, #0x20]
007e1700: bl       #0x30ed6c
007e1704: mov      r1, r0
007e1708: mov      r0, r5
007e170c: bl       #0x30eba4
007e1710: mov      r1, sl
007e1714: mov      r5, r0
007e1718: mov      r0, sb
007e171c: bl       #0x30ed6c
007e1720: mov      r1, r7
007e1724: mov      sl, r0
007e1728: ldr      r0, [r4, #0x20]
007e172c: bl       #0x30ed6c
007e1730: mov      r1, r0
007e1734: mov      r0, sl
007e1738: bl       #0x30eba4
007e173c: ldr      r1, [r4, #4]
007e1740: mov      r7, r0
007e1744: mov      r0, r5
007e1748: bl       #0x30eba4
007e174c: ldr      r1, [r4, #8]
007e1750: mov      r5, r0
007e1754: mov      r0, r7
007e1758: bl       #0x30eba4
007e175c: str      r5, [r4, #0x2c]
007e1760: str      r0, [r4, #0x30]
007e1764: ldr      r5, [r4, #0x64]
007e1768: ldr      r2, [r4, #0x2c]
007e176c: ldr      r3, [r4, #0x30]
007e1770: cmp      r5, #0
007e1774: str      r2, [r4, #0x24]
007e1778: str      r3, [r4, #0x28]
007e177c: str      r6, [r4, #0x34]
007e1780: str      r6, [r4, #0x38]
007e1784: beq      #0x7e168c
007e1788: mov      r7, #0x19000
007e178c: add      r7, r7, #0x1d8
007e1790: add      r6, r4, #4
007e1794: b        #0x7e17a8
007e1798: ldr      r5, [r5, #8]
007e179c: cmp      r5, #0
007e17a0: beq      #0x7e1688
007e17a4: ldr      r8, [r4, #0x58]
007e17a8: ldr      r1, [r8, r7]
007e17ac: mov      r0, r5
007e17b0: mov      r2, r6
007e17b4: mov      r3, r6
007e17b8: bl       #0x7e63d0
007e17bc: cmp      r0, #0
007e17c0: bne      #0x7e1798
007e17c4: ldrh     r2, [r4]
007e17c8: ldr      r5, [r4, #0x64]
007e17cc: mov      r3, #0
007e17d0: orr      r2, r2, #2
007e17d4: cmp      r5, #0
007e17d8: strh     r2, [r4]
007e17dc: str      r3, [r4, #0x48]
007e17e0: str      r3, [r4, #0x40]
007e17e4: str      r3, [r4, #0x44]
007e17e8: beq      #0x7e1680
007e17ec: mov      r6, #0x19000
007e17f0: add      r6, r6, #0x1d8
007e17f4: ldr      r3, [r4, #0x58]
007e17f8: mov      r0, r5
007e17fc: ldr      r1, [r3, r6]
007e1800: bl       #0x7e6180
007e1804: ldr      r5, [r5, #8]
007e1808: cmp      r5, #0
007e180c: bne      #0x7e17f4
007e1810: mov      r0, #0
007e1814: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
