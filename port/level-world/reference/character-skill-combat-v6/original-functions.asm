
# _ZN17PlayerStatManager13DecrementStatE9EStatTypei
003790f8: mov      r3, r2
003790fc: mvn      r2, #0
00379100: b        #0x3790e0

# _ZNK17PlayerStatManager10GetRankingE9EStatTypeb
00379ac4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00379ac8: ldr      r4, [pc, #0x228]
00379acc: ldr      r5, [pc, #0x228]
00379ad0: sub      sp, sp, #0x18
00379ad4: add      r4, pc, r4
00379ad8: ldr      r3, [r4, #0x2c]
00379adc: add      r5, pc, r5
00379ae0: mov      r7, r0
00379ae4: ands     r6, r3, #1
00379ae8: mov      r8, r1
00379aec: mov      sl, r2
00379af0: beq      #0x379c14
00379af4: cmp      sl, #0
00379af8: bne      #0x379b10
00379afc: ldr      r0, [pc, #0x1fc]
00379b00: add      r0, pc, r0
00379b04: add      r0, r0, #0xc
00379b08: add      sp, sp, #0x18
00379b0c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00379b10: ldr      r3, [pc, #0x1ec]
00379b14: ldr      r3, [r5, r3]
00379b18: ldr      r0, [r3, #0x40]
00379b1c: bl       #0x36d7a8 ; _ZN13PlayerManager13GetNumPlayersEv
00379b20: subs     r4, r0, #0
00379b24: ble      #0x379b68
00379b28: ldr      sl, [pc, #0x1d8]
00379b2c: mov      r6, #0
00379b30: mov      r5, r6
00379b34: add      sl, pc, sl
00379b38: add      sl, sl, #0xc
00379b3c: mov      r2, r5
00379b40: mov      r0, r7
00379b44: mov      r1, r8
00379b48: bl       #0x379824 ; _ZNK17PlayerStatManager12GetStatValueE9EStatTypei
00379b4c: add      r3, sl, r6
00379b50: str      r5, [r3, #4]
00379b54: add      r5, r5, #1
00379b58: cmp      r5, r4
00379b5c: str      r0, [sl, r6]
00379b60: add      r6, r6, #8
00379b64: bne      #0x379b3c
00379b68: ldr      r6, [pc, #0x19c]
00379b6c: add      r6, pc, r6
00379b70: add      r6, r6, #0xc
00379b74: add      r4, r6, r4, lsl #3
00379b78: cmp      r4, r6
00379b7c: beq      #0x379afc
00379b80: rsb      r6, r6, r4
00379b84: asr      r2, r6, #3
00379b88: cmp      r2, #1
00379b8c: moveq    r3, #0
00379b90: beq      #0x379bac
00379b94: mov      r3, #0
00379b98: asr      r2, r2, #1
00379b9c: cmp      r2, #1
00379ba0: add      r3, r3, #1
00379ba4: bne      #0x379b98
00379ba8: lsl      r3, r3, #1
00379bac: ldr      r5, [pc, #0x15c]
00379bb0: mov      ip, #0
00379bb4: mov      r1, r4
00379bb8: add      r5, pc, r5
00379bbc: add      r0, r5, #0xc
00379bc0: mov      r2, #0
00379bc4: strb     ip, [sp]
00379bc8: bl       #0x37942c ; _ZNSt4priv16__introsort_loopIPSt4pairIiiES2_iN17PlayerStatManager9_StatCompEEEvT_S6_PT0_T1_T2_
00379bcc: cmp      r6, #0x87
00379bd0: bgt      #0x379c54
00379bd4: add      r5, r5, #0x14
00379bd8: cmp      r4, r5
00379bdc: beq      #0x379afc
00379be0: add      r6, sp, #8
00379be4: ldr      r3, [r5]
00379be8: mov      r0, r5
00379bec: mov      r1, r6
00379bf0: str      r3, [sp, #8]
00379bf4: ldr      r3, [r5, #4]
00379bf8: mov      r2, #0
00379bfc: add      r5, r5, #8
00379c00: str      r3, [sp, #0xc]
00379c04: bl       #0x379a2c ; 
00379c08: cmp      r4, r5
00379c0c: bne      #0x379be4
00379c10: b        #0x379afc
00379c14: add      sb, r4, #0x2c
00379c18: mov      r0, sb
00379c1c: bl       #0x30e76c ; 
00379c20: cmp      r0, #0
00379c24: beq      #0x379af4
00379c28: str      r6, [r4, #0x28]
00379c2c: str      r6, [r4, #0xc]
00379c30: str      r6, [r4, #0x10]
00379c34: str      r6, [r4, #0x14]
00379c38: str      r6, [r4, #0x18]
00379c3c: str      r6, [r4, #0x1c]
00379c40: str      r6, [r4, #0x20]
00379c44: str      r6, [r4, #0x24]
00379c48: mov      r0, sb
00379c4c: bl       #0x30ea3c ; 
00379c50: b        #0x379af4
00379c54: add      r6, r5, #0x14
00379c58: add      r7, sp, #0x10
00379c5c: add      r5, r5, #0x8c
00379c60: ldr      r3, [r6]
00379c64: mov      r0, r6
00379c68: mov      r1, r7
00379c6c: str      r3, [sp, #0x10]
00379c70: ldr      r3, [r6, #4]
00379c74: mov      r2, #0
00379c78: add      r6, r6, #8
00379c7c: str      r3, [sp, #0x14]
00379c80: bl       #0x379a2c ; 
00379c84: cmp      r6, r5
00379c88: bne      #0x379c60
00379c8c: ldr      r5, [pc, #0x80]
00379c90: add      r5, pc, r5
00379c94: add      r6, r5, #0x8c
00379c98: cmp      r4, r6
00379c9c: beq      #0x379afc
00379ca0: add      r5, r5, #0x84
00379ca4: ldr      ip, [r5, #8]
00379ca8: ldr      r7, [r5, #0xc]
00379cac: mov      r3, r5
00379cb0: mov      r2, r6
00379cb4: b        #0x379ccc
00379cb8: ldr      r1, [r3, #8]
00379cbc: str      r1, [r2]
00379cc0: ldr      r1, [r3, #0xc]
00379cc4: str      r1, [r2, #4]
00379cc8: mov      r2, r0
00379ccc: mov      r0, r3
00379cd0: ldr      r1, [r3], #-8
00379cd4: cmp      ip, r1
00379cd8: bgt      #0x379cb8
00379cdc: add      r6, r6, #8
00379ce0: cmp      r4, r6
00379ce4: str      ip, [r2]
00379ce8: str      r7, [r2, #4]
00379cec: add      r5, r5, #8
00379cf0: bne      #0x379ca4
00379cf4: b        #0x379afc

# _ZN17PlayerStatManager18ResetAllStatValuesEi
0037912c: push     {r4, r5, r6, lr}
00379130: mov      r6, r0
00379134: mov      r5, r1
00379138: mov      r4, #0
0037913c: mov      r1, r4
00379140: mov      r0, r6
00379144: add      r4, r4, #1
00379148: mov      r2, r5
0037914c: bl       #0x379108 ; _ZN17PlayerStatManager14ResetStatValueE9EStatTypei
00379150: cmp      r4, #7
00379154: bne      #0x37913c
00379158: pop      {r4, r5, r6, pc}

# _ZNK6CharAI11AI_GetAggroEP9Character
003d4ac8: push     {r4, r5, lr}
003d4acc: ldr      r3, [pc, #0xd8]
003d4ad0: subs     r4, r1, #0
003d4ad4: sub      sp, sp, #0xc
003d4ad8: mov      r5, r0
003d4adc: add      r3, pc, r3
003d4ae0: beq      #0x3d4b50
003d4ae4: ldr      r3, [r5, #0x80]
003d4ae8: add      r5, r5, #0x7c
003d4aec: cmp      r3, #0
003d4af0: beq      #0x3d4b48
003d4af4: mov      r1, r5
003d4af8: b        #0x3d4b00
003d4afc: mov      r3, r2
003d4b00: ldr      r2, [r3, #0x10]
003d4b04: cmp      r4, r2
003d4b08: ldrhi    r2, [r3, #0xc]
003d4b0c: ldrls    r2, [r3, #8]
003d4b10: movhi    r3, r1
003d4b14: mov      r1, r3
003d4b18: cmp      r2, #0
003d4b1c: bne      #0x3d4afc
003d4b20: cmp      r5, r3
003d4b24: beq      #0x3d4ba4
003d4b28: ldr      r2, [r3, #0x10]
003d4b2c: cmp      r4, r2
003d4b30: blo      #0x3d4b48
003d4b34: cmp      r5, r3
003d4b38: ldrne    r0, [r3, #0x14]
003d4b3c: beq      #0x3d4ba4
003d4b40: add      sp, sp, #0xc
003d4b44: pop      {r4, r5, pc}
003d4b48: mov      r3, r5
003d4b4c: b        #0x3d4b34
003d4b50: ldr      r2, [pc, #0x58]
003d4b54: ldr      r2, [r3, r2]
003d4b58: ldr      r2, [r2]
003d4b5c: cmp      r2, #2
003d4b60: streq    r4, [r4]
003d4b64: beq      #0x3d4ae4
003d4b68: cmp      r2, #1
003d4b6c: bne      #0x3d4ae4
003d4b70: ldr      r0, [pc, #0x3c]
003d4b74: ldr      r1, [pc, #0x3c]
003d4b78: ldr      r2, [pc, #0x3c]
003d4b7c: ldr      r0, [r3, r0]
003d4b80: ldr      r3, [pc, #0x38]
003d4b84: movw     ip, #0x229
003d4b88: add      r1, pc, r1
003d4b8c: add      r2, pc, r2
003d4b90: add      r3, pc, r3
003d4b94: add      r0, r0, #0xa8
003d4b98: str      ip, [sp]
003d4b9c: bl       #0x30e004 ; 
003d4ba0: b        #0x3d4ae4
003d4ba4: mov      r0, #0
003d4ba8: b        #0x3d4b40
003d4bac: ldrheq   pc, [fp], #-0xf4
003d4bb0: andeq    r3, r0, r0, asr #19
003d4bb4: andeq    r1, r0, r0, asr #19
003d4bb8: subeq    sb, lr, r0, asr r8
003d4bbc: ldrsheq  sp, [r1], #-0x4c
003d4bc0: subeq    r0, pc, r0, lsr sl

# _ZN14PlayerSavegame11_InitSkillsEv
00469764: push     {r4, r5, r6, r7, r8, lr}
00469768: ldr      r5, [r0, #0x80]
0046976c: mov      r4, r0
00469770: cmp      r5, #0
00469774: beq      #0x46977c
00469778: pop      {r4, r5, r6, r7, r8, pc}
0046977c: ldr      r0, [r0, #0x10]
00469780: bl       #0x3bc5fc ; _ZNK9Character16GetCharSkillListEv
00469784: mov      r6, r0
00469788: ldr      r0, [r0, #4]
0046978c: mov      r1, r5
00469790: str      r0, [r4, #0x84]
00469794: lsl      r0, r0, #3
00469798: bl       #0x31056c ; _Znaj15MemoryHintState
0046979c: ldr      r3, [r4, #0x84]
004697a0: str      r0, [r4, #0x80]
004697a4: cmp      r3, #0
004697a8: beq      #0x4697e4
004697ac: mov      r1, r5
004697b0: b        #0x4697b8
004697b4: ldr      r0, [r4, #0x80]
004697b8: ldr      r2, [r6, #8]
004697bc: add      r3, r0, r5, lsl #3
004697c0: ldr      r2, [r2, r5, lsl #2]
004697c4: str      r2, [r0, r5, lsl #3]
004697c8: mov      r2, #0
004697cc: strb     r1, [r3, #6]
004697d0: strh     r2, [r3, #4]
004697d4: ldr      r3, [r4, #0x84]
004697d8: add      r5, r5, #1
004697dc: cmp      r3, r5
004697e0: bhi      #0x4697b4
004697e4: ldr      r1, [r4, #0x88]
004697e8: ldr      r0, [r4, #0x8c]
004697ec: rsb      r3, r1, r0
004697f0: asr      r3, r3, #3
004697f4: add      r2, r3, r3, lsl #2
004697f8: add      r2, r2, r2, lsl #4
004697fc: add      r2, r2, r2, lsl #8
00469800: add      r2, r2, r2, lsl #16
00469804: add      r3, r3, r2, lsl #1
00469808: cmp      r3, #0
0046980c: beq      #0x469778
00469810: mov      r6, #0
00469814: mov      r7, r6
00469818: mov      r8, r6
0046981c: b        #0x469844
00469820: rsb      r3, r1, r0
00469824: asr      r3, r3, #3
00469828: add      r2, r3, r3, lsl #2
0046982c: add      r2, r2, r2, lsl #4
00469830: add      r2, r2, r2, lsl #8
00469834: add      r2, r2, r2, lsl #16
00469838: add      r3, r3, r2, lsl #1
0046983c: cmp      r7, r3
00469840: bhs      #0x469778
00469844: add      r5, r1, r6
00469848: ldr      r3, [r5, #0x10]
0046984c: add      r7, r7, #1
00469850: add      r6, r6, #0x18
00469854: cmp      r3, #0
00469858: beq      #0x469820
0046985c: mov      r0, r5
00469860: ldr      r1, [r5, #4]
00469864: bl       #0x345c94 ; _ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiiENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
00469868: str      r8, [r5, #0x10]
0046986c: str      r5, [r5, #8]
00469870: str      r8, [r5, #4]
00469874: str      r5, [r5, #0xc]
00469878: ldr      r1, [r4, #0x88]
0046987c: ldr      r0, [r4, #0x8c]
00469880: b        #0x469820

# _ZSt10__pop_heapIPSt4pairIiiES1_N17PlayerStatManager9_StatCompEiEvT_S5_S5_T0_T1_PT2_
00379304: push     {r4, lr}
00379308: ldr      r4, [r0]
0037930c: mov      lr, r2
00379310: sub      sp, sp, #0x10
00379314: str      r4, [r2]
00379318: ldr      r4, [r0, #4]
0037931c: rsb      r2, r0, r1
00379320: asr      r2, r2, #3
00379324: str      r4, [lr, #4]
00379328: ldr      ip, [r3, #4]
0037932c: ldr      lr, [r3]
00379330: mov      r1, #0
00379334: str      ip, [sp, #0xc]
00379338: add      r3, sp, #8
0037933c: mov      ip, #0
00379340: str      lr, [sp, #8]
00379344: strb     ip, [sp]
00379348: bl       #0x3791d4 ; _ZSt13__adjust_heapIPSt4pairIiiEiS1_N17PlayerStatManager9_StatCompEEvT_T0_S6_T1_T2_
0037934c: add      sp, sp, #0x10
00379350: pop      {r4, pc}

# _ZN14PlayerSavegame15SG_ReloadSkillsEv
00467324: push     {r4, lr}
00467328: mov      r4, r0
0046732c: ldr      r0, [r0, #0x80]
00467330: cmp      r0, #0
00467334: beq      #0x467344
00467338: bl       #0x310440 ; _Z10CustomFreePv
0046733c: mov      r3, #0
00467340: str      r3, [r4, #0x80]
00467344: mov      r0, r4
00467348: bl       #0x469764 ; _ZN14PlayerSavegame11_InitSkillsEv
0046734c: mov      r0, r4
00467350: mov      r1, #8
00467354: pop      {r4, lr}
00467358: b        #0x465430

# _ZNK6CharAI16AI_GetAggroEntryEiRP9CharacterRf
003d72d0: push     {r4, r5, r6, r7, lr}
003d72d4: mov      r5, r1
003d72d8: sub      sp, sp, #0x1c
003d72dc: mov      r1, r0
003d72e0: mov      r6, r2
003d72e4: mov      r0, sp
003d72e8: add      r2, r1, #0x7c
003d72ec: mov      r7, r3
003d72f0: bl       #0x3d7208 ; _ZNK6CharAI26AI_GetRelationsConverseMapERKSt3mapIP9CharacterfSt4lessIS2_ESaISt4pairIKS2_fEEE
003d72f4: ldr      r2, [sp, #8]
003d72f8: mov      r4, sp
003d72fc: mov      r1, #0
003d7300: cmp      r2, r4
003d7304: beq      #0x3d7370
003d7308: cmp      r1, r5
003d730c: beq      #0x3d7398
003d7310: ldr      r0, [r2, #0xc]
003d7314: add      r1, r1, #1
003d7318: cmp      r0, #0
003d731c: beq      #0x3d7338
003d7320: mov      r2, r0
003d7324: ldr      r3, [r2, #8]
003d7328: cmp      r3, #0
003d732c: beq      #0x3d7300
003d7330: mov      r2, r3
003d7334: b        #0x3d7324
003d7338: ldr      r3, [r2, #4]
003d733c: ldr      ip, [r3, #0xc]
003d7340: cmp      r2, ip
003d7344: bne      #0x3d7360
003d7348: mov      r2, r3
003d734c: ldr      r3, [r3, #4]
003d7350: ldr      r0, [r3, #0xc]
003d7354: cmp      r0, r2
003d7358: beq      #0x3d7348
003d735c: ldr      r0, [r2, #0xc]
003d7360: cmp      r0, r3
003d7364: movne    r2, r3
003d7368: cmp      r2, r4
003d736c: bne      #0x3d7308
003d7370: mov      r3, #0
003d7374: str      r3, [r6]
003d7378: ldr      r3, [sp, #0x10]
003d737c: cmp      r3, #0
003d7380: beq      #0x3d7390
003d7384: mov      r0, sp
003d7388: ldr      r1, [sp, #4]
003d738c: bl       #0x3d5d64 ; _ZNSt4priv8_Rb_treeIfSt4lessIfESt4pairIKfP9CharacterENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE8_M_eraseEPNS_18_Rb_tree_node_baseE
003d7390: add      sp, sp, #0x1c
003d7394: pop      {r4, r5, r6, r7, pc}
003d7398: ldr      r3, [r2, #0x14]
003d739c: str      r3, [r6]
003d73a0: ldr      r3, [r2, #0x10]
003d73a4: str      r3, [r7]
003d73a8: b        #0x3d7378

# _ZN17PlayerStatManagerC1Ev
003790a8: ldr      r3, [pc, #0x14]
003790ac: ldr      r2, [pc, #0x14]
003790b0: add      r3, pc, r3
003790b4: ldr      r2, [r3, r2]
003790b8: add      r2, r2, #8
003790bc: str      r2, [r0]
003790c0: bx       lr
003790c4: rsbeq    fp, r1, r0, ror #19
003790c8: andeq    r3, r0, r8, lsl #12

# _ZNSt4priv16__introsort_loopIPSt4pairIiiES2_iN17PlayerStatManager9_StatCompEEEvT_S6_PT0_T1_T2_
0037942c: push     {r4, r5, r6, r7, r8, lr}
00379430: rsb      r2, r0, r1
00379434: cmp      r2, #0x87
00379438: sub      sp, sp, #8
0037943c: mov      r5, r0
00379440: mov      r6, r3
00379444: ble      #0x379560
00379448: cmp      r3, #0
0037944c: beq      #0x379520
00379450: asr      r2, r2, #4
00379454: ldr      r3, [r5, r2, lsl #3]
00379458: ldr      r2, [r5]
0037945c: sub      r6, r6, #1
00379460: cmp      r2, r3
00379464: ble      #0x379540
00379468: ldr      r0, [r1, #-8]
0037946c: cmp      r3, r0
00379470: bgt      #0x379554
00379474: cmp      r2, r0
00379478: bgt      #0x3794e8
0037947c: mov      ip, r0
00379480: mov      r0, r2
00379484: mov      lr, r1
00379488: mov      r7, r5
0037948c: mov      r4, r7
00379490: b        #0x379498
00379494: ldr      r2, [r4, #8]!
00379498: cmp      r2, r0
0037949c: bgt      #0x379494
003794a0: sub      r3, lr, #8
003794a4: b        #0x3794ac
003794a8: ldr      ip, [r3, #-8]!
003794ac: cmp      r0, ip
003794b0: bgt      #0x3794a8
003794b4: cmp      r4, r3
003794b8: mov      lr, r3
003794bc: bhs      #0x3794f0
003794c0: ldr      r8, [r3]
003794c4: ldr      ip, [r4, #4]
003794c8: add      r7, r4, #8
003794cc: str      r8, [r4]
003794d0: ldr      r8, [r3, #4]
003794d4: str      r8, [r4, #4]
003794d8: stm      r3, {r2, ip}
003794dc: ldr      ip, [r3, #-8]
003794e0: ldr      r2, [r4, #8]
003794e4: b        #0x37948c
003794e8: mov      ip, r0
003794ec: b        #0x379484
003794f0: mov      r2, #0
003794f4: mov      ip, #0
003794f8: mov      r0, r4
003794fc: mov      r3, r6
00379500: strb     ip, [sp]
00379504: bl       #0x37942c ; _ZNSt4priv16__introsort_loopIPSt4pairIiiES2_iN17PlayerStatManager9_StatCompEEEvT_S6_PT0_T1_T2_
00379508: rsb      r2, r5, r4
0037950c: cmp      r2, #0x87
00379510: ble      #0x379560
00379514: cmp      r6, #0
00379518: mov      r1, r4
0037951c: bne      #0x379450
00379520: mov      ip, #0
00379524: mov      r0, r5
00379528: mov      r2, r1
0037952c: mov      r3, #0
00379530: strb     ip, [sp, #0x20]
00379534: add      sp, sp, #8
00379538: pop      {r4, r5, r6, r7, r8, lr}
0037953c: b        #0x379354
00379540: ldr      r0, [r1, #-8]
00379544: cmp      r2, r0
00379548: bgt      #0x37947c
0037954c: cmp      r3, r0
00379550: bgt      #0x3794e8
00379554: mov      ip, r0
00379558: mov      r0, r3
0037955c: b        #0x379484
00379560: add      sp, sp, #8
00379564: pop      {r4, r5, r6, r7, r8, pc}

# _ZN17PlayerStatManager17ApplyPlayersBonusEv
003790dc: bx       lr

# _ZN9Character8_RegenMPERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b7774: str      lr, [sp, #-4]!
003b7778: ldr      r3, [r0, #4]
003b777c: sub      sp, sp, #0xc
003b7780: ldr      r1, [r3, #4]
003b7784: ldr      ip, [r3]
003b7788: rsb      r3, ip, r1
003b778c: asr      r3, r3, #4
003b7790: add      r1, r3, r3, lsl #3
003b7794: add      r1, r1, r1, lsl #6
003b7798: add      r1, r3, r1, lsl #3
003b779c: add      r1, r1, r1, lsl #15
003b77a0: add      r3, r3, r1, lsl #3
003b77a4: cmp      r3, #0
003b77a8: bne      #0x3b77b4
003b77ac: add      sp, sp, #0xc
003b77b0: ldm      sp!, {pc}
003b77b4: ldr      r3, [ip, #4]
003b77b8: cmp      r3, #3
003b77bc: bne      #0x3b77ac
003b77c0: mov      r1, #0
003b77c4: str      r2, [sp, #4]
003b77c8: bl       #0x37baf8 ; _ZNK3sfc6script3lua9ArgumentsixEj
003b77cc: bl       #0x31bbf0 ; _ZNK3sfc6script3lua5Value9getNumberEv
003b77d0: bl       #0x30e4cc ; 
003b77d4: ldr      r2, [sp, #4]
003b77d8: mov      r1, r0
003b77dc: mov      r0, r2
003b77e0: add      sp, sp, #0xc
003b77e4: pop      {lr}
003b77e8: b        #0x3bdbb8

# _ZN17CharAISkillScriptD0Ev
003cc448: ldr      r3, [pc, #0x2c]
003cc44c: ldr      r2, [pc, #0x2c]
003cc450: push     {r4, lr}
003cc454: add      r3, pc, r3
003cc458: ldr      r2, [r3, r2]
003cc45c: mov      r4, r0
003cc460: add      r2, r2, #8
003cc464: str      r2, [r0], #0xc
003cc468: bl       #0x319228 ; _ZN3sfc6script3lua9ArgumentsD1Ev
003cc46c: mov      r0, r4
003cc470: bl       #0x310440 ; _Z10CustomFreePv
003cc474: mov      r0, r4
003cc478: pop      {r4, pc}
003cc47c: subseq   r8, ip, ip, lsr r6
003cc480: andeq    r0, r0, ip, lsr #23

# _ZN9Character18_F_CalculateResultERNS_12AttackResultEPS_S2_iiii
003b2638: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b263c: ldr      r6, [pc, #0x7c0]
003b2640: ldr      r7, [pc, #0x7c0]
003b2644: mov      r5, r0
003b2648: add      r6, pc, r6
003b264c: ldr      ip, [r6, r7]
003b2650: ldr      r0, [pc, #0x7b4]
003b2654: sub      sp, sp, #0x60
003b2658: ldr      ip, [ip]
003b265c: add      r0, pc, r0
003b2660: mov      r4, r3
003b2664: mov      sl, r1
003b2668: mov      sb, r2
003b266c: ldr      r8, [sp, #0x84]
003b2670: str      ip, [sp, #0x5c]
003b2674: bl       #0x3136b4 ; _Z20PushProfilingContextPKc
003b2678: mov      r0, r5
003b267c: bl       #0x3af3b0 ; _ZN9Character14_F_ResetResultERNS_12AttackResultE
003b2680: ldr      r3, [sp, #0x80]
003b2684: ubfx     ip, r4, #0x1b, #1
003b2688: str      r4, [r5, #0x1c]
003b268c: str      r3, [r5, #0x20]
003b2690: str      r8, [r5, #0x24]
003b2694: mov      r0, sl
003b2698: mov      r1, sb
003b269c: mov      r2, r8
003b26a0: ubfx     r3, r4, #0x1a, #1
003b26a4: str      ip, [sp]
003b26a8: bl       #0x3b059c ; _Z16CF_SetCombatantsP9CharacterS0_ibb
003b26ac: tst      r4, #5
003b26b0: bne      #0x3b27e8
003b26b4: tst      r4, #0xa
003b26b8: bne      #0x3b2b44
003b26bc: ldrb     r8, [r5, #0x18]
003b26c0: ands     r8, r8, #3
003b26c4: bne      #0x3b286c
003b26c8: tst      r4, #0x10
003b26cc: bne      #0x3b2ac0
003b26d0: tst      r4, #0x20
003b26d4: bne      #0x3b2a70
003b26d8: tst      r4, #0x40
003b26dc: mvneq    r8, #0
003b26e0: bne      #0x3b2b08
003b26e4: tst      r4, #0x80
003b26e8: beq      #0x3b287c
003b26ec: cmn      r8, #1
003b26f0: beq      #0x3b2de0
003b26f4: ldr      r3, [pc, #0x714]
003b26f8: add      r3, pc, r3
003b26fc: ldr      r0, [r3, #0x1c]
003b2700: cmp      r0, #0
003b2704: beq      #0x3b28c8
003b2708: ldr      r1, [r3, #0x20]
003b270c: cmp      r1, #0
003b2710: beq      #0x3b28c8
003b2714: mov      r2, r8
003b2718: mov      r3, #0
003b271c: bl       #0x3b0e8c ; _Z12CF__CalcHurtP9CharacterS0_ii
003b2720: ldrb     r3, [r5, #0x18]
003b2724: bfi      r3, r0, #4, #1
003b2728: strb     r3, [r5, #0x18]
003b272c: tst      r4, #0x200
003b2730: bne      #0x3b2a28
003b2734: tst      r4, #0x400
003b2738: bne      #0x3b2d24
003b273c: tst      r4, #0x800
003b2740: bne      #0x3b2948
003b2744: tst      r4, #0x1000
003b2748: bne      #0x3b2ce8
003b274c: tst      r4, #0x2000
003b2750: bne      #0x3b2994
003b2754: tst      r4, #0x4000
003b2758: bne      #0x3b2cac
003b275c: tst      r4, #0x8000
003b2760: bne      #0x3b29e0
003b2764: tst      r4, #0x10000
003b2768: bne      #0x3b2c70
003b276c: ldr      r3, [pc, #0x6a0]
003b2770: add      r8, sp, #0x44
003b2774: ldr      sl, [r6, r3]
003b2778: mov      r0, sl
003b277c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b2780: ldr      r1, [pc, #0x690]
003b2784: add      r2, sp, #0x40
003b2788: mov      r0, r8
003b278c: add      r1, pc, r1
003b2790: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b2794: mov      r1, r8
003b2798: mov      r0, sl
003b279c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b27a0: mov      r0, r8
003b27a4: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b27a8: ands     r3, r4, #0x20000
003b27ac: bne      #0x3b28d0
003b27b0: tst      r4, #0x40000
003b27b4: bne      #0x3b2c00
003b27b8: tst      r4, #0x80000
003b27bc: bne      #0x3b2d60
003b27c0: ldr      r0, [pc, #0x654]
003b27c4: add      r0, pc, r0
003b27c8: bl       #0x3136b8 ; _Z19PopProfilingContextPKc
003b27cc: ldr      r3, [r6, r7]
003b27d0: ldr      r2, [sp, #0x5c]
003b27d4: ldr      r3, [r3]
003b27d8: cmp      r2, r3
003b27dc: bne      #0x3b2e00
003b27e0: add      sp, sp, #0x60
003b27e4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b27e8: mov      r0, #0x64
003b27ec: bl       #0x3af6d8 ; 
003b27f0: ldr      r3, [pc, #0x628]
003b27f4: mov      r2, r0
003b27f8: add      r3, pc, r3
003b27fc: ldr      r0, [r3, #0x1c]
003b2800: cmp      r0, #0
003b2804: beq      #0x3b28bc
003b2808: ldr      r1, [r3, #0x20]
003b280c: cmp      r1, #0
003b2810: beq      #0x3b28bc
003b2814: ldr      ip, [r3, #0x2c]
003b2818: lsl      r2, r2, #8
003b281c: str      ip, [sp]
003b2820: ldrb     ip, [r3, #0x30]
003b2824: mov      r3, #0
003b2828: str      ip, [sp, #4]
003b282c: bl       #0x3b1df0 ; _Z19CF__CalcMissOrDodgeP9CharacterS0_iiib
003b2830: sxth     r0, r0
003b2834: ubfx     r3, r0, #8, #8
003b2838: strb     r3, [sp, #0x11]
003b283c: strb     r0, [sp, #0x10]
003b2840: ldrh     r3, [sp, #0x10]
003b2844: strh     r3, [sp, #0x3c]
003b2848: lsr      r2, r3, #8
003b284c: uxtb     r1, r3
003b2850: ldrb     r3, [r5, #0x18]
003b2854: bfi      r3, r1, #0, #1
003b2858: bfi      r3, r2, #1, #1
003b285c: strb     r3, [r5, #0x18]
003b2860: ldrb     r8, [r5, #0x18]
003b2864: ands     r8, r8, #3
003b2868: beq      #0x3b26c8
003b286c: ldr      r0, [pc, #0x5b0]
003b2870: add      r0, pc, r0
003b2874: bl       #0x3136b8 ; _Z19PopProfilingContextPKc
003b2878: b        #0x3b27cc
003b287c: tst      r4, #0x100
003b2880: beq      #0x3b272c
003b2884: cmn      r8, #1
003b2888: beq      #0x3b2df0
003b288c: ldr      r3, [pc, #0x594]
003b2890: add      r3, pc, r3
003b2894: ldr      r0, [r3, #0x1c]
003b2898: cmp      r0, #0
003b289c: beq      #0x3b28c8
003b28a0: ldr      r1, [r3, #0x20]
003b28a4: cmp      r1, #0
003b28a8: beq      #0x3b28c8
003b28ac: mov      r2, r8
003b28b0: mov      r3, #2
003b28b4: bl       #0x3b0e8c ; _Z12CF__CalcHurtP9CharacterS0_ii
003b28b8: b        #0x3b2720
003b28bc: mov      r2, #0
003b28c0: mov      r1, r2
003b28c4: b        #0x3b2850
003b28c8: mov      r0, #0
003b28cc: b        #0x3b2720
003b28d0: ldr      ip, [pc, #0x554]
003b28d4: add      ip, pc, ip
003b28d8: ldr      r1, [ip, #0x1c]
003b28dc: cmp      r1, #0
003b28e0: beq      #0x3b2bd8
003b28e4: ldr      r2, [ip, #0x20]
003b28e8: cmp      r2, #0
003b28ec: beq      #0x3b2bd8
003b28f0: mov      r3, #0
003b28f4: str      r3, [sp]
003b28f8: ldr      r0, [ip, #0x2c]
003b28fc: str      r0, [sp, #4]
003b2900: ldrb     lr, [ip, #0x30]
003b2904: add      r0, sp, #0x1c
003b2908: str      lr, [sp, #8]
003b290c: ldrb     ip, [ip, #0x31]
003b2910: str      ip, [sp, #0xc]
003b2914: bl       #0x3b1fb8 ; _Z14CF__CalcDamageP9CharacterS0_iiibb
003b2918: add      r0, sp, #0x28
003b291c: ldm      r0, {r0, r1, r2, r4}
003b2920: ldr      ip, [sp, #0x24]
003b2924: ldr      lr, [sp, #0x20]
003b2928: ldr      r3, [sp, #0x1c]
003b292c: stm      r5, {r3, r4}
003b2930: str      lr, [r5, #0x24]
003b2934: str      ip, [r5, #0x10]
003b2938: str      r0, [r5, #0x14]
003b293c: str      r1, [r5, #8]
003b2940: str      r2, [r5, #0xc]
003b2944: b        #0x3b27c0
003b2948: mov      r0, #0x64
003b294c: bl       #0x3af6d8 ; 
003b2950: ldr      r3, [pc, #0x4d8]
003b2954: mov      r2, r0
003b2958: add      r3, pc, r3
003b295c: ldr      r0, [r3, #0x1c]
003b2960: cmp      r0, #0
003b2964: beq      #0x3b2bb8
003b2968: ldr      r1, [r3, #0x20]
003b296c: cmp      r1, #0
003b2970: beq      #0x3b2bb8
003b2974: lsl      r2, r2, #8
003b2978: mov      r3, #0
003b297c: bl       #0x3b0fa0 ; _Z12CF__CalcStunP9CharacterS0_ii
003b2980: ldrb     r3, [r5, #0x18]
003b2984: tst      r4, #0x2000
003b2988: bfi      r3, r0, #6, #1
003b298c: strb     r3, [r5, #0x18]
003b2990: beq      #0x3b2754
003b2994: mov      r0, #0x64
003b2998: bl       #0x3af6d8 ; 
003b299c: ldr      r3, [pc, #0x490]
003b29a0: mov      r2, r0
003b29a4: add      r3, pc, r3
003b29a8: ldr      r0, [r3, #0x1c]
003b29ac: cmp      r0, #0
003b29b0: beq      #0x3b2bc0
003b29b4: ldr      r1, [r3, #0x20]
003b29b8: cmp      r1, #0
003b29bc: beq      #0x3b2bc0
003b29c0: lsl      r2, r2, #8
003b29c4: mov      r3, #0
003b29c8: bl       #0x3b0c64 ; _Z12CF__CalcFearP9CharacterS0_ii
003b29cc: ldrb     r3, [r5, #0x18]
003b29d0: tst      r4, #0x8000
003b29d4: bfi      r3, r0, #5, #1
003b29d8: strb     r3, [r5, #0x18]
003b29dc: beq      #0x3b2764
003b29e0: mov      r0, #0x64
003b29e4: bl       #0x3af6d8 ; 
003b29e8: ldr      r3, [pc, #0x448]
003b29ec: mov      r2, r0
003b29f0: add      r3, pc, r3
003b29f4: ldr      r0, [r3, #0x1c]
003b29f8: cmp      r0, #0
003b29fc: beq      #0x3b2bc8
003b2a00: ldr      r1, [r3, #0x20]
003b2a04: cmp      r1, #0
003b2a08: beq      #0x3b2bc8
003b2a0c: lsl      r2, r2, #8
003b2a10: mov      r3, #0
003b2a14: bl       #0x3b0ad4 ; _Z12CF__CalcSlowP9CharacterS0_ii
003b2a18: ldrb     r3, [r5, #0x19]
003b2a1c: bfi      r3, r0, #0, #1
003b2a20: strb     r3, [r5, #0x19]
003b2a24: b        #0x3b276c
003b2a28: mov      r0, #0x64
003b2a2c: bl       #0x3af6d8 ; 
003b2a30: ldr      r3, [pc, #0x404]
003b2a34: mov      r2, r0
003b2a38: add      r3, pc, r3
003b2a3c: ldr      r0, [r3, #0x1c]
003b2a40: cmp      r0, #0
003b2a44: beq      #0x3b2bb0
003b2a48: ldr      r1, [r3, #0x20]
003b2a4c: cmp      r1, #0
003b2a50: beq      #0x3b2bb0
003b2a54: lsl      r2, r2, #8
003b2a58: mov      r3, #0
003b2a5c: bl       #0x3b0d78 ; _Z12CF__CalcPushP9CharacterS0_ii
003b2a60: ldrb     r3, [r5, #0x18]
003b2a64: bfi      r3, r0, #7, #1
003b2a68: strb     r3, [r5, #0x18]
003b2a6c: b        #0x3b273c
003b2a70: mov      r0, #0x64
003b2a74: bl       #0x3af6d8 ; 
003b2a78: ldr      r3, [pc, #0x3c0]
003b2a7c: lsl      r8, r0, #8
003b2a80: add      r3, pc, r3
003b2a84: ldr      r0, [r3, #0x1c]
003b2a88: cmp      r0, #0
003b2a8c: beq      #0x3b2bd0
003b2a90: ldr      r1, [r3, #0x20]
003b2a94: cmp      r1, #0
003b2a98: beq      #0x3b2bd0
003b2a9c: mov      r2, r8
003b2aa0: mov      r3, #0
003b2aa4: bl       #0x3b0868 ; _Z12CF__CalcCritP9CharacterS0_ii
003b2aa8: ldrb     r3, [r5, #0x18]
003b2aac: tst      r4, #0x80
003b2ab0: bfi      r3, r0, #3, #1
003b2ab4: strb     r3, [r5, #0x18]
003b2ab8: beq      #0x3b287c
003b2abc: b        #0x3b26ec
003b2ac0: mov      r0, #0x64
003b2ac4: bl       #0x3af6d8 ; 
003b2ac8: ldr      r3, [pc, #0x374]
003b2acc: mov      r2, r0
003b2ad0: add      r3, pc, r3
003b2ad4: ldr      r0, [r3, #0x1c]
003b2ad8: cmp      r0, #0
003b2adc: beq      #0x3b2bf8
003b2ae0: ldr      r1, [r3, #0x20]
003b2ae4: cmp      r1, #0
003b2ae8: beq      #0x3b2bf8
003b2aec: lsl      r2, r2, #8
003b2af0: mov      r3, r8
003b2af4: bl       #0x3b0918 ; _Z13CF__CalcBlockP9CharacterS0_ii
003b2af8: ldrb     r3, [r5, #0x18]
003b2afc: bfi      r3, r0, #2, #1
003b2b00: strb     r3, [r5, #0x18]
003b2b04: b        #0x3b26d0
003b2b08: mov      r0, #0x64
003b2b0c: bl       #0x3af6d8 ; 
003b2b10: ldr      r3, [pc, #0x330]
003b2b14: lsl      r8, r0, #8
003b2b18: add      r3, pc, r3
003b2b1c: ldr      r0, [r3, #0x1c]
003b2b20: cmp      r0, #0
003b2b24: beq      #0x3b2bd0
003b2b28: ldr      r1, [r3, #0x20]
003b2b2c: cmp      r1, #0
003b2b30: beq      #0x3b2bd0
003b2b34: mov      r2, r8
003b2b38: mov      r3, #2
003b2b3c: bl       #0x3b0868 ; _Z12CF__CalcCritP9CharacterS0_ii
003b2b40: b        #0x3b2aa8
003b2b44: mov      r0, #0x64
003b2b48: bl       #0x3af6d8 ; 
003b2b4c: ldr      r3, [pc, #0x2f8]
003b2b50: mov      r2, r0
003b2b54: add      r3, pc, r3
003b2b58: ldr      r0, [r3, #0x1c]
003b2b5c: cmp      r0, #0
003b2b60: beq      #0x3b28bc
003b2b64: ldr      r1, [r3, #0x20]
003b2b68: cmp      r1, #0
003b2b6c: beq      #0x3b28bc
003b2b70: ldr      ip, [r3, #0x2c]
003b2b74: lsl      r2, r2, #8
003b2b78: str      ip, [sp]
003b2b7c: ldrb     ip, [r3, #0x30]
003b2b80: mov      r3, #2
003b2b84: str      ip, [sp, #4]
003b2b88: bl       #0x3b1df0 ; _Z19CF__CalcMissOrDodgeP9CharacterS0_iiib
003b2b8c: sxth     r0, r0
003b2b90: ubfx     r3, r0, #8, #8
003b2b94: strb     r3, [sp, #0x11]
003b2b98: strb     r0, [sp, #0x10]
003b2b9c: ldrh     r3, [sp, #0x10]
003b2ba0: strh     r3, [sp, #0x38]
003b2ba4: lsr      r2, r3, #8
003b2ba8: uxtb     r1, r3
003b2bac: b        #0x3b2850
003b2bb0: mov      r0, #0
003b2bb4: b        #0x3b2a60
003b2bb8: mov      r0, #0
003b2bbc: b        #0x3b2980
003b2bc0: mov      r0, #0
003b2bc4: b        #0x3b29cc
003b2bc8: mov      r0, #0
003b2bcc: b        #0x3b2a18
003b2bd0: mov      r0, #0
003b2bd4: b        #0x3b2aa8
003b2bd8: mov      r3, #0
003b2bdc: mov      lr, r3
003b2be0: mov      ip, r3
003b2be4: mov      r0, r3
003b2be8: mov      r1, r3
003b2bec: mov      r2, r3
003b2bf0: mov      r4, r3
003b2bf4: b        #0x3b292c
003b2bf8: mov      r0, #0
003b2bfc: b        #0x3b2af8
003b2c00: ldr      ip, [pc, #0x248]
003b2c04: add      ip, pc, ip
003b2c08: ldr      r1, [ip, #0x1c]
003b2c0c: cmp      r1, #0
003b2c10: beq      #0x3b2db8
003b2c14: ldr      r2, [ip, #0x20]
003b2c18: cmp      r2, #0
003b2c1c: beq      #0x3b2db8
003b2c20: mov      r0, #2
003b2c24: str      r0, [sp]
003b2c28: ldr      r0, [ip, #0x2c]
003b2c2c: str      r0, [sp, #4]
003b2c30: ldrb     lr, [ip, #0x30]
003b2c34: add      r0, sp, #0x1c
003b2c38: str      lr, [sp, #8]
003b2c3c: ldrb     ip, [ip, #0x31]
003b2c40: str      ip, [sp, #0xc]
003b2c44: bl       #0x3b1fb8 ; _Z14CF__CalcDamageP9CharacterS0_iiibb
003b2c48: add      r0, sp, #0x28
003b2c4c: ldm      r0, {r0, r1, r2, lr}
003b2c50: ldr      ip, [sp, #0x24]
003b2c54: ldr      r3, [sp, #0x1c]
003b2c58: stm      r5, {r3, lr}
003b2c5c: str      ip, [r5, #0x10]
003b2c60: str      r0, [r5, #0x14]
003b2c64: str      r1, [r5, #8]
003b2c68: str      r2, [r5, #0xc]
003b2c6c: b        #0x3b27c0
003b2c70: mov      r0, #0x64
003b2c74: bl       #0x3af6d8 ; 
003b2c78: ldr      r3, [pc, #0x1d4]
003b2c7c: mov      r2, r0
003b2c80: add      r3, pc, r3
003b2c84: ldr      r0, [r3, #0x1c]
003b2c88: cmp      r0, #0
003b2c8c: beq      #0x3b2bc8
003b2c90: ldr      r1, [r3, #0x20]
003b2c94: cmp      r1, #0
003b2c98: beq      #0x3b2bc8
003b2c9c: lsl      r2, r2, #8
003b2ca0: mov      r3, #2
003b2ca4: bl       #0x3b0ad4 ; _Z12CF__CalcSlowP9CharacterS0_ii
003b2ca8: b        #0x3b2a18
003b2cac: mov      r0, #0x64
003b2cb0: bl       #0x3af6d8 ; 
003b2cb4: ldr      r3, [pc, #0x19c]
003b2cb8: mov      r2, r0
003b2cbc: add      r3, pc, r3
003b2cc0: ldr      r0, [r3, #0x1c]
003b2cc4: cmp      r0, #0
003b2cc8: beq      #0x3b2bc0
003b2ccc: ldr      r1, [r3, #0x20]
003b2cd0: cmp      r1, #0
003b2cd4: beq      #0x3b2bc0
003b2cd8: lsl      r2, r2, #8
003b2cdc: mov      r3, #2
003b2ce0: bl       #0x3b0c64 ; _Z12CF__CalcFearP9CharacterS0_ii
003b2ce4: b        #0x3b29cc
003b2ce8: mov      r0, #0x64
003b2cec: bl       #0x3af6d8 ; 
003b2cf0: ldr      r3, [pc, #0x164]
003b2cf4: mov      r2, r0
003b2cf8: add      r3, pc, r3
003b2cfc: ldr      r0, [r3, #0x1c]
003b2d00: cmp      r0, #0
003b2d04: beq      #0x3b2bb8
003b2d08: ldr      r1, [r3, #0x20]
003b2d0c: cmp      r1, #0
003b2d10: beq      #0x3b2bb8
003b2d14: lsl      r2, r2, #8
003b2d18: mov      r3, #2
003b2d1c: bl       #0x3b0fa0 ; _Z12CF__CalcStunP9CharacterS0_ii
003b2d20: b        #0x3b2980
003b2d24: mov      r0, #0x64
003b2d28: bl       #0x3af6d8 ; 
003b2d2c: ldr      r3, [pc, #0x12c]
003b2d30: mov      r2, r0
003b2d34: add      r3, pc, r3
003b2d38: ldr      r0, [r3, #0x1c]
003b2d3c: cmp      r0, #0
003b2d40: beq      #0x3b2bb0
003b2d44: ldr      r1, [r3, #0x20]
003b2d48: cmp      r1, #0
003b2d4c: beq      #0x3b2bb0
003b2d50: lsl      r2, r2, #8
003b2d54: mov      r3, #2
003b2d58: bl       #0x3b0d78 ; _Z12CF__CalcPushP9CharacterS0_ii
003b2d5c: b        #0x3b2a60
003b2d60: ldr      ip, [pc, #0xfc]
003b2d64: add      ip, pc, ip
003b2d68: ldr      r1, [ip, #0x1c]
003b2d6c: cmp      r1, #0
003b2d70: beq      #0x3b2dd4
003b2d74: ldr      r2, [ip, #0x20]
003b2d78: cmp      r2, #0
003b2d7c: beq      #0x3b2dd4
003b2d80: mov      r3, #3
003b2d84: str      r3, [sp]
003b2d88: ldr      r3, [ip, #0x2c]
003b2d8c: add      r0, sp, #0x1c
003b2d90: str      r3, [sp, #4]
003b2d94: ldrb     lr, [ip, #0x30]
003b2d98: ldr      r3, [sp, #0x88]
003b2d9c: str      lr, [sp, #8]
003b2da0: ldrb     ip, [ip, #0x31]
003b2da4: str      ip, [sp, #0xc]
003b2da8: bl       #0x3b1fb8 ; _Z14CF__CalcDamageP9CharacterS0_iiibb
003b2dac: ldr      r3, [sp, #0x1c]
003b2db0: str      r3, [r5]
003b2db4: b        #0x3b27c0
003b2db8: mov      r3, #0
003b2dbc: mov      ip, r3
003b2dc0: mov      r0, r3
003b2dc4: mov      r1, r3
003b2dc8: mov      r2, r3
003b2dcc: mov      lr, r3
003b2dd0: b        #0x3b2c58
003b2dd4: mov      r3, #0
003b2dd8: str      r3, [r5]
003b2ddc: b        #0x3b27c0
003b2de0: mov      r0, #0x64
003b2de4: bl       #0x3af6d8 ; 
003b2de8: lsl      r8, r0, #8
003b2dec: b        #0x3b26f4
003b2df0: mov      r0, #0x64
003b2df4: bl       #0x3af6d8 ; 
003b2df8: lsl      r8, r0, #8
003b2dfc: b        #0x3b288c
003b2e00: bl       #0x30e310 ; 
003b2e04: subseq   r2, lr, r8, asr #8
003b2e08: andeq    r4, r0, ip, lsr #1
003b2e0c: ldrsbeq  r1, [r1], #-0x64
003b2e10: subseq   r0, pc, r0, lsr #5
003b2e14: andeq    r0, r0, r4, lsl #17
003b2e18: subseq   r1, r1, ip, lsr #10
003b2e1c: subseq   r1, r1, ip, ror #10
003b2e20: subseq   r0, pc, r0, lsr #3
003b2e24: subseq   r1, r1, r0, asr #9
003b2e28: subseq   r0, pc, r8, lsl #2
003b2e2c: subseq   r0, pc, r4, asr #1
003b2e30: subseq   r0, pc, r0, asr #32
003b2e34: ldrsheq  pc, [lr], #-0xf4
003b2e38: subseq   pc, lr, r8, lsr #31
003b2e3c: subseq   pc, lr, r0, ror #30
003b2e40: subseq   pc, lr, r8, lsl pc
003b2e44: subseq   pc, lr, r8, asr #29
003b2e48: subseq   pc, lr, r0, lsl #29
003b2e4c: subseq   pc, lr, r4, asr #28

# _ZN9AISPlayer7OnAggroEP9Character
003ddb70: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003ddb74: ldr      r4, [pc, #0x28c]
003ddb78: ldr      r6, [pc, #0x28c]
003ddb7c: ldr      r7, [pc, #0x28c]
003ddb80: add      r4, pc, r4
003ddb84: ldr      r3, [r4, r6]
003ddb88: sub      sp, sp, #0x48
003ddb8c: mov      sb, r1
003ddb90: ldr      r3, [r3]
003ddb94: mov      r5, r0
003ddb98: add      r8, sp, #0x2c
003ddb9c: str      r3, [sp, #0x44]
003ddba0: bl       #0x3dbea4 ; _ZN10AISDefault7OnAggroEP9Character
003ddba4: ldr      sl, [r4, r7]
003ddba8: mov      r0, sl
003ddbac: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003ddbb0: ldr      r1, [pc, #0x25c]
003ddbb4: add      r2, sp, #0x10
003ddbb8: mov      r0, r8
003ddbbc: add      r1, pc, r1
003ddbc0: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003ddbc4: mov      r0, sl
003ddbc8: mov      r1, r8
003ddbcc: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003ddbd0: mov      sl, r0
003ddbd4: ldr      r0, [sp, #0x40]
003ddbd8: cmp      r0, r8
003ddbdc: beq      #0x3ddbfc
003ddbe0: cmp      r0, #0
003ddbe4: beq      #0x3ddbfc
003ddbe8: ldr      r1, [sp, #0x2c]
003ddbec: rsb      r1, r0, r1
003ddbf0: cmp      r1, #0x80
003ddbf4: bhi      #0x3ddd58
003ddbf8: bl       #0x708f00 ; 
003ddbfc: cmp      sl, #0
003ddc00: bne      #0x3ddd34
003ddc04: ldr      r3, [r5, #0xd0]
003ddc08: add      r3, r3, #1
003ddc0c: str      r3, [r5, #0xd0]
003ddc10: bl       #0x7fd794 ; _Z9GetOnlinev
003ddc14: ldrb     r3, [r0, #5]
003ddc18: cmp      r3, #0
003ddc1c: bne      #0x3ddd14
003ddc20: ldr      r8, [pc, #0x1f0]
003ddc24: ldr      r3, [pc, #0x1f0]
003ddc28: ldr      r0, [r4, r8]
003ddc2c: ldr      r3, [r4, r3]
003ddc30: ldr      sl, [r3]
003ddc34: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
003ddc38: mov      r8, r0
003ddc3c: mov      r0, sb
003ddc40: ldr      sb, [r5, #0xd4]
003ddc44: bl       #0x3a3024 ; _ZNK9Character9GetCharAIEv
003ddc48: ldr      r3, [r0, #0x14]
003ddc4c: cmp      r8, #0
003ddc50: add      sb, r3, sb
003ddc54: str      sb, [r5, #0xd4]
003ddc58: beq      #0x3ddc74
003ddc5c: ldr      r3, [r8, #0x38]
003ddc60: cmp      r3, #0
003ddc64: beq      #0x3dddac
003ddc68: ldrb     r3, [r3, #0x1c8]
003ddc6c: cmp      r3, #0
003ddc70: bne      #0x3ddd60
003ddc74: ldr      r8, [r4, r7]
003ddc78: add      r7, sp, #0x14
003ddc7c: mov      r0, r8
003ddc80: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003ddc84: ldr      r1, [pc, #0x194]
003ddc88: add      r2, sp, #0xc
003ddc8c: mov      r0, r7
003ddc90: add      r1, pc, r1
003ddc94: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003ddc98: mov      r0, r8
003ddc9c: mov      r1, r7
003ddca0: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003ddca4: mov      r8, r0
003ddca8: ldr      r0, [sp, #0x28]
003ddcac: cmp      r0, r7
003ddcb0: beq      #0x3ddcd0
003ddcb4: cmp      r0, #0
003ddcb8: beq      #0x3ddcd0
003ddcbc: ldr      r1, [sp, #0x14]
003ddcc0: rsb      r1, r0, r1
003ddcc4: cmp      r1, #0x80
003ddcc8: bhi      #0x3ddda4
003ddccc: bl       #0x708f00 ; 
003ddcd0: cmp      r8, #0
003ddcd4: beq      #0x3ddcf8
003ddcd8: ldr      r0, [pc, #0x144]
003ddcdc: ldr      r1, [pc, #0x144]
003ddce0: ldr      r3, [r5, #0xd4]
003ddce4: ldr      r0, [r4, r0]
003ddce8: add      r1, pc, r1
003ddcec: ldr      r2, [r5, #0xd0]
003ddcf0: add      r0, r0, #0xa8
003ddcf4: bl       #0x30e004 ; 
003ddcf8: ldr      r3, [r4, r6]
003ddcfc: ldr      r2, [sp, #0x44]
003ddd00: ldr      r3, [r3]
003ddd04: cmp      r2, r3
003ddd08: bne      #0x3dde04
003ddd0c: add      sp, sp, #0x48
003ddd10: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003ddd14: ldr      r8, [pc, #0xfc]
003ddd18: ldr      r1, [r5, #0x98]
003ddd1c: ldr      r3, [r4, r8]
003ddd20: ldr      r0, [r3, #0x40]
003ddd24: bl       #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
003ddd28: cmp      r0, #0
003ddd2c: beq      #0x3ddcf8
003ddd30: b        #0x3ddc24
003ddd34: ldr      r0, [pc, #0xe8]
003ddd38: ldr      r1, [pc, #0xec]
003ddd3c: ldr      r2, [r5, #0xd0]
003ddd40: ldr      r0, [r4, r0]
003ddd44: add      r1, pc, r1
003ddd48: ldr      r3, [r5, #0xd4]
003ddd4c: add      r0, r0, #0xa8
003ddd50: bl       #0x30e004 ; 
003ddd54: b        #0x3ddc04
003ddd58: bl       #0x310440 ; _Z10CustomFreePv
003ddd5c: b        #0x3ddbfc
003ddd60: ldrb     r3, [sl, #0x31]
003ddd64: cmp      r3, #0
003ddd68: beq      #0x3ddc74
003ddd6c: ldr      r3, [pc, #0xbc]
003ddd70: ldr      r2, [r5, #0xd4]
003ddd74: ldr      r3, [r4, r3]
003ddd78: ldr      r3, [r3]
003ddd7c: ldr      r3, [r3, #0x14]
003ddd80: cmp      r2, r3
003ddd84: blt      #0x3ddc74
003ddd88: ldr      r1, [pc, #0xa4]
003ddd8c: mov      r0, sl
003ddd90: add      r1, pc, r1
003ddd94: bl       #0x369514 ; _ZN15VoxSoundManager13SetMusicStateEPKc
003ddd98: mov      r3, #0
003ddd9c: strb     r3, [sl, #0x31]
003ddda0: b        #0x3ddc74
003ddda4: bl       #0x310440 ; _Z10CustomFreePv
003ddda8: b        #0x3ddcd0
003dddac: ldr      r2, [pc, #0x84]
003dddb0: ldr      r2, [r4, r2]
003dddb4: ldr      r2, [r2]
003dddb8: cmp      r2, #2
003dddbc: streq    r3, [r3]
003dddc0: beq      #0x3ddc68
003dddc4: cmp      r2, #1
003dddc8: bne      #0x3ddc68
003dddcc: ldr      r0, [pc, #0x50]
003dddd0: ldr      r1, [pc, #0x64]
003dddd4: ldr      r2, [pc, #0x64]
003dddd8: ldr      r0, [r4, r0]
003ddddc: ldr      r3, [pc, #0x60]
003ddde0: mov      ip, #0x1dc
003ddde4: add      r1, pc, r1
003ddde8: add      r3, pc, r3
003dddec: add      r0, r0, #0xa8
003dddf0: add      r2, pc, r2
003dddf4: str      ip, [sp]
003dddf8: bl       #0x30e004 ; 
003dddfc: ldr      r3, [r8, #0x38]
003dde00: b        #0x3ddc68
003dde04: bl       #0x30e310 ; 
003dde08: subseq   r6, fp, r0, lsl pc
003dde0c: andeq    r4, r0, ip, lsr #1
003dde10: andeq    r0, r0, r4, lsl #17
003dde14: subeq    r7, lr, ip, ror #31
003dde18: strdeq   r3, r4, [r0], -r4
003dde1c: andeq    r0, r0, r4, lsr #27
003dde20: subeq    r7, lr, r8, lsl pc
003dde24: andeq    r1, r0, r0, asr #19
003dde28: subeq    r7, lr, r0, lsr #30
003dde2c: subeq    r7, lr, ip, ror lr
003dde30: andeq    r3, r0, r8, asr #5
003dde34: umaaleq  r3, lr, r8, lr
003dde38: andeq    r3, r0, r0, asr #19
003dde3c: strdeq   r0, r1, [lr], #-0x54
003dde40: subeq    r7, lr, r0, lsl #28
003dde44: subeq    r3, lr, r0, ror lr

# _ZSt13__adjust_heapIPSt4pairIiiEiS1_N17PlayerStatManager9_StatCompEEvT_T0_S6_T1_T2_
003791d4: push     {r4, r5, r6, r7, lr}
003791d8: mov      ip, r1
003791dc: add      r1, r1, #1
003791e0: lsl      lr, r1, #1
003791e4: cmp      lr, r2
003791e8: sub      sp, sp, #0x14
003791ec: movge    r1, ip
003791f0: bge      #0x37923c
003791f4: mov      r5, ip
003791f8: sub      r1, lr, #1
003791fc: ldr      r6, [r0, r1, lsl #3]
00379200: ldr      r7, [r0, lr, lsl #3]
00379204: add      r4, r0, lr, lsl #3
00379208: cmp      r7, r6
0037920c: addgt    r4, r0, r1, lsl #3
00379210: ldr      r6, [r4]
00379214: movle    r1, lr
00379218: add      lr, r1, #1
0037921c: str      r6, [r0, r5, lsl #3]
00379220: ldr      r4, [r4, #4]
00379224: lsl      lr, lr, #1
00379228: add      r5, r0, r5, lsl #3
0037922c: cmp      r2, lr
00379230: str      r4, [r5, #4]
00379234: mov      r5, r1
00379238: bgt      #0x3791f8
0037923c: cmp      lr, r2
00379240: beq      #0x37926c
00379244: ldm      r3, {r4, lr}
00379248: mov      r2, ip
0037924c: add      r3, sp, #8
00379250: mov      ip, #0
00379254: str      r4, [sp, #8]
00379258: str      lr, [sp, #0xc]
0037925c: strb     ip, [sp]
00379260: bl       #0x37915c ; _ZSt11__push_heapIPSt4pairIiiEiS1_N17PlayerStatManager9_StatCompEEvT_T0_S6_T1_T2_
00379264: add      sp, sp, #0x14
00379268: pop      {r4, r5, r6, r7, pc}
0037926c: sub      lr, lr, #1
00379270: ldr      r5, [r0, lr, lsl #3]
00379274: add      r4, r0, lr, lsl #3
00379278: add      r2, r0, r1, lsl #3
0037927c: str      r5, [r0, r1, lsl #3]
00379280: ldr      r4, [r4, #4]
00379284: mov      r1, lr
00379288: str      r4, [r2, #4]
0037928c: b        #0x379244

# _ZN9Character6HitForEjP10GameObject
003a8bc4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a8bc8: ldr      r4, [pc, #0x64c]
003a8bcc: ldr      r6, [pc, #0x64c]
003a8bd0: mov      fp, r1
003a8bd4: add      r4, pc, r4
003a8bd8: ldr      ip, [r4, r6]
003a8bdc: sub      sp, sp, #0x6c
003a8be0: ldr      r3, [r0]
003a8be4: ldr      r1, [ip]
003a8be8: mov      r5, r0
003a8bec: mov      r7, r2
003a8bf0: str      r1, [sp, #0x64]
003a8bf4: mov      lr, pc
003a8bf8: ldr      pc, [r3, #0x34]
003a8bfc: subs     sl, r0, #0
003a8c00: beq      #0x3a8c20
003a8c04: ldr      r3, [r4, r6]
003a8c08: ldr      r2, [sp, #0x64]
003a8c0c: ldr      r3, [r3]
003a8c10: cmp      r2, r3
003a8c14: bne      #0x3a9218
003a8c18: add      sp, sp, #0x6c
003a8c1c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a8c20: ldr      r0, [pc, #0x5fc]
003a8c24: mov      r1, sl
003a8c28: mov      r2, #1
003a8c2c: ldr      r3, [r4, r0]
003a8c30: str      r0, [sp, #8]
003a8c34: add      r8, sp, #0x4c
003a8c38: ldr      r0, [r3, #0x40]
003a8c3c: movw     r3, #0x1088
003a8c40: ldr      r3, [r5, r3]
003a8c44: str      r3, [sp, #0x1c]
003a8c48: movw     r3, #0x1090
003a8c4c: ldr      r3, [r5, r3]
003a8c50: str      r3, [sp, #0x18]
003a8c54: bl       #0x36e478 ; _ZN13PlayerManager14GetLocalPlayerEib
003a8c58: ldr      r2, [pc, #0x5c8]
003a8c5c: str      r2, [sp, #0xc]
003a8c60: ldr      sb, [r4, r2]
003a8c64: ldr      r0, [r0, #0x660]
003a8c68: str      r0, [sp, #0x14]
003a8c6c: mov      r0, sb
003a8c70: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003a8c74: ldr      r1, [pc, #0x5b0]
003a8c78: add      r2, sp, #0x30
003a8c7c: mov      r0, r8
003a8c80: add      r1, pc, r1
003a8c84: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003a8c88: mov      r0, sb
003a8c8c: mov      r1, r8
003a8c90: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003a8c94: cmp      r0, #0
003a8c98: bne      #0x3a9014
003a8c9c: mov      r0, r8
003a8ca0: bl       #0x318254 ; _ZNSsD1Ev
003a8ca4: bl       #0x7fd794 ; _Z9GetOnlinev
003a8ca8: ldrb     r3, [r0, #5]
003a8cac: cmp      r3, #0
003a8cb0: beq      #0x3a8f30
003a8cb4: ldr      r0, [sp, #8]
003a8cb8: ldr      r3, [r4, r0]
003a8cbc: ldr      r3, [r3, #0x40]
003a8cc0: cmp      r3, #0
003a8cc4: asreq    r2, fp, #8
003a8cc8: streq    r2, [sp, #0x10]
003a8ccc: rsbeq    r2, fp, #0
003a8cd0: beq      #0x3a8d00
003a8cd4: ldr      r3, [r3, #0x714]
003a8cd8: cmp      r3, #0
003a8cdc: beq      #0x3a8f54
003a8ce0: cmp      r3, #5
003a8ce4: asreq    r0, fp, #8
003a8ce8: streq    r0, [sp, #0x10]
003a8cec: rsbeq    r2, fp, #0
003a8cf0: beq      #0x3a8d00
003a8cf4: mov      r0, #0
003a8cf8: str      r0, [sp, #0x10]
003a8cfc: mov      r2, r0
003a8d00: add      sl, r5, #0x560
003a8d04: mov      r0, sl
003a8d08: mov      r1, #0x24
003a8d0c: ldr      r8, [pc, #0x51c]
003a8d10: bl       #0x3e0708 ; _ZN14CharProperties9PROPS_AddEii
003a8d14: ldr      r2, [sp, #8]
003a8d18: add      r8, pc, r8
003a8d1c: mov      r1, r8
003a8d20: ldr      r0, [r4, r2]
003a8d24: bl       #0x320e14 ; _ZN11Application15IsSavedOptionOnEPKc
003a8d28: cmp      r0, #0
003a8d2c: beq      #0x3a8fcc
003a8d30: ldr      r3, [r5]
003a8d34: mov      r0, r5
003a8d38: mov      lr, pc
003a8d3c: ldr      pc, [r3, #0x28]
003a8d40: cmp      r0, #0
003a8d44: bne      #0x3a9038
003a8d48: mov      r0, sl
003a8d4c: mov      r1, #0x24
003a8d50: mov      r2, #0
003a8d54: bl       #0x3e07a0 ; _ZN14CharProperties9PROPS_SetEii
003a8d58: movw     r3, #0x1088
003a8d5c: ldr      r3, [r5, r3]
003a8d60: cmp      r3, #0
003a8d64: movgt    r0, #0
003a8d68: strgt    r0, [sp, #0xc]
003a8d6c: ble      #0x3a90e8
003a8d70: ldr      r3, [r5]
003a8d74: mov      r0, r5
003a8d78: mov      lr, pc
003a8d7c: ldr      pc, [r3, #0x28]
003a8d80: cmp      r0, #0
003a8d84: beq      #0x3a8f64
003a8d88: movw     r3, #0x1090
003a8d8c: ldr      r3, [r5, r3]
003a8d90: movw     r2, #0x1088
003a8d94: ldr      r2, [r5, r2]
003a8d98: add      r3, r3, r3, lsr #31
003a8d9c: cmp      r2, r3, asr #1
003a8da0: bgt      #0x3a8f64
003a8da4: bl       #0x7fd794 ; _Z9GetOnlinev
003a8da8: ldrb     r3, [r0, #5]
003a8dac: cmp      r3, #0
003a8db0: beq      #0x3a9060
003a8db4: movw     r3, #0x1448
003a8db8: ldrb     r2, [r5, r3]
003a8dbc: cmp      r2, #0
003a8dc0: beq      #0x3a8e54
003a8dc4: ldr      r0, [sp, #0x14]
003a8dc8: mov      r8, #0
003a8dcc: strb     r8, [r5, r3]
003a8dd0: cmp      r5, r0
003a8dd4: beq      #0x3a91a4
003a8dd8: ldr      r3, [pc, #0x454]
003a8ddc: ldr      r2, [r4, r3]
003a8de0: ldr      r3, [pc, #0x450]
003a8de4: ldr      sl, [r2]
003a8de8: ldr      r3, [r4, r3]
003a8dec: cmp      sl, #0
003a8df0: ldr      r3, [r3]
003a8df4: str      r3, [sp, #0x14]
003a8df8: beq      #0x3a919c
003a8dfc: ldr      r3, [pc, #0x438]
003a8e00: ldr      fp, [pc, #0x438]
003a8e04: ldr      r3, [r4, r3]
003a8e08: add      fp, pc, fp
003a8e0c: ldr      sb, [r3]
003a8e10: b        #0x3a8e20
003a8e14: add      r8, r8, #1
003a8e18: cmp      r8, sl
003a8e1c: beq      #0x3a919c
003a8e20: mov      r0, fp
003a8e24: ldr      r1, [sb, r8, lsl #2]
003a8e28: bl       #0x30e31c ; 
003a8e2c: cmp      r0, #0
003a8e30: bne      #0x3a8e14
003a8e34: mov      r1, r8
003a8e38: mov      ip, #0
003a8e3c: mov      r2, ip
003a8e40: ldr      r0, [sp, #0x14]
003a8e44: mov      r3, ip
003a8e48: str      ip, [sp]
003a8e4c: str      ip, [sp, #4]
003a8e50: bl       #0x36b80c ; _ZN15VoxSoundManager4PlayEibiib
003a8e54: add      r8, sp, #0x20
003a8e58: mov      r1, r7
003a8e5c: mov      r0, r8
003a8e60: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
003a8e64: mov      r0, r8
003a8e68: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
003a8e6c: ldr      r3, [pc, #0x3d0]
003a8e70: subs     r8, r0, #0
003a8e74: ldr      r3, [r4, r3]
003a8e78: ldr      r7, [r3]
003a8e7c: beq      #0x3a8c04
003a8e80: ldr      r3, [r8]
003a8e84: mov      lr, pc
003a8e88: ldr      pc, [r3, #0x28]
003a8e8c: cmp      r0, #0
003a8e90: beq      #0x3a8c04
003a8e94: cmp      r5, r8
003a8e98: beq      #0x3a8c04
003a8e9c: ldr      r2, [sp, #8]
003a8ea0: mov      r1, r8
003a8ea4: ldr      r3, [r4, r2]
003a8ea8: ldr      r0, [r3, #0x40]
003a8eac: bl       #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
003a8eb0: cmp      r0, #0
003a8eb4: beq      #0x3a8c04
003a8eb8: ldr      r3, [sp, #0x10]
003a8ebc: cmp      r3, #0xc7
003a8ec0: bgt      #0x3a9138
003a8ec4: ldr      r0, [sp, #0x10]
003a8ec8: cmp      r0, #0x95
003a8ecc: bgt      #0x3a9150
003a8ed0: ldr      r2, [sp, #0x10]
003a8ed4: cmp      r2, #0x63
003a8ed8: bgt      #0x3a9168
003a8edc: ldr      r3, [sp, #0x10]
003a8ee0: cmp      r3, #0x31
003a8ee4: bgt      #0x3a9180
003a8ee8: ldr      r0, [sp, #0x1c]
003a8eec: ldr      r2, [sp, #0x18]
003a8ef0: cmp      r0, r2
003a8ef4: bne      #0x3a8c04
003a8ef8: ldr      r3, [sp, #0xc]
003a8efc: cmp      r3, #0
003a8f00: beq      #0x3a8c04
003a8f04: mov      r0, r5
003a8f08: bl       #0x3a3158 ; _ZNK9Character6IsBossEv
003a8f0c: cmp      r0, #0
003a8f10: beq      #0x3a9204
003a8f14: ldr      r0, [pc, #0x32c]
003a8f18: add      r0, pc, r0
003a8f1c: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003a8f20: mov      r1, r0
003a8f24: mov      r0, r7
003a8f28: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003a8f2c: b        #0x3a8c04
003a8f30: ldr      r2, [sp, #0x14]
003a8f34: cmp      r2, #0
003a8f38: beq      #0x3a8cf4
003a8f3c: ldr      r3, [r2]
003a8f40: mov      r0, r2
003a8f44: mov      lr, pc
003a8f48: ldr      pc, [r3, #0x34]
003a8f4c: cmp      r0, #0
003a8f50: bne      #0x3a8cf4
003a8f54: asr      r3, fp, #8
003a8f58: str      r3, [sp, #0x10]
003a8f5c: rsb      r2, fp, #0
003a8f60: b        #0x3a8d00
003a8f64: ldr      r3, [r5]
003a8f68: mov      r0, r5
003a8f6c: mov      lr, pc
003a8f70: ldr      pc, [r3, #0x28]
003a8f74: cmp      r0, #0
003a8f78: beq      #0x3a8e54
003a8f7c: movw     r8, #0x1448
003a8f80: ldrb     r3, [r5, r8]
003a8f84: cmp      r3, #0
003a8f88: bne      #0x3a8e54
003a8f8c: movw     r3, #0x1088
003a8f90: ldr      r0, [r5, r3]
003a8f94: bl       #0x30e964 ; 
003a8f98: movw     r3, #0x1090
003a8f9c: mov      sl, r0
003a8fa0: ldr      r0, [r5, r3]
003a8fa4: bl       #0x30e964 ; 
003a8fa8: mov      r1, #0x3f400000
003a8fac: bl       #0x30ed6c ; 
003a8fb0: mov      r1, r0
003a8fb4: mov      r0, sl
003a8fb8: bl       #0x30e4b4 ; 
003a8fbc: cmp      r0, #0
003a8fc0: movne    r3, #1
003a8fc4: strbne   r3, [r5, r8]
003a8fc8: b        #0x3a8e54
003a8fcc: ldr      r3, [sp, #0xc]
003a8fd0: add      sb, sp, #0x34
003a8fd4: ldr      fp, [r4, r3]
003a8fd8: mov      r0, fp
003a8fdc: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003a8fe0: mov      r1, r8
003a8fe4: add      r2, sp, #0x2c
003a8fe8: mov      r0, sb
003a8fec: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003a8ff0: mov      r1, sb
003a8ff4: mov      r0, fp
003a8ff8: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003a8ffc: mov      r8, r0
003a9000: mov      r0, sb
003a9004: bl       #0x318254 ; _ZNSsD1Ev
003a9008: cmp      r8, #0
003a900c: beq      #0x3a8d58
003a9010: b        #0x3a8d30
003a9014: mov      r0, r5
003a9018: bl       #0x3a3064 ; _ZNK9Character9IsMonsterEv
003a901c: cmp      r0, #0
003a9020: beq      #0x3a8c9c
003a9024: mov      r0, r8
003a9028: str      sl, [sp, #0x10]
003a902c: bl       #0x318254 ; _ZNSsD1Ev
003a9030: ldr      r2, [sp, #0x10]
003a9034: b        #0x3a8d00
003a9038: ldr      r3, [r5, #0x418]
003a903c: cmp      r3, #0
003a9040: beq      #0x3a8d58
003a9044: mov      r0, r3
003a9048: ldr      r3, [r3]
003a904c: mov      lr, pc
003a9050: ldr      pc, [r3, #0x28]
003a9054: cmp      r0, #0
003a9058: bne      #0x3a8d58
003a905c: b        #0x3a8d48
003a9060: mov      r0, r5
003a9064: bl       #0x3bb8e4 ; _ZN9Character20SG_GetGameDifficultyEv
003a9068: subs     r8, r0, #0
003a906c: bne      #0x3a8db4
003a9070: ldr      r0, [sp, #8]
003a9074: ldr      r3, [r4, r0]
003a9078: ldr      r3, [r3, #0x4c]
003a907c: ldrb     r3, [r3, #0x2d]
003a9080: cmp      r3, #0
003a9084: beq      #0x3a8db4
003a9088: ldr      r3, [pc, #0x1bc]
003a908c: ldr      r1, [pc, #0x1bc]
003a9090: mov      r2, #1
003a9094: ldr      sl, [r4, r3]
003a9098: add      r1, pc, r1
003a909c: mov      r0, sl
003a90a0: bl       #0x4591f0 ; _ZNK13ScriptManager13GetIDFromNameEPKcb
003a90a4: cmn      r0, #1
003a90a8: mov      r1, r0
003a90ac: beq      #0x3a90c0
003a90b0: mov      r0, sl
003a90b4: mov      r3, r8
003a90b8: mvn      r2, #0
003a90bc: bl       #0x4605c0 ; _ZN13ScriptManager11StartScriptEiib
003a90c0: ldr      r2, [sp, #8]
003a90c4: mov      r1, #0
003a90c8: ldr      r3, [r4, r2]
003a90cc: ldr      r2, [r3, #0x4c]
003a90d0: ldr      r3, [pc, #0x17c]
003a90d4: strb     r1, [r2, #0x2d]
003a90d8: ldr      r3, [r4, r3]
003a90dc: ldr      r0, [r3]
003a90e0: bl       #0x317e98 ; _ZN16updateJob_thread6Start2Ev
003a90e4: b        #0x3a8db4
003a90e8: mov      r0, sl
003a90ec: mov      r1, #0x24
003a90f0: mov      r2, #0
003a90f4: bl       #0x3e07a0 ; _ZN14CharProperties9PROPS_SetEii
003a90f8: mov      r2, #0
003a90fc: ldr      r0, [r5, #0x378]
003a9100: mov      r1, r7
003a9104: bl       #0x40570c ; _ZN12v2Controller8Cmd_KillEP10GameObjectb
003a9108: ldr      r3, [r5]
003a910c: mov      r0, r5
003a9110: mov      lr, pc
003a9114: ldr      pc, [r3, #0x54]
003a9118: cmp      r0, #0
003a911c: moveq    r3, #3
003a9120: streq    r3, [r5, #0x11c]
003a9124: movne    r2, #1
003a9128: moveq    r3, #1
003a912c: strne    r2, [sp, #0xc]
003a9130: streq    r3, [sp, #0xc]
003a9134: b        #0x3a8d70
003a9138: ldr      r0, [pc, #0x118]
003a913c: add      r0, pc, r0
003a9140: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003a9144: mov      r1, r0
003a9148: mov      r0, r7
003a914c: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003a9150: ldr      r0, [pc, #0x104]
003a9154: add      r0, pc, r0
003a9158: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003a915c: mov      r1, r0
003a9160: mov      r0, r7
003a9164: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003a9168: ldr      r0, [pc, #0xf0]
003a916c: add      r0, pc, r0
003a9170: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003a9174: mov      r1, r0
003a9178: mov      r0, r7
003a917c: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003a9180: ldr      r0, [pc, #0xdc]
003a9184: add      r0, pc, r0
003a9188: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003a918c: mov      r1, r0
003a9190: mov      r0, r7
003a9194: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003a9198: b        #0x3a8ee8
003a919c: mvn      r1, #0
003a91a0: b        #0x3a8e38
003a91a4: ldr      r3, [pc, #0x88]
003a91a8: ldr      r2, [r4, r3]
003a91ac: ldr      r3, [pc, #0x84]
003a91b0: ldr      sl, [r2]
003a91b4: ldr      r3, [r4, r3]
003a91b8: cmp      sl, r8
003a91bc: ldr      r3, [r3]
003a91c0: str      r3, [sp, #0x14]
003a91c4: beq      #0x3a919c
003a91c8: ldr      r3, [pc, #0x6c]
003a91cc: ldr      sb, [pc, #0x94]
003a91d0: ldr      r3, [r4, r3]
003a91d4: add      sb, pc, sb
003a91d8: ldr      fp, [r3]
003a91dc: b        #0x3a91ec
003a91e0: add      r8, r8, #1
003a91e4: cmp      r8, sl
003a91e8: beq      #0x3a919c
003a91ec: mov      r0, sb
003a91f0: ldr      r1, [fp, r8, lsl #2]
003a91f4: bl       #0x30e31c ; 
003a91f8: cmp      r0, #0
003a91fc: bne      #0x3a91e0
003a9200: b        #0x3a8e34
003a9204: mov      r0, r5
003a9208: bl       #0x3a3144 ; _ZNK9Character10IsMiniBossEv
003a920c: cmp      r0, #0
003a9210: beq      #0x3a8c04
003a9214: b        #0x3a8f14
003a9218: bl       #0x30e310 ; 
003a921c: ldrheq   fp, [lr], #-0xec
003a9220: andeq    r4, r0, ip, lsr #1
003a9224: strdeq   r3, r4, [r0], -r4
003a9228: andeq    r0, r0, r4, lsl #17
003a922c: subseq   sl, r1, r8, lsl #15
003a9230: subseq   sl, r1, r0, lsl #14
003a9234: andeq    r3, r0, r8, lsr sp
003a9238: andeq    r0, r0, r4, lsr #27
003a923c: andeq    r3, r0, r8, lsr #19
003a9240: subseq   sl, r1, r0, asr r6
003a9244: andeq    r1, r0, r0, ror sp

# _ZNK17PlayerStatManager6InvokeE9EStatTypeii
003796c4: ldr      ip, [pc, #0x148]
003796c8: ldr      r0, [pc, #0x148]
003796cc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003796d0: add      ip, pc, ip
003796d4: ldr      r5, [ip, r0]
003796d8: ldr      r0, [pc, #0x13c]
003796dc: sub      sp, sp, #0x64
003796e0: ldr      lr, [r5]
003796e4: ldr      r0, [ip, r0]
003796e8: mov      r6, r1
003796ec: mov      r1, r2
003796f0: ldr      r0, [r0, #0x40]
003796f4: mov      r2, #0
003796f8: str      lr, [sp, #0x5c]
003796fc: mov      fp, r3
00379700: bl       #0x36dfb0 ; _ZN13PlayerManager21GetPlayerByInternalIDEib
00379704: mov      r4, r0
00379708: bl       #0x42ca8c ; _ZN11MenuManager11GetInstanceEv
0037970c: bl       #0x42cb8c ; _ZN11MenuManager10GetHUDRootEv
00379710: mov      r7, #0
00379714: mov      r8, r0
00379718: mov      sl, #2
0037971c: ldr      r0, [r4, #0x67c]
00379720: strb     r7, [sp, #8]
00379724: strb     sl, [sp, #9]
00379728: bl       #0x30ed30 ; 
0037972c: strd     r0, r1, [sp, #0x38]
00379730: ldr      r3, [sp, #0x38]
00379734: mov      r0, r4
00379738: add      r4, sp, #8
0037973c: str      r3, [sp, #0xc]
00379740: ldr      r3, [sp, #0x3c]
00379744: add      sb, sp, #0x44
00379748: str      r3, [r4, #8]
0037974c: bl       #0x80f164 ; _ZN14CNetPlayerInfo8IsRemoteEv
00379750: mov      r3, #1
00379754: mov      r1, r6
00379758: strb     r0, [sp, #0x18]
0037975c: add      r6, r4, #0x18
00379760: mov      r0, sb
00379764: strb     r3, [sp, #0x15]
00379768: strb     r7, [sp, #0x14]
0037976c: bl       #0x3795c8 ; _ZN17PlayerStatManager10GetStatStrE9EStatType
00379770: ldr      r1, [sp, #0x58]
00379774: mov      r0, r6
00379778: strb     r7, [sp, #0x20]
0037977c: strb     r7, [sp, #0x21]
00379780: bl       #0x797350 ; _ZN7gameswf8as_value10set_stringEPKc
00379784: mov      r0, sb
00379788: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
0037978c: mov      r0, fp
00379790: strb     r7, [sp, #0x2c]
00379794: strb     sl, [sp, #0x2d]
00379798: bl       #0x30ed30 ; 
0037979c: mov      r2, r0
003797a0: mov      r3, r1
003797a4: mov      r0, r8
003797a8: strd     r2, r3, [sp, #0x30]
003797ac: strd     r2, r3, [sp, #0x38]
003797b0: bl       #0x7a7cac ; _ZNK8RenderFX7GetRootEv
003797b4: bl       #0x774154 ; _ZN7gameswf4root14get_root_movieEv
003797b8: ldr      r3, [pc, #0x60]
003797bc: mov      r1, r0
003797c0: mov      ip, #4
003797c4: ldr      r2, [pc, r3]
003797c8: mov      r0, r8
003797cc: mov      r3, r4
003797d0: str      ip, [sp]
003797d4: bl       #0x7abe0c ; _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi
003797d8: add      r0, r4, #0x24
003797dc: bl       #0x797124 ; _ZN7gameswf8as_value9drop_refsEv
003797e0: mov      r0, r6
003797e4: bl       #0x797124 ; _ZN7gameswf8as_value9drop_refsEv
003797e8: add      r0, r4, #0xc
003797ec: bl       #0x797124 ; _ZN7gameswf8as_value9drop_refsEv
003797f0: mov      r0, r4
003797f4: bl       #0x797124 ; _ZN7gameswf8as_value9drop_refsEv
003797f8: ldr      r2, [sp, #0x5c]
003797fc: ldr      r3, [r5]
00379800: cmp      r2, r3
00379804: bne      #0x379810
00379808: add      sp, sp, #0x64
0037980c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00379810: bl       #0x30e310 ; 
00379814: rsbeq    fp, r1, r0, asr #7
00379818: andeq    r4, r0, ip, lsr #1
0037981c: strdeq   r3, r4, [r0], -r4
00379820: subseq   sp, sp, r4, ror #3

# _ZSt11__push_heapIPSt4pairIiiEiS1_N17PlayerStatManager9_StatCompEEvT_T0_S6_T1_T2_
0037915c: cmp      r1, r2
00379160: push     {r4, r5, r6, r7}
00379164: addle    r5, r0, r1, lsl #3
00379168: ble      #0x379194
0037916c: sub      ip, r1, #1
00379170: add      ip, ip, ip, lsr #31
00379174: asr      ip, ip, #1
00379178: ldr      r4, [r0, ip, lsl #3]
0037917c: ldr      r6, [r3]
00379180: add      r7, r0, r1, lsl #3
00379184: add      r5, r0, ip, lsl #3
00379188: cmp      r4, r6
0037918c: bgt      #0x3791ac
00379190: mov      r5, r7
00379194: ldr      r2, [r3]
00379198: str      r2, [r5]
0037919c: ldr      r3, [r3, #4]
003791a0: str      r3, [r5, #4]
003791a4: pop      {r4, r5, r6, r7}
003791a8: bx       lr
003791ac: str      r4, [r0, r1, lsl #3]
003791b0: ldr      r4, [r5, #4]
003791b4: cmp      r2, ip
003791b8: sub      r1, ip, #1
003791bc: str      r4, [r7, #4]
003791c0: bge      #0x379194
003791c4: add      r4, r1, r1, lsr #31
003791c8: mov      r1, ip
003791cc: asr      ip, r4, #1
003791d0: b        #0x379178

# _ZN9Character8_RegenHPERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b77ec: str      lr, [sp, #-4]!
003b77f0: ldr      r3, [r0, #4]
003b77f4: sub      sp, sp, #0xc
003b77f8: ldr      r1, [r3, #4]
003b77fc: ldr      ip, [r3]
003b7800: rsb      r3, ip, r1
003b7804: asr      r3, r3, #4
003b7808: add      r1, r3, r3, lsl #3
003b780c: add      r1, r1, r1, lsl #6
003b7810: add      r1, r3, r1, lsl #3
003b7814: add      r1, r1, r1, lsl #15
003b7818: add      r3, r3, r1, lsl #3
003b781c: cmp      r3, #0
003b7820: bne      #0x3b782c
003b7824: add      sp, sp, #0xc
003b7828: ldm      sp!, {pc}
003b782c: ldr      r3, [ip, #4]
003b7830: cmp      r3, #3
003b7834: bne      #0x3b7824
003b7838: mov      r1, #0
003b783c: str      r2, [sp, #4]
003b7840: bl       #0x37baf8 ; _ZNK3sfc6script3lua9ArgumentsixEj
003b7844: bl       #0x31bbf0 ; _ZNK3sfc6script3lua5Value9getNumberEv
003b7848: bl       #0x30e4cc ; 
003b784c: ldr      r2, [sp, #4]
003b7850: mov      r1, r0
003b7854: mov      r0, r2
003b7858: add      sp, sp, #0xc
003b785c: pop      {lr}
003b7860: b        #0x3bdca4

# _ZN9Character13F_ApplyResultERKNS_12AttackResultEPS_S3_b
003b10b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b10b8: ldr      r7, [pc, #0xcb4]
003b10bc: ldr      sb, [pc, #0xcb4]
003b10c0: mov      r4, r0
003b10c4: add      r7, pc, r7
003b10c8: ldr      r0, [r7, sb]
003b10cc: mov      r5, r2
003b10d0: sub      sp, sp, #0x15c
003b10d4: ldr      r2, [r0]
003b10d8: mov      r8, r3
003b10dc: mov      r6, r1
003b10e0: str      r2, [sp, #0x154]
003b10e4: bl       #0x7fd794 ; _Z9GetOnlinev
003b10e8: ldrb     r3, [r0, #5]
003b10ec: cmp      r3, #0
003b10f0: bne      #0x3b1440
003b10f4: ldrb     r3, [r4, #0x18]
003b10f8: ldr      fp, [pc, #0xc7c]
003b10fc: add      r8, sp, #0x13c
003b1100: tst      r3, #3
003b1104: movw     r3, #0x14d0
003b1108: ldrheq   r2, [r6, r3]
003b110c: ldr      sl, [r7, fp]
003b1110: movne    r2, #0
003b1114: addeq    r2, r2, #1
003b1118: strh     r2, [r6, r3]
003b111c: mov      r0, sl
003b1120: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b1124: ldr      r1, [pc, #0xc54]
003b1128: add      r2, sp, #0x48
003b112c: mov      r0, r8
003b1130: add      r1, pc, r1
003b1134: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b1138: mov      r0, sl
003b113c: mov      r1, r8
003b1140: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1144: cmp      r0, #0
003b1148: beq      #0x3b14cc
003b114c: mov      r0, r8
003b1150: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1154: ldr      r1, [r4, #0x10]
003b1158: mov      r0, r6
003b115c: bl       #0x3bdca4 ; _ZN9Character7RegenHPEi
003b1160: mov      r0, r6
003b1164: ldr      r1, [r4, #0x14]
003b1168: bl       #0x3bdbb8 ; _ZN9Character7RegenMPEi
003b116c: ldr      r3, [r5]
003b1170: mov      r0, r5
003b1174: mov      lr, pc
003b1178: ldr      pc, [r3, #0x34]
003b117c: cmp      r0, #0
003b1180: beq      #0x3b1204
003b1184: mov      r0, r5
003b1188: bl       #0x3bc6b8 ; _ZN9Character14CancelSneakingEv
003b118c: mov      r0, r4
003b1190: mov      r1, r6
003b1194: mov      r2, r5
003b1198: bl       #0x3af77c ; _ZN9Character26F_ApplyScrollingCombatTextERKNS_12AttackResultEPS_S3_
003b119c: mov      r0, r4
003b11a0: mov      r1, r6
003b11a4: mov      r2, r5
003b11a8: bl       #0x3afee0 ; _ZN9Character18F_ApplyCombatSoundERKNS_12AttackResultEPS_S3_
003b11ac: ldr      r3, [r4, #0x1c]
003b11b0: tst      r3, #0x20000000
003b11b4: beq      #0x3b1788
003b11b8: ldr      r3, [r5]
003b11bc: mov      r0, r5
003b11c0: mov      lr, pc
003b11c4: ldr      pc, [r3, #0x28]
003b11c8: cmp      r0, #0
003b11cc: bne      #0x3b1758
003b11d0: ldr      r3, [r6]
003b11d4: mov      r0, r6
003b11d8: mov      lr, pc
003b11dc: ldr      pc, [r3, #0x28]
003b11e0: cmp      r0, #0
003b11e4: bne      #0x3b13e0
003b11e8: ldr      r3, [r7, sb]
003b11ec: ldr      r2, [sp, #0x154]
003b11f0: ldr      r3, [r3]
003b11f4: cmp      r2, r3
003b11f8: bne      #0x3b1d70
003b11fc: add      sp, sp, #0x15c
003b1200: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b1204: ldr      r1, [r4, #8]
003b1208: cmp      r1, #0
003b120c: ble      #0x3b122c
003b1210: ldr      r2, [r4, #0xc]
003b1214: cmp      r2, #0
003b1218: ble      #0x3b122c
003b121c: asr      r1, r1, #8
003b1220: add      r0, r5, #0x560
003b1224: ldr      r3, [r4, #4]
003b1228: bl       #0x3e2720 ; _ZN14CharProperties12PROPS_AddDotEiii
003b122c: ldr      r2, [r4, #0x1c]
003b1230: ldr      r3, [r5]
003b1234: mov      r0, r5
003b1238: tst      r2, #0x18000000
003b123c: moveq    ip, #0
003b1240: movne    ip, #1
003b1244: str      ip, [sp, #0xc]
003b1248: mov      lr, pc
003b124c: ldr      pc, [r3, #0x28]
003b1250: cmp      r0, #0
003b1254: bne      #0x3b17ec
003b1258: ldrb     r3, [r4, #0x18]
003b125c: tst      r3, #2
003b1260: bne      #0x3b1834
003b1264: tst      r3, #4
003b1268: bne      #0x3b18a4
003b126c: tst      r3, #0x10
003b1270: bne      #0x3b1914
003b1274: tst      r3, #0x80
003b1278: bne      #0x3b196c
003b127c: tst      r3, #0x40
003b1280: beq      #0x3b1304
003b1284: ldr      r3, [r4, #0x1c]
003b1288: add      r1, r6, #0xff0
003b128c: add      r1, r1, #4
003b1290: tst      r3, #0x1000
003b1294: movne    r2, #0xb9
003b1298: moveq    r2, #0x8c
003b129c: add      r0, r6, #0x560
003b12a0: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003b12a4: mov      r1, r0
003b12a8: add      r0, r5, #0x4f0
003b12ac: mov      r3, r6
003b12b0: mov      ip, #0
003b12b4: asr      r1, r1, #8
003b12b8: mov      r2, #1
003b12bc: add      r0, r0, #0xc
003b12c0: str      ip, [sp]
003b12c4: bl       #0x3c5ffc ; _ZN16CharStateMachine15SM_SetStunStateEjbPvb
003b12c8: ldr      sl, [r7, fp]
003b12cc: add      r8, sp, #0x7c
003b12d0: mov      r0, sl
003b12d4: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b12d8: ldr      r1, [pc, #0xaa4]
003b12dc: add      r2, sp, #0x28
003b12e0: mov      r0, r8
003b12e4: add      r1, pc, r1
003b12e8: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b12ec: mov      r1, r8
003b12f0: mov      r0, sl
003b12f4: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b12f8: mov      r0, r8
003b12fc: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1300: ldrb     r3, [r4, #0x18]
003b1304: tst      r3, #0x20
003b1308: beq      #0x3b136c
003b130c: ldr      r3, [r4, #0x1c]
003b1310: add      r1, r6, #0xff0
003b1314: add      r1, r1, #4
003b1318: tst      r3, #0x4000
003b131c: movne    r2, #0xbb
003b1320: moveq    r2, #0x8f
003b1324: add      r0, r6, #0x560
003b1328: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003b132c: asrs     r1, r0, #8
003b1330: bne      #0x3b19e0
003b1334: ldr      sl, [r7, fp]
003b1338: add      r8, sp, #0x64
003b133c: mov      r0, sl
003b1340: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b1344: ldr      r1, [pc, #0xa3c]
003b1348: add      r2, sp, #0x24
003b134c: mov      r0, r8
003b1350: add      r1, pc, r1
003b1354: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b1358: mov      r0, sl
003b135c: mov      r1, r8
003b1360: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1364: mov      r0, r8
003b1368: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b136c: ldrb     r3, [r4, #0x19]
003b1370: tst      r3, #1
003b1374: beq      #0x3b1184
003b1378: ldr      r3, [r4, #0x1c]
003b137c: add      r1, r6, #0xff0
003b1380: add      r1, r1, #4
003b1384: tst      r3, #0x10000
003b1388: movne    r2, #0xbd
003b138c: moveq    r2, #0x92
003b1390: add      r0, r6, #0x560
003b1394: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003b1398: asr      r1, r0, #8
003b139c: add      r0, r5, #0x560
003b13a0: bl       #0x3e2a5c ; _ZN14CharProperties16PROPS_DebuffSlowEj
003b13a4: ldr      sl, [r7, fp]
003b13a8: add      r8, sp, #0x4c
003b13ac: mov      r0, sl
003b13b0: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b13b4: ldr      r1, [pc, #0x9d0]
003b13b8: add      r2, sp, #0x20
003b13bc: mov      r0, r8
003b13c0: add      r1, pc, r1
003b13c4: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b13c8: mov      r0, sl
003b13cc: mov      r1, r8
003b13d0: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b13d4: mov      r0, r8
003b13d8: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b13dc: b        #0x3b1184
003b13e0: ldr      r3, [pc, #0x9a8]
003b13e4: mov      r1, r6
003b13e8: mov      r2, #0
003b13ec: ldr      r3, [r7, r3]
003b13f0: ldr      r0, [r3, #0x40]
003b13f4: bl       #0x36eea8 ; _ZN13PlayerManager20GetPlayerByCharacterEPK9Characterb
003b13f8: ldrh     r3, [r4, #0x18]
003b13fc: ldr      r6, [r0, #0x670]
003b1400: tst      r3, #0x160
003b1404: bne      #0x3b1a4c
003b1408: ldr      r8, [pc, #0x984]
003b140c: mov      r0, r5
003b1410: ldr      r3, [r5]
003b1414: mov      lr, pc
003b1418: ldr      pc, [r3, #0x34]
003b141c: cmp      r0, #0
003b1420: bne      #0x3b1a38
003b1424: ldr      r2, [r4]
003b1428: ldr      r0, [r7, r8]
003b142c: mov      r3, r6
003b1430: asr      r2, r2, #8
003b1434: mov      r1, #1
003b1438: bl       #0x3790e0 ; _ZN17PlayerStatManager12IncreaseStatE9EStatTypeii
003b143c: b        #0x3b11e8
003b1440: ldr      r3, [r6]
003b1444: mov      r0, r6
003b1448: mov      lr, pc
003b144c: ldr      pc, [r3, #0x54]
003b1450: cmp      r0, #0
003b1454: bne      #0x3b17c4
003b1458: cmp      r8, #0
003b145c: bne      #0x3b10f4
003b1460: cmp      r5, #0
003b1464: beq      #0x3b1aa8
003b1468: ldr      sl, [r5, #0x108]
003b146c: ldr      r8, [r6, #0x108]
003b1470: lsr      r3, sl, #0x1f
003b1474: orrs     r3, r3, r8, lsr #31
003b1478: beq      #0x3b14a0
003b147c: ldr      r3, [pc, #0x914]
003b1480: ldr      r3, [r7, r3]
003b1484: ldr      r3, [r3]
003b1488: cmp      r3, #2
003b148c: moveq    r3, #0
003b1490: streq    r3, [r3]
003b1494: beq      #0x3b14a0
003b1498: cmp      r3, #1
003b149c: beq      #0x3b1d08
003b14a0: bl       #0x80b1bc ; _ZN10CMessaging3GetEv
003b14a4: mov      r1, sl
003b14a8: mov      fp, r0
003b14ac: mov      r2, r4
003b14b0: mov      r0, r8
003b14b4: mov      r3, #1
003b14b8: bl       #0x3af330 ; _ZN16CMsgAttackResult6CreateEiiRKN9Character12AttackResultEb
003b14bc: mov      r1, r0
003b14c0: mov      r0, fp
003b14c4: bl       #0x80e2a4 ; _ZN10CMessaging7SendMsgEP8CMessage
003b14c8: b        #0x3b10f4
003b14cc: ldr      r3, [pc, #0x8c8]
003b14d0: add      ip, sp, #0x124
003b14d4: mov      r0, sl
003b14d8: add      r3, pc, r3
003b14dc: str      ip, [sp, #0xc]
003b14e0: str      r3, [sp, #8]
003b14e4: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b14e8: ldr      r3, [sp, #8]
003b14ec: add      r2, sp, #0x44
003b14f0: ldr      r0, [sp, #0xc]
003b14f4: mov      r1, r3
003b14f8: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b14fc: mov      r0, sl
003b1500: ldr      r1, [sp, #0xc]
003b1504: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1508: cmp      r0, #0
003b150c: ldr      r3, [sp, #8]
003b1510: beq      #0x3b17d0
003b1514: ldr      r3, [r5]
003b1518: mov      r0, r5
003b151c: mov      lr, pc
003b1520: ldr      pc, [r3, #0x28]
003b1524: cmp      r0, #0
003b1528: bne      #0x3b1a2c
003b152c: movw     r3, #0x14f0
003b1530: ldrb     r3, [r5, r3]
003b1534: cmp      r3, #0
003b1538: bne      #0x3b1a2c
003b153c: ldr      r0, [sp, #0xc]
003b1540: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1544: mov      r0, r8
003b1548: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b154c: ldr      r3, [r4, #0x1c]
003b1550: tst      r3, #0x400000
003b1554: bne      #0x3b1a00
003b1558: ldr      r8, [r4]
003b155c: cmp      r8, #0
003b1560: ble      #0x3b1154
003b1564: ldr      r2, [pc, #0x824]
003b1568: ldr      r3, [r7, r2]
003b156c: str      r2, [sp, #0xc]
003b1570: ldr      r3, [r3, #0x40]
003b1574: ldr      sl, [r3, #0x6c4]
003b1578: cmp      sl, #1
003b157c: ble      #0x3b15c4
003b1580: mov      r0, r6
003b1584: bl       #0x3a3064 ; _ZNK9Character9IsMonsterEv
003b1588: cmp      r0, #0
003b158c: beq      #0x3b1ad8
003b1590: sub      r0, sl, #1
003b1594: bl       #0x30e964 ; 
003b1598: ldr      r3, [pc, #0x800]
003b159c: ldr      r3, [r7, r3]
003b15a0: ldr      r3, [r3]
003b15a4: ldr      r1, [r3, #0x34]
003b15a8: bl       #0x30ed6c ; 
003b15ac: mov      r1, #0x3f800000
003b15b0: bl       #0x30eba4 ; 
003b15b4: bl       #0x30e4cc ; 
003b15b8: ldr      r8, [r4]
003b15bc: mul      r8, r8, r0
003b15c0: str      r8, [r4]
003b15c4: mov      r0, r6
003b15c8: bl       #0x3bd394 ; _ZNK9Character27GetEffectiveThreatPerDamageEv
003b15cc: mov      sl, r0
003b15d0: ldr      r0, [r4]
003b15d4: bl       #0x30e964 ; 
003b15d8: mov      r1, #0x3b800000
003b15dc: bl       #0x30ed6c ; 
003b15e0: mov      r1, r0
003b15e4: mov      r0, sl
003b15e8: bl       #0x30ed6c ; 
003b15ec: add      sl, r5, #0x3c8
003b15f0: mov      r2, r0
003b15f4: mov      r1, r6
003b15f8: mov      r0, sl
003b15fc: bl       #0x3d7c68 ; _ZN6CharAI11AI_AddAggroEP9Characterf
003b1600: mov      r1, #0
003b1604: bl       #0x30e2f8 ; 
003b1608: cmp      r0, #0
003b160c: bne      #0x3b1a64
003b1610: ldrb     r3, [r4, #0x18]
003b1614: ldr      r2, [r5, #0x110]
003b1618: and      r3, r3, #0x80
003b161c: uxtb     r3, r3
003b1620: cmp      r3, #0
003b1624: ldrne    r3, [r4, #0x1c]
003b1628: ubfxne   r3, r3, #0x14, #1
003b162c: cmn      r2, #1
003b1630: strb     r3, [r5, #0x53b]
003b1634: beq      #0x3b1b2c
003b1638: ldr      r3, [r4, #0x1c]
003b163c: tst      r3, #0x200000
003b1640: beq      #0x3b1680
003b1644: ldr      r8, [r4, #0x24]
003b1648: cmn      r8, #1
003b164c: addne    r8, r8, #0x7c
003b1650: beq      #0x3b1cd0
003b1654: mov      r0, r5
003b1658: bl       #0x3935dc ; _ZNK10GameObject17GetTargetPositionEv
003b165c: ldr      r3, [pc, #0x740]
003b1660: mov      ip, #0
003b1664: mov      r2, r0
003b1668: mov      r1, r8
003b166c: ldr      r0, [r7, r3]
003b1670: add      r3, r5, #0x16c
003b1674: str      ip, [sp, #4]
003b1678: str      ip, [sp]
003b167c: bl       #0x495888 ; _ZN15VisualFXManager13PlayAnimFXSetEiRK7Point3DIfES3_PK10GameObjectPNS_13AnimFXSetDataE
003b1680: ldr      r3, [r5]
003b1684: mov      r0, r5
003b1688: mov      lr, pc
003b168c: ldr      pc, [r3, #0x34]
003b1690: cmp      r0, #0
003b1694: beq      #0x3b16b4
003b1698: ldrb     r3, [r4, #0x18]
003b169c: ldrb     r2, [r4, #0x19]
003b16a0: and      r3, r3, #0xbf
003b16a4: bfc      r2, #0, #1
003b16a8: bfc      r3, #5, #1
003b16ac: strb     r2, [r4, #0x19]
003b16b0: strb     r3, [r4, #0x18]
003b16b4: ldrb     r3, [r4, #0x18]
003b16b8: tst      r3, #8
003b16bc: beq      #0x3b1154
003b16c0: ldr      r3, [r6]
003b16c4: mov      r0, r6
003b16c8: mov      lr, pc
003b16cc: ldr      pc, [r3, #0x28]
003b16d0: cmp      r0, #0
003b16d4: beq      #0x3b1154
003b16d8: ldr      r3, [sp, #0xc]
003b16dc: ldr      r0, [r7, r3]
003b16e0: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
003b16e4: cmp      r0, #0
003b16e8: beq      #0x3b1154
003b16ec: ldr      r8, [r0, #0x128]
003b16f0: cmp      r8, #0
003b16f4: beq      #0x3b1154
003b16f8: mov      r0, r8
003b16fc: mov      r1, r6
003b1700: bl       #0x40f980 ; _ZNK11CameraLevel16CanPlayShakeAnimEP9Character
003b1704: cmp      r0, #0
003b1708: beq      #0x3b1154
003b170c: mov      r3, #0
003b1710: mov      r0, r6
003b1714: add      r1, sp, #0x14
003b1718: str      r3, [sp, #0x1c]
003b171c: str      r3, [sp, #0x14]
003b1720: str      r3, [sp, #0x18]
003b1724: bl       #0x393ae4 ; _ZNK10GameObject12GetLookAtVecER7Point3DIfE
003b1728: ldr      r3, [pc, #0x678]
003b172c: ldr      r1, [r8, #0x80]
003b1730: mov      ip, #0x1c
003b1734: ldr      r3, [r7, r3]
003b1738: mov      r0, r8
003b173c: mov      r2, #0
003b1740: ldr      lr, [r3]
003b1744: mov      r3, #1
003b1748: mla      r1, ip, r1, lr
003b174c: ldr      r1, [r1, #0xc]
003b1750: bl       #0x40f904 ; _ZN11CameraLevel8PlayAnimEiib
003b1754: b        #0x3b1154
003b1758: ldr      r3, [pc, #0x630]
003b175c: mov      r1, r5
003b1760: mov      r2, #0
003b1764: ldr      r3, [r7, r3]
003b1768: ldr      r8, [pc, #0x624]
003b176c: ldr      r0, [r3, #0x40]
003b1770: bl       #0x36eea8 ; _ZN13PlayerManager20GetPlayerByCharacterEPK9Characterb
003b1774: mov      r1, #3
003b1778: ldr      r2, [r0, #0x670]
003b177c: ldr      r0, [r7, r8]
003b1780: bl       #0x3790ec ; _ZN17PlayerStatManager13IncrementStatE9EStatTypei
003b1784: b        #0x3b11d0
003b1788: add      r0, r6, #0x3c8
003b178c: mov      r1, r6
003b1790: mov      r2, r5
003b1794: mov      r3, r4
003b1798: ldr      ip, [r6, #0x3c8]
003b179c: mov      lr, pc
003b17a0: ldr      pc, [ip, #0xb4]
003b17a4: ldr      ip, [r5, #0x3c8]
003b17a8: add      r0, r5, #0x3c8
003b17ac: mov      r1, r6
003b17b0: mov      r2, r5
003b17b4: mov      r3, r4
003b17b8: mov      lr, pc
003b17bc: ldr      pc, [ip, #0xb4]
003b17c0: b        #0x3b11b8
003b17c4: cmp      r8, #0
003b17c8: beq      #0x3b11e8
003b17cc: b        #0x3b10f4
003b17d0: mov      r1, r3
003b17d4: ldr      r3, [pc, #0x5b4]
003b17d8: ldr      r0, [r7, r3]
003b17dc: bl       #0x320e14 ; _ZN11Application15IsSavedOptionOnEPKc
003b17e0: cmp      r0, #0
003b17e4: beq      #0x3b152c
003b17e8: b        #0x3b1514
003b17ec: add      r0, r5, #0x4f0
003b17f0: add      r0, r0, #0xc
003b17f4: mov      r1, #0
003b17f8: bl       #0x3c0260 ; _ZNK16CharStateMachine9SM_IsIdleEb
003b17fc: cmp      r0, #0
003b1800: beq      #0x3b1258
003b1804: ldrb     r3, [r4, #0x18]
003b1808: tst      r3, #0x16
003b180c: bne      #0x3b125c
003b1810: ldr      r2, [r4]
003b1814: cmp      r2, #0
003b1818: orrle    r3, r3, #2
003b181c: orrgt    r3, r3, #0x10
003b1820: strble   r3, [r4, #0x18]
003b1824: uxtble   r3, r3
003b1828: strbgt   r3, [r4, #0x18]
003b182c: tst      r3, #2
003b1830: beq      #0x3b1264
003b1834: add      r0, r5, #0x4f0
003b1838: add      r0, r0, #0xc
003b183c: mov      r1, r6
003b1840: mov      r2, #0
003b1844: bl       #0x3c5b3c ; _ZN16CharStateMachine18SM_SetDodgingStateEPvb
003b1848: ldr      r3, [r5]
003b184c: mov      r0, r5
003b1850: mov      lr, pc
003b1854: ldr      pc, [r3, #0x28]
003b1858: cmp      r0, #0
003b185c: bne      #0x3b1bf0
003b1860: ldr      sl, [r7, fp]
003b1864: add      r8, sp, #0xdc
003b1868: mov      r0, sl
003b186c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b1870: ldr      r1, [pc, #0x534]
003b1874: add      r2, sp, #0x38
003b1878: mov      r0, r8
003b187c: add      r1, pc, r1
003b1880: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b1884: mov      r1, r8
003b1888: mov      r0, sl
003b188c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1890: mov      r0, r8
003b1894: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1898: ldrb     r3, [r4, #0x18]
003b189c: tst      r3, #4
003b18a0: beq      #0x3b126c
003b18a4: add      r0, r5, #0x4f0
003b18a8: add      r0, r0, #0xc
003b18ac: mov      r1, r6
003b18b0: mov      r2, #0
003b18b4: bl       #0x3c5c60 ; _ZN16CharStateMachine19SM_SetBlockingStateEPvb
003b18b8: ldr      r3, [r5]
003b18bc: mov      r0, r5
003b18c0: mov      lr, pc
003b18c4: ldr      pc, [r3, #0x28]
003b18c8: cmp      r0, #0
003b18cc: bne      #0x3b1b80
003b18d0: ldr      sl, [r7, fp]
003b18d4: add      r8, sp, #0xc4
003b18d8: mov      r0, sl
003b18dc: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b18e0: ldr      r1, [pc, #0x4c8]
003b18e4: add      r2, sp, #0x34
003b18e8: mov      r0, r8
003b18ec: add      r1, pc, r1
003b18f0: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b18f4: mov      r1, r8
003b18f8: mov      r0, sl
003b18fc: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1900: mov      r0, r8
003b1904: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1908: ldrb     r3, [r4, #0x18]
003b190c: tst      r3, #0x10
003b1910: beq      #0x3b1274
003b1914: add      r0, r5, #0x4f0
003b1918: mov      r1, r6
003b191c: ldr      r2, [sp, #0xc]
003b1920: add      r0, r0, #0xc
003b1924: bl       #0x3c5d84 ; _ZN16CharStateMachine17SM_SetInjureStateEPvb
003b1928: ldr      sl, [r7, fp]
003b192c: add      r8, sp, #0xac
003b1930: mov      r0, sl
003b1934: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b1938: ldr      r1, [pc, #0x474]
003b193c: add      r2, sp, #0x30
003b1940: mov      r0, r8
003b1944: add      r1, pc, r1
003b1948: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b194c: mov      r1, r8
003b1950: mov      r0, sl
003b1954: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1958: mov      r0, r8
003b195c: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1960: ldrb     r3, [r4, #0x18]
003b1964: tst      r3, #0x80
003b1968: beq      #0x3b127c
003b196c: ldr      r1, [r4, #0x1c]
003b1970: add      r0, r5, #0x4f0
003b1974: add      r0, r0, #0xc
003b1978: ubfx     r1, r1, #0x14, #1
003b197c: mov      r2, r6
003b1980: ldr      r3, [sp, #0xc]
003b1984: bl       #0x3c5ea0 ; _ZN16CharStateMachine20SM_SetKnockBackStateEbPvb
003b1988: ldr      r3, [r5]
003b198c: mov      r0, r5
003b1990: mov      lr, pc
003b1994: ldr      pc, [r3, #0x28]
003b1998: cmp      r0, #0
003b199c: bne      #0x3b1c60
003b19a0: ldr      sl, [r7, fp]
003b19a4: add      r8, sp, #0x94
003b19a8: mov      r0, sl
003b19ac: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b19b0: ldr      r1, [pc, #0x400]
003b19b4: add      r2, sp, #0x2c
003b19b8: mov      r0, r8
003b19bc: add      r1, pc, r1
003b19c0: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b19c4: mov      r1, r8
003b19c8: mov      r0, sl
003b19cc: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b19d0: mov      r0, r8
003b19d4: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b19d8: ldrb     r3, [r4, #0x18]
003b19dc: b        #0x3b127c
003b19e0: ldr      ip, [sp, #0xc]
003b19e4: add      r0, r5, #0x4f0
003b19e8: add      r0, r0, #0xc
003b19ec: mov      r2, #1
003b19f0: mov      r3, r6
003b19f4: str      ip, [sp]
003b19f8: bl       #0x3c6144 ; _ZN16CharStateMachine16SM_SetScareStateEjbPvb
003b19fc: b        #0x3b1334
003b1a00: ldr      r3, [r6, #0x39c]
003b1a04: ldr      r2, [r4]
003b1a08: add      r0, r6, #0x37c
003b1a0c: lsl      r3, r3, #8
003b1a10: cmp      r3, r2
003b1a14: movge    r3, r2
003b1a18: asr      r1, r3, #8
003b1a1c: str      r3, [r4]
003b1a20: rsb      r1, r1, #0
003b1a24: bl       #0x3fe164 ; _ZN13ItemInventory7AddGoldEi
003b1a28: b        #0x3b1558
003b1a2c: ldr      r0, [sp, #0xc]
003b1a30: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1a34: b        #0x3b114c
003b1a38: ldr      r0, [r7, r8]
003b1a3c: mov      r1, #0
003b1a40: mov      r2, r6
003b1a44: bl       #0x3790ec ; _ZN17PlayerStatManager13IncrementStatE9EStatTypei
003b1a48: b        #0x3b1424
003b1a4c: ldr      r8, [pc, #0x340]
003b1a50: mov      r1, #2
003b1a54: mov      r2, r6
003b1a58: ldr      r0, [r7, r8]
003b1a5c: bl       #0x3790ec ; _ZN17PlayerStatManager13IncrementStatE9EStatTypei
003b1a60: b        #0x3b140c
003b1a64: ldr      r3, [r7, fp]
003b1a68: add      sl, sp, #0x10c
003b1a6c: mov      r0, r3
003b1a70: str      r3, [sp, #8]
003b1a74: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b1a78: ldr      r1, [pc, #0x33c]
003b1a7c: add      r2, sp, #0x40
003b1a80: mov      r0, sl
003b1a84: add      r1, pc, r1
003b1a88: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b1a8c: ldr      r3, [sp, #8]
003b1a90: mov      r1, sl
003b1a94: mov      r0, r3
003b1a98: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1a9c: mov      r0, sl
003b1aa0: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1aa4: b        #0x3b1610
003b1aa8: ldr      r3, [pc, #0x2e8]
003b1aac: ldr      r3, [r7, r3]
003b1ab0: ldr      r3, [r3]
003b1ab4: cmp      r3, #2
003b1ab8: streq    r5, [r5]
003b1abc: beq      #0x3b1ac8
003b1ac0: cmp      r3, #1
003b1ac4: beq      #0x3b1d3c
003b1ac8: ldr      r8, [r6, #0x108]
003b1acc: mov      r3, #1
003b1ad0: mvn      sl, #0
003b1ad4: b        #0x3b1474
003b1ad8: ldr      r3, [r6]
003b1adc: mov      r0, r6
003b1ae0: mov      lr, pc
003b1ae4: ldr      pc, [r3, #0x28]
003b1ae8: cmp      r0, #0
003b1aec: beq      #0x3b15c4
003b1af0: sub      r0, sl, #1
003b1af4: bl       #0x30e964 ; 
003b1af8: ldr      r3, [pc, #0x2a0]
003b1afc: ldr      r3, [r7, r3]
003b1b00: ldr      r3, [r3]
003b1b04: ldr      r1, [r3, #0x38]
003b1b08: bl       #0x30ed6c ; 
003b1b0c: mov      r1, #0x3f800000
003b1b10: bl       #0x30eba4 ; 
003b1b14: bl       #0x30e4cc ; 
003b1b18: mov      r1, r0
003b1b1c: mov      r0, r8
003b1b20: bl       #0x30e2a4 ; 
003b1b24: mov      r8, r0
003b1b28: b        #0x3b15c4
003b1b2c: ldr      r3, [r7, fp]
003b1b30: add      sl, sp, #0xf4
003b1b34: mov      r0, r3
003b1b38: str      r3, [sp, #8]
003b1b3c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b1b40: ldr      r1, [pc, #0x278]
003b1b44: add      r2, sp, #0x3c
003b1b48: mov      r0, sl
003b1b4c: add      r1, pc, r1
003b1b50: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b1b54: ldr      r3, [sp, #8]
003b1b58: mov      r1, sl
003b1b5c: mov      r0, r3
003b1b60: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b1b64: mov      r0, sl
003b1b68: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b1b6c: mov      r0, r5
003b1b70: mov      r1, r8
003b1b74: mov      r2, r6
003b1b78: bl       #0x3a8bc4 ; _ZN9Character6HitForEjP10GameObject
003b1b7c: b        #0x3b1638
003b1b80: add      r8, r5, #0x560
003b1b84: mov      r0, r8
003b1b88: mov      r1, #0xd6
003b1b8c: mov      r2, #1
003b1b90: bl       #0x3e0798 ; _ZN14CharProperties12PROPS_AddIntEii
003b1b94: ldr      r3, [pc, #0x228]
003b1b98: mov      r0, r8
003b1b9c: mov      r1, #0xd6
003b1ba0: ldr      r3, [r7, r3]
003b1ba4: mov      r2, #0
003b1ba8: ldr      r8, [r3]
003b1bac: bl       #0x3df6e0 ; _ZNK14CharProperties12PROPS_GetIntEib
003b1bb0: cmp      r0, #0x1f4
003b1bb4: blt      #0x3b18d0
003b1bb8: ldr      r3, [pc, #0x1d0]
003b1bbc: mov      r1, r5
003b1bc0: ldr      r3, [r7, r3]
003b1bc4: ldr      r0, [r3, #0x40]
003b1bc8: bl       #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b1bcc: cmp      r0, #0
003b1bd0: beq      #0x3b18d0
003b1bd4: ldr      r0, [pc, #0x1ec]
003b1bd8: add      r0, pc, r0
003b1bdc: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003b1be0: mov      r1, r0
003b1be4: mov      r0, r8
003b1be8: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003b1bec: b        #0x3b18d0
003b1bf0: add      r8, r5, #0x560
003b1bf4: mov      r0, r8
003b1bf8: mov      r1, #0xd7
003b1bfc: mov      r2, #1
003b1c00: bl       #0x3e0798 ; _ZN14CharProperties12PROPS_AddIntEii
003b1c04: ldr      r3, [pc, #0x1b8]
003b1c08: mov      r0, r8
003b1c0c: mov      r1, #0xd7
003b1c10: ldr      r3, [r7, r3]
003b1c14: mov      r2, #0
003b1c18: ldr      r8, [r3]
003b1c1c: bl       #0x3df6e0 ; _ZNK14CharProperties12PROPS_GetIntEib
003b1c20: cmp      r0, #0x1f4
003b1c24: blt      #0x3b1860
003b1c28: ldr      r3, [pc, #0x160]
003b1c2c: mov      r1, r5
003b1c30: ldr      r3, [r7, r3]
003b1c34: ldr      r0, [r3, #0x40]
003b1c38: bl       #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b1c3c: cmp      r0, #0
003b1c40: beq      #0x3b1860
003b1c44: ldr      r0, [pc, #0x180]
003b1c48: add      r0, pc, r0
003b1c4c: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003b1c50: mov      r1, r0
003b1c54: mov      r0, r8
003b1c58: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003b1c5c: b        #0x3b1860
003b1c60: add      r8, r5, #0x560
003b1c64: mov      r0, r8
003b1c68: mov      r1, #0xde
003b1c6c: mov      r2, #1
003b1c70: bl       #0x3e0798 ; _ZN14CharProperties12PROPS_AddIntEii
003b1c74: ldr      r3, [pc, #0x148]
003b1c78: mov      r0, r8
003b1c7c: mov      r1, #0xde
003b1c80: ldr      r3, [r7, r3]
003b1c84: mov      r2, #0
003b1c88: ldr      r8, [r3]
003b1c8c: bl       #0x3df6e0 ; _ZNK14CharProperties12PROPS_GetIntEib
003b1c90: cmp      r0, #0x31
003b1c94: ble      #0x3b19a0
003b1c98: ldr      r3, [pc, #0xf0]
003b1c9c: mov      r1, r5
003b1ca0: ldr      r3, [r7, r3]
003b1ca4: ldr      r0, [r3, #0x40]
003b1ca8: bl       #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
003b1cac: cmp      r0, #0
003b1cb0: beq      #0x3b19a0
003b1cb4: ldr      r0, [pc, #0x114]
003b1cb8: add      r0, pc, r0
003b1cbc: bl       #0x3a3f70 ; _ZN6Arrays19GetMemberIDByStringINS_11TrophyTableEEEiPKc
003b1cc0: mov      r1, r0
003b1cc4: mov      r0, r8
003b1cc8: bl       #0x3813b8 ; _ZN13TrophyManager12UnlockTrophyEi
003b1ccc: b        #0x3b19a0
003b1cd0: ldr      r3, [r5]
003b1cd4: mov      r0, r5
003b1cd8: mov      lr, pc
003b1cdc: ldr      pc, [r3, #0x34]
003b1ce0: cmp      r0, #0
003b1ce4: beq      #0x3b1cf8
003b1ce8: mov      r0, r5
003b1cec: bl       #0x3a3368 ; _ZNK9Character15GetFXBloodDeathEv
003b1cf0: mov      r8, r0
003b1cf4: b        #0x3b1654
003b1cf8: mov      r0, r5
003b1cfc: bl       #0x3a33d0 ; _ZNK9Character10GetFXBloodEv
003b1d00: mov      r8, r0
003b1d04: b        #0x3b1654
003b1d08: ldr      r0, [pc, #0xc4]
003b1d0c: ldr      r1, [pc, #0xc4]
003b1d10: ldr      r2, [pc, #0xc4]
003b1d14: ldr      r0, [r7, r0]
003b1d18: ldr      r3, [pc, #0xc0]
003b1d1c: movw     ip, #0x2d2
003b1d20: add      r1, pc, r1
003b1d24: add      r2, pc, r2
003b1d28: add      r3, pc, r3
003b1d2c: add      r0, r0, #0xa8
003b1d30: str      ip, [sp]
003b1d34: bl       #0x30e004 ; 
003b1d38: b        #0x3b14a0
003b1d3c: ldr      r0, [pc, #0x90]
003b1d40: ldr      r1, [pc, #0x9c]
003b1d44: ldr      r2, [pc, #0x9c]
003b1d48: ldr      r0, [r7, r0]
003b1d4c: ldr      r3, [pc, #0x98]
003b1d50: mov      ip, #0x2c8
003b1d54: add      r1, pc, r1
003b1d58: add      r2, pc, r2
003b1d5c: add      r3, pc, r3
003b1d60: add      r0, r0, #0xa8
003b1d64: str      ip, [sp]
003b1d68: bl       #0x30e004 ; 
003b1d6c: b        #0x3b1ac8
003b1d70: bl       #0x30e310 ; 
003b1d74: subseq   r3, lr, ip, asr #19
003b1d78: andeq    r4, r0, ip, lsr #1
003b1d7c: andeq    r0, r0, r4, lsl #17
003b1d80: subseq   r2, r1, r0, ror fp
003b1d84: ldrsbeq  r2, [r1], #-0x94
003b1d88: subseq   r2, r1, r8, ror #18
003b1d8c: ldrsheq  r2, [r1], #-0x88
003b1d90: strdeq   r3, r4, [r0], -r4
003b1d94: andeq    r2, r0, r4, lsl r7
003b1d98: andeq    r3, r0, r0, asr #19
003b1d9c: ldrsbeq  r2, [r1], #-0x78
003b1da0: andeq    r3, r0, r8, asr #5
003b1da4: andeq    r1, r0, r8, lsl #22
003b1da8: ldrdeq   r3, r4, [r0], -r4
003b1dac: subseq   r2, r1, ip, lsr r4
003b1db0: subseq   r2, r1, ip, asr #7
003b1db4: subseq   r2, r1, r4, ror r3
003b1db8: ldrsheq  r2, [r1], #-0x2c
003b1dbc: subseq   r2, r1, ip, asr r2
003b1dc0: subseq   r2, r1, ip, ror #2
003b1dc4: andeq    r1, r0, r0, ror sp
003b1dc8: subseq   r2, r1, r0, lsr r1
003b1dcc: ldrheq   r2, [r1], #-0
003b1dd0: subseq   r2, r1, r0, rrx
003b1dd4: andeq    r1, r0, r0, asr #19
003b1dd8: ldrheq   ip, [r0], #-0x68
003b1ddc: subseq   r1, r1, r4, asr #30
003b1de0: subseq   r1, r1, r8, ror #29
003b1de4: subseq   ip, r0, r4, lsl #13
003b1de8: subseq   r1, r1, r0, lsr #29
003b1dec: ldrheq   r1, [r1], #-0xe4

# _ZN9AISPlayer9OnDeAggroEP9Character
003dde48: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dde4c: ldr      r4, [pc, #0x264]
003dde50: ldr      r6, [pc, #0x264]
003dde54: ldr      r7, [pc, #0x264]
003dde58: add      r4, pc, r4
003dde5c: ldr      r3, [r4, r6]
003dde60: sub      sp, sp, #0x4c
003dde64: mov      sb, r1
003dde68: ldr      r3, [r3]
003dde6c: mov      r5, r0
003dde70: add      r8, sp, #0x2c
003dde74: str      r3, [sp, #0x44]
003dde78: bl       #0x3dbea8 ; _ZN10AISDefault9OnDeAggroEP9Character
003dde7c: ldr      sl, [r4, r7]
003dde80: mov      r0, sl
003dde84: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003dde88: ldr      r1, [pc, #0x234]
003dde8c: add      r2, sp, #0x10
003dde90: mov      r0, r8
003dde94: add      r1, pc, r1
003dde98: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003dde9c: mov      r0, sl
003ddea0: mov      r1, r8
003ddea4: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003ddea8: mov      sl, r0
003ddeac: ldr      r0, [sp, #0x40]
003ddeb0: cmp      r0, r8
003ddeb4: beq      #0x3dded4
003ddeb8: cmp      r0, #0
003ddebc: beq      #0x3dded4
003ddec0: ldr      r1, [sp, #0x2c]
003ddec4: rsb      r1, r0, r1
003ddec8: cmp      r1, #0x80
003ddecc: bhi      #0x3de054
003dded0: bl       #0x708f00 ; 
003dded4: cmp      sl, #0
003dded8: bne      #0x3de030
003ddedc: ldr      r3, [r5, #0xd0]
003ddee0: sub      r3, r3, #1
003ddee4: str      r3, [r5, #0xd0]
003ddee8: bl       #0x7fd794 ; _Z9GetOnlinev
003ddeec: ldrb     r3, [r0, #5]
003ddef0: cmp      r3, #0
003ddef4: bne      #0x3de010
003ddef8: ldr      r8, [pc, #0x1c8]
003ddefc: ldr      r3, [pc, #0x1c8]
003ddf00: ldr      fp, [r4, r8]
003ddf04: ldr      r3, [r4, r3]
003ddf08: mov      r0, fp
003ddf0c: ldr      r8, [r3]
003ddf10: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
003ddf14: mov      r0, sb
003ddf18: ldr      sl, [r5, #0xd4]
003ddf1c: bl       #0x3a3024 ; _ZNK9Character9GetCharAIEv
003ddf20: ldr      r3, [r0, #0x14]
003ddf24: rsb      sl, r3, sl
003ddf28: str      sl, [r5, #0xd4]
003ddf2c: ldrb     r3, [r8, #0x31]
003ddf30: cmp      r3, #0
003ddf34: bne      #0x3ddf64
003ddf38: cmp      sl, #0
003ddf3c: bne      #0x3ddf64
003ddf40: ldr      r1, [pc, #0x188]
003ddf44: mov      r0, r8
003ddf48: mov      sb, #1
003ddf4c: add      r1, pc, r1
003ddf50: bl       #0x369514 ; _ZN15VoxSoundManager13SetMusicStateEPKc
003ddf54: ldrb     r3, [r8, #0x32]
003ddf58: strb     sb, [r8, #0x31]
003ddf5c: cmp      r3, #0
003ddf60: bne      #0x3de084
003ddf64: ldr      r3, [r5, #0xd0]
003ddf68: cmp      r3, #0
003ddf6c: beq      #0x3de05c
003ddf70: ldr      r8, [r4, r7]
003ddf74: add      r7, sp, #0x14
003ddf78: mov      r0, r8
003ddf7c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003ddf80: ldr      r1, [pc, #0x14c]
003ddf84: add      r2, sp, #0xc
003ddf88: mov      r0, r7
003ddf8c: add      r1, pc, r1
003ddf90: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003ddf94: mov      r0, r8
003ddf98: mov      r1, r7
003ddf9c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003ddfa0: mov      r8, r0
003ddfa4: ldr      r0, [sp, #0x28]
003ddfa8: cmp      r0, r7
003ddfac: beq      #0x3ddfcc
003ddfb0: cmp      r0, #0
003ddfb4: beq      #0x3ddfcc
003ddfb8: ldr      r1, [sp, #0x14]
003ddfbc: rsb      r1, r0, r1
003ddfc0: cmp      r1, #0x80
003ddfc4: bhi      #0x3de07c
003ddfc8: bl       #0x708f00 ; 
003ddfcc: cmp      r8, #0
003ddfd0: beq      #0x3ddff4
003ddfd4: ldr      r0, [pc, #0xfc]
003ddfd8: ldr      r1, [pc, #0xfc]
003ddfdc: ldr      r3, [r5, #0xd4]
003ddfe0: ldr      r0, [r4, r0]
003ddfe4: add      r1, pc, r1
003ddfe8: ldr      r2, [r5, #0xd0]
003ddfec: add      r0, r0, #0xa8
003ddff0: bl       #0x30e004 ; 
003ddff4: ldr      r3, [r4, r6]
003ddff8: ldr      r2, [sp, #0x44]
003ddffc: ldr      r3, [r3]
003de000: cmp      r2, r3
003de004: bne      #0x3de0b4
003de008: add      sp, sp, #0x4c
003de00c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003de010: ldr      r8, [pc, #0xb0]
003de014: ldr      r1, [r5, #0x98]
003de018: ldr      r3, [r4, r8]
003de01c: ldr      r0, [r3, #0x40]
003de020: bl       #0x36effc ; _ZN13PlayerManager13IsLocalPlayerEPK9Character
003de024: cmp      r0, #0
003de028: beq      #0x3ddff4
003de02c: b        #0x3ddefc
003de030: ldr      r0, [pc, #0xa0]
003de034: ldr      r1, [pc, #0xa4]
003de038: ldr      r2, [r5, #0xd0]
003de03c: ldr      r0, [r4, r0]
003de040: add      r1, pc, r1
003de044: ldr      r3, [r5, #0xd4]
003de048: add      r0, r0, #0xa8
003de04c: bl       #0x30e004 ; 
003de050: b        #0x3ddedc
003de054: bl       #0x310440 ; _Z10CustomFreePv
003de058: b        #0x3dded4
003de05c: ldrb     r3, [r8, #0x32]
003de060: cmp      r3, #0
003de064: bne      #0x3ddf70
003de068: ldr      r3, [r5, #0xc4]
003de06c: ldr      r2, [r5, #0xc8]
003de070: cmp      r3, r2
003de074: strne    r3, [r5, #0xc8]
003de078: b        #0x3ddf70
003de07c: bl       #0x310440 ; _Z10CustomFreePv
003de080: b        #0x3ddfcc
003de084: mov      r0, fp
003de088: bl       #0x31f594 ; _ZNK11Application15GetCurrentLevelEv
003de08c: ldr      r1, [r0, #0x120]
003de090: cmp      r1, #0
003de094: blt      #0x3ddf64
003de098: mov      ip, #0x7d0
003de09c: mov      r2, sb
003de0a0: mov      r3, sl
003de0a4: mov      r0, r8
003de0a8: str      ip, [sp]
003de0ac: bl       #0x36bd78 ; _ZN15VoxSoundManager9PlayMusicEibbi
003de0b0: b        #0x3ddf64
003de0b4: bl       #0x30e310 ; 
003de0b8: subseq   r6, fp, r8, lsr ip
003de0bc: andeq    r4, r0, ip, lsr #1
003de0c0: andeq    r0, r0, r4, lsl #17
003de0c4: subeq    r7, lr, r4, lsl sp
003de0c8: strdeq   r3, r4, [r0], -r4
003de0cc: andeq    r0, r0, r4, lsr #27
003de0d0: ldrdeq   r3, r4, [lr], #-0xc4
003de0d4: subeq    r7, lr, ip, lsl ip
003de0d8: andeq    r1, r0, r0, asr #19
003de0dc: umaaleq  r7, lr, r4, ip
003de0e0: subeq    r7, lr, r0, lsl #24

# _ZN17PlayerStatManager12SetStatValueE9EStatTypeiib
00379104: bx       lr

# _ZN17PlayerStatManager12DecreaseStatE9EStatTypeii
003790e4: rsb      r2, r2, #0
003790e8: b        #0x3790e0

# _ZN10AISDefault7OnAggroEP9Character
003dbea4: bx       lr

# _Z20PushProfilingContextPKc
003136b4: bx       lr

# _ZN9Character14CancelSneakingEv
003bc6b8: push     {r4, r5, r6, lr}
003bc6bc: ldr      r3, [r0]
003bc6c0: mov      r5, r0
003bc6c4: mov      lr, pc
003bc6c8: ldr      pc, [r3, #0x28]
003bc6cc: ldr      r4, [pc, #0xa8]
003bc6d0: cmp      r0, #0
003bc6d4: add      r4, pc, r4
003bc6d8: bne      #0x3bc760
003bc6dc: mov      r0, r5
003bc6e0: bl       #0x3bc690 ; _ZNK9Character10IsSneakingEv
003bc6e4: cmp      r0, #0
003bc6e8: bne      #0x3bc6f0
003bc6ec: pop      {r4, r5, r6, pc}
003bc6f0: mov      r0, r5
003bc6f4: bl       #0x3bc5fc ; _ZNK9Character16GetCharSkillListEv
003bc6f8: ldr      r2, [r0, #4]
003bc6fc: cmp      r2, #0
003bc700: beq      #0x3bc6ec
003bc704: ldr      r1, [pc, #0x74]
003bc708: ldr      r0, [r0, #8]
003bc70c: mov      r3, #0x4c
003bc710: ldr      ip, [r4, r1]
003bc714: ldr      r1, [r0]
003bc718: ldr      ip, [ip]
003bc71c: mla      r1, r3, r1, ip
003bc720: ldr      r1, [r1, #0x1c]
003bc724: ands     r1, r1, #0x2000000
003bc728: movne    r1, #0
003bc72c: bne      #0x3bc754
003bc730: mov      r4, r3
003bc734: add      r1, r1, #1
003bc738: cmp      r1, r2
003bc73c: beq      #0x3bc6ec
003bc740: ldr      r3, [r0, r1, lsl #2]
003bc744: mla      r3, r4, r3, ip
003bc748: ldr      r3, [r3, #0x1c]
003bc74c: tst      r3, #0x2000000
003bc750: beq      #0x3bc734
003bc754: add      r0, r5, #0x3c8
003bc758: pop      {r4, r5, r6, lr}
003bc75c: b        #0x3d84e0
003bc760: add      r0, r5, #0x560
003bc764: mov      r1, #0x92
003bc768: mov      r2, #0
003bc76c: bl       #0x3e101c ; _ZN14CharProperties13PROPS_DelBuffEiPN7Structs19CharacterPropertiesE
003bc770: mov      r3, #1
003bc774: strb     r3, [r5, #0x415]
003bc778: b        #0x3bc6dc
003bc77c: ldrheq   r8, [sp], #-0x3c
003bc780: andeq    r4, r0, ip, lsl r4

# _ZNK17PlayerStatManager13UpdateRankingE9EStatType
00379d18: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00379d1c: ldr      sl, [pc, #0xc4]
00379d20: ldr      sb, [pc, #0xc4]
00379d24: sub      sp, sp, #0x20
00379d28: add      sl, pc, sl
00379d2c: ldr      r3, [sl, sb]
00379d30: mov      r2, #1
00379d34: mov      r7, r1
00379d38: ldr      r3, [r3]
00379d3c: mov      r6, r0
00379d40: add      r4, sp, #4
00379d44: str      r3, [sp, #0x1c]
00379d48: bl       #0x379ac4 ; _ZNK17PlayerStatManager10GetRankingE9EStatTypeb
00379d4c: ldr      r3, [pc, #0x9c]
00379d50: mov      r5, r0
00379d54: ldr      r8, [sl, r3]
00379d58: mov      r0, r8
00379d5c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
00379d60: ldr      r1, [pc, #0x8c]
00379d64: mov      r2, sp
00379d68: mov      r0, r4
00379d6c: add      r1, pc, r1
00379d70: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00379d74: mov      r1, r4
00379d78: mov      r0, r8
00379d7c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
00379d80: mov      r0, r4
00379d84: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
00379d88: ldr      r3, [pc, #0x68]
00379d8c: ldr      r3, [sl, r3]
00379d90: ldr      r0, [r3, #0x40]
00379d94: bl       #0x36d7a8 ; _ZN13PlayerManager13GetNumPlayersEv
00379d98: subs     r8, r0, #0
00379d9c: ble      #0x379dc8
00379da0: mov      r4, #0
00379da4: add      r3, r5, r4, lsl #3
00379da8: ldr      r2, [r3, #4]
00379dac: mov      r0, r6
00379db0: mov      r3, r4
00379db4: mov      r1, r7
00379db8: add      r4, r4, #1
00379dbc: bl       #0x3796c4 ; _ZNK17PlayerStatManager6InvokeE9EStatTypeii
00379dc0: cmp      r4, r8
00379dc4: bne      #0x379da4
00379dc8: ldr      r3, [sl, sb]
00379dcc: ldr      r2, [sp, #0x1c]
00379dd0: ldr      r3, [r3]
00379dd4: cmp      r2, r3
00379dd8: bne      #0x379de4
00379ddc: add      sp, sp, #0x20
00379de0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00379de4: bl       #0x30e310 ; 
00379de8: rsbeq    sl, r1, r8, ror #26
00379dec: andeq    r4, r0, ip, lsr #1
00379df0: andeq    r0, r0, r4, lsl #17
00379df4: subseq   r7, r4, r4, asr #24
00379df8: strdeq   r3, r4, [r0], -r4

# _ZN9Character11F_DotAttackERNS_12AttackResultEPS_S2_ii
003b2e68: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b2e6c: ldr      r4, [pc, #0x160]
003b2e70: ldr      r6, [pc, #0x160]
003b2e74: subs     r7, r1, #0
003b2e78: add      r4, pc, r4
003b2e7c: ldr      r1, [r4, r6]
003b2e80: mov      r5, r2
003b2e84: sub      sp, sp, #0x34
003b2e88: ldr      r2, [r1]
003b2e8c: mov      sb, r0
003b2e90: mov      fp, r3
003b2e94: str      r2, [sp, #0x2c]
003b2e98: beq      #0x3b2f28
003b2e9c: cmp      r5, #0
003b2ea0: beq      #0x3b2f7c
003b2ea4: ldr      r3, [pc, #0x130]
003b2ea8: add      r8, sp, #0x14
003b2eac: ldr      sl, [r4, r3]
003b2eb0: mov      r0, sl
003b2eb4: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b2eb8: ldr      r1, [pc, #0x120]
003b2ebc: add      r2, sp, #0x10
003b2ec0: mov      r0, r8
003b2ec4: add      r1, pc, r1
003b2ec8: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b2ecc: mov      r1, r8
003b2ed0: mov      r0, sl
003b2ed4: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b2ed8: mov      r0, r8
003b2edc: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b2ee0: mvn      ip, #0
003b2ee4: str      ip, [sp]
003b2ee8: ldr      ip, [sp, #0x58]
003b2eec: mov      r3, #0x20000000
003b2ef0: mov      r2, r5
003b2ef4: add      r3, r3, #0x80000
003b2ef8: mov      r0, sb
003b2efc: mov      r1, r7
003b2f00: str      ip, [sp, #4]
003b2f04: str      fp, [sp, #8]
003b2f08: bl       #0x3b2638 ; _ZN9Character18_F_CalculateResultERNS_12AttackResultEPS_S2_iiii
003b2f0c: ldr      r3, [r4, r6]
003b2f10: ldr      r2, [sp, #0x2c]
003b2f14: ldr      r3, [r3]
003b2f18: cmp      r2, r3
003b2f1c: bne      #0x3b2fd0
003b2f20: add      sp, sp, #0x34
003b2f24: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b2f28: ldr      r3, [pc, #0xb4]
003b2f2c: ldr      r3, [r4, r3]
003b2f30: ldr      r3, [r3]
003b2f34: cmp      r3, #2
003b2f38: streq    r7, [r7]
003b2f3c: beq      #0x3b2e9c
003b2f40: cmp      r3, #1
003b2f44: bne      #0x3b2e9c
003b2f48: ldr      r0, [pc, #0x98]
003b2f4c: ldr      r1, [pc, #0x98]
003b2f50: ldr      r2, [pc, #0x98]
003b2f54: ldr      r0, [r4, r0]
003b2f58: ldr      r3, [pc, #0x94]
003b2f5c: movw     ip, #0x289
003b2f60: add      r1, pc, r1
003b2f64: add      r2, pc, r2
003b2f68: add      r3, pc, r3
003b2f6c: add      r0, r0, #0xa8
003b2f70: str      ip, [sp]
003b2f74: bl       #0x30e004 ; 
003b2f78: b        #0x3b2e9c
003b2f7c: ldr      r3, [pc, #0x60]
003b2f80: ldr      r3, [r4, r3]
003b2f84: ldr      r3, [r3]
003b2f88: cmp      r3, #2
003b2f8c: streq    r5, [r5]
003b2f90: beq      #0x3b2ea4
003b2f94: cmp      r3, #1
003b2f98: bne      #0x3b2ea4
003b2f9c: ldr      r0, [pc, #0x44]
003b2fa0: ldr      r1, [pc, #0x50]
003b2fa4: ldr      r2, [pc, #0x50]
003b2fa8: ldr      r0, [r4, r0]
003b2fac: ldr      r3, [pc, #0x4c]
003b2fb0: movw     ip, #0x28a
003b2fb4: add      r1, pc, r1
003b2fb8: add      r2, pc, r2
003b2fbc: add      r3, pc, r3
003b2fc0: add      r0, r0, #0xa8
003b2fc4: str      ip, [sp]
003b2fc8: bl       #0x30e004 ; 
003b2fcc: b        #0x3b2ea4
003b2fd0: bl       #0x30e310 ; 
003b2fd4: subseq   r1, lr, r8, lsl ip
003b2fd8: andeq    r4, r0, ip, lsr #1
003b2fdc: andeq    r0, r0, r4, lsl #17
003b2fe0: ldrsheq  r0, [r1], #-0xd4
003b2fe4: andeq    r3, r0, r0, asr #19
003b2fe8: andeq    r1, r0, r0, asr #19
003b2fec: subseq   fp, r0, r8, ror r4
003b2ff0: subseq   r0, r1, ip, ror #26
003b2ff4: subseq   r0, r1, r8, lsr #25
003b2ff8: subseq   fp, r0, r4, lsr #8
003b2ffc: subseq   lr, r0, r8, asr #18
003b3000: subseq   r0, r1, r4, asr ip

# _ZN10AISDefault9OnDeAggroEP9Character
003dbea8: bx       lr

# _ZN17PlayerStatManagerD1Ev
003790d0: bx       lr

# _ZNK17PlayerStatManager9GetLeaderE9EStatType
00379dfc: push     {r4, lr}
00379e00: mov      r2, #0
00379e04: bl       #0x379ac4 ; _ZNK17PlayerStatManager10GetRankingE9EStatTypeb
00379e08: ldr      r0, [r0, #4]
00379e0c: pop      {r4, pc}

# _ZN17PlayerStatManager12IncreaseStatE9EStatTypeii
003790e0: bx       lr

# _ZNK17PlayerStatManager6InvokeERKSs
00379568: push     {r4, r5, lr}
0037956c: sub      sp, sp, #0xc
00379570: mov      r5, r1
00379574: bl       #0x42ca8c ; _ZN11MenuManager11GetInstanceEv
00379578: bl       #0x42cb8c ; _ZN11MenuManager10GetHUDRootEv
0037957c: mov      r4, r0
00379580: bl       #0x7a7cac ; _ZNK8RenderFX7GetRootEv
00379584: bl       #0x774154 ; _ZN7gameswf4root14get_root_movieEv
00379588: mov      ip, #0
0037958c: mov      r1, r0
00379590: ldr      r2, [r5, #0x14]
00379594: mov      r0, r4
00379598: mov      r3, ip
0037959c: str      ip, [sp]
003795a0: bl       #0x7abe0c ; _ZN8RenderFX16InvokeASCallbackEPN7gameswf9characterEPKcPKNS0_8as_valueEi
003795a4: add      sp, sp, #0xc
003795a8: pop      {r4, r5, pc}

# _ZNK6CharAI16AI_GetAggroCountEv
003d4a10: ldr      r0, [r0, #0x8c]
003d4a14: bx       lr

# _ZN9Character13F_SkillAttackERNS_12AttackResultEPS_S2_ii
003b31a4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b31a8: ldr      r4, [pc, #0x188]
003b31ac: ldr      sb, [pc, #0x188]
003b31b0: subs     r6, r1, #0
003b31b4: add      r4, pc, r4
003b31b8: ldr      r1, [r4, sb]
003b31bc: mov      r5, r2
003b31c0: sub      sp, sp, #0x34
003b31c4: ldr      r2, [r1]
003b31c8: mov      fp, r0
003b31cc: mov      r8, r3
003b31d0: str      r2, [sp, #0x2c]
003b31d4: beq      #0x3b328c
003b31d8: cmp      r5, #0
003b31dc: beq      #0x3b32e0
003b31e0: ldr      r3, [pc, #0x158]
003b31e4: add      r7, sp, #0x14
003b31e8: orr      r8, r8, #0x8000000
003b31ec: ldr      sl, [r4, r3]
003b31f0: mov      r0, sl
003b31f4: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b31f8: ldr      r1, [pc, #0x144]
003b31fc: add      r2, sp, #0x10
003b3200: mov      r0, r7
003b3204: add      r1, pc, r1
003b3208: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b320c: mov      r1, r7
003b3210: mov      r0, sl
003b3214: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b3218: mov      r0, r7
003b321c: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b3220: tst      r8, #0x4000000
003b3224: movne    r1, #2
003b3228: moveq    r1, #1
003b322c: add      r0, r6, #0x37c
003b3230: bl       #0x3ffe3c ; _ZN13ItemInventory15GetEquippedItemEj
003b3234: cmp      r0, #0
003b3238: mvneq    ip, #0
003b323c: beq      #0x3b3248
003b3240: bl       #0x3f9e08 ; _ZNK12ItemInstance7GetItemEv
003b3244: ldr      ip, [r0, #0x94]
003b3248: str      ip, [sp]
003b324c: ldr      ip, [sp, #0x58]
003b3250: mov      r2, r5
003b3254: mov      r3, r8
003b3258: str      ip, [sp, #4]
003b325c: mov      r0, fp
003b3260: mov      ip, #0
003b3264: mov      r1, r6
003b3268: str      ip, [sp, #8]
003b326c: bl       #0x3b2638 ; _ZN9Character18_F_CalculateResultERNS_12AttackResultEPS_S2_iiii
003b3270: ldr      r3, [r4, sb]
003b3274: ldr      r2, [sp, #0x2c]
003b3278: ldr      r3, [r3]
003b327c: cmp      r2, r3
003b3280: bne      #0x3b3334
003b3284: add      sp, sp, #0x34
003b3288: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b328c: ldr      r3, [pc, #0xb4]
003b3290: ldr      r3, [r4, r3]
003b3294: ldr      r3, [r3]
003b3298: cmp      r3, #2
003b329c: streq    r6, [r6]
003b32a0: beq      #0x3b31d8
003b32a4: cmp      r3, #1
003b32a8: bne      #0x3b31d8
003b32ac: ldr      r0, [pc, #0x98]
003b32b0: ldr      r1, [pc, #0x98]
003b32b4: ldr      r2, [pc, #0x98]
003b32b8: ldr      r0, [r4, r0]
003b32bc: ldr      r3, [pc, #0x94]
003b32c0: movw     ip, #0x265
003b32c4: add      r1, pc, r1
003b32c8: add      r2, pc, r2
003b32cc: add      r3, pc, r3
003b32d0: add      r0, r0, #0xa8
003b32d4: str      ip, [sp]
003b32d8: bl       #0x30e004 ; 
003b32dc: b        #0x3b31d8
003b32e0: ldr      r3, [pc, #0x60]
003b32e4: ldr      r3, [r4, r3]
003b32e8: ldr      r3, [r3]
003b32ec: cmp      r3, #2
003b32f0: streq    r5, [r5]
003b32f4: beq      #0x3b31e0
003b32f8: cmp      r3, #1
003b32fc: bne      #0x3b31e0
003b3300: ldr      r0, [pc, #0x44]
003b3304: ldr      r1, [pc, #0x50]
003b3308: ldr      r2, [pc, #0x50]
003b330c: ldr      r0, [r4, r0]
003b3310: ldr      r3, [pc, #0x4c]
003b3314: movw     ip, #0x266
003b3318: add      r1, pc, r1
003b331c: add      r2, pc, r2
003b3320: add      r3, pc, r3
003b3324: add      r0, r0, #0xa8
003b3328: str      ip, [sp]
003b332c: bl       #0x30e004 ; 
003b3330: b        #0x3b31e0
003b3334: bl       #0x30e310 ; 
003b3338: ldrsbeq  r1, [lr], #-0x8c
003b333c: andeq    r4, r0, ip, lsr #1
003b3340: andeq    r0, r0, r4, lsl #17
003b3344: ldrheq   r0, [r1], #-0xa4
003b3348: andeq    r3, r0, r0, asr #19
003b334c: andeq    r1, r0, r0, asr #19
003b3350: subseq   fp, r0, r4, lsl r1
003b3354: subseq   r0, r1, r8, lsl #20
003b3358: subseq   r0, r1, r4, asr #18
003b335c: subseq   fp, r0, r0, asr #1
003b3360: subseq   lr, r0, r4, ror #11
003b3364: ldrsheq  r0, [r1], #-0x80

# _ZN9Character7RegenMPEi
003bdbb8: push     {r4, r5, r6, r7, r8, sl, lr}
003bdbbc: ldr      r5, [pc, #0xd0]
003bdbc0: ldr      r8, [pc, #0xd0]
003bdbc4: add      r6, r0, #0xff0
003bdbc8: add      r5, pc, r5
003bdbcc: ldr      r3, [r5, r8]
003bdbd0: add      r6, r6, #4
003bdbd4: add      r7, r0, #0x560
003bdbd8: ldr      r3, [r3]
003bdbdc: mov      r4, r1
003bdbe0: sub      sp, sp, #0x24
003bdbe4: mov      r2, #0x29
003bdbe8: mov      r1, r6
003bdbec: mov      r0, r7
003bdbf0: str      r3, [sp, #0x1c]
003bdbf4: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003bdbf8: mov      sl, r0
003bdbfc: mov      r1, r6
003bdc00: mov      r0, r7
003bdc04: mov      r2, #0x2b
003bdc08: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003bdc0c: cmp      r4, #0
003bdc10: movlt    r4, r0
003bdc14: add      r3, r4, sl
003bdc18: cmp      r3, r0
003bdc1c: rsbgt    r4, sl, r0
003bdc20: cmp      r4, #0
003bdc24: ble      #0x3bdc74
003bdc28: ldr      r3, [pc, #0x6c]
003bdc2c: add      r6, sp, #4
003bdc30: ldr      sl, [r5, r3]
003bdc34: mov      r0, sl
003bdc38: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003bdc3c: ldr      r1, [pc, #0x5c]
003bdc40: mov      r2, sp
003bdc44: mov      r0, r6
003bdc48: add      r1, pc, r1
003bdc4c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003bdc50: mov      r1, r6
003bdc54: mov      r0, sl
003bdc58: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003bdc5c: mov      r0, r6
003bdc60: bl       #0x318254 ; _ZNSsD1Ev
003bdc64: mov      r0, r7
003bdc68: mov      r2, r4
003bdc6c: mov      r1, #0x29
003bdc70: bl       #0x3e0708 ; _ZN14CharProperties9PROPS_AddEii
003bdc74: ldr      r3, [r5, r8]
003bdc78: ldr      r2, [sp, #0x1c]
003bdc7c: ldr      r3, [r3]
003bdc80: cmp      r2, r3
003bdc84: bne      #0x3bdc90
003bdc88: add      sp, sp, #0x24
003bdc8c: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdc90: bl       #0x30e310 ; 
003bdc94: subseq   r6, sp, r8, asr #29
003bdc98: andeq    r4, r0, ip, lsr #1
003bdc9c: andeq    r0, r0, r4, lsl #17
003bdca0: subseq   r6, r0, r8, asr #24

# _ZN6CharAI15AI_ReloadSkillsEv
003d8cfc: push     {r4, r5, r6, r7, r8, lr}
003d8d00: ldr      r4, [r0, #0xb4]
003d8d04: ldr      r5, [r0, #0xb8]
003d8d08: mov      r6, r0
003d8d0c: cmp      r4, r5
003d8d10: beq      #0x3d8d54
003d8d14: mov      r7, #0
003d8d18: ldr      r3, [r4]
003d8d1c: cmp      r3, #0
003d8d20: beq      #0x3d8d38
003d8d24: mov      r0, r3
003d8d28: ldr      r3, [r3]
003d8d2c: mov      lr, pc
003d8d30: ldr      pc, [r3, #4]
003d8d34: str      r7, [r4]
003d8d38: add      r4, r4, #4
003d8d3c: cmp      r5, r4
003d8d40: bne      #0x3d8d18
003d8d44: ldr      r3, [r6, #0xb4]
003d8d48: ldr      r2, [r6, #0xb8]
003d8d4c: cmp      r3, r2
003d8d50: strne    r3, [r6, #0xb8]
003d8d54: ldr      r0, [r6, #4]
003d8d58: bl       #0x3bbe2c ; _ZN9Character15SG_ReloadSkillsEv
003d8d5c: mov      r0, r6
003d8d60: bl       #0x3ce044 ; _ZN6CharAI18SetSkillsAndSpellsEv
003d8d64: mov      r0, r6
003d8d68: pop      {r4, r5, r6, r7, r8, lr}
003d8d6c: b        #0x3d8894

# _ZN17PlayerStatManager14ResetStatValueE9EStatTypei
00379108: str      lr, [sp, #-4]!
0037910c: mov      ip, #0
00379110: sub      sp, sp, #0xc
00379114: mov      r3, r2
00379118: mov      r2, ip
0037911c: str      ip, [sp]
00379120: bl       #0x379104 ; _ZN17PlayerStatManager12SetStatValueE9EStatTypeiib
00379124: add      sp, sp, #0xc
00379128: ldm      sp!, {pc}

# _Z19PopProfilingContextPKc
003136b8: bx       lr

# _ZN17PlayerStatManagerD0Ev
003795ac: push     {r4, lr}
003795b0: mov      r4, r0
003795b4: bl       #0x3790d0 ; _ZN17PlayerStatManagerD1Ev
003795b8: mov      r0, r4
003795bc: bl       #0x310440 ; _Z10CustomFreePv
003795c0: mov      r0, r4
003795c4: pop      {r4, pc}

# _ZSt11__make_heapIPSt4pairIiiEN17PlayerStatManager9_StatCompES1_iEvT_S5_T0_PT1_PT2_
00379290: push     {r4, r5, r6, r7, r8, lr}
00379294: rsb      r1, r0, r1
00379298: cmp      r1, #0xf
0037929c: sub      sp, sp, #0x10
003792a0: mov      r6, r0
003792a4: ble      #0x3792fc
003792a8: asr      r7, r1, #3
003792ac: sub      r5, r7, #2
003792b0: asr      r5, r5, #1
003792b4: add      r8, sp, #8
003792b8: add      r4, r0, r5, lsl #3
003792bc: b        #0x3792c4
003792c0: sub      r5, r5, #1
003792c4: ldr      r3, [r4]
003792c8: mov      r1, r5
003792cc: mov      r0, r6
003792d0: str      r3, [sp, #8]
003792d4: ldr      ip, [r4, #4]
003792d8: mov      r2, r7
003792dc: mov      r3, r8
003792e0: str      ip, [sp, #0xc]
003792e4: mov      ip, #0
003792e8: strb     ip, [sp]
003792ec: bl       #0x3791d4 ; _ZSt13__adjust_heapIPSt4pairIiiEiS1_N17PlayerStatManager9_StatCompEEvT_T0_S6_T1_T2_
003792f0: cmp      r5, #0
003792f4: sub      r4, r4, #8
003792f8: bne      #0x3792c0
003792fc: add      sp, sp, #0x10
00379300: pop      {r4, r5, r6, r7, r8, pc}

# _ZN3sfc6script3lua9ArgumentsD1Ev
00319228: ldr      r3, [pc, #0x28]
0031922c: ldr      r2, [pc, #0x28]
00319230: push     {r4, lr}
00319234: add      r3, pc, r3
00319238: ldr      r2, [r3, r2]
0031923c: mov      r4, r0
00319240: ldr      r0, [r0, #4]
00319244: add      r2, r2, #8
00319248: str      r2, [r4]
0031924c: bl       #0x31d194 ; _ZN3sfc6script3lua5Value13freeValueListEPSt6vectorIS2_SaIS2_EE
00319250: mov      r0, r4
00319254: pop      {r4, pc}
00319258: rsbeq    fp, r7, ip, asr r8
0031925c: andeq    r2, r0, r0, ror #6

# _ZN17PlayerStatManager5ResetEv
003790d4: bx       lr

# _ZNK17PlayerStatManager12GetStatValueE9EStatTypei
00379824: push     {r4, r5, r6, lr}
00379828: ldr      r4, [pc, #0xf8]
0037982c: cmp      r1, #6
00379830: sub      sp, sp, #8
00379834: mov      r5, r1
00379838: add      r4, pc, r4
0037983c: mov      r6, r2
00379840: ble      #0x379868
00379844: ldr      r3, [pc, #0xe0]
00379848: ldr      r3, [r4, r3]
0037984c: ldr      r3, [r3]
00379850: cmp      r3, #2
00379854: moveq    r3, #0
00379858: streq    r3, [r3]
0037985c: beq      #0x379868
00379860: cmp      r3, #1
00379864: beq      #0x3798f4
00379868: cmp      r6, #3
0037986c: ble      #0x379894
00379870: ldr      r3, [pc, #0xb4]
00379874: ldr      r3, [r4, r3]
00379878: ldr      r3, [r3]
0037987c: cmp      r3, #2
00379880: moveq    r3, #0
00379884: streq    r3, [r3]
00379888: beq      #0x379894
0037988c: cmp      r3, #1
00379890: beq      #0x3798c0
00379894: ldr      r3, [pc, #0x94]
00379898: mov      r1, r6
0037989c: mov      r2, #0
003798a0: ldr      r3, [r4, r3]
003798a4: ldr      r0, [r3, #0x40]
003798a8: bl       #0x36dfb0 ; _ZN13PlayerManager21GetPlayerByInternalIDEib
003798ac: mov      r3, #0x28
003798b0: mla      r5, r3, r5, r0
003798b4: ldr      r0, [r5, #0x568]
003798b8: add      sp, sp, #8
003798bc: pop      {r4, r5, r6, pc}
003798c0: ldr      r0, [pc, #0x6c]
003798c4: ldr      r1, [pc, #0x6c]
003798c8: ldr      r2, [pc, #0x6c]
003798cc: ldr      r0, [r4, r0]
003798d0: ldr      r3, [pc, #0x68]
003798d4: mov      ip, #0xe0
003798d8: add      r1, pc, r1
003798dc: add      r2, pc, r2
003798e0: add      r3, pc, r3
003798e4: add      r0, r0, #0xa8
003798e8: str      ip, [sp]
003798ec: bl       #0x30e004 ; 
003798f0: b        #0x379894
003798f4: ldr      r0, [pc, #0x38]
003798f8: ldr      r1, [pc, #0x44]
003798fc: ldr      r2, [pc, #0x44]
00379900: ldr      r0, [r4, r0]
00379904: ldr      r3, [pc, #0x40]
00379908: mov      ip, #0xdf
0037990c: add      r1, pc, r1
00379910: add      r2, pc, r2
00379914: add      r3, pc, r3
00379918: add      r0, r0, #0xa8
0037991c: str      ip, [sp]
00379920: bl       #0x30e004 ; 
00379924: b        #0x379868
00379928: rsbeq    fp, r1, r8, asr r2
0037992c: andeq    r3, r0, r0, asr #19
00379930: strdeq   r3, r4, [r0], -r4
00379934: andeq    r1, r0, r0, asr #19
00379938: subseq   r4, r4, r0, lsl #22
0037993c: ldrheq   r8, [r4], #-0xc
00379940: subseq   r8, r4, r0, rrx
00379944: subseq   r4, r4, ip, asr #21
00379948: subseq   r8, r4, r8, lsl r0
0037994c: subseq   r8, r4, ip, lsr #32

# _ZN6CharAI11AI_SetAggroEP9Characterf
003d79ec: push     {r4, r5, r6, r7, lr}
003d79f0: ldr      r3, [pc, #0x258]
003d79f4: subs     r4, r1, #0
003d79f8: sub      sp, sp, #0x2c
003d79fc: mov      r6, r0
003d7a00: add      r3, pc, r3
003d7a04: mov      r5, r2
003d7a08: beq      #0x3d7bd4
003d7a0c: ldr      r3, [r6, #4]
003d7a10: mov      r0, r3
003d7a14: ldr      r3, [r3]
003d7a18: mov      lr, pc
003d7a1c: ldr      pc, [r3, #0x28]
003d7a20: cmp      r0, #0
003d7a24: beq      #0x3d7a38
003d7a28: mov      r5, #0
003d7a2c: mov      r0, r5
003d7a30: add      sp, sp, #0x2c
003d7a34: pop      {r4, r5, r6, r7, pc}
003d7a38: ldr      r3, [r6, #4]
003d7a3c: mov      r0, r3
003d7a40: ldr      r3, [r3]
003d7a44: mov      lr, pc
003d7a48: ldr      pc, [r3, #0x34]
003d7a4c: cmp      r0, #0
003d7a50: bne      #0x3d7a28
003d7a54: ldr      r3, [r4]
003d7a58: mov      r0, r4
003d7a5c: mov      lr, pc
003d7a60: ldr      pc, [r3, #0x34]
003d7a64: cmp      r0, #0
003d7a68: bne      #0x3d7a28
003d7a6c: ldr      ip, [r6, #0x80]
003d7a70: add      r7, r6, #0x7c
003d7a74: cmp      ip, #0
003d7a78: movne    r1, r7
003d7a7c: movne    r3, ip
003d7a80: bne      #0x3d7a8c
003d7a84: b        #0x3d7bcc
003d7a88: mov      r3, r2
003d7a8c: ldr      r2, [r3, #0x10]
003d7a90: cmp      r4, r2
003d7a94: ldrhi    r2, [r3, #0xc]
003d7a98: ldrls    r2, [r3, #8]
003d7a9c: movhi    r3, r1
003d7aa0: mov      r1, r3
003d7aa4: cmp      r2, #0
003d7aa8: bne      #0x3d7a88
003d7aac: cmp      r7, r3
003d7ab0: beq      #0x3d7c34
003d7ab4: ldr      r2, [r3, #0x10]
003d7ab8: cmp      r4, r2
003d7abc: blo      #0x3d7bcc
003d7ac0: cmp      r7, r3
003d7ac4: beq      #0x3d7c34
003d7ac8: cmp      ip, #0
003d7acc: moveq    ip, r7
003d7ad0: beq      #0x3d7b00
003d7ad4: mov      r2, r7
003d7ad8: b        #0x3d7ae0
003d7adc: mov      ip, r3
003d7ae0: ldr      r3, [ip, #0x10]
003d7ae4: cmp      r4, r3
003d7ae8: ldrhi    r3, [ip, #0xc]
003d7aec: ldrls    r3, [ip, #8]
003d7af0: movhi    ip, r2
003d7af4: mov      r2, ip
003d7af8: cmp      r3, #0
003d7afc: bne      #0x3d7adc
003d7b00: cmp      r7, ip
003d7b04: beq      #0x3d7b18
003d7b08: ldr      r2, [ip, #0x10]
003d7b0c: mov      r3, ip
003d7b10: cmp      r4, r2
003d7b14: bhs      #0x3d7b40
003d7b18: add      r3, sp, #0x10
003d7b1c: mov      lr, #0
003d7b20: mov      r1, r7
003d7b24: add      r0, sp, #0x20
003d7b28: add      r2, sp, #0x24
003d7b2c: str      lr, [sp, #0x14]
003d7b30: str      ip, [sp, #0x24]
003d7b34: str      r4, [sp, #0x10]
003d7b38: bl       #0x3d7678 ; _ZNSt4priv8_Rb_treeIP9CharacterSt4lessIS2_ESt4pairIKS2_fENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueENS_17_Rb_tree_iteratorIS7_SB_EERKS7_
003d7b3c: ldr      r3, [sp, #0x20]
003d7b40: str      r5, [r3, #0x14]
003d7b44: ldr      ip, [r4, #0x460]
003d7b48: add      r1, r4, #0x450
003d7b4c: add      r1, r1, #0xc
003d7b50: cmp      ip, #0
003d7b54: beq      #0x3d7c28
003d7b58: ldr      r6, [r6, #4]
003d7b5c: mov      r2, r1
003d7b60: b        #0x3d7b68
003d7b64: mov      ip, r3
003d7b68: ldr      r3, [ip, #0x10]
003d7b6c: cmp      r6, r3
003d7b70: ldrhi    r3, [ip, #0xc]
003d7b74: ldrls    r3, [ip, #8]
003d7b78: movhi    ip, r2
003d7b7c: mov      r2, ip
003d7b80: cmp      r3, #0
003d7b84: bne      #0x3d7b64
003d7b88: cmp      r1, ip
003d7b8c: beq      #0x3d7ba0
003d7b90: ldr      r2, [ip, #0x10]
003d7b94: mov      r3, ip
003d7b98: cmp      r6, r2
003d7b9c: bhs      #0x3d7bc4
003d7ba0: add      r3, sp, #8
003d7ba4: mov      lr, #0
003d7ba8: add      r0, sp, #0x18
003d7bac: add      r2, sp, #0x1c
003d7bb0: str      r6, [sp, #8]
003d7bb4: str      lr, [sp, #0xc]
003d7bb8: str      ip, [sp, #0x1c]
003d7bbc: bl       #0x3d7678 ; _ZNSt4priv8_Rb_treeIP9CharacterSt4lessIS2_ESt4pairIKS2_fENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueENS_17_Rb_tree_iteratorIS7_SB_EERKS7_
003d7bc0: ldr      r3, [sp, #0x18]
003d7bc4: str      r5, [r3, #0x14]
003d7bc8: b        #0x3d7a2c
003d7bcc: mov      r3, r7
003d7bd0: b        #0x3d7ac0
003d7bd4: ldr      r2, [pc, #0x78]
003d7bd8: ldr      r2, [r3, r2]
003d7bdc: ldr      r2, [r2]
003d7be0: cmp      r2, #2
003d7be4: streq    r4, [r4]
003d7be8: beq      #0x3d7a0c
003d7bec: cmp      r2, #1
003d7bf0: bne      #0x3d7a0c
003d7bf4: ldr      r0, [pc, #0x5c]
003d7bf8: ldr      r1, [pc, #0x5c]
003d7bfc: ldr      r2, [pc, #0x5c]
003d7c00: ldr      r0, [r3, r0]
003d7c04: ldr      r3, [pc, #0x58]
003d7c08: mov      ip, #0x274
003d7c0c: add      r1, pc, r1
003d7c10: add      r2, pc, r2
003d7c14: add      r3, pc, r3
003d7c18: add      r0, r0, #0xa8
003d7c1c: str      ip, [sp]
003d7c20: bl       #0x30e004 ; 
003d7c24: b        #0x3d7a0c
003d7c28: ldr      r6, [r6, #4]
003d7c2c: mov      ip, r1
003d7c30: b        #0x3d7b88
003d7c34: ldr      r3, [r4, #0x3c8]
003d7c38: add      r0, r4, #0x3c8
003d7c3c: ldr      r1, [r6, #4]
003d7c40: mov      lr, pc
003d7c44: ldr      pc, [r3, #0x38]
003d7c48: ldr      ip, [r6, #0x80]
003d7c4c: b        #0x3d7ac8

# _ZN13TrophyManager12UnlockTrophyEi
003813b8: push     {r4, r5, r6, r7, lr}
003813bc: subs     r5, r1, #0
003813c0: sub      sp, sp, #0xc
003813c4: mov      r4, r0
003813c8: blt      #0x3813d8
003813cc: bl       #0x380058 ; _ZN13TrophyManager16IsTrophyUnlockedEi
003813d0: cmp      r0, #0
003813d4: beq      #0x3813e0
003813d8: add      sp, sp, #0xc
003813dc: pop      {r4, r5, r6, r7, pc}
003813e0: mov      r0, r4
003813e4: mov      r1, r5
003813e8: bl       #0x37f9e4 ; _ZN13TrophyManager17IsTrophyUnlockingEi
003813ec: cmp      r0, #0
003813f0: bne      #0x3813d8
003813f4: ldr      r6, [r4, #0x14]
003813f8: ldr      r3, [r4, #0x18]
003813fc: cmp      r6, r3
00381400: beq      #0x381420
00381404: str      r5, [r6]
00381408: ldr      r3, [r4, #0x14]
0038140c: add      r3, r3, #4
00381410: str      r3, [r4, #0x14]
00381414: mov      r0, r5
00381418: bl       #0x380e48 ; _ZN13TrophyManager16TrophyUnlockedCBEi
0038141c: b        #0x3813d8
00381420: ldr      r3, [r4, #0x10]
00381424: rsb      r3, r3, r6
00381428: asr      r3, r3, #2
0038142c: cmp      r3, #1
00381430: addhs    r1, r3, r3
00381434: addlo    r1, r3, #1
00381438: cmn      r1, #0xc0000001
0038143c: bhi      #0x3814b8
00381440: cmp      r3, r1
00381444: bhi      #0x3814b8
00381448: add      r2, sp, #8
0038144c: str      r1, [r2, #-4]!
00381450: add      r0, r4, #0x18
00381454: bl       #0x35fd5c ; _ZNSaIiE11_M_allocateEjRj
00381458: ldr      r1, [r4, #0x10]
0038145c: mov      r7, r0
00381460: subs     r6, r6, r1
00381464: moveq    r6, r0
00381468: beq      #0x381478
0038146c: mov      r2, r6
00381470: bl       #0x30df38 ; 
00381474: add      r6, r0, r6
00381478: str      r5, [r6], #4
0038147c: ldr      r0, [r4, #0x10]
00381480: ldr      r1, [r4, #0x18]
00381484: cmp      r0, #0
00381488: beq      #0x3814a0
0038148c: rsb      r1, r0, r1
00381490: bic      r1, r1, #3
00381494: cmp      r1, #0x80
00381498: bhi      #0x3814c0
0038149c: bl       #0x708f00 ; 
003814a0: ldr      r3, [sp, #4]
003814a4: str      r7, [r4, #0x10]
003814a8: str      r6, [r4, #0x14]
003814ac: add      r7, r7, r3, lsl #2
003814b0: str      r7, [r4, #0x18]
003814b4: b        #0x381414
003814b8: mvn      r1, #0xc0000000
003814bc: b        #0x381448
003814c0: bl       #0x310440 ; _Z10CustomFreePv
003814c4: b        #0x3814a0

# _ZN17PlayerStatManagerD2Ev
003790cc: bx       lr

# _ZN9Character15SG_ReloadSkillsEv
003bbe2c: movw     r3, #0x14e8
003bbe30: ldr      r0, [r0, r3]
003bbe34: cmp      r0, #0
003bbe38: bxeq     lr
003bbe3c: b        #0x467324

# _Z16CF_SetCombatantsP9CharacterS0_ibb
003b059c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b05a0: ldr      r4, [pc, #0x70]
003b05a4: mov      r5, r1
003b05a8: add      r1, r0, #0xff0
003b05ac: add      r4, pc, r4
003b05b0: mov      r6, r2
003b05b4: add      r1, r1, #4
003b05b8: str      r0, [r4, #0x1c]
003b05bc: mov      r2, #0x13
003b05c0: str      r5, [r4, #0x20]
003b05c4: add      r0, r0, #0x560
003b05c8: mov      r8, r3
003b05cc: ldrb     r7, [sp, #0x20]
003b05d0: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003b05d4: add      r1, r5, #0xff0
003b05d8: mov      sl, r0
003b05dc: mov      r2, #0x13
003b05e0: add      r1, r1, #4
003b05e4: add      r0, r5, #0x560
003b05e8: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003b05ec: rsb      r0, r0, sl
003b05f0: mov      r3, #0
003b05f4: rsb      r2, r0, #0
003b05f8: strb     r3, [r4, #0x33]
003b05fc: str      r2, [r4, #0x28]
003b0600: str      r6, [r4, #0x2c]
003b0604: strb     r8, [r4, #0x30]
003b0608: strb     r7, [r4, #0x31]
003b060c: str      r0, [r4, #0x24]
003b0610: strb     r3, [r4, #0x32]
003b0614: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0618: subseq   r2, pc, ip, ror #7

# _ZNSt4priv14__partial_sortIPSt4pairIiiES2_N17PlayerStatManager9_StatCompEEEvT_S6_S6_PT0_T1_
00379354: push     {r4, r5, r6, r7, r8, sl, lr}
00379358: mov      sl, #0
0037935c: sub      sp, sp, #0x1c
00379360: mov      r7, r2
00379364: mov      r4, r1
00379368: mov      r2, #0
0037936c: mov      r3, sl
00379370: str      sl, [sp]
00379374: mov      r6, r0
00379378: bl       #0x379290 ; _ZSt11__make_heapIPSt4pairIiiEN17PlayerStatManager9_StatCompES1_iEvT_S5_T0_PT1_PT2_
0037937c: cmp      r4, r7
00379380: bhs      #0x3793d4
00379384: mov      r5, r4
00379388: add      r8, sp, #0x10
0037938c: ldr      r3, [r6]
00379390: ldr      ip, [r5]
00379394: mov      r2, r5
00379398: mov      r0, r6
0037939c: cmp      ip, r3
003793a0: mov      r1, r4
003793a4: mov      r3, r8
003793a8: ble      #0x3793c8
003793ac: ldr      lr, [r5, #4]
003793b0: str      ip, [sp, #0x10]
003793b4: mov      ip, #0
003793b8: strb     ip, [sp]
003793bc: str      lr, [sp, #0x14]
003793c0: str      sl, [sp, #4]
003793c4: bl       #0x379304 ; _ZSt10__pop_heapIPSt4pairIiiES1_N17PlayerStatManager9_StatCompEiEvT_S5_S5_T0_T1_PT2_
003793c8: add      r5, r5, #8
003793cc: cmp      r7, r5
003793d0: bhi      #0x37938c
003793d4: rsb      r5, r6, r4
003793d8: cmp      r5, #0xf
003793dc: ble      #0x379424
003793e0: add      r7, sp, #8
003793e4: mov      r8, #0
003793e8: ldr      lr, [r4, #-8]!
003793ec: sub      r5, r5, #8
003793f0: mov      r0, r6
003793f4: ldr      ip, [r4, #4]
003793f8: mov      r1, r4
003793fc: mov      r2, r4
00379400: str      ip, [sp, #0xc]
00379404: mov      r3, r7
00379408: mov      ip, #0
0037940c: str      lr, [sp, #8]
00379410: strb     ip, [sp]
00379414: str      r8, [sp, #4]
00379418: bl       #0x379304 ; _ZSt10__pop_heapIPSt4pairIiiES1_N17PlayerStatManager9_StatCompEiEvT_S5_S5_T0_T1_PT2_
0037941c: cmp      r5, #0xf
00379420: bgt      #0x3793e8
00379424: add      sp, sp, #0x1c
00379428: pop      {r4, r5, r6, r7, r8, sl, pc}

# _ZNK9Character37GetEffectiveThreatPerDamageToFriendlyEv
003bd3e4: add      r1, r0, #0xff0
003bd3e8: push     {r4, lr}
003bd3ec: add      r1, r1, #4
003bd3f0: mov      r2, #0xce
003bd3f4: add      r0, r0, #0x560
003bd3f8: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003bd3fc: bl       #0x30e964 ; 
003bd400: mov      r1, #0x3b800000
003bd404: bl       #0x30ed6c ; 
003bd408: pop      {r4, pc}

# _ZN6CharAI9OnDeAggroEP9Character
003d2014: push     {r4, r5, r6, r7, r8, sl, lr}
003d2018: ldr      r4, [pc, #0x98]
003d201c: ldr      r6, [pc, #0x98]
003d2020: ldr      r2, [pc, #0x98]
003d2024: add      r4, pc, r4
003d2028: ldr      r3, [r4, r6]
003d202c: ldr      r7, [r4, r2]
003d2030: sub      sp, sp, #0x24
003d2034: ldr      r3, [r3]
003d2038: mov      r8, r0
003d203c: mov      r0, r7
003d2040: str      r3, [sp, #0x1c]
003d2044: mov      sl, r1
003d2048: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003d204c: ldr      r1, [pc, #0x70]
003d2050: add      r5, sp, #4
003d2054: mov      r2, sp
003d2058: add      r1, pc, r1
003d205c: mov      r0, r5
003d2060: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003d2064: mov      r1, r5
003d2068: mov      r0, r7
003d206c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003d2070: mov      r0, r5
003d2074: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003d2078: ldr      r3, [r8, #0x1c]
003d207c: cmp      r3, #0
003d2080: beq      #0x3d2098
003d2084: mov      r0, r3
003d2088: mov      r1, sl
003d208c: ldr      r3, [r3]
003d2090: mov      lr, pc
003d2094: ldr      pc, [r3, #0x3c]
003d2098: ldr      r3, [r4, r6]
003d209c: ldr      r2, [sp, #0x1c]
003d20a0: ldr      r3, [r3]
003d20a4: cmp      r2, r3
003d20a8: bne      #0x3d20b4
003d20ac: add      sp, sp, #0x24
003d20b0: pop      {r4, r5, r6, r7, r8, sl, pc}
003d20b4: bl       #0x30e310 ; 
003d20b8: subseq   r2, ip, ip, ror #20
003d20bc: andeq    r4, r0, ip, lsr #1
003d20c0: andeq    r0, r0, r4, lsl #17
003d20c4: umaaleq  r3, pc, r0, r4

# _ZN17CharAISkillScriptC1EP9CharacterPKcj
003cde2c: push     {r4, r5, r6, r7, r8, sl, lr}
003cde30: ldr      r5, [pc, #0x11c]
003cde34: ldr      ip, [pc, #0x11c]
003cde38: mov      r4, r0
003cde3c: add      r5, pc, r5
003cde40: ldr      ip, [r5, ip]
003cde44: add      r7, r0, #0xc
003cde48: mov      sl, r1
003cde4c: add      ip, ip, #8
003cde50: str      ip, [r0]
003cde54: sub      sp, sp, #0xc
003cde58: stmib    r4, {r1, r2}
003cde5c: mov      r0, r7
003cde60: mov      r8, r3
003cde64: mov      r6, r2
003cde68: bl       #0x3192b4 ; _ZN3sfc6script3lua9ArgumentsC1Ev
003cde6c: mvn      r3, #0
003cde70: cmp      sl, #0
003cde74: str      r3, [r4, #0x18]
003cde78: str      r8, [r4, #0x14]
003cde7c: beq      #0x3cdeac
003cde80: cmp      r6, #0
003cde84: beq      #0x3cdf00
003cde88: mov      r1, r6
003cde8c: mov      r0, r7
003cde90: bl       #0x39ec10 ; _ZN3sfc6script3lua9Arguments10pushStringEPKc
003cde94: mov      r0, r7
003cde98: mov      r1, r8
003cde9c: bl       #0x3cdd78 ; _ZN3sfc6script3lua9Arguments11pushIntegerEi
003cdea0: mov      r0, r4
003cdea4: add      sp, sp, #0xc
003cdea8: pop      {r4, r5, r6, r7, r8, sl, pc}
003cdeac: ldr      r3, [pc, #0xa8]
003cdeb0: ldr      r3, [r5, r3]
003cdeb4: ldr      r3, [r3]
003cdeb8: cmp      r3, #2
003cdebc: streq    sl, [sl]
003cdec0: beq      #0x3cde80
003cdec4: cmp      r3, #1
003cdec8: bne      #0x3cde80
003cdecc: ldr      r0, [pc, #0x8c]
003cded0: ldr      r1, [pc, #0x8c]
003cded4: ldr      r2, [pc, #0x8c]
003cded8: ldr      r0, [r5, r0]
003cdedc: ldr      r3, [pc, #0x88]
003cdee0: mov      ip, #0x2e
003cdee4: add      r1, pc, r1
003cdee8: add      r2, pc, r2
003cdeec: add      r3, pc, r3
003cdef0: add      r0, r0, #0xa8
003cdef4: str      ip, [sp]
003cdef8: bl       #0x30e004 ; 
003cdefc: b        #0x3cde80
003cdf00: ldr      r3, [pc, #0x54]
003cdf04: ldr      r3, [r5, r3]
003cdf08: ldr      r3, [r3]
003cdf0c: cmp      r3, #2
003cdf10: streq    r6, [r6]
003cdf14: beq      #0x3cde88
003cdf18: cmp      r3, #1
003cdf1c: bne      #0x3cde88
003cdf20: ldr      r0, [pc, #0x38]
003cdf24: ldr      r1, [pc, #0x44]
003cdf28: ldr      r2, [pc, #0x44]
003cdf2c: ldr      r0, [r5, r0]
003cdf30: ldr      r3, [pc, #0x40]
003cdf34: mov      ip, #0x2e
003cdf38: add      r1, pc, r1
003cdf3c: add      r2, pc, r2
003cdf40: add      r3, pc, r3
003cdf44: add      r0, r0, #0xa8
003cdf48: str      ip, [sp]
003cdf4c: bl       #0x30e004 ; 
003cdf50: b        #0x3cde88
003cdf54: subseq   r6, ip, r4, asr ip
003cdf58: andeq    r0, r0, ip, lsr #23
003cdf5c: andeq    r3, r0, r0, asr #19
003cdf60: andeq    r1, r0, r0, asr #19
003cdf64: strdeq   r0, r1, [pc], #-0x44
003cdf68: subeq    r7, pc, r0, lsl r4
003cdf6c: subeq    r7, pc, r4, lsl r4
003cdf70: subeq    r0, pc, r0, lsr #9
003cdf74: subseq   r3, r1, ip, lsr #3
003cdf78: subeq    r7, pc, r0, asr #7

# _ZN9Character13F_ApplyResultERKNS_12AttackResultEP10GameObjectPS_b
003b01b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b01bc: ldr      r5, [pc, #0x39c]
003b01c0: ldr      sb, [pc, #0x39c]
003b01c4: mov      r6, r0
003b01c8: add      r5, pc, r5
003b01cc: ldr      r0, [r5, sb]
003b01d0: mov      r4, r2
003b01d4: sub      sp, sp, #0x6c
003b01d8: ldr      r2, [r0]
003b01dc: mov      r8, r3
003b01e0: mov      r7, r1
003b01e4: str      r2, [sp, #0x64]
003b01e8: bl       #0x7fd794 ; _Z9GetOnlinev
003b01ec: ldrb     r3, [r0, #5]
003b01f0: cmp      r3, #0
003b01f4: beq      #0x3b0200
003b01f8: cmp      r8, #0
003b01fc: beq      #0x3b02c4
003b0200: ldr      fp, [pc, #0x360]
003b0204: add      r8, sp, #0x4c
003b0208: ldr      sl, [r5, fp]
003b020c: mov      r0, sl
003b0210: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b0214: ldr      r1, [pc, #0x350]
003b0218: add      r2, sp, #0x18
003b021c: mov      r0, r8
003b0220: add      r1, pc, r1
003b0224: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b0228: mov      r0, sl
003b022c: mov      r1, r8
003b0230: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b0234: cmp      r0, #0
003b0238: beq      #0x3b0398
003b023c: mov      r0, r8
003b0240: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b0244: ldr      r3, [r4]
003b0248: mov      r0, r4
003b024c: mov      lr, pc
003b0250: ldr      pc, [r3, #0x34]
003b0254: subs     r2, r0, #0
003b0258: bne      #0x3b0268
003b025c: ldrb     r3, [r6, #0x18]
003b0260: tst      r3, #0x10
003b0264: bne      #0x3b034c
003b0268: mov      r0, r4
003b026c: bl       #0x3bc6b8 ; _ZN9Character14CancelSneakingEv
003b0270: mov      r0, r6
003b0274: mov      r1, r7
003b0278: mov      r2, r4
003b027c: bl       #0x3af1fc ; _ZN9Character26F_ApplyScrollingCombatTextERKNS_12AttackResultEP10GameObjectPS_
003b0280: mov      r0, r6
003b0284: mov      r1, r7
003b0288: mov      r2, r4
003b028c: bl       #0x3afd38 ; _ZN9Character18F_ApplyCombatSoundERKNS_12AttackResultEP10GameObjectPS_
003b0290: mov      r2, r6
003b0294: ldr      r3, [r4, #0x3c8]
003b0298: add      r0, r4, #0x3c8
003b029c: mov      r1, r7
003b02a0: mov      lr, pc
003b02a4: ldr      pc, [r3, #0xb8]
003b02a8: ldr      r3, [r5, sb]
003b02ac: ldr      r2, [sp, #0x64]
003b02b0: ldr      r3, [r3]
003b02b4: cmp      r2, r3
003b02b8: bne      #0x3b055c
003b02bc: add      sp, sp, #0x6c
003b02c0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b02c4: ldr      r3, [r7, #0x110]
003b02c8: cmn      r3, #1
003b02cc: bne      #0x3b0200
003b02d0: cmp      r4, #0
003b02d4: beq      #0x3b04b0
003b02d8: cmp      r4, #0
003b02dc: ldrne    sl, [r4, #0x108]
003b02e0: ldr      r8, [r7, #0x108]
003b02e4: moveq    r2, #1
003b02e8: lsrne    r2, sl, #0x1f
003b02ec: lsr      r3, r8, #0x1f
003b02f0: mvneq    sl, #0
003b02f4: orrs     r3, r2, r3
003b02f8: beq      #0x3b0320
003b02fc: ldr      r3, [pc, #0x26c]
003b0300: ldr      r3, [r5, r3]
003b0304: ldr      r3, [r3]
003b0308: cmp      r3, #2
003b030c: moveq    r3, #0
003b0310: streq    r3, [r3]
003b0314: beq      #0x3b0320
003b0318: cmp      r3, #1
003b031c: beq      #0x3b0528
003b0320: bl       #0x80b1bc ; _ZN10CMessaging3GetEv
003b0324: mov      r1, sl
003b0328: mov      fp, r0
003b032c: mov      r2, r6
003b0330: mov      r0, r8
003b0334: mov      r3, #0
003b0338: bl       #0x3af330 ; _ZN16CMsgAttackResult6CreateEiiRKN9Character12AttackResultEb
003b033c: mov      r1, r0
003b0340: mov      r0, fp
003b0344: bl       #0x80e2a4 ; _ZN10CMessaging7SendMsgEP8CMessage
003b0348: b        #0x3b0200
003b034c: add      r0, r4, #0x4f0
003b0350: mov      r1, r7
003b0354: add      r0, r0, #0xc
003b0358: bl       #0x3c5d84 ; _ZN16CharStateMachine17SM_SetInjureStateEPvb
003b035c: ldr      sl, [r5, fp]
003b0360: add      r8, sp, #0x1c
003b0364: mov      r0, sl
003b0368: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b036c: ldr      r1, [pc, #0x200]
003b0370: add      r2, sp, #0x10
003b0374: mov      r0, r8
003b0378: add      r1, pc, r1
003b037c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b0380: mov      r0, sl
003b0384: mov      r1, r8
003b0388: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b038c: mov      r0, r8
003b0390: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b0394: b        #0x3b0268
003b0398: mov      r0, sl
003b039c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003b03a0: ldr      r1, [pc, #0x1d0]
003b03a4: add      r3, sp, #0x34
003b03a8: add      r2, sp, #0x14
003b03ac: add      r1, pc, r1
003b03b0: mov      r0, r3
003b03b4: str      r3, [sp, #0xc]
003b03b8: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003b03bc: mov      r0, sl
003b03c0: ldr      r1, [sp, #0xc]
003b03c4: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003b03c8: cmp      r0, #0
003b03cc: bne      #0x3b048c
003b03d0: movw     r3, #0x14f0
003b03d4: ldrb     r3, [r4, r3]
003b03d8: cmp      r3, #0
003b03dc: bne      #0x3b04a4
003b03e0: ldr      r0, [sp, #0xc]
003b03e4: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b03e8: mov      r0, r8
003b03ec: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b03f0: ldr      r3, [r6]
003b03f4: cmp      r3, #0
003b03f8: ble      #0x3b0244
003b03fc: ldr      r3, [r6, #0x1c]
003b0400: tst      r3, #0x200000
003b0404: beq      #0x3b0440
003b0408: ldr      r8, [r6, #0x24]
003b040c: cmn      r8, #1
003b0410: addne    r8, r8, #0x7c
003b0414: beq      #0x3b0518
003b0418: mov      r0, r4
003b041c: bl       #0x3935dc ; _ZNK10GameObject17GetTargetPositionEv
003b0420: ldr      r3, [pc, #0x154]
003b0424: mov      ip, #0
003b0428: mov      r2, r0
003b042c: mov      r1, r8
003b0430: ldr      r0, [r5, r3]
003b0434: mov      r3, ip
003b0438: str      ip, [sp]
003b043c: bl       #0x495d14 ; _ZN15VisualFXManager13PlayAnimFXSetEiRK7Point3DIfEPK10GameObjectPNS_13AnimFXSetDataE
003b0440: ldr      r3, [r4, #0x110]
003b0444: mov      r2, #0
003b0448: strb     r2, [r4, #0x53b]
003b044c: cmn      r3, #1
003b0450: beq      #0x3b0504
003b0454: ldr      r3, [r4]
003b0458: mov      r0, r4
003b045c: mov      lr, pc
003b0460: ldr      pc, [r3, #0x34]
003b0464: cmp      r0, #0
003b0468: beq      #0x3b0244
003b046c: ldrb     r3, [r6, #0x18]
003b0470: ldrb     r2, [r6, #0x19]
003b0474: and      r3, r3, #0xbf
003b0478: bfc      r2, #0, #1
003b047c: bfc      r3, #5, #1
003b0480: strb     r2, [r6, #0x19]
003b0484: strb     r3, [r6, #0x18]
003b0488: b        #0x3b0244
003b048c: ldr      r3, [r4]
003b0490: mov      r0, r4
003b0494: mov      lr, pc
003b0498: ldr      pc, [r3, #0x28]
003b049c: cmp      r0, #0
003b04a0: beq      #0x3b03d0
003b04a4: ldr      r0, [sp, #0xc]
003b04a8: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003b04ac: b        #0x3b023c
003b04b0: ldr      r3, [pc, #0xb8]
003b04b4: ldr      r3, [r5, r3]
003b04b8: ldr      r3, [r3]
003b04bc: cmp      r3, #2
003b04c0: streq    r4, [r4]
003b04c4: beq      #0x3b02d8
003b04c8: cmp      r3, #1
003b04cc: bne      #0x3b02d8
003b04d0: ldr      r0, [pc, #0xa8]
003b04d4: ldr      r1, [pc, #0xa8]
003b04d8: ldr      r2, [pc, #0xa8]
003b04dc: ldr      r0, [r5, r0]
003b04e0: ldr      r3, [pc, #0xa4]
003b04e4: movw     ip, #0x40b
003b04e8: add      r1, pc, r1
003b04ec: add      r2, pc, r2
003b04f0: add      r3, pc, r3
003b04f4: add      r0, r0, #0xa8
003b04f8: str      ip, [sp]
003b04fc: bl       #0x30e004 ; 
003b0500: b        #0x3b02d8
003b0504: mov      r0, r4
003b0508: ldr      r1, [r6]
003b050c: mov      r2, r7
003b0510: bl       #0x3a8bc4 ; _ZN9Character6HitForEjP10GameObject
003b0514: b        #0x3b0454
003b0518: mov      r0, r4
003b051c: bl       #0x3a33d0 ; _ZNK9Character10GetFXBloodEv
003b0520: mov      r8, r0
003b0524: b        #0x3b0418
003b0528: ldr      r0, [pc, #0x50]
003b052c: ldr      r1, [pc, #0x5c]
003b0530: ldr      r2, [pc, #0x5c]
003b0534: ldr      r0, [r5, r0]
003b0538: ldr      r3, [pc, #0x58]
003b053c: movw     ip, #0x415
003b0540: add      r1, pc, r1
003b0544: add      r2, pc, r2
003b0548: add      r3, pc, r3
003b054c: add      r0, r0, #0xa8
003b0550: str      ip, [sp]
003b0554: bl       #0x30e004 ; 
003b0558: b        #0x3b0320
003b055c: bl       #0x30e310 ; 
003b0560: subseq   r4, lr, r8, asr #17
003b0564: andeq    r4, r0, ip, lsr #1
003b0568: andeq    r0, r0, r4, lsl #17
003b056c: subseq   r3, r1, r0, lsl #21
003b0570: andeq    r3, r0, r0, asr #19
003b0574: subseq   r3, r1, r0, asr #18
003b0578: subseq   r3, r1, r4, lsl #18
003b057c: andeq    r1, r0, r8, lsl #22
003b0580: andeq    r1, r0, r0, asr #19
003b0584: ldrsheq  sp, [r0], #-0xe0
003b0588: subseq   r3, r1, ip, lsl #14
003b058c: subseq   r3, r1, r0, lsr #14

# _ZN9Character16_SkillCombatRollERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b9fbc: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b9fc0: ldr      r6, [r0, #4]
003b9fc4: mov      r5, r1
003b9fc8: mov      r4, r2
003b9fcc: ldm      r6, {r1, r3}
003b9fd0: sub      sp, sp, #0x40
003b9fd4: mov      r7, r0
003b9fd8: rsb      r3, r1, r3
003b9fdc: asr      r3, r3, #4
003b9fe0: add      r2, r3, r3, lsl #3
003b9fe4: add      r2, r2, r2, lsl #6
003b9fe8: add      r2, r3, r2, lsl #3
003b9fec: add      r2, r2, r2, lsl #15
003b9ff0: add      r3, r3, r2, lsl #3
003b9ff4: rsb      r3, r3, #0
003b9ff8: cmp      r3, #1
003b9ffc: bls      #0x3ba014
003ba000: cmp      r3, #0
003ba004: beq      #0x3ba01c
003ba008: ldr      r3, [r1, #4]
003ba00c: cmp      r3, #3
003ba010: beq      #0x3ba030
003ba014: add      sp, sp, #0x40
003ba018: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003ba01c: ldr      r0, [pc, #0x268]
003ba020: add      r0, pc, r0
003ba024: bl       #0x708eb0 ; 
003ba028: ldr      r1, [r6]
003ba02c: b        #0x3ba008
003ba030: mov      r1, #0
003ba034: mov      r0, r7
003ba038: bl       #0x37baf8 ; _ZNK3sfc6script3lua9ArgumentsixEj
003ba03c: bl       #0x38d798 ; _ZNK3sfc6script3lua5Value11getUIntegerEv
003ba040: mov      r6, r0
003ba044: mov      r0, r4
003ba048: bl       #0x3bc5fc ; _ZNK9Character16GetCharSkillListEv
003ba04c: ldr      r3, [r0, #4]
003ba050: cmp      r6, r3
003ba054: bhs      #0x3ba014
003ba058: ldr      r6, [r7, #4]
003ba05c: ldr      r3, [r6]
003ba060: ldr      r2, [r6, #4]
003ba064: rsb      r2, r3, r2
003ba068: asr      r2, r2, #4
003ba06c: add      r1, r2, r2, lsl #3
003ba070: add      r1, r1, r1, lsl #6
003ba074: add      r1, r2, r1, lsl #3
003ba078: add      r1, r1, r1, lsl #15
003ba07c: add      r2, r2, r1, lsl #3
003ba080: rsb      r2, r2, #0
003ba084: cmp      r2, #1
003ba088: bhi      #0x3ba09c
003ba08c: ldr      r0, [pc, #0x1fc]
003ba090: add      r0, pc, r0
003ba094: bl       #0x708eb0 ; 
003ba098: ldr      r3, [r6]
003ba09c: ldr      r3, [r3, #0x74]
003ba0a0: cmp      r3, #2
003ba0a4: beq      #0x3ba0f4
003ba0a8: ldr      r6, [r7, #4]
003ba0ac: ldm      r6, {r2, r3}
003ba0b0: rsb      r3, r2, r3
003ba0b4: asr      r3, r3, #4
003ba0b8: add      r1, r3, r3, lsl #3
003ba0bc: add      r1, r1, r1, lsl #6
003ba0c0: add      r1, r3, r1, lsl #3
003ba0c4: add      r1, r1, r1, lsl #15
003ba0c8: add      r3, r3, r1, lsl #3
003ba0cc: rsb      r3, r3, #0
003ba0d0: cmp      r3, #1
003ba0d4: bhi      #0x3ba0e8
003ba0d8: ldr      r0, [pc, #0x1b4]
003ba0dc: add      r0, pc, r0
003ba0e0: bl       #0x708eb0 ; 
003ba0e4: ldr      r2, [r6]
003ba0e8: ldr      r3, [r2, #0x74]
003ba0ec: cmp      r3, #7
003ba0f0: bne      #0x3ba014
003ba0f4: mov      r1, #0
003ba0f8: mov      r0, r7
003ba0fc: bl       #0x37baf8 ; _ZNK3sfc6script3lua9ArgumentsixEj
003ba100: bl       #0x38d798 ; _ZNK3sfc6script3lua5Value11getUIntegerEv
003ba104: mov      r1, #1
003ba108: mov      r8, r0
003ba10c: mov      r0, r7
003ba110: bl       #0x37baf8 ; _ZNK3sfc6script3lua9ArgumentsixEj
003ba114: bl       #0x31b5a0 ; _ZNK3sfc6script3lua5Value11getUserDataEv
003ba118: subs     r6, r0, #0
003ba11c: beq      #0x3ba014
003ba120: add      r7, sp, #0x34
003ba124: mov      r1, r6
003ba128: mov      r0, r7
003ba12c: bl       #0x33dd2c ; _ZN10ObjectBase9GetHandleEv
003ba130: mov      r0, r7
003ba134: bl       #0x33ff54 ; _ZN12ObjectHandlecvP9CharacterEv
003ba138: subs     sl, r0, #0
003ba13c: beq      #0x3ba24c
003ba140: mov      r1, r8
003ba144: mov      r0, r4
003ba148: bl       #0x3bc784 ; _ZNK9Character12GetCharSkillEi
003ba14c: ldr      r8, [r0, #0x1c]
003ba150: mov      r6, r0
003ba154: ands     r7, r8, #0x800000
003ba158: beq      #0x3ba1c4
003ba15c: add      sb, r4, #0x37c
003ba160: mov      r0, sb
003ba164: bl       #0x3ffe8c ; _ZNK13ItemInventory17HasMainHandWeaponEv
003ba168: cmp      r0, #0
003ba16c: bne      #0x3ba208
003ba170: mov      r0, sb
003ba174: bl       #0x400158 ; _ZNK13ItemInventory16HasOffHandWeaponEv
003ba178: cmp      r0, #0
003ba17c: beq      #0x3ba014
003ba180: ldr      ip, [r6, #0x14]
003ba184: add      r6, sp, #0xc
003ba188: orr      r3, r8, #0x4000000
003ba18c: mov      r0, r6
003ba190: mov      r1, r4
003ba194: mov      r2, sl
003ba198: str      ip, [sp]
003ba19c: bl       #0x3b31a4 ; _ZN9Character13F_SkillAttackERNS_12AttackResultEPS_S2_ii
003ba1a0: mov      r0, r6
003ba1a4: mov      r1, r4
003ba1a8: mov      r2, sl
003ba1ac: mov      r3, #0
003ba1b0: bl       #0x3b10b4 ; _ZN9Character13F_ApplyResultERKNS_12AttackResultEPS_S3_b
003ba1b4: mov      r0, r5
003ba1b8: ldr      r1, [sp, #0xc]
003ba1bc: bl       #0x37cb24 ; _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
003ba1c0: b        #0x3ba014
003ba1c4: ldr      ip, [r0, #0x14]
003ba1c8: add      r6, sp, #0xc
003ba1cc: mov      r3, r8
003ba1d0: mov      r0, r6
003ba1d4: mov      r1, r4
003ba1d8: mov      r2, sl
003ba1dc: str      ip, [sp]
003ba1e0: bl       #0x3b31a4 ; _ZN9Character13F_SkillAttackERNS_12AttackResultEPS_S2_ii
003ba1e4: mov      r0, r6
003ba1e8: mov      r1, r4
003ba1ec: mov      r2, sl
003ba1f0: mov      r3, r7
003ba1f4: bl       #0x3b10b4 ; _ZN9Character13F_ApplyResultERKNS_12AttackResultEPS_S3_b
003ba1f8: mov      r0, r5
003ba1fc: ldr      r1, [sp, #0xc]
003ba200: bl       #0x37cb24 ; _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
003ba204: b        #0x3ba014
003ba208: ldr      ip, [r6, #0x14]
003ba20c: add      r7, sp, #0xc
003ba210: mov      r3, r8
003ba214: mov      r0, r7
003ba218: mov      r1, r4
003ba21c: mov      r2, sl
003ba220: str      ip, [sp]
003ba224: bl       #0x3b31a4 ; _ZN9Character13F_SkillAttackERNS_12AttackResultEPS_S2_ii
003ba228: mov      r0, r7
003ba22c: mov      r1, r4
003ba230: mov      r2, sl
003ba234: mov      r3, #0
003ba238: bl       #0x3b10b4 ; _ZN9Character13F_ApplyResultERKNS_12AttackResultEPS_S3_b
003ba23c: mov      r0, r5
003ba240: ldr      r1, [sp, #0xc]
003ba244: bl       #0x37cb24 ; _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
003ba248: b        #0x3ba170
003ba24c: ldr      r3, [r6]
003ba250: mov      r0, r6
003ba254: mov      r1, r4
003ba258: mov      lr, pc
003ba25c: ldr      pc, [r3, #0x90]
003ba260: cmp      r0, #8
003ba264: bne      #0x3ba014
003ba268: mov      r0, r6
003ba26c: mov      r1, r4
003ba270: ldr      r3, [r6]
003ba274: mov      lr, pc
003ba278: ldr      pc, [r3, #0x98]
003ba27c: mov      r0, r5
003ba280: mov      r1, sl
003ba284: bl       #0x37c7e4 ; _ZN3sfc6script3lua12ReturnValues11pushBooleanEb
003ba288: b        #0x3ba014
003ba28c: subseq   r4, r0, r8, asr #8
003ba290: ldrsbeq  r4, [r0], #-0x38
003ba294: subseq   r4, r0, ip, lsl #7

# _ZN6CharAI15UpdateAllSkillsEv
003d8894: push     {r4, r5, r6, lr}
003d8898: mov      r5, r0
003d889c: ldr      r0, [r0, #4]
003d88a0: add      r0, r0, #0x4f0
003d88a4: add      r0, r0, #0xc
003d88a8: bl       #0x3c02e8 ; _ZNK16CharStateMachine15SM_IsUsingSkillEv
003d88ac: cmp      r0, #0
003d88b0: beq      #0x3d88b8
003d88b4: pop      {r4, r5, r6, pc}
003d88b8: ldr      r0, [r5, #4]
003d88bc: add      r0, r0, #0x4f0
003d88c0: add      r0, r0, #0xc
003d88c4: bl       #0x3c0334 ; _ZNK16CharStateMachine12SM_IsCastingEv
003d88c8: subs     r4, r0, #0
003d88cc: bne      #0x3d88b4
003d88d0: ldr      r3, [r5, #0xb4]
003d88d4: ldr      r6, [r5, #0xb8]
003d88d8: rsb      r6, r3, r6
003d88dc: asrs     r6, r6, #2
003d88e0: bne      #0x3d88ec
003d88e4: b        #0x3d8908
003d88e8: ldr      r3, [r5, #0xb4]
003d88ec: ldr      r0, [r3, r4, lsl #2]
003d88f0: add      r4, r4, #1
003d88f4: cmp      r0, #0
003d88f8: beq      #0x3d8900
003d88fc: bl       #0x3dabd0 ; _ZN17CharAISkillScript13OnSkillUpdateEv
003d8900: cmp      r4, r6
003d8904: bne      #0x3d88e8
003d8908: ldr      r3, [r5, #0xc0]
003d890c: ldr      r6, [r5, #0xc4]
003d8910: rsb      r6, r3, r6
003d8914: asrs     r6, r6, #2
003d8918: beq      #0x3d88b4
003d891c: mov      r4, #0
003d8920: b        #0x3d8928
003d8924: ldr      r3, [r5, #0xc0]
003d8928: ldr      r0, [r3, r4, lsl #2]
003d892c: add      r4, r4, #1
003d8930: cmp      r0, #0
003d8934: beq      #0x3d893c
003d8938: bl       #0x3dabd0 ; _ZN17CharAISkillScript13OnSkillUpdateEv
003d893c: cmp      r4, r6
003d8940: bne      #0x3d8924
003d8944: pop      {r4, r5, r6, pc}

# _ZNK17PlayerStatManager18GetNumLeadingStatsEi
00379e10: push     {r4, r5, r6, r7, r8, lr}
00379e14: mov      r4, #0
00379e18: mov      r6, r0
00379e1c: mov      r5, r1
00379e20: mov      r7, r4
00379e24: mov      r1, r4
00379e28: mov      r0, r6
00379e2c: bl       #0x379dfc ; _ZNK17PlayerStatManager9GetLeaderE9EStatType
00379e30: cmp      r0, r5
00379e34: beq      #0x379e4c
00379e38: add      r4, r4, #1
00379e3c: cmp      r4, #6
00379e40: bne      #0x379e24
00379e44: mov      r0, r7
00379e48: pop      {r4, r5, r6, r7, r8, pc}
00379e4c: mov      r1, r4
00379e50: mov      r0, r6
00379e54: mov      r2, r5
00379e58: bl       #0x379824 ; _ZNK17PlayerStatManager12GetStatValueE9EStatTypei
00379e5c: cmp      r0, #0
00379e60: addgt    r7, r7, #1
00379e64: b        #0x379e38

# _ZN6CharAI7OnAggroEP9Character
003d20c8: push     {r4, r5, r6, r7, r8, sl, lr}
003d20cc: ldr      r4, [pc, #0x98]
003d20d0: ldr      r6, [pc, #0x98]
003d20d4: ldr      r2, [pc, #0x98]
003d20d8: add      r4, pc, r4
003d20dc: ldr      r3, [r4, r6]
003d20e0: ldr      r7, [r4, r2]
003d20e4: sub      sp, sp, #0x24
003d20e8: ldr      r3, [r3]
003d20ec: mov      r8, r0
003d20f0: mov      r0, r7
003d20f4: str      r3, [sp, #0x1c]
003d20f8: mov      sl, r1
003d20fc: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003d2100: ldr      r1, [pc, #0x70]
003d2104: add      r5, sp, #4
003d2108: mov      r2, sp
003d210c: add      r1, pc, r1
003d2110: mov      r0, r5
003d2114: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003d2118: mov      r1, r5
003d211c: mov      r0, r7
003d2120: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003d2124: mov      r0, r5
003d2128: bl       #0x3139ac ; _ZNSt4priv12_String_baseIcSaIcEE19_M_deallocate_blockEv
003d212c: ldr      r3, [r8, #0x1c]
003d2130: cmp      r3, #0
003d2134: beq      #0x3d214c
003d2138: mov      r0, r3
003d213c: mov      r1, sl
003d2140: ldr      r3, [r3]
003d2144: mov      lr, pc
003d2148: ldr      pc, [r3, #0x38]
003d214c: ldr      r3, [r4, r6]
003d2150: ldr      r2, [sp, #0x1c]
003d2154: ldr      r3, [r3]
003d2158: cmp      r2, r3
003d215c: bne      #0x3d2168
003d2160: add      sp, sp, #0x24
003d2164: pop      {r4, r5, r6, r7, r8, sl, pc}
003d2168: bl       #0x30e310 ; 
003d216c: ldrheq   r2, [ip], #-0x98
003d2170: andeq    r4, r0, ip, lsr #1
003d2174: andeq    r0, r0, r4, lsl #17
003d2178: ldrdeq   r3, r4, [pc], #-0x3c

# _ZN3sfc6script3lua5Value13freeValueListEPSt6vectorIS2_SaIS2_EE
0031d194: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0031d198: ldr      r5, [pc, #0x3b4]
0031d19c: sub      sp, sp, #0x114
0031d1a0: subs     r3, r0, #0
0031d1a4: str      r0, [sp, #0xc]
0031d1a8: add      r5, pc, r5
0031d1ac: beq      #0x31d4f4
0031d1b0: ldr      r6, [pc, #0x3a0]
0031d1b4: add      r3, sp, #0xc
0031d1b8: add      r2, sp, #0xe0
0031d1bc: ldr      r4, [r5, r6]
0031d1c0: add      r0, sp, #0xf0
0031d1c4: add      r1, sp, #0xd0
0031d1c8: ldmib    r4, {r7, ip}
0031d1cc: ldr      r8, [r4]
0031d1d0: str      ip, [sp, #0xd8]
0031d1d4: ldr      ip, [r4, #0x10]
0031d1d8: ldr      sl, [r4, #0x1c]
0031d1dc: ldr      sb, [r4, #0x18]
0031d1e0: ldr      fp, [r4, #0x14]
0031d1e4: ldr      lr, [r4, #0xc]
0031d1e8: str      ip, [sp, #0xe0]
0031d1ec: add      ip, sp, #0x10c
0031d1f0: str      r3, [sp, #8]
0031d1f4: str      lr, [sp, #0xdc]
0031d1f8: str      r7, [sp, #0xd4]
0031d1fc: str      r8, [sp, #0xd0]
0031d200: str      sl, [sp, #0xec]
0031d204: str      sb, [sp, #0xe8]
0031d208: str      fp, [sp, #0xe4]
0031d20c: str      ip, [sp]
0031d210: bl       #0x31b65c ; _ZNSt4priv6__findINS_15_Deque_iteratorIPSt6vectorIN3sfc6script3lua5ValueESaIS6_EESt16_Nonconst_traitsIS9_EEES9_EET_SD_SD_RKT0_RKSt26random_access_iterator_tag
0031d214: ldr      r3, [r4, #0x10]
0031d218: ldr      r2, [sp, #0xf0]
0031d21c: cmp      r3, r2
0031d220: movne    r3, r2
0031d224: beq      #0x31d498
0031d228: ldr      r8, [sp, #0xf4]
0031d22c: ldr      r7, [sp, #0xf8]
0031d230: ldr      sl, [sp, #0xfc]
0031d234: add      fp, r3, #4
0031d238: cmp      r7, fp
0031d23c: ldr      r4, [r5, r6]
0031d240: str      r7, [sp, #0xc8]
0031d244: str      r3, [sp, #0xc0]
0031d248: str      sl, [sp, #0xcc]
0031d24c: str      r8, [sp, #0xc4]
0031d250: ldreq    r8, [sl, #4]!
0031d254: add      ip, sp, #0xa0
0031d258: ldm      r4, {r0, r1, r2, r3}
0031d25c: stm      ip, {r0, r1, r2, r3}
0031d260: mov      r1, ip
0031d264: add      r0, sp, #0xc0
0031d268: addeq    r7, r8, #0x80
0031d26c: moveq    fp, r8
0031d270: bl       #0x31b618 ; _ZNKSt4priv20_Deque_iterator_baseIPSt6vectorIN3sfc6script3lua5ValueESaIS5_EEE11_M_subtractERKS9_
0031d274: add      ip, sp, #0x90
0031d278: mov      sb, r0
0031d27c: ldm      r4, {r0, r1, r2, r3}
0031d280: stm      ip, {r0, r1, r2, r3}
0031d284: mov      r1, ip
0031d288: add      r0, r4, #0x10
0031d28c: bl       #0x31b618 ; _ZNKSt4priv20_Deque_iterator_baseIPSt6vectorIN3sfc6script3lua5ValueESaIS5_EEE11_M_subtractERKS9_
0031d290: cmp      sb, r0, lsr #1
0031d294: bhs      #0x31d394
0031d298: ldr      ip, [r4, #0xc]
0031d29c: ldr      lr, [r4, #8]
0031d2a0: add      r2, sp, #0x60
0031d2a4: str      ip, [sp, #0x7c]
0031d2a8: ldr      ip, [r4, #4]
0031d2ac: add      r3, sp, #0x50
0031d2b0: add      r0, sp, #0x80
0031d2b4: str      ip, [sp, #0x74]
0031d2b8: ldr      ip, [r4]
0031d2bc: add      r1, sp, #0x70
0031d2c0: str      lr, [sp, #0x78]
0031d2c4: str      ip, [sp, #0x70]
0031d2c8: ldr      ip, [sp, #0xcc]
0031d2cc: str      sl, [sp, #0x5c]
0031d2d0: str      r7, [sp, #0x58]
0031d2d4: str      ip, [sp, #0x6c]
0031d2d8: ldr      ip, [sp, #0xc8]
0031d2dc: str      r8, [sp, #0x54]
0031d2e0: str      fp, [sp, #0x50]
0031d2e4: str      ip, [sp, #0x68]
0031d2e8: ldr      ip, [sp, #0xc4]
0031d2ec: str      ip, [sp, #0x64]
0031d2f0: ldr      ip, [sp, #0xc0]
0031d2f4: str      ip, [sp, #0x60]
0031d2f8: add      ip, sp, #0x104
0031d2fc: str      ip, [sp]
0031d300: mov      ip, #0
0031d304: str      ip, [sp, #4]
0031d308: bl       #0x31b8dc ; _ZNSt4priv15__copy_backwardINS_15_Deque_iteratorIPSt6vectorIN3sfc6script3lua5ValueESaIS6_EESt16_Nonconst_traitsIS9_EEESC_iEET0_T_SE_SD_RKSt26random_access_iterator_tagPT1_
0031d30c: ldr      r2, [r4, #8]
0031d310: ldr      r3, [r4]
0031d314: sub      r2, r2, #4
0031d318: cmp      r3, r2
0031d31c: addne    r3, r3, #4
0031d320: strne    r3, [r4]
0031d324: beq      #0x31d45c
0031d328: ldr      r3, [r5, r6]
0031d32c: add      ip, sp, #0xb0
0031d330: ldm      r3, {r0, r1, r2, r3}
0031d334: stm      ip, {r0, r1, r2, r3}
0031d338: mov      r0, ip
0031d33c: mov      r1, sb
0031d340: bl       #0x31bad0 ; _ZNSt4priv20_Deque_iterator_baseIPSt6vectorIN3sfc6script3lua5ValueESaIS5_EEE10_M_advanceEi
0031d344: ldr      r3, [pc, #0x210]
0031d348: ldr      r0, [r5, r3]
0031d34c: ldr      r2, [r0, #0x18]
0031d350: ldr      r3, [r0, #0x10]
0031d354: sub      r2, r2, #4
0031d358: cmp      r3, r2
0031d35c: beq      #0x31d548
0031d360: ldr      r2, [sp, #0xc]
0031d364: str      r2, [r3]
0031d368: ldr      r3, [r0, #0x10]
0031d36c: add      r3, r3, #4
0031d370: str      r3, [r0, #0x10]
0031d374: ldr      r0, [sp, #0xc]
0031d378: ldm      r0, {r1, r2}
0031d37c: cmp      r1, r2
0031d380: beq      #0x31d38c
0031d384: add      r3, sp, #0x108
0031d388: bl       #0x31c3cc ; _ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE8_M_eraseEPS3_S6_RKSt12__false_type
0031d38c: add      sp, sp, #0x114
0031d390: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031d394: ldr      ip, [r4, #0x1c]
0031d398: ldr      lr, [r4, #0x18]
0031d39c: add      r0, sp, #0x40
0031d3a0: str      ip, [sp, #0x2c]
0031d3a4: ldr      ip, [r4, #0x14]
0031d3a8: add      r3, sp, #0x10
0031d3ac: add      r1, sp, #0x30
0031d3b0: str      ip, [sp, #0x24]
0031d3b4: ldr      ip, [r4, #0x10]
0031d3b8: add      r2, sp, #0x20
0031d3bc: str      sl, [sp, #0x3c]
0031d3c0: str      ip, [sp, #0x20]
0031d3c4: ldr      ip, [sp, #0xcc]
0031d3c8: str      r7, [sp, #0x38]
0031d3cc: str      r8, [sp, #0x34]
0031d3d0: str      ip, [sp, #0x1c]
0031d3d4: ldr      ip, [sp, #0xc8]
0031d3d8: str      fp, [sp, #0x30]
0031d3dc: str      lr, [sp, #0x28]
0031d3e0: str      ip, [sp, #0x18]
0031d3e4: ldr      ip, [sp, #0xc4]
0031d3e8: str      ip, [sp, #0x14]
0031d3ec: ldr      ip, [sp, #0xc0]
0031d3f0: str      ip, [sp, #0x10]
0031d3f4: add      ip, sp, #0x100
0031d3f8: str      ip, [sp]
0031d3fc: mov      ip, #0
0031d400: str      ip, [sp, #4]
0031d404: bl       #0x31b9d8 ; _ZNSt4priv6__copyINS_15_Deque_iteratorIPSt6vectorIN3sfc6script3lua5ValueESaIS6_EESt16_Nonconst_traitsIS9_EEESC_iEET0_T_SE_SD_RKSt26random_access_iterator_tagPT1_
0031d408: ldr      r0, [r4, #0x10]
0031d40c: ldr      r3, [r4, #0x14]
0031d410: cmp      r0, r3
0031d414: subne    r0, r0, #4
0031d418: strne    r0, [r4, #0x10]
0031d41c: bne      #0x31d328
0031d420: cmp      r0, #0
0031d424: beq      #0x31d430
0031d428: mov      r1, #0x80
0031d42c: bl       #0x31bb44 ; _ZNSt12__node_alloc10deallocateEPvj
0031d430: ldr      r3, [r5, r6]
0031d434: ldr      r2, [r3, #0x1c]
0031d438: sub      r1, r2, #4
0031d43c: str      r1, [r3, #0x1c]
0031d440: ldr      r2, [r2, #-4]
0031d444: add      r0, r2, #0x7c
0031d448: add      r1, r2, #0x80
0031d44c: str      r0, [r3, #0x10]
0031d450: str      r1, [r3, #0x18]
0031d454: str      r2, [r3, #0x14]
0031d458: b        #0x31d328
0031d45c: ldr      r0, [r4, #4]
0031d460: cmp      r0, #0
0031d464: beq      #0x31d470
0031d468: mov      r1, #0x80
0031d46c: bl       #0x708f00 ; 
0031d470: ldr      r3, [r5, r6]
0031d474: ldr      r2, [r3, #0xc]
0031d478: add      r1, r2, #4
0031d47c: str      r1, [r3, #0xc]
0031d480: ldr      r2, [r2, #4]
0031d484: add      r1, r2, #0x80
0031d488: str      r2, [r3]
0031d48c: str      r1, [r3, #8]
0031d490: str      r2, [r3, #4]
0031d494: b        #0x31d328
0031d498: ldr      r2, [pc, #0xc0]
0031d49c: ldr      r2, [r5, r2]
0031d4a0: ldr      r2, [r2]
0031d4a4: cmp      r2, #2
0031d4a8: moveq    r2, #0
0031d4ac: streq    r2, [r2]
0031d4b0: beq      #0x31d228
0031d4b4: cmp      r2, #1
0031d4b8: bne      #0x31d228
0031d4bc: ldr      r0, [pc, #0xa0]
0031d4c0: ldr      r1, [pc, #0xa0]
0031d4c4: ldr      r2, [pc, #0xa0]
0031d4c8: ldr      r0, [r5, r0]
0031d4cc: ldr      r3, [pc, #0x9c]
0031d4d0: mov      ip, #0x42
0031d4d4: add      r1, pc, r1
0031d4d8: add      r3, pc, r3
0031d4dc: add      r0, r0, #0xa8
0031d4e0: add      r2, pc, r2
0031d4e4: str      ip, [sp]
0031d4e8: bl       #0x30e004 ; 
0031d4ec: ldr      r3, [sp, #0xf0]
0031d4f0: b        #0x31d228
0031d4f4: ldr      r2, [pc, #0x64]
0031d4f8: ldr      r2, [r5, r2]
0031d4fc: ldr      r2, [r2]
0031d500: cmp      r2, #2
0031d504: streq    r3, [r3]
0031d508: beq      #0x31d1b0
0031d50c: cmp      r2, #1
0031d510: bne      #0x31d1b0
0031d514: ldr      r0, [pc, #0x48]
0031d518: ldr      r1, [pc, #0x54]
0031d51c: ldr      r2, [pc, #0x54]
0031d520: ldr      r0, [r5, r0]
0031d524: ldr      r3, [pc, #0x50]
0031d528: mov      ip, #0x3e
0031d52c: add      r1, pc, r1
0031d530: add      r2, pc, r2
0031d534: add      r3, pc, r3
0031d538: add      r0, r0, #0xa8
0031d53c: str      ip, [sp]
0031d540: bl       #0x30e004 ; 
0031d544: b        #0x31d1b0
0031d548: ldr      r1, [sp, #8]
0031d54c: bl       #0x31cd00 ; _ZNSt5dequeIPSt6vectorIN3sfc6script3lua5ValueESaIS4_EESaIS7_EE18_M_push_back_aux_vERKS7_
0031d550: b        #0x31d374
0031d554: rsbeq    r7, r7, r8, ror #17
0031d558: strheq   r0, [r0], -r4
0031d55c: andeq    r0, r0, r0, lsr #19
0031d560: andeq    r3, r0, r0, asr #19
0031d564: andeq    r1, r0, r0, asr #19
0031d568: subseq   r0, sl, r4, lsl #30
0031d56c: subseq   r1, sl, r0, lsr #9
0031d570: subseq   r1, sl, r8, asr r4
0031d574: subseq   r0, sl, ip, lsr #29
0031d578: subseq   r1, sl, r8, asr #8
0031d57c: ldrsheq  r1, [sl], #-0x3c

# _ZNK9Character10IsSneakingEv
003bc690: add      r1, r0, #0xff0
003bc694: push     {r4, lr}
003bc698: add      r1, r1, #4
003bc69c: mov      r2, #0xc6
003bc6a0: add      r0, r0, #0x560
003bc6a4: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003bc6a8: cmp      r0, #0
003bc6ac: movle    r0, #0
003bc6b0: movgt    r0, #1
003bc6b4: pop      {r4, pc}

# _ZN6CharAI18SetSkillsAndSpellsEv
003ce044: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ce048: ldr      r6, [pc, #0x714]
003ce04c: ldr      r2, [pc, #0x714]
003ce050: sub      sp, sp, #0xa4
003ce054: add      r6, pc, r6
003ce058: str      r2, [sp, #0x14]
003ce05c: ldr      r2, [r6, r2]
003ce060: ldr      r3, [r0, #0x1c]
003ce064: mov      r4, r0
003ce068: ldr      r2, [r2]
003ce06c: cmp      r3, #0
003ce070: str      r2, [sp, #0x9c]
003ce074: beq      #0x3ce6ac
003ce078: ldr      fp, [pc, #0x6ec]
003ce07c: add      r5, sp, #0x84
003ce080: ldr      r7, [r6, fp]
003ce084: mov      r0, r7
003ce088: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003ce08c: ldr      r1, [pc, #0x6dc]
003ce090: add      r2, sp, #0x50
003ce094: mov      r0, r5
003ce098: add      r1, pc, r1
003ce09c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003ce0a0: mov      r0, r7
003ce0a4: mov      r1, r5
003ce0a8: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003ce0ac: ldr      r0, [sp, #0x98]
003ce0b0: cmp      r0, r5
003ce0b4: beq      #0x3ce0d4
003ce0b8: cmp      r0, #0
003ce0bc: beq      #0x3ce0d4
003ce0c0: ldr      r1, [sp, #0x84]
003ce0c4: rsb      r1, r0, r1
003ce0c8: cmp      r1, #0x80
003ce0cc: bhi      #0x3ce694
003ce0d0: bl       #0x708f00 ; 
003ce0d4: ldr      r3, [r4, #0x1c]
003ce0d8: add      r8, sp, #0x6c
003ce0dc: str      r8, [sp, #0x7c]
003ce0e0: str      r8, [sp, #0x80]
003ce0e4: ldr      r2, [r3, #0x78]
003ce0e8: ldr      r1, [r3, #0x7c]
003ce0ec: mov      r0, r8
003ce0f0: bl       #0x3116e8 ; _ZNSs19_M_range_initializeEPKcS0_
003ce0f4: ldr      r1, [pc, #0x678]
003ce0f8: ldr      r0, [r4, #0x1c]
003ce0fc: add      r1, pc, r1
003ce100: add      r0, r0, #0x68
003ce104: add      r2, r1, #0x14
003ce108: bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
003ce10c: ldr      r5, [r4, #0xb8]
003ce110: ldr      r3, [r4, #0xb4]
003ce114: rsb      r5, r3, r5
003ce118: asrs     r5, r5, #2
003ce11c: beq      #0x3ce448
003ce120: ldr      r5, [r4, #0xc4]
003ce124: ldr      r3, [r4, #0xc0]
003ce128: rsb      r5, r3, r5
003ce12c: asrs     r5, r5, #2
003ce130: beq      #0x3ce204
003ce134: ldr      r0, [r4, #0x1c]
003ce138: add      r0, r0, #0x68
003ce13c: cmp      r0, r8
003ce140: beq      #0x3ce150
003ce144: ldr      r1, [sp, #0x80]
003ce148: ldr      r2, [sp, #0x7c]
003ce14c: bl       #0x3109e0 ; _ZNSs9_M_assignEPKcS0_
003ce150: ldr      r7, [r6, fp]
003ce154: add      r5, sp, #0x54
003ce158: mov      r0, r7
003ce15c: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003ce160: ldr      r1, [pc, #0x610]
003ce164: add      r2, sp, #0x4c
003ce168: mov      r0, r5
003ce16c: add      r1, pc, r1
003ce170: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003ce174: mov      r0, r7
003ce178: mov      r1, r5
003ce17c: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003ce180: ldr      r0, [sp, #0x68]
003ce184: cmp      r0, r5
003ce188: beq      #0x3ce1a8
003ce18c: cmp      r0, #0
003ce190: beq      #0x3ce1a8
003ce194: ldr      r1, [sp, #0x54]
003ce198: rsb      r1, r0, r1
003ce19c: cmp      r1, #0x80
003ce1a0: bhi      #0x3ce69c
003ce1a4: bl       #0x708f00 ; 
003ce1a8: ldr      r3, [r4, #0x1c]
003ce1ac: mov      r0, r3
003ce1b0: ldr      r3, [r3]
003ce1b4: mov      lr, pc
003ce1b8: ldr      pc, [r3, #0xcc]
003ce1bc: ldr      r0, [sp, #0x80]
003ce1c0: cmp      r0, r8
003ce1c4: beq      #0x3ce1e4
003ce1c8: cmp      r0, #0
003ce1cc: beq      #0x3ce1e4
003ce1d0: ldr      r1, [sp, #0x6c]
003ce1d4: rsb      r1, r0, r1
003ce1d8: cmp      r1, #0x80
003ce1dc: bhi      #0x3ce6a4
003ce1e0: bl       #0x708f00 ; 
003ce1e4: ldr      r2, [sp, #0x14]
003ce1e8: ldr      r3, [r6, r2]
003ce1ec: ldr      r2, [sp, #0x9c]
003ce1f0: ldr      r3, [r3]
003ce1f4: cmp      r2, r3
003ce1f8: bne      #0x3ce760
003ce1fc: add      sp, sp, #0xa4
003ce200: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ce204: add      r2, r4, #0xc0
003ce208: ldr      r0, [r4, #4]
003ce20c: str      r2, [sp, #0x18]
003ce210: bl       #0x3ae5dc ; _ZNK9Character16GetCharFaeryListEv
003ce214: add      sb, sp, #0x2c
003ce218: ldr      r1, [r0, #4]
003ce21c: mov      r7, r0
003ce220: ldr      r0, [sp, #0x18]
003ce224: bl       #0x3cd584 ; _ZNSt6vectorIP17CharAISkillScriptSaIS1_EE7reserveEj
003ce228: mov      r0, sb
003ce22c: bl       #0x3192b4 ; _ZN3sfc6script3lua9ArgumentsC1Ev
003ce230: ldr      r1, [pc, #0x544]
003ce234: mov      r0, sb
003ce238: add      r1, pc, r1
003ce23c: bl       #0x39ec10 ; _ZN3sfc6script3lua9Arguments10pushStringEPKc
003ce240: mov      r0, sb
003ce244: mvn      r1, #0
003ce248: bl       #0x3cdd78 ; _ZN3sfc6script3lua9Arguments11pushIntegerEi
003ce24c: ldr      r3, [r7, #4]
003ce250: cmp      r3, #0
003ce254: beq      #0x3ce414
003ce258: ldr      r3, [pc, #0x520]
003ce25c: str      r6, [sp, #0x24]
003ce260: mov      sl, r8
003ce264: str      r3, [sp, #0x20]
003ce268: ldr      r3, [pc, #0x514]
003ce26c: add      r3, pc, r3
003ce270: str      r3, [sp, #8]
003ce274: ldr      r3, [pc, #0x50c]
003ce278: add      r3, pc, r3
003ce27c: str      r3, [sp, #0xc]
003ce280: ldr      r3, [pc, #0x504]
003ce284: add      r3, pc, r3
003ce288: str      r3, [sp, #0x10]
003ce28c: ldr      r3, [pc, #0x4fc]
003ce290: add      r3, pc, r3
003ce294: str      r3, [sp, #0x1c]
003ce298: b        #0x3ce3bc
003ce29c: ldr      r0, [r4, #0x1c]
003ce2a0: ldr      r1, [sp, #8]
003ce2a4: bl       #0x37b574 ; _ZN9LuaScript4LoadEPKc
003ce2a8: ldr      r6, [sp, #0x30]
003ce2ac: ldm      r6, {r0, r3}
003ce2b0: rsb      r3, r0, r3
003ce2b4: asr      r3, r3, #4
003ce2b8: add      r2, r3, r3, lsl #3
003ce2bc: add      r2, r2, r2, lsl #6
003ce2c0: add      r2, r3, r2, lsl #3
003ce2c4: add      r2, r2, r2, lsl #15
003ce2c8: add      r2, r3, r2, lsl #3
003ce2cc: cmp      r2, #0
003ce2d0: bne      #0x3ce2e4
003ce2d4: ldr      r2, [sp, #0x20]
003ce2d8: add      r0, pc, r2
003ce2dc: bl       #0x708eb0 ; 
003ce2e0: ldr      r0, [r6]
003ce2e4: ldr      r1, [r8, #0x18]
003ce2e8: bl       #0x31c46c ; _ZN3sfc6script3lua5Value9setStringEPKc
003ce2ec: ldr      r6, [sp, #0x30]
003ce2f0: ldm      r6, {r0, r3}
003ce2f4: rsb      r3, r0, r3
003ce2f8: asr      r3, r3, #4
003ce2fc: add      r2, r3, r3, lsl #3
003ce300: add      r2, r2, r2, lsl #6
003ce304: add      r2, r3, r2, lsl #3
003ce308: add      r2, r2, r2, lsl #15
003ce30c: add      r2, r3, r2, lsl #3
003ce310: rsb      r2, r2, #0
003ce314: cmp      r2, #1
003ce318: bhi      #0x3ce328
003ce31c: ldr      r0, [sp, #0x1c]
003ce320: bl       #0x708eb0 ; 
003ce324: ldr      r0, [r6]
003ce328: mov      r1, #0xbf000000
003ce32c: add      r0, r0, #0x70
003ce330: add      r1, r1, #0x800000
003ce334: bl       #0x31b5e8 ; _ZN3sfc6script3lua5Value9setNumberEf
003ce338: ldr      r0, [r4, #0x1c]
003ce33c: ldr      r1, [sp, #0xc]
003ce340: mov      r2, sb
003ce344: bl       #0x37c41c ; _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE
003ce348: ldr      r0, [r4, #0x1c]
003ce34c: ldr      r1, [r8, #0x18]
003ce350: bl       #0x37b574 ; _ZN9LuaScript4LoadEPKc
003ce354: cmp      r0, #0
003ce358: beq      #0x3ce420
003ce35c: mov      r1, #0
003ce360: mov      r0, #0x1c
003ce364: bl       #0x310570 ; _Znwj15MemoryHintState
003ce368: ldr      r1, [r4, #4]
003ce36c: mvn      r3, #0
003ce370: ldr      r2, [r8, #0x18]
003ce374: mov      r6, r0
003ce378: bl       #0x3cde2c ; _ZN17CharAISkillScriptC1EP9CharacterPKcj
003ce37c: ldr      r1, [r4, #0xc4]
003ce380: ldr      r3, [r4, #0xc8]
003ce384: str      r6, [sp, #0x3c]
003ce388: cmp      r1, r3
003ce38c: beq      #0x3ce730
003ce390: str      r6, [r1]
003ce394: ldr      r3, [r4, #0xc4]
003ce398: add      r3, r3, #4
003ce39c: str      r3, [r4, #0xc4]
003ce3a0: ldr      r0, [r4, #0x1c]
003ce3a4: ldr      r1, [sp, #0x10]
003ce3a8: bl       #0x37c514 ; _ZN9LuaScript4CallEPKc
003ce3ac: ldr      r3, [r7, #4]
003ce3b0: add      r5, r5, #1
003ce3b4: cmp      r3, r5
003ce3b8: bls      #0x3ce40c
003ce3bc: ldr      r0, [r4, #4]
003ce3c0: mov      r1, r5
003ce3c4: bl       #0x3aeac0 ; _ZNK9Character12GetCharFaeryEi
003ce3c8: ldr      r3, [r0, #0x14]
003ce3cc: mov      r8, r0
003ce3d0: cmp      r3, #0
003ce3d4: bne      #0x3ce29c
003ce3d8: ldr      r1, [r4, #0xc4]
003ce3dc: ldr      r2, [r4, #0xc8]
003ce3e0: str      r3, [sp, #0x34]
003ce3e4: cmp      r1, r2
003ce3e8: beq      #0x3ce700
003ce3ec: str      r3, [r1]
003ce3f0: ldr      r3, [r4, #0xc4]
003ce3f4: add      r5, r5, #1
003ce3f8: add      r3, r3, #4
003ce3fc: str      r3, [r4, #0xc4]
003ce400: ldr      r3, [r7, #4]
003ce404: cmp      r3, r5
003ce408: bhi      #0x3ce3bc
003ce40c: ldr      r6, [sp, #0x24]
003ce410: mov      r8, sl
003ce414: mov      r0, sb
003ce418: bl       #0x319228 ; _ZN3sfc6script3lua9ArgumentsD1Ev
003ce41c: b        #0x3ce134
003ce420: ldr      r1, [r4, #0xc4]
003ce424: ldr      r3, [r4, #0xc8]
003ce428: str      r0, [sp, #0x38]
003ce42c: cmp      r1, r3
003ce430: beq      #0x3ce750
003ce434: str      r0, [r1]
003ce438: ldr      r3, [r4, #0xc4]
003ce43c: add      r3, r3, #4
003ce440: str      r3, [r4, #0xc4]
003ce444: b        #0x3ce3a0
003ce448: add      r3, r4, #0xb4
003ce44c: ldr      r0, [r4, #4]
003ce450: str      r3, [sp, #0x18]
003ce454: bl       #0x3bc5fc ; _ZNK9Character16GetCharSkillListEv
003ce458: add      sb, sp, #0x2c
003ce45c: ldr      r1, [r0, #4]
003ce460: mov      r7, r0
003ce464: ldr      r0, [sp, #0x18]
003ce468: bl       #0x3cd584 ; _ZNSt6vectorIP17CharAISkillScriptSaIS1_EE7reserveEj
003ce46c: mov      r0, sb
003ce470: bl       #0x3192b4 ; _ZN3sfc6script3lua9ArgumentsC1Ev
003ce474: ldr      r1, [pc, #0x318]
003ce478: mov      r0, sb
003ce47c: add      r1, pc, r1
003ce480: bl       #0x39ec10 ; _ZN3sfc6script3lua9Arguments10pushStringEPKc
003ce484: mov      r0, sb
003ce488: mvn      r1, #0
003ce48c: bl       #0x3cdd78 ; _ZN3sfc6script3lua9Arguments11pushIntegerEi
003ce490: ldr      r3, [r7, #4]
003ce494: cmp      r3, #0
003ce498: beq      #0x3ce660
003ce49c: ldr      r3, [pc, #0x2f4]
003ce4a0: ldr      r2, [pc, #0x2f4]
003ce4a4: str      r6, [sp, #0x24]
003ce4a8: add      r3, pc, r3
003ce4ac: str      r3, [sp, #8]
003ce4b0: ldr      r3, [pc, #0x2e8]
003ce4b4: str      r2, [sp, #0x20]
003ce4b8: mov      sl, r8
003ce4bc: add      r3, pc, r3
003ce4c0: str      r3, [sp, #0xc]
003ce4c4: ldr      r3, [pc, #0x2d8]
003ce4c8: add      r3, pc, r3
003ce4cc: str      r3, [sp, #0x10]
003ce4d0: ldr      r3, [pc, #0x2d0]
003ce4d4: add      r3, pc, r3
003ce4d8: str      r3, [sp, #0x1c]
003ce4dc: b        #0x3ce608
003ce4e0: ldr      r0, [r4, #0x1c]
003ce4e4: ldr      r1, [sp, #8]
003ce4e8: bl       #0x37b574 ; _ZN9LuaScript4LoadEPKc
003ce4ec: ldr      r6, [sp, #0x30]
003ce4f0: ldm      r6, {r0, r3}
003ce4f4: rsb      r3, r0, r3
003ce4f8: asr      r3, r3, #4
003ce4fc: add      r2, r3, r3, lsl #3
003ce500: add      r2, r2, r2, lsl #6
003ce504: add      r2, r3, r2, lsl #3
003ce508: add      r2, r2, r2, lsl #15
003ce50c: add      r2, r3, r2, lsl #3
003ce510: cmp      r2, #0
003ce514: bne      #0x3ce528
003ce518: ldr      r3, [sp, #0x20]
003ce51c: add      r0, pc, r3
003ce520: bl       #0x708eb0 ; 
003ce524: ldr      r0, [r6]
003ce528: ldr      r1, [r8, #0x28]
003ce52c: bl       #0x31c46c ; _ZN3sfc6script3lua5Value9setStringEPKc
003ce530: ldr      r6, [sp, #0x30]
003ce534: ldm      r6, {r2, r3}
003ce538: rsb      r3, r2, r3
003ce53c: asr      r3, r3, #4
003ce540: add      r1, r3, r3, lsl #3
003ce544: add      r1, r1, r1, lsl #6
003ce548: add      r1, r3, r1, lsl #3
003ce54c: add      r1, r1, r1, lsl #15
003ce550: add      r1, r3, r1, lsl #3
003ce554: rsb      r1, r1, #0
003ce558: cmp      r1, #1
003ce55c: bhi      #0x3ce56c
003ce560: ldr      r0, [sp, #0x1c]
003ce564: bl       #0x708eb0 ; 
003ce568: ldr      r2, [r6]
003ce56c: mov      r0, r5
003ce570: add      r6, r2, #0x70
003ce574: bl       #0x30e964 ; 
003ce578: mov      r1, r0
003ce57c: mov      r0, r6
003ce580: bl       #0x31b5e8 ; _ZN3sfc6script3lua5Value9setNumberEf
003ce584: ldr      r0, [r4, #0x1c]
003ce588: ldr      r1, [sp, #0xc]
003ce58c: mov      r2, sb
003ce590: bl       #0x37c41c ; _ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE
003ce594: ldr      r0, [r4, #0x1c]
003ce598: ldr      r1, [r8, #0x28]
003ce59c: bl       #0x37b574 ; _ZN9LuaScript4LoadEPKc
003ce5a0: cmp      r0, #0
003ce5a4: beq      #0x3ce66c
003ce5a8: mov      r1, #0
003ce5ac: mov      r0, #0x1c
003ce5b0: bl       #0x310570 ; _Znwj15MemoryHintState
003ce5b4: ldr      r1, [r4, #4]
003ce5b8: mov      r3, r5
003ce5bc: ldr      r2, [r8, #0x28]
003ce5c0: mov      r6, r0
003ce5c4: bl       #0x3cde2c ; _ZN17CharAISkillScriptC1EP9CharacterPKcj
003ce5c8: ldr      r1, [r4, #0xb8]
003ce5cc: ldr      r3, [r4, #0xbc]
003ce5d0: str      r6, [sp, #0x48]
003ce5d4: cmp      r1, r3
003ce5d8: beq      #0x3ce740
003ce5dc: str      r6, [r1]
003ce5e0: ldr      r3, [r4, #0xb8]
003ce5e4: add      r3, r3, #4
003ce5e8: str      r3, [r4, #0xb8]
003ce5ec: ldr      r0, [r4, #0x1c]
003ce5f0: ldr      r1, [sp, #0x10]
003ce5f4: bl       #0x37c514 ; _ZN9LuaScript4CallEPKc
003ce5f8: ldr      r3, [r7, #4]
003ce5fc: add      r5, r5, #1
003ce600: cmp      r3, r5
003ce604: bls      #0x3ce658
003ce608: ldr      r0, [r4, #4]
003ce60c: mov      r1, r5
003ce610: bl       #0x3bc784 ; _ZNK9Character12GetCharSkillEi
003ce614: ldr      r3, [r0, #0x24]
003ce618: mov      r8, r0
003ce61c: cmp      r3, #0
003ce620: bne      #0x3ce4e0
003ce624: ldr      r1, [r4, #0xb8]
003ce628: ldr      r2, [r4, #0xbc]
003ce62c: str      r3, [sp, #0x40]
003ce630: cmp      r1, r2
003ce634: beq      #0x3ce710
003ce638: str      r3, [r1]
003ce63c: ldr      r3, [r4, #0xb8]
003ce640: add      r5, r5, #1
003ce644: add      r3, r3, #4
003ce648: str      r3, [r4, #0xb8]
003ce64c: ldr      r3, [r7, #4]
003ce650: cmp      r3, r5
003ce654: bhi      #0x3ce608
003ce658: ldr      r6, [sp, #0x24]
003ce65c: mov      r8, sl
003ce660: mov      r0, sb
003ce664: bl       #0x319228 ; _ZN3sfc6script3lua9ArgumentsD1Ev
003ce668: b        #0x3ce120
003ce66c: ldr      r1, [r4, #0xb8]
003ce670: ldr      r3, [r4, #0xbc]
003ce674: str      r0, [sp, #0x44]
003ce678: cmp      r1, r3
003ce67c: beq      #0x3ce720
003ce680: str      r0, [r1]
003ce684: ldr      r3, [r4, #0xb8]
003ce688: add      r3, r3, #4
003ce68c: str      r3, [r4, #0xb8]
003ce690: b        #0x3ce5ec
003ce694: bl       #0x310440 ; _Z10CustomFreePv
003ce698: b        #0x3ce0d4
003ce69c: bl       #0x310440 ; _Z10CustomFreePv
003ce6a0: b        #0x3ce1a8
003ce6a4: bl       #0x310440 ; _Z10CustomFreePv
003ce6a8: b        #0x3ce1e4
003ce6ac: ldr      r2, [pc, #0xf8]
003ce6b0: ldr      r2, [r6, r2]
003ce6b4: ldr      r2, [r2]
003ce6b8: cmp      r2, #2
003ce6bc: streq    r3, [r3]
003ce6c0: beq      #0x3ce078
003ce6c4: cmp      r2, #1
003ce6c8: bne      #0x3ce078
003ce6cc: ldr      r0, [pc, #0xdc]
003ce6d0: ldr      r1, [pc, #0xdc]
003ce6d4: ldr      r2, [pc, #0xdc]
003ce6d8: ldr      r0, [r6, r0]
003ce6dc: ldr      r3, [pc, #0xd8]
003ce6e0: mov      ip, #0x294
003ce6e4: add      r1, pc, r1
003ce6e8: add      r2, pc, r2
003ce6ec: add      r3, pc, r3
003ce6f0: add      r0, r0, #0xa8
003ce6f4: str      ip, [sp]
003ce6f8: bl       #0x30e004 ; 
003ce6fc: b        #0x3ce078
003ce700: ldr      r0, [sp, #0x18]
003ce704: add      r2, sp, #0x34
003ce708: bl       #0x3cd89c ; 
003ce70c: b        #0x3ce3ac
003ce710: ldr      r0, [sp, #0x18]
003ce714: add      r2, sp, #0x40
003ce718: bl       #0x3cd89c ; 
003ce71c: b        #0x3ce5f8
003ce720: ldr      r0, [sp, #0x18]
003ce724: add      r2, sp, #0x44
003ce728: bl       #0x3cd89c ; 
003ce72c: b        #0x3ce5ec
003ce730: ldr      r0, [sp, #0x18]
003ce734: add      r2, sp, #0x3c
003ce738: bl       #0x3cd89c ; 
003ce73c: b        #0x3ce3a0
003ce740: ldr      r0, [sp, #0x18]
003ce744: add      r2, sp, #0x48
003ce748: bl       #0x3cd89c ; 
003ce74c: b        #0x3ce5ec
003ce750: ldr      r0, [sp, #0x18]
003ce754: add      r2, sp, #0x38
003ce758: bl       #0x3cd89c ; 
003ce75c: b        #0x3ce3a0
003ce760: bl       #0x30e310 ; 
003ce764: subseq   r6, ip, ip, lsr sl
003ce768: andeq    r4, r0, ip, lsr #1
003ce76c: andeq    r0, r0, r4, lsl #17
003ce770: subeq    r7, pc, r0, asr #5
003ce774: subeq    r5, pc, ip, ror #10
003ce778: subeq    r7, pc, ip, ror #3
003ce77c: ldrdeq   sp, lr, [pc], #-0x50
003ce780: umaaleq  r0, pc, r0, r1
003ce784: subeq    r6, pc, ip, lsr #31
003ce788: subeq    r7, pc, r8, lsl #2
003ce78c: strdeq   r7, r8, [pc], #-0xc
003ce790: ldrdeq   r0, r1, [pc], #-0x18
003ce794: subeq    sp, pc, ip, lsl #7
003ce798: subeq    r6, pc, r0, ror sp
003ce79c: subeq    pc, lr, ip, asr #30
003ce7a0: subeq    r6, pc, r4, asr #29
003ce7a4: strheq   r6, [pc], #-0xe8
003ce7a8: umaaleq  pc, lr, r4, pc
003ce7ac: andeq    r3, r0, r0, asr #19
003ce7b0: andeq    r1, r0, r0, asr #19

# _ZN17PlayerStatManager6UpdateEv
003790d8: bx       lr

# _ZN17PlayerStatManager13IncrementStatE9EStatTypei
003790ec: mov      r3, r2
003790f0: mov      r2, #1
003790f4: b        #0x3790e0

# _ZN17PlayerStatManager10GetStatStrE9EStatType
003795c8: push     {r4, lr}
003795cc: mov      r4, r0
003795d0: sub      sp, sp, #0x20
003795d4: cmp      r1, #6
003795d8: addls    pc, pc, r1, lsl #2
003795dc: b        #0x379618
003795e0: b        #0x37962c
003795e4: b        #0x379640
003795e8: b        #0x379654
003795ec: b        #0x379668
003795f0: b        #0x37967c
003795f4: b        #0x379690
003795f8: b        #0x3795fc
003795fc: ldr      r1, [pc, #0xa0]
00379600: add      r2, sp, #4
00379604: add      r1, pc, r1
00379608: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0037960c: mov      r0, r4
00379610: add      sp, sp, #0x20
00379614: pop      {r4, pc}
00379618: ldr      r1, [pc, #0x88]
0037961c: mov      r2, sp
00379620: add      r1, pc, r1
00379624: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00379628: b        #0x37960c
0037962c: ldr      r1, [pc, #0x78]
00379630: add      r2, sp, #0x1c
00379634: add      r1, pc, r1
00379638: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0037963c: b        #0x37960c
00379640: ldr      r1, [pc, #0x68]
00379644: add      r2, sp, #0x18
00379648: add      r1, pc, r1
0037964c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00379650: b        #0x37960c
00379654: ldr      r1, [pc, #0x58]
00379658: add      r2, sp, #0x14
0037965c: add      r1, pc, r1
00379660: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00379664: b        #0x37960c
00379668: ldr      r1, [pc, #0x48]
0037966c: add      r2, sp, #0x10
00379670: add      r1, pc, r1
00379674: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
00379678: b        #0x37960c
0037967c: ldr      r1, [pc, #0x38]
00379680: add      r2, sp, #0xc
00379684: add      r1, pc, r1
00379688: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
0037968c: b        #0x37960c
00379690: ldr      r1, [pc, #0x28]
00379694: add      r2, sp, #8
00379698: add      r1, pc, r1
0037969c: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003796a0: b        #0x37960c
003796a4: subseq   r8, r4, ip, lsl r3
003796a8: subseq   r2, r5, r8, ror #3
003796ac: subseq   r8, r4, ip, lsr #5
003796b0: subseq   r8, r4, r0, lsr #5

# _ZNK9Character27GetEffectiveThreatPerDamageEv
003bd394: add      r1, r0, #0xff0
003bd398: push     {r4, lr}
003bd39c: add      r1, r1, #4
003bd3a0: mov      r2, #0xcc
003bd3a4: add      r0, r0, #0x560
003bd3a8: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003bd3ac: bl       #0x30e964 ; 
003bd3b0: mov      r1, #0x3b800000
003bd3b4: bl       #0x30ed6c ; 
003bd3b8: pop      {r4, pc}

# _ZN17PlayerStatManagerC2Ev
00379084: ldr      r3, [pc, #0x14]
00379088: ldr      r2, [pc, #0x14]
0037908c: add      r3, pc, r3
00379090: ldr      r2, [r3, r2]
00379094: add      r2, r2, #8
00379098: str      r2, [r0]
0037909c: bx       lr
003790a0: rsbeq    fp, r1, r4, lsl #20
003790a4: andeq    r3, r0, r8, lsl #12

# _ZN6CharAI11AI_AddAggroEP9Characterf
003d7c68: push     {r4, r5, r6, r7, lr}
003d7c6c: ldr      r3, [pc, #0x110]
003d7c70: subs     r4, r1, #0
003d7c74: sub      sp, sp, #0xc
003d7c78: mov      r5, r0
003d7c7c: add      r3, pc, r3
003d7c80: mov      r6, r2
003d7c84: beq      #0x3d7d18
003d7c88: ldr      r3, [r5, #0x80]
003d7c8c: add      r0, r5, #0x7c
003d7c90: cmp      r3, #0
003d7c94: beq      #0x3d7d10
003d7c98: mov      r1, r0
003d7c9c: b        #0x3d7ca4
003d7ca0: mov      r3, r2
003d7ca4: ldr      r2, [r3, #0x10]
003d7ca8: cmp      r4, r2
003d7cac: ldrhi    r2, [r3, #0xc]
003d7cb0: ldrls    r2, [r3, #8]
003d7cb4: movhi    r3, r1
003d7cb8: mov      r1, r3
003d7cbc: cmp      r2, #0
003d7cc0: bne      #0x3d7ca0
003d7cc4: cmp      r0, r3
003d7cc8: beq      #0x3d7d6c
003d7ccc: ldr      r2, [r3, #0x10]
003d7cd0: cmp      r4, r2
003d7cd4: blo      #0x3d7d10
003d7cd8: cmp      r0, r3
003d7cdc: beq      #0x3d7d6c
003d7ce0: ldr      r7, [r3, #0x14]
003d7ce4: mov      r0, r6
003d7ce8: mov      r1, r7
003d7cec: bl       #0x30eba4 ; 
003d7cf0: mov      r1, r4
003d7cf4: mov      r2, r0
003d7cf8: mov      r0, r5
003d7cfc: bl       #0x3d79ec ; _ZN6CharAI11AI_SetAggroEP9Characterf
003d7d00: mov      r1, r7
003d7d04: bl       #0x30e3ac ; 
003d7d08: add      sp, sp, #0xc
003d7d0c: pop      {r4, r5, r6, r7, pc}
003d7d10: mov      r3, r0
003d7d14: b        #0x3d7cd8
003d7d18: ldr      r2, [pc, #0x68]
003d7d1c: ldr      r2, [r3, r2]
003d7d20: ldr      r2, [r2]
003d7d24: cmp      r2, #2
003d7d28: streq    r4, [r4]
003d7d2c: beq      #0x3d7c88
003d7d30: cmp      r2, #1
003d7d34: bne      #0x3d7c88
003d7d38: ldr      r0, [pc, #0x4c]
003d7d3c: ldr      r1, [pc, #0x4c]
003d7d40: ldr      r2, [pc, #0x4c]
003d7d44: ldr      r0, [r3, r0]
003d7d48: ldr      r3, [pc, #0x48]
003d7d4c: movw     ip, #0x292
003d7d50: add      r1, pc, r1
003d7d54: add      r2, pc, r2
003d7d58: add      r3, pc, r3
003d7d5c: add      r0, r0, #0xa8
003d7d60: str      ip, [sp]
003d7d64: bl       #0x30e004 ; 
003d7d68: b        #0x3d7c88
003d7d6c: mov      r0, r5
003d7d70: mov      r1, r4
003d7d74: mov      r2, r6
003d7d78: add      sp, sp, #0xc
003d7d7c: pop      {r4, r5, r6, r7, lr}
003d7d80: b        #0x3d79ec
003d7d84: subseq   ip, fp, r4, lsl lr
003d7d88: andeq    r3, r0, r0, asr #19
003d7d8c: andeq    r1, r0, r0, asr #19
003d7d90: subeq    r6, lr, r8, lsl #13
003d7d94: subseq   sl, r1, r4, lsr r3
003d7d98: subeq    sp, lr, r8, ror #16

# _ZN9Character7RegenHPEi
003bdca4: push     {r4, r5, r6, r7, r8, sl, lr}
003bdca8: ldr      r5, [pc, #0xd0]
003bdcac: ldr      r8, [pc, #0xd0]
003bdcb0: add      r6, r0, #0xff0
003bdcb4: add      r5, pc, r5
003bdcb8: ldr      r3, [r5, r8]
003bdcbc: add      r6, r6, #4
003bdcc0: add      r7, r0, #0x560
003bdcc4: ldr      r3, [r3]
003bdcc8: mov      r4, r1
003bdccc: sub      sp, sp, #0x24
003bdcd0: mov      r2, #0x24
003bdcd4: mov      r1, r6
003bdcd8: mov      r0, r7
003bdcdc: str      r3, [sp, #0x1c]
003bdce0: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003bdce4: mov      sl, r0
003bdce8: mov      r1, r6
003bdcec: mov      r0, r7
003bdcf0: mov      r2, #0x26
003bdcf4: bl       #0x3dedb4 ; _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003bdcf8: cmp      r4, #0
003bdcfc: movlt    r4, r0
003bdd00: add      r3, r4, sl
003bdd04: cmp      r3, r0
003bdd08: rsbgt    r4, sl, r0
003bdd0c: cmp      r4, #0
003bdd10: ble      #0x3bdd60
003bdd14: ldr      r3, [pc, #0x6c]
003bdd18: add      r6, sp, #4
003bdd1c: ldr      sl, [r5, r3]
003bdd20: mov      r0, sl
003bdd24: bl       #0x337888 ; _ZN13DebugSwitches4loadEv
003bdd28: ldr      r1, [pc, #0x5c]
003bdd2c: mov      r2, sp
003bdd30: mov      r0, r6
003bdd34: add      r1, pc, r1
003bdd38: bl       #0x3140ec ; _ZNSsC1EPKcRKSaIcE
003bdd3c: mov      r1, r6
003bdd40: mov      r0, sl
003bdd44: bl       #0x337a88 ; _ZN13DebugSwitches9GetSwitchERKSs
003bdd48: mov      r0, r6
003bdd4c: bl       #0x318254 ; _ZNSsD1Ev
003bdd50: mov      r0, r7
003bdd54: mov      r2, r4
003bdd58: mov      r1, #0x24
003bdd5c: bl       #0x3e0708 ; _ZN14CharProperties9PROPS_AddEii
003bdd60: ldr      r3, [r5, r8]
003bdd64: ldr      r2, [sp, #0x1c]
003bdd68: ldr      r3, [r3]
003bdd6c: cmp      r2, r3
003bdd70: bne      #0x3bdd7c
003bdd74: add      sp, sp, #0x24
003bdd78: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdd7c: bl       #0x30e310 ; 
003bdd80: ldrsbeq  r6, [sp], #-0xdc
003bdd84: andeq    r4, r0, ip, lsr #1
003bdd88: andeq    r0, r0, r4, lsl #17
003bdd8c: subseq   r6, r0, ip, asr fp
