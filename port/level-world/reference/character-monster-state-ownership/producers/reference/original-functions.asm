
# _ZN5Level15_LoadCharStatesEv
003eff98: ldr      r3, [pc, #0x70]
003eff9c: ldr      r2, [pc, #0x70]
003effa0: push     {r4, r5, r6, lr}
003effa4: add      r3, pc, r3
003effa8: ldr      r2, [r3, r2]
003effac: ldr      r6, [r2, #0x38]
003effb0: ldr      r4, [r6, #0x60]!
003effb4: cmp      r6, r4
003effb8: beq      #0x3efff8
003effbc: ldr      r5, [r4, #8]
003effc0: subs     r0, r5, #0
003effc4: beq      #0x3effec
003effc8: bl       #0x3a5784
003effcc: subs     r3, r0, #0
003effd0: beq      #0x3efffc
003effd4: add      r0, r5, #0x4f0
003effd8: cmp      r3, #0x11
003effdc: add      r0, r0, #0xc
003effe0: mov      r1, #0
003effe4: beq      #0x3efffc
003effe8: bl       #0x3c1a00
003effec: ldr      r4, [r4]
003efff0: cmp      r6, r4
003efff4: bne      #0x3effbc
003efff8: pop      {r4, r5, r6, pc}
003efffc: add      r0, r5, #0x4f0
003f0000: add      r0, r0, #0xc
003f0004: bl       #0x3c1a64
003f0008: ldr      r4, [r4]
003f000c: b        #0x3efff0
003f0010: subseq   r4, sl, ip, ror #21
003f0014: strdeq   r3, r4, [r0], -r4

# _Z11GetNewStateI8CSAttackEP9CharStatev
003c04e0: ldr      r3, [pc, #0xc]
003c04e4: ldr      r2, [pc, #0xc]
003c04e8: add      r3, pc, r3
003c04ec: ldr      r0, [r3, r2]
003c04f0: bx       lr
003c04f4: subseq   r4, sp, r8, lsr #11
003c04f8: andeq    r0, r0, r4, lsr #16

# _ZNK16CharStateMachine9_HasStateEi
003c0084: ldr      r3, [r0, #0xc]
003c0088: add      r0, r0, #8
003c008c: cmp      r3, #0
003c0090: beq      #0x3c00d8
003c0094: mov      ip, r0
003c0098: b        #0x3c00a0
003c009c: mov      r3, r2
003c00a0: ldr      r2, [r3, #0x10]
003c00a4: cmp      r1, r2
003c00a8: ldrgt    r2, [r3, #0xc]
003c00ac: ldrle    r2, [r3, #8]
003c00b0: movgt    r3, ip
003c00b4: mov      ip, r3
003c00b8: cmp      r2, #0
003c00bc: bne      #0x3c009c
003c00c0: cmp      r0, r3
003c00c4: beq      #0x3c00d8
003c00c8: ldr      r3, [r3, #0x10]
003c00cc: cmp      r1, r3
003c00d0: movge    r0, #1
003c00d4: bxge     lr
003c00d8: mov      r0, #0
003c00dc: bx       lr

# _Z11GetNewStateI8CSLimbusEP9CharStatev
003c0454: ldr      r3, [pc, #0xc]
003c0458: ldr      r2, [pc, #0xc]
003c045c: add      r3, pc, r3
003c0460: ldr      r0, [r3, r2]
003c0464: bx       lr
003c0468: subseq   r4, sp, r4, lsr r6
003c046c: andeq    r0, r0, r0, lsl #15

# _Z11GetNewStateI6CSMoveEP9CharStatev
003c04c4: ldr      r3, [pc, #0xc]
003c04c8: ldr      r2, [pc, #0xc]
003c04cc: add      r3, pc, r3
003c04d0: ldr      r0, [r3, r2]
003c04d4: bx       lr
003c04d8: subseq   r4, sp, r4, asr #11
003c04dc: andeq    r4, r0, r8, asr #14

# _ZN9CharacterC1EN10ObjectBase6GO_IDSE
003aa1b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8: add      ip, r0, #0x374
003aa1bc: sub      sp, sp, #0x3c
003aa1c0: mov      r4, r0
003aa1c4: str      ip, [sp, #0xc]
003aa1c8: bl       #0x38c398
003aa1cc: ldr      ip, [sp, #0xc]
003aa1d0: add      r5, r4, #0x4f0
003aa1d4: add      r5, r5, #0xc
003aa1d8: mov      r0, ip
003aa1dc: bl       #0x404db8
003aa1e0: add      r0, r4, #0x3b4
003aa1e4: str      r0, [sp, #0x20]
003aa1e8: add      r0, r4, #0x37c
003aa1ec: bl       #0x3ff330
003aa1f0: add      r2, r4, #0x490
003aa1f4: add      r1, r4, #0x3c8
003aa1f8: add      r2, r2, #0xc
003aa1fc: ldr      r0, [sp, #0x20]
003aa200: str      r1, [sp, #0x1c]
003aa204: str      r2, [sp, #0x14]
003aa208: bl       #0x3dbb0c
003aa20c: ldr      r0, [sp, #0x1c]
003aa210: bl       #0x3cebf0
003aa214: ldr      r0, [sp, #0x14]
003aa218: bl       #0x3c8ff4
003aa21c: add      r3, r4, #0x560
003aa220: mov      r0, r5
003aa224: str      r3, [sp, #0x18]
003aa228: ldr      sb, [pc, #0x50c]
003aa22c: bl       #0x3c1b58
003aa230: ldr      r0, [sp, #0x18]
003aa234: bl       #0x3df084
003aa238: ldr      lr, [pc, #0x500]
003aa23c: add      sb, pc, sb
003aa240: mov      r8, #0
003aa244: ldr      lr, [sb, lr]
003aa248: mov      fp, #1
003aa24c: mvn      r6, #0
003aa250: add      sl, lr, #0x324
003aa254: str      sl, [sp, #0x34]
003aa258: add      sl, lr, #0x180
003aa25c: str      sl, [sp, #0x10]
003aa260: add      sl, lr, #0x1f4
003aa264: str      sl, [sp, #0x24]
003aa268: add      sl, lr, #0x220
003aa26c: str      sl, [sp, #0x28]
003aa270: add      sl, lr, #0x230
003aa274: str      sl, [sp, #0x2c]
003aa278: add      r0, lr, #8
003aa27c: add      r1, lr, #0x15c
003aa280: add      r2, lr, #0x168
003aa284: add      sl, lr, #0x304
003aa288: str      sl, [sp, #0x30]
003aa28c: stm      r4, {r0, r1}
003aa290: str      r2, [r4, #0x24]
003aa294: ldr      r0, [sp, #0x10]
003aa298: add      lr, lr, #0x314
003aa29c: add      r7, r4, #0x1380
003aa2a0: str      r0, [r4, #0x374]
003aa2a4: ldr      r1, [sp, #0x24]
003aa2a8: add      r3, r7, #0x18
003aa2ac: movw     sl, #0x13a8
003aa2b0: str      r1, [r4, #0x37c]
003aa2b4: ldr      r2, [sp, #0x28]
003aa2b8: add      r7, r7, #0x30
003aa2bc: str      r2, [r4, #0x3b4]
003aa2c0: ldr      r0, [sp, #0x2c]
003aa2c4: str      r0, [r4, #0x3c8]
003aa2c8: ldr      r1, [sp, #0x30]
003aa2cc: str      lr, [r4, #0x4fc]
003aa2d0: mov      r0, r3
003aa2d4: str      r1, [r4, #0x49c]
003aa2d8: ldr      r2, [sp, #0x34]
003aa2dc: mov      r1, #0x10
003aa2e0: str      r2, [r4, #0x560]
003aa2e4: movw     r2, #0x1394
003aa2e8: strb     r8, [r4, r2]
003aa2ec: movw     r2, #0x1395
003aa2f0: strb     r8, [r4, r2]
003aa2f4: movw     r2, #0x1396
003aa2f8: strb     fp, [r4, r2]
003aa2fc: movw     r2, #0x1397
003aa300: strb     r6, [r4, r2]
003aa304: movw     r2, #0x13ac
003aa308: str      r3, [r4, r2]
003aa30c: str      r3, [r4, sl]
003aa310: bl       #0x31167c
003aa314: ldr      r3, [r4, sl]
003aa318: mov      sl, #0x13c0
003aa31c: mov      r0, r7
003aa320: strb     r8, [r3]
003aa324: movw     r3, #0x13c4
003aa328: str      r7, [r4, r3]
003aa32c: mov      r1, #0x10
003aa330: str      r7, [r4, sl]
003aa334: bl       #0x31167c
003aa338: ldr      r2, [r4, sl]
003aa33c: add      r7, r4, sl
003aa340: add      r3, r7, #0xc
003aa344: strb     r8, [r2]
003aa348: movw     r2, #0x13c8
003aa34c: strh     r6, [r4, r2]
003aa350: movw     r2, #0x13ca
003aa354: strh     r6, [r4, r2]
003aa358: movw     sl, #0x13dc
003aa35c: movw     r2, #0x13e0
003aa360: str      r3, [r4, r2]
003aa364: mov      r0, r3
003aa368: str      r3, [r4, sl]
003aa36c: mov      r1, #0x10
003aa370: bl       #0x31167c
003aa374: ldr      r3, [r4, sl]
003aa378: add      r7, r7, #0x28
003aa37c: movw     sl, #0x13f8
003aa380: strb     r8, [r3]
003aa384: movw     r3, #0x13e4
003aa388: strb     fp, [r4, r3]
003aa38c: movw     r3, #0x13fc
003aa390: str      r7, [r4, r3]
003aa394: mov      r0, r7
003aa398: str      r7, [r4, sl]
003aa39c: mov      r1, #0x10
003aa3a0: bl       #0x31167c
003aa3a4: ldr      r3, [r4, sl]
003aa3a8: add      r7, r4, #0x1400
003aa3ac: movw     sl, #0x1410
003aa3b0: strb     r8, [r3]
003aa3b4: movw     r3, #0x1414
003aa3b8: str      r7, [r4, r3]
003aa3bc: mov      r0, r7
003aa3c0: str      r7, [r4, sl]
003aa3c4: mov      r1, #0x10
003aa3c8: bl       #0x31167c
003aa3cc: ldr      r3, [r4, sl]
003aa3d0: add      r7, r7, #0x18
003aa3d4: movw     sl, #0x1428
003aa3d8: strb     r8, [r3]
003aa3dc: movw     r3, #0x142c
003aa3e0: str      r7, [r4, r3]
003aa3e4: mov      r0, r7
003aa3e8: str      r7, [r4, sl]
003aa3ec: mov      r1, #0x10
003aa3f0: bl       #0x31167c
003aa3f4: ldr      r2, [r4, sl]
003aa3f8: mov      r3, #0
003aa3fc: mov      r1, #0xbf000000
003aa400: strb     r8, [r2]
003aa404: movw     r2, #0x14a8
003aa408: strb     r6, [r4, r2]
003aa40c: movw     r2, #0x1430
003aa410: strb     fp, [r4, r2]
003aa414: movw     r2, #0x1434
003aa418: str      r8, [r4, r2]
003aa41c: movw     r2, #0x1438
003aa420: str      r8, [r4, r2]
003aa424: movw     r2, #0x1448
003aa428: strb     fp, [r4, r2]
003aa42c: movw     r2, #0x1449
003aa430: strb     r8, [r4, r2]
003aa434: movw     r2, #0x144c
003aa438: str      r8, [r4, r2]
003aa43c: movw     r2, #0x1450
003aa440: str      r3, [r4, r2]
003aa444: movw     r2, #0x1454
003aa448: str      r3, [r4, r2]
003aa44c: movw     r2, #0x1458
003aa450: str      r3, [r4, r2]
003aa454: movw     r2, #0x145c
003aa458: str      r3, [r4, r2]
003aa45c: movw     r2, #0x1460
003aa460: str      r3, [r4, r2]
003aa464: movw     r2, #0x1464
003aa468: str      r3, [r4, r2]
003aa46c: movw     r2, #0x1468
003aa470: str      r3, [r4, r2]
003aa474: movw     r2, #0x146c
003aa478: str      r3, [r4, r2]
003aa47c: movw     r2, #0x1470
003aa480: str      r3, [r4, r2]
003aa484: movw     r2, #0x1474
003aa488: str      r3, [r4, r2]
003aa48c: movw     r2, #0x1478
003aa490: str      r3, [r4, r2]
003aa494: movw     r2, #0x147c
003aa498: str      r3, [r4, r2]
003aa49c: mov      r2, #0x1480
003aa4a0: strb     r8, [r4, r2]
003aa4a4: movw     r2, #0x1481
003aa4a8: strb     r8, [r4, r2]
003aa4ac: movw     r2, #0x1484
003aa4b0: str      r8, [r4, r2]
003aa4b4: movw     r2, #0x1488
003aa4b8: str      r8, [r4, r2]
003aa4bc: movw     r2, #0x148c
003aa4c0: str      r8, [r4, r2]
003aa4c4: movw     r2, #0x1490
003aa4c8: str      r8, [r4, r2]
003aa4cc: movw     r2, #0x1494
003aa4d0: str      r8, [r4, r2]
003aa4d4: movw     r2, #0x1498
003aa4d8: str      r6, [r4, r2]
003aa4dc: movw     r2, #0x149c
003aa4e0: str      r8, [r4, r2]
003aa4e4: movw     r2, #0x14a0
003aa4e8: str      r8, [r4, r2]
003aa4ec: movw     r2, #0x14a4
003aa4f0: str      r8, [r4, r2]
003aa4f4: movw     r2, #0x14aa
003aa4f8: strh     r8, [r4, r2]
003aa4fc: movw     r2, #0x14ac
003aa500: strb     r8, [r4, r2]
003aa504: movw     r2, #0x14d8
003aa508: str      r3, [r4, r2]
003aa50c: add      r1, r1, #0x800000
003aa510: movw     r2, #0x14fc
003aa514: str      r1, [r4, r2]
003aa518: movw     r2, #0x1504
003aa51c: str      r6, [r4, r2]
003aa520: movw     r2, #0x14ad
003aa524: strb     r8, [r4, r2]
003aa528: movw     r2, #0x14b0
003aa52c: str      r3, [r4, r2]
003aa530: movw     r2, #0x14b4
003aa534: str      r3, [r4, r2]
003aa538: movw     r2, #0x14b8
003aa53c: str      r3, [r4, r2]
003aa540: movw     r2, #0x14bc
003aa544: str      r3, [r4, r2]
003aa548: mov      r2, #0x14c0
003aa54c: str      r3, [r4, r2]
003aa550: movw     r2, #0x14c4
003aa554: str      r3, [r4, r2]
003aa558: movw     r3, #0x14c8
003aa55c: strb     r8, [r4, r3]
003aa560: movw     r3, #0x14ca
003aa564: strh     r6, [r4, r3]
003aa568: movw     r3, #0x14cc
003aa56c: str      r8, [r4, r3]
003aa570: movw     r3, #0x14d0
003aa574: strh     r8, [r4, r3]
003aa578: movw     r3, #0x14d4
003aa57c: str      r8, [r4, r3]
003aa580: movw     r3, #0x14dc
003aa584: strb     r8, [r4, r3]
003aa588: movw     r3, #0x14e4
003aa58c: strb     r8, [r4, r3]
003aa590: movw     r3, #0x14e5
003aa594: strb     r8, [r4, r3]
003aa598: movw     r3, #0x14e8
003aa59c: str      r8, [r4, r3]
003aa5a0: movw     r3, #0x14ec
003aa5a4: str      r8, [r4, r3]
003aa5a8: add      r7, r4, #0x1500
003aa5ac: movw     r3, #0x14f0
003aa5b0: add      r0, r4, #0x1a40
003aa5b4: strb     r8, [r4, r3]
003aa5b8: add      r0, r0, #8
003aa5bc: mov      r3, #0x1500
003aa5c0: add      r7, r7, #8
003aa5c4: str      r6, [r4, r3]
003aa5c8: str      r0, [sp, #0x10]
003aa5cc: mov      r0, r7
003aa5d0: bl       #0x3a6a24
003aa5d4: ldr      r0, [sp, #0x10]
003aa5d8: bl       #0x3a6a24
003aa5dc: add      r0, r4, #0x304
003aa5e0: mov      r1, r4
003aa5e4: strb     fp, [r4, #0x28]
003aa5e8: bl       #0x4a191c
003aa5ec: strb     fp, [r4, #0x1c4]
003aa5f0: strb     fp, [r4, #0x85]
003aa5f4: mov      r0, #0x10
003aa5f8: mov      r1, r8
003aa5fc: bl       #0x310570
003aa600: ldr      r3, [pc, #0x13c]
003aa604: ldr      ip, [sp, #0xc]
003aa608: mov      r6, r0
003aa60c: ldr      r3, [sb, r3]
003aa610: cmp      ip, r8
003aa614: strb     r8, [r6, #0xa]
003aa618: add      r3, r3, #8
003aa61c: str      r8, [r0, #0xc]
003aa620: stm      r0, {r3, ip}
003aa624: strb     r8, [r6, #8]
003aa628: strb     r8, [r6, #9]
003aa62c: beq      #0x3aa6e0
003aa630: mov      r0, ip
003aa634: mov      r1, r6
003aa638: bl       #0x404e10
003aa63c: ldr      r3, [r4, #0x378]
003aa640: ldr      r0, [sp, #0x20]
003aa644: mov      r1, r4
003aa648: str      r4, [r3, #0xc]
003aa64c: bl       #0x3db480
003aa650: ldr      r0, [sp, #0x1c]
003aa654: mov      r1, r4
003aa658: bl       #0x3cb7c0
003aa65c: ldr      r0, [sp, #0x14]
003aa660: mov      r1, r4
003aa664: bl       #0x3c9890
003aa668: mov      r0, r5
003aa66c: mov      r1, r4
003aa670: bl       #0x3c1600
003aa674: ldr      r0, [sp, #0x18]
003aa678: mov      r1, r4
003aa67c: bl       #0x3dec0c
003aa680: mov      r6, #0
003aa684: str      r4, [r4, #0x380]
003aa688: mov      r1, r6
003aa68c: mov      r0, r5
003aa690: add      r6, r6, #1
003aa694: bl       #0x3c7318
003aa698: cmp      r6, #0x14
003aa69c: bne      #0x3aa688
003aa6a0: mov      r1, #0
003aa6a4: movw     r2, #0x14e0
003aa6a8: str      r1, [r4, r2]
003aa6ac: mvn      r3, #0
003aa6b0: movw     r2, #0x14f4
003aa6b4: str      r3, [r4, r2]
003aa6b8: str      r7, [r4, #0x100]
003aa6bc: ldr      sl, [sp, #0x10]
003aa6c0: movw     r2, #0x14f8
003aa6c4: mov      r0, r4
003aa6c8: str      sl, [r4, #0x104]
003aa6cc: str      r3, [r4, r2]
003aa6d0: mov      r3, #1
003aa6d4: strb     r3, [r4, #0xf8]
003aa6d8: add      sp, sp, #0x3c
003aa6dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0: ldr      r3, [pc, #0x60]
003aa6e4: ldr      r3, [sb, r3]
003aa6e8: ldr      r3, [r3]
003aa6ec: cmp      r3, #2
003aa6f0: streq    ip, [r4, #0x374]
003aa6f4: beq      #0x3aa630
003aa6f8: cmp      r3, #1
003aa6fc: bne      #0x3aa630
003aa700: ldr      r0, [pc, #0x44]
003aa704: ldr      r1, [pc, #0x44]
003aa708: ldr      r2, [pc, #0x44]
003aa70c: ldr      r0, [sb, r0]
003aa710: ldr      r3, [pc, #0x40]
003aa714: mov      lr, #0x44
003aa718: add      r1, pc, r1
003aa71c: add      r0, r0, #0xa8
003aa720: add      r2, pc, r2
003aa724: add      r3, pc, r3
003aa728: str      ip, [sp, #0xc]
003aa72c: str      lr, [sp]
003aa730: bl       #0x30e004
003aa734: ldr      ip, [sp, #0xc]
003aa738: b        #0x3aa630
003aa73c: subseq   sl, lr, r4, asr r8
003aa740: andeq    r2, r0, r8, lsl #28
003aa744: andeq    r2, r0, r4, lsr #21
003aa748: andeq    r3, r0, r0, asr #19
003aa74c: andeq    r1, r0, r0, asr #19
003aa750: subseq   r3, r1, r0, asr #25
003aa754: subseq   r8, r1, r0, lsr #27
003aa758: subseq   r8, r1, ip, lsr #27

# _Z11GetNewStateI6CSDeadEP9CharStatev
003c05a4: ldr      r3, [pc, #0xc]
003c05a8: ldr      r2, [pc, #0xc]
003c05ac: add      r3, pc, r3
003c05b0: ldr      r0, [r3, r2]
003c05b4: bx       lr
003c05b8: subseq   r4, sp, r4, ror #9
003c05bc: muleq    r0, r8, fp

# _ZN9CharacterC2EN10ObjectBase6GO_IDSE
003a9340: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a9344: add      ip, r0, #0x374
003a9348: sub      sp, sp, #0x3c
003a934c: mov      r4, r0
003a9350: str      ip, [sp, #0xc]
003a9354: bl       #0x38c398
003a9358: ldr      ip, [sp, #0xc]
003a935c: add      r5, r4, #0x4f0
003a9360: add      r5, r5, #0xc
003a9364: mov      r0, ip
003a9368: bl       #0x404db8
003a936c: add      r0, r4, #0x3b4
003a9370: str      r0, [sp, #0x20]
003a9374: add      r0, r4, #0x37c
003a9378: bl       #0x3ff330
003a937c: add      r2, r4, #0x490
003a9380: add      r1, r4, #0x3c8
003a9384: add      r2, r2, #0xc
003a9388: ldr      r0, [sp, #0x20]
003a938c: str      r1, [sp, #0x1c]
003a9390: str      r2, [sp, #0x14]
003a9394: bl       #0x3dbb0c
003a9398: ldr      r0, [sp, #0x1c]
003a939c: bl       #0x3cebf0
003a93a0: ldr      r0, [sp, #0x14]
003a93a4: bl       #0x3c8ff4
003a93a8: add      r3, r4, #0x560
003a93ac: mov      r0, r5
003a93b0: str      r3, [sp, #0x18]
003a93b4: ldr      sb, [pc, #0x50c]
003a93b8: bl       #0x3c1b58
003a93bc: ldr      r0, [sp, #0x18]
003a93c0: bl       #0x3df084
003a93c4: ldr      lr, [pc, #0x500]
003a93c8: add      sb, pc, sb
003a93cc: mov      r8, #0
003a93d0: ldr      lr, [sb, lr]
003a93d4: mov      fp, #1
003a93d8: mvn      r6, #0
003a93dc: add      sl, lr, #0x324
003a93e0: str      sl, [sp, #0x34]
003a93e4: add      sl, lr, #0x180
003a93e8: str      sl, [sp, #0x10]
003a93ec: add      sl, lr, #0x1f4
003a93f0: str      sl, [sp, #0x24]
003a93f4: add      sl, lr, #0x220
003a93f8: str      sl, [sp, #0x28]
003a93fc: add      sl, lr, #0x230
003a9400: str      sl, [sp, #0x2c]
003a9404: add      r0, lr, #8
003a9408: add      r1, lr, #0x15c
003a940c: add      r2, lr, #0x168
003a9410: add      sl, lr, #0x304
003a9414: str      sl, [sp, #0x30]
003a9418: stm      r4, {r0, r1}
003a941c: str      r2, [r4, #0x24]
003a9420: ldr      r0, [sp, #0x10]
003a9424: add      lr, lr, #0x314
003a9428: add      r7, r4, #0x1380
003a942c: str      r0, [r4, #0x374]
003a9430: ldr      r1, [sp, #0x24]
003a9434: add      r3, r7, #0x18
003a9438: movw     sl, #0x13a8
003a943c: str      r1, [r4, #0x37c]
003a9440: ldr      r2, [sp, #0x28]
003a9444: add      r7, r7, #0x30
003a9448: str      r2, [r4, #0x3b4]
003a944c: ldr      r0, [sp, #0x2c]
003a9450: str      r0, [r4, #0x3c8]
003a9454: ldr      r1, [sp, #0x30]
003a9458: str      lr, [r4, #0x4fc]
003a945c: mov      r0, r3
003a9460: str      r1, [r4, #0x49c]
003a9464: ldr      r2, [sp, #0x34]
003a9468: mov      r1, #0x10
003a946c: str      r2, [r4, #0x560]
003a9470: movw     r2, #0x1394
003a9474: strb     r8, [r4, r2]
003a9478: movw     r2, #0x1395
003a947c: strb     r8, [r4, r2]
003a9480: movw     r2, #0x1396
003a9484: strb     fp, [r4, r2]
003a9488: movw     r2, #0x1397
003a948c: strb     r6, [r4, r2]
003a9490: movw     r2, #0x13ac
003a9494: str      r3, [r4, r2]
003a9498: str      r3, [r4, sl]
003a949c: bl       #0x31167c
003a94a0: ldr      r3, [r4, sl]
003a94a4: mov      sl, #0x13c0
003a94a8: mov      r0, r7
003a94ac: strb     r8, [r3]
003a94b0: movw     r3, #0x13c4
003a94b4: str      r7, [r4, r3]
003a94b8: mov      r1, #0x10
003a94bc: str      r7, [r4, sl]
003a94c0: bl       #0x31167c
003a94c4: ldr      r2, [r4, sl]
003a94c8: add      r7, r4, sl
003a94cc: add      r3, r7, #0xc
003a94d0: strb     r8, [r2]
003a94d4: movw     r2, #0x13c8
003a94d8: strh     r6, [r4, r2]
003a94dc: movw     r2, #0x13ca
003a94e0: strh     r6, [r4, r2]
003a94e4: movw     sl, #0x13dc
003a94e8: movw     r2, #0x13e0
003a94ec: str      r3, [r4, r2]
003a94f0: mov      r0, r3
003a94f4: str      r3, [r4, sl]
003a94f8: mov      r1, #0x10
003a94fc: bl       #0x31167c
003a9500: ldr      r3, [r4, sl]
003a9504: add      r7, r7, #0x28
003a9508: movw     sl, #0x13f8
003a950c: strb     r8, [r3]
003a9510: movw     r3, #0x13e4
003a9514: strb     fp, [r4, r3]
003a9518: movw     r3, #0x13fc
003a951c: str      r7, [r4, r3]
003a9520: mov      r0, r7
003a9524: str      r7, [r4, sl]
003a9528: mov      r1, #0x10
003a952c: bl       #0x31167c
003a9530: ldr      r3, [r4, sl]
003a9534: add      r7, r4, #0x1400
003a9538: movw     sl, #0x1410
003a953c: strb     r8, [r3]
003a9540: movw     r3, #0x1414
003a9544: str      r7, [r4, r3]
003a9548: mov      r0, r7
003a954c: str      r7, [r4, sl]
003a9550: mov      r1, #0x10
003a9554: bl       #0x31167c
003a9558: ldr      r3, [r4, sl]
003a955c: add      r7, r7, #0x18
003a9560: movw     sl, #0x1428
003a9564: strb     r8, [r3]
003a9568: movw     r3, #0x142c
003a956c: str      r7, [r4, r3]
003a9570: mov      r0, r7
003a9574: str      r7, [r4, sl]
003a9578: mov      r1, #0x10
003a957c: bl       #0x31167c
003a9580: ldr      r2, [r4, sl]
003a9584: mov      r3, #0
003a9588: mov      r1, #0xbf000000
003a958c: strb     r8, [r2]
003a9590: movw     r2, #0x14a8
003a9594: strb     r6, [r4, r2]
003a9598: movw     r2, #0x1430
003a959c: strb     fp, [r4, r2]
003a95a0: movw     r2, #0x1434
003a95a4: str      r8, [r4, r2]
003a95a8: movw     r2, #0x1438
003a95ac: str      r8, [r4, r2]
003a95b0: movw     r2, #0x1448
003a95b4: strb     fp, [r4, r2]
003a95b8: movw     r2, #0x1449
003a95bc: strb     r8, [r4, r2]
003a95c0: movw     r2, #0x144c
003a95c4: str      r8, [r4, r2]
003a95c8: movw     r2, #0x1450
003a95cc: str      r3, [r4, r2]
003a95d0: movw     r2, #0x1454
003a95d4: str      r3, [r4, r2]
003a95d8: movw     r2, #0x1458
003a95dc: str      r3, [r4, r2]
003a95e0: movw     r2, #0x145c
003a95e4: str      r3, [r4, r2]
003a95e8: movw     r2, #0x1460
003a95ec: str      r3, [r4, r2]
003a95f0: movw     r2, #0x1464
003a95f4: str      r3, [r4, r2]
003a95f8: movw     r2, #0x1468
003a95fc: str      r3, [r4, r2]
003a9600: movw     r2, #0x146c
003a9604: str      r3, [r4, r2]
003a9608: movw     r2, #0x1470
003a960c: str      r3, [r4, r2]
003a9610: movw     r2, #0x1474
003a9614: str      r3, [r4, r2]
003a9618: movw     r2, #0x1478
003a961c: str      r3, [r4, r2]
003a9620: movw     r2, #0x147c
003a9624: str      r3, [r4, r2]
003a9628: mov      r2, #0x1480
003a962c: strb     r8, [r4, r2]
003a9630: movw     r2, #0x1481
003a9634: strb     r8, [r4, r2]
003a9638: movw     r2, #0x1484
003a963c: str      r8, [r4, r2]
003a9640: movw     r2, #0x1488
003a9644: str      r8, [r4, r2]
003a9648: movw     r2, #0x148c
003a964c: str      r8, [r4, r2]
003a9650: movw     r2, #0x1490
003a9654: str      r8, [r4, r2]
003a9658: movw     r2, #0x1494
003a965c: str      r8, [r4, r2]
003a9660: movw     r2, #0x1498
003a9664: str      r6, [r4, r2]
003a9668: movw     r2, #0x149c
003a966c: str      r8, [r4, r2]
003a9670: movw     r2, #0x14a0
003a9674: str      r8, [r4, r2]
003a9678: movw     r2, #0x14a4
003a967c: str      r8, [r4, r2]
003a9680: movw     r2, #0x14aa
003a9684: strh     r8, [r4, r2]
003a9688: movw     r2, #0x14ac
003a968c: strb     r8, [r4, r2]
003a9690: movw     r2, #0x14d8
003a9694: str      r3, [r4, r2]
003a9698: add      r1, r1, #0x800000
003a969c: movw     r2, #0x14fc
003a96a0: str      r1, [r4, r2]
003a96a4: movw     r2, #0x1504
003a96a8: str      r6, [r4, r2]
003a96ac: movw     r2, #0x14ad
003a96b0: strb     r8, [r4, r2]
003a96b4: movw     r2, #0x14b0
003a96b8: str      r3, [r4, r2]
003a96bc: movw     r2, #0x14b4
003a96c0: str      r3, [r4, r2]
003a96c4: movw     r2, #0x14b8
003a96c8: str      r3, [r4, r2]
003a96cc: movw     r2, #0x14bc
003a96d0: str      r3, [r4, r2]
003a96d4: mov      r2, #0x14c0
003a96d8: str      r3, [r4, r2]
003a96dc: movw     r2, #0x14c4
003a96e0: str      r3, [r4, r2]
003a96e4: movw     r3, #0x14c8
003a96e8: strb     r8, [r4, r3]
003a96ec: movw     r3, #0x14ca
003a96f0: strh     r6, [r4, r3]
003a96f4: movw     r3, #0x14cc
003a96f8: str      r8, [r4, r3]
003a96fc: movw     r3, #0x14d0
003a9700: strh     r8, [r4, r3]
003a9704: movw     r3, #0x14d4
003a9708: str      r8, [r4, r3]
003a970c: movw     r3, #0x14dc
003a9710: strb     r8, [r4, r3]
003a9714: movw     r3, #0x14e4
003a9718: strb     r8, [r4, r3]
003a971c: movw     r3, #0x14e5
003a9720: strb     r8, [r4, r3]
003a9724: movw     r3, #0x14e8
003a9728: str      r8, [r4, r3]
003a972c: movw     r3, #0x14ec
003a9730: str      r8, [r4, r3]
003a9734: add      r7, r4, #0x1500
003a9738: movw     r3, #0x14f0
003a973c: add      r0, r4, #0x1a40
003a9740: strb     r8, [r4, r3]
003a9744: add      r0, r0, #8
003a9748: mov      r3, #0x1500
003a974c: add      r7, r7, #8
003a9750: str      r6, [r4, r3]
003a9754: str      r0, [sp, #0x10]
003a9758: mov      r0, r7
003a975c: bl       #0x3a6a24
003a9760: ldr      r0, [sp, #0x10]
003a9764: bl       #0x3a6a24
003a9768: add      r0, r4, #0x304
003a976c: mov      r1, r4
003a9770: strb     fp, [r4, #0x28]
003a9774: bl       #0x4a191c
003a9778: strb     fp, [r4, #0x1c4]
003a977c: strb     fp, [r4, #0x85]
003a9780: mov      r0, #0x10
003a9784: mov      r1, r8
003a9788: bl       #0x310570
003a978c: ldr      r3, [pc, #0x13c]
003a9790: ldr      ip, [sp, #0xc]
003a9794: mov      r6, r0
003a9798: ldr      r3, [sb, r3]
003a979c: cmp      ip, r8
003a97a0: strb     r8, [r6, #0xa]
003a97a4: add      r3, r3, #8
003a97a8: str      r8, [r0, #0xc]
003a97ac: stm      r0, {r3, ip}
003a97b0: strb     r8, [r6, #8]
003a97b4: strb     r8, [r6, #9]
003a97b8: beq      #0x3a986c
003a97bc: mov      r0, ip
003a97c0: mov      r1, r6
003a97c4: bl       #0x404e10
003a97c8: ldr      r3, [r4, #0x378]
003a97cc: ldr      r0, [sp, #0x20]
003a97d0: mov      r1, r4
003a97d4: str      r4, [r3, #0xc]
003a97d8: bl       #0x3db480
003a97dc: ldr      r0, [sp, #0x1c]
003a97e0: mov      r1, r4
003a97e4: bl       #0x3cb7c0
003a97e8: ldr      r0, [sp, #0x14]
003a97ec: mov      r1, r4
003a97f0: bl       #0x3c9890
003a97f4: mov      r0, r5
003a97f8: mov      r1, r4
003a97fc: bl       #0x3c1600
003a9800: ldr      r0, [sp, #0x18]
003a9804: mov      r1, r4
003a9808: bl       #0x3dec0c
003a980c: mov      r6, #0
003a9810: str      r4, [r4, #0x380]
003a9814: mov      r1, r6
003a9818: mov      r0, r5
003a981c: add      r6, r6, #1
003a9820: bl       #0x3c7318
003a9824: cmp      r6, #0x14
003a9828: bne      #0x3a9814
003a982c: mov      r1, #0
003a9830: movw     r2, #0x14e0
003a9834: str      r1, [r4, r2]
003a9838: mvn      r3, #0
003a983c: movw     r2, #0x14f4
003a9840: str      r3, [r4, r2]
003a9844: str      r7, [r4, #0x100]
003a9848: ldr      sl, [sp, #0x10]
003a984c: movw     r2, #0x14f8
003a9850: mov      r0, r4
003a9854: str      sl, [r4, #0x104]
003a9858: str      r3, [r4, r2]
003a985c: mov      r3, #1
003a9860: strb     r3, [r4, #0xf8]
003a9864: add      sp, sp, #0x3c
003a9868: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a986c: ldr      r3, [pc, #0x60]
003a9870: ldr      r3, [sb, r3]
003a9874: ldr      r3, [r3]
003a9878: cmp      r3, #2
003a987c: streq    ip, [r4, #0x374]
003a9880: beq      #0x3a97bc
003a9884: cmp      r3, #1
003a9888: bne      #0x3a97bc
003a988c: ldr      r0, [pc, #0x44]
003a9890: ldr      r1, [pc, #0x44]
003a9894: ldr      r2, [pc, #0x44]
003a9898: ldr      r0, [sb, r0]
003a989c: ldr      r3, [pc, #0x40]
003a98a0: mov      lr, #0x44
003a98a4: add      r1, pc, r1
003a98a8: add      r0, r0, #0xa8
003a98ac: add      r2, pc, r2
003a98b0: add      r3, pc, r3
003a98b4: str      ip, [sp, #0xc]
003a98b8: str      lr, [sp]
003a98bc: bl       #0x30e004
003a98c0: ldr      ip, [sp, #0xc]
003a98c4: b        #0x3a97bc
003a98c8: subseq   fp, lr, r8, asr #13
003a98cc: andeq    r2, r0, r8, lsl #28
003a98d0: andeq    r2, r0, r4, lsr #21
003a98d4: andeq    r3, r0, r0, asr #19
003a98d8: andeq    r1, r0, r0, asr #19
003a98dc: subseq   r4, r1, r4, lsr fp
003a98e0: subseq   sb, r1, r4, lsl ip
003a98e4: subseq   sb, r1, r0, lsr #24

# _Z11GetNewStateI6CSIdleEP9CharStatev
003c04a8: ldr      r3, [pc, #0xc]
003c04ac: ldr      r2, [pc, #0xc]
003c04b0: add      r3, pc, r3
003c04b4: ldr      r0, [r3, r2]
003c04b8: bx       lr
003c04bc: subseq   r4, sp, r0, ror #11
003c04c0: andeq    r1, r0, r4, ror #20
