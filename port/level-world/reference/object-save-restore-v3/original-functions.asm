
# _ZN10ObjectBase11DeserializeEP11IStreamBase 0033e0f8 size64
0033e0f8: push {r4, r5, r6, lr}
0033e0fc: mov r4, r0
0033e100: mov r5, r1
0033e104: mov r0, r1
0033e108: add r1, r4, #0x80
0033e10c: bl #0x33e040
0033e110: mov r0, r5
0033e114: add r1, r4, #0x8a
0033e118: bl #0x33e040
0033e11c: add r0, r4, #0x8c
0033e120: mov r1, #0
0033e124: bl #0x33dd24
0033e128: add r0, r4, #0xb0
0033e12c: mov r1, #0
0033e130: pop {r4, r5, r6, lr}
0033e134: b #0x33dd24

# _ZN10ObjectBase9SerializeEP11IStreamBase 0033e1f0 size40
0033e1f0: push {r4, r5, r6, lr}
0033e1f4: mov r4, r0
0033e1f8: mov r5, r1
0033e1fc: mov r0, r1
0033e200: add r1, r4, #0x80
0033e204: bl #0x33e138
0033e208: mov r0, r5
0033e20c: add r1, r4, #0x8a
0033e210: pop {r4, r5, r6, lr}
0033e214: b #0x33e138

