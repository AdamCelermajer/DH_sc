
# _ZNK13ItemInventory20IsEquipmentSlotTakenEj
004002d8: push     {r4, r5, lr}
004002dc: ldr      r2, [r0, #0x14]
004002e0: mov      r4, r1
004002e4: ldr      r3, [pc, #0xa0]
004002e8: ldm      r2, {r1, r2}
004002ec: add      r3, pc, r3
004002f0: sub      sp, sp, #0xc
004002f4: rsb      r2, r1, r2
004002f8: cmp      r4, r2, asr #2
004002fc: mov      r5, r0
00400300: blo      #0x400328
00400304: ldr      r2, [pc, #0x84]
00400308: ldr      r2, [r3, r2]
0040030c: ldr      r2, [r2]
00400310: cmp      r2, #2
00400314: moveq    r3, #0
00400318: streq    r3, [r3]
0040031c: beq      #0x400328
00400320: cmp      r2, #1
00400324: beq      #0x400358
00400328: mov      r0, r5
0040032c: mov      r1, r4
00400330: bl       #0x3fc6a8
00400334: mov      r3, #0xc
00400338: mul      r3, r3, r0
0040033c: ldr      r2, [r5, #0x14]
00400340: ldr      r3, [r2, r3]
00400344: ldr      r0, [r3, r4, lsl #2]
00400348: subs     r0, r0, #0
0040034c: movne    r0, #1
00400350: add      sp, sp, #0xc
00400354: pop      {r4, r5, pc}
00400358: ldr      r0, [pc, #0x34]
0040035c: ldr      r1, [pc, #0x34]
00400360: ldr      r2, [pc, #0x34]
00400364: ldr      r0, [r3, r0]
00400368: ldr      r3, [pc, #0x30]
0040036c: mov      ip, #0x140
00400370: add      r1, pc, r1
00400374: add      r2, pc, r2
00400378: add      r3, pc, r3
0040037c: add      r0, r0, #0xa8
00400380: str      ip, [sp]
00400384: bl       #0x30e004
00400388: b        #0x400328
0040038c: subseq   r4, sb, r4, lsr #15
00400390: andeq    r3, r0, r0, asr #19
00400394: andeq    r1, r0, r0, asr #19
00400398: subeq    lr, fp, r8, rrx
0040039c: subeq    r7, ip, ip, ror r0
004003a0: subeq    r7, ip, r0, ror #2

# _ZN13ItemInventory16_EquipItemToSlotEjjb
00400634: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00400638: mov      r4, r0
0040063c: ldr      r0, [r0, #0x14]
00400640: mov      r7, r1
00400644: ldr      r5, [pc, #0x374]
00400648: ldr      r1, [r0]
0040064c: ldr      r0, [r0, #4]
00400650: add      r5, pc, r5
00400654: sub      sp, sp, #0x14
00400658: rsb      r1, r1, r0
0040065c: cmp      r7, r1, asr #2
00400660: mov      r6, r2
00400664: mov      sl, r3
00400668: blo      #0x400690
0040066c: ldr      r3, [pc, #0x350]
00400670: ldr      r3, [r5, r3]
00400674: ldr      r3, [r3]
00400678: cmp      r3, #2
0040067c: moveq    r3, #0
00400680: streq    r3, [r3]
00400684: beq      #0x400690
00400688: cmp      r3, #1
0040068c: beq      #0x400894
00400690: ldr      r3, [r4, #8]
00400694: ldr      r2, [r4, #0xc]
00400698: rsb      r2, r3, r2
0040069c: cmp      r6, r2, asr #2
004006a0: blo      #0x400714
004006a4: ldr      r3, [pc, #0x318]
004006a8: ldr      r3, [r5, r3]
004006ac: ldr      r3, [r3]
004006b0: cmp      r3, #2
004006b4: moveq    r3, #0
004006b8: streq    r3, [r3]
004006bc: beq      #0x4006c8
004006c0: cmp      r3, #1
004006c4: beq      #0x4006d0
004006c8: add      sp, sp, #0x14
004006cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004006d0: ldr      r0, [pc, #0x2f0]
004006d4: ldr      r1, [pc, #0x2f0]
004006d8: ldr      r2, [pc, #0x2f0]
004006dc: ldr      r0, [r5, r0]
004006e0: ldr      r3, [pc, #0x2ec]
004006e4: add      r2, pc, r2
004006e8: mov      ip, #0xbd
004006ec: add      r3, pc, r3
004006f0: add      r1, pc, r1
004006f4: add      r0, r0, #0xa8
004006f8: str      ip, [sp]
004006fc: bl       #0x30e004
00400700: ldr      r3, [r4, #8]
00400704: ldr      r2, [r4, #0xc]
00400708: rsb      r2, r3, r2
0040070c: cmp      r6, r2, asr #2
00400710: bhs      #0x4006c8
00400714: ldr      r6, [r3, r6, lsl #2]
00400718: cmp      r6, #0
0040071c: beq      #0x4006c8
00400720: ldr      r3, [r6]
00400724: cmp      r3, #0
00400728: beq      #0x4006c8
0040072c: mov      r1, r7
00400730: mov      r0, r4
00400734: bl       #0x3fc6a8
00400738: mov      r8, r0
0040073c: ldr      r0, [r6]
00400740: bl       #0x3f9e08
00400744: ldr      sb, [r0, #0x68]
00400748: ldr      r0, [r6]
0040074c: ldr      fp, [r4, #4]
00400750: bl       #0x3f9e08
00400754: ldr      r3, [r0, #0x58]
00400758: cmp      r3, #5
0040075c: beq      #0x40077c
00400760: ldr      r0, [r6]
00400764: bl       #0x3f9e08
00400768: ldr      r3, [r0, #0x58]
0040076c: cmp      r3, #4
00400770: beq      #0x40077c
00400774: cmn      sb, #4
00400778: beq      #0x400880
0040077c: ldr      r0, [r6]
00400780: bl       #0x3f9e68
00400784: cmp      r0, #0
00400788: beq      #0x4006c8
0040078c: add      fp, r6, r8
00400790: ldrsb    r3, [fp, #4]
00400794: cmp      r3, r7
00400798: beq      #0x400858
0040079c: mov      r3, #0xc
004007a0: mul      r3, r3, r8
004007a4: str      r3, [sp, #0xc]
004007a8: mov      r1, r7
004007ac: mov      r0, r4
004007b0: mvn      r2, #0
004007b4: bl       #0x4003a4
004007b8: ldrsb    r1, [fp, #4]
004007bc: cmn      r1, #1
004007c0: beq      #0x4007dc
004007c4: ldr      r3, [r4, #0x14]
004007c8: ldr      r2, [sp, #0xc]
004007cc: ldr      r3, [r3, r2]
004007d0: ldr      r3, [r3, r1, lsl #2]
004007d4: cmp      r3, r6
004007d8: beq      #0x400914
004007dc: cmn      sb, #4
004007e0: beq      #0x4008f0
004007e4: cmp      r7, #2
004007e8: beq      #0x400924
004007ec: lsl      sl, r7, #2
004007f0: uxtb     r7, r7
004007f4: ldr      r0, [r6]
004007f8: ldrsh    r1, [r0, #0x50]
004007fc: cmp      r1, #1
00400800: beq      #0x4008c8
00400804: sub      r1, r1, #1
00400808: bl       #0x3fc3e0
0040080c: subs     sb, r0, #0
00400810: beq      #0x400958
00400814: ldr      r3, [r4, #0x14]
00400818: ldr      r1, [sp, #0xc]
0040081c: mov      r0, r4
00400820: mov      r2, #1
00400824: ldr      ip, [r3, r1]
00400828: mov      r1, sb
0040082c: mov      r3, r2
00400830: str      r6, [ip, sl]
00400834: ldr      ip, [r4, #0x14]
00400838: ldr      r4, [sp, #0xc]
0040083c: ldr      ip, [ip, r4]
00400840: ldr      ip, [ip, sl]
00400844: add      r8, ip, r8
00400848: strb     r7, [r8, #4]
0040084c: add      sp, sp, #0x14
00400850: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00400854: b        #0x3ff5d4
00400858: mov      r3, #0xc
0040085c: mul      r3, r3, r8
00400860: str      r3, [sp, #0xc]
00400864: ldr      r3, [r4, #0x14]
00400868: ldr      r1, [sp, #0xc]
0040086c: ldr      r3, [r3, r1]
00400870: ldr      r3, [r3, r7, lsl #2]
00400874: cmp      r3, r6
00400878: bne      #0x4007a8
0040087c: b        #0x4006c8
00400880: movw     r3, #0x1324
00400884: ldr      r3, [fp, r3]
00400888: cmp      r3, #0
0040088c: movne    sb, #1
00400890: b        #0x40077c
00400894: ldr      r0, [pc, #0x12c]
00400898: ldr      r1, [pc, #0x138]
0040089c: ldr      r2, [pc, #0x138]
004008a0: ldr      r0, [r5, r0]
004008a4: ldr      r3, [pc, #0x134]
004008a8: mov      ip, #0xbc
004008ac: add      r1, pc, r1
004008b0: add      r2, pc, r2
004008b4: add      r3, pc, r3
004008b8: add      r0, r0, #0xa8
004008bc: str      ip, [sp]
004008c0: bl       #0x30e004
004008c4: b        #0x400690
004008c8: ldr      r1, [sp, #0xc]
004008cc: ldr      r3, [r4, #0x14]
004008d0: ldr      r3, [r3, r1]
004008d4: str      r6, [r3, sl]
004008d8: ldr      r3, [r4, #0x14]
004008dc: ldr      r3, [r3, r1]
004008e0: ldr      r3, [r3, sl]
004008e4: add      r8, r3, r8
004008e8: strb     r7, [r8, #4]
004008ec: b        #0x4006c8
004008f0: cmp      sl, #0
004008f4: beq      #0x4009ac
004008f8: mov      r0, r4
004008fc: mov      r1, #1
00400900: mvn      r2, #0
00400904: bl       #0x4003a4
00400908: mov      sl, #4
0040090c: mov      r7, #1
00400910: b        #0x4007f4
00400914: mov      r0, r4
00400918: mvn      r2, #0
0040091c: bl       #0x4003a4
00400920: b        #0x4007dc
00400924: mov      r0, r4
00400928: mov      r1, #0
0040092c: bl       #0x4001a0
00400930: cmp      r0, #0
00400934: beq      #0x4007ec
00400938: cmp      sl, #0
0040093c: bne      #0x4007ec
00400940: mov      r0, r4
00400944: mov      r1, #1
00400948: mvn      r2, #0
0040094c: bl       #0x4003a4
00400950: mov      sl, #8
00400954: b        #0x4007f4
00400958: ldr      r3, [pc, #0x64]
0040095c: ldr      r3, [r5, r3]
00400960: ldr      r3, [r3]
00400964: cmp      r3, #2
00400968: streq    sb, [sb]
0040096c: beq      #0x400814
00400970: cmp      r3, #1
00400974: bne      #0x400814
00400978: ldr      r0, [pc, #0x48]
0040097c: ldr      r1, [pc, #0x60]
00400980: ldr      r2, [pc, #0x60]
00400984: ldr      r0, [r5, r0]
00400988: ldr      r3, [pc, #0x5c]
0040098c: mov      ip, #0x100
00400990: add      r1, pc, r1
00400994: add      r2, pc, r2
00400998: add      r3, pc, r3
0040099c: add      r0, r0, #0xa8
004009a0: str      ip, [sp]
004009a4: bl       #0x30e004
004009a8: b        #0x400814
004009ac: mov      r0, r4
004009b0: mov      r1, #2
004009b4: mvn      r2, #0
004009b8: bl       #0x4003a4
004009bc: b        #0x4008f8
004009c0: subseq   r4, sb, r0, asr #8
004009c4: andeq    r3, r0, r0, asr #19
004009c8: andeq    r1, r0, r0, asr #19
004009cc: subeq    sp, fp, r8, ror #25
004009d0: subeq    r6, ip, r4, lsr #25
004009d4: subeq    r6, ip, ip, ror #27
004009d8: subeq    sp, fp, ip, lsr #22
004009dc: subeq    r6, ip, r0, asr #22
004009e0: subeq    r6, ip, r4, lsr #24
004009e4: subeq    sp, fp, r8, asr #20
004009e8: umaaleq  r6, ip, r4, fp
004009ec: subeq    r6, ip, r0, asr #22

# _ZNK13ItemInventory12HasTwoHanderEb
004001a0: push     {r4, r5, r6, lr}
004001a4: mov      r5, r1
004001a8: mov      r1, #1
004001ac: mov      r4, r0
004001b0: bl       #0x3fc6a8
004001b4: mov      r3, #0xc
004001b8: mul      r3, r3, r0
004001bc: ldr      r2, [r4, #0x14]
004001c0: ldr      r3, [r2, r3]
004001c4: ldr      r0, [r3, #4]
004001c8: cmp      r0, #0
004001cc: beq      #0x400218
004001d0: ldr      r0, [r0]
004001d4: bl       #0x3f9e08
004001d8: ldr      r3, [r0, #0x58]
004001dc: ldr      r2, [r4, #4]
004001e0: ldr      r0, [r0, #0x68]
004001e4: sub      r3, r3, #4
004001e8: cmp      r3, #1
004001ec: bls      #0x40021c
004001f0: cmp      r5, #0
004001f4: bne      #0x40021c
004001f8: cmn      r0, #4
004001fc: beq      #0x400208
00400200: mov      r0, r5
00400204: pop      {r4, r5, r6, pc}
00400208: movw     r3, #0x1324
0040020c: ldr      r0, [r2, r3]
00400210: rsbs     r0, r0, #1
00400214: movlo    r0, #0
00400218: pop      {r4, r5, r6, pc}
0040021c: cmn      r0, #4
00400220: movne    r0, #0
00400224: moveq    r0, #1
00400228: pop      {r4, r5, r6, pc}

# _ZN13ItemInventory20_UnEquipItemFromSlotEji
004003a4: push     {r4, r5, r6, r7, lr}
004003a8: mov      r6, r0
004003ac: ldr      r0, [r0, #0x14]
004003b0: mov      r4, r1
004003b4: ldr      r3, [pc, #0x138]
004003b8: ldr      r1, [r0]
004003bc: ldr      r0, [r0, #4]
004003c0: sub      sp, sp, #0x14
004003c4: add      r3, pc, r3
004003c8: rsb      r1, r1, r0
004003cc: cmp      r4, r1, asr #2
004003d0: mov      r5, r2
004003d4: blo      #0x4003fc
004003d8: ldr      r2, [pc, #0x118]
004003dc: ldr      r2, [r3, r2]
004003e0: ldr      r2, [r2]
004003e4: cmp      r2, #2
004003e8: moveq    r3, #0
004003ec: streq    r3, [r3]
004003f0: beq      #0x4003fc
004003f4: cmp      r2, #1
004003f8: beq      #0x4004c0
004003fc: cmn      r5, #1
00400400: beq      #0x4004ac
00400404: mov      r3, #0xc
00400408: ldr      r2, [r6, #0x14]
0040040c: mul      r3, r3, r5
00400410: ldr      r3, [r2, r3]
00400414: mov      r2, #0
00400418: ldr      r7, [r3, r4, lsl #2]
0040041c: str      r2, [r3, r4, lsl #2]
00400420: cmp      r7, r2
00400424: beq      #0x400440
00400428: mvn      r3, #0
0040042c: add      r5, r7, r5
00400430: strb     r3, [r5, #4]
00400434: ldrsh    r3, [r7, #4]
00400438: cmn      r3, #1
0040043c: beq      #0x400448
00400440: add      sp, sp, #0x14
00400444: pop      {r4, r5, r6, r7, pc}
00400448: ldr      r0, [r7]
0040044c: bl       #0x3f9e58
00400450: cmp      r0, #0
00400454: beq      #0x400440
00400458: mov      r0, r6
0040045c: ldr      r1, [r7]
00400460: add      r2, sp, #0xc
00400464: bl       #0x3fe1cc
00400468: cmp      r0, #0
0040046c: beq      #0x400440
00400470: mov      r0, r6
00400474: ldr      r1, [sp, #0xc]
00400478: bl       #0x3fdaf0
0040047c: cmp      r0, #0
00400480: bne      #0x400440
00400484: ldr      r1, [sp, #0xc]
00400488: mov      r0, r6
0040048c: bl       #0x3fc61c
00400490: ldr      r3, [r7]
00400494: ldrsh    r1, [r3, #0x50]
00400498: bl       #0x3fa17c
0040049c: mov      r0, r6
004004a0: ldr      r1, [r7]
004004a4: bl       #0x3fe7d8
004004a8: b        #0x400440
004004ac: mov      r0, r6
004004b0: mov      r1, r4
004004b4: bl       #0x3fc6a8
004004b8: mov      r5, r0
004004bc: b        #0x400404
004004c0: ldr      r0, [pc, #0x34]
004004c4: ldr      r1, [pc, #0x34]
004004c8: ldr      r2, [pc, #0x34]
004004cc: ldr      r0, [r3, r0]
004004d0: ldr      r3, [pc, #0x30]
004004d4: movw     ip, #0x111
004004d8: add      r1, pc, r1
004004dc: add      r2, pc, r2
004004e0: add      r3, pc, r3
004004e4: add      r0, r0, #0xa8
004004e8: str      ip, [sp]
004004ec: bl       #0x30e004
004004f0: b        #0x4003fc
004004f4: subseq   r4, sb, ip, asr #13
004004f8: andeq    r3, r0, r0, asr #19
004004fc: andeq    r1, r0, r0, asr #19
00400500: subeq    sp, fp, r0, lsl #30
00400504: subeq    r6, ip, r4, lsl pc
00400508: strdeq   r6, r7, [ip], #-0xf8

# _ZN12ItemInstance12_UpdateStatsEv
003fb290: push     {r4, r5, r6, r7, r8, sl, lr}
003fb294: ldr      r1, [pc, #0x1d8]
003fb298: add      r5, r0, #0x20
003fb29c: sub      sp, sp, #0xc
003fb2a0: add      r1, pc, r1
003fb2a4: mov      r2, r1
003fb2a8: mov      r4, r0
003fb2ac: mov      r0, r5
003fb2b0: bl       #0x3109e0
003fb2b4: mov      r0, r4
003fb2b8: bl       #0x3f9e08
003fb2bc: ldr      r2, [r0, #0x58]
003fb2c0: ldr      r3, [pc, #0x1b0]
003fb2c4: cmp      r2, #0xc
003fb2c8: add      r3, pc, r3
003fb2cc: bhi      #0x3fb3c0
003fb2d0: mov      r1, #1
003fb2d4: lsl      r2, r1, r2
003fb2d8: tst      r2, #0x1780
003fb2dc: beq      #0x3fb340
003fb2e0: ldr      r2, [pc, #0x194]
003fb2e4: ldr      r1, [pc, #0x194]
003fb2e8: ldr      r6, [r3, r2]
003fb2ec: ldr      r2, [pc, #0x190]
003fb2f0: add      r1, pc, r1
003fb2f4: ldr      r0, [r6, #0x2c]
003fb2f8: add      r2, pc, r2
003fb2fc: ldr      r7, [r6, #0x34]
003fb300: bl       #0x4c4bdc
003fb304: mov      r1, r0
003fb308: mov      r0, r7
003fb30c: bl       #0x508edc
003fb310: mov      r7, r0
003fb314: mov      r0, r4
003fb318: ldr      r4, [r6, #0x34]
003fb31c: bl       #0x3f9e08
003fb320: ldr      r3, [r0, #0x8c]
003fb324: mov      r1, r5
003fb328: mov      r0, r4
003fb32c: mov      r2, r7
003fb330: asr      r3, r3, #8
003fb334: add      sp, sp, #0xc
003fb338: pop      {r4, r5, r6, r7, r8, sl, lr}
003fb33c: b        #0x508ef4
003fb340: tst      r2, #0x40
003fb344: bne      #0x3fb3c8
003fb348: tst      r2, #0x3f
003fb34c: beq      #0x3fb3c0
003fb350: ldr      r2, [pc, #0x124]
003fb354: ldr      r1, [pc, #0x12c]
003fb358: ldr      r6, [r3, r2]
003fb35c: ldr      r2, [pc, #0x128]
003fb360: add      r1, pc, r1
003fb364: ldr      r0, [r6, #0x2c]
003fb368: add      r2, pc, r2
003fb36c: ldr      r7, [r6, #0x34]
003fb370: bl       #0x4c4bdc
003fb374: mov      r1, r0
003fb378: mov      r0, r7
003fb37c: bl       #0x508edc
003fb380: mov      r7, r0
003fb384: mov      r0, r4
003fb388: ldr      r6, [r6, #0x34]
003fb38c: bl       #0x3f9e08
003fb390: ldr      r3, [r0, #0x8c]
003fb394: mov      r0, r4
003fb398: asr      r4, r3, #8
003fb39c: bl       #0x3f9e08
003fb3a0: ldr      ip, [r0, #0x90]
003fb3a4: mov      r1, r5
003fb3a8: mov      r0, r6
003fb3ac: asr      ip, ip, #8
003fb3b0: mov      r2, r7
003fb3b4: mov      r3, r4
003fb3b8: str      ip, [sp]
003fb3bc: bl       #0x508ef4
003fb3c0: add      sp, sp, #0xc
003fb3c4: pop      {r4, r5, r6, r7, r8, sl, pc}
003fb3c8: ldr      r2, [pc, #0xac]
003fb3cc: ldr      r7, [pc, #0xbc]
003fb3d0: ldr      r6, [r3, r2]
003fb3d4: ldr      r2, [pc, #0xb8]
003fb3d8: add      r7, pc, r7
003fb3dc: mov      r1, r7
003fb3e0: add      r2, pc, r2
003fb3e4: ldr      r0, [r6, #0x2c]
003fb3e8: ldr      r8, [r6, #0x34]
003fb3ec: bl       #0x4c4bdc
003fb3f0: mov      r1, r0
003fb3f4: mov      r0, r8
003fb3f8: bl       #0x508edc
003fb3fc: mov      sl, r0
003fb400: mov      r0, r4
003fb404: ldr      r8, [r6, #0x34]
003fb408: bl       #0x3f9e08
003fb40c: ldr      r3, [r0, #0x8c]
003fb410: mov      r2, sl
003fb414: mov      r1, r5
003fb418: asr      r3, r3, #8
003fb41c: mov      r0, r8
003fb420: bl       #0x508ef4
003fb424: ldr      r1, [pc, #0x6c]
003fb428: mov      r0, r5
003fb42c: add      r1, pc, r1
003fb430: add      r2, r1, #1
003fb434: bl       #0x310804
003fb438: ldr      r2, [pc, #0x5c]
003fb43c: mov      r1, r7
003fb440: ldr      r0, [r6, #0x2c]
003fb444: add      r2, pc, r2
003fb448: ldr      r7, [r6, #0x34]
003fb44c: bl       #0x4c4bdc
003fb450: mov      r1, r0
003fb454: mov      r0, r7
003fb458: bl       #0x508edc
003fb45c: mov      r7, r0
003fb460: mov      r0, r4
003fb464: ldr      r4, [r6, #0x34]
003fb468: bl       #0x3f9e08
003fb46c: ldr      r3, [r0, #0x90]
003fb470: b        #0x3fb324
003fb474: subeq    r0, sp, r8, ror #10
003fb478: subseq   sb, sb, r8, asr #15
003fb47c: strdeq   r3, r4, [r0], -r4
003fb480: subeq    r3, ip, r8, lsr sb
003fb484: subeq    fp, ip, r8, lsr #29
003fb488: subeq    r3, ip, r8, asr #17
003fb48c: subeq    fp, ip, r8, lsl lr
003fb490: subeq    r3, ip, r0, asr r8
003fb494: subeq    fp, ip, r0, asr #27
003fb498: subeq    r6, ip, r4
003fb49c: subeq    fp, ip, ip, ror sp

# _ZN12ItemInstance11_UpdateReqsEv
003facdc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003face0: ldr      r1, [pc, #0x560]
003face4: add      r6, r0, #0x38
003face8: sub      sp, sp, #4
003facec: add      r1, pc, r1
003facf0: mov      r4, r0
003facf4: mov      r2, r1
003facf8: mov      r0, r6
003facfc: bl       #0x3109e0
003fad00: mov      r0, r4
003fad04: bl       #0x3f9e08
003fad08: ldr      r3, [r0, #0x74]
003fad0c: ldr      r5, [pc, #0x538]
003fad10: cmp      r3, #0
003fad14: add      r5, pc, r5
003fad18: bne      #0x3fad30
003fad1c: mov      r0, r4
003fad20: bl       #0x3f9e08
003fad24: ldr      r3, [r0, #0x78]
003fad28: cmp      r3, #0
003fad2c: beq      #0x3fb01c
003fad30: mov      r0, r4
003fad34: bl       #0x3f9e08
003fad38: ldr      r3, [r0, #0x58]
003fad3c: cmp      r3, #0xe
003fad40: beq      #0x3fb06c
003fad44: ldr      r7, [pc, #0x504]
003fad48: ldr      sl, [pc, #0x504]
003fad4c: ldr      r2, [pc, #0x504]
003fad50: ldr      r8, [r5, r7]
003fad54: add      sl, pc, sl
003fad58: add      r2, pc, r2
003fad5c: ldr      r0, [r8, #0x2c]
003fad60: mov      r1, sl
003fad64: ldr      sb, [r8, #0x34]
003fad68: bl       #0x4c4bdc
003fad6c: mov      r1, r0
003fad70: mov      r0, sb
003fad74: bl       #0x508edc
003fad78: mov      sb, r0
003fad7c: bl       #0x30de54
003fad80: mov      r1, sb
003fad84: add      r2, sb, r0
003fad88: mov      r0, r6
003fad8c: bl       #0x3109e0
003fad90: ldr      r2, [pc, #0x4c4]
003fad94: ldr      r0, [r8, #0x2c]
003fad98: mov      r1, sl
003fad9c: add      r2, pc, r2
003fada0: ldr      sb, [r8, #0x34]
003fada4: bl       #0x4c4bdc
003fada8: mov      r1, r0
003fadac: mov      r0, sb
003fadb0: bl       #0x508edc
003fadb4: mov      sb, r0
003fadb8: mov      r0, r4
003fadbc: bl       #0x3f9e08
003fadc0: ldr      r3, [r0, #0x74]
003fadc4: cmp      r3, #0
003fadc8: moveq    r8, r3
003fadcc: bne      #0x3fb0d0
003fadd0: mov      r0, r4
003fadd4: bl       #0x3f9e08
003fadd8: ldr      r3, [r0, #0x78]
003faddc: cmp      r3, #0
003fade0: beq      #0x3fae40
003fade4: cmp      r8, #0
003fade8: bne      #0x3fb120
003fadec: ldr      r8, [r5, r7]
003fadf0: ldr      r1, [pc, #0x468]
003fadf4: ldr      r2, [pc, #0x468]
003fadf8: ldr      r0, [r8, #0x2c]
003fadfc: add      r1, pc, r1
003fae00: add      r2, pc, r2
003fae04: ldr      sl, [r8, #0x34]
003fae08: bl       #0x4c4bdc
003fae0c: mov      r1, r0
003fae10: mov      r0, sl
003fae14: bl       #0x508edc
003fae18: mov      sl, r0
003fae1c: mov      r0, r4
003fae20: ldr      r8, [r8, #0x34]
003fae24: bl       #0x3f9e08
003fae28: mov      r2, sl
003fae2c: ldr      r3, [r0, #0x78]
003fae30: mov      r1, r6
003fae34: mov      r0, r8
003fae38: bl       #0x508ef4
003fae3c: mov      r8, #1
003fae40: mov      r0, r4
003fae44: bl       #0x3f9e08
003fae48: ldr      r3, [r0, #0x7c]
003fae4c: cmp      r3, #0
003fae50: beq      #0x3faeb0
003fae54: cmp      r8, #0
003fae58: bne      #0x3fb150
003fae5c: ldr      r8, [r5, r7]
003fae60: ldr      r1, [pc, #0x400]
003fae64: ldr      r2, [pc, #0x400]
003fae68: ldr      r0, [r8, #0x2c]
003fae6c: add      r1, pc, r1
003fae70: add      r2, pc, r2
003fae74: ldr      sl, [r8, #0x34]
003fae78: bl       #0x4c4bdc
003fae7c: mov      r1, r0
003fae80: mov      r0, sl
003fae84: bl       #0x508edc
003fae88: mov      sl, r0
003fae8c: mov      r0, r4
003fae90: ldr      r8, [r8, #0x34]
003fae94: bl       #0x3f9e08
003fae98: mov      r2, sl
003fae9c: ldr      r3, [r0, #0x7c]
003faea0: mov      r1, r6
003faea4: mov      r0, r8
003faea8: bl       #0x508ef4
003faeac: mov      r8, #1
003faeb0: mov      r0, r4
003faeb4: bl       #0x3f9e08
003faeb8: ldr      r3, [r0, #0x80]
003faebc: cmp      r3, #0
003faec0: beq      #0x3faf20
003faec4: cmp      r8, #0
003faec8: bne      #0x3fb130
003faecc: ldr      r8, [r5, r7]
003faed0: ldr      r1, [pc, #0x398]
003faed4: ldr      r2, [pc, #0x398]
003faed8: ldr      r0, [r8, #0x2c]
003faedc: add      r1, pc, r1
003faee0: add      r2, pc, r2
003faee4: ldr      sl, [r8, #0x34]
003faee8: bl       #0x4c4bdc
003faeec: mov      r1, r0
003faef0: mov      r0, sl
003faef4: bl       #0x508edc
003faef8: mov      sl, r0
003faefc: mov      r0, r4
003faf00: ldr      r8, [r8, #0x34]
003faf04: bl       #0x3f9e08
003faf08: mov      r2, sl
003faf0c: ldr      r3, [r0, #0x80]
003faf10: mov      r1, r6
003faf14: mov      r0, r8
003faf18: bl       #0x508ef4
003faf1c: mov      r8, #1
003faf20: mov      r0, r4
003faf24: bl       #0x3f9e08
003faf28: ldr      r3, [r0, #0x84]
003faf2c: cmp      r3, #0
003faf30: beq      #0x3faf90
003faf34: cmp      r8, #0
003faf38: bne      #0x3fb140
003faf3c: ldr      r8, [r5, r7]
003faf40: ldr      r1, [pc, #0x330]
003faf44: ldr      r2, [pc, #0x330]
003faf48: ldr      r0, [r8, #0x2c]
003faf4c: add      r1, pc, r1
003faf50: add      r2, pc, r2
003faf54: ldr      sl, [r8, #0x34]
003faf58: bl       #0x4c4bdc
003faf5c: mov      r1, r0
003faf60: mov      r0, sl
003faf64: bl       #0x508edc
003faf68: mov      sl, r0
003faf6c: mov      r0, r4
003faf70: ldr      r8, [r8, #0x34]
003faf74: bl       #0x3f9e08
003faf78: mov      r2, sl
003faf7c: ldr      r3, [r0, #0x84]
003faf80: mov      r1, r6
003faf84: mov      r0, r8
003faf88: bl       #0x508ef4
003faf8c: mov      r8, #1
003faf90: mov      r0, r4
003faf94: bl       #0x3f9e08
003faf98: ldr      r3, [r0, #0x88]
003faf9c: cmp      r3, #0
003fafa0: beq      #0x3fb06c
003fafa4: cmp      r8, #0
003fafa8: bne      #0x3fb0c0
003fafac: ldr      r3, [r5, r7]
003fafb0: ldr      r1, [pc, #0x2c8]
003fafb4: ldr      r2, [pc, #0x2c8]
003fafb8: ldr      r0, [r3, #0x2c]
003fafbc: add      r1, pc, r1
003fafc0: add      r2, pc, r2
003fafc4: ldr      r8, [r3, #0x34]
003fafc8: bl       #0x4c4bdc
003fafcc: mov      r1, r0
003fafd0: mov      r0, r8
003fafd4: bl       #0x508edc
003fafd8: mov      r8, r0
003fafdc: mov      r0, r4
003fafe0: bl       #0x3f9e08
003fafe4: ldr      r3, [r0, #0x88]
003fafe8: sub      r3, r3, #1
003fafec: cmp      r3, #8
003faff0: addls    pc, pc, r3, lsl #2
003faff4: b        #0x3fb090
003faff8: b        #0x3fb1bc
003faffc: b        #0x3fb1d8
003fb000: b        #0x3fb160
003fb004: b        #0x3fb17c
003fb008: b        #0x3fb074
003fb00c: b        #0x3fb19c
003fb010: b        #0x3fb210
003fb014: b        #0x3fb22c
003fb018: b        #0x3fb1f4
003fb01c: mov      r0, r4
003fb020: bl       #0x3f9e08
003fb024: ldr      r3, [r0, #0x7c]
003fb028: cmp      r3, #0
003fb02c: bne      #0x3fad30
003fb030: mov      r0, r4
003fb034: bl       #0x3f9e08
003fb038: ldr      r3, [r0, #0x80]
003fb03c: cmp      r3, #0
003fb040: bne      #0x3fad30
003fb044: mov      r0, r4
003fb048: bl       #0x3f9e08
003fb04c: ldr      r3, [r0, #0x84]
003fb050: cmp      r3, #0
003fb054: bne      #0x3fad30
003fb058: mov      r0, r4
003fb05c: bl       #0x3f9e08
003fb060: ldr      r3, [r0, #0x88]
003fb064: cmp      r3, #0
003fb068: bne      #0x3fad30
003fb06c: add      sp, sp, #4
003fb070: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003fb074: ldr      r3, [pc, #0x20c]
003fb078: ldr      r3, [r5, r3]
003fb07c: ldr      r3, [r3]
003fb080: add      r3, r3, #0x47000
003fb084: add      r3, r3, #0xd90
003fb088: add      r3, r3, #0xc
003fb08c: ldr      fp, [r3, #0x18]
003fb090: ldr      r3, [r5, r7]
003fb094: mov      r1, fp
003fb098: ldr      r4, [r3, #0x34]
003fb09c: mov      r0, r4
003fb0a0: bl       #0x508edc
003fb0a4: mov      r1, r6
003fb0a8: mov      r3, r0
003fb0ac: mov      r2, r8
003fb0b0: mov      r0, r4
003fb0b4: add      sp, sp, #4
003fb0b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003fb0bc: b        #0x508ef4
003fb0c0: mov      r1, sb
003fb0c4: mov      r0, r6
003fb0c8: bl       #0x379ef8
003fb0cc: b        #0x3fafac
003fb0d0: ldr      r2, [pc, #0x1b4]
003fb0d4: mov      r1, sl
003fb0d8: ldr      r0, [r8, #0x2c]
003fb0dc: add      r2, pc, r2
003fb0e0: ldr      sl, [r8, #0x34]
003fb0e4: bl       #0x4c4bdc
003fb0e8: mov      r1, r0
003fb0ec: mov      r0, sl
003fb0f0: bl       #0x508edc
003fb0f4: mov      sl, r0
003fb0f8: mov      r0, r4
003fb0fc: ldr      r8, [r8, #0x34]
003fb100: bl       #0x3f9e08
003fb104: mov      r2, sl
003fb108: ldr      r3, [r0, #0x74]
003fb10c: mov      r1, r6
003fb110: mov      r0, r8
003fb114: bl       #0x508ef4
003fb118: mov      r8, #1
003fb11c: b        #0x3fadd0
003fb120: mov      r0, r6
003fb124: mov      r1, sb
003fb128: bl       #0x379ef8
003fb12c: b        #0x3fadec
003fb130: mov      r0, r6
003fb134: mov      r1, sb
003fb138: bl       #0x379ef8
003fb13c: b        #0x3faecc
003fb140: mov      r0, r6
003fb144: mov      r1, sb
003fb148: bl       #0x379ef8
003fb14c: b        #0x3faf3c
003fb150: mov      r0, r6
003fb154: mov      r1, sb
003fb158: bl       #0x379ef8
003fb15c: b        #0x3fae5c
003fb160: ldr      r3, [pc, #0x120]
003fb164: ldr      r3, [r5, r3]
003fb168: ldr      r3, [r3]
003fb16c: add      r3, r3, #0x3a000
003fb170: add      r3, r3, #0x3a4
003fb174: ldr      fp, [r3, #0x18]
003fb178: b        #0x3fb090
003fb17c: ldr      r3, [pc, #0x104]
003fb180: ldr      r3, [r5, r3]
003fb184: ldr      r3, [r3]
003fb188: add      r3, r3, #0x47000
003fb18c: add      r3, r3, #0x690
003fb190: add      r3, r3, #4
003fb194: ldr      fp, [r3, #0x18]
003fb198: b        #0x3fb090
003fb19c: ldr      r3, [pc, #0xe4]
003fb1a0: ldr      r3, [r5, r3]
003fb1a4: ldr      r3, [r3]
003fb1a8: add      r3, r3, #0x47000
003fb1ac: add      r3, r3, #0xa10
003fb1b0: add      r3, r3, #8
003fb1b4: ldr      fp, [r3, #0x18]
003fb1b8: b        #0x3fb090
003fb1bc: ldr      r3, [pc, #0xc4]
003fb1c0: ldr      r3, [r5, r3]
003fb1c4: ldr      r3, [r3]
003fb1c8: add      r3, r3, #0x39c00
003fb1cc: add      r3, r3, #0x9c
003fb1d0: ldr      fp, [r3, #0x18]
003fb1d4: b        #0x3fb090
003fb1d8: ldr      r3, [pc, #0xa8]
003fb1dc: ldr      r3, [r5, r3]
003fb1e0: ldr      r3, [r3]
003fb1e4: add      r3, r3, #0x3a000
003fb1e8: add      r3, r3, #0x20
003fb1ec: ldr      fp, [r3, #0x18]
003fb1f0: b        #0x3fb090
003fb1f4: ldr      r3, [pc, #0x8c]
003fb1f8: ldr      r3, [r5, r3]
003fb1fc: ldr      r3, [r3]
003fb200: add      r3, r3, #0x3fc00
003fb204: add      r3, r3, #0x30c
003fb208: ldr      fp, [r3, #0x18]
003fb20c: b        #0x3fb090
003fb210: ldr      r3, [pc, #0x70]
003fb214: ldr      r3, [r5, r3]
003fb218: ldr      r3, [r3]
003fb21c: add      r3, r3, #0x3f800
003fb220: add      r3, r3, #0x388
003fb224: ldr      fp, [r3, #0x18]
003fb228: b        #0x3fb090
003fb22c: ldr      r3, [pc, #0x54]
003fb230: ldr      r3, [r5, r3]
003fb234: ldr      r3, [r3]
003fb238: add      r3, r3, #0x40000
003fb23c: add      r3, r3, #0x290
003fb240: ldr      fp, [r3, #0x18]
003fb244: b        #0x3fb090
003fb248: subeq    r0, sp, ip, lsl fp
003fb24c: subseq   sb, sb, ip, ror sp
003fb250: strdeq   r3, r4, [r0], -r4
003fb254: ldrdeq   r3, r4, [ip], #-0xe4
003fb258: strdeq   ip, sp, [ip], #-0x38
003fb25c: subeq    ip, ip, ip, asr #7
003fb260: subeq    r3, ip, ip, lsr #28
003fb264: subeq    ip, ip, r0, asr #5
003fb268: strheq   r3, [ip], #-0xdc
003fb26c: subeq    ip, ip, r0, ror r2
003fb270: subeq    r3, ip, ip, asr #26
003fb274: subeq    ip, ip, r0, lsr #4
003fb278: ldrdeq   r3, r4, [ip], #-0xcc
003fb27c: ldrdeq   ip, sp, [ip], #-0x10
003fb280: subeq    r3, ip, ip, ror #24
003fb284: subeq    ip, ip, r8, ror r1
003fb288: andeq    r2, r0, r0, asr fp
003fb28c: subeq    fp, ip, ip, asr #31
