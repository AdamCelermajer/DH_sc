# _ZN18NetStructByteArrayILj3EE9GetBufferEPvi 36d730 60
0036d730 push {r4, lr}
0036d734 ldr r3, [r0, #0x20]
0036d738 mov r4, r0
0036d73c cmp r3, #0
0036d740 beq #0x36d764
0036d744 ldr r2, [r0, #0x24]
0036d748 cmp r2, #0
0036d74c ble #0x36d764
0036d750 mov r0, r1
0036d754 mov r1, r3
0036d758 bl #0x30e868
0036d75c ldr r0, [r4, #0x24]
0036d760 pop {r4, pc}
0036d764 mov r0, #0
0036d768 pop {r4, pc}
# _ZN18NetStructByteArrayILj30EE9GetBufferEPvi 36d76c 60
0036d76c push {r4, lr}
0036d770 ldr r3, [r0, #0x20]
0036d774 mov r4, r0
0036d778 cmp r3, #0
0036d77c beq #0x36d7a0
0036d780 ldr r2, [r0, #0x24]
0036d784 cmp r2, #0
0036d788 ble #0x36d7a0
0036d78c mov r0, r1
0036d790 mov r1, r3
0036d794 bl #0x30e868
0036d798 ldr r0, [r4, #0x24]
0036d79c pop {r4, pc}
0036d7a0 mov r0, #0
0036d7a4 pop {r4, pc}
# _ZN13PlayerManager13GetNumPlayersEv 36d7a8 108
0036d7a8 push {r4, lr}
0036d7ac mov r4, r0
0036d7b0 bl #0x7fd794
0036d7b4 ldrb r3, [r0, #5]
0036d7b8 cmp r3, #0
0036d7bc bne #0x36d7c8
0036d7c0 ldr r0, [r4, #0x6a0]
0036d7c4 pop {r4, pc}
0036d7c8 bl #0x320e98
0036d7cc ldrb r3, [r0, #0x24]
0036d7d0 cmp r3, #0
0036d7d4 beq #0x36d7c0
0036d7d8 bl #0x800f8c
0036d7dc ldr r3, [r0]
0036d7e0 mov lr, pc
0036d7e4 ldr pc, [r3, #0x64]
0036d7e8 cmp r0, #0
0036d7ec beq #0x36d7c0
0036d7f0 bl #0x8100dc
0036d7f4 bl #0x8100e0
0036d7f8 cmp r0, #0
0036d7fc beq #0x36d7c0
0036d800 ldr r3, [r4, #0x6a8]
0036d804 ldr r0, [r4, #0x6ac]
0036d808 rsb r0, r3, r0
0036d80c asr r0, r0, #2
0036d810 pop {r4, pc}
# _ZN18NetStructByteArrayILj3EEC1E9ByteArray 370608 184
00370608 push {r4, r5, r6, r7, lr}
0037060c sub sp, sp, #0xc
00370610 mov r4, r0
00370614 mov r5, #0
00370618 ldr r2, [r1, #4]
0037061c mov r0, sp
00370620 ldr r1, [r1]
00370624 ldr r6, [pc, #0x88]
00370628 str r5, [sp]
0037062c str r5, [sp, #4]
00370630 bl #0x36ddac
00370634 ldr r3, [pc, #0x7c]
00370638 add r6, pc, r6
0037063c mov r1, #0x18
00370640 ldr r3, [r6, r3]
00370644 mvn r2, #0
00370648 str r1, [r4, #4]
0037064c add r3, r3, #8
00370650 mov r0, #0
00370654 mov r1, #0
00370658 strd r0, r1, [r4, #8]
0037065c mov r0, r4
00370660 str r2, [r4, #0x14]
00370664 str r3, [r4]
00370668 str r5, [r4, #0x24]
0037066c str r2, [r4, #0x10]
00370670 str r5, [r4, #0x18]
00370674 strb r5, [r4, #0x1c]
00370678 str r5, [r4, #0x20]
0037067c mov r1, sp
00370680 bl #0x36f264
00370684 ldr r0, [sp]
00370688 mov r7, sp
0037068c cmp r0, r5
00370690 beq #0x370698
00370694 bl #0x310440
00370698 ldr r3, [pc, #0x1c]
0037069c mov r0, r4
003706a0 ldr r3, [r6, r3]
003706a4 add r3, r3, #8
003706a8 str r3, [r4]
003706ac add sp, sp, #0xc
003706b0 pop {r4, r5, r6, r7, pc}
003706b4 rsbeq r4, r2, r8, asr r4
003706b8 andeq r2, r0, ip, ror #21
003706bc strdeq r1, r2, [r0], -r8
# _ZN18NetStructByteArrayILj30EEC1E9ByteArray 3706c0 184
003706c0 push {r4, r5, r6, r7, lr}
003706c4 sub sp, sp, #0xc
003706c8 mov r4, r0
003706cc mov r5, #0
003706d0 ldr r2, [r1, #4]
003706d4 mov r0, sp
003706d8 ldr r1, [r1]
003706dc ldr r6, [pc, #0x88]
003706e0 str r5, [sp]
003706e4 str r5, [sp, #4]
003706e8 bl #0x36ddac
003706ec ldr r3, [pc, #0x7c]
003706f0 add r6, pc, r6
003706f4 mov r1, #0xf0
003706f8 ldr r3, [r6, r3]
003706fc mvn r2, #0
00370700 str r1, [r4, #4]
00370704 add r3, r3, #8
00370708 mov r0, #0
0037070c mov r1, #0
00370710 strd r0, r1, [r4, #8]
00370714 mov r0, r4
00370718 str r2, [r4, #0x14]
0037071c str r3, [r4]
00370720 str r5, [r4, #0x24]
00370724 str r2, [r4, #0x10]
00370728 str r5, [r4, #0x18]
0037072c strb r5, [r4, #0x1c]
00370730 str r5, [r4, #0x20]
00370734 mov r1, sp
00370738 bl #0x36f264
0037073c ldr r0, [sp]
00370740 mov r7, sp
00370744 cmp r0, r5
00370748 beq #0x370750
0037074c bl #0x310440
00370750 ldr r3, [pc, #0x1c]
00370754 mov r0, r4
00370758 ldr r3, [r6, r3]
0037075c add r3, r3, #8
00370760 str r3, [r4]
00370764 add sp, sp, #0xc
00370768 pop {r4, r5, r6, r7, pc}
0037076c rsbeq r4, r2, r0, lsr #7
00370770 andeq r2, r0, ip, ror #21
00370774 andeq r2, r0, ip, lsr #17
# _ZN13PlayerManager13_AddCharacterEi 372220 1180
00372220 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00372224 ldr r4, [pc, #0x45c]
00372228 ldr r7, [pc, #0x45c]
0037222c sub sp, sp, #0x24c
00372230 add r4, pc, r4
00372234 ldr r3, [r4, r7]
00372238 mov r2, #0
0037223c mov fp, r0
00372240 ldr r3, [r3]
00372244 str r1, [sp, #8]
00372248 str r3, [sp, #0x244]
0037224c bl #0x36dfb0
00372250 ldr r3, [r0, #0x380]
00372254 mov r6, r0
00372258 ldr r2, [r0, #0x660]
0037225c cmn r3, #1
00372260 beq #0x37226c
00372264 cmp r2, #0
00372268 beq #0x372288
0037226c ldr r3, [r4, r7]
00372270 ldr r2, [sp, #0x244]
00372274 ldr r3, [r3]
00372278 cmp r2, r3
0037227c bne #0x372684
00372280 add sp, sp, #0x24c
00372284 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00372288 ldr r2, [pc, #0x400]
0037228c ldr r1, [pc, #0x400]
00372290 add r5, sp, #0x24
00372294 str r2, [sp, #0xc]
00372298 add r1, pc, r1
0037229c ldr r2, [sp, #8]
003722a0 mov r0, r5
003722a4 bl #0x30eae4
003722a8 ldr r2, [sp, #0xc]
003722ac add r8, sp, #0x14
003722b0 mov ip, #1
003722b4 ldr r3, [r4, r2]
003722b8 ldr r2, [pc, #0x3d8]
003722bc mov r0, r8
003722c0 ldr r1, [r3, #0x38]
003722c4 add r2, pc, r2
003722c8 mov r3, r5
003722cc str ip, [sp, #4]
003722d0 str ip, [sp]
003722d4 bl #0x34b724
003722d8 mov r0, r8
003722dc bl #0x33ff54
003722e0 subs r5, r0, #0
003722e4 beq #0x3725fc
003722e8 mov r0, r5
003722ec str r5, [r6, #0x660]
003722f0 bl #0x3b36b0
003722f4 ldr r3, [r6]
003722f8 mov r0, r6
003722fc mov lr, pc
00372300 ldr pc, [r3, #0x50]
00372304 cmp r0, #0
00372308 bne #0x3724e4
0037230c mov r0, r5
00372310 ldr r1, [r6, #0x380]
00372314 bl #0x3bb814
00372318 ldr r2, [r6, #0x674]
0037231c movw r3, #0x1f88
00372320 str r2, [r5, r3]
00372324 ldr r2, [r6, #0x670]
00372328 movw r3, #0x1f8c
0037232c str r2, [r5, r3]
00372330 bl #0x7fd794
00372334 ldrb r3, [r0, #5]
00372338 cmp r3, #0
0037233c bne #0x372518
00372340 mov r0, r5
00372344 bl #0x3b35f0
00372348 ldr r3, [r6]
0037234c mov r0, r6
00372350 mov lr, pc
00372354 ldr pc, [r3, #0x50]
00372358 cmp r0, #0
0037235c bne #0x3724f4
00372360 add r0, r5, #0x4f0
00372364 add r0, r0, #0xc
00372368 mov r1, #0
0037236c add sl, sp, #0x20
00372370 bl #0x3c1a00
00372374 add r0, r6, #0x3d8
00372378 mov r1, sl
0037237c mov r2, #3
00372380 bl #0x36d730
00372384 mov r8, #0
00372388 mvn sb, #0
0037238c ldrsb r2, [sl, r8]
00372390 mov r1, r8
00372394 mov r0, r5
00372398 cmn r2, #1
0037239c strblt sb, [sl, r8]
003723a0 mvnlt r2, #0
003723a4 add r8, r8, #1
003723a8 bl #0x3bbe54
003723ac cmp r8, #3
003723b0 bne #0x37238c
003723b4 movw r3, #0x14e8
003723b8 ldr r3, [r5, r3]
003723bc cmp r3, #0
003723c0 beq #0x3723f4
003723c4 ldr r3, [r3, #0x84]
003723c8 cmp r3, #0x1e
003723cc bls #0x3723f4
003723d0 ldr r3, [pc, #0x2c4]
003723d4 ldr r3, [r4, r3]
003723d8 ldr r3, [r3]
003723dc cmp r3, #2
003723e0 moveq r3, #0
003723e4 streq r3, [r3]
003723e8 beq #0x3723f4
003723ec cmp r3, #1
003723f0 beq #0x372650
003723f4 add sb, sp, #0x224
003723f8 add r0, r6, #0x400
003723fc mov r1, sb
00372400 mov r2, #0x1e
00372404 bl #0x36d76c
00372408 mov r8, #0
0037240c movw sl, #0x14e8
00372410 ldr r3, [r5, sl]
00372414 cmp r3, #0
00372418 beq #0x372440
0037241c ldr r3, [r3, #0x84]
00372420 cmp r3, r8
00372424 bls #0x372440
00372428 ldrsb r2, [sb, r8]
0037242c cmp r2, #0
00372430 blt #0x372440
00372434 mov r0, r5
00372438 mov r1, r8
0037243c bl #0x3bbebc
00372440 add r8, r8, #1
00372444 cmp r8, #0x1e
00372448 bne #0x372410
0037244c ldr r3, [r6]
00372450 mov r0, r6
00372454 mov lr, pc
00372458 ldr pc, [r3, #0x50]
0037245c cmp r0, #0
00372460 movne r1, #1
00372464 ldrbeq r1, [r6, #0x4e5]
00372468 ldr r3, [r5]
0037246c mov r0, r5
00372470 mov lr, pc
00372474 ldr pc, [r3, #0x40]
00372478 ldr r3, [sp, #0xc]
0037247c ldr r0, [r4, r3]
00372480 bl #0x31f594
00372484 cmp r0, #0
00372488 beq #0x372494
0037248c mov r1, #0
00372490 bl #0x3f059c
00372494 ldr r3, [fp, #0x6c4]
00372498 add r3, r3, #1
0037249c str r3, [fp, #0x6c4]
003724a0 bl #0x7fd794
003724a4 ldrb r3, [r0, #5]
003724a8 cmp r3, #0
003724ac bne #0x3725d0
003724b0 mov r0, fp
003724b4 ldr r1, [sp, #8]
003724b8 bl #0x36f0dc
003724bc mov r0, r6
003724c0 ldr r3, [r6]
003724c4 mov lr, pc
003724c8 ldr pc, [r3, #0x50]
003724cc cmp r0, #0
003724d0 beq #0x37226c
003724d4 mov r0, fp
003724d8 ldr r1, [sp, #8]
003724dc bl #0x371050
003724e0 b #0x37226c
003724e4 mov r0, r5
003724e8 ldr r1, [r6, #0x664]
003724ec bl #0x3bb740
003724f0 b #0x372318
003724f4 ldr r3, [r6]
003724f8 mov r0, r6
003724fc mov lr, pc
00372500 ldr pc, [r3, #0x5c]
00372504 cmp r0, #0
00372508 beq #0x372360
0037250c mov r0, r5
00372510 bl #0x3b4bc4
00372514 b #0x372360
00372518 mov r0, r6
0037251c bl #0x80f23c
00372520 cmp r0, #0
00372524 bne #0x372340
00372528 mov r0, fp
0037252c bl #0x36e09c
00372530 ldr r8, [r0, #0x660]
00372534 cmp r8, #0
00372538 beq #0x372340
0037253c mov r2, #1
00372540 add r1, r8, #0x160
00372544 mov r0, r5
00372548 bl #0x393db4
0037254c mov r0, r5
00372550 add r1, r8, #0x16c
00372554 bl #0x3938a0
00372558 add r1, r8, #0x1440
0037255c mov r0, r5
00372560 add r1, r1, #0x10
00372564 bl #0x3a58f4
00372568 movw r1, #0x145c
0037256c ldr r0, [r8, r1]
00372570 movw r2, #0x1460
00372574 movw r3, #0x1464
00372578 str r0, [r5, r1]
0037257c ldr r1, [r8, r2]
00372580 str r1, [r5, r2]
00372584 ldr r2, [r8, r3]
00372588 str r2, [r5, r3]
0037258c ldr r0, [r8, #0x2f4]
00372590 cmp r0, #0
00372594 beq #0x3725a8
00372598 mov r1, r5
0037259c bl #0x396a90
003725a0 cmp r0, #0
003725a4 bne #0x372340
003725a8 ldr r2, [sp, #0xc]
003725ac mov r1, r5
003725b0 ldr r3, [r4, r2]
003725b4 ldr r0, [r3, #0x38]
003725b8 bl #0x344184
003725bc mov r3, #1
003725c0 strb r3, [r5, #0x2ef]
003725c4 mov r0, r5
003725c8 bl #0x38c710
003725cc b #0x372340
003725d0 mov r0, fp
003725d4 ldr r1, [sp, #8]
003725d8 mov r2, #1
003725dc bl #0x36dfb0
003725e0 ldr r3, [r0, #0x660]
003725e4 cmp r5, r3
003725e8 beq #0x3724b0
003725ec mov r0, fp
003725f0 mov r1, r5
003725f4 bl #0x371d80
003725f8 b #0x37226c
003725fc ldr r3, [pc, #0x98]
00372600 ldr r3, [r4, r3]
00372604 ldr r3, [r3]
00372608 cmp r3, #2
0037260c streq r5, [r5]
00372610 beq #0x3722e8
00372614 cmp r3, #1
00372618 bne #0x3722e8
0037261c ldr r0, [pc, #0x7c]
00372620 ldr r1, [pc, #0x7c]
00372624 ldr r2, [pc, #0x7c]
00372628 ldr r0, [r4, r0]
0037262c ldr r3, [pc, #0x78]
00372630 movw ip, #0x464
00372634 add r1, pc, r1
00372638 add r2, pc, r2
0037263c add r3, pc, r3
00372640 add r0, r0, #0xa8
00372644 str ip, [sp]
00372648 bl #0x30e004
0037264c b #0x3722e8
00372650 ldr r0, [pc, #0x48]
00372654 ldr r1, [pc, #0x54]
00372658 ldr r2, [pc, #0x54]
0037265c ldr r0, [r4, r0]
00372660 ldr r3, [pc, #0x50]
00372664 movw ip, #0x4a4
00372668 add r1, pc, r1
0037266c add r2, pc, r2
00372670 add r3, pc, r3
00372674 add r0, r0, #0xa8
00372678 str ip, [sp]
0037267c bl #0x30e004
00372680 b #0x3723f4
00372684 bl #0x30e310
00372688 rsbeq r2, r2, r0, ror #16
0037268c andeq r4, r0, ip, lsr #1
00372690 strdeq r3, r4, [r0], -r4
00372694 subseq pc, r4, r0, lsr r5
00372698 ldrsheq lr, [r4], #-0x14
0037269c andeq r3, r0, r0, asr #19
003726a0 andeq r1, r0, r0, asr #19
003726a4 subseq fp, r4, r4, lsr #27
003726a8 subseq pc, r7, r0, asr sl
003726ac ldrheq pc, [r4], #-4
003726b0 subseq fp, r4, r0, ror sp
003726b4 subseq pc, r4, r4, ror r1
003726b8 subseq pc, r4, r0, lsl #1
# _ZN10PlayerInfoC1Ev 37418c 2460
0037418c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00374190 ldr r5, [pc, #0x95c]
00374194 ldr r1, [pc, #0x95c]
00374198 sub sp, sp, #0x8c
0037419c add r5, pc, r5
003741a0 ldr r3, [r5, r1]
003741a4 mov r4, r0
003741a8 str r1, [sp, #0x28]
003741ac ldr r3, [r3]
003741b0 ldr r7, [pc, #0x944]
003741b4 mov r8, #0
003741b8 str r3, [sp, #0x84]
003741bc bl #0x80fc28
003741c0 ldr r3, [pc, #0x938]
003741c4 ldr r2, [r4, #0x2a8]
003741c8 ldr r0, [r5, r7]
003741cc ldr r3, [r5, r3]
003741d0 cmp r2, #0
003741d4 mov sb, #0
003741d8 mov r2, #0
003741dc add r3, r3, #8
003741e0 mov ip, #0x290
003741e4 strd r8, sb, [r4, ip]
003741e8 mvn r1, #0
003741ec str r3, [r4]
003741f0 str r2, [r4, #0x2a0]
003741f4 strb r2, [r4, #0x2a4]
003741f8 add r0, r0, #8
003741fc mov r3, #8
00374200 addeq r2, r4, #0x288
00374204 str r3, [r4, #0x28c]
00374208 str r1, [r4, #0x29c]
0037420c str r0, [r4, #0x288]
00374210 str r1, [r4, #0x298]
00374214 streq r2, [sp, #0x34]
00374218 beq #0x374230
0037421c add r3, r4, #0x288
00374220 str r3, [sp, #0x34]
00374224 str r2, [r4, #0x2a8]
00374228 ldr r0, [sp, #0x34]
0037422c bl #0x814f84
00374230 ldr r3, [pc, #0x8cc]
00374234 ldr r1, [pc, #0x8cc]
00374238 add r6, sp, #0x6c
0037423c ldr r3, [r5, r3]
00374240 add r0, r4, #0x2b0
00374244 add r2, sp, #0x68
00374248 add r3, r3, #8
0037424c str r3, [r4, #0x288]
00374250 add r1, pc, r1
00374254 str r0, [sp, #0x20]
00374258 mov r0, r6
0037425c bl #0x3140ec
00374260 mov r1, r6
00374264 ldr r0, [sp, #0x20]
00374268 bl #0x371bec
0037426c mov r0, r6
00374270 bl #0x318254
00374274 ldr r3, [r4, #0x308]
00374278 ldr r1, [r5, r7]
0037427c mov r0, #0x2f0
00374280 cmp r3, #0
00374284 add r1, r1, #8
00374288 mov r8, #0
0037428c mov sb, #0
00374290 strd r8, sb, [r4, r0]
00374294 mvn r2, #0
00374298 mov r3, #0
0037429c str r1, [r4, #0x2e8]
003742a0 mov r0, #0x10
003742a4 addeq r1, r4, #0x2e8
003742a8 str r0, [r4, #0x2ec]
003742ac str r2, [r4, #0x2fc]
003742b0 str r2, [r4, #0x2f8]
003742b4 str r3, [r4, #0x300]
003742b8 strb r3, [r4, #0x304]
003742bc streq r1, [sp, #0x48]
003742c0 beq #0x3742d8
003742c4 add r2, r4, #0x2e8
003742c8 str r2, [sp, #0x48]
003742cc str r3, [r4, #0x308]
003742d0 ldr r0, [sp, #0x48]
003742d4 bl #0x814f84
003742d8 ldr r6, [pc, #0x82c]
003742dc ldr r3, [r4, #0x330]
003742e0 ldr r1, [r5, r7]
003742e4 ldr r0, [r5, r6]
003742e8 cmp r3, #0
003742ec mov r8, #0
003742f0 mov r3, #0
003742f4 add r0, r0, #8
003742f8 mov sb, #0
003742fc mov ip, #0x318
00374300 strd r8, sb, [r4, ip]
00374304 mvn r2, #0
00374308 str r0, [r4, #0x2e8]
0037430c str r3, [r4, #0x328]
00374310 strb r3, [r4, #0x32c]
00374314 add r1, r1, #8
00374318 mov r0, #0x10
0037431c addeq r3, r4, #0x310
00374320 str r0, [r4, #0x314]
00374324 str r2, [r4, #0x324]
00374328 str r1, [r4, #0x310]
0037432c str r2, [r4, #0x320]
00374330 streq r3, [sp, #0x30]
00374334 beq #0x37434c
00374338 add r0, r4, #0x310
0037433c str r0, [sp, #0x30]
00374340 str r3, [r4, #0x330]
00374344 ldr r0, [sp, #0x30]
00374348 bl #0x814f84
0037434c ldr r3, [r4, #0x358]
00374350 ldr r0, [r5, r6]
00374354 ldr r1, [r5, r7]
00374358 cmp r3, #0
0037435c add r0, r0, #8
00374360 add r1, r1, #8
00374364 mov r8, #0
00374368 mov sb, #0
0037436c mov ip, #0x340
00374370 strd r8, sb, [r4, ip]
00374374 mvn r2, #0
00374378 mov r3, #0
0037437c str r0, [r4, #0x310]
00374380 str r1, [r4, #0x338]
00374384 mov r0, #0x10
00374388 addeq r1, r4, #0x338
0037438c str r0, [r4, #0x33c]
00374390 str r2, [r4, #0x34c]
00374394 str r2, [r4, #0x348]
00374398 str r3, [r4, #0x350]
0037439c strb r3, [r4, #0x354]
003743a0 streq r1, [sp, #0x38]
003743a4 beq #0x3743bc
003743a8 add r2, r4, #0x338
003743ac str r2, [sp, #0x38]
003743b0 str r3, [r4, #0x358]
003743b4 ldr r0, [sp, #0x38]
003743b8 bl #0x814f84
003743bc ldr r3, [r4, #0x380]
003743c0 ldr r0, [r5, r6]
003743c4 ldr r1, [r5, r7]
003743c8 cmp r3, #0
003743cc add r0, r0, #8
003743d0 mov r3, #0
003743d4 mov r8, #0
003743d8 mov sb, #0
003743dc mov ip, #0x368
003743e0 strd r8, sb, [r4, ip]
003743e4 mvn r2, #0
003743e8 str r0, [r4, #0x338]
003743ec str r3, [r4, #0x378]
003743f0 strb r3, [r4, #0x37c]
003743f4 add r1, r1, #8
003743f8 mov r0, #0x10
003743fc addeq r3, r4, #0x360
00374400 str r0, [r4, #0x364]
00374404 str r2, [r4, #0x374]
00374408 str r1, [r4, #0x360]
0037440c str r2, [r4, #0x370]
00374410 streq r3, [sp, #0x40]
00374414 beq #0x37442c
00374418 add r0, r4, #0x360
0037441c str r0, [sp, #0x40]
00374420 str r3, [r4, #0x380]
00374424 ldr r0, [sp, #0x40]
00374428 bl #0x814f84
0037442c ldr r3, [r4, #0x3a8]
00374430 ldr r0, [r5, r6]
00374434 ldr r1, [r5, r7]
00374438 cmn r3, #1
0037443c add r0, r0, #8
00374440 add r1, r1, #8
00374444 mov r8, #0
00374448 mov sb, #0
0037444c mov ip, #0x390
00374450 strd r8, sb, [r4, ip]
00374454 mvn r3, #0
00374458 mov r2, #0
0037445c str r0, [r4, #0x360]
00374460 str r1, [r4, #0x388]
00374464 mov r0, #0x10
00374468 addeq r1, r4, #0x388
0037446c str r0, [r4, #0x38c]
00374470 strb r2, [r4, #0x3a4]
00374474 str r3, [r4, #0x398]
00374478 str r3, [r4, #0x39c]
0037447c str r2, [r4, #0x3a0]
00374480 streq r1, [sp, #0x44]
00374484 beq #0x37449c
00374488 add r2, r4, #0x388
0037448c str r2, [sp, #0x44]
00374490 str r3, [r4, #0x3a8]
00374494 ldr r0, [sp, #0x44]
00374498 bl #0x814f84
0037449c ldr r3, [r5, r6]
003744a0 add r0, r4, #0x3b0
003744a4 str r0, [sp, #0x18]
003744a8 add r3, r3, #8
003744ac str r3, [r4, #0x388]
003744b0 mov r8, #0
003744b4 ldr r0, [sp, #0x18]
003744b8 add r1, sp, #0x60
003744bc str r8, [sp, #0x60]
003744c0 str r8, [sp, #0x64]
003744c4 bl #0x370550
003744c8 ldr r0, [sp, #0x60]
003744cc cmp r0, r8
003744d0 beq #0x3744dc
003744d4 bl #0x310440
003744d8 str r8, [sp, #0x60]
003744dc add r1, r4, #0x3d8
003744e0 mov r8, #0
003744e4 str r1, [sp, #0x1c]
003744e8 mov r0, r1
003744ec add r1, sp, #0x58
003744f0 str r8, [sp, #0x58]
003744f4 str r8, [sp, #0x5c]
003744f8 bl #0x370608
003744fc ldr r0, [sp, #0x58]
00374500 cmp r0, r8
00374504 beq #0x374510
00374508 bl #0x310440
0037450c str r8, [sp, #0x58]
00374510 add r2, r4, #0x400
00374514 mov r8, #0
00374518 mov r0, r2
0037451c add r1, sp, #0x50
00374520 str r2, [sp, #0x24]
00374524 str r8, [sp, #0x50]
00374528 str r8, [sp, #0x54]
0037452c bl #0x3706c0
00374530 ldr r0, [sp, #0x50]
00374534 cmp r0, r8
00374538 beq #0x374544
0037453c bl #0x310440
00374540 str r8, [sp, #0x50]
00374544 ldr r3, [r4, #0x448]
00374548 ldr r1, [r5, r7]
0037454c mov r0, #0x430
00374550 cmp r3, #0
00374554 mov r8, #0
00374558 mov r3, #0
0037455c mov sb, #0
00374560 strd r8, sb, [r4, r0]
00374564 str r3, [r4, #0x440]
00374568 strb r3, [r4, #0x444]
0037456c addeq r3, r4, #0x420
00374570 mvn r2, #0
00374574 add r1, r1, #8
00374578 mov r0, #0x20
0037457c addeq r3, r3, #8
00374580 str r0, [r4, #0x42c]
00374584 str r2, [r4, #0x43c]
00374588 str r1, [r4, #0x428]
0037458c str r2, [r4, #0x438]
00374590 streq r3, [sp, #0x14]
00374594 beq #0x3745b0
00374598 add r0, r4, #0x420
0037459c add r0, r0, #8
003745a0 str r0, [sp, #0x14]
003745a4 str r3, [r4, #0x448]
003745a8 ldr r0, [sp, #0x14]
003745ac bl #0x814f84
003745b0 ldr r8, [pc, #0x558]
003745b4 ldr r3, [r4, #0x470]
003745b8 ldr r1, [r5, r7]
003745bc ldr r0, [r5, r8]
003745c0 cmp r3, #0
003745c4 add r1, r1, #8
003745c8 add r0, r0, #8
003745cc mov sl, #0
003745d0 mov fp, #0
003745d4 movw ip, #0x458
003745d8 strd sl, fp, [r4, ip]
003745dc mvn r2, #0
003745e0 mov r3, #0
003745e4 str r0, [r4, #0x428]
003745e8 str r1, [r4, #0x450]
003745ec mov r0, #0x20
003745f0 addeq r1, r4, #0x450
003745f4 str r0, [r4, #0x454]
003745f8 str r2, [r4, #0x464]
003745fc str r2, [r4, #0x460]
00374600 str r3, [r4, #0x468]
00374604 strb r3, [r4, #0x46c]
00374608 streq r1, [sp, #0x2c]
0037460c beq #0x374624
00374610 add r2, r4, #0x450
00374614 str r2, [sp, #0x2c]
00374618 str r3, [r4, #0x470]
0037461c ldr r0, [sp, #0x2c]
00374620 bl #0x814f84
00374624 ldr r3, [r4, #0x498]
00374628 ldr r0, [r5, r8]
0037462c ldr r1, [r5, r7]
00374630 cmp r3, #0
00374634 mov sl, #0
00374638 mov r3, #0
0037463c mov fp, #0
00374640 mov ip, #0x480
00374644 strd sl, fp, [r4, ip]
00374648 add r0, r0, #8
0037464c str r3, [r4, #0x490]
00374650 strb r3, [r4, #0x494]
00374654 addeq r3, r4, #0x470
00374658 mvn r2, #0
0037465c str r0, [r4, #0x450]
00374660 add r1, r1, #8
00374664 mov r0, #0x20
00374668 addeq r3, r3, #8
0037466c str r0, [r4, #0x47c]
00374670 str r2, [r4, #0x48c]
00374674 str r1, [r4, #0x478]
00374678 str r2, [r4, #0x488]
0037467c streq r3, [sp, #8]
00374680 beq #0x37469c
00374684 add r0, r4, #0x470
00374688 add r0, r0, #8
0037468c str r0, [sp, #8]
00374690 str r3, [r4, #0x498]
00374694 ldr r0, [sp, #8]
00374698 bl #0x814f84
0037469c ldr r3, [r4, #0x4c0]
003746a0 ldr r0, [r5, r8]
003746a4 ldr r1, [r5, r7]
003746a8 cmp r3, #0
003746ac add r0, r0, #8
003746b0 add r1, r1, #8
003746b4 mov sl, #0
003746b8 mov fp, #0
003746bc movw ip, #0x4a8
003746c0 strd sl, fp, [r4, ip]
003746c4 mvn r2, #0
003746c8 mov r3, #0
003746cc str r0, [r4, #0x478]
003746d0 str r1, [r4, #0x4a0]
003746d4 mov r0, #0x20
003746d8 addeq r1, r4, #0x4a0
003746dc str r0, [r4, #0x4a4]
003746e0 str r2, [r4, #0x4b4]
003746e4 str r2, [r4, #0x4b0]
003746e8 str r3, [r4, #0x4b8]
003746ec strb r3, [r4, #0x4bc]
003746f0 streq r1, [sp, #0x3c]
003746f4 beq #0x37470c
003746f8 add r2, r4, #0x4a0
003746fc str r2, [sp, #0x3c]
00374700 str r3, [r4, #0x4c0]
00374704 ldr r0, [sp, #0x3c]
00374708 bl #0x814f84
0037470c ldr sl, [pc, #0x400]
00374710 ldrb r3, [r4, #0x4e5]
00374714 ldr r0, [r5, r8]
00374718 ldr r1, [r5, sl]
0037471c cmp r3, #0
00374720 mov r8, #0
00374724 mov r3, #0
00374728 mov sb, #0
0037472c mov ip, #0x4d0
00374730 strd r8, sb, [r4, ip]
00374734 add r0, r0, #8
00374738 str r3, [r4, #0x4e0]
0037473c strb r3, [r4, #0x4e4]
00374740 addeq r3, r4, #0x4c0
00374744 mvn r2, #0
00374748 str r0, [r4, #0x4a0]
0037474c add r1, r1, #8
00374750 mov r0, #1
00374754 addeq r3, r3, #8
00374758 str r0, [r4, #0x4cc]
0037475c str r2, [r4, #0x4dc]
00374760 str r1, [r4, #0x4c8]
00374764 str r2, [r4, #0x4d8]
00374768 streq r3, [sp, #0x10]
0037476c beq #0x374788
00374770 add r0, r4, #0x4c0
00374774 add r0, r0, #8
00374778 str r0, [sp, #0x10]
0037477c strb r3, [r4, #0x4e5]
00374780 ldr r0, [sp, #0x10]
00374784 bl #0x814f84
00374788 ldr r8, [pc, #0x388]
0037478c ldrb r3, [r4, #0x505]
00374790 ldr r1, [r5, sl]
00374794 ldr r0, [r5, r8]
00374798 cmp r3, #0
0037479c add sb, r1, #8
003747a0 add lr, r0, #8
003747a4 mov r1, #0
003747a8 mov r0, #0
003747ac mov ip, #0x4f0
003747b0 strd r0, r1, [r4, ip]
003747b4 addeq r1, r4, #0x4e0
003747b8 mvn r2, #0
003747bc mov r3, #0
003747c0 mov r0, #1
003747c4 addeq r1, r1, #8
003747c8 str lr, [r4, #0x4c8]
003747cc str r0, [r4, #0x4ec]
003747d0 str r2, [r4, #0x4fc]
003747d4 str sb, [r4, #0x4e8]
003747d8 str r2, [r4, #0x4f8]
003747dc str r3, [r4, #0x500]
003747e0 strb r3, [r4, #0x504]
003747e4 streq r1, [sp]
003747e8 beq #0x374804
003747ec add r2, r4, #0x4e0
003747f0 add r2, r2, #8
003747f4 str r2, [sp]
003747f8 strb r3, [r4, #0x505]
003747fc ldr r0, [sp]
00374800 bl #0x814f84
00374804 ldrb r3, [r4, #0x525]
00374808 ldr r0, [r5, r8]
0037480c ldr r1, [r5, sl]
00374810 cmp r3, #0
00374814 add lr, r0, #8
00374818 add sb, r1, #8
0037481c mov r0, #0
00374820 mov r1, #0
00374824 mov ip, #0x510
00374828 strd r0, r1, [r4, ip]
0037482c addeq r1, r4, #0x500
00374830 mvn r2, #0
00374834 mov r3, #0
00374838 mov r0, #1
0037483c addeq r1, r1, #8
00374840 str lr, [r4, #0x4e8]
00374844 str r0, [r4, #0x50c]
00374848 str r2, [r4, #0x51c]
0037484c str sb, [r4, #0x508]
00374850 str r2, [r4, #0x518]
00374854 str r3, [r4, #0x520]
00374858 strb r3, [r4, #0x524]
0037485c streq r1, [sp, #4]
00374860 beq #0x37487c
00374864 add r2, r4, #0x500
00374868 add r2, r2, #8
0037486c str r2, [sp, #4]
00374870 strb r3, [r4, #0x525]
00374874 ldr r0, [sp, #4]
00374878 bl #0x814f84
0037487c ldrb r3, [r4, #0x545]
00374880 ldr r0, [r5, r8]
00374884 ldr r1, [r5, sl]
00374888 cmp r3, #0
0037488c mov sl, #0
00374890 mov r3, #0
00374894 mov fp, #0
00374898 mov ip, #0x530
0037489c strd sl, fp, [r4, ip]
003748a0 add r0, r0, #8
003748a4 str r3, [r4, #0x540]
003748a8 strb r3, [r4, #0x544]
003748ac addeq r3, r4, #0x520
003748b0 mvn r2, #0
003748b4 str r0, [r4, #0x508]
003748b8 add r1, r1, #8
003748bc mov r0, #1
003748c0 addeq r3, r3, #8
003748c4 str r0, [r4, #0x52c]
003748c8 str r2, [r4, #0x53c]
003748cc str r1, [r4, #0x528]
003748d0 str r2, [r4, #0x538]
003748d4 streq r3, [sp, #0xc]
003748d8 beq #0x3748f4
003748dc add r0, r4, #0x520
003748e0 add r0, r0, #8
003748e4 str r0, [sp, #0xc]
003748e8 strb r3, [r4, #0x545]
003748ec ldr r0, [sp, #0xc]
003748f0 bl #0x814f84
003748f4 ldr r3, [r5, r8]
003748f8 ldr r2, [r5, r7]
003748fc add r7, r4, #0x540
00374900 add r3, r3, #8
00374904 add r2, r2, #8
00374908 str r3, [r4, #0x528]
0037490c add fp, r4, #0x660
00374910 str r4, [sp, #0x4c]
00374914 add r7, r7, #8
00374918 mov sb, #0x10
0037491c mvn sl, #0
00374920 mov r8, #0
00374924 mov r4, r2
00374928 ldr r3, [r7, #0x20]
0037492c mov r0, #0
00374930 mov r1, #0
00374934 cmp r3, #0
00374938 str sb, [r7, #4]
0037493c strd r0, r1, [r7, #8]
00374940 str sl, [r7, #0x10]
00374944 str sl, [r7, #0x14]
00374948 str r8, [r7, #0x18]
0037494c strb r8, [r7, #0x1c]
00374950 str r4, [r7]
00374954 beq #0x374964
00374958 str r8, [r7, #0x20]
0037495c mov r0, r7
00374960 bl #0x814f84
00374964 ldr r3, [r5, r6]
00374968 add r3, r3, #8
0037496c str r3, [r7], #0x28
00374970 cmp r7, fp
00374974 bne #0x374928
00374978 ldr r6, [pc, #0x19c]
0037497c ldr r4, [sp, #0x4c]
00374980 add r6, pc, r6
00374984 ldr r3, [r6]
00374988 tst r3, #1
0037498c beq #0x374ac0
00374990 ldr r1, [sp, #0x34]
00374994 mov r0, r4
00374998 bl #0x81324c
0037499c mov r0, r4
003749a0 ldr r1, [sp, #0x30]
003749a4 bl #0x81324c
003749a8 mov r0, r4
003749ac ldr r1, [sp, #0x38]
003749b0 bl #0x81324c
003749b4 mov r0, r4
003749b8 ldr r1, [sp, #0x40]
003749bc bl #0x81324c
003749c0 mov r0, r4
003749c4 ldr r1, [sp, #0x20]
003749c8 bl #0x81324c
003749cc mov r0, r4
003749d0 ldr r1, [sp, #0x44]
003749d4 bl #0x81324c
003749d8 mov r0, r4
003749dc ldr r1, [sp, #0x48]
003749e0 bl #0x81324c
003749e4 mov r0, r4
003749e8 ldr r1, [sp, #0x18]
003749ec bl #0x81324c
003749f0 mov r0, r4
003749f4 ldr r1, [sp, #0x1c]
003749f8 bl #0x81324c
003749fc mov r0, r4
00374a00 ldr r1, [sp, #0x24]
00374a04 bl #0x81324c
00374a08 mov r0, r4
00374a0c ldr r1, [sp, #0x14]
00374a10 bl #0x81324c
00374a14 mov r0, r4
00374a18 ldr r1, [sp, #0x2c]
00374a1c bl #0x81324c
00374a20 mov r0, r4
00374a24 ldr r1, [sp, #8]
00374a28 bl #0x81324c
00374a2c mov r0, r4
00374a30 ldr r1, [sp, #0x3c]
00374a34 bl #0x81324c
00374a38 mov r0, r4
00374a3c ldr r1, [sp, #0x10]
00374a40 bl #0x81324c
00374a44 mov r0, r4
00374a48 ldr r1, [sp]
00374a4c bl #0x81324c
00374a50 mov r0, r4
00374a54 ldr r1, [sp, #4]
00374a58 bl #0x81324c
00374a5c mov r0, r4
00374a60 ldr r1, [sp, #0xc]
00374a64 bl #0x81324c
00374a68 mov r6, #0
00374a6c mov r7, #0x28
00374a70 mul r1, r7, r6
00374a74 mov r0, r4
00374a78 add r1, r1, #0x540
00374a7c add r1, r1, #8
00374a80 add r6, r6, #1
00374a84 add r1, r4, r1
00374a88 bl #0x81324c
00374a8c cmp r6, #7
00374a90 bne #0x374a70
00374a94 mov r0, r4
00374a98 bl #0x373bdc
00374a9c ldr r1, [sp, #0x28]
00374aa0 ldr r2, [sp, #0x84]
00374aa4 mov r0, r4
00374aa8 ldr r3, [r5, r1]
00374aac ldr r3, [r3]
00374ab0 cmp r2, r3
00374ab4 bne #0x374af0
00374ab8 add sp, sp, #0x8c
00374abc pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00374ac0 mov r0, r6
00374ac4 bl #0x30e76c
00374ac8 cmp r0, #0
00374acc beq #0x374990
00374ad0 ldr r3, [pc, #0x48]
00374ad4 ldr r0, [r5, r3]
00374ad8 ldr r3, [pc, #0x44]
00374adc ldr r1, [r5, r3]
00374ae0 bl #0x8102c0
00374ae4 mov r0, r6
00374ae8 bl #0x30ea3c
00374aec b #0x374990
00374af0 bl #0x30e310
# _ZN17PlayerStatManager12IncreaseStatE9EStatTypeii 3790e0 4
003790e0 bx lr
# _ZN9LuaScript14_GetNumPlayersERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 37cbd8 40
0037cbd8 ldr r3, [pc, #0x18]
0037cbdc ldr r2, [pc, #0x18]
0037cbe0 mov r0, r1
0037cbe4 add r3, pc, r3
0037cbe8 ldr r2, [r3, r2]
0037cbec ldr r3, [r2, #0x40]
0037cbf0 ldr r1, [r3, #0x6c4]
0037cbf4 b #0x37cb24
0037cbf8 rsbeq r7, r1, ip, lsr #29
0037cbfc strdeq r3, r4, [r0], -r4
# _ZN9Character28F_ApplyScrollingCombatTextXPEPS_iii 3af0c8 308
003af0c8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003af0cc ldr r5, [pc, #0x114]
003af0d0 mov r7, r0
003af0d4 ldr r0, [pc, #0x110]
003af0d8 add r5, pc, r5
003af0dc mov ip, r3
003af0e0 ldr r6, [r5, r0]
003af0e4 sub sp, sp, #0x3c
003af0e8 mov sl, r2
003af0ec ldr r3, [r6]
003af0f0 mov r8, r1
003af0f4 str ip, [sp, #0xc]
003af0f8 str r3, [sp, #0x34]
003af0fc bl #0x413e90
003af100 mov sb, r0
003af104 mov r0, r7
003af108 bl #0x3935dc
003af10c ldr r2, [r0]
003af110 ldr r3, [r0, #4]
003af114 ldr r1, [r7, #0x14c]
003af118 ldr fp, [r0, #8]
003af11c ldr r0, [r7, #0x158]
003af120 str r2, [sp, #0x10]
003af124 str r3, [sp, #0x14]
003af128 bl #0x30e3ac
003af12c mov r1, r0
003af130 mov r0, fp
003af134 bl #0x30eba4
003af138 ldr r3, [pc, #0xb0]
003af13c ldr r2, [pc, #0xb0]
003af140 ldr r1, [pc, #0xb0]
003af144 ldr r5, [r5, r3]
003af148 add r2, pc, r2
003af14c str r0, [sp, #0x18]
003af150 add r1, pc, r1
003af154 ldr r0, [r5, #0x2c]
003af158 ldr r7, [r5, #0x34]
003af15c bl #0x4c4bdc
003af160 mov r1, r0
003af164 mov r0, r7
003af168 bl #0x508edc
003af16c add r4, sp, #0x1c
003af170 mov r7, r0
003af174 mov r1, #0x10
003af178 mov r0, r4
003af17c str r4, [sp, #0x2c]
003af180 str r4, [sp, #0x30]
003af184 bl #0x31167c
003af188 ldr r3, [sp, #0x2c]
003af18c mov r1, #0
003af190 mov r2, r7
003af194 strb r1, [r3]
003af198 ldr r0, [r5, #0x34]
003af19c mov r3, r8
003af1a0 mov r1, r4
003af1a4 bl #0x508ef4
003af1a8 ldr ip, [sp, #0xc]
003af1ac ldr r3, [sp, #0x30]
003af1b0 add r2, sp, #0x10
003af1b4 mov r1, sl
003af1b8 mov r0, sb
003af1bc str ip, [sp]
003af1c0 bl #0x413dc4
003af1c4 mov r0, r4
003af1c8 bl #0x3139ac
003af1cc ldr r2, [sp, #0x34]
003af1d0 ldr r3, [r6]
003af1d4 cmp r2, r3
003af1d8 bne #0x3af1e4
003af1dc add sp, sp, #0x3c
003af1e0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003af1e4 bl #0x30e310
003af1e8 ldrheq r5, [lr], #-0x98
003af1ec andeq r4, r0, ip, lsr #1
003af1f0 strdeq r3, r4, [r0], -r4
003af1f4 subseq r4, r1, r0, lsr #17
003af1f8 ldrsbeq pc, [r0], #-0xa8
# _ZN9Character7InitAllEv 3b35f0 40
003b35f0 push {r4, lr}
003b35f4 mov r4, r0
003b35f8 ldr r3, [r0]
003b35fc mov lr, pc
003b3600 ldr pc, [r3, #0x1c]
003b3604 mov r0, r4
003b3608 ldr r3, [r4]
003b360c mov lr, pc
003b3610 ldr pc, [r3, #0x58]
003b3614 pop {r4, pc}
# _ZN9Character24InitializePlayerSavegameEv 3b36b0 52
003b36b0 push {r4, r5, r6, lr}
003b36b4 mov r1, #0
003b36b8 mov r4, r0
003b36bc mov r0, #0x198
003b36c0 bl #0x310570
003b36c4 mov r5, r0
003b36c8 bl #0x465ae0
003b36cc movw r3, #0x14e8
003b36d0 mov r0, r4
003b36d4 mov r1, r4
003b36d8 str r5, [r4, r3]
003b36dc pop {r4, r5, r6, lr}
003b36e0 b #0x3bb754
# _ZN9Character7SG_SaveEv 3bc4a8 20
003bc4a8 movw r3, #0x14e8
003bc4ac ldr r0, [r0, r3]
003bc4b0 cmp r0, #0
003bc4b4 bxeq lr
003bc4b8 b #0x464b2c
# _ZN9Character7LevelUpEi 3beb88 2320
003beb88 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003beb8c ldr r4, [pc, #0x878]
003beb90 ldr r7, [pc, #0x878]
003beb94 ldr sb, [pc, #0x878]
003beb98 add r4, pc, r4
003beb9c ldr r3, [r4, r7]
003beba0 ldr r6, [pc, #0x870]
003beba4 ldr sl, [r4, sb]
003beba8 ldr r2, [pc, #0x86c]
003bebac ldr r3, [r3]
003bebb0 sub sp, sp, #0x94
003bebb4 add r6, pc, r6
003bebb8 mov r5, r0
003bebbc add r2, pc, r2
003bebc0 str r1, [sp, #0x14]
003bebc4 ldr r0, [sl, #0x2c]
003bebc8 mov r1, r6
003bebcc str r3, [sp, #0x8c]
003bebd0 bl #0x4c4bdc
003bebd4 mov r8, r0
003bebd8 mov r0, r5
003bebdc bl #0x3bb918
003bebe0 cmp r0, #1
003bebe4 beq #0x3bf030
003bebe8 mov r0, r5
003bebec bl #0x3bb918
003bebf0 cmp r0, #2
003bebf4 beq #0x3bf04c
003bebf8 add r6, r5, #0x560
003bebfc mov r0, r6
003bec00 mov r1, #0x13
003bec04 mov r2, #0
003bec08 bl #0x3df6e0
003bec0c cmp r8, r0
003bec10 bgt #0x3bec30
003bec14 ldr r3, [r4, r7]
003bec18 ldr r2, [sp, #0x8c]
003bec1c ldr r3, [r3]
003bec20 cmp r2, r3
003bec24 bne #0x3bf408
003bec28 add sp, sp, #0x94
003bec2c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bec30 ldr r3, [pc, #0x7e8]
003bec34 add sl, sp, #0x74
003bec38 ldr fp, [r4, r3]
003bec3c mov r0, fp
003bec40 bl #0x337888
003bec44 ldr r1, [pc, #0x7d8]
003bec48 add r2, sp, #0x54
003bec4c mov r0, sl
003bec50 add r1, pc, r1
003bec54 bl #0x3140ec
003bec58 mov r1, sl
003bec5c mov r0, fp
003bec60 bl #0x337a88
003bec64 mov r0, sl
003bec68 bl #0x318254
003bec6c mov r0, r6
003bec70 mov r1, #0x13
003bec74 mov r2, #1
003bec78 bl #0x3e0798
003bec7c mov r2, #0
003bec80 mov r0, r6
003bec84 mov r1, #0x21
003bec88 bl #0x3e0808
003bec8c movw r3, #0x13c8
003bec90 ldrsh r1, [r5, r3]
003bec94 mov r0, r6
003bec98 bl #0x3e087c
003bec9c mov r0, r5
003beca0 mvn r1, #0
003beca4 bl #0x3bdca4
003beca8 mov r0, r5
003becac mvn r1, #0
003becb0 bl #0x3bdbb8
003becb4 mov r2, #0
003becb8 mov r1, #0x13
003becbc mov r0, r6
003becc0 bl #0x3df6e0
003becc4 mov r1, r0
003becc8 mov r0, r5
003beccc bl #0x3bb840
003becd0 mov r0, r5
003becd4 bl #0x3bc4a8
003becd8 ldr r3, [r5]
003becdc mov r0, r5
003bece0 mov lr, pc
003bece4 ldr pc, [r3, #0x28]
003bece8 cmp r0, #0
003becec beq #0x3befe4
003becf0 add sl, sp, #0x58
003becf4 mov r0, sl
003becf8 mov r1, #0x10
003becfc str sl, [sp, #0x68]
003bed00 str sl, [sp, #0x6c]
003bed04 bl #0x31167c
003bed08 ldr r2, [sp, #0x68]
003bed0c mov r1, #0
003bed10 ldr r3, [r4, sb]
003bed14 strb r1, [r2]
003bed18 ldr r2, [pc, #0x708]
003bed1c ldr r1, [pc, #0x708]
003bed20 ldr r0, [r3, #0x2c]
003bed24 add r2, pc, r2
003bed28 add r1, pc, r1
003bed2c ldr fp, [r3, #0x34]
003bed30 bl #0x4c4bdc
003bed34 mov r1, r0
003bed38 mov r0, fp
003bed3c bl #0x508edc
003bed40 mov fp, r0
003bed44 bl #0x30de54
003bed48 ldr r1, [pc, #0x6e0]
003bed4c add r2, fp, r0
003bed50 mov r0, sl
003bed54 str r1, [sp, #0x10]
003bed58 mov r1, fp
003bed5c bl #0x3109e0
003bed60 ldr r2, [sp, #0x10]
003bed64 mov r1, sl
003bed68 ldr r2, [r4, r2]
003bed6c add fp, r2, #4
003bed70 mov r0, fp
003bed74 str r2, [sp, #0x10]
003bed78 bl #0x3be28c
003bed7c ldm fp, {r0, r1, r2, r3}
003bed80 add ip, sp, #0x20
003bed84 stm ip, {r0, r1, r2, r3}
003bed88 ldr r3, [sp, #0x10]
003bed8c mov r1, ip
003bed90 add r0, r3, #0x14
003bed94 bl #0x3bcf8c
003bed98 cmp r0, #1
003bed9c beq #0x3bf22c
003beda0 ldr r1, [pc, #0x68c]
003beda4 ldr r3, [r4, sb]
003beda8 mov r2, #1
003bedac ldr fp, [r4, r1]
003bedb0 str r1, [sp, #0x10]
003bedb4 ldr r1, [pc, #0x67c]
003bedb8 ldr r3, [r3, #0x4c]
003bedbc mov r0, fp
003bedc0 add r1, pc, r1
003bedc4 str r3, [sp, #0x18]
003bedc8 bl #0x4591f0
003bedcc cmn r0, #1
003bedd0 mov r1, r0
003bedd4 beq #0x3bede8
003bedd8 mov r0, fp
003beddc mvn r2, #0
003bede0 mov r3, #0
003bede4 bl #0x4605c0
003bede8 ldr r3, [pc, #0x64c]
003bedec mov r1, #0x87
003bedf0 mov r2, r5
003bedf4 ldr r0, [r4, r3]
003bedf8 mov r3, #0
003bedfc bl #0x495f04
003bee00 mov r0, r6
003bee04 mov r1, #0x13
003bee08 mov r2, #0
003bee0c bl #0x3df6e0
003bee10 cmp r0, #2
003bee14 mov fp, r0
003bee18 beq #0x3bf138
003bee1c cmp r0, #0xc
003bee20 beq #0x3bf1e0
003bee24 ldr r3, [r4, sb]
003bee28 mov r1, r5
003bee2c ldr r0, [r3, #0x40]
003bee30 bl #0x36effc
003bee34 cmp r0, #0
003bee38 beq #0x3befdc
003bee3c sub fp, fp, #0xa
003bee40 cmp fp, #0x5a
003bee44 addls pc, pc, fp, lsl #2
003bee48 b #0x3befdc
003bee4c b #0x3bf108
003bee50 b #0x3befdc
003bee54 b #0x3befdc
003bee58 b #0x3befdc
003bee5c b #0x3befdc
003bee60 b #0x3befdc
003bee64 b #0x3befdc
003bee68 b #0x3befdc
003bee6c b #0x3befdc
003bee70 b #0x3befdc
003bee74 b #0x3bf0f4
003bee78 b #0x3befdc
003bee7c b #0x3befdc
003bee80 b #0x3befdc
003bee84 b #0x3befdc
003bee88 b #0x3befdc
003bee8c b #0x3befdc
003bee90 b #0x3befdc
003bee94 b #0x3befdc
003bee98 b #0x3befdc
003bee9c b #0x3bf0e0
003beea0 b #0x3befdc
003beea4 b #0x3befdc
003beea8 b #0x3befdc
003beeac b #0x3befdc
003beeb0 b #0x3befdc
003beeb4 b #0x3befdc
003beeb8 b #0x3befdc
003beebc b #0x3befdc
003beec0 b #0x3befdc
003beec4 b #0x3bf0cc
003beec8 b #0x3befdc
003beecc b #0x3befdc
003beed0 b #0x3befdc
003beed4 b #0x3befdc
003beed8 b #0x3befdc
003beedc b #0x3befdc
003beee0 b #0x3befdc
003beee4 b #0x3befdc
003beee8 b #0x3befdc
003beeec b #0x3bf0b8
003beef0 b #0x3befdc
003beef4 b #0x3befdc
003beef8 b #0x3befdc
003beefc b #0x3befdc
003bef00 b #0x3befdc
003bef04 b #0x3befdc
003bef08 b #0x3befdc
003bef0c b #0x3befdc
003bef10 b #0x3befdc
003bef14 b #0x3bf0a4
003bef18 b #0x3befdc
003bef1c b #0x3befdc
003bef20 b #0x3befdc
003bef24 b #0x3befdc
003bef28 b #0x3befdc
003bef2c b #0x3befdc
003bef30 b #0x3befdc
003bef34 b #0x3befdc
003bef38 b #0x3befdc
003bef3c b #0x3bf090
003bef40 b #0x3befdc
003bef44 b #0x3befdc
003bef48 b #0x3befdc
003bef4c b #0x3befdc
003bef50 b #0x3befdc
003bef54 b #0x3befdc
003bef58 b #0x3befdc
003bef5c b #0x3befdc
003bef60 b #0x3befdc
003bef64 b #0x3bf07c
003bef68 b #0x3befdc
003bef6c b #0x3befdc
003bef70 b #0x3befdc
003bef74 b #0x3befdc
003bef78 b #0x3befdc
003bef7c b #0x3befdc
003bef80 b #0x3befdc
003bef84 b #0x3befdc
003bef88 b #0x3befdc
003bef8c b #0x3bf068
003bef90 b #0x3befdc
003bef94 b #0x3befdc
003bef98 b #0x3befdc
003bef9c b #0x3befdc
003befa0 b #0x3befdc
003befa4 b #0x3befdc
003befa8 b #0x3befdc
003befac b #0x3befdc
003befb0 b #0x3befdc
003befb4 b #0x3befb8
003befb8 ldr r3, [pc, #0x480]
003befbc ldr r0, [pc, #0x480]
003befc0 ldr r3, [r4, r3]
003befc4 add r0, pc, r0
003befc8 ldr sb, [r3]
003befcc bl #0x3a3f70
003befd0 mov r1, r0
003befd4 mov r0, sb
003befd8 bl #0x3813b8
003befdc mov r0, sl
003befe0 bl #0x318254
003befe4 mov r0, r6
003befe8 mov r1, #0x13
003befec mov r2, #0
003beff0 bl #0x3df6e0
003beff4 cmp r8, r0
003beff8 ble #0x3bec14
003beffc add r1, r5, #0xff0
003bf000 add r1, r1, #4
003bf004 mov r0, r6
003bf008 mov r2, #0x22
003bf00c bl #0x3dedb4
003bf010 ldr r1, [sp, #0x14]
003bf014 cmp r1, r0, asr #8
003bf018 bgt #0x3bf11c
003bf01c mov r0, r6
003bf020 ldr r2, [sp, #0x14]
003bf024 mov r1, #0x21
003bf028 bl #0x3e0798
003bf02c b #0x3bec14
003bf030 ldr r2, [pc, #0x410]
003bf034 ldr r0, [sl, #0x2c]
003bf038 mov r1, r6
003bf03c add r2, pc, r2
003bf040 bl #0x4c4bdc
003bf044 mov r8, r0
003bf048 b #0x3bebf8
003bf04c ldr r2, [pc, #0x3f8]
003bf050 ldr r0, [sl, #0x2c]
003bf054 mov r1, r6
003bf058 add r2, pc, r2
003bf05c bl #0x4c4bdc
003bf060 mov r8, r0
003bf064 b #0x3bebf8
003bf068 ldr r3, [pc, #0x3d0]
003bf06c ldr r0, [pc, #0x3dc]
003bf070 ldr r3, [r4, r3]
003bf074 add r0, pc, r0
003bf078 b #0x3befc8
003bf07c ldr r3, [pc, #0x3bc]
003bf080 ldr r0, [pc, #0x3cc]
003bf084 ldr r3, [r4, r3]
003bf088 add r0, pc, r0
003bf08c b #0x3befc8
003bf090 ldr r3, [pc, #0x3a8]
003bf094 ldr r0, [pc, #0x3bc]
003bf098 ldr r3, [r4, r3]
003bf09c add r0, pc, r0
003bf0a0 b #0x3befc8
003bf0a4 ldr r3, [pc, #0x394]
003bf0a8 ldr r0, [pc, #0x3ac]
003bf0ac ldr r3, [r4, r3]
003bf0b0 add r0, pc, r0
003bf0b4 b #0x3befc8
003bf0b8 ldr r3, [pc, #0x380]
003bf0bc ldr r0, [pc, #0x39c]
003bf0c0 ldr r3, [r4, r3]
003bf0c4 add r0, pc, r0
003bf0c8 b #0x3befc8
003bf0cc ldr r3, [pc, #0x36c]
003bf0d0 ldr r0, [pc, #0x38c]
003bf0d4 ldr r3, [r4, r3]
003bf0d8 add r0, pc, r0
003bf0dc b #0x3befc8
003bf0e0 ldr r3, [pc, #0x358]
003bf0e4 ldr r0, [pc, #0x37c]
003bf0e8 ldr r3, [r4, r3]
003bf0ec add r0, pc, r0
003bf0f0 b #0x3befc8
003bf0f4 ldr r3, [pc, #0x344]
003bf0f8 ldr r0, [pc, #0x36c]
003bf0fc ldr r3, [r4, r3]
003bf100 add r0, pc, r0
003bf104 b #0x3befc8
003bf108 ldr r3, [pc, #0x330]
003bf10c ldr r0, [pc, #0x35c]
003bf110 ldr r3, [r4, r3]
003bf114 add r0, pc, r0
003bf118 b #0x3befc8
003bf11c mov r0, r6
003bf120 mov r1, #0x22
003bf124 bl #0x3bd130
003bf128 asr r0, r0, #8
003bf12c sub r0, r0, #1
003bf130 str r0, [sp, #0x14]
003bf134 b #0x3bf01c
003bf138 bl #0x7fd794
003bf13c ldrb r3, [r0, #5]
003bf140 cmp r3, #0
003bf144 beq #0x3bf380
003bf148 bl #0x7fd794
003bf14c ldrb r3, [r0, #5]
003bf150 cmp r3, #0
003bf154 beq #0x3bf318
003bf158 bl #0x7fd794
003bf15c ldrb r3, [r0, #5]
003bf160 cmp r3, #0
003bf164 bne #0x3bee24
003bf168 mov r0, r5
003bf16c bl #0x3bb8e4
003bf170 cmp r0, #0
003bf174 str r0, [sp, #0x1c]
003bf178 bne #0x3bee24
003bf17c ldr r1, [sp, #0x18]
003bf180 ldrb r3, [r1, #0x2c]
003bf184 cmp r3, #0
003bf188 beq #0x3bee24
003bf18c ldr r2, [sp, #0x10]
003bf190 ldr r1, [pc, #0x2dc]
003bf194 ldr r2, [r4, r2]
003bf198 add r1, pc, r1
003bf19c str r2, [sp, #0x18]
003bf1a0 ldr r0, [sp, #0x18]
003bf1a4 mov r2, #1
003bf1a8 bl #0x4591f0
003bf1ac cmn r0, #1
003bf1b0 str r0, [sp, #0x10]
003bf1b4 beq #0x3bee24
003bf1b8 bl #0x7fd794
003bf1bc ldrb r3, [r0, #5]
003bf1c0 cmp r3, #0
003bf1c4 beq #0x3bee24
003bf1c8 ldr r0, [sp, #0x18]
003bf1cc ldr r1, [sp, #0x10]
003bf1d0 ldr r3, [sp, #0x1c]
003bf1d4 mvn r2, #0
003bf1d8 bl #0x4605c0
003bf1dc b #0x3bee24
003bf1e0 bl #0x42ca8c
003bf1e4 ldr r1, [pc, #0x28c]
003bf1e8 ldr r3, [r0, #0xf4]
003bf1ec ldr r2, [pc, #0x288]
003bf1f0 mov ip, #1
003bf1f4 ldr r0, [r3, #0x138]
003bf1f8 mov lr, #0
003bf1fc add r3, sp, #0x3c
003bf200 add r1, pc, r1
003bf204 add r2, pc, r2
003bf208 str r3, [sp, #0x10]
003bf20c strb lr, [sp, #0x3c]
003bf210 str ip, [sp]
003bf214 strb ip, [sp, #0x3d]
003bf218 strb ip, [sp, #0x40]
003bf21c bl #0x7ad7e8
003bf220 ldr r0, [sp, #0x10]
003bf224 bl #0x797124
003bf228 b #0x3bee24
003bf22c ldr r3, [pc, #0x24c]
003bf230 ldr r3, [r4, r3]
003bf234 ldr r3, [r3]
003bf238 str r3, [sp, #0x18]
003bf23c bl #0x42ca8c
003bf240 bl #0x42cb8c
003bf244 cmp r0, #0
003bf248 str r0, [sp, #0x10]
003bf24c beq #0x3beda0
003bf250 ldr fp, [pc, #0x22c]
003bf254 ldr r3, [r4, fp]
003bf258 ldr r2, [r3, #0x2c]
003bf25c cmp r2, #0
003bf260 beq #0x3bf29c
003bf264 ldr r0, [r3, #0x28]
003bf268 ldrb r3, [r0, #4]
003bf26c cmp r3, #0
003bf270 bne #0x3bf2b8
003bf274 ldr r1, [r0]
003bf278 sub r1, r1, #1
003bf27c cmp r1, #0
003bf280 str r1, [r0]
003bf284 bne #0x3bf28c
003bf288 bl #0x752b38
003bf28c ldr r3, [r4, fp]
003bf290 mov r2, #0
003bf294 str r2, [r3, #0x2c]
003bf298 str r2, [r3, #0x28]
003bf29c ldr r3, [pc, #0x1e4]
003bf2a0 ldr r0, [r4, fp]
003bf2a4 ldr r2, [sp, #0x10]
003bf2a8 ldr r1, [r4, r3]
003bf2ac mov r3, #0
003bf2b0 ldr r1, [r1]
003bf2b4 bl #0x427ca0
003bf2b8 ldr r0, [r4, fp]
003bf2bc bl #0x427d50
003bf2c0 mov ip, #0
003bf2c4 strb ip, [sp, #0x30]
003bf2c8 mov r2, #0
003bf2cc mov r3, #0
003bf2d0 mov ip, #2
003bf2d4 strd r2, r3, [sp, #0x48]
003bf2d8 strb ip, [sp, #0x31]
003bf2dc mov ip, #0
003bf2e0 str ip, [sp, #0x34]
003bf2e4 ldr ip, [sp, #0x4c]
003bf2e8 add fp, sp, #0x30
003bf2ec mov r1, r0
003bf2f0 ldr r2, [sp, #0x18]
003bf2f4 ldr r0, [sp, #0x10]
003bf2f8 mov r3, fp
003bf2fc str ip, [fp, #8]
003bf300 mov ip, #1
003bf304 str ip, [sp]
003bf308 bl #0x7abe0c
003bf30c mov r0, fp
003bf310 bl #0x797124
003bf314 b #0x3beda0
003bf318 mov r0, r5
003bf31c bl #0x3bb8e4
003bf320 subs r3, r0, #0
003bf324 bne #0x3bf158
003bf328 ldr r1, [sp, #0x18]
003bf32c ldrb r2, [r1, #0x29]
003bf330 cmp r2, #0
003bf334 beq #0x3bf158
003bf338 ldr r2, [sp, #0x10]
003bf33c ldr r1, [pc, #0x148]
003bf340 str r3, [sp, #0xc]
003bf344 ldr ip, [r4, r2]
003bf348 add r1, pc, r1
003bf34c mov r2, #1
003bf350 mov r0, ip
003bf354 str ip, [sp, #8]
003bf358 bl #0x4591f0
003bf35c cmn r0, #1
003bf360 mov r1, r0
003bf364 ldr r3, [sp, #0xc]
003bf368 ldr ip, [sp, #8]
003bf36c beq #0x3bf158
003bf370 mov r0, ip
003bf374 mvn r2, #0
003bf378 bl #0x4605c0
003bf37c b #0x3bf158
003bf380 mov r0, r5
003bf384 bl #0x3bb8e4
003bf388 subs r3, r0, #0
003bf38c bne #0x3bf148
003bf390 ldr r1, [sp, #0x18]
003bf394 ldrb r2, [r1, #0x2e]
003bf398 cmp r2, #0
003bf39c beq #0x3bf148
003bf3a0 ldr r2, [sp, #0x10]
003bf3a4 ldr r1, [pc, #0xe4]
003bf3a8 str r3, [sp, #0xc]
003bf3ac ldr ip, [r4, r2]
003bf3b0 add r1, pc, r1
003bf3b4 mov r2, #1
003bf3b8 mov r0, ip
003bf3bc str ip, [sp, #8]
003bf3c0 bl #0x4591f0
003bf3c4 cmn r0, #1
003bf3c8 mov r1, r0
003bf3cc ldr r3, [sp, #0xc]
003bf3d0 ldr ip, [sp, #8]
003bf3d4 beq #0x3bf3e4
003bf3d8 mov r0, ip
003bf3dc mvn r2, #0
003bf3e0 bl #0x4605c0
003bf3e4 ldr r3, [r4, sb]
003bf3e8 mov r1, #0
003bf3ec ldr r2, [r3, #0x4c]
003bf3f0 ldr r3, [pc, #0x9c]
003bf3f4 strb r1, [r2, #0x2e]
003bf3f8 ldr r3, [r4, r3]
003bf3fc ldr r0, [r3]
003bf400 bl #0x317e98
003bf404 b #0x3bf148
003bf408 bl #0x30e310
003bf40c ldrsheq r5, [sp], #-0xe8
003bf410 andeq r4, r0, ip, lsr #1
003bf414 strdeq r3, r4, [r0], -r4
# _ZN9Character7_GiveXPEib 3bf498 912
003bf498 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003bf49c ldr r4, [pc, #0x344]
003bf4a0 ldr r6, [pc, #0x344]
003bf4a4 ldr sb, [pc, #0x344]
003bf4a8 add r4, pc, r4
003bf4ac ldr r3, [r4, r6]
003bf4b0 sub sp, sp, #0x54
003bf4b4 ldr r7, [pc, #0x338]
003bf4b8 ldr sl, [r4, sb]
003bf4bc str r2, [sp, #8]
003bf4c0 ldr r2, [pc, #0x330]
003bf4c4 ldr r3, [r3]
003bf4c8 add r7, pc, r7
003bf4cc mov r5, r0
003bf4d0 add r2, pc, r2
003bf4d4 mov fp, r1
003bf4d8 ldr r0, [sl, #0x2c]
003bf4dc mov r1, r7
003bf4e0 str r3, [sp, #0x4c]
003bf4e4 bl #0x4c4bdc
003bf4e8 mov r8, r0
003bf4ec mov r0, r5
003bf4f0 bl #0x3bb918
003bf4f4 cmp r0, #1
003bf4f8 beq #0x3bf690
003bf4fc mov r0, r5
003bf500 bl #0x3bb918
003bf504 cmp r0, #2
003bf508 beq #0x3bf6ac
003bf50c add r7, r5, #0x560
003bf510 mov r0, r7
003bf514 mov r1, #0x13
003bf518 mov r2, #0
003bf51c bl #0x3df6e0
003bf520 cmp r8, r0
003bf524 bgt #0x3bf548
003bf528 mov r0, #0
003bf52c ldr r3, [r4, r6]
003bf530 ldr r2, [sp, #0x4c]
003bf534 ldr r3, [r3]
003bf538 cmp r2, r3
003bf53c bne #0x3bf7e4
003bf540 add sp, sp, #0x54
003bf544 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bf548 ldr r3, [r5]
003bf54c mov r0, r5
003bf550 mov lr, pc
003bf554 ldr pc, [r3, #0x28]
003bf558 cmp r0, #0
003bf55c beq #0x3bf528
003bf560 ldr r3, [r5]
003bf564 mov r0, r5
003bf568 mov lr, pc
003bf56c ldr pc, [r3, #0x54]
003bf570 cmp r0, #0
003bf574 bne #0x3bf528
003bf578 ldr r0, [r4, sb]
003bf57c bl #0x31f594
003bf580 ldr r3, [r0, #0x150]
003bf584 cmp r3, #0
003bf588 bne #0x3bf528
003bf58c cmp fp, #0
003bf590 blt #0x3bf790
003bf594 ldr r3, [pc, #0x260]
003bf598 add r8, sp, #0x34
003bf59c ldr sl, [r4, r3]
003bf5a0 str r3, [sp, #0xc]
003bf5a4 mov r0, sl
003bf5a8 bl #0x337888
003bf5ac ldr r1, [pc, #0x24c]
003bf5b0 add r2, sp, #0x18
003bf5b4 mov r0, r8
003bf5b8 add r1, pc, r1
003bf5bc bl #0x3140ec
003bf5c0 mov r0, sl
003bf5c4 mov r1, r8
003bf5c8 bl #0x337a88
003bf5cc mov sl, r0
003bf5d0 mov r0, r8
003bf5d4 bl #0x318254
003bf5d8 cmp sl, #0
003bf5dc bne #0x3bf76c
003bf5e0 mov r0, r5
003bf5e4 bl #0x3bb918
003bf5e8 mov sl, r0
003bf5ec ldr r0, [r4, sb]
003bf5f0 bl #0x31f594
003bf5f4 ldr r3, [r0, #0x118]
003bf5f8 mov r0, r7
003bf5fc add r8, sp, #0x1c
003bf600 cmp sl, r3
003bf604 movlt fp, #0x100
003bf608 mov r1, fp
003bf60c bl #0x3de7ec
003bf610 mov r1, #0x21
003bf614 mov r2, r0
003bf618 mov r0, r7
003bf61c bl #0x3e0708
003bf620 ldr r3, [sp, #0xc]
003bf624 ldr sl, [r4, r3]
003bf628 mov r0, sl
003bf62c bl #0x337888
003bf630 ldr r1, [pc, #0x1cc]
003bf634 add r2, sp, #0x14
003bf638 mov r0, r8
003bf63c add r1, pc, r1
003bf640 bl #0x3140ec
003bf644 mov r1, r8
003bf648 mov r0, sl
003bf64c bl #0x337a88
003bf650 mov r0, r8
003bf654 bl #0x318254
003bf658 mov r1, #0x21
003bf65c mov r0, r7
003bf660 bl #0x3bd130
003bf664 mov r1, #0x22
003bf668 mov r8, r0
003bf66c mov r0, r7
003bf670 bl #0x3bd130
003bf674 cmp r8, r0
003bf678 bge #0x3bf6fc
003bf67c ldr r3, [sp, #8]
003bf680 cmp r3, #0
003bf684 bne #0x3bf6c8
003bf688 mov r0, #1
003bf68c b #0x3bf52c
003bf690 ldr r2, [pc, #0x170]
003bf694 ldr r0, [sl, #0x2c]
003bf698 mov r1, r7
003bf69c add r2, pc, r2
003bf6a0 bl #0x4c4bdc
003bf6a4 mov r8, r0
003bf6a8 b #0x3bf50c
003bf6ac ldr r2, [pc, #0x158]
003bf6b0 ldr r0, [sl, #0x2c]
003bf6b4 mov r1, r7
003bf6b8 add r2, pc, r2
003bf6bc bl #0x4c4bdc
003bf6c0 mov r8, r0
003bf6c4 b #0x3bf50c
003bf6c8 ldr r3, [r4, sb]
003bf6cc mov r1, r5
003bf6d0 mov r2, #0
003bf6d4 ldr r0, [r3, #0x40]
003bf6d8 bl #0x36eea8
003bf6dc ldr r1, [pc, #0x12c]
003bf6e0 ldr r3, [r0, #0x670]
003bf6e4 asr r2, fp, #8
003bf6e8 ldr r0, [r4, r1]
003bf6ec mov r1, #6
003bf6f0 bl #0x3790e0
003bf6f4 mov r0, #1
003bf6f8 b #0x3bf52c
003bf6fc mov r1, #0x21
003bf700 mov r0, r7
003bf704 bl #0x3bd130
003bf708 mov r1, #0x22
003bf70c mov r8, r0
003bf710 mov r0, r7
003bf714 bl #0x3bd130
003bf718 rsb r1, r0, r8
003bf71c asr r1, r1, #8
003bf720 mov r0, r5
003bf724 bl #0x3beb88
003bf728 mov r1, #0x21
003bf72c mov r0, r7
003bf730 bl #0x3bd130
003bf734 mov r1, #0x22
003bf738 mov r8, r0
003bf73c mov r0, r7
003bf740 bl #0x3bd130
003bf744 cmp r8, r0
003bf748 ble #0x3bf67c
003bf74c mov r1, #0x22
003bf750 mov r0, r7
003bf754 bl #0x3bd130
003bf758 mov r1, #0x21
003bf75c mov r2, r0
003bf760 mov r0, r7
003bf764 bl #0x3e07a0
003bf768 b #0x3bf67c
003bf76c mov r1, #0x22
003bf770 mov r0, r7
003bf774 bl #0x3bd130
003bf778 mov r1, #0x21
003bf77c mov fp, r0
003bf780 mov r0, r7
003bf784 bl #0x3bd130
003bf788 rsb fp, r0, fp
003bf78c b #0x3bf5e0
003bf790 ldr r2, [pc, #0x7c]
003bf794 ldr r2, [r4, r2]
003bf798 ldr r2, [r2]
003bf79c cmp r2, #2
003bf7a0 streq r3, [r3]
003bf7a4 beq #0x3bf594
003bf7a8 cmp r2, #1
003bf7ac bne #0x3bf594
003bf7b0 ldr r0, [pc, #0x60]
003bf7b4 ldr r1, [pc, #0x60]
003bf7b8 ldr r2, [pc, #0x60]
003bf7bc ldr r0, [r4, r0]
003bf7c0 ldr r3, [pc, #0x5c]
003bf7c4 movw ip, #0x15e
003bf7c8 add r1, pc, r1
003bf7cc add r2, pc, r2
003bf7d0 add r3, pc, r3
003bf7d4 add r0, r0, #0xa8
003bf7d8 str ip, [sp]
003bf7dc bl #0x30e004
003bf7e0 b #0x3bf594
003bf7e4 bl #0x30e310
003bf7e8 subseq r5, sp, r8, ror #11
003bf7ec andeq r4, r0, ip, lsr #1
003bf7f0 strdeq r3, r4, [r0], -r4
003bf7f4 subseq r2, r0, r8, lsl #5
003bf7f8 ldrheq r5, [r0], #-0x40
003bf7fc andeq r0, r0, r4, lsl #17
003bf800 ldrsheq r5, [r0], #-0x48
003bf804 subseq r5, r0, r4, asr r2
003bf808 ldrsheq r5, [r0], #-0x24
003bf80c subseq r4, r0, r0, ror lr
003bf810 andeq r2, r0, r4, lsl r7
003bf814 andeq r3, r0, r0, asr #19
003bf818 andeq r1, r0, r0, asr #19
003bf81c subeq lr, pc, r0, lsl ip
003bf820 subseq r5, r0, r4, lsr #2
003bf824 subseq r5, r0, r0, lsr r1
# _ZN9Character12DistributeXPEPS_S0_ 3bf828 1972
003bf828 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003bf82c ldr r4, [pc, #0x73c]
003bf830 ldr r2, [pc, #0x73c]
003bf834 sub sp, sp, #0x174
003bf838 add r4, pc, r4
003bf83c ldr r3, [r4, r2]
003bf840 cmp r1, #0
003bf844 str r2, [sp, #0x1c]
003bf848 ldr r3, [r3]
003bf84c str r1, [sp, #0x14]
003bf850 str r0, [sp, #0xc]
003bf854 str r3, [sp, #0x16c]
003bf858 beq #0x3bfee0
003bf85c ldr r3, [pc, #0x714]
003bf860 ldr ip, [sp, #0x14]
003bf864 mov r2, #0x23
003bf868 ldr r3, [r4, r3]
003bf86c add r1, ip, #0xff0
003bf870 add r0, ip, #0x560
003bf874 ldr r3, [r3]
003bf878 add r1, r1, #4
003bf87c ldr sb, [pc, #0x6f8]
003bf880 str r3, [sp, #0x20]
003bf884 bl #0x3dedb4
003bf888 asr r0, r0, #8
003bf88c bl #0x30e964
003bf890 mov r1, #0
003bf894 str r0, [sp, #0x10]
003bf898 bl #0x30e2f8
003bf89c ldr r5, [r4, sb]
003bf8a0 cmp r0, #0
003bf8a4 moveq r2, #0
003bf8a8 mov r0, r5
003bf8ac streq r2, [sp, #0x10]
003bf8b0 bl #0x337888
003bf8b4 ldr r1, [pc, #0x6c4]
003bf8b8 add r8, sp, #0x154
003bf8bc add r2, sp, #0x78
003bf8c0 mov r0, r8
003bf8c4 add r1, pc, r1
003bf8c8 ldr r6, [pc, #0x6b4]
003bf8cc bl #0x3140ec
003bf8d0 mov r1, r8
003bf8d4 mov r0, r5
003bf8d8 bl #0x337a88
003bf8dc mov r0, r8
003bf8e0 bl #0x318254
003bf8e4 add r7, sp, #0x13c
003bf8e8 add r6, pc, r6
003bf8ec mov r0, r5
003bf8f0 bl #0x337888
003bf8f4 add r2, sp, #0x74
003bf8f8 mov r0, r7
003bf8fc mov r1, r6
003bf900 bl #0x3140ec
003bf904 mov r1, r7
003bf908 mov r0, r5
003bf90c bl #0x337a88
003bf910 mov r0, r7
003bf914 bl #0x318254
003bf918 mov r0, r5
003bf91c bl #0x337888
003bf920 ldr r3, [pc, #0x660]
003bf924 add r7, sp, #0x124
003bf928 add r2, sp, #0x70
003bf92c mov r1, r6
003bf930 mov r0, r7
003bf934 str r3, [sp, #0x2c]
003bf938 bl #0x3140ec
003bf93c mov r1, r7
003bf940 mov r0, r5
003bf944 bl #0x337a88
003bf948 mov r0, r7
003bf94c bl #0x318254
003bf950 ldr ip, [sp, #0x2c]
003bf954 ldr r3, [r4, ip]
003bf958 ldr r8, [r3, #0x40]
003bf95c ldr r7, [r8, #0x6c4]
003bf960 cmp r7, #4
003bf964 bgt #0x3bfcec
003bf968 ldr r2, [sp, #0x20]
003bf96c cmp r7, #0
003bf970 ldr r2, [r2, #0xa0]
003bf974 str r2, [sp, #0x24]
003bf978 ble #0x3bfd20
003bf97c ldr r3, [pc, #0x608]
003bf980 mov r6, #0
003bf984 str r6, [sp, #0x18]
003bf988 str r3, [sp, #0x30]
003bf98c ldr r3, [pc, #0x5fc]
003bf990 mov r5, r6
003bf994 add r3, pc, r3
003bf998 str r3, [sp, #0x28]
003bf99c ldr r3, [pc, #0x5f0]
003bf9a0 add r3, pc, r3
003bf9a4 str r3, [sp, #0x34]
003bf9a8 ldr r3, [pc, #0x5e8]
003bf9ac add r3, pc, r3
003bf9b0 str r3, [sp, #0x38]
003bf9b4 ldr r3, [pc, #0x5e0]
003bf9b8 add r3, pc, r3
003bf9bc str r3, [sp, #0x3c]
003bf9c0 mov r0, r8
003bf9c4 mov r1, r5
003bf9c8 mov r2, #1
003bf9cc bl #0x36e744
003bf9d0 ldr sl, [r0, #0x660]
003bf9d4 cmp sl, #0
003bf9d8 beq #0x3bfde4
003bf9dc mov r1, sl
003bf9e0 ldr r2, [sp, #0x14]
003bf9e4 ldr r0, [sp, #0x10]
003bf9e8 bl #0x3bd918
003bf9ec add fp, sp, #0x44
003bf9f0 mov r1, #0
003bf9f4 str r0, [fp, r6]
003bf9f8 bl #0x30e4b4
003bf9fc cmp r0, #0
003bfa00 beq #0x3bfad8
003bfa04 ldr r2, [sp, #0xc]
003bfa08 cmp r2, #0
003bfa0c beq #0x3bfe78
003bfa10 ldr r1, [r2, #0x160]
003bfa14 ldr r0, [sl, #0x160]
003bfa18 bl #0x30e3ac
003bfa1c ldr ip, [sp, #0xc]
003bfa20 mov r3, r0
003bfa24 ldr r0, [sl, #0x164]
003bfa28 ldr r1, [ip, #0x164]
003bfa2c str r3, [sp, #8]
003bfa30 bl #0x30e3ac
003bfa34 ldr r3, [sp, #8]
003bfa38 mov r2, r0
003bfa3c str r2, [sp, #8]
003bfa40 mov r1, r3
003bfa44 mov r0, r3
003bfa48 bl #0x30ed6c
003bfa4c ldr r2, [sp, #8]
003bfa50 mov r3, r0
003bfa54 str r3, [sp, #8]
003bfa58 mov r1, r2
003bfa5c mov r0, r2
003bfa60 bl #0x30ed6c
003bfa64 ldr r3, [sp, #8]
003bfa68 mov r1, r0
003bfa6c mov r0, r3
003bfa70 bl #0x30eba4
003bfa74 bl #0x30e124
003bfa78 ldr r2, [sp, #0xc]
003bfa7c cmp r2, sl
003bfa80 beq #0x3bfa98
003bfa84 mov r1, r0
003bfa88 ldr r0, [sp, #0x24]
003bfa8c bl #0x30e4b4
003bfa90 cmp r0, #0
003bfa94 beq #0x3bfe30
003bfa98 ldr fp, [r4, sb]
003bfa9c ldr r2, [sp, #0x18]
003bfaa0 add sl, sp, #0x10c
003bfaa4 mov r0, fp
003bfaa8 add r2, r2, #1
003bfaac str r2, [sp, #0x18]
003bfab0 bl #0x337888
003bfab4 add r2, sp, #0x6c
003bfab8 ldr r1, [sp, #0x28]
003bfabc mov r0, sl
003bfac0 bl #0x3140ec
003bfac4 mov r0, fp
003bfac8 mov r1, sl
003bfacc bl #0x337a88
003bfad0 mov r0, sl
003bfad4 bl #0x318254
003bfad8 add r5, r5, #1
003bfadc cmp r5, r7
003bfae0 add r6, r6, #4
003bfae4 bne #0x3bf9c0
003bfae8 ldr r3, [sp, #0x18]
003bfaec cmp r3, #0
003bfaf0 beq #0x3bfd20
003bfaf4 sub r0, r3, #1
003bfaf8 bl #0x30e964
003bfafc ldr ip, [sp, #0x20]
003bfb00 add r6, sp, #0xdc
003bfb04 ldr fp, [pc, #0x494]
003bfb08 ldr r1, [ip, #0xa4]
003bfb0c bl #0x30ed6c
003bfb10 ldr sl, [r4, sb]
003bfb14 str r0, [sp, #0xc]
003bfb18 mov r5, #0
003bfb1c mov r0, sl
003bfb20 bl #0x337888
003bfb24 ldr r1, [pc, #0x478]
003bfb28 add r2, sp, #0x64
003bfb2c mov r0, r6
003bfb30 add r1, pc, r1
003bfb34 bl #0x3140ec
003bfb38 mov r0, sl
003bfb3c mov r1, r6
003bfb40 bl #0x337a88
003bfb44 mov r0, r6
003bfb48 bl #0x318254
003bfb4c ldr r3, [pc, #0x454]
003bfb50 add r2, sp, #0x44
003bfb54 add fp, pc, fp
003bfb58 add r3, pc, r3
003bfb5c str r3, [sp, #0x18]
003bfb60 ldr r3, [pc, #0x444]
003bfb64 str r2, [sp, #0x10]
003bfb68 mov sl, r7
003bfb6c add r3, pc, r3
003bfb70 str r3, [sp, #0x20]
003bfb74 ldr r3, [pc, #0x434]
003bfb78 add r3, pc, r3
003bfb7c str r3, [sp, #0x24]
003bfb80 b #0x3bfbc4
003bfb84 ldr r7, [r4, sb]
003bfb88 add r6, sp, #0xc4
003bfb8c mov r0, r7
003bfb90 bl #0x337888
003bfb94 add r2, sp, #0x60
003bfb98 mov r1, fp
003bfb9c mov r0, r6
003bfba0 bl #0x3140ec
003bfba4 mov r0, r7
003bfba8 mov r1, r6
003bfbac bl #0x337a88
003bfbb0 mov r0, r6
003bfbb4 bl #0x318254
003bfbb8 add r5, r5, #1
003bfbbc cmp r5, sl
003bfbc0 beq #0x3bfd58
003bfbc4 mov r0, r8
003bfbc8 mov r1, r5
003bfbcc mov r2, #1
003bfbd0 bl #0x36e744
003bfbd4 ldr r6, [r0, #0x660]
003bfbd8 cmp r6, #0
003bfbdc beq #0x3bfbb8
003bfbe0 mov r0, #0x42000000
003bfbe4 ldr r1, [sp, #0xc]
003bfbe8 add r0, r0, #0xc80000
003bfbec bl #0x30e3ac
003bfbf0 ldr r3, [sp, #0x10]
003bfbf4 ldr r1, [r3, r5, lsl #2]
003bfbf8 bl #0x30ed6c
003bfbfc mov r1, #0x42000000
003bfc00 add r1, r1, #0xc80000
003bfc04 bl #0x30ec94
003bfc08 mov r1, #0
003bfc0c mov r7, r0
003bfc10 bl #0x30e4b4
003bfc14 cmp r0, #0
003bfc18 beq #0x3bfb84
003bfc1c mov r1, #0x3f800000
003bfc20 mov r0, r7
003bfc24 bl #0x30eba4
003bfc28 bl #0x30e4cc
003bfc2c lsl r7, r0, #8
003bfc30 mov r1, r7
003bfc34 mov r0, r6
003bfc38 mov r2, #1
003bfc3c bl #0x3bf498
003bfc40 cmp r0, #0
003bfc44 beq #0x3bfb84
003bfc48 ldr ip, [sp, #0x2c]
003bfc4c mov r1, r6
003bfc50 ldr ip, [r4, ip]
003bfc54 ldr r0, [ip, #0x40]
003bfc58 str ip, [sp, #0x28]
003bfc5c bl #0x36effc
003bfc60 cmp r0, #0
003bfc64 beq #0x3bfb84
003bfc68 mov r0, r6
003bfc6c bl #0x3bb918
003bfc70 mov r3, r0
003bfc74 ldr r0, [sp, #0x28]
003bfc78 str r3, [sp, #8]
003bfc7c bl #0x31f594
003bfc80 ldr r3, [sp, #8]
003bfc84 ldr r2, [r0, #0x118]
003bfc88 cmp r3, r2
003bfc8c movlt r7, #0x100
003bfc90 bl #0x413e90
003bfc94 mov r1, r7
003bfc98 mov r3, r0
003bfc9c add r0, r6, #0x560
003bfca0 str r3, [sp, #8]
003bfca4 bl #0x3de7ec
003bfca8 ldr r3, [sp, #8]
003bfcac mov r7, r0
003bfcb0 ldr r1, [sp, #0x18]
003bfcb4 mov r0, r3
003bfcb8 bl #0x414678
003bfcbc ldr r2, [sp, #0x28]
003bfcc0 mov r6, r0
003bfcc4 ldr r1, [sp, #0x20]
003bfcc8 ldr r0, [r2, #0x2c]
003bfccc ldr r2, [sp, #0x24]
003bfcd0 bl #0x4c4bdc
003bfcd4 asr r1, r7, #8
003bfcd8 mov r3, r0
003bfcdc mov r2, r6
003bfce0 ldr r0, [sp, #0x14]
003bfce4 bl #0x3af0c8
003bfce8 b #0x3bfb84
003bfcec ldr r3, [pc, #0x298]
003bfcf0 ldr r3, [r4, r3]
003bfcf4 ldr r3, [r3]
003bfcf8 cmp r3, #2
003bfcfc moveq r3, #0
003bfd00 streq r3, [r3]
003bfd04 beq #0x3bfd10
003bfd08 cmp r3, #1
003bfd0c beq #0x3bff38
003bfd10 ldr r3, [sp, #0x20]
003bfd14 ldr r3, [r3, #0xa0]
003bfd18 str r3, [sp, #0x24]
003bfd1c b #0x3bf97c
003bfd20 ldr r6, [r4, sb]
003bfd24 add r5, sp, #0xac
003bfd28 mov r0, r6
003bfd2c bl #0x337888
003bfd30 ldr r1, [pc, #0x27c]
003bfd34 add r2, sp, #0x5c
003bfd38 mov r0, r5
003bfd3c add r1, pc, r1
003bfd40 bl #0x3140ec
003bfd44 mov r0, r6
003bfd48 mov r1, r5
003bfd4c bl #0x337a88
003bfd50 mov r0, r5
003bfd54 bl #0x318254
003bfd58 ldr r5, [r4, sb]
003bfd5c ldr r6, [pc, #0x254]
003bfd60 add r7, sp, #0x94
003bfd64 mov r0, r5
003bfd68 add r6, pc, r6
003bfd6c bl #0x337888
003bfd70 add r2, sp, #0x58
003bfd74 mov r0, r7
003bfd78 mov r1, r6
003bfd7c bl #0x3140ec
003bfd80 mov r1, r7
003bfd84 mov r0, r5
003bfd88 bl #0x337a88
003bfd8c mov r0, r7
003bfd90 bl #0x318254
003bfd94 add r7, sp, #0x7c
003bfd98 mov r0, r5
003bfd9c bl #0x337888
003bfda0 mov r1, r6
003bfda4 add r2, sp, #0x54
003bfda8 mov r0, r7
003bfdac bl #0x3140ec
003bfdb0 mov r0, r5
003bfdb4 mov r1, r7
003bfdb8 bl #0x337a88
003bfdbc mov r0, r7
003bfdc0 bl #0x318254
003bfdc4 ldr ip, [sp, #0x1c]
003bfdc8 ldr r2, [sp, #0x16c]
003bfdcc ldr r3, [r4, ip]
003bfdd0 ldr r3, [r3]
003bfdd4 cmp r2, r3
003bfdd8 bne #0x3bff6c
003bfddc add sp, sp, #0x174
003bfde0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bfde4 ldr ip, [sp, #0x30]
003bfde8 ldr r3, [r4, ip]
003bfdec ldr r3, [r3]
003bfdf0 cmp r3, #2
003bfdf4 streq sl, [sl]
003bfdf8 beq #0x3bfad8
003bfdfc cmp r3, #1
003bfe00 bne #0x3bfad8
003bfe04 ldr r0, [pc, #0x1b0]
003bfe08 ldr r3, [pc, #0x1b0]
003bfe0c movw ip, #0x103
003bfe10 ldr r0, [r4, r0]
003bfe14 add r3, pc, r3
003bfe18 ldr r1, [sp, #0x38]
003bfe1c ldr r2, [sp, #0x3c]
003bfe20 add r0, r0, #0xa8
003bfe24 str ip, [sp]
003bfe28 bl #0x30e004
003bfe2c b #0x3bfad8
003bfe30 ldr r3, [r4, sb]
003bfe34 mov r2, #0
003bfe38 add sl, sp, #0xf4
003bfe3c mov r0, r3
003bfe40 str r2, [fp, r6]
003bfe44 str r3, [sp, #8]
003bfe48 bl #0x337888
003bfe4c add r2, sp, #0x68
003bfe50 ldr r1, [sp, #0x34]
003bfe54 mov r0, sl
003bfe58 bl #0x3140ec
003bfe5c ldr r3, [sp, #8]
003bfe60 mov r1, sl
003bfe64 mov r0, r3
003bfe68 bl #0x337a88
003bfe6c mov r0, sl
003bfe70 bl #0x318254
003bfe74 b #0x3bfad8
003bfe78 ldr r3, [sp, #0x14]
003bfe7c ldr r0, [sl, #0x160]
003bfe80 ldr r1, [r3, #0x160]
003bfe84 bl #0x30e3ac
003bfe88 ldr ip, [sp, #0x14]
003bfe8c mov r3, r0
003bfe90 ldr r0, [sl, #0x164]
003bfe94 ldr r1, [ip, #0x164]
003bfe98 str r3, [sp, #8]
003bfe9c bl #0x30e3ac
003bfea0 ldr r3, [sp, #8]
003bfea4 mov r2, r0
003bfea8 str r2, [sp, #8]
003bfeac mov r1, r3
003bfeb0 mov r0, r3
003bfeb4 bl #0x30ed6c
003bfeb8 ldr r2, [sp, #8]
003bfebc mov sl, r0
003bfec0 mov r1, r2
003bfec4 mov r0, r2
003bfec8 bl #0x30ed6c
003bfecc mov r1, r0
003bfed0 mov r0, sl
003bfed4 bl #0x30eba4
003bfed8 bl #0x30e124
003bfedc b #0x3bfa84
003bfee0 ldr r3, [pc, #0xa4]
003bfee4 ldr r3, [r4, r3]
003bfee8 ldr r3, [r3]
003bfeec cmp r3, #2
003bfef0 moveq r3, r1
003bfef4 streq r3, [r3]
003bfef8 beq #0x3bfdc4
003bfefc cmp r3, #1
003bff00 bne #0x3bfdc4
003bff04 ldr r0, [pc, #0xb0]
003bff08 ldr r1, [pc, #0xb4]
003bff0c ldr r2, [pc, #0xb4]
003bff10 ldr r0, [r4, r0]
003bff14 ldr r3, [pc, #0xb0]
003bff18 mov ip, #0xdb
003bff1c add r1, pc, r1
003bff20 add r2, pc, r2
003bff24 add r3, pc, r3
003bff28 add r0, r0, #0xa8
003bff2c str ip, [sp]
003bff30 bl #0x30e004
003bff34 b #0x3bfdc4
003bff38 ldr r0, [pc, #0x7c]
003bff3c ldr r1, [pc, #0x8c]
003bff40 ldr r2, [pc, #0x8c]
003bff44 ldr r0, [r4, r0]
003bff48 ldr r3, [pc, #0x88]
003bff4c mov ip, #0xf7
003bff50 add r1, pc, r1
003bff54 add r2, pc, r2
003bff58 add r3, pc, r3
003bff5c add r0, r0, #0xa8
003bff60 str ip, [sp]
003bff64 bl #0x30e004
003bff68 b #0x3bfd10
003bff6c bl #0x30e310
003bff70 subseq r5, sp, r8, asr r2
003bff74 andeq r4, r0, ip, lsr #1
003bff78 andeq r3, r0, r8, asr #5
003bff7c andeq r0, r0, r4, lsl #17
003bff80 subseq r5, r0, ip, lsl #4
003bff84 subseq r5, r0, r0, ror r0
003bff88 strdeq r3, r4, [r0], -r4
003bff8c andeq r3, r0, r0, asr #19
003bff90 subseq r4, r0, r4, asr #31
003bff94 ldrheq r4, [r0], #-0xf8
003bff98 subeq lr, pc, ip, lsr #20
003bff9c subseq r5, r0, r0, ror r1
003bffa0 subseq r4, r0, r4, lsl #28
003bffa4 subseq r4, r0, r8, lsr #28
003bffa8 subseq r4, r0, r0, ror #31
003bffac subseq r3, r0, r4, lsl #23
003bffb0 ldrsbeq r4, [r0], #-0xf0
003bffb4 subseq r4, r0, ip, lsl ip
003bffb8 ldrsheq r4, [r0], #-0xb0
003bffbc andeq r1, r0, r0, asr #19
003bffc0 subseq r4, r0, ip, ror #21
003bffc4 strheq lr, [pc], #-0x4c
003bffc8 subseq r4, r0, r0, lsr #23
003bffcc ldrsbeq r4, [r0], #-0x9c
003bffd0 subeq lr, pc, r8, lsl #9
# _ZN13ItemInventory7AddLootEiiiib 40407c 1500
0040407c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00404080 ldr r6, [pc, #0x5a0]
00404084 ldr ip, [pc, #0x5a0]
00404088 subs r5, r1, #0
0040408c add r6, pc, r6
00404090 ldr r1, [r6, ip]
00404094 sub sp, sp, #0x1d4
00404098 str r2, [sp, #0xc]
0040409c ldr r2, [r1]
004040a0 str ip, [sp, #0x1c]
004040a4 mov sb, r0
004040a8 str r3, [sp, #0x10]
004040ac str r2, [sp, #0x1cc]
004040b0 ldrb fp, [sp, #0x1fc]
004040b4 blt #0x4040cc
004040b8 ldr r3, [pc, #0x570]
004040bc ldr r3, [r6, r3]
004040c0 ldr r3, [r3]
004040c4 cmp r5, r3
004040c8 blt #0x4040ec
004040cc ldr r2, [sp, #0x1c]
004040d0 ldr r3, [r6, r2]
004040d4 ldr r2, [sp, #0x1cc]
004040d8 ldr r3, [r3]
004040dc cmp r2, r3
004040e0 bne #0x404624
004040e4 add sp, sp, #0x1d4
004040e8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004040ec ldr r3, [pc, #0x540]
004040f0 add r4, sp, #0x1b4
004040f4 ldr r7, [r6, r3]
004040f8 mov r0, r7
004040fc bl #0x337888
00404100 ldr r1, [pc, #0x530]
00404104 add r2, sp, #0x80
00404108 mov r0, r4
0040410c add r1, pc, r1
00404110 bl #0x3140ec
00404114 mov r1, r4
00404118 mov r0, r7
0040411c bl #0x337a88
00404120 mov r8, r0
00404124 mov r0, r4
00404128 bl #0x318254
0040412c cmp r8, #0
00404130 bne #0x4040cc
00404134 ldr sl, [pc, #0x500]
00404138 add r4, sp, #0x19c
0040413c mov r0, r7
00404140 add sl, pc, sl
00404144 bl #0x337888
00404148 add r2, sp, #0x7c
0040414c mov r0, r4
00404150 mov r1, sl
00404154 bl #0x3140ec
00404158 mov r1, r4
0040415c mov r0, r7
00404160 bl #0x337a88
00404164 mov r0, r4
00404168 bl #0x318254
0040416c add r4, sp, #0x184
00404170 mov r0, r7
00404174 bl #0x337888
00404178 add r2, sp, #0x78
0040417c mov r1, sl
00404180 mov r0, r4
00404184 bl #0x3140ec
00404188 mov r1, r4
0040418c mov r0, r7
00404190 bl #0x337a88
00404194 add r1, sp, #0x4c
00404198 mov r0, r4
0040419c str r1, [sp, #0x30]
004041a0 bl #0x318254
004041a4 mov r2, r8
004041a8 mov r0, r5
004041ac ldr r1, [sp, #0x30]
004041b0 str r8, [sp, #0x4c]
004041b4 str r8, [sp, #0x50]
004041b8 str r8, [sp, #0x54]
004041bc bl #0x4039a0
004041c0 ldr r3, [sp, #0x4c]
004041c4 ldr r2, [sp, #0x50]
004041c8 rsb r3, r3, r2
004041cc lsrs r3, r3, #2
004041d0 bne #0x404240
004041d4 add r4, sp, #0x16c
004041d8 mov r0, r7
004041dc bl #0x337888
004041e0 add r2, sp, #0x74
004041e4 mov r0, r4
004041e8 mov r1, sl
004041ec bl #0x3140ec
004041f0 mov r1, r4
004041f4 mov r0, r7
004041f8 bl #0x337a88
004041fc mov r0, r4
00404200 bl #0x318254
00404204 add r4, sp, #0x154
00404208 mov r0, r7
0040420c bl #0x337888
00404210 mov r1, sl
00404214 add r2, sp, #0x70
00404218 mov r0, r4
0040421c bl #0x3140ec
00404220 mov r0, r7
00404224 mov r1, r4
00404228 bl #0x337a88
0040422c mov r0, r4
00404230 bl #0x318254
00404234 ldr r0, [sp, #0x30]
00404238 bl #0x4024bc
0040423c b #0x4040cc
00404240 add r4, sp, #0x13c
00404244 mov r0, r7
00404248 bl #0x337888
0040424c add r2, sp, #0x6c
00404250 mov r0, r4
00404254 mov r1, sl
00404258 bl #0x3140ec
0040425c mov r1, r4
00404260 mov r0, r7
00404264 bl #0x337a88
00404268 mov r0, r4
0040426c bl #0x318254
00404270 add r4, sp, #0x124
00404274 mov r0, r7
00404278 bl #0x337888
0040427c add r2, sp, #0x68
00404280 mov r1, sl
00404284 mov r0, r4
00404288 bl #0x3140ec
0040428c mov r1, r4
00404290 mov r0, r7
00404294 bl #0x337a88
00404298 add r2, sp, #0x40
0040429c mov r0, r4
004042a0 str r2, [sp, #0x34]
004042a4 bl #0x318254
004042a8 ldrb r2, [sb, #0x2d]
004042ac ldr r0, [sp, #0x30]
004042b0 ldr r1, [sp, #0x34]
004042b4 str r8, [sp, #0x48]
004042b8 str r8, [sp, #0x40]
004042bc str r8, [sp, #0x44]
004042c0 bl #0x402dfc
004042c4 ldr r3, [sp, #0x40]
004042c8 ldr r2, [sp, #0x44]
004042cc rsb r3, r3, r2
004042d0 lsrs r3, r3, #4
004042d4 beq #0x404550
004042d8 add r4, sp, #0xdc
004042dc mov r0, r7
004042e0 bl #0x337888
004042e4 add r2, sp, #0x5c
004042e8 mov r0, r4
004042ec mov r1, sl
004042f0 bl #0x3140ec
004042f4 mov r1, r4
004042f8 mov r0, r7
004042fc bl #0x337a88
00404300 mov r0, r4
00404304 bl #0x318254
00404308 add r4, sp, #0xc4
0040430c mov r0, r7
00404310 bl #0x337888
00404314 add r2, sp, #0x58
00404318 mov r1, sl
0040431c mov r0, r4
00404320 bl #0x3140ec
00404324 mov r1, r4
00404328 mov r0, r7
0040432c bl #0x337a88
00404330 mov r0, r4
00404334 bl #0x318254
00404338 ldr r4, [sp, #0x40]
0040433c ldr r3, [sp, #0x44]
00404340 cmp r4, r3
00404344 beq #0x4044dc
00404348 ldr r2, [pc, #0x2f0]
0040434c ldr r1, [pc, #0x2f0]
00404350 ldr ip, [pc, #0x2f0]
00404354 str r2, [sp, #0x20]
00404358 ldr r2, [pc, #0x2ec]
0040435c str fp, [sp, #0x18]
00404360 str ip, [sp, #0x24]
00404364 add r2, pc, r2
00404368 str r2, [sp, #0x3c]
0040436c ldr r2, [pc, #0x2dc]
00404370 add r4, r4, #0x10
00404374 str sb, [sp, #0x14]
00404378 add r2, pc, r2
0040437c str r2, [sp, #0x38]
00404380 mov r8, r6
00404384 mov fp, r1
00404388 ldr r2, [r4, #-4]
0040438c ldr r2, [r2, #0x10]
00404390 sub r2, r2, #2
00404394 cmp r2, #1
00404398 movhi sb, #1
0040439c bhi #0x4043b4
004043a0 ldr r2, [r8, fp]
004043a4 ldr r2, [r2, #0x40]
004043a8 ldr sb, [r2, #0x6c4]
004043ac cmp sb, #0
004043b0 ble #0x4044cc
004043b4 ldr r3, [pc, #0x298]
004043b8 mov r6, #0
004043bc str r3, [sp, #0x2c]
004043c0 ldr sl, [r8, fp]
004043c4 ldrsh r5, [r4, #-0x10]
004043c8 mov r0, sl
004043cc bl #0x31f594
004043d0 subs r7, r0, #0
004043d4 beq #0x4043e4
004043d8 mov r0, sl
004043dc bl #0x31f594
004043e0 ldr r7, [r0, #0x118]
004043e4 ldr ip, [sp, #0x18]
004043e8 cmp ip, #0
004043ec bne #0x404418
004043f0 ldr r1, [sp, #0x24]
004043f4 add sl, r5, #2
004043f8 ldr r3, [r8, r1]
004043fc ldr r3, [r3]
00404400 cmp sl, r3
00404404 bhs #0x404418
00404408 cmp r7, #1
0040440c beq #0x4045bc
00404410 cmp r7, #2
00404414 beq #0x4044f0
00404418 mov sl, r5
0040441c ldrsh r3, [r4, #-0x10]
00404420 cmp r3, sl
00404424 beq #0x40444c
00404428 ldr ip, [sp, #0x20]
0040442c mov r1, #0xa4
00404430 ldr r3, [r8, ip]
00404434 ldr r2, [r3]
00404438 ldrb r3, [r4, #-8]
0040443c strh sl, [r4, #-0x10]
00404440 mla r2, r1, sl, r2
00404444 strb r3, [r4, #-8]
00404448 str r2, [r4, #-4]
0040444c mov r1, #0
00404450 mov r0, #0x6c
00404454 bl #0x310570
00404458 mov r2, #1
0040445c mov r1, sl
00404460 mov r5, r0
00404464 bl #0x3fc26c
00404468 ldrsb r1, [r4, #-8]
0040446c mov r0, r5
00404470 add r6, r6, #1
00404474 cmn r1, #2
00404478 moveq r1, #0x63
0040447c strbeq r1, [r4, #-8]
00404480 moveq r1, #0x63
00404484 bl #0x3fa0e4
00404488 ldr r0, [r4, #-0xc]
0040448c ldr r2, [sp, #0x10]
00404490 ldr r3, [sp, #0x1f8]
00404494 mov r1, r5
00404498 str r7, [sp]
0040449c bl #0x403310
004044a0 mov r0, r5
004044a4 ldr r1, [sp, #0xc]
004044a8 bl #0x4020b4
004044ac ldr r0, [sp, #0x14]
004044b0 mov r1, r5
004044b4 mov r2, #1
004044b8 mov r3, #0
004044bc bl #0x3ff5d4
004044c0 cmp r6, sb
004044c4 bne #0x4043c0
004044c8 ldr r3, [sp, #0x44]
004044cc cmp r3, r4
004044d0 add r4, r4, #0x10
004044d4 bne #0x404388
004044d8 mov r6, r8
004044dc ldr r0, [sp, #0x34]
004044e0 bl #0x40247c
004044e4 ldr r0, [sp, #0x30]
004044e8 bl #0x4024bc
004044ec b #0x4040cc
004044f0 ldr r3, [sp, #0x2c]
004044f4 add r1, sp, #0x84
004044f8 str r1, [sp, #0x28]
004044fc ldr ip, [r8, r3]
00404500 mov r0, r1
00404504 ldr r3, [ip]
00404508 ldr r1, [r3, r5, lsl #2]
0040450c str ip, [sp, #8]
00404510 bl #0x30e520
00404514 ldr r0, [sp, #0x28]
00404518 bl #0x30de54
0040451c ldr r2, [sp, #0x28]
00404520 ldr r1, [sp, #0x3c]
00404524 add r0, r2, r0
00404528 mov r2, #0xa
0040452c bl #0x30e868
00404530 ldr ip, [sp, #8]
00404534 ldr r0, [sp, #0x28]
00404538 ldr r3, [ip]
0040453c ldr r1, [r3, sl, lsl #2]
00404540 bl #0x30e31c
00404544 cmp r0, #0
00404548 beq #0x40441c
0040454c b #0x404418
00404550 add r4, sp, #0x10c
00404554 mov r0, r7
00404558 bl #0x337888
0040455c add r2, sp, #0x64
00404560 mov r1, sl
00404564 mov r0, r4
00404568 bl #0x3140ec
0040456c mov r1, r4
00404570 mov r0, r7
00404574 bl #0x337a88
00404578 mov r0, r4
0040457c bl #0x318254
00404580 add r4, sp, #0xf4
00404584 mov r0, r7
00404588 bl #0x337888
0040458c add r2, sp, #0x60
00404590 mov r1, sl
00404594 mov r0, r4
00404598 bl #0x3140ec
0040459c mov r1, r4
004045a0 mov r0, r7
004045a4 bl #0x337a88
004045a8 mov r0, r4
004045ac bl #0x318254
004045b0 ldr r0, [sp, #0x34]
004045b4 bl #0x40247c
004045b8 b #0x404234
004045bc ldr r2, [sp, #0x2c]
004045c0 add ip, r5, #1
004045c4 add sl, sp, #0x84
004045c8 ldr r3, [r8, r2]
004045cc str ip, [sp, #0x28]
004045d0 mov r0, sl
004045d4 ldr r2, [r3]
004045d8 ldr r1, [r2, r5, lsl #2]
004045dc str r3, [sp, #8]
004045e0 bl #0x30e520
004045e4 mov r0, sl
004045e8 bl #0x30de54
004045ec ldr r1, [sp, #0x38]
004045f0 add r0, sl, r0
004045f4 mov r2, #6
004045f8 bl #0x30e868
004045fc ldr r3, [sp, #8]
00404600 ldr r2, [sp, #0x28]
00404604 mov r0, sl
00404608 ldr r3, [r3]
0040460c ldr r1, [r3, r2, lsl #2]
00404610 bl #0x30e31c
00404614 cmp r0, #0
00404618 ldreq sl, [sp, #0x28]
0040461c beq #0x40441c
00404620 b #0x404418
00404624 bl #0x30e310
00404628 subseq r0, sb, r4, lsl #20
0040462c andeq r4, r0, ip, lsr #1
00404630 andeq r3, r0, r0, lsr #10
00404634 andeq r0, r0, r4, lsl #17
# _ZN14CNetPlayerInfo5ResetEv 80f27c 100
0080f27c push {r4, lr}
0080f280 ldr r3, [r0, #0x1a0]
0080f284 mov r4, r0
0080f288 cmn r3, #1
0080f28c beq #0x80f2a0
0080f290 mvn r3, #0
0080f294 str r3, [r0, #0x1a0]
0080f298 add r0, r0, #0x180
0080f29c bl #0x814f84
0080f2a0 ldr r3, [r4, #0x1c8]
0080f2a4 cmn r3, #1
0080f2a8 beq #0x80f2bc
0080f2ac mvn r3, #0
0080f2b0 str r3, [r4, #0x1c8]
0080f2b4 add r0, r4, #0x1a8
0080f2b8 bl #0x814f84
0080f2bc ldr r3, [r4, #0x1f0]
0080f2c0 cmp r3, #0
0080f2c4 beq #0x80f2dc
0080f2c8 mov r3, #0
0080f2cc add r0, r4, #0x1d0
0080f2d0 str r3, [r4, #0x1f0]
0080f2d4 pop {r4, lr}
0080f2d8 b #0x814f84
0080f2dc pop {r4, pc}
# _ZN14CNetPlayerInfoC1Ev 80f7b4 1140
0080f7b4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0080f7b8 ldr r5, [pc, #0x440]
0080f7bc ldr r7, [pc, #0x440]
0080f7c0 sub sp, sp, #0x44
0080f7c4 add r5, pc, r5
0080f7c8 ldr r3, [r5, r7]
0080f7cc mov r4, r0
0080f7d0 ldr r6, [pc, #0x430]
0080f7d4 ldr r3, [r3]
0080f7d8 mov r8, #0
0080f7dc mov sb, #0
0080f7e0 str r3, [sp, #0x3c]
0080f7e4 bl #0x8138f4
0080f7e8 ldr r3, [pc, #0x41c]
0080f7ec ldr r2, [r4, #0x150]
0080f7f0 ldr r0, [r5, r6]
0080f7f4 ldr r3, [r5, r3]
0080f7f8 cmp r2, #0
0080f7fc mov ip, #0x138
0080f800 mov r2, #0
0080f804 add r3, r3, #8
0080f808 strd r8, sb, [r4, ip]
0080f80c mvn r1, #0
0080f810 str r3, [r4]
0080f814 str r2, [r4, #0x148]
0080f818 strb r2, [r4, #0x14c]
0080f81c add r0, r0, #8
0080f820 mov r3, #0x11
0080f824 addeq r2, r4, #0x130
0080f828 str r3, [r4, #0x134]
0080f82c str r1, [r4, #0x144]
0080f830 str r0, [r4, #0x130]
0080f834 str r1, [r4, #0x140]
0080f838 streq r2, [sp, #8]
0080f83c beq #0x80f854
0080f840 add r3, r4, #0x130
0080f844 str r3, [sp, #8]
0080f848 str r2, [r4, #0x150]
0080f84c ldr r0, [sp, #8]
0080f850 bl #0x814f84
0080f854 ldr r8, [pc, #0x3b4]
0080f858 ldr r3, [r4, #0x178]
0080f85c ldr r1, [r5, r6]
0080f860 ldr r0, [r5, r8]
0080f864 cmp r3, #0
0080f868 mov fp, #0
0080f86c add r0, r0, #8
0080f870 mov ip, #0x160
0080f874 mov sl, #0
0080f878 strd sl, fp, [r4, ip]
0080f87c mvn r2, #0
0080f880 mov r3, #0
0080f884 str r0, [r4, #0x130]
0080f888 add r1, r1, #8
0080f88c mov r0, #8
0080f890 addeq fp, r4, #0x158
0080f894 str r0, [r4, #0x15c]
0080f898 str r2, [r4, #0x16c]
0080f89c str r1, [r4, #0x158]
0080f8a0 str r2, [r4, #0x168]
0080f8a4 str r3, [r4, #0x170]
0080f8a8 strb r3, [r4, #0x174]
0080f8ac streq fp, [sp, #0xc]
0080f8b0 beq #0x80f8c8
0080f8b4 add r2, r4, #0x158
0080f8b8 str r2, [sp, #0xc]
0080f8bc str r3, [r4, #0x178]
0080f8c0 ldr r0, [sp, #0xc]
0080f8c4 bl #0x814f84
0080f8c8 ldr r3, [pc, #0x344]
0080f8cc ldr r2, [r4, #0x1a0]
0080f8d0 ldr r0, [r5, r6]
0080f8d4 ldr r3, [r5, r3]
0080f8d8 cmp r2, #0
0080f8dc mov fp, #0
0080f8e0 add r3, r3, #8
0080f8e4 mov ip, #0x188
0080f8e8 mov sl, #0
0080f8ec strd sl, fp, [r4, ip]
0080f8f0 mvn r1, #0
0080f8f4 mov r2, #0
0080f8f8 str r3, [r4, #0x158]
0080f8fc add r0, r0, #8
0080f900 mov r3, #0x11
0080f904 addeq fp, r4, #0x180
0080f908 str r3, [r4, #0x184]
0080f90c str r1, [r4, #0x194]
0080f910 str r0, [r4, #0x180]
0080f914 str r1, [r4, #0x190]
0080f918 str r2, [r4, #0x198]
0080f91c strb r2, [r4, #0x19c]
0080f920 streq fp, [sp, #4]
0080f924 beq #0x80f93c
0080f928 add r3, r4, #0x180
0080f92c str r3, [sp, #4]
0080f930 str r2, [r4, #0x1a0]
0080f934 ldr r0, [sp, #4]
0080f938 bl #0x814f84
0080f93c ldr r3, [r4, #0x1c8]
0080f940 ldr r0, [r5, r8]
0080f944 ldr r1, [r5, r6]
0080f948 cmp r3, #0
0080f94c add r0, r0, #8
0080f950 mov sl, #0
0080f954 mov fp, #0
0080f958 mov ip, #0x1b0
0080f95c strd sl, fp, [r4, ip]
0080f960 mvn r2, #0
0080f964 mov r3, #0
0080f968 str r0, [r4, #0x180]
0080f96c add r1, r1, #8
0080f970 mov r0, #0x10
0080f974 addeq r8, r4, #0x1a8
0080f978 str r0, [r4, #0x1ac]
0080f97c str r2, [r4, #0x1bc]
0080f980 str r1, [r4, #0x1a8]
0080f984 str r2, [r4, #0x1b8]
0080f988 str r3, [r4, #0x1c0]
0080f98c strb r3, [r4, #0x1c4]
0080f990 streq r8, [sp, #0x14]
0080f994 beq #0x80f9ac
0080f998 add sb, r4, #0x1a8
0080f99c str sb, [sp, #0x14]
0080f9a0 str r3, [r4, #0x1c8]
0080f9a4 ldr r0, [sp, #0x14]
0080f9a8 bl #0x814f84
0080f9ac ldr r0, [pc, #0x264]
0080f9b0 ldr r8, [pc, #0x264]
0080f9b4 ldr r3, [r4, #0x1f0]
0080f9b8 ldr r0, [r5, r0]
0080f9bc ldr r1, [r5, r8]
0080f9c0 cmp r3, #0
0080f9c4 mov fp, #0
0080f9c8 add r0, r0, #8
0080f9cc mov ip, #0x1d8
0080f9d0 mov sl, #0
0080f9d4 strd sl, fp, [r4, ip]
0080f9d8 mvn r2, #0
0080f9dc mov r3, #0
0080f9e0 str r0, [r4, #0x1a8]
0080f9e4 add r1, r1, #8
0080f9e8 mov r0, #8
0080f9ec addeq fp, r4, #0x1d0
0080f9f0 str r0, [r4, #0x1d4]
0080f9f4 str r2, [r4, #0x1e4]
0080f9f8 str r1, [r4, #0x1d0]
0080f9fc str r2, [r4, #0x1e0]
0080fa00 str r3, [r4, #0x1e8]
0080fa04 strb r3, [r4, #0x1ec]
0080fa08 streq fp, [sp, #0x1c]
0080fa0c beq #0x80fa24
0080fa10 add r2, r4, #0x1d0
0080fa14 str r2, [sp, #0x1c]
0080fa18 str r3, [r4, #0x1f0]
0080fa1c ldr r0, [sp, #0x1c]
0080fa20 bl #0x814f84
0080fa24 ldr r6, [pc, #0x1f4]
0080fa28 ldr r3, [r4, #0x218]
0080fa2c ldr r1, [r5, r8]
0080fa30 ldr r0, [r5, r6]
0080fa34 cmp r3, #0
0080fa38 mov fp, #0
0080fa3c add r0, r0, #8
0080fa40 mov ip, #0x200
0080fa44 mov sl, #0
0080fa48 strd sl, fp, [r4, ip]
0080fa4c mvn r2, #0
0080fa50 mov r3, #0
0080fa54 str r0, [r4, #0x1d0]
0080fa58 add r1, r1, #8
0080fa5c mov r0, #8
0080fa60 addeq fp, r4, #0x1f8
0080fa64 str r0, [r4, #0x1fc]
0080fa68 str r2, [r4, #0x20c]
0080fa6c str r1, [r4, #0x1f8]
0080fa70 str r2, [r4, #0x208]
0080fa74 str r3, [r4, #0x210]
0080fa78 strb r3, [r4, #0x214]
0080fa7c streq fp, [sp, #0x18]
0080fa80 beq #0x80fa98
0080fa84 add r2, r4, #0x1f8
0080fa88 str r2, [sp, #0x18]
0080fa8c str r3, [r4, #0x218]
0080fa90 ldr r0, [sp, #0x18]
0080fa94 bl #0x814f84
0080fa98 ldr r0, [r5, r6]
0080fa9c ldr r3, [r4, #0x240]
0080faa0 ldr r1, [r5, r8]
0080faa4 add r0, r0, #8
0080faa8 mov sl, #0
0080faac mov fp, #0
0080fab0 mov ip, #0x228
0080fab4 strd sl, fp, [r4, ip]
0080fab8 cmp r3, #0
0080fabc mvn r2, #0
0080fac0 mov r3, #0
0080fac4 add r1, r1, #8
0080fac8 str r0, [r4, #0x1f8]
0080facc mov r0, #8
0080fad0 str r0, [r4, #0x224]
0080fad4 str r2, [r4, #0x234]
0080fad8 str r1, [r4, #0x220]
0080fadc str r2, [r4, #0x230]
0080fae0 str r3, [r4, #0x238]
0080fae4 strb r3, [r4, #0x23c]
0080fae8 addeq sl, r4, #0x220
0080faec beq #0x80fb00
0080faf0 add sl, r4, #0x220
0080faf4 str r3, [r4, #0x240]
0080faf8 mov r0, sl
0080fafc bl #0x814f84
0080fb00 ldr r3, [r5, r6]
0080fb04 ldr r1, [pc, #0x118]
0080fb08 add r6, sp, #0x24
0080fb0c add r3, r3, #8
0080fb10 add r1, pc, r1
0080fb14 str r3, [r4, #0x220]
0080fb18 mov r2, r1
0080fb1c mov r0, r6
0080fb20 add r8, r4, #0x248
0080fb24 str r6, [sp, #0x34]
0080fb28 str r6, [sp, #0x38]
0080fb2c bl #0x3116e8
0080fb30 mov r0, r8
0080fb34 mov r1, r6
0080fb38 bl #0x371bec
0080fb3c ldr r0, [sp, #0x38]
0080fb40 cmp r0, r6
0080fb44 beq #0x80fb64
0080fb48 cmp r0, #0
0080fb4c beq #0x80fb64
0080fb50 ldr r1, [sp, #0x24]
0080fb54 rsb r1, r0, r1
0080fb58 cmp r1, #0x80
0080fb5c bhi #0x80fbf4
0080fb60 bl #0x8be338
0080fb64 mov r3, #0
0080fb68 str r3, [r4, #0x280]
0080fb6c ldr r1, [sp, #8]
0080fb70 mov r0, r4
0080fb74 bl #0x81324c
0080fb78 mov r0, r4
0080fb7c ldr r1, [sp, #0xc]
0080fb80 bl #0x81324c
0080fb84 mov r0, r4
0080fb88 ldr r1, [sp, #4]
0080fb8c bl #0x81324c
0080fb90 mov r0, r4
0080fb94 ldr r1, [sp, #0x14]
0080fb98 bl #0x81324c
0080fb9c mov r0, r4
0080fba0 ldr r1, [sp, #0x1c]
0080fba4 bl #0x81324c
0080fba8 mov r0, r4
0080fbac ldr r1, [sp, #0x18]
0080fbb0 bl #0x81324c
0080fbb4 mov r0, r4
0080fbb8 mov r1, sl
0080fbbc bl #0x81324c
0080fbc0 mov r0, r4
0080fbc4 mov r1, r8
0080fbc8 bl #0x81324c
0080fbcc mov r0, r4
0080fbd0 bl #0x80f27c
0080fbd4 ldr r3, [r5, r7]
0080fbd8 ldr r2, [sp, #0x3c]
0080fbdc mov r0, r4
0080fbe0 ldr r3, [r3]
0080fbe4 cmp r2, r3
0080fbe8 bne #0x80fbfc
0080fbec add sp, sp, #0x44
0080fbf0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0080fbf4 bl #0x310440
0080fbf8 b #0x80fb64
0080fbfc bl #0x30e310
0080fc00 andseq r5, r8, ip, asr #5
0080fc04 andeq r4, r0, ip, lsr #1
0080fc08 andeq r2, r0, r4, lsl #19
0080fc0c muleq r0, r4, r4
0080fc10 andeq r3, r0, ip, lsr #16
0080fc14 andeq r1, r0, r0, asr r5
0080fc18 andeq r3, r0, ip, lsr r5
0080fc1c andeq r4, r0, r8, rrx
0080fc20 andeq r1, r0, r4, lsr #32
0080fc24 strdeq fp, ip, [fp], -r8
