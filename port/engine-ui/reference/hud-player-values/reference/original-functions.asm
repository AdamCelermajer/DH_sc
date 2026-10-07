
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

# _ZN14InfoHUDManager10SlowUpdateEv
0041de54: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0041de58: ldr      r5, [pc, #0x1f4]
0041de5c: ldr      r1, [pc, #0x1f4]
0041de60: ldr      r4, [pc, #0x1f4]
0041de64: add      r5, pc, r5
0041de68: ldr      r3, [r5, r1]
0041de6c: ldr      r2, [r5, r4]
0041de70: sub      sp, sp, #0x3c
0041de74: ldr      r3, [r3]
0041de78: str      r1, [sp, #0xc]
0041de7c: mov      r1, #0
0041de80: mov      r6, r0
0041de84: ldr      r0, [r2, #0x40]
0041de88: mov      r2, r1
0041de8c: str      r3, [sp, #0x34]
0041de90: bl       #0x36e478
0041de94: ldr      r8, [r0, #0x660]
0041de98: cmp      r8, #0
0041de9c: beq      #0x41dfdc
0041dea0: mov      r7, #0
0041dea4: add      fp, r8, #0x3c8
0041dea8: add      sl, sp, #0x1c
0041deac: mov      sb, r7
0041deb0: mov      r0, r8
0041deb4: mov      r1, r7
0041deb8: bl       #0x3bbe68
0041debc: cmn      r0, #1
0041dec0: strb     sb, [sl, r7]
0041dec4: beq      #0x41ded8
0041dec8: mov      r1, r0
0041decc: mov      r0, fp
0041ded0: bl       #0x3d8358
0041ded4: strb     r0, [sl, r7]
0041ded8: add      r7, r7, #1
0041dedc: cmp      r7, #3
0041dee0: bne      #0x41deb0
0041dee4: add      r0, r6, #0x15c
0041dee8: bl       #0x427d50
0041deec: mov      r7, r0
0041def0: mov      r0, fp
0041def4: bl       #0x3d80b4
0041def8: ldr      r1, [pc, #0x160]
0041defc: eor      r0, r0, #1
0041df00: strb     r0, [r7, #0x9b]
0041df04: add      r1, pc, r1
0041df08: ldr      r0, [r5, r4]
0041df0c: bl       #0x320e44
0041df10: cmp      r0, #1
0041df14: ble      #0x41e00c
0041df18: mov      r4, #0
0041df1c: add      r8, sp, #0x20
0041df20: add      r7, sp, #0x10
0041df24: mov      sb, r4
0041df28: mov      fp, #0x30
0041df2c: mla      r0, fp, r4, r6
0041df30: strb     sb, [sp, #0x10]
0041df34: add      r0, r0, #0x18c
0041df38: strb     sb, [sp, #0x11]
0041df3c: bl       #0x427d50
0041df40: ldr      r2, [r0]
0041df44: mov      r3, r0
0041df48: mov      r0, r8
0041df4c: ldr      sl, [r2, #0x20]
0041df50: str      r3, [sp, #4]
0041df54: bl       #0x41ddec
0041df58: ldr      r3, [sp, #4]
0041df5c: mov      r1, r8
0041df60: mov      r2, r7
0041df64: mov      r0, r3
0041df68: blx      sl
0041df6c: ldrsb    r3, [sp, #0x20]
0041df70: cmn      r3, #1
0041df74: beq      #0x41dffc
0041df78: mov      r0, r7
0041df7c: bl       #0x797a54
0041df80: bl       #0x30ea24
0041df84: mla      sl, fp, r4, r6
0041df88: cmp      r0, #2
0041df8c: movhi    r0, #0
0041df90: add      sl, sl, #0x2ac
0041df94: str      r0, [sp, #8]
0041df98: mov      r0, sl
0041df9c: bl       #0x427d50
0041dfa0: cmp      r0, #0
0041dfa4: beq      #0x41dfc8
0041dfa8: mov      r0, sl
0041dfac: bl       #0x427d50
0041dfb0: ldr      r2, [sp, #8]
0041dfb4: add      r1, sp, #0x38
0041dfb8: add      r3, r1, r2
0041dfbc: ldrb     r3, [r3, #-0x1c]
0041dfc0: eor      r3, r3, #1
0041dfc4: strb     r3, [r0, #0x9b]
0041dfc8: add      r4, r4, #1
0041dfcc: mov      r0, r7
0041dfd0: bl       #0x797124
0041dfd4: cmp      r4, #3
0041dfd8: bne      #0x41df2c
0041dfdc: ldr      r2, [sp, #0xc]
0041dfe0: ldr      r3, [r5, r2]
0041dfe4: ldr      r2, [sp, #0x34]
0041dfe8: ldr      r3, [r3]
0041dfec: cmp      r2, r3
0041dff0: bne      #0x41e050
0041dff4: add      sp, sp, #0x3c
0041dff8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041dffc: ldr      r0, [sp, #0x2c]
0041e000: ldr      r1, [sp, #0x28]
0041e004: bl       #0x752b38
0041e008: b        #0x41df78
0041e00c: mov      r4, #0
0041e010: mov      r8, #0x30
0041e014: mla      r7, r8, r4, r6
0041e018: add      r7, r7, #0x2ac
0041e01c: mov      r0, r7
0041e020: bl       #0x427d50
0041e024: cmp      r0, #0
0041e028: beq      #0x41e040
0041e02c: mov      r0, r7
0041e030: bl       #0x427d50
0041e034: ldrb     r3, [sl, r4]
0041e038: eor      r3, r3, #1
0041e03c: strb     r3, [r0, #0x9b]
0041e040: add      r4, r4, #1
0041e044: cmp      r4, #3
0041e048: bne      #0x41e014
0041e04c: b        #0x41dfdc
0041e050: bl       #0x30e310
0041e054: subseq   r6, r7, ip, lsr #24
0041e058: andeq    r4, r0, ip, lsr #1
0041e05c: strdeq   r3, r4, [r0], -r4
0041e060: strdeq   r3, r4, [sl], #-0x64

# _ZN14InfoHUDManager6UpdateEv
0041ec00: push     {r4, r5, r6, r7, r8, lr}
0041ec04: ldr      r4, [pc, #0x9c]
0041ec08: ldr      r6, [pc, #0x9c]
0041ec0c: mov      r5, r0
0041ec10: add      r4, pc, r4
0041ec14: ldr      r0, [r4, r6]
0041ec18: bl       #0x31f594
0041ec1c: cmp      r0, #0
0041ec20: beq      #0x41ec30
0041ec24: ldrb     r3, [r0, #0x198]
0041ec28: cmp      r3, #0
0041ec2c: beq      #0x41ec70
0041ec30: ldr      r3, [r5, #0x57c]
0041ec34: cmp      r3, #0
0041ec38: beq      #0x41ec70
0041ec3c: ldrb     r7, [r5, #4]
0041ec40: cmp      r7, #0
0041ec44: beq      #0x41ec74
0041ec48: ldr      r7, [r5, #8]
0041ec4c: cmp      r7, #0
0041ec50: blt      #0x41ec94
0041ec54: ldr      r0, [r4, r6]
0041ec58: bl       #0x31f66c
0041ec5c: rsb      r0, r0, r7
0041ec60: str      r0, [r5, #8]
0041ec64: mov      r0, r5
0041ec68: pop      {r4, r5, r6, r7, r8, lr}
0041ec6c: b        #0x41e064
0041ec70: pop      {r4, r5, r6, r7, r8, pc}
0041ec74: mov      r0, r5
0041ec78: bl       #0x41d880
0041ec7c: mov      r0, r5
0041ec80: bl       #0x41d728
0041ec84: str      r7, [r5]
0041ec88: ldr      r7, [r5, #8]
0041ec8c: cmp      r7, #0
0041ec90: bge      #0x41ec54
0041ec94: mov      r3, #0x1f4
0041ec98: str      r3, [r5, #8]
0041ec9c: mov      r0, r5
0041eca0: bl       #0x41de54
0041eca4: b        #0x41ec64
0041eca8: subseq   r5, r7, r0, lsl #29
0041ecac: strdeq   r3, r4, [r0], -r4

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

# _ZN14InfoHUDManager10FastUpdateEv
0041e064: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0041e068: ldr      r8, [pc, #0xb5c]
0041e06c: ldr      r1, [pc, #0xb5c]
0041e070: ldr      r2, [pc, #0xb5c]
0041e074: add      r8, pc, r8
0041e078: ldr      r3, [r8, r1]
0041e07c: sub      sp, sp, #0x18c
0041e080: str      r2, [sp, #0xc]
0041e084: ldr      r2, [r8, r2]
0041e088: ldr      r3, [r3]
0041e08c: str      r1, [sp, #0x18]
0041e090: mov      r1, #0
0041e094: mov      r4, r0
0041e098: ldr      r0, [r2, #0x40]
0041e09c: mov      r2, r1
0041e0a0: str      r3, [sp, #0x184]
0041e0a4: bl       #0x36e478
0041e0a8: ldr      r6, [r0, #0x660]
0041e0ac: cmp      r6, #0
0041e0b0: beq      #0x41e5c0
0041e0b4: add      r0, r6, #0x37c
0041e0b8: bl       #0x3fc690
0041e0bc: ldr      r1, [pc, #0xb14]
0041e0c0: add      r5, sp, #0xd8
0041e0c4: mov      r2, r0
0041e0c8: add      r1, pc, r1
0041e0cc: mov      r0, r5
0041e0d0: bl       #0x30eae4
0041e0d4: add      r0, r4, #0xfc
0041e0d8: ldr      r7, [r4, #0x57c]
0041e0dc: bl       #0x427d50
0041e0e0: mov      r2, r5
0041e0e4: mov      r1, r0
0041e0e8: mov      r3, #0
0041e0ec: mov      r0, r7
0041e0f0: bl       #0x7a92e0
0041e0f4: movw     r3, #0x1088
0041e0f8: ldr      r0, [r6, r3]
0041e0fc: mov      sl, #0x64
0041e100: movw     r3, #0x1090
0041e104: ldr      r1, [r6, r3]
0041e108: mul      r0, sl, r0
0041e10c: bl       #0x30e2a4
0041e110: movw     r3, #0x109c
0041e114: ldr      r3, [r6, r3]
0041e118: movw     r2, #0x10a4
0041e11c: ldr      r1, [r6, r2]
0041e120: sub      r5, r0, #1
0041e124: mul      r0, sl, r3
0041e128: bl       #0x30e2a4
0041e12c: movw     r3, #0x107c
0041e130: ldr      r3, [r6, r3]
0041e134: sub      r7, r0, #1
0041e138: bic      r5, r5, r5, asr #31
0041e13c: mul      r0, sl, r3
0041e140: mov      r3, #0x1080
0041e144: ldr      r1, [r6, r3]
0041e148: bl       #0x30e2a4
0041e14c: cmp      r0, #0x63
0041e150: movlt    sl, r0
0041e154: movge    sl, #0x63
0041e158: add      r0, r4, #0xc
0041e15c: ldr      sb, [r4, #0x57c]
0041e160: bl       #0x427d50
0041e164: cmp      r5, #0x63
0041e168: movge    r5, #0x63
0041e16c: mov      r1, r0
0041e170: mov      r2, r5
0041e174: mov      r0, sb
0041e178: mov      r3, #0
0041e17c: bl       #0x7a7d34
0041e180: add      r0, r4, #0x9c
0041e184: ldr      sb, [r4, #0x57c]
0041e188: bl       #0x427d50
0041e18c: cmp      r7, #0x63
0041e190: movge    r7, #0x63
0041e194: mov      r1, r0
0041e198: mov      r2, r7
0041e19c: mov      r0, sb
0041e1a0: mov      r3, #0
0041e1a4: bl       #0x7a7d34
0041e1a8: add      r0, r4, #0xcc
0041e1ac: ldr      r7, [r4, #0x57c]
0041e1b0: bl       #0x427d50
0041e1b4: mov      r2, sl
0041e1b8: mov      r1, r0
0041e1bc: mov      r3, #0
0041e1c0: mov      r0, r7
0041e1c4: bl       #0x7a7d34
0041e1c8: add      r0, r4, #0x3c
0041e1cc: ldr      r7, [r4, #0x57c]
0041e1d0: bl       #0x427d50
0041e1d4: mov      r2, r5
0041e1d8: mov      r1, r0
0041e1dc: mov      r3, #0
0041e1e0: mov      r0, r7
0041e1e4: bl       #0x7a7d34
0041e1e8: add      r0, r4, #0x6c
0041e1ec: ldr      r7, [r4, #0x57c]
0041e1f0: bl       #0x427d50
0041e1f4: mov      r2, r5
0041e1f8: mov      r1, r0
0041e1fc: mov      r3, #0
0041e200: mov      r0, r7
0041e204: bl       #0x7a7d34
0041e208: add      r0, r4, #0x33c
0041e20c: bl       #0x427d50
0041e210: mov      r1, #0x94
0041e214: mov      r5, r0
0041e218: mov      r2, #0
0041e21c: add      r0, r6, #0x560
0041e220: bl       #0x3df6e0
0041e224: mov      r7, #0
0041e228: subs     r0, r0, #0
0041e22c: movne    r0, #1
0041e230: strb     r0, [r5, #0x9b]
0041e234: mov      sl, #0
0041e238: mov      sb, r7
0041e23c: add      r5, sp, #0xa4
0041e240: mov      r0, r6
0041e244: mov      r1, sb
0041e248: bl       #0x3bbe68
0041e24c: cmn      r0, #1
0041e250: str      sl, [r5, r7]
0041e254: beq      #0x41e270
0041e258: ldr      r3, [r6, #0x47c]
0041e25c: ldr      r0, [r3, r0, lsl #2]
0041e260: cmp      r0, #0
0041e264: beq      #0x41e270
0041e268: bl       #0x3da3d0
0041e26c: str      r0, [r5, r7]
0041e270: add      r7, r7, #4
0041e274: cmp      r7, #0xc
0041e278: add      sb, sb, #1
0041e27c: bne      #0x41e240
0041e280: ldr      r3, [sp, #0xc]
0041e284: ldr      r1, [pc, #0x950]
0041e288: ldr      r0, [r8, r3]
0041e28c: add      r1, pc, r1
0041e290: bl       #0x320e44
0041e294: cmp      r0, #1
0041e298: ble      #0x41eaac
0041e29c: mov      r5, #0
0041e2a0: add      sb, sp, #0x170
0041e2a4: add      r7, sp, #0x98
0041e2a8: mov      fp, r5
0041e2ac: str      r6, [sp, #0x10]
0041e2b0: mov      r1, #0x30
0041e2b4: mla      r0, r1, r5, r4
0041e2b8: strb     fp, [sp, #0x98]
0041e2bc: add      r0, r0, #0x18c
0041e2c0: strb     fp, [sp, #0x99]
0041e2c4: bl       #0x427d50
0041e2c8: ldr      r3, [r0]
0041e2cc: mov      sl, r0
0041e2d0: mov      r0, sb
0041e2d4: ldr      r6, [r3, #0x20]
0041e2d8: bl       #0x41ddec
0041e2dc: mov      r0, sl
0041e2e0: mov      r1, sb
0041e2e4: mov      r2, r7
0041e2e8: blx      r6
0041e2ec: ldrb     r0, [sp, #0x170]
0041e2f0: sxtb     r3, r0
0041e2f4: cmn      r3, #1
0041e2f8: beq      #0x41e620
0041e2fc: mov      r0, r7
0041e300: bl       #0x797a54
0041e304: bl       #0x30ea24
0041e308: mov      r1, #0x30
0041e30c: mla      r2, r1, r5, r4
0041e310: cmp      r0, #2
0041e314: movls    r3, r0
0041e318: movhi    r3, #0
0041e31c: add      r0, r2, #0x21c
0041e320: ldr      r6, [r4, #0x57c]
0041e324: str      r3, [sp, #8]
0041e328: bl       #0x427d50
0041e32c: ldr      r3, [sp, #8]
0041e330: add      r2, sp, #0x188
0041e334: mov      r1, #0x42000000
0041e338: add      r3, r2, r3, lsl #2
0041e33c: add      r1, r1, #0xc80000
0041e340: mov      sl, r0
0041e344: ldr      r0, [r3, #-0xe4]
0041e348: bl       #0x30ed6c
0041e34c: bl       #0x30e4cc
0041e350: sub      r2, r0, #1
0041e354: mov      r1, sl
0041e358: bic      r2, r2, r2, asr #31
0041e35c: mov      r0, r6
0041e360: mov      r3, #0
0041e364: bl       #0x7a7d34
0041e368: add      r5, r5, #1
0041e36c: mov      r0, r7
0041e370: bl       #0x797124
0041e374: cmp      r5, #3
0041e378: bne      #0x41e2b0
0041e37c: ldr      r6, [sp, #0x10]
0041e380: ldr      r3, [r6, #0x47c]
0041e384: ldr      r3, [r3]
0041e388: cmp      r3, #0
0041e38c: beq      #0x41e3d4
0041e390: add      r0, r4, #0x12c
0041e394: ldr      r5, [r4, #0x57c]
0041e398: bl       #0x427d50
0041e39c: ldr      r3, [r6, #0x488]
0041e3a0: mov      r7, r0
0041e3a4: ldr      r0, [r3]
0041e3a8: bl       #0x3da3d0
0041e3ac: mov      r1, #0x42000000
0041e3b0: add      r1, r1, #0xc80000
0041e3b4: bl       #0x30ed6c
0041e3b8: bl       #0x30e4cc
0041e3bc: sub      r2, r0, #1
0041e3c0: mov      r1, r7
0041e3c4: mov      r0, r5
0041e3c8: bic      r2, r2, r2, asr #31
0041e3cc: mov      r3, #0
0041e3d0: bl       #0x7a7d34
0041e3d4: bl       #0x7fd794
0041e3d8: ldrb     r3, [r0, #5]
0041e3dc: cmp      r3, #0
0041e3e0: bne      #0x41e674
0041e3e4: ldr      r3, [r6]
0041e3e8: mov      r0, r6
0041e3ec: mov      lr, pc
0041e3f0: ldr      pc, [r3, #0x34]
0041e3f4: cmp      r0, #0
0041e3f8: beq      #0x41e5e0
0041e3fc: mov      r7, #0
0041e400: add      r5, r6, #0x3c8
0041e404: mov      r0, r5
0041e408: bl       #0x3d5450
0041e40c: cmp      r0, #0
0041e410: beq      #0x41e428
0041e414: mov      r0, r5
0041e418: bl       #0x3d5450
0041e41c: bl       #0x3a3064
0041e420: cmp      r0, #0
0041e424: bne      #0x41eb04
0041e428: cmp      r7, #0
0041e42c: beq      #0x41eb58
0041e430: movw     r3, #0x14a4
0041e434: ldr      r5, [r6, r3]
0041e438: cmp      r5, #0
0041e43c: beq      #0x41eb58
0041e440: ldr      r3, [r4]
0041e444: cmp      r5, r3
0041e448: beq      #0x41e56c
0041e44c: ldr      r1, [sp, #0xc]
0041e450: mov      r2, #0x1040
0041e454: str      r5, [r4]
0041e458: ldr      r3, [r8, r1]
0041e45c: ldr      r1, [r5, r2]
0041e460: ldr      r0, [r3, #0x34]
0041e464: bl       #0x508edc
0041e468: mov      sb, r0
0041e46c: mov      r0, r5
0041e470: bl       #0x3a3158
0041e474: cmp      r0, #0
0041e478: beq      #0x41eb2c
0041e47c: movw     r3, #0x3f3f
0041e480: strh     r3, [sp, #0x98]
0041e484: mov      r3, #0
0041e488: strb     r3, [sp, #0x9a]
0041e48c: add      r7, sp, #0x98
0041e490: add      r6, r4, #0x39c
0041e494: mov      r0, r6
0041e498: bl       #0x427d50
0041e49c: mov      r3, #1
0041e4a0: strb     r3, [r0, #0x9b]
0041e4a4: mov      r0, r6
0041e4a8: ldr      r6, [r4, #0x57c]
0041e4ac: bl       #0x427d50
0041e4b0: ldr      r2, [pc, #0x728]
0041e4b4: mov      r1, r0
0041e4b8: mov      r3, #0
0041e4bc: add      r2, pc, r2
0041e4c0: mov      r0, r6
0041e4c4: bl       #0x7ab924
0041e4c8: ldr      r3, [pc, #0x714]
0041e4cc: add      sl, sp, #0xf8
0041e4d0: ldr      r6, [r8, r3]
0041e4d4: mov      r0, r6
0041e4d8: bl       #0x337888
0041e4dc: ldr      r1, [pc, #0x704]
0041e4e0: mov      r0, sl
0041e4e4: str      sl, [sp, #0x108]
0041e4e8: add      r1, pc, r1
0041e4ec: add      r2, r1, #0x19
0041e4f0: str      sl, [sp, #0x10c]
0041e4f4: bl       #0x3116e8
0041e4f8: mov      r0, r6
0041e4fc: mov      r1, sl
0041e500: bl       #0x337a88
0041e504: mov      r6, r0
0041e508: mov      r0, sl
0041e50c: bl       #0x3139ac
0041e510: cmp      r6, #0
0041e514: beq      #0x41e630
0041e518: ldr      r1, [pc, #0x6cc]
0041e51c: ldr      r2, [r5, #0x108]
0041e520: mov      r0, r7
0041e524: add      r1, pc, r1
0041e528: bl       #0x30eae4
0041e52c: add      r0, r4, #0x3cc
0041e530: ldr      r6, [r4, #0x57c]
0041e534: bl       #0x427d50
0041e538: ldr      r2, [r5, #0x44]
0041e53c: mov      r1, r0
0041e540: mov      r3, #0
0041e544: mov      r0, r6
0041e548: bl       #0x7a92e0
0041e54c: add      r0, r4, #0x3fc
0041e550: ldr      r6, [r4, #0x57c]
0041e554: bl       #0x427d50
0041e558: mov      r2, r7
0041e55c: mov      r1, r0
0041e560: mov      r3, #0
0041e564: mov      r0, r6
0041e568: bl       #0x7a92e0
0041e56c: mov      r0, r5
0041e570: bl       #0x3bd2dc
0041e574: mov      r6, r0
0041e578: add      r0, r4, #0x420
0041e57c: add      r0, r0, #0xc
0041e580: ldr      r4, [r4, #0x57c]
0041e584: bl       #0x427d50
0041e588: mov      r1, #0x42000000
0041e58c: add      r1, r1, #0xc80000
0041e590: mov      r5, r0
0041e594: mov      r0, r6
0041e598: bl       #0x30ed6c
0041e59c: bl       #0x30e4cc
0041e5a0: cmp      r0, #0x64
0041e5a4: movlt    r2, r0
0041e5a8: movge    r2, #0x64
0041e5ac: mov      r1, r5
0041e5b0: mov      r0, r4
0041e5b4: sub      r2, r2, #1
0041e5b8: mov      r3, #0
0041e5bc: bl       #0x7a7d34
0041e5c0: ldr      r2, [sp, #0x18]
0041e5c4: ldr      r3, [r8, r2]
0041e5c8: ldr      r2, [sp, #0x184]
0041e5cc: ldr      r3, [r3]
0041e5d0: cmp      r2, r3
0041e5d4: bne      #0x41ebc8
0041e5d8: add      sp, sp, #0x18c
0041e5dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041e5e0: movw     r5, #0x14a4
0041e5e4: ldr      r3, [r6, r5]
0041e5e8: cmp      r3, #0
0041e5ec: beq      #0x41e3fc
0041e5f0: mov      r0, r3
0041e5f4: ldr      r3, [r3]
0041e5f8: mov      lr, pc
0041e5fc: ldr      pc, [r3, #0x24]
0041e600: cmp      r0, #0
0041e604: beq      #0x41e3fc
0041e608: ldr      r0, [r6, r5]
0041e60c: bl       #0x3a3064
0041e610: cmp      r0, #0
0041e614: movne    r7, #1
0041e618: bne      #0x41e400
0041e61c: b        #0x41e3fc
0041e620: ldr      r0, [sp, #0x17c]
0041e624: ldr      r1, [sp, #0x178]
0041e628: bl       #0x752b38
0041e62c: b        #0x41e2fc
0041e630: add      r0, r4, #0x3cc
0041e634: ldr      sl, [r4, #0x57c]
0041e638: bl       #0x427d50
0041e63c: mov      r2, sb
0041e640: mov      r1, r0
0041e644: mov      r3, r6
0041e648: mov      r0, sl
0041e64c: bl       #0x7a92e0
0041e650: add      r0, r4, #0x3fc
0041e654: ldr      sl, [r4, #0x57c]
0041e658: bl       #0x427d50
0041e65c: mov      r2, r7
0041e660: mov      r1, r0
0041e664: mov      r3, r6
0041e668: mov      r0, sl
0041e66c: bl       #0x7a92e0
0041e670: b        #0x41e56c
0041e674: ldr      r0, [sp, #0xc]
0041e678: mov      r1, #0
0041e67c: mov      r2, r1
0041e680: ldr      r3, [r8, r0]
0041e684: ldr      r0, [r3, #0x40]
0041e688: bl       #0x36e478
0041e68c: ldr      r3, [r0, #0x3a8]
0041e690: cmp      r3, #0
0041e694: blt      #0x41e6e4
0041e698: movw     r2, #0x4dd3
0041e69c: movt     r2, #0x1062
0041e6a0: smull    r1, r2, r2, r3
0041e6a4: ldr      r1, [pc, #0x544]
0041e6a8: asr      r3, r3, #0x1f
0041e6ac: add      r5, sp, #0xb8
0041e6b0: rsb      r2, r3, r2, asr #6
0041e6b4: add      r1, pc, r1
0041e6b8: mov      r0, r5
0041e6bc: bl       #0x30eae4
0041e6c0: add      r0, r4, #0x4b0
0041e6c4: add      r0, r0, #0xc
0041e6c8: ldr      r7, [r4, #0x57c]
0041e6cc: bl       #0x427d50
0041e6d0: mov      r2, r5
0041e6d4: mov      r1, r0
0041e6d8: mov      r3, #0
0041e6dc: mov      r0, r7
0041e6e0: bl       #0x7a92e0
0041e6e4: ldr      r2, [sp, #0xc]
0041e6e8: add      r5, sp, #0x44
0041e6ec: mov      sl, #0
0041e6f0: ldr      r3, [r8, r2]
0041e6f4: mov      fp, r4
0041e6f8: mov      r7, sl
0041e6fc: ldr      r0, [r3, #0x40]
0041e700: bl       #0x36d7a8
0041e704: movw     r3, #0x4dd3
0041e708: movt     r3, #0x1062
0041e70c: str      r3, [sp, #0x38]
0041e710: ldr      r3, [pc, #0x4dc]
0041e714: str      r0, [sp, #0x28]
0041e718: add      r1, r5, #0xc
0041e71c: add      r3, pc, r3
0041e720: str      r3, [sp, #0x34]
0041e724: add      r0, sp, #0x110
0041e728: add      r3, sp, #0x158
0041e72c: str      sl, [sp, #0x20]
0041e730: add      sb, sp, #0x128
0041e734: str      r0, [sp, #0x10]
0041e738: str      r1, [sp, #0x30]
0041e73c: str      r6, [sp, #0x3c]
0041e740: str      r8, [sp, #0x24]
0041e744: mov      r4, r3
0041e748: mov      r0, #0x30
0041e74c: mla      r0, r0, sl, fp
0041e750: add      r0, r0, #0x4e0
0041e754: add      r0, r0, #0xc
0041e758: bl       #0x427d50
0041e75c: mov      r1, #0x10
0041e760: str      r0, [sp, #0x2c]
0041e764: mov      r0, r4
0041e768: str      r4, [sp, #0x168]
0041e76c: str      r4, [sp, #0x16c]
0041e770: bl       #0x31167c
0041e774: ldr      r3, [sp, #0x28]
0041e778: ldr      r2, [sp, #0x20]
0041e77c: cmp      r2, r3
0041e780: ldr      r3, [sp, #0x168]
0041e784: strb     r7, [r3]
0041e788: blt      #0x41e910
0041e78c: mov      r6, #0
0041e790: mvn      r0, #0
0041e794: str      r6, [sp, #0x14]
0041e798: mov      r3, r6
0041e79c: str      r0, [sp, #0x1c]
0041e7a0: ldr      r0, [sp, #0x24]
0041e7a4: ldr      r1, [sp, #0xc]
0041e7a8: mov      r8, #1
0041e7ac: str      sb, [sp, #0x138]
0041e7b0: ldr      r2, [r0, r1]
0041e7b4: mov      r0, sb
0041e7b8: ldr      r1, [sp, #0x16c]
0041e7bc: ldr      ip, [r2, #0x34]
0041e7c0: ldr      r2, [sp, #0x168]
0041e7c4: strb     r3, [sp, #0x48]
0041e7c8: strb     r8, [sp, #0x45]
0041e7cc: str      ip, [sp, #8]
0041e7d0: strb     r7, [sp, #0x44]
0041e7d4: str      sb, [sp, #0x13c]
0041e7d8: bl       #0x3116e8
0041e7dc: ldr      ip, [sp, #8]
0041e7e0: mov      r2, sb
0041e7e4: mov      r3, r8
0041e7e8: mov      r1, ip
0041e7ec: ldr      r0, [sp, #0x10]
0041e7f0: bl       #0x507c0c
0041e7f4: ldr      r1, [sp, #0x124]
0041e7f8: ldr      r0, [sp, #0x30]
0041e7fc: strb     r7, [sp, #0x50]
0041e800: strb     r7, [sp, #0x51]
0041e804: bl       #0x797350
0041e808: ldr      r0, [sp, #0x10]
0041e80c: bl       #0x3139ac
0041e810: mov      r0, sb
0041e814: bl       #0x3139ac
0041e818: mov      r8, #2
0041e81c: ldr      r0, [sp, #0x14]
0041e820: strb     r7, [sp, #0x5c]
0041e824: strb     r8, [sp, #0x5d]
0041e828: bl       #0x30ed30
0041e82c: strd     r0, r1, [sp, #0xb0]
0041e830: ldr      r3, [sp, #0xb0]
0041e834: mov      r0, r6
0041e838: str      r3, [r5, #0x1c]
0041e83c: ldr      r3, [sp, #0xb4]
0041e840: str      r3, [r5, #0x20]
0041e844: strb     r7, [sp, #0x68]
0041e848: strb     r8, [sp, #0x69]
0041e84c: bl       #0x30ed30
0041e850: strd     r0, r1, [sp, #0xb0]
0041e854: ldr      r3, [sp, #0xb0]
0041e858: mov      r0, sl
0041e85c: add      sl, sl, #1
0041e860: str      r3, [r5, #0x28]
0041e864: ldr      r3, [sp, #0xb4]
0041e868: str      r3, [r5, #0x2c]
0041e86c: strb     r7, [sp, #0x74]
0041e870: strb     r8, [sp, #0x75]
0041e874: bl       #0x30ed30
0041e878: strd     r0, r1, [sp, #0xb0]
0041e87c: ldr      r3, [sp, #0xb0]
0041e880: ldr      r0, [sp, #0x1c]
0041e884: str      r3, [r5, #0x34]
0041e888: ldr      r3, [sp, #0xb4]
0041e88c: str      r3, [r5, #0x38]
0041e890: strb     r8, [sp, #0x81]
0041e894: strb     r7, [sp, #0x80]
0041e898: bl       #0x30ed30
0041e89c: strd     r0, r1, [sp, #0xb0]
0041e8a0: ldr      r3, [sp, #0xb0]
0041e8a4: ldr      r6, [fp, #0x57c]
0041e8a8: str      r3, [r5, #0x40]
0041e8ac: ldr      r3, [sp, #0xb4]
0041e8b0: mov      r0, r6
0041e8b4: str      r3, [r5, #0x44]
0041e8b8: bl       #0x7a7ca0
0041e8bc: mov      ip, #6
0041e8c0: mov      r1, r0
0041e8c4: ldr      r2, [sp, #0x34]
0041e8c8: mov      r0, r6
0041e8cc: mov      r3, r5
0041e8d0: str      ip, [sp]
0041e8d4: bl       #0x7abe0c
0041e8d8: add      r6, r5, #0x48
0041e8dc: sub      r6, r6, #0xc
0041e8e0: mov      r0, r6
0041e8e4: bl       #0x797124
0041e8e8: cmp      r6, r5
0041e8ec: bne      #0x41e8dc
0041e8f0: mov      r0, r4
0041e8f4: bl       #0x3139ac
0041e8f8: cmp      sl, #2
0041e8fc: ble      #0x41e748
0041e900: ldr      r6, [sp, #0x3c]
0041e904: mov      r4, fp
0041e908: ldr      r8, [sp, #0x24]
0041e90c: b        #0x41e3e4
0041e910: ldr      r1, [sp, #0x24]
0041e914: ldr      r0, [sp, #0xc]
0041e918: mov      r2, #0
0041e91c: ldr      r3, [r1, r0]
0041e920: ldr      r1, [sp, #0x20]
0041e924: ldr      r0, [r3, #0x40]
0041e928: bl       #0x36e744
0041e92c: ldr      r3, [r0]
0041e930: mov      r8, r0
0041e934: mov      lr, pc
0041e938: ldr      pc, [r3, #0x50]
0041e93c: cmp      r0, #0
0041e940: bne      #0x41eb14
0041e944: ldr      r0, [r8, #0x660]
0041e948: cmp      r0, #0
0041e94c: beq      #0x41e78c
0041e950: ldrb     r3, [r8, #0x4e5]
0041e954: cmp      r3, #0
0041e958: beq      #0x41e78c
0041e95c: ldr      r3, [r8, #0x330]
0041e960: str      r3, [sp, #0x14]
0041e964: bl       #0x3bd2dc
0041e968: mov      r1, #0x42000000
0041e96c: add      r1, r1, #0xc80000
0041e970: bl       #0x30ed6c
0041e974: bl       #0x30e4cc
0041e978: add      r3, sp, #0x140
0041e97c: str      r3, [sp, #0x150]
0041e980: str      r3, [sp, #0x154]
0041e984: ldr      r1, [r8, #0x2e4]
0041e988: ldr      r2, [r8, #0x2e0]
0041e98c: sub      r6, r0, #1
0041e990: mov      r0, r3
0041e994: str      r3, [sp, #8]
0041e998: bl       #0x3116e8
0041e99c: ldr      r1, [sp, #0x154]
0041e9a0: ldr      r2, [sp, #0x150]
0041e9a4: mov      r0, r4
0041e9a8: bl       #0x3109e0
0041e9ac: ldr      r3, [sp, #8]
0041e9b0: cmp      r6, #0x63
0041e9b4: movge    r6, #0x63
0041e9b8: bic      r6, r6, r6, asr #31
0041e9bc: mov      r0, r3
0041e9c0: bl       #0x3139ac
0041e9c4: ldr      r3, [r8, #0x3a8]
0041e9c8: cmp      r3, #0
0041e9cc: ldrge    r1, [sp, #0x38]
0041e9d0: mvnlt    r0, #0
0041e9d4: strlt    r0, [sp, #0x1c]
0041e9d8: smullge  r1, r2, r1, r3
0041e9dc: asrge    r3, r3, #0x1f
0041e9e0: rsbge    r3, r3, r2, asr #6
0041e9e4: ldr      r2, [sp, #0x20]
0041e9e8: strge    r3, [sp, #0x1c]
0041e9ec: ldr      r3, [r8, #0x660]
0041e9f0: add      r2, r2, #1
0041e9f4: str      r2, [sp, #0x20]
0041e9f8: ldr      r2, [r3, #0x160]
0041e9fc: ldr      r0, [r3, #0x168]
0041ea00: ldr      r3, [r3, #0x164]
0041ea04: mov      r1, #0x43000000
0041ea08: add      r1, r1, #0xaf0000
0041ea0c: str      r2, [sp, #0x8c]
0041ea10: str      r3, [sp, #0x90]
0041ea14: bl       #0x30eba4
0041ea18: mov      r3, #0
0041ea1c: add      r1, sp, #0x98
0041ea20: str      r0, [sp, #0x94]
0041ea24: add      r0, sp, #0x8c
0041ea28: str      r3, [sp, #0x98]
0041ea2c: str      r3, [sp, #0x9c]
0041ea30: bl       #0x50e830
0041ea34: ldr      r0, [fp, #0x57c]
0041ea38: bl       #0x7a7cac
0041ea3c: bl       #0x416538
0041ea40: mov      r8, r0
0041ea44: ldr      r0, [fp, #0x57c]
0041ea48: bl       #0x7a7cac
0041ea4c: bl       #0x416578
0041ea50: mov      r3, r0
0041ea54: ldr      r0, [sp, #0x98]
0041ea58: str      r3, [sp, #8]
0041ea5c: bl       #0x30e964
0041ea60: mov      r1, r0
0041ea64: mov      r0, r8
0041ea68: bl       #0x30ed6c
0041ea6c: bl       #0x30e4cc
0041ea70: mov      r8, r0
0041ea74: ldr      r0, [sp, #0x9c]
0041ea78: bl       #0x30e964
0041ea7c: ldr      r3, [sp, #8]
0041ea80: mov      r1, r0
0041ea84: mov      r0, r3
0041ea88: bl       #0x30ed6c
0041ea8c: bl       #0x30e4cc
0041ea90: ldr      r1, [sp, #0x2c]
0041ea94: mov      r3, r0
0041ea98: mov      r2, r8
0041ea9c: ldr      r0, [fp, #0x57c]
0041eaa0: bl       #0x7aa3f0
0041eaa4: mov      r3, #1
0041eaa8: b        #0x41e7a0
0041eaac: mov      r7, #0
0041eab0: mov      fp, #0x30
0041eab4: mla      r0, fp, r7, r4
0041eab8: ldr      sl, [r4, #0x57c]
0041eabc: add      r0, r0, #0x21c
0041eac0: bl       #0x427d50
0041eac4: mov      r1, #0x42000000
0041eac8: add      r1, r1, #0xc80000
0041eacc: mov      sb, r0
0041ead0: ldr      r0, [r5, r7, lsl #2]
0041ead4: bl       #0x30ed6c
0041ead8: bl       #0x30e4cc
0041eadc: sub      r2, r0, #1
0041eae0: mov      r1, sb
0041eae4: mov      r0, sl
0041eae8: bic      r2, r2, r2, asr #31
0041eaec: add      r7, r7, #1
0041eaf0: mov      r3, #0
0041eaf4: bl       #0x7a7d34
0041eaf8: cmp      r7, #3
0041eafc: bne      #0x41eab4
0041eb00: b        #0x41e380
0041eb04: mov      r0, r5
0041eb08: bl       #0x3d5450
0041eb0c: mov      r5, r0
0041eb10: b        #0x41e438
0041eb14: ldr      r2, [sp, #0x20]
0041eb18: mov      r0, r4
0041eb1c: add      r2, r2, #1
0041eb20: str      r2, [sp, #0x20]
0041eb24: bl       #0x3139ac
0041eb28: b        #0x41e8f8
0041eb2c: mov      r0, r5
0041eb30: bl       #0x3bd120
0041eb34: cmn      r0, #1
0041eb38: mov      r2, r0
0041eb3c: beq      #0x41e47c
0041eb40: ldr      r1, [pc, #0xb0]
0041eb44: add      r7, sp, #0x98
0041eb48: mov      r0, r7
0041eb4c: add      r1, pc, r1
0041eb50: bl       #0x30eae4
0041eb54: b        #0x41e490
0041eb58: ldr      r3, [r4]
0041eb5c: cmp      r3, #0
0041eb60: beq      #0x41e5c0
0041eb64: mov      r5, #0
0041eb68: mov      r6, r4
0041eb6c: str      r5, [r6], #0x39c
0041eb70: mov      r0, r6
0041eb74: bl       #0x427d50
0041eb78: strb     r5, [r0, #0x9b]
0041eb7c: add      r0, r4, #0x420
0041eb80: add      r0, r0, #0xc
0041eb84: ldr      r7, [r4, #0x57c]
0041eb88: bl       #0x427d50
0041eb8c: mov      r2, r5
0041eb90: mov      r1, r0
0041eb94: mov      r3, r5
0041eb98: mov      r0, r7
0041eb9c: bl       #0x7a7d34
0041eba0: mov      r0, r6
0041eba4: ldr      r4, [r4, #0x57c]
0041eba8: bl       #0x427d50
0041ebac: ldr      r2, [pc, #0x48]
0041ebb0: mov      r1, r0
0041ebb4: mov      r3, r5
0041ebb8: mov      r0, r4
0041ebbc: add      r2, pc, r2
0041ebc0: bl       #0x7ab924
0041ebc4: b        #0x41e5c0
0041ebc8: bl       #0x30e310
0041ebcc: subseq   r6, r7, ip, lsl sl
0041ebd0: andeq    r4, r0, ip, lsr #1
0041ebd4: strdeq   r3, r4, [r0], -r4
0041ebd8: subeq    r3, sl, r8, ror #27
0041ebdc: subeq    r3, sl, ip, ror #6
0041ebe0: subeq    sl, sl, ip, lsr r4
0041ebe4: andeq    r0, r0, r4, lsl #17
0041ebe8: umaaleq  sl, sl, r0, sb
0041ebec: subeq    r3, sl, ip, lsl #19
0041ebf0: strdeq   r3, r4, [sl], #-0x7c
0041ebf4: subeq    sl, sl, r4, asr #14
0041ebf8: subeq    r3, sl, r4, ror #6
0041ebfc: subeq    sb, sl, r4, lsr sp
