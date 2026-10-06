
_ZN9LuaScript4CallEPKcRN3sfc6script3lua12ReturnValuesE 0x37c494 128
0037c494: push {r4, r5, r6, r7, lr}
0037c498: ldr r3, [r2, #0x24]
0037c49c: mov r5, r2
0037c4a0: ldr r4, [pc, #0x64]
0037c4a4: ldr ip, [r3]
0037c4a8: ldr r2, [r3, #4]
0037c4ac: sub sp, sp, #0xc
0037c4b0: mov r6, r0
0037c4b4: cmp ip, r2
0037c4b8: add r4, pc, r4
0037c4bc: mov r7, r1
0037c4c0: beq #0x37c4d4
0037c4c4: mov r0, r3
0037c4c8: mov r1, ip
0037c4cc: add r3, sp, #4
0037c4d0: bl #0x31c3cc
0037c4d4: mov r1, r7
0037c4d8: mov r0, r6
0037c4dc: bl #0x37c314
0037c4e0: mov r2, r5
0037c4e4: mov r1, r0
0037c4e8: add r0, r6, #4
0037c4ec: bl #0x31ab14
0037c4f0: ldr r3, [pc, #0x18]
0037c4f4: ldr r3, [r4, r3]
0037c4f8: ldr r2, [r3]
0037c4fc: add r2, r2, #1
0037c500: str r2, [r3]
0037c504: add sp, sp, #0xc
0037c508: pop {r4, r5, r6, r7, pc}

_ZN3sfc6script3lua5ValueD0Ev 0x319454 28
00319454: push {r4, lr}
00319458: mov r4, r0
0031945c: bl #0x3193e8
00319460: mov r0, r4
00319464: bl #0x310440
00319468: mov r0, r4
0031946c: pop {r4, pc}

_ZN3sfc6script3lua5ValueC1Eb 0x37c764 128
0037c764: ldr r2, [pc, #0x70]
0037c768: ldr ip, [pc, #0x70]
0037c76c: mov r3, r0
0037c770: add r2, pc, r2
0037c774: ldr ip, [r2, ip]
0037c778: push {r4, r5, r6, lr}
0037c77c: add ip, ip, #8
0037c780: mov r4, r0
0037c784: str ip, [r3], #0xc
0037c788: mov r6, r1
0037c78c: mov r0, r3
0037c790: str r3, [r4, #0x1c]
0037c794: str r3, [r4, #0x20]
0037c798: mov r1, #0x10
0037c79c: bl #0x31167c
0037c7a0: ldr r2, [r4, #0x1c]
0037c7a4: add r3, r4, #0x24
0037c7a8: mov r5, #0
0037c7ac: strb r5, [r2]
0037c7b0: mov r0, r3
0037c7b4: str r3, [r4, #0x64]
0037c7b8: str r3, [r4, #0x68]
0037c7bc: bl #0x37be44
0037c7c0: ldr r3, [r4, #0x64]
0037c7c4: mov r0, r4
0037c7c8: mov r1, r6
0037c7cc: str r5, [r3]
0037c7d0: bl #0x31b5cc
0037c7d4: mov r0, r4
0037c7d8: pop {r4, r5, r6, pc}
0037c7dc: rsbeq r8, r1, r0, lsr #6
0037c7e0: muleq r0, r8, r7

_ZN3sfc6script3lua8UserData14createBindingsERNS1_6BinderE 0x33dcc4 4
0033dcc4: bx lr

_ZN6TestUD16TestStaticMethodERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x386e08 92
00386e08: push {r4, r5, r6, lr}
00386e0c: ldr r0, [r0, #4]
00386e10: mov r4, r2
00386e14: mov r5, r1
00386e18: ldr r2, [r0, #4]
00386e1c: ldr r3, [r0]
00386e20: ldr r0, [pc, #0x38]
00386e24: mov r1, r4
00386e28: rsb r3, r3, r2
00386e2c: asr r3, r3, #4
00386e30: add r0, pc, r0
00386e34: add r2, r3, r3, lsl #3
00386e38: add r2, r2, r2, lsl #6
00386e3c: add r2, r3, r2, lsl #3
00386e40: add r2, r2, r2, lsl #15
00386e44: add r2, r3, r2, lsl #3
00386e48: rsb r2, r2, #0
00386e4c: bl #0x30de84
00386e50: mov r0, r5
00386e54: mov r1, r4
00386e58: pop {r4, r5, r6, lr}
00386e5c: b #0x37c9f8
00386e60: subseq fp, r3, r8, lsl #5

_ZN10LuaManagerD1Ev 0x37a0ac 100
0037a0ac: ldr r3, [pc, #0x54]
0037a0b0: ldr r2, [pc, #0x54]
0037a0b4: push {r4, r5, r6, lr}
0037a0b8: add r3, pc, r3
0037a0bc: ldr r2, [r3, r2]
0037a0c0: mov r4, r0
0037a0c4: add r2, r2, #8
0037a0c8: str r2, [r0]
0037a0cc: bl #0x379fe8
0037a0d0: ldr r3, [r4, #0x14]
0037a0d4: cmp r3, #0
0037a0d8: beq #0x37a100
0037a0dc: add r5, r4, #4
0037a0e0: mov r0, r5
0037a0e4: ldr r1, [r4, #8]
0037a0e8: bl #0x379fa8
0037a0ec: mov r3, #0
0037a0f0: str r5, [r4, #0x10]
0037a0f4: str r3, [r4, #0x14]
0037a0f8: str r5, [r4, #0xc]
0037a0fc: str r3, [r4, #8]
0037a100: mov r0, r4
0037a104: pop {r4, r5, r6, pc}

_ZN3sfc6script3lua9ArgumentsD0Ev 0x319260 28
00319260: push {r4, lr}
00319264: mov r4, r0
00319268: bl #0x319228
0031926c: mov r0, r4
00319270: bl #0x310440
00319274: mov r0, r4
00319278: pop {r4, pc}

_ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsERNS4_12ReturnValuesE 0x37c390 140
0037c390: push {r4, r5, r6, r7, r8, lr}
0037c394: mov r5, r3
0037c398: ldr r3, [r3, #0x24]
0037c39c: ldr r4, [pc, #0x70]
0037c3a0: sub sp, sp, #8
0037c3a4: ldr lr, [r3]
0037c3a8: ldr ip, [r3, #4]
0037c3ac: add r4, pc, r4
0037c3b0: mov r6, r0
0037c3b4: cmp lr, ip
0037c3b8: mov r7, r1
0037c3bc: mov r8, r2
0037c3c0: beq #0x37c3d8
0037c3c4: mov r0, r3
0037c3c8: mov r1, lr
0037c3cc: mov r2, ip
0037c3d0: add r3, sp, #4
0037c3d4: bl #0x31c3cc
0037c3d8: mov r1, r7
0037c3dc: mov r0, r6
0037c3e0: bl #0x37c314
0037c3e4: mov r2, r8
0037c3e8: mov r1, r0
0037c3ec: mov r3, r5
0037c3f0: add r0, r6, #4
0037c3f4: bl #0x31abe8
0037c3f8: ldr r3, [pc, #0x18]
0037c3fc: ldr r3, [r4, r3]
0037c400: ldr r2, [r3]
0037c404: add r2, r2, #1
0037c408: str r2, [r3]
0037c40c: add sp, sp, #8
0037c410: pop {r4, r5, r6, r7, r8, pc}
0037c414: rsbeq r8, r1, r4, ror #13
0037c418: strdeq r2, r3, [r0], -ip

_ZN3sfc6script3lua8Instance8loadFileER11IFileStream 0x31ade8 240
0031ade8: push {r4, r5, r6, r7, r8, sl, lr}
0031adec: ldr r4, [pc, #0xd4]
0031adf0: ldr r8, [pc, #0xd4]
0031adf4: sub sp, sp, #0x410
0031adf8: add r4, pc, r4
0031adfc: ldr r3, [r4, r8]
0031ae00: sub sp, sp, #0xc
0031ae04: mov r6, r1
0031ae08: ldr r3, [r3]
0031ae0c: mov sl, r2
0031ae10: mov r5, r0
0031ae14: str r3, [sp, #0x414]
0031ae18: bl #0x31a804
0031ae1c: ldr r2, [pc, #0xac]
0031ae20: ldr r7, [r6, #4]
0031ae24: mov ip, #0
0031ae28: ldr r3, [pc, #0xa4]
0031ae2c: str ip, [sp, #8]
0031ae30: add ip, sp, #0x18
0031ae34: ldr r1, [r4, r2]
0031ae38: sub ip, ip, #4
0031ae3c: add r2, sp, #8
0031ae40: add r3, pc, r3
0031ae44: sub r2, r2, #8
0031ae48: str ip, [sp, #0xc]
0031ae4c: mov r0, r7
0031ae50: mov ip, #0x400
0031ae54: str ip, [sp, #0x10]
0031ae58: stm sp, {r6, sl}
0031ae5c: bl #0x84bbbc
0031ae60: mov r1, r7
0031ae64: mov r2, r0
0031ae68: mov r0, r5
0031ae6c: bl #0x31a8ac
0031ae70: ldr r1, [r5, #4]
0031ae74: cmp r1, #0
0031ae78: bne #0x31aea0
0031ae7c: ldr r6, [r6, #4]
0031ae80: mov r2, r1
0031ae84: mov r3, r1
0031ae88: mov r0, r6
0031ae8c: bl #0x84bc50
0031ae90: mov r1, r6
0031ae94: mov r2, r0
0031ae98: mov r0, r5
0031ae9c: bl #0x31a8ac
0031aea0: ldr r3, [r4, r8]
0031aea4: ldr r2, [sp, #0x414]
0031aea8: mov r0, r5
0031aeac: ldr r3, [r3]
0031aeb0: cmp r2, r3
0031aeb4: bne #0x31aec4
0031aeb8: add sp, sp, #0x1c
0031aebc: add sp, sp, #0x400
0031aec0: pop {r4, r5, r6, r7, r8, sl, pc}
0031aec4: bl #0x30e310
0031aec8: mlseq r7, r8, ip, sb
0031aecc: andeq r4, r0, ip, lsr #1
0031aed0: andeq r0, r0, r4, asr sb
0031aed4: subseq r3, sl, r8, lsr #20

_ZN10GameObject17_TargetListResortERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38fbf8 124
0038fbf8: push {r4, lr}
0038fbfc: ldr r3, [r0, #4]
0038fc00: sub sp, sp, #8
0038fc04: ldr r1, [r3, #4]
0038fc08: ldr ip, [r3]
0038fc0c: rsb r3, ip, r1
0038fc10: asr r3, r3, #4
0038fc14: add r1, r3, r3, lsl #3
0038fc18: add r1, r1, r1, lsl #6
0038fc1c: add r1, r3, r1, lsl #3
0038fc20: add r1, r1, r1, lsl #15
0038fc24: add r3, r3, r1, lsl #3
0038fc28: cmp r3, #0
0038fc2c: bne #0x38fc38
0038fc30: add sp, sp, #8
0038fc34: pop {r4, pc}
0038fc38: ldr r3, [ip, #4]
0038fc3c: cmp r3, #3
0038fc40: bne #0x38fc30
0038fc44: mov r1, #0
0038fc48: str r2, [sp, #4]
0038fc4c: bl #0x37baf8
0038fc50: ldr r2, [sp, #4]
0038fc54: add r4, r2, #0x304
0038fc58: bl #0x31bbf0
0038fc5c: bl #0x30e4cc
0038fc60: mov r1, r0
0038fc64: mov r0, r4
0038fc68: add sp, sp, #8
0038fc6c: pop {r4, lr}
0038fc70: b #0x4a35b4

_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE9push_backERKS3_ 0x3195c0 300
003195c0: push {r4, r5, r6, r7, r8, sb, sl, lr}
003195c4: mov r4, r0
003195c8: ldr r5, [r4, #8]
003195cc: ldr r0, [r0, #4]
003195d0: sub sp, sp, #8
003195d4: mov r6, r1
003195d8: cmp r0, r5
003195dc: beq #0x3195f8
003195e0: bl #0x31c634
003195e4: ldr r3, [r4, #4]
003195e8: add r3, r3, #0x70
003195ec: str r3, [r4, #4]
003195f0: add sp, sp, #8
003195f4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003195f8: ldr r2, [r4]
003195fc: movw r3, #0x2492
00319600: orr r3, r3, r3, lsl #12
00319604: rsb r2, r2, r5
00319608: asr r2, r2, #4
0031960c: add r1, r2, r2, lsl #3
00319610: add r1, r1, r1, lsl #6
00319614: add r1, r2, r1, lsl #3
00319618: add r1, r1, r1, lsl #15
0031961c: add r2, r2, r1, lsl #3
00319620: rsb r2, r2, #0
00319624: cmp r2, #1
00319628: addhs r1, r2, r2
0031962c: addlo r1, r2, #1
00319630: cmp r1, r3
00319634: bls #0x3196e0
00319638: movw r1, #0x2492
0031963c: orr r1, r1, r1, lsl #12
00319640: add r2, sp, #8
00319644: str r1, [r2, #-4]!
00319648: add r0, r4, #8
0031964c: bl #0x319538
00319650: ldr sl, [r4]
00319654: mov r8, r0
00319658: rsb r5, sl, r5
0031965c: asr r5, r5, #4
00319660: add sb, r5, r5, lsl #3
00319664: add sb, sb, sb, lsl #6
00319668: add sb, r5, sb, lsl #3
0031966c: add sb, sb, sb, lsl #15
00319670: add sb, r5, sb, lsl #3
00319674: rsb sb, sb, #0
00319678: cmp sb, #0
0031967c: movle sb, r0
00319680: ble #0x3196ac
00319684: mov r7, sb
00319688: mov r5, #0
0031968c: add r0, r8, r5
00319690: add r1, sl, r5
00319694: bl #0x31c634
00319698: subs r7, r7, #1
0031969c: add r5, r5, #0x70
003196a0: bne #0x31968c
003196a4: mov r3, #0x70
003196a8: mla sb, r3, sb, r8
003196ac: mov r1, r6
003196b0: mov r0, sb
003196b4: bl #0x31c634
003196b8: mov r0, r4
003196bc: bl #0x319324
003196c0: ldr r3, [sp, #4]
003196c4: mov r2, #0x70
003196c8: add sb, sb, #0x70
003196cc: mla r3, r2, r3, r8
003196d0: str r8, [r4]
003196d4: str r3, [r4, #8]
003196d8: str sb, [r4, #4]
003196dc: b #0x3195f0
003196e0: cmp r2, r1
003196e4: bls #0x319640
003196e8: b #0x319638

_ZN3sfc6script3lua12ReturnValuesC1Ev 0x31b434 60
0031b434: ldr r3, [pc, #0x2c]
0031b438: ldr r2, [pc, #0x2c]
0031b43c: push {r4, lr}
0031b440: add r3, pc, r3
0031b444: ldr r2, [r3, r2]
0031b448: mov r4, r0
0031b44c: add r2, r2, #8
0031b450: str r2, [r0], #4
0031b454: bl #0x31a804
0031b458: bl #0x31ce84
0031b45c: str r0, [r4, #0x24]
0031b460: mov r0, r4
0031b464: pop {r4, pc}
0031b468: rsbeq sb, r7, r0, asr r6
0031b46c: andeq r1, r0, r4, lsr #1

_ZN9Character24_GetEquippedFaeryElementERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6dc4 52
003b6dc4: push {r4, r5, r6, lr}
003b6dc8: mov r0, r2
003b6dcc: mov r5, r1
003b6dd0: mvn r1, #0
003b6dd4: mov r4, r2
003b6dd8: bl #0x3bb98c
003b6ddc: mov r1, r0
003b6de0: mov r0, r4
003b6de4: bl #0x3aeac0
003b6de8: ldr r1, [r0, #8]
003b6dec: mov r0, r5
003b6df0: pop {r4, r5, r6, lr}
003b6df4: b #0x37cb24

_ZN11TriggerTrap14createBindingsERN3sfc6script3lua6BinderE 0x39dd98 212
0039dd98: push {r4, r5, r6, r7, r8, lr}
0039dd9c: ldr r5, [pc, #0xac]
0039dda0: mov r4, r1
0039dda4: mov r7, r0
0039dda8: bl #0x38d7ec
0039ddac: ldr r3, [pc, #0xa0]
0039ddb0: add r5, pc, r5
0039ddb4: ldr r6, [pc, #0x9c]
0039ddb8: ldr r8, [r5, r3]
0039ddbc: mov r0, r4
0039ddc0: add r6, pc, r6
0039ddc4: mov r3, r7
0039ddc8: mov r1, r6
0039ddcc: mov r2, r8
0039ddd0: bl #0x31a4d4
0039ddd4: mov r0, r4
0039ddd8: mov r1, r6
0039dddc: mov r2, r8
0039dde0: bl #0x319af4
0039dde4: ldr r2, [pc, #0x70]
0039dde8: ldr r6, [pc, #0x70]
0039ddec: mov r3, r7
0039ddf0: ldr r8, [r5, r2]
0039ddf4: add r6, pc, r6
0039ddf8: mov r0, r4
0039ddfc: mov r1, r6
0039de00: mov r2, r8
0039de04: bl #0x31a4d4
0039de08: mov r0, r4
0039de0c: mov r1, r6
0039de10: mov r2, r8
0039de14: bl #0x319af4
0039de18: ldr r2, [pc, #0x44]
0039de1c: ldr r6, [pc, #0x44]
0039de20: mov r0, r4
0039de24: ldr r5, [r5, r2]
0039de28: add r6, pc, r6
0039de2c: mov r1, r6
0039de30: mov r2, r5
0039de34: mov r3, r7
0039de38: bl #0x31a4d4
0039de3c: mov r0, r4
0039de40: mov r1, r6
0039de44: mov r2, r5
0039de48: pop {r4, r5, r6, r7, r8, lr}
0039de4c: b #0x319af4
0039de50: subseq r6, pc, r0, ror #25
0039de54: andeq r4, r0, r0, ror fp
0039de58: subseq r5, r2, r0, asr #1
0039de5c: andeq r4, r0, r8, lsr #14

_ZN9LuaScript7_BitAndERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e9ec 472
0037e9ec: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037e9f0: ldr r4, [r0, #4]
0037e9f4: mov sl, r1
0037e9f8: sub sp, sp, #4
0037e9fc: ldm r4, {r2, r3}
0037ea00: mov r5, r0
0037ea04: rsb r3, r2, r3
0037ea08: asr r1, r3, #4
0037ea0c: add r8, r1, r1, lsl #3
0037ea10: add r8, r8, r8, lsl #6
0037ea14: add r8, r1, r8, lsl #3
0037ea18: add r8, r8, r8, lsl #15
0037ea1c: add r8, r1, r8, lsl #3
0037ea20: rsb r8, r8, #0
0037ea24: cmp r8, #1
0037ea28: bls #0x37ebb0
0037ea2c: ldr sb, [pc, #0x184]
0037ea30: mov r7, #0
0037ea34: mov r6, r7
0037ea38: add sb, pc, sb
0037ea3c: b #0x37ea4c
0037ea40: ldr r4, [r5, #4]
0037ea44: ldm r4, {r2, r3}
0037ea48: rsb r3, r2, r3
0037ea4c: asr r3, r3, #4
0037ea50: add r1, r3, r3, lsl #3
0037ea54: add r1, r1, r1, lsl #6
0037ea58: add r1, r3, r1, lsl #3
0037ea5c: add r1, r1, r1, lsl #15
0037ea60: add r3, r3, r1, lsl #3
0037ea64: rsb r3, r3, #0
0037ea68: cmp r6, r3
0037ea6c: add r6, r6, #1
0037ea70: blo #0x37ea80
0037ea74: mov r0, sb
0037ea78: bl #0x708eb0
0037ea7c: ldr r2, [r4]
0037ea80: add r2, r2, r7
0037ea84: ldr r3, [r2, #4]
0037ea88: add r7, r7, #0x70
0037ea8c: cmp r3, #3
0037ea90: bne #0x37ebb0
0037ea94: cmp r6, r8
0037ea98: bne #0x37ea40
0037ea9c: ldr r4, [r5, #4]
0037eaa0: ldm r4, {r0, r3}
0037eaa4: rsb r3, r0, r3
0037eaa8: asr r3, r3, #4
0037eaac: add r2, r3, r3, lsl #3
0037eab0: add r2, r2, r2, lsl #6
0037eab4: add r2, r3, r2, lsl #3
0037eab8: add r2, r2, r2, lsl #15
0037eabc: add r3, r3, r2, lsl #3
0037eac0: cmp r3, #0
0037eac4: bne #0x37ead8
0037eac8: ldr r0, [pc, #0xec]
0037eacc: add r0, pc, r0
0037ead0: bl #0x708eb0
0037ead4: ldr r0, [r4]
0037ead8: bl #0x31bbf0
0037eadc: bl #0x30e4cc
0037eae0: ldr r2, [r5, #4]
0037eae4: mov r8, r0
0037eae8: ldm r2, {r2, r3}
0037eaec: rsb r3, r2, r3
0037eaf0: asr r3, r3, #4
0037eaf4: add sb, r3, r3, lsl #3
0037eaf8: add sb, sb, sb, lsl #6
0037eafc: add sb, r3, sb, lsl #3
0037eb00: add sb, sb, sb, lsl #15
0037eb04: add sb, r3, sb, lsl #3
0037eb08: rsb sb, sb, #0
0037eb0c: cmp sb, #1
0037eb10: bls #0x37eb9c
0037eb14: ldr fp, [pc, #0xa4]
0037eb18: mov r7, #0x70
0037eb1c: mov r4, #1
0037eb20: add fp, pc, fp
0037eb24: add r0, r2, r7
0037eb28: bl #0x31bbf0
0037eb2c: bl #0x30e4cc
0037eb30: add r4, r4, #1
0037eb34: cmp r4, sb
0037eb38: and r8, r8, r0
0037eb3c: beq #0x37eb9c
0037eb40: ldr r6, [r5, #4]
0037eb44: mov r0, fp
0037eb48: add r7, r7, #0x70
0037eb4c: ldm r6, {r2, r3}
0037eb50: rsb r3, r2, r3
0037eb54: asr r3, r3, #4
0037eb58: add r1, r3, r3, lsl #3
0037eb5c: add r1, r1, r1, lsl #6
0037eb60: add r1, r3, r1, lsl #3
0037eb64: add r1, r1, r1, lsl #15
0037eb68: add r3, r3, r1, lsl #3
0037eb6c: rsb r3, r3, #0
0037eb70: cmp r4, r3
0037eb74: blo #0x37eb24
0037eb78: bl #0x708eb0
0037eb7c: ldr r2, [r6]
0037eb80: add r4, r4, #1
0037eb84: add r0, r2, r7
0037eb88: bl #0x31bbf0
0037eb8c: bl #0x30e4cc
0037eb90: cmp r4, sb
0037eb94: and r8, r8, r0
0037eb98: bne #0x37eb40
0037eb9c: mov r0, sl
0037eba0: mov r1, r8
0037eba4: add sp, sp, #4
0037eba8: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037ebac: b #0x37cb24
0037ebb0: add sp, sp, #4
0037ebb4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037ebb8: subseq pc, r3, r0, lsr sl

_ZN3sfc6script3lua12ReturnValuesD0Ev 0x31b3d8 28
0031b3d8: push {r4, lr}
0031b3dc: mov r4, r0
0031b3e0: bl #0x31b398
0031b3e4: mov r0, r4
0031b3e8: bl #0x310440
0031b3ec: mov r0, r4
0031b3f0: pop {r4, pc}

_ZN9LuaScript4CallEPKcRKN3sfc6script3lua9ArgumentsE 0x37c41c 120
0037c41c: ldr r3, [pc, #0x68]
0037c420: ldr ip, [pc, #0x68]
0037c424: push {r4, r5, r6, r7, r8, lr}
0037c428: add r3, pc, r3
0037c42c: ldr r5, [r3, ip]
0037c430: sub sp, sp, #0x30
0037c434: mov r8, r1
0037c438: ldr r1, [r5]
0037c43c: add r4, sp, #4
0037c440: mov r6, r0
0037c444: mov r7, r2
0037c448: mov r0, r4
0037c44c: str r1, [sp, #0x2c]
0037c450: bl #0x31b434
0037c454: mov r2, r7
0037c458: mov r3, r4
0037c45c: mov r0, r6
0037c460: mov r1, r8
0037c464: bl #0x37c390
0037c468: mov r0, r4
0037c46c: bl #0x31b398
0037c470: ldr r2, [sp, #0x2c]
0037c474: ldr r3, [r5]
0037c478: cmp r2, r3
0037c47c: bne #0x37c488
0037c480: add sp, sp, #0x30
0037c484: pop {r4, r5, r6, r7, r8, pc}
0037c488: bl #0x30e310
0037c48c: rsbeq r8, r1, r8, ror #12
0037c490: andeq r4, r0, ip, lsr #1

_ZN9Character10_GetMasterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6c70 12
003b6c70: mov r0, r1
003b6c74: ldr r1, [r2, #0x418]
003b6c78: b #0x37c9f8

_ZN3sfc6script3lua8InstanceC1EP9lua_State 0x31aa20 48
0031aa20: ldr r3, [pc, #0x20]
0031aa24: ldr ip, [pc, #0x20]
0031aa28: str r1, [r0, #4]
0031aa2c: add r3, pc, r3
0031aa30: ldr ip, [r3, ip]
0031aa34: mov r1, #0
0031aa38: strb r1, [r0, #8]
0031aa3c: add ip, ip, #8
0031aa40: str ip, [r0]
0031aa44: bx lr
0031aa48: rsbeq sl, r7, r4, rrx
0031aa4c: andeq r4, r0, r8, lsl r0

_ZNSt5dequeIPSt6vectorIN3sfc6script3lua5ValueESaIS4_EESaIS7_EED1Ev 0x31bef0 20
0031bef0: push {r4, lr}
0031bef4: mov r4, r0
0031bef8: bl #0x31be6c
0031befc: mov r0, r4
0031bf00: pop {r4, pc}

_ZN3sfc6script3lua9Arguments11pushPointerEPv 0x31a46c 104
0031a46c: ldr r3, [pc, #0x58]
0031a470: ldr r2, [pc, #0x58]
0031a474: push {r4, r5, r6, lr}
0031a478: add r3, pc, r3
0031a47c: ldr r5, [r3, r2]
0031a480: sub sp, sp, #0x78
0031a484: add r4, sp, #4
0031a488: ldr r3, [r5]
0031a48c: str r3, [sp, #0x74]
0031a490: ldr r6, [r0, #4]
0031a494: mov r0, r4
0031a498: bl #0x31a414
0031a49c: mov r0, r6
0031a4a0: mov r1, r4
0031a4a4: bl #0x3195c0
0031a4a8: mov r0, r4
0031a4ac: bl #0x3193e8
0031a4b0: ldr r2, [sp, #0x74]
0031a4b4: ldr r3, [r5]
0031a4b8: cmp r2, r3
0031a4bc: bne #0x31a4c8
0031a4c0: add sp, sp, #0x78
0031a4c4: pop {r4, r5, r6, pc}
0031a4c8: bl #0x30e310
0031a4cc: rsbeq sl, r7, r8, lsl r6
0031a4d0: andeq r4, r0, ip, lsr #1

_ZN3sfc6script3lua8InstanceC1Ev 0x31b268 64
0031b268: ldr r3, [pc, #0x30]
0031b26c: ldr r2, [pc, #0x30]
0031b270: mov r1, #1
0031b274: add r3, pc, r3
0031b278: ldr r2, [r3, r2]
0031b27c: push {r4, lr}
0031b280: add r2, r2, #8
0031b284: strb r1, [r0, #8]
0031b288: str r2, [r0]
0031b28c: mov r4, r0
0031b290: bl #0x31b224
0031b294: str r0, [r4, #4]
0031b298: mov r0, r4
0031b29c: pop {r4, pc}
0031b2a0: rsbeq sb, r7, ip, lsl r8
0031b2a4: andeq r4, r0, r8, lsl r0

_ZN9LuaScript9_MulFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e1a4 308
0037e1a4: push {r4, r5, r6, lr}
0037e1a8: ldr r5, [r0, #4]
0037e1ac: mov r6, r1
0037e1b0: mov r4, r0
0037e1b4: ldm r5, {r1, r3}
0037e1b8: rsb r3, r1, r3
0037e1bc: asr r3, r3, #4
0037e1c0: add r2, r3, r3, lsl #3
0037e1c4: add r2, r2, r2, lsl #6
0037e1c8: add r2, r3, r2, lsl #3
0037e1cc: add r2, r2, r2, lsl #15
0037e1d0: add r3, r3, r2, lsl #3
0037e1d4: rsb r3, r3, #0
0037e1d8: cmp r3, #1
0037e1dc: bls #0x37e2b8
0037e1e0: cmp r3, #0
0037e1e4: beq #0x37e2a4
0037e1e8: ldr r3, [r1, #4]
0037e1ec: cmp r3, #3
0037e1f0: beq #0x37e2bc
0037e1f4: ldr r5, [r4, #4]
0037e1f8: ldm r5, {r0, r3}
0037e1fc: rsb r3, r0, r3
0037e200: asr r3, r3, #4
0037e204: add r2, r3, r3, lsl #3
0037e208: add r2, r2, r2, lsl #6
0037e20c: add r2, r3, r2, lsl #3
0037e210: add r2, r2, r2, lsl #15
0037e214: add r3, r3, r2, lsl #3
0037e218: cmp r3, #0
0037e21c: beq #0x37e290
0037e220: bl #0x31bbf0
0037e224: bl #0x30e4cc
0037e228: ldr r4, [r4, #4]
0037e22c: mov r5, r0
0037e230: ldm r4, {r0, r3}
0037e234: rsb r3, r0, r3
0037e238: asr r3, r3, #4
0037e23c: add r2, r3, r3, lsl #3
0037e240: add r2, r2, r2, lsl #6
0037e244: add r2, r3, r2, lsl #3
0037e248: add r2, r2, r2, lsl #15
0037e24c: add r3, r3, r2, lsl #3
0037e250: rsb r3, r3, #0
0037e254: cmp r3, #1
0037e258: bls #0x37e27c
0037e25c: add r0, r0, #0x70
0037e260: bl #0x31bbf0
0037e264: bl #0x30e4cc
0037e268: mul r1, r5, r0
0037e26c: mov r0, r6
0037e270: asr r1, r1, #8
0037e274: pop {r4, r5, r6, lr}
0037e278: b #0x37cb24
0037e27c: ldr r0, [pc, #0x48]
0037e280: add r0, pc, r0
0037e284: bl #0x708eb0
0037e288: ldr r0, [r4]
0037e28c: b #0x37e25c
0037e290: ldr r0, [pc, #0x38]
0037e294: add r0, pc, r0
0037e298: bl #0x708eb0
0037e29c: ldr r0, [r5]
0037e2a0: b #0x37e220
0037e2a4: ldr r0, [pc, #0x28]
0037e2a8: add r0, pc, r0
0037e2ac: bl #0x708eb0
0037e2b0: ldr r1, [r5]
0037e2b4: b #0x37e1e8
0037e2b8: pop {r4, r5, r6, pc}
0037e2bc: mov r0, r4
0037e2c0: mov r1, #1
0037e2c4: bl #0x37baf8
0037e2c8: b #0x37e1f4
0037e2cc: subseq r0, r4, r8, ror #3
0037e2d0: ldrsbeq r0, [r4], #-0x14
0037e2d4: subseq r0, r4, r0, asr #3

_ZN9LuaScriptD0Ev 0x37c148 28
0037c148: push {r4, lr}
0037c14c: mov r4, r0
0037c150: bl #0x37c004
0037c154: mov r0, r4
0037c158: bl #0x310440
0037c15c: mov r0, r4
0037c160: pop {r4, pc}

_ZN3sfc6script3lua5ErrorC1EP9lua_Statei 0x31a918 104
0031a918: ldr ip, [pc, #0x58]
0031a91c: push {r4, r5, r6, lr}
0031a920: ldr lr, [pc, #0x54]
0031a924: add ip, pc, ip
0031a928: mov r3, r0
0031a92c: ldr lr, [ip, lr]
0031a930: mov r4, r0
0031a934: mov r5, r2
0031a938: add lr, lr, #8
0031a93c: str lr, [r3], #8
0031a940: mov r0, r3
0031a944: str r3, [r4, #0x18]
0031a948: str r3, [r4, #0x1c]
0031a94c: mov r6, r1
0031a950: bl #0x31a710
0031a954: ldr r3, [r4, #0x18]
0031a958: mov r2, #0
0031a95c: mov r0, r4
0031a960: strb r2, [r3]
0031a964: mov r1, r6
0031a968: mov r2, r5
0031a96c: bl #0x31a8ac
0031a970: mov r0, r4
0031a974: pop {r4, r5, r6, pc}
0031a978: rsbeq sl, r7, ip, ror #2
0031a97c: muleq r0, r8, r4

_ZN10GameObject14createBindingsERN3sfc6script3lua6BinderE 0x38d7ec 2648
0038d7ec: push {r4, r5, r6, r7, r8, lr}
0038d7f0: ldr r5, [pc, #0x8d0]
0038d7f4: mov r4, r1
0038d7f8: ldr r3, [pc, #0x8cc]
0038d7fc: ldr r1, [pc, #0x8cc]
0038d800: add r5, pc, r5
0038d804: mov r6, r0
0038d808: ldr r2, [r5, r3]
0038d80c: mov r0, r4
0038d810: add r1, pc, r1
0038d814: bl #0x319af4
0038d818: ldr r3, [pc, #0x8b4]
0038d81c: ldr r1, [pc, #0x8b4]
0038d820: mov r0, r4
0038d824: ldr r2, [r5, r3]
0038d828: add r1, pc, r1
0038d82c: bl #0x319af4
0038d830: ldr r2, [pc, #0x8a4]
0038d834: ldr r7, [pc, #0x8a4]
0038d838: mov r3, r6
0038d83c: ldr r8, [r5, r2]
0038d840: add r7, pc, r7
0038d844: mov r0, r4
0038d848: mov r1, r7
0038d84c: mov r2, r8
0038d850: bl #0x31a4d4
0038d854: mov r0, r4
0038d858: mov r1, r7
0038d85c: mov r2, r8
0038d860: bl #0x319af4
0038d864: ldr r2, [pc, #0x878]
0038d868: ldr r7, [pc, #0x878]
0038d86c: mov r3, r6
0038d870: ldr r8, [r5, r2]
0038d874: add r7, pc, r7
0038d878: mov r0, r4
0038d87c: mov r1, r7
0038d880: mov r2, r8
0038d884: bl #0x31a4d4
0038d888: mov r0, r4
0038d88c: mov r1, r7
0038d890: mov r2, r8
0038d894: bl #0x319af4
0038d898: ldr r3, [pc, #0x84c]
0038d89c: ldr r1, [pc, #0x84c]
0038d8a0: mov r0, r4
0038d8a4: ldr r2, [r5, r3]
0038d8a8: add r1, pc, r1
0038d8ac: mov r3, r6
0038d8b0: bl #0x31a4d4
0038d8b4: ldr r3, [pc, #0x838]
0038d8b8: ldr r1, [pc, #0x838]
0038d8bc: mov r0, r4
0038d8c0: ldr r2, [r5, r3]
0038d8c4: add r1, pc, r1
0038d8c8: mov r3, r6
0038d8cc: bl #0x31a4d4
0038d8d0: ldr r2, [pc, #0x824]
0038d8d4: ldr r7, [pc, #0x824]
0038d8d8: mov r3, r6
0038d8dc: ldr r8, [r5, r2]
0038d8e0: add r7, pc, r7
0038d8e4: mov r0, r4
0038d8e8: mov r1, r7
0038d8ec: mov r2, r8
0038d8f0: bl #0x31a4d4
0038d8f4: mov r0, r4
0038d8f8: mov r1, r7
0038d8fc: mov r2, r8
0038d900: bl #0x319af4
0038d904: ldr r2, [pc, #0x7f8]
0038d908: ldr r7, [pc, #0x7f8]
0038d90c: mov r3, r6
0038d910: ldr r8, [r5, r2]
0038d914: add r7, pc, r7
0038d918: mov r0, r4
0038d91c: mov r1, r7
0038d920: mov r2, r8
0038d924: bl #0x31a4d4
0038d928: mov r0, r4
0038d92c: mov r1, r7
0038d930: mov r2, r8
0038d934: bl #0x319af4
0038d938: ldr r2, [pc, #0x7cc]
0038d93c: ldr r7, [pc, #0x7cc]
0038d940: mov r3, r6
0038d944: ldr r8, [r5, r2]
0038d948: add r7, pc, r7
0038d94c: mov r0, r4
0038d950: mov r1, r7
0038d954: mov r2, r8
0038d958: bl #0x31a4d4
0038d95c: mov r0, r4
0038d960: mov r1, r7
0038d964: mov r2, r8
0038d968: bl #0x319af4
0038d96c: ldr r2, [pc, #0x7a0]
0038d970: ldr r7, [pc, #0x7a0]
0038d974: mov r3, r6
0038d978: ldr r8, [r5, r2]
0038d97c: add r7, pc, r7
0038d980: mov r0, r4
0038d984: mov r1, r7
0038d988: mov r2, r8
0038d98c: bl #0x31a4d4
0038d990: mov r0, r4
0038d994: mov r1, r7
0038d998: mov r2, r8
0038d99c: bl #0x319af4
0038d9a0: ldr r2, [pc, #0x774]
0038d9a4: ldr r7, [pc, #0x774]
0038d9a8: mov r3, r6
0038d9ac: ldr r8, [r5, r2]
0038d9b0: add r7, pc, r7
0038d9b4: mov r0, r4
0038d9b8: mov r1, r7
0038d9bc: mov r2, r8
0038d9c0: bl #0x31a4d4
0038d9c4: mov r0, r4
0038d9c8: mov r1, r7
0038d9cc: mov r2, r8
0038d9d0: bl #0x319af4
0038d9d4: ldr r2, [pc, #0x748]
0038d9d8: ldr r7, [pc, #0x748]
0038d9dc: mov r3, r6
0038d9e0: ldr r8, [r5, r2]
0038d9e4: add r7, pc, r7
0038d9e8: mov r0, r4
0038d9ec: mov r1, r7
0038d9f0: mov r2, r8
0038d9f4: bl #0x31a4d4
0038d9f8: mov r0, r4
0038d9fc: mov r1, r7
0038da00: mov r2, r8
0038da04: bl #0x319af4
0038da08: ldr r2, [pc, #0x71c]
0038da0c: ldr r7, [pc, #0x71c]
0038da10: mov r3, r6
0038da14: ldr r8, [r5, r2]
0038da18: add r7, pc, r7
0038da1c: mov r0, r4
0038da20: mov r1, r7
0038da24: mov r2, r8
0038da28: bl #0x31a4d4
0038da2c: mov r0, r4
0038da30: mov r1, r7
0038da34: mov r2, r8
0038da38: bl #0x319af4
0038da3c: ldr r2, [pc, #0x6f0]
0038da40: ldr r7, [pc, #0x6f0]
0038da44: mov r3, r6
0038da48: ldr r8, [r5, r2]
0038da4c: add r7, pc, r7
0038da50: mov r0, r4
0038da54: mov r1, r7
0038da58: mov r2, r8
0038da5c: bl #0x31a4d4
0038da60: mov r0, r4
0038da64: mov r1, r7
0038da68: mov r2, r8
0038da6c: bl #0x319af4
0038da70: ldr r3, [pc, #0x6c4]
0038da74: ldr r1, [pc, #0x6c4]
0038da78: mov r0, r4
0038da7c: ldr r2, [r5, r3]
0038da80: add r1, pc, r1
0038da84: mov r3, #0
0038da88: bl #0x31a4d4
0038da8c: ldr r2, [pc, #0x6b0]
0038da90: ldr r7, [pc, #0x6b0]
0038da94: mov r3, r6
0038da98: ldr r8, [r5, r2]
0038da9c: add r7, pc, r7
0038daa0: mov r0, r4
0038daa4: mov r1, r7
0038daa8: mov r2, r8
0038daac: bl #0x31a4d4
0038dab0: mov r0, r4
0038dab4: mov r1, r7
0038dab8: mov r2, r8
0038dabc: bl #0x319af4
0038dac0: ldr r2, [pc, #0x684]
0038dac4: ldr r7, [pc, #0x684]
0038dac8: mov r3, r6
0038dacc: ldr r8, [r5, r2]
0038dad0: add r7, pc, r7
0038dad4: mov r0, r4
0038dad8: mov r1, r7
0038dadc: mov r2, r8
0038dae0: bl #0x31a4d4
0038dae4: mov r0, r4
0038dae8: mov r1, r7
0038daec: mov r2, r8
0038daf0: bl #0x319af4
0038daf4: ldr r2, [pc, #0x658]
0038daf8: ldr r7, [pc, #0x658]
0038dafc: mov r3, r6
0038db00: ldr r8, [r5, r2]
0038db04: add r7, pc, r7
0038db08: mov r0, r4
0038db0c: mov r1, r7
0038db10: mov r2, r8
0038db14: bl #0x31a4d4
0038db18: mov r0, r4
0038db1c: mov r1, r7
0038db20: mov r2, r8
0038db24: bl #0x319af4
0038db28: ldr r2, [pc, #0x62c]
0038db2c: ldr r7, [pc, #0x62c]
0038db30: mov r3, r6
0038db34: ldr r8, [r5, r2]
0038db38: add r7, pc, r7
0038db3c: mov r0, r4
0038db40: mov r1, r7
0038db44: mov r2, r8
0038db48: bl #0x31a4d4
0038db4c: mov r0, r4
0038db50: mov r1, r7
0038db54: mov r2, r8
0038db58: bl #0x319af4
0038db5c: ldr r2, [pc, #0x600]
0038db60: ldr r7, [pc, #0x600]
0038db64: mov r3, r6
0038db68: ldr r8, [r5, r2]
0038db6c: add r7, pc, r7
0038db70: mov r0, r4
0038db74: mov r1, r7
0038db78: mov r2, r8
0038db7c: bl #0x31a4d4
0038db80: mov r0, r4
0038db84: mov r1, r7
0038db88: mov r2, r8
0038db8c: bl #0x319af4
0038db90: ldr r2, [pc, #0x5d4]
0038db94: ldr r7, [pc, #0x5d4]
0038db98: mov r3, r6
0038db9c: ldr r8, [r5, r2]
0038dba0: add r7, pc, r7
0038dba4: mov r0, r4
0038dba8: mov r1, r7
0038dbac: mov r2, r8
0038dbb0: bl #0x31a4d4
0038dbb4: mov r0, r4
0038dbb8: mov r1, r7
0038dbbc: mov r2, r8
0038dbc0: bl #0x319af4
0038dbc4: ldr r2, [pc, #0x5a8]
0038dbc8: ldr r7, [pc, #0x5a8]
0038dbcc: mov r3, r6
0038dbd0: ldr r8, [r5, r2]
0038dbd4: add r7, pc, r7
0038dbd8: mov r0, r4
0038dbdc: mov r1, r7
0038dbe0: mov r2, r8
0038dbe4: bl #0x31a4d4
0038dbe8: mov r0, r4
0038dbec: mov r1, r7
0038dbf0: mov r2, r8
0038dbf4: bl #0x319af4
0038dbf8: ldr r2, [pc, #0x57c]
0038dbfc: ldr r7, [pc, #0x57c]
0038dc00: mov r3, r6
0038dc04: ldr r8, [r5, r2]
0038dc08: add r7, pc, r7
0038dc0c: mov r0, r4
0038dc10: mov r1, r7
0038dc14: mov r2, r8
0038dc18: bl #0x31a4d4
0038dc1c: mov r0, r4
0038dc20: mov r1, r7
0038dc24: mov r2, r8
0038dc28: bl #0x319af4
0038dc2c: ldr r2, [pc, #0x550]
0038dc30: ldr r7, [pc, #0x550]
0038dc34: mov r3, r6
0038dc38: ldr r8, [r5, r2]
0038dc3c: add r7, pc, r7
0038dc40: mov r0, r4
0038dc44: mov r1, r7
0038dc48: mov r2, r8
0038dc4c: bl #0x31a4d4
0038dc50: mov r0, r4
0038dc54: mov r1, r7
0038dc58: mov r2, r8
0038dc5c: bl #0x319af4
0038dc60: ldr r2, [pc, #0x524]
0038dc64: ldr r7, [pc, #0x524]
0038dc68: mov r3, r6
0038dc6c: ldr r8, [r5, r2]
0038dc70: add r7, pc, r7
0038dc74: mov r0, r4
0038dc78: mov r1, r7
0038dc7c: mov r2, r8
0038dc80: bl #0x31a4d4
0038dc84: mov r0, r4
0038dc88: mov r1, r7
0038dc8c: mov r2, r8
0038dc90: bl #0x319af4
0038dc94: ldr r2, [pc, #0x4f8]
0038dc98: ldr r7, [pc, #0x4f8]
0038dc9c: mov r3, r6
0038dca0: ldr r8, [r5, r2]
0038dca4: add r7, pc, r7
0038dca8: mov r0, r4
0038dcac: mov r1, r7
0038dcb0: mov r2, r8
0038dcb4: bl #0x31a4d4
0038dcb8: mov r0, r4
0038dcbc: mov r1, r7
0038dcc0: mov r2, r8
0038dcc4: bl #0x319af4
0038dcc8: ldr r2, [pc, #0x4cc]
0038dccc: ldr r7, [pc, #0x4cc]
0038dcd0: mov r3, r6
0038dcd4: ldr r8, [r5, r2]
0038dcd8: add r7, pc, r7
0038dcdc: mov r0, r4
0038dce0: mov r1, r7
0038dce4: mov r2, r8
0038dce8: bl #0x31a4d4
0038dcec: mov r0, r4
0038dcf0: mov r1, r7
0038dcf4: mov r2, r8
0038dcf8: bl #0x319af4
0038dcfc: ldr r2, [pc, #0x4a0]
0038dd00: ldr r7, [pc, #0x4a0]
0038dd04: mov r3, r6
0038dd08: ldr r8, [r5, r2]
0038dd0c: add r7, pc, r7
0038dd10: mov r0, r4
0038dd14: mov r1, r7
0038dd18: mov r2, r8
0038dd1c: bl #0x31a4d4
0038dd20: mov r0, r4
0038dd24: mov r1, r7
0038dd28: mov r2, r8
0038dd2c: bl #0x319af4
0038dd30: ldr r2, [pc, #0x474]
0038dd34: ldr r7, [pc, #0x474]
0038dd38: mov r3, r6
0038dd3c: ldr r8, [r5, r2]
0038dd40: add r7, pc, r7
0038dd44: mov r0, r4
0038dd48: mov r1, r7
0038dd4c: mov r2, r8
0038dd50: bl #0x31a4d4
0038dd54: mov r0, r4
0038dd58: mov r1, r7
0038dd5c: mov r2, r8
0038dd60: bl #0x319af4
0038dd64: ldr r2, [pc, #0x448]
0038dd68: ldr r7, [pc, #0x448]
0038dd6c: mov r3, r6
0038dd70: ldr r8, [r5, r2]
0038dd74: add r7, pc, r7
0038dd78: mov r0, r4
0038dd7c: mov r1, r7
0038dd80: mov r2, r8
0038dd84: bl #0x31a4d4
0038dd88: mov r0, r4
0038dd8c: mov r1, r7
0038dd90: mov r2, r8
0038dd94: bl #0x319af4
0038dd98: ldr r2, [pc, #0x41c]
0038dd9c: ldr r7, [pc, #0x41c]
0038dda0: mov r3, r6
0038dda4: ldr r8, [r5, r2]
0038dda8: add r7, pc, r7
0038ddac: mov r0, r4
0038ddb0: mov r1, r7
0038ddb4: mov r2, r8
0038ddb8: bl #0x31a4d4
0038ddbc: mov r0, r4
0038ddc0: mov r1, r7
0038ddc4: mov r2, r8
0038ddc8: bl #0x319af4
0038ddcc: ldr r2, [pc, #0x3f0]
0038ddd0: ldr r7, [pc, #0x3f0]
0038ddd4: mov r3, r6
0038ddd8: ldr r8, [r5, r2]
0038dddc: add r7, pc, r7
0038dde0: mov r0, r4
0038dde4: mov r1, r7
0038dde8: mov r2, r8
0038ddec: bl #0x31a4d4
0038ddf0: mov r0, r4
0038ddf4: mov r1, r7
0038ddf8: mov r2, r8
0038ddfc: bl #0x319af4
0038de00: ldr r2, [pc, #0x3c4]
0038de04: ldr r7, [pc, #0x3c4]
0038de08: mov r3, r6
0038de0c: ldr r8, [r5, r2]
0038de10: add r7, pc, r7
0038de14: mov r0, r4
0038de18: mov r1, r7
0038de1c: mov r2, r8
0038de20: bl #0x31a4d4
0038de24: mov r0, r4
0038de28: mov r1, r7
0038de2c: mov r2, r8
0038de30: bl #0x319af4
0038de34: ldr r2, [pc, #0x398]
0038de38: ldr r7, [pc, #0x398]
0038de3c: mov r3, r6
0038de40: ldr r8, [r5, r2]
0038de44: add r7, pc, r7
0038de48: mov r0, r4
0038de4c: mov r1, r7
0038de50: mov r2, r8
0038de54: bl #0x31a4d4
0038de58: mov r0, r4
0038de5c: mov r1, r7
0038de60: mov r2, r8
0038de64: bl #0x319af4
0038de68: ldr r2, [pc, #0x36c]
0038de6c: ldr r7, [pc, #0x36c]
0038de70: mov r3, r6
0038de74: ldr r8, [r5, r2]
0038de78: add r7, pc, r7
0038de7c: mov r0, r4
0038de80: mov r1, r7
0038de84: mov r2, r8
0038de88: bl #0x31a4d4
0038de8c: mov r0, r4
0038de90: mov r1, r7
0038de94: mov r2, r8
0038de98: bl #0x319af4
0038de9c: ldr r2, [pc, #0x340]
0038dea0: ldr r7, [pc, #0x340]
0038dea4: mov r3, r6
0038dea8: ldr r8, [r5, r2]
0038deac: add r7, pc, r7
0038deb0: mov r0, r4
0038deb4: mov r1, r7
0038deb8: mov r2, r8
0038debc: bl #0x31a4d4
0038dec0: mov r0, r4
0038dec4: mov r1, r7
0038dec8: mov r2, r8
0038decc: bl #0x319af4
0038ded0: ldr r2, [pc, #0x314]
0038ded4: ldr r7, [pc, #0x314]
0038ded8: mov r3, r6
0038dedc: ldr r8, [r5, r2]
0038dee0: add r7, pc, r7
0038dee4: mov r0, r4
0038dee8: mov r1, r7
0038deec: mov r2, r8
0038def0: bl #0x31a4d4
0038def4: mov r0, r4
0038def8: mov r1, r7
0038defc: mov r2, r8
0038df00: bl #0x319af4
0038df04: ldr r3, [pc, #0x2e8]
0038df08: ldr r1, [pc, #0x2e8]
0038df0c: mov r0, r4
0038df10: ldr r2, [r5, r3]
0038df14: add r1, pc, r1
0038df18: mov r3, #0
0038df1c: bl #0x31a4d4
0038df20: ldr r3, [pc, #0x2d4]
0038df24: ldr r1, [pc, #0x2d4]
0038df28: mov r0, r4
0038df2c: ldr r2, [r5, r3]
0038df30: add r1, pc, r1
0038df34: mov r3, r6
0038df38: bl #0x31a4d4
0038df3c: ldr r2, [pc, #0x2c0]
0038df40: ldr r7, [pc, #0x2c0]
0038df44: mov r3, r6
0038df48: ldr r8, [r5, r2]
0038df4c: add r7, pc, r7
0038df50: mov r0, r4
0038df54: mov r1, r7
0038df58: mov r2, r8
0038df5c: bl #0x31a4d4
0038df60: mov r0, r4
0038df64: mov r1, r7
0038df68: mov r2, r8
0038df6c: bl #0x319af4
0038df70: ldr r3, [pc, #0x294]
0038df74: ldr r1, [pc, #0x294]
0038df78: mov r0, r4
0038df7c: ldr r2, [r5, r3]
0038df80: add r1, pc, r1
0038df84: mov r3, #0
0038df88: bl #0x31a4d4
0038df8c: ldr r2, [pc, #0x280]
0038df90: ldr r7, [pc, #0x280]
0038df94: mov r3, r6
0038df98: ldr r8, [r5, r2]
0038df9c: add r7, pc, r7
0038dfa0: mov r0, r4
0038dfa4: mov r1, r7
0038dfa8: mov r2, r8
0038dfac: bl #0x31a4d4
0038dfb0: mov r0, r4
0038dfb4: mov r1, r7
0038dfb8: mov r2, r8
0038dfbc: bl #0x319af4
0038dfc0: ldr r2, [pc, #0x254]
0038dfc4: ldr r7, [pc, #0x254]
0038dfc8: mov r3, r6
0038dfcc: ldr r8, [r5, r2]
0038dfd0: add r7, pc, r7
0038dfd4: mov r0, r4
0038dfd8: mov r1, r7
0038dfdc: mov r2, r8
0038dfe0: bl #0x31a4d4
0038dfe4: mov r0, r4
0038dfe8: mov r1, r7
0038dfec: mov r2, r8
0038dff0: bl #0x319af4
0038dff4: ldr r2, [pc, #0x228]
0038dff8: ldr r7, [pc, #0x228]
0038dffc: mov r3, r6
0038e000: ldr r8, [r5, r2]
0038e004: add r7, pc, r7
0038e008: mov r0, r4
0038e00c: mov r1, r7
0038e010: mov r2, r8
0038e014: bl #0x31a4d4
0038e018: mov r0, r4
0038e01c: mov r1, r7
0038e020: mov r2, r8
0038e024: bl #0x319af4
0038e028: ldr r2, [pc, #0x1fc]
0038e02c: ldr r7, [pc, #0x1fc]
0038e030: mov r3, r6
0038e034: ldr r8, [r5, r2]
0038e038: add r7, pc, r7
0038e03c: mov r0, r4
0038e040: mov r1, r7
0038e044: mov r2, r8
0038e048: bl #0x31a4d4
0038e04c: mov r0, r4
0038e050: mov r1, r7
0038e054: mov r2, r8
0038e058: bl #0x319af4
0038e05c: ldr r2, [pc, #0x1d0]
0038e060: ldr r7, [pc, #0x1d0]
0038e064: mov r3, r6
0038e068: ldr r8, [r5, r2]
0038e06c: add r7, pc, r7
0038e070: mov r0, r4
0038e074: mov r1, r7
0038e078: mov r2, r8
0038e07c: bl #0x31a4d4
0038e080: mov r0, r4
0038e084: mov r1, r7
0038e088: mov r2, r8
0038e08c: bl #0x319af4
0038e090: ldr r2, [pc, #0x1a4]
0038e094: ldr r7, [pc, #0x1a4]
0038e098: mov r0, r4
0038e09c: ldr r5, [r5, r2]
0038e0a0: add r7, pc, r7
0038e0a4: mov r1, r7
0038e0a8: mov r2, r5
0038e0ac: mov r3, r6
0038e0b0: bl #0x31a4d4
0038e0b4: mov r0, r4
0038e0b8: mov r1, r7
0038e0bc: mov r2, r5
0038e0c0: pop {r4, r5, r6, r7, r8, lr}
0038e0c4: b #0x319af4
0038e0c8: mlseq r0, r0, r2, r7
0038e0cc: andeq r1, r0, r8, lsr #3
0038e0d0: subseq r4, r3, r0, lsl sp
0038e0d4: andeq r1, r0, r0, lsl #18
0038e0d8: subseq r4, r3, r0, lsl #26
0038e0dc: strheq r0, [r0], -r8
0038e0e0: ldrsheq r4, [r3], #-0xc0
0038e0e4: muleq r0, r8, r6
0038e0e8: subseq r4, r3, r4, asr #25
0038e0ec: andeq r1, r0, r0, lsl r1

_ZN3sfc6script3lua12ReturnValues13_addFromStackEP9lua_Statei 0x31b4ac 208
0031b4ac: push {r4, r5, r6, r7, r8, sb, sl, lr}
0031b4b0: ldr r4, [pc, #0xb8]
0031b4b4: ldr r6, [pc, #0xb8]
0031b4b8: sub sp, sp, #0x78
0031b4bc: add r4, pc, r4
0031b4c0: ldr r3, [r4, r6]
0031b4c4: ldr r8, [r0, #0x24]
0031b4c8: add r5, sp, #4
0031b4cc: ldr r3, [r3]
0031b4d0: mov r7, r0
0031b4d4: mov r0, r5
0031b4d8: mov sl, r2
0031b4dc: str r3, [sp, #0x74]
0031b4e0: mov sb, r1
0031b4e4: bl #0x3194e0
0031b4e8: mov r1, r5
0031b4ec: mov r0, r8
0031b4f0: bl #0x3195c0
0031b4f4: mov r0, r5
0031b4f8: bl #0x3193e8
0031b4fc: ldr r5, [r7, #0x24]
0031b500: ldm r5, {r2, r3}
0031b504: rsb r3, r2, r3
0031b508: asr r3, r3, #4
0031b50c: add r7, r3, r3, lsl #3
0031b510: add r7, r7, r7, lsl #6
0031b514: add r7, r3, r7, lsl #3
0031b518: add r7, r7, r7, lsl #15
0031b51c: add r7, r3, r7, lsl #3
0031b520: rsb r7, r7, #0
0031b524: subs r7, r7, #1
0031b528: bhs #0x31b53c
0031b52c: ldr r0, [pc, #0x44]
0031b530: add r0, pc, r0
0031b534: bl #0x708eb0
0031b538: ldr r2, [r5]
0031b53c: mov r0, #0x70
0031b540: mla r0, r0, r7, r2
0031b544: mov r1, sb
0031b548: mov r2, sl
0031b54c: bl #0x31c9c8
0031b550: ldr r3, [r4, r6]
0031b554: ldr r2, [sp, #0x74]
0031b558: ldr r3, [r3]
0031b55c: cmp r2, r3
0031b560: bne #0x31b56c
0031b564: add sp, sp, #0x78
0031b568: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0031b56c: bl #0x30e310

_ZN3sfc6script3lua8Instance12_chunkReaderEP9lua_StatePvPj 0x31b018 360
0031b018: push {r4, r5, r6, r7, lr}
0031b01c: ldr r3, [r1, #4]
0031b020: mov r4, r1
0031b024: ldr r1, [pc, #0x13c]
0031b028: cmp r3, #0
0031b02c: sub sp, sp, #0xc
0031b030: mov r6, r2
0031b034: add r1, pc, r1
0031b038: bne #0x31b078
0031b03c: ldr r5, [r4, #8]
0031b040: cmp r5, #0
0031b044: bne #0x31b0e8
0031b048: ldr r3, [pc, #0x11c]
0031b04c: ldr r3, [r1, r3]
0031b050: ldr r3, [r3]
0031b054: cmp r3, #2
0031b058: streq r5, [r5]
0031b05c: moveq r0, r5
0031b060: beq #0x31b070
0031b064: cmp r3, #1
0031b068: beq #0x31b130
0031b06c: mov r0, #0
0031b070: add sp, sp, #0xc
0031b074: pop {r4, r5, r6, r7, pc}
0031b078: mov r0, r3
0031b07c: ldr r3, [r3]
0031b080: mov lr, pc
0031b084: ldr pc, [r3, #0x24]
0031b088: ldr r3, [r4, #4]
0031b08c: mov r5, r0
0031b090: mov r7, r1
0031b094: mov r0, r3
0031b098: ldr r3, [r3]
0031b09c: mov lr, pc
0031b0a0: ldr pc, [r3, #8]
0031b0a4: cmp r5, r0
0031b0a8: beq #0x31b0d8
0031b0ac: ldr r3, [r4, #4]
0031b0b0: mov r0, r3
0031b0b4: ldr ip, [r3]
0031b0b8: ldr r1, [r4, #0xc]
0031b0bc: ldr r2, [r4, #0x10]
0031b0c0: mov r3, #0
0031b0c4: mov lr, pc
0031b0c8: ldr pc, [ip, #0x18]
0031b0cc: str r0, [r6]
0031b0d0: ldr r0, [r4, #0xc]
0031b0d4: b #0x31b070
0031b0d8: cmp r7, r1
0031b0dc: bne #0x31b0ac
0031b0e0: mov r0, #0
0031b0e4: b #0x31b070
0031b0e8: ldr r3, [r5]
0031b0ec: mov r0, r5
0031b0f0: mov lr, pc
0031b0f4: ldr pc, [r3, #0x24]
0031b0f8: ldr r3, [r4, #8]
0031b0fc: mov r5, r0
0031b100: mov r7, r1
0031b104: mov r0, r3
0031b108: ldr r3, [r3]
0031b10c: mov lr, pc
0031b110: ldr pc, [r3, #8]
0031b114: cmp r5, r0
0031b118: ldrne r3, [r4, #8]
0031b11c: bne #0x31b0b0
0031b120: cmp r7, r1
0031b124: beq #0x31b06c
0031b128: ldr r3, [r4, #8]
0031b12c: b #0x31b0b0
0031b130: ldr r0, [pc, #0x38]
0031b134: ldr r2, [pc, #0x38]
0031b138: ldr r3, [pc, #0x38]
0031b13c: ldr r0, [r1, r0]
0031b140: ldr r1, [pc, #0x34]
0031b144: mov ip, #0x5e
0031b148: add r0, r0, #0xa8
0031b14c: add r1, pc, r1
0031b150: add r2, pc, r2
0031b154: add r3, pc, r3
0031b158: str ip, [sp]
0031b15c: bl #0x30e004
0031b160: mov r0, r5
0031b164: b #0x31b070
0031b168: rsbeq sb, r7, ip, asr sl
0031b16c: andeq r3, r0, r0, asr #19
0031b170: andeq r1, r0, r0, asr #19
0031b174: subseq r3, sl, r8, lsr #14
0031b178: subseq r3, sl, r4, asr #14
0031b17c: subseq r3, sl, ip, lsl #5

_ZN9LuaScript18_IsPlayerCharacterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ddc8 148
0037ddc8: ldr r3, [pc, #0x80]
0037ddcc: ldr r2, [pc, #0x80]
0037ddd0: push {r4, r5, r6, lr}
0037ddd4: add r3, pc, r3
0037ddd8: mov r5, r0
0037dddc: ldr r0, [r3, r2]
0037dde0: mov r4, r1
0037dde4: mov r2, #1
0037dde8: ldr r0, [r0, #0x40]
0037ddec: mov r1, #0
0037ddf0: bl #0x36e478
0037ddf4: ldr r5, [r5, #4]
0037ddf8: ldr r6, [r0, #0x660]
0037ddfc: ldm r5, {r0, r3}
0037de00: rsb r3, r0, r3
0037de04: asr r3, r3, #4
0037de08: add r2, r3, r3, lsl #3
0037de0c: add r2, r2, r2, lsl #6
0037de10: add r2, r3, r2, lsl #3
0037de14: add r2, r2, r2, lsl #15
0037de18: add r3, r3, r2, lsl #3
0037de1c: cmp r3, #0
0037de20: bne #0x37de34
0037de24: ldr r0, [pc, #0x2c]
0037de28: add r0, pc, r0
0037de2c: bl #0x708eb0
0037de30: ldr r0, [r5]
0037de34: bl #0x31b580
0037de38: cmp r6, r0
0037de3c: movne r1, #0
0037de40: moveq r1, #1
0037de44: mov r0, r4
0037de48: pop {r4, r5, r6, lr}
0037de4c: b #0x37c7e4
0037de50: strhteq r6, [r1], #-0xcc
0037de54: strdeq r3, r4, [r0], -r4
0037de58: subseq r0, r4, r0, asr #12

_ZN9Character13_RegisterAnimERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b70d8 108
003b70d8: push {r4, lr}
003b70dc: ldr r3, [r0, #4]
003b70e0: ldr r1, [r3, #4]
003b70e4: ldr ip, [r3]
003b70e8: rsb r3, ip, r1
003b70ec: asr r3, r3, #4
003b70f0: add r1, r3, r3, lsl #3
003b70f4: add r1, r1, r1, lsl #6
003b70f8: add r1, r3, r1, lsl #3
003b70fc: add r1, r1, r1, lsl #15
003b7100: add r3, r3, r1, lsl #3
003b7104: cmp r3, #0
003b7108: bne #0x3b7110
003b710c: pop {r4, pc}
003b7110: ldr r3, [ip, #4]
003b7114: cmp r3, #3
003b7118: bne #0x3b710c
003b711c: mov r1, #0
003b7120: add r4, r2, #0x490
003b7124: bl #0x37baf8
003b7128: bl #0x31bbf0
003b712c: bl #0x30e4cc
003b7130: add r4, r4, #0xc
003b7134: mov r1, r0
003b7138: mov r0, r4
003b713c: pop {r4, lr}
003b7140: b #0x3c9b14

_ZN3sfc6script3lua8UserDataD0Ev 0x33e2f0 52
0033e2f0: ldr r3, [pc, #0x24]
0033e2f4: ldr r2, [pc, #0x24]
0033e2f8: push {r4, lr}
0033e2fc: add r3, pc, r3
0033e300: ldr r2, [r3, r2]
0033e304: mov r4, r0
0033e308: add r2, r2, #8
0033e30c: str r2, [r0]
0033e310: bl #0x310440
0033e314: mov r0, r4
0033e318: pop {r4, pc}
0033e31c: mlseq r5, r4, r7, r6
0033e320: andeq r1, r0, ip, lsl #1

_ZN3sfc6script3lua8InstanceC2EP9lua_State 0x31a9f0 48
0031a9f0: ldr r3, [pc, #0x20]
0031a9f4: ldr ip, [pc, #0x20]
0031a9f8: str r1, [r0, #4]
0031a9fc: add r3, pc, r3
0031aa00: ldr ip, [r3, ip]
0031aa04: mov r1, #0
0031aa08: strb r1, [r0, #8]
0031aa0c: add ip, ip, #8
0031aa10: str ip, [r0]
0031aa14: bx lr
0031aa18: mlseq r7, r4, r0, sl
0031aa1c: andeq r4, r0, r8, lsl r0

_ZN9Character11_ClearAggroERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8648 96
003b8648: push {r4, lr}
003b864c: ldr r3, [r0, #4]
003b8650: ldm r3, {r0, r1}
003b8654: rsb r3, r0, r1
003b8658: asr r3, r3, #4
003b865c: add r1, r3, r3, lsl #3
003b8660: add r1, r1, r1, lsl #6
003b8664: add r1, r3, r1, lsl #3
003b8668: add r1, r1, r1, lsl #15
003b866c: add r3, r3, r1, lsl #3
003b8670: cmp r3, #0
003b8674: bne #0x3b867c
003b8678: pop {r4, pc}
003b867c: ldr r3, [r0, #4]
003b8680: cmp r3, #2
003b8684: beq #0x3b8690
003b8688: cmp r3, #7
003b868c: bne #0x3b8678
003b8690: add r4, r2, #0x3c8
003b8694: bl #0x31b5a0
003b8698: mov r1, r0
003b869c: mov r0, r4
003b86a0: pop {r4, lr}
003b86a4: b #0x3d6d68

_ZN9Character7_AttackERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b9f44 120
003b9f44: push {r4, lr}
003b9f48: ldr r3, [r0, #4]
003b9f4c: ldm r3, {r0, r1}
003b9f50: rsb r3, r0, r1
003b9f54: asr r3, r3, #4
003b9f58: add r1, r3, r3, lsl #3
003b9f5c: add r1, r1, r1, lsl #6
003b9f60: add r1, r3, r1, lsl #3
003b9f64: add r1, r1, r1, lsl #15
003b9f68: add r3, r3, r1, lsl #3
003b9f6c: cmp r3, #0
003b9f70: bne #0x3b9f8c
003b9f74: ldr r1, [r2, #0x408]
003b9f78: cmp r1, #0
003b9f7c: beq #0x3b9fa0
003b9f80: ldr r0, [r2, #0x378]
003b9f84: pop {r4, lr}
003b9f88: b #0x405b04
003b9f8c: ldr r3, [r0, #4]
003b9f90: cmp r3, #2
003b9f94: beq #0x3b9fa4
003b9f98: cmp r3, #7
003b9f9c: beq #0x3b9fa4
003b9fa0: pop {r4, pc}
003b9fa4: ldr r4, [r2, #0x378]
003b9fa8: bl #0x31b5a0
003b9fac: mov r1, r0
003b9fb0: mov r0, r4
003b9fb4: pop {r4, lr}
003b9fb8: b #0x405b04

_ZN9Character18_SetKnockBackStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7680 132
003b7680: str lr, [sp, #-4]!
003b7684: ldr r3, [r0, #4]
003b7688: sub sp, sp, #0xc
003b768c: ldr r1, [r3, #4]
003b7690: ldr ip, [r3]
003b7694: rsb r3, ip, r1
003b7698: asr r3, r3, #4
003b769c: add r1, r3, r3, lsl #3
003b76a0: add r1, r1, r1, lsl #6
003b76a4: add r1, r3, r1, lsl #3
003b76a8: add r1, r1, r1, lsl #15
003b76ac: add r3, r3, r1, lsl #3
003b76b0: cmp r3, #0
003b76b4: bne #0x3b76d8
003b76b8: mov r1, #1
003b76bc: add r0, r2, #0x4f0
003b76c0: mov r2, #0
003b76c4: add r0, r0, #0xc
003b76c8: mov r3, r2
003b76cc: add sp, sp, #0xc
003b76d0: pop {lr}
003b76d4: b #0x3c5ea0
003b76d8: ldr r3, [ip, #4]
003b76dc: cmp r3, #1
003b76e0: bne #0x3b76b8
003b76e4: mov r1, #0
003b76e8: str r2, [sp, #4]
003b76ec: bl #0x37baf8
003b76f0: bl #0x31bc80
003b76f4: subs r1, r0, #0
003b76f8: ldr r2, [sp, #4]
003b76fc: beq #0x3b76bc
003b7700: b #0x3b76b8

_ZNSt4priv10_List_baseISt6vectorIN3sfc6script3lua5ValueESaIS5_EESaIS7_EE5clearEv 0x31bf8c 76
0031bf8c: push {r4, r5, r6, lr}
0031bf90: ldr r6, [r0]
0031bf94: mov r5, r0
0031bf98: cmp r6, r0
0031bf9c: bne #0x31bfa8
0031bfa0: b #0x31bfcc
0031bfa4: mov r6, r4
0031bfa8: mov r0, r6
0031bfac: ldr r4, [r0], #8
0031bfb0: bl #0x31bf04
0031bfb4: mov r0, r6
0031bfb8: mov r1, #0x14
0031bfbc: bl #0x708f00
0031bfc0: cmp r4, r5
0031bfc4: bne #0x31bfa4
0031bfc8: mov r6, r5
0031bfcc: str r6, [r5, #4]
0031bfd0: str r6, [r5]
0031bfd4: pop {r4, r5, r6, pc}

_ZN9LuaScript12_PushVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37be00 68
0037be00: ldr r3, [r2, #0x5c]
0037be04: push {r4, r5, r6, lr}
0037be08: cmp r3, #0
0037be0c: mov r4, r2
0037be10: beq #0x37be38
0037be14: add r5, r2, #0x4c
0037be18: mov r0, r5
0037be1c: ldr r1, [r2, #0x50]
0037be20: bl #0x37bd7c
0037be24: mov r3, #0
0037be28: str r5, [r4, #0x58]
0037be2c: str r3, [r4, #0x5c]
0037be30: str r5, [r4, #0x54]
0037be34: str r3, [r4, #0x50]
0037be38: mov r3, #1
0037be3c: strb r3, [r4, #0x64]
0037be40: pop {r4, r5, r6, pc}

_ZN3sfc6script3lua5ErrorC2Ev 0x31a858 84
0031a858: ldr r2, [pc, #0x44]
0031a85c: ldr r1, [pc, #0x44]
0031a860: mov r3, r0
0031a864: add r2, pc, r2
0031a868: ldr r1, [r2, r1]
0031a86c: push {r4, lr}
0031a870: add r1, r1, #8
0031a874: mov r4, r0
0031a878: str r1, [r3], #8
0031a87c: mov r0, r3
0031a880: str r3, [r4, #0x18]
0031a884: str r3, [r4, #0x1c]
0031a888: bl #0x31a710
0031a88c: ldr r2, [r4, #0x18]
0031a890: mov r3, #0
0031a894: mov r0, r4
0031a898: strb r3, [r2]
0031a89c: str r3, [r4, #4]
0031a8a0: pop {r4, pc}
0031a8a4: rsbeq sl, r7, ip, lsr #4
0031a8a8: muleq r0, r8, r4

_ZN7TestUD25Test2ERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x386ad4 4
00386ad4: bx lr

_ZN3sfc6script3lua8Instance5pCallERKNS1_9ArgumentsERNS1_12ReturnValuesE 0x31ab4c 156
0031ab4c: push {r4, r5, r6, r7, r8, lr}
0031ab50: ldr r3, [r1, #4]
0031ab54: mov r4, r0
0031ab58: mov r8, r2
0031ab5c: ldm r3, {r0, r2}
0031ab60: mov r5, r1
0031ab64: rsb r3, r0, r2
0031ab68: asr r3, r3, #4
0031ab6c: add r2, r3, r3, lsl #3
0031ab70: add r2, r2, r2, lsl #6
0031ab74: add r2, r3, r2, lsl #3
0031ab78: add r2, r2, r2, lsl #15
0031ab7c: add r1, r3, r2, lsl #3
0031ab80: rsb r1, r1, #0
0031ab84: cmp r1, #0
0031ab88: beq #0x31abd8
0031ab8c: mov r6, #0
0031ab90: mov r7, r6
0031ab94: add r0, r0, r6
0031ab98: ldr r1, [r4, #4]
0031ab9c: bl #0x31cac4
0031aba0: ldr r2, [r5, #4]
0031aba4: add r7, r7, #1
0031aba8: add r6, r6, #0x70
0031abac: ldm r2, {r0, r3}
0031abb0: rsb r3, r0, r3
0031abb4: asr r3, r3, #4
0031abb8: add r1, r3, r3, lsl #3
0031abbc: add r1, r1, r1, lsl #6
0031abc0: add r1, r3, r1, lsl #3
0031abc4: add r1, r1, r1, lsl #15
0031abc8: add r1, r3, r1, lsl #3
0031abcc: rsb r1, r1, #0
0031abd0: cmp r7, r1
0031abd4: blo #0x31ab94
0031abd8: mov r0, r4
0031abdc: mov r2, r8
0031abe0: pop {r4, r5, r6, r7, r8, lr}
0031abe4: b #0x31aa78

_ZN9Character29_GetCurrentEquippedFaeryLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6df8 56
003b6df8: push {r4, r5, r6, lr}
003b6dfc: mov r0, r2
003b6e00: mov r4, r1
003b6e04: mvn r1, #0
003b6e08: mov r5, r2
003b6e0c: bl #0x3bb98c
003b6e10: mvn r2, #0
003b6e14: mov r1, r0
003b6e18: mov r0, r5
003b6e1c: bl #0x3bbc18
003b6e20: mov r1, r0
003b6e24: mov r0, r4
003b6e28: pop {r4, r5, r6, lr}
003b6e2c: b #0x37cb24

_ZN10GameObject12_PlaySound3DERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x39212c 672
0039212c: push {r4, r5, r6, r7, r8, sb, sl, lr}
00392130: ldr r4, [r0, #4]
00392134: mov r7, r0
00392138: mov sl, r2
0039213c: ldm r4, {r0, r3}
00392140: ldr r6, [pc, #0x264]
00392144: sub sp, sp, #0x28
00392148: rsb r3, r0, r3
0039214c: asr r3, r3, #4
00392150: add r6, pc, r6
00392154: add r2, r3, r3, lsl #3
00392158: add r2, r2, r2, lsl #6
0039215c: add r2, r3, r2, lsl #3
00392160: add r2, r2, r2, lsl #15
00392164: add r3, r3, r2, lsl #3
00392168: cmp r3, #0
0039216c: bne #0x392180
00392170: ldr r0, [pc, #0x238]
00392174: add r0, pc, r0
00392178: bl #0x708eb0
0039217c: ldr r0, [r4]
00392180: bl #0x31c49c
00392184: ldr r3, [pc, #0x228]
00392188: mov r5, r0
0039218c: ldr r3, [r6, r3]
00392190: ldr r4, [r3]
00392194: cmp r4, #0
00392198: beq #0x392260
0039219c: ldr r3, [pc, #0x214]
003921a0: mov r8, #0
003921a4: ldr r3, [r6, r3]
003921a8: ldr sb, [r3]
003921ac: b #0x3921bc
003921b0: add r8, r8, #1
003921b4: cmp r8, r4
003921b8: beq #0x392260
003921bc: ldr r1, [sb, r8, lsl #2]
003921c0: mov r0, r5
003921c4: bl #0x30e31c
003921c8: cmp r0, #0
003921cc: bne #0x3921b0
003921d0: ldr r2, [r7, #4]
003921d4: ldm r2, {r1, r3}
003921d8: rsb r3, r1, r3
003921dc: asr r3, r3, #4
003921e0: add r2, r3, r3, lsl #3
003921e4: add r2, r2, r2, lsl #6
003921e8: add r2, r3, r2, lsl #3
003921ec: add r2, r2, r2, lsl #15
003921f0: add r3, r3, r2, lsl #3
003921f4: rsb r3, r3, #0
003921f8: cmp r3, #3
003921fc: bls #0x392268
00392200: ldr r3, [r1, #0x74]
00392204: cmp r3, #3
00392208: beq #0x3922a4
0039220c: ldr lr, [sl, #0x168]
00392210: ldr r5, [sl, #0x160]
00392214: ldr r4, [sl, #0x164]
00392218: ldr r3, [pc, #0x19c]
0039221c: mov ip, #0xbf000000
00392220: add ip, ip, #0x800000
00392224: ldr r3, [r6, r3]
00392228: mov r1, r8
0039222c: add r2, sp, #0x1c
00392230: ldr r0, [r3]
00392234: str r5, [sp, #0x1c]
00392238: mov r3, #0
0039223c: str r4, [sp, #0x20]
00392240: str lr, [sp, #0x24]
00392244: mov lr, #1
00392248: str lr, [sp]
0039224c: str ip, [sp, #8]
00392250: str ip, [sp, #4]
00392254: bl #0x36b5d8
00392258: add sp, sp, #0x28
0039225c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00392260: mvn r8, #0
00392264: b #0x3921d0
00392268: ldr r3, [pc, #0x14c]
0039226c: ldr r4, [sl, #0x160]
00392270: ldr r5, [sl, #0x164]
00392274: ldr lr, [sl, #0x168]
00392278: ldr r3, [r6, r3]
0039227c: mov ip, #0xbf000000
00392280: add ip, ip, #0x800000
00392284: ldr r0, [r3]
00392288: mov r1, r8
0039228c: add r2, sp, #0x10
00392290: mov r3, #0
00392294: str r4, [sp, #0x10]
00392298: str r5, [sp, #0x14]
0039229c: str lr, [sp, #0x18]
003922a0: b #0x392244
003922a4: mov r1, #2
003922a8: mov r0, r7
003922ac: bl #0x37baf8
003922b0: ldr r1, [r0, #4]
003922b4: cmp r1, #3
003922b8: bne #0x39220c
003922bc: mov r0, r7
003922c0: bl #0x37baf8
003922c4: ldr r3, [r0, #4]
003922c8: cmp r3, #3
003922cc: bne #0x39220c
003922d0: ldr r4, [r7, #4]
003922d4: movw r3, #0x6db7
003922d8: movt r3, #0xb6db
003922dc: ldm r4, {r0, r2}
003922e0: rsb r2, r0, r2
003922e4: asr r2, r2, #4
003922e8: mul r3, r3, r2
003922ec: cmp r3, #1
003922f0: bhi #0x392304
003922f4: ldr r0, [pc, #0xc4]
003922f8: add r0, pc, r0
003922fc: bl #0x708eb0
00392300: ldr r0, [r4]
00392304: add r0, r0, #0x70
00392308: bl #0x31bbf0
0039230c: ldr r4, [r7, #4]
00392310: mov r5, r0
00392314: ldm r4, {r0, r3}
00392318: rsb r3, r0, r3
0039231c: asr r3, r3, #4
00392320: add r2, r3, r3, lsl #3
00392324: add r2, r2, r2, lsl #6
00392328: add r2, r3, r2, lsl #3
0039232c: add r2, r2, r2, lsl #15
00392330: add r3, r3, r2, lsl #3
00392334: rsb r3, r3, #0
00392338: cmp r3, #2
0039233c: bhi #0x392350
00392340: ldr r0, [pc, #0x7c]
00392344: add r0, pc, r0
00392348: bl #0x708eb0
0039234c: ldr r0, [r4]
00392350: add r0, r0, #0xe0
00392354: bl #0x31bbf0
00392358: ldr r7, [r7, #4]
0039235c: mov r4, r0
00392360: ldm r7, {r0, r3}
00392364: rsb r3, r0, r3
00392368: asr r3, r3, #4
0039236c: add r2, r3, r3, lsl #3
00392370: add r2, r2, r2, lsl #6
00392374: add r2, r3, r2, lsl #3
00392378: add r2, r2, r2, lsl #15
0039237c: add r3, r3, r2, lsl #3
00392380: rsb r3, r3, #0
00392384: cmp r3, #3
00392388: bhi #0x39239c
0039238c: ldr r0, [pc, #0x34]
00392390: add r0, pc, r0
00392394: bl #0x708eb0
00392398: ldr r0, [r7]
0039239c: add r0, r0, #0x150
003923a0: bl #0x31bbf0
003923a4: mov lr, r0
003923a8: b #0x392218
003923ac: rsbeq r2, r0, r0, asr #18
003923b0: ldrsheq ip, [r2], #-0x24
003923b4: andeq r3, r0, r8, lsr sp
003923b8: andeq r3, r0, r8, lsr #19
003923bc: andeq r0, r0, r4, lsr #27
003923c0: subseq ip, r2, r0, ror r1
003923c4: subseq ip, r2, r4, lsr #2
003923c8: ldrsbeq ip, [r2], #-8

_ZN3sfc6script3lua6BinderD0Ev 0x31bb54 52
0031bb54: ldr r3, [pc, #0x24]
0031bb58: ldr r2, [pc, #0x24]
0031bb5c: push {r4, lr}
0031bb60: add r3, pc, r3
0031bb64: ldr r2, [r3, r2]
0031bb68: mov r4, r0
0031bb6c: add r2, r2, #8
0031bb70: str r2, [r0]
0031bb74: bl #0x310440
0031bb78: mov r0, r4
0031bb7c: pop {r4, pc}
0031bb80: rsbeq r8, r7, r0, lsr pc
0031bb84: andeq r3, r0, r8, asr r6

_ZN9Character16_AllowSkillBreakERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7974 120
003b7974: str lr, [sp, #-4]!
003b7978: ldr r3, [r0, #4]
003b797c: sub sp, sp, #0xc
003b7980: ldr r1, [r3, #4]
003b7984: ldr ip, [r3]
003b7988: rsb r3, ip, r1
003b798c: asr r3, r3, #4
003b7990: add r1, r3, r3, lsl #3
003b7994: add r1, r1, r1, lsl #6
003b7998: add r1, r3, r1, lsl #3
003b799c: add r1, r1, r1, lsl #15
003b79a0: add r3, r3, r1, lsl #3
003b79a4: cmp r3, #0
003b79a8: bne #0x3b79b4
003b79ac: add sp, sp, #0xc
003b79b0: ldm sp!, {pc}
003b79b4: ldr r3, [ip, #4]
003b79b8: cmp r3, #1
003b79bc: bne #0x3b79ac
003b79c0: mov r1, #0
003b79c4: str r2, [sp, #4]
003b79c8: bl #0x37baf8
003b79cc: bl #0x31bc80
003b79d0: ldr r2, [sp, #4]
003b79d4: cmp r0, #0
003b79d8: ldr r3, [r2, #0x520]
003b79dc: bicne r3, r3, #0x10000
003b79e0: orreq r3, r3, #0x10000
003b79e4: str r3, [r2, #0x520]
003b79e8: b #0x3b79ac

_ZN9Character12_ClearTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b5690 36
003b5690: push {r4, lr}
003b5694: mov r1, #0
003b5698: add r4, r2, #0x3c8
003b569c: mov r0, r4
003b56a0: mov r2, r1
003b56a4: bl #0x3d6890
003b56a8: mov r0, r4
003b56ac: pop {r4, lr}
003b56b0: b #0x3d49c4

_ZN3sfc6script3lua5Value10setPointerEPv 0x31b5f8 16
0031b5f8: mov r3, #2
0031b5fc: str r1, [r0, #0x6c]
0031b600: str r3, [r0, #4]
0031b604: bx lr

_ZN10GameObject5_RandERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x393310 308
00393310: push {r4, r5, r6, r7, r8, lr}
00393314: ldr r3, [r0, #4]
00393318: mov r6, r1
0039331c: mov r7, r2
00393320: ldr r1, [r3, #4]
00393324: ldr r3, [r3]
00393328: ldr r4, [pc, #0x10c]
0039332c: mov r5, r0
00393330: rsb r1, r3, r1
00393334: asr r1, r1, #4
00393338: add r4, pc, r4
0039333c: add r2, r1, r1, lsl #3
00393340: add r2, r2, r2, lsl #6
00393344: add r2, r1, r2, lsl #3
00393348: add r2, r2, r2, lsl #15
0039334c: add r1, r1, r2, lsl #3
00393350: rsb r1, r1, #0
00393354: cmp r1, #1
00393358: beq #0x3933c8
0039335c: cmp r1, #2
00393360: movne r5, #0x64
00393364: movne r8, #0
00393368: beq #0x3933d8
0039336c: bl #0x7fd794
00393370: bl #0x7fd794
00393374: ldrb r3, [r0, #5]
00393378: cmp r3, #0
0039337c: bne #0x393398
00393380: mov r0, r5
00393384: bl #0x38e578
00393388: add r1, r0, r8
0039338c: mov r0, r6
00393390: pop {r4, r5, r6, r7, r8, lr}
00393394: b #0x37cb24
00393398: ldr r2, [pc, #0xa0]
0039339c: ldr r3, [r7, #0xfc]
003933a0: mov r0, r5
003933a4: ldr r4, [r4, r2]
003933a8: str r3, [r4]
003933ac: bl #0x38e578
003933b0: add r1, r0, r8
003933b4: mov r0, r6
003933b8: bl #0x37cb24
003933bc: ldr r3, [r4]
003933c0: str r3, [r7, #0xfc]
003933c4: pop {r4, r5, r6, r7, r8, pc}
003933c8: ldr r3, [r3, #4]
003933cc: cmp r3, #3
003933d0: beq #0x393424
003933d4: pop {r4, r5, r6, r7, r8, pc}
003933d8: ldr r3, [r3, #4]
003933dc: cmp r3, #3
003933e0: bne #0x3933d4
003933e4: mov r1, #1
003933e8: bl #0x37baf8
003933ec: ldr r3, [r0, #4]
003933f0: cmp r3, #3
003933f4: bne #0x3933d4
003933f8: mov r1, #0
003933fc: mov r0, r5
00393400: bl #0x37baf8
00393404: bl #0x38d798
00393408: mov r1, #1
0039340c: mov r8, r0
00393410: mov r0, r5
00393414: bl #0x37baf8
00393418: bl #0x38d798
0039341c: rsb r5, r8, r0
00393420: b #0x39336c
00393424: mov r1, #0
00393428: bl #0x37baf8
0039342c: bl #0x38d798
00393430: mov r8, #0
00393434: mov r5, r0
00393438: b #0x39336c
0039343c: rsbeq r1, r0, r8, asr r7
00393440: muleq r0, r4, ip

_ZN9LuaScript7_GetIntERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ec14 92
0037ec14: push {r4, r5, r6, lr}
0037ec18: ldr r3, [r0, #4]
0037ec1c: mov r5, r2
0037ec20: mov r4, r1
0037ec24: ldm r3, {r0, r2}
0037ec28: rsb r3, r0, r2
0037ec2c: asr r3, r3, #4
0037ec30: add r2, r3, r3, lsl #3
0037ec34: add r2, r2, r2, lsl #6
0037ec38: add r2, r3, r2, lsl #3
0037ec3c: add r2, r2, r2, lsl #15
0037ec40: add r3, r3, r2, lsl #3
0037ec44: cmp r3, #0
0037ec48: bne #0x37ec50
0037ec4c: pop {r4, r5, r6, pc}
0037ec50: bl #0x31c49c
0037ec54: mov r1, r0
0037ec58: mov r0, r5
0037ec5c: bl #0x37da30
0037ec60: mov r1, r0
0037ec64: mov r0, r4
0037ec68: pop {r4, r5, r6, lr}
0037ec6c: b #0x37cb24

_ZN3sfc6script3lua5Value9setNumberEf 0x31b5e8 16
0031b5e8: mov r3, #3
0031b5ec: str r1, [r0, #8]
0031b5f0: str r3, [r0, #4]
0031b5f4: bx lr

_ZN9Character10_SetMasterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b9084 96
003b9084: push {r4, lr}
003b9088: ldr r3, [r0, #4]
003b908c: ldm r3, {r0, r1}
003b9090: rsb r3, r0, r1
003b9094: asr r3, r3, #4
003b9098: add r1, r3, r3, lsl #3
003b909c: add r1, r1, r1, lsl #6
003b90a0: add r1, r3, r1, lsl #3
003b90a4: add r1, r1, r1, lsl #15
003b90a8: add r3, r3, r1, lsl #3
003b90ac: cmp r3, #0
003b90b0: bne #0x3b90b8
003b90b4: pop {r4, pc}
003b90b8: ldr r3, [r0, #4]
003b90bc: cmp r3, #2
003b90c0: beq #0x3b90cc
003b90c4: cmp r3, #7
003b90c8: bne #0x3b90b4
003b90cc: add r4, r2, #0x3c8
003b90d0: bl #0x31b5a0
003b90d4: mov r1, r0
003b90d8: mov r0, r4
003b90dc: pop {r4, lr}
003b90e0: b #0x3d4d80

_ZN9Character13_GetStateTimeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6d6c 12
003b6d6c: mov r0, r1
003b6d70: ldr r1, [r2, #0x55c]
003b6d74: b #0x37cb24

_ZN10GameObject6_GetIDERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ebe4 12
0038ebe4: mov r0, r1
0038ebe8: mov r1, r2
0038ebec: b #0x38eb00

_ZNSt4priv6__copyINS_15_Deque_iteratorIPSt6vectorIN3sfc6script3lua5ValueESaIS6_EESt16_Nonconst_traitsIS9_EEESC_iEET0_T_SE_SD_RKSt26random_access_iterator_tagPT1_ 0x31b9d8 248
0031b9d8: push {r4, r5, r6, lr}
0031b9dc: sub sp, sp, #0x10
0031b9e0: mov lr, r2
0031b9e4: mov ip, sp
0031b9e8: mov r5, r1
0031b9ec: mov r6, r0
0031b9f0: mov r4, r3
0031b9f4: ldm r1, {r0, r1, r2, r3}
0031b9f8: stm ip, {r0, r1, r2, r3}
0031b9fc: mov r0, lr
0031ba00: mov r1, sp
0031ba04: bl #0x31b618
0031ba08: cmp r0, #0
0031ba0c: bgt #0x31ba34
0031ba10: b #0x31babc
0031ba14: ldr r3, [r4]
0031ba18: ldr r2, [r4, #8]
0031ba1c: add r3, r3, #4
0031ba20: cmp r3, r2
0031ba24: str r3, [r4]
0031ba28: beq #0x31ba94
0031ba2c: subs r0, r0, #1
0031ba30: beq #0x31babc
0031ba34: ldr r2, [r5]
0031ba38: ldr r3, [r4]
0031ba3c: ldr r2, [r2]
0031ba40: str r2, [r3]
0031ba44: ldr r3, [r5]
0031ba48: ldr r2, [r5, #8]
0031ba4c: add r3, r3, #4
0031ba50: cmp r3, r2
0031ba54: str r3, [r5]
0031ba58: bne #0x31ba14
0031ba5c: ldr r3, [r5, #0xc]
0031ba60: add r2, r3, #4
0031ba64: str r2, [r5, #0xc]
0031ba68: ldr r3, [r3, #4]
0031ba6c: add r2, r3, #0x80
0031ba70: str r2, [r5, #8]
0031ba74: str r3, [r5]
0031ba78: str r3, [r5, #4]
0031ba7c: ldr r3, [r4]
0031ba80: ldr r2, [r4, #8]
0031ba84: add r3, r3, #4
0031ba88: cmp r3, r2
0031ba8c: str r3, [r4]
0031ba90: bne #0x31ba2c
0031ba94: ldr r3, [r4, #0xc]
0031ba98: subs r0, r0, #1
0031ba9c: add r2, r3, #4
0031baa0: str r2, [r4, #0xc]
0031baa4: ldr r3, [r3, #4]
0031baa8: add r2, r3, #0x80
0031baac: str r2, [r4, #8]
0031bab0: str r3, [r4]
0031bab4: str r3, [r4, #4]
0031bab8: bne #0x31ba34
0031babc: ldm r4, {r0, r1, r2, r3}
0031bac0: stm r6, {r0, r1, r2, r3}
0031bac4: mov r0, r6
0031bac8: add sp, sp, #0x10
0031bacc: pop {r4, r5, r6, pc}

_ZN3sfc6script3lua5ValueC1Ei 0x37ca9c 136
0037ca9c: ldr r2, [pc, #0x78]
0037caa0: ldr ip, [pc, #0x78]
0037caa4: mov r3, r0
0037caa8: add r2, pc, r2
0037caac: ldr ip, [r2, ip]
0037cab0: push {r4, r5, r6, lr}
0037cab4: add ip, ip, #8
0037cab8: mov r4, r0
0037cabc: str ip, [r3], #0xc
0037cac0: mov r5, r1
0037cac4: mov r0, r3
0037cac8: mov r1, #0x10
0037cacc: str r3, [r4, #0x1c]
0037cad0: str r3, [r4, #0x20]
0037cad4: bl #0x31167c
0037cad8: ldr r2, [r4, #0x1c]
0037cadc: add r3, r4, #0x24
0037cae0: mov r6, #0
0037cae4: strb r6, [r2]
0037cae8: mov r0, r3
0037caec: str r3, [r4, #0x64]
0037caf0: str r3, [r4, #0x68]
0037caf4: bl #0x37be44
0037caf8: ldr r3, [r4, #0x64]
0037cafc: mov r0, r5
0037cb00: str r6, [r3]
0037cb04: bl #0x30e964
0037cb08: mov r1, r0
0037cb0c: mov r0, r4
0037cb10: bl #0x31b5e8
0037cb14: mov r0, r4
0037cb18: pop {r4, r5, r6, pc}
0037cb1c: rsbeq r7, r1, r8, ror #31
0037cb20: muleq r0, r8, r7

_ZN9Character19_TargetInMeleeRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7020 36
003b7020: push {r4, lr}
003b7024: add r0, r2, #0x3c8
003b7028: mov r4, r1
003b702c: ldr r1, [r2, #0x408]
003b7030: bl #0x3d6188
003b7034: mov r1, r0
003b7038: mov r0, r4
003b703c: pop {r4, lr}
003b7040: b #0x37c7e4

_ZN10GameObject10_IsInWaterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38eab4 32
0038eab4: push {r4, lr}
0038eab8: add r0, r2, #0x1c8
0038eabc: mov r4, r1
0038eac0: bl #0x5241d4
0038eac4: mov r1, r0
0038eac8: mov r0, r4
0038eacc: pop {r4, lr}
0038ead0: b #0x37c7e4

_ZN10GameObject11_IsSwimmingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ea74 32
0038ea74: push {r4, lr}
0038ea78: add r0, r2, #0x1c8
0038ea7c: mov r4, r1
0038ea80: bl #0x52420c
0038ea84: mov r1, r0
0038ea88: mov r0, r4
0038ea8c: pop {r4, lr}
0038ea90: b #0x37c7e4

_ZN10GameObject8_GetSelfERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38eaf4 12
0038eaf4: mov r0, r1
0038eaf8: mov r1, r2
0038eafc: b #0x37c9f8

_ZN3sfc6script3lua9Arguments12pushUserDataEPNS1_8UserDataE 0x386f28 104
00386f28: ldr r3, [pc, #0x58]
00386f2c: ldr r2, [pc, #0x58]
00386f30: push {r4, r5, r6, lr}
00386f34: add r3, pc, r3
00386f38: ldr r5, [r3, r2]
00386f3c: sub sp, sp, #0x78
00386f40: add r4, sp, #4
00386f44: ldr r3, [r5]
00386f48: str r3, [sp, #0x74]
00386f4c: ldr r6, [r0, #4]
00386f50: mov r0, r4
00386f54: bl #0x37c978
00386f58: mov r0, r6
00386f5c: mov r1, r4
00386f60: bl #0x3195c0
00386f64: mov r0, r4
00386f68: bl #0x3193e8
00386f6c: ldr r2, [sp, #0x74]
00386f70: ldr r3, [r5]
00386f74: cmp r2, r3
00386f78: bne #0x386f84
00386f7c: add sp, sp, #0x78
00386f80: pop {r4, r5, r6, pc}
00386f84: bl #0x30e310
00386f88: rsbeq sp, r0, ip, asr fp
00386f8c: andeq r4, r0, ip, lsr #1

_ZN10GameObject16_SummonTimerTrapERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390c50 716
00390c50: push {r4, r5, r6, r7, r8, lr}
00390c54: ldr r5, [r0, #4]
00390c58: mov r7, r1
00390c5c: mov r8, r2
00390c60: ldr r3, [r5]
00390c64: ldr r1, [r5, #4]
00390c68: ldr r4, [pc, #0x298]
00390c6c: sub sp, sp, #0x10
00390c70: rsb r1, r3, r1
00390c74: asr r1, r1, #4
00390c78: add r4, pc, r4
00390c7c: add r2, r1, r1, lsl #3
00390c80: mov r6, r0
00390c84: add r2, r2, r2, lsl #6
00390c88: add r2, r1, r2, lsl #3
00390c8c: add r2, r2, r2, lsl #15
00390c90: add r1, r1, r2, lsl #3
00390c94: rsb r1, r1, #0
00390c98: cmp r1, #1
00390c9c: bls #0x390cb4
00390ca0: cmp r1, #0
00390ca4: beq #0x390cbc
00390ca8: ldr r3, [r3, #4]
00390cac: cmp r3, #3
00390cb0: beq #0x390cd0
00390cb4: add sp, sp, #0x10
00390cb8: pop {r4, r5, r6, r7, r8, pc}
00390cbc: ldr r0, [pc, #0x248]
00390cc0: add r0, pc, r0
00390cc4: bl #0x708eb0
00390cc8: ldr r3, [r5]
00390ccc: b #0x390ca8
00390cd0: mov r1, #0
00390cd4: mov r0, r6
00390cd8: bl #0x37baf8
00390cdc: bl #0x38d798
00390ce0: ldr r3, [pc, #0x228]
00390ce4: ldr r3, [r4, r3]
00390ce8: ldr r3, [r3]
00390cec: cmp r0, r3
00390cf0: bhs #0x390cb4
00390cf4: ldr r5, [r6, #4]
00390cf8: ldr r3, [r5]
00390cfc: ldr r2, [r5, #4]
00390d00: rsb r2, r3, r2
00390d04: asr r2, r2, #4
00390d08: add r1, r2, r2, lsl #3
00390d0c: add r1, r1, r1, lsl #6
00390d10: add r1, r2, r1, lsl #3
00390d14: add r1, r1, r1, lsl #15
00390d18: add r2, r2, r1, lsl #3
00390d1c: rsb r2, r2, #0
00390d20: cmp r2, #1
00390d24: bhi #0x390d38
00390d28: ldr r0, [pc, #0x1e4]
00390d2c: add r0, pc, r0
00390d30: bl #0x708eb0
00390d34: ldr r3, [r5]
00390d38: ldr r3, [r3, #0x74]
00390d3c: cmp r3, #3
00390d40: bne #0x390cb4
00390d44: mov r1, #1
00390d48: mov r0, r6
00390d4c: bl #0x37baf8
00390d50: bl #0x38d798
00390d54: ldr r3, [pc, #0x1bc]
00390d58: ldr r3, [r4, r3]
00390d5c: ldr r3, [r3]
00390d60: cmp r0, r3
00390d64: bhs #0x390cb4
00390d68: mov r1, #0
00390d6c: mov r0, r6
00390d70: bl #0x37baf8
00390d74: bl #0x31bbf0
00390d78: mov r1, #1
00390d7c: mov r4, r0
00390d80: mov r0, r6
00390d84: bl #0x37baf8
00390d88: bl #0x31bbf0
00390d8c: mov r5, r0
00390d90: mov r0, r4
00390d94: bl #0x30e4cc
00390d98: mov r4, r0
00390d9c: mov r0, r5
00390da0: bl #0x30e4cc
00390da4: mov r1, r4
00390da8: mov r2, r0
00390dac: mov r0, r8
00390db0: bl #0x39d724
00390db4: ldr r2, [r6, #4]
00390db8: mov r4, r0
00390dbc: ldr r3, [r2]
00390dc0: ldr r2, [r2, #4]
00390dc4: rsb r3, r3, r2
00390dc8: asr r3, r3, #4
00390dcc: add r2, r3, r3, lsl #3
00390dd0: add r2, r2, r2, lsl #6
00390dd4: add r2, r3, r2, lsl #3
00390dd8: add r2, r2, r2, lsl #15
00390ddc: add r3, r3, r2, lsl #3
00390de0: rsb r3, r3, #0
00390de4: cmp r3, #2
00390de8: bhi #0x390dfc
00390dec: mov r0, r7
00390df0: mov r1, r4
00390df4: bl #0x37c9f8
00390df8: b #0x390cb4
00390dfc: mov r0, r6
00390e00: mov r1, #2
00390e04: bl #0x37baf8
00390e08: ldr r3, [r0, #4]
00390e0c: cmp r3, #7
00390e10: beq #0x390ee4
00390e14: ldr r2, [r6, #4]
00390e18: ldr r1, [r2, #4]
00390e1c: ldr r3, [r2]
00390e20: rsb r3, r3, r1
00390e24: asr r3, r3, #4
00390e28: add r2, r3, r3, lsl #3
00390e2c: add r2, r2, r2, lsl #6
00390e30: add r2, r3, r2, lsl #3
00390e34: add r2, r2, r2, lsl #15
00390e38: add r3, r3, r2, lsl #3
00390e3c: rsb r3, r3, #0
00390e40: cmp r3, #4
00390e44: bls #0x390dec
00390e48: mov r1, #2
00390e4c: mov r0, r6
00390e50: bl #0x37baf8
00390e54: ldr r1, [r0, #4]
00390e58: cmp r1, #3
00390e5c: bne #0x390dec
00390e60: mov r0, r6
00390e64: bl #0x37baf8
00390e68: ldr r3, [r0, #4]
00390e6c: cmp r3, #3
00390e70: bne #0x390dec
00390e74: mov r0, r6
00390e78: mov r1, #4
00390e7c: bl #0x37baf8
00390e80: ldr r5, [r0, #4]
00390e84: cmp r5, #3
00390e88: bne #0x390dec
00390e8c: mov r1, #2
00390e90: mov r0, r6
00390e94: bl #0x37baf8
00390e98: bl #0x31bbf0
00390e9c: mov r1, r5
00390ea0: mov r8, r0
00390ea4: mov r0, r6
00390ea8: bl #0x37baf8
00390eac: bl #0x31bbf0
00390eb0: mov r1, #4
00390eb4: mov r5, r0
00390eb8: mov r0, r6
00390ebc: bl #0x37baf8
00390ec0: bl #0x31bbf0
00390ec4: add r1, sp, #4
00390ec8: str r0, [sp, #0xc]
00390ecc: mov r2, #1
00390ed0: mov r0, r4
00390ed4: str r8, [sp, #4]
00390ed8: str r5, [sp, #8]
00390edc: bl #0x393db4
00390ee0: b #0x390dec
00390ee4: mov r1, #2
00390ee8: mov r0, r6
00390eec: bl #0x37baf8
00390ef0: bl #0x31b5a0
00390ef4: mov r2, #1
00390ef8: add r1, r0, #0x160
00390efc: mov r0, r4
00390f00: bl #0x393db4
00390f04: b #0x390dec
00390f08: rsbeq r3, r0, r8, lsl lr
00390f0c: subseq sp, r2, r8, lsr #15
00390f10: strdeq r0, r1, [r0], -r8
00390f14: subseq sp, r2, ip, lsr r7
00390f18: andeq r2, r0, ip, lsl #30

_ZN3sfc6script3lua8Instance6pCall_EjRNS1_12ReturnValuesE 0x31aa78 156
0031aa78: push {r4, r5, r6, r7, r8, lr}
0031aa7c: mov r4, r0
0031aa80: ldr r0, [r0, #4]
0031aa84: mov r5, r2
0031aa88: mov r7, r1
0031aa8c: bl #0x84b12c
0031aa90: ldr r6, [r4, #4]
0031aa94: mov r3, #0
0031aa98: mov r1, r7
0031aa9c: mvn r2, #0
0031aaa0: mov r8, r0
0031aaa4: mov r0, r6
0031aaa8: bl #0x84bc50
0031aaac: mov r1, r6
0031aab0: mov r2, r0
0031aab4: add r0, r5, #4
0031aab8: bl #0x31a8ac
0031aabc: ldr r3, [r5, #8]
0031aac0: cmp r3, #0
0031aac4: beq #0x31aacc
0031aac8: pop {r4, r5, r6, r7, r8, pc}
0031aacc: ldr r0, [r4, #4]
0031aad0: bl #0x84b12c
0031aad4: rsb r7, r7, r8
0031aad8: rsb r7, r7, #1
0031aadc: add r7, r7, r0
0031aae0: cmp r7, #0
0031aae4: ble #0x31ab04
0031aae8: rsb r6, r7, #0
0031aaec: mov r2, r6
0031aaf0: mov r0, r5
0031aaf4: ldr r1, [r4, #4]
0031aaf8: bl #0x31b4ac
0031aafc: adds r6, r6, #1
0031ab00: bne #0x31aaec
0031ab04: ldr r0, [r4, #4]
0031ab08: mvn r1, r7
0031ab0c: pop {r4, r5, r6, r7, r8, lr}
0031ab10: b #0x84b140

_ZN10GameObject5_LockERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38d5f8 16
0038d5f8: ldrb r3, [r2, #0x29]
0038d5fc: add r3, r3, #1
0038d600: strb r3, [r2, #0x29]
0038d604: bx lr

_ZN9Character8_RegenMPERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7774 120
003b7774: str lr, [sp, #-4]!
003b7778: ldr r3, [r0, #4]
003b777c: sub sp, sp, #0xc
003b7780: ldr r1, [r3, #4]
003b7784: ldr ip, [r3]
003b7788: rsb r3, ip, r1
003b778c: asr r3, r3, #4
003b7790: add r1, r3, r3, lsl #3
003b7794: add r1, r1, r1, lsl #6
003b7798: add r1, r3, r1, lsl #3
003b779c: add r1, r1, r1, lsl #15
003b77a0: add r3, r3, r1, lsl #3
003b77a4: cmp r3, #0
003b77a8: bne #0x3b77b4
003b77ac: add sp, sp, #0xc
003b77b0: ldm sp!, {pc}
003b77b4: ldr r3, [ip, #4]
003b77b8: cmp r3, #3
003b77bc: bne #0x3b77ac
003b77c0: mov r1, #0
003b77c4: str r2, [sp, #4]
003b77c8: bl #0x37baf8
003b77cc: bl #0x31bbf0
003b77d0: bl #0x30e4cc
003b77d4: ldr r2, [sp, #4]
003b77d8: mov r1, r0
003b77dc: mov r0, r2
003b77e0: add sp, sp, #0xc
003b77e4: pop {lr}
003b77e8: b #0x3bdbb8

_ZN3sfc6script3lua6Binder17__smethodCallbackEP9lua_State 0x319fd0 640
00319fd0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00319fd4: ldr r4, [pc, #0x238]
00319fd8: ldr sb, [pc, #0x238]
00319fdc: sub sp, sp, #0x4c
00319fe0: add r4, pc, r4
00319fe4: ldr r3, [r4, sb]
00319fe8: mov r1, #1
00319fec: mov r5, r0
00319ff0: ldr r3, [r3]
00319ff4: str r3, [sp, #0x44]
00319ff8: bl #0x84b264
00319ffc: cmp r0, #5
0031a000: beq #0x31a028
0031a004: ldr r3, [pc, #0x210]
0031a008: ldr r3, [r4, r3]
0031a00c: ldr r3, [r3]
0031a010: cmp r3, #2
0031a014: moveq r3, #0
0031a018: streq r3, [r3]
0031a01c: beq #0x31a028
0031a020: cmp r3, #1
0031a024: beq #0x31a188
0031a028: ldr r2, [pc, #0x1f0]
0031a02c: mov r1, #1
0031a030: mov r0, r5
0031a034: add r2, pc, r2
0031a038: bl #0x84c1ec
0031a03c: mvn r1, #0
0031a040: mov r0, r5
0031a044: bl #0x84b390
0031a048: add r7, sp, #0x14
0031a04c: mvn r1, #1
0031a050: mov r8, r0
0031a054: mov r0, r5
0031a058: bl #0x84b140
0031a05c: add sl, sp, #0xc
0031a060: mov r1, r5
0031a064: mvn r2, #0
0031a068: mov r0, r7
0031a06c: bl #0x3196ec
0031a070: add r6, sp, #0x1c
0031a074: mov r2, #1
0031a078: mov r1, r5
0031a07c: mov r0, sl
0031a080: bl #0x3196ec
0031a084: mov r0, r6
0031a088: bl #0x31b434
0031a08c: ldr fp, [sp, #0x10]
0031a090: ldm fp, {r0, r3}
0031a094: rsb r3, r0, r3
0031a098: asr r3, r3, #4
0031a09c: add r2, r3, r3, lsl #3
0031a0a0: add r2, r2, r2, lsl #6
0031a0a4: add r2, r3, r2, lsl #3
0031a0a8: add r2, r2, r2, lsl #15
0031a0ac: add r3, r3, r2, lsl #3
0031a0b0: cmp r3, #0
0031a0b4: bne #0x31a0c8
0031a0b8: ldr r0, [pc, #0x164]
0031a0bc: add r0, pc, r0
0031a0c0: bl #0x708eb0
0031a0c4: ldr r0, [fp]
0031a0c8: bl #0x31b580
0031a0cc: subs fp, r0, #0
0031a0d0: beq #0x31a1bc
0031a0d4: cmp r8, #0
0031a0d8: beq #0x31a134
0031a0dc: mov r2, r8
0031a0e0: mov r0, r7
0031a0e4: mov r1, r6
0031a0e8: blx fp
0031a0ec: mov r1, r5
0031a0f0: mov r0, r6
0031a0f4: bl #0x31b308
0031a0f8: mov r5, r0
0031a0fc: mov r0, r6
0031a100: bl #0x31b398
0031a104: mov r0, sl
0031a108: bl #0x319228
0031a10c: mov r0, r7
0031a110: bl #0x319228
0031a114: ldr r3, [r4, sb]
0031a118: ldr r2, [sp, #0x44]
0031a11c: mov r0, r5
0031a120: ldr r3, [r3]
0031a124: cmp r2, r3
0031a128: bne #0x31a210
0031a12c: add sp, sp, #0x4c
0031a130: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031a134: ldr r3, [pc, #0xe0]
0031a138: ldr r3, [r4, r3]
0031a13c: ldr r3, [r3]
0031a140: cmp r3, #2
0031a144: streq r8, [r8]
0031a148: beq #0x31a0dc
0031a14c: cmp r3, #1
0031a150: bne #0x31a0dc
0031a154: ldr r0, [pc, #0xcc]
0031a158: ldr r1, [pc, #0xcc]
0031a15c: ldr r2, [pc, #0xcc]
0031a160: ldr r0, [r4, r0]
0031a164: ldr r3, [pc, #0xc8]
0031a168: mov ip, #0x3a
0031a16c: add r1, pc, r1
0031a170: add r2, pc, r2
0031a174: add r3, pc, r3
0031a178: add r0, r0, #0xa8
0031a17c: str ip, [sp]
0031a180: bl #0x30e004
0031a184: b #0x31a0dc
0031a188: ldr r0, [pc, #0x98]
0031a18c: ldr r1, [pc, #0xa4]
0031a190: ldr r2, [pc, #0xa4]
0031a194: ldr r0, [r4, r0]
0031a198: ldr r3, [pc, #0xa0]
0031a19c: mov ip, #0x2e
0031a1a0: add r1, pc, r1
0031a1a4: add r2, pc, r2
0031a1a8: add r3, pc, r3
0031a1ac: add r0, r0, #0xa8
0031a1b0: str ip, [sp]
0031a1b4: bl #0x30e004
0031a1b8: b #0x31a028
0031a1bc: ldr r3, [pc, #0x58]
0031a1c0: ldr r3, [r4, r3]
0031a1c4: ldr r3, [r3]
0031a1c8: cmp r3, #2
0031a1cc: streq fp, [fp]
0031a1d0: beq #0x31a0d4
0031a1d4: cmp r3, #1
0031a1d8: bne #0x31a0d4
0031a1dc: ldr r0, [pc, #0x44]
0031a1e0: ldr r1, [pc, #0x5c]
0031a1e4: ldr r2, [pc, #0x5c]
0031a1e8: ldr r0, [r4, r0]
0031a1ec: ldr r3, [pc, #0x58]
0031a1f0: mov ip, #0x39
0031a1f4: add r1, pc, r1
0031a1f8: add r2, pc, r2
0031a1fc: add r3, pc, r3
0031a200: add r0, r0, #0xa8
0031a204: str ip, [sp]
0031a208: bl #0x30e004
0031a20c: b #0x31a0d4
0031a210: bl #0x30e310
0031a214: strhteq sl, [r7], #-0xa0
0031a218: andeq r4, r0, ip, lsr #1
0031a21c: andeq r3, r0, r0, asr #19
0031a220: subseq r4, sl, ip, lsl r8
0031a224: subseq r4, sl, ip, lsr #7
0031a228: andeq r1, r0, r0, asr #19
0031a22c: subseq r4, sl, ip, ror #4
0031a230: subseq r4, sl, r0, ror #13
0031a234: subseq r4, sl, r4, ror #12
0031a238: subseq r4, sl, r8, lsr r2
0031a23c: subseq r4, sl, ip, lsl #13
0031a240: subseq r4, sl, r0, lsr r6
0031a244: subseq r4, sl, r4, ror #3
0031a248: subseq r4, sl, r8, lsr #12
0031a24c: ldrsbeq r4, [sl], #-0x5c

_ZNK9LuaScript6GetIntEPKc 0x37da30 148
0037da30: push {r4, r5, r6, lr}
0037da34: mov r4, r0
0037da38: mov r0, r1
0037da3c: mov r5, r1
0037da40: bl #0x37c164
0037da44: ldr r3, [r4, #0x20]
0037da48: add ip, r4, #0x1c
0037da4c: cmp r3, #0
0037da50: beq #0x37daa4
0037da54: mov r1, ip
0037da58: b #0x37da60
0037da5c: mov r3, r2
0037da60: ldr r2, [r3, #0x10]
0037da64: cmp r0, r2
0037da68: ldrhi r2, [r3, #0xc]
0037da6c: ldrls r2, [r3, #8]
0037da70: movhi r3, r1
0037da74: mov r1, r3
0037da78: cmp r2, #0
0037da7c: bne #0x37da5c
0037da80: cmp ip, r3
0037da84: beq #0x37daac
0037da88: ldr r2, [r3, #0x10]
0037da8c: cmp r0, r2
0037da90: blo #0x37daa4
0037da94: cmp ip, r3
0037da98: beq #0x37daac
0037da9c: ldr r0, [r3, #0x14]
0037daa0: pop {r4, r5, r6, pc}
0037daa4: mov r3, ip
0037daa8: b #0x37da94
0037daac: mov r0, r4
0037dab0: mov r1, r5
0037dab4: mov r2, #0
0037dab8: bl #0x37d990
0037dabc: mov r0, #0
0037dac0: pop {r4, r5, r6, pc}

_ZNSaIN3sfc6script3lua5ValueEE11_M_allocateEjRj 0x319538 136
00319538: push {r4, lr}
0031953c: movw r3, #0x2492
00319540: orr r3, r3, r3, lsl #12
00319544: cmp r1, r3
00319548: sub sp, sp, #8
0031954c: mov r4, r2
00319550: bhi #0x3195a8
00319554: cmp r1, #0
00319558: moveq r0, r1
0031955c: bne #0x319568
00319560: add sp, sp, #8
00319564: pop {r4, pc}
00319568: mov r0, #0x70
0031956c: mul r0, r0, r1
00319570: cmp r0, #0x80
00319574: str r0, [sp, #4]
00319578: bhi #0x3195a0
0031957c: add r0, sp, #4
00319580: bl #0x708ec0
00319584: ldr r2, [sp, #4]
00319588: movw r3, #0x4926
0031958c: movt r3, #0x2492
00319590: lsr r2, r2, #4
00319594: umull r1, r3, r3, r2
00319598: str r3, [r4]
0031959c: b #0x319560
003195a0: bl #0x310454
003195a4: b #0x319584
003195a8: ldr r0, [pc, #0xc]
003195ac: add r0, pc, r0
003195b0: bl #0x30e0c4
003195b4: mov r0, #1
003195b8: bl #0x30de48
003195bc: subseq r4, sl, r4, asr #29

_ZN9LuaScript16_IncludePyStructERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37b56c 4
0037b56c: bx lr

_ZN9LuaScript9_DivFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e068 316
0037e068: push {r4, r5, r6, lr}
0037e06c: ldr r5, [r0, #4]
0037e070: mov r6, r1
0037e074: mov r4, r0
0037e078: ldm r5, {r1, r3}
0037e07c: rsb r3, r1, r3
0037e080: asr r3, r3, #4
0037e084: add r2, r3, r3, lsl #3
0037e088: add r2, r2, r2, lsl #6
0037e08c: add r2, r3, r2, lsl #3
0037e090: add r2, r2, r2, lsl #15
0037e094: add r3, r3, r2, lsl #3
0037e098: rsb r3, r3, #0
0037e09c: cmp r3, #1
0037e0a0: bls #0x37e184
0037e0a4: cmp r3, #0
0037e0a8: beq #0x37e170
0037e0ac: ldr r3, [r1, #4]
0037e0b0: cmp r3, #3
0037e0b4: beq #0x37e188
0037e0b8: ldr r5, [r4, #4]
0037e0bc: ldm r5, {r0, r3}
0037e0c0: rsb r3, r0, r3
0037e0c4: asr r3, r3, #4
0037e0c8: add r2, r3, r3, lsl #3
0037e0cc: add r2, r2, r2, lsl #6
0037e0d0: add r2, r3, r2, lsl #3
0037e0d4: add r2, r2, r2, lsl #15
0037e0d8: add r3, r3, r2, lsl #3
0037e0dc: cmp r3, #0
0037e0e0: beq #0x37e15c
0037e0e4: bl #0x31bbf0
0037e0e8: bl #0x30e4cc
0037e0ec: ldr r4, [r4, #4]
0037e0f0: mov r5, r0
0037e0f4: ldm r4, {r0, r3}
0037e0f8: rsb r3, r0, r3
0037e0fc: asr r3, r3, #4
0037e100: add r2, r3, r3, lsl #3
0037e104: add r2, r2, r2, lsl #6
0037e108: add r2, r3, r2, lsl #3
0037e10c: add r2, r2, r2, lsl #15
0037e110: add r3, r3, r2, lsl #3
0037e114: rsb r3, r3, #0
0037e118: cmp r3, #1
0037e11c: bls #0x37e148
0037e120: add r0, r0, #0x70
0037e124: bl #0x31bbf0
0037e128: bl #0x30e4cc
0037e12c: asr r1, r0, #8
0037e130: mov r0, r5
0037e134: bl #0x30e2a4
0037e138: mov r1, r0
0037e13c: mov r0, r6
0037e140: pop {r4, r5, r6, lr}
0037e144: b #0x37cb24
0037e148: ldr r0, [pc, #0x48]
0037e14c: add r0, pc, r0
0037e150: bl #0x708eb0
0037e154: ldr r0, [r4]
0037e158: b #0x37e120
0037e15c: ldr r0, [pc, #0x38]
0037e160: add r0, pc, r0
0037e164: bl #0x708eb0
0037e168: ldr r0, [r5]
0037e16c: b #0x37e0e4
0037e170: ldr r0, [pc, #0x28]
0037e174: add r0, pc, r0
0037e178: bl #0x708eb0
0037e17c: ldr r1, [r5]
0037e180: b #0x37e0ac
0037e184: pop {r4, r5, r6, pc}
0037e188: mov r0, r4
0037e18c: mov r1, #1
0037e190: bl #0x37baf8
0037e194: b #0x37e0b8
0037e198: subseq r0, r4, ip, lsl r3
0037e19c: subseq r0, r4, r8, lsl #6
0037e1a0: ldrsheq r0, [r4], #-0x24

_ZN9LuaScript12_GetPyStructERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f4a8 340
0037f4a8: push {r4, r5, r6, r7, r8, lr}
0037f4ac: ldr r6, [r0, #4]
0037f4b0: mov r4, r1
0037f4b4: ldr r5, [pc, #0x12c]
0037f4b8: ldm r6, {r1, r3}
0037f4bc: add r5, pc, r5
0037f4c0: mov r7, r0
0037f4c4: rsb r3, r1, r3
0037f4c8: asr r3, r3, #4
0037f4cc: add r2, r3, r3, lsl #3
0037f4d0: add r2, r2, r2, lsl #6
0037f4d4: add r2, r3, r2, lsl #3
0037f4d8: add r2, r2, r2, lsl #15
0037f4dc: add r3, r3, r2, lsl #3
0037f4e0: rsb r3, r3, #0
0037f4e4: cmp r3, #1
0037f4e8: bls #0x37f500
0037f4ec: cmp r3, #0
0037f4f0: beq #0x37f504
0037f4f4: ldr r3, [r1, #4]
0037f4f8: cmp r3, #4
0037f4fc: beq #0x37f518
0037f500: pop {r4, r5, r6, r7, r8, pc}
0037f504: ldr r0, [pc, #0xe0]
0037f508: add r0, pc, r0
0037f50c: bl #0x708eb0
0037f510: ldr r1, [r6]
0037f514: b #0x37f4f4
0037f518: mov r0, r7
0037f51c: mov r1, #1
0037f520: bl #0x37baf8
0037f524: ldr r3, [r0, #4]
0037f528: cmp r3, #4
0037f52c: bne #0x37f500
0037f530: ldr r6, [r7, #4]
0037f534: ldr r2, [pc, #0xb4]
0037f538: ldm r6, {r0, r3}
0037f53c: ldr r2, [r5, r2]
0037f540: rsb r3, r0, r3
0037f544: asr r3, r3, #4
0037f548: ldr r8, [r2, #0x30]
0037f54c: add r2, r3, r3, lsl #3
0037f550: add r2, r2, r2, lsl #6
0037f554: add r2, r3, r2, lsl #3
0037f558: add r2, r2, r2, lsl #15
0037f55c: add r3, r3, r2, lsl #3
0037f560: cmp r3, #0
0037f564: bne #0x37f578
0037f568: ldr r0, [pc, #0x84]
0037f56c: add r0, pc, r0
0037f570: bl #0x708eb0
0037f574: ldr r0, [r6]
0037f578: bl #0x31c49c
0037f57c: ldr r5, [r7, #4]
0037f580: mov r6, r0
0037f584: ldm r5, {r0, r3}
0037f588: rsb r3, r0, r3
0037f58c: asr r3, r3, #4
0037f590: add r2, r3, r3, lsl #3
0037f594: add r2, r2, r2, lsl #6
0037f598: add r2, r3, r2, lsl #3
0037f59c: add r2, r2, r2, lsl #15
0037f5a0: add r3, r3, r2, lsl #3
0037f5a4: rsb r3, r3, #0
0037f5a8: cmp r3, #1
0037f5ac: bhi #0x37f5c0
0037f5b0: ldr r0, [pc, #0x40]
0037f5b4: add r0, pc, r0
0037f5b8: bl #0x708eb0
0037f5bc: ldr r0, [r5]
0037f5c0: add r0, r0, #0x70
0037f5c4: bl #0x31c49c
0037f5c8: mov r1, r6
0037f5cc: mov r2, r0
0037f5d0: mov r0, r8
0037f5d4: bl #0x4bd640
0037f5d8: mov r1, r0
0037f5dc: mov r0, r4
0037f5e0: pop {r4, r5, r6, r7, r8, lr}
0037f5e4: b #0x37cb24

_ZN9Character15_GetCharAIFlagsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6e80 68
003b6e80: ldr r3, [pc, #0x34]
003b6e84: mov r0, r2
003b6e88: ldr r2, [pc, #0x30]
003b6e8c: push {r4, r5, r6, lr}
003b6e90: add r3, pc, r3
003b6e94: ldr r2, [r3, r2]
003b6e98: mov r4, r1
003b6e9c: ldr r5, [r2]
003b6ea0: bl #0x3a2fec
003b6ea4: mov r3, #0x44
003b6ea8: mla r3, r3, r0, r5
003b6eac: mov r0, r4
003b6eb0: ldr r1, [r3, #0x14]
003b6eb4: pop {r4, r5, r6, lr}
003b6eb8: b #0x37cb24
003b6ebc: subseq sp, sp, r0, lsl #24
003b6ec0: andeq r0, r0, r8, asr r7

_ZN3sfc6script3lua12ReturnValuesD1Ev 0x31b398 64
0031b398: ldr r3, [pc, #0x30]
0031b39c: ldr r2, [pc, #0x30]
0031b3a0: push {r4, lr}
0031b3a4: add r3, pc, r3
0031b3a8: ldr r2, [r3, r2]
0031b3ac: mov r4, r0
0031b3b0: ldr r0, [r0, #0x24]
0031b3b4: add r2, r2, #8
0031b3b8: str r2, [r4]
0031b3bc: bl #0x31d194
0031b3c0: add r0, r4, #4
0031b3c4: bl #0x31a68c
0031b3c8: mov r0, r4
0031b3cc: pop {r4, pc}
0031b3d0: rsbeq sb, r7, ip, ror #13
0031b3d4: andeq r1, r0, r4, lsr #1

_ZN9LuaScript11_PopVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37dc44 388
0037dc44: push {r4, r5, r6, r7, r8, sb, sl, lr}
0037dc48: ldr sb, [pc, #0x174]
0037dc4c: ldr r4, [r2, #0x54]
0037dc50: sub sp, sp, #8
0037dc54: mov r8, r2
0037dc58: add sb, pc, sb
0037dc5c: add r7, r2, #0x4c
0037dc60: add r5, r2, #0x34
0037dc64: add sl, sp, #4
0037dc68: cmp r7, r4
0037dc6c: beq #0x37dcd4
0037dc70: ldr r0, [r4, #0x28]
0037dc74: ldr r6, [r4, #0x24]
0037dc78: rsb r6, r0, r6
0037dc7c: cmp r6, #0
0037dc80: ble #0x37dd44
0037dc84: mov r0, r5
0037dc88: add r1, r4, #0x10
0037dc8c: bl #0x37dac4
0037dc90: add r3, r4, #0x14
0037dc94: cmp r3, r0
0037dc98: beq #0x37dca8
0037dc9c: ldr r1, [r4, #0x28]
0037dca0: ldr r2, [r4, #0x24]
0037dca4: bl #0x3109e0
0037dca8: ldr r2, [r4, #0xc]
0037dcac: cmp r2, #0
0037dcb0: bne #0x37dcbc
0037dcb4: b #0x37dd10
0037dcb8: mov r2, r3
0037dcbc: ldr r3, [r2, #8]
0037dcc0: cmp r3, #0
0037dcc4: bne #0x37dcb8
0037dcc8: mov r4, r2
0037dccc: cmp r7, r4
0037dcd0: bne #0x37dc70
0037dcd4: ldr r3, [r8, #0x5c]
0037dcd8: cmp r3, #0
0037dcdc: beq #0x37dd00
0037dce0: mov r0, r7
0037dce4: ldr r1, [r8, #0x50]
0037dce8: bl #0x37bd7c
0037dcec: mov r3, #0
0037dcf0: str r7, [r8, #0x58]
0037dcf4: str r3, [r8, #0x5c]
0037dcf8: str r7, [r8, #0x54]
0037dcfc: str r3, [r8, #0x50]
0037dd00: mov r3, #0
0037dd04: strb r3, [r8, #0x64]
0037dd08: add sp, sp, #8
0037dd0c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0037dd10: ldr r3, [r4, #4]
0037dd14: ldr r1, [r3, #0xc]
0037dd18: cmp r4, r1
0037dd1c: bne #0x37dd38
0037dd20: mov r4, r3
0037dd24: ldr r3, [r3, #4]
0037dd28: ldr r2, [r3, #0xc]
0037dd2c: cmp r2, r4
0037dd30: beq #0x37dd20
0037dd34: ldr r2, [r4, #0xc]
0037dd38: cmp r2, r3
0037dd3c: movne r4, r3
0037dd40: b #0x37dc68
0037dd44: mov r1, sb
0037dd48: mov r2, r6
0037dd4c: bl #0x30e5e0
0037dd50: cmp r0, #0
0037dd54: bne #0x37dc84
0037dd58: cmp r6, #0
0037dd5c: bne #0x37dc84
0037dd60: ldr r3, [r8, #0x38]
0037dd64: cmp r3, #0
0037dd68: ldrne r0, [r4, #0x10]
0037dd6c: movne r1, r5
0037dd70: bne #0x37dd7c
0037dd74: b #0x37dca8
0037dd78: mov r3, r2
0037dd7c: ldr r2, [r3, #0x10]
0037dd80: cmp r2, r0
0037dd84: ldrlo r2, [r3, #0xc]
0037dd88: ldrhs r2, [r3, #8]
0037dd8c: movlo r3, r1
0037dd90: mov r1, r3
0037dd94: cmp r2, #0
0037dd98: bne #0x37dd78
0037dd9c: cmp r5, r3
0037dda0: beq #0x37dca8
0037dda4: ldr r2, [r3, #0x10]
0037dda8: cmp r2, r0
0037ddac: bhi #0x37dca8
0037ddb0: mov r0, r5
0037ddb4: mov r1, sl
0037ddb8: str r3, [sp, #4]
0037ddbc: bl #0x37be48
0037ddc0: b #0x37dca8
0037ddc4: ldrheq sp, [r4], #-0xb0

_ZN3sfc6script3lua5ValueC1EPNS1_8UserDataE 0x37c978 128
0037c978: ldr r2, [pc, #0x70]
0037c97c: ldr ip, [pc, #0x70]
0037c980: mov r3, r0
0037c984: add r2, pc, r2
0037c988: ldr ip, [r2, ip]
0037c98c: push {r4, r5, r6, lr}
0037c990: add ip, ip, #8
0037c994: mov r4, r0
0037c998: str ip, [r3], #0xc
0037c99c: mov r6, r1
0037c9a0: mov r0, r3
0037c9a4: str r3, [r4, #0x1c]
0037c9a8: str r3, [r4, #0x20]
0037c9ac: mov r1, #0x10
0037c9b0: bl #0x31167c
0037c9b4: ldr r2, [r4, #0x1c]
0037c9b8: add r3, r4, #0x24
0037c9bc: mov r5, #0
0037c9c0: strb r5, [r2]
0037c9c4: mov r0, r3
0037c9c8: str r3, [r4, #0x64]
0037c9cc: str r3, [r4, #0x68]
0037c9d0: bl #0x37be44
0037c9d4: ldr r3, [r4, #0x64]
0037c9d8: mov r0, r4
0037c9dc: mov r1, r6
0037c9e0: str r5, [r3]
0037c9e4: bl #0x31b608
0037c9e8: mov r0, r4
0037c9ec: pop {r4, r5, r6, pc}
0037c9f0: rsbeq r8, r1, ip, lsl #2
0037c9f4: muleq r0, r8, r7

_ZN10GameObject16_GetDistanceFromERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x393444 408
00393444: push {r4, r5, r6, r7, r8, lr}
00393448: ldr ip, [r0, #4]
0039344c: mov r5, r1
00393450: mov r4, r2
00393454: ldm ip, {r1, r6}
00393458: ldr r3, [pc, #0x170]
0039345c: sub sp, sp, #0x18
00393460: rsb r6, r1, r6
00393464: asr r6, r6, #4
00393468: add r3, pc, r3
0039346c: add r2, r6, r6, lsl #3
00393470: add r2, r2, r2, lsl #6
00393474: add r2, r6, r2, lsl #3
00393478: add r2, r2, r2, lsl #15
0039347c: add r6, r6, r2, lsl #3
00393480: cmp r6, #0
00393484: bne #0x393490
00393488: add sp, sp, #0x18
0039348c: pop {r4, r5, r6, r7, r8, pc}
00393490: ldr r2, [r1, #4]
00393494: cmp r2, #4
00393498: beq #0x39357c
0039349c: cmp r2, #7
003934a0: bne #0x393488
003934a4: ldr r6, [r0, #4]
003934a8: ldm r6, {r0, r3}
003934ac: rsb r3, r0, r3
003934b0: asr r3, r3, #4
003934b4: add r2, r3, r3, lsl #3
003934b8: add r2, r2, r2, lsl #6
003934bc: add r2, r3, r2, lsl #3
003934c0: add r2, r2, r2, lsl #15
003934c4: add r3, r3, r2, lsl #3
003934c8: cmp r3, #0
003934cc: bne #0x3934e0
003934d0: ldr r0, [pc, #0xfc]
003934d4: add r0, pc, r0
003934d8: bl #0x708eb0
003934dc: ldr r0, [r6]
003934e0: bl #0x31b5a0
003934e4: mov r6, r0
003934e8: cmp r6, #0
003934ec: moveq r1, #0xbf000000
003934f0: addeq r1, r1, #0x800000
003934f4: beq #0x393570
003934f8: ldr r1, [r4, #0x160]
003934fc: ldr r0, [r6, #0x160]
00393500: bl #0x30e3ac
00393504: ldr r1, [r4, #0x164]
00393508: mov r8, r0
0039350c: ldr r0, [r6, #0x164]
00393510: bl #0x30e3ac
00393514: ldr r1, [r4, #0x168]
00393518: mov r7, r0
0039351c: ldr r0, [r6, #0x168]
00393520: bl #0x30e3ac
00393524: mov r1, r8
00393528: mov r6, r0
0039352c: mov r0, r8
00393530: bl #0x30ed6c
00393534: mov r1, r7
00393538: mov r4, r0
0039353c: mov r0, r7
00393540: bl #0x30ed6c
00393544: mov r1, r0
00393548: mov r0, r4
0039354c: bl #0x30eba4
00393550: mov r1, r6
00393554: mov r4, r0
00393558: mov r0, r6
0039355c: bl #0x30ed6c
00393560: mov r1, r0
00393564: mov r0, r4
00393568: bl #0x30eba4
0039356c: mov r1, r0
00393570: mov r0, r5
00393574: bl #0x37ccbc
00393578: b #0x393488
0039357c: cmp r2, #4
00393580: bne #0x3934a4
00393584: ldr r2, [pc, #0x4c]
00393588: mov r1, #0
0039358c: add r6, sp, #0xc
00393590: ldr r3, [r3, r2]
00393594: ldr r7, [r3, #0x38]
00393598: bl #0x37baf8
0039359c: bl #0x31c49c
003935a0: mov ip, #0
003935a4: mov r2, r0
003935a8: mov r1, r7
003935ac: mov r0, r6
003935b0: mvn r3, #0
003935b4: str ip, [sp, #4]
003935b8: str ip, [sp]
003935bc: bl #0x34aca0
003935c0: mov r0, r6
003935c4: bl #0x33fee4
003935c8: mov r6, r0
003935cc: b #0x3934e8
003935d0: rsbeq r1, r0, r8, lsr #12

_ZN3sfc6script3lua12ReturnValuesC2Ev 0x31b470 60
0031b470: ldr r3, [pc, #0x2c]
0031b474: ldr r2, [pc, #0x2c]
0031b478: push {r4, lr}
0031b47c: add r3, pc, r3
0031b480: ldr r2, [r3, r2]
0031b484: mov r4, r0
0031b488: add r2, r2, #8
0031b48c: str r2, [r0], #4
0031b490: bl #0x31a804
0031b494: bl #0x31ce84
0031b498: str r0, [r4, #0x24]
0031b49c: mov r0, r4
0031b4a0: pop {r4, pc}
0031b4a4: rsbeq sb, r7, r4, lsl r6
0031b4a8: andeq r1, r0, r4, lsr #1

_ZN6TestUD10TestMethodERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesE 0x386e64 92
00386e64: push {r4, r5, r6, lr}
00386e68: ldr r1, [r1, #4]
00386e6c: mov r5, r2
00386e70: mov r4, r0
00386e74: ldr r3, [r1]
00386e78: ldr r1, [r1, #4]
00386e7c: ldr r0, [pc, #0x38]
00386e80: rsb r3, r3, r1
00386e84: asr r3, r3, #4
00386e88: mov r1, r4
00386e8c: add r2, r3, r3, lsl #3
00386e90: add r0, pc, r0
00386e94: add r2, r2, r2, lsl #6
00386e98: add r2, r3, r2, lsl #3
00386e9c: add r2, r2, r2, lsl #15
00386ea0: add r2, r3, r2, lsl #3
00386ea4: rsb r2, r2, #0
00386ea8: bl #0x30de84
00386eac: mov r0, r5
00386eb0: mov r1, r4
00386eb4: pop {r4, r5, r6, lr}
00386eb8: b #0x37c9f8
00386ebc: subseq fp, r3, r8, asr r2

_ZN9LuaScript9_GetPyCstERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f354 340
0037f354: push {r4, r5, r6, r7, r8, lr}
0037f358: ldr r6, [r0, #4]
0037f35c: mov r4, r1
0037f360: ldr r5, [pc, #0x12c]
0037f364: ldm r6, {r1, r3}
0037f368: add r5, pc, r5
0037f36c: mov r7, r0
0037f370: rsb r3, r1, r3
0037f374: asr r3, r3, #4
0037f378: add r2, r3, r3, lsl #3
0037f37c: add r2, r2, r2, lsl #6
0037f380: add r2, r3, r2, lsl #3
0037f384: add r2, r2, r2, lsl #15
0037f388: add r3, r3, r2, lsl #3
0037f38c: rsb r3, r3, #0
0037f390: cmp r3, #1
0037f394: bls #0x37f3ac
0037f398: cmp r3, #0
0037f39c: beq #0x37f3b0
0037f3a0: ldr r3, [r1, #4]
0037f3a4: cmp r3, #4
0037f3a8: beq #0x37f3c4
0037f3ac: pop {r4, r5, r6, r7, r8, pc}
0037f3b0: ldr r0, [pc, #0xe0]
0037f3b4: add r0, pc, r0
0037f3b8: bl #0x708eb0
0037f3bc: ldr r1, [r6]
0037f3c0: b #0x37f3a0
0037f3c4: mov r0, r7
0037f3c8: mov r1, #1
0037f3cc: bl #0x37baf8
0037f3d0: ldr r3, [r0, #4]
0037f3d4: cmp r3, #4
0037f3d8: bne #0x37f3ac
0037f3dc: ldr r6, [r7, #4]
0037f3e0: ldr r2, [pc, #0xb4]
0037f3e4: ldm r6, {r0, r3}
0037f3e8: ldr r2, [r5, r2]
0037f3ec: rsb r3, r0, r3
0037f3f0: asr r3, r3, #4
0037f3f4: ldr r8, [r2, #0x2c]
0037f3f8: add r2, r3, r3, lsl #3
0037f3fc: add r2, r2, r2, lsl #6
0037f400: add r2, r3, r2, lsl #3
0037f404: add r2, r2, r2, lsl #15
0037f408: add r3, r3, r2, lsl #3
0037f40c: cmp r3, #0
0037f410: bne #0x37f424
0037f414: ldr r0, [pc, #0x84]
0037f418: add r0, pc, r0
0037f41c: bl #0x708eb0
0037f420: ldr r0, [r6]
0037f424: bl #0x31c49c
0037f428: ldr r5, [r7, #4]
0037f42c: mov r6, r0
0037f430: ldm r5, {r0, r3}
0037f434: rsb r3, r0, r3
0037f438: asr r3, r3, #4
0037f43c: add r2, r3, r3, lsl #3
0037f440: add r2, r2, r2, lsl #6
0037f444: add r2, r3, r2, lsl #3
0037f448: add r2, r2, r2, lsl #15
0037f44c: add r3, r3, r2, lsl #3
0037f450: rsb r3, r3, #0
0037f454: cmp r3, #1
0037f458: bhi #0x37f46c
0037f45c: ldr r0, [pc, #0x40]
0037f460: add r0, pc, r0
0037f464: bl #0x708eb0
0037f468: ldr r0, [r5]
0037f46c: add r0, r0, #0x70
0037f470: bl #0x31c49c
0037f474: mov r1, r6
0037f478: mov r2, r0
0037f47c: mov r0, r8
0037f480: bl #0x4c4bdc
0037f484: mov r1, r0
0037f488: mov r0, r4
0037f48c: pop {r4, r5, r6, r7, r8, lr}
0037f490: b #0x37cb24
0037f494: rsbeq r5, r1, r8, lsr #14
0037f498: ldrheq pc, [r3], #-4
0037f49c: strdeq r3, r4, [r0], -r4
0037f4a0: subseq pc, r3, r0, asr r0
0037f4a4: subseq pc, r3, r8

_ZN10GameObject8_GetNameERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ebf0 12
0038ebf0: mov r0, r1
0038ebf4: ldr r1, [r2, #0x44]
0038ebf8: b #0x37c8cc

_ZNK9LuaScript12_GetFuncNameEPKc 0x37c314 124
0037c314: push {r4, r5, r6, lr}
0037c318: mov r5, r0
0037c31c: mov r0, r1
0037c320: mov r4, r1
0037c324: bl #0x37c164
0037c328: ldr r3, [r5, #0x38]
0037c32c: add r5, r5, #0x34
0037c330: cmp r3, #0
0037c334: beq #0x37c388
0037c338: mov r1, r5
0037c33c: b #0x37c344
0037c340: mov r3, r2
0037c344: ldr r2, [r3, #0x10]
0037c348: cmp r0, r2
0037c34c: ldrhi r2, [r3, #0xc]
0037c350: ldrls r2, [r3, #8]
0037c354: movhi r3, r1
0037c358: mov r1, r3
0037c35c: cmp r2, #0
0037c360: bne #0x37c340
0037c364: cmp r5, r3
0037c368: beq #0x37c380
0037c36c: ldr r2, [r3, #0x10]
0037c370: cmp r0, r2
0037c374: blo #0x37c388
0037c378: cmp r5, r3
0037c37c: ldrne r4, [r3, #0x28]
0037c380: mov r0, r4
0037c384: pop {r4, r5, r6, pc}
0037c388: mov r3, r5
0037c38c: b #0x37c378

_ZN9LuaScript7_SetIntERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37de5c 212
0037de5c: push {r4, r5, r6, r7, r8, lr}
0037de60: ldr r4, [r0, #4]
0037de64: mov r5, r0
0037de68: mov r6, r2
0037de6c: ldm r4, {r0, r3}
0037de70: rsb r3, r0, r3
0037de74: asr r3, r3, #4
0037de78: add r2, r3, r3, lsl #3
0037de7c: add r2, r2, r2, lsl #6
0037de80: add r2, r3, r2, lsl #3
0037de84: add r2, r2, r2, lsl #15
0037de88: add r3, r3, r2, lsl #3
0037de8c: rsb r3, r3, #0
0037de90: cmp r3, #1
0037de94: bls #0x37df24
0037de98: cmp r3, #0
0037de9c: beq #0x37df10
0037dea0: bl #0x31c49c
0037dea4: ldr r4, [r5, #4]
0037dea8: mov r7, r0
0037deac: ldm r4, {r0, r3}
0037deb0: rsb r3, r0, r3
0037deb4: asr r3, r3, #4
0037deb8: add r2, r3, r3, lsl #3
0037debc: add r2, r2, r2, lsl #6
0037dec0: add r2, r3, r2, lsl #3
0037dec4: add r2, r2, r2, lsl #15
0037dec8: add r3, r3, r2, lsl #3
0037decc: rsb r3, r3, #0
0037ded0: cmp r3, #1
0037ded4: bls #0x37defc
0037ded8: add r0, r0, #0x70
0037dedc: bl #0x31bbf0
0037dee0: bl #0x30e4cc
0037dee4: mov r3, r0
0037dee8: mov r1, r7
0037deec: mov r0, r6
0037def0: mov r2, r3
0037def4: pop {r4, r5, r6, r7, r8, lr}
0037def8: b #0x37d990
0037defc: ldr r0, [pc, #0x24]
0037df00: add r0, pc, r0
0037df04: bl #0x708eb0
0037df08: ldr r0, [r4]
0037df0c: b #0x37ded8
0037df10: ldr r0, [pc, #0x14]
0037df14: add r0, pc, r0
0037df18: bl #0x708eb0
0037df1c: ldr r0, [r4]
0037df20: b #0x37dea0
0037df24: pop {r4, r5, r6, r7, r8, pc}
0037df28: subseq r0, r4, r8, ror #10
0037df2c: subseq r0, r4, r4, asr r5

_ZN9LuaScript14_GetGameScriptERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37c934 68
0037c934: ldr r3, [pc, #0x34]
0037c938: ldr r2, [pc, #0x34]
0037c93c: push {r4, lr}
0037c940: add r3, pc, r3
0037c944: ldr r0, [r3, r2]
0037c948: mov r4, r1
0037c94c: bl #0x31f594
0037c950: cmp r0, #0
0037c954: beq #0x37c96c
0037c958: bl #0x3f1394
0037c95c: mov r1, r0
0037c960: mov r0, r4
0037c964: pop {r4, lr}
0037c968: b #0x37c8cc
0037c96c: pop {r4, pc}
0037c970: rsbeq r8, r1, r0, asr r1
0037c974: strdeq r3, r4, [r0], -r4

_ZN9Character10_SetTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8f38 100
003b8f38: push {r4, lr}
003b8f3c: ldr r3, [r0, #4]
003b8f40: ldm r3, {r0, r1}
003b8f44: rsb r3, r0, r1
003b8f48: asr r3, r3, #4
003b8f4c: add r1, r3, r3, lsl #3
003b8f50: add r1, r1, r1, lsl #6
003b8f54: add r1, r3, r1, lsl #3
003b8f58: add r1, r1, r1, lsl #15
003b8f5c: add r3, r3, r1, lsl #3
003b8f60: cmp r3, #0
003b8f64: bne #0x3b8f6c
003b8f68: pop {r4, pc}
003b8f6c: ldr r3, [r0, #4]
003b8f70: cmp r3, #2
003b8f74: beq #0x3b8f80
003b8f78: cmp r3, #7
003b8f7c: bne #0x3b8f68
003b8f80: add r4, r2, #0x3c8
003b8f84: bl #0x31b5a0
003b8f88: mov r2, #0
003b8f8c: mov r1, r0
003b8f90: mov r0, r4
003b8f94: pop {r4, lr}
003b8f98: b #0x3d6890

_ZN10LuaManagerC1Ev 0x379eb0 72
00379eb0: ldr r1, [pc, #0x38]
00379eb4: str r4, [sp, #-4]!
00379eb8: ldr r4, [pc, #0x34]
00379ebc: add r1, pc, r1
00379ec0: mov ip, #0
00379ec4: ldr r4, [r1, r4]
00379ec8: mov r2, r0
00379ecc: str ip, [r0, #8]
00379ed0: add r4, r4, #8
00379ed4: str r4, [r0]
00379ed8: strb ip, [r2, #4]!
00379edc: str r2, [r0, #0x10]
00379ee0: str ip, [r0, #0x14]
00379ee4: str r2, [r0, #0xc]
00379ee8: ldm sp!, {r4}
00379eec: bx lr

_ZN9Character18_GetSkillIDFromOIDERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b856c 220
003b856c: push {r4, r5, r6, r7, r8, lr}
003b8570: ldr r3, [r0, #4]
003b8574: mov r5, r0
003b8578: mov r6, r1
003b857c: ldm r3, {r0, r1}
003b8580: ldr r4, [pc, #0xb4]
003b8584: mov r7, r2
003b8588: rsb r1, r0, r1
003b858c: asr r1, r1, #4
003b8590: add r4, pc, r4
003b8594: add r3, r1, r1, lsl #3
003b8598: add r3, r3, r3, lsl #6
003b859c: add r3, r1, r3, lsl #3
003b85a0: add r3, r3, r3, lsl #15
003b85a4: add r1, r1, r3, lsl #3
003b85a8: cmp r1, #0
003b85ac: bne #0x3b85b4
003b85b0: pop {r4, r5, r6, r7, r8, pc}
003b85b4: ldr r3, [r0, #4]
003b85b8: cmp r3, #3
003b85bc: beq #0x3b8618
003b85c0: bl #0x31bbf0
003b85c4: ldr r3, [pc, #0x74]
003b85c8: ldr r4, [r4, r3]
003b85cc: bl #0x8be2a0
003b85d0: ldr r3, [r4]
003b85d4: cmp r3, r0
003b85d8: bls #0x3b85b0
003b85dc: ldr r4, [r5, #4]
003b85e0: ldm r4, {r0, r3}
003b85e4: rsb r3, r0, r3
003b85e8: asr r3, r3, #4
003b85ec: add r2, r3, r3, lsl #3
003b85f0: add r2, r2, r2, lsl #6
003b85f4: add r2, r3, r2, lsl #3
003b85f8: add r2, r2, r2, lsl #15
003b85fc: add r3, r3, r2, lsl #3
003b8600: cmp r3, #0
003b8604: bne #0x3b8618
003b8608: ldr r0, [pc, #0x34]
003b860c: add r0, pc, r0
003b8610: bl #0x708eb0
003b8614: ldr r0, [r4]
003b8618: bl #0x31bbf0
003b861c: bl #0x8be2a0
003b8620: mov r1, r0
003b8624: mov r0, r7
003b8628: bl #0x3bc62c
003b862c: mov r1, r0
003b8630: mov r0, r6
003b8634: pop {r4, r5, r6, r7, r8, lr}
003b8638: b #0x37cb24
003b863c: subseq ip, sp, r0, lsl #10
003b8640: andeq r2, r0, r8, lsr #14
003b8644: subseq r5, r0, ip, asr lr

_ZN9Character16_SetIsTargetableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7a64 104
003b7a64: str lr, [sp, #-4]!
003b7a68: ldr r3, [r0, #4]
003b7a6c: sub sp, sp, #0xc
003b7a70: ldr r1, [r3, #4]
003b7a74: ldr ip, [r3]
003b7a78: rsb r3, ip, r1
003b7a7c: asr r3, r3, #4
003b7a80: add r1, r3, r3, lsl #3
003b7a84: add r1, r1, r1, lsl #6
003b7a88: add r1, r3, r1, lsl #3
003b7a8c: add r1, r1, r1, lsl #15
003b7a90: add r3, r3, r1, lsl #3
003b7a94: cmp r3, #0
003b7a98: bne #0x3b7aa4
003b7a9c: add sp, sp, #0xc
003b7aa0: ldm sp!, {pc}
003b7aa4: ldr r3, [ip, #4]
003b7aa8: cmp r3, #1
003b7aac: bne #0x3b7a9c
003b7ab0: mov r1, #0
003b7ab4: str r2, [sp, #4]
003b7ab8: bl #0x37baf8
003b7abc: bl #0x31bc80
003b7ac0: ldr r2, [sp, #4]
003b7ac4: strb r0, [r2, #0x415]
003b7ac8: b #0x3b7a9c

_ZN10GameObject7_UnlockERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38d7a8 20
0038d7a8: ldrb r3, [r2, #0x29]
0038d7ac: cmp r3, #0
0038d7b0: subne r3, r3, #1
0038d7b4: strbne r3, [r2, #0x29]
0038d7b8: bx lr

_ZN9Character8_RegenHPERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b77ec 120
003b77ec: str lr, [sp, #-4]!
003b77f0: ldr r3, [r0, #4]
003b77f4: sub sp, sp, #0xc
003b77f8: ldr r1, [r3, #4]
003b77fc: ldr ip, [r3]
003b7800: rsb r3, ip, r1
003b7804: asr r3, r3, #4
003b7808: add r1, r3, r3, lsl #3
003b780c: add r1, r1, r1, lsl #6
003b7810: add r1, r3, r1, lsl #3
003b7814: add r1, r1, r1, lsl #15
003b7818: add r3, r3, r1, lsl #3
003b781c: cmp r3, #0
003b7820: bne #0x3b782c
003b7824: add sp, sp, #0xc
003b7828: ldm sp!, {pc}
003b782c: ldr r3, [ip, #4]
003b7830: cmp r3, #3
003b7834: bne #0x3b7824
003b7838: mov r1, #0
003b783c: str r2, [sp, #4]
003b7840: bl #0x37baf8
003b7844: bl #0x31bbf0
003b7848: bl #0x30e4cc
003b784c: ldr r2, [sp, #4]
003b7850: mov r1, r0
003b7854: mov r0, r2
003b7858: add sp, sp, #0xc
003b785c: pop {lr}
003b7860: b #0x3bdca4

_ZN3sfc6script3lua5ErrorC1ERKS2_ 0x31a714 120
0031a714: ldr r3, [pc, #0x68]
0031a718: ldr r2, [pc, #0x68]
0031a71c: push {r4, r5, r6, lr}
0031a720: add r3, pc, r3
0031a724: ldr r2, [r3, r2]
0031a728: mov r5, r0
0031a72c: mov r4, r0
0031a730: add r2, r2, #8
0031a734: str r2, [r5], #8
0031a738: str r5, [r0, #0x18]
0031a73c: str r5, [r0, #0x1c]
0031a740: mov r0, r5
0031a744: mov r6, r1
0031a748: bl #0x31a710
0031a74c: ldr r3, [r4, #0x18]
0031a750: mov r1, #0
0031a754: add r2, r6, #8
0031a758: strb r1, [r3]
0031a75c: ldr r3, [r6, #4]
0031a760: cmp r5, r2
0031a764: str r3, [r4, #4]
0031a768: beq #0x31a77c
0031a76c: mov r0, r5
0031a770: ldr r2, [r6, #0x18]
0031a774: ldr r1, [r6, #0x1c]
0031a778: bl #0x3109e0
0031a77c: mov r0, r4
0031a780: pop {r4, r5, r6, pc}
0031a784: rsbeq sl, r7, r0, ror r3
0031a788: muleq r0, r8, r4

_ZN10GameObject8_IsDecorERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38e9c0 24
0038e9c0: ldr r3, [r2, #0xf4]
0038e9c4: mov r0, r1
0038e9c8: cmp r3, #0x15
0038e9cc: movne r1, #0
0038e9d0: moveq r1, #1
0038e9d4: b #0x37c7e4

_ZN12CharAIScript12_ChangeStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3d9710 128
003d9710: push {r4, r5, r6, lr}
003d9714: ldr r3, [r0, #4]
003d9718: mov r4, r2
003d971c: sub sp, sp, #8
003d9720: ldm r3, {r0, r2}
003d9724: rsb r3, r0, r2
003d9728: asr r3, r3, #4
003d972c: add r2, r3, r3, lsl #3
003d9730: add r2, r2, r2, lsl #6
003d9734: add r2, r3, r2, lsl #3
003d9738: add r2, r2, r2, lsl #15
003d973c: add r3, r3, r2, lsl #3
003d9740: cmn r3, #1
003d9744: beq #0x3d9750
003d9748: add sp, sp, #8
003d974c: pop {r4, r5, r6, pc}
003d9750: bl #0x31c49c
003d9754: add r5, r4, #0x9c
003d9758: add r1, sp, #8
003d975c: str r0, [r1, #-4]!
003d9760: mov r0, r5
003d9764: bl #0x3d9358
003d9768: cmp r0, r5
003d976c: mov r6, r0
003d9770: beq #0x3d9748
003d9774: mov r0, r4
003d9778: add r6, r6, #0x28
003d977c: bl #0x3d8e78
003d9780: str r6, [r4, #0xb4]
003d9784: mov r0, r4
003d9788: bl #0x3d8e8c
003d978c: b #0x3d9748

_ZN9Character11_PauseTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b751c 116
003b751c: str lr, [sp, #-4]!
003b7520: ldr r3, [r0, #4]
003b7524: sub sp, sp, #0xc
003b7528: ldr r1, [r3, #4]
003b752c: ldr ip, [r3]
003b7530: rsb r3, ip, r1
003b7534: asr r3, r3, #4
003b7538: add r1, r3, r3, lsl #3
003b753c: add r1, r1, r1, lsl #6
003b7540: add r1, r3, r1, lsl #3
003b7544: add r1, r1, r1, lsl #15
003b7548: add r3, r3, r1, lsl #3
003b754c: cmp r3, #0
003b7550: bne #0x3b755c
003b7554: add sp, sp, #0xc
003b7558: ldm sp!, {pc}
003b755c: ldr r3, [ip, #4]
003b7560: cmp r3, #3
003b7564: bne #0x3b7554
003b7568: mov r1, #0
003b756c: str r2, [sp, #4]
003b7570: bl #0x37baf8
003b7574: bl #0x38d798
003b7578: ldr r2, [sp, #4]
003b757c: mov r1, r0
003b7580: add r0, r2, #0x3b4
003b7584: add sp, sp, #0xc
003b7588: pop {lr}
003b758c: b #0x3db298

_ZN9LuaScript13_IncludePyCstERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37b568 4
0037b568: bx lr

_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE19_M_clear_after_moveEv 0x319324 140
00319324: push {r4, r5, r6, lr}
00319328: ldr r4, [r0, #4]
0031932c: ldr r5, [r0]
00319330: mov r6, r0
00319334: cmp r4, r5
00319338: beq #0x319358
0031933c: ldr r3, [r4, #-0x70]!
00319340: mov r0, r4
00319344: mov lr, pc
00319348: ldr pc, [r3]
0031934c: cmp r5, r4
00319350: bne #0x31933c
00319354: ldr r4, [r6]
00319358: cmp r4, #0
0031935c: ldr r3, [r6, #8]
00319360: beq #0x3193ac
00319364: rsb r3, r4, r3
00319368: asr r3, r3, #4
0031936c: mov r1, #0x70
00319370: add r2, r3, r3, lsl #3
00319374: add r2, r2, r2, lsl #6
00319378: add r2, r3, r2, lsl #3
0031937c: add r2, r2, r2, lsl #15
00319380: add r3, r3, r2, lsl #3
00319384: rsb r3, r3, #0
00319388: mul r1, r1, r3
0031938c: cmp r1, #0x80
00319390: bhi #0x3193a0
00319394: mov r0, r4
00319398: pop {r4, r5, r6, lr}
0031939c: b #0x708f00
003193a0: mov r0, r4
003193a4: pop {r4, r5, r6, lr}
003193a8: b #0x310440
003193ac: pop {r4, r5, r6, pc}

_ZN9Character26_GetCurrentEquippedFaeryIdERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6da0 36
003b6da0: push {r4, lr}
003b6da4: mov r0, r2
003b6da8: mov r4, r1
003b6dac: mvn r1, #0
003b6db0: bl #0x3bb98c
003b6db4: mov r1, r0
003b6db8: mov r0, r4
003b6dbc: pop {r4, lr}
003b6dc0: b #0x37cb24

_ZN3sfc6script3lua5Value7setBoolEb 0x31b5cc 28
0031b5cc: mov r3, #1
0031b5d0: cmp r1, #0
0031b5d4: str r3, [r0, #4]
0031b5d8: moveq r3, #0
0031b5dc: movne r3, #0x3f800000
0031b5e0: str r3, [r0, #8]
0031b5e4: bx lr

_ZN9LuaScript19_GetHostPlayerLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37cc00 60
0037cc00: ldr r3, [pc, #0x2c]
0037cc04: ldr r2, [pc, #0x2c]
0037cc08: push {r4, lr}
0037cc0c: add r3, pc, r3
0037cc10: ldr r2, [r3, r2]
0037cc14: mov r4, r1
0037cc18: ldr r0, [r2, #0x40]
0037cc1c: bl #0x36e09c
0037cc20: ldr r3, [r0, #0x330]
0037cc24: mov r0, r4
0037cc28: mov r1, r3
0037cc2c: pop {r4, lr}
0037cc30: b #0x37cb24
0037cc34: rsbeq r7, r1, r4, lsl #29
0037cc38: strdeq r3, r4, [r0], -r4

_ZN3sfc6script3lua8InstanceD1Ev 0x31b180 68
0031b180: push {r4, lr}
0031b184: ldr r3, [pc, #0x30]
0031b188: ldr r2, [pc, #0x30]
0031b18c: ldrb r1, [r0, #8]
0031b190: add r3, pc, r3
0031b194: ldr r2, [r3, r2]
0031b198: cmp r1, #0
0031b19c: mov r4, r0
0031b1a0: add r2, r2, #8
0031b1a4: str r2, [r0]
0031b1a8: beq #0x31b1b4
0031b1ac: ldr r0, [r0, #4]
0031b1b0: bl #0x85797c
0031b1b4: mov r0, r4
0031b1b8: pop {r4, pc}
0031b1bc: rsbeq sb, r7, r0, lsl #18
0031b1c0: andeq r4, r0, r8, lsl r0

_ZN10GameObject21_TargetListSearchRectERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x392b80 1936
00392b80: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00392b84: ldr r7, [r0, #4]
00392b88: mov r6, r2
00392b8c: ldr r4, [pc, #0x758]
00392b90: ldm r7, {r1, r3}
00392b94: add r4, pc, r4
00392b98: sub sp, sp, #0x3c
00392b9c: rsb r3, r1, r3
00392ba0: asr r3, r3, #4
00392ba4: mov r5, r0
00392ba8: add r2, r3, r3, lsl #3
00392bac: add r2, r2, r2, lsl #6
00392bb0: add r2, r3, r2, lsl #3
00392bb4: add r2, r2, r2, lsl #15
00392bb8: add r3, r3, r2, lsl #3
00392bbc: rsb r3, r3, #0
00392bc0: cmp r3, #1
00392bc4: bls #0x392bdc
00392bc8: cmp r3, #0
00392bcc: beq #0x392be4
00392bd0: ldr r3, [r1, #4]
00392bd4: cmp r3, #3
00392bd8: beq #0x392c00
00392bdc: add sp, sp, #0x3c
00392be0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00392be4: ldr r0, [pc, #0x704]
00392be8: add r0, pc, r0
00392bec: bl #0x708eb0
00392bf0: ldr r1, [r7]
00392bf4: ldr r3, [r1, #4]
00392bf8: cmp r3, #3
00392bfc: bne #0x392bdc
00392c00: ldr r7, [r5, #4]
00392c04: ldm r7, {r2, r3}
00392c08: rsb r3, r2, r3
00392c0c: asr r3, r3, #4
00392c10: add r1, r3, r3, lsl #3
00392c14: add r1, r1, r1, lsl #6
00392c18: add r1, r3, r1, lsl #3
00392c1c: add r1, r1, r1, lsl #15
00392c20: add r3, r3, r1, lsl #3
00392c24: rsb r3, r3, #0
00392c28: cmp r3, #1
00392c2c: bhi #0x392c40
00392c30: ldr r0, [pc, #0x6bc]
00392c34: add r0, pc, r0
00392c38: bl #0x708eb0
00392c3c: ldr r2, [r7]
00392c40: ldr r7, [r2, #0x74]
00392c44: cmp r7, #3
00392c48: bne #0x392bdc
00392c4c: ldr r2, [r5, #4]
00392c50: ldr r3, [r6, #0x168]
00392c54: ldr r0, [r6, #0x160]
00392c58: ldr r1, [r6, #0x164]
00392c5c: ldm r2, {r8, sl}
00392c60: str r0, [sp, #0x2c]
00392c64: str r1, [sp, #0x30]
00392c68: str r3, [sp, #0x34]
00392c6c: ldr r3, [r2]
00392c70: ldr r2, [r2, #4]
00392c74: rsb r3, r3, r2
00392c78: asr r3, r3, #4
00392c7c: add r2, r3, r3, lsl #3
00392c80: add r2, r2, r2, lsl #6
00392c84: add r2, r3, r2, lsl #3
00392c88: add r2, r2, r2, lsl #15
00392c8c: add r3, r3, r2, lsl #3
00392c90: rsb r3, r3, #0
00392c94: cmp r3, #2
00392c98: bhi #0x392e4c
00392c9c: rsb r8, r8, sl
00392ca0: asr r8, r8, #4
00392ca4: add r7, r8, r8, lsl #3
00392ca8: add r7, r7, r7, lsl #6
00392cac: add r7, r8, r7, lsl #3
00392cb0: add r7, r7, r7, lsl #15
00392cb4: add r7, r8, r7, lsl #3
00392cb8: mvn r7, r7
00392cbc: cmp r7, r3
00392cc0: blo #0x392d88
00392cc4: ldr r3, [r6, #0x338]
00392cc8: ands r3, r3, #0x80
00392ccc: beq #0x392d3c
00392cd0: ldr r2, [pc, #0x620]
00392cd4: ldr ip, [pc, #0x620]
00392cd8: mov r3, #0
00392cdc: ldr r2, [r4, r2]
00392ce0: ldr ip, [r4, ip]
00392ce4: mov r1, r3
00392ce8: ldr r2, [r2, #0x40]
00392cec: add ip, ip, #8
00392cf0: mov r0, r5
00392cf4: str ip, [sp, #0x18]
00392cf8: str r2, [sp, #0x1c]
00392cfc: str r3, [sp, #0x20]
00392d00: bl #0x37baf8
00392d04: bl #0x31bbf0
00392d08: mov r1, #1
00392d0c: mov r4, r0
00392d10: mov r0, r5
00392d14: bl #0x37baf8
00392d18: bl #0x31bbf0
00392d1c: add ip, sp, #0x18
00392d20: mov r3, r0
00392d24: mov r2, r4
00392d28: add r0, r6, #0x304
00392d2c: add r1, sp, #0x2c
00392d30: str ip, [sp]
00392d34: bl #0x4a2e70
00392d38: b #0x392bdc
00392d3c: ldr r2, [r6, #0x33c]
00392d40: cmp r2, #2
00392d44: beq #0x392ee0
00392d48: ldr r0, [pc, #0x5a8]
00392d4c: ldr r2, [pc, #0x5ac]
00392d50: mov r1, r3
00392d54: ldr ip, [r4, r0]
00392d58: ldr r2, [r4, r2]
00392d5c: mov r0, r5
00392d60: ldr ip, [ip, #0x38]
00392d64: add r2, r2, #8
00392d68: str r2, [sp, #0x18]
00392d6c: add r2, ip, #0x80
00392d70: str r2, [sp, #0x1c]
00392d74: ldr ip, [ip, #0x80]
00392d78: str r2, [sp, #0x24]
00392d7c: str r3, [sp, #0x28]
00392d80: str ip, [sp, #0x20]
00392d84: b #0x392d00
00392d88: mov r0, r5
00392d8c: mov r1, r7
00392d90: bl #0x37baf8
00392d94: ldr r8, [r0, #4]
00392d98: cmp r8, #1
00392d9c: beq #0x392f74
00392da0: ldr r3, [r5, #4]
00392da4: ldm r3, {r2, r3}
00392da8: rsb r3, r2, r3
00392dac: asr r3, r3, #4
00392db0: add r2, r3, r3, lsl #3
00392db4: add r2, r2, r2, lsl #6
00392db8: add r2, r3, r2, lsl #3
00392dbc: add r2, r2, r2, lsl #15
00392dc0: add r3, r3, r2, lsl #3
00392dc4: rsb r3, r3, #0
00392dc8: cmp r7, r3
00392dcc: bhs #0x392cc4
00392dd0: mov r0, r5
00392dd4: mov r1, r7
00392dd8: bl #0x37baf8
00392ddc: ldr r3, [r0, #4]
00392de0: cmp r3, #4
00392de4: bne #0x392cc4
00392de8: mov r1, r7
00392dec: mov r0, r5
00392df0: bl #0x37baf8
00392df4: bl #0x31c49c
00392df8: add r6, r6, #0x304
00392dfc: mov r1, r0
00392e00: mov r0, r6
00392e04: bl #0x38f72c
00392e08: mov r1, #0
00392e0c: mov r4, r0
00392e10: mov r0, r5
00392e14: bl #0x37baf8
00392e18: bl #0x31bbf0
00392e1c: mov r1, #1
00392e20: mov r7, r0
00392e24: mov r0, r5
00392e28: bl #0x37baf8
00392e2c: bl #0x31bbf0
00392e30: mov r2, r7
00392e34: mov r3, r0
00392e38: add r1, sp, #0x2c
00392e3c: mov r0, r6
00392e40: str r4, [sp]
00392e44: bl #0x4a2e70
00392e48: b #0x392bdc
00392e4c: mov r0, r5
00392e50: mov r1, #2
00392e54: bl #0x37baf8
00392e58: ldr r3, [r0, #4]
00392e5c: cmp r3, #7
00392e60: beq #0x392f1c
00392e64: ldr r3, [r5, #4]
00392e68: ldr r2, [r3, #4]
00392e6c: ldr r3, [r3]
00392e70: rsb r3, r3, r2
00392e74: asr r3, r3, #4
00392e78: add r2, r3, r3, lsl #3
00392e7c: add r2, r2, r2, lsl #6
00392e80: add r2, r3, r2, lsl #3
00392e84: add r2, r2, r2, lsl #15
00392e88: add r3, r3, r2, lsl #3
00392e8c: rsb r3, r3, #0
00392e90: cmp r3, #4
00392e94: bls #0x392c9c
00392e98: mov r1, #2
00392e9c: mov r0, r5
00392ea0: bl #0x37baf8
00392ea4: ldr r1, [r0, #4]
00392ea8: cmp r1, #3
00392eac: beq #0x392fc4
00392eb0: ldr r3, [r5, #4]
00392eb4: ldr r2, [r3, #4]
00392eb8: ldr r3, [r3]
00392ebc: rsb r3, r3, r2
00392ec0: asr r3, r3, #4
00392ec4: add r2, r3, r3, lsl #3
00392ec8: add r2, r2, r2, lsl #6
00392ecc: add r2, r3, r2, lsl #3
00392ed0: add r2, r2, r2, lsl #15
00392ed4: add r3, r3, r2, lsl #3
00392ed8: rsb r3, r3, #0
00392edc: b #0x392c9c
00392ee0: mov r1, r3
00392ee4: ldr r3, [pc, #0x40c]
00392ee8: ldr r2, [pc, #0x414]
00392eec: mov r0, r5
00392ef0: ldr r3, [r4, r3]
00392ef4: ldr r2, [r4, r2]
00392ef8: ldr ip, [r3, #0x38]
00392efc: add r2, r2, #8
00392f00: str r2, [sp, #0x18]
00392f04: add r3, ip, #0x60
00392f08: str r3, [sp, #0x1c]
00392f0c: ldr r2, [ip, #0x60]
00392f10: str r3, [sp, #0x24]
00392f14: str r2, [sp, #0x20]
00392f18: b #0x392d00
00392f1c: mov r1, #2
00392f20: mov r0, r5
00392f24: bl #0x37baf8
00392f28: bl #0x31b5a0
00392f2c: ldr r2, [r0, #0x160]
00392f30: ldr r3, [r5, #4]
00392f34: str r2, [sp, #0x2c]
00392f38: ldr r2, [r0, #0x164]
00392f3c: str r2, [sp, #0x30]
00392f40: ldr r2, [r0, #0x168]
00392f44: str r2, [sp, #0x34]
00392f48: ldr r2, [r3, #4]
00392f4c: ldr r3, [r3]
00392f50: rsb r3, r3, r2
00392f54: asr r3, r3, #4
00392f58: add r2, r3, r3, lsl #3
00392f5c: add r2, r2, r2, lsl #6
00392f60: add r2, r3, r2, lsl #3
00392f64: add r2, r2, r2, lsl #15
00392f68: add r3, r3, r2, lsl #3
00392f6c: rsb r3, r3, #0
00392f70: b #0x392cbc
00392f74: mov r1, r7
00392f78: mov r0, r5
00392f7c: bl #0x37baf8
00392f80: bl #0x31bc80
00392f84: cmp r0, #0
00392f88: beq #0x392da0
00392f8c: ldr r1, [pc, #0x374]
00392f90: add r6, r6, #0x304
00392f94: mov r0, r6
00392f98: add r1, pc, r1
00392f9c: bl #0x38f72c
00392fa0: mov r1, #0
00392fa4: mov r4, r0
00392fa8: mov r0, r5
00392fac: bl #0x37baf8
00392fb0: bl #0x31bbf0
00392fb4: mov r1, r8
00392fb8: mov r7, r0
00392fbc: mov r0, r5
00392fc0: b #0x392e28
00392fc4: mov r0, r5
00392fc8: bl #0x37baf8
00392fcc: ldr r3, [r0, #4]
00392fd0: cmp r3, #3
00392fd4: bne #0x392eb0
00392fd8: mov r0, r5
00392fdc: mov r1, #4
00392fe0: bl #0x37baf8
00392fe4: ldr r3, [r0, #4]
00392fe8: cmp r3, #3
00392fec: beq #0x393014
00392ff0: ldr r2, [r5, #4]
00392ff4: movw r3, #0x6db7
00392ff8: movt r3, #0xb6db
00392ffc: ldr r1, [r2, #4]
00393000: ldr r2, [r2]
00393004: rsb r2, r2, r1
00393008: asr r2, r2, #4
0039300c: mul r3, r3, r2
00393010: b #0x392c9c
00393014: ldr r2, [r5, #4]
00393018: movw r3, #0x6db7
0039301c: movt r3, #0xb6db
00393020: ldm r2, {r1, r2}
00393024: rsb r2, r1, r2
00393028: asr r2, r2, #4
0039302c: mul r3, r3, r2
00393030: cmp r3, #5
00393034: bhi #0x393090
00393038: mov r1, #2
0039303c: mov r0, r5
00393040: bl #0x37baf8
00393044: bl #0x31bbf0
00393048: mov r1, #3
0039304c: mov r8, r0
00393050: mov r0, r5
00393054: bl #0x37baf8
00393058: bl #0x31bbf0
0039305c: mov r1, #4
00393060: mov r7, r0
00393064: mov r0, r5
00393068: bl #0x37baf8
0039306c: bl #0x31bbf0
00393070: ldr r3, [r5, #4]
00393074: str r7, [sp, #0x30]
00393078: str r8, [sp, #0x2c]
0039307c: str r0, [sp, #0x34]
00393080: ldr r2, [r3, #4]
00393084: mov r7, #6
00393088: ldr r3, [r3]
0039308c: b #0x392f50
00393090: mov r0, r5
00393094: mov r1, #5
00393098: bl #0x37baf8
0039309c: ldr r3, [r0, #4]
003930a0: cmp r3, #1
003930a4: bne #0x393038
003930a8: mov r1, #5
003930ac: mov r0, r5
003930b0: bl #0x37baf8
003930b4: bl #0x31bc80
003930b8: cmp r0, #0
003930bc: beq #0x393038
003930c0: mov r3, #0
003930c4: mov r0, r6
003930c8: add r1, sp, #0x18
003930cc: str r3, [sp, #0x20]
003930d0: str r3, [sp, #0x18]
003930d4: str r3, [sp, #0x1c]
003930d8: bl #0x393ae4
003930dc: ldr r3, [pc, #0x228]
003930e0: ldr ip, [r6, #0x160]
003930e4: ldr r2, [r6, #0x164]
003930e8: ldr r7, [r4, r3]
003930ec: ldr r3, [r6, #0x168]
003930f0: mov r1, #2
003930f4: mov r0, r5
003930f8: str r3, [sp, #0x34]
003930fc: ldr r3, [r7, #4]
00393100: str ip, [sp, #0x2c]
00393104: str r2, [sp, #0x30]
00393108: str r3, [sp, #0xc]
0039310c: ldr r3, [sp, #0x1c]
00393110: ldr fp, [r7, #8]
00393114: ldr sl, [r7]
00393118: str r3, [sp, #0x10]
0039311c: ldr r3, [sp, #0x18]
00393120: ldr sb, [sp, #0x20]
00393124: str r3, [sp, #0x14]
00393128: bl #0x37baf8
0039312c: bl #0x31bbf0
00393130: mov r1, sb
00393134: mov r8, r0
00393138: ldr r0, [sp, #0xc]
0039313c: bl #0x30ed6c
00393140: ldr r1, [sp, #0x10]
00393144: mov r3, r0
00393148: mov r0, fp
0039314c: str r3, [sp, #8]
00393150: bl #0x30ed6c
00393154: ldr r3, [sp, #8]
00393158: mov r1, r0
0039315c: mov r0, r3
00393160: bl #0x30e3ac
00393164: mov r1, r0
00393168: mov r0, r8
0039316c: bl #0x30ed6c
00393170: mov r1, r0
00393174: ldr r0, [sp, #0x2c]
00393178: bl #0x30eba4
0039317c: ldr r1, [sp, #0x14]
00393180: str r0, [sp, #0x2c]
00393184: mov r0, fp
00393188: bl #0x30ed6c
0039318c: mov r1, sl
00393190: mov fp, r0
00393194: mov r0, sb
00393198: bl #0x30ed6c
0039319c: mov r1, r0
003931a0: mov r0, fp
003931a4: bl #0x30e3ac
003931a8: mov r1, r0
003931ac: mov r0, r8
003931b0: bl #0x30ed6c
003931b4: mov r1, r0
003931b8: ldr r0, [sp, #0x30]
003931bc: bl #0x30eba4
003931c0: mov r1, sl
003931c4: str r0, [sp, #0x30]
003931c8: ldr r0, [sp, #0x10]
003931cc: bl #0x30ed6c
003931d0: ldr r1, [sp, #0x14]
003931d4: mov sl, r0
003931d8: ldr r0, [sp, #0xc]
003931dc: bl #0x30ed6c
003931e0: mov r1, r0
003931e4: mov r0, sl
003931e8: bl #0x30e3ac
003931ec: mov r1, r0
003931f0: mov r0, r8
003931f4: bl #0x30ed6c
003931f8: mov r1, r0
003931fc: ldr r0, [sp, #0x34]
00393200: bl #0x30eba4
00393204: mov r1, #3
00393208: str r0, [sp, #0x34]
0039320c: mov r0, r5
00393210: bl #0x37baf8
00393214: bl #0x31bbf0
00393218: ldr r1, [sp, #0x1c]
0039321c: mov r8, r0
00393220: bl #0x30ed6c
00393224: ldr r1, [sp, #0x20]
00393228: mov sb, r0
0039322c: mov r0, r8
00393230: bl #0x30ed6c
00393234: ldr r1, [sp, #0x18]
00393238: mov sl, r0
0039323c: mov r0, r8
00393240: bl #0x30ed6c
00393244: mov r1, r0
00393248: ldr r0, [sp, #0x2c]
0039324c: bl #0x30eba4
00393250: mov r1, sb
00393254: str r0, [sp, #0x2c]
00393258: ldr r0, [sp, #0x30]
0039325c: bl #0x30eba4
00393260: mov r1, sl
00393264: str r0, [sp, #0x30]
00393268: ldr r0, [sp, #0x34]
0039326c: bl #0x30eba4
00393270: mov r1, #4
00393274: str r0, [sp, #0x34]
00393278: mov r0, r5
0039327c: bl #0x37baf8
00393280: bl #0x31bbf0
00393284: ldr r1, [r7, #4]
00393288: mov r8, r0
0039328c: bl #0x30ed6c
00393290: ldr r1, [r7, #8]
00393294: mov sb, r0
00393298: mov r0, r8
0039329c: bl #0x30ed6c
003932a0: ldr r1, [r7]
003932a4: mov sl, r0
003932a8: mov r0, r8
003932ac: bl #0x30ed6c
003932b0: mov r1, r0
003932b4: ldr r0, [sp, #0x2c]
003932b8: bl #0x30eba4
003932bc: mov r1, sb
003932c0: str r0, [sp, #0x2c]
003932c4: ldr r0, [sp, #0x30]
003932c8: bl #0x30eba4
003932cc: mov r1, sl
003932d0: str r0, [sp, #0x30]
003932d4: ldr r0, [sp, #0x34]
003932d8: bl #0x30eba4
003932dc: mov r7, #6
003932e0: ldr r3, [r5, #4]
003932e4: str r0, [sp, #0x34]
003932e8: b #0x392f48

_ZN9Character7_WarpToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3bb268 1216
003bb268: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003bb26c: ldr r6, [r0, #4]
003bb270: mov r5, r2
003bb274: ldr r7, [pc, #0x4a0]
003bb278: ldm r6, {r1, r3}
003bb27c: add r7, pc, r7
003bb280: sub sp, sp, #0x2c
003bb284: rsb r3, r1, r3
003bb288: asr r3, r3, #4
003bb28c: mov r4, r0
003bb290: add r2, r3, r3, lsl #3
003bb294: add r2, r2, r2, lsl #6
003bb298: add r2, r3, r2, lsl #3
003bb29c: add r2, r2, r2, lsl #15
003bb2a0: add r3, r3, r2, lsl #3
003bb2a4: rsb r3, r3, #0
003bb2a8: cmp r3, #1
003bb2ac: beq #0x3bb3b0
003bb2b0: cmp r3, #2
003bb2b4: bls #0x3bb3a8
003bb2b8: cmp r3, #0
003bb2bc: bne #0x3bb2d0
003bb2c0: ldr r0, [pc, #0x458]
003bb2c4: add r0, pc, r0
003bb2c8: bl #0x708eb0
003bb2cc: ldr r1, [r6]
003bb2d0: ldr r3, [r1, #4]
003bb2d4: cmp r3, #3
003bb2d8: bne #0x3bb414
003bb2dc: ldr r2, [r4, #4]
003bb2e0: ldr r3, [r2]
003bb2e4: ldr r1, [r2, #4]
003bb2e8: rsb r1, r3, r1
003bb2ec: asr r1, r1, #4
003bb2f0: add r3, r1, r1, lsl #3
003bb2f4: add r3, r3, r3, lsl #6
003bb2f8: add r3, r1, r3, lsl #3
003bb2fc: add r3, r3, r3, lsl #15
003bb300: add r3, r1, r3, lsl #3
003bb304: rsb r3, r3, #0
003bb308: cmp r3, #1
003bb30c: beq #0x3bb6f8
003bb310: cmp r3, #2
003bb314: bls #0x3bb3a8
003bb318: mov r8, #0
003bb31c: str r8, [sp, #0x1c]
003bb320: str r8, [sp, #0x20]
003bb324: str r8, [sp, #0x24]
003bb328: ldr r3, [r2]
003bb32c: ldr r2, [r2, #4]
003bb330: rsb r3, r3, r2
003bb334: asr r3, r3, #4
003bb338: add r2, r3, r3, lsl #3
003bb33c: add r2, r2, r2, lsl #6
003bb340: add r2, r3, r2, lsl #3
003bb344: add r2, r2, r2, lsl #15
003bb348: add r3, r3, r2, lsl #3
003bb34c: rsb r3, r3, #0
003bb350: cmp r3, #3
003bb354: bhi #0x3bb4a8
003bb358: mov r1, #0
003bb35c: mov r0, r4
003bb360: bl #0x37baf8
003bb364: bl #0x31bbf0
003bb368: mov r1, #1
003bb36c: mov r7, r0
003bb370: mov r0, r4
003bb374: bl #0x37baf8
003bb378: bl #0x31bbf0
003bb37c: mov r1, #2
003bb380: mov r6, r0
003bb384: mov r0, r4
003bb388: bl #0x37baf8
003bb38c: bl #0x31bbf0
003bb390: str r7, [sp, #0x1c]
003bb394: str r6, [sp, #0x20]
003bb398: str r0, [sp, #0x24]
003bb39c: ldr r0, [r5, #0x378]
003bb3a0: add r1, sp, #0x1c
003bb3a4: bl #0x405318
003bb3a8: add sp, sp, #0x2c
003bb3ac: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bb3b0: mov r1, #0
003bb3b4: bl #0x37baf8
003bb3b8: ldr r3, [r0, #4]
003bb3bc: cmp r3, #2
003bb3c0: beq #0x3bb474
003bb3c4: mov r0, r4
003bb3c8: mov r1, #0
003bb3cc: bl #0x37baf8
003bb3d0: ldr r3, [r0, #4]
003bb3d4: cmp r3, #7
003bb3d8: bne #0x3bb3a8
003bb3dc: ldr r2, [r4, #4]
003bb3e0: ldm r2, {r1, r3}
003bb3e4: mov r6, r2
003bb3e8: rsb r3, r1, r3
003bb3ec: asr r3, r3, #4
003bb3f0: add r0, r3, r3, lsl #3
003bb3f4: add r0, r0, r0, lsl #6
003bb3f8: add r0, r3, r0, lsl #3
003bb3fc: add r0, r0, r0, lsl #15
003bb400: add r3, r3, r0, lsl #3
003bb404: rsb r3, r3, #0
003bb408: cmp r3, #2
003bb40c: bls #0x3bb308
003bb410: b #0x3bb2b8
003bb414: mov r0, r4
003bb418: mov r1, #1
003bb41c: bl #0x37baf8
003bb420: ldr r3, [r0, #4]
003bb424: cmp r3, #3
003bb428: beq #0x3bb444
003bb42c: mov r0, r4
003bb430: mov r1, #2
003bb434: bl #0x37baf8
003bb438: ldr r3, [r0, #4]
003bb43c: cmp r3, #3
003bb440: bne #0x3bb3a8
003bb444: ldr r2, [r4, #4]
003bb448: ldr r1, [r2, #4]
003bb44c: ldr r3, [r2]
003bb450: rsb r3, r3, r1
003bb454: asr r3, r3, #4
003bb458: add r1, r3, r3, lsl #3
003bb45c: add r1, r1, r1, lsl #6
003bb460: add r1, r3, r1, lsl #3
003bb464: add r1, r1, r1, lsl #15
003bb468: add r3, r3, r1, lsl #3
003bb46c: rsb r3, r3, #0
003bb470: b #0x3bb308
003bb474: ldr r2, [r4, #4]
003bb478: ldr r1, [r2]
003bb47c: ldr r0, [r2, #4]
003bb480: mov r6, r2
003bb484: rsb r0, r1, r0
003bb488: asr r0, r0, #4
003bb48c: add r3, r0, r0, lsl #3
003bb490: add r3, r3, r3, lsl #6
003bb494: add r3, r0, r3, lsl #3
003bb498: add r3, r3, r3, lsl #15
003bb49c: add r3, r0, r3, lsl #3
003bb4a0: rsb r3, r3, #0
003bb4a4: b #0x3bb408
003bb4a8: mov r0, r4
003bb4ac: mov r1, #3
003bb4b0: bl #0x37baf8
003bb4b4: ldr r6, [r0, #4]
003bb4b8: cmp r6, #1
003bb4bc: bne #0x3bb358
003bb4c0: mov r1, #3
003bb4c4: mov r0, r4
003bb4c8: bl #0x37baf8
003bb4cc: bl #0x31bc80
003bb4d0: cmp r0, #0
003bb4d4: beq #0x3bb358
003bb4d8: mov r0, r5
003bb4dc: add r1, sp, #0x10
003bb4e0: str r8, [sp, #0x18]
003bb4e4: str r8, [sp, #0x10]
003bb4e8: str r8, [sp, #0x14]
003bb4ec: bl #0x393ae4
003bb4f0: ldr r3, [pc, #0x22c]
003bb4f4: ldr ip, [r5, #0x160]
003bb4f8: ldr r2, [r5, #0x164]
003bb4fc: ldr r7, [r7, r3]
003bb500: ldr r3, [r5, #0x168]
003bb504: mov r1, #0
003bb508: mov r0, r4
003bb50c: str r3, [sp, #0x24]
003bb510: ldr r3, [r7, #4]
003bb514: str ip, [sp, #0x1c]
003bb518: str r2, [sp, #0x20]
003bb51c: str r3, [sp, #4]
003bb520: ldr r3, [sp, #0x14]
003bb524: ldr fp, [r7, #8]
003bb528: ldr sl, [r7]
003bb52c: str r3, [sp, #8]
003bb530: ldr r3, [sp, #0x10]
003bb534: ldr sb, [sp, #0x18]
003bb538: str r3, [sp, #0xc]
003bb53c: bl #0x37baf8
003bb540: bl #0x31bbf0
003bb544: mov r1, sb
003bb548: mov r8, r0
003bb54c: ldr r0, [sp, #4]
003bb550: bl #0x30ed6c
003bb554: ldr r1, [sp, #8]
003bb558: mov r3, r0
003bb55c: mov r0, fp
003bb560: str r3, [sp]
003bb564: bl #0x30ed6c
003bb568: ldr r3, [sp]
003bb56c: mov r1, r0
003bb570: mov r0, r3
003bb574: bl #0x30e3ac
003bb578: mov r1, r0
003bb57c: mov r0, r8
003bb580: bl #0x30ed6c
003bb584: mov r1, r0
003bb588: ldr r0, [sp, #0x1c]
003bb58c: bl #0x30eba4
003bb590: ldr r1, [sp, #0xc]
003bb594: str r0, [sp, #0x1c]
003bb598: mov r0, fp
003bb59c: bl #0x30ed6c
003bb5a0: mov r1, sl
003bb5a4: mov fp, r0
003bb5a8: mov r0, sb
003bb5ac: bl #0x30ed6c
003bb5b0: mov r1, r0
003bb5b4: mov r0, fp
003bb5b8: bl #0x30e3ac
003bb5bc: mov r1, r0
003bb5c0: mov r0, r8
003bb5c4: bl #0x30ed6c
003bb5c8: mov r1, r0
003bb5cc: ldr r0, [sp, #0x20]
003bb5d0: bl #0x30eba4
003bb5d4: mov r1, sl
003bb5d8: str r0, [sp, #0x20]
003bb5dc: ldr r0, [sp, #8]
003bb5e0: bl #0x30ed6c
003bb5e4: ldr r1, [sp, #0xc]
003bb5e8: mov sl, r0
003bb5ec: ldr r0, [sp, #4]
003bb5f0: bl #0x30ed6c
003bb5f4: mov r1, r0
003bb5f8: mov r0, sl
003bb5fc: bl #0x30e3ac
003bb600: mov r1, r0
003bb604: mov r0, r8
003bb608: bl #0x30ed6c
003bb60c: mov r1, r0
003bb610: ldr r0, [sp, #0x24]
003bb614: bl #0x30eba4
003bb618: mov r1, r6
003bb61c: str r0, [sp, #0x24]
003bb620: mov r0, r4
003bb624: bl #0x37baf8
003bb628: bl #0x31bbf0
003bb62c: ldr r1, [sp, #0x14]
003bb630: mov r6, r0
003bb634: bl #0x30ed6c
003bb638: ldr r1, [sp, #0x18]
003bb63c: mov sl, r0
003bb640: mov r0, r6
003bb644: bl #0x30ed6c
003bb648: ldr r1, [sp, #0x10]
003bb64c: mov r8, r0
003bb650: mov r0, r6
003bb654: bl #0x30ed6c
003bb658: mov r1, r0
003bb65c: ldr r0, [sp, #0x1c]
003bb660: bl #0x30eba4
003bb664: mov r1, sl
003bb668: str r0, [sp, #0x1c]
003bb66c: ldr r0, [sp, #0x20]
003bb670: bl #0x30eba4
003bb674: mov r1, r8
003bb678: str r0, [sp, #0x20]
003bb67c: ldr r0, [sp, #0x24]
003bb680: bl #0x30eba4
003bb684: mov r1, #2
003bb688: str r0, [sp, #0x24]
003bb68c: mov r0, r4
003bb690: bl #0x37baf8
003bb694: bl #0x31bbf0
003bb698: ldr r1, [r7, #4]
003bb69c: mov r4, r0
003bb6a0: bl #0x30ed6c
003bb6a4: ldr r1, [r7, #8]
003bb6a8: mov r8, r0
003bb6ac: mov r0, r4
003bb6b0: bl #0x30ed6c
003bb6b4: ldr r1, [r7]
003bb6b8: mov r6, r0
003bb6bc: mov r0, r4
003bb6c0: bl #0x30ed6c
003bb6c4: mov r1, r0
003bb6c8: ldr r0, [sp, #0x1c]
003bb6cc: bl #0x30eba4
003bb6d0: mov r1, r8
003bb6d4: str r0, [sp, #0x1c]
003bb6d8: ldr r0, [sp, #0x20]
003bb6dc: bl #0x30eba4
003bb6e0: mov r1, r6
003bb6e4: str r0, [sp, #0x20]
003bb6e8: ldr r0, [sp, #0x24]
003bb6ec: bl #0x30eba4
003bb6f0: str r0, [sp, #0x24]
003bb6f4: b #0x3bb39c
003bb6f8: mov r1, #0
003bb6fc: mov r0, r4
003bb700: ldr r4, [r5, #0x378]
003bb704: bl #0x37baf8
003bb708: bl #0x31b5a0
003bb70c: add r1, r0, #0x160
003bb710: mov r0, r4
003bb714: bl #0x405318
003bb718: b #0x3bb3a8
003bb71c: subseq sb, sp, r4, lsl r8
003bb720: subseq r3, r0, r4, lsr #3
003bb724: andeq r4, r0, r0, asr #6

_ZN3sfc6script3lua6Binder12bindFunctionEPKcPFvRKNS1_9ArgumentsERNS1_12ReturnValuesEPvESA_ 0x31a4d4 344
0031a4d4: push {r4, r5, r6, r7, r8, sl, lr}
0031a4d8: mov r6, r0
0031a4dc: ldr r0, [r0, #4]
0031a4e0: ldr r5, [pc, #0x11c]
0031a4e4: sub sp, sp, #0x14
0031a4e8: cmp r0, #0
0031a4ec: add r5, pc, r5
0031a4f0: mov r8, r1
0031a4f4: mov r7, r2
0031a4f8: mov sl, r3
0031a4fc: beq #0x31a554
0031a500: cmp r1, #0
0031a504: beq #0x31a55c
0031a508: cmp r7, #0
0031a50c: beq #0x31a5b0
0031a510: add r4, sp, #8
0031a514: mov r0, r4
0031a518: bl #0x3192b4
0031a51c: mov r0, r4
0031a520: mov r1, r7
0031a524: bl #0x31a46c
0031a528: mov r0, r4
0031a52c: mov r1, sl
0031a530: bl #0x31a46c
0031a534: ldr r3, [pc, #0xcc]
0031a538: ldr r0, [r6, #4]
0031a53c: mov r1, r8
0031a540: ldr r2, [r5, r3]
0031a544: mov r3, r4
0031a548: bl #0x31af08
0031a54c: mov r0, r4
0031a550: bl #0x319228
0031a554: add sp, sp, #0x14
0031a558: pop {r4, r5, r6, r7, r8, sl, pc}
0031a55c: ldr r3, [pc, #0xa8]
0031a560: ldr r3, [r5, r3]
0031a564: ldr r3, [r3]
0031a568: cmp r3, #2
0031a56c: streq r1, [r1]
0031a570: beq #0x31a508
0031a574: cmp r3, #1
0031a578: bne #0x31a508
0031a57c: ldr r0, [pc, #0x8c]
0031a580: ldr r1, [pc, #0x8c]
0031a584: ldr r2, [pc, #0x8c]
0031a588: ldr r0, [r5, r0]
0031a58c: ldr r3, [pc, #0x88]
0031a590: mov ip, #0x79
0031a594: add r1, pc, r1
0031a598: add r2, pc, r2
0031a59c: add r3, pc, r3
0031a5a0: add r0, r0, #0xa8
0031a5a4: str ip, [sp]
0031a5a8: bl #0x30e004
0031a5ac: b #0x31a508
0031a5b0: ldr r3, [pc, #0x54]
0031a5b4: ldr r3, [r5, r3]
0031a5b8: ldr r3, [r3]
0031a5bc: cmp r3, #2
0031a5c0: streq r7, [r7]
0031a5c4: beq #0x31a510
0031a5c8: cmp r3, #1
0031a5cc: bne #0x31a510
0031a5d0: ldr r0, [pc, #0x38]
0031a5d4: ldr r1, [pc, #0x44]
0031a5d8: ldr r2, [pc, #0x44]
0031a5dc: ldr r0, [r5, r0]
0031a5e0: ldr r3, [pc, #0x40]
0031a5e4: mov ip, #0x7a
0031a5e8: add r1, pc, r1
0031a5ec: add r2, pc, r2
0031a5f0: add r3, pc, r3
0031a5f4: add r0, r0, #0xa8
0031a5f8: str ip, [sp]
0031a5fc: bl #0x30e004
0031a600: b #0x31a510
0031a604: rsbeq sl, r7, r4, lsr #11
0031a608: andeq r2, r0, r0, asr r4
0031a60c: andeq r3, r0, r0, asr #19
0031a610: andeq r1, r0, r0, asr #19
0031a614: subseq r3, sl, r4, asr #28
0031a618: subseq r4, sl, r0, lsr r2
0031a61c: subseq r4, sl, ip, lsr r2
0031a620: ldrsheq r3, [sl], #-0xd0
0031a624: subseq r4, sl, r4, lsr r2
0031a628: subseq r4, sl, r8, ror #3

_ZN9LuaScript4CallEPKc 0x37c514 112
0037c514: ldr r3, [pc, #0x60]
0037c518: ldr r2, [pc, #0x60]
0037c51c: push {r4, r5, r6, r7, lr}
0037c520: add r3, pc, r3
0037c524: ldr r5, [r3, r2]
0037c528: sub sp, sp, #0x34
0037c52c: add r4, sp, #4
0037c530: ldr r2, [r5]
0037c534: mov r6, r0
0037c538: mov r7, r1
0037c53c: mov r0, r4
0037c540: str r2, [sp, #0x2c]
0037c544: bl #0x31b434
0037c548: mov r2, r4
0037c54c: mov r0, r6
0037c550: mov r1, r7
0037c554: bl #0x37c494
0037c558: mov r0, r4
0037c55c: bl #0x31b398
0037c560: ldr r2, [sp, #0x2c]
0037c564: ldr r3, [r5]
0037c568: cmp r2, r3
0037c56c: bne #0x37c578
0037c570: add sp, sp, #0x34
0037c574: pop {r4, r5, r6, r7, pc}
0037c578: bl #0x30e310
0037c57c: rsbeq r8, r1, r0, ror r5
0037c580: andeq r4, r0, ip, lsr #1

_ZN10GameObject17_TargetListBackupERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390bd4 124
00390bd4: str lr, [sp, #-4]!
00390bd8: ldr r3, [r0, #4]
00390bdc: sub sp, sp, #0xc
00390be0: ldm r3, {r0, r1}
00390be4: rsb r3, r0, r1
00390be8: asr r3, r3, #4
00390bec: add r1, r3, r3, lsl #3
00390bf0: add r1, r1, r1, lsl #6
00390bf4: add r1, r3, r1, lsl #3
00390bf8: add r1, r1, r1, lsl #15
00390bfc: add r3, r3, r1, lsl #3
00390c00: cmp r3, #0
00390c04: bne #0x390c20
00390c08: ldr r1, [pc, #0x3c]
00390c0c: add r0, r2, #0x304
00390c10: add r1, pc, r1
00390c14: add sp, sp, #0xc
00390c18: pop {lr}
00390c1c: b #0x4a36b0
00390c20: ldr r3, [r0, #4]
00390c24: cmp r3, #4
00390c28: bne #0x390c08
00390c2c: str r2, [sp, #4]
00390c30: bl #0x31c49c
00390c34: ldr r2, [sp, #4]
00390c38: mov r1, r0
00390c3c: add r0, r2, #0x304
00390c40: add sp, sp, #0xc
00390c44: pop {lr}
00390c48: b #0x4a36b0
00390c4c: ldrheq pc, [r2], #-0xc0

_ZN3sfc6script3lua5ValueD1Ev 0x3193e8 108
003193e8: ldr r3, [pc, #0x5c]
003193ec: ldr r2, [pc, #0x5c]
003193f0: push {r4, lr}
003193f4: add r3, pc, r3
003193f8: ldr r2, [r3, r2]
003193fc: mov r4, r0
00319400: add r2, r2, #8
00319404: str r2, [r0], #0x24
00319408: bl #0x3193b0
0031940c: add r3, r4, #0xc
00319410: ldr r0, [r3, #0x14]
00319414: cmp r0, r3
00319418: beq #0x319438
0031941c: cmp r0, #0
00319420: beq #0x319438
00319424: ldr r1, [r4, #0xc]
00319428: rsb r1, r0, r1
0031942c: cmp r1, #0x80
00319430: bhi #0x319440
00319434: bl #0x708f00
00319438: mov r0, r4
0031943c: pop {r4, pc}
00319440: bl #0x310440
00319444: mov r0, r4
00319448: pop {r4, pc}
0031944c: mlseq r7, ip, r6, fp
00319450: muleq r0, r8, r7

_ZN3sfc6script3lua5Value6setNilEv 0x31b5c0 12
0031b5c0: mov r3, #0
0031b5c4: str r3, [r0, #4]
0031b5c8: bx lr

_ZNKSt4priv20_Deque_iterator_baseIPSt6vectorIN3sfc6script3lua5ValueESaIS5_EEE11_M_subtractERKS9_ 0x31b618 68
0031b618: push {r4, r5}
0031b61c: ldr ip, [r1, #0xc]
0031b620: ldr r5, [r0]
0031b624: ldr r2, [r0, #4]
0031b628: ldr r4, [r0, #0xc]
0031b62c: ldr r3, [r1, #8]
0031b630: ldr r1, [r1]
0031b634: rsb r2, r2, r5
0031b638: rsb r0, ip, r4
0031b63c: rsb r3, r1, r3
0031b640: asr r2, r2, #2
0031b644: asr r0, r0, #2
0031b648: add r3, r2, r3, asr #2
0031b64c: sub r0, r0, #1
0031b650: add r0, r3, r0, lsl #5
0031b654: pop {r4, r5}
0031b658: bx lr

_ZN9Character10_StopTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7064 116
003b7064: str lr, [sp, #-4]!
003b7068: ldr r3, [r0, #4]
003b706c: sub sp, sp, #0xc
003b7070: ldr r1, [r3, #4]
003b7074: ldr ip, [r3]
003b7078: rsb r3, ip, r1
003b707c: asr r3, r3, #4
003b7080: add r1, r3, r3, lsl #3
003b7084: add r1, r1, r1, lsl #6
003b7088: add r1, r3, r1, lsl #3
003b708c: add r1, r1, r1, lsl #15
003b7090: add r3, r3, r1, lsl #3
003b7094: cmp r3, #0
003b7098: bne #0x3b70a4
003b709c: add sp, sp, #0xc
003b70a0: ldm sp!, {pc}
003b70a4: ldr r3, [ip, #4]
003b70a8: cmp r3, #3
003b70ac: bne #0x3b709c
003b70b0: mov r1, #0
003b70b4: str r2, [sp, #4]
003b70b8: bl #0x37baf8
003b70bc: bl #0x38d798
003b70c0: ldr r2, [sp, #4]
003b70c4: mov r1, r0
003b70c8: add r0, r2, #0x3b4
003b70cc: add sp, sp, #0xc
003b70d0: pop {lr}
003b70d4: b #0x3db2d8

_ZN9Character5_FleeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b91a0 260
003b91a0: push {r4, r5, r6, r7, r8, sl, lr}
003b91a4: ldr r3, [r0, #4]
003b91a8: mov r4, r2
003b91ac: sub sp, sp, #0x14
003b91b0: ldm r3, {r0, r2}
003b91b4: rsb r3, r0, r2
003b91b8: asr r3, r3, #4
003b91bc: add r2, r3, r3, lsl #3
003b91c0: add r2, r2, r2, lsl #6
003b91c4: add r2, r3, r2, lsl #3
003b91c8: add r2, r2, r2, lsl #15
003b91cc: add r3, r3, r2, lsl #3
003b91d0: cmp r3, #0
003b91d4: bne #0x3b91e0
003b91d8: add sp, sp, #0x14
003b91dc: pop {r4, r5, r6, r7, r8, sl, pc}
003b91e0: ldr r3, [r0, #4]
003b91e4: cmp r3, #2
003b91e8: beq #0x3b91f4
003b91ec: cmp r3, #7
003b91f0: bne #0x3b91d8
003b91f4: bl #0x31b5a0
003b91f8: ldr r3, [r4, #0x408]
003b91fc: cmp r3, #0
003b9200: beq #0x3b91d8
003b9204: mov r0, r4
003b9208: bl #0x3935dc
003b920c: mov r5, r0
003b9210: ldr r0, [r4, #0x408]
003b9214: bl #0x3935dc
003b9218: mov r6, r0
003b921c: ldr r1, [r0]
003b9220: ldr r0, [r5]
003b9224: bl #0x30e3ac
003b9228: ldr r1, [r6, #4]
003b922c: mov r8, r0
003b9230: ldr r0, [r5, #4]
003b9234: bl #0x30e3ac
003b9238: ldr r1, [r6, #8]
003b923c: mov sl, r0
003b9240: ldr r0, [r5, #8]
003b9244: bl #0x30e3ac
003b9248: mov r5, r0
003b924c: mov r0, r4
003b9250: ldr r7, [r4, #0x378]
003b9254: bl #0x3935dc
003b9258: mov r4, r0
003b925c: ldr r1, [r4, #4]
003b9260: mov r0, sl
003b9264: bl #0x30eba4
003b9268: ldr r1, [r4, #8]
003b926c: mov r6, r0
003b9270: mov r0, r5
003b9274: bl #0x30eba4
003b9278: ldr r1, [r4]
003b927c: mov r5, r0
003b9280: mov r0, r8
003b9284: bl #0x30eba4
003b9288: add r1, sp, #4
003b928c: str r0, [sp, #4]
003b9290: mov r0, r7
003b9294: str r6, [sp, #8]
003b9298: str r5, [sp, #0xc]
003b929c: bl #0x4054e4
003b92a0: b #0x3b91d8

_ZN9Character8_SetPropERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7eac 492
003b7eac: push {r4, r5, r6, r7, r8, lr}
003b7eb0: ldr r5, [r0, #4]
003b7eb4: mov r6, r2
003b7eb8: mov r4, r0
003b7ebc: ldm r5, {r1, r3}
003b7ec0: rsb r3, r1, r3
003b7ec4: asr r3, r3, #4
003b7ec8: add r2, r3, r3, lsl #3
003b7ecc: add r2, r2, r2, lsl #6
003b7ed0: add r2, r3, r2, lsl #3
003b7ed4: add r2, r2, r2, lsl #15
003b7ed8: add r3, r3, r2, lsl #3
003b7edc: rsb r3, r3, #0
003b7ee0: cmp r3, #1
003b7ee4: bls #0x3b7efc
003b7ee8: cmp r3, #0
003b7eec: beq #0x3b7f00
003b7ef0: ldr r3, [r1, #4]
003b7ef4: cmp r3, #3
003b7ef8: beq #0x3b7f14
003b7efc: pop {r4, r5, r6, r7, r8, pc}
003b7f00: ldr r0, [pc, #0x188]
003b7f04: add r0, pc, r0
003b7f08: bl #0x708eb0
003b7f0c: ldr r1, [r5]
003b7f10: b #0x3b7ef0
003b7f14: mov r1, #0
003b7f18: mov r0, r4
003b7f1c: bl #0x37baf8
003b7f20: bl #0x38d798
003b7f24: mov r1, #0
003b7f28: mov r0, r4
003b7f2c: bl #0x37baf8
003b7f30: bl #0x38d798
003b7f34: cmp r0, #0xdf
003b7f38: bhi #0x3b7efc
003b7f3c: ldr r5, [r4, #4]
003b7f40: ldr r3, [r5]
003b7f44: ldr r2, [r5, #4]
003b7f48: rsb r2, r3, r2
003b7f4c: asr r2, r2, #4
003b7f50: add r1, r2, r2, lsl #3
003b7f54: add r1, r1, r1, lsl #6
003b7f58: add r1, r2, r1, lsl #3
003b7f5c: add r1, r1, r1, lsl #15
003b7f60: add r2, r2, r1, lsl #3
003b7f64: rsb r2, r2, #0
003b7f68: cmp r2, #1
003b7f6c: bhi #0x3b7f80
003b7f70: ldr r0, [pc, #0x11c]
003b7f74: add r0, pc, r0
003b7f78: bl #0x708eb0
003b7f7c: ldr r3, [r5]
003b7f80: ldr r3, [r3, #0x74]
003b7f84: cmp r3, #3
003b7f88: bne #0x3b7efc
003b7f8c: ldr r2, [r4, #4]
003b7f90: ldr r3, [r2]
003b7f94: ldr r2, [r2, #4]
003b7f98: rsb r3, r3, r2
003b7f9c: asr r3, r3, #4
003b7fa0: add r2, r3, r3, lsl #3
003b7fa4: add r2, r2, r2, lsl #6
003b7fa8: add r2, r3, r2, lsl #3
003b7fac: add r2, r2, r2, lsl #15
003b7fb0: add r3, r3, r2, lsl #3
003b7fb4: rsb r3, r3, #0
003b7fb8: cmp r3, #2
003b7fbc: bhi #0x3b8000
003b7fc0: mov r1, #0
003b7fc4: mov r0, r4
003b7fc8: bl #0x37baf8
003b7fcc: bl #0x38d798
003b7fd0: mov r1, #1
003b7fd4: mov r5, r0
003b7fd8: mov r0, r4
003b7fdc: bl #0x37baf8
003b7fe0: bl #0x31bbf0
003b7fe4: bl #0x30e4cc
003b7fe8: add r6, r6, #0x560
003b7fec: mov r2, r0
003b7ff0: mov r1, r5
003b7ff4: mov r0, r6
003b7ff8: pop {r4, r5, r6, r7, r8, lr}
003b7ffc: b #0x3e07a0
003b8000: mov r0, r4
003b8004: mov r1, #2
003b8008: bl #0x37baf8
003b800c: ldr r5, [r0, #4]
003b8010: cmp r5, #2
003b8014: bne #0x3b7fc0
003b8018: mov r1, r5
003b801c: mov r0, r4
003b8020: bl #0x37baf8
003b8024: bl #0x31b580
003b8028: cmp r0, #0
003b802c: beq #0x3b7efc
003b8030: mov r1, #0
003b8034: mov r0, r4
003b8038: bl #0x37baf8
003b803c: bl #0x38d798
003b8040: mov r1, #1
003b8044: mov r7, r0
003b8048: mov r0, r4
003b804c: bl #0x37baf8
003b8050: bl #0x31bbf0
003b8054: mov r1, r5
003b8058: mov r8, r0
003b805c: mov r0, r4
003b8060: bl #0x37baf8
003b8064: bl #0x31b580
003b8068: mov r4, r0
003b806c: mov r0, r8
003b8070: bl #0x30e4cc
003b8074: add r6, r6, #0x560
003b8078: mov r2, r0
003b807c: mov r1, r7
003b8080: mov r0, r6
003b8084: mov r3, r4
003b8088: pop {r4, r5, r6, r7, r8, lr}
003b808c: b #0x3e0614
003b8090: subseq r6, r0, r4, ror #10
003b8094: ldrsheq r6, [r0], #-0x44

_ZN3sfc6script3lua9Arguments7pushNilEv 0x39eba8 104
0039eba8: ldr r3, [pc, #0x58]
0039ebac: ldr r2, [pc, #0x58]
0039ebb0: push {r4, r5, r6, lr}
0039ebb4: add r3, pc, r3
0039ebb8: ldr r5, [r3, r2]
0039ebbc: sub sp, sp, #0x78
0039ebc0: add r4, sp, #4
0039ebc4: ldr r3, [r5]
0039ebc8: str r3, [sp, #0x74]
0039ebcc: ldr r6, [r0, #4]
0039ebd0: mov r0, r4
0039ebd4: bl #0x3194e0
0039ebd8: mov r0, r6
0039ebdc: mov r1, r4
0039ebe0: bl #0x3195c0
0039ebe4: mov r0, r4
0039ebe8: bl #0x3193e8
0039ebec: ldr r2, [sp, #0x74]
0039ebf0: ldr r3, [r5]
0039ebf4: cmp r2, r3
0039ebf8: bne #0x39ec04
0039ebfc: add sp, sp, #0x78
0039ec00: pop {r4, r5, r6, pc}
0039ec04: bl #0x30e310
0039ec08: ldrsbeq r5, [pc], #-0xec
0039ec0c: andeq r4, r0, ip, lsr #1

_ZN3sfc6script3lua8UserDataD1Ev 0x33dcb0 4
0033dcb0: bx lr

_ZN3sfc6script3lua5ErrorD2Ev 0x31a6c0 52
0031a6c0: ldr r3, [pc, #0x24]
0031a6c4: ldr r2, [pc, #0x24]
0031a6c8: push {r4, lr}
0031a6cc: add r3, pc, r3
0031a6d0: ldr r2, [r3, r2]
0031a6d4: mov r4, r0
0031a6d8: add r2, r2, #8
0031a6dc: str r2, [r0], #8
0031a6e0: bl #0x3139ac
0031a6e4: mov r0, r4
0031a6e8: pop {r4, pc}
0031a6ec: rsbeq sl, r7, r4, asr #7
0031a6f0: muleq r0, r8, r4

_ZN9Character14_SetScareStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b96b0 304
003b96b0: push {r4, r5, r6, lr}
003b96b4: ldr r3, [r0, #4]
003b96b8: sub sp, sp, #0x10
003b96bc: mov r4, r0
003b96c0: ldr r1, [r3, #4]
003b96c4: ldr ip, [r3]
003b96c8: rsb r3, ip, r1
003b96cc: asr r3, r3, #4
003b96d0: add r1, r3, r3, lsl #3
003b96d4: add r1, r1, r1, lsl #6
003b96d8: add r1, r3, r1, lsl #3
003b96dc: add r1, r1, r1, lsl #15
003b96e0: add r3, r3, r1, lsl #3
003b96e4: rsb r3, r3, #0
003b96e8: cmp r3, #0
003b96ec: bne #0x3b96f8
003b96f0: add sp, sp, #0x10
003b96f4: pop {r4, r5, r6, pc}
003b96f8: ldr r1, [ip, #4]
003b96fc: cmp r1, #3
003b9700: bne #0x3b96f0
003b9704: cmp r3, #1
003b9708: addls r5, r2, #0x4f0
003b970c: addls r5, r5, #0xc
003b9710: movls r4, #1
003b9714: bls #0x3b9784
003b9718: mov r1, #1
003b971c: str r2, [sp, #0xc]
003b9720: bl #0x37baf8
003b9724: ldr r5, [r0, #4]
003b9728: ldr r2, [sp, #0xc]
003b972c: cmp r5, #1
003b9730: beq #0x3b97b0
003b9734: ldr r6, [r4, #4]
003b9738: mov r4, #1
003b973c: ldr ip, [r6]
003b9740: ldr r3, [r6, #4]
003b9744: add r5, r2, #0x4f0
003b9748: add r5, r5, #0xc
003b974c: rsb r3, ip, r3
003b9750: asr r3, r3, #4
003b9754: add r1, r3, r3, lsl #3
003b9758: add r1, r1, r1, lsl #6
003b975c: add r1, r3, r1, lsl #3
003b9760: add r1, r1, r1, lsl #15
003b9764: add r3, r3, r1, lsl #3
003b9768: rsb r3, r3, #0
003b976c: cmp r3, #0
003b9770: bne #0x3b9784
003b9774: ldr r0, [pc, #0x60]
003b9778: add r0, pc, r0
003b977c: bl #0x708eb0
003b9780: ldr ip, [r6]
003b9784: mov r0, ip
003b9788: bl #0x31bbf0
003b978c: bl #0x8be2a0
003b9790: mov ip, #0
003b9794: mov r1, r0
003b9798: mov r2, r4
003b979c: mov r0, r5
003b97a0: mov r3, ip
003b97a4: str ip, [sp]
003b97a8: bl #0x3c6144
003b97ac: b #0x3b96f0
003b97b0: mov r0, r4
003b97b4: mov r1, r5
003b97b8: str r2, [sp, #0xc]
003b97bc: bl #0x37baf8
003b97c0: bl #0x31bc80
003b97c4: cmp r0, #0
003b97c8: ldr r6, [r4, #4]
003b97cc: ldr r2, [sp, #0xc]
003b97d0: movne r4, r5
003b97d4: moveq r4, r0
003b97d8: b #0x3b973c
003b97dc: ldrsheq r4, [r0], #-0xc0

_ZN3sfc6script3lua8Instance16registerFunctionEPKcPFiP9lua_StateE 0x31afbc 52
0031afbc: push {r4, r5, r6, lr}
0031afc0: mov r4, r0
0031afc4: mov r5, r1
0031afc8: ldr r0, [r0, #4]
0031afcc: mov r1, r2
0031afd0: mov r2, #0
0031afd4: bl #0x84bcdc
0031afd8: ldr r0, [r4, #4]
0031afdc: mvn r1, #0x2700
0031afe0: sub r1, r1, #0x11
0031afe4: mov r2, r5
0031afe8: pop {r4, r5, r6, lr}
0031afec: b #0x84c080

_ZN9Character11_BeginSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8cb8 224
003b8cb8: push {r4, r5, lr}
003b8cbc: ldr r3, [r0, #4]
003b8cc0: sub sp, sp, #0xc
003b8cc4: mov r4, r0
003b8cc8: ldr r1, [r3, #4]
003b8ccc: ldr ip, [r3]
003b8cd0: rsb r3, ip, r1
003b8cd4: asr r3, r3, #4
003b8cd8: add r1, r3, r3, lsl #3
003b8cdc: add r1, r1, r1, lsl #6
003b8ce0: add r1, r3, r1, lsl #3
003b8ce4: add r1, r1, r1, lsl #15
003b8ce8: add r3, r3, r1, lsl #3
003b8cec: cmp r3, #0
003b8cf0: bne #0x3b8cfc
003b8cf4: add sp, sp, #0xc
003b8cf8: pop {r4, r5, pc}
003b8cfc: ldr r3, [ip, #4]
003b8d00: cmp r3, #3
003b8d04: bne #0x3b8cf4
003b8d08: mov r1, #0
003b8d0c: str r2, [sp, #4]
003b8d10: bl #0x37baf8
003b8d14: bl #0x38d798
003b8d18: ldr r2, [sp, #4]
003b8d1c: mov r5, r0
003b8d20: mov r0, r2
003b8d24: bl #0x3bc5fc
003b8d28: ldr r3, [r0, #4]
003b8d2c: ldr r2, [sp, #4]
003b8d30: cmp r5, r3
003b8d34: bhs #0x3b8cf4
003b8d38: ldr r5, [r4, #4]
003b8d3c: add r4, r2, #0x3c8
003b8d40: ldm r5, {r0, r3}
003b8d44: rsb r3, r0, r3
003b8d48: asr r3, r3, #4
003b8d4c: add r2, r3, r3, lsl #3
003b8d50: add r2, r2, r2, lsl #6
003b8d54: add r2, r3, r2, lsl #3
003b8d58: add r2, r2, r2, lsl #15
003b8d5c: add r3, r3, r2, lsl #3
003b8d60: cmp r3, #0
003b8d64: bne #0x3b8d78
003b8d68: ldr r0, [pc, #0x24]
003b8d6c: add r0, pc, r0
003b8d70: bl #0x708eb0
003b8d74: ldr r0, [r5]
003b8d78: bl #0x31bbf0
003b8d7c: bl #0x8be2a0
003b8d80: mov r1, r0
003b8d84: mov r0, r4
003b8d88: add sp, sp, #0xc
003b8d8c: pop {r4, r5, lr}
003b8d90: b #0x3d86bc
003b8d94: ldrsheq r5, [r0], #-0x6c

_ZN9Character9_GetStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6d78 40
003b6d78: add r0, r2, #0x4f0
003b6d7c: push {r4, lr}
003b6d80: add r0, r0, #0xc
003b6d84: mov r4, r1
003b6d88: bl #0x3c01ac
003b6d8c: mov r3, r0
003b6d90: mov r1, r3
003b6d94: mov r0, r4
003b6d98: pop {r4, lr}
003b6d9c: b #0x37cb24

_ZNSt4priv15__copy_backwardINS_15_Deque_iteratorIPSt6vectorIN3sfc6script3lua5ValueESaIS6_EESt16_Nonconst_traitsIS9_EEESC_iEET0_T_SE_SD_RKSt26random_access_iterator_tagPT1_ 0x31b8dc 252
0031b8dc: push {r4, r5, r6, lr}
0031b8e0: sub sp, sp, #0x10
0031b8e4: mov r5, r2
0031b8e8: mov ip, sp
0031b8ec: mov r6, r0
0031b8f0: mov r4, r3
0031b8f4: ldm r1, {r0, r1, r2, r3}
0031b8f8: stm ip, {r0, r1, r2, r3}
0031b8fc: mov r1, sp
0031b900: mov r0, r5
0031b904: bl #0x31b618
0031b908: cmp r0, #0
0031b90c: bgt #0x31b944
0031b910: b #0x31b9c4
0031b914: sub r2, r3, #4
0031b918: str r2, [r4]
0031b91c: ldr r3, [r5]
0031b920: ldr r1, [r5, #4]
0031b924: cmp r3, r1
0031b928: beq #0x31b98c
0031b92c: sub r1, r3, #4
0031b930: str r1, [r5]
0031b934: ldr r3, [r3, #-4]
0031b938: subs r0, r0, #1
0031b93c: str r3, [r2]
0031b940: beq #0x31b9c4
0031b944: ldr r3, [r4]
0031b948: ldr r2, [r4, #4]
0031b94c: cmp r3, r2
0031b950: bne #0x31b914
0031b954: ldr r3, [r4, #0xc]
0031b958: sub r2, r3, #4
0031b95c: str r2, [r4, #0xc]
0031b960: ldr r2, [r3, #-4]
0031b964: add r3, r2, #0x80
0031b968: str r2, [r4, #4]
0031b96c: sub r2, r3, #4
0031b970: str r3, [r4]
0031b974: str r3, [r4, #8]
0031b978: str r2, [r4]
0031b97c: ldr r3, [r5]
0031b980: ldr r1, [r5, #4]
0031b984: cmp r3, r1
0031b988: bne #0x31b92c
0031b98c: ldr r3, [r5, #0xc]
0031b990: subs r0, r0, #1
0031b994: sub r1, r3, #4
0031b998: str r1, [r5, #0xc]
0031b99c: ldr r1, [r3, #-4]
0031b9a0: add r3, r1, #0x80
0031b9a4: str r1, [r5, #4]
0031b9a8: sub r1, r3, #4
0031b9ac: str r3, [r5]
0031b9b0: str r3, [r5, #8]
0031b9b4: str r1, [r5]
0031b9b8: ldr r3, [r3, #-4]
0031b9bc: str r3, [r2]
0031b9c0: bne #0x31b944
0031b9c4: ldm r4, {r0, r1, r2, r3}
0031b9c8: stm r6, {r0, r1, r2, r3}
0031b9cc: mov r0, r6
0031b9d0: add sp, sp, #0x10
0031b9d4: pop {r4, r5, r6, pc}

_ZN3sfc6script3lua5Value11setUserDataEPNS1_8UserDataE 0x31b608 16
0031b608: mov r3, #7
0031b60c: str r1, [r0, #0x6c]
0031b610: str r3, [r0, #4]
0031b614: bx lr

_ZN9LuaScript14_GetHostPlayerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ca60 60
0037ca60: ldr r3, [pc, #0x2c]
0037ca64: ldr r2, [pc, #0x2c]
0037ca68: push {r4, lr}
0037ca6c: add r3, pc, r3
0037ca70: ldr r2, [r3, r2]
0037ca74: mov r4, r1
0037ca78: ldr r0, [r2, #0x40]
0037ca7c: bl #0x36e09c
0037ca80: ldr r3, [r0, #0x660]
0037ca84: mov r0, r4
0037ca88: mov r1, r3
0037ca8c: pop {r4, lr}
0037ca90: b #0x37c9f8
0037ca94: rsbeq r8, r1, r4, lsr #32
0037ca98: strdeq r3, r4, [r0], -r4

_ZN9LuaScriptC1Eb 0x37c584 240
0037c584: push {r4, r5, r6, r7, r8, lr}
0037c588: ldr r6, [pc, #0xd8]
0037c58c: ldr r3, [pc, #0xd8]
0037c590: mov r7, r0
0037c594: add r6, pc, r6
0037c598: ldr r3, [r6, r3]
0037c59c: mov r4, r0
0037c5a0: mov r8, r1
0037c5a4: add r3, r3, #8
0037c5a8: str r3, [r7], #4
0037c5ac: mov r0, r7
0037c5b0: bl #0x31b268
0037c5b4: ldr r2, [pc, #0xb4]
0037c5b8: mov r5, #0
0037c5bc: mov r3, r4
0037c5c0: ldr r2, [r6, r2]
0037c5c4: str r7, [r4, #0x14]
0037c5c8: str r5, [r4, #0x18]
0037c5cc: add r2, r2, #8
0037c5d0: str r2, [r4, #0x10]
0037c5d4: str r5, [r4, #0x20]
0037c5d8: mov r2, r4
0037c5dc: strb r5, [r3, #0x1c]!
0037c5e0: str r3, [r4, #0x28]
0037c5e4: str r3, [r4, #0x24]
0037c5e8: str r5, [r4, #0x2c]
0037c5ec: mov r3, r4
0037c5f0: str r5, [r4, #0x38]
0037c5f4: strb r5, [r2, #0x34]!
0037c5f8: str r2, [r4, #0x40]
0037c5fc: str r2, [r4, #0x3c]
0037c600: add r0, r4, #0x68
0037c604: str r5, [r4, #0x44]
0037c608: str r5, [r4, #0x50]
0037c60c: strb r5, [r3, #0x4c]!
0037c610: str r3, [r4, #0x58]
0037c614: str r3, [r4, #0x54]
0037c618: str r5, [r4, #0x5c]
0037c61c: strb r5, [r4, #0x64]
0037c620: str r0, [r4, #0x78]
0037c624: str r0, [r4, #0x7c]
0037c628: mov r1, #0x10
0037c62c: bl #0x31167c
0037c630: ldr r2, [r4, #0x78]
0037c634: mov r3, r4
0037c638: cmp r8, r5
0037c63c: strb r5, [r2]
0037c640: str r5, [r4, #0x84]
0037c644: strb r5, [r3, #0x80]!
0037c648: str r3, [r4, #0x8c]
0037c64c: str r5, [r4, #0x90]
0037c650: str r3, [r4, #0x88]
0037c654: bne #0x37c660
0037c658: mov r0, r4
0037c65c: bl #0x37b5a0
0037c660: mov r0, r4
0037c664: pop {r4, r5, r6, r7, r8, pc}

_ZN10GameObject15_RegisterSummonERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x392714 308
00392714: push {r4, r5, r6, lr}
00392718: ldr r2, [r0, #4]
0039271c: ldr r4, [pc, #0x118]
00392720: mov r5, r0
00392724: ldm r2, {r1, r3}
00392728: add r4, pc, r4
0039272c: rsb r3, r1, r3
00392730: asr r3, r3, #4
00392734: add r2, r3, r3, lsl #3
00392738: add r2, r2, r2, lsl #6
0039273c: add r2, r3, r2, lsl #3
00392740: add r2, r2, r2, lsl #15
00392744: add r3, r3, r2, lsl #3
00392748: cmp r3, #0
0039274c: bne #0x392754
00392750: pop {r4, r5, r6, pc}
00392754: ldr r3, [r1, #4]
00392758: cmp r3, #3
0039275c: bne #0x392750
00392760: mov r1, #0
00392764: bl #0x37baf8
00392768: bl #0x38d798
0039276c: ldr r3, [pc, #0xcc]
00392770: ldr r3, [r4, r3]
00392774: ldr r3, [r3]
00392778: cmp r0, r3
0039277c: bhs #0x392750
00392780: ldr r4, [r5, #4]
00392784: ldm r4, {r0, r3}
00392788: rsb r3, r0, r3
0039278c: asr r3, r3, #4
00392790: add r2, r3, r3, lsl #3
00392794: add r2, r2, r2, lsl #6
00392798: add r2, r3, r2, lsl #3
0039279c: add r2, r2, r2, lsl #15
003927a0: add r3, r3, r2, lsl #3
003927a4: cmp r3, #0
003927a8: bne #0x3927bc
003927ac: ldr r0, [pc, #0x90]
003927b0: add r0, pc, r0
003927b4: bl #0x708eb0
003927b8: ldr r0, [r4]
003927bc: bl #0x31bbf0
003927c0: bl #0x30e4cc
003927c4: ldr r2, [r5, #4]
003927c8: mov r4, r0
003927cc: ldr r3, [r2]
003927d0: ldr r2, [r2, #4]
003927d4: rsb r3, r3, r2
003927d8: asr r3, r3, #4
003927dc: add r2, r3, r3, lsl #3
003927e0: add r2, r2, r2, lsl #6
003927e4: add r2, r3, r2, lsl #3
003927e8: add r2, r2, r2, lsl #15
003927ec: add r3, r3, r2, lsl #3
003927f0: rsb r3, r3, #0
003927f4: cmp r3, #1
003927f8: bls #0x392814
003927fc: mov r0, r5
00392800: mov r1, #1
00392804: bl #0x37baf8
00392808: ldr r3, [r0, #4]
0039280c: cmp r3, #3
00392810: beq #0x392824
00392814: mov r1, #1
00392818: mov r0, r4
0039281c: pop {r4, r5, r6, lr}
00392820: b #0x3ab674
00392824: mov r1, #1
00392828: mov r0, r5
0039282c: bl #0x37baf8
00392830: bl #0x38d798
00392834: mov r1, r0
00392838: b #0x392818
0039283c: rsbeq r2, r0, r8, ror #6
00392840: andeq r4, r0, r4, lsl #4
00392844: ldrheq fp, [r2], #-0xc8

_ZN10GameObject17_EnableCollisionsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x39040c 316
0039040c: push {r4, r5, r6, lr}
00390410: ldr r3, [r0, #4]
00390414: sub sp, sp, #0x10
00390418: mov r5, r0
0039041c: ldr r1, [r3, #4]
00390420: ldr ip, [r3]
00390424: rsb r3, ip, r1
00390428: asr r3, r3, #4
0039042c: add r1, r3, r3, lsl #3
00390430: add r1, r1, r1, lsl #6
00390434: add r1, r3, r1, lsl #3
00390438: add r1, r1, r1, lsl #15
0039043c: add r3, r3, r1, lsl #3
00390440: cmp r3, #0
00390444: bne #0x390450
00390448: add sp, sp, #0x10
0039044c: pop {r4, r5, r6, pc}
00390450: ldr r6, [ip, #4]
00390454: cmp r6, #1
00390458: bne #0x390448
0039045c: mov r1, #0
00390460: str r2, [sp, #0xc]
00390464: bl #0x37baf8
00390468: bl #0x31bc80
0039046c: ldr r1, [r5, #4]
00390470: mov r4, r0
00390474: ldr r2, [sp, #0xc]
00390478: ldr r3, [r1]
0039047c: ldr r1, [r1, #4]
00390480: rsb r3, r3, r1
00390484: asr r3, r3, #4
00390488: add r1, r3, r3, lsl #3
0039048c: add r1, r1, r1, lsl #6
00390490: add r1, r3, r1, lsl #3
00390494: add r1, r1, r1, lsl #15
00390498: add r3, r3, r1, lsl #3
0039049c: rsb r3, r3, #0
003904a0: cmp r3, #1
003904a4: bls #0x3904fc
003904a8: mov r1, r6
003904ac: mov r0, r5
003904b0: bl #0x37baf8
003904b4: ldr r1, [r0, #4]
003904b8: ldr r2, [sp, #0xc]
003904bc: cmp r1, #1
003904c0: bne #0x3904fc
003904c4: mov r0, r5
003904c8: bl #0x37baf8
003904cc: bl #0x31bc80
003904d0: cmp r0, #0
003904d4: ldr r2, [sp, #0xc]
003904d8: beq #0x3904fc
003904dc: ldr r0, [r2, #0x2dc]
003904e0: cmp r0, #0
003904e4: beq #0x390448
003904e8: cmp r4, #0
003904ec: beq #0x390524
003904f0: add sp, sp, #0x10
003904f4: pop {r4, r5, r6, lr}
003904f8: b #0x46ec6c
003904fc: cmp r4, #0
00390500: bne #0x390514
00390504: mov r0, r2
00390508: add sp, sp, #0x10
0039050c: pop {r4, r5, r6, lr}
00390510: b #0x3949b0
00390514: mov r0, r2
00390518: add sp, sp, #0x10
0039051c: pop {r4, r5, r6, lr}
00390520: b #0x394a3c
00390524: ldrh r3, [r0, #0x22]
00390528: ldrsh r1, [r0, #0x24]
0039052c: ldrh r2, [r0, #0x20]
00390530: bic r3, r3, #0x1c
00390534: lsl r3, r3, #0x10
00390538: str r4, [sp]
0039053c: lsr r3, r3, #0x10
00390540: bl #0x46ece8
00390544: b #0x390448

_ZN3sfc6script3lua8Instance9includeIOEv 0x31aa50 4
0031aa50: bx lr

_ZN10GameObject17_GetTargetListTopERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38eb68 124
0038eb68: push {r4, r5, r6, lr}
0038eb6c: ldr r3, [r2, #0x314]
0038eb70: ldr r4, [r2, #0x304]
0038eb74: mov r5, r1
0038eb78: cmp r3, r4
0038eb7c: beq #0x38ebd4
0038eb80: ldr r1, [r4]
0038eb84: mov r0, r5
0038eb88: bl #0x37c9f8
0038eb8c: mov r0, r5
0038eb90: ldr r1, [r4, #4]
0038eb94: bl #0x37ccbc
0038eb98: movw r1, #0x2ee0
0038eb9c: ldr r0, [r4, #8]
0038eba0: movt r1, #0x4265
0038eba4: bl #0x30ed6c
0038eba8: mov r1, r0
0038ebac: mov r0, r5
0038ebb0: bl #0x37ccbc
0038ebb4: ldr r1, [r4, #0xc]
0038ebb8: mov r0, r5
0038ebbc: and r1, r1, #1
0038ebc0: bl #0x37c7e4
0038ebc4: ldr r1, [r4, #0x10]
0038ebc8: mov r0, r5
0038ebcc: pop {r4, r5, r6, lr}
0038ebd0: b #0x37ccbc
0038ebd4: mov r0, r1
0038ebd8: mov r1, #0
0038ebdc: pop {r4, r5, r6, lr}
0038ebe0: b #0x38eb00

_ZN9Character9_SetLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b73a4 260
003b73a4: push {r4, r5, r6, r7, r8, sb, sl, lr}
003b73a8: ldr r3, [r0, #4]
003b73ac: mov r6, r2
003b73b0: ldr r4, [pc, #0xe0]
003b73b4: ldm r3, {r1, r2}
003b73b8: add r4, pc, r4
003b73bc: mov r5, r0
003b73c0: rsb r3, r1, r2
003b73c4: asr r3, r3, #4
003b73c8: add r2, r3, r3, lsl #3
003b73cc: add r2, r2, r2, lsl #6
003b73d0: add r2, r3, r2, lsl #3
003b73d4: add r2, r2, r2, lsl #15
003b73d8: add r3, r3, r2, lsl #3
003b73dc: cmp r3, #0
003b73e0: bne #0x3b73e8
003b73e4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003b73e8: ldr r3, [r1, #4]
003b73ec: cmp r3, #3
003b73f0: bne #0x3b73e4
003b73f4: mov r1, #0
003b73f8: bl #0x37baf8
003b73fc: bl #0x31bbf0
003b7400: ldr r3, [pc, #0x94]
003b7404: ldr r8, [pc, #0x94]
003b7408: ldr r7, [pc, #0x94]
003b740c: ldr r4, [r4, r3]
003b7410: add r8, pc, r8
003b7414: add r7, pc, r7
003b7418: mov sl, r0
003b741c: mov r1, r8
003b7420: mov r2, r7
003b7424: ldr r0, [r4, #0x2c]
003b7428: bl #0x4c4bdc
003b742c: lsl sb, r0, #8
003b7430: mov r0, sl
003b7434: bl #0x30e4cc
003b7438: cmp sb, r0
003b743c: blt #0x3b7480
003b7440: mov r1, #0
003b7444: mov r0, r5
003b7448: bl #0x37baf8
003b744c: bl #0x31bbf0
003b7450: bl #0x30e4cc
003b7454: str r0, [r6, #0x5b8]
003b7458: mov r1, #1
003b745c: add r0, r6, #0x560
003b7460: bl #0x3e0810
003b7464: mov r0, r6
003b7468: mvn r1, #0
003b746c: bl #0x3bdca4
003b7470: mov r0, r6
003b7474: mvn r1, #0
003b7478: pop {r4, r5, r6, r7, r8, sb, sl, lr}
003b747c: b #0x3bdbb8
003b7480: ldr r0, [r4, #0x2c]
003b7484: mov r1, r8
003b7488: mov r2, r7
003b748c: bl #0x4c4bdc
003b7490: lsl r0, r0, #8
003b7494: b #0x3b7454
003b7498: ldrsbeq sp, [sp], #-0x68
003b749c: strdeq r3, r4, [r0], -r4
003b74a0: subseq sl, r0, r0, asr #6
003b74a4: subseq sp, r0, r4, lsl r1

_ZN3sfc6script3lua12ReturnValues12pushUserDataEPNS1_8UserDataE 0x37c9f8 104
0037c9f8: ldr r3, [pc, #0x58]
0037c9fc: ldr r2, [pc, #0x58]
0037ca00: push {r4, r5, r6, lr}
0037ca04: add r3, pc, r3
0037ca08: ldr r5, [r3, r2]
0037ca0c: sub sp, sp, #0x78
0037ca10: add r4, sp, #4
0037ca14: ldr r3, [r5]
0037ca18: str r3, [sp, #0x74]
0037ca1c: ldr r6, [r0, #0x24]
0037ca20: mov r0, r4
0037ca24: bl #0x37c978
0037ca28: mov r0, r6
0037ca2c: mov r1, r4
0037ca30: bl #0x3195c0
0037ca34: mov r0, r4
0037ca38: bl #0x3193e8
0037ca3c: ldr r2, [sp, #0x74]
0037ca40: ldr r3, [r5]
0037ca44: cmp r2, r3
0037ca48: bne #0x37ca54
0037ca4c: add sp, sp, #0x78
0037ca50: pop {r4, r5, r6, pc}
0037ca54: bl #0x30e310
0037ca58: rsbeq r8, r1, ip, lsl #1
0037ca5c: andeq r4, r0, ip, lsr #1

_ZN9Character11_ClearPropsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b82e4 328
003b82e4: push {r4, r5, lr}
003b82e8: ldr r3, [r0, #4]
003b82ec: sub sp, sp, #0xc
003b82f0: mov r4, r0
003b82f4: ldr r1, [r3, #4]
003b82f8: ldr ip, [r3]
003b82fc: rsb r3, ip, r1
003b8300: asr r3, r3, #4
003b8304: add r1, r3, r3, lsl #3
003b8308: add r1, r1, r1, lsl #6
003b830c: add r1, r3, r1, lsl #3
003b8310: add r1, r1, r1, lsl #15
003b8314: add r3, r3, r1, lsl #3
003b8318: cmp r3, #0
003b831c: bne #0x3b8328
003b8320: add sp, sp, #0xc
003b8324: pop {r4, r5, pc}
003b8328: ldr r3, [ip, #4]
003b832c: cmp r3, #2
003b8330: beq #0x3b8370
003b8334: cmp r3, #1
003b8338: bne #0x3b8320
003b833c: mov r1, #0
003b8340: mov r0, r4
003b8344: str r2, [sp, #4]
003b8348: bl #0x37baf8
003b834c: bl #0x31bc80
003b8350: cmp r0, #0
003b8354: ldr r2, [sp, #4]
003b8358: beq #0x3b8320
003b835c: add r0, r2, #0x560
003b8360: mov r1, #0
003b8364: add sp, sp, #0xc
003b8368: pop {r4, r5, lr}
003b836c: b #0x3def84
003b8370: mov r1, #0
003b8374: str r2, [sp, #4]
003b8378: bl #0x37baf8
003b837c: bl #0x31b580
003b8380: cmp r0, #0
003b8384: ldr r2, [sp, #4]
003b8388: beq #0x3b83f0
003b838c: ldr r5, [r4, #4]
003b8390: add r4, r2, #0x560
003b8394: ldm r5, {r0, r3}
003b8398: rsb r3, r0, r3
003b839c: asr r3, r3, #4
003b83a0: add r2, r3, r3, lsl #3
003b83a4: add r2, r2, r2, lsl #6
003b83a8: add r2, r3, r2, lsl #3
003b83ac: add r2, r2, r2, lsl #15
003b83b0: add r3, r3, r2, lsl #3
003b83b4: cmp r3, #0
003b83b8: bne #0x3b83cc
003b83bc: ldr r0, [pc, #0x64]
003b83c0: add r0, pc, r0
003b83c4: bl #0x708eb0
003b83c8: ldr r0, [r5]
003b83cc: bl #0x31b580
003b83d0: mov r1, r0
003b83d4: mov r0, r4
003b83d8: bl #0x3def84
003b83dc: mov r0, r4
003b83e0: mov r1, #1
003b83e4: add sp, sp, #0xc
003b83e8: pop {r4, r5, lr}
003b83ec: b #0x3e0810
003b83f0: ldr r3, [r4, #4]
003b83f4: ldr r1, [r3, #4]
003b83f8: ldr ip, [r3]
003b83fc: rsb r3, ip, r1
003b8400: asr r3, r3, #4
003b8404: add r1, r3, r3, lsl #3
003b8408: add r1, r1, r1, lsl #6
003b840c: add r1, r3, r1, lsl #3
003b8410: add r1, r1, r1, lsl #15
003b8414: add r3, r3, r1, lsl #3
003b8418: cmp r3, #0
003b841c: ldrne r3, [ip, #4]
003b8420: bne #0x3b8334
003b8424: b #0x3b8320
003b8428: subseq r6, r0, r8, lsr #1

_ZN10GameObject7_DropFXERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38fc74 124
0038fc74: push {r4, lr}
0038fc78: ldr r2, [r0, #4]
0038fc7c: ldr r4, [pc, #0x64]
0038fc80: sub sp, sp, #8
0038fc84: ldm r2, {r1, r3}
0038fc88: add r4, pc, r4
0038fc8c: rsb r3, r1, r3
0038fc90: asr r3, r3, #4
0038fc94: add r2, r3, r3, lsl #3
0038fc98: add r2, r2, r2, lsl #6
0038fc9c: add r2, r3, r2, lsl #3
0038fca0: add r2, r2, r2, lsl #15
0038fca4: add r3, r3, r2, lsl #3
0038fca8: cmp r3, #0
0038fcac: bne #0x38fcb8
0038fcb0: add sp, sp, #8
0038fcb4: pop {r4, pc}
0038fcb8: ldr r3, [r1, #4]
0038fcbc: cmp r3, #2
0038fcc0: bne #0x38fcb0
0038fcc4: mov r1, #0
0038fcc8: bl #0x37baf8
0038fccc: bl #0x31b580
0038fcd0: ldr r3, [pc, #0x14]
0038fcd4: add r1, sp, #8
0038fcd8: str r0, [r1, #-4]!
0038fcdc: ldr r0, [r4, r3]
0038fce0: bl #0x494978
0038fce4: b #0x38fcb0
0038fce8: rsbeq r4, r0, r8, lsl #28
0038fcec: andeq r1, r0, r8, lsl #22

_ZN9LuaScript13_CallPyScriptERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ef3c 168
0037ef3c: push {r4, r5, r6, lr}
0037ef40: ldr r2, [r0, #4]
0037ef44: ldr r4, [pc, #0x90]
0037ef48: ldm r2, {r1, r3}
0037ef4c: add r4, pc, r4
0037ef50: rsb r3, r1, r3
0037ef54: asr r3, r3, #4
0037ef58: add r2, r3, r3, lsl #3
0037ef5c: add r2, r2, r2, lsl #6
0037ef60: add r2, r3, r2, lsl #3
0037ef64: add r2, r2, r2, lsl #15
0037ef68: add r3, r3, r2, lsl #3
0037ef6c: cmp r3, #0
0037ef70: bne #0x37ef78
0037ef74: pop {r4, r5, r6, pc}
0037ef78: ldr r3, [r1, #4]
0037ef7c: cmp r3, #4
0037ef80: bne #0x37ef74
0037ef84: mov r1, #0
0037ef88: bl #0x37baf8
0037ef8c: bl #0x31c49c
0037ef90: ldr r3, [pc, #0x48]
0037ef94: mov r1, r0
0037ef98: mov r2, #1
0037ef9c: ldr r4, [r4, r3]
0037efa0: mov r0, r4
0037efa4: bl #0x4591f0
0037efa8: cmn r0, #1
0037efac: mov r5, r0
0037efb0: beq #0x37ef74
0037efb4: mov r0, r4
0037efb8: mov r1, r5
0037efbc: bl #0x455bec
0037efc0: subs r3, r0, #0
0037efc4: bne #0x37ef74
0037efc8: mov r0, r4
0037efcc: mov r1, r5
0037efd0: mvn r2, #0
0037efd4: pop {r4, r5, r6, lr}
0037efd8: b #0x4605c0
0037efdc: rsbeq r5, r1, r4, asr #22
0037efe0: andeq r1, r0, r0, lsr #20

_ZN9Character7_LookAtERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8ed8 96
003b8ed8: push {r4, lr}
003b8edc: ldr r3, [r0, #4]
003b8ee0: ldm r3, {r0, r1}
003b8ee4: rsb r3, r0, r1
003b8ee8: asr r3, r3, #4
003b8eec: add r1, r3, r3, lsl #3
003b8ef0: add r1, r1, r1, lsl #6
003b8ef4: add r1, r3, r1, lsl #3
003b8ef8: add r1, r1, r1, lsl #15
003b8efc: add r3, r3, r1, lsl #3
003b8f00: cmp r3, #0
003b8f04: bne #0x3b8f0c
003b8f08: pop {r4, pc}
003b8f0c: ldr r3, [r0, #4]
003b8f10: cmp r3, #2
003b8f14: beq #0x3b8f20
003b8f18: cmp r3, #7
003b8f1c: bne #0x3b8f08
003b8f20: ldr r4, [r2, #0x378]
003b8f24: bl #0x31b5a0
003b8f28: mov r1, r0
003b8f2c: mov r0, r4
003b8f30: pop {r4, lr}
003b8f34: b #0x4052bc

_ZN10GameObject16_GetObjectByNameERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x392848 168
00392848: push {r4, r5, r6, lr}
0039284c: ldr r2, [r0, #4]
00392850: mov r4, r1
00392854: ldr r3, [pc, #0x8c]
00392858: ldm r2, {r0, r1}
0039285c: add r3, pc, r3
00392860: sub sp, sp, #0x18
00392864: rsb r2, r0, r1
00392868: asr r2, r2, #4
0039286c: add r1, r2, r2, lsl #3
00392870: add r1, r1, r1, lsl #6
00392874: add r1, r2, r1, lsl #3
00392878: add r1, r1, r1, lsl #15
0039287c: add r2, r2, r1, lsl #3
00392880: cmn r2, #1
00392884: beq #0x392890
00392888: add sp, sp, #0x18
0039288c: pop {r4, r5, r6, pc}
00392890: ldr r2, [r0, #4]
00392894: cmp r2, #4
00392898: bne #0x392888
0039289c: ldr r2, [pc, #0x48]
003928a0: add r5, sp, #0xc
003928a4: ldr r3, [r3, r2]
003928a8: ldr r6, [r3, #0x38]
003928ac: bl #0x31c49c
003928b0: mov ip, #0
003928b4: mov r2, r0
003928b8: mov r1, r6
003928bc: mvn r3, #0
003928c0: mov r0, r5
003928c4: str ip, [sp, #4]
003928c8: str ip, [sp]
003928cc: bl #0x34aca0
003928d0: mov r0, r5
003928d4: bl #0x33fee4
003928d8: mov r1, r0
003928dc: mov r0, r4
003928e0: bl #0x37c9f8
003928e4: b #0x392888
003928e8: rsbeq r2, r0, r4, lsr r2
003928ec: strdeq r3, r4, [r0], -r4

_ZN3sfc6script3lua8Instance5pCallEPKcRKNS1_9ArgumentsERNS1_12ReturnValuesE 0x31abe8 60
0031abe8: push {r4, r5, r6, lr}
0031abec: mov r4, r0
0031abf0: mov r0, r1
0031abf4: mvn r1, #0x2700
0031abf8: mov r5, r2
0031abfc: sub r1, r1, #0x11
0031ac00: mov r2, r0
0031ac04: ldr r0, [r4, #4]
0031ac08: mov r6, r3
0031ac0c: bl #0x84c1ec
0031ac10: mov r0, r4
0031ac14: mov r1, r5
0031ac18: mov r2, r6
0031ac1c: pop {r4, r5, r6, lr}
0031ac20: b #0x31ab4c

_ZN9LuaScript12_SetGameTypeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f878 340
0037f878: push {r4, r5, r6, lr}
0037f87c: ldr r2, [r0, #4]
0037f880: ldr r6, [pc, #0x124]
0037f884: sub sp, sp, #8
0037f888: ldm r2, {r1, r3}
0037f88c: add r6, pc, r6
0037f890: mov r4, r0
0037f894: rsb r3, r1, r3
0037f898: asr r3, r3, #4
0037f89c: add r2, r3, r3, lsl #3
0037f8a0: add r2, r2, r2, lsl #6
0037f8a4: add r2, r3, r2, lsl #3
0037f8a8: add r2, r2, r2, lsl #15
0037f8ac: add r3, r3, r2, lsl #3
0037f8b0: cmp r3, #0
0037f8b4: bne #0x37f8c0
0037f8b8: add sp, sp, #8
0037f8bc: pop {r4, r5, r6, pc}
0037f8c0: ldr r3, [r1, #4]
0037f8c4: cmp r3, #3
0037f8c8: bne #0x37f8b8
0037f8cc: mov r1, #0
0037f8d0: bl #0x37baf8
0037f8d4: bl #0x31bbf0
0037f8d8: bl #0x8be2a0
0037f8dc: cmp r0, #3
0037f8e0: bhi #0x37f8b8
0037f8e4: ldr r3, [pc, #0xc4]
0037f8e8: ldr r0, [r6, r3]
0037f8ec: bl #0x31f594
0037f8f0: subs r5, r0, #0
0037f8f4: beq #0x37f8b8
0037f8f8: ldr r4, [r4, #4]
0037f8fc: ldm r4, {r0, r3}
0037f900: rsb r3, r0, r3
0037f904: asr r3, r3, #4
0037f908: add r2, r3, r3, lsl #3
0037f90c: add r2, r2, r2, lsl #6
0037f910: add r2, r3, r2, lsl #3
0037f914: add r2, r2, r2, lsl #15
0037f918: add r3, r3, r2, lsl #3
0037f91c: cmp r3, #0
0037f920: bne #0x37f934
0037f924: ldr r0, [pc, #0x88]
0037f928: add r0, pc, r0
0037f92c: bl #0x708eb0
0037f930: ldr r0, [r4]
0037f934: bl #0x31bbf0
0037f938: bl #0x8be2a0
0037f93c: cmp r0, #3
0037f940: mov r4, r0
0037f944: bls #0x37f96c
0037f948: ldr r3, [pc, #0x68]
0037f94c: ldr r3, [r6, r3]
0037f950: ldr r3, [r3]
0037f954: cmp r3, #2
0037f958: moveq r3, #0
0037f95c: streq r3, [r3]
0037f960: beq #0x37f96c
0037f964: cmp r3, #1
0037f968: beq #0x37f974
0037f96c: str r4, [r5, #0x150]
0037f970: b #0x37f8b8
0037f974: ldr r0, [pc, #0x40]
0037f978: ldr r1, [pc, #0x40]
0037f97c: ldr r2, [pc, #0x40]
0037f980: ldr r0, [r6, r0]
0037f984: ldr r3, [pc, #0x3c]
0037f988: movw ip, #0x1ee
0037f98c: add r1, pc, r1
0037f990: add r0, r0, #0xa8
0037f994: add r2, pc, r2
0037f998: add r3, pc, r3
0037f99c: str ip, [sp]
0037f9a0: bl #0x30e004
0037f9a4: str r4, [r5, #0x150]
0037f9a8: b #0x37f8b8
0037f9ac: rsbeq r5, r1, r4, lsl #4
0037f9b0: strdeq r3, r4, [r0], -r4
0037f9b4: subseq lr, r3, r0, asr #22
0037f9b8: andeq r3, r0, r0, asr #19
0037f9bc: andeq r1, r0, r0, asr #19
0037f9c0: subseq lr, r3, ip, asr #20

_ZN3sfc6script3lua6Binder18__functionCallbackEP9lua_State 0x31a250 452
0031a250: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0031a254: ldr r4, [pc, #0x194]
0031a258: ldr sb, [pc, #0x194]
0031a25c: sub sp, sp, #0x4c
0031a260: add r4, pc, r4
0031a264: ldr r3, [r4, sb]
0031a268: add r6, sp, #0x14
0031a26c: mov r1, r0
0031a270: ldr r3, [r3]
0031a274: mov r7, r0
0031a278: mov r2, #0
0031a27c: mov r0, r6
0031a280: add sl, sp, #0xc
0031a284: str r3, [sp, #0x44]
0031a288: add r5, sp, #0x1c
0031a28c: bl #0x3196ec
0031a290: mov r2, #2
0031a294: mov r1, r7
0031a298: mov r0, sl
0031a29c: bl #0x3196ec
0031a2a0: mov r0, r5
0031a2a4: bl #0x31b434
0031a2a8: ldr r8, [sp, #0x10]
0031a2ac: ldm r8, {r0, r3}
0031a2b0: rsb r3, r0, r3
0031a2b4: asr r3, r3, #4
0031a2b8: add r2, r3, r3, lsl #3
0031a2bc: add r2, r2, r2, lsl #6
0031a2c0: add r2, r3, r2, lsl #3
0031a2c4: add r2, r2, r2, lsl #15
0031a2c8: add r3, r3, r2, lsl #3
0031a2cc: cmp r3, #0
0031a2d0: bne #0x31a2e4
0031a2d4: ldr r0, [pc, #0x11c]
0031a2d8: add r0, pc, r0
0031a2dc: bl #0x708eb0
0031a2e0: ldr r0, [r8]
0031a2e4: bl #0x31b580
0031a2e8: ldr fp, [sp, #0x10]
0031a2ec: mov r8, r0
0031a2f0: ldm fp, {r0, r3}
0031a2f4: rsb r3, r0, r3
0031a2f8: asr r3, r3, #4
0031a2fc: add r2, r3, r3, lsl #3
0031a300: add r2, r2, r2, lsl #6
0031a304: add r2, r3, r2, lsl #3
0031a308: add r2, r2, r2, lsl #15
0031a30c: add r3, r3, r2, lsl #3
0031a310: rsb r3, r3, #0
0031a314: cmp r3, #1
0031a318: bhi #0x31a32c
0031a31c: ldr r0, [pc, #0xd8]
0031a320: add r0, pc, r0
0031a324: bl #0x708eb0
0031a328: ldr r0, [fp]
0031a32c: add r0, r0, #0x70
0031a330: bl #0x31b580
0031a334: cmp r8, #0
0031a338: mov fp, r0
0031a33c: beq #0x31a398
0031a340: mov r2, fp
0031a344: mov r0, r6
0031a348: mov r1, r5
0031a34c: blx r8
0031a350: mov r1, r7
0031a354: mov r0, r5
0031a358: bl #0x31b308
0031a35c: mov r7, r0
0031a360: mov r0, r5
0031a364: bl #0x31b398
0031a368: mov r0, sl
0031a36c: bl #0x319228
0031a370: mov r0, r6
0031a374: bl #0x319228
0031a378: ldr r3, [r4, sb]
0031a37c: ldr r2, [sp, #0x44]
0031a380: mov r0, r7
0031a384: ldr r3, [r3]
0031a388: cmp r2, r3
0031a38c: bne #0x31a3ec
0031a390: add sp, sp, #0x4c
0031a394: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031a398: ldr r3, [pc, #0x60]
0031a39c: ldr r3, [r4, r3]
0031a3a0: ldr r3, [r3]
0031a3a4: cmp r3, #2
0031a3a8: streq r8, [r8]
0031a3ac: beq #0x31a340
0031a3b0: cmp r3, #1
0031a3b4: bne #0x31a340
0031a3b8: ldr r0, [pc, #0x44]
0031a3bc: ldr r1, [pc, #0x44]
0031a3c0: ldr r2, [pc, #0x44]
0031a3c4: ldr r0, [r4, r0]
0031a3c8: ldr r3, [pc, #0x40]
0031a3cc: mov ip, #0x20
0031a3d0: add r1, pc, r1
0031a3d4: add r2, pc, r2
0031a3d8: add r3, pc, r3
0031a3dc: add r0, r0, #0xa8
0031a3e0: str ip, [sp]
0031a3e4: bl #0x30e004
0031a3e8: b #0x31a340
0031a3ec: bl #0x30e310
0031a3f0: rsbeq sl, r7, r0, lsr r8
0031a3f4: andeq r4, r0, ip, lsr #1

_ZN9Character5_KillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7e40 108
003b7e40: push {r4, lr}
003b7e44: ldr r3, [r0, #4]
003b7e48: ldm r3, {r0, r1}
003b7e4c: rsb r3, r0, r1
003b7e50: asr r3, r3, #4
003b7e54: add r1, r3, r3, lsl #3
003b7e58: add r1, r1, r1, lsl #6
003b7e5c: add r1, r3, r1, lsl #3
003b7e60: add r1, r1, r1, lsl #15
003b7e64: add r3, r3, r1, lsl #3
003b7e68: cmp r3, #0
003b7e6c: bne #0x3b7e84
003b7e70: ldr r0, [r2, #0x378]
003b7e74: mov r1, #0
003b7e78: mov r2, r1
003b7e7c: pop {r4, lr}
003b7e80: b #0x40570c
003b7e84: ldr r3, [r0, #4]
003b7e88: cmp r3, #7
003b7e8c: bne #0x3b7e70
003b7e90: ldr r4, [r2, #0x378]
003b7e94: bl #0x31b5a0
003b7e98: mov r2, #0
003b7e9c: mov r1, r0
003b7ea0: mov r0, r4
003b7ea4: pop {r4, lr}
003b7ea8: b #0x40570c

_ZN9Character10_HasTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6f50 20
003b6f50: ldr r3, [r2, #0x408]
003b6f54: mov r0, r1
003b6f58: subs r1, r3, #0
003b6f5c: movne r1, #1
003b6f60: b #0x37c7e4

_ZN9Character19_IsMasterHostPlayerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6fc4 92
003b6fc4: push {r4, r5, r6, lr}
003b6fc8: mov r4, r2
003b6fcc: ldr r2, [r2, #0x418]
003b6fd0: ldr r3, [pc, #0x40]
003b6fd4: mov r5, r1
003b6fd8: cmp r2, #0
003b6fdc: add r3, pc, r3
003b6fe0: moveq r1, r2
003b6fe4: beq #0x3b700c
003b6fe8: ldr r2, [pc, #0x2c]
003b6fec: ldr r3, [r3, r2]
003b6ff0: ldr r0, [r3, #0x40]
003b6ff4: bl #0x36e09c
003b6ff8: ldr r3, [r4, #0x418]
003b6ffc: ldr r1, [r0, #0x660]
003b7000: cmp r1, r3
003b7004: movne r1, #0
003b7008: moveq r1, #1
003b700c: mov r0, r5
003b7010: pop {r4, r5, r6, lr}
003b7014: b #0x37c7e4
003b7018: ldrheq sp, [sp], #-0xa4
003b701c: strdeq r3, r4, [r0], -r4

_ZN10GameObject14_PopTargetListERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38fbb8 64
0038fbb8: push {r4, lr}
0038fbbc: sub sp, sp, #0x10
0038fbc0: mov lr, r2
0038fbc4: mov ip, sp
0038fbc8: add r4, r2, #0x304
0038fbcc: ldm r4, {r0, r1, r2, r3}
0038fbd0: stm ip, {r0, r1, r2, r3}
0038fbd4: add r0, lr, #0x314
0038fbd8: mov r1, sp
0038fbdc: bl #0x38d610
0038fbe0: cmp r0, #0
0038fbe4: beq #0x38fbf0
0038fbe8: mov r0, r4
0038fbec: bl #0x38fb18
0038fbf0: add sp, sp, #0x10
0038fbf4: pop {r4, pc}

_ZN9Character9_PlayAnimERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3ba388 916
003ba388: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ba38c: ldr r3, [r0, #4]
003ba390: mov r6, r2
003ba394: ldr r4, [pc, #0x344]
003ba398: ldm r3, {r1, r2}
003ba39c: add r4, pc, r4
003ba3a0: sub sp, sp, #0xc
003ba3a4: rsb r3, r1, r2
003ba3a8: asr r3, r3, #4
003ba3ac: mov r5, r0
003ba3b0: add r2, r3, r3, lsl #3
003ba3b4: add r2, r2, r2, lsl #6
003ba3b8: add r2, r3, r2, lsl #3
003ba3bc: add r2, r2, r2, lsl #15
003ba3c0: add r3, r3, r2, lsl #3
003ba3c4: cmp r3, #0
003ba3c8: bne #0x3ba3d4
003ba3cc: add sp, sp, #0xc
003ba3d0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ba3d4: ldr r3, [r1, #4]
003ba3d8: cmp r3, #3
003ba3dc: bne #0x3ba3cc
003ba3e0: mov r1, #0
003ba3e4: bl #0x37baf8
003ba3e8: bl #0x38d798
003ba3ec: ldr r3, [pc, #0x2f0]
003ba3f0: ldr r3, [r4, r3]
003ba3f4: ldr r3, [r3]
003ba3f8: cmp r0, r3
003ba3fc: bhs #0x3ba3cc
003ba400: ldr r3, [pc, #0x2e0]
003ba404: add r3, pc, r3
003ba408: ldr fp, [r3, #0xc]
003ba40c: add r2, fp, #1
003ba410: cmp r2, #4
003ba414: str r2, [r3, #0xc]
003ba418: bgt #0x3ba568
003ba41c: add sl, fp, #0x15
003ba420: mov r8, #0x14
003ba424: add fp, fp, #0x10
003ba428: mul sl, r8, sl
003ba42c: mul r8, r8, fp
003ba430: ldr r7, [pc, #0x2b4]
003ba434: ldr sb, [r4, r7]
003ba438: ldr r2, [sb]
003ba43c: add r3, r2, r8
003ba440: ldr r3, [r3, #8]
003ba444: cmp r3, #2
003ba448: beq #0x3ba470
003ba44c: ldr r3, [pc, #0x29c]
003ba450: ldr r3, [r4, r3]
003ba454: ldr r3, [r3]
003ba458: cmp r3, #2
003ba45c: moveq r3, #0
003ba460: streq r3, [r3]
003ba464: beq #0x3ba470
003ba468: cmp r3, #1
003ba46c: beq #0x3ba66c
003ba470: add r3, r2, sl
003ba474: ldr r3, [r3, #8]
003ba478: cmp r3, #1
003ba47c: beq #0x3ba4a4
003ba480: ldr r3, [pc, #0x268]
003ba484: ldr r3, [r4, r3]
003ba488: ldr r3, [r3]
003ba48c: cmp r3, #2
003ba490: moveq r3, #0
003ba494: streq r3, [r3]
003ba498: beq #0x3ba4a4
003ba49c: cmp r3, #1
003ba4a0: beq #0x3ba6a4
003ba4a4: ldr sb, [r5, #4]
003ba4a8: ldm sb, {r0, r3}
003ba4ac: rsb r3, r0, r3
003ba4b0: asr r3, r3, #4
003ba4b4: add r1, r3, r3, lsl #3
003ba4b8: add r1, r1, r1, lsl #6
003ba4bc: add r1, r3, r1, lsl #3
003ba4c0: add r1, r1, r1, lsl #15
003ba4c4: add r3, r3, r1, lsl #3
003ba4c8: rsb r3, r3, #0
003ba4cc: cmp r3, #1
003ba4d0: bls #0x3ba4e0
003ba4d4: ldr r1, [r0, #0x74]
003ba4d8: cmp r1, #3
003ba4dc: beq #0x3ba5dc
003ba4e0: add r8, r2, r8
003ba4e4: cmp r3, #0
003ba4e8: ldr r8, [r8, #0xc]
003ba4ec: beq #0x3ba5c8
003ba4f0: bl #0x31bbf0
003ba4f4: bl #0x30e4cc
003ba4f8: ldr r3, [r4, r7]
003ba4fc: str r0, [r8, #8]
003ba500: mvn r2, #0
003ba504: ldr r3, [r3]
003ba508: add sl, r3, sl
003ba50c: ldr r3, [sl, #0xc]
003ba510: str r2, [r3, #8]
003ba514: ldr r2, [r5, #4]
003ba518: ldr r3, [r2]
003ba51c: ldr r2, [r2, #4]
003ba520: rsb r3, r3, r2
003ba524: asr r3, r3, #4
003ba528: add r2, r3, r3, lsl #3
003ba52c: add r2, r2, r2, lsl #6
003ba530: add r2, r3, r2, lsl #3
003ba534: add r2, r2, r2, lsl #15
003ba538: add r3, r3, r2, lsl #3
003ba53c: rsb r3, r3, #0
003ba540: cmp r3, #2
003ba544: bhi #0x3ba580
003ba548: add r0, r6, #0x4f0
003ba54c: mov r2, #0
003ba550: add r0, r0, #0xc
003ba554: mov r1, fp
003ba558: mov r3, r2
003ba55c: add sp, sp, #0xc
003ba560: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ba564: b #0x3c1aa8
003ba568: mov r2, #0
003ba56c: str r2, [r3, #0xc]
003ba570: mov sl, #0x190
003ba574: mov r8, #0x12c
003ba578: mov fp, #0xf
003ba57c: b #0x3ba430
003ba580: mov r0, r5
003ba584: mov r1, #2
003ba588: bl #0x37baf8
003ba58c: ldr r3, [r0, #4]
003ba590: cmp r3, #1
003ba594: bne #0x3ba548
003ba598: mov r1, #2
003ba59c: mov r0, r5
003ba5a0: bl #0x37baf8
003ba5a4: bl #0x31bc80
003ba5a8: mov r2, r0
003ba5ac: add r0, r6, #0x4f0
003ba5b0: add r0, r0, #0xc
003ba5b4: mov r1, fp
003ba5b8: mov r3, #0
003ba5bc: add sp, sp, #0xc
003ba5c0: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ba5c4: b #0x3c1aa8
003ba5c8: ldr r0, [pc, #0x124]
003ba5cc: add r0, pc, r0
003ba5d0: bl #0x708eb0
003ba5d4: ldr r0, [sb]
003ba5d8: b #0x3ba4f0
003ba5dc: add r2, r2, r8
003ba5e0: cmp r3, #0
003ba5e4: ldr r8, [r2, #0xc]
003ba5e8: bne #0x3ba5fc
003ba5ec: ldr r0, [pc, #0x104]
003ba5f0: add r0, pc, r0
003ba5f4: bl #0x708eb0
003ba5f8: ldr r0, [sb]
003ba5fc: bl #0x31bbf0
003ba600: bl #0x30e4cc
003ba604: str r0, [r8, #8]
003ba608: ldr r8, [r5, #4]
003ba60c: ldr r2, [r4, r7]
003ba610: ldm r8, {r0, r3}
003ba614: ldr r2, [r2]
003ba618: rsb r3, r0, r3
003ba61c: asr r3, r3, #4
003ba620: add sl, r2, sl
003ba624: add r2, r3, r3, lsl #3
003ba628: ldr r4, [sl, #0xc]
003ba62c: add r2, r2, r2, lsl #6
003ba630: add r2, r3, r2, lsl #3
003ba634: add r2, r2, r2, lsl #15
003ba638: add r3, r3, r2, lsl #3
003ba63c: rsb r3, r3, #0
003ba640: cmp r3, #1
003ba644: bhi #0x3ba658
003ba648: ldr r0, [pc, #0xac]
003ba64c: add r0, pc, r0
003ba650: bl #0x708eb0
003ba654: ldr r0, [r8]
003ba658: add r0, r0, #0x70
003ba65c: bl #0x31bbf0
003ba660: bl #0x30e4cc
003ba664: str r0, [r4, #8]
003ba668: b #0x3ba514
003ba66c: ldr r0, [pc, #0x8c]
003ba670: ldr r1, [pc, #0x8c]
003ba674: ldr r2, [pc, #0x8c]
003ba678: ldr r0, [r4, r0]
003ba67c: ldr r3, [pc, #0x88]
003ba680: add r2, pc, r2
003ba684: mov ip, #0x4d0
003ba688: add r1, pc, r1
003ba68c: add r0, r0, #0xa8
003ba690: add r3, pc, r3
003ba694: str ip, [sp]
003ba698: bl #0x30e004
003ba69c: ldr r2, [sb]
003ba6a0: b #0x3ba470
003ba6a4: ldr r0, [pc, #0x54]
003ba6a8: ldr r1, [pc, #0x60]
003ba6ac: ldr r2, [pc, #0x60]
003ba6b0: ldr r0, [r4, r0]
003ba6b4: ldr r3, [pc, #0x5c]
003ba6b8: add r2, pc, r2
003ba6bc: movw ip, #0x4d1
003ba6c0: add r3, pc, r3
003ba6c4: add r1, pc, r1
003ba6c8: add r0, r0, #0xa8
003ba6cc: str ip, [sp]
003ba6d0: bl #0x30e004
003ba6d4: ldr r3, [r4, r7]
003ba6d8: ldr r2, [r3]
003ba6dc: b #0x3ba4a4
003ba6e0: ldrsheq sl, [sp], #-0x64
003ba6e4: andeq r2, r0, r8, lsr r2
003ba6e8: ldrsbeq r8, [lr], #-0x54
003ba6ec: andeq r3, r0, ip, ror ip
003ba6f0: andeq r3, r0, r0, asr #19

_ZN3sfc6script3lua5ErrorC2EP9lua_Statei 0x31a980 104
0031a980: ldr ip, [pc, #0x58]
0031a984: push {r4, r5, r6, lr}
0031a988: ldr lr, [pc, #0x54]
0031a98c: add ip, pc, ip
0031a990: mov r3, r0
0031a994: ldr lr, [ip, lr]
0031a998: mov r4, r0
0031a99c: mov r5, r2
0031a9a0: add lr, lr, #8
0031a9a4: str lr, [r3], #8
0031a9a8: mov r0, r3
0031a9ac: str r3, [r4, #0x18]
0031a9b0: str r3, [r4, #0x1c]
0031a9b4: mov r6, r1
0031a9b8: bl #0x31a710
0031a9bc: ldr r3, [r4, #0x18]
0031a9c0: mov r2, #0
0031a9c4: mov r0, r4
0031a9c8: strb r2, [r3]
0031a9cc: mov r1, r6
0031a9d0: mov r2, r5
0031a9d4: bl #0x31a8ac
0031a9d8: mov r0, r4
0031a9dc: pop {r4, r5, r6, pc}
0031a9e0: rsbeq sl, r7, r4, lsl #2
0031a9e4: muleq r0, r8, r4

_ZNSt4priv11_Deque_baseIPSt6vectorIN3sfc6script3lua5ValueESaIS5_EESaIS8_EED2Ev 0x31be6c 132
0031be6c: push {r4, r5, r6, lr}
0031be70: mov r6, r0
0031be74: ldr r0, [r0, #0x20]
0031be78: cmp r0, #0
0031be7c: beq #0x31bed4
0031be80: ldr r5, [r6, #0x1c]
0031be84: ldr r4, [r6, #0xc]
0031be88: add r5, r5, #4
0031be8c: cmp r4, r5
0031be90: bhs #0x31bee8
0031be94: ldr r0, [r4]
0031be98: mov r1, #0x80
0031be9c: add r4, r4, #4
0031bea0: cmp r0, #0
0031bea4: beq #0x31beac
0031bea8: bl #0x708f00
0031beac: cmp r5, r4
0031beb0: bhi #0x31be94
0031beb4: ldr r0, [r6, #0x20]
0031beb8: ldr r1, [r6, #0x24]
0031bebc: cmp r0, #0
0031bec0: beq #0x31bed4
0031bec4: lsl r1, r1, #2
0031bec8: cmp r1, #0x80
0031becc: bhi #0x31bedc
0031bed0: bl #0x708f00
0031bed4: mov r0, r6
0031bed8: pop {r4, r5, r6, pc}
0031bedc: bl #0x310440
0031bee0: mov r0, r6
0031bee4: pop {r4, r5, r6, pc}
0031bee8: ldr r1, [r6, #0x24]
0031beec: b #0x31bec4

_ZN9Character25_GetPropBonusAttackRatingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7b84 92
003b7b84: push {r4, r5, r6, lr}
003b7b88: ldr r3, [r0, #4]
003b7b8c: mov r5, r2
003b7b90: mov r4, r1
003b7b94: ldm r3, {r0, r2}
003b7b98: rsb r3, r0, r2
003b7b9c: asr r3, r3, #4
003b7ba0: add r2, r3, r3, lsl #3
003b7ba4: add r2, r2, r2, lsl #6
003b7ba8: add r2, r3, r2, lsl #3
003b7bac: add r2, r2, r2, lsl #15
003b7bb0: add r3, r3, r2, lsl #3
003b7bb4: cmp r3, #0
003b7bb8: bne #0x3b7bc0
003b7bbc: pop {r4, r5, r6, pc}
003b7bc0: bl #0x31bc80
003b7bc4: mov r1, r0
003b7bc8: add r0, r5, #0x560
003b7bcc: bl #0x3df81c
003b7bd0: mov r1, r0
003b7bd4: mov r0, r4
003b7bd8: pop {r4, r5, r6, lr}
003b7bdc: b #0x37cb24

_ZN10GameObject12_SetPositionERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3928f0 656
003928f0: push {r4, r5, r6, r7, lr}
003928f4: ldr r3, [r0, #4]
003928f8: mov r4, r2
003928fc: ldr r5, [pc, #0x274]
00392900: ldm r3, {r1, r2}
00392904: add r5, pc, r5
00392908: sub sp, sp, #0x24
0039290c: rsb r3, r1, r2
00392910: asr r3, r3, #4
00392914: mov r6, r0
00392918: add r2, r3, r3, lsl #3
0039291c: add r2, r2, r2, lsl #6
00392920: add r2, r3, r2, lsl #3
00392924: add r2, r2, r2, lsl #15
00392928: add r3, r3, r2, lsl #3
0039292c: rsb r3, r3, #0
00392930: cmp r3, #1
00392934: beq #0x3929a4
00392938: cmp r3, #3
0039293c: beq #0x392948
00392940: add sp, sp, #0x24
00392944: pop {r4, r5, r6, r7, pc}
00392948: ldr r3, [r1, #4]
0039294c: cmp r3, #3
00392950: bne #0x392940
00392954: mov r1, #1
00392958: bl #0x37baf8
0039295c: ldr r3, [r0, #4]
00392960: cmp r3, #3
00392964: bne #0x392940
00392968: mov r0, r6
0039296c: mov r1, #2
00392970: bl #0x37baf8
00392974: ldr r3, [r0, #4]
00392978: cmp r3, #3
0039297c: bne #0x392940
00392980: ldr r2, [r6, #4]
00392984: movw r3, #0x6db7
00392988: movt r3, #0xb6db
0039298c: ldr r1, [r2, #4]
00392990: ldr r2, [r2]
00392994: rsb r2, r2, r1
00392998: asr r2, r2, #4
0039299c: mul r3, r3, r2
003929a0: b #0x392a70
003929a4: ldr r3, [r1, #4]
003929a8: cmp r3, #4
003929ac: bne #0x392a18
003929b0: mov r0, r6
003929b4: mov r1, #0
003929b8: bl #0x37baf8
003929bc: ldr r3, [r0, #4]
003929c0: cmp r3, #4
003929c4: beq #0x392b28
003929c8: mov r1, #0
003929cc: mov r0, r6
003929d0: bl #0x37baf8
003929d4: bl #0x31b5a0
003929d8: mov r5, r0
003929dc: cmp r5, #0
003929e0: beq #0x392940
003929e4: mov r0, r4
003929e8: add r1, r5, #0x160
003929ec: mov r2, #1
003929f0: bl #0x393db4
003929f4: ldr r3, [r5, #0x160]
003929f8: mov r0, r4
003929fc: str r3, [r4, #0x1e0]
00392a00: ldr r3, [r5, #0x164]
00392a04: str r3, [r4, #0x1e4]
00392a08: ldr r3, [r5, #0x168]
00392a0c: str r3, [r4, #0x1e8]
00392a10: bl #0x393e90
00392a14: b #0x392940
00392a18: mov r1, #0
00392a1c: bl #0x37baf8
00392a20: ldr r3, [r0, #4]
00392a24: cmp r3, #7
00392a28: beq #0x392af8
00392a2c: mov r0, r6
00392a30: mov r1, #0
00392a34: bl #0x37baf8
00392a38: ldr r3, [r0, #4]
00392a3c: cmp r3, #2
00392a40: bne #0x392940
00392a44: ldr r3, [r6, #4]
00392a48: ldr r2, [r3, #4]
00392a4c: ldr r3, [r3]
00392a50: rsb r3, r3, r2
00392a54: asr r3, r3, #4
00392a58: add r2, r3, r3, lsl #3
00392a5c: add r2, r2, r2, lsl #6
00392a60: add r2, r3, r2, lsl #3
00392a64: add r2, r2, r2, lsl #15
00392a68: add r3, r3, r2, lsl #3
00392a6c: rsb r3, r3, #0
00392a70: cmp r3, #1
00392a74: beq #0x3929b0
00392a78: cmp r3, #3
00392a7c: bne #0x392940
00392a80: mov r1, #0
00392a84: mov r0, r6
00392a88: bl #0x37baf8
00392a8c: bl #0x31bbf0
00392a90: mov r1, #1
00392a94: mov r7, r0
00392a98: mov r0, r6
00392a9c: bl #0x37baf8
00392aa0: bl #0x31bbf0
00392aa4: mov r1, #2
00392aa8: mov r5, r0
00392aac: mov r0, r6
00392ab0: bl #0x37baf8
00392ab4: bl #0x31bbf0
00392ab8: add r1, sp, #8
00392abc: str r0, [sp, #0x10]
00392ac0: mov r2, #1
00392ac4: mov r0, r4
00392ac8: str r7, [sp, #8]
00392acc: str r5, [sp, #0xc]
00392ad0: bl #0x393db4
00392ad4: ldr r2, [sp, #0xc]
00392ad8: ldr r3, [sp, #0x10]
00392adc: ldr r1, [sp, #8]
00392ae0: mov r0, r4
00392ae4: str r2, [r4, #0x1e4]
00392ae8: str r1, [r4, #0x1e0]
00392aec: str r3, [r4, #0x1e8]
00392af0: bl #0x393e90
00392af4: b #0x392940
00392af8: ldr r3, [r6, #4]
00392afc: ldr r2, [r3, #4]
00392b00: ldr r3, [r3]
00392b04: rsb r2, r3, r2
00392b08: asr r2, r2, #4
00392b0c: add r3, r2, r2, lsl #3
00392b10: add r3, r3, r3, lsl #6
00392b14: add r3, r2, r3, lsl #3
00392b18: add r3, r3, r3, lsl #15
00392b1c: add r3, r2, r3, lsl #3
00392b20: rsb r3, r3, #0
00392b24: b #0x392a70
00392b28: ldr r3, [pc, #0x4c]
00392b2c: mov r1, #0
00392b30: mov r0, r6
00392b34: ldr r3, [r5, r3]
00392b38: add r5, sp, #0x14
00392b3c: ldr r6, [r3, #0x38]
00392b40: bl #0x37baf8
00392b44: bl #0x31c49c
00392b48: mov ip, #0
00392b4c: mov r2, r0
00392b50: mov r1, r6
00392b54: mov r0, r5
00392b58: mvn r3, #0
00392b5c: str ip, [sp, #4]
00392b60: str ip, [sp]
00392b64: bl #0x34aca0
00392b68: mov r0, r5
00392b6c: bl #0x33fee4
00392b70: mov r5, r0
00392b74: b #0x3929dc
00392b78: rsbeq r2, r0, ip, lsl #3
00392b7c: strdeq r3, r4, [r0], -r4

_ZN9LuaScript10_PlaySoundERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e578 440
0037e578: push {r4, r5, r6, r7, r8, lr}
0037e57c: ldr r6, [r0, #4]
0037e580: mov r5, r0
0037e584: ldr r4, [pc, #0x18c]
0037e588: ldm r6, {r0, r3}
0037e58c: add r4, pc, r4
0037e590: sub sp, sp, #8
0037e594: rsb r3, r0, r3
0037e598: asr r3, r3, #4
0037e59c: add r2, r3, r3, lsl #3
0037e5a0: add r2, r2, r2, lsl #6
0037e5a4: add r2, r3, r2, lsl #3
0037e5a8: add r2, r2, r2, lsl #15
0037e5ac: add r3, r3, r2, lsl #3
0037e5b0: rsb r3, r3, #0
0037e5b4: cmp r3, #3
0037e5b8: bhi #0x37e5cc
0037e5bc: ldr r0, [pc, #0x158]
0037e5c0: add r0, pc, r0
0037e5c4: bl #0x708eb0
0037e5c8: ldr r0, [r6]
0037e5cc: add r0, r0, #0x150
0037e5d0: bl #0x31bc80
0037e5d4: cmp r0, #0
0037e5d8: bne #0x37e700
0037e5dc: ldr r6, [r5, #4]
0037e5e0: ldm r6, {r0, r3}
0037e5e4: rsb r3, r0, r3
0037e5e8: asr r3, r3, #4
0037e5ec: add r2, r3, r3, lsl #3
0037e5f0: add r2, r2, r2, lsl #6
0037e5f4: add r2, r3, r2, lsl #3
0037e5f8: add r2, r2, r2, lsl #15
0037e5fc: add r3, r3, r2, lsl #3
0037e600: cmp r3, #0
0037e604: bne #0x37e618
0037e608: ldr r0, [pc, #0x110]
0037e60c: add r0, pc, r0
0037e610: bl #0x708eb0
0037e614: ldr r0, [r6]
0037e618: bl #0x31c49c
0037e61c: bl #0x37ba84
0037e620: cmn r0, #1
0037e624: mov r7, r0
0037e628: beq #0x37e6d0
0037e62c: ldr r8, [r5, #4]
0037e630: ldr r2, [pc, #0xec]
0037e634: ldm r8, {r0, r3}
0037e638: ldr r2, [r4, r2]
0037e63c: rsb r3, r0, r3
0037e640: asr r3, r3, #4
0037e644: ldr r6, [r2]
0037e648: add r2, r3, r3, lsl #3
0037e64c: add r2, r2, r2, lsl #6
0037e650: add r2, r3, r2, lsl #3
0037e654: add r2, r2, r2, lsl #15
0037e658: add r3, r3, r2, lsl #3
0037e65c: rsb r3, r3, #0
0037e660: cmp r3, #1
0037e664: bls #0x37e6ec
0037e668: add r0, r0, #0x70
0037e66c: bl #0x31bc80
0037e670: ldr r5, [r5, #4]
0037e674: mov r4, r0
0037e678: ldm r5, {r0, r3}
0037e67c: rsb r3, r0, r3
0037e680: asr r3, r3, #4
0037e684: add r2, r3, r3, lsl #3
0037e688: add r2, r2, r2, lsl #6
0037e68c: add r2, r3, r2, lsl #3
0037e690: add r2, r2, r2, lsl #15
0037e694: add r3, r3, r2, lsl #3
0037e698: rsb r3, r3, #0
0037e69c: cmp r3, #2
0037e6a0: bls #0x37e6d8
0037e6a4: add r0, r0, #0xe0
0037e6a8: bl #0x31bbf0
0037e6ac: bl #0x30e4cc
0037e6b0: mov ip, #0
0037e6b4: mov r3, r0
0037e6b8: mov r1, r7
0037e6bc: mov r0, r6
0037e6c0: mov r2, r4
0037e6c4: str ip, [sp, #4]
0037e6c8: str ip, [sp]
0037e6cc: bl #0x36b80c
0037e6d0: add sp, sp, #8
0037e6d4: pop {r4, r5, r6, r7, r8, pc}
0037e6d8: ldr r0, [pc, #0x48]
0037e6dc: add r0, pc, r0
0037e6e0: bl #0x708eb0
0037e6e4: ldr r0, [r5]
0037e6e8: b #0x37e6a4
0037e6ec: ldr r0, [pc, #0x38]
0037e6f0: add r0, pc, r0
0037e6f4: bl #0x708eb0
0037e6f8: ldr r0, [r8]
0037e6fc: b #0x37e668
0037e700: ldr r3, [pc, #0x1c]
0037e704: mov r1, #0
0037e708: ldr r3, [r4, r3]
0037e70c: ldr r0, [r3]
0037e710: bl #0x36a1a0
0037e714: b #0x37e5dc
0037e718: rsbeq r6, r1, r4, lsl #10
0037e71c: subseq pc, r3, r8, lsr #29
0037e720: subseq pc, r3, ip, asr lr
0037e724: andeq r0, r0, r4, lsr #27
0037e728: subseq pc, r3, ip, lsl #27
0037e72c: subseq pc, r3, r8, ror sp

_ZN3sfc6script3lua12ReturnValues9_doReturnEP9lua_State 0x31b308 144
0031b308: push {r4, r5, r6, r7, r8, lr}
0031b30c: ldr r3, [r0, #0x24]
0031b310: mov r7, r1
0031b314: mov r6, r0
0031b318: ldr r1, [r3, #4]
0031b31c: ldr r2, [r3]
0031b320: rsb r3, r2, r1
0031b324: asr r3, r3, #4
0031b328: add r0, r3, r3, lsl #3
0031b32c: add r0, r0, r0, lsl #6
0031b330: add r0, r3, r0, lsl #3
0031b334: add r0, r0, r0, lsl #15
0031b338: add r0, r3, r0, lsl #3
0031b33c: rsb r0, r0, #0
0031b340: cmp r0, #0
0031b344: beq #0x31b394
0031b348: mov r4, #0
0031b34c: mov r5, r4
0031b350: add r0, r2, r4
0031b354: mov r1, r7
0031b358: bl #0x31cac4
0031b35c: ldr r2, [r6, #0x24]
0031b360: add r5, r5, #1
0031b364: add r4, r4, #0x70
0031b368: ldm r2, {r2, r3}
0031b36c: rsb r3, r2, r3
0031b370: asr r3, r3, #4
0031b374: add r1, r3, r3, lsl #3
0031b378: add r1, r1, r1, lsl #6
0031b37c: add r1, r3, r1, lsl #3
0031b380: add r1, r1, r1, lsl #15
0031b384: add r3, r3, r1, lsl #3
0031b388: rsb r0, r3, #0
0031b38c: cmp r5, r0
0031b390: blo #0x31b350
0031b394: pop {r4, r5, r6, r7, r8, pc}

_ZN10GameObject7_GrabFXERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x392620 244
00392620: push {r4, r5, r6, r7, r8, lr}
00392624: ldr r3, [r0, #4]
00392628: mov r6, r1
0039262c: mov r7, r2
00392630: ldr r1, [r3, #4]
00392634: ldr r3, [r3]
00392638: ldr r4, [pc, #0xc4]
0039263c: mov r5, r0
00392640: rsb r1, r3, r1
00392644: asr r1, r1, #4
00392648: add r4, pc, r4
0039264c: add r2, r1, r1, lsl #3
00392650: add r2, r2, r2, lsl #6
00392654: add r2, r1, r2, lsl #3
00392658: add r2, r2, r2, lsl #15
0039265c: add r1, r1, r2, lsl #3
00392660: cmp r1, #0
00392664: bne #0x39266c
00392668: pop {r4, r5, r6, r7, r8, pc}
0039266c: ldr r3, [r3, #4]
00392670: cmp r3, #3
00392674: bne #0x392668
00392678: mov r1, #0
0039267c: bl #0x37baf8
00392680: bl #0x38d798
00392684: ldr r3, [pc, #0x7c]
00392688: ldr r3, [r4, r3]
0039268c: ldr r3, [r3]
00392690: cmp r0, r3
00392694: bhs #0x392668
00392698: ldr r5, [r5, #4]
0039269c: ldm r5, {r0, r3}
003926a0: rsb r3, r0, r3
003926a4: asr r3, r3, #4
003926a8: add r2, r3, r3, lsl #3
003926ac: add r2, r2, r2, lsl #6
003926b0: add r2, r3, r2, lsl #3
003926b4: add r2, r2, r2, lsl #15
003926b8: add r3, r3, r2, lsl #3
003926bc: cmp r3, #0
003926c0: bne #0x3926d4
003926c4: ldr r0, [pc, #0x40]
003926c8: add r0, pc, r0
003926cc: bl #0x708eb0
003926d0: ldr r0, [r5]
003926d4: bl #0x31bbf0
003926d8: ldr r3, [pc, #0x30]
003926dc: ldr r4, [r4, r3]
003926e0: bl #0x8be2a0
003926e4: mov r2, r7
003926e8: mov r1, r0
003926ec: mov r0, r4
003926f0: bl #0x495430
003926f4: mov r1, r0
003926f8: mov r0, r6
003926fc: pop {r4, r5, r6, r7, r8, lr}
00392700: b #0x38eb00
00392704: rsbeq r2, r0, r8, asr #8
00392708: andeq r0, r0, r4, asr #13
0039270c: subseq fp, r2, r0, lsr #27
00392710: andeq r1, r0, r8, lsl #22

_ZN9Character10_StopSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b94ac 212
003b94ac: push {r4, r5, r6, r7, r8, lr}
003b94b0: ldr r3, [r0, #4]
003b94b4: mov r4, r0
003b94b8: mov r5, r2
003b94bc: ldm r3, {r0, r2}
003b94c0: rsb r3, r0, r2
003b94c4: asr r3, r3, #4
003b94c8: add r2, r3, r3, lsl #3
003b94cc: add r2, r2, r2, lsl #6
003b94d0: add r2, r3, r2, lsl #3
003b94d4: add r2, r2, r2, lsl #15
003b94d8: add r3, r3, r2, lsl #3
003b94dc: cmp r3, #0
003b94e0: bne #0x3b94e8
003b94e4: pop {r4, r5, r6, r7, r8, pc}
003b94e8: ldr r3, [r0, #4]
003b94ec: cmp r3, #3
003b94f0: beq #0x3b9574
003b94f4: bl #0x31bbf0
003b94f8: mov r7, r0
003b94fc: mov r0, r5
003b9500: bl #0x3bc5fc
003b9504: mov r6, r0
003b9508: mov r0, r7
003b950c: bl #0x8be2a0
003b9510: ldr r3, [r6, #4]
003b9514: cmp r3, r0
003b9518: bls #0x3b94e4
003b951c: ldr r4, [r4, #4]
003b9520: add r5, r5, #0x3c8
003b9524: ldm r4, {r0, r3}
003b9528: rsb r3, r0, r3
003b952c: asr r3, r3, #4
003b9530: add r2, r3, r3, lsl #3
003b9534: add r2, r2, r2, lsl #6
003b9538: add r2, r3, r2, lsl #3
003b953c: add r2, r2, r2, lsl #15
003b9540: add r3, r3, r2, lsl #3
003b9544: cmp r3, #0
003b9548: bne #0x3b955c
003b954c: ldr r0, [pc, #0x28]
003b9550: add r0, pc, r0
003b9554: bl #0x708eb0
003b9558: ldr r0, [r4]
003b955c: bl #0x31bbf0
003b9560: bl #0x8be2a0
003b9564: mov r1, r0
003b9568: mov r0, r5
003b956c: pop {r4, r5, r6, r7, r8, lr}
003b9570: b #0x3d8474
003b9574: add r5, r5, #0x3c8
003b9578: b #0x3b955c
003b957c: subseq r4, r0, r8, lsl pc

_ZN10LuaManagerD2Ev 0x37a12c 100
0037a12c: ldr r3, [pc, #0x54]
0037a130: ldr r2, [pc, #0x54]
0037a134: push {r4, r5, r6, lr}
0037a138: add r3, pc, r3
0037a13c: ldr r2, [r3, r2]
0037a140: mov r4, r0
0037a144: add r2, r2, #8
0037a148: str r2, [r0]
0037a14c: bl #0x379fe8
0037a150: ldr r3, [r4, #0x14]
0037a154: cmp r3, #0
0037a158: beq #0x37a180
0037a15c: add r5, r4, #4
0037a160: mov r0, r5
0037a164: ldr r1, [r4, #8]
0037a168: bl #0x379fa8
0037a16c: mov r3, #0
0037a170: str r5, [r4, #0x10]
0037a174: str r3, [r4, #0x14]
0037a178: str r5, [r4, #0xc]
0037a17c: str r3, [r4, #8]
0037a180: mov r0, r4
0037a184: pop {r4, r5, r6, pc}
0037a188: rsbeq sl, r1, r8, asr sb
0037a18c: andeq r3, r0, r8, ror #26

_ZN3sfc6script3lua12ReturnValues11pushIntegerEi 0x37cb24 104
0037cb24: ldr r3, [pc, #0x58]
0037cb28: ldr r2, [pc, #0x58]
0037cb2c: push {r4, r5, r6, lr}
0037cb30: add r3, pc, r3
0037cb34: ldr r5, [r3, r2]
0037cb38: sub sp, sp, #0x78
0037cb3c: add r4, sp, #4
0037cb40: ldr r3, [r5]
0037cb44: str r3, [sp, #0x74]
0037cb48: ldr r6, [r0, #0x24]
0037cb4c: mov r0, r4
0037cb50: bl #0x37ca9c
0037cb54: mov r0, r6
0037cb58: mov r1, r4
0037cb5c: bl #0x3195c0
0037cb60: mov r0, r4
0037cb64: bl #0x3193e8
0037cb68: ldr r2, [sp, #0x74]
0037cb6c: ldr r3, [r5]
0037cb70: cmp r2, r3
0037cb74: bne #0x37cb80
0037cb78: add sp, sp, #0x78
0037cb7c: pop {r4, r5, r6, pc}
0037cb80: bl #0x30e310
0037cb84: rsbeq r7, r1, r0, ror #30
0037cb88: andeq r4, r0, ip, lsr #1

_ZN9Character13_LookAtTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b5684 12
003b5684: ldr r1, [r2, #0x408]
003b5688: mov r0, r2
003b568c: b #0x393d48

_ZN11TriggerTrap7_RemoveERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x39dd18 128
0039dd18: push {r4, lr}
0039dd1c: ldr r3, [r2, #0x2d8]
0039dd20: mov r4, r2
0039dd24: mov r2, #1
0039dd28: cmp r3, #0
0039dd2c: sub sp, sp, #8
0039dd30: strb r2, [r4, #0x400]
0039dd34: beq #0x39dd68
0039dd38: ldr ip, [r3, #0x38]
0039dd3c: ldr r1, [pc, #0x50]
0039dd40: mov r3, #0
0039dd44: mov r0, ip
0039dd48: mov r2, r3
0039dd4c: ldr ip, [ip]
0039dd50: add r1, pc, r1
0039dd54: str r3, [sp]
0039dd58: mov lr, pc
0039dd5c: ldr pc, [ip, #0x20]
0039dd60: cmp r0, #0
0039dd64: bne #0x39dd8c
0039dd68: mov r0, r4
0039dd6c: ldr r3, [r4]
0039dd70: mov r1, #0
0039dd74: mov lr, pc
0039dd78: ldr pc, [r3, #0x40]
0039dd7c: mov r0, r4
0039dd80: add sp, sp, #8
0039dd84: pop {r4, lr}
0039dd88: b #0x33ddb4
0039dd8c: add sp, sp, #8
0039dd90: pop {r4, pc}
0039dd94: subseq r5, r2, r8, lsr #2

_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EED1Ev 0x31bf04 136
0031bf04: push {r4, r5, r6, lr}
0031bf08: ldr r5, [r0, #4]
0031bf0c: ldr r6, [r0]
0031bf10: mov r4, r0
0031bf14: cmp r5, r6
0031bf18: beq #0x31bf34
0031bf1c: ldr r3, [r5, #-0x70]!
0031bf20: mov r0, r5
0031bf24: mov lr, pc
0031bf28: ldr pc, [r3]
0031bf2c: cmp r6, r5
0031bf30: bne #0x31bf1c
0031bf34: ldr r0, [r4]
0031bf38: cmp r0, #0
0031bf3c: beq #0x31bf78
0031bf40: ldr r3, [r4, #8]
0031bf44: mov r1, #0x70
0031bf48: rsb r3, r0, r3
0031bf4c: asr r3, r3, #4
0031bf50: add r2, r3, r3, lsl #3
0031bf54: add r2, r2, r2, lsl #6
0031bf58: add r2, r3, r2, lsl #3
0031bf5c: add r2, r2, r2, lsl #15
0031bf60: add r3, r3, r2, lsl #3
0031bf64: rsb r3, r3, #0
0031bf68: mul r1, r1, r3
0031bf6c: cmp r1, #0x80
0031bf70: bhi #0x31bf80
0031bf74: bl #0x708f00
0031bf78: mov r0, r4
0031bf7c: pop {r4, r5, r6, pc}
0031bf80: bl #0x310440
0031bf84: mov r0, r4
0031bf88: pop {r4, r5, r6, pc}

_ZN3sfc6script3lua9ArgumentsD2Ev 0x31927c 56
0031927c: ldr r3, [pc, #0x28]
00319280: ldr r2, [pc, #0x28]
00319284: push {r4, lr}
00319288: add r3, pc, r3
0031928c: ldr r2, [r3, r2]
00319290: mov r4, r0
00319294: ldr r0, [r0, #4]
00319298: add r2, r2, #8
0031929c: str r2, [r4]
003192a0: bl #0x31d194
003192a4: mov r0, r4
003192a8: pop {r4, pc}
003192ac: rsbeq fp, r7, r8, lsl #16
003192b0: andeq r2, r0, r0, ror #6

_ZN9Character7_HeadToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3ba71c 1216
003ba71c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ba720: ldr r6, [r0, #4]
003ba724: mov r5, r2
003ba728: ldr r7, [pc, #0x4a0]
003ba72c: ldm r6, {r1, r3}
003ba730: add r7, pc, r7
003ba734: sub sp, sp, #0x2c
003ba738: rsb r3, r1, r3
003ba73c: asr r3, r3, #4
003ba740: mov r4, r0
003ba744: add r2, r3, r3, lsl #3
003ba748: add r2, r2, r2, lsl #6
003ba74c: add r2, r3, r2, lsl #3
003ba750: add r2, r2, r2, lsl #15
003ba754: add r3, r3, r2, lsl #3
003ba758: rsb r3, r3, #0
003ba75c: cmp r3, #1
003ba760: beq #0x3ba864
003ba764: cmp r3, #2
003ba768: bls #0x3ba85c
003ba76c: cmp r3, #0
003ba770: bne #0x3ba784
003ba774: ldr r0, [pc, #0x458]
003ba778: add r0, pc, r0
003ba77c: bl #0x708eb0
003ba780: ldr r1, [r6]
003ba784: ldr r3, [r1, #4]
003ba788: cmp r3, #3
003ba78c: bne #0x3ba8c8
003ba790: ldr r2, [r4, #4]
003ba794: ldr r3, [r2]
003ba798: ldr r1, [r2, #4]
003ba79c: rsb r1, r3, r1
003ba7a0: asr r1, r1, #4
003ba7a4: add r3, r1, r1, lsl #3
003ba7a8: add r3, r3, r3, lsl #6
003ba7ac: add r3, r1, r3, lsl #3
003ba7b0: add r3, r3, r3, lsl #15
003ba7b4: add r3, r1, r3, lsl #3
003ba7b8: rsb r3, r3, #0
003ba7bc: cmp r3, #1
003ba7c0: beq #0x3babac
003ba7c4: cmp r3, #2
003ba7c8: bls #0x3ba85c
003ba7cc: mov r8, #0
003ba7d0: str r8, [sp, #0x1c]
003ba7d4: str r8, [sp, #0x20]
003ba7d8: str r8, [sp, #0x24]
003ba7dc: ldr r3, [r2]
003ba7e0: ldr r2, [r2, #4]
003ba7e4: rsb r3, r3, r2
003ba7e8: asr r3, r3, #4
003ba7ec: add r2, r3, r3, lsl #3
003ba7f0: add r2, r2, r2, lsl #6
003ba7f4: add r2, r3, r2, lsl #3
003ba7f8: add r2, r2, r2, lsl #15
003ba7fc: add r3, r3, r2, lsl #3
003ba800: rsb r3, r3, #0
003ba804: cmp r3, #3
003ba808: bhi #0x3ba95c
003ba80c: mov r1, #0
003ba810: mov r0, r4
003ba814: bl #0x37baf8
003ba818: bl #0x31bbf0
003ba81c: mov r1, #1
003ba820: mov r7, r0
003ba824: mov r0, r4
003ba828: bl #0x37baf8
003ba82c: bl #0x31bbf0
003ba830: mov r1, #2
003ba834: mov r6, r0
003ba838: mov r0, r4
003ba83c: bl #0x37baf8
003ba840: bl #0x31bbf0
003ba844: str r7, [sp, #0x1c]
003ba848: str r6, [sp, #0x20]
003ba84c: str r0, [sp, #0x24]
003ba850: ldr r0, [r5, #0x378]
003ba854: add r1, sp, #0x1c
003ba858: bl #0x40542c
003ba85c: add sp, sp, #0x2c
003ba860: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ba864: mov r1, #0
003ba868: bl #0x37baf8
003ba86c: ldr r3, [r0, #4]
003ba870: cmp r3, #2
003ba874: beq #0x3ba928
003ba878: mov r0, r4
003ba87c: mov r1, #0
003ba880: bl #0x37baf8
003ba884: ldr r3, [r0, #4]
003ba888: cmp r3, #7
003ba88c: bne #0x3ba85c
003ba890: ldr r2, [r4, #4]
003ba894: ldm r2, {r1, r3}
003ba898: mov r6, r2
003ba89c: rsb r3, r1, r3
003ba8a0: asr r3, r3, #4
003ba8a4: add r0, r3, r3, lsl #3
003ba8a8: add r0, r0, r0, lsl #6
003ba8ac: add r0, r3, r0, lsl #3
003ba8b0: add r0, r0, r0, lsl #15
003ba8b4: add r3, r3, r0, lsl #3
003ba8b8: rsb r3, r3, #0
003ba8bc: cmp r3, #2
003ba8c0: bls #0x3ba7bc
003ba8c4: b #0x3ba76c
003ba8c8: mov r0, r4
003ba8cc: mov r1, #1
003ba8d0: bl #0x37baf8
003ba8d4: ldr r3, [r0, #4]
003ba8d8: cmp r3, #3
003ba8dc: beq #0x3ba8f8
003ba8e0: mov r0, r4
003ba8e4: mov r1, #2
003ba8e8: bl #0x37baf8
003ba8ec: ldr r3, [r0, #4]
003ba8f0: cmp r3, #3
003ba8f4: bne #0x3ba85c
003ba8f8: ldr r2, [r4, #4]
003ba8fc: ldr r1, [r2, #4]
003ba900: ldr r3, [r2]
003ba904: rsb r3, r3, r1
003ba908: asr r3, r3, #4
003ba90c: add r1, r3, r3, lsl #3
003ba910: add r1, r1, r1, lsl #6
003ba914: add r1, r3, r1, lsl #3
003ba918: add r1, r1, r1, lsl #15
003ba91c: add r3, r3, r1, lsl #3
003ba920: rsb r3, r3, #0
003ba924: b #0x3ba7bc
003ba928: ldr r2, [r4, #4]
003ba92c: ldr r1, [r2]
003ba930: ldr r0, [r2, #4]
003ba934: mov r6, r2
003ba938: rsb r0, r1, r0
003ba93c: asr r0, r0, #4
003ba940: add r3, r0, r0, lsl #3
003ba944: add r3, r3, r3, lsl #6
003ba948: add r3, r0, r3, lsl #3
003ba94c: add r3, r3, r3, lsl #15
003ba950: add r3, r0, r3, lsl #3
003ba954: rsb r3, r3, #0
003ba958: b #0x3ba8bc
003ba95c: mov r0, r4
003ba960: mov r1, #3
003ba964: bl #0x37baf8
003ba968: ldr r6, [r0, #4]
003ba96c: cmp r6, #1
003ba970: bne #0x3ba80c
003ba974: mov r1, #3
003ba978: mov r0, r4
003ba97c: bl #0x37baf8
003ba980: bl #0x31bc80
003ba984: cmp r0, #0
003ba988: beq #0x3ba80c
003ba98c: mov r0, r5
003ba990: add r1, sp, #0x10
003ba994: str r8, [sp, #0x18]
003ba998: str r8, [sp, #0x10]
003ba99c: str r8, [sp, #0x14]
003ba9a0: bl #0x393ae4
003ba9a4: ldr r3, [pc, #0x22c]
003ba9a8: ldr ip, [r5, #0x160]
003ba9ac: ldr r2, [r5, #0x164]
003ba9b0: ldr r7, [r7, r3]
003ba9b4: ldr r3, [r5, #0x168]
003ba9b8: mov r1, #0
003ba9bc: mov r0, r4
003ba9c0: str r3, [sp, #0x24]
003ba9c4: ldr r3, [r7, #4]
003ba9c8: str ip, [sp, #0x1c]
003ba9cc: str r2, [sp, #0x20]
003ba9d0: str r3, [sp, #4]
003ba9d4: ldr r3, [sp, #0x14]
003ba9d8: ldr fp, [r7, #8]
003ba9dc: ldr sl, [r7]
003ba9e0: str r3, [sp, #8]
003ba9e4: ldr r3, [sp, #0x10]
003ba9e8: ldr sb, [sp, #0x18]
003ba9ec: str r3, [sp, #0xc]
003ba9f0: bl #0x37baf8
003ba9f4: bl #0x31bbf0
003ba9f8: mov r1, sb
003ba9fc: mov r8, r0
003baa00: ldr r0, [sp, #4]
003baa04: bl #0x30ed6c
003baa08: ldr r1, [sp, #8]
003baa0c: mov r3, r0
003baa10: mov r0, fp
003baa14: str r3, [sp]
003baa18: bl #0x30ed6c
003baa1c: ldr r3, [sp]
003baa20: mov r1, r0
003baa24: mov r0, r3
003baa28: bl #0x30e3ac
003baa2c: mov r1, r0
003baa30: mov r0, r8
003baa34: bl #0x30ed6c
003baa38: mov r1, r0
003baa3c: ldr r0, [sp, #0x1c]
003baa40: bl #0x30eba4
003baa44: ldr r1, [sp, #0xc]
003baa48: str r0, [sp, #0x1c]
003baa4c: mov r0, fp
003baa50: bl #0x30ed6c
003baa54: mov r1, sl
003baa58: mov fp, r0
003baa5c: mov r0, sb
003baa60: bl #0x30ed6c
003baa64: mov r1, r0
003baa68: mov r0, fp
003baa6c: bl #0x30e3ac
003baa70: mov r1, r0
003baa74: mov r0, r8
003baa78: bl #0x30ed6c
003baa7c: mov r1, r0
003baa80: ldr r0, [sp, #0x20]
003baa84: bl #0x30eba4
003baa88: mov r1, sl
003baa8c: str r0, [sp, #0x20]
003baa90: ldr r0, [sp, #8]
003baa94: bl #0x30ed6c
003baa98: ldr r1, [sp, #0xc]
003baa9c: mov sl, r0
003baaa0: ldr r0, [sp, #4]
003baaa4: bl #0x30ed6c
003baaa8: mov r1, r0
003baaac: mov r0, sl
003baab0: bl #0x30e3ac
003baab4: mov r1, r0
003baab8: mov r0, r8
003baabc: bl #0x30ed6c
003baac0: mov r1, r0
003baac4: ldr r0, [sp, #0x24]
003baac8: bl #0x30eba4
003baacc: mov r1, r6
003baad0: str r0, [sp, #0x24]
003baad4: mov r0, r4
003baad8: bl #0x37baf8
003baadc: bl #0x31bbf0
003baae0: ldr r1, [sp, #0x14]
003baae4: mov r6, r0
003baae8: bl #0x30ed6c
003baaec: ldr r1, [sp, #0x18]
003baaf0: mov sl, r0
003baaf4: mov r0, r6
003baaf8: bl #0x30ed6c
003baafc: ldr r1, [sp, #0x10]
003bab00: mov r8, r0
003bab04: mov r0, r6
003bab08: bl #0x30ed6c
003bab0c: mov r1, r0
003bab10: ldr r0, [sp, #0x1c]
003bab14: bl #0x30eba4
003bab18: mov r1, sl
003bab1c: str r0, [sp, #0x1c]
003bab20: ldr r0, [sp, #0x20]
003bab24: bl #0x30eba4
003bab28: mov r1, r8
003bab2c: str r0, [sp, #0x20]
003bab30: ldr r0, [sp, #0x24]
003bab34: bl #0x30eba4
003bab38: mov r1, #2
003bab3c: str r0, [sp, #0x24]
003bab40: mov r0, r4
003bab44: bl #0x37baf8
003bab48: bl #0x31bbf0
003bab4c: ldr r1, [r7, #4]
003bab50: mov r4, r0
003bab54: bl #0x30ed6c
003bab58: ldr r1, [r7, #8]
003bab5c: mov r8, r0
003bab60: mov r0, r4
003bab64: bl #0x30ed6c
003bab68: ldr r1, [r7]
003bab6c: mov r6, r0
003bab70: mov r0, r4
003bab74: bl #0x30ed6c
003bab78: mov r1, r0
003bab7c: ldr r0, [sp, #0x1c]
003bab80: bl #0x30eba4
003bab84: mov r1, r8
003bab88: str r0, [sp, #0x1c]
003bab8c: ldr r0, [sp, #0x20]
003bab90: bl #0x30eba4
003bab94: mov r1, r6
003bab98: str r0, [sp, #0x20]
003bab9c: ldr r0, [sp, #0x24]
003baba0: bl #0x30eba4
003baba4: str r0, [sp, #0x24]
003baba8: b #0x3ba850
003babac: mov r1, #0
003babb0: mov r0, r4
003babb4: ldr r4, [r5, #0x378]
003babb8: bl #0x37baf8
003babbc: bl #0x31b5a0
003babc0: mov r1, r0
003babc4: mov r0, r4
003babc8: bl #0x405540
003babcc: b #0x3ba85c
003babd0: subseq sl, sp, r0, ror #6
003babd4: ldrsheq r3, [r0], #-0xc0
003babd8: andeq r4, r0, r0, asr #6

_ZN9Character10_GetPropHPERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6cd8 132
003b6cd8: movw r3, #0x1090
003b6cdc: push {r4, r5, r6, lr}
003b6ce0: ldr r5, [r2, r3]
003b6ce4: movw r3, #0x1088
003b6ce8: mov r4, r1
003b6cec: cmp r5, #0
003b6cf0: ldr r6, [r2, r3]
003b6cf4: beq #0x3b6d34
003b6cf8: asr r5, r5, #8
003b6cfc: mov r0, r1
003b6d00: asr r1, r6, #8
003b6d04: bl #0x37cb24
003b6d08: mov r0, r4
003b6d0c: mov r1, r5
003b6d10: bl #0x37cb24
003b6d14: mov r0, #0x64
003b6d18: mov r1, r5
003b6d1c: mul r0, r0, r6
003b6d20: bl #0x30e2a4
003b6d24: asr r1, r0, #8
003b6d28: mov r0, r4
003b6d2c: pop {r4, r5, r6, lr}
003b6d30: b #0x37cb24
003b6d34: mov r0, r1
003b6d38: mov r1, r5
003b6d3c: bl #0x37cb24
003b6d40: mov r0, r4
003b6d44: mov r1, r5
003b6d48: bl #0x37cb24
003b6d4c: mov r0, r4
003b6d50: mov r1, r5
003b6d54: pop {r4, r5, r6, lr}
003b6d58: b #0x37cb24

_ZN3sfc6script3lua8Instance8loadFileER12StreamBuffer 0x31acf4 244
0031acf4: push {r4, r5, r6, r7, r8, sl, lr}
0031acf8: ldr r4, [pc, #0xd8]
0031acfc: ldr r8, [pc, #0xd8]
0031ad00: sub sp, sp, #0x410
0031ad04: add r4, pc, r4
0031ad08: ldr r3, [r4, r8]
0031ad0c: sub sp, sp, #0xc
0031ad10: mov r6, r1
0031ad14: ldr r3, [r3]
0031ad18: mov sl, r2
0031ad1c: mov r5, r0
0031ad20: str r3, [sp, #0x414]
0031ad24: bl #0x31a804
0031ad28: ldr r2, [pc, #0xb0]
0031ad2c: ldr r7, [r6, #4]
0031ad30: mov ip, #0
0031ad34: ldr r3, [pc, #0xa8]
0031ad38: str ip, [sp, #4]
0031ad3c: add ip, sp, #0x18
0031ad40: ldr r1, [r4, r2]
0031ad44: sub ip, ip, #4
0031ad48: add r2, sp, #8
0031ad4c: add r3, pc, r3
0031ad50: sub r2, r2, #8
0031ad54: str ip, [sp, #0xc]
0031ad58: mov r0, r7
0031ad5c: mov ip, #0x400
0031ad60: str ip, [sp, #0x10]
0031ad64: str sl, [sp, #8]
0031ad68: str r6, [sp]
0031ad6c: bl #0x84bbbc
0031ad70: mov r1, r7
0031ad74: mov r2, r0
0031ad78: mov r0, r5
0031ad7c: bl #0x31a8ac
0031ad80: ldr r1, [r5, #4]
0031ad84: cmp r1, #0
0031ad88: bne #0x31adb0
0031ad8c: ldr r6, [r6, #4]
0031ad90: mov r2, r1
0031ad94: mov r3, r1
0031ad98: mov r0, r6
0031ad9c: bl #0x84bc50
0031ada0: mov r1, r6
0031ada4: mov r2, r0
0031ada8: mov r0, r5
0031adac: bl #0x31a8ac
0031adb0: ldr r3, [r4, r8]
0031adb4: ldr r2, [sp, #0x414]
0031adb8: mov r0, r5
0031adbc: ldr r3, [r3]
0031adc0: cmp r2, r3
0031adc4: bne #0x31add4
0031adc8: add sp, sp, #0x1c
0031adcc: add sp, sp, #0x400
0031add0: pop {r4, r5, r6, r7, r8, sl, pc}
0031add4: bl #0x30e310
0031add8: rsbeq sb, r7, ip, lsl #27
0031addc: andeq r4, r0, ip, lsr #1
0031ade0: andeq r0, r0, r4, asr sb
0031ade4: subseq r3, sl, ip, lsl fp

_ZN9Character11_RemoveBuffERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b842c 320
003b842c: push {r4, r5, lr}
003b8430: ldr r3, [r0, #4]
003b8434: ldr r4, [pc, #0x128]
003b8438: sub sp, sp, #0xc
003b843c: ldr r1, [r3, #4]
003b8440: ldr ip, [r3]
003b8444: add r4, pc, r4
003b8448: mov r5, r0
003b844c: rsb r3, ip, r1
003b8450: asr r3, r3, #4
003b8454: add r1, r3, r3, lsl #3
003b8458: add r1, r1, r1, lsl #6
003b845c: add r1, r3, r1, lsl #3
003b8460: add r1, r1, r1, lsl #15
003b8464: add r3, r3, r1, lsl #3
003b8468: cmp r3, #0
003b846c: bne #0x3b8478
003b8470: add sp, sp, #0xc
003b8474: pop {r4, r5, pc}
003b8478: ldr r3, [ip, #4]
003b847c: cmp r3, #3
003b8480: bne #0x3b8470
003b8484: mov r1, #0
003b8488: str r2, [sp, #4]
003b848c: bl #0x37baf8
003b8490: bl #0x38d798
003b8494: ldr r3, [pc, #0xcc]
003b8498: ldr r2, [sp, #4]
003b849c: ldr r3, [r4, r3]
003b84a0: ldr r3, [r3]
003b84a4: cmp r0, r3
003b84a8: bhs #0x3b8470
003b84ac: ldr r3, [r5, #4]
003b84b0: ldr r1, [r3, #4]
003b84b4: ldr r3, [r3]
003b84b8: rsb r1, r3, r1
003b84bc: asr r1, r1, #4
003b84c0: add r0, r1, r1, lsl #3
003b84c4: add r0, r0, r0, lsl #6
003b84c8: add r0, r1, r0, lsl #3
003b84cc: add r0, r0, r0, lsl #15
003b84d0: add r1, r1, r0, lsl #3
003b84d4: rsb r1, r1, #0
003b84d8: cmp r1, #1
003b84dc: bls #0x3b8534
003b84e0: ldr r3, [r3, #0x74]
003b84e4: cmp r3, #2
003b84e8: bne #0x3b8470
003b84ec: mov r1, #0
003b84f0: mov r0, r5
003b84f4: str r2, [sp, #4]
003b84f8: bl #0x37baf8
003b84fc: bl #0x38d798
003b8500: mov r1, #1
003b8504: mov r4, r0
003b8508: mov r0, r5
003b850c: bl #0x37baf8
003b8510: bl #0x31b580
003b8514: ldr r2, [sp, #4]
003b8518: mov r3, r0
003b851c: mov r1, r4
003b8520: add r0, r2, #0x560
003b8524: mov r2, r3
003b8528: add sp, sp, #0xc
003b852c: pop {r4, r5, lr}
003b8530: b #0x3e101c
003b8534: mov r1, #0
003b8538: mov r0, r5
003b853c: str r2, [sp, #4]
003b8540: bl #0x37baf8
003b8544: bl #0x38d798
003b8548: ldr r2, [sp, #4]
003b854c: mov r1, r0
003b8550: add r0, r2, #0x560
003b8554: mov r2, #0
003b8558: add sp, sp, #0xc
003b855c: pop {r4, r5, lr}
003b8560: b #0x3e101c
003b8564: subseq ip, sp, ip, asr #12
003b8568: andeq r3, r0, r8, ror #10

_ZN3sfc6script3lua5ValueC1Ev 0x3194e0 88
003194e0: ldr r3, [pc, #0x48]
003194e4: ldr r1, [pc, #0x48]
003194e8: mov r2, r0
003194ec: add r3, pc, r3
003194f0: ldr r1, [r3, r1]
003194f4: push {r4, lr}
003194f8: add r1, r1, #8
003194fc: str r1, [r2], #0xc
00319500: mov ip, #0
00319504: add r1, r0, #0x24
00319508: mov r4, r0
0031950c: str r2, [r0, #0x20]
00319510: str r1, [r0, #0x68]
00319514: str ip, [r0, #0x24]
00319518: str r2, [r0, #0x1c]
0031951c: strb ip, [r0, #0xc]
00319520: str r1, [r0, #0x64]
00319524: bl #0x31b5c0
00319528: mov r0, r4
0031952c: pop {r4, pc}
00319530: rsbeq fp, r7, r4, lsr #11
00319534: muleq r0, r8, r7

_ZN9Character5_StopERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b56b4 8
003b56b4: ldr r0, [r2, #0x378]
003b56b8: b #0x40559c

_ZN3sfc6script3lua5ValueC1ERKS2_ 0x31c634 300
0031c634: ldr r2, [pc, #0x11c]
0031c638: ldr ip, [pc, #0x11c]
0031c63c: mov r3, r0
0031c640: add r2, pc, r2
0031c644: ldr ip, [r2, ip]
0031c648: push {r4, r5, r6, lr}
0031c64c: add ip, ip, #8
0031c650: mov r4, r0
0031c654: str ip, [r3], #0xc
0031c658: mov r0, r3
0031c65c: mov r5, r1
0031c660: str r3, [r4, #0x1c]
0031c664: str r3, [r4, #0x20]
0031c668: mov r1, #0x10
0031c66c: bl #0x31167c
0031c670: ldr r2, [r4, #0x1c]
0031c674: add r3, r4, #0x24
0031c678: mov r6, #0
0031c67c: strb r6, [r2]
0031c680: mov r0, r3
0031c684: str r3, [r4, #0x64]
0031c688: str r3, [r4, #0x68]
0031c68c: bl #0x31bd7c
0031c690: ldr r3, [r4, #0x64]
0031c694: str r6, [r3]
0031c698: ldr r3, [r5, #4]
0031c69c: cmp r3, r6
0031c6a0: beq #0x31c6cc
0031c6a4: cmp r3, #1
0031c6a8: beq #0x31c6f8
0031c6ac: cmp r3, #3
0031c6b0: beq #0x31c710
0031c6b4: cmp r3, #4
0031c6b8: beq #0x31c728
0031c6bc: cmp r3, #2
0031c6c0: beq #0x31c740
0031c6c4: cmp r3, #7
0031c6c8: beq #0x31c6dc
0031c6cc: mov r0, r4
0031c6d0: bl #0x31b5c0
0031c6d4: mov r0, r4
0031c6d8: pop {r4, r5, r6, pc}
0031c6dc: mov r0, r5
0031c6e0: bl #0x31b5a0
0031c6e4: mov r1, r0
0031c6e8: mov r0, r4
0031c6ec: bl #0x31b608
0031c6f0: mov r0, r4
0031c6f4: pop {r4, r5, r6, pc}
0031c6f8: mov r0, r5
0031c6fc: bl #0x31bc80
0031c700: mov r1, r0
0031c704: mov r0, r4
0031c708: bl #0x31b5cc
0031c70c: b #0x31c6d4
0031c710: mov r0, r5
0031c714: bl #0x31bbf0
0031c718: mov r1, r0
0031c71c: mov r0, r4
0031c720: bl #0x31b5e8
0031c724: b #0x31c6d4
0031c728: mov r0, r5
0031c72c: bl #0x31c49c
0031c730: mov r1, r0
0031c734: mov r0, r4
0031c738: bl #0x31c46c
0031c73c: b #0x31c6d4
0031c740: mov r0, r5
0031c744: bl #0x31b580
0031c748: mov r1, r0
0031c74c: mov r0, r4
0031c750: bl #0x31b5f8
0031c754: b #0x31c6d4
0031c758: rsbeq r8, r7, r0, asr r4
0031c75c: muleq r0, r8, r7

_ZN9LuaScript15_IncludePyArrayERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37b570 4
0037b570: bx lr

_ZN10GameObject10_IsInRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390f1c 1308
00390f1c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00390f20: ldr r7, [r0, #4]
00390f24: mov r6, r1
00390f28: mov r5, r2
00390f2c: ldr r3, [r7]
00390f30: ldr r1, [r7, #4]
00390f34: ldr r8, [pc, #0x4e8]
00390f38: sub sp, sp, #0x34
00390f3c: rsb r1, r3, r1
00390f40: asr r1, r1, #4
00390f44: add r8, pc, r8
00390f48: add r2, r1, r1, lsl #3
00390f4c: mov r4, r0
00390f50: add r2, r2, r2, lsl #6
00390f54: add r2, r1, r2, lsl #3
00390f58: add r2, r2, r2, lsl #15
00390f5c: add r1, r1, r2, lsl #3
00390f60: rsb r1, r1, #0
00390f64: cmp r1, #1
00390f68: bls #0x390f98
00390f6c: cmp r1, #0
00390f70: movne r2, r3
00390f74: beq #0x391160
00390f78: ldr r2, [r2, #4]
00390f7c: cmp r2, #4
00390f80: beq #0x390fcc
00390f84: cmp r1, #0
00390f88: beq #0x3911ac
00390f8c: ldr r3, [r3, #4]
00390f90: cmp r3, #7
00390f94: beq #0x390fa0
00390f98: add sp, sp, #0x34
00390f9c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00390fa0: ldr r7, [r4, #4]
00390fa4: ldr r3, [r7]
00390fa8: ldr r2, [r7, #4]
00390fac: rsb r2, r3, r2
00390fb0: asr r2, r2, #4
00390fb4: add r1, r2, r2, lsl #3
00390fb8: add r1, r1, r1, lsl #6
00390fbc: add r1, r2, r1, lsl #3
00390fc0: add r1, r1, r1, lsl #15
00390fc4: add r1, r2, r1, lsl #3
00390fc8: rsb r1, r1, #0
00390fcc: cmp r1, #1
00390fd0: bls #0x3911cc
00390fd4: ldr r3, [r3, #0x74]
00390fd8: cmp r3, #3
00390fdc: bne #0x390f98
00390fe0: mov r0, r4
00390fe4: mov r1, #2
00390fe8: bl #0x37baf8
00390fec: ldr r3, [r0, #4]
00390ff0: cmp r3, #3
00390ff4: bne #0x390f98
00390ff8: mov r0, r4
00390ffc: mov r1, #0
00391000: bl #0x37baf8
00391004: ldr r3, [r0, #4]
00391008: cmp r3, #4
0039100c: beq #0x3912b0
00391010: mov r1, #0
00391014: mov r0, r4
00391018: bl #0x37baf8
0039101c: bl #0x31b5a0
00391020: mov r7, r0
00391024: mov r1, #1
00391028: mov r0, r4
0039102c: bl #0x37baf8
00391030: bl #0x31bbf0
00391034: cmp r7, #0
00391038: mov r8, r0
0039103c: beq #0x3911e0
00391040: ldr sl, [r7, #0x2dc]
00391044: cmp sl, #0
00391048: beq #0x3911e0
0039104c: ldrb r3, [sl, #0x10]
00391050: cmp r3, #0
00391054: beq #0x3911e0
00391058: ldr r0, [r5, #0x2dc]
0039105c: cmp r0, #0
00391060: beq #0x3911e0
00391064: bl #0x46e750
00391068: mov r3, r0
0039106c: mov r0, sl
00391070: str r3, [sp, #0x10]
00391074: bl #0x46e750
00391078: ldr r1, [r7, #0x160]
0039107c: mov r2, r0
00391080: ldr r0, [r5, #0x160]
00391084: str r2, [sp, #0xc]
00391088: bl #0x30e3ac
0039108c: ldr r1, [r7, #0x164]
00391090: mov sb, r0
00391094: ldr r0, [r5, #0x164]
00391098: bl #0x30e3ac
0039109c: ldr r1, [r7, #0x168]
003910a0: mov fp, r0
003910a4: ldr r0, [r5, #0x168]
003910a8: bl #0x30e3ac
003910ac: mov r1, sb
003910b0: mov sl, r0
003910b4: mov r0, sb
003910b8: bl #0x30ed6c
003910bc: mov r1, fp
003910c0: mov sb, r0
003910c4: mov r0, fp
003910c8: bl #0x30ed6c
003910cc: mov r1, r0
003910d0: mov r0, sb
003910d4: bl #0x30eba4
003910d8: mov r1, sl
003910dc: mov sb, r0
003910e0: mov r0, sl
003910e4: bl #0x30ed6c
003910e8: mov r1, r0
003910ec: mov r0, sb
003910f0: bl #0x30eba4
003910f4: bl #0x30e124
003910f8: ldr r3, [sp, #0x10]
003910fc: mov r1, r3
00391100: bl #0x30e3ac
00391104: ldr r2, [sp, #0xc]
00391108: mov r1, r2
0039110c: bl #0x30e3ac
00391110: mov r1, r0
00391114: mov r0, r8
00391118: bl #0x30e4b4
0039111c: cmp r0, #0
00391120: beq #0x391214
00391124: ldr r3, [r4, #4]
00391128: ldm r3, {r2, r3}
0039112c: rsb r3, r2, r3
00391130: asr r3, r3, #4
00391134: add r2, r3, r3, lsl #3
00391138: add r2, r2, r2, lsl #6
0039113c: add r2, r3, r2, lsl #3
00391140: add r2, r2, r2, lsl #15
00391144: add r3, r3, r2, lsl #3
00391148: cmn r3, #3
0039114c: beq #0x391300
00391150: mov r0, r6
00391154: mov r1, #1
00391158: bl #0x37c7e4
0039115c: b #0x390f98
00391160: ldr r0, [pc, #0x2c0]
00391164: add r0, pc, r0
00391168: bl #0x708eb0
0039116c: ldr r2, [r7]
00391170: ldr r7, [r4, #4]
00391174: ldr r2, [r2, #4]
00391178: ldr r3, [r7]
0039117c: ldr r0, [r7, #4]
00391180: cmp r2, #4
00391184: rsb r0, r3, r0
00391188: asr r0, r0, #4
0039118c: add r1, r0, r0, lsl #3
00391190: add r1, r1, r1, lsl #6
00391194: add r1, r0, r1, lsl #3
00391198: add r1, r1, r1, lsl #15
0039119c: add r1, r0, r1, lsl #3
003911a0: rsb r1, r1, #0
003911a4: bne #0x390f84
003911a8: b #0x390fcc
003911ac: ldr r0, [pc, #0x278]
003911b0: add r0, pc, r0
003911b4: bl #0x708eb0
003911b8: ldr r3, [r7]
003911bc: ldr r3, [r3, #4]
003911c0: cmp r3, #7
003911c4: bne #0x390f98
003911c8: b #0x390fa0
003911cc: ldr r0, [pc, #0x25c]
003911d0: add r0, pc, r0
003911d4: bl #0x708eb0
003911d8: ldr r3, [r7]
003911dc: b #0x390fd4
003911e0: mov r1, r8
003911e4: ldr r0, [r5, #0x12c]
003911e8: bl #0x30e3ac
003911ec: ldr r1, [r7, #0x138]
003911f0: bl #0x30e9ac
003911f4: ldr r3, [r5, #0x140]
003911f8: cmp r0, #0
003911fc: ldr fp, [r5, #0x130]
00391200: ldr sl, [r5, #0x134]
00391204: ldr r1, [r5, #0x138]
00391208: ldr sb, [r5, #0x13c]
0039120c: str r3, [sp, #0x14]
00391210: bne #0x391224
00391214: mov r0, r6
00391218: mov r1, #0
0039121c: bl #0x37c7e4
00391220: b #0x390f98
00391224: mov r0, r8
00391228: bl #0x30eba4
0039122c: ldr r1, [r7, #0x12c]
00391230: bl #0x30e4b4
00391234: cmp r0, #0
00391238: beq #0x391214
0039123c: mov r1, r8
00391240: mov r0, fp
00391244: bl #0x30e3ac
00391248: ldr r1, [r7, #0x13c]
0039124c: bl #0x30e9ac
00391250: cmp r0, #0
00391254: beq #0x391214
00391258: mov r1, sb
0039125c: mov r0, r8
00391260: bl #0x30eba4
00391264: ldr r1, [r7, #0x130]
00391268: bl #0x30e4b4
0039126c: cmp r0, #0
00391270: beq #0x391214
00391274: mov r1, r8
00391278: mov r0, sl
0039127c: bl #0x30e3ac
00391280: ldr r1, [r7, #0x140]
00391284: bl #0x30e9ac
00391288: cmp r0, #0
0039128c: beq #0x391214
00391290: ldr r1, [sp, #0x14]
00391294: mov r0, r8
00391298: bl #0x30eba4
0039129c: ldr r1, [r7, #0x134]
003912a0: bl #0x30e4b4
003912a4: cmp r0, #0
003912a8: beq #0x391214
003912ac: b #0x391124
003912b0: ldr r3, [pc, #0x17c]
003912b4: mov r1, #0
003912b8: mov r0, r4
003912bc: ldr r3, [r8, r3]
003912c0: add r7, sp, #0x24
003912c4: ldr r8, [r3, #0x38]
003912c8: bl #0x37baf8
003912cc: bl #0x31c49c
003912d0: mov ip, #0
003912d4: mov r2, r0
003912d8: mov r1, r8
003912dc: mov r0, r7
003912e0: mvn r3, #0
003912e4: str ip, [sp, #4]
003912e8: str ip, [sp]
003912ec: bl #0x34aca0
003912f0: mov r0, r7
003912f4: bl #0x33fee4
003912f8: mov r7, r0
003912fc: b #0x391024
00391300: mov r1, #2
00391304: mov r0, r4
00391308: bl #0x37baf8
0039130c: bl #0x31bbf0
00391310: mov r3, #0
00391314: mov sb, r0
00391318: add r1, sp, #0x18
0039131c: mov r0, r5
00391320: str r3, [sp, #0x20]
00391324: str r3, [sp, #0x18]
00391328: str r3, [sp, #0x1c]
0039132c: bl #0x393ae4
00391330: ldr r1, [r5, #0x160]
00391334: ldr r0, [r7, #0x160]
00391338: bl #0x30e3ac
0039133c: ldr r1, [r5, #0x164]
00391340: mov sl, r0
00391344: ldr r0, [r7, #0x164]
00391348: bl #0x30e3ac
0039134c: ldr r1, [r5, #0x168]
00391350: mov r8, r0
00391354: ldr r0, [r7, #0x168]
00391358: bl #0x30e3ac
0039135c: ldr r1, [sp, #0x18]
00391360: mov r4, r0
00391364: mov r0, sl
00391368: bl #0x30ed6c
0039136c: ldr r1, [sp, #0x1c]
00391370: mov r5, r0
00391374: mov r0, r8
00391378: bl #0x30ed6c
0039137c: mov r1, r0
00391380: mov r0, r5
00391384: bl #0x30eba4
00391388: ldr r1, [sp, #0x20]
0039138c: mov r5, r0
00391390: mov r0, r4
00391394: bl #0x30ed6c
00391398: mov r1, r0
0039139c: mov r0, r5
003913a0: bl #0x30eba4
003913a4: mov r1, sl
003913a8: mov r5, r0
003913ac: mov r0, sl
003913b0: bl #0x30ed6c
003913b4: mov r1, r8
003913b8: mov r7, r0
003913bc: mov r0, r8
003913c0: bl #0x30ed6c
003913c4: mov r1, r0
003913c8: mov r0, r7
003913cc: bl #0x30eba4
003913d0: mov r1, r4
003913d4: mov r7, r0
003913d8: mov r0, r4
003913dc: bl #0x30ed6c
003913e0: mov r1, r0
003913e4: mov r0, r7
003913e8: bl #0x30eba4
003913ec: bl #0x30e124
003913f0: mov r1, r0
003913f4: mov r0, r5
003913f8: bl #0x30ec94
003913fc: mov r1, r0
00391400: mov r0, sb
00391404: bl #0x30e9ac
00391408: cmp r0, #0
0039140c: mov r1, #0
00391410: movne r1, #1
00391414: mov r0, r6
00391418: and r1, r1, #1
0039141c: bl #0x37c7e4
00391420: b #0x390f98
00391424: rsbeq r3, r0, ip, asr #22
00391428: subseq sp, r2, r4, lsl #6
0039142c: ldrheq sp, [r2], #-0x28

_ZN10GameObject19_GetDistanceBetweenERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x391438 568
00391438: push {r4, r5, r6, r7, r8, sl, lr}
0039143c: ldr r7, [r0, #4]
00391440: mov r6, r1
00391444: ldr r4, [pc, #0x210]
00391448: ldm r7, {r1, r3}
0039144c: add r4, pc, r4
00391450: sub sp, sp, #0x24
00391454: rsb r3, r1, r3
00391458: asr r3, r3, #4
0039145c: mov r5, r0
00391460: add r2, r3, r3, lsl #3
00391464: add r2, r2, r2, lsl #6
00391468: add r2, r3, r2, lsl #3
0039146c: add r2, r2, r2, lsl #15
00391470: add r3, r3, r2, lsl #3
00391474: rsb r3, r3, #0
00391478: cmp r3, #1
0039147c: bls #0x391494
00391480: cmp r3, #0
00391484: beq #0x39149c
00391488: ldr r3, [r1, #4]
0039148c: cmp r3, #4
00391490: beq #0x3914b0
00391494: add sp, sp, #0x24
00391498: pop {r4, r5, r6, r7, r8, sl, pc}
0039149c: ldr r0, [pc, #0x1bc]
003914a0: add r0, pc, r0
003914a4: bl #0x708eb0
003914a8: ldr r1, [r7]
003914ac: b #0x391488
003914b0: mov r0, r5
003914b4: mov r1, #1
003914b8: bl #0x37baf8
003914bc: ldr r3, [r0, #4]
003914c0: cmp r3, #4
003914c4: bne #0x391494
003914c8: ldr r7, [r5, #4]
003914cc: ldr r8, [pc, #0x190]
003914d0: ldm r7, {r0, r3}
003914d4: ldr r2, [r4, r8]
003914d8: rsb r3, r0, r3
003914dc: asr r3, r3, #4
003914e0: ldr sl, [r2, #0x38]
003914e4: add r2, r3, r3, lsl #3
003914e8: add r2, r2, r2, lsl #6
003914ec: add r2, r3, r2, lsl #3
003914f0: add r2, r2, r2, lsl #15
003914f4: add r3, r3, r2, lsl #3
003914f8: cmp r3, #0
003914fc: bne #0x391510
00391500: ldr r0, [pc, #0x160]
00391504: add r0, pc, r0
00391508: bl #0x708eb0
0039150c: ldr r0, [r7]
00391510: bl #0x31c49c
00391514: add r7, sp, #0x14
00391518: mov r2, r0
0039151c: mov ip, #0
00391520: mvn r3, #0
00391524: mov r0, r7
00391528: mov r1, sl
0039152c: str ip, [sp, #4]
00391530: str ip, [sp]
00391534: bl #0x34aca0
00391538: mov r0, r7
0039153c: bl #0x33fee4
00391540: ldr r5, [r5, #4]
00391544: mov r7, r0
00391548: ldr r2, [r4, r8]
0039154c: ldm r5, {r0, r3}
00391550: ldr r8, [r2, #0x38]
00391554: rsb r3, r0, r3
00391558: asr r3, r3, #4
0039155c: add r2, r3, r3, lsl #3
00391560: add r2, r2, r2, lsl #6
00391564: add r2, r3, r2, lsl #3
00391568: add r2, r2, r2, lsl #15
0039156c: add r3, r3, r2, lsl #3
00391570: rsb r3, r3, #0
00391574: cmp r3, #1
00391578: bhi #0x39158c
0039157c: ldr r0, [pc, #0xe8]
00391580: add r0, pc, r0
00391584: bl #0x708eb0
00391588: ldr r0, [r5]
0039158c: add r0, r0, #0x70
00391590: bl #0x31c49c
00391594: add r4, sp, #8
00391598: mov r1, r8
0039159c: mov ip, #0
003915a0: mov r2, r0
003915a4: mvn r3, #0
003915a8: mov r0, r4
003915ac: str ip, [sp, #4]
003915b0: str ip, [sp]
003915b4: bl #0x34aca0
003915b8: mov r0, r4
003915bc: bl #0x33fee4
003915c0: cmp r0, #0
003915c4: cmpne r7, #0
003915c8: moveq r1, #0xbf000000
003915cc: mov r4, r0
003915d0: addeq r1, r1, #0x800000
003915d4: beq #0x391650
003915d8: ldr r1, [r0, #0x160]
003915dc: ldr r0, [r7, #0x160]
003915e0: bl #0x30e3ac
003915e4: ldr r1, [r4, #0x164]
003915e8: mov sl, r0
003915ec: ldr r0, [r7, #0x164]
003915f0: bl #0x30e3ac
003915f4: ldr r1, [r4, #0x168]
003915f8: mov r8, r0
003915fc: ldr r0, [r7, #0x168]
00391600: bl #0x30e3ac
00391604: mov r1, sl
00391608: mov r5, r0
0039160c: mov r0, sl
00391610: bl #0x30ed6c
00391614: mov r1, r8
00391618: mov r4, r0
0039161c: mov r0, r8
00391620: bl #0x30ed6c
00391624: mov r1, r0
00391628: mov r0, r4
0039162c: bl #0x30eba4
00391630: mov r1, r5
00391634: mov r4, r0
00391638: mov r0, r5
0039163c: bl #0x30ed6c
00391640: mov r1, r0
00391644: mov r0, r4
00391648: bl #0x30eba4
0039164c: mov r1, r0
00391650: mov r0, r6
00391654: bl #0x37ccbc
00391658: b #0x391494
0039165c: rsbeq r3, r0, r4, asr #12
00391660: subseq ip, r2, r8, asr #31
00391664: strdeq r3, r4, [r0], -r4
00391668: subseq ip, r2, r4, ror #30
0039166c: subseq ip, r2, r8, ror #29

_ZN10LuaManagerC2Ev 0x379e68 72
00379e68: ldr r1, [pc, #0x38]
00379e6c: str r4, [sp, #-4]!
00379e70: ldr r4, [pc, #0x34]
00379e74: add r1, pc, r1
00379e78: mov ip, #0
00379e7c: ldr r4, [r1, r4]
00379e80: mov r2, r0
00379e84: str ip, [r0, #8]
00379e88: add r4, r4, #8
00379e8c: str r4, [r0]
00379e90: strb ip, [r2, #4]!
00379e94: str r2, [r0, #0x10]
00379e98: str ip, [r0, #0x14]
00379e9c: str r2, [r0, #0xc]
00379ea0: ldm sp!, {r4}
00379ea4: bx lr
00379ea8: rsbeq sl, r1, ip, lsl ip
00379eac: andeq r3, r0, r8, ror #26

_ZN3sfc6script3lua5ValueC1Ef 0x37cc3c 128
0037cc3c: ldr r2, [pc, #0x70]
0037cc40: ldr ip, [pc, #0x70]
0037cc44: mov r3, r0
0037cc48: add r2, pc, r2
0037cc4c: ldr ip, [r2, ip]
0037cc50: push {r4, r5, r6, lr}
0037cc54: add ip, ip, #8
0037cc58: mov r4, r0
0037cc5c: str ip, [r3], #0xc
0037cc60: mov r6, r1
0037cc64: mov r0, r3
0037cc68: str r3, [r4, #0x1c]
0037cc6c: str r3, [r4, #0x20]
0037cc70: mov r1, #0x10
0037cc74: bl #0x31167c
0037cc78: ldr r2, [r4, #0x1c]
0037cc7c: add r3, r4, #0x24
0037cc80: mov r5, #0
0037cc84: strb r5, [r2]
0037cc88: mov r0, r3
0037cc8c: str r3, [r4, #0x64]
0037cc90: str r3, [r4, #0x68]
0037cc94: bl #0x37be44
0037cc98: ldr r3, [r4, #0x64]
0037cc9c: mov r0, r4
0037cca0: mov r1, r6
0037cca4: str r5, [r3]
0037cca8: bl #0x31b5e8
0037ccac: mov r0, r4
0037ccb0: pop {r4, r5, r6, pc}
0037ccb4: rsbeq r7, r1, r8, asr #28
0037ccb8: muleq r0, r8, r7

_ZN9LuaScript13_AddToVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ec70 528
0037ec70: push {r4, r5, r6, r7, lr}
0037ec74: ldr r5, [r0, #4]
0037ec78: mov r6, r2
0037ec7c: sub sp, sp, #0xc
0037ec80: ldm r5, {r1, r3}
0037ec84: mov r4, r0
0037ec88: rsb r3, r1, r3
0037ec8c: asr r3, r3, #4
0037ec90: add r2, r3, r3, lsl #3
0037ec94: add r2, r2, r2, lsl #6
0037ec98: add r2, r3, r2, lsl #3
0037ec9c: add r2, r2, r2, lsl #15
0037eca0: add r3, r3, r2, lsl #3
0037eca4: rsb r3, r3, #0
0037eca8: cmp r3, #1
0037ecac: bls #0x37ecc4
0037ecb0: cmp r3, #0
0037ecb4: beq #0x37eccc
0037ecb8: ldr r3, [r1, #4]
0037ecbc: cmp r3, #4
0037ecc0: beq #0x37ece8
0037ecc4: add sp, sp, #0xc
0037ecc8: pop {r4, r5, r6, r7, pc}
0037eccc: ldr r0, [pc, #0x1a0]
0037ecd0: add r0, pc, r0
0037ecd4: bl #0x708eb0
0037ecd8: ldr r1, [r5]
0037ecdc: ldr r3, [r1, #4]
0037ece0: cmp r3, #4
0037ece4: bne #0x37ecc4
0037ece8: mov r0, r4
0037ecec: mov r1, #1
0037ecf0: bl #0x37baf8
0037ecf4: ldr r3, [r0, #4]
0037ecf8: cmp r3, #4
0037ecfc: bne #0x37ecc4
0037ed00: ldr r5, [r4, #4]
0037ed04: ldm r5, {r0, r3}
0037ed08: rsb r3, r0, r3
0037ed0c: asr r3, r3, #4
0037ed10: add r2, r3, r3, lsl #3
0037ed14: add r2, r2, r2, lsl #6
0037ed18: add r2, r3, r2, lsl #3
0037ed1c: add r2, r2, r2, lsl #15
0037ed20: add r3, r3, r2, lsl #3
0037ed24: cmp r3, #0
0037ed28: bne #0x37ed3c
0037ed2c: ldr r0, [pc, #0x144]
0037ed30: add r0, pc, r0
0037ed34: bl #0x708eb0
0037ed38: ldr r0, [r5]
0037ed3c: bl #0x31c49c
0037ed40: bl #0x37c164
0037ed44: ldrb r3, [r6, #0x64]
0037ed48: str r0, [sp, #4]
0037ed4c: cmp r3, #0
0037ed50: beq #0x37edac
0037ed54: ldr r3, [r6, #0x50]
0037ed58: add ip, r6, #0x4c
0037ed5c: cmp r3, #0
0037ed60: beq #0x37ee28
0037ed64: mov r1, ip
0037ed68: b #0x37ed70
0037ed6c: mov r3, r2
0037ed70: ldr r2, [r3, #0x10]
0037ed74: cmp r0, r2
0037ed78: ldrhi r2, [r3, #0xc]
0037ed7c: ldrls r2, [r3, #8]
0037ed80: movhi r3, r1
0037ed84: mov r1, r3
0037ed88: cmp r2, #0
0037ed8c: bne #0x37ed6c
0037ed90: cmp ip, r3
0037ed94: beq #0x37ee30
0037ed98: ldr r2, [r3, #0x10]
0037ed9c: cmp r0, r2
0037eda0: blo #0x37ee28
0037eda4: cmp ip, r3
0037eda8: beq #0x37ee30
0037edac: add r6, r6, #0x34
0037edb0: add r5, sp, #4
0037edb4: mov r1, r5
0037edb8: mov r0, r6
0037edbc: bl #0x37dac4
0037edc0: ldr r4, [r4, #4]
0037edc4: mov r5, r0
0037edc8: ldm r4, {r0, r3}
0037edcc: rsb r3, r0, r3
0037edd0: asr r3, r3, #4
0037edd4: add r2, r3, r3, lsl #3
0037edd8: add r2, r2, r2, lsl #6
0037eddc: add r2, r3, r2, lsl #3
0037ede0: add r2, r2, r2, lsl #15
0037ede4: add r3, r3, r2, lsl #3
0037ede8: rsb r3, r3, #0
0037edec: cmp r3, #1
0037edf0: bhi #0x37ee04
0037edf4: ldr r0, [pc, #0x80]
0037edf8: add r0, pc, r0
0037edfc: bl #0x708eb0
0037ee00: ldr r0, [r4]
0037ee04: add r0, r0, #0x70
0037ee08: bl #0x31c49c
0037ee0c: mov r4, r0
0037ee10: bl #0x30de54
0037ee14: mov r1, r4
0037ee18: add r2, r4, r0
0037ee1c: mov r0, r5
0037ee20: bl #0x3109e0
0037ee24: b #0x37ecc4
0037ee28: mov r3, ip
0037ee2c: b #0x37eda4
0037ee30: add r5, sp, #4
0037ee34: mov r0, ip
0037ee38: mov r1, r5
0037ee3c: bl #0x37dac4
0037ee40: add r6, r6, #0x34
0037ee44: mov r7, r0
0037ee48: mov r1, r5
0037ee4c: mov r0, r6
0037ee50: bl #0x37dac4
0037ee54: cmp r7, r0
0037ee58: mov r3, r0
0037ee5c: beq #0x37edb4
0037ee60: mov r0, r7
0037ee64: ldr r2, [r3, #0x10]
0037ee68: ldr r1, [r3, #0x14]
0037ee6c: bl #0x3109e0
0037ee70: b #0x37edb4

_ZN9LuaScriptD2Ev 0x37bec0 324
0037bec0: push {r4, r5, r6, lr}
0037bec4: ldr r5, [pc, #0x12c]
0037bec8: ldr r3, [pc, #0x12c]
0037becc: ldr r2, [r0, #0x90]
0037bed0: add r5, pc, r5
0037bed4: ldr r3, [r5, r3]
0037bed8: cmp r2, #0
0037bedc: mov r4, r0
0037bee0: add r3, r3, #8
0037bee4: str r3, [r0]
0037bee8: bne #0x37bfc8
0037beec: add r3, r4, #0x68
0037bef0: ldr r0, [r3, #0x14]
0037bef4: cmp r0, r3
0037bef8: beq #0x37bf18
0037befc: cmp r0, #0
0037bf00: beq #0x37bf18
0037bf04: ldr r1, [r4, #0x68]
0037bf08: rsb r1, r0, r1
0037bf0c: cmp r1, #0x80
0037bf10: bhi #0x37bff0
0037bf14: bl #0x708f00
0037bf18: ldr r3, [r4, #0x5c]
0037bf1c: cmp r3, #0
0037bf20: beq #0x37bf48
0037bf24: add r6, r4, #0x4c
0037bf28: mov r0, r6
0037bf2c: ldr r1, [r4, #0x50]
0037bf30: bl #0x37bd7c
0037bf34: mov r3, #0
0037bf38: str r6, [r4, #0x58]
0037bf3c: str r3, [r4, #0x5c]
0037bf40: str r6, [r4, #0x54]
0037bf44: str r3, [r4, #0x50]
0037bf48: ldr r3, [r4, #0x44]
0037bf4c: cmp r3, #0
0037bf50: beq #0x37bf78
0037bf54: add r6, r4, #0x34
0037bf58: mov r0, r6
0037bf5c: ldr r1, [r4, #0x38]
0037bf60: bl #0x37bd7c
0037bf64: mov r3, #0
0037bf68: str r6, [r4, #0x40]
0037bf6c: str r3, [r4, #0x44]
0037bf70: str r6, [r4, #0x3c]
0037bf74: str r3, [r4, #0x38]
0037bf78: ldr r3, [r4, #0x2c]
0037bf7c: cmp r3, #0
0037bf80: beq #0x37bfa8
0037bf84: add r6, r4, #0x1c
0037bf88: mov r0, r6
0037bf8c: ldr r1, [r4, #0x20]
0037bf90: bl #0x37bcc0
0037bf94: mov r3, #0
0037bf98: str r6, [r4, #0x28]
0037bf9c: str r3, [r4, #0x2c]
0037bfa0: str r6, [r4, #0x24]
0037bfa4: str r3, [r4, #0x20]
0037bfa8: ldr r3, [pc, #0x50]
0037bfac: add r0, r4, #4
0037bfb0: ldr r3, [r5, r3]
0037bfb4: add r3, r3, #8
0037bfb8: str r3, [r4, #0x10]
0037bfbc: bl #0x31b180
0037bfc0: mov r0, r4
0037bfc4: pop {r4, r5, r6, pc}
0037bfc8: add r6, r0, #0x80
0037bfcc: mov r0, r6
0037bfd0: ldr r1, [r4, #0x84]
0037bfd4: bl #0x37bcf8
0037bfd8: mov r3, #0
0037bfdc: str r6, [r4, #0x8c]
0037bfe0: str r3, [r4, #0x90]
0037bfe4: str r6, [r4, #0x88]
0037bfe8: str r3, [r4, #0x84]
0037bfec: b #0x37beec
0037bff0: bl #0x310440
0037bff4: b #0x37bf18
0037bff8: rsbeq r8, r1, r0, asr #23
0037bffc: andeq r1, r0, r4, ror r6
0037c000: andeq r3, r0, r8, asr r6

_ZN9Character14_DBG_DumpPropsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7dbc 132
003b7dbc: str lr, [sp, #-4]!
003b7dc0: ldr r3, [r0, #4]
003b7dc4: sub sp, sp, #0xc
003b7dc8: ldm r3, {r0, r1}
003b7dcc: rsb r3, r0, r1
003b7dd0: asr r3, r3, #4
003b7dd4: add r1, r3, r3, lsl #3
003b7dd8: add r1, r1, r1, lsl #6
003b7ddc: add r1, r3, r1, lsl #3
003b7de0: add r1, r1, r1, lsl #15
003b7de4: add r3, r3, r1, lsl #3
003b7de8: cmp r3, #0
003b7dec: bne #0x3b7e0c
003b7df0: ldr r1, [pc, #0x44]
003b7df4: add r0, r2, #0x560
003b7df8: mov r2, #1
003b7dfc: add r1, pc, r1
003b7e00: add sp, sp, #0xc
003b7e04: pop {lr}
003b7e08: b #0x3de840
003b7e0c: ldr r3, [r0, #4]
003b7e10: cmp r3, #4
003b7e14: bne #0x3b7df0
003b7e18: str r2, [sp, #4]
003b7e1c: bl #0x31c49c
003b7e20: ldr r2, [sp, #4]
003b7e24: mov r1, r0
003b7e28: add r0, r2, #0x560
003b7e2c: mov r2, #1
003b7e30: add sp, sp, #0xc
003b7e34: pop {lr}
003b7e38: b #0x3de840
003b7e3c: subseq ip, r0, r4, asr #14

_ZN3sfc6script3lua6Binder11_bindMethodEPKcPvS5_ 0x319c44 100
00319c44: push {r4, r5, r6, lr}
00319c48: mov r4, r0
00319c4c: mov r5, r2
00319c50: ldr r0, [r0, #8]
00319c54: mov r6, r3
00319c58: bl #0x84c04c
00319c5c: mov r1, r5
00319c60: ldr r0, [r4, #8]
00319c64: bl #0x84b4cc
00319c68: ldr r5, [pc, #0x30]
00319c6c: mov r1, r6
00319c70: ldr r0, [r4, #8]
00319c74: bl #0x84b4cc
00319c78: ldr r3, [pc, #0x24]
00319c7c: add r5, pc, r5
00319c80: ldr r0, [r4, #8]
00319c84: ldr r1, [r5, r3]
00319c88: mov r2, #2
00319c8c: bl #0x84bcdc
00319c90: ldr r0, [r4, #8]
00319c94: mvn r1, #2
00319c98: pop {r4, r5, r6, lr}
00319c9c: b #0x84c0e8
00319ca0: rsbeq sl, r7, r4, lsl lr
00319ca4: strheq r3, [r0], -ip

_ZN3sfc6script3lua5ErrorD1Ev 0x31a68c 52
0031a68c: ldr r3, [pc, #0x24]
0031a690: ldr r2, [pc, #0x24]
0031a694: push {r4, lr}
0031a698: add r3, pc, r3
0031a69c: ldr r2, [r3, r2]
0031a6a0: mov r4, r0
0031a6a4: add r2, r2, #8
0031a6a8: str r2, [r0], #8
0031a6ac: bl #0x3139ac
0031a6b0: mov r0, r4
0031a6b4: pop {r4, pc}

_ZN9Character12_IsConnectedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6ec4 76
003b6ec4: ldr r3, [pc, #0x3c]
003b6ec8: push {r4, lr}
003b6ecc: mov r4, r1
003b6ed0: ldr r1, [pc, #0x34]
003b6ed4: add r3, pc, r3
003b6ed8: ldr r0, [r3, r1]
003b6edc: mov r1, r2
003b6ee0: mov r2, #0
003b6ee4: ldr r0, [r0, #0x40]
003b6ee8: bl #0x36eea8
003b6eec: ldr r3, [r0]
003b6ef0: mov lr, pc
003b6ef4: ldr pc, [r3, #0x5c]
003b6ef8: mov r1, r0
003b6efc: mov r0, r4
003b6f00: pop {r4, lr}
003b6f04: b #0x37c7e4
003b6f08: ldrheq sp, [sp], #-0xbc
003b6f0c: strdeq r3, r4, [r0], -r4

_ZN9Character8_DoSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8bd8 224
003b8bd8: push {r4, r5, lr}
003b8bdc: ldr r3, [r0, #4]
003b8be0: sub sp, sp, #0xc
003b8be4: mov r4, r0
003b8be8: ldr r1, [r3, #4]
003b8bec: ldr ip, [r3]
003b8bf0: rsb r3, ip, r1
003b8bf4: asr r3, r3, #4
003b8bf8: add r1, r3, r3, lsl #3
003b8bfc: add r1, r1, r1, lsl #6
003b8c00: add r1, r3, r1, lsl #3
003b8c04: add r1, r1, r1, lsl #15
003b8c08: add r3, r3, r1, lsl #3
003b8c0c: cmp r3, #0
003b8c10: bne #0x3b8c1c
003b8c14: add sp, sp, #0xc
003b8c18: pop {r4, r5, pc}
003b8c1c: ldr r3, [ip, #4]
003b8c20: cmp r3, #3
003b8c24: bne #0x3b8c14
003b8c28: mov r1, #0
003b8c2c: str r2, [sp, #4]
003b8c30: bl #0x37baf8
003b8c34: bl #0x38d798
003b8c38: ldr r2, [sp, #4]
003b8c3c: mov r5, r0
003b8c40: mov r0, r2
003b8c44: bl #0x3bc5fc
003b8c48: ldr r3, [r0, #4]
003b8c4c: ldr r2, [sp, #4]
003b8c50: cmp r5, r3
003b8c54: bhs #0x3b8c14
003b8c58: ldr r5, [r4, #4]
003b8c5c: add r4, r2, #0x3c8
003b8c60: ldm r5, {r0, r3}
003b8c64: rsb r3, r0, r3
003b8c68: asr r3, r3, #4
003b8c6c: add r2, r3, r3, lsl #3
003b8c70: add r2, r2, r2, lsl #6
003b8c74: add r2, r3, r2, lsl #3
003b8c78: add r2, r2, r2, lsl #15
003b8c7c: add r3, r3, r2, lsl #3
003b8c80: cmp r3, #0
003b8c84: bne #0x3b8c98
003b8c88: ldr r0, [pc, #0x24]
003b8c8c: add r0, pc, r0
003b8c90: bl #0x708eb0
003b8c94: ldr r0, [r5]
003b8c98: bl #0x31bbf0
003b8c9c: bl #0x8be2a0
003b8ca0: mov r1, r0
003b8ca4: mov r0, r4
003b8ca8: add sp, sp, #0xc
003b8cac: pop {r4, r5, lr}
003b8cb0: b #0x3d8868
003b8cb4: ldrsbeq r5, [r0], #-0x7c

_ZN4Door14createBindingsERN3sfc6script3lua6BinderE 0x3e74c8 152
003e74c8: push {r4, r5, r6, r7, r8, lr}
003e74cc: ldr r4, [pc, #0x78]
003e74d0: mov r5, r1
003e74d4: mov r8, r0
003e74d8: bl #0x38d7ec
003e74dc: ldr r3, [pc, #0x6c]
003e74e0: add r4, pc, r4
003e74e4: ldr r6, [pc, #0x68]
003e74e8: ldr r7, [r4, r3]
003e74ec: mov r0, r5
003e74f0: add r6, pc, r6
003e74f4: mov r3, r8
003e74f8: mov r1, r6
003e74fc: mov r2, r7
003e7500: bl #0x31a4d4
003e7504: mov r0, r5
003e7508: mov r1, r6
003e750c: mov r2, r7
003e7510: bl #0x319af4
003e7514: ldr r2, [pc, #0x3c]
003e7518: ldr r6, [pc, #0x3c]
003e751c: mov r0, r5
003e7520: ldr r4, [r4, r2]
003e7524: add r6, pc, r6
003e7528: mov r1, r6
003e752c: mov r2, r4
003e7530: mov r3, r8
003e7534: bl #0x31a4d4
003e7538: mov r0, r5
003e753c: mov r1, r6
003e7540: mov r2, r4
003e7544: pop {r4, r5, r6, r7, r8, lr}
003e7548: b #0x319af4
003e754c: ldrheq sp, [sl], #-0x50
003e7550: andeq r3, r0, r4, asr #23
003e7554: subeq sl, sp, r8, asr r1
003e7558: andeq r3, r0, r8, ror #31
003e755c: umaaleq lr, sp, ip, fp

_ZN10GameObject9_IsFlyingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ea94 32
0038ea94: push {r4, lr}
0038ea98: add r0, r2, #0x1c8
0038ea9c: mov r4, r1
0038eaa0: bl #0x5241e8
0038eaa4: mov r1, r0
0038eaa8: mov r0, r4
0038eaac: pop {r4, lr}
0038eab0: b #0x37c7e4

_ZNK9LuaScript11IsInVFTableEPKc 0x37c2a0 116
0037c2a0: push {r4, lr}
0037c2a4: mov r4, r0
0037c2a8: mov r0, r1
0037c2ac: bl #0x37c164
0037c2b0: ldr r3, [r4, #0x38]
0037c2b4: add r4, r4, #0x34
0037c2b8: cmp r3, #0
0037c2bc: beq #0x37c300
0037c2c0: mov r1, r4
0037c2c4: b #0x37c2cc
0037c2c8: mov r3, r2
0037c2cc: ldr r2, [r3, #0x10]
0037c2d0: cmp r0, r2
0037c2d4: ldrhi r2, [r3, #0xc]
0037c2d8: ldrls r2, [r3, #8]
0037c2dc: movhi r3, r1
0037c2e0: mov r1, r3
0037c2e4: cmp r2, #0
0037c2e8: bne #0x37c2c8
0037c2ec: cmp r4, r3
0037c2f0: beq #0x37c300
0037c2f4: ldr r2, [r3, #0x10]
0037c2f8: cmp r0, r2
0037c2fc: bhs #0x37c308
0037c300: mov r0, #0
0037c304: pop {r4, pc}
0037c308: subs r0, r3, r4
0037c30c: movne r0, #1
0037c310: pop {r4, pc}

_ZN3sfc6script3lua9ArgumentsC2EP9lua_Statei 0x3198e0 500
003198e0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003198e4: ldr r3, [pc, #0x1d4]
003198e8: sub sp, sp, #0xfc
003198ec: ldr sb, [pc, #0x1d0]
003198f0: str r3, [sp, #0xc]
003198f4: ldr lr, [sp, #0xc]
003198f8: ldr r3, [pc, #0x1c8]
003198fc: add sb, pc, sb
00319900: ldr ip, [sb, lr]
00319904: ldr r3, [sb, r3]
00319908: mov r6, r0
0031990c: ldr r0, [ip]
00319910: add r3, r3, #8
00319914: str r3, [r6]
00319918: mov r8, r2
0031991c: str r0, [sp, #0xf4]
00319920: mov r7, r1
00319924: bl #0x31ce84
00319928: cmp r8, #0
0031992c: mov fp, r0
00319930: str r0, [r6, #4]
00319934: ble #0x319a00
00319938: ldr r3, [pc, #0x18c]
0031993c: mov r4, #1
00319940: add r5, sp, #0x84
00319944: add r3, pc, r3
00319948: str r3, [sp, #8]
0031994c: mov sl, #0x70
00319950: b #0x319958
00319954: ldr fp, [r6, #4]
00319958: mov r0, r5
0031995c: bl #0x3194e0
00319960: mov r0, fp
00319964: mov r1, r5
00319968: bl #0x3195c0
0031996c: mov r0, r5
00319970: bl #0x3193e8
00319974: ldr r3, [r6, #4]
00319978: ldm r3, {r0, r2}
0031997c: rsb r2, r0, r2
00319980: asr r2, r2, #4
00319984: add fp, r2, r2, lsl #3
00319988: add fp, fp, fp, lsl #6
0031998c: add fp, r2, fp, lsl #3
00319990: add fp, fp, fp, lsl #15
00319994: add fp, r2, fp, lsl #3
00319998: rsb fp, fp, #0
0031999c: subs fp, fp, #1
003199a0: bhs #0x3199b8
003199a4: ldr r0, [sp, #8]
003199a8: str r3, [sp, #4]
003199ac: bl #0x708eb0
003199b0: ldr r3, [sp, #4]
003199b4: ldr r0, [r3]
003199b8: movw r2, #0xd8ee
003199bc: movt r2, #0xffff
003199c0: rsb r2, r4, r2
003199c4: mla r0, sl, fp, r0
003199c8: add r4, r4, #1
003199cc: mov r1, r7
003199d0: bl #0x31c9c8
003199d4: cmp r8, r4
003199d8: bge #0x319954
003199dc: ldr r2, [sp, #0xc]
003199e0: mov r0, r6
003199e4: ldr r3, [sb, r2]
003199e8: ldr r2, [sp, #0xf4]
003199ec: ldr r3, [r3]
003199f0: cmp r2, r3
003199f4: bne #0x319abc
003199f8: add sp, sp, #0xfc
003199fc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00319a00: mov r0, r7
00319a04: bl #0x84b12c
00319a08: rsb r8, r8, #1
00319a0c: cmp r0, r8
00319a10: mov r5, r0
00319a14: blt #0x319aac
00319a18: ldr r3, [pc, #0xb0]
00319a1c: add r4, sp, #0x14
00319a20: mov sl, #0x70
00319a24: add r3, pc, r3
00319a28: str r3, [sp, #8]
00319a2c: ldr fp, [r6, #4]
00319a30: mov r0, r4
00319a34: bl #0x3194e0
00319a38: mov r0, fp
00319a3c: mov r1, r4
00319a40: bl #0x3195c0
00319a44: mov r0, r4
00319a48: bl #0x3193e8
00319a4c: ldr fp, [r6, #4]
00319a50: ldm fp, {r0, r2}
00319a54: rsb r2, r0, r2
00319a58: asr r2, r2, #4
00319a5c: add r3, r2, r2, lsl #3
00319a60: add r3, r3, r3, lsl #6
00319a64: add r3, r2, r3, lsl #3
00319a68: add r3, r3, r3, lsl #15
00319a6c: add r3, r2, r3, lsl #3
00319a70: rsb r3, r3, #0
00319a74: subs r3, r3, #1
00319a78: bhs #0x319a90
00319a7c: ldr r0, [sp, #8]
00319a80: str r3, [sp, #4]
00319a84: bl #0x708eb0
00319a88: ldr r0, [fp]
00319a8c: ldr r3, [sp, #4]
00319a90: mov r2, r8
00319a94: mla r0, sl, r3, r0
00319a98: add r8, r8, #1
00319a9c: mov r1, r7
00319aa0: bl #0x31c9c8
00319aa4: cmp r5, r8
00319aa8: bge #0x319a2c
00319aac: mov r0, r7
00319ab0: mvn r1, r5
00319ab4: bl #0x84b140
00319ab8: b #0x3199dc
00319abc: bl #0x30e310
00319ac0: andeq r4, r0, ip, lsr #1
00319ac4: mlseq r7, r4, r1, fp
00319ac8: andeq r2, r0, r0, ror #6
00319acc: subseq r4, sl, r4, lsr #22
00319ad0: subseq r4, sl, r4, asr #20

_ZN9LuaScript9_GetPyOIDERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f5fc 340
0037f5fc: push {r4, r5, r6, r7, r8, lr}
0037f600: ldr r6, [r0, #4]
0037f604: mov r4, r1
0037f608: ldr r5, [pc, #0x12c]
0037f60c: ldm r6, {r1, r3}
0037f610: add r5, pc, r5
0037f614: mov r7, r0
0037f618: rsb r3, r1, r3
0037f61c: asr r3, r3, #4
0037f620: add r2, r3, r3, lsl #3
0037f624: add r2, r2, r2, lsl #6
0037f628: add r2, r3, r2, lsl #3
0037f62c: add r2, r2, r2, lsl #15
0037f630: add r3, r3, r2, lsl #3
0037f634: rsb r3, r3, #0
0037f638: cmp r3, #1
0037f63c: bls #0x37f654
0037f640: cmp r3, #0
0037f644: beq #0x37f658
0037f648: ldr r3, [r1, #4]
0037f64c: cmp r3, #4
0037f650: beq #0x37f66c
0037f654: pop {r4, r5, r6, r7, r8, pc}
0037f658: ldr r0, [pc, #0xe0]
0037f65c: add r0, pc, r0
0037f660: bl #0x708eb0
0037f664: ldr r1, [r6]
0037f668: b #0x37f648
0037f66c: mov r0, r7
0037f670: mov r1, #1
0037f674: bl #0x37baf8
0037f678: ldr r3, [r0, #4]
0037f67c: cmp r3, #4
0037f680: bne #0x37f654
0037f684: ldr r6, [r7, #4]
0037f688: ldr r2, [pc, #0xb4]
0037f68c: ldm r6, {r0, r3}
0037f690: ldr r2, [r5, r2]
0037f694: rsb r3, r0, r3
0037f698: asr r3, r3, #4
0037f69c: ldr r8, [r2, #0x30]
0037f6a0: add r2, r3, r3, lsl #3
0037f6a4: add r2, r2, r2, lsl #6
0037f6a8: add r2, r3, r2, lsl #3
0037f6ac: add r2, r2, r2, lsl #15
0037f6b0: add r3, r3, r2, lsl #3
0037f6b4: cmp r3, #0
0037f6b8: bne #0x37f6cc
0037f6bc: ldr r0, [pc, #0x84]
0037f6c0: add r0, pc, r0
0037f6c4: bl #0x708eb0
0037f6c8: ldr r0, [r6]
0037f6cc: bl #0x31c49c
0037f6d0: ldr r5, [r7, #4]
0037f6d4: mov r6, r0
0037f6d8: ldm r5, {r0, r3}
0037f6dc: rsb r3, r0, r3
0037f6e0: asr r3, r3, #4
0037f6e4: add r2, r3, r3, lsl #3
0037f6e8: add r2, r2, r2, lsl #6
0037f6ec: add r2, r3, r2, lsl #3
0037f6f0: add r2, r2, r2, lsl #15
0037f6f4: add r3, r3, r2, lsl #3
0037f6f8: rsb r3, r3, #0
0037f6fc: cmp r3, #1
0037f700: bhi #0x37f714
0037f704: ldr r0, [pc, #0x40]
0037f708: add r0, pc, r0
0037f70c: bl #0x708eb0
0037f710: ldr r0, [r5]
0037f714: add r0, r0, #0x70
0037f718: bl #0x31c49c
0037f71c: mov r1, r6
0037f720: mov r2, r0
0037f724: mov r0, r8
0037f728: bl #0x4bd640
0037f72c: mov r1, r0
0037f730: mov r0, r4
0037f734: pop {r4, r5, r6, r7, r8, lr}
0037f738: b #0x37cb24
0037f73c: rsbeq r5, r1, r0, lsl #9
0037f740: subseq lr, r3, ip, lsl #28
0037f744: strdeq r3, r4, [r0], -r4
0037f748: subseq lr, r3, r8, lsr #27
0037f74c: subseq lr, r3, r0, ror #26

_ZN9Character17_CanAttackInMeleeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6f64 32
003b6f64: push {r4, lr}
003b6f68: add r0, r2, #0x37c
003b6f6c: mov r4, r1
003b6f70: bl #0x3ffd38
003b6f74: mov r1, r0
003b6f78: mov r0, r4
003b6f7c: pop {r4, lr}
003b6f80: b #0x37c7e4

_ZN3sfc6script3lua5Value14allocValueListEv 0x31ce84 784
0031ce84: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0031ce88: ldr r5, [pc, #0x2e0]
0031ce8c: ldr r6, [pc, #0x2e0]
0031ce90: sub sp, sp, #0x44
0031ce94: add r5, pc, r5
0031ce98: ldr lr, [r5, r6]
0031ce9c: add ip, sp, #0x18
0031cea0: mov r3, #0
0031cea4: str r3, [sp, #0x3c]
0031cea8: ldm lr, {r0, r1, r2, r3}
0031ceac: stm ip, {r0, r1, r2, r3}
0031ceb0: add r0, lr, #0x10
0031ceb4: mov r1, ip
0031ceb8: bl #0x31b618
0031cebc: cmp r0, #0
0031cec0: bne #0x31d010
0031cec4: ldr sl, [pc, #0x2ac]
0031cec8: ldr r2, [r5, sl]
0031cecc: ldr r3, [r2]
0031ced0: cmp r3, r2
0031ced4: beq #0x31cee4
0031ced8: ldr r3, [r3]
0031cedc: cmp r3, r2
0031cee0: bne #0x31ced8
0031cee4: mov r3, #0
0031cee8: mov r7, #0x14
0031ceec: add sb, sp, #0x40
0031cef0: str r3, [sp, #0x30]
0031cef4: str r3, [sp, #0x28]
0031cef8: str r3, [sp, #0x2c]
0031cefc: str r7, [sb, #-0xc]!
0031cf00: mov r0, sb
0031cf04: bl #0x708ec0
0031cf08: add r8, sp, #0x28
0031cf0c: mov r4, r0
0031cf10: mov r1, r8
0031cf14: add r0, r0, #8
0031cf18: bl #0x31c760
0031cf1c: ldr fp, [r5, sl]
0031cf20: mov r0, r8
0031cf24: ldr r3, [fp, #4]
0031cf28: str fp, [r4]
0031cf2c: str r3, [r4, #4]
0031cf30: str r4, [r3]
0031cf34: str r4, [fp, #4]
0031cf38: bl #0x31bf04
0031cf3c: ldr r4, [fp, #4]
0031cf40: str r7, [sp, #0x34]
0031cf44: ldr r2, [r4, #8]
0031cf48: ldr r3, [r4, #0x10]
0031cf4c: rsb r3, r2, r3
0031cf50: asr r3, r3, #4
0031cf54: add r1, r3, r3, lsl #3
0031cf58: add r1, r1, r1, lsl #6
0031cf5c: add r1, r3, r1, lsl #3
0031cf60: add r1, r1, r1, lsl #15
0031cf64: add r3, r3, r1, lsl #3
0031cf68: rsb r3, r3, #0
0031cf6c: cmp r3, #0x13
0031cf70: bhi #0x31cfe0
0031cf74: ldr r3, [r4, #0xc]
0031cf78: cmp r2, #0
0031cf7c: rsb r1, r2, r3
0031cf80: asr r1, r1, #4
0031cf84: add r8, r1, r1, lsl #3
0031cf88: add r8, r8, r8, lsl #6
0031cf8c: add r8, r1, r8, lsl #3
0031cf90: add r8, r8, r8, lsl #15
0031cf94: add r8, r1, r8, lsl #3
0031cf98: rsb r8, r8, #0
0031cf9c: beq #0x31d0b8
0031cfa0: add r7, r4, #8
0031cfa4: mov r1, sb
0031cfa8: mov r0, r7
0031cfac: bl #0x31c830
0031cfb0: mov sb, r0
0031cfb4: mov r0, r7
0031cfb8: bl #0x31bde0
0031cfbc: ldr r2, [sp, #0x34]
0031cfc0: mov r3, #0x70
0031cfc4: mla r8, r3, r8, sb
0031cfc8: mla r3, r3, r2, sb
0031cfcc: ldr r2, [r5, sl]
0031cfd0: str r3, [r4, #0x10]
0031cfd4: str r8, [r4, #0xc]
0031cfd8: str sb, [r4, #8]
0031cfdc: ldr r4, [r2, #4]
0031cfe0: ldr r0, [r5, r6]
0031cfe4: add r4, r4, #8
0031cfe8: str r4, [sp, #0x38]
0031cfec: ldr r2, [r0, #0x18]
0031cff0: ldr r3, [r0, #0x10]
0031cff4: sub r2, r2, #4
0031cff8: cmp r3, r2
0031cffc: beq #0x31d0d0
0031d000: str r4, [r3]
0031d004: ldr r3, [r0, #0x10]
0031d008: add r3, r3, #4
0031d00c: str r3, [r0, #0x10]
0031d010: ldr lr, [r5, r6]
0031d014: add ip, sp, #8
0031d018: ldm lr, {r0, r1, r2, r3}
0031d01c: stm ip, {r0, r1, r2, r3}
0031d020: add r0, lr, #0x10
0031d024: mov r1, ip
0031d028: bl #0x31b618
0031d02c: cmp r0, #0
0031d030: bne #0x31d054
0031d034: ldr r3, [pc, #0x140]
0031d038: ldr r3, [r5, r3]
0031d03c: ldr r3, [r3]
0031d040: cmp r3, #2
0031d044: streq r0, [r0]
0031d048: beq #0x31d054
0031d04c: cmp r3, #1
0031d050: beq #0x31d13c
0031d054: ldr r3, [r5, r6]
0031d058: ldr r2, [r3]
0031d05c: ldr r0, [r3, #8]
0031d060: ldr r1, [r2]
0031d064: sub r0, r0, #4
0031d068: cmp r2, r0
0031d06c: addne r2, r2, #4
0031d070: str r1, [sp, #0x3c]
0031d074: strne r2, [r3]
0031d078: beq #0x31d0dc
0031d07c: ldr r3, [pc, #0xfc]
0031d080: ldr r0, [r5, r3]
0031d084: ldr r2, [r0, #0x18]
0031d088: ldr r3, [r0, #0x10]
0031d08c: sub r2, r2, #4
0031d090: cmp r3, r2
0031d094: beq #0x31d130
0031d098: ldr r2, [sp, #0x3c]
0031d09c: str r2, [r3]
0031d0a0: ldr r3, [r0, #0x10]
0031d0a4: add r3, r3, #4
0031d0a8: str r3, [r0, #0x10]
0031d0ac: ldr r0, [sp, #0x3c]
0031d0b0: add sp, sp, #0x44
0031d0b4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031d0b8: mov r2, sb
0031d0bc: mov r1, r7
0031d0c0: add r0, r4, #0x10
0031d0c4: bl #0x319538
0031d0c8: mov sb, r0
0031d0cc: b #0x31cfbc
0031d0d0: add r1, sp, #0x38
0031d0d4: bl #0x31cd00
0031d0d8: b #0x31d010
0031d0dc: ldr r0, [r3, #4]
0031d0e0: cmp r0, #0
0031d0e4: beq #0x31d0f0
0031d0e8: mov r1, #0x80
0031d0ec: bl #0x708f00
0031d0f0: ldr r3, [r5, r6]
0031d0f4: ldr r2, [r3, #0xc]
0031d0f8: add r1, r2, #4
0031d0fc: str r1, [r3, #0xc]
0031d100: ldr r2, [r2, #4]
0031d104: add r1, r2, #0x80
0031d108: str r2, [r3]
0031d10c: str r2, [r3, #4]
0031d110: str r1, [r3, #8]
0031d114: ldr r3, [pc, #0x64]
0031d118: ldr r0, [r5, r3]
0031d11c: ldr r2, [r0, #0x18]
0031d120: ldr r3, [r0, #0x10]
0031d124: sub r2, r2, #4
0031d128: cmp r3, r2
0031d12c: bne #0x31d098
0031d130: add r1, sp, #0x3c
0031d134: bl #0x31cd00
0031d138: b #0x31d0ac
0031d13c: ldr r0, [pc, #0x40]
0031d140: ldr r1, [pc, #0x40]
0031d144: ldr r2, [pc, #0x40]
0031d148: ldr r0, [r5, r0]
0031d14c: ldr r3, [pc, #0x3c]
0031d150: mov ip, #0x32
0031d154: add r1, pc, r1
0031d158: add r2, pc, r2
0031d15c: add r3, pc, r3
0031d160: add r0, r0, #0xa8
0031d164: str ip, [sp]
0031d168: bl #0x30e004
0031d16c: b #0x31d054

_ZN3sfc6script3lua8Instance13includeStringEv 0x31b008 8
0031b008: ldr r0, [r0, #4]
0031b00c: b #0x85a0dc

_ZN9Character19_CanAttackFromRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6f10 44
003b6f10: push {r4, lr}
003b6f14: ldr r3, [r2]
003b6f18: mov r0, r2
003b6f1c: mov r4, r1
003b6f20: mov lr, pc
003b6f24: ldr pc, [r3, #0x124]
003b6f28: mov r3, r0
003b6f2c: mov r1, r3
003b6f30: mov r0, r4
003b6f34: pop {r4, lr}
003b6f38: b #0x37c7e4

_ZN9LuaScript10_StopSoundERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e730 228
0037e730: push {r4, r5, r6, lr}
0037e734: ldr r5, [r0, #4]
0037e738: mov r6, r0
0037e73c: ldr r4, [pc, #0xc0]
0037e740: ldm r5, {r0, r3}
0037e744: add r4, pc, r4
0037e748: rsb r3, r0, r3
0037e74c: asr r3, r3, #4
0037e750: add r2, r3, r3, lsl #3
0037e754: add r2, r2, r2, lsl #6
0037e758: add r2, r3, r2, lsl #3
0037e75c: add r2, r2, r2, lsl #15
0037e760: add r3, r3, r2, lsl #3
0037e764: cmp r3, #0
0037e768: bne #0x37e77c
0037e76c: ldr r0, [pc, #0x94]
0037e770: add r0, pc, r0
0037e774: bl #0x708eb0
0037e778: ldr r0, [r5]
0037e77c: bl #0x31c49c
0037e780: bl #0x37ba84
0037e784: cmn r0, #1
0037e788: mov r5, r0
0037e78c: beq #0x37e800
0037e790: ldr r6, [r6, #4]
0037e794: ldr r2, [pc, #0x70]
0037e798: ldm r6, {r0, r3}
0037e79c: ldr r2, [r4, r2]
0037e7a0: rsb r3, r0, r3
0037e7a4: asr r3, r3, #4
0037e7a8: ldr r4, [r2]
0037e7ac: add r2, r3, r3, lsl #3
0037e7b0: add r2, r2, r2, lsl #6
0037e7b4: add r2, r3, r2, lsl #3
0037e7b8: add r2, r2, r2, lsl #15
0037e7bc: add r3, r3, r2, lsl #3
0037e7c0: rsb r3, r3, #0
0037e7c4: cmp r3, #1
0037e7c8: bls #0x37e7ec
0037e7cc: add r0, r0, #0x70
0037e7d0: bl #0x31bbf0
0037e7d4: bl #0x30e4cc
0037e7d8: mov r1, r5
0037e7dc: mov r2, r0
0037e7e0: mov r0, r4
0037e7e4: pop {r4, r5, r6, lr}
0037e7e8: b #0x369fec
0037e7ec: ldr r0, [pc, #0x1c]
0037e7f0: add r0, pc, r0
0037e7f4: bl #0x708eb0
0037e7f8: ldr r0, [r6]
0037e7fc: b #0x37e7cc
0037e800: pop {r4, r5, r6, pc}
0037e804: rsbeq r6, r1, ip, asr #6
0037e808: ldrsheq pc, [r3], #-0xc8
0037e80c: andeq r0, r0, r4, lsr #27
0037e810: subseq pc, r3, r8, ror ip

_ZN3sfc6script3lua8InstanceD0Ev 0x31b1c4 28
0031b1c4: push {r4, lr}
0031b1c8: mov r4, r0
0031b1cc: bl #0x31b180
0031b1d0: mov r0, r4
0031b1d4: bl #0x310440
0031b1d8: mov r0, r4
0031b1dc: pop {r4, pc}

_ZN3sfc6script3lua8Instance5pCallEPKcRNS1_12ReturnValuesE 0x31ab14 56
0031ab14: push {r4, r5, r6, lr}
0031ab18: mov r3, r1
0031ab1c: mvn r1, #0x2700
0031ab20: mov r4, r0
0031ab24: mov r5, r2
0031ab28: sub r1, r1, #0x11
0031ab2c: mov r2, r3
0031ab30: ldr r0, [r0, #4]
0031ab34: bl #0x84c1ec
0031ab38: mov r0, r4
0031ab3c: mov r2, r5
0031ab40: mov r1, #0
0031ab44: pop {r4, r5, r6, lr}
0031ab48: b #0x31aa78

_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE20_M_allocate_and_copyIPS3_EES7_RjT_S9_ 0x31c830 108
0031c830: push {r4, r5, r6, r7, r8, lr}
0031c834: add r0, r0, #8
0031c838: mov r4, r2
0031c83c: mov r2, r1
0031c840: ldr r1, [r1]
0031c844: mov r5, r3
0031c848: bl #0x319538
0031c84c: rsb r5, r4, r5
0031c850: asr r5, r5, #4
0031c854: mov r7, r0
0031c858: add r6, r5, r5, lsl #3
0031c85c: add r6, r6, r6, lsl #6
0031c860: add r6, r5, r6, lsl #3
0031c864: add r6, r6, r6, lsl #15
0031c868: add r6, r5, r6, lsl #3
0031c86c: rsb r6, r6, #0
0031c870: cmp r6, #0
0031c874: ble #0x31c894
0031c878: mov r5, #0
0031c87c: add r0, r7, r5
0031c880: add r1, r4, r5
0031c884: bl #0x31c634
0031c888: subs r6, r6, #1
0031c88c: add r5, r5, #0x70
0031c890: bne #0x31c87c
0031c894: mov r0, r7
0031c898: pop {r4, r5, r6, r7, r8, pc}

_ZN10GameObject9_IsPlayerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38e9f0 44
0038e9f0: push {r4, lr}
0038e9f4: ldr r3, [r2]
0038e9f8: mov r0, r2
0038e9fc: mov r4, r1
0038ea00: mov lr, pc
0038ea04: ldr pc, [r3, #0x28]
0038ea08: mov r3, r0
0038ea0c: mov r1, r3
0038ea10: mov r0, r4
0038ea14: pop {r4, lr}
0038ea18: b #0x37c7e4

_ZN10GameObject7_IsDeadERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ea48 44
0038ea48: push {r4, lr}
0038ea4c: ldr r3, [r2]
0038ea50: mov r0, r2
0038ea54: mov r4, r1
0038ea58: mov lr, pc
0038ea5c: ldr pc, [r3, #0x34]
0038ea60: mov r3, r0
0038ea64: mov r1, r3
0038ea68: mov r0, r4
0038ea6c: pop {r4, lr}
0038ea70: b #0x37c7e4

_ZN9Character12_GetHitCountERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6cc8 16
003b6cc8: movw r3, #0x14d0
003b6ccc: mov r0, r1
003b6cd0: ldrh r1, [r2, r3]
003b6cd4: b #0x37cb24

_ZNSaIPPSt6vectorIN3sfc6script3lua5ValueESaIS3_EEE8allocateEjPKv 0x31bd80 96
0031bd80: str lr, [sp, #-4]!
0031bd84: cmn r1, #0xc0000001
0031bd88: sub sp, sp, #0xc
0031bd8c: bhi #0x31bdc8
0031bd90: cmp r1, #0
0031bd94: moveq r0, r1
0031bd98: bne #0x31bda4
0031bd9c: add sp, sp, #0xc
0031bda0: ldm sp!, {pc}
0031bda4: lsl r0, r1, #2
0031bda8: cmp r0, #0x80
0031bdac: str r0, [sp, #4]
0031bdb0: bhi #0x31bdc0
0031bdb4: add r0, sp, #4
0031bdb8: bl #0x708ec0
0031bdbc: b #0x31bd9c
0031bdc0: bl #0x310454
0031bdc4: b #0x31bd9c
0031bdc8: ldr r0, [pc, #0xc]
0031bdcc: add r0, pc, r0
0031bdd0: bl #0x30e0c4
0031bdd4: mov r0, #1
0031bdd8: bl #0x30de48
0031bddc: subseq r2, sl, r4, lsr #13

_ZN9LuaScript7_BitXOrERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f750 196
0037f750: push {r4, r5, r6, lr}
0037f754: ldr r3, [r0, #4]
0037f758: mov r4, r0
0037f75c: mov r5, r1
0037f760: ldm r3, {r0, r2}
0037f764: rsb r3, r0, r2
0037f768: asr r3, r3, #4
0037f76c: add r2, r3, r3, lsl #3
0037f770: add r2, r2, r2, lsl #6
0037f774: add r2, r3, r2, lsl #3
0037f778: add r2, r2, r2, lsl #15
0037f77c: add r3, r3, r2, lsl #3
0037f780: cmn r3, #2
0037f784: beq #0x37f78c
0037f788: pop {r4, r5, r6, pc}
0037f78c: ldr r3, [r0, #4]
0037f790: cmp r3, #3
0037f794: bne #0x37f788
0037f798: ldr r3, [r0, #0x74]
0037f79c: cmp r3, #3
0037f7a0: bne #0x37f788
0037f7a4: bl #0x31bbf0
0037f7a8: bl #0x30e4cc
0037f7ac: ldr r4, [r4, #4]
0037f7b0: mov r6, r0
0037f7b4: ldm r4, {r0, r3}
0037f7b8: rsb r3, r0, r3
0037f7bc: asr r3, r3, #4
0037f7c0: add r2, r3, r3, lsl #3
0037f7c4: add r2, r2, r2, lsl #6
0037f7c8: add r2, r3, r2, lsl #3
0037f7cc: add r2, r2, r2, lsl #15
0037f7d0: add r3, r3, r2, lsl #3
0037f7d4: rsb r3, r3, #0
0037f7d8: cmp r3, #1
0037f7dc: bls #0x37f7fc
0037f7e0: add r0, r0, #0x70
0037f7e4: bl #0x31bbf0
0037f7e8: bl #0x30e4cc
0037f7ec: eor r1, r0, r6
0037f7f0: mov r0, r5
0037f7f4: pop {r4, r5, r6, lr}
0037f7f8: b #0x37cb24
0037f7fc: ldr r0, [pc, #0xc]
0037f800: add r0, pc, r0
0037f804: bl #0x708eb0
0037f808: ldr r0, [r4]
0037f80c: b #0x37f7e0
0037f810: subseq lr, r3, r8, ror #24

_ZN3sfc6script3lua9ArgumentsC1EP9lua_Statei 0x3196ec 500
003196ec: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003196f0: ldr r3, [pc, #0x1d4]
003196f4: sub sp, sp, #0xfc
003196f8: ldr sb, [pc, #0x1d0]
003196fc: str r3, [sp, #0xc]
00319700: ldr lr, [sp, #0xc]
00319704: ldr r3, [pc, #0x1c8]
00319708: add sb, pc, sb
0031970c: ldr ip, [sb, lr]
00319710: ldr r3, [sb, r3]
00319714: mov r6, r0
00319718: ldr r0, [ip]
0031971c: add r3, r3, #8
00319720: str r3, [r6]
00319724: mov r8, r2
00319728: str r0, [sp, #0xf4]
0031972c: mov r7, r1
00319730: bl #0x31ce84
00319734: cmp r8, #0
00319738: mov fp, r0
0031973c: str r0, [r6, #4]
00319740: ble #0x31980c
00319744: ldr r3, [pc, #0x18c]
00319748: mov r4, #1
0031974c: add r5, sp, #0x84
00319750: add r3, pc, r3
00319754: str r3, [sp, #8]
00319758: mov sl, #0x70
0031975c: b #0x319764
00319760: ldr fp, [r6, #4]
00319764: mov r0, r5
00319768: bl #0x3194e0
0031976c: mov r0, fp
00319770: mov r1, r5
00319774: bl #0x3195c0
00319778: mov r0, r5
0031977c: bl #0x3193e8
00319780: ldr r3, [r6, #4]
00319784: ldm r3, {r0, r2}
00319788: rsb r2, r0, r2
0031978c: asr r2, r2, #4
00319790: add fp, r2, r2, lsl #3
00319794: add fp, fp, fp, lsl #6
00319798: add fp, r2, fp, lsl #3
0031979c: add fp, fp, fp, lsl #15
003197a0: add fp, r2, fp, lsl #3
003197a4: rsb fp, fp, #0
003197a8: subs fp, fp, #1
003197ac: bhs #0x3197c4
003197b0: ldr r0, [sp, #8]
003197b4: str r3, [sp, #4]
003197b8: bl #0x708eb0
003197bc: ldr r3, [sp, #4]
003197c0: ldr r0, [r3]
003197c4: movw r2, #0xd8ee
003197c8: movt r2, #0xffff
003197cc: rsb r2, r4, r2
003197d0: mla r0, sl, fp, r0
003197d4: add r4, r4, #1
003197d8: mov r1, r7
003197dc: bl #0x31c9c8
003197e0: cmp r8, r4
003197e4: bge #0x319760
003197e8: ldr r2, [sp, #0xc]
003197ec: mov r0, r6
003197f0: ldr r3, [sb, r2]
003197f4: ldr r2, [sp, #0xf4]
003197f8: ldr r3, [r3]
003197fc: cmp r2, r3
00319800: bne #0x3198c8
00319804: add sp, sp, #0xfc
00319808: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031980c: mov r0, r7
00319810: bl #0x84b12c
00319814: rsb r8, r8, #1
00319818: cmp r0, r8
0031981c: mov r5, r0
00319820: blt #0x3198b8
00319824: ldr r3, [pc, #0xb0]
00319828: add r4, sp, #0x14
0031982c: mov sl, #0x70
00319830: add r3, pc, r3
00319834: str r3, [sp, #8]
00319838: ldr fp, [r6, #4]
0031983c: mov r0, r4
00319840: bl #0x3194e0
00319844: mov r0, fp
00319848: mov r1, r4
0031984c: bl #0x3195c0
00319850: mov r0, r4
00319854: bl #0x3193e8
00319858: ldr fp, [r6, #4]
0031985c: ldm fp, {r0, r2}
00319860: rsb r2, r0, r2
00319864: asr r2, r2, #4
00319868: add r3, r2, r2, lsl #3
0031986c: add r3, r3, r3, lsl #6
00319870: add r3, r2, r3, lsl #3
00319874: add r3, r3, r3, lsl #15
00319878: add r3, r2, r3, lsl #3
0031987c: rsb r3, r3, #0
00319880: subs r3, r3, #1
00319884: bhs #0x31989c
00319888: ldr r0, [sp, #8]
0031988c: str r3, [sp, #4]
00319890: bl #0x708eb0
00319894: ldr r0, [fp]
00319898: ldr r3, [sp, #4]
0031989c: mov r2, r8
003198a0: mla r0, sl, r3, r0
003198a4: add r8, r8, #1
003198a8: mov r1, r7
003198ac: bl #0x31c9c8
003198b0: cmp r5, r8
003198b4: bge #0x319838
003198b8: mov r0, r7
003198bc: mvn r1, r5
003198c0: bl #0x84b140
003198c4: b #0x3197e8
003198c8: bl #0x30e310
003198cc: andeq r4, r0, ip, lsr #1
003198d0: rsbeq fp, r7, r8, lsl #7
003198d4: andeq r2, r0, r0, ror #6
003198d8: subseq r4, sl, r8, lsl sp
003198dc: subseq r4, sl, r8, lsr ip

_ZNSt4listISt6vectorIN3sfc6script3lua5ValueESaIS4_EESaIS6_EED1Ev 0x31bfd8 20
0031bfd8: push {r4, lr}
0031bfdc: mov r4, r0
0031bfe0: bl #0x31bf8c
0031bfe4: mov r0, r4
0031bfe8: pop {r4, pc}

_ZN9Character14_AllowRotationERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b79ec 120
003b79ec: str lr, [sp, #-4]!
003b79f0: ldr r3, [r0, #4]
003b79f4: sub sp, sp, #0xc
003b79f8: ldr r1, [r3, #4]
003b79fc: ldr ip, [r3]
003b7a00: rsb r3, ip, r1
003b7a04: asr r3, r3, #4
003b7a08: add r1, r3, r3, lsl #3
003b7a0c: add r1, r1, r1, lsl #6
003b7a10: add r1, r3, r1, lsl #3
003b7a14: add r1, r1, r1, lsl #15
003b7a18: add r3, r3, r1, lsl #3
003b7a1c: cmp r3, #0
003b7a20: bne #0x3b7a2c
003b7a24: add sp, sp, #0xc
003b7a28: ldm sp!, {pc}
003b7a2c: ldr r3, [ip, #4]
003b7a30: cmp r3, #1
003b7a34: bne #0x3b7a24
003b7a38: mov r1, #0
003b7a3c: str r2, [sp, #4]
003b7a40: bl #0x37baf8
003b7a44: bl #0x31bc80
003b7a48: ldr r2, [sp, #4]
003b7a4c: cmp r0, #0
003b7a50: ldr r3, [r2, #0x520]
003b7a54: bicne r3, r3, #0x4000
003b7a58: orreq r3, r3, #0x4000
003b7a5c: str r3, [r2, #0x520]
003b7a60: b #0x3b7a24

_ZN3sfc6script3lua8Instance11includeMathEv 0x31b000 8
0031b000: ldr r0, [r0, #4]
0031b004: b #0x853e38

_ZN3sfc6script3lua5Value13_setFromStackEP9lua_Statei 0x31c9c8 252
0031c9c8: push {r4, r5, r6, lr}
0031c9cc: mov r5, r1
0031c9d0: mov r4, r0
0031c9d4: mov r1, r2
0031c9d8: mov r0, r5
0031c9dc: mov r6, r2
0031c9e0: bl #0x84b264
0031c9e4: str r0, [r4, #4]
0031c9e8: cmp r0, #5
0031c9ec: addls pc, pc, r0, lsl #2
0031c9f0: b #0x31ca0c
0031c9f4: b #0x31ca14
0031c9f8: b #0x31ca54
0031c9fc: b #0x31ca6c
0031ca00: b #0x31ca80
0031ca04: b #0x31ca94
0031ca08: b #0x31ca18
0031ca0c: mov r3, #0
0031ca10: str r3, [r4, #4]
0031ca14: pop {r4, r5, r6, pc}
0031ca18: ldr r2, [pc, #0xa0]
0031ca1c: mov r1, r6
0031ca20: mov r0, r5
0031ca24: add r2, pc, r2
0031ca28: bl #0x84c1ec
0031ca2c: mvn r1, #0
0031ca30: mov r0, r5
0031ca34: bl #0x84b390
0031ca38: mvn r1, #1
0031ca3c: str r0, [r4, #0x6c]
0031ca40: mov r0, r5
0031ca44: bl #0x84b140
0031ca48: mov r3, #7
0031ca4c: str r3, [r4, #4]
0031ca50: pop {r4, r5, r6, pc}
0031ca54: mov r1, r6
0031ca58: mov r0, r5
0031ca5c: bl #0x84b320
0031ca60: bl #0x30e964
0031ca64: str r0, [r4, #8]
0031ca68: pop {r4, r5, r6, pc}
0031ca6c: mov r0, r5
0031ca70: mov r1, r6
0031ca74: bl #0x84b390
0031ca78: str r0, [r4, #0x6c]
0031ca7c: pop {r4, r5, r6, pc}
0031ca80: mov r0, r5
0031ca84: mov r1, r6
0031ca88: bl #0x84c450
0031ca8c: str r0, [r4, #8]
0031ca90: pop {r4, r5, r6, pc}
0031ca94: mov r1, r6
0031ca98: mov r2, #0
0031ca9c: mov r0, r5
0031caa0: bl #0x84c384
0031caa4: mov r5, r0
0031caa8: bl #0x30de54
0031caac: mov r1, r5
0031cab0: add r2, r5, r0
0031cab4: add r0, r4, #0xc
0031cab8: pop {r4, r5, r6, lr}
0031cabc: b #0x3109e0
0031cac0: subseq r1, sl, ip, lsr #28

_ZN9Character16_SpellCombatRollERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b92a4 296
003b92a4: push {r4, r5, r6, r7, r8, lr}
003b92a8: mov r6, r0
003b92ac: sub sp, sp, #0x40
003b92b0: mov r0, r2
003b92b4: mov r5, r1
003b92b8: mvn r1, #0
003b92bc: mov r4, r2
003b92c0: bl #0x3bb98c
003b92c4: ldr r2, [r6, #4]
003b92c8: mov r7, r0
003b92cc: ldm r2, {r0, r3}
003b92d0: rsb r3, r0, r3
003b92d4: asr r3, r3, #4
003b92d8: add r2, r3, r3, lsl #3
003b92dc: add r2, r2, r2, lsl #6
003b92e0: add r2, r3, r2, lsl #3
003b92e4: add r2, r2, r2, lsl #15
003b92e8: add r3, r3, r2, lsl #3
003b92ec: cmp r3, #0
003b92f0: bne #0x3b92fc
003b92f4: add sp, sp, #0x40
003b92f8: pop {r4, r5, r6, r7, r8, pc}
003b92fc: ldr r3, [r0, #4]
003b9300: cmp r3, #2
003b9304: beq #0x3b9310
003b9308: cmp r3, #7
003b930c: bne #0x3b92f4
003b9310: bl #0x31b5a0
003b9314: subs r8, r0, #0
003b9318: beq #0x3b92f4
003b931c: add r6, sp, #0x34
003b9320: mov r0, r6
003b9324: mov r1, r8
003b9328: bl #0x33dd2c
003b932c: mov r0, r6
003b9330: bl #0x33ff54
003b9334: subs r6, r0, #0
003b9338: beq #0x3b938c
003b933c: mov r1, r7
003b9340: mov r0, r4
003b9344: bl #0x3aeac0
003b9348: add r7, sp, #0xc
003b934c: ldr r3, [r0, #8]
003b9350: mov r8, #0
003b9354: mov r0, r7
003b9358: mov r1, r4
003b935c: mov r2, r6
003b9360: str r8, [sp]
003b9364: bl #0x3b3004
003b9368: mov r0, r7
003b936c: mov r1, r4
003b9370: mov r2, r6
003b9374: mov r3, r8
003b9378: bl #0x3b10b4
003b937c: mov r0, r5
003b9380: ldr r1, [sp, #0xc]
003b9384: bl #0x37cb24
003b9388: b #0x3b92f4
003b938c: ldr r3, [r8]
003b9390: mov r0, r8
003b9394: mov r1, r4
003b9398: mov lr, pc
003b939c: ldr pc, [r3, #0x90]
003b93a0: cmp r0, #8
003b93a4: bne #0x3b92f4
003b93a8: mov r0, r8
003b93ac: mov r1, r4
003b93b0: ldr r3, [r8]
003b93b4: mov lr, pc
003b93b8: ldr pc, [r3, #0x98]
003b93bc: mov r0, r5
003b93c0: mov r1, r6
003b93c4: bl #0x37c7e4
003b93c8: b #0x3b92f4

_ZN3sfc6script3lua5ValueC1EPv 0x31a414 88
0031a414: ldr r3, [pc, #0x48]
0031a418: ldr ip, [pc, #0x48]
0031a41c: mov r2, r0
0031a420: add r3, pc, r3
0031a424: ldr ip, [r3, ip]
0031a428: push {r4, lr}
0031a42c: add ip, ip, #8
0031a430: str ip, [r2], #0xc
0031a434: mov lr, #0
0031a438: add ip, r0, #0x24
0031a43c: mov r4, r0
0031a440: str r2, [r0, #0x20]
0031a444: str ip, [r0, #0x68]
0031a448: str lr, [r0, #0x24]
0031a44c: str r2, [r0, #0x1c]
0031a450: strb lr, [r0, #0xc]
0031a454: str ip, [r0, #0x64]
0031a458: bl #0x31b5f8
0031a45c: mov r0, r4
0031a460: pop {r4, pc}
0031a464: rsbeq sl, r7, r0, ror r6
0031a468: muleq r0, r8, r7

_ZN3sfc6script3lua5ValueC1EPKc 0x37c84c 128
0037c84c: ldr r2, [pc, #0x70]
0037c850: ldr ip, [pc, #0x70]
0037c854: mov r3, r0
0037c858: add r2, pc, r2
0037c85c: ldr ip, [r2, ip]
0037c860: push {r4, r5, r6, lr}
0037c864: add ip, ip, #8
0037c868: mov r4, r0
0037c86c: str ip, [r3], #0xc
0037c870: mov r6, r1
0037c874: mov r0, r3
0037c878: str r3, [r4, #0x1c]
0037c87c: str r3, [r4, #0x20]
0037c880: mov r1, #0x10
0037c884: bl #0x31167c
0037c888: ldr r2, [r4, #0x1c]
0037c88c: add r3, r4, #0x24
0037c890: mov r5, #0
0037c894: strb r5, [r2]
0037c898: mov r0, r3
0037c89c: str r3, [r4, #0x64]
0037c8a0: str r3, [r4, #0x68]
0037c8a4: bl #0x37be44
0037c8a8: ldr r3, [r4, #0x64]
0037c8ac: mov r0, r4
0037c8b0: mov r1, r6
0037c8b4: str r5, [r3]
0037c8b8: bl #0x31c46c
0037c8bc: mov r0, r4
0037c8c0: pop {r4, r5, r6, pc}
0037c8c4: rsbeq r8, r1, r8, lsr r2
0037c8c8: muleq r0, r8, r7

_ZN9Character14_GetSpotTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6c88 64
003b6c88: push {r4, r5, r6, lr}
003b6c8c: movw r3, #0x14bc
003b6c90: mov r4, r1
003b6c94: mov r0, r4
003b6c98: ldr r1, [r2, r3]
003b6c9c: mov r5, r2
003b6ca0: bl #0x37ccbc
003b6ca4: mov r3, #0x14c0
003b6ca8: ldr r1, [r5, r3]
003b6cac: mov r0, r4
003b6cb0: bl #0x37ccbc
003b6cb4: movw r3, #0x14c4
003b6cb8: ldr r1, [r5, r3]
003b6cbc: mov r0, r4
003b6cc0: pop {r4, r5, r6, lr}
003b6cc4: b #0x37ccbc

_ZN9Character20_GetCurrentSkillInfoERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8f9c 232
003b8f9c: push {r4, r5, r6, r7, r8, lr}
003b8fa0: ldr r3, [r0, #4]
003b8fa4: mov r4, r0
003b8fa8: mov r6, r2
003b8fac: ldm r3, {r0, r2}
003b8fb0: mov r5, r1
003b8fb4: rsb r3, r0, r2
003b8fb8: asr r3, r3, #4
003b8fbc: add r2, r3, r3, lsl #3
003b8fc0: add r2, r2, r2, lsl #6
003b8fc4: add r2, r3, r2, lsl #3
003b8fc8: add r2, r2, r2, lsl #15
003b8fcc: add r3, r3, r2, lsl #3
003b8fd0: cmp r3, #0
003b8fd4: bne #0x3b8fdc
003b8fd8: pop {r4, r5, r6, r7, r8, pc}
003b8fdc: ldr r3, [r0, #4]
003b8fe0: cmp r3, #3
003b8fe4: beq #0x3b904c
003b8fe8: bl #0x31bbf0
003b8fec: mov r8, r0
003b8ff0: mov r0, r6
003b8ff4: bl #0x3bc5fc
003b8ff8: mov r7, r0
003b8ffc: mov r0, r8
003b9000: bl #0x8be2a0
003b9004: ldr r3, [r7, #4]
003b9008: cmp r3, r0
003b900c: bls #0x3b8fd8
003b9010: ldr r4, [r4, #4]
003b9014: ldm r4, {r0, r3}
003b9018: rsb r3, r0, r3
003b901c: asr r3, r3, #4
003b9020: add r2, r3, r3, lsl #3
003b9024: add r2, r2, r2, lsl #6
003b9028: add r2, r3, r2, lsl #3
003b902c: add r2, r2, r2, lsl #15
003b9030: add r3, r3, r2, lsl #3
003b9034: cmp r3, #0
003b9038: bne #0x3b904c
003b903c: ldr r0, [pc, #0x3c]
003b9040: add r0, pc, r0
003b9044: bl #0x708eb0
003b9048: ldr r0, [r4]
003b904c: bl #0x31bbf0
003b9050: bl #0x30e4cc
003b9054: mov r4, r0
003b9058: mov r1, r4
003b905c: mov r0, r6
003b9060: bl #0x3bc784
003b9064: mov r1, r4
003b9068: mov r0, r6
003b906c: bl #0x3bbed0
003b9070: mov r1, r0
003b9074: mov r0, r5
003b9078: pop {r4, r5, r6, r7, r8, lr}
003b907c: b #0x37cb24
003b9080: subseq r5, r0, r8, lsr #8

_ZN9Character9_RotateByERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7704 112
003b7704: push {r4, lr}
003b7708: ldr r3, [r0, #4]
003b770c: ldr r1, [r3, #4]
003b7710: ldr ip, [r3]
003b7714: rsb r3, ip, r1
003b7718: asr r3, r3, #4
003b771c: add r1, r3, r3, lsl #3
003b7720: add r1, r1, r1, lsl #6
003b7724: add r1, r3, r1, lsl #3
003b7728: add r1, r1, r1, lsl #15
003b772c: add r3, r3, r1, lsl #3
003b7730: cmp r3, #0
003b7734: bne #0x3b773c
003b7738: pop {r4, pc}
003b773c: ldr r3, [ip, #4]
003b7740: cmp r3, #3
003b7744: bne #0x3b7738
003b7748: mov r1, #0
003b774c: ldr r4, [r2, #0x378]
003b7750: bl #0x37baf8
003b7754: bl #0x31bbf0
003b7758: movw r1, #0xfa35
003b775c: movt r1, #0x3c8e
003b7760: bl #0x30ed6c
003b7764: mov r1, r0
003b7768: mov r0, r4
003b776c: pop {r4, lr}
003b7770: b #0x405204

_ZN9Character23_GetPropBonusCritRatingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7b28 92
003b7b28: push {r4, r5, r6, lr}
003b7b2c: ldr r3, [r0, #4]
003b7b30: mov r5, r2
003b7b34: mov r4, r1
003b7b38: ldm r3, {r0, r2}
003b7b3c: rsb r3, r0, r2
003b7b40: asr r3, r3, #4
003b7b44: add r2, r3, r3, lsl #3
003b7b48: add r2, r2, r2, lsl #6
003b7b4c: add r2, r3, r2, lsl #3
003b7b50: add r2, r2, r2, lsl #15
003b7b54: add r3, r3, r2, lsl #3
003b7b58: cmp r3, #0
003b7b5c: bne #0x3b7b64
003b7b60: pop {r4, r5, r6, pc}
003b7b64: bl #0x31bc80
003b7b68: mov r1, r0
003b7b6c: add r0, r5, #0x560
003b7b70: bl #0x3df7c4
003b7b74: mov r1, r0
003b7b78: mov r0, r4
003b7b7c: pop {r4, r5, r6, lr}
003b7b80: b #0x37cb24

_ZN3sfc6script3lua12ReturnValues11pushBooleanEb 0x37c7e4 104
0037c7e4: ldr r3, [pc, #0x58]
0037c7e8: ldr r2, [pc, #0x58]
0037c7ec: push {r4, r5, r6, lr}
0037c7f0: add r3, pc, r3
0037c7f4: ldr r5, [r3, r2]
0037c7f8: sub sp, sp, #0x78
0037c7fc: add r4, sp, #4
0037c800: ldr r3, [r5]
0037c804: str r3, [sp, #0x74]
0037c808: ldr r6, [r0, #0x24]
0037c80c: mov r0, r4
0037c810: bl #0x37c764
0037c814: mov r0, r6
0037c818: mov r1, r4
0037c81c: bl #0x3195c0
0037c820: mov r0, r4
0037c824: bl #0x3193e8
0037c828: ldr r2, [sp, #0x74]
0037c82c: ldr r3, [r5]
0037c830: cmp r2, r3
0037c834: bne #0x37c840
0037c838: add sp, sp, #0x78
0037c83c: pop {r4, r5, r6, pc}
0037c840: bl #0x30e310
0037c844: rsbeq r8, r1, r0, lsr #5
0037c848: andeq r4, r0, ip, lsr #1

_ZN10LuaManager7AddFileEP9LuaScriptPKc 0x37b23c 808
0037b23c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037b240: ldr r4, [pc, #0x2e0]
0037b244: ldr r6, [pc, #0x2e0]
0037b248: sub sp, sp, #0x7c
0037b24c: add r4, pc, r4
0037b250: ldr r3, [r4, r6]
0037b254: subs r7, r1, #0
0037b258: mov sl, r0
0037b25c: ldr r3, [r3]
0037b260: mov r5, r2
0037b264: str r3, [sp, #0x74]
0037b268: beq #0x37b318
0037b26c: cmp r5, #0
0037b270: beq #0x37b280
0037b274: ldrsb r3, [r5]
0037b278: cmp r3, #0
0037b27c: bne #0x37b2a4
0037b280: mov r5, #0
0037b284: ldr r3, [r4, r6]
0037b288: ldr r2, [sp, #0x74]
0037b28c: mov r0, r5
0037b290: ldr r3, [r3]
0037b294: cmp r2, r3
0037b298: bne #0x37b524
0037b29c: add sp, sp, #0x7c
0037b2a0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037b2a4: add r8, sp, #0x5c
0037b2a8: mov r0, r8
0037b2ac: add r1, r7, #0x68
0037b2b0: mov r2, r5
0037b2b4: bl #0x3338cc
0037b2b8: ldr r1, [pc, #0x270]
0037b2bc: mov r0, r5
0037b2c0: add r1, pc, r1
0037b2c4: bl #0x30ebd4
0037b2c8: cmp r0, #0
0037b2cc: beq #0x37b3f8
0037b2d0: ldr r1, [pc, #0x25c]
0037b2d4: mov r2, #5
0037b2d8: add r1, pc, r1
0037b2dc: bl #0x30ec7c
0037b2e0: cmp r0, #0
0037b2e4: bne #0x37b36c
0037b2e8: ldr r3, [sp, #0x70]
0037b2ec: add r1, sp, #0x78
0037b2f0: add r5, r7, #0x80
0037b2f4: str r3, [r1, #-0x5c]!
0037b2f8: mov r0, r5
0037b2fc: bl #0x37a190
0037b300: cmp r5, r0
0037b304: beq #0x37b380
0037b308: mov r5, #1
0037b30c: mov r0, r8
0037b310: bl #0x3139ac
0037b314: b #0x37b284
0037b318: ldr r3, [pc, #0x218]
0037b31c: ldr r3, [r4, r3]
0037b320: ldr r3, [r3]
0037b324: cmp r3, #2
0037b328: streq r7, [r7]
0037b32c: beq #0x37b26c
0037b330: cmp r3, #1
0037b334: bne #0x37b26c
0037b338: ldr r0, [pc, #0x1fc]
0037b33c: ldr r1, [pc, #0x1fc]
0037b340: ldr r2, [pc, #0x1fc]
0037b344: ldr r0, [r4, r0]
0037b348: ldr r3, [pc, #0x1f8]
0037b34c: mov ip, #0x23
0037b350: add r1, pc, r1
0037b354: add r2, pc, r2
0037b358: add r3, pc, r3
0037b35c: add r0, r0, #0xa8
0037b360: str ip, [sp]
0037b364: bl #0x30e004
0037b368: b #0x37b26c
0037b36c: ldr r1, [pc, #0x1d8]
0037b370: mov r0, r8
0037b374: add r1, pc, r1
0037b378: bl #0x379ef8
0037b37c: b #0x37b2e8
0037b380: ldr r3, [sp, #0x70]
0037b384: add r1, sp, #0x78
0037b388: add sl, sl, #4
0037b38c: str r3, [r1, #-0x60]!
0037b390: mov r0, sl
0037b394: bl #0x37a300
0037b398: cmp r0, sl
0037b39c: mov sb, r0
0037b3a0: beq #0x37b444
0037b3a4: ldr sl, [r0, #0x28]
0037b3a8: mov r2, #0
0037b3ac: mov r3, #0
0037b3b0: ldr r1, [sl]
0037b3b4: mov r0, sl
0037b3b8: mov lr, pc
0037b3bc: ldr pc, [r1, #0x20]
0037b3c0: cmp sl, #0
0037b3c4: beq #0x37b4d0
0037b3c8: add sb, sp, #0x24
0037b3cc: add r1, r7, #4
0037b3d0: mov r2, sl
0037b3d4: mov r0, sb
0037b3d8: bl #0x31acf4
0037b3dc: ldr r3, [sp, #0x28]
0037b3e0: cmp r3, #0
0037b3e4: beq #0x37b40c
0037b3e8: mov r0, sb
0037b3ec: bl #0x31a68c
0037b3f0: mov r5, #0
0037b3f4: b #0x37b30c
0037b3f8: ldr r1, [pc, #0x150]
0037b3fc: mov r0, r8
0037b400: add r1, pc, r1
0037b404: bl #0x379ef8
0037b408: b #0x37b2e8
0037b40c: add r7, sp, #0x44
0037b410: mov r0, sb
0037b414: bl #0x31a68c
0037b418: ldr r1, [sp, #0x70]
0037b41c: add r2, sp, #0x20
0037b420: mov r0, r7
0037b424: bl #0x3140ec
0037b428: add r0, sp, #8
0037b42c: mov r1, r5
0037b430: mov r2, r7
0037b434: bl #0x37a9e8
0037b438: mov r0, r7
0037b43c: bl #0x3139ac
0037b440: b #0x37b308
0037b444: ldr r3, [pc, #0x108]
0037b448: mov r2, #0
0037b44c: ldr r1, [sp, #0x70]
0037b450: ldr fp, [r4, r3]
0037b454: mov r3, r2
0037b458: ldr r0, [fp, #0x10]
0037b45c: ldr ip, [r0, #0x34]
0037b460: mov r0, ip
0037b464: ldr ip, [ip]
0037b468: mov lr, pc
0037b46c: ldr pc, [ip, #0x88]
0037b470: cmp r0, #0
0037b474: str r0, [sp, #0x14]
0037b478: moveq r5, r0
0037b47c: beq #0x37b30c
0037b480: mov r1, #0
0037b484: mov r0, #0x30
0037b488: bl #0x310570
0037b48c: ldr r1, [sp, #0x14]
0037b490: mov sl, r0
0037b494: bl #0x3172d8
0037b498: ldr r3, [sp, #0x70]
0037b49c: add r1, sp, #0x78
0037b4a0: mov r0, sb
0037b4a4: str r3, [r1, #-0x68]!
0037b4a8: bl #0x37b0fc
0037b4ac: str sl, [r0]
0037b4b0: ldr r3, [fp, #0x10]
0037b4b4: add r1, sp, #0x14
0037b4b8: ldr r3, [r3, #0x34]
0037b4bc: mov r0, r3
0037b4c0: ldr r3, [r3]
0037b4c4: mov lr, pc
0037b4c8: ldr pc, [r3, #0x78]
0037b4cc: b #0x37b3c0
0037b4d0: ldr r3, [pc, #0x60]
0037b4d4: ldr r3, [r4, r3]
0037b4d8: ldr r3, [r3]
0037b4dc: cmp r3, #2
0037b4e0: streq sl, [sl]
0037b4e4: beq #0x37b3c8
0037b4e8: cmp r3, #1
0037b4ec: bne #0x37b3c8
0037b4f0: ldr r0, [pc, #0x44]
0037b4f4: ldr r1, [pc, #0x5c]
0037b4f8: ldr r2, [pc, #0x5c]
0037b4fc: ldr r0, [r4, r0]
0037b500: ldr r3, [pc, #0x58]
0037b504: mov ip, #0x61
0037b508: add r1, pc, r1
0037b50c: add r2, pc, r2
0037b510: add r3, pc, r3
0037b514: add r0, r0, #0xa8
0037b518: str ip, [sp]
0037b51c: bl #0x30e004
0037b520: b #0x37b3c8
0037b524: bl #0x30e310
0037b528: rsbeq sb, r1, r4, asr #16
0037b52c: andeq r4, r0, ip, lsr #1
0037b530: subseq r6, r4, r0, ror r7
0037b534: subseq r6, r4, r0, ror #14
0037b538: andeq r3, r0, r0, asr #19
0037b53c: andeq r1, r0, r0, asr #19
0037b540: subseq r3, r4, r8, lsl #1
0037b544: subseq r6, r4, r4, lsl #13
0037b548: subseq r6, r4, r8, lsl #13
0037b54c: subseq r6, r7, r4, lsl sp
0037b550: subseq r6, r4, r8, lsr r6
0037b554: strdeq r3, r4, [r0], -r4
0037b558: ldrsbeq r2, [r4], #-0xe0
0037b55c: subseq r6, r4, r4, lsr r5
0037b560: ldrsbeq r6, [r4], #-0x40

_ZN10GameObject12_GetPositionERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38e700 52
0038e700: push {r4, r5, r6, lr}
0038e704: mov r0, r1
0038e708: mov r4, r2
0038e70c: mov r5, r1
0038e710: ldr r1, [r2, #0x160]
0038e714: bl #0x37ccbc
0038e718: mov r0, r5
0038e71c: ldr r1, [r4, #0x164]
0038e720: bl #0x37ccbc
0038e724: ldr r1, [r4, #0x168]
0038e728: mov r0, r5
0038e72c: pop {r4, r5, r6, lr}
0038e730: b #0x37ccbc

_ZNSt5dequeIPSt6vectorIN3sfc6script3lua5ValueESaIS4_EESaIS7_EE18_M_push_back_aux_vERKS7_ 0x31cd00 388
0031cd00: push {r4, r5, r6, r7, r8, sb, sl, lr}
0031cd04: ldr sl, [r0, #0x1c]
0031cd08: ldr r2, [r0, #0x20]
0031cd0c: ldr r3, [r0, #0x24]
0031cd10: mov r5, r1
0031cd14: rsb r1, r2, sl
0031cd18: sub r1, r3, r1, asr #2
0031cd1c: cmp r1, #1
0031cd20: mov r4, r0
0031cd24: bls #0x31cd64
0031cd28: add r0, r4, #0x24
0031cd2c: bl #0x31c1c4
0031cd30: str r0, [sl, #4]
0031cd34: ldr r2, [r5]
0031cd38: ldr r3, [r4, #0x10]
0031cd3c: str r2, [r3]
0031cd40: ldr r3, [r4, #0x1c]
0031cd44: add r2, r3, #4
0031cd48: str r2, [r4, #0x1c]
0031cd4c: ldr r3, [r3, #4]
0031cd50: add r2, r3, #0x80
0031cd54: str r3, [r4, #0x10]
0031cd58: str r2, [r4, #0x18]
0031cd5c: str r3, [r4, #0x14]
0031cd60: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0031cd64: ldr r1, [r0, #0xc]
0031cd68: rsb r7, r1, sl
0031cd6c: asr r7, r7, #2
0031cd70: add r7, r7, #1
0031cd74: add sb, r7, #1
0031cd78: cmp r3, sb, lsl #1
0031cd7c: bls #0x31cdac
0031cd80: rsb r6, sb, r3
0031cd84: lsr r6, r6, #1
0031cd88: add r6, r2, r6, lsl #2
0031cd8c: cmp r1, r6
0031cd90: bls #0x31ce50
0031cd94: add r2, sl, #4
0031cd98: subs r2, r2, r1
0031cd9c: beq #0x31ce1c
0031cda0: mov r0, r6
0031cda4: bl #0x30df38
0031cda8: b #0x31ce1c
0031cdac: cmp r3, #0
0031cdb0: movne r2, r3
0031cdb4: moveq r2, #1
0031cdb8: add r8, r3, #2
0031cdbc: add r8, r8, r2
0031cdc0: mov r1, r8
0031cdc4: mov r2, #0
0031cdc8: add r0, r0, #0x20
0031cdcc: bl #0x31bd80
0031cdd0: ldr r2, [r4, #0x1c]
0031cdd4: ldr r1, [r4, #0xc]
0031cdd8: rsb r6, sb, r8
0031cddc: lsr r6, r6, #1
0031cde0: add r2, r2, #4
0031cde4: subs r2, r2, r1
0031cde8: mov sl, r0
0031cdec: add r6, r0, r6, lsl #2
0031cdf0: bne #0x31ce78
0031cdf4: ldr r0, [r4, #0x20]
0031cdf8: ldr r1, [r4, #0x24]
0031cdfc: cmp r0, #0
0031ce00: beq #0x31ce14
0031ce04: lsl r1, r1, #2
0031ce08: cmp r1, #0x80
0031ce0c: bhi #0x31ce70
0031ce10: bl #0x708f00
0031ce14: str sl, [r4, #0x20]
0031ce18: str r8, [r4, #0x24]
0031ce1c: str r6, [r4, #0xc]
0031ce20: ldr r3, [r6]
0031ce24: sub r7, r7, #1
0031ce28: add sl, r6, r7, lsl #2
0031ce2c: add r2, r3, #0x80
0031ce30: str r2, [r4, #8]
0031ce34: str r3, [r4, #4]
0031ce38: str sl, [r4, #0x1c]
0031ce3c: ldr r3, [r6, r7, lsl #2]
0031ce40: add r2, r3, #0x80
0031ce44: str r2, [r4, #0x18]
0031ce48: str r3, [r4, #0x14]
0031ce4c: b #0x31cd28
0031ce50: add r2, sl, #4
0031ce54: rsb r2, r1, r2
0031ce58: cmp r2, #0
0031ce5c: ble #0x31ce1c
0031ce60: add r0, r6, r7, lsl #2
0031ce64: rsb r0, r2, r0
0031ce68: bl #0x30df38
0031ce6c: b #0x31ce1c
0031ce70: bl #0x310440
0031ce74: b #0x31ce14
0031ce78: mov r0, r6
0031ce7c: bl #0x30df38
0031ce80: b #0x31cdf4

_ZN9LuaScript7_BitNotERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f814 100
0037f814: push {r4, lr}
0037f818: ldr r3, [r0, #4]
0037f81c: mov r4, r1
0037f820: ldm r3, {r1, r2}
0037f824: rsb r3, r1, r2
0037f828: asr r3, r3, #4
0037f82c: add r2, r3, r3, lsl #3
0037f830: add r2, r2, r2, lsl #6
0037f834: add r2, r3, r2, lsl #3
0037f838: add r2, r2, r2, lsl #15
0037f83c: add r3, r3, r2, lsl #3
0037f840: cmn r3, #1
0037f844: beq #0x37f84c
0037f848: pop {r4, pc}
0037f84c: ldr r3, [r1, #4]
0037f850: cmp r3, #3
0037f854: bne #0x37f848
0037f858: mov r1, #0
0037f85c: bl #0x37baf8
0037f860: bl #0x31bbf0
0037f864: bl #0x30e4cc
0037f868: mvn r1, r0
0037f86c: mov r0, r4
0037f870: pop {r4, lr}
0037f874: b #0x37cb24

_ZN9LuaScript12BindFunctionEv 0x37b5a0 1252
0037b5a0: push {r4, r5, r6, lr}
0037b5a4: add r4, r0, #4
0037b5a8: mov r5, r0
0037b5ac: mov r0, r4
0037b5b0: bl #0x31b010
0037b5b4: mov r0, r4
0037b5b8: bl #0x31b000
0037b5bc: mov r0, r4
0037b5c0: bl #0x31aff8
0037b5c4: mov r0, r4
0037b5c8: ldr r4, [pc, #0x3a8]
0037b5cc: bl #0x31b008
0037b5d0: ldr r3, [pc, #0x3a4]
0037b5d4: ldr r1, [pc, #0x3a4]
0037b5d8: add r4, pc, r4
0037b5dc: add r6, r5, #0x10
0037b5e0: ldr r2, [r4, r3]
0037b5e4: mov r0, r6
0037b5e8: mov r3, r5
0037b5ec: add r1, pc, r1
0037b5f0: bl #0x31a4d4
0037b5f4: ldr r3, [pc, #0x388]
0037b5f8: ldr r1, [pc, #0x388]
0037b5fc: mov r0, r6
0037b600: ldr r2, [r4, r3]
0037b604: add r1, pc, r1
0037b608: mov r3, r5
0037b60c: bl #0x31a4d4
0037b610: ldr r3, [pc, #0x374]
0037b614: ldr r1, [pc, #0x374]
0037b618: mov r0, r6
0037b61c: ldr r2, [r4, r3]
0037b620: add r1, pc, r1
0037b624: mov r3, r5
0037b628: bl #0x31a4d4
0037b62c: ldr r3, [pc, #0x360]
0037b630: ldr r1, [pc, #0x360]
0037b634: mov r0, r6
0037b638: ldr r2, [r4, r3]
0037b63c: add r1, pc, r1
0037b640: mov r3, r5
0037b644: bl #0x31a4d4
0037b648: ldr r3, [pc, #0x34c]
0037b64c: ldr r1, [pc, #0x34c]
0037b650: mov r0, r6
0037b654: ldr r2, [r4, r3]
0037b658: add r1, pc, r1
0037b65c: mov r3, r5
0037b660: bl #0x31a4d4
0037b664: ldr r3, [pc, #0x338]
0037b668: ldr r1, [pc, #0x338]
0037b66c: mov r0, r6
0037b670: ldr r2, [r4, r3]
0037b674: add r1, pc, r1
0037b678: mov r3, r5
0037b67c: bl #0x31a4d4
0037b680: ldr r3, [pc, #0x324]
0037b684: ldr r1, [pc, #0x324]
0037b688: mov r0, r6
0037b68c: ldr r2, [r4, r3]
0037b690: add r1, pc, r1
0037b694: mov r3, r5
0037b698: bl #0x31a4d4
0037b69c: ldr r3, [pc, #0x310]
0037b6a0: ldr r1, [pc, #0x310]
0037b6a4: mov r0, r6
0037b6a8: ldr r2, [r4, r3]
0037b6ac: add r1, pc, r1
0037b6b0: mov r3, r5
0037b6b4: bl #0x31a4d4
0037b6b8: ldr r3, [pc, #0x2fc]
0037b6bc: ldr r1, [pc, #0x2fc]
0037b6c0: mov r0, r6
0037b6c4: ldr r2, [r4, r3]
0037b6c8: add r1, pc, r1
0037b6cc: mov r3, r5
0037b6d0: bl #0x31a4d4
0037b6d4: ldr r3, [pc, #0x2e8]
0037b6d8: ldr r1, [pc, #0x2e8]
0037b6dc: mov r0, r6
0037b6e0: ldr r2, [r4, r3]
0037b6e4: add r1, pc, r1
0037b6e8: mov r3, r5
0037b6ec: bl #0x31a4d4
0037b6f0: ldr r3, [pc, #0x2d4]
0037b6f4: ldr r1, [pc, #0x2d4]
0037b6f8: mov r0, r6
0037b6fc: ldr r2, [r4, r3]
0037b700: add r1, pc, r1
0037b704: mov r3, r5
0037b708: bl #0x31a4d4
0037b70c: ldr r3, [pc, #0x2c0]
0037b710: ldr r1, [pc, #0x2c0]
0037b714: mov r0, r6
0037b718: ldr r2, [r4, r3]
0037b71c: add r1, pc, r1
0037b720: mov r3, r5
0037b724: bl #0x31a4d4
0037b728: ldr r3, [pc, #0x2ac]
0037b72c: ldr r1, [pc, #0x2ac]
0037b730: mov r0, r6
0037b734: ldr r2, [r4, r3]
0037b738: add r1, pc, r1
0037b73c: mov r3, r5
0037b740: bl #0x31a4d4
0037b744: ldr r3, [pc, #0x298]
0037b748: ldr r1, [pc, #0x298]
0037b74c: mov r0, r6
0037b750: ldr r2, [r4, r3]
0037b754: add r1, pc, r1
0037b758: mov r3, r5
0037b75c: bl #0x31a4d4
0037b760: ldr r3, [pc, #0x284]
0037b764: ldr r1, [pc, #0x284]
0037b768: mov r0, r6
0037b76c: ldr r2, [r4, r3]
0037b770: add r1, pc, r1
0037b774: mov r3, r5
0037b778: bl #0x31a4d4
0037b77c: ldr r3, [pc, #0x270]
0037b780: ldr r1, [pc, #0x270]
0037b784: mov r0, r6
0037b788: ldr r2, [r4, r3]
0037b78c: add r1, pc, r1
0037b790: mov r3, r5
0037b794: bl #0x31a4d4
0037b798: ldr r3, [pc, #0x25c]
0037b79c: ldr r1, [pc, #0x25c]
0037b7a0: mov r0, r6
0037b7a4: ldr r2, [r4, r3]
0037b7a8: add r1, pc, r1
0037b7ac: mov r3, r5
0037b7b0: bl #0x31a4d4
0037b7b4: ldr r3, [pc, #0x248]
0037b7b8: ldr r1, [pc, #0x248]
0037b7bc: mov r0, r6
0037b7c0: ldr r2, [r4, r3]
0037b7c4: add r1, pc, r1
0037b7c8: mov r3, r5
0037b7cc: bl #0x31a4d4
0037b7d0: ldr r3, [pc, #0x234]
0037b7d4: ldr r1, [pc, #0x234]
0037b7d8: mov r0, r6
0037b7dc: ldr r2, [r4, r3]
0037b7e0: add r1, pc, r1
0037b7e4: mov r3, r5
0037b7e8: bl #0x31a4d4
0037b7ec: ldr r3, [pc, #0x220]
0037b7f0: ldr r1, [pc, #0x220]
0037b7f4: mov r0, r6
0037b7f8: ldr r2, [r4, r3]
0037b7fc: add r1, pc, r1
0037b800: mov r3, r5
0037b804: bl #0x31a4d4
0037b808: ldr r3, [pc, #0x20c]
0037b80c: ldr r1, [pc, #0x20c]
0037b810: mov r0, r6
0037b814: ldr r2, [r4, r3]
0037b818: add r1, pc, r1
0037b81c: mov r3, r5
0037b820: bl #0x31a4d4
0037b824: ldr r3, [pc, #0x1f8]
0037b828: ldr r1, [pc, #0x1f8]
0037b82c: mov r0, r6
0037b830: ldr r2, [r4, r3]
0037b834: add r1, pc, r1
0037b838: mov r3, r5
0037b83c: bl #0x31a4d4
0037b840: ldr r3, [pc, #0x1e4]
0037b844: ldr r1, [pc, #0x1e4]
0037b848: mov r0, r6
0037b84c: ldr r2, [r4, r3]
0037b850: add r1, pc, r1
0037b854: mov r3, r5
0037b858: bl #0x31a4d4
0037b85c: ldr r3, [pc, #0x1d0]
0037b860: ldr r1, [pc, #0x1d0]
0037b864: mov r0, r6
0037b868: ldr r2, [r4, r3]
0037b86c: add r1, pc, r1
0037b870: mov r3, r5
0037b874: bl #0x31a4d4
0037b878: ldr r3, [pc, #0x1bc]
0037b87c: ldr r1, [pc, #0x1bc]
0037b880: mov r0, r6
0037b884: ldr r2, [r4, r3]
0037b888: add r1, pc, r1
0037b88c: mov r3, r5
0037b890: bl #0x31a4d4
0037b894: ldr r3, [pc, #0x1a8]
0037b898: ldr r1, [pc, #0x1a8]
0037b89c: mov r0, r6
0037b8a0: ldr r2, [r4, r3]
0037b8a4: add r1, pc, r1
0037b8a8: mov r3, r5
0037b8ac: bl #0x31a4d4
0037b8b0: ldr r3, [pc, #0x194]
0037b8b4: ldr r1, [pc, #0x194]
0037b8b8: mov r0, r6
0037b8bc: ldr r2, [r4, r3]
0037b8c0: add r1, pc, r1
0037b8c4: mov r3, r5
0037b8c8: bl #0x31a4d4
0037b8cc: ldr r3, [pc, #0x180]
0037b8d0: ldr r1, [pc, #0x180]
0037b8d4: mov r0, r6
0037b8d8: ldr r2, [r4, r3]
0037b8dc: add r1, pc, r1
0037b8e0: mov r3, r5
0037b8e4: bl #0x31a4d4
0037b8e8: ldr r3, [pc, #0x16c]
0037b8ec: ldr r1, [pc, #0x16c]
0037b8f0: mov r0, r6
0037b8f4: ldr r2, [r4, r3]
0037b8f8: add r1, pc, r1
0037b8fc: mov r3, r5
0037b900: bl #0x31a4d4
0037b904: ldr r3, [pc, #0x158]
0037b908: ldr r1, [pc, #0x158]
0037b90c: mov r0, r6
0037b910: ldr r2, [r4, r3]
0037b914: add r1, pc, r1
0037b918: mov r3, r5
0037b91c: bl #0x31a4d4
0037b920: ldr r3, [pc, #0x144]
0037b924: ldr r1, [pc, #0x144]
0037b928: mov r0, r6
0037b92c: ldr r2, [r4, r3]
0037b930: add r1, pc, r1
0037b934: mov r3, r5
0037b938: bl #0x31a4d4
0037b93c: ldr r3, [pc, #0x130]
0037b940: ldr r1, [pc, #0x130]
0037b944: mov r0, r6
0037b948: ldr r2, [r4, r3]
0037b94c: add r1, pc, r1
0037b950: mov r3, r5
0037b954: bl #0x31a4d4
0037b958: ldr r3, [pc, #0x11c]
0037b95c: ldr r1, [pc, #0x11c]
0037b960: mov r0, r6
0037b964: ldr r2, [r4, r3]
0037b968: add r1, pc, r1
0037b96c: mov r3, r5
0037b970: pop {r4, r5, r6, lr}
0037b974: b #0x31a4d4
0037b978: strhteq sb, [r1], #-0x48
0037b97c: andeq r3, r0, r0, asr ip
0037b980: subseq r6, r4, ip, asr r4
0037b984: andeq r3, r0, r0, asr #20
0037b988: subseq r6, r4, ip, asr #8
0037b98c: andeq r1, r0, r4, lsr r7
0037b990: subseq r6, r4, r8, lsr r4
0037b994: andeq r0, r0, r0, ror #29
0037b998: subseq r6, r4, r4, lsr #8
0037b99c: muleq r0, ip, fp
0037b9a0: subseq r6, r4, r0, lsl r4
0037b9a4: muleq r0, r8, ip
0037b9a8: subseq r6, r4, r4, lsl #8
0037b9ac: muleq r0, ip, r5
0037b9b0: ldrsheq r6, [r4], #-0x38
0037b9b4: andeq r3, r0, r8, ror #15
0037b9b8: subseq r6, r4, ip, ror #7
0037b9bc: andeq r4, r0, r4, asr #23
0037b9c0: ldrsbeq r6, [r4], #-0x38
0037b9c4: muleq r0, r0, r8
0037b9c8: subseq r6, r4, ip, asr #7
0037b9cc: andeq r1, r0, r4, ror #7
0037b9d0: subseq r6, r4, r0, asr #7
0037b9d4: strdeq r4, r5, [r0], -r4
0037b9d8: ldrheq r6, [r4], #-0x34
0037b9dc: andeq r4, r0, r0, lsl #6
0037b9e0: subseq r6, r4, r0, lsr #7
0037b9e4: andeq r3, r0, ip, asr #10
0037b9e8: subseq r6, r4, ip, lsl #7
0037b9ec: andeq r0, r0, r8, lsr r7
0037b9f0: subseq r6, r4, r8, ror r3
0037b9f4: andeq r4, r0, r4, lsr r6
0037b9f8: subseq r6, r4, r4, ror #6
0037b9fc: muleq r0, r0, r0
0037ba00: subseq r6, r4, r0, asr r3
0037ba04: andeq r1, r0, ip, lsl #13
0037ba08: subseq r6, r4, ip, lsr r3
0037ba0c: andeq r1, r0, ip, lsl #9
0037ba10: subseq r6, r4, r0, lsr r3
0037ba14: andeq r2, r0, ip, lsr #29
0037ba18: subseq r6, r4, r4, lsr #6
0037ba1c: andeq r2, r0, ip, lsl #5
0037ba20: subseq r6, r4, r8, lsl r3
0037ba24: ldrdeq r4, r5, [r0], -ip
0037ba28: subseq r6, r4, ip, lsl #6
0037ba2c: muleq r0, r8, pc
0037ba30: subseq r6, r4, r0, lsl #6
0037ba34: andeq r1, r0, r0, asr #21
0037ba38: ldrsheq r6, [r4], #-0x24
0037ba3c: andeq r3, r0, r8, lsr #11
0037ba40: ldrsheq r6, [r4], #-0x20
0037ba44: andeq r4, r0, r0, asr r0
0037ba48: subseq r6, r4, ip, ror #5
0037ba4c: strdeq r3, r4, [r0], -r4
0037ba50: subseq r6, r4, r8, ror #5
0037ba54: andeq r2, r0, ip, lsl #8
0037ba58: subseq r6, r4, r4, ror #5
0037ba5c: andeq r1, r0, ip, ror #15
0037ba60: ldrsbeq r6, [r4], #-0x28
0037ba64: muleq r0, r8, sb
0037ba68: subseq r6, r4, ip, asr #5
0037ba6c: andeq r3, r0, r8, ror pc
0037ba70: subseq r6, r4, r0, asr #5
0037ba74: andeq r2, r0, ip, lsr #13
0037ba78: ldrheq r6, [r4], #-0x24
0037ba7c: andeq r2, r0, r8, lsl #30
0037ba80: subseq r6, r4, r8, lsr #5

_ZN9LuaScript24_GetHostPlayerDifficultyERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37cb8c 76
0037cb8c: ldr r3, [pc, #0x3c]
0037cb90: ldr r2, [pc, #0x3c]
0037cb94: push {r4, lr}
0037cb98: add r3, pc, r3
0037cb9c: ldr r0, [r3, r2]
0037cba0: mov r4, r1
0037cba4: bl #0x31f594
0037cba8: subs r3, r0, #0
0037cbac: beq #0x37cbc0
0037cbb0: ldr r1, [r3, #0x118]
0037cbb4: mov r0, r4
0037cbb8: pop {r4, lr}
0037cbbc: b #0x37cb24
0037cbc0: mov r0, r4
0037cbc4: mov r1, r3
0037cbc8: pop {r4, lr}
0037cbcc: b #0x37cb24

_ZN10GameObject7_PlayFXERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3923cc 596
003923cc: push {r4, r5, r6, lr}
003923d0: ldr r3, [r0, #4]
003923d4: ldr r4, [pc, #0x230]
003923d8: sub sp, sp, #0x20
003923dc: ldr r1, [r3, #4]
003923e0: ldr ip, [r3]
003923e4: add r4, pc, r4
003923e8: mov r5, r0
003923ec: rsb r3, ip, r1
003923f0: asr r3, r3, #4
003923f4: add r1, r3, r3, lsl #3
003923f8: add r1, r1, r1, lsl #6
003923fc: add r1, r3, r1, lsl #3
00392400: add r1, r1, r1, lsl #15
00392404: add r3, r3, r1, lsl #3
00392408: cmp r3, #0
0039240c: bne #0x392418
00392410: add sp, sp, #0x20
00392414: pop {r4, r5, r6, pc}
00392418: ldr r3, [ip, #4]
0039241c: cmp r3, #3
00392420: bne #0x392410
00392424: mov r1, #0
00392428: str r2, [sp, #0xc]
0039242c: bl #0x37baf8
00392430: bl #0x38d798
00392434: ldr r3, [pc, #0x1d4]
00392438: ldr r2, [sp, #0xc]
0039243c: ldr r3, [r4, r3]
00392440: ldr r3, [r3]
00392444: cmp r0, r3
00392448: bhs #0x392410
0039244c: ldr r6, [r5, #4]
00392450: ldr r1, [r6, #4]
00392454: ldr r3, [r6]
00392458: rsb r3, r3, r1
0039245c: asr r3, r3, #4
00392460: add r1, r3, r3, lsl #3
00392464: add r1, r1, r1, lsl #6
00392468: add r1, r3, r1, lsl #3
0039246c: add r1, r1, r1, lsl #15
00392470: add r3, r3, r1, lsl #3
00392474: rsb r3, r3, #0
00392478: cmp r3, #3
0039247c: bls #0x392564
00392480: mov r3, #0
00392484: str r3, [sp, #0x1c]
00392488: str r3, [sp, #0x14]
0039248c: str r3, [sp, #0x18]
00392490: ldm r6, {r1, r3}
00392494: rsb r3, r1, r3
00392498: asr r3, r3, #4
0039249c: add r0, r3, r3, lsl #3
003924a0: add r0, r0, r0, lsl #6
003924a4: add r0, r3, r0, lsl #3
003924a8: add r0, r0, r0, lsl #15
003924ac: add r3, r3, r0, lsl #3
003924b0: rsb r3, r3, #0
003924b4: cmp r3, #1
003924b8: bhi #0x3924d4
003924bc: ldr r0, [pc, #0x150]
003924c0: str r2, [sp, #0xc]
003924c4: add r0, pc, r0
003924c8: bl #0x708eb0
003924cc: ldr r1, [r6]
003924d0: ldr r2, [sp, #0xc]
003924d4: ldr r3, [r1, #0x74]
003924d8: cmp r3, #3
003924dc: beq #0x392594
003924e0: ldr r0, [r2, #0x168]
003924e4: ldr r1, [r2, #0x160]
003924e8: ldr r3, [r2, #0x164]
003924ec: str r0, [sp, #0x1c]
003924f0: str r1, [sp, #0x14]
003924f4: str r3, [sp, #0x18]
003924f8: ldr r5, [r5, #4]
003924fc: ldm r5, {r0, r3}
00392500: rsb r3, r0, r3
00392504: asr r3, r3, #4
00392508: add r2, r3, r3, lsl #3
0039250c: add r2, r2, r2, lsl #6
00392510: add r2, r3, r2, lsl #3
00392514: add r2, r2, r2, lsl #15
00392518: add r3, r3, r2, lsl #3
0039251c: cmp r3, #0
00392520: bne #0x392534
00392524: ldr r0, [pc, #0xec]
00392528: add r0, pc, r0
0039252c: bl #0x708eb0
00392530: ldr r0, [r5]
00392534: bl #0x31bbf0
00392538: ldr r3, [pc, #0xdc]
0039253c: ldr r4, [r4, r3]
00392540: bl #0x8be2a0
00392544: mov ip, #0
00392548: mov r1, r0
0039254c: mov r3, ip
00392550: mov r0, r4
00392554: add r2, sp, #0x14
00392558: str ip, [sp]
0039255c: bl #0x495d14
00392560: b #0x392410
00392564: mov r1, #0
00392568: mov r0, r5
0039256c: str r2, [sp, #0xc]
00392570: bl #0x37baf8
00392574: bl #0x38d798
00392578: ldr r3, [pc, #0x9c]
0039257c: mov r1, r0
00392580: ldr r2, [sp, #0xc]
00392584: ldr r0, [r4, r3]
00392588: mov r3, #0
0039258c: bl #0x495f04
00392590: b #0x392410
00392594: mov r1, #2
00392598: mov r0, r5
0039259c: str r2, [sp, #0xc]
003925a0: bl #0x37baf8
003925a4: ldr r1, [r0, #4]
003925a8: ldr r2, [sp, #0xc]
003925ac: cmp r1, #3
003925b0: bne #0x3924e0
003925b4: mov r0, r5
003925b8: bl #0x37baf8
003925bc: ldr r6, [r0, #4]
003925c0: ldr r2, [sp, #0xc]
003925c4: cmp r6, #3
003925c8: bne #0x3924e0
003925cc: mov r1, #1
003925d0: mov r0, r5
003925d4: bl #0x37baf8
003925d8: bl #0x31bbf0
003925dc: mov r1, #2
003925e0: str r0, [sp, #0x14]
003925e4: mov r0, r5
003925e8: bl #0x37baf8
003925ec: bl #0x31bbf0
003925f0: mov r1, r6
003925f4: str r0, [sp, #0x18]
003925f8: mov r0, r5
003925fc: bl #0x37baf8
00392600: bl #0x31bbf0
00392604: str r0, [sp, #0x1c]
00392608: b #0x3924f8
0039260c: rsbeq r2, r0, ip, lsr #13
00392610: andeq r0, r0, r4, asr #13
00392614: subseq fp, r2, r4, lsr #31
00392618: subseq fp, r2, r0, asr #30
0039261c: andeq r1, r0, r8, lsl #22

_ZN3sfc6script3lua9ArgumentsD1Ev 0x319228 56
00319228: ldr r3, [pc, #0x28]
0031922c: ldr r2, [pc, #0x28]
00319230: push {r4, lr}
00319234: add r3, pc, r3
00319238: ldr r2, [r3, r2]
0031923c: mov r4, r0
00319240: ldr r0, [r0, #4]
00319244: add r2, r2, #8
00319248: str r2, [r4]
0031924c: bl #0x31d194
00319250: mov r0, r4
00319254: pop {r4, pc}
00319258: rsbeq fp, r7, ip, asr r8
0031925c: andeq r2, r0, r0, ror #6

_ZN10GameObject13_MarkAsFlyingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390900 116
00390900: str lr, [sp, #-4]!
00390904: ldr r3, [r0, #4]
00390908: sub sp, sp, #0xc
0039090c: ldr r1, [r3, #4]
00390910: ldr ip, [r3]
00390914: rsb r3, ip, r1
00390918: asr r3, r3, #4
0039091c: add r1, r3, r3, lsl #3
00390920: add r1, r1, r1, lsl #6
00390924: add r1, r3, r1, lsl #3
00390928: add r1, r1, r1, lsl #15
0039092c: add r3, r3, r1, lsl #3
00390930: cmp r3, #0
00390934: bne #0x390940
00390938: add sp, sp, #0xc
0039093c: ldm sp!, {pc}
00390940: ldr r3, [ip, #4]
00390944: cmp r3, #1
00390948: bne #0x390938
0039094c: mov r1, #0
00390950: str r2, [sp, #4]
00390954: bl #0x37baf8
00390958: bl #0x31bc80
0039095c: ldr r2, [sp, #4]
00390960: mov r1, r0
00390964: add r0, r2, #0x1c8
00390968: add sp, sp, #0xc
0039096c: pop {lr}
00390970: b #0x5241f4

_ZN3sfc6script3lua8InstanceC2Ev 0x31b2a8 64
0031b2a8: ldr r3, [pc, #0x30]
0031b2ac: ldr r2, [pc, #0x30]
0031b2b0: mov r1, #1
0031b2b4: add r3, pc, r3
0031b2b8: ldr r2, [r3, r2]
0031b2bc: push {r4, lr}
0031b2c0: add r2, r2, #8
0031b2c4: strb r1, [r0, #8]
0031b2c8: str r2, [r0]
0031b2cc: mov r4, r0
0031b2d0: bl #0x31b224
0031b2d4: str r0, [r4, #4]
0031b2d8: mov r0, r4
0031b2dc: pop {r4, pc}

_ZN10LuaManager18FlushBufferedFilesEv 0x379fe8 196
00379fe8: push {r4, r5, r6, lr}
00379fec: ldr r6, [r0, #0xc]
00379ff0: mov r5, r0
00379ff4: add r4, r0, #4
00379ff8: cmp r4, r6
00379ffc: beq #0x37a048
0037a000: ldr r3, [r6, #0x28]
0037a004: cmp r3, #0
0037a008: beq #0x37a01c
0037a00c: mov r0, r3
0037a010: ldr r3, [r3]
0037a014: mov lr, pc
0037a018: ldr pc, [r3, #4]
0037a01c: ldr r2, [r6, #0xc]
0037a020: cmp r2, #0
0037a024: bne #0x37a030
0037a028: b #0x37a078
0037a02c: mov r2, r3
0037a030: ldr r3, [r2, #8]
0037a034: cmp r3, #0
0037a038: bne #0x37a02c
0037a03c: mov r6, r2
0037a040: cmp r4, r6
0037a044: bne #0x37a000
0037a048: ldr r3, [r5, #0x14]
0037a04c: cmp r3, #0
0037a050: beq #0x37a074
0037a054: mov r0, r4
0037a058: ldr r1, [r5, #8]
0037a05c: bl #0x379fa8
0037a060: mov r3, #0
0037a064: str r3, [r5, #0x14]
0037a068: str r4, [r5, #0x10]
0037a06c: str r4, [r5, #0xc]
0037a070: str r3, [r5, #8]
0037a074: pop {r4, r5, r6, pc}
0037a078: ldr r3, [r6, #4]
0037a07c: ldr r1, [r3, #0xc]
0037a080: cmp r6, r1
0037a084: bne #0x37a0a0
0037a088: mov r6, r3
0037a08c: ldr r3, [r3, #4]
0037a090: ldr r2, [r3, #0xc]
0037a094: cmp r2, r6
0037a098: beq #0x37a088
0037a09c: ldr r2, [r6, #0xc]
0037a0a0: cmp r2, r3
0037a0a4: movne r6, r3
0037a0a8: b #0x379ff8

_ZN9Character10_GetCharIDERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6d5c 16
003b6d5c: movw r3, #0x13c8
003b6d60: mov r0, r1
003b6d64: ldrsh r1, [r2, r3]
003b6d68: b #0x37cb24

_ZN9LuaScript8_ToFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ebc4 80
0037ebc4: push {r4, lr}
0037ebc8: ldr r3, [r0, #4]
0037ebcc: mov r4, r1
0037ebd0: ldm r3, {r0, r2}
0037ebd4: rsb r3, r0, r2
0037ebd8: asr r3, r3, #4
0037ebdc: add r2, r3, r3, lsl #3
0037ebe0: add r2, r2, r2, lsl #6
0037ebe4: add r2, r3, r2, lsl #3
0037ebe8: add r2, r2, r2, lsl #15
0037ebec: add r3, r3, r2, lsl #3
0037ebf0: cmp r3, #0
0037ebf4: bne #0x37ebfc
0037ebf8: pop {r4, pc}
0037ebfc: bl #0x31bbf0
0037ec00: bl #0x30e4cc
0037ec04: lsl r1, r0, #8
0037ec08: mov r0, r4
0037ec0c: pop {r4, lr}
0037ec10: b #0x37cb24

_ZN9Character13_SetStunStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b9580 304
003b9580: push {r4, r5, r6, lr}
003b9584: ldr r3, [r0, #4]
003b9588: sub sp, sp, #0x10
003b958c: mov r4, r0
003b9590: ldr r1, [r3, #4]
003b9594: ldr ip, [r3]
003b9598: rsb r3, ip, r1
003b959c: asr r3, r3, #4
003b95a0: add r1, r3, r3, lsl #3
003b95a4: add r1, r1, r1, lsl #6
003b95a8: add r1, r3, r1, lsl #3
003b95ac: add r1, r1, r1, lsl #15
003b95b0: add r3, r3, r1, lsl #3
003b95b4: rsb r3, r3, #0
003b95b8: cmp r3, #0
003b95bc: bne #0x3b95c8
003b95c0: add sp, sp, #0x10
003b95c4: pop {r4, r5, r6, pc}
003b95c8: ldr r1, [ip, #4]
003b95cc: cmp r1, #3
003b95d0: bne #0x3b95c0
003b95d4: cmp r3, #1
003b95d8: addls r5, r2, #0x4f0
003b95dc: addls r5, r5, #0xc
003b95e0: movls r4, #1
003b95e4: bls #0x3b9654
003b95e8: mov r1, #1
003b95ec: str r2, [sp, #0xc]
003b95f0: bl #0x37baf8
003b95f4: ldr r5, [r0, #4]
003b95f8: ldr r2, [sp, #0xc]
003b95fc: cmp r5, #1
003b9600: beq #0x3b9680
003b9604: ldr r6, [r4, #4]
003b9608: mov r4, #1
003b960c: ldr ip, [r6]
003b9610: ldr r3, [r6, #4]
003b9614: add r5, r2, #0x4f0
003b9618: add r5, r5, #0xc
003b961c: rsb r3, ip, r3
003b9620: asr r3, r3, #4
003b9624: add r1, r3, r3, lsl #3
003b9628: add r1, r1, r1, lsl #6
003b962c: add r1, r3, r1, lsl #3
003b9630: add r1, r1, r1, lsl #15
003b9634: add r3, r3, r1, lsl #3
003b9638: rsb r3, r3, #0
003b963c: cmp r3, #0
003b9640: bne #0x3b9654
003b9644: ldr r0, [pc, #0x60]
003b9648: add r0, pc, r0
003b964c: bl #0x708eb0
003b9650: ldr ip, [r6]
003b9654: mov r0, ip
003b9658: bl #0x31bbf0
003b965c: bl #0x8be2a0
003b9660: mov ip, #0
003b9664: mov r1, r0
003b9668: mov r2, r4
003b966c: mov r0, r5
003b9670: mov r3, ip
003b9674: str ip, [sp]
003b9678: bl #0x3c5ffc
003b967c: b #0x3b95c0
003b9680: mov r0, r4
003b9684: mov r1, r5
003b9688: str r2, [sp, #0xc]
003b968c: bl #0x37baf8
003b9690: bl #0x31bc80
003b9694: cmp r0, #0
003b9698: ldr r6, [r4, #4]
003b969c: ldr r2, [sp, #0xc]
003b96a0: movne r4, r5
003b96a4: moveq r4, r0
003b96a8: b #0x3b960c
003b96ac: subseq r4, r0, r0, lsr #28

_ZN9Character11_StartTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7590 240
003b7590: push {r4, r5, r6, lr}
003b7594: ldr r3, [r0, #4]
003b7598: mov r4, r1
003b759c: mov r6, r2
003b75a0: ldm r3, {r1, r2}
003b75a4: sub sp, sp, #8
003b75a8: mov r5, r0
003b75ac: rsb r3, r1, r2
003b75b0: asr r3, r3, #4
003b75b4: add r2, r3, r3, lsl #3
003b75b8: add r2, r2, r2, lsl #6
003b75bc: add r2, r3, r2, lsl #3
003b75c0: add r2, r2, r2, lsl #15
003b75c4: add r3, r3, r2, lsl #3
003b75c8: rsb r3, r3, #0
003b75cc: cmp r3, #0
003b75d0: bne #0x3b75dc
003b75d4: add sp, sp, #8
003b75d8: pop {r4, r5, r6, pc}
003b75dc: ldr r2, [r1, #4]
003b75e0: cmp r2, #3
003b75e4: bne #0x3b75d4
003b75e8: cmp r3, #1
003b75ec: bls #0x3b7604
003b75f0: mov r1, #1
003b75f4: bl #0x37baf8
003b75f8: bl #0x31bc80
003b75fc: cmp r0, #0
003b7600: bne #0x3b764c
003b7604: mov r1, #0
003b7608: mov r0, r5
003b760c: bl #0x37baf8
003b7610: bl #0x38d798
003b7614: mov ip, #0
003b7618: mov r1, r0
003b761c: mov r2, ip
003b7620: add r0, r6, #0x3b4
003b7624: mov r3, #0x35
003b7628: str ip, [sp]
003b762c: bl #0x3dbe24
003b7630: mov r1, r0
003b7634: cmn r1, #1
003b7638: beq #0x3b75d4
003b763c: mov r0, r4
003b7640: add sp, sp, #8
003b7644: pop {r4, r5, r6, lr}
003b7648: b #0x37cb24
003b764c: mov r1, #0
003b7650: mov r0, r5
003b7654: bl #0x37baf8
003b7658: bl #0x38d798
003b765c: mov ip, #0
003b7660: mov r1, r0
003b7664: mvn r2, #0
003b7668: add r0, r6, #0x3b4
003b766c: mov r3, #0x35
003b7670: str ip, [sp]
003b7674: bl #0x3dbe24
003b7678: mov r1, r0
003b767c: b #0x3b7634

_ZN3sfc6script3lua8InstanceD2Ev 0x31b1e0 68
0031b1e0: push {r4, lr}
0031b1e4: ldr r3, [pc, #0x30]
0031b1e8: ldr r2, [pc, #0x30]
0031b1ec: ldrb r1, [r0, #8]
0031b1f0: add r3, pc, r3
0031b1f4: ldr r2, [r3, r2]
0031b1f8: cmp r1, #0
0031b1fc: mov r4, r0
0031b200: add r2, r2, #8
0031b204: str r2, [r0]
0031b208: beq #0x31b214
0031b20c: ldr r0, [r0, #4]
0031b210: bl #0x85797c
0031b214: mov r0, r4
0031b218: pop {r4, pc}
0031b21c: rsbeq sb, r7, r0, lsr #17
0031b220: andeq r4, r0, r8, lsl r0

_ZN9LuaScriptD1Ev 0x37c004 324
0037c004: push {r4, r5, r6, lr}
0037c008: ldr r5, [pc, #0x12c]
0037c00c: ldr r3, [pc, #0x12c]
0037c010: ldr r2, [r0, #0x90]
0037c014: add r5, pc, r5
0037c018: ldr r3, [r5, r3]
0037c01c: cmp r2, #0
0037c020: mov r4, r0
0037c024: add r3, r3, #8
0037c028: str r3, [r0]
0037c02c: bne #0x37c10c
0037c030: add r3, r4, #0x68
0037c034: ldr r0, [r3, #0x14]
0037c038: cmp r0, r3
0037c03c: beq #0x37c05c
0037c040: cmp r0, #0
0037c044: beq #0x37c05c
0037c048: ldr r1, [r4, #0x68]
0037c04c: rsb r1, r0, r1
0037c050: cmp r1, #0x80
0037c054: bhi #0x37c134
0037c058: bl #0x708f00
0037c05c: ldr r3, [r4, #0x5c]
0037c060: cmp r3, #0
0037c064: beq #0x37c08c
0037c068: add r6, r4, #0x4c
0037c06c: mov r0, r6
0037c070: ldr r1, [r4, #0x50]
0037c074: bl #0x37bd7c
0037c078: mov r3, #0
0037c07c: str r6, [r4, #0x58]
0037c080: str r3, [r4, #0x5c]
0037c084: str r6, [r4, #0x54]
0037c088: str r3, [r4, #0x50]
0037c08c: ldr r3, [r4, #0x44]
0037c090: cmp r3, #0
0037c094: beq #0x37c0bc
0037c098: add r6, r4, #0x34
0037c09c: mov r0, r6
0037c0a0: ldr r1, [r4, #0x38]
0037c0a4: bl #0x37bd7c
0037c0a8: mov r3, #0
0037c0ac: str r6, [r4, #0x40]
0037c0b0: str r3, [r4, #0x44]
0037c0b4: str r6, [r4, #0x3c]
0037c0b8: str r3, [r4, #0x38]
0037c0bc: ldr r3, [r4, #0x2c]
0037c0c0: cmp r3, #0
0037c0c4: beq #0x37c0ec
0037c0c8: add r6, r4, #0x1c
0037c0cc: mov r0, r6
0037c0d0: ldr r1, [r4, #0x20]
0037c0d4: bl #0x37bcc0
0037c0d8: mov r3, #0
0037c0dc: str r6, [r4, #0x28]
0037c0e0: str r3, [r4, #0x2c]
0037c0e4: str r6, [r4, #0x24]
0037c0e8: str r3, [r4, #0x20]
0037c0ec: ldr r3, [pc, #0x50]
0037c0f0: add r0, r4, #4
0037c0f4: ldr r3, [r5, r3]
0037c0f8: add r3, r3, #8
0037c0fc: str r3, [r4, #0x10]
0037c100: bl #0x31b180
0037c104: mov r0, r4
0037c108: pop {r4, r5, r6, pc}
0037c10c: add r6, r0, #0x80
0037c110: mov r0, r6
0037c114: ldr r1, [r4, #0x84]
0037c118: bl #0x37bcf8
0037c11c: mov r3, #0
0037c120: str r6, [r4, #0x8c]
0037c124: str r3, [r4, #0x90]
0037c128: str r6, [r4, #0x88]
0037c12c: str r3, [r4, #0x84]
0037c130: b #0x37c030
0037c134: bl #0x310440
0037c138: b #0x37c05c
0037c13c: rsbeq r8, r1, ip, ror sl
0037c140: andeq r1, r0, r4, ror r6
0037c144: andeq r3, r0, r8, asr r6

_ZN9Character10_HasMasterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6f3c 20
003b6f3c: ldr r3, [r2, #0x418]
003b6f40: mov r0, r1
003b6f44: subs r1, r3, #0
003b6f48: movne r1, #1
003b6f4c: b #0x37c7e4

_ZNSt4priv20_Deque_iterator_baseIPSt6vectorIN3sfc6script3lua5ValueESaIS5_EEE10_M_advanceEi 0x31bad0 116
0031bad0: ldr r3, [r0]
0031bad4: ldr r2, [r0, #4]
0031bad8: str r4, [sp, #-4]!
0031badc: rsb r2, r2, r3
0031bae0: add r2, r1, r2, asr #2
0031bae4: mvn ip, r2
0031bae8: lsr r4, ip, #0x1f
0031baec: cmp r2, #0x1f
0031baf0: movgt r4, #0
0031baf4: andle r4, r4, #1
0031baf8: cmp r4, #0
0031bafc: addne r3, r3, r1, lsl #2
0031bb00: strne r3, [r0]
0031bb04: bne #0x31bb3c
0031bb08: ldr r1, [r0, #0xc]
0031bb0c: cmp r2, #0
0031bb10: lsrgt r3, r2, #5
0031bb14: mvnle r3, ip, lsr #5
0031bb18: add ip, r1, r3, lsl #2
0031bb1c: str ip, [r0, #0xc]
0031bb20: sub r2, r2, r3, lsl #5
0031bb24: ldr r3, [r1, r3, lsl #2]
0031bb28: add r2, r3, r2, lsl #2
0031bb2c: add r1, r3, #0x80
0031bb30: str r2, [r0]
0031bb34: str r1, [r0, #8]
0031bb38: str r3, [r0, #4]
0031bb3c: ldm sp!, {r4}
0031bb40: bx lr

_ZN3sfc6script3lua6Binder16__methodCallbackEP9lua_State 0x319ca8 808
00319ca8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00319cac: ldr r4, [pc, #0x2dc]
00319cb0: ldr r2, [pc, #0x2dc]
00319cb4: sub sp, sp, #0x5c
00319cb8: add r4, pc, r4
00319cbc: ldr r3, [r4, r2]
00319cc0: mov r1, #1
00319cc4: str r2, [sp, #0xc]
00319cc8: ldr r3, [r3]
00319ccc: mov r5, r0
00319cd0: str r3, [sp, #0x54]
00319cd4: bl #0x84b264
00319cd8: cmp r0, #5
00319cdc: beq #0x319d04
00319ce0: ldr r3, [pc, #0x2b0]
00319ce4: ldr r3, [r4, r3]
00319ce8: ldr r3, [r3]
00319cec: cmp r3, #2
00319cf0: moveq r3, #0
00319cf4: streq r3, [r3]
00319cf8: beq #0x319d04
00319cfc: cmp r3, #1
00319d00: beq #0x319e8c
00319d04: ldr r2, [pc, #0x290]
00319d08: mov r1, #1
00319d0c: mov r0, r5
00319d10: add r2, pc, r2
00319d14: bl #0x84c1ec
00319d18: mvn r1, #0
00319d1c: mov r0, r5
00319d20: bl #0x84b390
00319d24: add r8, sp, #0x24
00319d28: mvn r1, #1
00319d2c: mov sb, r0
00319d30: mov r0, r5
00319d34: bl #0x84b140
00319d38: add fp, sp, #0x1c
00319d3c: mov r1, r5
00319d40: mvn r2, #0
00319d44: mov r0, r8
00319d48: bl #0x3196ec
00319d4c: add r6, sp, #0x2c
00319d50: mov r2, #2
00319d54: mov r1, r5
00319d58: mov r0, fp
00319d5c: bl #0x3196ec
00319d60: mov r0, r6
00319d64: bl #0x31b434
00319d68: ldr r7, [sp, #0x20]
00319d6c: mov r3, #0
00319d70: str r3, [sp, #0x18]
00319d74: str r3, [sp, #0x14]
00319d78: ldm r7, {r0, r3}
00319d7c: rsb r3, r0, r3
00319d80: asr r3, r3, #4
00319d84: add r2, r3, r3, lsl #3
00319d88: add r2, r2, r2, lsl #6
00319d8c: add r2, r3, r2, lsl #3
00319d90: add r2, r2, r2, lsl #15
00319d94: add r3, r3, r2, lsl #3
00319d98: cmp r3, #0
00319d9c: bne #0x319db0
00319da0: ldr r0, [pc, #0x1f8]
00319da4: add r0, pc, r0
00319da8: bl #0x708eb0
00319dac: ldr r0, [r7]
00319db0: bl #0x31b580
00319db4: ldr r7, [sp, #0x20]
00319db8: str r0, [sp, #0x14]
00319dbc: ldm r7, {r0, r3}
00319dc0: rsb r3, r0, r3
00319dc4: asr r3, r3, #4
00319dc8: add r2, r3, r3, lsl #3
00319dcc: add r2, r2, r2, lsl #6
00319dd0: add r2, r3, r2, lsl #3
00319dd4: add r2, r2, r2, lsl #15
00319dd8: add r3, r3, r2, lsl #3
00319ddc: rsb r3, r3, #0
00319de0: cmp r3, #1
00319de4: bhi #0x319df8
00319de8: ldr r0, [pc, #0x1b4]
00319dec: add r0, pc, r0
00319df0: bl #0x708eb0
00319df4: ldr r0, [r7]
00319df8: add r0, r0, #0x70
00319dfc: bl #0x31b580
00319e00: ldr sl, [sp, #0x14]
00319e04: mov r7, r0
00319e08: str r0, [sp, #0x18]
00319e0c: cmp sl, #0
00319e10: andne ip, r0, #1
00319e14: beq #0x319ec0
00319e18: cmp sb, #0
00319e1c: beq #0x319ef8
00319e20: cmp ip, #0
00319e24: ldrne r3, [sb, r7, asr #1]
00319e28: addeq r0, sb, r7, asr #1
00319e2c: addne r0, sb, r7, asr #1
00319e30: ldrne sl, [r3, sl]
00319e34: mov r2, r6
00319e38: mov r1, r8
00319e3c: blx sl
00319e40: mov r1, r5
00319e44: mov r0, r6
00319e48: bl #0x31b308
00319e4c: mov r5, r0
00319e50: mov r0, r6
00319e54: bl #0x31b398
00319e58: mov r0, fp
00319e5c: bl #0x319228
00319e60: mov r0, r8
00319e64: bl #0x319228
00319e68: ldr r2, [sp, #0xc]
00319e6c: mov r0, r5
00319e70: ldr r3, [r4, r2]
00319e74: ldr r2, [sp, #0x54]
00319e78: ldr r3, [r3]
00319e7c: cmp r2, r3
00319e80: bne #0x319f8c
00319e84: add sp, sp, #0x5c
00319e88: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00319e8c: ldr r0, [pc, #0x114]
00319e90: ldr r1, [pc, #0x114]
00319e94: ldr r2, [pc, #0x114]
00319e98: ldr r0, [r4, r0]
00319e9c: ldr r3, [pc, #0x110]
00319ea0: mov ip, #0x48
00319ea4: add r1, pc, r1
00319ea8: add r2, pc, r2
00319eac: add r3, pc, r3
00319eb0: add r0, r0, #0xa8
00319eb4: str ip, [sp]
00319eb8: bl #0x30e004
00319ebc: b #0x319d04
00319ec0: tst r0, #1
00319ec4: movne ip, #1
00319ec8: bne #0x319e18
00319ecc: ldr r3, [pc, #0xc4]
00319ed0: ldr r3, [r4, r3]
00319ed4: ldr r3, [r3]
00319ed8: cmp r3, #2
00319edc: streq sl, [sl]
00319ee0: moveq ip, sl
00319ee4: beq #0x319e18
00319ee8: cmp r3, #1
00319eec: beq #0x319f54
00319ef0: mov ip, sl
00319ef4: b #0x319e18
00319ef8: ldr r3, [pc, #0x98]
00319efc: ldr r3, [r4, r3]
00319f00: ldr r3, [r3]
00319f04: cmp r3, #2
00319f08: streq sb, [sb]
00319f0c: beq #0x319e20
00319f10: cmp r3, #1
00319f14: bne #0x319e20
00319f18: ldr r0, [pc, #0x88]
00319f1c: ldr r1, [pc, #0x94]
00319f20: ldr r2, [pc, #0x94]
00319f24: ldr r0, [r4, r0]
00319f28: ldr r3, [pc, #0x90]
00319f2c: mov lr, #0x57
00319f30: add r1, pc, r1
00319f34: add r0, r0, #0xa8
00319f38: add r2, pc, r2
00319f3c: add r3, pc, r3
00319f40: str ip, [sp, #8]
00319f44: str lr, [sp]
00319f48: bl #0x30e004
00319f4c: ldr ip, [sp, #8]
00319f50: b #0x319e20
00319f54: ldr r0, [pc, #0x4c]
00319f58: ldr r1, [pc, #0x64]
00319f5c: ldr r2, [pc, #0x64]
00319f60: ldr r0, [r4, r0]
00319f64: ldr r3, [pc, #0x60]
00319f68: mov ip, #0x56
00319f6c: add r1, pc, r1
00319f70: add r0, r0, #0xa8
00319f74: add r2, pc, r2
00319f78: add r3, pc, r3
00319f7c: str ip, [sp]
00319f80: bl #0x30e004
00319f84: mov ip, sl
00319f88: b #0x319e18
00319f8c: bl #0x30e310

_ZN10GameObject7_IsDoorERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38e9d8 24
0038e9d8: ldr r3, [r2, #0xf4]
0038e9dc: mov r0, r1
0038e9e0: cmp r3, #2
0038e9e4: movne r1, #0
0038e9e8: moveq r1, #1
0038e9ec: b #0x37c7e4

_ZN9LuaScript6_TraceERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ee80 4
0037ee80: bx lr

_ZN9Character11_WarpBehindERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8d98 320
003b8d98: push {r4, r5, r6, r7, r8, lr}
003b8d9c: ldr r3, [r0, #4]
003b8da0: mov r4, r2
003b8da4: sub sp, sp, #0x28
003b8da8: ldm r3, {r0, r2}
003b8dac: rsb r3, r0, r2
003b8db0: asr r3, r3, #4
003b8db4: add r2, r3, r3, lsl #3
003b8db8: add r2, r2, r2, lsl #6
003b8dbc: add r2, r3, r2, lsl #3
003b8dc0: add r2, r2, r2, lsl #15
003b8dc4: add r3, r3, r2, lsl #3
003b8dc8: cmp r3, #0
003b8dcc: bne #0x3b8dd8
003b8dd0: add sp, sp, #0x28
003b8dd4: pop {r4, r5, r6, r7, r8, pc}
003b8dd8: ldr r3, [r0, #4]
003b8ddc: cmp r3, #2
003b8de0: beq #0x3b8dec
003b8de4: cmp r3, #7
003b8de8: bne #0x3b8dd0
003b8dec: bl #0x31b5a0
003b8df0: subs r6, r0, #0
003b8df4: beq #0x3b8dd0
003b8df8: mov r3, #0
003b8dfc: add r1, sp, #0x1c
003b8e00: str r3, [sp, #0x24]
003b8e04: str r3, [sp, #0x1c]
003b8e08: str r3, [sp, #0x20]
003b8e0c: bl #0x393ae4
003b8e10: add r1, sp, #0x1c
003b8e14: ldm r1, {r1, r2, r3}
003b8e18: add r2, r2, #0x80000000
003b8e1c: add r3, r3, #0x80000000
003b8e20: add r1, r1, #0x80000000
003b8e24: add r0, sp, #0x10
003b8e28: str r2, [sp, #0x14]
003b8e2c: str r3, [sp, #0x18]
003b8e30: str r1, [sp, #0x10]
003b8e34: bl #0x34d0b0
003b8e38: mov r1, #0x42000000
003b8e3c: mov r5, r0
003b8e40: add r1, r1, #0xc80000
003b8e44: ldr r0, [r0, #4]
003b8e48: bl #0x30ed6c
003b8e4c: mov r1, #0x42000000
003b8e50: mov r8, r0
003b8e54: add r1, r1, #0xc80000
003b8e58: ldr r0, [r5, #8]
003b8e5c: bl #0x30ed6c
003b8e60: mov r1, #0x42000000
003b8e64: mov r7, r0
003b8e68: add r1, r1, #0xc80000
003b8e6c: ldr r0, [r5]
003b8e70: bl #0x30ed6c
003b8e74: str r8, [sp, #0x20]
003b8e78: str r0, [sp, #0x1c]
003b8e7c: str r7, [sp, #0x24]
003b8e80: mov r0, r6
003b8e84: ldr r7, [r4, #0x378]
003b8e88: bl #0x3935dc
003b8e8c: ldr r1, [sp, #0x20]
003b8e90: mov r4, r0
003b8e94: ldr r0, [r0, #4]
003b8e98: bl #0x30eba4
003b8e9c: ldr r1, [sp, #0x24]
003b8ea0: mov r6, r0
003b8ea4: ldr r0, [r4, #8]
003b8ea8: bl #0x30eba4
003b8eac: ldr r1, [sp, #0x1c]
003b8eb0: mov r5, r0
003b8eb4: ldr r0, [r4]
003b8eb8: bl #0x30eba4
003b8ebc: add r1, sp, #4
003b8ec0: str r0, [sp, #4]
003b8ec4: mov r0, r7
003b8ec8: str r6, [sp, #8]
003b8ecc: str r5, [sp, #0xc]
003b8ed0: bl #0x405318
003b8ed4: b #0x3b8dd0

_ZN10LuaManagerD0Ev 0x37a110 28
0037a110: push {r4, lr}
0037a114: mov r4, r0
0037a118: bl #0x37a0ac
0037a11c: mov r0, r4
0037a120: bl #0x310440
0037a124: mov r0, r4
0037a128: pop {r4, pc}

_ZN4Door5_OpenERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3e79a8 12
003e79a8: mov r0, r2
003e79ac: mov r1, #0
003e79b0: b #0x3e786c

_ZN9Character10_HasShieldERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6fa4 32
003b6fa4: push {r4, lr}
003b6fa8: add r0, r2, #0x37c
003b6fac: mov r4, r1
003b6fb0: bl #0x400110
003b6fb4: mov r1, r0
003b6fb8: mov r0, r4
003b6fbc: pop {r4, lr}
003b6fc0: b #0x37c7e4

_ZN3sfc6script3lua5ErrorC2ERKS2_ 0x31a78c 120
0031a78c: ldr r3, [pc, #0x68]
0031a790: ldr r2, [pc, #0x68]
0031a794: push {r4, r5, r6, lr}
0031a798: add r3, pc, r3
0031a79c: ldr r2, [r3, r2]
0031a7a0: mov r5, r0
0031a7a4: mov r4, r0
0031a7a8: add r2, r2, #8
0031a7ac: str r2, [r5], #8
0031a7b0: str r5, [r0, #0x18]
0031a7b4: str r5, [r0, #0x1c]
0031a7b8: mov r0, r5
0031a7bc: mov r6, r1
0031a7c0: bl #0x31a710
0031a7c4: ldr r3, [r4, #0x18]
0031a7c8: mov r1, #0
0031a7cc: add r2, r6, #8
0031a7d0: strb r1, [r3]
0031a7d4: ldr r3, [r6, #4]
0031a7d8: cmp r5, r2
0031a7dc: str r3, [r4, #4]
0031a7e0: beq #0x31a7f4
0031a7e4: mov r0, r5
0031a7e8: ldr r2, [r6, #0x18]
0031a7ec: ldr r1, [r6, #0x1c]
0031a7f0: bl #0x3109e0
0031a7f4: mov r0, r4
0031a7f8: pop {r4, r5, r6, pc}

_ZN9LuaScript8_IncludeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37efe4 88
0037efe4: push {r4, lr}
0037efe8: ldr r3, [r0, #4]
0037efec: mov r4, r2
0037eff0: ldm r3, {r0, r2}
0037eff4: rsb r3, r0, r2
0037eff8: asr r3, r3, #4
0037effc: add r2, r3, r3, lsl #3
0037f000: add r2, r2, r2, lsl #6
0037f004: add r2, r3, r2, lsl #3
0037f008: add r2, r2, r2, lsl #15
0037f00c: add r3, r3, r2, lsl #3
0037f010: cmp r3, #0
0037f014: bne #0x37f01c
0037f018: pop {r4, pc}
0037f01c: ldr r3, [r0, #4]
0037f020: cmp r3, #4
0037f024: bne #0x37f018
0037f028: bl #0x31c49c
0037f02c: mov r1, r0
0037f030: mov r0, r4
0037f034: pop {r4, lr}
0037f038: b #0x37b574

_ZN3sfc6script3lua5ErrorD0Ev 0x31a6f4 28
0031a6f4: push {r4, lr}
0031a6f8: mov r4, r0
0031a6fc: bl #0x31a68c
0031a700: mov r0, r4
0031a704: bl #0x310440
0031a708: mov r0, r4
0031a70c: pop {r4, pc}

_ZN9Character16_SkillCombatRollERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b9fbc 732
003b9fbc: push {r4, r5, r6, r7, r8, sb, sl, lr}
003b9fc0: ldr r6, [r0, #4]
003b9fc4: mov r5, r1
003b9fc8: mov r4, r2
003b9fcc: ldm r6, {r1, r3}
003b9fd0: sub sp, sp, #0x40
003b9fd4: mov r7, r0
003b9fd8: rsb r3, r1, r3
003b9fdc: asr r3, r3, #4
003b9fe0: add r2, r3, r3, lsl #3
003b9fe4: add r2, r2, r2, lsl #6
003b9fe8: add r2, r3, r2, lsl #3
003b9fec: add r2, r2, r2, lsl #15
003b9ff0: add r3, r3, r2, lsl #3
003b9ff4: rsb r3, r3, #0
003b9ff8: cmp r3, #1
003b9ffc: bls #0x3ba014
003ba000: cmp r3, #0
003ba004: beq #0x3ba01c
003ba008: ldr r3, [r1, #4]
003ba00c: cmp r3, #3
003ba010: beq #0x3ba030
003ba014: add sp, sp, #0x40
003ba018: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003ba01c: ldr r0, [pc, #0x268]
003ba020: add r0, pc, r0
003ba024: bl #0x708eb0
003ba028: ldr r1, [r6]
003ba02c: b #0x3ba008
003ba030: mov r1, #0
003ba034: mov r0, r7
003ba038: bl #0x37baf8
003ba03c: bl #0x38d798
003ba040: mov r6, r0
003ba044: mov r0, r4
003ba048: bl #0x3bc5fc
003ba04c: ldr r3, [r0, #4]
003ba050: cmp r6, r3
003ba054: bhs #0x3ba014
003ba058: ldr r6, [r7, #4]
003ba05c: ldr r3, [r6]
003ba060: ldr r2, [r6, #4]
003ba064: rsb r2, r3, r2
003ba068: asr r2, r2, #4
003ba06c: add r1, r2, r2, lsl #3
003ba070: add r1, r1, r1, lsl #6
003ba074: add r1, r2, r1, lsl #3
003ba078: add r1, r1, r1, lsl #15
003ba07c: add r2, r2, r1, lsl #3
003ba080: rsb r2, r2, #0
003ba084: cmp r2, #1
003ba088: bhi #0x3ba09c
003ba08c: ldr r0, [pc, #0x1fc]
003ba090: add r0, pc, r0
003ba094: bl #0x708eb0
003ba098: ldr r3, [r6]
003ba09c: ldr r3, [r3, #0x74]
003ba0a0: cmp r3, #2
003ba0a4: beq #0x3ba0f4
003ba0a8: ldr r6, [r7, #4]
003ba0ac: ldm r6, {r2, r3}
003ba0b0: rsb r3, r2, r3
003ba0b4: asr r3, r3, #4
003ba0b8: add r1, r3, r3, lsl #3
003ba0bc: add r1, r1, r1, lsl #6
003ba0c0: add r1, r3, r1, lsl #3
003ba0c4: add r1, r1, r1, lsl #15
003ba0c8: add r3, r3, r1, lsl #3
003ba0cc: rsb r3, r3, #0
003ba0d0: cmp r3, #1
003ba0d4: bhi #0x3ba0e8
003ba0d8: ldr r0, [pc, #0x1b4]
003ba0dc: add r0, pc, r0
003ba0e0: bl #0x708eb0
003ba0e4: ldr r2, [r6]
003ba0e8: ldr r3, [r2, #0x74]
003ba0ec: cmp r3, #7
003ba0f0: bne #0x3ba014
003ba0f4: mov r1, #0
003ba0f8: mov r0, r7
003ba0fc: bl #0x37baf8
003ba100: bl #0x38d798
003ba104: mov r1, #1
003ba108: mov r8, r0
003ba10c: mov r0, r7
003ba110: bl #0x37baf8
003ba114: bl #0x31b5a0
003ba118: subs r6, r0, #0
003ba11c: beq #0x3ba014
003ba120: add r7, sp, #0x34
003ba124: mov r1, r6
003ba128: mov r0, r7
003ba12c: bl #0x33dd2c
003ba130: mov r0, r7
003ba134: bl #0x33ff54
003ba138: subs sl, r0, #0
003ba13c: beq #0x3ba24c
003ba140: mov r1, r8
003ba144: mov r0, r4
003ba148: bl #0x3bc784
003ba14c: ldr r8, [r0, #0x1c]
003ba150: mov r6, r0
003ba154: ands r7, r8, #0x800000
003ba158: beq #0x3ba1c4
003ba15c: add sb, r4, #0x37c
003ba160: mov r0, sb
003ba164: bl #0x3ffe8c
003ba168: cmp r0, #0
003ba16c: bne #0x3ba208
003ba170: mov r0, sb
003ba174: bl #0x400158
003ba178: cmp r0, #0
003ba17c: beq #0x3ba014
003ba180: ldr ip, [r6, #0x14]
003ba184: add r6, sp, #0xc
003ba188: orr r3, r8, #0x4000000
003ba18c: mov r0, r6
003ba190: mov r1, r4
003ba194: mov r2, sl
003ba198: str ip, [sp]
003ba19c: bl #0x3b31a4
003ba1a0: mov r0, r6
003ba1a4: mov r1, r4
003ba1a8: mov r2, sl
003ba1ac: mov r3, #0
003ba1b0: bl #0x3b10b4
003ba1b4: mov r0, r5
003ba1b8: ldr r1, [sp, #0xc]
003ba1bc: bl #0x37cb24
003ba1c0: b #0x3ba014
003ba1c4: ldr ip, [r0, #0x14]
003ba1c8: add r6, sp, #0xc
003ba1cc: mov r3, r8
003ba1d0: mov r0, r6
003ba1d4: mov r1, r4
003ba1d8: mov r2, sl
003ba1dc: str ip, [sp]
003ba1e0: bl #0x3b31a4
003ba1e4: mov r0, r6
003ba1e8: mov r1, r4
003ba1ec: mov r2, sl
003ba1f0: mov r3, r7
003ba1f4: bl #0x3b10b4
003ba1f8: mov r0, r5
003ba1fc: ldr r1, [sp, #0xc]
003ba200: bl #0x37cb24
003ba204: b #0x3ba014
003ba208: ldr ip, [r6, #0x14]
003ba20c: add r7, sp, #0xc
003ba210: mov r3, r8
003ba214: mov r0, r7
003ba218: mov r1, r4
003ba21c: mov r2, sl
003ba220: str ip, [sp]
003ba224: bl #0x3b31a4
003ba228: mov r0, r7
003ba22c: mov r1, r4
003ba230: mov r2, sl
003ba234: mov r3, #0
003ba238: bl #0x3b10b4
003ba23c: mov r0, r5
003ba240: ldr r1, [sp, #0xc]
003ba244: bl #0x37cb24
003ba248: b #0x3ba170
003ba24c: ldr r3, [r6]
003ba250: mov r0, r6
003ba254: mov r1, r4
003ba258: mov lr, pc
003ba25c: ldr pc, [r3, #0x90]
003ba260: cmp r0, #8
003ba264: bne #0x3ba014
003ba268: mov r0, r6
003ba26c: mov r1, r4
003ba270: ldr r3, [r6]
003ba274: mov lr, pc
003ba278: ldr pc, [r3, #0x98]
003ba27c: mov r0, r5
003ba280: mov r1, sl
003ba284: bl #0x37c7e4
003ba288: b #0x3ba014
003ba28c: subseq r4, r0, r8, asr #8
003ba290: ldrsbeq r4, [r0], #-0x38
003ba294: subseq r4, r0, ip, lsl #7

_ZN9Character11_RemoveDotsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7be0 476
003b7be0: push {r4, r5, r6, r7, r8, sb, sl, lr}
003b7be4: ldr r3, [r0, #4]
003b7be8: mov r5, r2
003b7bec: mov r4, r0
003b7bf0: ldr r1, [r3, #4]
003b7bf4: ldr r2, [r3]
003b7bf8: movw r3, #0x1390
003b7bfc: ldr r6, [r5, r3]
003b7c00: rsb r3, r2, r1
003b7c04: asr r3, r3, #4
003b7c08: add r1, r3, r3, lsl #3
003b7c0c: add r1, r1, r1, lsl #6
003b7c10: add r1, r3, r1, lsl #3
003b7c14: add r1, r1, r1, lsl #15
003b7c18: add r3, r3, r1, lsl #3
003b7c1c: rsb r3, r3, #0
003b7c20: cmp r3, #0
003b7c24: bne #0x3b7cbc
003b7c28: cmp r3, #1
003b7c2c: bls #0x3b7c40
003b7c30: add r0, r2, #0x70
003b7c34: ldr r3, [r0, #4]
003b7c38: cmp r3, #3
003b7c3c: beq #0x3b7d58
003b7c40: cmp r6, #0
003b7c44: ble #0x3b7cb8
003b7c48: movw r7, #0x1390
003b7c4c: ldr r3, [r5, r7]
003b7c50: cmp r3, #0
003b7c54: addne r8, r5, #0x560
003b7c58: movne r4, #0
003b7c5c: beq #0x3b7cb8
003b7c60: add r4, r4, #1
003b7c64: mov r0, r8
003b7c68: mvn r1, #0
003b7c6c: bl #0x3de83c
003b7c70: cmp r4, r6
003b7c74: bne #0x3b7d78
003b7c78: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003b7c7c: movw r7, #0x1390
003b7c80: ldr r3, [r5, r7]
003b7c84: cmp r3, #0
003b7c88: addne sl, r5, #0x560
003b7c8c: movne r4, #0
003b7c90: beq #0x3b7cb8
003b7c94: add r4, r4, #1
003b7c98: mov r0, sl
003b7c9c: mov r1, r8
003b7ca0: bl #0x3de83c
003b7ca4: cmp r4, r6
003b7ca8: beq #0x3b7cb8
003b7cac: ldr r3, [r5, r7]
003b7cb0: cmp r3, #0
003b7cb4: bne #0x3b7c94
003b7cb8: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003b7cbc: ldr r1, [r2, #4]
003b7cc0: cmp r1, #3
003b7cc4: bne #0x3b7c28
003b7cc8: mov r1, #0
003b7ccc: bl #0x37baf8
003b7cd0: bl #0x31bbf0
003b7cd4: bl #0x30e4cc
003b7cd8: cmp r0, #0
003b7cdc: blt #0x3b7d88
003b7ce0: ldr r6, [r4, #4]
003b7ce4: ldm r6, {r0, r3}
003b7ce8: rsb r3, r0, r3
003b7cec: asr r3, r3, #4
003b7cf0: add r2, r3, r3, lsl #3
003b7cf4: add r2, r2, r2, lsl #6
003b7cf8: add r2, r3, r2, lsl #3
003b7cfc: add r2, r2, r2, lsl #15
003b7d00: add r3, r3, r2, lsl #3
003b7d04: cmp r3, #0
003b7d08: bne #0x3b7d1c
003b7d0c: ldr r0, [pc, #0xa4]
003b7d10: add r0, pc, r0
003b7d14: bl #0x708eb0
003b7d18: ldr r0, [r6]
003b7d1c: bl #0x31bbf0
003b7d20: bl #0x30e4cc
003b7d24: ldr r3, [r4, #4]
003b7d28: mov r6, r0
003b7d2c: ldr r1, [r3, #4]
003b7d30: ldr r2, [r3]
003b7d34: rsb r1, r2, r1
003b7d38: asr r1, r1, #4
003b7d3c: add r3, r1, r1, lsl #3
003b7d40: add r3, r3, r3, lsl #6
003b7d44: add r3, r1, r3, lsl #3
003b7d48: add r3, r3, r3, lsl #15
003b7d4c: add r3, r1, r3, lsl #3
003b7d50: rsb r3, r3, #0
003b7d54: b #0x3b7c28
003b7d58: bl #0x31bbf0
003b7d5c: bl #0x30e4cc
003b7d60: cmn r0, #2
003b7d64: mov r8, r0
003b7d68: beq #0x3b7c40
003b7d6c: cmp r6, #0
003b7d70: bgt #0x3b7c7c
003b7d74: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003b7d78: ldr r3, [r5, r7]
003b7d7c: cmp r3, #0
003b7d80: bne #0x3b7c60
003b7d84: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003b7d88: ldr r3, [r4, #4]
003b7d8c: ldr r1, [r3, #4]
003b7d90: ldr r2, [r3]
003b7d94: rsb r3, r2, r1
003b7d98: asr r3, r3, #4
003b7d9c: add r1, r3, r3, lsl #3
003b7da0: add r1, r1, r1, lsl #6
003b7da4: add r1, r3, r1, lsl #3
003b7da8: add r1, r1, r1, lsl #15
003b7dac: add r3, r3, r1, lsl #3
003b7db0: rsb r3, r3, #0
003b7db4: b #0x3b7c28
003b7db8: subseq r6, r0, r8, asr r7

_ZN9Character21_SpawnSkillProjectileERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b99d0 956
003b99d0: push {r4, r5, r6, r7, r8, lr}
003b99d4: ldr r6, [r0, #4]
003b99d8: mov r5, r1
003b99dc: mov r8, r2
003b99e0: ldr r3, [r6]
003b99e4: ldr r1, [r6, #4]
003b99e8: ldr r7, [pc, #0x378]
003b99ec: sub sp, sp, #0x18
003b99f0: rsb r1, r3, r1
003b99f4: asr r1, r1, #4
003b99f8: add r7, pc, r7
003b99fc: add r2, r1, r1, lsl #3
003b9a00: mov r4, r0
003b9a04: add r2, r2, r2, lsl #6
003b9a08: add r2, r1, r2, lsl #3
003b9a0c: add r2, r2, r2, lsl #15
003b9a10: add r1, r1, r2, lsl #3
003b9a14: rsb r1, r1, #0
003b9a18: cmp r1, #1
003b9a1c: bls #0x3b9a34
003b9a20: cmp r1, #0
003b9a24: beq #0x3b9a3c
003b9a28: ldr r3, [r3, #4]
003b9a2c: cmp r3, #3
003b9a30: beq #0x3b9a58
003b9a34: add sp, sp, #0x18
003b9a38: pop {r4, r5, r6, r7, r8, pc}
003b9a3c: ldr r0, [pc, #0x328]
003b9a40: add r0, pc, r0
003b9a44: bl #0x708eb0
003b9a48: ldr r3, [r6]
003b9a4c: ldr r3, [r3, #4]
003b9a50: cmp r3, #3
003b9a54: bne #0x3b9a34
003b9a58: ldr r6, [r4, #4]
003b9a5c: ldm r6, {r2, r3}
003b9a60: rsb r3, r2, r3
003b9a64: asr r3, r3, #4
003b9a68: add r1, r3, r3, lsl #3
003b9a6c: add r1, r1, r1, lsl #6
003b9a70: add r1, r3, r1, lsl #3
003b9a74: add r1, r1, r1, lsl #15
003b9a78: add r3, r3, r1, lsl #3
003b9a7c: rsb r3, r3, #0
003b9a80: cmp r3, #1
003b9a84: bhi #0x3b9a98
003b9a88: ldr r0, [pc, #0x2e0]
003b9a8c: add r0, pc, r0
003b9a90: bl #0x708eb0
003b9a94: ldr r2, [r6]
003b9a98: ldr r3, [r2, #0x74]
003b9a9c: cmp r3, #3
003b9aa0: bne #0x3b9a34
003b9aa4: mov r1, #1
003b9aa8: mov r0, r4
003b9aac: bl #0x37baf8
003b9ab0: bl #0x38d798
003b9ab4: ldr r3, [pc, #0x2b8]
003b9ab8: ldr r3, [r7, r3]
003b9abc: ldr r3, [r3]
003b9ac0: cmp r0, r3
003b9ac4: bhs #0x3b9a34
003b9ac8: mov r1, #0
003b9acc: mov r0, r4
003b9ad0: bl #0x37baf8
003b9ad4: bl #0x31bbf0
003b9ad8: bl #0x30e4cc
003b9adc: ldr r3, [r8, #0x47c]
003b9ae0: ldr r6, [r3, r0, lsl #2]
003b9ae4: cmp r6, #0
003b9ae8: beq #0x3b9cdc
003b9aec: ldr r2, [r4, #4]
003b9af0: ldr r3, [r2]
003b9af4: ldr r2, [r2, #4]
003b9af8: rsb r3, r3, r2
003b9afc: asr r3, r3, #4
003b9b00: add r2, r3, r3, lsl #3
003b9b04: add r2, r2, r2, lsl #6
003b9b08: add r2, r3, r2, lsl #3
003b9b0c: add r2, r2, r2, lsl #15
003b9b10: add r3, r3, r2, lsl #3
003b9b14: rsb r3, r3, #0
003b9b18: cmp r3, #2
003b9b1c: bhi #0x3b9b88
003b9b20: mov r1, #1
003b9b24: mov r0, r4
003b9b28: bl #0x37baf8
003b9b2c: bl #0x38d798
003b9b30: mov r2, #0
003b9b34: mov r1, r0
003b9b38: mov r0, r6
003b9b3c: bl #0x3da510
003b9b40: mov r6, r0
003b9b44: ldr r2, [r4, #4]
003b9b48: ldr r3, [r2]
003b9b4c: ldr r2, [r2, #4]
003b9b50: rsb r3, r3, r2
003b9b54: asr r3, r3, #4
003b9b58: add r2, r3, r3, lsl #3
003b9b5c: add r2, r2, r2, lsl #6
003b9b60: add r2, r3, r2, lsl #3
003b9b64: add r2, r2, r2, lsl #15
003b9b68: add r3, r3, r2, lsl #3
003b9b6c: rsb r3, r3, #0
003b9b70: cmp r3, #2
003b9b74: bhi #0x3b9c24
003b9b78: mov r0, r5
003b9b7c: mov r1, r6
003b9b80: bl #0x38eb00
003b9b84: b #0x3b9a34
003b9b88: mov r1, #2
003b9b8c: mov r0, r4
003b9b90: bl #0x37baf8
003b9b94: ldr r1, [r0, #4]
003b9b98: cmp r1, #1
003b9b9c: beq #0x3b9d30
003b9ba0: ldr r3, [r4, #4]
003b9ba4: ldm r3, {r2, r3}
003b9ba8: rsb r3, r2, r3
003b9bac: asr r3, r3, #4
003b9bb0: add r2, r3, r3, lsl #3
003b9bb4: add r2, r2, r2, lsl #6
003b9bb8: add r2, r3, r2, lsl #3
003b9bbc: add r2, r2, r2, lsl #15
003b9bc0: add r3, r3, r2, lsl #3
003b9bc4: rsb r3, r3, #0
003b9bc8: cmp r3, #2
003b9bcc: bls #0x3b9b20
003b9bd0: mov r0, r4
003b9bd4: mov r1, #2
003b9bd8: bl #0x37baf8
003b9bdc: ldr r3, [r0, #4]
003b9be0: cmp r3, #3
003b9be4: bne #0x3b9b20
003b9be8: mov r1, #1
003b9bec: mov r0, r4
003b9bf0: bl #0x37baf8
003b9bf4: bl #0x38d798
003b9bf8: mov r1, #2
003b9bfc: mov r7, r0
003b9c00: mov r0, r4
003b9c04: bl #0x37baf8
003b9c08: bl #0x31bbf0
003b9c0c: mov r1, r7
003b9c10: mov r2, r0
003b9c14: mov r0, r6
003b9c18: bl #0x3da494
003b9c1c: mov r6, r0
003b9c20: b #0x3b9b44
003b9c24: mov r0, r4
003b9c28: mov r1, #2
003b9c2c: bl #0x37baf8
003b9c30: ldr r3, [r0, #4]
003b9c34: cmp r3, #7
003b9c38: bne #0x3b9b78
003b9c3c: mov r1, #2
003b9c40: mov r0, r4
003b9c44: ldr r7, [r6, #0x168]
003b9c48: bl #0x37baf8
003b9c4c: bl #0x31b5a0
003b9c50: ldr r3, [r0, #0x164]
003b9c54: ldr ip, [r0, #0x160]
003b9c58: mov r2, #1
003b9c5c: mov r0, r6
003b9c60: add r1, sp, #0xc
003b9c64: str r3, [sp, #0x10]
003b9c68: str ip, [sp, #0xc]
003b9c6c: str r7, [sp, #0x14]
003b9c70: bl #0x393db4
003b9c74: ldr r3, [r4, #4]
003b9c78: ldm r3, {r2, r3}
003b9c7c: rsb r3, r2, r3
003b9c80: asr r3, r3, #4
003b9c84: add r2, r3, r3, lsl #3
003b9c88: add r2, r2, r2, lsl #6
003b9c8c: add r2, r3, r2, lsl #3
003b9c90: add r2, r2, r2, lsl #15
003b9c94: add r3, r3, r2, lsl #3
003b9c98: rsb r3, r3, #0
003b9c9c: cmp r3, #3
003b9ca0: bls #0x3b9b78
003b9ca4: mov r0, r4
003b9ca8: mov r1, #3
003b9cac: bl #0x37baf8
003b9cb0: ldr r3, [r0, #4]
003b9cb4: cmp r3, #7
003b9cb8: bne #0x3b9b78
003b9cbc: mov r1, #3
003b9cc0: mov r0, r4
003b9cc4: bl #0x37baf8
003b9cc8: bl #0x31b5a0
003b9ccc: add r1, r0, #0x160
003b9cd0: mov r0, r6
003b9cd4: bl #0x393600
003b9cd8: b #0x3b9b78
003b9cdc: ldr r3, [pc, #0x94]
003b9ce0: ldr r3, [r7, r3]
003b9ce4: ldr r3, [r3]
003b9ce8: cmp r3, #2
003b9cec: streq r6, [r6]
003b9cf0: beq #0x3b9aec
003b9cf4: cmp r3, #1
003b9cf8: bne #0x3b9aec
003b9cfc: ldr r0, [pc, #0x78]
003b9d00: ldr r1, [pc, #0x78]
003b9d04: ldr r2, [pc, #0x78]
003b9d08: ldr r0, [r7, r0]
003b9d0c: ldr r3, [pc, #0x74]
003b9d10: mov ip, #0x394
003b9d14: add r1, pc, r1
003b9d18: add r2, pc, r2
003b9d1c: add r3, pc, r3
003b9d20: add r0, r0, #0xa8
003b9d24: str ip, [sp]
003b9d28: bl #0x30e004
003b9d2c: b #0x3b9aec
003b9d30: mov r0, r4
003b9d34: bl #0x37baf8
003b9d38: bl #0x38d798
003b9d3c: mov r1, #2
003b9d40: mov r7, r0
003b9d44: mov r0, r4
003b9d48: bl #0x37baf8
003b9d4c: bl #0x31bc80
003b9d50: mov r1, r7
003b9d54: mov r2, r0
003b9d58: mov r0, r6
003b9d5c: bl #0x3da510
003b9d60: mov r6, r0
003b9d64: b #0x3b9b44

_ZN3sfc6script3lua8Instance9setGlobalEPKcRKNS1_5ValueE 0x31aed8 48
0031aed8: push {r4, r5, r6, lr}
0031aedc: mov r4, r0
0031aee0: mov r5, r1
0031aee4: mov r0, r2
0031aee8: ldr r1, [r4, #4]
0031aeec: bl #0x31cac4
0031aef0: ldr r0, [r4, #4]
0031aef4: mvn r1, #0x2700
0031aef8: sub r1, r1, #0x11
0031aefc: mov r2, r5
0031af00: pop {r4, r5, r6, lr}
0031af04: b #0x84c080

_ZN9Character19_GetPropBonusDamageERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7acc 92
003b7acc: push {r4, r5, r6, lr}
003b7ad0: ldr r3, [r0, #4]
003b7ad4: mov r5, r2
003b7ad8: mov r4, r1
003b7adc: ldm r3, {r0, r2}
003b7ae0: rsb r3, r0, r2
003b7ae4: asr r3, r3, #4
003b7ae8: add r2, r3, r3, lsl #3
003b7aec: add r2, r2, r2, lsl #6
003b7af0: add r2, r3, r2, lsl #3
003b7af4: add r2, r2, r2, lsl #15
003b7af8: add r3, r3, r2, lsl #3
003b7afc: cmp r3, #0
003b7b00: bne #0x3b7b08
003b7b04: pop {r4, r5, r6, pc}
003b7b08: bl #0x31bc80
003b7b0c: mov r1, r0
003b7b10: add r0, r5, #0x560
003b7b14: bl #0x3df8ac
003b7b18: mov r1, r0
003b7b1c: mov r0, r4
003b7b20: pop {r4, r5, r6, lr}
003b7b24: b #0x37cb24

_ZNSt4priv6__findINS_15_Deque_iteratorIPSt6vectorIN3sfc6script3lua5ValueESaIS6_EESt16_Nonconst_traitsIS9_EEES9_EET_SD_SD_RKT0_RKSt26random_access_iterator_tag 0x31b65c 640
0031b65c: push {r4, r5, r6, r7, lr}
0031b660: sub sp, sp, #0x24
0031b664: add ip, sp, #0x10
0031b668: mov r7, r2
0031b66c: mov r4, r1
0031b670: mov r6, r0
0031b674: mov r5, r3
0031b678: ldm r1, {r0, r1, r2, r3}
0031b67c: stm ip, {r0, r1, r2, r3}
0031b680: mov r1, ip
0031b684: mov r0, r7
0031b688: bl #0x31b618
0031b68c: asr r0, r0, #2
0031b690: cmp r0, #0
0031b694: bgt #0x31b710
0031b698: b #0x31b7ec
0031b69c: ldr r1, [r3]
0031b6a0: ldr r2, [r5]
0031b6a4: cmp r1, r2
0031b6a8: beq #0x31b768
0031b6ac: ldr r2, [r4, #8]
0031b6b0: add r3, r3, #4
0031b6b4: str r3, [r4]
0031b6b8: cmp r3, r2
0031b6bc: beq #0x31b77c
0031b6c0: ldr r1, [r3]
0031b6c4: ldr r2, [r5]
0031b6c8: cmp r1, r2
0031b6cc: beq #0x31b768
0031b6d0: ldr r2, [r4, #8]
0031b6d4: add r3, r3, #4
0031b6d8: str r3, [r4]
0031b6dc: cmp r3, r2
0031b6e0: beq #0x31b7a0
0031b6e4: ldr r1, [r3]
0031b6e8: ldr r2, [r5]
0031b6ec: cmp r1, r2
0031b6f0: beq #0x31b768
0031b6f4: ldr r2, [r4, #8]
0031b6f8: add r3, r3, #4
0031b6fc: str r3, [r4]
0031b700: cmp r3, r2
0031b704: beq #0x31b7c4
0031b708: subs r0, r0, #1
0031b70c: beq #0x31b7ec
0031b710: ldr r3, [r4]
0031b714: ldr r2, [r5]
0031b718: ldr r1, [r3]
0031b71c: cmp r1, r2
0031b720: beq #0x31b768
0031b724: ldr r2, [r4, #8]
0031b728: add r3, r3, #4
0031b72c: str r3, [r4]
0031b730: cmp r3, r2
0031b734: bne #0x31b69c
0031b738: ldr r3, [r4, #0xc]
0031b73c: add r2, r3, #4
0031b740: str r2, [r4, #0xc]
0031b744: ldr r3, [r3, #4]
0031b748: add r2, r3, #0x80
0031b74c: str r2, [r4, #8]
0031b750: str r3, [r4, #4]
0031b754: str r3, [r4]
0031b758: ldr r1, [r3]
0031b75c: ldr r2, [r5]
0031b760: cmp r1, r2
0031b764: bne #0x31b6ac
0031b768: ldm r4, {r0, r1, r2, r3}
0031b76c: stm r6, {r0, r1, r2, r3}
0031b770: mov r0, r6
0031b774: add sp, sp, #0x24
0031b778: pop {r4, r5, r6, r7, pc}
0031b77c: ldr r3, [r4, #0xc]
0031b780: add r2, r3, #4
0031b784: str r2, [r4, #0xc]
0031b788: ldr r3, [r3, #4]
0031b78c: add r2, r3, #0x80
0031b790: str r2, [r4, #8]
0031b794: str r3, [r4, #4]
0031b798: str r3, [r4]
0031b79c: b #0x31b6c0
0031b7a0: ldr r3, [r4, #0xc]
0031b7a4: add r2, r3, #4
0031b7a8: str r2, [r4, #0xc]
0031b7ac: ldr r3, [r3, #4]
0031b7b0: add r2, r3, #0x80
0031b7b4: str r2, [r4, #8]
0031b7b8: str r3, [r4, #4]
0031b7bc: str r3, [r4]
0031b7c0: b #0x31b6e4
0031b7c4: ldr r3, [r4, #0xc]
0031b7c8: subs r0, r0, #1
0031b7cc: add r2, r3, #4
0031b7d0: str r2, [r4, #0xc]
0031b7d4: ldr r3, [r3, #4]
0031b7d8: add r2, r3, #0x80
0031b7dc: str r2, [r4, #8]
0031b7e0: str r3, [r4]
0031b7e4: str r3, [r4, #4]
0031b7e8: bne #0x31b710
0031b7ec: mov ip, sp
0031b7f0: ldm r4, {r0, r1, r2, r3}
0031b7f4: stm ip, {r0, r1, r2, r3}
0031b7f8: mov r1, sp
0031b7fc: mov r0, r7
0031b800: bl #0x31b618
0031b804: cmp r0, #2
0031b808: beq #0x31b828
0031b80c: cmp r0, #3
0031b810: beq #0x31b86c
0031b814: cmp r0, #1
0031b818: beq #0x31b864
0031b81c: ldm r7, {r0, r1, r2, r3}
0031b820: stm r6, {r0, r1, r2, r3}
0031b824: b #0x31b770
0031b828: ldr r3, [r4]
0031b82c: ldr r1, [r3]
0031b830: ldr r2, [r5]
0031b834: cmp r1, r2
0031b838: beq #0x31b768
0031b83c: ldr r2, [r4, #8]
0031b840: add r3, r3, #4
0031b844: str r3, [r4]
0031b848: cmp r3, r2
0031b84c: beq #0x31b8b8
0031b850: ldr r2, [r3]
0031b854: ldr r3, [r5]
0031b858: cmp r2, r3
0031b85c: bne #0x31b81c
0031b860: b #0x31b768
0031b864: ldr r3, [r4]
0031b868: b #0x31b850
0031b86c: ldr r3, [r4]
0031b870: ldr r2, [r5]
0031b874: ldr r1, [r3]
0031b878: cmp r1, r2
0031b87c: beq #0x31b768
0031b880: ldr r2, [r4, #8]
0031b884: add r3, r3, #4
0031b888: str r3, [r4]
0031b88c: cmp r3, r2
0031b890: bne #0x31b82c
0031b894: ldr r3, [r4, #0xc]
0031b898: add r2, r3, #4
0031b89c: str r2, [r4, #0xc]
0031b8a0: ldr r3, [r3, #4]
0031b8a4: add r2, r3, #0x80
0031b8a8: str r2, [r4, #8]
0031b8ac: str r3, [r4, #4]
0031b8b0: str r3, [r4]
0031b8b4: b #0x31b82c
0031b8b8: ldr r3, [r4, #0xc]
0031b8bc: add r2, r3, #4
0031b8c0: str r2, [r4, #0xc]
0031b8c4: ldr r3, [r3, #4]
0031b8c8: add r2, r3, #0x80
0031b8cc: str r2, [r4, #8]
0031b8d0: str r3, [r4, #4]
0031b8d4: str r3, [r4]
0031b8d8: b #0x31b850

_ZN10GameObject15_MarkAsSwimmingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x39088c 116
0039088c: str lr, [sp, #-4]!
00390890: ldr r3, [r0, #4]
00390894: sub sp, sp, #0xc
00390898: ldr r1, [r3, #4]
0039089c: ldr ip, [r3]
003908a0: rsb r3, ip, r1
003908a4: asr r3, r3, #4
003908a8: add r1, r3, r3, lsl #3
003908ac: add r1, r1, r1, lsl #6
003908b0: add r1, r3, r1, lsl #3
003908b4: add r1, r1, r1, lsl #15
003908b8: add r3, r3, r1, lsl #3
003908bc: cmp r3, #0
003908c0: bne #0x3908cc
003908c4: add sp, sp, #0xc
003908c8: ldm sp!, {pc}
003908cc: ldr r3, [ip, #4]
003908d0: cmp r3, #1
003908d4: bne #0x3908c4
003908d8: mov r1, #0
003908dc: str r2, [sp, #4]
003908e0: bl #0x37baf8
003908e4: bl #0x31bc80
003908e8: ldr r2, [sp, #4]
003908ec: mov r1, r0
003908f0: add r0, r2, #0x1c8
003908f4: add sp, sp, #0xc
003908f8: pop {lr}
003908fc: b #0x524218

_ZN3sfc6script3lua8Instance12includeDebugEv 0x31aff0 8
0031aff0: ldr r0, [r0, #4]
0031aff4: b #0x84ed24

_ZN9LuaScript10_PlayMusicERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e420 344
0037e420: push {r4, r5, r6, r7, lr}
0037e424: ldr r5, [r0, #4]
0037e428: mov r6, r0
0037e42c: ldr r4, [pc, #0x128]
0037e430: ldm r5, {r0, r3}
0037e434: add r4, pc, r4
0037e438: sub sp, sp, #0xc
0037e43c: rsb r3, r0, r3
0037e440: asr r3, r3, #4
0037e444: add r2, r3, r3, lsl #3
0037e448: add r2, r2, r2, lsl #6
0037e44c: add r2, r3, r2, lsl #3
0037e450: add r2, r2, r2, lsl #15
0037e454: add r3, r3, r2, lsl #3
0037e458: cmp r3, #0
0037e45c: bne #0x37e470
0037e460: ldr r0, [pc, #0xf8]
0037e464: add r0, pc, r0
0037e468: bl #0x708eb0
0037e46c: ldr r0, [r5]
0037e470: bl #0x31c49c
0037e474: bl #0x37ba84
0037e478: cmn r0, #1
0037e47c: mov r5, r0
0037e480: beq #0x37e504
0037e484: ldr r7, [r6, #4]
0037e488: ldr r2, [pc, #0xd4]
0037e48c: ldm r7, {r0, r3}
0037e490: ldr r2, [r4, r2]
0037e494: rsb r3, r0, r3
0037e498: asr r3, r3, #4
0037e49c: ldr r6, [r2]
0037e4a0: add r2, r3, r3, lsl #3
0037e4a4: add r2, r2, r2, lsl #6
0037e4a8: add r2, r3, r2, lsl #3
0037e4ac: add r2, r2, r2, lsl #15
0037e4b0: add r3, r3, r2, lsl #3
0037e4b4: rsb r3, r3, #0
0037e4b8: cmp r3, #1
0037e4bc: bls #0x37e50c
0037e4c0: add r0, r0, #0x70
0037e4c4: bl #0x31bbf0
0037e4c8: bl #0x30e4cc
0037e4cc: mov r1, r5
0037e4d0: str r0, [sp]
0037e4d4: mov r2, #1
0037e4d8: mov r0, r6
0037e4dc: mov r3, #0
0037e4e0: bl #0x36bd78
0037e4e4: ldr r3, [pc, #0x7c]
0037e4e8: ldr r0, [r4, r3]
0037e4ec: bl #0x31f594
0037e4f0: cmp r0, #0
0037e4f4: beq #0x37e504
0037e4f8: ldr r3, [r0, #0x11c]
0037e4fc: cmp r5, r3
0037e500: beq #0x37e520
0037e504: add sp, sp, #0xc
0037e508: pop {r4, r5, r6, r7, pc}
0037e50c: ldr r0, [pc, #0x58]
0037e510: add r0, pc, r0
0037e514: bl #0x708eb0
0037e518: ldr r0, [r7]
0037e51c: b #0x37e4c0
0037e520: ldrb r3, [r6, #0x31]
0037e524: cmp r3, #0
0037e528: bne #0x37e544
0037e52c: ldr r1, [pc, #0x3c]
0037e530: mov r0, r6
0037e534: add r1, pc, r1
0037e538: add sp, sp, #0xc
0037e53c: pop {r4, r5, r6, r7, lr}
0037e540: b #0x369514
0037e544: ldr r1, [pc, #0x28]
0037e548: mov r0, r6
0037e54c: add r1, pc, r1
0037e550: add sp, sp, #0xc
0037e554: pop {r4, r5, r6, r7, lr}
0037e558: b #0x369514
0037e55c: rsbeq r6, r1, ip, asr r6
0037e560: subseq r0, r4, r4
0037e564: andeq r0, r0, r4, lsr #27
0037e568: strdeq r3, r4, [r0], -r4
0037e56c: subseq pc, r3, r8, asr pc
0037e570: ldrsheq r3, [r4], #-0x64
0037e574: ldrsbeq r3, [r4], #-0x64

_ZN9LuaScript4LoadEPKc 0x37b574 44
0037b574: ldr r3, [pc, #0x1c]
0037b578: ldr r2, [pc, #0x1c]
0037b57c: mov ip, r0
0037b580: add r3, pc, r3
0037b584: ldr r0, [r3, r2]
0037b588: mov r2, r1
0037b58c: mov r1, ip
0037b590: ldr r0, [r0, #0x3c]
0037b594: b #0x37b23c
0037b598: rsbeq sb, r1, r0, lsl r5
0037b59c: strdeq r3, r4, [r0], -r4

_ZN9Character12_ResumeTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b74a8 116
003b74a8: str lr, [sp, #-4]!
003b74ac: ldr r3, [r0, #4]
003b74b0: sub sp, sp, #0xc
003b74b4: ldr r1, [r3, #4]
003b74b8: ldr ip, [r3]
003b74bc: rsb r3, ip, r1
003b74c0: asr r3, r3, #4
003b74c4: add r1, r3, r3, lsl #3
003b74c8: add r1, r1, r1, lsl #6
003b74cc: add r1, r3, r1, lsl #3
003b74d0: add r1, r1, r1, lsl #15
003b74d4: add r3, r3, r1, lsl #3
003b74d8: cmp r3, #0
003b74dc: bne #0x3b74e8
003b74e0: add sp, sp, #0xc
003b74e4: ldm sp!, {pc}
003b74e8: ldr r3, [ip, #4]
003b74ec: cmp r3, #3
003b74f0: bne #0x3b74e0
003b74f4: mov r1, #0
003b74f8: str r2, [sp, #4]
003b74fc: bl #0x37baf8
003b7500: bl #0x38d798
003b7504: ldr r2, [sp, #4]
003b7508: mov r1, r0
003b750c: add r0, r2, #0x3b4
003b7510: add sp, sp, #0xc
003b7514: pop {lr}
003b7518: b #0x3db2b8

_ZN4Door6_CloseERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3e777c 12
003e777c: mov r0, r2
003e7780: mov r1, #0
003e7784: b #0x3e763c

_ZN9LuaScript21_GetGameObjectsByTypeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f03c 436
0037f03c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037f040: ldr r3, [r0, #4]
0037f044: sub sp, sp, #0x1c
0037f048: str r1, [sp, #4]
0037f04c: ldr r2, [r3, #4]
0037f050: mov r5, r0
0037f054: ldr r0, [r3]
0037f058: ldr r4, [pc, #0x188]
0037f05c: rsb r3, r0, r2
0037f060: asr r3, r3, #4
0037f064: add r4, pc, r4
0037f068: add r2, r3, r3, lsl #3
0037f06c: add r2, r2, r2, lsl #6
0037f070: add r2, r3, r2, lsl #3
0037f074: add r2, r2, r2, lsl #15
0037f078: add r3, r3, r2, lsl #3
0037f07c: rsb r3, r3, #0
0037f080: cmp r3, #1
0037f084: bls #0x37f098
0037f088: add r2, r0, #0x70
0037f08c: ldr r1, [r2, #4]
0037f090: cmp r1, #3
0037f094: beq #0x37f1ac
0037f098: mov fp, #0
0037f09c: cmp r3, #0
0037f0a0: bne #0x37f0ac
0037f0a4: add sp, sp, #0x1c
0037f0a8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037f0ac: ldr r3, [r0, #4]
0037f0b0: cmp r3, #4
0037f0b4: bne #0x37f0a4
0037f0b8: bl #0x31c49c
0037f0bc: ldr r3, [pc, #0x128]
0037f0c0: mov sb, r0
0037f0c4: ldr r3, [r4, r3]
0037f0c8: ldr sl, [r3, #0x38]
0037f0cc: ldr r4, [sl, #0x14]
0037f0d0: add sl, sl, #0xc
0037f0d4: cmp r4, sl
0037f0d8: beq #0x37f0a4
0037f0dc: mov r8, #0
0037f0e0: mov r7, r8
0037f0e4: add r6, sp, #0xc
0037f0e8: ldr r1, [r4, #0x2c]
0037f0ec: cmp r1, #0
0037f0f0: beq #0x37f140
0037f0f4: mov r0, r6
0037f0f8: bl #0x33dd2c
0037f0fc: mov r0, r6
0037f100: bl #0x33fee4
0037f104: subs r5, r0, #0
0037f108: beq #0x37f140
0037f10c: add r0, r5, #4
0037f110: bl #0x510b4c
0037f114: mov r1, sb
0037f118: bl #0x30e31c
0037f11c: cmp r0, #0
0037f120: bne #0x37f140
0037f124: cmp fp, r8
0037f128: addhi r8, r8, #1
0037f12c: bhi #0x37f140
0037f130: mov r1, r5
0037f134: ldr r0, [sp, #4]
0037f138: bl #0x37c9f8
0037f13c: add r7, r7, #1
0037f140: ldr r2, [r4, #0xc]
0037f144: cmp r2, #0
0037f148: bne #0x37f154
0037f14c: b #0x37f178
0037f150: mov r2, r3
0037f154: ldr r3, [r2, #8]
0037f158: cmp r3, #0
0037f15c: bne #0x37f150
0037f160: mov r4, r2
0037f164: cmp sl, r4
0037f168: beq #0x37f0a4
0037f16c: cmp r7, #0xe
0037f170: bhi #0x37f0a4
0037f174: b #0x37f0e8
0037f178: ldr r3, [r4, #4]
0037f17c: ldr r1, [r3, #0xc]
0037f180: cmp r4, r1
0037f184: bne #0x37f1a0
0037f188: mov r4, r3
0037f18c: ldr r3, [r3, #4]
0037f190: ldr r2, [r3, #0xc]
0037f194: cmp r2, r4
0037f198: beq #0x37f188
0037f19c: ldr r2, [r4, #0xc]
0037f1a0: cmp r2, r3
0037f1a4: movne r4, r3
0037f1a8: b #0x37f164
0037f1ac: mov r0, r2
0037f1b0: bl #0x31bbf0
0037f1b4: bl #0x8be2a0
0037f1b8: ldr r3, [r5, #4]
0037f1bc: mov fp, r0
0037f1c0: ldm r3, {r0, r2}
0037f1c4: rsb r3, r0, r2
0037f1c8: asr r3, r3, #4
0037f1cc: add r2, r3, r3, lsl #3
0037f1d0: add r2, r2, r2, lsl #6
0037f1d4: add r2, r3, r2, lsl #3
0037f1d8: add r2, r2, r2, lsl #15
0037f1dc: add r3, r3, r2, lsl #3
0037f1e0: rsb r3, r3, #0
0037f1e4: b #0x37f09c
0037f1e8: rsbeq r5, r1, ip, lsr #20
0037f1ec: strdeq r3, r4, [r0], -r4

_ZN10GameObject21_SetTargetListSortingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390548 220
00390548: push {r4, r5, r6, r7, r8, lr}
0039054c: ldr r3, [r0, #4]
00390550: mov r5, r2
00390554: ldr r4, [pc, #0xb8]
00390558: ldm r3, {r1, r2}
0039055c: add r4, pc, r4
00390560: rsb r3, r1, r2
00390564: asr r3, r3, #4
00390568: add r2, r3, r3, lsl #3
0039056c: add r2, r2, r2, lsl #6
00390570: add r2, r3, r2, lsl #3
00390574: add r2, r2, r2, lsl #15
00390578: add r3, r3, r2, lsl #3
0039057c: cmp r3, #0
00390580: bne #0x390588
00390584: pop {r4, r5, r6, r7, r8, pc}
00390588: ldr r3, [r1, #4]
0039058c: cmp r3, #3
00390590: bne #0x390584
00390594: mov r1, #0
00390598: bl #0x37baf8
0039059c: bl #0x31bbf0
003905a0: bl #0x30e4cc
003905a4: ldr r2, [r5, #0x314]
003905a8: ldr r3, [r5, #0x304]
003905ac: mov r7, r0
003905b0: cmp r2, r3
003905b4: beq #0x3905d4
003905b8: add r6, r5, #0x304
003905bc: mov r0, r6
003905c0: bl #0x38fb18
003905c4: ldr r2, [r5, #0x314]
003905c8: ldr r3, [r5, #0x304]
003905cc: cmp r2, r3
003905d0: bne #0x3905bc
003905d4: cmp r7, #1
003905d8: beq #0x390604
003905dc: cmp r7, #2
003905e0: beq #0x3905f4
003905e4: ldr r3, [pc, #0x2c]
003905e8: ldr r3, [r4, r3]
003905ec: str r3, [r5, #0x32c]
003905f0: b #0x390584
003905f4: ldr r3, [pc, #0x20]
003905f8: ldr r3, [r4, r3]
003905fc: str r3, [r5, #0x32c]
00390600: pop {r4, r5, r6, r7, r8, pc}
00390604: ldr r3, [pc, #0x14]
00390608: ldr r3, [r4, r3]
0039060c: str r3, [r5, #0x32c]
00390610: pop {r4, r5, r6, r7, r8, pc}
00390614: rsbeq r4, r0, r4, lsr r5
00390618: andeq r2, r0, r4, ror ip
0039061c: ldrdeq r1, r2, [r0], -r4
00390620: andeq r4, r0, r4, asr #21

_ZN10GameObject26_SetTargetListObjectFilterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390624 108
00390624: str lr, [sp, #-4]!
00390628: ldr r3, [r0, #4]
0039062c: sub sp, sp, #0xc
00390630: ldr r1, [r3, #4]
00390634: ldr ip, [r3]
00390638: rsb r3, ip, r1
0039063c: asr r3, r3, #4
00390640: add r1, r3, r3, lsl #3
00390644: add r1, r1, r1, lsl #6
00390648: add r1, r3, r1, lsl #3
0039064c: add r1, r1, r1, lsl #15
00390650: add r3, r3, r1, lsl #3
00390654: cmp r3, #0
00390658: bne #0x390664
0039065c: add sp, sp, #0xc
00390660: ldm sp!, {pc}
00390664: ldr r3, [ip, #4]
00390668: cmp r3, #3
0039066c: bne #0x39065c
00390670: mov r1, #0
00390674: str r2, [sp, #4]
00390678: bl #0x37baf8
0039067c: bl #0x31bbf0
00390680: bl #0x30e4cc
00390684: ldr r2, [sp, #4]
00390688: str r0, [r2, #0x33c]
0039068c: b #0x39065c

_ZN9LuaScript21_GetCurrentLevelRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37f1f0 356
0037f1f0: push {r4, r5, r6, r7, r8, lr}
0037f1f4: ldr r4, [pc, #0x14c]
0037f1f8: ldr r3, [pc, #0x14c]
0037f1fc: mov r7, r0
0037f200: add r4, pc, r4
0037f204: ldr r0, [r4, r3]
0037f208: mov r6, r1
0037f20c: bl #0x31f594
0037f210: ldr r5, [r0, #0x3c]
0037f214: cmn r5, #1
0037f218: beq #0x37f32c
0037f21c: ldr r2, [r7, #4]
0037f220: ldm r2, {r0, r3}
0037f224: rsb r3, r0, r3
0037f228: asr r3, r3, #4
0037f22c: add r2, r3, r3, lsl #3
0037f230: add r2, r2, r2, lsl #6
0037f234: add r2, r3, r2, lsl #3
0037f238: add r2, r2, r2, lsl #15
0037f23c: add r3, r3, r2, lsl #3
0037f240: cmp r3, #0
0037f244: bne #0x37f284
0037f248: mov r3, #0x48
0037f24c: mul r5, r3, r5
0037f250: ldr r3, [pc, #0xf8]
0037f254: mov r0, r6
0037f258: ldr r4, [r4, r3]
0037f25c: ldr r3, [r4]
0037f260: add r3, r3, r5
0037f264: ldr r1, [r3, #0x3c]
0037f268: bl #0x37cb24
0037f26c: ldr r3, [r4]
0037f270: mov r0, r6
0037f274: add r5, r3, r5
0037f278: ldr r1, [r5, #0x30]
0037f27c: pop {r4, r5, r6, r7, r8, lr}
0037f280: b #0x37cb24
0037f284: ldr r3, [r0, #4]
0037f288: cmp r3, #3
0037f28c: bne #0x37f248
0037f290: bl #0x31bbf0
0037f294: bl #0x30e4cc
0037f298: cmp r0, #1
0037f29c: beq #0x37f2b4
0037f2a0: cmp r0, #2
0037f2a4: beq #0x37f2f0
0037f2a8: cmp r0, #0
0037f2ac: beq #0x37f248
0037f2b0: pop {r4, r5, r6, r7, r8, pc}
0037f2b4: mov r3, #0x48
0037f2b8: mul r5, r3, r5
0037f2bc: ldr r3, [pc, #0x8c]
0037f2c0: mov r0, r6
0037f2c4: ldr r4, [r4, r3]
0037f2c8: ldr r3, [r4]
0037f2cc: add r3, r3, r5
0037f2d0: ldr r1, [r3, #0x40]
0037f2d4: bl #0x37cb24
0037f2d8: ldr r3, [r4]
0037f2dc: mov r0, r6
0037f2e0: add r5, r3, r5
0037f2e4: ldr r1, [r5, #0x34]
0037f2e8: pop {r4, r5, r6, r7, r8, lr}
0037f2ec: b #0x37cb24
0037f2f0: mov r3, #0x48
0037f2f4: mul r5, r3, r5
0037f2f8: ldr r3, [pc, #0x50]
0037f2fc: mov r0, r6
0037f300: ldr r4, [r4, r3]
0037f304: ldr r3, [r4]
0037f308: add r3, r3, r5
0037f30c: ldr r1, [r3, #0x44]
0037f310: bl #0x37cb24
0037f314: ldr r3, [r4]
0037f318: mov r0, r6
0037f31c: add r5, r3, r5
0037f320: ldr r1, [r5, #0x38]
0037f324: pop {r4, r5, r6, r7, r8, lr}
0037f328: b #0x37cb24
0037f32c: mov r0, r6
0037f330: mov r1, r5
0037f334: bl #0x37cb24
0037f338: mov r0, r6
0037f33c: mov r1, r5
0037f340: pop {r4, r5, r6, r7, r8, lr}
0037f344: b #0x37cb24
0037f348: mlseq r1, r0, r8, r5
0037f34c: strdeq r3, r4, [r0], -r4
0037f350: andeq r0, r0, r4, ror r8

_ZN3sfc6script3lua9Arguments10pushStringEPKc 0x39ec10 104
0039ec10: ldr r3, [pc, #0x58]
0039ec14: ldr r2, [pc, #0x58]
0039ec18: push {r4, r5, r6, lr}
0039ec1c: add r3, pc, r3
0039ec20: ldr r5, [r3, r2]
0039ec24: sub sp, sp, #0x78
0039ec28: add r4, sp, #4
0039ec2c: ldr r3, [r5]
0039ec30: str r3, [sp, #0x74]
0039ec34: ldr r6, [r0, #4]
0039ec38: mov r0, r4
0039ec3c: bl #0x37c84c
0039ec40: mov r0, r6
0039ec44: mov r1, r4
0039ec48: bl #0x3195c0
0039ec4c: mov r0, r4
0039ec50: bl #0x3193e8
0039ec54: ldr r2, [sp, #0x74]
0039ec58: ldr r3, [r5]
0039ec5c: cmp r2, r3
0039ec60: bne #0x39ec6c
0039ec64: add sp, sp, #0x78
0039ec68: pop {r4, r5, r6, pc}
0039ec6c: bl #0x30e310
0039ec70: subseq r5, pc, r4, ror lr
0039ec74: andeq r4, r0, ip, lsr #1

_ZN10GameObject14_SetFXEndPointERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390974 456
00390974: push {r4, r5, lr}
00390978: ldr r2, [r0, #4]
0039097c: sub sp, sp, #0x14
00390980: mov r4, r0
00390984: ldm r2, {r1, r3}
00390988: rsb r3, r1, r3
0039098c: asr r3, r3, #4
00390990: add r2, r3, r3, lsl #3
00390994: add r2, r2, r2, lsl #6
00390998: add r2, r3, r2, lsl #3
0039099c: add r2, r2, r2, lsl #15
003909a0: add r3, r3, r2, lsl #3
003909a4: rsb r3, r3, #0
003909a8: cmp r3, #2
003909ac: beq #0x3909c0
003909b0: cmp r3, #4
003909b4: beq #0x3909c0
003909b8: add sp, sp, #0x14
003909bc: pop {r4, r5, pc}
003909c0: ldr r2, [r1, #4]
003909c4: cmp r2, #2
003909c8: bne #0x3909b8
003909cc: cmp r3, #2
003909d0: beq #0x390a7c
003909d4: cmp r3, #4
003909d8: beq #0x390ac4
003909dc: mov r1, #0
003909e0: mov r0, r4
003909e4: bl #0x37baf8
003909e8: bl #0x31b580
003909ec: ldr r2, [r4, #4]
003909f0: mov r3, #0
003909f4: str r3, [sp, #0xc]
003909f8: str r3, [sp, #4]
003909fc: str r3, [sp, #8]
00390a00: ldr r3, [r2]
00390a04: ldr r2, [r2, #4]
00390a08: mov r5, r0
00390a0c: rsb r3, r3, r2
00390a10: asr r3, r3, #4
00390a14: add r2, r3, r3, lsl #3
00390a18: add r2, r2, r2, lsl #6
00390a1c: add r2, r3, r2, lsl #3
00390a20: add r2, r2, r2, lsl #15
00390a24: add r3, r3, r2, lsl #3
00390a28: cmn r3, #2
00390a2c: beq #0x390b0c
00390a30: mov r1, #1
00390a34: mov r0, r4
00390a38: bl #0x37baf8
00390a3c: bl #0x31bbf0
00390a40: mov r1, #2
00390a44: str r0, [sp, #4]
00390a48: mov r0, r4
00390a4c: bl #0x37baf8
00390a50: bl #0x31bbf0
00390a54: mov r1, #3
00390a58: str r0, [sp, #8]
00390a5c: mov r0, r4
00390a60: bl #0x37baf8
00390a64: bl #0x31bbf0
00390a68: str r0, [sp, #0xc]
00390a6c: mov r0, r5
00390a70: add r1, sp, #4
00390a74: bl #0x492560
00390a78: b #0x3909b8
00390a7c: mov r0, r4
00390a80: mov r1, #1
00390a84: bl #0x37baf8
00390a88: ldr r3, [r0, #4]
00390a8c: cmp r3, #7
00390a90: bne #0x3909b8
00390a94: ldr r3, [r4, #4]
00390a98: ldr r2, [r3, #4]
00390a9c: ldr r3, [r3]
00390aa0: rsb r3, r3, r2
00390aa4: asr r3, r3, #4
00390aa8: add r2, r3, r3, lsl #3
00390aac: add r2, r2, r2, lsl #6
00390ab0: add r2, r3, r2, lsl #3
00390ab4: add r2, r2, r2, lsl #15
00390ab8: add r3, r3, r2, lsl #3
00390abc: rsb r3, r3, #0
00390ac0: b #0x3909d4
00390ac4: mov r0, r4
00390ac8: mov r1, #1
00390acc: bl #0x37baf8
00390ad0: ldr r3, [r0, #4]
00390ad4: cmp r3, #3
00390ad8: bne #0x3909b8
00390adc: mov r1, #2
00390ae0: mov r0, r4
00390ae4: bl #0x37baf8
00390ae8: ldr r1, [r0, #4]
00390aec: cmp r1, #3
00390af0: bne #0x3909b8
00390af4: mov r0, r4
00390af8: bl #0x37baf8
00390afc: ldr r3, [r0, #4]
00390b00: cmp r3, #3
00390b04: bne #0x3909b8
00390b08: b #0x3909dc
00390b0c: mov r1, #1
00390b10: mov r0, r4
00390b14: bl #0x37baf8
00390b18: bl #0x31b5a0
00390b1c: bl #0x3935dc
00390b20: ldr r3, [r0]
00390b24: str r3, [sp, #4]
00390b28: ldr r3, [r0, #4]
00390b2c: str r3, [sp, #8]
00390b30: ldr r3, [r0, #8]
00390b34: str r3, [sp, #0xc]
00390b38: b #0x390a6c

_ZN9LuaScript6SetIntEPKci 0x37d990 160
0037d990: push {r4, r5, r6, lr}
0037d994: mov r6, r0
0037d998: sub sp, sp, #0x10
0037d99c: mov r0, r1
0037d9a0: mov r5, r2
0037d9a4: bl #0x37c164
0037d9a8: ldr ip, [r6, #0x20]
0037d9ac: add r1, r6, #0x1c
0037d9b0: mov r4, r0
0037d9b4: cmp ip, #0
0037d9b8: moveq ip, r1
0037d9bc: beq #0x37d9ec
0037d9c0: mov r2, r1
0037d9c4: b #0x37d9cc
0037d9c8: mov ip, r3
0037d9cc: ldr r3, [ip, #0x10]
0037d9d0: cmp r4, r3
0037d9d4: ldrhi r3, [ip, #0xc]
0037d9d8: ldrls r3, [ip, #8]
0037d9dc: movhi ip, r2
0037d9e0: mov r2, ip
0037d9e4: cmp r3, #0
0037d9e8: bne #0x37d9c8
0037d9ec: cmp r1, ip
0037d9f0: beq #0x37da04
0037d9f4: ldr r2, [ip, #0x10]
0037d9f8: mov r3, ip
0037d9fc: cmp r4, r2
0037da00: bhs #0x37da24
0037da04: mov r3, sp
0037da08: mov lr, #0
0037da0c: add r0, sp, #8
0037da10: add r2, sp, #0xc
0037da14: stm sp, {r4, lr}
0037da18: str ip, [sp, #0xc]
0037da1c: bl #0x37d61c
0037da20: ldr r3, [sp, #8]
0037da24: str r5, [r3, #0x14]
0037da28: add sp, sp, #0x10
0037da2c: pop {r4, r5, r6, pc}

_ZN9LuaScriptC2Eb 0x37c674 240
0037c674: push {r4, r5, r6, r7, r8, lr}
0037c678: ldr r6, [pc, #0xd8]
0037c67c: ldr r3, [pc, #0xd8]
0037c680: mov r7, r0
0037c684: add r6, pc, r6
0037c688: ldr r3, [r6, r3]
0037c68c: mov r4, r0
0037c690: mov r8, r1
0037c694: add r3, r3, #8
0037c698: str r3, [r7], #4
0037c69c: mov r0, r7
0037c6a0: bl #0x31b268
0037c6a4: ldr r2, [pc, #0xb4]
0037c6a8: mov r5, #0
0037c6ac: mov r3, r4
0037c6b0: ldr r2, [r6, r2]
0037c6b4: str r7, [r4, #0x14]
0037c6b8: str r5, [r4, #0x18]
0037c6bc: add r2, r2, #8
0037c6c0: str r2, [r4, #0x10]
0037c6c4: str r5, [r4, #0x20]
0037c6c8: mov r2, r4
0037c6cc: strb r5, [r3, #0x1c]!
0037c6d0: str r3, [r4, #0x28]
0037c6d4: str r3, [r4, #0x24]
0037c6d8: str r5, [r4, #0x2c]
0037c6dc: mov r3, r4
0037c6e0: str r5, [r4, #0x38]
0037c6e4: strb r5, [r2, #0x34]!
0037c6e8: str r2, [r4, #0x40]
0037c6ec: str r2, [r4, #0x3c]
0037c6f0: add r0, r4, #0x68
0037c6f4: str r5, [r4, #0x44]
0037c6f8: str r5, [r4, #0x50]
0037c6fc: strb r5, [r3, #0x4c]!
0037c700: str r3, [r4, #0x58]
0037c704: str r3, [r4, #0x54]
0037c708: str r5, [r4, #0x5c]
0037c70c: strb r5, [r4, #0x64]
0037c710: str r0, [r4, #0x78]
0037c714: str r0, [r4, #0x7c]
0037c718: mov r1, #0x10
0037c71c: bl #0x31167c
0037c720: ldr r2, [r4, #0x78]
0037c724: mov r3, r4
0037c728: cmp r8, r5
0037c72c: strb r5, [r2]
0037c730: str r5, [r4, #0x84]
0037c734: strb r5, [r3, #0x80]!
0037c738: str r3, [r4, #0x8c]
0037c73c: str r5, [r4, #0x90]
0037c740: str r3, [r4, #0x88]
0037c744: bne #0x37c750
0037c748: mov r0, r4
0037c74c: bl #0x37b5a0
0037c750: mov r0, r4
0037c754: pop {r4, r5, r6, r7, r8, pc}
0037c758: rsbeq r8, r1, ip, lsl #8
0037c75c: andeq r1, r0, r4, ror r6
0037c760: andeq r3, r0, r8, asr r6

_ZN3sfc6script3lua5ErrorC1Ev 0x31a804 84
0031a804: ldr r2, [pc, #0x44]
0031a808: ldr r1, [pc, #0x44]
0031a80c: mov r3, r0
0031a810: add r2, pc, r2
0031a814: ldr r1, [r2, r1]
0031a818: push {r4, lr}
0031a81c: add r1, r1, #8
0031a820: mov r4, r0
0031a824: str r1, [r3], #8
0031a828: mov r0, r3
0031a82c: str r3, [r4, #0x18]
0031a830: str r3, [r4, #0x1c]
0031a834: bl #0x31a710
0031a838: ldr r2, [r4, #0x18]
0031a83c: mov r3, #0
0031a840: mov r0, r4
0031a844: strb r3, [r2]
0031a848: str r3, [r4, #4]
0031a84c: pop {r4, pc}
0031a850: rsbeq sl, r7, r0, lsl #5
0031a854: muleq r0, r8, r4

_ZN9Character10_GetTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6c7c 12
003b6c7c: mov r0, r1
003b6c80: ldr r1, [r2, #0x408]
003b6c84: b #0x37c9f8

_ZN9Character20_EnableSpotTargetingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7144 608
003b7144: push {r4, r5, r6, lr}
003b7148: ldr r3, [r0, #4]
003b714c: ldr r4, [pc, #0x248]
003b7150: sub sp, sp, #8
003b7154: ldr r1, [r3, #4]
003b7158: ldr ip, [r3]
003b715c: add r4, pc, r4
003b7160: mov r5, r0
003b7164: rsb r3, ip, r1
003b7168: asr r3, r3, #4
003b716c: add r1, r3, r3, lsl #3
003b7170: add r1, r1, r1, lsl #6
003b7174: add r1, r3, r1, lsl #3
003b7178: add r1, r1, r1, lsl #15
003b717c: add r3, r3, r1, lsl #3
003b7180: cmp r3, #0
003b7184: bne #0x3b7190
003b7188: add sp, sp, #8
003b718c: pop {r4, r5, r6, pc}
003b7190: ldr r6, [ip, #4]
003b7194: cmp r6, #1
003b7198: bne #0x3b7188
003b719c: mov r1, #0
003b71a0: str r2, [sp, #4]
003b71a4: bl #0x37baf8
003b71a8: bl #0x31bc80
003b71ac: cmp r0, #0
003b71b0: ldr r2, [sp, #4]
003b71b4: beq #0x3b722c
003b71b8: ldr r1, [r5, #4]
003b71bc: ldr r3, [r1]
003b71c0: ldr r1, [r1, #4]
003b71c4: rsb r3, r3, r1
003b71c8: asr r3, r3, #4
003b71cc: add r1, r3, r3, lsl #3
003b71d0: add r1, r1, r1, lsl #6
003b71d4: add r1, r3, r1, lsl #3
003b71d8: add r1, r1, r1, lsl #15
003b71dc: add r3, r3, r1, lsl #3
003b71e0: rsb r3, r3, #0
003b71e4: cmp r3, #1
003b71e8: bls #0x3b7188
003b71ec: mov r0, r5
003b71f0: mov r1, r6
003b71f4: bl #0x37baf8
003b71f8: ldr r3, [r0, #4]
003b71fc: cmp r3, #3
003b7200: bne #0x3b7188
003b7204: mov r1, r6
003b7208: mov r0, r5
003b720c: bl #0x37baf8
003b7210: bl #0x38d798
003b7214: ldr r3, [pc, #0x184]
003b7218: ldr r2, [sp, #4]
003b721c: ldr r3, [r4, r3]
003b7220: ldr r3, [r3]
003b7224: cmp r0, r3
003b7228: bhs #0x3b7188
003b722c: mov r1, #0
003b7230: mov r0, r5
003b7234: str r2, [sp, #4]
003b7238: bl #0x37baf8
003b723c: bl #0x31bc80
003b7240: cmp r0, #0
003b7244: ldr r2, [sp, #4]
003b7248: beq #0x3b72e8
003b724c: ldr r3, [r5, #4]
003b7250: ldm r3, {r1, r3}
003b7254: rsb r3, r1, r3
003b7258: asr r3, r3, #4
003b725c: add r1, r3, r3, lsl #3
003b7260: add r1, r1, r1, lsl #6
003b7264: add r1, r3, r1, lsl #3
003b7268: add r1, r1, r1, lsl #15
003b726c: add r3, r3, r1, lsl #3
003b7270: rsb r3, r3, #0
003b7274: cmp r3, #2
003b7278: bhi #0x3b7300
003b727c: mov r1, #1
003b7280: mov r0, r5
003b7284: str r2, [sp, #4]
003b7288: bl #0x37baf8
003b728c: bl #0x38d798
003b7290: ldr r2, [sp, #4]
003b7294: movw r4, #0x14ca
003b7298: ldr ip, [r2, #0x160]
003b729c: ldr r1, [r2, #0x164]
003b72a0: ldr r3, [r2, #0x168]
003b72a4: strh r0, [r2, r4]
003b72a8: movw r0, #0x14c8
003b72ac: mov r4, #1
003b72b0: strb r4, [r2, r0]
003b72b4: movw r0, #0x14bc
003b72b8: str ip, [r2, r0]
003b72bc: mov r0, #0x14c0
003b72c0: str r1, [r2, r0]
003b72c4: movw r0, #0x14c4
003b72c8: str r3, [r2, r0]
003b72cc: movw r0, #0x14b0
003b72d0: str ip, [r2, r0]
003b72d4: movw r0, #0x14b4
003b72d8: str r1, [r2, r0]
003b72dc: movw r1, #0x14b8
003b72e0: str r3, [r2, r1]
003b72e4: b #0x3b7188
003b72e8: movw r3, #0x14c8
003b72ec: strb r0, [r2, r3]
003b72f0: mvn r1, #0
003b72f4: movw r3, #0x14ca
003b72f8: strh r1, [r2, r3]
003b72fc: b #0x3b7188
003b7300: mov r0, r5
003b7304: mov r1, #2
003b7308: str r2, [sp, #4]
003b730c: bl #0x37baf8
003b7310: ldr r3, [r0, #4]
003b7314: ldr r2, [sp, #4]
003b7318: cmp r3, #7
003b731c: bne #0x3b727c
003b7320: mov r1, #2
003b7324: mov r0, r5
003b7328: bl #0x37baf8
003b732c: bl #0x31b5a0
003b7330: mov r1, #1
003b7334: mov r4, r0
003b7338: mov r0, r5
003b733c: bl #0x37baf8
003b7340: bl #0x38d798
003b7344: ldr r2, [sp, #4]
003b7348: mov r1, #1
003b734c: movw r3, #0x14c8
003b7350: strb r1, [r2, r3]
003b7354: ldr ip, [r4, #0x160]
003b7358: movw r3, #0x14b0
003b735c: str ip, [r2, r3]
003b7360: ldr r1, [r4, #0x164]
003b7364: movw r3, #0x14b4
003b7368: str r1, [r2, r3]
003b736c: ldr r3, [r4, #0x168]
003b7370: movw r4, #0x14ca
003b7374: strh r0, [r2, r4]
003b7378: movw r0, #0x14bc
003b737c: str ip, [r2, r0]
003b7380: mov r0, #0x14c0
003b7384: str r1, [r2, r0]
003b7388: movw r1, #0x14c4
003b738c: str r3, [r2, r1]
003b7390: movw r1, #0x14b8
003b7394: str r3, [r2, r1]
003b7398: b #0x3b7188
003b739c: subseq sp, sp, r4, lsr sb
003b73a0: andeq r0, r0, r4, asr #13

_ZN10GameObject12_IsOverAHoleERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ead4 32
0038ead4: push {r4, lr}
0038ead8: add r0, r2, #0x1c8
0038eadc: mov r4, r1
0038eae0: bl #0x5241c0
0038eae4: mov r1, r0
0038eae8: mov r0, r4
0038eaec: pop {r4, lr}
0038eaf0: b #0x37c7e4

_ZN3sfc6script3lua5Value13freeValueListEPSt6vectorIS2_SaIS2_EE 0x31d194 1004
0031d194: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0031d198: ldr r5, [pc, #0x3b4]
0031d19c: sub sp, sp, #0x114
0031d1a0: subs r3, r0, #0
0031d1a4: str r0, [sp, #0xc]
0031d1a8: add r5, pc, r5
0031d1ac: beq #0x31d4f4
0031d1b0: ldr r6, [pc, #0x3a0]
0031d1b4: add r3, sp, #0xc
0031d1b8: add r2, sp, #0xe0
0031d1bc: ldr r4, [r5, r6]
0031d1c0: add r0, sp, #0xf0
0031d1c4: add r1, sp, #0xd0
0031d1c8: ldmib r4, {r7, ip}
0031d1cc: ldr r8, [r4]
0031d1d0: str ip, [sp, #0xd8]
0031d1d4: ldr ip, [r4, #0x10]
0031d1d8: ldr sl, [r4, #0x1c]
0031d1dc: ldr sb, [r4, #0x18]
0031d1e0: ldr fp, [r4, #0x14]
0031d1e4: ldr lr, [r4, #0xc]
0031d1e8: str ip, [sp, #0xe0]
0031d1ec: add ip, sp, #0x10c
0031d1f0: str r3, [sp, #8]
0031d1f4: str lr, [sp, #0xdc]
0031d1f8: str r7, [sp, #0xd4]
0031d1fc: str r8, [sp, #0xd0]
0031d200: str sl, [sp, #0xec]
0031d204: str sb, [sp, #0xe8]
0031d208: str fp, [sp, #0xe4]
0031d20c: str ip, [sp]
0031d210: bl #0x31b65c
0031d214: ldr r3, [r4, #0x10]
0031d218: ldr r2, [sp, #0xf0]
0031d21c: cmp r3, r2
0031d220: movne r3, r2
0031d224: beq #0x31d498
0031d228: ldr r8, [sp, #0xf4]
0031d22c: ldr r7, [sp, #0xf8]
0031d230: ldr sl, [sp, #0xfc]
0031d234: add fp, r3, #4
0031d238: cmp r7, fp
0031d23c: ldr r4, [r5, r6]
0031d240: str r7, [sp, #0xc8]
0031d244: str r3, [sp, #0xc0]
0031d248: str sl, [sp, #0xcc]
0031d24c: str r8, [sp, #0xc4]
0031d250: ldreq r8, [sl, #4]!
0031d254: add ip, sp, #0xa0
0031d258: ldm r4, {r0, r1, r2, r3}
0031d25c: stm ip, {r0, r1, r2, r3}
0031d260: mov r1, ip
0031d264: add r0, sp, #0xc0
0031d268: addeq r7, r8, #0x80
0031d26c: moveq fp, r8
0031d270: bl #0x31b618
0031d274: add ip, sp, #0x90
0031d278: mov sb, r0
0031d27c: ldm r4, {r0, r1, r2, r3}
0031d280: stm ip, {r0, r1, r2, r3}
0031d284: mov r1, ip
0031d288: add r0, r4, #0x10
0031d28c: bl #0x31b618
0031d290: cmp sb, r0, lsr #1
0031d294: bhs #0x31d394
0031d298: ldr ip, [r4, #0xc]
0031d29c: ldr lr, [r4, #8]
0031d2a0: add r2, sp, #0x60
0031d2a4: str ip, [sp, #0x7c]
0031d2a8: ldr ip, [r4, #4]
0031d2ac: add r3, sp, #0x50
0031d2b0: add r0, sp, #0x80
0031d2b4: str ip, [sp, #0x74]
0031d2b8: ldr ip, [r4]
0031d2bc: add r1, sp, #0x70
0031d2c0: str lr, [sp, #0x78]
0031d2c4: str ip, [sp, #0x70]
0031d2c8: ldr ip, [sp, #0xcc]
0031d2cc: str sl, [sp, #0x5c]
0031d2d0: str r7, [sp, #0x58]
0031d2d4: str ip, [sp, #0x6c]
0031d2d8: ldr ip, [sp, #0xc8]
0031d2dc: str r8, [sp, #0x54]
0031d2e0: str fp, [sp, #0x50]
0031d2e4: str ip, [sp, #0x68]
0031d2e8: ldr ip, [sp, #0xc4]
0031d2ec: str ip, [sp, #0x64]
0031d2f0: ldr ip, [sp, #0xc0]
0031d2f4: str ip, [sp, #0x60]
0031d2f8: add ip, sp, #0x104
0031d2fc: str ip, [sp]
0031d300: mov ip, #0
0031d304: str ip, [sp, #4]
0031d308: bl #0x31b8dc
0031d30c: ldr r2, [r4, #8]
0031d310: ldr r3, [r4]
0031d314: sub r2, r2, #4
0031d318: cmp r3, r2
0031d31c: addne r3, r3, #4
0031d320: strne r3, [r4]
0031d324: beq #0x31d45c
0031d328: ldr r3, [r5, r6]
0031d32c: add ip, sp, #0xb0
0031d330: ldm r3, {r0, r1, r2, r3}
0031d334: stm ip, {r0, r1, r2, r3}
0031d338: mov r0, ip
0031d33c: mov r1, sb
0031d340: bl #0x31bad0
0031d344: ldr r3, [pc, #0x210]
0031d348: ldr r0, [r5, r3]
0031d34c: ldr r2, [r0, #0x18]
0031d350: ldr r3, [r0, #0x10]
0031d354: sub r2, r2, #4
0031d358: cmp r3, r2
0031d35c: beq #0x31d548
0031d360: ldr r2, [sp, #0xc]
0031d364: str r2, [r3]
0031d368: ldr r3, [r0, #0x10]
0031d36c: add r3, r3, #4
0031d370: str r3, [r0, #0x10]
0031d374: ldr r0, [sp, #0xc]
0031d378: ldm r0, {r1, r2}
0031d37c: cmp r1, r2
0031d380: beq #0x31d38c
0031d384: add r3, sp, #0x108
0031d388: bl #0x31c3cc
0031d38c: add sp, sp, #0x114
0031d390: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031d394: ldr ip, [r4, #0x1c]
0031d398: ldr lr, [r4, #0x18]
0031d39c: add r0, sp, #0x40
0031d3a0: str ip, [sp, #0x2c]
0031d3a4: ldr ip, [r4, #0x14]
0031d3a8: add r3, sp, #0x10
0031d3ac: add r1, sp, #0x30
0031d3b0: str ip, [sp, #0x24]
0031d3b4: ldr ip, [r4, #0x10]
0031d3b8: add r2, sp, #0x20
0031d3bc: str sl, [sp, #0x3c]
0031d3c0: str ip, [sp, #0x20]
0031d3c4: ldr ip, [sp, #0xcc]
0031d3c8: str r7, [sp, #0x38]
0031d3cc: str r8, [sp, #0x34]
0031d3d0: str ip, [sp, #0x1c]
0031d3d4: ldr ip, [sp, #0xc8]
0031d3d8: str fp, [sp, #0x30]
0031d3dc: str lr, [sp, #0x28]
0031d3e0: str ip, [sp, #0x18]
0031d3e4: ldr ip, [sp, #0xc4]
0031d3e8: str ip, [sp, #0x14]
0031d3ec: ldr ip, [sp, #0xc0]
0031d3f0: str ip, [sp, #0x10]
0031d3f4: add ip, sp, #0x100
0031d3f8: str ip, [sp]
0031d3fc: mov ip, #0
0031d400: str ip, [sp, #4]
0031d404: bl #0x31b9d8
0031d408: ldr r0, [r4, #0x10]
0031d40c: ldr r3, [r4, #0x14]
0031d410: cmp r0, r3
0031d414: subne r0, r0, #4
0031d418: strne r0, [r4, #0x10]
0031d41c: bne #0x31d328
0031d420: cmp r0, #0
0031d424: beq #0x31d430
0031d428: mov r1, #0x80
0031d42c: bl #0x31bb44
0031d430: ldr r3, [r5, r6]
0031d434: ldr r2, [r3, #0x1c]
0031d438: sub r1, r2, #4
0031d43c: str r1, [r3, #0x1c]
0031d440: ldr r2, [r2, #-4]
0031d444: add r0, r2, #0x7c
0031d448: add r1, r2, #0x80
0031d44c: str r0, [r3, #0x10]
0031d450: str r1, [r3, #0x18]
0031d454: str r2, [r3, #0x14]
0031d458: b #0x31d328
0031d45c: ldr r0, [r4, #4]
0031d460: cmp r0, #0
0031d464: beq #0x31d470
0031d468: mov r1, #0x80
0031d46c: bl #0x708f00
0031d470: ldr r3, [r5, r6]
0031d474: ldr r2, [r3, #0xc]
0031d478: add r1, r2, #4
0031d47c: str r1, [r3, #0xc]
0031d480: ldr r2, [r2, #4]
0031d484: add r1, r2, #0x80
0031d488: str r2, [r3]
0031d48c: str r1, [r3, #8]
0031d490: str r2, [r3, #4]
0031d494: b #0x31d328
0031d498: ldr r2, [pc, #0xc0]
0031d49c: ldr r2, [r5, r2]
0031d4a0: ldr r2, [r2]
0031d4a4: cmp r2, #2
0031d4a8: moveq r2, #0
0031d4ac: streq r2, [r2]
0031d4b0: beq #0x31d228
0031d4b4: cmp r2, #1
0031d4b8: bne #0x31d228
0031d4bc: ldr r0, [pc, #0xa0]
0031d4c0: ldr r1, [pc, #0xa0]
0031d4c4: ldr r2, [pc, #0xa0]
0031d4c8: ldr r0, [r5, r0]
0031d4cc: ldr r3, [pc, #0x9c]
0031d4d0: mov ip, #0x42
0031d4d4: add r1, pc, r1
0031d4d8: add r3, pc, r3
0031d4dc: add r0, r0, #0xa8
0031d4e0: add r2, pc, r2
0031d4e4: str ip, [sp]
0031d4e8: bl #0x30e004
0031d4ec: ldr r3, [sp, #0xf0]
0031d4f0: b #0x31d228
0031d4f4: ldr r2, [pc, #0x64]
0031d4f8: ldr r2, [r5, r2]
0031d4fc: ldr r2, [r2]
0031d500: cmp r2, #2
0031d504: streq r3, [r3]
0031d508: beq #0x31d1b0
0031d50c: cmp r2, #1
0031d510: bne #0x31d1b0
0031d514: ldr r0, [pc, #0x48]
0031d518: ldr r1, [pc, #0x54]
0031d51c: ldr r2, [pc, #0x54]
0031d520: ldr r0, [r5, r0]
0031d524: ldr r3, [pc, #0x50]
0031d528: mov ip, #0x3e
0031d52c: add r1, pc, r1
0031d530: add r2, pc, r2
0031d534: add r3, pc, r3
0031d538: add r0, r0, #0xa8
0031d53c: str ip, [sp]
0031d540: bl #0x30e004
0031d544: b #0x31d1b0
0031d548: ldr r1, [sp, #8]
0031d54c: bl #0x31cd00
0031d550: b #0x31d374
0031d554: rsbeq r7, r7, r8, ror #17
0031d558: strheq r0, [r0], -r4
0031d55c: andeq r0, r0, r0, lsr #19
0031d560: andeq r3, r0, r0, asr #19
0031d564: andeq r1, r0, r0, asr #19
0031d568: subseq r0, sl, r4, lsl #30
0031d56c: subseq r1, sl, r0, lsr #9
0031d570: subseq r1, sl, r8, asr r4
0031d574: subseq r0, sl, ip, lsr #29
0031d578: subseq r1, sl, r8, asr #8
0031d57c: ldrsheq r1, [sl], #-0x3c

_ZN3sfc6script3lua9ArgumentsC2Ev 0x3192ec 56
003192ec: ldr r3, [pc, #0x28]
003192f0: ldr r2, [pc, #0x28]
003192f4: push {r4, lr}
003192f8: add r3, pc, r3
003192fc: ldr r2, [r3, r2]
00319300: mov r4, r0
00319304: add r2, r2, #8
00319308: str r2, [r0]
0031930c: bl #0x31ce84
00319310: str r0, [r4, #4]
00319314: mov r0, r4
00319318: pop {r4, pc}
0031931c: mlseq r7, r8, r7, fp
00319320: andeq r2, r0, r0, ror #6

_ZN9LuaScript5_RandERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37df30 312
0037df30: push {r4, r5, r6, lr}
0037df34: ldr r5, [r0, #4]
0037df38: mov r6, r1
0037df3c: mov r4, r0
0037df40: ldm r5, {r1, r3}
0037df44: rsb r3, r1, r3
0037df48: asr r3, r3, #4
0037df4c: add r2, r3, r3, lsl #3
0037df50: add r2, r2, r2, lsl #6
0037df54: add r2, r3, r2, lsl #3
0037df58: add r2, r2, r2, lsl #15
0037df5c: add r3, r3, r2, lsl #3
0037df60: rsb r3, r3, #0
0037df64: cmp r3, #1
0037df68: bls #0x37e048
0037df6c: cmp r3, #0
0037df70: beq #0x37e034
0037df74: ldr r3, [r1, #4]
0037df78: cmp r3, #3
0037df7c: beq #0x37e04c
0037df80: ldr r5, [r4, #4]
0037df84: ldm r5, {r0, r3}
0037df88: rsb r3, r0, r3
0037df8c: asr r3, r3, #4
0037df90: add r2, r3, r3, lsl #3
0037df94: add r2, r2, r2, lsl #6
0037df98: add r2, r3, r2, lsl #3
0037df9c: add r2, r2, r2, lsl #15
0037dfa0: add r3, r3, r2, lsl #3
0037dfa4: cmp r3, #0
0037dfa8: beq #0x37e020
0037dfac: bl #0x31bbf0
0037dfb0: bl #0x30e4cc
0037dfb4: ldr r4, [r4, #4]
0037dfb8: mov r5, r0
0037dfbc: ldm r4, {r0, r3}
0037dfc0: rsb r3, r0, r3
0037dfc4: asr r3, r3, #4
0037dfc8: add r2, r3, r3, lsl #3
0037dfcc: add r2, r2, r2, lsl #6
0037dfd0: add r2, r3, r2, lsl #3
0037dfd4: add r2, r2, r2, lsl #15
0037dfd8: add r3, r3, r2, lsl #3
0037dfdc: rsb r3, r3, #0
0037dfe0: cmp r3, #1
0037dfe4: bls #0x37e00c
0037dfe8: add r0, r0, #0x70
0037dfec: bl #0x31bbf0
0037dff0: bl #0x30e4cc
0037dff4: rsb r0, r5, r0
0037dff8: bl #0x37bc2c
0037dffc: add r1, r0, r5
0037e000: mov r0, r6
0037e004: pop {r4, r5, r6, lr}
0037e008: b #0x37cb24
0037e00c: ldr r0, [pc, #0x48]
0037e010: add r0, pc, r0
0037e014: bl #0x708eb0
0037e018: ldr r0, [r4]
0037e01c: b #0x37dfe8
0037e020: ldr r0, [pc, #0x38]
0037e024: add r0, pc, r0
0037e028: bl #0x708eb0
0037e02c: ldr r0, [r5]
0037e030: b #0x37dfac
0037e034: ldr r0, [pc, #0x28]
0037e038: add r0, pc, r0
0037e03c: bl #0x708eb0
0037e040: ldr r1, [r5]
0037e044: b #0x37df74
0037e048: pop {r4, r5, r6, pc}
0037e04c: mov r0, r4
0037e050: mov r1, #1
0037e054: bl #0x37baf8
0037e058: b #0x37df80
0037e05c: subseq r0, r4, r8, asr r4
0037e060: subseq r0, r4, r4, asr #8
0037e064: subseq r0, r4, r0, lsr r4

_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE8_M_eraseEPS3_S6_RKSt12__false_type 0x31c3cc 160
0031c3cc: push {r4, r5, r6, r7, r8, sb, sl, lr}
0031c3d0: ldr r4, [r0, #4]
0031c3d4: mov r5, r0
0031c3d8: mov r8, r2
0031c3dc: rsb r3, r2, r4
0031c3e0: asr r3, r3, #4
0031c3e4: mov r7, r1
0031c3e8: add sl, r3, r3, lsl #3
0031c3ec: add sl, sl, sl, lsl #6
0031c3f0: add sl, r3, sl, lsl #3
0031c3f4: add sl, sl, sl, lsl #15
0031c3f8: add sl, r3, sl, lsl #3
0031c3fc: rsb sl, sl, #0
0031c400: cmp sl, #0
0031c404: movle sl, r1
0031c408: ble #0x31c438
0031c40c: mov r6, sl
0031c410: mov r4, #0
0031c414: add r0, r7, r4
0031c418: add r1, r8, r4
0031c41c: bl #0x31c368
0031c420: subs r6, r6, #1
0031c424: add r4, r4, #0x70
0031c428: bne #0x31c414
0031c42c: mov r3, #0x70
0031c430: mla sl, r3, sl, r7
0031c434: ldr r4, [r5, #4]
0031c438: cmp sl, r4
0031c43c: beq #0x31c460
0031c440: mov r6, sl
0031c444: ldr r3, [r6]
0031c448: mov r0, r6
0031c44c: add r6, r6, #0x70
0031c450: mov lr, pc
0031c454: ldr pc, [r3]
0031c458: cmp r6, r4
0031c45c: bne #0x31c444
0031c460: str sl, [r5, #4]
0031c464: mov r0, r7
0031c468: pop {r4, r5, r6, r7, r8, sb, sl, pc}

_ZN3sfc6script3lua8Instance12includeTableEv 0x31aff8 8
0031aff8: ldr r0, [r0, #4]
0031affc: b #0x85b174

_ZN3sfc6script3lua8Instance16registerFunctionEPKcPFiP9lua_StateERKNS1_9ArgumentsE 0x31af08 180
0031af08: push {r4, r5, r6, r7, r8, sb, sl, lr}
0031af0c: mov r5, r3
0031af10: ldr r3, [r3, #4]
0031af14: mov r4, r0
0031af18: mov r6, r1
0031af1c: ldm r3, {r0, r1}
0031af20: mov sl, r2
0031af24: rsb r1, r0, r1
0031af28: asr r1, r1, #4
0031af2c: add r2, r1, r1, lsl #3
0031af30: add r2, r2, r2, lsl #6
0031af34: add r2, r1, r2, lsl #3
0031af38: add r2, r2, r2, lsl #15
0031af3c: add r2, r1, r2, lsl #3
0031af40: rsb r2, r2, #0
0031af44: cmp r2, #0
0031af48: beq #0x31af98
0031af4c: mov r7, #0
0031af50: mov r8, r7
0031af54: add r0, r0, r7
0031af58: ldr r1, [r4, #4]
0031af5c: bl #0x31cac4
0031af60: ldr r1, [r5, #4]
0031af64: add r8, r8, #1
0031af68: add r7, r7, #0x70
0031af6c: ldm r1, {r0, r3}
0031af70: rsb r3, r0, r3
0031af74: asr r3, r3, #4
0031af78: add r2, r3, r3, lsl #3
0031af7c: add r2, r2, r2, lsl #6
0031af80: add r2, r3, r2, lsl #3
0031af84: add r2, r2, r2, lsl #15
0031af88: add r2, r3, r2, lsl #3
0031af8c: rsb r2, r2, #0
0031af90: cmp r8, r2
0031af94: blo #0x31af54
0031af98: mov r1, sl
0031af9c: ldr r0, [r4, #4]
0031afa0: bl #0x84bcdc
0031afa4: ldr r0, [r4, #4]
0031afa8: mvn r1, #0x2700
0031afac: sub r1, r1, #0x11
0031afb0: mov r2, r6
0031afb4: pop {r4, r5, r6, r7, r8, sb, sl, lr}
0031afb8: b #0x84c080

_ZN10GameObject7_SummonERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x39193c 2032
0039193c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00391940: ldr r8, [r0, #4]
00391944: mov r7, r1
00391948: mov r6, r2
0039194c: ldr r3, [r8]
00391950: ldr r1, [r8, #4]
00391954: ldr r4, [pc, #0x7ac]
00391958: sub sp, sp, #0x5c
0039195c: rsb r1, r3, r1
00391960: asr r1, r1, #4
00391964: add r4, pc, r4
00391968: add r2, r1, r1, lsl #3
0039196c: mov r5, r0
00391970: add r2, r2, r2, lsl #6
00391974: add r2, r1, r2, lsl #3
00391978: add r2, r2, r2, lsl #15
0039197c: add r1, r1, r2, lsl #3
00391980: rsb r1, r1, #0
00391984: cmp r1, #1
00391988: bls #0x3919a0
0039198c: cmp r1, #0
00391990: beq #0x3919a8
00391994: ldr r3, [r3, #4]
00391998: cmp r3, #3
0039199c: beq #0x3919bc
003919a0: add sp, sp, #0x5c
003919a4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003919a8: ldr r0, [pc, #0x75c]
003919ac: add r0, pc, r0
003919b0: bl #0x708eb0
003919b4: ldr r3, [r8]
003919b8: b #0x391994
003919bc: mov r1, #0
003919c0: mov r0, r5
003919c4: bl #0x37baf8
003919c8: bl #0x38d798
003919cc: ldr r3, [pc, #0x73c]
003919d0: ldr r3, [r4, r3]
003919d4: ldr r3, [r3]
003919d8: cmp r0, r3
003919dc: bhs #0x3919a0
003919e0: ldr r8, [r5, #4]
003919e4: ldr r3, [r8]
003919e8: ldr r2, [r8, #4]
003919ec: rsb r2, r3, r2
003919f0: asr r2, r2, #4
003919f4: add r1, r2, r2, lsl #3
003919f8: add r1, r1, r1, lsl #6
003919fc: add r1, r2, r1, lsl #3
00391a00: add r1, r1, r1, lsl #15
00391a04: add r2, r2, r1, lsl #3
00391a08: rsb r2, r2, #0
00391a0c: cmp r2, #1
00391a10: bhi #0x391a24
00391a14: ldr r0, [pc, #0x6f8]
00391a18: add r0, pc, r0
00391a1c: bl #0x708eb0
00391a20: ldr r3, [r8]
00391a24: ldr r1, [r3, #0x74]
00391a28: cmp r1, #1
00391a2c: bne #0x3919a0
00391a30: mov r0, r5
00391a34: bl #0x37baf8
00391a38: bl #0x31bc80
00391a3c: ldr ip, [r6, #0x16c]
00391a40: ldr r1, [r6, #0x174]
00391a44: mov sb, r0
00391a48: ldr r0, [r6, #0x170]
00391a4c: ldr r2, [r5, #4]
00391a50: mov r3, #0
00391a54: str r3, [sp, #0x54]
00391a58: str ip, [sp, #0x40]
00391a5c: str r0, [sp, #0x44]
00391a60: str r1, [sp, #0x48]
00391a64: str r3, [sp, #0x4c]
00391a68: str r3, [sp, #0x50]
00391a6c: ldr r3, [r2]
00391a70: ldr r2, [r2, #4]
00391a74: rsb r3, r3, r2
00391a78: asr r3, r3, #4
00391a7c: add r2, r3, r3, lsl #3
00391a80: add r2, r2, r2, lsl #6
00391a84: add r2, r3, r2, lsl #3
00391a88: add r2, r2, r2, lsl #15
00391a8c: add r3, r3, r2, lsl #3
00391a90: rsb r3, r3, #0
00391a94: cmp r3, #2
00391a98: bhi #0x391d08
00391a9c: ldr r0, [r6, #0x2d8]
00391aa0: cmp r0, #0
00391aa4: beq #0x391e20
00391aa8: ldr r1, [pc, #0x668]
00391aac: add r1, pc, r1
00391ab0: bl #0x470a18
00391ab4: subs r1, r0, #0
00391ab8: beq #0x391e20
00391abc: add r0, sp, #0x28
00391ac0: bl #0x597180
00391ac4: ldr r3, [sp, #0x28]
00391ac8: str r3, [sp, #0x4c]
00391acc: ldr r3, [sp, #0x2c]
00391ad0: str r3, [sp, #0x50]
00391ad4: ldr r3, [sp, #0x30]
00391ad8: str r3, [sp, #0x54]
00391adc: ldr r2, [r5, #4]
00391ae0: ldr r3, [r2]
00391ae4: ldr r2, [r2, #4]
00391ae8: rsb r3, r3, r2
00391aec: asr r3, r3, #4
00391af0: add r2, r3, r3, lsl #3
00391af4: add r2, r2, r2, lsl #6
00391af8: add r2, r3, r2, lsl #3
00391afc: add r2, r2, r2, lsl #15
00391b00: add r3, r3, r2, lsl #3
00391b04: cmn r3, #3
00391b08: beq #0x391cd4
00391b0c: mov sl, #0
00391b10: ldr r3, [pc, #0x604]
00391b14: mov ip, #0
00391b18: add r8, sp, #0x4c
00391b1c: ldr r0, [r4, r3]
00391b20: mov r2, ip
00391b24: mov r1, r8
00391b28: mov r3, ip
00391b2c: str ip, [sp]
00391b30: str ip, [sp, #4]
00391b34: str ip, [sp, #8]
00391b38: bl #0x525508
00391b3c: cmp r0, #0
00391b40: bne #0x391b5c
00391b44: ldr r1, [r6, #0x160]
00391b48: ldr r2, [r6, #0x164]
00391b4c: ldr r3, [r6, #0x168]
00391b50: str r1, [sp, #0x4c]
00391b54: str r2, [sp, #0x50]
00391b58: str r3, [sp, #0x54]
00391b5c: mov r1, #0
00391b60: mov r0, r5
00391b64: bl #0x37baf8
00391b68: bl #0x38d798
00391b6c: mov r1, #0
00391b70: mov r2, r1
00391b74: mov fp, r0
00391b78: bl #0x3ad1f8
00391b7c: subs r5, r0, #0
00391b80: beq #0x3919a0
00391b84: mov r1, r8
00391b88: bl #0x3a58f4
00391b8c: mov r2, #1
00391b90: mov r0, r5
00391b94: mov r1, r8
00391b98: bl #0x393db4
00391b9c: mov r0, r5
00391ba0: add r1, sp, #0x40
00391ba4: bl #0x3938a0
00391ba8: mov r2, #1
00391bac: movw r3, #0x14e4
00391bb0: strb r2, [r5, r3]
00391bb4: ldr r0, [r6, #0x2f4]
00391bb8: cmp r0, #0
00391bbc: beq #0x391bd0
00391bc0: mov r1, r5
00391bc4: bl #0x396a90
00391bc8: cmp r0, #0
00391bcc: bne #0x391bf4
00391bd0: ldr r3, [pc, #0x548]
00391bd4: mov r1, r5
00391bd8: ldr r3, [r4, r3]
00391bdc: ldr r0, [r3, #0x38]
00391be0: bl #0x344184
00391be4: mov r3, #1
00391be8: strb r3, [r5, #0x2ef]
00391bec: mov r0, r5
00391bf0: bl #0x38c710
00391bf4: cmp sb, #0
00391bf8: bne #0x391e08
00391bfc: mov r0, r7
00391c00: mov r1, r5
00391c04: bl #0x37c9f8
00391c08: cmp sl, #0
00391c0c: ble #0x391c28
00391c10: ldr r3, [r5, #0x2d8]
00391c14: cmp r3, #0
00391c18: beq #0x391c28
00391c1c: ldr r0, [r3, #8]
00391c20: mov r1, sl
00391c24: bl #0x35c09c
00391c28: bl #0x7fd794
00391c2c: ldrb r3, [r0, #5]
00391c30: cmp r3, #0
00391c34: beq #0x3919a0
00391c38: ldr r3, [pc, #0x4e0]
00391c3c: mov r1, r5
00391c40: mov r7, #1
00391c44: ldr r3, [r4, r3]
00391c48: ldr r0, [r3, #0x38]
00391c4c: bl #0x3431c0
00391c50: ldr r3, [r6, #0x110]
00391c54: strb r7, [r5, #0x118]
00391c58: str r3, [r5, #0x110]
00391c5c: mov r3, #0
00391c60: str r3, [r5, #0x114]
00391c64: bl #0x320e98
00391c68: ldr r3, [r0, #0x34]
00391c6c: sub r3, r3, #3
00391c70: cmp r3, r7
00391c74: bhi #0x3919a0
00391c78: ldr sl, [r6, #0x108]
00391c7c: ldr sb, [r5, #0x108]
00391c80: ldr r6, [r5, #0x168]
00391c84: ldr r8, [r5, #0x160]
00391c88: ldr r5, [r5, #0x164]
00391c8c: bl #0x80b1bc
00391c90: mov r4, r0
00391c94: ldr r0, [pc, #0x488]
00391c98: mov r1, r7
00391c9c: add r0, pc, r0
00391ca0: bl #0x80a244
00391ca4: mov r3, #0
00391ca8: mov r1, r0
00391cac: str sb, [r0, #0x50]
00391cb0: str fp, [r0, #0x54]
00391cb4: str sl, [r0, #0x58]
00391cb8: str r8, [r0, #0x5c]
00391cbc: str r5, [r0, #0x60]
00391cc0: str r6, [r0, #0x64]
00391cc4: strb r3, [r0, #0x68]
00391cc8: mov r0, r4
00391ccc: bl #0x80e2a4
00391cd0: b #0x3919a0
00391cd4: mov r0, r5
00391cd8: mov r1, #2
00391cdc: bl #0x37baf8
00391ce0: ldr r3, [r0, #4]
00391ce4: cmp r3, #3
00391ce8: bne #0x391b0c
00391cec: mov r1, #2
00391cf0: mov r0, r5
00391cf4: bl #0x37baf8
00391cf8: bl #0x31bbf0
00391cfc: bl #0x30e4cc
00391d00: mov sl, r0
00391d04: b #0x391b10
00391d08: mov r0, r5
00391d0c: mov r1, #2
00391d10: bl #0x37baf8
00391d14: ldr r3, [r0, #4]
00391d18: cmp r3, #7
00391d1c: beq #0x3920ac
00391d20: ldr r3, [r5, #4]
00391d24: ldr r2, [r3, #4]
00391d28: ldr r3, [r3]
00391d2c: rsb r3, r3, r2
00391d30: asr r3, r3, #4
00391d34: add r2, r3, r3, lsl #3
00391d38: add r2, r2, r2, lsl #6
00391d3c: add r2, r3, r2, lsl #3
00391d40: add r2, r2, r2, lsl #15
00391d44: add r3, r3, r2, lsl #3
00391d48: rsb r3, r3, #0
00391d4c: cmp r3, #4
00391d50: bls #0x391a9c
00391d54: mov r1, #2
00391d58: mov r0, r5
00391d5c: bl #0x37baf8
00391d60: ldr r1, [r0, #4]
00391d64: cmp r1, #3
00391d68: bne #0x391a9c
00391d6c: mov r0, r5
00391d70: bl #0x37baf8
00391d74: ldr r3, [r0, #4]
00391d78: cmp r3, #3
00391d7c: bne #0x391a9c
00391d80: mov r0, r5
00391d84: mov r1, #4
00391d88: bl #0x37baf8
00391d8c: ldr r3, [r0, #4]
00391d90: cmp r3, #3
00391d94: bne #0x391a9c
00391d98: ldr r2, [r5, #4]
00391d9c: movw r3, #0x6db7
00391da0: movt r3, #0xb6db
00391da4: ldm r2, {r1, r2}
00391da8: rsb r2, r1, r2
00391dac: asr r2, r2, #4
00391db0: mul r3, r3, r2
00391db4: cmp r3, #5
00391db8: bhi #0x391e3c
00391dbc: mov r1, #2
00391dc0: mov r0, r5
00391dc4: bl #0x37baf8
00391dc8: bl #0x31bbf0
00391dcc: mov r1, #3
00391dd0: mov sl, r0
00391dd4: mov r0, r5
00391dd8: bl #0x37baf8
00391ddc: bl #0x31bbf0
00391de0: mov r1, #4
00391de4: mov r8, r0
00391de8: mov r0, r5
00391dec: bl #0x37baf8
00391df0: bl #0x31bbf0
00391df4: str sl, [sp, #0x4c]
00391df8: str r8, [sp, #0x50]
00391dfc: str r0, [sp, #0x54]
00391e00: mov sl, #0
00391e04: b #0x391b10
00391e08: add r0, r5, #0x4f0
00391e0c: mov r1, #0
00391e10: add r0, r0, #0xc
00391e14: mov r2, r1
00391e18: bl #0x3c2734
00391e1c: b #0x391bfc
00391e20: ldr r1, [r6, #0x160]
00391e24: ldr r2, [r6, #0x164]
00391e28: ldr r3, [r6, #0x168]
00391e2c: str r1, [sp, #0x4c]
00391e30: str r2, [sp, #0x50]
00391e34: str r3, [sp, #0x54]
00391e38: b #0x391adc
00391e3c: mov r0, r5
00391e40: mov r1, #5
00391e44: bl #0x37baf8
00391e48: ldr r3, [r0, #4]
00391e4c: cmp r3, #1
00391e50: bne #0x391dbc
00391e54: mov r1, #5
00391e58: mov r0, r5
00391e5c: bl #0x37baf8
00391e60: bl #0x31bc80
00391e64: cmp r0, #0
00391e68: beq #0x391dbc
00391e6c: mov r3, #0
00391e70: mov r0, r6
00391e74: add r1, sp, #0x34
00391e78: str r3, [sp, #0x3c]
00391e7c: str r3, [sp, #0x34]
00391e80: str r3, [sp, #0x38]
00391e84: bl #0x393ae4
00391e88: ldr r3, [pc, #0x298]
00391e8c: ldr ip, [r6, #0x160]
00391e90: ldr r2, [r6, #0x164]
00391e94: ldr r8, [r4, r3]
00391e98: ldr r3, [r6, #0x168]
00391e9c: mov r1, #2
00391ea0: mov r0, r5
00391ea4: str r3, [sp, #0x54]
00391ea8: ldr r3, [r8, #4]
00391eac: str ip, [sp, #0x4c]
00391eb0: str r2, [sp, #0x50]
00391eb4: str r3, [sp, #0x14]
00391eb8: ldr r3, [sp, #0x3c]
00391ebc: ldr fp, [r8, #8]
00391ec0: str r3, [sp, #0x18]
00391ec4: ldr r3, [sp, #0x38]
00391ec8: str r3, [sp, #0x1c]
00391ecc: ldr r3, [sp, #0x34]
00391ed0: str r3, [sp, #0x20]
00391ed4: ldr r3, [r8]
00391ed8: str r3, [sp, #0x24]
00391edc: bl #0x37baf8
00391ee0: bl #0x31bbf0
00391ee4: ldr r1, [sp, #0x18]
00391ee8: mov sl, r0
00391eec: ldr r0, [sp, #0x14]
00391ef0: bl #0x30ed6c
00391ef4: ldr r1, [sp, #0x1c]
00391ef8: mov r3, r0
00391efc: mov r0, fp
00391f00: str r3, [sp, #0x10]
00391f04: bl #0x30ed6c
00391f08: ldr r3, [sp, #0x10]
00391f0c: mov r1, r0
00391f10: mov r0, r3
00391f14: bl #0x30e3ac
00391f18: mov r1, r0
00391f1c: mov r0, sl
00391f20: bl #0x30ed6c
00391f24: mov r1, r0
00391f28: ldr r0, [sp, #0x4c]
00391f2c: bl #0x30eba4
00391f30: ldr r1, [sp, #0x20]
00391f34: str r0, [sp, #0x4c]
00391f38: mov r0, fp
00391f3c: bl #0x30ed6c
00391f40: ldr r1, [sp, #0x24]
00391f44: mov fp, r0
00391f48: ldr r0, [sp, #0x18]
00391f4c: bl #0x30ed6c
00391f50: mov r1, r0
00391f54: mov r0, fp
00391f58: bl #0x30e3ac
00391f5c: mov r1, r0
00391f60: mov r0, sl
00391f64: bl #0x30ed6c
00391f68: mov r1, r0
00391f6c: ldr r0, [sp, #0x50]
00391f70: bl #0x30eba4
00391f74: ldr r1, [sp, #0x24]
00391f78: str r0, [sp, #0x50]
00391f7c: ldr r0, [sp, #0x1c]
00391f80: bl #0x30ed6c
00391f84: ldr r1, [sp, #0x20]
00391f88: mov fp, r0
00391f8c: ldr r0, [sp, #0x14]
00391f90: bl #0x30ed6c
00391f94: mov r1, r0
00391f98: mov r0, fp
00391f9c: bl #0x30e3ac
00391fa0: mov r1, r0
00391fa4: mov r0, sl
00391fa8: bl #0x30ed6c
00391fac: mov r1, r0
00391fb0: ldr r0, [sp, #0x54]
00391fb4: bl #0x30eba4
00391fb8: mov r1, #3
00391fbc: str r0, [sp, #0x54]
00391fc0: mov r0, r5
00391fc4: bl #0x37baf8
00391fc8: bl #0x31bbf0
00391fcc: ldr r1, [sp, #0x38]
00391fd0: mov sl, r0
00391fd4: bl #0x30ed6c
00391fd8: ldr r1, [sp, #0x3c]
00391fdc: mov fp, r0
00391fe0: mov r0, sl
00391fe4: bl #0x30ed6c
00391fe8: ldr r1, [sp, #0x34]
00391fec: mov r3, r0
00391ff0: mov r0, sl
00391ff4: str r3, [sp, #0x10]
00391ff8: bl #0x30ed6c
00391ffc: mov r1, r0
00392000: ldr r0, [sp, #0x4c]
00392004: bl #0x30eba4
00392008: mov r1, fp
0039200c: str r0, [sp, #0x4c]
00392010: ldr r0, [sp, #0x50]
00392014: bl #0x30eba4
00392018: ldr r3, [sp, #0x10]
0039201c: str r0, [sp, #0x50]
00392020: ldr r0, [sp, #0x54]
00392024: mov r1, r3
00392028: bl #0x30eba4
0039202c: mov r1, #4
00392030: str r0, [sp, #0x54]
00392034: mov r0, r5
00392038: bl #0x37baf8
0039203c: bl #0x31bbf0
00392040: ldr r1, [r8, #4]
00392044: mov sl, r0
00392048: bl #0x30ed6c
0039204c: ldr r1, [r8, #8]
00392050: mov fp, r0
00392054: mov r0, sl
00392058: bl #0x30ed6c
0039205c: ldr r1, [r8]
00392060: mov r3, r0
00392064: mov r0, sl
00392068: str r3, [sp, #0x10]
0039206c: bl #0x30ed6c
00392070: mov r1, r0
00392074: ldr r0, [sp, #0x4c]
00392078: bl #0x30eba4
0039207c: mov r1, fp
00392080: str r0, [sp, #0x4c]
00392084: ldr r0, [sp, #0x50]
00392088: bl #0x30eba4
0039208c: ldr r3, [sp, #0x10]
00392090: str r0, [sp, #0x50]
00392094: ldr r0, [sp, #0x54]
00392098: mov r1, r3
0039209c: bl #0x30eba4
003920a0: mov sl, #0
003920a4: str r0, [sp, #0x54]
003920a8: b #0x391b10
003920ac: mov r1, #2
003920b0: mov r0, r5
003920b4: bl #0x37baf8
003920b8: bl #0x31b5a0
003920bc: ldr r2, [r0, #0x160]
003920c0: mov r3, r0
003920c4: mov r1, #2
003920c8: str r2, [sp, #0x4c]
003920cc: ldr r2, [r3, #0x164]
003920d0: mov r0, r5
003920d4: mov sl, #0
003920d8: str r2, [sp, #0x50]
003920dc: ldr r3, [r3, #0x168]
003920e0: str r3, [sp, #0x54]
003920e4: bl #0x37baf8
003920e8: bl #0x31b5a0
003920ec: ldr r3, [r0, #0x16c]
003920f0: str r3, [sp, #0x40]
003920f4: ldr r3, [r0, #0x170]
003920f8: str r3, [sp, #0x44]
003920fc: ldr r3, [r0, #0x174]
00392100: str r3, [sp, #0x48]
00392104: b #0x391b10
00392108: rsbeq r3, r0, ip, lsr #2
0039210c: ldrheq ip, [r2], #-0xac
00392110: andeq r4, r0, r4, lsl #4
00392114: subseq ip, r2, r0, asr sl
00392118: subseq r0, r3, ip, lsr #27
0039211c: andeq r1, r0, r4, lsl #4
00392120: strdeq r3, r4, [r0], -r4
00392124: subseq sp, r2, ip, asr #3
00392128: andeq r4, r0, r0, asr #6

_ZN10GameObject18_GetTargetListSizeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38e930 64
0038e930: push {r4, lr}
0038e934: sub sp, sp, #0x10
0038e938: mov lr, r2
0038e93c: mov ip, sp
0038e940: add r3, r2, #0x304
0038e944: mov r4, r1
0038e948: ldm r3, {r0, r1, r2, r3}
0038e94c: stm ip, {r0, r1, r2, r3}
0038e950: mov r1, sp
0038e954: add r0, lr, #0x314
0038e958: bl #0x38d610
0038e95c: mov r1, r0
0038e960: mov r0, r4
0038e964: bl #0x37cb24
0038e968: add sp, sp, #0x10
0038e96c: pop {r4, pc}

_ZN9LuaScript6_RandFERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e2d8 328
0037e2d8: push {r4, r5, r6, lr}
0037e2dc: ldr r5, [r0, #4]
0037e2e0: mov r6, r1
0037e2e4: mov r4, r0
0037e2e8: ldm r5, {r1, r3}
0037e2ec: rsb r3, r1, r3
0037e2f0: asr r3, r3, #4
0037e2f4: add r2, r3, r3, lsl #3
0037e2f8: add r2, r2, r2, lsl #6
0037e2fc: add r2, r3, r2, lsl #3
0037e300: add r2, r2, r2, lsl #15
0037e304: add r3, r3, r2, lsl #3
0037e308: rsb r3, r3, #0
0037e30c: cmp r3, #1
0037e310: bls #0x37e400
0037e314: cmp r3, #0
0037e318: beq #0x37e3ec
0037e31c: ldr r3, [r1, #4]
0037e320: cmp r3, #3
0037e324: beq #0x37e404
0037e328: ldr r5, [r4, #4]
0037e32c: ldm r5, {r0, r3}
0037e330: rsb r3, r0, r3
0037e334: asr r3, r3, #4
0037e338: add r2, r3, r3, lsl #3
0037e33c: add r2, r2, r2, lsl #6
0037e340: add r2, r3, r2, lsl #3
0037e344: add r2, r2, r2, lsl #15
0037e348: add r3, r3, r2, lsl #3
0037e34c: cmp r3, #0
0037e350: beq #0x37e3d8
0037e354: bl #0x31bbf0
0037e358: ldr r4, [r4, #4]
0037e35c: mov r5, r0
0037e360: ldm r4, {r0, r3}
0037e364: rsb r3, r0, r3
0037e368: asr r3, r3, #4
0037e36c: add r2, r3, r3, lsl #3
0037e370: add r2, r2, r2, lsl #6
0037e374: add r2, r3, r2, lsl #3
0037e378: add r2, r2, r2, lsl #15
0037e37c: add r3, r3, r2, lsl #3
0037e380: rsb r3, r3, #0
0037e384: cmp r3, #1
0037e388: bls #0x37e3c4
0037e38c: add r0, r0, #0x70
0037e390: bl #0x31bbf0
0037e394: mov r1, r5
0037e398: bl #0x30e3ac
0037e39c: bl #0x30e4cc
0037e3a0: bl #0x37bc2c
0037e3a4: bl #0x30e964
0037e3a8: mov r1, r0
0037e3ac: mov r0, r5
0037e3b0: bl #0x30eba4
0037e3b4: mov r1, r0
0037e3b8: mov r0, r6
0037e3bc: pop {r4, r5, r6, lr}
0037e3c0: b #0x37ccbc
0037e3c4: ldr r0, [pc, #0x48]
0037e3c8: add r0, pc, r0
0037e3cc: bl #0x708eb0
0037e3d0: ldr r0, [r4]
0037e3d4: b #0x37e38c
0037e3d8: ldr r0, [pc, #0x38]
0037e3dc: add r0, pc, r0
0037e3e0: bl #0x708eb0
0037e3e4: ldr r0, [r5]
0037e3e8: b #0x37e354
0037e3ec: ldr r0, [pc, #0x28]
0037e3f0: add r0, pc, r0
0037e3f4: bl #0x708eb0
0037e3f8: ldr r1, [r5]
0037e3fc: b #0x37e31c
0037e400: pop {r4, r5, r6, pc}
0037e404: mov r0, r4
0037e408: mov r1, #1
0037e40c: bl #0x37baf8
0037e410: b #0x37e328
0037e414: subseq r0, r4, r0, lsr #1
0037e418: subseq r0, r4, ip, lsl #1
0037e41c: subseq r0, r4, r8, ror r0

_ZN9Character24_SetSkillCooldownTimerIdERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b97e0 496
003b97e0: push {r4, r5, r6, r7, r8, lr}
003b97e4: ldr r5, [r0, #4]
003b97e8: mov r6, r2
003b97ec: mov r4, r0
003b97f0: ldm r5, {r2, r3}
003b97f4: rsb r3, r2, r3
003b97f8: asr r3, r3, #4
003b97fc: add r1, r3, r3, lsl #3
003b9800: add r1, r1, r1, lsl #6
003b9804: add r1, r3, r1, lsl #3
003b9808: add r1, r1, r1, lsl #15
003b980c: add r3, r3, r1, lsl #3
003b9810: rsb r3, r3, #0
003b9814: cmp r3, #1
003b9818: bls #0x3b9864
003b981c: cmp r3, #0
003b9820: movne r0, r2
003b9824: beq #0x3b9938
003b9828: ldr r2, [r2, #4]
003b982c: cmp r2, #3
003b9830: beq #0x3b9890
003b9834: cmp r3, #0
003b9838: beq #0x3b9980
003b983c: bl #0x31bbf0
003b9840: mov r7, r0
003b9844: mov r0, r6
003b9848: bl #0x3bc5fc
003b984c: mov r5, r0
003b9850: mov r0, r7
003b9854: bl #0x8be2a0
003b9858: ldr r3, [r5, #4]
003b985c: cmp r3, r0
003b9860: bhi #0x3b9868
003b9864: pop {r4, r5, r6, r7, r8, pc}
003b9868: ldr r5, [r4, #4]
003b986c: ldm r5, {r0, r2}
003b9870: rsb r2, r0, r2
003b9874: asr r2, r2, #4
003b9878: add r3, r2, r2, lsl #3
003b987c: add r3, r3, r3, lsl #6
003b9880: add r3, r2, r3, lsl #3
003b9884: add r3, r3, r3, lsl #15
003b9888: add r3, r2, r3, lsl #3
003b988c: rsb r3, r3, #0
003b9890: cmp r3, #1
003b9894: bls #0x3b9994
003b9898: ldr r3, [r0, #0x74]
003b989c: cmp r3, #3
003b98a0: beq #0x3b98f0
003b98a4: ldr r5, [r4, #4]
003b98a8: ldm r5, {r2, r3}
003b98ac: rsb r3, r2, r3
003b98b0: asr r3, r3, #4
003b98b4: add r1, r3, r3, lsl #3
003b98b8: add r1, r1, r1, lsl #6
003b98bc: add r1, r3, r1, lsl #3
003b98c0: add r1, r1, r1, lsl #15
003b98c4: add r3, r3, r1, lsl #3
003b98c8: rsb r3, r3, #0
003b98cc: cmp r3, #1
003b98d0: bhi #0x3b98e4
003b98d4: ldr r0, [pc, #0xe4]
003b98d8: add r0, pc, r0
003b98dc: bl #0x708eb0
003b98e0: ldr r2, [r5]
003b98e4: ldr r3, [r2, #0x74]
003b98e8: cmp r3, #0
003b98ec: bne #0x3b9864
003b98f0: mov r1, #0
003b98f4: mov r0, r4
003b98f8: bl #0x37baf8
003b98fc: bl #0x31bbf0
003b9900: bl #0x30e4cc
003b9904: ldr r3, [r6, #0x47c]
003b9908: ldr r5, [r3, r0, lsl #2]
003b990c: cmp r5, #0
003b9910: beq #0x3b9864
003b9914: mov r0, r4
003b9918: mov r1, #1
003b991c: bl #0x37baf8
003b9920: ldr r3, [r0, #4]
003b9924: cmp r3, #0
003b9928: bne #0x3b99a8
003b992c: mvn r3, #0
003b9930: str r3, [r5, #0x18]
003b9934: pop {r4, r5, r6, r7, r8, pc}
003b9938: ldr r0, [pc, #0x84]
003b993c: add r0, pc, r0
003b9940: bl #0x708eb0
003b9944: ldr r2, [r5]
003b9948: ldr r5, [r4, #4]
003b994c: ldr r2, [r2, #4]
003b9950: ldm r5, {r0, r1}
003b9954: cmp r2, #3
003b9958: rsb r1, r0, r1
003b995c: asr r1, r1, #4
003b9960: add r3, r1, r1, lsl #3
003b9964: add r3, r3, r3, lsl #6
003b9968: add r3, r1, r3, lsl #3
003b996c: add r3, r3, r3, lsl #15
003b9970: add r3, r1, r3, lsl #3
003b9974: rsb r3, r3, #0
003b9978: bne #0x3b9834
003b997c: b #0x3b9890
003b9980: ldr r0, [pc, #0x40]
003b9984: add r0, pc, r0
003b9988: bl #0x708eb0
003b998c: ldr r0, [r5]
003b9990: b #0x3b983c
003b9994: ldr r0, [pc, #0x30]
003b9998: add r0, pc, r0
003b999c: bl #0x708eb0
003b99a0: ldr r0, [r5]
003b99a4: b #0x3b9898
003b99a8: mov r1, #1
003b99ac: mov r0, r4
003b99b0: bl #0x37baf8
003b99b4: bl #0x38d798
003b99b8: str r0, [r5, #0x18]
003b99bc: pop {r4, r5, r6, r7, r8, pc}

_ZN9Character15_ApplyPropClassERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3babdc 460
003babdc: push {r4, r5, r6, r7, r8, lr}
003babe0: ldr r3, [r0, #4]
003babe4: mov r6, r2
003babe8: ldr r4, [pc, #0x1ac]
003babec: ldm r3, {r1, r2}
003babf0: add r4, pc, r4
003babf4: mov r5, r0
003babf8: rsb r3, r1, r2
003babfc: asr r3, r3, #4
003bac00: add r2, r3, r3, lsl #3
003bac04: add r2, r2, r2, lsl #6
003bac08: add r2, r3, r2, lsl #3
003bac0c: add r2, r2, r2, lsl #15
003bac10: add r3, r3, r2, lsl #3
003bac14: cmp r3, #0
003bac18: bne #0x3bac20
003bac1c: pop {r4, r5, r6, r7, r8, pc}
003bac20: ldr r3, [r1, #4]
003bac24: cmp r3, #3
003bac28: bne #0x3bac1c
003bac2c: mov r1, #0
003bac30: bl #0x37baf8
003bac34: bl #0x38d798
003bac38: ldr r3, [pc, #0x160]
003bac3c: ldr r3, [r4, r3]
003bac40: ldr r3, [r3]
003bac44: cmp r0, r3
003bac48: bhi #0x3bac1c
003bac4c: ldr r7, [r5, #4]
003bac50: ldm r7, {r0, r3}
003bac54: rsb r3, r0, r3
003bac58: asr r3, r3, #4
003bac5c: add r2, r3, r3, lsl #3
003bac60: add r2, r2, r2, lsl #6
003bac64: add r2, r3, r2, lsl #3
003bac68: add r2, r2, r2, lsl #15
003bac6c: add r3, r3, r2, lsl #3
003bac70: rsb r3, r3, #0
003bac74: cmp r3, #1
003bac78: bls #0x3bacc8
003bac7c: ldr r3, [r0, #0x74]
003bac80: cmp r3, #2
003bac84: beq #0x3bad04
003bac88: mov r0, r5
003bac8c: mov r1, #1
003bac90: bl #0x37baf8
003bac94: ldr r4, [r0, #4]
003bac98: cmp r4, #1
003bac9c: beq #0x3bad64
003baca0: ldr r7, [r5, #4]
003baca4: ldm r7, {r0, r3}
003baca8: rsb r3, r0, r3
003bacac: asr r3, r3, #4
003bacb0: add r2, r3, r3, lsl #3
003bacb4: add r2, r2, r2, lsl #6
003bacb8: add r2, r3, r2, lsl #3
003bacbc: add r2, r2, r2, lsl #15
003bacc0: add r3, r3, r2, lsl #3
003bacc4: rsb r3, r3, #0
003bacc8: cmp r3, #0
003baccc: add r6, r6, #0x560
003bacd0: beq #0x3bacf0
003bacd4: bl #0x31bbf0
003bacd8: bl #0x8be2a0
003bacdc: mov r2, #0
003bace0: mov r1, r0
003bace4: mov r0, r6
003bace8: pop {r4, r5, r6, r7, r8, lr}
003bacec: b #0x3df3b8
003bacf0: ldr r0, [pc, #0xac]
003bacf4: add r0, pc, r0
003bacf8: bl #0x708eb0
003bacfc: ldr r0, [r7]
003bad00: b #0x3bacd4
003bad04: mov r1, #1
003bad08: mov r0, r5
003bad0c: bl #0x37baf8
003bad10: bl #0x31b580
003bad14: cmp r0, #0
003bad18: beq #0x3bac1c
003bad1c: mov r1, #0
003bad20: mov r0, r5
003bad24: bl #0x37baf8
003bad28: bl #0x38d798
003bad2c: mov r1, #1
003bad30: mov r7, r0
003bad34: mov r0, r5
003bad38: bl #0x37baf8
003bad3c: bl #0x31b580
003bad40: add r4, r6, #0x560
003bad44: mov r2, r0
003bad48: mov r1, r7
003bad4c: mov r0, r4
003bad50: bl #0x3df314
003bad54: mov r0, r4
003bad58: mov r1, #1
003bad5c: pop {r4, r5, r6, r7, r8, lr}
003bad60: b #0x3e0810
003bad64: mov r1, #0
003bad68: mov r0, r5
003bad6c: bl #0x37baf8
003bad70: bl #0x38d798
003bad74: mov r1, r4
003bad78: mov r7, r0
003bad7c: mov r0, r5
003bad80: bl #0x37baf8
003bad84: bl #0x31bc80
003bad88: mov r1, r7
003bad8c: mov r2, r0
003bad90: add r0, r6, #0x560
003bad94: pop {r4, r5, r6, r7, r8, lr}
003bad98: b #0x3df3b8
003bad9c: subseq sb, sp, r0, lsr #29
003bada0: andeq r3, r0, r8, ror #10
003bada4: subseq r3, r0, r4, ror r7

_ZN9Character7_HasBowERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6f84 32
003b6f84: push {r4, lr}
003b6f88: add r0, r2, #0x37c
003b6f8c: mov r4, r1
003b6f90: bl #0x400080
003b6f94: mov r1, r0
003b6f98: mov r0, r4
003b6f9c: pop {r4, lr}
003b6fa0: b #0x37c7e4

_ZN3sfc6script3lua12ReturnValues10pushNumberEf 0x37ccbc 104
0037ccbc: ldr r3, [pc, #0x58]
0037ccc0: ldr r2, [pc, #0x58]
0037ccc4: push {r4, r5, r6, lr}
0037ccc8: add r3, pc, r3
0037cccc: ldr r5, [r3, r2]
0037ccd0: sub sp, sp, #0x78
0037ccd4: add r4, sp, #4
0037ccd8: ldr r3, [r5]
0037ccdc: str r3, [sp, #0x74]
0037cce0: ldr r6, [r0, #0x24]
0037cce4: mov r0, r4
0037cce8: bl #0x37cc3c
0037ccec: mov r0, r6
0037ccf0: mov r1, r4
0037ccf4: bl #0x3195c0
0037ccf8: mov r0, r4
0037ccfc: bl #0x3193e8
0037cd00: ldr r2, [sp, #0x74]
0037cd04: ldr r3, [r5]
0037cd08: cmp r2, r3
0037cd0c: bne #0x37cd18
0037cd10: add sp, sp, #0x78
0037cd14: pop {r4, r5, r6, pc}
0037cd18: bl #0x30e310
0037cd1c: rsbeq r7, r1, r8, asr #27
0037cd20: andeq r4, r0, ip, lsr #1

_ZN10GameObject18_SummonTriggerTrapERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x391670 716
00391670: push {r4, r5, r6, r7, r8, lr}
00391674: ldr r5, [r0, #4]
00391678: mov r7, r1
0039167c: mov r8, r2
00391680: ldr r3, [r5]
00391684: ldr r1, [r5, #4]
00391688: ldr r4, [pc, #0x298]
0039168c: sub sp, sp, #0x10
00391690: rsb r1, r3, r1
00391694: asr r1, r1, #4
00391698: add r4, pc, r4
0039169c: add r2, r1, r1, lsl #3
003916a0: mov r6, r0
003916a4: add r2, r2, r2, lsl #6
003916a8: add r2, r1, r2, lsl #3
003916ac: add r2, r2, r2, lsl #15
003916b0: add r1, r1, r2, lsl #3
003916b4: rsb r1, r1, #0
003916b8: cmp r1, #1
003916bc: bls #0x3916d4
003916c0: cmp r1, #0
003916c4: beq #0x3916dc
003916c8: ldr r3, [r3, #4]
003916cc: cmp r3, #3
003916d0: beq #0x3916f0
003916d4: add sp, sp, #0x10
003916d8: pop {r4, r5, r6, r7, r8, pc}
003916dc: ldr r0, [pc, #0x248]
003916e0: add r0, pc, r0
003916e4: bl #0x708eb0
003916e8: ldr r3, [r5]
003916ec: b #0x3916c8
003916f0: mov r1, #0
003916f4: mov r0, r6
003916f8: bl #0x37baf8
003916fc: bl #0x38d798
00391700: ldr r3, [pc, #0x228]
00391704: ldr r3, [r4, r3]
00391708: ldr r3, [r3]
0039170c: cmp r0, r3
00391710: bhs #0x3916d4
00391714: ldr r5, [r6, #4]
00391718: ldr r3, [r5]
0039171c: ldr r2, [r5, #4]
00391720: rsb r2, r3, r2
00391724: asr r2, r2, #4
00391728: add r1, r2, r2, lsl #3
0039172c: add r1, r1, r1, lsl #6
00391730: add r1, r2, r1, lsl #3
00391734: add r1, r1, r1, lsl #15
00391738: add r2, r2, r1, lsl #3
0039173c: rsb r2, r2, #0
00391740: cmp r2, #1
00391744: bhi #0x391758
00391748: ldr r0, [pc, #0x1e4]
0039174c: add r0, pc, r0
00391750: bl #0x708eb0
00391754: ldr r3, [r5]
00391758: ldr r3, [r3, #0x74]
0039175c: cmp r3, #3
00391760: bne #0x3916d4
00391764: mov r1, #1
00391768: mov r0, r6
0039176c: bl #0x37baf8
00391770: bl #0x38d798
00391774: ldr r3, [pc, #0x1bc]
00391778: ldr r3, [r4, r3]
0039177c: ldr r3, [r3]
00391780: cmp r0, r3
00391784: bhs #0x3916d4
00391788: mov r1, #0
0039178c: mov r0, r6
00391790: bl #0x37baf8
00391794: bl #0x31bbf0
00391798: mov r1, #1
0039179c: mov r4, r0
003917a0: mov r0, r6
003917a4: bl #0x37baf8
003917a8: bl #0x31bbf0
003917ac: mov r5, r0
003917b0: mov r0, r4
003917b4: bl #0x30e4cc
003917b8: mov r4, r0
003917bc: mov r0, r5
003917c0: bl #0x30e4cc
003917c4: mov r1, r4
003917c8: mov r2, r0
003917cc: mov r0, r8
003917d0: bl #0x39e298
003917d4: ldr r2, [r6, #4]
003917d8: mov r4, r0
003917dc: ldr r3, [r2]
003917e0: ldr r2, [r2, #4]
003917e4: rsb r3, r3, r2
003917e8: asr r3, r3, #4
003917ec: add r2, r3, r3, lsl #3
003917f0: add r2, r2, r2, lsl #6
003917f4: add r2, r3, r2, lsl #3
003917f8: add r2, r2, r2, lsl #15
003917fc: add r3, r3, r2, lsl #3
00391800: rsb r3, r3, #0
00391804: cmp r3, #2
00391808: bhi #0x39181c
0039180c: mov r0, r7
00391810: mov r1, r4
00391814: bl #0x37c9f8
00391818: b #0x3916d4
0039181c: mov r0, r6
00391820: mov r1, #2
00391824: bl #0x37baf8
00391828: ldr r3, [r0, #4]
0039182c: cmp r3, #7
00391830: beq #0x391904
00391834: ldr r2, [r6, #4]
00391838: ldr r1, [r2, #4]
0039183c: ldr r3, [r2]
00391840: rsb r3, r3, r1
00391844: asr r3, r3, #4
00391848: add r2, r3, r3, lsl #3
0039184c: add r2, r2, r2, lsl #6
00391850: add r2, r3, r2, lsl #3
00391854: add r2, r2, r2, lsl #15
00391858: add r3, r3, r2, lsl #3
0039185c: rsb r3, r3, #0
00391860: cmp r3, #4
00391864: bls #0x39180c
00391868: mov r1, #2
0039186c: mov r0, r6
00391870: bl #0x37baf8
00391874: ldr r1, [r0, #4]
00391878: cmp r1, #3
0039187c: bne #0x39180c
00391880: mov r0, r6
00391884: bl #0x37baf8
00391888: ldr r3, [r0, #4]
0039188c: cmp r3, #3
00391890: bne #0x39180c
00391894: mov r0, r6
00391898: mov r1, #4
0039189c: bl #0x37baf8
003918a0: ldr r5, [r0, #4]
003918a4: cmp r5, #3
003918a8: bne #0x39180c
003918ac: mov r1, #2
003918b0: mov r0, r6
003918b4: bl #0x37baf8
003918b8: bl #0x31bbf0
003918bc: mov r1, r5
003918c0: mov r8, r0
003918c4: mov r0, r6
003918c8: bl #0x37baf8
003918cc: bl #0x31bbf0
003918d0: mov r1, #4
003918d4: mov r5, r0
003918d8: mov r0, r6
003918dc: bl #0x37baf8
003918e0: bl #0x31bbf0
003918e4: add r1, sp, #4
003918e8: str r0, [sp, #0xc]
003918ec: mov r2, #1
003918f0: mov r0, r4
003918f4: str r8, [sp, #4]
003918f8: str r5, [sp, #8]
003918fc: bl #0x393db4
00391900: b #0x39180c
00391904: mov r1, #2
00391908: mov r0, r6
0039190c: bl #0x37baf8
00391910: bl #0x31b5a0
00391914: mov r2, #1
00391918: add r1, r0, #0x160
0039191c: mov r0, r4
00391920: bl #0x393db4
00391924: b #0x39180c

_ZN3sfc6script3lua5ValueaSERKS2_ 0x31c368 100
0031c368: push {r4, r5, r6, lr}
0031c36c: ldr r3, [r1, #4]
0031c370: mov r5, r0
0031c374: add r2, r1, #0xc
0031c378: str r3, [r5, #4]
0031c37c: ldr r3, [r1, #8]
0031c380: add r0, r0, #0xc
0031c384: cmp r0, r2
0031c388: mov r4, r1
0031c38c: str r3, [r5, #8]
0031c390: beq #0x31c3a0
0031c394: ldr r1, [r1, #0x20]
0031c398: ldr r2, [r4, #0x1c]
0031c39c: bl #0x3109e0
0031c3a0: add r0, r5, #0x24
0031c3a4: add r3, r4, #0x24
0031c3a8: cmp r0, r3
0031c3ac: beq #0x31c3bc
0031c3b0: ldr r1, [r4, #0x68]
0031c3b4: ldr r2, [r4, #0x64]
0031c3b8: bl #0x31c144
0031c3bc: ldr r3, [r4, #0x6c]
0031c3c0: mov r0, r5
0031c3c4: str r3, [r5, #0x6c]
0031c3c8: pop {r4, r5, r6, pc}

_ZN6TestUD8TestFuncERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x386ec0 104
00386ec0: push {r4, r5, r6, lr}
00386ec4: ldr r0, [r0, #4]
00386ec8: mov r4, r2
00386ecc: mov r5, r1
00386ed0: ldr r2, [r0, #4]
00386ed4: ldr r3, [r0]
00386ed8: ldr r0, [pc, #0x44]
00386edc: mov r1, r4
00386ee0: rsb r3, r3, r2
00386ee4: asr r3, r3, #4
00386ee8: add r0, pc, r0
00386eec: add r2, r3, r3, lsl #3
00386ef0: add r2, r2, r2, lsl #6
00386ef4: add r2, r3, r2, lsl #3
00386ef8: add r2, r2, r2, lsl #15
00386efc: add r2, r3, r2, lsl #3
00386f00: rsb r2, r2, #0
00386f04: bl #0x30de84
00386f08: mov r0, r5
00386f0c: mov r1, r4
00386f10: bl #0x37c9f8
00386f14: mov r0, r5
00386f18: mov r1, r4
00386f1c: pop {r4, r5, r6, lr}
00386f20: b #0x37c9f8
00386f24: subseq fp, r3, r8, lsr #4

_ZN9Character14createBindingsERN3sfc6script3lua6BinderE 0x3b56bc 5332
003b56bc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b56c0: ldr r6, [pc, #0xff8]
003b56c4: sub sp, sp, #0xc
003b56c8: mov r4, r1
003b56cc: mov r5, r0
003b56d0: bl #0x38d7ec
003b56d4: ldr r3, [pc, #0xfe8]
003b56d8: add r6, pc, r6
003b56dc: ldr r7, [pc, #0xfe4]
003b56e0: ldr r8, [r6, r3]
003b56e4: mov r0, r4
003b56e8: add r7, pc, r7
003b56ec: mov r3, r5
003b56f0: mov r1, r7
003b56f4: mov r2, r8
003b56f8: bl #0x31a4d4
003b56fc: mov r0, r4
003b5700: mov r1, r7
003b5704: mov r2, r8
003b5708: bl #0x319af4
003b570c: ldr r2, [pc, #0xfb8]
003b5710: ldr r7, [pc, #0xfb8]
003b5714: mov r3, r5
003b5718: ldr r8, [r6, r2]
003b571c: add r7, pc, r7
003b5720: mov r0, r4
003b5724: mov r1, r7
003b5728: mov r2, r8
003b572c: bl #0x31a4d4
003b5730: mov r0, r4
003b5734: mov r1, r7
003b5738: mov r2, r8
003b573c: bl #0x319af4
003b5740: ldr r2, [pc, #0xf8c]
003b5744: ldr r7, [pc, #0xf8c]
003b5748: mov r3, r5
003b574c: ldr r8, [r6, r2]
003b5750: add r7, pc, r7
003b5754: mov r0, r4
003b5758: mov r1, r7
003b575c: mov r2, r8
003b5760: bl #0x31a4d4
003b5764: mov r0, r4
003b5768: mov r1, r7
003b576c: mov r2, r8
003b5770: bl #0x319af4
003b5774: ldr r2, [pc, #0xf60]
003b5778: ldr r7, [pc, #0xf60]
003b577c: mov r3, r5
003b5780: ldr r8, [r6, r2]
003b5784: add r7, pc, r7
003b5788: mov r0, r4
003b578c: mov r1, r7
003b5790: mov r2, r8
003b5794: bl #0x31a4d4
003b5798: mov r0, r4
003b579c: mov r1, r7
003b57a0: mov r2, r8
003b57a4: bl #0x319af4
003b57a8: ldr r2, [pc, #0xf34]
003b57ac: ldr r7, [pc, #0xf34]
003b57b0: mov r3, r5
003b57b4: ldr r8, [r6, r2]
003b57b8: add r7, pc, r7
003b57bc: mov r0, r4
003b57c0: mov r1, r7
003b57c4: mov r2, r8
003b57c8: bl #0x31a4d4
003b57cc: mov r0, r4
003b57d0: mov r1, r7
003b57d4: mov r2, r8
003b57d8: bl #0x319af4
003b57dc: ldr r2, [pc, #0xf08]
003b57e0: ldr r7, [pc, #0xf08]
003b57e4: mov r3, r5
003b57e8: ldr r8, [r6, r2]
003b57ec: add r7, pc, r7
003b57f0: mov r0, r4
003b57f4: mov r1, r7
003b57f8: mov r2, r8
003b57fc: bl #0x31a4d4
003b5800: mov r0, r4
003b5804: mov r1, r7
003b5808: mov r2, r8
003b580c: bl #0x319af4
003b5810: ldr r2, [pc, #0xedc]
003b5814: ldr r7, [pc, #0xedc]
003b5818: mov r3, r5
003b581c: ldr r8, [r6, r2]
003b5820: add r7, pc, r7
003b5824: mov r0, r4
003b5828: mov r1, r7
003b582c: mov r2, r8
003b5830: bl #0x31a4d4
003b5834: mov r0, r4
003b5838: mov r1, r7
003b583c: mov r2, r8
003b5840: bl #0x319af4
003b5844: ldr r2, [pc, #0xeb0]
003b5848: ldr r7, [pc, #0xeb0]
003b584c: mov r3, r5
003b5850: ldr r8, [r6, r2]
003b5854: add r7, pc, r7
003b5858: mov r0, r4
003b585c: mov r1, r7
003b5860: mov r2, r8
003b5864: bl #0x31a4d4
003b5868: mov r0, r4
003b586c: mov r1, r7
003b5870: mov r2, r8
003b5874: bl #0x319af4
003b5878: ldr r2, [pc, #0xe84]
003b587c: ldr r7, [pc, #0xe84]
003b5880: mov r3, r5
003b5884: ldr r8, [r6, r2]
003b5888: add r7, pc, r7
003b588c: mov r0, r4
003b5890: mov r1, r7
003b5894: mov r2, r8
003b5898: bl #0x31a4d4
003b589c: mov r0, r4
003b58a0: mov r1, r7
003b58a4: mov r2, r8
003b58a8: bl #0x319af4
003b58ac: ldr r2, [pc, #0xe58]
003b58b0: ldr r7, [pc, #0xe58]
003b58b4: mov r3, r5
003b58b8: ldr r8, [r6, r2]
003b58bc: add r7, pc, r7
003b58c0: mov r0, r4
003b58c4: mov r1, r7
003b58c8: mov r2, r8
003b58cc: bl #0x31a4d4
003b58d0: mov r0, r4
003b58d4: mov r1, r7
003b58d8: mov r2, r8
003b58dc: bl #0x319af4
003b58e0: ldr r2, [pc, #0xe2c]
003b58e4: ldr r7, [pc, #0xe2c]
003b58e8: mov r3, r5
003b58ec: ldr r8, [r6, r2]
003b58f0: add r7, pc, r7
003b58f4: mov r0, r4
003b58f8: mov r1, r7
003b58fc: mov r2, r8
003b5900: bl #0x31a4d4
003b5904: mov r0, r4
003b5908: mov r1, r7
003b590c: mov r2, r8
003b5910: bl #0x319af4
003b5914: ldr r2, [pc, #0xe00]
003b5918: ldr r7, [pc, #0xe00]
003b591c: mov r3, r5
003b5920: ldr r8, [r6, r2]
003b5924: add r7, pc, r7
003b5928: mov r0, r4
003b592c: mov r1, r7
003b5930: mov r2, r8
003b5934: bl #0x31a4d4
003b5938: mov r0, r4
003b593c: mov r1, r7
003b5940: mov r2, r8
003b5944: bl #0x319af4
003b5948: ldr r2, [pc, #0xdd4]
003b594c: ldr r7, [pc, #0xdd4]
003b5950: mov r3, r5
003b5954: ldr r8, [r6, r2]
003b5958: add r7, pc, r7
003b595c: mov r0, r4
003b5960: mov r1, r7
003b5964: mov r2, r8
003b5968: bl #0x31a4d4
003b596c: mov r0, r4
003b5970: mov r1, r7
003b5974: mov r2, r8
003b5978: bl #0x319af4
003b597c: ldr r2, [pc, #0xda8]
003b5980: ldr r7, [pc, #0xda8]
003b5984: mov r3, r5
003b5988: ldr r8, [r6, r2]
003b598c: add r7, pc, r7
003b5990: mov r0, r4
003b5994: mov r1, r7
003b5998: mov r2, r8
003b599c: bl #0x31a4d4
003b59a0: mov r0, r4
003b59a4: mov r1, r7
003b59a8: mov r2, r8
003b59ac: bl #0x319af4
003b59b0: ldr r2, [pc, #0xd7c]
003b59b4: ldr r7, [pc, #0xd7c]
003b59b8: mov r3, r5
003b59bc: ldr r8, [r6, r2]
003b59c0: add r7, pc, r7
003b59c4: mov r0, r4
003b59c8: mov r1, r7
003b59cc: mov r2, r8
003b59d0: bl #0x31a4d4
003b59d4: mov r0, r4
003b59d8: mov r1, r7
003b59dc: mov r2, r8
003b59e0: bl #0x319af4
003b59e4: ldr r2, [pc, #0xd50]
003b59e8: ldr r7, [pc, #0xd50]
003b59ec: mov r3, r5
003b59f0: ldr r8, [r6, r2]
003b59f4: add r7, pc, r7
003b59f8: mov r0, r4
003b59fc: mov r1, r7
003b5a00: mov r2, r8
003b5a04: bl #0x31a4d4
003b5a08: mov r0, r4
003b5a0c: mov r1, r7
003b5a10: mov r2, r8
003b5a14: bl #0x319af4
003b5a18: ldr r2, [pc, #0xd24]
003b5a1c: ldr r7, [pc, #0xd24]
003b5a20: mov r3, r5
003b5a24: ldr r8, [r6, r2]
003b5a28: add r7, pc, r7
003b5a2c: mov r0, r4
003b5a30: mov r1, r7
003b5a34: mov r2, r8
003b5a38: bl #0x31a4d4
003b5a3c: mov r0, r4
003b5a40: mov r1, r7
003b5a44: mov r2, r8
003b5a48: bl #0x319af4
003b5a4c: ldr r2, [pc, #0xcf8]
003b5a50: ldr r7, [pc, #0xcf8]
003b5a54: mov r3, r5
003b5a58: ldr r8, [r6, r2]
003b5a5c: add r7, pc, r7
003b5a60: mov r0, r4
003b5a64: mov r1, r7
003b5a68: mov r2, r8
003b5a6c: bl #0x31a4d4
003b5a70: mov r0, r4
003b5a74: mov r1, r7
003b5a78: mov r2, r8
003b5a7c: bl #0x319af4
003b5a80: ldr r2, [pc, #0xccc]
003b5a84: ldr r7, [pc, #0xccc]
003b5a88: mov r3, r5
003b5a8c: ldr r8, [r6, r2]
003b5a90: add r7, pc, r7
003b5a94: mov r0, r4
003b5a98: mov r1, r7
003b5a9c: mov r2, r8
003b5aa0: bl #0x31a4d4
003b5aa4: mov r0, r4
003b5aa8: mov r1, r7
003b5aac: mov r2, r8
003b5ab0: bl #0x319af4
003b5ab4: ldr r2, [pc, #0xca0]
003b5ab8: ldr r7, [pc, #0xca0]
003b5abc: mov r3, r5
003b5ac0: ldr r8, [r6, r2]
003b5ac4: add r7, pc, r7
003b5ac8: mov r0, r4
003b5acc: mov r1, r7
003b5ad0: mov r2, r8
003b5ad4: bl #0x31a4d4
003b5ad8: mov r0, r4
003b5adc: mov r1, r7
003b5ae0: mov r2, r8
003b5ae4: bl #0x319af4
003b5ae8: ldr r2, [pc, #0xc74]
003b5aec: ldr r7, [pc, #0xc74]
003b5af0: mov r3, r5
003b5af4: ldr r8, [r6, r2]
003b5af8: add r7, pc, r7
003b5afc: mov r0, r4
003b5b00: mov r1, r7
003b5b04: mov r2, r8
003b5b08: bl #0x31a4d4
003b5b0c: mov r0, r4
003b5b10: mov r1, r7
003b5b14: mov r2, r8
003b5b18: bl #0x319af4
003b5b1c: ldr r2, [pc, #0xc48]
003b5b20: ldr r7, [pc, #0xc48]
003b5b24: mov r3, r5
003b5b28: ldr r8, [r6, r2]
003b5b2c: add r7, pc, r7
003b5b30: mov r0, r4
003b5b34: mov r1, r7
003b5b38: mov r2, r8
003b5b3c: bl #0x31a4d4
003b5b40: mov r0, r4
003b5b44: mov r1, r7
003b5b48: mov r2, r8
003b5b4c: bl #0x319af4
003b5b50: ldr r3, [pc, #0xc1c]
003b5b54: ldr r7, [pc, #0xc1c]
003b5b58: ldr r8, [pc, #0xc1c]
003b5b5c: ldr sl, [r6, r3]
003b5b60: add r7, pc, r7
003b5b64: mov r3, r5
003b5b68: mov r0, r4
003b5b6c: mov r1, r7
003b5b70: mov r2, sl
003b5b74: bl #0x31a4d4
003b5b78: add r8, pc, r8
003b5b7c: mov r0, r4
003b5b80: mov r1, r7
003b5b84: mov r2, sl
003b5b88: bl #0x319af4
003b5b8c: mov r3, r5
003b5b90: mov r0, r4
003b5b94: mov r1, r8
003b5b98: mov r2, sl
003b5b9c: bl #0x31a4d4
003b5ba0: mov r0, r4
003b5ba4: mov r1, r8
003b5ba8: mov r2, sl
003b5bac: bl #0x319af4
003b5bb0: ldr r2, [pc, #0xbc8]
003b5bb4: ldr r7, [pc, #0xbc8]
003b5bb8: mov r3, r5
003b5bbc: ldr r8, [r6, r2]
003b5bc0: add r7, pc, r7
003b5bc4: mov r0, r4
003b5bc8: mov r1, r7
003b5bcc: mov r2, r8
003b5bd0: bl #0x31a4d4
003b5bd4: mov r0, r4
003b5bd8: mov r1, r7
003b5bdc: mov r2, r8
003b5be0: bl #0x319af4
003b5be4: ldr r2, [pc, #0xb9c]
003b5be8: ldr r7, [pc, #0xb9c]
003b5bec: mov r3, r5
003b5bf0: ldr r8, [r6, r2]
003b5bf4: add r7, pc, r7
003b5bf8: mov r0, r4
003b5bfc: mov r1, r7
003b5c00: mov r2, r8
003b5c04: bl #0x31a4d4
003b5c08: mov r0, r4
003b5c0c: mov r1, r7
003b5c10: mov r2, r8
003b5c14: bl #0x319af4
003b5c18: ldr r2, [pc, #0xb70]
003b5c1c: ldr r7, [pc, #0xb70]
003b5c20: mov r3, r5
003b5c24: ldr r8, [r6, r2]
003b5c28: add r7, pc, r7
003b5c2c: mov r0, r4
003b5c30: mov r1, r7
003b5c34: mov r2, r8
003b5c38: bl #0x31a4d4
003b5c3c: mov r0, r4
003b5c40: mov r1, r7
003b5c44: mov r2, r8
003b5c48: bl #0x319af4
003b5c4c: ldr r2, [pc, #0xb44]
003b5c50: ldr r7, [pc, #0xb44]
003b5c54: mov r3, r5
003b5c58: ldr r8, [r6, r2]
003b5c5c: add r7, pc, r7
003b5c60: mov r0, r4
003b5c64: mov r1, r7
003b5c68: mov r2, r8
003b5c6c: bl #0x31a4d4
003b5c70: mov r0, r4
003b5c74: mov r1, r7
003b5c78: mov r2, r8
003b5c7c: bl #0x319af4
003b5c80: ldr r2, [pc, #0xb18]
003b5c84: ldr r7, [pc, #0xb18]
003b5c88: mov r3, r5
003b5c8c: ldr r8, [r6, r2]
003b5c90: add r7, pc, r7
003b5c94: mov r0, r4
003b5c98: mov r1, r7
003b5c9c: mov r2, r8
003b5ca0: bl #0x31a4d4
003b5ca4: mov r0, r4
003b5ca8: mov r1, r7
003b5cac: mov r2, r8
003b5cb0: bl #0x319af4
003b5cb4: ldr r2, [pc, #0xaec]
003b5cb8: ldr r7, [pc, #0xaec]
003b5cbc: mov r3, r5
003b5cc0: ldr r8, [r6, r2]
003b5cc4: add r7, pc, r7
003b5cc8: mov r0, r4
003b5ccc: mov r1, r7
003b5cd0: mov r2, r8
003b5cd4: bl #0x31a4d4
003b5cd8: mov r0, r4
003b5cdc: mov r1, r7
003b5ce0: mov r2, r8
003b5ce4: bl #0x319af4
003b5ce8: ldr r2, [pc, #0xac0]
003b5cec: ldr r7, [pc, #0xac0]
003b5cf0: mov r3, r5
003b5cf4: ldr r8, [r6, r2]
003b5cf8: add r7, pc, r7
003b5cfc: mov r0, r4
003b5d00: mov r1, r7
003b5d04: mov r2, r8
003b5d08: bl #0x31a4d4
003b5d0c: mov r0, r4
003b5d10: mov r1, r7
003b5d14: mov r2, r8
003b5d18: bl #0x319af4
003b5d1c: ldr r2, [pc, #0xa94]
003b5d20: ldr r7, [pc, #0xa94]
003b5d24: mov r3, r5
003b5d28: ldr r8, [r6, r2]
003b5d2c: add r7, pc, r7
003b5d30: mov r0, r4
003b5d34: mov r1, r7
003b5d38: mov r2, r8
003b5d3c: bl #0x31a4d4
003b5d40: mov r0, r4
003b5d44: mov r1, r7
003b5d48: mov r2, r8
003b5d4c: bl #0x319af4
003b5d50: ldr r2, [pc, #0xa68]
003b5d54: ldr r7, [pc, #0xa68]
003b5d58: mov r3, r5
003b5d5c: ldr r8, [r6, r2]
003b5d60: add r7, pc, r7
003b5d64: mov r0, r4
003b5d68: mov r1, r7
003b5d6c: mov r2, r8
003b5d70: bl #0x31a4d4
003b5d74: mov r0, r4
003b5d78: mov r1, r7
003b5d7c: mov r2, r8
003b5d80: bl #0x319af4
003b5d84: ldr r2, [pc, #0xa3c]
003b5d88: ldr r7, [pc, #0xa3c]
003b5d8c: mov r3, r5
003b5d90: ldr r8, [r6, r2]
003b5d94: add r7, pc, r7
003b5d98: mov r0, r4
003b5d9c: mov r1, r7
003b5da0: mov r2, r8
003b5da4: bl #0x31a4d4
003b5da8: mov r0, r4
003b5dac: mov r1, r7
003b5db0: mov r2, r8
003b5db4: bl #0x319af4
003b5db8: ldr r2, [pc, #0xa10]
003b5dbc: ldr r7, [pc, #0xa10]
003b5dc0: mov r3, r5
003b5dc4: ldr r8, [r6, r2]
003b5dc8: add r7, pc, r7
003b5dcc: mov r0, r4
003b5dd0: mov r1, r7
003b5dd4: mov r2, r8
003b5dd8: bl #0x31a4d4
003b5ddc: mov r0, r4
003b5de0: mov r1, r7
003b5de4: mov r2, r8
003b5de8: bl #0x319af4
003b5dec: ldr r2, [pc, #0x9e4]
003b5df0: ldr r7, [pc, #0x9e4]
003b5df4: mov r3, r5
003b5df8: ldr r8, [r6, r2]
003b5dfc: add r7, pc, r7
003b5e00: mov r0, r4
003b5e04: mov r1, r7
003b5e08: mov r2, r8
003b5e0c: bl #0x31a4d4
003b5e10: mov r0, r4
003b5e14: mov r1, r7
003b5e18: mov r2, r8
003b5e1c: bl #0x319af4
003b5e20: ldr r2, [pc, #0x9b8]
003b5e24: ldr r7, [pc, #0x9b8]
003b5e28: mov r3, r5
003b5e2c: ldr r8, [r6, r2]
003b5e30: add r7, pc, r7
003b5e34: mov r0, r4
003b5e38: mov r1, r7
003b5e3c: mov r2, r8
003b5e40: bl #0x31a4d4
003b5e44: mov r0, r4
003b5e48: mov r1, r7
003b5e4c: mov r2, r8
003b5e50: bl #0x319af4
003b5e54: ldr r2, [pc, #0x98c]
003b5e58: ldr r7, [pc, #0x98c]
003b5e5c: mov r3, r5
003b5e60: ldr r8, [r6, r2]
003b5e64: add r7, pc, r7
003b5e68: mov r0, r4
003b5e6c: mov r1, r7
003b5e70: mov r2, r8
003b5e74: bl #0x31a4d4
003b5e78: mov r0, r4
003b5e7c: mov r1, r7
003b5e80: mov r2, r8
003b5e84: bl #0x319af4
003b5e88: ldr r3, [pc, #0x960]
003b5e8c: ldr r1, [pc, #0x960]
003b5e90: mov r0, r4
003b5e94: ldr r2, [r6, r3]
003b5e98: add r1, pc, r1
003b5e9c: mov r3, #0
003b5ea0: bl #0x31a4d4
003b5ea4: ldr r2, [pc, #0x94c]
003b5ea8: ldr r7, [pc, #0x94c]
003b5eac: mov r3, r5
003b5eb0: ldr r8, [r6, r2]
003b5eb4: add r7, pc, r7
003b5eb8: mov r0, r4
003b5ebc: mov r1, r7
003b5ec0: mov r2, r8
003b5ec4: bl #0x31a4d4
003b5ec8: mov r0, r4
003b5ecc: mov r1, r7
003b5ed0: mov r2, r8
003b5ed4: bl #0x319af4
003b5ed8: ldr r2, [pc, #0x920]
003b5edc: ldr r7, [pc, #0x920]
003b5ee0: mov r3, r5
003b5ee4: ldr r8, [r6, r2]
003b5ee8: add r7, pc, r7
003b5eec: mov r0, r4
003b5ef0: mov r1, r7
003b5ef4: mov r2, r8
003b5ef8: bl #0x31a4d4
003b5efc: mov r0, r4
003b5f00: mov r1, r7
003b5f04: mov r2, r8
003b5f08: bl #0x319af4
003b5f0c: ldr r2, [pc, #0x8f4]
003b5f10: ldr r7, [pc, #0x8f4]
003b5f14: mov r3, r5
003b5f18: ldr r8, [r6, r2]
003b5f1c: add r7, pc, r7
003b5f20: mov r0, r4
003b5f24: mov r1, r7
003b5f28: mov r2, r8
003b5f2c: bl #0x31a4d4
003b5f30: mov r0, r4
003b5f34: mov r1, r7
003b5f38: mov r2, r8
003b5f3c: bl #0x319af4
003b5f40: ldr r2, [pc, #0x8c8]
003b5f44: ldr r7, [pc, #0x8c8]
003b5f48: mov r3, r5
003b5f4c: ldr r8, [r6, r2]
003b5f50: add r7, pc, r7
003b5f54: mov r0, r4
003b5f58: mov r1, r7
003b5f5c: mov r2, r8
003b5f60: bl #0x31a4d4
003b5f64: mov r0, r4
003b5f68: mov r1, r7
003b5f6c: mov r2, r8
003b5f70: bl #0x319af4
003b5f74: ldr r2, [pc, #0x89c]
003b5f78: ldr r7, [pc, #0x89c]
003b5f7c: mov r3, r5
003b5f80: ldr r8, [r6, r2]
003b5f84: add r7, pc, r7
003b5f88: mov r0, r4
003b5f8c: mov r1, r7
003b5f90: mov r2, r8
003b5f94: bl #0x31a4d4
003b5f98: mov r0, r4
003b5f9c: mov r1, r7
003b5fa0: mov r2, r8
003b5fa4: bl #0x319af4
003b5fa8: ldr r3, [pc, #0x870]
003b5fac: ldr r7, [pc, #0x870]
003b5fb0: ldr r8, [pc, #0x870]
003b5fb4: ldr sl, [r6, r3]
003b5fb8: add r7, pc, r7
003b5fbc: mov r3, r5
003b5fc0: mov r0, r4
003b5fc4: mov r1, r7
003b5fc8: mov r2, sl
003b5fcc: bl #0x31a4d4
003b5fd0: add r8, pc, r8
003b5fd4: mov r0, r4
003b5fd8: mov r1, r7
003b5fdc: mov r2, sl
003b5fe0: bl #0x319af4
003b5fe4: mov r3, r5
003b5fe8: mov r0, r4
003b5fec: mov r1, r8
003b5ff0: mov r2, sl
003b5ff4: bl #0x31a4d4
003b5ff8: mov r0, r4
003b5ffc: mov r1, r8
003b6000: mov r2, sl
003b6004: bl #0x319af4
003b6008: ldr r3, [pc, #0x81c]
003b600c: ldr r7, [pc, #0x81c]
003b6010: ldr r8, [pc, #0x81c]
003b6014: ldr sl, [r6, r3]
003b6018: add r7, pc, r7
003b601c: mov r3, r5
003b6020: mov r0, r4
003b6024: mov r1, r7
003b6028: mov r2, sl
003b602c: bl #0x31a4d4
003b6030: add r8, pc, r8
003b6034: mov r0, r4
003b6038: mov r1, r7
003b603c: mov r2, sl
003b6040: bl #0x319af4
003b6044: mov r3, r5
003b6048: mov r0, r4
003b604c: mov r1, r8
003b6050: mov r2, sl
003b6054: bl #0x31a4d4
003b6058: mov r0, r4
003b605c: mov r1, r8
003b6060: mov r2, sl
003b6064: bl #0x319af4
003b6068: ldr r2, [pc, #0x7c8]
003b606c: ldr r7, [pc, #0x7c8]
003b6070: mov r3, r5
003b6074: ldr r8, [r6, r2]
003b6078: add r7, pc, r7
003b607c: mov r0, r4
003b6080: mov r1, r7
003b6084: mov r2, r8
003b6088: bl #0x31a4d4
003b608c: mov r0, r4
003b6090: mov r1, r7
003b6094: mov r2, r8
003b6098: bl #0x319af4
003b609c: ldr r2, [pc, #0x79c]
003b60a0: ldr r7, [pc, #0x79c]
003b60a4: mov r3, r5
003b60a8: ldr r8, [r6, r2]
003b60ac: add r7, pc, r7
003b60b0: mov r0, r4
003b60b4: mov r1, r7
003b60b8: mov r2, r8
003b60bc: bl #0x31a4d4
003b60c0: mov r0, r4
003b60c4: mov r1, r7
003b60c8: mov r2, r8
003b60cc: bl #0x319af4
003b60d0: ldr r2, [pc, #0x770]
003b60d4: ldr r7, [pc, #0x770]
003b60d8: mov r3, r5
003b60dc: ldr r8, [r6, r2]
003b60e0: add r7, pc, r7
003b60e4: mov r0, r4
003b60e8: mov r1, r7
003b60ec: mov r2, r8
003b60f0: bl #0x31a4d4
003b60f4: mov r0, r4
003b60f8: mov r1, r7
003b60fc: mov r2, r8
003b6100: bl #0x319af4
003b6104: ldr r2, [pc, #0x744]
003b6108: ldr r7, [pc, #0x744]
003b610c: mov r3, r5
003b6110: ldr r8, [r6, r2]
003b6114: add r7, pc, r7
003b6118: mov r0, r4
003b611c: mov r1, r7
003b6120: mov r2, r8
003b6124: bl #0x31a4d4
003b6128: mov r0, r4
003b612c: mov r1, r7
003b6130: mov r2, r8
003b6134: bl #0x319af4
003b6138: ldr r2, [pc, #0x718]
003b613c: ldr r7, [pc, #0x718]
003b6140: mov r3, r5
003b6144: ldr r8, [r6, r2]
003b6148: add r7, pc, r7
003b614c: mov r0, r4
003b6150: mov r1, r7
003b6154: mov r2, r8
003b6158: bl #0x31a4d4
003b615c: mov r0, r4
003b6160: mov r1, r7
003b6164: mov r2, r8
003b6168: bl #0x319af4
003b616c: ldr r3, [pc, #0x6ec]
003b6170: ldr r7, [pc, #0x6ec]
003b6174: ldr r8, [pc, #0x6ec]
003b6178: ldr sl, [r6, r3]
003b617c: add r7, pc, r7
003b6180: mov r3, r5
003b6184: mov r0, r4
003b6188: mov r1, r7
003b618c: mov r2, sl
003b6190: bl #0x31a4d4
003b6194: add r8, pc, r8
003b6198: mov r0, r4
003b619c: mov r1, r7
003b61a0: mov r2, sl
003b61a4: bl #0x319af4
003b61a8: mov r3, r5
003b61ac: mov r0, r4
003b61b0: mov r1, r8
003b61b4: mov r2, sl
003b61b8: bl #0x31a4d4
003b61bc: mov r0, r4
003b61c0: mov r1, r8
003b61c4: mov r2, sl
003b61c8: bl #0x319af4
003b61cc: ldr r2, [pc, #0x698]
003b61d0: ldr r7, [pc, #0x698]
003b61d4: mov r3, r5
003b61d8: ldr r8, [r6, r2]
003b61dc: add r7, pc, r7
003b61e0: mov r0, r4
003b61e4: mov r1, r7
003b61e8: mov r2, r8
003b61ec: bl #0x31a4d4
003b61f0: mov r0, r4
003b61f4: mov r1, r7
003b61f8: mov r2, r8
003b61fc: bl #0x319af4
003b6200: ldr r2, [pc, #0x66c]
003b6204: ldr r7, [pc, #0x66c]
003b6208: mov r3, r5
003b620c: ldr r8, [r6, r2]
003b6210: add r7, pc, r7
003b6214: mov r0, r4
003b6218: mov r1, r7
003b621c: mov r2, r8
003b6220: bl #0x31a4d4
003b6224: mov r0, r4
003b6228: mov r1, r7
003b622c: mov r2, r8
003b6230: bl #0x319af4
003b6234: ldr r2, [pc, #0x640]
003b6238: ldr r7, [pc, #0x640]
003b623c: mov r3, r5
003b6240: ldr r8, [r6, r2]
003b6244: add r7, pc, r7
003b6248: mov r0, r4
003b624c: mov r1, r7
003b6250: mov r2, r8
003b6254: bl #0x31a4d4
003b6258: mov r0, r4
003b625c: mov r1, r7
003b6260: mov r2, r8
003b6264: bl #0x319af4
003b6268: ldr r2, [pc, #0x614]
003b626c: ldr r7, [pc, #0x614]
003b6270: mov r3, r5
003b6274: ldr r8, [r6, r2]
003b6278: add r7, pc, r7
003b627c: mov r0, r4
003b6280: mov r1, r7
003b6284: mov r2, r8
003b6288: bl #0x31a4d4
003b628c: mov r0, r4
003b6290: mov r1, r7
003b6294: mov r2, r8
003b6298: bl #0x319af4
003b629c: ldr r2, [pc, #0x5e8]
003b62a0: ldr r7, [pc, #0x5e8]
003b62a4: mov r3, r5
003b62a8: ldr r8, [r6, r2]
003b62ac: add r7, pc, r7
003b62b0: mov r0, r4
003b62b4: mov r1, r7
003b62b8: mov r2, r8
003b62bc: bl #0x31a4d4
003b62c0: mov r0, r4
003b62c4: mov r1, r7
003b62c8: mov r2, r8
003b62cc: bl #0x319af4
003b62d0: ldr r2, [pc, #0x5bc]
003b62d4: ldr r7, [pc, #0x5bc]
003b62d8: mov r3, r5
003b62dc: ldr r8, [r6, r2]
003b62e0: add r7, pc, r7
003b62e4: mov r0, r4
003b62e8: mov r1, r7
003b62ec: mov r2, r8
003b62f0: bl #0x31a4d4
003b62f4: mov r0, r4
003b62f8: mov r1, r7
003b62fc: mov r2, r8
003b6300: bl #0x319af4
003b6304: ldr r2, [pc, #0x590]
003b6308: ldr r7, [pc, #0x590]
003b630c: mov r3, r5
003b6310: ldr r8, [r6, r2]
003b6314: add r7, pc, r7
003b6318: mov r0, r4
003b631c: mov r1, r7
003b6320: mov r2, r8
003b6324: bl #0x31a4d4
003b6328: mov r0, r4
003b632c: mov r1, r7
003b6330: mov r2, r8
003b6334: bl #0x319af4
003b6338: ldr r2, [pc, #0x564]
003b633c: ldr r7, [pc, #0x564]
003b6340: mov r3, r5
003b6344: ldr r8, [r6, r2]
003b6348: add r7, pc, r7
003b634c: mov r0, r4
003b6350: mov r1, r7
003b6354: mov r2, r8
003b6358: bl #0x31a4d4
003b635c: mov r0, r4
003b6360: mov r1, r7
003b6364: mov r2, r8
003b6368: bl #0x319af4
003b636c: ldr r2, [pc, #0x538]
003b6370: ldr r7, [pc, #0x538]
003b6374: mov r3, r5
003b6378: ldr r2, [r6, r2]
003b637c: add r7, pc, r7
003b6380: mov r0, r4
003b6384: mov r1, r7
003b6388: str r2, [sp, #4]
003b638c: bl #0x31a4d4
003b6390: mov r0, r4
003b6394: mov r1, r7
003b6398: ldr r2, [sp, #4]
003b639c: bl #0x319af4
003b63a0: ldr r2, [pc, #0x50c]
003b63a4: ldr r7, [pc, #0x50c]
003b63a8: mov r3, r5
003b63ac: ldr fp, [r6, r2]
003b63b0: add r7, pc, r7
003b63b4: mov r0, r4
003b63b8: mov r1, r7
003b63bc: mov r2, fp
003b63c0: bl #0x31a4d4
003b63c4: mov r0, r4
003b63c8: mov r1, r7
003b63cc: mov r2, fp
003b63d0: bl #0x319af4
003b63d4: ldr r2, [pc, #0x4e0]
003b63d8: ldr r7, [pc, #0x4e0]
003b63dc: mov r3, r5
003b63e0: ldr sb, [r6, r2]
003b63e4: add r7, pc, r7
003b63e8: mov r0, r4
003b63ec: mov r1, r7
003b63f0: mov r2, sb
003b63f4: bl #0x31a4d4
003b63f8: mov r0, r4
003b63fc: mov r1, r7
003b6400: mov r2, sb
003b6404: bl #0x319af4
003b6408: ldr r2, [pc, #0x4b4]
003b640c: ldr r7, [pc, #0x4b4]
003b6410: mov r3, r5
003b6414: ldr r8, [r6, r2]
003b6418: add r7, pc, r7
003b641c: mov r0, r4
003b6420: mov r1, r7
003b6424: mov r2, r8
003b6428: bl #0x31a4d4
003b642c: mov r0, r4
003b6430: mov r1, r7
003b6434: mov r2, r8
003b6438: bl #0x319af4
003b643c: ldr r2, [pc, #0x488]
003b6440: ldr r7, [pc, #0x488]
003b6444: mov r3, r5
003b6448: ldr r8, [r6, r2]
003b644c: add r7, pc, r7
003b6450: mov r0, r4
003b6454: mov r1, r7
003b6458: mov r2, r8
003b645c: bl #0x31a4d4
003b6460: mov r0, r4
003b6464: mov r1, r7
003b6468: mov r2, r8
003b646c: bl #0x319af4
003b6470: ldr r3, [pc, #0x45c]
003b6474: ldr r7, [pc, #0x45c]
003b6478: mov r0, r4
003b647c: ldr ip, [r6, r3]
003b6480: add r7, pc, r7
003b6484: mov r3, r5
003b6488: mov r1, r7
003b648c: mov r2, ip
003b6490: str ip, [sp]
003b6494: ldr r8, [pc, #0x440]
003b6498: bl #0x31a4d4
003b649c: ldr ip, [sp]
003b64a0: ldr sl, [pc, #0x438]
003b64a4: mov r0, r4
003b64a8: mov r2, ip
003b64ac: mov r1, r7
003b64b0: add r8, pc, r8
003b64b4: bl #0x319af4
003b64b8: ldr r7, [pc, #0x424]
003b64bc: mov r3, r5
003b64c0: mov r0, r4
003b64c4: mov r1, r8
003b64c8: ldr r2, [sp, #4]
003b64cc: bl #0x31a4d4
003b64d0: add sl, pc, sl
003b64d4: mov r0, r4
003b64d8: mov r1, r8
003b64dc: ldr r2, [sp, #4]
003b64e0: bl #0x319af4
003b64e4: mov r3, r5
003b64e8: mov r0, r4
003b64ec: mov r1, sl
003b64f0: mov r2, fp
003b64f4: bl #0x31a4d4
003b64f8: add r7, pc, r7
003b64fc: mov r0, r4
003b6500: mov r1, sl
003b6504: mov r2, fp
003b6508: bl #0x319af4
003b650c: mov r3, r5
003b6510: mov r0, r4
003b6514: mov r1, r7
003b6518: mov r2, sb
003b651c: bl #0x31a4d4
003b6520: mov r0, r4
003b6524: mov r1, r7
003b6528: mov r2, sb
003b652c: bl #0x319af4
003b6530: ldr r3, [pc, #0x3b0]
003b6534: ldr r7, [pc, #0x3b0]
003b6538: ldr r8, [pc, #0x3b0]
003b653c: ldr sl, [r6, r3]
003b6540: add r7, pc, r7
003b6544: mov r3, r5
003b6548: mov r0, r4
003b654c: mov r1, r7
003b6550: mov r2, sl
003b6554: bl #0x31a4d4
003b6558: add r8, pc, r8
003b655c: mov r0, r4
003b6560: mov r1, r7
003b6564: mov r2, sl
003b6568: bl #0x319af4
003b656c: mov r3, r5
003b6570: mov r0, r4
003b6574: mov r1, r8
003b6578: mov r2, sl
003b657c: bl #0x31a4d4
003b6580: mov r0, r4
003b6584: mov r1, r8
003b6588: mov r2, sl
003b658c: bl #0x319af4
003b6590: ldr r2, [pc, #0x35c]
003b6594: ldr r7, [pc, #0x35c]
003b6598: mov r3, r5
003b659c: ldr r8, [r6, r2]
003b65a0: add r7, pc, r7
003b65a4: mov r0, r4
003b65a8: mov r1, r7
003b65ac: mov r2, r8
003b65b0: bl #0x31a4d4
003b65b4: mov r0, r4
003b65b8: mov r1, r7
003b65bc: mov r2, r8
003b65c0: bl #0x319af4
003b65c4: ldr r2, [pc, #0x330]
003b65c8: ldr r7, [pc, #0x330]
003b65cc: mov r3, r5
003b65d0: ldr r8, [r6, r2]
003b65d4: add r7, pc, r7
003b65d8: mov r0, r4
003b65dc: mov r1, r7
003b65e0: mov r2, r8
003b65e4: bl #0x31a4d4
003b65e8: mov r0, r4
003b65ec: mov r1, r7
003b65f0: mov r2, r8
003b65f4: bl #0x319af4
003b65f8: ldr r2, [pc, #0x304]
003b65fc: ldr r7, [pc, #0x304]
003b6600: mov r3, r5
003b6604: ldr r8, [r6, r2]
003b6608: add r7, pc, r7
003b660c: mov r0, r4
003b6610: mov r1, r7
003b6614: mov r2, r8
003b6618: bl #0x31a4d4
003b661c: mov r0, r4
003b6620: mov r1, r7
003b6624: mov r2, r8
003b6628: bl #0x319af4
003b662c: ldr r2, [pc, #0x2d8]
003b6630: ldr r7, [pc, #0x2d8]
003b6634: mov r3, r5
003b6638: ldr r8, [r6, r2]
003b663c: add r7, pc, r7
003b6640: mov r0, r4
003b6644: mov r1, r7
003b6648: mov r2, r8
003b664c: bl #0x31a4d4
003b6650: mov r0, r4
003b6654: mov r1, r7
003b6658: mov r2, r8
003b665c: bl #0x319af4
003b6660: ldr r2, [pc, #0x2ac]
003b6664: ldr r7, [pc, #0x2ac]
003b6668: mov r3, r5
003b666c: ldr r8, [r6, r2]
003b6670: add r7, pc, r7
003b6674: mov r0, r4
003b6678: mov r1, r7
003b667c: mov r2, r8
003b6680: bl #0x31a4d4
003b6684: mov r0, r4
003b6688: mov r1, r7
003b668c: mov r2, r8
003b6690: bl #0x319af4
003b6694: ldr r2, [pc, #0x280]
003b6698: ldr r7, [pc, #0x280]
003b669c: mov r3, r5
003b66a0: ldr r8, [r6, r2]
003b66a4: add r7, pc, r7
003b66a8: mov r0, r4
003b66ac: mov r1, r7
003b66b0: mov r2, r8
003b66b4: bl #0x31a4d4
003b66b8: mov r0, r4
003b66bc: b #0x3b6974
003b66c0: ldrheq pc, [sp], #-0x38
003b66c4: andeq r2, r0, r8, ror r6
003b66c8: subseq lr, r0, r0, asr r8
003b66cc: andeq r3, r0, r8, asr #6
003b66d0: subseq lr, r0, r4, lsr r8
003b66d4: strheq r2, [r0], -r8
003b66d8: subseq lr, r0, r0, lsl r8
003b66dc: strheq r2, [r0], -ip
003b66e0: subseq fp, r0, r4, lsr #29
003b66e4: andeq r2, r0, r8, lsr #17
003b66e8: ldrheq lr, [r0], #-0x70
003b66ec: andeq r4, r0, ip, lsl #24
003b66f0: subseq lr, r0, r4, lsl #15
003b66f4: andeq r1, r0, r0, ror #22
003b66f8: subseq lr, r0, r8, asr r7
003b66fc: andeq r3, r0, r8, asr #21
003b6700: subseq lr, r0, ip, lsr #14
003b6704: andeq r1, r0, r0, lsl #26
003b6708: subseq lr, r0, r8, lsl #14
003b670c: ldrdeq r0, r1, [r0], -ip
003b6710: ldrsbeq lr, [r0], #-0x6c
003b6714: strheq r2, [r0], -r8
003b6718: ldrheq lr, [r0], #-0x60
003b671c: strdeq r1, r2, [r0], -r0
003b6720: subseq lr, r0, r4, lsl #13
003b6724: strheq r4, [r0], -r4
003b6728: subseq lr, r0, r0, ror #12
003b672c: strheq r2, [r0], -r8
003b6730: subseq lr, r0, ip, lsr r6
003b6734: andeq r1, r0, r4, lsr #9
003b6738: subseq lr, r0, r0, lsl r6
003b673c: strdeq r4, r5, [r0], -r8
003b6740: subseq lr, r0, ip, ror #11
003b6744: andeq r4, r0, r0, ror #14
003b6748: subseq lr, r0, r8, asr #11
003b674c: strdeq r0, r1, [r0], -r8
003b6750: subseq lr, r0, r4, lsr #11
003b6754: andeq r2, r0, r4, asr #9
003b6758: subseq lr, r0, r0, lsl #11
003b675c: andeq r4, r0, r8, lsr r1
003b6760: subseq lr, r0, ip, asr r5
003b6764: andeq r1, r0, r4, lsl #16
003b6768: subseq lr, r0, r8, lsr r5
003b676c: andeq r0, r0, r0, lsl lr
003b6770: subseq lr, r0, r4, lsl r5
003b6774: strdeq r1, r2, [r0], -r8
003b6778: ldrsheq lr, [r0], #-0x40
003b677c: ldrsheq lr, [r0], #-0x40
003b6780: muleq r0, r0, r7
003b6784: ldrheq lr, [r0], #-0x48
003b6788: ldrdeq r0, r1, [r0], -ip

_ZN9LuaScript6_BitOrERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37e814 472
0037e814: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037e818: ldr r4, [r0, #4]
0037e81c: mov sl, r1
0037e820: sub sp, sp, #4
0037e824: ldm r4, {r2, r3}
0037e828: mov r5, r0
0037e82c: rsb r3, r2, r3
0037e830: asr r1, r3, #4
0037e834: add r8, r1, r1, lsl #3
0037e838: add r8, r8, r8, lsl #6
0037e83c: add r8, r1, r8, lsl #3
0037e840: add r8, r8, r8, lsl #15
0037e844: add r8, r1, r8, lsl #3
0037e848: rsb r8, r8, #0
0037e84c: cmp r8, #1
0037e850: bls #0x37e9d8
0037e854: ldr sb, [pc, #0x184]
0037e858: mov r7, #0
0037e85c: mov r6, r7
0037e860: add sb, pc, sb
0037e864: b #0x37e874
0037e868: ldr r4, [r5, #4]
0037e86c: ldm r4, {r2, r3}
0037e870: rsb r3, r2, r3
0037e874: asr r3, r3, #4
0037e878: add r1, r3, r3, lsl #3
0037e87c: add r1, r1, r1, lsl #6
0037e880: add r1, r3, r1, lsl #3
0037e884: add r1, r1, r1, lsl #15
0037e888: add r3, r3, r1, lsl #3
0037e88c: rsb r3, r3, #0
0037e890: cmp r6, r3
0037e894: add r6, r6, #1
0037e898: blo #0x37e8a8
0037e89c: mov r0, sb
0037e8a0: bl #0x708eb0
0037e8a4: ldr r2, [r4]
0037e8a8: add r2, r2, r7
0037e8ac: ldr r3, [r2, #4]
0037e8b0: add r7, r7, #0x70
0037e8b4: cmp r3, #3
0037e8b8: bne #0x37e9d8
0037e8bc: cmp r6, r8
0037e8c0: bne #0x37e868
0037e8c4: ldr r4, [r5, #4]
0037e8c8: ldm r4, {r0, r3}
0037e8cc: rsb r3, r0, r3
0037e8d0: asr r3, r3, #4
0037e8d4: add r2, r3, r3, lsl #3
0037e8d8: add r2, r2, r2, lsl #6
0037e8dc: add r2, r3, r2, lsl #3
0037e8e0: add r2, r2, r2, lsl #15
0037e8e4: add r3, r3, r2, lsl #3
0037e8e8: cmp r3, #0
0037e8ec: bne #0x37e900
0037e8f0: ldr r0, [pc, #0xec]
0037e8f4: add r0, pc, r0
0037e8f8: bl #0x708eb0
0037e8fc: ldr r0, [r4]
0037e900: bl #0x31bbf0
0037e904: bl #0x30e4cc
0037e908: ldr r2, [r5, #4]
0037e90c: mov r8, r0
0037e910: ldm r2, {r2, r3}
0037e914: rsb r3, r2, r3
0037e918: asr r3, r3, #4
0037e91c: add sb, r3, r3, lsl #3
0037e920: add sb, sb, sb, lsl #6
0037e924: add sb, r3, sb, lsl #3
0037e928: add sb, sb, sb, lsl #15
0037e92c: add sb, r3, sb, lsl #3
0037e930: rsb sb, sb, #0
0037e934: cmp sb, #1
0037e938: bls #0x37e9c4
0037e93c: ldr fp, [pc, #0xa4]
0037e940: mov r7, #0x70
0037e944: mov r4, #1
0037e948: add fp, pc, fp
0037e94c: add r0, r2, r7
0037e950: bl #0x31bbf0
0037e954: bl #0x30e4cc
0037e958: add r4, r4, #1
0037e95c: cmp r4, sb
0037e960: orr r8, r8, r0
0037e964: beq #0x37e9c4
0037e968: ldr r6, [r5, #4]
0037e96c: mov r0, fp
0037e970: add r7, r7, #0x70
0037e974: ldm r6, {r2, r3}
0037e978: rsb r3, r2, r3
0037e97c: asr r3, r3, #4
0037e980: add r1, r3, r3, lsl #3
0037e984: add r1, r1, r1, lsl #6
0037e988: add r1, r3, r1, lsl #3
0037e98c: add r1, r1, r1, lsl #15
0037e990: add r3, r3, r1, lsl #3
0037e994: rsb r3, r3, #0
0037e998: cmp r4, r3
0037e99c: blo #0x37e94c
0037e9a0: bl #0x708eb0
0037e9a4: ldr r2, [r6]
0037e9a8: add r4, r4, #1
0037e9ac: add r0, r2, r7
0037e9b0: bl #0x31bbf0
0037e9b4: bl #0x30e4cc
0037e9b8: cmp r4, sb
0037e9bc: orr r8, r8, r0
0037e9c0: bne #0x37e968
0037e9c4: mov r0, sl
0037e9c8: mov r1, r8
0037e9cc: add sp, sp, #4
0037e9d0: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037e9d4: b #0x37cb24
0037e9d8: add sp, sp, #4
0037e9dc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037e9e0: subseq pc, r3, r8, lsl #24
0037e9e4: subseq pc, r3, r4, ror fp
0037e9e8: subseq pc, r3, r0, lsr #22

_ZN3sfc6script3lua5Error8setErrorEP9lua_Statei 0x31a8ac 108
0031a8ac: cmp r2, #0
0031a8b0: push {r4, r5, r6, lr}
0031a8b4: mov r4, r0
0031a8b8: mov r5, r1
0031a8bc: str r2, [r0, #4]
0031a8c0: bne #0x31a8dc
0031a8c4: ldr r1, [pc, #0x48]
0031a8c8: add r0, r0, #8
0031a8cc: add r1, pc, r1
0031a8d0: mov r2, r1
0031a8d4: pop {r4, r5, r6, lr}
0031a8d8: b #0x3109e0
0031a8dc: mvn r1, #0
0031a8e0: mov r2, #0
0031a8e4: mov r0, r5
0031a8e8: bl #0x84c384
0031a8ec: mov r6, r0
0031a8f0: bl #0x30de54
0031a8f4: mov r1, r6
0031a8f8: add r2, r6, r0
0031a8fc: add r0, r4, #8
0031a900: bl #0x3109e0
0031a904: mov r0, r5
0031a908: mvn r1, #1
0031a90c: pop {r4, r5, r6, lr}
0031a910: b #0x84b140
0031a914: subseq r0, fp, ip, lsr pc

_ZN12CharAIScript14_RegisterStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3da144 652
003da144: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003da148: ldr r4, [r0, #4]
003da14c: mov r5, r0
003da150: sub sp, sp, #0x1c
003da154: ldm r4, {r0, r3}
003da158: rsb r3, r0, r3
003da15c: asr r3, r3, #4
003da160: add r7, r3, r3, lsl #3
003da164: add r7, r7, r7, lsl #6
003da168: add r7, r3, r7, lsl #3
003da16c: add r7, r7, r7, lsl #15
003da170: add r7, r3, r7, lsl #3
003da174: rsb r7, r7, #0
003da178: cmp r7, #1
003da17c: bls #0x3da1b4
003da180: cmp r7, #5
003da184: bhi #0x3da1b4
003da188: mov r3, #0
003da18c: mov r1, r3
003da190: b #0x3da19c
003da194: cmp r1, r7
003da198: bhs #0x3da1bc
003da19c: add ip, r0, r3
003da1a0: ldr ip, [ip, #4]
003da1a4: add r1, r1, #1
003da1a8: add r3, r3, #0x70
003da1ac: cmp ip, #4
003da1b0: beq #0x3da194
003da1b4: add sp, sp, #0x1c
003da1b8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003da1bc: cmp r7, #0
003da1c0: add r6, r2, #0x9c
003da1c4: beq #0x3da3a8
003da1c8: bl #0x31c49c
003da1cc: add r1, sp, #0x18
003da1d0: str r0, [r1, #-4]!
003da1d4: mov r0, r6
003da1d8: bl #0x3d9fdc
003da1dc: ldr r3, [r5, #4]
003da1e0: mov r6, r0
003da1e4: ldm r3, {r0, r1}
003da1e8: rsb r1, r0, r1
003da1ec: asr r1, r1, #4
003da1f0: add r2, r1, r1, lsl #3
003da1f4: add r2, r2, r2, lsl #6
003da1f8: add r2, r1, r2, lsl #3
003da1fc: add r2, r2, r2, lsl #15
003da200: add r2, r1, r2, lsl #3
003da204: rsb r2, r2, #0
003da208: cmp r2, #1
003da20c: bls #0x3da1b4
003da210: ldr r1, [pc, #0x1a4]
003da214: ldr sb, [pc, #0x1a4]
003da218: ldr fp, [pc, #0x1a4]
003da21c: add r1, pc, r1
003da220: str r1, [sp, #8]
003da224: ldr r1, [pc, #0x19c]
003da228: add sb, pc, sb
003da22c: add fp, pc, fp
003da230: add r1, pc, r1
003da234: str r1, [sp, #0xc]
003da238: add sl, r6, #0x18
003da23c: add r8, r6, #0x30
003da240: add r7, r6, #0x48
003da244: mov r4, #1
003da248: sub ip, r4, #1
003da24c: cmp ip, #3
003da250: addls pc, pc, ip, lsl #2
003da254: b #0x3da2a8
003da258: b #0x3da364
003da25c: b #0x3da320
003da260: b #0x3da2dc
003da264: b #0x3da268
003da268: cmp r2, #4
003da26c: bhi #0x3da284
003da270: mov r0, sb
003da274: str r3, [sp, #4]
003da278: bl #0x708eb0
003da27c: ldr r3, [sp, #4]
003da280: ldr r0, [r3]
003da284: add r0, r0, #0x1c0
003da288: bl #0x31c49c
003da28c: str r0, [sp, #4]
003da290: bl #0x30de54
003da294: ldr r1, [sp, #4]
003da298: add r2, r1, r0
003da29c: mov r0, r7
003da2a0: bl #0x3109e0
003da2a4: ldr r3, [r5, #4]
003da2a8: ldm r3, {r0, r2}
003da2ac: add r4, r4, #1
003da2b0: rsb r2, r0, r2
003da2b4: asr r2, r2, #4
003da2b8: add r1, r2, r2, lsl #3
003da2bc: add r1, r1, r1, lsl #6
003da2c0: add r1, r2, r1, lsl #3
003da2c4: add r1, r1, r1, lsl #15
003da2c8: add r2, r2, r1, lsl #3
003da2cc: rsb r2, r2, #0
003da2d0: cmp r4, r2
003da2d4: blo #0x3da248
003da2d8: b #0x3da1b4
003da2dc: cmp r2, #3
003da2e0: bhi #0x3da2f8
003da2e4: mov r0, fp
003da2e8: str r3, [sp, #4]
003da2ec: bl #0x708eb0
003da2f0: ldr r3, [sp, #4]
003da2f4: ldr r0, [r3]
003da2f8: add r0, r0, #0x150
003da2fc: bl #0x31c49c
003da300: str r0, [sp, #4]
003da304: bl #0x30de54
003da308: ldr r1, [sp, #4]
003da30c: add r2, r1, r0
003da310: mov r0, r8
003da314: bl #0x3109e0
003da318: ldr r3, [r5, #4]
003da31c: b #0x3da2a8
003da320: cmp r2, #2
003da324: bhi #0x3da33c
003da328: ldr r0, [sp, #8]
003da32c: str r3, [sp, #4]
003da330: bl #0x708eb0
003da334: ldr r3, [sp, #4]
003da338: ldr r0, [r3]
003da33c: add r0, r0, #0xe0
003da340: bl #0x31c49c
003da344: str r0, [sp, #4]
003da348: bl #0x30de54
003da34c: ldr r1, [sp, #4]
003da350: add r2, r1, r0
003da354: mov r0, sl
003da358: bl #0x3109e0
003da35c: ldr r3, [r5, #4]
003da360: b #0x3da2a8
003da364: cmp r2, #1
003da368: bhi #0x3da380
003da36c: ldr r0, [sp, #0xc]
003da370: str r3, [sp, #4]
003da374: bl #0x708eb0
003da378: ldr r3, [sp, #4]
003da37c: ldr r0, [r3]
003da380: add r0, r0, #0x70
003da384: bl #0x31c49c
003da388: str r0, [sp, #4]
003da38c: bl #0x30de54
003da390: ldr r1, [sp, #4]
003da394: add r2, r1, r0
003da398: mov r0, r6
003da39c: bl #0x3109e0
003da3a0: ldr r3, [r5, #4]
003da3a4: b #0x3da2a8
003da3a8: ldr r0, [pc, #0x1c]
003da3ac: add r0, pc, r0
003da3b0: bl #0x708eb0
003da3b4: ldr r0, [r4]
003da3b8: b #0x3da1c8
003da3bc: subeq r4, lr, ip, asr #4
003da3c0: subeq r4, lr, r0, asr #4
003da3c4: subeq r4, lr, ip, lsr r2
003da3c8: subeq r4, lr, r8, lsr r2
003da3cc: strheq r4, [lr], #-0xc

_ZN10GameObject18_IsTargetListEmptyERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38e970 28
0038e970: ldr r3, [r2, #0x304]
0038e974: ldr r2, [r2, #0x314]
0038e978: mov r0, r1
0038e97c: cmp r2, r3
0038e980: movne r1, #0
0038e984: moveq r1, #1
0038e988: b #0x37c7e4

_ZN9Character24_SetSpellCooldownTimerIdERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b90e4 188
003b90e4: push {r4, lr}
003b90e8: ldr r3, [r0, #4]
003b90ec: sub sp, sp, #8
003b90f0: ldr r1, [r3, #4]
003b90f4: ldr ip, [r3]
003b90f8: rsb r3, ip, r1
003b90fc: asr r3, r3, #4
003b9100: add r1, r3, r3, lsl #3
003b9104: add r1, r1, r1, lsl #6
003b9108: add r1, r3, r1, lsl #3
003b910c: add r1, r1, r1, lsl #15
003b9110: add r3, r3, r1, lsl #3
003b9114: cmp r3, #0
003b9118: bne #0x3b9124
003b911c: add sp, sp, #8
003b9120: pop {r4, pc}
003b9124: ldr r3, [ip, #4]
003b9128: cmp r3, #3
003b912c: beq #0x3b917c
003b9130: cmp r3, #0
003b9134: bne #0x3b911c
003b9138: mvn r4, #0
003b913c: mov r0, r2
003b9140: str r2, [sp, #4]
003b9144: bl #0x3ae5dc
003b9148: ldr r0, [r0, #4]
003b914c: ldr r2, [sp, #4]
003b9150: cmp r0, #0
003b9154: beq #0x3b911c
003b9158: mov r3, #0
003b915c: ldr r1, [r2, #0x488]
003b9160: ldr r1, [r1, r3, lsl #2]
003b9164: add r3, r3, #1
003b9168: cmp r1, #0
003b916c: strne r4, [r1, #0x18]
003b9170: cmp r3, r0
003b9174: bne #0x3b915c
003b9178: b #0x3b911c
003b917c: cmp r3, #0
003b9180: beq #0x3b9138
003b9184: mov r1, #0
003b9188: str r2, [sp, #4]
003b918c: bl #0x37baf8
003b9190: bl #0x38d798
003b9194: ldr r2, [sp, #4]
003b9198: mov r4, r0
003b919c: b #0x3b913c

_ZN9Character12_ClearMasterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b5678 12
003b5678: add r0, r2, #0x3c8
003b567c: mov r1, #0
003b5680: b #0x3d4d80

_ZN3sfc6script3lua12ReturnValues10pushStringEPKc 0x37c8cc 104
0037c8cc: ldr r3, [pc, #0x58]
0037c8d0: ldr r2, [pc, #0x58]
0037c8d4: push {r4, r5, r6, lr}
0037c8d8: add r3, pc, r3
0037c8dc: ldr r5, [r3, r2]
0037c8e0: sub sp, sp, #0x78
0037c8e4: add r4, sp, #4
0037c8e8: ldr r3, [r5]
0037c8ec: str r3, [sp, #0x74]
0037c8f0: ldr r6, [r0, #0x24]
0037c8f4: mov r0, r4
0037c8f8: bl #0x37c84c
0037c8fc: mov r0, r6
0037c900: mov r1, r4
0037c904: bl #0x3195c0
0037c908: mov r0, r4
0037c90c: bl #0x3193e8
0037c910: ldr r2, [sp, #0x74]
0037c914: ldr r3, [r5]
0037c918: cmp r2, r3
0037c91c: bne #0x37c928
0037c920: add sp, sp, #0x78
0037c924: pop {r4, r5, r6, pc}
0037c928: bl #0x30e310
0037c92c: strhteq r8, [r1], #-0x18
0037c930: andeq r4, r0, ip, lsr #1

_ZN3sfc6script3lua9Arguments11pushIntegerEi 0x3cdd78 180
003cdd78: push {r4, r5, r6, r7, lr}
003cdd7c: ldr r4, [pc, #0x9c]
003cdd80: ldr r6, [pc, #0x9c]
003cdd84: sub sp, sp, #0x7c
003cdd88: add r4, pc, r4
003cdd8c: ldr r3, [r4, r6]
003cdd90: add r5, sp, #4
003cdd94: ldr r3, [r3]
003cdd98: str r3, [sp, #0x74]
003cdd9c: ldr r7, [r0, #4]
003cdda0: mov r0, r5
003cdda4: bl #0x37ca9c
003cdda8: mov r1, r5
003cddac: mov r0, r7
003cddb0: bl #0x3195c0
003cddb4: ldr r3, [pc, #0x6c]
003cddb8: add r0, r5, #0x24
003cddbc: add r5, r5, #0xc
003cddc0: ldr r3, [r4, r3]
003cddc4: add r3, r3, #8
003cddc8: str r3, [sp, #4]
003cddcc: bl #0x3193b0
003cddd0: ldr r0, [sp, #0x24]
003cddd4: cmp r0, r5
003cddd8: beq #0x3cddf8
003cdddc: cmp r0, #0
003cdde0: beq #0x3cddf8
003cdde4: ldr r1, [sp, #0x10]
003cdde8: rsb r1, r0, r1
003cddec: cmp r1, #0x80
003cddf0: bhi #0x3cde14
003cddf4: bl #0x708f00
003cddf8: ldr r3, [r4, r6]
003cddfc: ldr r2, [sp, #0x74]
003cde00: ldr r3, [r3]
003cde04: cmp r2, r3
003cde08: bne #0x3cde1c
003cde0c: add sp, sp, #0x7c
003cde10: pop {r4, r5, r6, r7, pc}
003cde14: bl #0x310440
003cde18: b #0x3cddf8
003cde1c: bl #0x30e310
003cde20: subseq r6, ip, r8, lsl #26
003cde24: andeq r4, r0, ip, lsr #1
003cde28: muleq r0, r8, r7

_ZN9LuaScript14_GetNumPlayersERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37cbd8 40
0037cbd8: ldr r3, [pc, #0x18]
0037cbdc: ldr r2, [pc, #0x18]
0037cbe0: mov r0, r1
0037cbe4: add r3, pc, r3
0037cbe8: ldr r2, [r3, r2]
0037cbec: ldr r3, [r2, #0x40]
0037cbf0: ldr r1, [r3, #0x6c4]
0037cbf4: b #0x37cb24
0037cbf8: rsbeq r7, r1, ip, lsr #29
0037cbfc: strdeq r3, r4, [r0], -r4

_ZN3sfc6script3lua6BinderD1Ev 0x31b57c 4
0031b57c: bx lr

_ZN3sfc6script3lua8Instance8execFileER11IFileStreamRNS1_12ReturnValuesE 0x31ac24 208
0031ac24: push {r4, r5, r6, r7, r8, lr}
0031ac28: ldr r4, [pc, #0xb4]
0031ac2c: ldr r8, [pc, #0xb4]
0031ac30: sub sp, sp, #0x410
0031ac34: add r4, pc, r4
0031ac38: ldr r3, [r4, r8]
0031ac3c: sub sp, sp, #8
0031ac40: ldr r7, [r0, #4]
0031ac44: ldr ip, [r3]
0031ac48: ldr r3, [pc, #0x9c]
0031ac4c: str r1, [sp, #4]
0031ac50: str ip, [sp, #0x414]
0031ac54: ldr r1, [r4, r3]
0031ac58: mov ip, #0
0031ac5c: ldr r3, [pc, #0x8c]
0031ac60: str ip, [sp, #8]
0031ac64: add ip, sp, #0x18
0031ac68: mov r5, r2
0031ac6c: sub ip, ip, #4
0031ac70: add r2, sp, #8
0031ac74: mov r6, r0
0031ac78: sub r2, r2, #8
0031ac7c: add r3, pc, r3
0031ac80: str ip, [sp, #0xc]
0031ac84: mov r0, r7
0031ac88: mov ip, #0x400
0031ac8c: str ip, [sp, #0x10]
0031ac90: str r6, [sp]
0031ac94: bl #0x84bbbc
0031ac98: mov r2, r0
0031ac9c: mov r1, r7
0031aca0: add r0, r5, #4
0031aca4: bl #0x31a8ac
0031aca8: ldr r1, [r5, #8]
0031acac: cmp r1, #0
0031acb0: bne #0x31acc0
0031acb4: mov r0, r6
0031acb8: mov r2, r5
0031acbc: bl #0x31aa78
0031acc0: ldr r3, [r4, r8]
0031acc4: ldr r2, [sp, #0x414]
0031acc8: ldr r3, [r3]
0031accc: cmp r2, r3
0031acd0: bne #0x31ace0
0031acd4: add sp, sp, #0x18
0031acd8: add sp, sp, #0x400
0031acdc: pop {r4, r5, r6, r7, r8, pc}
0031ace0: bl #0x30e310
0031ace4: rsbeq sb, r7, ip, asr lr
0031ace8: andeq r4, r0, ip, lsr #1
0031acec: andeq r0, r0, r4, asr sb
0031acf0: subseq r3, sl, ip, ror #23

_ZN11TriggerTrap11_GetDamagerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x39ea64 44
0039ea64: push {r4, lr}
0039ea68: ldr r3, [r2]
0039ea6c: mov r0, r2
0039ea70: mov r4, r1
0039ea74: mov lr, pc
0039ea78: ldr pc, [r3, #0xe4]
0039ea7c: mov r3, r0
0039ea80: mov r1, r3
0039ea84: mov r0, r4
0039ea88: pop {r4, lr}
0039ea8c: b #0x37cb24

_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EEC1ERKS5_ 0x31c760 208
0031c760: push {r4, r5, r6, r7, r8, sl, lr}
0031c764: ldr r2, [r1, #4]
0031c768: ldr r3, [r1]
0031c76c: mov r5, r1
0031c770: sub sp, sp, #0xc
0031c774: rsb r3, r3, r2
0031c778: asr r3, r3, #4
0031c77c: mov r4, r0
0031c780: add r1, r3, r3, lsl #3
0031c784: mov r6, #0
0031c788: add r1, r1, r1, lsl #6
0031c78c: add r2, sp, #8
0031c790: add r1, r3, r1, lsl #3
0031c794: str r6, [r4]
0031c798: add r1, r1, r1, lsl #15
0031c79c: str r6, [r4, #4]
0031c7a0: add r1, r3, r1, lsl #3
0031c7a4: rsb r1, r1, #0
0031c7a8: str r6, [r0, #8]!
0031c7ac: str r1, [r2, #-4]!
0031c7b0: bl #0x319538
0031c7b4: ldr r3, [sp, #4]
0031c7b8: mov r2, #0x70
0031c7bc: str r0, [r4]
0031c7c0: mla r3, r2, r3, r0
0031c7c4: stmib r4, {r0, r3}
0031c7c8: ldr r3, [r5, #4]
0031c7cc: ldr r8, [r5]
0031c7d0: mov r7, r0
0031c7d4: rsb r3, r8, r3
0031c7d8: asr r3, r3, #4
0031c7dc: add sl, r3, r3, lsl #3
0031c7e0: add sl, sl, sl, lsl #6
0031c7e4: add sl, r3, sl, lsl #3
0031c7e8: add sl, sl, sl, lsl #15
0031c7ec: add sl, r3, sl, lsl #3
0031c7f0: rsb sl, sl, #0
0031c7f4: cmp sl, r6
0031c7f8: ble #0x31c820
0031c7fc: mov r5, sl
0031c800: add r0, r7, r6
0031c804: add r1, r8, r6
0031c808: bl #0x31c634
0031c80c: subs r5, r5, #1
0031c810: add r6, r6, #0x70
0031c814: bne #0x31c800
0031c818: mov r3, #0x70
0031c81c: mla r7, r3, sl, r7
0031c820: str r7, [r4, #4]
0031c824: mov r0, r4
0031c828: add sp, sp, #0xc
0031c82c: pop {r4, r5, r6, r7, r8, sl, pc}

_ZN9Character9_AddAggroERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b8098 588
003b8098: push {r4, r5, r6, r7, r8, lr}
003b809c: ldr r4, [pc, #0x224]
003b80a0: ldr r5, [pc, #0x224]
003b80a4: ldr r8, [r0, #4]
003b80a8: add r4, pc, r4
003b80ac: ldr r3, [r4, r5]
003b80b0: sub sp, sp, #0x20
003b80b4: mov r7, r2
003b80b8: ldr r3, [r3]
003b80bc: mov r6, r0
003b80c0: str r3, [sp, #0x1c]
003b80c4: ldm r8, {r2, r3}
003b80c8: rsb r3, r2, r3
003b80cc: asr r3, r3, #4
003b80d0: add r1, r3, r3, lsl #3
003b80d4: add r1, r1, r1, lsl #6
003b80d8: add r1, r3, r1, lsl #3
003b80dc: add r1, r1, r1, lsl #15
003b80e0: add r3, r3, r1, lsl #3
003b80e4: rsb r3, r3, #0
003b80e8: cmp r3, #1
003b80ec: bls #0x3b811c
003b80f0: cmp r3, #0
003b80f4: movne r1, r2
003b80f8: beq #0x3b823c
003b80fc: ldr r2, [r2, #4]
003b8100: cmp r2, #2
003b8104: beq #0x3b8160
003b8108: cmp r3, #0
003b810c: beq #0x3b8288
003b8110: ldr r3, [r1, #4]
003b8114: cmp r3, #7
003b8118: beq #0x3b8138
003b811c: ldr r3, [r4, r5]
003b8120: ldr r2, [sp, #0x1c]
003b8124: ldr r3, [r3]
003b8128: cmp r2, r3
003b812c: bne #0x3b82c4
003b8130: add sp, sp, #0x20
003b8134: pop {r4, r5, r6, r7, r8, pc}
003b8138: ldr r8, [r6, #4]
003b813c: ldm r8, {r1, r2}
003b8140: rsb r2, r1, r2
003b8144: asr r2, r2, #4
003b8148: add r3, r2, r2, lsl #3
003b814c: add r3, r3, r3, lsl #6
003b8150: add r3, r2, r3, lsl #3
003b8154: add r3, r3, r3, lsl #15
003b8158: add r3, r2, r3, lsl #3
003b815c: rsb r3, r3, #0
003b8160: cmp r3, #1
003b8164: bls #0x3b82a8
003b8168: ldr r3, [r1, #0x74]
003b816c: cmp r3, #3
003b8170: bne #0x3b811c
003b8174: mov r1, #0
003b8178: mov r0, r6
003b817c: bl #0x37baf8
003b8180: bl #0x31b5a0
003b8184: mov r1, #1
003b8188: mov r8, r0
003b818c: mov r0, r6
003b8190: bl #0x37baf8
003b8194: bl #0x31bbf0
003b8198: mov r1, r8
003b819c: mov r2, r0
003b81a0: add r0, r7, #0x3c8
003b81a4: bl #0x3d7c68
003b81a8: mov r1, #0
003b81ac: bl #0x30e2f8
003b81b0: cmp r0, #0
003b81b4: beq #0x3b811c
003b81b8: ldr r3, [pc, #0x110]
003b81bc: add r6, sp, #4
003b81c0: ldr r7, [r4, r3]
003b81c4: mov r0, r7
003b81c8: bl #0x337888
003b81cc: mov r0, r6
003b81d0: mov r1, #0x16
003b81d4: str r6, [sp, #0x14]
003b81d8: str r6, [sp, #0x18]
003b81dc: bl #0x31167c
003b81e0: ldr r1, [pc, #0xec]
003b81e4: mov r2, #0x15
003b81e8: ldr r0, [sp, #0x18]
003b81ec: add r1, pc, r1
003b81f0: bl #0x30e868
003b81f4: add r3, r0, #0x15
003b81f8: str r3, [sp, #0x14]
003b81fc: mov r3, #0
003b8200: strb r3, [r0, #0x15]
003b8204: mov r1, r6
003b8208: mov r0, r7
003b820c: bl #0x337a88
003b8210: ldr r0, [sp, #0x18]
003b8214: cmp r0, r6
003b8218: beq #0x3b811c
003b821c: cmp r0, #0
003b8220: beq #0x3b811c
003b8224: ldr r1, [sp, #4]
003b8228: rsb r1, r0, r1
003b822c: cmp r1, #0x80
003b8230: bhi #0x3b82bc
003b8234: bl #0x708f00
003b8238: b #0x3b811c
003b823c: ldr r0, [pc, #0x94]
003b8240: add r0, pc, r0
003b8244: bl #0x708eb0
003b8248: ldr r2, [r8]
003b824c: ldr r8, [r6, #4]
003b8250: ldr r2, [r2, #4]
003b8254: ldr r1, [r8]
003b8258: ldr r0, [r8, #4]
003b825c: cmp r2, #2
003b8260: rsb r0, r1, r0
003b8264: asr r0, r0, #4
003b8268: add r3, r0, r0, lsl #3
003b826c: add r3, r3, r3, lsl #6
003b8270: add r3, r0, r3, lsl #3
003b8274: add r3, r3, r3, lsl #15
003b8278: add r3, r0, r3, lsl #3
003b827c: rsb r3, r3, #0
003b8280: bne #0x3b8108
003b8284: b #0x3b8160
003b8288: ldr r0, [pc, #0x4c]
003b828c: add r0, pc, r0
003b8290: bl #0x708eb0
003b8294: ldr r1, [r8]
003b8298: ldr r3, [r1, #4]
003b829c: cmp r3, #7
003b82a0: bne #0x3b811c
003b82a4: b #0x3b8138
003b82a8: ldr r0, [pc, #0x30]
003b82ac: add r0, pc, r0
003b82b0: bl #0x708eb0
003b82b4: ldr r1, [r8]
003b82b8: b #0x3b8168
003b82bc: bl #0x310440
003b82c0: b #0x3b811c
003b82c4: bl #0x30e310
003b82c8: subseq ip, sp, r8, ror #19
003b82cc: andeq r4, r0, ip, lsr #1
003b82d0: andeq r0, r0, r4, lsl #17
003b82d4: ldrsheq fp, [r0], #-0xa4
003b82d8: subseq r6, r0, r8, lsr #4
003b82dc: ldrsbeq r6, [r0], #-0x1c
003b82e0: ldrheq r6, [r0], #-0x1c

_ZN3sfc6script3lua8Instance11includeBaseEv 0x31b010 8
0031b010: ldr r0, [r0, #4]
0031b014: b #0x84dbfc

_ZN7TestUD214createBindingsERN3sfc6script3lua6BinderE 0x386da4 92
00386da4: push {r4, r5, r6, r7, r8, lr}
00386da8: ldr r4, [pc, #0x44]
00386dac: mov r7, r1
00386db0: mov r8, r0
00386db4: bl #0x386d00
00386db8: ldr r3, [pc, #0x38]
00386dbc: add r4, pc, r4
00386dc0: ldr r5, [pc, #0x34]
00386dc4: ldr r6, [r4, r3]
00386dc8: mov r0, r7
00386dcc: add r5, pc, r5
00386dd0: mov r1, r5
00386dd4: mov r2, r6
00386dd8: mov r3, r8
00386ddc: bl #0x31a4d4
00386de0: mov r0, r7
00386de4: mov r1, r5
00386de8: mov r2, r6
00386dec: pop {r4, r5, r6, r7, r8, lr}
00386df0: b #0x319af4

_ZN3sfc6script3lua12ReturnValues11pushPointerEPv 0x38eb00 104
0038eb00: ldr r3, [pc, #0x58]
0038eb04: ldr r2, [pc, #0x58]
0038eb08: push {r4, r5, r6, lr}
0038eb0c: add r3, pc, r3
0038eb10: ldr r5, [r3, r2]
0038eb14: sub sp, sp, #0x78
0038eb18: add r4, sp, #4
0038eb1c: ldr r3, [r5]
0038eb20: str r3, [sp, #0x74]
0038eb24: ldr r6, [r0, #0x24]
0038eb28: mov r0, r4
0038eb2c: bl #0x31a414
0038eb30: mov r0, r6
0038eb34: mov r1, r4
0038eb38: bl #0x3195c0
0038eb3c: mov r0, r4
0038eb40: bl #0x3193e8
0038eb44: ldr r2, [sp, #0x74]
0038eb48: ldr r3, [r5]
0038eb4c: cmp r2, r3
0038eb50: bne #0x38eb5c
0038eb54: add sp, sp, #0x78
0038eb58: pop {r4, r5, r6, pc}
0038eb5c: bl #0x30e310
0038eb60: rsbeq r5, r0, r4, lsl #31
0038eb64: andeq r4, r0, ip, lsr #1

_ZN10GameObject17_TargetListSearchERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38fcf0 1820
0038fcf0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038fcf4: ldr r3, [r0, #4]
0038fcf8: mov r6, r2
0038fcfc: ldr r4, [pc, #0x6ec]
0038fd00: ldm r3, {r1, r2}
0038fd04: add r4, pc, r4
0038fd08: sub sp, sp, #0x6c
0038fd0c: rsb r2, r1, r2
0038fd10: asr r2, r2, #4
0038fd14: mov r5, r0
0038fd18: add r7, r2, r2, lsl #3
0038fd1c: add r7, r7, r7, lsl #6
0038fd20: add r7, r2, r7, lsl #3
0038fd24: add r7, r7, r7, lsl #15
0038fd28: add r7, r2, r7, lsl #3
0038fd2c: rsb r7, r7, #0
0038fd30: cmp r7, #0
0038fd34: bne #0x38fd40
0038fd38: add sp, sp, #0x6c
0038fd3c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038fd40: ldr r2, [r1, #4]
0038fd44: cmp r2, #3
0038fd48: bne #0x38fd38
0038fd4c: cmp r7, #1
0038fd50: bls #0x38fd94
0038fd54: mov r1, #1
0038fd58: bl #0x37baf8
0038fd5c: ldr r3, [r0, #4]
0038fd60: cmp r3, #3
0038fd64: bne #0x38fd38
0038fd68: ldr r3, [r5, #4]
0038fd6c: ldr r1, [r3, #4]
0038fd70: ldr r2, [r3]
0038fd74: rsb r2, r2, r1
0038fd78: asr r2, r2, #4
0038fd7c: add r1, r2, r2, lsl #3
0038fd80: add r1, r1, r1, lsl #6
0038fd84: add r1, r2, r1, lsl #3
0038fd88: add r1, r1, r1, lsl #15
0038fd8c: add r2, r2, r1, lsl #3
0038fd90: rsb r7, r2, #0
0038fd94: ldr r1, [r6, #0x164]
0038fd98: ldr r2, [r6, #0x168]
0038fd9c: ldr r0, [r6, #0x160]
0038fda0: str r1, [sp, #0x60]
0038fda4: str r2, [sp, #0x64]
0038fda8: str r0, [sp, #0x5c]
0038fdac: ldr r1, [r3, #4]
0038fdb0: ldr r2, [r3]
0038fdb4: rsb r2, r2, r1
0038fdb8: asr r2, r2, #4
0038fdbc: add r1, r2, r2, lsl #3
0038fdc0: add r1, r1, r1, lsl #6
0038fdc4: add r1, r2, r1, lsl #3
0038fdc8: add r1, r1, r1, lsl #15
0038fdcc: add r2, r2, r1, lsl #3
0038fdd0: rsb r2, r2, #0
0038fdd4: cmp r2, #2
0038fdd8: bhi #0x38ff0c
0038fddc: sub r7, r7, #1
0038fde0: ldr r2, [pc, #0x60c]
0038fde4: ldr ip, [pc, #0x60c]
0038fde8: ldr r0, [pc, #0x60c]
0038fdec: ldr r1, [r4, r2]
0038fdf0: ldr ip, [r4, ip]
0038fdf4: ldr r0, [r4, r0]
0038fdf8: ldr r2, [r1, #0x38]
0038fdfc: ldr sl, [r1, #0x40]
0038fe00: ldr r1, [pc, #0x5f8]
0038fe04: add r8, r2, #0x60
0038fe08: add ip, ip, #8
0038fe0c: add r0, r0, #8
0038fe10: mov lr, #0
0038fe14: str r0, [sp, #0x34]
0038fe18: str ip, [sp, #0x44]
0038fe1c: str sl, [sp, #0x48]
0038fe20: str lr, [sp, #0x4c]
0038fe24: str r8, [sp, #0x38]
0038fe28: ldr r1, [r4, r1]
0038fe2c: ldr ip, [r2, #0x60]
0038fe30: add r0, r2, #0x80
0038fe34: add r1, r1, #8
0038fe38: str ip, [sp, #0x3c]
0038fe3c: str r8, [sp, #0x40]
0038fe40: str r1, [sp, #0x20]
0038fe44: str r0, [sp, #0x24]
0038fe48: ldr r2, [r2, #0x80]
0038fe4c: str r0, [sp, #0x2c]
0038fe50: str lr, [sp, #0x30]
0038fe54: str r2, [sp, #0x28]
0038fe58: ldm r3, {r2, r3}
0038fe5c: rsb r3, r2, r3
0038fe60: asr r3, r3, #4
0038fe64: add r2, r3, r3, lsl #3
0038fe68: add r2, r2, r2, lsl #6
0038fe6c: add r2, r3, r2, lsl #3
0038fe70: add r2, r2, r2, lsl #15
0038fe74: add r3, r3, r2, lsl #3
0038fe78: rsb r3, r3, #0
0038fe7c: cmp r7, r3
0038fe80: blo #0x39001c
0038fe84: ldr r2, [r6, #0x338]
0038fe88: tst r2, #0x80
0038fe8c: addne r6, r6, #0x304
0038fe90: addne r4, sp, #0x44
0038fe94: beq #0x38fef4
0038fe98: cmp r3, #1
0038fe9c: bls #0x3900b0
0038fea0: mov r1, #0
0038fea4: mov r0, r5
0038fea8: bl #0x37baf8
0038feac: bl #0x31bbf0
0038feb0: mov r1, #1
0038feb4: mov r7, r0
0038feb8: mov r0, r5
0038febc: bl #0x37baf8
0038fec0: bl #0x31bbf0
0038fec4: movw r1, #0xfa35
0038fec8: movt r1, #0x3c8e
0038fecc: bl #0x30ed6c
0038fed0: mov r1, #0x3f000000
0038fed4: bl #0x30ed6c
0038fed8: add r1, sp, #0x5c
0038fedc: mov r3, r0
0038fee0: mov r2, r7
0038fee4: mov r0, r6
0038fee8: str r4, [sp]
0038feec: bl #0x4a33c8
0038fef0: b #0x38fd38
0038fef4: ldr r2, [r6, #0x33c]
0038fef8: add r6, r6, #0x304
0038fefc: cmp r2, #2
0038ff00: addne r4, sp, #0x20
0038ff04: addeq r4, sp, #0x34
0038ff08: b #0x38fe98
0038ff0c: mov r0, r5
0038ff10: mov r1, #2
0038ff14: bl #0x37baf8
0038ff18: ldr r3, [r0, #4]
0038ff1c: cmp r3, #7
0038ff20: beq #0x39033c
0038ff24: ldr r3, [r5, #4]
0038ff28: ldr r1, [r3, #4]
0038ff2c: ldr r2, [r3]
0038ff30: rsb r2, r2, r1
0038ff34: asr r2, r2, #4
0038ff38: add r1, r2, r2, lsl #3
0038ff3c: add r1, r1, r1, lsl #6
0038ff40: add r1, r2, r1, lsl #3
0038ff44: add r1, r1, r1, lsl #15
0038ff48: add r2, r2, r1, lsl #3
0038ff4c: rsb r2, r2, #0
0038ff50: cmp r2, #4
0038ff54: bls #0x38fddc
0038ff58: mov r1, #2
0038ff5c: mov r0, r5
0038ff60: bl #0x37baf8
0038ff64: ldr r1, [r0, #4]
0038ff68: cmp r1, #3
0038ff6c: beq #0x38ff78
0038ff70: ldr r3, [r5, #4]
0038ff74: b #0x38fddc
0038ff78: mov r0, r5
0038ff7c: bl #0x37baf8
0038ff80: ldr r3, [r0, #4]
0038ff84: cmp r3, #3
0038ff88: bne #0x38ff70
0038ff8c: mov r0, r5
0038ff90: mov r1, #4
0038ff94: bl #0x37baf8
0038ff98: ldr r0, [r0, #4]
0038ff9c: cmp r0, #3
0038ffa0: str r0, [sp, #0x1c]
0038ffa4: bne #0x38ff70
0038ffa8: ldr r2, [r5, #4]
0038ffac: movw r3, #0x6db7
0038ffb0: movt r3, #0xb6db
0038ffb4: ldm r2, {r1, r2}
0038ffb8: rsb r2, r1, r2
0038ffbc: asr r2, r2, #4
0038ffc0: mul r3, r3, r2
0038ffc4: cmp r3, #5
0038ffc8: bhi #0x3900e0
0038ffcc: mov r1, #2
0038ffd0: mov r0, r5
0038ffd4: bl #0x37baf8
0038ffd8: bl #0x31bbf0
0038ffdc: mov r1, #3
0038ffe0: mov r8, r0
0038ffe4: mov r0, r5
0038ffe8: bl #0x37baf8
0038ffec: bl #0x31bbf0
0038fff0: mov r1, #4
0038fff4: mov r7, r0
0038fff8: mov r0, r5
0038fffc: bl #0x37baf8
00390000: bl #0x31bbf0
00390004: str r7, [sp, #0x60]
00390008: str r8, [sp, #0x5c]
0039000c: str r0, [sp, #0x64]
00390010: mov r7, #6
00390014: ldr r3, [r5, #4]
00390018: b #0x38fde0
0039001c: mov r0, r5
00390020: mov r1, r7
00390024: bl #0x37baf8
00390028: ldr r3, [r0, #4]
0039002c: cmp r3, #1
00390030: beq #0x3903c4
00390034: ldr r2, [r5, #4]
00390038: ldr r3, [r2]
0039003c: ldr r2, [r2, #4]
00390040: rsb r3, r3, r2
00390044: asr r3, r3, #4
00390048: add r2, r3, r3, lsl #3
0039004c: add r2, r2, r2, lsl #6
00390050: add r2, r3, r2, lsl #3
00390054: add r2, r2, r2, lsl #15
00390058: add r3, r3, r2, lsl #3
0039005c: rsb r3, r3, #0
00390060: cmp r7, r3
00390064: bhs #0x38fe84
00390068: mov r0, r5
0039006c: mov r1, r7
00390070: bl #0x37baf8
00390074: ldr r3, [r0, #4]
00390078: cmp r3, #4
0039007c: beq #0x390370
00390080: ldr r3, [r5, #4]
00390084: ldr r2, [r3, #4]
00390088: ldr r3, [r3]
0039008c: rsb r3, r3, r2
00390090: asr r3, r3, #4
00390094: add r2, r3, r3, lsl #3
00390098: add r2, r2, r2, lsl #6
0039009c: add r2, r3, r2, lsl #3
003900a0: add r2, r2, r2, lsl #15
003900a4: add r3, r3, r2, lsl #3
003900a8: rsb r3, r3, #0
003900ac: b #0x38fe84
003900b0: mov r1, #0
003900b4: mov r0, r5
003900b8: bl #0x37baf8
003900bc: bl #0x31bbf0
003900c0: movw r3, #0xfdb
003900c4: mov r2, r0
003900c8: add r1, sp, #0x5c
003900cc: mov r0, r6
003900d0: movt r3, #0x4049
003900d4: str r4, [sp]
003900d8: bl #0x4a33c8
003900dc: b #0x38fd38
003900e0: mov r0, r5
003900e4: mov r1, #5
003900e8: bl #0x37baf8
003900ec: ldr r3, [r0, #4]
003900f0: cmp r3, #1
003900f4: bne #0x38ffcc
003900f8: mov r1, #5
003900fc: mov r0, r5
00390100: bl #0x37baf8
00390104: bl #0x31bc80
00390108: cmp r0, #0
0039010c: beq #0x38ffcc
00390110: mov r3, #0
00390114: mov r0, r6
00390118: add r1, sp, #0x50
0039011c: str r3, [sp, #0x58]
00390120: str r3, [sp, #0x50]
00390124: str r3, [sp, #0x54]
00390128: bl #0x393ae4
0039012c: ldr r3, [pc, #0x2d0]
00390130: ldr ip, [r6, #0x160]
00390134: ldr r2, [r6, #0x164]
00390138: ldr r7, [r4, r3]
0039013c: ldr r3, [r6, #0x168]
00390140: mov r1, #2
00390144: mov r0, r5
00390148: str r3, [sp, #0x64]
0039014c: ldr r3, [r7, #4]
00390150: str ip, [sp, #0x5c]
00390154: str r2, [sp, #0x60]
00390158: str r3, [sp, #0x10]
0039015c: ldr r3, [sp, #0x54]
00390160: ldr fp, [r7, #8]
00390164: ldr sl, [r7]
00390168: str r3, [sp, #0x14]
0039016c: ldr r3, [sp, #0x50]
00390170: ldr sb, [sp, #0x58]
00390174: str r3, [sp, #0x18]
00390178: bl #0x37baf8
0039017c: bl #0x31bbf0
00390180: mov r1, sb
00390184: mov r8, r0
00390188: ldr r0, [sp, #0x10]
0039018c: bl #0x30ed6c
00390190: ldr r1, [sp, #0x14]
00390194: mov r3, r0
00390198: mov r0, fp
0039019c: str r3, [sp, #0xc]
003901a0: bl #0x30ed6c
003901a4: ldr r3, [sp, #0xc]
003901a8: mov r1, r0
003901ac: mov r0, r3
003901b0: bl #0x30e3ac
003901b4: mov r1, r0
003901b8: mov r0, r8
003901bc: bl #0x30ed6c
003901c0: mov r1, r0
003901c4: ldr r0, [sp, #0x5c]
003901c8: bl #0x30eba4
003901cc: ldr r1, [sp, #0x18]
003901d0: str r0, [sp, #0x5c]
003901d4: mov r0, fp
003901d8: bl #0x30ed6c
003901dc: mov r1, sl
003901e0: mov fp, r0
003901e4: mov r0, sb
003901e8: bl #0x30ed6c
003901ec: mov r1, r0
003901f0: mov r0, fp
003901f4: bl #0x30e3ac
003901f8: mov r1, r0
003901fc: mov r0, r8
00390200: bl #0x30ed6c
00390204: mov r1, r0
00390208: ldr r0, [sp, #0x60]
0039020c: bl #0x30eba4
00390210: mov r1, sl
00390214: str r0, [sp, #0x60]
00390218: ldr r0, [sp, #0x14]
0039021c: bl #0x30ed6c
00390220: ldr r1, [sp, #0x18]
00390224: mov sl, r0
00390228: ldr r0, [sp, #0x10]
0039022c: bl #0x30ed6c
00390230: mov r1, r0
00390234: mov r0, sl
00390238: bl #0x30e3ac
0039023c: mov r1, r0
00390240: mov r0, r8
00390244: bl #0x30ed6c
00390248: mov r1, r0
0039024c: ldr r0, [sp, #0x64]
00390250: bl #0x30eba4
00390254: ldr r1, [sp, #0x1c]
00390258: str r0, [sp, #0x64]
0039025c: mov r0, r5
00390260: bl #0x37baf8
00390264: bl #0x31bbf0
00390268: ldr r1, [sp, #0x54]
0039026c: mov r8, r0
00390270: bl #0x30ed6c
00390274: ldr r1, [sp, #0x58]
00390278: mov sb, r0
0039027c: mov r0, r8
00390280: bl #0x30ed6c
00390284: ldr r1, [sp, #0x50]
00390288: mov sl, r0
0039028c: mov r0, r8
00390290: bl #0x30ed6c
00390294: mov r1, r0
00390298: ldr r0, [sp, #0x5c]
0039029c: bl #0x30eba4
003902a0: mov r1, sb
003902a4: str r0, [sp, #0x5c]
003902a8: ldr r0, [sp, #0x60]
003902ac: bl #0x30eba4
003902b0: mov r1, sl
003902b4: str r0, [sp, #0x60]
003902b8: ldr r0, [sp, #0x64]
003902bc: bl #0x30eba4
003902c0: mov r1, #4
003902c4: str r0, [sp, #0x64]
003902c8: mov r0, r5
003902cc: bl #0x37baf8
003902d0: bl #0x31bbf0
003902d4: ldr r1, [r7, #4]
003902d8: mov r8, r0
003902dc: bl #0x30ed6c
003902e0: ldr r1, [r7, #8]
003902e4: mov sb, r0
003902e8: mov r0, r8
003902ec: bl #0x30ed6c
003902f0: ldr r1, [r7]
003902f4: mov sl, r0
003902f8: mov r0, r8
003902fc: bl #0x30ed6c
00390300: mov r1, r0
00390304: ldr r0, [sp, #0x5c]
00390308: bl #0x30eba4
0039030c: mov r1, sb
00390310: str r0, [sp, #0x5c]
00390314: ldr r0, [sp, #0x60]
00390318: bl #0x30eba4
0039031c: mov r1, sl
00390320: str r0, [sp, #0x60]
00390324: ldr r0, [sp, #0x64]
00390328: bl #0x30eba4
0039032c: mov r7, #6
00390330: ldr r3, [r5, #4]
00390334: str r0, [sp, #0x64]
00390338: b #0x38fde0
0039033c: mov r1, #2
00390340: mov r0, r5
00390344: bl #0x37baf8
00390348: bl #0x31b5a0
0039034c: ldr r2, [r0, #0x160]
00390350: ldr r3, [r5, #4]
00390354: mov r7, #3
00390358: str r2, [sp, #0x5c]
0039035c: ldr r2, [r0, #0x164]
00390360: str r2, [sp, #0x60]
00390364: ldr r2, [r0, #0x168]
00390368: str r2, [sp, #0x64]
0039036c: b #0x38fde0
00390370: mov r1, r7
00390374: mov r0, r5
00390378: bl #0x37baf8
0039037c: bl #0x31c49c
00390380: add r6, r6, #0x304
00390384: mov r1, r0
00390388: mov r0, r6
0039038c: bl #0x38f72c
00390390: ldr r3, [r5, #4]
00390394: mov r4, r0
00390398: ldr r2, [r3, #4]
0039039c: ldr r3, [r3]
003903a0: rsb r3, r3, r2
003903a4: asr r3, r3, #4
003903a8: add r2, r3, r3, lsl #3
003903ac: add r2, r2, r2, lsl #6
003903b0: add r2, r3, r2, lsl #3
003903b4: add r2, r2, r2, lsl #15
003903b8: add r3, r3, r2, lsl #3
003903bc: rsb r3, r3, #0
003903c0: b #0x38fe98
003903c4: mov r1, r7
003903c8: mov r0, r5
003903cc: bl #0x37baf8
003903d0: bl #0x31bc80
003903d4: cmp r0, #0
003903d8: beq #0x390034
003903dc: ldr r1, [pc, #0x24]
003903e0: add r6, r6, #0x304
003903e4: mov r0, r6
003903e8: add r1, pc, r1
003903ec: b #0x39038c
003903f0: rsbeq r4, r0, ip, lsl #27
003903f4: strdeq r3, r4, [r0], -r4
003903f8: andeq r2, r0, r8, lsl #17
003903fc: andeq r2, r0, r0, lsr r6
00390400: andeq r3, r0, r4, ror #10
00390404: andeq r4, r0, r0, asr #6
00390408: ldrsbeq r0, [r3], #-0x48

_ZN9Character8_UseManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7864 136
003b7864: push {r4, lr}
003b7868: ldr r3, [r0, #4]
003b786c: mov r4, r1
003b7870: sub sp, sp, #8
003b7874: ldr r1, [r3, #4]
003b7878: ldr ip, [r3]
003b787c: rsb r3, ip, r1
003b7880: asr r3, r3, #4
003b7884: add r1, r3, r3, lsl #3
003b7888: add r1, r1, r1, lsl #6
003b788c: add r1, r3, r1, lsl #3
003b7890: add r1, r1, r1, lsl #15
003b7894: add r3, r3, r1, lsl #3
003b7898: cmp r3, #0
003b789c: bne #0x3b78a8
003b78a0: add sp, sp, #8
003b78a4: pop {r4, pc}
003b78a8: ldr r3, [ip, #4]
003b78ac: cmp r3, #3
003b78b0: bne #0x3b78a0
003b78b4: mov r1, #0
003b78b8: str r2, [sp, #4]
003b78bc: bl #0x37baf8
003b78c0: bl #0x31bbf0
003b78c4: bl #0x30e4cc
003b78c8: ldr r2, [sp, #4]
003b78cc: mov r1, r0
003b78d0: mov r0, r2
003b78d4: bl #0x3bdef4
003b78d8: mov r1, r0
003b78dc: mov r0, r4
003b78e0: add sp, sp, #8
003b78e4: pop {r4, lr}
003b78e8: b #0x37c7e4

_ZN3sfc6script3lua6Binder10bindMethodEPKcPFvRKNS1_9ArgumentsERNS1_12ReturnValuesEPvE 0x319af4 336
00319af4: push {r4, r5, r6, r7, lr}
00319af8: ldr r3, [r0, #8]
00319afc: ldr r5, [pc, #0x118]
00319b00: sub sp, sp, #0xc
00319b04: cmp r3, #0
00319b08: mov r4, r0
00319b0c: mov r7, r1
00319b10: add r5, pc, r5
00319b14: mov r6, r2
00319b18: beq #0x319c14
00319b1c: cmp r1, #0
00319b20: beq #0x319b6c
00319b24: cmp r6, #0
00319b28: beq #0x319bc0
00319b2c: mov r1, r7
00319b30: ldr r0, [r4, #8]
00319b34: bl #0x84c04c
00319b38: mov r1, r6
00319b3c: ldr r0, [r4, #8]
00319b40: bl #0x84b4cc
00319b44: ldr r3, [pc, #0xd4]
00319b48: ldr r0, [r4, #8]
00319b4c: mov r2, #1
00319b50: ldr r1, [r5, r3]
00319b54: bl #0x84bcdc
00319b58: ldr r0, [r4, #8]
00319b5c: mvn r1, #2
00319b60: add sp, sp, #0xc
00319b64: pop {r4, r5, r6, r7, lr}
00319b68: b #0x84c0e8
00319b6c: ldr r3, [pc, #0xb0]
00319b70: ldr r3, [r5, r3]
00319b74: ldr r3, [r3]
00319b78: cmp r3, #2
00319b7c: streq r1, [r1]
00319b80: beq #0x319b24
00319b84: cmp r3, #1
00319b88: bne #0x319b24
00319b8c: ldr r0, [pc, #0x94]
00319b90: ldr r1, [pc, #0x94]
00319b94: ldr r2, [pc, #0x94]
00319b98: ldr r0, [r5, r0]
00319b9c: ldr r3, [pc, #0x90]
00319ba0: mov ip, #0x8b
00319ba4: add r1, pc, r1
00319ba8: add r2, pc, r2
00319bac: add r3, pc, r3
00319bb0: add r0, r0, #0xa8
00319bb4: str ip, [sp]
00319bb8: bl #0x30e004
00319bbc: b #0x319b24
00319bc0: ldr r3, [pc, #0x5c]
00319bc4: ldr r3, [r5, r3]
00319bc8: ldr r3, [r3]
00319bcc: cmp r3, #2
00319bd0: streq r6, [r6]
00319bd4: beq #0x319b2c
00319bd8: cmp r3, #1
00319bdc: bne #0x319b2c
00319be0: ldr r0, [pc, #0x40]
00319be4: ldr r1, [pc, #0x4c]
00319be8: ldr r2, [pc, #0x4c]
00319bec: ldr r0, [r5, r0]
00319bf0: ldr r3, [pc, #0x48]
00319bf4: mov ip, #0x8c
00319bf8: add r1, pc, r1
00319bfc: add r2, pc, r2
00319c00: add r3, pc, r3
00319c04: add r0, r0, #0xa8
00319c08: str ip, [sp]
00319c0c: bl #0x30e004
00319c10: b #0x319b2c
00319c14: add sp, sp, #0xc
00319c18: pop {r4, r5, r6, r7, pc}
00319c1c: rsbeq sl, r7, r0, lsl #31
00319c20: andeq r1, r0, r4, ror #4
00319c24: andeq r3, r0, r0, asr #19
00319c28: andeq r1, r0, r0, asr #19
00319c2c: subseq r4, sl, r4, lsr r8
00319c30: subseq r4, sl, r0, lsr #24
00319c34: subseq r4, sl, ip, lsr #24
00319c38: subseq r4, sl, r0, ror #15
00319c3c: subseq r4, sl, r4, lsr #24
00319c40: ldrsbeq r4, [sl], #-0xb8

_ZN3sfc6script3lua5ValueC2ERKS2_ 0x31c89c 300
0031c89c: ldr r2, [pc, #0x11c]
0031c8a0: ldr ip, [pc, #0x11c]
0031c8a4: mov r3, r0
0031c8a8: add r2, pc, r2
0031c8ac: ldr ip, [r2, ip]
0031c8b0: push {r4, r5, r6, lr}
0031c8b4: add ip, ip, #8
0031c8b8: mov r4, r0
0031c8bc: str ip, [r3], #0xc
0031c8c0: mov r0, r3
0031c8c4: mov r5, r1
0031c8c8: str r3, [r4, #0x1c]
0031c8cc: str r3, [r4, #0x20]
0031c8d0: mov r1, #0x10
0031c8d4: bl #0x31167c
0031c8d8: ldr r2, [r4, #0x1c]
0031c8dc: add r3, r4, #0x24
0031c8e0: mov r6, #0
0031c8e4: strb r6, [r2]
0031c8e8: mov r0, r3
0031c8ec: str r3, [r4, #0x64]
0031c8f0: str r3, [r4, #0x68]
0031c8f4: bl #0x31bd7c
0031c8f8: ldr r3, [r4, #0x64]
0031c8fc: str r6, [r3]
0031c900: ldr r3, [r5, #4]
0031c904: cmp r3, r6
0031c908: beq #0x31c934
0031c90c: cmp r3, #1
0031c910: beq #0x31c960
0031c914: cmp r3, #3
0031c918: beq #0x31c978
0031c91c: cmp r3, #4
0031c920: beq #0x31c990
0031c924: cmp r3, #2
0031c928: beq #0x31c9a8
0031c92c: cmp r3, #7
0031c930: beq #0x31c944
0031c934: mov r0, r4
0031c938: bl #0x31b5c0
0031c93c: mov r0, r4
0031c940: pop {r4, r5, r6, pc}
0031c944: mov r0, r5
0031c948: bl #0x31b5a0
0031c94c: mov r1, r0
0031c950: mov r0, r4
0031c954: bl #0x31b608
0031c958: mov r0, r4
0031c95c: pop {r4, r5, r6, pc}
0031c960: mov r0, r5
0031c964: bl #0x31bc80
0031c968: mov r1, r0
0031c96c: mov r0, r4
0031c970: bl #0x31b5cc
0031c974: b #0x31c93c
0031c978: mov r0, r5
0031c97c: bl #0x31bbf0
0031c980: mov r1, r0
0031c984: mov r0, r4
0031c988: bl #0x31b5e8
0031c98c: b #0x31c93c
0031c990: mov r0, r5
0031c994: bl #0x31c49c
0031c998: mov r1, r0
0031c99c: mov r0, r4
0031c9a0: bl #0x31c46c
0031c9a4: b #0x31c93c
0031c9a8: mov r0, r5
0031c9ac: bl #0x31b580
0031c9b0: mov r1, r0
0031c9b4: mov r0, r4
0031c9b8: bl #0x31b5f8
0031c9bc: b #0x31c93c
0031c9c0: rsbeq r8, r7, r8, ror #3
0031c9c4: muleq r0, r8, r7

_ZN3sfc6script3lua5Value9setStringEPKc 0x31c46c 48
0031c46c: mov r3, #4
0031c470: push {r4, r5, r6, lr}
0031c474: mov r4, r0
0031c478: str r3, [r0, #4]
0031c47c: mov r0, r1
0031c480: mov r5, r1
0031c484: bl #0x30de54
0031c488: mov r1, r5
0031c48c: add r2, r5, r0
0031c490: add r0, r4, #0xc
0031c494: pop {r4, r5, r6, lr}
0031c498: b #0x3109e0

_ZN9Character9_HasAggroERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7044 32
003b7044: push {r4, lr}
003b7048: add r0, r2, #0x3c8
003b704c: mov r4, r1
003b7050: bl #0x3d49f0
003b7054: mov r1, r0
003b7058: mov r0, r4
003b705c: pop {r4, lr}
003b7060: b #0x37c7e4

_ZN9Character20_GetCurrentSpellInfoERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b6e30 80
003b6e30: push {r4, r5, r6, lr}
003b6e34: mov r0, r2
003b6e38: mov r4, r1
003b6e3c: mvn r1, #0
003b6e40: mov r5, r2
003b6e44: bl #0x3bb98c
003b6e48: mov r1, r0
003b6e4c: mov r0, r5
003b6e50: bl #0x3aeac0
003b6e54: mov r0, r5
003b6e58: mvn r1, #0
003b6e5c: bl #0x3bb98c
003b6e60: mvn r2, #0
003b6e64: mov r1, r0
003b6e68: mov r0, r5
003b6e6c: bl #0x3bbc18
003b6e70: mov r1, r0
003b6e74: mov r0, r4
003b6e78: pop {r4, r5, r6, lr}
003b6e7c: b #0x37cb24

_ZN10GameObject11_SetMaxPathERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390b3c 152
00390b3c: push {r4, r5, r6, lr}
00390b40: ldr r3, [r0, #4]
00390b44: mov r5, r2
00390b48: mov r4, r0
00390b4c: ldm r3, {r1, r2}
00390b50: rsb r3, r1, r2
00390b54: asr r3, r3, #4
00390b58: add r2, r3, r3, lsl #3
00390b5c: add r2, r2, r2, lsl #6
00390b60: add r2, r3, r2, lsl #3
00390b64: add r2, r2, r2, lsl #15
00390b68: add r3, r3, r2, lsl #3
00390b6c: cmp r3, #0
00390b70: bne #0x390b78
00390b74: pop {r4, r5, r6, pc}
00390b78: ldr r3, [r1, #4]
00390b7c: cmp r3, #3
00390b80: bne #0x390b74
00390b84: ldr r3, [r5]
00390b88: mov r0, r5
00390b8c: mov lr, pc
00390b90: ldr pc, [r3, #0x24]
00390b94: cmp r0, #0
00390b98: beq #0x390b74
00390b9c: mov r0, r5
00390ba0: bl #0x3a307c
00390ba4: cmp r0, #0
00390ba8: beq #0x390b74
00390bac: mov r1, #0
00390bb0: mov r0, r4
00390bb4: bl #0x37baf8
00390bb8: bl #0x38d798
00390bbc: cmp r0, #0x64
00390bc0: str r0, [r5, #0x26c]
00390bc4: bls #0x390b74
00390bc8: mov r3, #0x64
00390bcc: str r3, [r5, #0x26c]
00390bd0: pop {r4, r5, r6, pc}

_ZN9Character9_EndSkillERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b93cc 224
003b93cc: push {r4, r5, lr}
003b93d0: ldr r3, [r0, #4]
003b93d4: sub sp, sp, #0xc
003b93d8: mov r4, r0
003b93dc: ldr r1, [r3, #4]
003b93e0: ldr ip, [r3]
003b93e4: rsb r3, ip, r1
003b93e8: asr r3, r3, #4
003b93ec: add r1, r3, r3, lsl #3
003b93f0: add r1, r1, r1, lsl #6
003b93f4: add r1, r3, r1, lsl #3
003b93f8: add r1, r1, r1, lsl #15
003b93fc: add r3, r3, r1, lsl #3
003b9400: cmp r3, #0
003b9404: bne #0x3b9410
003b9408: add sp, sp, #0xc
003b940c: pop {r4, r5, pc}
003b9410: ldr r3, [ip, #4]
003b9414: cmp r3, #3
003b9418: bne #0x3b9408
003b941c: mov r1, #0
003b9420: str r2, [sp, #4]
003b9424: bl #0x37baf8
003b9428: bl #0x38d798
003b942c: ldr r2, [sp, #4]
003b9430: mov r5, r0
003b9434: mov r0, r2
003b9438: bl #0x3bc5fc
003b943c: ldr r3, [r0, #4]
003b9440: ldr r2, [sp, #4]
003b9444: cmp r5, r3
003b9448: bhs #0x3b9408
003b944c: ldr r5, [r4, #4]
003b9450: add r4, r2, #0x3c8
003b9454: ldm r5, {r0, r3}
003b9458: rsb r3, r0, r3
003b945c: asr r3, r3, #4
003b9460: add r2, r3, r3, lsl #3
003b9464: add r2, r2, r2, lsl #6
003b9468: add r2, r3, r2, lsl #3
003b946c: add r2, r2, r2, lsl #15
003b9470: add r3, r3, r2, lsl #3
003b9474: cmp r3, #0
003b9478: bne #0x3b948c
003b947c: ldr r0, [pc, #0x24]
003b9480: add r0, pc, r0
003b9484: bl #0x708eb0
003b9488: ldr r0, [r5]
003b948c: bl #0x31bbf0
003b9490: bl #0x8be2a0
003b9494: mov r1, r0
003b9498: mov r0, r4
003b949c: add sp, sp, #0xc
003b94a0: pop {r4, r5, lr}
003b94a4: b #0x3d8474
003b94a8: subseq r4, r0, r8, ror #31

_ZN10GameObject29_SetTargetListCharacterFilterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x390690 108
00390690: str lr, [sp, #-4]!
00390694: ldr r3, [r0, #4]
00390698: sub sp, sp, #0xc
0039069c: ldr r1, [r3, #4]
003906a0: ldr ip, [r3]
003906a4: rsb r3, ip, r1
003906a8: asr r3, r3, #4
003906ac: add r1, r3, r3, lsl #3
003906b0: add r1, r1, r1, lsl #6
003906b4: add r1, r3, r1, lsl #3
003906b8: add r1, r1, r1, lsl #15
003906bc: add r3, r3, r1, lsl #3
003906c0: cmp r3, #0
003906c4: bne #0x3906d0
003906c8: add sp, sp, #0xc
003906cc: ldm sp!, {pc}
003906d0: ldr r3, [ip, #4]
003906d4: cmp r3, #3
003906d8: bne #0x3906c8
003906dc: mov r1, #0
003906e0: str r2, [sp, #4]
003906e4: bl #0x37baf8
003906e8: bl #0x31bbf0
003906ec: bl #0x30e4cc
003906f0: ldr r2, [sp, #4]
003906f4: str r0, [r2, #0x338]
003906f8: b #0x3906c8

_ZN10GameObject12_IsCharacterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ea1c 44
0038ea1c: push {r4, lr}
0038ea20: ldr r3, [r2]
0038ea24: mov r0, r2
0038ea28: mov r4, r1
0038ea2c: mov lr, pc
0038ea30: ldr pc, [r3, #0x24]
0038ea34: mov r3, r0
0038ea38: mov r1, r3
0038ea3c: mov r0, r4
0038ea40: pop {r4, lr}
0038ea44: b #0x37c7e4

_ZN9Character11_CreateBuffERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b86a8 1328
003b86a8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b86ac: ldr r3, [r0, #4]
003b86b0: mov r4, r1
003b86b4: mov r8, r2
003b86b8: ldr r1, [r3, #4]
003b86bc: ldr r3, [r3]
003b86c0: ldr r5, [pc, #0x4f8]
003b86c4: sub sp, sp, #0x14
003b86c8: rsb r1, r3, r1
003b86cc: asr r1, r1, #4
003b86d0: add r5, pc, r5
003b86d4: add r2, r1, r1, lsl #3
003b86d8: mov r6, r0
003b86dc: add r2, r2, r2, lsl #6
003b86e0: add r2, r1, r2, lsl #3
003b86e4: add r2, r2, r2, lsl #15
003b86e8: add r1, r1, r2, lsl #3
003b86ec: cmp r1, #0
003b86f0: bne #0x3b86fc
003b86f4: add sp, sp, #0x14
003b86f8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b86fc: ldr r3, [r3, #4]
003b8700: cmp r3, #3
003b8704: bne #0x3b86f4
003b8708: mov r1, #0
003b870c: bl #0x37baf8
003b8710: bl #0x38d798
003b8714: ldr r3, [pc, #0x4a8]
003b8718: ldr r3, [r5, r3]
003b871c: ldr r3, [r3]
003b8720: cmp r0, r3
003b8724: bhs #0x3b86f4
003b8728: ldr r7, [r6, #4]
003b872c: ldm r7, {r0, r3}
003b8730: rsb r3, r0, r3
003b8734: asr r3, r3, #4
003b8738: add r2, r3, r3, lsl #3
003b873c: add r2, r2, r2, lsl #6
003b8740: add r2, r3, r2, lsl #3
003b8744: add r2, r2, r2, lsl #15
003b8748: add r3, r3, r2, lsl #3
003b874c: cmp r3, #0
003b8750: bne #0x3b8764
003b8754: ldr r0, [pc, #0x46c]
003b8758: add r0, pc, r0
003b875c: bl #0x708eb0
003b8760: ldr r0, [r7]
003b8764: bl #0x31bbf0
003b8768: bl #0x30e4cc
003b876c: ldr r3, [r6, #4]
003b8770: mov sb, r0
003b8774: ldm r3, {r2, r3}
003b8778: rsb r2, r2, r3
003b877c: asr r2, r2, #4
003b8780: add r3, r2, r2, lsl #3
003b8784: add r3, r3, r3, lsl #6
003b8788: add r3, r2, r3, lsl #3
003b878c: add r3, r3, r3, lsl #15
003b8790: add r3, r2, r3, lsl #3
003b8794: rsb r3, r3, #0
003b8798: cmp r3, #1
003b879c: bls #0x3b87e4
003b87a0: mov r0, r6
003b87a4: mov r1, #1
003b87a8: bl #0x37baf8
003b87ac: ldr r3, [r0, #4]
003b87b0: cmp r3, #0
003b87b4: bne #0x3b8964
003b87b8: ldr r3, [r6, #4]
003b87bc: ldr r2, [r3, #4]
003b87c0: ldr r3, [r3]
003b87c4: rsb r2, r3, r2
003b87c8: asr r2, r2, #4
003b87cc: add r3, r2, r2, lsl #3
003b87d0: add r3, r3, r3, lsl #6
003b87d4: add r3, r2, r3, lsl #3
003b87d8: add r3, r3, r3, lsl #15
003b87dc: add r3, r2, r3, lsl #3
003b87e0: rsb r3, r3, #0
003b87e4: mov sl, #0
003b87e8: cmp r3, #2
003b87ec: bhi #0x3b8898
003b87f0: mov r7, #1
003b87f4: cmp r3, #3
003b87f8: bhi #0x3b88e0
003b87fc: mov fp, #0
003b8800: cmp r3, #4
003b8804: bhi #0x3b8850
003b8808: mvn r5, #0
003b880c: cmp r3, #5
003b8810: bhi #0x3b8928
003b8814: ldr ip, [pc, #0x3b0]
003b8818: add ip, pc, ip
003b881c: mov r1, sb
003b8820: add r0, r8, #0x560
003b8824: mov r2, sl
003b8828: mov r3, r7
003b882c: str fp, [sp]
003b8830: stmib sp, {r5, ip}
003b8834: bl #0x3e232c
003b8838: subs r1, r0, #0
003b883c: beq #0x3b86f4
003b8840: mov r0, r4
003b8844: add sp, sp, #0x14
003b8848: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b884c: b #0x38eb00
003b8850: mov r0, r6
003b8854: mov r1, #4
003b8858: bl #0x37baf8
003b885c: ldr r3, [r0, #4]
003b8860: cmp r3, #0
003b8864: bne #0x3b8a10
003b8868: ldr r2, [r6, #4]
003b886c: ldr r1, [r2, #4]
003b8870: ldr r3, [r2]
003b8874: rsb r3, r3, r1
003b8878: asr r3, r3, #4
003b887c: add r2, r3, r3, lsl #3
003b8880: add r2, r2, r2, lsl #6
003b8884: add r2, r3, r2, lsl #3
003b8888: add r2, r2, r2, lsl #15
003b888c: add r3, r3, r2, lsl #3
003b8890: rsb r3, r3, #0
003b8894: b #0x3b8808
003b8898: mov r0, r6
003b889c: mov r1, #2
003b88a0: bl #0x37baf8
003b88a4: ldr r3, [r0, #4]
003b88a8: cmp r3, #0
003b88ac: bne #0x3b89b0
003b88b0: ldr r2, [r6, #4]
003b88b4: ldr r1, [r2, #4]
003b88b8: ldr r3, [r2]
003b88bc: rsb r3, r3, r1
003b88c0: asr r3, r3, #4
003b88c4: add r2, r3, r3, lsl #3
003b88c8: add r2, r2, r2, lsl #6
003b88cc: add r2, r3, r2, lsl #3
003b88d0: add r2, r2, r2, lsl #15
003b88d4: add r3, r3, r2, lsl #3
003b88d8: rsb r3, r3, #0
003b88dc: b #0x3b87f0
003b88e0: mov r0, r6
003b88e4: mov r1, #3
003b88e8: bl #0x37baf8
003b88ec: ldr r3, [r0, #4]
003b88f0: cmp r3, #0
003b88f4: bne #0x3b8ac8
003b88f8: ldr r2, [r6, #4]
003b88fc: ldr r1, [r2, #4]
003b8900: ldr r3, [r2]
003b8904: rsb r3, r3, r1
003b8908: asr r3, r3, #4
003b890c: add r2, r3, r3, lsl #3
003b8910: add r2, r2, r2, lsl #6
003b8914: add r2, r3, r2, lsl #3
003b8918: add r2, r2, r2, lsl #15
003b891c: add r3, r3, r2, lsl #3
003b8920: rsb r3, r3, #0
003b8924: b #0x3b87fc
003b8928: mov r0, r6
003b892c: mov r1, #5
003b8930: bl #0x37baf8
003b8934: ldr r3, [r0, #4]
003b8938: cmp r3, #0
003b893c: beq #0x3b8814
003b8940: mov r0, r6
003b8944: mov r1, #5
003b8948: bl #0x37baf8
003b894c: ldr r3, [r0, #4]
003b8950: cmp r3, #4
003b8954: beq #0x3b8b84
003b8958: ldr ip, [pc, #0x270]
003b895c: add ip, pc, ip
003b8960: b #0x3b881c
003b8964: mov r0, r6
003b8968: mov r1, #1
003b896c: bl #0x37baf8
003b8970: ldr r3, [r0, #4]
003b8974: cmp r3, #3
003b8978: beq #0x3b8b40
003b897c: ldr r3, [r6, #4]
003b8980: mov sl, #0
003b8984: ldr r2, [r3, #4]
003b8988: ldr r3, [r3]
003b898c: rsb r2, r3, r2
003b8990: asr r2, r2, #4
003b8994: add r3, r2, r2, lsl #3
003b8998: add r3, r3, r3, lsl #6
003b899c: add r3, r2, r3, lsl #3
003b89a0: add r3, r3, r3, lsl #15
003b89a4: add r3, r2, r3, lsl #3
003b89a8: rsb r3, r3, #0
003b89ac: b #0x3b87e8
003b89b0: mov r0, r6
003b89b4: mov r1, #2
003b89b8: bl #0x37baf8
003b89bc: ldr r7, [r0, #4]
003b89c0: cmp r7, #1
003b89c4: beq #0x3b8b14
003b89c8: mov r1, #2
003b89cc: mov r0, r6
003b89d0: bl #0x37baf8
003b89d4: bl #0x31bbf0
003b89d8: bl #0x30e4cc
003b89dc: ldr r2, [r6, #4]
003b89e0: mov r7, r0
003b89e4: ldr r1, [r2, #4]
003b89e8: ldr r3, [r2]
003b89ec: rsb r3, r3, r1
003b89f0: asr r3, r3, #4
003b89f4: add r2, r3, r3, lsl #3
003b89f8: add r2, r2, r2, lsl #6
003b89fc: add r2, r3, r2, lsl #3
003b8a00: add r2, r2, r2, lsl #15
003b8a04: add r3, r3, r2, lsl #3
003b8a08: rsb r3, r3, #0
003b8a0c: b #0x3b87f4
003b8a10: mov r0, r6
003b8a14: mov r1, #4
003b8a18: bl #0x37baf8
003b8a1c: ldr r3, [r0, #4]
003b8a20: cmp r3, #3
003b8a24: beq #0x3b8a80
003b8a28: mov r1, #4
003b8a2c: mov r0, r6
003b8a30: bl #0x37baf8
003b8a34: bl #0x38d798
003b8a38: ldr r3, [pc, #0x194]
003b8a3c: ldr r3, [r5, r3]
003b8a40: ldr r3, [r3]
003b8a44: cmp r0, r3
003b8a48: blo #0x3b8a80
003b8a4c: ldr r3, [r6, #4]
003b8a50: mvn r5, #0
003b8a54: ldr r2, [r3, #4]
003b8a58: ldr r3, [r3]
003b8a5c: rsb r3, r3, r2
003b8a60: asr r3, r3, #4
003b8a64: add r2, r3, r3, lsl #3
003b8a68: add r2, r2, r2, lsl #6
003b8a6c: add r2, r3, r2, lsl #3
003b8a70: add r2, r2, r2, lsl #15
003b8a74: add r3, r3, r2, lsl #3
003b8a78: rsb r3, r3, #0
003b8a7c: b #0x3b880c
003b8a80: mov r1, #4
003b8a84: mov r0, r6
003b8a88: bl #0x37baf8
003b8a8c: bl #0x31bbf0
003b8a90: bl #0x30e4cc
003b8a94: ldr r3, [r6, #4]
003b8a98: mov r5, r0
003b8a9c: ldr r2, [r3, #4]
003b8aa0: ldr r3, [r3]
003b8aa4: rsb r2, r3, r2
003b8aa8: asr r2, r2, #4
003b8aac: add r3, r2, r2, lsl #3
003b8ab0: add r3, r3, r3, lsl #6
003b8ab4: add r3, r2, r3, lsl #3
003b8ab8: add r3, r3, r3, lsl #15
003b8abc: add r3, r2, r3, lsl #3
003b8ac0: rsb r3, r3, #0
003b8ac4: b #0x3b880c
003b8ac8: mov r1, #3
003b8acc: mov r0, r6
003b8ad0: bl #0x37baf8
003b8ad4: ldr r1, [r0, #4]
003b8ad8: cmp r1, #3
003b8adc: beq #0x3b8b9c
003b8ae0: ldr r2, [r6, #4]
003b8ae4: mov fp, #0
003b8ae8: ldr r1, [r2, #4]
003b8aec: ldr r3, [r2]
003b8af0: rsb r3, r3, r1
003b8af4: asr r3, r3, #4
003b8af8: add r2, r3, r3, lsl #3
003b8afc: add r2, r2, r2, lsl #6
003b8b00: add r2, r3, r2, lsl #3
003b8b04: add r2, r2, r2, lsl #15
003b8b08: add r3, r3, r2, lsl #3
003b8b0c: rsb r3, r3, #0
003b8b10: b #0x3b8800
003b8b14: mov r1, #2
003b8b18: mov r0, r6
003b8b1c: bl #0x37baf8
003b8b20: bl #0x31bc80
003b8b24: ldr r3, [r6, #4]
003b8b28: cmp r0, #0
003b8b2c: movne r7, #0
003b8b30: ldr r2, [r3, #4]
003b8b34: ldr r3, [r3]
003b8b38: rsb r3, r3, r2
003b8b3c: b #0x3b89f0
003b8b40: mov r1, #1
003b8b44: mov r0, r6
003b8b48: bl #0x37baf8
003b8b4c: bl #0x38d798
003b8b50: ldr r3, [r6, #4]
003b8b54: mov sl, r0
003b8b58: ldr r2, [r3, #4]
003b8b5c: ldr r3, [r3]
003b8b60: rsb r3, r3, r2
003b8b64: asr r3, r3, #4
003b8b68: add r2, r3, r3, lsl #3
003b8b6c: add r2, r2, r2, lsl #6
003b8b70: add r2, r3, r2, lsl #3
003b8b74: add r2, r2, r2, lsl #15
003b8b78: add r3, r3, r2, lsl #3
003b8b7c: rsb r3, r3, #0
003b8b80: b #0x3b87e8
003b8b84: mov r1, #5
003b8b88: mov r0, r6
003b8b8c: bl #0x37baf8
003b8b90: bl #0x31c49c
003b8b94: mov ip, r0
003b8b98: b #0x3b881c
003b8b9c: mov r0, r6
003b8ba0: bl #0x37baf8
003b8ba4: bl #0x38d798
003b8ba8: ldr r3, [r6, #4]
003b8bac: mov fp, r0
003b8bb0: ldr r2, [r3, #4]
003b8bb4: ldr r3, [r3]
003b8bb8: rsb r3, r3, r2
003b8bbc: b #0x3b8af4
003b8bc0: subseq ip, sp, r0, asr #7
003b8bc4: andeq r3, r0, r8, ror #10
003b8bc8: subseq r5, r0, r0, lsl sp
003b8bcc: ldrsheq r2, [r1], #-0xf0
003b8bd0: subseq r2, r1, ip, lsr #29
003b8bd4: andeq r0, r0, r4, asr #13

_ZN3sfc6script3lua9ArgumentsC1Ev 0x3192b4 56
003192b4: ldr r3, [pc, #0x28]
003192b8: ldr r2, [pc, #0x28]
003192bc: push {r4, lr}
003192c0: add r3, pc, r3
003192c4: ldr r2, [r3, r2]
003192c8: mov r4, r0
003192cc: add r2, r2, #8
003192d0: str r2, [r0]
003192d4: bl #0x31ce84
003192d8: str r0, [r4, #4]
003192dc: mov r0, r4
003192e0: pop {r4, pc}

_ZN6TestUD14createBindingsERN3sfc6script3lua6BinderE 0x386d00 164
00386d00: push {r4, r5, r6, lr}
00386d04: mov r6, r0
00386d08: ldr r0, [pc, #0x78]
00386d0c: sub sp, sp, #8
00386d10: mov r5, r1
00386d14: add r0, pc, r0
00386d18: mov r1, r6
00386d1c: ldr r4, [pc, #0x68]
00386d20: bl #0x30de84
00386d24: ldr r3, [pc, #0x64]
00386d28: ldr r1, [pc, #0x64]
00386d2c: add r4, pc, r4
00386d30: ldr r2, [r4, r3]
00386d34: mov r0, r5
00386d38: mov r3, r6
00386d3c: add r1, pc, r1
00386d40: bl #0x31a4d4
00386d44: ldr r3, [pc, #0x4c]
00386d48: mov r1, #0
00386d4c: mov r2, r1
00386d50: ldr r3, [r4, r3]
00386d54: mov r0, r5
00386d58: str r1, [sp, #4]
00386d5c: mov r1, r3
00386d60: str r3, [sp]
00386d64: bl #0x386c20
00386d68: ldr r3, [pc, #0x2c]
00386d6c: ldr r1, [pc, #0x2c]
00386d70: mov r0, r5
00386d74: ldr r2, [r4, r3]
00386d78: add r1, pc, r1
00386d7c: add sp, sp, #8
00386d80: pop {r4, r5, r6, lr}
00386d84: b #0x319af4
00386d88: subseq fp, r3, ip, asr r3
00386d8c: rsbeq sp, r0, r4, ror #26
00386d90: andeq r4, r0, ip, lsl r5
00386d94: subseq fp, r3, ip, asr #6
00386d98: andeq r1, r0, ip, lsl #12
00386d9c: andeq r0, r0, r0, lsr #12
00386da0: subseq fp, r3, r0, lsr #6

_ZN10GameObject8_HasPathERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38e98c 52
0038e98c: ldr r3, [r2, #0x200]!
0038e990: mov r0, r1
0038e994: cmp r3, r2
0038e998: moveq r1, #0
0038e99c: beq #0x38e9bc
0038e9a0: mov ip, #0
0038e9a4: ldr r3, [r3]
0038e9a8: add ip, ip, #1
0038e9ac: cmp r2, r3
0038e9b0: bne #0x38e9a4
0038e9b4: subs r1, ip, #0
0038e9b8: movne r1, #1
0038e9bc: b #0x37c7e4

_ZN11TriggerTrap9_GetOwnerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x39ea90 12
0039ea90: mov r0, r1
0039ea94: ldr r1, [r2, #0x3f8]
0039ea98: b #0x37c9f8

_ZN10GameObject12_DealDamagesERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3906fc 400
003906fc: push {r4, r5, r6, r7, r8, lr}
00390700: ldr r7, [r0, #4]
00390704: mov r5, r1
00390708: mov r4, r2
0039070c: ldr r3, [r7]
00390710: ldr r1, [r7, #4]
00390714: ldr r6, [pc, #0x164]
00390718: sub sp, sp, #0x40
0039071c: rsb r1, r3, r1
00390720: asr r1, r1, #4
00390724: add r6, pc, r6
00390728: add r2, r1, r1, lsl #3
0039072c: mov r8, r0
00390730: add r2, r2, r2, lsl #6
00390734: add r2, r1, r2, lsl #3
00390738: add r2, r2, r2, lsl #15
0039073c: add r1, r1, r2, lsl #3
00390740: rsb r1, r1, #0
00390744: cmp r1, #2
00390748: bhi #0x390754
0039074c: add sp, sp, #0x40
00390750: pop {r4, r5, r6, r7, r8, pc}
00390754: cmp r1, #0
00390758: bne #0x39076c
0039075c: ldr r0, [pc, #0x120]
00390760: add r0, pc, r0
00390764: bl #0x708eb0
00390768: ldr r3, [r7]
0039076c: ldr r3, [r3, #4]
00390770: cmp r3, #7
00390774: bne #0x39074c
00390778: mov r0, r8
0039077c: mov r1, #1
00390780: bl #0x37baf8
00390784: ldr r3, [r0, #4]
00390788: cmp r3, #3
0039078c: bne #0x39074c
00390790: mov r1, #1
00390794: mov r0, r8
00390798: bl #0x37baf8
0039079c: bl #0x38d798
003907a0: ldr r3, [pc, #0xe0]
003907a4: ldr r3, [r6, r3]
003907a8: ldr r3, [r3]
003907ac: cmp r0, r3
003907b0: bhs #0x39074c
003907b4: mov r1, #0
003907b8: mov r0, r8
003907bc: bl #0x37baf8
003907c0: bl #0x31b5a0
003907c4: subs r7, r0, #0
003907c8: beq #0x39074c
003907cc: add r6, sp, #0x34
003907d0: mov r0, r6
003907d4: mov r1, r7
003907d8: bl #0x33dd2c
003907dc: mov r0, r6
003907e0: bl #0x33ff54
003907e4: subs r6, r0, #0
003907e8: beq #0x390840
003907ec: mov r1, #1
003907f0: mov r0, r8
003907f4: bl #0x37baf8
003907f8: bl #0x38d798
003907fc: add r7, sp, #0xc
00390800: mov r3, r0
00390804: mov r8, #0
00390808: mov r0, r7
0039080c: mov r1, r4
00390810: mov r2, r6
00390814: str r8, [sp]
00390818: bl #0x3b061c
0039081c: mov r0, r7
00390820: mov r1, r4
00390824: mov r2, r6
00390828: mov r3, r8
0039082c: bl #0x3b01b8
00390830: mov r0, r5
00390834: ldr r1, [sp, #0xc]
00390838: bl #0x37cb24
0039083c: b #0x39074c
00390840: ldr r3, [r7]
00390844: mov r0, r7
00390848: mov r1, r4
0039084c: mov lr, pc
00390850: ldr pc, [r3, #0x90]
00390854: cmp r0, #8
00390858: bne #0x39074c
0039085c: mov r0, r7
00390860: mov r1, r4
00390864: ldr r3, [r7]
00390868: mov lr, pc
0039086c: ldr pc, [r3, #0x98]
00390870: mov r0, r5
00390874: mov r1, r6
00390878: bl #0x37c7e4
0039087c: b #0x39074c
00390880: rsbeq r4, r0, ip, ror #6
00390884: subseq sp, r2, r8, lsl #26
00390888: andeq r2, r0, ip, lsl #30

_ZNSt6vectorIN3sfc6script3lua5ValueESaIS3_EE8_M_clearEv 0x31bde0 140
0031bde0: push {r4, r5, r6, lr}
0031bde4: ldr r4, [r0, #4]
0031bde8: ldr r5, [r0]
0031bdec: mov r6, r0
0031bdf0: cmp r4, r5
0031bdf4: beq #0x31be14
0031bdf8: ldr r3, [r4, #-0x70]!
0031bdfc: mov r0, r4
0031be00: mov lr, pc
0031be04: ldr pc, [r3]
0031be08: cmp r5, r4
0031be0c: bne #0x31bdf8
0031be10: ldr r4, [r6]
0031be14: cmp r4, #0
0031be18: ldr r3, [r6, #8]
0031be1c: beq #0x31be68
0031be20: rsb r3, r4, r3
0031be24: asr r3, r3, #4
0031be28: mov r1, #0x70
0031be2c: add r2, r3, r3, lsl #3
0031be30: add r2, r2, r2, lsl #6
0031be34: add r2, r3, r2, lsl #3
0031be38: add r2, r2, r2, lsl #15
0031be3c: add r3, r3, r2, lsl #3
0031be40: rsb r3, r3, #0
0031be44: mul r1, r1, r3
0031be48: cmp r1, #0x80
0031be4c: bhi #0x31be5c
0031be50: mov r0, r4
0031be54: pop {r4, r5, r6, lr}
0031be58: b #0x708f00
0031be5c: mov r0, r4
0031be60: pop {r4, r5, r6, lr}
0031be64: b #0x310440
0031be68: pop {r4, r5, r6, pc}

_ZN9LuaScript10_FromFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x37ee84 184
0037ee84: push {r4, r5, r6, lr}
0037ee88: ldr r3, [r0, #4]
0037ee8c: mov r4, r0
0037ee90: mov r5, r1
0037ee94: ldm r3, {r0, r2}
0037ee98: rsb r3, r0, r2
0037ee9c: asr r3, r3, #4
0037eea0: add r2, r3, r3, lsl #3
0037eea4: add r2, r2, r2, lsl #6
0037eea8: add r2, r3, r2, lsl #3
0037eeac: add r2, r2, r2, lsl #15
0037eeb0: add r3, r3, r2, lsl #3
0037eeb4: cmp r3, #0
0037eeb8: bne #0x37eec0
0037eebc: pop {r4, r5, r6, pc}
0037eec0: bl #0x31bbf0
0037eec4: bl #0x30e4cc
0037eec8: asr r1, r0, #8
0037eecc: mov r0, r5
0037eed0: bl #0x37cb24
0037eed4: ldr r4, [r4, #4]
0037eed8: ldm r4, {r0, r3}
0037eedc: rsb r3, r0, r3
0037eee0: asr r3, r3, #4
0037eee4: add r2, r3, r3, lsl #3
0037eee8: add r2, r2, r2, lsl #6
0037eeec: add r2, r3, r2, lsl #3
0037eef0: add r2, r2, r2, lsl #15
0037eef4: add r3, r3, r2, lsl #3
0037eef8: cmp r3, #0
0037eefc: beq #0x37ef24
0037ef00: bl #0x31bbf0
0037ef04: bl #0x30e4cc
0037ef08: bl #0x30e964
0037ef0c: mov r1, #0x3b800000
0037ef10: bl #0x30ed6c
0037ef14: mov r1, r0
0037ef18: mov r0, r5
0037ef1c: pop {r4, r5, r6, lr}
0037ef20: b #0x37ccbc
0037ef24: ldr r0, [pc, #0xc]
0037ef28: add r0, pc, r0
0037ef2c: bl #0x708eb0
0037ef30: ldr r0, [r4]
0037ef34: b #0x37ef00
0037ef38: subseq pc, r3, r0, asr #10

_ZN3sfc6script3lua8Instance14includePackageEv 0x31aa54 4
0031aa54: bx lr

_ZN3sfc6script3lua12ReturnValuesD2Ev 0x31b3f4 64
0031b3f4: ldr r3, [pc, #0x30]
0031b3f8: ldr r2, [pc, #0x30]
0031b3fc: push {r4, lr}
0031b400: add r3, pc, r3
0031b404: ldr r2, [r3, r2]
0031b408: mov r4, r0
0031b40c: ldr r0, [r0, #0x24]
0031b410: add r2, r2, #8
0031b414: str r2, [r4]
0031b418: bl #0x31d194
0031b41c: add r0, r4, #4
0031b420: bl #0x31a68c
0031b424: mov r0, r4
0031b428: pop {r4, pc}
0031b42c: mlseq r7, r0, r6, sb
0031b430: andeq r1, r0, r4, lsr #1

_ZN9Character7_MoveToERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3bada8 1216
003bada8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003badac: ldr r6, [r0, #4]
003badb0: mov r5, r2
003badb4: ldr r7, [pc, #0x4a0]
003badb8: ldm r6, {r1, r3}
003badbc: add r7, pc, r7
003badc0: sub sp, sp, #0x2c
003badc4: rsb r3, r1, r3
003badc8: asr r3, r3, #4
003badcc: mov r4, r0
003badd0: add r2, r3, r3, lsl #3
003badd4: add r2, r2, r2, lsl #6
003badd8: add r2, r3, r2, lsl #3
003baddc: add r2, r2, r2, lsl #15
003bade0: add r3, r3, r2, lsl #3
003bade4: rsb r3, r3, #0
003bade8: cmp r3, #1
003badec: beq #0x3baef0
003badf0: cmp r3, #2
003badf4: bls #0x3baee8
003badf8: cmp r3, #0
003badfc: bne #0x3bae10
003bae00: ldr r0, [pc, #0x458]
003bae04: add r0, pc, r0
003bae08: bl #0x708eb0
003bae0c: ldr r1, [r6]
003bae10: ldr r3, [r1, #4]
003bae14: cmp r3, #3
003bae18: bne #0x3baf54
003bae1c: ldr r2, [r4, #4]
003bae20: ldr r3, [r2]
003bae24: ldr r1, [r2, #4]
003bae28: rsb r1, r3, r1
003bae2c: asr r1, r1, #4
003bae30: add r3, r1, r1, lsl #3
003bae34: add r3, r3, r3, lsl #6
003bae38: add r3, r1, r3, lsl #3
003bae3c: add r3, r3, r3, lsl #15
003bae40: add r3, r1, r3, lsl #3
003bae44: rsb r3, r3, #0
003bae48: cmp r3, #1
003bae4c: beq #0x3bb238
003bae50: cmp r3, #2
003bae54: bls #0x3baee8
003bae58: mov r8, #0
003bae5c: str r8, [sp, #0x1c]
003bae60: str r8, [sp, #0x20]
003bae64: str r8, [sp, #0x24]
003bae68: ldr r3, [r2]
003bae6c: ldr r2, [r2, #4]
003bae70: rsb r3, r3, r2
003bae74: asr r3, r3, #4
003bae78: add r2, r3, r3, lsl #3
003bae7c: add r2, r2, r2, lsl #6
003bae80: add r2, r3, r2, lsl #3
003bae84: add r2, r2, r2, lsl #15
003bae88: add r3, r3, r2, lsl #3
003bae8c: rsb r3, r3, #0
003bae90: cmp r3, #3
003bae94: bhi #0x3bafe8
003bae98: mov r1, #0
003bae9c: mov r0, r4
003baea0: bl #0x37baf8
003baea4: bl #0x31bbf0
003baea8: mov r1, #1
003baeac: mov r7, r0
003baeb0: mov r0, r4
003baeb4: bl #0x37baf8
003baeb8: bl #0x31bbf0
003baebc: mov r1, #2
003baec0: mov r6, r0
003baec4: mov r0, r4
003baec8: bl #0x37baf8
003baecc: bl #0x31bbf0
003baed0: str r7, [sp, #0x1c]
003baed4: str r6, [sp, #0x20]
003baed8: str r0, [sp, #0x24]
003baedc: ldr r0, [r5, #0x378]
003baee0: add r1, sp, #0x1c
003baee4: bl #0x4054e4
003baee8: add sp, sp, #0x2c
003baeec: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003baef0: mov r1, #0
003baef4: bl #0x37baf8
003baef8: ldr r3, [r0, #4]
003baefc: cmp r3, #2
003baf00: beq #0x3bafb4
003baf04: mov r0, r4
003baf08: mov r1, #0
003baf0c: bl #0x37baf8
003baf10: ldr r3, [r0, #4]
003baf14: cmp r3, #7
003baf18: bne #0x3baee8
003baf1c: ldr r2, [r4, #4]
003baf20: ldm r2, {r1, r3}
003baf24: mov r6, r2
003baf28: rsb r3, r1, r3
003baf2c: asr r3, r3, #4
003baf30: add r0, r3, r3, lsl #3
003baf34: add r0, r0, r0, lsl #6
003baf38: add r0, r3, r0, lsl #3
003baf3c: add r0, r0, r0, lsl #15
003baf40: add r3, r3, r0, lsl #3
003baf44: rsb r3, r3, #0
003baf48: cmp r3, #2
003baf4c: bls #0x3bae48
003baf50: b #0x3badf8
003baf54: mov r0, r4
003baf58: mov r1, #1
003baf5c: bl #0x37baf8
003baf60: ldr r3, [r0, #4]
003baf64: cmp r3, #3
003baf68: beq #0x3baf84
003baf6c: mov r0, r4
003baf70: mov r1, #2
003baf74: bl #0x37baf8
003baf78: ldr r3, [r0, #4]
003baf7c: cmp r3, #3
003baf80: bne #0x3baee8
003baf84: ldr r2, [r4, #4]
003baf88: ldr r1, [r2, #4]
003baf8c: ldr r3, [r2]
003baf90: rsb r3, r3, r1
003baf94: asr r3, r3, #4
003baf98: add r1, r3, r3, lsl #3
003baf9c: add r1, r1, r1, lsl #6
003bafa0: add r1, r3, r1, lsl #3
003bafa4: add r1, r1, r1, lsl #15
003bafa8: add r3, r3, r1, lsl #3
003bafac: rsb r3, r3, #0
003bafb0: b #0x3bae48
003bafb4: ldr r2, [r4, #4]
003bafb8: ldr r1, [r2]
003bafbc: ldr r0, [r2, #4]
003bafc0: mov r6, r2
003bafc4: rsb r0, r1, r0
003bafc8: asr r0, r0, #4
003bafcc: add r3, r0, r0, lsl #3
003bafd0: add r3, r3, r3, lsl #6
003bafd4: add r3, r0, r3, lsl #3
003bafd8: add r3, r3, r3, lsl #15
003bafdc: add r3, r0, r3, lsl #3
003bafe0: rsb r3, r3, #0
003bafe4: b #0x3baf48
003bafe8: mov r0, r4
003bafec: mov r1, #3
003baff0: bl #0x37baf8
003baff4: ldr r6, [r0, #4]
003baff8: cmp r6, #1
003baffc: bne #0x3bae98
003bb000: mov r1, #3
003bb004: mov r0, r4
003bb008: bl #0x37baf8
003bb00c: bl #0x31bc80
003bb010: cmp r0, #0
003bb014: beq #0x3bae98
003bb018: mov r0, r5
003bb01c: add r1, sp, #0x10
003bb020: str r8, [sp, #0x18]
003bb024: str r8, [sp, #0x10]
003bb028: str r8, [sp, #0x14]
003bb02c: bl #0x393ae4
003bb030: ldr r3, [pc, #0x22c]
003bb034: ldr ip, [r5, #0x160]
003bb038: ldr r2, [r5, #0x164]
003bb03c: ldr r7, [r7, r3]
003bb040: ldr r3, [r5, #0x168]
003bb044: mov r1, #0
003bb048: mov r0, r4
003bb04c: str r3, [sp, #0x24]
003bb050: ldr r3, [r7, #4]
003bb054: str ip, [sp, #0x1c]
003bb058: str r2, [sp, #0x20]
003bb05c: str r3, [sp, #4]
003bb060: ldr r3, [sp, #0x14]
003bb064: ldr fp, [r7, #8]
003bb068: ldr sl, [r7]
003bb06c: str r3, [sp, #8]
003bb070: ldr r3, [sp, #0x10]
003bb074: ldr sb, [sp, #0x18]
003bb078: str r3, [sp, #0xc]
003bb07c: bl #0x37baf8
003bb080: bl #0x31bbf0
003bb084: mov r1, sb
003bb088: mov r8, r0
003bb08c: ldr r0, [sp, #4]
003bb090: bl #0x30ed6c
003bb094: ldr r1, [sp, #8]
003bb098: mov r3, r0
003bb09c: mov r0, fp
003bb0a0: str r3, [sp]
003bb0a4: bl #0x30ed6c
003bb0a8: ldr r3, [sp]
003bb0ac: mov r1, r0
003bb0b0: mov r0, r3
003bb0b4: bl #0x30e3ac
003bb0b8: mov r1, r0
003bb0bc: mov r0, r8
003bb0c0: bl #0x30ed6c
003bb0c4: mov r1, r0
003bb0c8: ldr r0, [sp, #0x1c]
003bb0cc: bl #0x30eba4
003bb0d0: ldr r1, [sp, #0xc]
003bb0d4: str r0, [sp, #0x1c]
003bb0d8: mov r0, fp
003bb0dc: bl #0x30ed6c
003bb0e0: mov r1, sl
003bb0e4: mov fp, r0
003bb0e8: mov r0, sb
003bb0ec: bl #0x30ed6c
003bb0f0: mov r1, r0
003bb0f4: mov r0, fp
003bb0f8: bl #0x30e3ac
003bb0fc: mov r1, r0
003bb100: mov r0, r8
003bb104: bl #0x30ed6c
003bb108: mov r1, r0
003bb10c: ldr r0, [sp, #0x20]
003bb110: bl #0x30eba4
003bb114: mov r1, sl
003bb118: str r0, [sp, #0x20]
003bb11c: ldr r0, [sp, #8]
003bb120: bl #0x30ed6c
003bb124: ldr r1, [sp, #0xc]
003bb128: mov sl, r0
003bb12c: ldr r0, [sp, #4]
003bb130: bl #0x30ed6c
003bb134: mov r1, r0
003bb138: mov r0, sl
003bb13c: bl #0x30e3ac
003bb140: mov r1, r0
003bb144: mov r0, r8
003bb148: bl #0x30ed6c
003bb14c: mov r1, r0
003bb150: ldr r0, [sp, #0x24]
003bb154: bl #0x30eba4
003bb158: mov r1, r6
003bb15c: str r0, [sp, #0x24]
003bb160: mov r0, r4
003bb164: bl #0x37baf8
003bb168: bl #0x31bbf0
003bb16c: ldr r1, [sp, #0x14]
003bb170: mov r6, r0
003bb174: bl #0x30ed6c
003bb178: ldr r1, [sp, #0x18]
003bb17c: mov sl, r0
003bb180: mov r0, r6
003bb184: bl #0x30ed6c
003bb188: ldr r1, [sp, #0x10]
003bb18c: mov r8, r0
003bb190: mov r0, r6
003bb194: bl #0x30ed6c
003bb198: mov r1, r0
003bb19c: ldr r0, [sp, #0x1c]
003bb1a0: bl #0x30eba4
003bb1a4: mov r1, sl
003bb1a8: str r0, [sp, #0x1c]
003bb1ac: ldr r0, [sp, #0x20]
003bb1b0: bl #0x30eba4
003bb1b4: mov r1, r8
003bb1b8: str r0, [sp, #0x20]
003bb1bc: ldr r0, [sp, #0x24]
003bb1c0: bl #0x30eba4
003bb1c4: mov r1, #2
003bb1c8: str r0, [sp, #0x24]
003bb1cc: mov r0, r4
003bb1d0: bl #0x37baf8
003bb1d4: bl #0x31bbf0
003bb1d8: ldr r1, [r7, #4]
003bb1dc: mov r4, r0
003bb1e0: bl #0x30ed6c
003bb1e4: ldr r1, [r7, #8]
003bb1e8: mov r8, r0
003bb1ec: mov r0, r4
003bb1f0: bl #0x30ed6c
003bb1f4: ldr r1, [r7]
003bb1f8: mov r6, r0
003bb1fc: mov r0, r4
003bb200: bl #0x30ed6c
003bb204: mov r1, r0
003bb208: ldr r0, [sp, #0x1c]
003bb20c: bl #0x30eba4
003bb210: mov r1, r8
003bb214: str r0, [sp, #0x1c]
003bb218: ldr r0, [sp, #0x20]
003bb21c: bl #0x30eba4
003bb220: mov r1, r6
003bb224: str r0, [sp, #0x20]
003bb228: ldr r0, [sp, #0x24]
003bb22c: bl #0x30eba4
003bb230: str r0, [sp, #0x24]
003bb234: b #0x3baedc
003bb238: mov r1, #0
003bb23c: mov r0, r4
003bb240: ldr r4, [r5, #0x378]
003bb244: bl #0x37baf8
003bb248: bl #0x31b5a0
003bb24c: mov r1, r0
003bb250: mov r0, r4
003bb254: bl #0x405540
003bb258: b #0x3baee8
003bb25c: ldrsbeq sb, [sp], #-0xc4
003bb260: subseq r3, r0, r4, ror #12
003bb264: andeq r4, r0, r0, asr #6

_ZN9Character8_GetPropERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b9d8c 440
003b9d8c: push {r4, r5, r6, r7, r8, sb, sl, lr}
003b9d90: ldr r3, [r0, #4]
003b9d94: mov r7, r1
003b9d98: mov r6, r2
003b9d9c: ldr r1, [r3, #4]
003b9da0: ldr r3, [r3]
003b9da4: ldr r4, [pc, #0x190]
003b9da8: mov r5, r0
003b9dac: rsb r1, r3, r1
003b9db0: asr r1, r1, #4
003b9db4: add r4, pc, r4
003b9db8: add r2, r1, r1, lsl #3
003b9dbc: add r2, r2, r2, lsl #6
003b9dc0: add r2, r1, r2, lsl #3
003b9dc4: add r2, r2, r2, lsl #15
003b9dc8: add r1, r1, r2, lsl #3
003b9dcc: cmp r1, #0
003b9dd0: bne #0x3b9dd8
003b9dd4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003b9dd8: ldr r3, [r3, #4]
003b9ddc: cmp r3, #3
003b9de0: bne #0x3b9dd4
003b9de4: mov r1, #0
003b9de8: bl #0x37baf8
003b9dec: bl #0x38d798
003b9df0: mov r1, #0
003b9df4: mov r0, r5
003b9df8: bl #0x37baf8
003b9dfc: bl #0x38d798
003b9e00: cmp r0, #0xdf
003b9e04: bhi #0x3b9dd4
003b9e08: ldr r3, [r5, #4]
003b9e0c: ldr r2, [r3, #4]
003b9e10: ldr r3, [r3]
003b9e14: rsb r2, r3, r2
003b9e18: asr r2, r2, #4
003b9e1c: add r1, r2, r2, lsl #3
003b9e20: add r1, r1, r1, lsl #6
003b9e24: add r1, r2, r1, lsl #3
003b9e28: add r1, r1, r1, lsl #15
003b9e2c: add r2, r2, r1, lsl #3
003b9e30: rsb r2, r2, #0
003b9e34: cmp r2, #1
003b9e38: bls #0x3b9e60
003b9e3c: ldr r3, [r3, #0x74]
003b9e40: cmp r3, #2
003b9e44: beq #0x3b9e94
003b9e48: mov r0, r5
003b9e4c: mov r1, #1
003b9e50: bl #0x37baf8
003b9e54: ldr r8, [r0, #4]
003b9e58: cmp r8, #1
003b9e5c: beq #0x3b9ef0
003b9e60: mov r1, #0
003b9e64: mov r0, r5
003b9e68: bl #0x37baf8
003b9e6c: bl #0x38d798
003b9e70: add r1, r6, #0xff0
003b9e74: mov r2, r0
003b9e78: add r1, r1, #4
003b9e7c: add r0, r6, #0x560
003b9e80: bl #0x3dedb4
003b9e84: mov r1, r0
003b9e88: mov r0, r7
003b9e8c: pop {r4, r5, r6, r7, r8, sb, sl, lr}
003b9e90: b #0x37cb24
003b9e94: mov r1, #1
003b9e98: mov r0, r5
003b9e9c: bl #0x37baf8
003b9ea0: bl #0x31b580
003b9ea4: cmp r0, #0
003b9ea8: beq #0x3b9dd4
003b9eac: mov r1, #0
003b9eb0: mov r0, r5
003b9eb4: bl #0x37baf8
003b9eb8: bl #0x38d798
003b9ebc: mov r1, #1
003b9ec0: mov r4, r0
003b9ec4: mov r0, r5
003b9ec8: bl #0x37baf8
003b9ecc: bl #0x31b580
003b9ed0: mov r1, r4
003b9ed4: mov r2, r0
003b9ed8: add r0, r6, #0x560
003b9edc: bl #0x3b55d4
003b9ee0: mov r1, r0
003b9ee4: mov r0, r7
003b9ee8: pop {r4, r5, r6, r7, r8, sb, sl, lr}
003b9eec: b #0x37cb24
003b9ef0: mov r1, #0
003b9ef4: mov r0, r5
003b9ef8: bl #0x37baf8
003b9efc: bl #0x38d798
003b9f00: mov r1, r8
003b9f04: mov sl, r0
003b9f08: mov r0, r5
003b9f0c: bl #0x37baf8
003b9f10: bl #0x31bc80
003b9f14: cmp r0, #0
003b9f18: addeq r1, r6, #0xff0
003b9f1c: add r0, r6, #0x560
003b9f20: addeq r1, r1, #4
003b9f24: moveq r2, sl
003b9f28: beq #0x3b9e80
003b9f2c: ldr r3, [pc, #0xc]
003b9f30: mov r2, sl
003b9f34: ldr r1, [r4, r3]
003b9f38: b #0x3b9e80
003b9f3c: ldrsbeq sl, [sp], #-0xcc
003b9f40: andeq r1, r0, ip, asr #32

_ZN9Character8_HasManaERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b78ec 136
003b78ec: push {r4, lr}
003b78f0: ldr r3, [r0, #4]
003b78f4: mov r4, r1
003b78f8: sub sp, sp, #8
003b78fc: ldr r1, [r3, #4]
003b7900: ldr ip, [r3]
003b7904: rsb r3, ip, r1
003b7908: asr r3, r3, #4
003b790c: add r1, r3, r3, lsl #3
003b7910: add r1, r1, r1, lsl #6
003b7914: add r1, r3, r1, lsl #3
003b7918: add r1, r1, r1, lsl #15
003b791c: add r3, r3, r1, lsl #3
003b7920: cmp r3, #0
003b7924: bne #0x3b7930
003b7928: add sp, sp, #8
003b792c: pop {r4, pc}
003b7930: ldr r3, [ip, #4]
003b7934: cmp r3, #3
003b7938: bne #0x3b7928
003b793c: mov r1, #0
003b7940: str r2, [sp, #4]
003b7944: bl #0x37baf8
003b7948: bl #0x31bbf0
003b794c: bl #0x30e4cc
003b7950: ldr r2, [sp, #4]
003b7954: mov r1, r0
003b7958: mov r0, r2
003b795c: bl #0x3bd40c
003b7960: mov r1, r0
003b7964: mov r0, r4
003b7968: add sp, sp, #8
003b796c: pop {r4, lr}
003b7970: b #0x37c7e4

_ZN9Character20_SetProjectileTargetERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3ba298 240
003ba298: push {r4, r5, r6, lr}
003ba29c: ldr r5, [r0, #4]
003ba2a0: mov r4, r0
003ba2a4: ldm r5, {r1, r3}
003ba2a8: rsb r3, r1, r3
003ba2ac: asr r3, r3, #4
003ba2b0: add r2, r3, r3, lsl #3
003ba2b4: add r2, r2, r2, lsl #6
003ba2b8: add r2, r3, r2, lsl #3
003ba2bc: add r2, r2, r2, lsl #15
003ba2c0: add r3, r3, r2, lsl #3
003ba2c4: rsb r3, r3, #0
003ba2c8: cmp r3, #1
003ba2cc: bls #0x3ba2e4
003ba2d0: cmp r3, #0
003ba2d4: beq #0x3ba2e8
003ba2d8: ldr r3, [r1, #4]
003ba2dc: cmp r3, #2
003ba2e0: beq #0x3ba304
003ba2e4: pop {r4, r5, r6, pc}
003ba2e8: ldr r0, [pc, #0x90]
003ba2ec: add r0, pc, r0
003ba2f0: bl #0x708eb0
003ba2f4: ldr r1, [r5]
003ba2f8: ldr r3, [r1, #4]
003ba2fc: cmp r3, #2
003ba300: bne #0x3ba2e4
003ba304: ldr r5, [r4, #4]
003ba308: ldm r5, {r2, r3}
003ba30c: rsb r3, r2, r3
003ba310: asr r3, r3, #4
003ba314: add r1, r3, r3, lsl #3
003ba318: add r1, r1, r1, lsl #6
003ba31c: add r1, r3, r1, lsl #3
003ba320: add r1, r1, r1, lsl #15
003ba324: add r3, r3, r1, lsl #3
003ba328: rsb r3, r3, #0
003ba32c: cmp r3, #1
003ba330: bhi #0x3ba344
003ba334: ldr r0, [pc, #0x48]
003ba338: add r0, pc, r0
003ba33c: bl #0x708eb0
003ba340: ldr r2, [r5]
003ba344: ldr r3, [r2, #0x74]
003ba348: cmp r3, #7
003ba34c: bne #0x3ba2e4
003ba350: mov r1, #0
003ba354: mov r0, r4
003ba358: bl #0x37baf8
003ba35c: bl #0x31b580
003ba360: subs r5, r0, #0
003ba364: beq #0x3ba2e4
003ba368: mov r1, #1
003ba36c: mov r0, r4
003ba370: bl #0x37baf8
003ba374: bl #0x31b5a0
003ba378: str r0, [r5, #0x384]
003ba37c: b #0x3ba2e4
003ba380: subseq r4, r0, ip, ror r1
003ba384: subseq r4, r0, r0, lsr r1

_ZN3sfc6script3luaL5panicEP9lua_State 0x31a9e8 8
0031a9e8: mov r0, #0
0031a9ec: bx lr

_ZN3sfc6script3luaL8newstateEv 0x31b224 68
0031b224: ldr r3, [pc, #0x30]
0031b228: ldr r2, [pc, #0x30]
0031b22c: push {r4, lr}
0031b230: add r3, pc, r3
0031b234: mov r1, #0
0031b238: ldr r0, [r3, r2]
0031b23c: bl #0x8579f8
0031b240: subs r4, r0, #0
0031b244: beq #0x31b254
0031b248: ldr r1, [pc, #0x14]
0031b24c: add r1, pc, r1
0031b250: bl #0x84b11c
0031b254: mov r0, r4
0031b258: pop {r4, pc}
0031b25c: rsbeq sb, r7, r0, ror #16
0031b260: andeq r2, r0, ip, ror #22

_ZNSaIPSt6vectorIN3sfc6script3lua5ValueESaIS3_EEE8allocateEjPKv.clone.4 0x31c1c4 32
0031c1c4: str lr, [sp, #-4]!
0031c1c8: sub sp, sp, #0xc
0031c1cc: add r0, sp, #8
0031c1d0: mov r3, #0x80
0031c1d4: str r3, [r0, #-4]!
0031c1d8: bl #0x708ec0
0031c1dc: add sp, sp, #0xc
0031c1e0: ldm sp!, {pc}

_ZNSt4priv11_Deque_baseIPSt6vectorIN3sfc6script3lua5ValueESaIS5_EESaIS8_EE17_M_initialize_mapEj.clone.9 0x31c1e4 116
0031c1e4: mov r3, #8
0031c1e8: push {r4, r5, r6, lr}
0031c1ec: mov r1, r3
0031c1f0: mov r4, r0
0031c1f4: str r3, [r0, #0x24]
0031c1f8: mov r2, #0
0031c1fc: add r0, r0, #0x20
0031c200: bl #0x31bd80
0031c204: mov r5, r0
0031c208: str r0, [r4, #0x20]
0031c20c: mov r0, r4
0031c210: ldr r6, [r0, #0x24]!
0031c214: bl #0x31c1c4
0031c218: sub r6, r6, #1
0031c21c: lsr r6, r6, #1
0031c220: str r0, [r5, r6, lsl #2]
0031c224: add r3, r5, r6, lsl #2
0031c228: str r3, [r4, #0xc]
0031c22c: ldr r2, [r5, r6, lsl #2]
0031c230: str r3, [r4, #0x1c]
0031c234: add r3, r2, #0x80
0031c238: stmib r4, {r2, r3}
0031c23c: ldr r3, [r5, r6, lsl #2]
0031c240: str r2, [r4]
0031c244: add r2, r3, #0x80
0031c248: str r3, [r4, #0x10]
0031c24c: str r2, [r4, #0x18]
0031c250: str r3, [r4, #0x14]
0031c254: pop {r4, r5, r6, pc}

_GLOBAL__I_.._.._sources_Core_ScriptManager_LuaManager.cpp 0x379f20 136
00379f20: push {r4, r5, r6, lr}
00379f24: ldr r4, [pc, #0x64]
00379f28: ldr r2, [pc, #0x64]
00379f2c: ldr r3, [pc, #0x64]
00379f30: add r4, pc, r4
00379f34: ldr r1, [r4, r2]
00379f38: add r3, pc, r3
00379f3c: mov r2, #0x3f000000
00379f40: ldr r0, [r1]
00379f44: str r2, [r3, #8]
00379f48: str r2, [r3]
00379f4c: tst r0, #1
00379f50: str r2, [r3, #4]
00379f54: beq #0x379f5c
00379f58: pop {r4, r5, r6, pc}
00379f5c: mov r3, #1
00379f60: str r3, [r1]
00379f64: ldr r3, [pc, #0x30]
00379f68: ldr r5, [r4, r3]
00379f6c: mov r0, r5
00379f70: bl #0x32d79c
00379f74: ldr r3, [pc, #0x24]
00379f78: mov r0, r5
00379f7c: ldr r1, [r4, r3]
00379f80: ldr r3, [pc, #0x1c]
00379f84: ldr r2, [r4, r3]
00379f88: pop {r4, r5, r6, lr}
00379f8c: b #0x30e304
00379f90: rsbeq sl, r1, r0, ror #22
00379f94: andeq r0, r0, ip, lsr #31
00379f98: rsbeq r8, r2, r8, lsr #9
00379f9c: strdeq r3, r4, [r0], -r4
00379fa0: andeq r0, r0, r0, asr #17
00379fa4: muleq r0, r0, r8

_GLOBAL__I_.._.._sources_Core_ScriptManager_LuaScript.cpp 0x37bb50 220
0037bb50: push {r4, r5, r6, lr}
0037bb54: ldr r4, [pc, #0xac]
0037bb58: ldr r2, [pc, #0xac]
0037bb5c: ldr r3, [pc, #0xac]
0037bb60: add r4, pc, r4
0037bb64: ldr r1, [r4, r2]
0037bb68: add r3, pc, r3
0037bb6c: mov r2, #0x3f000000
0037bb70: ldr r0, [r1]
0037bb74: str r2, [r3, #8]
0037bb78: str r2, [r3]
0037bb7c: tst r0, #1
0037bb80: str r2, [r3, #4]
0037bb84: beq #0x37bbd4
0037bb88: ldr r3, [pc, #0x84]
0037bb8c: ldr r3, [r4, r3]
0037bb90: ldr r2, [r3]
0037bb94: tst r2, #1
0037bb98: beq #0x37bba0
0037bb9c: pop {r4, r5, r6, pc}
0037bba0: mov r2, #1
0037bba4: str r2, [r3]
0037bba8: ldr r3, [pc, #0x68]
0037bbac: ldr r5, [r4, r3]
0037bbb0: mov r0, r5
0037bbb4: bl #0x32d79c
0037bbb8: ldr r3, [pc, #0x5c]
0037bbbc: mov r0, r5
0037bbc0: ldr r1, [r4, r3]
0037bbc4: ldr r3, [pc, #0x54]
0037bbc8: ldr r2, [r4, r3]
0037bbcc: pop {r4, r5, r6, lr}
0037bbd0: b #0x30e304
0037bbd4: mov r3, #1
0037bbd8: str r3, [r1]
0037bbdc: ldr r3, [pc, #0x40]
0037bbe0: ldr r5, [r4, r3]
0037bbe4: mov r0, r5
0037bbe8: bl #0x3790a8
0037bbec: ldr r3, [pc, #0x34]
0037bbf0: mov r0, r5
0037bbf4: ldr r1, [r4, r3]
0037bbf8: ldr r3, [pc, #0x20]
0037bbfc: ldr r2, [r4, r3]
0037bc00: bl #0x30e304
0037bc04: b #0x37bb88
0037bc08: rsbeq r8, r1, r0, lsr pc
0037bc0c: strdeq r0, r1, [r0], -r4
0037bc10: rsbeq r6, r2, r4, lsl #17
0037bc14: andeq r0, r0, ip, lsr #31
0037bc18: strdeq r3, r4, [r0], -r4
0037bc1c: andeq r0, r0, r0, asr #17
0037bc20: muleq r0, r0, r8
0037bc24: andeq r2, r0, r4, lsl r7
0037bc28: muleq r0, ip, r5

_ZN3sfc6script3lua6Binder10bindMethodI6TestUDEEvPKcMT_FvRKNS1_9ArgumentsERNS1_12ReturnValuesEE.clone.1 0x386c20 224
00386c20: push {r4, r5, r6, lr}
00386c24: mov r6, r0
00386c28: ldr r0, [r0, #8]
00386c2c: ldr r3, [pc, #0xb0]
00386c30: sub sp, sp, #0x18
00386c34: cmp r0, #0
00386c38: add r3, pc, r3
00386c3c: str r1, [sp, #8]
00386c40: str r2, [sp, #0xc]
00386c44: mov r4, r1
00386c48: mov r5, r2
00386c4c: beq #0x386cdc
00386c50: cmp r1, #0
00386c54: beq #0x386c80
00386c58: ldr r1, [pc, #0x88]
00386c5c: mov r0, r6
00386c60: mov r2, r4
00386c64: add r1, pc, r1
00386c68: mov r3, r5
00386c6c: str r5, [sp, #0x14]
00386c70: str r4, [sp, #0x10]
00386c74: add sp, sp, #0x18
00386c78: pop {r4, r5, r6, lr}
00386c7c: b #0x319c44
00386c80: tst r2, #1
00386c84: bne #0x386c58
00386c88: ldr r2, [pc, #0x5c]
00386c8c: ldr r2, [r3, r2]
00386c90: ldr r2, [r2]
00386c94: cmp r2, #2
00386c98: streq r1, [r1]
00386c9c: beq #0x386c58
00386ca0: cmp r2, #1
00386ca4: bne #0x386c58
00386ca8: ldr r0, [pc, #0x40]
00386cac: ldr r1, [pc, #0x40]
00386cb0: ldr r2, [pc, #0x40]
00386cb4: ldr r0, [r3, r0]
00386cb8: ldr r3, [pc, #0x3c]
00386cbc: mov ip, #0x87
00386cc0: add r1, pc, r1
00386cc4: add r2, pc, r2
00386cc8: add r3, pc, r3
00386ccc: add r0, r0, #0xa8
00386cd0: str ip, [sp]
00386cd4: bl #0x30e004
00386cd8: b #0x386c58
00386cdc: add sp, sp, #0x18
00386ce0: pop {r4, r5, r6, pc}
00386ce4: rsbeq sp, r0, r8, asr lr
00386ce8: ldrsheq fp, [r3], #-0x3c
00386cec: andeq r3, r0, r0, asr #19
00386cf0: andeq r1, r0, r0, asr #19
00386cf4: subseq r7, r3, r8, lsl r7
00386cf8: subseq fp, r3, ip, asr #6
00386cfc: subseq fp, r3, r0, asr r3