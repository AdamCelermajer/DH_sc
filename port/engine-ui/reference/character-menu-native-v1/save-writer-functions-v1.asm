0x3bc4a8 _ZN9Character7SG_SaveEv
003bc4a8: movw r3, #0x14e8
003bc4ac: ldr r0, [r0, r3]
003bc4b0: cmp r0, #0
003bc4b4: bxeq lr
003bc4b8: b #0x464b2c

0x468f18 _ZN14PlayerSavegame13__SaveFaeriesEP11IStreamBasePv
00468f18: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00468f1c: ldr r3, [pc, #0x134]
00468f20: sub sp, sp, #0x2c
00468f24: ldr sl, [pc, #0x130]
00468f28: str r3, [sp, #0xc]
00468f2c: ldr r3, [pc, #0x12c]
00468f30: mov r7, r1
00468f34: mov r5, r0
00468f38: str r3, [sp, #0x10]
00468f3c: ldr r3, [pc, #0x120]
00468f40: mov r4, r1
00468f44: mov r6, #0
00468f48: add r3, pc, r3
00468f4c: str r3, [sp, #0x14]
00468f50: ldr r3, [pc, #0x110]
00468f54: add r8, sp, #0x24
00468f58: add sl, pc, sl
00468f5c: add r3, pc, r3
00468f60: str r3, [sp, #0x18]
00468f64: ldr r3, [pc, #0x100]
00468f68: add r3, pc, r3
00468f6c: str r3, [sp, #0x1c]
00468f70: ldr r3, [r4, #0x94]
00468f74: cmp r3, #0
00468f78: beq #0x469000
00468f7c: add r1, r7, r6, lsl #2
00468f80: add r1, r1, #0xac
00468f84: mov r0, r5
00468f88: bl #0x38b808
00468f8c: ldr r3, [r4, #0xa0]
00468f90: mov r0, r5
00468f94: mov r1, r8
00468f98: str r3, [sp, #0x24]
00468f9c: bl #0x461770
00468fa0: ldr r3, [sp, #0x24]
00468fa4: cmp r3, #0
00468fa8: beq #0x468fe8
00468fac: mov sb, #0
00468fb0: ldr r1, [r4, #0x94]
00468fb4: lsl fp, sb, #2
00468fb8: mov r0, r5
00468fbc: add r1, r1, fp
00468fc0: add r1, r1, #2
00468fc4: bl #0x468db8
00468fc8: ldr r1, [r4, #0x94]
00468fcc: mov r0, r5
00468fd0: add sb, sb, #1
00468fd4: add r1, r1, fp
00468fd8: bl #0x468e68
00468fdc: ldr r3, [sp, #0x24]
00468fe0: cmp r3, sb
00468fe4: bhi #0x468fb0
00468fe8: add r6, r6, #1
00468fec: cmp r6, #3
00468ff0: add r4, r4, #4
00468ff4: bne #0x468f70
00468ff8: add sp, sp, #0x2c
00468ffc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469000: ldr r1, [sp, #0xc]
00469004: ldr r2, [sl, r1]
00469008: ldr r2, [r2]
0046900c: cmp r2, #2
00469010: beq #0x469050
00469014: cmp r2, #1
00469018: bne #0x468ff8
0046901c: ldr r3, [sp, #0x10]
00469020: mov ip, #0x22c
00469024: ldr r1, [sp, #0x14]
00469028: ldr r0, [sl, r3]
0046902c: ldr r2, [sp, #0x18]
00469030: ldr r3, [sp, #0x1c]
00469034: add r0, r0, #0xa8
00469038: str ip, [sp]
0046903c: bl #0x30e004
00469040: ldr r3, [r4, #0x94]
00469044: cmp r3, #0
00469048: bne #0x468f7c
0046904c: b #0x468ff8
00469050: str r3, [r3]
00469054: b #0x468ff8
00469058: andeq r3, r0, r0, asr #19
0046905c: subseq fp, r2, r8, lsr fp
00469060: andeq r1, r0, r0, asr #19
00469064: umaaleq r5, r5, r0, r4
00469068: subeq r4, r6, r4, lsr #11
0046906c: subeq r4, r6, r0, asr #10

0x4656bc _ZN14PlayerSavegameC2Ejib
004656bc: ldr r3, [pc, #0x100]
004656c0: push {r4, r5, r6, lr}
004656c4: ldr lr, [pc, #0xfc]
004656c8: add r3, pc, r3
004656cc: mov r4, r0
004656d0: ldr lr, [r3, lr]
004656d4: mov r5, #0
004656d8: add ip, r0, #0x18
004656dc: add lr, lr, #8
004656e0: str lr, [r0]
004656e4: str r1, [r0, #4]
004656e8: mov r0, ip
004656ec: str ip, [r4, #0x28]
004656f0: str ip, [r4, #0x2c]
004656f4: mov r1, #0x10
004656f8: str r5, [r4, #8]
004656fc: strb r5, [r4, #0xc]
00465700: str r5, [r4, #0x10]
00465704: strb r5, [r4, #0x14]
00465708: mov r6, r2
0046570c: bl #0x31167c
00465710: ldr r3, [r4, #0x28]
00465714: add r0, r4, #0xb8
00465718: strb r5, [r3]
0046571c: mov r3, #1
00465720: str r3, [r4, #0x30]
00465724: mvn r3, #0
00465728: str r3, [r4, #0x34]
0046572c: str r5, [r4, #0x3c]
00465730: str r5, [r4, #0x80]
00465734: str r5, [r4, #0x84]
00465738: str r5, [r4, #0x88]
0046573c: str r5, [r4, #0x8c]
00465740: str r5, [r4, #0x90]
00465744: bl #0x46b0a4
00465748: add r0, r4, #0x118
0046574c: bl #0x46b0a4
00465750: add r2, r4, #0x17c
00465754: mov r3, r5
00465758: str r3, [r2, r5]
0046575c: add r1, r2, r5
00465760: add r5, r5, #8
00465764: cmp r5, #0x18
00465768: str r3, [r1, #4]
0046576c: bne #0x465758
00465770: mov r1, r3
00465774: strb r3, [r4, #0x194]
00465778: mov r2, r1
0046577c: mov r3, r4
00465780: add r1, r1, #1
00465784: cmp r1, #3
00465788: str r2, [r3, #0x94]
0046578c: str r2, [r3, #0xa0]
00465790: str r2, [r3, #0xac]
00465794: str r2, [r3, #0x40]
00465798: str r2, [r3, #0x68]
0046579c: str r2, [r3, #0x74]
004657a0: str r2, [r3, #0x5c]
004657a4: str r2, [r3, #0x50]
004657a8: add r3, r3, #4
004657ac: bne #0x465780
004657b0: mov r0, r4
004657b4: mov r1, r6
004657b8: bl #0x465430
004657bc: mov r0, r4
004657c0: pop {r4, r5, r6, pc}
004657c4: subseq pc, r2, r8, asr #7
004657c8: strheq r4, [r0], -ip

0x4655ac _ZN14PlayerSavegameC1Ejib
004655ac: ldr r3, [pc, #0x100]
004655b0: push {r4, r5, r6, lr}
004655b4: ldr lr, [pc, #0xfc]
004655b8: add r3, pc, r3
004655bc: mov r4, r0
004655c0: ldr lr, [r3, lr]
004655c4: mov r5, #0
004655c8: add ip, r0, #0x18
004655cc: add lr, lr, #8
004655d0: str lr, [r0]
004655d4: str r1, [r0, #4]
004655d8: mov r0, ip
004655dc: str ip, [r4, #0x28]
004655e0: str ip, [r4, #0x2c]
004655e4: mov r1, #0x10
004655e8: str r5, [r4, #8]
004655ec: strb r5, [r4, #0xc]
004655f0: str r5, [r4, #0x10]
004655f4: strb r5, [r4, #0x14]
004655f8: mov r6, r2
004655fc: bl #0x31167c
00465600: ldr r3, [r4, #0x28]
00465604: add r0, r4, #0xb8
00465608: strb r5, [r3]
0046560c: mov r3, #1
00465610: str r3, [r4, #0x30]
00465614: mvn r3, #0
00465618: str r3, [r4, #0x34]
0046561c: str r5, [r4, #0x3c]
00465620: str r5, [r4, #0x80]
00465624: str r5, [r4, #0x84]
00465628: str r5, [r4, #0x88]
0046562c: str r5, [r4, #0x8c]
00465630: str r5, [r4, #0x90]
00465634: bl #0x46b0a4
00465638: add r0, r4, #0x118
0046563c: bl #0x46b0a4
00465640: add r2, r4, #0x17c
00465644: mov r3, r5
00465648: str r3, [r2, r5]
0046564c: add r1, r2, r5
00465650: add r5, r5, #8
00465654: cmp r5, #0x18
00465658: str r3, [r1, #4]
0046565c: bne #0x465648
00465660: mov r1, r3
00465664: strb r3, [r4, #0x194]
00465668: mov r2, r1
0046566c: mov r3, r4
00465670: add r1, r1, #1
00465674: cmp r1, #3
00465678: str r2, [r3, #0x94]
0046567c: str r2, [r3, #0xa0]
00465680: str r2, [r3, #0xac]
00465684: str r2, [r3, #0x40]
00465688: str r2, [r3, #0x68]
0046568c: str r2, [r3, #0x74]
00465690: str r2, [r3, #0x5c]
00465694: str r2, [r3, #0x50]
00465698: add r3, r3, #4
0046569c: bne #0x465670
004656a0: mov r0, r4
004656a4: mov r1, r6
004656a8: bl #0x465430
004656ac: mov r0, r4
004656b0: pop {r4, r5, r6, pc}
004656b4: ldrsbeq pc, [r2], #-0x48
004656b8: strheq r4, [r0], -ip

0x468930 _ZN14PlayerSavegame17__SavePlayerLevelEP11IStreamBasePv
00468930: add r1, r1, #0x30
00468934: b #0x38b808

0x4688f8 _ZN14PlayerSavegame21__SaveDifficultyLevelEP11IStreamBasePv
004688f8: ldr r3, [pc, #0x28]
004688fc: ldr r2, [pc, #0x28]
00468900: push {r4, r5, r6, lr}
00468904: add r3, pc, r3
00468908: mov r4, r1
0046890c: ldr r1, [r3, r2]
00468910: mov r5, r0
00468914: bl #0x38b808
00468918: mov r0, r5
0046891c: add r1, r4, #0x3c
00468920: pop {r4, r5, r6, lr}
00468924: b #0x38b808
00468928: subseq ip, r2, ip, lsl #3
0046892c: muleq r0, ip, sl

0x469e6c _ZN14PlayerSavegame12__SaveSkillsEP11IStreamBasePv
00469e6c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469e70: ldr sb, [pc, #0x21c]
00469e74: ldr r2, [pc, #0x21c]
00469e78: sub sp, sp, #0x3c
00469e7c: add sb, pc, sb
00469e80: str r2, [sp, #0xc]
00469e84: ldr r2, [sb, r2]
00469e88: ldr r3, [r1, #0x80]
00469e8c: mov r5, r1
00469e90: ldr r2, [r2]
00469e94: cmp r3, #0
00469e98: mov r4, r0
00469e9c: str r2, [sp, #0x34]
00469ea0: beq #0x46a030
00469ea4: ldr r3, [r5, #0x84]
00469ea8: add r7, sp, #0x1c
00469eac: mov r0, r7
00469eb0: mov r1, #0x10
00469eb4: str r3, [sp, #0x18]
00469eb8: str r7, [sp, #0x2c]
00469ebc: str r7, [sp, #0x30]
00469ec0: bl #0x31167c
00469ec4: ldr r3, [sp, #0x2c]
00469ec8: mov r6, #0
00469ecc: mov r0, r4
00469ed0: strb r6, [r3]
00469ed4: add r1, sp, #0x18
00469ed8: bl #0x38b808
00469edc: ldr r3, [sp, #0x18]
00469ee0: cmp r3, r6
00469ee4: ble #0x469f4c
00469ee8: ldr r3, [pc, #0x1ac]
00469eec: ldr sl, [sb, r3]
00469ef0: ldr r2, [r5, #0x80]
00469ef4: ldr r3, [sl]
00469ef8: lsl r8, r6, #3
00469efc: ldr r2, [r2, r6, lsl #3]
00469f00: add r6, r6, #1
00469f04: ldr fp, [r3, r2, lsl #2]
00469f08: mov r0, fp
00469f0c: bl #0x30de54
00469f10: mov r1, fp
00469f14: add r2, fp, r0
00469f18: mov r0, r7
00469f1c: bl #0x3109e0
00469f20: mov r0, r4
00469f24: mov r1, r7
00469f28: bl #0x461668
00469f2c: ldr r1, [r5, #0x80]
00469f30: mov r0, r4
00469f34: add r1, r1, r8
00469f38: add r1, r1, #4
00469f3c: bl #0x468db8
00469f40: ldr r3, [sp, #0x18]
00469f44: cmp r3, r6
00469f48: bgt #0x469ef0
00469f4c: mov sl, #0
00469f50: add fp, sp, #0x14
00469f54: ldr r3, [r5, #0x88]
00469f58: mov r0, r4
00469f5c: mov r1, fp
00469f60: add r3, r3, sl
00469f64: ldr r3, [r3, #0x10]
00469f68: str r3, [sp, #0x14]
00469f6c: bl #0x461770
00469f70: ldr r8, [r5, #0x88]
00469f74: add r8, r8, sl
00469f78: ldr r6, [r8, #8]
00469f7c: cmp r6, r8
00469f80: beq #0x469fc8
00469f84: add r1, r6, #0x10
00469f88: mov r0, r4
00469f8c: bl #0x38b808
00469f90: mov r0, r4
00469f94: add r1, r6, #0x14
00469f98: bl #0x38b808
00469f9c: ldr r2, [r6, #0xc]
00469fa0: cmp r2, #0
00469fa4: bne #0x469fb0
00469fa8: b #0x469ffc
00469fac: mov r2, r3
00469fb0: ldr r3, [r2, #8]
00469fb4: cmp r3, #0
00469fb8: bne #0x469fac
00469fbc: mov r6, r2
00469fc0: cmp r8, r6
00469fc4: bne #0x469f84
00469fc8: add sl, sl, #0x18
00469fcc: cmp sl, #0x30
00469fd0: bne #0x469f54
00469fd4: mov r0, r7
00469fd8: bl #0x3139ac
00469fdc: ldr r2, [sp, #0xc]
00469fe0: ldr r3, [sb, r2]
00469fe4: ldr r2, [sp, #0x34]
00469fe8: ldr r3, [r3]
00469fec: cmp r2, r3
00469ff0: bne #0x46a090
00469ff4: add sp, sp, #0x3c
00469ff8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469ffc: ldr r3, [r6, #4]
0046a000: ldr r1, [r3, #0xc]
0046a004: cmp r6, r1
0046a008: bne #0x46a024
0046a00c: mov r6, r3
0046a010: ldr r3, [r3, #4]
0046a014: ldr r2, [r3, #0xc]
0046a018: cmp r2, r6
0046a01c: beq #0x46a00c
0046a020: ldr r2, [r6, #0xc]
0046a024: cmp r3, r2
0046a028: movne r6, r3
0046a02c: b #0x469fc0
0046a030: ldr r2, [pc, #0x68]
0046a034: ldr r2, [sb, r2]
0046a038: ldr r2, [r2]
0046a03c: cmp r2, #2
0046a040: streq r3, [r3]
0046a044: beq #0x469fdc
0046a048: cmp r2, #1
0046a04c: bne #0x469fdc
0046a050: ldr r0, [pc, #0x4c]
0046a054: ldr r1, [pc, #0x4c]
0046a058: ldr r2, [pc, #0x4c]
0046a05c: ldr r0, [sb, r0]
0046a060: ldr r3, [pc, #0x48]
0046a064: movw ip, #0x1cf
0046a068: add r1, pc, r1
0046a06c: add r3, pc, r3
0046a070: add r0, r0, #0xa8
0046a074: add r2, pc, r2
0046a078: str ip, [sp]
0046a07c: bl #0x30e004
0046a080: ldr r3, [r5, #0x80]
0046a084: cmp r3, #0
0046a088: bne #0x469ea4
0046a08c: b #0x469fdc
0046a090: bl #0x30e310
0046a094: subseq sl, r2, r4, lsl ip
0046a098: andeq r4, r0, ip, lsr #1
0046a09c: ldrdeq r3, r4, [r0], -r8
0046a0a0: andeq r3, r0, r0, asr #19
0046a0a4: andeq r1, r0, r0, asr #19
0046a0a8: subeq r4, r5, r0, ror r3
0046a0ac: subeq r3, r6, r4, lsr #9
0046a0b0: subeq r3, r6, ip, lsr r4

0x4688c0 _ZN14PlayerSavegame16__SavePlayerNameEP11IStreamBasePv
004688c0: add r1, r1, #0x18
004688c4: b #0x461668

0x469cdc _ZN14PlayerSavegame20__SaveFastTravelListEP11IStreamBasePv
00469cdc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469ce0: ldr r6, [pc, #0x98]
00469ce4: ldr fp, [pc, #0x98]
00469ce8: sub sp, sp, #0x24
00469cec: add r6, pc, r6
00469cf0: ldr r3, [r6, fp]
00469cf4: mov r5, #0
00469cf8: mov sb, r0
00469cfc: ldr r3, [r3]
00469d00: mov sl, r1
00469d04: add r4, sp, #4
00469d08: mov r8, r5
00469d0c: str r3, [sp, #0x1c]
00469d10: mov r0, r4
00469d14: mov r1, #0x10
00469d18: str r4, [sp, #0x14]
00469d1c: str r4, [sp, #0x18]
00469d20: bl #0x31167c
00469d24: ldr r3, [sp, #0x14]
00469d28: add r7, sl, r5, lsl #3
00469d2c: add r7, r7, #0x17c
00469d30: strb r8, [r3]
00469d34: mov r0, r7
00469d38: mov r1, r4
00469d3c: bl #0x4699a0
00469d40: mov r0, sb
00469d44: mov r1, r4
00469d48: bl #0x461668
00469d4c: add r5, r5, #1
00469d50: mov r0, r4
00469d54: bl #0x3139ac
00469d58: cmp r5, #3
00469d5c: bne #0x469d10
00469d60: ldr r3, [r6, fp]
00469d64: ldr r2, [sp, #0x1c]
00469d68: ldr r3, [r3]
00469d6c: cmp r2, r3
00469d70: bne #0x469d7c
00469d74: add sp, sp, #0x24
00469d78: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469d7c: bl #0x30e310
00469d80: subseq sl, r2, r4, lsr #27
00469d84: andeq r4, r0, ip, lsr #1

0x4689a8 _ZN14PlayerSavegame19__SaveUseSpawnPointEP11IStreamBasePv
004689a8: push {r4, r5, r6, lr}
004689ac: mov r4, r1
004689b0: mov r5, r0
004689b4: add r1, r1, #0x4c
004689b8: bl #0x33e138
004689bc: mov r0, r5
004689c0: add r1, r4, #0x4d
004689c4: bl #0x33e138
004689c8: mov r0, r5
004689cc: add r1, r4, #0x4e
004689d0: pop {r4, r5, r6, lr}
004689d4: b #0x33e138

0x4688c8 _ZN14PlayerSavegame21__SaveLevelEntryPointEP11IStreamBasePv
004688c8: push {r4, r5, r6, lr}
004688cc: mov r4, r1
004688d0: mov r5, r0
004688d4: add r1, r1, #0x40
004688d8: bl #0x38b808
004688dc: mov r0, r5
004688e0: add r1, r4, #0x44
004688e4: bl #0x38b808
004688e8: mov r0, r5
004688ec: add r1, r4, #0x48
004688f0: pop {r4, r5, r6, lr}
004688f4: b #0x38b808

0x465c40 _ZN14PlayerSavegameC2Ev
00465c40: ldr r3, [pc, #0x150]
00465c44: ldr r1, [pc, #0x150]
00465c48: push {r4, r5, r6, r7, r8, lr}
00465c4c: add r3, pc, r3
00465c50: ldr r1, [r3, r1]
00465c54: mov r4, r0
00465c58: mov r5, #0
00465c5c: add r2, r0, #0x18
00465c60: add r1, r1, #8
00465c64: mvn r6, #0
00465c68: str r1, [r0]
00465c6c: sub sp, sp, #0x18
00465c70: mov r0, r2
00465c74: str r2, [r4, #0x28]
00465c78: str r2, [r4, #0x2c]
00465c7c: mov r1, #0x10
00465c80: str r6, [r4, #4]
00465c84: str r5, [r4, #8]
00465c88: strb r5, [r4, #0xc]
00465c8c: str r5, [r4, #0x10]
00465c90: strb r5, [r4, #0x14]
00465c94: bl #0x31167c
00465c98: ldr r3, [r4, #0x28]
00465c9c: add r0, r4, #0xb8
00465ca0: strb r5, [r3]
00465ca4: str r6, [r4, #0x34]
00465ca8: str r5, [r4, #0x30]
00465cac: str r5, [r4, #0x3c]
00465cb0: str r5, [r4, #0x80]
00465cb4: str r5, [r4, #0x84]
00465cb8: str r5, [r4, #0x88]
00465cbc: str r5, [r4, #0x8c]
00465cc0: str r5, [r4, #0x90]
00465cc4: bl #0x46b0a4
00465cc8: add r0, r4, #0x118
00465ccc: bl #0x46b0a4
00465cd0: mov r3, r5
00465cd4: str r5, [r4, #0x178]
00465cd8: add r1, r4, #0x17c
00465cdc: mov r2, r5
00465ce0: str r2, [r1, r3]
00465ce4: add r0, r1, r3
00465ce8: add r3, r3, #8
00465cec: cmp r3, #0x18
00465cf0: str r2, [r0, #4]
00465cf4: bne #0x465ce0
00465cf8: mov r5, r2
00465cfc: strb r2, [r4, #0x194]
00465d00: add r8, r4, #0x88
00465d04: mov r6, sp
00465d08: mov r7, r2
00465d0c: mov r0, r8
00465d10: mov r1, sp
00465d14: str r7, [sp, #4]
00465d18: strb r7, [sp]
00465d1c: str r6, [sp, #8]
00465d20: str r6, [sp, #0xc]
00465d24: str r7, [sp, #0x10]
00465d28: bl #0x465a48
00465d2c: ldr r3, [sp, #0x10]
00465d30: add r5, r5, #1
00465d34: cmp r3, #0
00465d38: beq #0x465d48
00465d3c: mov r0, sp
00465d40: ldr r1, [sp, #4]
00465d44: bl #0x345c94
00465d48: cmp r5, #2
00465d4c: bne #0x465d0c
00465d50: mov r1, #0
00465d54: mov r3, r4
00465d58: mov r2, r1
00465d5c: add r1, r1, #1
00465d60: cmp r1, #3
00465d64: str r2, [r3, #0x94]
00465d68: str r2, [r3, #0xa0]
00465d6c: str r2, [r3, #0xac]
00465d70: str r2, [r3, #0x40]
00465d74: str r2, [r3, #0x68]
00465d78: str r2, [r3, #0x74]
00465d7c: str r2, [r3, #0x5c]
00465d80: str r2, [r3, #0x50]
00465d84: add r3, r3, #4
00465d88: bne #0x465d5c
00465d8c: mov r0, r4
00465d90: add sp, sp, #0x18
00465d94: pop {r4, r5, r6, r7, r8, pc}
00465d98: subseq lr, r2, r4, asr #28
00465d9c: strheq r4, [r0], -ip

0x468bf0 _ZN14PlayerSavegame18__SaveCurrentFaeryEP11IStreamBasePv
00468bf0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00468bf4: ldr r3, [pc, #0xc0]
00468bf8: sub sp, sp, #0x14
00468bfc: ldr r8, [pc, #0xbc]
00468c00: add r3, pc, r3
00468c04: str r3, [sp, #8]
00468c08: ldr r3, [pc, #0xb4]
00468c0c: ldr fp, [pc, #0xb4]
00468c10: ldr sl, [pc, #0xb4]
00468c14: add r3, pc, r3
00468c18: ldr sb, [pc, #0xb0]
00468c1c: mov r6, r1
00468c20: mov r7, r0
00468c24: add fp, pc, fp
00468c28: str r3, [sp, #0xc]
00468c2c: mov r5, r1
00468c30: mov r4, #0
00468c34: add r8, pc, r8
00468c38: ldr r3, [r5, #0x94]
00468c3c: cmp r3, #0
00468c40: beq #0x468c6c
00468c44: add r1, r6, r4, lsl #2
00468c48: add r1, r1, #0xac
00468c4c: add r4, r4, #1
00468c50: mov r0, r7
00468c54: bl #0x38b808
00468c58: cmp r4, #3
00468c5c: add r5, r5, #4
00468c60: bne #0x468c38
00468c64: add sp, sp, #0x14
00468c68: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00468c6c: ldr r2, [r8, sl]
00468c70: ldr r2, [r2]
00468c74: cmp r2, #2
00468c78: beq #0x468cb4
00468c7c: cmp r2, #1
00468c80: bne #0x468c64
00468c84: ldr r0, [r8, sb]
00468c88: ldr r3, [sp, #0xc]
00468c8c: mov ip, #0x254
00468c90: mov r1, fp
00468c94: ldr r2, [sp, #8]
00468c98: add r0, r0, #0xa8
00468c9c: str ip, [sp]
00468ca0: bl #0x30e004
00468ca4: ldr r3, [r5, #0x94]
00468ca8: cmp r3, #0
00468cac: bne #0x468c44
00468cb0: b #0x468c64
00468cb4: str r3, [r3]
00468cb8: b #0x468c64
00468cbc: subeq r4, r6, r0, lsl #18
00468cc0: subseq fp, r2, ip, asr lr
00468cc4: umaaleq r4, r6, r4, r8
00468cc8: strheq r5, [r5], #-0x74
00468ccc: andeq r3, r0, r0, asr #19
00468cd0: andeq r1, r0, r0, asr #19

0x4698e4 _ZN14PlayerSavegame17__SavePlayerClassEP11IStreamBasePv
004698e4: push {r4, r5, r6, r7, r8, lr}
004698e8: ldr r4, [pc, #0xa0]
004698ec: ldr r5, [pc, #0xa0]
004698f0: sub sp, sp, #0x20
004698f4: add r4, pc, r4
004698f8: ldr r3, [r4, r5]
004698fc: mov r7, r0
00469900: ldr r3, [r3]
00469904: str r3, [sp, #0x1c]
00469908: ldr r3, [r1, #0x34]
0046990c: cmp r3, #0
00469910: blt #0x469970
00469914: ldr r2, [pc, #0x7c]
00469918: ldr r2, [r4, r2]
0046991c: ldr r2, [r2]
00469920: cmp r3, r2
00469924: bhi #0x469970
00469928: ldr r2, [pc, #0x6c]
0046992c: add r6, sp, #4
00469930: ldr r2, [r4, r2]
00469934: ldr r2, [r2]
00469938: ldr r8, [r2, r3, lsl #2]
0046993c: str r6, [sp, #0x14]
00469940: str r6, [sp, #0x18]
00469944: mov r0, r8
00469948: bl #0x30de54
0046994c: mov r1, r8
00469950: add r2, r8, r0
00469954: mov r0, r6
00469958: bl #0x3116e8
0046995c: mov r0, r7
00469960: mov r1, r6
00469964: bl #0x461668
00469968: mov r0, r6
0046996c: bl #0x3139ac
00469970: ldr r3, [r4, r5]
00469974: ldr r2, [sp, #0x1c]
00469978: ldr r3, [r3]
0046997c: cmp r2, r3
00469980: bne #0x46998c
00469984: add sp, sp, #0x20
00469988: pop {r4, r5, r6, r7, r8, pc}
0046998c: bl #0x30e310

0x469454 _ZN14PlayerSavegame12__SaveQuestsEP11IStreamBasePv
00469454: ldr r2, [r1, #0x178]
00469458: mov r3, r1
0046945c: mov r1, r0
00469460: tst r2, #1
00469464: bne #0x469470
00469468: add r0, r3, #0x118
0046946c: b #0x46c6fc
00469470: add r0, r3, #0xb8
00469474: b #0x46c6fc

0x468b20 _ZN14PlayerSavegame15__SaveLevelNameEP11IStreamBasePv
00468b20: push {r4, r5, r6, lr}
00468b24: mov r5, r1
00468b28: add r1, r1, #0x38
00468b2c: mov r6, r0
00468b30: bl #0x461770
00468b34: mov r4, #0
00468b38: add r1, r4, #0x14
00468b3c: add r1, r5, r1, lsl #2
00468b40: mov r0, r6
00468b44: bl #0x38b808
00468b48: add r1, r5, r4, lsl #2
00468b4c: add r1, r1, #0x5c
00468b50: mov r0, r6
00468b54: bl #0x38b808
00468b58: add r1, r5, r4, lsl #2
00468b5c: add r1, r1, #0xfc
00468b60: add r4, r4, #1
00468b64: mov r0, r6
00468b68: bl #0x38b808
00468b6c: cmp r4, #3
00468b70: bne #0x468b38
00468b74: pop {r4, r5, r6, pc}

0x465ae0 _ZN14PlayerSavegameC1Ev
00465ae0: ldr r3, [pc, #0x150]
00465ae4: ldr r1, [pc, #0x150]
00465ae8: push {r4, r5, r6, r7, r8, lr}
00465aec: add r3, pc, r3
00465af0: ldr r1, [r3, r1]
00465af4: mov r4, r0
00465af8: mov r5, #0
00465afc: add r2, r0, #0x18
00465b00: add r1, r1, #8
00465b04: mvn r6, #0
00465b08: str r1, [r0]
00465b0c: sub sp, sp, #0x18
00465b10: mov r0, r2
00465b14: str r2, [r4, #0x28]
00465b18: str r2, [r4, #0x2c]
00465b1c: mov r1, #0x10
00465b20: str r6, [r4, #4]
00465b24: str r5, [r4, #8]
00465b28: strb r5, [r4, #0xc]
00465b2c: str r5, [r4, #0x10]
00465b30: strb r5, [r4, #0x14]
00465b34: bl #0x31167c
00465b38: ldr r3, [r4, #0x28]
00465b3c: add r0, r4, #0xb8
00465b40: strb r5, [r3]
00465b44: str r6, [r4, #0x34]
00465b48: str r5, [r4, #0x30]
00465b4c: str r5, [r4, #0x3c]
00465b50: str r5, [r4, #0x80]
00465b54: str r5, [r4, #0x84]
00465b58: str r5, [r4, #0x88]
00465b5c: str r5, [r4, #0x8c]
00465b60: str r5, [r4, #0x90]
00465b64: bl #0x46b0a4
00465b68: add r0, r4, #0x118
00465b6c: bl #0x46b0a4
00465b70: mov r3, r5
00465b74: str r5, [r4, #0x178]
00465b78: add r1, r4, #0x17c
00465b7c: mov r2, r5
00465b80: str r2, [r1, r3]
00465b84: add r0, r1, r3
00465b88: add r3, r3, #8
00465b8c: cmp r3, #0x18
00465b90: str r2, [r0, #4]
00465b94: bne #0x465b80
00465b98: mov r5, r2
00465b9c: strb r2, [r4, #0x194]
00465ba0: add r8, r4, #0x88
00465ba4: mov r6, sp
00465ba8: mov r7, r2
00465bac: mov r0, r8
00465bb0: mov r1, sp
00465bb4: str r7, [sp, #4]
00465bb8: strb r7, [sp]
00465bbc: str r6, [sp, #8]
00465bc0: str r6, [sp, #0xc]
00465bc4: str r7, [sp, #0x10]
00465bc8: bl #0x465a48
00465bcc: ldr r3, [sp, #0x10]
00465bd0: add r5, r5, #1
00465bd4: cmp r3, #0
00465bd8: beq #0x465be8
00465bdc: mov r0, sp
00465be0: ldr r1, [sp, #4]
00465be4: bl #0x345c94
00465be8: cmp r5, #2
00465bec: bne #0x465bac
00465bf0: mov r1, #0
00465bf4: mov r3, r4
00465bf8: mov r2, r1
00465bfc: add r1, r1, #1
00465c00: cmp r1, #3
00465c04: str r2, [r3, #0x94]
00465c08: str r2, [r3, #0xa0]
00465c0c: str r2, [r3, #0xac]
00465c10: str r2, [r3, #0x40]
00465c14: str r2, [r3, #0x68]
00465c18: str r2, [r3, #0x74]
00465c1c: str r2, [r3, #0x5c]
00465c20: str r2, [r3, #0x50]
00465c24: add r3, r3, #4
00465c28: bne #0x465bfc
00465c2c: mov r0, r4
00465c30: add sp, sp, #0x18
00465c34: pop {r4, r5, r6, r7, r8, pc}
00465c38: subseq lr, r2, r4, lsr #31
00465c3c: strheq r4, [r0], -ip

0x46a0b4 _ZN14PlayerSavegame15__SaveInventoryEP11IStreamBasePv
0046a0b4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a0b8: ldr sb, [pc, #0x2bc]
0046a0bc: ldr r2, [pc, #0x2bc]
0046a0c0: sub sp, sp, #0x74
0046a0c4: add sb, pc, sb
0046a0c8: str r2, [sp, #0x2c]
0046a0cc: ldr r2, [sb, r2]
0046a0d0: ldr r3, [r1, #0x10]
0046a0d4: mov r5, r1
0046a0d8: ldr r2, [r2]
0046a0dc: cmp r3, #0
0046a0e0: mov r4, r0
0046a0e4: str r2, [sp, #0x6c]
0046a0e8: beq #0x46a318
0046a0ec: ldr r2, [r3, #0x39c]
0046a0f0: add r0, r3, #0x37c
0046a0f4: mvn r1, #0
0046a0f8: str r2, [sp, #0x4c]
0046a0fc: bl #0x3fc6a8
0046a100: ldr r3, [r5, #0x10]
0046a104: str r0, [sp, #0x48]
0046a108: add r1, sp, #0x4c
0046a10c: ldr r2, [r3, #0x384]
0046a110: ldr r3, [r3, #0x388]
0046a114: mov r0, r4
0046a118: add r6, sp, #0x54
0046a11c: rsb r3, r2, r3
0046a120: asr r3, r3, #2
0046a124: str r3, [sp, #0x44]
0046a128: bl #0x461770
0046a12c: mov r0, r4
0046a130: add r1, sp, #0x48
0046a134: bl #0x461770
0046a138: mov r0, r4
0046a13c: add r1, sp, #0x44
0046a140: bl #0x461770
0046a144: ldr r3, [r5, #0x10]
0046a148: mov r0, r6
0046a14c: mov r1, #0x10
0046a150: ldr r2, [r3, #0x388]
0046a154: str r2, [sp, #8]
0046a158: ldr sl, [r3, #0x384]
0046a15c: str r6, [sp, #0x64]
0046a160: str r6, [sp, #0x68]
0046a164: bl #0x31167c
0046a168: ldr r3, [sp, #8]
0046a16c: mov r2, #0
0046a170: cmp sl, r3
0046a174: ldr r3, [sp, #0x64]
0046a178: strb r2, [r3]
0046a17c: beq #0x46a2f0
0046a180: add r3, sp, #0x3c
0046a184: str r3, [sp, #0x1c]
0046a188: ldr r3, [pc, #0x1f4]
0046a18c: ldr r2, [pc, #0x1f4]
0046a190: ldr r3, [sb, r3]
0046a194: str r2, [sp, #0x28]
0046a198: add r2, sp, #0x38
0046a19c: str r3, [sp, #0x24]
0046a1a0: str r2, [sp, #0x18]
0046a1a4: add r3, sp, #0x34
0046a1a8: add r2, sp, #0x30
0046a1ac: str r3, [sp, #0x14]
0046a1b0: str r2, [sp, #0x10]
0046a1b4: add r3, sp, #0x53
0046a1b8: add r2, sp, #0x40
0046a1bc: str r3, [sp, #0xc]
0046a1c0: str r2, [sp, #0x20]
0046a1c4: ldr r5, [sl]
0046a1c8: ldr r3, [sp, #0x24]
0046a1cc: ldr r7, [r5]
0046a1d0: ldr r8, [r3]
0046a1d4: mov r0, r7
0046a1d8: bl #0x3f9e00
0046a1dc: ldr r8, [r8, r0, lsl #2]
0046a1e0: mov r0, r8
0046a1e4: bl #0x30de54
0046a1e8: mov r1, r8
0046a1ec: add r2, r8, r0
0046a1f0: mov r0, r6
0046a1f4: bl #0x3109e0
0046a1f8: mov r0, r4
0046a1fc: mov r1, r6
0046a200: bl #0x461668
0046a204: ldrsb r3, [r5, #4]
0046a208: mov r0, r4
0046a20c: ldr r1, [sp, #0x1c]
0046a210: str r3, [sp, #0x3c]
0046a214: bl #0x38b808
0046a218: ldrsb r3, [r5, #5]
0046a21c: mov r0, r4
0046a220: ldr r1, [sp, #0x18]
0046a224: str r3, [sp, #0x38]
0046a228: bl #0x38b808
0046a22c: ldrsh r3, [r7, #0x50]
0046a230: mov r0, r4
0046a234: ldr r1, [sp, #0x14]
0046a238: str r3, [sp, #0x34]
0046a23c: bl #0x38b808
0046a240: ldr r3, [r7, #0x54]
0046a244: mov r0, r4
0046a248: ldr r1, [sp, #0x10]
0046a24c: str r3, [sp, #0x30]
0046a250: bl #0x38b808
0046a254: ldrb r3, [r7, #0x68]
0046a258: ldr r1, [sp, #0xc]
0046a25c: mov r0, r4
0046a260: strb r3, [sp, #0x53]
0046a264: bl #0x39f828
0046a268: mov r0, r7
0046a26c: bl #0x3f9e80
0046a270: ldr r1, [sp, #0x20]
0046a274: str r0, [sp, #0x40]
0046a278: mov r0, r4
0046a27c: bl #0x461770
0046a280: ldr r3, [sp, #0x40]
0046a284: cmp r3, #0
0046a288: beq #0x46a2e0
0046a28c: ldr r2, [sp, #0x28]
0046a290: mov r5, #0
0046a294: ldr r8, [sb, r2]
0046a298: mov r1, r5
0046a29c: mov r0, r7
0046a2a0: ldr fp, [r8]
0046a2a4: bl #0x3fa038
0046a2a8: ldr fp, [fp, r0, lsl #2]
0046a2ac: add r5, r5, #1
0046a2b0: mov r0, fp
0046a2b4: bl #0x30de54
0046a2b8: mov r1, fp
0046a2bc: add r2, fp, r0
0046a2c0: mov r0, r6
0046a2c4: bl #0x3109e0
0046a2c8: mov r0, r4
0046a2cc: mov r1, r6
0046a2d0: bl #0x461668
0046a2d4: ldr r3, [sp, #0x40]
0046a2d8: cmp r3, r5
0046a2dc: bhi #0x46a298
0046a2e0: ldr r3, [sp, #8]
0046a2e4: add sl, sl, #4
0046a2e8: cmp sl, r3
0046a2ec: bne #0x46a1c4
0046a2f0: mov r0, r6
0046a2f4: bl #0x3139ac
0046a2f8: ldr r2, [sp, #0x2c]
0046a2fc: ldr r3, [sb, r2]
0046a300: ldr r2, [sp, #0x6c]
0046a304: ldr r3, [r3]
0046a308: cmp r2, r3
0046a30c: bne #0x46a378
0046a310: add sp, sp, #0x74
0046a314: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046a318: ldr r2, [pc, #0x6c]
0046a31c: ldr r2, [sb, r2]
0046a320: ldr r2, [r2]
0046a324: cmp r2, #2
0046a328: streq r3, [r3]
0046a32c: beq #0x46a2f8
0046a330: cmp r2, #1
0046a334: bne #0x46a2f8
0046a338: ldr r0, [pc, #0x50]
0046a33c: ldr r1, [pc, #0x50]
0046a340: ldr r2, [pc, #0x50]
0046a344: ldr r0, [sb, r0]
0046a348: ldr r3, [pc, #0x4c]
0046a34c: movw ip, #0x2de
0046a350: add r1, pc, r1
0046a354: add r3, pc, r3
0046a358: add r0, r0, #0xa8
0046a35c: add r2, pc, r2
0046a360: str ip, [sp]
0046a364: bl #0x30e004
0046a368: ldr r3, [r5, #0x10]
0046a36c: cmp r3, #0
0046a370: beq #0x46a2f8
0046a374: b #0x46a0ec
0046a378: bl #0x30e310
0046a37c: subseq sl, r2, ip, asr #19
0046a380: andeq r4, r0, ip, lsr #1
0046a384: andeq r1, r0, r4, asr ip
0046a388: andeq r1, r0, ip, asr #5
0046a38c: andeq r3, r0, r0, asr #19
0046a390: andeq r1, r0, r0, asr #19
0046a394: subeq r4, r5, r8, lsl #1
0046a398: subeq r3, r6, ip, lsr r1
0046a39c: subeq r3, r6, r4, asr r1

0x46a7a0 _ZN14PlayerSavegame17__SaveLevelStatesEP11IStreamBasePv
0046a7a0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a7a4: ldr r2, [pc, #0x208]
0046a7a8: ldr r3, [pc, #0x208]
0046a7ac: sub sp, sp, #0x44
0046a7b0: add r2, pc, r2
0046a7b4: str r3, [sp, #0x14]
0046a7b8: ldr r3, [r2, r3]
0046a7bc: cmn r1, #0x68
0046a7c0: str r2, [sp, #4]
0046a7c4: ldr r3, [r3]
0046a7c8: mov r7, r1
0046a7cc: mov r5, r0
0046a7d0: str r3, [sp, #0x3c]
0046a7d4: beq #0x46a98c
0046a7d8: cmn r1, #0x74
0046a7dc: beq #0x46a98c
0046a7e0: add r6, sp, #0x24
0046a7e4: mov r1, #0x10
0046a7e8: mov r0, r6
0046a7ec: str r6, [sp, #0x34]
0046a7f0: str r6, [sp, #0x38]
0046a7f4: bl #0x31167c
0046a7f8: ldr r2, [sp, #4]
0046a7fc: ldr r3, [pc, #0x1b8]
0046a800: ldr r1, [pc, #0x1b8]
0046a804: mov r8, #0
0046a808: ldr r3, [r2, r3]
0046a80c: str r1, [sp, #0xc]
0046a810: add sb, sp, #0x1c
0046a814: str r3, [sp, #8]
0046a818: ldr r3, [sp, #0x34]
0046a81c: strb r8, [r3]
0046a820: add r3, sp, #0x20
0046a824: str r3, [sp, #0x10]
0046a828: ldr r1, [sp, #8]
0046a82c: mov r0, r5
0046a830: ldr r3, [r1]
0046a834: ldr r1, [sp, #0x10]
0046a838: str r3, [sp, #0x20]
0046a83c: bl #0x38b808
0046a840: ldr r3, [sp, #0x20]
0046a844: cmp r3, #0
0046a848: ble #0x46a8b8
0046a84c: ldr r3, [sp, #4]
0046a850: ldr r2, [sp, #0xc]
0046a854: mov r4, #0
0046a858: ldr sl, [r3, r2]
0046a85c: ldr r3, [sl]
0046a860: ldr fp, [r3, r4, lsl #2]
0046a864: mov r0, fp
0046a868: bl #0x30de54
0046a86c: mov r1, fp
0046a870: add r2, fp, r0
0046a874: mov r0, r6
0046a878: bl #0x3109e0
0046a87c: mov r0, r5
0046a880: mov r1, r6
0046a884: bl #0x461668
0046a888: mov r1, r4
0046a88c: mov r2, r8
0046a890: mov r0, r7
0046a894: bl #0x467040
0046a898: mov r1, sb
0046a89c: str r0, [sp, #0x1c]
0046a8a0: mov r0, r5
0046a8a4: bl #0x38b808
0046a8a8: ldr r3, [sp, #0x20]
0046a8ac: add r4, r4, #1
0046a8b0: cmp r3, r4
0046a8b4: bgt #0x46a85c
0046a8b8: add r8, r8, #1
0046a8bc: cmp r8, #3
0046a8c0: bne #0x46a828
0046a8c4: ldr r3, [pc, #0xf8]
0046a8c8: ldr r2, [sp, #4]
0046a8cc: ldr r1, [pc, #0xf4]
0046a8d0: mov r8, #0
0046a8d4: ldr r3, [r2, r3]
0046a8d8: str r1, [sp, #0xc]
0046a8dc: add sb, sp, #0x1c
0046a8e0: str r3, [sp, #8]
0046a8e4: mov sl, r7
0046a8e8: ldr r1, [sp, #8]
0046a8ec: mov r0, r5
0046a8f0: ldr r3, [r1]
0046a8f4: ldr r1, [sp, #0x10]
0046a8f8: str r3, [sp, #0x20]
0046a8fc: bl #0x38b808
0046a900: ldr r3, [sp, #0x20]
0046a904: cmp r3, #0
0046a908: ble #0x46a978
0046a90c: ldr r3, [sp, #4]
0046a910: ldr r2, [sp, #0xc]
0046a914: mov r4, #0
0046a918: ldr r7, [r3, r2]
0046a91c: ldr r3, [r7]
0046a920: ldr fp, [r3, r4, lsl #2]
0046a924: mov r0, fp
0046a928: bl #0x30de54
0046a92c: mov r1, fp
0046a930: add r2, fp, r0
0046a934: mov r0, r6
0046a938: bl #0x3109e0
0046a93c: mov r0, r5
0046a940: mov r1, r6
0046a944: bl #0x461668
0046a948: mov r1, r4
0046a94c: mov r2, r8
0046a950: mov r0, sl
0046a954: bl #0x466d10
0046a958: mov r1, sb
0046a95c: str r0, [sp, #0x1c]
0046a960: mov r0, r5
0046a964: bl #0x38b808
0046a968: ldr r3, [sp, #0x20]
0046a96c: add r4, r4, #1
0046a970: cmp r3, r4
0046a974: bgt #0x46a91c
0046a978: add r8, r8, #1
0046a97c: cmp r8, #3
0046a980: bne #0x46a8e8
0046a984: mov r0, r6
0046a988: bl #0x3139ac
0046a98c: ldr r2, [sp, #4]
0046a990: ldr r1, [sp, #0x14]
0046a994: ldr r3, [r2, r1]
0046a998: ldr r2, [sp, #0x3c]
0046a99c: ldr r3, [r3]
0046a9a0: cmp r2, r3
0046a9a4: bne #0x46a9b0
0046a9a8: add sp, sp, #0x44
0046a9ac: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046a9b0: bl #0x30e310
0046a9b4: subseq sl, r2, r0, ror #5
0046a9b8: andeq r4, r0, ip, lsr #1
0046a9bc: andeq r1, r0, r0, asr #17
0046a9c0: andeq r3, r0, ip, asr fp
0046a9c4: andeq r2, r0, r4, ror r2
0046a9c8: andeq r2, r0, r8, ror #6

0x4689d8 _ZN14PlayerSavegame16__SavePropertiesEP11IStreamBasePv
004689d8: push {r4, r5, r6, r7, r8, sl, lr}
004689dc: ldr r7, [r1, #0x10]
004689e0: ldr r5, [pc, #0xec]
004689e4: sub sp, sp, #0x14
004689e8: cmp r7, #0
004689ec: mov r6, r1
004689f0: mov r4, r0
004689f4: add r5, pc, r5
004689f8: beq #0x468a74
004689fc: mov r3, #0xe0
00468a00: add r1, sp, #0x10
00468a04: str r3, [r1, #-4]!
00468a08: mov r0, r4
00468a0c: bl #0x38b808
00468a10: ldr r3, [sp, #0xc]
00468a14: cmp r3, #0
00468a18: ble #0x468a60
00468a1c: ldr r3, [pc, #0xb4]
00468a20: add r7, r7, #0x8e0
00468a24: add r7, r7, #0xc
00468a28: ldr sl, [r5, r3]
00468a2c: add r8, sp, #8
00468a30: mov r5, #0
00468a34: ldr r3, [sl, r5, lsl #2]
00468a38: mov r0, r4
00468a3c: mov r1, r8
00468a40: add r3, r7, r3
00468a44: ldr r3, [r3, #4]
00468a48: add r5, r5, #1
00468a4c: str r3, [sp, #8]
00468a50: bl #0x38b808
00468a54: ldr r3, [sp, #0xc]
00468a58: cmp r3, r5
00468a5c: bgt #0x468a34
00468a60: mov r0, r4
00468a64: add r1, r6, #0x194
00468a68: bl #0x33e138
00468a6c: add sp, sp, #0x14
00468a70: pop {r4, r5, r6, r7, r8, sl, pc}
00468a74: ldr r3, [pc, #0x60]
00468a78: ldr r3, [r5, r3]
00468a7c: ldr r3, [r3]
00468a80: cmp r3, #2
00468a84: streq r7, [r7]
00468a88: beq #0x468a6c
00468a8c: cmp r3, #1
00468a90: bne #0x468a6c
00468a94: ldr r0, [pc, #0x44]
00468a98: ldr r1, [pc, #0x44]
00468a9c: ldr r2, [pc, #0x44]
00468aa0: ldr r0, [r5, r0]
00468aa4: ldr r3, [pc, #0x40]
00468aa8: mov ip, #0x344
00468aac: add r1, pc, r1
00468ab0: add r0, r0, #0xa8
00468ab4: add r2, pc, r2
00468ab8: add r3, pc, r3
00468abc: str ip, [sp]
00468ac0: bl #0x30e004
00468ac4: ldr r7, [r6, #0x10]
00468ac8: cmp r7, #0
00468acc: bne #0x4689fc
00468ad0: b #0x468a6c

0x469764 _ZN14PlayerSavegame11_InitSkillsEv
00469764: push {r4, r5, r6, r7, r8, lr}
00469768: ldr r5, [r0, #0x80]
0046976c: mov r4, r0
00469770: cmp r5, #0
00469774: beq #0x46977c
00469778: pop {r4, r5, r6, r7, r8, pc}
0046977c: ldr r0, [r0, #0x10]
00469780: bl #0x3bc5fc
00469784: mov r6, r0
00469788: ldr r0, [r0, #4]
0046978c: mov r1, r5
00469790: str r0, [r4, #0x84]
00469794: lsl r0, r0, #3
00469798: bl #0x31056c
0046979c: ldr r3, [r4, #0x84]
004697a0: str r0, [r4, #0x80]
004697a4: cmp r3, #0
004697a8: beq #0x4697e4
004697ac: mov r1, r5
004697b0: b #0x4697b8
004697b4: ldr r0, [r4, #0x80]
004697b8: ldr r2, [r6, #8]
004697bc: add r3, r0, r5, lsl #3
004697c0: ldr r2, [r2, r5, lsl #2]
004697c4: str r2, [r0, r5, lsl #3]
004697c8: mov r2, #0
004697cc: strb r1, [r3, #6]
004697d0: strh r2, [r3, #4]
004697d4: ldr r3, [r4, #0x84]
004697d8: add r5, r5, #1
004697dc: cmp r3, r5
004697e0: bhi #0x4697b4
004697e4: ldr r1, [r4, #0x88]
004697e8: ldr r0, [r4, #0x8c]
004697ec: rsb r3, r1, r0
004697f0: asr r3, r3, #3
004697f4: add r2, r3, r3, lsl #2
004697f8: add r2, r2, r2, lsl #4
004697fc: add r2, r2, r2, lsl #8
00469800: add r2, r2, r2, lsl #16
00469804: add r3, r3, r2, lsl #1
00469808: cmp r3, #0
0046980c: beq #0x469778
00469810: mov r6, #0
00469814: mov r7, r6
00469818: mov r8, r6
0046981c: b #0x469844
00469820: rsb r3, r1, r0
00469824: asr r3, r3, #3
00469828: add r2, r3, r3, lsl #2
0046982c: add r2, r2, r2, lsl #4
00469830: add r2, r2, r2, lsl #8
00469834: add r2, r2, r2, lsl #16
00469838: add r3, r3, r2, lsl #1
0046983c: cmp r7, r3
00469840: bhs #0x469778
00469844: add r5, r1, r6
00469848: ldr r3, [r5, #0x10]
0046984c: add r7, r7, #1
00469850: add r6, r6, #0x18
00469854: cmp r3, #0
00469858: beq #0x469820
0046985c: mov r0, r5
00469860: ldr r1, [r5, #4]
00469864: bl #0x345c94
00469868: str r8, [r5, #0x10]
0046986c: str r5, [r5, #8]
00469870: str r8, [r5, #4]
00469874: str r5, [r5, #0xc]
00469878: ldr r1, [r4, #0x88]
0046987c: ldr r0, [r4, #0x8c]
00469880: b #0x469820

0x469b18 _ZN14PlayerSavegame20__LoadFastTravelListEP11IStreamBasePv
00469b18: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469b1c: ldr fp, [pc, #0x1a8]
00469b20: ldr r2, [pc, #0x1a8]
00469b24: sub sp, sp, #0x44
00469b28: add fp, pc, fp
00469b2c: ldr r3, [fp, r2]
00469b30: ldr r6, [pc, #0x19c]
00469b34: str r2, [sp, #0x14]
00469b38: ldr r3, [r3]
00469b3c: str r0, [sp, #0xc]
00469b40: mov r5, r1
00469b44: str r3, [sp, #0x3c]
00469b48: ldr r3, [pc, #0x188]
00469b4c: add r6, pc, r6
00469b50: mov r8, #0
00469b54: add r3, pc, r3
00469b58: str r3, [sp, #0x10]
00469b5c: add r4, sp, #0x24
00469b60: add sb, sp, #0x1c
00469b64: mov sl, #1
00469b68: mov r0, r4
00469b6c: mov r1, #0x10
00469b70: str r4, [sp, #0x34]
00469b74: str r4, [sp, #0x38]
00469b78: bl #0x31167c
00469b7c: ldr r3, [sp, #0x34]
00469b80: mov r7, #0
00469b84: ldr r0, [sp, #0xc]
00469b88: mov r1, r4
00469b8c: strb r7, [r3]
00469b90: bl #0x461da8
00469b94: ldr r1, [sp, #0x38]
00469b98: ldr r3, [sp, #0x34]
00469b9c: rsb r3, r1, r3
00469ba0: cmp r3, #0x40
00469ba4: bhi #0x469cbc
00469ba8: mvn r2, #0
00469bac: cmp r3, r2
00469bb0: movhs r3, r2
00469bb4: cmp r3, #0x3f
00469bb8: str r7, [sb, #4]
00469bbc: str r7, [sb]
00469bc0: bls #0x469cac
00469bc4: mov r2, #0x3f
00469bc8: mov r3, #0x40
00469bcc: stmib sp, {r4, sb}
00469bd0: mov r7, #0
00469bd4: mov sb, r8
00469bd8: mov r4, r3
00469bdc: mov r8, r5
00469be0: mov r5, r2
00469be4: b #0x469c08
00469be8: cmp r3, #0x30
00469bec: beq #0x469bf8
00469bf0: mov r0, r6
00469bf4: bl #0x708f50
00469bf8: add r7, r7, #1
00469bfc: cmp r4, r7
00469c00: bls #0x469c48
00469c04: ldr r1, [sp, #0x38]
00469c08: rsb r3, r7, r5
00469c0c: ldrb r3, [r1, r3]
00469c10: cmp r3, #0x31
00469c14: bne #0x469be8
00469c18: cmp r7, #0x3f
00469c1c: bhi #0x469ca0
00469c20: lsr r3, r7, #5
00469c24: add r2, sp, #0x40
00469c28: add r3, r2, r3, lsl #2
00469c2c: ldr r2, [r3, #-0x24]
00469c30: and r1, r7, #0x1f
00469c34: add r7, r7, #1
00469c38: orr r2, r2, sl, lsl r1
00469c3c: cmp r4, r7
00469c40: str r2, [r3, #-0x24]
00469c44: bhi #0x469c04
00469c48: mov r5, r8
00469c4c: ldr r4, [sp, #4]
00469c50: mov r8, sb
00469c54: ldr sb, [sp, #8]
00469c58: ldr r3, [sp, #0x1c]
00469c5c: add r8, r8, #1
00469c60: mov r0, r4
00469c64: str r3, [r5, #0x17c]
00469c68: ldr r3, [sp, #0x20]
00469c6c: str r3, [r5, #0x180]
00469c70: bl #0x3139ac
00469c74: cmp r8, #3
00469c78: add r5, r5, #8
00469c7c: bne #0x469b68
00469c80: ldr r2, [sp, #0x14]
00469c84: ldr r3, [fp, r2]
00469c88: ldr r2, [sp, #0x3c]
00469c8c: ldr r3, [r3]
00469c90: cmp r2, r3
00469c94: bne #0x469cc8
00469c98: add sp, sp, #0x44
00469c9c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469ca0: ldr r0, [sp, #0x10]
00469ca4: bl #0x708eb0
00469ca8: b #0x469c20
00469cac: cmp r3, #0
00469cb0: subne r2, r3, #1
00469cb4: bne #0x469bcc
00469cb8: b #0x469c58
00469cbc: mov r0, r4
00469cc0: bl #0x3139ac
00469cc4: b #0x469c80
00469cc8: bl #0x30e310
00469ccc: subseq sl, r2, r8, ror #30
00469cd0: andeq r4, r0, ip, lsr #1
00469cd4: subeq r8, r5, ip, ror r1
00469cd8: subeq r8, r5, r4, ror r1

0x4680a8 _ZN14PlayerSavegame17SG_SetSkillInSlotEij
004680a8: push {r4, r5, r6, r7, r8, sb, sl, lr}
004680ac: ldr r3, [r0, #0x84]
004680b0: mov r5, r2
004680b4: ldr r7, [pc, #0x384]
004680b8: cmp r3, r2
004680bc: movhi r2, #0
004680c0: movls r2, #1
004680c4: cmn r5, #1
004680c8: moveq r2, #0
004680cc: cmp r2, #0
004680d0: sub sp, sp, #0x20
004680d4: mov r6, r0
004680d8: add r7, pc, r7
004680dc: mov r4, r1
004680e0: beq #0x468108
004680e4: ldr r3, [pc, #0x358]
004680e8: ldr r3, [r7, r3]
004680ec: ldr r3, [r3]
004680f0: cmp r3, #2
004680f4: moveq r3, #0
004680f8: streq r3, [r3]
004680fc: beq #0x468108
00468100: cmp r3, #1
00468104: beq #0x4683b8
00468108: cmp r4, #0
0046810c: blt #0x468360
00468110: ldr r3, [r6, #0x80]
00468114: cmp r3, #0
00468118: beq #0x4683ec
0046811c: ldr r0, [r6, #0x10]
00468120: mvn r1, #0
00468124: add r0, r0, #0x37c
00468128: bl #0x3fc6a0
0046812c: mov r1, r4
00468130: mov r7, r0
00468134: mov r0, r6
00468138: bl #0x467488
0046813c: cmn r5, #1
00468140: beq #0x4682f8
00468144: mov sl, #0x18
00468148: mul sl, sl, r7
0046814c: ldr r0, [r6, #0x88]
00468150: add sb, sp, #0x18
00468154: add r8, r0, sl
00468158: ldr r3, [r8, #8]
0046815c: cmp r8, r3
00468160: beq #0x46819c
00468164: ldr r2, [r3, #0x14]
00468168: cmp r5, r2
0046816c: beq #0x468214
00468170: ldr r2, [r3, #0xc]
00468174: cmp r2, #0
00468178: bne #0x468184
0046817c: b #0x468250
00468180: mov r2, r3
00468184: ldr r3, [r2, #8]
00468188: cmp r3, #0
0046818c: bne #0x468180
00468190: mov r3, r2
00468194: cmp r8, r3
00468198: bne #0x468164
0046819c: add r1, r0, sl
004681a0: ldr ip, [r1, #4]
004681a4: cmp ip, #0
004681a8: moveq ip, r1
004681ac: beq #0x4681dc
004681b0: mov r2, r1
004681b4: b #0x4681bc
004681b8: mov ip, r3
004681bc: ldr r3, [ip, #0x10]
004681c0: cmp r4, r3
004681c4: ldrgt r3, [ip, #0xc]
004681c8: ldrle r3, [ip, #8]
004681cc: movgt ip, r2
004681d0: mov r2, ip
004681d4: cmp r3, #0
004681d8: bne #0x4681b8
004681dc: cmp r1, ip
004681e0: beq #0x468284
004681e4: ldr r2, [ip, #0x10]
004681e8: mov r3, ip
004681ec: cmp r4, r2
004681f0: blt #0x468284
004681f4: str r5, [r3, #0x14]
004681f8: ldr r0, [r6, #0x10]
004681fc: cmp r0, #0
00468200: beq #0x46820c
00468204: add r0, r0, #0x3c8
00468208: bl #0x3d8a04
0046820c: add sp, sp, #0x20
00468210: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00468214: ldr r7, [r3, #0xc]
00468218: add r0, r0, sl
0046821c: cmp r7, #0
00468220: bne #0x46822c
00468224: b #0x4682ac
00468228: mov r7, r2
0046822c: ldr r2, [r7, #8]
00468230: cmp r2, #0
00468234: bne #0x468228
00468238: mov r1, sb
0046823c: str r3, [sp, #0x18]
00468240: bl #0x467c18
00468244: mov r3, r7
00468248: ldr r0, [r6, #0x88]
0046824c: b #0x46815c
00468250: ldr r1, [r3, #4]
00468254: ldr ip, [r1, #0xc]
00468258: cmp r3, ip
0046825c: bne #0x468278
00468260: mov r3, r1
00468264: ldr r1, [r1, #4]
00468268: ldr r2, [r1, #0xc]
0046826c: cmp r2, r3
00468270: beq #0x468260
00468274: ldr r2, [r3, #0xc]
00468278: cmp r2, r1
0046827c: movne r3, r1
00468280: b #0x46815c
00468284: add r3, sp, #8
00468288: mov lr, #0
0046828c: add r0, sp, #0x10
00468290: add r2, sp, #0x14
00468294: str r4, [sp, #8]
00468298: str lr, [sp, #0xc]
0046829c: str ip, [sp, #0x14]
004682a0: bl #0x342bbc
004682a4: ldr r3, [sp, #0x10]
004682a8: b #0x4681f4
004682ac: ldr r2, [r3, #4]
004682b0: ldr r1, [r2, #0xc]
004682b4: cmp r3, r1
004682b8: movne r7, r3
004682bc: bne #0x4682d4
004682c0: mov r7, r2
004682c4: ldr r2, [r2, #4]
004682c8: ldr r1, [r2, #0xc]
004682cc: cmp r1, r7
004682d0: beq #0x4682c0
004682d4: ldr r1, [r7, #0xc]
004682d8: str r3, [sp, #0x18]
004682dc: cmp r2, r1
004682e0: movne r7, r2
004682e4: mov r1, sb
004682e8: bl #0x467c18
004682ec: mov r3, r7
004682f0: ldr r0, [r6, #0x88]
004682f4: b #0x46815c
004682f8: ldr r3, [r6, #0x88]
004682fc: mov r0, #0x18
00468300: mla r0, r0, r7, r3
00468304: ldr r3, [r0, #4]
00468308: cmp r3, #0
0046830c: beq #0x46820c
00468310: mov r1, r0
00468314: b #0x46831c
00468318: mov r3, r2
0046831c: ldr r2, [r3, #0x10]
00468320: cmp r4, r2
00468324: ldrgt r2, [r3, #0xc]
00468328: ldrle r2, [r3, #8]
0046832c: movgt r3, r1
00468330: mov r1, r3
00468334: cmp r2, #0
00468338: bne #0x468318
0046833c: cmp r0, r3
00468340: beq #0x46820c
00468344: ldr r2, [r3, #0x10]
00468348: cmp r4, r2
0046834c: blt #0x46820c
00468350: add r1, sp, #0x20
00468354: str r3, [r1, #-4]!
00468358: bl #0x467c18
0046835c: b #0x46820c
00468360: ldr r3, [pc, #0xdc]
00468364: ldr r3, [r7, r3]
00468368: ldr r3, [r3]
0046836c: cmp r3, #2
00468370: moveq r3, #0
00468374: streq r3, [r3]
00468378: beq #0x468110
0046837c: cmp r3, #1
00468380: bne #0x468110
00468384: ldr r0, [pc, #0xbc]
00468388: ldr r1, [pc, #0xbc]
0046838c: ldr r2, [pc, #0xbc]
00468390: ldr r0, [r7, r0]
00468394: ldr r3, [pc, #0xb8]
00468398: mov ip, #0xe2
0046839c: add r1, pc, r1
004683a0: add r2, pc, r2
004683a4: add r3, pc, r3
004683a8: add r0, r0, #0xa8
004683ac: str ip, [sp]
004683b0: bl #0x30e004
004683b4: b #0x468110
004683b8: ldr r0, [pc, #0x88]
004683bc: ldr r1, [pc, #0x94]
004683c0: ldr r2, [pc, #0x94]
004683c4: ldr r0, [r7, r0]
004683c8: ldr r3, [pc, #0x90]
004683cc: mov ip, #0xe1
004683d0: add r1, pc, r1
004683d4: add r2, pc, r2
004683d8: add r3, pc, r3
004683dc: add r0, r0, #0xa8
004683e0: str ip, [sp]
004683e4: bl #0x30e004
004683e8: b #0x468108
004683ec: ldr r2, [pc, #0x50]
004683f0: ldr r2, [r7, r2]
004683f4: ldr r2, [r2]
004683f8: cmp r2, #2
004683fc: streq r3, [r3]
00468400: beq #0x46811c
00468404: cmp r2, #1
00468408: bne #0x46811c
0046840c: ldr r0, [pc, #0x34]
00468410: ldr r1, [pc, #0x4c]
00468414: ldr r2, [pc, #0x4c]
00468418: ldr r0, [r7, r0]
0046841c: ldr r3, [pc, #0x48]
00468420: mov ip, #0xe3
00468424: add r1, pc, r1
00468428: add r2, pc, r2
0046842c: add r3, pc, r3
00468430: add r0, r0, #0xa8
00468434: str ip, [sp]
00468438: bl #0x30e004
0046843c: b #0x46811c
00468440: ldrheq ip, [r2], #-0x98
00468444: andeq r3, r0, r0, asr #19
00468448: andeq r1, r0, r0, asr #19
0046844c: subeq r6, r5, ip, lsr r0
00468450: subeq r5, r6, r8, ror #1
00468454: ldrdeq r4, r5, [r6], #-0xec
00468458: subeq r6, r5, r8
0046845c: subeq r5, r6, ip, ror r0
00468460: subeq r4, r6, r8, lsr #29
00468464: strheq r5, [r5], #-0xf4
00468468: subeq r4, r6, r0, asr #29
0046846c: subeq r4, r6, r4, asr lr

0x4691d0 _ZN14PlayerSavegame13__LoadFaeriesEP11IStreamBasePv
004691d0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004691d4: ldr r3, [pc, #0x138]
004691d8: sub sp, sp, #0x2c
004691dc: ldr sl, [pc, #0x134]
004691e0: str r3, [sp, #0xc]
004691e4: ldr r3, [pc, #0x130]
004691e8: mov r7, r1
004691ec: mov r5, r0
004691f0: str r3, [sp, #0x10]
004691f4: ldr r3, [pc, #0x124]
004691f8: mov r4, r1
004691fc: mov r6, #0
00469200: add r3, pc, r3
00469204: str r3, [sp, #0x14]
00469208: ldr r3, [pc, #0x114]
0046920c: add r8, sp, #0x24
00469210: add sl, pc, sl
00469214: add r3, pc, r3
00469218: str r3, [sp, #0x18]
0046921c: ldr r3, [pc, #0x104]
00469220: add r3, pc, r3
00469224: str r3, [sp, #0x1c]
00469228: ldr r3, [r4, #0x94]
0046922c: cmp r3, #0
00469230: beq #0x4692bc
00469234: add r1, r7, r6, lsl #2
00469238: add r1, r1, #0xac
0046923c: mov r0, r5
00469240: bl #0x38b758
00469244: mov r0, r5
00469248: mov r1, r8
0046924c: bl #0x313b48
00469250: ldr r3, [r4, #0xa0]
00469254: ldr r2, [sp, #0x24]
00469258: cmp r3, r2
0046925c: bne #0x4692b4
00469260: cmp r3, #0
00469264: beq #0x4692a4
00469268: mov sb, #0
0046926c: ldr r1, [r4, #0x94]
00469270: lsl fp, sb, #2
00469274: mov r0, r5
00469278: add r1, r1, fp
0046927c: add r1, r1, #2
00469280: bl #0x469070
00469284: ldr r1, [r4, #0x94]
00469288: mov r0, r5
0046928c: add sb, sb, #1
00469290: add r1, r1, fp
00469294: bl #0x469120
00469298: ldr r3, [sp, #0x24]
0046929c: cmp r3, sb
004692a0: bhi #0x46926c
004692a4: add r6, r6, #1
004692a8: cmp r6, #3
004692ac: add r4, r4, #4
004692b0: bne #0x469228
004692b4: add sp, sp, #0x2c
004692b8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004692bc: ldr r1, [sp, #0xc]
004692c0: ldr r2, [sl, r1]
004692c4: ldr r2, [r2]
004692c8: cmp r2, #2
004692cc: beq #0x46930c
004692d0: cmp r2, #1
004692d4: bne #0x4692b4
004692d8: ldr r3, [sp, #0x10]
004692dc: movw ip, #0x20d
004692e0: ldr r1, [sp, #0x14]
004692e4: ldr r0, [sl, r3]
004692e8: ldr r2, [sp, #0x18]
004692ec: ldr r3, [sp, #0x1c]
004692f0: add r0, r0, #0xa8
004692f4: str ip, [sp]
004692f8: bl #0x30e004
004692fc: ldr r3, [r4, #0x94]
00469300: cmp r3, #0
00469304: bne #0x469234
00469308: b #0x4692b4
0046930c: str r3, [r3]
00469310: b #0x4692b4
00469314: andeq r3, r0, r0, asr #19
00469318: subseq fp, r2, r0, lsl #17
0046931c: andeq r1, r0, r0, asr #19
00469320: ldrdeq r5, r6, [r5], #-0x18
00469324: subeq r4, r6, ip, ror #5
00469328: subeq r4, r6, r8, lsl #5

0x46a3a0 _ZN14PlayerSavegame15__LoadInventoryEP11IStreamBasePv
0046a3a0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a3a4: ldr r4, [pc, #0x3c8]
0046a3a8: ldr r2, [pc, #0x3c8]
0046a3ac: sub sp, sp, #0x84
0046a3b0: add r4, pc, r4
0046a3b4: str r2, [sp, #0x3c]
0046a3b8: ldr r2, [r4, r2]
0046a3bc: str r1, [sp, #0x14]
0046a3c0: ldr r3, [r1, #0x10]
0046a3c4: ldr r2, [r2]
0046a3c8: mov r5, r0
0046a3cc: cmp r3, #0
0046a3d0: str r2, [sp, #0x7c]
0046a3d4: beq #0x46a70c
0046a3d8: add r1, sp, #0x64
0046a3dc: mov r0, r1
0046a3e0: str r1, [sp, #8]
0046a3e4: mov r1, #0x10
0046a3e8: str r0, [sp, #0x74]
0046a3ec: str r0, [sp, #0x78]
0046a3f0: bl #0x31167c
0046a3f4: ldr r3, [sp, #0x74]
0046a3f8: mov r6, #0
0046a3fc: add r1, sp, #0x5c
0046a400: strb r6, [r3]
0046a404: mov r0, r5
0046a408: bl #0x313b48
0046a40c: mov r0, r5
0046a410: add r1, sp, #0x58
0046a414: bl #0x313b48
0046a418: mov r0, r5
0046a41c: add r1, sp, #0x54
0046a420: bl #0x313b48
0046a424: ldr r3, [sp, #0x14]
0046a428: ldr r1, [sp, #0x5c]
0046a42c: ldr r0, [r3, #0x10]
0046a430: add r0, r0, #0x37c
0046a434: bl #0x3fdfd8
0046a438: ldr r0, [sp, #0x14]
0046a43c: ldr r2, [sp, #0x58]
0046a440: ldr r3, [r0, #0x10]
0046a444: strb r2, [r3, #0x3aa]
0046a448: ldr r3, [sp, #0x54]
0046a44c: cmp r3, r6
0046a450: beq #0x46a6d4
0046a454: ldr r1, [pc, #0x320]
0046a458: ldr r2, [pc, #0x320]
0046a45c: ldr r3, [pc, #0x320]
0046a460: ldr r0, [pc, #0x320]
0046a464: str r1, [sp, #0x20]
0046a468: str r2, [sp, #0x38]
0046a46c: add r1, sp, #0x50
0046a470: add r2, sp, #0x4c
0046a474: str r3, [sp, #0xc]
0046a478: str r0, [sp, #0x10]
0046a47c: str r1, [sp, #0x1c]
0046a480: str r2, [sp, #0x34]
0046a484: add r3, sp, #0x48
0046a488: add r0, sp, #0x44
0046a48c: add r1, sp, #0x63
0046a490: add r2, sp, #0x40
0046a494: str r6, [sp, #0x18]
0046a498: str r3, [sp, #0x30]
0046a49c: str r0, [sp, #0x2c]
0046a4a0: str r1, [sp, #0x24]
0046a4a4: str r2, [sp, #0x28]
0046a4a8: mov r0, r5
0046a4ac: ldr r1, [sp, #8]
0046a4b0: bl #0x461da8
0046a4b4: ldr r0, [sp, #0x20]
0046a4b8: ldr sl, [sp, #0x78]
0046a4bc: ldr r3, [r4, r0]
0046a4c0: ldr r7, [r3]
0046a4c4: cmp r7, #0
0046a4c8: beq #0x46a704
0046a4cc: ldr r1, [sp, #0x38]
0046a4d0: mov r6, #0
0046a4d4: ldr r3, [r4, r1]
0046a4d8: ldr r8, [r3]
0046a4dc: b #0x46a4ec
0046a4e0: add r6, r6, #1
0046a4e4: cmp r6, r7
0046a4e8: beq #0x46a704
0046a4ec: mov r0, sl
0046a4f0: ldr r1, [r8, r6, lsl #2]
0046a4f4: bl #0x30e31c
0046a4f8: cmp r0, #0
0046a4fc: bne #0x46a4e0
0046a500: mov r0, r5
0046a504: ldr r1, [sp, #0x1c]
0046a508: bl #0x38b758
0046a50c: mov r0, r5
0046a510: ldr r1, [sp, #0x34]
0046a514: bl #0x38b758
0046a518: mov r0, r5
0046a51c: ldr r1, [sp, #0x30]
0046a520: bl #0x38b758
0046a524: mov r0, r5
0046a528: ldr r1, [sp, #0x2c]
0046a52c: bl #0x38b758
0046a530: mov r0, r5
0046a534: ldr r1, [sp, #0x24]
0046a538: bl #0x39f638
0046a53c: mov r0, r5
0046a540: ldr r1, [sp, #0x28]
0046a544: bl #0x313b48
0046a548: mov r1, #0
0046a54c: mov r0, #0x6c
0046a550: bl #0x310570
0046a554: mov r1, r6
0046a558: mov r7, r0
0046a55c: ldr r2, [sp, #0x48]
0046a560: bl #0x3fc26c
0046a564: mov r0, r7
0046a568: ldr r1, [sp, #0x44]
0046a56c: bl #0x3fbc58
0046a570: ldrb r3, [sp, #0x63]
0046a574: subs r3, r3, #0
0046a578: movne r3, #1
0046a57c: strb r3, [r7, #0x68]
0046a580: ldr r3, [sp, #0x40]
0046a584: cmp r3, #0
0046a588: beq #0x46a608
0046a58c: mov r6, #0
0046a590: mov r0, r5
0046a594: ldr r1, [sp, #8]
0046a598: bl #0x461da8
0046a59c: ldr r2, [sp, #0xc]
0046a5a0: ldr sb, [sp, #0x78]
0046a5a4: ldr r3, [r4, r2]
0046a5a8: ldr sl, [r3]
0046a5ac: cmp sl, #0
0046a5b0: beq #0x46a6fc
0046a5b4: ldr r0, [sp, #0x10]
0046a5b8: mov r8, #0
0046a5bc: ldr r3, [r4, r0]
0046a5c0: ldr fp, [r3]
0046a5c4: b #0x46a5d4
0046a5c8: add r8, r8, #1
0046a5cc: cmp r8, sl
0046a5d0: beq #0x46a6fc
0046a5d4: mov r0, sb
0046a5d8: ldr r1, [fp, r8, lsl #2]
0046a5dc: bl #0x30e31c
0046a5e0: cmp r0, #0
0046a5e4: bne #0x46a5c8
0046a5e8: mov r1, r8
0046a5ec: mov r0, r7
0046a5f0: mvn r2, #0
0046a5f4: bl #0x3fbc60
0046a5f8: ldr r3, [sp, #0x40]
0046a5fc: add r6, r6, #1
0046a600: cmp r3, r6
0046a604: bhi #0x46a590
0046a608: ldr r1, [sp, #0x14]
0046a60c: mov r2, #1
0046a610: mov r3, r2
0046a614: ldr r0, [r1, #0x10]
0046a618: mov r1, r7
0046a61c: add r0, r0, #0x37c
0046a620: bl #0x3ff5d4
0046a624: ldr r3, [sp, #0x50]
0046a628: mov r6, r0
0046a62c: cmn r3, #1
0046a630: beq #0x46a670
0046a634: ldr r2, [sp, #0x14]
0046a638: mov r3, #1
0046a63c: ldr r1, [r2, #0x10]
0046a640: mov r2, r0
0046a644: mov r0, #0
0046a648: ldrb r7, [r1, #0x3aa]
0046a64c: strb r0, [r1, #0x3aa]
0046a650: ldr r1, [sp, #0x14]
0046a654: ldr r0, [r1, #0x10]
0046a658: ldr r1, [sp, #0x50]
0046a65c: add r0, r0, #0x37c
0046a660: bl #0x400634
0046a664: ldr r2, [sp, #0x14]
0046a668: ldr r3, [r2, #0x10]
0046a66c: strb r7, [r3, #0x3aa]
0046a670: ldr r3, [sp, #0x4c]
0046a674: cmn r3, #1
0046a678: beq #0x46a6b8
0046a67c: ldr r3, [sp, #0x14]
0046a680: mov r0, #1
0046a684: mov r2, r6
0046a688: ldr r1, [r3, #0x10]
0046a68c: mov r3, #1
0046a690: ldrb r6, [r1, #0x3aa]
0046a694: strb r0, [r1, #0x3aa]
0046a698: ldr r1, [sp, #0x14]
0046a69c: ldr r0, [r1, #0x10]
0046a6a0: ldr r1, [sp, #0x4c]
0046a6a4: add r0, r0, #0x37c
0046a6a8: bl #0x400634
0046a6ac: ldr r2, [sp, #0x14]
0046a6b0: ldr r3, [r2, #0x10]
0046a6b4: strb r6, [r3, #0x3aa]
0046a6b8: ldr r3, [sp, #0x18]
0046a6bc: add r3, r3, #1
0046a6c0: str r3, [sp, #0x18]
0046a6c4: ldr r0, [sp, #0x18]
0046a6c8: ldr r3, [sp, #0x54]
0046a6cc: cmp r3, r0
0046a6d0: bhi #0x46a4a8
0046a6d4: ldr r0, [sp, #8]
0046a6d8: bl #0x3139ac
0046a6dc: ldr r1, [sp, #0x3c]
0046a6e0: ldr r2, [sp, #0x7c]
0046a6e4: ldr r3, [r4, r1]
0046a6e8: ldr r3, [r3]
0046a6ec: cmp r2, r3
0046a6f0: bne #0x46a770
0046a6f4: add sp, sp, #0x84
0046a6f8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046a6fc: mvn r1, #0
0046a700: b #0x46a5ec
0046a704: mvn r6, #0
0046a708: b #0x46a500
0046a70c: ldr r2, [pc, #0x78]
0046a710: ldr r2, [r4, r2]
0046a714: ldr r2, [r2]
0046a718: cmp r2, #2
0046a71c: streq r3, [r3]
0046a720: beq #0x46a6dc
0046a724: cmp r2, #1
0046a728: bne #0x46a6dc
0046a72c: ldr r0, [pc, #0x5c]
0046a730: ldr r1, [pc, #0x5c]
0046a734: ldr r2, [pc, #0x5c]
0046a738: ldr r0, [r4, r0]
0046a73c: ldr r3, [pc, #0x58]
0046a740: movw ip, #0x28f
0046a744: add r1, pc, r1
0046a748: add r3, pc, r3
0046a74c: add r0, r0, #0xa8
0046a750: add r2, pc, r2
0046a754: str ip, [sp]
0046a758: bl #0x30e004
0046a75c: ldr r0, [sp, #0x14]
0046a760: ldr r3, [r0, #0x10]
0046a764: cmp r3, #0
0046a768: beq #0x46a6dc
0046a76c: b #0x46a3d8
0046a770: bl #0x30e310
0046a774: subseq sl, r2, r0, ror #13
0046a778: andeq r4, r0, ip, lsr #1
0046a77c: andeq r0, r0, r0, ror #26
0046a780: andeq r1, r0, r4, asr ip
0046a784: andeq r1, r0, r8, lsl #3
0046a788: andeq r1, r0, ip, asr #5
0046a78c: andeq r3, r0, r0, asr #19
0046a790: andeq r1, r0, r0, asr #19
0046a794: umaaleq r3, r5, r4, ip
0046a798: subeq r2, r6, r8, asr #26
0046a79c: subeq r2, r6, r0, ror #26

0x46ac74 _ZN14PlayerSavegame12__LoadSkillsEP11IStreamBasePv
0046ac74: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046ac78: ldr r7, [pc, #0x354]
0046ac7c: ldr r2, [pc, #0x354]
0046ac80: sub sp, sp, #0x64
0046ac84: add r7, pc, r7
0046ac88: str r2, [sp, #0x1c]
0046ac8c: ldr r2, [r7, r2]
0046ac90: ldr r3, [r1, #0x80]
0046ac94: mov r6, r1
0046ac98: ldr r2, [r2]
0046ac9c: cmp r3, #0
0046aca0: mov r4, r0
0046aca4: str r2, [sp, #0x5c]
0046aca8: beq #0x46af28
0046acac: ldr r3, [r6, #0x10]
0046acb0: cmp r3, #0
0046acb4: beq #0x46af7c
0046acb8: ldr r3, [r6, #0x80]
0046acbc: cmp r3, #0
0046acc0: beq #0x46aee4
0046acc4: ldr r3, [r6, #0x10]
0046acc8: cmp r3, #0
0046accc: beq #0x46aee4
0046acd0: add r3, sp, #0x44
0046acd4: mov r0, r3
0046acd8: mov r1, #0x10
0046acdc: str r3, [sp, #0xc]
0046ace0: str r3, [sp, #0x54]
0046ace4: str r3, [sp, #0x58]
0046ace8: bl #0x31167c
0046acec: ldr r3, [sp, #0x54]
0046acf0: mov fp, #0
0046acf4: mov r0, r4
0046acf8: strb fp, [r3]
0046acfc: add r1, sp, #0x3c
0046ad00: bl #0x38b758
0046ad04: ldr r3, [sp, #0x3c]
0046ad08: cmp r3, fp
0046ad0c: ble #0x46add8
0046ad10: ldr r2, [pc, #0x2c4]
0046ad14: ldr r3, [pc, #0x2c4]
0046ad18: add ip, sp, #0x42
0046ad1c: str r2, [sp, #0x10]
0046ad20: str r3, [sp, #0x14]
0046ad24: str ip, [sp, #0x18]
0046ad28: mov r0, r4
0046ad2c: ldr r1, [sp, #0xc]
0046ad30: bl #0x461da8
0046ad34: ldr r2, [sp, #0x10]
0046ad38: ldr sl, [sp, #0x58]
0046ad3c: ldr r3, [r7, r2]
0046ad40: ldr r8, [r3]
0046ad44: cmp r8, #0
0046ad48: beq #0x46af04
0046ad4c: ldr ip, [sp, #0x14]
0046ad50: mov r5, #0
0046ad54: ldr r3, [r7, ip]
0046ad58: ldr sb, [r3]
0046ad5c: b #0x46ad6c
0046ad60: add r5, r5, #1
0046ad64: cmp r5, r8
0046ad68: beq #0x46af04
0046ad6c: mov r0, sl
0046ad70: ldr r1, [sb, r5, lsl #2]
0046ad74: bl #0x30e31c
0046ad78: cmp r0, #0
0046ad7c: bne #0x46ad60
0046ad80: ldr r0, [r6, #0x84]
0046ad84: cmp r0, #0
0046ad88: beq #0x46adbc
0046ad8c: ldr r2, [r6, #0x80]
0046ad90: ldr r3, [r2]
0046ad94: cmp r3, r5
0046ad98: movne r3, #0
0046ad9c: bne #0x46adb0
0046ada0: b #0x46af0c
0046ada4: ldr r1, [r2, #8]!
0046ada8: cmp r1, r5
0046adac: beq #0x46af0c
0046adb0: add r3, r3, #1
0046adb4: cmp r3, r0
0046adb8: bne #0x46ada4
0046adbc: mov r0, r4
0046adc0: ldr r1, [sp, #0x18]
0046adc4: bl #0x469070
0046adc8: ldr r3, [sp, #0x3c]
0046adcc: add fp, fp, #1
0046add0: cmp r3, fp
0046add4: bgt #0x46ad28
0046add8: mov r3, #0
0046addc: mov r8, r3
0046ade0: str r3, [sp, #0x38]
0046ade4: str r3, [sp, #0x34]
0046ade8: add r2, sp, #0x38
0046adec: add r3, sp, #0x24
0046adf0: str r7, [sp, #0x14]
0046adf4: str r2, [sp, #0x10]
0046adf8: add sl, sp, #0x34
0046adfc: add sb, sp, #0x2c
0046ae00: add fp, sp, #0x30
0046ae04: mov r7, r3
0046ae08: mov r0, r4
0046ae0c: ldr r1, [sp, #0x10]
0046ae10: bl #0x38b758
0046ae14: ldr r3, [sp, #0x38]
0046ae18: cmp r3, #0
0046ae1c: ble #0x46aecc
0046ae20: mov r5, #0
0046ae24: mov r1, sl
0046ae28: mov r0, r4
0046ae2c: bl #0x38b758
0046ae30: ldr r1, [r6, #0x88]
0046ae34: add r1, r1, r8
0046ae38: ldr ip, [r1, #4]
0046ae3c: cmp ip, #0
0046ae40: beq #0x46af1c
0046ae44: ldr lr, [sp, #0x34]
0046ae48: mov r2, r1
0046ae4c: b #0x46ae58
0046ae50: mov r2, ip
0046ae54: mov ip, r3
0046ae58: ldr r3, [ip, #0x10]
0046ae5c: cmp r3, lr
0046ae60: ldrlt r3, [ip, #0xc]
0046ae64: ldrge r3, [ip, #8]
0046ae68: movlt ip, r2
0046ae6c: cmp r3, #0
0046ae70: bne #0x46ae50
0046ae74: cmp r1, ip
0046ae78: beq #0x46ae8c
0046ae7c: ldr r2, [ip, #0x10]
0046ae80: mov r3, ip
0046ae84: cmp r2, lr
0046ae88: ble #0x46aeb0
0046ae8c: mov r3, r7
0046ae90: str ip, [sp, #0x30]
0046ae94: mov r0, sb
0046ae98: mov ip, #0
0046ae9c: mov r2, fp
0046aea0: str lr, [sp, #0x24]
0046aea4: str ip, [sp, #0x28]
0046aea8: bl #0x342bbc
0046aeac: ldr r3, [sp, #0x2c]
0046aeb0: add r1, r3, #0x14
0046aeb4: mov r0, r4
0046aeb8: bl #0x38b758
0046aebc: ldr r3, [sp, #0x38]
0046aec0: add r5, r5, #1
0046aec4: cmp r3, r5
0046aec8: bgt #0x46ae24
0046aecc: add r8, r8, #0x18
0046aed0: cmp r8, #0x30
0046aed4: bne #0x46ae08
0046aed8: ldr r0, [sp, #0xc]
0046aedc: ldr r7, [sp, #0x14]
0046aee0: bl #0x3139ac
0046aee4: ldr r2, [sp, #0x1c]
0046aee8: ldr r3, [r7, r2]
0046aeec: ldr r2, [sp, #0x5c]
0046aef0: ldr r3, [r3]
0046aef4: cmp r2, r3
0046aef8: bne #0x46afd0
0046aefc: add sp, sp, #0x64
0046af00: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046af04: mvn r5, #0
0046af08: b #0x46ad80
0046af0c: add r1, r2, #4
0046af10: mov r0, r4
0046af14: bl #0x469070
0046af18: b #0x46adc8
0046af1c: ldr lr, [sp, #0x34]
0046af20: mov ip, r1
0046af24: b #0x46ae74
0046af28: ldr r2, [pc, #0xb4]
0046af2c: ldr r2, [r7, r2]
0046af30: ldr r2, [r2]
0046af34: cmp r2, #2
0046af38: streq r3, [r3]
0046af3c: beq #0x46acac
0046af40: cmp r2, #1
0046af44: bne #0x46acac
0046af48: ldr r0, [pc, #0x98]
0046af4c: ldr r1, [pc, #0x98]
0046af50: ldr r2, [pc, #0x98]
0046af54: ldr r0, [r7, r0]
0046af58: ldr r3, [pc, #0x94]
0046af5c: movw ip, #0x192
0046af60: add r1, pc, r1
0046af64: add r2, pc, r2
0046af68: add r3, pc, r3
0046af6c: add r0, r0, #0xa8
0046af70: str ip, [sp]
0046af74: bl #0x30e004
0046af78: b #0x46acac
0046af7c: ldr r2, [pc, #0x60]
0046af80: ldr r2, [r7, r2]
0046af84: ldr r2, [r2]
0046af88: cmp r2, #2
0046af8c: streq r3, [r3]
0046af90: beq #0x46acb8
0046af94: cmp r2, #1
0046af98: bne #0x46acb8
0046af9c: ldr r0, [pc, #0x44]
0046afa0: ldr r1, [pc, #0x50]
0046afa4: ldr r2, [pc, #0x50]
0046afa8: ldr r0, [r7, r0]
0046afac: ldr r3, [pc, #0x4c]
0046afb0: movw ip, #0x193
0046afb4: add r1, pc, r1
0046afb8: add r2, pc, r2
0046afbc: add r3, pc, r3
0046afc0: add r0, r0, #0xa8
0046afc4: str ip, [sp]
0046afc8: bl #0x30e004
0046afcc: b #0x46acb8
0046afd0: bl #0x30e310
0046afd4: subseq sb, r2, ip, lsl #28
0046afd8: andeq r4, r0, ip, lsr #1
0046afdc: andeq r2, r0, r8, lsr #14
0046afe0: ldrdeq r3, r4, [r0], -r8
0046afe4: andeq r3, r0, r0, asr #19
0046afe8: andeq r1, r0, r0, asr #19
0046afec: subeq r3, r5, r8, ror r4
0046aff0: strheq r2, [r6], #-0x54
0046aff4: subeq r2, r6, r0, asr #10
0046aff8: subeq r3, r5, r4, lsr #8
0046affc: subeq r2, r6, r0, ror #9
0046b000: subeq r2, r6, ip, ror #9

0x468cd4 _ZN14PlayerSavegame18__LoadCurrentFaeryEP11IStreamBasePv
00468cd4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00468cd8: ldr r3, [pc, #0xc0]
00468cdc: sub sp, sp, #0x14
00468ce0: ldr r8, [pc, #0xbc]
00468ce4: add r3, pc, r3
00468ce8: str r3, [sp, #8]
00468cec: ldr r3, [pc, #0xb4]
00468cf0: ldr fp, [pc, #0xb4]
00468cf4: ldr sl, [pc, #0xb4]
00468cf8: add r3, pc, r3
00468cfc: ldr sb, [pc, #0xb0]
00468d00: mov r6, r1
00468d04: mov r7, r0
00468d08: add fp, pc, fp
00468d0c: str r3, [sp, #0xc]
00468d10: mov r5, r1
00468d14: mov r4, #0
00468d18: add r8, pc, r8
00468d1c: ldr r3, [r5, #0x94]
00468d20: cmp r3, #0
00468d24: beq #0x468d50
00468d28: add r1, r6, r4, lsl #2
00468d2c: add r1, r1, #0xac
00468d30: add r4, r4, #1
00468d34: mov r0, r7
00468d38: bl #0x38b758
00468d3c: cmp r4, #3
00468d40: add r5, r5, #4
00468d44: bne #0x468d1c
00468d48: add sp, sp, #0x14
00468d4c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00468d50: ldr r2, [r8, sl]
00468d54: ldr r2, [r2]
00468d58: cmp r2, #2
00468d5c: beq #0x468d98
00468d60: cmp r2, #1
00468d64: bne #0x468d48
00468d68: ldr r0, [r8, sb]
00468d6c: ldr r3, [sp, #0xc]
00468d70: movw ip, #0x245
00468d74: mov r1, fp
00468d78: ldr r2, [sp, #8]
00468d7c: add r0, r0, #0xa8
00468d80: str ip, [sp]
00468d84: bl #0x30e004
00468d88: ldr r3, [r5, #0x94]
00468d8c: cmp r3, #0
00468d90: bne #0x468d28
00468d94: b #0x468d48
00468d98: str r3, [r3]
00468d9c: b #0x468d48
00468da0: subeq r4, r6, ip, lsl r8
00468da4: subseq fp, r2, r8, ror sp
00468da8: strheq r4, [r6], #-0x70
00468dac: ldrdeq r5, r6, [r5], #-0x60
00468db0: andeq r3, r0, r0, asr #19
00468db4: andeq r1, r0, r0, asr #19

0x464b2c _ZN14PlayerSavegame7SG_SaveEv
00464b2c: push {r4, r5, r6, lr}
00464b30: ldr r3, [r0, #8]
00464b34: ldr r6, [pc, #0x158]
00464b38: sub sp, sp, #8
00464b3c: cmp r3, #0
00464b40: mov r4, r0
00464b44: add r6, pc, r6
00464b48: beq #0x464b58
00464b4c: ldrb r3, [r0, #0xc]
00464b50: cmp r3, #0
00464b54: beq #0x464b60
00464b58: add sp, sp, #8
00464b5c: pop {r4, r5, r6, pc}
00464b60: bl #0x7fd794
00464b64: ldrb r3, [r0, #5]
00464b68: cmp r3, #0
00464b6c: bne #0x464ba0
00464b70: mov r3, #1
00464b74: str r3, [r4, #0x178]
00464b78: bl #0x7fd794
00464b7c: ldrb r3, [r0, #5]
00464b80: cmp r3, #0
00464b84: bne #0x464bd4
00464b88: ldr r0, [r4, #8]
00464b8c: bl #0x315fb8
00464b90: mov r0, r4
00464b94: add sp, sp, #8
00464b98: pop {r4, r5, r6, lr}
00464b9c: b #0x4684f0
00464ba0: ldr r3, [pc, #0xf0]
00464ba4: ldr r5, [r6, r3]
00464ba8: ldr r0, [r5, #0x40]
00464bac: bl #0x36f074
00464bb0: cmp r0, #0
00464bb4: beq #0x464b70
00464bb8: ldr r3, [r5, #0x40]
00464bbc: ldrb r3, [r3, #0x719]
00464bc0: cmp r3, #0
00464bc4: bne #0x464b70
00464bc8: mov r3, #2
00464bcc: str r3, [r4, #0x178]
00464bd0: b #0x464b78
00464bd4: ldr r3, [pc, #0xbc]
00464bd8: ldr r5, [r6, r3]
00464bdc: ldr r0, [r5, #0x40]
00464be0: bl #0x36f074
00464be4: cmp r0, #0
00464be8: bne #0x464c74
00464bec: mov r1, #1
00464bf0: mov r0, r4
00464bf4: bl #0x463654
00464bf8: mov r5, r0
00464bfc: mov r1, #1
00464c00: mov r0, r4
00464c04: mov r2, r5
00464c08: bl #0x468630
00464c0c: ldr r0, [r4, #8]
00464c10: ldr r1, [r0, #0x1c]
00464c14: cmp r1, #0
00464c18: beq #0x464c88
00464c1c: bl #0x315fb8
00464c20: mov r0, r4
00464c24: mov r1, #0
00464c28: mov r2, r5
00464c2c: bl #0x468630
00464c30: cmp r5, #0
00464c34: beq #0x464b90
00464c38: ldr r3, [pc, #0x5c]
00464c3c: mov r5, #0
00464c40: ldr r0, [r4, #4]
00464c44: ldr r3, [r6, r3]
00464c48: mov r1, r5
00464c4c: mov ip, #1
00464c50: ldr r3, [r3]
00464c54: mov r2, r5
00464c58: str ip, [sp]
00464c5c: str r5, [sp, #4]
00464c60: bl #0x4626f4
00464c64: mov r1, r5
00464c68: ldr r0, [r4, #4]
00464c6c: bl #0x464a68
00464c70: b #0x464b90
00464c74: ldr r3, [r5, #0x40]
00464c78: ldrb r3, [r3, #0x719]
00464c7c: cmp r3, #0
00464c80: beq #0x464b88
00464c84: b #0x464bec
00464c88: bl #0x315ad0
00464c8c: ldr r0, [r4, #8]
00464c90: b #0x464c1c
00464c94: subseq pc, r2, ip, asr #30
00464c98: strdeq r3, r4, [r0], -r4
00464c9c: muleq r0, ip, sl

0x46403c _ZN14PlayerSavegame18SG_GetSavegameListEb
0046403c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00464040: ldr r8, [pc, #0x274]
00464044: ldr r2, [pc, #0x274]
00464048: ldr sl, [pc, #0x274]
0046404c: sub sp, sp, #0x34
00464050: add r8, pc, r8
00464054: str r2, [sp, #8]
00464058: ldr r2, [r8, r2]
0046405c: ldr r3, [r8, sl]
00464060: mov r6, r0
00464064: mov r0, #0
00464068: ldr r2, [r2]
0046406c: str r0, [r6, #8]
00464070: str r0, [r6]
00464074: str r0, [r6, #4]
00464078: ldr r3, [r3, #0x10]
0046407c: str r2, [sp, #0x2c]
00464080: str r1, [sp, #4]
00464084: ldr r3, [r3, #0x34]
00464088: cmp r3, r0
0046408c: beq #0x464178
00464090: ldr r1, [pc, #0x230]
00464094: mov r0, r3
00464098: mov r2, r6
0046409c: ldr r3, [r3]
004640a0: add r1, pc, r1
004640a4: mov lr, pc
004640a8: ldr pc, [r3, #0x7c]
004640ac: ldr r3, [pc, #0x218]
004640b0: ldr r5, [pc, #0x218]
004640b4: ldr sb, [pc, #0x218]
004640b8: add r3, pc, r3
004640bc: str r3, [sp, #0xc]
004640c0: add r5, pc, r5
004640c4: add sb, pc, sb
004640c8: ldr fp, [r6]
004640cc: add r4, sp, #0x10
004640d0: b #0x4640e8
004640d4: mov r1, fp
004640d8: mov r0, r6
004640dc: mov r2, r4
004640e0: bl #0x3303ec
004640e4: mov fp, r0
004640e8: ldr r3, [r6, #4]
004640ec: cmp fp, r3
004640f0: beq #0x46416c
004640f4: ldr r7, [fp, #0x14]
004640f8: mov r1, r5
004640fc: mov r0, r7
00464100: bl #0x30ebd4
00464104: cmp r0, #0
00464108: bne #0x4640d4
0046410c: mov r0, r7
00464110: mov r1, sb
00464114: bl #0x30ebd4
00464118: cmp r0, #0
0046411c: bne #0x4640d4
00464120: mov r0, r7
00464124: ldr r1, [sp, #0xc]
00464128: bl #0x30ebd4
0046412c: cmp r0, #0
00464130: bne #0x4640d4
00464134: bl #0x4634f4
00464138: mov r1, r0
0046413c: mov r0, r7
00464140: bl #0x30ebd4
00464144: cmp r0, #0
00464148: beq #0x4640d4
0046414c: bl #0x4634e4
00464150: mov r1, r0
00464154: mov r0, r7
00464158: bl #0x30ebd4
0046415c: cmp r0, #0
00464160: beq #0x4640d4
00464164: add fp, fp, #0x18
00464168: b #0x4640e8
0046416c: ldr r3, [sp, #4]
00464170: cmp r3, #0
00464174: bne #0x46419c
00464178: ldr r2, [sp, #8]
0046417c: mov r0, r6
00464180: ldr r3, [r8, r2]
00464184: ldr r2, [sp, #0x2c]
00464188: ldr r3, [r3]
0046418c: cmp r2, r3
00464190: bne #0x4642b8
00464194: add sp, sp, #0x34
00464198: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046419c: ldr r2, [r6]
004641a0: rsb fp, r2, fp
004641a4: asr r3, fp, #3
004641a8: add r1, r3, r3, lsl #2
004641ac: add r1, r1, r1, lsl #4
004641b0: add r1, r1, r1, lsl #8
004641b4: add r1, r1, r1, lsl #16
004641b8: add r3, r3, r1, lsl #1
004641bc: cmp r3, #0
004641c0: beq #0x464178
004641c4: mov r1, #0
004641c8: str r1, [sp, #4]
004641cc: add sb, sp, #0x14
004641d0: mov r4, #0
004641d4: mov r7, r4
004641d8: b #0x4641e8
004641dc: ldm r6, {r2, fp}
004641e0: mov r4, r5
004641e4: rsb fp, r2, fp
004641e8: asr r3, fp, #3
004641ec: add r1, r3, r3, lsl #2
004641f0: add r1, r1, r1, lsl #4
004641f4: add r1, r1, r1, lsl #8
004641f8: add r1, r1, r1, lsl #16
004641fc: add r3, r3, r1, lsl #1
00464200: sub r1, r3, #1
00464204: cmp r1, r7
00464208: bls #0x4642a0
0046420c: ldr r3, [r8, sl]
00464210: add r5, r4, #0x18
00464214: add r0, r2, r4
00464218: ldr r3, [r3, #0x10]
0046421c: add r2, r2, r5
00464220: ldr r1, [r2, #0x14]
00464224: ldr r3, [r3, #0x34]
00464228: ldr r2, [r0, #0x14]
0046422c: add r7, r7, #1
00464230: mov r0, r3
00464234: ldr r3, [r3]
00464238: mov lr, pc
0046423c: ldr pc, [r3, #0xb4]
00464240: cmp r0, #0
00464244: beq #0x4641dc
00464248: ldr r1, [r6]
0046424c: mov r0, sb
00464250: add r1, r1, r4
00464254: bl #0x32b918
00464258: ldr r3, [r6]
0046425c: add r0, r3, r4
00464260: add r3, r3, r5
00464264: cmp r0, r3
00464268: beq #0x464280
0046426c: ldr r2, [r3, #0x10]
00464270: ldr r1, [r3, #0x14]
00464274: bl #0x3109e0
00464278: ldr r0, [r6]
0046427c: add r0, r0, r5
00464280: cmp r0, sb
00464284: beq #0x464294
00464288: ldr r1, [sp, #0x28]
0046428c: ldr r2, [sp, #0x24]
00464290: bl #0x3109e0
00464294: mov r0, sb
00464298: bl #0x3139ac
0046429c: b #0x4641dc
004642a0: ldr r1, [sp, #4]
004642a4: add r1, r1, #1
004642a8: cmp r1, r3
004642ac: str r1, [sp, #4]
004642b0: blo #0x4641d0
004642b4: b #0x464178
004642b8: bl #0x30e310
004642bc: subseq r0, r3, r0, asr #20
004642c0: andeq r4, r0, ip, lsr #1
004642c4: strdeq r3, r4, [r0], -r4
004642c8: subeq sb, r6, r8, lsr #1
004642cc: subeq fp, sl, r0, lsr #17
004642d0: subeq sl, r5, r0, lsr #9
004642d4: subeq sb, r6, r4, lsl r1

0x466184 _ZN14PlayerSavegame17SG_SaveCheckpointEv
00466184: push {r4, r5, r6, r7, r8, sl, lr}
00466188: ldr r4, [pc, #0x194]
0046618c: ldr r6, [pc, #0x194]
00466190: ldr r2, [r0, #8]
00466194: add r4, pc, r4
00466198: ldr r3, [r4, r6]
0046619c: sub sp, sp, #0x24
004661a0: cmp r2, #0
004661a4: ldr r3, [r3]
004661a8: mov r5, r0
004661ac: str r3, [sp, #0x1c]
004661b0: beq #0x4661c0
004661b4: ldrb r8, [r0, #0xc]
004661b8: cmp r8, #0
004661bc: beq #0x4661dc
004661c0: ldr r3, [r4, r6]
004661c4: ldr r2, [sp, #0x1c]
004661c8: ldr r3, [r3]
004661cc: cmp r2, r3
004661d0: bne #0x466320
004661d4: add sp, sp, #0x24
004661d8: pop {r4, r5, r6, r7, r8, sl, pc}
004661dc: add r7, sp, #4
004661e0: mov r0, r7
004661e4: mov r1, #0x10
004661e8: str r7, [sp, #0x14]
004661ec: str r7, [sp, #0x18]
004661f0: bl #0x31167c
004661f4: ldr r3, [sp, #0x14]
004661f8: strb r8, [r3]
004661fc: ldr r8, [r5, #4]
00466200: bl #0x7fd794
00466204: ldrb r3, [r0, #5]
00466208: cmp r3, #0
0046620c: bne #0x4662e4
00466210: mov r3, #0
00466214: mov r0, r8
00466218: mov r1, r7
0046621c: mov r2, #1
00466220: bl #0x463c84
00466224: ldr r8, [sp, #0x18]
00466228: mov r0, r8
0046622c: bl #0x30de54
00466230: ldr r3, [r5, #8]
00466234: add r2, r8, r0
00466238: mov r1, r8
0046623c: add r0, r3, #4
00466240: bl #0x3109e0
00466244: bl #0x7fd794
00466248: ldrb r3, [r0, #5]
0046624c: cmp r3, #0
00466250: beq #0x466294
00466254: mov r3, #2
00466258: mov r0, r5
0046625c: mov r1, #1
00466260: str r3, [r5, #0x178]
00466264: mov r2, #0
00466268: bl #0x468630
0046626c: ldr r0, [r5, #8]
00466270: ldr r1, [r0, #0x1c]
00466274: cmp r1, #0
00466278: beq #0x466314
0046627c: bl #0x315fb8
00466280: mov r1, #0
00466284: mov r0, r5
00466288: mov r2, r1
0046628c: bl #0x468630
00466290: b #0x4662a4
00466294: mov r3, #1
00466298: str r3, [r5, #0x178]
0046629c: ldr r0, [r5, #8]
004662a0: bl #0x315fb8
004662a4: mov r2, #0
004662a8: mov r3, r2
004662ac: mov r1, r7
004662b0: ldr r0, [r5, #4]
004662b4: bl #0x463c84
004662b8: ldr r8, [sp, #0x18]
004662bc: mov r0, r8
004662c0: bl #0x30de54
004662c4: ldr r3, [r5, #8]
004662c8: add r2, r8, r0
004662cc: mov r1, r8
004662d0: add r0, r3, #4
004662d4: bl #0x3109e0
004662d8: mov r0, r7
004662dc: bl #0x3139ac
004662e0: b #0x4661c0
004662e4: ldr r3, [pc, #0x40]
004662e8: ldr sl, [r4, r3]
004662ec: ldr r0, [sl, #0x40]
004662f0: bl #0x36f074
004662f4: cmp r0, #0
004662f8: beq #0x46630c
004662fc: ldr r3, [sl, #0x40]
00466300: ldrb r3, [r3, #0x719]
00466304: cmp r3, #0
00466308: beq #0x466210
0046630c: mov r3, #1
00466310: b #0x466214
00466314: bl #0x315ad0
00466318: ldr r0, [r5, #8]
0046631c: b #0x46627c
00466320: bl #0x30e310
00466324: ldrsheq lr, [r2], #-0x8c
00466328: andeq r4, r0, ip, lsr #1
0046632c: strdeq r3, r4, [r0], -r4

0x46a9cc _ZN14PlayerSavegame17__LoadLevelStatesEP11IStreamBasePv
0046a9cc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a9d0: ldr r4, [pc, #0x264]
0046a9d4: ldr r2, [pc, #0x264]
0046a9d8: sub sp, sp, #0x4c
0046a9dc: add r4, pc, r4
0046a9e0: ldr r3, [r4, r2]
0046a9e4: cmn r1, #0x68
0046a9e8: str r2, [sp, #0x1c]
0046a9ec: ldr r3, [r3]
0046a9f0: str r1, [sp, #0x14]
0046a9f4: mov r5, r0
0046a9f8: str r3, [sp, #0x44]
0046a9fc: beq #0x46abf8
0046aa00: cmn r1, #0x74
0046aa04: beq #0x46abf8
0046aa08: add r7, sp, #0x2c
0046aa0c: mov r0, r7
0046aa10: mov r1, #0x10
0046aa14: str r7, [sp, #0x3c]
0046aa18: str r7, [sp, #0x40]
0046aa1c: bl #0x31167c
0046aa20: ldr r2, [pc, #0x21c]
0046aa24: ldr r3, [pc, #0x21c]
0046aa28: str r2, [sp, #4]
0046aa2c: ldr r2, [sp, #0x3c]
0046aa30: str r3, [sp, #0xc]
0046aa34: mov r3, #0
0046aa38: strb r3, [r2]
0046aa3c: str r3, [sp, #0x10]
0046aa40: add r2, sp, #0x28
0046aa44: add r3, sp, #0x24
0046aa48: str r2, [sp, #0x18]
0046aa4c: str r3, [sp, #8]
0046aa50: mov r0, r5
0046aa54: ldr r1, [sp, #0x18]
0046aa58: bl #0x38b758
0046aa5c: ldr r3, [sp, #0x28]
0046aa60: cmp r3, #0
0046aa64: ble #0x46aafc
0046aa68: mov r6, #0
0046aa6c: mov r0, r5
0046aa70: mov r1, r7
0046aa74: bl #0x461da8
0046aa78: ldr r2, [sp, #4]
0046aa7c: ldr sb, [sp, #0x40]
0046aa80: ldr r3, [r4, r2]
0046aa84: ldr sl, [r3]
0046aa88: cmp sl, #0
0046aa8c: beq #0x46ac28
0046aa90: ldr r2, [sp, #0xc]
0046aa94: mov r8, #0
0046aa98: ldr r3, [r4, r2]
0046aa9c: ldr fp, [r3]
0046aaa0: b #0x46aab0
0046aaa4: add r8, r8, #1
0046aaa8: cmp r8, sl
0046aaac: beq #0x46ac28
0046aab0: mov r0, sb
0046aab4: ldr r1, [fp, r8, lsl #2]
0046aab8: bl #0x30e31c
0046aabc: cmp r0, #0
0046aac0: bne #0x46aaa4
0046aac4: mov r0, r5
0046aac8: ldr r1, [sp, #8]
0046aacc: bl #0x38b758
0046aad0: cmn r8, #1
0046aad4: beq #0x46aaec
0046aad8: mov r1, r8
0046aadc: ldr r0, [sp, #0x14]
0046aae0: ldr r2, [sp, #0x24]
0046aae4: ldr r3, [sp, #0x10]
0046aae8: bl #0x466e48
0046aaec: ldr r3, [sp, #0x28]
0046aaf0: add r6, r6, #1
0046aaf4: cmp r3, r6
0046aaf8: bgt #0x46aa6c
0046aafc: ldr r3, [sp, #0x10]
0046ab00: add r3, r3, #1
0046ab04: cmp r3, #3
0046ab08: str r3, [sp, #0x10]
0046ab0c: bne #0x46aa50
0046ab10: ldr r2, [pc, #0x134]
0046ab14: ldr r3, [pc, #0x134]
0046ab18: str r2, [sp, #8]
0046ab1c: str r3, [sp, #0xc]
0046ab20: mov r2, #0
0046ab24: add r3, sp, #0x24
0046ab28: str r2, [sp, #0x10]
0046ab2c: str r3, [sp, #4]
0046ab30: mov r0, r5
0046ab34: ldr r1, [sp, #0x18]
0046ab38: bl #0x38b758
0046ab3c: ldr r3, [sp, #0x28]
0046ab40: cmp r3, #0
0046ab44: ble #0x46abdc
0046ab48: mov r6, #0
0046ab4c: mov r0, r5
0046ab50: mov r1, r7
0046ab54: bl #0x461da8
0046ab58: ldr r2, [sp, #8]
0046ab5c: ldr sb, [sp, #0x40]
0046ab60: ldr r3, [r4, r2]
0046ab64: ldr sl, [r3]
0046ab68: cmp sl, #0
0046ab6c: beq #0x46ac18
0046ab70: ldr r2, [sp, #0xc]
0046ab74: mov r8, #0
0046ab78: ldr r3, [r4, r2]
0046ab7c: ldr fp, [r3]
0046ab80: b #0x46ab90
0046ab84: add r8, r8, #1
0046ab88: cmp r8, sl
0046ab8c: beq #0x46ac18
0046ab90: mov r0, sb
0046ab94: ldr r1, [fp, r8, lsl #2]
0046ab98: bl #0x30e31c
0046ab9c: cmp r0, #0
0046aba0: bne #0x46ab84
0046aba4: mov r0, r5
0046aba8: ldr r1, [sp, #4]
0046abac: bl #0x38b758
0046abb0: cmn r8, #1
0046abb4: beq #0x46abcc
0046abb8: mov r1, r8
0046abbc: ldr r0, [sp, #0x14]
0046abc0: ldr r2, [sp, #0x24]
0046abc4: ldr r3, [sp, #0x10]
0046abc8: bl #0x466b18
0046abcc: ldr r3, [sp, #0x28]
0046abd0: add r6, r6, #1
0046abd4: cmp r3, r6
0046abd8: bgt #0x46ab4c
0046abdc: ldr r3, [sp, #0x10]
0046abe0: add r3, r3, #1
0046abe4: cmp r3, #3
0046abe8: str r3, [sp, #0x10]
0046abec: bne #0x46ab30
0046abf0: mov r0, r7
0046abf4: bl #0x3139ac
0046abf8: ldr r2, [sp, #0x1c]
0046abfc: ldr r3, [r4, r2]
0046ac00: ldr r2, [sp, #0x44]
0046ac04: ldr r3, [r3]
0046ac08: cmp r2, r3
0046ac0c: bne #0x46ac38
0046ac10: add sp, sp, #0x4c
0046ac14: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046ac18: mov r0, r5
0046ac1c: ldr r1, [sp, #4]
0046ac20: bl #0x38b758
0046ac24: b #0x46abcc
0046ac28: mov r0, r5
0046ac2c: ldr r1, [sp, #8]
0046ac30: bl #0x38b758
0046ac34: b #0x46aaec
0046ac38: bl #0x30e310
0046ac3c: ldrheq sl, [r2], #-4
0046ac40: andeq r4, r0, ip, lsr #1
0046ac44: andeq r1, r0, r0, asr #17
0046ac48: andeq r3, r0, ip, asr fp
0046ac4c: andeq r2, r0, r4, ror r2
0046ac50: andeq r2, r0, r8, ror #6

0x38b808 _ZN11IStreamBase7writeAsIiEEvRKT_
0038b808: str lr, [sp, #-4]!
0038b80c: mov r3, #0
0038b810: sub sp, sp, #0xc
0038b814: ldr ip, [r0]
0038b818: mov r2, #4
0038b81c: mov lr, pc
0038b820: ldr pc, [ip, #0x1c]
0038b824: ldr r3, [pc, #0x74]
0038b828: cmp r0, #4
0038b82c: add r3, pc, r3
0038b830: beq #0x38b860
0038b834: ldr r2, [pc, #0x68]
0038b838: ldr r2, [r3, r2]
0038b83c: ldr r2, [r2]
0038b840: cmp r2, #2
0038b844: moveq r3, #0
0038b848: streq r3, [r3]
0038b84c: beq #0x38b858
0038b850: cmp r2, #1
0038b854: beq #0x38b86c
0038b858: add sp, sp, #0xc
0038b85c: ldm sp!, {pc}
0038b860: cmp r1, #0
0038b864: beq #0x38b858
0038b868: b #0x38b834
0038b86c: ldr r0, [pc, #0x34]
0038b870: ldr r1, [pc, #0x34]
0038b874: ldr r2, [pc, #0x34]
0038b878: ldr r0, [r3, r0]
0038b87c: ldr r3, [pc, #0x30]
0038b880: mov ip, #0x4d
0038b884: add r1, pc, r1
0038b888: add r2, pc, r2
0038b88c: add r3, pc, r3
0038b890: add r0, r0, #0xa8
0038b894: str ip, [sp]
0038b898: bl #0x30e004
0038b89c: b #0x38b858
0038b8a0: rsbeq sb, r0, r4, ror #4
0038b8a4: andeq r3, r0, r0, asr #19
0038b8a8: andeq r1, r0, r0, asr #19
0038b8ac: subseq r2, r3, r4, asr fp
0038b8b0: subseq r2, r3, r8, lsl ip
0038b8b4: subseq r2, r3, ip, lsl #25

0x461770 _ZN11IStreamBase7writeAsIjEEvRKT_
00461770: str lr, [sp, #-4]!
00461774: mov r3, #0
00461778: sub sp, sp, #0xc
0046177c: ldr ip, [r0]
00461780: mov r2, #4
00461784: mov lr, pc
00461788: ldr pc, [ip, #0x1c]
0046178c: ldr r3, [pc, #0x74]
00461790: cmp r0, #4
00461794: add r3, pc, r3
00461798: beq #0x4617c8
0046179c: ldr r2, [pc, #0x68]
004617a0: ldr r2, [r3, r2]
004617a4: ldr r2, [r2]
004617a8: cmp r2, #2
004617ac: moveq r3, #0
004617b0: streq r3, [r3]
004617b4: beq #0x4617c0
004617b8: cmp r2, #1
004617bc: beq #0x4617d4
004617c0: add sp, sp, #0xc
004617c4: ldm sp!, {pc}
004617c8: cmp r1, #0
004617cc: beq #0x4617c0
004617d0: b #0x46179c
004617d4: ldr r0, [pc, #0x34]
004617d8: ldr r1, [pc, #0x34]
004617dc: ldr r2, [pc, #0x34]
004617e0: ldr r0, [r3, r0]
004617e4: ldr r3, [pc, #0x30]
004617e8: mov ip, #0x4d
004617ec: add r1, pc, r1
004617f0: add r2, pc, r2
004617f4: add r3, pc, r3
004617f8: add r0, r0, #0xa8
004617fc: str ip, [sp]
00461800: bl #0x30e004
00461804: b #0x4617c0
00461808: ldrsheq r3, [r3], #-0x2c
0046180c: andeq r3, r0, r0, asr #19
00461810: andeq r1, r0, r0, asr #19
00461814: subeq ip, r5, ip, ror #23
00461818: strheq ip, [r5], #-0xc0
0046181c: subeq ip, r5, r4, lsr #26

0x468db8 _ZN11IStreamBase7writeAsItEEvRKT_
00468db8: str lr, [sp, #-4]!
00468dbc: mov r3, #0
00468dc0: sub sp, sp, #0xc
00468dc4: ldr ip, [r0]
00468dc8: mov r2, #2
00468dcc: mov lr, pc
00468dd0: ldr pc, [ip, #0x1c]
00468dd4: ldr r3, [pc, #0x74]
00468dd8: cmp r0, #2
00468ddc: add r3, pc, r3
00468de0: beq #0x468e10
00468de4: ldr r2, [pc, #0x68]
00468de8: ldr r2, [r3, r2]
00468dec: ldr r2, [r2]
00468df0: cmp r2, #2
00468df4: moveq r3, #0
00468df8: streq r3, [r3]
00468dfc: beq #0x468e08
00468e00: cmp r2, #1
00468e04: beq #0x468e1c
00468e08: add sp, sp, #0xc
00468e0c: ldm sp!, {pc}
00468e10: cmp r1, #0
00468e14: beq #0x468e08
00468e18: b #0x468de4
00468e1c: ldr r0, [pc, #0x34]
00468e20: ldr r1, [pc, #0x34]
00468e24: ldr r2, [pc, #0x34]
00468e28: ldr r0, [r3, r0]
00468e2c: ldr r3, [pc, #0x30]
00468e30: mov ip, #0x4d
00468e34: add r1, pc, r1
00468e38: add r2, pc, r2
00468e3c: add r3, pc, r3
00468e40: add r0, r0, #0xa8
00468e44: str ip, [sp]
00468e48: bl #0x30e004
00468e4c: b #0x468e08
00468e50: ldrheq fp, [r2], #-0xc4
00468e54: andeq r3, r0, r0, asr #19
00468e58: andeq r1, r0, r0, asr #19
00468e5c: subeq r5, r5, r4, lsr #11
00468e60: subeq r5, r5, r8, ror #12
00468e64: ldrdeq r5, r6, [r5], #-0x6c

0x468e68 _ZN11IStreamBase7writeAsIaEEvRKT_
00468e68: str lr, [sp, #-4]!
00468e6c: mov r3, #0
00468e70: sub sp, sp, #0xc
00468e74: ldr ip, [r0]
00468e78: mov r2, #1
00468e7c: mov lr, pc
00468e80: ldr pc, [ip, #0x1c]
00468e84: ldr r3, [pc, #0x74]
00468e88: cmp r0, #1
00468e8c: add r3, pc, r3
00468e90: beq #0x468ec0
00468e94: ldr r2, [pc, #0x68]
00468e98: ldr r2, [r3, r2]
00468e9c: ldr r2, [r2]
00468ea0: cmp r2, #2
00468ea4: moveq r3, #0
00468ea8: streq r3, [r3]
00468eac: beq #0x468eb8
00468eb0: cmp r2, #1
00468eb4: beq #0x468ecc
00468eb8: add sp, sp, #0xc
00468ebc: ldm sp!, {pc}
00468ec0: cmp r1, #0
00468ec4: beq #0x468eb8
00468ec8: b #0x468e94
00468ecc: ldr r0, [pc, #0x34]
00468ed0: ldr r1, [pc, #0x34]
00468ed4: ldr r2, [pc, #0x34]
00468ed8: ldr r0, [r3, r0]
00468edc: ldr r3, [pc, #0x30]
00468ee0: mov ip, #0x4d
00468ee4: add r1, pc, r1
00468ee8: add r2, pc, r2
00468eec: add r3, pc, r3
00468ef0: add r0, r0, #0xa8
00468ef4: str ip, [sp]
00468ef8: bl #0x30e004
00468efc: b #0x468eb8
00468f00: subseq fp, r2, r4, lsl #24
00468f04: andeq r3, r0, r0, asr #19
00468f08: andeq r1, r0, r0, asr #19
00468f0c: strdeq r5, r6, [r5], #-0x44
00468f10: strheq r5, [r5], #-0x58
00468f14: subeq r5, r5, ip, lsr #12

0x46b0a4 _ZN13QuestSavegameC1Ev
0046b0a4: ldr r3, [pc, #0x90]
0046b0a8: ldr r2, [pc, #0x90]
0046b0ac: mov r1, #0
0046b0b0: add r3, pc, r3
0046b0b4: ldr r2, [r3, r2]
0046b0b8: push {r4, r5, r6}
0046b0bc: mov ip, r1
0046b0c0: add r5, r0, #0x10
0046b0c4: add r4, r0, #0x1c
0046b0c8: add r2, r2, #8
0046b0cc: str r2, [r0]
0046b0d0: str r1, [r0, #4]
0046b0d4: str r1, [r0, #8]
0046b0d8: str r1, [r0, #0xc]
0046b0dc: str r1, [r0, #0x10]
0046b0e0: mov r2, r0
0046b0e4: str r1, [r5, #8]
0046b0e8: str r1, [r5, #4]
0046b0ec: mov r6, ip
0046b0f0: str r1, [r0, #0x1c]
0046b0f4: mvn r5, #0
0046b0f8: str r1, [r4, #8]
0046b0fc: str r1, [r4, #4]
0046b100: str r1, [r0, #0x5c]
0046b104: mov r4, #1
0046b108: mov r1, r0
0046b10c: add ip, ip, #1
0046b110: cmp ip, #3
0046b114: str r5, [r2, #0x2c]
0046b118: str r5, [r2, #0x38]
0046b11c: str r4, [r2, #0x44]
0046b120: str r4, [r2, #0x50]
0046b124: strb r6, [r1, #0x28]
0046b128: add r2, r2, #4
0046b12c: add r1, r1, #1
0046b130: bne #0x46b10c
0046b134: pop {r4, r5, r6}
0046b138: bx lr
0046b13c: subseq sb, r2, r0, ror #19
0046b140: andeq r3, r0, r4, ror #9

0x465430 _ZN14PlayerSavegame7SG_LoadEi
00465430: push {r4, r5, r6, lr}
00465434: mov r5, r0
00465438: mov r4, r1
0046543c: bl #0x464f4c
00465440: mov r0, r5
00465444: mov r1, r4
00465448: pop {r4, r5, r6, lr}
0046544c: b #0x468574

0x461668 _ZN11IStreamBase7writeAsERKSs
00461668: push {r4, r5, lr}
0046166c: ldr r2, [r1, #0x10]
00461670: ldr r3, [r1, #0x14]
00461674: sub sp, sp, #0xc
00461678: mov r5, r1
0046167c: rsb r3, r3, r2
00461680: add r1, sp, #8
00461684: add r3, r3, #1
00461688: str r3, [r1, #-4]!
0046168c: mov r4, r0
00461690: bl #0x38b808
00461694: ldr r2, [sp, #4]
00461698: mov r0, r4
0046169c: ldr r1, [r5, #0x14]
004616a0: asr r3, r2, #0x1f
004616a4: ldr ip, [r4]
004616a8: mov lr, pc
004616ac: ldr pc, [ip, #0x1c]
004616b0: add sp, sp, #0xc
004616b4: pop {r4, r5, pc}

0x33e138 _ZN11IStreamBase7writeAsIbEEvRKT_
0033e138: str lr, [sp, #-4]!
0033e13c: mov r3, #0
0033e140: sub sp, sp, #0xc
0033e144: ldr ip, [r0]
0033e148: mov r2, #1
0033e14c: mov lr, pc
0033e150: ldr pc, [ip, #0x1c]
0033e154: ldr r3, [pc, #0x74]
0033e158: cmp r0, #1
0033e15c: add r3, pc, r3
0033e160: beq #0x33e190
0033e164: ldr r2, [pc, #0x68]
0033e168: ldr r2, [r3, r2]
0033e16c: ldr r2, [r2]
0033e170: cmp r2, #2
0033e174: moveq r3, #0
0033e178: streq r3, [r3]
0033e17c: beq #0x33e188
0033e180: cmp r2, #1
0033e184: beq #0x33e19c
0033e188: add sp, sp, #0xc
0033e18c: ldm sp!, {pc}
0033e190: cmp r1, #0
0033e194: beq #0x33e188
0033e198: b #0x33e164
0033e19c: ldr r0, [pc, #0x34]
0033e1a0: ldr r1, [pc, #0x34]
0033e1a4: ldr r2, [pc, #0x34]
0033e1a8: ldr r0, [r3, r0]
0033e1ac: ldr r3, [pc, #0x30]
0033e1b0: mov ip, #0x4d
0033e1b4: add r1, pc, r1
0033e1b8: add r2, pc, r2
0033e1bc: add r3, pc, r3
0033e1c0: add r0, r0, #0xa8
0033e1c4: str ip, [sp]
0033e1c8: bl #0x30e004
0033e1cc: b #0x33e188
0033e1d0: rsbeq r6, r5, r4, lsr sb
0033e1d4: andeq r3, r0, r0, asr #19
0033e1d8: andeq r1, r0, r0, asr #19
0033e1dc: subseq r0, r8, r4, lsr #4
0033e1e0: subseq r0, r8, r8, ror #5
0033e1e4: subseq r0, r8, ip, asr r3

0x46c6fc _ZN13QuestSavegame10SaveQuestsEP11IStreamBase
0046c6fc: push {r4, r5, r6, lr}
0046c700: mov r4, r1
0046c704: mov r5, r0
0046c708: mov r2, r4
0046c70c: mov r1, #0
0046c710: bl #0x46c658
0046c714: mov r0, r5
0046c718: mov r2, r4
0046c71c: mov r1, #1
0046c720: bl #0x46c658
0046c724: mov r0, r5
0046c728: mov r2, r4
0046c72c: mov r1, #2
0046c730: pop {r4, r5, r6, lr}
0046c734: b #0x46c658

0x39f828 _ZN11IStreamBase7writeAsIhEEvRKT_
0039f828: str lr, [sp, #-4]!
0039f82c: mov r3, #0
0039f830: sub sp, sp, #0xc
0039f834: ldr ip, [r0]
0039f838: mov r2, #1
0039f83c: mov lr, pc
0039f840: ldr pc, [ip, #0x1c]
0039f844: ldr r3, [pc, #0x74]
0039f848: cmp r0, #1
0039f84c: add r3, pc, r3
0039f850: beq #0x39f880
0039f854: ldr r2, [pc, #0x68]
0039f858: ldr r2, [r3, r2]
0039f85c: ldr r2, [r2]
0039f860: cmp r2, #2
0039f864: moveq r3, #0
0039f868: streq r3, [r3]
0039f86c: beq #0x39f878
0039f870: cmp r2, #1
0039f874: beq #0x39f88c
0039f878: add sp, sp, #0xc
0039f87c: ldm sp!, {pc}
0039f880: cmp r1, #0
0039f884: beq #0x39f878
0039f888: b #0x39f854
0039f88c: ldr r0, [pc, #0x34]
0039f890: ldr r1, [pc, #0x34]
0039f894: ldr r2, [pc, #0x34]
0039f898: ldr r0, [r3, r0]
0039f89c: ldr r3, [pc, #0x30]
0039f8a0: mov ip, #0x4d
0039f8a4: add r1, pc, r1
0039f8a8: add r2, pc, r2
0039f8ac: add r3, pc, r3
0039f8b0: add r0, r0, #0xa8
0039f8b4: str ip, [sp]
0039f8b8: bl #0x30e004
0039f8bc: b #0x39f878
0039f8c0: subseq r5, pc, r4, asr #4
0039f8c4: andeq r3, r0, r0, asr #19
0039f8c8: andeq r1, r0, r0, asr #19
0039f8cc: subseq lr, r1, r4, lsr fp
0039f8d0: ldrsheq lr, [r1], #-0xb8
0039f8d4: subseq lr, r1, ip, ror #24

0x467040 _ZNK14PlayerSavegame16SG_GetLevelStateEii
00467040: push {r4, r5, r6, r7, lr}
00467044: ldr r4, [pc, #0xec]
00467048: subs r5, r1, #0
0046704c: sub sp, sp, #0xc
00467050: mov r6, r0
00467054: add r4, pc, r4
00467058: mov r7, r2
0046705c: blt #0x4670ac
00467060: ldr r3, [pc, #0xd4]
00467064: ldr r3, [r4, r3]
00467068: ldr r3, [r3]
0046706c: cmp r5, r3
00467070: blt #0x467098
00467074: ldr r3, [pc, #0xc4]
00467078: ldr r3, [r4, r3]
0046707c: ldr r3, [r3]
00467080: cmp r3, #2
00467084: moveq r3, #0
00467088: streq r3, [r3]
0046708c: beq #0x467098
00467090: cmp r3, #1
00467094: beq #0x467104
00467098: add r7, r7, #0x1a
0046709c: ldr r3, [r6, r7, lsl #2]
004670a0: ldr r0, [r3, r5, lsl #2]
004670a4: add sp, sp, #0xc
004670a8: pop {r4, r5, r6, r7, pc}
004670ac: ldr r3, [pc, #0x8c]
004670b0: ldr r3, [r4, r3]
004670b4: ldr r3, [r3]
004670b8: cmp r3, #2
004670bc: moveq r3, #0
004670c0: streq r3, [r3]
004670c4: beq #0x467060
004670c8: cmp r3, #1
004670cc: bne #0x467060
004670d0: ldr r0, [pc, #0x6c]
004670d4: ldr r1, [pc, #0x6c]
004670d8: ldr r2, [pc, #0x6c]
004670dc: ldr r0, [r4, r0]
004670e0: ldr r3, [pc, #0x68]
004670e4: mov ip, #0x51
004670e8: add r1, pc, r1
004670ec: add r2, pc, r2
004670f0: add r3, pc, r3
004670f4: add r0, r0, #0xa8
004670f8: str ip, [sp]
004670fc: bl #0x30e004
00467100: b #0x467060
00467104: ldr r0, [pc, #0x38]
00467108: ldr r1, [pc, #0x44]
0046710c: ldr r2, [pc, #0x44]
00467110: ldr r0, [r4, r0]
00467114: ldr r3, [pc, #0x40]
00467118: mov ip, #0x52
0046711c: add r1, pc, r1
00467120: add r2, pc, r2
00467124: add r3, pc, r3
00467128: add r0, r0, #0xa8
0046712c: str ip, [sp]
00467130: bl #0x30e004
00467134: b #0x467098
00467138: subseq sp, r2, ip, lsr sl
0046713c: andeq r1, r0, r0, asr #17
00467140: andeq r3, r0, r0, asr #19
00467144: andeq r1, r0, r0, asr #19
00467148: strdeq r7, r8, [r5], #-0x20
0046714c: subeq r6, r6, r4, lsl #5
00467150: umaaleq r6, r6, r0, r1
00467154: strheq r7, [r5], #-0x2c
00467158: subeq r6, r6, r0, ror #4
0046715c: subeq r6, r6, ip, asr r1

0x466d10 _ZNK14PlayerSavegame17SG_GetMapLocStateEii
00466d10: push {r4, r5, r6, r7, lr}
00466d14: ldr r4, [pc, #0xec]
00466d18: subs r5, r1, #0
00466d1c: sub sp, sp, #0xc
00466d20: mov r6, r0
00466d24: add r4, pc, r4
00466d28: mov r7, r2
00466d2c: blt #0x466d7c
00466d30: ldr r3, [pc, #0xd4]
00466d34: ldr r3, [r4, r3]
00466d38: ldr r3, [r3]
00466d3c: cmp r5, r3
00466d40: blt #0x466d68
00466d44: ldr r3, [pc, #0xc4]
00466d48: ldr r3, [r4, r3]
00466d4c: ldr r3, [r3]
00466d50: cmp r3, #2
00466d54: moveq r3, #0
00466d58: streq r3, [r3]
00466d5c: beq #0x466d68
00466d60: cmp r3, #1
00466d64: beq #0x466dd4
00466d68: add r6, r6, r7, lsl #2
00466d6c: ldr r3, [r6, #0x74]
00466d70: ldr r0, [r3, r5, lsl #2]
00466d74: add sp, sp, #0xc
00466d78: pop {r4, r5, r6, r7, pc}
00466d7c: ldr r3, [pc, #0x8c]
00466d80: ldr r3, [r4, r3]
00466d84: ldr r3, [r3]
00466d88: cmp r3, #2
00466d8c: moveq r3, #0
00466d90: streq r3, [r3]
00466d94: beq #0x466d30
00466d98: cmp r3, #1
00466d9c: bne #0x466d30
00466da0: ldr r0, [pc, #0x6c]
00466da4: ldr r1, [pc, #0x6c]
00466da8: ldr r2, [pc, #0x6c]
00466dac: ldr r0, [r4, r0]
00466db0: ldr r3, [pc, #0x68]
00466db4: mov ip, #0x73
00466db8: add r1, pc, r1
00466dbc: add r2, pc, r2
00466dc0: add r3, pc, r3
00466dc4: add r0, r0, #0xa8
00466dc8: str ip, [sp]
00466dcc: bl #0x30e004
00466dd0: b #0x466d30
00466dd4: ldr r0, [pc, #0x38]
00466dd8: ldr r1, [pc, #0x44]
00466ddc: ldr r2, [pc, #0x44]
00466de0: ldr r0, [r4, r0]
00466de4: ldr r3, [pc, #0x40]
00466de8: mov ip, #0x74
00466dec: add r1, pc, r1
00466df0: add r2, pc, r2
00466df4: add r3, pc, r3
00466df8: add r0, r0, #0xa8
00466dfc: str ip, [sp]
00466e00: bl #0x30e004
00466e04: b #0x466d68
00466e08: subseq sp, r2, ip, ror #26
00466e0c: andeq r2, r0, r4, ror r2
00466e10: andeq r3, r0, r0, asr #19
00466e14: andeq r1, r0, r0, asr #19
00466e18: subeq r7, r5, r0, lsr #12
00466e1c: subeq r6, r6, ip, lsr r5
00466e20: subeq r6, r6, r0, asr #9
00466e24: subeq r7, r5, ip, ror #11
00466e28: subeq r6, r6, r8, lsl r5
00466e2c: subeq r6, r6, ip, lsl #9

0x467488 _ZNK14PlayerSavegame17SG_GetSkillInSlotEi
00467488: push {r4, r5, r6, lr}
0046748c: mov r6, r0
00467490: ldr r0, [r0, #0x10]
00467494: mov r5, r1
00467498: mvn r1, #0
0046749c: add r0, r0, #0x37c
004674a0: bl #0x3fc6a0
004674a4: ldr r3, [r6, #0x88]
004674a8: mov r2, #0x18
004674ac: mla r3, r2, r0, r3
004674b0: ldr r4, [r3, #4]
004674b4: cmp r4, #0
004674b8: beq #0x4674fc
004674bc: mov ip, r3
004674c0: b #0x4674c8
004674c4: mov r4, r1
004674c8: ldr r1, [r4, #0x10]
004674cc: cmp r5, r1
004674d0: ldrgt r1, [r4, #0xc]
004674d4: ldrle r1, [r4, #8]
004674d8: movgt r4, ip
004674dc: mov ip, r4
004674e0: cmp r1, #0
004674e4: bne #0x4674c4
004674e8: cmp r3, r4
004674ec: beq #0x467500
004674f0: ldr r2, [r4, #0x10]
004674f4: cmp r5, r2
004674f8: bge #0x467500
004674fc: mov r4, r3
00467500: ldr r0, [r6, #0x10]
00467504: mvn r1, #0
00467508: add r0, r0, #0x37c
0046750c: bl #0x3fc6a0
00467510: ldr r3, [r6, #0x88]
00467514: mov r2, #0x18
00467518: mla r3, r2, r0, r3
0046751c: cmp r4, r3
00467520: mvneq r0, #0
00467524: ldrne r0, [r4, #0x14]
00467528: pop {r4, r5, r6, pc}

0x315fb8 _ZN8Savegame7saveAllEv
00315fb8: ldr r1, [pc, #0x3dc]
00315fbc: ldr r2, [pc, #0x3dc]
00315fc0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315fc4: add r1, pc, r1
00315fc8: ldr r3, [r1, r2]
00315fcc: mov sb, r0
00315fd0: ldr r0, [pc, #0x3cc]
00315fd4: ldr r3, [r3]
00315fd8: sub sp, sp, #0x8c
00315fdc: str r1, [sp, #0x14]
00315fe0: add r0, pc, r0
00315fe4: add r1, sp, #0x88
00315fe8: str r2, [sp, #0x20]
00315fec: str r1, [sp, #0x18]
00315ff0: str r3, [sp, #0x84]
00315ff4: bl #0x3136b4
00315ff8: ldr r2, [sp, #0x18]
00315ffc: mov r4, #0
00316000: add fp, sb, #0x20
00316004: str r4, [r2, #-0x24]!
00316008: add r3, r2, #4
0031600c: str r2, [sp, #0x18]
00316010: ldr r2, [sb, #0x14]
00316014: ldr r1, [sb, #0x18]
00316018: mov r0, r3
0031601c: str r3, [sp, #0x78]
00316020: str r3, [sp, #0x7c]
00316024: bl #0x3116e8
00316028: mov r3, #1
0031602c: ldr r0, [sp, #0x18]
00316030: strb r3, [sp, #0x80]
00316034: strb r4, [sp, #0x81]
00316038: bl #0x315110
0031603c: mov r0, #0x30
00316040: bl #0x310454
00316044: add r3, sp, #0x88
00316048: mov r4, r0
0031604c: str r3, [sp, #0x1c]
00316050: bl #0x316d3c
00316054: ldr r0, [sp, #0x1c]
00316058: mvn r3, #0
0031605c: add r7, sp, #0x3c
00316060: str r3, [r0, #-0x48]!
00316064: str r0, [sp, #0x1c]
00316068: ldr r1, [sp, #0x1c]
0031606c: mov r0, r4
00316070: bl #0x3139e0
00316074: ldr r3, [pc, #0x32c]
00316078: ldr r1, [pc, #0x32c]
0031607c: ldr r2, [pc, #0x32c]
00316080: add r3, pc, r3
00316084: str r3, [sp, #0x2c]
00316088: ldr r3, [pc, #0x324]
0031608c: add r3, pc, r3
00316090: str r3, [sp, #0x30]
00316094: ldr r3, [pc, #0x31c]
00316098: add r3, pc, r3
0031609c: str r3, [sp, #0x34]
003160a0: ldr r5, [sb, #0x28]
003160a4: str r1, [sp, #0x24]
003160a8: str r2, [sp, #0x28]
003160ac: cmp r5, fp
003160b0: beq #0x3161a0
003160b4: ldr r3, [r4]
003160b8: mov r0, r4
003160bc: mov lr, pc
003160c0: ldr pc, [r3, #0x30]
003160c4: mov r3, #0
003160c8: strd r0, r1, [sp, #8]
003160cc: mov r1, r7
003160d0: mov r0, r4
003160d4: str r3, [sp, #0x3c]
003160d8: bl #0x3139e0
003160dc: ldr r1, [r5, #0x24]
003160e0: mov r2, #4
003160e4: mov r3, #0
003160e8: mov r0, r4
003160ec: bl #0x317604
003160f0: ldr r3, [r4]
003160f4: mov r0, r4
003160f8: mov lr, pc
003160fc: ldr pc, [r3, #0x30]
00316100: ldr r3, [r5, #0x38]
00316104: mov r8, r0
00316108: cmp r3, #0
0031610c: beq #0x316284
00316110: mov r0, r4
00316114: ldr r1, [r5, #0x3c]
00316118: blx r3
0031611c: ldr r3, [r4]
00316120: mov r0, r4
00316124: mov lr, pc
00316128: ldr pc, [r3, #0x30]
0031612c: ldrd r2, r3, [sp, #8]
00316130: rsb r8, r8, r0
00316134: str r8, [sp, #0x3c]
00316138: mov r6, r0
0031613c: mov sl, r1
00316140: mov r0, r4
00316144: ldr r1, [r4]
00316148: mov lr, pc
0031614c: ldr pc, [r1, #0x2c]
00316150: mov r0, r4
00316154: mov r1, r7
00316158: bl #0x3139e0
0031615c: mov r3, sl
00316160: mov r2, r6
00316164: ldr r1, [r4]
00316168: mov r0, r4
0031616c: mov lr, pc
00316170: ldr pc, [r1, #0x2c]
00316174: ldr r3, [r5, #0xc]
00316178: cmp r3, #0
0031617c: bne #0x316188
00316180: b #0x3162ec
00316184: mov r3, r2
00316188: ldr r2, [r3, #8]
0031618c: cmp r2, #0
00316190: bne #0x316184
00316194: mov r5, r3
00316198: cmp r5, fp
0031619c: bne #0x3160b4
003161a0: ldr r3, [r4]
003161a4: mov r0, r4
003161a8: mov lr, pc
003161ac: ldr pc, [r3, #0x30]
003161b0: mov r2, #0
003161b4: mov r6, r0
003161b8: mov r7, r1
003161bc: mov r3, #0
003161c0: mov r0, r4
003161c4: ldr r1, [r4]
003161c8: mov lr, pc
003161cc: ldr pc, [r1, #0x2c]
003161d0: ldr r3, [sb, #0x30]
003161d4: ldr r1, [sp, #0x1c]
003161d8: mov r0, r4
003161dc: str r3, [sp, #0x40]
003161e0: bl #0x3139e0
003161e4: mov r2, r6
003161e8: mov r3, r7
003161ec: ldr r1, [r4]
003161f0: mov r0, r4
003161f4: mov lr, pc
003161f8: ldr pc, [r1, #0x2c]
003161fc: add r5, sp, #0x88
00316200: mov r0, sb
00316204: mov r1, r4
00316208: bl #0x315ad0
0031620c: str r4, [r5, #-0x44]!
00316210: add r3, r5, #4
00316214: ldr r2, [sb, #0x14]
00316218: ldr r1, [sb, #0x18]
0031621c: mov r0, r3
00316220: str r3, [sp, #0x58]
00316224: str r3, [sp, #0x5c]
00316228: bl #0x3116e8
0031622c: mov r3, #0
00316230: mov r0, r5
00316234: strb r3, [sp, #0x60]
00316238: mov r3, #1
0031623c: strb r3, [sp, #0x61]
00316240: bl #0x315110
00316244: mov r0, r5
00316248: bl #0x313c90
0031624c: ldr r0, [sp, #0x18]
00316250: bl #0x313c90
00316254: ldr r0, [pc, #0x160]
00316258: add r0, pc, r0
0031625c: bl #0x3136b8
00316260: ldr r1, [sp, #0x14]
00316264: ldr r0, [sp, #0x20]
00316268: ldr r2, [sp, #0x84]
0031626c: ldr r3, [r1, r0]
00316270: ldr r3, [r3]
00316274: cmp r2, r3
00316278: bne #0x316398
0031627c: add sp, sp, #0x8c
00316280: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00316284: ldr r6, [sb, #0x1c]
00316288: cmp r6, #0
0031628c: beq #0x316320
00316290: ldrb r3, [r6, #0x2c]
00316294: ldr r2, [r4]
00316298: cmp r3, #0
0031629c: ldr sl, [r2, #0x1c]
003162a0: bne #0x3162c8
003162a4: ldr r1, [sp, #0x14]
003162a8: ldr r0, [sp, #0x24]
003162ac: ldr r2, [r1, r0]
003162b0: ldr r2, [r2]
003162b4: cmp r2, #2
003162b8: streq r3, [r3]
003162bc: beq #0x3162c8
003162c0: cmp r2, #1
003162c4: beq #0x31636c
003162c8: ldr r3, [r6, #0x1c]
003162cc: ldr ip, [r5, #0x28]
003162d0: mov r0, r4
003162d4: ldr r1, [r3]
003162d8: ldr r2, [r5, #0x30]
003162dc: mov r3, #0
003162e0: add r1, r1, ip
003162e4: blx sl
003162e8: b #0x31611c
003162ec: ldr r2, [r5, #4]
003162f0: ldr r1, [r2, #0xc]
003162f4: cmp r5, r1
003162f8: bne #0x316314
003162fc: mov r5, r2
00316300: ldr r2, [r2, #4]
00316304: ldr r3, [r2, #0xc]
00316308: cmp r3, r5
0031630c: beq #0x3162fc
00316310: ldr r3, [r5, #0xc]
00316314: cmp r3, r2
00316318: movne r5, r2
0031631c: b #0x3160ac
00316320: mov r1, r6
00316324: ldr r0, [r5, #0x30]
00316328: bl #0x31056c
0031632c: mov r1, r6
00316330: mov sl, r0
00316334: ldr r2, [r5, #0x30]
00316338: bl #0x30e460
0031633c: mov r3, r6
00316340: ldr ip, [r4]
00316344: mov r0, r4
00316348: mov r1, sl
0031634c: ldr r2, [r5, #0x30]
00316350: mov lr, pc
00316354: ldr pc, [ip, #0x1c]
00316358: cmp sl, #0
0031635c: beq #0x31611c
00316360: mov r0, sl
00316364: bl #0x310440
00316368: b #0x31611c
0031636c: ldr r3, [sp, #0x14]
00316370: ldr r2, [sp, #0x28]
00316374: mov ip, #0x82
00316378: ldr r1, [sp, #0x2c]
0031637c: ldr r0, [r3, r2]
00316380: ldr r2, [sp, #0x30]
00316384: ldr r3, [sp, #0x34]
00316388: add r0, r0, #0xa8
0031638c: str ip, [sp]
00316390: bl #0x30e004
00316394: b #0x3162c8
00316398: bl #0x30e310
0031639c: rsbeq lr, r7, ip, asr #21
003163a0: andeq r4, r0, ip, lsr #1
003163a4: ldrsbeq r8, [sl], #-0x50
003163a8: subseq r8, sl, r8, asr r3
003163ac: andeq r3, r0, r0, asr #19
003163b0: andeq r1, r0, r0, asr #19
003163b4: subseq r8, sl, ip, lsr r5
003163b8: subseq r8, sl, r8, lsr r5
003163bc: subseq r8, sl, r8, asr r3

0x4684f0 _ZN14PlayerSavegame22_SaveVolatileQuestsLogEv
004684f0: push {r4, r5, r6, lr}
004684f4: mov r5, r0
004684f8: bl #0x7fd794
004684fc: ldrb r3, [r0, #5]
00468500: ldr r4, [pc, #0x60]
00468504: cmp r3, #0
00468508: add r4, pc, r4
0046850c: bne #0x468514
00468510: pop {r4, r5, r6, pc}
00468514: ldr r3, [pc, #0x50]
00468518: ldr r6, [r4, r3]
0046851c: ldr r0, [r6, #0x40]
00468520: bl #0x36f074
00468524: cmp r0, #0
00468528: ldreq r6, [r6, #0x40]
0046852c: beq #0x468540
00468530: ldr r6, [r6, #0x40]
00468534: ldrb r3, [r6, #0x719]
00468538: cmp r3, #0
0046853c: beq #0x468510
00468540: add r6, r6, #0x6e0
00468544: mov r0, r6
00468548: bl #0x316a48
0046854c: ldr r3, [pc, #0x1c]
00468550: add r0, r5, #0x118
00468554: mov r2, r6
00468558: ldr r3, [r4, r3]
0046855c: ldr r1, [r3]
00468560: pop {r4, r5, r6, lr}
00468564: b #0x46c658
00468568: subseq ip, r2, r8, lsl #11
0046856c: strdeq r3, r4, [r0], -r4
00468570: muleq r0, ip, sl

0x463654 _ZN14PlayerSavegame14SG_SynchronizeEb
00463654: ldr r3, [pc, #0x44]
00463658: ldr r2, [pc, #0x44]
0046365c: str r4, [sp, #-4]!
00463660: add r3, pc, r3
00463664: ldr r4, [r3, r2]
00463668: ldr ip, [r0, #0x3c]
0046366c: mov r2, r1
00463670: ldr r3, [r4]
00463674: cmp ip, r3
00463678: blt #0x46368c
0046367c: add r1, r0, #0x118
00463680: add r0, r0, #0xb8
00463684: ldm sp!, {r4}
00463688: b #0x46b644
0046368c: cmp r1, #0
00463690: beq #0x46367c
00463694: mov r0, #0
00463698: ldm sp!, {r4}
0046369c: bx lr
004636a0: subseq r1, r3, r0, lsr r4
004636a4: muleq r0, ip, sl

0x468630 _ZN14PlayerSavegame19_SetupSavedSectionsEbb
00468630: push {r4, r5, r6, lr}
00468634: ldr r4, [pc, #0x210]
00468638: cmp r1, #0
0046863c: sub sp, sp, #8
00468640: mov r5, r0
00468644: add r4, pc, r4
00468648: mov r6, r2
0046864c: beq #0x468660
00468650: cmp r2, #0
00468654: beq #0x468768
00468658: add sp, sp, #8
0046865c: pop {r4, r5, r6, pc}
00468660: cmp r2, #0
00468664: bne #0x468658
00468668: ldr r3, [pc, #0x1e0]
0046866c: ldr r1, [pc, #0x1e0]
00468670: ldr r0, [r0, #8]
00468674: ldr r2, [r4, r3]
00468678: ldr r3, [pc, #0x1d8]
0046867c: add r1, pc, r1
00468680: str r5, [sp]
00468684: ldr r3, [r4, r3]
00468688: bl #0x315904
0046868c: ldr r3, [pc, #0x1c8]
00468690: ldr r1, [pc, #0x1c8]
00468694: ldr r0, [r5, #8]
00468698: ldr r2, [r4, r3]
0046869c: ldr r3, [pc, #0x1c0]
004686a0: add r1, pc, r1
004686a4: str r5, [sp]
004686a8: ldr r3, [r4, r3]
004686ac: bl #0x315904
004686b0: ldr r3, [pc, #0x1b0]
004686b4: ldr r1, [pc, #0x1b0]
004686b8: ldr r0, [r5, #8]
004686bc: ldr r2, [r4, r3]
004686c0: ldr r3, [pc, #0x1a8]
004686c4: add r1, pc, r1
004686c8: str r5, [sp]
004686cc: ldr r3, [r4, r3]
004686d0: bl #0x315904
004686d4: ldr r3, [pc, #0x198]
004686d8: ldr r1, [pc, #0x198]
004686dc: ldr r0, [r5, #8]
004686e0: ldr r2, [r4, r3]
004686e4: ldr r3, [pc, #0x190]
004686e8: add r1, pc, r1
004686ec: str r5, [sp]
004686f0: ldr r3, [r4, r3]
004686f4: bl #0x315904
004686f8: ldr r3, [pc, #0x180]
004686fc: ldr r1, [pc, #0x180]
00468700: ldr r0, [r5, #8]
00468704: ldr r2, [r4, r3]
00468708: ldr r3, [pc, #0x178]
0046870c: add r1, pc, r1
00468710: str r5, [sp]
00468714: ldr r3, [r4, r3]
00468718: bl #0x315904
0046871c: ldr r3, [pc, #0x168]
00468720: ldr r1, [pc, #0x168]
00468724: ldr r0, [r5, #8]
00468728: ldr r2, [r4, r3]
0046872c: ldr r3, [pc, #0x160]
00468730: add r1, pc, r1
00468734: str r5, [sp]
00468738: ldr r3, [r4, r3]
0046873c: bl #0x315904
00468740: ldr r3, [pc, #0x150]
00468744: ldr r1, [pc, #0x150]
00468748: ldr r0, [r5, #8]
0046874c: ldr r2, [r4, r3]
00468750: ldr r3, [pc, #0x148]
00468754: add r1, pc, r1
00468758: str r5, [sp]
0046875c: ldr r3, [r4, r3]
00468760: bl #0x315904
00468764: b #0x468658
00468768: ldr r3, [pc, #0xe0]
0046876c: ldr r1, [pc, #0x130]
00468770: ldr r0, [r0, #8]
00468774: ldr r2, [r4, r3]
00468778: add r1, pc, r1
0046877c: mov r3, r6
00468780: str r5, [sp]
00468784: bl #0x315904
00468788: ldr r3, [pc, #0xcc]
0046878c: ldr r1, [pc, #0x114]
00468790: ldr r0, [r5, #8]
00468794: ldr r2, [r4, r3]
00468798: add r1, pc, r1
0046879c: mov r3, r6
004687a0: str r5, [sp]
004687a4: bl #0x315904
004687a8: ldr r3, [pc, #0xb8]
004687ac: ldr r1, [pc, #0xf8]
004687b0: ldr r0, [r5, #8]
004687b4: ldr r2, [r4, r3]
004687b8: add r1, pc, r1
004687bc: mov r3, r6
004687c0: str r5, [sp]
004687c4: bl #0x315904
004687c8: ldr r3, [pc, #0xa4]
004687cc: ldr r1, [pc, #0xdc]
004687d0: ldr r0, [r5, #8]
004687d4: ldr r2, [r4, r3]
004687d8: add r1, pc, r1
004687dc: mov r3, r6
004687e0: str r5, [sp]
004687e4: bl #0x315904
004687e8: ldr r3, [pc, #0x90]
004687ec: ldr r1, [pc, #0xc0]
004687f0: ldr r0, [r5, #8]
004687f4: ldr r2, [r4, r3]
004687f8: add r1, pc, r1
004687fc: mov r3, r6
00468800: str r5, [sp]
00468804: bl #0x315904
00468808: ldr r3, [pc, #0x7c]
0046880c: ldr r1, [pc, #0xa4]
00468810: ldr r0, [r5, #8]
00468814: ldr r2, [r4, r3]
00468818: add r1, pc, r1
0046881c: mov r3, r6
00468820: str r5, [sp]
00468824: bl #0x315904
00468828: ldr r3, [pc, #0x68]
0046882c: ldr r1, [pc, #0x88]
00468830: ldr r0, [r5, #8]
00468834: ldr r2, [r4, r3]
00468838: add r1, pc, r1
0046883c: mov r3, r6
00468840: str r5, [sp]
00468844: bl #0x315904
00468848: b #0x468658
0046884c: subseq ip, r2, ip, asr #8
00468850: andeq r0, r0, r8, lsr #26
00468854: subeq r4, r6, r4, lsl #23
00468858: strheq r1, [r0], -r8
0046885c: andeq r3, r0, r4, lsr #12
00468860: subeq r4, r6, r8, ror #22
00468864: andeq r3, r0, r8, ror r7
00468868: andeq r3, r0, ip, asr #26
0046886c: subeq r4, r6, ip, asr #22
00468870: strheq r2, [r0], -r0
00468874: andeq r4, r0, r0, lsr r0
00468878: subeq r4, r6, r0, lsr fp
0046887c: andeq r2, r0, ip, ror #12
00468880: andeq r3, r0, r0, lsl #30
00468884: subeq r4, r6, r4, lsl fp
00468888: andeq r4, r0, ip, ror r6
0046888c: andeq r1, r0, ip, lsr r3
00468890: subeq r4, r6, r0, lsl #22
00468894: andeq r0, r0, r4, lsr #14
00468898: andeq r0, r0, ip, lsl #29
0046889c: subeq r4, r6, r4, lsl #22
004688a0: andeq r1, r0, r0, ror #24
004688a4: subeq r4, r6, r8, lsl #21
004688a8: subeq r4, r6, r0, ror sl
004688ac: subeq r4, r6, r8, asr sl
004688b0: subeq r4, r6, r0, asr #20
004688b4: subeq r4, r6, r8, lsr #20
004688b8: subeq r4, r6, r8, lsl sl
004688bc: subeq r4, r6, r0, lsr #20

0x4626f4 _ZN13LevelSavegame6DeleteEjiiibb
004626f4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004626f8: ldr r4, [pc, #0x114]
004626fc: ldr r6, [pc, #0x114]
00462700: sub sp, sp, #0x34
00462704: add r4, pc, r4
00462708: ldr ip, [r4, r6]
0046270c: add r5, sp, #0x14
00462710: str r1, [sp, #0xc]
00462714: ldr ip, [ip]
00462718: ldrb r7, [sp, #0x58]
0046271c: mov r8, r0
00462720: mov r1, #0x10
00462724: mov r0, r5
00462728: mov sb, r2
0046272c: mov sl, r3
00462730: str ip, [sp, #0x2c]
00462734: str r5, [sp, #0x24]
00462738: str r5, [sp, #0x28]
0046273c: ldrb fp, [sp, #0x5c]
00462740: bl #0x31167c
00462744: ldr r3, [sp, #0x24]
00462748: mov r2, #0
0046274c: cmp r7, #0
00462750: strb r2, [r3]
00462754: beq #0x4627f4
00462758: mov r0, r8
0046275c: mov r1, sl
00462760: mov r2, fp
00462764: mov r3, r5
00462768: bl #0x4622c8
0046276c: ldr r3, [pc, #0xa8]
00462770: ldr r1, [sp, #0x28]
00462774: ldr r7, [r4, r3]
00462778: ldr r3, [r7, #0x10]
0046277c: ldr r3, [r3, #0x34]
00462780: mov r0, r3
00462784: ldr r3, [r3]
00462788: mov lr, pc
0046278c: ldr pc, [r3, #0x9c]
00462790: ldr r1, [pc, #0x88]
00462794: mov r8, r0
00462798: mov r0, r5
0046279c: add r1, pc, r1
004627a0: add r2, r1, #4
004627a4: bl #0x310804
004627a8: ldr r3, [r7, #0x10]
004627ac: ldr r1, [sp, #0x28]
004627b0: ldr r3, [r3, #0x34]
004627b4: mov r0, r3
004627b8: ldr r3, [r3]
004627bc: mov lr, pc
004627c0: ldr pc, [r3, #0x9c]
004627c4: orr r8, r0, r8
004627c8: mov r0, r5
004627cc: bl #0x3139ac
004627d0: ldr r3, [r4, r6]
004627d4: ldr r2, [sp, #0x2c]
004627d8: uxtb r8, r8
004627dc: ldr r3, [r3]
004627e0: mov r0, r8
004627e4: cmp r2, r3
004627e8: bne #0x462810
004627ec: add sp, sp, #0x34
004627f0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004627f4: mov r0, r8
004627f8: ldr r1, [sp, #0xc]
004627fc: mov r2, sb
00462800: mov r3, sl
00462804: str r5, [sp]
00462808: bl #0x4624e4
0046280c: b #0x46276c
00462810: bl #0x30e310
00462814: subseq r2, r3, ip, lsl #7
00462818: andeq r4, r0, ip, lsr #1
0046281c: strdeq r3, r4, [r0], -r4
00462820: subeq fp, r5, r4, asr #27

0x464a68 _ZN14PlayerSavegame19SG_DeleteCheckpointEjb
00464a68: ldr r3, [pc, #0xb0]
00464a6c: ldr r2, [pc, #0xb0]
00464a70: push {r4, r5, r6, r7, lr}
00464a74: add r3, pc, r3
00464a78: ldr r5, [r3, r2]
00464a7c: sub sp, sp, #0x24
00464a80: add r4, sp, #4
00464a84: ldr r2, [r5]
00464a88: mov r6, r0
00464a8c: mov r7, r1
00464a90: mov r0, r4
00464a94: mov r1, #0x10
00464a98: str r2, [sp, #0x1c]
00464a9c: str r4, [sp, #0x14]
00464aa0: str r4, [sp, #0x18]
00464aa4: bl #0x31167c
00464aa8: ldr r2, [sp, #0x14]
00464aac: mov r1, #0
00464ab0: mov r3, r7
00464ab4: strb r1, [r2]
00464ab8: mov r0, r6
00464abc: mov r1, r4
00464ac0: mov r2, #1
00464ac4: bl #0x463c84
00464ac8: ldr r0, [sp, #0x18]
00464acc: bl #0x46355c
00464ad0: ldr r1, [pc, #0x50]
00464ad4: mov r6, r0
00464ad8: mov r0, r4
00464adc: add r1, pc, r1
00464ae0: add r2, r1, #4
00464ae4: bl #0x310804
00464ae8: ldr r0, [sp, #0x18]
00464aec: bl #0x46355c
00464af0: orr r6, r0, r6
00464af4: mov r0, r4
00464af8: bl #0x3139ac
00464afc: ldr r2, [sp, #0x1c]
00464b00: ldr r3, [r5]
00464b04: uxtb r6, r6
00464b08: mov r0, r6
00464b0c: cmp r2, r3
00464b10: bne #0x464b1c
00464b14: add sp, sp, #0x24
00464b18: pop {r4, r5, r6, r7, pc}
00464b1c: bl #0x30e310
00464b20: subseq r0, r3, ip, lsl r0
00464b24: andeq r4, r0, ip, lsr #1
00464b28: subeq sb, r5, r4, lsl #21

0x315ad0 _ZN8Savegame10_cacheFileEP12StreamBuffer
00315ad0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315ad4: ldr r3, [r0, #0x1c]
00315ad8: ldr r6, [pc, #0x3ec]
00315adc: sub sp, sp, #0x1c
00315ae0: cmp r3, #0
00315ae4: mov r4, r0
00315ae8: mov r5, r1
00315aec: add r6, pc, r6
00315af0: beq #0x315b0c
00315af4: mov r0, r3
00315af8: ldr r3, [r3]
00315afc: mov lr, pc
00315b00: ldr pc, [r3, #4]
00315b04: mov r3, #0
00315b08: str r3, [r4, #0x1c]
00315b0c: cmp r5, #0
00315b10: beq #0x315e30
00315b14: mov r0, r5
00315b18: mov r2, #0
00315b1c: mov r3, #0
00315b20: ldr r1, [r5]
00315b24: mov lr, pc
00315b28: ldr pc, [r1, #0x20]
00315b2c: mov r2, #0
00315b30: mov r3, #0
00315b34: mov r0, r5
00315b38: ldr r1, [r5]
00315b3c: mov lr, pc
00315b40: ldr pc, [r1, #0x2c]
00315b44: mov r1, #0
00315b48: mov r0, #0x30
00315b4c: bl #0x310570
00315b50: mov r1, r5
00315b54: mov r7, r0
00315b58: bl #0x3172d8
00315b5c: str r7, [r4, #0x1c]
00315b60: ldrb r3, [r4, #0x38]
00315b64: cmp r3, #0
00315b68: bne #0x315df4
00315b6c: ldr r3, [r4, #0x1c]
00315b70: cmp r3, #0
00315b74: beq #0x315bbc
00315b78: mov r0, r3
00315b7c: ldr r3, [r3]
00315b80: mov lr, pc
00315b84: ldr pc, [r3, #8]
00315b88: cmp r1, #0
00315b8c: bne #0x315dfc
00315b90: cmp r0, #3
00315b94: bhi #0x315dfc
00315b98: ldr r3, [r4, #0x1c]
00315b9c: cmp r3, #0
00315ba0: beq #0x315bbc
00315ba4: mov r0, r3
00315ba8: ldr r3, [r3]
00315bac: mov lr, pc
00315bb0: ldr pc, [r3, #4]
00315bb4: mov r3, #0
00315bb8: str r3, [r4, #0x1c]
00315bbc: ldr r3, [r4, #0x18]
00315bc0: ldr r0, [r4, #0x14]
00315bc4: mov r1, #0
00315bc8: ldr r7, [pc, #0x300]
00315bcc: rsb r0, r3, r0
00315bd0: add r0, r0, #5
00315bd4: bl #0x31056c
00315bd8: ldr r1, [r4, #0x18]
00315bdc: mov r5, r0
00315be0: bl #0x30e520
00315be4: mov r0, r5
00315be8: bl #0x30de54
00315bec: ldr r1, [pc, #0x2e0]
00315bf0: mov r2, #5
00315bf4: add r0, r5, r0
00315bf8: add r1, pc, r1
00315bfc: bl #0x30e868
00315c00: ldr r3, [r6, r7]
00315c04: mov r1, r5
00315c08: mov r2, #0
00315c0c: ldr r3, [r3, #0x10]
00315c10: ldr r3, [r3, #0x34]
00315c14: mov r0, r3
00315c18: ldr r3, [r3]
00315c1c: mov lr, pc
00315c20: ldr pc, [r3, #0x94]
00315c24: cmp r5, #0
00315c28: str r0, [sp, #0x14]
00315c2c: beq #0x315c3c
00315c30: mov r0, r5
00315c34: bl #0x310440
00315c38: ldr r0, [sp, #0x14]
00315c3c: cmp r0, #0
00315c40: beq #0x315c94
00315c44: mov r1, #0
00315c48: mov r0, #0x30
00315c4c: bl #0x310570
00315c50: add r5, sp, #0x18
00315c54: ldr r1, [r5, #-4]!
00315c58: mov r8, r0
00315c5c: bl #0x3172d8
00315c60: ldr r3, [r6, r7]
00315c64: str r8, [r4, #0x1c]
00315c68: mov r1, r5
00315c6c: ldr r3, [r3, #0x10]
00315c70: ldr r3, [r3, #0x34]
00315c74: mov r0, r3
00315c78: ldr r3, [r3]
00315c7c: mov lr, pc
00315c80: ldr pc, [r3, #0x78]
00315c84: ldr r0, [r4, #0x1c]
00315c88: bl #0x313a90
00315c8c: cmn r0, #1
00315c90: beq #0x315ea4
00315c94: ldr r3, [r4, #0x1c]
00315c98: cmp r3, #0
00315c9c: beq #0x315df4
00315ca0: mov r0, r3
00315ca4: ldr r3, [r3]
00315ca8: mov lr, pc
00315cac: ldr pc, [r3, #8]
00315cb0: cmp r1, #0
00315cb4: bne #0x315cc0
00315cb8: cmp r0, #3
00315cbc: bls #0x315df4
00315cc0: ldr r1, [r4, #0x1c]
00315cc4: mov r2, #0
00315cc8: mov r3, #0
00315ccc: mov r0, r1
00315cd0: ldr r1, [r1]
00315cd4: mov lr, pc
00315cd8: ldr pc, [r1, #0x20]
00315cdc: ldr r0, [r4, #0x1c]
00315ce0: bl #0x313a90
00315ce4: cmp r0, #0
00315ce8: str r0, [sp, #4]
00315cec: beq #0x315df4
00315cf0: mov r5, #0
00315cf4: add r7, r4, #0x20
00315cf8: mov sb, r5
00315cfc: add r8, sp, #0xc
00315d00: ldr r3, [r4, #0x1c]
00315d04: mov r0, r3
00315d08: ldr r3, [r3]
00315d0c: mov lr, pc
00315d10: ldr pc, [r3, #0x24]
00315d14: ldr r3, [r4, #0x1c]
00315d18: mov sl, r0
00315d1c: mov r6, r1
00315d20: mov r0, r3
00315d24: ldr r3, [r3]
00315d28: mov lr, pc
00315d2c: ldr pc, [r3, #8]
00315d30: cmp r1, r6
00315d34: bhi #0x315d44
00315d38: bne #0x315df4
00315d3c: cmp r0, sl
00315d40: bls #0x315df4
00315d44: ldr r0, [r4, #0x1c]
00315d48: bl #0x313a90
00315d4c: mov r2, #4
00315d50: mov r3, #0
00315d54: mov r1, r8
00315d58: mov r6, r0
00315d5c: ldr r0, [r4, #0x1c]
00315d60: strb sb, [sp, #0xc]
00315d64: strb sb, [sp, #0xd]
00315d68: strb sb, [sp, #0xe]
00315d6c: strb sb, [sp, #0xf]
00315d70: strb sb, [sp, #0x10]
00315d74: bl #0x317454
00315d78: ldr r3, [r4, #0x1c]
00315d7c: mov r0, r3
00315d80: ldr r3, [r3]
00315d84: mov lr, pc
00315d88: ldr pc, [r3, #0x24]
00315d8c: mov sl, r0
00315d90: mov fp, r1
00315d94: mov r0, r7
00315d98: mov r1, r8
00315d9c: bl #0x314380
00315da0: cmp r7, r0
00315da4: mov r1, r8
00315da8: beq #0x315e10
00315dac: mov r0, r7
00315db0: bl #0x315978
00315db4: mov r1, r8
00315db8: strd sl, fp, [r0]
00315dbc: mov r0, r7
00315dc0: bl #0x315978
00315dc4: str r6, [r0, #8]
00315dc8: ldr r1, [r4, #0x1c]
00315dcc: adds r2, sl, r6
00315dd0: adc r3, fp, #0
00315dd4: mov r0, r1
00315dd8: ldr r1, [r1]
00315ddc: mov lr, pc
00315de0: ldr pc, [r1, #0x20]
00315de4: ldr r3, [sp, #4]
00315de8: add r5, r5, #1
00315dec: cmp r5, r3
00315df0: bne #0x315d00
00315df4: add sp, sp, #0x1c
00315df8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00315dfc: ldr r0, [r4, #0x1c]
00315e00: bl #0x313a90
00315e04: cmn r0, #1
00315e08: bne #0x315c94
00315e0c: b #0x315b98
00315e10: mov r1, r8
00315e14: bl #0x315978
00315e18: strd sl, fp, [r0]
00315e1c: str sb, [r0, #0x14]
00315e20: str sb, [r0, #0x10]
00315e24: str sb, [r0, #0xc]
00315e28: str r6, [r0, #8]
00315e2c: b #0x315dc8
00315e30: ldr r3, [pc, #0x98]
00315e34: ldr r1, [r4, #0x18]
00315e38: mov r2, r5
00315e3c: ldr r7, [r6, r3]
00315e40: ldr r3, [r7, #0x10]
00315e44: ldr r3, [r3, #0x34]
00315e48: mov r0, r3
00315e4c: ldr r3, [r3]
00315e50: mov lr, pc
00315e54: ldr pc, [r3, #0x94]
00315e58: cmp r0, #0
00315e5c: str r0, [sp, #0x14]
00315e60: beq #0x315b60
00315e64: mov r1, r5
00315e68: mov r0, #0x30
00315e6c: bl #0x310570
00315e70: add r5, sp, #0x18
00315e74: mov r8, r0
00315e78: ldr r1, [r5, #-4]!
00315e7c: bl #0x3172d8
00315e80: str r8, [r4, #0x1c]
00315e84: ldr r3, [r7, #0x10]
00315e88: mov r1, r5
00315e8c: ldr r3, [r3, #0x34]
00315e90: mov r0, r3
00315e94: ldr r3, [r3]
00315e98: mov lr, pc
00315e9c: ldr pc, [r3, #0x78]
00315ea0: b #0x315b60
00315ea4: ldr r3, [r4, #0x1c]
00315ea8: cmp r3, #0
00315eac: beq #0x315df4
00315eb0: mov r0, r3
00315eb4: ldr r3, [r3]
00315eb8: mov lr, pc
00315ebc: ldr pc, [r3, #4]
00315ec0: mov r3, #0
00315ec4: str r3, [r4, #0x1c]
00315ec8: b #0x315df4
00315ecc: rsbeq lr, r7, r4, lsr #31
00315ed0: strdeq r3, r4, [r0], -r4
00315ed4: subseq r8, sl, r8, ror #18

0x4634f4 _ZN14PlayerSavegame23SG_GetFilenameExtensionEv
004634f4: ldr r0, [pc, #4]
004634f8: add r0, pc, r0
004634fc: bx lr
00463500: subeq sp, r5, r8, lsl #5

0x4634e4 _ZN14PlayerSavegame20SG_GetFilenamePrefixEv
004634e4: ldr r0, [pc, #4]
004634e8: add r0, pc, r0
004634ec: bx lr
004634f0: subeq fp, r5, r0, ror r0

0x463c84 _ZN14PlayerSavegame14SG_GetFilenameEjRSsbb
00463c84: push {r4, r5, r6, r7, r8, sb, sl, lr}
00463c88: ldr r4, [pc, #0xc4]
00463c8c: ldr r6, [pc, #0xc4]
00463c90: mov r5, r2
00463c94: add r4, pc, r4
00463c98: ldr r2, [r4, r6]
00463c9c: sub sp, sp, #0x50
00463ca0: mov r8, r0
00463ca4: ldr r2, [r2]
00463ca8: mov r7, r1
00463cac: mov sl, r3
00463cb0: str r2, [sp, #0x4c]
00463cb4: bl #0x4634e4
00463cb8: cmp r5, #0
00463cbc: mov sb, r0
00463cc0: beq #0x463ce0
00463cc4: cmp sl, #0
00463cc8: bne #0x463d44
00463ccc: ldr sl, [pc, #0x88]
00463cd0: add sl, pc, sl
00463cd4: bl #0x463504
00463cd8: mov ip, r0
00463cdc: b #0x463cf0
00463ce0: bl #0x4634f4
00463ce4: ldr sl, [pc, #0x74]
00463ce8: mov ip, r0
00463cec: add sl, pc, sl
00463cf0: ldr r1, [pc, #0x6c]
00463cf4: add r5, sp, #0xc
00463cf8: mov r3, r8
00463cfc: mov r2, sb
00463d00: add r1, pc, r1
00463d04: mov r0, r5
00463d08: stm sp, {sl, ip}
00463d0c: bl #0x30eae4
00463d10: mov r0, r5
00463d14: bl #0x30de54
00463d18: mov r1, r5
00463d1c: add r2, r5, r0
00463d20: mov r0, r7
00463d24: bl #0x3109e0
00463d28: ldr r3, [r4, r6]
00463d2c: ldr r2, [sp, #0x4c]
00463d30: ldr r3, [r3]
00463d34: cmp r2, r3
00463d38: bne #0x463d50
00463d3c: add sp, sp, #0x50
00463d40: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00463d44: ldr sl, [pc, #0x1c]
00463d48: add sl, pc, sl
00463d4c: b #0x463cd4
00463d50: bl #0x30e310
00463d54: ldrsheq r0, [r3], #-0xdc
00463d58: andeq r4, r0, ip, lsr #1
00463d5c: umaaleq sb, r6, r8, r4
00463d60: subeq r7, r6, ip, lsl fp
00463d64: subeq sb, r6, r8, asr #9
00463d68: subeq sb, r6, r8, lsr #8

0x466e48 _ZN14PlayerSavegame16SG_SetLevelStateEiii
00466e48: push {r4, r5, r6, r7, r8, lr}
00466e4c: ldr r4, [pc, #0x1ac]
00466e50: subs r6, r1, #0
00466e54: sub sp, sp, #8
00466e58: add r4, pc, r4
00466e5c: mov r8, r0
00466e60: mov r5, r2
00466e64: mov r7, r3
00466e68: blt #0x466ee4
00466e6c: ldr r3, [pc, #0x190]
00466e70: ldr r3, [r4, r3]
00466e74: ldr r3, [r3]
00466e78: cmp r6, r3
00466e7c: blt #0x466ea4
00466e80: ldr r3, [pc, #0x180]
00466e84: ldr r3, [r4, r3]
00466e88: ldr r3, [r3]
00466e8c: cmp r3, #2
00466e90: moveq r3, #0
00466e94: streq r3, [r3]
00466e98: beq #0x466ea4
00466e9c: cmp r3, #1
00466ea0: beq #0x466fcc
00466ea4: cmp r5, #0
00466ea8: blt #0x466f3c
00466eac: cmp r5, #1
00466eb0: ble #0x466ed0
00466eb4: ldr r3, [pc, #0x14c]
00466eb8: ldr r3, [r4, r3]
00466ebc: ldr r3, [r3]
00466ec0: cmp r3, #2
00466ec4: beq #0x466f8c
00466ec8: cmp r3, #1
00466ecc: beq #0x466f98
00466ed0: add r7, r7, #0x1a
00466ed4: ldr r3, [r8, r7, lsl #2]
00466ed8: str r5, [r3, r6, lsl #2]
00466edc: add sp, sp, #8
00466ee0: pop {r4, r5, r6, r7, r8, pc}
00466ee4: ldr r3, [pc, #0x11c]
00466ee8: ldr r3, [r4, r3]
00466eec: ldr r3, [r3]
00466ef0: cmp r3, #2
00466ef4: moveq r3, #0
00466ef8: streq r3, [r3]
00466efc: beq #0x466e6c
00466f00: cmp r3, #1
00466f04: bne #0x466e6c
00466f08: ldr r0, [pc, #0xfc]
00466f0c: ldr r1, [pc, #0xfc]
00466f10: ldr r2, [pc, #0xfc]
00466f14: ldr r0, [r4, r0]
00466f18: ldr r3, [pc, #0xf8]
00466f1c: mov ip, #0x5b
00466f20: add r1, pc, r1
00466f24: add r2, pc, r2
00466f28: add r3, pc, r3
00466f2c: add r0, r0, #0xa8
00466f30: str ip, [sp]
00466f34: bl #0x30e004
00466f38: b #0x466e6c
00466f3c: ldr r3, [pc, #0xc4]
00466f40: ldr r3, [r4, r3]
00466f44: ldr r3, [r3]
00466f48: cmp r3, #2
00466f4c: beq #0x466f8c
00466f50: cmp r3, #1
00466f54: bne #0x466ed0
00466f58: ldr r0, [pc, #0xac]
00466f5c: ldr r1, [pc, #0xb8]
00466f60: ldr r2, [pc, #0xb8]
00466f64: ldr r0, [r4, r0]
00466f68: ldr r3, [pc, #0xb4]
00466f6c: mov ip, #0x5d
00466f70: add r1, pc, r1
00466f74: add r2, pc, r2
00466f78: add r3, pc, r3
00466f7c: add r0, r0, #0xa8
00466f80: str ip, [sp]
00466f84: bl #0x30e004
00466f88: b #0x466ed0
00466f8c: mov r3, #0
00466f90: str r3, [r3]
00466f94: b #0x466ed0
00466f98: ldr r0, [pc, #0x6c]
00466f9c: ldr r1, [pc, #0x84]
00466fa0: ldr r2, [pc, #0x84]
00466fa4: ldr r0, [r4, r0]
00466fa8: ldr r3, [pc, #0x80]
00466fac: mov ip, #0x5e
00466fb0: add r1, pc, r1
00466fb4: add r2, pc, r2
00466fb8: add r3, pc, r3
00466fbc: add r0, r0, #0xa8
00466fc0: str ip, [sp]
00466fc4: bl #0x30e004
00466fc8: b #0x466ed0
00466fcc: ldr r0, [pc, #0x38]
00466fd0: ldr r1, [pc, #0x5c]
00466fd4: ldr r2, [pc, #0x5c]
00466fd8: ldr r0, [r4, r0]
00466fdc: ldr r3, [pc, #0x58]
00466fe0: mov ip, #0x5c
00466fe4: add r1, pc, r1
00466fe8: add r2, pc, r2
00466fec: add r3, pc, r3
00466ff0: add r0, r0, #0xa8
00466ff4: str ip, [sp]
00466ff8: bl #0x30e004
00466ffc: b #0x466ea4
00467000: subseq sp, r2, r8, lsr ip
00467004: andeq r1, r0, r0, asr #17
00467008: andeq r3, r0, r0, asr #19
0046700c: andeq r1, r0, r0, asr #19
00467010: strheq r7, [r5], #-0x48
00467014: subeq r6, r6, ip, asr #8
00467018: subeq r6, r6, r8, asr r3
0046701c: subeq r7, r5, r8, ror #8
00467020: strheq r6, [r6], #-0x3c
00467024: subeq r6, r6, r8, lsl #6
00467028: subeq r7, r5, r8, lsr #8
0046702c: strdeq r6, r7, [r6], #-0x34
00467030: subeq r6, r6, r8, asr #5
00467034: strdeq r7, r8, [r5], #-0x34
00467038: umaaleq r6, r6, r8, r3
0046703c: umaaleq r6, r6, r4, r2

0x466b18 _ZN14PlayerSavegame17SG_SetMapLocStateEiii
00466b18: push {r4, r5, r6, r7, r8, lr}
00466b1c: ldr r4, [pc, #0x1ac]
00466b20: subs r6, r1, #0
00466b24: sub sp, sp, #8
00466b28: add r4, pc, r4
00466b2c: mov r8, r0
00466b30: mov r5, r2
00466b34: mov r7, r3
00466b38: blt #0x466bb4
00466b3c: ldr r3, [pc, #0x190]
00466b40: ldr r3, [r4, r3]
00466b44: ldr r3, [r3]
00466b48: cmp r6, r3
00466b4c: blt #0x466b74
00466b50: ldr r3, [pc, #0x180]
00466b54: ldr r3, [r4, r3]
00466b58: ldr r3, [r3]
00466b5c: cmp r3, #2
00466b60: moveq r3, #0
00466b64: streq r3, [r3]
00466b68: beq #0x466b74
00466b6c: cmp r3, #1
00466b70: beq #0x466c9c
00466b74: cmp r5, #0
00466b78: blt #0x466c0c
00466b7c: cmp r5, #2
00466b80: ble #0x466ba0
00466b84: ldr r3, [pc, #0x14c]
00466b88: ldr r3, [r4, r3]
00466b8c: ldr r3, [r3]
00466b90: cmp r3, #2
00466b94: beq #0x466c5c
00466b98: cmp r3, #1
00466b9c: beq #0x466c68
00466ba0: add r7, r8, r7, lsl #2
00466ba4: ldr r3, [r7, #0x74]
00466ba8: str r5, [r3, r6, lsl #2]
00466bac: add sp, sp, #8
00466bb0: pop {r4, r5, r6, r7, r8, pc}
00466bb4: ldr r3, [pc, #0x11c]
00466bb8: ldr r3, [r4, r3]
00466bbc: ldr r3, [r3]
00466bc0: cmp r3, #2
00466bc4: moveq r3, #0
00466bc8: streq r3, [r3]
00466bcc: beq #0x466b3c
00466bd0: cmp r3, #1
00466bd4: bne #0x466b3c
00466bd8: ldr r0, [pc, #0xfc]
00466bdc: ldr r1, [pc, #0xfc]
00466be0: ldr r2, [pc, #0xfc]
00466be4: ldr r0, [r4, r0]
00466be8: ldr r3, [pc, #0xf8]
00466bec: mov ip, #0x7d
00466bf0: add r1, pc, r1
00466bf4: add r2, pc, r2
00466bf8: add r3, pc, r3
00466bfc: add r0, r0, #0xa8
00466c00: str ip, [sp]
00466c04: bl #0x30e004
00466c08: b #0x466b3c
00466c0c: ldr r3, [pc, #0xc4]
00466c10: ldr r3, [r4, r3]
00466c14: ldr r3, [r3]
00466c18: cmp r3, #2
00466c1c: beq #0x466c5c
00466c20: cmp r3, #1
00466c24: bne #0x466ba0
00466c28: ldr r0, [pc, #0xac]
00466c2c: ldr r1, [pc, #0xb8]
00466c30: ldr r2, [pc, #0xb8]
00466c34: ldr r0, [r4, r0]
00466c38: ldr r3, [pc, #0xb4]
00466c3c: mov ip, #0x7f
00466c40: add r1, pc, r1
00466c44: add r2, pc, r2
00466c48: add r3, pc, r3
00466c4c: add r0, r0, #0xa8
00466c50: str ip, [sp]
00466c54: bl #0x30e004
00466c58: b #0x466ba0
00466c5c: mov r3, #0
00466c60: str r3, [r3]
00466c64: b #0x466ba0
00466c68: ldr r0, [pc, #0x6c]
00466c6c: ldr r1, [pc, #0x84]
00466c70: ldr r2, [pc, #0x84]
00466c74: ldr r0, [r4, r0]
00466c78: ldr r3, [pc, #0x80]
00466c7c: mov ip, #0x80
00466c80: add r1, pc, r1
00466c84: add r2, pc, r2
00466c88: add r3, pc, r3
00466c8c: add r0, r0, #0xa8
00466c90: str ip, [sp]
00466c94: bl #0x30e004
00466c98: b #0x466ba0
00466c9c: ldr r0, [pc, #0x38]
00466ca0: ldr r1, [pc, #0x5c]
00466ca4: ldr r2, [pc, #0x5c]
00466ca8: ldr r0, [r4, r0]
00466cac: ldr r3, [pc, #0x58]
00466cb0: mov ip, #0x7e
00466cb4: add r1, pc, r1
00466cb8: add r2, pc, r2
00466cbc: add r3, pc, r3
00466cc0: add r0, r0, #0xa8
00466cc4: str ip, [sp]
00466cc8: bl #0x30e004
00466ccc: b #0x466b74
00466cd0: subseq sp, r2, r8, ror #30
00466cd4: andeq r2, r0, r4, ror r2
00466cd8: andeq r3, r0, r0, asr #19
00466cdc: andeq r1, r0, r0, asr #19
00466ce0: subeq r7, r5, r8, ror #15
00466ce4: subeq r6, r6, r4, lsl #14
00466ce8: subeq r6, r6, r8, lsl #13
00466cec: umaaleq r7, r5, r8, r7
00466cf0: subeq r6, r6, ip, ror #13
00466cf4: subeq r6, r6, r8, lsr r6
00466cf8: subeq r7, r5, r8, asr r7
00466cfc: strheq r6, [r6], #-0x6c
00466d00: strdeq r6, r7, [r6], #-0x58
00466d04: subeq r7, r5, r4, lsr #14
00466d08: subeq r6, r6, r0, asr r6
00466d0c: subeq r6, r6, r4, asr #11

0x464f4c _ZN14PlayerSavegame5_LoadEi
00464f4c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00464f50: ldr r5, [pc, #0x40c]
00464f54: ldr r7, [pc, #0x40c]
00464f58: ldr r8, [r0, #8]
00464f5c: add r5, pc, r5
00464f60: ldr r3, [r5, r7]
00464f64: sub sp, sp, #0x2c
00464f68: cmp r8, #0
00464f6c: ldr r3, [r3]
00464f70: mov r4, r0
00464f74: mov r6, r1
00464f78: str r3, [sp, #0x24]
00464f7c: beq #0x4652f0
00464f80: tst r6, #1
00464f84: beq #0x46508c
00464f88: ldr r0, [r4, #8]
00464f8c: cmp r0, #0
00464f90: beq #0x46508c
00464f94: ldr r3, [pc, #0x3d0]
00464f98: ldr r1, [pc, #0x3d0]
00464f9c: str r4, [sp]
00464fa0: ldr r2, [r5, r3]
00464fa4: ldr r3, [pc, #0x3c8]
00464fa8: add r1, pc, r1
00464fac: ldr r3, [r5, r3]
00464fb0: bl #0x315848
00464fb4: ldr r3, [pc, #0x3bc]
00464fb8: ldr r1, [pc, #0x3bc]
00464fbc: ldr r0, [r4, #8]
00464fc0: ldr r2, [r5, r3]
00464fc4: ldr r3, [pc, #0x3b4]
00464fc8: add r1, pc, r1
00464fcc: str r4, [sp]
00464fd0: ldr r3, [r5, r3]
00464fd4: bl #0x315848
00464fd8: ldr r3, [pc, #0x3a4]
00464fdc: ldr r1, [pc, #0x3a4]
00464fe0: ldr r0, [r4, #8]
00464fe4: ldr r2, [r5, r3]
00464fe8: ldr r3, [pc, #0x39c]
00464fec: add r1, pc, r1
00464ff0: str r4, [sp]
00464ff4: ldr r3, [r5, r3]
00464ff8: bl #0x315848
00464ffc: ldr r3, [pc, #0x38c]
00465000: ldr r1, [pc, #0x38c]
00465004: ldr r0, [r4, #8]
00465008: ldr r2, [r5, r3]
0046500c: ldr r3, [pc, #0x384]
00465010: add r1, pc, r1
00465014: str r4, [sp]
00465018: ldr r3, [r5, r3]
0046501c: bl #0x315848
00465020: ldr r3, [pc, #0x374]
00465024: ldr r1, [pc, #0x374]
00465028: ldr r0, [r4, #8]
0046502c: ldr r2, [r5, r3]
00465030: ldr r3, [pc, #0x36c]
00465034: add r1, pc, r1
00465038: str r4, [sp]
0046503c: ldr r3, [r5, r3]
00465040: bl #0x315848
00465044: ldr r3, [pc, #0x35c]
00465048: ldr r1, [pc, #0x35c]
0046504c: ldr r0, [r4, #8]
00465050: ldr r2, [r5, r3]
00465054: ldr r3, [pc, #0x354]
00465058: add r1, pc, r1
0046505c: str r4, [sp]
00465060: ldr r3, [r5, r3]
00465064: bl #0x315848
00465068: ldr r3, [pc, #0x344]
0046506c: ldr r1, [pc, #0x344]
00465070: ldr r0, [r4, #8]
00465074: ldr r2, [r5, r3]
00465078: ldr r3, [pc, #0x33c]
0046507c: add r1, pc, r1
00465080: str r4, [sp]
00465084: ldr r3, [r5, r3]
00465088: bl #0x315848
0046508c: tst r6, #2
00465090: bne #0x4652c4
00465094: tst r6, #4
00465098: beq #0x4651f8
0046509c: ldr r0, [r4, #8]
004650a0: cmp r0, #0
004650a4: beq #0x4651f8
004650a8: ldr r3, [pc, #0x310]
004650ac: ldr r1, [pc, #0x310]
004650b0: str r4, [sp]
004650b4: ldr r2, [r5, r3]
004650b8: ldr r3, [pc, #0x308]
004650bc: add r1, pc, r1
004650c0: ldr r3, [r5, r3]
004650c4: bl #0x315848
004650c8: ldr r3, [pc, #0x2fc]
004650cc: ldr r1, [pc, #0x2fc]
004650d0: ldr r0, [r4, #8]
004650d4: ldr r2, [r5, r3]
004650d8: ldr r3, [pc, #0x2f4]
004650dc: add r1, pc, r1
004650e0: str r4, [sp]
004650e4: ldr r3, [r5, r3]
004650e8: bl #0x315848
004650ec: ldr r3, [pc, #0x2e4]
004650f0: ldr r1, [pc, #0x2e4]
004650f4: ldr r0, [r4, #8]
004650f8: ldr r2, [r5, r3]
004650fc: ldr r3, [pc, #0x2dc]
00465100: add r1, pc, r1
00465104: str r4, [sp]
00465108: ldr r3, [r5, r3]
0046510c: bl #0x315848
00465110: ldr r8, [r4, #8]
00465114: bl #0x7fd794
00465118: ldrb r3, [r0, #5]
0046511c: cmp r3, #0
00465120: beq #0x465148
00465124: ldr r3, [pc, #0x2b8]
00465128: ldr r3, [r5, r3]
0046512c: ldr r3, [r3, #0x40]
00465130: ldrb r3, [r3, #0x71b]
00465134: cmp r3, #0
00465138: bne #0x465148
0046513c: ldr r3, [pc, #0x2a4]
00465140: ldr r2, [r5, r3]
00465144: b #0x46514c
00465148: mov r2, #0
0046514c: ldr r3, [pc, #0x298]
00465150: ldr r1, [pc, #0x298]
00465154: mov r0, r8
00465158: ldr r3, [r5, r3]
0046515c: add r1, pc, r1
00465160: str r4, [sp]
00465164: bl #0x315848
00465168: ldr r3, [pc, #0x284]
0046516c: ldr r1, [pc, #0x284]
00465170: ldr r0, [r4, #8]
00465174: ldr r2, [r5, r3]
00465178: ldr r3, [pc, #0x27c]
0046517c: add r1, pc, r1
00465180: str r4, [sp]
00465184: ldr r3, [r5, r3]
00465188: bl #0x315848
0046518c: ldr r3, [pc, #0x26c]
00465190: ldr r1, [pc, #0x26c]
00465194: ldr r0, [r4, #8]
00465198: ldr r2, [r5, r3]
0046519c: ldr r3, [pc, #0x264]
004651a0: add r1, pc, r1
004651a4: str r4, [sp]
004651a8: ldr r3, [r5, r3]
004651ac: bl #0x315848
004651b0: ldr r3, [pc, #0x254]
004651b4: ldr r1, [pc, #0x254]
004651b8: ldr r0, [r4, #8]
004651bc: ldr r2, [r5, r3]
004651c0: ldr r3, [pc, #0x24c]
004651c4: add r1, pc, r1
004651c8: str r4, [sp]
004651cc: ldr r3, [r5, r3]
004651d0: bl #0x315848
004651d4: ldr r3, [pc, #0x23c]
004651d8: ldr r1, [pc, #0x23c]
004651dc: ldr r0, [r4, #8]
004651e0: ldr r2, [r5, r3]
004651e4: ldr r3, [pc, #0x234]
004651e8: add r1, pc, r1
004651ec: str r4, [sp]
004651f0: ldr r3, [r5, r3]
004651f4: bl #0x315848
004651f8: tst r6, #8
004651fc: beq #0x46522c
00465200: ldr r0, [r4, #8]
00465204: cmp r0, #0
00465208: beq #0x46522c
0046520c: ldr r3, [pc, #0x1b8]
00465210: ldr r1, [pc, #0x20c]
00465214: str r4, [sp]
00465218: ldr r2, [r5, r3]
0046521c: ldr r3, [pc, #0x1b0]
00465220: add r1, pc, r1
00465224: ldr r3, [r5, r3]
00465228: bl #0x315848
0046522c: tst r6, #0x20
00465230: beq #0x465260
00465234: ldr r0, [r4, #8]
00465238: cmp r0, #0
0046523c: beq #0x465260
00465240: ldr r3, [pc, #0x1b8]
00465244: ldr r1, [pc, #0x1dc]
00465248: str r4, [sp]
0046524c: ldr r2, [r5, r3]
00465250: ldr r3, [pc, #0x1b0]
00465254: add r1, pc, r1
00465258: ldr r3, [r5, r3]
0046525c: bl #0x315848
00465260: tst r6, #0x10
00465264: beq #0x4652a8
00465268: ldr r3, [r4, #8]
0046526c: cmp r3, #0
00465270: beq #0x4652a8
00465274: add r0, r4, #0xb8
00465278: bl #0x46c1a8
0046527c: add r0, r4, #0x118
00465280: bl #0x46c1a8
00465284: ldr r3, [pc, #0x168]
00465288: ldr r1, [pc, #0x19c]
0046528c: ldr r0, [r4, #8]
00465290: ldr r2, [r5, r3]
00465294: ldr r3, [pc, #0x160]
00465298: add r1, pc, r1
0046529c: str r4, [sp]
004652a0: ldr r3, [r5, r3]
004652a4: bl #0x315848
004652a8: ldr r3, [r5, r7]
004652ac: ldr r2, [sp, #0x24]
004652b0: ldr r3, [r3]
004652b4: cmp r2, r3
004652b8: bne #0x465360
004652bc: add sp, sp, #0x2c
004652c0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004652c4: mov r0, r4
004652c8: bl #0x46954c
004652cc: mov r0, r4
004652d0: bl #0x469764
004652d4: mov r0, r4
004652d8: bl #0x4694c8
004652dc: add r0, r4, #0xb8
004652e0: bl #0x46c1a8
004652e4: add r0, r4, #0x118
004652e8: bl #0x46c1a8
004652ec: b #0x465094
004652f0: ldr r3, [r0, #4]
004652f4: cmn r3, #1
004652f8: beq #0x464f80
004652fc: add sl, sp, #0xc
00465300: mov r0, sl
00465304: mov r1, #0x10
00465308: str sl, [sp, #0x1c]
0046530c: str sl, [sp, #0x20]
00465310: bl #0x31167c
00465314: ldr r2, [sp, #0x1c]
00465318: mov r3, r8
0046531c: mov r1, sl
00465320: strb r8, [r2]
00465324: ldr r0, [r4, #4]
00465328: mov r2, r8
0046532c: bl #0x463c84
00465330: mov r1, r8
00465334: mov r0, #0x3c
00465338: ldr fp, [sp, #0x20]
0046533c: bl #0x310570
00465340: mov r1, fp
00465344: mov sb, r0
00465348: mov r2, r8
0046534c: bl #0x315ed8
00465350: str sb, [r4, #8]
00465354: mov r0, sl
00465358: bl #0x3139ac
0046535c: b #0x464f80
00465360: bl #0x30e310
00465364: subseq pc, r2, r4, lsr fp
00465368: andeq r4, r0, ip, lsr #1
0046536c: andeq r1, r0, r8, lsl #10
00465370: subeq r8, r6, r0, asr #4
00465374: strdeq r1, r2, [r0], -r4
00465378: andeq r1, r0, r8, ror #30
0046537c: subeq r8, r6, r8, lsr #4
00465380: strdeq r1, r2, [r0], -r0
00465384: strheq r2, [r0], -ip
00465388: subeq r8, r6, ip, lsl #4
0046538c: andeq r2, r0, r8, asr #29
00465390: andeq r0, r0, r8, lsr #26
00465394: strdeq r8, sb, [r6], #-0x10
00465398: strheq r1, [r0], -r8
0046539c: andeq r3, r0, r4, lsr #12
004653a0: ldrdeq r8, sb, [r6], #-0x14
004653a4: andeq r3, r0, r8, ror r7
004653a8: andeq r3, r0, ip, asr #26
004653ac: strheq r8, [r6], #-0x18
004653b0: strheq r2, [r0], -r0
004653b4: andeq r4, r0, r0, lsr r0
004653b8: umaaleq r8, r6, ip, r1
004653bc: andeq r2, r0, ip, ror #12
004653c0: andeq r3, r0, r0, lsl #30
004653c4: subeq r8, r6, r4, ror #2
004653c8: andeq r4, r0, ip, ror r6
004653cc: muleq r0, r4, r8
004653d0: subeq r8, r6, ip, asr #2
004653d4: andeq r1, r0, r4, lsr fp
004653d8: andeq r1, r0, ip, lsr r3
004653dc: subeq r8, r6, r0, lsr r1
004653e0: andeq r0, r0, r4, lsr #14
004653e4: strdeq r3, r4, [r0], -r4
004653e8: andeq r2, r0, ip, asr #21
004653ec: strdeq r2, r3, [r0], -r4
004653f0: ldrdeq r8, sb, [r6], #-0xc
004653f4: andeq r2, r0, r8, asr #12
004653f8: subeq r8, r6, r4, asr #1
004653fc: andeq r3, r0, r8, lsl r4
00465400: andeq r0, r0, r4, asr #14
00465404: subeq r8, r6, r8, lsr #1
00465408: andeq r4, r0, ip, ror #23
0046540c: andeq r2, r0, ip, lsr r4
00465410: subeq r8, r6, ip, lsl #1
00465414: andeq r4, r0, r8, lsl #5
00465418: andeq r0, r0, ip, lsl #29
0046541c: subeq r8, r6, r0, ror r0
00465420: andeq r1, r0, r0, ror #24
00465424: subeq r8, r6, r8
00465428: strdeq r7, r8, [r6], #-0xf4
0046542c: subeq r7, r6, r8, lsr #31

0x468574 _ZN14PlayerSavegame22_LoadVolatileQuestsLogEi
00468574: push {r4, r5, r6, r7, r8, lr}
00468578: ldr r4, [pc, #0xa4]
0046857c: tst r1, #0x14
00468580: mov r5, r0
00468584: add r4, pc, r4
00468588: bne #0x468590
0046858c: pop {r4, r5, r6, r7, r8, pc}
00468590: bl #0x7fd794
00468594: ldrb r3, [r0, #5]
00468598: cmp r3, #0
0046859c: beq #0x46858c
004685a0: ldr r3, [pc, #0x80]
004685a4: ldr r6, [r4, r3]
004685a8: ldr r0, [r6, #0x40]
004685ac: bl #0x36f074
004685b0: cmp r0, #0
004685b4: ldreq r7, [r6, #0x40]
004685b8: bne #0x468610
004685bc: add r6, r7, #0x6e0
004685c0: ldr r3, [r7, #0x6e0]
004685c4: mov r0, r6
004685c8: mov lr, pc
004685cc: ldr pc, [r3, #8]
004685d0: orrs r1, r0, r1
004685d4: beq #0x46858c
004685d8: ldr r1, [r7, #0x6e0]
004685dc: mov r0, r6
004685e0: mov r2, #0
004685e4: mov r3, #0
004685e8: mov lr, pc
004685ec: ldr pc, [r1, #0x20]
004685f0: ldr r3, [pc, #0x34]
004685f4: add r0, r5, #0x118
004685f8: mov r2, r6
004685fc: ldr r1, [r4, r3]
00468600: mov r3, #0
00468604: ldr r1, [r1]
00468608: pop {r4, r5, r6, r7, r8, lr}
0046860c: b #0x46c48c
00468610: ldr r7, [r6, #0x40]
00468614: ldrb r3, [r7, #0x719]
00468618: cmp r3, #0
0046861c: beq #0x46858c
00468620: b #0x4685bc
00468624: subseq ip, r2, ip, lsl #10
00468628: strdeq r3, r4, [r0], -r4
0046862c: muleq r0, ip, sl

0x46c658 _ZN13QuestSavegame10PackQuestsEiP11IStreamBase
0046c658: push {r4, r5, r6, r7, lr}
0046c65c: mov r6, r0
0046c660: mov r0, #0xc
0046c664: mla r0, r0, r1, r6
0046c668: sub sp, sp, #0xc
0046c66c: ldmib r0, {r3, ip}
0046c670: mov r7, r1
0046c674: add r1, sp, #8
0046c678: rsb r3, r3, ip
0046c67c: asr r3, r3, #2
0046c680: str r3, [r1, #-4]!
0046c684: mov r0, r2
0046c688: mov r5, r2
0046c68c: bl #0x461770
0046c690: ldr r3, [sp, #4]
0046c694: cmp r3, #0
0046c698: beq #0x46c6c4
0046c69c: mov r4, #0
0046c6a0: mov r1, r4
0046c6a4: mov r3, r5
0046c6a8: mov r0, r6
0046c6ac: mov r2, r7
0046c6b0: bl #0x46c584
0046c6b4: ldr r3, [sp, #4]
0046c6b8: add r4, r4, #1
0046c6bc: cmp r3, r4
0046c6c0: bhi #0x46c6a0
0046c6c4: add r1, r6, r7, lsl #2
0046c6c8: mov r0, r5
0046c6cc: add r1, r1, #0x2c
0046c6d0: bl #0x38b808
0046c6d4: add r1, r7, #0xe
0046c6d8: add r1, r6, r1, lsl #2
0046c6dc: mov r0, r5
0046c6e0: add r6, r6, r7, lsl #2
0046c6e4: bl #0x38b808
0046c6e8: mov r0, r5
0046c6ec: add r1, r6, #0x44
0046c6f0: bl #0x38b808
0046c6f4: add sp, sp, #0xc
0046c6f8: pop {r4, r5, r6, r7, pc}

0x315110 _ZN8Savegame6AddJobERNS_3JobE
00315110: ldr r3, [pc, #0xcc]
00315114: ldr r2, [pc, #0xcc]
00315118: push {r4, r5, r6, r7, r8, sl, lr}
0031511c: add r3, pc, r3
00315120: ldr r2, [r3, r2]
00315124: sub sp, sp, #0x14
00315128: mov r5, r0
0031512c: ldr r4, [r2]
00315130: mov r7, r2
00315134: mov sl, sp
00315138: cmp r4, r7
0031513c: add r8, sp, #0xc
00315140: beq #0x315174
00315144: ldr r1, [r4, #0x20]
00315148: ldr r3, [r4, #0x1c]
0031514c: ldr r0, [r5, #0x18]
00315150: ldr r2, [r5, #0x14]
00315154: rsb r3, r1, r3
00315158: ldr r6, [r4]
0031515c: rsb r2, r0, r2
00315160: cmp r2, r3
00315164: beq #0x3151b0
00315168: mov r4, r6
0031516c: cmp r4, r7
00315170: bne #0x315144
00315174: mov r3, #0x28
00315178: add r0, sp, #0x10
0031517c: str r3, [r0, #-8]!
00315180: bl #0x708ec0
00315184: mov r1, r5
00315188: mov r6, r0
0031518c: add r0, r0, #8
00315190: bl #0x3145e0
00315194: ldr r3, [r4, #4]
00315198: str r4, [r6]
0031519c: str r3, [r6, #4]
003151a0: str r6, [r3]
003151a4: str r6, [r4, #4]
003151a8: add sp, sp, #0x14
003151ac: pop {r4, r5, r6, r7, r8, sl, pc}
003151b0: bl #0x30e5e0
003151b4: cmp r0, #0
003151b8: bne #0x315168
003151bc: ldrb r3, [r4, #0x24]
003151c0: ldrb r2, [r5, #0x1c]
003151c4: cmp r2, r3
003151c8: bne #0x315168
003151cc: mov r0, sp
003151d0: mov r1, r8
003151d4: str r4, [sp, #0xc]
003151d8: bl #0x3140b0
003151dc: mov r4, r6
003151e0: b #0x31516c
003151e4: rsbeq pc, r7, r4, ror sb
003151e8: andeq r2, r0, r4, lsl r4

0x3139e0 _ZN12StreamReader7writeAsIjEEvP11IStreamBasePKT_
003139e0: str lr, [sp, #-4]!
003139e4: mov r3, #0
003139e8: sub sp, sp, #0xc
003139ec: ldr ip, [r0]
003139f0: mov r2, #4
003139f4: mov lr, pc
003139f8: ldr pc, [ip, #0x1c]
003139fc: ldr r3, [pc, #0x74]
00313a00: cmp r0, #4
00313a04: add r3, pc, r3
00313a08: beq #0x313a38
00313a0c: ldr r2, [pc, #0x68]
00313a10: ldr r2, [r3, r2]
00313a14: ldr r2, [r2]
00313a18: cmp r2, #2
00313a1c: moveq r3, #0
00313a20: streq r3, [r3]
00313a24: beq #0x313a30
00313a28: cmp r2, #1
00313a2c: beq #0x313a44
00313a30: add sp, sp, #0xc
00313a34: ldm sp!, {pc}
00313a38: cmp r1, #0
00313a3c: beq #0x313a30
00313a40: b #0x313a0c
00313a44: ldr r0, [pc, #0x34]
00313a48: ldr r1, [pc, #0x34]
00313a4c: ldr r2, [pc, #0x34]
00313a50: ldr r0, [r3, r0]
00313a54: ldr r3, [pc, #0x30]
00313a58: mov ip, #0x80
00313a5c: add r1, pc, r1
00313a60: add r2, pc, r2
00313a64: add r3, pc, r3
00313a68: add r0, r0, #0xa8
00313a6c: str ip, [sp]
00313a70: bl #0x30e004
00313a74: b #0x313a30
00313a78: rsbeq r1, r8, ip, lsl #1
00313a7c: andeq r3, r0, r0, asr #19
00313a80: andeq r1, r0, r0, asr #19
00313a84: subseq sl, sl, ip, ror sb
00313a88: subseq sl, sl, r0, asr #20
00313a8c: subseq sl, sl, ip, asr sl

0x317604 _ZN12StreamReader13writeStringExEP11IStreamBasePKcy
00317604: push {r4, lr}
00317608: ldr ip, [r0]
0031760c: mov lr, pc
00317610: ldr pc, [ip, #0x1c]
00317614: mov r0, #1
00317618: pop {r4, pc}

0x313c90 _ZN8Savegame3JobD1Ev
00313c90: push {r4, lr}
00313c94: ldrb r3, [r0, #0x1d]
00313c98: mov r4, r0
00313c9c: cmp r3, #0
00313ca0: beq #0x313cc0
00313ca4: ldr r3, [r0]
00313ca8: cmp r3, #0
00313cac: beq #0x313cc0
00313cb0: mov r0, r3
00313cb4: ldr r3, [r3]
00313cb8: mov lr, pc
00313cbc: ldr pc, [r3, #4]
00313cc0: mov r3, #0
00313cc4: add r0, r4, #4
00313cc8: strb r3, [r4, #0x1d]
00313ccc: str r3, [r4]
00313cd0: bl #0x3139ac
00313cd4: mov r0, r4
00313cd8: pop {r4, pc}

0x46b644 _ZN13QuestSavegame11SynchronizeERKS_b
0046b644: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046b648: sub sp, sp, #0x14
0046b64c: str r0, [sp, #8]
0046b650: ldr r0, [r0, #0x5c]
0046b654: str r1, [sp, #0xc]
0046b658: cmp r0, #0
0046b65c: beq #0x46b754
0046b660: ldr r3, [r1, #0x5c]
0046b664: cmp r3, #0
0046b668: beq #0x46b754
0046b66c: cmp r2, #0
0046b670: bne #0x46b738
0046b674: ldr r0, [sp, #8]
0046b678: mov sb, r2
0046b67c: str r3, [r0, #0x5c]
0046b680: ldr sl, [sp, #0xc]
0046b684: ldr r8, [sp, #8]
0046b688: str sl, [sp, #4]
0046b68c: mov fp, r8
0046b690: ldr r3, [sl, #0x2c]
0046b694: str r3, [r8, #0x2c]
0046b698: ldr r3, [sl, #0x38]
0046b69c: str r3, [r8, #0x38]
0046b6a0: ldr r3, [sl, #0x44]
0046b6a4: str r3, [r8, #0x44]
0046b6a8: ldr r1, [sp, #4]
0046b6ac: ldrb r3, [r1, #0x28]
0046b6b0: cmp r3, #0
0046b6b4: strb r3, [fp, #0x28]
0046b6b8: beq #0x46b708
0046b6bc: ldr r2, [sp, #8]
0046b6c0: add r5, r2, sb
0046b6c4: ldmib r5, {r2, r6}
0046b6c8: rsb r6, r2, r6
0046b6cc: asrs r6, r6, #2
0046b6d0: beq #0x46b708
0046b6d4: ldr r3, [sp, #0xc]
0046b6d8: mov r4, #0
0046b6dc: add r7, r3, sb
0046b6e0: b #0x46b6e8
0046b6e4: ldr r2, [r5, #4]
0046b6e8: ldr r3, [r7, #4]
0046b6ec: ldr r0, [r2, r4, lsl #2]
0046b6f0: mov r2, #0
0046b6f4: ldr r1, [r3, r4, lsl #2]
0046b6f8: add r4, r4, #1
0046b6fc: bl #0x4802b4
0046b700: cmp r4, r6
0046b704: bne #0x46b6e4
0046b708: ldr r0, [sp, #4]
0046b70c: add sb, sb, #0xc
0046b710: cmp sb, #0x24
0046b714: add r0, r0, #1
0046b718: add sl, sl, #4
0046b71c: add r8, r8, #4
0046b720: str r0, [sp, #4]
0046b724: add fp, fp, #1
0046b728: bne #0x46b690
0046b72c: mov r0, #1
0046b730: add sp, sp, #0x14
0046b734: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046b738: bl #0x3bb8e4
0046b73c: mov r4, r0
0046b740: mov r1, r4
0046b744: ldr r0, [sp, #0xc]
0046b748: bl #0x46b5dc
0046b74c: cmp r0, #0
0046b750: bne #0x46b75c
0046b754: mov r0, #0
0046b758: b #0x46b730
0046b75c: ldr r0, [sp, #8]
0046b760: add r6, r4, #0x10
0046b764: lsl r6, r6, #2
0046b768: add r5, r0, r6
0046b76c: ldr r1, [r5, #4]
0046b770: ldr r0, [sp, #0xc]
0046b774: mov r2, r4
0046b778: bl #0x46b144
0046b77c: cmp r0, #0
0046b780: beq #0x46b754
0046b784: ldr r2, [sp, #0xc]
0046b788: ldr r0, [sp, #8]
0046b78c: add r3, r4, #0xe
0046b790: ldr r1, [r2, r3, lsl #2]
0046b794: add r2, r0, r4, lsl #2
0046b798: ldr r0, [sp, #0xc]
0046b79c: str r1, [r2, #0x2c]
0046b7a0: ldr r1, [r0, r3, lsl #2]
0046b7a4: add r6, r0, r6
0046b7a8: add r2, r0, r4
0046b7ac: ldr r0, [sp, #8]
0046b7b0: str r1, [r0, r3, lsl #2]
0046b7b4: ldr r1, [r6, #4]
0046b7b8: mov r3, #0xc
0046b7bc: mul r3, r3, r4
0046b7c0: str r1, [r5, #4]
0046b7c4: ldrb r2, [r2, #0x28]
0046b7c8: add r4, r0, r4
0046b7cc: add r7, r0, r3
0046b7d0: strb r2, [r4, #0x28]
0046b7d4: ldmib r7, {r2, r6}
0046b7d8: rsb r6, r2, r6
0046b7dc: asrs r6, r6, #2
0046b7e0: beq #0x46b72c
0046b7e4: ldr r1, [sp, #0xc]
0046b7e8: add r7, r7, #4
0046b7ec: mov r4, #0
0046b7f0: add r3, r1, r3
0046b7f4: add r5, r3, #4
0046b7f8: b #0x46b800
0046b7fc: ldr r2, [r7]
0046b800: ldr r3, [r5]
0046b804: ldr r0, [r2, r4, lsl #2]
0046b808: mov r2, #1
0046b80c: ldr r1, [r3, r4, lsl #2]
0046b810: add r4, r4, r2
0046b814: bl #0x4802b4
0046b818: cmp r4, r6
0046b81c: bne #0x46b7fc
0046b820: b #0x46b72c

0x315904 _ZN8Savegame15initSectionInfoEPKcPFvP11IStreamBasePvES6_S4_
00315904: push {r4, r5, r6, r7, r8, lr}
00315908: sub sp, sp, #8
0031590c: add r4, sp, #8
00315910: str r1, [r4, #-4]!
00315914: add r5, r0, #0x20
00315918: mov r0, r5
0031591c: mov r1, r4
00315920: mov r6, r2
00315924: mov r7, r3
00315928: ldr r8, [sp, #0x20]
0031592c: bl #0x314120
00315930: cmp r0, r5
00315934: strne r8, [r0, #0x3c]
00315938: strne r6, [r0, #0x34]
0031593c: strne r7, [r0, #0x38]
00315940: beq #0x31594c
00315944: add sp, sp, #8
00315948: pop {r4, r5, r6, r7, r8, pc}
0031594c: mov r1, r4
00315950: bl #0x3156f0
00315954: mov r3, #0
00315958: mov r2, #0
0031595c: strd r2, r3, [r0]
00315960: mov r3, #0
00315964: str r8, [r0, #0x14]
00315968: str r7, [r0, #0x10]
0031596c: str r6, [r0, #0xc]
00315970: str r3, [r0, #8]
00315974: b #0x315944

0x4622c8 _ZN13LevelSavegame21GetCheckpointFilenameEjibRSs
004622c8: push {r4, r5, r6, r7, r8, lr}
004622cc: ldr r4, [pc, #0xa4]
004622d0: ldr r6, [pc, #0xa4]
004622d4: cmp r2, #0
004622d8: add r4, pc, r4
004622dc: ldr r2, [r4, r6]
004622e0: sub sp, sp, #0x58
004622e4: mov r7, r1
004622e8: ldr r2, [r2]
004622ec: mov r8, r3
004622f0: str r2, [sp, #0x54]
004622f4: bne #0x462368
004622f8: ldr lr, [pc, #0x80]
004622fc: add lr, pc, lr
00462300: ldr ip, [pc, #0x7c]
00462304: ldr r1, [pc, #0x7c]
00462308: ldr r2, [pc, #0x7c]
0046230c: add r5, sp, #0x14
00462310: mov r3, r0
00462314: add ip, pc, ip
00462318: add r1, pc, r1
0046231c: add r2, pc, r2
00462320: mov r0, r5
00462324: str lr, [sp, #4]
00462328: str ip, [sp, #8]
0046232c: str r7, [sp]
00462330: bl #0x30eae4
00462334: mov r0, r5
00462338: bl #0x30de54
0046233c: mov r1, r5
00462340: add r2, r5, r0
00462344: mov r0, r8
00462348: bl #0x3109e0
0046234c: ldr r3, [r4, r6]
00462350: ldr r2, [sp, #0x54]
00462354: ldr r3, [r3]
00462358: cmp r2, r3
0046235c: bne #0x462374
00462360: add sp, sp, #0x58
00462364: pop {r4, r5, r6, r7, r8, pc}
00462368: ldr lr, [pc, #0x20]
0046236c: add lr, pc, lr
00462370: b #0x462300
00462374: bl #0x30e310
00462378: ldrheq r2, [r3], #-0x78
0046237c: andeq r4, r0, ip, lsr #1
00462380: subeq sl, r6, ip, ror #28
00462384: subeq sl, r6, r4, ror lr
00462388: subeq sl, r6, r0, ror #28
0046238c: subeq ip, r5, ip, lsr r2
00462390: subeq sl, r6, r4, lsl #28

0x4624e4 _ZN13LevelSavegame11GetFilenameEjiiiRSs
004624e4: push {r4, r5, r6, r7, r8, lr}
004624e8: ldr ip, [pc, #0x98]
004624ec: ldr lr, [pc, #0x98]
004624f0: sub sp, sp, #0x410
004624f4: add ip, pc, ip
004624f8: ldr r5, [ip, lr]
004624fc: sub sp, sp, #8
00462500: bic r6, r2, r2, asr #31
00462504: bic r7, r1, r1, asr #31
00462508: ldr lr, [pc, #0x80]
0046250c: ldr r1, [pc, #0x80]
00462510: ldr r2, [pc, #0x80]
00462514: ldr r8, [r5]
00462518: add r4, sp, #0x18
0046251c: sub r4, r4, #4
00462520: add lr, pc, lr
00462524: add r1, pc, r1
00462528: add r2, pc, r2
0046252c: str r3, [sp]
00462530: mov r3, r0
00462534: mov r0, r4
00462538: str lr, [sp, #0xc]
0046253c: str r6, [sp, #8]
00462540: str r8, [sp, #0x414]
00462544: ldr r6, [sp, #0x430]
00462548: str r7, [sp, #4]
0046254c: bl #0x30eae4
00462550: mov r0, r4
00462554: bl #0x30de54
00462558: mov r1, r4
0046255c: add r2, r4, r0
00462560: mov r0, r6
00462564: bl #0x3109e0
00462568: ldr r2, [sp, #0x414]
0046256c: ldr r3, [r5]
00462570: cmp r2, r3
00462574: bne #0x462584
00462578: add sp, sp, #0x18
0046257c: add sp, sp, #0x400
00462580: pop {r4, r5, r6, r7, r8, pc}
00462584: bl #0x30e310

0x46355c _ZN14PlayerSavegame9SG_DeleteEPKc
0046355c: ldr r3, [pc, #0x38]
00463560: subs r1, r0, #0
00463564: push {r4, lr}
00463568: add r3, pc, r3
0046356c: beq #0x463594
00463570: ldr r2, [pc, #0x28]
00463574: ldr r3, [r3, r2]
00463578: ldr r3, [r3, #0x10]
0046357c: ldr r3, [r3, #0x34]
00463580: mov r0, r3
00463584: ldr r3, [r3]
00463588: mov lr, pc
0046358c: ldr pc, [r3, #0x9c]
00463590: pop {r4, pc}
00463594: mov r0, r1
00463598: pop {r4, pc}
0046359c: subseq r1, r3, r8, lsr #10
004635a0: strdeq r3, r4, [r0], -r4

0x314380 _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE7_M_findIA5_cEEPNS_18_Rb_tree_node_baseERKT_
00314380: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00314384: ldr fp, [pc, #0x15c]
00314388: ldr r2, [pc, #0x15c]
0031438c: sub sp, sp, #0x54
00314390: add fp, pc, fp
00314394: ldr r3, [fp, r2]
00314398: str r2, [sp, #0xc]
0031439c: str r0, [sp, #8]
003143a0: ldr r4, [r0, #4]
003143a4: ldr r3, [r3]
003143a8: mov r8, r1
003143ac: cmp r4, #0
003143b0: str r3, [sp, #0x4c]
003143b4: beq #0x3144bc
003143b8: mov sl, r0
003143bc: add r7, sp, #0x34
003143c0: add sb, sp, #0x18
003143c4: mov r1, r8
003143c8: mov r2, sb
003143cc: mov r0, r7
003143d0: bl #0x3140ec
003143d4: ldr r3, [r4, #0x24]
003143d8: ldr r1, [sp, #0x48]
003143dc: ldr r6, [r4, #0x20]
003143e0: ldr r5, [sp, #0x44]
003143e4: mov r0, r3
003143e8: rsb r6, r3, r6
003143ec: rsb r5, r1, r5
003143f0: cmp r5, r6
003143f4: movlt r2, r5
003143f8: movge r2, r6
003143fc: bl #0x30e5e0
00314400: subs r3, r0, #0
00314404: bne #0x31441c
00314408: cmp r6, r5
0031440c: mvnlt r3, #0
00314410: blt #0x31441c
00314414: movle r3, #0
00314418: movgt r3, #1
0031441c: mov r0, r7
00314420: str r3, [sp, #4]
00314424: bl #0x3139ac
00314428: ldr r3, [sp, #4]
0031442c: cmp r3, #0
00314430: movge sl, r4
00314434: ldrlt r4, [r4, #0xc]
00314438: ldrge r4, [r4, #8]
0031443c: cmp r4, #0
00314440: bne #0x3143c4
00314444: ldr r3, [sp, #8]
00314448: cmp sl, r3
0031444c: beq #0x3144c0
00314450: add r4, sp, #0x1c
00314454: mov r1, r8
00314458: add r2, sp, #0x14
0031445c: mov r0, r4
00314460: bl #0x3140ec
00314464: ldr r3, [sp, #0x30]
00314468: ldr r1, [sl, #0x24]
0031446c: ldr r5, [sl, #0x20]
00314470: ldr r6, [sp, #0x2c]
00314474: mov r0, r3
00314478: rsb r5, r1, r5
0031447c: rsb r6, r3, r6
00314480: cmp r5, r6
00314484: movlt r2, r5
00314488: movge r2, r6
0031448c: bl #0x30e5e0
00314490: subs r7, r0, #0
00314494: bne #0x3144ac
00314498: cmp r6, r5
0031449c: mvnlt r7, #0
003144a0: blt #0x3144ac
003144a4: movle r7, #0
003144a8: movgt r7, #1
003144ac: mov r0, r4
003144b0: bl #0x3139ac
003144b4: cmp r7, #0
003144b8: bge #0x3144c0
003144bc: ldr sl, [sp, #8]
003144c0: ldr r2, [sp, #0xc]
003144c4: mov r0, sl
003144c8: ldr r3, [fp, r2]
003144cc: ldr r2, [sp, #0x4c]
003144d0: ldr r3, [r3]
003144d4: cmp r2, r3
003144d8: bne #0x3144e4
003144dc: add sp, sp, #0x54
003144e0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003144e4: bl #0x30e310
003144e8: rsbeq r0, r8, r0, lsl #14
003144ec: andeq r4, r0, ip, lsr #1

0x315978 _ZNSt3mapISsN8Savegame11SectionInfoESt4lessISsESaISt4pairIKSsS1_EEEixIA5_cEERS1_RKT_
00315978: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0031597c: ldr r4, [pc, #0x144]
00315980: ldr r7, [pc, #0x144]
00315984: sub sp, sp, #0x84
00315988: add r4, pc, r4
0031598c: ldr r3, [r4, r7]
00315990: mov r8, r0
00315994: mov r6, r1
00315998: ldr r3, [r3]
0031599c: str r3, [sp, #0x7c]
003159a0: bl #0x3144f0
003159a4: cmp r0, r8
003159a8: mov r5, r0
003159ac: beq #0x315a40
003159b0: add sl, sp, #0x64
003159b4: mov r1, r6
003159b8: add r2, sp, #0x14
003159bc: mov r0, sl
003159c0: bl #0x3140ec
003159c4: ldr r3, [sp, #0x78]
003159c8: ldr r1, [r5, #0x24]
003159cc: ldr fp, [r5, #0x20]
003159d0: ldr sb, [sp, #0x74]
003159d4: mov r0, r3
003159d8: rsb fp, r1, fp
003159dc: rsb sb, r3, sb
003159e0: cmp fp, sb
003159e4: movlt r2, fp
003159e8: movge r2, sb
003159ec: bl #0x30e5e0
003159f0: cmp r0, #0
003159f4: mov r3, r5
003159f8: bne #0x315a34
003159fc: cmp sb, fp
00315a00: blt #0x315a38
00315a04: mov r0, sl
00315a08: str r3, [sp, #4]
00315a0c: bl #0x3139ac
00315a10: ldr r3, [sp, #4]
00315a14: ldr r1, [r4, r7]
00315a18: ldr r2, [sp, #0x7c]
00315a1c: add r0, r3, #0x28
00315a20: ldr r3, [r1]
00315a24: cmp r2, r3
00315a28: bne #0x315ac4
00315a2c: add sp, sp, #0x84
00315a30: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00315a34: bge #0x315a04
00315a38: mov r0, sl
00315a3c: bl #0x3139ac
00315a40: add sl, sp, #0x4c
00315a44: mov r1, r6
00315a48: add r2, sp, #0x10
00315a4c: add r6, sp, #0x18
00315a50: mov r0, sl
00315a54: bl #0x3140ec
00315a58: mov r0, r6
00315a5c: ldr r1, [sp, #0x60]
00315a60: ldr r2, [sp, #0x5c]
00315a64: str r6, [sp, #0x28]
00315a68: str r6, [sp, #0x2c]
00315a6c: mov sb, #0
00315a70: bl #0x3116e8
00315a74: mov ip, #0
00315a78: mov r1, r8
00315a7c: mov r3, r6
00315a80: add r2, sp, #8
00315a84: add r0, sp, #0xc
00315a88: mov r8, #0
00315a8c: str ip, [sp, #0x38]
00315a90: str ip, [sp, #0x44]
00315a94: str ip, [sp, #0x40]
00315a98: str ip, [sp, #0x3c]
00315a9c: str r5, [sp, #8]
00315aa0: strd r8, sb, [sp, #0x30]
00315aa4: bl #0x3151ec
00315aa8: ldr r5, [sp, #0xc]
00315aac: mov r0, r6
00315ab0: bl #0x3139ac
00315ab4: mov r0, sl
00315ab8: bl #0x3139ac
00315abc: mov r3, r5
00315ac0: b #0x315a14
00315ac4: bl #0x30e310
00315ac8: rsbeq pc, r7, r8, lsl #2
00315acc: andeq r4, r0, ip, lsr #1

0x463504 _ZN14PlayerSavegame33SG_GetCheckpointFilenameExtensionEv
00463504: ldr r0, [pc, #4]
00463508: add r0, pc, r0
0046350c: bx lr
00463510: strheq sb, [r6], #-0xc0

0x315848 _ZN8Savegame4loadEPKcPFvP11IStreamBasePvES6_S4_
00315848: push {r4, r5, r6, r7, r8, sl, lr}
0031584c: sub sp, sp, #0xc
00315850: add r4, sp, #8
00315854: str r1, [r4, #-4]!
00315858: add r5, r0, #0x20
0031585c: mov r6, r0
00315860: mov r1, r4
00315864: mov r0, r5
00315868: mov sl, r3
0031586c: mov r7, r2
00315870: ldr r8, [sp, #0x28]
00315874: bl #0x314120
00315878: cmp r0, r5
0031587c: mov r3, r0
00315880: beq #0x3158d8
00315884: ldr r2, [r0, #0x30]
00315888: str sl, [r0, #0x38]
0031588c: str r7, [r0, #0x34]
00315890: cmp r2, #0
00315894: str r8, [r0, #0x3c]
00315898: beq #0x3158d0
0031589c: ldr r1, [r6, #0x1c]
003158a0: cmp r1, #0
003158a4: beq #0x3158d0
003158a8: mov r0, r1
003158ac: ldrd r2, r3, [r3, #0x28]
003158b0: ldr r1, [r1]
003158b4: mov lr, pc
003158b8: ldr pc, [r1, #0x20]
003158bc: cmp r7, #0
003158c0: beq #0x3158d0
003158c4: ldr r0, [r6, #0x1c]
003158c8: mov r1, r8
003158cc: blx r7
003158d0: add sp, sp, #0xc
003158d4: pop {r4, r5, r6, r7, r8, sl, pc}
003158d8: mov r1, r4
003158dc: bl #0x3156f0
003158e0: mov r3, #0
003158e4: mov r2, #0
003158e8: strd r2, r3, [r0]
003158ec: mov r3, #0
003158f0: str r8, [r0, #0x14]
003158f4: str sl, [r0, #0x10]
003158f8: str r7, [r0, #0xc]
003158fc: str r3, [r0, #8]
00315900: b #0x3158d0

0x46c1a8 _ZN13QuestSavegame10InitQuestsEv
0046c1a8: ldr r1, [pc, #0x184]
0046c1ac: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046c1b0: ldr r3, [pc, #0x180]
0046c1b4: add r1, pc, r1
0046c1b8: sub sp, sp, #0x2c
0046c1bc: ldr r3, [r1, r3]
0046c1c0: ldr r2, [pc, #0x174]
0046c1c4: str r1, [sp, #8]
0046c1c8: str r3, [sp, #0x14]
0046c1cc: ldr r3, [pc, #0x16c]
0046c1d0: mov r1, #0
0046c1d4: str r2, [sp, #0x1c]
0046c1d8: add r2, sp, #0x24
0046c1dc: str r3, [sp, #0x10]
0046c1e0: str r1, [sp, #0xc]
0046c1e4: mov r6, r0
0046c1e8: str r1, [sp, #4]
0046c1ec: str r2, [sp, #0x18]
0046c1f0: ldr r3, [sp, #0xc]
0046c1f4: ldr r1, [sp, #0x14]
0046c1f8: add r4, r6, r3
0046c1fc: ldmib r4, {r3, sl}
0046c200: ldr r5, [r1]
0046c204: rsb sl, r3, sl
0046c208: asrs sl, sl, #2
0046c20c: beq #0x46c260
0046c210: cmp r5, #0
0046c214: movne r7, #0
0046c218: bne #0x46c224
0046c21c: b #0x46c238
0046c220: ldr r3, [r4, #4]
0046c224: ldr r0, [r3, r7, lsl #2]
0046c228: add r7, r7, #1
0046c22c: bl #0x480060
0046c230: cmp r7, r5
0046c234: bne #0x46c220
0046c238: ldr r3, [sp, #0xc]
0046c23c: ldr r1, [sp, #4]
0046c240: add r3, r3, #0xc
0046c244: add r1, r1, #1
0046c248: cmp r3, #0x24
0046c24c: str r3, [sp, #0xc]
0046c250: str r1, [sp, #4]
0046c254: bne #0x46c1f0
0046c258: add sp, sp, #0x2c
0046c25c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046c260: ldr r2, [sp, #4]
0046c264: mov r3, #0xc
0046c268: mov r1, r5
0046c26c: mla r0, r3, r2, r6
0046c270: ldr r2, [sp, #0x18]
0046c274: add r0, r0, #4
0046c278: str sl, [sp, #0x24]
0046c27c: bl #0x46c164
0046c280: cmp r5, #0
0046c284: beq #0x46c238
0046c288: ldr r2, [sp, #8]
0046c28c: ldr r1, [sp, #0x1c]
0046c290: mov r8, sl
0046c294: ldr fp, [r2, r1]
0046c298: mov r1, #0
0046c29c: mov r0, #0x6c
0046c2a0: ldr sb, [fp]
0046c2a4: bl #0x310570
0046c2a8: ldr r1, [sp, #4]
0046c2ac: mov r7, r0
0046c2b0: bl #0x4808e4
0046c2b4: ldr r3, [r6, #0x5c]
0046c2b8: mov r0, r7
0046c2bc: add sb, sb, sl
0046c2c0: str r3, [r7, #0x60]
0046c2c4: bl #0x4807d8
0046c2c8: ldr r2, [sp, #8]
0046c2cc: ldr r1, [sp, #0x10]
0046c2d0: mov r0, r7
0046c2d4: add sl, sl, #0x11c
0046c2d8: ldr r3, [r2, r1]
0046c2dc: mov r1, sb
0046c2e0: ldr r3, [r3]
0046c2e4: ldr r3, [r3, r8, lsl #2]
0046c2e8: str r8, [r7, #8]
0046c2ec: str r3, [r7, #0x14]
0046c2f0: bl #0x48081c
0046c2f4: mov r0, r7
0046c2f8: bl #0x480060
0046c2fc: ldr r3, [r4, #4]
0046c300: str r7, [r3, r8, lsl #2]
0046c304: add r8, r8, #1
0046c308: cmp r8, r5
0046c30c: bne #0x46c298
0046c310: ldr r3, [sp, #0xc]
0046c314: ldr r1, [sp, #4]
0046c318: add r3, r3, #0xc
0046c31c: add r1, r1, #1
0046c320: cmp r3, #0x24
0046c324: str r3, [sp, #0xc]
0046c328: str r1, [sp, #4]
0046c32c: bne #0x46c1f0
0046c330: b #0x46c258
0046c334: ldrsbeq r8, [r2], #-0x8c
0046c338: andeq r4, r0, r4, lsr #8
0046c33c: andeq r4, r0, r8, asr #13
0046c340: andeq r2, r0, ip, asr #1

0x46954c _ZN14PlayerSavegame16_InitLevelStatesEv
0046954c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469550: ldr r7, [pc, #0x11c]
00469554: sub sp, sp, #0xc
00469558: ldr sl, [pc, #0x118]
0046955c: ldr sb, [pc, #0x118]
00469560: ldr r8, [pc, #0x118]
00469564: ldr ip, [pc, #0x118]
00469568: mov r4, r0
0046956c: mov r5, #0
00469570: add r7, pc, r7
00469574: ldr r6, [r4, #0x68]
00469578: cmp r6, #0
0046957c: beq #0x4695a4
00469580: ldr r6, [r4, #0x74]
00469584: cmp r6, #0
00469588: beq #0x469610
0046958c: add r5, r5, #1
00469590: cmp r5, #3
00469594: add r4, r4, #4
00469598: bne #0x469574
0046959c: add sp, sp, #0xc
004695a0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004695a4: ldr fp, [r7, sl]
004695a8: mov r1, r6
004695ac: ldr r0, [fp]
004695b0: str ip, [sp, #4]
004695b4: lsl r0, r0, #2
004695b8: bl #0x31056c
004695bc: str r0, [r4, #0x68]
004695c0: ldr r3, [fp]
004695c4: ldr ip, [sp, #4]
004695c8: cmp r3, #0
004695cc: beq #0x469580
004695d0: ldr r2, [r7, sb]
004695d4: mov r3, r6
004695d8: b #0x4695e0
004695dc: ldr r0, [r4, #0x68]
004695e0: ldr r1, [r2]
004695e4: add r1, r1, r6
004695e8: ldr r1, [r1, #0x28]
004695ec: add r6, r6, #0x48
004695f0: str r1, [r0, r3, lsl #2]
004695f4: ldr r1, [fp]
004695f8: add r3, r3, #1
004695fc: cmp r1, r3
00469600: bhi #0x4695dc
00469604: ldr r6, [r4, #0x74]
00469608: cmp r6, #0
0046960c: bne #0x46958c
00469610: ldr fp, [r7, r8]
00469614: mov r1, r6
00469618: ldr r0, [fp]
0046961c: str ip, [sp, #4]
00469620: lsl r0, r0, #2
00469624: bl #0x31056c
00469628: str r0, [r4, #0x74]
0046962c: ldr r3, [fp]
00469630: ldr ip, [sp, #4]
00469634: cmp r3, #0
00469638: beq #0x46958c
0046963c: ldr r2, [r7, ip]
00469640: mov r3, r6
00469644: b #0x46964c
00469648: ldr r0, [r4, #0x74]
0046964c: ldr r1, [r2]
00469650: add r1, r1, r6
00469654: ldr r1, [r1, #8]
00469658: add r6, r6, #0x14
0046965c: str r1, [r0, r3, lsl #2]
00469660: ldr r1, [fp]
00469664: add r3, r3, #1
00469668: cmp r1, r3
0046966c: bhi #0x469648
00469670: b #0x46958c
00469674: subseq fp, r2, r0, lsr #10
00469678: andeq r1, r0, r0, asr #17
0046967c: andeq r0, r0, r4, ror r8
00469680: andeq r2, r0, r4, ror r2
00469684: ldrdeq r3, r4, [r0], -r0

0x4694c8 _ZN14PlayerSavegame12_InitFaeriesEv
004694c8: push {r4, r5, r6, r7, r8, lr}
004694cc: mov r5, #0
004694d0: mov r4, r0
004694d4: mov r8, #5
004694d8: mov r7, r5
004694dc: ldr r6, [r4, #0x94]
004694e0: cmp r6, #0
004694e4: beq #0x4694fc
004694e8: add r5, r5, #1
004694ec: cmp r5, #3
004694f0: add r4, r4, #4
004694f4: bne #0x4694dc
004694f8: pop {r4, r5, r6, r7, r8, pc}
004694fc: str r8, [r4, #0xa0]
00469500: mov r0, #0x14
00469504: mov r1, r6
00469508: bl #0x31056c
0046950c: ldr r3, [r4, #0xa0]
00469510: str r0, [r4, #0x94]
00469514: cmp r3, #0
00469518: bne #0x469524
0046951c: b #0x4694e8
00469520: ldr r0, [r4, #0x94]
00469524: add r0, r0, r6, lsl #2
00469528: mov r3, #0
0046952c: strh r3, [r0, #2]
00469530: ldr r3, [r4, #0x94]
00469534: strb r7, [r3, r6, lsl #2]
00469538: ldr r3, [r4, #0xa0]
0046953c: add r6, r6, #1
00469540: cmp r3, r6
00469544: bhi #0x469520
00469548: b #0x4694e8

0x315ed8 _ZN8SavegameC1EPKcb
00315ed8: ldr r3, [pc, #0x60]
00315edc: ldr ip, [pc, #0x60]
00315ee0: push {r4, r5, lr}
00315ee4: add r3, pc, r3
00315ee8: ldr ip, [r3, ip]
00315eec: sub sp, sp, #0xc
00315ef0: mov r4, r0
00315ef4: add ip, ip, #8
00315ef8: mov r5, r2
00315efc: str ip, [r0], #4
00315f00: add r2, sp, #4
00315f04: bl #0x3140ec
00315f08: mov r1, #0
00315f0c: mov r3, r4
00315f10: str r1, [r4, #0x1c]
00315f14: str r1, [r4, #0x24]
00315f18: strb r1, [r3, #0x20]!
00315f1c: mov r0, r4
00315f20: str r3, [r4, #0x2c]
00315f24: strb r5, [r4, #0x38]
00315f28: str r3, [r4, #0x28]
00315f2c: str r1, [r4, #0x30]
00315f30: bl #0x315ad0
00315f34: mov r0, r4
00315f38: add sp, sp, #0xc
00315f3c: pop {r4, r5, pc}
00315f40: rsbeq lr, r7, ip, lsr #23
00315f44: strheq r0, [r0], -r8

0x46c48c _ZN13QuestSavegame12UnpackQuestsEiP11IStreamBaseb
0046c48c: push {r4, r5, r6, r7, r8, lr}
0046c490: mov r6, r1
0046c494: mov r1, #0xc
0046c498: mla r1, r1, r6, r0
0046c49c: mov r7, r2
0046c4a0: ldmib r1, {r2, r4}
0046c4a4: sub sp, sp, #0x10
0046c4a8: mov r5, r0
0046c4ac: add r1, sp, #0xc
0046c4b0: mov r0, r7
0046c4b4: rsb r4, r2, r4
0046c4b8: mov r8, r3
0046c4bc: bl #0x313b48
0046c4c0: ldr r3, [sp, #0xc]
0046c4c4: asr r4, r4, #2
0046c4c8: cmp r3, r4
0046c4cc: beq #0x46c4d8
0046c4d0: add sp, sp, #0x10
0046c4d4: pop {r4, r5, r6, r7, r8, pc}
0046c4d8: cmp r3, #0
0046c4dc: beq #0x46c50c
0046c4e0: mov r4, #0
0046c4e4: mov r1, r4
0046c4e8: mov r3, r7
0046c4ec: mov r0, r5
0046c4f0: mov r2, r6
0046c4f4: str r8, [sp]
0046c4f8: bl #0x46c344
0046c4fc: ldr r3, [sp, #0xc]
0046c500: add r4, r4, #1
0046c504: cmp r3, r4
0046c508: bhi #0x46c4e4
0046c50c: add r1, r5, r6, lsl #2
0046c510: add r1, r1, #0x2c
0046c514: mov r0, r7
0046c518: bl #0x38b758
0046c51c: add r4, r6, #0x10
0046c520: add r1, r6, #0xe
0046c524: add r1, r5, r1, lsl #2
0046c528: mov r0, r7
0046c52c: add r4, r5, r4, lsl #2
0046c530: bl #0x38b758
0046c534: mov r0, r7
0046c538: add r1, r4, #4
0046c53c: bl #0x38b758
0046c540: ldr r3, [r4, #4]
0046c544: add r6, r6, #0x14
0046c548: str r3, [r5, r6, lsl #2]
0046c54c: b #0x46c4d0

0x46c584 _ZN13QuestSavegame9PackQuestEiiP11IStreamBase
0046c584: push {r4, r5, r6, r7, r8, sl, lr}
0046c588: ldr r4, [pc, #0xc0]
0046c58c: ldr r6, [pc, #0xc0]
0046c590: sub sp, sp, #0x2c
0046c594: add r4, pc, r4
0046c598: ldr ip, [r4, r6]
0046c59c: add r5, sp, #0xc
0046c5a0: mov r7, r0
0046c5a4: ldr ip, [ip]
0046c5a8: mov r0, r5
0046c5ac: mov r8, r3
0046c5b0: mov sl, r2
0046c5b4: str ip, [sp, #0x24]
0046c5b8: str r1, [sp, #4]
0046c5bc: str r5, [sp, #0x1c]
0046c5c0: str r5, [sp, #0x20]
0046c5c4: bl #0x46be68
0046c5c8: mov r3, #0xc
0046c5cc: mla r7, r3, sl, r7
0046c5d0: ldr r3, [sp, #0x1c]
0046c5d4: mov r2, #0
0046c5d8: add r1, sp, #0x28
0046c5dc: strb r2, [r3]
0046c5e0: ldr r2, [r1, #-0x24]!
0046c5e4: ldr r3, [r7, #4]
0046c5e8: mov r0, r8
0046c5ec: ldr r7, [r3, r2, lsl #2]
0046c5f0: bl #0x38b808
0046c5f4: mov r0, r7
0046c5f8: mov r1, r8
0046c5fc: bl #0x47f734
0046c600: ldr r0, [sp, #0x20]
0046c604: cmp r0, r5
0046c608: beq #0x46c628
0046c60c: cmp r0, #0
0046c610: beq #0x46c628
0046c614: ldr r1, [sp, #0xc]
0046c618: rsb r1, r0, r1
0046c61c: cmp r1, #0x80
0046c620: bhi #0x46c644
0046c624: bl #0x708f00
0046c628: ldr r3, [r4, r6]
0046c62c: ldr r2, [sp, #0x24]
0046c630: ldr r3, [r3]
0046c634: cmp r2, r3
0046c638: bne #0x46c64c
0046c63c: add sp, sp, #0x2c
0046c640: pop {r4, r5, r6, r7, r8, sl, pc}
0046c644: bl #0x310440
0046c648: b #0x46c628
0046c64c: bl #0x30e310
0046c650: ldrsheq r8, [r2], #-0x4c
0046c654: andeq r4, r0, ip, lsr #1

0x3145e0 _ZN8Savegame3JobC1ERKS0_
003145e0: push {r4, r5, r6, lr}
003145e4: add r3, r0, #4
003145e8: mov r4, r0
003145ec: mov r5, r1
003145f0: mov r0, r3
003145f4: str r3, [r4, #0x14]
003145f8: str r3, [r4, #0x18]
003145fc: mov r1, #0x10
00314600: bl #0x31167c
00314604: ldr r2, [r4, #0x14]
00314608: mov r3, #0
0031460c: mov r0, r4
00314610: strb r3, [r2]
00314614: mov r1, r5
00314618: strb r3, [r4, #0x1d]
0031461c: bl #0x313cdc
00314620: mov r0, r4
00314624: pop {r4, r5, r6, pc}

0x3140b0 _ZNSt4listIN8Savegame3JobESaIS1_EE5eraseENSt4priv14_List_iteratorIS1_St16_Nonconst_traitsIS1_EEE.clone.4
003140b0: push {r4, r5, r6, lr}
003140b4: ldr r6, [r1]
003140b8: mov r5, r0
003140bc: ldr r4, [r6]
003140c0: ldr r3, [r6, #4]
003140c4: add r0, r6, #8
003140c8: str r4, [r3]
003140cc: str r3, [r4, #4]
003140d0: bl #0x313c90
003140d4: mov r0, r6
003140d8: mov r1, #0x28
003140dc: bl #0x708f00
003140e0: str r4, [r5]
003140e4: mov r0, r5
003140e8: pop {r4, r5, r6, pc}

0x46b5dc _ZNK13QuestSavegame19HasMainQuestUpdatedEi
0046b5dc: push {r4, r5, r6, lr}
0046b5e0: mov r3, #0xc
0046b5e4: mla r3, r3, r1, r0
0046b5e8: ldmib r3, {r2, r6}
0046b5ec: rsb r6, r2, r6
0046b5f0: asrs r6, r6, #2
0046b5f4: beq #0x46b63c
0046b5f8: add r5, r3, #4
0046b5fc: mov r4, #0
0046b600: ldr r0, [r2, r4, lsl #2]
0046b604: bl #0x47f8f8
0046b608: cmp r0, #0
0046b60c: beq #0x46b62c
0046b610: ldr r3, [r5]
0046b614: ldr r3, [r3, r4, lsl #2]
0046b618: ldrb r3, [r3, #0x5d]
0046b61c: cmp r3, #0
0046b620: beq #0x46b62c
0046b624: mov r0, #1
0046b628: pop {r4, r5, r6, pc}
0046b62c: add r4, r4, #1
0046b630: cmp r4, r6
0046b634: ldrne r2, [r5]
0046b638: bne #0x46b600
0046b63c: mov r0, #0
0046b640: pop {r4, r5, r6, pc}

0x46b144 _ZNK13QuestSavegame15IsActCompatibleEii
0046b144: add r3, r2, #0x14
0046b148: ldr r3, [r0, r3, lsl #2]
0046b14c: cmp r3, r1
0046b150: movgt r0, #0
0046b154: bxgt lr
0046b158: add r2, r0, r2, lsl #2
0046b15c: ldr r0, [r2, #0x44]
0046b160: cmp r1, r0
0046b164: movgt r0, #0
0046b168: movle r0, #1
0046b16c: bx lr

0x314120 _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_
00314120: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00314124: ldr fp, [pc, #0x15c]
00314128: ldr r2, [pc, #0x15c]
0031412c: sub sp, sp, #0x54
00314130: add fp, pc, fp
00314134: ldr r3, [fp, r2]
00314138: str r2, [sp, #0xc]
0031413c: str r0, [sp, #8]
00314140: ldr r4, [r0, #4]
00314144: ldr r3, [r3]
00314148: mov r8, r1
0031414c: cmp r4, #0
00314150: str r3, [sp, #0x4c]
00314154: beq #0x31425c
00314158: mov sl, r0
0031415c: add r7, sp, #0x34
00314160: add sb, sp, #0x18
00314164: ldr r1, [r8]
00314168: mov r2, sb
0031416c: mov r0, r7
00314170: bl #0x3140ec
00314174: ldr r3, [r4, #0x24]
00314178: ldr r1, [sp, #0x48]
0031417c: ldr r6, [r4, #0x20]
00314180: ldr r5, [sp, #0x44]
00314184: mov r0, r3
00314188: rsb r6, r3, r6
0031418c: rsb r5, r1, r5
00314190: cmp r5, r6
00314194: movlt r2, r5
00314198: movge r2, r6
0031419c: bl #0x30e5e0
003141a0: subs r3, r0, #0
003141a4: bne #0x3141bc
003141a8: cmp r6, r5
003141ac: mvnlt r3, #0
003141b0: blt #0x3141bc
003141b4: movle r3, #0
003141b8: movgt r3, #1
003141bc: mov r0, r7
003141c0: str r3, [sp, #4]
003141c4: bl #0x3139ac
003141c8: ldr r3, [sp, #4]
003141cc: cmp r3, #0
003141d0: movge sl, r4
003141d4: ldrlt r4, [r4, #0xc]
003141d8: ldrge r4, [r4, #8]
003141dc: cmp r4, #0
003141e0: bne #0x314164
003141e4: ldr r3, [sp, #8]
003141e8: cmp sl, r3
003141ec: beq #0x314260
003141f0: add r4, sp, #0x1c
003141f4: ldr r1, [r8]
003141f8: add r2, sp, #0x14
003141fc: mov r0, r4
00314200: bl #0x3140ec
00314204: ldr r3, [sp, #0x30]
00314208: ldr r1, [sl, #0x24]
0031420c: ldr r5, [sl, #0x20]
00314210: ldr r6, [sp, #0x2c]
00314214: mov r0, r3
00314218: rsb r5, r1, r5
0031421c: rsb r6, r3, r6
00314220: cmp r5, r6
00314224: movlt r2, r5
00314228: movge r2, r6
0031422c: bl #0x30e5e0
00314230: subs r7, r0, #0
00314234: bne #0x31424c
00314238: cmp r6, r5
0031423c: mvnlt r7, #0
00314240: blt #0x31424c
00314244: movle r7, #0
00314248: movgt r7, #1
0031424c: mov r0, r4
00314250: bl #0x3139ac
00314254: cmp r7, #0
00314258: bge #0x314260
0031425c: ldr sl, [sp, #8]
00314260: ldr r2, [sp, #0xc]
00314264: mov r0, sl
00314268: ldr r3, [fp, r2]
0031426c: ldr r2, [sp, #0x4c]
00314270: ldr r3, [r3]
00314274: cmp r2, r3
00314278: bne #0x314284
0031427c: add sp, sp, #0x54
00314280: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00314284: bl #0x30e310
00314288: rsbeq r0, r8, r0, ror #18
0031428c: andeq r4, r0, ip, lsr #1

0x3156f0 _ZNSt3mapISsN8Savegame11SectionInfoESt4lessISsESaISt4pairIKSsS1_EEEixIPKcEERS1_RKT_
003156f0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003156f4: ldr r4, [pc, #0x144]
003156f8: ldr r7, [pc, #0x144]
003156fc: sub sp, sp, #0x84
00315700: add r4, pc, r4
00315704: ldr r3, [r4, r7]
00315708: mov r8, r0
0031570c: mov r6, r1
00315710: ldr r3, [r3]
00315714: str r3, [sp, #0x7c]
00315718: bl #0x314290
0031571c: cmp r0, r8
00315720: mov r5, r0
00315724: beq #0x3157b8
00315728: add sl, sp, #0x64
0031572c: ldr r1, [r6]
00315730: add r2, sp, #0x14
00315734: mov r0, sl
00315738: bl #0x3140ec
0031573c: ldr r3, [sp, #0x78]
00315740: ldr r1, [r5, #0x24]
00315744: ldr fp, [r5, #0x20]
00315748: ldr sb, [sp, #0x74]
0031574c: mov r0, r3
00315750: rsb fp, r1, fp
00315754: rsb sb, r3, sb
00315758: cmp fp, sb
0031575c: movlt r2, fp
00315760: movge r2, sb
00315764: bl #0x30e5e0
00315768: cmp r0, #0
0031576c: mov r3, r5
00315770: bne #0x3157ac
00315774: cmp sb, fp
00315778: blt #0x3157b0
0031577c: mov r0, sl
00315780: str r3, [sp, #4]
00315784: bl #0x3139ac
00315788: ldr r3, [sp, #4]
0031578c: ldr r1, [r4, r7]
00315790: ldr r2, [sp, #0x7c]
00315794: add r0, r3, #0x28
00315798: ldr r3, [r1]
0031579c: cmp r2, r3
003157a0: bne #0x31583c
003157a4: add sp, sp, #0x84
003157a8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003157ac: bge #0x31577c
003157b0: mov r0, sl
003157b4: bl #0x3139ac
003157b8: add sl, sp, #0x4c
003157bc: ldr r1, [r6]
003157c0: add r2, sp, #0x10
003157c4: add r6, sp, #0x18
003157c8: mov r0, sl
003157cc: bl #0x3140ec
003157d0: mov r0, r6
003157d4: ldr r1, [sp, #0x60]
003157d8: ldr r2, [sp, #0x5c]
003157dc: str r6, [sp, #0x28]
003157e0: str r6, [sp, #0x2c]
003157e4: mov sb, #0
003157e8: bl #0x3116e8
003157ec: mov ip, #0
003157f0: mov r1, r8
003157f4: mov r3, r6
003157f8: add r2, sp, #8
003157fc: add r0, sp, #0xc
00315800: mov r8, #0
00315804: str ip, [sp, #0x38]
00315808: str ip, [sp, #0x44]
0031580c: str ip, [sp, #0x40]
00315810: str ip, [sp, #0x3c]
00315814: str r5, [sp, #8]
00315818: strd r8, sb, [sp, #0x30]
0031581c: bl #0x3151ec
00315820: ldr r5, [sp, #0xc]
00315824: mov r0, r6
00315828: bl #0x3139ac
0031582c: mov r0, sl
00315830: bl #0x3139ac
00315834: mov r3, r5
00315838: b #0x31578c
0031583c: bl #0x30e310
00315840: mlseq r7, r0, r3, pc
00315844: andeq r4, r0, ip, lsr #1

0x3144f0 _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE14_M_lower_boundIA5_cEEPNS_18_Rb_tree_node_baseERKT_
003144f0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003144f4: ldr fp, [pc, #0xdc]
003144f8: ldr r2, [pc, #0xdc]
003144fc: sub sp, sp, #0x2c
00314500: add fp, pc, fp
00314504: ldr r3, [fp, r2]
00314508: str r2, [sp, #4]
0031450c: mov sb, r0
00314510: ldr r3, [r3]
00314514: mov r8, r1
00314518: str r3, [sp, #0x24]
0031451c: ldr r4, [r0, #4]
00314520: cmp r4, #0
00314524: beq #0x3145b0
00314528: add r7, sp, #0xc
0031452c: add sl, sp, #8
00314530: mov r1, r8
00314534: mov r2, sl
00314538: mov r0, r7
0031453c: bl #0x3140ec
00314540: ldr r3, [r4, #0x24]
00314544: ldr r1, [sp, #0x20]
00314548: ldr r6, [r4, #0x20]
0031454c: ldr r5, [sp, #0x1c]
00314550: mov r0, r3
00314554: rsb r6, r3, r6
00314558: rsb r5, r1, r5
0031455c: cmp r5, r6
00314560: movlt r2, r5
00314564: movge r2, r6
00314568: bl #0x30e5e0
0031456c: subs r3, r0, #0
00314570: bne #0x314588
00314574: cmp r6, r5
00314578: mvnlt r3, #0
0031457c: blt #0x314588
00314580: movle r3, #0
00314584: movgt r3, #1
00314588: mov r0, r7
0031458c: str r3, [sp]
00314590: bl #0x3139ac
00314594: ldr r3, [sp]
00314598: cmp r3, #0
0031459c: movge sb, r4
003145a0: ldrlt r4, [r4, #0xc]
003145a4: ldrge r4, [r4, #8]
003145a8: cmp r4, #0
003145ac: bne #0x314530
003145b0: ldr r2, [sp, #4]
003145b4: mov r0, sb
003145b8: ldr r3, [fp, r2]
003145bc: ldr r2, [sp, #0x24]
003145c0: ldr r3, [r3]
003145c4: cmp r2, r3
003145c8: bne #0x3145d4
003145cc: add sp, sp, #0x2c
003145d0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003145d4: bl #0x30e310
003145d8: mlseq r8, r0, r5, r0
003145dc: andeq r4, r0, ip, lsr #1

0x3151ec _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueENS_17_Rb_tree_iteratorIS7_SB_EERKS7_
003151ec: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003151f0: sub sp, sp, #0x44
003151f4: str r2, [sp, #0x14]
003151f8: ldr r5, [r2]
003151fc: ldr r2, [r1, #8]
00315200: mov r6, r1
00315204: mov r7, r0
00315208: cmp r5, r2
0031520c: mov r8, r3
00315210: beq #0x315408
00315214: cmp r5, r1
00315218: beq #0x315560
0031521c: ldrb r3, [r5]
00315220: cmp r3, #0
00315224: beq #0x315300
00315228: ldr r4, [r5, #8]
0031522c: cmp r4, #0
00315230: bne #0x31523c
00315234: b #0x315320
00315238: mov r4, r3
0031523c: ldr r3, [r4, #0xc]
00315240: cmp r3, #0
00315244: bne #0x315238
00315248: ldr r3, [r5, #0x24]
0031524c: ldr sb, [r8, #0x14]
00315250: ldr fp, [r8, #0x10]
00315254: ldr r2, [r5, #0x20]
00315258: mov r1, r3
0031525c: rsb fp, sb, fp
00315260: rsb r2, r3, r2
00315264: str r2, [sp, #0x18]
00315268: mov r0, sb
0031526c: cmp r2, fp
00315270: movge r2, fp
00315274: str r3, [sp, #0xc]
00315278: str r2, [sp, #0x1c]
0031527c: bl #0x30e5e0
00315280: cmp r0, #0
00315284: ldr r3, [sp, #0xc]
00315288: bne #0x3152a4
0031528c: ldr r2, [sp, #0x18]
00315290: cmp fp, r2
00315294: mvnlt r0, #0
00315298: blt #0x3152a4
0031529c: movle r0, #0
003152a0: movgt r0, #1
003152a4: lsrs ip, r0, #0x1f
003152a8: bne #0x315350
003152ac: ldr r4, [r5, #0xc]
003152b0: cmp r4, #0
003152b4: bne #0x3152c0
003152b8: b #0x3155f0
003152bc: mov r4, r2
003152c0: ldr r2, [r4, #8]
003152c4: cmp r2, #0
003152c8: bne #0x3152bc
003152cc: cmp ip, #0
003152d0: bne #0x3153e4
003152d4: mov r0, r3
003152d8: mov r1, sb
003152dc: ldr r2, [sp, #0x1c]
003152e0: bl #0x30e5e0
003152e4: cmp r0, #0
003152e8: bne #0x3153c0
003152ec: ldr r3, [sp, #0x18]
003152f0: cmp fp, r3
003152f4: bgt #0x3153c4
003152f8: str r5, [r7]
003152fc: b #0x3153fc
00315300: ldr r3, [r5, #4]
00315304: ldr r3, [r3, #4]
00315308: cmp r5, r3
0031530c: ldreq r4, [r5, #0xc]
00315310: beq #0x315248
00315314: ldr r4, [r5, #8]
00315318: cmp r4, #0
0031531c: bne #0x31523c
00315320: ldr r4, [r5, #4]
00315324: ldr r3, [r4, #8]
00315328: cmp r5, r3
0031532c: beq #0x315338
00315330: b #0x315248
00315334: mov r4, r3
00315338: ldr r3, [r4, #4]
0031533c: ldr r2, [r3, #8]
00315340: cmp r2, r4
00315344: beq #0x315334
00315348: mov r4, r3
0031534c: b #0x315248
00315350: ldr r2, [r4, #0x24]
00315354: ldr sl, [r4, #0x20]
00315358: mov r1, sb
0031535c: mov r0, r2
00315360: rsb sl, r2, sl
00315364: cmp fp, sl
00315368: movlt r2, fp
0031536c: movge r2, sl
00315370: str r3, [sp, #0xc]
00315374: str ip, [sp, #0x10]
00315378: bl #0x30e5e0
0031537c: cmp r0, #0
00315380: ldr r3, [sp, #0xc]
00315384: ldr ip, [sp, #0x10]
00315388: bne #0x31554c
0031538c: cmp fp, sl
00315390: ble #0x3152ac
00315394: ldr ip, [r4, #0xc]
00315398: cmp ip, #0
0031539c: beq #0x31552c
003153a0: mov ip, #0
003153a4: mov r1, r6
003153a8: mov r2, r5
003153ac: mov r3, r8
003153b0: mov r0, r7
003153b4: stm sp, {r5, ip}
003153b8: bl #0x314e08
003153bc: b #0x3153fc
003153c0: bge #0x3152f8
003153c4: cmp r6, r4
003153c8: beq #0x31563c
003153cc: add r0, r6, #0x14
003153d0: mov r1, r8
003153d4: add r2, r4, #0x10
003153d8: bl #0x313bf8
003153dc: cmp r0, #0
003153e0: bne #0x315634
003153e4: mov r1, r6
003153e8: mov r2, r8
003153ec: add r0, sp, #0x20
003153f0: bl #0x314ef8
003153f4: ldr r3, [sp, #0x20]
003153f8: str r3, [r7]
003153fc: mov r0, r7
00315400: add sp, sp, #0x44
00315404: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00315408: ldr r3, [r1, #0x10]
0031540c: cmp r3, #0
00315410: beq #0x315694
00315414: ldr r3, [r8, #0x14]
00315418: ldr r1, [r5, #0x24]
0031541c: ldr r4, [r8, #0x10]
00315420: ldr sl, [r5, #0x20]
00315424: mov r0, r3
00315428: rsb r4, r3, r4
0031542c: rsb sl, r1, sl
00315430: cmp sl, r4
00315434: movlt r2, sl
00315438: movge r2, r4
0031543c: bl #0x30e5e0
00315440: cmp r0, #0
00315444: bne #0x315458
00315448: cmp r4, sl
0031544c: blt #0x3153a0
00315450: movle r0, #0
00315454: movgt r0, #1
00315458: cmp r0, #0
0031545c: blt #0x3153a0
00315460: add sl, r6, #0x14
00315464: add r1, r5, #0x10
00315468: mov r0, sl
0031546c: mov r2, r8
00315470: bl #0x313bf8
00315474: cmp r0, #0
00315478: beq #0x315684
0031547c: ldr r3, [sp, #0x14]
00315480: ldr ip, [r3]
00315484: ldr r4, [ip, #0xc]
00315488: cmp r4, #0
0031548c: bne #0x31551c
00315490: ldr r3, [ip, #4]
00315494: ldr r2, [r3, #0xc]
00315498: cmp ip, r2
0031549c: movne r4, ip
003154a0: bne #0x3154b8
003154a4: mov r4, r3
003154a8: ldr r3, [r3, #4]
003154ac: ldr r2, [r3, #0xc]
003154b0: cmp r2, r4
003154b4: beq #0x3154a4
003154b8: ldr r2, [r4, #0xc]
003154bc: cmp r3, r2
003154c0: movne r4, r3
003154c4: cmp r6, r4
003154c8: beq #0x3156cc
003154cc: mov r0, sl
003154d0: mov r1, r8
003154d4: add r2, r4, #0x10
003154d8: bl #0x313bf8
003154dc: cmp r0, #0
003154e0: beq #0x315668
003154e4: ldr r2, [sp, #0x14]
003154e8: ldr ip, [r2]
003154ec: ldr lr, [ip, #0xc]
003154f0: cmp lr, #0
003154f4: beq #0x3156ac
003154f8: mov ip, #0
003154fc: mov r1, r6
00315500: mov r2, r4
00315504: mov r3, r8
00315508: mov r0, r7
0031550c: stm sp, {r4, ip}
00315510: bl #0x314e08
00315514: b #0x3153fc
00315518: mov r4, r3
0031551c: ldr r3, [r4, #8]
00315520: cmp r3, #0
00315524: bne #0x315518
00315528: b #0x3154c4
0031552c: mov r1, r6
00315530: mov r2, r4
00315534: mov r3, r8
00315538: mov r0, r7
0031553c: str ip, [sp]
00315540: str r4, [sp, #4]
00315544: bl #0x314e08
00315548: b #0x3153fc
0031554c: bge #0x3152ac
00315550: ldr ip, [r4, #0xc]
00315554: cmp ip, #0
00315558: bne #0x3153a0
0031555c: b #0x31552c
00315560: ldr r4, [r5, #0xc]
00315564: ldr r1, [r3, #0x14]
00315568: ldr sb, [r3, #0x10]
0031556c: ldr sl, [r4, #0x20]
00315570: ldr r3, [r4, #0x24]
00315574: rsb sb, r1, sb
00315578: rsb sl, r3, sl
0031557c: cmp sb, sl
00315580: movlt r2, sb
00315584: movge r2, sl
00315588: mov r0, r3
0031558c: bl #0x30e5e0
00315590: cmp r0, #0
00315594: bne #0x3155a8
00315598: cmp sl, sb
0031559c: blt #0x3155b0
003155a0: movle r0, #0
003155a4: movgt r0, #1
003155a8: cmp r0, #0
003155ac: bge #0x3155d4
003155b0: mov ip, #0
003155b4: mov r1, r6
003155b8: mov r2, r4
003155bc: mov r3, r8
003155c0: mov r0, r7
003155c4: str ip, [sp]
003155c8: str r5, [sp, #4]
003155cc: bl #0x314e08
003155d0: b #0x3153fc
003155d4: mov r1, r6
003155d8: mov r2, r8
003155dc: add r0, sp, #0x28
003155e0: bl #0x314ef8
003155e4: ldr r3, [sp, #0x28]
003155e8: str r3, [r7]
003155ec: b #0x3153fc
003155f0: ldr r2, [r5, #4]
003155f4: ldr r1, [r2, #0xc]
003155f8: cmp r5, r1
003155fc: movne r4, r5
00315600: beq #0x315618
00315604: ldr r1, [r4, #0xc]
00315608: cmp r2, r1
0031560c: movne r4, r2
00315610: b #0x3152cc
00315614: mov r2, r1
00315618: ldr r1, [r2, #4]
0031561c: ldr r0, [r1, #0xc]
00315620: cmp r0, r2
00315624: beq #0x315614
00315628: mov r4, r2
0031562c: mov r2, r1
00315630: b #0x315604
00315634: ldr r2, [sp, #0x14]
00315638: ldr r5, [r2]
0031563c: ldr ip, [r5, #0xc]
00315640: cmp ip, #0
00315644: bne #0x3154f8
00315648: mov r1, r6
0031564c: mov r2, r5
00315650: mov r3, r8
00315654: mov r0, r7
00315658: str ip, [sp]
0031565c: str r5, [sp, #4]
00315660: bl #0x314e08
00315664: b #0x3153fc
00315668: mov r1, r6
0031566c: mov r2, r8
00315670: add r0, sp, #0x30
00315674: bl #0x314ef8
00315678: ldr r3, [sp, #0x30]
0031567c: str r3, [r7]
00315680: b #0x3153fc
00315684: ldr r2, [sp, #0x14]
00315688: ldr r3, [r2]
0031568c: str r3, [r7]
00315690: b #0x3153fc
00315694: mov r2, r8
00315698: add r0, sp, #0x38
0031569c: bl #0x314ef8
003156a0: ldr r3, [sp, #0x38]
003156a4: str r3, [r7]
003156a8: b #0x3153fc
003156ac: mov r1, r6
003156b0: mov r2, ip
003156b4: mov r3, r8
003156b8: mov r0, r7
003156bc: str lr, [sp]
003156c0: str ip, [sp, #4]
003156c4: bl #0x314e08
003156c8: b #0x3153fc
003156cc: mov lr, #0
003156d0: mov r1, r6
003156d4: mov r2, ip
003156d8: mov r3, r8
003156dc: mov r0, r7
003156e0: str lr, [sp]
003156e4: str ip, [sp, #4]
003156e8: bl #0x314e08
003156ec: b #0x3153fc

0x46c344 _ZN13QuestSavegame11UnpackQuestEiiP11IStreamBaseb
0046c344: push {r4, r5, r6, r7, r8, sb, sl, lr}
0046c348: ldr r4, [pc, #0x120]
0046c34c: ldr r6, [pc, #0x120]
0046c350: sub sp, sp, #0x30
0046c354: add r4, pc, r4
0046c358: ldr ip, [r4, r6]
0046c35c: add r5, sp, #0x14
0046c360: mov r7, r0
0046c364: ldr ip, [ip]
0046c368: mov r0, r5
0046c36c: mov r8, r2
0046c370: mov sl, r3
0046c374: str r1, [sp, #0xc]
0046c378: str ip, [sp, #0x2c]
0046c37c: str r5, [sp, #0x24]
0046c380: str r5, [sp, #0x28]
0046c384: ldrb sb, [sp, #0x50]
0046c388: bl #0x46be68
0046c38c: ldr r3, [sp, #0x24]
0046c390: mov r2, #0
0046c394: mov r0, sl
0046c398: strb r2, [r3]
0046c39c: add r1, sp, #0xc
0046c3a0: bl #0x38b758
0046c3a4: mov r3, #0xc
0046c3a8: mla r7, r3, r8, r7
0046c3ac: ldr r2, [sp, #0xc]
0046c3b0: ldr r3, [r7, #4]
0046c3b4: ldr r0, [r3, r2, lsl #2]
0046c3b8: cmp r0, #0
0046c3bc: beq #0x46c418
0046c3c0: mov r1, sl
0046c3c4: mov r2, sb
0046c3c8: bl #0x47f78c
0046c3cc: ldr r0, [sp, #0x28]
0046c3d0: cmp r0, r5
0046c3d4: beq #0x46c3f4
0046c3d8: cmp r0, #0
0046c3dc: beq #0x46c3f4
0046c3e0: ldr r1, [sp, #0x14]
0046c3e4: rsb r1, r0, r1
0046c3e8: cmp r1, #0x80
0046c3ec: bhi #0x46c410
0046c3f0: bl #0x708f00
0046c3f4: ldr r3, [r4, r6]
0046c3f8: ldr r2, [sp, #0x2c]
0046c3fc: ldr r3, [r3]
0046c400: cmp r2, r3
0046c404: bne #0x46c46c
0046c408: add sp, sp, #0x30
0046c40c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0046c410: bl #0x310440
0046c414: b #0x46c3f4
0046c418: ldr r3, [pc, #0x58]
0046c41c: ldr r3, [r4, r3]
0046c420: ldr r3, [r3]
0046c424: cmp r3, #2
0046c428: streq r0, [r0]
0046c42c: beq #0x46c3cc
0046c430: cmp r3, #1
0046c434: bne #0x46c3cc
0046c438: ldr r0, [pc, #0x3c]
0046c43c: ldr r1, [pc, #0x3c]
0046c440: ldr r2, [pc, #0x3c]
0046c444: ldr r0, [r4, r0]
0046c448: ldr r3, [pc, #0x38]
0046c44c: mov ip, #0xc2
0046c450: add r1, pc, r1
0046c454: add r2, pc, r2
0046c458: add r3, pc, r3
0046c45c: add r0, r0, #0xa8
0046c460: str ip, [sp]
0046c464: bl #0x30e004
0046c468: b #0x46c3cc
0046c46c: bl #0x30e310
0046c470: subseq r8, r2, ip, lsr r7
0046c474: andeq r4, r0, ip, lsr #1
0046c478: andeq r3, r0, r0, asr #19
0046c47c: andeq r1, r0, r0, asr #19
0046c480: subeq r1, r5, r8, lsl #31
0046c484: subeq pc, r5, r4, lsl #20
0046c488: subeq r1, r6, r0, lsl #2

0x313cdc _ZN8Savegame3Job4copyERKS0_
00313cdc: push {r4, r5, r6, lr}
00313ce0: ldrb r3, [r0, #0x1d]
00313ce4: mov r4, r0
00313ce8: mov r5, r1
00313cec: cmp r3, #0
00313cf0: beq #0x313d10
00313cf4: ldr r3, [r0]
00313cf8: cmp r3, #0
00313cfc: beq #0x313d10
00313d00: mov r0, r3
00313d04: ldr r3, [r3]
00313d08: mov lr, pc
00313d0c: ldr pc, [r3, #4]
00313d10: mov r3, #0
00313d14: strb r3, [r4, #0x1d]
00313d18: str r3, [r4]
00313d1c: mov r3, r5
00313d20: ldr r2, [r3], #4
00313d24: mov r0, r4
00313d28: str r2, [r0], #4
00313d2c: cmp r0, r3
00313d30: beq #0x313d40
00313d34: ldr r1, [r5, #0x18]
00313d38: ldr r2, [r5, #0x14]
00313d3c: bl #0x3109e0
00313d40: ldrb r3, [r5, #0x1d]
00313d44: strb r3, [r4, #0x1d]
00313d48: ldrb r3, [r5, #0x1c]
00313d4c: strb r3, [r4, #0x1c]
00313d50: mov r3, #0
00313d54: strb r3, [r5, #0x1d]
00313d58: pop {r4, r5, r6, pc}

0x314290 _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE14_M_lower_boundIPKcEEPNS_18_Rb_tree_node_baseERKT_
00314290: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00314294: ldr fp, [pc, #0xdc]
00314298: ldr r2, [pc, #0xdc]
0031429c: sub sp, sp, #0x2c
003142a0: add fp, pc, fp
003142a4: ldr r3, [fp, r2]
003142a8: str r2, [sp, #4]
003142ac: mov sb, r0
003142b0: ldr r3, [r3]
003142b4: mov r8, r1
003142b8: str r3, [sp, #0x24]
003142bc: ldr r4, [r0, #4]
003142c0: cmp r4, #0
003142c4: beq #0x314350
003142c8: add r7, sp, #0xc
003142cc: add sl, sp, #8
003142d0: ldr r1, [r8]
003142d4: mov r2, sl
003142d8: mov r0, r7
003142dc: bl #0x3140ec
003142e0: ldr r3, [r4, #0x24]
003142e4: ldr r1, [sp, #0x20]
003142e8: ldr r6, [r4, #0x20]
003142ec: ldr r5, [sp, #0x1c]
003142f0: mov r0, r3
003142f4: rsb r6, r3, r6
003142f8: rsb r5, r1, r5
003142fc: cmp r5, r6
00314300: movlt r2, r5
00314304: movge r2, r6
00314308: bl #0x30e5e0
0031430c: subs r3, r0, #0
00314310: bne #0x314328
00314314: cmp r6, r5
00314318: mvnlt r3, #0
0031431c: blt #0x314328
00314320: movle r3, #0
00314324: movgt r3, #1
00314328: mov r0, r7
0031432c: str r3, [sp]
00314330: bl #0x3139ac
00314334: ldr r3, [sp]
00314338: cmp r3, #0
0031433c: movge sb, r4
00314340: ldrlt r4, [r4, #0xc]
00314344: ldrge r4, [r4, #8]
00314348: cmp r4, #0
0031434c: bne #0x3142d0
00314350: ldr r2, [sp, #4]
00314354: mov r0, sb
00314358: ldr r3, [fp, r2]
0031435c: ldr r2, [sp, #0x24]
00314360: ldr r3, [r3]
00314364: cmp r2, r3
00314368: bne #0x314374
0031436c: add sp, sp, #0x2c
00314370: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00314374: bl #0x30e310

0x314e08 _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE9_M_insertEPNS_18_Rb_tree_node_baseERKS7_SF_SF_
00314e08: push {r4, r5, r6, r7, lr}
00314e0c: cmp r1, r2
00314e10: sub sp, sp, #0xc
00314e14: mov r4, r1
00314e18: mov r5, r2
00314e1c: mov r6, r0
00314e20: beq #0x314eb4
00314e24: ldr r2, [sp, #0x24]
00314e28: cmp r2, #0
00314e2c: beq #0x314e7c
00314e30: mov r1, r3
00314e34: mov r0, r4
00314e38: bl #0x314da0
00314e3c: str r0, [r5, #0xc]
00314e40: ldr r3, [r4, #0xc]
00314e44: mov r7, r0
00314e48: cmp r5, r3
00314e4c: beq #0x314eac
00314e50: mov r0, r7
00314e54: str r5, [r7, #4]
00314e58: add r1, r4, #4
00314e5c: bl #0x313760
00314e60: ldr r3, [r4, #0x10]
00314e64: mov r0, r6
00314e68: add r3, r3, #1
00314e6c: str r3, [r4, #0x10]
00314e70: str r7, [r6]
00314e74: add sp, sp, #0xc
00314e78: pop {r4, r5, r6, r7, pc}
00314e7c: ldr r2, [sp, #0x20]
00314e80: cmp r2, #0
00314e84: beq #0x314ed4
00314e88: mov r1, r3
00314e8c: mov r0, r4
00314e90: bl #0x314da0
00314e94: str r0, [r5, #8]
00314e98: ldr r3, [r4, #8]
00314e9c: mov r7, r0
00314ea0: cmp r5, r3
00314ea4: streq r0, [r4, #8]
00314ea8: b #0x314e50
00314eac: str r7, [r4, #0xc]
00314eb0: b #0x314e50
00314eb4: mov r1, r3
00314eb8: mov r0, r4
00314ebc: bl #0x314da0
00314ec0: mov r7, r0
00314ec4: str r0, [r4, #8]
00314ec8: str r0, [r4, #4]
00314ecc: str r0, [r4, #0xc]
00314ed0: b #0x314e50
00314ed4: add r0, r1, #0x14
00314ed8: add r2, r5, #0x10
00314edc: mov r1, r3
00314ee0: str r3, [sp, #4]
00314ee4: bl #0x313bf8
00314ee8: cmp r0, #0
00314eec: ldr r3, [sp, #4]
00314ef0: beq #0x314e30
00314ef4: b #0x314e88

0x314ef8 _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE13insert_uniqueERKS7_
00314ef8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00314efc: ldr r5, [r1, #4]
00314f00: sub sp, sp, #0x14
00314f04: mov sb, r1
00314f08: cmp r5, #0
00314f0c: mov r4, r0
00314f10: mov r8, r2
00314f14: beq #0x314ffc
00314f18: ldr r7, [r2, #0x14]
00314f1c: ldr fp, [r2, #0x10]
00314f20: rsb sl, r7, fp
00314f24: b #0x314f3c
00314f28: ldr r3, [r5, #8]
00314f2c: mov r1, #1
00314f30: cmp r3, #0
00314f34: beq #0x314f94
00314f38: mov r5, r3
00314f3c: ldr r3, [r5, #0x24]
00314f40: ldr r6, [r5, #0x20]
00314f44: mov r0, r7
00314f48: mov r1, r3
00314f4c: rsb r6, r3, r6
00314f50: cmp r6, sl
00314f54: movlt r2, r6
00314f58: movge r2, sl
00314f5c: bl #0x30e5e0
00314f60: cmp r0, #0
00314f64: mov r2, r5
00314f68: bne #0x314f7c
00314f6c: cmp sl, r6
00314f70: blt #0x314f28
00314f74: movle r0, #0
00314f78: movgt r0, #1
00314f7c: cmp r0, #0
00314f80: blt #0x314f28
00314f84: ldr r3, [r5, #0xc]
00314f88: mov r1, #0
00314f8c: cmp r3, #0
00314f90: bne #0x314f38
00314f94: cmp r1, #0
00314f98: moveq sl, r5
00314f9c: bne #0x315000
00314fa0: ldr r0, [r2, #0x24]
00314fa4: ldr r6, [r2, #0x20]
00314fa8: rsb fp, r7, fp
00314fac: mov r1, r7
00314fb0: rsb r6, r0, r6
00314fb4: cmp fp, r6
00314fb8: movlt r2, fp
00314fbc: movge r2, r6
00314fc0: bl #0x30e5e0
00314fc4: cmp r0, #0
00314fc8: bne #0x314fdc
00314fcc: cmp r6, fp
00314fd0: blt #0x315058
00314fd4: movle r0, #0
00314fd8: movgt r0, #1
00314fdc: cmp r0, #0
00314fe0: movge r3, #0
00314fe4: strge sl, [r4]
00314fe8: strbge r3, [r4, #4]
00314fec: blt #0x315058
00314ff0: mov r0, r4
00314ff4: add sp, sp, #0x14
00314ff8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00314ffc: mov r5, r1
00315000: ldr r3, [sb, #8]
00315004: cmp r5, r3
00315008: beq #0x3150d8
0031500c: ldrb r3, [r5]
00315010: cmp r3, #0
00315014: bne #0x315028
00315018: ldr r3, [r5, #4]
0031501c: ldr r3, [r3, #4]
00315020: cmp r5, r3
00315024: beq #0x3150c4
00315028: ldr r2, [r5, #8]
0031502c: cmp r2, #0
00315030: bne #0x31503c
00315034: b #0x31508c
00315038: mov r2, r3
0031503c: ldr r3, [r2, #0xc]
00315040: cmp r3, #0
00315044: bne #0x315038
00315048: mov sl, r2
0031504c: ldr r7, [r8, #0x14]
00315050: ldr fp, [r8, #0x10]
00315054: b #0x314fa0
00315058: mov ip, #0
0031505c: mov r2, r5
00315060: mov r3, r8
00315064: mov r1, sb
00315068: add r0, sp, #8
0031506c: str ip, [sp, #4]
00315070: str ip, [sp]
00315074: bl #0x314e08
00315078: ldr r3, [sp, #8]
0031507c: mov r2, #1
00315080: strb r2, [r4, #4]
00315084: str r3, [r4]
00315088: b #0x314ff0
0031508c: ldr r3, [r5, #4]
00315090: ldr r2, [r3, #8]
00315094: cmp r5, r2
00315098: beq #0x3150a4
0031509c: b #0x315108
003150a0: mov r3, r2
003150a4: ldr r2, [r3, #4]
003150a8: ldr r1, [r2, #8]
003150ac: cmp r1, r3
003150b0: beq #0x3150a0
003150b4: ldr r7, [r8, #0x14]
003150b8: ldr fp, [r8, #0x10]
003150bc: mov sl, r2
003150c0: b #0x314fa0
003150c4: ldr r2, [r5, #0xc]
003150c8: ldr r7, [r8, #0x14]
003150cc: ldr fp, [r8, #0x10]
003150d0: mov sl, r2
003150d4: b #0x314fa0
003150d8: mov r2, r5
003150dc: mov r3, r8
003150e0: mov ip, #0
003150e4: mov r1, sb
003150e8: add r0, sp, #0xc
003150ec: stm sp, {r5, ip}
003150f0: bl #0x314e08
003150f4: ldr r3, [sp, #0xc]
003150f8: mov r2, #1
003150fc: strb r2, [r4, #4]
00315100: str r3, [r4]
00315104: b #0x314ff0
00315108: mov r2, r3
0031510c: b #0x315048

0x314da0 _ZNSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsN8Savegame11SectionInfoEENS_10_Select1stIS7_EENS_11_MapTraitsTIS7_EESaIS7_EE14_M_create_nodeERKS7_
00314da0: push {r4, r5, lr}
00314da4: sub sp, sp, #0xc
00314da8: mov r3, #0x40
00314dac: add r0, sp, #8
00314db0: str r3, [r0, #-4]!
00314db4: mov r4, r1
00314db8: bl #0x708ec0
00314dbc: mov r5, r0
00314dc0: add r0, r0, #0x10
00314dc4: str r0, [r5, #0x20]
00314dc8: str r0, [r5, #0x24]
00314dcc: ldr r1, [r4, #0x14]
00314dd0: ldr r2, [r4, #0x10]
00314dd4: bl #0x3116e8
00314dd8: add r4, r4, #0x18
00314ddc: add ip, r5, #0x28
00314de0: ldm r4!, {r0, r1, r2, r3}
00314de4: stm ip!, {r0, r1, r2, r3}
00314de8: ldm r4, {r0, r1}
00314dec: mov r3, #0
00314df0: stm ip, {r0, r1}
00314df4: mov r0, r5
00314df8: str r3, [r5, #0xc]
00314dfc: str r3, [r5, #8]
00314e00: add sp, sp, #0xc
00314e04: pop {r4, r5, pc}
