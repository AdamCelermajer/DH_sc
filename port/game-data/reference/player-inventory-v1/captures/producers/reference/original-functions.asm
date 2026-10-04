
# _ZN8Savegame4loadEPKcPFvP11IStreamBasePvES6_S4_
00315848: push     {r4, r5, r6, r7, r8, sl, lr}
0031584c: sub      sp, sp, #0xc
00315850: add      r4, sp, #8
00315854: str      r1, [r4, #-4]!
00315858: add      r5, r0, #0x20
0031585c: mov      r6, r0
00315860: mov      r1, r4
00315864: mov      r0, r5
00315868: mov      sl, r3
0031586c: mov      r7, r2
00315870: ldr      r8, [sp, #0x28]
00315874: bl       #0x314120
00315878: cmp      r0, r5
0031587c: mov      r3, r0
00315880: beq      #0x3158d8
00315884: ldr      r2, [r0, #0x30]
00315888: str      sl, [r0, #0x38]
0031588c: str      r7, [r0, #0x34]
00315890: cmp      r2, #0
00315894: str      r8, [r0, #0x3c]
00315898: beq      #0x3158d0
0031589c: ldr      r1, [r6, #0x1c]
003158a0: cmp      r1, #0
003158a4: beq      #0x3158d0
003158a8: mov      r0, r1
003158ac: ldrd     r2, r3, [r3, #0x28]
003158b0: ldr      r1, [r1]
003158b4: mov      lr, pc
003158b8: ldr      pc, [r1, #0x20]
003158bc: cmp      r7, #0
003158c0: beq      #0x3158d0
003158c4: ldr      r0, [r6, #0x1c]
003158c8: mov      r1, r8
003158cc: blx      r7
003158d0: add      sp, sp, #0xc
003158d4: pop      {r4, r5, r6, r7, r8, sl, pc}
003158d8: mov      r1, r4
003158dc: bl       #0x3156f0
003158e0: mov      r3, #0
003158e4: mov      r2, #0
003158e8: strd     r2, r3, [r0]
003158ec: mov      r3, #0
003158f0: str      r8, [r0, #0x14]
003158f4: str      sl, [r0, #0x10]
003158f8: str      r7, [r0, #0xc]
003158fc: str      r3, [r0, #8]
00315900: b        #0x3158d0

# _ZNK9Character16GetCharSkillListEv
003bc5fc: ldr      r3, [pc, #0x20]
003bc600: ldr      r2, [pc, #0x20]
003bc604: push     {r4, lr}
003bc608: add      r3, pc, r3
003bc60c: ldr      r2, [r3, r2]
003bc610: ldr      r4, [r2]
003bc614: bl       #0x3bc5c0
003bc618: mov      r3, #0xc
003bc61c: mla      r0, r3, r0, r4
003bc620: pop      {r4, pc}
003bc624: subseq   r8, sp, r8, lsl #9
003bc628: andeq    r1, r0, r8, asr #3

# _ZN12ItemInstance11_UpdateNameEv
003fb754: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003fb758: ldr      r4, [pc, #0x4bc]
003fb75c: ldr      r3, [pc, #0x4bc]
003fb760: ldr      r6, [pc, #0x4bc]
003fb764: add      r4, pc, r4
003fb768: ldr      r3, [r4, r3]
003fb76c: ldr      r2, [r4, r6]
003fb770: mov      r7, r0
003fb774: ldr      r3, [r3]
003fb778: ldr      r0, [r0, #4]
003fb77c: ldr      r1, [pc, #0x4a4]
003fb780: mov      r5, #0xa4
003fb784: mla      r5, r5, r0, r3
003fb788: ldr      r3, [r2]
003fb78c: add      r1, pc, r1
003fb790: add      sl, r7, #8
003fb794: sub      sp, sp, #0xf4
003fb798: mov      r2, r1
003fb79c: mov      r0, sl
003fb7a0: str      r3, [sp, #0xec]
003fb7a4: bl       #0x3109e0
003fb7a8: ldr      r1, [r5, #0x44]
003fb7ac: cmn      r1, #1
003fb7b0: beq      #0x3fbacc
003fb7b4: ldr      r2, [pc, #0x470]
003fb7b8: ldr      r3, [r4, r2]
003fb7bc: str      r2, [sp, #4]
003fb7c0: ldr      r0, [r3, #0x34]
003fb7c4: bl       #0x508edc
003fb7c8: ldr      r3, [pc, #0x460]
003fb7cc: str      r0, [sp, #0x14]
003fb7d0: ldr      r3, [r4, r3]
003fb7d4: ldr      r1, [r3]
003fb7d8: bl       #0x30ebd4
003fb7dc: cmp      r0, #0
003fb7e0: str      r0, [sp, #8]
003fb7e4: movne    sb, #0
003fb7e8: movne    fp, #1
003fb7ec: beq      #0x3fbbc0
003fb7f0: ldr      r1, [r5, #0x48]
003fb7f4: cmn      r1, #1
003fb7f8: moveq    r5, #0
003fb7fc: beq      #0x3fb814
003fb800: ldr      r2, [sp, #4]
003fb804: ldr      r3, [r4, r2]
003fb808: ldr      r0, [r3, #0x34]
003fb80c: bl       #0x508edc
003fb810: mov      r5, r0
003fb814: ldr      r1, [pc, #0x418]
003fb818: add      r3, sp, #0xd4
003fb81c: mov      r0, r3
003fb820: add      r1, pc, r1
003fb824: add      r2, sp, #0x40
003fb828: str      r3, [sp]
003fb82c: bl       #0x3140ec
003fb830: cmp      r5, #0
003fb834: streq    r5, [sp, #0xc]
003fb838: beq      #0x3fb988
003fb83c: ldr      r3, [pc, #0x3f4]
003fb840: mov      r0, r5
003fb844: ldr      r3, [r4, r3]
003fb848: ldr      r1, [r3]
003fb84c: bl       #0x30ebd4
003fb850: cmp      r0, #0
003fb854: streq    r0, [sp, #0xc]
003fb858: moveq    r8, r0
003fb85c: beq      #0x3fb870
003fb860: subs     r1, r5, r0
003fb864: movne    r1, #1
003fb868: str      r1, [sp, #0xc]
003fb86c: rsb      r8, r5, r0
003fb870: cmp      sb, #0
003fb874: movne    sb, #2
003fb878: ldr      r3, [pc, #0x3bc]
003fb87c: cmp      fp, #0
003fb880: movne    r2, sb
003fb884: addne    r2, r2, #1
003fb888: mov      r0, r5
003fb88c: str      sb, [sp, #0x10]
003fb890: str      r3, [sp, #0x18]
003fb894: strne    r2, [sp, #0x10]
003fb898: bl       #0x30de54
003fb89c: mvn      r2, #0
003fb8a0: add      sb, sp, #0xf0
003fb8a4: mov      fp, r0
003fb8a8: add      r0, r2, #1
003fb8ac: str      r2, [sb, #-0xd0]!
003fb8b0: cmp      r2, #0
003fb8b4: cmpne    r0, fp
003fb8b8: add      r3, sb, #8
003fb8bc: movge    r1, #0
003fb8c0: str      r8, [sp, #0x1c]
003fb8c4: strge    r1, [sb, #4]
003fb8c8: mov      r8, r3
003fb8cc: bge      #0x3fb8f0
003fb8d0: ldr      r2, [sp, #0x18]
003fb8d4: add      r0, r5, r0
003fb8d8: ldr      r3, [r4, r2]
003fb8dc: ldr      r1, [r3]
003fb8e0: bl       #0x30ebd4
003fb8e4: cmp      r0, #0
003fb8e8: rsbne    r0, r5, r0
003fb8ec: str      r0, [sb, #4]
003fb8f0: cmp      sb, r8
003fb8f4: beq      #0x3fb91c
003fb8f8: ldr      r2, [sb, #4]!
003fb8fc: add      r0, r2, #1
003fb900: cmp      r2, #0
003fb904: cmpne    r0, fp
003fb908: movge    r1, #0
003fb90c: strge    r1, [sb, #4]
003fb910: blt      #0x3fb8d0
003fb914: cmp      sb, r8
003fb918: bne      #0x3fb8f8
003fb91c: ldr      r3, [sp, #0xc]
003fb920: ldr      r8, [sp, #0x1c]
003fb924: ldr      r2, [sp, #0x10]
003fb928: cmp      r3, #0
003fb92c: moveq    r8, fp
003fb930: add      r1, sp, #0xf0
003fb934: str      r8, [sp, #0x30]
003fb938: add      r3, r1, r2, lsl #2
003fb93c: ldr      fp, [r3, #-0xd0]
003fb940: cmp      fp, #0
003fb944: beq      #0x3fb954
003fb948: ldr      sb, [r3, #-0xcc]
003fb94c: cmp      sb, #0
003fb950: bne      #0x3fbb88
003fb954: add      sb, sp, #0xbc
003fb958: mov      r1, r5
003fb95c: add      r2, sp, #0x3c
003fb960: mov      r0, sb
003fb964: bl       #0x3140ec
003fb968: ldr      r2, [sp, #0x20]
003fb96c: mov      r3, r8
003fb970: ldr      r0, [sp]
003fb974: add      r2, r2, #1
003fb978: mov      r1, sb
003fb97c: bl       #0x3fa720
003fb980: mov      r0, sb
003fb984: bl       #0x3139ac
003fb988: add      r8, sp, #0x8c
003fb98c: mov      r0, r8
003fb990: mov      r1, #0x10
003fb994: str      r8, [sp, #0x9c]
003fb998: str      r8, [sp, #0xa0]
003fb99c: bl       #0x31167c
003fb9a0: ldr      r3, [sp, #0x9c]
003fb9a4: ldr      r1, [pc, #0x294]
003fb9a8: add      sb, sp, #0x74
003fb9ac: mov      r2, #0
003fb9b0: strb     r2, [r3]
003fb9b4: add      r1, pc, r1
003fb9b8: add      r2, sp, #0x34
003fb9bc: mov      r0, sb
003fb9c0: bl       #0x3140ec
003fb9c4: ldr      r3, [sp, #4]
003fb9c8: mov      r1, sb
003fb9cc: ldr      r2, [r4, r3]
003fb9d0: ldr      r3, [r7, #0x54]
003fb9d4: ldr      r0, [r2, #0x34]
003fb9d8: ldr      r2, [sp, #0x14]
003fb9dc: bl       #0x508ef4
003fb9e0: ldr      r1, [sp, #8]
003fb9e4: mov      r0, r8
003fb9e8: cmp      r1, #0
003fb9ec: ldreq    r2, [sp, #0x84]
003fb9f0: ldrne    r2, [sp, #8]
003fb9f4: ldrne    r1, [sp, #0x14]
003fb9f8: ldreq    r3, [sp, #0x88]
003fb9fc: rsbne    r3, r1, r2
003fba00: rsbeq    r3, r3, r2
003fba04: mov      r1, sb
003fba08: mov      r2, #0
003fba0c: bl       #0x3fa720
003fba10: ldr      r1, [pc, #0x22c]
003fba14: mov      r0, sl
003fba18: add      r1, pc, r1
003fba1c: mov      r2, r1
003fba20: bl       #0x3109e0
003fba24: ldr      r2, [sp, #0xc]
003fba28: cmp      r2, #0
003fba2c: bne      #0x3fbae8
003fba30: cmp      r5, #0
003fba34: beq      #0x3fbaa4
003fba38: ldr      r3, [sp, #0xe8]
003fba3c: ldr      r1, [sp, #0xe4]
003fba40: add      r5, sp, #0x5c
003fba44: mov      r0, r5
003fba48: rsb      r1, r3, r1
003fba4c: add      r1, r1, #2
003fba50: str      r5, [sp, #0x6c]
003fba54: str      r5, [sp, #0x70]
003fba58: bl       #0x31167c
003fba5c: ldr      r3, [sp, #0x6c]
003fba60: ldr      r1, [sp, #0xc]
003fba64: mov      r0, r5
003fba68: strb     r1, [r3]
003fba6c: ldr      r2, [sp, #0xe4]
003fba70: ldr      r1, [sp, #0xe8]
003fba74: bl       #0x310804
003fba78: ldr      r1, [pc, #0x1c8]
003fba7c: mov      r0, r5
003fba80: add      r1, pc, r1
003fba84: add      r1, r1, #1
003fba88: bl       #0x3fa868
003fba8c: mov      r0, sl
003fba90: ldr      r1, [sp, #0x70]
003fba94: ldr      r2, [sp, #0x6c]
003fba98: bl       #0x310804
003fba9c: mov      r0, r5
003fbaa0: bl       #0x3139ac
003fbaa4: ldr      r1, [sp, #0xa0]
003fbaa8: ldr      r2, [sp, #0x9c]
003fbaac: mov      r0, sl
003fbab0: bl       #0x310804
003fbab4: mov      r0, sb
003fbab8: bl       #0x3139ac
003fbabc: mov      r0, r8
003fbac0: bl       #0x3139ac
003fbac4: ldr      r0, [sp]
003fbac8: bl       #0x3139ac
003fbacc: ldr      r3, [r4, r6]
003fbad0: ldr      r2, [sp, #0xec]
003fbad4: ldr      r3, [r3]
003fbad8: cmp      r2, r3
003fbadc: bne      #0x3fbc18
003fbae0: add      sp, sp, #0xf4
003fbae4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003fbae8: mov      r0, sl
003fbaec: ldr      r1, [sp, #0xa0]
003fbaf0: ldr      r2, [sp, #0x9c]
003fbaf4: bl       #0x310804
003fbaf8: cmp      r5, #0
003fbafc: beq      #0x3fbb6c
003fbb00: ldr      r3, [sp, #0xe8]
003fbb04: ldr      r1, [sp, #0xe4]
003fbb08: add      r5, sp, #0x44
003fbb0c: mov      r0, r5
003fbb10: rsb      r1, r3, r1
003fbb14: add      r1, r1, #2
003fbb18: str      r5, [sp, #0x54]
003fbb1c: str      r5, [sp, #0x58]
003fbb20: bl       #0x31167c
003fbb24: ldr      r1, [pc, #0x120]
003fbb28: ldr      r3, [sp, #0x54]
003fbb2c: mov      r2, #0
003fbb30: add      r1, pc, r1
003fbb34: strb     r2, [r3]
003fbb38: add      r1, r1, #1
003fbb3c: mov      r0, r5
003fbb40: bl       #0x3fa868
003fbb44: ldr      r1, [sp, #0xe8]
003fbb48: ldr      r2, [sp, #0xe4]
003fbb4c: mov      r0, r5
003fbb50: bl       #0x310804
003fbb54: mov      r0, sl
003fbb58: ldr      r1, [sp, #0x58]
003fbb5c: ldr      r2, [sp, #0x54]
003fbb60: bl       #0x310804
003fbb64: mov      r0, r5
003fbb68: bl       #0x3139ac
003fbb6c: mov      r0, sb
003fbb70: bl       #0x3139ac
003fbb74: mov      r0, r8
003fbb78: bl       #0x3139ac
003fbb7c: ldr      r0, [sp]
003fbb80: bl       #0x3139ac
003fbb84: b        #0x3fbacc
003fbb88: add      r8, sp, #0xa4
003fbb8c: mov      r1, r5
003fbb90: add      r2, sp, #0x38
003fbb94: mov      r0, r8
003fbb98: bl       #0x3140ec
003fbb9c: rsb      r3, fp, sb
003fbba0: add      r2, fp, #1
003fbba4: sub      r3, r3, #1
003fbba8: ldr      r0, [sp]
003fbbac: mov      r1, r8
003fbbb0: bl       #0x3fa720
003fbbb4: mov      r0, r8
003fbbb8: bl       #0x3139ac
003fbbbc: b        #0x3fb988
003fbbc0: ldr      r3, [pc, #0x88]
003fbbc4: ldr      r0, [sp, #0x14]
003fbbc8: ldr      r3, [r4, r3]
003fbbcc: ldr      r1, [r3]
003fbbd0: bl       #0x30ebd4
003fbbd4: cmp      r0, #0
003fbbd8: movne    sb, #1
003fbbdc: mov      fp, r0
003fbbe0: str      r0, [sp, #8]
003fbbe4: movne    fp, sb
003fbbe8: bne      #0x3fb7f0
003fbbec: ldr      r3, [pc, #0x60]
003fbbf0: ldr      r0, [sp, #0x14]
003fbbf4: ldr      r3, [r4, r3]
003fbbf8: ldr      r1, [r3]
003fbbfc: bl       #0x30ebd4
003fbc00: cmp      r0, #0
003fbc04: str      r0, [sp, #8]
003fbc08: ldreq    sb, [sp, #8]
003fbc0c: movne    sb, #1
003fbc10: moveq    fp, sb
003fbc14: b        #0x3fb7f0
003fbc18: bl       #0x30e310
003fbc1c: subseq   sb, sb, ip, lsr #6
003fbc20: andeq    r2, r0, ip, ror #16
003fbc24: andeq    r4, r0, ip, lsr #1
003fbc28: subeq    r0, sp, ip, ror r0
003fbc2c: strdeq   r3, r4, [r0], -r4
003fbc30: strdeq   r3, r4, [r0], -r4
003fbc34: subeq    pc, ip, r8, ror #31
003fbc38: andeq    r2, r0, r4, asr #6
003fbc3c: strdeq   r1, r2, [r0], -ip
003fbc40: subeq    pc, ip, r4, asr lr

# _ZN14PlayerSavegame17__LoadPlayerClassEP11IStreamBasePv
00469d88: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469d8c: ldr      r8, [pc, #0xc8]
00469d90: ldr      sb, [pc, #0xc8]
00469d94: sub      sp, sp, #0x24
00469d98: add      r8, pc, r8
00469d9c: ldr      r3, [r8, sb]
00469da0: add      sl, sp, #4
00469da4: mov      r5, r0
00469da8: ldr      r3, [r3]
00469dac: mov      r0, sl
00469db0: mov      fp, r1
00469db4: mov      r1, #0x10
00469db8: str      r3, [sp, #0x1c]
00469dbc: str      sl, [sp, #0x14]
00469dc0: str      sl, [sp, #0x18]
00469dc4: bl       #0x31167c
00469dc8: ldr      r3, [sp, #0x14]
00469dcc: mov      r4, #0
00469dd0: mov      r0, r5
00469dd4: strb     r4, [r3]
00469dd8: mov      r1, sl
00469ddc: bl       #0x461da8
00469de0: ldr      r3, [pc, #0x7c]
00469de4: ldr      r6, [sp, #0x18]
00469de8: ldr      r3, [r8, r3]
00469dec: ldr      r5, [r3]
00469df0: cmp      r5, r4
00469df4: beq      #0x469e50
00469df8: ldr      r3, [pc, #0x68]
00469dfc: ldr      r3, [r8, r3]
00469e00: ldr      r7, [r3]
00469e04: b        #0x469e14
00469e08: add      r4, r4, #1
00469e0c: cmp      r4, r5
00469e10: beq      #0x469e50
00469e14: mov      r0, r6
00469e18: ldr      r1, [r7, r4, lsl #2]
00469e1c: bl       #0x30e31c
00469e20: cmp      r0, #0
00469e24: bne      #0x469e08
00469e28: str      r4, [fp, #0x34]
00469e2c: mov      r0, sl
00469e30: bl       #0x3139ac
00469e34: ldr      r3, [r8, sb]
00469e38: ldr      r2, [sp, #0x1c]
00469e3c: ldr      r3, [r3]
00469e40: cmp      r2, r3
00469e44: bne      #0x469e58
00469e48: add      sp, sp, #0x24
00469e4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469e50: mvn      r4, #0
00469e54: b        #0x469e28
00469e58: bl       #0x30e310
00469e5c: ldrsheq  sl, [r2], #-0xc8
00469e60: andeq    r4, r0, ip, lsr #1
00469e64: andeq    r4, r0, r4, lsl #4
00469e68: andeq    r3, r0, r8, lsl #24

# _ZN13ItemInventory7SetGoldEi
003fdfd8: push     {r4, r5, r6, lr}
003fdfdc: ldr      r4, [pc, #0x154]
003fdfe0: subs     r6, r1, #0
003fdfe4: sub      sp, sp, #8
003fdfe8: mov      r5, r0
003fdfec: add      r4, pc, r4
003fdff0: blt      #0x3fe0e0
003fdff4: ldr      r2, [pc, #0x140]
003fdff8: ldr      r1, [r5, #0x28]
003fdffc: ldr      r3, [r5, #4]
003fe000: ldr      r2, [r4, r2]
003fe004: cmp      r6, r1
003fe008: strle    r6, [r5, #0x20]
003fe00c: strgt    r1, [r5, #0x20]
003fe010: cmp      r3, #0
003fe014: ldr      r6, [r2]
003fe018: beq      #0x3fe034
003fe01c: mov      r0, r3
003fe020: ldr      r3, [r3]
003fe024: mov      lr, pc
003fe028: ldr      pc, [r3, #0x28]
003fe02c: cmp      r0, #0
003fe030: bne      #0x3fe03c
003fe034: add      sp, sp, #8
003fe038: pop      {r4, r5, r6, pc}
003fe03c: ldr      r3, [pc, #0xfc]
003fe040: ldr      r1, [r5, #4]
003fe044: ldr      r3, [r4, r3]
003fe048: ldr      r0, [r3, #0x40]
003fe04c: bl       #0x36effc
003fe050: cmp      r0, #0
003fe054: beq      #0x3fe034
003fe058: ldr      r2, [r5, #0x20]
003fe05c: movw     r3, #0x270f
003fe060: cmp      r2, r3
003fe064: ble      #0x3fe034
003fe068: ldr      r0, [pc, #0xd4]
003fe06c: add      r0, pc, r0
003fe070: bl       #0x3a3f70
003fe074: mov      r1, r0
003fe078: mov      r0, r6
003fe07c: bl       #0x3813b8
003fe080: ldr      r2, [r5, #0x20]
003fe084: movw     r3, #0x869f
003fe088: movt     r3, #1
003fe08c: cmp      r2, r3
003fe090: ble      #0x3fe034
003fe094: ldr      r0, [pc, #0xac]
003fe098: add      r0, pc, r0
003fe09c: bl       #0x3a3f70
003fe0a0: mov      r1, r0
003fe0a4: mov      r0, r6
003fe0a8: bl       #0x3813b8
003fe0ac: ldr      r2, [r5, #0x20]
003fe0b0: movw     r3, #0x423f
003fe0b4: movt     r3, #0xf
003fe0b8: cmp      r2, r3
003fe0bc: ble      #0x3fe034
003fe0c0: ldr      r0, [pc, #0x84]
003fe0c4: add      r0, pc, r0
003fe0c8: bl       #0x3a3f70
003fe0cc: mov      r1, r0
003fe0d0: mov      r0, r6
003fe0d4: add      sp, sp, #8
003fe0d8: pop      {r4, r5, r6, lr}
003fe0dc: b        #0x3813b8
003fe0e0: ldr      r3, [pc, #0x68]
003fe0e4: ldr      r3, [r4, r3]
003fe0e8: ldr      r3, [r3]
003fe0ec: cmp      r3, #2
003fe0f0: moveq    r3, #0
003fe0f4: streq    r3, [r3]
003fe0f8: beq      #0x3fdff4
003fe0fc: cmp      r3, #1
003fe100: bne      #0x3fdff4
003fe104: ldr      r0, [pc, #0x48]
003fe108: ldr      r1, [pc, #0x48]
003fe10c: ldr      r2, [pc, #0x48]
003fe110: ldr      r0, [r4, r0]
003fe114: ldr      r3, [pc, #0x44]
003fe118: movw     ip, #0x142
003fe11c: add      r1, pc, r1
003fe120: add      r2, pc, r2
003fe124: add      r3, pc, r3
003fe128: add      r0, r0, #0xa8
003fe12c: str      ip, [sp]
003fe130: bl       #0x30e004
003fe134: b        #0x3fdff4
003fe138: subseq   r6, sb, r4, lsr #21
003fe13c: andeq    r1, r0, r0, ror sp
003fe140: strdeq   r3, r4, [r0], -r4
003fe144: subeq    sb, ip, ip, lsr #7
003fe148: umaaleq  sb, ip, r0, r3
003fe14c: subeq    sb, ip, r4, ror r3
003fe150: andeq    r3, r0, r0, asr #19
003fe154: andeq    r1, r0, r0, asr #19
003fe158: strheq   r0, [ip], #-0x2c
003fe15c: ldrdeq   r6, r7, [ip], #-0x70
003fe160: subeq    sb, ip, r4, lsl #5

# _ZN12ItemInstanceC1Eij
003fc26c: push     {r4, r5, r6, r7, r8, lr}
003fc270: ldr      r5, [pc, #0x148]
003fc274: ldr      r3, [pc, #0x148]
003fc278: mov      r4, r0
003fc27c: add      r5, pc, r5
003fc280: ldr      r3, [r5, r3]
003fc284: mov      r6, r1
003fc288: add      r1, r0, #8
003fc28c: add      r3, r3, #8
003fc290: str      r3, [r0]
003fc294: sub      sp, sp, #8
003fc298: mov      r0, r1
003fc29c: str      r1, [r4, #0x18]
003fc2a0: str      r1, [r4, #0x1c]
003fc2a4: str      r6, [r4, #4]
003fc2a8: mov      r1, #0x10
003fc2ac: mov      r8, r2
003fc2b0: bl       #0x31167c
003fc2b4: ldr      r2, [r4, #0x18]
003fc2b8: mov      r7, #0
003fc2bc: add      r3, r4, #0x20
003fc2c0: strb     r7, [r2]
003fc2c4: mov      r0, r3
003fc2c8: str      r3, [r4, #0x30]
003fc2cc: str      r3, [r4, #0x34]
003fc2d0: mov      r1, #0x10
003fc2d4: bl       #0x31167c
003fc2d8: ldr      r2, [r4, #0x30]
003fc2dc: add      r3, r4, #0x38
003fc2e0: mov      r0, r3
003fc2e4: strb     r7, [r2]
003fc2e8: mov      r1, #0x10
003fc2ec: str      r3, [r4, #0x48]
003fc2f0: str      r3, [r4, #0x4c]
003fc2f4: bl       #0x31167c
003fc2f8: ldr      r3, [r4, #0x48]
003fc2fc: cmp      r6, r7
003fc300: strb     r7, [r3]
003fc304: mov      r3, #1
003fc308: strb     r3, [r4, #0x68]
003fc30c: mvn      r3, #0
003fc310: strh     r8, [r4, #0x50]
003fc314: strb     r7, [r4, #0x69]
003fc318: str      r7, [r4, #0x54]
003fc31c: strh     r3, [r4, #0x58]
003fc320: str      r7, [r4, #0x5c]
003fc324: str      r7, [r4, #0x60]
003fc328: str      r7, [r4, #0x64]
003fc32c: blt      #0x3fc344
003fc330: ldr      r3, [pc, #0x90]
003fc334: ldr      r3, [r5, r3]
003fc338: ldr      r3, [r3]
003fc33c: cmp      r3, r7
003fc340: bne      #0x3fc368
003fc344: ldr      r3, [pc, #0x80]
003fc348: ldr      r3, [r5, r3]
003fc34c: ldr      r3, [r3]
003fc350: cmp      r3, #2
003fc354: moveq    r3, #0
003fc358: streq    r3, [r3]
003fc35c: beq      #0x3fc368
003fc360: cmp      r3, #1
003fc364: beq      #0x3fc38c
003fc368: mov      r0, r4
003fc36c: bl       #0x3fb754
003fc370: mov      r0, r4
003fc374: bl       #0x3fb290
003fc378: mov      r0, r4
003fc37c: bl       #0x3facdc
003fc380: mov      r0, r4
003fc384: add      sp, sp, #8
003fc388: pop      {r4, r5, r6, r7, r8, pc}
003fc38c: ldr      r0, [pc, #0x3c]
003fc390: ldr      r1, [pc, #0x3c]
003fc394: ldr      r2, [pc, #0x3c]
003fc398: ldr      r0, [r5, r0]
003fc39c: ldr      r3, [pc, #0x38]
003fc3a0: mov      ip, #0x54
003fc3a4: add      r1, pc, r1
003fc3a8: add      r2, pc, r2
003fc3ac: add      r3, pc, r3
003fc3b0: add      r0, r0, #0xa8
003fc3b4: str      ip, [sp]
003fc3b8: bl       #0x30e004
003fc3bc: b        #0x3fc368
003fc3c0: subseq   r8, sb, r4, lsl r8
003fc3c4: andeq    r4, r0, r0, lsl #16
003fc3c8: andeq    r0, r0, r0, ror #26
003fc3cc: andeq    r3, r0, r0, asr #19
003fc3d0: andeq    r1, r0, r0, asr #19
003fc3d4: subeq    r2, ip, r4, lsr r0
003fc3d8: umaaleq  sl, ip, r0, lr
003fc3dc: umaaleq  sl, ip, ip, fp

# _ZN12ItemInstance12_UpdateStatsEv
003fb290: push     {r4, r5, r6, r7, r8, sl, lr}
003fb294: ldr      r1, [pc, #0x1d8]
003fb298: add      r5, r0, #0x20
003fb29c: sub      sp, sp, #0xc
003fb2a0: add      r1, pc, r1
003fb2a4: mov      r2, r1
003fb2a8: mov      r4, r0
003fb2ac: mov      r0, r5
003fb2b0: bl       #0x3109e0
003fb2b4: mov      r0, r4
003fb2b8: bl       #0x3f9e08
003fb2bc: ldr      r2, [r0, #0x58]
003fb2c0: ldr      r3, [pc, #0x1b0]
003fb2c4: cmp      r2, #0xc
003fb2c8: add      r3, pc, r3
003fb2cc: bhi      #0x3fb3c0
003fb2d0: mov      r1, #1
003fb2d4: lsl      r2, r1, r2
003fb2d8: tst      r2, #0x1780
003fb2dc: beq      #0x3fb340
003fb2e0: ldr      r2, [pc, #0x194]
003fb2e4: ldr      r1, [pc, #0x194]
003fb2e8: ldr      r6, [r3, r2]
003fb2ec: ldr      r2, [pc, #0x190]
003fb2f0: add      r1, pc, r1
003fb2f4: ldr      r0, [r6, #0x2c]
003fb2f8: add      r2, pc, r2
003fb2fc: ldr      r7, [r6, #0x34]
003fb300: bl       #0x4c4bdc
003fb304: mov      r1, r0
003fb308: mov      r0, r7
003fb30c: bl       #0x508edc
003fb310: mov      r7, r0
003fb314: mov      r0, r4
003fb318: ldr      r4, [r6, #0x34]
003fb31c: bl       #0x3f9e08
003fb320: ldr      r3, [r0, #0x8c]
003fb324: mov      r1, r5
003fb328: mov      r0, r4
003fb32c: mov      r2, r7
003fb330: asr      r3, r3, #8
003fb334: add      sp, sp, #0xc
003fb338: pop      {r4, r5, r6, r7, r8, sl, lr}
003fb33c: b        #0x508ef4
003fb340: tst      r2, #0x40
003fb344: bne      #0x3fb3c8
003fb348: tst      r2, #0x3f
003fb34c: beq      #0x3fb3c0
003fb350: ldr      r2, [pc, #0x124]
003fb354: ldr      r1, [pc, #0x12c]
003fb358: ldr      r6, [r3, r2]
003fb35c: ldr      r2, [pc, #0x128]
003fb360: add      r1, pc, r1
003fb364: ldr      r0, [r6, #0x2c]
003fb368: add      r2, pc, r2
003fb36c: ldr      r7, [r6, #0x34]
003fb370: bl       #0x4c4bdc
003fb374: mov      r1, r0
003fb378: mov      r0, r7
003fb37c: bl       #0x508edc
003fb380: mov      r7, r0
003fb384: mov      r0, r4
003fb388: ldr      r6, [r6, #0x34]
003fb38c: bl       #0x3f9e08
003fb390: ldr      r3, [r0, #0x8c]
003fb394: mov      r0, r4
003fb398: asr      r4, r3, #8
003fb39c: bl       #0x3f9e08
003fb3a0: ldr      ip, [r0, #0x90]
003fb3a4: mov      r1, r5
003fb3a8: mov      r0, r6
003fb3ac: asr      ip, ip, #8
003fb3b0: mov      r2, r7
003fb3b4: mov      r3, r4
003fb3b8: str      ip, [sp]
003fb3bc: bl       #0x508ef4
003fb3c0: add      sp, sp, #0xc
003fb3c4: pop      {r4, r5, r6, r7, r8, sl, pc}
003fb3c8: ldr      r2, [pc, #0xac]
003fb3cc: ldr      r7, [pc, #0xbc]
003fb3d0: ldr      r6, [r3, r2]
003fb3d4: ldr      r2, [pc, #0xb8]
003fb3d8: add      r7, pc, r7
003fb3dc: mov      r1, r7
003fb3e0: add      r2, pc, r2
003fb3e4: ldr      r0, [r6, #0x2c]
003fb3e8: ldr      r8, [r6, #0x34]
003fb3ec: bl       #0x4c4bdc
003fb3f0: mov      r1, r0
003fb3f4: mov      r0, r8
003fb3f8: bl       #0x508edc
003fb3fc: mov      sl, r0
003fb400: mov      r0, r4
003fb404: ldr      r8, [r6, #0x34]
003fb408: bl       #0x3f9e08
003fb40c: ldr      r3, [r0, #0x8c]
003fb410: mov      r2, sl
003fb414: mov      r1, r5
003fb418: asr      r3, r3, #8
003fb41c: mov      r0, r8
003fb420: bl       #0x508ef4
003fb424: ldr      r1, [pc, #0x6c]
003fb428: mov      r0, r5
003fb42c: add      r1, pc, r1
003fb430: add      r2, r1, #1
003fb434: bl       #0x310804
003fb438: ldr      r2, [pc, #0x5c]
003fb43c: mov      r1, r7
003fb440: ldr      r0, [r6, #0x2c]
003fb444: add      r2, pc, r2
003fb448: ldr      r7, [r6, #0x34]
003fb44c: bl       #0x4c4bdc
003fb450: mov      r1, r0
003fb454: mov      r0, r7
003fb458: bl       #0x508edc
003fb45c: mov      r7, r0
003fb460: mov      r0, r4
003fb464: ldr      r4, [r6, #0x34]
003fb468: bl       #0x3f9e08
003fb46c: ldr      r3, [r0, #0x90]
003fb470: b        #0x3fb324
003fb474: subeq    r0, sp, r8, ror #10
003fb478: subseq   sb, sb, r8, asr #15
003fb47c: strdeq   r3, r4, [r0], -r4
003fb480: subeq    r3, ip, r8, lsr sb
003fb484: subeq    fp, ip, r8, lsr #29
003fb488: subeq    r3, ip, r8, asr #17
003fb48c: subeq    fp, ip, r8, lsl lr
003fb490: subeq    r3, ip, r0, asr r8
003fb494: subeq    fp, ip, r0, asr #27
003fb498: subeq    r6, ip, r4
003fb49c: subeq    fp, ip, ip, ror sp

# _ZN13ItemInventory16SwapEquipmentSetEv
003fc6c8: ldrsb    r2, [r0, #0x2e]
003fc6cc: add      r2, r2, #1
003fc6d0: lsr      r3, r2, #0x1f
003fc6d4: add      r2, r2, r3
003fc6d8: and      r2, r2, #1
003fc6dc: rsb      r3, r3, r2
003fc6e0: strb     r3, [r0, #0x2e]
003fc6e4: bx       lr

# _ZNK13ItemInventory18GetCurrentEquipSetEi
003fc6a8: cmp      r1, #0
003fc6ac: blt      #0x3fc6c0
003fc6b0: sub      r1, r1, #1
003fc6b4: cmp      r1, #1
003fc6b8: movhi    r0, #0
003fc6bc: bxhi     lr
003fc6c0: ldrsb    r0, [r0, #0x2e]
003fc6c4: bx       lr

# _ZN8Savegame10_cacheFileEP12StreamBuffer
00315ad0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315ad4: ldr      r3, [r0, #0x1c]
00315ad8: ldr      r6, [pc, #0x3ec]
00315adc: sub      sp, sp, #0x1c
00315ae0: cmp      r3, #0
00315ae4: mov      r4, r0
00315ae8: mov      r5, r1
00315aec: add      r6, pc, r6
00315af0: beq      #0x315b0c
00315af4: mov      r0, r3
00315af8: ldr      r3, [r3]
00315afc: mov      lr, pc
00315b00: ldr      pc, [r3, #4]
00315b04: mov      r3, #0
00315b08: str      r3, [r4, #0x1c]
00315b0c: cmp      r5, #0
00315b10: beq      #0x315e30
00315b14: mov      r0, r5
00315b18: mov      r2, #0
00315b1c: mov      r3, #0
00315b20: ldr      r1, [r5]
00315b24: mov      lr, pc
00315b28: ldr      pc, [r1, #0x20]
00315b2c: mov      r2, #0
00315b30: mov      r3, #0
00315b34: mov      r0, r5
00315b38: ldr      r1, [r5]
00315b3c: mov      lr, pc
00315b40: ldr      pc, [r1, #0x2c]
00315b44: mov      r1, #0
00315b48: mov      r0, #0x30
00315b4c: bl       #0x310570
00315b50: mov      r1, r5
00315b54: mov      r7, r0
00315b58: bl       #0x3172d8
00315b5c: str      r7, [r4, #0x1c]
00315b60: ldrb     r3, [r4, #0x38]
00315b64: cmp      r3, #0
00315b68: bne      #0x315df4
00315b6c: ldr      r3, [r4, #0x1c]
00315b70: cmp      r3, #0
00315b74: beq      #0x315bbc
00315b78: mov      r0, r3
00315b7c: ldr      r3, [r3]
00315b80: mov      lr, pc
00315b84: ldr      pc, [r3, #8]
00315b88: cmp      r1, #0
00315b8c: bne      #0x315dfc
00315b90: cmp      r0, #3
00315b94: bhi      #0x315dfc
00315b98: ldr      r3, [r4, #0x1c]
00315b9c: cmp      r3, #0
00315ba0: beq      #0x315bbc
00315ba4: mov      r0, r3
00315ba8: ldr      r3, [r3]
00315bac: mov      lr, pc
00315bb0: ldr      pc, [r3, #4]
00315bb4: mov      r3, #0
00315bb8: str      r3, [r4, #0x1c]
00315bbc: ldr      r3, [r4, #0x18]
00315bc0: ldr      r0, [r4, #0x14]
00315bc4: mov      r1, #0
00315bc8: ldr      r7, [pc, #0x300]
00315bcc: rsb      r0, r3, r0
00315bd0: add      r0, r0, #5
00315bd4: bl       #0x31056c
00315bd8: ldr      r1, [r4, #0x18]
00315bdc: mov      r5, r0
00315be0: bl       #0x30e520
00315be4: mov      r0, r5
00315be8: bl       #0x30de54
00315bec: ldr      r1, [pc, #0x2e0]
00315bf0: mov      r2, #5
00315bf4: add      r0, r5, r0
00315bf8: add      r1, pc, r1
00315bfc: bl       #0x30e868
00315c00: ldr      r3, [r6, r7]
00315c04: mov      r1, r5
00315c08: mov      r2, #0
00315c0c: ldr      r3, [r3, #0x10]
00315c10: ldr      r3, [r3, #0x34]
00315c14: mov      r0, r3
00315c18: ldr      r3, [r3]
00315c1c: mov      lr, pc
00315c20: ldr      pc, [r3, #0x94]
00315c24: cmp      r5, #0
00315c28: str      r0, [sp, #0x14]
00315c2c: beq      #0x315c3c
00315c30: mov      r0, r5
00315c34: bl       #0x310440
00315c38: ldr      r0, [sp, #0x14]
00315c3c: cmp      r0, #0
00315c40: beq      #0x315c94
00315c44: mov      r1, #0
00315c48: mov      r0, #0x30
00315c4c: bl       #0x310570
00315c50: add      r5, sp, #0x18
00315c54: ldr      r1, [r5, #-4]!
00315c58: mov      r8, r0
00315c5c: bl       #0x3172d8
00315c60: ldr      r3, [r6, r7]
00315c64: str      r8, [r4, #0x1c]
00315c68: mov      r1, r5
00315c6c: ldr      r3, [r3, #0x10]
00315c70: ldr      r3, [r3, #0x34]
00315c74: mov      r0, r3
00315c78: ldr      r3, [r3]
00315c7c: mov      lr, pc
00315c80: ldr      pc, [r3, #0x78]
00315c84: ldr      r0, [r4, #0x1c]
00315c88: bl       #0x313a90
00315c8c: cmn      r0, #1
00315c90: beq      #0x315ea4
00315c94: ldr      r3, [r4, #0x1c]
00315c98: cmp      r3, #0
00315c9c: beq      #0x315df4
00315ca0: mov      r0, r3
00315ca4: ldr      r3, [r3]
00315ca8: mov      lr, pc
00315cac: ldr      pc, [r3, #8]
00315cb0: cmp      r1, #0
00315cb4: bne      #0x315cc0
00315cb8: cmp      r0, #3
00315cbc: bls      #0x315df4
00315cc0: ldr      r1, [r4, #0x1c]
00315cc4: mov      r2, #0
00315cc8: mov      r3, #0
00315ccc: mov      r0, r1
00315cd0: ldr      r1, [r1]
00315cd4: mov      lr, pc
00315cd8: ldr      pc, [r1, #0x20]
00315cdc: ldr      r0, [r4, #0x1c]
00315ce0: bl       #0x313a90
00315ce4: cmp      r0, #0
00315ce8: str      r0, [sp, #4]
00315cec: beq      #0x315df4
00315cf0: mov      r5, #0
00315cf4: add      r7, r4, #0x20
00315cf8: mov      sb, r5
00315cfc: add      r8, sp, #0xc
00315d00: ldr      r3, [r4, #0x1c]
00315d04: mov      r0, r3
00315d08: ldr      r3, [r3]
00315d0c: mov      lr, pc
00315d10: ldr      pc, [r3, #0x24]
00315d14: ldr      r3, [r4, #0x1c]
00315d18: mov      sl, r0
00315d1c: mov      r6, r1
00315d20: mov      r0, r3
00315d24: ldr      r3, [r3]
00315d28: mov      lr, pc
00315d2c: ldr      pc, [r3, #8]
00315d30: cmp      r1, r6
00315d34: bhi      #0x315d44
00315d38: bne      #0x315df4
00315d3c: cmp      r0, sl
00315d40: bls      #0x315df4
00315d44: ldr      r0, [r4, #0x1c]
00315d48: bl       #0x313a90
00315d4c: mov      r2, #4
00315d50: mov      r3, #0
00315d54: mov      r1, r8
00315d58: mov      r6, r0
00315d5c: ldr      r0, [r4, #0x1c]
00315d60: strb     sb, [sp, #0xc]
00315d64: strb     sb, [sp, #0xd]
00315d68: strb     sb, [sp, #0xe]
00315d6c: strb     sb, [sp, #0xf]
00315d70: strb     sb, [sp, #0x10]
00315d74: bl       #0x317454
00315d78: ldr      r3, [r4, #0x1c]
00315d7c: mov      r0, r3
00315d80: ldr      r3, [r3]
00315d84: mov      lr, pc
00315d88: ldr      pc, [r3, #0x24]
00315d8c: mov      sl, r0
00315d90: mov      fp, r1
00315d94: mov      r0, r7
00315d98: mov      r1, r8
00315d9c: bl       #0x314380
00315da0: cmp      r7, r0
00315da4: mov      r1, r8
00315da8: beq      #0x315e10
00315dac: mov      r0, r7
00315db0: bl       #0x315978
00315db4: mov      r1, r8
00315db8: strd     sl, fp, [r0]
00315dbc: mov      r0, r7
00315dc0: bl       #0x315978
00315dc4: str      r6, [r0, #8]
00315dc8: ldr      r1, [r4, #0x1c]
00315dcc: adds     r2, sl, r6
00315dd0: adc      r3, fp, #0
00315dd4: mov      r0, r1
00315dd8: ldr      r1, [r1]
00315ddc: mov      lr, pc
00315de0: ldr      pc, [r1, #0x20]
00315de4: ldr      r3, [sp, #4]
00315de8: add      r5, r5, #1
00315dec: cmp      r5, r3
00315df0: bne      #0x315d00
00315df4: add      sp, sp, #0x1c
00315df8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00315dfc: ldr      r0, [r4, #0x1c]
00315e00: bl       #0x313a90
00315e04: cmn      r0, #1
00315e08: bne      #0x315c94
00315e0c: b        #0x315b98
00315e10: mov      r1, r8
00315e14: bl       #0x315978
00315e18: strd     sl, fp, [r0]
00315e1c: str      sb, [r0, #0x14]
00315e20: str      sb, [r0, #0x10]
00315e24: str      sb, [r0, #0xc]
00315e28: str      r6, [r0, #8]
00315e2c: b        #0x315dc8
00315e30: ldr      r3, [pc, #0x98]
00315e34: ldr      r1, [r4, #0x18]
00315e38: mov      r2, r5
00315e3c: ldr      r7, [r6, r3]
00315e40: ldr      r3, [r7, #0x10]
00315e44: ldr      r3, [r3, #0x34]
00315e48: mov      r0, r3
00315e4c: ldr      r3, [r3]
00315e50: mov      lr, pc
00315e54: ldr      pc, [r3, #0x94]
00315e58: cmp      r0, #0
00315e5c: str      r0, [sp, #0x14]
00315e60: beq      #0x315b60
00315e64: mov      r1, r5
00315e68: mov      r0, #0x30
00315e6c: bl       #0x310570
00315e70: add      r5, sp, #0x18
00315e74: mov      r8, r0
00315e78: ldr      r1, [r5, #-4]!
00315e7c: bl       #0x3172d8
00315e80: str      r8, [r4, #0x1c]
00315e84: ldr      r3, [r7, #0x10]
00315e88: mov      r1, r5
00315e8c: ldr      r3, [r3, #0x34]
00315e90: mov      r0, r3
00315e94: ldr      r3, [r3]
00315e98: mov      lr, pc
00315e9c: ldr      pc, [r3, #0x78]
00315ea0: b        #0x315b60
00315ea4: ldr      r3, [r4, #0x1c]
00315ea8: cmp      r3, #0
00315eac: beq      #0x315df4
00315eb0: mov      r0, r3
00315eb4: ldr      r3, [r3]
00315eb8: mov      lr, pc
00315ebc: ldr      pc, [r3, #4]
00315ec0: mov      r3, #0
00315ec4: str      r3, [r4, #0x1c]
00315ec8: b        #0x315df4
00315ecc: rsbeq    lr, r7, r4, lsr #31
00315ed0: strdeq   r3, r4, [r0], -r4
00315ed4: subseq   r8, sl, r8, ror #18

# _ZN12ItemInstance11_UpdateReqsEv
003facdc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003face0: ldr      r1, [pc, #0x560]
003face4: add      r6, r0, #0x38
003face8: sub      sp, sp, #4
003facec: add      r1, pc, r1
003facf0: mov      r4, r0
003facf4: mov      r2, r1
003facf8: mov      r0, r6
003facfc: bl       #0x3109e0
003fad00: mov      r0, r4
003fad04: bl       #0x3f9e08
003fad08: ldr      r3, [r0, #0x74]
003fad0c: ldr      r5, [pc, #0x538]
003fad10: cmp      r3, #0
003fad14: add      r5, pc, r5
003fad18: bne      #0x3fad30
003fad1c: mov      r0, r4
003fad20: bl       #0x3f9e08
003fad24: ldr      r3, [r0, #0x78]
003fad28: cmp      r3, #0
003fad2c: beq      #0x3fb01c
003fad30: mov      r0, r4
003fad34: bl       #0x3f9e08
003fad38: ldr      r3, [r0, #0x58]
003fad3c: cmp      r3, #0xe
003fad40: beq      #0x3fb06c
003fad44: ldr      r7, [pc, #0x504]
003fad48: ldr      sl, [pc, #0x504]
003fad4c: ldr      r2, [pc, #0x504]
003fad50: ldr      r8, [r5, r7]
003fad54: add      sl, pc, sl
003fad58: add      r2, pc, r2
003fad5c: ldr      r0, [r8, #0x2c]
003fad60: mov      r1, sl
003fad64: ldr      sb, [r8, #0x34]
003fad68: bl       #0x4c4bdc
003fad6c: mov      r1, r0
003fad70: mov      r0, sb
003fad74: bl       #0x508edc
003fad78: mov      sb, r0
003fad7c: bl       #0x30de54
003fad80: mov      r1, sb
003fad84: add      r2, sb, r0
003fad88: mov      r0, r6
003fad8c: bl       #0x3109e0
003fad90: ldr      r2, [pc, #0x4c4]
003fad94: ldr      r0, [r8, #0x2c]
003fad98: mov      r1, sl
003fad9c: add      r2, pc, r2
003fada0: ldr      sb, [r8, #0x34]
003fada4: bl       #0x4c4bdc
003fada8: mov      r1, r0
003fadac: mov      r0, sb
003fadb0: bl       #0x508edc
003fadb4: mov      sb, r0
003fadb8: mov      r0, r4
003fadbc: bl       #0x3f9e08
003fadc0: ldr      r3, [r0, #0x74]
003fadc4: cmp      r3, #0
003fadc8: moveq    r8, r3
003fadcc: bne      #0x3fb0d0
003fadd0: mov      r0, r4
003fadd4: bl       #0x3f9e08
003fadd8: ldr      r3, [r0, #0x78]
003faddc: cmp      r3, #0
003fade0: beq      #0x3fae40
003fade4: cmp      r8, #0
003fade8: bne      #0x3fb120
003fadec: ldr      r8, [r5, r7]
003fadf0: ldr      r1, [pc, #0x468]
003fadf4: ldr      r2, [pc, #0x468]
003fadf8: ldr      r0, [r8, #0x2c]
003fadfc: add      r1, pc, r1
003fae00: add      r2, pc, r2
003fae04: ldr      sl, [r8, #0x34]
003fae08: bl       #0x4c4bdc
003fae0c: mov      r1, r0
003fae10: mov      r0, sl
003fae14: bl       #0x508edc
003fae18: mov      sl, r0
003fae1c: mov      r0, r4
003fae20: ldr      r8, [r8, #0x34]
003fae24: bl       #0x3f9e08
003fae28: mov      r2, sl
003fae2c: ldr      r3, [r0, #0x78]
003fae30: mov      r1, r6
003fae34: mov      r0, r8
003fae38: bl       #0x508ef4
003fae3c: mov      r8, #1
003fae40: mov      r0, r4
003fae44: bl       #0x3f9e08
003fae48: ldr      r3, [r0, #0x7c]
003fae4c: cmp      r3, #0
003fae50: beq      #0x3faeb0
003fae54: cmp      r8, #0
003fae58: bne      #0x3fb150
003fae5c: ldr      r8, [r5, r7]
003fae60: ldr      r1, [pc, #0x400]
003fae64: ldr      r2, [pc, #0x400]
003fae68: ldr      r0, [r8, #0x2c]
003fae6c: add      r1, pc, r1
003fae70: add      r2, pc, r2
003fae74: ldr      sl, [r8, #0x34]
003fae78: bl       #0x4c4bdc
003fae7c: mov      r1, r0
003fae80: mov      r0, sl
003fae84: bl       #0x508edc
003fae88: mov      sl, r0
003fae8c: mov      r0, r4
003fae90: ldr      r8, [r8, #0x34]
003fae94: bl       #0x3f9e08
003fae98: mov      r2, sl
003fae9c: ldr      r3, [r0, #0x7c]
003faea0: mov      r1, r6
003faea4: mov      r0, r8
003faea8: bl       #0x508ef4
003faeac: mov      r8, #1
003faeb0: mov      r0, r4
003faeb4: bl       #0x3f9e08
003faeb8: ldr      r3, [r0, #0x80]
003faebc: cmp      r3, #0
003faec0: beq      #0x3faf20
003faec4: cmp      r8, #0
003faec8: bne      #0x3fb130
003faecc: ldr      r8, [r5, r7]
003faed0: ldr      r1, [pc, #0x398]
003faed4: ldr      r2, [pc, #0x398]
003faed8: ldr      r0, [r8, #0x2c]
003faedc: add      r1, pc, r1
003faee0: add      r2, pc, r2
003faee4: ldr      sl, [r8, #0x34]
003faee8: bl       #0x4c4bdc
003faeec: mov      r1, r0
003faef0: mov      r0, sl
003faef4: bl       #0x508edc
003faef8: mov      sl, r0
003faefc: mov      r0, r4
003faf00: ldr      r8, [r8, #0x34]
003faf04: bl       #0x3f9e08
003faf08: mov      r2, sl
003faf0c: ldr      r3, [r0, #0x80]
003faf10: mov      r1, r6
003faf14: mov      r0, r8
003faf18: bl       #0x508ef4
003faf1c: mov      r8, #1
003faf20: mov      r0, r4
003faf24: bl       #0x3f9e08
003faf28: ldr      r3, [r0, #0x84]
003faf2c: cmp      r3, #0
003faf30: beq      #0x3faf90
003faf34: cmp      r8, #0
003faf38: bne      #0x3fb140
003faf3c: ldr      r8, [r5, r7]
003faf40: ldr      r1, [pc, #0x330]
003faf44: ldr      r2, [pc, #0x330]
003faf48: ldr      r0, [r8, #0x2c]
003faf4c: add      r1, pc, r1
003faf50: add      r2, pc, r2
003faf54: ldr      sl, [r8, #0x34]
003faf58: bl       #0x4c4bdc
003faf5c: mov      r1, r0
003faf60: mov      r0, sl
003faf64: bl       #0x508edc
003faf68: mov      sl, r0
003faf6c: mov      r0, r4
003faf70: ldr      r8, [r8, #0x34]
003faf74: bl       #0x3f9e08
003faf78: mov      r2, sl
003faf7c: ldr      r3, [r0, #0x84]
003faf80: mov      r1, r6
003faf84: mov      r0, r8
003faf88: bl       #0x508ef4
003faf8c: mov      r8, #1
003faf90: mov      r0, r4
003faf94: bl       #0x3f9e08
003faf98: ldr      r3, [r0, #0x88]
003faf9c: cmp      r3, #0
003fafa0: beq      #0x3fb06c
003fafa4: cmp      r8, #0
003fafa8: bne      #0x3fb0c0
003fafac: ldr      r3, [r5, r7]
003fafb0: ldr      r1, [pc, #0x2c8]
003fafb4: ldr      r2, [pc, #0x2c8]
003fafb8: ldr      r0, [r3, #0x2c]
003fafbc: add      r1, pc, r1
003fafc0: add      r2, pc, r2
003fafc4: ldr      r8, [r3, #0x34]
003fafc8: bl       #0x4c4bdc
003fafcc: mov      r1, r0
003fafd0: mov      r0, r8
003fafd4: bl       #0x508edc
003fafd8: mov      r8, r0
003fafdc: mov      r0, r4
003fafe0: bl       #0x3f9e08
003fafe4: ldr      r3, [r0, #0x88]
003fafe8: sub      r3, r3, #1
003fafec: cmp      r3, #8
003faff0: addls    pc, pc, r3, lsl #2
003faff4: b        #0x3fb090
003faff8: b        #0x3fb1bc
003faffc: b        #0x3fb1d8
003fb000: b        #0x3fb160
003fb004: b        #0x3fb17c
003fb008: b        #0x3fb074
003fb00c: b        #0x3fb19c
003fb010: b        #0x3fb210
003fb014: b        #0x3fb22c
003fb018: b        #0x3fb1f4
003fb01c: mov      r0, r4
003fb020: bl       #0x3f9e08
003fb024: ldr      r3, [r0, #0x7c]
003fb028: cmp      r3, #0
003fb02c: bne      #0x3fad30
003fb030: mov      r0, r4
003fb034: bl       #0x3f9e08
003fb038: ldr      r3, [r0, #0x80]
003fb03c: cmp      r3, #0
003fb040: bne      #0x3fad30
003fb044: mov      r0, r4
003fb048: bl       #0x3f9e08
003fb04c: ldr      r3, [r0, #0x84]
003fb050: cmp      r3, #0
003fb054: bne      #0x3fad30
003fb058: mov      r0, r4
003fb05c: bl       #0x3f9e08
003fb060: ldr      r3, [r0, #0x88]
003fb064: cmp      r3, #0
003fb068: bne      #0x3fad30
003fb06c: add      sp, sp, #4
003fb070: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003fb074: ldr      r3, [pc, #0x20c]
003fb078: ldr      r3, [r5, r3]
003fb07c: ldr      r3, [r3]
003fb080: add      r3, r3, #0x47000
003fb084: add      r3, r3, #0xd90
003fb088: add      r3, r3, #0xc
003fb08c: ldr      fp, [r3, #0x18]
003fb090: ldr      r3, [r5, r7]
003fb094: mov      r1, fp
003fb098: ldr      r4, [r3, #0x34]
003fb09c: mov      r0, r4
003fb0a0: bl       #0x508edc
003fb0a4: mov      r1, r6
003fb0a8: mov      r3, r0
003fb0ac: mov      r2, r8
003fb0b0: mov      r0, r4
003fb0b4: add      sp, sp, #4
003fb0b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003fb0bc: b        #0x508ef4
003fb0c0: mov      r1, sb
003fb0c4: mov      r0, r6
003fb0c8: bl       #0x379ef8
003fb0cc: b        #0x3fafac
003fb0d0: ldr      r2, [pc, #0x1b4]
003fb0d4: mov      r1, sl
003fb0d8: ldr      r0, [r8, #0x2c]
003fb0dc: add      r2, pc, r2
003fb0e0: ldr      sl, [r8, #0x34]
003fb0e4: bl       #0x4c4bdc
003fb0e8: mov      r1, r0
003fb0ec: mov      r0, sl
003fb0f0: bl       #0x508edc
003fb0f4: mov      sl, r0
003fb0f8: mov      r0, r4
003fb0fc: ldr      r8, [r8, #0x34]
003fb100: bl       #0x3f9e08
003fb104: mov      r2, sl
003fb108: ldr      r3, [r0, #0x74]
003fb10c: mov      r1, r6
003fb110: mov      r0, r8
003fb114: bl       #0x508ef4
003fb118: mov      r8, #1
003fb11c: b        #0x3fadd0
003fb120: mov      r0, r6
003fb124: mov      r1, sb
003fb128: bl       #0x379ef8
003fb12c: b        #0x3fadec
003fb130: mov      r0, r6
003fb134: mov      r1, sb
003fb138: bl       #0x379ef8
003fb13c: b        #0x3faecc
003fb140: mov      r0, r6
003fb144: mov      r1, sb
003fb148: bl       #0x379ef8
003fb14c: b        #0x3faf3c
003fb150: mov      r0, r6
003fb154: mov      r1, sb
003fb158: bl       #0x379ef8
003fb15c: b        #0x3fae5c
003fb160: ldr      r3, [pc, #0x120]
003fb164: ldr      r3, [r5, r3]
003fb168: ldr      r3, [r3]
003fb16c: add      r3, r3, #0x3a000
003fb170: add      r3, r3, #0x3a4
003fb174: ldr      fp, [r3, #0x18]
003fb178: b        #0x3fb090
003fb17c: ldr      r3, [pc, #0x104]
003fb180: ldr      r3, [r5, r3]
003fb184: ldr      r3, [r3]
003fb188: add      r3, r3, #0x47000
003fb18c: add      r3, r3, #0x690
003fb190: add      r3, r3, #4
003fb194: ldr      fp, [r3, #0x18]
003fb198: b        #0x3fb090
003fb19c: ldr      r3, [pc, #0xe4]
003fb1a0: ldr      r3, [r5, r3]
003fb1a4: ldr      r3, [r3]
003fb1a8: add      r3, r3, #0x47000
003fb1ac: add      r3, r3, #0xa10
003fb1b0: add      r3, r3, #8
003fb1b4: ldr      fp, [r3, #0x18]
003fb1b8: b        #0x3fb090
003fb1bc: ldr      r3, [pc, #0xc4]
003fb1c0: ldr      r3, [r5, r3]
003fb1c4: ldr      r3, [r3]
003fb1c8: add      r3, r3, #0x39c00
003fb1cc: add      r3, r3, #0x9c
003fb1d0: ldr      fp, [r3, #0x18]
003fb1d4: b        #0x3fb090
003fb1d8: ldr      r3, [pc, #0xa8]
003fb1dc: ldr      r3, [r5, r3]
003fb1e0: ldr      r3, [r3]
003fb1e4: add      r3, r3, #0x3a000
003fb1e8: add      r3, r3, #0x20
003fb1ec: ldr      fp, [r3, #0x18]
003fb1f0: b        #0x3fb090
003fb1f4: ldr      r3, [pc, #0x8c]
003fb1f8: ldr      r3, [r5, r3]
003fb1fc: ldr      r3, [r3]
003fb200: add      r3, r3, #0x3fc00
003fb204: add      r3, r3, #0x30c
003fb208: ldr      fp, [r3, #0x18]
003fb20c: b        #0x3fb090
003fb210: ldr      r3, [pc, #0x70]
003fb214: ldr      r3, [r5, r3]
003fb218: ldr      r3, [r3]
003fb21c: add      r3, r3, #0x3f800
003fb220: add      r3, r3, #0x388
003fb224: ldr      fp, [r3, #0x18]
003fb228: b        #0x3fb090
003fb22c: ldr      r3, [pc, #0x54]
003fb230: ldr      r3, [r5, r3]
003fb234: ldr      r3, [r3]
003fb238: add      r3, r3, #0x40000
003fb23c: add      r3, r3, #0x290
003fb240: ldr      fp, [r3, #0x18]
003fb244: b        #0x3fb090
003fb248: subeq    r0, sp, ip, lsl fp
003fb24c: subseq   sb, sb, ip, ror sp
003fb250: strdeq   r3, r4, [r0], -r4
003fb254: ldrdeq   r3, r4, [ip], #-0xe4
003fb258: strdeq   ip, sp, [ip], #-0x38
003fb25c: subeq    ip, ip, ip, asr #7
003fb260: subeq    r3, ip, ip, lsr #28
003fb264: subeq    ip, ip, r0, asr #5
003fb268: strheq   r3, [ip], #-0xdc
003fb26c: subeq    ip, ip, r0, ror r2
003fb270: subeq    r3, ip, ip, asr #26
003fb274: subeq    ip, ip, r0, lsr #4
003fb278: ldrdeq   r3, r4, [ip], #-0xcc
003fb27c: ldrdeq   ip, sp, [ip], #-0x10
003fb280: subeq    r3, ip, ip, ror #24
003fb284: subeq    ip, ip, r8, ror r1
003fb288: andeq    r2, r0, r0, asr fp
003fb28c: subeq    fp, ip, ip, asr #31

# _ZN9Character24InitializePlayerSavegameEv
003b36b0: push     {r4, r5, r6, lr}
003b36b4: mov      r1, #0
003b36b8: mov      r4, r0
003b36bc: mov      r0, #0x198
003b36c0: bl       #0x310570
003b36c4: mov      r5, r0
003b36c8: bl       #0x465ae0
003b36cc: movw     r3, #0x14e8
003b36d0: mov      r0, r4
003b36d4: mov      r1, r4
003b36d8: str      r5, [r4, r3]
003b36dc: pop      {r4, r5, r6, lr}
003b36e0: b        #0x3bb754

# _ZN9Character10SG_SetSlotEj
003bb740: movw     r3, #0x14e8
003bb744: ldr      r3, [r0, r3]
003bb748: cmp      r3, #0
003bb74c: strne    r1, [r3, #4]
003bb750: bx       lr

# _ZN13ItemInventory15RemoveOnePotionEv
003fe878: ldr      r1, [r0, #0x24]
003fe87c: cmp      r1, #0
003fe880: bxeq     lr
003fe884: ldrsh    r3, [r1, #0x50]
003fe888: cmp      r3, #1
003fe88c: ble      #0x3fe89c
003fe890: mov      r0, r1
003fe894: mvn      r1, #0
003fe898: b        #0x3fa17c
003fe89c: b        #0x3fe7d8
