
# _ZNK16CharStateMachine14SM_IsAttackingEv
003c02d0: push     {r4, lr}
003c02d4: bl       #0x3c01ac
003c02d8: cmp      r0, #5
003c02dc: movne    r0, #0
003c02e0: moveq    r0, #1
003c02e4: pop      {r4, pc}

# _ZN9Character9RegenTickEb
003bdd90: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003bdd94: ldr      r4, [pc, #0x144]
003bdd98: ldr      r6, [pc, #0x144]
003bdd9c: sub      sp, sp, #0x40
003bdda0: add      r4, pc, r4
003bdda4: ldr      r3, [r4, r6]
003bdda8: cmp      r1, #0
003bddac: mov      r5, r0
003bddb0: ldr      r3, [r3]
003bddb4: str      r3, [sp, #0x3c]
003bddb8: beq      #0x3bde58
003bddbc: ldr      r3, [pc, #0x124]
003bddc0: add      sb, r0, #0x560
003bddc4: add      r7, r0, #0xff0
003bddc8: ldr      sl, [r4, r3]
003bddcc: add      r8, sp, #0x24
003bddd0: add      r7, r7, #4
003bddd4: mov      r0, sl
003bddd8: bl       #0x337888
003bdddc: ldr      r1, [pc, #0x108]
003bdde0: add      r2, sp, #8
003bdde4: mov      r0, r8
003bdde8: add      r1, pc, r1
003bddec: bl       #0x3140ec
003bddf0: mov      r1, r8
003bddf4: mov      r0, sl
003bddf8: bl       #0x337a88
003bddfc: mov      r0, r8
003bde00: bl       #0x318254
003bde04: mov      r2, #0x28
003bde08: mov      r1, r7
003bde0c: mov      r0, sb
003bde10: bl       #0x3dedb4
003bde14: mov      r1, r0
003bde18: mov      r0, r5
003bde1c: bl       #0x3bdca4
003bde20: mov      r1, r7
003bde24: mov      r0, sb
003bde28: mov      r2, #0x2d
003bde2c: bl       #0x3dedb4
003bde30: mov      r1, r0
003bde34: mov      r0, r5
003bde38: bl       #0x3bdbb8
003bde3c: ldr      r3, [r4, r6]
003bde40: ldr      r2, [sp, #0x3c]
003bde44: ldr      r3, [r3]
003bde48: cmp      r2, r3
003bde4c: bne      #0x3bdedc
003bde50: add      sp, sp, #0x40
003bde54: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003bde58: ldr      r3, [pc, #0x88]
003bde5c: add      sb, r0, #0x560
003bde60: add      r7, r0, #0xff0
003bde64: ldr      sl, [r4, r3]
003bde68: add      r8, sp, #0xc
003bde6c: add      r7, r7, #4
003bde70: mov      r0, sl
003bde74: bl       #0x337888
003bde78: ldr      r1, [pc, #0x70]
003bde7c: add      r2, sp, #4
003bde80: mov      r0, r8
003bde84: add      r1, pc, r1
003bde88: bl       #0x3140ec
003bde8c: mov      r1, r8
003bde90: mov      r0, sl
003bde94: bl       #0x337a88
003bde98: mov      r0, r8
003bde9c: bl       #0x318254
003bdea0: mov      r2, #0x27
003bdea4: mov      r1, r7
003bdea8: mov      r0, sb
003bdeac: bl       #0x3dedb4
003bdeb0: mov      r1, r0
003bdeb4: mov      r0, r5
003bdeb8: bl       #0x3bdca4
003bdebc: mov      r1, r7
003bdec0: mov      r0, sb
003bdec4: mov      r2, #0x2c
003bdec8: bl       #0x3dedb4
003bdecc: mov      r1, r0
003bded0: mov      r0, r5
003bded4: bl       #0x3bdbb8
003bded8: b        #0x3bde3c
003bdedc: bl       #0x30e310
003bdee0: ldrsheq  r6, [sp], #-0xc0
003bdee4: andeq    r4, r0, ip, lsr #1
003bdee8: andeq    r0, r0, r4, lsl #17
003bdeec: subseq   r6, r0, r8, lsr #21
003bdef0: subseq   r6, r0, ip, lsl #20

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

# _ZN6CharAI12_UpdateRegenEv
003cb77c: push     {r4, lr}
003cb780: ldr      r3, [r0, #4]
003cb784: mov      r4, r0
003cb788: mov      r0, r3
003cb78c: ldr      r3, [r3]
003cb790: mov      lr, pc
003cb794: ldr      pc, [r3, #0x54]
003cb798: cmp      r0, #0
003cb79c: beq      #0x3cb7a4
003cb7a0: pop      {r4, pc}
003cb7a4: mov      r0, r4
003cb7a8: ldr      r4, [r4, #4]
003cb7ac: bl       #0x3d4bc4
003cb7b0: mov      r1, r0
003cb7b4: mov      r0, r4
003cb7b8: pop      {r4, lr}
003cb7bc: b        #0x3bdd90

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

# _ZN9Character11F_DotAttackERNS_12AttackResultEPS_S2_ii
003b2e68: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b2e6c: ldr      r4, [pc, #0x160]
003b2e70: ldr      r6, [pc, #0x160]
003b2e74: subs     r7, r1, #0
003b2e78: add      r4, pc, r4
003b2e7c: ldr      r1, [r4, r6]
003b2e80: mov      r5, r2
003b2e84: sub      sp, sp, #0x34
003b2e88: ldr      r2, [r1]
003b2e8c: mov      sb, r0
003b2e90: mov      fp, r3
003b2e94: str      r2, [sp, #0x2c]
003b2e98: beq      #0x3b2f28
003b2e9c: cmp      r5, #0
003b2ea0: beq      #0x3b2f7c
003b2ea4: ldr      r3, [pc, #0x130]
003b2ea8: add      r8, sp, #0x14
003b2eac: ldr      sl, [r4, r3]
003b2eb0: mov      r0, sl
003b2eb4: bl       #0x337888
003b2eb8: ldr      r1, [pc, #0x120]
003b2ebc: add      r2, sp, #0x10
003b2ec0: mov      r0, r8
003b2ec4: add      r1, pc, r1
003b2ec8: bl       #0x3140ec
003b2ecc: mov      r1, r8
003b2ed0: mov      r0, sl
003b2ed4: bl       #0x337a88
003b2ed8: mov      r0, r8
003b2edc: bl       #0x3139ac
003b2ee0: mvn      ip, #0
003b2ee4: str      ip, [sp]
003b2ee8: ldr      ip, [sp, #0x58]
003b2eec: mov      r3, #0x20000000
003b2ef0: mov      r2, r5
003b2ef4: add      r3, r3, #0x80000
003b2ef8: mov      r0, sb
003b2efc: mov      r1, r7
003b2f00: str      ip, [sp, #4]
003b2f04: str      fp, [sp, #8]
003b2f08: bl       #0x3b2638
003b2f0c: ldr      r3, [r4, r6]
003b2f10: ldr      r2, [sp, #0x2c]
003b2f14: ldr      r3, [r3]
003b2f18: cmp      r2, r3
003b2f1c: bne      #0x3b2fd0
003b2f20: add      sp, sp, #0x34
003b2f24: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b2f28: ldr      r3, [pc, #0xb4]
003b2f2c: ldr      r3, [r4, r3]
003b2f30: ldr      r3, [r3]
003b2f34: cmp      r3, #2
003b2f38: streq    r7, [r7]
003b2f3c: beq      #0x3b2e9c
003b2f40: cmp      r3, #1
003b2f44: bne      #0x3b2e9c
003b2f48: ldr      r0, [pc, #0x98]
003b2f4c: ldr      r1, [pc, #0x98]
003b2f50: ldr      r2, [pc, #0x98]
003b2f54: ldr      r0, [r4, r0]
003b2f58: ldr      r3, [pc, #0x94]
003b2f5c: movw     ip, #0x289
003b2f60: add      r1, pc, r1
003b2f64: add      r2, pc, r2
003b2f68: add      r3, pc, r3
003b2f6c: add      r0, r0, #0xa8
003b2f70: str      ip, [sp]
003b2f74: bl       #0x30e004
003b2f78: b        #0x3b2e9c
003b2f7c: ldr      r3, [pc, #0x60]
003b2f80: ldr      r3, [r4, r3]
003b2f84: ldr      r3, [r3]
003b2f88: cmp      r3, #2
003b2f8c: streq    r5, [r5]
003b2f90: beq      #0x3b2ea4
003b2f94: cmp      r3, #1
003b2f98: bne      #0x3b2ea4
003b2f9c: ldr      r0, [pc, #0x44]
003b2fa0: ldr      r1, [pc, #0x50]
003b2fa4: ldr      r2, [pc, #0x50]
003b2fa8: ldr      r0, [r4, r0]
003b2fac: ldr      r3, [pc, #0x4c]
003b2fb0: movw     ip, #0x28a
003b2fb4: add      r1, pc, r1
003b2fb8: add      r2, pc, r2
003b2fbc: add      r3, pc, r3
003b2fc0: add      r0, r0, #0xa8
003b2fc4: str      ip, [sp]
003b2fc8: bl       #0x30e004
003b2fcc: b        #0x3b2ea4
003b2fd0: bl       #0x30e310
003b2fd4: subseq   r1, lr, r8, lsl ip
003b2fd8: andeq    r4, r0, ip, lsr #1
003b2fdc: andeq    r0, r0, r4, lsl #17
003b2fe0: ldrsheq  r0, [r1], #-0xd4
003b2fe4: andeq    r3, r0, r0, asr #19
003b2fe8: andeq    r1, r0, r0, asr #19
003b2fec: subseq   fp, r0, r8, ror r4
003b2ff0: subseq   r0, r1, ip, ror #26
003b2ff4: subseq   r0, r1, r8, lsr #25
003b2ff8: subseq   fp, r0, r4, lsr #8
003b2ffc: subseq   lr, r0, r8, asr #18
003b3000: subseq   r0, r1, r4, asr ip

# _ZN9Character7RegenMPEi
003bdbb8: push     {r4, r5, r6, r7, r8, sl, lr}
003bdbbc: ldr      r5, [pc, #0xd0]
003bdbc0: ldr      r8, [pc, #0xd0]
003bdbc4: add      r6, r0, #0xff0
003bdbc8: add      r5, pc, r5
003bdbcc: ldr      r3, [r5, r8]
003bdbd0: add      r6, r6, #4
003bdbd4: add      r7, r0, #0x560
003bdbd8: ldr      r3, [r3]
003bdbdc: mov      r4, r1
003bdbe0: sub      sp, sp, #0x24
003bdbe4: mov      r2, #0x29
003bdbe8: mov      r1, r6
003bdbec: mov      r0, r7
003bdbf0: str      r3, [sp, #0x1c]
003bdbf4: bl       #0x3dedb4
003bdbf8: mov      sl, r0
003bdbfc: mov      r1, r6
003bdc00: mov      r0, r7
003bdc04: mov      r2, #0x2b
003bdc08: bl       #0x3dedb4
003bdc0c: cmp      r4, #0
003bdc10: movlt    r4, r0
003bdc14: add      r3, r4, sl
003bdc18: cmp      r3, r0
003bdc1c: rsbgt    r4, sl, r0
003bdc20: cmp      r4, #0
003bdc24: ble      #0x3bdc74
003bdc28: ldr      r3, [pc, #0x6c]
003bdc2c: add      r6, sp, #4
003bdc30: ldr      sl, [r5, r3]
003bdc34: mov      r0, sl
003bdc38: bl       #0x337888
003bdc3c: ldr      r1, [pc, #0x5c]
003bdc40: mov      r2, sp
003bdc44: mov      r0, r6
003bdc48: add      r1, pc, r1
003bdc4c: bl       #0x3140ec
003bdc50: mov      r1, r6
003bdc54: mov      r0, sl
003bdc58: bl       #0x337a88
003bdc5c: mov      r0, r6
003bdc60: bl       #0x318254
003bdc64: mov      r0, r7
003bdc68: mov      r2, r4
003bdc6c: mov      r1, #0x29
003bdc70: bl       #0x3e0708
003bdc74: ldr      r3, [r5, r8]
003bdc78: ldr      r2, [sp, #0x1c]
003bdc7c: ldr      r3, [r3]
003bdc80: cmp      r2, r3
003bdc84: bne      #0x3bdc90
003bdc88: add      sp, sp, #0x24
003bdc8c: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdc90: bl       #0x30e310
003bdc94: subseq   r6, sp, r8, asr #29
003bdc98: andeq    r4, r0, ip, lsr #1
003bdc9c: andeq    r0, r0, r4, lsl #17
003bdca0: subseq   r6, r0, r8, asr #24

# _ZNK6CharAI12AI_IsAggroedEv
003d4a00: ldr      r0, [r0, #0xa4]
003d4a04: subs     r0, r0, #0
003d4a08: movne    r0, #1
003d4a0c: bx       lr

# _ZNK16CharStateMachine12SM_IsCastingEv
003c0334: push     {r4, lr}
003c0338: bl       #0x3c01ac
003c033c: cmp      r0, #7
003c0340: movne    r0, #0
003c0344: moveq    r0, #1
003c0348: pop      {r4, pc}

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZN14CharProperties15PROPS_RemoveDotEi
003de83c: bx       lr

# _ZNK6CharAI11AI_HasAggroEv
003d49f0: ldr      r0, [r0, #0x8c]
003d49f4: subs     r0, r0, #0
003d49f8: movne    r0, #1
003d49fc: bx       lr

# _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
003def34: ldr      r3, [pc, #0x40]
003def38: ldr      r2, [pc, #0x40]
003def3c: push     {r4, r5, r6, r7, r8, lr}
003def40: add      r3, pc, r3
003def44: mov      r8, r0
003def48: ldr      r7, [r3, r2]
003def4c: mov      r6, r1
003def50: mov      r4, #0
003def54: mov      r1, r4
003def58: mov      r0, r8
003def5c: ldr      r5, [r7, r4, lsl #2]
003def60: bl       #0x3def10
003def64: add      r4, r4, #1
003def68: add      r5, r5, #4
003def6c: cmp      r4, #0xe0
003def70: str      r0, [r6, r5]
003def74: bne      #0x3def54
003def78: pop      {r4, r5, r6, r7, r8, pc}
003def7c: subseq   r5, fp, r0, asr fp
003def80: andeq    r2, r0, r8, lsr #5

# _ZN14CharProperties9PROPS_AddEii
003e0708: push     {r4, r5, r6, r7, r8, lr}
003e070c: mov      r7, r2
003e0710: mov      r4, r0
003e0714: mov      r5, r1
003e0718: bl       #0x3deed8
003e071c: tst      r0, #0x20
003e0720: bne      #0x3e0760
003e0724: tst      r0, #8
003e0728: bne      #0x3e0730
003e072c: pop      {r4, r5, r6, r7, r8, pc}
003e0730: add      r6, r4, #0xa90
003e0734: add      r6, r6, #4
003e0738: mov      r1, r6
003e073c: mov      r2, r5
003e0740: mov      r0, r4
003e0744: bl       #0x3dedb4
003e0748: mov      r1, r6
003e074c: add      r3, r0, r7
003e0750: mov      r2, r5
003e0754: mov      r0, r4
003e0758: pop      {r4, r5, r6, r7, r8, lr}
003e075c: b        #0x3deca0
003e0760: add      r6, r4, #0x38c
003e0764: mov      r1, r6
003e0768: mov      r2, r5
003e076c: mov      r0, r4
003e0770: bl       #0x3dedb4
003e0774: mov      r1, r6
003e0778: add      r3, r0, r7
003e077c: mov      r2, r5
003e0780: mov      r0, r4
003e0784: bl       #0x3deca0
003e0788: mov      r0, r4
003e078c: mov      r1, r5
003e0790: pop      {r4, r5, r6, r7, r8, lr}
003e0794: b        #0x3dfe60

# _ZNK14CharProperties11_GetDefaultEi
003def10: ldr      r3, [pc, #0x14]
003def14: mov      r2, r1
003def18: ldr      r1, [pc, #0x10]
003def1c: add      r3, pc, r3
003def20: ldr      ip, [r3, r1]
003def24: ldr      r1, [ip]
003def28: b        #0x3dedb4
003def2c: subseq   r5, fp, r4, ror fp
003def30: andeq    r2, r0, r0, asr fp

# _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003dedb4: str      lr, [sp, #-4]!
003dedb8: ldr      r3, [pc, #0xf0]
003dedbc: cmp      r2, #0
003dedc0: sub      sp, sp, #0xc
003dedc4: add      r3, pc, r3
003dedc8: blt      #0x3dedfc
003dedcc: cmp      r2, #0xdf
003dedd0: ble      #0x3dee20
003dedd4: ldr      r2, [pc, #0xd8]
003dedd8: ldr      r2, [r3, r2]
003deddc: ldr      r2, [r2]
003dede0: cmp      r2, #2
003dede4: beq      #0x3dee10
003dede8: cmp      r2, #1
003dedec: beq      #0x3dee78
003dedf0: mvn      r0, #0
003dedf4: add      sp, sp, #0xc
003dedf8: ldm      sp!, {pc}
003dedfc: ldr      r2, [pc, #0xb0]
003dee00: ldr      r2, [r3, r2]
003dee04: ldr      r2, [r2]
003dee08: cmp      r2, #2
003dee0c: bne      #0x3dee38
003dee10: mov      r3, #0
003dee14: str      r3, [r3]
003dee18: mvn      r0, #0
003dee1c: b        #0x3dedf4
003dee20: ldr      r0, [pc, #0x90]
003dee24: ldr      r3, [r3, r0]
003dee28: ldr      r3, [r3, r2, lsl #2]
003dee2c: add      r1, r1, r3
003dee30: ldr      r0, [r1, #4]
003dee34: b        #0x3dedf4
003dee38: cmp      r2, #1
003dee3c: bne      #0x3dedf0
003dee40: ldr      r0, [pc, #0x74]
003dee44: ldr      r1, [pc, #0x74]
003dee48: ldr      r2, [pc, #0x74]
003dee4c: ldr      r0, [r3, r0]
003dee50: ldr      r3, [pc, #0x70]
003dee54: movw     ip, #0x103
003dee58: add      r1, pc, r1
003dee5c: add      r0, r0, #0xa8
003dee60: add      r2, pc, r2
003dee64: add      r3, pc, r3
003dee68: str      ip, [sp]
003dee6c: bl       #0x30e004
003dee70: mvn      r0, #0
003dee74: b        #0x3dedf4
003dee78: ldr      r0, [pc, #0x3c]
003dee7c: ldr      r1, [pc, #0x48]
003dee80: ldr      r2, [pc, #0x48]
003dee84: ldr      r0, [r3, r0]
003dee88: ldr      r3, [pc, #0x44]
003dee8c: mov      ip, #0x104
003dee90: add      r1, pc, r1
003dee94: add      r0, r0, #0xa8
003dee98: add      r2, pc, r2
003dee9c: add      r3, pc, r3
003deea0: str      ip, [sp]
003deea4: bl       #0x30e004
003deea8: mvn      r0, #0
003deeac: b        #0x3dedf4
003deeb0: subseq   r5, fp, ip, asr #25
003deeb4: andeq    r3, r0, r0, asr #19
003deeb8: andeq    r2, r0, r8, lsr #5
003deebc: andeq    r1, r0, r0, asr #19
003deec0: subeq    pc, sp, r0, lsl #11
003deec4: strheq   r6, [lr], #-0xe0
003deec8: subeq    r6, lr, ip, asr #28
003deecc: subeq    pc, sp, r8, asr #10
003deed0: subeq    r6, lr, r8, lsl #29
003deed4: subeq    r6, lr, r4, lsl lr

# _ZNK16CharStateMachine15SM_IsUsingSkillEv
003c02e8: push     {r4, lr}
003c02ec: bl       #0x3c01ac
003c02f0: cmp      r0, #6
003c02f4: movne    r0, #0
003c02f8: moveq    r0, #1
003c02fc: pop      {r4, pc}

# _ZN9Character7RegenHPEi
003bdca4: push     {r4, r5, r6, r7, r8, sl, lr}
003bdca8: ldr      r5, [pc, #0xd0]
003bdcac: ldr      r8, [pc, #0xd0]
003bdcb0: add      r6, r0, #0xff0
003bdcb4: add      r5, pc, r5
003bdcb8: ldr      r3, [r5, r8]
003bdcbc: add      r6, r6, #4
003bdcc0: add      r7, r0, #0x560
003bdcc4: ldr      r3, [r3]
003bdcc8: mov      r4, r1
003bdccc: sub      sp, sp, #0x24
003bdcd0: mov      r2, #0x24
003bdcd4: mov      r1, r6
003bdcd8: mov      r0, r7
003bdcdc: str      r3, [sp, #0x1c]
003bdce0: bl       #0x3dedb4
003bdce4: mov      sl, r0
003bdce8: mov      r1, r6
003bdcec: mov      r0, r7
003bdcf0: mov      r2, #0x26
003bdcf4: bl       #0x3dedb4
003bdcf8: cmp      r4, #0
003bdcfc: movlt    r4, r0
003bdd00: add      r3, r4, sl
003bdd04: cmp      r3, r0
003bdd08: rsbgt    r4, sl, r0
003bdd0c: cmp      r4, #0
003bdd10: ble      #0x3bdd60
003bdd14: ldr      r3, [pc, #0x6c]
003bdd18: add      r6, sp, #4
003bdd1c: ldr      sl, [r5, r3]
003bdd20: mov      r0, sl
003bdd24: bl       #0x337888
003bdd28: ldr      r1, [pc, #0x5c]
003bdd2c: mov      r2, sp
003bdd30: mov      r0, r6
003bdd34: add      r1, pc, r1
003bdd38: bl       #0x3140ec
003bdd3c: mov      r1, r6
003bdd40: mov      r0, sl
003bdd44: bl       #0x337a88
003bdd48: mov      r0, r6
003bdd4c: bl       #0x318254
003bdd50: mov      r0, r7
003bdd54: mov      r2, r4
003bdd58: mov      r1, #0x24
003bdd5c: bl       #0x3e0708
003bdd60: ldr      r3, [r5, r8]
003bdd64: ldr      r2, [sp, #0x1c]
003bdd68: ldr      r3, [r3]
003bdd6c: cmp      r2, r3
003bdd70: bne      #0x3bdd7c
003bdd74: add      sp, sp, #0x24
003bdd78: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdd7c: bl       #0x30e310
003bdd80: ldrsbeq  r6, [sp], #-0xdc
003bdd84: andeq    r4, r0, ip, lsr #1
003bdd88: andeq    r0, r0, r4, lsl #17
003bdd8c: subseq   r6, r0, ip, asr fp

# _ZN14CharProperties10HandleDotsEv
003df3f0: push     {r4, r5, r6, r7, r8, lr}
003df3f4: add      r6, r0, #0xa90
003df3f8: sub      sp, sp, #0x30
003df3fc: mov      r5, r0
003df400: add      r6, r6, #4
003df404: mvn      r4, #0
003df408: add      r7, sp, #8
003df40c: add      r2, r4, #0x7f
003df410: mov      r1, r6
003df414: mov      r0, r5
003df418: bl       #0x3dedb4
003df41c: subs     r8, r0, #0
003df420: ble      #0x3df46c
003df424: ldr      r3, [r5, #4]
003df428: mov      r0, r3
003df42c: ldr      r3, [r3]
003df430: mov      lr, pc
003df434: ldr      pc, [r3, #0x34]
003df438: mov      r3, r8
003df43c: subs     r8, r0, #0
003df440: mov      r0, r7
003df444: bne      #0x3df46c
003df448: ldr      r1, [r5, #4]
003df44c: str      r4, [sp]
003df450: mov      r2, r1
003df454: bl       #0x3b2e68
003df458: ldr      r1, [r5, #4]
003df45c: mov      r3, r8
003df460: mov      r0, r7
003df464: mov      r2, r1
003df468: bl       #0x3b10b4
003df46c: add      r4, r4, #1
003df470: cmp      r4, #5
003df474: bne      #0x3df40c
003df478: add      sp, sp, #0x30
003df47c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN14CharProperties14RecalcPropertyEi
003dfe60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dfe64: sub      sp, sp, #0x54
003dfe68: mov      r6, r0
003dfe6c: mov      r7, r1
003dfe70: bl       #0x3deed8
003dfe74: tst      r0, #4
003dfe78: bne      #0x3e0110
003dfe7c: tst      r0, #2
003dfe80: bne      #0x3e02bc
003dfe84: tst      r0, #1
003dfe88: beq      #0x3e0080
003dfe8c: add      r2, r6, #0xa90
003dfe90: add      r2, r2, #4
003dfe94: str      r2, [sp, #4]
003dfe98: ldr      r3, [r6, #0xe20]
003dfe9c: add      sb, r6, #0xe10
003dfea0: add      sb, sb, #8
003dfea4: str      r3, [sp, #0xc]
003dfea8: ldr      r2, [sp, #0xc]
003dfeac: add      ip, sp, #0x20
003dfeb0: str      ip, [sp, #8]
003dfeb4: cmp      r2, sb
003dfeb8: add      r4, sp, #0x10
003dfebc: mov      r8, r6
003dfec0: beq      #0x3e0018
003dfec4: ldrb     r3, [sb]
003dfec8: cmp      r3, #0
003dfecc: bne      #0x3dfee4
003dfed0: ldr      r3, [sb, #4]
003dfed4: ldr      r3, [r3, #4]
003dfed8: cmp      r3, sb
003dfedc: ldreq    ip, [sb, #0xc]
003dfee0: beq      #0x3dff04
003dfee4: ldr      ip, [sb, #8]
003dfee8: cmp      ip, #0
003dfeec: bne      #0x3dfef8
003dfef0: b        #0x3e00b4
003dfef4: mov      ip, r3
003dfef8: ldr      r3, [ip, #0xc]
003dfefc: cmp      r3, #0
003dff00: bne      #0x3dfef4
003dff04: ldr      lr, [sp, #8]
003dff08: add      r5, ip, #0x34
003dff0c: ldm      r5, {r0, r1, r2, r3}
003dff10: stm      lr, {r0, r1, r2, r3}
003dff14: add      r0, ip, #0x44
003dff18: ldr      r1, [sp, #8]
003dff1c: bl       #0x3de870
003dff20: subs     sl, r0, #0
003dff24: beq      #0x3dffc8
003dff28: mov      fp, #0
003dff2c: mov      r6, fp
003dff30: ldm      r5, {r0, r1, r2, r3}
003dff34: stm      r4, {r0, r1, r2, r3}
003dff38: mov      r1, r6
003dff3c: mov      r0, r4
003dff40: bl       #0x3de8b4
003dff44: ldm      r5, {r0, r1, r2, r3}
003dff48: stm      r4, {r0, r1, r2, r3}
003dff4c: mov      r1, r6
003dff50: mov      r0, r4
003dff54: bl       #0x3de8b4
003dff58: ldr      r3, [sp, #0x10]
003dff5c: mov      r2, r7
003dff60: mov      r0, r8
003dff64: ldr      r1, [r3]
003dff68: bl       #0x3df114
003dff6c: cmp      r0, #0
003dff70: beq      #0x3dffb4
003dff74: ldm      r5, {r0, r1, r2, r3}
003dff78: stm      r4, {r0, r1, r2, r3}
003dff7c: mov      r1, r6
003dff80: mov      r0, r4
003dff84: bl       #0x3de8b4
003dff88: ldr      r3, [sp, #0x10]
003dff8c: mov      r2, r7
003dff90: mov      r0, r8
003dff94: ldr      r1, [r3]
003dff98: bl       #0x3dedb4
003dff9c: ldr      r1, [sp, #4]
003dffa0: mov      r3, r0
003dffa4: mov      r2, r7
003dffa8: mov      r0, r8
003dffac: bl       #0x3deca0
003dffb0: mov      fp, #1
003dffb4: add      r6, r6, #1
003dffb8: cmp      r6, sl
003dffbc: bne      #0x3dff30
003dffc0: cmp      fp, #0
003dffc4: bne      #0x3e0474
003dffc8: ldrb     r3, [sb]
003dffcc: cmp      r3, #0
003dffd0: bne      #0x3dffe8
003dffd4: ldr      r3, [sb, #4]
003dffd8: ldr      r3, [r3, #4]
003dffdc: cmp      r3, sb
003dffe0: ldreq    r3, [sb, #0xc]
003dffe4: beq      #0x3e0008
003dffe8: ldr      r3, [sb, #8]
003dffec: cmp      r3, #0
003dfff0: bne      #0x3dfffc
003dfff4: b        #0x3e00e4
003dfff8: mov      r3, r2
003dfffc: ldr      r2, [r3, #0xc]
003e0000: cmp      r2, #0
003e0004: bne      #0x3dfff8
003e0008: mov      sb, r3
003e000c: ldr      r2, [sp, #0xc]
003e0010: cmp      r2, sb
003e0014: bne      #0x3dfec4
003e0018: add      r4, r8, #0x710
003e001c: mov      r0, r8
003e0020: mov      r1, r4
003e0024: mov      r2, r7
003e0028: bl       #0x3df114
003e002c: cmp      r0, #0
003e0030: mov      r6, r8
003e0034: bne      #0x3e047c
003e0038: add      r4, r6, #0x38c
003e003c: mov      r0, r6
003e0040: mov      r1, r4
003e0044: mov      r2, r7
003e0048: bl       #0x3df114
003e004c: cmp      r0, #0
003e0050: bne      #0x3e047c
003e0054: add      r4, r6, #8
003e0058: mov      r0, r6
003e005c: mov      r1, r4
003e0060: mov      r2, r7
003e0064: bl       #0x3df114
003e0068: cmp      r0, #0
003e006c: bne      #0x3e047c
003e0070: mov      r0, r6
003e0074: mov      r1, r7
003e0078: bl       #0x3def10
003e007c: b        #0x3e048c
003e0080: tst      r0, #0x20
003e0084: bne      #0x3e0550
003e0088: tst      r0, #0x10
003e008c: addeq    ip, r6, #0xa90
003e0090: addeq    ip, ip, #4
003e0094: streq    ip, [sp, #4]
003e0098: bne      #0x3e0438
003e009c: mov      r0, r6
003e00a0: ldr      r1, [sp, #4]
003e00a4: mov      r2, r7
003e00a8: bl       #0x3dedb4
003e00ac: add      sp, sp, #0x54
003e00b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e00b4: ldr      ip, [sb, #4]
003e00b8: ldr      r3, [ip, #8]
003e00bc: cmp      r3, sb
003e00c0: beq      #0x3e00cc
003e00c4: b        #0x3dff04
003e00c8: mov      ip, r3
003e00cc: ldr      r3, [ip, #4]
003e00d0: ldr      r2, [r3, #8]
003e00d4: cmp      r2, ip
003e00d8: beq      #0x3e00c8
003e00dc: mov      ip, r3
003e00e0: b        #0x3dff04
003e00e4: ldr      r3, [sb, #4]
003e00e8: ldr      r2, [r3, #8]
003e00ec: cmp      sb, r2
003e00f0: bne      #0x3e0008
003e00f4: mov      r2, r3
003e00f8: ldr      r3, [r3, #4]
003e00fc: ldr      r1, [r3, #8]
003e0100: cmp      r1, r2
003e0104: beq      #0x3e00f4
003e0108: mov      sb, r3
003e010c: b        #0x3e000c
003e0110: add      r2, r6, #0xa90
003e0114: add      r2, r2, #4
003e0118: mov      r1, r7
003e011c: mov      r0, r6
003e0120: str      r2, [sp, #4]
003e0124: bl       #0x3def10
003e0128: add      r4, r6, #8
003e012c: mov      r3, r0
003e0130: ldr      r1, [sp, #4]
003e0134: mov      r0, r6
003e0138: mov      r2, r7
003e013c: bl       #0x3deca0
003e0140: mov      r0, r6
003e0144: mov      r1, r4
003e0148: mov      r2, r7
003e014c: bl       #0x3df114
003e0150: cmp      r0, #0
003e0154: bne      #0x3e0528
003e0158: add      r4, r6, #0x38c
003e015c: mov      r0, r6
003e0160: mov      r1, r4
003e0164: mov      r2, r7
003e0168: bl       #0x3df114
003e016c: cmp      r0, #0
003e0170: bne      #0x3e0500
003e0174: add      r4, r6, #0x710
003e0178: mov      r0, r6
003e017c: mov      r1, r4
003e0180: mov      r2, r7
003e0184: bl       #0x3df114
003e0188: cmp      r0, #0
003e018c: bne      #0x3e04d8
003e0190: add      r3, r6, #0xe10
003e0194: add      r3, r3, #8
003e0198: str      r3, [sp, #8]
003e019c: ldr      sb, [r6, #0xe20]
003e01a0: add      fp, sp, #0x40
003e01a4: add      r4, sp, #0x10
003e01a8: ldr      ip, [sp, #8]
003e01ac: cmp      sb, ip
003e01b0: beq      #0x3e009c
003e01b4: add      r8, sb, #0x34
003e01b8: ldm      r8, {r0, r1, r2, r3}
003e01bc: stm      fp, {r0, r1, r2, r3}
003e01c0: add      r0, sb, #0x44
003e01c4: mov      r1, fp
003e01c8: bl       #0x3de870
003e01cc: subs     sl, r0, #0
003e01d0: beq      #0x3e0260
003e01d4: mov      r5, #0
003e01d8: b        #0x3e01e8
003e01dc: add      r5, r5, #1
003e01e0: cmp      r5, sl
003e01e4: beq      #0x3e0260
003e01e8: ldm      r8, {r0, r1, r2, r3}
003e01ec: stm      r4, {r0, r1, r2, r3}
003e01f0: mov      r1, r5
003e01f4: mov      r0, r4
003e01f8: bl       #0x3de8b4
003e01fc: ldr      r3, [sp, #0x10]
003e0200: mov      r2, r7
003e0204: mov      r0, r6
003e0208: ldr      r1, [r3]
003e020c: bl       #0x3df114
003e0210: cmp      r0, #0
003e0214: beq      #0x3e01dc
003e0218: ldm      r8, {r0, r1, r2, r3}
003e021c: stm      r4, {r0, r1, r2, r3}
003e0220: mov      r1, r5
003e0224: mov      r0, r4
003e0228: bl       #0x3de8b4
003e022c: ldr      r3, [sp, #0x10]
003e0230: mov      r2, r7
003e0234: mov      r0, r6
003e0238: ldr      r1, [r3]
003e023c: bl       #0x3dedb4
003e0240: add      r5, r5, #1
003e0244: mov      r3, r0
003e0248: ldr      r1, [sp, #4]
003e024c: mov      r0, r6
003e0250: mov      r2, r7
003e0254: bl       #0x3df140
003e0258: cmp      r5, sl
003e025c: bne      #0x3e01e8
003e0260: ldr      r2, [sb, #0xc]
003e0264: cmp      r2, #0
003e0268: bne      #0x3e0274
003e026c: b        #0x3e0288
003e0270: mov      r2, r3
003e0274: ldr      r3, [r2, #8]
003e0278: cmp      r3, #0
003e027c: bne      #0x3e0270
003e0280: mov      sb, r2
003e0284: b        #0x3e01a8
003e0288: ldr      r3, [sb, #4]
003e028c: ldr      r1, [r3, #0xc]
003e0290: cmp      sb, r1
003e0294: bne      #0x3e02b0
003e0298: mov      sb, r3
003e029c: ldr      r3, [r3, #4]
003e02a0: ldr      r2, [r3, #0xc]
003e02a4: cmp      sb, r2
003e02a8: beq      #0x3e0298
003e02ac: ldr      r2, [sb, #0xc]
003e02b0: cmp      r3, r2
003e02b4: movne    sb, r3
003e02b8: b        #0x3e01a8
003e02bc: add      r4, r6, #8
003e02c0: mov      r1, r4
003e02c4: mov      r0, r6
003e02c8: mov      r2, r7
003e02cc: bl       #0x3df114
003e02d0: cmp      r0, #0
003e02d4: movne    r1, r4
003e02d8: bne      #0x3e043c
003e02dc: add      r4, r6, #0x38c
003e02e0: mov      r0, r6
003e02e4: mov      r1, r4
003e02e8: mov      r2, r7
003e02ec: bl       #0x3df114
003e02f0: cmp      r0, #0
003e02f4: bne      #0x3e05a8
003e02f8: add      r4, r6, #0x710
003e02fc: mov      r0, r6
003e0300: mov      r1, r4
003e0304: mov      r2, r7
003e0308: bl       #0x3df114
003e030c: cmp      r0, #0
003e0310: bne      #0x3e05f8
003e0314: add      lr, r6, #0xe10
003e0318: add      r2, r6, #0xa90
003e031c: add      lr, lr, #8
003e0320: add      r2, r2, #4
003e0324: str      lr, [sp, #0xc]
003e0328: str      r2, [sp, #4]
003e032c: add      r3, sp, #0x30
003e0330: ldr      sl, [r6, #0xe20]
003e0334: add      r4, sp, #0x10
003e0338: str      r3, [sp, #8]
003e033c: mov      r8, r6
003e0340: ldr      lr, [sp, #0xc]
003e0344: cmp      lr, sl
003e0348: beq      #0x3e05e4
003e034c: ldr      ip, [sp, #8]
003e0350: add      r5, sl, #0x34
003e0354: ldm      r5, {r0, r1, r2, r3}
003e0358: stm      ip, {r0, r1, r2, r3}
003e035c: add      r0, sl, #0x44
003e0360: ldr      r1, [sp, #8]
003e0364: bl       #0x3de870
003e0368: subs     sb, r0, #0
003e036c: beq      #0x3e0410
003e0370: mov      r6, #0
003e0374: mov      fp, r6
003e0378: ldm      r5, {r0, r1, r2, r3}
003e037c: stm      r4, {r0, r1, r2, r3}
003e0380: mov      r1, r6
003e0384: mov      r0, r4
003e0388: bl       #0x3de8b4
003e038c: ldm      r5, {r0, r1, r2, r3}
003e0390: stm      r4, {r0, r1, r2, r3}
003e0394: mov      r1, r6
003e0398: mov      r0, r4
003e039c: bl       #0x3de8b4
003e03a0: ldr      r3, [sp, #0x10]
003e03a4: mov      r2, r7
003e03a8: mov      r0, r8
003e03ac: ldr      r1, [r3]
003e03b0: bl       #0x3df114
003e03b4: cmp      r0, #0
003e03b8: beq      #0x3e03fc
003e03bc: ldm      r5, {r0, r1, r2, r3}
003e03c0: stm      r4, {r0, r1, r2, r3}
003e03c4: mov      r1, r6
003e03c8: mov      r0, r4
003e03cc: bl       #0x3de8b4
003e03d0: ldr      r3, [sp, #0x10]
003e03d4: mov      r2, r7
003e03d8: mov      r0, r8
003e03dc: ldr      r1, [r3]
003e03e0: bl       #0x3dedb4
003e03e4: ldr      r1, [sp, #4]
003e03e8: mov      r3, r0
003e03ec: mov      r2, r7
003e03f0: mov      r0, r8
003e03f4: bl       #0x3deca0
003e03f8: mov      fp, #1
003e03fc: add      r6, r6, #1
003e0400: cmp      r6, sb
003e0404: bne      #0x3e0378
003e0408: cmp      fp, #0
003e040c: bne      #0x3e0474
003e0410: ldr      r2, [sl, #0xc]
003e0414: cmp      r2, #0
003e0418: beq      #0x3e04a4
003e041c: mov      sl, r2
003e0420: b        #0x3e0428
003e0424: mov      sl, r3
003e0428: ldr      r3, [sl, #8]
003e042c: cmp      r3, #0
003e0430: bne      #0x3e0424
003e0434: b        #0x3e0340
003e0438: add      r1, r6, #8
003e043c: mov      r2, r7
003e0440: add      lr, r6, #0xa90
003e0444: mov      r0, r6
003e0448: str      lr, [sp, #4]
003e044c: bl       #0x3dedb4
003e0450: ldr      r2, [sp, #4]
003e0454: mov      r3, r0
003e0458: add      r2, r2, #4
003e045c: str      r2, [sp, #4]
003e0460: mov      r1, r2
003e0464: mov      r0, r6
003e0468: mov      r2, r7
003e046c: bl       #0x3deca0
003e0470: b        #0x3e009c
003e0474: mov      r6, r8
003e0478: b        #0x3e009c
003e047c: mov      r1, r4
003e0480: mov      r0, r6
003e0484: mov      r2, r7
003e0488: bl       #0x3dedb4
003e048c: mov      r3, r0
003e0490: ldr      r1, [sp, #4]
003e0494: mov      r0, r6
003e0498: mov      r2, r7
003e049c: bl       #0x3deca0
003e04a0: b        #0x3e009c
003e04a4: ldr      r3, [sl, #4]
003e04a8: ldr      r1, [r3, #0xc]
003e04ac: cmp      sl, r1
003e04b0: bne      #0x3e04cc
003e04b4: mov      sl, r3
003e04b8: ldr      r3, [r3, #4]
003e04bc: ldr      r2, [r3, #0xc]
003e04c0: cmp      r2, sl
003e04c4: beq      #0x3e04b4
003e04c8: ldr      r2, [sl, #0xc]
003e04cc: cmp      r3, r2
003e04d0: movne    sl, r3
003e04d4: b        #0x3e0340
003e04d8: mov      r1, r4
003e04dc: mov      r2, r7
003e04e0: mov      r0, r6
003e04e4: bl       #0x3dedb4
003e04e8: ldr      r1, [sp, #4]
003e04ec: mov      r3, r0
003e04f0: mov      r2, r7
003e04f4: mov      r0, r6
003e04f8: bl       #0x3df140
003e04fc: b        #0x3e0190
003e0500: mov      r1, r4
003e0504: mov      r2, r7
003e0508: mov      r0, r6
003e050c: bl       #0x3dedb4
003e0510: ldr      r1, [sp, #4]
003e0514: mov      r3, r0
003e0518: mov      r2, r7
003e051c: mov      r0, r6
003e0520: bl       #0x3df140
003e0524: b        #0x3e0174
003e0528: mov      r1, r4
003e052c: mov      r2, r7
003e0530: mov      r0, r6
003e0534: bl       #0x3dedb4
003e0538: ldr      r1, [sp, #4]
003e053c: mov      r3, r0
003e0540: mov      r2, r7
003e0544: mov      r0, r6
003e0548: bl       #0x3df140
003e054c: b        #0x3e0158
003e0550: add      r3, r6, #0xa90
003e0554: add      r3, r3, #4
003e0558: add      r1, r6, #8
003e055c: mov      r2, r7
003e0560: mov      r0, r6
003e0564: str      r3, [sp, #4]
003e0568: bl       #0x3dedb4
003e056c: ldr      r1, [sp, #4]
003e0570: mov      r3, r0
003e0574: mov      r2, r7
003e0578: mov      r0, r6
003e057c: bl       #0x3deca0
003e0580: add      r1, r6, #0x38c
003e0584: mov      r2, r7
003e0588: mov      r0, r6
003e058c: bl       #0x3dedb4
003e0590: ldr      r1, [sp, #4]
003e0594: mov      r3, r0
003e0598: mov      r2, r7
003e059c: mov      r0, r6
003e05a0: bl       #0x3df140
003e05a4: b        #0x3e009c
003e05a8: add      r3, r6, #0xa90
003e05ac: mov      r1, r4
003e05b0: mov      r2, r7
003e05b4: mov      r0, r6
003e05b8: str      r3, [sp, #4]
003e05bc: bl       #0x3dedb4
003e05c0: ldr      ip, [sp, #4]
003e05c4: mov      r3, r0
003e05c8: mov      r2, r7
003e05cc: add      ip, ip, #4
003e05d0: mov      r0, r6
003e05d4: mov      r1, ip
003e05d8: str      ip, [sp, #4]
003e05dc: bl       #0x3deca0
003e05e0: b        #0x3e009c
003e05e4: mov      r0, r8
003e05e8: mov      r1, r7
003e05ec: mov      r6, r8
003e05f0: bl       #0x3def10
003e05f4: b        #0x3e048c
003e05f8: mov      r2, r7
003e05fc: mov      r1, r4
003e0600: mov      r0, r6
003e0604: bl       #0x3dedb4
003e0608: add      r2, r6, #0xa90
003e060c: mov      r3, r0
003e0610: b        #0x3e0458
