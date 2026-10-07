
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

# _ZN14InfoHUDManager18applyOneTimeValuesEv
0041d728: push     {r4, r5, r6, r7, r8, lr}
0041d72c: mov      r4, r0
0041d730: add      r0, r0, #0x36c
0041d734: bl       #0x427d50
0041d738: ldr      r7, [pc, #0x134]
0041d73c: ldr      r3, [pc, #0x134]
0041d740: ldr      r1, [pc, #0x134]
0041d744: add      r7, pc, r7
0041d748: ldr      r5, [r7, r3]
0041d74c: mov      r6, r0
0041d750: add      r1, pc, r1
0041d754: mov      r0, r5
0041d758: bl       #0x320e44
0041d75c: subs     r0, r0, #0
0041d760: movne    r0, #1
0041d764: strb     r0, [r6, #0x9b]
0041d768: mov      r1, #0
0041d76c: ldr      r0, [r5, #0x40]
0041d770: mov      r2, r1
0041d774: bl       #0x36e478
0041d778: ldr      r0, [r0, #0x660]
0041d77c: cmp      r0, #0
0041d780: beq      #0x41d870
0041d784: bl       #0x3bb7fc
0041d788: sub      r0, r0, #0x120
0041d78c: sub      r0, r0, #2
0041d790: cmp      r0, #0x25
0041d794: addls    pc, pc, r0, lsl #2
0041d798: b        #0x41d868
0041d79c: b        #0x41d860
0041d7a0: b        #0x41d860
0041d7a4: b        #0x41d860
0041d7a8: b        #0x41d868
0041d7ac: b        #0x41d868
0041d7b0: b        #0x41d868
0041d7b4: b        #0x41d868
0041d7b8: b        #0x41d868
0041d7bc: b        #0x41d868
0041d7c0: b        #0x41d868
0041d7c4: b        #0x41d868
0041d7c8: b        #0x41d868
0041d7cc: b        #0x41d868
0041d7d0: b        #0x41d868
0041d7d4: b        #0x41d868
0041d7d8: b        #0x41d868
0041d7dc: b        #0x41d868
0041d7e0: b        #0x41d868
0041d7e4: b        #0x41d868
0041d7e8: b        #0x41d868
0041d7ec: b        #0x41d868
0041d7f0: b        #0x41d868
0041d7f4: b        #0x41d868
0041d7f8: b        #0x41d868
0041d7fc: b        #0x41d868
0041d800: b        #0x41d868
0041d804: b        #0x41d868
0041d808: b        #0x41d868
0041d80c: b        #0x41d868
0041d810: b        #0x41d868
0041d814: b        #0x41d868
0041d818: b        #0x41d868
0041d81c: b        #0x41d868
0041d820: b        #0x41d868
0041d824: b        #0x41d868
0041d828: b        #0x41d834
0041d82c: b        #0x41d834
0041d830: b        #0x41d834
0041d834: mov      r5, #1
0041d838: add      r0, r4, #0x450
0041d83c: add      r0, r0, #0xc
0041d840: ldr      r4, [r4, #0x57c]
0041d844: bl       #0x427d50
0041d848: mov      r2, r5
0041d84c: mov      r1, r0
0041d850: mov      r3, #0
0041d854: mov      r0, r4
0041d858: pop      {r4, r5, r6, r7, r8, lr}
0041d85c: b        #0x7a7d34
0041d860: mov      r5, #2
0041d864: b        #0x41d838
0041d868: mov      r5, #0
0041d86c: b        #0x41d838
0041d870: pop      {r4, r5, r6, r7, r8, pc}
0041d874: subseq   r7, r7, ip, asr #6
0041d878: strdeq   r3, r4, [r0], -r4
0041d87c: strdeq   r1, r2, [sl], #-0x40

# _ZN14InfoHUDManager15initCachedCharsEv
0041d880: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0041d884: ldr      r6, [pc, #0x3f4]
0041d888: ldr      sb, [pc, #0x3f4]
0041d88c: ldr      r2, [r0, #0x57c]
0041d890: add      r6, pc, r6
0041d894: ldr      r3, [r6, sb]
0041d898: sub      sp, sp, #0x64
0041d89c: cmp      r2, #0
0041d8a0: ldr      r3, [r3]
0041d8a4: mov      r4, r0
0041d8a8: str      r3, [sp, #0x5c]
0041d8ac: beq      #0x41dbd4
0041d8b0: ldr      r3, [pc, #0x3d0]
0041d8b4: ldr      r1, [pc, #0x3d0]
0041d8b8: add      r5, sp, #0x48
0041d8bc: ldr      r0, [r6, r3]
0041d8c0: add      r1, pc, r1
0041d8c4: bl       #0x320e44
0041d8c8: ldr      r1, [pc, #0x3c0]
0041d8cc: mov      r2, r0
0041d8d0: mov      r7, r0
0041d8d4: add      r1, pc, r1
0041d8d8: mov      r0, r5
0041d8dc: bl       #0x30eae4
0041d8e0: mov      r1, r5
0041d8e4: ldr      r0, [r4, #0x57c]
0041d8e8: bl       #0x7a9160
0041d8ec: ldr      r1, [pc, #0x3a0]
0041d8f0: mov      r5, r0
0041d8f4: ldr      r2, [r4, #0x57c]
0041d8f8: add      r0, r4, #0xc
0041d8fc: add      r1, pc, r1
0041d900: mov      r3, r5
0041d904: bl       #0x427ca0
0041d908: ldr      r1, [pc, #0x388]
0041d90c: add      r0, r4, #0x3c
0041d910: ldr      r2, [r4, #0x57c]
0041d914: add      r1, pc, r1
0041d918: mov      r3, r5
0041d91c: bl       #0x427ca0
0041d920: ldr      r1, [pc, #0x374]
0041d924: add      r0, r4, #0x6c
0041d928: ldr      r2, [r4, #0x57c]
0041d92c: add      r1, pc, r1
0041d930: mov      r3, #0
0041d934: bl       #0x427ca0
0041d938: ldr      r1, [pc, #0x360]
0041d93c: add      r0, r4, #0x9c
0041d940: ldr      r2, [r4, #0x57c]
0041d944: add      r1, pc, r1
0041d948: mov      r3, r5
0041d94c: bl       #0x427ca0
0041d950: ldr      r1, [pc, #0x34c]
0041d954: add      r0, r4, #0xcc
0041d958: ldr      r2, [r4, #0x57c]
0041d95c: add      r1, pc, r1
0041d960: mov      r3, r5
0041d964: bl       #0x427ca0
0041d968: ldr      r1, [pc, #0x338]
0041d96c: add      r0, r4, #0xfc
0041d970: ldr      r2, [r4, #0x57c]
0041d974: add      r1, pc, r1
0041d978: mov      r3, r5
0041d97c: bl       #0x427ca0
0041d980: ldr      r1, [pc, #0x324]
0041d984: add      r0, r4, #0x12c
0041d988: ldr      r2, [r4, #0x57c]
0041d98c: add      r1, pc, r1
0041d990: mov      r3, r5
0041d994: bl       #0x427ca0
0041d998: ldr      r1, [pc, #0x310]
0041d99c: add      r0, r4, #0x15c
0041d9a0: ldr      r2, [r4, #0x57c]
0041d9a4: add      r1, pc, r1
0041d9a8: mov      r3, r5
0041d9ac: bl       #0x427ca0
0041d9b0: cmp      r7, #1
0041d9b4: ble      #0x41dbf0
0041d9b8: ldr      r1, [pc, #0x2f4]
0041d9bc: add      r0, r4, #0x18c
0041d9c0: ldr      r2, [r4, #0x57c]
0041d9c4: add      r1, pc, r1
0041d9c8: mov      r3, r5
0041d9cc: bl       #0x427ca0
0041d9d0: ldr      r1, [pc, #0x2e0]
0041d9d4: add      r0, r4, #0x21c
0041d9d8: ldr      r2, [r4, #0x57c]
0041d9dc: add      r1, pc, r1
0041d9e0: mov      r3, r5
0041d9e4: bl       #0x427ca0
0041d9e8: ldr      r1, [pc, #0x2cc]
0041d9ec: add      r0, r4, #0x2ac
0041d9f0: ldr      r2, [r4, #0x57c]
0041d9f4: add      r1, pc, r1
0041d9f8: mov      r3, r5
0041d9fc: bl       #0x427ca0
0041da00: ldr      r1, [pc, #0x2b8]
0041da04: add      r0, r4, #0x1bc
0041da08: ldr      r2, [r4, #0x57c]
0041da0c: add      r1, pc, r1
0041da10: mov      r3, r5
0041da14: bl       #0x427ca0
0041da18: ldr      r1, [pc, #0x2a4]
0041da1c: add      r0, r4, #0x24c
0041da20: ldr      r2, [r4, #0x57c]
0041da24: add      r1, pc, r1
0041da28: mov      r3, r5
0041da2c: bl       #0x427ca0
0041da30: ldr      r1, [pc, #0x290]
0041da34: add      r0, r4, #0x2dc
0041da38: ldr      r2, [r4, #0x57c]
0041da3c: add      r1, pc, r1
0041da40: mov      r3, r5
0041da44: bl       #0x427ca0
0041da48: ldr      r1, [pc, #0x27c]
0041da4c: add      r0, r4, #0x1ec
0041da50: ldr      r2, [r4, #0x57c]
0041da54: add      r1, pc, r1
0041da58: mov      r3, r5
0041da5c: bl       #0x427ca0
0041da60: ldr      r1, [pc, #0x268]
0041da64: add      r0, r4, #0x27c
0041da68: ldr      r2, [r4, #0x57c]
0041da6c: add      r1, pc, r1
0041da70: mov      r3, r5
0041da74: bl       #0x427ca0
0041da78: ldr      r1, [pc, #0x254]
0041da7c: add      r0, r4, #0x30c
0041da80: ldr      r2, [r4, #0x57c]
0041da84: add      r1, pc, r1
0041da88: mov      r3, r5
0041da8c: bl       #0x427ca0
0041da90: ldr      r1, [pc, #0x240]
0041da94: add      r0, r4, #0x33c
0041da98: ldr      r2, [r4, #0x57c]
0041da9c: add      r1, pc, r1
0041daa0: mov      r3, r5
0041daa4: bl       #0x427ca0
0041daa8: ldr      r1, [pc, #0x22c]
0041daac: add      r0, r4, #0x36c
0041dab0: ldr      r2, [r4, #0x57c]
0041dab4: add      r1, pc, r1
0041dab8: mov      r3, r5
0041dabc: bl       #0x427ca0
0041dac0: ldr      r1, [pc, #0x218]
0041dac4: add      r0, r4, #0x39c
0041dac8: ldr      r2, [r4, #0x57c]
0041dacc: add      r1, pc, r1
0041dad0: mov      r3, r5
0041dad4: bl       #0x427ca0
0041dad8: ldr      r1, [pc, #0x204]
0041dadc: add      r0, r4, #0x3cc
0041dae0: ldr      r2, [r4, #0x57c]
0041dae4: add      r1, pc, r1
0041dae8: mov      r3, r5
0041daec: bl       #0x427ca0
0041daf0: ldr      r1, [pc, #0x1f0]
0041daf4: add      r0, r4, #0x3fc
0041daf8: ldr      r2, [r4, #0x57c]
0041dafc: add      r1, pc, r1
0041db00: mov      r3, r5
0041db04: bl       #0x427ca0
0041db08: ldr      r1, [pc, #0x1dc]
0041db0c: add      r0, r4, #0x420
0041db10: add      r0, r0, #0xc
0041db14: add      r1, pc, r1
0041db18: ldr      r2, [r4, #0x57c]
0041db1c: mov      r3, r5
0041db20: bl       #0x427ca0
0041db24: ldr      r1, [pc, #0x1c4]
0041db28: add      r0, r4, #0x4e0
0041db2c: add      r0, r0, #0xc
0041db30: add      r1, pc, r1
0041db34: ldr      r2, [r4, #0x57c]
0041db38: mov      r3, r5
0041db3c: bl       #0x427ca0
0041db40: ldr      r1, [pc, #0x1ac]
0041db44: add      r0, r4, #0x510
0041db48: add      r0, r0, #0xc
0041db4c: add      r1, pc, r1
0041db50: ldr      r2, [r4, #0x57c]
0041db54: mov      r3, r5
0041db58: bl       #0x427ca0
0041db5c: ldr      r1, [pc, #0x194]
0041db60: add      r0, r4, #0x540
0041db64: add      r0, r0, #0xc
0041db68: add      r1, pc, r1
0041db6c: ldr      r2, [r4, #0x57c]
0041db70: mov      r3, r5
0041db74: bl       #0x427ca0
0041db78: ldr      r1, [pc, #0x17c]
0041db7c: add      r0, r4, #0x450
0041db80: add      r0, r0, #0xc
0041db84: add      r1, pc, r1
0041db88: mov      r3, r5
0041db8c: ldr      r2, [r4, #0x57c]
0041db90: bl       #0x427ca0
0041db94: ldr      r1, [pc, #0x164]
0041db98: add      r0, r4, #0x480
0041db9c: add      r0, r0, #0xc
0041dba0: add      r1, pc, r1
0041dba4: ldr      r2, [r4, #0x57c]
0041dba8: mov      r3, #0
0041dbac: bl       #0x427ca0
0041dbb0: ldr      r1, [pc, #0x14c]
0041dbb4: add      r0, r4, #0x4b0
0041dbb8: mov      r3, #0
0041dbbc: add      r0, r0, #0xc
0041dbc0: add      r1, pc, r1
0041dbc4: ldr      r2, [r4, #0x57c]
0041dbc8: bl       #0x427ca0
0041dbcc: mov      r3, #1
0041dbd0: strb     r3, [r4, #4]
0041dbd4: ldr      r3, [r6, sb]
0041dbd8: ldr      r2, [sp, #0x5c]
0041dbdc: ldr      r3, [r3]
0041dbe0: cmp      r2, r3
0041dbe4: bne      #0x41dc7c
0041dbe8: add      sp, sp, #0x64
0041dbec: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041dbf0: ldr      r3, [pc, #0x110]
0041dbf4: ldr      fp, [pc, #0x110]
0041dbf8: mov      r7, #0
0041dbfc: add      r3, pc, r3
0041dc00: add      fp, pc, fp
0041dc04: str      r3, [sp, #4]
0041dc08: add      r8, sp, #8
0041dc0c: mov      sl, r6
0041dc10: mov      r3, #0x30
0041dc14: mul      r6, r3, r7
0041dc18: add      r7, r7, #1
0041dc1c: mov      r1, fp
0041dc20: mov      r2, r7
0041dc24: mov      r0, r8
0041dc28: bl       #0x30eae4
0041dc2c: add      r0, r4, r6
0041dc30: mov      r3, r5
0041dc34: add      r0, r0, #0x21c
0041dc38: mov      r1, r8
0041dc3c: ldr      r2, [r4, #0x57c]
0041dc40: bl       #0x427ca0
0041dc44: ldr      r1, [sp, #4]
0041dc48: mov      r2, r7
0041dc4c: mov      r0, r8
0041dc50: bl       #0x30eae4
0041dc54: add      r0, r4, r6
0041dc58: add      r0, r0, #0x2ac
0041dc5c: mov      r1, r8
0041dc60: ldr      r2, [r4, #0x57c]
0041dc64: mov      r3, r5
0041dc68: bl       #0x427ca0
0041dc6c: cmp      r7, #3
0041dc70: bne      #0x41dc10
0041dc74: mov      r6, sl
0041dc78: b        #0x41da90
0041dc7c: bl       #0x30e310
0041dc80: subseq   r7, r7, r0, lsl #4
0041dc84: andeq    r4, r0, ip, lsr #1
0041dc88: strdeq   r3, r4, [r0], -r4
0041dc8c: subeq    r3, sl, r8, lsr sp
0041dc90: subeq    r3, sl, ip, ror r7
0041dc94: subeq    fp, sl, r4
0041dc98: subeq    fp, sl, r4, lsl r0
0041dc9c: subeq    fp, sl, ip, lsr #32
0041dca0: subeq    fp, sl, ip, lsr #32
0041dca4: subeq    fp, sl, ip, lsr r0
0041dca8: subeq    fp, sl, ip, asr #32
0041dcac: subeq    fp, sl, r4, rrx
0041dcb0: subeq    fp, sl, r4, lsl #1
0041dcb4: strdeq   fp, ip, [sl], #-0xc
0041dcb8: subeq    fp, sl, r4, lsl r1
0041dcbc: subeq    fp, sl, r4, lsr r1
0041dcc0: subeq    fp, sl, ip, asr #2
0041dcc4: subeq    fp, sl, r4, ror #2
0041dcc8: subeq    fp, sl, r4, lsl #3
0041dccc: subeq    fp, sl, r4, lsr #3
0041dcd0: strheq   fp, [sl], #-0x1c
0041dcd4: ldrdeq   fp, ip, [sl], #-0x1c
0041dcd8: strdeq   fp, ip, [sl], #-0x1c
0041dcdc: subeq    sl, sl, r4, lsl sl
0041dce0: strdeq   fp, ip, [sl], #-0x1c
0041dce4: subeq    fp, sl, r4, lsl #4
0041dce8: subeq    fp, sl, ip, lsl r2
0041dcec: subeq    fp, sl, r4, lsr r2
0041dcf0: subeq    fp, sl, r8, asr #4
0041dcf4: subeq    fp, sl, ip, asr #4
0041dcf8: subeq    fp, sl, r0, asr r2
0041dcfc: subeq    fp, sl, r4, asr r2
0041dd00: subeq    fp, sl, r8, ror #4
0041dd04: subeq    fp, sl, r0, ror #4
0041dd08: umaaleq  sl, sl, r4, lr
0041dd0c: subeq    sl, sl, r8, asr lr

# _ZN7gameswf30create_render_handler_irrlichtEPv
007d6658: push     {r4, r5, r6, lr}
007d665c: mov      r1, #0
007d6660: mov      r5, r0
007d6664: movw     r0, #0x494
007d6668: bl       #0x752ba8
007d666c: mov      r1, r5
007d6670: mov      r4, r0
007d6674: bl       #0x7d60f4
007d6678: mov      r0, r4
007d667c: pop      {r4, r5, r6, pc}

# _ZN7gameswf27substitute_bitmap_characterERNS_9tu_stringEPNS_20bitmap_character_defEPNS_20movie_definition_subE
00759cfc: ldr      r3, [pc, #0x90]
00759d00: ldr      r2, [pc, #0x90]
00759d04: push     {r4, r5, r6, r7, r8, lr}
00759d08: add      r3, pc, r3
00759d0c: ldr      r5, [r3, r2]
00759d10: mov      r6, r0
00759d14: ldr      r3, [r5]
00759d18: cmp      r3, #0
00759d1c: beq      #0x759d90
00759d20: ldr      r3, [r1]
00759d24: mov      r0, r1
00759d28: mov      lr, pc
00759d2c: ldr      pc, [r3, #0x2c]
00759d30: ldrsb    r3, [r6]
00759d34: mov      r4, r0
00759d38: ldr      r5, [r5]
00759d3c: cmn      r3, #1
00759d40: ldr      r3, [r0]
00759d44: addne    r6, r6, #1
00759d48: ldreq    r6, [r6, #0xc]
00759d4c: mov      lr, pc
00759d50: ldr      pc, [r3, #0x24]
00759d54: ldr      r3, [r4]
00759d58: mov      r7, r0
00759d5c: mov      r0, r4
00759d60: mov      lr, pc
00759d64: ldr      pc, [r3, #0x28]
00759d68: mov      r1, r7
00759d6c: mov      r2, r0
00759d70: mov      r0, r6
00759d74: blx      r5
00759d78: subs     r1, r0, #0
00759d7c: beq      #0x759d90
00759d80: mov      r0, r4
00759d84: ldr      r3, [r4]
00759d88: mov      lr, pc
00759d8c: ldr      pc, [r3, #0x20]
00759d90: pop      {r4, r5, r6, r7, r8, pc}
00759d94: eoreq    sl, r3, r8, lsl #27
00759d98: andeq    r2, r0, r8, ror r7

# _ZN7gameswf10scene_node24update_inverse_transformEv
00777350: push     {r4, r5, r6, r7, r8, sl, lr}
00777354: mov      r4, r0
00777358: sub      sp, sp, #0x2c
0077735c: ldr      r0, [r0, #0x234]
00777360: bl       #0x76d5b4
00777364: ldr      r5, [r0, #0x48]
00777368: mov      r3, r0
0077736c: ldr      r0, [r4, #0x258]
00777370: mov      r1, r5
00777374: ldr      r6, [r3, #0x4c]
00777378: bl       #0x30df8c
0077737c: cmp      r0, #0
00777380: bne      #0x77743c
00777384: ldr      r3, [r4, #0x234]
00777388: mov      r0, r5
0077738c: mov      r8, sp
00777390: ldr      r3, [r3, #0xac]
00777394: ldr      r3, [r3, #0x28]
00777398: ldr      r3, [r3, #0xd4]
0077739c: ldr      r3, [r3, #0x1c]
007773a0: ldr      sl, [r3, #0x2c]
007773a4: ldr      r3, [sl]
007773a8: ldr      r7, [r3, #0x14]
007773ac: bl       #0x30e4cc
007773b0: str      r0, [sp, #0x20]
007773b4: mov      r0, r6
007773b8: bl       #0x30e4cc
007773bc: mov      r1, sl
007773c0: str      r0, [sp, #0x24]
007773c4: add      r2, sp, #0x20
007773c8: mov      r0, sp
007773cc: mov      r3, #0
007773d0: blx      r7
007773d4: mov      r3, #0
007773d8: mov      r1, sp
007773dc: mov      r0, r4
007773e0: add      r2, sp, #0x18
007773e4: str      r3, [sp, #0x1c]
007773e8: str      r3, [sp, #0x18]
007773ec: bl       #0x776564
007773f0: cmp      r0, #0
007773f4: bne      #0x7774a4
007773f8: movw     r3, #0x5000
007773fc: movt     r3, #0xc7c3
00777400: str      r3, [r4, #0x260]
00777404: str      r3, [r4, #0x264]
00777408: ldr      r3, [r4, #0x130]
0077740c: mov      r0, r3
00777410: ldr      r3, [r3]
00777414: mov      lr, pc
00777418: ldr      pc, [r3, #0x38]
0077741c: mov      r2, #0x41
00777420: mov      r1, r0
00777424: add      r0, r4, #0x1d8
00777428: bl       #0x30e868
0077742c: str      r5, [r4, #0x258]
00777430: str      r6, [r4, #0x25c]
00777434: add      sp, sp, #0x2c
00777438: pop      {r4, r5, r6, r7, r8, sl, pc}
0077743c: ldr      r0, [r4, #0x25c]
00777440: mov      r1, r6
00777444: bl       #0x30df8c
00777448: cmp      r0, #0
0077744c: beq      #0x777384
00777450: ldr      r3, [r4, #0x130]
00777454: mov      r0, r3
00777458: ldr      r3, [r3]
0077745c: mov      lr, pc
00777460: ldr      pc, [r3, #0x38]
00777464: ldrb     r3, [r0, #0x40]
00777468: mov      sl, r0
0077746c: cmp      r3, #0
00777470: bne      #0x777504
00777474: mov      r8, r4
00777478: mov      r7, #0
0077747c: ldr      r0, [sl, r7]
00777480: ldr      r1, [r8, #0x1d8]
00777484: bl       #0x30df8c
00777488: cmp      r0, #0
0077748c: add      r7, r7, #4
00777490: beq      #0x777384
00777494: cmp      r7, #0x40
00777498: add      r8, r8, #4
0077749c: bne      #0x77747c
007774a0: b        #0x777408
007774a4: ldr      r0, [r4, #0x234]
007774a8: ldr      r7, [sp, #0x18]
007774ac: bl       #0x76d5b4
007774b0: ldr      r0, [r0, #0x1c]
007774b4: bl       #0x30e964
007774b8: mov      r1, r0
007774bc: mov      r0, r7
007774c0: bl       #0x30ed6c
007774c4: mov      r7, r0
007774c8: ldr      r0, [r4, #0x234]
007774cc: ldr      r8, [sp, #0x1c]
007774d0: bl       #0x76d5b4
007774d4: ldr      r0, [r0, #0x20]
007774d8: bl       #0x30e964
007774dc: mov      r1, r0
007774e0: mov      r0, r8
007774e4: bl       #0x30ed6c
007774e8: str      r7, [r4, #0x260]
007774ec: str      r0, [r4, #0x264]
007774f0: ldr      r0, [r4, #0x234]
007774f4: bl       #0x76d5b4
007774f8: add      r1, r4, #0x260
007774fc: bl       #0x773dc0
00777500: b        #0x777408
00777504: ldrb     r3, [r4, #0x218]
00777508: cmp      r3, #0
0077750c: bne      #0x777408
00777510: b        #0x777474

# _ZN11HUDControls15initCachedCharsEv
00419b4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00419b50: ldr      r6, [pc, #0xb94]
00419b54: ldr      r2, [pc, #0xb94]
00419b58: sub      sp, sp, #0x54
00419b5c: add      r6, pc, r6
00419b60: ldr      r3, [r6, r2]
00419b64: str      r2, [sp, #8]
00419b68: ldr      r2, [r0, #0x658]
00419b6c: ldr      r3, [r3]
00419b70: mov      r4, r0
00419b74: cmp      r2, #0
00419b78: str      r3, [sp, #0x4c]
00419b7c: beq      #0x41a230
00419b80: ldr      r3, [pc, #0xb6c]
00419b84: ldr      r1, [pc, #0xb6c]
00419b88: add      r5, sp, #0x38
00419b8c: ldr      r0, [r6, r3]
00419b90: add      r1, pc, r1
00419b94: bl       #0x320e44
00419b98: ldr      r1, [pc, #0xb5c]
00419b9c: mov      r2, r0
00419ba0: str      r0, [sp, #0xc]
00419ba4: add      r1, pc, r1
00419ba8: mov      r0, r5
00419bac: bl       #0x30eae4
00419bb0: mov      r1, r5
00419bb4: ldr      r0, [r4, #0x658]
00419bb8: bl       #0x7a9160
00419bbc: add      r5, r4, #0x388
00419bc0: mov      r1, r0
00419bc4: ldr      r2, [r4, #0x658]
00419bc8: mov      r3, #0
00419bcc: mov      r0, r5
00419bd0: bl       #0x427c44
00419bd4: mov      r0, r5
00419bd8: ldr      r7, [r4, #0x658]
00419bdc: bl       #0x427d50
00419be0: ldr      r1, [pc, #0xb18]
00419be4: add      r2, r4, #0x1c
00419be8: str      r2, [sp, #0x28]
00419bec: mov      r3, r0
00419bf0: mov      r2, r7
00419bf4: add      r1, pc, r1
00419bf8: ldr      r0, [sp, #0x28]
00419bfc: bl       #0x427ca0
00419c00: mov      r0, r5
00419c04: ldr      r7, [r4, #0x658]
00419c08: bl       #0x427d50
00419c0c: ldr      r1, [pc, #0xaf0]
00419c10: add      r2, r4, #0x88
00419c14: str      r2, [sp, #0x34]
00419c18: mov      r3, r0
00419c1c: mov      r2, r7
00419c20: add      r1, pc, r1
00419c24: ldr      r0, [sp, #0x34]
00419c28: bl       #0x427ca0
00419c2c: mov      r0, r5
00419c30: ldr      r7, [r4, #0x658]
00419c34: bl       #0x427d50
00419c38: ldr      r1, [pc, #0xac8]
00419c3c: add      r2, r4, #0x4c
00419c40: str      r2, [sp, #0x24]
00419c44: mov      r3, r0
00419c48: mov      r2, r7
00419c4c: add      r1, pc, r1
00419c50: ldr      r0, [sp, #0x24]
00419c54: bl       #0x427ca0
00419c58: ldr      r1, [pc, #0xaac]
00419c5c: ldr      r2, [r4, #0x658]
00419c60: mov      r3, #0
00419c64: add      r1, pc, r1
00419c68: add      r0, r4, #0xb8
00419c6c: bl       #0x427ca0
00419c70: mov      r0, r5
00419c74: ldr      r8, [r4, #0x658]
00419c78: bl       #0x427d50
00419c7c: ldr      r7, [pc, #0xa8c]
00419c80: mov      r3, r0
00419c84: mov      r2, r8
00419c88: add      r7, pc, r7
00419c8c: mov      r1, r7
00419c90: add      r0, r4, #0xe8
00419c94: bl       #0x427ca0
00419c98: mov      r0, r5
00419c9c: ldr      r8, [r4, #0x658]
00419ca0: bl       #0x427d50
00419ca4: ldr      r1, [pc, #0xa68]
00419ca8: add      r2, r4, #0x3b8
00419cac: str      r2, [sp, #0x30]
00419cb0: mov      r3, r0
00419cb4: mov      r2, r8
00419cb8: add      r1, pc, r1
00419cbc: ldr      r0, [sp, #0x30]
00419cc0: bl       #0x427ca0
00419cc4: add      r3, r4, #0x3e8
00419cc8: mov      r0, r5
00419ccc: ldr      sl, [r4, #0x658]
00419cd0: str      r3, [sp, #0x2c]
00419cd4: bl       #0x427d50
00419cd8: ldr      r8, [pc, #0xa38]
00419cdc: mov      r3, r0
00419ce0: mov      r2, sl
00419ce4: add      r8, pc, r8
00419ce8: mov      r1, r8
00419cec: ldr      r0, [sp, #0x2c]
00419cf0: bl       #0x427ca0
00419cf4: add      r2, r4, #0x410
00419cf8: add      r2, r2, #8
00419cfc: mov      r0, r5
00419d00: ldr      sb, [r4, #0x658]
00419d04: str      r2, [sp, #0x20]
00419d08: bl       #0x427d50
00419d0c: ldr      sl, [pc, #0xa08]
00419d10: mov      r2, sb
00419d14: mov      r3, r0
00419d18: add      sl, pc, sl
00419d1c: mov      r1, sl
00419d20: ldr      r0, [sp, #0x20]
00419d24: bl       #0x427ca0
00419d28: add      r3, r4, #0x440
00419d2c: add      r3, r3, #8
00419d30: mov      r0, r5
00419d34: ldr      fp, [r4, #0x658]
00419d38: str      r3, [sp, #0x1c]
00419d3c: bl       #0x427d50
00419d40: ldr      sb, [pc, #0x9d8]
00419d44: mov      r3, r0
00419d48: mov      r2, fp
00419d4c: add      sb, pc, sb
00419d50: mov      r1, sb
00419d54: ldr      r0, [sp, #0x1c]
00419d58: bl       #0x427ca0
00419d5c: mov      r0, r5
00419d60: ldr      fp, [r4, #0x658]
00419d64: bl       #0x427d50
00419d68: add      r2, r4, #0x470
00419d6c: ldr      r1, [pc, #0x9b0]
00419d70: add      r2, r2, #8
00419d74: str      r2, [sp, #0x18]
00419d78: mov      r3, r0
00419d7c: mov      r2, fp
00419d80: add      r1, pc, r1
00419d84: ldr      r0, [sp, #0x18]
00419d88: bl       #0x427ca0
00419d8c: mov      r0, r5
00419d90: ldr      fp, [r4, #0x658]
00419d94: bl       #0x427d50
00419d98: add      r2, r4, #0x4a0
00419d9c: ldr      r1, [pc, #0x984]
00419da0: add      r2, r2, #8
00419da4: str      r2, [sp, #4]
00419da8: mov      r3, r0
00419dac: mov      r2, fp
00419db0: add      r1, pc, r1
00419db4: ldr      r0, [sp, #4]
00419db8: bl       #0x427ca0
00419dbc: mov      r0, r5
00419dc0: ldr      fp, [r4, #0x658]
00419dc4: bl       #0x427d50
00419dc8: add      r2, r4, #0x4d0
00419dcc: ldr      r1, [pc, #0x958]
00419dd0: add      r2, r2, #8
00419dd4: str      r2, [sp, #0x14]
00419dd8: mov      r3, r0
00419ddc: mov      r2, fp
00419de0: add      r1, pc, r1
00419de4: ldr      r0, [sp, #0x14]
00419de8: bl       #0x427ca0
00419dec: add      r3, r4, #0x500
00419df0: add      r3, r3, #8
00419df4: mov      r0, r5
00419df8: ldr      fp, [r4, #0x658]
00419dfc: str      r3, [sp, #0x10]
00419e00: bl       #0x427d50
00419e04: mov      r1, r7
00419e08: mov      r3, r0
00419e0c: mov      r2, fp
00419e10: ldr      r0, [sp, #0x10]
00419e14: bl       #0x427ca0
00419e18: mov      r0, r5
00419e1c: ldr      r7, [r4, #0x658]
00419e20: bl       #0x427d50
00419e24: ldr      r1, [pc, #0x904]
00419e28: mov      r3, r0
00419e2c: add      r0, r4, #0x530
00419e30: mov      r2, r7
00419e34: add      r1, pc, r1
00419e38: add      r0, r0, #8
00419e3c: bl       #0x427ca0
00419e40: mov      r0, r5
00419e44: ldr      r7, [r4, #0x658]
00419e48: bl       #0x427d50
00419e4c: ldr      r1, [pc, #0x8e0]
00419e50: add      fp, r4, #0x560
00419e54: add      fp, fp, #8
00419e58: mov      r3, r0
00419e5c: mov      r2, r7
00419e60: add      r1, pc, r1
00419e64: mov      r0, fp
00419e68: bl       #0x427ca0
00419e6c: ldr      r2, [sp, #0xc]
00419e70: cmp      r2, #1
00419e74: ble      #0x41a2e4
00419e78: mov      r0, r5
00419e7c: ldr      r8, [r4, #0x658]
00419e80: bl       #0x427d50
00419e84: ldr      r7, [pc, #0x8ac]
00419e88: mov      r3, r0
00419e8c: mov      r2, r8
00419e90: add      r7, pc, r7
00419e94: mov      r1, r7
00419e98: add      r0, r4, #0x118
00419e9c: bl       #0x427ca0
00419ea0: mov      r0, r5
00419ea4: ldr      r8, [r4, #0x658]
00419ea8: bl       #0x427d50
00419eac: mov      r2, r8
00419eb0: mov      r3, r0
00419eb4: mov      r1, r7
00419eb8: add      r0, r4, #0x148
00419ebc: bl       #0x427ca0
00419ec0: mov      r0, r5
00419ec4: ldr      r8, [r4, #0x658]
00419ec8: bl       #0x427d50
00419ecc: mov      r1, r7
00419ed0: mov      r3, r0
00419ed4: mov      r2, r8
00419ed8: add      r0, r4, #0x178
00419edc: bl       #0x427ca0
00419ee0: mov      r0, r5
00419ee4: ldr      r7, [r4, #0x658]
00419ee8: bl       #0x427d50
00419eec: ldr      r1, [pc, #0x848]
00419ef0: mov      r3, r0
00419ef4: mov      r2, r7
00419ef8: add      r1, pc, r1
00419efc: add      r0, r4, #0x1a8
00419f00: bl       #0x427ca0
00419f04: mov      r0, r5
00419f08: ldr      r7, [r4, #0x658]
00419f0c: bl       #0x427d50
00419f10: ldr      r1, [pc, #0x828]
00419f14: mov      r3, r0
00419f18: mov      r2, r7
00419f1c: add      r1, pc, r1
00419f20: add      r0, r4, #0x1d8
00419f24: bl       #0x427ca0
00419f28: mov      r0, r5
00419f2c: ldr      r7, [r4, #0x658]
00419f30: bl       #0x427d50
00419f34: ldr      r1, [pc, #0x808]
00419f38: mov      r3, r0
00419f3c: mov      r2, r7
00419f40: add      r1, pc, r1
00419f44: add      r0, r4, #0x208
00419f48: bl       #0x427ca0
00419f4c: mov      r0, r5
00419f50: ldr      r7, [r4, #0x658]
00419f54: bl       #0x427d50
00419f58: ldr      r1, [pc, #0x7e8]
00419f5c: mov      r3, r0
00419f60: mov      r2, r7
00419f64: add      r1, pc, r1
00419f68: add      r0, r4, #0x238
00419f6c: bl       #0x427ca0
00419f70: mov      r0, r5
00419f74: ldr      r7, [r4, #0x658]
00419f78: bl       #0x427d50
00419f7c: ldr      r1, [pc, #0x7c8]
00419f80: mov      r3, r0
00419f84: mov      r2, r7
00419f88: add      r1, pc, r1
00419f8c: add      r0, r4, #0x268
00419f90: bl       #0x427ca0
00419f94: mov      r0, r5
00419f98: ldr      r7, [r4, #0x658]
00419f9c: bl       #0x427d50
00419fa0: ldr      r1, [pc, #0x7a8]
00419fa4: mov      r3, r0
00419fa8: mov      r2, r7
00419fac: add      r1, pc, r1
00419fb0: add      r0, r4, #0x298
00419fb4: bl       #0x427ca0
00419fb8: mov      r0, r5
00419fbc: ldr      r7, [r4, #0x658]
00419fc0: bl       #0x427d50
00419fc4: ldr      r1, [pc, #0x788]
00419fc8: mov      r3, r0
00419fcc: mov      r2, r7
00419fd0: add      r1, pc, r1
00419fd4: add      r0, r4, #0x2c8
00419fd8: bl       #0x427ca0
00419fdc: mov      r0, r5
00419fe0: ldr      r7, [r4, #0x658]
00419fe4: bl       #0x427d50
00419fe8: ldr      r1, [pc, #0x768]
00419fec: mov      r3, r0
00419ff0: mov      r2, r7
00419ff4: add      r1, pc, r1
00419ff8: add      r0, r4, #0x2f8
00419ffc: bl       #0x427ca0
0041a000: mov      r0, r5
0041a004: ldr      r7, [r4, #0x658]
0041a008: bl       #0x427d50
0041a00c: ldr      r1, [pc, #0x748]
0041a010: mov      r3, r0
0041a014: mov      r2, r7
0041a018: add      r1, pc, r1
0041a01c: add      r0, r4, #0x328
0041a020: bl       #0x427ca0
0041a024: ldr      r1, [pc, #0x734]
0041a028: mov      r3, #0
0041a02c: ldr      r2, [r4, #0x658]
0041a030: add      r1, pc, r1
0041a034: add      r0, r4, #0x358
0041a038: bl       #0x427ca0
0041a03c: ldr      r0, [sp, #0x24]
0041a040: ldr      r7, [r4, #0x658]
0041a044: bl       #0x427d50
0041a048: ldr      r1, [pc, #0x714]
0041a04c: mov      r2, r0
0041a050: mov      r0, r7
0041a054: add      r1, pc, r1
0041a058: bl       #0x7a8a84
0041a05c: ldr      r3, [r0]
0041a060: mov      lr, pc
0041a064: ldr      pc, [r3, #0x128]
0041a068: mov      r1, #0x41000000
0041a06c: add      r1, r1, #0xa00000
0041a070: bl       #0x30ec94
0041a074: mov      r1, #0x3f000000
0041a078: bl       #0x30ed6c
0041a07c: bl       #0x30e4cc
0041a080: str      r0, [r4, #0xc]
0041a084: str      r0, [r4, #0x10]
0041a088: ldr      r0, [sp, #0x28]
0041a08c: ldr      r7, [r4, #0x658]
0041a090: bl       #0x427d50
0041a094: mov      r2, #0
0041a098: mov      r3, r2
0041a09c: mov      r1, r0
0041a0a0: mov      r0, r7
0041a0a4: bl       #0x7aa3f0
0041a0a8: ldr      r1, [pc, #0x6b8]
0041a0ac: ldr      r0, [r4, #0x658]
0041a0b0: add      r7, r4, #0x590
0041a0b4: add      r1, pc, r1
0041a0b8: bl       #0x7a9160
0041a0bc: ldr      r2, [r4, #0x658]
0041a0c0: mov      r1, r0
0041a0c4: mov      r3, #0
0041a0c8: mov      r0, r5
0041a0cc: bl       #0x427c44
0041a0d0: mov      r0, r5
0041a0d4: ldr      r8, [r4, #0x658]
0041a0d8: bl       #0x427d50
0041a0dc: ldr      r1, [pc, #0x688]
0041a0e0: add      r7, r7, #8
0041a0e4: mov      r3, r0
0041a0e8: mov      r2, r8
0041a0ec: add      r1, pc, r1
0041a0f0: mov      r0, r7
0041a0f4: bl       #0x427ca0
0041a0f8: mov      r0, r5
0041a0fc: ldr      sl, [r4, #0x658]
0041a100: bl       #0x427d50
0041a104: ldr      r1, [pc, #0x664]
0041a108: add      r8, r4, #0x5c0
0041a10c: add      r8, r8, #8
0041a110: mov      r3, r0
0041a114: mov      r2, sl
0041a118: add      r1, pc, r1
0041a11c: mov      r0, r8
0041a120: bl       #0x427ca0
0041a124: mov      r0, r5
0041a128: bl       #0x427d50
0041a12c: ldr      r1, [pc, #0x640]
0041a130: mov      sb, #1
0041a134: strb     sb, [r0, #0x9b]
0041a138: add      r1, pc, r1
0041a13c: ldr      r0, [r4, #0x658]
0041a140: bl       #0x7a9160
0041a144: mov      r3, #0
0041a148: mov      r1, r0
0041a14c: ldr      r2, [r4, #0x658]
0041a150: mov      r0, r5
0041a154: bl       #0x427c44
0041a158: ldr      r2, [r4, #0x658]
0041a15c: mov      r0, r5
0041a160: add      sl, r4, #0x5f0
0041a164: str      r2, [sp]
0041a168: bl       #0x427d50
0041a16c: ldr      r1, [pc, #0x604]
0041a170: add      sl, sl, #8
0041a174: mov      r3, r0
0041a178: add      r1, pc, r1
0041a17c: ldr      r2, [sp]
0041a180: mov      r0, sl
0041a184: bl       #0x427ca0
0041a188: ldr      r2, [r4, #0x658]
0041a18c: mov      r0, r5
0041a190: add      r5, r4, #0x620
0041a194: str      r2, [sp]
0041a198: bl       #0x427d50
0041a19c: ldr      r1, [pc, #0x5d8]
0041a1a0: add      r5, r5, #8
0041a1a4: mov      r3, r0
0041a1a8: add      r1, pc, r1
0041a1ac: ldr      r2, [sp]
0041a1b0: mov      r0, r5
0041a1b4: bl       #0x427ca0
0041a1b8: ldrb     r3, [r4, #0x66c]
0041a1bc: cmp      r3, #0
0041a1c0: bne      #0x41a250
0041a1c4: mov      r0, r7
0041a1c8: str      r3, [sp]
0041a1cc: bl       #0x427d50
0041a1d0: ldr      r3, [sp]
0041a1d4: strb     r3, [r0, #0x9b]
0041a1d8: mov      r0, r8
0041a1dc: str      r3, [sp]
0041a1e0: bl       #0x427d50
0041a1e4: ldr      r3, [sp]
0041a1e8: strb     r3, [r0, #0x9b]
0041a1ec: ldr      r0, [sp, #4]
0041a1f0: bl       #0x427d50
0041a1f4: strb     sb, [r0, #0x9b]
0041a1f8: mov      r0, fp
0041a1fc: bl       #0x427d50
0041a200: strb     sb, [r0, #0x9b]
0041a204: ldr      r3, [sp, #0xc]
0041a208: cmp      r3, #0
0041a20c: bne      #0x41a2a8
0041a210: ldrb     r3, [r4, #0x66d]
0041a214: cmp      r3, #0
0041a218: beq      #0x41a524
0041a21c: ldrb     r3, [r4, #0x66f]
0041a220: cmp      r3, #0
0041a224: bne      #0x41a50c
0041a228: mov      r3, #1
0041a22c: strb     r3, [r4, #8]
0041a230: ldr      r2, [sp, #8]
0041a234: ldr      r3, [r6, r2]
0041a238: ldr      r2, [sp, #0x4c]
0041a23c: ldr      r3, [r3]
0041a240: cmp      r2, r3
0041a244: bne      #0x41a6e8
0041a248: add      sp, sp, #0x54
0041a24c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0041a250: mov      r0, r7
0041a254: bl       #0x427d50
0041a258: strb     sb, [r0, #0x9b]
0041a25c: mov      r0, r8
0041a260: bl       #0x427d50
0041a264: strb     sb, [r0, #0x9b]
0041a268: ldr      r0, [sp, #4]
0041a26c: bl       #0x427d50
0041a270: mov      r7, #0
0041a274: strb     r7, [r0, #0x9b]
0041a278: mov      r0, fp
0041a27c: bl       #0x427d50
0041a280: strb     r7, [r0, #0x9b]
0041a284: mov      r0, sl
0041a288: bl       #0x427d50
0041a28c: strb     r7, [r0, #0x9b]
0041a290: mov      r0, r5
0041a294: bl       #0x427d50
0041a298: strb     r7, [r0, #0x9b]
0041a29c: ldr      r3, [sp, #0xc]
0041a2a0: cmp      r3, #0
0041a2a4: beq      #0x41a210
0041a2a8: ldr      r2, [sp, #0xc]
0041a2ac: cmp      r2, #1
0041a2b0: bne      #0x41a228
0041a2b4: ldrb     r3, [r4, #0x66e]
0041a2b8: cmp      r3, #0
0041a2bc: beq      #0x41a348
0041a2c0: ldrb     r3, [r4, #0x670]
0041a2c4: cmp      r3, #0
0041a2c8: beq      #0x41a228
0041a2cc: add      r1, r4, #0x770
0041a2d0: add      r1, r1, #0xc
0041a2d4: mov      r0, r4
0041a2d8: mov      r2, #0
0041a2dc: bl       #0x4187a0
0041a2e0: b        #0x41a228
0041a2e4: mov      r0, r5
0041a2e8: ldr      r7, [r4, #0x658]
0041a2ec: bl       #0x427d50
0041a2f0: mov      r1, r8
0041a2f4: mov      r3, r0
0041a2f8: mov      r2, r7
0041a2fc: add      r0, r4, #0x118
0041a300: bl       #0x427ca0
0041a304: mov      r0, r5
0041a308: ldr      r7, [r4, #0x658]
0041a30c: bl       #0x427d50
0041a310: mov      r1, sl
0041a314: mov      r3, r0
0041a318: mov      r2, r7
0041a31c: add      r0, r4, #0x148
0041a320: bl       #0x427ca0
0041a324: mov      r0, r5
0041a328: ldr      r7, [r4, #0x658]
0041a32c: bl       #0x427d50
0041a330: mov      r1, sb
0041a334: mov      r3, r0
0041a338: mov      r2, r7
0041a33c: add      r0, r4, #0x178
0041a340: bl       #0x427ca0
0041a344: b        #0x419ee0
0041a348: ldr      r0, [sp, #0x24]
0041a34c: bl       #0x427d50
0041a350: bl       #0x753f74
0041a354: ldr      r3, [r0, #8]
0041a358: ldr      r0, [sp, #0x24]
0041a35c: str      r3, [r4, #0x6cc]
0041a360: bl       #0x427d50
0041a364: bl       #0x753f74
0041a368: ldr      r3, [r0, #0x14]
0041a36c: ldr      r0, [sp, #0x34]
0041a370: str      r3, [r4, #0x6d0]
0041a374: bl       #0x427d50
0041a378: bl       #0x753f74
0041a37c: ldr      r3, [r0, #8]
0041a380: ldr      r0, [sp, #0x34]
0041a384: str      r3, [r4, #0x6d4]
0041a388: bl       #0x427d50
0041a38c: bl       #0x753f74
0041a390: ldr      r3, [r0, #0x14]
0041a394: ldr      r0, [sp, #0x30]
0041a398: str      r3, [r4, #0x6d8]
0041a39c: bl       #0x427d50
0041a3a0: bl       #0x753f74
0041a3a4: ldr      r3, [r0, #8]
0041a3a8: ldr      r0, [sp, #0x30]
0041a3ac: str      r3, [r4, #0x6dc]
0041a3b0: bl       #0x427d50
0041a3b4: bl       #0x753f74
0041a3b8: ldr      r3, [r0, #0x14]
0041a3bc: ldr      r0, [sp, #0x2c]
0041a3c0: str      r3, [r4, #0x6e0]
0041a3c4: bl       #0x427d50
0041a3c8: bl       #0x753f74
0041a3cc: ldr      r3, [r0, #8]
0041a3d0: ldr      r0, [sp, #0x2c]
0041a3d4: str      r3, [r4, #0x6e4]
0041a3d8: bl       #0x427d50
0041a3dc: bl       #0x753f74
0041a3e0: ldr      r3, [r0, #0x14]
0041a3e4: ldr      r0, [sp, #0x20]
0041a3e8: str      r3, [r4, #0x6e8]
0041a3ec: bl       #0x427d50
0041a3f0: bl       #0x753f74
0041a3f4: ldr      r3, [r0, #8]
0041a3f8: ldr      r0, [sp, #0x20]
0041a3fc: str      r3, [r4, #0x6ec]
0041a400: bl       #0x427d50
0041a404: bl       #0x753f74
0041a408: ldr      r3, [r0, #0x14]
0041a40c: ldr      r0, [sp, #0x1c]
0041a410: str      r3, [r4, #0x6f0]
0041a414: bl       #0x427d50
0041a418: bl       #0x753f74
0041a41c: ldr      r3, [r0, #8]
0041a420: ldr      r0, [sp, #0x1c]
0041a424: str      r3, [r4, #0x6f4]
0041a428: bl       #0x427d50
0041a42c: bl       #0x753f74
0041a430: ldr      r3, [r0, #0x14]
0041a434: ldr      r0, [sp, #0x18]
0041a438: str      r3, [r4, #0x6f8]
0041a43c: bl       #0x427d50
0041a440: bl       #0x753f74
0041a444: ldr      r3, [r0, #8]
0041a448: ldr      r0, [sp, #0x18]
0041a44c: str      r3, [r4, #0x6fc]
0041a450: bl       #0x427d50
0041a454: bl       #0x753f74
0041a458: ldr      r3, [r0, #0x14]
0041a45c: ldr      r0, [sp, #0x14]
0041a460: str      r3, [r4, #0x700]
0041a464: bl       #0x427d50
0041a468: bl       #0x753f74
0041a46c: ldr      r3, [r0, #8]
0041a470: ldr      r0, [sp, #0x14]
0041a474: str      r3, [r4, #0x704]
0041a478: bl       #0x427d50
0041a47c: bl       #0x753f74
0041a480: ldr      r3, [r0, #0x14]
0041a484: ldr      r0, [sp, #4]
0041a488: str      r3, [r4, #0x708]
0041a48c: bl       #0x427d50
0041a490: bl       #0x753f74
0041a494: ldr      r3, [r0, #8]
0041a498: ldr      r0, [sp, #4]
0041a49c: str      r3, [r4, #0x70c]
0041a4a0: bl       #0x427d50
0041a4a4: bl       #0x753f74
0041a4a8: ldr      r3, [r0, #0x14]
0041a4ac: ldr      r0, [sp, #0x10]
0041a4b0: str      r3, [r4, #0x710]
0041a4b4: bl       #0x427d50
0041a4b8: bl       #0x753f74
0041a4bc: ldr      r3, [r0, #8]
0041a4c0: ldr      r0, [sp, #0x10]
0041a4c4: str      r3, [r4, #0x714]
0041a4c8: bl       #0x427d50
0041a4cc: bl       #0x753f74
0041a4d0: ldr      r3, [r0, #0x14]
0041a4d4: mov      r0, fp
0041a4d8: str      r3, [r4, #0x718]
0041a4dc: bl       #0x427d50
0041a4e0: bl       #0x753f74
0041a4e4: ldr      r3, [r0, #8]
0041a4e8: mov      r0, fp
0041a4ec: str      r3, [r4, #0x71c]
0041a4f0: bl       #0x427d50
0041a4f4: bl       #0x753f74
0041a4f8: ldr      r2, [sp, #0xc]
0041a4fc: ldr      r3, [r0, #0x14]
0041a500: strb     r2, [r4, #0x66e]
0041a504: str      r3, [r4, #0x720]
0041a508: b        #0x41a2c0
0041a50c: add      r1, r4, #0x720
0041a510: add      r1, r1, #4
0041a514: mov      r0, r4
0041a518: mov      r2, #0
0041a51c: bl       #0x4187a0
0041a520: b        #0x41a228
0041a524: ldr      r0, [sp, #0x24]
0041a528: bl       #0x427d50
0041a52c: bl       #0x753f74
0041a530: ldr      r3, [r0, #8]
0041a534: ldr      r0, [sp, #0x24]
0041a538: str      r3, [r4, #0x674]
0041a53c: bl       #0x427d50
0041a540: bl       #0x753f74
0041a544: ldr      r3, [r0, #0x14]
0041a548: ldr      r0, [sp, #0x34]
0041a54c: str      r3, [r4, #0x678]
0041a550: bl       #0x427d50
0041a554: bl       #0x753f74
0041a558: ldr      r3, [r0, #8]
0041a55c: ldr      r0, [sp, #0x34]
0041a560: str      r3, [r4, #0x67c]
0041a564: bl       #0x427d50
0041a568: bl       #0x753f74
0041a56c: ldr      r3, [r0, #0x14]
0041a570: ldr      r0, [sp, #0x30]
0041a574: str      r3, [r4, #0x680]
0041a578: bl       #0x427d50
0041a57c: bl       #0x753f74
0041a580: ldr      r3, [r0, #8]
0041a584: ldr      r0, [sp, #0x30]
0041a588: str      r3, [r4, #0x684]
0041a58c: bl       #0x427d50
0041a590: bl       #0x753f74
0041a594: ldr      r3, [r0, #0x14]
0041a598: ldr      r0, [sp, #0x2c]
0041a59c: str      r3, [r4, #0x688]
0041a5a0: bl       #0x427d50
0041a5a4: bl       #0x753f74
0041a5a8: ldr      r3, [r0, #8]
0041a5ac: ldr      r0, [sp, #0x2c]
0041a5b0: str      r3, [r4, #0x68c]
0041a5b4: bl       #0x427d50
0041a5b8: bl       #0x753f74
0041a5bc: ldr      r3, [r0, #0x14]
0041a5c0: ldr      r0, [sp, #0x20]
0041a5c4: str      r3, [r4, #0x690]
0041a5c8: bl       #0x427d50
0041a5cc: bl       #0x753f74
0041a5d0: ldr      r3, [r0, #8]
0041a5d4: ldr      r0, [sp, #0x20]
0041a5d8: str      r3, [r4, #0x694]
0041a5dc: bl       #0x427d50
0041a5e0: bl       #0x753f74
0041a5e4: ldr      r3, [r0, #0x14]
0041a5e8: ldr      r0, [sp, #0x1c]
0041a5ec: str      r3, [r4, #0x698]
0041a5f0: bl       #0x427d50
0041a5f4: bl       #0x753f74
0041a5f8: ldr      r3, [r0, #8]
0041a5fc: ldr      r0, [sp, #0x1c]
0041a600: str      r3, [r4, #0x69c]
0041a604: bl       #0x427d50
0041a608: bl       #0x753f74
0041a60c: ldr      r3, [r0, #0x14]
0041a610: ldr      r0, [sp, #0x18]
0041a614: str      r3, [r4, #0x6a0]
0041a618: bl       #0x427d50
0041a61c: bl       #0x753f74
0041a620: ldr      r3, [r0, #8]
0041a624: ldr      r0, [sp, #0x18]
0041a628: str      r3, [r4, #0x6a4]
0041a62c: bl       #0x427d50
0041a630: bl       #0x753f74
0041a634: ldr      r3, [r0, #0x14]
0041a638: ldr      r0, [sp, #0x14]
0041a63c: str      r3, [r4, #0x6a8]
0041a640: bl       #0x427d50
0041a644: bl       #0x753f74
0041a648: ldr      r3, [r0, #8]
0041a64c: ldr      r0, [sp, #0x14]
0041a650: str      r3, [r4, #0x6ac]
0041a654: bl       #0x427d50
0041a658: bl       #0x753f74
0041a65c: ldr      r3, [r0, #0x14]
0041a660: ldr      r0, [sp, #4]
0041a664: str      r3, [r4, #0x6b0]
0041a668: bl       #0x427d50
0041a66c: bl       #0x753f74
0041a670: ldr      r3, [r0, #8]
0041a674: ldr      r0, [sp, #4]
0041a678: str      r3, [r4, #0x6b4]
0041a67c: bl       #0x427d50
0041a680: bl       #0x753f74
0041a684: ldr      r3, [r0, #0x14]
0041a688: ldr      r0, [sp, #0x10]
0041a68c: str      r3, [r4, #0x6b8]
0041a690: bl       #0x427d50
0041a694: bl       #0x753f74
0041a698: ldr      r3, [r0, #8]
0041a69c: ldr      r0, [sp, #0x10]
0041a6a0: str      r3, [r4, #0x6bc]
0041a6a4: bl       #0x427d50
0041a6a8: bl       #0x753f74
0041a6ac: ldr      r3, [r0, #0x14]
0041a6b0: mov      r0, fp
0041a6b4: str      r3, [r4, #0x6c0]
0041a6b8: bl       #0x427d50
0041a6bc: bl       #0x753f74
0041a6c0: ldr      r3, [r0, #8]
0041a6c4: mov      r0, fp
0041a6c8: str      r3, [r4, #0x6c4]
0041a6cc: bl       #0x427d50
0041a6d0: bl       #0x753f74
0041a6d4: ldr      r3, [r0, #0x14]
0041a6d8: mov      r2, #1
0041a6dc: strb     r2, [r4, #0x66d]
0041a6e0: str      r3, [r4, #0x6c8]
0041a6e4: b        #0x41a21c
0041a6e8: bl       #0x30e310
0041a6ec: subseq   sl, r7, r4, lsr pc
0041a6f0: andeq    r4, r0, ip, lsr #1
0041a6f4: strdeq   r3, r4, [r0], -r4
0041a6f8: subeq    r7, sl, r8, ror #20
0041a6fc: subeq    r7, sl, ip, lsr #9
0041a700: subeq    lr, sl, r4, ror r8
0041a704: subeq    lr, sl, r8, ror r8
0041a708: subeq    lr, sl, ip, ror r8
0041a70c: subeq    lr, sl, ip, lsl #17
0041a710: subeq    lr, sl, r0, ror r8
0041a714: subeq    lr, sl, r8, ror #16
0041a718: subeq    lr, sl, r4, ror #16
0041a71c: subeq    lr, sl, r0, ror #16
0041a720: subeq    lr, sl, ip, asr r8
0041a724: subeq    lr, sl, r8, asr r8
0041a728: subeq    lr, sl, r8, asr #16
0041a72c: subeq    lr, sl, r8, lsr r8
0041a730: subeq    lr, sl, r4, lsl #16
0041a734: subeq    lr, sl, r0, lsl #16
0041a738: strdeq   lr, pc, [sl], #-0x70
0041a73c: subeq    lr, sl, r8, lsr #12
0041a740: strheq   lr, [sl], #-0x6c
0041a744: strheq   lr, [sl], #-0x68
0041a748: ldrdeq   lr, pc, [sl], #-0x64
0041a74c: subeq    lr, sl, r8, lsr #14
0041a750: subeq    lr, sl, r4, lsr r7
0041a754: subeq    lr, sl, r8, asr #14
0041a758: subeq    lr, sl, r4, asr r7
0041a75c: subeq    lr, sl, r8, ror #14
0041a760: subeq    lr, sl, r8, lsl #15
0041a764: subeq    lr, sl, r4, lsl #15
0041a768: subeq    lr, sl, ip, lsr #14
0041a76c: subeq    lr, sl, ip, lsl #14
0041a770: strdeq   lr, pc, [sl], #-0x68
0041a774: strdeq   lr, pc, [sl], #-0x60
0041a778: subeq    lr, sl, r8, asr #13
0041a77c: subeq    lr, sl, r8, lsr #13
