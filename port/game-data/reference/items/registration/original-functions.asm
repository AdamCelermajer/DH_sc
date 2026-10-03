
# _ZN12PyDataArrays10reloadDataEP11IStreamBasePKc
004bd478: push     {r4, r5, r6, lr}
004bd47c: sub      sp, sp, #8
004bd480: add      r3, sp, #8
004bd484: str      r2, [r3, #-4]!
004bd488: add      r5, r0, #4
004bd48c: mov      r4, r1
004bd490: mov      r0, r5
004bd494: mov      r1, r3
004bd498: bl       #0x4bd2bc
004bd49c: cmp      r0, r5
004bd4a0: beq      #0x4bd4c8
004bd4a4: ldr      r6, [r0, #0x2c]
004bd4a8: ldr      r5, [r0, #0x28]
004bd4ac: cmp      r5, r6
004bd4b0: beq      #0x4bd4c8
004bd4b4: ldr      r3, [r5], #8
004bd4b8: mov      r0, r4
004bd4bc: blx      r3
004bd4c0: cmp      r5, r6
004bd4c4: bne      #0x4bd4b4
004bd4c8: add      sp, sp, #8
004bd4cc: pop      {r4, r5, r6, pc}

# _ZN12PyDataArrays15addFuncsForFileEPKcPFvP11IStreamBaseEPFvvE
004be3e0: push     {r4, r5, r6, r7, r8, sl, lr}
004be3e4: sub      sp, sp, #0x1c
004be3e8: add      r4, sp, #0x18
004be3ec: str      r1, [r4, #-0x14]!
004be3f0: add      r5, r0, #4
004be3f4: mov      r0, r5
004be3f8: mov      r1, r4
004be3fc: mov      r6, r2
004be400: mov      r7, r3
004be404: bl       #0x4bd2bc
004be408: cmp      r5, r0
004be40c: beq      #0x4be518
004be410: mov      r0, r5
004be414: mov      r1, r4
004be418: bl       #0x4be274
004be41c: ldr      r5, [r0, #4]
004be420: ldr      r3, [r0, #8]
004be424: mov      r4, r0
004be428: cmp      r5, r3
004be42c: beq      #0x4be448
004be430: stm      r5, {r6, r7}
004be434: ldr      r3, [r0, #4]
004be438: add      r3, r3, #8
004be43c: str      r3, [r0, #4]
004be440: add      sp, sp, #0x1c
004be444: pop      {r4, r5, r6, r7, r8, sl, pc}
004be448: ldr      r3, [r0]
004be44c: rsb      r3, r3, r5
004be450: asr      r3, r3, #3
004be454: cmp      r3, #1
004be458: addhs    r1, r3, r3
004be45c: addlo    r1, r3, #1
004be460: cmn      r1, #0xe0000001
004be464: bhi      #0x4be510
004be468: cmp      r3, r1
004be46c: bhi      #0x4be510
004be470: add      r2, sp, #0x18
004be474: str      r1, [r2, #-4]!
004be478: add      r0, r4, #8
004be47c: bl       #0x4afae0
004be480: ldr      sl, [r4]
004be484: mov      r8, r0
004be488: rsb      r5, sl, r5
004be48c: asr      r5, r5, #3
004be490: cmp      r5, #0
004be494: movle    r5, r0
004be498: ble      #0x4be4cc
004be49c: mov      r1, r5
004be4a0: mov      r0, #0
004be4a4: mov      r2, sl
004be4a8: ldr      ip, [r2, r0]!
004be4ac: mov      r3, r8
004be4b0: subs     r1, r1, #1
004be4b4: str      ip, [r3, r0]!
004be4b8: ldr      r2, [r2, #4]
004be4bc: add      r0, r0, #8
004be4c0: str      r2, [r3, #4]
004be4c4: bne      #0x4be4a4
004be4c8: add      r5, r8, r5, lsl #3
004be4cc: mov      sl, r5
004be4d0: str      r7, [r5, #4]
004be4d4: str      r6, [sl], #8
004be4d8: ldr      r0, [r4]
004be4dc: ldr      r1, [r4, #8]
004be4e0: cmp      r0, #0
004be4e4: beq      #0x4be4fc
004be4e8: rsb      r1, r0, r1
004be4ec: bic      r1, r1, #7
004be4f0: cmp      r1, #0x80
004be4f4: bhi      #0x4be548
004be4f8: bl       #0x708f00
004be4fc: ldr      r3, [sp, #0x14]
004be500: stm      r4, {r8, sl}
004be504: add      r8, r8, r3, lsl #3
004be508: str      r8, [r4, #8]
004be50c: b        #0x4be440
004be510: mvn      r1, #0xe0000000
004be514: b        #0x4be470
004be518: mov      r1, r4
004be51c: add      r8, sp, #8
004be520: bl       #0x4be274
004be524: mov      r3, #0
004be528: mov      r1, r8
004be52c: str      r3, [sp, #0x10]
004be530: str      r3, [sp, #8]
004be534: str      r3, [sp, #0xc]
004be538: bl       #0x4afba8
004be53c: mov      r0, r8
004be540: bl       #0x4af7b4
004be544: b        #0x4be410
004be548: bl       #0x310440
004be54c: b        #0x4be4fc

# _ZN12PyDataArraysC1EP19DataReloaderManager
004be550: push     {r4, r5, r6, r7, r8, lr}
004be554: ldr      r4, [pc, #0xff4]
004be558: ldr      r2, [pc, #0xff4]
004be55c: mov      ip, #0
004be560: add      r4, pc, r4
004be564: ldr      r2, [r4, r2]
004be568: mov      r3, r0
004be56c: str      ip, [r0, #8]
004be570: add      r2, r2, #8
004be574: str      r2, [r0]
004be578: strb     ip, [r3, #4]!
004be57c: str      r3, [r0, #0x10]
004be580: str      r3, [r0, #0xc]
004be584: ldr      r3, [pc, #0xfcc]
004be588: mov      lr, r0
004be58c: str      ip, [r0, #0x14]
004be590: str      ip, [r0, #0x20]
004be594: ldr      r2, [r4, r3]
004be598: strb     ip, [lr, #0x1c]!
004be59c: ldr      r3, [pc, #0xfb8]
004be5a0: str      r1, [r0, #0x34]
004be5a4: ldr      r1, [pc, #0xfb4]
004be5a8: str      ip, [r0, #0x38]
004be5ac: str      ip, [r0, #0x2c]
004be5b0: str      lr, [r0, #0x28]
004be5b4: str      lr, [r0, #0x24]
004be5b8: ldr      r3, [r4, r3]
004be5bc: add      r1, pc, r1
004be5c0: mov      r5, r0
004be5c4: bl       #0x4be3e0
004be5c8: ldr      r3, [pc, #0xf94]
004be5cc: ldr      r1, [pc, #0xf94]
004be5d0: mov      r0, r5
004be5d4: ldr      r2, [r4, r3]
004be5d8: ldr      r3, [pc, #0xf8c]
004be5dc: add      r1, pc, r1
004be5e0: ldr      r7, [pc, #0xf88]
004be5e4: ldr      r3, [r4, r3]
004be5e8: bl       #0x4be3e0
004be5ec: ldr      r3, [pc, #0xf80]
004be5f0: ldr      r1, [pc, #0xf80]
004be5f4: mov      r0, r5
004be5f8: ldr      r2, [r4, r3]
004be5fc: add      r1, pc, r1
004be600: bl       #0x4bdccc
004be604: ldr      r3, [pc, #0xf70]
004be608: ldr      r1, [pc, #0xf70]
004be60c: mov      r0, r5
004be610: ldr      r2, [r4, r3]
004be614: add      r1, pc, r1
004be618: bl       #0x4bdccc
004be61c: ldr      r3, [pc, #0xf60]
004be620: ldr      r1, [pc, #0xf60]
004be624: mov      r0, r5
004be628: ldr      r2, [r4, r3]
004be62c: ldr      r3, [pc, #0xf58]
004be630: add      r1, pc, r1
004be634: add      r7, pc, r7
004be638: ldr      r3, [r4, r3]
004be63c: bl       #0x4be3e0
004be640: ldr      r3, [pc, #0xf48]
004be644: ldr      r1, [pc, #0xf48]
004be648: mov      r0, r5
004be64c: ldr      r2, [r4, r3]
004be650: ldr      r3, [pc, #0xf40]
004be654: add      r1, pc, r1
004be658: ldr      r6, [pc, #0xf3c]
004be65c: ldr      r3, [r4, r3]
004be660: bl       #0x4be3e0
004be664: ldr      r3, [pc, #0xf34]
004be668: ldr      r1, [pc, #0xf34]
004be66c: mov      r0, r5
004be670: ldr      r2, [r4, r3]
004be674: add      r1, pc, r1
004be678: bl       #0x4bdccc
004be67c: ldr      r3, [pc, #0xf24]
004be680: ldr      r1, [pc, #0xf24]
004be684: mov      r0, r5
004be688: ldr      r2, [r4, r3]
004be68c: add      r1, pc, r1
004be690: bl       #0x4bdccc
004be694: ldr      r3, [pc, #0xf14]
004be698: mov      r0, r5
004be69c: mov      r1, r7
004be6a0: ldr      r2, [r4, r3]
004be6a4: ldr      r3, [pc, #0xf08]
004be6a8: add      r6, pc, r6
004be6ac: ldr      r3, [r4, r3]
004be6b0: bl       #0x4be3e0
004be6b4: ldr      r3, [pc, #0xefc]
004be6b8: mov      r0, r5
004be6bc: mov      r1, r6
004be6c0: ldr      r2, [r4, r3]
004be6c4: ldr      r3, [pc, #0xef0]
004be6c8: ldr      r3, [r4, r3]
004be6cc: bl       #0x4be3e0
004be6d0: ldr      r3, [pc, #0xee8]
004be6d4: ldr      r1, [pc, #0xee8]
004be6d8: mov      r0, r5
004be6dc: ldr      r2, [r4, r3]
004be6e0: add      r1, pc, r1
004be6e4: bl       #0x4bdccc
004be6e8: ldr      r3, [pc, #0xed8]
004be6ec: ldr      r1, [pc, #0xed8]
004be6f0: mov      r0, r5
004be6f4: ldr      r2, [r4, r3]
004be6f8: add      r1, pc, r1
004be6fc: bl       #0x4bdccc
004be700: ldr      r3, [pc, #0xec8]
004be704: mov      r0, r5
004be708: mov      r1, r7
004be70c: ldr      r2, [r4, r3]
004be710: ldr      r3, [pc, #0xebc]
004be714: ldr      r3, [r4, r3]
004be718: bl       #0x4be3e0
004be71c: ldr      r3, [pc, #0xeb4]
004be720: mov      r0, r5
004be724: mov      r1, r6
004be728: ldr      r2, [r4, r3]
004be72c: ldr      r3, [pc, #0xea8]
004be730: ldr      r3, [r4, r3]
004be734: bl       #0x4be3e0
004be738: ldr      r3, [pc, #0xea0]
004be73c: ldr      r1, [pc, #0xea0]
004be740: mov      r0, r5
004be744: ldr      r2, [r4, r3]
004be748: add      r1, pc, r1
004be74c: bl       #0x4bdccc
004be750: ldr      r3, [pc, #0xe90]
004be754: ldr      r1, [pc, #0xe90]
004be758: mov      r0, r5
004be75c: ldr      r2, [r4, r3]
004be760: add      r1, pc, r1
004be764: bl       #0x4bdccc
004be768: ldr      r3, [pc, #0xe80]
004be76c: mov      r1, r7
004be770: mov      r0, r5
004be774: ldr      r2, [r4, r3]
004be778: ldr      r3, [pc, #0xe74]
004be77c: ldr      r7, [pc, #0xe74]
004be780: ldr      r3, [r4, r3]
004be784: bl       #0x4be3e0
004be788: ldr      r3, [pc, #0xe6c]
004be78c: mov      r1, r6
004be790: mov      r0, r5
004be794: ldr      r2, [r4, r3]
004be798: ldr      r3, [pc, #0xe60]
004be79c: add      r7, pc, r7
004be7a0: ldr      r6, [pc, #0xe5c]
004be7a4: ldr      r3, [r4, r3]
004be7a8: bl       #0x4be3e0
004be7ac: ldr      r3, [pc, #0xe54]
004be7b0: ldr      r1, [pc, #0xe54]
004be7b4: mov      r0, r5
004be7b8: ldr      r2, [r4, r3]
004be7bc: add      r1, pc, r1
004be7c0: bl       #0x4bdccc
004be7c4: ldr      r3, [pc, #0xe44]
004be7c8: ldr      r1, [pc, #0xe44]
004be7cc: mov      r0, r5
004be7d0: ldr      r2, [r4, r3]
004be7d4: add      r1, pc, r1
004be7d8: bl       #0x4bdccc
004be7dc: ldr      r3, [pc, #0xe34]
004be7e0: ldr      r1, [pc, #0xe34]
004be7e4: mov      r0, r5
004be7e8: ldr      r2, [r4, r3]
004be7ec: ldr      r3, [pc, #0xe2c]
004be7f0: add      r1, pc, r1
004be7f4: add      r6, pc, r6
004be7f8: ldr      r3, [r4, r3]
004be7fc: bl       #0x4be3e0
004be800: ldr      r3, [pc, #0xe1c]
004be804: ldr      r1, [pc, #0xe1c]
004be808: mov      r0, r5
004be80c: ldr      r2, [r4, r3]
004be810: ldr      r3, [pc, #0xe14]
004be814: add      r1, pc, r1
004be818: ldr      r3, [r4, r3]
004be81c: bl       #0x4be3e0
004be820: ldr      r3, [pc, #0xe08]
004be824: ldr      r1, [pc, #0xe08]
004be828: mov      r0, r5
004be82c: ldr      r2, [r4, r3]
004be830: add      r1, pc, r1
004be834: bl       #0x4bdccc
004be838: ldr      r3, [pc, #0xdf8]
004be83c: ldr      r1, [pc, #0xdf8]
004be840: mov      r0, r5
004be844: ldr      r2, [r4, r3]
004be848: add      r1, pc, r1
004be84c: bl       #0x4bdccc
004be850: ldr      r3, [pc, #0xde8]
004be854: mov      r0, r5
004be858: mov      r1, r7
004be85c: ldr      r2, [r4, r3]
004be860: ldr      r3, [pc, #0xddc]
004be864: ldr      r3, [r4, r3]
004be868: bl       #0x4be3e0
004be86c: ldr      r3, [pc, #0xdd4]
004be870: mov      r0, r5
004be874: mov      r1, r6
004be878: ldr      r2, [r4, r3]
004be87c: ldr      r3, [pc, #0xdc8]
004be880: ldr      r3, [r4, r3]
004be884: bl       #0x4be3e0
004be888: ldr      r3, [pc, #0xdc0]
004be88c: ldr      r1, [pc, #0xdc0]
004be890: mov      r0, r5
004be894: ldr      r2, [r4, r3]
004be898: add      r1, pc, r1
004be89c: bl       #0x4bdccc
004be8a0: ldr      r3, [pc, #0xdb0]
004be8a4: ldr      r1, [pc, #0xdb0]
004be8a8: mov      r0, r5
004be8ac: ldr      r2, [r4, r3]
004be8b0: add      r1, pc, r1
004be8b4: bl       #0x4bdccc
004be8b8: ldr      r3, [pc, #0xda0]
004be8bc: mov      r0, r5
004be8c0: mov      r1, r7
004be8c4: ldr      r2, [r4, r3]
004be8c8: ldr      r3, [pc, #0xd94]
004be8cc: ldr      r3, [r4, r3]
004be8d0: bl       #0x4be3e0
004be8d4: ldr      r3, [pc, #0xd8c]
004be8d8: mov      r0, r5
004be8dc: mov      r1, r6
004be8e0: ldr      r2, [r4, r3]
004be8e4: ldr      r3, [pc, #0xd80]
004be8e8: ldr      r3, [r4, r3]
004be8ec: bl       #0x4be3e0
004be8f0: ldr      r3, [pc, #0xd78]
004be8f4: ldr      r1, [pc, #0xd78]
004be8f8: mov      r0, r5
004be8fc: ldr      r2, [r4, r3]
004be900: add      r1, pc, r1
004be904: bl       #0x4bdccc
004be908: ldr      r3, [pc, #0xd68]
004be90c: ldr      r1, [pc, #0xd68]
004be910: mov      r0, r5
004be914: ldr      r2, [r4, r3]
004be918: add      r1, pc, r1
004be91c: bl       #0x4bdccc
004be920: ldr      r3, [pc, #0xd58]
004be924: mov      r1, r7
004be928: mov      r0, r5
004be92c: ldr      r2, [r4, r3]
004be930: ldr      r3, [pc, #0xd4c]
004be934: ldr      r7, [pc, #0xd4c]
004be938: ldr      r3, [r4, r3]
004be93c: bl       #0x4be3e0
004be940: ldr      r3, [pc, #0xd44]
004be944: mov      r1, r6
004be948: mov      r0, r5
004be94c: ldr      r2, [r4, r3]
004be950: ldr      r3, [pc, #0xd38]
004be954: add      r7, pc, r7
004be958: ldr      r6, [pc, #0xd34]
004be95c: ldr      r3, [r4, r3]
004be960: bl       #0x4be3e0
004be964: ldr      r3, [pc, #0xd2c]
004be968: ldr      r1, [pc, #0xd2c]
004be96c: mov      r0, r5
004be970: ldr      r2, [r4, r3]
004be974: add      r1, pc, r1
004be978: bl       #0x4bdccc
004be97c: ldr      r3, [pc, #0xd1c]
004be980: ldr      r1, [pc, #0xd1c]
004be984: mov      r0, r5
004be988: ldr      r2, [r4, r3]
004be98c: add      r1, pc, r1
004be990: bl       #0x4bdccc
004be994: ldr      r3, [pc, #0xd0c]
004be998: ldr      r1, [pc, #0xd0c]
004be99c: mov      r0, r5
004be9a0: ldr      r2, [r4, r3]
004be9a4: ldr      r3, [pc, #0xd04]
004be9a8: add      r1, pc, r1
004be9ac: add      r6, pc, r6
004be9b0: ldr      r3, [r4, r3]
004be9b4: bl       #0x4be3e0
004be9b8: ldr      r3, [pc, #0xcf4]
004be9bc: ldr      r1, [pc, #0xcf4]
004be9c0: mov      r0, r5
004be9c4: ldr      r2, [r4, r3]
004be9c8: ldr      r3, [pc, #0xcec]
004be9cc: add      r1, pc, r1
004be9d0: ldr      r3, [r4, r3]
004be9d4: bl       #0x4be3e0
004be9d8: ldr      r3, [pc, #0xce0]
004be9dc: ldr      r1, [pc, #0xce0]
004be9e0: mov      r0, r5
004be9e4: ldr      r2, [r4, r3]
004be9e8: add      r1, pc, r1
004be9ec: bl       #0x4bdccc
004be9f0: ldr      r3, [pc, #0xcd0]
004be9f4: ldr      r1, [pc, #0xcd0]
004be9f8: mov      r0, r5
004be9fc: ldr      r2, [r4, r3]
004bea00: add      r1, pc, r1
004bea04: bl       #0x4bdccc
004bea08: ldr      r3, [pc, #0xcc0]
004bea0c: ldr      r1, [pc, #0xcc0]
004bea10: mov      r0, r5
004bea14: ldr      r2, [r4, r3]
004bea18: ldr      r3, [pc, #0xcb8]
004bea1c: add      r1, pc, r1
004bea20: ldr      r3, [r4, r3]
004bea24: bl       #0x4be3e0
004bea28: ldr      r3, [pc, #0xcac]
004bea2c: ldr      r1, [pc, #0xcac]
004bea30: mov      r0, r5
004bea34: ldr      r2, [r4, r3]
004bea38: ldr      r3, [pc, #0xca4]
004bea3c: add      r1, pc, r1
004bea40: ldr      r3, [r4, r3]
004bea44: bl       #0x4be3e0
004bea48: ldr      r3, [pc, #0xc98]
004bea4c: ldr      r1, [pc, #0xc98]
004bea50: mov      r0, r5
004bea54: ldr      r2, [r4, r3]
004bea58: add      r1, pc, r1
004bea5c: bl       #0x4bdccc
004bea60: ldr      r3, [pc, #0xc88]
004bea64: ldr      r1, [pc, #0xc88]
004bea68: mov      r0, r5
004bea6c: ldr      r2, [r4, r3]
004bea70: add      r1, pc, r1
004bea74: bl       #0x4bdccc
004bea78: ldr      r3, [pc, #0xc78]
004bea7c: mov      r0, r5
004bea80: mov      r1, r7
004bea84: ldr      r2, [r4, r3]
004bea88: ldr      r3, [pc, #0xc6c]
004bea8c: ldr      r3, [r4, r3]
004bea90: bl       #0x4be3e0
004bea94: ldr      r3, [pc, #0xc64]
004bea98: mov      r0, r5
004bea9c: mov      r1, r6
004beaa0: ldr      r2, [r4, r3]
004beaa4: ldr      r3, [pc, #0xc58]
004beaa8: ldr      r3, [r4, r3]
004beaac: bl       #0x4be3e0
004beab0: ldr      r3, [pc, #0xc50]
004beab4: ldr      r1, [pc, #0xc50]
004beab8: mov      r0, r5
004beabc: ldr      r2, [r4, r3]
004beac0: add      r1, pc, r1
004beac4: bl       #0x4bdccc
004beac8: ldr      r3, [pc, #0xc40]
004beacc: ldr      r1, [pc, #0xc40]
004bead0: mov      r0, r5
004bead4: ldr      r2, [r4, r3]
004bead8: add      r1, pc, r1
004beadc: bl       #0x4bdccc
004beae0: ldr      r3, [pc, #0xc30]
004beae4: mov      r0, r5
004beae8: mov      r1, r7
004beaec: ldr      r2, [r4, r3]
004beaf0: ldr      r3, [pc, #0xc24]
004beaf4: ldr      r3, [r4, r3]
004beaf8: bl       #0x4be3e0
004beafc: ldr      r3, [pc, #0xc1c]
004beb00: mov      r0, r5
004beb04: mov      r1, r6
004beb08: ldr      r2, [r4, r3]
004beb0c: ldr      r3, [pc, #0xc10]
004beb10: ldr      r3, [r4, r3]
004beb14: bl       #0x4be3e0
004beb18: ldr      r3, [pc, #0xc08]
004beb1c: ldr      r1, [pc, #0xc08]
004beb20: mov      r0, r5
004beb24: ldr      r2, [r4, r3]
004beb28: add      r1, pc, r1
004beb2c: bl       #0x4bdccc
004beb30: ldr      r3, [pc, #0xbf8]
004beb34: ldr      r1, [pc, #0xbf8]
004beb38: mov      r0, r5
004beb3c: ldr      r2, [r4, r3]
004beb40: add      r1, pc, r1
004beb44: bl       #0x4bdccc
004beb48: ldr      r3, [pc, #0xbe8]
004beb4c: mov      r1, r7
004beb50: mov      r0, r5
004beb54: ldr      r2, [r4, r3]
004beb58: ldr      r3, [pc, #0xbdc]
004beb5c: ldr      r7, [pc, #0xbdc]
004beb60: ldr      r3, [r4, r3]
004beb64: bl       #0x4be3e0
004beb68: ldr      r3, [pc, #0xbd4]
004beb6c: mov      r1, r6
004beb70: mov      r0, r5
004beb74: ldr      r2, [r4, r3]
004beb78: ldr      r3, [pc, #0xbc8]
004beb7c: add      r7, pc, r7
004beb80: ldr      r6, [pc, #0xbc4]
004beb84: ldr      r3, [r4, r3]
004beb88: bl       #0x4be3e0
004beb8c: ldr      r3, [pc, #0xbbc]
004beb90: ldr      r1, [pc, #0xbbc]
004beb94: mov      r0, r5
004beb98: ldr      r2, [r4, r3]
004beb9c: add      r1, pc, r1
004beba0: bl       #0x4bdccc
004beba4: ldr      r3, [pc, #0xbac]
004beba8: ldr      r1, [pc, #0xbac]
004bebac: mov      r0, r5
004bebb0: ldr      r2, [r4, r3]
004bebb4: add      r1, pc, r1
004bebb8: bl       #0x4bdccc
004bebbc: ldr      r3, [pc, #0xb9c]
004bebc0: mov      r0, r5
004bebc4: mov      r1, r7
004bebc8: ldr      r2, [r4, r3]
004bebcc: ldr      r3, [pc, #0xb90]
004bebd0: add      r6, pc, r6
004bebd4: ldr      r3, [r4, r3]
004bebd8: bl       #0x4be3e0
004bebdc: ldr      r3, [pc, #0xb84]
004bebe0: mov      r0, r5
004bebe4: mov      r1, r6
004bebe8: ldr      r2, [r4, r3]
004bebec: ldr      r3, [pc, #0xb78]
004bebf0: ldr      r3, [r4, r3]
004bebf4: bl       #0x4be3e0
004bebf8: ldr      r3, [pc, #0xb70]
004bebfc: ldr      r1, [pc, #0xb70]
004bec00: mov      r0, r5
004bec04: ldr      r2, [r4, r3]
004bec08: add      r1, pc, r1
004bec0c: bl       #0x4bdccc
004bec10: ldr      r3, [pc, #0xb60]
004bec14: ldr      r1, [pc, #0xb60]
004bec18: mov      r0, r5
004bec1c: ldr      r2, [r4, r3]
004bec20: add      r1, pc, r1
004bec24: bl       #0x4bdccc
004bec28: ldr      r3, [pc, #0xb50]
004bec2c: mov      r1, r7
004bec30: mov      r0, r5
004bec34: ldr      r2, [r4, r3]
004bec38: ldr      r3, [pc, #0xb44]
004bec3c: ldr      r7, [pc, #0xb44]
004bec40: ldr      r3, [r4, r3]
004bec44: bl       #0x4be3e0
004bec48: ldr      r3, [pc, #0xb3c]
004bec4c: mov      r1, r6
004bec50: mov      r0, r5
004bec54: ldr      r2, [r4, r3]
004bec58: ldr      r3, [pc, #0xb30]
004bec5c: add      r7, pc, r7
004bec60: ldr      r6, [pc, #0xb2c]
004bec64: ldr      r3, [r4, r3]
004bec68: bl       #0x4be3e0
004bec6c: ldr      r3, [pc, #0xb24]
004bec70: ldr      r1, [pc, #0xb24]
004bec74: mov      r0, r5
004bec78: ldr      r2, [r4, r3]
004bec7c: add      r1, pc, r1
004bec80: bl       #0x4bdccc
004bec84: ldr      r3, [pc, #0xb14]
004bec88: ldr      r1, [pc, #0xb14]
004bec8c: mov      r0, r5
004bec90: ldr      r2, [r4, r3]
004bec94: add      r1, pc, r1
004bec98: bl       #0x4bdccc
004bec9c: ldr      r3, [pc, #0xb04]
004beca0: mov      r0, r5
004beca4: mov      r1, r7
004beca8: ldr      r2, [r4, r3]
004becac: ldr      r3, [pc, #0xaf8]
004becb0: add      r6, pc, r6
004becb4: ldr      r3, [r4, r3]
004becb8: bl       #0x4be3e0
004becbc: ldr      r3, [pc, #0xaec]
004becc0: mov      r0, r5
004becc4: mov      r1, r6
004becc8: ldr      r2, [r4, r3]
004beccc: ldr      r3, [pc, #0xae0]
004becd0: ldr      r3, [r4, r3]
004becd4: bl       #0x4be3e0
004becd8: ldr      r3, [pc, #0xad8]
004becdc: ldr      r1, [pc, #0xad8]
004bece0: mov      r0, r5
004bece4: ldr      r2, [r4, r3]
004bece8: add      r1, pc, r1
004becec: bl       #0x4bdccc
004becf0: ldr      r3, [pc, #0xac8]
004becf4: ldr      r1, [pc, #0xac8]
004becf8: mov      r0, r5
004becfc: ldr      r2, [r4, r3]
004bed00: add      r1, pc, r1
004bed04: bl       #0x4bdccc
004bed08: ldr      r3, [pc, #0xab8]
004bed0c: mov      r0, r5
004bed10: mov      r1, r7
004bed14: ldr      r2, [r4, r3]
004bed18: ldr      r3, [pc, #0xaac]
004bed1c: ldr      r3, [r4, r3]
004bed20: bl       #0x4be3e0
004bed24: ldr      r3, [pc, #0xaa4]
004bed28: mov      r0, r5
004bed2c: mov      r1, r6
004bed30: ldr      r2, [r4, r3]
004bed34: ldr      r3, [pc, #0xa98]
004bed38: ldr      r3, [r4, r3]
004bed3c: bl       #0x4be3e0
004bed40: ldr      r3, [pc, #0xa90]
004bed44: ldr      r1, [pc, #0xa90]
004bed48: mov      r0, r5
004bed4c: ldr      r2, [r4, r3]
004bed50: add      r1, pc, r1
004bed54: bl       #0x4bdccc
004bed58: ldr      r3, [pc, #0xa80]
004bed5c: ldr      r1, [pc, #0xa80]
004bed60: mov      r0, r5
004bed64: ldr      r2, [r4, r3]
004bed68: add      r1, pc, r1
004bed6c: bl       #0x4bdccc
004bed70: ldr      r3, [pc, #0xa70]
004bed74: mov      r1, r7
004bed78: mov      r0, r5
004bed7c: ldr      r2, [r4, r3]
004bed80: ldr      r3, [pc, #0xa64]
004bed84: ldr      r7, [pc, #0xa64]
004bed88: ldr      r3, [r4, r3]
004bed8c: bl       #0x4be3e0
004bed90: ldr      r3, [pc, #0xa5c]
004bed94: mov      r1, r6
004bed98: mov      r0, r5
004bed9c: ldr      r2, [r4, r3]
004beda0: ldr      r3, [pc, #0xa50]
004beda4: add      r7, pc, r7
004beda8: ldr      r6, [pc, #0xa4c]
004bedac: ldr      r3, [r4, r3]
004bedb0: bl       #0x4be3e0
004bedb4: ldr      r3, [pc, #0xa44]
004bedb8: ldr      r1, [pc, #0xa44]
004bedbc: mov      r0, r5
004bedc0: ldr      r2, [r4, r3]
004bedc4: add      r1, pc, r1
004bedc8: bl       #0x4bdccc
004bedcc: ldr      r3, [pc, #0xa34]
004bedd0: ldr      r1, [pc, #0xa34]
004bedd4: mov      r0, r5
004bedd8: ldr      r2, [r4, r3]
004beddc: add      r1, pc, r1
004bede0: bl       #0x4bdccc
004bede4: ldr      r3, [pc, #0xa24]
004bede8: mov      r0, r5
004bedec: mov      r1, r7
004bedf0: ldr      r2, [r4, r3]
004bedf4: ldr      r3, [pc, #0xa18]
004bedf8: add      r6, pc, r6
004bedfc: ldr      r3, [r4, r3]
004bee00: bl       #0x4be3e0
004bee04: ldr      r3, [pc, #0xa0c]
004bee08: mov      r0, r5
004bee0c: mov      r1, r6
004bee10: ldr      r2, [r4, r3]
004bee14: ldr      r3, [pc, #0xa00]
004bee18: ldr      r3, [r4, r3]
004bee1c: bl       #0x4be3e0
004bee20: ldr      r3, [pc, #0x9f8]
004bee24: ldr      r1, [pc, #0x9f8]
004bee28: mov      r0, r5
004bee2c: ldr      r2, [r4, r3]
004bee30: add      r1, pc, r1
004bee34: bl       #0x4bdccc
004bee38: ldr      r3, [pc, #0x9e8]
004bee3c: ldr      r1, [pc, #0x9e8]
004bee40: mov      r0, r5
004bee44: ldr      r2, [r4, r3]
004bee48: add      r1, pc, r1
004bee4c: bl       #0x4bdccc
004bee50: ldr      r3, [pc, #0x9d8]
004bee54: mov      r1, r7
004bee58: mov      r0, r5
004bee5c: ldr      r2, [r4, r3]
004bee60: ldr      r3, [pc, #0x9cc]
004bee64: ldr      r7, [pc, #0x9cc]
004bee68: ldr      r3, [r4, r3]
004bee6c: bl       #0x4be3e0
004bee70: ldr      r3, [pc, #0x9c4]
004bee74: mov      r1, r6
004bee78: mov      r0, r5
004bee7c: ldr      r2, [r4, r3]
004bee80: ldr      r3, [pc, #0x9b8]
004bee84: add      r7, pc, r7
004bee88: ldr      r6, [pc, #0x9b4]
004bee8c: ldr      r3, [r4, r3]
004bee90: bl       #0x4be3e0
004bee94: ldr      r3, [pc, #0x9ac]
004bee98: ldr      r1, [pc, #0x9ac]
004bee9c: mov      r0, r5
004beea0: ldr      r2, [r4, r3]
004beea4: add      r1, pc, r1
004beea8: bl       #0x4bdccc
004beeac: ldr      r3, [pc, #0x99c]
004beeb0: ldr      r1, [pc, #0x99c]
004beeb4: mov      r0, r5
004beeb8: ldr      r2, [r4, r3]
004beebc: add      r1, pc, r1
004beec0: bl       #0x4bdccc
004beec4: ldr      r3, [pc, #0x98c]
004beec8: ldr      r1, [pc, #0x98c]
004beecc: mov      r0, r5
004beed0: ldr      r2, [r4, r3]
004beed4: ldr      r3, [pc, #0x984]
004beed8: add      r1, pc, r1
004beedc: add      r6, pc, r6
004beee0: ldr      r3, [r4, r3]
004beee4: bl       #0x4be3e0
004beee8: ldr      r3, [pc, #0x974]
004beeec: ldr      r1, [pc, #0x974]
004beef0: mov      r0, r5
004beef4: ldr      r2, [r4, r3]
004beef8: ldr      r3, [pc, #0x96c]
004beefc: add      r1, pc, r1
004bef00: ldr      r3, [r4, r3]
004bef04: bl       #0x4be3e0
004bef08: ldr      r3, [pc, #0x960]
004bef0c: ldr      r1, [pc, #0x960]
004bef10: mov      r0, r5
004bef14: ldr      r2, [r4, r3]
004bef18: add      r1, pc, r1
004bef1c: bl       #0x4bdccc
004bef20: ldr      r3, [pc, #0x950]
004bef24: ldr      r1, [pc, #0x950]
004bef28: mov      r0, r5
004bef2c: ldr      r2, [r4, r3]
004bef30: add      r1, pc, r1
004bef34: bl       #0x4bdccc
004bef38: ldr      r3, [pc, #0x940]
004bef3c: mov      r0, r5
004bef40: mov      r1, r7
004bef44: ldr      r2, [r4, r3]
004bef48: ldr      r3, [pc, #0x934]
004bef4c: ldr      r3, [r4, r3]
004bef50: bl       #0x4be3e0
004bef54: ldr      r3, [pc, #0x92c]
004bef58: mov      r0, r5
004bef5c: mov      r1, r6
004bef60: ldr      r2, [r4, r3]
004bef64: ldr      r3, [pc, #0x920]
004bef68: ldr      r3, [r4, r3]
004bef6c: bl       #0x4be3e0
004bef70: ldr      r3, [pc, #0x918]
004bef74: ldr      r1, [pc, #0x918]
004bef78: mov      r0, r5
004bef7c: ldr      r2, [r4, r3]
004bef80: add      r1, pc, r1
004bef84: bl       #0x4bdccc
004bef88: ldr      r3, [pc, #0x908]
004bef8c: ldr      r1, [pc, #0x908]
004bef90: mov      r0, r5
004bef94: ldr      r2, [r4, r3]
004bef98: add      r1, pc, r1
004bef9c: bl       #0x4bdccc
004befa0: ldr      r3, [pc, #0x8f8]
004befa4: mov      r0, r5
004befa8: mov      r1, r7
004befac: ldr      r2, [r4, r3]
004befb0: ldr      r3, [pc, #0x8ec]
004befb4: ldr      r3, [r4, r3]
004befb8: bl       #0x4be3e0
004befbc: ldr      r3, [pc, #0x8e4]
004befc0: mov      r0, r5
004befc4: mov      r1, r6
004befc8: ldr      r2, [r4, r3]
004befcc: ldr      r3, [pc, #0x8d8]
004befd0: ldr      r3, [r4, r3]
004befd4: bl       #0x4be3e0
004befd8: ldr      r3, [pc, #0x8d0]
004befdc: ldr      r1, [pc, #0x8d0]
004befe0: mov      r0, r5
004befe4: ldr      r2, [r4, r3]
004befe8: add      r1, pc, r1
004befec: bl       #0x4bdccc
004beff0: ldr      r3, [pc, #0x8c0]
004beff4: ldr      r1, [pc, #0x8c0]
004beff8: mov      r0, r5
004beffc: ldr      r2, [r4, r3]
004bf000: add      r1, pc, r1
004bf004: bl       #0x4bdccc
004bf008: ldr      r3, [pc, #0x8b0]
004bf00c: mov      r0, r5
004bf010: mov      r1, r7
004bf014: ldr      r2, [r4, r3]
004bf018: ldr      r3, [pc, #0x8a4]
004bf01c: ldr      r3, [r4, r3]
004bf020: bl       #0x4be3e0
004bf024: ldr      r3, [pc, #0x89c]
004bf028: mov      r0, r5
004bf02c: mov      r1, r6
004bf030: ldr      r2, [r4, r3]
004bf034: ldr      r3, [pc, #0x890]
004bf038: ldr      r3, [r4, r3]
004bf03c: bl       #0x4be3e0
004bf040: ldr      r3, [pc, #0x888]
004bf044: ldr      r1, [pc, #0x888]
004bf048: mov      r0, r5
004bf04c: ldr      r2, [r4, r3]
004bf050: add      r1, pc, r1
004bf054: bl       #0x4bdccc
004bf058: ldr      r3, [pc, #0x878]
004bf05c: ldr      r1, [pc, #0x878]
004bf060: mov      r0, r5
004bf064: ldr      r2, [r4, r3]
004bf068: add      r1, pc, r1
004bf06c: bl       #0x4bdccc
004bf070: ldr      r3, [pc, #0x868]
004bf074: mov      r0, r5
004bf078: mov      r1, r7
004bf07c: ldr      r2, [r4, r3]
004bf080: ldr      r3, [pc, #0x85c]
004bf084: ldr      r3, [r4, r3]
004bf088: bl       #0x4be3e0
004bf08c: ldr      r3, [pc, #0x854]
004bf090: mov      r0, r5
004bf094: mov      r1, r6
004bf098: ldr      r2, [r4, r3]
004bf09c: ldr      r3, [pc, #0x848]
004bf0a0: ldr      r3, [r4, r3]
004bf0a4: bl       #0x4be3e0
004bf0a8: ldr      r3, [pc, #0x840]
004bf0ac: ldr      r1, [pc, #0x840]
004bf0b0: mov      r0, r5
004bf0b4: ldr      r2, [r4, r3]
004bf0b8: add      r1, pc, r1
004bf0bc: bl       #0x4bdccc
004bf0c0: ldr      r3, [pc, #0x830]
004bf0c4: ldr      r1, [pc, #0x830]
004bf0c8: mov      r0, r5
004bf0cc: ldr      r2, [r4, r3]
004bf0d0: add      r1, pc, r1
004bf0d4: bl       #0x4bdccc
004bf0d8: ldr      r3, [pc, #0x820]
004bf0dc: mov      r0, r5
004bf0e0: mov      r1, r7
004bf0e4: ldr      r2, [r4, r3]
004bf0e8: ldr      r3, [pc, #0x814]
004bf0ec: ldr      r3, [r4, r3]
004bf0f0: bl       #0x4be3e0
004bf0f4: ldr      r3, [pc, #0x80c]
004bf0f8: mov      r0, r5
004bf0fc: mov      r1, r6
004bf100: ldr      r2, [r4, r3]
004bf104: ldr      r3, [pc, #0x800]
004bf108: ldr      r3, [r4, r3]
004bf10c: bl       #0x4be3e0
004bf110: ldr      r3, [pc, #0x7f8]
004bf114: ldr      r1, [pc, #0x7f8]
004bf118: mov      r0, r5
004bf11c: ldr      r2, [r4, r3]
004bf120: add      r1, pc, r1
004bf124: bl       #0x4bdccc
004bf128: ldr      r3, [pc, #0x7e8]
004bf12c: ldr      r1, [pc, #0x7e8]
004bf130: mov      r0, r5
004bf134: ldr      r2, [r4, r3]
004bf138: add      r1, pc, r1
004bf13c: bl       #0x4bdccc
004bf140: ldr      r3, [pc, #0x7d8]
004bf144: mov      r0, r5
004bf148: mov      r1, r7
004bf14c: ldr      r2, [r4, r3]
004bf150: ldr      r3, [pc, #0x7cc]
004bf154: ldr      r3, [r4, r3]
004bf158: bl       #0x4be3e0
004bf15c: ldr      r3, [pc, #0x7c4]
004bf160: mov      r0, r5
004bf164: mov      r1, r6
004bf168: ldr      r2, [r4, r3]
004bf16c: ldr      r3, [pc, #0x7b8]
004bf170: ldr      r3, [r4, r3]
004bf174: bl       #0x4be3e0
004bf178: ldr      r3, [pc, #0x7b0]
004bf17c: ldr      r1, [pc, #0x7b0]
004bf180: mov      r0, r5
004bf184: ldr      r2, [r4, r3]
004bf188: add      r1, pc, r1
004bf18c: bl       #0x4bdccc
004bf190: ldr      r3, [pc, #0x7a0]
004bf194: ldr      r1, [pc, #0x7a0]
004bf198: mov      r0, r5
004bf19c: ldr      r2, [r4, r3]
004bf1a0: add      r1, pc, r1
004bf1a4: bl       #0x4bdccc
004bf1a8: ldr      r3, [pc, #0x790]
004bf1ac: mov      r0, r5
004bf1b0: mov      r1, r7
004bf1b4: ldr      r2, [r4, r3]
004bf1b8: ldr      r3, [pc, #0x784]
004bf1bc: ldr      r3, [r4, r3]
004bf1c0: bl       #0x4be3e0
004bf1c4: ldr      r3, [pc, #0x77c]
004bf1c8: mov      r0, r5
004bf1cc: mov      r1, r6
004bf1d0: ldr      r2, [r4, r3]
004bf1d4: ldr      r3, [pc, #0x770]
004bf1d8: ldr      r3, [r4, r3]
004bf1dc: bl       #0x4be3e0
004bf1e0: ldr      r3, [pc, #0x768]
004bf1e4: ldr      r1, [pc, #0x768]
004bf1e8: mov      r0, r5
004bf1ec: ldr      r2, [r4, r3]
004bf1f0: add      r1, pc, r1
004bf1f4: bl       #0x4bdccc
004bf1f8: ldr      r3, [pc, #0x758]
004bf1fc: ldr      r1, [pc, #0x758]
004bf200: mov      r0, r5
004bf204: ldr      r2, [r4, r3]
004bf208: add      r1, pc, r1
004bf20c: bl       #0x4bdccc
004bf210: ldr      r3, [pc, #0x748]
004bf214: mov      r0, r5
004bf218: mov      r1, r7
004bf21c: ldr      r2, [r4, r3]
004bf220: ldr      r3, [pc, #0x73c]
004bf224: ldr      r3, [r4, r3]
004bf228: bl       #0x4be3e0
004bf22c: ldr      r3, [pc, #0x734]
004bf230: mov      r0, r5
004bf234: mov      r1, r6
004bf238: ldr      r2, [r4, r3]
004bf23c: ldr      r3, [pc, #0x728]
004bf240: ldr      r3, [r4, r3]
004bf244: bl       #0x4be3e0
004bf248: ldr      r3, [pc, #0x720]
004bf24c: ldr      r1, [pc, #0x720]
004bf250: mov      r0, r5
004bf254: ldr      r2, [r4, r3]
004bf258: add      r1, pc, r1
004bf25c: bl       #0x4bdccc
004bf260: ldr      r3, [pc, #0x710]
004bf264: ldr      r1, [pc, #0x710]
004bf268: mov      r0, r5
004bf26c: ldr      r2, [r4, r3]
004bf270: add      r1, pc, r1
004bf274: bl       #0x4bdccc
004bf278: ldr      r3, [pc, #0x700]
004bf27c: mov      r0, r5
004bf280: mov      r1, r7
004bf284: ldr      r2, [r4, r3]
004bf288: ldr      r3, [pc, #0x6f4]
004bf28c: ldr      r3, [r4, r3]
004bf290: bl       #0x4be3e0
004bf294: ldr      r3, [pc, #0x6ec]
004bf298: mov      r0, r5
004bf29c: mov      r1, r6
004bf2a0: ldr      r2, [r4, r3]
004bf2a4: ldr      r3, [pc, #0x6e0]
004bf2a8: ldr      r3, [r4, r3]
004bf2ac: bl       #0x4be3e0
004bf2b0: ldr      r3, [pc, #0x6d8]
004bf2b4: ldr      r1, [pc, #0x6d8]
004bf2b8: mov      r0, r5
004bf2bc: ldr      r2, [r4, r3]
004bf2c0: add      r1, pc, r1
004bf2c4: bl       #0x4bdccc
004bf2c8: ldr      r3, [pc, #0x6c8]
004bf2cc: ldr      r1, [pc, #0x6c8]
004bf2d0: mov      r0, r5
004bf2d4: ldr      r2, [r4, r3]
004bf2d8: add      r1, pc, r1
004bf2dc: bl       #0x4bdccc
004bf2e0: ldr      r3, [pc, #0x6b8]
004bf2e4: mov      r0, r5
004bf2e8: mov      r1, r7
004bf2ec: ldr      r2, [r4, r3]
004bf2f0: ldr      r3, [pc, #0x6ac]
004bf2f4: ldr      r3, [r4, r3]
004bf2f8: bl       #0x4be3e0
004bf2fc: ldr      r3, [pc, #0x6a4]
004bf300: mov      r0, r5
004bf304: mov      r1, r6
004bf308: ldr      r2, [r4, r3]
004bf30c: ldr      r3, [pc, #0x698]
004bf310: ldr      r3, [r4, r3]
004bf314: bl       #0x4be3e0
004bf318: ldr      r3, [pc, #0x690]
004bf31c: ldr      r1, [pc, #0x690]
004bf320: mov      r0, r5
004bf324: ldr      r2, [r4, r3]
004bf328: add      r1, pc, r1
004bf32c: bl       #0x4bdccc
004bf330: ldr      r3, [pc, #0x680]
004bf334: ldr      r1, [pc, #0x680]
004bf338: mov      r0, r5
004bf33c: ldr      r2, [r4, r3]
004bf340: add      r1, pc, r1
004bf344: bl       #0x4bdccc
004bf348: ldr      r3, [pc, #0x670]
004bf34c: mov      r1, r7
004bf350: mov      r0, r5
004bf354: ldr      r2, [r4, r3]
004bf358: ldr      r3, [pc, #0x664]
004bf35c: ldr      r7, [pc, #0x664]
004bf360: ldr      r3, [r4, r3]
004bf364: bl       #0x4be3e0
004bf368: ldr      r3, [pc, #0x65c]
004bf36c: mov      r1, r6
004bf370: mov      r0, r5
004bf374: ldr      r2, [r4, r3]
004bf378: ldr      r3, [pc, #0x650]
004bf37c: add      r7, pc, r7
004bf380: ldr      r6, [pc, #0x64c]
004bf384: ldr      r3, [r4, r3]
004bf388: bl       #0x4be3e0
004bf38c: ldr      r3, [pc, #0x644]
004bf390: ldr      r1, [pc, #0x644]
004bf394: mov      r0, r5
004bf398: ldr      r2, [r4, r3]
004bf39c: add      r1, pc, r1
004bf3a0: bl       #0x4bdccc
004bf3a4: ldr      r3, [pc, #0x634]
004bf3a8: ldr      r1, [pc, #0x634]
004bf3ac: mov      r0, r5
004bf3b0: ldr      r2, [r4, r3]
004bf3b4: add      r1, pc, r1
004bf3b8: bl       #0x4bdccc
004bf3bc: ldr      r3, [pc, #0x624]
004bf3c0: mov      r0, r5
004bf3c4: mov      r1, r7
004bf3c8: ldr      r2, [r4, r3]
004bf3cc: ldr      r3, [pc, #0x618]
004bf3d0: add      r6, pc, r6
004bf3d4: ldr      r3, [r4, r3]
004bf3d8: bl       #0x4be3e0
004bf3dc: ldr      r3, [pc, #0x60c]
004bf3e0: mov      r0, r5
004bf3e4: mov      r1, r6
004bf3e8: ldr      r2, [r4, r3]
004bf3ec: ldr      r3, [pc, #0x600]
004bf3f0: ldr      r3, [r4, r3]
004bf3f4: bl       #0x4be3e0
004bf3f8: ldr      r3, [pc, #0x5f8]
004bf3fc: ldr      r1, [pc, #0x5f8]
004bf400: mov      r0, r5
004bf404: ldr      r2, [r4, r3]
004bf408: add      r1, pc, r1
004bf40c: bl       #0x4bdccc
004bf410: ldr      r3, [pc, #0x5e8]
004bf414: ldr      r1, [pc, #0x5e8]
004bf418: mov      r0, r5
004bf41c: ldr      r2, [r4, r3]
004bf420: add      r1, pc, r1
004bf424: bl       #0x4bdccc
004bf428: ldr      r3, [pc, #0x5d8]
004bf42c: mov      r1, r7
004bf430: mov      r0, r5
004bf434: ldr      r2, [r4, r3]
004bf438: ldr      r3, [pc, #0x5cc]
004bf43c: ldr      r7, [pc, #0x5cc]
004bf440: ldr      r3, [r4, r3]
004bf444: bl       #0x4be3e0
004bf448: ldr      r3, [pc, #0x5c4]
004bf44c: mov      r1, r6
004bf450: mov      r0, r5
004bf454: ldr      r2, [r4, r3]
004bf458: ldr      r3, [pc, #0x5b8]
004bf45c: add      r7, pc, r7
004bf460: ldr      r6, [pc, #0x5b4]
004bf464: ldr      r3, [r4, r3]
004bf468: bl       #0x4be3e0
004bf46c: ldr      r3, [pc, #0x5ac]
004bf470: ldr      r1, [pc, #0x5ac]
004bf474: mov      r0, r5
004bf478: ldr      r2, [r4, r3]
004bf47c: add      r1, pc, r1
004bf480: bl       #0x4bdccc
004bf484: ldr      r3, [pc, #0x59c]
004bf488: ldr      r1, [pc, #0x59c]
004bf48c: mov      r0, r5
004bf490: ldr      r2, [r4, r3]
004bf494: add      r1, pc, r1
004bf498: bl       #0x4bdccc
004bf49c: ldr      r3, [pc, #0x58c]
004bf4a0: mov      r0, r5
004bf4a4: mov      r1, r7
004bf4a8: ldr      r2, [r4, r3]
004bf4ac: ldr      r3, [pc, #0x580]
004bf4b0: add      r6, pc, r6
004bf4b4: ldr      r3, [r4, r3]
004bf4b8: bl       #0x4be3e0
004bf4bc: ldr      r3, [pc, #0x574]
004bf4c0: mov      r0, r5
004bf4c4: mov      r1, r6
004bf4c8: ldr      r2, [r4, r3]
004bf4cc: ldr      r3, [pc, #0x568]
004bf4d0: ldr      r3, [r4, r3]
004bf4d4: bl       #0x4be3e0
004bf4d8: ldr      r3, [pc, #0x560]
004bf4dc: ldr      r1, [pc, #0x560]
004bf4e0: mov      r0, r5
004bf4e4: ldr      r2, [r4, r3]
004bf4e8: add      r1, pc, r1
004bf4ec: bl       #0x4bdccc
004bf4f0: ldr      r3, [pc, #0x550]
004bf4f4: ldr      r1, [pc, #0x550]
004bf4f8: mov      r0, r5
004bf4fc: ldr      r2, [r4, r3]
004bf500: add      r1, pc, r1
004bf504: bl       #0x4bdccc
004bf508: ldr      r3, [pc, #0x540]
004bf50c: mov      r1, r7
004bf510: mov      r0, r5
004bf514: ldr      r2, [r4, r3]
004bf518: ldr      r3, [pc, #0x534]
004bf51c: ldr      r7, [pc, #0x534]
004bf520: ldr      r3, [r4, r3]
004bf524: bl       #0x4be3e0
004bf528: ldr      r3, [pc, #0x52c]
004bf52c: mov      r1, r6
004bf530: mov      r0, r5
004bf534: ldr      r2, [r4, r3]
004bf538: ldr      r3, [pc, #0x520]
004bf53c: add      r7, pc, r7
004bf540: ldr      r6, [pc, #0x51c]
004bf544: ldr      r3, [r4, r3]
004bf548: bl       #0x4be3e0
004bf54c: b        #0x4bff30
004bf550: subeq    r6, sp, r0, lsr r5
004bf554: andeq    r1, r0, r8, lsl r0
004bf558: strheq   r2, [r0], -r0
004bf55c: ldrdeq   r2, r3, [r0], -ip
004bf560: subeq    r7, r1, ip, lsr #2
004bf564: andeq    r4, r0, ip, lsr fp
004bf568: subeq    r8, r1, r4, lsl #7
004bf56c: andeq    r2, r0, r4, lsl #25
004bf570: umaaleq  r7, r1, ip, r1
004bf574: ldrdeq   r0, r1, [r0], -r0
004bf578: subeq    sb, r1, r4, lsl #16
004bf57c: andeq    r4, r0, r4, lsr #15
004bf580: strdeq   sb, sl, [r1], #-0x74
004bf584: strheq   r1, [r0], -r0
004bf588: subeq    r7, r1, r0, lsr #2
004bf58c: strheq   r1, [r0], -r0
004bf590: andeq    r3, r0, r4, lsr #31
004bf594: subeq    r8, r1, ip, lsl #7
004bf598: andeq    r1, r0, r8, lsl fp
004bf59c: subeq    r8, r1, r8, asr #7
004bf5a0: andeq    r0, r0, r4, ror #12
004bf5a4: umaaleq  sb, r1, ip, r7
004bf5a8: andeq    r3, r0, r8, ror #27
004bf5ac: umaaleq  sb, r1, r4, r7
004bf5b0: muleq    r0, ip, ip
004bf5b4: muleq    r0, ip, ip
004bf5b8: andeq    r2, r0, ip, lsl #27
004bf5bc: strdeq   r2, r3, [r0], -ip
004bf5c0: ldrdeq   r4, r5, [r0], -r0
004bf5c4: subeq    sb, r1, r0, asr r7
004bf5c8: andeq    r2, r0, r8, lsr sp
004bf5cc: subeq    sb, r1, r0, asr #14
004bf5d0: ldrdeq   r2, r3, [r0], -r0
004bf5d4: andeq    r1, r0, ip, asr sl
004bf5d8: andeq    r4, r0, r0, asr #24
004bf5dc: andeq    r4, r0, r0, lsl sb
004bf5e0: andeq    r0, r0, r4, asr #28
004bf5e4: subeq    sb, r1, r0, lsl #14
004bf5e8: andeq    r1, r0, r0, lsr #28
004bf5ec: strdeq   sb, sl, [r1], #-0x68
004bf5f0: andeq    r2, r0, r0, ror #11
004bf5f4: andeq    r4, r0, r0, asr #13
004bf5f8: subeq    r7, r1, r4, asr #2
004bf5fc: andeq    r0, r0, r4, lsr #29
004bf600: andeq    r4, r0, ip, lsr r4
004bf604: strheq   r8, [r1], #-0x3c
004bf608: andeq    r1, r0, r8, ror fp
004bf60c: subeq    sb, r1, ip, lsr #13
004bf610: andeq    r4, r0, r4, lsl #13
004bf614: subeq    sb, r1, r4, lsr #13
004bf618: andeq    r4, r0, r8, lsr fp
004bf61c: subeq    r7, r1, r0, rrx
004bf620: andeq    r1, r0, r0, ror #9
004bf624: andeq    r1, r0, ip, lsl #27
004bf628: strdeq   r8, sb, [r1], #-0x24
004bf62c: andeq    r3, r0, r4, ror #16
004bf630: andeq    r1, r0, r0, lsr r0
004bf634: subeq    sb, r1, r8, asr r6
004bf638: andeq    r2, r0, r4, ror #11
004bf63c: subeq    sb, r1, r0, asr r6
004bf640: strheq   r3, [r0], -ip
004bf644: andeq    r2, r0, ip, lsl #9
004bf648: andeq    r3, r0, ip, asr #32
004bf64c: andeq    r2, r0, r8, ror #24
004bf650: andeq    r3, r0, ip, lsl r2
004bf654: subeq    sb, r1, r0, lsl r6
004bf658: andeq    r3, r0, ip, ror fp
004bf65c: subeq    sb, r1, r0, lsl r6
004bf660: andeq    r3, r0, ip, lsl sb
004bf664: strheq   r1, [r0], -r8
004bf668: andeq    r3, r0, r8, ror r6
004bf66c: muleq    r0, r0, r6
004bf670: andeq    r4, r0, r0, asr r1
004bf674: ldrdeq   sb, sl, [r1], #-0x50
004bf678: andeq    r4, r0, r0, lsr #22
004bf67c: ldrdeq   sb, sl, [r1], #-0x58
004bf680: ldrdeq   r3, r4, [r0], -r8
004bf684: andeq    r4, r0, r4, lsl #23
004bf688: subeq    r7, r1, r4, lsr r1
004bf68c: andeq    r1, r0, r4, ror #2
004bf690: andeq    r3, r0, r0, ror #31
004bf694: subeq    r8, r1, r4, asr #7
004bf698: andeq    r4, r0, r4, lsl #24
004bf69c: umaaleq  sb, r1, ip, r5
004bf6a0: andeq    r4, r0, r8, lsr #22
004bf6a4: umaaleq  sb, r1, r4, r5
004bf6a8: andeq    r2, r0, r0, lsr #3
004bf6ac: ldrdeq   r6, r7, [r1], #-0xf8
004bf6b0: andeq    r2, r0, r4, asr ip
004bf6b4: andeq    r2, r0, r4, ror #19
004bf6b8: subeq    r8, r1, ip, lsl #5
004bf6bc: andeq    r0, r0, r0, ror #13
004bf6c0: ldrdeq   r0, r1, [r0], -r0
004bf6c4: subeq    sb, r1, r8, asr #10
004bf6c8: andeq    r0, r0, r8, ror #30
004bf6cc: subeq    sb, r1, r0, asr #10
004bf6d0: andeq    r2, r0, r0, lsr #11
004bf6d4: strdeq   r6, r7, [r1], #-0xf4
004bf6d8: muleq    r0, r0, sb
004bf6dc: strheq   r3, [r0], -ip
004bf6e0: strheq   r8, [r1], #-0x24
004bf6e4: andeq    r0, r0, r0, lsr r7
004bf6e8: andeq    r4, r0, r4, ror #2
004bf6ec: subeq    r3, r0, r8, ror ip
004bf6f0: andeq    r2, r0, ip, lsl r0
004bf6f4: subeq    sb, r1, r8, ror #9
004bf6f8: strheq   r3, [r0], -r8
004bf6fc: andeq    r3, r0, r4
004bf700: andeq    r2, r0, r4, ror #10
004bf704: andeq    r4, r0, ip, lsr r0
004bf708: andeq    r3, r0, ip, lsl #11
004bf70c: subeq    sb, r1, r8, lsr #9
004bf710: muleq    r0, r8, r0
004bf714: subeq    sb, r1, r0, lsr #9
004bf718: andeq    r3, r0, r8, ror #28
004bf71c: strheq   r0, [r0], -ip
004bf720: andeq    r4, r0, ip, asr #32
004bf724: andeq    r3, r0, r0, ror #5
004bf728: andeq    r1, r0, r4, asr #21
004bf72c: subeq    sb, r1, r8, ror #8
004bf730: andeq    r2, r0, ip, ror #26
004bf734: subeq    sb, r1, r0, ror #8
004bf738: andeq    r0, r0, r4, lsl #19
004bf73c: strdeq   r4, r5, [r0], -ip
004bf740: subeq    r6, r1, r4, lsl #31
004bf744: andeq    r3, r0, r0, asr #14
004bf748: andeq    r3, r0, ip, ror #6
004bf74c: subeq    r8, r1, r0, lsr #4
004bf750: andeq    r3, r0, r0, ror r2
004bf754: subeq    sb, r1, ip, lsl r4
004bf758: andeq    r0, r0, ip, lsl #15
004bf75c: subeq    sb, r1, r4, lsl r4
004bf760: andeq    r2, r0, r0, lsr #5
004bf764: strdeq   r0, r1, [r0], -ip
004bf768: andeq    r3, r0, r8, asr r4
004bf76c: andeq    r4, r0, r8, asr #11
004bf770: andeq    r4, r0, r4, lsl #16
004bf774: ldrdeq   sb, sl, [r1], #-0x30
004bf778: andeq    r0, r0, ip, lsl #16
004bf77c: subeq    sb, r1, r8, asr #7
004bf780: andeq    r3, r0, r0, ror #27
004bf784: strheq   r3, [r0], -r4
004bf788: subeq    r6, r1, ip, lsl pc
004bf78c: andeq    r0, r0, r8, ror r8
004bf790: strheq   r1, [r0], -r0
004bf794: subeq    r8, r1, r8, asr #3
004bf798: strheq   r0, [r0], -r0
004bf79c: subeq    sb, r1, ip, ror r3
004bf7a0: andeq    r3, r0, r4, ror #31
004bf7a4: subeq    sb, r1, r4, ror r3
004bf7a8: andeq    r2, r0, r8, asr fp
004bf7ac: strheq   r1, [r0], -ip
004bf7b0: andeq    r2, r0, r0, lsl sp
004bf7b4: andeq    r3, r0, r8, ror r5
004bf7b8: andeq    r0, r0, r4, ror #13
004bf7bc: subeq    sb, r1, r8, lsr #6
004bf7c0: andeq    r1, r0, r0, ror #10
004bf7c4: subeq    sb, r1, r0, lsr #6
004bf7c8: andeq    r1, r0, r8, lsr #14
004bf7cc: andeq    r0, r0, r8, ror r7
004bf7d0: andeq    r0, r0, ip, lsl r8
004bf7d4: andeq    r3, r0, r0, lsl #5
004bf7d8: andeq    r2, r0, r4, ror #9
004bf7dc: subeq    sb, r1, r8, ror #5
004bf7e0: andeq    r1, r0, r4, asr #9
004bf7e4: subeq    sb, r1, r0, ror #5
004bf7e8: ldrdeq   r3, r4, [r0], -r4
004bf7ec: andeq    r1, r0, ip, ror r2
004bf7f0: subeq    r6, r1, ip, asr #28
004bf7f4: strdeq   r0, r1, [r0], -ip
004bf7f8: andeq    r2, r0, ip, lsl #10
004bf7fc: subeq    r8, r1, r8, lsl #2
004bf800: andeq    r2, r0, r8, lsr ip
004bf804: umaaleq  sb, r1, r4, r2
004bf808: andeq    r1, r0, r8, lsr ip
004bf80c: subeq    sb, r1, ip, lsl #5
004bf810: andeq    r1, r0, r4, lsr #28
004bf814: muleq    r0, r8, sb
004bf818: ldrdeq   r3, r4, [r0], -ip
004bf81c: andeq    r2, r0, ip, lsr r2
004bf820: andeq    r4, r0, ip, lsr #5
004bf824: subeq    sb, r1, r0, asr r2
004bf828: andeq    r4, r0, r0, ror #10
004bf82c: subeq    sb, r1, r8, asr #4
004bf830: andeq    r1, r0, r4, lsl #28
004bf834: andeq    r3, r0, r4, lsl #3
004bf838: subeq    r6, r1, ip, asr lr
004bf83c: andeq    r1, r0, r0, lsl #19
004bf840: andeq    r1, r0, ip, lsr #24
004bf844: subeq    r8, r1, ip, lsr #2
004bf848: andeq    r1, r0, r0, asr #27
004bf84c: strdeq   sb, sl, [r1], #-0x1c
004bf850: ldrdeq   r4, r5, [r0], -r0
004bf854: subeq    sb, r1, ip, ror #3
004bf858: andeq    r1, r0, r8, asr #30
004bf85c: umaaleq  r6, r1, r0, sp
004bf860: andeq    r2, r0, r8, asr pc
004bf864: andeq    r1, r0, ip, ror #17
004bf868: subeq    r8, r1, ip, lsl #1
004bf86c: andeq    r4, r0, r0, ror #8
004bf870: andeq    r3, r0, r0, lsl #8
004bf874: subeq    sb, r1, r0, lsr #3
004bf878: andeq    r1, r0, r0, lsl #1
004bf87c: umaaleq  sb, r1, r8, r1
004bf880: andeq    r2, r0, r4, rrx
004bf884: andeq    r3, r0, r0, ror #3
004bf888: andeq    r4, r0, r8, ror r5
004bf88c: strdeq   r1, r2, [r0], -r4
004bf890: andeq    r1, r0, ip, lsl #6
004bf894: subeq    r1, r0, r0, asr #12
004bf898: andeq    r2, r0, r4, lsl #15
004bf89c: subeq    sb, r1, r0, asr #2
004bf8a0: strdeq   r1, r2, [r0], -r0
004bf8a4: strheq   r1, [r0], -ip
004bf8a8: andeq    r0, r0, r8, lsl #22
004bf8ac: andeq    r0, r0, ip, lsr #24
004bf8b0: muleq    r0, r0, lr
004bf8b4: subeq    r1, r0, r8, ror r5
004bf8b8: andeq    r2, r0, ip, asr r7
004bf8bc: strdeq   sb, sl, [r1], #-0
004bf8c0: strdeq   r0, r1, [r0], -r0
004bf8c4: andeq    r2, r0, r8, lsr r7
004bf8c8: andeq    r1, r0, r8, ror #8
004bf8cc: ldrdeq   r2, r3, [r0], -r4
004bf8d0: andeq    r2, r0, r8, asr #22
004bf8d4: subeq    sb, r1, r8, lsr #1
004bf8d8: andeq    r2, r0, r4, asr #18
004bf8dc: subeq    sb, r1, r0, lsr #1
004bf8e0: andeq    r3, r0, ip, asr r6
004bf8e4: andeq    r0, r0, r8, lsr #21
004bf8e8: andeq    r4, r0, ip, lsl #14
004bf8ec: muleq    r0, r0, r3
004bf8f0: andeq    r3, r0, ip, lsr r2
004bf8f4: subeq    sb, r1, r0, rrx
004bf8f8: muleq    r0, ip, ip
004bf8fc: subeq    sb, r1, r0, rrx
004bf900: andeq    r0, r0, r0, asr #14
004bf904: strheq   r2, [r0], -ip
004bf908: andeq    r0, r0, r0, ror #31
004bf90c: ldrdeq   r3, r4, [r0], -ip
004bf910: andeq    r4, r0, ip, ror #9
004bf914: subeq    r1, r0, r8, ror #8
004bf918: andeq    r1, r0, r4, lsr #11
004bf91c: subeq    sb, r1, r0, lsl r0
004bf920: andeq    r2, r0, ip, ror r5
004bf924: ldrdeq   r1, r2, [r0], -r8
004bf928: andeq    r1, r0, ip, lsl fp
004bf92c: andeq    r4, r0, r4, asr r2
004bf930: muleq    r0, r8, sp
004bf934: subeq    r1, r0, r0, lsl r4
004bf938: strheq   r1, [r0], -r4
004bf93c: strheq   r8, [r1], #-0xf8
004bf940: andeq    r2, r0, ip, ror pc
004bf944: andeq    r0, r0, r8, lsr #20
004bf948: andeq    r3, r0, r8, asr #24
004bf94c: andeq    r2, r0, r4, lsl lr
004bf950: andeq    r0, r0, r4, ror #25
004bf954: subeq    r1, r0, r0, ror #6
004bf958: andeq    r4, r0, r8, lsr r2
004bf95c: subeq    r8, r1, r8, ror #30
004bf960: andeq    r3, r0, r0, lsl r0
004bf964: andeq    r1, r0, r4, lsl #26
004bf968: muleq    r0, ip, r4
004bf96c: andeq    r0, r0, ip, ror sl
004bf970: muleq    r0, ip, lr
004bf974: subeq    r1, r0, r8, ror #5
004bf978: andeq    r2, r0, r8, lsl #25
004bf97c: subeq    r8, r1, r0, lsl pc
004bf980: strheq   r0, [r0], -r8
004bf984: strheq   r2, [r0], -r4
004bf988: andeq    r2, r0, r8, ror #29
004bf98c: andeq    r4, r0, r8, ror r3
004bf990: strdeq   r4, r5, [r0], -r8
004bf994: subeq    r1, r0, r8, lsr #4
004bf998: andeq    r2, r0, r0, lsl #6
004bf99c: strheq   r8, [r1], #-0xe8
004bf9a0: ldrdeq   r0, r1, [r0], -r4
004bf9a4: andeq    r3, r0, r8, lsr #3
004bf9a8: andeq    r2, r0, r0, lsr r4
004bf9ac: andeq    r3, r0, ip, ror r2
004bf9b0: andeq    r4, r0, r8, lsl #15
004bf9b4: ldrdeq   r1, r2, [r0], #-0x10
004bf9b8: muleq    r0, r4, r4
004bf9bc: subeq    r8, r1, r0, ror #28
004bf9c0: andeq    r0, r0, ip, ror #21
004bf9c4: ldrdeq   r0, r1, [r0], -r4
004bf9c8: subeq    r6, r1, ip, ror #19
004bf9cc: andeq    r3, r0, r4, ror #15
004bf9d0: andeq    r4, r0, r8, asr #4
004bf9d4: subeq    r7, r1, r8, asr #25
004bf9d8: andeq    r4, r0, ip, rrx
004bf9dc: umaaleq  r1, r0, r4, r1
004bf9e0: andeq    r1, r0, r4, asr lr
004bf9e4: strdeq   r8, sb, [r1], #-0xdc
004bf9e8: andeq    r1, r0, r4, lsr #17
004bf9ec: andeq    r0, r0, r4, asr #29
004bf9f0: andeq    r1, r0, ip, lsr #21
004bf9f4: andeq    r1, r0, r8, asr r7
004bf9f8: andeq    r2, r0, ip, lsl r7
004bf9fc: strheq   r8, [r1], #-0xd8
004bfa00: strheq   r1, [r0], -r0
004bfa04: strheq   r8, [r1], #-0xd0
004bfa08: andeq    r4, r0, r4, ror #4
004bfa0c: andeq    r2, r0, r4, ror #6
004bfa10: subeq    r6, r1, r4, lsl #19
004bfa14: andeq    r4, r0, ip, lsl #16
004bfa18: andeq    r0, r0, r4, asr r6
004bfa1c: subeq    r7, r1, r8, ror ip
004bfa20: strheq   r0, [r0], -r0
004bfa24: subeq    r8, r1, r4, ror #26
004bfa28: andeq    r2, r0, r4, lsr #15
004bfa2c: subeq    r8, r1, ip, asr sp
004bfa30: andeq    r2, r0, r4, lsl r6
004bfa34: andeq    r1, r0, r0, lsr #27
004bfa38: andeq    r4, r0, r4, lsr r0
004bfa3c: andeq    r1, r0, ip, lsr #9
004bfa40: strdeq   r2, r3, [r0], -r0
004bfa44: subeq    r8, r1, r8, lsl sp
004bfa48: andeq    r4, r0, r0, lsr #16
004bfa4c: subeq    r8, r1, r8, lsl sp
004bfa50: andeq    r1, r0, r0, asr pc
004bfa54: andeq    r1, r0, r8, lsr #15
004bfa58: subeq    r6, r1, r4, asr #19
004bfa5c: andeq    r0, r0, ip, lsl #28
004bfa60: andeq    r4, r0, r0, lsr #24
004bfa64: subeq    r7, r1, r0, ror #5
004bfa68: andeq    r3, r0, r0, lsl #3
004bfa6c: subeq    r8, r1, r8, ror #5
004bfa70: andeq    r1, r0, ip, asr #17
004bfa74: subeq    r8, r1, r0, ror #5
004bfa78: andeq    r2, r0, ip, lsr #2
004bfa7c: strdeq   r5, r6, [r1], #-0xe4
004bfa80: andeq    r2, r0, r8, lsr r6
004bfa84: andeq    r0, r0, ip, lsr #25
004bfa88: subeq    r7, r1, r8, lsr #4
004bfa8c: andeq    r3, r0, r8, asr fp
004bfa90: andeq    r1, r0, r4, ror #19
004bfa94: umaaleq  r8, r1, r4, r2
004bfa98: strheq   r4, [r0], -ip
004bfa9c: umaaleq  r8, r1, r4, r2
004bfaa0: andeq    r0, r0, r8, lsl #11
004bfaa4: andeq    r1, r0, r0, asr #28
004bfaa8: strdeq   r3, r4, [r0], -r0
004bfaac: andeq    r3, r0, r8, asr #19
004bfab0: andeq    r2, r0, r4, lsr r6
004bfab4: subeq    r8, r1, ip, asr r2
004bfab8: andeq    r2, r0, r4, lsl sb
004bfabc: subeq    r8, r1, ip, asr r2
004bfac0: ldrdeq   r4, r5, [r0], -r8
004bfac4: andeq    r2, r0, r8, ror #16
004bfac8: umaaleq  r5, r1, r8, pc
004bfacc: andeq    r0, r0, r4, lsl #20
004bfad0: andeq    r4, r0, ip, asr #19
004bfad4: subeq    r7, r1, r0, asr #5
004bfad8: andeq    r1, r0, ip, lsr #31
004bfadc: subeq    r8, r1, r0, lsl r2
004bfae0: strdeq   r1, r2, [r0], -r8
004bfae4: subeq    r8, r1, r0, lsl r2
004bfae8: ldrdeq   r3, r4, [r0], -r0
004bfaec: strheq   r5, [r1], #-0xec
004bfaf0: andeq    r2, r0, r0, ror #31
004bfaf4: strdeq   r2, r3, [r0], -r0
004bfaf8: subeq    r7, r1, r0, lsl #4
004bfafc: andeq    r4, r0, r4, lsl #10
004bfb00: andeq    r2, r0, r8, asr sl
004bfb04: subeq    r8, r1, r4, asr #3
004bfb08: muleq    r0, ip, pc
004bfb0c: strheq   r8, [r1], #-0x1c
004bfb10: andeq    r1, r0, r8, lsl #8
004bfb14: andeq    r1, r0, ip, ror #27
004bfb18: andeq    r4, r0, r8
004bfb1c: muleq    r0, r0, r3
004bfb20: andeq    r0, r0, ip, lsl #12
004bfb24: subeq    r8, r1, r4, lsl #3
004bfb28: strdeq   r0, r1, [r0], -r8
004bfb2c: subeq    r8, r1, ip, ror r1
004bfb30: andeq    r3, r0, r4, lsr #27
004bfb34: strheq   r4, [r0], -ip
004bfb38: andeq    r4, r0, r8, asr #5
004bfb3c: andeq    r4, r0, r4, lsl #7
004bfb40: strheq   r4, [r0], -r0
004bfb44: subeq    r7, r0, ip, ror r2
004bfb48: andeq    r0, r0, r8, lsl #14
004bfb4c: subeq    r8, r1, ip, lsr #2
004bfb50: strheq   r0, [r0], -r0
004bfb54: andeq    r3, r0, ip, asr #11
004bfb58: andeq    r2, r0, r0, lsl fp
004bfb5c: strdeq   r2, r3, [r0], -r0
004bfb60: andeq    r0, r0, r8, asr #25
004bfb64: subeq    r8, r1, ip, ror #1
004bfb68: muleq    r0, ip, fp
004bfb6c: subeq    r8, r1, ip, ror #1
004bfb70: andeq    r0, r0, r0, ror #12
004bfb74: andeq    r2, r0, r8, ror r1
004bfb78: andeq    r4, r0, r4, lsr #10
004bfb7c: andeq    r1, r0, ip, ror #31
004bfb80: andeq    r1, r0, r0, lsr #10
004bfb84: ldrdeq   sl, fp, [r0], #-0x2c
004bfb88: strheq   r0, [r0], -r0
004bfb8c: umaaleq  r8, r1, r4, r0
004bfb90: muleq    r0, ip, lr
004bfb94: andeq    r4, r0, r8, lsr #19
004bfb98: ldrdeq   r0, r1, [r0], -r4
004bfb9c: andeq    r1, r0, r0, lsr #18
004bfba0: andeq    r0, r0, r8, ror #12
004bfba4: subeq    r8, r1, r4, asr r0
004bfba8: muleq    r0, r0, r0
004bfbac: subeq    r8, r1, r4, asr r0
004bfbb0: andeq    r4, r0, ip, ror #2
004bfbb4: andeq    r1, r0, r0, asr r6
004bfbb8: andeq    r2, r0, r4, ror r0
004bfbbc: andeq    r2, r0, ip, asr #6
004bfbc0: andeq    r2, r0, r4, lsr sp
004bfbc4: eorseq   lr, pc, ip, lsl fp
004bfbc8: andeq    r3, r0, ip, lsl #1
004bfbcc: strdeq   r7, r8, [r1], #-0xfc
004bfbd0: andeq    r4, r0, r4, ror r6
004bfbd4: andeq    r3, r0, ip, asr #21
004bfbd8: andeq    r3, r0, r0, ror #20
004bfbdc: andeq    r0, r0, r0, lsr sp
004bfbe0: andeq    r3, r0, ip, lsl #29
004bfbe4: strheq   r7, [r1], #-0xfc
004bfbe8: andeq    r3, r0, ip, lsl #20
004bfbec: strheq   r7, [r1], #-0xf4
004bfbf0: strdeq   r4, r5, [r0], -r4
004bfbf4: andeq    r3, r0, r0, lsl #20
004bfbf8: ldrdeq   r5, r6, [r1], #-0xc0
004bfbfc: strheq   r1, [r0], -r4
004bfc00: andeq    r0, r0, r4, lsr ip
004bfc04: subeq    r7, r1, r8, lsl r0
004bfc08: andeq    r2, r0, ip, ror r6
004bfc0c: subeq    r7, r1, r8, ror #30
004bfc10: andeq    r3, r0, r4, asr r2
004bfc14: subeq    r7, r1, r0, ror #30
004bfc18: muleq    r0, r0, r0
004bfc1c: strdeq   r5, r6, [r1], #-0xbc
004bfc20: andeq    r3, r0, ip, ror #16
004bfc24: ldrdeq   r1, r2, [r0], -r4
004bfc28: subeq    r6, r1, r0, ror pc
004bfc2c: andeq    r1, r0, ip, lsr sp
004bfc30: andeq    r4, r0, r0, lsl r3
004bfc34: subeq    r0, r0, r4, lsl r1
004bfc38: andeq    r4, r0, r8, lsl r6
004bfc3c: strdeq   r7, r8, [r1], #-0xec
004bfc40: strdeq   r3, r4, [r0], -r0
004bfc44: strdeq   r2, r3, [r0], -r8
004bfc48: andeq    r2, r0, r8, lsr #9
004bfc4c: andeq    r3, r0, r4, asr fp
004bfc50: andeq    r3, r0, r0, lsl #28
004bfc54: strheq   r7, [r1], #-0xec
004bfc58: andeq    r4, r0, ip, lsr #11
004bfc5c: strheq   r7, [r1], #-0xe4
004bfc60: andeq    r2, r0, r0, ror r5
004bfc64: andeq    r3, r0, ip, lsl #10
004bfc68: strdeq   r5, r6, [r1], #-0xb8
004bfc6c: andeq    r2, r0, r8, asr #27
004bfc70: andeq    r0, r0, r0, ror pc
004bfc74: subeq    r6, r1, ip, asr #30
004bfc78: andeq    r3, r0, r8, lsr #5
004bfc7c: subeq    r7, r1, r8, ror #28
004bfc80: andeq    r3, r0, ip, lsl #2
004bfc84: subeq    r7, r1, r8, asr lr
004bfc88: andeq    r0, r0, ip, lsl sp
004bfc8c: andeq    r3, r0, r4, lsr #17
004bfc90: andeq    r2, r0, r4, asr sb
004bfc94: andeq    r4, r0, r8, lsl r2
004bfc98: andeq    r1, r0, r0, ror #30
004bfc9c: subeq    r7, r1, r4, lsl lr
004bfca0: andeq    r2, r0, r4, ror #15
004bfca4: subeq    r7, r1, ip, lsl #28
004bfca8: andeq    r3, r0, ip, asr r2
004bfcac: ldrdeq   r4, r5, [r0], -r4
004bfcb0: andeq    r1, r0, r4, lsr #14
004bfcb4: strheq   r4, [r0], -r0
004bfcb8: andeq    r4, r0, r0, ror #1
004bfcbc: subeq    r8, r4, r4, lsl #25
004bfcc0: andeq    r2, r0, r8, lsr #24
004bfcc4: strheq   r7, [r1], #-0xd4
004bfcc8: andeq    r4, r0, r4, lsr #20
004bfccc: andeq    r3, r0, ip, ror #8
004bfcd0: andeq    r3, r0, ip, ror #30
004bfcd4: andeq    r2, r0, r0, ror #21
004bfcd8: andeq    r2, r0, ip, ror #7
004bfcdc: subeq    r7, r1, r4, ror sp
004bfce0: andeq    r1, r0, r4, lsr #27
004bfce4: subeq    r7, r1, ip, ror #26
004bfce8: strdeq   r1, r2, [r0], -r4
004bfcec: ldrdeq   r4, r5, [r0], -r0
004bfcf0: andeq    r2, r0, r0, asr #10
004bfcf4: strheq   r4, [r0], -ip
004bfcf8: andeq    r2, r0, ip, ror r0
004bfcfc: subeq    r7, r1, r4, lsr sp
004bfd00: andeq    r3, r0, r8, ror #25
004bfd04: subeq    r7, r1, ip, lsr #26
004bfd08: andeq    r1, r0, r0, lsl lr
004bfd0c: andeq    r2, r0, r8, asr sp
004bfd10: subeq    r5, r1, r8, ror #25
004bfd14: andeq    r2, r0, r4, ror #27
004bfd18: ldrdeq   r0, r1, [r0], -ip
004bfd1c: umaaleq  r7, r1, r0, r0
004bfd20: andeq    r4, r0, r8, lsl r5
004bfd24: subeq    sl, r1, r0, lsl #10
004bfd28: ldrdeq   r1, r2, [r0], -ip
004bfd2c: ldrdeq   r7, r8, [r1], #-0xc0
004bfd30: andeq    r0, r0, ip, lsr ip
004bfd34: subeq    r5, r1, r4, lsl #20
004bfd38: andeq    r2, r0, r0, asr #29
004bfd3c: ldrdeq   r4, r5, [r0], -ip
004bfd40: umaaleq  r6, r1, r8, sp
004bfd44: andeq    r3, r0, r0, lsl #23
004bfd48: andeq    r3, r0, r0, asr r5
004bfd4c: subeq    r7, r1, r4, lsl #25
004bfd50: andeq    r4, r0, ip, lsr #22
004bfd54: subeq    r7, r1, ip, ror ip
004bfd58: andeq    r1, r0, r0, lsr #7
004bfd5c: subeq    r5, r1, r8, lsl sl
004bfd60: andeq    r2, r0, ip, lsl #1
004bfd64: andeq    r2, r0, r0, lsr #17
004bfd68: strheq   r6, [r1], #-0xd8
004bfd6c: andeq    r4, r0, r4, lsl r4
004bfd70: andeq    r1, r0, r8, lsl sb
004bfd74: subeq    r7, r1, r4, lsr ip
004bfd78: andeq    r3, r0, r0, asr #24
004bfd7c: subeq    r7, r1, r4, lsr #24
004bfd80: andeq    r4, r0, r8, lsr r5
004bfd84: subeq    r5, r1, r0, lsr #20
004bfd88: andeq    r3, r0, r4, lsr r0
004bfd8c: andeq    r3, r0, ip, lsr #21
004bfd90: ldrdeq   r6, r7, [r1], #-0xd8
004bfd94: muleq    r0, ip, pc
004bfd98: andeq    r4, r0, r8, lsl sb
004bfd9c: ldrdeq   r7, r8, [r1], #-0xbc
004bfda0: andeq    r2, r0, ip, asr #2
004bfda4: ldrdeq   r7, r8, [r1], #-0xb4
004bfda8: andeq    r1, r0, r4, asr #25
004bfdac: subeq    r5, r1, r0, asr #20
004bfdb0: andeq    r3, r0, r0, lsl #16
004bfdb4: andeq    r2, r0, r8, lsr #11
004bfdb8: strdeq   r6, r7, [r1], #-0xd8
004bfdbc: andeq    r3, r0, r0, asr sp
004bfdc0: andeq    r4, r0, r4, asr #24
004bfdc4: subeq    r7, r1, ip, lsl #23
004bfdc8: andeq    r3, r0, r0, lsl r1
004bfdcc: subeq    r7, r1, ip, ror fp
004bfdd0: andeq    r1, r0, r4, asr #16
004bfdd4: subeq    r5, r1, r8, asr sl
004bfdd8: andeq    r3, r0, r8, lsr fp
004bfddc: andeq    r0, r0, r0, ror #11
004bfde0: subeq    r6, r1, r0, lsr #28
004bfde4: strdeq   r0, r1, [r0], -r0
004bfde8: andeq    r4, r0, r4, lsr fp
004bfdec: subeq    r7, r1, r4, lsr fp
004bfdf0: andeq    r0, r0, r0, lsl #27
004bfdf4: subeq    r7, r1, r4, lsr #22
004bfdf8: andeq    r1, r0, r0, lsr #1
004bfdfc: strheq   r1, [r0], -r8
004bfe00: andeq    r3, r0, r0, lsr #29
004bfe04: andeq    r1, r0, r8, lsr #7
004bfe08: andeq    r0, r0, r8, lsr ip
004bfe0c: subeq    r7, r1, r4, ror #21
004bfe10: muleq    r0, ip, r3
004bfe14: ldrdeq   r7, r8, [r1], #-0xac
004bfe18: andeq    r2, r0, ip, lsl #12
004bfe1c: andeq    r3, r0, r4, ror #1
004bfe20: andeq    r1, r0, r8, ror #16
004bfe24: andeq    r2, r0, ip, asr #24
004bfe28: subeq    r7, r1, r0, lsl #21
004bfe2c: andeq    r4, r0, r0, ror r0
004bfe30: umaaleq  r7, r1, r8, sl
004bfe34: andeq    r2, r0, ip, rrx
004bfe38: umaaleq  r7, r1, r0, sl
004bfe3c: muleq    r0, r8, r5
004bfe40: subeq    r5, r1, r4, lsl sl
004bfe44: ldrdeq   r1, r2, [r0], -r8
004bfe48: strheq   r1, [r0], -r4
004bfe4c: subeq    r6, r1, r8, lsl #28
004bfe50: andeq    r2, r0, r4, lsr pc
004bfe54: andeq    r2, r0, r4, asr sp
004bfe58: strdeq   r2, r3, [r0], -ip
004bfe5c: subeq    r7, r1, ip, lsr sl
004bfe60: andeq    r4, r0, r8, lsr r0
004bfe64: subeq    r5, r1, r0, asr sl
004bfe68: andeq    r3, r0, r0, lsl #29
004bfe6c: strheq   r4, [r0], -r8
004bfe70: subeq    r6, r1, r0, asr lr
004bfe74: andeq    r2, r0, r0, lsl sb
004bfe78: andeq    r1, r0, r8, lsl ip
004bfe7c: subeq    r7, r1, r4, ror #19
004bfe80: ldrdeq   r0, r1, [r0], -r0
004bfe84: subeq    r5, r1, r8, lsl #21
004bfe88: andeq    r1, r0, r0, asr #32
004bfe8c: andeq    r1, r0, r8, asr pc
004bfe90: umaaleq  r6, r1, r0, lr
004bfe94: ldrdeq   r2, r3, [r0], -r4
004bfe98: ldrdeq   r4, r5, [r0], -ip
004bfe9c: subeq    r7, r1, ip, lsl #19
004bfea0: andeq    r3, r0, ip, lsr r7
004bfea4: strheq   r5, [r1], #-0xa8
004bfea8: andeq    r3, r0, r8, lsr lr
004bfeac: strdeq   r4, r5, [r0], -r4
004bfeb0: ldrdeq   r6, r7, [r1], #-0xe0
004bfeb4: andeq    r2, r0, r8, lsl #5
004bfeb8: andeq    r4, r0, r4, asr #3
004bfebc: subeq    r7, r1, r4, lsr sb
004bfec0: andeq    r3, r0, r4, ror ip
004bfec4: strdeq   r5, r6, [r1], #-0xa8
004bfec8: andeq    r2, r0, ip, lsr r6
004bfecc: andeq    r1, r0, ip, lsr #27
004bfed0: subeq    r6, r1, r8, lsl pc
004bfed4: andeq    r0, r0, r0, lsl #25
004bfed8: andeq    r4, r0, r8, asr r6
004bfedc: ldrdeq   r7, r8, [r1], #-0x8c
004bfee0: strheq   r3, [r0], -ip
004bfee4: subeq    r5, r1, r8, lsr #22
004bfee8: andeq    r1, r0, ip, ror #26
004bfeec: andeq    r0, r0, r0, lsr #17
004bfef0: subeq    r6, r1, r0, ror #30
004bfef4: andeq    r4, r0, ip, lsr r3
004bfef8: andeq    r1, r0, r8, asr #8
004bfefc: umaaleq  r7, r1, r4, r8
004bff00: strdeq   r2, r3, [r0], -r4
004bff04: subeq    r7, r1, ip, lsl #17
004bff08: muleq    r0, ip, r1
004bff0c: subeq    r5, r1, r8, asr #22
004bff10: andeq    r4, r0, r0, lsl #14
004bff14: muleq    r0, ip, r1
004bff18: umaaleq  r6, r1, r0, pc
004bff1c: andeq    r3, r0, r0, asr r0
004bff20: andeq    r4, r0, ip, asr #24
004bff24: subeq    r7, r1, r4, asr #16
004bff28: andeq    r0, r0, ip, ror #20
004bff2c: subeq    r7, r1, ip, lsr r8
004bff30: ldr      r3, [pc, #-0x4d0]
004bff34: ldr      r1, [pc, #-0x4d0]
004bff38: mov      r0, r5
004bff3c: ldr      r2, [r4, r3]
004bff40: add      r1, pc, r1
004bff44: bl       #0x4bdccc
004bff48: ldr      r3, [pc, #-0x4e0]
004bff4c: ldr      r1, [pc, #-0x4e0]
004bff50: mov      r0, r5
004bff54: ldr      r2, [r4, r3]
004bff58: add      r1, pc, r1
004bff5c: bl       #0x4bdccc
004bff60: ldr      r3, [pc, #-0x4f0]
004bff64: ldr      r1, [pc, #-0x4f0]
004bff68: mov      r0, r5
004bff6c: ldr      r2, [r4, r3]
004bff70: ldr      r3, [pc, #-0x4f8]
004bff74: add      r1, pc, r1
004bff78: add      r6, pc, r6
004bff7c: ldr      r3, [r4, r3]
004bff80: bl       #0x4be3e0
004bff84: ldr      r3, [pc, #-0x508]
004bff88: ldr      r1, [pc, #-0x508]
004bff8c: mov      r0, r5
004bff90: ldr      r2, [r4, r3]
004bff94: ldr      r3, [pc, #-0x510]
004bff98: add      r1, pc, r1
004bff9c: ldr      r3, [r4, r3]
004bffa0: bl       #0x4be3e0
004bffa4: ldr      r3, [pc, #-0x51c]
004bffa8: ldr      r1, [pc, #-0x51c]
004bffac: mov      r0, r5
004bffb0: ldr      r2, [r4, r3]
004bffb4: add      r1, pc, r1
004bffb8: bl       #0x4bdccc
004bffbc: ldr      r3, [pc, #-0x52c]
004bffc0: ldr      r1, [pc, #-0x52c]
004bffc4: mov      r0, r5
004bffc8: ldr      r2, [r4, r3]
004bffcc: add      r1, pc, r1
004bffd0: bl       #0x4bdccc
004bffd4: ldr      r3, [pc, #-0x53c]
004bffd8: mov      r0, r5
004bffdc: mov      r1, r7
004bffe0: ldr      r2, [r4, r3]
004bffe4: ldr      r3, [pc, #-0x548]
004bffe8: ldr      r3, [r4, r3]
004bffec: bl       #0x4be3e0
004bfff0: ldr      r3, [pc, #-0x550]
004bfff4: mov      r0, r5
004bfff8: mov      r1, r6
004bfffc: ldr      r2, [r4, r3]
004c0000: ldr      r3, [pc, #-0x55c]
004c0004: ldr      r3, [r4, r3]
004c0008: bl       #0x4be3e0
004c000c: ldr      r3, [pc, #-0x564]
004c0010: ldr      r1, [pc, #-0x564]
004c0014: mov      r0, r5
004c0018: ldr      r2, [r4, r3]
004c001c: add      r1, pc, r1
004c0020: bl       #0x4bdccc
004c0024: ldr      r3, [pc, #-0x574]
004c0028: ldr      r1, [pc, #-0x574]
004c002c: mov      r0, r5
004c0030: ldr      r2, [r4, r3]
004c0034: add      r1, pc, r1
004c0038: bl       #0x4bdccc
004c003c: ldr      r3, [pc, #-0x584]
004c0040: mov      r1, r7
004c0044: mov      r0, r5
004c0048: ldr      r2, [r4, r3]
004c004c: ldr      r3, [pc, #-0x590]
004c0050: ldr      r7, [pc, #-0x590]
004c0054: ldr      r3, [r4, r3]
004c0058: bl       #0x4be3e0
004c005c: ldr      r3, [pc, #-0x598]
004c0060: mov      r1, r6
004c0064: mov      r0, r5
004c0068: ldr      r2, [r4, r3]
004c006c: ldr      r3, [pc, #-0x5a4]
004c0070: add      r7, pc, r7
004c0074: ldr      r6, [pc, #-0x5a8]
004c0078: ldr      r3, [r4, r3]
004c007c: bl       #0x4be3e0
004c0080: ldr      r3, [pc, #-0x5b0]
004c0084: ldr      r1, [pc, #-0x5b0]
004c0088: mov      r0, r5
004c008c: ldr      r2, [r4, r3]
004c0090: add      r1, pc, r1
004c0094: bl       #0x4bdccc
004c0098: ldr      r3, [pc, #-0x5c0]
004c009c: ldr      r1, [pc, #-0x5c0]
004c00a0: mov      r0, r5
004c00a4: ldr      r2, [r4, r3]
004c00a8: add      r1, pc, r1
004c00ac: bl       #0x4bdccc
004c00b0: ldr      r3, [pc, #-0x5d0]
004c00b4: ldr      r1, [pc, #-0x5d0]
004c00b8: mov      r0, r5
004c00bc: ldr      r2, [r4, r3]
004c00c0: ldr      r3, [pc, #-0x5d8]
004c00c4: add      r1, pc, r1
004c00c8: add      r6, pc, r6
004c00cc: ldr      r3, [r4, r3]
004c00d0: bl       #0x4be3e0
004c00d4: ldr      r3, [pc, #-0x5e8]
004c00d8: ldr      r1, [pc, #-0x5e8]
004c00dc: mov      r0, r5
004c00e0: ldr      r2, [r4, r3]
004c00e4: ldr      r3, [pc, #-0x5f0]
004c00e8: add      r1, pc, r1
004c00ec: ldr      r3, [r4, r3]
004c00f0: bl       #0x4be3e0
004c00f4: ldr      r3, [pc, #-0x5fc]
004c00f8: ldr      r1, [pc, #-0x5fc]
004c00fc: mov      r0, r5
004c0100: ldr      r2, [r4, r3]
004c0104: add      r1, pc, r1
004c0108: bl       #0x4bdccc
004c010c: ldr      r3, [pc, #-0x60c]
004c0110: ldr      r1, [pc, #-0x60c]
004c0114: mov      r0, r5
004c0118: ldr      r2, [r4, r3]
004c011c: add      r1, pc, r1
004c0120: bl       #0x4bdccc
004c0124: ldr      r3, [pc, #-0x61c]
004c0128: mov      r0, r5
004c012c: mov      r1, r7
004c0130: ldr      r2, [r4, r3]
004c0134: ldr      r3, [pc, #-0x628]
004c0138: ldr      r3, [r4, r3]
004c013c: bl       #0x4be3e0
004c0140: ldr      r3, [pc, #-0x630]
004c0144: mov      r0, r5
004c0148: mov      r1, r6
004c014c: ldr      r2, [r4, r3]
004c0150: ldr      r3, [pc, #-0x63c]
004c0154: ldr      r3, [r4, r3]
004c0158: bl       #0x4be3e0
004c015c: ldr      r3, [pc, #-0x644]
004c0160: ldr      r1, [pc, #-0x644]
004c0164: mov      r0, r5
004c0168: ldr      r2, [r4, r3]
004c016c: add      r1, pc, r1
004c0170: bl       #0x4bdccc
004c0174: ldr      r3, [pc, #-0x654]
004c0178: ldr      r1, [pc, #-0x654]
004c017c: mov      r0, r5
004c0180: ldr      r2, [r4, r3]
004c0184: add      r1, pc, r1
004c0188: bl       #0x4bdccc
004c018c: ldr      r3, [pc, #-0x664]
004c0190: mov      r0, r5
004c0194: mov      r1, r7
004c0198: ldr      r2, [r4, r3]
004c019c: ldr      r3, [pc, #-0x670]
004c01a0: ldr      r3, [r4, r3]
004c01a4: bl       #0x4be3e0
004c01a8: ldr      r3, [pc, #-0x678]
004c01ac: mov      r0, r5
004c01b0: mov      r1, r6
004c01b4: ldr      r2, [r4, r3]
004c01b8: ldr      r3, [pc, #-0x684]
004c01bc: ldr      r3, [r4, r3]
004c01c0: bl       #0x4be3e0
004c01c4: ldr      r3, [pc, #-0x68c]
004c01c8: ldr      r1, [pc, #-0x68c]
004c01cc: mov      r0, r5
004c01d0: ldr      r2, [r4, r3]
004c01d4: add      r1, pc, r1
004c01d8: bl       #0x4bdccc
004c01dc: ldr      r3, [pc, #-0x69c]
004c01e0: ldr      r1, [pc, #-0x69c]
004c01e4: mov      r0, r5
004c01e8: ldr      r2, [r4, r3]
004c01ec: add      r1, pc, r1
004c01f0: bl       #0x4bdccc
004c01f4: ldr      r3, [pc, #-0x6ac]
004c01f8: mov      r0, r5
004c01fc: mov      r1, r7
004c0200: ldr      r2, [r4, r3]
004c0204: ldr      r3, [pc, #-0x6b8]
004c0208: ldr      r3, [r4, r3]
004c020c: bl       #0x4be3e0
004c0210: ldr      r3, [pc, #-0x6c0]
004c0214: mov      r0, r5
004c0218: mov      r1, r6
004c021c: ldr      r2, [r4, r3]
004c0220: ldr      r3, [pc, #-0x6cc]
004c0224: ldr      r3, [r4, r3]
004c0228: bl       #0x4be3e0
004c022c: ldr      r3, [pc, #-0x6d4]
004c0230: ldr      r1, [pc, #-0x6d4]
004c0234: mov      r0, r5
004c0238: ldr      r2, [r4, r3]
004c023c: add      r1, pc, r1
004c0240: bl       #0x4bdccc
004c0244: ldr      r3, [pc, #-0x6e4]
004c0248: ldr      r1, [pc, #-0x6e4]
004c024c: mov      r0, r5
004c0250: ldr      r2, [r4, r3]
004c0254: add      r1, pc, r1
004c0258: bl       #0x4bdccc
004c025c: ldr      r3, [pc, #-0x6f4]
004c0260: mov      r0, r5
004c0264: mov      r1, r7
004c0268: ldr      r2, [r4, r3]
004c026c: ldr      r3, [pc, #-0x700]
004c0270: ldr      r3, [r4, r3]
004c0274: bl       #0x4be3e0
004c0278: ldr      r3, [pc, #-0x708]
004c027c: mov      r0, r5
004c0280: mov      r1, r6
004c0284: ldr      r2, [r4, r3]
004c0288: ldr      r3, [pc, #-0x714]
004c028c: ldr      r3, [r4, r3]
004c0290: bl       #0x4be3e0
004c0294: ldr      r3, [pc, #-0x71c]
004c0298: ldr      r1, [pc, #-0x71c]
004c029c: mov      r0, r5
004c02a0: ldr      r2, [r4, r3]
004c02a4: add      r1, pc, r1
004c02a8: bl       #0x4bdccc
004c02ac: ldr      r3, [pc, #-0x72c]
004c02b0: ldr      r1, [pc, #-0x72c]
004c02b4: mov      r0, r5
004c02b8: ldr      r2, [r4, r3]
004c02bc: add      r1, pc, r1
004c02c0: bl       #0x4bdccc
004c02c4: ldr      r3, [pc, #-0x73c]
004c02c8: mov      r0, r5
004c02cc: mov      r1, r7
004c02d0: ldr      r2, [r4, r3]
004c02d4: ldr      r3, [pc, #-0x748]
004c02d8: ldr      r3, [r4, r3]
004c02dc: bl       #0x4be3e0
004c02e0: ldr      r3, [pc, #-0x750]
004c02e4: mov      r0, r5
004c02e8: mov      r1, r6
004c02ec: ldr      r2, [r4, r3]
004c02f0: ldr      r3, [pc, #-0x75c]
004c02f4: ldr      r3, [r4, r3]
004c02f8: bl       #0x4be3e0
004c02fc: ldr      r3, [pc, #-0x764]
004c0300: ldr      r1, [pc, #-0x764]
004c0304: mov      r0, r5
004c0308: ldr      r2, [r4, r3]
004c030c: add      r1, pc, r1
004c0310: bl       #0x4bdccc
004c0314: ldr      r3, [pc, #-0x774]
004c0318: ldr      r1, [pc, #-0x774]
004c031c: mov      r0, r5
004c0320: ldr      r2, [r4, r3]
004c0324: add      r1, pc, r1
004c0328: bl       #0x4bdccc
004c032c: ldr      r3, [pc, #-0x784]
004c0330: mov      r0, r5
004c0334: mov      r1, r7
004c0338: ldr      r2, [r4, r3]
004c033c: ldr      r3, [pc, #-0x790]
004c0340: ldr      r3, [r4, r3]
004c0344: bl       #0x4be3e0
004c0348: ldr      r3, [pc, #-0x798]
004c034c: mov      r0, r5
004c0350: mov      r1, r6
004c0354: ldr      r2, [r4, r3]
004c0358: ldr      r3, [pc, #-0x7a4]
004c035c: ldr      r3, [r4, r3]
004c0360: bl       #0x4be3e0
004c0364: ldr      r3, [pc, #-0x7ac]
004c0368: ldr      r1, [pc, #-0x7ac]
004c036c: mov      r0, r5
004c0370: ldr      r2, [r4, r3]
004c0374: add      r1, pc, r1
004c0378: bl       #0x4bdccc
004c037c: ldr      r3, [pc, #-0x7bc]
004c0380: ldr      r1, [pc, #-0x7bc]
004c0384: mov      r0, r5
004c0388: ldr      r2, [r4, r3]
004c038c: add      r1, pc, r1
004c0390: bl       #0x4bdccc
004c0394: ldr      r3, [pc, #-0x7cc]
004c0398: mov      r0, r5
004c039c: mov      r1, r7
004c03a0: ldr      r2, [r4, r3]
004c03a4: ldr      r3, [pc, #-0x7d8]
004c03a8: ldr      r3, [r4, r3]
004c03ac: bl       #0x4be3e0
004c03b0: ldr      r3, [pc, #-0x7e0]
004c03b4: mov      r0, r5
004c03b8: mov      r1, r6
004c03bc: ldr      r2, [r4, r3]
004c03c0: ldr      r3, [pc, #-0x7ec]
004c03c4: ldr      r3, [r4, r3]
004c03c8: bl       #0x4be3e0
004c03cc: ldr      r3, [pc, #-0x7f4]
004c03d0: ldr      r1, [pc, #-0x7f4]
004c03d4: mov      r0, r5
004c03d8: ldr      r2, [r4, r3]
004c03dc: add      r1, pc, r1
004c03e0: bl       #0x4bdccc
004c03e4: ldr      r3, [pc, #-0x804]
004c03e8: ldr      r1, [pc, #-0x804]
004c03ec: mov      r0, r5
004c03f0: ldr      r2, [r4, r3]
004c03f4: add      r1, pc, r1
004c03f8: bl       #0x4bdccc
004c03fc: ldr      r3, [pc, #-0x814]
004c0400: mov      r1, r7
004c0404: mov      r0, r5
004c0408: ldr      r2, [r4, r3]
004c040c: ldr      r3, [pc, #-0x820]
004c0410: ldr      r7, [pc, #-0x820]
004c0414: ldr      r3, [r4, r3]
004c0418: bl       #0x4be3e0
004c041c: ldr      r3, [pc, #-0x828]
004c0420: mov      r1, r6
004c0424: mov      r0, r5
004c0428: ldr      r2, [r4, r3]
004c042c: ldr      r3, [pc, #-0x834]
004c0430: add      r7, pc, r7
004c0434: ldr      r6, [pc, #-0x838]
004c0438: ldr      r3, [r4, r3]
004c043c: bl       #0x4be3e0
004c0440: ldr      r3, [pc, #-0x840]
004c0444: ldr      r1, [pc, #-0x840]
004c0448: mov      r0, r5
004c044c: ldr      r2, [r4, r3]
004c0450: add      r1, pc, r1
004c0454: bl       #0x4bdccc
004c0458: ldr      r3, [pc, #-0x850]
004c045c: ldr      r1, [pc, #-0x850]
004c0460: mov      r0, r5
004c0464: ldr      r2, [r4, r3]
004c0468: add      r1, pc, r1
004c046c: bl       #0x4bdccc
004c0470: ldr      r3, [pc, #-0x860]
004c0474: ldr      r1, [pc, #-0x860]
004c0478: mov      r0, r5
004c047c: ldr      r2, [r4, r3]
004c0480: ldr      r3, [pc, #-0x868]
004c0484: add      r1, pc, r1
004c0488: add      r6, pc, r6
004c048c: ldr      r3, [r4, r3]
004c0490: bl       #0x4be3e0
004c0494: ldr      r3, [pc, #-0x878]
004c0498: ldr      r1, [pc, #-0x878]
004c049c: mov      r0, r5
004c04a0: ldr      r2, [r4, r3]
004c04a4: ldr      r3, [pc, #-0x880]
004c04a8: add      r1, pc, r1
004c04ac: ldr      r3, [r4, r3]
004c04b0: bl       #0x4be3e0
004c04b4: ldr      r3, [pc, #-0x88c]
004c04b8: ldr      r1, [pc, #-0x88c]
004c04bc: mov      r0, r5
004c04c0: ldr      r2, [r4, r3]
004c04c4: add      r1, pc, r1
004c04c8: bl       #0x4bdccc
004c04cc: ldr      r3, [pc, #-0x89c]
004c04d0: ldr      r1, [pc, #-0x89c]
004c04d4: mov      r0, r5
004c04d8: ldr      r2, [r4, r3]
004c04dc: add      r1, pc, r1
004c04e0: bl       #0x4bdccc
004c04e4: ldr      r3, [pc, #-0x8ac]
004c04e8: mov      r0, r5
004c04ec: mov      r1, r7
004c04f0: ldr      r2, [r4, r3]
004c04f4: ldr      r3, [pc, #-0x8b8]
004c04f8: ldr      r3, [r4, r3]
004c04fc: bl       #0x4be3e0
004c0500: ldr      r3, [pc, #-0x8c0]
004c0504: mov      r0, r5
004c0508: mov      r1, r6
004c050c: ldr      r2, [r4, r3]
004c0510: ldr      r3, [pc, #-0x8cc]
004c0514: ldr      r3, [r4, r3]
004c0518: bl       #0x4be3e0
004c051c: ldr      r3, [pc, #-0x8d4]
004c0520: ldr      r1, [pc, #-0x8d4]
004c0524: mov      r0, r5
004c0528: ldr      r2, [r4, r3]
004c052c: add      r1, pc, r1
004c0530: bl       #0x4bdccc
004c0534: ldr      r3, [pc, #-0x8e4]
004c0538: ldr      r1, [pc, #-0x8e4]
004c053c: mov      r0, r5
004c0540: ldr      r2, [r4, r3]
004c0544: add      r1, pc, r1
004c0548: bl       #0x4bdccc
004c054c: ldr      r3, [pc, #-0x8f4]
004c0550: mov      r1, r7
004c0554: mov      r0, r5
004c0558: ldr      r2, [r4, r3]
004c055c: ldr      r3, [pc, #-0x900]
004c0560: ldr      r7, [pc, #-0x900]
004c0564: ldr      r3, [r4, r3]
004c0568: bl       #0x4be3e0
004c056c: ldr      r3, [pc, #-0x908]
004c0570: mov      r1, r6
004c0574: mov      r0, r5
004c0578: ldr      r2, [r4, r3]
004c057c: ldr      r3, [pc, #-0x914]
004c0580: add      r7, pc, r7
004c0584: ldr      r6, [pc, #-0x918]
004c0588: ldr      r3, [r4, r3]
004c058c: bl       #0x4be3e0
004c0590: ldr      r3, [pc, #-0x920]
004c0594: ldr      r1, [pc, #-0x920]
004c0598: mov      r0, r5
004c059c: ldr      r2, [r4, r3]
004c05a0: add      r1, pc, r1
004c05a4: bl       #0x4bdccc
004c05a8: ldr      r3, [pc, #-0x930]
004c05ac: ldr      r1, [pc, #-0x930]
004c05b0: mov      r0, r5
004c05b4: ldr      r2, [r4, r3]
004c05b8: add      r1, pc, r1
004c05bc: bl       #0x4bdccc
004c05c0: ldr      r3, [pc, #-0x940]
004c05c4: mov      r0, r5
004c05c8: mov      r1, r7
004c05cc: ldr      r2, [r4, r3]
004c05d0: ldr      r3, [pc, #-0x94c]
004c05d4: add      r6, pc, r6
004c05d8: ldr      r3, [r4, r3]
004c05dc: bl       #0x4be3e0
004c05e0: ldr      r3, [pc, #-0x958]
004c05e4: mov      r0, r5
004c05e8: mov      r1, r6
004c05ec: ldr      r2, [r4, r3]
004c05f0: ldr      r3, [pc, #-0x964]
004c05f4: ldr      r3, [r4, r3]
004c05f8: bl       #0x4be3e0
004c05fc: ldr      r3, [pc, #-0x96c]
004c0600: ldr      r1, [pc, #-0x96c]
004c0604: mov      r0, r5
004c0608: ldr      r2, [r4, r3]
004c060c: add      r1, pc, r1
004c0610: bl       #0x4bdccc
004c0614: ldr      r3, [pc, #-0x97c]
004c0618: ldr      r1, [pc, #-0x97c]
004c061c: mov      r0, r5
004c0620: ldr      r2, [r4, r3]
004c0624: add      r1, pc, r1
004c0628: bl       #0x4bdccc
004c062c: ldr      r3, [pc, #-0x98c]
004c0630: mov      r0, r5
004c0634: mov      r1, r7
004c0638: ldr      r2, [r4, r3]
004c063c: ldr      r3, [pc, #-0x998]
004c0640: ldr      r3, [r4, r3]
004c0644: bl       #0x4be3e0
004c0648: ldr      r3, [pc, #-0x9a0]
004c064c: mov      r0, r5
004c0650: mov      r1, r6
004c0654: ldr      r2, [r4, r3]
004c0658: ldr      r3, [pc, #-0x9ac]
004c065c: ldr      r3, [r4, r3]
004c0660: bl       #0x4be3e0
004c0664: ldr      r3, [pc, #-0x9b4]
004c0668: ldr      r1, [pc, #-0x9b4]
004c066c: mov      r0, r5
004c0670: ldr      r2, [r4, r3]
004c0674: add      r1, pc, r1
004c0678: bl       #0x4bdccc
004c067c: ldr      r3, [pc, #-0x9c4]
004c0680: ldr      r1, [pc, #-0x9c4]
004c0684: mov      r0, r5
004c0688: ldr      r2, [r4, r3]
004c068c: add      r1, pc, r1
004c0690: bl       #0x4bdccc
004c0694: ldr      r3, [pc, #-0x9d4]
004c0698: mov      r0, r5
004c069c: mov      r1, r7
004c06a0: ldr      r2, [r4, r3]
004c06a4: ldr      r3, [pc, #-0x9e0]
004c06a8: ldr      r3, [r4, r3]
004c06ac: bl       #0x4be3e0
004c06b0: ldr      r3, [pc, #-0x9e8]
004c06b4: mov      r0, r5
004c06b8: mov      r1, r6
004c06bc: ldr      r2, [r4, r3]
004c06c0: ldr      r3, [pc, #-0x9f4]
004c06c4: ldr      r3, [r4, r3]
004c06c8: bl       #0x4be3e0
004c06cc: ldr      r3, [pc, #-0x9fc]
004c06d0: ldr      r1, [pc, #-0x9fc]
004c06d4: mov      r0, r5
004c06d8: ldr      r2, [r4, r3]
004c06dc: add      r1, pc, r1
004c06e0: bl       #0x4bdccc
004c06e4: ldr      r3, [pc, #-0xa0c]
004c06e8: ldr      r1, [pc, #-0xa0c]
004c06ec: mov      r0, r5
004c06f0: ldr      r2, [r4, r3]
004c06f4: add      r1, pc, r1
004c06f8: bl       #0x4bdccc
004c06fc: ldr      r3, [pc, #-0xa1c]
004c0700: mov      r0, r5
004c0704: mov      r1, r7
004c0708: ldr      r2, [r4, r3]
004c070c: ldr      r3, [pc, #-0xa28]
004c0710: ldr      r3, [r4, r3]
004c0714: bl       #0x4be3e0
004c0718: ldr      r3, [pc, #-0xa30]
004c071c: mov      r0, r5
004c0720: mov      r1, r6
004c0724: ldr      r2, [r4, r3]
004c0728: ldr      r3, [pc, #-0xa3c]
004c072c: ldr      r3, [r4, r3]
004c0730: bl       #0x4be3e0
004c0734: ldr      r3, [pc, #-0xa44]
004c0738: ldr      r1, [pc, #-0xa44]
004c073c: mov      r0, r5
004c0740: ldr      r2, [r4, r3]
004c0744: add      r1, pc, r1
004c0748: bl       #0x4bdccc
004c074c: ldr      r3, [pc, #-0xa54]
004c0750: ldr      r1, [pc, #-0xa54]
004c0754: mov      r0, r5
004c0758: ldr      r2, [r4, r3]
004c075c: add      r1, pc, r1
004c0760: bl       #0x4bdccc
004c0764: ldr      r3, [pc, #-0xa64]
004c0768: mov      r1, r7
004c076c: mov      r0, r5
004c0770: ldr      r2, [r4, r3]
004c0774: ldr      r3, [pc, #-0xa70]
004c0778: ldr      r7, [pc, #-0xa70]
004c077c: ldr      r3, [r4, r3]
004c0780: bl       #0x4be3e0
004c0784: ldr      r3, [pc, #-0xa78]
004c0788: mov      r1, r6
004c078c: mov      r0, r5
004c0790: ldr      r2, [r4, r3]
004c0794: ldr      r3, [pc, #-0xa84]
004c0798: add      r7, pc, r7
004c079c: ldr      r6, [pc, #-0xa88]
004c07a0: ldr      r3, [r4, r3]
004c07a4: bl       #0x4be3e0
004c07a8: ldr      r3, [pc, #-0xa90]
004c07ac: ldr      r1, [pc, #-0xa90]
004c07b0: mov      r0, r5
004c07b4: ldr      r2, [r4, r3]
004c07b8: add      r1, pc, r1
004c07bc: bl       #0x4bdccc
004c07c0: ldr      r3, [pc, #-0xaa0]
004c07c4: ldr      r1, [pc, #-0xaa0]
004c07c8: mov      r0, r5
004c07cc: ldr      r2, [r4, r3]
004c07d0: add      r1, pc, r1
004c07d4: bl       #0x4bdccc
004c07d8: ldr      r3, [pc, #-0xab0]
004c07dc: ldr      r1, [pc, #-0xab0]
004c07e0: mov      r0, r5
004c07e4: ldr      r2, [r4, r3]
004c07e8: ldr      r3, [pc, #-0xab8]
004c07ec: add      r1, pc, r1
004c07f0: add      r6, pc, r6
004c07f4: ldr      r3, [r4, r3]
004c07f8: bl       #0x4be3e0
004c07fc: ldr      r3, [pc, #-0xac8]
004c0800: ldr      r1, [pc, #-0xac8]
004c0804: mov      r0, r5
004c0808: ldr      r2, [r4, r3]
004c080c: ldr      r3, [pc, #-0xad0]
004c0810: add      r1, pc, r1
004c0814: ldr      r3, [r4, r3]
004c0818: bl       #0x4be3e0
004c081c: ldr      r3, [pc, #-0xadc]
004c0820: ldr      r1, [pc, #-0xadc]
004c0824: mov      r0, r5
004c0828: ldr      r2, [r4, r3]
004c082c: add      r1, pc, r1
004c0830: bl       #0x4bdccc
004c0834: ldr      r3, [pc, #-0xaec]
004c0838: ldr      r1, [pc, #-0xaec]
004c083c: mov      r0, r5
004c0840: ldr      r2, [r4, r3]
004c0844: add      r1, pc, r1
004c0848: bl       #0x4bdccc
004c084c: ldr      r3, [pc, #-0xafc]
004c0850: ldr      r1, [pc, #-0xafc]
004c0854: mov      r0, r5
004c0858: ldr      r2, [r4, r3]
004c085c: ldr      r3, [pc, #-0xb04]
004c0860: add      r1, pc, r1
004c0864: ldr      r3, [r4, r3]
004c0868: bl       #0x4be3e0
004c086c: ldr      r3, [pc, #-0xb10]
004c0870: ldr      r1, [pc, #-0xb10]
004c0874: mov      r0, r5
004c0878: ldr      r2, [r4, r3]
004c087c: ldr      r3, [pc, #-0xb18]
004c0880: add      r1, pc, r1
004c0884: ldr      r3, [r4, r3]
004c0888: bl       #0x4be3e0
004c088c: ldr      r3, [pc, #-0xb24]
004c0890: ldr      r1, [pc, #-0xb24]
004c0894: mov      r0, r5
004c0898: ldr      r2, [r4, r3]
004c089c: add      r1, pc, r1
004c08a0: bl       #0x4bdccc
004c08a4: ldr      r3, [pc, #-0xb34]
004c08a8: ldr      r1, [pc, #-0xb34]
004c08ac: mov      r0, r5
004c08b0: ldr      r2, [r4, r3]
004c08b4: add      r1, pc, r1
004c08b8: bl       #0x4bdccc
004c08bc: ldr      r3, [pc, #-0xb44]
004c08c0: ldr      r1, [pc, #-0xb44]
004c08c4: mov      r0, r5
004c08c8: ldr      r2, [r4, r3]
004c08cc: ldr      r3, [pc, #-0xb4c]
004c08d0: add      r1, pc, r1
004c08d4: ldr      r3, [r4, r3]
004c08d8: bl       #0x4be3e0
004c08dc: ldr      r3, [pc, #-0xb58]
004c08e0: ldr      r1, [pc, #-0xb58]
004c08e4: mov      r0, r5
004c08e8: ldr      r2, [r4, r3]
004c08ec: ldr      r3, [pc, #-0xb60]
004c08f0: add      r1, pc, r1
004c08f4: ldr      r3, [r4, r3]
004c08f8: bl       #0x4be3e0
004c08fc: ldr      r3, [pc, #-0xb6c]
004c0900: ldr      r1, [pc, #-0xb6c]
004c0904: mov      r0, r5
004c0908: ldr      r2, [r4, r3]
004c090c: add      r1, pc, r1
004c0910: bl       #0x4bdccc
004c0914: ldr      r3, [pc, #-0xb7c]
004c0918: ldr      r1, [pc, #-0xb7c]
004c091c: mov      r0, r5
004c0920: ldr      r2, [r4, r3]
004c0924: add      r1, pc, r1
004c0928: bl       #0x4bdccc
004c092c: ldr      r3, [pc, #-0xb8c]
004c0930: ldr      r1, [pc, #-0xb8c]
004c0934: mov      r0, r5
004c0938: ldr      r2, [r4, r3]
004c093c: ldr      r3, [pc, #-0xb94]
004c0940: add      r1, pc, r1
004c0944: ldr      r3, [r4, r3]
004c0948: bl       #0x4be3e0
004c094c: ldr      r3, [pc, #-0xba0]
004c0950: ldr      r1, [pc, #-0xba0]
004c0954: mov      r0, r5
004c0958: ldr      r2, [r4, r3]
004c095c: ldr      r3, [pc, #-0xba8]
004c0960: add      r1, pc, r1
004c0964: ldr      r3, [r4, r3]
004c0968: bl       #0x4be3e0
004c096c: ldr      r3, [pc, #-0xbb4]
004c0970: ldr      r1, [pc, #-0xbb4]
004c0974: mov      r0, r5
004c0978: ldr      r2, [r4, r3]
004c097c: add      r1, pc, r1
004c0980: bl       #0x4bdccc
004c0984: ldr      r3, [pc, #-0xbc4]
004c0988: ldr      r1, [pc, #-0xbc4]
004c098c: mov      r0, r5
004c0990: ldr      r2, [r4, r3]
004c0994: add      r1, pc, r1
004c0998: bl       #0x4bdccc
004c099c: ldr      r3, [pc, #-0xbd4]
004c09a0: ldr      r1, [pc, #-0xbd4]
004c09a4: mov      r0, r5
004c09a8: ldr      r2, [r4, r3]
004c09ac: ldr      r3, [pc, #-0xbdc]
004c09b0: add      r1, pc, r1
004c09b4: ldr      r3, [r4, r3]
004c09b8: bl       #0x4be3e0
004c09bc: ldr      r3, [pc, #-0xbe8]
004c09c0: ldr      r1, [pc, #-0xbe8]
004c09c4: mov      r0, r5
004c09c8: ldr      r2, [r4, r3]
004c09cc: ldr      r3, [pc, #-0xbf0]
004c09d0: add      r1, pc, r1
004c09d4: ldr      r3, [r4, r3]
004c09d8: bl       #0x4be3e0
004c09dc: ldr      r3, [pc, #-0xbfc]
004c09e0: ldr      r1, [pc, #-0xbfc]
004c09e4: mov      r0, r5
004c09e8: ldr      r2, [r4, r3]
004c09ec: add      r1, pc, r1
004c09f0: bl       #0x4bdccc
004c09f4: ldr      r3, [pc, #-0xc0c]
004c09f8: ldr      r1, [pc, #-0xc0c]
004c09fc: mov      r0, r5
004c0a00: ldr      r2, [r4, r3]
004c0a04: add      r1, pc, r1
004c0a08: bl       #0x4bdccc
004c0a0c: ldr      r3, [pc, #-0xc1c]
004c0a10: mov      r0, r5
004c0a14: mov      r1, r7
004c0a18: ldr      r2, [r4, r3]
004c0a1c: ldr      r3, [pc, #-0xc28]
004c0a20: ldr      r3, [r4, r3]
004c0a24: bl       #0x4be3e0
004c0a28: ldr      r3, [pc, #-0xc30]
004c0a2c: mov      r0, r5
004c0a30: mov      r1, r6
004c0a34: ldr      r2, [r4, r3]
004c0a38: ldr      r3, [pc, #-0xc3c]
004c0a3c: ldr      r3, [r4, r3]
004c0a40: bl       #0x4be3e0
004c0a44: ldr      r3, [pc, #-0xc44]
004c0a48: ldr      r1, [pc, #-0xc44]
004c0a4c: mov      r0, r5
004c0a50: ldr      r2, [r4, r3]
004c0a54: add      r1, pc, r1
004c0a58: bl       #0x4bdccc
004c0a5c: ldr      r3, [pc, #-0xc54]
004c0a60: ldr      r1, [pc, #-0xc54]
004c0a64: mov      r0, r5
004c0a68: ldr      r2, [r4, r3]
004c0a6c: add      r1, pc, r1
004c0a70: bl       #0x4bdccc
004c0a74: ldr      r3, [pc, #-0xc64]
004c0a78: mov      r1, r7
004c0a7c: mov      r0, r5
004c0a80: ldr      r2, [r4, r3]
004c0a84: ldr      r3, [pc, #-0xc70]
004c0a88: ldr      r3, [r4, r3]
004c0a8c: bl       #0x4be3e0
004c0a90: ldr      r3, [pc, #-0xc78]
004c0a94: mov      r1, r6
004c0a98: mov      r0, r5
004c0a9c: ldr      r2, [r4, r3]
004c0aa0: ldr      r3, [pc, #-0xc84]
004c0aa4: ldr      r6, [pc, #-0xc84]
004c0aa8: ldr      r3, [r4, r3]
004c0aac: bl       #0x4be3e0
004c0ab0: ldr      r3, [pc, #-0xc8c]
004c0ab4: ldr      r1, [pc, #-0xc8c]
004c0ab8: mov      r0, r5
004c0abc: ldr      r2, [r4, r3]
004c0ac0: add      r1, pc, r1
004c0ac4: bl       #0x4bdccc
004c0ac8: ldr      r3, [pc, #-0xc9c]
004c0acc: ldr      r1, [pc, #-0xc9c]
004c0ad0: mov      r0, r5
004c0ad4: ldr      r2, [r4, r3]
004c0ad8: add      r1, pc, r1
004c0adc: bl       #0x4bdccc
004c0ae0: ldr      r3, [pc, #-0xcac]
004c0ae4: ldr      r1, [pc, #-0xcac]
004c0ae8: mov      r0, r5
004c0aec: ldr      r2, [r4, r3]
004c0af0: ldr      r3, [pc, #-0xcb4]
004c0af4: add      r1, pc, r1
004c0af8: add      r6, pc, r6
004c0afc: ldr      r3, [r4, r3]
004c0b00: bl       #0x4be3e0
004c0b04: ldr      r3, [pc, #-0xcc4]
004c0b08: ldr      r1, [pc, #-0xcc4]
004c0b0c: mov      r0, r5
004c0b10: ldr      r2, [r4, r3]
004c0b14: ldr      r3, [pc, #-0xccc]
004c0b18: add      r1, pc, r1
004c0b1c: ldr      r3, [r4, r3]
004c0b20: bl       #0x4be3e0
004c0b24: ldr      r3, [pc, #-0xcd8]
004c0b28: mov      r0, r5
004c0b2c: mov      r1, r6
004c0b30: ldr      r7, [r4, r3]
004c0b34: mov      r2, r7
004c0b38: bl       #0x4bdccc
004c0b3c: ldr      r3, [pc, #-0xcec]
004c0b40: ldr      r1, [pc, #-0xcec]
004c0b44: mov      r0, r5
004c0b48: ldr      r2, [r4, r3]
004c0b4c: add      r1, pc, r1
004c0b50: bl       #0x4bdccc
004c0b54: ldr      r3, [pc, #-0xcfc]
004c0b58: ldr      r1, [pc, #-0xcfc]
004c0b5c: mov      r0, r5
004c0b60: ldr      r2, [r4, r3]
004c0b64: ldr      r3, [pc, #-0xd04]
004c0b68: add      r1, pc, r1
004c0b6c: ldr      r3, [r4, r3]
004c0b70: bl       #0x4be3e0
004c0b74: ldr      r3, [pc, #-0xd10]
004c0b78: ldr      r1, [pc, #-0xd10]
004c0b7c: mov      r0, r5
004c0b80: ldr      r2, [r4, r3]
004c0b84: ldr      r3, [pc, #-0xd18]
004c0b88: add      r1, pc, r1
004c0b8c: ldr      r3, [r4, r3]
004c0b90: bl       #0x4be3e0
004c0b94: mov      r0, r5
004c0b98: mov      r1, r6
004c0b9c: mov      r2, r7
004c0ba0: bl       #0x4bdccc
004c0ba4: ldr      r3, [pc, #-0xd34]
004c0ba8: ldr      r1, [pc, #-0xd34]
004c0bac: mov      r0, r5
004c0bb0: ldr      r2, [r4, r3]
004c0bb4: add      r1, pc, r1
004c0bb8: bl       #0x4bdccc
004c0bbc: ldr      r3, [pc, #-0xd44]
004c0bc0: ldr      r1, [pc, #-0xd44]
004c0bc4: mov      r0, r5
004c0bc8: ldr      r2, [r4, r3]
004c0bcc: ldr      r3, [pc, #-0xd4c]
004c0bd0: add      r1, pc, r1
004c0bd4: ldr      r3, [r4, r3]
004c0bd8: bl       #0x4be3e0
004c0bdc: ldr      r3, [pc, #-0xd58]
004c0be0: ldr      r1, [pc, #-0xd58]
004c0be4: mov      r0, r5
004c0be8: ldr      r2, [r4, r3]
004c0bec: ldr      r3, [pc, #-0xd60]
004c0bf0: add      r1, pc, r1
004c0bf4: ldr      r3, [r4, r3]
004c0bf8: bl       #0x4be3e0
004c0bfc: mov      r0, r5
004c0c00: mov      r1, r6
004c0c04: mov      r2, r7
004c0c08: bl       #0x4bdccc
004c0c0c: ldr      r3, [pc, #-0xd7c]
004c0c10: ldr      r1, [pc, #-0xd7c]
004c0c14: mov      r0, r5
004c0c18: ldr      r2, [r4, r3]
004c0c1c: add      r1, pc, r1
004c0c20: bl       #0x4bdccc
004c0c24: ldr      r3, [pc, #-0xd8c]
004c0c28: ldr      r1, [pc, #-0xd8c]
004c0c2c: mov      r0, r5
004c0c30: ldr      r2, [r4, r3]
004c0c34: ldr      r3, [pc, #-0xd94]
004c0c38: add      r1, pc, r1
004c0c3c: ldr      r3, [r4, r3]
004c0c40: bl       #0x4be3e0
004c0c44: ldr      r3, [pc, #-0xda0]
004c0c48: ldr      r1, [pc, #-0xda0]
004c0c4c: mov      r0, r5
004c0c50: ldr      r2, [r4, r3]
004c0c54: ldr      r3, [pc, #-0xda8]
004c0c58: add      r1, pc, r1
004c0c5c: ldr      r3, [r4, r3]
004c0c60: bl       #0x4be3e0
004c0c64: mov      r0, r5
004c0c68: mov      r1, r6
004c0c6c: mov      r2, r7
004c0c70: bl       #0x4bdccc
004c0c74: ldr      r3, [pc, #-0xdc4]
004c0c78: ldr      r1, [pc, #-0xdc4]
004c0c7c: mov      r0, r5
004c0c80: ldr      r2, [r4, r3]
004c0c84: add      r1, pc, r1
004c0c88: bl       #0x4bdccc
004c0c8c: ldr      r3, [pc, #-0xdd4]
004c0c90: ldr      r1, [pc, #-0xdd4]
004c0c94: mov      r0, r5
004c0c98: ldr      r2, [r4, r3]
004c0c9c: ldr      r3, [pc, #-0xddc]
004c0ca0: add      r1, pc, r1
004c0ca4: ldr      r3, [r4, r3]
004c0ca8: bl       #0x4be3e0
004c0cac: ldr      r3, [pc, #-0xde8]
004c0cb0: ldr      r1, [pc, #-0xde8]
004c0cb4: mov      r0, r5
004c0cb8: ldr      r2, [r4, r3]
004c0cbc: ldr      r3, [pc, #-0xdf0]
004c0cc0: add      r1, pc, r1
004c0cc4: ldr      r3, [r4, r3]
004c0cc8: bl       #0x4be3e0
004c0ccc: mov      r0, r5
004c0cd0: mov      r1, r6
004c0cd4: mov      r2, r7
004c0cd8: bl       #0x4bdccc
004c0cdc: ldr      r3, [pc, #-0xe0c]
004c0ce0: ldr      r1, [pc, #-0xe0c]
004c0ce4: mov      r0, r5
004c0ce8: ldr      r2, [r4, r3]
004c0cec: add      r1, pc, r1
004c0cf0: bl       #0x4bdccc
004c0cf4: ldr      r3, [pc, #-0xe1c]
004c0cf8: ldr      r1, [pc, #-0xe1c]
004c0cfc: mov      r0, r5
004c0d00: ldr      r2, [r4, r3]
004c0d04: ldr      r3, [pc, #-0xe24]
004c0d08: add      r1, pc, r1
004c0d0c: ldr      r3, [r4, r3]
004c0d10: bl       #0x4be3e0
004c0d14: ldr      r3, [pc, #-0xe30]
004c0d18: ldr      r1, [pc, #-0xe30]
004c0d1c: mov      r0, r5
004c0d20: ldr      r2, [r4, r3]
004c0d24: ldr      r3, [pc, #-0xe38]
004c0d28: add      r1, pc, r1
004c0d2c: ldr      r3, [r4, r3]
004c0d30: bl       #0x4be3e0
004c0d34: ldr      r3, [pc, #-0xe44]
004c0d38: ldr      r1, [pc, #-0xe44]
004c0d3c: mov      r0, r5
004c0d40: ldr      r2, [r4, r3]
004c0d44: add      r1, pc, r1
004c0d48: bl       #0x4bdccc
004c0d4c: ldr      r3, [pc, #-0xe54]
004c0d50: ldr      r1, [pc, #-0xe54]
004c0d54: mov      r0, r5
004c0d58: ldr      r2, [r4, r3]
004c0d5c: add      r1, pc, r1
004c0d60: bl       #0x4bdccc
004c0d64: ldr      r3, [pc, #-0xe64]
004c0d68: ldr      r1, [pc, #-0xe64]
004c0d6c: mov      r0, r5
004c0d70: ldr      r2, [r4, r3]
004c0d74: ldr      r3, [pc, #-0xe6c]
004c0d78: add      r1, pc, r1
004c0d7c: ldr      r3, [r4, r3]
004c0d80: bl       #0x4be3e0
004c0d84: ldr      r3, [pc, #-0xe78]
004c0d88: ldr      r1, [pc, #-0xe78]
004c0d8c: mov      r0, r5
004c0d90: ldr      r2, [r4, r3]
004c0d94: ldr      r3, [pc, #-0xe80]
004c0d98: add      r1, pc, r1
004c0d9c: ldr      r3, [r4, r3]
004c0da0: bl       #0x4be3e0
004c0da4: ldr      r3, [pc, #-0xe8c]
004c0da8: ldr      r1, [pc, #-0xe8c]
004c0dac: mov      r0, r5
004c0db0: ldr      r2, [r4, r3]
004c0db4: add      r1, pc, r1
004c0db8: bl       #0x4bdccc
004c0dbc: ldr      r3, [pc, #-0xe9c]
004c0dc0: ldr      r1, [pc, #-0xe9c]
004c0dc4: mov      r0, r5
004c0dc8: ldr      r2, [r4, r3]
004c0dcc: add      r1, pc, r1
004c0dd0: bl       #0x4bdccc
004c0dd4: mov      r0, r5
004c0dd8: pop      {r4, r5, r6, r7, r8, pc}
