# _ZN7gameswf6player11action_initEv
0076e104: push     {r4, r5, r6, r7, r8, sl, lr}
0076e108: ldr      r5, [pc, #0xd74]
0076e10c: ldr      r7, [pc, #0xd74]
0076e110: sub      sp, sp, #0x3bc
0076e114: add      r5, pc, r5
0076e118: ldr      r3, [r5, r7]
0076e11c: mov      r4, r0
0076e120: ldr      r3, [r3]
0076e124: str      r3, [sp, #0x3b4]
0076e128: bl       #0x7b794c
0076e12c: ldr      r3, [r4, #0x34]
0076e130: ldr      r2, [r4, #0x30]
0076e134: strd     r0, r1, [r4, #0x40]
0076e138: str      r2, [r3, #0x34]
0076e13c: ldr      r0, [r4, #0x34]
0076e140: ldr      r3, [r0, #0xc]
0076e144: add      r0, r0, #0xc
0076e148: cmp      r3, #0
0076e14c: moveq    r1, #0x30
0076e150: beq      #0x76e168
0076e154: ldr      r1, [r3]
0076e158: cmp      r1, #0x20
0076e15c: movlt    r1, #0x20
0076e160: add      r1, r1, r1, lsl #1
0076e164: asr      r1, r1, #1
0076e168: bl       #0x76a2f8
0076e16c: ldr      r1, [pc, #0xd18]
0076e170: add      r8, sp, #0x3a0
0076e174: mov      r0, r8
0076e178: add      r1, pc, r1
0076e17c: ldr      sl, [r4, #0x34]
0076e180: bl       #0x413a7c
0076e184: ldr      r2, [pc, #0xd04]
0076e188: add      r6, sp, #0x150
0076e18c: mov      r3, #0
0076e190: ldr      r1, [r5, r2]
0076e194: mov      r0, r6
0076e198: strb     r3, [sp, #0x151]
0076e19c: strb     r3, [sp, #0x150]
0076e1a0: bl       #0x7972a0
0076e1a4: mov      r2, r6
0076e1a8: mov      r0, sl
0076e1ac: mov      r1, r8
0076e1b0: bl       #0x768b54
0076e1b4: mov      r0, r6
0076e1b8: bl       #0x797124
0076e1bc: ldrb     r2, [sp, #0x3a0]
0076e1c0: sxtb     r3, r2
0076e1c4: cmn      r3, #1
0076e1c8: beq      #0x76ec98
0076e1cc: ldr      r1, [pc, #0xcc0]
0076e1d0: add      r8, sp, #0x38c
0076e1d4: mov      r0, r8
0076e1d8: add      r1, pc, r1
0076e1dc: ldr      sl, [r4, #0x34]
0076e1e0: bl       #0x413a7c
0076e1e4: ldr      r2, [pc, #0xcac]
0076e1e8: add      r6, sp, #0x144
0076e1ec: mov      r3, #0
0076e1f0: ldr      r1, [r5, r2]
0076e1f4: mov      r0, r6
0076e1f8: strb     r3, [sp, #0x145]
0076e1fc: strb     r3, [sp, #0x144]
0076e200: bl       #0x7972a0
0076e204: mov      r2, r6
0076e208: mov      r0, sl
0076e20c: mov      r1, r8
0076e210: bl       #0x768b54
0076e214: mov      r0, r6
0076e218: bl       #0x797124
0076e21c: ldrb     r2, [sp, #0x38c]
0076e220: sxtb     r3, r2
0076e224: cmn      r3, #1
0076e228: beq      #0x76eca8
0076e22c: ldr      r1, [pc, #0xc68]
0076e230: add      r8, sp, #0x378
0076e234: mov      r0, r8
0076e238: add      r1, pc, r1
0076e23c: ldr      sl, [r4, #0x34]
0076e240: bl       #0x413a7c
0076e244: ldr      r2, [pc, #0xc54]
0076e248: add      r6, sp, #0x138
0076e24c: mov      r3, #0
0076e250: ldr      r1, [r5, r2]
0076e254: mov      r0, r6
0076e258: strb     r3, [sp, #0x139]
0076e25c: strb     r3, [sp, #0x138]
0076e260: bl       #0x7972a0
0076e264: mov      r2, r6
0076e268: mov      r0, sl
0076e26c: mov      r1, r8
0076e270: bl       #0x768b54
0076e274: mov      r0, r6
0076e278: bl       #0x797124
0076e27c: ldrb     r2, [sp, #0x378]
0076e280: sxtb     r3, r2
0076e284: cmn      r3, #1
0076e288: beq      #0x76ecb8
0076e28c: ldr      r1, [pc, #0xc10]
0076e290: add      r8, sp, #0x364
0076e294: mov      r0, r8
0076e298: add      r1, pc, r1
0076e29c: ldr      sl, [r4, #0x34]
0076e2a0: bl       #0x413a7c
0076e2a4: mov      r0, r4
0076e2a8: bl       #0x798e44
0076e2ac: mov      r2, #0
0076e2b0: strb     r2, [sp, #0x12c]
0076e2b4: cmp      r0, #0
0076e2b8: mov      r2, #5
0076e2bc: strb     r2, [sp, #0x12d]
0076e2c0: str      r0, [sp, #0x130]
0076e2c4: beq      #0x76e2cc
0076e2c8: bl       #0x759c64
0076e2cc: add      r6, sp, #0x12c
0076e2d0: mov      r2, r6
0076e2d4: mov      r1, r8
0076e2d8: mov      r0, sl
0076e2dc: bl       #0x768b54
0076e2e0: mov      r0, r6
0076e2e4: bl       #0x797124
0076e2e8: ldrb     r2, [sp, #0x364]
0076e2ec: sxtb     r3, r2
0076e2f0: cmn      r3, #1
0076e2f4: beq      #0x76ecc8
0076e2f8: ldr      r1, [pc, #0xba8]
0076e2fc: add      r8, sp, #0x350
0076e300: mov      r0, r8
0076e304: add      r1, pc, r1
0076e308: ldr      sl, [r4, #0x34]
0076e30c: bl       #0x413a7c
0076e310: ldr      r2, [pc, #0xb94]
0076e314: add      r6, sp, #0x120
0076e318: mov      r3, #0
0076e31c: ldr      r1, [r5, r2]
0076e320: mov      r0, r6
0076e324: strb     r3, [sp, #0x121]
0076e328: strb     r3, [sp, #0x120]
0076e32c: bl       #0x7972a0
0076e330: mov      r2, r6
0076e334: mov      r0, sl
0076e338: mov      r1, r8
0076e33c: bl       #0x768b54
0076e340: mov      r0, r6
0076e344: bl       #0x797124
0076e348: ldrb     r2, [sp, #0x350]
0076e34c: sxtb     r3, r2
0076e350: cmn      r3, #1
0076e354: beq      #0x76ecd8
0076e358: ldr      r1, [pc, #0xb50]
0076e35c: add      r8, sp, #0x33c
0076e360: mov      r0, r8
0076e364: add      r1, pc, r1
0076e368: ldr      sl, [r4, #0x34]
0076e36c: bl       #0x413a7c
0076e370: ldr      r2, [pc, #0xb3c]
0076e374: add      r6, sp, #0x114
0076e378: mov      r3, #0
0076e37c: ldr      r1, [r5, r2]
0076e380: mov      r0, r6
0076e384: strb     r3, [sp, #0x115]
0076e388: strb     r3, [sp, #0x114]
0076e38c: bl       #0x7972a0
0076e390: mov      r2, r6
0076e394: mov      r0, sl
0076e398: mov      r1, r8
0076e39c: bl       #0x768b54
0076e3a0: mov      r0, r6
0076e3a4: bl       #0x797124
0076e3a8: ldrb     r2, [sp, #0x33c]
0076e3ac: sxtb     r3, r2
0076e3b0: cmn      r3, #1
0076e3b4: beq      #0x76ece8
0076e3b8: ldr      r1, [pc, #0xaf8]
0076e3bc: add      r8, sp, #0x328
0076e3c0: mov      r0, r8
0076e3c4: add      r1, pc, r1
0076e3c8: ldr      sl, [r4, #0x34]
0076e3cc: bl       #0x413a7c
0076e3d0: ldr      r2, [pc, #0xae4]
0076e3d4: add      r6, sp, #0x108
0076e3d8: mov      r3, #0
0076e3dc: ldr      r1, [r5, r2]
0076e3e0: mov      r0, r6
0076e3e4: strb     r3, [sp, #0x109]
0076e3e8: strb     r3, [sp, #0x108]
0076e3ec: bl       #0x7972a0
0076e3f0: mov      r2, r6
0076e3f4: mov      r0, sl
0076e3f8: mov      r1, r8
0076e3fc: bl       #0x768b54
0076e400: mov      r0, r6
0076e404: bl       #0x797124
0076e408: ldrb     r2, [sp, #0x328]
0076e40c: sxtb     r3, r2
0076e410: cmn      r3, #1
0076e414: beq      #0x76ecf8
0076e418: ldr      r1, [pc, #0xaa0]
0076e41c: add      r8, sp, #0x314
0076e420: mov      r0, r8
0076e424: add      r1, pc, r1
0076e428: ldr      sl, [r4, #0x34]
0076e42c: bl       #0x413a7c
0076e430: ldr      r2, [pc, #0xa8c]
0076e434: add      r6, sp, #0xfc
0076e438: mov      r3, #0
0076e43c: ldr      r1, [r5, r2]
0076e440: mov      r0, r6
0076e444: strb     r3, [sp, #0xfd]
0076e448: strb     r3, [sp, #0xfc]
0076e44c: bl       #0x7972a0
0076e450: mov      r2, r6
0076e454: mov      r0, sl
0076e458: mov      r1, r8
0076e45c: bl       #0x768b54
0076e460: mov      r0, r6
0076e464: bl       #0x797124
0076e468: ldrb     r2, [sp, #0x314]
0076e46c: sxtb     r3, r2
0076e470: cmn      r3, #1
0076e474: beq      #0x76ed08
0076e478: ldr      r1, [pc, #0xa48]
0076e47c: add      r8, sp, #0x300
0076e480: mov      r0, r8
0076e484: add      r1, pc, r1
0076e488: ldr      sl, [r4, #0x34]
0076e48c: bl       #0x413a7c
0076e490: ldr      r2, [pc, #0xa34]
0076e494: add      r6, sp, #0xf0
0076e498: mov      r3, #0
0076e49c: ldr      r1, [r5, r2]
0076e4a0: mov      r0, r6
0076e4a4: strb     r3, [sp, #0xf1]
0076e4a8: strb     r3, [sp, #0xf0]
0076e4ac: bl       #0x7972a0
0076e4b0: mov      r2, r6
0076e4b4: mov      r0, sl
0076e4b8: mov      r1, r8
0076e4bc: bl       #0x768b54
0076e4c0: mov      r0, r6
0076e4c4: bl       #0x797124
0076e4c8: ldrb     r2, [sp, #0x300]
0076e4cc: sxtb     r3, r2
0076e4d0: cmn      r3, #1
0076e4d4: beq      #0x76ed18
0076e4d8: ldr      r1, [pc, #0x9f0]
0076e4dc: add      r8, sp, #0x2ec
0076e4e0: mov      r0, r8
0076e4e4: add      r1, pc, r1
0076e4e8: ldr      sl, [r4, #0x34]
0076e4ec: bl       #0x413a7c
0076e4f0: ldr      r2, [pc, #0x9dc]
0076e4f4: add      r6, sp, #0xe4
0076e4f8: mov      r3, #0
0076e4fc: ldr      r1, [r5, r2]
0076e500: mov      r0, r6
0076e504: strb     r3, [sp, #0xe5]
0076e508: strb     r3, [sp, #0xe4]
0076e50c: bl       #0x7972a0
0076e510: mov      r2, r6
0076e514: mov      r0, sl
0076e518: mov      r1, r8
0076e51c: bl       #0x768b54
0076e520: mov      r0, r6
0076e524: bl       #0x797124
0076e528: ldrb     r2, [sp, #0x2ec]
0076e52c: sxtb     r3, r2
0076e530: cmn      r3, #1
0076e534: beq      #0x76ed28
0076e538: ldr      r1, [pc, #0x998]
0076e53c: add      r8, sp, #0x2d8
0076e540: mov      r0, r8
0076e544: add      r1, pc, r1
0076e548: ldr      sl, [r4, #0x34]
0076e54c: bl       #0x413a7c
0076e550: ldr      r2, [pc, #0x984]
0076e554: add      r6, sp, #0xd8
0076e558: mov      r3, #0
0076e55c: ldr      r1, [r5, r2]
0076e560: mov      r0, r6
0076e564: strb     r3, [sp, #0xd9]
0076e568: strb     r3, [sp, #0xd8]
0076e56c: bl       #0x7972a0
0076e570: mov      r2, r6
0076e574: mov      r0, sl
0076e578: mov      r1, r8
0076e57c: bl       #0x768b54
0076e580: mov      r0, r6
0076e584: bl       #0x797124
0076e588: ldrb     r2, [sp, #0x2d8]
0076e58c: sxtb     r3, r2
0076e590: cmn      r3, #1
0076e594: beq      #0x76ed38
0076e598: ldr      r1, [pc, #0x940]
0076e59c: add      r8, sp, #0x2c4
0076e5a0: mov      r0, r8
0076e5a4: add      r1, pc, r1
0076e5a8: ldr      sl, [r4, #0x34]
0076e5ac: bl       #0x413a7c
0076e5b0: ldr      r2, [pc, #0x92c]
0076e5b4: add      r6, sp, #0xcc
0076e5b8: mov      r3, #0
0076e5bc: ldr      r1, [r5, r2]
0076e5c0: mov      r0, r6
0076e5c4: strb     r3, [sp, #0xcd]
0076e5c8: strb     r3, [sp, #0xcc]
0076e5cc: bl       #0x7972a0
0076e5d0: mov      r2, r6
0076e5d4: mov      r0, sl
0076e5d8: mov      r1, r8
0076e5dc: bl       #0x768b54
0076e5e0: mov      r0, r6
0076e5e4: bl       #0x797124
0076e5e8: ldrb     r2, [sp, #0x2c4]
0076e5ec: sxtb     r3, r2
0076e5f0: cmn      r3, #1
0076e5f4: beq      #0x76ed48
0076e5f8: ldr      r1, [pc, #0x8e8]
0076e5fc: add      r8, sp, #0x2b0
0076e600: mov      r0, r8
0076e604: add      r1, pc, r1
0076e608: ldr      sl, [r4, #0x34]
0076e60c: bl       #0x413a7c
0076e610: ldr      r2, [pc, #0x8d4]
0076e614: add      r6, sp, #0xc0
0076e618: mov      r3, #0
0076e61c: ldr      r1, [r5, r2]
0076e620: mov      r0, r6
0076e624: strb     r3, [sp, #0xc1]
0076e628: strb     r3, [sp, #0xc0]
0076e62c: bl       #0x7972a0
0076e630: mov      r0, sl
0076e634: mov      r1, r8
0076e638: mov      r2, r6
0076e63c: bl       #0x768b54
0076e640: mov      r0, r6
0076e644: bl       #0x797124
0076e648: ldrb     r3, [sp, #0x2b0]
0076e64c: cmp      r3, #0xff
0076e650: beq      #0x76ed58
0076e654: ldr      r1, [pc, #0x894]
0076e658: add      r8, sp, #0x29c
0076e65c: mov      r0, r8
0076e660: add      r1, pc, r1
0076e664: ldr      sl, [r4, #0x34]
0076e668: bl       #0x413a7c
0076e66c: mov      r0, r4
0076e670: bl       #0x7a4c00
0076e674: mov      r2, #0
0076e678: strb     r2, [sp, #0xb4]
0076e67c: cmp      r0, #0
0076e680: mov      r2, #5
0076e684: strb     r2, [sp, #0xb5]
0076e688: str      r0, [sp, #0xb8]
0076e68c: beq      #0x76e694
0076e690: bl       #0x759c64
0076e694: add      r6, sp, #0xb4
0076e698: mov      r1, r8
0076e69c: mov      r0, sl
0076e6a0: mov      r2, r6
0076e6a4: bl       #0x768b54
0076e6a8: mov      r0, r6
0076e6ac: bl       #0x797124
0076e6b0: ldrb     r3, [sp, #0x29c]
0076e6b4: cmp      r3, #0xff
0076e6b8: beq      #0x76ed68
0076e6bc: ldr      r1, [pc, #0x830]
0076e6c0: add      r8, sp, #0x288
0076e6c4: mov      r0, r8
0076e6c8: add      r1, pc, r1
0076e6cc: ldr      sl, [r4, #0x34]
0076e6d0: bl       #0x413a7c
0076e6d4: ldr      r2, [pc, #0x81c]
0076e6d8: add      r6, sp, #0xa8
0076e6dc: mov      r3, #0
0076e6e0: ldr      r1, [r5, r2]
0076e6e4: mov      r0, r6
0076e6e8: strb     r3, [sp, #0xa9]
0076e6ec: strb     r3, [sp, #0xa8]
0076e6f0: bl       #0x7972a0
0076e6f4: mov      r0, sl
0076e6f8: mov      r1, r8
0076e6fc: mov      r2, r6
0076e700: bl       #0x768b54
0076e704: mov      r0, r6
0076e708: bl       #0x797124
0076e70c: ldrb     r3, [sp, #0x288]
0076e710: cmp      r3, #0xff
0076e714: beq      #0x76ed78
0076e718: ldr      r1, [pc, #0x7dc]
0076e71c: add      r8, sp, #0x274
0076e720: mov      r0, r8
0076e724: add      r1, pc, r1
0076e728: ldr      sl, [r4, #0x34]
0076e72c: bl       #0x413a7c
0076e730: ldr      r2, [pc, #0x7c8]
0076e734: add      r6, sp, #0x9c
0076e738: mov      r3, #0
0076e73c: ldr      r1, [r5, r2]
0076e740: mov      r0, r6
0076e744: strb     r3, [sp, #0x9d]
0076e748: strb     r3, [sp, #0x9c]
0076e74c: bl       #0x7972a0
0076e750: mov      r0, sl
0076e754: mov      r1, r8
0076e758: mov      r2, r6
0076e75c: bl       #0x768b54
0076e760: mov      r0, r6
0076e764: bl       #0x797124
0076e768: ldrb     r3, [sp, #0x274]
0076e76c: cmp      r3, #0xff
0076e770: beq      #0x76ed88
0076e774: ldr      r1, [pc, #0x788]
0076e778: add      r8, sp, #0x260
0076e77c: mov      r0, r8
0076e780: add      r1, pc, r1
0076e784: ldr      sl, [r4, #0x34]
0076e788: bl       #0x413a7c
0076e78c: ldr      r2, [pc, #0x774]
0076e790: add      r6, sp, #0x90
0076e794: mov      r3, #0
0076e798: ldr      r1, [r5, r2]
0076e79c: mov      r0, r6
0076e7a0: strb     r3, [sp, #0x91]
0076e7a4: strb     r3, [sp, #0x90]
0076e7a8: bl       #0x7972a0
0076e7ac: mov      r0, sl
0076e7b0: mov      r1, r8
0076e7b4: mov      r2, r6
0076e7b8: bl       #0x768b54
0076e7bc: mov      r0, r6
0076e7c0: bl       #0x797124
0076e7c4: ldrb     r3, [sp, #0x260]
0076e7c8: cmp      r3, #0xff
0076e7cc: beq      #0x76ed98
0076e7d0: ldr      r1, [pc, #0x734]
0076e7d4: add      r8, sp, #0x24c
0076e7d8: mov      r0, r8
0076e7dc: add      r1, pc, r1
0076e7e0: ldr      sl, [r4, #0x34]
0076e7e4: bl       #0x413a7c
0076e7e8: mov      r0, r4
0076e7ec: bl       #0x7a0690
0076e7f0: mov      r2, #0
0076e7f4: strb     r2, [sp, #0x84]
0076e7f8: cmp      r0, #0
0076e7fc: mov      r2, #5
0076e800: strb     r2, [sp, #0x85]
0076e804: str      r0, [sp, #0x88]
0076e808: beq      #0x76e810
0076e80c: bl       #0x759c64
0076e810: add      r6, sp, #0x84
0076e814: mov      r1, r8
0076e818: mov      r0, sl
0076e81c: mov      r2, r6
0076e820: bl       #0x768b54
0076e824: mov      r0, r6
0076e828: bl       #0x797124
0076e82c: ldrb     r3, [sp, #0x24c]
0076e830: cmp      r3, #0xff
0076e834: beq      #0x76eda8
0076e838: ldr      r1, [pc, #0x6d0]
0076e83c: add      r8, sp, #0x238
0076e840: mov      r0, r8
0076e844: add      r1, pc, r1
0076e848: ldr      sl, [r4, #0x34]
0076e84c: bl       #0x413a7c
0076e850: mov      r0, r4
0076e854: bl       #0x79f3c4
0076e858: mov      r2, #0
0076e85c: strb     r2, [sp, #0x78]
0076e860: cmp      r0, #0
0076e864: mov      r2, #5
0076e868: strb     r2, [sp, #0x79]
0076e86c: str      r0, [sp, #0x7c]
0076e870: beq      #0x76e878
0076e874: bl       #0x759c64
0076e878: add      r6, sp, #0x78
0076e87c: mov      r1, r8
0076e880: mov      r0, sl
0076e884: mov      r2, r6
0076e888: bl       #0x768b54
0076e88c: mov      r0, r6
0076e890: bl       #0x797124
0076e894: ldrb     r3, [sp, #0x238]
0076e898: cmp      r3, #0xff
0076e89c: beq      #0x76edb8
0076e8a0: ldr      r1, [pc, #0x66c]
0076e8a4: add      r8, sp, #0x224
0076e8a8: mov      r0, r8
0076e8ac: add      r1, pc, r1
0076e8b0: ldr      sl, [r4, #0x34]
0076e8b4: bl       #0x413a7c
0076e8b8: mov      r0, r4
0076e8bc: bl       #0x79b8e4
0076e8c0: mov      r2, #0
0076e8c4: strb     r2, [sp, #0x6c]
0076e8c8: cmp      r0, #0
0076e8cc: mov      r2, #5
0076e8d0: strb     r2, [sp, #0x6d]
0076e8d4: str      r0, [sp, #0x70]
0076e8d8: beq      #0x76e8e0
0076e8dc: bl       #0x759c64
0076e8e0: add      r6, sp, #0x6c
0076e8e4: mov      r1, r8
0076e8e8: mov      r0, sl
0076e8ec: mov      r2, r6
0076e8f0: bl       #0x768b54
0076e8f4: mov      r0, r6
0076e8f8: bl       #0x797124
0076e8fc: ldrb     r3, [sp, #0x224]
0076e900: cmp      r3, #0xff
0076e904: beq      #0x76edc8
0076e908: ldr      r1, [pc, #0x608]
0076e90c: add      r8, sp, #0x210
0076e910: mov      r0, r8
0076e914: add      r1, pc, r1
0076e918: ldr      sl, [r4, #0x34]
0076e91c: bl       #0x413a7c
0076e920: mov      r0, r4
0076e924: bl       #0x79e380
0076e928: mov      r2, #0
0076e92c: strb     r2, [sp, #0x60]
0076e930: cmp      r0, #0
0076e934: mov      r2, #5
0076e938: strb     r2, [sp, #0x61]
0076e93c: str      r0, [sp, #0x64]
0076e940: beq      #0x76e948
0076e944: bl       #0x759c64
0076e948: add      r6, sp, #0x60
0076e94c: mov      r1, r8
0076e950: mov      r0, sl
0076e954: mov      r2, r6
0076e958: bl       #0x768b54
0076e95c: mov      r0, r6
0076e960: bl       #0x797124
0076e964: ldrb     r3, [sp, #0x210]
0076e968: cmp      r3, #0xff
0076e96c: beq      #0x76edd8
0076e970: ldr      r1, [pc, #0x5a4]
0076e974: add      r8, sp, #0x1fc
0076e978: mov      r0, r8
0076e97c: add      r1, pc, r1
0076e980: ldr      sl, [r4, #0x34]
0076e984: bl       #0x413a7c
0076e988: ldr      r2, [pc, #0x590]
0076e98c: add      r6, sp, #0x54
0076e990: mov      r3, #0
0076e994: ldr      r1, [r5, r2]
0076e998: mov      r0, r6
0076e99c: strb     r3, [sp, #0x55]
0076e9a0: strb     r3, [sp, #0x54]
0076e9a4: bl       #0x7972a0
0076e9a8: mov      r0, sl
0076e9ac: mov      r1, r8
0076e9b0: mov      r2, r6
0076e9b4: bl       #0x768b54
0076e9b8: mov      r0, r6
0076e9bc: bl       #0x797124
0076e9c0: ldrb     r3, [sp, #0x1fc]
0076e9c4: cmp      r3, #0xff
0076e9c8: beq      #0x76ede8
0076e9cc: ldr      r1, [pc, #0x550]
0076e9d0: add      r8, sp, #0x1e8
0076e9d4: mov      r0, r8
0076e9d8: add      r1, pc, r1
0076e9dc: ldr      sl, [r4, #0x34]
0076e9e0: bl       #0x413a7c
0076e9e4: ldr      r2, [pc, #0x53c]
0076e9e8: add      r6, sp, #0x48
0076e9ec: mov      r3, #0
0076e9f0: ldr      r1, [r5, r2]
0076e9f4: mov      r0, r6
0076e9f8: strb     r3, [sp, #0x49]
0076e9fc: strb     r3, [sp, #0x48]
0076ea00: bl       #0x7972a0
0076ea04: mov      r0, sl
0076ea08: mov      r1, r8
0076ea0c: mov      r2, r6
0076ea10: bl       #0x768b54
0076ea14: mov      r0, r6
0076ea18: bl       #0x797124
0076ea1c: ldrb     r3, [sp, #0x1e8]
0076ea20: cmp      r3, #0xff
0076ea24: beq      #0x76edf8
0076ea28: ldr      r1, [pc, #0x4fc]
0076ea2c: add      r8, sp, #0x1d4
0076ea30: mov      r0, r8
0076ea34: add      r1, pc, r1
0076ea38: ldr      sl, [r4, #0x34]
0076ea3c: bl       #0x413a7c
0076ea40: ldr      r2, [pc, #0x4e8]
0076ea44: add      r6, sp, #0x3c
0076ea48: mov      r3, #0
0076ea4c: ldr      r1, [r5, r2]
0076ea50: mov      r0, r6
0076ea54: strb     r3, [sp, #0x3d]
0076ea58: strb     r3, [sp, #0x3c]
0076ea5c: bl       #0x7972a0
0076ea60: mov      r0, sl
0076ea64: mov      r1, r8
0076ea68: mov      r2, r6
0076ea6c: bl       #0x768b54
0076ea70: mov      r0, r6
0076ea74: bl       #0x797124
0076ea78: ldrb     r3, [sp, #0x1d4]
0076ea7c: cmp      r3, #0xff
0076ea80: beq      #0x76ee08
0076ea84: ldr      r1, [pc, #0x4a8]
0076ea88: add      r8, sp, #0x1c0
0076ea8c: mov      r0, r8
0076ea90: add      r1, pc, r1
0076ea94: ldr      sl, [r4, #0x34]
0076ea98: bl       #0x413a7c
0076ea9c: ldr      r2, [pc, #0x494]
0076eaa0: add      r6, sp, #0x30
0076eaa4: mov      r3, #0
0076eaa8: ldr      r1, [r5, r2]
0076eaac: mov      r0, r6
0076eab0: strb     r3, [sp, #0x31]
0076eab4: strb     r3, [sp, #0x30]
0076eab8: bl       #0x7972a0
0076eabc: mov      r0, sl
0076eac0: mov      r1, r8
0076eac4: mov      r2, r6
0076eac8: bl       #0x768b54
0076eacc: mov      r0, r6
0076ead0: bl       #0x797124
0076ead4: ldrb     r3, [sp, #0x1c0]
0076ead8: cmp      r3, #0xff
0076eadc: beq      #0x76ee18
0076eae0: ldr      r1, [pc, #0x454]
0076eae4: add      r8, sp, #0x1ac
0076eae8: mov      r0, r8
0076eaec: add      r1, pc, r1
0076eaf0: ldr      sl, [r4, #0x34]
0076eaf4: bl       #0x413a7c
0076eaf8: ldr      r2, [pc, #0x440]
0076eafc: add      r6, sp, #0x24
0076eb00: mov      r3, #0
0076eb04: ldr      r1, [r5, r2]
0076eb08: mov      r0, r6
0076eb0c: strb     r3, [sp, #0x25]
0076eb10: strb     r3, [sp, #0x24]
0076eb14: bl       #0x7972a0
0076eb18: mov      r0, sl
0076eb1c: mov      r1, r8
0076eb20: mov      r2, r6
0076eb24: bl       #0x768b54
0076eb28: mov      r0, r6
0076eb2c: bl       #0x797124
0076eb30: ldrb     r3, [sp, #0x1ac]
0076eb34: cmp      r3, #0xff
0076eb38: beq      #0x76ee28
0076eb3c: ldr      r1, [pc, #0x400]
0076eb40: add      r8, sp, #0x198
0076eb44: mov      r0, r8
0076eb48: add      r1, pc, r1
0076eb4c: ldr      sl, [r4, #0x34]
0076eb50: bl       #0x413a7c
0076eb54: ldr      r2, [pc, #0x3ec]
0076eb58: add      r6, sp, #0x18
0076eb5c: mov      r3, #0
0076eb60: ldr      r1, [r5, r2]
0076eb64: mov      r0, r6
0076eb68: strb     r3, [sp, #0x19]
0076eb6c: strb     r3, [sp, #0x18]
0076eb70: bl       #0x7972a0
0076eb74: mov      r0, sl
0076eb78: mov      r1, r8
0076eb7c: mov      r2, r6
0076eb80: bl       #0x768b54
0076eb84: mov      r0, r6
0076eb88: bl       #0x797124
0076eb8c: ldrb     r3, [sp, #0x198]
0076eb90: cmp      r3, #0xff
0076eb94: beq      #0x76ee38
0076eb98: ldr      r1, [pc, #0x3ac]
0076eb9c: add      r8, sp, #0x170
0076eba0: mov      r0, r8
0076eba4: add      r1, pc, r1
0076eba8: ldr      sl, [r4, #0x34]
0076ebac: bl       #0x413a7c
0076ebb0: ldr      r1, [pc, #0x398]
0076ebb4: add      r6, sp, #0x184
0076ebb8: mov      r0, r6
0076ebbc: add      r1, pc, r1
0076ebc0: bl       #0x413a7c
0076ebc4: mov      r1, r6
0076ebc8: add      r0, r4, #0x2c
0076ebcc: bl       #0x75c2cc
0076ebd0: add      r6, sp, #0xc
0076ebd4: mov      r3, #0
0076ebd8: mov      r1, r0
0076ebdc: mov      r0, r6
0076ebe0: str      r3, [sp, #0x10]
0076ebe4: strb     r3, [sp, #0xc]
0076ebe8: strb     r3, [sp, #0xd]
0076ebec: bl       #0x7972d8
0076ebf0: mov      r0, sl
0076ebf4: mov      r1, r8
0076ebf8: mov      r2, r6
0076ebfc: bl       #0x768b54
0076ec00: mov      r0, r6
0076ec04: bl       #0x797124
0076ec08: ldrb     r3, [sp, #0x184]
0076ec0c: cmp      r3, #0xff
0076ec10: beq      #0x76ee48
0076ec14: ldrb     r3, [sp, #0x170]
0076ec18: cmp      r3, #0xff
0076ec1c: beq      #0x76ee60
0076ec20: ldr      r1, [pc, #0x32c]
0076ec24: add      r6, sp, #0x15c
0076ec28: mov      r0, r6
0076ec2c: add      r1, pc, r1
0076ec30: ldr      r8, [r4, #0x34]
0076ec34: bl       #0x413a7c
0076ec38: ldr      r2, [pc, #0x318]
0076ec3c: mov      r3, #0
0076ec40: mov      r0, sp
0076ec44: ldr      r1, [r5, r2]
0076ec48: strb     r3, [sp, #1]
0076ec4c: strb     r3, [sp]
0076ec50: bl       #0x7972a0
0076ec54: mov      r0, r8
0076ec58: mov      r1, r6
0076ec5c: mov      r2, sp
0076ec60: bl       #0x768b54
0076ec64: mov      r0, sp
0076ec68: bl       #0x797124
0076ec6c: ldrb     r3, [sp, #0x15c]
0076ec70: mov      r4, sp
0076ec74: cmp      r3, #0xff
0076ec78: beq      #0x76ee70
0076ec7c: ldr      r3, [r5, r7]
0076ec80: ldr      r2, [sp, #0x3b4]
0076ec84: ldr      r3, [r3]
0076ec88: cmp      r2, r3
0076ec8c: bne      #0x76ee80
0076ec90: add      sp, sp, #0x3bc
0076ec94: pop      {r4, r5, r6, r7, r8, sl, pc}
0076ec98: ldr      r0, [sp, #0x3ac]
0076ec9c: ldr      r1, [sp, #0x3a8]
0076eca0: bl       #0x752b38
0076eca4: b        #0x76e1cc
0076eca8: ldr      r0, [sp, #0x398]
0076ecac: ldr      r1, [sp, #0x394]
0076ecb0: bl       #0x752b38
0076ecb4: b        #0x76e22c
0076ecb8: ldr      r0, [sp, #0x384]
0076ecbc: ldr      r1, [sp, #0x380]
0076ecc0: bl       #0x752b38
0076ecc4: b        #0x76e28c
0076ecc8: ldr      r0, [sp, #0x370]
0076eccc: ldr      r1, [sp, #0x36c]
0076ecd0: bl       #0x752b38
0076ecd4: b        #0x76e2f8
0076ecd8: ldr      r0, [sp, #0x35c]
0076ecdc: ldr      r1, [sp, #0x358]
0076ece0: bl       #0x752b38
0076ece4: b        #0x76e358
0076ece8: ldr      r0, [sp, #0x348]
0076ecec: ldr      r1, [sp, #0x344]
0076ecf0: bl       #0x752b38
0076ecf4: b        #0x76e3b8
0076ecf8: ldr      r0, [sp, #0x334]
0076ecfc: ldr      r1, [sp, #0x330]
0076ed00: bl       #0x752b38
0076ed04: b        #0x76e418
0076ed08: ldr      r0, [sp, #0x320]
0076ed0c: ldr      r1, [sp, #0x31c]
0076ed10: bl       #0x752b38
0076ed14: b        #0x76e478
0076ed18: ldr      r0, [sp, #0x30c]
0076ed1c: ldr      r1, [sp, #0x308]
0076ed20: bl       #0x752b38
0076ed24: b        #0x76e4d8
0076ed28: ldr      r0, [sp, #0x2f8]
0076ed2c: ldr      r1, [sp, #0x2f4]
0076ed30: bl       #0x752b38
0076ed34: b        #0x76e538
0076ed38: ldr      r0, [sp, #0x2e4]
0076ed3c: ldr      r1, [sp, #0x2e0]
0076ed40: bl       #0x752b38
0076ed44: b        #0x76e598
0076ed48: ldr      r0, [sp, #0x2d0]
0076ed4c: ldr      r1, [sp, #0x2cc]
0076ed50: bl       #0x752b38
0076ed54: b        #0x76e5f8
0076ed58: ldr      r0, [sp, #0x2bc]
0076ed5c: ldr      r1, [sp, #0x2b8]
0076ed60: bl       #0x752b38
0076ed64: b        #0x76e654
0076ed68: ldr      r0, [sp, #0x2a8]
0076ed6c: ldr      r1, [sp, #0x2a4]
0076ed70: bl       #0x752b38
0076ed74: b        #0x76e6bc
0076ed78: ldr      r0, [sp, #0x294]
0076ed7c: ldr      r1, [sp, #0x290]
0076ed80: bl       #0x752b38
0076ed84: b        #0x76e718
0076ed88: ldr      r0, [sp, #0x280]
0076ed8c: ldr      r1, [sp, #0x27c]
0076ed90: bl       #0x752b38
0076ed94: b        #0x76e774
0076ed98: ldr      r0, [sp, #0x26c]
0076ed9c: ldr      r1, [sp, #0x268]
0076eda0: bl       #0x752b38
0076eda4: b        #0x76e7d0
0076eda8: ldr      r0, [sp, #0x258]
0076edac: ldr      r1, [sp, #0x254]
0076edb0: bl       #0x752b38
0076edb4: b        #0x76e838
0076edb8: ldr      r0, [sp, #0x244]
0076edbc: ldr      r1, [sp, #0x240]
0076edc0: bl       #0x752b38
0076edc4: b        #0x76e8a0
0076edc8: ldr      r0, [sp, #0x230]
0076edcc: ldr      r1, [sp, #0x22c]
0076edd0: bl       #0x752b38
0076edd4: b        #0x76e908
0076edd8: ldr      r0, [sp, #0x21c]
0076eddc: ldr      r1, [sp, #0x218]
0076ede0: bl       #0x752b38
0076ede4: b        #0x76e970
0076ede8: ldr      r0, [sp, #0x208]
0076edec: ldr      r1, [sp, #0x204]
0076edf0: bl       #0x752b38
0076edf4: b        #0x76e9cc
0076edf8: ldr      r0, [sp, #0x1f4]
0076edfc: ldr      r1, [sp, #0x1f0]
0076ee00: bl       #0x752b38
0076ee04: b        #0x76ea28
0076ee08: ldr      r0, [sp, #0x1e0]
0076ee0c: ldr      r1, [sp, #0x1dc]
0076ee10: bl       #0x752b38
0076ee14: b        #0x76ea84
0076ee18: ldr      r0, [sp, #0x1cc]
0076ee1c: ldr      r1, [sp, #0x1c8]
0076ee20: bl       #0x752b38
0076ee24: b        #0x76eae0
0076ee28: ldr      r0, [sp, #0x1b8]
0076ee2c: ldr      r1, [sp, #0x1b4]
0076ee30: bl       #0x752b38
0076ee34: b        #0x76eb3c
0076ee38: ldr      r0, [sp, #0x1a4]
0076ee3c: ldr      r1, [sp, #0x1a0]
0076ee40: bl       #0x752b38
0076ee44: b        #0x76eb98
0076ee48: ldr      r0, [sp, #0x190]
0076ee4c: ldr      r1, [sp, #0x18c]
0076ee50: bl       #0x752b38
0076ee54: ldrb     r3, [sp, #0x170]
0076ee58: cmp      r3, #0xff
0076ee5c: bne      #0x76ec20
0076ee60: ldr      r0, [sp, #0x17c]
0076ee64: ldr      r1, [sp, #0x178]
0076ee68: bl       #0x752b38
0076ee6c: b        #0x76ec20
0076ee70: ldr      r0, [sp, #0x168]
0076ee74: ldr      r1, [sp, #0x164]
0076ee78: bl       #0x752b38
0076ee7c: b        #0x76ec7c
0076ee80: bl       #0x30e310
0076ee84: eoreq    r6, r2, ip, ror sb
0076ee88: andeq    r4, r0, ip, lsr #1
0076ee8c: ldrsheq  sl, [sb], -r8
0076ee90: andeq    r2, r0, ip, asr sp
0076ee94: andseq   ip, r6, r8, lsl #24
0076ee98: muleq    r0, r4, r0
0076ee9c: andseq   ip, r6, r0, lsl #21
0076eea0: andeq    r4, r0, r4, lsl #18
0076eea4: andseq   sl, sb, r0, ror #29
0076eea8: andseq   sl, sb, ip, ror lr
0076eeac: andeq    r4, r0, r0, ror #11
0076eeb0: andseq   sl, sb, ip, lsr #28
0076eeb4: muleq    r0, r8, sl
0076eeb8: ldrsbeq  sl, [sb], -ip
0076eebc: strdeq   r4, r5, [r0], -r8
0076eec0: andseq   sl, sb, ip, lsl #27
0076eec4: andeq    r0, r0, r8, lsl #20
0076eec8: andseq   sl, sb, ip, lsr sp
0076eecc: ldrdeq   r2, r3, [r0], -ip
0076eed0: andseq   sl, sb, r4, ror #25
0076eed4: andeq    r0, r0, r0, ror #27
0076eed8: andseq   sl, sb, ip, lsl #25
0076eedc: andeq    r3, r0, r0, lsr #23
0076eee0: andseq   r4, r7, r4, lsl r8
0076eee4: andeq    r3, r0, r4, asr r6
0076eee8: ldrsbeq  sl, [sb], -r4
0076eeec: andeq    r1, r0, r4, ror #24
0076eef0: andseq   sl, sb, r0, lsl #23
0076eef4: andseq   sl, sb, r8, lsr #22
0076eef8: andeq    r1, r0, r0, lsr #14
0076eefc: ldrsbeq  sl, [sb], -ip
0076ef00: strheq   r0, [r0], -r4
0076ef04: mulseq   sb, r0, sl
0076ef08: andeq    r0, r0, r0, lsl #18
0076ef0c: andseq   sl, sb, r4, asr #20
0076ef10: ldrsheq  sl, [sb], -r4
0076ef14: andseq   sl, sb, ip, ror sb
0076ef18: andseq   sl, sb, r4, lsl #14
0076ef1c: ldrheq   sl, [sb], -ip
0076ef20: ldrdeq   r2, r3, [r0], -r0
0076ef24: andseq   sl, sb, r0, ror r8
0076ef28: andeq    r2, r0, r4, lsr r4
0076ef2c: andseq   sl, sb, r4, lsr #16
0076ef30: muleq    r0, r8, r2
0076ef34: ldrsbeq  sl, [sb], -r8
0076ef38: andeq    r4, r0, r0, asr #20
0076ef3c: andseq   sl, sb, ip, lsl #15
0076ef40: muleq    r0, r0, r6
0076ef44: andseq   sl, sb, r0, asr #14
0076ef48: andeq    r3, r0, ip, asr r8
0076ef4c: andseq   sl, sb, r4, ror r1
0076ef50: ldrsbeq  sl, [sb], -r4
0076ef54: andseq   sl, sb, ip, ror #12
0076ef58: ldrdeq   r3, r4, [r0], -ip

# _ZN7gameswf23new_standard_method_mapENS_14builtin_objectE
0076cb04: push     {r4, lr}
0076cb08: ldr      r4, [pc, #0x30]
0076cb0c: add      r4, pc, r4
0076cb10: add      r4, r4, r0, lsl #2
0076cb14: ldr      r0, [r4, #0x28]
0076cb18: cmp      r0, #0
0076cb1c: beq      #0x76cb24
0076cb20: pop      {r4, pc}
0076cb24: mov      r1, r0
0076cb28: mov      r0, #4
0076cb2c: bl       #0x752ba8
0076cb30: mov      r3, #0
0076cb34: str      r3, [r0]
0076cb38: str      r0, [r4, #0x28]
0076cb3c: pop      {r4, pc}
0076cb40: eoreq    pc, r8, ip, ror #24

# _ZN7gameswf11get_builtinENS_14builtin_objectERKNS_10tu_stringiEPNS_8as_valueE
0076ce08: ldr      r3, [pc, #0x14]
0076ce0c: add      r3, pc, r3
0076ce10: add      r3, r3, r0, lsl #2
0076ce14: ldr      r0, [r3, #0x28]
0076ce18: cmp      r0, #0
0076ce1c: bxeq     lr
0076ce20: b        #0x769048
0076ce24: eoreq    pc, r8, ip, ror #18

# _ZN7gameswf14set_textformatERKNS_7fn_callE
0078f9f0: push     {r4, r5, r6, lr}
0078f9f4: ldr      r5, [r0, #4]
0078f9f8: mov      r4, r0
0078f9fc: cmp      r5, #0
0078fa00: beq      #0x78fa30
0078fa04: ldr      r3, [r5]
0078fa08: mov      r0, r5
0078fa0c: mov      r1, #0x20
0078fa10: mov      lr, pc
0078fa14: ldr      pc, [r3, #8]
0078fa18: cmp      r0, #0
0078fa1c: beq      #0x78fa30
0078fa20: ldr      r3, [r4, #0x10]
0078fa24: cmp      r3, #1
0078fa28: beq      #0x78fa40
0078fa2c: pop      {r4, r5, r6, pc}
0078fa30: ldr      r3, [r4, #0x10]
0078fa34: mov      r5, #0
0078fa38: cmp      r3, #1
0078fa3c: bne      #0x78fa2c
0078fa40: ldr      r2, [r4, #0xc]
0078fa44: ldr      r3, [r4, #0x14]
0078fa48: mov      r1, #0xc
0078fa4c: ldr      r2, [r2]
0078fa50: mla      r3, r1, r3, r2
0078fa54: ldrsb    r2, [r3, #1]
0078fa58: cmp      r2, #5
0078fa5c: beq      #0x78fa70
0078fa60: mov      r1, #0
0078fa64: mov      r0, r5
0078fa68: pop      {r4, r5, r6, lr}
0078fa6c: b        #0x78f288
0078fa70: ldr      r4, [r3, #4]
0078fa74: cmp      r4, #0
0078fa78: beq      #0x78fa60
0078fa7c: mov      r1, #0x23
0078fa80: ldr      r3, [r4]
0078fa84: mov      r0, r4
0078fa88: mov      lr, pc
0078fa8c: ldr      pc, [r3, #8]
0078fa90: cmp      r0, #0
0078fa94: movne    r1, r4
0078fa98: bne      #0x78fa64
0078fa9c: b        #0x78fa60

# _ZN7gameswf24as_global_textfield_ctorERKNS_7fn_callE
0079181c: push     {r4, r5, r6, r7, lr}
00791820: ldr      r5, [r0, #0xc]
00791824: mov      r4, r0
00791828: sub      sp, sp, #0xc
0079182c: ldr      r0, [r5, #0x68]
00791830: cmp      r0, #0
00791834: beq      #0x791848
00791838: ldr      r3, [r5, #0x64]
0079183c: ldrb     r2, [r3, #4]
00791840: cmp      r2, #0
00791844: beq      #0x7918e8
00791848: bl       #0x76d5b4
0079184c: ldr      r5, [r4, #0xc]
00791850: ldr      r7, [r5, #0x68]
00791854: cmp      r7, #0
00791858: beq      #0x79186c
0079185c: ldr      r0, [r5, #0x64]
00791860: ldrb     r3, [r0, #4]
00791864: cmp      r3, #0
00791868: beq      #0x79193c
0079186c: mov      r1, #0
00791870: mov      r0, #0xa0
00791874: bl       #0x752ba8
00791878: mov      r2, #0
0079187c: mov      r1, r7
00791880: mov      r3, r2
00791884: mov      r6, r0
00791888: bl       #0x78d970
0079188c: ldr      r5, [r4, #0xc]
00791890: ldr      r7, [r5, #0x68]
00791894: cmp      r7, #0
00791898: beq      #0x7918ac
0079189c: ldr      r0, [r5, #0x64]
007918a0: ldrb     r3, [r0, #4]
007918a4: cmp      r3, #0
007918a8: beq      #0x791914
007918ac: mov      r1, #0
007918b0: mov      r0, #0x198
007918b4: bl       #0x752ba8
007918b8: mov      ip, #0
007918bc: mov      r1, r7
007918c0: mov      r2, ip
007918c4: mov      r3, r6
007918c8: mov      r5, r0
007918cc: str      ip, [sp]
007918d0: bl       #0x7916bc
007918d4: ldr      r0, [r4]
007918d8: mov      r1, r5
007918dc: add      sp, sp, #0xc
007918e0: pop      {r4, r5, r6, r7, lr}
007918e4: b        #0x797250
007918e8: ldr      r1, [r3]
007918ec: sub      r1, r1, #1
007918f0: cmp      r1, #0
007918f4: str      r1, [r3]
007918f8: bne      #0x791904
007918fc: mov      r0, r3
00791900: bl       #0x752b38
00791904: mov      r0, #0
00791908: str      r0, [r5, #0x68]
0079190c: str      r0, [r5, #0x64]
00791910: b        #0x791848
00791914: ldr      r1, [r0]
00791918: sub      r1, r1, #1
0079191c: cmp      r1, #0
00791920: str      r1, [r0]
00791924: bne      #0x79192c
00791928: bl       #0x752b38
0079192c: mov      r7, #0
00791930: str      r7, [r5, #0x68]
00791934: str      r7, [r5, #0x64]
00791938: b        #0x7918ac
0079193c: ldr      r1, [r0]
00791940: sub      r1, r1, #1
00791944: cmp      r1, #0
00791948: str      r1, [r0]
0079194c: bne      #0x791954
00791950: bl       #0x752b38
00791954: mov      r7, #0
00791958: str      r7, [r5, #0x68]
0079195c: str      r7, [r5, #0x64]
00791960: b        #0x79186c

