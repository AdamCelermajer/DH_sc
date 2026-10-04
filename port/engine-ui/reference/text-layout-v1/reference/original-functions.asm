
# _ZN7gameswf19edit_text_character11format_textEb
0078efb8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0078efbc: mvn      r2, #0
0078efc0: mvn      r3, #0
0078efc4: mov      r4, r0
0078efc8: strd     r2, r3, [r0, #0xe0]
0078efcc: strd     r2, r3, [r0, #0xd8]
0078efd0: sub      sp, sp, #0x10
0078efd4: add      r0, r0, #0xa4
0078efd8: mov      r6, r1
0078efdc: mov      r1, #0
0078efe0: bl       #0x78bccc
0078efe4: mov      r5, #0
0078efe8: mov      r3, #0
0078efec: mvn      r2, #0
0078eff0: mov      r1, r3
0078eff4: str      r2, [r4, #0x16c]
0078eff8: str      r3, [r4, #0x15c]
0078effc: str      r3, [r4, #0x160]
0078f000: str      r5, [r4, #0x164]
0078f004: str      r5, [r4, #0x168]
0078f008: mov      r0, r4
0078f00c: mov      r2, r3
0078f010: bl       #0x78a370
0078f014: ldr      r1, [r4, #0x178]
0078f018: cmp      r1, r5
0078f01c: beq      #0x78f194
0078f020: cmp      r6, r5
0078f024: beq      #0x78f19c
0078f028: mov      r0, sp
0078f02c: mov      r1, r4
0078f030: str      r5, [sp]
0078f034: str      r5, [sp, #4]
0078f038: str      r5, [sp, #8]
0078f03c: strb     r5, [sp, #0xc]
0078f040: bl       #0x78e354
0078f044: mov      r0, sp
0078f048: mov      r1, r5
0078f04c: bl       #0x78bc0c
0078f050: mov      r0, sp
0078f054: mov      r1, r5
0078f058: mov      r6, sp
0078f05c: bl       #0x78a67c
0078f060: ldr      r3, [r4, #0x15c]
0078f064: mov      r0, r4
0078f068: ldr      r1, [r4, #0x17c]
0078f06c: ldr      r2, [r4, #0x164]
0078f070: bl       #0x78a398
0078f074: ldr      r3, [r4, #0xa0]
0078f078: ldrb     r5, [r3, #0x49]
0078f07c: cmp      r5, #0
0078f080: bne      #0x78f178
0078f084: ldr      r8, [r4, #0xa8]
0078f088: cmp      r8, #1
0078f08c: ble      #0x78f178
0078f090: ldr      r6, [r4, #0xa4]
0078f094: mov      r7, r5
0078f098: mov      sb, #0
0078f09c: add      r3, r6, r5
0078f0a0: ldrb     r2, [r3, #0x1d]
0078f0a4: mov      r1, sb
0078f0a8: add      r7, r7, #1
0078f0ac: cmp      r2, #0
0078f0b0: add      r5, r5, #0x30
0078f0b4: beq      #0x78f0f4
0078f0b8: ldr      sl, [r3, #0x14]
0078f0bc: mov      r0, sl
0078f0c0: bl       #0x30e2f8
0078f0c4: cmp      r0, #0
0078f0c8: beq      #0x78f0f4
0078f0cc: cmp      r7, r8
0078f0d0: beq      #0x78f100
0078f0d4: add      r3, r6, r5
0078f0d8: ldrb     r2, [r3, #0x1d]
0078f0dc: mov      sb, sl
0078f0e0: mov      r1, sb
0078f0e4: cmp      r2, #0
0078f0e8: add      r7, r7, #1
0078f0ec: add      r5, r5, #0x30
0078f0f0: bne      #0x78f0b8
0078f0f4: cmp      r7, r8
0078f0f8: mov      sl, sb
0078f0fc: bne      #0x78f0d4
0078f100: mov      r0, sl
0078f104: mov      r1, #0xbf000000
0078f108: bl       #0x30ed6c
0078f10c: mov      r1, #0xbf000000
0078f110: mov      r5, r0
0078f114: ldr      r0, [r6, #0x18]
0078f118: bl       #0x30ed6c
0078f11c: ldr      r1, [r6, #0x14]
0078f120: bl       #0x30eba4
0078f124: mov      r1, r0
0078f128: mov      r0, r5
0078f12c: bl       #0x30eba4
0078f130: mov      r5, #0
0078f134: mov      sl, r0
0078f138: mov      r7, r5
0078f13c: b        #0x78f144
0078f140: ldr      r6, [r4, #0xa4]
0078f144: add      r6, r6, r5
0078f148: ldrb     r3, [r6, #0x1d]
0078f14c: mov      r1, sl
0078f150: add      r7, r7, #1
0078f154: cmp      r3, #0
0078f158: add      r5, r5, #0x30
0078f15c: beq      #0x78f170
0078f160: ldr      r0, [r6, #0x14]
0078f164: bl       #0x30eba4
0078f168: str      r0, [r6, #0x14]
0078f16c: ldr      r8, [r4, #0xa8]
0078f170: cmp      r7, r8
0078f174: blt      #0x78f140
0078f178: mov      r0, r4
0078f17c: bl       #0x78dde8
0078f180: ldrb     r3, [r0, #0x87]
0078f184: cmp      r3, #0
0078f188: beq      #0x78f194
0078f18c: mov      r0, r4
0078f190: bl       #0x78c2d0
0078f194: add      sp, sp, #0x10
0078f198: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0078f19c: ldr      r3, [r4, #0x170]
0078f1a0: mov      r2, #0xc
0078f1a4: mov      r0, sp
0078f1a8: stmib    sp, {r2, r3}
0078f1ac: str      r6, [sp]
0078f1b0: strb     r6, [sp, #0xc]
0078f1b4: bl       #0x764234
0078f1b8: ldr      r0, [r4, #0x174]
0078f1bc: bl       #0x30e4cc
0078f1c0: mov      r2, sp
0078f1c4: str      r0, [sp, #4]
0078f1c8: mov      r3, r6
0078f1cc: mov      r0, r4
0078f1d0: add      r1, r4, #0x138
0078f1d4: bl       #0x78cb90
0078f1d8: ldr      r0, [sp]
0078f1dc: cmp      r0, #0
0078f1e0: beq      #0x78f060
0078f1e4: bl       #0x75a240
0078f1e8: b        #0x78f060

# _ZN7gameswf19edit_text_character12reset_formatEPNS_13as_textformatE
0078f288: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078f28c: ldr      r7, [pc, #0x718]
0078f290: ldr      r8, [pc, #0x718]
0078f294: ldr      r3, [r1]
0078f298: add      r7, pc, r7
0078f29c: ldr      r2, [r7, r8]
0078f2a0: mov      r4, r1
0078f2a4: sub      sp, sp, #0x10c
0078f2a8: ldr      r1, [r2]
0078f2ac: add      sb, sp, #0xf0
0078f2b0: mov      r2, #0
0078f2b4: str      r1, [sp, #0x104]
0078f2b8: ldr      r1, [pc, #0x6f4]
0078f2bc: strb     r2, [sp, #9]
0078f2c0: strb     r2, [sp, #8]
0078f2c4: add      r1, pc, r1
0078f2c8: mov      r6, r0
0078f2cc: add      r5, sp, #8
0078f2d0: mov      r0, sb
0078f2d4: ldr      sl, [r3, #0x20]
0078f2d8: bl       #0x413a7c
0078f2dc: mov      r1, sb
0078f2e0: mov      r0, r4
0078f2e4: mov      r2, r5
0078f2e8: blx      sl
0078f2ec: ldrsb    r3, [sp, #0xf0]
0078f2f0: mov      sl, r0
0078f2f4: cmn      r3, #1
0078f2f8: beq      #0x78f8e8
0078f2fc: cmp      sl, #0
0078f300: bne      #0x78f6b0
0078f304: ldr      r1, [pc, #0x6ac]
0078f308: ldr      r3, [r4]
0078f30c: add      sb, sp, #0xdc
0078f310: add      r1, pc, r1
0078f314: mov      r0, sb
0078f318: ldr      sl, [r3, #0x20]
0078f31c: bl       #0x413a7c
0078f320: mov      r0, r4
0078f324: mov      r1, sb
0078f328: mov      r2, r5
0078f32c: blx      sl
0078f330: ldrsb    r3, [sp, #0xdc]
0078f334: mov      sl, r0
0078f338: cmn      r3, #1
0078f33c: beq      #0x78f928
0078f340: cmp      sl, #0
0078f344: bne      #0x78f6f0
0078f348: ldr      r1, [pc, #0x66c]
0078f34c: ldr      r3, [r4]
0078f350: add      sb, sp, #0xc8
0078f354: add      r1, pc, r1
0078f358: mov      r0, sb
0078f35c: ldr      sl, [r3, #0x20]
0078f360: bl       #0x413a7c
0078f364: mov      r0, r4
0078f368: mov      r1, sb
0078f36c: mov      r2, r5
0078f370: blx      sl
0078f374: ldrsb    r3, [sp, #0xc8]
0078f378: mov      sl, r0
0078f37c: cmn      r3, #1
0078f380: beq      #0x78f8f8
0078f384: cmp      sl, #0
0078f388: bne      #0x78f6d0
0078f38c: ldr      r1, [pc, #0x62c]
0078f390: ldr      r3, [r4]
0078f394: add      sb, sp, #0xb4
0078f398: add      r1, pc, r1
0078f39c: mov      r0, sb
0078f3a0: ldr      sl, [r3, #0x20]
0078f3a4: bl       #0x413a7c
0078f3a8: mov      r0, r4
0078f3ac: mov      r1, sb
0078f3b0: mov      r2, r5
0078f3b4: blx      sl
0078f3b8: ldrsb    r3, [sp, #0xb4]
0078f3bc: mov      sl, r0
0078f3c0: cmn      r3, #1
0078f3c4: beq      #0x78f908
0078f3c8: cmp      sl, #0
0078f3cc: bne      #0x78f89c
0078f3d0: ldr      r1, [pc, #0x5ec]
0078f3d4: ldr      r3, [r4]
0078f3d8: add      sb, sp, #0xa0
0078f3dc: add      r1, pc, r1
0078f3e0: mov      r0, sb
0078f3e4: ldr      sl, [r3, #0x20]
0078f3e8: bl       #0x413a7c
0078f3ec: mov      r0, r4
0078f3f0: mov      r1, sb
0078f3f4: mov      r2, r5
0078f3f8: blx      sl
0078f3fc: ldrsb    r3, [sp, #0xa0]
0078f400: mov      sl, r0
0078f404: cmn      r3, #1
0078f408: beq      #0x78f978
0078f40c: cmp      sl, #0
0078f410: bne      #0x78f87c
0078f414: ldr      r1, [pc, #0x5ac]
0078f418: ldr      r3, [r4]
0078f41c: add      sb, sp, #0x8c
0078f420: add      r1, pc, r1
0078f424: mov      r0, sb
0078f428: ldr      sl, [r3, #0x20]
0078f42c: bl       #0x413a7c
0078f430: mov      r0, r4
0078f434: mov      r1, sb
0078f438: mov      r2, r5
0078f43c: blx      sl
0078f440: ldrsb    r3, [sp, #0x8c]
0078f444: mov      sl, r0
0078f448: cmn      r3, #1
0078f44c: beq      #0x78f988
0078f450: cmp      sl, #0
0078f454: bne      #0x78f850
0078f458: ldr      r1, [pc, #0x56c]
0078f45c: ldr      r3, [r4]
0078f460: add      sb, sp, #0x78
0078f464: add      r1, pc, r1
0078f468: mov      r0, sb
0078f46c: ldr      sl, [r3, #0x20]
0078f470: bl       #0x413a7c
0078f474: mov      r0, r4
0078f478: mov      r1, sb
0078f47c: mov      r2, r5
0078f480: blx      sl
0078f484: ldrsb    r3, [sp, #0x78]
0078f488: mov      sl, r0
0078f48c: cmn      r3, #1
0078f490: beq      #0x78f918
0078f494: cmp      sl, #0
0078f498: bne      #0x78f830
0078f49c: ldr      r1, [pc, #0x52c]
0078f4a0: ldr      r3, [r4]
0078f4a4: add      sb, sp, #0x64
0078f4a8: add      r1, pc, r1
0078f4ac: mov      r0, sb
0078f4b0: ldr      sl, [r3, #0x20]
0078f4b4: bl       #0x413a7c
0078f4b8: mov      r0, r4
0078f4bc: mov      r1, sb
0078f4c0: mov      r2, r5
0078f4c4: blx      sl
0078f4c8: ldrsb    r3, [sp, #0x64]
0078f4cc: mov      sl, r0
0078f4d0: cmn      r3, #1
0078f4d4: beq      #0x78f968
0078f4d8: cmp      sl, #0
0078f4dc: bne      #0x78f794
0078f4e0: ldr      r1, [r6, #0x178]
0078f4e4: add      sb, sp, #0x50
0078f4e8: mov      r0, sb
0078f4ec: add      r1, r1, #0x30
0078f4f0: bl       #0x75302c
0078f4f4: ldr      r1, [pc, #0x4d8]
0078f4f8: ldr      r3, [r4]
0078f4fc: add      fp, sp, #0x3c
0078f500: add      r1, pc, r1
0078f504: mov      r0, fp
0078f508: ldr      sl, [r3, #0x20]
0078f50c: bl       #0x413a7c
0078f510: mov      r0, r4
0078f514: mov      r1, fp
0078f518: mov      r2, r5
0078f51c: blx      sl
0078f520: ldrsb    r3, [sp, #0x3c]
0078f524: mov      sl, r0
0078f528: cmn      r3, #1
0078f52c: beq      #0x78f998
0078f530: cmp      sl, #0
0078f534: bne      #0x78f77c
0078f538: ldr      r3, [r6, #0x178]
0078f53c: ldr      r1, [pc, #0x494]
0078f540: ldr      r2, [r4]
0078f544: ldrb     r3, [r3, #0x4d]
0078f548: add      fp, sp, #0x28
0078f54c: add      r1, pc, r1
0078f550: mov      r0, fp
0078f554: ldr      sl, [r2, #0x20]
0078f558: str      r3, [sp, #4]
0078f55c: bl       #0x413a7c
0078f560: mov      r0, r4
0078f564: mov      r1, fp
0078f568: mov      r2, r5
0078f56c: blx      sl
0078f570: ldrsb    r3, [sp, #0x28]
0078f574: mov      sl, r0
0078f578: cmn      r3, #1
0078f57c: beq      #0x78f948
0078f580: cmp      sl, #0
0078f584: bne      #0x78f76c
0078f588: ldr      r3, [r4]
0078f58c: ldr      r1, [pc, #0x448]
0078f590: ldr      r2, [r6, #0x178]
0078f594: ldr      r3, [r3, #0x20]
0078f598: add      fp, sp, #0x14
0078f59c: add      r1, pc, r1
0078f5a0: mov      r0, fp
0078f5a4: ldrb     sl, [r2, #0x4c]
0078f5a8: str      r3, [sp]
0078f5ac: bl       #0x413a7c
0078f5b0: mov      r0, r4
0078f5b4: ldr      r3, [sp]
0078f5b8: mov      r1, fp
0078f5bc: mov      r2, r5
0078f5c0: blx      r3
0078f5c4: ldrsb    r3, [sp, #0x14]
0078f5c8: mov      r4, r0
0078f5cc: cmn      r3, #1
0078f5d0: beq      #0x78f958
0078f5d4: cmp      r4, #0
0078f5d8: bne      #0x78f710
0078f5dc: ldr      r1, [r6, #0x178]
0078f5e0: ldrb     r3, [r1, #0x4c]
0078f5e4: cmp      r3, sl
0078f5e8: beq      #0x78f72c
0078f5ec: ldr      r3, [r6]
0078f5f0: mov      r0, r6
0078f5f4: mov      r1, sb
0078f5f8: mov      lr, pc
0078f5fc: ldr      pc, [r3, #0x84]
0078f600: subs     r4, r0, #0
0078f604: beq      #0x78f620
0078f608: ldr      r3, [r4]
0078f60c: mov      r1, #0x13
0078f610: mov      lr, pc
0078f614: ldr      pc, [r3, #8]
0078f618: cmp      r0, #0
0078f61c: bne      #0x78f8bc
0078f620: mov      r0, r6
0078f624: bl       #0x780374
0078f628: mov      r1, #0
0078f62c: mov      r4, r0
0078f630: mov      r0, #0x88
0078f634: bl       #0x752ba8
0078f638: mov      r1, r4
0078f63c: mov      fp, r0
0078f640: bl       #0x7cf8d8
0078f644: mov      r1, fp
0078f648: add      r0, r6, #0x178
0078f64c: bl       #0x764234
0078f650: ldr      r3, [r6, #0x178]
0078f654: ldr      r2, [sp, #4]
0078f658: mov      r1, sb
0078f65c: strb     r2, [r3, #0x4d]
0078f660: ldr      r3, [r6, #0x178]
0078f664: strb     sl, [r3, #0x4c]
0078f668: ldr      r0, [r6, #0x178]
0078f66c: add      r0, r0, #0x30
0078f670: bl       #0x752f50
0078f674: mov      r0, r6
0078f678: mov      r1, #0
0078f67c: bl       #0x78efb8
0078f680: ldrsb    r3, [sp, #0x50]
0078f684: cmn      r3, #1
0078f688: beq      #0x78f938
0078f68c: mov      r0, r5
0078f690: bl       #0x797124
0078f694: ldr      r3, [r7, r8]
0078f698: ldr      r2, [sp, #0x104]
0078f69c: ldr      r3, [r3]
0078f6a0: cmp      r2, r3
0078f6a4: bne      #0x78f9a8
0078f6a8: add      sp, sp, #0x10c
0078f6ac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078f6b0: mov      r0, r5
0078f6b4: bl       #0x797a54
0078f6b8: bl       #0x30e6a0
0078f6bc: mov      r1, #0x41000000
0078f6c0: add      r1, r1, #0xa00000
0078f6c4: bl       #0x30ed6c
0078f6c8: str      r0, [r6, #0x180]
0078f6cc: b        #0x78f304
0078f6d0: mov      r0, r5
0078f6d4: bl       #0x797a54
0078f6d8: bl       #0x30e6a0
0078f6dc: mov      r1, #0x41000000
0078f6e0: add      r1, r1, #0xa00000
0078f6e4: bl       #0x30ed6c
0078f6e8: str      r0, [r6, #0x184]
0078f6ec: b        #0x78f38c
0078f6f0: mov      r0, r5
0078f6f4: bl       #0x797a54
0078f6f8: bl       #0x30e6a0
0078f6fc: mov      r1, #0x41000000
0078f700: add      r1, r1, #0xa00000
0078f704: bl       #0x30ed6c
0078f708: str      r0, [r6, #0x188]
0078f70c: b        #0x78f348
0078f710: mov      r0, r5
0078f714: bl       #0x797960
0078f718: ldr      r1, [r6, #0x178]
0078f71c: mov      sl, r0
0078f720: ldrb     r3, [r1, #0x4c]
0078f724: cmp      r3, sl
0078f728: bne      #0x78f5ec
0078f72c: ldrb     r3, [r1, #0x4d]
0078f730: ldr      r2, [sp, #4]
0078f734: cmp      r3, r2
0078f738: bne      #0x78f5ec
0078f73c: ldrsb    r3, [sp, #0x50]
0078f740: cmn      r3, #1
0078f744: ldrsb    r3, [r1, #0x30]
0078f748: addne    r0, sb, #1
0078f74c: ldreq    r0, [sp, #0x5c]
0078f750: cmn      r3, #1
0078f754: addne    r1, r1, #0x31
0078f758: ldreq    r1, [r1, #0x3c]
0078f75c: bl       #0x30e31c
0078f760: cmp      r0, #0
0078f764: beq      #0x78f674
0078f768: b        #0x78f5ec
0078f76c: mov      r0, r5
0078f770: bl       #0x797960
0078f774: str      r0, [sp, #4]
0078f778: b        #0x78f588
0078f77c: mov      r0, r5
0078f780: bl       #0x420a84
0078f784: mov      r1, r0
0078f788: mov      r0, sb
0078f78c: bl       #0x752f50
0078f790: b        #0x78f538
0078f794: mov      r0, r5
0078f798: bl       #0x420a84
0078f79c: ldrsb    r3, [r0]
0078f7a0: ldr      r1, [pc, #0x238]
0078f7a4: cmn      r3, #1
0078f7a8: ldreq    r0, [r0, #0xc]
0078f7ac: addne    r0, r0, #1
0078f7b0: add      r1, pc, r1
0078f7b4: bl       #0x30e31c
0078f7b8: cmp      r0, #0
0078f7bc: streq    r0, [r6, #0x17c]
0078f7c0: beq      #0x78f4e0
0078f7c4: mov      r0, r5
0078f7c8: bl       #0x420a84
0078f7cc: ldr      r1, [pc, #0x210]
0078f7d0: add      r1, pc, r1
0078f7d4: bl       #0x78aebc
0078f7d8: cmp      r0, #0
0078f7dc: movne    r3, #2
0078f7e0: strne    r3, [r6, #0x17c]
0078f7e4: bne      #0x78f4e0
0078f7e8: mov      r0, r5
0078f7ec: bl       #0x420a84
0078f7f0: ldr      r1, [pc, #0x1f0]
0078f7f4: add      r1, pc, r1
0078f7f8: bl       #0x78aebc
0078f7fc: cmp      r0, #0
0078f800: movne    r3, #1
0078f804: strne    r3, [r6, #0x17c]
0078f808: bne      #0x78f4e0
0078f80c: mov      r0, r5
0078f810: bl       #0x420a84
0078f814: ldr      r1, [pc, #0x1d0]
0078f818: add      r1, pc, r1
0078f81c: bl       #0x78aebc
0078f820: cmp      r0, #0
0078f824: movne    r3, #3
0078f828: strne    r3, [r6, #0x17c]
0078f82c: b        #0x78f4e0
0078f830: mov      r0, r5
0078f834: bl       #0x797a54
0078f838: bl       #0x30e6a0
0078f83c: mov      r1, #0x41000000
0078f840: add      r1, r1, #0xa00000
0078f844: bl       #0x30ed6c
0078f848: str      r0, [r6, #0x174]
0078f84c: b        #0x78f49c
0078f850: mov      r0, r5
0078f854: bl       #0x797a54
0078f858: bl       #0x30ea24
0078f85c: asr      r3, r0, #8
0078f860: asr      r2, r0, #0x10
0078f864: strb     r3, [r6, #0x171]
0078f868: mvn      r3, #0
0078f86c: strb     r2, [r6, #0x170]
0078f870: strb     r0, [r6, #0x172]
0078f874: strb     r3, [r6, #0x173]
0078f878: b        #0x78f458
0078f87c: mov      r0, r5
0078f880: bl       #0x797a54
0078f884: bl       #0x30e6a0
0078f888: mov      r1, #0x41000000
0078f88c: add      r1, r1, #0xa00000
0078f890: bl       #0x30ed6c
0078f894: str      r0, [r6, #0x190]
0078f898: b        #0x78f414
0078f89c: mov      r0, r5
0078f8a0: bl       #0x797a54
0078f8a4: bl       #0x30e6a0
0078f8a8: mov      r1, #0x41000000
0078f8ac: add      r1, r1, #0xa00000
0078f8b0: bl       #0x30ed6c
0078f8b4: str      r0, [r6, #0x18c]
0078f8b8: b        #0x78f3d0
0078f8bc: mov      r1, #0x13
0078f8c0: ldr      r3, [r4]
0078f8c4: mov      r0, r4
0078f8c8: mov      lr, pc
0078f8cc: ldr      pc, [r3, #8]
0078f8d0: cmp      r0, #0
0078f8d4: movne    r1, r4
0078f8d8: moveq    r1, #0
0078f8dc: add      r0, r6, #0x178
0078f8e0: bl       #0x764234
0078f8e4: b        #0x78f650
0078f8e8: ldr      r0, [sp, #0xfc]
0078f8ec: ldr      r1, [sp, #0xf8]
0078f8f0: bl       #0x752b38
0078f8f4: b        #0x78f2fc
0078f8f8: ldr      r0, [sp, #0xd4]
0078f8fc: ldr      r1, [sp, #0xd0]
0078f900: bl       #0x752b38
0078f904: b        #0x78f384
0078f908: ldr      r0, [sp, #0xc0]
0078f90c: ldr      r1, [sp, #0xbc]
0078f910: bl       #0x752b38
0078f914: b        #0x78f3c8
0078f918: ldr      r0, [sp, #0x84]
0078f91c: ldr      r1, [sp, #0x80]
0078f920: bl       #0x752b38
0078f924: b        #0x78f494
0078f928: ldr      r0, [sp, #0xe8]
0078f92c: ldr      r1, [sp, #0xe4]
0078f930: bl       #0x752b38
0078f934: b        #0x78f340
0078f938: ldr      r0, [sp, #0x5c]
0078f93c: ldr      r1, [sp, #0x58]
0078f940: bl       #0x752b38
0078f944: b        #0x78f68c
0078f948: ldr      r0, [sp, #0x34]
0078f94c: ldr      r1, [sp, #0x30]
0078f950: bl       #0x752b38
0078f954: b        #0x78f580
0078f958: ldr      r0, [sp, #0x20]
0078f95c: ldr      r1, [sp, #0x1c]
0078f960: bl       #0x752b38
0078f964: b        #0x78f5d4
0078f968: ldr      r0, [sp, #0x70]
0078f96c: ldr      r1, [sp, #0x6c]
0078f970: bl       #0x752b38
0078f974: b        #0x78f4d8
0078f978: ldr      r0, [sp, #0xac]
0078f97c: ldr      r1, [sp, #0xa8]
0078f980: bl       #0x752b38
0078f984: b        #0x78f40c
0078f988: ldr      r0, [sp, #0x98]
0078f98c: ldr      r1, [sp, #0x94]
0078f990: bl       #0x752b38
0078f994: b        #0x78f450
0078f998: ldr      r0, [sp, #0x48]
0078f99c: ldr      r1, [sp, #0x44]
0078f9a0: bl       #0x752b38
0078f9a4: b        #0x78f530
0078f9a8: bl       #0x30e310

# _ZN7gameswf19edit_text_character14set_text_valueERKNS_9tu_stringEb
00790ab0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00790ab4: ldr      r4, [pc, #0x1e0]
00790ab8: ldr      r6, [pc, #0x1e0]
00790abc: sub      sp, sp, #0x50
00790ac0: add      r4, pc, r4
00790ac4: ldr      r3, [r4, r6]
00790ac8: mov      r5, r0
00790acc: mov      sl, r1
00790ad0: ldr      r3, [r3]
00790ad4: str      r3, [sp, #0x4c]
00790ad8: bl       #0x78f1ec
00790adc: mov      r0, r5
00790ae0: bl       #0x78a364
00790ae4: ldrsb    r3, [r0]
00790ae8: cmn      r3, #1
00790aec: ldreq    r3, [r0, #4]
00790af0: sub      r3, r3, #1
00790af4: cmp      r3, #0
00790af8: ble      #0x790c1c
00790afc: ldr      r7, [r5, #0x40]
00790b00: cmp      r7, #0
00790b04: beq      #0x790b18
00790b08: ldr      r0, [r5, #0x3c]
00790b0c: ldrb     r3, [r0, #4]
00790b10: cmp      r3, #0
00790b14: beq      #0x790c38
00790b18: ldr      r3, [sp, #0x48]
00790b1c: mvn      r1, #0
00790b20: mov      r2, #0
00790b24: bfi      r3, r1, #0, #0x18
00790b28: lsr      r1, r3, #0x18
00790b2c: bfi      r1, r2, #0, #1
00790b30: mov      ip, #1
00790b34: mov      r0, r5
00790b38: str      r3, [sp, #0x48]
00790b3c: strb     ip, [sp, #0x38]
00790b40: strb     r2, [sp, #0x39]
00790b44: strb     r1, [sp, #0x4b]
00790b48: bl       #0x78a364
00790b4c: add      r8, sp, #0x24
00790b50: mov      r1, r0
00790b54: mov      r0, r8
00790b58: bl       #0x75302c
00790b5c: mov      r0, r5
00790b60: add      r5, sp, #0x38
00790b64: bl       #0x78a364
00790b68: mov      r1, r5
00790b6c: mov      r2, r8
00790b70: bl       #0x7cd130
00790b74: cmp      r0, #0
00790b78: beq      #0x790b98
00790b7c: ldrsb    r3, [sp, #0x38]
00790b80: mov      r0, r7
00790b84: cmn      r3, #1
00790b88: addne    r1, r5, #1
00790b8c: ldreq    r1, [sp, #0x44]
00790b90: bl       #0x76b284
00790b94: mov      r7, r0
00790b98: cmp      r7, #0
00790b9c: beq      #0x790c04
00790ba0: ldr      r3, [r7]
00790ba4: add      sb, sp, #0x10
00790ba8: mov      r1, r8
00790bac: mov      r0, sb
00790bb0: ldr      r8, [r3, #0x1c]
00790bb4: bl       #0x75302c
00790bb8: ldrsb    r3, [sl]
00790bbc: add      r5, sp, #4
00790bc0: mov      r0, r5
00790bc4: cmn      r3, #1
00790bc8: ldreq    r1, [sl, #0xc]
00790bcc: mov      r3, #0
00790bd0: addne    r1, sl, #1
00790bd4: strb     r3, [sp, #5]
00790bd8: strb     r3, [sp, #4]
00790bdc: bl       #0x797350
00790be0: mov      r1, sb
00790be4: mov      r2, r5
00790be8: mov      r0, r7
00790bec: blx      r8
00790bf0: mov      r0, r5
00790bf4: bl       #0x797124
00790bf8: ldrsb    r3, [sp, #0x10]
00790bfc: cmn      r3, #1
00790c00: beq      #0x790c60
00790c04: ldrsb    r3, [sp, #0x24]
00790c08: cmn      r3, #1
00790c0c: beq      #0x790c70
00790c10: ldrsb    r3, [sp, #0x38]
00790c14: cmn      r3, #1
00790c18: beq      #0x790c88
00790c1c: ldr      r3, [r4, r6]
00790c20: ldr      r2, [sp, #0x4c]
00790c24: ldr      r3, [r3]
00790c28: cmp      r2, r3
00790c2c: bne      #0x790c98
00790c30: add      sp, sp, #0x50
00790c34: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00790c38: ldr      r1, [r0]
00790c3c: sub      r1, r1, #1
00790c40: cmp      r1, #0
00790c44: str      r1, [r0]
00790c48: bne      #0x790c50
00790c4c: bl       #0x752b38
00790c50: mov      r7, #0
00790c54: str      r7, [r5, #0x3c]
00790c58: str      r7, [r5, #0x40]
00790c5c: b        #0x790b18
00790c60: ldr      r0, [sp, #0x1c]
00790c64: ldr      r1, [sp, #0x18]
00790c68: bl       #0x752b38
00790c6c: b        #0x790c04
00790c70: ldr      r0, [sp, #0x30]
00790c74: ldr      r1, [sp, #0x2c]
00790c78: bl       #0x752b38
00790c7c: ldrsb    r3, [sp, #0x38]
00790c80: cmn      r3, #1
00790c84: bne      #0x790c1c
00790c88: ldr      r0, [sp, #0x44]
00790c8c: ldr      r1, [sp, #0x40]
00790c90: bl       #0x752b38
00790c94: b        #0x790c1c
00790c98: bl       #0x30e310

# _ZN7gameswf11html_reader5parseEPNS_19edit_text_characterE
0078e354: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078e358: ldr      r8, [pc, #0xc14]
0078e35c: ldr      r2, [pc, #0xc14]
0078e360: sub      sp, sp, #0x3b4
0078e364: add      r8, pc, r8
0078e368: str      r2, [sp, #4]
0078e36c: ldr      r2, [r8, r2]
0078e370: ldrb     r3, [r1, #0x138]
0078e374: mov      r4, r1
0078e378: ldr      r2, [r2]
0078e37c: sxtb     r3, r3
0078e380: cmn      r3, #1
0078e384: str      r2, [sp, #0x3ac]
0078e388: ldreq    r3, [r1, #0x13c]
0078e38c: mov      r6, r0
0078e390: sub      r3, r3, #1
0078e394: cmp      r3, #0
0078e398: bne      #0x78e3bc
0078e39c: ldr      sb, [sp, #4]
0078e3a0: ldr      r2, [sp, #0x3ac]
0078e3a4: ldr      r3, [r8, sb]
0078e3a8: ldr      r3, [r3]
0078e3ac: cmp      r2, r3
0078e3b0: bne      #0x78eee8
0078e3b4: add      sp, sp, #0x3b4
0078e3b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078e3bc: ldr      r2, [r1, #0x170]
0078e3c0: add      r5, sp, #0x44
0078e3c4: ldr      r1, [r1, #0x178]
0078e3c8: mov      r3, #0
0078e3cc: mov      r0, r5
0078e3d0: mov      ip, #0xc
0078e3d4: str      ip, [sp, #0x48]
0078e3d8: strb     r3, [sp, #0x50]
0078e3dc: str      r3, [sp, #0x44]
0078e3e0: str      r2, [sp, #0x4c]
0078e3e4: bl       #0x764234
0078e3e8: ldr      r0, [r4, #0x174]
0078e3ec: bl       #0x30e4cc
0078e3f0: mov      r1, r5
0078e3f4: str      r0, [sp, #0x48]
0078e3f8: mov      r0, r6
0078e3fc: bl       #0x78ad54
0078e400: ldrb     fp, [r4, #0x138]
0078e404: ldr      r3, [pc, #0xb70]
0078e408: ldr      ip, [pc, #0xb70]
0078e40c: sxtb     sb, fp
0078e410: cmn      sb, #1
0078e414: add      r3, pc, r3
0078e418: ldreq    r5, [r4, #0x144]
0078e41c: str      r3, [sp, #0x10]
0078e420: ldr      r3, [pc, #0xb5c]
0078e424: addne    r5, r4, #0x138
0078e428: mov      r7, #0
0078e42c: add      r3, pc, r3
0078e430: str      r3, [sp, #0x18]
0078e434: ldr      r3, [pc, #0xb4c]
0078e438: addne    r5, r5, #1
0078e43c: str      r7, [sp, #0x14]
0078e440: add      r3, pc, r3
0078e444: str      r3, [sp, #0x1c]
0078e448: ldr      r3, [pc, #0xb3c]
0078e44c: str      ip, [sp, #0x24]
0078e450: add      r3, pc, r3
0078e454: str      r3, [sp, #0x20]
0078e458: cmn      sb, #1
0078e45c: ldrbne   r3, [r4, #0x138]
0078e460: ldreq    r3, [r4, #0x13c]
0078e464: sxtbne   r3, r3
0078e468: sub      r3, r3, #1
0078e46c: cmp      r7, r3
0078e470: bge      #0x78e4f0
0078e474: ldrsb    r3, [r5, r7]
0078e478: add      sl, r5, r7
0078e47c: cmp      r3, #0x3c
0078e480: beq      #0x78e568
0078e484: mov      r0, sl
0078e488: mov      r1, #0x3c
0078e48c: bl       #0x30ec28
0078e490: subs     r7, r0, #0
0078e494: bne      #0x78e504
0078e498: cmn      sb, #1
0078e49c: ldrbne   r3, [r4, #0x138]
0078e4a0: ldreq    r3, [r4, #0x13c]
0078e4a4: add      r7, sp, #0x258
0078e4a8: sxtbne   r3, r3
0078e4ac: sub      r3, r3, #1
0078e4b0: add      r5, r5, r3
0078e4b4: mov      r1, sl
0078e4b8: rsb      r2, sl, r5
0078e4bc: mov      r0, r7
0078e4c0: bl       #0x751eb4
0078e4c4: ldr      r2, [r6, #4]
0078e4c8: ldr      r3, [r6]
0078e4cc: mov      r0, r4
0078e4d0: sub      r2, r2, #1
0078e4d4: add      r2, r3, r2, lsl #4
0078e4d8: mov      r1, r7
0078e4dc: mov      r3, #1
0078e4e0: bl       #0x78cb90
0078e4e4: ldrb     r3, [sp, #0x258]
0078e4e8: cmp      r3, #0xff
0078e4ec: beq      #0x78eacc
0078e4f0: ldr      r0, [sp, #0x44]
0078e4f4: cmp      r0, #0
0078e4f8: beq      #0x78e39c
0078e4fc: bl       #0x75a240
0078e500: b        #0x78e39c
0078e504: mov      r1, sl
0078e508: add      sl, sp, #0x258
0078e50c: rsb      r2, r1, r7
0078e510: mov      r0, sl
0078e514: bl       #0x751eb4
0078e518: ldr      r2, [r6, #4]
0078e51c: ldr      r3, [r6]
0078e520: mov      r1, sl
0078e524: sub      r2, r2, #1
0078e528: add      r2, r3, r2, lsl #4
0078e52c: mov      r0, r4
0078e530: mov      r3, #1
0078e534: bl       #0x78cb90
0078e538: ldrb     r3, [sp, #0x258]
0078e53c: rsb      r7, r5, r7
0078e540: cmp      r3, #0xff
0078e544: beq      #0x78e558
0078e548: ldrb     fp, [r4, #0x138]
0078e54c: mov      r3, fp
0078e550: sxtb     sb, fp
0078e554: b        #0x78e458
0078e558: ldr      r0, [sp, #0x264]
0078e55c: ldr      r1, [sp, #0x260]
0078e560: bl       #0x752b38
0078e564: b        #0x78e548
0078e568: mov      r0, sl
0078e56c: mov      r1, #0x3e
0078e570: bl       #0x30ec28
0078e574: cmp      r0, #0
0078e578: str      r0, [sp]
0078e57c: beq      #0x78e4f0
0078e580: cmn      sb, #1
0078e584: ldrbne   r3, [r4, #0x138]
0078e588: ldreq    r3, [r4, #0x13c]
0078e58c: add      r7, r7, #1
0078e590: sxtbne   r3, r3
0078e594: sub      r3, r3, #1
0078e598: cmp      r7, r3
0078e59c: bge      #0x78e4f0
0078e5a0: ldrsb    r3, [r5, r7]
0078e5a4: add      r7, r5, r7
0078e5a8: cmp      r3, #0x2f
0078e5ac: bne      #0x78e5dc
0078e5b0: ldr      r1, [r6, #4]
0078e5b4: cmp      r1, #1
0078e5b8: ble      #0x78e5cc
0078e5bc: sub      r1, r1, #1
0078e5c0: mov      r0, r6
0078e5c4: bl       #0x78bc0c
0078e5c8: ldrb     fp, [r4, #0x138]
0078e5cc: ldr      r2, [sp]
0078e5d0: rsb      r7, r5, #1
0078e5d4: add      r7, r2, r7
0078e5d8: b        #0x78e54c
0078e5dc: add      sb, sp, #0x58
0078e5e0: mov      r1, #0
0078e5e4: mov      r2, #0x200
0078e5e8: mov      r0, sb
0078e5ec: bl       #0x30e460
0078e5f0: ldr      lr, [sp]
0078e5f4: mvn      r2, sl
0078e5f8: mov      r1, r7
0078e5fc: mov      sl, #0
0078e600: add      r2, lr, r2
0078e604: add      r7, sp, #0x3b0
0078e608: mov      r0, sb
0078e60c: bl       #0x30e868
0078e610: str      sl, [r7, #-0x35c]!
0078e614: mov      r2, sb
0078e618: mov      r1, r7
0078e61c: mov      r0, r6
0078e620: bl       #0x78e060
0078e624: ldm      r6, {r3, sb}
0078e628: mov      r2, #0xc
0078e62c: mov      fp, #1
0078e630: add      r1, sp, #0x34
0078e634: str      r2, [sp, #0x38]
0078e638: str      sl, [sp, #0x34]
0078e63c: strb     sl, [sp, #0x3c]
0078e640: strb     sl, [sp, #0x3d]
0078e644: strb     sl, [sp, #0x3e]
0078e648: strb     sl, [sp, #0x40]
0078e64c: str      r1, [sp, #0xc]
0078e650: sub      sb, sb, #1
0078e654: strb     fp, [sp, #0x3f]
0078e658: ldr      r1, [r3, sb, lsl #4]
0078e65c: ldr      r0, [sp, #0xc]
0078e660: add      sb, r3, sb, lsl #4
0078e664: bl       #0x764234
0078e668: ldr      r2, [sb, #4]
0078e66c: ldr      r3, [sp, #0x3a8]
0078e670: add      lr, sp, #0x370
0078e674: str      r2, [sp, #0x38]
0078e678: ldr      r1, [sb, #8]
0078e67c: mvn      r2, #0
0078e680: bfi      r3, r2, #0, #0x18
0078e684: str      r1, [sp, #0x3c]
0078e688: ldrb     ip, [sb, #0xc]
0078e68c: lsr      r2, r3, #0x18
0078e690: add      sb, sp, #0x384
0078e694: bfi      r2, sl, #0, #1
0078e698: ldr      r1, [sp, #0x10]
0078e69c: mov      r0, sb
0078e6a0: str      r3, [sp, #0x3a8]
0078e6a4: str      lr, [sp, #8]
0078e6a8: strb     ip, [sp, #0x40]
0078e6ac: strb     r2, [sp, #0x3ab]
0078e6b0: strb     sl, [sp, #0x399]
0078e6b4: strb     fp, [sp, #0x398]
0078e6b8: add      sl, sp, #0x398
0078e6bc: bl       #0x413a7c
0078e6c0: mov      r1, sb
0078e6c4: ldr      r0, [sp, #8]
0078e6c8: bl       #0x75302c
0078e6cc: ldr      r1, [sp, #8]
0078e6d0: mov      r0, r7
0078e6d4: mov      r2, sl
0078e6d8: bl       #0x78dfbc
0078e6dc: mov      sb, r0
0078e6e0: ldrb     r0, [sp, #0x370]
0078e6e4: sxtb     r3, r0
0078e6e8: cmn      r3, #1
0078e6ec: beq      #0x78e9e8
0078e6f0: ldrb     r1, [sp, #0x384]
0078e6f4: sxtb     r3, r1
0078e6f8: cmn      r3, #1
0078e6fc: beq      #0x78e9d8
0078e700: cmp      sb, #0
0078e704: bne      #0x78e738
0078e708: ldrb     r2, [sp, #0x398]
0078e70c: sxtb     sb, r2
0078e710: cmn      sb, #1
0078e714: beq      #0x78e7c0
0078e718: ldr      r0, [sp, #0x34]
0078e71c: cmp      r0, #0
0078e720: beq      #0x78e728
0078e724: bl       #0x75a240
0078e728: mov      r0, r7
0078e72c: bl       #0x78c600
0078e730: ldrb     fp, [r4, #0x138]
0078e734: b        #0x78e5cc
0078e738: ldrb     r3, [sp, #0x398]
0078e73c: ldr      r1, [sp, #0x18]
0078e740: sxtb     sb, r3
0078e744: cmn      sb, #1
0078e748: addne    r0, sl, #1
0078e74c: ldreq    r0, [sp, #0x3a4]
0078e750: bl       #0x30e31c
0078e754: cmp      r0, #0
0078e758: bne      #0x78e7d0
0078e75c: ldr      ip, [sp, #0x14]
0078e760: cmp      ip, #0
0078e764: beq      #0x78e7ac
0078e768: ldr      r1, [pc, #0x820]
0078e76c: add      sl, sp, #0x35c
0078e770: mov      r0, sl
0078e774: add      r1, pc, r1
0078e778: bl       #0x413a7c
0078e77c: ldr      r2, [r6, #4]
0078e780: ldr      r3, [r6]
0078e784: mov      r1, sl
0078e788: sub      r2, r2, #1
0078e78c: add      r2, r3, r2, lsl #4
0078e790: mov      r0, r4
0078e794: mov      r3, #1
0078e798: bl       #0x78cb90
0078e79c: mov      r0, sl
0078e7a0: bl       #0x41fed8
0078e7a4: ldrb     lr, [sp, #0x398]
0078e7a8: sxtb     sb, lr
0078e7ac: ldr      r0, [sp, #0x14]
0078e7b0: cmn      sb, #1
0078e7b4: add      r0, r0, #1
0078e7b8: str      r0, [sp, #0x14]
0078e7bc: bne      #0x78e718
0078e7c0: ldr      r0, [sp, #0x3a4]
0078e7c4: ldr      r1, [sp, #0x3a0]
0078e7c8: bl       #0x752b38
0078e7cc: b        #0x78e718
0078e7d0: mov      r0, sl
0078e7d4: ldr      r1, [sp, #0x1c]
0078e7d8: bl       #0x78aebc
0078e7dc: subs     sb, r0, #0
0078e7e0: beq      #0x78eadc
0078e7e4: ldr      r3, [sp, #0x358]
0078e7e8: mvn      r2, #0
0078e7ec: ldr      r1, [pc, #0x7a0]
0078e7f0: bfi      r3, r2, #0, #0x18
0078e7f4: lsr      r2, r3, #0x18
0078e7f8: add      sl, sp, #0x334
0078e7fc: mov      sb, #0
0078e800: add      ip, sp, #0x348
0078e804: bfi      r2, sb, #0, #1
0078e808: add      r1, pc, r1
0078e80c: str      ip, [sp, #0x28]
0078e810: mov      r0, sl
0078e814: mov      ip, #1
0078e818: str      r3, [sp, #0x358]
0078e81c: strb     ip, [sp, #0x348]
0078e820: strb     r2, [sp, #0x35b]
0078e824: strb     sb, [sp, #0x349]
0078e828: bl       #0x413a7c
0078e82c: mov      r1, sl
0078e830: ldr      r2, [sp, #0x28]
0078e834: mov      r0, r7
0078e838: bl       #0x78dfbc
0078e83c: mov      fp, r0
0078e840: mov      r0, sl
0078e844: bl       #0x41fed8
0078e848: cmp      fp, sb
0078e84c: bne      #0x78ea80
0078e850: ldr      r3, [sp, #0x330]
0078e854: mvn      r2, #0
0078e858: ldr      r1, [pc, #0x738]
0078e85c: bfi      r3, r2, #0, #0x18
0078e860: mov      ip, #0
0078e864: lsr      r2, r3, #0x18
0078e868: add      sl, sp, #0x30c
0078e86c: add      lr, sp, #0x320
0078e870: bfi      r2, ip, #0, #1
0078e874: add      r1, pc, r1
0078e878: str      lr, [sp, #8]
0078e87c: mov      r0, sl
0078e880: mov      lr, #1
0078e884: str      r3, [sp, #0x330]
0078e888: strb     lr, [sp, #0x320]
0078e88c: strb     r2, [sp, #0x333]
0078e890: strb     ip, [sp, #0x321]
0078e894: bl       #0x413a7c
0078e898: mov      r0, r7
0078e89c: mov      r1, sl
0078e8a0: ldr      r2, [sp, #8]
0078e8a4: bl       #0x78dfbc
0078e8a8: cmp      r0, #0
0078e8ac: bne      #0x78e9f8
0078e8b0: mov      r0, sl
0078e8b4: bl       #0x41fed8
0078e8b8: ldr      r3, [sp, #0x308]
0078e8bc: mvn      r2, #0
0078e8c0: ldr      r1, [pc, #0x6d4]
0078e8c4: bfi      r3, r2, #0, #0x18
0078e8c8: mov      ip, #0
0078e8cc: lsr      r2, r3, #0x18
0078e8d0: add      sb, sp, #0x2e4
0078e8d4: bfi      r2, ip, #0, #1
0078e8d8: mov      lr, #1
0078e8dc: add      r1, pc, r1
0078e8e0: mov      r0, sb
0078e8e4: add      sl, sp, #0x2f8
0078e8e8: str      r3, [sp, #0x308]
0078e8ec: strb     lr, [sp, #0x2f8]
0078e8f0: strb     ip, [sp, #0x2f9]
0078e8f4: strb     r2, [sp, #0x30b]
0078e8f8: bl       #0x413a7c
0078e8fc: mov      r1, sb
0078e900: mov      r2, sl
0078e904: mov      r0, r7
0078e908: bl       #0x78dfbc
0078e90c: mov      fp, r0
0078e910: mov      r0, sb
0078e914: bl       #0x41fed8
0078e918: cmp      fp, #0
0078e91c: beq      #0x78e9a8
0078e920: ldrb     r2, [sp, #0x2f8]
0078e924: mov      r1, #0x25
0078e928: sxtb     sb, r2
0078e92c: cmn      sb, #1
0078e930: addne    r0, sl, #1
0078e934: ldreq    r0, [sp, #0x304]
0078e938: bl       #0x30ec28
0078e93c: cmp      r0, #0
0078e940: beq      #0x78ecac
0078e944: cmn      sb, #1
0078e948: ldrbne   r3, [sp, #0x2f8]
0078e94c: ldreq    r1, [sp, #0x2fc]
0078e950: mov      r0, sl
0078e954: sxtbne   r1, r3
0078e958: sub      r1, r1, #1
0078e95c: sub      r1, r1, #1
0078e960: bl       #0x78b94c
0078e964: ldrb     sb, [sp, #0x2f8]
0078e968: ldr      r1, [r6, #4]
0078e96c: ldr      r2, [r6]
0078e970: sxtb     r3, sb
0078e974: cmn      r3, #1
0078e978: add      r2, r2, r1, lsl #4
0078e97c: addne    r0, sl, #1
0078e980: ldreq    r0, [sp, #0x304]
0078e984: ldr      sb, [r2, #-0xc]
0078e988: bl       #0x30e094
0078e98c: mul      r0, sb, r0
0078e990: movw     r3, #0x851f
0078e994: movt     r3, #0x51eb
0078e998: smull    ip, r3, r3, r0
0078e99c: asr      r0, r0, #0x1f
0078e9a0: rsb      r3, r0, r3, asr #5
0078e9a4: str      r3, [sp, #0x38]
0078e9a8: ldr      r1, [sp, #0xc]
0078e9ac: mov      r0, r6
0078e9b0: bl       #0x78ad54
0078e9b4: mov      r0, sl
0078e9b8: bl       #0x41fed8
0078e9bc: ldr      r0, [sp, #8]
0078e9c0: bl       #0x41fed8
0078e9c4: ldr      r0, [sp, #0x28]
0078e9c8: bl       #0x41fed8
0078e9cc: ldrb     r0, [sp, #0x398]
0078e9d0: sxtb     sb, r0
0078e9d4: b        #0x78e710
0078e9d8: ldr      r0, [sp, #0x390]
0078e9dc: ldr      r1, [sp, #0x38c]
0078e9e0: bl       #0x752b38
0078e9e4: b        #0x78e700
0078e9e8: ldr      r0, [sp, #0x37c]
0078e9ec: ldr      r1, [sp, #0x378]
0078e9f0: bl       #0x752b38
0078e9f4: b        #0x78e6f0
0078e9f8: ldrb     r0, [sp, #0x320]
0078e9fc: sxtb     r3, r0
0078ea00: cmn      r3, #1
0078ea04: ldreq    r3, [sp, #0x324]
0078ea08: sub      r3, r3, #1
0078ea0c: cmp      r3, #0
0078ea10: ble      #0x78e8b0
0078ea14: mov      r0, sl
0078ea18: bl       #0x41fed8
0078ea1c: ldrb     r1, [sp, #0x320]
0078ea20: sxtb     sl, r1
0078ea24: cmn      sl, #1
0078ea28: ldrne    r2, [sp, #8]
0078ea2c: ldreq    r3, [sp, #0x32c]
0078ea30: addne    r3, r2, #1
0078ea34: ldrsb    r3, [r3]
0078ea38: cmp      r3, #0x23
0078ea3c: beq      #0x78ebc0
0078ea40: cmn      sl, #1
0078ea44: ldrne    r1, [sp, #8]
0078ea48: ldreq    r0, [sp, #0x32c]
0078ea4c: addne    r0, r1, #1
0078ea50: bl       #0x30e094
0078ea54: orr      r0, r0, #0xff000000
0078ea58: bl       #0x30ed30
0078ea5c: bl       #0x30ea24
0078ea60: mvn      r1, #0
0078ea64: ubfx     r3, r0, #0x10, #8
0078ea68: ubfx     r2, r0, #8, #8
0078ea6c: strb     r1, [sp, #0x3f]
0078ea70: strb     r0, [sp, #0x3e]
0078ea74: strb     r2, [sp, #0x3d]
0078ea78: strb     r3, [sp, #0x3c]
0078ea7c: b        #0x78e8b8
0078ea80: mov      r0, r4
0078ea84: bl       #0x780374
0078ea88: mov      r1, sb
0078ea8c: mov      fp, r0
0078ea90: mov      r0, #0x88
0078ea94: bl       #0x752ba8
0078ea98: mov      r1, fp
0078ea9c: mov      sl, r0
0078eaa0: bl       #0x7cf8d8
0078eaa4: mov      r0, sl
0078eaa8: ldr      r1, [sp, #0x34]
0078eaac: bl       #0x7ce628
0078eab0: add      r0, sl, #0x30
0078eab4: ldr      r1, [sp, #0x28]
0078eab8: bl       #0x752f50
0078eabc: ldr      r0, [sp, #0xc]
0078eac0: mov      r1, sl
0078eac4: bl       #0x764234
0078eac8: b        #0x78e850
0078eacc: ldr      r0, [sp, #0x264]
0078ead0: ldr      r1, [sp, #0x260]
0078ead4: bl       #0x752b38
0078ead8: b        #0x78e4f0
0078eadc: mov      r0, sl
0078eae0: ldr      r1, [sp, #0x20]
0078eae4: bl       #0x78aebc
0078eae8: subs     fp, r0, #0
0078eaec: beq      #0x78eb4c
0078eaf0: mov      r0, r4
0078eaf4: bl       #0x780374
0078eaf8: mov      r1, sb
0078eafc: mov      fp, r0
0078eb00: mov      r0, #0x88
0078eb04: bl       #0x752ba8
0078eb08: mov      r1, fp
0078eb0c: mov      sl, r0
0078eb10: bl       #0x7cf8d8
0078eb14: mov      r0, sl
0078eb18: ldr      r1, [sp, #0x34]
0078eb1c: bl       #0x7ce628
0078eb20: mov      r3, #1
0078eb24: strb     r3, [sl, #0x4d]
0078eb28: mov      r1, sl
0078eb2c: ldr      r0, [sp, #0xc]
0078eb30: bl       #0x764234
0078eb34: ldr      r1, [sp, #0xc]
0078eb38: mov      r0, r6
0078eb3c: bl       #0x78ad54
0078eb40: ldrb     r1, [sp, #0x398]
0078eb44: sxtb     sb, r1
0078eb48: b        #0x78e710
0078eb4c: ldr      r2, [sp, #0x24]
0078eb50: mov      r0, sl
0078eb54: add      r1, pc, r2
0078eb58: bl       #0x78aebc
0078eb5c: cmp      r0, #0
0078eb60: beq      #0x78ec74
0078eb64: mov      r0, r4
0078eb68: bl       #0x780374
0078eb6c: mov      r1, fp
0078eb70: mov      sb, r0
0078eb74: mov      r0, #0x88
0078eb78: bl       #0x752ba8
0078eb7c: mov      r1, sb
0078eb80: mov      sl, r0
0078eb84: bl       #0x7cf8d8
0078eb88: mov      r0, sl
0078eb8c: ldr      r1, [sp, #0x34]
0078eb90: bl       #0x7ce628
0078eb94: mov      r3, #1
0078eb98: strb     r3, [sl, #0x4c]
0078eb9c: mov      r1, sl
0078eba0: ldr      r0, [sp, #0xc]
0078eba4: bl       #0x764234
0078eba8: mov      r0, r6
0078ebac: ldr      r1, [sp, #0xc]
0078ebb0: bl       #0x78ad54
0078ebb4: ldrb     r3, [sp, #0x398]
0078ebb8: sxtb     sb, r3
0078ebbc: b        #0x78e710
0078ebc0: cmn      sl, #1
0078ebc4: ldrbne   sb, [sp, #0x320]
0078ebc8: ldreq    r3, [sp, #0x324]
0078ebcc: sxtbne   r3, sb
0078ebd0: sub      r3, r3, #1
0078ebd4: sub      r3, r3, #1
0078ebd8: cmp      r3, #0
0078ebdc: ble      #0x78eee0
0078ebe0: ldr      r2, [pc, #0x3b8]
0078ebe4: ldr      ip, [sp, #8]
0078ebe8: ldr      sb, [sp, #0x32c]
0078ebec: ldr      r2, [r8, r2]
0078ebf0: mov      r1, #0
0078ebf4: mov      r0, #0xff000000
0078ebf8: ldr      r2, [r2]
0078ebfc: add      fp, ip, #1
0078ec00: str      r2, [sp, #0x2c]
0078ec04: b        #0x78ec0c
0078ec08: add      r1, r1, #4
0078ec0c: cmn      sl, #1
0078ec10: moveq    r2, sb
0078ec14: movne    r2, fp
0078ec18: ldrsb    r2, [r2, r3]
0078ec1c: cmp      r2, #0xff
0078ec20: ldrls    lr, [sp, #0x2c]
0078ec24: addls    r2, lr, r2, lsl #1
0078ec28: ldrshls  r2, [r2, #2]
0078ec2c: uxtb     r2, r2
0078ec30: uxtb     ip, r2
0078ec34: sub      lr, ip, #0x30
0078ec38: uxtb     lr, lr
0078ec3c: cmp      lr, #9
0078ec40: sxtbls   r2, r2
0078ec44: subls    r2, r2, #0x30
0078ec48: orrls    r0, r0, r2, lsl r1
0078ec4c: bls      #0x78ec68
0078ec50: sub      ip, ip, #0x61
0078ec54: uxtb     ip, ip
0078ec58: cmp      ip, #5
0078ec5c: sxtbls   r2, r2
0078ec60: subls    r2, r2, #0x57
0078ec64: orrls    r0, r0, r2, lsl r1
0078ec68: subs     r3, r3, #1
0078ec6c: bne      #0x78ec08
0078ec70: b        #0x78ea58
0078ec74: ldr      r1, [pc, #0x328]
0078ec78: mov      r0, sl
0078ec7c: add      r1, pc, r1
0078ec80: bl       #0x78aebc
0078ec84: subs     fp, r0, #0
0078ec88: beq      #0x78ed50
0078ec8c: mov      r3, #1
0078ec90: ldr      r1, [sp, #0xc]
0078ec94: mov      r0, r6
0078ec98: strb     r3, [sp, #0x40]
0078ec9c: bl       #0x78ad54
0078eca0: ldrb     ip, [sp, #0x398]
0078eca4: sxtb     sb, ip
0078eca8: b        #0x78e710
0078ecac: cmn      sb, #1
0078ecb0: ldreq    fp, [sp, #0x304]
0078ecb4: addne    r3, sl, #1
0078ecb8: moveq    r1, #0x2b
0078ecbc: moveq    r0, fp
0078ecc0: movne    r0, r3
0078ecc4: movne    r1, #0x2b
0078ecc8: movne    fp, r3
0078eccc: bl       #0x30ec28
0078ecd0: cmp      r0, fp
0078ecd4: beq      #0x78ef30
0078ecd8: cmn      sb, #1
0078ecdc: ldreq    fp, [sp, #0x304]
0078ece0: addne    r3, sl, #1
0078ece4: moveq    r1, #0x2d
0078ece8: moveq    r0, fp
0078ecec: movne    r0, r3
0078ecf0: movne    r1, #0x2d
0078ecf4: movne    fp, r3
0078ecf8: bl       #0x30ec28
0078ecfc: cmp      r0, fp
0078ed00: beq      #0x78eeec
0078ed04: cmn      sb, #1
0078ed08: addne    r0, sl, #1
0078ed0c: ldreq    r0, [sp, #0x304]
0078ed10: bl       #0x30e094
0078ed14: cmp      r0, #0
0078ed18: ble      #0x78e9a8
0078ed1c: ldrb     lr, [sp, #0x2f8]
0078ed20: sxtb     r3, lr
0078ed24: cmn      r3, #1
0078ed28: addne    r0, sl, #1
0078ed2c: ldreq    r0, [sp, #0x304]
0078ed30: bl       #0x30e094
0078ed34: bl       #0x30e964
0078ed38: mov      r1, #0x41000000
0078ed3c: add      r1, r1, #0xa00000
0078ed40: bl       #0x30ed6c
0078ed44: bl       #0x30e4cc
0078ed48: str      r0, [sp, #0x38]
0078ed4c: b        #0x78e9a8
0078ed50: ldr      r1, [pc, #0x250]
0078ed54: mov      r0, sl
0078ed58: add      r1, pc, r1
0078ed5c: bl       #0x78aebc
0078ed60: cmp      r0, #0
0078ed64: ldrbeq   lr, [sp, #0x398]
0078ed68: sxtbeq   sb, lr
0078ed6c: beq      #0x78e710
0078ed70: ldr      r3, [sp, #0x2b8]
0078ed74: ldr      ip, [sp, #0x2e0]
0078ed78: ldr      r2, [sp, #0x2cc]
0078ed7c: mvn      r1, #0
0078ed80: bfi      r3, r1, #0, #0x18
0078ed84: lsr      r0, r3, #0x18
0078ed88: bfi      r2, r1, #0, #0x18
0078ed8c: bfi      ip, r1, #0, #0x18
0078ed90: bfi      r0, fp, #0, #1
0078ed94: add      r1, sp, #0x294
0078ed98: str      r1, [sp, #8]
0078ed9c: strb     r0, [sp, #0x28]
0078eda0: ldr      r1, [pc, #0x204]
0078eda4: str      ip, [sp, #0x2e0]
0078eda8: lsr      lr, ip, #0x18
0078edac: ldrb     ip, [sp, #0x28]
0078edb0: lsr      sl, r2, #0x18
0078edb4: add      sb, sp, #0x2d0
0078edb8: bfi      lr, fp, #0, #1
0078edbc: bfi      sl, fp, #0, #1
0078edc0: add      r1, pc, r1
0078edc4: str      sb, [sp, #0xc]
0078edc8: ldr      r0, [sp, #8]
0078edcc: mov      sb, #1
0078edd0: strb     lr, [sp, #0x2e3]
0078edd4: str      r3, [sp, #0x2b8]
0078edd8: str      r2, [sp, #0x2cc]
0078eddc: strb     ip, [sp, #0x2bb]
0078ede0: strb     sb, [sp, #0x2a8]
0078ede4: strb     sl, [sp, #0x2cf]
0078ede8: strb     sb, [sp, #0x2d0]
0078edec: strb     fp, [sp, #0x2d1]
0078edf0: strb     sb, [sp, #0x2bc]
0078edf4: strb     fp, [sp, #0x2bd]
0078edf8: strb     fp, [sp, #0x2a9]
0078edfc: bl       #0x413a7c
0078ee00: ldr      r2, [sp, #0xc]
0078ee04: ldr      r1, [sp, #8]
0078ee08: mov      r0, r7
0078ee0c: bl       #0x78dfbc
0078ee10: ldr      r0, [sp, #8]
0078ee14: bl       #0x41fed8
0078ee18: ldr      r1, [pc, #0x190]
0078ee1c: add      sl, sp, #0x280
0078ee20: add      fp, sp, #0x2bc
0078ee24: mov      r0, sl
0078ee28: add      r1, pc, r1
0078ee2c: bl       #0x413a7c
0078ee30: mov      r2, fp
0078ee34: mov      r1, sl
0078ee38: mov      r0, r7
0078ee3c: bl       #0x78dfbc
0078ee40: mov      r0, sl
0078ee44: bl       #0x41fed8
0078ee48: ldr      r1, [pc, #0x164]
0078ee4c: add      sl, sp, #0x26c
0078ee50: add      sb, sp, #0x2a8
0078ee54: mov      r0, sl
0078ee58: add      r1, pc, r1
0078ee5c: bl       #0x413a7c
0078ee60: mov      r1, sl
0078ee64: mov      r2, sb
0078ee68: mov      r0, r7
0078ee6c: bl       #0x78dfbc
0078ee70: mov      r0, sl
0078ee74: bl       #0x41fed8
0078ee78: ldrb     r0, [sp, #0x2bc]
0078ee7c: sxtb     r3, r0
0078ee80: cmn      r3, #1
0078ee84: addne    r0, fp, #1
0078ee88: ldreq    r0, [sp, #0x2c8]
0078ee8c: bl       #0x30e094
0078ee90: ldrb     r3, [sp, #0x2a8]
0078ee94: mov      sl, r0
0078ee98: cmp      r3, #0xff
0078ee9c: addne    r0, sb, #1
0078eea0: ldreq    r0, [sp, #0x2b4]
0078eea4: bl       #0x30e094
0078eea8: ldr      r1, [sp, #0xc]
0078eeac: mov      r3, r0
0078eeb0: mov      r2, sl
0078eeb4: mov      r0, r4
0078eeb8: bl       #0x78a990
0078eebc: mov      r0, sb
0078eec0: bl       #0x41fed8
0078eec4: mov      r0, fp
0078eec8: bl       #0x41fed8
0078eecc: ldr      r0, [sp, #0xc]
0078eed0: bl       #0x41fed8
0078eed4: ldrb     r1, [sp, #0x398]
0078eed8: sxtb     sb, r1
0078eedc: b        #0x78e710
0078eee0: mov      r0, #0xff000000
0078eee4: b        #0x78ea58
0078eee8: bl       #0x30e310
0078eeec: cmn      sb, #1
0078eef0: ldr      r2, [r6, #4]
0078eef4: ldreq    r0, [sp, #0x304]
0078eef8: ldr      r3, [r6]
0078eefc: addne    r0, sl, #1
0078ef00: add      r0, r0, #1
0078ef04: add      r3, r3, r2, lsl #4
0078ef08: ldr      sb, [r3, #-0xc]
0078ef0c: bl       #0x30e094
0078ef10: bl       #0x30e964
0078ef14: mov      r1, #0x41000000
0078ef18: add      r1, r1, #0xa00000
0078ef1c: bl       #0x30ed6c
0078ef20: bl       #0x30e4cc
0078ef24: rsb      r0, r0, sb
0078ef28: str      r0, [sp, #0x38]
0078ef2c: b        #0x78e9a8
0078ef30: cmn      sb, #1
0078ef34: ldr      r2, [r6, #4]
0078ef38: ldreq    r0, [sp, #0x304]
0078ef3c: ldr      r3, [r6]
0078ef40: addne    r0, sl, #1
0078ef44: add      r0, r0, #1
0078ef48: add      r3, r3, r2, lsl #4
0078ef4c: ldr      sb, [r3, #-0xc]
0078ef50: bl       #0x30e094
0078ef54: bl       #0x30e964
0078ef58: mov      r1, #0x41000000
0078ef5c: add      r1, r1, #0xa00000
0078ef60: bl       #0x30ed6c
0078ef64: bl       #0x30e4cc
0078ef68: add      r0, r0, sb
0078ef6c: str      r0, [sp, #0x38]
0078ef70: b        #0x78e9a8
0078ef74: eoreq    r6, r0, ip, lsr #14
0078ef78: andeq    r4, r0, ip, lsr #1
0078ef7c: ldrsbeq  r2, [r5], -r4
0078ef80: andseq   ip, r7, r4, lsl sp
0078ef84: andseq   sl, r3, ip, lsl sp
0078ef88: andseq   fp, r7, r8, lsl fp
0078ef8c: andseq   sp, r7, r8, asr #8
0078ef90: andseq   sp, r3, ip, ror r2
0078ef94: andseq   fp, r7, r8, asr r7
0078ef98: andseq   r4, r5, ip, ror #5
0078ef9c: ldrsheq  r6, [r3], -r4
0078efa0: andeq    r3, r0, r0, ror #13
0078efa4: andseq   r8, r5, ip, asr #18
0078efa8: andseq   fp, r7, r0, lsl r2
0078efac: ldrheq   fp, [r7], -r0
0078efb0: andseq   pc, r4, r0, lsl #27
0078efb4: ldrsbeq  sl, [r7], -r8

# _ZN7gameswf19edit_text_character12append_imageERKNS_9tu_stringEii
0078a990: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078a994: sub      sp, sp, #0x64
0078a998: ldr      ip, [r0]
0078a99c: mov      r4, r0
0078a9a0: mov      r5, r2
0078a9a4: mov      r6, r3
0078a9a8: mov      sl, r1
0078a9ac: mov      lr, pc
0078a9b0: ldr      pc, [ip, #0x84]
0078a9b4: ldr      r7, [pc, #0x38c]
0078a9b8: subs     r8, r0, #0
0078a9bc: add      r7, pc, r7
0078a9c0: beq      #0x78a9dc
0078a9c4: ldr      r3, [r8]
0078a9c8: mov      r1, #0x21
0078a9cc: mov      lr, pc
0078a9d0: ldr      pc, [r3, #8]
0078a9d4: cmp      r0, #0
0078a9d8: bne      #0x78acf8
0078a9dc: ldr      r3, [pc, #0x368]
0078a9e0: ldr      r3, [r7, r3]
0078a9e4: ldr      r3, [r3]
0078a9e8: cmp      r3, #0
0078a9ec: beq      #0x78acf0
0078a9f0: ldrsb    r2, [sl]
0078a9f4: mov      r1, r5
0078a9f8: cmn      r2, #1
0078a9fc: addne    r0, sl, #1
0078aa00: ldreq    r0, [sl, #0xc]
0078aa04: mov      r2, r6
0078aa08: blx      r3
0078aa0c: subs     r1, r0, #0
0078aa10: beq      #0x78acf0
0078aa14: ldr      r3, [pc, #0x334]
0078aa18: ldr      r3, [r7, r3]
0078aa1c: ldr      r3, [r3]
0078aa20: mov      r0, r3
0078aa24: ldr      r3, [r3]
0078aa28: mov      lr, pc
0078aa2c: ldr      pc, [r3, #0x18]
0078aa30: cmp      r5, #0
0078aa34: mov      r7, r0
0078aa38: ble      #0x78ad14
0078aa3c: cmp      r6, #0
0078aa40: ble      #0x78ad30
0078aa44: add      r3, sp, #0x3c
0078aa48: add      r0, r3, #4
0078aa4c: str      r3, [sp, #4]
0078aa50: mov      r3, #0x44000000
0078aa54: mvn      r8, #0
0078aa58: mov      r1, r7
0078aa5c: str      r3, [sp, #0x3c]
0078aa60: mov      r7, #0
0078aa64: mov      r3, #2
0078aa68: strb     r3, [sp, #0x5e]
0078aa6c: str      r7, [sp, #0x40]
0078aa70: str      r7, [sp, #0x54]
0078aa74: strh     r7, [sp, #0x58]
0078aa78: strh     r8, [sp, #0x5a]
0078aa7c: strh     r7, [sp, #0x5c]
0078aa80: bl       #0x77a740
0078aa84: mov      r0, r5
0078aa88: bl       #0x30e964
0078aa8c: mov      r1, #0x41000000
0078aa90: add      r1, r1, #0xa00000
0078aa94: bl       #0x30ed6c
0078aa98: mov      r5, #0
0078aa9c: mov      r3, #0x400
0078aaa0: mov      fp, r0
0078aaa4: mov      r0, r6
0078aaa8: strh     r3, [sp, #0x58]
0078aaac: str      r5, [sp, #0x44]
0078aab0: str      r5, [sp, #0x4c]
0078aab4: str      fp, [sp, #0x3c]
0078aab8: strh     r8, [sp, #0x5c]
0078aabc: str      fp, [sp, #0x48]
0078aac0: bl       #0x30e964
0078aac4: mov      r1, #0x41000000
0078aac8: add      r1, r1, #0xa00000
0078aacc: bl       #0x30ed6c
0078aad0: ldr      r1, [r4, #0x160]
0078aad4: str      r0, [sp, #0x50]
0078aad8: bl       #0x30eba4
0078aadc: ldr      r1, [r4, #0xa8]
0078aae0: mov      sb, r0
0078aae4: mov      r0, #0x3f800000
0078aae8: cmp      r1, r7
0078aaec: str      r0, [sp, #0x24]
0078aaf0: mov      r0, #1
0078aaf4: str      r5, [sp, #0x20]
0078aaf8: str      r5, [sp, #0x1c]
0078aafc: str      r8, [sp, #0xc]
0078ab00: strb     r8, [sp, #0x17]
0078ab04: strb     r0, [sp, #0x2a]
0078ab08: str      r7, [sp, #0x34]
0078ab0c: strb     r7, [sp, #0x38]
0078ab10: str      r7, [sp, #0x10]
0078ab14: strb     r8, [sp, #0x14]
0078ab18: strb     r8, [sp, #0x15]
0078ab1c: strb     r8, [sp, #0x16]
0078ab20: strb     r7, [sp, #0x18]
0078ab24: strb     r7, [sp, #0x28]
0078ab28: strb     r7, [sp, #0x29]
0078ab2c: str      r7, [sp, #0x2c]
0078ab30: str      r7, [sp, #0x30]
0078ab34: strle    sb, [sp, #0x20]
0078ab38: addle    r5, sp, #0xc
0078ab3c: ble      #0x78ac3c
0078ab40: mov      r3, #0x30
0078ab44: add      r1, r1, r8
0078ab48: ldr      r7, [r4, #0xa4]
0078ab4c: mul      r1, r3, r1
0078ab50: add      r5, sp, #0x60
0078ab54: ldr      r3, [r7, r1]
0078ab58: add      r7, r7, r1
0078ab5c: str      r3, [r5, #-0x54]!
0078ab60: add      r0, r5, #4
0078ab64: ldr      r1, [r7, #4]
0078ab68: bl       #0x764234
0078ab6c: ldr      r3, [r7, #8]
0078ab70: mov      r0, sb
0078ab74: str      r3, [sp, #0x14]
0078ab78: ldrb     r3, [r7, #0xc]
0078ab7c: strb     r3, [sp, #0x18]
0078ab80: ldr      r3, [r7, #0x10]
0078ab84: str      r3, [sp, #0x1c]
0078ab88: ldr      r6, [r7, #0x14]
0078ab8c: str      r6, [sp, #0x20]
0078ab90: ldr      r3, [r7, #0x18]
0078ab94: mov      r1, r6
0078ab98: str      r3, [sp, #0x24]
0078ab9c: ldrb     r3, [r7, #0x1c]
0078aba0: strb     r3, [sp, #0x28]
0078aba4: ldrb     r3, [r7, #0x1d]
0078aba8: strb     r3, [sp, #0x29]
0078abac: ldrb     r3, [r7, #0x1e]
0078abb0: strb     r3, [sp, #0x2a]
0078abb4: bl       #0x30e2f8
0078abb8: cmp      r0, #0
0078abbc: beq      #0x78ac3c
0078abc0: ldr      sl, [r4, #0xa8]
0078abc4: adds     r7, sl, r8
0078abc8: bmi      #0x78ac38
0078abcc: mov      r3, #0x30
0078abd0: mul      r7, r3, r7
0078abd4: ldr      r8, [r4, #0xa4]
0078abd8: mov      r0, r6
0078abdc: add      r8, r8, r7
0078abe0: ldr      r1, [r8, #0x14]
0078abe4: bl       #0x30df8c
0078abe8: cmp      r0, #0
0078abec: beq      #0x78ac38
0078abf0: sub      r6, sl, #2
0078abf4: mov      r3, #0x30
0078abf8: mul      r6, r3, r6
0078abfc: b        #0x78ac04
0078ac00: add      r8, r8, r7
0078ac04: cmp      sl, #1
0078ac08: str      sb, [r8, #0x14]
0078ac0c: beq      #0x78ac38
0078ac10: ldr      r8, [r4, #0xa4]
0078ac14: ldr      r1, [sp, #0x20]
0078ac18: sub      r7, r7, #0x30
0078ac1c: add      r3, r8, r6
0078ac20: ldr      r0, [r3, #0x14]
0078ac24: bl       #0x30df8c
0078ac28: cmp      r0, #0
0078ac2c: sub      sl, sl, #1
0078ac30: sub      r6, r6, #0x30
0078ac34: bne      #0x78ac00
0078ac38: str      sb, [sp, #0x20]
0078ac3c: ldr      r1, [r4, #0x188]
0078ac40: ldr      r0, [r4, #0x180]
0078ac44: bl       #0x30eba4
0078ac48: mov      r1, #0
0078ac4c: mov      r6, r0
0078ac50: bl       #0x30e2f8
0078ac54: cmp      r0, #0
0078ac58: moveq    r6, #0
0078ac5c: ldr      r0, [r4, #0x15c]
0078ac60: mov      r1, r6
0078ac64: bl       #0x30eba4
0078ac68: mov      r1, #0
0078ac6c: str      r0, [sp, #0x1c]
0078ac70: add      r0, r5, #4
0078ac74: bl       #0x764234
0078ac78: mov      lr, #0x44000000
0078ac7c: ldr      r0, [r4, #0x15c]
0078ac80: mvn      r3, #0
0078ac84: mov      ip, #0
0078ac88: mov      r2, #1
0078ac8c: add      lr, lr, #0x800000
0078ac90: mov      r1, fp
0078ac94: strb     r3, [sp, #0x17]
0078ac98: str      lr, [sp, #0x24]
0078ac9c: strb     r2, [sp, #0x29]
0078aca0: strb     ip, [sp, #0x2a]
0078aca4: strb     r3, [sp, #0x14]
0078aca8: strb     r3, [sp, #0x15]
0078acac: strb     r3, [sp, #0x16]
0078acb0: strb     ip, [sp, #0x18]
0078acb4: strb     r2, [sp, #0x28]
0078acb8: bl       #0x30eba4
0078acbc: str      r0, [r4, #0x15c]
0078acc0: ldr      r1, [sp, #4]
0078acc4: add      r0, r5, #0x20
0078acc8: bl       #0x78a81c
0078accc: add      r0, r4, #0xa4
0078acd0: mov      r1, r5
0078acd4: bl       #0x78a948
0078acd8: mov      r0, r5
0078acdc: bl       #0x78a5b8
0078ace0: ldr      r0, [sp, #0x40]
0078ace4: cmp      r0, #0
0078ace8: beq      #0x78acf0
0078acec: bl       #0x75a240
0078acf0: add      sp, sp, #0x64
0078acf4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078acf8: mov      r0, r8
0078acfc: ldr      r3, [r8]
0078ad00: mov      lr, pc
0078ad04: ldr      pc, [r3, #0x2c]
0078ad08: cmp      r5, #0
0078ad0c: mov      r7, r0
0078ad10: bgt      #0x78aa3c
0078ad14: ldr      r3, [r7]
0078ad18: mov      r0, r7
0078ad1c: mov      lr, pc
0078ad20: ldr      pc, [r3, #0x24]
0078ad24: cmp      r6, #0
0078ad28: mov      r5, r0
0078ad2c: bgt      #0x78aa44
0078ad30: ldr      r3, [r7]
0078ad34: mov      r0, r7
0078ad38: mov      lr, pc
0078ad3c: ldr      pc, [r3, #0x28]
0078ad40: mov      r6, r0
0078ad44: b        #0x78aa44

# _ZN7gameswf11html_reader9parse_tagERNS_12stringi_hashINS_9tu_stringEEEPKc
0078e060: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078e064: ldr      r0, [pc, #0x2dc]
0078e068: ldr      r3, [pc, #0x2dc]
0078e06c: sub      sp, sp, #0x94
0078e070: add      r0, pc, r0
0078e074: mov      r4, r2
0078e078: ldr      r2, [r0, r3]
0078e07c: str      r0, [sp, #8]
0078e080: str      r3, [sp, #0xc]
0078e084: ldrsb    r3, [r4]
0078e088: ldr      r2, [r2]
0078e08c: str      r1, [sp, #4]
0078e090: cmp      r3, #0x2f
0078e094: str      r2, [sp, #0x8c]
0078e098: moveq    r4, #0
0078e09c: beq      #0x78e248
0078e0a0: mov      r0, r4
0078e0a4: mov      r1, #0x20
0078e0a8: bl       #0x30ec28
0078e0ac: cmp      r0, #0
0078e0b0: beq      #0x78e320
0078e0b4: add      r5, sp, #0x78
0078e0b8: rsb      r2, r4, r0
0078e0bc: mov      r1, r4
0078e0c0: mov      r0, r5
0078e0c4: bl       #0x751eb4
0078e0c8: ldr      r1, [pc, #0x280]
0078e0cc: add      r7, sp, #0x64
0078e0d0: add      r6, sp, #0x50
0078e0d4: add      r1, pc, r1
0078e0d8: mov      r0, r7
0078e0dc: bl       #0x413a7c
0078e0e0: mov      r1, r7
0078e0e4: mov      r0, r6
0078e0e8: bl       #0x75302c
0078e0ec: ldr      r0, [sp, #4]
0078e0f0: mov      r1, r6
0078e0f4: mov      r2, r5
0078e0f8: bl       #0x78e010
0078e0fc: ldrsb    r3, [sp, #0x50]
0078e100: cmn      r3, #1
0078e104: beq      #0x78e2f8
0078e108: ldrsb    r3, [sp, #0x64]
0078e10c: cmn      r3, #1
0078e110: beq      #0x78e310
0078e114: mov      r0, r4
0078e118: mov      r1, #0x3d
0078e11c: bl       #0x30ec28
0078e120: subs     r6, r0, #0
0078e124: beq      #0x78e238
0078e128: mov      fp, r6
0078e12c: add      sl, sp, #0x3c
0078e130: add      r8, sp, #0x28
0078e134: add      sb, sp, #0x14
0078e138: ldrsb    r3, [r6]
0078e13c: cmp      r3, #0x20
0078e140: bne      #0x78e2e4
0078e144: ldrsb    r3, [fp, #-1]!
0078e148: cmp      r3, #0x20
0078e14c: beq      #0x78e144
0078e150: mov      r4, r6
0078e154: mov      r1, #0x22
0078e158: mov      r0, r6
0078e15c: bl       #0x30ec28
0078e160: mov      r1, #0x27
0078e164: mov      r7, r0
0078e168: mov      r0, r6
0078e16c: bl       #0x30ec28
0078e170: cmp      r0, #0
0078e174: cmpeq    r7, #0
0078e178: beq      #0x78e294
0078e17c: subs     r5, r0, #0
0078e180: movne    r5, #1
0078e184: cmp      r7, #0
0078e188: cmpne    r0, #0
0078e18c: beq      #0x78e270
0078e190: cmp      r7, r0
0078e194: movlo    r5, r7
0078e198: movhs    r5, r0
0078e19c: add      r6, r5, #1
0078e1a0: mov      r0, r6
0078e1a4: ldrsb    r1, [r5]
0078e1a8: bl       #0x30ec28
0078e1ac: subs     r7, r0, #0
0078e1b0: beq      #0x78e294
0078e1b4: sub      r2, fp, #1
0078e1b8: rsb      r2, r4, r2
0078e1bc: add      r1, r4, #1
0078e1c0: mov      r0, sl
0078e1c4: bl       #0x751eb4
0078e1c8: sub      r2, r7, #1
0078e1cc: rsb      r2, r5, r2
0078e1d0: mov      r1, r6
0078e1d4: mov      r0, r8
0078e1d8: bl       #0x751eb4
0078e1dc: mov      r1, sl
0078e1e0: mov      r0, sb
0078e1e4: bl       #0x75302c
0078e1e8: ldr      r0, [sp, #4]
0078e1ec: mov      r1, sb
0078e1f0: mov      r2, r8
0078e1f4: bl       #0x78e010
0078e1f8: ldrsb    r3, [sp, #0x14]
0078e1fc: cmn      r3, #1
0078e200: beq      #0x78e2d4
0078e204: mov      r0, r7
0078e208: mov      r1, #0x3d
0078e20c: bl       #0x30ec28
0078e210: ldrsb    r3, [sp, #0x28]
0078e214: mov      r6, r0
0078e218: cmn      r3, #1
0078e21c: beq      #0x78e2c4
0078e220: ldrsb    r3, [sp, #0x3c]
0078e224: cmn      r3, #1
0078e228: beq      #0x78e2b4
0078e22c: cmp      r6, #0
0078e230: mov      fp, r6
0078e234: bne      #0x78e138
0078e238: ldrsb    r3, [sp, #0x78]
0078e23c: mov      r4, #1
0078e240: cmn      r3, #1
0078e244: beq      #0x78e2a4
0078e248: ldr      r0, [sp, #0xc]
0078e24c: ldr      r1, [sp, #8]
0078e250: ldr      r2, [sp, #0x8c]
0078e254: ldr      r3, [r1, r0]
0078e258: mov      r0, r4
0078e25c: ldr      r3, [r3]
0078e260: cmp      r2, r3
0078e264: bne      #0x78e344
0078e268: add      sp, sp, #0x94
0078e26c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078e270: cmp      r5, #0
0078e274: movne    r5, r0
0078e278: moveq    r5, r7
0078e27c: add      r6, r5, #1
0078e280: mov      r0, r6
0078e284: ldrsb    r1, [r5]
0078e288: bl       #0x30ec28
0078e28c: subs     r7, r0, #0
0078e290: bne      #0x78e1b4
0078e294: ldrsb    r3, [sp, #0x78]
0078e298: mov      r4, #0
0078e29c: cmn      r3, #1
0078e2a0: bne      #0x78e248
0078e2a4: ldr      r0, [sp, #0x84]
0078e2a8: ldr      r1, [sp, #0x80]
0078e2ac: bl       #0x752b38
0078e2b0: b        #0x78e248
0078e2b4: ldr      r0, [sp, #0x48]
0078e2b8: ldr      r1, [sp, #0x44]
0078e2bc: bl       #0x752b38
0078e2c0: b        #0x78e22c
0078e2c4: ldr      r0, [sp, #0x34]
0078e2c8: ldr      r1, [sp, #0x30]
0078e2cc: bl       #0x752b38
0078e2d0: b        #0x78e220
0078e2d4: ldr      r0, [sp, #0x20]
0078e2d8: ldr      r1, [sp, #0x1c]
0078e2dc: bl       #0x752b38
0078e2e0: b        #0x78e204
0078e2e4: mov      r4, r6
0078e2e8: ldrsb    r3, [r4, #-1]!
0078e2ec: cmp      r3, #0x20
0078e2f0: bne      #0x78e2e8
0078e2f4: b        #0x78e154
0078e2f8: ldr      r0, [sp, #0x5c]
0078e2fc: ldr      r1, [sp, #0x58]
0078e300: bl       #0x752b38
0078e304: ldrsb    r3, [sp, #0x64]
0078e308: cmn      r3, #1
0078e30c: bne      #0x78e114
0078e310: ldr      r0, [sp, #0x70]
0078e314: ldr      r1, [sp, #0x6c]
0078e318: bl       #0x752b38
0078e31c: b        #0x78e114
0078e320: mov      r0, r4
0078e324: mov      r1, #0x2f
0078e328: bl       #0x30ec28
0078e32c: cmp      r0, #0
0078e330: bne      #0x78e0b4
0078e334: mov      r0, r4
0078e338: bl       #0x30de54
0078e33c: add      r0, r4, r0
0078e340: b        #0x78e0b4
0078e344: bl       #0x30e310
0078e348: eoreq    r6, r0, r0, lsr #20
0078e34c: andeq    r4, r0, ip, lsr #1
0078e350: andseq   r3, r5, r4, lsl r0

# _ZN7gameswf19edit_text_character10align_lineENS_23edit_text_character_def9alignmentEif
0078a398: push     {r4, r5, r6, r7, r8, lr}
0078a39c: mov      r4, r0
0078a3a0: ldr      r0, [r0, #0xa0]
0078a3a4: subs     r5, r1, #0
0078a3a8: mov      r6, r2
0078a3ac: ldr      r1, [r0, #0x24]
0078a3b0: mov      r7, r3
0078a3b4: ldr      r0, [r0, #0x28]
0078a3b8: ldr      r8, [r4, #0x184]
0078a3bc: beq      #0x78a450
0078a3c0: bl       #0x30e3ac
0078a3c4: mov      r1, r8
0078a3c8: bl       #0x30e3ac
0078a3cc: mov      r1, r7
0078a3d0: bl       #0x30e3ac
0078a3d4: mov      r1, #0x42000000
0078a3d8: add      r1, r1, #0xa00000
0078a3dc: bl       #0x30e3ac
0078a3e0: cmp      r5, #2
0078a3e4: mov      r8, r0
0078a3e8: beq      #0x78a454
0078a3ec: cmp      r5, #1
0078a3f0: movne    r8, #0
0078a3f4: ldr      r2, [r4, #0xa8]
0078a3f8: cmp      r6, r2
0078a3fc: bge      #0x78a440
0078a400: mov      r5, #0x30
0078a404: mul      r5, r5, r6
0078a408: ldr      r7, [r4, #0xa4]
0078a40c: mov      r1, r8
0078a410: add      r6, r6, #1
0078a414: add      r7, r7, r5
0078a418: ldrb     r3, [r7, #0x1c]
0078a41c: add      r5, r5, #0x30
0078a420: cmp      r3, #0
0078a424: beq      #0x78a438
0078a428: ldr      r0, [r7, #0x10]
0078a42c: bl       #0x30eba4
0078a430: str      r0, [r7, #0x10]
0078a434: ldr      r2, [r4, #0xa8]
0078a438: cmp      r6, r2
0078a43c: blt      #0x78a408
0078a440: ldr      r0, [r4, #0x154]
0078a444: mov      r1, r8
0078a448: bl       #0x30eba4
0078a44c: str      r0, [r4, #0x154]
0078a450: pop      {r4, r5, r6, r7, r8, pc}
0078a454: mov      r1, #0x3f000000
0078a458: bl       #0x30ed6c
0078a45c: mov      r8, r0
0078a460: b        #0x78a3f4

# _ZN7gameswf19edit_text_character18reset_bounding_boxEff
0078a370: str      r2, [r0, #0x134]
0078a374: str      r1, [r0, #0x12c]
0078a378: str      r1, [r0, #0x128]
0078a37c: str      r2, [r0, #0x130]
0078a380: bx       lr

# _ZN7gameswf19edit_text_character11append_textERKNS_9tu_stringERNS0_15text_attributesEb
0078cb90: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0078cb94: mov      r4, r0
0078cb98: sub      sp, sp, #0xa4
0078cb9c: ldr      r0, [r2, #4]
0078cba0: mov      r7, r2
0078cba4: str      r3, [sp, #0x34]
0078cba8: mov      fp, r1
0078cbac: bl       #0x30e964
0078cbb0: ldr      r3, [r4, #0x30]
0078cbb4: mov      r5, r0
0078cbb8: cmp      r3, #0
0078cbbc: beq      #0x78cbd0
0078cbc0: ldr      r0, [r4, #0x2c]
0078cbc4: ldrb     r2, [r0, #4]
0078cbc8: cmp      r2, #0
0078cbcc: beq      #0x78d69c
0078cbd0: ldr      r3, [r3, #0xac]
0078cbd4: mov      r1, #0x44000000
0078cbd8: add      r1, r1, #0x800000
0078cbdc: ldr      r3, [r3, #0xc]
0078cbe0: ldr      r0, [r3, #4]
0078cbe4: bl       #0x30ed6c
0078cbe8: mov      r1, r0
0078cbec: mov      r0, r5
0078cbf0: bl       #0x30ec94
0078cbf4: str      r0, [sp, #4]
0078cbf8: ldr      r5, [r7]
0078cbfc: ldr      r3, [r5, #0x7c]
0078cc00: cmp      r3, #0
0078cc04: beq      #0x78cc18
0078cc08: mov      r1, #0x41000000
0078cc0c: add      r1, r1, #0xa00000
0078cc10: bl       #0x30ec94
0078cc14: str      r0, [sp, #4]
0078cc18: mov      r0, r5
0078cc1c: bl       #0x7cf41c
0078cc20: mov      r6, r0
0078cc24: ldr      r0, [r7, #4]
0078cc28: bl       #0x30e964
0078cc2c: mov      r5, r0
0078cc30: ldr      r0, [r7]
0078cc34: bl       #0x7cf520
0078cc38: mov      r1, r6
0078cc3c: bl       #0x30ec94
0078cc40: mov      r1, r0
0078cc44: mov      r0, r5
0078cc48: bl       #0x30ed6c
0078cc4c: mov      r1, #0
0078cc50: str      r0, [sp, #0x24]
0078cc54: bl       #0x30df8c
0078cc58: cmp      r0, #0
0078cc5c: beq      #0x78cc6c
0078cc60: ldr      r0, [r7, #4]
0078cc64: bl       #0x30e964
0078cc68: str      r0, [sp, #0x24]
0078cc6c: mov      ip, #0x3f800000
0078cc70: ldr      r5, [r7]
0078cc74: ldr      r0, [r7, #4]
0078cc78: mov      r3, #0
0078cc7c: mvn      r2, #0
0078cc80: mov      r1, #0
0078cc84: str      ip, [sp, #0x60]
0078cc88: mov      ip, #1
0078cc8c: strb     r3, [sp, #0x74]
0078cc90: str      r3, [sp, #0x4c]
0078cc94: strb     r3, [sp, #0x54]
0078cc98: strb     r3, [sp, #0x64]
0078cc9c: strb     r3, [sp, #0x65]
0078cca0: str      r3, [sp, #0x68]
0078cca4: str      r3, [sp, #0x6c]
0078cca8: str      r3, [sp, #0x70]
0078ccac: strb     r2, [sp, #0x53]
0078ccb0: strb     ip, [sp, #0x66]
0078ccb4: str      r2, [sp, #0x48]
0078ccb8: strb     r2, [sp, #0x50]
0078ccbc: strb     r2, [sp, #0x51]
0078ccc0: strb     r2, [sp, #0x52]
0078ccc4: str      r1, [sp, #0x5c]
0078ccc8: str      r1, [sp, #0x58]
0078cccc: bl       #0x30e964
0078ccd0: ldr      r1, [r4, #0x160]
0078ccd4: bl       #0x30eba4
0078ccd8: ldr      r1, [r5, #0x58]
0078ccdc: mov      r6, r0
0078cce0: ldr      r0, [r5, #0x5c]
0078cce4: bl       #0x30e3ac
0078cce8: ldr      r1, [sp, #4]
0078ccec: bl       #0x30ed6c
0078ccf0: mov      r1, r0
0078ccf4: mov      r0, r6
0078ccf8: bl       #0x30eba4
0078ccfc: ldr      r3, [r4, #0xa8]
0078cd00: mov      sl, r0
0078cd04: cmp      r3, #0
0078cd08: ble      #0x78d610
0078cd0c: mov      r1, #0x30
0078cd10: sub      r3, r3, #1
0078cd14: ldr      r6, [r4, #0xa4]
0078cd18: mul      r3, r1, r3
0078cd1c: add      r2, sp, #0xa0
0078cd20: str      r2, [sp, #8]
0078cd24: ldr      r2, [r6, r3]
0078cd28: add      r6, r6, r3
0078cd2c: ldr      r3, [sp, #8]
0078cd30: str      r2, [r3, #-0x58]!
0078cd34: str      r3, [sp, #8]
0078cd38: add      r0, r3, #4
0078cd3c: ldr      r1, [r6, #4]
0078cd40: bl       #0x764234
0078cd44: ldr      r3, [r6, #8]
0078cd48: mov      r0, sl
0078cd4c: str      r3, [sp, #0x50]
0078cd50: ldrb     r3, [r6, #0xc]
0078cd54: strb     r3, [sp, #0x54]
0078cd58: ldr      r3, [r6, #0x10]
0078cd5c: str      r3, [sp, #0x58]
0078cd60: ldr      r5, [r6, #0x14]
0078cd64: str      r5, [sp, #0x5c]
0078cd68: ldr      r3, [r6, #0x18]
0078cd6c: mov      r1, r5
0078cd70: str      r3, [sp, #0x60]
0078cd74: ldrb     r3, [r6, #0x1c]
0078cd78: strb     r3, [sp, #0x64]
0078cd7c: ldrb     r3, [r6, #0x1d]
0078cd80: strb     r3, [sp, #0x65]
0078cd84: ldrb     r3, [r6, #0x1e]
0078cd88: strb     r3, [sp, #0x66]
0078cd8c: bl       #0x30e2f8
0078cd90: cmp      r0, #0
0078cd94: bne      #0x78d590
0078cd98: ldr      r5, [r7]
0078cd9c: ldr      r1, [r4, #0x188]
0078cda0: ldr      r0, [r4, #0x180]
0078cda4: bl       #0x30eba4
0078cda8: mov      r1, #0
0078cdac: mov      r6, r0
0078cdb0: bl       #0x30e2f8
0078cdb4: ldr      r2, [sp, #8]
0078cdb8: cmp      r0, #0
0078cdbc: moveq    r6, #0
0078cdc0: mov      r1, r5
0078cdc4: add      r0, r2, #4
0078cdc8: str      r6, [sp, #0x58]
0078cdcc: bl       #0x764234
0078cdd0: ldr      r2, [r7, #8]
0078cdd4: ldrb     r3, [r7, #0xc]
0078cdd8: ldr      r0, [r7, #4]
0078cddc: str      r2, [sp, #0x50]
0078cde0: strb     r3, [sp, #0x54]
0078cde4: bl       #0x30e964
0078cde8: str      r0, [sp, #0x60]
0078cdec: ldr      r1, [r4, #0x15c]
0078cdf0: mov      r3, #1
0078cdf4: ldr      r0, [sp, #0x58]
0078cdf8: strb     r3, [sp, #0x66]
0078cdfc: strb     r3, [sp, #0x64]
0078ce00: strb     r3, [sp, #0x65]
0078ce04: bl       #0x30eba4
0078ce08: ldr      r1, [sp, #0x5c]
0078ce0c: str      r0, [sp, #0x1c]
0078ce10: ldr      r3, [r7]
0078ce14: str      r0, [sp, #0x58]
0078ce18: str      r1, [sp, #0x28]
0078ce1c: ldr      r5, [r4, #0x18c]
0078ce20: ldr      r1, [r3, #0x5c]
0078ce24: ldr      r0, [sp, #4]
0078ce28: bl       #0x30ed6c
0078ce2c: mov      r1, r5
0078ce30: bl       #0x30eba4
0078ce34: ldr      r2, [sp, #0x1c]
0078ce38: str      r0, [sp, #0x30]
0078ce3c: mov      r1, #0
0078ce40: str      r2, [r4, #0x154]
0078ce44: ldr      r3, [sp, #0x28]
0078ce48: add      r2, sp, #0xa0
0078ce4c: mvn      sl, #0
0078ce50: str      r3, [r4, #0x158]
0078ce54: ldrsb    r3, [fp]
0078ce58: ldr      r6, [sp, #0x1c]
0078ce5c: cmn      r3, #1
0078ce60: ldr      r3, [pc, #0x85c]
0078ce64: ldreq    fp, [fp, #0xc]
0078ce68: addne    fp, fp, #1
0078ce6c: add      r3, pc, r3
0078ce70: str      r3, [sp, #0x2c]
0078ce74: ldr      r3, [pc, #0x84c]
0078ce78: str      r1, [sp, #0x14]
0078ce7c: str      fp, [r2, #-4]!
0078ce80: add      r3, pc, r3
0078ce84: str      r3, [sp, #0x38]
0078ce88: ldr      r3, [pc, #0x83c]
0078ce8c: ldr      r1, [sp, #0x28]
0078ce90: str      r2, [sp, #0x10]
0078ce94: add      r3, pc, r3
0078ce98: str      r3, [sp, #0x3c]
0078ce9c: ldr      r0, [sp, #0x10]
0078cea0: add      r3, r4, #0xa4
0078cea4: str      r3, [sp, #0x20]
0078cea8: str      r1, [sp, #0xc]
0078ceac: bl       #0x752494
0078ceb0: subs     r8, r0, #0
0078ceb4: beq      #0x78cf24
0078ceb8: mov      r2, r8
0078cebc: mov      r1, sl
0078cec0: ldr      r0, [r7]
0078cec4: bl       #0x7ce4a8
0078cec8: ldr      r1, [sp, #4]
0078cecc: bl       #0x30ed6c
0078ced0: mov      r1, r0
0078ced4: mov      r0, r6
0078ced8: bl       #0x30eba4
0078cedc: cmp      r8, #0xa
0078cee0: movne    r3, #0
0078cee4: moveq    r3, #1
0078cee8: cmp      r8, #0xa
0078ceec: cmpne    r8, #0xd
0078cef0: mov      r5, r8
0078cef4: mov      r6, r0
0078cef8: bne      #0x78d0a8
0078cefc: cmp      sl, #0xd
0078cf00: movne    sl, #0
0078cf04: andeq    sl, r3, #1
0078cf08: cmp      sl, #0
0078cf0c: beq      #0x78cfd8
0078cf10: mov      sl, r5
0078cf14: ldr      r0, [sp, #0x10]
0078cf18: bl       #0x752494
0078cf1c: subs     r8, r0, #0
0078cf20: bne      #0x78ceb8
0078cf24: ldr      r3, [r7]
0078cf28: ldr      r0, [sp, #4]
0078cf2c: ldr      r1, [r3, #0x5c]
0078cf30: bl       #0x30ed6c
0078cf34: mov      r1, r0
0078cf38: ldr      r0, [r4, #0x154]
0078cf3c: bl       #0x30eba4
0078cf40: str      r0, [r4, #0x154]
0078cf44: ldr      r0, [r7, #4]
0078cf48: bl       #0x30e964
0078cf4c: ldr      r5, [r7]
0078cf50: mov      r7, r0
0078cf54: ldr      r1, [r5, #0x58]
0078cf58: ldr      r0, [r5, #0x5c]
0078cf5c: bl       #0x30e3ac
0078cf60: ldr      r1, [sp, #4]
0078cf64: bl       #0x30ed6c
0078cf68: mov      r1, r0
0078cf6c: mov      r0, r7
0078cf70: bl       #0x30eba4
0078cf74: mov      r1, r0
0078cf78: ldr      r0, [r4, #0x158]
0078cf7c: bl       #0x30e3ac
0078cf80: str      r0, [r4, #0x158]
0078cf84: ldr      r0, [sp, #0x20]
0078cf88: ldr      r1, [sp, #8]
0078cf8c: bl       #0x78a948
0078cf90: ldr      r1, [sp, #0x1c]
0078cf94: mov      r0, r6
0078cf98: bl       #0x30e3ac
0078cf9c: mov      r1, r0
0078cfa0: ldr      r0, [r4, #0x15c]
0078cfa4: bl       #0x30eba4
0078cfa8: str      r0, [r4, #0x15c]
0078cfac: ldr      r1, [sp, #0x28]
0078cfb0: ldr      r0, [sp, #0xc]
0078cfb4: bl       #0x30e3ac
0078cfb8: mov      r1, r0
0078cfbc: ldr      r0, [r4, #0x160]
0078cfc0: bl       #0x30eba4
0078cfc4: str      r0, [r4, #0x160]
0078cfc8: ldr      r0, [sp, #8]
0078cfcc: bl       #0x78a5b8
0078cfd0: add      sp, sp, #0xa4
0078cfd4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0078cfd8: ldr      r1, [sp, #8]
0078cfdc: ldr      r0, [sp, #0x20]
0078cfe0: bl       #0x78a948
0078cfe4: mov      r3, r6
0078cfe8: ldr      r2, [r4, #0x164]
0078cfec: mov      r0, r4
0078cff0: ldr      r1, [r4, #0x17c]
0078cff4: bl       #0x78a398
0078cff8: ldr      r1, [r4, #0x188]
0078cffc: ldr      r0, [r4, #0x180]
0078d000: bl       #0x30eba4
0078d004: mov      r1, #0
0078d008: mov      r6, r0
0078d00c: bl       #0x30e2f8
0078d010: ldr      r1, [sp, #0x30]
0078d014: cmp      r0, #0
0078d018: ldr      r0, [sp, #0x24]
0078d01c: moveq    r6, #0
0078d020: bl       #0x30eba4
0078d024: mov      r1, r0
0078d028: ldr      r0, [sp, #0xc]
0078d02c: bl       #0x30eba4
0078d030: ldr      r2, [sp, #8]
0078d034: str      r0, [sp, #0xc]
0078d038: mov      r1, #0
0078d03c: add      r0, r2, #0x20
0078d040: bl       #0x78a4ec
0078d044: ldr      r3, [sp, #8]
0078d048: ldr      r1, [r7]
0078d04c: mov      sl, r5
0078d050: add      r0, r3, #4
0078d054: bl       #0x764234
0078d058: ldr      r2, [r7, #8]
0078d05c: ldrb     r3, [r7, #0xc]
0078d060: ldr      r1, [sp, #0xc]
0078d064: ldr      r0, [r7, #4]
0078d068: str      r2, [sp, #0x50]
0078d06c: strb     r3, [sp, #0x54]
0078d070: str      r1, [sp, #0x5c]
0078d074: str      r6, [sp, #0x58]
0078d078: bl       #0x30e964
0078d07c: ldr      r2, [r4, #0xa8]
0078d080: mov      r3, #1
0078d084: mvn      r1, #0
0078d088: str      r0, [sp, #0x60]
0078d08c: strb     r3, [sp, #0x66]
0078d090: str      r1, [r4, #0x16c]
0078d094: str      r2, [r4, #0x164]
0078d098: strb     r3, [sp, #0x64]
0078d09c: strb     r3, [sp, #0x65]
0078d0a0: str      r2, [r4, #0x168]
0078d0a4: b        #0x78cf14
0078d0a8: cmp      r8, #8
0078d0ac: beq      #0x78d334
0078d0b0: cmp      r8, #0x11
0078d0b4: moveq    r2, #0
0078d0b8: streq    r2, [sp, #0x18]
0078d0bc: beq      #0x78d314
0078d0c0: cmp      r8, #0x20
0078d0c4: beq      #0x78d30c
0078d0c8: cmp      r8, #0xa0
0078d0cc: bne      #0x78d520
0078d0d0: mov      r5, #0x20
0078d0d4: mov      r1, #0x3f800000
0078d0d8: mov      sb, r5
0078d0dc: str      r1, [sp, #0x18]
0078d0e0: mov      r8, r5
0078d0e4: mov      r3, #0x44000000
0078d0e8: ldr      r0, [r7, #4]
0078d0ec: mov      r1, #0
0078d0f0: mvn      r2, #0
0078d0f4: str      r3, [sp, #0x78]
0078d0f8: mov      r3, #0
0078d0fc: strh     r2, [sp, #0x96]
0078d100: strh     r3, [sp, #0x98]
0078d104: str      r1, [sp, #0x7c]
0078d108: str      r1, [sp, #0x90]
0078d10c: strh     r1, [sp, #0x94]
0078d110: strb     r1, [sp, #0x9a]
0078d114: bl       #0x30e964
0078d118: mov      r1, #0x41000000
0078d11c: add      r1, r1, #0xa00000
0078d120: bl       #0x30ec94
0078d124: bl       #0x30e4cc
0078d128: ldr      fp, [r7]
0078d12c: add      sl, sp, #0x78
0078d130: mov      r3, r0
0078d134: mov      r1, sl
0078d138: mov      r0, fp
0078d13c: mov      r2, sb
0078d140: bl       #0x7d01bc
0078d144: cmp      r0, #0
0078d148: bne      #0x78d184
0078d14c: ldr      r2, [sp, #0x2c]
0078d150: ldr      r3, [r2]
0078d154: cmp      r3, #9
0078d158: bgt      #0x78d184
0078d15c: add      r3, r3, #1
0078d160: str      r3, [r2]
0078d164: ldr      r2, [r7]
0078d168: ldr      r0, [sp, #0x38]
0078d16c: mov      r1, r8
0078d170: ldrsb    r3, [r2, #0x30]
0078d174: cmn      r3, #1
0078d178: addne    r2, r2, #0x31
0078d17c: ldreq    r2, [r2, #0x3c]
0078d180: bl       #0x761184
0078d184: ldr      r1, [r4, #0x190]
0078d188: ldr      r0, [sp, #0x78]
0078d18c: bl       #0x30eba4
0078d190: ldr      r1, [sp, #0x18]
0078d194: mov      fp, r0
0078d198: ldr      r0, [sp, #4]
0078d19c: bl       #0x30ed6c
0078d1a0: mov      r1, r0
0078d1a4: mov      r0, fp
0078d1a8: bl       #0x30ed6c
0078d1ac: cmp      r8, #0x1000
0078d1b0: str      r0, [sp, #0x78]
0078d1b4: bls      #0x78d1c8
0078d1b8: movw     r1, #0xcccd
0078d1bc: movt     r1, #0x3f8c
0078d1c0: bl       #0x30ed6c
0078d1c4: str      r0, [sp, #0x78]
0078d1c8: ldr      r0, [r7, #4]
0078d1cc: bl       #0x30e964
0078d1d0: mov      r1, #0x41000000
0078d1d4: add      r1, r1, #0xa00000
0078d1d8: bl       #0x30ec94
0078d1dc: bl       #0x30e4cc
0078d1e0: ldr      r3, [sp, #8]
0078d1e4: mov      r1, sl
0078d1e8: strh     r0, [sp, #0x94]
0078d1ec: add      r8, r3, #0x20
0078d1f0: mov      r0, r8
0078d1f4: strh     sb, [sp, #0x98]
0078d1f8: bl       #0x78a81c
0078d1fc: mov      r0, r6
0078d200: ldr      r1, [sp, #0x78]
0078d204: bl       #0x30eba4
0078d208: ldr      r3, [r4, #0xa0]
0078d20c: mov      sl, r0
0078d210: ldr      r1, [r3, #0x24]
0078d214: ldr      r0, [r3, #0x28]
0078d218: bl       #0x30e3ac
0078d21c: ldr      r1, [r4, #0x184]
0078d220: bl       #0x30e3ac
0078d224: mov      r1, #0x42000000
0078d228: add      r1, r1, #0xa00000
0078d22c: bl       #0x30e3ac
0078d230: mov      r1, sl
0078d234: bl       #0x30e9ac
0078d238: cmp      r0, #0
0078d23c: moveq    r6, sl
0078d240: bne      #0x78d36c
0078d244: ldr      r2, [sp, #0x14]
0078d248: ldr      r3, [r4, #0x150]
0078d24c: cmp      r2, r3
0078d250: strlt    r6, [r4, #0x154]
0078d254: ldrlt    r3, [sp, #0xc]
0078d258: strlt    r3, [r4, #0x158]
0078d25c: ldr      r1, [sp, #0x14]
0078d260: ldr      r3, [r7]
0078d264: ldr      r0, [sp, #4]
0078d268: add      r1, r1, #1
0078d26c: str      r1, [sp, #0x14]
0078d270: ldr      r1, [r3, #0x58]
0078d274: bl       #0x30ed6c
0078d278: ldr      r1, [sp, #0xc]
0078d27c: bl       #0x30eba4
0078d280: ldr      sl, [r4, #0x128]
0078d284: mov      r8, r0
0078d288: mov      r0, r6
0078d28c: mov      r1, sl
0078d290: bl       #0x30e2f8
0078d294: ldr      sb, [r4, #0x130]
0078d298: cmp      r0, #0
0078d29c: moveq    sl, r6
0078d2a0: mov      r1, sb
0078d2a4: str      sl, [r4, #0x128]
0078d2a8: mov      r0, r8
0078d2ac: bl       #0x30e2f8
0078d2b0: ldr      sl, [r4, #0x12c]
0078d2b4: cmp      r0, #0
0078d2b8: moveq    sb, r8
0078d2bc: mov      r1, sl
0078d2c0: str      sb, [r4, #0x130]
0078d2c4: mov      r0, r6
0078d2c8: bl       #0x30e2f8
0078d2cc: ldr      sb, [r4, #0x134]
0078d2d0: cmp      r0, #0
0078d2d4: movne    sl, r6
0078d2d8: mov      r0, r8
0078d2dc: str      sl, [r4, #0x12c]
0078d2e0: mov      r1, sb
0078d2e4: bl       #0x30e2f8
0078d2e8: cmp      r0, #0
0078d2ec: ldr      r0, [sp, #0x7c]
0078d2f0: moveq    r8, sb
0078d2f4: str      r8, [r4, #0x134]
0078d2f8: cmp      r0, #0
0078d2fc: beq      #0x78cf10
0078d300: bl       #0x75a240
0078d304: mov      sl, r5
0078d308: b        #0x78cf14
0078d30c: mov      r3, #0x3f800000
0078d310: str      r3, [sp, #0x18]
0078d314: ldr      r2, [sp, #0x6c]
0078d318: ldr      r3, [r4, #0xa8]
0078d31c: mov      r5, #0x20
0078d320: str      r2, [r4, #0x16c]
0078d324: str      r3, [r4, #0x168]
0078d328: mov      sb, r5
0078d32c: mov      r8, r5
0078d330: b        #0x78d0e4
0078d334: ldr      r8, [sp, #0x6c]
0078d338: cmp      r8, #0
0078d33c: ble      #0x78cf10
0078d340: mov      r3, #0x24
0078d344: sub      r8, r8, #1
0078d348: mul      r8, r3, r8
0078d34c: ldr      sl, [sp, #0x68]
0078d350: ldr      r1, [sl, r8]
0078d354: bl       #0x30e3ac
0078d358: mov      r3, #0
0078d35c: str      r3, [sl, r8]
0078d360: mov      r6, r0
0078d364: mov      sl, r5
0078d368: b        #0x78cf14
0078d36c: ldr      r0, [sp, #0x20]
0078d370: ldr      r1, [sp, #8]
0078d374: bl       #0x78a948
0078d378: ldr      r1, [sp, #0x30]
0078d37c: ldr      r0, [sp, #0x24]
0078d380: bl       #0x30eba4
0078d384: mov      r1, r0
0078d388: ldr      r0, [sp, #0xc]
0078d38c: bl       #0x30eba4
0078d390: mov      r1, #0
0078d394: str      r0, [sp, #0xc]
0078d398: mov      r0, r8
0078d39c: ldr      r6, [r4, #0x180]
0078d3a0: bl       #0x78a4ec
0078d3a4: ldr      r1, [sp, #8]
0078d3a8: add      r0, r1, #4
0078d3ac: ldr      r1, [r7]
0078d3b0: bl       #0x764234
0078d3b4: ldr      r2, [r7, #8]
0078d3b8: ldrb     r3, [r7, #0xc]
0078d3bc: ldr      r0, [r7, #4]
0078d3c0: str      r2, [sp, #0x50]
0078d3c4: ldr      r2, [sp, #0xc]
0078d3c8: strb     r3, [sp, #0x54]
0078d3cc: str      r6, [sp, #0x58]
0078d3d0: str      r2, [sp, #0x5c]
0078d3d4: bl       #0x30e964
0078d3d8: ldr      sb, [r4, #0xa8]
0078d3dc: ldr      ip, [r4, #0x16c]
0078d3e0: ldr      r1, [r4, #0xa4]
0078d3e4: mov      r3, #1
0078d3e8: sub      sb, sb, #1
0078d3ec: mov      r2, #0x30
0078d3f0: cmn      ip, #1
0078d3f4: str      r0, [sp, #0x60]
0078d3f8: strb     r3, [sp, #0x65]
0078d3fc: strb     r3, [sp, #0x64]
0078d400: mla      fp, r2, sb, r1
0078d404: beq      #0x78d620
0078d408: ldr      r8, [r4, #0x168]
0078d40c: mov      r3, #0x24
0078d410: mul      lr, r3, ip
0078d414: mla      r3, r2, r8, r1
0078d418: mov      r0, sl
0078d41c: ldr      r3, [r3, #0x20]
0078d420: ldr      r1, [r3, lr]
0078d424: str      ip, [sp]
0078d428: bl       #0x30e3ac
0078d42c: ldr      ip, [sp]
0078d430: ldr      r3, [fp, #0x24]
0078d434: cmp      r8, sb
0078d438: movne    sl, #0
0078d43c: addeq    sl, ip, #1
0078d440: cmp      sl, r3
0078d444: mov      r2, r0
0078d448: bge      #0x78d4d8
0078d44c: ldr      r1, [sp, #8]
0078d450: mov      r8, #0x24
0078d454: mul      r8, r8, sl
0078d458: add      r3, r1, #0x20
0078d45c: ldr      sb, [fp, #0x20]
0078d460: str      r5, [sp, #0x18]
0078d464: str      r7, [sp, #0x44]
0078d468: mov      r5, r0
0078d46c: str      r4, [sp, #0x40]
0078d470: mov      r7, r3
0078d474: add      r1, sb, r8
0078d478: mov      r0, r7
0078d47c: bl       #0x78a81c
0078d480: ldr      sb, [fp, #0x20]
0078d484: mov      r0, r6
0078d488: add      sl, sl, #1
0078d48c: ldr      r4, [sb, r8]
0078d490: add      r8, r8, #0x24
0078d494: mov      r1, r4
0078d498: bl       #0x30eba4
0078d49c: mov      r1, r4
0078d4a0: mov      r6, r0
0078d4a4: mov      r0, r5
0078d4a8: bl       #0x30e3ac
0078d4ac: ldr      r3, [fp, #0x24]
0078d4b0: mov      r5, r0
0078d4b4: cmp      sl, r3
0078d4b8: blt      #0x78d474
0078d4bc: ldr      r4, [sp, #0x40]
0078d4c0: ldr      r5, [sp, #0x18]
0078d4c4: ldr      r7, [sp, #0x44]
0078d4c8: ldr      sb, [r4, #0xa8]
0078d4cc: ldr      r8, [r4, #0x168]
0078d4d0: mov      r2, r0
0078d4d4: sub      sb, sb, #1
0078d4d8: cmp      sb, r8
0078d4dc: ldreq    r1, [r4, #0x16c]
0078d4e0: movne    r1, #0
0078d4e4: add      r0, fp, #0x20
0078d4e8: str      r2, [sp]
0078d4ec: bl       #0x78a4ec
0078d4f0: ldr      r2, [sp]
0078d4f4: mov      r3, r2
0078d4f8: mov      r0, r4
0078d4fc: ldr      r2, [r4, #0x164]
0078d500: ldr      r1, [r4, #0x17c]
0078d504: bl       #0x78a398
0078d508: ldr      r3, [r4, #0xa8]
0078d50c: mvn      r2, #0
0078d510: str      r2, [r4, #0x16c]
0078d514: str      r3, [r4, #0x164]
0078d518: str      r3, [r4, #0x168]
0078d51c: b        #0x78d244
0078d520: cmp      r8, #0x26
0078d524: movne    r2, #0x3f800000
0078d528: uxthne   sb, r8
0078d52c: strne    r2, [sp, #0x18]
0078d530: bne      #0x78d0e4
0078d534: ldr      r3, [sp, #0x34]
0078d538: cmp      r3, #0
0078d53c: beq      #0x78d57c
0078d540: ldr      r5, [sp, #0x9c]
0078d544: ldr      r1, [sp, #0x3c]
0078d548: mov      r2, #5
0078d54c: mov      r0, r5
0078d550: bl       #0x30ec7c
0078d554: cmp      r0, #0
0078d558: bne      #0x78d57c
0078d55c: add      r3, r5, #5
0078d560: mov      r1, #0x3f800000
0078d564: mov      r5, #0x20
0078d568: str      r3, [sp, #0x9c]
0078d56c: mov      sb, r5
0078d570: str      r1, [sp, #0x18]
0078d574: mov      r8, r5
0078d578: b        #0x78d0e4
0078d57c: mov      r2, #0x3f800000
0078d580: mov      r5, #0x26
0078d584: str      r2, [sp, #0x18]
0078d588: mov      sb, r5
0078d58c: b        #0x78d0e4
0078d590: ldr      r8, [r4, #0xa8]
0078d594: subs     r6, r8, #1
0078d598: bmi      #0x78d608
0078d59c: mov      r1, #0x30
0078d5a0: mul      r6, r1, r6
0078d5a4: ldr      sb, [r4, #0xa4]
0078d5a8: mov      r1, r5
0078d5ac: add      sb, sb, r6
0078d5b0: ldr      r0, [sb, #0x14]
0078d5b4: bl       #0x30df8c
0078d5b8: cmp      r0, #0
0078d5bc: beq      #0x78d608
0078d5c0: sub      r5, r8, #2
0078d5c4: mov      r2, #0x30
0078d5c8: mul      r5, r2, r5
0078d5cc: b        #0x78d5d4
0078d5d0: add      sb, sb, r6
0078d5d4: cmp      r8, #1
0078d5d8: str      sl, [sb, #0x14]
0078d5dc: beq      #0x78d608
0078d5e0: ldr      sb, [r4, #0xa4]
0078d5e4: ldr      r1, [sp, #0x5c]
0078d5e8: sub      r6, r6, #0x30
0078d5ec: add      r3, sb, r5
0078d5f0: ldr      r0, [r3, #0x14]
0078d5f4: bl       #0x30df8c
0078d5f8: cmp      r0, #0
0078d5fc: sub      r8, r8, #1
0078d600: sub      r5, r5, #0x30
0078d604: bne      #0x78d5d0
0078d608: str      sl, [sp, #0x5c]
0078d60c: b        #0x78cd98
0078d610: add      r3, sp, #0x48
0078d614: str      r0, [sp, #0x5c]
0078d618: str      r3, [sp, #8]
0078d61c: b        #0x78cd9c
0078d620: ldr      r1, [fp, #0x24]
0078d624: cmp      r1, #0
0078d628: movle    r2, sl
0078d62c: ble      #0x78d4f4
0078d630: ldr      r3, [fp, #0x20]
0078d634: mov      sb, #0x24
0078d638: sub      r1, r1, #1
0078d63c: mla      r1, sb, r1, r3
0078d640: mov      r0, r8
0078d644: bl       #0x78a81c
0078d648: ldr      r3, [fp, #0x24]
0078d64c: ldr      r2, [fp, #0x20]!
0078d650: mov      r0, r6
0078d654: sub      r3, r3, #1
0078d658: mul      sb, sb, r3
0078d65c: ldr      r8, [r2, sb]
0078d660: str      r3, [sp]
0078d664: mov      r1, r8
0078d668: bl       #0x30eba4
0078d66c: mov      r1, r8
0078d670: mov      r6, r0
0078d674: mov      r0, sl
0078d678: bl       #0x30e3ac
0078d67c: ldr      r3, [sp]
0078d680: mov      r2, r0
0078d684: mov      r0, fp
0078d688: mov      r1, r3
0078d68c: str      r2, [sp]
0078d690: bl       #0x78a4ec
0078d694: ldr      r2, [sp]
0078d698: b        #0x78d4f4
0078d69c: ldr      r1, [r0]
0078d6a0: sub      r1, r1, #1
0078d6a4: cmp      r1, #0
0078d6a8: str      r1, [r0]
0078d6ac: bne      #0x78d6b4
0078d6b0: bl       #0x752b38
0078d6b4: mov      r3, #0
0078d6b8: str      r3, [r4, #0x2c]
0078d6bc: str      r3, [r4, #0x30]
0078d6c0: b        #0x78cbd0
0078d6c4: eoreq    pc, sb, ip, asr #23
0078d6c8: andseq   sp, r7, r8, asr #32
0078d6cc: andseq   sp, r7, ip, lsr #32

# _ZN7gameswf19edit_text_character8set_textERKNS_9tu_stringEb
0078f1ec: push     {r4, r5, r6, r7, r8, lr}
0078f1f0: add      r6, r0, #0x138
0078f1f4: cmp      r6, r1
0078f1f8: mov      r4, r0
0078f1fc: mov      r5, r1
0078f200: mov      r7, r2
0078f204: beq      #0x78f234
0078f208: ldrb     r3, [r0, #0x138]
0078f20c: cmp      r3, #0xff
0078f210: ldrsb    r3, [r1]
0078f214: addne    r0, r6, #1
0078f218: ldreq    r0, [r4, #0x144]
0078f21c: cmn      r3, #1
0078f220: addne    r1, r1, #1
0078f224: ldreq    r1, [r5, #0xc]
0078f228: bl       #0x30e31c
0078f22c: cmp      r0, #0
0078f230: bne      #0x78f238
0078f234: pop      {r4, r5, r6, r7, r8, pc}
0078f238: mov      r1, r5
0078f23c: mov      r0, r6
0078f240: bl       #0x752f50
0078f244: ldr      r3, [r4, #0xa0]
0078f248: ldr      r1, [r3, #0x64]
0078f24c: cmp      r1, #0
0078f250: ble      #0x78f278
0078f254: ldrb     r3, [r4, #0x138]
0078f258: sxtb     r3, r3
0078f25c: cmn      r3, #1
0078f260: ldreq    r3, [r4, #0x13c]
0078f264: sub      r3, r3, #1
0078f268: cmp      r1, r3
0078f26c: bge      #0x78f278
0078f270: mov      r0, r6
0078f274: bl       #0x751d14
0078f278: mov      r0, r4
0078f27c: mov      r1, r7
0078f280: pop      {r4, r5, r6, r7, r8, lr}
0078f284: b        #0x78efb8
