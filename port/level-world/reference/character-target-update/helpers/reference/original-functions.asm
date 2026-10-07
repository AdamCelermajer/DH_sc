
# _ZNK16CharStateMachine20SM_IsAwaitingToSpawnEv
003c0230: push     {r4, lr}
003c0234: bl       #0x3c01ac
003c0238: cmp      r0, #0x11
003c023c: movne    r0, #0
003c0240: moveq    r0, #1
003c0244: pop      {r4, pc}

# _ZNK6CharAI17AI_IsInCloseRangeEPK10GameObject
003d63d8: push     {r4, r5, r6, r7, r8, sl, lr}
003d63dc: ldr      r4, [pc, #0x20c]
003d63e0: ldr      r5, [pc, #0x20c]
003d63e4: sub      sp, sp, #0x54
003d63e8: add      r4, pc, r4
003d63ec: ldr      r3, [r4, r5]
003d63f0: subs     r6, r1, #0
003d63f4: mov      r7, r0
003d63f8: ldr      r3, [r3]
003d63fc: str      r3, [sp, #0x4c]
003d6400: beq      #0x3d65a0
003d6404: mov      r1, r6
003d6408: mov      r0, sp
003d640c: bl       #0x33dd70
003d6410: mov      r0, sp
003d6414: mov      r1, #0
003d6418: bl       #0x33ff8c
003d641c: subs     r3, r0, #0
003d6420: mov      r8, sp
003d6424: bne      #0x3d6450
003d6428: mov      r0, r7
003d642c: mov      r1, r6
003d6430: bl       #0x3d4f98
003d6434: ldr      r3, [r4, r5]
003d6438: ldr      r2, [sp, #0x4c]
003d643c: ldr      r3, [r3]
003d6440: cmp      r2, r3
003d6444: bne      #0x3d65ec
003d6448: add      sp, sp, #0x54
003d644c: pop      {r4, r5, r6, r7, r8, sl, pc}
003d6450: ldr      r2, [r3, #0xf4]
003d6454: cmp      r2, #0
003d6458: bne      #0x3d6428
003d645c: ldr      r3, [r3]
003d6460: ldr      r1, [r7, #4]
003d6464: mov      lr, pc
003d6468: ldr      pc, [r3, #0x90]
003d646c: cmp      r0, #8
003d6470: bne      #0x3d6428
003d6474: ldr      r3, [r7, #4]
003d6478: add      r2, sp, #0xc
003d647c: add      r1, sp, #0x10
003d6480: mov      r0, r3
003d6484: ldr      ip, [r3]
003d6488: mov      r3, r2
003d648c: mov      lr, pc
003d6490: ldr      pc, [ip, #0x128]
003d6494: cmp      r0, #0
003d6498: moveq    r0, #0
003d649c: beq      #0x3d6434
003d64a0: ldr      r0, [r7, #4]
003d64a4: bl       #0x3935dc
003d64a8: mov      r7, r0
003d64ac: mov      r0, r6
003d64b0: bl       #0x3935dc
003d64b4: mov      r6, r0
003d64b8: ldr      r1, [r0]
003d64bc: ldr      r0, [r7]
003d64c0: bl       #0x30e3ac
003d64c4: ldr      r1, [r6, #4]
003d64c8: mov      sl, r0
003d64cc: ldr      r0, [r7, #4]
003d64d0: bl       #0x30e3ac
003d64d4: ldr      r1, [r6, #8]
003d64d8: mov      r8, r0
003d64dc: ldr      r0, [r7, #8]
003d64e0: bl       #0x30e3ac
003d64e4: mov      r1, sl
003d64e8: mov      r7, r0
003d64ec: mov      r0, sl
003d64f0: bl       #0x30ed6c
003d64f4: mov      r1, r8
003d64f8: mov      r6, r0
003d64fc: mov      r0, r8
003d6500: bl       #0x30ed6c
003d6504: mov      r1, r0
003d6508: mov      r0, r6
003d650c: bl       #0x30eba4
003d6510: mov      r1, r7
003d6514: mov      r6, r0
003d6518: mov      r0, r7
003d651c: bl       #0x30ed6c
003d6520: mov      r1, r0
003d6524: mov      r0, r6
003d6528: bl       #0x30eba4
003d652c: ldr      r3, [pc, #0xc4]
003d6530: mov      r8, r0
003d6534: add      r6, sp, #0x34
003d6538: ldr      r7, [r4, r3]
003d653c: mov      r0, r7
003d6540: bl       #0x337888
003d6544: ldr      r1, [pc, #0xb0]
003d6548: add      r2, sp, #0x18
003d654c: mov      r0, r6
003d6550: add      r1, pc, r1
003d6554: bl       #0x3140ec
003d6558: mov      r1, r6
003d655c: mov      r0, r7
003d6560: bl       #0x337a88
003d6564: mov      sl, r0
003d6568: mov      r0, r6
003d656c: bl       #0x318254
003d6570: cmp      sl, #0
003d6574: bne      #0x3d65b4
003d6578: ldr      r0, [sp, #0x10]
003d657c: mov      r6, #0
003d6580: mul      r0, r0, r0
003d6584: bl       #0x30e964
003d6588: mov      r1, r8
003d658c: bl       #0x30e2f8
003d6590: cmp      r0, #0
003d6594: movne    r6, #1
003d6598: uxtb     r0, r6
003d659c: b        #0x3d6434
003d65a0: ldr      r6, [r0, #0x40]
003d65a4: cmp      r6, #0
003d65a8: bne      #0x3d6404
003d65ac: mov      r0, #0
003d65b0: b        #0x3d6434
003d65b4: mov      r0, r7
003d65b8: bl       #0x337888
003d65bc: ldr      r1, [pc, #0x3c]
003d65c0: add      r6, sp, #0x1c
003d65c4: add      r2, sp, #0x14
003d65c8: add      r1, pc, r1
003d65cc: mov      r0, r6
003d65d0: bl       #0x3140ec
003d65d4: mov      r0, r7
003d65d8: mov      r1, r6
003d65dc: bl       #0x337a88
003d65e0: mov      r0, r6
003d65e4: bl       #0x318254
003d65e8: b        #0x3d6578
003d65ec: bl       #0x30e310
003d65f0: subseq   lr, fp, r8, lsr #13
003d65f4: andeq    r4, r0, ip, lsr #1
003d65f8: andeq    r0, r0, r4, lsl #17
003d65fc: subeq    pc, lr, r8, lsl #3
003d6600: subeq    pc, lr, r8, lsr #2

# _ZNK16CharStateMachine13SM_IsInLimbusEv
003c01c0: push     {r4, lr}
003c01c4: bl       #0x3c01ac
003c01c8: rsbs     r0, r0, #1
003c01cc: movlo    r0, #0
003c01d0: pop      {r4, pc}

# _ZNK6CharAI17AI_IsInMeleeRangeEPK10GameObject
003d6188: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003d618c: ldr      r4, [pc, #0x230]
003d6190: ldr      r5, [pc, #0x230]
003d6194: sub      sp, sp, #0x4c
003d6198: add      r4, pc, r4
003d619c: ldr      r3, [r4, r5]
003d61a0: subs     r6, r1, #0
003d61a4: mov      r8, r0
003d61a8: ldr      r3, [r3]
003d61ac: str      r3, [sp, #0x44]
003d61b0: beq      #0x3d6368
003d61b4: mov      r1, r6
003d61b8: mov      r0, sp
003d61bc: bl       #0x33dd70
003d61c0: mov      r0, sp
003d61c4: mov      r1, #0
003d61c8: bl       #0x33ff8c
003d61cc: subs     sl, r0, #0
003d61d0: mov      r7, sp
003d61d4: bne      #0x3d6200
003d61d8: mov      r0, r8
003d61dc: mov      r1, r6
003d61e0: bl       #0x3d4f98
003d61e4: ldr      r3, [r4, r5]
003d61e8: ldr      r2, [sp, #0x44]
003d61ec: ldr      r3, [r3]
003d61f0: cmp      r2, r3
003d61f4: bne      #0x3d63c0
003d61f8: add      sp, sp, #0x4c
003d61fc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003d6200: ldr      r3, [sl, #0xf4]
003d6204: cmp      r3, #0
003d6208: bne      #0x3d61d8
003d620c: ldr      r3, [sl]
003d6210: ldr      r1, [r8, #4]
003d6214: mov      lr, pc
003d6218: ldr      pc, [r3, #0x90]
003d621c: cmp      r0, #8
003d6220: bne      #0x3d61d8
003d6224: ldr      r0, [r8, #4]
003d6228: bl       #0x3935dc
003d622c: mov      r7, r0
003d6230: mov      r0, r6
003d6234: bl       #0x3935dc
003d6238: mov      r6, r0
003d623c: ldr      r1, [r0]
003d6240: ldr      r0, [r7]
003d6244: bl       #0x30e3ac
003d6248: ldr      r1, [r6, #4]
003d624c: mov      fp, r0
003d6250: ldr      r0, [r7, #4]
003d6254: bl       #0x30e3ac
003d6258: ldr      r1, [r6, #8]
003d625c: mov      sb, r0
003d6260: ldr      r0, [r7, #8]
003d6264: bl       #0x30e3ac
003d6268: mov      r1, fp
003d626c: mov      r7, r0
003d6270: mov      r0, fp
003d6274: bl       #0x30ed6c
003d6278: mov      r1, sb
003d627c: mov      r6, r0
003d6280: mov      r0, sb
003d6284: bl       #0x30ed6c
003d6288: mov      r1, r0
003d628c: mov      r0, r6
003d6290: bl       #0x30eba4
003d6294: mov      r1, r7
003d6298: mov      r6, r0
003d629c: mov      r0, r7
003d62a0: bl       #0x30ed6c
003d62a4: mov      r1, r0
003d62a8: mov      r0, r6
003d62ac: bl       #0x30eba4
003d62b0: mov      r7, r0
003d62b4: mov      r0, r8
003d62b8: bl       #0x3d4c34
003d62bc: mov      r6, r0
003d62c0: add      r0, sl, #0x3c8
003d62c4: bl       #0x3d4c34
003d62c8: mov      r1, r0
003d62cc: mov      r0, r6
003d62d0: bl       #0x30eba4
003d62d4: ldr      sb, [pc, #0xf0]
003d62d8: mov      r6, r0
003d62dc: add      r8, sp, #0x2c
003d62e0: ldr      sl, [r4, sb]
003d62e4: mov      r0, sl
003d62e8: bl       #0x337888
003d62ec: ldr      r1, [pc, #0xdc]
003d62f0: add      r2, sp, #0x10
003d62f4: mov      r0, r8
003d62f8: add      r1, pc, r1
003d62fc: bl       #0x3140ec
003d6300: mov      r0, sl
003d6304: mov      r1, r8
003d6308: bl       #0x337a88
003d630c: mov      sl, r0
003d6310: ldr      r0, [sp, #0x40]
003d6314: cmp      r0, r8
003d6318: beq      #0x3d6338
003d631c: cmp      r0, #0
003d6320: beq      #0x3d6338
003d6324: ldr      r1, [sp, #0x2c]
003d6328: rsb      r1, r0, r1
003d632c: cmp      r1, #0x80
003d6330: bhi      #0x3d63b8
003d6334: bl       #0x708f00
003d6338: cmp      sl, #0
003d633c: bne      #0x3d637c
003d6340: mov      r1, r6
003d6344: mov      r0, r6
003d6348: bl       #0x30ed6c
003d634c: mov      r1, r7
003d6350: bl       #0x30e2f8
003d6354: cmp      r0, #0
003d6358: mov      r0, #0
003d635c: movne    r0, #1
003d6360: uxtb     r0, r0
003d6364: b        #0x3d61e4
003d6368: ldr      r6, [r0, #0x40]
003d636c: cmp      r6, #0
003d6370: moveq    r0, r6
003d6374: beq      #0x3d61e4
003d6378: b        #0x3d61b4
003d637c: ldr      sl, [r4, sb]
003d6380: add      r8, sp, #0x14
003d6384: mov      r0, sl
003d6388: bl       #0x337888
003d638c: ldr      r1, [pc, #0x40]
003d6390: add      r2, sp, #0xc
003d6394: mov      r0, r8
003d6398: add      r1, pc, r1
003d639c: bl       #0x3140ec
003d63a0: mov      r0, sl
003d63a4: mov      r1, r8
003d63a8: bl       #0x337a88
003d63ac: mov      r0, r8
003d63b0: bl       #0x318254
003d63b4: b        #0x3d6340
003d63b8: bl       #0x310440
003d63bc: b        #0x3d6338
003d63c0: bl       #0x30e310
003d63c4: ldrsheq  lr, [fp], #-0x88
003d63c8: andeq    r4, r0, ip, lsr #1
003d63cc: andeq    r0, r0, r4, lsl #17
003d63d0: subeq    pc, lr, r0, ror #7
003d63d4: subeq    pc, lr, r8, asr r3
