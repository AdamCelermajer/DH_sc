
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

# _ZN14ObjectSearcher12TargetSorter12_sortFrontalERKNS_10TargetInfoES3_
0038d5b4: push     {r4, lr}
0038d5b8: mov      r3, r0
0038d5bc: ldr      r2, [r3, #0xc]
0038d5c0: ldr      r0, [r1, #0xc]
0038d5c4: and      r2, r2, #1
0038d5c8: and      r0, r0, #1
0038d5cc: cmp      r2, r0
0038d5d0: beq      #0x38d5d8
0038d5d4: pop      {r4, pc}
0038d5d8: ldr      r0, [r3, #8]
0038d5dc: ldr      r1, [r1, #8]
0038d5e0: bl       #0x30e2f8
0038d5e4: cmp      r0, #0
0038d5e8: mov      r0, #0
0038d5ec: movne    r0, #1
0038d5f0: uxtb     r0, r0
0038d5f4: pop      {r4, pc}

# _ZN13ItemInventoryC1Ev
003ff200: ldr      r3, [pc, #0x120]
003ff204: ldr      r2, [pc, #0x120]
003ff208: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003ff20c: add      r3, pc, r3
003ff210: ldr      r2, [r3, r2]
003ff214: mov      r6, r0
003ff218: mov      r1, #0
003ff21c: add      r2, r2, #8
003ff220: str      r2, [r6]
003ff224: mvn      r2, #0x80000000
003ff228: sub      sp, sp, #0x10
003ff22c: add      r0, r0, #0x30
003ff230: str      r2, [r6, #0x28]
003ff234: mvn      r2, #0
003ff238: mov      r7, r1
003ff23c: strb     r2, [r6, #0x2c]
003ff240: str      r0, [r6, #0x34]
003ff244: str      r1, [r6, #4]
003ff248: str      r1, [r6, #8]
003ff24c: str      r1, [r6, #0xc]
003ff250: str      r1, [r6, #0x10]
003ff254: str      r1, [r6, #0x14]
003ff258: str      r1, [r6, #0x18]
003ff25c: str      r1, [r6, #0x1c]
003ff260: str      r1, [r6, #0x20]
003ff264: str      r1, [r6, #0x24]
003ff268: strb     r1, [r6, #0x2d]
003ff26c: strb     r1, [r6, #0x2e]
003ff270: strb     r1, [r6, #0x2f]
003ff274: str      r0, [r6, #0x30]
003ff278: add      sl, r6, #0x14
003ff27c: mov      r8, sp
003ff280: mov      r5, r1
003ff284: add      sb, sp, #0xc
003ff288: mov      r0, sl
003ff28c: mov      r1, sp
003ff290: str      r5, [sp]
003ff294: str      r5, [sp, #4]
003ff298: str      r5, [sp, #8]
003ff29c: bl       #0x3ff178
003ff2a0: ldr      r0, [sp]
003ff2a4: cmp      r0, #0
003ff2a8: beq      #0x3ff2c4
003ff2ac: ldr      r1, [sp, #8]
003ff2b0: rsb      r1, r0, r1
003ff2b4: bic      r1, r1, #3
003ff2b8: cmp      r1, #0x80
003ff2bc: bhi      #0x3ff320
003ff2c0: bl       #0x708f00
003ff2c4: mov      r4, #0
003ff2c8: ldr      r0, [r6, #0x14]
003ff2cc: str      r5, [sp, #0xc]
003ff2d0: add      r0, r0, r7
003ff2d4: ldmib    r0, {r1, r3}
003ff2d8: cmp      r1, r3
003ff2dc: beq      #0x3ff314
003ff2e0: str      r5, [r1]
003ff2e4: ldr      r3, [r0, #4]
003ff2e8: add      r3, r3, #4
003ff2ec: str      r3, [r0, #4]
003ff2f0: add      r4, r4, #1
003ff2f4: cmp      r4, #9
003ff2f8: bne      #0x3ff2c8
003ff2fc: add      r7, r7, #0xc
003ff300: cmp      r7, #0x18
003ff304: bne      #0x3ff288
003ff308: mov      r0, r6
003ff30c: add      sp, sp, #0x10
003ff310: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003ff314: mov      r2, sb
003ff318: bl       #0x3fef4c
003ff31c: b        #0x3ff2f0
003ff320: bl       #0x310440
003ff324: b        #0x3ff2c4
003ff328: subseq   r5, sb, r4, lsl #17
003ff32c: strdeq   r2, r3, [r0], -ip

# _ZN14ObjectSearcher12TargetSorter12_sortClosestERKNS_10TargetInfoES3_
0038d570: push     {r4, lr}
0038d574: mov      r3, r0
0038d578: ldr      r2, [r3, #0xc]
0038d57c: ldr      r0, [r1, #0xc]
0038d580: and      r2, r2, #1
0038d584: and      r0, r0, #1
0038d588: cmp      r2, r0
0038d58c: beq      #0x38d594
0038d590: pop      {r4, pc}
0038d594: ldr      r0, [r3, #4]
0038d598: ldr      r1, [r1, #4]
0038d59c: bl       #0x30e2f8
0038d5a0: cmp      r0, #0
0038d5a4: mov      r0, #0
0038d5a8: movne    r0, #1
0038d5ac: uxtb     r0, r0
0038d5b0: pop      {r4, pc}
