
SOURCE 0x468f18 _ZN14PlayerSavegame13__SaveFaeriesEP11IStreamBasePv
00468f18 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00468f1c ldr r3, [pc, #0x134]
00468f20 sub sp, sp, #0x2c
00468f24 ldr sl, [pc, #0x130]
00468f28 str r3, [sp, #0xc]
00468f2c ldr r3, [pc, #0x12c]
00468f30 mov r7, r1
00468f34 mov r5, r0
00468f38 str r3, [sp, #0x10]
00468f3c ldr r3, [pc, #0x120]
00468f40 mov r4, r1
00468f44 mov r6, #0
00468f48 add r3, pc, r3
00468f4c str r3, [sp, #0x14]
00468f50 ldr r3, [pc, #0x110]
00468f54 add r8, sp, #0x24
00468f58 add sl, pc, sl
00468f5c add r3, pc, r3
00468f60 str r3, [sp, #0x18]
00468f64 ldr r3, [pc, #0x100]
00468f68 add r3, pc, r3
00468f6c str r3, [sp, #0x1c]
00468f70 ldr r3, [r4, #0x94]
00468f74 cmp r3, #0
00468f78 beq #0x469000
00468f7c add r1, r7, r6, lsl #2
00468f80 add r1, r1, #0xac
00468f84 mov r0, r5
00468f88 bl #0x38b808
00468f8c ldr r3, [r4, #0xa0]
00468f90 mov r0, r5
00468f94 mov r1, r8
00468f98 str r3, [sp, #0x24]
00468f9c bl #0x461770
00468fa0 ldr r3, [sp, #0x24]
00468fa4 cmp r3, #0
00468fa8 beq #0x468fe8
00468fac mov sb, #0
00468fb0 ldr r1, [r4, #0x94]
00468fb4 lsl fp, sb, #2
00468fb8 mov r0, r5
00468fbc add r1, r1, fp
00468fc0 add r1, r1, #2
00468fc4 bl #0x468db8
00468fc8 ldr r1, [r4, #0x94]
00468fcc mov r0, r5
00468fd0 add sb, sb, #1
00468fd4 add r1, r1, fp
00468fd8 bl #0x468e68
00468fdc ldr r3, [sp, #0x24]
00468fe0 cmp r3, sb
00468fe4 bhi #0x468fb0
00468fe8 add r6, r6, #1
00468fec cmp r6, #3
00468ff0 add r4, r4, #4
00468ff4 bne #0x468f70
00468ff8 add sp, sp, #0x2c
00468ffc pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469000 ldr r1, [sp, #0xc]
00469004 ldr r2, [sl, r1]
00469008 ldr r2, [r2]
0046900c cmp r2, #2
00469010 beq #0x469050
00469014 cmp r2, #1
00469018 bne #0x468ff8
0046901c ldr r3, [sp, #0x10]
00469020 mov ip, #0x22c
00469024 ldr r1, [sp, #0x14]
00469028 ldr r0, [sl, r3]
0046902c ldr r2, [sp, #0x18]
00469030 ldr r3, [sp, #0x1c]
00469034 add r0, r0, #0xa8
00469038 str ip, [sp]
0046903c bl #0x30e004
00469040 ldr r3, [r4, #0x94]
00469044 cmp r3, #0
00469048 bne #0x468f7c
0046904c b #0x468ff8
00469050 str r3, [r3]
00469054 b #0x468ff8
00469058 andeq r3, r0, r0, asr #19
0046905c subseq fp, r2, r8, lsr fp
00469060 andeq r1, r0, r0, asr #19
00469064 umaaleq r5, r5, r0, r4
00469068 subeq r4, r6, r4, lsr #11
0046906c subeq r4, r6, r0, asr #10

SOURCE 0x468930 _ZN14PlayerSavegame17__SavePlayerLevelEP11IStreamBasePv
00468930 add r1, r1, #0x30
00468934 b #0x38b808

SOURCE 0x4688f8 _ZN14PlayerSavegame21__SaveDifficultyLevelEP11IStreamBasePv
004688f8 ldr r3, [pc, #0x28]
004688fc ldr r2, [pc, #0x28]
00468900 push {r4, r5, r6, lr}
00468904 add r3, pc, r3
00468908 mov r4, r1
0046890c ldr r1, [r3, r2]
00468910 mov r5, r0
00468914 bl #0x38b808
00468918 mov r0, r5
0046891c add r1, r4, #0x3c
00468920 pop {r4, r5, r6, lr}
00468924 b #0x38b808
00468928 subseq ip, r2, ip, lsl #3
0046892c muleq r0, ip, sl

SOURCE 0x469e6c _ZN14PlayerSavegame12__SaveSkillsEP11IStreamBasePv
00469e6c push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469e70 ldr sb, [pc, #0x21c]
00469e74 ldr r2, [pc, #0x21c]
00469e78 sub sp, sp, #0x3c
00469e7c add sb, pc, sb
00469e80 str r2, [sp, #0xc]
00469e84 ldr r2, [sb, r2]
00469e88 ldr r3, [r1, #0x80]
00469e8c mov r5, r1
00469e90 ldr r2, [r2]
00469e94 cmp r3, #0
00469e98 mov r4, r0
00469e9c str r2, [sp, #0x34]
00469ea0 beq #0x46a030
00469ea4 ldr r3, [r5, #0x84]
00469ea8 add r7, sp, #0x1c
00469eac mov r0, r7
00469eb0 mov r1, #0x10
00469eb4 str r3, [sp, #0x18]
00469eb8 str r7, [sp, #0x2c]
00469ebc str r7, [sp, #0x30]
00469ec0 bl #0x31167c
00469ec4 ldr r3, [sp, #0x2c]
00469ec8 mov r6, #0
00469ecc mov r0, r4
00469ed0 strb r6, [r3]
00469ed4 add r1, sp, #0x18
00469ed8 bl #0x38b808
00469edc ldr r3, [sp, #0x18]
00469ee0 cmp r3, r6
00469ee4 ble #0x469f4c
00469ee8 ldr r3, [pc, #0x1ac]
00469eec ldr sl, [sb, r3]
00469ef0 ldr r2, [r5, #0x80]
00469ef4 ldr r3, [sl]
00469ef8 lsl r8, r6, #3
00469efc ldr r2, [r2, r6, lsl #3]
00469f00 add r6, r6, #1
00469f04 ldr fp, [r3, r2, lsl #2]
00469f08 mov r0, fp
00469f0c bl #0x30de54
00469f10 mov r1, fp
00469f14 add r2, fp, r0
00469f18 mov r0, r7
00469f1c bl #0x3109e0
00469f20 mov r0, r4
00469f24 mov r1, r7
00469f28 bl #0x461668
00469f2c ldr r1, [r5, #0x80]
00469f30 mov r0, r4
00469f34 add r1, r1, r8
00469f38 add r1, r1, #4
00469f3c bl #0x468db8
00469f40 ldr r3, [sp, #0x18]
00469f44 cmp r3, r6
00469f48 bgt #0x469ef0
00469f4c mov sl, #0
00469f50 add fp, sp, #0x14
00469f54 ldr r3, [r5, #0x88]
00469f58 mov r0, r4
00469f5c mov r1, fp
00469f60 add r3, r3, sl
00469f64 ldr r3, [r3, #0x10]
00469f68 str r3, [sp, #0x14]
00469f6c bl #0x461770
00469f70 ldr r8, [r5, #0x88]
00469f74 add r8, r8, sl
00469f78 ldr r6, [r8, #8]
00469f7c cmp r6, r8
00469f80 beq #0x469fc8
00469f84 add r1, r6, #0x10
00469f88 mov r0, r4
00469f8c bl #0x38b808
00469f90 mov r0, r4
00469f94 add r1, r6, #0x14
00469f98 bl #0x38b808
00469f9c ldr r2, [r6, #0xc]
00469fa0 cmp r2, #0
00469fa4 bne #0x469fb0
00469fa8 b #0x469ffc
00469fac mov r2, r3
00469fb0 ldr r3, [r2, #8]
00469fb4 cmp r3, #0
00469fb8 bne #0x469fac
00469fbc mov r6, r2
00469fc0 cmp r8, r6
00469fc4 bne #0x469f84
00469fc8 add sl, sl, #0x18
00469fcc cmp sl, #0x30
00469fd0 bne #0x469f54
00469fd4 mov r0, r7
00469fd8 bl #0x3139ac
00469fdc ldr r2, [sp, #0xc]
00469fe0 ldr r3, [sb, r2]
00469fe4 ldr r2, [sp, #0x34]
00469fe8 ldr r3, [r3]
00469fec cmp r2, r3
00469ff0 bne #0x46a090
00469ff4 add sp, sp, #0x3c
00469ff8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469ffc ldr r3, [r6, #4]
0046a000 ldr r1, [r3, #0xc]
0046a004 cmp r6, r1
0046a008 bne #0x46a024
0046a00c mov r6, r3
0046a010 ldr r3, [r3, #4]
0046a014 ldr r2, [r3, #0xc]
0046a018 cmp r2, r6
0046a01c beq #0x46a00c
0046a020 ldr r2, [r6, #0xc]
0046a024 cmp r3, r2
0046a028 movne r6, r3
0046a02c b #0x469fc0
0046a030 ldr r2, [pc, #0x68]
0046a034 ldr r2, [sb, r2]
0046a038 ldr r2, [r2]
0046a03c cmp r2, #2
0046a040 streq r3, [r3]
0046a044 beq #0x469fdc
0046a048 cmp r2, #1
0046a04c bne #0x469fdc
0046a050 ldr r0, [pc, #0x4c]
0046a054 ldr r1, [pc, #0x4c]
0046a058 ldr r2, [pc, #0x4c]
0046a05c ldr r0, [sb, r0]
0046a060 ldr r3, [pc, #0x48]
0046a064 movw ip, #0x1cf
0046a068 add r1, pc, r1
0046a06c add r3, pc, r3
0046a070 add r0, r0, #0xa8
0046a074 add r2, pc, r2
0046a078 str ip, [sp]
0046a07c bl #0x30e004
0046a080 ldr r3, [r5, #0x80]
0046a084 cmp r3, #0
0046a088 bne #0x469ea4
0046a08c b #0x469fdc
0046a090 bl #0x30e310
0046a094 subseq sl, r2, r4, lsl ip
0046a098 andeq r4, r0, ip, lsr #1
0046a09c ldrdeq r3, r4, [r0], -r8
0046a0a0 andeq r3, r0, r0, asr #19
0046a0a4 andeq r1, r0, r0, asr #19
0046a0a8 subeq r4, r5, r0, ror r3
0046a0ac subeq r3, r6, r4, lsr #9
0046a0b0 subeq r3, r6, ip, lsr r4

SOURCE 0x4688c0 _ZN14PlayerSavegame16__SavePlayerNameEP11IStreamBasePv
004688c0 add r1, r1, #0x18
004688c4 b #0x461668

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

SOURCE 0x4689a8 _ZN14PlayerSavegame19__SaveUseSpawnPointEP11IStreamBasePv
004689a8 push {r4, r5, r6, lr}
004689ac mov r4, r1
004689b0 mov r5, r0
004689b4 add r1, r1, #0x4c
004689b8 bl #0x33e138
004689bc mov r0, r5
004689c0 add r1, r4, #0x4d
004689c4 bl #0x33e138
004689c8 mov r0, r5
004689cc add r1, r4, #0x4e
004689d0 pop {r4, r5, r6, lr}
004689d4 b #0x33e138

SOURCE 0x4688c8 _ZN14PlayerSavegame21__SaveLevelEntryPointEP11IStreamBasePv
004688c8 push {r4, r5, r6, lr}
004688cc mov r4, r1
004688d0 mov r5, r0
004688d4 add r1, r1, #0x40
004688d8 bl #0x38b808
004688dc mov r0, r5
004688e0 add r1, r4, #0x44
004688e4 bl #0x38b808
004688e8 mov r0, r5
004688ec add r1, r4, #0x48
004688f0 pop {r4, r5, r6, lr}
004688f4 b #0x38b808

SOURCE 0x468bf0 _ZN14PlayerSavegame18__SaveCurrentFaeryEP11IStreamBasePv
00468bf0 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00468bf4 ldr r3, [pc, #0xc0]
00468bf8 sub sp, sp, #0x14
00468bfc ldr r8, [pc, #0xbc]
00468c00 add r3, pc, r3
00468c04 str r3, [sp, #8]
00468c08 ldr r3, [pc, #0xb4]
00468c0c ldr fp, [pc, #0xb4]
00468c10 ldr sl, [pc, #0xb4]
00468c14 add r3, pc, r3
00468c18 ldr sb, [pc, #0xb0]
00468c1c mov r6, r1
00468c20 mov r7, r0
00468c24 add fp, pc, fp
00468c28 str r3, [sp, #0xc]
00468c2c mov r5, r1
00468c30 mov r4, #0
00468c34 add r8, pc, r8
00468c38 ldr r3, [r5, #0x94]
00468c3c cmp r3, #0
00468c40 beq #0x468c6c
00468c44 add r1, r6, r4, lsl #2
00468c48 add r1, r1, #0xac
00468c4c add r4, r4, #1
00468c50 mov r0, r7
00468c54 bl #0x38b808
00468c58 cmp r4, #3
00468c5c add r5, r5, #4
00468c60 bne #0x468c38
00468c64 add sp, sp, #0x14
00468c68 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00468c6c ldr r2, [r8, sl]
00468c70 ldr r2, [r2]
00468c74 cmp r2, #2
00468c78 beq #0x468cb4
00468c7c cmp r2, #1
00468c80 bne #0x468c64
00468c84 ldr r0, [r8, sb]
00468c88 ldr r3, [sp, #0xc]
00468c8c mov ip, #0x254
00468c90 mov r1, fp
00468c94 ldr r2, [sp, #8]
00468c98 add r0, r0, #0xa8
00468c9c str ip, [sp]
00468ca0 bl #0x30e004
00468ca4 ldr r3, [r5, #0x94]
00468ca8 cmp r3, #0
00468cac bne #0x468c44
00468cb0 b #0x468c64
00468cb4 str r3, [r3]
00468cb8 b #0x468c64
00468cbc subeq r4, r6, r0, lsl #18
00468cc0 subseq fp, r2, ip, asr lr
00468cc4 umaaleq r4, r6, r4, r8
00468cc8 strheq r5, [r5], #-0x74
00468ccc andeq r3, r0, r0, asr #19
00468cd0 andeq r1, r0, r0, asr #19

SOURCE 0x4698e4 _ZN14PlayerSavegame17__SavePlayerClassEP11IStreamBasePv
004698e4 push {r4, r5, r6, r7, r8, lr}
004698e8 ldr r4, [pc, #0xa0]
004698ec ldr r5, [pc, #0xa0]
004698f0 sub sp, sp, #0x20
004698f4 add r4, pc, r4
004698f8 ldr r3, [r4, r5]
004698fc mov r7, r0
00469900 ldr r3, [r3]
00469904 str r3, [sp, #0x1c]
00469908 ldr r3, [r1, #0x34]
0046990c cmp r3, #0
00469910 blt #0x469970
00469914 ldr r2, [pc, #0x7c]
00469918 ldr r2, [r4, r2]
0046991c ldr r2, [r2]
00469920 cmp r3, r2
00469924 bhi #0x469970
00469928 ldr r2, [pc, #0x6c]
0046992c add r6, sp, #4
00469930 ldr r2, [r4, r2]
00469934 ldr r2, [r2]
00469938 ldr r8, [r2, r3, lsl #2]
0046993c str r6, [sp, #0x14]
00469940 str r6, [sp, #0x18]
00469944 mov r0, r8
00469948 bl #0x30de54
0046994c mov r1, r8
00469950 add r2, r8, r0
00469954 mov r0, r6
00469958 bl #0x3116e8
0046995c mov r0, r7
00469960 mov r1, r6
00469964 bl #0x461668
00469968 mov r0, r6
0046996c bl #0x3139ac
00469970 ldr r3, [r4, r5]
00469974 ldr r2, [sp, #0x1c]
00469978 ldr r3, [r3]
0046997c cmp r2, r3
00469980 bne #0x46998c
00469984 add sp, sp, #0x20
00469988 pop {r4, r5, r6, r7, r8, pc}
0046998c bl #0x30e310

SOURCE 0x469454 _ZN14PlayerSavegame12__SaveQuestsEP11IStreamBasePv
00469454 ldr r2, [r1, #0x178]
00469458 mov r3, r1
0046945c mov r1, r0
00469460 tst r2, #1
00469464 bne #0x469470
00469468 add r0, r3, #0x118
0046946c b #0x46c6fc
00469470 add r0, r3, #0xb8
00469474 b #0x46c6fc

SOURCE 0x468b20 _ZN14PlayerSavegame15__SaveLevelNameEP11IStreamBasePv
00468b20 push {r4, r5, r6, lr}
00468b24 mov r5, r1
00468b28 add r1, r1, #0x38
00468b2c mov r6, r0
00468b30 bl #0x461770
00468b34 mov r4, #0
00468b38 add r1, r4, #0x14
00468b3c add r1, r5, r1, lsl #2
00468b40 mov r0, r6
00468b44 bl #0x38b808
00468b48 add r1, r5, r4, lsl #2
00468b4c add r1, r1, #0x5c
00468b50 mov r0, r6
00468b54 bl #0x38b808
00468b58 add r1, r5, r4, lsl #2
00468b5c add r1, r1, #0xfc
00468b60 add r4, r4, #1
00468b64 mov r0, r6
00468b68 bl #0x38b808
00468b6c cmp r4, #3
00468b70 bne #0x468b38
00468b74 pop {r4, r5, r6, pc}

SOURCE 0x46a0b4 _ZN14PlayerSavegame15__SaveInventoryEP11IStreamBasePv
0046a0b4 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a0b8 ldr sb, [pc, #0x2bc]
0046a0bc ldr r2, [pc, #0x2bc]
0046a0c0 sub sp, sp, #0x74
0046a0c4 add sb, pc, sb
0046a0c8 str r2, [sp, #0x2c]
0046a0cc ldr r2, [sb, r2]
0046a0d0 ldr r3, [r1, #0x10]
0046a0d4 mov r5, r1
0046a0d8 ldr r2, [r2]
0046a0dc cmp r3, #0
0046a0e0 mov r4, r0
0046a0e4 str r2, [sp, #0x6c]
0046a0e8 beq #0x46a318
0046a0ec ldr r2, [r3, #0x39c]
0046a0f0 add r0, r3, #0x37c
0046a0f4 mvn r1, #0
0046a0f8 str r2, [sp, #0x4c]
0046a0fc bl #0x3fc6a8
0046a100 ldr r3, [r5, #0x10]
0046a104 str r0, [sp, #0x48]
0046a108 add r1, sp, #0x4c
0046a10c ldr r2, [r3, #0x384]
0046a110 ldr r3, [r3, #0x388]
0046a114 mov r0, r4
0046a118 add r6, sp, #0x54
0046a11c rsb r3, r2, r3
0046a120 asr r3, r3, #2
0046a124 str r3, [sp, #0x44]
0046a128 bl #0x461770
0046a12c mov r0, r4
0046a130 add r1, sp, #0x48
0046a134 bl #0x461770
0046a138 mov r0, r4
0046a13c add r1, sp, #0x44
0046a140 bl #0x461770
0046a144 ldr r3, [r5, #0x10]
0046a148 mov r0, r6
0046a14c mov r1, #0x10
0046a150 ldr r2, [r3, #0x388]
0046a154 str r2, [sp, #8]
0046a158 ldr sl, [r3, #0x384]
0046a15c str r6, [sp, #0x64]
0046a160 str r6, [sp, #0x68]
0046a164 bl #0x31167c
0046a168 ldr r3, [sp, #8]
0046a16c mov r2, #0
0046a170 cmp sl, r3
0046a174 ldr r3, [sp, #0x64]
0046a178 strb r2, [r3]
0046a17c beq #0x46a2f0
0046a180 add r3, sp, #0x3c
0046a184 str r3, [sp, #0x1c]
0046a188 ldr r3, [pc, #0x1f4]
0046a18c ldr r2, [pc, #0x1f4]
0046a190 ldr r3, [sb, r3]
0046a194 str r2, [sp, #0x28]
0046a198 add r2, sp, #0x38
0046a19c str r3, [sp, #0x24]
0046a1a0 str r2, [sp, #0x18]
0046a1a4 add r3, sp, #0x34
0046a1a8 add r2, sp, #0x30
0046a1ac str r3, [sp, #0x14]
0046a1b0 str r2, [sp, #0x10]
0046a1b4 add r3, sp, #0x53
0046a1b8 add r2, sp, #0x40
0046a1bc str r3, [sp, #0xc]
0046a1c0 str r2, [sp, #0x20]
0046a1c4 ldr r5, [sl]
0046a1c8 ldr r3, [sp, #0x24]
0046a1cc ldr r7, [r5]
0046a1d0 ldr r8, [r3]
0046a1d4 mov r0, r7
0046a1d8 bl #0x3f9e00
0046a1dc ldr r8, [r8, r0, lsl #2]
0046a1e0 mov r0, r8
0046a1e4 bl #0x30de54
0046a1e8 mov r1, r8
0046a1ec add r2, r8, r0
0046a1f0 mov r0, r6
0046a1f4 bl #0x3109e0
0046a1f8 mov r0, r4
0046a1fc mov r1, r6
0046a200 bl #0x461668
0046a204 ldrsb r3, [r5, #4]
0046a208 mov r0, r4
0046a20c ldr r1, [sp, #0x1c]
0046a210 str r3, [sp, #0x3c]
0046a214 bl #0x38b808
0046a218 ldrsb r3, [r5, #5]
0046a21c mov r0, r4
0046a220 ldr r1, [sp, #0x18]
0046a224 str r3, [sp, #0x38]
0046a228 bl #0x38b808
0046a22c ldrsh r3, [r7, #0x50]
0046a230 mov r0, r4
0046a234 ldr r1, [sp, #0x14]
0046a238 str r3, [sp, #0x34]
0046a23c bl #0x38b808
0046a240 ldr r3, [r7, #0x54]
0046a244 mov r0, r4
0046a248 ldr r1, [sp, #0x10]
0046a24c str r3, [sp, #0x30]
0046a250 bl #0x38b808
0046a254 ldrb r3, [r7, #0x68]
0046a258 ldr r1, [sp, #0xc]
0046a25c mov r0, r4
0046a260 strb r3, [sp, #0x53]
0046a264 bl #0x39f828
0046a268 mov r0, r7
0046a26c bl #0x3f9e80
0046a270 ldr r1, [sp, #0x20]
0046a274 str r0, [sp, #0x40]
0046a278 mov r0, r4
0046a27c bl #0x461770
0046a280 ldr r3, [sp, #0x40]
0046a284 cmp r3, #0
0046a288 beq #0x46a2e0
0046a28c ldr r2, [sp, #0x28]
0046a290 mov r5, #0
0046a294 ldr r8, [sb, r2]
0046a298 mov r1, r5
0046a29c mov r0, r7
0046a2a0 ldr fp, [r8]
0046a2a4 bl #0x3fa038
0046a2a8 ldr fp, [fp, r0, lsl #2]
0046a2ac add r5, r5, #1
0046a2b0 mov r0, fp
0046a2b4 bl #0x30de54
0046a2b8 mov r1, fp
0046a2bc add r2, fp, r0
0046a2c0 mov r0, r6
0046a2c4 bl #0x3109e0
0046a2c8 mov r0, r4
0046a2cc mov r1, r6
0046a2d0 bl #0x461668
0046a2d4 ldr r3, [sp, #0x40]
0046a2d8 cmp r3, r5
0046a2dc bhi #0x46a298
0046a2e0 ldr r3, [sp, #8]
0046a2e4 add sl, sl, #4
0046a2e8 cmp sl, r3
0046a2ec bne #0x46a1c4
0046a2f0 mov r0, r6
0046a2f4 bl #0x3139ac
0046a2f8 ldr r2, [sp, #0x2c]
0046a2fc ldr r3, [sb, r2]
0046a300 ldr r2, [sp, #0x6c]
0046a304 ldr r3, [r3]
0046a308 cmp r2, r3
0046a30c bne #0x46a378
0046a310 add sp, sp, #0x74
0046a314 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046a318 ldr r2, [pc, #0x6c]
0046a31c ldr r2, [sb, r2]
0046a320 ldr r2, [r2]
0046a324 cmp r2, #2
0046a328 streq r3, [r3]
0046a32c beq #0x46a2f8
0046a330 cmp r2, #1
0046a334 bne #0x46a2f8
0046a338 ldr r0, [pc, #0x50]
0046a33c ldr r1, [pc, #0x50]
0046a340 ldr r2, [pc, #0x50]
0046a344 ldr r0, [sb, r0]
0046a348 ldr r3, [pc, #0x4c]
0046a34c movw ip, #0x2de
0046a350 add r1, pc, r1
0046a354 add r3, pc, r3
0046a358 add r0, r0, #0xa8
0046a35c add r2, pc, r2
0046a360 str ip, [sp]
0046a364 bl #0x30e004
0046a368 ldr r3, [r5, #0x10]
0046a36c cmp r3, #0
0046a370 beq #0x46a2f8
0046a374 b #0x46a0ec
0046a378 bl #0x30e310
0046a37c subseq sl, r2, ip, asr #19
0046a380 andeq r4, r0, ip, lsr #1
0046a384 andeq r1, r0, r4, asr ip
0046a388 andeq r1, r0, ip, asr #5
0046a38c andeq r3, r0, r0, asr #19
0046a390 andeq r1, r0, r0, asr #19
0046a394 subeq r4, r5, r8, lsl #1
0046a398 subeq r3, r6, ip, lsr r1
0046a39c subeq r3, r6, r4, asr r1

SOURCE 0x46a7a0 _ZN14PlayerSavegame17__SaveLevelStatesEP11IStreamBasePv
0046a7a0 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a7a4 ldr r2, [pc, #0x208]
0046a7a8 ldr r3, [pc, #0x208]
0046a7ac sub sp, sp, #0x44
0046a7b0 add r2, pc, r2
0046a7b4 str r3, [sp, #0x14]
0046a7b8 ldr r3, [r2, r3]
0046a7bc cmn r1, #0x68
0046a7c0 str r2, [sp, #4]
0046a7c4 ldr r3, [r3]
0046a7c8 mov r7, r1
0046a7cc mov r5, r0
0046a7d0 str r3, [sp, #0x3c]
0046a7d4 beq #0x46a98c
0046a7d8 cmn r1, #0x74
0046a7dc beq #0x46a98c
0046a7e0 add r6, sp, #0x24
0046a7e4 mov r1, #0x10
0046a7e8 mov r0, r6
0046a7ec str r6, [sp, #0x34]
0046a7f0 str r6, [sp, #0x38]
0046a7f4 bl #0x31167c
0046a7f8 ldr r2, [sp, #4]
0046a7fc ldr r3, [pc, #0x1b8]
0046a800 ldr r1, [pc, #0x1b8]
0046a804 mov r8, #0
0046a808 ldr r3, [r2, r3]
0046a80c str r1, [sp, #0xc]
0046a810 add sb, sp, #0x1c
0046a814 str r3, [sp, #8]
0046a818 ldr r3, [sp, #0x34]
0046a81c strb r8, [r3]
0046a820 add r3, sp, #0x20
0046a824 str r3, [sp, #0x10]
0046a828 ldr r1, [sp, #8]
0046a82c mov r0, r5
0046a830 ldr r3, [r1]
0046a834 ldr r1, [sp, #0x10]
0046a838 str r3, [sp, #0x20]
0046a83c bl #0x38b808
0046a840 ldr r3, [sp, #0x20]
0046a844 cmp r3, #0
0046a848 ble #0x46a8b8
0046a84c ldr r3, [sp, #4]
0046a850 ldr r2, [sp, #0xc]
0046a854 mov r4, #0
0046a858 ldr sl, [r3, r2]
0046a85c ldr r3, [sl]
0046a860 ldr fp, [r3, r4, lsl #2]
0046a864 mov r0, fp
0046a868 bl #0x30de54
0046a86c mov r1, fp
0046a870 add r2, fp, r0
0046a874 mov r0, r6
0046a878 bl #0x3109e0
0046a87c mov r0, r5
0046a880 mov r1, r6
0046a884 bl #0x461668
0046a888 mov r1, r4
0046a88c mov r2, r8
0046a890 mov r0, r7
0046a894 bl #0x467040
0046a898 mov r1, sb
0046a89c str r0, [sp, #0x1c]
0046a8a0 mov r0, r5
0046a8a4 bl #0x38b808
0046a8a8 ldr r3, [sp, #0x20]
0046a8ac add r4, r4, #1
0046a8b0 cmp r3, r4
0046a8b4 bgt #0x46a85c
0046a8b8 add r8, r8, #1
0046a8bc cmp r8, #3
0046a8c0 bne #0x46a828
0046a8c4 ldr r3, [pc, #0xf8]
0046a8c8 ldr r2, [sp, #4]
0046a8cc ldr r1, [pc, #0xf4]
0046a8d0 mov r8, #0
0046a8d4 ldr r3, [r2, r3]
0046a8d8 str r1, [sp, #0xc]
0046a8dc add sb, sp, #0x1c
0046a8e0 str r3, [sp, #8]
0046a8e4 mov sl, r7
0046a8e8 ldr r1, [sp, #8]
0046a8ec mov r0, r5
0046a8f0 ldr r3, [r1]
0046a8f4 ldr r1, [sp, #0x10]
0046a8f8 str r3, [sp, #0x20]
0046a8fc bl #0x38b808
0046a900 ldr r3, [sp, #0x20]
0046a904 cmp r3, #0
0046a908 ble #0x46a978
0046a90c ldr r3, [sp, #4]
0046a910 ldr r2, [sp, #0xc]
0046a914 mov r4, #0
0046a918 ldr r7, [r3, r2]
0046a91c ldr r3, [r7]
0046a920 ldr fp, [r3, r4, lsl #2]
0046a924 mov r0, fp
0046a928 bl #0x30de54
0046a92c mov r1, fp
0046a930 add r2, fp, r0
0046a934 mov r0, r6
0046a938 bl #0x3109e0
0046a93c mov r0, r5
0046a940 mov r1, r6
0046a944 bl #0x461668
0046a948 mov r1, r4
0046a94c mov r2, r8
0046a950 mov r0, sl
0046a954 bl #0x466d10
0046a958 mov r1, sb
0046a95c str r0, [sp, #0x1c]
0046a960 mov r0, r5
0046a964 bl #0x38b808
0046a968 ldr r3, [sp, #0x20]
0046a96c add r4, r4, #1
0046a970 cmp r3, r4
0046a974 bgt #0x46a91c
0046a978 add r8, r8, #1
0046a97c cmp r8, #3
0046a980 bne #0x46a8e8
0046a984 mov r0, r6
0046a988 bl #0x3139ac
0046a98c ldr r2, [sp, #4]
0046a990 ldr r1, [sp, #0x14]
0046a994 ldr r3, [r2, r1]
0046a998 ldr r2, [sp, #0x3c]
0046a99c ldr r3, [r3]
0046a9a0 cmp r2, r3
0046a9a4 bne #0x46a9b0
0046a9a8 add sp, sp, #0x44
0046a9ac pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046a9b0 bl #0x30e310
0046a9b4 subseq sl, r2, r0, ror #5
0046a9b8 andeq r4, r0, ip, lsr #1
0046a9bc andeq r1, r0, r0, asr #17
0046a9c0 andeq r3, r0, ip, asr fp
0046a9c4 andeq r2, r0, r4, ror r2
0046a9c8 andeq r2, r0, r8, ror #6

SOURCE 0x4689d8 _ZN14PlayerSavegame16__SavePropertiesEP11IStreamBasePv
004689d8 push {r4, r5, r6, r7, r8, sl, lr}
004689dc ldr r7, [r1, #0x10]
004689e0 ldr r5, [pc, #0xec]
004689e4 sub sp, sp, #0x14
004689e8 cmp r7, #0
004689ec mov r6, r1
004689f0 mov r4, r0
004689f4 add r5, pc, r5
004689f8 beq #0x468a74
004689fc mov r3, #0xe0
00468a00 add r1, sp, #0x10
00468a04 str r3, [r1, #-4]!
00468a08 mov r0, r4
00468a0c bl #0x38b808
00468a10 ldr r3, [sp, #0xc]
00468a14 cmp r3, #0
00468a18 ble #0x468a60
00468a1c ldr r3, [pc, #0xb4]
00468a20 add r7, r7, #0x8e0
00468a24 add r7, r7, #0xc
00468a28 ldr sl, [r5, r3]
00468a2c add r8, sp, #8
00468a30 mov r5, #0
00468a34 ldr r3, [sl, r5, lsl #2]
00468a38 mov r0, r4
00468a3c mov r1, r8
00468a40 add r3, r7, r3
00468a44 ldr r3, [r3, #4]
00468a48 add r5, r5, #1
00468a4c str r3, [sp, #8]
00468a50 bl #0x38b808
00468a54 ldr r3, [sp, #0xc]
00468a58 cmp r3, r5
00468a5c bgt #0x468a34
00468a60 mov r0, r4
00468a64 add r1, r6, #0x194
00468a68 bl #0x33e138
00468a6c add sp, sp, #0x14
00468a70 pop {r4, r5, r6, r7, r8, sl, pc}
00468a74 ldr r3, [pc, #0x60]
00468a78 ldr r3, [r5, r3]
00468a7c ldr r3, [r3]
00468a80 cmp r3, #2
00468a84 streq r7, [r7]
00468a88 beq #0x468a6c
00468a8c cmp r3, #1
00468a90 bne #0x468a6c
00468a94 ldr r0, [pc, #0x44]
00468a98 ldr r1, [pc, #0x44]
00468a9c ldr r2, [pc, #0x44]
00468aa0 ldr r0, [r5, r0]
00468aa4 ldr r3, [pc, #0x40]
00468aa8 mov ip, #0x344
00468aac add r1, pc, r1
00468ab0 add r0, r0, #0xa8
00468ab4 add r2, pc, r2
00468ab8 add r3, pc, r3
00468abc str ip, [sp]
00468ac0 bl #0x30e004
00468ac4 ldr r7, [r6, #0x10]
00468ac8 cmp r7, #0
00468acc bne #0x4689fc
00468ad0 b #0x468a6c
