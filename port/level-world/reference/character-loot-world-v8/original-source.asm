# 0x3a2fcc _ZNK9Character7GetLootEv
003a2fcc: movw r3, #0x101c
003a2fd0: ldr r0, [r0, r3]
003a2fd4: bx lr

# 0x3a5ae4 _ZNK9Character8DropLootEP10GameObject
003a5ae4: push {r4, r5, lr}
003a5ae8: mov r4, r1
003a5aec: sub sp, sp, #0xc
003a5af0: mov r5, r0
003a5af4: bl #0x3a2fcc
003a5af8: mov ip, #0
003a5afc: mov r1, r5
003a5b00: mov r2, r4
003a5b04: mvn r3, #0
003a5b08: str ip, [sp]
003a5b0c: bl #0x3ecba0
003a5b10: add sp, sp, #0xc
003a5b14: pop {r4, r5, pc}

# 0x3ecba0 _ZN10ItemObject13DropLootTableEiPK10GameObjectS2_ib
003ecba0: push {r4, r5, r6, r7, r8, sl, lr}
003ecba4: subs r5, r2, #0
003ecba8: sub sp, sp, #0x54
003ecbac: mov r8, r0
003ecbb0: mov r6, r1
003ecbb4: mov r7, r3
003ecbb8: moveq r4, r5
003ecbbc: beq #0x3ecbe8
003ecbc0: add r4, sp, #0x44
003ecbc4: mov r0, r4
003ecbc8: mov r1, r5
003ecbcc: bl #0x33dd70
003ecbd0: mov r0, r4
003ecbd4: mov r1, #0
003ecbd8: bl #0x33ff8c
003ecbdc: subs r4, r0, #0
003ecbe0: bne #0x3ecc1c
003ecbe4: mov r4, #0
003ecbe8: cmp r6, #0
003ecbec: beq #0x3ecc14
003ecbf0: add sl, sp, #0x38
003ecbf4: mov r1, r6
003ecbf8: mov r0, sl
003ecbfc: bl #0x33dd70
003ecc00: mov r0, sl
003ecc04: mov r1, #0
003ecc08: bl #0x33ff8c
003ecc0c: subs r3, r0, #0
003ecc10: bne #0x3ecc2c
003ecc14: mov sl, #0
003ecc18: b #0x3ecc9c
003ecc1c: ldr r3, [r4, #0xf4]
003ecc20: cmp r3, #0
003ecc24: beq #0x3ecbe8
003ecc28: b #0x3ecbe4
003ecc2c: ldr r2, [r3, #0xf4]
003ecc30: cmp r2, #0
003ecc34: bne #0x3ecc14
003ecc38: mov sl, r3
003ecc3c: bl #0x3a3158
003ecc40: cmp r0, #0
003ecc44: beq #0x3ecc8c
003ecc48: mov r0, sp
003ecc4c: bl #0x3ff200
003ecc50: mov r0, sp
003ecc54: mov r1, r8
003ecc58: mov r2, r5
003ecc5c: mov r3, r7
003ecc60: bl #0x3ecae4
003ecc64: mov r0, sp
003ecc68: mov r1, r6
003ecc6c: mov r2, r5
003ecc70: mov r3, r7
003ecc74: bl #0x3ec8a0
003ecc78: mov r0, sp
003ecc7c: mov r4, sp
003ecc80: bl #0x3ff460
003ecc84: add sp, sp, #0x54
003ecc88: pop {r4, r5, r6, r7, r8, sl, pc}
003ecc8c: mov r0, sl
003ecc90: bl #0x3a3144
003ecc94: cmp r0, #0
003ecc98: bne #0x3ecc48
003ecc9c: cmp r4, #0
003ecca0: beq #0x3ecc84
003ecca4: ldr r2, [r4]
003ecca8: mov r0, r4
003eccac: mov lr, pc
003eccb0: ldr pc, [r2, #0x28]
003eccb4: cmp r0, #0
003eccb8: bne #0x3ecc48
003eccbc: cmp sl, r4
003eccc0: bne #0x3ecc84
003eccc4: b #0x3ecc48

# 0x3ecae4 _ZN10ItemObject18GetInventoryToDropER13ItemInventoryiPK10GameObjecti
003ecae4: push {r4, r5, r6, r7, r8, sb, sl, lr}
003ecae8: cmp r2, #0
003ecaec: sub sp, sp, #0x18
003ecaf0: mov r6, r0
003ecaf4: mov r5, r1
003ecaf8: mov r4, r3
003ecafc: beq #0x3ecb24
003ecb00: add r7, sp, #0xc
003ecb04: mov r1, r2
003ecb08: mov r0, r7
003ecb0c: bl #0x33dd70
003ecb10: mov r0, r7
003ecb14: mov r1, #0
003ecb18: bl #0x33ff8c
003ecb1c: cmp r0, #0
003ecb20: bne #0x3ecb48
003ecb24: mov ip, #0
003ecb28: mov r2, ip
003ecb2c: mov r0, r6
003ecb30: mov r1, r5
003ecb34: mov r3, ip
003ecb38: stm sp, {r4, ip}
003ecb3c: bl #0x40407c
003ecb40: add sp, sp, #0x18
003ecb44: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003ecb48: ldr r7, [r0, #0xf4]
003ecb4c: cmp r7, #0
003ecb50: bne #0x3ecb24
003ecb54: add r8, r0, #0xff0
003ecb58: add r8, r8, #4
003ecb5c: add sl, r0, #0x560
003ecb60: mov r1, r8
003ecb64: mov r0, sl
003ecb68: mov r2, #0xc3
003ecb6c: bl #0x3dedb4
003ecb70: mov r1, r8
003ecb74: mov sb, r0
003ecb78: mov r2, #0xc4
003ecb7c: mov r0, sl
003ecb80: bl #0x3dedb4
003ecb84: mov r1, r5
003ecb88: mov r3, r0
003ecb8c: mov r2, sb
003ecb90: mov r0, r6
003ecb94: stm sp, {r4, r7}
003ecb98: bl #0x40407c
003ecb9c: b #0x3ecb40

# 0x3ec8a0 _ZN10ItemObject16DropAndAwardLootER13ItemInventoryPK10GameObjectS4_i
003ec8a0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ec8a4: sub sp, sp, #0x24
003ec8a8: mov r4, r1
003ec8ac: mov r7, r2
003ec8b0: mov r5, r0
003ec8b4: bl #0x3fc608
003ec8b8: ldr r6, [pc, #0xa8]
003ec8bc: cmp r0, #0
003ec8c0: add r6, pc, r6
003ec8c4: beq #0x3ec960
003ec8c8: mov r3, #0
003ec8cc: add sl, r4, #0x160
003ec8d0: add r8, sp, #0x14
003ec8d4: str r3, [sp, #0x1c]
003ec8d8: str r3, [sp, #0x14]
003ec8dc: str r3, [sp, #0x18]
003ec8e0: ldr sb, [pc, #0x84]
003ec8e4: ldr fp, [pc, #0x84]
003ec8e8: b #0x3ec944
003ec8ec: bl #0x3ec668
003ec8f0: mov r1, #0
003ec8f4: mov r0, r5
003ec8f8: bl #0x3fc61c
003ec8fc: ldr r3, [r6, sb]
003ec900: mov r1, #0
003ec904: mov r2, #1
003ec908: ldr r0, [r3, #0x40]
003ec90c: bl #0x36e478
003ec910: ldr r3, [r6, fp]
003ec914: str sl, [sp]
003ec918: str r8, [sp, #4]
003ec91c: ldr ip, [r0, #0x660]
003ec920: mov r1, r5
003ec924: mov r0, r3
003ec928: mov r2, #0
003ec92c: mov r3, r4
003ec930: str ip, [sp, #8]
003ec934: bl #0x3eacd0
003ec938: mov r1, r4
003ec93c: mov r2, r7
003ec940: bl #0x3ec474
003ec944: mov r0, r5
003ec948: bl #0x3fc608
003ec94c: cmp r0, #0
003ec950: mov r2, r7
003ec954: mov r1, r4
003ec958: mov r0, r8
003ec95c: bne #0x3ec8ec
003ec960: add sp, sp, #0x24
003ec964: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ec968: ldrsbeq r8, [sl], #-0x10
003ec96c: strdeq r3, r4, [r0], -r4
003ec970: andeq r0, r0, ip, lsr #28

# 0x3ec474 _ZN10ItemObject17_DoAutoPickupHackEPS_PK10GameObjectS3_
003ec474: push {r4, r5, r6, r7, r8, lr}
003ec478: mov r1, #0
003ec47c: mov r4, r0
003ec480: add r0, r0, #0x374
003ec484: mov r5, r2
003ec488: bl #0x3fc61c
003ec48c: ldr r7, [pc, #0x60]
003ec490: cmp r0, #0
003ec494: cmpne r4, #0
003ec498: add r7, pc, r7
003ec49c: bne #0x3ec4a4
003ec4a0: pop {r4, r5, r6, r7, r8, pc}
003ec4a4: bl #0x3f9e34
003ec4a8: ldr r3, [pc, #0x48]
003ec4ac: ldr r1, [pc, #0x48]
003ec4b0: ldr r2, [pc, #0x48]
003ec4b4: ldr r3, [r7, r3]
003ec4b8: mov r6, r0
003ec4bc: add r1, pc, r1
003ec4c0: add r2, pc, r2
003ec4c4: ldr r0, [r3, #0x2c]
003ec4c8: bl #0x4c4bdc
003ec4cc: cmp r6, r0
003ec4d0: bne #0x3ec4a0
003ec4d4: cmp r5, #0
003ec4d8: beq #0x3ec4a0
003ec4dc: mov r0, r4
003ec4e0: mov r1, r5
003ec4e4: ldr r3, [r4]
003ec4e8: mov lr, pc
003ec4ec: ldr pc, [r3, #0x98]
003ec4f0: pop {r4, r5, r6, r7, r8, pc}
003ec4f4: ldrsheq r8, [sl], #-0x58
003ec4f8: strdeq r3, r4, [r0], -r4
003ec4fc: subeq sb, sp, r4, asr #27
003ec500: ldrdeq sb, sl, [sp], #-0xd0

# 0x3ec668 _ZN10ItemObject17_GetRandomDropPosER7Point3DIfEPK10GameObjectS5_
003ec668: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ec66c: ldr r6, [pc, #0x224]
003ec670: subs r7, r2, #0
003ec674: sub sp, sp, #0x1c
003ec678: mov r5, r0
003ec67c: add r6, pc, r6
003ec680: mov r4, r1
003ec684: beq #0x3ec83c
003ec688: ldr r0, [r7, #0x164]
003ec68c: ldr r1, [r1, #0x164]
003ec690: bl #0x30e3ac
003ec694: ldr r1, [r4, #0x168]
003ec698: mov sl, r0
003ec69c: ldr r0, [r7, #0x168]
003ec6a0: bl #0x30e3ac
003ec6a4: ldr r1, [r4, #0x160]
003ec6a8: mov r8, r0
003ec6ac: ldr r0, [r7, #0x160]
003ec6b0: bl #0x30e3ac
003ec6b4: str r0, [sp, #0xc]
003ec6b8: add r0, sp, #0xc
003ec6bc: str sl, [sp, #0x10]
003ec6c0: str r8, [sp, #0x14]
003ec6c4: bl #0x34d0b0
003ec6c8: ldr r3, [pc, #0x1cc]
003ec6cc: mov r0, #0xc8
003ec6d0: ldr fp, [sp, #0xc]
003ec6d4: ldr r3, [r6, r3]
003ec6d8: ldr sl, [sp, #0x10]
003ec6dc: ldr r7, [sp, #0x14]
003ec6e0: ldr sb, [r3]
003ec6e4: ldr r8, [r3, #8]
003ec6e8: ldr r3, [r3, #4]
003ec6ec: str r3, [sp, #4]
003ec6f0: bl #0x3ec5d4
003ec6f4: add r0, r0, #0x96
003ec6f8: bl #0x30e964
003ec6fc: mov r6, r0
003ec700: mov r1, r0
003ec704: ldr r0, [sp, #0xc]
003ec708: bl #0x30ed6c
003ec70c: mov r1, r6
003ec710: str r0, [sp, #0xc]
003ec714: ldr r0, [sp, #0x10]
003ec718: bl #0x30ed6c
003ec71c: mov r1, r6
003ec720: str r0, [sp, #0x10]
003ec724: ldr r0, [sp, #0x14]
003ec728: bl #0x30ed6c
003ec72c: str r0, [sp, #0x14]
003ec730: mov r0, #0x12c
003ec734: bl #0x3ec5d4
003ec738: sub r0, r0, #0x96
003ec73c: bl #0x30e964
003ec740: mov r1, sb
003ec744: mov r6, r0
003ec748: mov r0, r7
003ec74c: bl #0x30ed6c
003ec750: mov r1, fp
003ec754: mov r3, r0
003ec758: mov r0, r8
003ec75c: str r3, [sp]
003ec760: bl #0x30ed6c
003ec764: ldr r3, [sp]
003ec768: mov r1, r0
003ec76c: mov r0, r3
003ec770: bl #0x30e3ac
003ec774: mov r1, r0
003ec778: mov r0, r6
003ec77c: bl #0x30ed6c
003ec780: ldr r1, [sp, #0x10]
003ec784: bl #0x30eba4
003ec788: ldr r1, [r4, #0x164]
003ec78c: bl #0x30eba4
003ec790: mov r1, fp
003ec794: mov r3, r0
003ec798: ldr r0, [sp, #4]
003ec79c: str r3, [sp]
003ec7a0: bl #0x30ed6c
003ec7a4: mov r1, sb
003ec7a8: mov fp, r0
003ec7ac: mov r0, sl
003ec7b0: bl #0x30ed6c
003ec7b4: mov r1, r0
003ec7b8: mov r0, fp
003ec7bc: bl #0x30e3ac
003ec7c0: mov r1, r0
003ec7c4: mov r0, r6
003ec7c8: bl #0x30ed6c
003ec7cc: ldr r1, [sp, #0x14]
003ec7d0: bl #0x30eba4
003ec7d4: ldr r1, [r4, #0x168]
003ec7d8: bl #0x30eba4
003ec7dc: mov r1, r8
003ec7e0: mov sb, r0
003ec7e4: mov r0, sl
003ec7e8: bl #0x30ed6c
003ec7ec: ldr r1, [sp, #4]
003ec7f0: mov r8, r0
003ec7f4: mov r0, r7
003ec7f8: bl #0x30ed6c
003ec7fc: mov r1, r0
003ec800: mov r0, r8
003ec804: bl #0x30e3ac
003ec808: mov r1, r0
003ec80c: mov r0, r6
003ec810: bl #0x30ed6c
003ec814: ldr r1, [sp, #0xc]
003ec818: bl #0x30eba4
003ec81c: ldr r1, [r4, #0x160]
003ec820: bl #0x30eba4
003ec824: str sb, [r5, #8]
003ec828: str r0, [r5]
003ec82c: ldr r3, [sp]
003ec830: str r3, [r5, #4]
003ec834: add sp, sp, #0x1c
003ec838: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ec83c: ldr r6, [r1, #0x160]
003ec840: mov r0, #0x1f4
003ec844: str r6, [r5]
003ec848: ldr r3, [r1, #0x164]
003ec84c: str r3, [r5, #4]
003ec850: ldr r3, [r1, #0x168]
003ec854: str r3, [r5, #8]
003ec858: bl #0x3ec5d4
003ec85c: sub r0, r0, #0xfa
003ec860: bl #0x30e964
003ec864: mov r1, r6
003ec868: bl #0x30eba4
003ec86c: str r0, [r5]
003ec870: mov r0, #0x1f4
003ec874: ldr r4, [r5, #4]
003ec878: bl #0x3ec5d4
003ec87c: sub r0, r0, #0xfa
003ec880: bl #0x30e964
003ec884: mov r1, r0
003ec888: mov r0, r4
003ec88c: bl #0x30eba4
003ec890: str r0, [r5, #4]
003ec894: b #0x3ec834
003ec898: subseq r8, sl, r4, lsl r4
003ec89c: andeq r4, r0, r0, asr #6

# 0x3eac00 _ZN11ItemManagerC1Ev
003eac00: ldr r2, [pc, #0x24]
003eac04: ldr ip, [pc, #0x24]
003eac08: mov r1, #0
003eac0c: add r2, pc, r2
003eac10: ldr ip, [r2, ip]
003eac14: str r1, [r0, #0xc]
003eac18: str r1, [r0, #4]
003eac1c: add ip, ip, #8
003eac20: str ip, [r0]
003eac24: str r1, [r0, #8]
003eac28: bx lr
003eac2c: subseq sb, sl, r4, lsl #29
003eac30: andeq r0, r0, ip, ror #17

# 0x3eb6e4 _ZN11ItemManager8PreCacheEv
003eb6e4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003eb6e8: ldr r1, [pc, #0x3ec]
003eb6ec: ldr r2, [pc, #0x3ec]
003eb6f0: sub sp, sp, #0x9c
003eb6f4: add r1, pc, r1
003eb6f8: ldr r3, [r1, r2]
003eb6fc: str r2, [sp, #0x4c]
003eb700: ldr r2, [pc, #0x3dc]
003eb704: ldr r3, [r3]
003eb708: mov r6, r0
003eb70c: ldr r2, [r1, r2]
003eb710: add sb, sp, #0x70
003eb714: add ip, r6, #4
003eb718: ldr r2, [r2]
003eb71c: mov r0, sb
003eb720: str r1, [sp, #0x1c]
003eb724: str r2, [sp, #0x40]
003eb728: str ip, [sp, #0x44]
003eb72c: str r3, [sp, #0x94]
003eb730: bl #0x33f50c
003eb734: ldr r0, [sp, #0x44]
003eb738: ldr r1, [sp, #0x40]
003eb73c: bl #0x3eb420
003eb740: ldr r0, [sp, #0x40]
003eb744: cmp r0, #0
003eb748: beq #0x3eba88
003eb74c: ldr r3, [pc, #0x394]
003eb750: ldr r1, [pc, #0x394]
003eb754: add fp, sp, #0x50
003eb758: add r3, pc, r3
003eb75c: str r3, [sp, #0x2c]
003eb760: ldr r3, [pc, #0x388]
003eb764: add r2, fp, #4
003eb768: str r1, [sp, #0x24]
003eb76c: add r3, pc, r3
003eb770: str r3, [sp, #0x30]
003eb774: add ip, sp, #0x60
003eb778: mov r3, #0
003eb77c: add r0, sp, #0x80
003eb780: add r1, r2, #4
003eb784: str r2, [sp, #0x20]
003eb788: str r3, [sp, #0x14]
003eb78c: str ip, [sp, #0x34]
003eb790: str r0, [sp, #0x18]
003eb794: str r1, [sp, #0x28]
003eb798: str sb, [sp, #0x10]
003eb79c: mov r3, #0
003eb7a0: ldr r1, [sp, #0x34]
003eb7a4: ldr r0, [sp, #0x44]
003eb7a8: str r3, [sp, #0x6c]
003eb7ac: str r3, [sp, #0x60]
003eb7b0: str r3, [sp, #0x64]
003eb7b4: str r3, [sp, #0x68]
003eb7b8: bl #0x3eb530
003eb7bc: ldr r0, [sp, #0x34]
003eb7c0: bl #0x3eb378
003eb7c4: ldr r2, [sp, #0x14]
003eb7c8: ldr r3, [r6, #4]
003eb7cc: mov r1, #5
003eb7d0: lsl r7, r2, #4
003eb7d4: str r1, [sp, #0x7c]
003eb7d8: add r4, r3, r7
003eb7dc: ldr r2, [r3, r2, lsl #4]
003eb7e0: ldr r0, [r4, #8]
003eb7e4: rsb r0, r2, r0
003eb7e8: asr r0, r0, #3
003eb7ec: cmp r0, #4
003eb7f0: bhi #0x3eb83c
003eb7f4: ldr r3, [r4, #4]
003eb7f8: cmp r2, #0
003eb7fc: rsb r0, r2, r3
003eb800: asr r8, r0, #3
003eb804: beq #0x3ebac4
003eb808: mov r0, r4
003eb80c: add r1, sp, #0x7c
003eb810: bl #0x3eb1c0
003eb814: mov r5, r0
003eb818: mov r0, r4
003eb81c: bl #0x3eb218
003eb820: ldr r3, [sp, #0x7c]
003eb824: add r0, r5, r8, lsl #3
003eb828: str r0, [r4, #4]
003eb82c: add r3, r5, r3, lsl #3
003eb830: str r3, [r4, #8]
003eb834: str r5, [r4]
003eb838: ldr r3, [r6, #4]
003eb83c: add r0, sp, #0x7c
003eb840: mov sl, r7
003eb844: str r0, [sp, #0x3c]
003eb848: add r7, r3, sl
003eb84c: ldr r8, [r7, #4]
003eb850: ldr r2, [r7, #8]
003eb854: mov r4, #0
003eb858: mov r5, r4
003eb85c: cmp r8, r2
003eb860: beq #0x3eb95c
003eb864: str r5, [r8]
003eb868: strb r5, [r8, #4]
003eb86c: ldr r3, [r7, #4]
003eb870: add r3, r3, #8
003eb874: str r3, [r7, #4]
003eb878: ldr r1, [sp, #0x2c]
003eb87c: ldr r2, [sp, #0x14]
003eb880: mov r3, r4
003eb884: ldr r0, [sp, #0x18]
003eb888: bl #0x30eae4
003eb88c: ldr r1, [sp, #0x1c]
003eb890: ldr r0, [sp, #0x24]
003eb894: mov ip, #1
003eb898: ldr r2, [sp, #0x30]
003eb89c: ldr r3, [r1, r0]
003eb8a0: mov r0, fp
003eb8a4: ldr r1, [r3, #0x38]
003eb8a8: ldr r3, [sp, #0x18]
003eb8ac: stm sp, {r5, ip}
003eb8b0: bl #0x34b724
003eb8b4: ldr r2, [fp]
003eb8b8: ldr r1, [sp, #0x20]
003eb8bc: mov r3, sb
003eb8c0: str r2, [r3], #4
003eb8c4: ldr r2, [r1]
003eb8c8: ldr ip, [sp, #0x28]
003eb8cc: ldr r0, [sp, #0x10]
003eb8d0: str r2, [sb, #4]
003eb8d4: ldr r2, [ip]
003eb8d8: mov r1, r5
003eb8dc: mov sb, r0
003eb8e0: str r2, [r3, #4]
003eb8e4: bl #0x33fdc0
003eb8e8: cmp r0, #0
003eb8ec: beq #0x3eb938
003eb8f0: ldr r3, [r6, #4]
003eb8f4: ldr r0, [sp, #0x10]
003eb8f8: mov r1, r5
003eb8fc: ldr r7, [r3, sl]
003eb900: bl #0x33fdc0
003eb904: cmp r0, #0
003eb908: lsl r3, r4, #3
003eb90c: beq #0x3eb91c
003eb910: ldr r2, [r0, #0xf4]
003eb914: cmp r2, #3
003eb918: beq #0x3eb920
003eb91c: mov r0, #0
003eb920: str r0, [r7, r3]
003eb924: ldr r2, [r6, #4]
003eb928: ldr r1, [sp, #0x14]
003eb92c: ldr r2, [r2, sl]
003eb930: ldr r0, [r2, r3]
003eb934: bl #0x3ece80
003eb938: add r4, r4, #1
003eb93c: cmp r4, #5
003eb940: beq #0x3eba48
003eb944: ldr r3, [r6, #4]
003eb948: add r7, r3, sl
003eb94c: ldr r8, [r7, #4]
003eb950: ldr r2, [r7, #8]
003eb954: cmp r8, r2
003eb958: bne #0x3eb864
003eb95c: ldr r2, [r3, sl]
003eb960: rsb r2, r2, r8
003eb964: asr r2, r2, #3
003eb968: cmp r2, #1
003eb96c: addhs r3, r2, r2
003eb970: addlo r3, r2, #1
003eb974: cmn r3, #0xe0000001
003eb978: bhi #0x3ebaac
003eb97c: cmp r2, r3
003eb980: bhi #0x3ebaac
003eb984: mov r1, r3
003eb988: add r0, r7, #8
003eb98c: ldr r2, [sp, #0x3c]
003eb990: str r3, [sp, #0x7c]
003eb994: bl #0x3eaff8
003eb998: str r0, [sp, #0x38]
003eb99c: ldr lr, [r7]
003eb9a0: rsb r8, lr, r8
003eb9a4: asr r8, r8, #3
003eb9a8: cmp r8, #0
003eb9ac: movle r8, r0
003eb9b0: ble #0x3eb9f4
003eb9b4: mov r1, r8
003eb9b8: str r8, [sp, #0x48]
003eb9bc: ldr r8, [sp, #0x38]
003eb9c0: mov r0, #0
003eb9c4: mov r2, lr
003eb9c8: ldr ip, [r2, r0]!
003eb9cc: mov r3, r8
003eb9d0: subs r1, r1, #1
003eb9d4: str ip, [r3, r0]!
003eb9d8: ldrb r2, [r2, #4]
003eb9dc: add r0, r0, #8
003eb9e0: strb r2, [r3, #4]
003eb9e4: bne #0x3eb9c4
003eb9e8: ldr r8, [sp, #0x48]
003eb9ec: ldr r1, [sp, #0x38]
003eb9f0: add r8, r1, r8, lsl #3
003eb9f4: mov r2, r8
003eb9f8: strb r5, [r8, #4]
003eb9fc: str r5, [r2], #8
003eba00: ldr r0, [r7]
003eba04: ldr r1, [r7, #8]
003eba08: cmp r0, #0
003eba0c: beq #0x3eba2c
003eba10: rsb r1, r0, r1
003eba14: bic r1, r1, #7
003eba18: cmp r1, #0x80
003eba1c: bhi #0x3ebab4
003eba20: str r2, [sp, #0xc]
003eba24: bl #0x708f00
003eba28: ldr r2, [sp, #0xc]
003eba2c: ldr r3, [sp, #0x7c]
003eba30: ldr ip, [sp, #0x38]
003eba34: str r2, [r7, #4]
003eba38: add r3, ip, r3, lsl #3
003eba3c: str ip, [r7]
003eba40: str r3, [r7, #8]
003eba44: b #0x3eb878
003eba48: mov r7, sl
003eba4c: mov r4, #0
003eba50: ldr r3, [r6, #4]
003eba54: mov r0, r6
003eba58: ldr r3, [r3, r7]
003eba5c: ldr r1, [r3, r4]
003eba60: add r4, r4, #8
003eba64: bl #0x3eac34
003eba68: cmp r4, #0x28
003eba6c: bne #0x3eba50
003eba70: ldr r0, [sp, #0x14]
003eba74: ldr r1, [sp, #0x40]
003eba78: add r0, r0, #1
003eba7c: cmp r0, r1
003eba80: str r0, [sp, #0x14]
003eba84: bne #0x3eb79c
003eba88: ldr r2, [sp, #0x4c]
003eba8c: ldr ip, [sp, #0x1c]
003eba90: ldr r3, [ip, r2]
003eba94: ldr r2, [sp, #0x94]
003eba98: ldr r3, [r3]
003eba9c: cmp r2, r3
003ebaa0: bne #0x3ebad8
003ebaa4: add sp, sp, #0x9c
003ebaa8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ebaac: mvn r3, #0xe0000000
003ebab0: b #0x3eb984
003ebab4: str r2, [sp, #0xc]
003ebab8: bl #0x310440
003ebabc: ldr r2, [sp, #0xc]
003ebac0: b #0x3eba2c
003ebac4: add r0, r4, #8
003ebac8: add r2, sp, #0x7c
003ebacc: bl #0x3eaff8
003ebad0: mov r5, r0
003ebad4: b #0x3eb820
003ebad8: bl #0x30e310

# 0x3eacd0 _ZN11ItemManager5SpawnER13ItemInventoryjPK10GameObjectRK7Point3DIfES8_PK9Character
003eacd0: push {r4, r5, r6, r7, r8, sl, lr}
003eacd4: ldr ip, [pc, #0x164]
003eacd8: cmp r3, #0
003eacdc: sub sp, sp, #0xc
003eace0: add ip, pc, ip
003eace4: mov r6, r0
003eace8: mov r4, r1
003eacec: mov r5, r2
003eacf0: beq #0x3ead10
003eacf4: mov r0, r4
003eacf8: bl #0x3fc608
003eacfc: cmp r0, r5
003ead00: bhi #0x3ead64
003ead04: mov r0, #0
003ead08: add sp, sp, #0xc
003ead0c: pop {r4, r5, r6, r7, r8, sl, pc}
003ead10: ldr r2, [pc, #0x12c]
003ead14: ldr r2, [ip, r2]
003ead18: ldr r2, [r2]
003ead1c: cmp r2, #2
003ead20: streq r3, [r3]
003ead24: beq #0x3eacf4
003ead28: cmp r2, #1
003ead2c: bne #0x3eacf4
003ead30: ldr r0, [pc, #0x110]
003ead34: ldr r1, [pc, #0x110]
003ead38: ldr r2, [pc, #0x110]
003ead3c: ldr r0, [ip, r0]
003ead40: ldr r3, [pc, #0x10c]
003ead44: mov ip, #0x66
003ead48: add r1, pc, r1
003ead4c: add r2, pc, r2
003ead50: add r3, pc, r3
003ead54: add r0, r0, #0xa8
003ead58: str ip, [sp]
003ead5c: bl #0x30e004
003ead60: b #0x3eacf4
003ead64: mov r1, r5
003ead68: mov r0, r4
003ead6c: bl #0x3fc61c
003ead70: bl #0x3f9e08
003ead74: ldr r3, [r0, #0x54]
003ead78: cmp r3, #0
003ead7c: blt #0x3ead04
003ead80: ldr r2, [r6, #4]
003ead84: ldr r1, [r6, #8]
003ead88: rsb r1, r2, r1
003ead8c: cmp r3, r1, asr #4
003ead90: bge #0x3ead04
003ead94: add r1, r2, r3, lsl #4
003ead98: ldr r7, [r1, #0xc]
003ead9c: ldr r8, [r2, r3, lsl #4]
003eada0: add r3, r7, #1
003eada4: cmp r3, #4
003eada8: str r3, [r1, #0xc]
003eadac: movhi r3, #0
003eadb0: strhi r3, [r1, #0xc]
003eadb4: add sl, r8, r7, lsl #3
003eadb8: ldrb r3, [sl, #4]
003eadbc: cmp r3, #0
003eadc0: beq #0x3eae30
003eadc4: mov r0, r6
003eadc8: ldr r1, [r8, r7, lsl #3]
003eadcc: bl #0x3eac34
003eadd0: ldr r0, [r8, r7, lsl #3]
003eadd4: mov r2, #1
003eadd8: ldr r1, [sp, #0x28]
003eaddc: bl #0x393db4
003eade0: ldr r1, [sp, #0x2c]
003eade4: ldr r0, [r8, r7, lsl #3]
003eade8: bl #0x393600
003eadec: mov r2, r5
003eadf0: mov r1, r4
003eadf4: ldr r3, [sp, #0x30]
003eadf8: ldr r0, [r8, r7, lsl #3]
003eadfc: bl #0x3ec0f0
003eae00: ldr r3, [r8, r7, lsl #3]
003eae04: mov r1, #1
003eae08: mov r0, r3
003eae0c: ldr r3, [r3]
003eae10: mov lr, pc
003eae14: ldr pc, [r3, #0x40]
003eae18: ldr r2, [r8, r7, lsl #3]
003eae1c: mov r3, #1
003eae20: strb r3, [r2, #0x85]
003eae24: strb r3, [sl, #4]
003eae28: ldr r0, [r8, r7, lsl #3]
003eae2c: b #0x3ead08
003eae30: ldr r0, [r8, r7, lsl #3]
003eae34: cmp r0, #0
003eae38: bne #0x3eadd4
003eae3c: b #0x3ead04
003eae40: ldrheq sb, [sl], #-0xd0
003eae44: andeq r3, r0, r0, asr #19
003eae48: andeq r1, r0, r0, asr #19
003eae4c: umaaleq r3, sp, r0, r6
003eae50: subeq fp, sp, r4, asr #8
003eae54: subeq fp, sp, r8, asr #8

# 0x3eac34 _ZN11ItemManager7DeSpawnEP10ItemObject
003eac34: push {r4, r5, r6, lr}
003eac38: subs r4, r1, #0
003eac3c: beq #0x3eac8c
003eac40: mov r3, #0x3ac
003eac44: ldrsh r3, [r4, r3]
003eac48: cmp r3, #0
003eac4c: blt #0x3eac8c
003eac50: ldr r1, [r0, #8]
003eac54: ldr r2, [r0, #4]
003eac58: rsb r1, r2, r1
003eac5c: cmp r3, r1, asr #4
003eac60: bge #0x3eac8c
003eac64: ldr r2, [r2, r3, lsl #4]
003eac68: mov r3, #0
003eac6c: ldr r1, [r2, r3]
003eac70: add r0, r2, r3
003eac74: add r3, r3, #8
003eac78: cmp r4, r1
003eac7c: beq #0x3eac90
003eac80: cmp r3, #0x28
003eac84: bne #0x3eac6c
003eac88: pop {r4, r5, r6, pc}
003eac8c: pop {r4, r5, r6, pc}
003eac90: mov r5, #0
003eac94: strb r5, [r0, #4]
003eac98: ldr r3, [r4]
003eac9c: mov r0, r4
003eaca0: mov r1, r5
003eaca4: mov lr, pc
003eaca8: ldr pc, [r3, #0x40]
003eacac: add r0, r4, #0x374
003eacb0: mov r1, #1
003eacb4: bl #0x3fe6bc
003eacb8: mov r0, r4
003eacbc: mov r1, r5
003eacc0: mov r2, r5
003eacc4: bl #0x394bf8
003eacc8: strb r5, [r4, #0x85]
003eaccc: pop {r4, r5, r6, pc}

# 0x3eb6bc _ZN11ItemManager5FlushEv
003eb6bc: str lr, [sp, #-4]!
003eb6c0: ldmib r0, {r1, r2}
003eb6c4: sub sp, sp, #0xc
003eb6c8: cmp r1, r2
003eb6cc: beq #0x3eb6dc
003eb6d0: add r0, r0, #4
003eb6d4: add r3, sp, #4
003eb6d8: bl #0x3eb634
003eb6dc: add sp, sp, #0xc
003eb6e0: ldm sp!, {pc}

# 0x3ec324 _ZN10ItemObjectC1EN10ObjectBase6GO_IDSE
003ec324: push {r4, r5, r6, r7, r8, lr}
003ec328: mov r4, r0
003ec32c: ldr r5, [pc, #0x90]
003ec330: bl #0x38c398
003ec334: add r0, r4, #0x374
003ec338: bl #0x3ff330
003ec33c: ldr r3, [pc, #0x84]
003ec340: add r5, pc, r5
003ec344: mvn r1, #0
003ec348: ldr r3, [r5, r3]
003ec34c: mov r7, #0x3c0
003ec350: strh r1, [r4, r7]
003ec354: add r0, r3, #0xfc
003ec358: add r6, r3, #8
003ec35c: add ip, r3, #0xd8
003ec360: add r3, r3, #0xe4
003ec364: str r3, [r4, #0x24]
003ec368: mov r3, #0x3ac
003ec36c: str r0, [r4, #0x374]
003ec370: stm r4, {r6, ip}
003ec374: strh r1, [r4, r3]
003ec378: mov r3, #0x40000000
003ec37c: add r3, r3, #0x200000
003ec380: str r3, [r4, #0x3b0]
003ec384: mov r3, #0x3b4
003ec388: strh r1, [r4, r3]
003ec38c: movw r3, #0x3b6
003ec390: strh r1, [r4, r3]
003ec394: mov r3, #0x3b8
003ec398: strh r1, [r4, r3]
003ec39c: mov r2, #0
003ec3a0: mov r3, #1
003ec3a4: strb r3, [r4, #0x85]
003ec3a8: strb r2, [r4, #0x2ee]
003ec3ac: str r2, [r4, #0x3bc]
003ec3b0: str r2, [r4, #0x3c4]
003ec3b4: str r2, [r4, #0x3c8]
003ec3b8: str r2, [r4, #0x3cc]
003ec3bc: mov r0, r4
003ec3c0: pop {r4, r5, r6, r7, r8, pc}
003ec3c4: subseq r8, sl, r0, asr r7
003ec3c8: andeq r2, r0, r8, lsr #12

# 0x3ece80 _ZN10ItemObject8InitOnceEi
003ece80: ldr r3, [pc, #0xcc]
003ece84: push {r4, r5, r6, lr}
003ece88: add r3, pc, r3
003ece8c: mov r5, r1
003ece90: mov r1, r3
003ece94: mov r3, #0x3ac
003ece98: strh r5, [r0, r3]
003ece9c: mov r4, r0
003ecea0: add r2, r1, #0x22
003ecea4: add r0, r0, #0x290
003ecea8: bl #0x3109e0
003eceac: ldr r3, [pc, #0xa4]
003eceb0: mov r2, #0x40000000
003eceb4: cmp r5, #0
003eceb8: add r2, r2, #0xc00000
003ecebc: str r2, [r4, #0x3b0]
003ecec0: add r3, pc, r3
003ecec4: blt #0x3ecedc
003ecec8: ldr r2, [pc, #0x8c]
003ececc: ldr r2, [r3, r2]
003eced0: ldr r2, [r2]
003eced4: cmp r5, r2
003eced8: blt #0x3ecf0c
003ecedc: ldr r1, [pc, #0x7c]
003ecee0: add r0, r4, #0x2a8
003ecee4: add r1, pc, r1
003ecee8: add r2, r1, #0x12
003eceec: bl #0x3109e0
003ecef0: mov r0, r4
003ecef4: bl #0x38be5c
003ecef8: ldr r0, [r4, #0x2d8]
003ecefc: cmp r0, #0
003ecf00: beq #0x3ecf50
003ecf04: pop {r4, r5, r6, lr}
003ecf08: b #0x470a54
003ecf0c: ldr r2, [pc, #0x50]
003ecf10: ldr r3, [r3, r2]
003ecf14: mov r2, #0x14
003ecf18: ldr r3, [r3]
003ecf1c: mla r5, r2, r5, r3
003ecf20: ldr r5, [r5, #0x10]
003ecf24: mov r0, r5
003ecf28: bl #0x30de54
003ecf2c: mov r1, r5
003ecf30: add r2, r5, r0
003ecf34: add r0, r4, #0x2a8
003ecf38: bl #0x3109e0
003ecf3c: mov r0, r4
003ecf40: bl #0x38be5c
003ecf44: ldr r0, [r4, #0x2d8]
003ecf48: cmp r0, #0
003ecf4c: bne #0x3ecf04
003ecf50: pop {r4, r5, r6, pc}
003ecf54: subeq sb, sp, r8, lsl r4
003ecf58: ldrsbeq r7, [sl], #-0xb0
003ecf5c: andeq r4, r0, r0, lsl r5
003ecf60: subeq sb, sp, r4, ror #7
003ecf64: andeq r0, r0, ip, lsr sb

# 0x3ec0f0 _ZN10ItemObject9InitAgainER13ItemInventoryjPK9Character
003ec0f0: push {r4, r5, r6, r7, r8, sl, lr}
003ec0f4: add r7, r0, #0x374
003ec0f8: sub sp, sp, #0x34
003ec0fc: mov r5, #0
003ec100: mov r4, r0
003ec104: mov r6, r3
003ec108: mov r0, r1
003ec10c: mov r3, #1
003ec110: mov r1, r2
003ec114: mov r2, r7
003ec118: str r5, [sp]
003ec11c: bl #0x3ffa44
003ec120: mov r0, r7
003ec124: mov r1, r5
003ec128: bl #0x3fc61c
003ec12c: ldr r5, [pc, #0x1bc]
003ec130: subs r7, r0, #0
003ec134: add r5, pc, r5
003ec138: beq #0x3ec29c
003ec13c: bl #0x3f9e08
003ec140: ldr r3, [r0, #0x54]
003ec144: cmn r3, #1
003ec148: beq #0x3ec184
003ec14c: ldr r3, [pc, #0x1a0]
003ec150: mov r0, r7
003ec154: ldr r3, [r5, r3]
003ec158: ldr r8, [r3]
003ec15c: bl #0x3f9e08
003ec160: ldr r3, [r0, #0x54]
003ec164: mov r2, #0x14
003ec168: mla r8, r2, r3, r8
003ec16c: mov r3, #0x3b4
003ec170: ldrh r2, [r8, #4]
003ec174: strh r2, [r4, r3]
003ec178: ldrh r8, [r8, #8]
003ec17c: movw r3, #0x3b6
003ec180: strh r8, [r4, r3]
003ec184: ldr r8, [r4, #0x2d8]
003ec188: cmp r8, #0
003ec18c: beq #0x3ec1bc
003ec190: mov r0, r7
003ec194: bl #0x3fa710
003ec198: ldr r2, [r8, #0x38]
003ec19c: mov r3, #0
003ec1a0: mov r1, r3
003ec1a4: ldr ip, [r2]
003ec1a8: mov r0, r2
003ec1ac: str r3, [sp]
003ec1b0: mov r2, r3
003ec1b4: mov lr, pc
003ec1b8: ldr pc, [ip, #0x1c]
003ec1bc: cmp r6, #0
003ec1c0: strne r6, [r4, #0x3bc]
003ec1c4: mov r3, #0x3b4
003ec1c8: ldrsh r1, [r4, r3]
003ec1cc: ldr r3, [pc, #0x124]
003ec1d0: ldr lr, [r4, #0x1b0]
003ec1d4: ldr r6, [r4, #0x1ac]
003ec1d8: ldr r3, [r5, r3]
003ec1dc: ldr r8, [r4, #0x1a8]
003ec1e0: mov ip, #0xbf000000
003ec1e4: ldr r0, [r3]
003ec1e8: add ip, ip, #0x800000
003ec1ec: mov r7, #1
003ec1f0: add r2, sp, #0x24
003ec1f4: mov r3, #0
003ec1f8: str lr, [sp, #0x2c]
003ec1fc: str ip, [sp, #8]
003ec200: str ip, [sp, #4]
003ec204: str r8, [sp, #0x24]
003ec208: str r6, [sp, #0x28]
003ec20c: str r7, [sp]
003ec210: bl #0x36b5d8
003ec214: ldr r3, [pc, #0xe0]
003ec218: mov r6, #0
003ec21c: mov r1, r6
003ec220: ldr r3, [r5, r3]
003ec224: mov r0, #0x28
003ec228: ldr sl, [r3, #0x44]
003ec22c: bl #0x310570
003ec230: mvn ip, #2
003ec234: str ip, [sp, #0xc]
003ec238: mov ip, #0x40
003ec23c: mov r1, sl
003ec240: mov r2, r4
003ec244: mov r3, r6
003ec248: str ip, [sp, #0x10]
003ec24c: mov ip, #4
003ec250: mov r8, r0
003ec254: str ip, [sp, #0x14]
003ec258: str r7, [sp, #4]
003ec25c: str r7, [sp]
003ec260: str r6, [sp, #8]
003ec264: str r6, [sp, #0x18]
003ec268: bl #0x46f2f0
003ec26c: ldr r3, [pc, #0x8c]
003ec270: mov r0, r4
003ec274: mov r1, r8
003ec278: ldr r3, [r5, r3]
003ec27c: mov r2, r6
003ec280: add r3, r3, #8
003ec284: str r3, [r8]
003ec288: bl #0x394bf8
003ec28c: mov r0, r4
003ec290: bl #0x3ebca4
003ec294: add sp, sp, #0x34
003ec298: pop {r4, r5, r6, r7, r8, sl, pc}
003ec29c: ldr r3, [pc, #0x60]
003ec2a0: ldr r3, [r5, r3]
003ec2a4: ldr r3, [r3]
003ec2a8: cmp r3, #2
003ec2ac: streq r7, [r7]
003ec2b0: beq #0x3ec1c4
003ec2b4: cmp r3, #1
003ec2b8: bne #0x3ec1c4
003ec2bc: ldr r0, [pc, #0x44]
003ec2c0: ldr r1, [pc, #0x44]
003ec2c4: ldr r2, [pc, #0x44]
003ec2c8: ldr r0, [r5, r0]
003ec2cc: ldr r3, [pc, #0x40]
003ec2d0: movw ip, #0x1c7
003ec2d4: add r1, pc, r1
003ec2d8: add r2, pc, r2
003ec2dc: add r3, pc, r3
003ec2e0: add r0, r0, #0xa8
003ec2e4: str ip, [sp]
003ec2e8: bl #0x30e004
003ec2ec: b #0x3ec1c4
003ec2f0: subseq r8, sl, ip, asr sb
003ec2f4: andeq r0, r0, ip, lsr sb
003ec2f8: andeq r0, r0, r4, lsr #27
003ec2fc: strdeq r3, r4, [r0], -r4
003ec300: muleq r0, r0, r6
003ec304: andeq r3, r0, r0, asr #19
003ec308: andeq r1, r0, r0, asr #19
003ec30c: subeq r2, sp, r4, lsl #2
003ec310: subeq sb, sp, r0, asr pc
003ec314: subeq sb, sp, r4, asr pc

# 0x3ed144 _ZN10ItemObject8InteractEP10GameObject
003ed144: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ed148: ldr r4, [pc, #0x8e0]
003ed14c: ldr r5, [pc, #0x8e0]
003ed150: sub sp, sp, #0x184
003ed154: add r4, pc, r4
003ed158: ldr r3, [r4, r5]
003ed15c: mov r8, r1
003ed160: mov r6, r0
003ed164: ldr r3, [r3]
003ed168: str r3, [sp, #0x17c]
003ed16c: bl #0x3ebffc
003ed170: cmp r0, #0
003ed174: beq #0x3ed194
003ed178: ldr r3, [r4, r5]
003ed17c: ldr r2, [sp, #0x17c]
003ed180: ldr r3, [r3]
003ed184: cmp r2, r3
003ed188: bne #0x3ed8ec
003ed18c: add sp, sp, #0x184
003ed190: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ed194: add r7, sp, #0x3c
003ed198: mov r0, r7
003ed19c: mov r1, r8
003ed1a0: bl #0x33dd2c
003ed1a4: mov r0, r7
003ed1a8: bl #0x33ff54
003ed1ac: subs r7, r0, #0
003ed1b0: beq #0x3ed178
003ed1b4: ldr r3, [r6, #0x3bc]
003ed1b8: cmp r3, #0
003ed1bc: beq #0x3ed1c8
003ed1c0: cmp r7, r3
003ed1c4: bne #0x3ed178
003ed1c8: mov r3, #0x3c0
003ed1cc: ldrsh r8, [r6, r3]
003ed1d0: cmn r8, #1
003ed1d4: beq #0x3ed1fc
003ed1d8: ldr r3, [pc, #0x858]
003ed1dc: mov r1, r7
003ed1e0: mov r2, #0
003ed1e4: ldr r3, [r4, r3]
003ed1e8: ldr r0, [r3, #0x40]
003ed1ec: bl #0x36eea8
003ed1f0: ldr r3, [r0, #0x678]
003ed1f4: cmp r8, r3
003ed1f8: beq #0x3ed654
003ed1fc: ldr r3, [r7]
003ed200: mov r0, r7
003ed204: mov lr, pc
003ed208: ldr pc, [r3, #0x28]
003ed20c: cmp r0, #0
003ed210: beq #0x3ed178
003ed214: add r1, r6, #0x374
003ed218: str r1, [sp, #0x20]
003ed21c: ldr r0, [sp, #0x20]
003ed220: mov r1, #0
003ed224: bl #0x3fc61c
003ed228: ldr fp, [pc, #0x808]
003ed22c: ldr r1, [pc, #0x808]
003ed230: mov r8, r0
003ed234: ldr sl, [r4, fp]
003ed238: add r1, pc, r1
003ed23c: mov r0, sl
003ed240: bl #0x320e44
003ed244: str r0, [sp, #0x1c]
003ed248: mov r0, r8
003ed24c: bl #0x3f9e08
003ed250: ldr sb, [r0, #0x68]
003ed254: mov r0, r8
003ed258: bl #0x3f9e08
003ed25c: cmn sb, #1
003ed260: addeq r2, r7, #0x37c
003ed264: ldr sb, [r0, #0x58]
003ed268: streq r2, [sp, #0x1c]
003ed26c: bne #0x3ed668
003ed270: cmp sb, #0xe
003ed274: beq #0x3ed8f0
003ed278: ldr r3, [pc, #0x7c0]
003ed27c: add r1, sp, #0x124
003ed280: mov r0, r8
003ed284: ldr r3, [r4, r3]
003ed288: str r1, [sp, #0x28]
003ed28c: add sb, sp, #0x16c
003ed290: ldr r2, [r3]
003ed294: add sl, sp, #0x10c
003ed298: str r2, [sp, #0x18]
003ed29c: bl #0x3fa6cc
003ed2a0: ldr r2, [sp, #0x18]
003ed2a4: mov r3, #0xc
003ed2a8: ldr r1, [pc, #0x794]
003ed2ac: mla r3, r3, r0, r2
003ed2b0: add r1, pc, r1
003ed2b4: ldr r2, [r3, #8]
003ed2b8: mov r0, sb
003ed2bc: bl #0x30eae4
003ed2c0: mov r1, sb
003ed2c4: add r2, sp, #0x54
003ed2c8: ldr r0, [sp, #0x28]
003ed2cc: bl #0x3140ec
003ed2d0: ldr r3, [sp, #0x138]
003ed2d4: ldr r1, [sp, #0x134]
003ed2d8: mov r0, sl
003ed2dc: str sl, [sp, #0x11c]
003ed2e0: rsb r1, r3, r1
003ed2e4: add r1, r1, #0xf
003ed2e8: str sl, [sp, #0x120]
003ed2ec: bl #0x31167c
003ed2f0: ldr r1, [pc, #0x750]
003ed2f4: ldr r3, [sp, #0x11c]
003ed2f8: mov ip, #0
003ed2fc: add r1, pc, r1
003ed300: strb ip, [r3]
003ed304: add r2, r1, #0xe
003ed308: add r3, sp, #0x48
003ed30c: mov r0, sl
003ed310: str ip, [sp, #0x14]
003ed314: bl #0x32a78c
003ed318: mov r0, sl
003ed31c: ldr r1, [sp, #0x138]
003ed320: ldr r2, [sp, #0x134]
003ed324: bl #0x310804
003ed328: add r2, sp, #0xdc
003ed32c: str r2, [sp, #0x24]
003ed330: ldr r2, [pc, #0x714]
003ed334: add r3, sp, #0xf4
003ed338: mov r0, r3
003ed33c: mov r1, sl
003ed340: add r2, pc, r2
003ed344: str r3, [sp, #0x18]
003ed348: bl #0x3338cc
003ed34c: ldr r1, [r8, #0x1c]
003ed350: add r2, sp, #0x50
003ed354: ldr r0, [sp, #0x24]
003ed358: bl #0x3140ec
003ed35c: ldr r3, [sp, #0x18]
003ed360: add sb, sp, #0xc4
003ed364: mov r0, sb
003ed368: mov r1, r3
003ed36c: ldr r2, [sp, #0x24]
003ed370: bl #0x3ecf68
003ed374: ldr r2, [pc, #0x6d4]
003ed378: add r1, sp, #0x13c
003ed37c: str r1, [sp, #0x2c]
003ed380: add r2, pc, r2
003ed384: mov r1, sb
003ed388: ldr r0, [sp, #0x2c]
003ed38c: bl #0x3338cc
003ed390: mov r0, sb
003ed394: bl #0x3139ac
003ed398: ldr r0, [sp, #0x24]
003ed39c: bl #0x3139ac
003ed3a0: ldr r3, [sp, #0x18]
003ed3a4: mov r0, r3
003ed3a8: bl #0x3139ac
003ed3ac: mov r0, sl
003ed3b0: bl #0x3139ac
003ed3b4: add sl, sp, #0x58
003ed3b8: ldr r0, [sp, #0x28]
003ed3bc: bl #0x3139ac
003ed3c0: mov r0, sl
003ed3c4: mov r1, #0x10
003ed3c8: str sl, [sp, #0x68]
003ed3cc: str sl, [sp, #0x6c]
003ed3d0: bl #0x31167c
003ed3d4: ldr ip, [sp, #0x14]
003ed3d8: ldr r3, [sp, #0x68]
003ed3dc: mov r0, sl
003ed3e0: strb ip, [r3]
003ed3e4: ldr r1, [sp, #0x150]
003ed3e8: ldr r2, [sp, #0x14c]
003ed3ec: str ip, [sp, #0x14]
003ed3f0: bl #0x3109e0
003ed3f4: ldr r3, [r4, fp]
003ed3f8: ldr ip, [sp, #0x14]
003ed3fc: mov r2, #1
003ed400: ldr r0, [r3, #0x40]
003ed404: mov r1, ip
003ed408: bl #0x36e478
003ed40c: ldr r3, [r0, #0x660]
003ed410: cmp r7, r3
003ed414: beq #0x3eda24
003ed418: mov r0, r8
003ed41c: bl #0x3f9e08
003ed420: ldr r3, [r0, #0x58]
003ed424: cmp r3, #0xd
003ed428: beq #0x3ed43c
003ed42c: bl #0x7fd794
003ed430: ldrb r3, [r0, #5]
003ed434: cmp r3, #0
003ed438: beq #0x3ed9c4
003ed43c: add r3, r7, #0x560
003ed440: str r3, [sp, #0x24]
003ed444: ldr r3, [pc, #0x608]
003ed448: add r8, sp, #0xac
003ed44c: ldr sb, [r4, r3]
003ed450: mov r0, sb
003ed454: bl #0x337888
003ed458: ldr r1, [pc, #0x5f8]
003ed45c: add r2, sp, #0x4c
003ed460: mov r0, r8
003ed464: add r1, pc, r1
003ed468: bl #0x3140ec
003ed46c: mov r1, r8
003ed470: mov r0, sb
003ed474: bl #0x337a88
003ed478: mov r0, r8
003ed47c: bl #0x3139ac
003ed480: mov r3, #1
003ed484: ldr r0, [sp, #0x20]
003ed488: ldr r1, [sp, #0x1c]
003ed48c: mov r2, #0
003ed490: bl #0x3ffa68
003ed494: ldr r0, [sp, #0x24]
003ed498: mov r1, #0xdf
003ed49c: mov r2, #1
003ed4a0: bl #0x3e0798
003ed4a4: ldr r3, [pc, #0x5b0]
003ed4a8: ldr r0, [sp, #0x24]
003ed4ac: mov r1, #0xdf
003ed4b0: ldr r3, [r4, r3]
003ed4b4: mov r2, #0
003ed4b8: ldr r3, [r3]
003ed4bc: str r3, [sp, #0x24]
003ed4c0: bl #0x3df6e0
003ed4c4: cmp r0, #0x12c
003ed4c8: bge #0x3ed918
003ed4cc: mov r0, sl
003ed4d0: bl #0x3139ac
003ed4d4: ldr r0, [sp, #0x2c]
003ed4d8: bl #0x3139ac
003ed4dc: mov r0, r6
003ed4e0: bl #0x3ebffc
003ed4e4: cmp r0, #0
003ed4e8: beq #0x3ed178
003ed4ec: bl #0x7fd794
003ed4f0: ldrb r3, [r0, #5]
003ed4f4: cmp r3, #0
003ed4f8: beq #0x3ed54c
003ed4fc: ldr r3, [r7, #0x378]
003ed500: ldrb r3, [r3, #0xa]
003ed504: cmp r3, #0
003ed508: beq #0x3ed54c
003ed50c: bl #0x80b1bc
003ed510: mov r8, r0
003ed514: ldr r0, [pc, #0x544]
003ed518: mov r1, #1
003ed51c: ldr sl, [r6, #0x108]
003ed520: add r0, pc, r0
003ed524: ldrb sb, [r7, #0x108]
003ed528: bl #0x80a244
003ed52c: uxth sl, sl
003ed530: mov r3, #5
003ed534: mov r1, r0
003ed538: strb sb, [r0, #0x54]
003ed53c: strb r3, [r0, #0x50]
003ed540: strh sl, [r0, #0x52]
003ed544: mov r0, r8
003ed548: bl #0x80e2a4
003ed54c: movw r3, #0x3b6
003ed550: ldrsh r1, [r6, r3]
003ed554: ldr r3, [pc, #0x508]
003ed558: ldr lr, [r6, #0x164]
003ed55c: ldr r8, [r6, #0x168]
003ed560: ldr r3, [r4, r3]
003ed564: ldr sl, [r6, #0x160]
003ed568: mov ip, #0xbf000000
003ed56c: ldr r0, [r3]
003ed570: add ip, ip, #0x800000
003ed574: str lr, [sp, #0x34]
003ed578: add r2, sp, #0x30
003ed57c: mov lr, #1
003ed580: mov r3, #0
003ed584: str r8, [sp, #0x38]
003ed588: str sl, [sp, #0x30]
003ed58c: str lr, [sp]
003ed590: str ip, [sp, #8]
003ed594: str ip, [sp, #4]
003ed598: bl #0x36b5d8
003ed59c: ldr r8, [r6, #0x3c8]
003ed5a0: cmp r8, #0
003ed5a4: beq #0x3ed5c0
003ed5a8: mov r0, r8
003ed5ac: bl #0x498dec
003ed5b0: mov r0, r8
003ed5b4: bl #0x310440
003ed5b8: mov r3, #0
003ed5bc: str r3, [r6, #0x3c8]
003ed5c0: mov r0, r6
003ed5c4: bl #0x3ebccc
003ed5c8: movw r3, #0x14a4
003ed5cc: ldr r2, [r7, r3]
003ed5d0: ldr r0, [pc, #0x490]
003ed5d4: cmp r6, r2
003ed5d8: moveq r2, #0
003ed5dc: streq r2, [r7, r3]
003ed5e0: add r0, pc, r0
003ed5e4: ldr r8, [r0]
003ed5e8: ands r8, r8, #1
003ed5ec: beq #0x3ed874
003ed5f0: ldr r3, [pc, #0x474]
003ed5f4: mov r8, #0
003ed5f8: add r2, r7, #0x160
003ed5fc: add r3, pc, r3
003ed600: ldr r1, [r3, #4]
003ed604: ldr r3, [pc, #0x464]
003ed608: str r8, [sp]
003ed60c: ldr r0, [r4, r3]
003ed610: mov r3, r8
003ed614: bl #0x495d14
003ed618: ldr r3, [pc, #0x454]
003ed61c: mov r1, r6
003ed620: ldr r0, [r4, r3]
003ed624: bl #0x3eac34
003ed628: ldr r3, [r4, fp]
003ed62c: mov r1, r7
003ed630: mov r2, r8
003ed634: ldr r0, [r3, #0x40]
003ed638: bl #0x36eea8
003ed63c: ldr r3, [pc, #0x434]
003ed640: ldr r2, [r0, #0x678]
003ed644: mov r1, #4
003ed648: ldr r0, [r4, r3]
003ed64c: bl #0x3790ec
003ed650: b #0x3ed178
003ed654: mov r3, #0x3b8
003ed658: ldrsh r3, [r6, r3]
003ed65c: cmp r3, #0
003ed660: bgt #0x3ed178
003ed664: b #0x3ed1fc
003ed668: mov r0, r8
003ed66c: bl #0x3f9e80
003ed670: ldr r3, [sp, #0x1c]
003ed674: cmp r0, r3
003ed678: blo #0x3ed70c
003ed67c: add r2, r7, #0x37c
003ed680: mov r0, r2
003ed684: str r2, [sp, #0x1c]
003ed688: bl #0x3fe330
003ed68c: cmp r0, #0
003ed690: beq #0x3ed270
003ed694: add r8, sp, #0x74
003ed698: mov r0, r8
003ed69c: mov r1, #0x10
003ed6a0: str r8, [sp, #0x84]
003ed6a4: str r8, [sp, #0x88]
003ed6a8: bl #0x31167c
003ed6ac: ldr r3, [sp, #0x84]
003ed6b0: mov r2, #0
003ed6b4: ldr r1, [pc, #0x3c0]
003ed6b8: strb r2, [r3]
003ed6bc: ldr r2, [pc, #0x3bc]
003ed6c0: ldr r0, [sl, #0x2c]
003ed6c4: add r1, pc, r1
003ed6c8: add r2, pc, r2
003ed6cc: ldr sl, [sl, #0x34]
003ed6d0: bl #0x4c4bdc
003ed6d4: mov r1, r0
003ed6d8: mov r0, sl
003ed6dc: bl #0x508edc
003ed6e0: mov sl, r0
003ed6e4: bl #0x30de54
003ed6e8: mov r1, sl
003ed6ec: add r2, sl, r0
003ed6f0: mov r0, r8
003ed6f4: bl #0x3109e0
003ed6f8: mov r0, r8
003ed6fc: bl #0x3ecff8
003ed700: mov r0, r8
003ed704: bl #0x3139ac
003ed708: b #0x3ed4dc
003ed70c: add sb, sp, #0x154
003ed710: mov r0, sb
003ed714: mov r1, #0x10
003ed718: str sb, [sp, #0x164]
003ed71c: str sb, [sp, #0x168]
003ed720: bl #0x31167c
003ed724: ldr r3, [sp, #0x164]
003ed728: mov r2, #0
003ed72c: ldr r1, [pc, #0x350]
003ed730: strb r2, [r3]
003ed734: ldr r2, [pc, #0x34c]
003ed738: ldr r3, [sl, #0x34]
003ed73c: add r1, pc, r1
003ed740: add r2, pc, r2
003ed744: ldr r0, [sl, #0x2c]
003ed748: str r3, [sp, #0x18]
003ed74c: bl #0x4c4bdc
003ed750: ldr r3, [sp, #0x18]
003ed754: mov r1, r0
003ed758: mov r0, r3
003ed75c: bl #0x508edc
003ed760: ldr r3, [pc, #0x2d8]
003ed764: str r0, [sp, #0x1c]
003ed768: mov r0, r8
003ed76c: ldr r3, [r4, r3]
003ed770: ldr r3, [r3]
003ed774: str r3, [sp, #0x28]
003ed778: bl #0x3fa6cc
003ed77c: mov r1, r8
003ed780: str r0, [sp, #0x24]
003ed784: mov r2, #1
003ed788: mov r0, r7
003ed78c: bl #0x3a4a3c
003ed790: ldr r1, [sp, #0x1c]
003ed794: mov ip, r0
003ed798: cmp r1, #0
003ed79c: beq #0x3ed7f8
003ed7a0: ldr r2, [sp, #0x24]
003ed7a4: ldr r1, [sp, #0x28]
003ed7a8: mov r3, #0xc
003ed7ac: mla r3, r3, r2, r1
003ed7b0: ldr r1, [pc, #0x2d4]
003ed7b4: ldr r2, [r3, #8]
003ed7b8: add r3, sp, #0x16c
003ed7bc: add r1, pc, r1
003ed7c0: bic r2, r2, #0xff000000
003ed7c4: mov r0, r3
003ed7c8: str r3, [sp, #0x18]
003ed7cc: str ip, [sp, #0x14]
003ed7d0: bl #0x30eae4
003ed7d4: ldr lr, [r8, #0x1c]
003ed7d8: ldr ip, [sp, #0x14]
003ed7dc: ldr r0, [sl, #0x34]
003ed7e0: ldr r2, [sp, #0x1c]
003ed7e4: ldr r3, [sp, #0x18]
003ed7e8: mov r1, sb
003ed7ec: str lr, [sp]
003ed7f0: str ip, [sp, #4]
003ed7f4: bl #0x508ef4
003ed7f8: add r8, sp, #0x90
003ed7fc: mov r0, r8
003ed800: mov r1, #0x10
003ed804: str r8, [sp, #0xa0]
003ed808: str r8, [sp, #0xa4]
003ed80c: bl #0x31167c
003ed810: ldr r3, [sp, #0xa0]
003ed814: mov sl, #0
003ed818: mov r0, r8
003ed81c: strb sl, [r3]
003ed820: ldr r1, [sp, #0x168]
003ed824: ldr r2, [sp, #0x164]
003ed828: bl #0x3109e0
003ed82c: mov r0, r8
003ed830: bl #0x3ecff8
003ed834: mov ip, #1
003ed838: mov r3, sl
003ed83c: mov r1, sl
003ed840: add r2, r7, #0x37c
003ed844: ldr r0, [sp, #0x20]
003ed848: str ip, [sp]
003ed84c: bl #0x3ffa44
003ed850: mov r2, sl
003ed854: mov r1, r0
003ed858: mov r0, r7
003ed85c: bl #0x3a4bec
003ed860: mov r0, r8
003ed864: bl #0x3139ac
003ed868: mov r0, sb
003ed86c: bl #0x3139ac
003ed870: b #0x3ed4dc
003ed874: bl #0x30e76c
003ed878: cmp r0, #0
003ed87c: beq #0x3ed5f0
003ed880: ldr r3, [pc, #0x208]
003ed884: ldr r3, [r4, r3]
003ed888: ldr sl, [r3]
003ed88c: cmp sl, #0
003ed890: beq #0x3ed910
003ed894: ldr r3, [pc, #0x1f8]
003ed898: ldr sb, [pc, #0x1f8]
003ed89c: str r7, [sp, #0x20]
003ed8a0: ldr r3, [r4, r3]
003ed8a4: add sb, pc, sb
003ed8a8: ldr r3, [r3]
003ed8ac: mov r7, r3
003ed8b0: b #0x3ed8c0
003ed8b4: add r8, r8, #1
003ed8b8: cmp r8, sl
003ed8bc: beq #0x3ed90c
003ed8c0: mov r0, sb
003ed8c4: ldr r1, [r7, r8, lsl #2]
003ed8c8: bl #0x30e31c
003ed8cc: cmp r0, #0
003ed8d0: bne #0x3ed8b4
003ed8d4: ldr r7, [sp, #0x20]
003ed8d8: ldr r0, [pc, #0x1bc]
003ed8dc: add r0, pc, r0
003ed8e0: str r8, [r0, #4]
003ed8e4: bl #0x30ea3c
003ed8e8: b #0x3ed5f0
003ed8ec: bl #0x30e310
003ed8f0: ldr r0, [sp, #0x1c]
003ed8f4: bl #0x3fc690
003ed8f8: ldrb r3, [r7, #0x3a8]
003ed8fc: sxtb r3, r3
003ed900: cmp r0, r3
003ed904: bge #0x3ed4dc
003ed908: b #0x3ed278
003ed90c: ldr r7, [sp, #0x20]
003ed910: mvn r8, #0
003ed914: b #0x3ed8d8
003ed918: ldr r3, [r7]
003ed91c: mov r0, r7
003ed920: mov lr, pc
003ed924: ldr pc, [r3, #0x28]
003ed928: cmp r0, #0
003ed92c: beq #0x3ed4cc
003ed930: ldr r3, [r4, fp]
003ed934: mov r1, r7
003ed938: ldr r0, [r3, #0x40]
003ed93c: bl #0x36effc
003ed940: cmp r0, #0
003ed944: beq #0x3ed4cc
003ed948: ldr r3, [pc, #0x150]
003ed94c: ldr r3, [r4, r3]
003ed950: ldr sb, [r3]
003ed954: cmp sb, #0
003ed958: beq #0x3ed9bc
003ed95c: ldr r3, [pc, #0x140]
003ed960: ldr r2, [pc, #0x140]
003ed964: str r7, [sp, #0x1c]
003ed968: ldr r3, [r4, r3]
003ed96c: add r2, pc, r2
003ed970: mov r8, #0
003ed974: ldr r3, [r3]
003ed978: str r2, [sp, #0x20]
003ed97c: mov r7, r3
003ed980: b #0x3ed990
003ed984: add r8, r8, #1
003ed988: cmp r8, sb
003ed98c: beq #0x3ed9b8
003ed990: ldr r0, [sp, #0x20]
003ed994: ldr r1, [r7, r8, lsl #2]
003ed998: bl #0x30e31c
003ed99c: cmp r0, #0
003ed9a0: bne #0x3ed984
003ed9a4: ldr r7, [sp, #0x1c]
003ed9a8: mov r1, r8
003ed9ac: ldr r0, [sp, #0x24]
003ed9b0: bl #0x3813b8
003ed9b4: b #0x3ed4cc
003ed9b8: ldr r7, [sp, #0x1c]
003ed9bc: mvn r1, #0
003ed9c0: b #0x3ed9ac
003ed9c4: mov r0, r7
003ed9c8: bl #0x3bb8e4
003ed9cc: subs sb, r0, #0
003ed9d0: bne #0x3ed43c
003ed9d4: ldr r3, [r4, fp]
003ed9d8: ldr r3, [r3, #0x4c]
003ed9dc: ldrb r3, [r3, #0x2a]
003ed9e0: cmp r3, #0
003ed9e4: beq #0x3ed43c
003ed9e8: ldr r3, [pc, #0xbc]
003ed9ec: ldr r1, [pc, #0xbc]
003ed9f0: mov r2, #1
003ed9f4: ldr r8, [r4, r3]
003ed9f8: add r1, pc, r1
003ed9fc: mov r0, r8
003eda00: bl #0x4591f0
003eda04: cmn r0, #1
003eda08: mov r1, r0
003eda0c: beq #0x3ed43c
003eda10: mov r0, r8
003eda14: mov r3, sb
003eda18: mvn r2, #0
003eda1c: bl #0x4605c0
003eda20: b #0x3ed43c
003eda24: mov r0, sl
003eda28: bl #0x3ecff8
003eda2c: b #0x3ed418
003eda30: subseq r7, sl, ip, lsr sb
003eda34: andeq r4, r0, ip, lsr #1
003eda38: strdeq r3, r4, [r0], -r4
003eda3c: subeq sb, sp, r8, lsr #1
003eda40: andeq r1, r0, r4, asr #4
003eda44: subeq sb, sp, r0, lsl #2
003eda48: subeq sb, sp, r4, lsr r0
003eda4c: subeq sb, sp, r0
003eda50: subeq r8, sp, r8, asr #31
003eda54: andeq r0, r0, r4, lsl #17
003eda58: subeq r8, sp, ip, lsl #30
003eda5c: andeq r1, r0, r0, ror sp
003eda60: subeq r1, sp, r8, ror #19
003eda64: andeq r0, r0, r4, lsr #27
003eda68: ldrheq r5, [fp], #-0xa0

# 0x3ec048 _ZN10ItemObject17OnCollisionBeginsEP9Character
003ec048: push {r4, r5, r6, r7, r8, lr}
003ec04c: ldr r4, [pc, #0x94]
003ec050: subs r6, r1, #0
003ec054: mov r5, r0
003ec058: add r4, pc, r4
003ec05c: beq #0x3ec06c
003ec060: bl #0x3ebffc
003ec064: subs r7, r0, #0
003ec068: beq #0x3ec070
003ec06c: pop {r4, r5, r6, r7, r8, pc}
003ec070: ldr r3, [r5]
003ec074: mov r0, r5
003ec078: mov r1, r6
003ec07c: mov lr, pc
003ec080: ldr pc, [r3, #0x90]
003ec084: cmn r0, #1
003ec088: beq #0x3ec0c8
003ec08c: movw r3, #0x14a4
003ec090: ldr r3, [r6, r3]
003ec094: cmp r5, r3
003ec098: bne #0x3ec06c
003ec09c: ldr r3, [pc, #0x48]
003ec0a0: mov r1, r6
003ec0a4: ldr r3, [r4, r3]
003ec0a8: ldr r0, [r3, #0x40]
003ec0ac: bl #0x36effc
003ec0b0: cmp r0, #0
003ec0b4: beq #0x3ec06c
003ec0b8: mov r0, r5
003ec0bc: str r6, [r5, #0x3c4]
003ec0c0: pop {r4, r5, r6, r7, r8, lr}
003ec0c4: b #0x3ebd5c
003ec0c8: add r0, r6, #0x4f0
003ec0cc: add r0, r0, #0xc
003ec0d0: mov r1, r7
003ec0d4: bl #0x3c029c
003ec0d8: cmp r0, #0
003ec0dc: beq #0x3ec06c
003ec0e0: str r6, [r5, #0x2e4]
003ec0e4: pop {r4, r5, r6, r7, r8, pc}
003ec0e8: subseq r8, sl, r8, lsr sl
003ec0ec: strdeq r3, r4, [r0], -r4

# 0x3ec014 _ZNK10ItemObject13IsInteractiveEP10GameObject
003ec014: push {r4, lr}
003ec018: ldrb r2, [r0, #0x81]
003ec01c: cmp r2, #0
003ec020: bne #0x3ec030
003ec024: ldrb r3, [r0, #0x80]
003ec028: cmp r3, #0
003ec02c: bne #0x3ec038
003ec030: mov r0, #0
003ec034: pop {r4, pc}
003ec038: bl #0x3ebffc
003ec03c: eor r0, r0, #1
003ec040: uxtb r0, r0
003ec044: pop {r4, pc}

# 0x3ebffc _ZNK10ItemObject13HasBeenLootedEv
003ebffc: push {r4, lr}
003ec000: add r0, r0, #0x374
003ec004: bl #0x3fc608
003ec008: rsbs r0, r0, #1
003ec00c: movlo r0, #0
003ec010: pop {r4, pc}

# 0x3f9e34 _ZNK12ItemInstance13GetPickUpTypeEv
003f9e34: push {r4, lr}
003f9e38: ldrsh r3, [r0, #0x58]
003f9e3c: cmn r3, #1
003f9e40: beq #0x3f9e4c
003f9e44: mov r0, r3
003f9e48: pop {r4, pc}
003f9e4c: bl #0x3f9e08
003f9e50: ldr r0, [r0, #0xc]
003f9e54: pop {r4, pc}

# 0x3fc26c _ZN12ItemInstanceC1Eij
003fc26c: push {r4, r5, r6, r7, r8, lr}
003fc270: ldr r5, [pc, #0x148]
003fc274: ldr r3, [pc, #0x148]
003fc278: mov r4, r0
003fc27c: add r5, pc, r5
003fc280: ldr r3, [r5, r3]
003fc284: mov r6, r1
003fc288: add r1, r0, #8
003fc28c: add r3, r3, #8
003fc290: str r3, [r0]
003fc294: sub sp, sp, #8
003fc298: mov r0, r1
003fc29c: str r1, [r4, #0x18]
003fc2a0: str r1, [r4, #0x1c]
003fc2a4: str r6, [r4, #4]
003fc2a8: mov r1, #0x10
003fc2ac: mov r8, r2
003fc2b0: bl #0x31167c
003fc2b4: ldr r2, [r4, #0x18]
003fc2b8: mov r7, #0
003fc2bc: add r3, r4, #0x20
003fc2c0: strb r7, [r2]
003fc2c4: mov r0, r3
003fc2c8: str r3, [r4, #0x30]
003fc2cc: str r3, [r4, #0x34]
003fc2d0: mov r1, #0x10
003fc2d4: bl #0x31167c
003fc2d8: ldr r2, [r4, #0x30]
003fc2dc: add r3, r4, #0x38
003fc2e0: mov r0, r3
003fc2e4: strb r7, [r2]
003fc2e8: mov r1, #0x10
003fc2ec: str r3, [r4, #0x48]
003fc2f0: str r3, [r4, #0x4c]
003fc2f4: bl #0x31167c
003fc2f8: ldr r3, [r4, #0x48]
003fc2fc: cmp r6, r7
003fc300: strb r7, [r3]
003fc304: mov r3, #1
003fc308: strb r3, [r4, #0x68]
003fc30c: mvn r3, #0
003fc310: strh r8, [r4, #0x50]
003fc314: strb r7, [r4, #0x69]
003fc318: str r7, [r4, #0x54]
003fc31c: strh r3, [r4, #0x58]
003fc320: str r7, [r4, #0x5c]
003fc324: str r7, [r4, #0x60]
003fc328: str r7, [r4, #0x64]
003fc32c: blt #0x3fc344
003fc330: ldr r3, [pc, #0x90]
003fc334: ldr r3, [r5, r3]
003fc338: ldr r3, [r3]
003fc33c: cmp r3, r7
003fc340: bne #0x3fc368
003fc344: ldr r3, [pc, #0x80]
003fc348: ldr r3, [r5, r3]
003fc34c: ldr r3, [r3]
003fc350: cmp r3, #2
003fc354: moveq r3, #0
003fc358: streq r3, [r3]
003fc35c: beq #0x3fc368
003fc360: cmp r3, #1
003fc364: beq #0x3fc38c
003fc368: mov r0, r4
003fc36c: bl #0x3fb754
003fc370: mov r0, r4
003fc374: bl #0x3fb290
003fc378: mov r0, r4
003fc37c: bl #0x3facdc
003fc380: mov r0, r4
003fc384: add sp, sp, #8
003fc388: pop {r4, r5, r6, r7, r8, pc}
003fc38c: ldr r0, [pc, #0x3c]
003fc390: ldr r1, [pc, #0x3c]
003fc394: ldr r2, [pc, #0x3c]
003fc398: ldr r0, [r5, r0]
003fc39c: ldr r3, [pc, #0x38]
003fc3a0: mov ip, #0x54
003fc3a4: add r1, pc, r1
003fc3a8: add r2, pc, r2
003fc3ac: add r3, pc, r3
003fc3b0: add r0, r0, #0xa8
003fc3b4: str ip, [sp]
003fc3b8: bl #0x30e004
003fc3bc: b #0x3fc368
003fc3c0: subseq r8, sb, r4, lsl r8
003fc3c4: andeq r4, r0, r0, lsl #16
003fc3c8: andeq r0, r0, r0, ror #26
003fc3cc: andeq r3, r0, r0, asr #19
003fc3d0: andeq r1, r0, r0, asr #19
003fc3d4: subeq r2, ip, r4, lsr r0
003fc3d8: umaaleq sl, ip, r0, lr
003fc3dc: umaaleq sl, ip, ip, fp

# 0x3ffa68 _ZN13ItemInventory19TransferInventoryToERS_bb
003ffa68: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ffa6c: mov r7, r0
003ffa70: ldr r4, [r0, #8]
003ffa74: ldr r0, [r0, #0xc]
003ffa78: ldr fp, [pc, #0x1a8]
003ffa7c: sub sp, sp, #0x44
003ffa80: cmp r0, r4
003ffa84: add fp, pc, fp
003ffa88: mov r6, r1
003ffa8c: mov sb, r2
003ffa90: mov r8, r3
003ffa94: beq #0x3ffc10
003ffa98: ldr r3, [pc, #0x18c]
003ffa9c: ldr r1, [pc, #0x18c]
003ffaa0: ldr r2, [pc, #0x18c]
003ffaa4: str r3, [sp, #0x1c]
003ffaa8: ldr r3, [pc, #0x188]
003ffaac: str r1, [sp, #8]
003ffab0: add r1, sp, #0x24
003ffab4: add r3, pc, r3
003ffab8: str r3, [sp, #0xc]
003ffabc: ldr r3, [pc, #0x178]
003ffac0: str r2, [sp, #0x14]
003ffac4: add r4, r4, #4
003ffac8: add r3, pc, r3
003ffacc: str r3, [sp, #0x10]
003ffad0: add r5, r6, #0x30
003ffad4: str r1, [sp, #0x18]
003ffad8: ldr r1, [r4, #-4]
003ffadc: mov r2, sb
003ffae0: mov r3, r8
003ffae4: ldr r1, [r1]
003ffae8: mov r0, r6
003ffaec: bl #0x3ff5d4
003ffaf0: ldr r3, [r4, #-4]
003ffaf4: ldr r0, [r3]
003ffaf8: bl #0x3f9e00
003ffafc: ldr r3, [r6, #4]
003ffb00: mov sl, r0
003ffb04: cmp r3, #0
003ffb08: beq #0x3ffbe8
003ffb0c: mov r0, r3
003ffb10: ldr r3, [r3]
003ffb14: mov lr, pc
003ffb18: ldr pc, [r3, #0x28]
003ffb1c: cmp r0, #0
003ffb20: beq #0x3ffbe8
003ffb24: ldr r3, [r6, #0x30]
003ffb28: cmp r3, r5
003ffb2c: beq #0x3ffb4c
003ffb30: ldr r2, [r3, #8]
003ffb34: cmp sl, r2
003ffb38: beq #0x3ffb4c
003ffb3c: ldr r3, [r3]
003ffb40: cmp r5, r3
003ffb44: bne #0x3ffb30
003ffb48: mov r3, r5
003ffb4c: cmp r5, r3
003ffb50: beq #0x3ffbe8
003ffb54: ldr r3, [sp, #8]
003ffb58: ldr r2, [fp, r3]
003ffb5c: mov r0, r2
003ffb60: str r2, [sp, #4]
003ffb64: bl #0x31f594
003ffb68: subs r3, r0, #0
003ffb6c: ldr r2, [sp, #4]
003ffb70: beq #0x3ffbe8
003ffb74: ldr ip, [r6, #4]
003ffb78: ldr r0, [r2, #0x2c]
003ffb7c: ldr r1, [sp, #0xc]
003ffb80: ldr r2, [sp, #0x10]
003ffb84: str r3, [sp, #4]
003ffb88: str ip, [sp]
003ffb8c: bl #0x4c4bdc
003ffb90: ldr r1, [sp, #0x14]
003ffb94: ldr r3, [sp, #4]
003ffb98: ldr ip, [sp]
003ffb9c: ldr r2, [fp, r1]
003ffba0: str r0, [sp, #0x28]
003ffba4: ldr r1, [sp, #0x18]
003ffba8: add r2, r2, #8
003ffbac: mov r0, r3
003ffbb0: str r2, [sp, #0x24]
003ffbb4: mov r3, #0
003ffbb8: mvn r2, #0
003ffbbc: strb r3, [sp, #0x35]
003ffbc0: strb r3, [sp, #0x34]
003ffbc4: str ip, [sp, #0x2c]
003ffbc8: str sl, [sp, #0x3c]
003ffbcc: str r2, [sp, #0x30]
003ffbd0: str r2, [sp, #0x38]
003ffbd4: bl #0x339090
003ffbd8: ldr r1, [sp, #0x1c]
003ffbdc: ldr r3, [fp, r1]
003ffbe0: add r3, r3, #8
003ffbe4: str r3, [sp, #0x24]
003ffbe8: ldr r0, [r4, #-4]
003ffbec: bl #0x310440
003ffbf0: ldr r3, [r7, #0xc]
003ffbf4: mov r2, r4
003ffbf8: add r4, r4, #4
003ffbfc: cmp r3, r2
003ffc00: bne #0x3ffad8
003ffc04: ldr r2, [r7, #8]
003ffc08: cmp r3, r2
003ffc0c: strne r2, [r7, #0xc]
003ffc10: mov r0, r7
003ffc14: mov r2, r6
003ffc18: ldr r1, [r7, #0x20]
003ffc1c: bl #0x3fe1a8
003ffc20: add sp, sp, #0x44
003ffc24: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ffc28: subseq r5, sb, ip
003ffc2c: strheq r0, [r0], -r0
003ffc30: strdeq r3, r4, [r0], -r4
003ffc34: andeq r1, r0, r4, lsr r4
003ffc38: strheq r2, [ip], #-0xe4
003ffc3c: subeq r7, ip, r8, lsr #19

# 0x3ff858 _ZN13ItemInventory14TransferItemToEjRS_ibb
003ff858: push {r4, r5, r6, r7, r8, sb, sl, lr}
003ff85c: mov r4, r0
003ff860: ldr r5, [r0, #0xc]
003ff864: ldr r0, [r0, #8]
003ff868: ldr ip, [pc, #0x1bc]
003ff86c: sub sp, sp, #8
003ff870: rsb r0, r0, r5
003ff874: cmp r1, r0, asr #2
003ff878: add ip, pc, ip
003ff87c: mov r6, r1
003ff880: mov sb, r2
003ff884: mov r7, r3
003ff888: ldrb sl, [sp, #0x28]
003ff88c: ldrb r8, [sp, #0x2c]
003ff890: blo #0x3ff8b8
003ff894: ldr r3, [pc, #0x194]
003ff898: ldr r3, [ip, r3]
003ff89c: ldr r3, [r3]
003ff8a0: cmp r3, #2
003ff8a4: moveq r3, #0
003ff8a8: streq r3, [r3]
003ff8ac: beq #0x3ff8b8
003ff8b0: cmp r3, #1
003ff8b4: beq #0x3ff9f0
003ff8b8: cmp r7, #0
003ff8bc: ble #0x3ffa24
003ff8c0: ldr r5, [r4, #8]
003ff8c4: cmp r6, #0
003ff8c8: addne r5, r5, r6, lsl #2
003ff8cc: ldr r3, [r5]
003ff8d0: ldr r0, [r3]
003ff8d4: ldrsh r6, [r0, #0x50]
003ff8d8: cmp r6, r7
003ff8dc: bge #0x3ff8f8
003ff8e0: cmp r6, #0
003ff8e4: moveq r7, r6
003ff8e8: bne #0x3ff930
003ff8ec: mov r0, r7
003ff8f0: add sp, sp, #8
003ff8f4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003ff8f8: bl #0x3f9e08
003ff8fc: cmp r6, r7
003ff900: beq #0x3ff934
003ff904: ldr r3, [r5]
003ff908: mov r1, r7
003ff90c: ldr r0, [r3]
003ff910: bl #0x3fc3e0
003ff914: mov r2, sl
003ff918: mov r1, r0
003ff91c: mov r3, r8
003ff920: mov r0, sb
003ff924: add sp, sp, #8
003ff928: pop {r4, r5, r6, r7, r8, sb, sl, lr}
003ff92c: b #0x3ff5d4
003ff930: bl #0x3f9e08
003ff934: ldr r3, [r5]
003ff938: ldrsb r1, [r3, #4]
003ff93c: cmn r1, #1
003ff940: beq #0x3ff95c
003ff944: ldr r3, [r4]
003ff948: mov r0, r4
003ff94c: mov r2, #0
003ff950: mov lr, pc
003ff954: ldr pc, [r3, #0x20]
003ff958: ldr r3, [r5]
003ff95c: ldrsb r1, [r3, #5]
003ff960: cmn r1, #1
003ff964: beq #0x3ff980
003ff968: ldr r3, [r4]
003ff96c: mov r0, r4
003ff970: mov r2, #1
003ff974: mov lr, pc
003ff978: ldr pc, [r3, #0x20]
003ff97c: ldr r3, [r5]
003ff980: ldr r1, [r3]
003ff984: ldr r3, [r4, #0x24]
003ff988: mov r2, sl
003ff98c: mov r0, sb
003ff990: cmp r1, r3
003ff994: moveq r3, #0
003ff998: streq r3, [r4, #0x24]
003ff99c: ldreq r3, [r5]
003ff9a0: mov r6, r5
003ff9a4: ldreq r1, [r3]
003ff9a8: mov r3, r8
003ff9ac: bl #0x3ff5d4
003ff9b0: mov r7, r0
003ff9b4: ldr r0, [r6], #4
003ff9b8: bl #0x310440
003ff9bc: ldr r3, [r4, #0xc]
003ff9c0: cmp r6, r3
003ff9c4: beq #0x3ff9e4
003ff9c8: subs r2, r3, r6
003ff9cc: moveq r6, r3
003ff9d0: beq #0x3ff9e4
003ff9d4: mov r1, r6
003ff9d8: mov r0, r5
003ff9dc: bl #0x30df38
003ff9e0: ldr r6, [r4, #0xc]
003ff9e4: sub r6, r6, #4
003ff9e8: str r6, [r4, #0xc]
003ff9ec: b #0x3ff8ec
003ff9f0: ldr r0, [pc, #0x3c]
003ff9f4: ldr r1, [pc, #0x3c]
003ff9f8: ldr r2, [pc, #0x3c]
003ff9fc: ldr r0, [ip, r0]
003ffa00: ldr r3, [pc, #0x38]
003ffa04: movw ip, #0x1aa
003ffa08: add r1, pc, r1
003ffa0c: add r2, pc, r2
003ffa10: add r3, pc, r3
003ffa14: add r0, r0, #0xa8
003ffa18: str ip, [sp]
003ffa1c: bl #0x30e004
003ffa20: b #0x3ff8b8
003ffa24: mvn r7, #0
003ffa28: b #0x3ff8ec
003ffa2c: subseq r5, sb, r8, lsl r2
003ffa30: andeq r3, r0, r0, asr #19
003ffa34: andeq r1, r0, r0, asr #19
003ffa38: ldrdeq lr, pc, [fp], #-0x90
003ffa3c: subeq r7, ip, ip, ror sb
003ffa40: umaaleq r7, ip, r8, sb

# 0x36ea50 _ZN13PlayerManager29GetNumPlayerCharactersOfClassEi
0036ea50: push {r4, r5, r6, r7, r8, lr}
0036ea54: ldr r3, [r0, #0x6c4]
0036ea58: mov r5, r0
0036ea5c: mov r6, r1
0036ea60: cmp r3, #0
0036ea64: movle r8, #0
0036ea68: ble #0x36eab0
0036ea6c: mov r4, #0
0036ea70: mov r8, r4
0036ea74: movw r7, #0x13c8
0036ea78: mov r1, r4
0036ea7c: mov r0, r5
0036ea80: mov r2, #1
0036ea84: bl #0x36e744
0036ea88: ldr r3, [r0, #0x660]
0036ea8c: add r4, r4, #1
0036ea90: cmp r3, #0
0036ea94: beq #0x36eaa4
0036ea98: ldrsh r3, [r3, r7]
0036ea9c: cmp r6, r3
0036eaa0: addeq r8, r8, #1
0036eaa4: ldr r3, [r5, #0x6c4]
0036eaa8: cmp r4, r3
0036eaac: blt #0x36ea78
0036eab0: mov r0, r8
0036eab4: pop {r4, r5, r6, r7, r8, pc}

# 0x36eab8 _ZN13PlayerManager28GetNumPlayerCharacterWarriorEv
0036eab8: movw r1, #0x107
0036eabc: b #0x36ea50

# 0x36eac0 _ZN13PlayerManager26GetNumPlayerCharacterRogueEv
0036eac0: movw r1, #0x145
0036eac4: b #0x36ea50

# 0x36eac8 _ZN13PlayerManager25GetNumPlayerCharacterMageEv
0036eac8: movw r1, #0x122
0036eacc: b #0x36ea50

# 0x36e478 _ZN13PlayerManager14GetLocalPlayerEib
0036e478: push {r4, lr}
0036e47c: mov r4, r0
0036e480: bl #0x36e2cc
0036e484: mov r2, #0
0036e488: mov r1, r0
0036e48c: mov r0, r4
0036e490: pop {r4, lr}
0036e494: b #0x36dfb0

# 0x36e2cc _ZN13PlayerManager23_GetInternalIDByLocalIDEib
0036e2cc: push {r4, r5, r6, r7, r8, lr}
0036e2d0: mov r5, r0
0036e2d4: mov r4, r1
0036e2d8: mov r6, r2
0036e2dc: bl #0x7fd794
0036e2e0: ldrb r3, [r0, #5]
0036e2e4: cmp r3, #0
0036e2e8: bne #0x36e300
0036e2ec: ldr r3, [r5, #0x6a0]
0036e2f0: cmp r4, r3
0036e2f4: blo #0x36e360
0036e2f8: mvn r0, #0
0036e2fc: pop {r4, r5, r6, r7, r8, pc}
0036e300: bl #0x320e98
0036e304: ldrb r3, [r0, #0x24]
0036e308: cmp r3, #0
0036e30c: beq #0x36e2ec
0036e310: bl #0x800f8c
0036e314: ldr r3, [r0]
0036e318: mov lr, pc
0036e31c: ldr pc, [r3, #0x64]
0036e320: cmp r0, #0
0036e324: beq #0x36e2ec
0036e328: bl #0x8100dc
0036e32c: bl #0x8100e0
0036e330: cmp r0, #0
0036e334: beq #0x36e2ec
0036e338: ldr r2, [r5, #0x6b4]
0036e33c: ldr r3, [r5, #0x6b8]
0036e340: rsb r3, r2, r3
0036e344: asr r3, r3, #2
0036e348: cmp r4, r3
0036e34c: bhs #0x36e2f8
0036e350: cmp r6, #0
0036e354: bne #0x36e3fc
0036e358: ldr r0, [r2, r4, lsl #2]
0036e35c: pop {r4, r5, r6, r7, r8, pc}
0036e360: ldr r2, [r5, #0x698]
0036e364: add ip, r5, #0x690
0036e368: mov r0, #0
0036e36c: cmp ip, r2
0036e370: beq #0x36e470
0036e374: ldrb r3, [r2, #0x684]
0036e378: cmp r3, #0
0036e37c: beq #0x36e394
0036e380: cmp r6, #0
0036e384: bne #0x36e3b8
0036e388: cmp r0, r4
0036e38c: beq #0x36e468
0036e390: add r0, r0, #1
0036e394: ldr r1, [r2, #0xc]
0036e398: cmp r1, #0
0036e39c: beq #0x36e3c8
0036e3a0: mov r2, r1
0036e3a4: ldr r3, [r2, #8]
0036e3a8: cmp r3, #0
0036e3ac: beq #0x36e36c
0036e3b0: mov r2, r3
0036e3b4: b #0x36e3a4
0036e3b8: ldr r3, [r2, #0x678]
0036e3bc: cmp r3, #0
0036e3c0: bne #0x36e388
0036e3c4: b #0x36e394
0036e3c8: ldr r3, [r2, #4]
0036e3cc: ldr r5, [r3, #0xc]
0036e3d0: cmp r2, r5
0036e3d4: bne #0x36e3f0
0036e3d8: mov r2, r3
0036e3dc: ldr r3, [r3, #4]
0036e3e0: ldr r1, [r3, #0xc]
0036e3e4: cmp r1, r2
0036e3e8: beq #0x36e3d8
0036e3ec: ldr r1, [r2, #0xc]
0036e3f0: cmp r3, r1
0036e3f4: movne r2, r3
0036e3f8: b #0x36e36c
0036e3fc: cmp r3, #0
0036e400: movne r3, #0
0036e404: movne r6, r3
0036e408: movne r7, r3
0036e40c: bne #0x36e42c
0036e410: b #0x36e470
0036e414: add r7, r7, #1
0036e418: ldr r2, [r5, #0x6b4]
0036e41c: ldr r1, [r5, #0x6b8]
0036e420: rsb r1, r2, r1
0036e424: cmp r6, r1, asr #2
0036e428: bhs #0x36e470
0036e42c: ldr r1, [r2, r3, lsl #2]
0036e430: mov r0, r5
0036e434: mov r2, #0
0036e438: lsl r8, r3, #2
0036e43c: bl #0x36dfb0
0036e440: ldr r2, [r0, #0x660]
0036e444: add r6, r6, #1
0036e448: mov r3, r6
0036e44c: cmp r2, #0
0036e450: beq #0x36e418
0036e454: cmp r7, r4
0036e458: bne #0x36e414
0036e45c: ldr r3, [r5, #0x6b4]
0036e460: ldr r0, [r3, r8]
0036e464: pop {r4, r5, r6, r7, r8, pc}
0036e468: ldr r0, [r2, #0x688]
0036e46c: pop {r4, r5, r6, r7, r8, pc}
0036e470: mvn r0, #0
0036e474: pop {r4, r5, r6, r7, r8, pc}

# 0x36dfb0 _ZN13PlayerManager21GetPlayerByInternalIDEib
0036dfb0: cmn r1, #1
0036dfb4: push {r4, r5, r6, lr}
0036dfb8: mov r4, r1
0036dfbc: mov r5, r0
0036dfc0: mov r6, r2
0036dfc4: beq #0x36e040
0036dfc8: bl #0x7fd794
0036dfcc: ldrb r3, [r0, #5]
0036dfd0: cmp r3, #0
0036dfd4: bne #0x36e048
0036dfd8: ldr r0, [r5, #0x694]
0036dfdc: add r1, r5, #0x690
0036dfe0: cmp r0, #0
0036dfe4: movne r2, r1
0036dfe8: bne #0x36dff4
0036dfec: b #0x36e038
0036dff0: mov r0, r3
0036dff4: ldr r3, [r0, #0x10]
0036dff8: cmp r4, r3
0036dffc: ldrgt r3, [r0, #0xc]
0036e000: ldrle r3, [r0, #8]
0036e004: movgt r0, r2
0036e008: mov r2, r0
0036e00c: cmp r3, #0
0036e010: bne #0x36dff0
0036e014: cmp r1, r0
0036e018: beq #0x36e094
0036e01c: ldr r3, [r0, #0x10]
0036e020: cmp r4, r3
0036e024: blt #0x36e038
0036e028: cmp r1, r0
0036e02c: beq #0x36e094
0036e030: add r0, r0, #0x18
0036e034: pop {r4, r5, r6, pc}
0036e038: mov r0, r1
0036e03c: b #0x36e028
0036e040: add r0, r0, #8
0036e044: pop {r4, r5, r6, pc}
0036e048: bl #0x320e98
0036e04c: ldrb r3, [r0, #0x24]
0036e050: cmp r3, #0
0036e054: beq #0x36dfd8
0036e058: bl #0x800f8c
0036e05c: ldr r3, [r0]
0036e060: mov lr, pc
0036e064: ldr pc, [r3, #0x64]
0036e068: cmp r0, #0
0036e06c: beq #0x36dfd8
0036e070: bl #0x8100dc
0036e074: bl #0x8100e0
0036e078: cmp r0, #0
0036e07c: beq #0x36dfd8
0036e080: mov r0, r5
0036e084: mov r1, r4
0036e088: mov r2, r6
0036e08c: pop {r4, r5, r6, lr}
0036e090: b #0x36dec4
0036e094: add r0, r5, #8
0036e098: pop {r4, r5, r6, pc}

# 0x36e744 _ZN13PlayerManager9GetPlayerEib
0036e744: push {r4, lr}
0036e748: mov r4, r0
0036e74c: bl #0x36e5b4
0036e750: mov r2, #0
0036e754: mov r1, r0
0036e758: mov r0, r4
0036e75c: pop {r4, lr}
0036e760: b #0x36dfb0