# _ZN10GameObject11DeserializeEP11IStreamBase 0038b91c size192
0038b91c: push {r4, r5, lr}
0038b920: mov r5, r1
0038b924: sub sp, sp, #0x14
0038b928: mov r4, r0
0038b92c: bl #0x33e0f8
0038b930: add r1, sp, #0xc
0038b934: mov r0, r5
0038b938: bl #0x38b758
0038b93c: ldr r1, [r4, #0x270]
0038b940: ldr r2, [sp, #0xc]
0038b944: ldr r3, [pc, #0x78]
0038b948: cmp r1, r2
0038b94c: add r3, pc, r3
0038b950: beq #0x38b978
0038b954: ldr r2, [pc, #0x6c]
0038b958: ldr r2, [r3, r2]
0038b95c: ldr r2, [r2]
0038b960: cmp r2, #2
0038b964: moveq r3, #0
0038b968: streq r3, [r3]
0038b96c: beq #0x38b978
0038b970: cmp r2, #1
0038b974: beq #0x38b990
0038b978: ldr r0, [r4, #0x2d8]
0038b97c: cmp r0, #0
0038b980: beq #0x38b988
0038b984: bl #0x4713d0
0038b988: add sp, sp, #0x14
0038b98c: pop {r4, r5, pc}
0038b990: ldr r0, [pc, #0x34]
0038b994: ldr r1, [pc, #0x34]
0038b998: ldr r2, [pc, #0x34]
0038b99c: ldr r0, [r3, r0]
0038b9a0: ldr r3, [pc, #0x30]
0038b9a4: movw ip, #0x11e
0038b9a8: add r1, pc, r1
0038b9ac: add r2, pc, r2
0038b9b0: add r3, pc, r3
0038b9b4: add r0, r0, #0xa8
0038b9b8: str ip, [sp]
0038b9bc: bl #0x30e004
0038b9c0: b #0x38b978
0038b9c4: rsbeq sb, r0, r4, asr #2
0038b9c8: andeq r3, r0, r0, asr #19
0038b9cc: andeq r1, r0, r0, asr #19
0038b9d0: subseq r2, r3, r0, lsr sl
0038b9d4: subseq r6, r3, ip, lsr sl
0038b9d8: ldrsheq r6, [r3], #-0x90

# _ZN10GameObject9SerializeEP11IStreamBase 0038b9e4 size32
0038b9e4: push {r4, r5, r6, lr}
0038b9e8: mov r4, r0
0038b9ec: mov r5, r1
0038b9f0: bl #0x33e1f0
0038b9f4: mov r0, r5
0038b9f8: add r1, r4, #0x270
0038b9fc: pop {r4, r5, r6, lr}
0038ba00: b #0x38b808

# _ZN9Character11DeserializeEP11IStreamBase 003a6210 size940
003a6210: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a6214: sub sp, sp, #0x14
003a6218: mov r6, r1
003a621c: mov r4, r0
003a6220: bl #0x3a3064
003a6224: ldr r5, [pc, #0x388]
003a6228: cmp r0, #0
003a622c: add r5, pc, r5
003a6230: bne #0x3a6430
003a6234: add r7, r4, #0x560
003a6238: mov r0, r7
003a623c: bl #0x3e0af8
003a6240: mov r0, r4
003a6244: mov r1, r6
003a6248: bl #0x38b91c
003a624c: mov r0, r6
003a6250: add r1, sp, #8
003a6254: bl #0x38b758
003a6258: ldr r3, [r4]
003a625c: mov r0, r4
003a6260: mov lr, pc
003a6264: ldr pc, [r3, #0x28]
003a6268: cmp r0, #0
003a626c: beq #0x3a644c
003a6270: ldr r3, [r4]
003a6274: mov r0, r4
003a6278: mov r1, #1
003a627c: mov lr, pc
003a6280: ldr pc, [r3, #0x40]
003a6284: mov r0, r6
003a6288: add r1, r4, #0x160
003a628c: add fp, r4, #0x16c
003a6290: bl #0x3a3a20
003a6294: mov r1, fp
003a6298: mov r0, r6
003a629c: bl #0x3a3a20
003a62a0: add r8, r4, #0x1440
003a62a4: mov r0, r6
003a62a8: add r1, sp, #0xf
003a62ac: bl #0x33e040
003a62b0: add r1, r8, #0x28
003a62b4: mov r0, r6
003a62b8: bl #0x3a3a20
003a62bc: mov r0, r6
003a62c0: add r1, r8, #0x34
003a62c4: bl #0x3a3a20
003a62c8: ldr r3, [r4, #0x3fc]
003a62cc: cmp r3, #0
003a62d0: beq #0x3a6314
003a62d4: add r1, sp, #0x10
003a62d8: mov r3, #0
003a62dc: str r3, [r1, #-0xc]!
003a62e0: mov r0, r6
003a62e4: bl #0x38b758
003a62e8: ldr r3, [r4, #0x3fc]
003a62ec: ldr r2, [sp, #4]
003a62f0: mov r0, r6
003a62f4: str r2, [r3, #0x24]
003a62f8: ldr r1, [r4, #0x3fc]
003a62fc: add r1, r1, #0x28
003a6300: bl #0x33e040
003a6304: ldr r1, [r4, #0x3fc]
003a6308: mov r0, r6
003a630c: add r1, r1, #0x29
003a6310: bl #0x33e040
003a6314: movw r3, #0x1449
003a6318: ldrb r2, [r4, r3]
003a631c: cmp r2, #0
003a6320: bne #0x3a64e4
003a6324: ldrb sb, [sp, #0xf]
003a6328: cmp sb, #0
003a632c: bne #0x3a6544
003a6330: add sl, r4, #0x4f0
003a6334: add sl, sl, #0xc
003a6338: movw r3, #0x1449
003a633c: add r6, r4, #0x3c8
003a6340: strb sb, [r4, r3]
003a6344: mov r0, r6
003a6348: bl #0x3cfd7c
003a634c: mov r0, r6
003a6350: bl #0x3cfde4
003a6354: ldr r3, [sp, #8]
003a6358: cmp r3, #0x11
003a635c: bls #0x3a6498
003a6360: movw r3, #0x1449
003a6364: ldrb r2, [r4, r3]
003a6368: cmp r2, #0
003a636c: bne #0x3a6570
003a6370: ldr r3, [r4, #0x2dc]
003a6374: cmp r3, #0
003a6378: beq #0x3a65a8
003a637c: mov r0, sl
003a6380: mov r1, #0
003a6384: bl #0x3c1a00
003a6388: mov r1, #0
003a638c: mov r2, r1
003a6390: mov r0, r6
003a6394: bl #0x3d6890
003a6398: mov r0, r6
003a639c: bl #0x3d49c4
003a63a0: mov r0, r4
003a63a4: bl #0x3a3064
003a63a8: cmp r0, #0
003a63ac: bne #0x3a63e0
003a63b0: ldr r3, [r4]
003a63b4: mov r0, r4
003a63b8: mov lr, pc
003a63bc: ldr pc, [r3, #0x28]
003a63c0: cmp r0, #0
003a63c4: beq #0x3a6470
003a63c8: ldr r3, [pc, #0x1e8]
003a63cc: ldr r0, [r5, r3]
003a63d0: bl #0x31f594
003a63d4: ldrb r3, [r0, #0xf2]
003a63d8: cmp r3, #0
003a63dc: beq #0x3a6470
003a63e0: add r1, r8, #0x10
003a63e4: mov r0, r4
003a63e8: mov r2, #1
003a63ec: bl #0x393db4
003a63f0: mov r0, r4
003a63f4: add r1, r8, #0x1c
003a63f8: bl #0x3938a0
003a63fc: ldr r0, [r4, #0x2d8]
003a6400: cmp r0, #0
003a6404: beq #0x3a640c
003a6408: bl #0x38ba74
003a640c: ldr r3, [r4, #0x2e0]
003a6410: cmp r3, #0
003a6414: beq #0x3a6428
003a6418: mov r0, r3
003a641c: ldr r3, [r3]
003a6420: mov lr, pc
003a6424: ldr pc, [r3, #8]
003a6428: add sp, sp, #0x14
003a642c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a6430: ldr r3, [pc, #0x180]
003a6434: ldr r0, [r5, r3]
003a6438: bl #0x31f594
003a643c: ldrb r3, [r0, #0xf3]
003a6440: cmp r3, #0
003a6444: bne #0x3a6428
003a6448: b #0x3a6234
003a644c: add r1, r4, #0x8e0
003a6450: mov r0, r6
003a6454: add r1, r1, #0xc
003a6458: bl #0x3a3970
003a645c: add r1, r4, #0xff0
003a6460: mov r0, r6
003a6464: add r1, r1, #4
003a6468: bl #0x3a3970
003a646c: b #0x3a6284
003a6470: mov r0, r4
003a6474: bl #0x3935dc
003a6478: mov r2, #1
003a647c: mov r1, r0
003a6480: mov r0, r4
003a6484: bl #0x393db4
003a6488: mov r0, r4
003a648c: mov r1, fp
003a6490: bl #0x3938a0
003a6494: b #0x3a63fc
003a6498: mov r1, #1
003a649c: lsl r1, r1, r3
003a64a0: movw r3, #0x1005
003a64a4: movt r3, #0
003a64a8: and r3, r1, r3
003a64ac: cmp r3, #0
003a64b0: bne #0x3a6524
003a64b4: movw r2, #2
003a64b8: movt r2, #2
003a64bc: and r2, r1, r2
003a64c0: cmp r2, #0
003a64c4: beq #0x3a6360
003a64c8: mov r0, sl
003a64cc: bl #0x3c01d4
003a64d0: cmp r0, #0
003a64d4: bne #0x3a6388
003a64d8: mov r0, sl
003a64dc: bl #0x3c1a64
003a64e0: b #0x3a6388
003a64e4: ldrb sb, [sp, #0xf]
003a64e8: cmp sb, #0
003a64ec: bne #0x3a6330
003a64f0: add sl, r4, #0x4f0
003a64f4: mov r1, sb
003a64f8: mov r0, r4
003a64fc: mov r2, #1
003a6500: add sl, sl, #0xc
003a6504: bl #0x3a59ac
003a6508: mov r1, sb
003a650c: mov r0, sl
003a6510: bl #0x3c1a00
003a6514: ldr r3, [r4, #0x378]
003a6518: strb sb, [r3, #8]
003a651c: ldrb sb, [sp, #0xf]
003a6520: b #0x3a6338
003a6524: mov r0, sl
003a6528: bl #0x3c01c0
003a652c: cmp r0, #0
003a6530: beq #0x3a6558
003a6534: mov r2, #1
003a6538: movw r3, #0x1449
003a653c: strb r2, [r4, r3]
003a6540: b #0x3a6388
003a6544: mov r0, r7
003a6548: mov r1, #0x24
003a654c: bl #0x3e07a0
003a6550: ldrb sb, [sp, #0xf]
003a6554: b #0x3a6330
003a6558: mov r0, r4
003a655c: bl #0x3a5248
003a6560: mov r1, r0
003a6564: mov r0, sl
003a6568: bl #0x3c1a74
003a656c: b #0x3a6534
003a6570: mov r2, #1
003a6574: strb r2, [r4, r3]
003a6578: mov r1, #0x24
003a657c: mov r2, #0
003a6580: mov r0, r7
003a6584: bl #0x3e07a0
003a6588: mov r0, r6
003a658c: bl #0x3d6cdc
003a6590: mov r0, r4
003a6594: bl #0x3a5248
003a6598: mov r1, r0
003a659c: mov r0, sl
003a65a0: bl #0x3c1a74
003a65a4: b #0x3a6388
003a65a8: mov r0, r4
003a65ac: bl #0x3b4088
003a65b0: b #0x3a637c
003a65b4: subseq lr, lr, r4, ror #16
003a65b8: strdeq r3, r4, [r0], -r4

# _ZN9Character9SerializeEP11IStreamBase 003a65c4 size248
003a65c4: push {r4, r5, r6, lr}
003a65c8: mov r4, r0
003a65cc: sub sp, sp, #8
003a65d0: mov r5, r1
003a65d4: bl #0x38b9e4
003a65d8: add r0, r4, #0x4f0
003a65dc: add r0, r0, #0xc
003a65e0: bl #0x3c01ac
003a65e4: add r1, sp, #8
003a65e8: str r0, [r1, #-4]!
003a65ec: mov r0, r5
003a65f0: bl #0x38b808
003a65f4: ldr r3, [r4]
003a65f8: mov r0, r4
003a65fc: mov lr, pc
003a6600: ldr pc, [r3, #0x28]
003a6604: cmp r0, #0
003a6608: beq #0x3a6698
003a660c: add r1, r4, #0x160
003a6610: mov r0, r5
003a6614: bl #0x3a3b80
003a6618: add r6, r4, #0x1440
003a661c: mov r0, r5
003a6620: add r1, r4, #0x16c
003a6624: bl #0x3a3b80
003a6628: add r1, r6, #9
003a662c: mov r0, r5
003a6630: bl #0x33e138
003a6634: mov r0, r5
003a6638: add r1, r6, #0x28
003a663c: bl #0x3a3b80
003a6640: mov r0, r5
003a6644: add r1, r6, #0x34
003a6648: bl #0x3a3b80
003a664c: ldr r3, [r4, #0x3fc]
003a6650: cmp r3, #0
003a6654: beq #0x3a6690
003a6658: ldr r3, [r3, #0x24]
003a665c: add r1, sp, #8
003a6660: mov r0, r5
003a6664: str r3, [r1, #-8]!
003a6668: mov r1, sp
003a666c: bl #0x38b808
003a6670: ldr r1, [r4, #0x3fc]
003a6674: mov r0, r5
003a6678: add r1, r1, #0x28
003a667c: bl #0x33e138
003a6680: ldr r1, [r4, #0x3fc]
003a6684: mov r0, r5
003a6688: add r1, r1, #0x29
003a668c: bl #0x33e138
003a6690: add sp, sp, #8
003a6694: pop {r4, r5, r6, pc}
003a6698: add r1, r4, #0x8e0
003a669c: mov r0, r5
003a66a0: add r1, r1, #0xc
003a66a4: bl #0x3a3ad0
003a66a8: add r1, r4, #0xff0
003a66ac: mov r0, r5
003a66b0: add r1, r1, #4
003a66b4: bl #0x3a3ad0
003a66b8: b #0x3a660c
