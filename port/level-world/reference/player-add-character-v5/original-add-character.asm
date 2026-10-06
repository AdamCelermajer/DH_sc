# 0x372220 _ZN13PlayerManager13_AddCharacterEi
00372220: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00372224: ldr r4, [pc, #0x45c]
00372228: ldr r7, [pc, #0x45c]
0037222c: sub sp, sp, #0x24c
00372230: add r4, pc, r4
00372234: ldr r3, [r4, r7]
00372238: mov r2, #0
0037223c: mov fp, r0
00372240: ldr r3, [r3]
00372244: str r1, [sp, #8]
00372248: str r3, [sp, #0x244]
0037224c: bl #0x36dfb0
00372250: ldr r3, [r0, #0x380]
00372254: mov r6, r0
00372258: ldr r2, [r0, #0x660]
0037225c: cmn r3, #1
00372260: beq #0x37226c
00372264: cmp r2, #0
00372268: beq #0x372288
0037226c: ldr r3, [r4, r7]
00372270: ldr r2, [sp, #0x244]
00372274: ldr r3, [r3]
00372278: cmp r2, r3
0037227c: bne #0x372684
00372280: add sp, sp, #0x24c
00372284: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00372288: ldr r2, [pc, #0x400]
0037228c: ldr r1, [pc, #0x400]
00372290: add r5, sp, #0x24
00372294: str r2, [sp, #0xc]
00372298: add r1, pc, r1
0037229c: ldr r2, [sp, #8]
003722a0: mov r0, r5
003722a4: bl #0x30eae4
003722a8: ldr r2, [sp, #0xc]
003722ac: add r8, sp, #0x14
003722b0: mov ip, #1
003722b4: ldr r3, [r4, r2]
003722b8: ldr r2, [pc, #0x3d8]
003722bc: mov r0, r8
003722c0: ldr r1, [r3, #0x38]
003722c4: add r2, pc, r2
003722c8: mov r3, r5
003722cc: str ip, [sp, #4]
003722d0: str ip, [sp]
003722d4: bl #0x34b724
003722d8: mov r0, r8
003722dc: bl #0x33ff54
003722e0: subs r5, r0, #0
003722e4: beq #0x3725fc
003722e8: mov r0, r5
003722ec: str r5, [r6, #0x660]
003722f0: bl #0x3b36b0
003722f4: ldr r3, [r6]
003722f8: mov r0, r6
003722fc: mov lr, pc
00372300: ldr pc, [r3, #0x50]
00372304: cmp r0, #0
00372308: bne #0x3724e4
0037230c: mov r0, r5
00372310: ldr r1, [r6, #0x380]
00372314: bl #0x3bb814
00372318: ldr r2, [r6, #0x674]
0037231c: movw r3, #0x1f88
00372320: str r2, [r5, r3]
00372324: ldr r2, [r6, #0x670]
00372328: movw r3, #0x1f8c
0037232c: str r2, [r5, r3]
00372330: bl #0x7fd794
00372334: ldrb r3, [r0, #5]
00372338: cmp r3, #0
0037233c: bne #0x372518
00372340: mov r0, r5
00372344: bl #0x3b35f0
00372348: ldr r3, [r6]
0037234c: mov r0, r6
00372350: mov lr, pc
00372354: ldr pc, [r3, #0x50]
00372358: cmp r0, #0
0037235c: bne #0x3724f4
00372360: add r0, r5, #0x4f0
00372364: add r0, r0, #0xc
00372368: mov r1, #0
0037236c: add sl, sp, #0x20
00372370: bl #0x3c1a00
00372374: add r0, r6, #0x3d8
00372378: mov r1, sl
0037237c: mov r2, #3
00372380: bl #0x36d730
00372384: mov r8, #0
00372388: mvn sb, #0
0037238c: ldrsb r2, [sl, r8]
00372390: mov r1, r8
00372394: mov r0, r5
00372398: cmn r2, #1
0037239c: strblt sb, [sl, r8]
003723a0: mvnlt r2, #0
003723a4: add r8, r8, #1
003723a8: bl #0x3bbe54
003723ac: cmp r8, #3
003723b0: bne #0x37238c
003723b4: movw r3, #0x14e8
003723b8: ldr r3, [r5, r3]
003723bc: cmp r3, #0
003723c0: beq #0x3723f4
003723c4: ldr r3, [r3, #0x84]
003723c8: cmp r3, #0x1e
003723cc: bls #0x3723f4
003723d0: ldr r3, [pc, #0x2c4]
003723d4: ldr r3, [r4, r3]
003723d8: ldr r3, [r3]
003723dc: cmp r3, #2
003723e0: moveq r3, #0
003723e4: streq r3, [r3]
003723e8: beq #0x3723f4
003723ec: cmp r3, #1
003723f0: beq #0x372650
003723f4: add sb, sp, #0x224
003723f8: add r0, r6, #0x400
003723fc: mov r1, sb
00372400: mov r2, #0x1e
00372404: bl #0x36d76c
00372408: mov r8, #0
0037240c: movw sl, #0x14e8
00372410: ldr r3, [r5, sl]
00372414: cmp r3, #0
00372418: beq #0x372440
0037241c: ldr r3, [r3, #0x84]
00372420: cmp r3, r8
00372424: bls #0x372440
00372428: ldrsb r2, [sb, r8]
0037242c: cmp r2, #0
00372430: blt #0x372440
00372434: mov r0, r5
00372438: mov r1, r8
0037243c: bl #0x3bbebc
00372440: add r8, r8, #1
00372444: cmp r8, #0x1e
00372448: bne #0x372410
0037244c: ldr r3, [r6]
00372450: mov r0, r6
00372454: mov lr, pc
00372458: ldr pc, [r3, #0x50]
0037245c: cmp r0, #0
00372460: movne r1, #1
00372464: ldrbeq r1, [r6, #0x4e5]
00372468: ldr r3, [r5]
0037246c: mov r0, r5
00372470: mov lr, pc
00372474: ldr pc, [r3, #0x40]
00372478: ldr r3, [sp, #0xc]
0037247c: ldr r0, [r4, r3]
00372480: bl #0x31f594
00372484: cmp r0, #0
00372488: beq #0x372494
0037248c: mov r1, #0
00372490: bl #0x3f059c
00372494: ldr r3, [fp, #0x6c4]
00372498: add r3, r3, #1
0037249c: str r3, [fp, #0x6c4]
003724a0: bl #0x7fd794
003724a4: ldrb r3, [r0, #5]
003724a8: cmp r3, #0
003724ac: bne #0x3725d0
003724b0: mov r0, fp
003724b4: ldr r1, [sp, #8]
003724b8: bl #0x36f0dc
003724bc: mov r0, r6
003724c0: ldr r3, [r6]
003724c4: mov lr, pc
003724c8: ldr pc, [r3, #0x50]
003724cc: cmp r0, #0
003724d0: beq #0x37226c
003724d4: mov r0, fp
003724d8: ldr r1, [sp, #8]
003724dc: bl #0x371050
003724e0: b #0x37226c
003724e4: mov r0, r5
003724e8: ldr r1, [r6, #0x664]
003724ec: bl #0x3bb740
003724f0: b #0x372318
003724f4: ldr r3, [r6]
003724f8: mov r0, r6
003724fc: mov lr, pc
00372500: ldr pc, [r3, #0x5c]
00372504: cmp r0, #0
00372508: beq #0x372360
0037250c: mov r0, r5
00372510: bl #0x3b4bc4
00372514: b #0x372360
00372518: mov r0, r6
0037251c: bl #0x80f23c
00372520: cmp r0, #0
00372524: bne #0x372340
00372528: mov r0, fp
0037252c: bl #0x36e09c
00372530: ldr r8, [r0, #0x660]
00372534: cmp r8, #0
00372538: beq #0x372340
0037253c: mov r2, #1
00372540: add r1, r8, #0x160
00372544: mov r0, r5
00372548: bl #0x393db4
0037254c: mov r0, r5
00372550: add r1, r8, #0x16c
00372554: bl #0x3938a0
00372558: add r1, r8, #0x1440
0037255c: mov r0, r5
00372560: add r1, r1, #0x10
00372564: bl #0x3a58f4
00372568: movw r1, #0x145c
0037256c: ldr r0, [r8, r1]
00372570: movw r2, #0x1460
00372574: movw r3, #0x1464
00372578: str r0, [r5, r1]
0037257c: ldr r1, [r8, r2]
00372580: str r1, [r5, r2]
00372584: ldr r2, [r8, r3]
00372588: str r2, [r5, r3]
0037258c: ldr r0, [r8, #0x2f4]
00372590: cmp r0, #0
00372594: beq #0x3725a8
00372598: mov r1, r5
0037259c: bl #0x396a90
003725a0: cmp r0, #0
003725a4: bne #0x372340
003725a8: ldr r2, [sp, #0xc]
003725ac: mov r1, r5
003725b0: ldr r3, [r4, r2]
003725b4: ldr r0, [r3, #0x38]
003725b8: bl #0x344184
003725bc: mov r3, #1
003725c0: strb r3, [r5, #0x2ef]
003725c4: mov r0, r5
003725c8: bl #0x38c710
003725cc: b #0x372340
003725d0: mov r0, fp
003725d4: ldr r1, [sp, #8]
003725d8: mov r2, #1
003725dc: bl #0x36dfb0
003725e0: ldr r3, [r0, #0x660]
003725e4: cmp r5, r3
003725e8: beq #0x3724b0
003725ec: mov r0, fp
003725f0: mov r1, r5
003725f4: bl #0x371d80
003725f8: b #0x37226c
003725fc: ldr r3, [pc, #0x98]
00372600: ldr r3, [r4, r3]
00372604: ldr r3, [r3]
00372608: cmp r3, #2
0037260c: streq r5, [r5]
00372610: beq #0x3722e8
00372614: cmp r3, #1
00372618: bne #0x3722e8
0037261c: ldr r0, [pc, #0x7c]
00372620: ldr r1, [pc, #0x7c]
00372624: ldr r2, [pc, #0x7c]
00372628: ldr r0, [r4, r0]
0037262c: ldr r3, [pc, #0x78]
00372630: movw ip, #0x464
00372634: add r1, pc, r1
00372638: add r2, pc, r2
0037263c: add r3, pc, r3
00372640: add r0, r0, #0xa8
00372644: str ip, [sp]
00372648: bl #0x30e004
0037264c: b #0x3722e8
00372650: ldr r0, [pc, #0x48]
00372654: ldr r1, [pc, #0x54]
00372658: ldr r2, [pc, #0x54]
0037265c: ldr r0, [r4, r0]
00372660: ldr r3, [pc, #0x50]
00372664: movw ip, #0x4a4
00372668: add r1, pc, r1
0037266c: add r2, pc, r2
00372670: add r3, pc, r3
00372674: add r0, r0, #0xa8
00372678: str ip, [sp]
0037267c: bl #0x30e004
00372680: b #0x3723f4
00372684: bl #0x30e310
00372688: rsbeq r2, r2, r0, ror #16
0037268c: andeq r4, r0, ip, lsr #1
00372690: strdeq r3, r4, [r0], -r4
00372694: subseq pc, r4, r0, lsr r5
00372698: ldrsheq lr, [r4], #-0x14
0037269c: andeq r3, r0, r0, asr #19
003726a0: andeq r1, r0, r0, asr #19
003726a4: subseq fp, r4, r4, lsr #27
003726a8: subseq pc, r7, r0, asr sl
003726ac: ldrheq pc, [r4], #-4
003726b0: subseq fp, r4, r0, ror sp
003726b4: subseq pc, r4, r4, ror r1
003726b8: subseq pc, r4, r0, lsl #1
