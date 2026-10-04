
# _ZNK14PlayerSavegame15SG_GetSkillSlotEj
0046752c: push     {r4, r5, r6, lr}
00467530: ldr      r3, [r0, #0x84]
00467534: ldr      r6, [pc, #0x184]
00467538: sub      sp, sp, #8
0046753c: cmp      r3, r1
00467540: mov      r5, r0
00467544: mov      r4, r1
00467548: add      r6, pc, r6
0046754c: bhi      #0x467574
00467550: ldr      r3, [pc, #0x16c]
00467554: ldr      r3, [r6, r3]
00467558: ldr      r3, [r3]
0046755c: cmp      r3, #2
00467560: moveq    r3, #0
00467564: streq    r3, [r3]
00467568: beq      #0x467574
0046756c: cmp      r3, #1
00467570: beq      #0x467638
00467574: ldr      r3, [r5, #0x80]
00467578: cmp      r3, #0
0046757c: beq      #0x46766c
00467580: ldr      r0, [r5, #0x10]
00467584: mvn      r1, #0
00467588: add      r0, r0, #0x37c
0046758c: bl       #0x3fc6a0
00467590: ldr      r3, [r5, #0x88]
00467594: mov      r2, #0x18
00467598: mla      r3, r2, r0, r3
0046759c: ldr      r1, [r3, #8]
004675a0: cmp      r1, r3
004675a4: beq      #0x46762c
004675a8: ldr      r2, [r1, #0x14]
004675ac: cmp      r4, r2
004675b0: beq      #0x4675ec
004675b4: ldr      ip, [r1, #0xc]
004675b8: cmp      ip, #0
004675bc: bne      #0x4675c8
004675c0: b        #0x4675f4
004675c4: mov      ip, r1
004675c8: ldr      r1, [ip, #8]
004675cc: cmp      r1, #0
004675d0: bne      #0x4675c4
004675d4: mov      r1, ip
004675d8: cmp      r3, r1
004675dc: beq      #0x46762c
004675e0: ldr      ip, [r1, #0x14]
004675e4: cmp      r4, ip
004675e8: bne      #0x4675b4
004675ec: ldr      r0, [r1, #0x10]
004675f0: b        #0x467630
004675f4: ldr      r5, [r1, #4]
004675f8: ldr      r2, [r5, #0xc]
004675fc: cmp      r2, r1
00467600: bne      #0x46761c
00467604: mov      r1, r5
00467608: ldr      r5, [r5, #4]
0046760c: ldr      ip, [r5, #0xc]
00467610: cmp      ip, r1
00467614: beq      #0x467604
00467618: ldr      ip, [r1, #0xc]
0046761c: cmp      r5, ip
00467620: movne    r1, r5
00467624: cmp      r3, r1
00467628: bne      #0x4675e0
0046762c: mvn      r0, #0
00467630: add      sp, sp, #8
00467634: pop      {r4, r5, r6, pc}
00467638: ldr      r0, [pc, #0x88]
0046763c: ldr      r1, [pc, #0x88]
00467640: ldr      r2, [pc, #0x88]
00467644: ldr      r0, [r6, r0]
00467648: ldr      r3, [pc, #0x84]
0046764c: mov      ip, #0xc0
00467650: add      r1, pc, r1
00467654: add      r2, pc, r2
00467658: add      r3, pc, r3
0046765c: add      r0, r0, #0xa8
00467660: str      ip, [sp]
00467664: bl       #0x30e004
00467668: b        #0x467574
0046766c: ldr      r2, [pc, #0x50]
00467670: ldr      r2, [r6, r2]
00467674: ldr      r2, [r2]
00467678: cmp      r2, #2
0046767c: streq    r3, [r3]
00467680: beq      #0x467580
00467684: cmp      r2, #1
00467688: bne      #0x467580
0046768c: ldr      r0, [pc, #0x34]
00467690: ldr      r1, [pc, #0x40]
00467694: ldr      r2, [pc, #0x40]
00467698: ldr      r0, [r6, r0]
0046769c: ldr      r3, [pc, #0x3c]
004676a0: mov      ip, #0xc1
004676a4: add      r1, pc, r1
004676a8: add      r2, pc, r2
004676ac: add      r3, pc, r3
004676b0: add      r0, r0, #0xa8
004676b4: str      ip, [sp]
004676b8: bl       #0x30e004
004676bc: b        #0x467580
004676c0: subseq   sp, r2, r8, asr #10
004676c4: andeq    r3, r0, r0, asr #19
004676c8: andeq    r1, r0, r0, asr #19
004676cc: subeq    r6, r5, r8, lsl #27
004676d0: subeq    r5, r6, ip, ror ip
004676d4: subeq    r5, r6, r8, lsr #24
004676d8: subeq    r6, r5, r4, lsr sp
004676dc: subeq    r5, r6, r0, asr #24
004676e0: ldrdeq   r5, r6, [r6], #-0xb4

# _ZNK14PlayerSavegame17SG_GetFaerieLevelEji
00466700: push     {r4, r5, r6, r7, lr}
00466704: add      r7, r2, #0x28
00466708: mov      r6, r2
0046670c: ldr      r2, [r0, r7, lsl #2]
00466710: ldr      r3, [pc, #0x94]
00466714: sub      sp, sp, #0xc
00466718: cmp      r2, r1
0046671c: mov      r5, r0
00466720: mov      r4, r1
00466724: add      r3, pc, r3
00466728: bhi      #0x466798
0046672c: ldr      r2, [pc, #0x7c]
00466730: ldr      r2, [r3, r2]
00466734: ldr      r2, [r2]
00466738: cmp      r2, #2
0046673c: moveq    r0, #0
00466740: streq    r0, [r0]
00466744: beq      #0x466754
00466748: cmp      r2, #1
0046674c: beq      #0x46675c
00466750: mov      r0, #0
00466754: add      sp, sp, #0xc
00466758: pop      {r4, r5, r6, r7, pc}
0046675c: ldr      r0, [pc, #0x50]
00466760: ldr      r1, [pc, #0x50]
00466764: ldr      r2, [pc, #0x50]
00466768: ldr      r0, [r3, r0]
0046676c: ldr      r3, [pc, #0x4c]
00466770: mov      ip, #0x128
00466774: add      r1, pc, r1
00466778: add      r3, pc, r3
0046677c: add      r0, r0, #0xa8
00466780: add      r2, pc, r2
00466784: str      ip, [sp]
00466788: bl       #0x30e004
0046678c: ldr      r3, [r5, r7, lsl #2]
00466790: cmp      r4, r3
00466794: bhs      #0x466750
00466798: add      r5, r5, r6, lsl #2
0046679c: ldr      r3, [r5, #0x94]
004667a0: add      r4, r3, r4, lsl #2
004667a4: ldrh     r0, [r4, #2]
004667a8: b        #0x466754
004667ac: subseq   lr, r2, ip, ror #6
004667b0: andeq    r3, r0, r0, asr #19
004667b4: andeq    r1, r0, r0, asr #19
004667b8: subeq    r7, r5, r4, ror #24
004667bc: subeq    r6, r6, r0, ror #21
004667c0: subeq    r6, r6, r8, lsl #22

# _ZNK9Character18GetCharSkillListIdEv
003bc5c0: movw     r3, #0x1068
003bc5c4: ldr      r0, [r0, r3]
003bc5c8: ldr      r3, [pc, #0x24]
003bc5cc: cmp      r0, #0
003bc5d0: add      r3, pc, r3
003bc5d4: blt      #0x3bc5ec
003bc5d8: ldr      r2, [pc, #0x18]
003bc5dc: ldr      r3, [r3, r2]
003bc5e0: ldr      r3, [r3]
003bc5e4: cmp      r0, r3
003bc5e8: bxlt     lr
003bc5ec: mov      r0, #3
003bc5f0: bx       lr
003bc5f4: subseq   r8, sp, r0, asr #9
003bc5f8: andeq    r2, r0, r8, ror sp

# _ZNK14PlayerSavegame13SG_GetSkillIdEj
00466a08: push     {r4, r5, r6, lr}
00466a0c: ldr      r3, [r0, #0x84]
00466a10: ldr      r4, [pc, #0xdc]
00466a14: sub      sp, sp, #8
00466a18: cmp      r3, r1
00466a1c: mov      r5, r0
00466a20: mov      r6, r1
00466a24: add      r4, pc, r4
00466a28: bhi      #0x466a50
00466a2c: ldr      r3, [pc, #0xc4]
00466a30: ldr      r3, [r4, r3]
00466a34: ldr      r3, [r3]
00466a38: cmp      r3, #2
00466a3c: moveq    r3, #0
00466a40: streq    r3, [r3]
00466a44: beq      #0x466a50
00466a48: cmp      r3, #1
00466a4c: beq      #0x466ac0
00466a50: ldr      r3, [r5, #0x80]
00466a54: cmp      r3, #0
00466a58: beq      #0x466a68
00466a5c: ldr      r0, [r3, r6, lsl #3]
00466a60: add      sp, sp, #8
00466a64: pop      {r4, r5, r6, pc}
00466a68: ldr      r2, [pc, #0x88]
00466a6c: ldr      r2, [r4, r2]
00466a70: ldr      r2, [r2]
00466a74: cmp      r2, #2
00466a78: streq    r3, [r3]
00466a7c: beq      #0x466a5c
00466a80: cmp      r2, #1
00466a84: bne      #0x466a5c
00466a88: ldr      r0, [pc, #0x6c]
00466a8c: ldr      r1, [pc, #0x6c]
00466a90: ldr      r2, [pc, #0x6c]
00466a94: ldr      r0, [r4, r0]
00466a98: ldr      r3, [pc, #0x68]
00466a9c: mov      ip, #0x9c
00466aa0: add      r1, pc, r1
00466aa4: add      r3, pc, r3
00466aa8: add      r0, r0, #0xa8
00466aac: add      r2, pc, r2
00466ab0: str      ip, [sp]
00466ab4: bl       #0x30e004
00466ab8: ldr      r3, [r5, #0x80]
00466abc: b        #0x466a5c
00466ac0: ldr      r0, [pc, #0x34]
00466ac4: ldr      r1, [pc, #0x40]
00466ac8: ldr      r2, [pc, #0x40]
00466acc: ldr      r0, [r4, r0]
00466ad0: ldr      r3, [pc, #0x3c]
00466ad4: mov      ip, #0x9b
00466ad8: add      r1, pc, r1
00466adc: add      r2, pc, r2
00466ae0: add      r3, pc, r3
00466ae4: add      r0, r0, #0xa8
00466ae8: str      ip, [sp]
00466aec: bl       #0x30e004
00466af0: b        #0x466a50
00466af4: subseq   lr, r2, ip, rrx
00466af8: andeq    r3, r0, r0, asr #19
00466afc: andeq    r1, r0, r0, asr #19
00466b00: subeq    r7, r5, r8, lsr sb
00466b04: subeq    r6, r6, ip, lsr r8
00466b08: ldrdeq   r6, r7, [r6], #-0x7c
00466b0c: subeq    r7, r5, r0, lsl #18
00466b10: strdeq   r6, r7, [r6], #-0x74
00466b14: subeq    r6, r6, r0, lsr #15

# _ZNK9Character18GetCharFaeryListIdEv
003ae5a0: movw     r3, #0x106c
003ae5a4: ldr      r0, [r0, r3]
003ae5a8: ldr      r3, [pc, #0x24]
003ae5ac: cmp      r0, #0
003ae5b0: add      r3, pc, r3
003ae5b4: blt      #0x3ae5cc
003ae5b8: ldr      r2, [pc, #0x18]
003ae5bc: ldr      r3, [r3, r2]
003ae5c0: ldr      r3, [r3]
003ae5c4: cmp      r0, r3
003ae5c8: bxlt     lr
003ae5cc: mov      r0, #0
003ae5d0: bx       lr
003ae5d4: subseq   r6, lr, r0, ror #9
003ae5d8: andeq    r4, r0, r4, lsl r5
