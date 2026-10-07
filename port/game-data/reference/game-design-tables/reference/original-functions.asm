
# _ZN7Structs19GetMemberIDByStringINS_7AIPropsEEEiPKc
004af590: ldr      r3, [pc, #0x48]
004af594: ldr      r2, [pc, #0x48]
004af598: push     {r4, r5, r6, lr}
004af59c: add      r3, pc, r3
004af5a0: mov      r6, r0
004af5a4: ldr      r5, [r3, r2]
004af5a8: mov      r4, #0
004af5ac: ldr      r1, [r5, #0x14]
004af5b0: mov      r0, r6
004af5b4: bl       #0x30e31c
004af5b8: cmp      r0, #0
004af5bc: beq      #0x4af5d8
004af5c0: add      r4, r4, #1
004af5c4: cmp      r4, #0xf
004af5c8: add      r5, r5, #0x18
004af5cc: bne      #0x4af5ac
004af5d0: mvn      r0, #0
004af5d4: pop      {r4, r5, r6, pc}
004af5d8: mov      r0, r4
004af5dc: pop      {r4, r5, r6, pc}
004af5e0: strdeq   r5, r6, [lr], #-0x44
004af5e4: andeq    r1, r0, r0, lsl #29

# _ZN7Structs19GetMemberIDByStringINS_13ClassFuncListEEEiPKc
004af1e0: ldr      r3, [pc, #0x20]
004af1e4: ldr      r2, [pc, #0x20]
004af1e8: push     {r4, lr}
004af1ec: add      r3, pc, r3
004af1f0: ldr      r2, [r3, r2]
004af1f4: ldr      r1, [r2, #0x14]
004af1f8: bl       #0x30e31c
004af1fc: cmp      r0, #0
004af200: mvnne    r0, #0
004af204: pop      {r4, pc}
004af208: subeq    r5, lr, r4, lsr #17
004af20c: andeq    r1, r0, r4, ror sp

# _ZN12PyDataArrays19registerClassByNameEPKcPFiS1_E
004bdccc: push     {r4, r5, lr}
004bdcd0: ldr      r3, [pc, #0x80]
004bdcd4: sub      sp, sp, #0x14
004bdcd8: subs     r4, r2, #0
004bdcdc: str      r1, [sp, #0xc]
004bdce0: add      r3, pc, r3
004bdce4: mov      r5, r0
004bdce8: beq      #0x4bdd04
004bdcec: add      r0, r5, #0x1c
004bdcf0: add      r1, sp, #0xc
004bdcf4: bl       #0x4bdb8c
004bdcf8: str      r4, [r0]
004bdcfc: add      sp, sp, #0x14
004bdd00: pop      {r4, r5, pc}
004bdd04: ldr      r2, [pc, #0x50]
004bdd08: ldr      r2, [r3, r2]
004bdd0c: ldr      r2, [r2]
004bdd10: cmp      r2, #2
004bdd14: streq    r4, [r4]
004bdd18: beq      #0x4bdcec
004bdd1c: cmp      r2, #1
004bdd20: bne      #0x4bdcec
004bdd24: ldr      r0, [pc, #0x34]
004bdd28: ldr      r1, [pc, #0x34]
004bdd2c: ldr      r2, [pc, #0x34]
004bdd30: ldr      r0, [r3, r0]
004bdd34: ldr      r3, [pc, #0x30]
004bdd38: mov      ip, #0x46
004bdd3c: add      r1, pc, r1
004bdd40: add      r2, pc, r2
004bdd44: add      r3, pc, r3
004bdd48: add      r0, r0, #0xa8
004bdd4c: str      ip, [sp]
004bdd50: bl       #0x30e004
004bdd54: b        #0x4bdcec
004bdd58: strheq   r6, [sp], #-0xd0
004bdd5c: andeq    r3, r0, r0, asr #19
004bdd60: andeq    r1, r0, r0, asr #19
004bdd64: umaaleq  r0, r0, ip, r6
004bdd68: subeq    sl, r1, r0, ror r0
004bdd6c: subeq    sl, r1, r4, ror r0

# _ZNSt3mapISsPFiPKcESt4lessISsESaISt4pairIKSsS3_EEEixIS1_EERS3_RKT_
004bdb8c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004bdb90: ldr      r4, [pc, #0x12c]
004bdb94: ldr      r7, [pc, #0x12c]
004bdb98: sub      sp, sp, #0x6c
004bdb9c: add      r4, pc, r4
004bdba0: ldr      r3, [r4, r7]
004bdba4: mov      r8, r0
004bdba8: mov      r6, r1
004bdbac: ldr      r3, [r3]
004bdbb0: str      r3, [sp, #0x64]
004bdbb4: bl       #0x4bcd9c
004bdbb8: cmp      r0, r8
004bdbbc: mov      r5, r0
004bdbc0: beq      #0x4bdc54
004bdbc4: add      sl, sp, #0x4c
004bdbc8: ldr      r1, [r6]
004bdbcc: add      r2, sp, #0x14
004bdbd0: mov      r0, sl
004bdbd4: bl       #0x3140ec
004bdbd8: ldr      r3, [sp, #0x60]
004bdbdc: ldr      r1, [r5, #0x24]
004bdbe0: ldr      fp, [r5, #0x20]
004bdbe4: ldr      sb, [sp, #0x5c]
004bdbe8: mov      r0, r3
004bdbec: rsb      fp, r1, fp
004bdbf0: rsb      sb, r3, sb
004bdbf4: cmp      fp, sb
004bdbf8: movlt    r2, fp
004bdbfc: movge    r2, sb
004bdc00: bl       #0x30e5e0
004bdc04: cmp      r0, #0
004bdc08: mov      r3, r5
004bdc0c: bne      #0x4bdc48
004bdc10: cmp      sb, fp
004bdc14: blt      #0x4bdc4c
004bdc18: mov      r0, sl
004bdc1c: str      r3, [sp, #4]
004bdc20: bl       #0x318254
004bdc24: ldr      r3, [sp, #4]
004bdc28: ldr      r1, [r4, r7]
004bdc2c: ldr      r2, [sp, #0x64]
004bdc30: add      r0, r3, #0x28
004bdc34: ldr      r3, [r1]
004bdc38: cmp      r2, r3
004bdc3c: bne      #0x4bdcc0
004bdc40: add      sp, sp, #0x6c
004bdc44: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004bdc48: bge      #0x4bdc18
004bdc4c: mov      r0, sl
004bdc50: bl       #0x318254
004bdc54: add      sl, sp, #0x34
004bdc58: ldr      r1, [r6]
004bdc5c: add      r2, sp, #0x10
004bdc60: add      r6, sp, #0x18
004bdc64: mov      r0, sl
004bdc68: bl       #0x3140ec
004bdc6c: mov      r0, r6
004bdc70: ldr      r1, [sp, #0x48]
004bdc74: ldr      r2, [sp, #0x44]
004bdc78: str      r6, [sp, #0x28]
004bdc7c: str      r6, [sp, #0x2c]
004bdc80: bl       #0x3116e8
004bdc84: mov      r3, r6
004bdc88: mov      ip, #0
004bdc8c: mov      r1, r8
004bdc90: add      r2, sp, #8
004bdc94: add      r0, sp, #0xc
004bdc98: str      ip, [sp, #0x30]
004bdc9c: str      r5, [sp, #8]
004bdca0: bl       #0x4bd688
004bdca4: ldr      r5, [sp, #0xc]
004bdca8: mov      r0, r6
004bdcac: bl       #0x318254
004bdcb0: mov      r0, sl
004bdcb4: bl       #0x318254
004bdcb8: mov      r3, r5
004bdcbc: b        #0x4bdc28
004bdcc0: bl       #0x30e310
004bdcc4: strdeq   r6, r7, [sp], #-0xe4
004bdcc8: andeq    r4, r0, ip, lsr #1

# _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsPFiPKcEENS_10_Select1stIS9_EENS_11_MapTraitsTIS9_EESaIS9_EE7_M_findIS6_EEPNS_18_Rb_tree_node_baseERKT_
004bd4d0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004bd4d4: ldr      fp, [pc, #0x15c]
004bd4d8: ldr      r2, [pc, #0x15c]
004bd4dc: sub      sp, sp, #0x54
004bd4e0: add      fp, pc, fp
004bd4e4: ldr      r3, [fp, r2]
004bd4e8: str      r2, [sp, #0xc]
004bd4ec: str      r0, [sp, #8]
004bd4f0: ldr      r4, [r0, #4]
004bd4f4: ldr      r3, [r3]
004bd4f8: mov      r8, r1
004bd4fc: cmp      r4, #0
004bd500: str      r3, [sp, #0x4c]
004bd504: beq      #0x4bd60c
004bd508: mov      sl, r0
004bd50c: add      r7, sp, #0x34
004bd510: add      sb, sp, #0x18
004bd514: ldr      r1, [r8]
004bd518: mov      r2, sb
004bd51c: mov      r0, r7
004bd520: bl       #0x3140ec
004bd524: ldr      r3, [r4, #0x24]
004bd528: ldr      r1, [sp, #0x48]
004bd52c: ldr      r6, [r4, #0x20]
004bd530: ldr      r5, [sp, #0x44]
004bd534: mov      r0, r3
004bd538: rsb      r6, r3, r6
004bd53c: rsb      r5, r1, r5
004bd540: cmp      r5, r6
004bd544: movlt    r2, r5
004bd548: movge    r2, r6
004bd54c: bl       #0x30e5e0
004bd550: subs     r3, r0, #0
004bd554: bne      #0x4bd56c
004bd558: cmp      r6, r5
004bd55c: mvnlt    r3, #0
004bd560: blt      #0x4bd56c
004bd564: movle    r3, #0
004bd568: movgt    r3, #1
004bd56c: mov      r0, r7
004bd570: str      r3, [sp, #4]
004bd574: bl       #0x318254
004bd578: ldr      r3, [sp, #4]
004bd57c: cmp      r3, #0
004bd580: movge    sl, r4
004bd584: ldrlt    r4, [r4, #0xc]
004bd588: ldrge    r4, [r4, #8]
004bd58c: cmp      r4, #0
004bd590: bne      #0x4bd514
004bd594: ldr      r3, [sp, #8]
004bd598: cmp      sl, r3
004bd59c: beq      #0x4bd610
004bd5a0: add      r4, sp, #0x1c
004bd5a4: ldr      r1, [r8]
004bd5a8: add      r2, sp, #0x14
004bd5ac: mov      r0, r4
004bd5b0: bl       #0x3140ec
004bd5b4: ldr      r3, [sp, #0x30]
004bd5b8: ldr      r1, [sl, #0x24]
004bd5bc: ldr      r5, [sl, #0x20]
004bd5c0: ldr      r6, [sp, #0x2c]
004bd5c4: mov      r0, r3
004bd5c8: rsb      r5, r1, r5
004bd5cc: rsb      r6, r3, r6
004bd5d0: cmp      r5, r6
004bd5d4: movlt    r2, r5
004bd5d8: movge    r2, r6
004bd5dc: bl       #0x30e5e0
004bd5e0: subs     r7, r0, #0
004bd5e4: bne      #0x4bd5fc
004bd5e8: cmp      r6, r5
004bd5ec: mvnlt    r7, #0
004bd5f0: blt      #0x4bd5fc
004bd5f4: movle    r7, #0
004bd5f8: movgt    r7, #1
004bd5fc: mov      r0, r4
004bd600: bl       #0x318254
004bd604: cmp      r7, #0
004bd608: bge      #0x4bd610
004bd60c: ldr      sl, [sp, #8]
004bd610: ldr      r2, [sp, #0xc]
004bd614: mov      r0, sl
004bd618: ldr      r3, [fp, r2]
004bd61c: ldr      r2, [sp, #0x4c]
004bd620: ldr      r3, [r3]
004bd624: cmp      r2, r3
004bd628: bne      #0x4bd634
004bd62c: add      sp, sp, #0x54
004bd630: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004bd634: bl       #0x30e310
004bd638: strheq   r7, [sp], #-0x50
004bd63c: andeq    r4, r0, ip, lsr #1

# _ZN6Arrays19GetMemberIDByStringINS_10ClassTableEEEiPKc
004af16c: ldr      r3, [pc, #0x60]
004af170: ldr      r2, [pc, #0x60]
004af174: push     {r4, r5, r6, r7, r8, lr}
004af178: add      r3, pc, r3
004af17c: ldr      r2, [r3, r2]
004af180: mov      r6, r0
004af184: ldr      r5, [r2]
004af188: cmp      r5, #0
004af18c: beq      #0x4af1cc
004af190: ldr      r2, [pc, #0x44]
004af194: mov      r4, #0
004af198: ldr      r3, [r3, r2]
004af19c: ldr      r7, [r3]
004af1a0: b        #0x4af1b0
004af1a4: add      r4, r4, #1
004af1a8: cmp      r4, r5
004af1ac: beq      #0x4af1cc
004af1b0: ldr      r1, [r7, r4, lsl #2]
004af1b4: mov      r0, r6
004af1b8: bl       #0x30e31c
004af1bc: cmp      r0, #0
004af1c0: bne      #0x4af1a4
004af1c4: mov      r0, r4
004af1c8: pop      {r4, r5, r6, r7, r8, pc}
004af1cc: mvn      r0, #0
004af1d0: pop      {r4, r5, r6, r7, r8, pc}
004af1d4: subeq    r5, lr, r8, lsl sb
004af1d8: andeq    r3, r0, r8, ror #10
004af1dc: muleq    r0, r0, sl

# _ZN12PyDataArrays6GetOIDEPKcS1_
004bd640: push     {r4, r5, lr}
004bd644: sub      sp, sp, #0xc
004bd648: add      r3, sp, #8
004bd64c: str      r1, [r3, #-4]!
004bd650: add      r4, r0, #0x1c
004bd654: mov      r1, r3
004bd658: mov      r0, r4
004bd65c: mov      r5, r2
004bd660: bl       #0x4bd4d0
004bd664: cmp      r0, r4
004bd668: mov      r3, r0
004bd66c: mvneq    r0, #0
004bd670: beq      #0x4bd680
004bd674: mov      r0, r5
004bd678: mov      lr, pc
004bd67c: ldr      pc, [r3, #0x28]
004bd680: add      sp, sp, #0xc
004bd684: pop      {r4, r5, pc}

# _ZN7Structs19GetMemberIDByStringINS_19CharacterPropertiesEEEiPKc
004af110: ldr      r3, [pc, #0x4c]
004af114: ldr      r2, [pc, #0x4c]
004af118: push     {r4, r5, r6, lr}
004af11c: add      r3, pc, r3
004af120: mov      r6, r0
004af124: ldr      r4, [r3, r2]
004af128: mov      r5, #0
004af12c: b        #0x4af140
004af130: add      r5, r5, #1
004af134: cmp      r5, #0xe0
004af138: add      r4, r4, #0x18
004af13c: beq      #0x4af15c
004af140: ldr      r1, [r4, #0x14]
004af144: mov      r0, r6
004af148: bl       #0x30e31c
004af14c: cmp      r0, #0
004af150: bne      #0x4af130
004af154: mov      r0, r5
004af158: pop      {r4, r5, r6, pc}
004af15c: mvn      r0, #0
004af160: pop      {r4, r5, r6, pc}
004af164: subeq    r5, lr, r4, ror sb
004af168: andeq    r1, r0, ip, asr #19

# _ZN6Arrays19GetMemberIDByStringINS_14CharacterTableEEEiPKc
003fa188: ldr      r3, [pc, #0x60]
003fa18c: ldr      r2, [pc, #0x60]
003fa190: push     {r4, r5, r6, r7, r8, lr}
003fa194: add      r3, pc, r3
003fa198: ldr      r2, [r3, r2]
003fa19c: mov      r6, r0
003fa1a0: ldr      r5, [r2]
003fa1a4: cmp      r5, #0
003fa1a8: beq      #0x3fa1e8
003fa1ac: ldr      r2, [pc, #0x44]
003fa1b0: mov      r4, #0
003fa1b4: ldr      r3, [r3, r2]
003fa1b8: ldr      r7, [r3]
003fa1bc: b        #0x3fa1cc
003fa1c0: add      r4, r4, #1
003fa1c4: cmp      r4, r5
003fa1c8: beq      #0x3fa1e8
003fa1cc: ldr      r1, [r7, r4, lsl #2]
003fa1d0: mov      r0, r6
003fa1d4: bl       #0x30e31c
003fa1d8: cmp      r0, #0
003fa1dc: bne      #0x3fa1c0
003fa1e0: mov      r0, r4
003fa1e4: pop      {r4, r5, r6, r7, r8, pc}
003fa1e8: mvn      r0, #0
003fa1ec: pop      {r4, r5, r6, r7, r8, pc}
003fa1f0: ldrsheq  sl, [sb], #-0x8c
003fa1f4: andeq    r4, r0, r4, lsl #4
003fa1f8: andeq    r3, r0, r8, lsl #24

# _ZN6Arrays19GetMemberIDByStringINS_7AITableEEEiPKc
004af51c: ldr      r3, [pc, #0x60]
004af520: ldr      r2, [pc, #0x60]
004af524: push     {r4, r5, r6, r7, r8, lr}
004af528: add      r3, pc, r3
004af52c: ldr      r2, [r3, r2]
004af530: mov      r6, r0
004af534: ldr      r5, [r2]
004af538: cmp      r5, #0
004af53c: beq      #0x4af57c
004af540: ldr      r2, [pc, #0x44]
004af544: mov      r4, #0
004af548: ldr      r3, [r3, r2]
004af54c: ldr      r7, [r3]
004af550: b        #0x4af560
004af554: add      r4, r4, #1
004af558: cmp      r4, r5
004af55c: beq      #0x4af57c
004af560: ldr      r1, [r7, r4, lsl #2]
004af564: mov      r0, r6
004af568: bl       #0x30e31c
004af56c: cmp      r0, #0
004af570: bne      #0x4af554
004af574: mov      r0, r4
004af578: pop      {r4, r5, r6, r7, r8, pc}
004af57c: mvn      r0, #0
004af580: pop      {r4, r5, r6, r7, r8, pc}
004af584: subeq    r5, lr, r8, ror #10
004af588: andeq    r1, r0, ip, lsr #2
004af58c: strdeq   r4, r5, [r0], -ip
