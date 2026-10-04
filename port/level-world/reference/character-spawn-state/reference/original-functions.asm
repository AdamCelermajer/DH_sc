
# _ZN6Random9GetRandomEib.clone.1
003c26a0: push     {r4, lr}
003c26a4: ldr      r4, [pc, #0x7c]
003c26a8: cmp      r0, #0
003c26ac: add      r4, pc, r4
003c26b0: beq      #0x3c2710
003c26b4: ldr      r2, [pc, #0x70]
003c26b8: mov      r1, r0
003c26bc: movw     r0, #0xe6ab
003c26c0: ldr      r2, [r4, r2]
003c26c4: movw     r3, #0xdb17
003c26c8: movt     r3, #0x2b52
003c26cc: ldr      lr, [r2]
003c26d0: movw     ip, #0xf26b
003c26d4: movt     ip, #0xda
003c26d8: mul      r0, r0, lr
003c26dc: add      r0, r0, #0x2b000
003c26e0: add      r0, r0, #0x3fc
003c26e4: add      r0, r0, #1
003c26e8: umull    lr, r3, r3, r0
003c26ec: rsb      lr, r3, r0
003c26f0: add      r3, r3, lr, lsr #1
003c26f4: lsr      r3, r3, #0x17
003c26f8: mls      r3, ip, r3, r0
003c26fc: mov      r0, r3
003c2700: str      r3, [r2]
003c2704: bl       #0x30eb2c
003c2708: eor      r0, r1, r1, asr #31
003c270c: sub      r0, r0, r1, asr #31
003c2710: ldr      r3, [pc, #0x18]
003c2714: ldr      r3, [r4, r3]
003c2718: ldr      r2, [r3]
003c271c: add      r2, r2, #1
003c2720: str      r2, [r3]
003c2724: pop      {r4, pc}
003c2728: subseq   r2, sp, r4, ror #7
003c272c: muleq    r0, r4, ip
003c2730: andeq    r1, r0, r8, lsl #1

# _ZN10CharTimers9TMR_StartEjiiPv
003dbe24: push     {r4, r5, r6, lr}
003dbe28: mov      r6, r3
003dbe2c: mov      r4, r1
003dbe30: mov      r5, r2
003dbe34: bl       #0x3dbd70
003dbe38: subs     r3, r0, #0
003dbe3c: beq      #0x3dbe70
003dbe40: mov      r2, #0
003dbe44: mov      r1, #1
003dbe48: strb     r1, [r3, #0x14]
003dbe4c: str      r5, [r3, #8]
003dbe50: str      r4, [r3, #0xc]
003dbe54: str      r2, [r3, #0x10]
003dbe58: str      r6, [r3, #0x18]
003dbe5c: ldr      r1, [sp, #0x10]
003dbe60: ldr      r0, [r3, #4]
003dbe64: strb     r2, [r3, #0x15]
003dbe68: str      r1, [r3, #0x1c]
003dbe6c: pop      {r4, r5, r6, pc}
003dbe70: mvn      r0, #0
003dbe74: pop      {r4, r5, r6, pc}

# _ZN16CharStateMachine9_SetStateEiiPv
003c1938: push     {r4, r5, r6, r7, r8, sl, lr}
003c193c: ldr      ip, [r0, #0x20]
003c1940: sub      sp, sp, #0x14
003c1944: mov      r4, r0
003c1948: cmp      ip, #0
003c194c: mvneq    r7, #0
003c1950: mov      r5, r1
003c1954: mov      r8, r2
003c1958: mov      sl, r3
003c195c: moveq    r6, r7
003c1960: beq      #0x3c1990
003c1964: ldr      r3, [ip, #4]
003c1968: ldr      r6, [ip]
003c196c: ldr      r2, [r0, #4]
003c1970: ldr      ip, [r3]
003c1974: mov      r0, r3
003c1978: str      r1, [sp]
003c197c: mov      r3, r4
003c1980: mov      r1, r6
003c1984: mov      lr, pc
003c1988: ldr      pc, [ip, #0x10]
003c198c: mov      r7, r6
003c1990: mov      r0, r4
003c1994: mov      r1, r5
003c1998: bl       #0x3c0084
003c199c: cmp      r0, #0
003c19a0: streq    r0, [r4, #0x20]
003c19a4: bne      #0x3c19c0
003c19a8: ldr      r0, [r4, #4]
003c19ac: mov      r2, r7
003c19b0: mov      r1, #0x1d
003c19b4: add      sp, sp, #0x14
003c19b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003c19bc: b        #0x3a4d5c
003c19c0: mov      r1, r5
003c19c4: mov      r0, r4
003c19c8: bl       #0x3c184c
003c19cc: cmp      r6, r5
003c19d0: movne    r3, #0
003c19d4: str      r0, [r4, #0x20]
003c19d8: strne    r3, [r4, #0x60]
003c19dc: ldm      r0, {r1, r3}
003c19e0: ldr      r2, [r4, #4]
003c19e4: ldr      ip, [r3]
003c19e8: mov      r0, r3
003c19ec: stm      sp, {r6, r8, sl}
003c19f0: mov      r3, r4
003c19f4: mov      lr, pc
003c19f8: ldr      pc, [ip, #0xc]
003c19fc: b        #0x3c19a8

# _ZN9Character9CSM_SpawnEiPviRi
003ad2e4: cmp      r3, #0
003ad2e8: push     {r4, lr}
003ad2ec: mov      r4, r0
003ad2f0: bne      #0x3ad2fc
003ad2f4: pop      {r4, lr}
003ad2f8: b        #0x3a5248
003ad2fc: cmp      r3, #0x11
003ad300: beq      #0x3ad30c
003ad304: mov      r0, #1
003ad308: pop      {r4, pc}
003ad30c: ldr      r0, [r0, #0x3fc]
003ad310: cmp      r0, #0
003ad314: beq      #0x3ad328
003ad318: ldr      r1, [r4, #0x3cc]
003ad31c: bl       #0x3d24fc
003ad320: cmp      r0, #0
003ad324: beq      #0x3ad330
003ad328: movw     r3, #0x1430
003ad32c: ldrb     r0, [r4, r3]
003ad330: pop      {r4, pc}

# _ZN9Character17DeclarePropertiesEv
003a9fe4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003a9fe8: sub      sp, sp, #8
003a9fec: mov      r7, r0
003a9ff0: bl       #0x38cee8
003a9ff4: ldr      r1, [pc, #0x180]
003a9ff8: add      r4, r7, #4
003a9ffc: add      r5, r7, #0x1380
003aa000: mov      r0, r4
003aa004: add      r2, r5, #0x30
003aa008: add      r1, pc, r1
003aa00c: bl       #0x33ef7c
003aa010: ldr      r1, [pc, #0x168]
003aa014: add      r2, r5, #0x18
003aa018: mov      r0, r4
003aa01c: add      r1, pc, r1
003aa020: bl       #0x33ef7c
003aa024: ldr      r1, [pc, #0x158]
003aa028: add      r5, r7, #0x13c0
003aa02c: mov      r0, r4
003aa030: add      r2, r5, #0xc
003aa034: add      r1, pc, r1
003aa038: bl       #0x33ef7c
003aa03c: ldr      r1, [pc, #0x144]
003aa040: mov      r0, r4
003aa044: add      r2, r5, #0x24
003aa048: add      r1, pc, r1
003aa04c: bl       #0x3a92b4
003aa050: ldr      r1, [pc, #0x134]
003aa054: add      r2, r5, #0x28
003aa058: mov      r0, r4
003aa05c: add      r1, pc, r1
003aa060: bl       #0x33ef7c
003aa064: ldr      r1, [pc, #0x124]
003aa068: add      r7, r7, #0x1400
003aa06c: mov      r0, r4
003aa070: mov      r2, r7
003aa074: add      r1, pc, r1
003aa078: bl       #0x33ef7c
003aa07c: ldr      r1, [pc, #0x110]
003aa080: mov      r0, r4
003aa084: add      r2, r7, #0x18
003aa088: add      r1, pc, r1
003aa08c: bl       #0x33ef7c
003aa090: ldr      r1, [pc, #0x100]
003aa094: add      r2, r7, #0x30
003aa098: mov      r0, r4
003aa09c: add      r1, pc, r1
003aa0a0: bl       #0x3a92b4
003aa0a4: mov      r1, #0
003aa0a8: mov      r0, #0x28
003aa0ac: bl       #0x310570
003aa0b0: ldr      r5, [pc, #0xe4]
003aa0b4: ldr      sb, [pc, #0xe4]
003aa0b8: ldr      r8, [pc, #0xe4]
003aa0bc: add      r5, pc, r5
003aa0c0: ldr      sb, [r5, sb]
003aa0c4: add      r8, pc, r8
003aa0c8: mov      r6, r0
003aa0cc: add      sb, sb, #8
003aa0d0: mov      r1, r8
003aa0d4: add      r2, sp, #4
003aa0d8: str      sb, [r0], #8
003aa0dc: bl       #0x3140ec
003aa0e0: ldr      r3, [pc, #0xc0]
003aa0e4: add      r2, r7, #0x34
003aa0e8: mov      sl, #0
003aa0ec: ldr      r3, [r5, r3]
003aa0f0: rsb      r2, r4, r2
003aa0f4: str      r2, [r6, #4]
003aa0f8: add      r3, r3, #8
003aa0fc: str      r3, [r6]
003aa100: mov      r2, r6
003aa104: mov      r1, r8
003aa108: str      sl, [r6, #0x20]
003aa10c: str      sl, [r6, #0x24]
003aa110: mov      r0, r4
003aa114: bl       #0x513ce4
003aa118: mov      r1, sl
003aa11c: mov      r0, #0x24
003aa120: bl       #0x310570
003aa124: ldr      r8, [pc, #0x80]
003aa128: mov      r6, r0
003aa12c: mov      r2, sp
003aa130: add      r8, pc, r8
003aa134: mov      r1, r8
003aa138: str      sb, [r0], #8
003aa13c: bl       #0x3140ec
003aa140: ldr      r3, [pc, #0x68]
003aa144: add      r7, r7, #0x3c
003aa148: rsb      r7, r4, r7
003aa14c: ldr      r3, [r5, r3]
003aa150: str      r7, [r6, #4]
003aa154: mov      r0, r4
003aa158: add      r3, r3, #8
003aa15c: str      r3, [r6]
003aa160: mov      r3, #0
003aa164: str      r3, [r6, #0x20]
003aa168: mov      r1, r8
003aa16c: mov      r2, r6
003aa170: bl       #0x513ce4
003aa174: add      sp, sp, #8
003aa178: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN16CharStateMachine16SM_SetSpawnStateEbb
003c2734: push     {r4, r5, lr}
003c2738: subs     r3, r1, #0
003c273c: sub      sp, sp, #0xc
003c2740: mov      r4, r0
003c2744: beq      #0x3c27d8
003c2748: ldr      r2, [r0, #4]
003c274c: movw     r1, #0x1434
003c2750: ldr      r3, [r2, r1]
003c2754: cmp      r3, #0
003c2758: movlt    r3, #0
003c275c: strlt    r3, [r2, r1]
003c2760: movw     r1, #0x1438
003c2764: ldr      r0, [r2, r1]
003c2768: cmp      r0, r3
003c276c: movwlt   r0, #0x1434
003c2770: ldrlt    r5, [r2, r0]
003c2774: movge    r5, r3
003c2778: movge    r3, r0
003c277c: strlt    r3, [r2, r1]
003c2780: cmp      r3, r5
003c2784: bne      #0x3c27a8
003c2788: cmp      r3, #0
003c278c: bne      #0x3c27ec
003c2790: mov      r0, r4
003c2794: mov      r1, #1
003c2798: mvn      r2, #0
003c279c: add      sp, sp, #0xc
003c27a0: pop      {r4, r5, lr}
003c27a4: b        #0x3c1938
003c27a8: rsb      r0, r5, r3
003c27ac: bl       #0x3c26a0
003c27b0: ldr      r3, [r4, #4]
003c27b4: mov      ip, #0
003c27b8: add      r1, r0, r5
003c27bc: mov      r2, ip
003c27c0: add      r0, r3, #0x3b4
003c27c4: mov      r3, #0x2d
003c27c8: str      ip, [sp]
003c27cc: bl       #0x3dbe24
003c27d0: add      sp, sp, #0xc
003c27d4: pop      {r4, r5, pc}
003c27d8: mov      r1, #1
003c27dc: mvn      r2, #0
003c27e0: add      sp, sp, #0xc
003c27e4: pop      {r4, r5, lr}
003c27e8: b        #0x3c1938
003c27ec: ldr      r0, [r4, #4]
003c27f0: mov      ip, #0
003c27f4: mov      r1, r3
003c27f8: mov      r2, ip
003c27fc: mov      r3, #0x2d
003c2800: add      r0, r0, #0x3b4
003c2804: str      ip, [sp]
003c2808: bl       #0x3dbe24
003c280c: b        #0x3c27d0

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
