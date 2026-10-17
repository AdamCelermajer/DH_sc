_ZN9Character18SafeGetCharPropsIdEv 0x3b3d38
003b3d38: push {r4, r5, r6, r7, r8, sl, lr}
003b3d3c: movw r6, #0x13c8
003b3d40: ldrsh r3, [r0, r6]
003b3d44: ldr r7, [pc, #0x294]
003b3d48: mov r4, r0
003b3d4c: cmn r3, #1
003b3d50: add r7, pc, r7
003b3d54: sub sp, sp, #0xc
003b3d58: movne r0, r3
003b3d5c: beq #0x3b3d68
003b3d60: add sp, sp, #0xc
003b3d64: pop {r4, r5, r6, r7, r8, sl, pc}
003b3d68: ldr r3, [r4]
003b3d6c: mov lr, pc
003b3d70: ldr pc, [r3, #0x28]
003b3d74: subs sl, r0, #0
003b3d78: bne #0x3b3dcc
003b3d7c: movw r3, #0x13a8
003b3d80: ldr r2, [r4, r3]
003b3d84: movw r3, #0x13ac
003b3d88: ldr r3, [r4, r3]
003b3d8c: cmp r2, r3
003b3d90: beq #0x3b3f20
003b3d94: mov r0, r4
003b3d98: bl #0x3b36ec
003b3d9c: cmp r0, #0
003b3da0: blt #0x3b3dc4
003b3da4: ldr r3, [pc, #0x238]
003b3da8: mov r5, #0xc
003b3dac: ldr r3, [r7, r3]
003b3db0: ldr r3, [r3]
003b3db4: mla r5, r5, r0, r3
003b3db8: ldr r1, [r5, #4]
003b3dbc: cmp r1, #0
003b3dc0: bne #0x3b3e08
003b3dc4: ldrsh r0, [r4, r6]
003b3dc8: b #0x3b3d60
003b3dcc: mov r1, #1
003b3dd0: mov r0, r4
003b3dd4: bl #0x3bc4d0
003b3dd8: mov r0, r4
003b3ddc: bl #0x3bb7fc
003b3de0: uxth r0, r0
003b3de4: sxth r1, r0
003b3de8: cmn r1, #1
003b3dec: strh r0, [r4, r6]
003b3df0: beq #0x3b3ebc
003b3df4: mov r0, r4
003b3df8: bl #0x3bb814
003b3dfc: movw r3, #0x13c8
003b3e00: ldrsh r0, [r4, r3]
003b3e04: b #0x3b3d60
003b3e08: ldr r2, [pc, #0x1d8]
003b3e0c: movw lr, #0xe6ab
003b3e10: movw r3, #0xdb17
003b3e14: ldr r2, [r7, r2]
003b3e18: movt r3, #0x2b52
003b3e1c: movw ip, #0xf26b
003b3e20: ldr r0, [r2]
003b3e24: movt ip, #0xda
003b3e28: mul r0, lr, r0
003b3e2c: add r0, r0, #0x2b000
003b3e30: add r0, r0, #0x3fc
003b3e34: add r0, r0, #1
003b3e38: umull lr, r3, r3, r0
003b3e3c: rsb lr, r3, r0
003b3e40: add r3, r3, lr, lsr #1
003b3e44: lsr r3, r3, #0x17
003b3e48: mls r3, ip, r3, r0
003b3e4c: str r3, [r2]
003b3e50: mov r0, r3
003b3e54: bl #0x30eb2c
003b3e58: ldr r3, [pc, #0x18c]
003b3e5c: eor r6, r1, r1, asr #31
003b3e60: sub r6, r6, r1, asr #31
003b3e64: ldr r3, [r7, r3]
003b3e68: ldr r2, [r3]
003b3e6c: add r2, r2, #1
003b3e70: str r2, [r3]
003b3e74: ldr r3, [r5, #4]
003b3e78: cmp r3, r6
003b3e7c: bgt #0x3b3ea0
003b3e80: ldr r3, [pc, #0x168]
003b3e84: ldr r3, [r7, r3]
003b3e88: ldr r3, [r3]
003b3e8c: cmp r3, #2
003b3e90: streq sl, [sl]
003b3e94: beq #0x3b3ea0
003b3e98: cmp r3, #1
003b3e9c: beq #0x3b3fac
003b3ea0: ldr r3, [r5, #8]
003b3ea4: add r6, r3, r6, lsl #3
003b3ea8: ldrh r0, [r6, #4]
003b3eac: movw r3, #0x13c8
003b3eb0: strh r0, [r4, r3]
003b3eb4: sxth r0, r0
003b3eb8: b #0x3b3d60
003b3ebc: ldr r3, [pc, #0x130]
003b3ec0: ldr r3, [r7, r3]
003b3ec4: ldr r6, [r3]
003b3ec8: cmp r6, #0
003b3ecc: beq #0x3b3fa0
003b3ed0: ldr r3, [pc, #0x120]
003b3ed4: ldr r8, [pc, #0x120]
003b3ed8: mov r5, #0
003b3edc: ldr r3, [r7, r3]
003b3ee0: add r8, pc, r8
003b3ee4: ldr r7, [r3]
003b3ee8: b #0x3b3ef8
003b3eec: add r5, r5, #1
003b3ef0: cmp r5, r6
003b3ef4: beq #0x3b3fa0
003b3ef8: ldr r1, [r7, r5, lsl #2]
003b3efc: mov r0, r8
003b3f00: bl #0x30e31c
003b3f04: cmp r0, #0
003b3f08: bne #0x3b3eec
003b3f0c: uxth r5, r5
003b3f10: sxth r1, r5
003b3f14: movw r3, #0x13c8
003b3f18: strh r5, [r4, r3]
003b3f1c: b #0x3b3df4
003b3f20: movw r3, #0x13c4
003b3f24: ldr r8, [r4, r3]
003b3f28: mov r3, #0x13c0
003b3f2c: ldr r3, [r4, r3]
003b3f30: cmp r3, r8
003b3f34: beq #0x3b3dc4
003b3f38: ldr r3, [pc, #0xb4]
003b3f3c: ldr r3, [r7, r3]
003b3f40: ldr r6, [r3]
003b3f44: cmp r6, #0
003b3f48: beq #0x3b3f94
003b3f4c: ldr r3, [pc, #0xa4]
003b3f50: mov r5, sl
003b3f54: ldr r3, [r7, r3]
003b3f58: ldr r7, [r3]
003b3f5c: b #0x3b3f6c
003b3f60: add r5, r5, #1
003b3f64: cmp r5, r6
003b3f68: beq #0x3b3f94
003b3f6c: ldr r1, [r7, r5, lsl #2]
003b3f70: mov r0, r8
003b3f74: bl #0x30e31c
003b3f78: cmp r0, #0
003b3f7c: bne #0x3b3f60
003b3f80: uxth r5, r5
003b3f84: sxth r0, r5
003b3f88: movw r3, #0x13c8
003b3f8c: strh r5, [r4, r3]
003b3f90: b #0x3b3d60
003b3f94: mvn r0, #0
003b3f98: movw r5, #0xffff
003b3f9c: b #0x3b3f88
003b3fa0: mvn r1, #0
003b3fa4: movw r5, #0xffff
003b3fa8: b #0x3b3f14
003b3fac: ldr r0, [pc, #0x4c]
003b3fb0: ldr r1, [pc, #0x4c]
003b3fb4: ldr r2, [pc, #0x4c]
003b3fb8: ldr r0, [r7, r0]
003b3fbc: ldr r3, [pc, #0x48]
003b3fc0: mov ip, #0x2f4
003b3fc4: add r1, pc, r1
003b3fc8: add r2, pc, r2
003b3fcc: add r3, pc, r3
003b3fd0: add r0, r0, #0xa8
003b3fd4: str ip, [sp]
003b3fd8: bl #0x30e004
003b3fdc: b #0x3b3ea0
003b3fe0: subseq r0, lr, r0, asr #26
003b3fe4: strheq r4, [r0], -r8
003b3fe8: muleq r0, r4, ip
003b3fec: andeq r1, r0, r8, lsl #1
003b3ff0: andeq r3, r0, r0, asr #19
003b3ff4: andeq r4, r0, r4, lsl #4
003b3ff8: andeq r3, r0, r8, lsl #24
003b3ffc: subseq pc, r0, r0, lsl #29
003b4000: andeq r1, r0, r0, asr #19
003b4004: subseq sl, r0, r4, lsl r4
003b4008: ldrheq pc, [r0], #-0xd0
003b400c: subseq pc, r0, r4, ror #27

_ZN9Character26SafeGetCharPropsTemplateIdEv 0x3b36ec
003b36ec: push {r4, r5, r6, r7, r8, lr}
003b36f0: movw r3, #0x13ac
003b36f4: ldr r5, [r0, r3]
003b36f8: movw r3, #0x13a8
003b36fc: ldr r2, [r0, r3]
003b3700: ldr r3, [pc, #0x88]
003b3704: mov r8, r0
003b3708: cmp r2, r5
003b370c: add r3, pc, r3
003b3710: beq #0x3b3784
003b3714: ldr r2, [pc, #0x78]
003b3718: ldr r2, [r3, r2]
003b371c: ldr r6, [r2]
003b3720: cmp r6, #0
003b3724: beq #0x3b3770
003b3728: ldr r2, [pc, #0x68]
003b372c: mov r4, #0
003b3730: ldr r3, [r3, r2]
003b3734: ldr r7, [r3]
003b3738: b #0x3b3748
003b373c: add r4, r4, #1
003b3740: cmp r4, r6
003b3744: beq #0x3b3770
003b3748: ldr r1, [r7, r4, lsl #2]
003b374c: mov r0, r5
003b3750: bl #0x30e31c
003b3754: cmp r0, #0
003b3758: bne #0x3b373c
003b375c: uxth r4, r4
003b3760: movw r3, #0x13ca
003b3764: sxth r0, r4
003b3768: strh r4, [r8, r3]
003b376c: pop {r4, r5, r6, r7, r8, pc}
003b3770: movw r4, #0xffff
003b3774: movw r3, #0x13ca
003b3778: mvn r0, #0
003b377c: strh r4, [r8, r3]
003b3780: pop {r4, r5, r6, r7, r8, pc}
003b3784: movw r3, #0x13ca
003b3788: ldrsh r0, [r0, r3]
003b378c: pop {r4, r5, r6, r7, r8, pc}
003b3790: subseq r1, lr, r4, lsl #7
003b3794: andeq r0, r0, r8, asr #23
003b3798: andeq r1, r0, ip, lsr r7

_ZN9Character7SG_LoadEi 0x3bc4d0
003bc4d0: movw r3, #0x14e8
003bc4d4: ldr r0, [r0, r3]
003bc4d8: cmp r0, #0
003bc4dc: bxeq lr
003bc4e0: b #0x465430

_ZNK9Character17SG_GetPlayerClassEv 0x3bb7fc
003bb7fc: movw r3, #0x14e8
003bb800: ldr r3, [r0, r3]
003bb804: cmp r3, #0
003bb808: mvneq r0, #0
003bb80c: ldrne r0, [r3, #0x34]
003bb810: bx lr

_ZN9Character17SG_SetPlayerClassEi 0x3bb814
003bb814: movw r3, #0x14e8
003bb818: ldr r3, [r0, r3]
003bb81c: cmp r3, #0
003bb820: strne r1, [r3, #0x34]
003bb824: bx lr

_ZN14PlayerSavegame16__LoadPropertiesEP11IStreamBasePv 0x46932c
0046932c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469330: ldr sb, [r1, #0x10]
00469334: ldr r5, [pc, #0xfc]
00469338: sub sp, sp, #0x14
0046933c: cmp sb, #0
00469340: mov r6, r1
00469344: mov r4, r0
00469348: add r5, pc, r5
0046934c: beq #0x4693d8
00469350: mov r0, r4
00469354: add r1, sp, #0xc
00469358: bl #0x38b758
0046935c: ldr r3, [sp, #0xc]
00469360: add r8, sb, #0x560
00469364: cmp r3, #0xe0
00469368: beq #0x469374
0046936c: add sp, sp, #0x14
00469370: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469374: ldr fp, [pc, #0xc0]
00469378: add sb, sb, #0x8e0
0046937c: add sb, sb, #0xc
00469380: mov r7, #0
00469384: add sl, sp, #8
00469388: mov r0, r4
0046938c: mov r1, sl
00469390: bl #0x38b758
00469394: mov r1, r7
00469398: mov r0, r8
0046939c: bl #0x3deed8
004693a0: tst r0, #0x20
004693a4: ldrne r3, [r5, fp]
004693a8: ldrne r2, [sp, #8]
004693ac: ldrne r3, [r3, r7, lsl #2]
004693b0: add r7, r7, #1
004693b4: addne r3, sb, r3
004693b8: strne r2, [r3, #4]
004693bc: ldr r3, [sp, #0xc]
004693c0: cmp r3, r7
004693c4: bgt #0x469388
004693c8: mov r0, r4
004693cc: add r1, r6, #0x194
004693d0: bl #0x33e040
004693d4: b #0x46936c
004693d8: ldr r3, [pc, #0x60]
004693dc: ldr r3, [r5, r3]
004693e0: ldr r3, [r3]
004693e4: cmp r3, #2
004693e8: streq sb, [sb]
004693ec: beq #0x46936c
004693f0: cmp r3, #1
004693f4: bne #0x46936c
004693f8: ldr r0, [pc, #0x44]
004693fc: ldr r1, [pc, #0x44]
00469400: ldr r2, [pc, #0x44]
00469404: ldr r0, [r5, r0]
00469408: ldr r3, [pc, #0x40]
0046940c: movw ip, #0x31a
00469410: add r1, pc, r1
00469414: add r0, r0, #0xa8
00469418: add r2, pc, r2
0046941c: add r3, pc, r3
00469420: str ip, [sp]
00469424: bl #0x30e004
00469428: ldr sb, [r6, #0x10]
0046942c: cmp sb, #0
00469430: bne #0x469350
00469434: b #0x46936c
00469438: subseq fp, r2, r8, asr #14
0046943c: andeq r2, r0, r8, lsr #5
00469440: andeq r3, r0, r0, asr #19
00469444: andeq r1, r0, r0, asr #19
00469448: subeq r4, r5, r8, asr #31
0046944c: subeq r4, r6, r0, lsl #1
00469450: subeq r4, r6, ip, lsl #1

_ZNK14CharProperties8_GetTypeEi 0x3deed8
003deed8: ldr r3, [pc, #0x28]
003deedc: mov r2, r1
003deee0: ldr r1, [pc, #0x24]
003deee4: push {r4, lr}
003deee8: add r3, pc, r3
003deeec: ldr ip, [r3, r1]
003deef0: ldr r1, [ip]
003deef4: add r1, r1, #0x384
003deef8: bl #0x3dedb4
003deefc: cmn r0, #1
003def00: moveq r0, #0x10
003def04: pop {r4, pc}
003def08: subseq r5, fp, r8, lsr #23
003def0c: andeq r2, r0, r0, asr fp

_ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi 0x3dedb4
003dedb4: str lr, [sp, #-4]!
003dedb8: ldr r3, [pc, #0xf0]
003dedbc: cmp r2, #0
003dedc0: sub sp, sp, #0xc
003dedc4: add r3, pc, r3
003dedc8: blt #0x3dedfc
003dedcc: cmp r2, #0xdf
003dedd0: ble #0x3dee20
003dedd4: ldr r2, [pc, #0xd8]
003dedd8: ldr r2, [r3, r2]
003deddc: ldr r2, [r2]
003dede0: cmp r2, #2
003dede4: beq #0x3dee10
003dede8: cmp r2, #1
003dedec: beq #0x3dee78
003dedf0: mvn r0, #0
003dedf4: add sp, sp, #0xc
003dedf8: ldm sp!, {pc}
003dedfc: ldr r2, [pc, #0xb0]
003dee00: ldr r2, [r3, r2]
003dee04: ldr r2, [r2]
003dee08: cmp r2, #2
003dee0c: bne #0x3dee38
003dee10: mov r3, #0
003dee14: str r3, [r3]
003dee18: mvn r0, #0
003dee1c: b #0x3dedf4
003dee20: ldr r0, [pc, #0x90]
003dee24: ldr r3, [r3, r0]
003dee28: ldr r3, [r3, r2, lsl #2]
003dee2c: add r1, r1, r3
003dee30: ldr r0, [r1, #4]
003dee34: b #0x3dedf4
003dee38: cmp r2, #1
003dee3c: bne #0x3dedf0
003dee40: ldr r0, [pc, #0x74]
003dee44: ldr r1, [pc, #0x74]
003dee48: ldr r2, [pc, #0x74]
003dee4c: ldr r0, [r3, r0]
003dee50: ldr r3, [pc, #0x70]
003dee54: movw ip, #0x103
003dee58: add r1, pc, r1
003dee5c: add r0, r0, #0xa8
003dee60: add r2, pc, r2
003dee64: add r3, pc, r3
003dee68: str ip, [sp]
003dee6c: bl #0x30e004
003dee70: mvn r0, #0
003dee74: b #0x3dedf4
003dee78: ldr r0, [pc, #0x3c]
003dee7c: ldr r1, [pc, #0x48]
003dee80: ldr r2, [pc, #0x48]
003dee84: ldr r0, [r3, r0]
003dee88: ldr r3, [pc, #0x44]
003dee8c: mov ip, #0x104
003dee90: add r1, pc, r1
003dee94: add r0, r0, #0xa8
003dee98: add r2, pc, r2
003dee9c: add r3, pc, r3
003deea0: str ip, [sp]
003deea4: bl #0x30e004
003deea8: mvn r0, #0
003deeac: b #0x3dedf4
003deeb0: subseq r5, fp, ip, asr #25
003deeb4: andeq r3, r0, r0, asr #19
003deeb8: andeq r2, r0, r8, lsr #5
003deebc: andeq r1, r0, r0, asr #19
003deec0: subeq pc, sp, r0, lsl #11
003deec4: strheq r6, [lr], #-0xe0
003deec8: subeq r6, lr, ip, asr #28
003deecc: subeq pc, sp, r8, asr #10
003deed0: subeq r6, lr, r8, lsl #29
003deed4: subeq r6, lr, r4, lsl lr

_ZN11IStreamBase6readAsIiEEvRT_ 0x38b758
0038b758: str lr, [sp, #-4]!
0038b75c: mov r3, #0
0038b760: sub sp, sp, #0xc
0038b764: ldr ip, [r0]
0038b768: mov r2, #4
0038b76c: mov lr, pc
0038b770: ldr pc, [ip, #0x18]
0038b774: ldr r3, [pc, #0x74]
0038b778: cmp r0, #4
0038b77c: add r3, pc, r3
0038b780: beq #0x38b7b0
0038b784: ldr r2, [pc, #0x68]
0038b788: ldr r2, [r3, r2]
0038b78c: ldr r2, [r2]
0038b790: cmp r2, #2
0038b794: moveq r3, #0
0038b798: streq r3, [r3]
0038b79c: beq #0x38b7a8
0038b7a0: cmp r2, #1
0038b7a4: beq #0x38b7bc
0038b7a8: add sp, sp, #0xc
0038b7ac: ldm sp!, {pc}
0038b7b0: cmp r1, #0
0038b7b4: beq #0x38b7a8
0038b7b8: b #0x38b784
0038b7bc: ldr r0, [pc, #0x34]
0038b7c0: ldr r1, [pc, #0x34]
0038b7c4: ldr r2, [pc, #0x34]
0038b7c8: ldr r0, [r3, r0]
0038b7cc: ldr r3, [pc, #0x30]
0038b7d0: mov ip, #0x45
0038b7d4: add r1, pc, r1
0038b7d8: add r2, pc, r2
0038b7dc: add r3, pc, r3
0038b7e0: add r0, r0, #0xa8
0038b7e4: str ip, [sp]
0038b7e8: bl #0x30e004
0038b7ec: b #0x38b7a8
0038b7f0: rsbeq sb, r0, r4, lsl r3
0038b7f4: andeq r3, r0, r0, asr #19
0038b7f8: andeq r1, r0, r0, asr #19
0038b7fc: subseq r2, r3, r4, lsl #24
0038b800: subseq r2, r3, r8, lsr #26
0038b804: subseq r2, r3, ip, lsr sp

_ZN11IStreamBase6readAsIbEEvRT_ 0x33e040
0033e040: str lr, [sp, #-4]!
0033e044: mov r3, #0
0033e048: sub sp, sp, #0xc
0033e04c: ldr ip, [r0]
0033e050: mov r2, #1
0033e054: mov lr, pc
0033e058: ldr pc, [ip, #0x18]
0033e05c: ldr r3, [pc, #0x74]
0033e060: cmp r0, #1
0033e064: add r3, pc, r3
0033e068: beq #0x33e098
0033e06c: ldr r2, [pc, #0x68]
0033e070: ldr r2, [r3, r2]
0033e074: ldr r2, [r2]
0033e078: cmp r2, #2
0033e07c: moveq r3, #0
0033e080: streq r3, [r3]
0033e084: beq #0x33e090
0033e088: cmp r2, #1
0033e08c: beq #0x33e0a4
0033e090: add sp, sp, #0xc
0033e094: ldm sp!, {pc}
0033e098: cmp r1, #0
0033e09c: beq #0x33e090
0033e0a0: b #0x33e06c
0033e0a4: ldr r0, [pc, #0x34]
0033e0a8: ldr r1, [pc, #0x34]
0033e0ac: ldr r2, [pc, #0x34]
0033e0b0: ldr r0, [r3, r0]
0033e0b4: ldr r3, [pc, #0x30]
0033e0b8: mov ip, #0x45
0033e0bc: add r1, pc, r1
0033e0c0: add r2, pc, r2
0033e0c4: add r3, pc, r3
0033e0c8: add r0, r0, #0xa8
0033e0cc: str ip, [sp]
0033e0d0: bl #0x30e004
0033e0d4: b #0x33e090
0033e0d8: rsbeq r6, r5, ip, lsr #20
0033e0dc: andeq r3, r0, r0, asr #19
0033e0e0: andeq r1, r0, r0, asr #19
0033e0e4: subseq r0, r8, ip, lsl r3
0033e0e8: subseq r0, r8, r0, asr #8
0033e0ec: subseq r0, r8, r4, asr r4
