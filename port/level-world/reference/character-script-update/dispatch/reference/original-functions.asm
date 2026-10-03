
# _ZN9LuaScript4CallEPKc
0037c514: ldr      r3, [pc, #0x60]
0037c518: ldr      r2, [pc, #0x60]
0037c51c: push     {r4, r5, r6, r7, lr}
0037c520: add      r3, pc, r3
0037c524: ldr      r5, [r3, r2]
0037c528: sub      sp, sp, #0x34
0037c52c: add      r4, sp, #4
0037c530: ldr      r2, [r5]
0037c534: mov      r6, r0
0037c538: mov      r7, r1
0037c53c: mov      r0, r4
0037c540: str      r2, [sp, #0x2c]
0037c544: bl       #0x31b434
0037c548: mov      r2, r4
0037c54c: mov      r0, r6
0037c550: mov      r1, r7
0037c554: bl       #0x37c494
0037c558: mov      r0, r4
0037c55c: bl       #0x31b398
0037c560: ldr      r2, [sp, #0x2c]
0037c564: ldr      r3, [r5]
0037c568: cmp      r2, r3
0037c56c: bne      #0x37c578
0037c570: add      sp, sp, #0x34
0037c574: pop      {r4, r5, r6, r7, pc}
0037c578: bl       #0x30e310
0037c57c: rsbeq    r8, r1, r0, ror r5
0037c580: andeq    r4, r0, ip, lsr #1

# _ZN11Application5GetDtEv
0031f66c: ldr      r0, [r0, #0x8c]
0031f670: bx       lr

# _ZNK16CharStateMachine11SM_IsMovingEb
003c029c: push     {r4, lr}
003c02a0: mov      r4, r1
003c02a4: bl       #0x3c01ac
003c02a8: cmp      r0, #4
003c02ac: beq      #0x3c02c0
003c02b0: cmp      r0, #0x13
003c02b4: beq      #0x3c02c8
003c02b8: mov      r0, #0
003c02bc: pop      {r4, pc}
003c02c0: mov      r0, #1
003c02c4: pop      {r4, pc}
003c02c8: eor      r0, r4, #1
003c02cc: pop      {r4, pc}

# _ZN10LuaManager7AddFileEP9LuaScriptPKc
0037b23c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037b240: ldr      r4, [pc, #0x2e0]
0037b244: ldr      r6, [pc, #0x2e0]
0037b248: sub      sp, sp, #0x7c
0037b24c: add      r4, pc, r4
0037b250: ldr      r3, [r4, r6]
0037b254: subs     r7, r1, #0
0037b258: mov      sl, r0
0037b25c: ldr      r3, [r3]
0037b260: mov      r5, r2
0037b264: str      r3, [sp, #0x74]
0037b268: beq      #0x37b318
0037b26c: cmp      r5, #0
0037b270: beq      #0x37b280
0037b274: ldrsb    r3, [r5]
0037b278: cmp      r3, #0
0037b27c: bne      #0x37b2a4
0037b280: mov      r5, #0
0037b284: ldr      r3, [r4, r6]
0037b288: ldr      r2, [sp, #0x74]
0037b28c: mov      r0, r5
0037b290: ldr      r3, [r3]
0037b294: cmp      r2, r3
0037b298: bne      #0x37b524
0037b29c: add      sp, sp, #0x7c
0037b2a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037b2a4: add      r8, sp, #0x5c
0037b2a8: mov      r0, r8
0037b2ac: add      r1, r7, #0x68
0037b2b0: mov      r2, r5
0037b2b4: bl       #0x3338cc
0037b2b8: ldr      r1, [pc, #0x270]
0037b2bc: mov      r0, r5
0037b2c0: add      r1, pc, r1
0037b2c4: bl       #0x30ebd4
0037b2c8: cmp      r0, #0
0037b2cc: beq      #0x37b3f8
0037b2d0: ldr      r1, [pc, #0x25c]
0037b2d4: mov      r2, #5
0037b2d8: add      r1, pc, r1
0037b2dc: bl       #0x30ec7c
0037b2e0: cmp      r0, #0
0037b2e4: bne      #0x37b36c
0037b2e8: ldr      r3, [sp, #0x70]
0037b2ec: add      r1, sp, #0x78
0037b2f0: add      r5, r7, #0x80
0037b2f4: str      r3, [r1, #-0x5c]!
0037b2f8: mov      r0, r5
0037b2fc: bl       #0x37a190
0037b300: cmp      r5, r0
0037b304: beq      #0x37b380
0037b308: mov      r5, #1
0037b30c: mov      r0, r8
0037b310: bl       #0x3139ac
0037b314: b        #0x37b284
0037b318: ldr      r3, [pc, #0x218]
0037b31c: ldr      r3, [r4, r3]
0037b320: ldr      r3, [r3]
0037b324: cmp      r3, #2
0037b328: streq    r7, [r7]
0037b32c: beq      #0x37b26c
0037b330: cmp      r3, #1
0037b334: bne      #0x37b26c
0037b338: ldr      r0, [pc, #0x1fc]
0037b33c: ldr      r1, [pc, #0x1fc]
0037b340: ldr      r2, [pc, #0x1fc]
0037b344: ldr      r0, [r4, r0]
0037b348: ldr      r3, [pc, #0x1f8]
0037b34c: mov      ip, #0x23
0037b350: add      r1, pc, r1
0037b354: add      r2, pc, r2
0037b358: add      r3, pc, r3
0037b35c: add      r0, r0, #0xa8
0037b360: str      ip, [sp]
0037b364: bl       #0x30e004
0037b368: b        #0x37b26c
0037b36c: ldr      r1, [pc, #0x1d8]
0037b370: mov      r0, r8
0037b374: add      r1, pc, r1
0037b378: bl       #0x379ef8
0037b37c: b        #0x37b2e8
0037b380: ldr      r3, [sp, #0x70]
0037b384: add      r1, sp, #0x78
0037b388: add      sl, sl, #4
0037b38c: str      r3, [r1, #-0x60]!
0037b390: mov      r0, sl
0037b394: bl       #0x37a300
0037b398: cmp      r0, sl
0037b39c: mov      sb, r0
0037b3a0: beq      #0x37b444
0037b3a4: ldr      sl, [r0, #0x28]
0037b3a8: mov      r2, #0
0037b3ac: mov      r3, #0
0037b3b0: ldr      r1, [sl]
0037b3b4: mov      r0, sl
0037b3b8: mov      lr, pc
0037b3bc: ldr      pc, [r1, #0x20]
0037b3c0: cmp      sl, #0
0037b3c4: beq      #0x37b4d0
0037b3c8: add      sb, sp, #0x24
0037b3cc: add      r1, r7, #4
0037b3d0: mov      r2, sl
0037b3d4: mov      r0, sb
0037b3d8: bl       #0x31acf4
0037b3dc: ldr      r3, [sp, #0x28]
0037b3e0: cmp      r3, #0
0037b3e4: beq      #0x37b40c
0037b3e8: mov      r0, sb
0037b3ec: bl       #0x31a68c
0037b3f0: mov      r5, #0
0037b3f4: b        #0x37b30c
0037b3f8: ldr      r1, [pc, #0x150]
0037b3fc: mov      r0, r8
0037b400: add      r1, pc, r1
0037b404: bl       #0x379ef8
0037b408: b        #0x37b2e8
0037b40c: add      r7, sp, #0x44
0037b410: mov      r0, sb
0037b414: bl       #0x31a68c
0037b418: ldr      r1, [sp, #0x70]
0037b41c: add      r2, sp, #0x20
0037b420: mov      r0, r7
0037b424: bl       #0x3140ec
0037b428: add      r0, sp, #8
0037b42c: mov      r1, r5
0037b430: mov      r2, r7
0037b434: bl       #0x37a9e8
0037b438: mov      r0, r7
0037b43c: bl       #0x3139ac
0037b440: b        #0x37b308
0037b444: ldr      r3, [pc, #0x108]
0037b448: mov      r2, #0
0037b44c: ldr      r1, [sp, #0x70]
0037b450: ldr      fp, [r4, r3]
0037b454: mov      r3, r2
0037b458: ldr      r0, [fp, #0x10]
0037b45c: ldr      ip, [r0, #0x34]
0037b460: mov      r0, ip
0037b464: ldr      ip, [ip]
0037b468: mov      lr, pc
0037b46c: ldr      pc, [ip, #0x88]
0037b470: cmp      r0, #0
0037b474: str      r0, [sp, #0x14]
0037b478: moveq    r5, r0
0037b47c: beq      #0x37b30c
0037b480: mov      r1, #0
0037b484: mov      r0, #0x30
0037b488: bl       #0x310570
0037b48c: ldr      r1, [sp, #0x14]
0037b490: mov      sl, r0
0037b494: bl       #0x3172d8
0037b498: ldr      r3, [sp, #0x70]
0037b49c: add      r1, sp, #0x78
0037b4a0: mov      r0, sb
0037b4a4: str      r3, [r1, #-0x68]!
0037b4a8: bl       #0x37b0fc
0037b4ac: str      sl, [r0]
0037b4b0: ldr      r3, [fp, #0x10]
0037b4b4: add      r1, sp, #0x14
0037b4b8: ldr      r3, [r3, #0x34]
0037b4bc: mov      r0, r3
0037b4c0: ldr      r3, [r3]
0037b4c4: mov      lr, pc
0037b4c8: ldr      pc, [r3, #0x78]
0037b4cc: b        #0x37b3c0
0037b4d0: ldr      r3, [pc, #0x60]
0037b4d4: ldr      r3, [r4, r3]
0037b4d8: ldr      r3, [r3]
0037b4dc: cmp      r3, #2
0037b4e0: streq    sl, [sl]
0037b4e4: beq      #0x37b3c8
0037b4e8: cmp      r3, #1
0037b4ec: bne      #0x37b3c8
0037b4f0: ldr      r0, [pc, #0x44]
0037b4f4: ldr      r1, [pc, #0x5c]
0037b4f8: ldr      r2, [pc, #0x5c]
0037b4fc: ldr      r0, [r4, r0]
0037b500: ldr      r3, [pc, #0x58]
0037b504: mov      ip, #0x61
0037b508: add      r1, pc, r1
0037b50c: add      r2, pc, r2
0037b510: add      r3, pc, r3
0037b514: add      r0, r0, #0xa8
0037b518: str      ip, [sp]
0037b51c: bl       #0x30e004
0037b520: b        #0x37b3c8
0037b524: bl       #0x30e310
0037b528: rsbeq    sb, r1, r4, asr #16
0037b52c: andeq    r4, r0, ip, lsr #1
0037b530: subseq   r6, r4, r0, ror r7
0037b534: subseq   r6, r4, r0, ror #14
0037b538: andeq    r3, r0, r0, asr #19
0037b53c: andeq    r1, r0, r0, asr #19
0037b540: subseq   r3, r4, r8, lsl #1
0037b544: subseq   r6, r4, r4, lsl #13
0037b548: subseq   r6, r4, r8, lsl #13
0037b54c: subseq   r6, r7, r4, lsl sp
0037b550: subseq   r6, r4, r8, lsr r6
0037b554: strdeq   r3, r4, [r0], -r4
0037b558: ldrsbeq  r2, [r4], #-0xe0
0037b55c: subseq   r6, r4, r4, lsr r5
0037b560: ldrsbeq  r6, [r4], #-0x40

# _ZN10AISDefault7InitVCBEv
003dc7d8: ldr      r1, [pc, #0x4c]
003dc7dc: mov      r3, #0
003dc7e0: push     {r4, r5, r6, lr}
003dc7e4: add      r1, pc, r1
003dc7e8: str      r3, [r0, #0xb8]
003dc7ec: mov      r4, r0
003dc7f0: bl       #0x37c2a0
003dc7f4: ldr      r1, [pc, #0x34]
003dc7f8: cmp      r0, #0
003dc7fc: movne    r5, #0x800
003dc800: moveq    r5, #0
003dc804: str      r5, [r4, #0xb8]
003dc808: add      r1, pc, r1
003dc80c: mov      r0, r4
003dc810: bl       #0x37c2a0
003dc814: cmp      r0, #0
003dc818: movne    r0, #0x1000
003dc81c: moveq    r0, #0
003dc820: orr      r5, r0, r5
003dc824: str      r5, [r4, #0xb8]
003dc828: pop      {r4, r5, r6, pc}
003dc82c: subeq    sb, lr, ip, ror r1
003dc830: subeq    sb, lr, r8, ror #2

# _ZN6CharAI8OnUpdateEv
003d1050: push     {r4, lr}
003d1054: ldr      r3, [r0, #0x1c]
003d1058: sub      sp, sp, #0x10
003d105c: mov      r4, r0
003d1060: cmp      r3, #0
003d1064: beq      #0x3d1078
003d1068: mov      r0, r3
003d106c: ldr      r3, [r3]
003d1070: mov      lr, pc
003d1074: ldr      pc, [r3, #0x18]
003d1078: ldr      r0, [r4, #4]
003d107c: mov      r1, #0
003d1080: add      r0, r0, #0x4f0
003d1084: add      r0, r0, #0xc
003d1088: bl       #0x3c0260
003d108c: cmp      r0, #0
003d1090: beq      #0x3d10c8
003d1094: ldr      r3, [r4, #4]
003d1098: ldr      r2, [r3, #0x408]
003d109c: cmp      r2, #0
003d10a0: beq      #0x3d10f4
003d10a4: ldr      r4, [r3, #0x2d8]
003d10a8: mov      r0, r3
003d10ac: bl       #0x38c600
003d10b0: cmp      r4, #0
003d10b4: beq      #0x3d10c0
003d10b8: mov      r0, r4
003d10bc: bl       #0x4713d0
003d10c0: add      sp, sp, #0x10
003d10c4: pop      {r4, pc}
003d10c8: ldr      r0, [r4, #4]
003d10cc: add      r0, r0, #0x4f0
003d10d0: add      r0, r0, #0xc
003d10d4: bl       #0x3c0230
003d10d8: cmp      r0, #0
003d10dc: ldreq    r3, [r4, #4]
003d10e0: beq      #0x3d10a4
003d10e4: ldr      r3, [r4, #4]
003d10e8: ldr      r2, [r3, #0x408]
003d10ec: cmp      r2, #0
003d10f0: bne      #0x3d10a4
003d10f4: ldr      r2, [r3, #0x418]
003d10f8: cmp      r2, #0
003d10fc: bne      #0x3d10a4
003d1100: ldrb     r2, [r3, #0x2ee]
003d1104: cmp      r2, #0
003d1108: bne      #0x3d10c0
003d110c: mov      r0, r3
003d1110: ldr      r3, [r3]
003d1114: mov      lr, pc
003d1118: ldr      pc, [r3, #0xc4]
003d111c: cmp      r0, #0
003d1120: beq      #0x3d10c0
003d1124: ldr      r0, [r4, #4]
003d1128: bl       #0x38c790
003d112c: ldr      r3, [r4, #4]
003d1130: mov      r0, r3
003d1134: ldr      r3, [r3]
003d1138: mov      lr, pc
003d113c: ldr      pc, [r3, #0x34]
003d1140: cmp      r0, #0
003d1144: bne      #0x3d10c0
003d1148: ldr      r0, [r4, #4]
003d114c: ldrb     r3, [r0, #0x85]
003d1150: cmp      r3, #0
003d1154: bne      #0x3d10c0
003d1158: add      r1, r0, #0x1440
003d115c: mov      r2, #1
003d1160: add      r1, r1, #0x10
003d1164: bl       #0x393db4
003d1168: ldr      r3, [r4, #4]
003d116c: ldr      r2, [r3, #0x2d8]
003d1170: cmp      r2, #0
003d1174: beq      #0x3d10c0
003d1178: ldr      r0, [r2, #8]
003d117c: cmp      r0, #0
003d1180: beq      #0x3d10c0
003d1184: movw     r2, #0x1450
003d1188: ldr      lr, [r3, r2]
003d118c: movw     r2, #0x1454
003d1190: ldr      ip, [r3, r2]
003d1194: movw     r2, #0x1458
003d1198: ldr      r2, [r3, r2]
003d119c: ldr      r3, [r0]
003d11a0: add      r1, sp, #4
003d11a4: ldr      r3, [r3, #0xa4]
003d11a8: str      lr, [sp, #4]
003d11ac: str      ip, [sp, #8]
003d11b0: str      r2, [sp, #0xc]
003d11b4: blx      r3
003d11b8: b        #0x3d10c0

# _ZN10AISDefault13OnScriptTimerEj
003dcc80: push     {r4, r5, r6, lr}
003dcc84: sub      sp, sp, #8
003dcc88: mov      r5, r0
003dcc8c: mov      r6, r1
003dcc90: mov      r0, sp
003dcc94: bl       #0x3192b4
003dcc98: mov      r0, sp
003dcc9c: mov      r1, r6
003dcca0: bl       #0x3cdd78
003dcca4: ldr      r1, [pc, #0x20]
003dcca8: mov      r0, r5
003dccac: mov      r2, sp
003dccb0: add      r1, pc, r1
003dccb4: bl       #0x37c41c
003dccb8: mov      r0, sp
003dccbc: mov      r4, sp
003dccc0: bl       #0x319228
003dccc4: add      sp, sp, #8
003dccc8: pop      {r4, r5, r6, pc}
003dcccc: strdeq   r8, sb, [lr], #-0xc0

# _ZN10AISDefault11OnAnimEventEPKc
003dca50: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dca54: sub      sp, sp, #0x3c
003dca58: add      r5, sp, #0x30
003dca5c: mov      r6, r0
003dca60: mov      r7, r1
003dca64: mov      r0, r5
003dca68: bl       #0x3192b4
003dca6c: mov      r0, r5
003dca70: mov      r1, r7
003dca74: bl       #0x39ec10
003dca78: ldr      r3, [r6, #0x98]
003dca7c: mov      r0, r5
003dca80: ldr      r4, [pc, #0x1d0]
003dca84: ldr      r1, [r3, #0x4f4]
003dca88: bl       #0x3cdd78
003dca8c: ldr      r1, [pc, #0x1c8]
003dca90: mov      r0, r6
003dca94: mov      r2, r5
003dca98: add      r1, pc, r1
003dca9c: bl       #0x37c41c
003dcaa0: ldr      r1, [pc, #0x1b8]
003dcaa4: mov      r0, r7
003dcaa8: add      r4, pc, r4
003dcaac: add      r1, pc, r1
003dcab0: bl       #0x30e31c
003dcab4: cmp      r0, #0
003dcab8: beq      #0x3dcad4
003dcabc: ldr      r1, [pc, #0x1a0]
003dcac0: mov      r0, r7
003dcac4: add      r1, pc, r1
003dcac8: bl       #0x30e31c
003dcacc: cmp      r0, #0
003dcad0: bne      #0x3dcc0c
003dcad4: ldr      r1, [pc, #0x18c]
003dcad8: mov      r3, #0
003dcadc: mov      r0, r7
003dcae0: add      r1, pc, r1
003dcae4: str      r3, [sp, #0x2c]
003dcae8: str      r3, [sp, #0x24]
003dcaec: str      r3, [sp, #0x28]
003dcaf0: bl       #0x30e31c
003dcaf4: cmp      r0, #0
003dcaf8: beq      #0x3dcc1c
003dcafc: add      r0, sp, #0xc
003dcb00: ldr      r1, [r6, #0x98]
003dcb04: bl       #0x3a57f4
003dcb08: ldr      r3, [sp, #0xc]
003dcb0c: str      r3, [sp, #0x24]
003dcb10: ldr      r3, [sp, #0x10]
003dcb14: str      r3, [sp, #0x28]
003dcb18: ldr      r3, [sp, #0x14]
003dcb1c: str      r3, [sp, #0x2c]
003dcb20: ldr      r0, [r6, #0x98]
003dcb24: bl       #0x3a3300
003dcb28: ldr      r7, [pc, #0x13c]
003dcb2c: mov      ip, #0
003dcb30: add      fp, sp, #0x24
003dcb34: mov      r1, r0
003dcb38: mov      r3, ip
003dcb3c: mov      r2, fp
003dcb40: ldr      r0, [r4, r7]
003dcb44: str      ip, [sp]
003dcb48: bl       #0x495d14
003dcb4c: ldr      r2, [r6, #0x98]
003dcb50: movw     r3, #0x1014
003dcb54: ldr      r3, [r2, r3]
003dcb58: cmp      r3, #0
003dcb5c: blt      #0x3dcc44
003dcb60: ldr      r1, [pc, #0x108]
003dcb64: ldr      r1, [r4, r1]
003dcb68: ldr      r1, [r1]
003dcb6c: cmp      r3, r1
003dcb70: bge      #0x3dcc44
003dcb74: ldr      r1, [pc, #0xf8]
003dcb78: mov      r0, #0x18
003dcb7c: ldr      r1, [r4, r1]
003dcb80: ldr      r1, [r1]
003dcb84: mla      r3, r0, r3, r1
003dcb88: ldrb     r3, [r3, #0x14]
003dcb8c: ldr      sl, [r2, #0x1d8]
003dcb90: cmp      sl, #0
003dcb94: ldrne    sl, [sl, #0x3c]
003dcb98: cmp      r3, #0
003dcb9c: beq      #0x3dcc0c
003dcba0: cmp      sl, #0
003dcba4: beq      #0x3dcc0c
003dcba8: ldr      r3, [pc, #0xc8]
003dcbac: ldr      r3, [r4, r3]
003dcbb0: ldr      sb, [r3]
003dcbb4: cmp      sb, #0
003dcbb8: ble      #0x3dcc0c
003dcbbc: ldr      r3, [pc, #0xb8]
003dcbc0: mov      r8, #0
003dcbc4: ldr      r3, [r4, r3]
003dcbc8: ldr      r6, [r3]
003dcbcc: b        #0x3dcbdc
003dcbd0: cmp      r8, sb
003dcbd4: add      r6, r6, #0x20
003dcbd8: beq      #0x3dcc0c
003dcbdc: ldr      r0, [r6, #0xc]
003dcbe0: mov      r1, sl
003dcbe4: bl       #0x30e31c
003dcbe8: subs     ip, r0, #0
003dcbec: add      r8, r8, #1
003dcbf0: bne      #0x3dcbd0
003dcbf4: ldr      r1, [r6, #4]
003dcbf8: ldr      r0, [r4, r7]
003dcbfc: mov      r2, fp
003dcc00: mov      r3, ip
003dcc04: str      ip, [sp]
003dcc08: bl       #0x495d14
003dcc0c: mov      r0, r5
003dcc10: bl       #0x319228
003dcc14: add      sp, sp, #0x3c
003dcc18: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003dcc1c: add      r0, sp, #0x18
003dcc20: ldr      r1, [r6, #0x98]
003dcc24: bl       #0x3a5874
003dcc28: ldr      r3, [sp, #0x18]
003dcc2c: str      r3, [sp, #0x24]
003dcc30: ldr      r3, [sp, #0x1c]
003dcc34: str      r3, [sp, #0x28]
003dcc38: ldr      r3, [sp, #0x20]
003dcc3c: str      r3, [sp, #0x2c]
003dcc40: b        #0x3dcb20
003dcc44: ldr      r3, [pc, #0x28]
003dcc48: ldr      r3, [r4, r3]
003dcc4c: ldr      r3, [r3]
003dcc50: ldrb     r3, [r3, #0x14]
003dcc54: b        #0x3dcb8c
003dcc58: subseq   r7, fp, r8, ror #31
003dcc5c: umaaleq  r6, lr, r8, r4
003dcc60: ldrdeq   r8, sb, [lr], #-0xe4
003dcc64: subeq    r8, lr, ip, asr #29
003dcc68: subeq    r8, lr, r0, lsr #29
003dcc6c: andeq    r1, r0, r8, lsl #22
003dcc70: andeq    r1, r0, ip, ror #3
003dcc74: andeq    r0, r0, r4, asr #30
003dcc78: andeq    r1, r0, ip, lsr #23
003dcc7c: strheq   r3, [r0], -r4
