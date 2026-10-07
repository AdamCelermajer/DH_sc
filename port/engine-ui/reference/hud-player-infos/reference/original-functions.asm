
# _Z23NativeGetPlayerHUDInfosRKN7gameswf7fn_callE
0044e5cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044e5d0: ldr      r5, [pc, #0xc44]
0044e5d4: ldr      r7, [pc, #0xc44]
0044e5d8: ldr      r2, [r0, #0x10]
0044e5dc: add      r5, pc, r5
0044e5e0: ldr      r3, [r5, r7]
0044e5e4: sub      r2, r2, #2
0044e5e8: sub      sp, sp, #0x27c
0044e5ec: ldr      r3, [r3]
0044e5f0: cmp      r2, #1
0044e5f4: mov      r6, r0
0044e5f8: str      r3, [sp, #0x274]
0044e5fc: bls      #0x44e61c
0044e600: ldr      r3, [r5, r7]
0044e604: ldr      r2, [sp, #0x274]
0044e608: ldr      r3, [r3]
0044e60c: cmp      r2, r3
0044e610: bne      #0x44f218
0044e614: add      sp, sp, #0x27c
0044e618: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044e61c: ldr      r2, [r0, #0xc]
0044e620: ldr      r3, [r0, #0x14]
0044e624: mov      r1, #0xc
0044e628: ldr      r2, [r2]
0044e62c: mov      r8, #0xc
0044e630: mla      r3, r1, r3, r2
0044e634: ldrsb    r2, [r3, #1]
0044e638: cmp      r2, #5
0044e63c: ldreq    r0, [r3, #4]
0044e640: movne    r0, #0
0044e644: bl       #0x439cb4
0044e648: ldr      r3, [r6, #0xc]
0044e64c: mov      r4, r0
0044e650: ldr      r0, [r6, #0x14]
0044e654: ldr      r3, [r3]
0044e658: sub      r0, r0, #1
0044e65c: mla      r0, r8, r0, r3
0044e660: bl       #0x797a54
0044e664: bl       #0x30ea24
0044e668: ldr      r3, [r6, #0x10]
0044e66c: mov      sl, r0
0044e670: cmp      r3, #3
0044e674: movne    r1, #0
0044e678: beq      #0x44f0e8
0044e67c: mov      r0, sl
0044e680: bl       #0x43c388
0044e684: subs     r8, r0, #0
0044e688: beq      #0x44f080
0044e68c: ldrb     sb, [r8, #0x81]
0044e690: cmp      sb, #0
0044e694: bne      #0x44f080
0044e698: ldrb     r3, [r8, #0x80]
0044e69c: cmp      r3, #0
0044e6a0: beq      #0x44f080
0044e6a4: mov      r3, #0
0044e6a8: mov      r1, sb
0044e6ac: str      r3, [sp, #0xfc]
0044e6b0: str      r3, [sp, #0x104]
0044e6b4: str      r3, [sp, #0x100]
0044e6b8: bl       #0x3bbe68
0044e6bc: mov      r1, #1
0044e6c0: str      r0, [sp, #4]
0044e6c4: mov      r0, r8
0044e6c8: bl       #0x3bbe68
0044e6cc: mov      r1, #2
0044e6d0: mov      sl, r0
0044e6d4: mov      r0, r8
0044e6d8: bl       #0x3bbe68
0044e6dc: ldr      r1, [sp, #4]
0044e6e0: mov      fp, r0
0044e6e4: cmn      r1, #1
0044e6e8: streq    sb, [sp, #0x14]
0044e6ec: addeq    sb, r8, #0x3c8
0044e6f0: beq      #0x44e728
0044e6f4: add      sb, r8, #0x3c8
0044e6f8: ldr      r1, [sp, #4]
0044e6fc: mov      r0, sb
0044e700: bl       #0x3d8358
0044e704: ldr      r1, [sp, #4]
0044e708: str      r0, [sp, #0x14]
0044e70c: mov      r0, r8
0044e710: bl       #0x3bbed0
0044e714: ldr      r1, [sp, #4]
0044e718: mov      r2, r0
0044e71c: add      r3, sp, #0x104
0044e720: mov      r0, sb
0044e724: bl       #0x3d7e88
0044e728: cmn      sl, #1
0044e72c: moveq    r2, #0
0044e730: streq    r2, [sp, #0x10]
0044e734: beq      #0x44e768
0044e738: mov      r1, sl
0044e73c: mov      r0, sb
0044e740: bl       #0x3d8358
0044e744: mov      r1, sl
0044e748: str      r0, [sp, #0x10]
0044e74c: mov      r0, r8
0044e750: bl       #0x3bbed0
0044e754: mov      r1, sl
0044e758: mov      r2, r0
0044e75c: add      r3, sp, #0x100
0044e760: mov      r0, sb
0044e764: bl       #0x3d7e88
0044e768: cmn      fp, #1
0044e76c: moveq    r3, #0
0044e770: streq    r3, [sp, #0xc]
0044e774: beq      #0x44e7a8
0044e778: mov      r1, fp
0044e77c: mov      r0, sb
0044e780: bl       #0x3d8358
0044e784: mov      r1, fp
0044e788: str      r0, [sp, #0xc]
0044e78c: mov      r0, r8
0044e790: bl       #0x3bbed0
0044e794: mov      r1, fp
0044e798: mov      r2, r0
0044e79c: add      r3, sp, #0xfc
0044e7a0: mov      r0, sb
0044e7a4: bl       #0x3d7e88
0044e7a8: mov      r0, sb
0044e7ac: add      r1, sp, #0x108
0044e7b0: bl       #0x3d7da8
0044e7b4: ldr      r3, [r4]
0044e7b8: ldr      r1, [pc, #0xa64]
0044e7bc: add      fp, sp, #0x260
0044e7c0: ldr      r3, [r3, #0x1c]
0044e7c4: add      r1, pc, r1
0044e7c8: mov      r0, fp
0044e7cc: str      r3, [sp]
0044e7d0: add      sl, sp, #0xe4
0044e7d4: bl       #0x413a7c
0044e7d8: mov      r2, #1
0044e7dc: mov      r1, #0
0044e7e0: ldr      r3, [sp]
0044e7e4: strb     r1, [sp, #0xe4]
0044e7e8: strb     r2, [sp, #0xe8]
0044e7ec: mov      r1, fp
0044e7f0: strb     r2, [sp, #0xe5]
0044e7f4: mov      r0, r4
0044e7f8: mov      r2, sl
0044e7fc: blx      r3
0044e800: mov      r0, sl
0044e804: bl       #0x797124
0044e808: ldrb     ip, [sp, #0x260]
0044e80c: sxtb     r3, ip
0044e810: cmn      r3, #1
0044e814: beq      #0x44f1f8
0044e818: ldr      r3, [r4]
0044e81c: add      r2, r8, #0x560
0044e820: str      r2, [sp, #4]
0044e824: ldr      r1, [pc, #0x9fc]
0044e828: ldr      r3, [r3, #0x1c]
0044e82c: add      fp, sp, #0x24c
0044e830: add      r1, pc, r1
0044e834: mov      r0, fp
0044e838: str      r3, [sp]
0044e83c: bl       #0x413a7c
0044e840: mov      r2, #0
0044e844: mov      r1, #0x13
0044e848: ldr      r0, [sp, #4]
0044e84c: bl       #0x3df6e0
0044e850: mov      r2, #0
0044e854: strb     r2, [sp, #0xd8]
0044e858: mov      r2, #2
0044e85c: strb     r2, [sp, #0xd9]
0044e860: bl       #0x30ed30
0044e864: mov      r2, #0x9e000000
0044e868: add      ip, sp, #0x278
0044e86c: asr      r2, r2, #0x16
0044e870: strd     r0, r1, [ip, r2]
0044e874: ldr      r2, [sp, #0xf0]
0044e878: add      sl, sp, #0xd8
0044e87c: mov      r1, fp
0044e880: str      r2, [sp, #0xdc]
0044e884: ldr      r2, [sp, #0xf4]
0044e888: mov      r0, r4
0044e88c: str      r2, [sl, #8]
0044e890: ldr      r3, [sp]
0044e894: mov      r2, sl
0044e898: blx      r3
0044e89c: mov      r0, sl
0044e8a0: bl       #0x797124
0044e8a4: ldrb     r1, [sp, #0x24c]
0044e8a8: sxtb     r3, r1
0044e8ac: cmn      r3, #1
0044e8b0: beq      #0x44f1e8
0044e8b4: ldr      r3, [r4]
0044e8b8: ldr      r1, [pc, #0x96c]
0044e8bc: add      fp, sp, #0x238
0044e8c0: ldr      r3, [r3, #0x1c]
0044e8c4: add      r1, pc, r1
0044e8c8: mov      r0, fp
0044e8cc: str      r3, [sp]
0044e8d0: bl       #0x413a7c
0044e8d4: movw     r2, #0x1088
0044e8d8: ldr      r2, [r8, r2]
0044e8dc: mov      r0, #0x64
0044e8e0: add      sl, sp, #0xcc
0044e8e4: mul      r0, r0, r2
0044e8e8: movw     r2, #0x1090
0044e8ec: ldr      r1, [r8, r2]
0044e8f0: bl       #0x30e2a4
0044e8f4: mov      r2, #0
0044e8f8: strb     r2, [sp, #0xcc]
0044e8fc: mov      r2, #2
0044e900: strb     r2, [sp, #0xcd]
0044e904: bl       #0x30ed30
0044e908: mov      r2, #0x9e000000
0044e90c: add      ip, sp, #0x278
0044e910: asr      r2, r2, #0x16
0044e914: strd     r0, r1, [ip, r2]
0044e918: ldr      r2, [sp, #0xf0]
0044e91c: mov      r1, fp
0044e920: mov      r0, r4
0044e924: str      r2, [sp, #0xd0]
0044e928: ldr      r2, [sp, #0xf4]
0044e92c: str      r2, [sl, #8]
0044e930: ldr      r3, [sp]
0044e934: mov      r2, sl
0044e938: blx      r3
0044e93c: mov      r0, sl
0044e940: bl       #0x797124
0044e944: ldrb     r1, [sp, #0x238]
0044e948: sxtb     r3, r1
0044e94c: cmn      r3, #1
0044e950: beq      #0x44f1d8
0044e954: ldr      r2, [pc, #0x8d4]
0044e958: ldr      r3, [r4]
0044e95c: ldr      r1, [pc, #0x8d0]
0044e960: str      r2, [sp, #8]
0044e964: ldr      r3, [r3, #0x1c]
0044e968: add      fp, sp, #0x224
0044e96c: add      r1, pc, r1
0044e970: mov      r0, fp
0044e974: str      r3, [sp]
0044e978: bl       #0x413a7c
0044e97c: ldr      ip, [sp, #8]
0044e980: ldr      r1, [pc, #0x8b0]
0044e984: add      sl, sp, #0xc0
0044e988: ldr      r2, [r5, ip]
0044e98c: add      r1, pc, r1
0044e990: ldr      r0, [r2, #0x2c]
0044e994: ldr      r2, [pc, #0x8a0]
0044e998: add      r2, pc, r2
0044e99c: bl       #0x4c4bdc
0044e9a0: mov      r2, #0
0044e9a4: strb     r2, [sp, #0xc0]
0044e9a8: mov      r2, #2
0044e9ac: strb     r2, [sp, #0xc1]
0044e9b0: bl       #0x30ed30
0044e9b4: mov      r2, #0x9e000000
0044e9b8: add      ip, sp, #0x278
0044e9bc: asr      r2, r2, #0x16
0044e9c0: strd     r0, r1, [ip, r2]
0044e9c4: ldr      r2, [sp, #0xf0]
0044e9c8: mov      r1, fp
0044e9cc: mov      r0, r4
0044e9d0: str      r2, [sp, #0xc4]
0044e9d4: ldr      r2, [sp, #0xf4]
0044e9d8: str      r2, [sl, #8]
0044e9dc: ldr      r3, [sp]
0044e9e0: mov      r2, sl
0044e9e4: blx      r3
0044e9e8: mov      r0, sl
0044e9ec: bl       #0x797124
0044e9f0: ldrb     r1, [sp, #0x224]
0044e9f4: sxtb     r3, r1
0044e9f8: cmn      r3, #1
0044e9fc: beq      #0x44f1c8
0044ea00: ldr      r3, [r4]
0044ea04: ldr      r1, [pc, #0x834]
0044ea08: add      fp, sp, #0x210
0044ea0c: ldr      r3, [r3, #0x1c]
0044ea10: add      r1, pc, r1
0044ea14: mov      r0, fp
0044ea18: str      r3, [sp]
0044ea1c: bl       #0x413a7c
0044ea20: movw     r2, #0x109c
0044ea24: ldr      r2, [r8, r2]
0044ea28: mov      r0, #0x64
0044ea2c: add      sl, sp, #0xb4
0044ea30: mul      r0, r0, r2
0044ea34: movw     r2, #0x10a4
0044ea38: ldr      r1, [r8, r2]
0044ea3c: bl       #0x30e2a4
0044ea40: mov      r2, #0
0044ea44: strb     r2, [sp, #0xb4]
0044ea48: mov      r2, #2
0044ea4c: strb     r2, [sp, #0xb5]
0044ea50: bl       #0x30ed30
0044ea54: mov      r2, #0x9e000000
0044ea58: add      ip, sp, #0x278
0044ea5c: asr      r2, r2, #0x16
0044ea60: strd     r0, r1, [ip, r2]
0044ea64: ldr      r2, [sp, #0xf0]
0044ea68: mov      r1, fp
0044ea6c: mov      r0, r4
0044ea70: str      r2, [sp, #0xb8]
0044ea74: ldr      r2, [sp, #0xf4]
0044ea78: str      r2, [sl, #8]
0044ea7c: ldr      r3, [sp]
0044ea80: mov      r2, sl
0044ea84: blx      r3
0044ea88: mov      r0, sl
0044ea8c: bl       #0x797124
0044ea90: ldrb     r1, [sp, #0x210]
0044ea94: sxtb     r3, r1
0044ea98: cmn      r3, #1
0044ea9c: beq      #0x44f1b8
0044eaa0: ldr      r3, [r4]
0044eaa4: ldr      r1, [pc, #0x798]
0044eaa8: add      fp, sp, #0x1fc
0044eaac: ldr      r3, [r3, #0x1c]
0044eab0: add      r1, pc, r1
0044eab4: mov      r0, fp
0044eab8: str      r3, [sp]
0044eabc: bl       #0x413a7c
0044eac0: movw     r2, #0x107c
0044eac4: ldr      r2, [r8, r2]
0044eac8: mov      r0, #0x64
0044eacc: add      sl, sp, #0xa8
0044ead0: mul      r0, r0, r2
0044ead4: mov      r2, #0x1080
0044ead8: ldr      r1, [r8, r2]
0044eadc: bl       #0x30e2a4
0044eae0: mov      r2, #0
0044eae4: strb     r2, [sp, #0xa8]
0044eae8: mov      r2, #2
0044eaec: strb     r2, [sp, #0xa9]
0044eaf0: bl       #0x30ed30
0044eaf4: mov      r2, #0x9e000000
0044eaf8: add      ip, sp, #0x278
0044eafc: asr      r2, r2, #0x16
0044eb00: strd     r0, r1, [ip, r2]
0044eb04: ldr      r2, [sp, #0xf0]
0044eb08: mov      r1, fp
0044eb0c: mov      r0, r4
0044eb10: str      r2, [sp, #0xac]
0044eb14: ldr      r2, [sp, #0xf4]
0044eb18: str      r2, [sl, #8]
0044eb1c: ldr      r3, [sp]
0044eb20: mov      r2, sl
0044eb24: blx      r3
0044eb28: mov      r0, sl
0044eb2c: bl       #0x797124
0044eb30: ldrb     r1, [sp, #0x1fc]
0044eb34: sxtb     r3, r1
0044eb38: cmn      r3, #1
0044eb3c: beq      #0x44f1a8
0044eb40: ldr      r3, [r4]
0044eb44: ldr      r1, [pc, #0x6fc]
0044eb48: add      fp, sp, #0x1e8
0044eb4c: ldr      r3, [r3, #0x1c]
0044eb50: add      r1, pc, r1
0044eb54: mov      r0, fp
0044eb58: str      r3, [sp]
0044eb5c: bl       #0x413a7c
0044eb60: mov      r1, #0x42000000
0044eb64: add      r1, r1, #0xc80000
0044eb68: ldr      r0, [sp, #0x108]
0044eb6c: bl       #0x30ed6c
0044eb70: bl       #0x30e4cc
0044eb74: mov      r2, #0
0044eb78: strb     r2, [sp, #0x9c]
0044eb7c: mov      r2, #2
0044eb80: strb     r2, [sp, #0x9d]
0044eb84: bl       #0x30ed30
0044eb88: mov      r2, #0x9e000000
0044eb8c: add      ip, sp, #0x278
0044eb90: asr      r2, r2, #0x16
0044eb94: strd     r0, r1, [ip, r2]
0044eb98: ldr      r2, [sp, #0xf0]
0044eb9c: add      sl, sp, #0x9c
0044eba0: mov      r1, fp
0044eba4: str      r2, [sp, #0xa0]
0044eba8: ldr      r2, [sp, #0xf4]
0044ebac: mov      r0, r4
0044ebb0: str      r2, [sl, #8]
0044ebb4: ldr      r3, [sp]
0044ebb8: mov      r2, sl
0044ebbc: blx      r3
0044ebc0: mov      r0, sl
0044ebc4: bl       #0x797124
0044ebc8: ldrb     r1, [sp, #0x1e8]
0044ebcc: sxtb     r3, r1
0044ebd0: cmn      r3, #1
0044ebd4: beq      #0x44f198
0044ebd8: ldr      r3, [r4]
0044ebdc: ldr      r1, [pc, #0x668]
0044ebe0: add      fp, sp, #0x1d4
0044ebe4: ldr      r3, [r3, #0x1c]
0044ebe8: add      r1, pc, r1
0044ebec: mov      r0, fp
0044ebf0: str      r3, [sp]
0044ebf4: bl       #0x413a7c
0044ebf8: mov      r0, sb
0044ebfc: bl       #0x3d80b4
0044ec00: mov      r2, #0
0044ec04: add      sl, sp, #0x90
0044ec08: strb     r2, [sp, #0x90]
0044ec0c: mov      r2, #1
0044ec10: ldr      r3, [sp]
0044ec14: strb     r0, [sp, #0x94]
0044ec18: strb     r2, [sp, #0x91]
0044ec1c: mov      r1, fp
0044ec20: mov      r2, sl
0044ec24: mov      r0, r4
0044ec28: blx      r3
0044ec2c: mov      r0, sl
0044ec30: bl       #0x797124
0044ec34: ldrb     r2, [sp, #0x1d4]
0044ec38: sxtb     r3, r2
0044ec3c: cmn      r3, #1
0044ec40: beq      #0x44f188
0044ec44: ldr      r1, [pc, #0x604]
0044ec48: ldr      r3, [r4]
0044ec4c: add      fp, sp, #0x1c0
0044ec50: add      r1, pc, r1
0044ec54: mov      r0, fp
0044ec58: ldr      sb, [r3, #0x1c]
0044ec5c: bl       #0x413a7c
0044ec60: mov      r1, #0x42000000
0044ec64: add      r1, r1, #0xc80000
0044ec68: ldr      r0, [sp, #0x104]
0044ec6c: bl       #0x30ed6c
0044ec70: bl       #0x30e4cc
0044ec74: mov      r3, #0
0044ec78: strb     r3, [sp, #0x84]
0044ec7c: mov      r3, #2
0044ec80: strb     r3, [sp, #0x85]
0044ec84: bl       #0x30ed30
0044ec88: mov      r3, #0x9e000000
0044ec8c: add      ip, sp, #0x278
0044ec90: asr      r3, r3, #0x16
0044ec94: strd     r0, r1, [ip, r3]
0044ec98: ldr      r3, [sp, #0xf0]
0044ec9c: add      sl, sp, #0x84
0044eca0: mov      r1, fp
0044eca4: str      r3, [sp, #0x88]
0044eca8: ldr      r3, [sp, #0xf4]
0044ecac: mov      r2, sl
0044ecb0: mov      r0, r4
0044ecb4: str      r3, [sl, #8]
0044ecb8: blx      sb
0044ecbc: mov      r0, sl
0044ecc0: bl       #0x797124
0044ecc4: ldrb     r1, [sp, #0x1c0]
0044ecc8: sxtb     r3, r1
0044eccc: cmn      r3, #1
0044ecd0: beq      #0x44f178
0044ecd4: ldr      r1, [pc, #0x578]
0044ecd8: ldr      r3, [r4]
0044ecdc: add      fp, sp, #0x1ac
0044ece0: add      r1, pc, r1
0044ece4: mov      r0, fp
0044ece8: ldr      sb, [r3, #0x1c]
0044ecec: bl       #0x413a7c
0044ecf0: ldr      r2, [sp, #0x14]
0044ecf4: mov      r3, #0
0044ecf8: add      sl, sp, #0x78
0044ecfc: strb     r3, [sp, #0x78]
0044ed00: mov      r3, #1
0044ed04: strb     r3, [sp, #0x79]
0044ed08: strb     r2, [sp, #0x7c]
0044ed0c: mov      r1, fp
0044ed10: mov      r2, sl
0044ed14: mov      r0, r4
0044ed18: blx      sb
0044ed1c: mov      r0, sl
0044ed20: bl       #0x797124
0044ed24: ldrb     ip, [sp, #0x1ac]
0044ed28: sxtb     r3, ip
0044ed2c: cmn      r3, #1
0044ed30: beq      #0x44f168
0044ed34: ldr      r1, [pc, #0x51c]
0044ed38: ldr      r3, [r4]
0044ed3c: add      fp, sp, #0x198
0044ed40: add      r1, pc, r1
0044ed44: mov      r0, fp
0044ed48: ldr      sb, [r3, #0x1c]
0044ed4c: bl       #0x413a7c
0044ed50: mov      r1, #0x42000000
0044ed54: add      r1, r1, #0xc80000
0044ed58: ldr      r0, [sp, #0x100]
0044ed5c: bl       #0x30ed6c
0044ed60: bl       #0x30e4cc
0044ed64: mov      r3, #0
0044ed68: strb     r3, [sp, #0x6c]
0044ed6c: mov      r3, #2
0044ed70: strb     r3, [sp, #0x6d]
0044ed74: bl       #0x30ed30
0044ed78: mov      r3, #0x9e000000
0044ed7c: asr      r3, r3, #0x16
0044ed80: add      r2, sp, #0x278
0044ed84: strd     r0, r1, [r2, r3]
0044ed88: ldr      r3, [sp, #0xf0]
0044ed8c: add      sl, sp, #0x6c
0044ed90: mov      r1, fp
0044ed94: str      r3, [sp, #0x70]
0044ed98: ldr      r3, [sp, #0xf4]
0044ed9c: mov      r2, sl
0044eda0: mov      r0, r4
0044eda4: str      r3, [sl, #8]
0044eda8: blx      sb
0044edac: mov      r0, sl
0044edb0: bl       #0x797124
0044edb4: ldrb     ip, [sp, #0x198]
0044edb8: sxtb     r3, ip
0044edbc: cmn      r3, #1
0044edc0: beq      #0x44f158
0044edc4: ldr      r1, [pc, #0x490]
0044edc8: ldr      r3, [r4]
0044edcc: add      fp, sp, #0x184
0044edd0: add      r1, pc, r1
0044edd4: mov      r0, fp
0044edd8: ldr      sb, [r3, #0x1c]
0044eddc: bl       #0x413a7c
0044ede0: ldr      r1, [sp, #0x10]
0044ede4: mov      r3, #0
0044ede8: add      sl, sp, #0x60
0044edec: strb     r3, [sp, #0x60]
0044edf0: mov      r3, #1
0044edf4: strb     r3, [sp, #0x61]
0044edf8: mov      r2, sl
0044edfc: strb     r1, [sp, #0x64]
0044ee00: mov      r0, r4
0044ee04: mov      r1, fp
0044ee08: blx      sb
0044ee0c: mov      r0, sl
0044ee10: bl       #0x797124
0044ee14: ldrb     r2, [sp, #0x184]
0044ee18: sxtb     r3, r2
0044ee1c: cmn      r3, #1
0044ee20: beq      #0x44f148
0044ee24: ldr      r1, [pc, #0x434]
0044ee28: ldr      r3, [r4]
0044ee2c: add      fp, sp, #0x170
0044ee30: add      r1, pc, r1
0044ee34: mov      r0, fp
0044ee38: ldr      sb, [r3, #0x1c]
0044ee3c: bl       #0x413a7c
0044ee40: mov      r1, #0x42000000
0044ee44: add      r1, r1, #0xc80000
0044ee48: ldr      r0, [sp, #0xfc]
0044ee4c: bl       #0x30ed6c
0044ee50: bl       #0x30e4cc
0044ee54: mov      r3, #0
0044ee58: strb     r3, [sp, #0x54]
0044ee5c: mov      r3, #2
0044ee60: strb     r3, [sp, #0x55]
0044ee64: bl       #0x30ed30
0044ee68: mov      r3, #0x9e000000
0044ee6c: add      ip, sp, #0x278
0044ee70: asr      r3, r3, #0x16
0044ee74: strd     r0, r1, [ip, r3]
0044ee78: ldr      r3, [sp, #0xf0]
0044ee7c: add      sl, sp, #0x54
0044ee80: mov      r1, fp
0044ee84: str      r3, [sp, #0x58]
0044ee88: ldr      r3, [sp, #0xf4]
0044ee8c: mov      r2, sl
0044ee90: mov      r0, r4
0044ee94: str      r3, [sl, #8]
0044ee98: blx      sb
0044ee9c: mov      r0, sl
0044eea0: bl       #0x797124
0044eea4: ldrb     r3, [sp, #0x170]
0044eea8: cmp      r3, #0xff
0044eeac: beq      #0x44f138
0044eeb0: ldr      r1, [pc, #0x3ac]
0044eeb4: ldr      r3, [r4]
0044eeb8: add      fp, sp, #0x15c
0044eebc: add      r1, pc, r1
0044eec0: mov      r0, fp
0044eec4: ldr      sb, [r3, #0x1c]
0044eec8: bl       #0x413a7c
0044eecc: ldr      r1, [sp, #0xc]
0044eed0: mov      r3, #0
0044eed4: add      sl, sp, #0x48
0044eed8: strb     r3, [sp, #0x48]
0044eedc: mov      r3, #1
0044eee0: strb     r3, [sp, #0x49]
0044eee4: strb     r1, [sp, #0x4c]
0044eee8: mov      r2, sl
0044eeec: mov      r1, fp
0044eef0: mov      r0, r4
0044eef4: blx      sb
0044eef8: mov      r0, sl
0044eefc: bl       #0x797124
0044ef00: ldrb     r3, [sp, #0x15c]
0044ef04: cmp      r3, #0xff
0044ef08: beq      #0x44f128
0044ef0c: ldr      r1, [pc, #0x354]
0044ef10: ldr      r3, [r4]
0044ef14: add      sb, sp, #0x148
0044ef18: add      r1, pc, r1
0044ef1c: mov      r0, sb
0044ef20: ldr      sl, [r3, #0x1c]
0044ef24: bl       #0x413a7c
0044ef28: add      r0, r8, #0x37c
0044ef2c: bl       #0x3fc690
0044ef30: mov      r3, #0
0044ef34: strb     r3, [sp, #0x3c]
0044ef38: mov      r3, #2
0044ef3c: strb     r3, [sp, #0x3d]
0044ef40: bl       #0x30ed30
0044ef44: mov      r3, #0x9e000000
0044ef48: asr      r3, r3, #0x16
0044ef4c: add      r2, sp, #0x278
0044ef50: strd     r0, r1, [r2, r3]
0044ef54: ldr      r3, [sp, #0xf0]
0044ef58: add      r8, sp, #0x3c
0044ef5c: mov      r1, sb
0044ef60: str      r3, [sp, #0x40]
0044ef64: ldr      r3, [sp, #0xf4]
0044ef68: mov      r2, r8
0044ef6c: mov      r0, r4
0044ef70: str      r3, [r8, #8]
0044ef74: blx      sl
0044ef78: mov      r0, r8
0044ef7c: bl       #0x797124
0044ef80: ldrb     r3, [sp, #0x148]
0044ef84: cmp      r3, #0xff
0044ef88: beq      #0x44f118
0044ef8c: ldr      r1, [pc, #0x2d8]
0044ef90: ldr      r3, [r4]
0044ef94: add      sl, sp, #0x134
0044ef98: add      r1, pc, r1
0044ef9c: mov      r0, sl
0044efa0: ldr      sb, [r3, #0x1c]
0044efa4: bl       #0x413a7c
0044efa8: mov      r1, #0x94
0044efac: mov      r2, #0
0044efb0: ldr      r0, [sp, #4]
0044efb4: bl       #0x3df6e0
0044efb8: mov      r3, #0
0044efbc: subs     r0, r0, #0
0044efc0: movne    r0, #1
0044efc4: add      r8, sp, #0x30
0044efc8: strb     r3, [sp, #0x30]
0044efcc: mov      r3, #1
0044efd0: strb     r3, [sp, #0x31]
0044efd4: strb     r0, [sp, #0x34]
0044efd8: mov      r1, sl
0044efdc: mov      r2, r8
0044efe0: mov      r0, r4
0044efe4: blx      sb
0044efe8: mov      r0, r8
0044efec: bl       #0x797124
0044eff0: ldrb     r3, [sp, #0x134]
0044eff4: cmp      r3, #0xff
0044eff8: beq      #0x44f108
0044effc: ldr      r1, [pc, #0x26c]
0044f000: ldr      r3, [r4]
0044f004: add      sb, sp, #0x120
0044f008: add      r1, pc, r1
0044f00c: mov      r0, sb
0044f010: ldr      r8, [r3, #0x1c]
0044f014: bl       #0x413a7c
0044f018: ldr      r3, [sp, #8]
0044f01c: ldr      r1, [pc, #0x250]
0044f020: add      sl, sp, #0x24
0044f024: ldr      r0, [r5, r3]
0044f028: add      r1, pc, r1
0044f02c: bl       #0x320e44
0044f030: mov      r3, #0
0044f034: rsbs     r0, r0, #1
0044f038: movlo    r0, #0
0044f03c: strb     r3, [sp, #0x24]
0044f040: mov      r3, #1
0044f044: strb     r3, [sp, #0x25]
0044f048: strb     r0, [sp, #0x28]
0044f04c: mov      r1, sb
0044f050: mov      r2, sl
0044f054: mov      r0, r4
0044f058: blx      r8
0044f05c: mov      r0, sl
0044f060: bl       #0x797124
0044f064: ldrb     r3, [sp, #0x120]
0044f068: cmp      r3, #0xff
0044f06c: bne      #0x44f0d8
0044f070: ldr      r0, [sp, #0x12c]
0044f074: ldr      r1, [sp, #0x128]
0044f078: bl       #0x752b38
0044f07c: b        #0x44f0d8
0044f080: ldr      r1, [pc, #0x1f0]
0044f084: ldr      r3, [r4]
0044f088: add      sb, sp, #0x10c
0044f08c: add      r1, pc, r1
0044f090: mov      r0, sb
0044f094: ldr      sl, [r3, #0x1c]
0044f098: add      r8, sp, #0x18
0044f09c: bl       #0x413a7c
0044f0a0: mov      r3, #0
0044f0a4: mov      r2, #1
0044f0a8: strb     r3, [sp, #0x1c]
0044f0ac: strb     r3, [sp, #0x18]
0044f0b0: strb     r2, [sp, #0x19]
0044f0b4: mov      r1, sb
0044f0b8: mov      r2, r8
0044f0bc: mov      r0, r4
0044f0c0: blx      sl
0044f0c4: mov      r0, r8
0044f0c8: bl       #0x797124
0044f0cc: ldrb     r3, [sp, #0x10c]
0044f0d0: cmp      r3, #0xff
0044f0d4: beq      #0x44f208
0044f0d8: ldr      r0, [r6]
0044f0dc: mov      r1, r4
0044f0e0: bl       #0x797250
0044f0e4: b        #0x44e600
0044f0e8: ldr      r3, [r6, #0xc]
0044f0ec: ldr      r0, [r6, #0x14]
0044f0f0: ldr      r3, [r3]
0044f0f4: sub      r0, r0, #2
0044f0f8: mla      r0, r8, r0, r3
0044f0fc: bl       #0x797960
0044f100: mov      r1, r0
0044f104: b        #0x44e67c
0044f108: ldr      r0, [sp, #0x140]
0044f10c: ldr      r1, [sp, #0x13c]
0044f110: bl       #0x752b38
0044f114: b        #0x44effc
0044f118: ldr      r0, [sp, #0x154]
0044f11c: ldr      r1, [sp, #0x150]
0044f120: bl       #0x752b38
0044f124: b        #0x44ef8c
0044f128: ldr      r0, [sp, #0x168]
0044f12c: ldr      r1, [sp, #0x164]
0044f130: bl       #0x752b38
0044f134: b        #0x44ef0c
0044f138: ldr      r0, [sp, #0x17c]
0044f13c: ldr      r1, [sp, #0x178]
0044f140: bl       #0x752b38
0044f144: b        #0x44eeb0
0044f148: ldr      r0, [sp, #0x190]
0044f14c: ldr      r1, [sp, #0x18c]
0044f150: bl       #0x752b38
0044f154: b        #0x44ee24
0044f158: ldr      r0, [sp, #0x1a4]
0044f15c: ldr      r1, [sp, #0x1a0]
0044f160: bl       #0x752b38
0044f164: b        #0x44edc4
0044f168: ldr      r0, [sp, #0x1b8]
0044f16c: ldr      r1, [sp, #0x1b4]
0044f170: bl       #0x752b38
0044f174: b        #0x44ed34
0044f178: ldr      r0, [sp, #0x1cc]
0044f17c: ldr      r1, [sp, #0x1c8]
0044f180: bl       #0x752b38
0044f184: b        #0x44ecd4
0044f188: ldr      r0, [sp, #0x1e0]
0044f18c: ldr      r1, [sp, #0x1dc]
0044f190: bl       #0x752b38
0044f194: b        #0x44ec44
0044f198: ldr      r0, [sp, #0x1f4]
0044f19c: ldr      r1, [sp, #0x1f0]
0044f1a0: bl       #0x752b38
0044f1a4: b        #0x44ebd8
0044f1a8: ldr      r0, [sp, #0x208]
0044f1ac: ldr      r1, [sp, #0x204]
0044f1b0: bl       #0x752b38
0044f1b4: b        #0x44eb40
0044f1b8: ldr      r0, [sp, #0x21c]
0044f1bc: ldr      r1, [sp, #0x218]
0044f1c0: bl       #0x752b38
0044f1c4: b        #0x44eaa0
0044f1c8: ldr      r0, [sp, #0x230]
0044f1cc: ldr      r1, [sp, #0x22c]
0044f1d0: bl       #0x752b38
0044f1d4: b        #0x44ea00
0044f1d8: ldr      r0, [sp, #0x244]
0044f1dc: ldr      r1, [sp, #0x240]
0044f1e0: bl       #0x752b38
0044f1e4: b        #0x44e954
0044f1e8: ldr      r0, [sp, #0x258]
0044f1ec: ldr      r1, [sp, #0x254]
0044f1f0: bl       #0x752b38
0044f1f4: b        #0x44e8b4
0044f1f8: ldr      r0, [sp, #0x26c]
0044f1fc: ldr      r1, [sp, #0x268]
0044f200: bl       #0x752b38
0044f204: b        #0x44e818
0044f208: ldr      r0, [sp, #0x118]
0044f20c: ldr      r1, [sp, #0x114]
0044f210: bl       #0x752b38
0044f214: b        #0x44f0d8
0044f218: bl       #0x30e310
0044f21c: ldrheq   r6, [r4], #-0x44
0044f220: andeq    r4, r0, ip, lsr #1
0044f224: subeq    lr, r7, ip, lsl #6
0044f228: subeq    sp, r7, r8, lsr r8
0044f22c: subeq    lr, r7, ip, lsl r2
0044f230: strdeq   r3, r4, [r0], -r4
0044f234: subeq    lr, r7, ip, ror r1
0044f238: subeq    r2, r7, r4, asr #27
0044f23c: subeq    lr, r7, r0, ror #2
0044f240: subeq    lr, r7, r0, lsl #2
0044f244: subeq    lr, r7, r8, rrx
0044f248: ldrdeq   sp, lr, [r7], #-0xf0
0044f24c: subeq    sp, r7, r8, asr #30
0044f250: strdeq   sp, lr, [r7], #-0xe0
0044f254: subeq    sp, r7, r0, ror lr
0044f258: subeq    sp, r7, r0, lsr #28
0044f25c: subeq    sp, r7, r0, lsr #27
0044f260: subeq    sp, r7, r0, asr sp
0044f264: ldrdeq   sp, lr, [r7], #-0xc4
0044f268: subeq    sp, r7, r8, lsl #25
0044f26c: subeq    sp, r7, r8, lsl ip
0044f270: strheq   sp, [r7], #-0xb8
0044f274: subeq    pc, r6, r8, lsl ip
0044f278: subeq    sp, r7, r4, asr #20

# _ZNK9Character16SG_GetSkillLevelEj
003bbed0: movw     r3, #0x14e8
003bbed4: ldr      r0, [r0, r3]
003bbed8: cmp      r0, #0
003bbedc: beq      #0x3bbee4
003bbee0: b        #0x4668dc
003bbee4: mvn      r0, #0
003bbee8: bx       lr

# _ZNK6CharAI16AI_IsSkillUsableEj
003d8358: push     {r4, r5, r6, r7, lr}
003d835c: mov      r4, r0
003d8360: ldr      r0, [r0, #4]
003d8364: sub      sp, sp, #0xc
003d8368: mov      r5, r1
003d836c: add      r0, r0, #0x4f0
003d8370: add      r0, r0, #0xc
003d8374: bl       #0x3c02e8
003d8378: ldr      r6, [pc, #0xdc]
003d837c: cmp      r0, #0
003d8380: ldreq    r0, [r4, #4]
003d8384: add      r6, pc, r6
003d8388: beq      #0x3d83a8
003d838c: ldr      r0, [r4, #4]
003d8390: ldr      r3, [r0, #0x520]
003d8394: tst      r3, #0x8000
003d8398: bne      #0x3d83a8
003d839c: mov      r0, #0
003d83a0: add      sp, sp, #0xc
003d83a4: pop      {r4, r5, r6, r7, pc}
003d83a8: add      r0, r0, #0x4f0
003d83ac: add      r0, r0, #0xc
003d83b0: bl       #0x3c0334
003d83b4: subs     r7, r0, #0
003d83b8: bne      #0x3d839c
003d83bc: mov      r0, r4
003d83c0: bl       #0x3cb458
003d83c4: cmp      r0, #0
003d83c8: beq      #0x3d839c
003d83cc: ldr      r3, [r4, #0xb4]
003d83d0: ldr      r2, [r4, #0xb8]
003d83d4: rsb      r2, r3, r2
003d83d8: cmp      r5, r2, asr #2
003d83dc: blo      #0x3d8444
003d83e0: ldr      r3, [pc, #0x78]
003d83e4: ldr      r3, [r6, r3]
003d83e8: ldr      r3, [r3]
003d83ec: cmp      r3, #2
003d83f0: streq    r7, [r7]
003d83f4: beq      #0x3d839c
003d83f8: cmp      r3, #1
003d83fc: bne      #0x3d839c
003d8400: ldr      r0, [pc, #0x5c]
003d8404: ldr      r1, [pc, #0x5c]
003d8408: ldr      r2, [pc, #0x5c]
003d840c: ldr      r0, [r6, r0]
003d8410: ldr      r3, [pc, #0x58]
003d8414: add      r2, pc, r2
003d8418: mov      ip, #0xb5
003d841c: add      r3, pc, r3
003d8420: add      r1, pc, r1
003d8424: add      r0, r0, #0xa8
003d8428: str      ip, [sp]
003d842c: bl       #0x30e004
003d8430: ldr      r2, [r4, #0xb8]
003d8434: ldr      r3, [r4, #0xb4]
003d8438: rsb      r2, r3, r2
003d843c: cmp      r5, r2, asr #2
003d8440: bhs      #0x3d839c
003d8444: ldr      r0, [r3, r5, lsl #2]
003d8448: cmp      r0, #0
003d844c: beq      #0x3d839c
003d8450: add      sp, sp, #0xc
003d8454: pop      {r4, r5, r6, r7, lr}
003d8458: b        #0x3da9dc
003d845c: subseq   ip, fp, ip, lsl #14
003d8460: andeq    r3, r0, r0, asr #19
003d8464: andeq    r1, r0, r0, asr #19
003d8468: strheq   r5, [lr], #-0xf8
003d846c: subeq    sp, lr, ip, ror #6
003d8470: subeq    sp, lr, ip, lsl #6

# _ZNK6CharAI16AI_IsSpellUsableEv
003d80b4: push     {r4, r5, r6, r7, lr}
003d80b8: mov      r4, r0
003d80bc: ldr      r0, [r0, #4]
003d80c0: sub      sp, sp, #0xc
003d80c4: ldr      r6, [pc, #0xdc]
003d80c8: add      r0, r0, #0x4f0
003d80cc: add      r0, r0, #0xc
003d80d0: bl       #0x3c02e8
003d80d4: cmp      r0, #0
003d80d8: add      r6, pc, r6
003d80dc: beq      #0x3d80ec
003d80e0: mov      r0, #0
003d80e4: add      sp, sp, #0xc
003d80e8: pop      {r4, r5, r6, r7, pc}
003d80ec: ldr      r0, [r4, #4]
003d80f0: add      r0, r0, #0x4f0
003d80f4: add      r0, r0, #0xc
003d80f8: bl       #0x3c0334
003d80fc: subs     r7, r0, #0
003d8100: bne      #0x3d80e0
003d8104: mov      r0, r4
003d8108: bl       #0x3cb458
003d810c: cmp      r0, #0
003d8110: beq      #0x3d80e0
003d8114: ldr      r0, [r4, #4]
003d8118: mvn      r1, #0
003d811c: bl       #0x3bb98c
003d8120: ldr      r2, [r4, #0xc0]
003d8124: ldr      r3, [r4, #0xc4]
003d8128: mov      r5, r0
003d812c: rsb      r3, r2, r3
003d8130: cmp      r0, r3, asr #2
003d8134: blt      #0x3d8158
003d8138: ldr      r3, [pc, #0x6c]
003d813c: ldr      r3, [r6, r3]
003d8140: ldr      r3, [r3]
003d8144: cmp      r3, #2
003d8148: streq    r7, [r7]
003d814c: beq      #0x3d8158
003d8150: cmp      r3, #1
003d8154: beq      #0x3d8170
003d8158: ldr      r0, [r2, r5, lsl #2]
003d815c: cmp      r0, #0
003d8160: beq      #0x3d80e0
003d8164: add      sp, sp, #0xc
003d8168: pop      {r4, r5, r6, r7, lr}
003d816c: b        #0x3da9dc
003d8170: ldr      r0, [pc, #0x38]
003d8174: ldr      r1, [pc, #0x38]
003d8178: ldr      r2, [pc, #0x38]
003d817c: ldr      r0, [r6, r0]
003d8180: ldr      r3, [pc, #0x34]
003d8184: add      r2, pc, r2
003d8188: movw     ip, #0x141
003d818c: add      r1, pc, r1
003d8190: add      r0, r0, #0xa8
003d8194: add      r3, pc, r3
003d8198: str      ip, [sp]
003d819c: bl       #0x30e004
003d81a0: ldr      r2, [r4, #0xc0]
003d81a4: b        #0x3d8158
003d81a8: ldrheq   ip, [fp], #-0x98
003d81ac: andeq    r3, r0, r0, asr #19
003d81b0: andeq    r1, r0, r0, asr #19
003d81b4: subeq    r6, lr, ip, asr #4
003d81b8: subeq    sp, lr, ip, lsl r6
003d81bc: umaaleq  sp, lr, r4, r5

# _ZNK6CharAI12AI_SkillInfoEjjRf
003d7e88: push     {r4, r5, r6, r7, lr}
003d7e8c: mov      r4, r0
003d7e90: ldr      r6, [r4, #0xb8]
003d7e94: ldr      r0, [r0, #0xb4]
003d7e98: ldr      ip, [pc, #0xa8]
003d7e9c: sub      sp, sp, #0xc
003d7ea0: rsb      r6, r0, r6
003d7ea4: cmp      r1, r6, asr #2
003d7ea8: add      ip, pc, ip
003d7eac: mov      r5, r1
003d7eb0: mov      r7, r2
003d7eb4: mov      r6, r3
003d7eb8: blo      #0x3d7ee0
003d7ebc: ldr      r3, [pc, #0x88]
003d7ec0: ldr      r3, [ip, r3]
003d7ec4: ldr      r3, [r3]
003d7ec8: cmp      r3, #2
003d7ecc: moveq    r3, #0
003d7ed0: streq    r3, [r3]
003d7ed4: beq      #0x3d7ee0
003d7ed8: cmp      r3, #1
003d7edc: beq      #0x3d7f10
003d7ee0: ldr      r0, [r0, r5, lsl #2]
003d7ee4: cmp      r0, #0
003d7ee8: beq      #0x3d7f00
003d7eec: mov      r1, r7
003d7ef0: mov      r2, r6
003d7ef4: add      sp, sp, #0xc
003d7ef8: pop      {r4, r5, r6, r7, lr}
003d7efc: b        #0x3daca8
003d7f00: mov      r3, #0
003d7f04: str      r3, [r6]
003d7f08: add      sp, sp, #0xc
003d7f0c: pop      {r4, r5, r6, r7, pc}
003d7f10: ldr      r0, [pc, #0x38]
003d7f14: ldr      r1, [pc, #0x38]
003d7f18: ldr      r2, [pc, #0x38]
003d7f1c: ldr      r0, [ip, r0]
003d7f20: ldr      r3, [pc, #0x34]
003d7f24: movw     ip, #0x1b7
003d7f28: add      r1, pc, r1
003d7f2c: add      r0, r0, #0xa8
003d7f30: add      r2, pc, r2
003d7f34: add      r3, pc, r3
003d7f38: str      ip, [sp]
003d7f3c: bl       #0x30e004
003d7f40: ldr      r0, [r4, #0xb4]
003d7f44: b        #0x3d7ee0
003d7f48: subseq   ip, fp, r8, ror #23
003d7f4c: andeq    r3, r0, r0, asr #19
003d7f50: andeq    r1, r0, r0, asr #19
003d7f54: strheq   r6, [lr], #-0x40
003d7f58: subeq    sp, lr, r0, asr r8
003d7f5c: strdeq   sp, lr, [lr], #-0x74

# _ZN11Application14GetSavedOptionEPKc
00320e44: push     {r4, r5, r6, lr}
00320e48: ldr      r4, [r0, #0x4c]
00320e4c: mov      r5, r1
00320e50: mov      r0, r4
00320e54: bl       #0x46d4a8
00320e58: cmp      r0, #0
00320e5c: bne      #0x320e64
00320e60: pop      {r4, r5, r6, pc}
00320e64: mov      r0, r4
00320e68: mov      r1, r5
00320e6c: pop      {r4, r5, r6, lr}
00320e70: b        #0x46d474

# _ZNK14CharProperties12PROPS_GetIntEib
003df6e0: ldr      r3, [pc, #0x28]
003df6e4: cmp      r2, #0
003df6e8: mov      r2, r1
003df6ec: addeq    r1, r0, #0xa90
003df6f0: add      r3, pc, r3
003df6f4: push     {r4, lr}
003df6f8: addeq    r1, r1, #4
003df6fc: ldrne    r1, [pc, #0x10]
003df700: ldrne    r1, [r3, r1]
003df704: bl       #0x3dedb4
003df708: asr      r0, r0, #8
003df70c: pop      {r4, pc}
003df710: subseq   r5, fp, r0, lsr #7
003df714: andeq    r1, r0, ip, asr #32

# _ZN17CharAISkillScript11GetCooldownEv
003da3d0: push     {r4, lr}
003da3d4: ldr      r1, [r0, #0x18]
003da3d8: sub      sp, sp, #8
003da3dc: cmn      r1, #1
003da3e0: beq      #0x3da430
003da3e4: ldr      r0, [r0, #4]
003da3e8: add      r2, sp, #4
003da3ec: mov      r3, sp
003da3f0: add      r0, r0, #0x3b4
003da3f4: bl       #0x3db344
003da3f8: cmp      r0, #0
003da3fc: beq      #0x3da430
003da400: ldr      r0, [sp, #4]
003da404: bl       #0x30e2e0
003da408: mov      r4, r0
003da40c: ldr      r0, [sp]
003da410: bl       #0x30e2e0
003da414: mov      r1, r0
003da418: mov      r0, r4
003da41c: bl       #0x30ec94
003da420: mov      r1, r0
003da424: mov      r0, #0x3f800000
003da428: bl       #0x30e3ac
003da42c: b        #0x3da434
003da430: mov      r0, #0
003da434: add      sp, sp, #8
003da438: pop      {r4, pc}

# _Z19NativeGetPlayerCharib
0043c388: ldr      r3, [pc, #0x6c]
0043c38c: ldr      r2, [pc, #0x6c]
0043c390: push     {r4, r5, r6, lr}
0043c394: add      r3, pc, r3
0043c398: ldr      r2, [r3, r2]
0043c39c: subs     r4, r0, #0
0043c3a0: mov      r5, r1
0043c3a4: ldr      r6, [r2, #0x40]
0043c3a8: blt      #0x43c3bc
0043c3ac: mov      r0, r6
0043c3b0: bl       #0x36d7a8
0043c3b4: cmp      r4, r0
0043c3b8: blt      #0x43c3c4
0043c3bc: mov      r0, #0
0043c3c0: pop      {r4, r5, r6, pc}
0043c3c4: cmp      r5, #0
0043c3c8: bne      #0x43c3e4
0043c3cc: mov      r0, r6
0043c3d0: mov      r1, r4
0043c3d4: mov      r2, r5
0043c3d8: bl       #0x36e478
0043c3dc: ldr      r0, [r0, #0x660]
0043c3e0: pop      {r4, r5, r6, pc}
0043c3e4: mov      r0, r6
0043c3e8: mov      r1, r4
0043c3ec: mov      r2, #0
0043c3f0: bl       #0x36e2ac
0043c3f4: ldr      r0, [r0, #0x660]
0043c3f8: pop      {r4, r5, r6, pc}
0043c3fc: ldrsheq  r8, [r5], #-0x6c
0043c400: strdeq   r3, r4, [r0], -r4

# _ZNK6CharAI12AI_SpellInfoERf
003d7da8: push     {r4, r5, r6, lr}
003d7dac: mov      r4, r0
003d7db0: sub      sp, sp, #8
003d7db4: mov      r6, r1
003d7db8: ldr      r0, [r0, #4]
003d7dbc: mvn      r1, #0
003d7dc0: bl       #0x3bb98c
003d7dc4: ldr      r2, [r4, #0xc0]
003d7dc8: ldr      r1, [r4, #0xc4]
003d7dcc: ldr      r3, [pc, #0x9c]
003d7dd0: mov      r5, r0
003d7dd4: rsb      r1, r2, r1
003d7dd8: cmp      r0, r1, asr #2
003d7ddc: add      r3, pc, r3
003d7de0: blo      #0x3d7e08
003d7de4: ldr      r1, [pc, #0x88]
003d7de8: ldr      r1, [r3, r1]
003d7dec: ldr      r1, [r1]
003d7df0: cmp      r1, #2
003d7df4: moveq    r3, #0
003d7df8: streq    r3, [r3]
003d7dfc: beq      #0x3d7e08
003d7e00: cmp      r1, #1
003d7e04: beq      #0x3d7e38
003d7e08: ldr      r0, [r2, r5, lsl #2]
003d7e0c: cmp      r0, #0
003d7e10: beq      #0x3d7e28
003d7e14: mov      r2, r6
003d7e18: mov      r1, #0
003d7e1c: add      sp, sp, #8
003d7e20: pop      {r4, r5, r6, lr}
003d7e24: b        #0x3daca8
003d7e28: mov      r3, #0
003d7e2c: str      r3, [r6]
003d7e30: add      sp, sp, #8
003d7e34: pop      {r4, r5, r6, pc}
003d7e38: ldr      r0, [pc, #0x38]
003d7e3c: ldr      r1, [pc, #0x38]
003d7e40: ldr      r2, [pc, #0x38]
003d7e44: ldr      r0, [r3, r0]
003d7e48: ldr      r3, [pc, #0x34]
003d7e4c: add      r2, pc, r2
003d7e50: movw     ip, #0x1c9
003d7e54: add      r1, pc, r1
003d7e58: add      r0, r0, #0xa8
003d7e5c: add      r3, pc, r3
003d7e60: str      ip, [sp]
003d7e64: bl       #0x30e004
003d7e68: ldr      r2, [r4, #0xc0]
003d7e6c: b        #0x3d7e08
003d7e70: ldrheq   ip, [fp], #-0xc4
003d7e74: andeq    r3, r0, r0, asr #19
003d7e78: andeq    r1, r0, r0, asr #19
003d7e7c: subeq    r6, lr, r4, lsl #11
003d7e80: strheq   sp, [lr], #-0x8c
003d7e84: subeq    sp, lr, ip, asr #17

# _ZN7gameswf7cast_toINS_9as_objectEEEPT_PNS_19as_object_interfaceE
00439cb4: push     {r4, lr}
00439cb8: subs     r4, r0, #0
00439cbc: beq      #0x439ce0
00439cc0: ldr      r3, [r4]
00439cc4: mov      r1, #0
00439cc8: mov      lr, pc
00439ccc: ldr      pc, [r3, #8]
00439cd0: cmp      r0, #0
00439cd4: beq      #0x439ce0
00439cd8: mov      r0, r4
00439cdc: pop      {r4, pc}
00439ce0: mov      r0, #0
00439ce4: pop      {r4, pc}

# _ZNK13ItemInventory13GetNumPotionsEv
003fc690: ldr      r0, [r0, #0x24]
003fc694: cmp      r0, #0
003fc698: ldrshne  r0, [r0, #0x50]
003fc69c: bx       lr

# _ZN9Character17SG_GetSkillInSlotEi
003bbe68: movw     r3, #0x14e8
003bbe6c: ldr      r0, [r0, r3]
003bbe70: cmp      r0, #0
003bbe74: beq      #0x3bbe7c
003bbe78: b        #0x467488
003bbe7c: mvn      r0, #0
003bbe80: bx       lr
