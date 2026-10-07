
SOURCE 0x466384 _ZN14PlayerSavegame23SG_UnlockAllFastTravelsEv
00466384 push {r4, r5, r6, r7, r8, sb, sl, lr}
00466388 ldr r5, [pc, #0x60]
0046638c ldr r7, [pc, #0x60]
00466390 mov sb, r0
00466394 add r5, pc, r5
00466398 ldr sl, [r5, r7]
0046639c mov r8, #0
004663a0 ldr r3, [sl]
004663a4 cmp r3, #0
004663a8 beq #0x4663e0
004663ac add r6, sb, r8, lsl #3
004663b0 mov r1, #0
004663b4 add r6, r6, #0x17c
004663b8 mov r4, r1
004663bc mov r0, r6
004663c0 mov r2, #1
004663c4 bl #0x466330
004663c8 ldr r3, [r5, r7]
004663cc add r4, r4, #1
004663d0 mov r1, r4
004663d4 ldr r3, [r3]
004663d8 cmp r4, r3
004663dc blo #0x4663bc
004663e0 add r8, r8, #1
004663e4 cmp r8, #3
004663e8 bne #0x4663a0
004663ec pop {r4, r5, r6, r7, r8, sb, sl, pc}
004663f0 ldrsheq lr, [r2], #-0x6c
004663f4 strdeq r4, r5, [r0], -r4

SOURCE 0x469b18 _ZN14PlayerSavegame20__LoadFastTravelListEP11IStreamBasePv
00469b18 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469b1c ldr fp, [pc, #0x1a8]
00469b20 ldr r2, [pc, #0x1a8]
00469b24 sub sp, sp, #0x44
00469b28 add fp, pc, fp
00469b2c ldr r3, [fp, r2]
00469b30 ldr r6, [pc, #0x19c]
00469b34 str r2, [sp, #0x14]
00469b38 ldr r3, [r3]
00469b3c str r0, [sp, #0xc]
00469b40 mov r5, r1
00469b44 str r3, [sp, #0x3c]
00469b48 ldr r3, [pc, #0x188]
00469b4c add r6, pc, r6
00469b50 mov r8, #0
00469b54 add r3, pc, r3
00469b58 str r3, [sp, #0x10]
00469b5c add r4, sp, #0x24
00469b60 add sb, sp, #0x1c
00469b64 mov sl, #1
00469b68 mov r0, r4
00469b6c mov r1, #0x10
00469b70 str r4, [sp, #0x34]
00469b74 str r4, [sp, #0x38]
00469b78 bl #0x31167c
00469b7c ldr r3, [sp, #0x34]
00469b80 mov r7, #0
00469b84 ldr r0, [sp, #0xc]
00469b88 mov r1, r4
00469b8c strb r7, [r3]
00469b90 bl #0x461da8
00469b94 ldr r1, [sp, #0x38]
00469b98 ldr r3, [sp, #0x34]
00469b9c rsb r3, r1, r3
00469ba0 cmp r3, #0x40
00469ba4 bhi #0x469cbc
00469ba8 mvn r2, #0
00469bac cmp r3, r2
00469bb0 movhs r3, r2
00469bb4 cmp r3, #0x3f
00469bb8 str r7, [sb, #4]
00469bbc str r7, [sb]
00469bc0 bls #0x469cac
00469bc4 mov r2, #0x3f
00469bc8 mov r3, #0x40
00469bcc stmib sp, {r4, sb}
00469bd0 mov r7, #0
00469bd4 mov sb, r8
00469bd8 mov r4, r3
00469bdc mov r8, r5
00469be0 mov r5, r2
00469be4 b #0x469c08
00469be8 cmp r3, #0x30
00469bec beq #0x469bf8
00469bf0 mov r0, r6
00469bf4 bl #0x708f50
00469bf8 add r7, r7, #1
00469bfc cmp r4, r7
00469c00 bls #0x469c48
00469c04 ldr r1, [sp, #0x38]
00469c08 rsb r3, r7, r5
00469c0c ldrb r3, [r1, r3]
00469c10 cmp r3, #0x31
00469c14 bne #0x469be8
00469c18 cmp r7, #0x3f
00469c1c bhi #0x469ca0
00469c20 lsr r3, r7, #5
00469c24 add r2, sp, #0x40
00469c28 add r3, r2, r3, lsl #2
00469c2c ldr r2, [r3, #-0x24]
00469c30 and r1, r7, #0x1f
00469c34 add r7, r7, #1
00469c38 orr r2, r2, sl, lsl r1
00469c3c cmp r4, r7
00469c40 str r2, [r3, #-0x24]
00469c44 bhi #0x469c04
00469c48 mov r5, r8
00469c4c ldr r4, [sp, #4]
00469c50 mov r8, sb
00469c54 ldr sb, [sp, #8]
00469c58 ldr r3, [sp, #0x1c]
00469c5c add r8, r8, #1
00469c60 mov r0, r4
00469c64 str r3, [r5, #0x17c]
00469c68 ldr r3, [sp, #0x20]
00469c6c str r3, [r5, #0x180]
00469c70 bl #0x3139ac
00469c74 cmp r8, #3
00469c78 add r5, r5, #8
00469c7c bne #0x469b68
00469c80 ldr r2, [sp, #0x14]
00469c84 ldr r3, [fp, r2]
00469c88 ldr r2, [sp, #0x3c]
00469c8c ldr r3, [r3]
00469c90 cmp r2, r3
00469c94 bne #0x469cc8
00469c98 add sp, sp, #0x44
00469c9c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469ca0 ldr r0, [sp, #0x10]
00469ca4 bl #0x708eb0
00469ca8 b #0x469c20
00469cac cmp r3, #0
00469cb0 subne r2, r3, #1
00469cb4 bne #0x469bcc
00469cb8 b #0x469c58
00469cbc mov r0, r4
00469cc0 bl #0x3139ac
00469cc4 b #0x469c80
00469cc8 bl #0x30e310
00469ccc subseq sl, r2, r8, ror #30
00469cd0 andeq r4, r0, ip, lsr #1
00469cd4 subeq r8, r5, ip, ror r1
00469cd8 subeq r8, r5, r4, ror r1

SOURCE 0x46649c _ZN14PlayerSavegame26SG_GetFastTravelIdUnlockedEPKci
0046649c ldr r3, [pc, #0xb0]
004664a0 push {r4, r5, r6, r7, r8, sb, sl, lr}
004664a4 mov r5, r0
004664a8 ldr r0, [pc, #0xa8]
004664ac add r3, pc, r3
004664b0 mov r4, r1
004664b4 ldr r0, [r3, r0]
004664b8 mov sl, r2
004664bc ldr r7, [r0]
004664c0 cmp r7, #0
004664c4 beq #0x46653c
004664c8 ldr r2, [pc, #0x8c]
004664cc mov r6, #0
004664d0 ldr r3, [r3, r2]
004664d4 ldr r8, [r3]
004664d8 b #0x4664e8
004664dc add r6, r6, #1
004664e0 cmp r6, r7
004664e4 beq #0x46653c
004664e8 ldr r1, [r8, r6, lsl #2]
004664ec mov r0, r4
004664f0 bl #0x30e31c
004664f4 cmp r0, #0
004664f8 bne #0x4664dc
004664fc cmp r6, #0
00466500 blt #0x46653c
00466504 cmp r6, r7
00466508 bhs #0x46653c
0046650c cmp r6, #0x3f
00466510 bhi #0x466544
00466514 lsl sl, sl, #1
00466518 add sl, sl, r6, lsr #5
0046651c mov r2, #1
00466520 add r5, r5, sl, lsl #2
00466524 ldr r3, [r5, #0x17c]
00466528 and r6, r6, #0x1f
0046652c ands r3, r3, r2, lsl r6
00466530 moveq r0, #0
00466534 movne r0, #1
00466538 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0046653c mov r0, #0
00466540 pop {r4, r5, r6, r7, r8, sb, sl, pc}
00466544 ldr r0, [pc, #0x14]
00466548 add r0, pc, r0
0046654c bl #0x708eb0
00466550 b #0x466514
00466554 subseq lr, r2, r4, ror #11
00466558 strdeq r4, r5, [r0], -r4
0046655c andeq r1, r0, r4, ror #29
00466560 subeq fp, r5, r0, lsl #15

SOURCE 0x469cdc _ZN14PlayerSavegame20__SaveFastTravelListEP11IStreamBasePv
00469cdc push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469ce0 ldr r6, [pc, #0x98]
00469ce4 ldr fp, [pc, #0x98]
00469ce8 sub sp, sp, #0x24
00469cec add r6, pc, r6
00469cf0 ldr r3, [r6, fp]
00469cf4 mov r5, #0
00469cf8 mov sb, r0
00469cfc ldr r3, [r3]
00469d00 mov sl, r1
00469d04 add r4, sp, #4
00469d08 mov r8, r5
00469d0c str r3, [sp, #0x1c]
00469d10 mov r0, r4
00469d14 mov r1, #0x10
00469d18 str r4, [sp, #0x14]
00469d1c str r4, [sp, #0x18]
00469d20 bl #0x31167c
00469d24 ldr r3, [sp, #0x14]
00469d28 add r7, sl, r5, lsl #3
00469d2c add r7, r7, #0x17c
00469d30 strb r8, [r3]
00469d34 mov r0, r7
00469d38 mov r1, r4
00469d3c bl #0x4699a0
00469d40 mov r0, sb
00469d44 mov r1, r4
00469d48 bl #0x461668
00469d4c add r5, r5, #1
00469d50 mov r0, r4
00469d54 bl #0x3139ac
00469d58 cmp r5, #3
00469d5c bne #0x469d10
00469d60 ldr r3, [r6, fp]
00469d64 ldr r2, [sp, #0x1c]
00469d68 ldr r3, [r3]
00469d6c cmp r2, r3
00469d70 bne #0x469d7c
00469d74 add sp, sp, #0x24
00469d78 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469d7c bl #0x30e310
00469d80 subseq sl, r2, r4, lsr #27
00469d84 andeq r4, r0, ip, lsr #1

SOURCE 0x46954c _ZN14PlayerSavegame16_InitLevelStatesEv
0046954c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469550 ldr r7, [pc, #0x11c]
00469554 sub sp, sp, #0xc
00469558 ldr sl, [pc, #0x118]
0046955c ldr sb, [pc, #0x118]
00469560 ldr r8, [pc, #0x118]
00469564 ldr ip, [pc, #0x118]
00469568 mov r4, r0
0046956c mov r5, #0
00469570 add r7, pc, r7
00469574 ldr r6, [r4, #0x68]
00469578 cmp r6, #0
0046957c beq #0x4695a4
00469580 ldr r6, [r4, #0x74]
00469584 cmp r6, #0
00469588 beq #0x469610
0046958c add r5, r5, #1
00469590 cmp r5, #3
00469594 add r4, r4, #4
00469598 bne #0x469574
0046959c add sp, sp, #0xc
004695a0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004695a4 ldr fp, [r7, sl]
004695a8 mov r1, r6
004695ac ldr r0, [fp]
004695b0 str ip, [sp, #4]
004695b4 lsl r0, r0, #2
004695b8 bl #0x31056c
004695bc str r0, [r4, #0x68]
004695c0 ldr r3, [fp]
004695c4 ldr ip, [sp, #4]
004695c8 cmp r3, #0
004695cc beq #0x469580
004695d0 ldr r2, [r7, sb]
004695d4 mov r3, r6
004695d8 b #0x4695e0
004695dc ldr r0, [r4, #0x68]
004695e0 ldr r1, [r2]
004695e4 add r1, r1, r6
004695e8 ldr r1, [r1, #0x28]
004695ec add r6, r6, #0x48
004695f0 str r1, [r0, r3, lsl #2]
004695f4 ldr r1, [fp]
004695f8 add r3, r3, #1
004695fc cmp r1, r3
00469600 bhi #0x4695dc
00469604 ldr r6, [r4, #0x74]
00469608 cmp r6, #0
0046960c bne #0x46958c
00469610 ldr fp, [r7, r8]
00469614 mov r1, r6
00469618 ldr r0, [fp]
0046961c str ip, [sp, #4]
00469620 lsl r0, r0, #2
00469624 bl #0x31056c
00469628 str r0, [r4, #0x74]
0046962c ldr r3, [fp]
00469630 ldr ip, [sp, #4]
00469634 cmp r3, #0
00469638 beq #0x46958c
0046963c ldr r2, [r7, ip]
00469640 mov r3, r6
00469644 b #0x46964c
00469648 ldr r0, [r4, #0x74]
0046964c ldr r1, [r2]
00469650 add r1, r1, r6
00469654 ldr r1, [r1, #8]
00469658 add r6, r6, #0x14
0046965c str r1, [r0, r3, lsl #2]
00469660 ldr r1, [fp]
00469664 add r3, r3, #1
00469668 cmp r1, r3
0046966c bhi #0x469648
00469670 b #0x46958c
00469674 subseq fp, r2, r0, lsr #10
00469678 andeq r1, r0, r0, asr #17
0046967c andeq r0, r0, r4, ror r8
00469680 andeq r2, r0, r4, ror r2
00469684 ldrdeq r3, r4, [r0], -r0

SOURCE 0x467c54 _ZN14PlayerSavegame26SG_GetFastTravelIdUnlockedEii
00467c54 push {r4, r5, lr}
00467c58 ldr r3, [pc, #0x78]
00467c5c subs r4, r1, #0
00467c60 sub sp, sp, #0xc
00467c64 mov r5, r0
00467c68 add r3, pc, r3
00467c6c blt #0x467c84
00467c70 ldr r1, [pc, #0x64]
00467c74 ldr r3, [r3, r1]
00467c78 ldr r3, [r3]
00467c7c cmp r4, r3
00467c80 blo #0x467c90
00467c84 mov r0, #0
00467c88 add sp, sp, #0xc
00467c8c pop {r4, r5, pc}
00467c90 cmp r4, #0x3f
00467c94 bhi #0x467cc0
00467c98 lsl r2, r2, #1
00467c9c add r2, r2, r4, lsr #5
00467ca0 and r4, r4, #0x1f
00467ca4 add r5, r5, r2, lsl #2
00467ca8 ldr r3, [r5, #0x17c]
00467cac mov r2, #1
00467cb0 ands r3, r3, r2, lsl r4
00467cb4 moveq r0, #0
00467cb8 movne r0, #1
00467cbc b #0x467c88
00467cc0 ldr r0, [pc, #0x18]
00467cc4 str r2, [sp, #4]
00467cc8 add r0, pc, r0
00467ccc bl #0x708eb0
00467cd0 ldr r2, [sp, #4]
00467cd4 b #0x467c98
00467cd8 subseq ip, r2, r8, lsr #28
00467cdc strdeq r4, r5, [r0], -r4
00467ce0 subeq sl, r5, r0

SOURCE 0x465ae0 _ZN14PlayerSavegameC1Ev
00465ae0 ldr r3, [pc, #0x150]
00465ae4 ldr r1, [pc, #0x150]
00465ae8 push {r4, r5, r6, r7, r8, lr}
00465aec add r3, pc, r3
00465af0 ldr r1, [r3, r1]
00465af4 mov r4, r0
00465af8 mov r5, #0
00465afc add r2, r0, #0x18
00465b00 add r1, r1, #8
00465b04 mvn r6, #0
00465b08 str r1, [r0]
00465b0c sub sp, sp, #0x18
00465b10 mov r0, r2
00465b14 str r2, [r4, #0x28]
00465b18 str r2, [r4, #0x2c]
00465b1c mov r1, #0x10
00465b20 str r6, [r4, #4]
00465b24 str r5, [r4, #8]
00465b28 strb r5, [r4, #0xc]
00465b2c str r5, [r4, #0x10]
00465b30 strb r5, [r4, #0x14]
00465b34 bl #0x31167c
00465b38 ldr r3, [r4, #0x28]
00465b3c add r0, r4, #0xb8
00465b40 strb r5, [r3]
00465b44 str r6, [r4, #0x34]
00465b48 str r5, [r4, #0x30]
00465b4c str r5, [r4, #0x3c]
00465b50 str r5, [r4, #0x80]
00465b54 str r5, [r4, #0x84]
00465b58 str r5, [r4, #0x88]
00465b5c str r5, [r4, #0x8c]
00465b60 str r5, [r4, #0x90]
00465b64 bl #0x46b0a4
00465b68 add r0, r4, #0x118
00465b6c bl #0x46b0a4
00465b70 mov r3, r5
00465b74 str r5, [r4, #0x178]
00465b78 add r1, r4, #0x17c
00465b7c mov r2, r5
00465b80 str r2, [r1, r3]
00465b84 add r0, r1, r3
00465b88 add r3, r3, #8
00465b8c cmp r3, #0x18
00465b90 str r2, [r0, #4]
00465b94 bne #0x465b80
00465b98 mov r5, r2
00465b9c strb r2, [r4, #0x194]
00465ba0 add r8, r4, #0x88
00465ba4 mov r6, sp
00465ba8 mov r7, r2
00465bac mov r0, r8
00465bb0 mov r1, sp
00465bb4 str r7, [sp, #4]
00465bb8 strb r7, [sp]
00465bbc str r6, [sp, #8]
00465bc0 str r6, [sp, #0xc]
00465bc4 str r7, [sp, #0x10]
00465bc8 bl #0x465a48
00465bcc ldr r3, [sp, #0x10]
00465bd0 add r5, r5, #1
00465bd4 cmp r3, #0
00465bd8 beq #0x465be8
00465bdc mov r0, sp
00465be0 ldr r1, [sp, #4]
00465be4 bl #0x345c94
00465be8 cmp r5, #2
00465bec bne #0x465bac
00465bf0 mov r1, #0
00465bf4 mov r3, r4
00465bf8 mov r2, r1
00465bfc add r1, r1, #1
00465c00 cmp r1, #3
00465c04 str r2, [r3, #0x94]
00465c08 str r2, [r3, #0xa0]
00465c0c str r2, [r3, #0xac]
00465c10 str r2, [r3, #0x40]
00465c14 str r2, [r3, #0x68]
00465c18 str r2, [r3, #0x74]
00465c1c str r2, [r3, #0x5c]
00465c20 str r2, [r3, #0x50]
00465c24 add r3, r3, #4
00465c28 bne #0x465bfc
00465c2c mov r0, r4
00465c30 add sp, sp, #0x18
00465c34 pop {r4, r5, r6, r7, r8, pc}
00465c38 subseq lr, r2, r4, lsr #31
00465c3c strheq r4, [r0], -ip

SOURCE 0x4663f8 _ZN14PlayerSavegame26SG_SetFastTravelIdUnlockedEPKcbi
004663f8 ldr ip, [pc, #0x90]
004663fc push {r4, r5, r6, r7, r8, sb, sl, lr}
00466400 mov r5, r0
00466404 ldr r0, [pc, #0x88]
00466408 add ip, pc, ip
0046640c mov r4, r1
00466410 ldr r0, [ip, r0]
00466414 mov r7, r2
00466418 mov sb, r3
0046641c ldr r6, [r0]
00466420 cmp r6, #0
00466424 beq #0x466484
00466428 ldr r3, [pc, #0x68]
0046642c mov r8, #0
00466430 ldr r3, [ip, r3]
00466434 ldr sl, [r3]
00466438 b #0x466448
0046643c add r8, r8, #1
00466440 cmp r8, r6
00466444 beq #0x466488
00466448 ldr r1, [sl, r8, lsl #2]
0046644c mov r0, r4
00466450 bl #0x30e31c
00466454 cmp r0, #0
00466458 bne #0x46643c
0046645c cmp r8, #0
00466460 blt #0x46648c
00466464 cmp r6, r8
00466468 bls #0x466484
0046646c add r0, r5, sb, lsl #3
00466470 add r0, r0, #0x17c
00466474 mov r1, r8
00466478 mov r2, r7
0046647c pop {r4, r5, r6, r7, r8, sb, sl, lr}
00466480 b #0x466330
00466484 pop {r4, r5, r6, r7, r8, sb, sl, pc}
00466488 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0046648c pop {r4, r5, r6, r7, r8, sb, sl, pc}
00466490 subseq lr, r2, r8, lsl #13
00466494 strdeq r4, r5, [r0], -r4
00466498 andeq r1, r0, r4, ror #29

SOURCE 0x46a9cc _ZN14PlayerSavegame17__LoadLevelStatesEP11IStreamBasePv
0046a9cc push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a9d0 ldr r4, [pc, #0x264]
0046a9d4 ldr r2, [pc, #0x264]
0046a9d8 sub sp, sp, #0x4c
0046a9dc add r4, pc, r4
0046a9e0 ldr r3, [r4, r2]
0046a9e4 cmn r1, #0x68
0046a9e8 str r2, [sp, #0x1c]
0046a9ec ldr r3, [r3]
0046a9f0 str r1, [sp, #0x14]
0046a9f4 mov r5, r0
0046a9f8 str r3, [sp, #0x44]
0046a9fc beq #0x46abf8
0046aa00 cmn r1, #0x74
0046aa04 beq #0x46abf8
0046aa08 add r7, sp, #0x2c
0046aa0c mov r0, r7
0046aa10 mov r1, #0x10
0046aa14 str r7, [sp, #0x3c]
0046aa18 str r7, [sp, #0x40]
0046aa1c bl #0x31167c
0046aa20 ldr r2, [pc, #0x21c]
0046aa24 ldr r3, [pc, #0x21c]
0046aa28 str r2, [sp, #4]
0046aa2c ldr r2, [sp, #0x3c]
0046aa30 str r3, [sp, #0xc]
0046aa34 mov r3, #0
0046aa38 strb r3, [r2]
0046aa3c str r3, [sp, #0x10]
0046aa40 add r2, sp, #0x28
0046aa44 add r3, sp, #0x24
0046aa48 str r2, [sp, #0x18]
0046aa4c str r3, [sp, #8]
0046aa50 mov r0, r5
0046aa54 ldr r1, [sp, #0x18]
0046aa58 bl #0x38b758
0046aa5c ldr r3, [sp, #0x28]
0046aa60 cmp r3, #0
0046aa64 ble #0x46aafc
0046aa68 mov r6, #0
0046aa6c mov r0, r5
0046aa70 mov r1, r7
0046aa74 bl #0x461da8
0046aa78 ldr r2, [sp, #4]
0046aa7c ldr sb, [sp, #0x40]
0046aa80 ldr r3, [r4, r2]
0046aa84 ldr sl, [r3]
0046aa88 cmp sl, #0
0046aa8c beq #0x46ac28
0046aa90 ldr r2, [sp, #0xc]
0046aa94 mov r8, #0
0046aa98 ldr r3, [r4, r2]
0046aa9c ldr fp, [r3]
0046aaa0 b #0x46aab0
0046aaa4 add r8, r8, #1
0046aaa8 cmp r8, sl
0046aaac beq #0x46ac28
0046aab0 mov r0, sb
0046aab4 ldr r1, [fp, r8, lsl #2]
0046aab8 bl #0x30e31c
0046aabc cmp r0, #0
0046aac0 bne #0x46aaa4
0046aac4 mov r0, r5
0046aac8 ldr r1, [sp, #8]
0046aacc bl #0x38b758
0046aad0 cmn r8, #1
0046aad4 beq #0x46aaec
0046aad8 mov r1, r8
0046aadc ldr r0, [sp, #0x14]
0046aae0 ldr r2, [sp, #0x24]
0046aae4 ldr r3, [sp, #0x10]
0046aae8 bl #0x466e48
0046aaec ldr r3, [sp, #0x28]
0046aaf0 add r6, r6, #1
0046aaf4 cmp r3, r6
0046aaf8 bgt #0x46aa6c
0046aafc ldr r3, [sp, #0x10]
0046ab00 add r3, r3, #1
0046ab04 cmp r3, #3
0046ab08 str r3, [sp, #0x10]
0046ab0c bne #0x46aa50
0046ab10 ldr r2, [pc, #0x134]
0046ab14 ldr r3, [pc, #0x134]
0046ab18 str r2, [sp, #8]
0046ab1c str r3, [sp, #0xc]
0046ab20 mov r2, #0
0046ab24 add r3, sp, #0x24
0046ab28 str r2, [sp, #0x10]
0046ab2c str r3, [sp, #4]
0046ab30 mov r0, r5
0046ab34 ldr r1, [sp, #0x18]
0046ab38 bl #0x38b758
0046ab3c ldr r3, [sp, #0x28]
0046ab40 cmp r3, #0
0046ab44 ble #0x46abdc
0046ab48 mov r6, #0
0046ab4c mov r0, r5
0046ab50 mov r1, r7
0046ab54 bl #0x461da8
0046ab58 ldr r2, [sp, #8]
0046ab5c ldr sb, [sp, #0x40]
0046ab60 ldr r3, [r4, r2]
0046ab64 ldr sl, [r3]
0046ab68 cmp sl, #0
0046ab6c beq #0x46ac18
0046ab70 ldr r2, [sp, #0xc]
0046ab74 mov r8, #0
0046ab78 ldr r3, [r4, r2]
0046ab7c ldr fp, [r3]
0046ab80 b #0x46ab90
0046ab84 add r8, r8, #1
0046ab88 cmp r8, sl
0046ab8c beq #0x46ac18
0046ab90 mov r0, sb
0046ab94 ldr r1, [fp, r8, lsl #2]
0046ab98 bl #0x30e31c
0046ab9c cmp r0, #0
0046aba0 bne #0x46ab84
0046aba4 mov r0, r5
0046aba8 ldr r1, [sp, #4]
0046abac bl #0x38b758
0046abb0 cmn r8, #1
0046abb4 beq #0x46abcc
0046abb8 mov r1, r8
0046abbc ldr r0, [sp, #0x14]
0046abc0 ldr r2, [sp, #0x24]
0046abc4 ldr r3, [sp, #0x10]
0046abc8 bl #0x466b18
0046abcc ldr r3, [sp, #0x28]
0046abd0 add r6, r6, #1
0046abd4 cmp r3, r6
0046abd8 bgt #0x46ab4c
0046abdc ldr r3, [sp, #0x10]
0046abe0 add r3, r3, #1
0046abe4 cmp r3, #3
0046abe8 str r3, [sp, #0x10]
0046abec bne #0x46ab30
0046abf0 mov r0, r7
0046abf4 bl #0x3139ac
0046abf8 ldr r2, [sp, #0x1c]
0046abfc ldr r3, [r4, r2]
0046ac00 ldr r2, [sp, #0x44]
0046ac04 ldr r3, [r3]
0046ac08 cmp r2, r3
0046ac0c bne #0x46ac38
0046ac10 add sp, sp, #0x4c
0046ac14 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046ac18 mov r0, r5
0046ac1c ldr r1, [sp, #4]
0046ac20 bl #0x38b758
0046ac24 b #0x46abcc
0046ac28 mov r0, r5
0046ac2c ldr r1, [sp, #8]
0046ac30 bl #0x38b758
0046ac34 b #0x46aaec
0046ac38 bl #0x30e310
0046ac3c ldrheq sl, [r2], #-4
0046ac40 andeq r4, r0, ip, lsr #1
0046ac44 andeq r1, r0, r0, asr #17
0046ac48 andeq r3, r0, ip, asr fp
0046ac4c andeq r2, r0, r4, ror r2
0046ac50 andeq r2, r0, r8, ror #6
