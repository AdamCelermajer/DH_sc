
# _ZN7gameswf14glyph_provider15get_face_entityERKNS_9tu_stringEbb
007d113c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d1140: ldr      r4, [pc, #0x4b0]
007d1144: ldr      ip, [pc, #0x4b0]
007d1148: sub      sp, sp, #0x6c
007d114c: add      r4, pc, r4
007d1150: str      r0, [sp, #8]
007d1154: ldr      r0, [r4, ip]
007d1158: str      ip, [sp, #0x10]
007d115c: ldr      ip, [sp, #8]
007d1160: mov      r7, r2
007d1164: ldr      r2, [r0]
007d1168: add      r6, ip, #0xc
007d116c: mov      r0, r6
007d1170: mov      sl, r3
007d1174: str      r2, [sp, #0x64]
007d1178: mov      r8, r1
007d117c: bl       #0x752f50
007d1180: cmp      r7, #0
007d1184: bne      #0x7d1464
007d1188: cmp      sl, #0
007d118c: bne      #0x7d1450
007d1190: ldr      r2, [sp, #8]
007d1194: add      r5, sp, #0x68
007d1198: mov      r3, #0
007d119c: add      r2, r2, #0x24
007d11a0: str      r3, [r5, #-0x1c]!
007d11a4: str      r2, [sp, #0x14]
007d11a8: mov      r0, r2
007d11ac: mov      r1, r6
007d11b0: mov      r2, r5
007d11b4: bl       #0x7d0cac
007d11b8: cmp      r0, #0
007d11bc: beq      #0x7d11f8
007d11c0: ldr      r8, [sp, #0x4c]
007d11c4: mov      r0, r8
007d11c8: cmp      r0, #0
007d11cc: beq      #0x7d11d4
007d11d0: bl       #0x75a240
007d11d4: ldr      ip, [sp, #0x10]
007d11d8: ldr      r2, [sp, #0x64]
007d11dc: mov      r0, r8
007d11e0: ldr      r3, [r4, ip]
007d11e4: ldr      r3, [r3]
007d11e8: cmp      r2, r3
007d11ec: bne      #0x7d15f4
007d11f0: add      sp, sp, #0x6c
007d11f4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d11f8: ldr      r3, [sp, #0x60]
007d11fc: ldrsb    r1, [r8]
007d1200: mvn      r2, #0
007d1204: bfi      r3, r2, #0, #0x18
007d1208: lsr      r2, r3, #0x18
007d120c: bfi      r2, r0, #0, #1
007d1210: cmn      r1, #1
007d1214: mov      r1, #1
007d1218: str      r3, [sp, #0x60]
007d121c: strb     r1, [sp, #0x50]
007d1220: strb     r2, [sp, #0x63]
007d1224: strb     r0, [sp, #0x51]
007d1228: add      r3, sp, #0x50
007d122c: ldreq    r0, [r8, #0xc]
007d1230: str      r3, [sp, #0xc]
007d1234: addne    r0, r8, r1
007d1238: mov      r2, r7
007d123c: mov      r3, sl
007d1240: ldr      r1, [sp, #0xc]
007d1244: bl       #0x7d0fe4
007d1248: cmp      r0, #0
007d124c: beq      #0x7d13b0
007d1250: ldr      ip, [sp, #8]
007d1254: ldr      sl, [ip, #0x24]
007d1258: cmp      sl, #0
007d125c: beq      #0x7d1284
007d1260: ldr      r3, [sl, #4]
007d1264: cmp      r3, #0
007d1268: movlt    r7, #0
007d126c: bge      #0x7d1418
007d1270: ldr      r2, [sp, #0x14]
007d1274: cmp      r2, #0
007d1278: beq      #0x7d1284
007d127c: cmp      sl, #0
007d1280: bne      #0x7d1524
007d1284: ldr      ip, [sp, #8]
007d1288: mov      r2, #0
007d128c: ldrb     r3, [ip, #8]
007d1290: str      r2, [sp, #0x40]
007d1294: cmp      r3, r2
007d1298: beq      #0x7d1478
007d129c: ldrsb    r3, [sp, #0x50]
007d12a0: add      r8, sp, #0x18
007d12a4: mov      r0, r8
007d12a8: cmn      r3, #1
007d12ac: ldrne    r2, [sp, #0xc]
007d12b0: ldreq    r1, [sp, #0x5c]
007d12b4: addne    r1, r2, #1
007d12b8: ldr      r2, [pc, #0x340]
007d12bc: add      r2, pc, r2
007d12c0: bl       #0x7b68c8
007d12c4: ldr      r0, [sp, #0x18]
007d12c8: cmp      r0, #0
007d12cc: beq      #0x7d14f4
007d12d0: mov      lr, pc
007d12d4: ldr      pc, [sp, #0x2c]
007d12d8: ldr      r0, [sp, #0x18]
007d12dc: mov      lr, pc
007d12e0: ldr      pc, [sp, #0x30]
007d12e4: ldr      r1, [sp, #0x18]
007d12e8: mov      sl, r0
007d12ec: mov      r0, #0
007d12f0: mov      lr, pc
007d12f4: ldr      pc, [sp, #0x28]
007d12f8: mov      r1, #0
007d12fc: mov      r0, #0x10
007d1300: bl       #0x752ba8
007d1304: mov      r7, r0
007d1308: bl       #0x7b628c
007d130c: mov      r1, sl
007d1310: mov      r0, r7
007d1314: bl       #0x75ae5c
007d1318: mov      r0, r8
007d131c: mov      r1, r7
007d1320: mvn      r2, #0
007d1324: bl       #0x7b6a80
007d1328: ldr      r3, [sp, #8]
007d132c: ldr      r1, [r7, #8]
007d1330: mov      r2, sl
007d1334: ldr      r0, [r3]
007d1338: add      ip, sp, #0x40
007d133c: mov      r3, #0
007d1340: str      ip, [sp]
007d1344: bl       #0x70c890
007d1348: ldr      sl, [sp, #0x40]
007d134c: cmp      sl, #0
007d1350: beq      #0x7d14e0
007d1354: mov      r1, #0
007d1358: mov      r0, #0x2c
007d135c: bl       #0x752ba8
007d1360: ldr      r3, [sp, #0xc]
007d1364: mov      r2, r7
007d1368: mov      sl, r0
007d136c: ldr      r1, [sp, #0x40]
007d1370: bl       #0x7d0cf8
007d1374: mov      r0, r5
007d1378: mov      r1, sl
007d137c: bl       #0x7d07dc
007d1380: ldr      r0, [sp, #0x14]
007d1384: mov      r1, r6
007d1388: mov      r2, r5
007d138c: bl       #0x7d089c
007d1390: mov      r0, r8
007d1394: bl       #0x7b69e8
007d1398: ldr      r8, [sp, #0x4c]
007d139c: ldrsb    r3, [sp, #0x50]
007d13a0: cmn      r3, #1
007d13a4: beq      #0x7d1404
007d13a8: ldr      r0, [sp, #0x4c]
007d13ac: b        #0x7d11c8
007d13b0: ldrsb    r3, [r8]
007d13b4: ldr      r0, [pc, #0x248]
007d13b8: cmn      r3, #1
007d13bc: addne    r1, r8, #1
007d13c0: ldreq    r1, [r8, #0xc]
007d13c4: add      r0, pc, r0
007d13c8: bl       #0x761184
007d13cc: mov      r8, #0
007d13d0: add      r2, sp, #0x68
007d13d4: str      r8, [r2, #-0x20]!
007d13d8: ldr      r0, [sp, #0x14]
007d13dc: mov      r1, r6
007d13e0: bl       #0x7d089c
007d13e4: ldr      r0, [sp, #0x48]
007d13e8: cmp      r0, r8
007d13ec: moveq    r8, r0
007d13f0: beq      #0x7d13f8
007d13f4: bl       #0x75a240
007d13f8: ldrsb    r3, [sp, #0x50]
007d13fc: cmn      r3, #1
007d1400: bne      #0x7d13a8
007d1404: ldr      r0, [sp, #0x5c]
007d1408: ldr      r1, [sp, #0x58]
007d140c: bl       #0x752b38
007d1410: ldr      r0, [sp, #0x4c]
007d1414: b        #0x7d11c8
007d1418: mov      r2, #8
007d141c: mov      r7, #0
007d1420: ldr      r1, [sl, r2]
007d1424: add      r0, sl, r2
007d1428: cmn      r1, #2
007d142c: beq      #0x7d143c
007d1430: ldr      r1, [r0, #4]
007d1434: cmn      r1, #1
007d1438: bne      #0x7d1270
007d143c: add      r7, r7, #1
007d1440: cmp      r7, r3
007d1444: add      r2, r2, #0x20
007d1448: ble      #0x7d1420
007d144c: b        #0x7d1270
007d1450: ldr      r1, [pc, #0x1b0]
007d1454: mov      r0, r6
007d1458: add      r1, pc, r1
007d145c: bl       #0x7521cc
007d1460: b        #0x7d1190
007d1464: ldr      r1, [pc, #0x1a0]
007d1468: mov      r0, r6
007d146c: add      r1, pc, r1
007d1470: bl       #0x7521cc
007d1474: b        #0x7d1188
007d1478: ldrsb    r3, [sp, #0x50]
007d147c: ldr      r2, [sp, #8]
007d1480: cmn      r3, #1
007d1484: ldrne    r3, [sp, #0xc]
007d1488: ldreq    r1, [sp, #0x5c]
007d148c: ldr      r0, [r2]
007d1490: addne    r1, r3, #1
007d1494: mov      r2, #0
007d1498: add      r3, sp, #0x40
007d149c: bl       #0x70c8cc
007d14a0: mov      r1, #0
007d14a4: mov      r0, #0x2c
007d14a8: bl       #0x752ba8
007d14ac: ldr      r2, [sp, #0xc]
007d14b0: mov      r7, r0
007d14b4: ldr      r1, [sp, #0x40]
007d14b8: bl       #0x7d0d7c
007d14bc: mov      r0, r5
007d14c0: mov      r1, r7
007d14c4: bl       #0x7d07dc
007d14c8: ldr      r0, [sp, #0x14]
007d14cc: mov      r1, r6
007d14d0: mov      r2, r5
007d14d4: bl       #0x7d089c
007d14d8: ldr      r8, [sp, #0x4c]
007d14dc: b        #0x7d139c
007d14e0: mov      r0, r7
007d14e4: bl       #0x7b6548
007d14e8: mov      r0, r7
007d14ec: mov      r1, sl
007d14f0: bl       #0x752b38
007d14f4: mov      r0, r8
007d14f8: bl       #0x7b69e8
007d14fc: ldrsb    r3, [sp, #0x50]
007d1500: cmn      r3, #1
007d1504: ldreq    r1, [sp, #0x5c]
007d1508: ldrne    ip, [sp, #0xc]
007d150c: addne    r1, ip, #1
007d1510: ldr      r0, [pc, #0xf8]
007d1514: add      r0, pc, r0
007d1518: bl       #0x761184
007d151c: ldr      r8, [sp, #0x4c]
007d1520: b        #0x7d139c
007d1524: ldr      r3, [sp, #0xc]
007d1528: ldr      sb, [sl, #4]
007d152c: add      fp, r3, #1
007d1530: cmp      sb, r7
007d1534: blt      #0x7d1284
007d1538: add      r3, sl, r7, lsl #5
007d153c: ldr      r8, [r3, #0x24]
007d1540: ldr      r2, [sp, #0xc]
007d1544: add      r3, r8, #0xc
007d1548: cmp      r2, r3
007d154c: beq      #0x7d15c0
007d1550: ldrsb    r3, [r8, #0xc]
007d1554: cmn      r3, #1
007d1558: ldrsb    r3, [sp, #0x50]
007d155c: addne    r0, r8, #0xd
007d1560: ldreq    r0, [r8, #0x18]
007d1564: cmn      r3, #1
007d1568: movne    r1, fp
007d156c: ldreq    r1, [sp, #0x5c]
007d1570: bl       #0x30e31c
007d1574: cmp      r0, #0
007d1578: beq      #0x7d15c0
007d157c: add      r7, r7, #1
007d1580: cmp      r7, sb
007d1584: bgt      #0x7d1530
007d1588: lsl      r3, r7, #5
007d158c: add      r3, r3, #8
007d1590: ldr      r2, [sl, r3]
007d1594: add      r1, sl, r3
007d1598: cmn      r2, #2
007d159c: beq      #0x7d15ac
007d15a0: ldr      r2, [r1, #4]
007d15a4: cmn      r2, #1
007d15a8: bne      #0x7d1530
007d15ac: add      r7, r7, #1
007d15b0: cmp      r7, sb
007d15b4: add      r3, r3, #0x20
007d15b8: ble      #0x7d1590
007d15bc: b        #0x7d1530
007d15c0: cmp      r8, #0
007d15c4: str      r8, [sp, #0x44]
007d15c8: beq      #0x7d15d4
007d15cc: mov      r0, r8
007d15d0: bl       #0x759c64
007d15d4: ldr      r0, [sp, #0x14]
007d15d8: mov      r1, r6
007d15dc: add      r2, sp, #0x44
007d15e0: bl       #0x7d089c
007d15e4: ldr      r0, [sp, #0x44]
007d15e8: cmp      r0, #0
007d15ec: bne      #0x7d13f4
007d15f0: b        #0x7d13f8
007d15f4: bl       #0x30e310
007d15f8: andseq   r3, ip, r4, asr #18
007d15fc: andeq    r4, r0, ip, lsr #1
007d1600: andeq    pc, lr, r4, ror #9
007d1604: andseq   sl, r3, r4, lsl ip
007d1608: andseq   sb, r3, r0, lsr #25
007d160c: andseq   r2, r2, r4, asr #19
007d1610: andseq   sl, r3, r4, ror #21

# _ZN7gameswf14glyph_provider14get_char_imageEtRKNS_9tu_stringEbbiPNS_4rectEPf
007d1614: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d1618: sub      sp, sp, #0x1c
007d161c: mov      r5, r1
007d1620: mov      r1, r2
007d1624: mov      r2, r3
007d1628: ldrb     r3, [sp, #0x40]
007d162c: mov      r4, r0
007d1630: ldr      sb, [sp, #0x44]
007d1634: ldr      fp, [sp, #0x48]
007d1638: bl       #0x7d113c
007d163c: subs     r6, r0, #0
007d1640: beq      #0x7d16b4
007d1644: add      r8, r6, #0x28
007d1648: add      sl, sp, #0x14
007d164c: orr      r3, r5, sb, lsl #16
007d1650: mov      r7, #0
007d1654: mov      r0, r8
007d1658: mov      r1, sl
007d165c: str      r3, [sp, #0x14]
007d1660: str      r7, [sp, #0x10]
007d1664: bl       #0x7c4418
007d1668: cmp      r0, #0
007d166c: blt      #0x7d16bc
007d1670: ldr      r3, [r6, #0x28]
007d1674: add      r0, r3, r0, lsl #4
007d1678: ldr      r3, [r0, #0x14]
007d167c: str      r3, [sp, #0x10]
007d1680: mov      ip, r3
007d1684: add      r3, r3, #8
007d1688: ldm      r3, {r0, r1, r2, r3}
007d168c: stm      fp, {r0, r1, r2, r3}
007d1690: ldr      r3, [sp, #0x4c]
007d1694: ldr      r2, [ip, #4]
007d1698: str      r2, [r3]
007d169c: ldr      r3, [r4, #0x28]
007d16a0: cmp      r3, #0
007d16a4: ldreq    r0, [ip]
007d16a8: ldrne    r0, [r3, #0x34]
007d16ac: add      sp, sp, #0x1c
007d16b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d16b4: mov      r0, #0
007d16b8: b        #0x7d16ac
007d16bc: mov      r0, sb
007d16c0: bl       #0x30e964
007d16c4: ldr      r1, [r4, #4]
007d16c8: mov      sb, r0
007d16cc: bl       #0x30ed6c
007d16d0: bl       #0x30e4cc
007d16d4: mov      r1, r7
007d16d8: mov      r2, r0
007d16dc: ldr      r0, [r6, #0x24]
007d16e0: bl       #0x70a7fc
007d16e4: ldr      r3, [r4, #0x28]
007d16e8: cmp      r3, #0
007d16ec: beq      #0x7d191c
007d16f0: mov      r1, r5
007d16f4: mov      r2, r7
007d16f8: ldr      r0, [r6, #0x24]
007d16fc: bl       #0x709660
007d1700: subs     r5, r0, #0
007d1704: bne      #0x7d16b4
007d1708: mov      r1, r5
007d170c: mov      r0, #0x18
007d1710: bl       #0x752ba8
007d1714: mov      r3, #0
007d1718: str      r5, [r0]
007d171c: str      r3, [r0, #0x14]
007d1720: str      r3, [r0, #4]
007d1724: str      r3, [r0, #8]
007d1728: str      r3, [r0, #0xc]
007d172c: str      r3, [r0, #0x10]
007d1730: str      r0, [sp, #0x10]
007d1734: ldr      r3, [r6, #0x24]
007d1738: add      r1, sp, #8
007d173c: add      r0, sp, #0xc
007d1740: ldr      r3, [r3, #0x54]
007d1744: ldr      r3, [r3, #0x18]
007d1748: add      r2, r3, #0x3f
007d174c: cmp      r3, #0
007d1750: movlt    r3, r2
007d1754: asr      r3, r3, #6
007d1758: add      r3, r3, #1
007d175c: str      r3, [sp, #0xc]
007d1760: ldr      r3, [r6, #0x24]
007d1764: ldr      r3, [r3, #0x54]
007d1768: ldr      r3, [r3, #0x1c]
007d176c: add      r2, r3, #0x3f
007d1770: cmp      r3, #0
007d1774: movlt    r3, r2
007d1778: asr      r3, r3, #6
007d177c: add      r3, r3, #1
007d1780: str      r3, [sp, #8]
007d1784: bl       #0x793560
007d1788: ldr      r3, [r6, #0x24]
007d178c: ldr      r3, [r3, #0x54]
007d1790: ldr      r3, [r3, #0x18]
007d1794: add      r2, r3, #0x3f
007d1798: cmp      r3, #0
007d179c: movlt    r3, r2
007d17a0: asr      r0, r3, #6
007d17a4: bl       #0x30e964
007d17a8: mov      r5, r0
007d17ac: ldr      r0, [sp, #0xc]
007d17b0: bl       #0x30e964
007d17b4: mov      r1, r0
007d17b8: mov      r0, r5
007d17bc: bl       #0x30ec94
007d17c0: ldr      r3, [sp, #0x10]
007d17c4: str      r0, [r3, #0xc]
007d17c8: ldr      r3, [r6, #0x24]
007d17cc: ldr      r3, [r3, #0x54]
007d17d0: ldr      r3, [r3, #0x1c]
007d17d4: add      r2, r3, #0x3f
007d17d8: cmp      r3, #0
007d17dc: movlt    r3, r2
007d17e0: asr      r0, r3, #6
007d17e4: bl       #0x30e964
007d17e8: mov      r5, r0
007d17ec: ldr      r0, [sp, #8]
007d17f0: bl       #0x30e964
007d17f4: mov      r1, r0
007d17f8: mov      r0, r5
007d17fc: bl       #0x30ec94
007d1800: ldr      r3, [sp, #0x10]
007d1804: str      r0, [r3, #0x14]
007d1808: ldr      r3, [r6, #0x24]
007d180c: ldr      r5, [sp, #0x10]
007d1810: ldr      r3, [r3, #0x54]
007d1814: ldr      r7, [r3, #0x18]
007d1818: cmp      r7, #0
007d181c: movle    r0, #0
007d1820: ble      #0x7d184c
007d1824: ldr      r0, [r3, #0x20]
007d1828: bl       #0x30e964
007d182c: mov      r3, r0
007d1830: mov      r0, r7
007d1834: str      r3, [sp, #4]
007d1838: bl       #0x30e964
007d183c: ldr      r3, [sp, #4]
007d1840: mov      r1, r0
007d1844: mov      r0, r3
007d1848: bl       #0x30ec94
007d184c: str      r0, [r5, #8]
007d1850: ldr      r3, [r6, #0x24]
007d1854: ldr      r5, [sp, #0x10]
007d1858: ldr      r3, [r3, #0x54]
007d185c: ldr      r7, [r3, #0x1c]
007d1860: cmp      r7, #0
007d1864: movle    r0, #0
007d1868: ble      #0x7d1894
007d186c: ldr      r0, [r3, #0x24]
007d1870: bl       #0x30e964
007d1874: mov      r3, r0
007d1878: mov      r0, r7
007d187c: str      r3, [sp, #4]
007d1880: bl       #0x30e964
007d1884: ldr      r3, [sp, #4]
007d1888: mov      r1, r0
007d188c: mov      r0, r3
007d1890: bl       #0x30ec94
007d1894: str      r0, [r5, #0x10]
007d1898: ldr      r7, [sp, #0x10]
007d189c: add      r5, sp, #0x18
007d18a0: ldr      r1, [r7, #0xc]
007d18a4: ldr      r0, [r7, #8]
007d18a8: add      r1, r1, #0x80000000
007d18ac: bl       #0x30ed6c
007d18b0: str      r0, [r7, #8]
007d18b4: ldr      r7, [sp, #0x10]
007d18b8: ldr      r1, [r7, #0x14]
007d18bc: ldr      r0, [r7, #0x10]
007d18c0: bl       #0x30ed6c
007d18c4: str      r0, [r7, #0x10]
007d18c8: ldr      r3, [r6, #0x24]
007d18cc: ldr      r6, [r5, #-8]!
007d18d0: ldr      r3, [r3, #0x54]
007d18d4: ldr      r0, [r3, #0x28]
007d18d8: bl       #0x30e964
007d18dc: mov      r7, r0
007d18e0: mov      r0, #0x41000000
007d18e4: mov      r1, sb
007d18e8: add      r0, r0, #0x800000
007d18ec: bl       #0x30ec94
007d18f0: mov      r1, r0
007d18f4: mov      r0, r7
007d18f8: bl       #0x30ed6c
007d18fc: mov      r1, sl
007d1900: str      r0, [r6, #4]
007d1904: mov      r2, r5
007d1908: mov      r0, r8
007d190c: bl       #0x7c5878
007d1910: ldr      ip, [sp, #0x10]
007d1914: mov      r3, ip
007d1918: b        #0x7d1684
007d191c: mov      r1, r5
007d1920: ldr      r0, [r6, #0x24]
007d1924: mov      r2, #4
007d1928: bl       #0x709660
007d192c: subs     r5, r0, #0
007d1930: bne      #0x7d16b4
007d1934: mov      r1, r5
007d1938: mov      r0, #0x18
007d193c: bl       #0x752ba8
007d1940: mov      r2, #0
007d1944: mov      r3, r0
007d1948: str      r5, [r0]
007d194c: str      r2, [r3, #0x14]
007d1950: str      r2, [r3, #4]
007d1954: str      r2, [r3, #8]
007d1958: str      r2, [r3, #0xc]
007d195c: str      r2, [r3, #0x10]
007d1960: str      r3, [sp, #0x10]
007d1964: ldr      r3, [r6, #0x24]
007d1968: mov      r0, r4
007d196c: ldr      r1, [r3, #0x54]
007d1970: add      r1, r1, #0x4c
007d1974: bl       #0x7d06c4
007d1978: mov      r5, r0
007d197c: ldr      r2, [r0, #8]
007d1980: ldr      r1, [r5, #0x10]
007d1984: ldr      r0, [r0, #0xc]
007d1988: ldr      r7, [sp, #0x10]
007d198c: bl       #0x773bac
007d1990: mov      r1, r0
007d1994: mov      r0, r7
007d1998: bl       #0x77a740
007d199c: mov      r0, r5
007d19a0: bl       #0x7d0444
007d19a4: ldr      r3, [r6, #0x24]
007d19a8: ldr      r3, [r3, #0x54]
007d19ac: ldr      r0, [r3, #0x50]
007d19b0: bl       #0x30e964
007d19b4: ldr      r5, [sp, #0x10]
007d19b8: mov      r7, r0
007d19bc: ldr      r3, [r5]
007d19c0: mov      r0, r3
007d19c4: ldr      r3, [r3]
007d19c8: mov      lr, pc
007d19cc: ldr      pc, [r3, #0x24]
007d19d0: bl       #0x30e964
007d19d4: mov      r1, r0
007d19d8: mov      r0, r7
007d19dc: bl       #0x30ec94
007d19e0: str      r0, [r5, #0xc]
007d19e4: ldr      r3, [r6, #0x24]
007d19e8: ldr      r3, [r3, #0x54]
007d19ec: ldr      r0, [r3, #0x4c]
007d19f0: bl       #0x30e964
007d19f4: ldr      r5, [sp, #0x10]
007d19f8: mov      r7, r0
007d19fc: ldr      r3, [r5]
007d1a00: mov      r0, r3
007d1a04: ldr      r3, [r3]
007d1a08: mov      lr, pc
007d1a0c: ldr      pc, [r3, #0x28]
007d1a10: bl       #0x30e964
007d1a14: mov      r1, r0
007d1a18: mov      r0, r7
007d1a1c: bl       #0x30ec94
007d1a20: str      r0, [r5, #0x14]
007d1a24: b        #0x7d1808
