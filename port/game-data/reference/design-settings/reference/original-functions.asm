
# _ZN6Arrays19DesignSettingsTable9readNamesEP11IStreamBase
004b7638: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b763c: mov      r7, r0
004b7640: sub      sp, sp, #0x1c
004b7644: bl       #0x4a901c
004b7648: mov      r0, r7
004b764c: bl       #0x313a90
004b7650: ldr      r6, [pc, #0x16c]
004b7654: mov      r3, #1
004b7658: cmp      r3, #0
004b765c: add      r6, pc, r6
004b7660: str      r0, [sp, #0x14]
004b7664: str      r3, [sp, #0xc]
004b7668: bne      #0x4b76b8
004b766c: add      r3, sp, #0x14
004b7670: add      r2, r3, #2
004b7674: add      r3, r3, #1
004b7678: ldrb     r0, [r2, #1]
004b767c: ldrb     r1, [r3, #-1]
004b7680: cmp      r2, r3
004b7684: mov      r4, r2
004b7688: eor      r1, r0, r1
004b768c: strb     r1, [r3, #-1]
004b7690: ldrb     r0, [r2, #1]
004b7694: eor      r1, r1, r0
004b7698: strb     r1, [r2, #1]
004b769c: ldrb     r0, [r3, #-1]
004b76a0: sub      r2, r2, #1
004b76a4: eor      r1, r1, r0
004b76a8: strb     r1, [r3, #-1]
004b76ac: add      r3, r3, #1
004b76b0: bhi      #0x4b7678
004b76b4: ldr      r0, [sp, #0x14]
004b76b8: ldr      r3, [pc, #0x108]
004b76bc: ldr      r3, [r6, r3]
004b76c0: ldr      r3, [r3]
004b76c4: cmp      r3, r0
004b76c8: beq      #0x4b76d4
004b76cc: add      sp, sp, #0x1c
004b76d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b76d4: lsl      r0, r0, #2
004b76d8: mov      r1, #1
004b76dc: bl       #0x31056c
004b76e0: ldr      sb, [pc, #0xe4]
004b76e4: ldr      r2, [sp, #0x14]
004b76e8: ldr      r3, [r6, sb]
004b76ec: cmp      r2, #0
004b76f0: str      r0, [r3]
004b76f4: beq      #0x4b76cc
004b76f8: add      sl, sp, #0x10
004b76fc: mov      r8, #1
004b7700: add      r1, sl, r8
004b7704: add      r3, sl, #2
004b7708: mov      r4, #0
004b770c: stm      sp, {r1, r3}
004b7710: mov      r0, r7
004b7714: mov      r1, sl
004b7718: bl       #0x3df1a0
004b771c: cmp      r8, #0
004b7720: str      r8, [sp, #0xc]
004b7724: bne      #0x4b7768
004b7728: ldr      r3, [sp]
004b772c: ldr      r2, [sp, #4]
004b7730: ldrb     r0, [r2, #1]
004b7734: ldrb     r1, [r3, #-1]
004b7738: cmp      r2, r3
004b773c: eor      r1, r0, r1
004b7740: strb     r1, [r3, #-1]
004b7744: ldrb     r0, [r2, #1]
004b7748: eor      r1, r1, r0
004b774c: strb     r1, [r2, #1]
004b7750: ldrb     r0, [r3, #-1]
004b7754: sub      r2, r2, #1
004b7758: eor      r1, r1, r0
004b775c: strb     r1, [r3, #-1]
004b7760: add      r3, r3, #1
004b7764: bhi      #0x4b7730
004b7768: ldr      r0, [sp, #0x10]
004b776c: ldr      r5, [r6, sb]
004b7770: mov      r1, #1
004b7774: add      r0, r0, r1
004b7778: ldr      fp, [r5]
004b777c: bl       #0x31056c
004b7780: str      r0, [fp, r4, lsl #2]
004b7784: ldr      r3, [r5]
004b7788: ldr      r2, [sp, #0x10]
004b778c: mov      r0, r7
004b7790: ldr      r1, [r3, r4, lsl #2]
004b7794: mov      r3, #0
004b7798: bl       #0x317454
004b779c: ldr      r3, [r5]
004b77a0: mov      r1, #0
004b77a4: ldr      r2, [r3, r4, lsl #2]
004b77a8: ldr      r3, [sp, #0x10]
004b77ac: add      r4, r4, #1
004b77b0: strb     r1, [r2, r3]
004b77b4: ldr      r3, [sp, #0x14]
004b77b8: cmp      r3, r4
004b77bc: bhi      #0x4b7710
004b77c0: b        #0x4b76cc
004b77c4: subeq    sp, sp, r4, lsr r4
004b77c8: andeq    r2, r0, r4, ror #29
004b77cc: andeq    r4, r0, r4, lsr #16

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

# _ZN12StreamReader6readAsIfEEvP11IStreamBasePT_
004db94c: str      lr, [sp, #-4]!
004db950: mov      r3, #0
004db954: sub      sp, sp, #0xc
004db958: ldr      ip, [r0]
004db95c: mov      r2, #4
004db960: mov      lr, pc
004db964: ldr      pc, [ip, #0x18]
004db968: ldr      r3, [pc, #0x74]
004db96c: cmp      r0, #4
004db970: add      r3, pc, r3
004db974: beq      #0x4db9a4
004db978: ldr      r2, [pc, #0x68]
004db97c: ldr      r2, [r3, r2]
004db980: ldr      r2, [r2]
004db984: cmp      r2, #2
004db988: moveq    r3, #0
004db98c: streq    r3, [r3]
004db990: beq      #0x4db99c
004db994: cmp      r2, #1
004db998: beq      #0x4db9b0
004db99c: add      sp, sp, #0xc
004db9a0: ldm      sp!, {pc}
004db9a4: cmp      r1, #0
004db9a8: beq      #0x4db99c
004db9ac: b        #0x4db978
004db9b0: ldr      r0, [pc, #0x34]
004db9b4: ldr      r1, [pc, #0x34]
004db9b8: ldr      r2, [pc, #0x34]
004db9bc: ldr      r0, [r3, r0]
004db9c0: ldr      r3, [pc, #0x30]
004db9c4: mov      ip, #0x50
004db9c8: add      r1, pc, r1
004db9cc: add      r2, pc, r2
004db9d0: add      r3, pc, r3
004db9d4: add      r0, r0, #0xa8
004db9d8: str      ip, [sp]
004db9dc: bl       #0x30e004
004db9e0: b        #0x4db99c
004db9e4: subeq    sb, fp, r0, lsr #2
004db9e8: andeq    r3, r0, r0, asr #19
004db9ec: andeq    r1, r0, r0, asr #19
004db9f0: eorseq   r2, lr, r0, lsl sl
004db9f4: eorseq   r2, lr, r4, lsr fp
004db9f8: eorseq   r4, lr, r0, ror r3

# _ZN6Arrays19DesignSettingsTable8finalizeEv
004a90b8: push     {r4, r5, r6, r7, r8, lr}
004a90bc: ldr      r5, [pc, #0xcc]
004a90c0: ldr      r7, [pc, #0xcc]
004a90c4: add      r5, pc, r5
004a90c8: ldr      r3, [r5, r7]
004a90cc: ldr      r3, [r3]
004a90d0: cmp      r3, #0
004a90d4: beq      #0x4a918c
004a90d8: ldr      r8, [pc, #0xb8]
004a90dc: ldr      r2, [r5, r8]
004a90e0: ldr      r2, [r2]
004a90e4: cmp      r2, #0
004a90e8: beq      #0x4a9138
004a90ec: mov      r4, #0
004a90f0: mov      r6, r4
004a90f4: b        #0x4a9100
004a90f8: ldr      r3, [r5, r7]
004a90fc: ldr      r3, [r3]
004a9100: add      r0, r3, r4
004a9104: ldr      r3, [r3, r4]
004a9108: mov      lr, pc
004a910c: ldr      pc, [r3, #8]
004a9110: ldr      r3, [r5, r8]
004a9114: add      r6, r6, #1
004a9118: add      r4, r4, #0xb0
004a911c: ldr      r3, [r3]
004a9120: cmp      r3, r6
004a9124: bhi      #0x4a90f8
004a9128: ldr      r3, [r5, r7]
004a912c: ldr      r3, [r3]
004a9130: cmp      r3, #0
004a9134: beq      #0x4a9180
004a9138: ldr      r2, [r3, #-4]
004a913c: mov      r0, #0xb0
004a9140: mla      r0, r0, r2, r3
004a9144: cmp      r3, r0
004a9148: bne      #0x4a9154
004a914c: b        #0x4a9178
004a9150: mov      r0, r4
004a9154: sub      r4, r0, #0xb0
004a9158: ldr      r3, [r0, #-0xb0]
004a915c: mov      r0, r4
004a9160: mov      lr, pc
004a9164: ldr      pc, [r3]
004a9168: ldr      r3, [r5, r7]
004a916c: ldr      r0, [r3]
004a9170: cmp      r0, r4
004a9174: bne      #0x4a9150
004a9178: sub      r0, r0, #8
004a917c: bl       #0x310440
004a9180: ldr      r3, [r5, r7]
004a9184: mov      r2, #0
004a9188: str      r2, [r3]
004a918c: pop      {r4, r5, r6, r7, r8, pc}
004a9190: subeq    fp, lr, ip, asr #19
004a9194: andeq    r3, r0, r8, asr #5
004a9198: andeq    r2, r0, r4, ror #29

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

# _ZN6Arrays19DesignSettingsTable13finalizeNamesEv
004a901c: push     {r4, r5, r6, r7, r8, lr}
004a9020: ldr      r5, [pc, #0x84]
004a9024: ldr      r6, [pc, #0x84]
004a9028: add      r5, pc, r5
004a902c: ldr      r3, [r5, r6]
004a9030: ldr      r3, [r3]
004a9034: cmp      r3, #0
004a9038: beq      #0x4a90a8
004a903c: ldr      r7, [pc, #0x70]
004a9040: ldr      r2, [r5, r7]
004a9044: ldr      r2, [r2]
004a9048: cmp      r2, #0
004a904c: beq      #0x4a9094
004a9050: mov      r4, #0
004a9054: b        #0x4a9060
004a9058: ldr      r3, [r5, r6]
004a905c: ldr      r3, [r3]
004a9060: ldr      r0, [r3, r4, lsl #2]
004a9064: add      r4, r4, #1
004a9068: cmp      r0, #0
004a906c: beq      #0x4a907c
004a9070: bl       #0x310440
004a9074: ldr      r3, [r5, r6]
004a9078: ldr      r3, [r3]
004a907c: ldr      r2, [r5, r7]
004a9080: ldr      r2, [r2]
004a9084: cmp      r2, r4
004a9088: bhi      #0x4a9058
004a908c: cmp      r3, #0
004a9090: beq      #0x4a909c
004a9094: mov      r0, r3
004a9098: bl       #0x310440
004a909c: ldr      r3, [r5, r6]
004a90a0: mov      r2, #0
004a90a4: str      r2, [r3]
004a90a8: pop      {r4, r5, r6, r7, r8, pc}
004a90ac: subeq    fp, lr, r8, ror #20
004a90b0: andeq    r4, r0, r4, lsr #16
004a90b4: andeq    r2, r0, r4, ror #29

# _ZN6Arrays19GetMemberIDByStringINS_19DesignSettingsTableEEEiPKc
004aed88: ldr      r3, [pc, #0x60]
004aed8c: ldr      r2, [pc, #0x60]
004aed90: push     {r4, r5, r6, r7, r8, lr}
004aed94: add      r3, pc, r3
004aed98: ldr      r2, [r3, r2]
004aed9c: mov      r6, r0
004aeda0: ldr      r5, [r2]
004aeda4: cmp      r5, #0
004aeda8: beq      #0x4aede8
004aedac: ldr      r2, [pc, #0x44]
004aedb0: mov      r4, #0
004aedb4: ldr      r3, [r3, r2]
004aedb8: ldr      r7, [r3]
004aedbc: b        #0x4aedcc
004aedc0: add      r4, r4, #1
004aedc4: cmp      r4, r5
004aedc8: beq      #0x4aede8
004aedcc: ldr      r1, [r7, r4, lsl #2]
004aedd0: mov      r0, r6
004aedd4: bl       #0x30e31c
004aedd8: cmp      r0, #0
004aeddc: bne      #0x4aedc0
004aede0: mov      r0, r4
004aede4: pop      {r4, r5, r6, r7, r8, pc}
004aede8: mvn      r0, #0
004aedec: pop      {r4, r5, r6, r7, r8, pc}
004aedf0: strdeq   r5, r6, [lr], #-0xcc
004aedf4: andeq    r2, r0, r4, ror #29
004aedf8: andeq    r4, r0, r4, lsr #16

# _ZN12StreamReader6readAsIjEET_P11IStreamBase
00313a90: str      lr, [sp, #-4]!
00313a94: sub      sp, sp, #0x14
00313a98: mov      r3, #0
00313a9c: ldr      ip, [r0]
00313aa0: add      r1, sp, #0xc
00313aa4: mov      r2, #4
00313aa8: mov      lr, pc
00313aac: ldr      pc, [ip, #0x18]
00313ab0: ldr      r3, [pc, #0x78]
00313ab4: cmp      r0, #4
00313ab8: add      r3, pc, r3
00313abc: beq      #0x313af0
00313ac0: ldr      r2, [pc, #0x6c]
00313ac4: ldr      r2, [r3, r2]
00313ac8: ldr      r2, [r2]
00313acc: cmp      r2, #2
00313ad0: moveq    r3, #0
00313ad4: streq    r3, [r3]
00313ad8: beq      #0x313ae4
00313adc: cmp      r2, #1
00313ae0: beq      #0x313afc
00313ae4: ldr      r0, [sp, #0xc]
00313ae8: add      sp, sp, #0x14
00313aec: ldm      sp!, {pc}
00313af0: cmp      r1, #0
00313af4: beq      #0x313ae4
00313af8: b        #0x313ac0
00313afc: ldr      r0, [pc, #0x34]
00313b00: ldr      r1, [pc, #0x34]
00313b04: ldr      r2, [pc, #0x34]
00313b08: ldr      r0, [r3, r0]
00313b0c: ldr      r3, [pc, #0x30]
00313b10: mov      ip, #0x44
00313b14: add      r1, pc, r1
00313b18: add      r2, pc, r2
00313b1c: add      r3, pc, r3
00313b20: add      r0, r0, #0xa8
00313b24: str      ip, [sp]
00313b28: bl       #0x30e004
00313b2c: b        #0x313ae4

# _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
00459090: str      lr, [sp, #-4]!
00459094: mov      r3, #0
00459098: sub      sp, sp, #0xc
0045909c: ldr      ip, [r0]
004590a0: mov      r2, #4
004590a4: mov      lr, pc
004590a8: ldr      pc, [ip, #0x18]
004590ac: ldr      r3, [pc, #0x74]
004590b0: cmp      r0, #4
004590b4: add      r3, pc, r3
004590b8: beq      #0x4590e8
004590bc: ldr      r2, [pc, #0x68]
004590c0: ldr      r2, [r3, r2]
004590c4: ldr      r2, [r2]
004590c8: cmp      r2, #2
004590cc: moveq    r3, #0
004590d0: streq    r3, [r3]
004590d4: beq      #0x4590e0
004590d8: cmp      r2, #1
004590dc: beq      #0x4590f4
004590e0: add      sp, sp, #0xc
004590e4: ldm      sp!, {pc}
004590e8: cmp      r1, #0
004590ec: beq      #0x4590e0
004590f0: b        #0x4590bc
004590f4: ldr      r0, [pc, #0x34]
004590f8: ldr      r1, [pc, #0x34]
004590fc: ldr      r2, [pc, #0x34]
00459100: ldr      r0, [r3, r0]
00459104: ldr      r3, [pc, #0x30]
00459108: mov      ip, #0x50
0045910c: add      r1, pc, r1
00459110: add      r2, pc, r2
00459114: add      r3, pc, r3
00459118: add      r0, r0, #0xa8
0045911c: str      ip, [sp]
00459120: bl       #0x30e004
00459124: b        #0x4590e0
00459128: ldrsbeq  fp, [r3], #-0x9c
0045912c: andeq    r3, r0, r0, asr #19
00459130: andeq    r1, r0, r0, asr #19
00459134: subeq    r5, r6, ip, asr #5
00459138: strdeq   r5, r6, [r6], #-0x30
0045913c: subeq    r6, r6, ip, lsr #24
