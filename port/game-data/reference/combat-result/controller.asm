
# _ZN9Character13F_ApplyResultERKNS_12AttackResultEPS_S3_b
003b10b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b10b8: ldr      r7, [pc, #0xcb4]
003b10bc: ldr      sb, [pc, #0xcb4]
003b10c0: mov      r4, r0
003b10c4: add      r7, pc, r7
003b10c8: ldr      r0, [r7, sb]
003b10cc: mov      r5, r2
003b10d0: sub      sp, sp, #0x15c
003b10d4: ldr      r2, [r0]
003b10d8: mov      r8, r3
003b10dc: mov      r6, r1
003b10e0: str      r2, [sp, #0x154]
003b10e4: bl       #0x7fd794
003b10e8: ldrb     r3, [r0, #5]
003b10ec: cmp      r3, #0
003b10f0: bne      #0x3b1440
003b10f4: ldrb     r3, [r4, #0x18]
003b10f8: ldr      fp, [pc, #0xc7c]
003b10fc: add      r8, sp, #0x13c
003b1100: tst      r3, #3
003b1104: movw     r3, #0x14d0
003b1108: ldrheq   r2, [r6, r3]
003b110c: ldr      sl, [r7, fp]
003b1110: movne    r2, #0
003b1114: addeq    r2, r2, #1
003b1118: strh     r2, [r6, r3]
003b111c: mov      r0, sl
003b1120: bl       #0x337888
003b1124: ldr      r1, [pc, #0xc54]
003b1128: add      r2, sp, #0x48
003b112c: mov      r0, r8
003b1130: add      r1, pc, r1
003b1134: bl       #0x3140ec
003b1138: mov      r0, sl
003b113c: mov      r1, r8
003b1140: bl       #0x337a88
003b1144: cmp      r0, #0
003b1148: beq      #0x3b14cc
003b114c: mov      r0, r8
003b1150: bl       #0x3139ac
003b1154: ldr      r1, [r4, #0x10]
003b1158: mov      r0, r6
003b115c: bl       #0x3bdca4
003b1160: mov      r0, r6
003b1164: ldr      r1, [r4, #0x14]
003b1168: bl       #0x3bdbb8
003b116c: ldr      r3, [r5]
003b1170: mov      r0, r5
003b1174: mov      lr, pc
003b1178: ldr      pc, [r3, #0x34]
003b117c: cmp      r0, #0
003b1180: beq      #0x3b1204
003b1184: mov      r0, r5
003b1188: bl       #0x3bc6b8
003b118c: mov      r0, r4
003b1190: mov      r1, r6
003b1194: mov      r2, r5
003b1198: bl       #0x3af77c
003b119c: mov      r0, r4
003b11a0: mov      r1, r6
003b11a4: mov      r2, r5
003b11a8: bl       #0x3afee0
003b11ac: ldr      r3, [r4, #0x1c]
003b11b0: tst      r3, #0x20000000
003b11b4: beq      #0x3b1788
003b11b8: ldr      r3, [r5]
003b11bc: mov      r0, r5
003b11c0: mov      lr, pc
003b11c4: ldr      pc, [r3, #0x28]
003b11c8: cmp      r0, #0
003b11cc: bne      #0x3b1758
003b11d0: ldr      r3, [r6]
003b11d4: mov      r0, r6
003b11d8: mov      lr, pc
003b11dc: ldr      pc, [r3, #0x28]
003b11e0: cmp      r0, #0
003b11e4: bne      #0x3b13e0
003b11e8: ldr      r3, [r7, sb]
003b11ec: ldr      r2, [sp, #0x154]
003b11f0: ldr      r3, [r3]
003b11f4: cmp      r2, r3
003b11f8: bne      #0x3b1d70
003b11fc: add      sp, sp, #0x15c
003b1200: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b1204: ldr      r1, [r4, #8]
003b1208: cmp      r1, #0
003b120c: ble      #0x3b122c
003b1210: ldr      r2, [r4, #0xc]
003b1214: cmp      r2, #0
003b1218: ble      #0x3b122c
003b121c: asr      r1, r1, #8
003b1220: add      r0, r5, #0x560
003b1224: ldr      r3, [r4, #4]
003b1228: bl       #0x3e2720
003b122c: ldr      r2, [r4, #0x1c]
003b1230: ldr      r3, [r5]
003b1234: mov      r0, r5
003b1238: tst      r2, #0x18000000
003b123c: moveq    ip, #0
003b1240: movne    ip, #1
003b1244: str      ip, [sp, #0xc]
003b1248: mov      lr, pc
003b124c: ldr      pc, [r3, #0x28]
003b1250: cmp      r0, #0
003b1254: bne      #0x3b17ec
003b1258: ldrb     r3, [r4, #0x18]
003b125c: tst      r3, #2
003b1260: bne      #0x3b1834
003b1264: tst      r3, #4
003b1268: bne      #0x3b18a4
003b126c: tst      r3, #0x10
003b1270: bne      #0x3b1914
003b1274: tst      r3, #0x80
003b1278: bne      #0x3b196c
003b127c: tst      r3, #0x40
003b1280: beq      #0x3b1304
003b1284: ldr      r3, [r4, #0x1c]
003b1288: add      r1, r6, #0xff0
003b128c: add      r1, r1, #4
003b1290: tst      r3, #0x1000
003b1294: movne    r2, #0xb9
003b1298: moveq    r2, #0x8c
003b129c: add      r0, r6, #0x560
003b12a0: bl       #0x3dedb4
003b12a4: mov      r1, r0
003b12a8: add      r0, r5, #0x4f0
003b12ac: mov      r3, r6
003b12b0: mov      ip, #0
003b12b4: asr      r1, r1, #8
003b12b8: mov      r2, #1
003b12bc: add      r0, r0, #0xc
003b12c0: str      ip, [sp]
003b12c4: bl       #0x3c5ffc
003b12c8: ldr      sl, [r7, fp]
003b12cc: add      r8, sp, #0x7c
003b12d0: mov      r0, sl
003b12d4: bl       #0x337888
003b12d8: ldr      r1, [pc, #0xaa4]
003b12dc: add      r2, sp, #0x28
003b12e0: mov      r0, r8
003b12e4: add      r1, pc, r1
003b12e8: bl       #0x3140ec
003b12ec: mov      r1, r8
003b12f0: mov      r0, sl
003b12f4: bl       #0x337a88
003b12f8: mov      r0, r8
003b12fc: bl       #0x3139ac
003b1300: ldrb     r3, [r4, #0x18]
003b1304: tst      r3, #0x20
003b1308: beq      #0x3b136c
003b130c: ldr      r3, [r4, #0x1c]
003b1310: add      r1, r6, #0xff0
003b1314: add      r1, r1, #4
003b1318: tst      r3, #0x4000
003b131c: movne    r2, #0xbb
003b1320: moveq    r2, #0x8f
003b1324: add      r0, r6, #0x560
003b1328: bl       #0x3dedb4
003b132c: asrs     r1, r0, #8
003b1330: bne      #0x3b19e0
003b1334: ldr      sl, [r7, fp]
003b1338: add      r8, sp, #0x64
003b133c: mov      r0, sl
003b1340: bl       #0x337888
003b1344: ldr      r1, [pc, #0xa3c]
003b1348: add      r2, sp, #0x24
003b134c: mov      r0, r8
003b1350: add      r1, pc, r1
003b1354: bl       #0x3140ec
003b1358: mov      r0, sl
003b135c: mov      r1, r8
003b1360: bl       #0x337a88
003b1364: mov      r0, r8
003b1368: bl       #0x3139ac
003b136c: ldrb     r3, [r4, #0x19]
003b1370: tst      r3, #1
003b1374: beq      #0x3b1184
003b1378: ldr      r3, [r4, #0x1c]
003b137c: add      r1, r6, #0xff0
003b1380: add      r1, r1, #4
003b1384: tst      r3, #0x10000
003b1388: movne    r2, #0xbd
003b138c: moveq    r2, #0x92
003b1390: add      r0, r6, #0x560
003b1394: bl       #0x3dedb4
003b1398: asr      r1, r0, #8
003b139c: add      r0, r5, #0x560
003b13a0: bl       #0x3e2a5c
003b13a4: ldr      sl, [r7, fp]
003b13a8: add      r8, sp, #0x4c
003b13ac: mov      r0, sl
003b13b0: bl       #0x337888
003b13b4: ldr      r1, [pc, #0x9d0]
003b13b8: add      r2, sp, #0x20
003b13bc: mov      r0, r8
003b13c0: add      r1, pc, r1
003b13c4: bl       #0x3140ec
003b13c8: mov      r0, sl
003b13cc: mov      r1, r8
003b13d0: bl       #0x337a88
003b13d4: mov      r0, r8
003b13d8: bl       #0x3139ac
003b13dc: b        #0x3b1184
003b13e0: ldr      r3, [pc, #0x9a8]
003b13e4: mov      r1, r6
003b13e8: mov      r2, #0
003b13ec: ldr      r3, [r7, r3]
003b13f0: ldr      r0, [r3, #0x40]
003b13f4: bl       #0x36eea8
003b13f8: ldrh     r3, [r4, #0x18]
003b13fc: ldr      r6, [r0, #0x670]
003b1400: tst      r3, #0x160
003b1404: bne      #0x3b1a4c
003b1408: ldr      r8, [pc, #0x984]
003b140c: mov      r0, r5
003b1410: ldr      r3, [r5]
003b1414: mov      lr, pc
003b1418: ldr      pc, [r3, #0x34]
003b141c: cmp      r0, #0
003b1420: bne      #0x3b1a38
003b1424: ldr      r2, [r4]
003b1428: ldr      r0, [r7, r8]
003b142c: mov      r3, r6
003b1430: asr      r2, r2, #8
003b1434: mov      r1, #1
003b1438: bl       #0x3790e0
003b143c: b        #0x3b11e8
003b1440: ldr      r3, [r6]
003b1444: mov      r0, r6
003b1448: mov      lr, pc
003b144c: ldr      pc, [r3, #0x54]
003b1450: cmp      r0, #0
003b1454: bne      #0x3b17c4
003b1458: cmp      r8, #0
003b145c: bne      #0x3b10f4
003b1460: cmp      r5, #0
003b1464: beq      #0x3b1aa8
003b1468: ldr      sl, [r5, #0x108]
003b146c: ldr      r8, [r6, #0x108]
003b1470: lsr      r3, sl, #0x1f
003b1474: orrs     r3, r3, r8, lsr #31
003b1478: beq      #0x3b14a0
003b147c: ldr      r3, [pc, #0x914]
003b1480: ldr      r3, [r7, r3]
003b1484: ldr      r3, [r3]
003b1488: cmp      r3, #2
003b148c: moveq    r3, #0
003b1490: streq    r3, [r3]
003b1494: beq      #0x3b14a0
003b1498: cmp      r3, #1
003b149c: beq      #0x3b1d08
003b14a0: bl       #0x80b1bc
003b14a4: mov      r1, sl
003b14a8: mov      fp, r0
003b14ac: mov      r2, r4
003b14b0: mov      r0, r8
003b14b4: mov      r3, #1
003b14b8: bl       #0x3af330
003b14bc: mov      r1, r0
003b14c0: mov      r0, fp
003b14c4: bl       #0x80e2a4
003b14c8: b        #0x3b10f4
003b14cc: ldr      r3, [pc, #0x8c8]
003b14d0: add      ip, sp, #0x124
003b14d4: mov      r0, sl
003b14d8: add      r3, pc, r3
003b14dc: str      ip, [sp, #0xc]
003b14e0: str      r3, [sp, #8]
003b14e4: bl       #0x337888
003b14e8: ldr      r3, [sp, #8]
003b14ec: add      r2, sp, #0x44
003b14f0: ldr      r0, [sp, #0xc]
003b14f4: mov      r1, r3
003b14f8: bl       #0x3140ec
003b14fc: mov      r0, sl
003b1500: ldr      r1, [sp, #0xc]
003b1504: bl       #0x337a88
003b1508: cmp      r0, #0
003b150c: ldr      r3, [sp, #8]
003b1510: beq      #0x3b17d0
003b1514: ldr      r3, [r5]
003b1518: mov      r0, r5
003b151c: mov      lr, pc
003b1520: ldr      pc, [r3, #0x28]
003b1524: cmp      r0, #0
003b1528: bne      #0x3b1a2c
003b152c: movw     r3, #0x14f0
003b1530: ldrb     r3, [r5, r3]
003b1534: cmp      r3, #0
003b1538: bne      #0x3b1a2c
003b153c: ldr      r0, [sp, #0xc]
003b1540: bl       #0x3139ac
003b1544: mov      r0, r8
003b1548: bl       #0x3139ac
003b154c: ldr      r3, [r4, #0x1c]
003b1550: tst      r3, #0x400000
003b1554: bne      #0x3b1a00
003b1558: ldr      r8, [r4]
003b155c: cmp      r8, #0
003b1560: ble      #0x3b1154
003b1564: ldr      r2, [pc, #0x824]
003b1568: ldr      r3, [r7, r2]
003b156c: str      r2, [sp, #0xc]
003b1570: ldr      r3, [r3, #0x40]
003b1574: ldr      sl, [r3, #0x6c4]
003b1578: cmp      sl, #1
003b157c: ble      #0x3b15c4
003b1580: mov      r0, r6
003b1584: bl       #0x3a3064
003b1588: cmp      r0, #0
003b158c: beq      #0x3b1ad8
003b1590: sub      r0, sl, #1
003b1594: bl       #0x30e964
003b1598: ldr      r3, [pc, #0x800]
003b159c: ldr      r3, [r7, r3]
003b15a0: ldr      r3, [r3]
003b15a4: ldr      r1, [r3, #0x34]
003b15a8: bl       #0x30ed6c
003b15ac: mov      r1, #0x3f800000
003b15b0: bl       #0x30eba4
003b15b4: bl       #0x30e4cc
003b15b8: ldr      r8, [r4]
003b15bc: mul      r8, r8, r0
003b15c0: str      r8, [r4]
003b15c4: mov      r0, r6
003b15c8: bl       #0x3bd394
003b15cc: mov      sl, r0
003b15d0: ldr      r0, [r4]
003b15d4: bl       #0x30e964
003b15d8: mov      r1, #0x3b800000
003b15dc: bl       #0x30ed6c
003b15e0: mov      r1, r0
003b15e4: mov      r0, sl
003b15e8: bl       #0x30ed6c
003b15ec: add      sl, r5, #0x3c8
003b15f0: mov      r2, r0
003b15f4: mov      r1, r6
003b15f8: mov      r0, sl
003b15fc: bl       #0x3d7c68
003b1600: mov      r1, #0
003b1604: bl       #0x30e2f8
003b1608: cmp      r0, #0
003b160c: bne      #0x3b1a64
003b1610: ldrb     r3, [r4, #0x18]
003b1614: ldr      r2, [r5, #0x110]
003b1618: and      r3, r3, #0x80
003b161c: uxtb     r3, r3
003b1620: cmp      r3, #0
003b1624: ldrne    r3, [r4, #0x1c]
003b1628: ubfxne   r3, r3, #0x14, #1
003b162c: cmn      r2, #1
003b1630: strb     r3, [r5, #0x53b]
003b1634: beq      #0x3b1b2c
003b1638: ldr      r3, [r4, #0x1c]
003b163c: tst      r3, #0x200000
003b1640: beq      #0x3b1680
003b1644: ldr      r8, [r4, #0x24]
003b1648: cmn      r8, #1
003b164c: addne    r8, r8, #0x7c
003b1650: beq      #0x3b1cd0
003b1654: mov      r0, r5
003b1658: bl       #0x3935dc
003b165c: ldr      r3, [pc, #0x740]
003b1660: mov      ip, #0
003b1664: mov      r2, r0
003b1668: mov      r1, r8
003b166c: ldr      r0, [r7, r3]
003b1670: add      r3, r5, #0x16c
003b1674: str      ip, [sp, #4]
003b1678: str      ip, [sp]
003b167c: bl       #0x495888
003b1680: ldr      r3, [r5]
003b1684: mov      r0, r5
003b1688: mov      lr, pc
003b168c: ldr      pc, [r3, #0x34]
003b1690: cmp      r0, #0
003b1694: beq      #0x3b16b4
003b1698: ldrb     r3, [r4, #0x18]
003b169c: ldrb     r2, [r4, #0x19]
003b16a0: and      r3, r3, #0xbf
003b16a4: bfc      r2, #0, #1
003b16a8: bfc      r3, #5, #1
003b16ac: strb     r2, [r4, #0x19]
003b16b0: strb     r3, [r4, #0x18]
003b16b4: ldrb     r3, [r4, #0x18]
003b16b8: tst      r3, #8
003b16bc: beq      #0x3b1154
003b16c0: ldr      r3, [r6]
003b16c4: mov      r0, r6
003b16c8: mov      lr, pc
003b16cc: ldr      pc, [r3, #0x28]
003b16d0: cmp      r0, #0
003b16d4: beq      #0x3b1154
003b16d8: ldr      r3, [sp, #0xc]
003b16dc: ldr      r0, [r7, r3]
003b16e0: bl       #0x31f594
003b16e4: cmp      r0, #0
003b16e8: beq      #0x3b1154
003b16ec: ldr      r8, [r0, #0x128]
003b16f0: cmp      r8, #0
003b16f4: beq      #0x3b1154
003b16f8: mov      r0, r8
003b16fc: mov      r1, r6
003b1700: bl       #0x40f980
003b1704: cmp      r0, #0
003b1708: beq      #0x3b1154
003b170c: mov      r3, #0
003b1710: mov      r0, r6
003b1714: add      r1, sp, #0x14
003b1718: str      r3, [sp, #0x1c]
003b171c: str      r3, [sp, #0x14]
003b1720: str      r3, [sp, #0x18]
003b1724: bl       #0x393ae4
003b1728: ldr      r3, [pc, #0x678]
003b172c: ldr      r1, [r8, #0x80]
003b1730: mov      ip, #0x1c
003b1734: ldr      r3, [r7, r3]
003b1738: mov      r0, r8
003b173c: mov      r2, #0
003b1740: ldr      lr, [r3]
003b1744: mov      r3, #1
003b1748: mla      r1, ip, r1, lr
003b174c: ldr      r1, [r1, #0xc]
003b1750: bl       #0x40f904
003b1754: b        #0x3b1154
003b1758: ldr      r3, [pc, #0x630]
003b175c: mov      r1, r5
003b1760: mov      r2, #0
003b1764: ldr      r3, [r7, r3]
003b1768: ldr      r8, [pc, #0x624]
003b176c: ldr      r0, [r3, #0x40]
003b1770: bl       #0x36eea8
003b1774: mov      r1, #3
003b1778: ldr      r2, [r0, #0x670]
003b177c: ldr      r0, [r7, r8]
003b1780: bl       #0x3790ec
003b1784: b        #0x3b11d0
003b1788: add      r0, r6, #0x3c8
003b178c: mov      r1, r6
003b1790: mov      r2, r5
003b1794: mov      r3, r4
003b1798: ldr      ip, [r6, #0x3c8]
003b179c: mov      lr, pc
003b17a0: ldr      pc, [ip, #0xb4]
003b17a4: ldr      ip, [r5, #0x3c8]
003b17a8: add      r0, r5, #0x3c8
003b17ac: mov      r1, r6
003b17b0: mov      r2, r5
003b17b4: mov      r3, r4
003b17b8: mov      lr, pc
003b17bc: ldr      pc, [ip, #0xb4]
003b17c0: b        #0x3b11b8
003b17c4: cmp      r8, #0
003b17c8: beq      #0x3b11e8
003b17cc: b        #0x3b10f4
003b17d0: mov      r1, r3
003b17d4: ldr      r3, [pc, #0x5b4]
003b17d8: ldr      r0, [r7, r3]
003b17dc: bl       #0x320e14
003b17e0: cmp      r0, #0
003b17e4: beq      #0x3b152c
003b17e8: b        #0x3b1514
003b17ec: add      r0, r5, #0x4f0
003b17f0: add      r0, r0, #0xc
003b17f4: mov      r1, #0
003b17f8: bl       #0x3c0260
003b17fc: cmp      r0, #0
003b1800: beq      #0x3b1258
003b1804: ldrb     r3, [r4, #0x18]
003b1808: tst      r3, #0x16
003b180c: bne      #0x3b125c
003b1810: ldr      r2, [r4]
003b1814: cmp      r2, #0
003b1818: orrle    r3, r3, #2
003b181c: orrgt    r3, r3, #0x10
003b1820: strble   r3, [r4, #0x18]
003b1824: uxtble   r3, r3
003b1828: strbgt   r3, [r4, #0x18]
003b182c: tst      r3, #2
003b1830: beq      #0x3b1264
003b1834: add      r0, r5, #0x4f0
003b1838: add      r0, r0, #0xc
003b183c: mov      r1, r6
003b1840: mov      r2, #0
003b1844: bl       #0x3c5b3c
003b1848: ldr      r3, [r5]
003b184c: mov      r0, r5
003b1850: mov      lr, pc
003b1854: ldr      pc, [r3, #0x28]
003b1858: cmp      r0, #0
003b185c: bne      #0x3b1bf0
003b1860: ldr      sl, [r7, fp]
003b1864: add      r8, sp, #0xdc
003b1868: mov      r0, sl
003b186c: bl       #0x337888
003b1870: ldr      r1, [pc, #0x534]
003b1874: add      r2, sp, #0x38
003b1878: mov      r0, r8
003b187c: add      r1, pc, r1
003b1880: bl       #0x3140ec
003b1884: mov      r1, r8
003b1888: mov      r0, sl
003b188c: bl       #0x337a88
003b1890: mov      r0, r8
003b1894: bl       #0x3139ac
003b1898: ldrb     r3, [r4, #0x18]
003b189c: tst      r3, #4
003b18a0: beq      #0x3b126c
003b18a4: add      r0, r5, #0x4f0
003b18a8: add      r0, r0, #0xc
003b18ac: mov      r1, r6
003b18b0: mov      r2, #0
003b18b4: bl       #0x3c5c60
003b18b8: ldr      r3, [r5]
003b18bc: mov      r0, r5
003b18c0: mov      lr, pc
003b18c4: ldr      pc, [r3, #0x28]
003b18c8: cmp      r0, #0
003b18cc: bne      #0x3b1b80
003b18d0: ldr      sl, [r7, fp]
003b18d4: add      r8, sp, #0xc4
003b18d8: mov      r0, sl
003b18dc: bl       #0x337888
003b18e0: ldr      r1, [pc, #0x4c8]
003b18e4: add      r2, sp, #0x34
003b18e8: mov      r0, r8
003b18ec: add      r1, pc, r1
003b18f0: bl       #0x3140ec
003b18f4: mov      r1, r8
003b18f8: mov      r0, sl
003b18fc: bl       #0x337a88
003b1900: mov      r0, r8
003b1904: bl       #0x3139ac
003b1908: ldrb     r3, [r4, #0x18]
003b190c: tst      r3, #0x10
003b1910: beq      #0x3b1274
003b1914: add      r0, r5, #0x4f0
003b1918: mov      r1, r6
003b191c: ldr      r2, [sp, #0xc]
003b1920: add      r0, r0, #0xc
003b1924: bl       #0x3c5d84
003b1928: ldr      sl, [r7, fp]
003b192c: add      r8, sp, #0xac
003b1930: mov      r0, sl
003b1934: bl       #0x337888
003b1938: ldr      r1, [pc, #0x474]
003b193c: add      r2, sp, #0x30
003b1940: mov      r0, r8
003b1944: add      r1, pc, r1
003b1948: bl       #0x3140ec
003b194c: mov      r1, r8
003b1950: mov      r0, sl
003b1954: bl       #0x337a88
003b1958: mov      r0, r8
003b195c: bl       #0x3139ac
003b1960: ldrb     r3, [r4, #0x18]
003b1964: tst      r3, #0x80
003b1968: beq      #0x3b127c
003b196c: ldr      r1, [r4, #0x1c]
003b1970: add      r0, r5, #0x4f0
003b1974: add      r0, r0, #0xc
003b1978: ubfx     r1, r1, #0x14, #1
003b197c: mov      r2, r6
003b1980: ldr      r3, [sp, #0xc]
003b1984: bl       #0x3c5ea0
003b1988: ldr      r3, [r5]
003b198c: mov      r0, r5
003b1990: mov      lr, pc
003b1994: ldr      pc, [r3, #0x28]
003b1998: cmp      r0, #0
003b199c: bne      #0x3b1c60
003b19a0: ldr      sl, [r7, fp]
003b19a4: add      r8, sp, #0x94
003b19a8: mov      r0, sl
003b19ac: bl       #0x337888
003b19b0: ldr      r1, [pc, #0x400]
003b19b4: add      r2, sp, #0x2c
003b19b8: mov      r0, r8
003b19bc: add      r1, pc, r1
003b19c0: bl       #0x3140ec
003b19c4: mov      r1, r8
003b19c8: mov      r0, sl
003b19cc: bl       #0x337a88
003b19d0: mov      r0, r8
003b19d4: bl       #0x3139ac
003b19d8: ldrb     r3, [r4, #0x18]
003b19dc: b        #0x3b127c
003b19e0: ldr      ip, [sp, #0xc]
003b19e4: add      r0, r5, #0x4f0
003b19e8: add      r0, r0, #0xc
003b19ec: mov      r2, #1
003b19f0: mov      r3, r6
003b19f4: str      ip, [sp]
003b19f8: bl       #0x3c6144
003b19fc: b        #0x3b1334
003b1a00: ldr      r3, [r6, #0x39c]
003b1a04: ldr      r2, [r4]
003b1a08: add      r0, r6, #0x37c
003b1a0c: lsl      r3, r3, #8
003b1a10: cmp      r3, r2
003b1a14: movge    r3, r2
003b1a18: asr      r1, r3, #8
003b1a1c: str      r3, [r4]
003b1a20: rsb      r1, r1, #0
003b1a24: bl       #0x3fe164
003b1a28: b        #0x3b1558
003b1a2c: ldr      r0, [sp, #0xc]
003b1a30: bl       #0x3139ac
003b1a34: b        #0x3b114c
003b1a38: ldr      r0, [r7, r8]
003b1a3c: mov      r1, #0
003b1a40: mov      r2, r6
003b1a44: bl       #0x3790ec
003b1a48: b        #0x3b1424
003b1a4c: ldr      r8, [pc, #0x340]
003b1a50: mov      r1, #2
003b1a54: mov      r2, r6
003b1a58: ldr      r0, [r7, r8]
003b1a5c: bl       #0x3790ec
003b1a60: b        #0x3b140c
003b1a64: ldr      r3, [r7, fp]
003b1a68: add      sl, sp, #0x10c
003b1a6c: mov      r0, r3
003b1a70: str      r3, [sp, #8]
003b1a74: bl       #0x337888
003b1a78: ldr      r1, [pc, #0x33c]
003b1a7c: add      r2, sp, #0x40
003b1a80: mov      r0, sl
003b1a84: add      r1, pc, r1
003b1a88: bl       #0x3140ec
003b1a8c: ldr      r3, [sp, #8]
003b1a90: mov      r1, sl
003b1a94: mov      r0, r3
003b1a98: bl       #0x337a88
003b1a9c: mov      r0, sl
003b1aa0: bl       #0x3139ac
003b1aa4: b        #0x3b1610
003b1aa8: ldr      r3, [pc, #0x2e8]
003b1aac: ldr      r3, [r7, r3]
003b1ab0: ldr      r3, [r3]
003b1ab4: cmp      r3, #2
003b1ab8: streq    r5, [r5]
003b1abc: beq      #0x3b1ac8
003b1ac0: cmp      r3, #1
003b1ac4: beq      #0x3b1d3c
003b1ac8: ldr      r8, [r6, #0x108]
003b1acc: mov      r3, #1
003b1ad0: mvn      sl, #0
003b1ad4: b        #0x3b1474
003b1ad8: ldr      r3, [r6]
003b1adc: mov      r0, r6
003b1ae0: mov      lr, pc
003b1ae4: ldr      pc, [r3, #0x28]
003b1ae8: cmp      r0, #0
003b1aec: beq      #0x3b15c4
003b1af0: sub      r0, sl, #1
003b1af4: bl       #0x30e964
003b1af8: ldr      r3, [pc, #0x2a0]
003b1afc: ldr      r3, [r7, r3]
003b1b00: ldr      r3, [r3]
003b1b04: ldr      r1, [r3, #0x38]
003b1b08: bl       #0x30ed6c
003b1b0c: mov      r1, #0x3f800000
003b1b10: bl       #0x30eba4
003b1b14: bl       #0x30e4cc
003b1b18: mov      r1, r0
003b1b1c: mov      r0, r8
003b1b20: bl       #0x30e2a4
003b1b24: mov      r8, r0
003b1b28: b        #0x3b15c4
003b1b2c: ldr      r3, [r7, fp]
003b1b30: add      sl, sp, #0xf4
003b1b34: mov      r0, r3
003b1b38: str      r3, [sp, #8]
003b1b3c: bl       #0x337888
003b1b40: ldr      r1, [pc, #0x278]
003b1b44: add      r2, sp, #0x3c
003b1b48: mov      r0, sl
003b1b4c: add      r1, pc, r1
003b1b50: bl       #0x3140ec
003b1b54: ldr      r3, [sp, #8]
003b1b58: mov      r1, sl
003b1b5c: mov      r0, r3
003b1b60: bl       #0x337a88
003b1b64: mov      r0, sl
003b1b68: bl       #0x3139ac
003b1b6c: mov      r0, r5
003b1b70: mov      r1, r8
003b1b74: mov      r2, r6
003b1b78: bl       #0x3a8bc4
003b1b7c: b        #0x3b1638
003b1b80: add      r8, r5, #0x560
003b1b84: mov      r0, r8
003b1b88: mov      r1, #0xd6
003b1b8c: mov      r2, #1
003b1b90: bl       #0x3e0798
003b1b94: ldr      r3, [pc, #0x228]
003b1b98: mov      r0, r8
003b1b9c: mov      r1, #0xd6
003b1ba0: ldr      r3, [r7, r3]
003b1ba4: mov      r2, #0
003b1ba8: ldr      r8, [r3]
003b1bac: bl       #0x3df6e0
003b1bb0: cmp      r0, #0x1f4
003b1bb4: blt      #0x3b18d0
003b1bb8: ldr      r3, [pc, #0x1d0]
003b1bbc: mov      r1, r5
003b1bc0: ldr      r3, [r7, r3]
003b1bc4: ldr      r0, [r3, #0x40]
003b1bc8: bl       #0x36effc
003b1bcc: cmp      r0, #0
003b1bd0: beq      #0x3b18d0
003b1bd4: ldr      r0, [pc, #0x1ec]
003b1bd8: add      r0, pc, r0
003b1bdc: bl       #0x3a3f70
003b1be0: mov      r1, r0
003b1be4: mov      r0, r8
003b1be8: bl       #0x3813b8
003b1bec: b        #0x3b18d0
003b1bf0: add      r8, r5, #0x560
003b1bf4: mov      r0, r8
003b1bf8: mov      r1, #0xd7
003b1bfc: mov      r2, #1
003b1c00: bl       #0x3e0798
003b1c04: ldr      r3, [pc, #0x1b8]
003b1c08: mov      r0, r8
003b1c0c: mov      r1, #0xd7
003b1c10: ldr      r3, [r7, r3]
003b1c14: mov      r2, #0
003b1c18: ldr      r8, [r3]
003b1c1c: bl       #0x3df6e0
003b1c20: cmp      r0, #0x1f4
003b1c24: blt      #0x3b1860
003b1c28: ldr      r3, [pc, #0x160]
003b1c2c: mov      r1, r5
003b1c30: ldr      r3, [r7, r3]
003b1c34: ldr      r0, [r3, #0x40]
003b1c38: bl       #0x36effc
003b1c3c: cmp      r0, #0
003b1c40: beq      #0x3b1860
003b1c44: ldr      r0, [pc, #0x180]
003b1c48: add      r0, pc, r0
003b1c4c: bl       #0x3a3f70
003b1c50: mov      r1, r0
003b1c54: mov      r0, r8
003b1c58: bl       #0x3813b8
003b1c5c: b        #0x3b1860
003b1c60: add      r8, r5, #0x560
003b1c64: mov      r0, r8
003b1c68: mov      r1, #0xde
003b1c6c: mov      r2, #1
003b1c70: bl       #0x3e0798
003b1c74: ldr      r3, [pc, #0x148]
003b1c78: mov      r0, r8
003b1c7c: mov      r1, #0xde
003b1c80: ldr      r3, [r7, r3]
003b1c84: mov      r2, #0
003b1c88: ldr      r8, [r3]
003b1c8c: bl       #0x3df6e0
003b1c90: cmp      r0, #0x31
003b1c94: ble      #0x3b19a0
003b1c98: ldr      r3, [pc, #0xf0]
003b1c9c: mov      r1, r5
003b1ca0: ldr      r3, [r7, r3]
003b1ca4: ldr      r0, [r3, #0x40]
003b1ca8: bl       #0x36effc
003b1cac: cmp      r0, #0
003b1cb0: beq      #0x3b19a0
003b1cb4: ldr      r0, [pc, #0x114]
003b1cb8: add      r0, pc, r0
003b1cbc: bl       #0x3a3f70
003b1cc0: mov      r1, r0
003b1cc4: mov      r0, r8
003b1cc8: bl       #0x3813b8
003b1ccc: b        #0x3b19a0
003b1cd0: ldr      r3, [r5]
003b1cd4: mov      r0, r5
003b1cd8: mov      lr, pc
003b1cdc: ldr      pc, [r3, #0x34]
003b1ce0: cmp      r0, #0
003b1ce4: beq      #0x3b1cf8
003b1ce8: mov      r0, r5
003b1cec: bl       #0x3a3368
003b1cf0: mov      r8, r0
003b1cf4: b        #0x3b1654
003b1cf8: mov      r0, r5
003b1cfc: bl       #0x3a33d0
003b1d00: mov      r8, r0
003b1d04: b        #0x3b1654
003b1d08: ldr      r0, [pc, #0xc4]
003b1d0c: ldr      r1, [pc, #0xc4]
003b1d10: ldr      r2, [pc, #0xc4]
003b1d14: ldr      r0, [r7, r0]
003b1d18: ldr      r3, [pc, #0xc0]
003b1d1c: movw     ip, #0x2d2
003b1d20: add      r1, pc, r1
003b1d24: add      r2, pc, r2
003b1d28: add      r3, pc, r3
003b1d2c: add      r0, r0, #0xa8
003b1d30: str      ip, [sp]
003b1d34: bl       #0x30e004
003b1d38: b        #0x3b14a0
003b1d3c: ldr      r0, [pc, #0x90]
003b1d40: ldr      r1, [pc, #0x9c]
003b1d44: ldr      r2, [pc, #0x9c]
003b1d48: ldr      r0, [r7, r0]
003b1d4c: ldr      r3, [pc, #0x98]
003b1d50: mov      ip, #0x2c8
003b1d54: add      r1, pc, r1
003b1d58: add      r2, pc, r2
003b1d5c: add      r3, pc, r3
003b1d60: add      r0, r0, #0xa8
003b1d64: str      ip, [sp]
003b1d68: bl       #0x30e004
003b1d6c: b        #0x3b1ac8
003b1d70: bl       #0x30e310
003b1d74: subseq   r3, lr, ip, asr #19
003b1d78: andeq    r4, r0, ip, lsr #1
003b1d7c: andeq    r0, r0, r4, lsl #17
003b1d80: subseq   r2, r1, r0, ror fp
003b1d84: ldrsbeq  r2, [r1], #-0x94
003b1d88: subseq   r2, r1, r8, ror #18
003b1d8c: ldrsheq  r2, [r1], #-0x88
003b1d90: strdeq   r3, r4, [r0], -r4
003b1d94: andeq    r2, r0, r4, lsl r7
003b1d98: andeq    r3, r0, r0, asr #19
003b1d9c: ldrsbeq  r2, [r1], #-0x78
003b1da0: andeq    r3, r0, r8, asr #5
003b1da4: andeq    r1, r0, r8, lsl #22
003b1da8: ldrdeq   r3, r4, [r0], -r4
003b1dac: subseq   r2, r1, ip, lsr r4
003b1db0: subseq   r2, r1, ip, asr #7
003b1db4: subseq   r2, r1, r4, ror r3
003b1db8: ldrsheq  r2, [r1], #-0x2c
003b1dbc: subseq   r2, r1, ip, asr r2
003b1dc0: subseq   r2, r1, ip, ror #2
003b1dc4: andeq    r1, r0, r0, ror sp
003b1dc8: subseq   r2, r1, r0, lsr r1
003b1dcc: ldrheq   r2, [r1], #-0
003b1dd0: subseq   r2, r1, r0, rrx
003b1dd4: andeq    r1, r0, r0, asr #19
003b1dd8: ldrheq   ip, [r0], #-0x68
003b1ddc: subseq   r1, r1, r4, asr #30
003b1de0: subseq   r1, r1, r8, ror #29
003b1de4: subseq   ip, r0, r4, lsl #13
003b1de8: subseq   r1, r1, r0, lsr #29
003b1dec: ldrheq   r1, [r1], #-0xe4

# _Z20PushProfilingContextPKc
003136b4: bx       lr

# _ZN9Character13F_MeleeAttackERNS_12AttackResultEPS_S2_bb
003b3368: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b336c: ldr      r4, [pc, #0x240]
003b3370: ldr      ip, [pc, #0x240]
003b3374: subs     r5, r1, #0
003b3378: add      r4, pc, r4
003b337c: ldr      r1, [r4, ip]
003b3380: sub      sp, sp, #0x4c
003b3384: mov      r6, r2
003b3388: str      r3, [sp, #0x14]
003b338c: ldr      r2, [r1]
003b3390: ldrb     r3, [sp, #0x70]
003b3394: str      ip, [sp, #0x18]
003b3398: str      r0, [sp, #0x20]
003b339c: str      r3, [sp, #0x1c]
003b33a0: str      r2, [sp, #0x44]
003b33a4: beq      #0x3b355c
003b33a8: cmp      r6, #0
003b33ac: beq      #0x3b3508
003b33b0: ldr      r3, [pc, #0x204]
003b33b4: add      r7, sp, #0x2c
003b33b8: ldr      r8, [r4, r3]
003b33bc: mov      r0, r8
003b33c0: bl       #0x337888
003b33c4: ldr      r1, [pc, #0x1f4]
003b33c8: add      r2, sp, #0x28
003b33cc: mov      r0, r7
003b33d0: add      r1, pc, r1
003b33d4: bl       #0x3140ec
003b33d8: mov      r1, r7
003b33dc: mov      r0, r8
003b33e0: bl       #0x337a88
003b33e4: mov      r0, r7
003b33e8: bl       #0x3139ac
003b33ec: ldr      ip, [sp, #0x14]
003b33f0: ldr      r2, [sp, #0x1c]
003b33f4: movw     r3, #0xaab5
003b33f8: cmp      ip, #0
003b33fc: movt     r3, #0x22
003b3400: movne    r1, #2
003b3404: moveq    r1, #1
003b3408: movw     r7, #0x554a
003b340c: cmp      r2, #0
003b3410: add      r0, r5, #0x37c
003b3414: movt     r7, #5
003b3418: moveq    r7, r3
003b341c: bl       #0x3ffe3c
003b3420: cmp      r0, #0
003b3424: mvneq    r3, #0
003b3428: streq    r3, [sp, #0x24]
003b342c: beq      #0x3b343c
003b3430: bl       #0x3f9e08
003b3434: ldr      r0, [r0, #0x94]
003b3438: str      r0, [sp, #0x24]
003b343c: ldr      r3, [pc, #0x180]
003b3440: ldr      r3, [r4, r3]
003b3444: ldr      sl, [r3]
003b3448: cmp      sl, #0
003b344c: beq      #0x3b34b0
003b3450: ldr      r3, [pc, #0x170]
003b3454: ldr      fp, [pc, #0x170]
003b3458: mov      r8, #0
003b345c: ldr      r3, [r4, r3]
003b3460: add      fp, pc, fp
003b3464: ldr      sb, [r3]
003b3468: b        #0x3b3478
003b346c: add      r8, r8, #1
003b3470: cmp      r8, sl
003b3474: beq      #0x3b34b0
003b3478: mov      r0, fp
003b347c: ldr      r1, [sb, r8, lsl #2]
003b3480: bl       #0x30e31c
003b3484: cmp      r0, #0
003b3488: bne      #0x3b346c
003b348c: cmn      r8, #1
003b3490: beq      #0x3b34b0
003b3494: ldr      ip, [sp, #0x1c]
003b3498: cmp      ip, #0
003b349c: beq      #0x3b34b0
003b34a0: mov      r2, r0
003b34a4: mov      r1, r8
003b34a8: add      r0, r5, #0x560
003b34ac: bl       #0x3df3b8
003b34b0: ldr      r2, [sp, #0x14]
003b34b4: ldr      ip, [sp, #0x24]
003b34b8: ldr      r0, [sp, #0x20]
003b34bc: cmp      r2, #0
003b34c0: orrne    r7, r7, #0x4000000
003b34c4: str      ip, [sp]
003b34c8: mvn      ip, #0
003b34cc: mov      r2, r6
003b34d0: mov      r3, r7
003b34d4: str      ip, [sp, #4]
003b34d8: mov      r1, r5
003b34dc: mov      ip, #0
003b34e0: str      ip, [sp, #8]
003b34e4: bl       #0x3b2638
003b34e8: ldr      r2, [sp, #0x18]
003b34ec: ldr      r3, [r4, r2]
003b34f0: ldr      r2, [sp, #0x44]
003b34f4: ldr      r3, [r3]
003b34f8: cmp      r2, r3
003b34fc: bne      #0x3b35b0
003b3500: add      sp, sp, #0x4c
003b3504: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b3508: ldr      r3, [pc, #0xc0]
003b350c: ldr      r3, [r4, r3]
003b3510: ldr      r3, [r3]
003b3514: cmp      r3, #2
003b3518: streq    r6, [r6]
003b351c: beq      #0x3b33b0
003b3520: cmp      r3, #1
003b3524: bne      #0x3b33b0
003b3528: ldr      r0, [pc, #0xa4]
003b352c: ldr      r1, [pc, #0xa4]
003b3530: ldr      r2, [pc, #0xa4]
003b3534: ldr      r0, [r4, r0]
003b3538: ldr      r3, [pc, #0xa0]
003b353c: movw     ip, #0x249
003b3540: add      r1, pc, r1
003b3544: add      r2, pc, r2
003b3548: add      r3, pc, r3
003b354c: add      r0, r0, #0xa8
003b3550: str      ip, [sp]
003b3554: bl       #0x30e004
003b3558: b        #0x3b33b0
003b355c: ldr      r3, [pc, #0x6c]
003b3560: ldr      r3, [r4, r3]
003b3564: ldr      r3, [r3]
003b3568: cmp      r3, #2
003b356c: streq    r5, [r5]
003b3570: beq      #0x3b33a8
003b3574: cmp      r3, #1
003b3578: bne      #0x3b33a8
003b357c: ldr      r0, [pc, #0x50]
003b3580: ldr      r1, [pc, #0x5c]
003b3584: ldr      r2, [pc, #0x5c]
003b3588: ldr      r0, [r4, r0]
003b358c: ldr      r3, [pc, #0x58]
003b3590: mov      ip, #0x248
003b3594: add      r1, pc, r1
003b3598: add      r2, pc, r2
003b359c: add      r3, pc, r3
003b35a0: add      r0, r0, #0xa8
003b35a4: str      ip, [sp]
003b35a8: bl       #0x30e004
003b35ac: b        #0x3b33a8
003b35b0: bl       #0x30e310
003b35b4: subseq   r1, lr, r8, lsl r7
003b35b8: andeq    r4, r0, ip, lsr #1
003b35bc: andeq    r0, r0, r4, lsl #17
003b35c0: subseq   r0, r1, r8, ror #17
003b35c4: andeq    r3, r0, r8, ror #10
003b35c8: muleq    r0, r0, sl
003b35cc: ldrsheq  r0, [r1], #-0x80
003b35d0: andeq    r3, r0, r0, asr #19
003b35d4: andeq    r1, r0, r0, asr #19

# _Z19PopProfilingContextPKc
003136b8: bx       lr

# _ZNK14CharProperties26PROPS_GetBonusAttackRatingEb
003df81c: push     {r4, r5, r6, lr}
003df820: mov      r4, r0
003df824: ldr      r0, [r0, #4]
003df828: cmp      r1, #0
003df82c: movne    r1, #2
003df830: moveq    r1, #1
003df834: add      r0, r0, #0x37c
003df838: bl       #0x3ffe3c
003df83c: cmp      r0, #0
003df840: beq      #0x3df88c
003df844: bl       #0x3f9e08
003df848: ldr      r2, [r0, #0x94]
003df84c: cmn      r2, #1
003df850: beq      #0x3df88c
003df854: add      r5, r4, #0xa90
003df858: add      r5, r5, #4
003df85c: add      r2, r2, #0x33
003df860: mov      r1, r5
003df864: mov      r0, r4
003df868: bl       #0x3dedb4
003df86c: mov      r6, r0
003df870: ldr      r0, [r4, #4]
003df874: add      r0, r0, #0x37c
003df878: bl       #0x40019c
003df87c: cmp      r0, #0
003df880: bne      #0x3df894
003df884: add      r0, r0, r6
003df888: pop      {r4, r5, r6, pc}
003df88c: mov      r0, #0
003df890: pop      {r4, r5, r6, pc}
003df894: mov      r0, r4
003df898: mov      r1, r5
003df89c: mov      r2, #0x3a
003df8a0: bl       #0x3dedb4
003df8a4: add      r0, r0, r6
003df8a8: pop      {r4, r5, r6, pc}
