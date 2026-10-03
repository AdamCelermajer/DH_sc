
# _ZN6glitch5scene22CSceneCollisionManager17getCollisionPointERKNS_4core6line3dIfEEPNS0_17ITriangleSelectorERNS2_8vector3dIfEERNS2_10triangle3dIfEE
006c62e8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006c62ec: subs     r4, r2, #0
006c62f0: sub      sp, sp, #0xbc
006c62f4: mov      r7, r0
006c62f8: mov      sl, r1
006c62fc: str      r3, [sp, #0x38]
006c6300: beq      #0x6c6ab8
006c6304: ldr      r3, [r4]
006c6308: mov      r0, r4
006c630c: mov      lr, pc
006c6310: ldr      pc, [r3, #0xc]
006c6314: ldr      r3, [r7, #0x14]
006c6318: mov      r5, r0
006c631c: ldr      r0, [r7, #0x10]
006c6320: rsb      r3, r0, r3
006c6324: asr      r3, r3, #2
006c6328: str      r0, [sp, #0x14]
006c632c: lsl      r2, r3, #3
006c6330: rsb      r2, r3, r2
006c6334: add      r2, r2, r2, lsl #6
006c6338: add      r2, r3, r2, lsl #3
006c633c: lsl      r1, r2, #0xf
006c6340: rsb      r2, r2, r1
006c6344: add      r3, r3, r2, lsl #3
006c6348: cmp      r5, r3
006c634c: bgt      #0x6c6a74
006c6350: ldr      r3, [sl]
006c6354: ldr      sb, [sl, #0xc]
006c6358: ldr      r2, [sl, #8]
006c635c: ldr      fp, [sl, #4]
006c6360: mov      ip, #0
006c6364: mov      r0, r3
006c6368: mov      r1, sb
006c636c: str      ip, [sp, #0xb4]
006c6370: str      r2, [sp, #0x8c]
006c6374: str      r3, [sp, #0x78]
006c6378: str      r2, [sp, #0x80]
006c637c: str      r3, [sp, #0x84]
006c6380: str      fp, [sp, #0x7c]
006c6384: str      fp, [sp, #0x88]
006c6388: bl       #0x30e70c
006c638c: ldr      r8, [sl, #0x10]
006c6390: cmp      r0, #0
006c6394: mov      r0, fp
006c6398: mov      r1, r8
006c639c: ldr      r6, [sl, #0x14]
006c63a0: strne    sb, [sp, #0x84]
006c63a4: bl       #0x30e70c
006c63a8: ldr      r1, [sp, #0x8c]
006c63ac: cmp      r0, #0
006c63b0: mov      r0, r6
006c63b4: strne    r8, [sp, #0x88]
006c63b8: bl       #0x30e2f8
006c63bc: ldr      r1, [sp, #0x78]
006c63c0: cmp      r0, #0
006c63c4: mov      r0, sb
006c63c8: strne    r6, [sp, #0x8c]
006c63cc: bl       #0x30e70c
006c63d0: ldr      r1, [sp, #0x7c]
006c63d4: cmp      r0, #0
006c63d8: mov      r0, r8
006c63dc: strne    sb, [sp, #0x78]
006c63e0: bl       #0x30e70c
006c63e4: ldr      r1, [sp, #0x80]
006c63e8: cmp      r0, #0
006c63ec: mov      r0, r6
006c63f0: strne    r8, [sp, #0x7c]
006c63f4: bl       #0x30e70c
006c63f8: cmp      r0, #0
006c63fc: strne    r6, [sp, #0x80]
006c6400: add      lr, sp, #0x78
006c6404: ldr      ip, [r4]
006c6408: str      lr, [sp]
006c640c: mov      lr, #0
006c6410: str      lr, [sp, #4]
006c6414: mov      r2, r5
006c6418: add      r3, sp, #0xb4
006c641c: mov      r0, r4
006c6420: ldr      r1, [sp, #0x14]
006c6424: mov      lr, pc
006c6428: ldr      pc, [ip, #0x14]
006c642c: ldr      r1, [sl, #4]
006c6430: ldr      r0, [sl, #0x10]
006c6434: bl       #0x30e3ac
006c6438: ldr      r1, [sl, #8]
006c643c: mov      r5, r0
006c6440: ldr      r0, [sl, #0x14]
006c6444: bl       #0x30e3ac
006c6448: ldr      r1, [sl]
006c644c: mov      r4, r0
006c6450: ldr      r0, [sl, #0xc]
006c6454: bl       #0x30e3ac
006c6458: str      r0, [sp, #0x9c]
006c645c: add      r0, sp, #0x9c
006c6460: str      r5, [sp, #0xa0]
006c6464: str      r4, [sp, #0xa4]
006c6468: bl       #0x35e8e0
006c646c: ldr      r3, [r0]
006c6470: ldr      r8, [sl]
006c6474: ldr      sb, [sl, #0xc]
006c6478: str      r3, [sp, #0xa8]
006c647c: ldr      r2, [r0, #4]
006c6480: mov      r3, #0
006c6484: mov      r1, sb
006c6488: str      r2, [sp, #0xac]
006c648c: ldr      r2, [r0, #8]
006c6490: mov      r0, r8
006c6494: str      r3, [sp, #0x98]
006c6498: str      r2, [sp, #0xb0]
006c649c: str      r3, [sp, #0x90]
006c64a0: str      r3, [sp, #0x94]
006c64a4: bl       #0x30e3ac
006c64a8: ldr      r2, [sl, #4]
006c64ac: mov      r4, r0
006c64b0: str      r2, [sp, #0x18]
006c64b4: ldr      r3, [sl, #0x10]
006c64b8: mov      r0, r2
006c64bc: mov      r1, r3
006c64c0: str      r3, [sp, #0x1c]
006c64c4: bl       #0x30e3ac
006c64c8: mov      r5, r0
006c64cc: ldr      r0, [sl, #8]
006c64d0: str      r0, [sp, #0x20]
006c64d4: ldr      r1, [sl, #0x14]
006c64d8: str      r1, [sp, #0x24]
006c64dc: bl       #0x30e3ac
006c64e0: mov      r1, r4
006c64e4: mov      r6, r0
006c64e8: mov      r0, r4
006c64ec: bl       #0x30ed6c
006c64f0: mov      r1, r5
006c64f4: mov      r4, r0
006c64f8: mov      r0, r5
006c64fc: bl       #0x30ed6c
006c6500: mov      r1, r0
006c6504: mov      r0, r4
006c6508: bl       #0x30eba4
006c650c: mov      r1, r6
006c6510: mov      r4, r0
006c6514: mov      r0, r6
006c6518: bl       #0x30ed6c
006c651c: mov      r1, r0
006c6520: mov      r0, r4
006c6524: bl       #0x30eba4
006c6528: mov      r1, sb
006c652c: str      r0, [sp, #0x3c]
006c6530: mov      r0, r8
006c6534: bl       #0x30e70c
006c6538: cmp      r0, #0
006c653c: moveq    r3, sb
006c6540: ldr      r1, [sp, #0x1c]
006c6544: ldr      r0, [sp, #0x18]
006c6548: moveq    sb, r8
006c654c: moveq    r8, r3
006c6550: bl       #0x30e70c
006c6554: cmp      r0, #0
006c6558: ldreq    r3, [sp, #0x1c]
006c655c: ldreq    r2, [sp, #0x18]
006c6560: ldr      r1, [sp, #0x24]
006c6564: ldr      r0, [sp, #0x20]
006c6568: streq    r3, [sp, #0x18]
006c656c: streq    r2, [sp, #0x1c]
006c6570: bl       #0x30e70c
006c6574: cmp      r0, #0
006c6578: ldreq    r3, [sp, #0x24]
006c657c: ldreq    r0, [sp, #0x20]
006c6580: ldr      r1, [sp, #0xb4]
006c6584: streq    r3, [sp, #0x20]
006c6588: streq    r0, [sp, #0x24]
006c658c: cmp      r1, #0
006c6590: str      r1, [sp, #0x14]
006c6594: ble      #0x6c6ab8
006c6598: mvn      r2, #0x80000000
006c659c: mov      r4, #0
006c65a0: sub      r2, r2, #0x800000
006c65a4: add      r3, sp, #0xa8
006c65a8: add      r0, sp, #0x90
006c65ac: str      r2, [sp, #0x34]
006c65b0: mov      r5, r4
006c65b4: str      r4, [sp, #0x2c]
006c65b8: str      r3, [sp, #0x40]
006c65bc: str      r0, [sp, #0x44]
006c65c0: str      sl, [sp, #0x30]
006c65c4: ldr      r6, [r7, #0x10]
006c65c8: mov      r1, r8
006c65cc: ldr      sl, [r6, r4]
006c65d0: add      r6, r6, r4
006c65d4: mov      r0, sl
006c65d8: bl       #0x30e70c
006c65dc: cmp      r0, #0
006c65e0: beq      #0x6c660c
006c65e4: ldr      r0, [r6, #0xc]
006c65e8: mov      r1, r8
006c65ec: bl       #0x30e70c
006c65f0: cmp      r0, #0
006c65f4: beq      #0x6c660c
006c65f8: ldr      r0, [r6, #0x18]
006c65fc: mov      r1, r8
006c6600: bl       #0x30e70c
006c6604: cmp      r0, #0
006c6608: bne      #0x6c66b0
006c660c: mov      r0, sb
006c6610: mov      r1, sl
006c6614: bl       #0x30e70c
006c6618: cmp      r0, #0
006c661c: beq      #0x6c6648
006c6620: ldr      r0, [r6, #0xc]
006c6624: mov      r1, sb
006c6628: bl       #0x30e2f8
006c662c: cmp      r0, #0
006c6630: beq      #0x6c6648
006c6634: ldr      r0, [r6, #0x18]
006c6638: mov      r1, sb
006c663c: bl       #0x30e2f8
006c6640: cmp      r0, #0
006c6644: bne      #0x6c66b0
006c6648: ldr      fp, [r6, #4]
006c664c: ldr      r1, [sp, #0x18]
006c6650: mov      r0, fp
006c6654: bl       #0x30e70c
006c6658: cmp      r0, #0
006c665c: beq      #0x6c6674
006c6660: ldr      r0, [r6, #0x10]
006c6664: ldr      r1, [sp, #0x18]
006c6668: bl       #0x30e70c
006c666c: cmp      r0, #0
006c6670: bne      #0x6c6a1c
006c6674: ldr      r0, [sp, #0x1c]
006c6678: mov      r1, fp
006c667c: bl       #0x30e70c
006c6680: cmp      r0, #0
006c6684: beq      #0x6c66d0
006c6688: ldr      r0, [r6, #0x10]
006c668c: ldr      r1, [sp, #0x1c]
006c6690: bl       #0x30e2f8
006c6694: cmp      r0, #0
006c6698: beq      #0x6c66d0
006c669c: ldr      r0, [r6, #0x1c]
006c66a0: ldr      r1, [sp, #0x1c]
006c66a4: bl       #0x30e2f8
006c66a8: cmp      r0, #0
006c66ac: beq      #0x6c66d0
006c66b0: ldr      r2, [sp, #0x14]
006c66b4: add      r5, r5, #1
006c66b8: add      r4, r4, #0x24
006c66bc: cmp      r2, r5
006c66c0: bgt      #0x6c65c4
006c66c4: ldr      r0, [sp, #0x2c]
006c66c8: add      sp, sp, #0xbc
006c66cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006c66d0: ldr      r0, [r6, #8]
006c66d4: ldr      r1, [sp, #0x20]
006c66d8: str      r0, [sp, #0x28]
006c66dc: bl       #0x30e70c
006c66e0: cmp      r0, #0
006c66e4: bne      #0x6c6a48
006c66e8: ldr      r0, [sp, #0x24]
006c66ec: ldr      r1, [sp, #0x28]
006c66f0: bl       #0x30e70c
006c66f4: cmp      r0, #0
006c66f8: beq      #0x6c6724
006c66fc: ldr      r0, [r6, #0x14]
006c6700: ldr      r1, [sp, #0x24]
006c6704: bl       #0x30e2f8
006c6708: cmp      r0, #0
006c670c: beq      #0x6c6724
006c6710: ldr      r0, [r6, #0x20]
006c6714: ldr      r1, [sp, #0x24]
006c6718: bl       #0x30e2f8
006c671c: cmp      r0, #0
006c6720: bne      #0x6c66b0
006c6724: ldr      r1, [sp, #0x30]
006c6728: ldr      r1, [r1]
006c672c: str      r1, [sp, #0x48]
006c6730: ldr      r0, [sp, #0x48]
006c6734: mov      r1, sl
006c6738: bl       #0x30e3ac
006c673c: ldr      r2, [sp, #0x30]
006c6740: mov      r3, r0
006c6744: mov      r1, fp
006c6748: ldr      r2, [r2, #4]
006c674c: str      r3, [sp, #0x10]
006c6750: mov      r0, r2
006c6754: str      r2, [sp, #0x4c]
006c6758: bl       #0x30e3ac
006c675c: mov      r2, r0
006c6760: ldr      r0, [sp, #0x30]
006c6764: ldr      r1, [sp, #0x28]
006c6768: ldr      sl, [r0, #8]
006c676c: str      r2, [sp, #8]
006c6770: mov      r0, sl
006c6774: bl       #0x30e3ac
006c6778: ldr      r3, [sp, #0x10]
006c677c: mov      ip, r0
006c6780: str      ip, [sp, #0xc]
006c6784: mov      r1, r3
006c6788: mov      r0, r3
006c678c: bl       #0x30ed6c
006c6790: ldr      r2, [sp, #8]
006c6794: mov      fp, r0
006c6798: mov      r1, r2
006c679c: mov      r0, r2
006c67a0: bl       #0x30ed6c
006c67a4: mov      r1, r0
006c67a8: mov      r0, fp
006c67ac: bl       #0x30eba4
006c67b0: ldr      ip, [sp, #0xc]
006c67b4: mov      fp, r0
006c67b8: mov      r1, ip
006c67bc: mov      r0, ip
006c67c0: bl       #0x30ed6c
006c67c4: mov      r1, r0
006c67c8: mov      r0, fp
006c67cc: bl       #0x30eba4
006c67d0: mov      r1, r0
006c67d4: ldr      r0, [sp, #0x34]
006c67d8: bl       #0x30e9ac
006c67dc: cmp      r0, #0
006c67e0: bne      #0x6c6ac4
006c67e4: mov      r0, r6
006c67e8: ldr      r1, [sp, #0x30]
006c67ec: ldr      r2, [sp, #0x40]
006c67f0: ldr      r3, [sp, #0x44]
006c67f4: bl       #0x58615c
006c67f8: cmp      r0, #0
006c67fc: beq      #0x6c6bf8
006c6800: ldr      r2, [sp, #0x30]
006c6804: ldr      fp, [sp, #0x90]
006c6808: ldr      r1, [r2]
006c680c: mov      r0, fp
006c6810: bl       #0x30e3ac
006c6814: ldr      r3, [sp, #0x94]
006c6818: mov      sl, r0
006c681c: ldr      r0, [sp, #0x30]
006c6820: str      r3, [sp, #0x28]
006c6824: ldr      r1, [r0, #4]
006c6828: mov      r0, r3
006c682c: bl       #0x30e3ac
006c6830: ldr      r1, [sp, #0x98]
006c6834: ldr      r2, [sp, #0x30]
006c6838: mov      r3, r0
006c683c: str      r1, [sp, #0x48]
006c6840: ldr      r1, [r2, #8]
006c6844: ldr      r0, [sp, #0x48]
006c6848: str      r3, [sp, #0x10]
006c684c: bl       #0x30e3ac
006c6850: mov      r1, sl
006c6854: mov      r2, r0
006c6858: mov      r0, sl
006c685c: str      r2, [sp, #8]
006c6860: bl       #0x30ed6c
006c6864: ldr      r3, [sp, #0x10]
006c6868: mov      sl, r0
006c686c: mov      r1, r3
006c6870: mov      r0, r3
006c6874: bl       #0x30ed6c
006c6878: mov      r1, r0
006c687c: mov      r0, sl
006c6880: bl       #0x30eba4
006c6884: ldr      r2, [sp, #8]
006c6888: mov      sl, r0
006c688c: mov      r1, r2
006c6890: mov      r0, r2
006c6894: bl       #0x30ed6c
006c6898: mov      r1, r0
006c689c: mov      r0, sl
006c68a0: bl       #0x30eba4
006c68a4: mov      r1, r0
006c68a8: mov      sl, r0
006c68ac: ldr      r0, [sp, #0x3c]
006c68b0: bl       #0x30e2f8
006c68b4: ldr      r3, [sp, #0x30]
006c68b8: cmp      r0, #0
006c68bc: ldr      r0, [sp, #0x30]
006c68c0: ldr      r2, [r3, #0xc]
006c68c4: ldr      r3, [r3, #0x10]
006c68c8: ldr      r1, [r0, #0x14]
006c68cc: beq      #0x6c6bf8
006c68d0: ldr      r0, [sp, #0x48]
006c68d4: str      r2, [sp, #8]
006c68d8: str      r3, [sp, #0x10]
006c68dc: bl       #0x30e3ac
006c68e0: ldr      r3, [sp, #0x10]
006c68e4: mov      ip, r0
006c68e8: ldr      r0, [sp, #0x28]
006c68ec: mov      r1, r3
006c68f0: str      ip, [sp, #0xc]
006c68f4: bl       #0x30e3ac
006c68f8: ldr      r2, [sp, #8]
006c68fc: mov      r3, r0
006c6900: mov      r0, fp
006c6904: mov      r1, r2
006c6908: str      r3, [sp, #0x10]
006c690c: bl       #0x30e3ac
006c6910: mov      r1, r0
006c6914: bl       #0x30ed6c
006c6918: ldr      r3, [sp, #0x10]
006c691c: mov      r2, r0
006c6920: str      r2, [sp, #8]
006c6924: mov      r1, r3
006c6928: mov      r0, r3
006c692c: bl       #0x30ed6c
006c6930: ldr      r2, [sp, #8]
006c6934: mov      r1, r0
006c6938: mov      r0, r2
006c693c: bl       #0x30eba4
006c6940: ldr      ip, [sp, #0xc]
006c6944: mov      r3, r0
006c6948: str      r3, [sp, #0x10]
006c694c: mov      r1, ip
006c6950: mov      r0, ip
006c6954: bl       #0x30ed6c
006c6958: ldr      r3, [sp, #0x10]
006c695c: mov      r1, r0
006c6960: mov      r0, r3
006c6964: bl       #0x30eba4
006c6968: mov      r1, r0
006c696c: ldr      r0, [sp, #0x3c]
006c6970: bl       #0x30e2f8
006c6974: cmp      r0, #0
006c6978: ldreq    r2, [sp, #0xb4]
006c697c: streq    r2, [sp, #0x14]
006c6980: beq      #0x6c66b0
006c6984: ldr      r0, [sp, #0x34]
006c6988: mov      r1, sl
006c698c: bl       #0x30e2f8
006c6990: cmp      r0, #0
006c6994: ldreq    r3, [sp, #0xb4]
006c6998: streq    r3, [sp, #0x14]
006c699c: beq      #0x6c66b0
006c69a0: ldr      r3, [r6]
006c69a4: ldr      r0, [sp, #0xb4]
006c69a8: ldr      r1, [sp, #0xe0]
006c69ac: str      sl, [sp, #0x34]
006c69b0: str      r0, [sp, #0x14]
006c69b4: str      r3, [r1]
006c69b8: ldr      r3, [r6, #4]
006c69bc: mov      r2, #1
006c69c0: str      r2, [sp, #0x2c]
006c69c4: str      r3, [r1, #4]
006c69c8: ldr      r3, [r6, #8]
006c69cc: str      r3, [r1, #8]
006c69d0: ldr      r3, [r6, #0xc]
006c69d4: str      r3, [r1, #0xc]
006c69d8: ldr      r3, [r6, #0x10]
006c69dc: str      r3, [r1, #0x10]
006c69e0: ldr      r3, [r6, #0x14]
006c69e4: str      r3, [r1, #0x14]
006c69e8: ldr      r3, [r6, #0x18]
006c69ec: str      r3, [r1, #0x18]
006c69f0: ldr      r3, [r6, #0x1c]
006c69f4: str      r3, [r1, #0x1c]
006c69f8: ldr      r3, [r6, #0x20]
006c69fc: str      r3, [r1, #0x20]
006c6a00: ldr      r3, [sp, #0x38]
006c6a04: str      fp, [r3]
006c6a08: ldr      r0, [sp, #0x28]
006c6a0c: str      r0, [r3, #4]
006c6a10: ldr      r1, [sp, #0x48]
006c6a14: str      r1, [r3, #8]
006c6a18: b        #0x6c66b0
006c6a1c: ldr      r0, [r6, #0x1c]
006c6a20: ldr      r1, [sp, #0x18]
006c6a24: bl       #0x30e70c
006c6a28: cmp      r0, #0
006c6a2c: beq      #0x6c6674
006c6a30: ldr      r2, [sp, #0x14]
006c6a34: add      r5, r5, #1
006c6a38: add      r4, r4, #0x24
006c6a3c: cmp      r2, r5
006c6a40: bgt      #0x6c65c4
006c6a44: b        #0x6c66c4
006c6a48: ldr      r0, [r6, #0x14]
006c6a4c: ldr      r1, [sp, #0x20]
006c6a50: bl       #0x30e70c
006c6a54: cmp      r0, #0
006c6a58: beq      #0x6c66e8
006c6a5c: ldr      r0, [r6, #0x20]
006c6a60: ldr      r1, [sp, #0x20]
006c6a64: bl       #0x30e70c
006c6a68: cmp      r0, #0
006c6a6c: bne      #0x6c66b0
006c6a70: b        #0x6c66e8
006c6a74: mov      r3, #0
006c6a78: mov      r1, r5
006c6a7c: add      r0, r7, #0x10
006c6a80: add      r2, sp, #0x54
006c6a84: str      r3, [sp, #0x74]
006c6a88: str      r3, [sp, #0x54]
006c6a8c: str      r3, [sp, #0x58]
006c6a90: str      r3, [sp, #0x5c]
006c6a94: str      r3, [sp, #0x60]
006c6a98: str      r3, [sp, #0x64]
006c6a9c: str      r3, [sp, #0x68]
006c6aa0: str      r3, [sp, #0x6c]
006c6aa4: str      r3, [sp, #0x70]
006c6aa8: bl       #0x587dfc
006c6aac: ldr      r1, [r7, #0x10]
006c6ab0: str      r1, [sp, #0x14]
006c6ab4: b        #0x6c6350
006c6ab8: mov      r3, #0
006c6abc: str      r3, [sp, #0x2c]
006c6ac0: b        #0x6c66c4
006c6ac4: ldr      r1, [r6, #0xc]
006c6ac8: ldr      r0, [sp, #0x48]
006c6acc: bl       #0x30e3ac
006c6ad0: ldr      r1, [r6, #0x10]
006c6ad4: mov      fp, r0
006c6ad8: ldr      r0, [sp, #0x4c]
006c6adc: bl       #0x30e3ac
006c6ae0: ldr      r1, [r6, #0x14]
006c6ae4: mov      r3, r0
006c6ae8: mov      r0, sl
006c6aec: str      r3, [sp, #0x10]
006c6af0: bl       #0x30e3ac
006c6af4: mov      r1, fp
006c6af8: mov      r2, r0
006c6afc: mov      r0, fp
006c6b00: str      r2, [sp, #8]
006c6b04: bl       #0x30ed6c
006c6b08: ldr      r3, [sp, #0x10]
006c6b0c: mov      fp, r0
006c6b10: mov      r1, r3
006c6b14: mov      r0, r3
006c6b18: bl       #0x30ed6c
006c6b1c: mov      r1, r0
006c6b20: mov      r0, fp
006c6b24: bl       #0x30eba4
006c6b28: ldr      r2, [sp, #8]
006c6b2c: mov      fp, r0
006c6b30: mov      r1, r2
006c6b34: mov      r0, r2
006c6b38: bl       #0x30ed6c
006c6b3c: mov      r1, r0
006c6b40: mov      r0, fp
006c6b44: bl       #0x30eba4
006c6b48: mov      r1, r0
006c6b4c: ldr      r0, [sp, #0x34]
006c6b50: bl       #0x30e9ac
006c6b54: cmp      r0, #0
006c6b58: beq      #0x6c67e4
006c6b5c: ldr      r1, [r6, #0x18]
006c6b60: ldr      r0, [sp, #0x48]
006c6b64: bl       #0x30e3ac
006c6b68: ldr      r1, [r6, #0x1c]
006c6b6c: mov      fp, r0
006c6b70: ldr      r0, [sp, #0x4c]
006c6b74: bl       #0x30e3ac
006c6b78: ldr      r1, [r6, #0x20]
006c6b7c: mov      r3, r0
006c6b80: mov      r0, sl
006c6b84: str      r3, [sp, #0x10]
006c6b88: bl       #0x30e3ac
006c6b8c: mov      r1, fp
006c6b90: mov      r2, r0
006c6b94: mov      r0, fp
006c6b98: str      r2, [sp, #8]
006c6b9c: bl       #0x30ed6c
006c6ba0: ldr      r3, [sp, #0x10]
006c6ba4: mov      sl, r0
006c6ba8: mov      r1, r3
006c6bac: mov      r0, r3
006c6bb0: bl       #0x30ed6c
006c6bb4: mov      r1, r0
006c6bb8: mov      r0, sl
006c6bbc: bl       #0x30eba4
006c6bc0: ldr      r2, [sp, #8]
006c6bc4: mov      sl, r0
006c6bc8: mov      r1, r2
006c6bcc: mov      r0, r2
006c6bd0: bl       #0x30ed6c
006c6bd4: mov      r1, r0
006c6bd8: mov      r0, sl
006c6bdc: bl       #0x30eba4
006c6be0: mov      r1, r0
006c6be4: ldr      r0, [sp, #0x34]
006c6be8: bl       #0x30e9ac
006c6bec: cmp      r0, #0
006c6bf0: bne      #0x6c66b0
006c6bf4: b        #0x6c67e4
006c6bf8: ldr      r1, [sp, #0xb4]
006c6bfc: str      r1, [sp, #0x14]
006c6c00: b        #0x6c66b0

# _ZNK6glitch4core10triangle3dIfE12isOnSameSideERKNS0_8vector3dIfEES6_S6_S6_
00585bb8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00585bbc: sub      sp, sp, #0x1c
00585bc0: ldr      r4, [r3]
00585bc4: mov      r6, r3
00585bc8: ldr      r3, [sp, #0x40]
00585bcc: mov      r5, r1
00585bd0: mov      r1, r4
00585bd4: ldr      r0, [r3]
00585bd8: mov      sl, r2
00585bdc: bl       #0x30e3ac
00585be0: ldr      sb, [r6, #4]
00585be4: ldr      r3, [sp, #0x40]
00585be8: mov      r8, r0
00585bec: mov      r1, sb
00585bf0: ldr      r0, [r3, #4]
00585bf4: add      r3, r8, #0x80000000
00585bf8: str      r3, [sp, #8]
00585bfc: bl       #0x30e3ac
00585c00: ldr      fp, [r6, #8]
00585c04: ldr      r3, [sp, #0x40]
00585c08: mov      r7, r0
00585c0c: mov      r1, fp
00585c10: ldr      r0, [r3, #8]
00585c14: add      r3, r7, #0x80000000
00585c18: str      r3, [sp, #4]
00585c1c: bl       #0x30e3ac
00585c20: mov      r1, r4
00585c24: mov      r6, r0
00585c28: ldr      r0, [r5]
00585c2c: bl       #0x30e3ac
00585c30: str      r0, [sp, #0xc]
00585c34: ldr      r0, [r5, #4]
00585c38: mov      r1, sb
00585c3c: bl       #0x30e3ac
00585c40: str      r0, [sp, #0x10]
00585c44: ldr      r0, [r5, #8]
00585c48: mov      r1, fp
00585c4c: bl       #0x30e3ac
00585c50: str      r0, [sp, #0x14]
00585c54: mov      r1, r4
00585c58: ldr      r0, [sl]
00585c5c: bl       #0x30e3ac
00585c60: mov      r1, sb
00585c64: mov      r4, r0
00585c68: ldr      r0, [sl, #4]
00585c6c: bl       #0x30e3ac
00585c70: mov      r1, fp
00585c74: mov      r5, r0
00585c78: ldr      r0, [sl, #8]
00585c7c: bl       #0x30e3ac
00585c80: ldr      r1, [sp, #4]
00585c84: mov      sl, r0
00585c88: ldr      r0, [sp, #0x14]
00585c8c: bl       #0x30ed6c
00585c90: ldr      r1, [sp, #0x10]
00585c94: mov      sb, r0
00585c98: mov      r0, r6
00585c9c: bl       #0x30ed6c
00585ca0: mov      r1, r0
00585ca4: mov      r0, sb
00585ca8: bl       #0x30eba4
00585cac: mov      r1, sl
00585cb0: mov      sb, r0
00585cb4: ldr      r0, [sp, #4]
00585cb8: bl       #0x30ed6c
00585cbc: mov      r1, r5
00585cc0: mov      fp, r0
00585cc4: mov      r0, r6
00585cc8: bl       #0x30ed6c
00585ccc: mov      r1, r0
00585cd0: mov      r0, fp
00585cd4: bl       #0x30eba4
00585cd8: mov      r1, r0
00585cdc: mov      r0, sb
00585ce0: bl       #0x30ed6c
00585ce4: add      r6, r6, #0x80000000
00585ce8: mov      sb, r0
00585cec: mov      r1, r6
00585cf0: ldr      r0, [sp, #0xc]
00585cf4: bl       #0x30ed6c
00585cf8: ldr      r1, [sp, #0x14]
00585cfc: mov      fp, r0
00585d00: mov      r0, r8
00585d04: bl       #0x30ed6c
00585d08: mov      r1, r0
00585d0c: mov      r0, fp
00585d10: bl       #0x30eba4
00585d14: mov      r1, r4
00585d18: mov      fp, r0
00585d1c: mov      r0, r6
00585d20: bl       #0x30ed6c
00585d24: mov      r1, sl
00585d28: mov      r6, r0
00585d2c: mov      r0, r8
00585d30: bl       #0x30ed6c
00585d34: mov      r1, r0
00585d38: mov      r0, r6
00585d3c: bl       #0x30eba4
00585d40: mov      r1, r0
00585d44: mov      r0, fp
00585d48: bl       #0x30ed6c
00585d4c: mov      r1, r0
00585d50: mov      r0, sb
00585d54: bl       #0x30eba4
00585d58: ldr      r1, [sp, #8]
00585d5c: mov      r6, r0
00585d60: ldr      r0, [sp, #0x10]
00585d64: bl       #0x30ed6c
00585d68: ldr      r1, [sp, #0xc]
00585d6c: mov      r8, r0
00585d70: mov      r0, r7
00585d74: bl       #0x30ed6c
00585d78: mov      r1, r0
00585d7c: mov      r0, r8
00585d80: bl       #0x30eba4
00585d84: mov      r1, r5
00585d88: mov      r8, r0
00585d8c: ldr      r0, [sp, #8]
00585d90: bl       #0x30ed6c
00585d94: mov      r1, r4
00585d98: mov      r5, r0
00585d9c: mov      r0, r7
00585da0: bl       #0x30ed6c
00585da4: mov      r1, r0
00585da8: mov      r0, r5
00585dac: bl       #0x30eba4
00585db0: mov      r1, r0
00585db4: mov      r0, r8
00585db8: bl       #0x30ed6c
00585dbc: mov      r1, r0
00585dc0: mov      r0, r6
00585dc4: bl       #0x30eba4
00585dc8: mov      r1, #0
00585dcc: bl       #0x30e4b4
00585dd0: cmp      r0, #0
00585dd4: mov      r0, #0
00585dd8: movne    r0, #1
00585ddc: and      r0, r0, #1
00585de0: add      sp, sp, #0x1c
00585de4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN7PFFloor14GetCollisionAtERK7Point3DIfERS1_RN6glitch4core10triangle3dIfEE
0051b96c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0051b970: ldr      r5, [r1]
0051b974: sub      sp, sp, #0x30
0051b978: mov      r6, r1
0051b97c: mov      r4, r0
0051b980: mov      r1, r5
0051b984: ldr      r0, [r0, #0x44]
0051b988: mov      r7, r2
0051b98c: mov      sl, r3
0051b990: bl       #0x30e9ac
0051b994: ldr      r8, [pc, #0x138]
0051b998: cmp      r0, #0
0051b99c: add      r8, pc, r8
0051b9a0: beq      #0x51b9b8
0051b9a4: mov      r0, r5
0051b9a8: ldr      r1, [r4, #0x50]
0051b9ac: bl       #0x30e9ac
0051b9b0: cmp      r0, #0
0051b9b4: bne      #0x51b9c4
0051b9b8: mov      r0, #0
0051b9bc: add      sp, sp, #0x30
0051b9c0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0051b9c4: ldr      sb, [r6, #4]
0051b9c8: ldr      r0, [r4, #0x48]
0051b9cc: mov      r1, sb
0051b9d0: bl       #0x30e9ac
0051b9d4: cmp      r0, #0
0051b9d8: beq      #0x51b9b8
0051b9dc: mov      r0, sb
0051b9e0: ldr      r1, [r4, #0x54]
0051b9e4: bl       #0x30e9ac
0051b9e8: cmp      r0, #0
0051b9ec: beq      #0x51b9b8
0051b9f0: ldr      r6, [r6, #8]
0051b9f4: ldr      r0, [r4, #0x4c]
0051b9f8: mov      r1, r6
0051b9fc: bl       #0x30e9ac
0051ba00: cmp      r0, #0
0051ba04: beq      #0x51b9b8
0051ba08: mov      r0, r6
0051ba0c: ldr      r1, [r4, #0x58]
0051ba10: bl       #0x30e9ac
0051ba14: cmp      r0, #0
0051ba18: beq      #0x51b9b8
0051ba1c: ldr      r2, [pc, #0xb4]
0051ba20: mov      r1, #0x44000000
0051ba24: mov      r3, #0
0051ba28: ldr      r2, [r8, r2]
0051ba2c: add      r1, r1, #0x7a0000
0051ba30: mov      r0, r6
0051ba34: ldr      r2, [r2, #0x10]
0051ba38: ldr      r8, [r2, #0x1c]
0051ba3c: str      r3, [sp, #0x2c]
0051ba40: str      r3, [sp, #0x24]
0051ba44: str      r3, [sp, #0x28]
0051ba48: str      r5, [sp, #0x18]
0051ba4c: str      r5, [sp, #0xc]
0051ba50: str      sb, [sp, #0x1c]
0051ba54: str      sb, [sp, #0x10]
0051ba58: bl       #0x30eba4
0051ba5c: mov      r1, #0x44000000
0051ba60: add      r1, r1, #0x7a0000
0051ba64: str      r0, [sp, #0x14]
0051ba68: mov      r0, r6
0051ba6c: bl       #0x30e3ac
0051ba70: str      r0, [sp, #0x20]
0051ba74: ldr      r5, [r8, #0x2c]
0051ba78: ldr      r3, [r4, #0x40]
0051ba7c: ldr      r2, [r5]
0051ba80: mov      r0, r3
0051ba84: ldr      r3, [r3]
0051ba88: ldr      r4, [r2, #0xc]
0051ba8c: mov      lr, pc
0051ba90: ldr      pc, [r3, #0xb0]
0051ba94: str      sl, [sp]
0051ba98: mov      r2, r0
0051ba9c: add      r1, sp, #0xc
0051baa0: mov      r0, r5
0051baa4: add      r3, sp, #0x24
0051baa8: blx      r4
0051baac: cmp      r0, #0
0051bab0: beq      #0x51b9b8
0051bab4: ldr      r2, [sp, #0x28]
0051bab8: ldr      r3, [sp, #0x2c]
0051babc: ldr      r1, [sp, #0x24]
0051bac0: mov      r0, #1
0051bac4: str      r2, [r7, #4]
0051bac8: str      r1, [r7]
0051bacc: str      r3, [r7, #8]
0051bad0: b        #0x51b9bc
0051bad4: strdeq   sb, sl, [r7], #-4
0051bad8: strdeq   r3, r4, [r0], -r4

# _ZN6glitch4core8vector3dIfE9normalizeEv
0035e8e0: push     {r4, r5, r6, r7, r8, lr}
0035e8e4: mov      r4, r0
0035e8e8: ldr      r0, [r0]
0035e8ec: ldr      r7, [r4, #4]
0035e8f0: ldr      r6, [r4, #8]
0035e8f4: mov      r1, r0
0035e8f8: bl       #0x30ed6c
0035e8fc: mov      r1, r7
0035e900: mov      r5, r0
0035e904: mov      r0, r7
0035e908: bl       #0x30ed6c
0035e90c: mov      r1, r0
0035e910: mov      r0, r5
0035e914: bl       #0x30eba4
0035e918: mov      r1, r6
0035e91c: mov      r5, r0
0035e920: mov      r0, r6
0035e924: bl       #0x30ed6c
0035e928: mov      r1, r0
0035e92c: mov      r0, r5
0035e930: bl       #0x30eba4
0035e934: mov      r1, #0
0035e938: mov      r5, r0
0035e93c: bl       #0x30df8c
0035e940: cmp      r0, #0
0035e944: bne      #0x35e990
0035e948: mov      r0, r5
0035e94c: bl       #0x30e124
0035e950: mov      r1, r0
0035e954: mov      r0, #0x3f800000
0035e958: bl       #0x30ec94
0035e95c: mov      r5, r0
0035e960: mov      r1, r0
0035e964: ldr      r0, [r4]
0035e968: bl       #0x30ed6c
0035e96c: mov      r1, r5
0035e970: str      r0, [r4]
0035e974: ldr      r0, [r4, #4]
0035e978: bl       #0x30ed6c
0035e97c: mov      r1, r5
0035e980: str      r0, [r4, #4]
0035e984: ldr      r0, [r4, #8]
0035e988: bl       #0x30ed6c
0035e98c: str      r0, [r4, #8]
0035e990: mov      r0, r4
0035e994: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch4core10triangle3dIfE23getIntersectionWithLineERKNS0_8vector3dIfEES6_RS4_
0058615c: push     {r4, r5, r6, lr}
00586160: mov      r5, r0
00586164: mov      r4, r3
00586168: bl       #0x585e80
0058616c: cmp      r0, #0
00586170: bne      #0x586178
00586174: pop      {r4, r5, r6, pc}
00586178: mov      r0, r5
0058617c: mov      r1, r4
00586180: pop      {r4, r5, r6, lr}
00586184: b        #0x585de8

# _ZNK6glitch4core10triangle3dIfE30getIntersectionOfPlaneWithLineERKNS0_8vector3dIfEES6_RS4_
00585e80: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00585e84: ldr      r5, [r0]
00585e88: sub      sp, sp, #0x2c
00585e8c: str      r1, [sp, #0xc]
00585e90: mov      r4, r0
00585e94: mov      r1, r5
00585e98: ldr      r0, [r0, #0xc]
00585e9c: mov      r6, r2
00585ea0: str      r3, [sp, #0x10]
00585ea4: bl       #0x30e3ac
00585ea8: ldr      r8, [r4, #4]
00585eac: mov      sl, r0
00585eb0: ldr      r0, [r4, #0x10]
00585eb4: mov      r1, r8
00585eb8: bl       #0x30e3ac
00585ebc: ldr      fp, [r4, #8]
00585ec0: mov      r7, r0
00585ec4: ldr      r0, [r4, #0x14]
00585ec8: mov      r1, fp
00585ecc: bl       #0x30e3ac
00585ed0: mov      r1, r5
00585ed4: mov      sb, r0
00585ed8: ldr      r0, [r4, #0x18]
00585edc: bl       #0x30e3ac
00585ee0: mov      r1, r8
00585ee4: mov      r5, r0
00585ee8: ldr      r0, [r4, #0x1c]
00585eec: bl       #0x30e3ac
00585ef0: mov      r1, fp
00585ef4: mov      r8, r0
00585ef8: ldr      r0, [r4, #0x20]
00585efc: bl       #0x30e3ac
00585f00: add      r1, r7, #0x80000000
00585f04: str      r0, [sp, #4]
00585f08: bl       #0x30ed6c
00585f0c: mov      r1, r8
00585f10: mov      fp, r0
00585f14: mov      r0, sb
00585f18: bl       #0x30ed6c
00585f1c: mov      r1, r0
00585f20: mov      r0, fp
00585f24: bl       #0x30eba4
00585f28: add      r1, sb, #0x80000000
00585f2c: str      r0, [sp, #0x1c]
00585f30: mov      r0, r5
00585f34: bl       #0x30ed6c
00585f38: ldr      r3, [sp, #4]
00585f3c: mov      sb, r0
00585f40: mov      r0, sl
00585f44: mov      r1, r3
00585f48: bl       #0x30ed6c
00585f4c: mov      r1, r0
00585f50: mov      r0, sb
00585f54: bl       #0x30eba4
00585f58: add      r1, sl, #0x80000000
00585f5c: str      r0, [sp, #0x20]
00585f60: mov      r0, r8
00585f64: bl       #0x30ed6c
00585f68: mov      r1, r5
00585f6c: mov      r8, r0
00585f70: mov      r0, r7
00585f74: bl       #0x30ed6c
00585f78: mov      r1, r0
00585f7c: mov      r0, r8
00585f80: bl       #0x30eba4
00585f84: str      r0, [sp, #0x24]
00585f88: add      r0, sp, #0x1c
00585f8c: bl       #0x35e8e0
00585f90: ldr      sb, [r6]
00585f94: ldr      r8, [r0]
00585f98: mov      r3, r0
00585f9c: ldr      r7, [r0, #4]
00585fa0: mov      r1, sb
00585fa4: mov      r0, r8
00585fa8: ldr      r5, [r3, #8]
00585fac: bl       #0x30ed6c
00585fb0: ldr      sl, [r6, #4]
00585fb4: mov      fp, r0
00585fb8: mov      r0, r7
00585fbc: mov      r1, sl
00585fc0: bl       #0x30ed6c
00585fc4: mov      r1, r0
00585fc8: mov      r0, fp
00585fcc: bl       #0x30eba4
00585fd0: ldr      r6, [r6, #8]
00585fd4: mov      fp, r0
00585fd8: mov      r0, r5
00585fdc: mov      r1, r6
00585fe0: bl       #0x30ed6c
00585fe4: mov      r1, r0
00585fe8: mov      r0, fp
00585fec: bl       #0x30eba4
00585ff0: movw     r1, #0x37bd
00585ff4: mov      fp, r0
00585ff8: movt     r1, #0x3586
00585ffc: bic      r0, r0, #0x80000000
00586000: bl       #0x30e9ac
00586004: cmp      r0, #0
00586008: movne    r0, #0
0058600c: bne      #0x586154
00586010: ldr      ip, [sp, #0xc]
00586014: mov      r0, r8
00586018: ldr      r3, [ip]
0058601c: ldr      r2, [ip, #4]
00586020: mov      r1, r3
00586024: str      r3, [sp, #4]
00586028: str      r2, [sp, #0x14]
0058602c: bl       #0x30ed6c
00586030: ldr      ip, [sp, #0xc]
00586034: mov      r2, r0
00586038: ldr      r1, [sp, #0x14]
0058603c: ldr      ip, [ip, #8]
00586040: mov      r0, r7
00586044: str      r2, [sp, #8]
00586048: str      ip, [sp, #0xc]
0058604c: bl       #0x30ed6c
00586050: ldr      r2, [sp, #8]
00586054: mov      r1, r0
00586058: mov      r0, r2
0058605c: bl       #0x30eba4
00586060: ldr      r1, [sp, #0xc]
00586064: mov      r2, r0
00586068: mov      r0, r5
0058606c: str      r2, [sp, #8]
00586070: bl       #0x30ed6c
00586074: ldr      r2, [sp, #8]
00586078: mov      r1, r0
0058607c: mov      r0, r2
00586080: bl       #0x30eba4
00586084: ldr      r1, [r4]
00586088: mov      r2, r0
0058608c: mov      r0, r8
00586090: str      r2, [sp, #8]
00586094: bl       #0x30ed6c
00586098: ldr      r1, [r4, #4]
0058609c: mov      r8, r0
005860a0: mov      r0, r7
005860a4: bl       #0x30ed6c
005860a8: mov      r1, r0
005860ac: mov      r0, r8
005860b0: bl       #0x30eba4
005860b4: ldr      r1, [r4, #8]
005860b8: mov      r7, r0
005860bc: mov      r0, r5
005860c0: bl       #0x30ed6c
005860c4: mov      r1, r0
005860c8: mov      r0, r7
005860cc: bl       #0x30eba4
005860d0: ldr      r2, [sp, #8]
005860d4: mov      r1, r0
005860d8: mov      r0, r2
005860dc: bl       #0x30e3ac
005860e0: mov      r1, fp
005860e4: add      r0, r0, #0x80000000
005860e8: bl       #0x30ec94
005860ec: mov      r1, sb
005860f0: mov      r4, r0
005860f4: bl       #0x30ed6c
005860f8: ldr      r3, [sp, #4]
005860fc: mov      r1, r0
00586100: mov      r0, r3
00586104: bl       #0x30eba4
00586108: ldr      r2, [sp, #0x10]
0058610c: mov      r1, sl
00586110: str      r0, [r2]
00586114: mov      r0, r4
00586118: bl       #0x30ed6c
0058611c: mov      r1, r0
00586120: ldr      r0, [sp, #0x14]
00586124: bl       #0x30eba4
00586128: ldr      r3, [sp, #0x10]
0058612c: mov      r1, r6
00586130: str      r0, [r3, #4]
00586134: mov      r0, r4
00586138: bl       #0x30ed6c
0058613c: mov      r1, r0
00586140: ldr      r0, [sp, #0xc]
00586144: bl       #0x30eba4
00586148: ldr      ip, [sp, #0x10]
0058614c: str      r0, [ip, #8]
00586150: mov      r0, #1
00586154: add      sp, sp, #0x2c
00586158: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN7PFFloor16GetFloorHeightAtERK7Point3DIfEPfPS1_
0051badc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0051bae0: sub      sp, sp, #0x34
0051bae4: mov      ip, #0
0051bae8: mov      r5, r2
0051baec: mov      r4, r3
0051baf0: add      r2, sp, #0x24
0051baf4: mov      r3, sp
0051baf8: str      ip, [sp, #0x20]
0051bafc: str      ip, [sp, #0x24]
0051bb00: str      ip, [sp, #0x28]
0051bb04: str      ip, [sp, #0x2c]
0051bb08: str      ip, [sp]
0051bb0c: str      ip, [sp, #4]
0051bb10: str      ip, [sp, #8]
0051bb14: str      ip, [sp, #0xc]
0051bb18: str      ip, [sp, #0x10]
0051bb1c: str      ip, [sp, #0x14]
0051bb20: str      ip, [sp, #0x18]
0051bb24: str      ip, [sp, #0x1c]
0051bb28: bl       #0x51b96c
0051bb2c: cmp      r0, #0
0051bb30: beq      #0x51bc38
0051bb34: cmp      r5, #0
0051bb38: ldrne    r3, [sp, #0x2c]
0051bb3c: strne    r3, [r5]
0051bb40: cmp      r4, #0
0051bb44: beq      #0x51bc34
0051bb48: ldr      r5, [sp]
0051bb4c: ldr      r0, [sp, #0xc]
0051bb50: mov      r1, r5
0051bb54: bl       #0x30e3ac
0051bb58: ldr      r7, [sp, #4]
0051bb5c: mov      r8, r0
0051bb60: ldr      r0, [sp, #0x10]
0051bb64: mov      r1, r7
0051bb68: bl       #0x30e3ac
0051bb6c: ldr      sb, [sp, #8]
0051bb70: mov      r6, r0
0051bb74: ldr      r0, [sp, #0x14]
0051bb78: mov      r1, sb
0051bb7c: bl       #0x30e3ac
0051bb80: mov      r1, r5
0051bb84: mov      sl, r0
0051bb88: ldr      r0, [sp, #0x18]
0051bb8c: bl       #0x30e3ac
0051bb90: mov      r1, r7
0051bb94: mov      r5, r0
0051bb98: ldr      r0, [sp, #0x1c]
0051bb9c: bl       #0x30e3ac
0051bba0: mov      r1, sb
0051bba4: mov      r7, r0
0051bba8: ldr      r0, [sp, #0x20]
0051bbac: bl       #0x30e3ac
0051bbb0: add      r1, r6, #0x80000000
0051bbb4: mov      sb, r0
0051bbb8: bl       #0x30ed6c
0051bbbc: mov      r1, r7
0051bbc0: mov      fp, r0
0051bbc4: mov      r0, sl
0051bbc8: bl       #0x30ed6c
0051bbcc: mov      r1, r0
0051bbd0: mov      r0, fp
0051bbd4: bl       #0x30eba4
0051bbd8: add      r1, sl, #0x80000000
0051bbdc: str      r0, [r4]
0051bbe0: mov      r0, r5
0051bbe4: bl       #0x30ed6c
0051bbe8: mov      r1, sb
0051bbec: mov      sl, r0
0051bbf0: mov      r0, r8
0051bbf4: bl       #0x30ed6c
0051bbf8: mov      r1, r0
0051bbfc: mov      r0, sl
0051bc00: bl       #0x30eba4
0051bc04: add      r1, r8, #0x80000000
0051bc08: str      r0, [r4, #4]
0051bc0c: mov      r0, r7
0051bc10: bl       #0x30ed6c
0051bc14: mov      r1, r5
0051bc18: mov      r7, r0
0051bc1c: mov      r0, r6
0051bc20: bl       #0x30ed6c
0051bc24: mov      r1, r0
0051bc28: mov      r0, r7
0051bc2c: bl       #0x30eba4
0051bc30: str      r0, [r4, #8]
0051bc34: mov      r0, #1
0051bc38: add      sp, sp, #0x34
0051bc3c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch4core10triangle3dIfE13isPointInsideERKNS0_8vector3dIfEE
00585de8: push     {r4, r5, r6, r7, lr}
00585dec: add      r5, r0, #0xc
00585df0: sub      sp, sp, #0xc
00585df4: add      r6, r0, #0x18
00585df8: mov      r2, r0
00585dfc: mov      r3, r5
00585e00: mov      r4, r0
00585e04: str      r6, [sp]
00585e08: mov      r7, r1
00585e0c: bl       #0x585bb8
00585e10: cmp      r0, #0
00585e14: bne      #0x585e24
00585e18: mov      r0, #0
00585e1c: add      sp, sp, #0xc
00585e20: pop      {r4, r5, r6, r7, pc}
00585e24: mov      r0, r4
00585e28: mov      r1, r7
00585e2c: mov      r2, r5
00585e30: mov      r3, r4
00585e34: str      r6, [sp]
00585e38: bl       #0x585bb8
00585e3c: cmp      r0, #0
00585e40: beq      #0x585e18
00585e44: mov      r0, r4
00585e48: mov      r1, r7
00585e4c: mov      r2, r6
00585e50: mov      r3, r4
00585e54: str      r5, [sp]
00585e58: bl       #0x585bb8
00585e5c: b        #0x585e1c
