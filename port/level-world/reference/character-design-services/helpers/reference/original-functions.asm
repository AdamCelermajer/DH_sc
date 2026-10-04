
# _ZN12StreamReader6readAsIhEET_P11IStreamBase
003365a4: str      lr, [sp, #-4]!
003365a8: sub      sp, sp, #0x14
003365ac: mov      r3, #0
003365b0: ldr      ip, [r0]
003365b4: add      r1, sp, #0xf
003365b8: mov      r2, #1
003365bc: mov      lr, pc
003365c0: ldr      pc, [ip, #0x18]
003365c4: ldr      r3, [pc, #0x78]
003365c8: cmp      r0, #1
003365cc: add      r3, pc, r3
003365d0: beq      #0x336604
003365d4: ldr      r2, [pc, #0x6c]
003365d8: ldr      r2, [r3, r2]
003365dc: ldr      r2, [r2]
003365e0: cmp      r2, #2
003365e4: moveq    r3, #0
003365e8: streq    r3, [r3]
003365ec: beq      #0x3365f8
003365f0: cmp      r2, #1
003365f4: beq      #0x336610
003365f8: ldrb     r0, [sp, #0xf]
003365fc: add      sp, sp, #0x14
00336600: ldm      sp!, {pc}
00336604: cmp      r1, #0
00336608: beq      #0x3365f8
0033660c: b        #0x3365d4
00336610: ldr      r0, [pc, #0x34]
00336614: ldr      r1, [pc, #0x34]
00336618: ldr      r2, [pc, #0x34]
0033661c: ldr      r0, [r3, r0]
00336620: ldr      r3, [pc, #0x30]
00336624: mov      ip, #0x44
00336628: add      r1, pc, r1
0033662c: add      r2, pc, r2
00336630: add      r3, pc, r3
00336634: add      r0, r0, #0xa8
00336638: str      ip, [sp]
0033663c: bl       #0x30e004
00336640: b        #0x3365f8
00336644: rsbeq    lr, r5, r4, asr #9
00336648: andeq    r3, r0, r0, asr #19
0033664c: andeq    r1, r0, r0, asr #19
00336650: ldrheq   r7, [r8], #-0xd0
00336654: ldrsbeq  r7, [r8], #-0xe4
00336658: subseq   sb, r8, r0, lsl r7

# _ZN6CharAI6OnInitEv
003d12b0: push     {r4, r5, r6, r7, lr}
003d12b4: ldr      r3, [r0, #4]
003d12b8: sub      sp, sp, #0xc
003d12bc: mov      r4, r0
003d12c0: mov      r0, r3
003d12c4: ldr      r3, [r3]
003d12c8: mov      lr, pc
003d12cc: ldr      pc, [r3, #0x34]
003d12d0: ldr      r5, [pc, #0xe8]
003d12d4: cmp      r0, #0
003d12d8: add      r5, pc, r5
003d12dc: bne      #0x3d139c
003d12e0: ldr      r1, [r4, #0x10]
003d12e4: cmn      r1, #1
003d12e8: beq      #0x3d12f8
003d12ec: ldr      r0, [r4, #4]
003d12f0: add      r0, r0, #0x3b4
003d12f4: bl       #0x3db2d8
003d12f8: ldr      r6, [pc, #0xc4]
003d12fc: ldr      r1, [pc, #0xc4]
003d1300: ldr      r2, [pc, #0xc4]
003d1304: ldr      r3, [r5, r6]
003d1308: add      r1, pc, r1
003d130c: add      r2, pc, r2
003d1310: ldr      r0, [r3, #0x2c]
003d1314: ldr      r7, [r4, #4]
003d1318: bl       #0x4c4bdc
003d131c: add      r7, r7, #0x3b4
003d1320: mov      r1, r0
003d1324: mov      ip, #0
003d1328: mov      r0, r7
003d132c: mvn      r2, #0
003d1330: mov      r3, #0x33
003d1334: str      ip, [sp]
003d1338: bl       #0x3dbe24
003d133c: ldr      r1, [r4, #0x14]
003d1340: str      r0, [r4, #0x10]
003d1344: cmn      r1, #1
003d1348: beq      #0x3d1358
003d134c: ldr      r0, [r4, #4]
003d1350: add      r0, r0, #0x3b4
003d1354: bl       #0x3db2d8
003d1358: ldr      r3, [r5, r6]
003d135c: ldr      r1, [pc, #0x6c]
003d1360: ldr      r2, [pc, #0x6c]
003d1364: ldr      r0, [r3, #0x2c]
003d1368: add      r1, pc, r1
003d136c: add      r2, pc, r2
003d1370: ldr      r5, [r4, #4]
003d1374: bl       #0x4c4bdc
003d1378: add      r5, r5, #0x3b4
003d137c: mov      r1, r0
003d1380: mov      ip, #0
003d1384: mov      r0, r5
003d1388: mvn      r2, #0
003d138c: mov      r3, #0x34
003d1390: str      ip, [sp]
003d1394: bl       #0x3dbe24
003d1398: str      r0, [r4, #0x14]
003d139c: ldr      r3, [r4, #0x20]
003d13a0: cmp      r3, #0
003d13a4: beq      #0x3d13b8
003d13a8: mov      r0, r3
003d13ac: ldr      r3, [r3]
003d13b0: mov      lr, pc
003d13b4: ldr      pc, [r3, #8]
003d13b8: add      sp, sp, #0xc
003d13bc: pop      {r4, r5, r6, r7, pc}
003d13c0: ldrheq   r3, [ip], #-0x78
003d13c4: strdeq   r3, r4, [r0], -r4
003d13c8: subeq    r0, pc, r8, asr #8
003d13cc: subeq    r4, pc, ip, ror #2
003d13d0: subeq    r0, pc, r8, ror #7
003d13d4: subeq    r4, pc, r4, lsl r1

# _ZN13DebugSwitches4saveEv
00337d54: ldr      r3, [pc, #0x74]
00337d58: ldr      r2, [pc, #0x74]
00337d5c: push     {r4, r5, r6, lr}
00337d60: add      r3, pc, r3
00337d64: ldr      r2, [r3, r2]
00337d68: sub      sp, sp, #8
00337d6c: mov      r5, r0
00337d70: ldr      r3, [r2, #0x10]
00337d74: ldr      r4, [r3, #0x34]
00337d78: cmp      r4, #0
00337d7c: beq      #0x337dc8
00337d80: ldr      r1, [pc, #0x50]
00337d84: ldr      r3, [r4]
00337d88: mov      r0, r4
00337d8c: add      r1, pc, r1
00337d90: mov      r2, #1
00337d94: mov      lr, pc
00337d98: ldr      pc, [r3, #0x94]
00337d9c: subs     r1, r0, #0
00337da0: beq      #0x337dc8
00337da4: add      r6, sp, #8
00337da8: str      r1, [r6, #-4]!
00337dac: mov      r0, r5
00337db0: bl       #0x337b4c
00337db4: mov      r0, r4
00337db8: mov      r1, r6
00337dbc: ldr      r3, [r4]
00337dc0: mov      lr, pc
00337dc4: ldr      pc, [r3, #0x78]
00337dc8: add      sp, sp, #8
00337dcc: pop      {r4, r5, r6, pc}
00337dd0: rsbeq    ip, r5, r0, lsr sp
00337dd4: strdeq   r3, r4, [r0], -r4
00337dd8: subseq   r8, r8, ip, lsr #1

# _ZN13DebugSwitches13_saveSwitchesEP11IFileStream
00337b4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00337b50: ldr      fp, [pc, #0x1ec]
00337b54: ldr      r2, [pc, #0x1ec]
00337b58: sub      sp, sp, #0x2c
00337b5c: add      fp, pc, fp
00337b60: ldr      r3, [fp, r2]
00337b64: subs     r5, r1, #0
00337b68: str      r2, [sp, #4]
00337b6c: ldr      r3, [r3]
00337b70: mov      r8, r0
00337b74: str      r3, [sp, #0x24]
00337b78: beq      #0x337cb0
00337b7c: movw     r1, #0x5357
00337b80: movt     r1, #0x4442
00337b84: mov      r0, r5
00337b88: bl       #0x33665c
00337b8c: mov      r0, r5
00337b90: mov      r1, #0x20000
00337b94: bl       #0x33665c
00337b98: mov      r0, r5
00337b9c: ldr      r1, [r8, #0x28]
00337ba0: bl       #0x33665c
00337ba4: ldr      r4, [r8, #0x20]
00337ba8: add      r6, r8, #0x18
00337bac: cmp      r4, r6
00337bb0: beq      #0x337c04
00337bb4: ldr      r1, [r4, #0x24]
00337bb8: ldr      r2, [r4, #0x20]
00337bbc: mov      r3, #0
00337bc0: mov      r0, r5
00337bc4: rsb      r2, r1, r2
00337bc8: bl       #0x317490
00337bcc: mov      r0, r5
00337bd0: ldrb     r1, [r4, #0x28]
00337bd4: bl       #0x336718
00337bd8: ldr      r2, [r4, #0xc]
00337bdc: cmp      r2, #0
00337be0: bne      #0x337bec
00337be4: b        #0x337cd0
00337be8: mov      r2, r3
00337bec: ldr      r3, [r2, #8]
00337bf0: cmp      r3, #0
00337bf4: bne      #0x337be8
00337bf8: mov      r4, r2
00337bfc: cmp      r6, r4
00337c00: bne      #0x337bb4
00337c04: mov      r0, r5
00337c08: ldr      r1, [r8, #0x10]
00337c0c: bl       #0x33665c
00337c10: ldr      r4, [r8, #8]
00337c14: cmp      r4, r8
00337c18: beq      #0x337cb0
00337c1c: ldr      r3, [pc, #0x128]
00337c20: ldr      sl, [pc, #0x128]
00337c24: add      r6, sp, #0xc
00337c28: ldr      r7, [fp, r3]
00337c2c: add      sl, pc, sl
00337c30: add      sb, sp, #8
00337c34: mov      r0, r7
00337c38: bl       #0x337888
00337c3c: mov      r2, sb
00337c40: mov      r1, sl
00337c44: mov      r0, r6
00337c48: bl       #0x3140ec
00337c4c: mov      r1, r6
00337c50: mov      r0, r7
00337c54: bl       #0x337a88
00337c58: mov      r0, r6
00337c5c: bl       #0x318254
00337c60: ldr      r1, [r4, #0x24]
00337c64: ldr      r2, [r4, #0x20]
00337c68: mov      r3, #0
00337c6c: mov      r0, r5
00337c70: rsb      r2, r1, r2
00337c74: bl       #0x317490
00337c78: mov      r0, r5
00337c7c: ldrb     r1, [r4, #0x28]
00337c80: bl       #0x336718
00337c84: ldr      r2, [r4, #0xc]
00337c88: cmp      r2, #0
00337c8c: beq      #0x337d04
00337c90: mov      r4, r2
00337c94: b        #0x337c9c
00337c98: mov      r4, r3
00337c9c: ldr      r3, [r4, #8]
00337ca0: cmp      r3, #0
00337ca4: bne      #0x337c98
00337ca8: cmp      r4, r8
00337cac: bne      #0x337c34
00337cb0: ldr      r2, [sp, #4]
00337cb4: ldr      r3, [fp, r2]
00337cb8: ldr      r2, [sp, #0x24]
00337cbc: ldr      r3, [r3]
00337cc0: cmp      r2, r3
00337cc4: bne      #0x337d40
00337cc8: add      sp, sp, #0x2c
00337ccc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00337cd0: ldr      r3, [r4, #4]
00337cd4: ldr      r1, [r3, #0xc]
00337cd8: cmp      r4, r1
00337cdc: bne      #0x337cf8
00337ce0: mov      r4, r3
00337ce4: ldr      r3, [r3, #4]
00337ce8: ldr      r2, [r3, #0xc]
00337cec: cmp      r2, r4
00337cf0: beq      #0x337ce0
00337cf4: ldr      r2, [r4, #0xc]
00337cf8: cmp      r3, r2
00337cfc: movne    r4, r3
00337d00: b        #0x337bfc
00337d04: ldr      r3, [r4, #4]
00337d08: ldr      r1, [r3, #0xc]
00337d0c: cmp      r1, r4
00337d10: bne      #0x337d2c
00337d14: mov      r4, r3
00337d18: ldr      r3, [r3, #4]
00337d1c: ldr      r2, [r3, #0xc]
00337d20: cmp      r2, r4
00337d24: beq      #0x337d14
00337d28: ldr      r2, [r4, #0xc]
00337d2c: cmp      r3, r2
00337d30: movne    r4, r3
00337d34: cmp      r4, r8
00337d38: bne      #0x337c34
00337d3c: b        #0x337cb0
00337d40: bl       #0x30e310
00337d44: rsbeq    ip, r5, r4, lsr pc
00337d48: andeq    r4, r0, ip, lsr #1
00337d4c: andeq    r0, r0, r4, lsl #17
00337d50: subseq   r8, r8, ip, ror #2

# _ZN12StreamReader10readStringEP11IStreamBasePcy
00317734: push     {r4, r5, r6, r7, r8, sb, lr}
00317738: sub      sp, sp, #0xc
0031773c: mov      sb, r3
00317740: mov      r6, r1
00317744: mov      r7, r0
00317748: mov      r8, r2
0031774c: bl       #0x313a90
00317750: mov      r3, #1
00317754: cmp      r3, #0
00317758: mov      r4, r0
0031775c: str      r0, [sp, #4]
00317760: str      r3, [sp]
00317764: bne      #0x3177b0
00317768: add      r3, sp, #4
0031776c: add      r2, r3, #2
00317770: add      r3, r3, #1
00317774: ldrb     r0, [r2, #1]
00317778: ldrb     r1, [r3, #-1]
0031777c: cmp      r3, r2
00317780: eor      r1, r0, r1
00317784: strb     r1, [r3, #-1]
00317788: ldrb     r0, [r2, #1]
0031778c: eor      r1, r1, r0
00317790: strb     r1, [r2, #1]
00317794: ldrb     r0, [r3, #-1]
00317798: sub      r2, r2, #1
0031779c: eor      r1, r1, r0
003177a0: strb     r1, [r3, #-1]
003177a4: add      r3, r3, #1
003177a8: blo      #0x317774
003177ac: ldr      r4, [sp, #4]
003177b0: mvn      r0, #0
003177b4: adds     r0, r0, r8
003177b8: mvn      r1, #0
003177bc: adc      r1, r1, sb
003177c0: mov      r3, #0
003177c4: cmp      r3, r1
003177c8: mov      r5, r4
003177cc: beq      #0x317818
003177d0: mov      r0, r7
003177d4: ldr      ip, [r7]
003177d8: mov      r1, r6
003177dc: mov      r2, r5
003177e0: mov      lr, pc
003177e4: ldr      pc, [ip, #0x18]
003177e8: mov      r0, #0
003177ec: cmp      sb, #0
003177f0: strb     r0, [r6, r5]
003177f4: bhi      #0x317810
003177f8: beq      #0x317808
003177fc: and      r0, r0, #1
00317800: add      sp, sp, #0xc
00317804: pop      {r4, r5, r6, r7, r8, sb, pc}
00317808: cmp      r8, r4
0031780c: bls      #0x3177fc
00317810: mov      r0, #1
00317814: b        #0x3177fc
00317818: cmp      r4, r0
0031781c: movhi    r5, r0
00317820: movls    r5, r4
00317824: b        #0x3177d0

# _ZNSt3mapISsbSt4lessISsESaISt4pairIKSsbEEEixISsEERbRKT_
00337288: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0033728c: ldr      r5, [pc, #0x168]
00337290: ldr      r2, [pc, #0x168]
00337294: sub      sp, sp, #0x34
00337298: add      r5, pc, r5
0033729c: ldr      r3, [r5, r2]
003372a0: str      r2, [sp, #4]
003372a4: ldr      r4, [r0, #4]
003372a8: ldr      r3, [r3]
003372ac: mov      sb, r0
003372b0: cmp      r4, #0
003372b4: str      r3, [sp, #0x2c]
003372b8: beq      #0x3373e8
003372bc: ldr      fp, [r1, #0x10]
003372c0: ldr      sl, [r1, #0x14]
003372c4: mov      r8, r0
003372c8: rsb      r7, sl, fp
003372cc: ldr      r3, [r4, #0x24]
003372d0: ldr      r6, [r4, #0x20]
003372d4: mov      r1, sl
003372d8: mov      r0, r3
003372dc: rsb      r6, r3, r6
003372e0: cmp      r7, r6
003372e4: movlt    r2, r7
003372e8: movge    r2, r6
003372ec: bl       #0x30e5e0
003372f0: cmp      r0, #0
003372f4: bne      #0x337318
003372f8: cmp      r6, r7
003372fc: blt      #0x33731c
00337300: ldr      r3, [r4, #8]
00337304: cmp      r3, #0
00337308: beq      #0x33732c
0033730c: mov      r8, r4
00337310: mov      r4, r3
00337314: b        #0x3372cc
00337318: bge      #0x337300
0033731c: ldr      r3, [r4, #0xc]
00337320: mov      r4, r8
00337324: cmp      r3, #0
00337328: bne      #0x33730c
0033732c: cmp      sb, r4
00337330: beq      #0x337370
00337334: ldr      r3, [r4, #0x24]
00337338: ldr      r7, [r4, #0x20]
0033733c: rsb      r6, sl, fp
00337340: mov      r1, r3
00337344: rsb      r7, r3, r7
00337348: cmp      r7, r6
0033734c: movlt    r2, r7
00337350: movge    r2, r6
00337354: mov      r0, sl
00337358: bl       #0x30e5e0
0033735c: cmp      r0, #0
00337360: mov      r0, r4
00337364: bne      #0x3373e0
00337368: cmp      r6, r7
0033736c: bge      #0x3373bc
00337370: add      r6, sp, #0x10
00337374: mov      r1, sl
00337378: mov      r2, fp
0033737c: mov      r0, r6
00337380: str      r6, [sp, #0x20]
00337384: str      r6, [sp, #0x24]
00337388: bl       #0x3116e8
0033738c: mov      ip, #0
00337390: mov      r1, sb
00337394: add      r0, sp, #0xc
00337398: add      r2, sp, #8
0033739c: mov      r3, r6
003373a0: strb     ip, [sp, #0x28]
003373a4: str      r4, [sp, #8]
003373a8: bl       #0x336d84
003373ac: ldr      r4, [sp, #0xc]
003373b0: mov      r0, r6
003373b4: bl       #0x318254
003373b8: mov      r0, r4
003373bc: ldr      r2, [sp, #4]
003373c0: add      r0, r0, #0x28
003373c4: ldr      r3, [r5, r2]
003373c8: ldr      r2, [sp, #0x2c]
003373cc: ldr      r3, [r3]
003373d0: cmp      r2, r3
003373d4: bne      #0x3373f8
003373d8: add      sp, sp, #0x34
003373dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003373e0: blt      #0x337370
003373e4: b        #0x3373bc
003373e8: ldr      fp, [r1, #0x10]
003373ec: ldr      sl, [r1, #0x14]
003373f0: mov      r4, r0
003373f4: b        #0x33732c
003373f8: bl       #0x30e310

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsbENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE7_M_findISsEEPNS_18_Rb_tree_node_baseERKT_
003369a8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003369ac: ldr      r4, [r0, #4]
003369b0: mov      sl, r0
003369b4: mov      sb, r0
003369b8: cmp      r4, #0
003369bc: beq      #0x336a68
003369c0: ldr      r6, [r1, #0x10]
003369c4: ldr      r7, [r1, #0x14]
003369c8: mov      r8, r0
003369cc: rsb      r6, r7, r6
003369d0: b        #0x3369ec
003369d4: cmp      r5, r6
003369d8: blt      #0x336a1c
003369dc: mov      r8, r4
003369e0: ldr      r4, [r4, #8]
003369e4: cmp      r4, #0
003369e8: beq      #0x336a28
003369ec: ldr      r3, [r4, #0x24]
003369f0: ldr      r5, [r4, #0x20]
003369f4: mov      r1, r7
003369f8: mov      r0, r3
003369fc: rsb      r5, r3, r5
00336a00: cmp      r6, r5
00336a04: movlt    r2, r6
00336a08: movge    r2, r5
00336a0c: bl       #0x30e5e0
00336a10: cmp      r0, #0
00336a14: beq      #0x3369d4
00336a18: bge      #0x3369dc
00336a1c: ldr      r4, [r4, #0xc]
00336a20: cmp      r4, #0
00336a24: bne      #0x3369ec
00336a28: cmp      r8, sl
00336a2c: beq      #0x336a64
00336a30: ldr      r3, [r8, #0x24]
00336a34: ldr      r4, [r8, #0x20]
00336a38: mov      r0, r7
00336a3c: mov      r1, r3
00336a40: rsb      r4, r3, r4
00336a44: cmp      r4, r6
00336a48: movlt    r2, r4
00336a4c: movge    r2, r6
00336a50: bl       #0x30e5e0
00336a54: cmp      r0, #0
00336a58: bne      #0x336a70
00336a5c: cmp      r6, r4
00336a60: blt      #0x336a68
00336a64: mov      sb, r8
00336a68: mov      r0, sb
00336a6c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00336a70: bge      #0x336a64
00336a74: mov      r0, sb
00336a78: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN12StreamReader6readAsIiEET_P11IStreamBase
003364ec: str      lr, [sp, #-4]!
003364f0: sub      sp, sp, #0x14
003364f4: mov      r3, #0
003364f8: ldr      ip, [r0]
003364fc: add      r1, sp, #0xc
00336500: mov      r2, #4
00336504: mov      lr, pc
00336508: ldr      pc, [ip, #0x18]
0033650c: ldr      r3, [pc, #0x78]
00336510: cmp      r0, #4
00336514: add      r3, pc, r3
00336518: beq      #0x33654c
0033651c: ldr      r2, [pc, #0x6c]
00336520: ldr      r2, [r3, r2]
00336524: ldr      r2, [r2]
00336528: cmp      r2, #2
0033652c: moveq    r3, #0
00336530: streq    r3, [r3]
00336534: beq      #0x336540
00336538: cmp      r2, #1
0033653c: beq      #0x336558
00336540: ldr      r0, [sp, #0xc]
00336544: add      sp, sp, #0x14
00336548: ldm      sp!, {pc}
0033654c: cmp      r1, #0
00336550: beq      #0x336540
00336554: b        #0x33651c
00336558: ldr      r0, [pc, #0x34]
0033655c: ldr      r1, [pc, #0x34]
00336560: ldr      r2, [pc, #0x34]
00336564: ldr      r0, [r3, r0]
00336568: ldr      r3, [pc, #0x30]
0033656c: mov      ip, #0x44
00336570: add      r1, pc, r1
00336574: add      r2, pc, r2
00336578: add      r3, pc, r3
0033657c: add      r0, r0, #0xa8
00336580: str      ip, [sp]
00336584: bl       #0x30e004
00336588: b        #0x336540
0033658c: rsbeq    lr, r5, ip, ror r5
00336590: andeq    r3, r0, r0, asr #19
00336594: andeq    r1, r0, r0, asr #19
00336598: subseq   r7, r8, r8, ror #28
0033659c: subseq   r7, r8, ip, lsl #31
003365a0: subseq   sb, r8, r8, asr #15
