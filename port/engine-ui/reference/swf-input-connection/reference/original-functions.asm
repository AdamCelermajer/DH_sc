
# _ZN8RenderFX11UpdateInputEii
007ac4bc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ac4c0: ldr      r4, [pc, #0x44c]
007ac4c4: ldr      r6, [pc, #0x44c]
007ac4c8: mov      sb, #0x28
007ac4cc: add      r4, pc, r4
007ac4d0: ldr      r3, [r4, r6]
007ac4d4: mla      sb, sb, r2, r0
007ac4d8: ldr      r3, [r3]
007ac4dc: sub      sp, sp, #0x1ac
007ac4e0: mov      r7, r0
007ac4e4: str      r3, [sp, #0x1a4]
007ac4e8: ldr      r5, [sb, #0x68]
007ac4ec: mov      r8, r2
007ac4f0: mov      sl, r1
007ac4f4: cmp      r5, #0
007ac4f8: beq      #0x7ac514
007ac4fc: mov      r0, r5
007ac500: bl       #0x759c64
007ac504: cmp      sl, #0
007ac508: bne      #0x7ac530
007ac50c: mov      r0, r5
007ac510: bl       #0x75a240
007ac514: ldr      r3, [r4, r6]
007ac518: ldr      r2, [sp, #0x1a4]
007ac51c: ldr      r3, [r3]
007ac520: cmp      r2, r3
007ac524: bne      #0x7ac910
007ac528: add      sp, sp, #0x1ac
007ac52c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ac530: ldr      r3, [sb, #0x74]
007ac534: cmp      r3, #0
007ac538: bne      #0x7ac50c
007ac53c: str      r3, [sp, #0x94]
007ac540: mov      r2, #0
007ac544: mov      r3, #3
007ac548: str      r2, [sp, #0x90]
007ac54c: str      r3, [sp, #0x84]
007ac550: str      r2, [sp, #0x8c]
007ac554: str      r2, [sp, #0x88]
007ac558: str      r5, [sp, #0x7c]
007ac55c: ldr      r2, [r5, #0x44]
007ac560: mov      r0, r7
007ac564: add      r1, sp, #0x7c
007ac568: ldrsb    r3, [r2]
007ac56c: cmn      r3, #1
007ac570: ldreq    r2, [r2, #0xc]
007ac574: mov      r3, #0
007ac578: addne    r2, r2, #1
007ac57c: strb     r3, [sp, #0xa0]
007ac580: strb     r3, [sp, #0xa1]
007ac584: str      r2, [sp, #0x80]
007ac588: str      r8, [sp, #0x9c]
007ac58c: str      sl, [sp, #0x98]
007ac590: bl       #0x7abf34
007ac594: ldrb     r3, [sp, #0xa0]
007ac598: cmp      r3, #0
007ac59c: bne      #0x7ac50c
007ac5a0: mov      r0, r5
007ac5a4: bl       #0x753f74
007ac5a8: add      lr, sp, #0x64
007ac5ac: mov      ip, r0
007ac5b0: ldm      ip!, {r0, r1, r2, r3}
007ac5b4: stm      lr!, {r0, r1, r2, r3}
007ac5b8: ldm      ip, {r0, r1}
007ac5bc: tst      sl, #0xc
007ac5c0: stm      lr, {r0, r1}
007ac5c4: ldr      r3, [sp, #0x6c]
007ac5c8: ldr      lr, [sp, #0x78]
007ac5cc: moveq    sb, #0x41000000
007ac5d0: str      r3, [sp, #4]
007ac5d4: addeq    sb, sb, #0x200000
007ac5d8: movne    sb, #0x3f800000
007ac5dc: ands     r3, sl, #1
007ac5e0: str      lr, [sp]
007ac5e4: str      r3, [sp, #0x2c]
007ac5e8: bne      #0x7ac600
007ac5ec: tst      sl, #2
007ac5f0: moveq    lr, #0x41000000
007ac5f4: addeq    lr, lr, #0x200000
007ac5f8: streq    lr, [sp, #8]
007ac5fc: beq      #0x7ac608
007ac600: mov      r3, #0x3f800000
007ac604: str      r3, [sp, #8]
007ac608: ldr      r2, [pc, #0x30c]
007ac60c: mov      r3, #3
007ac610: mov      r0, r7
007ac614: ldr      r1, [r7, #0x40]
007ac618: add      r2, pc, r2
007ac61c: bl       #0x7a8c08
007ac620: ldr      r3, [r0, #4]
007ac624: cmp      r3, #0
007ac628: ble      #0x7ac8f8
007ac62c: mov      r3, #0x4f000000
007ac630: mov      lr, #0
007ac634: str      r3, [sp, #0xc]
007ac638: str      r3, [sp, #0x1c]
007ac63c: str      r3, [sp, #0x18]
007ac640: str      r3, [sp, #0x14]
007ac644: add      r3, sp, #0x4c
007ac648: str      r5, [sp, #0x30]
007ac64c: str      r7, [sp, #0x34]
007ac650: str      r8, [sp, #0x3c]
007ac654: str      r4, [sp, #0x40]
007ac658: str      lr, [sp, #0x10]
007ac65c: str      lr, [sp, #0x20]
007ac660: str      lr, [sp, #0x24]
007ac664: str      lr, [sp, #0x28]
007ac668: mov      r5, lr
007ac66c: mov      r7, r0
007ac670: str      sl, [sp, #0x38]
007ac674: mov      r8, sb
007ac678: str      r6, [sp, #0x44]
007ac67c: mov      r4, r3
007ac680: ldr      r3, [r7]
007ac684: ldr      r6, [r3, r5, lsl #2]
007ac688: mov      r0, r6
007ac68c: bl       #0x753f74
007ac690: mov      lr, r4
007ac694: mov      ip, r0
007ac698: ldm      ip!, {r0, r1, r2, r3}
007ac69c: stm      lr!, {r0, r1, r2, r3}
007ac6a0: ldm      ip, {r0, r1}
007ac6a4: stm      lr, {r0, r1}
007ac6a8: ldr      r1, [sp, #4]
007ac6ac: ldr      r0, [sp, #0x54]
007ac6b0: bl       #0x30e3ac
007ac6b4: mov      r1, r0
007ac6b8: mov      r0, r8
007ac6bc: bl       #0x30ed6c
007ac6c0: ldr      r1, [sp]
007ac6c4: mov      sl, r0
007ac6c8: ldr      r0, [sp, #0x60]
007ac6cc: bl       #0x30e3ac
007ac6d0: mov      r1, r0
007ac6d4: ldr      r0, [sp, #8]
007ac6d8: bl       #0x30ed6c
007ac6dc: mov      r1, sl
007ac6e0: mov      sb, r0
007ac6e4: mov      r0, sl
007ac6e8: bl       #0x30ed6c
007ac6ec: mov      r1, sb
007ac6f0: mov      fp, r0
007ac6f4: mov      r0, sb
007ac6f8: bl       #0x30ed6c
007ac6fc: mov      r1, r0
007ac700: mov      r0, fp
007ac704: bl       #0x30eba4
007ac708: mov      r1, #0
007ac70c: mov      fp, r0
007ac710: mov      r0, sb
007ac714: bl       #0x30e70c
007ac718: cmp      r0, #0
007ac71c: beq      #0x7ac74c
007ac720: bic      r0, sb, #0x80000000
007ac724: mov      r1, #0
007ac728: bl       #0x30e2f8
007ac72c: cmp      r0, #0
007ac730: beq      #0x7ac74c
007ac734: ldr      r1, [sp, #0x1c]
007ac738: mov      r0, fp
007ac73c: bl       #0x30e70c
007ac740: cmp      r0, #0
007ac744: strne    fp, [sp, #0x1c]
007ac748: strne    r6, [sp, #0x20]
007ac74c: mov      r0, sb
007ac750: mov      r1, #0
007ac754: bl       #0x30e2f8
007ac758: cmp      r0, #0
007ac75c: beq      #0x7ac78c
007ac760: bic      r0, sb, #0x80000000
007ac764: mov      r1, #0
007ac768: bl       #0x30e2f8
007ac76c: cmp      r0, #0
007ac770: beq      #0x7ac78c
007ac774: ldr      r0, [sp, #0x18]
007ac778: mov      r1, fp
007ac77c: bl       #0x30e2f8
007ac780: cmp      r0, #0
007ac784: strne    fp, [sp, #0x18]
007ac788: strne    r6, [sp, #0x24]
007ac78c: mov      r0, sl
007ac790: mov      r1, #0
007ac794: bl       #0x30e70c
007ac798: cmp      r0, #0
007ac79c: beq      #0x7ac7cc
007ac7a0: bic      r0, sl, #0x80000000
007ac7a4: mov      r1, #0
007ac7a8: bl       #0x30e2f8
007ac7ac: cmp      r0, #0
007ac7b0: beq      #0x7ac7cc
007ac7b4: ldr      r0, [sp, #0x14]
007ac7b8: mov      r1, fp
007ac7bc: bl       #0x30e2f8
007ac7c0: cmp      r0, #0
007ac7c4: strne    fp, [sp, #0x14]
007ac7c8: strne    r6, [sp, #0x10]
007ac7cc: mov      r0, sl
007ac7d0: mov      r1, #0
007ac7d4: bl       #0x30e2f8
007ac7d8: cmp      r0, #0
007ac7dc: beq      #0x7ac80c
007ac7e0: bic      r0, sl, #0x80000000
007ac7e4: mov      r1, #0
007ac7e8: bl       #0x30e2f8
007ac7ec: cmp      r0, #0
007ac7f0: beq      #0x7ac80c
007ac7f4: ldr      r1, [sp, #0xc]
007ac7f8: mov      r0, fp
007ac7fc: bl       #0x30e70c
007ac800: cmp      r0, #0
007ac804: strne    fp, [sp, #0xc]
007ac808: strne    r6, [sp, #0x28]
007ac80c: ldr      r3, [r7, #4]
007ac810: add      r5, r5, #1
007ac814: cmp      r5, r3
007ac818: blt      #0x7ac680
007ac81c: add      r5, sp, #0x30
007ac820: ldm      r5, {r5, r7, sl}
007ac824: ldr      r8, [sp, #0x3c]
007ac828: ldr      r4, [sp, #0x40]
007ac82c: ldr      r6, [sp, #0x44]
007ac830: ldr      r3, [sp, #0x2c]
007ac834: cmp      r3, #0
007ac838: beq      #0x7ac85c
007ac83c: ldr      lr, [sp, #0x20]
007ac840: cmp      lr, #0
007ac844: beq      #0x7ac85c
007ac848: mov      r0, r7
007ac84c: mov      r1, lr
007ac850: mov      r2, r8
007ac854: bl       #0x7ac228
007ac858: b        #0x7ac50c
007ac85c: tst      sl, #2
007ac860: beq      #0x7ac884
007ac864: ldr      r3, [sp, #0x24]
007ac868: cmp      r3, #0
007ac86c: beq      #0x7ac884
007ac870: mov      r0, r7
007ac874: mov      r1, r3
007ac878: mov      r2, r8
007ac87c: bl       #0x7ac228
007ac880: b        #0x7ac50c
007ac884: tst      sl, #4
007ac888: beq      #0x7ac898
007ac88c: ldr      lr, [sp, #0x10]
007ac890: cmp      lr, #0
007ac894: bne      #0x7ac848
007ac898: tst      sl, #8
007ac89c: beq      #0x7ac8ac
007ac8a0: ldr      r3, [sp, #0x28]
007ac8a4: cmp      r3, #0
007ac8a8: bne      #0x7ac870
007ac8ac: tst      sl, #0x10
007ac8b0: beq      #0x7ac50c
007ac8b4: ldr      r3, [r7, #0xfc]
007ac8b8: cmp      r3, #0
007ac8bc: beq      #0x7ac50c
007ac8c0: ldr      r3, [r7, #0xf8]
007ac8c4: ands     r3, r3, #0x40
007ac8c8: bne      #0x7ac50c
007ac8cc: ldr      r2, [pc, #0x4c]
007ac8d0: mov      r1, r5
007ac8d4: mov      r0, r7
007ac8d8: add      r2, pc, r2
007ac8dc: bl       #0x7aba04
007ac8e0: mov      r0, #0x28
007ac8e4: mla      r0, r0, r8, r7
007ac8e8: mov      r1, r5
007ac8ec: add      r0, r0, #0x74
007ac8f0: bl       #0x75518c
007ac8f4: b        #0x7ac50c
007ac8f8: mov      lr, #0
007ac8fc: str      lr, [sp, #0x10]
007ac900: str      lr, [sp, #0x20]
007ac904: str      lr, [sp, #0x24]
007ac908: str      lr, [sp, #0x28]
007ac90c: b        #0x7ac830
007ac910: bl       #0x30e310
007ac914: andseq   r8, lr, r4, asr #11
007ac918: andeq    r4, r0, ip, lsr #1
007ac91c: andseq   ip, r1, r8, ror #20
007ac920: ldrsbeq  lr, [r5], -r0

# _ZN8RenderFX12UpdateCursorERNS_6CursorEi
007ac924: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ac928: ldr      r5, [pc, #0xd48]
007ac92c: ldr      r8, [pc, #0xd48]
007ac930: sub      sp, sp, #0x2bc
007ac934: add      r5, pc, r5
007ac938: ldr      r3, [r5, r8]
007ac93c: cmp      r2, #3
007ac940: mov      r6, r2
007ac944: ldr      r3, [r3]
007ac948: mov      r4, r0
007ac94c: mov      r7, r1
007ac950: str      r3, [sp, #0x2b4]
007ac954: bhi      #0x7acaf8
007ac958: mov      sl, #0x28
007ac95c: mla      sl, sl, r2, r0
007ac960: ldm      r1, {r0, r1, r2, r3}
007ac964: ldr      lr, [sl, #0x5c]
007ac968: add      ip, sl, #0x58
007ac96c: str      lr, [sp, #0x18]
007ac970: ldr      lr, [sl, #0x58]
007ac974: str      lr, [sp, #0x14]
007ac978: ldr      lr, [sl, #0x64]
007ac97c: str      lr, [sp, #0x1c]
007ac980: stm      ip, {r0, r1, r2, r3}
007ac984: ldr      r1, [r7, #4]
007ac988: ldr      r2, [r7]
007ac98c: ldr      r3, [r4, #0x3c]
007ac990: str      r1, [sp, #0x60]
007ac994: str      r2, [sp, #0x5c]
007ac998: str      r1, [r3, #0x4c]
007ac99c: str      r2, [r3, #0x48]
007ac9a0: ldr      r3, [r4, #0x3c]
007ac9a4: add      r1, sp, #0x5c
007ac9a8: str      r6, [r3, #0x50]
007ac9ac: ldr      r0, [r4, #0x3c]
007ac9b0: bl       #0x773dc0
007ac9b4: ldr      r2, [sp, #0x5c]
007ac9b8: ldr      r3, [sl, #0x70]
007ac9bc: str      r2, [sp, #8]
007ac9c0: ldr      r2, [sp, #0x60]
007ac9c4: cmp      r3, #0
007ac9c8: str      r2, [sp, #0xc]
007ac9cc: beq      #0x7acae4
007ac9d0: add      sl, sp, #0x44
007ac9d4: mov      r2, #0
007ac9d8: add      r3, sl, #0xc
007ac9dc: str      r2, [r3], #4
007ac9e0: str      r2, [r3], #4
007ac9e4: mov      r1, #0x41000000
007ac9e8: mov      ip, #0x3f800000
007ac9ec: str      r2, [r3]
007ac9f0: add      r1, r1, #0xa00000
007ac9f4: ldr      r0, [sp, #8]
007ac9f8: str      ip, [sp, #0x54]
007ac9fc: str      r2, [sp, #0x48]
007aca00: str      ip, [sp, #0x44]
007aca04: bl       #0x30ed6c
007aca08: mov      r1, #0x41000000
007aca0c: mov      fp, r0
007aca10: add      r1, r1, #0xa00000
007aca14: ldr      r0, [sp, #0xc]
007aca18: bl       #0x30ed6c
007aca1c: mov      r1, #0
007aca20: str      r0, [sp, #0x10]
007aca24: bl       #0x30ed6c
007aca28: mov      r1, r0
007aca2c: mov      r0, fp
007aca30: bl       #0x30eba4
007aca34: mov      r1, #0
007aca38: bl       #0x30eba4
007aca3c: mvn      r1, #0x800000
007aca40: mov      sb, r0
007aca44: bl       #0x30e4b4
007aca48: cmp      r0, #0
007aca4c: beq      #0x7ad004
007aca50: mvn      r1, #0x80000000
007aca54: mov      r0, sb
007aca58: sub      r1, r1, #0x800000
007aca5c: bl       #0x30e9ac
007aca60: cmp      r0, #0
007aca64: beq      #0x7ad004
007aca68: ldr      r1, [sp, #0x50]
007aca6c: mov      r0, fp
007aca70: str      sb, [sp, #0x4c]
007aca74: bl       #0x30ed6c
007aca78: mov      r1, r0
007aca7c: ldr      r0, [sp, #0x10]
007aca80: bl       #0x30eba4
007aca84: ldr      r1, [sp, #0x58]
007aca88: bl       #0x30eba4
007aca8c: mvn      r1, #0x800000
007aca90: mov      sb, r0
007aca94: bl       #0x30e4b4
007aca98: cmp      r0, #0
007aca9c: beq      #0x7acffc
007acaa0: mvn      r1, #0x80000000
007acaa4: mov      r0, sb
007acaa8: sub      r1, r1, #0x800000
007acaac: bl       #0x30e9ac
007acab0: cmp      r0, #0
007acab4: beq      #0x7acffc
007acab8: mov      r1, #0x3f800000
007acabc: ldr      r3, [r7, #8]
007acac0: mov      r0, sl
007acac4: mov      r2, r1
007acac8: str      sb, [sp, #0x58]
007acacc: bl       #0x796920
007acad0: mov      r3, #0x28
007acad4: mla      r3, r3, r6, r4
007acad8: mov      r1, sl
007acadc: ldr      r0, [r3, #0x70]
007acae0: bl       #0x4121f8
007acae4: mov      r3, #0x28
007acae8: mla      r3, r3, r6, r4
007acaec: ldrb     r2, [r3, #0x7c]
007acaf0: cmp      r2, #0
007acaf4: bne      #0x7acb14
007acaf8: ldr      r3, [r5, r8]
007acafc: ldr      r2, [sp, #0x2b4]
007acb00: ldr      r3, [r3]
007acb04: cmp      r2, r3
007acb08: bne      #0x7ad640
007acb0c: add      sp, sp, #0x2bc
007acb10: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007acb14: ldr      r2, [r4, #0x40]
007acb18: cmp      r2, #0
007acb1c: beq      #0x7acaf8
007acb20: ldr      r2, [r4, #0xf8]
007acb24: tst      r2, #0x20
007acb28: bne      #0x7ad00c
007acb2c: ldr      r0, [sp, #8]
007acb30: bl       #0x30e4cc
007acb34: mov      sl, r0
007acb38: ldr      r0, [sp, #0xc]
007acb3c: bl       #0x30e4cc
007acb40: ldr      sb, [r4, #0x3c]
007acb44: mov      r2, r0
007acb48: mov      r3, #0
007acb4c: mov      r0, sb
007acb50: mov      r1, sl
007acb54: bl       #0x774128
007acb58: ldr      r3, [r7, #0xc]
007acb5c: cmp      r3, #0
007acb60: bne      #0x7ad0c8
007acb64: ldr      r2, [sp, #0x1c]
007acb68: cmp      r2, #0
007acb6c: ldreq    lr, [sp, #0x1c]
007acb70: strne    r3, [sp, #0x20]
007acb74: movne    r3, #1
007acb78: strne    r3, [sp, #0x1c]
007acb7c: streq    lr, [sp, #0x20]
007acb80: mov      r2, #0
007acb84: str      r2, [sp, #0x18]
007acb88: ldr      r3, [r4, #0xf8]
007acb8c: tst      r3, #4
007acb90: beq      #0x7ad0b0
007acb94: ldr      r3, [r4, #0x3c]
007acb98: ldr      sl, [r3, #0x10]
007acb9c: cmp      sl, #0
007acba0: beq      #0x7ad0bc
007acba4: mov      r0, sl
007acba8: str      sl, [sp, #0x24]
007acbac: bl       #0x759c64
007acbb0: mov      r1, #0x41000000
007acbb4: add      r1, r1, #0xa00000
007acbb8: ldr      r0, [sp, #8]
007acbbc: bl       #0x30ed6c
007acbc0: mov      r1, #0x41000000
007acbc4: str      r0, [sp, #0x14]
007acbc8: add      r1, r1, #0xa00000
007acbcc: ldr      r0, [sp, #0xc]
007acbd0: bl       #0x30ed6c
007acbd4: str      r0, [sp, #0x10]
007acbd8: ldr      r3, [sl]
007acbdc: mov      r0, sl
007acbe0: ldr      r1, [sp, #0x14]
007acbe4: ldr      r2, [sp, #0x10]
007acbe8: mov      lr, pc
007acbec: ldr      pc, [r3, #0x68]
007acbf0: subs     sl, r0, #0
007acbf4: beq      #0x7acbfc
007acbf8: bl       #0x759c64
007acbfc: mov      r3, #0x28
007acc00: mla      r3, r3, r6, r4
007acc04: ldr      fp, [r3, #0x68]
007acc08: cmp      fp, #0
007acc0c: beq      #0x7acc18
007acc10: mov      r0, fp
007acc14: bl       #0x759c64
007acc18: mov      r3, #0x28
007acc1c: mla      r3, r3, r6, r4
007acc20: ldr      r3, [r3, #0x78]
007acc24: cmp      r3, #0
007acc28: beq      #0x7ad1cc
007acc2c: ldr      r3, [r4, #0xf8]
007acc30: tst      r3, #0x80
007acc34: beq      #0x7acc4c
007acc38: cmp      sl, #0
007acc3c: beq      #0x7acc4c
007acc40: ldr      r2, [sp, #0x18]
007acc44: cmp      r2, #0
007acc48: bne      #0x7ad200
007acc4c: ldr      r3, [sp, #0x20]
007acc50: cmp      r3, #0
007acc54: bne      #0x7acc64
007acc58: ldr      lr, [sp, #0x1c]
007acc5c: cmp      lr, #0
007acc60: beq      #0x7acc90
007acc64: mov      r2, #0x28
007acc68: mul      r2, r2, r6
007acc6c: add      r3, r4, r2
007acc70: ldr      r1, [r3, #0x74]
007acc74: ldr      r3, [r3, #0x68]
007acc78: cmp      r3, r1
007acc7c: beq      #0x7acc90
007acc80: add      r2, r4, r2
007acc84: add      r0, r2, #0x74
007acc88: mov      r1, #0
007acc8c: bl       #0x75518c
007acc90: mov      sb, #0x28
007acc94: mul      sb, sb, r6
007acc98: add      r3, r4, sb
007acc9c: ldr      r0, [r3, #0x78]
007acca0: cmp      r0, #0
007acca4: beq      #0x7accc0
007acca8: bl       #0x7a8c6c
007accac: subs     r1, r0, #0
007accb0: bne      #0x7accc0
007accb4: add      r0, sb, #0x78
007accb8: add      r0, r4, r0
007accbc: bl       #0x75518c
007accc0: mov      sb, #0x28
007accc4: mla      sb, sb, r6, r4
007accc8: ldr      r3, [sb, #0x68]
007acccc: cmp      fp, r3
007accd0: beq      #0x7ace48
007accd4: ldr      r0, [sb, #0x6c]
007accd8: cmp      r0, #0
007accdc: beq      #0x7acd90
007acce0: bl       #0x7a8c6c
007acce4: cmp      r0, #0
007acce8: beq      #0x7acd90
007accec: ldr      r1, [sb, #0x6c]
007accf0: add      r0, sp, #0x44
007accf4: ldr      r2, [sp, #8]
007accf8: ldr      r3, [sp, #0xc]
007accfc: bl       #0x7aa2e0
007acd00: ldr      r2, [sb, #0x6c]
007acd04: mov      r1, #0
007acd08: str      r1, [sp, #0x1a4]
007acd0c: mov      r3, #0
007acd10: mov      r1, #9
007acd14: str      r3, [sp, #0x19c]
007acd18: str      r3, [sp, #0x198]
007acd1c: str      r2, [sp, #0x18c]
007acd20: str      r3, [sp, #0x1a0]
007acd24: str      r1, [sp, #0x194]
007acd28: ldr      r1, [r2, #0x44]
007acd2c: ldr      r0, [r7, #0xc]
007acd30: mov      r2, #0
007acd34: ldrsb    r3, [r1]
007acd38: add      sb, sp, #0x18c
007acd3c: cmn      r3, #1
007acd40: ldreq    r1, [r1, #0xc]
007acd44: addne    r1, r1, #1
007acd48: ldr      r3, [r4, #0xfc]
007acd4c: str      r1, [sp, #0x190]
007acd50: ldr      r1, [sp, #0x44]
007acd54: str      r0, [sp, #0x1a4]
007acd58: strb     r2, [sp, #0x1b0]
007acd5c: str      r1, [sp, #0x198]
007acd60: ldr      r1, [sp, #0x48]
007acd64: strb     r2, [sp, #0x1b1]
007acd68: str      r2, [sp, #0x1a8]
007acd6c: str      r1, [sp, #0x19c]
007acd70: str      r6, [sp, #0x1ac]
007acd74: mov      r0, r3
007acd78: mov      r1, sb
007acd7c: ldr      r3, [r3]
007acd80: mov      lr, pc
007acd84: ldr      pc, [r3, #8]
007acd88: cmp      r0, #0
007acd8c: bne      #0x7ad4ec
007acd90: cmp      sl, #0
007acd94: beq      #0x7ace48
007acd98: mov      r0, sl
007acd9c: bl       #0x7a8c6c
007acda0: cmp      r0, #0
007acda4: beq      #0x7ace48
007acda8: add      r0, sp, #0x44
007acdac: mov      r1, sl
007acdb0: ldr      r2, [sp, #8]
007acdb4: ldr      r3, [sp, #0xc]
007acdb8: bl       #0x7aa2e0
007acdbc: mov      r2, #0
007acdc0: str      r2, [sp, #0x1a4]
007acdc4: mov      r3, #0
007acdc8: mov      r2, #8
007acdcc: str      r2, [sp, #0x194]
007acdd0: str      r3, [sp, #0x19c]
007acdd4: str      r3, [sp, #0x198]
007acdd8: str      r3, [sp, #0x1a0]
007acddc: str      sl, [sp, #0x18c]
007acde0: ldr      r1, [sl, #0x44]
007acde4: ldr      r0, [r7, #0xc]
007acde8: mov      r2, #0
007acdec: ldrsb    r3, [r1]
007acdf0: add      sb, sp, #0x18c
007acdf4: cmn      r3, #1
007acdf8: ldreq    r1, [r1, #0xc]
007acdfc: addne    r1, r1, #1
007ace00: ldr      r3, [r4, #0xfc]
007ace04: str      r1, [sp, #0x190]
007ace08: ldr      r1, [sp, #0x44]
007ace0c: str      r0, [sp, #0x1a4]
007ace10: strb     r2, [sp, #0x1b0]
007ace14: str      r1, [sp, #0x198]
007ace18: ldr      r1, [sp, #0x48]
007ace1c: strb     r2, [sp, #0x1b1]
007ace20: str      r2, [sp, #0x1a8]
007ace24: str      r1, [sp, #0x19c]
007ace28: str      r6, [sp, #0x1ac]
007ace2c: mov      r0, r3
007ace30: mov      r1, sb
007ace34: ldr      r3, [r3]
007ace38: mov      lr, pc
007ace3c: ldr      pc, [r3, #8]
007ace40: cmp      r0, #0
007ace44: bne      #0x7ad4dc
007ace48: ldr      r2, [sp, #0x18]
007ace4c: cmp      r2, #0
007ace50: beq      #0x7ace7c
007ace54: mov      sb, #0x28
007ace58: mla      sb, sb, r6, r4
007ace5c: ldr      r0, [sb, #0x68]
007ace60: cmp      r0, #0
007ace64: beq      #0x7ace7c
007ace68: ldr      r3, [sb, #0x6c]
007ace6c: cmp      r0, r3
007ace70: beq      #0x7ad2cc
007ace74: cmp      sl, r0
007ace78: beq      #0x7ad524
007ace7c: mov      r3, #0x28
007ace80: mul      r3, r3, r6
007ace84: mov      r1, sl
007ace88: add      r0, r4, r3
007ace8c: add      r0, r0, #0x6c
007ace90: add      r3, r4, r3
007ace94: str      r3, [sp, #0xc]
007ace98: bl       #0x75518c
007ace9c: ldr      r3, [sp, #0xc]
007acea0: ldr      sb, [r3, #0x68]
007acea4: cmp      sb, #0
007acea8: beq      #0x7acfd0
007aceac: mov      r0, sb
007aceb0: bl       #0x759c64
007aceb4: mov      r0, sb
007aceb8: bl       #0x7a8c6c
007acebc: cmp      r0, #0
007acec0: beq      #0x7acfc8
007acec4: mov      r0, sb
007acec8: bl       #0x753f74
007acecc: add      ip, sp, #0x44
007aced0: mov      lr, r0
007aced4: ldm      lr!, {r0, r1, r2, r3}
007aced8: stm      ip!, {r0, r1, r2, r3}
007acedc: str      lr, [sp, #8]
007acee0: ldr      r2, [sp, #8]
007acee4: add      lr, sp, #0x2c
007acee8: add      r3, lr, #8
007aceec: ldm      r2, {r0, r1}
007acef0: mov      r2, #0
007acef4: str      r2, [r3], #4
007acef8: str      r2, [r3], #4
007acefc: str      r2, [r3], #4
007acf00: stm      ip, {r0, r1}
007acf04: str      r2, [r3]
007acf08: mov      r0, lr
007acf0c: mov      r3, #0x3f800000
007acf10: add      r1, sp, #0x44
007acf14: str      r2, [sp, #0x30]
007acf18: str      r3, [sp, #0x3c]
007acf1c: str      r3, [sp, #0x2c]
007acf20: bl       #0x795adc
007acf24: ldr      r1, [sp, #0x2c]
007acf28: ldr      r0, [sp, #0x14]
007acf2c: bl       #0x30ed6c
007acf30: ldr      r1, [sp, #0x30]
007acf34: mov      r3, r0
007acf38: ldr      r0, [sp, #0x10]
007acf3c: str      r3, [sp, #4]
007acf40: bl       #0x30ed6c
007acf44: ldr      r3, [sp, #4]
007acf48: mov      r1, r0
007acf4c: mov      r0, r3
007acf50: bl       #0x30eba4
007acf54: ldr      r1, [sp, #0x34]
007acf58: bl       #0x30eba4
007acf5c: ldr      r1, [sp, #0x38]
007acf60: str      r0, [sp, #8]
007acf64: ldr      r0, [sp, #0x14]
007acf68: bl       #0x30ed6c
007acf6c: ldr      r1, [sp, #0x3c]
007acf70: mov      r3, r0
007acf74: ldr      r0, [sp, #0x10]
007acf78: str      r3, [sp, #4]
007acf7c: bl       #0x30ed6c
007acf80: ldr      r3, [sp, #4]
007acf84: mov      r1, r0
007acf88: mov      r0, r3
007acf8c: bl       #0x30eba4
007acf90: ldr      r1, [sp, #0x40]
007acf94: bl       #0x30eba4
007acf98: ldr      lr, [sp, #0x20]
007acf9c: mov      ip, r0
007acfa0: cmp      lr, #0
007acfa4: beq      #0x7ad114
007acfa8: ldr      r3, [r4, #0xf8]
007acfac: tst      r3, #1
007acfb0: bne      #0x7ad01c
007acfb4: cmp      sl, #0
007acfb8: bne      #0x7ad01c
007acfbc: mov      r0, r4
007acfc0: mov      r1, r6
007acfc4: bl       #0x7ac410
007acfc8: mov      r0, sb
007acfcc: bl       #0x75a240
007acfd0: cmp      fp, #0
007acfd4: beq      #0x7acfe0
007acfd8: mov      r0, fp
007acfdc: bl       #0x75a240
007acfe0: cmp      sl, #0
007acfe4: beq      #0x7acff0
007acfe8: mov      r0, sl
007acfec: bl       #0x75a240
007acff0: ldr      r0, [sp, #0x24]
007acff4: bl       #0x75a240
007acff8: b        #0x7acaf8
007acffc: mov      sb, #0
007ad000: b        #0x7acab8
007ad004: mov      sb, #0
007ad008: b        #0x7aca68
007ad00c: ldr      r3, [r3, #0x74]
007ad010: cmp      r3, #0
007ad014: bne      #0x7acaf8
007ad018: b        #0x7acb2c
007ad01c: ands     r3, r3, #0x40
007ad020: beq      #0x7ad2ac
007ad024: mov      r2, #0
007ad028: str      r2, [sp, #0x1a4]
007ad02c: mov      r3, #0
007ad030: mov      r2, #4
007ad034: str      r3, [sp, #0x19c]
007ad038: str      r3, [sp, #0x198]
007ad03c: str      r3, [sp, #0x1a0]
007ad040: str      r2, [sp, #0x194]
007ad044: str      sb, [sp, #0x18c]
007ad048: ldr      r2, [sb, #0x44]
007ad04c: ldr      lr, [r7, #0xc]
007ad050: mov      r0, r4
007ad054: ldrsb    r3, [r2]
007ad058: add      r1, sp, #0x18c
007ad05c: cmn      r3, #1
007ad060: ldreq    r2, [r2, #0xc]
007ad064: addne    r2, r2, #1
007ad068: mov      r3, #0
007ad06c: str      r2, [sp, #0x190]
007ad070: ldr      r2, [sp, #8]
007ad074: strb     r3, [sp, #0x1b0]
007ad078: str      lr, [sp, #0x1a4]
007ad07c: str      r2, [sp, #0x198]
007ad080: str      ip, [sp, #0x19c]
007ad084: strb     r3, [sp, #0x1b1]
007ad088: str      r3, [sp, #0x1a8]
007ad08c: str      r6, [sp, #0x1ac]
007ad090: bl       #0x7abf34
007ad094: mov      r0, #0x28
007ad098: mul      r0, r0, r6
007ad09c: mov      r1, sb
007ad0a0: add      r0, r0, #0x78
007ad0a4: add      r0, r4, r0
007ad0a8: bl       #0x75518c
007ad0ac: b        #0x7acfc8
007ad0b0: ldr      sl, [r4, #0x40]
007ad0b4: cmp      sl, #0
007ad0b8: bne      #0x7acba4
007ad0bc: mov      r2, #0
007ad0c0: str      r2, [sp, #0x24]
007ad0c4: b        #0x7acbb0
007ad0c8: ldr      r0, [r7]
007ad0cc: ldr      r1, [sp, #0x14]
007ad0d0: bl       #0x30df8c
007ad0d4: ldr      r3, [sp, #0x1c]
007ad0d8: rsbs     r3, r3, #1
007ad0dc: movlo    r3, #0
007ad0e0: cmp      r0, #0
007ad0e4: str      r3, [sp, #0x20]
007ad0e8: beq      #0x7ad100
007ad0ec: ldr      r1, [sp, #0x18]
007ad0f0: ldr      r0, [r7, #4]
007ad0f4: bl       #0x30df8c
007ad0f8: cmp      r0, #0
007ad0fc: bne      #0x7ad214
007ad100: mov      r3, #0
007ad104: mov      lr, #1
007ad108: str      r3, [sp, #0x1c]
007ad10c: str      lr, [sp, #0x18]
007ad110: b        #0x7acb88
007ad114: ldr      r3, [sp, #0x1c]
007ad118: cmp      r3, #0
007ad11c: beq      #0x7ad220
007ad120: ldr      r3, [r4, #0xf8]
007ad124: tst      r3, #1
007ad128: bne      #0x7ad3bc
007ad12c: cmp      sl, sb
007ad130: beq      #0x7ad3bc
007ad134: ldr      lr, [sp, #0x20]
007ad138: mov      r3, #0
007ad13c: mov      r2, #7
007ad140: str      lr, [sp, #0x1a4]
007ad144: str      r3, [sp, #0x19c]
007ad148: str      r3, [sp, #0x198]
007ad14c: str      r3, [sp, #0x1a0]
007ad150: str      r2, [sp, #0x194]
007ad154: str      sb, [sp, #0x18c]
007ad158: ldr      r2, [sb, #0x44]
007ad15c: ldr      lr, [r7, #0xc]
007ad160: mov      r0, r4
007ad164: ldrsb    r3, [r2]
007ad168: add      r1, sp, #0x18c
007ad16c: cmn      r3, #1
007ad170: ldreq    r2, [r2, #0xc]
007ad174: addne    r2, r2, #1
007ad178: mov      r3, #0
007ad17c: str      r2, [sp, #0x190]
007ad180: ldr      r2, [sp, #8]
007ad184: strb     r3, [sp, #0x1b0]
007ad188: str      lr, [sp, #0x1a4]
007ad18c: str      r2, [sp, #0x198]
007ad190: str      ip, [sp, #0x19c]
007ad194: strb     r3, [sp, #0x1b1]
007ad198: str      r3, [sp, #0x1a8]
007ad19c: str      r6, [sp, #0x1ac]
007ad1a0: bl       #0x7abf34
007ad1a4: mov      r0, r4
007ad1a8: mov      r1, r6
007ad1ac: bl       #0x7ac410
007ad1b0: mov      r0, #0x28
007ad1b4: mul      r0, r0, r6
007ad1b8: mov      r1, #0
007ad1bc: add      r0, r0, #0x78
007ad1c0: add      r0, r4, r0
007ad1c4: bl       #0x75518c
007ad1c8: b        #0x7acfc8
007ad1cc: ldr      r3, [sp, #0x20]
007ad1d0: cmp      r3, #0
007ad1d4: ldr      r3, [r4, #0xf8]
007ad1d8: bne      #0x7ad200
007ad1dc: tst      r3, #0x10
007ad1e0: beq      #0x7ad200
007ad1e4: tst      r3, #0x80
007ad1e8: beq      #0x7acc58
007ad1ec: ldr      lr, [sp, #0x18]
007ad1f0: cmp      lr, #0
007ad1f4: beq      #0x7acc58
007ad1f8: cmp      sl, #0
007ad1fc: beq      #0x7acc58
007ad200: mov      r0, r4
007ad204: mov      r1, sl
007ad208: mov      r2, r6
007ad20c: bl       #0x7ac228
007ad210: b        #0x7acc4c
007ad214: mov      lr, #0
007ad218: str      lr, [sp, #0x1c]
007ad21c: b        #0x7acb80
007ad220: ldr      r2, [sp, #0x18]
007ad224: cmp      r2, #0
007ad228: beq      #0x7ad4fc
007ad22c: ldr      r3, [r4, #0xf8]
007ad230: tst      r3, #0x40
007ad234: bne      #0x7ad094
007ad238: ldr      lr, [sp, #0x1c]
007ad23c: mov      r3, #0
007ad240: mov      r2, #5
007ad244: str      lr, [sp, #0x7c]
007ad248: str      r3, [sp, #0x74]
007ad24c: str      r3, [sp, #0x70]
007ad250: str      r3, [sp, #0x78]
007ad254: str      r2, [sp, #0x6c]
007ad258: str      sb, [sp, #0x64]
007ad25c: ldr      r2, [sb, #0x44]
007ad260: ldr      lr, [r7, #0xc]
007ad264: mov      r0, r4
007ad268: ldrsb    r3, [r2]
007ad26c: add      r1, sp, #0x64
007ad270: cmn      r3, #1
007ad274: ldreq    r2, [r2, #0xc]
007ad278: addne    r2, r2, #1
007ad27c: mov      r3, #0
007ad280: str      r2, [sp, #0x68]
007ad284: ldr      r2, [sp, #8]
007ad288: strb     r3, [sp, #0x88]
007ad28c: str      lr, [sp, #0x7c]
007ad290: str      r2, [sp, #0x70]
007ad294: str      ip, [sp, #0x74]
007ad298: strb     r3, [sp, #0x89]
007ad29c: str      r3, [sp, #0x80]
007ad2a0: str      r6, [sp, #0x84]
007ad2a4: bl       #0x7abf34
007ad2a8: b        #0x7ad094
007ad2ac: ldr      r2, [pc, #0x3cc]
007ad2b0: mov      r0, r4
007ad2b4: mov      r1, sb
007ad2b8: add      r2, pc, r2
007ad2bc: str      ip, [sp, #4]
007ad2c0: bl       #0x7aba04
007ad2c4: ldr      ip, [sp, #4]
007ad2c8: b        #0x7ad024
007ad2cc: cmp      sl, r0
007ad2d0: moveq    r0, sl
007ad2d4: moveq    r3, sl
007ad2d8: beq      #0x7ad3b0
007ad2dc: bl       #0x7a8c6c
007ad2e0: cmp      r0, #0
007ad2e4: beq      #0x7ad3a8
007ad2e8: ldr      r2, [sb, #0x68]
007ad2ec: mov      r1, #0
007ad2f0: str      r1, [sp, #0x1a4]
007ad2f4: mov      r3, #0
007ad2f8: mov      r1, #0xb
007ad2fc: str      r3, [sp, #0x1a0]
007ad300: str      r1, [sp, #0x194]
007ad304: str      r3, [sp, #0x19c]
007ad308: str      r3, [sp, #0x198]
007ad30c: str      r2, [sp, #0x18c]
007ad310: ldr      r2, [r2, #0x44]
007ad314: mov      sb, #0x28
007ad318: mla      sb, sb, r6, r4
007ad31c: ldrsb    r3, [r2]
007ad320: add      r0, sp, #0x44
007ad324: cmn      r3, #1
007ad328: ldreq    r2, [r2, #0xc]
007ad32c: addne    r2, r2, #1
007ad330: mov      r3, #0
007ad334: str      r2, [sp, #0x190]
007ad338: strb     r3, [sp, #0x1b0]
007ad33c: strb     r3, [sp, #0x1b1]
007ad340: str      r3, [sp, #0x1a8]
007ad344: str      r6, [sp, #0x1ac]
007ad348: ldr      r1, [sb, #0x68]
007ad34c: ldr      r2, [sp, #8]
007ad350: ldr      r3, [sp, #0xc]
007ad354: bl       #0x7aa2e0
007ad358: ldr      r2, [r7, #0xc]
007ad35c: ldr      r3, [r4, #0xfc]
007ad360: str      r2, [sp, #0x1a4]
007ad364: ldr      r2, [sp, #0x44]
007ad368: mov      r0, r3
007ad36c: str      r2, [sp, #0x198]
007ad370: ldr      r2, [sp, #0x48]
007ad374: str      r2, [sp, #0x19c]
007ad378: add      r2, sp, #0x18c
007ad37c: ldr      r3, [r3]
007ad380: mov      r1, r2
007ad384: str      r2, [sp, #4]
007ad388: mov      lr, pc
007ad38c: ldr      pc, [r3, #8]
007ad390: cmp      r0, #0
007ad394: ldr      r2, [sp, #4]
007ad398: beq      #0x7ad3a8
007ad39c: mov      r1, r2
007ad3a0: mov      r0, r4
007ad3a4: bl       #0x7abf34
007ad3a8: ldr      r0, [sb, #0x68]
007ad3ac: ldr      r3, [sb, #0x6c]
007ad3b0: cmp      r0, r3
007ad3b4: beq      #0x7ace7c
007ad3b8: b        #0x7ace74
007ad3bc: mov      r2, #0
007ad3c0: mov      r3, #0
007ad3c4: str      r2, [sp, #0x1a4]
007ad3c8: mov      r2, #6
007ad3cc: str      r3, [sp, #0x1a0]
007ad3d0: str      r2, [sp, #0x194]
007ad3d4: str      r3, [sp, #0x19c]
007ad3d8: str      r3, [sp, #0x198]
007ad3dc: str      sb, [sp, #0x18c]
007ad3e0: ldr      r3, [sb, #0x44]
007ad3e4: ldrsb    r2, [r3]
007ad3e8: cmn      r2, #1
007ad3ec: addne    r1, r3, #1
007ad3f0: beq      #0x7ad5f8
007ad3f4: ldr      r0, [r7, #0xc]
007ad3f8: ldr      lr, [sp, #8]
007ad3fc: ldr      r3, [r4, #0xfc]
007ad400: mov      r2, #0
007ad404: strb     r2, [sp, #0x1b0]
007ad408: strb     r2, [sp, #0x1b1]
007ad40c: str      r2, [sp, #0x1a8]
007ad410: add      r2, sp, #0x18c
007ad414: str      r1, [sp, #0x190]
007ad418: str      r0, [sp, #0x1a4]
007ad41c: str      ip, [sp, #0x19c]
007ad420: str      r6, [sp, #0x1ac]
007ad424: str      lr, [sp, #0x198]
007ad428: str      r2, [sp, #0xc]
007ad42c: mov      r0, r3
007ad430: mov      r1, r2
007ad434: ldr      r3, [r3]
007ad438: str      ip, [sp, #4]
007ad43c: mov      lr, pc
007ad440: ldr      pc, [r3, #8]
007ad444: cmp      r0, #0
007ad448: ldr      ip, [sp, #4]
007ad44c: beq      #0x7ad1b0
007ad450: ldr      r3, [r4, #0xf8]
007ad454: ands     r3, r3, #0x40
007ad458: beq      #0x7ad600
007ad45c: ldr      r1, [sp, #0xc]
007ad460: mov      r0, r4
007ad464: str      ip, [sp, #4]
007ad468: bl       #0x7abf34
007ad46c: ldr      ip, [sp, #4]
007ad470: mov      r2, #0
007ad474: str      r2, [sp, #0x7c]
007ad478: mov      r3, #0
007ad47c: mov      r2, #2
007ad480: str      r3, [sp, #0x74]
007ad484: str      r3, [sp, #0x70]
007ad488: str      r3, [sp, #0x78]
007ad48c: str      r2, [sp, #0x6c]
007ad490: str      sb, [sp, #0x64]
007ad494: ldr      r2, [sb, #0x44]
007ad498: ldr      lr, [sp, #8]
007ad49c: mov      r0, r4
007ad4a0: ldrsb    r3, [r2]
007ad4a4: add      r1, sp, #0x64
007ad4a8: cmn      r3, #1
007ad4ac: ldreq    r2, [r2, #0xc]
007ad4b0: mov      r3, #0
007ad4b4: addne    r2, r2, #1
007ad4b8: str      r2, [sp, #0x68]
007ad4bc: strb     r3, [sp, #0x88]
007ad4c0: str      lr, [sp, #0x70]
007ad4c4: str      ip, [sp, #0x74]
007ad4c8: strb     r3, [sp, #0x89]
007ad4cc: str      r3, [sp, #0x80]
007ad4d0: str      r6, [sp, #0x84]
007ad4d4: bl       #0x7abf34
007ad4d8: b        #0x7ad1b0
007ad4dc: mov      r1, sb
007ad4e0: mov      r0, r4
007ad4e4: bl       #0x7abf34
007ad4e8: b        #0x7ace48
007ad4ec: mov      r1, sb
007ad4f0: mov      r0, r4
007ad4f4: bl       #0x7abf34
007ad4f8: b        #0x7acd90
007ad4fc: ldr      r3, [r4, #0xf8]
007ad500: tst      r3, #1
007ad504: bne      #0x7acfc8
007ad508: cmp      sl, #0
007ad50c: bne      #0x7acfc8
007ad510: ldr      lr, [sp, #0xc]
007ad514: ldr      r3, [lr, #0x78]
007ad518: cmp      r3, #0
007ad51c: bne      #0x7acfc8
007ad520: b        #0x7acfbc
007ad524: mov      r0, sl
007ad528: bl       #0x7a8c6c
007ad52c: cmp      r0, #0
007ad530: beq      #0x7ace7c
007ad534: mov      r3, #0x28
007ad538: mla      r2, r3, r6, r4
007ad53c: mov      r1, #0
007ad540: ldr      r2, [r2, #0x68]
007ad544: mov      r3, #0
007ad548: str      r1, [sp, #0x1a4]
007ad54c: mov      r1, #0xa
007ad550: str      r2, [sp, #0x18c]
007ad554: str      r3, [sp, #0x1a0]
007ad558: str      r1, [sp, #0x194]
007ad55c: str      r3, [sp, #0x19c]
007ad560: str      r3, [sp, #0x198]
007ad564: ldr      r1, [r2, #0x44]
007ad568: mov      r2, #0x28
007ad56c: mla      r2, r2, r6, r4
007ad570: ldrsb    r3, [r1]
007ad574: add      r0, sp, #0x44
007ad578: add      sb, sp, #0x18c
007ad57c: cmn      r3, #1
007ad580: ldreq    r1, [r1, #0xc]
007ad584: addne    r1, r1, #1
007ad588: mov      r3, #0
007ad58c: strb     r3, [sp, #0x1b0]
007ad590: strb     r3, [sp, #0x1b1]
007ad594: str      r3, [sp, #0x1a8]
007ad598: str      r1, [sp, #0x190]
007ad59c: str      r6, [sp, #0x1ac]
007ad5a0: ldr      r1, [r2, #0x68]
007ad5a4: ldr      r3, [sp, #0xc]
007ad5a8: ldr      r2, [sp, #8]
007ad5ac: bl       #0x7aa2e0
007ad5b0: ldr      r2, [r7, #0xc]
007ad5b4: ldr      r3, [r4, #0xfc]
007ad5b8: mov      r1, sb
007ad5bc: str      r2, [sp, #0x1a4]
007ad5c0: ldr      r2, [sp, #0x44]
007ad5c4: mov      r0, r3
007ad5c8: str      r2, [sp, #0x198]
007ad5cc: ldr      r2, [sp, #0x48]
007ad5d0: str      r2, [sp, #0x19c]
007ad5d4: ldr      r3, [r3]
007ad5d8: mov      lr, pc
007ad5dc: ldr      pc, [r3, #8]
007ad5e0: cmp      r0, #0
007ad5e4: beq      #0x7ace7c
007ad5e8: mov      r1, sb
007ad5ec: mov      r0, r4
007ad5f0: bl       #0x7abf34
007ad5f4: b        #0x7ace7c
007ad5f8: ldr      r1, [r3, #0xc]
007ad5fc: b        #0x7ad3f4
007ad600: ldr      r2, [pc, #0x7c]
007ad604: mov      r0, r4
007ad608: mov      r1, sb
007ad60c: add      r2, pc, r2
007ad610: bl       #0x7aba04
007ad614: subs     r3, r0, #0
007ad618: beq      #0x7ad644
007ad61c: ldr      r1, [sp, #0xc]
007ad620: mov      r0, r4
007ad624: bl       #0x7abf34
007ad628: mov      r0, #0x28
007ad62c: mla      r0, r0, r6, r4
007ad630: mov      r1, sb
007ad634: add      r0, r0, #0x74
007ad638: bl       #0x75518c
007ad63c: b        #0x7ad1b0
007ad640: bl       #0x30e310
007ad644: ldr      r2, [pc, #0x3c]
007ad648: mov      r1, sb
007ad64c: mov      r0, r4
007ad650: add      r2, pc, r2
007ad654: bl       #0x7aba04
007ad658: ldr      r1, [sp, #0xc]
007ad65c: mov      r7, r0
007ad660: mov      r0, r4
007ad664: bl       #0x7abf34
007ad668: cmp      r7, #0
007ad66c: ldr      ip, [sp, #4]
007ad670: bne      #0x7ad628
007ad674: b        #0x7ad470
007ad678: andseq   r8, lr, ip, asr r1
007ad67c: andeq    r4, r0, ip, lsr #1
007ad680: ldrsheq  sp, [r5], -r8
007ad684: andseq   sp, r5, ip, lsr #7
007ad688: andseq   sp, r5, r8, asr r3

# _ZN8RenderFX6UpdateEib
007ad68c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007ad690: ldr      r5, [pc, #0x148]
007ad694: ldr      r7, [pc, #0x148]
007ad698: sub      sp, sp, #0x13c
007ad69c: add      r5, pc, r5
007ad6a0: ldr      r3, [r5, r7]
007ad6a4: mov      r4, r0
007ad6a8: ldr      r0, [r0, #0x38]
007ad6ac: ldr      r3, [r3]
007ad6b0: mov      sl, r1
007ad6b4: mov      r8, r2
007ad6b8: str      r3, [sp, #0x134]
007ad6bc: bl       #0x76d5b4
007ad6c0: subs     r6, r0, #0
007ad6c4: beq      #0x7ad6cc
007ad6c8: bl       #0x759c64
007ad6cc: mov      r0, sl
007ad6d0: bl       #0x30e964
007ad6d4: mov      r1, #0x44000000
007ad6d8: add      r1, r1, #0x7a0000
007ad6dc: bl       #0x30ec94
007ad6e0: mov      r2, r8
007ad6e4: mov      r1, r0
007ad6e8: mov      r0, r6
007ad6ec: bl       #0x775304
007ad6f0: ldr      r8, [r4, #0xf8]
007ad6f4: ands     r8, r8, #0x40
007ad6f8: bne      #0x7ad744
007ad6fc: add      r2, sp, #0xc
007ad700: mov      fp, #0
007ad704: mov      sl, r4
007ad708: mov      sb, r8
007ad70c: str      r2, [sp, #4]
007ad710: ldr      r3, [sl, #0x74]
007ad714: cmp      r3, #0
007ad718: beq      #0x7ad734
007ad71c: mov      r0, r3
007ad720: ldr      r3, [r3]
007ad724: mov      lr, pc
007ad728: ldr      pc, [r3, #0x98]
007ad72c: cmp      r0, #1
007ad730: beq      #0x7ad770
007ad734: add      r8, r8, #1
007ad738: cmp      r8, #4
007ad73c: add      sl, sl, #0x28
007ad740: bne      #0x7ad710
007ad744: cmp      r6, #0
007ad748: beq      #0x7ad754
007ad74c: mov      r0, r6
007ad750: bl       #0x75a240
007ad754: ldr      r3, [r5, r7]
007ad758: ldr      r2, [sp, #0x134]
007ad75c: ldr      r3, [r3]
007ad760: cmp      r2, r3
007ad764: bne      #0x7ad7dc
007ad768: add      sp, sp, #0x13c
007ad76c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007ad770: ldr      r3, [sl, #0x74]
007ad774: mov      r2, #2
007ad778: str      fp, [sp, #0x1c]
007ad77c: str      fp, [sp, #0x18]
007ad780: str      fp, [sp, #0x20]
007ad784: str      sb, [sp, #0x24]
007ad788: str      r2, [sp, #0x14]
007ad78c: str      r3, [sp, #0xc]
007ad790: ldr      r3, [r3, #0x44]
007ad794: mov      r0, r4
007ad798: ldr      r1, [sp, #4]
007ad79c: ldrsb    r2, [r3]
007ad7a0: cmn      r2, #1
007ad7a4: ldreq    r3, [r3, #0xc]
007ad7a8: addne    r3, r3, #1
007ad7ac: strb     sb, [sp, #0x31]
007ad7b0: str      r3, [sp, #0x10]
007ad7b4: str      sb, [sp, #0x28]
007ad7b8: strb     sb, [sp, #0x30]
007ad7bc: str      r8, [sp, #0x2c]
007ad7c0: bl       #0x7abf34
007ad7c4: mov      r3, #0x28
007ad7c8: mla      r0, r3, r8, r4
007ad7cc: mov      r1, sb
007ad7d0: add      r0, r0, #0x74
007ad7d4: bl       #0x75518c
007ad7d8: b        #0x7ad734
007ad7dc: bl       #0x30e310
007ad7e0: ldrsheq  r7, [lr], -r4
007ad7e4: andeq    r4, r0, ip, lsr #1
