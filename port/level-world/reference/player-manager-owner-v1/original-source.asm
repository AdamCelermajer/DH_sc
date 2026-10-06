# 0x330794 _ZN13PlayerManager23GetLocalPlayerCharacterEv
00330794: push {r4, lr}
00330798: mov r1, #0
0033079c: mov r2, #1
003307a0: bl #0x36e478
003307a4: ldr r0, [r0, #0x660]
003307a8: pop {r4, pc}

# 0x36d280 _ZN13PlayerManager18IsPlayerInLocalMapEi
0036d280: ldr r3, [r0, #0x694]
0036d284: add r0, r0, #0x690
0036d288: cmp r3, #0
0036d28c: beq #0x36d2d0
0036d290: mov ip, r0
0036d294: b #0x36d29c
0036d298: mov r3, r2
0036d29c: ldr r2, [r3, #0x10]
0036d2a0: cmp r1, r2
0036d2a4: ldrgt r2, [r3, #0xc]
0036d2a8: ldrle r2, [r3, #8]
0036d2ac: movgt r3, ip
0036d2b0: mov ip, r3
0036d2b4: cmp r2, #0
0036d2b8: bne #0x36d298
0036d2bc: cmp r0, r3
0036d2c0: beq #0x36d2d0
0036d2c4: ldr r2, [r3, #0x10]
0036d2c8: cmp r1, r2
0036d2cc: bge #0x36d2d8
0036d2d0: mov r0, #0
0036d2d4: bx lr
0036d2d8: subs r0, r3, r0
0036d2dc: movne r0, #1
0036d2e0: bx lr

# 0x36d7a8 _ZN13PlayerManager13GetNumPlayersEv
0036d7a8: push {r4, lr}
0036d7ac: mov r4, r0
0036d7b0: bl #0x7fd794
0036d7b4: ldrb r3, [r0, #5]
0036d7b8: cmp r3, #0
0036d7bc: bne #0x36d7c8
0036d7c0: ldr r0, [r4, #0x6a0]
0036d7c4: pop {r4, pc}
0036d7c8: bl #0x320e98
0036d7cc: ldrb r3, [r0, #0x24]
0036d7d0: cmp r3, #0
0036d7d4: beq #0x36d7c0
0036d7d8: bl #0x800f8c
0036d7dc: ldr r3, [r0]
0036d7e0: mov lr, pc
0036d7e4: ldr pc, [r3, #0x64]
0036d7e8: cmp r0, #0
0036d7ec: beq #0x36d7c0
0036d7f0: bl #0x8100dc
0036d7f4: bl #0x8100e0
0036d7f8: cmp r0, #0
0036d7fc: beq #0x36d7c0
0036d800: ldr r3, [r4, #0x6a8]
0036d804: ldr r0, [r4, #0x6ac]
0036d808: rsb r0, r3, r0
0036d80c: asr r0, r0, #2
0036d810: pop {r4, pc}

# 0x36d814 _ZN13PlayerManager16ReceiveQuestSyncEP14PlayerSavegame
0036d814: push {r4, lr}
0036d818: mov r4, r0
0036d81c: mov r0, r1
0036d820: add r1, r4, #0x6e0
0036d824: bl #0x467160
0036d828: mov r3, #0
0036d82c: strb r3, [r4, #0x710]
0036d830: pop {r4, pc}

# 0x36d834 _ZN13PlayerManager20ClearQuestSyncBufferEv
0036d834: push {r4, lr}
0036d838: mov r4, r0
0036d83c: add r0, r0, #0x6e0
0036d840: bl #0x316a48
0036d844: mov r3, #0
0036d848: strb r3, [r4, #0x710]
0036d84c: pop {r4, pc}

# 0x36d850 _ZN13PlayerManager16_UnpackInventoryER10PlayerInfo
0036d850: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036d854: ldr r4, [pc, #0x188]
0036d858: ldr r3, [pc, #0x188]
0036d85c: sub sp, sp, #4
0036d860: add r4, pc, r4
0036d864: ldr r0, [r4, r3]
0036d868: mov r7, r1
0036d86c: bl #0x31f594
0036d870: cmp r0, #0
0036d874: beq #0x36d9cc
0036d878: ldrb r3, [r0, #0x144]
0036d87c: cmp r3, #0
0036d880: beq #0x36d9cc
0036d884: ldrb r3, [r7, #0x66c]
0036d888: cmp r3, #0
0036d88c: bne #0x36d9cc
0036d890: ldr r6, [r7, #0x660]
0036d894: cmp r6, #0
0036d898: beq #0x36d9cc
0036d89c: ldrb r3, [r7, #0x4e5]
0036d8a0: cmp r3, #0
0036d8a4: beq #0x36d9cc
0036d8a8: ldr r3, [r7, #0x4c0]
0036d8ac: ldr r2, [r7, #0x684]
0036d8b0: cmp r2, r3
0036d8b4: addne sb, r7, #0x3b0
0036d8b8: beq #0x36d9b4
0036d8bc: add r5, r6, #0x37c
0036d8c0: str r3, [r7, #0x684]
0036d8c4: mov r0, r5
0036d8c8: bl #0x3ffd20
0036d8cc: lsl r7, r0, #2
0036d8d0: mov r8, r0
0036d8d4: mov r1, #0
0036d8d8: mov r0, r7
0036d8dc: bl #0x31056c
0036d8e0: mov sl, r0
0036d8e4: mov r1, sl
0036d8e8: mov r2, r7
0036d8ec: mov r0, sb
0036d8f0: bl #0x36d6f4
0036d8f4: mov r0, r5
0036d8f8: mov r1, #1
0036d8fc: bl #0x3fe6bc
0036d900: cmp r8, #0
0036d904: ble #0x36d9d4
0036d908: mov r7, #0
0036d90c: ldr sb, [pc, #0xd8]
0036d910: mov r5, r7
0036d914: b #0x36d934
0036d918: ldr r3, [r6]
0036d91c: mov lr, pc
0036d920: ldr pc, [r3, #0x144]
0036d924: add r5, r5, #1
0036d928: cmp r8, r5
0036d92c: add r7, r7, #4
0036d930: beq #0x36d9d4
0036d934: ldr r3, [sl, r7]
0036d938: mov r1, r5
0036d93c: mov r0, r6
0036d940: cmp r3, #0
0036d944: blt #0x36d918
0036d948: ldr r2, [r4, sb]
0036d94c: ldr r2, [r2]
0036d950: cmp r2, #0
0036d954: beq #0x36d918
0036d958: cmp r2, r3
0036d95c: bls #0x36d918
0036d960: mov r1, #0
0036d964: mov r0, #0x6c
0036d968: bl #0x310570
0036d96c: ldr r1, [sl, r7]
0036d970: mov r2, #1
0036d974: mov fp, r0
0036d978: bl #0x3fc26c
0036d97c: mov r2, #1
0036d980: mov r1, fp
0036d984: mov r3, r2
0036d988: ldr ip, [r6]
0036d98c: mov r0, r6
0036d990: mov lr, pc
0036d994: ldr pc, [ip, #0x12c]
0036d998: ldr r3, [r6]
0036d99c: mov r2, r0
0036d9a0: mov r1, r5
0036d9a4: mov r0, r6
0036d9a8: mov lr, pc
0036d9ac: ldr pc, [r3, #0x13c]
0036d9b0: b #0x36d924
0036d9b4: add sb, r7, #0x3b0
0036d9b8: mov r0, sb
0036d9bc: bl #0x814f70
0036d9c0: cmp r0, #0
0036d9c4: ldrne r3, [r7, #0x4c0]
0036d9c8: bne #0x36d8bc
0036d9cc: add sp, sp, #4
0036d9d0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036d9d4: mov r0, sl
0036d9d8: add sp, sp, #4
0036d9dc: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036d9e0: b #0x310440
0036d9e4: rsbeq r7, r2, r0, lsr r2
0036d9e8: strdeq r3, r4, [r0], -r4
0036d9ec: andeq r0, r0, r0, ror #26

# 0x36de28 _ZN13PlayerManager19IsPlayerInRemoteMapEi
0036de28: push {r4, lr}
0036de2c: mov r4, r1
0036de30: bl #0x8100dc
0036de34: mov r1, r4
0036de38: mov r2, #0
0036de3c: bl #0x8101e0
0036de40: ldr r3, [r0]
0036de44: mov lr, pc
0036de48: ldr pc, [r3, #0x5c]
0036de4c: pop {r4, pc}

# 0x36de50 _ZN13PlayerManager15DoesPlayerExistEi
0036de50: push {r4, r5, r6, lr}
0036de54: mov r4, r0
0036de58: mov r5, r1
0036de5c: bl #0x7fd794
0036de60: ldrb r3, [r0, #5]
0036de64: cmp r3, #0
0036de68: bne #0x36de7c
0036de6c: mov r0, r4
0036de70: mov r1, r5
0036de74: pop {r4, r5, r6, lr}
0036de78: b #0x36d280
0036de7c: bl #0x320e98
0036de80: ldrb r3, [r0, #0x24]
0036de84: cmp r3, #0
0036de88: beq #0x36de6c
0036de8c: bl #0x800f8c
0036de90: ldr r3, [r0]
0036de94: mov lr, pc
0036de98: ldr pc, [r3, #0x64]
0036de9c: cmp r0, #0
0036dea0: beq #0x36de6c
0036dea4: bl #0x8100dc
0036dea8: bl #0x8100e0
0036deac: cmp r0, #0
0036deb0: beq #0x36de6c
0036deb4: mov r0, r4
0036deb8: mov r1, r5
0036debc: pop {r4, r5, r6, lr}
0036dec0: b #0x36de28

# 0x36dec4 _ZN13PlayerManager17_GetNetPlayerInfoEib
0036dec4: push {r4, r5, r6, lr}
0036dec8: sub sp, sp, #8
0036decc: mov r5, r1
0036ded0: mov r4, r2
0036ded4: bl #0x7fd794
0036ded8: ldrb r3, [r0, #5]
0036dedc: ldr r6, [pc, #0xb4]
0036dee0: cmp r3, #0
0036dee4: add r6, pc, r6
0036dee8: bne #0x36df28
0036deec: ldr r3, [pc, #0xa8]
0036def0: ldr r3, [r6, r3]
0036def4: ldr r3, [r3]
0036def8: cmp r3, #2
0036defc: moveq r3, #0
0036df00: streq r3, [r3]
0036df04: beq #0x36df10
0036df08: cmp r3, #1
0036df0c: beq #0x36df64
0036df10: bl #0x8100dc
0036df14: mov r1, r5
0036df18: mov r2, r4
0036df1c: add sp, sp, #8
0036df20: pop {r4, r5, r6, lr}
0036df24: b #0x8101e0
0036df28: bl #0x320e98
0036df2c: ldrb r3, [r0, #0x24]
0036df30: cmp r3, #0
0036df34: beq #0x36deec
0036df38: bl #0x800f8c
0036df3c: ldr r3, [r0]
0036df40: mov lr, pc
0036df44: ldr pc, [r3, #0x64]
0036df48: cmp r0, #0
0036df4c: beq #0x36deec
0036df50: bl #0x8100dc
0036df54: bl #0x8100e0
0036df58: cmp r0, #0
0036df5c: bne #0x36df10
0036df60: b #0x36deec
0036df64: ldr r0, [pc, #0x34]
0036df68: ldr r1, [pc, #0x34]
0036df6c: ldr r2, [pc, #0x34]
0036df70: ldr r0, [r6, r0]
0036df74: ldr r3, [pc, #0x30]
0036df78: movw ip, #0x776
0036df7c: add r1, pc, r1
0036df80: add r2, pc, r2
0036df84: add r3, pc, r3
0036df88: add r0, r0, #0xa8
0036df8c: str ip, [sp]
0036df90: bl #0x30e004
0036df94: b #0x36df10
0036df98: rsbeq r6, r2, ip, lsr #23
0036df9c: andeq r3, r0, r0, asr #19
0036dfa0: andeq r1, r0, r0, asr #19
0036dfa4: subseq r0, r5, ip, asr r4
0036dfa8: ldrsbeq r3, [r5], #-0x60
0036dfac: subseq r3, r5, ip, ror #14

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

# 0x36e09c _ZN13PlayerManager16GetHostingPlayerEv
0036e09c: push {r4, lr}
0036e0a0: mov r4, r0
0036e0a4: bl #0x7fd794
0036e0a8: ldrb r3, [r0, #5]
0036e0ac: cmp r3, #0
0036e0b0: bne #0x36e0c8
0036e0b4: mov r1, #0
0036e0b8: mov r0, r4
0036e0bc: mov r2, #0
0036e0c0: pop {r4, lr}
0036e0c4: b #0x36dfb0
0036e0c8: bl #0x320e98
0036e0cc: ldrb r3, [r0, #0x24]
0036e0d0: cmp r3, #0
0036e0d4: beq #0x36e0b4
0036e0d8: bl #0x800f8c
0036e0dc: ldr r3, [r0]
0036e0e0: mov lr, pc
0036e0e4: ldr pc, [r3, #0x64]
0036e0e8: cmp r0, #0
0036e0ec: beq #0x36e0b4
0036e0f0: bl #0x8100dc
0036e0f4: bl #0x8100e0
0036e0f8: cmp r0, #0
0036e0fc: beq #0x36e0b4
0036e100: bl #0x8100dc
0036e104: ldr r1, [r0, #0x170]
0036e108: b #0x36e0b8

# 0x36e10c _ZN13PlayerManager24_GetInternalIDByRemoteIDEib
0036e10c: push {r4, r5, r6, r7, r8, lr}
0036e110: mov r4, r0
0036e114: mov r5, r1
0036e118: mov r6, r2
0036e11c: bl #0x7fd794
0036e120: ldrb r3, [r0, #5]
0036e124: cmp r3, #0
0036e128: bne #0x36e140
0036e12c: ldr r3, [r4, #0x6a0]
0036e130: cmp r5, r3
0036e134: blo #0x36e200
0036e138: mvn r0, #0
0036e13c: pop {r4, r5, r6, r7, r8, pc}
0036e140: bl #0x320e98
0036e144: ldrb r3, [r0, #0x24]
0036e148: cmp r3, #0
0036e14c: beq #0x36e12c
0036e150: bl #0x800f8c
0036e154: ldr r3, [r0]
0036e158: mov lr, pc
0036e15c: ldr pc, [r3, #0x64]
0036e160: cmp r0, #0
0036e164: beq #0x36e12c
0036e168: bl #0x8100dc
0036e16c: bl #0x8100e0
0036e170: cmp r0, #0
0036e174: beq #0x36e12c
0036e178: ldr r3, [r4, #0x6a8]
0036e17c: ldr r2, [r4, #0x6ac]
0036e180: rsb r2, r3, r2
0036e184: cmp r5, r2, asr #2
0036e188: bhs #0x36e138
0036e18c: mov r2, #0
0036e190: mov r7, r2
0036e194: mov r8, r2
0036e198: b #0x36e1bc
0036e19c: cmp r8, r5
0036e1a0: beq #0x36e29c
0036e1a4: add r8, r8, #1
0036e1a8: ldr r3, [r4, #0x6a8]
0036e1ac: ldr r1, [r4, #0x6ac]
0036e1b0: rsb r1, r3, r1
0036e1b4: cmp r7, r1, asr #2
0036e1b8: bhs #0x36e1f8
0036e1bc: ldr r1, [r3, r2, lsl #2]
0036e1c0: mov r0, r4
0036e1c4: mov r2, #0
0036e1c8: bl #0x36dfb0
0036e1cc: ldrb r3, [r0, #0x66c]
0036e1d0: add r7, r7, #1
0036e1d4: mov r2, r7
0036e1d8: cmp r3, #1
0036e1dc: beq #0x36e1a8
0036e1e0: cmp r6, #0
0036e1e4: beq #0x36e19c
0036e1e8: ldr r3, [r0, #0x660]
0036e1ec: cmp r3, #0
0036e1f0: bne #0x36e19c
0036e1f4: b #0x36e1a8
0036e1f8: mvn r0, #0
0036e1fc: pop {r4, r5, r6, r7, r8, pc}
0036e200: ldr r2, [r4, #0x698]
0036e204: add ip, r4, #0x690
0036e208: mov r0, #0
0036e20c: cmp ip, r2
0036e210: beq #0x36e1f8
0036e214: ldrb r3, [r2, #0x684]
0036e218: cmp r3, #1
0036e21c: beq #0x36e234
0036e220: cmp r6, #0
0036e224: bne #0x36e258
0036e228: cmp r0, r5
0036e22c: beq #0x36e2a4
0036e230: add r0, r0, #1
0036e234: ldr r1, [r2, #0xc]
0036e238: cmp r1, #0
0036e23c: beq #0x36e268
0036e240: mov r2, r1
0036e244: ldr r3, [r2, #8]
0036e248: cmp r3, #0
0036e24c: beq #0x36e20c
0036e250: mov r2, r3
0036e254: b #0x36e244
0036e258: ldr r3, [r2, #0x678]
0036e25c: cmp r3, #0
0036e260: bne #0x36e228
0036e264: b #0x36e234
0036e268: ldr r3, [r2, #4]
0036e26c: ldr r4, [r3, #0xc]
0036e270: cmp r2, r4
0036e274: bne #0x36e290
0036e278: mov r2, r3
0036e27c: ldr r3, [r3, #4]
0036e280: ldr r1, [r3, #0xc]
0036e284: cmp r1, r2
0036e288: beq #0x36e278
0036e28c: ldr r1, [r2, #0xc]
0036e290: cmp r1, r3
0036e294: movne r2, r3
0036e298: b #0x36e20c
0036e29c: ldr r0, [r0, #0x670]
0036e2a0: pop {r4, r5, r6, r7, r8, pc}
0036e2a4: ldr r0, [r2, #0x688]
0036e2a8: pop {r4, r5, r6, r7, r8, pc}

# 0x36e2ac _ZN13PlayerManager15GetRemotePlayerEib
0036e2ac: push {r4, lr}
0036e2b0: mov r4, r0
0036e2b4: bl #0x36e10c
0036e2b8: mov r2, #0
0036e2bc: mov r1, r0
0036e2c0: mov r0, r4
0036e2c4: pop {r4, lr}
0036e2c8: b #0x36dfb0

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

# 0x36e478 _ZN13PlayerManager14GetLocalPlayerEib
0036e478: push {r4, lr}
0036e47c: mov r4, r0
0036e480: bl #0x36e2cc
0036e484: mov r2, #0
0036e488: mov r1, r0
0036e48c: mov r0, r4
0036e490: pop {r4, lr}
0036e494: b #0x36dfb0

# 0x36e498 _ZN13PlayerManager18HandleQuestSyncMsgEPKci
0036e498: push {r4, r5, r6, r7, r8, lr}
0036e49c: ldr r3, [pc, #0xf8]
0036e4a0: subs r6, r2, #0
0036e4a4: sub sp, sp, #8
0036e4a8: mov r4, r0
0036e4ac: add r3, pc, r3
0036e4b0: mov r5, r1
0036e4b4: ble #0x36e544
0036e4b8: mov r2, #1
0036e4bc: mov r0, r4
0036e4c0: mov r1, #0
0036e4c4: bl #0x36e478
0036e4c8: ldrb r3, [r4, #0x710]
0036e4cc: ldr r2, [r0, #0x660]
0036e4d0: cmp r3, #0
0036e4d4: bne #0x36e4fc
0036e4d8: cmp r2, #0
0036e4dc: beq #0x36e4fc
0036e4e0: movw r3, #0x14e8
0036e4e4: ldr r3, [r2, r3]
0036e4e8: cmp r3, #0
0036e4ec: beq #0x36e508
0036e4f0: ldrb r3, [r3, #0x14]
0036e4f4: cmp r3, #0
0036e4f8: beq #0x36e508
0036e4fc: ldrb r3, [r4, #0x71a]
0036e500: cmp r3, #0
0036e504: beq #0x36e53c
0036e508: asr r7, r6, #0x1f
0036e50c: add r8, r4, #0x6e0
0036e510: mov r0, r8
0036e514: mov r2, r6
0036e518: mov r3, r7
0036e51c: bl #0x317180
0036e520: mov r3, r7
0036e524: mov r0, r8
0036e528: mov r1, r5
0036e52c: mov r2, r6
0036e530: bl #0x316fd4
0036e534: mov r3, #1
0036e538: strb r3, [r4, #0x710]
0036e53c: add sp, sp, #8
0036e540: pop {r4, r5, r6, r7, r8, pc}
0036e544: ldr r2, [pc, #0x54]
0036e548: ldr r2, [r3, r2]
0036e54c: ldr r2, [r2]
0036e550: cmp r2, #2
0036e554: moveq r3, #0
0036e558: streq r3, [r3]
0036e55c: beq #0x36e4b8
0036e560: cmp r2, #1
0036e564: bne #0x36e4b8
0036e568: ldr r0, [pc, #0x34]
0036e56c: ldr r1, [pc, #0x34]
0036e570: ldr r2, [pc, #0x34]
0036e574: ldr r0, [r3, r0]
0036e578: ldr r3, [pc, #0x30]
0036e57c: movw ip, #0x955
0036e580: add r1, pc, r1
0036e584: add r2, pc, r2
0036e588: add r3, pc, r3
0036e58c: add r0, r0, #0xa8
0036e590: str ip, [sp]
0036e594: bl #0x30e004
0036e598: b #0x36e4b8
0036e59c: rsbeq r6, r2, r4, ror #11
0036e5a0: andeq r3, r0, r0, asr #19
0036e5a4: andeq r1, r0, r0, asr #19
0036e5a8: subseq pc, r4, r8, asr lr
0036e5ac: ldrheq r3, [r5], #-0x1c
0036e5b0: subseq r3, r5, r8, ror #2

# 0x36e5b4 _ZN13PlayerManager26_GetInternalIDByFriendlyIDEib
0036e5b4: push {r4, r5, r6, r7, r8, lr}
0036e5b8: mov r4, r0
0036e5bc: mov r5, r1
0036e5c0: mov r6, r2
0036e5c4: bl #0x7fd794
0036e5c8: ldrb r3, [r0, #5]
0036e5cc: cmp r3, #0
0036e5d0: bne #0x36e5e8
0036e5d4: ldr r3, [r4, #0x6a0]
0036e5d8: cmp r5, r3
0036e5dc: blo #0x36e6a8
0036e5e0: mvn r0, #0
0036e5e4: pop {r4, r5, r6, r7, r8, pc}
0036e5e8: bl #0x320e98
0036e5ec: ldrb r3, [r0, #0x24]
0036e5f0: cmp r3, #0
0036e5f4: beq #0x36e5d4
0036e5f8: bl #0x800f8c
0036e5fc: ldr r3, [r0]
0036e600: mov lr, pc
0036e604: ldr pc, [r3, #0x64]
0036e608: cmp r0, #0
0036e60c: beq #0x36e5d4
0036e610: bl #0x8100dc
0036e614: bl #0x8100e0
0036e618: cmp r0, #0
0036e61c: beq #0x36e5d4
0036e620: mov r0, r4
0036e624: bl #0x36d7a8
0036e628: cmp r0, r5
0036e62c: ble #0x36e5e0
0036e630: ldr r3, [r4, #0x6a8]
0036e634: ldr r2, [r4, #0x6ac]
0036e638: rsb r2, r3, r2
0036e63c: lsrs r2, r2, #2
0036e640: beq #0x36e734
0036e644: mov r7, #0
0036e648: mov r8, r7
0036e64c: b #0x36e668
0036e650: add r8, r8, #1
0036e654: ldr r3, [r4, #0x6a8]
0036e658: ldr r2, [r4, #0x6ac]
0036e65c: rsb r2, r3, r2
0036e660: cmp r7, r2, asr #2
0036e664: bhs #0x36e734
0036e668: ldr r1, [r3, r7, lsl #2]
0036e66c: mov r0, r4
0036e670: mov r2, #0
0036e674: bl #0x36dfb0
0036e678: cmp r6, #0
0036e67c: lsl r3, r7, #2
0036e680: add r7, r7, #1
0036e684: beq #0x36e694
0036e688: ldr r2, [r0, #0x660]
0036e68c: cmp r2, #0
0036e690: beq #0x36e654
0036e694: cmp r8, r5
0036e698: bne #0x36e650
0036e69c: ldr r2, [r4, #0x6a8]
0036e6a0: ldr r0, [r2, r3]
0036e6a4: pop {r4, r5, r6, r7, r8, pc}
0036e6a8: ldr r2, [r4, #0x698]
0036e6ac: add ip, r4, #0x690
0036e6b0: mov r0, #0
0036e6b4: cmp ip, r2
0036e6b8: beq #0x36e734
0036e6bc: cmp r6, #0
0036e6c0: beq #0x36e6d0
0036e6c4: ldr r3, [r2, #0x678]
0036e6c8: cmp r3, #0
0036e6cc: beq #0x36e6dc
0036e6d0: cmp r0, r5
0036e6d4: beq #0x36e73c
0036e6d8: add r0, r0, #1
0036e6dc: ldr r1, [r2, #0xc]
0036e6e0: cmp r1, #0
0036e6e4: beq #0x36e700
0036e6e8: mov r2, r1
0036e6ec: ldr r3, [r2, #8]
0036e6f0: cmp r3, #0
0036e6f4: beq #0x36e6b4
0036e6f8: mov r2, r3
0036e6fc: b #0x36e6ec
0036e700: ldr r3, [r2, #4]
0036e704: ldr r4, [r3, #0xc]
0036e708: cmp r2, r4
0036e70c: bne #0x36e728
0036e710: mov r2, r3
0036e714: ldr r3, [r3, #4]
0036e718: ldr r1, [r3, #0xc]
0036e71c: cmp r1, r2
0036e720: beq #0x36e710
0036e724: ldr r1, [r2, #0xc]
0036e728: cmp r1, r3
0036e72c: movne r2, r3
0036e730: b #0x36e6b4
0036e734: mvn r0, #0
0036e738: pop {r4, r5, r6, r7, r8, pc}
0036e73c: ldr r0, [r2, #0x688]
0036e740: pop {r4, r5, r6, r7, r8, pc}

# 0x36e744 _ZN13PlayerManager9GetPlayerEib
0036e744: push {r4, lr}
0036e748: mov r4, r0
0036e74c: bl #0x36e5b4
0036e750: mov r2, #0
0036e754: mov r1, r0
0036e758: mov r0, r4
0036e75c: pop {r4, lr}
0036e760: b #0x36dfb0

# 0x36e764 _ZN13PlayerManager14AllPlayersDeadEv
0036e764: push {r4, r5, r6, lr}
0036e768: mov r5, r0
0036e76c: mov r0, r5
0036e770: bl #0x36d7a8
0036e774: mov r4, #0
0036e778: cmp r4, r0
0036e77c: mov r1, r4
0036e780: mov r2, #0
0036e784: mov r0, r5
0036e788: bge #0x36e7d0
0036e78c: bl #0x36e744
0036e790: ldr r3, [r0, #0x660]
0036e794: subs r0, r3, #0
0036e798: beq #0x36e7f0
0036e79c: ldr r3, [r3]
0036e7a0: mov lr, pc
0036e7a4: ldr pc, [r3, #0x34]
0036e7a8: subs r2, r0, #0
0036e7ac: beq #0x36e7d8
0036e7b0: mov r0, r5
0036e7b4: bl #0x36d7a8
0036e7b8: add r4, r4, #1
0036e7bc: cmp r4, r0
0036e7c0: mov r1, r4
0036e7c4: mov r2, #0
0036e7c8: mov r0, r5
0036e7cc: blt #0x36e78c
0036e7d0: mov r0, #1
0036e7d4: pop {r4, r5, r6, pc}
0036e7d8: mov r1, r4
0036e7dc: mov r0, r5
0036e7e0: bl #0x36e744
0036e7e4: ldr r3, [r0, #0x3a8]
0036e7e8: cmn r3, #1
0036e7ec: bne #0x36e7b0
0036e7f0: mov r0, #0
0036e7f4: pop {r4, r5, r6, pc}

# 0x36e7f8 _ZN13PlayerManager14AllReadyToRollEv
0036e7f8: push {r4, r5, r6, r7, r8, lr}
0036e7fc: mov r5, r0
0036e800: bl #0x7fd794
0036e804: ldrb r3, [r0, #5]
0036e808: cmp r3, #0
0036e80c: bne #0x36e818
0036e810: mov r0, #1
0036e814: pop {r4, r5, r6, r7, r8, pc}
0036e818: bl #0x320e98
0036e81c: ldrb r3, [r0, #0x24]
0036e820: cmp r3, #0
0036e824: beq #0x36e810
0036e828: bl #0x800f8c
0036e82c: ldr r3, [r0]
0036e830: mov lr, pc
0036e834: ldr pc, [r3, #0x64]
0036e838: cmp r0, #0
0036e83c: beq #0x36e810
0036e840: bl #0x8100dc
0036e844: bl #0x8100e0
0036e848: cmp r0, #0
0036e84c: beq #0x36e810
0036e850: mov r0, r5
0036e854: bl #0x36d7a8
0036e858: subs r6, r0, #0
0036e85c: ble #0x36e810
0036e860: mov r4, #0
0036e864: b #0x36e87c
0036e868: ldrb r3, [r7, #0x545]
0036e86c: cmp r3, #0
0036e870: beq #0x36e8b4
0036e874: cmp r4, r6
0036e878: beq #0x36e810
0036e87c: mov r1, r4
0036e880: mov r2, #0
0036e884: mov r0, r5
0036e888: bl #0x36e744
0036e88c: ldr r3, [r0]
0036e890: mov r7, r0
0036e894: mov lr, pc
0036e898: ldr pc, [r3, #0x5c]
0036e89c: cmp r0, #0
0036e8a0: add r4, r4, #1
0036e8a4: beq #0x36e874
0036e8a8: ldrb r3, [r7, #0x525]
0036e8ac: cmp r3, #0
0036e8b0: bne #0x36e868
0036e8b4: mov r0, #0
0036e8b8: pop {r4, r5, r6, r7, r8, pc}

# 0x36e8bc _ZN13PlayerManager21AllClientsReadyToRollEv
0036e8bc: push {r4, r5, r6, r7, r8, lr}
0036e8c0: mov r5, r0
0036e8c4: bl #0x7fd794
0036e8c8: ldrb r3, [r0, #5]
0036e8cc: cmp r3, #0
0036e8d0: bne #0x36e8dc
0036e8d4: mov r0, #1
0036e8d8: pop {r4, r5, r6, r7, r8, pc}
0036e8dc: bl #0x320e98
0036e8e0: ldrb r3, [r0, #0x24]
0036e8e4: cmp r3, #0
0036e8e8: beq #0x36e8d4
0036e8ec: bl #0x800f8c
0036e8f0: ldr r3, [r0]
0036e8f4: mov lr, pc
0036e8f8: ldr pc, [r3, #0x64]
0036e8fc: cmp r0, #0
0036e900: beq #0x36e8d4
0036e904: bl #0x8100dc
0036e908: bl #0x8100e0
0036e90c: cmp r0, #0
0036e910: beq #0x36e8d4
0036e914: mov r0, r5
0036e918: bl #0x36d7a8
0036e91c: subs r6, r0, #0
0036e920: movgt r4, #0
0036e924: ble #0x36e8d4
0036e928: mov r1, r4
0036e92c: mov r2, #0
0036e930: mov r0, r5
0036e934: bl #0x36e744
0036e938: ldr r3, [r0]
0036e93c: mov r7, r0
0036e940: mov lr, pc
0036e944: ldr pc, [r3, #0x5c]
0036e948: cmp r0, #0
0036e94c: add r4, r4, #1
0036e950: mov r0, r7
0036e954: beq #0x36e984
0036e958: ldrb r3, [r7, #0x525]
0036e95c: cmp r3, #0
0036e960: beq #0x36e97c
0036e964: bl #0x80f23c
0036e968: cmp r0, #0
0036e96c: bne #0x36e984
0036e970: ldrb r3, [r7, #0x545]
0036e974: cmp r3, #0
0036e978: bne #0x36e984
0036e97c: mov r0, #0
0036e980: pop {r4, r5, r6, r7, r8, pc}
0036e984: cmp r4, r6
0036e988: bne #0x36e928
0036e98c: mov r0, #1
0036e990: pop {r4, r5, r6, r7, r8, pc}

# 0x36e994 _ZN13PlayerManager14AllLoadingDoneEv
0036e994: push {r4, r5, r6, r7, r8, lr}
0036e998: mov r5, r0
0036e99c: bl #0x36d7a8
0036e9a0: subs r6, r0, #0
0036e9a4: ble #0x36ea48
0036e9a8: mov r4, #0
0036e9ac: b #0x36e9b8
0036e9b0: cmp r4, r6
0036e9b4: beq #0x36ea48
0036e9b8: mov r1, r4
0036e9bc: mov r2, #0
0036e9c0: mov r0, r5
0036e9c4: bl #0x36e744
0036e9c8: ldr r3, [r0]
0036e9cc: mov r7, r0
0036e9d0: mov lr, pc
0036e9d4: ldr pc, [r3, #0x5c]
0036e9d8: cmp r0, #0
0036e9dc: add r4, r4, #1
0036e9e0: beq #0x36e9b0
0036e9e4: ldrb r3, [r7, #0x525]
0036e9e8: cmp r3, #0
0036e9ec: beq #0x36ea40
0036e9f0: ldrb r3, [r5, #0x71a]
0036e9f4: cmp r3, #0
0036e9f8: beq #0x36e9b0
0036e9fc: mov r0, r7
0036ea00: bl #0x80f164
0036ea04: cmp r0, #0
0036ea08: beq #0x36e9b0
0036ea0c: mov r0, r7
0036ea10: bl #0x80f23c
0036ea14: cmp r0, #0
0036ea18: beq #0x36e9b0
0036ea1c: ldrb r3, [r7, #0x525]
0036ea20: cmp r3, #0
0036ea24: beq #0x36e9b0
0036ea28: ldrb r3, [r7, #0x4e5]
0036ea2c: cmp r3, #0
0036ea30: beq #0x36e9b0
0036ea34: ldrb r3, [r7, #0x505]
0036ea38: cmp r3, #0
0036ea3c: beq #0x36e9b0
0036ea40: mov r0, #0
0036ea44: pop {r4, r5, r6, r7, r8, pc}
0036ea48: mov r0, #1
0036ea4c: pop {r4, r5, r6, r7, r8, pc}

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

# 0x36ead0 _ZN13PlayerManager18GetNumLocalPlayersEb
0036ead0: push {r4, r5, r6, lr}
0036ead4: mov r4, r0
0036ead8: mov r5, r1
0036eadc: bl #0x7fd794
0036eae0: ldrb r3, [r0, #5]
0036eae4: cmp r3, #0
0036eae8: bne #0x36eb98
0036eaec: ldr r2, [r4, #0x698]
0036eaf0: add r0, r4, #0x690
0036eaf4: mov r6, #0
0036eaf8: cmp r0, r2
0036eafc: beq #0x36eb44
0036eb00: ldrb r3, [r2, #0x684]
0036eb04: cmp r3, #0
0036eb08: beq #0x36eb18
0036eb0c: cmp r5, #0
0036eb10: bne #0x36eb4c
0036eb14: add r6, r6, #1
0036eb18: ldr r1, [r2, #0xc]
0036eb1c: cmp r1, #0
0036eb20: beq #0x36eb64
0036eb24: mov r2, r1
0036eb28: b #0x36eb30
0036eb2c: mov r2, r3
0036eb30: ldr r3, [r2, #8]
0036eb34: cmp r3, #0
0036eb38: bne #0x36eb2c
0036eb3c: cmp r0, r2
0036eb40: bne #0x36eb00
0036eb44: mov r0, r6
0036eb48: pop {r4, r5, r6, pc}
0036eb4c: ldr r3, [r2, #0x678]
0036eb50: cmp r3, #0
0036eb54: bne #0x36eb14
0036eb58: ldr r1, [r2, #0xc]
0036eb5c: cmp r1, #0
0036eb60: bne #0x36eb24
0036eb64: ldr r3, [r2, #4]
0036eb68: ldr ip, [r3, #0xc]
0036eb6c: cmp ip, r2
0036eb70: bne #0x36eb8c
0036eb74: mov r2, r3
0036eb78: ldr r3, [r3, #4]
0036eb7c: ldr r1, [r3, #0xc]
0036eb80: cmp r2, r1
0036eb84: beq #0x36eb74
0036eb88: ldr r1, [r2, #0xc]
0036eb8c: cmp r3, r1
0036eb90: movne r2, r3
0036eb94: b #0x36eaf8
0036eb98: bl #0x320e98
0036eb9c: ldrb r3, [r0, #0x24]
0036eba0: cmp r3, #0
0036eba4: beq #0x36eaec
0036eba8: bl #0x800f8c
0036ebac: ldr r3, [r0]
0036ebb0: mov lr, pc
0036ebb4: ldr pc, [r3, #0x64]
0036ebb8: cmp r0, #0
0036ebbc: beq #0x36eaec
0036ebc0: bl #0x8100dc
0036ebc4: bl #0x8100e0
0036ebc8: cmp r0, #0
0036ebcc: beq #0x36eaec
0036ebd0: cmp r5, #0
0036ebd4: beq #0x36ec3c
0036ebd8: ldr r3, [r4, #0x6b4]
0036ebdc: ldr r2, [r4, #0x6b8]
0036ebe0: rsb r2, r3, r2
0036ebe4: asrs r2, r2, #2
0036ebe8: moveq r6, r2
0036ebec: beq #0x36eb44
0036ebf0: mov r2, #0
0036ebf4: mov r5, r2
0036ebf8: mov r6, r2
0036ebfc: ldr r1, [r3, r2, lsl #2]
0036ec00: mov r0, r4
0036ec04: mov r2, #0
0036ec08: bl #0x36e744
0036ec0c: ldr r3, [r0, #0x660]
0036ec10: ldr r1, [r4, #0x6b8]
0036ec14: add r5, r5, #1
0036ec18: cmp r3, #0
0036ec1c: ldr r3, [r4, #0x6b4]
0036ec20: addne r6, r6, #1
0036ec24: mov r2, r5
0036ec28: rsb r1, r3, r1
0036ec2c: cmp r5, r1, asr #2
0036ec30: blo #0x36ebfc
0036ec34: mov r0, r6
0036ec38: pop {r4, r5, r6, pc}
0036ec3c: ldr r3, [r4, #0x6b4]
0036ec40: ldr r6, [r4, #0x6b8]
0036ec44: rsb r6, r3, r6
0036ec48: asr r6, r6, #2
0036ec4c: mov r0, r6
0036ec50: pop {r4, r5, r6, pc}

# 0x36ec54 _ZN13PlayerManager12InitialSetupE7Point3DIfE
0036ec54: push {r4, r5, r6, lr}
0036ec58: ldrb r2, [r0, #0x6d0]
0036ec5c: ldr r3, [pc, #0xa0]
0036ec60: mov r6, r1
0036ec64: cmp r2, #0
0036ec68: add r3, pc, r3
0036ec6c: bne #0x36ecf4
0036ec70: mov r2, #1
0036ec74: strb r2, [r0, #0x6d0]
0036ec78: ldr r2, [r6]
0036ec7c: ldr r1, [pc, #0x84]
0036ec80: mov r4, #0
0036ec84: str r2, [r0, #0x6d4]
0036ec88: ldr r2, [r6, #4]
0036ec8c: ldr r5, [r3, r1]
0036ec90: mov r1, #1
0036ec94: str r2, [r0, #0x6d8]
0036ec98: ldr r3, [r6, #8]
0036ec9c: str r3, [r0, #0x6dc]
0036eca0: ldr r0, [r5, #0x40]
0036eca4: bl #0x36ead0
0036eca8: cmp r4, r0
0036ecac: mov r1, r4
0036ecb0: mov r2, #1
0036ecb4: bge #0x36ecf0
0036ecb8: ldr r0, [r5, #0x40]
0036ecbc: bl #0x36e478
0036ecc0: mov r2, #1
0036ecc4: ldr r0, [r0, #0x660]
0036ecc8: mov r1, r6
0036eccc: bl #0x393db4
0036ecd0: mov r1, #1
0036ecd4: ldr r0, [r5, #0x40]
0036ecd8: bl #0x36ead0
0036ecdc: add r4, r4, #1
0036ece0: cmp r4, r0
0036ece4: mov r1, r4
0036ece8: mov r2, #1
0036ecec: blt #0x36ecb8
0036ecf0: pop {r4, r5, r6, pc}
0036ecf4: ldrb r2, [r0, #0x71a]
0036ecf8: cmp r2, #0
0036ecfc: bne #0x36ec70
0036ed00: pop {r4, r5, r6, pc}
0036ed04: rsbeq r5, r2, r8, lsr #28
0036ed08: strdeq r3, r4, [r0], -r4

# 0x36ed0c _ZN13PlayerManager20_UpdatePlayerNumbersEv
0036ed0c: push {r4, r5, r6, r7, r8, lr}
0036ed10: mov r5, r0
0036ed14: bl #0x7fd794
0036ed18: ldrb r3, [r0, #5]
0036ed1c: cmp r3, #0
0036ed20: bne #0x36edd8
0036ed24: ldr r3, [r5, #0x698]
0036ed28: add r6, r5, #0x690
0036ed2c: mov r2, #0
0036ed30: cmp r6, r3
0036ed34: mov r0, r2
0036ed38: mov r1, r2
0036ed3c: beq #0x36eda0
0036ed40: ldrb r4, [r3, #0x684]
0036ed44: str r2, [r3, #0x690]
0036ed48: add ip, r2, #1
0036ed4c: ldr r2, [r3, #0xc]
0036ed50: cmp r4, #0
0036ed54: strne r1, [r3, #0x694]
0036ed58: movne r5, r0
0036ed5c: addne r4, r1, #1
0036ed60: streq r0, [r3, #0x694]
0036ed64: moveq r4, r1
0036ed68: addeq r5, r0, #1
0036ed6c: cmp r2, #0
0036ed70: bne #0x36ed7c
0036ed74: b #0x36eda4
0036ed78: mov r2, r3
0036ed7c: ldr r3, [r2, #8]
0036ed80: cmp r3, #0
0036ed84: bne #0x36ed78
0036ed88: mov r3, r2
0036ed8c: cmp r6, r3
0036ed90: mov r2, ip
0036ed94: mov r0, r5
0036ed98: mov r1, r4
0036ed9c: bne #0x36ed40
0036eda0: pop {r4, r5, r6, r7, r8, pc}
0036eda4: ldr r1, [r3, #4]
0036eda8: ldr r0, [r1, #0xc]
0036edac: cmp r3, r0
0036edb0: bne #0x36edcc
0036edb4: mov r3, r1
0036edb8: ldr r1, [r1, #4]
0036edbc: ldr r0, [r1, #0xc]
0036edc0: cmp r0, r3
0036edc4: beq #0x36edb4
0036edc8: ldr r2, [r3, #0xc]
0036edcc: cmp r2, r1
0036edd0: movne r3, r1
0036edd4: b #0x36ed8c
0036edd8: bl #0x320e98
0036eddc: ldrb r3, [r0, #0x24]
0036ede0: cmp r3, #0
0036ede4: beq #0x36ed24
0036ede8: bl #0x800f8c
0036edec: ldr r3, [r0]
0036edf0: mov lr, pc
0036edf4: ldr pc, [r3, #0x64]
0036edf8: cmp r0, #0
0036edfc: beq #0x36ed24
0036ee00: bl #0x8100dc
0036ee04: bl #0x8100e0
0036ee08: cmp r0, #0
0036ee0c: beq #0x36ed24
0036ee10: ldr r3, [r5, #0x6a8]
0036ee14: ldr r2, [r5, #0x6ac]
0036ee18: rsb r2, r3, r2
0036ee1c: lsrs r2, r2, #2
0036ee20: beq #0x36eea4
0036ee24: mov r6, #0
0036ee28: mov r4, r6
0036ee2c: mov r8, r6
0036ee30: mov r7, r6
0036ee34: b #0x36ee3c
0036ee38: mov r7, r2
0036ee3c: ldr r1, [r3, r4, lsl #2]
0036ee40: mov r2, #0
0036ee44: mov r0, r5
0036ee48: bl #0x36dfb0
0036ee4c: ldr r3, [r0, #0x670]
0036ee50: add r1, r6, #1
0036ee54: cmn r3, #1
0036ee58: streq r3, [r0, #0x678]
0036ee5c: moveq r1, r6
0036ee60: moveq r2, r7
0036ee64: beq #0x36ee88
0036ee68: ldrb r3, [r0, #0x66c]
0036ee6c: mov r2, r7
0036ee70: str r6, [r0, #0x678]
0036ee74: cmp r3, #0
0036ee78: streq r8, [r0, #0x67c]
0036ee7c: strne r7, [r0, #0x67c]
0036ee80: addne r2, r7, #1
0036ee84: addeq r8, r8, #1
0036ee88: ldr r3, [r5, #0x6a8]
0036ee8c: ldr r0, [r5, #0x6ac]
0036ee90: add r4, r4, #1
0036ee94: mov r6, r1
0036ee98: rsb r1, r3, r0
0036ee9c: cmp r4, r1, asr #2
0036eea0: blo #0x36ee38
0036eea4: pop {r4, r5, r6, r7, r8, pc}

# 0x36eea8 _ZN13PlayerManager20GetPlayerByCharacterEPK9Characterb
0036eea8: push {r4, r5, r6, r7, r8, lr}
0036eeac: mov r5, r0
0036eeb0: mov r6, r1
0036eeb4: mov r7, r2
0036eeb8: bl #0x7fd794
0036eebc: ldrb r3, [r0, #5]
0036eec0: cmp r3, #0
0036eec4: bne #0x36ef50
0036eec8: ldr r3, [r5, #0x698]
0036eecc: add ip, r5, #0x690
0036eed0: cmp ip, r3
0036eed4: beq #0x36ef14
0036eed8: ldr r2, [r3, #0x678]
0036eedc: add r0, r3, #0x18
0036eee0: cmp r6, r2
0036eee4: beq #0x36ef18
0036eee8: ldr r2, [r3, #0xc]
0036eeec: cmp r2, #0
0036eef0: bne #0x36eefc
0036eef4: b #0x36ef1c
0036eef8: mov r2, r3
0036eefc: ldr r3, [r2, #8]
0036ef00: cmp r3, #0
0036ef04: bne #0x36eef8
0036ef08: mov r3, r2
0036ef0c: cmp ip, r3
0036ef10: bne #0x36eed8
0036ef14: add r0, r5, #8
0036ef18: pop {r4, r5, r6, r7, r8, pc}
0036ef1c: ldr r1, [r3, #4]
0036ef20: ldr r0, [r1, #0xc]
0036ef24: cmp r0, r3
0036ef28: bne #0x36ef44
0036ef2c: mov r3, r1
0036ef30: ldr r1, [r1, #4]
0036ef34: ldr r2, [r1, #0xc]
0036ef38: cmp r3, r2
0036ef3c: beq #0x36ef2c
0036ef40: ldr r2, [r3, #0xc]
0036ef44: cmp r1, r2
0036ef48: movne r3, r1
0036ef4c: b #0x36eed0
0036ef50: bl #0x320e98
0036ef54: ldrb r3, [r0, #0x24]
0036ef58: cmp r3, #0
0036ef5c: beq #0x36eec8
0036ef60: bl #0x800f8c
0036ef64: ldr r3, [r0]
0036ef68: mov lr, pc
0036ef6c: ldr pc, [r3, #0x64]
0036ef70: cmp r0, #0
0036ef74: beq #0x36eec8
0036ef78: bl #0x8100dc
0036ef7c: bl #0x8100e0
0036ef80: cmp r0, #0
0036ef84: beq #0x36eec8
0036ef88: ldr r2, [r5, #0x6a8]
0036ef8c: ldr r3, [r5, #0x6ac]
0036ef90: rsb r3, r2, r3
0036ef94: lsrs r3, r3, #2
0036ef98: beq #0x36ef14
0036ef9c: mov r3, #0
0036efa0: mov r4, r3
0036efa4: b #0x36efbc
0036efa8: ldr r2, [r5, #0x6a8]
0036efac: ldr r1, [r5, #0x6ac]
0036efb0: rsb r1, r2, r1
0036efb4: cmp r4, r1, asr #2
0036efb8: bhs #0x36ef14
0036efbc: ldr r1, [r2, r3, lsl #2]
0036efc0: mov r0, r5
0036efc4: mov r2, r7
0036efc8: lsl r8, r3, #2
0036efcc: bl #0x36dfb0
0036efd0: ldr r2, [r0, #0x660]
0036efd4: add r4, r4, #1
0036efd8: mov r3, r4
0036efdc: cmp r6, r2
0036efe0: bne #0x36efa8
0036efe4: ldr r3, [r5, #0x6a8]
0036efe8: mov r0, r5
0036efec: mov r2, r7
0036eff0: ldr r1, [r3, r8]
0036eff4: pop {r4, r5, r6, r7, r8, lr}
0036eff8: b #0x36dec4

# 0x36effc _ZN13PlayerManager13IsLocalPlayerEPK9Character
0036effc: subs r3, r1, #0
0036f000: push {r4, lr}
0036f004: beq #0x36f020
0036f008: mov r2, #0
0036f00c: bl #0x36eea8
0036f010: ldr r3, [r0]
0036f014: mov lr, pc
0036f018: ldr pc, [r3, #0x50]
0036f01c: pop {r4, pc}
0036f020: mov r0, r3
0036f024: pop {r4, pc}

# 0x36f074 _ZN13PlayerManager20IsLocalPlayerHostingEv
0036f074: push {r4, lr}
0036f078: mov r4, r0
0036f07c: bl #0x7fd794
0036f080: ldrb r3, [r0, #5]
0036f084: cmp r3, #0
0036f088: bne #0x36f0a4
0036f08c: bl #0x7fd794
0036f090: ldrb r3, [r0, #5]
0036f094: cmp r3, #0
0036f098: bne #0x36f0c4
0036f09c: mov r0, #1
0036f0a0: pop {r4, pc}
0036f0a4: bl #0x320e98
0036f0a8: ldr r3, [r0, #0x34]
0036f0ac: sub r3, r3, #3
0036f0b0: cmp r3, #1
0036f0b4: bhi #0x36f08c
0036f0b8: bl #0x800f8c
0036f0bc: pop {r4, lr}
0036f0c0: b #0x81f524
0036f0c4: mov r1, #0
0036f0c8: mov r0, r4
0036f0cc: mov r2, r1
0036f0d0: bl #0x36e478
0036f0d4: pop {r4, lr}
0036f0d8: b #0x80f23c

# 0x36f0dc _ZN13PlayerManager34_AttachControllerToPlayerCharacterEi
0036f0dc: push {r4, r5, r6, r7, r8, lr}
0036f0e0: mov r2, #0
0036f0e4: bl #0x36dfb0
0036f0e8: ldrb r1, [r0, #0x66c]
0036f0ec: mov r6, r0
0036f0f0: ldr r5, [r0, #0x660]
0036f0f4: cmp r1, #0
0036f0f8: beq #0x36f1d8
0036f0fc: mov r1, #0
0036f100: mov r0, #0x24
0036f104: bl #0x310570
0036f108: cmp r5, #0
0036f10c: addne r7, r5, #0x374
0036f110: moveq r7, #0x374
0036f114: movne r1, r7
0036f118: moveq r1, r5
0036f11c: mov r4, r0
0036f120: bl #0x408dd8
0036f124: mov r0, r7
0036f128: mov r1, r4
0036f12c: bl #0x404e10
0036f130: ldr r3, [r5, #0x378]
0036f134: str r5, [r3, #0xc]
0036f138: bl #0x7fd794
0036f13c: ldrb r3, [r0, #5]
0036f140: mov r1, #0
0036f144: mov r0, #0x20
0036f148: cmp r3, #0
0036f14c: ldrne r3, [r5, #0x378]
0036f150: movne r2, #1
0036f154: strbne r2, [r3, #0xa]
0036f158: ldr r7, [r6, #0x668]
0036f15c: bl #0x310570
0036f160: cmp r4, #0
0036f164: mov r6, r0
0036f168: beq #0x36f210
0036f16c: add r5, r4, #0x10
0036f170: mov r2, r7
0036f174: mov r1, r5
0036f178: bl #0x406978
0036f17c: mov r1, r6
0036f180: mov r0, r4
0036f184: bl #0x408fc0
0036f188: mov r1, #0
0036f18c: mov r0, #0x1c
0036f190: bl #0x310570
0036f194: mov r1, r5
0036f198: mov r6, r0
0036f19c: bl #0x4065ac
0036f1a0: mov r1, r6
0036f1a4: mov r0, r4
0036f1a8: bl #0x408fc0
0036f1ac: mov r0, #0x10
0036f1b0: mov r1, #0
0036f1b4: bl #0x310570
0036f1b8: mov r6, r0
0036f1bc: mov r1, r5
0036f1c0: mov r0, r6
0036f1c4: bl #0x408930
0036f1c8: mov r0, r4
0036f1cc: mov r1, r6
0036f1d0: pop {r4, r5, r6, r7, r8, lr}
0036f1d4: b #0x408fc0
0036f1d8: mov r0, #0x10
0036f1dc: bl #0x310570
0036f1e0: add r4, r5, #0x374
0036f1e4: cmp r5, #0
0036f1e8: movne r1, r4
0036f1ec: moveq r1, #0
0036f1f0: mov r6, r0
0036f1f4: bl #0x4091f4
0036f1f8: mov r0, r4
0036f1fc: mov r1, r6
0036f200: bl #0x404e10
0036f204: ldr r3, [r5, #0x378]
0036f208: str r5, [r3, #0xc]
0036f20c: pop {r4, r5, r6, r7, r8, pc}
0036f210: mov r2, r7
0036f214: mov r1, r4
0036f218: bl #0x406978
0036f21c: mov r1, r6
0036f220: mov r0, r4
0036f224: bl #0x408fc0
0036f228: mov r1, r4
0036f22c: mov r0, #0x1c
0036f230: bl #0x310570
0036f234: mov r1, r4
0036f238: mov r5, r0
0036f23c: bl #0x4065ac
0036f240: mov r1, r5
0036f244: mov r0, r4
0036f248: bl #0x408fc0
0036f24c: mov r0, #0x10
0036f250: mov r1, r4
0036f254: bl #0x310570
0036f258: mov r5, r4
0036f25c: mov r6, r0
0036f260: b #0x36f1bc

# 0x36f2bc _ZN13PlayerManager18PostInitCharactersEv
0036f2bc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036f2c0: sub sp, sp, #0x14
0036f2c4: mov r6, r0
0036f2c8: bl #0x36d7a8
0036f2cc: ldr r8, [pc, #0x120]
0036f2d0: subs r5, r0, #0
0036f2d4: add r8, pc, r8
0036f2d8: ble #0x36f388
0036f2dc: ldr r3, [pc, #0x114]
0036f2e0: ldr fp, [pc, #0x114]
0036f2e4: ldr sl, [pc, #0x114]
0036f2e8: add r3, pc, r3
0036f2ec: str r3, [sp, #8]
0036f2f0: ldr r3, [pc, #0x10c]
0036f2f4: ldr sb, [pc, #0x10c]
0036f2f8: add fp, pc, fp
0036f2fc: add r3, pc, r3
0036f300: str r3, [sp, #0xc]
0036f304: mov r4, #0
0036f308: b #0x36f318
0036f30c: add r4, r4, #1
0036f310: cmp r4, r5
0036f314: beq #0x36f388
0036f318: mov r1, r4
0036f31c: mov r2, #0
0036f320: mov r0, r6
0036f324: bl #0x36e744
0036f328: ldr r3, [r0]
0036f32c: mov r7, r0
0036f330: mov lr, pc
0036f334: ldr pc, [r3, #0x50]
0036f338: cmp r0, #0
0036f33c: beq #0x36f30c
0036f340: ldr r3, [r7]
0036f344: mov r0, r7
0036f348: mov lr, pc
0036f34c: ldr pc, [r3, #0x5c]
0036f350: cmp r0, #0
0036f354: beq #0x36f30c
0036f358: ldr r7, [r7, #0x660]
0036f35c: bl #0x7fd794
0036f360: ldrb r3, [r0, #5]
0036f364: cmp r3, #0
0036f368: beq #0x36f390
0036f36c: cmp r7, #0
0036f370: beq #0x36f3d8
0036f374: mov r0, r7
0036f378: add r4, r4, #1
0036f37c: bl #0x3bd000
0036f380: cmp r4, r5
0036f384: bne #0x36f318
0036f388: add sp, sp, #0x14
0036f38c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036f390: cmp r7, #0
0036f394: bne #0x36f374
0036f398: ldr r3, [r8, sl]
0036f39c: ldr r3, [r3]
0036f3a0: cmp r3, #2
0036f3a4: streq r7, [r7]
0036f3a8: beq #0x36f374
0036f3ac: cmp r3, #1
0036f3b0: bne #0x36f374
0036f3b4: ldr r0, [r8, sb]
0036f3b8: movw ip, #0x443
0036f3bc: mov r1, fp
0036f3c0: ldr r2, [sp, #8]
0036f3c4: ldr r3, [sp, #0xc]
0036f3c8: add r0, r0, #0xa8
0036f3cc: str ip, [sp]
0036f3d0: bl #0x30e004
0036f3d4: b #0x36f374
0036f3d8: bl #0x800f8c
0036f3dc: ldr r3, [r0]
0036f3e0: mov lr, pc
0036f3e4: ldr pc, [r3, #0x3c]
0036f3e8: bl #0x7fbd74
0036f3ec: bl #0x7fbe54
0036f3f0: b #0x36f30c
0036f3f4: strhteq r5, [r2], #-0x7c
0036f3f8: subseq r2, r8, r0, lsr #27
0036f3fc: subseq pc, r4, r0, ror #1
0036f400: andeq r3, r0, r0, asr #19
0036f404: ldrsheq r2, [r5], #-0x34
0036f408: andeq r1, r0, r0, asr #19

# 0x36f4b4 _ZN13PlayerManager24_UpdateJoiningControllerEii
0036f4b4: push {r4, r5, r6, r7, lr}
0036f4b8: sub sp, sp, #0xf4
0036f4bc: mov r6, r2
0036f4c0: mov r7, r0
0036f4c4: mov r5, r1
0036f4c8: bl #0x34dda4
0036f4cc: mov r1, r5
0036f4d0: ldr r3, [r0]
0036f4d4: mov lr, pc
0036f4d8: ldr pc, [r3, #8]
0036f4dc: mov r2, #0
0036f4e0: mov r4, r0
0036f4e4: mov r1, r6
0036f4e8: mov r0, r7
0036f4ec: bl #0x36dfb0
0036f4f0: ldr r1, [r4, #0x4c0]
0036f4f4: mov r7, r0
0036f4f8: ldr r0, [r4, #0x4c4]
0036f4fc: bl #0x30eba4
0036f500: mov r1, #0x3f800000
0036f504: bl #0x30eba4
0036f508: mov r1, #0x3f000000
0036f50c: bl #0x30ed6c
0036f510: mov r1, r0
0036f514: ldr r0, [r4, #0x4b8]
0036f518: bl #0x30e4b4
0036f51c: ldr r6, [pc, #0x718]
0036f520: cmp r0, #0
0036f524: add r6, pc, r6
0036f528: bne #0x36fb20
0036f52c: ldrb r3, [r4, #0x4c8]
0036f530: cmp r3, #0
0036f534: bne #0x36fb88
0036f538: ldr r1, [r4, #0x4e0]
0036f53c: ldr r0, [r4, #0x4e4]
0036f540: bl #0x30eba4
0036f544: mov r1, #0x3f800000
0036f548: bl #0x30eba4
0036f54c: mov r1, #0x3f000000
0036f550: bl #0x30ed6c
0036f554: mov r1, r0
0036f558: ldr r0, [r4, #0x4d8]
0036f55c: bl #0x30e4b4
0036f560: cmp r0, #0
0036f564: bne #0x36fab8
0036f568: ldrb r3, [r4, #0x4e8]
0036f56c: cmp r3, #0
0036f570: bne #0x36fb9c
0036f574: ldr r1, [r4, #0x500]
0036f578: ldr r0, [r4, #0x504]
0036f57c: bl #0x30eba4
0036f580: mov r1, #0x3f800000
0036f584: bl #0x30eba4
0036f588: mov r1, #0x3f000000
0036f58c: bl #0x30ed6c
0036f590: mov r1, r0
0036f594: ldr r0, [r4, #0x4f8]
0036f598: bl #0x30e4b4
0036f59c: cmp r0, #0
0036f5a0: bne #0x36fa50
0036f5a4: ldrb r3, [r4, #0x508]
0036f5a8: cmp r3, #0
0036f5ac: bne #0x36fbb0
0036f5b0: ldr r1, [r4, #0x520]
0036f5b4: ldr r0, [r4, #0x524]
0036f5b8: bl #0x30eba4
0036f5bc: mov r1, #0x3f800000
0036f5c0: bl #0x30eba4
0036f5c4: mov r1, #0x3f000000
0036f5c8: bl #0x30ed6c
0036f5cc: mov r1, r0
0036f5d0: ldr r0, [r4, #0x518]
0036f5d4: bl #0x30e4b4
0036f5d8: cmp r0, #0
0036f5dc: bne #0x36f9e8
0036f5e0: ldrb r3, [r4, #0x528]
0036f5e4: cmp r3, #0
0036f5e8: bne #0x36fbc4
0036f5ec: ldr r1, [r4, #0x160]
0036f5f0: ldr r0, [r4, #0x164]
0036f5f4: bl #0x30eba4
0036f5f8: mov r1, #0x3f800000
0036f5fc: bl #0x30eba4
0036f600: mov r1, #0x3f000000
0036f604: bl #0x30ed6c
0036f608: mov r1, r0
0036f60c: ldr r0, [r4, #0x158]
0036f610: bl #0x30e4b4
0036f614: cmp r0, #0
0036f618: bne #0x36f980
0036f61c: ldrb r3, [r4, #0x168]
0036f620: cmp r3, #0
0036f624: bne #0x36fbd8
0036f628: ldr r1, [r4, #0x180]
0036f62c: ldr r0, [r4, #0x184]
0036f630: bl #0x30eba4
0036f634: mov r1, #0x3f800000
0036f638: bl #0x30eba4
0036f63c: mov r1, #0x3f000000
0036f640: bl #0x30ed6c
0036f644: mov r1, r0
0036f648: ldr r0, [r4, #0x178]
0036f64c: bl #0x30e4b4
0036f650: cmp r0, #0
0036f654: bne #0x36f918
0036f658: ldrb r3, [r4, #0x188]
0036f65c: cmp r3, #0
0036f660: bne #0x36fbec
0036f664: ldr r1, [r4, #0x1a0]
0036f668: ldr r0, [r4, #0x1a4]
0036f66c: bl #0x30eba4
0036f670: mov r1, #0x3f800000
0036f674: bl #0x30eba4
0036f678: mov r1, #0x3f000000
0036f67c: bl #0x30ed6c
0036f680: mov r1, r0
0036f684: ldr r0, [r4, #0x198]
0036f688: bl #0x30e4b4
0036f68c: cmp r0, #0
0036f690: bne #0x36f8b0
0036f694: ldrb r3, [r4, #0x1a8]
0036f698: cmp r3, #0
0036f69c: bne #0x36fc00
0036f6a0: ldr r1, [r4, #0x1c0]
0036f6a4: ldr r0, [r4, #0x1c4]
0036f6a8: bl #0x30eba4
0036f6ac: mov r1, #0x3f800000
0036f6b0: bl #0x30eba4
0036f6b4: mov r1, #0x3f000000
0036f6b8: bl #0x30ed6c
0036f6bc: mov r1, r0
0036f6c0: ldr r0, [r4, #0x1b8]
0036f6c4: bl #0x30e4b4
0036f6c8: cmp r0, #0
0036f6cc: bne #0x36f848
0036f6d0: ldrb r3, [r4, #0x1c8]
0036f6d4: cmp r3, #0
0036f6d8: bne #0x36fc14
0036f6dc: ldr r1, [r4, #0x20]
0036f6e0: ldr r0, [r4, #0x24]
0036f6e4: bl #0x30eba4
0036f6e8: mov r1, #0x3f800000
0036f6ec: bl #0x30eba4
0036f6f0: mov r1, #0x3f000000
0036f6f4: bl #0x30ed6c
0036f6f8: mov r1, r0
0036f6fc: ldr r0, [r4, #0x18]
0036f700: bl #0x30e4b4
0036f704: cmp r0, #0
0036f708: bne #0x36f7e0
0036f70c: ldrb r3, [r4, #0x28]
0036f710: cmp r3, #0
0036f714: bne #0x36fc28
0036f718: ldr r1, [r4, #0x40]
0036f71c: ldr r0, [r4, #0x44]
0036f720: bl #0x30eba4
0036f724: mov r1, #0x3f800000
0036f728: bl #0x30eba4
0036f72c: mov r1, #0x3f000000
0036f730: bl #0x30ed6c
0036f734: mov r1, r0
0036f738: ldr r0, [r4, #0x38]
0036f73c: bl #0x30e4b4
0036f740: cmp r0, #0
0036f744: bne #0x36f768
0036f748: ldrb r3, [r4, #0x48]
0036f74c: cmp r3, #0
0036f750: beq #0x36f7d8
0036f754: ldr r3, [pc, #0x4e4]
0036f758: mov ip, #0
0036f75c: ldr r3, [r6, r3]
0036f760: ldr r0, [r3, #0x14]
0036f764: b #0x36f784
0036f768: ldrb r3, [r4, #0x48]
0036f76c: cmp r3, #0
0036f770: bne #0x36f7d8
0036f774: ldr r3, [pc, #0x4c4]
0036f778: mov ip, #1
0036f77c: ldr r3, [r6, r3]
0036f780: ldr r0, [r3, #0x14]
0036f784: ldr r3, [pc, #0x4b8]
0036f788: mov r1, sp
0036f78c: mov r2, #7
0036f790: ldr r3, [r6, r3]
0036f794: str r2, [sp, #4]
0036f798: str ip, [sp, #0xc]
0036f79c: add r3, r3, #8
0036f7a0: str r3, [sp]
0036f7a4: mov r3, #1
0036f7a8: str r3, [sp, #8]
0036f7ac: mov r3, #0x3f800000
0036f7b0: str r3, [sp, #0x14]
0036f7b4: str r5, [sp, #0x10]
0036f7b8: bl #0x338ebc
0036f7bc: ldr r3, [pc, #0x484]
0036f7c0: mov r0, r7
0036f7c4: mov r1, #0
0036f7c8: ldr r3, [r6, r3]
0036f7cc: add r3, r3, #8
0036f7d0: str r3, [sp]
0036f7d4: bl #0x36f40c
0036f7d8: add sp, sp, #0xf4
0036f7dc: pop {r4, r5, r6, r7, pc}
0036f7e0: ldrb r3, [r4, #0x28]
0036f7e4: cmp r3, #0
0036f7e8: bne #0x36f718
0036f7ec: ldr r3, [pc, #0x44c]
0036f7f0: mov ip, #1
0036f7f4: ldr r3, [r6, r3]
0036f7f8: ldr r0, [r3, #0x14]
0036f7fc: ldr r3, [pc, #0x440]
0036f800: mov r2, #7
0036f804: add r1, sp, #0x18
0036f808: ldr r3, [r6, r3]
0036f80c: str r2, [sp, #0x1c]
0036f810: str ip, [sp, #0x24]
0036f814: add r3, r3, #8
0036f818: str r3, [sp, #0x18]
0036f81c: mov r3, #0
0036f820: str r3, [sp, #0x20]
0036f824: mov r3, #0x3f800000
0036f828: str r3, [sp, #0x2c]
0036f82c: str r5, [sp, #0x28]
0036f830: bl #0x338ebc
0036f834: ldr r3, [pc, #0x40c]
0036f838: ldr r3, [r6, r3]
0036f83c: add r3, r3, #8
0036f840: str r3, [sp, #0x18]
0036f844: b #0x36f718
0036f848: ldrb r3, [r4, #0x1c8]
0036f84c: cmp r3, #0
0036f850: bne #0x36f6dc
0036f854: ldr r3, [pc, #0x3e4]
0036f858: mov ip, #1
0036f85c: ldr r3, [r6, r3]
0036f860: ldr r0, [r3, #0x14]
0036f864: ldr r3, [pc, #0x3d8]
0036f868: mov r2, #7
0036f86c: add r1, sp, #0x30
0036f870: ldr r3, [r6, r3]
0036f874: str r2, [sp, #0x34]
0036f878: str ip, [sp, #0x3c]
0036f87c: add r3, r3, #8
0036f880: str r3, [sp, #0x30]
0036f884: mov r3, #0xd
0036f888: str r3, [sp, #0x38]
0036f88c: mov r3, #0x3f800000
0036f890: str r3, [sp, #0x44]
0036f894: str r5, [sp, #0x40]
0036f898: bl #0x338ebc
0036f89c: ldr r3, [pc, #0x3a4]
0036f8a0: ldr r3, [r6, r3]
0036f8a4: add r3, r3, #8
0036f8a8: str r3, [sp, #0x30]
0036f8ac: b #0x36f6dc
0036f8b0: ldrb r3, [r4, #0x1a8]
0036f8b4: cmp r3, #0
0036f8b8: bne #0x36f6a0
0036f8bc: ldr r3, [pc, #0x37c]
0036f8c0: mov ip, #1
0036f8c4: ldr r3, [r6, r3]
0036f8c8: ldr r0, [r3, #0x14]
0036f8cc: ldr r3, [pc, #0x370]
0036f8d0: mov r2, #7
0036f8d4: add r1, sp, #0x48
0036f8d8: ldr r3, [r6, r3]
0036f8dc: str r2, [sp, #0x4c]
0036f8e0: str ip, [sp, #0x54]
0036f8e4: add r3, r3, #8
0036f8e8: str r3, [sp, #0x48]
0036f8ec: mov r3, #0xc
0036f8f0: str r3, [sp, #0x50]
0036f8f4: mov r3, #0x3f800000
0036f8f8: str r3, [sp, #0x5c]
0036f8fc: str r5, [sp, #0x58]
0036f900: bl #0x338ebc
0036f904: ldr r3, [pc, #0x33c]
0036f908: ldr r3, [r6, r3]
0036f90c: add r3, r3, #8
0036f910: str r3, [sp, #0x48]
0036f914: b #0x36f6a0
0036f918: ldrb r3, [r4, #0x188]
0036f91c: cmp r3, #0
0036f920: bne #0x36f664
0036f924: ldr r3, [pc, #0x314]
0036f928: mov ip, #1
0036f92c: ldr r3, [r6, r3]
0036f930: ldr r0, [r3, #0x14]
0036f934: ldr r3, [pc, #0x308]
0036f938: mov r2, #7
0036f93c: add r1, sp, #0x60
0036f940: ldr r3, [r6, r3]
0036f944: str r2, [sp, #0x64]
0036f948: str ip, [sp, #0x6c]
0036f94c: add r3, r3, #8
0036f950: str r3, [sp, #0x60]
0036f954: mov r3, #0xb
0036f958: str r3, [sp, #0x68]
0036f95c: mov r3, #0x3f800000
0036f960: str r3, [sp, #0x74]
0036f964: str r5, [sp, #0x70]
0036f968: bl #0x338ebc
0036f96c: ldr r3, [pc, #0x2d4]
0036f970: ldr r3, [r6, r3]
0036f974: add r3, r3, #8
0036f978: str r3, [sp, #0x60]
0036f97c: b #0x36f664
0036f980: ldrb r3, [r4, #0x168]
0036f984: cmp r3, #0
0036f988: bne #0x36f628
0036f98c: ldr r3, [pc, #0x2ac]
0036f990: mov ip, #1
0036f994: ldr r3, [r6, r3]
0036f998: ldr r0, [r3, #0x14]
0036f99c: ldr r3, [pc, #0x2a0]
0036f9a0: mov r2, #7
0036f9a4: add r1, sp, #0x78
0036f9a8: ldr r3, [r6, r3]
0036f9ac: str r2, [sp, #0x7c]
0036f9b0: str ip, [sp, #0x84]
0036f9b4: add r3, r3, #8
0036f9b8: str r3, [sp, #0x78]
0036f9bc: mov r3, #0xa
0036f9c0: str r3, [sp, #0x80]
0036f9c4: mov r3, #0x3f800000
0036f9c8: str r3, [sp, #0x8c]
0036f9cc: str r5, [sp, #0x88]
0036f9d0: bl #0x338ebc
0036f9d4: ldr r3, [pc, #0x26c]
0036f9d8: ldr r3, [r6, r3]
0036f9dc: add r3, r3, #8
0036f9e0: str r3, [sp, #0x78]
0036f9e4: b #0x36f628
0036f9e8: ldrb r3, [r4, #0x528]
0036f9ec: cmp r3, #0
0036f9f0: bne #0x36f5ec
0036f9f4: ldr r3, [pc, #0x244]
0036f9f8: mov ip, #1
0036f9fc: ldr r3, [r6, r3]
0036fa00: ldr r0, [r3, #0x14]
0036fa04: ldr r3, [pc, #0x238]
0036fa08: mov r2, #7
0036fa0c: add r1, sp, #0x90
0036fa10: ldr r3, [r6, r3]
0036fa14: str r2, [sp, #0x94]
0036fa18: str ip, [sp, #0x9c]
0036fa1c: add r3, r3, #8
0036fa20: str r3, [sp, #0x90]
0036fa24: mov r3, #0x28
0036fa28: str r3, [sp, #0x98]
0036fa2c: mov r3, #0x3f800000
0036fa30: str r3, [sp, #0xa4]
0036fa34: str r5, [sp, #0xa0]
0036fa38: bl #0x338ebc
0036fa3c: ldr r3, [pc, #0x204]
0036fa40: ldr r3, [r6, r3]
0036fa44: add r3, r3, #8
0036fa48: str r3, [sp, #0x90]
0036fa4c: b #0x36f5ec
0036fa50: ldrb r3, [r4, #0x508]
0036fa54: cmp r3, #0
0036fa58: bne #0x36f5b0
0036fa5c: ldr r3, [pc, #0x1dc]
0036fa60: mov ip, #1
0036fa64: ldr r3, [r6, r3]
0036fa68: ldr r0, [r3, #0x14]
0036fa6c: ldr r3, [pc, #0x1d0]
0036fa70: mov r2, #7
0036fa74: add r1, sp, #0xa8
0036fa78: ldr r3, [r6, r3]
0036fa7c: str r2, [sp, #0xac]
0036fa80: str ip, [sp, #0xb4]
0036fa84: add r3, r3, #8
0036fa88: str r3, [sp, #0xa8]
0036fa8c: mov r3, #0x27
0036fa90: str r3, [sp, #0xb0]
0036fa94: mov r3, #0x3f800000
0036fa98: str r3, [sp, #0xbc]
0036fa9c: str r5, [sp, #0xb8]
0036faa0: bl #0x338ebc
0036faa4: ldr r3, [pc, #0x19c]
0036faa8: ldr r3, [r6, r3]
0036faac: add r3, r3, #8
0036fab0: str r3, [sp, #0xa8]
0036fab4: b #0x36f5b0
0036fab8: ldrb r3, [r4, #0x4e8]
0036fabc: cmp r3, #0
0036fac0: bne #0x36f574
0036fac4: ldr r3, [pc, #0x174]
0036fac8: mov ip, #1
0036facc: ldr r3, [r6, r3]
0036fad0: ldr r0, [r3, #0x14]
0036fad4: ldr r3, [pc, #0x168]
0036fad8: mov r2, #7
0036fadc: add r1, sp, #0xc0
0036fae0: ldr r3, [r6, r3]
0036fae4: str r2, [sp, #0xc4]
0036fae8: str ip, [sp, #0xcc]
0036faec: add r3, r3, #8
0036faf0: str r3, [sp, #0xc0]
0036faf4: mov r3, #0x26
0036faf8: str r3, [sp, #0xc8]
0036fafc: mov r3, #0x3f800000
0036fb00: str r3, [sp, #0xd4]
0036fb04: str r5, [sp, #0xd0]
0036fb08: bl #0x338ebc
0036fb0c: ldr r3, [pc, #0x134]
0036fb10: ldr r3, [r6, r3]
0036fb14: add r3, r3, #8
0036fb18: str r3, [sp, #0xc0]
0036fb1c: b #0x36f574
0036fb20: ldrb r3, [r4, #0x4c8]
0036fb24: cmp r3, #0
0036fb28: bne #0x36f538
0036fb2c: ldr r3, [pc, #0x10c]
0036fb30: mov ip, #1
0036fb34: ldr r3, [r6, r3]
0036fb38: ldr r0, [r3, #0x14]
0036fb3c: ldr r3, [pc, #0x100]
0036fb40: mov r2, #7
0036fb44: add r1, sp, #0xd8
0036fb48: ldr r3, [r6, r3]
0036fb4c: str r2, [sp, #0xdc]
0036fb50: str ip, [sp, #0xe4]
0036fb54: add r3, r3, #8
0036fb58: str r3, [sp, #0xd8]
0036fb5c: mov r3, #0x25
0036fb60: str r3, [sp, #0xe0]
0036fb64: mov r3, #0x3f800000
0036fb68: str r3, [sp, #0xec]
0036fb6c: str r5, [sp, #0xe8]
0036fb70: bl #0x338ebc
0036fb74: ldr r3, [pc, #0xcc]
0036fb78: ldr r3, [r6, r3]
0036fb7c: add r3, r3, #8
0036fb80: str r3, [sp, #0xd8]
0036fb84: b #0x36f538
0036fb88: ldr r3, [pc, #0xb0]
0036fb8c: mov ip, #0
0036fb90: ldr r3, [r6, r3]
0036fb94: ldr r0, [r3, #0x14]
0036fb98: b #0x36fb3c
0036fb9c: ldr r3, [pc, #0x9c]
0036fba0: mov ip, #0
0036fba4: ldr r3, [r6, r3]
0036fba8: ldr r0, [r3, #0x14]
0036fbac: b #0x36fad4
0036fbb0: ldr r3, [pc, #0x88]
0036fbb4: mov ip, #0
0036fbb8: ldr r3, [r6, r3]
0036fbbc: ldr r0, [r3, #0x14]
0036fbc0: b #0x36fa6c
0036fbc4: ldr r3, [pc, #0x74]
0036fbc8: mov ip, #0
0036fbcc: ldr r3, [r6, r3]
0036fbd0: ldr r0, [r3, #0x14]
0036fbd4: b #0x36fa04
0036fbd8: ldr r3, [pc, #0x60]
0036fbdc: mov ip, #0
0036fbe0: ldr r3, [r6, r3]
0036fbe4: ldr r0, [r3, #0x14]
0036fbe8: b #0x36f99c
0036fbec: ldr r3, [pc, #0x4c]
0036fbf0: mov ip, #0
0036fbf4: ldr r3, [r6, r3]
0036fbf8: ldr r0, [r3, #0x14]
0036fbfc: b #0x36f934
0036fc00: ldr r3, [pc, #0x38]
0036fc04: mov ip, #0
0036fc08: ldr r3, [r6, r3]
0036fc0c: ldr r0, [r3, #0x14]
0036fc10: b #0x36f8cc
0036fc14: ldr r3, [pc, #0x24]
0036fc18: mov ip, #0
0036fc1c: ldr r3, [r6, r3]
0036fc20: ldr r0, [r3, #0x14]
0036fc24: b #0x36f864
0036fc28: ldr r3, [pc, #0x10]
0036fc2c: mov ip, #0
0036fc30: ldr r3, [r6, r3]
0036fc34: ldr r0, [r3, #0x14]
0036fc38: b #0x36f7fc
0036fc3c: rsbeq r5, r2, ip, ror #10
0036fc40: strdeq r3, r4, [r0], -r4
0036fc44: muleq r0, r0, sp
0036fc48: strheq r0, [r0], -r0

# 0x370020 _ZN13PlayerManager14_PackInventoryER10PlayerInfo
00370020: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00370024: ldrb r3, [r1, #0x66c]
00370028: ldr r5, [pc, #0x214]
0037002c: sub sp, sp, #0x2c
00370030: cmp r3, #0
00370034: mov sl, r1
00370038: add r5, pc, r5
0037003c: beq #0x37023c
00370040: ldr r8, [r1, #0x660]
00370044: cmp r8, #0
00370048: beq #0x37023c
0037004c: ldr r3, [pc, #0x1f4]
00370050: ldr r0, [r5, r3]
00370054: bl #0x31f594
00370058: cmp r0, #0
0037005c: beq #0x37023c
00370060: ldrb r3, [r0, #0x144]
00370064: cmp r3, #0
00370068: beq #0x37023c
0037006c: add r8, r8, #0x37c
00370070: mov r0, r8
00370074: bl #0x3ffd20
00370078: subs r7, r0, #0
0037007c: movle r6, #0
00370080: ble #0x370130
00370084: mov r4, #0
00370088: mov r6, r4
0037008c: b #0x3700b8
00370090: mov r0, r8
00370094: mov r1, r4
00370098: bl #0x3ffe3c
0037009c: cmp r0, #0
003700a0: beq #0x370120
003700a4: bl #0x3f9e00
003700a8: eor r6, r6, r0
003700ac: add r4, r4, #1
003700b0: cmp r7, r4
003700b4: beq #0x370130
003700b8: sub r3, r4, #1
003700bc: cmp r3, #1
003700c0: bhi #0x370090
003700c4: cmp r4, #2
003700c8: beq #0x3700ac
003700cc: mov r1, #1
003700d0: mov r0, r8
003700d4: bl #0x3ffe3c
003700d8: mov r1, #2
003700dc: mov sb, r0
003700e0: mov r0, r8
003700e4: bl #0x3ffe3c
003700e8: cmp sb, #0
003700ec: mov fp, r0
003700f0: beq #0x370100
003700f4: mov r0, sb
003700f8: bl #0x3f9e00
003700fc: mov sb, r0
00370100: cmp fp, #0
00370104: moveq r0, fp
00370108: beq #0x370114
0037010c: mov r0, fp
00370110: bl #0x3f9e00
00370114: adds sb, r0, sb
00370118: eorne r6, r6, sb
0037011c: bne #0x3700ac
00370120: add r4, r4, #1
00370124: cmp r7, r4
00370128: mvn r6, r6
0037012c: bne #0x3700b8
00370130: ldr r3, [sl, #0x4c0]
00370134: cmp r6, r3
00370138: beq #0x37023c
0037013c: ldr r3, [pc, #0x108]
00370140: ldr r2, [sp, #0x20]
00370144: mov r0, #0x20
00370148: ldr r3, [r5, r3]
0037014c: mvn ip, #0
00370150: cmp r6, r2
00370154: add r3, r3, #8
00370158: mov r2, #0
0037015c: str r0, [sp, #4]
00370160: mov r1, #0
00370164: mov r0, #0
00370168: strd r0, r1, [sp, #8]
0037016c: str ip, [sp, #0x14]
00370170: strb r2, [sp, #0x1c]
00370174: str r3, [sp]
00370178: str ip, [sp, #0x10]
0037017c: str r2, [sp, #0x18]
00370180: moveq r4, sp
00370184: beq #0x370198
00370188: mov r0, sp
0037018c: mov r4, sp
00370190: str r6, [sp, #0x20]
00370194: bl #0x814f84
00370198: ldr r2, [pc, #0xb0]
0037019c: add r1, r4, #0x20
003701a0: ldr r3, [sl, #0x4a0]
003701a4: ldr r2, [r5, r2]
003701a8: add r0, sl, #0x4a0
003701ac: lsl sb, r7, #2
003701b0: add r2, r2, #8
003701b4: str r2, [sp]
003701b8: mov lr, pc
003701bc: ldr pc, [r3, #0x1c]
003701c0: ldr r3, [pc, #0x8c]
003701c4: mov r0, sb
003701c8: mov r1, #0
003701cc: ldr r3, [r5, r3]
003701d0: add r3, r3, #8
003701d4: str r3, [sp]
003701d8: bl #0x31056c
003701dc: cmp r7, #0
003701e0: mov r4, r0
003701e4: ble #0x370224
003701e8: mov r6, #0
003701ec: mov r5, r6
003701f0: mvn fp, #0
003701f4: mov r1, r5
003701f8: mov r0, r8
003701fc: bl #0x3ffe3c
00370200: cmp r0, #0
00370204: streq fp, [r4, r6]
00370208: beq #0x370214
0037020c: bl #0x3f9e00
00370210: str r0, [r4, r6]
00370214: add r5, r5, #1
00370218: cmp r7, r5
0037021c: add r6, r6, #4
00370220: bne #0x3701f4
00370224: add r0, sl, #0x3b0
00370228: mov r2, sb
0037022c: mov r1, r4
00370230: bl #0x36ffd0
00370234: mov r0, r4
00370238: bl #0x310440
0037023c: add sp, sp, #0x2c
00370240: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00370244: rsbeq r4, r2, r8, asr sl
00370248: strdeq r3, r4, [r0], -r4
0037024c: andeq r2, r0, r4, lsl #19
00370250: andeq r1, r0, r8, asr #1
00370254: andeq r1, r0, r8, lsr #1

# 0x370c9c _ZN13PlayerManager24ResetCharacterDeathTimerEv
00370c9c: mov r1, #0
00370ca0: push {r4, r5, r6, lr}
00370ca4: mov r2, r1
00370ca8: bl #0x36e478
00370cac: ldr r4, [pc, #0x34]
00370cb0: ldr r3, [pc, #0x34]
00370cb4: ldr r1, [pc, #0x34]
00370cb8: add r4, pc, r4
00370cbc: ldr r3, [r4, r3]
00370cc0: ldr r2, [pc, #0x2c]
00370cc4: mov r5, r0
00370cc8: add r1, pc, r1
00370ccc: ldr r0, [r3, #0x2c]
00370cd0: add r2, pc, r2
00370cd4: bl #0x4c4bdc
00370cd8: mov r1, r0
00370cdc: mov r0, r5
00370ce0: pop {r4, r5, r6, lr}
00370ce4: b #0x370bf4

# 0x370e00 _ZN13PlayerManager16ClearLoadingInfoEv
00370e00: push {r4, r5, r6, lr}
00370e04: mov r4, r0
00370e08: mov r5, #0
00370e0c: bl #0x7fd794
00370e10: mvn r3, #0
00370e14: str r3, [r4, #0x6cc]
00370e18: mov r1, r5
00370e1c: mov r2, r5
00370e20: strb r5, [r4, #0x6d0]
00370e24: strb r5, [r4, #0x6cb]
00370e28: strb r5, [r4, #0x710]
00370e2c: strb r5, [r4, #0x71a]
00370e30: mov r0, r4
00370e34: bl #0x36e478
00370e38: bl #0x370cf8
00370e3c: str r5, [r4, #0x714]
00370e40: strb r5, [r4, #0x711]
00370e44: pop {r4, r5, r6, pc}

# 0x371050 _ZN13PlayerManager29_AttachLightToPlayerCharacterEi
00371050: ldr r3, [pc, #0x80]
00371054: push {r4, r5, lr}
00371058: ldr r2, [pc, #0x7c]
0037105c: add r3, pc, r3
00371060: sub sp, sp, #0xc
00371064: ldr r2, [r3, r2]
00371068: mov r1, sp
0037106c: str sp, [sp]
00371070: ldr r0, [r2, #0x38]
00371074: str sp, [sp, #4]
00371078: mov r5, sp
0037107c: bl #0x34336c
00371080: ldr r4, [sp]
00371084: b #0x3710a0
00371088: ldr r3, [r4, #8]
0037108c: mov r0, r3
00371090: ldr r3, [r3]
00371094: mov lr, pc
00371098: ldr pc, [r3, #0x58]
0037109c: ldr r4, [r4]
003710a0: cmp r4, r5
003710a4: bne #0x371088
003710a8: ldr r0, [sp]
003710ac: cmp r0, r5
003710b0: bne #0x3710bc
003710b4: b #0x3710d0
003710b8: mov r0, r4
003710bc: ldr r4, [r0]
003710c0: mov r1, #0xc
003710c4: bl #0x708f00
003710c8: cmp r4, r5
003710cc: bne #0x3710b8
003710d0: add sp, sp, #0xc
003710d4: pop {r4, r5, pc}
003710d8: rsbeq r3, r2, r4, lsr sl
003710dc: strdeq r3, r4, [r0], -r4

# 0x3714ac _ZN13PlayerManagerD1Ev
003714ac: ldr r3, [pc, #0xc4]
003714b0: ldr r2, [pc, #0xc4]
003714b4: push {r4, r5, r6, lr}
003714b8: add r3, pc, r3
003714bc: ldr r2, [r3, r2]
003714c0: mov r4, r0
003714c4: add r2, r2, #8
003714c8: str r2, [r0], #0x6e0
003714cc: bl #0x3169c4
003714d0: ldr r0, [r4, #0x6b4]
003714d4: add r3, r4, #0x6b0
003714d8: add r3, r3, #4
003714dc: cmp r0, #0
003714e0: beq #0x3714fc
003714e4: ldr r1, [r3, #8]
003714e8: rsb r1, r0, r1
003714ec: bic r1, r1, #3
003714f0: cmp r1, #0x80
003714f4: bhi #0x371570
003714f8: bl #0x708f00
003714fc: ldr r0, [r4, #0x6a8]
00371500: add r3, r4, #0x6a0
00371504: add r3, r3, #8
00371508: cmp r0, #0
0037150c: beq #0x371528
00371510: ldr r1, [r3, #8]
00371514: rsb r1, r0, r1
00371518: bic r1, r1, #3
0037151c: cmp r1, #0x80
00371520: bhi #0x371568
00371524: bl #0x708f00
00371528: ldr r3, [r4, #0x6a0]
0037152c: cmp r3, #0
00371530: beq #0x371558
00371534: add r5, r4, #0x690
00371538: mov r0, r5
0037153c: ldr r1, [r4, #0x694]
00371540: bl #0x371470
00371544: mov r3, #0
00371548: str r5, [r4, #0x69c]
0037154c: str r3, [r4, #0x6a0]
00371550: str r5, [r4, #0x698]
00371554: str r3, [r4, #0x694]
00371558: add r0, r4, #8
0037155c: bl #0x371294
00371560: mov r0, r4
00371564: pop {r4, r5, r6, pc}
00371568: bl #0x310440
0037156c: b #0x371528
00371570: bl #0x310440
00371574: b #0x3714fc

# 0x371580 _ZN13PlayerManagerD0Ev
00371580: push {r4, lr}
00371584: mov r4, r0
00371588: bl #0x3714ac
0037158c: mov r0, r4
00371590: bl #0x310440
00371594: mov r0, r4
00371598: pop {r4, pc}

# 0x37159c _ZN13PlayerManagerD2Ev
0037159c: ldr r3, [pc, #0xc4]
003715a0: ldr r2, [pc, #0xc4]
003715a4: push {r4, r5, r6, lr}
003715a8: add r3, pc, r3
003715ac: ldr r2, [r3, r2]
003715b0: mov r4, r0
003715b4: add r2, r2, #8
003715b8: str r2, [r0], #0x6e0
003715bc: bl #0x3169c4
003715c0: ldr r0, [r4, #0x6b4]
003715c4: add r3, r4, #0x6b0
003715c8: add r3, r3, #4
003715cc: cmp r0, #0
003715d0: beq #0x3715ec
003715d4: ldr r1, [r3, #8]
003715d8: rsb r1, r0, r1
003715dc: bic r1, r1, #3
003715e0: cmp r1, #0x80
003715e4: bhi #0x371660
003715e8: bl #0x708f00
003715ec: ldr r0, [r4, #0x6a8]
003715f0: add r3, r4, #0x6a0
003715f4: add r3, r3, #8
003715f8: cmp r0, #0
003715fc: beq #0x371618
00371600: ldr r1, [r3, #8]
00371604: rsb r1, r0, r1
00371608: bic r1, r1, #3
0037160c: cmp r1, #0x80
00371610: bhi #0x371658
00371614: bl #0x708f00
00371618: ldr r3, [r4, #0x6a0]
0037161c: cmp r3, #0
00371620: beq #0x371648
00371624: add r5, r4, #0x690
00371628: mov r0, r5
0037162c: ldr r1, [r4, #0x694]
00371630: bl #0x371470
00371634: mov r3, #0
00371638: str r5, [r4, #0x69c]
0037163c: str r3, [r4, #0x6a0]
00371640: str r5, [r4, #0x698]
00371644: str r3, [r4, #0x694]
00371648: add r0, r4, #8
0037164c: bl #0x371294
00371650: mov r0, r4
00371654: pop {r4, r5, r6, pc}
00371658: bl #0x310440
0037165c: b #0x371618
00371660: bl #0x310440
00371664: b #0x3715ec
00371668: rsbeq r3, r2, r8, ror #9
0037166c: andeq r4, r0, ip, lsr #8

# 0x37193c _ZN13PlayerManager22_CheckOnlineTransitionEv
0037193c: push {r4, r5, r6, r7, r8, lr}
00371940: ldrb r6, [r0, #0x6ca]
00371944: ldr r5, [pc, #0x194]
00371948: mov r4, r0
0037194c: cmp r6, #0
00371950: add r5, pc, r5
00371954: beq #0x371998
00371958: bl #0x7fd794
0037195c: ldrb r3, [r0, #5]
00371960: cmp r3, #0
00371964: bne #0x371a1c
00371968: bl #0x7fd794
0037196c: ldrb r3, [r0, #5]
00371970: cmp r3, #0
00371974: bne #0x371a6c
00371978: mov r3, #0
0037197c: mov r0, r4
00371980: strb r3, [r4, #0x719]
00371984: strb r3, [r4, #0x6ca]
00371988: bl #0x370e00
0037198c: mov r0, r4
00371990: pop {r4, r5, r6, r7, r8, lr}
00371994: b #0x36d834
00371998: bl #0x7fd794
0037199c: ldrb r3, [r0, #5]
003719a0: cmp r3, #0
003719a4: beq #0x371958
003719a8: bl #0x320e98
003719ac: ldrb r3, [r0, #0x24]
003719b0: cmp r3, #0
003719b4: beq #0x371958
003719b8: bl #0x800f8c
003719bc: ldr r3, [r0]
003719c0: mov lr, pc
003719c4: ldr pc, [r3, #0x64]
003719c8: cmp r0, #0
003719cc: beq #0x371958
003719d0: bl #0x8100dc
003719d4: bl #0x8100e0
003719d8: cmp r0, #0
003719dc: beq #0x371958
003719e0: ldr r3, [r4, #0x6a0]
003719e4: cmp r3, #0
003719e8: beq #0x371958
003719ec: add r7, r4, #0x690
003719f0: mov r0, r7
003719f4: ldr r1, [r4, #0x694]
003719f8: bl #0x371470
003719fc: str r7, [r4, #0x69c]
00371a00: str r6, [r4, #0x6a0]
00371a04: str r7, [r4, #0x698]
00371a08: str r6, [r4, #0x694]
00371a0c: bl #0x7fd794
00371a10: ldrb r3, [r0, #5]
00371a14: cmp r3, #0
00371a18: beq #0x371968
00371a1c: bl #0x320e98
00371a20: ldr r3, [r0, #0x34]
00371a24: sub r3, r3, #3
00371a28: cmp r3, #1
00371a2c: bhi #0x371968
00371a30: ldr r3, [pc, #0xac]
00371a34: ldr r5, [r5, r3]
00371a38: mov r0, r5
00371a3c: bl #0x31f594
00371a40: cmp r0, #0
00371a44: beq #0x371968
00371a48: bl #0x7fbd74
00371a4c: mov r1, #1
00371a50: bl #0x7fbe94
00371a54: cmp r0, #0
00371a58: bne #0x371968
00371a5c: mov r0, r5
00371a60: mov r1, #3
00371a64: bl #0x32c1f4
00371a68: b #0x371968
00371a6c: bl #0x320e98
00371a70: ldrb r3, [r0, #0x24]
00371a74: cmp r3, #0
00371a78: beq #0x371978
00371a7c: bl #0x800f8c
00371a80: ldr r3, [r0]
00371a84: mov lr, pc
00371a88: ldr pc, [r3, #0x64]
00371a8c: cmp r0, #0
00371a90: beq #0x371978
00371a94: bl #0x8100dc
00371a98: bl #0x8100e0
00371a9c: cmp r0, #0
00371aa0: beq #0x371978
00371aa4: mov r3, #1
00371aa8: strb r3, [r4, #0x6ca]
00371aac: bl #0x8100dc
00371ab0: bl #0x8113fc
00371ab4: mov r1, r0
00371ab8: add r0, r4, #0x6a0
00371abc: add r0, r0, #8
00371ac0: bl #0x371820
00371ac4: bl #0x8100dc
00371ac8: bl #0x812ea8
00371acc: mov r1, r0
00371ad0: add r0, r4, #0x6b0
00371ad4: add r0, r0, #4
00371ad8: pop {r4, r5, r6, r7, r8, lr}
00371adc: b #0x371820
00371ae0: rsbeq r3, r2, r0, asr #2
00371ae4: strdeq r3, r4, [r0], -r4

# 0x371d80 _ZN13PlayerManager15RemoveCharacterEP9Character
00371d80: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00371d84: ldr r4, [pc, #0x3ec]
00371d88: ldr r6, [pc, #0x3ec]
00371d8c: sub sp, sp, #0x264
00371d90: add r4, pc, r4
00371d94: ldr r3, [r4, r6]
00371d98: subs r5, r1, #0
00371d9c: mov r8, r0
00371da0: ldr r3, [r3]
00371da4: str r3, [sp, #0x25c]
00371da8: beq #0x3720c8
00371dac: ldrb sl, [r5, #0x81]
00371db0: cmp sl, #0
00371db4: bne #0x3720c8
00371db8: mov r2, #1
00371dbc: bl #0x36eea8
00371dc0: ldr r2, [r8, #0x6c4]
00371dc4: ldr fp, [r0, #0x670]
00371dc8: mov r7, r0
00371dcc: cmn fp, #1
00371dd0: movne fp, #0
00371dd4: moveq fp, #1
00371dd8: cmp r2, #0
00371ddc: ble #0x3720e4
00371de0: sub r2, r2, #1
00371de4: mov r3, #0
00371de8: str r2, [r8, #0x6c4]
00371dec: add r1, sp, #0x34
00371df0: mov r2, #1
00371df4: mov r0, r5
00371df8: str r3, [sp, #0x3c]
00371dfc: str r3, [sp, #0x34]
00371e00: str r3, [sp, #0x38]
00371e04: bl #0x393db4
00371e08: mov r0, r5
00371e0c: ldr r3, [r5]
00371e10: mov lr, pc
00371e14: ldr pc, [r3, #0x48]
00371e18: ldr r3, [r5]
00371e1c: mov r1, #0
00371e20: mov r0, r5
00371e24: mov lr, pc
00371e28: ldr pc, [r3, #0x40]
00371e2c: mov r0, r5
00371e30: bl #0x3b4bc4
00371e34: mov r0, r5
00371e38: bl #0x3a4068
00371e3c: add sl, r5, #0x3c8
00371e40: movw r3, #0x14a4
00371e44: mov sb, #0
00371e48: str sb, [r5, r3]
00371e4c: mov r2, sb
00371e50: mov r1, sb
00371e54: mov r0, sl
00371e58: bl #0x3d6890
00371e5c: mov r0, sl
00371e60: bl #0x3d49c4
00371e64: mov r0, sl
00371e68: bl #0x3d5fa8
00371e6c: mov r1, sb
00371e70: mov r0, sl
00371e74: bl #0x3d6abc
00371e78: mov r0, r5
00371e7c: bl #0x33ddb4
00371e80: ldr r3, [pc, #0x2f8]
00371e84: ldr r1, [pc, #0x2f8]
00371e88: add sb, sp, #0x44
00371e8c: ldr sl, [r4, r3]
00371e90: ldr r2, [r5, #0x44]
00371e94: add r1, pc, r1
00371e98: ldr r3, [sl]
00371e9c: mov r0, sb
00371ea0: bl #0x30eae4
00371ea4: ldr r3, [sl]
00371ea8: mov r0, r5
00371eac: mov r1, sb
00371eb0: add r3, r3, #1
00371eb4: str r3, [sl]
00371eb8: bl #0x34ac18
00371ebc: ldr r3, [pc, #0x2c4]
00371ec0: ldr r0, [r4, r3]
00371ec4: bl #0x31f594
00371ec8: subs r5, r0, #0
00371ecc: beq #0x37216c
00371ed0: cmp fp, #0
00371ed4: bne #0x372090
00371ed8: ldrb r3, [r7, #0x66c]
00371edc: cmp r3, #0
00371ee0: beq #0x371f54
00371ee4: ldr r3, [r8, #0x6c4]
00371ee8: cmp r3, #0
00371eec: ble #0x371f44
00371ef0: mov r0, r8
00371ef4: mov r1, fp
00371ef8: mov r2, #1
00371efc: bl #0x36e478
00371f00: ldr sl, [r0, #0x660]
00371f04: cmp sl, #0
00371f08: beq #0x372148
00371f0c: ldr r0, [r5, #0x128]
00371f10: cmp r0, #0
00371f14: beq #0x371f38
00371f18: mov r1, sl
00371f1c: mov r2, #0
00371f20: bl #0x4119c4
00371f24: ldr r3, [r5, #0x128]
00371f28: mov r0, r3
00371f2c: ldr r3, [r3]
00371f30: mov lr, pc
00371f34: ldr pc, [r3, #0x10]
00371f38: ldr r3, [r8, #0x6c4]
00371f3c: cmp r3, #1
00371f40: beq #0x37213c
00371f44: mov r0, r5
00371f48: mov r1, #1
00371f4c: mov r2, #4
00371f50: bl #0x3ef280
00371f54: ldrb r3, [r8, #0x6c9]
00371f58: ldr r1, [pc, #0x22c]
00371f5c: mov r2, #0xb6000000
00371f60: cmp r3, #0
00371f64: streq r3, [r7, #0x660]
00371f68: mov r3, #0
00371f6c: str r3, [r7, #0x684]
00371f70: ldr r0, [sp, #0x28]
00371f74: ldr r1, [r4, r1]
00371f78: asr r2, r2, #0x15
00371f7c: mov r8, #0
00371f80: mov sb, #0
00371f84: add ip, sp, #0x260
00371f88: cmp r0, r3
00371f8c: add r1, r1, #8
00371f90: mvn r0, #0
00371f94: strd r8, sb, [ip, r2]
00371f98: mov r2, #0x20
00371f9c: str r2, [sp, #0xc]
00371fa0: str r0, [sp, #0x1c]
00371fa4: str r1, [sp, #8]
00371fa8: str r0, [sp, #0x18]
00371fac: str r3, [sp, #0x20]
00371fb0: strb r3, [sp, #0x24]
00371fb4: addeq r5, sp, #8
00371fb8: beq #0x371fcc
00371fbc: add r5, sp, #8
00371fc0: mov r0, r5
00371fc4: str r3, [sp, #0x28]
00371fc8: bl #0x814f84
00371fcc: ldr r3, [pc, #0x1bc]
00371fd0: add r1, r5, #0x20
00371fd4: add r0, r7, #0x4a0
00371fd8: ldr r3, [r4, r3]
00371fdc: add r3, r3, #8
00371fe0: str r3, [sp, #8]
00371fe4: ldr r3, [r7, #0x4a0]
00371fe8: mov lr, pc
00371fec: ldr pc, [r3, #0x1c]
00371ff0: ldr r3, [pc, #0x19c]
00371ff4: mov r0, r7
00371ff8: ldr r3, [r4, r3]
00371ffc: add r3, r3, #8
00372000: str r3, [sp, #8]
00372004: ldr r3, [r7]
00372008: mov lr, pc
0037200c: ldr pc, [r3, #0x50]
00372010: cmp r0, #0
00372014: beq #0x372090
00372018: mov r0, r7
0037201c: mvn r1, #0
00372020: bl #0x370ef0
00372024: ldr r1, [pc, #0x16c]
00372028: add r5, sp, #0x244
0037202c: add r2, sp, #0x40
00372030: add r1, pc, r1
00372034: mov r0, r5
00372038: bl #0x3140ec
0037203c: mov r1, r5
00372040: mov r0, r7
00372044: bl #0x371ccc
00372048: mov r0, r5
0037204c: bl #0x318254
00372050: mov r0, r7
00372054: mvn r1, #0
00372058: bl #0x370e48
0037205c: mov r1, #0
00372060: mov r2, r1
00372064: add r0, r7, #0x3b0
00372068: bl #0x36ffd0
0037206c: mov r0, r7
00372070: mov r1, #0
00372074: bl #0x36fd58
00372078: mov r0, r7
0037207c: mov r1, #0
00372080: bl #0x36fe04
00372084: mov r0, r7
00372088: mov r1, #0
0037208c: bl #0x370a9c
00372090: bl #0x42ca8c
00372094: bl #0x42cb8c
00372098: cmp r0, #0
0037209c: beq #0x3720c8
003720a0: bl #0x42ca8c
003720a4: bl #0x42cb8c
003720a8: ldr r1, [pc, #0xec]
003720ac: ldr r2, [pc, #0xec]
003720b0: mov ip, #0
003720b4: add r1, pc, r1
003720b8: add r2, pc, r2
003720bc: mov r3, ip
003720c0: str ip, [sp]
003720c4: bl #0x7ad7e8
003720c8: ldr r3, [r4, r6]
003720cc: ldr r2, [sp, #0x25c]
003720d0: ldr r3, [r3]
003720d4: cmp r2, r3
003720d8: bne #0x372168
003720dc: add sp, sp, #0x264
003720e0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003720e4: ldr r3, [pc, #0xb8]
003720e8: ldr r3, [r4, r3]
003720ec: ldr r3, [r3]
003720f0: cmp r3, #2
003720f4: streq sl, [sl]
003720f8: beq #0x371de0
003720fc: cmp r3, #1
00372100: bne #0x371de0
00372104: ldr r0, [pc, #0x9c]
00372108: ldr r1, [pc, #0x9c]
0037210c: ldr r2, [pc, #0x9c]
00372110: ldr r0, [r4, r0]
00372114: ldr r3, [pc, #0x98]
00372118: add r2, pc, r2
0037211c: movw ip, #0x4df
00372120: add r1, pc, r1
00372124: add r0, r0, #0xa8
00372128: add r3, pc, r3
0037212c: str ip, [sp]
00372130: bl #0x30e004
00372134: ldr r2, [r8, #0x6c4]
00372138: b #0x371de0
0037213c: mov r0, sl
00372140: bl #0x3b4bc4
00372144: b #0x371f44
00372148: mov r1, fp
0037214c: mov r0, r8
00372150: mov r2, fp
00372154: bl #0x36e744
00372158: ldr sl, [r0, #0x660]
0037215c: cmp sl, #0
00372160: beq #0x371f44
00372164: b #0x371f0c
00372168: bl #0x30e310
0037216c: cmp fp, #0
00372170: bne #0x372090
00372174: b #0x371f54
00372178: rsbeq r2, r2, r0, lsl #26
0037217c: andeq r4, r0, ip, lsr #1
00372180: andeq r2, r0, ip, lsr #26
00372184: subseq pc, r4, r4, lsl #18
00372188: strdeq r3, r4, [r0], -r4
0037218c: andeq r2, r0, r4, lsl #19
00372190: andeq r1, r0, r8, asr #1
00372194: andeq r1, r0, r8, lsr #1
00372198: ldrsbeq sb, [r5], #-0x78
0037219c: ldrsheq pc, [r4], #-0x64
003721a0: subseq pc, r4, r0, lsl #14
003721a4: andeq r3, r0, r0, asr #19
003721a8: andeq r1, r0, r0, asr #19
003721ac: ldrheq ip, [r4], #-0x28
003721b0: subseq pc, r4, r0, ror #12
003721b4: subseq pc, r4, r8, asr #11

# 0x3721b8 _ZN13PlayerManager19RemoveAllCharactersEv
003721b8: push {r4, r5, r6, lr}
003721bc: mov r5, r0
003721c0: mov r4, #0
003721c4: mov r0, r5
003721c8: bl #0x36d7a8
003721cc: cmp r4, r0
003721d0: mov r1, r4
003721d4: mov r2, #0
003721d8: mov r0, r5
003721dc: bge #0x372218
003721e0: bl #0x36e744
003721e4: ldr r1, [r0, #0x660]
003721e8: add r4, r4, #1
003721ec: mov r0, r5
003721f0: cmp r1, #0
003721f4: beq #0x3721c4
003721f8: bl #0x371d80
003721fc: mov r0, r5
00372200: bl #0x36d7a8
00372204: cmp r4, r0
00372208: mov r1, r4
0037220c: mov r2, #0
00372210: mov r0, r5
00372214: blt #0x3721e0
00372218: str r2, [r5, #0x6c4]
0037221c: pop {r4, r5, r6, pc}

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

# 0x3726bc _ZN13PlayerManager12RemovePlayerEi
003726bc: push {r4, r5, r6, r7, lr}
003726c0: sub sp, sp, #0xc
003726c4: mov r5, r0
003726c8: mov r4, r1
003726cc: bl #0x7fd794
003726d0: ldrb r3, [r0, #5]
003726d4: cmp r3, #0
003726d8: bne #0x37277c
003726dc: mov r0, r5
003726e0: mov r1, r4
003726e4: bl #0x36d280
003726e8: cmp r0, #0
003726ec: beq #0x372774
003726f0: ldr r3, [r5, #0x694]
003726f4: add r6, r5, #0x690
003726f8: mov r7, r6
003726fc: cmp r3, #0
00372700: movne r1, r6
00372704: bne #0x372710
00372708: b #0x3727b8
0037270c: mov r3, r2
00372710: ldr r2, [r3, #0x10]
00372714: cmp r2, r4
00372718: ldrlt r2, [r3, #0xc]
0037271c: ldrge r2, [r3, #8]
00372720: movlt r3, r1
00372724: mov r1, r3
00372728: cmp r2, #0
0037272c: bne #0x37270c
00372730: cmp r6, r3
00372734: beq #0x3727b8
00372738: ldr r2, [r3, #0x10]
0037273c: cmp r2, r4
00372740: movgt r2, r6
00372744: movle r2, r3
00372748: movgt r7, r6
0037274c: movle r7, r3
00372750: ldr r1, [r2, #0x678]
00372754: mov r0, r5
00372758: bl #0x371d80
0037275c: add r1, sp, #8
00372760: str r7, [r1, #-4]!
00372764: mov r0, r6
00372768: bl #0x371428
0037276c: mov r0, r5
00372770: bl #0x36ed0c
00372774: add sp, sp, #0xc
00372778: pop {r4, r5, r6, r7, pc}
0037277c: bl #0x320e98
00372780: ldrb r3, [r0, #0x24]
00372784: cmp r3, #0
00372788: beq #0x3726dc
0037278c: bl #0x800f8c
00372790: ldr r3, [r0]
00372794: mov lr, pc
00372798: ldr pc, [r3, #0x64]
0037279c: cmp r0, #0
003727a0: beq #0x3726dc
003727a4: bl #0x8100dc
003727a8: bl #0x8100e0
003727ac: cmp r0, #0
003727b0: bne #0x37276c
003727b4: b #0x3726dc
003727b8: mov r2, r6
003727bc: b #0x372750

# 0x3727c0 _ZN13PlayerManager16RemoveAllPlayersEv
003727c0: push {r4, r5, r6, lr}
003727c4: mov r5, r0
003727c8: bl #0x36d7a8
003727cc: subs r6, r0, #0
003727d0: ble #0x372800
003727d4: mov r4, #0
003727d8: mov r1, #0
003727dc: mov r2, r1
003727e0: mov r0, r5
003727e4: bl #0x36e5b4
003727e8: add r4, r4, #1
003727ec: mov r1, r0
003727f0: mov r0, r5
003727f4: bl #0x3726bc
003727f8: cmp r4, r6
003727fc: bne #0x3727d8
00372800: mov r3, #0
00372804: str r3, [r5, #0x6c4]
00372808: pop {r4, r5, r6, pc}

# 0x37280c _ZN13PlayerManager17_ManageCharactersEv
0037280c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00372810: ldr r8, [pc, #0xeac]
00372814: ldr r1, [pc, #0xeac]
00372818: ldr r2, [pc, #0xeac]
0037281c: add r8, pc, r8
00372820: ldr r3, [r8, r1]
00372824: sub sp, sp, #0x1d4
00372828: mov r7, r0
0037282c: ldr r3, [r3]
00372830: ldr r0, [r8, r2]
00372834: str r1, [sp, #0x1c]
00372838: str r2, [sp, #0x18]
0037283c: str r3, [sp, #0x1cc]
00372840: bl #0x31f594
00372844: str r0, [sp, #0x24]
00372848: mov r0, r7
0037284c: bl #0x36d7a8
00372850: subs fp, r0, #0
00372854: ble #0x373a94
00372858: ldr r3, [pc, #0xe70]
0037285c: ldr lr, [pc, #0xe70]
00372860: mov r0, #0
00372864: add r3, pc, r3
00372868: str r3, [sp, #0x30]
0037286c: ldr r3, [pc, #0xe64]
00372870: mov r1, #1
00372874: str lr, [sp, #0x2c]
00372878: add r3, pc, r3
0037287c: str r3, [sp, #0x34]
00372880: ldr r3, [pc, #0xe54]
00372884: str r0, [sp, #0x20]
00372888: mov r6, r0
0037288c: add r3, pc, r3
00372890: str r3, [sp, #0x38]
00372894: ldr r3, [pc, #0xe44]
00372898: str r0, [sp, #0xc]
0037289c: str r1, [sp, #0x10]
003728a0: add r3, pc, r3
003728a4: str r3, [sp, #0x3c]
003728a8: mov r1, r6
003728ac: mov r2, #0
003728b0: mov r0, r7
003728b4: bl #0x36e744
003728b8: ldr r3, [r0]
003728bc: mov r4, r0
003728c0: ldr r5, [r0, #0x660]
003728c4: mov lr, pc
003728c8: ldr pc, [r3, #0x5c]
003728cc: cmp r0, #0
003728d0: beq #0x372ec0
003728d4: ldrb r3, [r4, #0x66c]
003728d8: cmp r3, #0
003728dc: beq #0x372f50
003728e0: ldr sl, [r4, #0x664]
003728e4: cmn sl, #1
003728e8: beq #0x372b6c
003728ec: ldr sb, [r4, #0x680]
003728f0: cmp sb, #0
003728f4: beq #0x3739d8
003728f8: bl #0x32bd08
003728fc: ldrb r3, [r0, #0x11]
00372900: cmp r3, #0
00372904: beq #0x372f8c
00372908: add r2, sp, #0x1b4
0037290c: mov r0, r2
00372910: add r1, r4, #0x2d0
00372914: str r2, [sp, #0x14]
00372918: bl #0x32b918
0037291c: bl #0x32bd08
00372920: add sl, sp, #0x19c
00372924: mov r1, r0
00372928: mov r0, sl
0037292c: bl #0x4995f4
00372930: ldr r0, [sp, #0x1c8]
00372934: ldr r1, [sp, #0x1b0]
00372938: ldr r2, [sp, #0x1c4]
0037293c: ldr r3, [sp, #0x1ac]
00372940: rsb r2, r0, r2
00372944: rsb r3, r1, r3
00372948: cmp r2, r3
0037294c: beq #0x373618
00372950: mov r0, sl
00372954: bl #0x318254
00372958: ldr r0, [sp, #0x14]
0037295c: bl #0x318254
00372960: bl #0x32bd08
00372964: add sl, sp, #0x184
00372968: mov r1, r0
0037296c: mov r0, sl
00372970: bl #0x4995f4
00372974: mov r0, r4
00372978: mov r1, sl
0037297c: bl #0x371ccc
00372980: mov r0, sl
00372984: bl #0x318254
00372988: cmp r5, #0
0037298c: beq #0x373028
00372990: ldr ip, [sp, #0x18]
00372994: ldr r0, [r8, ip]
00372998: bl #0x31f594
0037299c: subs sl, r0, #0
003729a0: beq #0x3729b4
003729a4: ldr r3, [sl, #0x130]
003729a8: cmp r3, #0x26
003729ac: ldrbeq r3, [sl, #0x144]
003729b0: beq #0x3729b8
003729b4: mov r3, #0
003729b8: ldrb r2, [r4, #0x4e5]
003729bc: cmp r2, r3
003729c0: beq #0x372a78
003729c4: ldr ip, [pc, #0xd18]
003729c8: ldrb r1, [sp, #0xa5]
003729cc: mov r2, #0xb0000000
003729d0: ldr ip, [r8, ip]
003729d4: cmp r1, r3
003729d8: asr r2, r2, #0x16
003729dc: mov r0, #0
003729e0: mov r1, #0
003729e4: add lr, sp, #0x1d0
003729e8: mvn sb, #0
003729ec: strd r0, r1, [lr, r2]
003729f0: add ip, ip, #8
003729f4: mov r2, #1
003729f8: mov r0, #0
003729fc: mov r1, #0
00372a00: str sb, [sp, #0x9c]
00372a04: str sb, [sp, #0x98]
00372a08: str r2, [sp, #0x8c]
00372a0c: strb r0, [sp, #0xa4]
00372a10: str ip, [sp, #0x88]
00372a14: str r1, [sp, #0xa0]
00372a18: addeq sb, sp, #0x88
00372a1c: beq #0x372a30
00372a20: add sb, sp, #0x88
00372a24: mov r0, sb
00372a28: strb r3, [sp, #0xa5]
00372a2c: bl #0x814f84
00372a30: ldr r3, [pc, #0xcb0]
00372a34: add r0, r4, #0x4c0
00372a38: add r1, sb, #0x1d
00372a3c: ldr r3, [r8, r3]
00372a40: add r0, r0, #8
00372a44: add r3, r3, #8
00372a48: str r3, [sp, #0x88]
00372a4c: ldr r3, [r4, #0x4c8]
00372a50: mov lr, pc
00372a54: ldr pc, [r3, #0x1c]
00372a58: ldr r3, [pc, #0xc8c]
00372a5c: ldr r2, [pc, #0xc8c]
00372a60: mov r1, #0x7d0
00372a64: ldr r3, [r8, r3]
00372a68: add r2, pc, r2
00372a6c: str r1, [r2]
00372a70: add r3, r3, #8
00372a74: str r3, [sp, #0x88]
00372a78: cmp sl, #0
00372a7c: beq #0x372a90
00372a80: ldr sl, [sl, #0x130]
00372a84: cmp sl, #0x23
00372a88: movle sl, #0
00372a8c: movgt sl, #1
00372a90: ldrb r3, [r4, #0x525]
00372a94: cmp r3, sl
00372a98: beq #0x372b4c
00372a9c: bl #0x7fd794
00372aa0: ldrb r3, [r0, #5]
00372aa4: cmp r3, #0
00372aa8: bne #0x3735e4
00372aac: ldr r0, [pc, #0xc30]
00372ab0: ldrb r2, [sp, #0x85]
00372ab4: mov r3, #0xa8000000
00372ab8: ldr r0, [r8, r0]
00372abc: asr r3, r3, #0x16
00372ac0: mov r1, #0
00372ac4: add sb, r0, #8
00372ac8: add ip, sp, #0x1d0
00372acc: mov r0, #0
00372ad0: cmp r2, sl
00372ad4: mvn lr, #0
00372ad8: mov r2, #0
00372adc: strd r0, r1, [ip, r3]
00372ae0: mov r3, #1
00372ae4: str sb, [sp, #0x68]
00372ae8: str r3, [sp, #0x6c]
00372aec: str lr, [sp, #0x7c]
00372af0: strb r2, [sp, #0x84]
00372af4: str lr, [sp, #0x78]
00372af8: str r2, [sp, #0x80]
00372afc: addeq sb, sp, #0x68
00372b00: beq #0x372b14
00372b04: add sb, sp, #0x68
00372b08: mov r0, sb
00372b0c: strb sl, [sp, #0x85]
00372b10: bl #0x814f84
00372b14: ldr r3, [pc, #0xbcc]
00372b18: add r0, r4, #0x500
00372b1c: add r0, r0, #8
00372b20: ldr r3, [r8, r3]
00372b24: add r1, sb, #0x1d
00372b28: add r3, r3, #8
00372b2c: str r3, [sp, #0x68]
00372b30: ldr r3, [r4, #0x508]
00372b34: mov lr, pc
00372b38: ldr pc, [r3, #0x1c]
00372b3c: ldr r3, [pc, #0xba8]
00372b40: ldr r3, [r8, r3]
00372b44: add r3, r3, #8
00372b48: str r3, [sp, #0x68]
00372b4c: bl #0x7fd794
00372b50: ldrb r3, [r0, #5]
00372b54: cmp r3, #0
00372b58: bne #0x3730b4
00372b5c: bl #0x7fd794
00372b60: ldrb r3, [r0, #5]
00372b64: cmp r3, #0
00372b68: bne #0x373120
00372b6c: ldrb r3, [r7, #0x6c9]
00372b70: cmp r3, #0
00372b74: beq #0x372ea0
00372b78: cmp r5, #0
00372b7c: beq #0x37305c
00372b80: bl #0x7fd794
00372b84: ldrb r3, [r0, #5]
00372b88: cmp r3, #0
00372b8c: beq #0x372ea0
00372b90: ldrb r3, [r4, #0x66c]
00372b94: cmp r3, #0
00372b98: beq #0x3731cc
00372b9c: mov r1, r4
00372ba0: mov r0, r7
00372ba4: bl #0x370020
00372ba8: ldr r1, [r5, #0x39c]
00372bac: ldr r3, [r4, #0x498]
00372bb0: cmp r3, r1
00372bb4: beq #0x372bc0
00372bb8: mov r0, r4
00372bbc: bl #0x36fe04
00372bc0: add sl, r5, #0x37c
00372bc4: mov r0, sl
00372bc8: ldr sb, [r4, #0x358]
00372bcc: bl #0x3fc690
00372bd0: cmp r0, sb
00372bd4: beq #0x372bec
00372bd8: mov r0, sl
00372bdc: bl #0x3fc690
00372be0: mov r1, r0
00372be4: mov r0, r4
00372be8: bl #0x36fcb0
00372bec: add sb, r5, #0x560
00372bf0: mov r0, sb
00372bf4: mov r1, #0x21
00372bf8: mov r2, #0
00372bfc: ldr sl, [r4, #0x448]
00372c00: bl #0x3df6e0
00372c04: cmp r0, sl
00372c08: beq #0x372c28
00372c0c: mov r1, #0x21
00372c10: mov r0, sb
00372c14: mov r2, #0
00372c18: bl #0x3df6e0
00372c1c: mov r1, r0
00372c20: mov r0, r4
00372c24: bl #0x36fd58
00372c28: mov r0, sb
00372c2c: mov r1, #0x24
00372c30: mov r2, #0
00372c34: ldr sl, [r4, #0x470]
00372c38: bl #0x3df6e0
00372c3c: cmp r0, sl
00372c40: beq #0x372cf8
00372c44: mov r1, #0x24
00372c48: mov r2, #0
00372c4c: mov r0, sb
00372c50: bl #0x3df6e0
00372c54: ldr ip, [pc, #0xa98]
00372c58: ldr r1, [sp, #0x60]
00372c5c: mov r2, #0x9e000000
00372c60: ldr ip, [r8, ip]
00372c64: cmp r0, r1
00372c68: asr r2, r2, #0x16
00372c6c: mov r1, #0
00372c70: mov r3, r0
00372c74: add lr, sp, #0x1d0
00372c78: mov r0, #0
00372c7c: mvn sl, #0
00372c80: strd r0, r1, [lr, r2]
00372c84: add ip, ip, #8
00372c88: mov r2, #0x20
00372c8c: mov r0, #0
00372c90: mov r1, #0
00372c94: str sl, [sp, #0x54]
00372c98: str sl, [sp, #0x50]
00372c9c: str r2, [sp, #0x44]
00372ca0: strb r0, [sp, #0x5c]
00372ca4: str ip, [sp, #0x40]
00372ca8: str r1, [sp, #0x58]
00372cac: addeq sl, sp, #0x40
00372cb0: beq #0x372cc4
00372cb4: add sl, sp, #0x40
00372cb8: mov r0, sl
00372cbc: str r3, [sp, #0x60]
00372cc0: bl #0x814f84
00372cc4: ldr r3, [pc, #0xa2c]
00372cc8: add r1, sl, #0x20
00372ccc: add r0, r4, #0x450
00372cd0: ldr r3, [r8, r3]
00372cd4: add r3, r3, #8
00372cd8: str r3, [sp, #0x40]
00372cdc: ldr r3, [r4, #0x450]
00372ce0: mov lr, pc
00372ce4: ldr pc, [r3, #0x1c]
00372ce8: ldr r3, [pc, #0x9fc]
00372cec: ldr r3, [r8, r3]
00372cf0: add r3, r3, #8
00372cf4: str r3, [sp, #0x40]
00372cf8: mov r0, sb
00372cfc: mov r1, #0x13
00372d00: mov r2, #0
00372d04: ldr sl, [r4, #0x330]
00372d08: bl #0x3df6e0
00372d0c: cmp r0, sl
00372d10: beq #0x372d30
00372d14: mov r1, #0x13
00372d18: mov r0, sb
00372d1c: mov r2, #0
00372d20: bl #0x3df6e0
00372d24: mov r1, r0
00372d28: mov r0, r4
00372d2c: bl #0x370e48
00372d30: mov r0, r5
00372d34: ldr sl, [r4, #0x380]
00372d38: bl #0x3bb7fc
00372d3c: cmp r0, sl
00372d40: beq #0x372d58
00372d44: mov r0, r5
00372d48: bl #0x3bb7fc
00372d4c: mov r1, r0
00372d50: mov r0, r4
00372d54: bl #0x370ef0
00372d58: mov r1, #0
00372d5c: mov r0, r5
00372d60: bl #0x3bbe68
00372d64: mov r1, #1
00372d68: strb r0, [sp, #0xc4]
00372d6c: mov r0, r5
00372d70: bl #0x3bbe68
00372d74: mov r1, #2
00372d78: strb r0, [sp, #0xc5]
00372d7c: mov r0, r5
00372d80: bl #0x3bbe68
00372d84: movw sl, #0x14e8
00372d88: strb r0, [sp, #0xc6]
00372d8c: add r1, sp, #0xc4
00372d90: add r0, r4, #0x3d8
00372d94: mov r2, #3
00372d98: bl #0x370258
00372d9c: ldr r3, [r5, sl]
00372da0: cmp r3, #0
00372da4: beq #0x372dd8
00372da8: ldr r2, [r3, #0x84]
00372dac: cmp r2, #0x1e
00372db0: bls #0x372dd8
00372db4: ldr r2, [pc, #0x964]
00372db8: ldr r2, [r8, r2]
00372dbc: ldr r2, [r2]
00372dc0: cmp r2, #2
00372dc4: moveq r2, #0
00372dc8: streq r2, [r2]
00372dcc: beq #0x372dd8
00372dd0: cmp r2, #1
00372dd4: beq #0x373ad0
00372dd8: add r2, sp, #0xd4
00372ddc: str r4, [sp, #0x28]
00372de0: mov sl, #0
00372de4: str r2, [sp, #0x14]
00372de8: movw sb, #0x14e8
00372dec: mov r4, r2
00372df0: b #0x372e04
00372df4: add sl, sl, #1
00372df8: cmp sl, #0x1e
00372dfc: beq #0x372e50
00372e00: ldr r3, [r5, sb]
00372e04: cmp r3, #0
00372e08: beq #0x372df4
00372e0c: ldr r3, [r3, #0x84]
00372e10: cmp r3, sl
00372e14: bls #0x372df4
00372e18: mov r0, r5
00372e1c: mov r1, sl
00372e20: bl #0x3bbed0
00372e24: cmp r0, #0
00372e28: mvnlt r3, #0
00372e2c: strblt r3, [r4, sl]
00372e30: blt #0x372df4
00372e34: mov r1, sl
00372e38: mov r0, r5
00372e3c: bl #0x3bbed0
00372e40: strb r0, [r4, sl]
00372e44: add sl, sl, #1
00372e48: cmp sl, #0x1e
00372e4c: bne #0x372e00
00372e50: ldr r4, [sp, #0x28]
00372e54: mov r2, sl
00372e58: ldr r1, [sp, #0x14]
00372e5c: add r0, r4, #0x400
00372e60: bl #0x3702a8
00372e64: add sl, r4, #0x4c0
00372e68: add sl, sl, #8
00372e6c: mov r0, sl
00372e70: bl #0x814f70
00372e74: cmp r0, #0
00372e78: beq #0x372ea0
00372e7c: ldr r3, [r5]
00372e80: mov r0, r5
00372e84: ldrb r1, [r4, #0x4e5]
00372e88: mov lr, pc
00372e8c: ldr pc, [r3, #0x40]
00372e90: ldr r2, [sp, #0x24]
00372e94: ldr r3, [r2, #0x130]
00372e98: cmp r3, #0x26
00372e9c: beq #0x3738ec
00372ea0: ldrb r3, [r4, #0x4e5]
00372ea4: ldr r1, [sp, #0x10]
00372ea8: ldr r2, [sp, #0xc]
00372eac: cmp r3, #0
00372eb0: moveq r1, r3
00372eb4: movne r2, #1
00372eb8: str r1, [sp, #0x10]
00372ebc: str r2, [sp, #0xc]
00372ec0: add r6, r6, #1
00372ec4: cmp r6, fp
00372ec8: bne #0x3728a8
00372ecc: bl #0x7fd794
00372ed0: ldrb r3, [r0, #5]
00372ed4: cmp r3, #0
00372ed8: bne #0x373734
00372edc: bl #0x7fd794
00372ee0: ldrb r3, [r0, #5]
00372ee4: cmp r3, #0
00372ee8: bne #0x3732a8
00372eec: mov r4, #0
00372ef0: bl #0x7fd794
00372ef4: ldrb r3, [r0, #5]
00372ef8: cmp r3, #0
00372efc: bne #0x373638
00372f00: cmp r4, #0
00372f04: bne #0x373698
00372f08: ldrb r3, [r7, #0x6c9]
00372f0c: cmp r3, #0
00372f10: bne #0x372f30
00372f14: ldr r3, [r7, #0x6c4]
00372f18: cmp r3, #0
00372f1c: ble #0x372f30
00372f20: mov r0, r7
00372f24: bl #0x370e00
00372f28: mov r0, r7
00372f2c: bl #0x3721b8
00372f30: ldr r0, [sp, #0x1c]
00372f34: ldr r2, [sp, #0x1cc]
00372f38: ldr r3, [r8, r0]
00372f3c: ldr r3, [r3]
00372f40: cmp r2, r3
00372f44: bne #0x373b84
00372f48: add sp, sp, #0x1d4
00372f4c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00372f50: add r0, r4, #0x500
00372f54: add r0, r0, #8
00372f58: bl #0x814f70
00372f5c: cmp r0, #0
00372f60: beq #0x372b6c
00372f64: ldrb r3, [r4, #0x525]
00372f68: cmp r3, #0
00372f6c: beq #0x372b6c
00372f70: mov r0, r7
00372f74: bl #0x36f074
00372f78: ldr lr, [sp, #0x20]
00372f7c: cmp r0, #0
00372f80: movne lr, #1
00372f84: str lr, [sp, #0x20]
00372f88: b #0x372b6c
00372f8c: bl #0x320e98
00372f90: ldrb r3, [r0, #0x28]
00372f94: cmp r3, #0
00372f98: beq #0x37317c
00372f9c: bl #0x81a6ac
00372fa0: add r3, sp, #0xd4
00372fa4: ldr r1, [r0, #4]
00372fa8: add r2, sp, #0xd0
00372fac: mov r0, r3
00372fb0: add sl, sp, #0x16c
00372fb4: str r3, [sp, #0x14]
00372fb8: bl #0x3140ec
00372fbc: add r1, r4, #0x2d0
00372fc0: mov r0, sl
00372fc4: bl #0x32b918
00372fc8: ldr r0, [sp, #0x180]
00372fcc: ldr r1, [sp, #0xe8]
00372fd0: ldr r2, [sp, #0x17c]
00372fd4: ldr r3, [sp, #0xe4]
00372fd8: rsb r2, r0, r2
00372fdc: rsb r3, r1, r3
00372fe0: cmp r2, r3
00372fe4: beq #0x3738d4
00372fe8: mov r0, sl
00372fec: add sl, sp, #0x154
00372ff0: bl #0x318254
00372ff4: add r2, sp, #0xcc
00372ff8: ldr r1, [sp, #0xe8]
00372ffc: mov r0, sl
00373000: bl #0x3140ec
00373004: mov r0, r4
00373008: mov r1, sl
0037300c: bl #0x371ccc
00373010: mov r0, sl
00373014: bl #0x318254
00373018: ldr r0, [sp, #0x14]
0037301c: bl #0x318254
00373020: cmp r5, #0
00373024: bne #0x372990
00373028: ldr r1, [sb, #0x34]
0037302c: ldr r3, [r4, #0x380]
00373030: cmp r3, r1
00373034: beq #0x373040
00373038: mov r0, r4
0037303c: bl #0x370ef0
00373040: ldr r1, [sb, #0x30]
00373044: ldr r3, [r4, #0x330]
00373048: cmp r3, r1
0037304c: beq #0x372990
00373050: mov r0, r4
00373054: bl #0x370e48
00373058: b #0x372990
0037305c: ldr lr, [sp, #0x24]
00373060: cmp lr, #0
00373064: beq #0x372ec0
00373068: ldr r3, [r4, #0x380]
0037306c: cmn r3, #1
00373070: beq #0x372ec0
00373074: bl #0x7fd794
00373078: ldrb r3, [r0, #5]
0037307c: cmp r3, #0
00373080: bne #0x373a64
00373084: ldr r1, [r4, #0x670]
00373088: mov r0, r7
0037308c: bl #0x372220
00373090: ldrb r3, [r4, #0x4e5]
00373094: ldr r1, [sp, #0x10]
00373098: ldr r2, [sp, #0xc]
0037309c: cmp r3, #0
003730a0: moveq r1, r3
003730a4: movne r2, #1
003730a8: str r1, [sp, #0x10]
003730ac: str r2, [sp, #0xc]
003730b0: b #0x372ec0
003730b4: bl #0x320e98
003730b8: ldrb r3, [r0, #0x24]
003730bc: cmp r3, #0
003730c0: beq #0x372b5c
003730c4: bl #0x800f8c
003730c8: ldr r3, [r0]
003730cc: mov lr, pc
003730d0: ldr pc, [r3, #0x64]
003730d4: cmp r0, #0
003730d8: beq #0x372b5c
003730dc: bl #0x8100dc
003730e0: bl #0x8100e0
003730e4: cmp r0, #0
003730e8: beq #0x372b5c
003730ec: ldrb r3, [r4, #0x525]
003730f0: cmp r3, #0
003730f4: bne #0x372b5c
003730f8: mov r0, r7
003730fc: bl #0x36e09c
00373100: ldrb r3, [r0, #0x4e5]
00373104: cmp r3, #0
00373108: beq #0x372b5c
0037310c: ldrb r3, [r7, #0x71a]
00373110: cmp r3, #0
00373114: moveq r3, #1
00373118: strbeq r3, [r7, #0x71a]
0037311c: b #0x372b5c
00373120: bl #0x320e98
00373124: ldrb r3, [r0, #0x24]
00373128: cmp r3, #0
0037312c: beq #0x372b6c
00373130: bl #0x800f8c
00373134: ldr r3, [r0]
00373138: mov lr, pc
0037313c: ldr pc, [r3, #0x64]
00373140: cmp r0, #0
00373144: beq #0x372b6c
00373148: bl #0x8100dc
0037314c: bl #0x8100e0
00373150: cmp r0, #0
00373154: beq #0x372b6c
00373158: ldrb r3, [r4, #0x525]
0037315c: cmp r3, #0
00373160: beq #0x372b6c
00373164: ldrb r3, [r4, #0x545]
00373168: cmp r3, #0
0037316c: bne #0x372b6c
00373170: mov r0, r4
00373174: bl #0x3709b4
00373178: b #0x372b6c
0037317c: add sl, sp, #0x13c
00373180: add r1, r4, #0x2d0
00373184: mov r0, sl
00373188: bl #0x32b918
0037318c: ldr r1, [sb, #0x2c]
00373190: mov r0, sl
00373194: bl #0x313c48
00373198: mov r3, r0
0037319c: mov r0, sl
003731a0: str r3, [sp, #8]
003731a4: bl #0x318254
003731a8: ldr r3, [sp, #8]
003731ac: cmp r3, #0
003731b0: bne #0x372988
003731b4: add sl, sp, #0x124
003731b8: add r2, sp, #0xc8
003731bc: mov r0, sl
003731c0: ldr r1, [sb, #0x2c]
003731c4: bl #0x3140ec
003731c8: b #0x372974
003731cc: mov r1, r4
003731d0: mov r0, r7
003731d4: bl #0x36d850
003731d8: mov r0, r5
003731dc: bl #0x3bb7e8
003731e0: cmp r0, #0
003731e4: addeq r3, r4, #0x2d0
003731e8: beq #0x373a00
003731ec: add r3, r4, #0x2d0
003731f0: add sl, sp, #0x10c
003731f4: mov r1, r3
003731f8: mov r0, sl
003731fc: str r3, [sp, #8]
00373200: bl #0x32b918
00373204: mov r0, r5
00373208: bl #0x3bb7e8
0037320c: mov r1, r0
00373210: mov r0, sl
00373214: bl #0x313c48
00373218: mov sb, r0
0037321c: eor sb, sb, #1
00373220: mov r0, sl
00373224: bl #0x318254
00373228: tst sb, #0xff
0037322c: ldr r3, [sp, #8]
00373230: bne #0x373a00
00373234: mov r0, r5
00373238: ldr sl, [r4, #0x380]
0037323c: bl #0x3bb7fc
00373240: cmp r0, sl
00373244: beq #0x373308
00373248: ldr ip, [sp, #0x18]
0037324c: ldr sl, [r8, ip]
00373250: mov r0, sl
00373254: bl #0x31f594
00373258: cmp r0, #0
0037325c: beq #0x373308
00373260: mov r0, sl
00373264: bl #0x31f594
00373268: ldrb r3, [r0, #0x144]
0037326c: cmp r3, #0
00373270: beq #0x373308
00373274: ldr sl, [r4, #0x380]
00373278: add sb, r5, #0x560
0037327c: mov r0, sb
00373280: mov r1, sl
00373284: bl #0x3e0854
00373288: mov r0, r5
0037328c: mov r1, sl
00373290: bl #0x3bb814
00373294: add r0, r5, #0x3c8
00373298: bl #0x3d8cfc
0037329c: mov lr, #1
003732a0: str lr, [sp, #0x28]
003732a4: b #0x373314
003732a8: bl #0x7fd794
003732ac: bl #0x7fd5b4
003732b0: cmp r0, #0
003732b4: beq #0x372eec
003732b8: ldr r0, [sp, #0xc]
003732bc: cmp r0, #0
003732c0: beq #0x372eec
003732c4: ldr r1, [sp, #0x10]
003732c8: cmp r1, #0
003732cc: bne #0x372eec
003732d0: ldr r2, [sp, #0x18]
003732d4: ldr r4, [pc, #0x420]
003732d8: ldr r0, [r8, r2]
003732dc: add r4, pc, r4
003732e0: ldr r5, [r4]
003732e4: bl #0x31f66c
003732e8: rsb r0, r0, r5
003732ec: cmp r0, #0
003732f0: movle r3, #0x7d0
003732f4: str r0, [r4]
003732f8: strle r3, [r4]
003732fc: movle r4, #1
00373300: bgt #0x372eec
00373304: b #0x372ef0
00373308: mov ip, #0
0037330c: str ip, [sp, #0x28]
00373310: add sb, r5, #0x560
00373314: ldr r1, [r4, #0x498]
00373318: ldr r3, [r5, #0x39c]
0037331c: cmp r1, r3
00373320: addeq sl, r5, #0x37c
00373324: beq #0x373334
00373328: add sl, r5, #0x37c
0037332c: mov r0, sl
00373330: bl #0x3fdfd8
00373334: ldr r3, [r4, #0x358]
00373338: mov r0, sl
0037333c: str r3, [sp, #8]
00373340: bl #0x3fc690
00373344: ldr r3, [sp, #8]
00373348: cmp r0, r3
0037334c: beq #0x37335c
00373350: mov r0, sl
00373354: ldr r1, [r4, #0x358]
00373358: bl #0x3ffc40
0037335c: mov r0, sb
00373360: mov r1, #0x21
00373364: mov r2, #0
00373368: ldr sl, [r4, #0x448]
0037336c: bl #0x3df6e0
00373370: cmp r0, sl
00373374: beq #0x373388
00373378: mov r0, sb
0037337c: mov r1, #0x21
00373380: ldr r2, [r4, #0x448]
00373384: bl #0x3e0808
00373388: mov r0, sb
0037338c: mov r1, #0x24
00373390: mov r2, #0
00373394: ldr sl, [r4, #0x470]
00373398: bl #0x3df6e0
0037339c: cmp r0, sl
003733a0: beq #0x3733d4
003733a4: ldr r3, [r5]
003733a8: mov r0, r5
003733ac: mov lr, pc
003733b0: ldr pc, [r3, #0x34]
003733b4: mov r1, #0x24
003733b8: mov r2, #0
003733bc: mov r0, sb
003733c0: bl #0x3df6e0
003733c4: mov r0, sb
003733c8: mov r1, #0x24
003733cc: ldr r2, [r4, #0x470]
003733d0: bl #0x3e0808
003733d4: ldr sl, [r4, #0x660]
003733d8: mov r0, r5
003733dc: ldr r2, [sl, #0x93c]
003733e0: ldr r3, [sl, #0x5b8]
003733e4: add r3, r2, r3
003733e8: asr r3, r3, #8
003733ec: str r3, [sp, #0x14]
003733f0: bl #0x3bb828
003733f4: ldr r1, [r4, #0x330]
003733f8: mov r3, r0
003733fc: cmp r0, r1
00373400: beq #0x373410
00373404: mov r0, r5
00373408: bl #0x3bb840
0037340c: ldr r3, [r4, #0x330]
00373410: ldr r0, [sp, #0x14]
00373414: cmp r0, r3
00373418: beq #0x373448
0037341c: ldr r2, [sl, #0x5b8]
00373420: mov r1, #0x13
00373424: mov r0, sb
00373428: rsb r2, r2, r3, lsl #8
0037342c: asr r2, r2, #8
00373430: bl #0x3e0808
00373434: ldr r2, [r4, #0x330]
00373438: ldr r1, [sp, #0x14]
0037343c: rsb r2, r1, r2
00373440: cmp r2, #1
00373444: beq #0x373978
00373448: add r0, r4, #0x3d8
0037344c: bl #0x814f70
00373450: cmp r0, #0
00373454: beq #0x3738a8
00373458: ldr r1, [r4, #0x3f8]
0037345c: cmp r1, #0
00373460: beq #0x3739d0
00373464: ldr r2, [r4, #0x3fc]
00373468: cmp r2, #0
0037346c: ble #0x3739d0
00373470: add sb, sp, #0xc4
00373474: mov r0, sb
00373478: bl #0x30e868
0037347c: mov sl, #0
00373480: ldrsb r2, [sb, sl]
00373484: mov r1, sl
00373488: mov r0, r5
0037348c: cmn r2, #1
00373490: mvnlt lr, #0
00373494: strblt lr, [sb, sl]
00373498: mvnlt r2, #0
0037349c: add sl, sl, #1
003734a0: bl #0x3bbe54
003734a4: cmp sl, #3
003734a8: bne #0x373480
003734ac: add r0, r4, #0x400
003734b0: bl #0x814f70
003734b4: cmp r0, #0
003734b8: beq #0x3738c4
003734bc: movw r3, #0x14e8
003734c0: ldr r3, [r5, r3]
003734c4: cmp r3, #0
003734c8: beq #0x3734fc
003734cc: ldr r3, [r3, #0x84]
003734d0: cmp r3, #0x1e
003734d4: bls #0x3734fc
003734d8: ldr r3, [pc, #0x240]
003734dc: ldr r3, [r8, r3]
003734e0: ldr r3, [r3]
003734e4: cmp r3, #2
003734e8: moveq r3, #0
003734ec: streq r3, [r3]
003734f0: beq #0x3734fc
003734f4: cmp r3, #1
003734f8: beq #0x373b04
003734fc: ldr r1, [r4, #0x420]
00373500: cmp r1, #0
00373504: beq #0x37351c
00373508: ldr r2, [r4, #0x424]
0037350c: cmp r2, #0
00373510: ble #0x37351c
00373514: add r0, sp, #0xd4
00373518: bl #0x30e868
0037351c: mov sl, #0
00373520: add r3, sp, #0xd4
00373524: str r4, [sp, #0x14]
00373528: mov r1, sl
0037352c: movw sb, #0x14e8
00373530: mov r4, r3
00373534: ldr r3, [r5, sb]
00373538: cmp r3, #0
0037353c: beq #0x373568
00373540: ldr r3, [r3, #0x84]
00373544: cmp sl, r3
00373548: bhs #0x373568
0037354c: ldrsb r2, [r4, sl]
00373550: cmp r2, #0
00373554: blt #0x373568
00373558: mov r1, sl
0037355c: mov r0, r5
00373560: bl #0x3bbebc
00373564: mov r1, #1
00373568: add sl, sl, #1
0037356c: cmp sl, #0x1e
00373570: bne #0x373534
00373574: cmp r1, #0
00373578: ldr r4, [sp, #0x14]
0037357c: bne #0x37396c
00373580: ldrb r3, [r4, #0x4e5]
00373584: cmp r3, #0
00373588: ldrne r1, [sp, #0x2c]
0037358c: moveq sb, r3
00373590: ldrne r2, [r8, r1]
00373594: ldrbne sb, [r2, #0x30]
00373598: ldrb r2, [r5, #0x80]
0037359c: eorne sb, sb, #1
003735a0: cmp r2, sb
003735a4: addne sl, r5, #0x4f0
003735a8: addne sl, sl, #0xc
003735ac: beq #0x373a28
003735b0: mov r0, sl
003735b4: mov r1, #0
003735b8: bl #0x3c1a00
003735bc: add sl, r4, #0x4c0
003735c0: mov r0, r5
003735c4: mov r1, sb
003735c8: add sl, sl, #8
003735cc: ldr r3, [r5]
003735d0: mov lr, pc
003735d4: ldr pc, [r3, #0x40]
003735d8: mov r0, sl
003735dc: bl #0x814f70
003735e0: b #0x372e6c
003735e4: bl #0x320e98
003735e8: ldrb r3, [r0, #0x24]
003735ec: cmp r3, #0
003735f0: beq #0x372aac
003735f4: bl #0x800f8c
003735f8: ldr r3, [r0]
003735fc: mov lr, pc
00373600: ldr pc, [r3, #0x64]
00373604: cmp r0, #0
00373608: beq #0x372aac
0037360c: bl #0x8100dc
00373610: bl #0x8100e0
00373614: b #0x372aac
00373618: bl #0x30e5e0
0037361c: cmp r0, #0
00373620: bne #0x372950
00373624: mov r0, sl
00373628: bl #0x318254
0037362c: ldr r0, [sp, #0x14]
00373630: bl #0x318254
00373634: b #0x372988
00373638: ldr r3, [sp, #0x18]
0037363c: ldr r5, [r8, r3]
00373640: mov r0, r5
00373644: bl #0x31f594
00373648: cmp r0, #0
0037364c: beq #0x372f00
00373650: mov r1, #0
00373654: mov r0, r7
00373658: mov r2, r1
0037365c: bl #0x36e478
00373660: ldrb r3, [r0, #0x4e5]
00373664: cmp r3, #0
00373668: bne #0x372f00
0037366c: ldr r6, [pc, #0x8c]
00373670: mov r0, r5
00373674: add r6, pc, r6
00373678: ldr r5, [r6, #4]
0037367c: bl #0x31f66c
00373680: rsb r0, r0, r5
00373684: cmp r0, #0
00373688: movwle r3, #0x1388
0037368c: str r0, [r6, #4]
00373690: strle r3, [r6, #4]
00373694: bgt #0x372f00
00373698: bl #0x8100dc
0037369c: bl #0x8103f4
003736a0: mov r1, #0
003736a4: mov r2, r1
003736a8: mov r0, r7
003736ac: bl #0x36e478
003736b0: bl #0x81347c
003736b4: bl #0x8151e8
003736b8: mov r3, #0
003736bc: str r3, [r0, #0x2c]
003736c0: b #0x372f08
003736c4: rsbeq r2, r2, r4, ror r2
003736c8: andeq r4, r0, ip, lsr #1
003736cc: strdeq r3, r4, [r0], -r4
003736d0: subseq lr, r4, r4, asr #30
003736d4: andeq r1, r0, r0, lsr #20
003736d8: subseq lr, r4, r0, asr #30

# 0x373bdc _ZN10PlayerInfo5ResetEv
00373bdc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00373be0: ldr r5, [pc, #0x57c]
00373be4: ldr r1, [pc, #0x57c]
00373be8: sub sp, sp, #0x17c
00373bec: add r5, pc, r5
00373bf0: ldr r3, [r5, r1]
00373bf4: mov r4, r0
00373bf8: str r1, [sp, #0x14]
00373bfc: ldr r3, [r3]
00373c00: add r6, sp, #0x15c
00373c04: mov r7, #0
00373c08: str r3, [sp, #0x174]
00373c0c: bl #0x80f27c
00373c10: ldr r1, [pc, #0x554]
00373c14: mov r3, #1
00373c18: strb r3, [r4, #0x66c]
00373c1c: add r2, sp, #0x114
00373c20: add r1, pc, r1
00373c24: str r7, [r4, #0x660]
00373c28: mov r0, r6
00373c2c: bl #0x3140ec
00373c30: mov r1, r6
00373c34: mov r0, r4
00373c38: bl #0x371ccc
00373c3c: mov r0, r6
00373c40: bl #0x318254
00373c44: mov r0, r4
00373c48: mvn r1, #0
00373c4c: bl #0x370e48
00373c50: mov r0, r4
00373c54: mvn r1, #0
00373c58: bl #0x36fcb0
00373c5c: mov r0, r4
00373c60: mvn r1, #0
00373c64: bl #0x370ef0
00373c68: mvn r1, #0
00373c6c: mov r0, r4
00373c70: bl #0x370bf4
00373c74: mov r2, r7
00373c78: add r1, sp, #0x118
00373c7c: mvn r3, #0
00373c80: strb r3, [r1, r2]
00373c84: add r2, r2, #1
00373c88: cmp r2, #0x24
00373c8c: mvn r6, #0
00373c90: bne #0x373c80
00373c94: add r0, r4, #0x3b0
00373c98: bl #0x36ffd0
00373c9c: add r1, sp, #0x110
00373ca0: mov r2, #3
00373ca4: add r0, r4, #0x3d8
00373ca8: strb r6, [sp, #0x110]
00373cac: strb r6, [sp, #0x111]
00373cb0: strb r6, [sp, #0x112]
00373cb4: bl #0x370258
00373cb8: mov r2, #0
00373cbc: add r1, sp, #0x13c
00373cc0: strb r6, [r1, r2]
00373cc4: add r2, r2, #1
00373cc8: cmp r2, #0x1e
00373ccc: bne #0x373cc0
00373cd0: ldr r3, [pc, #0x498]
00373cd4: add r0, r4, #0x400
00373cd8: ldr sb, [pc, #0x494]
00373cdc: str r3, [sp, #0x10]
00373ce0: bl #0x3702a8
00373ce4: mov r0, r4
00373ce8: mov r1, #0
00373cec: bl #0x36fd58
00373cf0: mov r1, #0
00373cf4: mov r0, r4
00373cf8: bl #0x36fe04
00373cfc: ldr ip, [sp, #0x10]
00373d00: ldr lr, [pc, #0x470]
00373d04: add r1, sp, #0x40
00373d08: ldr r3, [r5, ip]
00373d0c: mov r6, #0
00373d10: mov fp, #0xb4000000
00373d14: add r3, r3, #8
00373d18: add r2, r1, #0x20
00373d1c: str lr, [sp]
00373d20: str r1, [sp, #0xc]
00373d24: asr fp, fp, #0x16
00373d28: str r3, [sp, #4]
00373d2c: mov r7, r4
00373d30: mvn sl, #0
00373d34: mov r8, r6
00373d38: str r2, [sp, #8]
00373d3c: mov r3, #0x10
00373d40: str r3, [sp, #0x44]
00373d44: mov r2, #0
00373d48: mov r3, #0
00373d4c: add ip, sp, #0x178
00373d50: strd r2, r3, [ip, fp]
00373d54: ldr r3, [sp, #0x60]
00373d58: ldr lr, [sp, #4]
00373d5c: str sl, [sp, #0x50]
00373d60: cmp r3, #0
00373d64: str sl, [sp, #0x54]
00373d68: str r8, [sp, #0x58]
00373d6c: strb r8, [sp, #0x5c]
00373d70: str lr, [sp, #0x40]
00373d74: beq #0x373d84
00373d78: ldr r0, [sp, #0xc]
00373d7c: str r8, [sp, #0x60]
00373d80: bl #0x814f84
00373d84: ldr r1, [sp]
00373d88: mov r2, #0x28
00373d8c: mul r0, r2, r6
00373d90: ldr r3, [r5, r1]
00373d94: add r0, r0, #0x540
00373d98: add r0, r0, #8
00373d9c: add r3, r3, #8
00373da0: str r3, [sp, #0x40]
00373da4: ldr r3, [r7, #0x548]
00373da8: add r0, r4, r0
00373dac: ldr r1, [sp, #8]
00373db0: mov lr, pc
00373db4: ldr pc, [r3, #0x1c]
00373db8: ldr r3, [r5, sb]
00373dbc: add r6, r6, #1
00373dc0: cmp r6, #7
00373dc4: add r3, r3, #8
00373dc8: str r3, [sp, #0x40]
00373dcc: add r7, r7, #0x28
00373dd0: bne #0x373d3c
00373dd4: ldr r3, [sp, #0x10]
00373dd8: mov r1, #0xaa000000
00373ddc: asr r1, r1, #0x16
00373de0: ldr r0, [r5, r3]
00373de4: ldr r3, [sp, #0x38]
00373de8: mov r6, #0
00373dec: mov r7, #0
00373df0: add ip, sp, #0x178
00373df4: mvn r2, #0
00373df8: strd r6, r7, [ip, r1]
00373dfc: cmp r3, #0
00373e00: add r0, r0, #8
00373e04: mov r3, #0
00373e08: mov r1, #8
00373e0c: str r1, [sp, #0x1c]
00373e10: str r2, [sp, #0x2c]
00373e14: str r0, [sp, #0x18]
00373e18: addeq r6, sp, #0x18
00373e1c: str r2, [r4, #0x678]
00373e20: str r2, [sp, #0x28]
00373e24: str r3, [sp, #0x30]
00373e28: strb r3, [sp, #0x34]
00373e2c: beq #0x373e40
00373e30: add r6, sp, #0x18
00373e34: mov r0, r6
00373e38: str r3, [sp, #0x38]
00373e3c: bl #0x814f84
00373e40: ldr r2, [pc, #0x334]
00373e44: add r1, r6, #0x20
00373e48: ldr r3, [r4, #0x288]
00373e4c: ldr r2, [r5, r2]
00373e50: add r0, r4, #0x288
00373e54: mov r6, #0
00373e58: add r2, r2, #8
00373e5c: str r2, [sp, #0x18]
00373e60: mov lr, pc
00373e64: ldr pc, [r3, #0x1c]
00373e68: ldr lr, [sp, #0x10]
00373e6c: ldr r3, [sp, #0x88]
00373e70: ldr ip, [r5, sb]
00373e74: ldr r0, [r5, lr]
00373e78: mov r1, #0xbe000000
00373e7c: asr r1, r1, #0x16
00373e80: mov r7, #0
00373e84: add lr, sp, #0x178
00373e88: mvn r2, #0
00373e8c: strd r6, r7, [lr, r1]
00373e90: cmp r3, #0
00373e94: add ip, ip, #8
00373e98: mov r3, #0
00373e9c: add r0, r0, #8
00373ea0: mov r1, #0x20
00373ea4: str ip, [sp, #0x18]
00373ea8: str r1, [sp, #0x6c]
00373eac: str r2, [sp, #0x7c]
00373eb0: str r0, [sp, #0x68]
00373eb4: addeq r6, sp, #0x68
00373eb8: str r2, [r4, #0x664]
00373ebc: str r2, [r4, #0x668]
00373ec0: str r2, [r4, #0x670]
00373ec4: str r2, [r4, #0x674]
00373ec8: str r2, [r4, #0x67c]
00373ecc: str r2, [sp, #0x78]
00373ed0: str r3, [sp, #0x80]
00373ed4: strb r3, [sp, #0x84]
00373ed8: beq #0x373eec
00373edc: add r6, sp, #0x68
00373ee0: mov r0, r6
00373ee4: str r3, [sp, #0x88]
00373ee8: bl #0x814f84
00373eec: ldr r2, [pc, #0x28c]
00373ef0: ldr r7, [pc, #0x28c]
00373ef4: ldr r3, [r4, #0x4a0]
00373ef8: ldr r2, [r5, r2]
00373efc: add r1, r6, #0x20
00373f00: add r0, r4, #0x4a0
00373f04: add r2, r2, #8
00373f08: str r2, [sp, #0x68]
00373f0c: mov lr, pc
00373f10: ldr pc, [r3, #0x1c]
00373f14: ldr r0, [r5, sb]
00373f18: ldrb r3, [sp, #0x10d]
00373f1c: ldr r1, [r5, r7]
00373f20: add r0, r0, #8
00373f24: str r0, [sp, #0x68]
00373f28: cmp r3, #0
00373f2c: mvn r2, #0
00373f30: mov r3, #0
00373f34: add r1, r1, #8
00373f38: mov r0, #1
00373f3c: mov sl, #0
00373f40: mov fp, #0
00373f44: str r0, [sp, #0xf4]
00373f48: strd sl, fp, [sp, #0xf8]
00373f4c: str r2, [sp, #0x104]
00373f50: str r1, [sp, #0xf0]
00373f54: addeq r8, sp, #0xf0
00373f58: str r3, [r4, #0x684]
00373f5c: str r3, [r4, #0x680]
00373f60: str r2, [sp, #0x100]
00373f64: str r3, [sp, #0x108]
00373f68: strb r3, [sp, #0x10c]
00373f6c: beq #0x373f80
00373f70: add r8, sp, #0xf0
00373f74: mov r0, r8
00373f78: strb r3, [sp, #0x10d]
00373f7c: bl #0x814f84
00373f80: ldr r6, [pc, #0x200]
00373f84: add r0, r4, #0x4c0
00373f88: add r1, r8, #0x1d
00373f8c: ldr r2, [r5, r6]
00373f90: ldr r3, [r4, #0x4c8]
00373f94: add r0, r0, #8
00373f98: add r2, r2, #8
00373f9c: str r2, [sp, #0xf0]
00373fa0: mov lr, pc
00373fa4: ldr pc, [r3, #0x1c]
00373fa8: ldr r0, [r5, sb]
00373fac: ldrb r3, [sp, #0xed]
00373fb0: ldr r1, [r5, r7]
00373fb4: add r0, r0, #8
00373fb8: cmp r3, #0
00373fbc: mvn r2, #0
00373fc0: mov r3, #0
00373fc4: add r1, r1, #8
00373fc8: str r0, [sp, #0xf0]
00373fcc: mov sl, #0
00373fd0: mov r0, #1
00373fd4: mov fp, #0
00373fd8: str r0, [sp, #0xd4]
00373fdc: strd sl, fp, [sp, #0xd8]
00373fe0: str r2, [sp, #0xe4]
00373fe4: str r1, [sp, #0xd0]
00373fe8: str r2, [sp, #0xe0]
00373fec: str r3, [sp, #0xe8]
00373ff0: strb r3, [sp, #0xec]
00373ff4: addeq r8, sp, #0xd0
00373ff8: beq #0x37400c
00373ffc: add r8, sp, #0xd0
00374000: mov r0, r8
00374004: strb r3, [sp, #0xed]
00374008: bl #0x814f84
0037400c: ldr r3, [r5, r6]
00374010: add r0, r4, #0x4e0
00374014: add r1, r8, #0x1d
00374018: add r3, r3, #8
0037401c: str r3, [sp, #0xd0]
00374020: add r0, r0, #8
00374024: ldr r3, [r4, #0x4e8]
00374028: mov lr, pc
0037402c: ldr pc, [r3, #0x1c]
00374030: ldr r0, [r5, sb]
00374034: ldrb r3, [sp, #0xcd]
00374038: ldr r1, [r5, r7]
0037403c: add r0, r0, #8
00374040: cmp r3, #0
00374044: mvn r2, #0
00374048: mov r3, #0
0037404c: add r1, r1, #8
00374050: str r0, [sp, #0xd0]
00374054: mov sl, #0
00374058: mov r0, #1
0037405c: mov fp, #0
00374060: str r0, [sp, #0xb4]
00374064: strd sl, fp, [sp, #0xb8]
00374068: str r2, [sp, #0xc4]
0037406c: str r1, [sp, #0xb0]
00374070: str r2, [sp, #0xc0]
00374074: str r3, [sp, #0xc8]
00374078: strb r3, [sp, #0xcc]
0037407c: addeq r8, sp, #0xb0
00374080: beq #0x374094
00374084: add r8, sp, #0xb0
00374088: mov r0, r8
0037408c: strb r3, [sp, #0xcd]
00374090: bl #0x814f84
00374094: ldr r3, [r5, r6]
00374098: add r0, r4, #0x500
0037409c: add r1, r8, #0x1d
003740a0: add r3, r3, #8
003740a4: str r3, [sp, #0xb0]
003740a8: add r0, r0, #8
003740ac: ldr r3, [r4, #0x508]
003740b0: mov lr, pc
003740b4: ldr pc, [r3, #0x1c]
003740b8: ldr r0, [r5, sb]
003740bc: ldr r1, [r5, r7]
003740c0: ldrb r3, [sp, #0xad]
003740c4: add r0, r0, #8
003740c8: mvn r2, #0
003740cc: cmp r3, #0
003740d0: add r1, r1, #8
003740d4: mov r3, #0
003740d8: str r0, [sp, #0xb0]
003740dc: mov r8, #0
003740e0: mov r0, #1
003740e4: mov sb, #0
003740e8: str r0, [sp, #0x94]
003740ec: strd r8, sb, [sp, #0x98]
003740f0: str r2, [sp, #0xa4]
003740f4: str r1, [sp, #0x90]
003740f8: str r2, [sp, #0xa0]
003740fc: str r3, [sp, #0xa8]
00374100: strb r3, [sp, #0xac]
00374104: addeq r7, sp, #0x90
00374108: beq #0x37411c
0037410c: add r7, sp, #0x90
00374110: mov r0, r7
00374114: strb r3, [sp, #0xad]
00374118: bl #0x814f84
0037411c: ldr r3, [r5, r6]
00374120: add r0, r4, #0x520
00374124: add r1, r7, #0x1d
00374128: add r3, r3, #8
0037412c: str r3, [sp, #0x90]
00374130: ldr r3, [r4, #0x528]
00374134: add r0, r0, #8
00374138: mov lr, pc
0037413c: ldr pc, [r3, #0x1c]
00374140: ldr r1, [sp, #0x14]
00374144: ldr r2, [sp, #0x174]
00374148: ldr r3, [r5, r1]
0037414c: ldr r3, [r3]
00374150: cmp r2, r3
00374154: bne #0x374160
00374158: add sp, sp, #0x17c
0037415c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00374160: bl #0x30e310
00374164: rsbeq r0, r2, r4, lsr #29
00374168: andeq r4, r0, ip, lsr #1
0037416c: subseq r7, r5, r8, ror #23
00374170: andeq r2, r0, r4, lsl #19
00374174: andeq r1, r0, r8, lsr #1
00374178: andeq r3, r0, ip, lsr r5
0037417c: andeq r1, r0, r0, asr r5
00374180: andeq r1, r0, r8, asr #1
00374184: andeq r3, r0, r8, lsl r0
00374188: andeq r0, r0, r8, asr #21

# 0x37418c _ZN10PlayerInfoC1Ev
0037418c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00374190: ldr r5, [pc, #0x95c]
00374194: ldr r1, [pc, #0x95c]
00374198: sub sp, sp, #0x8c
0037419c: add r5, pc, r5
003741a0: ldr r3, [r5, r1]
003741a4: mov r4, r0
003741a8: str r1, [sp, #0x28]
003741ac: ldr r3, [r3]
003741b0: ldr r7, [pc, #0x944]
003741b4: mov r8, #0
003741b8: str r3, [sp, #0x84]
003741bc: bl #0x80fc28
003741c0: ldr r3, [pc, #0x938]
003741c4: ldr r2, [r4, #0x2a8]
003741c8: ldr r0, [r5, r7]
003741cc: ldr r3, [r5, r3]
003741d0: cmp r2, #0
003741d4: mov sb, #0
003741d8: mov r2, #0
003741dc: add r3, r3, #8
003741e0: mov ip, #0x290
003741e4: strd r8, sb, [r4, ip]
003741e8: mvn r1, #0
003741ec: str r3, [r4]
003741f0: str r2, [r4, #0x2a0]
003741f4: strb r2, [r4, #0x2a4]
003741f8: add r0, r0, #8
003741fc: mov r3, #8
00374200: addeq r2, r4, #0x288
00374204: str r3, [r4, #0x28c]
00374208: str r1, [r4, #0x29c]
0037420c: str r0, [r4, #0x288]
00374210: str r1, [r4, #0x298]
00374214: streq r2, [sp, #0x34]
00374218: beq #0x374230
0037421c: add r3, r4, #0x288
00374220: str r3, [sp, #0x34]
00374224: str r2, [r4, #0x2a8]
00374228: ldr r0, [sp, #0x34]
0037422c: bl #0x814f84
00374230: ldr r3, [pc, #0x8cc]
00374234: ldr r1, [pc, #0x8cc]
00374238: add r6, sp, #0x6c
0037423c: ldr r3, [r5, r3]
00374240: add r0, r4, #0x2b0
00374244: add r2, sp, #0x68
00374248: add r3, r3, #8
0037424c: str r3, [r4, #0x288]
00374250: add r1, pc, r1
00374254: str r0, [sp, #0x20]
00374258: mov r0, r6
0037425c: bl #0x3140ec
00374260: mov r1, r6
00374264: ldr r0, [sp, #0x20]
00374268: bl #0x371bec
0037426c: mov r0, r6
00374270: bl #0x318254
00374274: ldr r3, [r4, #0x308]
00374278: ldr r1, [r5, r7]
0037427c: mov r0, #0x2f0
00374280: cmp r3, #0
00374284: add r1, r1, #8
00374288: mov r8, #0
0037428c: mov sb, #0
00374290: strd r8, sb, [r4, r0]
00374294: mvn r2, #0
00374298: mov r3, #0
0037429c: str r1, [r4, #0x2e8]
003742a0: mov r0, #0x10
003742a4: addeq r1, r4, #0x2e8
003742a8: str r0, [r4, #0x2ec]
003742ac: str r2, [r4, #0x2fc]
003742b0: str r2, [r4, #0x2f8]
003742b4: str r3, [r4, #0x300]
003742b8: strb r3, [r4, #0x304]
003742bc: streq r1, [sp, #0x48]
003742c0: beq #0x3742d8
003742c4: add r2, r4, #0x2e8
003742c8: str r2, [sp, #0x48]
003742cc: str r3, [r4, #0x308]
003742d0: ldr r0, [sp, #0x48]
003742d4: bl #0x814f84
003742d8: ldr r6, [pc, #0x82c]
003742dc: ldr r3, [r4, #0x330]
003742e0: ldr r1, [r5, r7]
003742e4: ldr r0, [r5, r6]
003742e8: cmp r3, #0
003742ec: mov r8, #0
003742f0: mov r3, #0
003742f4: add r0, r0, #8
003742f8: mov sb, #0
003742fc: mov ip, #0x318
00374300: strd r8, sb, [r4, ip]
00374304: mvn r2, #0
00374308: str r0, [r4, #0x2e8]
0037430c: str r3, [r4, #0x328]
00374310: strb r3, [r4, #0x32c]
00374314: add r1, r1, #8
00374318: mov r0, #0x10
0037431c: addeq r3, r4, #0x310
00374320: str r0, [r4, #0x314]
00374324: str r2, [r4, #0x324]
00374328: str r1, [r4, #0x310]
0037432c: str r2, [r4, #0x320]
00374330: streq r3, [sp, #0x30]
00374334: beq #0x37434c
00374338: add r0, r4, #0x310
0037433c: str r0, [sp, #0x30]
00374340: str r3, [r4, #0x330]
00374344: ldr r0, [sp, #0x30]
00374348: bl #0x814f84
0037434c: ldr r3, [r4, #0x358]
00374350: ldr r0, [r5, r6]
00374354: ldr r1, [r5, r7]
00374358: cmp r3, #0
0037435c: add r0, r0, #8
00374360: add r1, r1, #8
00374364: mov r8, #0
00374368: mov sb, #0
0037436c: mov ip, #0x340
00374370: strd r8, sb, [r4, ip]
00374374: mvn r2, #0
00374378: mov r3, #0
0037437c: str r0, [r4, #0x310]
00374380: str r1, [r4, #0x338]
00374384: mov r0, #0x10
00374388: addeq r1, r4, #0x338
0037438c: str r0, [r4, #0x33c]
00374390: str r2, [r4, #0x34c]
00374394: str r2, [r4, #0x348]
00374398: str r3, [r4, #0x350]
0037439c: strb r3, [r4, #0x354]
003743a0: streq r1, [sp, #0x38]
003743a4: beq #0x3743bc
003743a8: add r2, r4, #0x338
003743ac: str r2, [sp, #0x38]
003743b0: str r3, [r4, #0x358]
003743b4: ldr r0, [sp, #0x38]
003743b8: bl #0x814f84
003743bc: ldr r3, [r4, #0x380]
003743c0: ldr r0, [r5, r6]
003743c4: ldr r1, [r5, r7]
003743c8: cmp r3, #0
003743cc: add r0, r0, #8
003743d0: mov r3, #0
003743d4: mov r8, #0
003743d8: mov sb, #0
003743dc: mov ip, #0x368
003743e0: strd r8, sb, [r4, ip]
003743e4: mvn r2, #0
003743e8: str r0, [r4, #0x338]
003743ec: str r3, [r4, #0x378]
003743f0: strb r3, [r4, #0x37c]
003743f4: add r1, r1, #8
003743f8: mov r0, #0x10
003743fc: addeq r3, r4, #0x360
00374400: str r0, [r4, #0x364]
00374404: str r2, [r4, #0x374]
00374408: str r1, [r4, #0x360]
0037440c: str r2, [r4, #0x370]
00374410: streq r3, [sp, #0x40]
00374414: beq #0x37442c
00374418: add r0, r4, #0x360
0037441c: str r0, [sp, #0x40]
00374420: str r3, [r4, #0x380]
00374424: ldr r0, [sp, #0x40]
00374428: bl #0x814f84
0037442c: ldr r3, [r4, #0x3a8]
00374430: ldr r0, [r5, r6]
00374434: ldr r1, [r5, r7]
00374438: cmn r3, #1
0037443c: add r0, r0, #8
00374440: add r1, r1, #8
00374444: mov r8, #0
00374448: mov sb, #0
0037444c: mov ip, #0x390
00374450: strd r8, sb, [r4, ip]
00374454: mvn r3, #0
00374458: mov r2, #0
0037445c: str r0, [r4, #0x360]
00374460: str r1, [r4, #0x388]
00374464: mov r0, #0x10
00374468: addeq r1, r4, #0x388
0037446c: str r0, [r4, #0x38c]
00374470: strb r2, [r4, #0x3a4]
00374474: str r3, [r4, #0x398]
00374478: str r3, [r4, #0x39c]
0037447c: str r2, [r4, #0x3a0]
00374480: streq r1, [sp, #0x44]
00374484: beq #0x37449c
00374488: add r2, r4, #0x388
0037448c: str r2, [sp, #0x44]
00374490: str r3, [r4, #0x3a8]
00374494: ldr r0, [sp, #0x44]
00374498: bl #0x814f84
0037449c: ldr r3, [r5, r6]
003744a0: add r0, r4, #0x3b0
003744a4: str r0, [sp, #0x18]
003744a8: add r3, r3, #8
003744ac: str r3, [r4, #0x388]
003744b0: mov r8, #0
003744b4: ldr r0, [sp, #0x18]
003744b8: add r1, sp, #0x60
003744bc: str r8, [sp, #0x60]
003744c0: str r8, [sp, #0x64]
003744c4: bl #0x370550
003744c8: ldr r0, [sp, #0x60]
003744cc: cmp r0, r8
003744d0: beq #0x3744dc
003744d4: bl #0x310440
003744d8: str r8, [sp, #0x60]
003744dc: add r1, r4, #0x3d8
003744e0: mov r8, #0
003744e4: str r1, [sp, #0x1c]
003744e8: mov r0, r1
003744ec: add r1, sp, #0x58
003744f0: str r8, [sp, #0x58]
003744f4: str r8, [sp, #0x5c]
003744f8: bl #0x370608
003744fc: ldr r0, [sp, #0x58]
00374500: cmp r0, r8
00374504: beq #0x374510
00374508: bl #0x310440
0037450c: str r8, [sp, #0x58]
00374510: add r2, r4, #0x400
00374514: mov r8, #0
00374518: mov r0, r2
0037451c: add r1, sp, #0x50
00374520: str r2, [sp, #0x24]
00374524: str r8, [sp, #0x50]
00374528: str r8, [sp, #0x54]
0037452c: bl #0x3706c0
00374530: ldr r0, [sp, #0x50]
00374534: cmp r0, r8
00374538: beq #0x374544
0037453c: bl #0x310440
00374540: str r8, [sp, #0x50]
00374544: ldr r3, [r4, #0x448]
00374548: ldr r1, [r5, r7]
0037454c: mov r0, #0x430
00374550: cmp r3, #0
00374554: mov r8, #0
00374558: mov r3, #0
0037455c: mov sb, #0
00374560: strd r8, sb, [r4, r0]
00374564: str r3, [r4, #0x440]
00374568: strb r3, [r4, #0x444]
0037456c: addeq r3, r4, #0x420
00374570: mvn r2, #0
00374574: add r1, r1, #8
00374578: mov r0, #0x20
0037457c: addeq r3, r3, #8
00374580: str r0, [r4, #0x42c]
00374584: str r2, [r4, #0x43c]
00374588: str r1, [r4, #0x428]
0037458c: str r2, [r4, #0x438]
00374590: streq r3, [sp, #0x14]
00374594: beq #0x3745b0
00374598: add r0, r4, #0x420
0037459c: add r0, r0, #8
003745a0: str r0, [sp, #0x14]
003745a4: str r3, [r4, #0x448]
003745a8: ldr r0, [sp, #0x14]
003745ac: bl #0x814f84
003745b0: ldr r8, [pc, #0x558]
003745b4: ldr r3, [r4, #0x470]
003745b8: ldr r1, [r5, r7]
003745bc: ldr r0, [r5, r8]
003745c0: cmp r3, #0
003745c4: add r1, r1, #8
003745c8: add r0, r0, #8
003745cc: mov sl, #0
003745d0: mov fp, #0
003745d4: movw ip, #0x458
003745d8: strd sl, fp, [r4, ip]
003745dc: mvn r2, #0
003745e0: mov r3, #0
003745e4: str r0, [r4, #0x428]
003745e8: str r1, [r4, #0x450]
003745ec: mov r0, #0x20
003745f0: addeq r1, r4, #0x450
003745f4: str r0, [r4, #0x454]
003745f8: str r2, [r4, #0x464]
003745fc: str r2, [r4, #0x460]
00374600: str r3, [r4, #0x468]
00374604: strb r3, [r4, #0x46c]
00374608: streq r1, [sp, #0x2c]
0037460c: beq #0x374624
00374610: add r2, r4, #0x450
00374614: str r2, [sp, #0x2c]
00374618: str r3, [r4, #0x470]
0037461c: ldr r0, [sp, #0x2c]
00374620: bl #0x814f84
00374624: ldr r3, [r4, #0x498]
00374628: ldr r0, [r5, r8]
0037462c: ldr r1, [r5, r7]
00374630: cmp r3, #0
00374634: mov sl, #0
00374638: mov r3, #0
0037463c: mov fp, #0
00374640: mov ip, #0x480
00374644: strd sl, fp, [r4, ip]
00374648: add r0, r0, #8
0037464c: str r3, [r4, #0x490]
00374650: strb r3, [r4, #0x494]
00374654: addeq r3, r4, #0x470
00374658: mvn r2, #0
0037465c: str r0, [r4, #0x450]
00374660: add r1, r1, #8
00374664: mov r0, #0x20
00374668: addeq r3, r3, #8
0037466c: str r0, [r4, #0x47c]
00374670: str r2, [r4, #0x48c]
00374674: str r1, [r4, #0x478]
00374678: str r2, [r4, #0x488]
0037467c: streq r3, [sp, #8]
00374680: beq #0x37469c
00374684: add r0, r4, #0x470
00374688: add r0, r0, #8
0037468c: str r0, [sp, #8]
00374690: str r3, [r4, #0x498]
00374694: ldr r0, [sp, #8]
00374698: bl #0x814f84
0037469c: ldr r3, [r4, #0x4c0]
003746a0: ldr r0, [r5, r8]
003746a4: ldr r1, [r5, r7]
003746a8: cmp r3, #0
003746ac: add r0, r0, #8
003746b0: add r1, r1, #8
003746b4: mov sl, #0
003746b8: mov fp, #0
003746bc: movw ip, #0x4a8
003746c0: strd sl, fp, [r4, ip]
003746c4: mvn r2, #0
003746c8: mov r3, #0
003746cc: str r0, [r4, #0x478]
003746d0: str r1, [r4, #0x4a0]
003746d4: mov r0, #0x20
003746d8: addeq r1, r4, #0x4a0
003746dc: str r0, [r4, #0x4a4]
003746e0: str r2, [r4, #0x4b4]
003746e4: str r2, [r4, #0x4b0]
003746e8: str r3, [r4, #0x4b8]
003746ec: strb r3, [r4, #0x4bc]
003746f0: streq r1, [sp, #0x3c]
003746f4: beq #0x37470c
003746f8: add r2, r4, #0x4a0
003746fc: str r2, [sp, #0x3c]
00374700: str r3, [r4, #0x4c0]
00374704: ldr r0, [sp, #0x3c]
00374708: bl #0x814f84
0037470c: ldr sl, [pc, #0x400]
00374710: ldrb r3, [r4, #0x4e5]
00374714: ldr r0, [r5, r8]
00374718: ldr r1, [r5, sl]
0037471c: cmp r3, #0
00374720: mov r8, #0
00374724: mov r3, #0
00374728: mov sb, #0
0037472c: mov ip, #0x4d0
00374730: strd r8, sb, [r4, ip]
00374734: add r0, r0, #8
00374738: str r3, [r4, #0x4e0]
0037473c: strb r3, [r4, #0x4e4]
00374740: addeq r3, r4, #0x4c0
00374744: mvn r2, #0
00374748: str r0, [r4, #0x4a0]
0037474c: add r1, r1, #8
00374750: mov r0, #1
00374754: addeq r3, r3, #8
00374758: str r0, [r4, #0x4cc]
0037475c: str r2, [r4, #0x4dc]
00374760: str r1, [r4, #0x4c8]
00374764: str r2, [r4, #0x4d8]
00374768: streq r3, [sp, #0x10]
0037476c: beq #0x374788
00374770: add r0, r4, #0x4c0
00374774: add r0, r0, #8
00374778: str r0, [sp, #0x10]
0037477c: strb r3, [r4, #0x4e5]
00374780: ldr r0, [sp, #0x10]
00374784: bl #0x814f84
00374788: ldr r8, [pc, #0x388]
0037478c: ldrb r3, [r4, #0x505]
00374790: ldr r1, [r5, sl]
00374794: ldr r0, [r5, r8]
00374798: cmp r3, #0
0037479c: add sb, r1, #8
003747a0: add lr, r0, #8
003747a4: mov r1, #0
003747a8: mov r0, #0
003747ac: mov ip, #0x4f0
003747b0: strd r0, r1, [r4, ip]
003747b4: addeq r1, r4, #0x4e0
003747b8: mvn r2, #0
003747bc: mov r3, #0
003747c0: mov r0, #1
003747c4: addeq r1, r1, #8
003747c8: str lr, [r4, #0x4c8]
003747cc: str r0, [r4, #0x4ec]
003747d0: str r2, [r4, #0x4fc]
003747d4: str sb, [r4, #0x4e8]
003747d8: str r2, [r4, #0x4f8]
003747dc: str r3, [r4, #0x500]
003747e0: strb r3, [r4, #0x504]
003747e4: streq r1, [sp]
003747e8: beq #0x374804
003747ec: add r2, r4, #0x4e0
003747f0: add r2, r2, #8
003747f4: str r2, [sp]
003747f8: strb r3, [r4, #0x505]
003747fc: ldr r0, [sp]
00374800: bl #0x814f84
00374804: ldrb r3, [r4, #0x525]
00374808: ldr r0, [r5, r8]
0037480c: ldr r1, [r5, sl]
00374810: cmp r3, #0
00374814: add lr, r0, #8
00374818: add sb, r1, #8
0037481c: mov r0, #0
00374820: mov r1, #0
00374824: mov ip, #0x510
00374828: strd r0, r1, [r4, ip]
0037482c: addeq r1, r4, #0x500
00374830: mvn r2, #0
00374834: mov r3, #0
00374838: mov r0, #1
0037483c: addeq r1, r1, #8
00374840: str lr, [r4, #0x4e8]
00374844: str r0, [r4, #0x50c]
00374848: str r2, [r4, #0x51c]
0037484c: str sb, [r4, #0x508]
00374850: str r2, [r4, #0x518]
00374854: str r3, [r4, #0x520]
00374858: strb r3, [r4, #0x524]
0037485c: streq r1, [sp, #4]
00374860: beq #0x37487c
00374864: add r2, r4, #0x500
00374868: add r2, r2, #8
0037486c: str r2, [sp, #4]
00374870: strb r3, [r4, #0x525]
00374874: ldr r0, [sp, #4]
00374878: bl #0x814f84
0037487c: ldrb r3, [r4, #0x545]
00374880: ldr r0, [r5, r8]
00374884: ldr r1, [r5, sl]
00374888: cmp r3, #0
0037488c: mov sl, #0
00374890: mov r3, #0
00374894: mov fp, #0
00374898: mov ip, #0x530
0037489c: strd sl, fp, [r4, ip]
003748a0: add r0, r0, #8
003748a4: str r3, [r4, #0x540]
003748a8: strb r3, [r4, #0x544]
003748ac: addeq r3, r4, #0x520
003748b0: mvn r2, #0
003748b4: str r0, [r4, #0x508]
003748b8: add r1, r1, #8
003748bc: mov r0, #1
003748c0: addeq r3, r3, #8
003748c4: str r0, [r4, #0x52c]
003748c8: str r2, [r4, #0x53c]
003748cc: str r1, [r4, #0x528]
003748d0: str r2, [r4, #0x538]
003748d4: streq r3, [sp, #0xc]
003748d8: beq #0x3748f4
003748dc: add r0, r4, #0x520
003748e0: add r0, r0, #8
003748e4: str r0, [sp, #0xc]
003748e8: strb r3, [r4, #0x545]
003748ec: ldr r0, [sp, #0xc]
003748f0: bl #0x814f84
003748f4: ldr r3, [r5, r8]
003748f8: ldr r2, [r5, r7]
003748fc: add r7, r4, #0x540
00374900: add r3, r3, #8
00374904: add r2, r2, #8
00374908: str r3, [r4, #0x528]
0037490c: add fp, r4, #0x660
00374910: str r4, [sp, #0x4c]
00374914: add r7, r7, #8
00374918: mov sb, #0x10
0037491c: mvn sl, #0
00374920: mov r8, #0
00374924: mov r4, r2
00374928: ldr r3, [r7, #0x20]
0037492c: mov r0, #0
00374930: mov r1, #0
00374934: cmp r3, #0
00374938: str sb, [r7, #4]
0037493c: strd r0, r1, [r7, #8]
00374940: str sl, [r7, #0x10]
00374944: str sl, [r7, #0x14]
00374948: str r8, [r7, #0x18]
0037494c: strb r8, [r7, #0x1c]
00374950: str r4, [r7]
00374954: beq #0x374964
00374958: str r8, [r7, #0x20]
0037495c: mov r0, r7
00374960: bl #0x814f84
00374964: ldr r3, [r5, r6]
00374968: add r3, r3, #8
0037496c: str r3, [r7], #0x28
00374970: cmp r7, fp
00374974: bne #0x374928
00374978: ldr r6, [pc, #0x19c]
0037497c: ldr r4, [sp, #0x4c]
00374980: add r6, pc, r6
00374984: ldr r3, [r6]
00374988: tst r3, #1
0037498c: beq #0x374ac0
00374990: ldr r1, [sp, #0x34]
00374994: mov r0, r4
00374998: bl #0x81324c
0037499c: mov r0, r4
003749a0: ldr r1, [sp, #0x30]
003749a4: bl #0x81324c
003749a8: mov r0, r4
003749ac: ldr r1, [sp, #0x38]
003749b0: bl #0x81324c
003749b4: mov r0, r4
003749b8: ldr r1, [sp, #0x40]
003749bc: bl #0x81324c
003749c0: mov r0, r4
003749c4: ldr r1, [sp, #0x20]
003749c8: bl #0x81324c
003749cc: mov r0, r4
003749d0: ldr r1, [sp, #0x44]
003749d4: bl #0x81324c
003749d8: mov r0, r4
003749dc: ldr r1, [sp, #0x48]
003749e0: bl #0x81324c
003749e4: mov r0, r4
003749e8: ldr r1, [sp, #0x18]
003749ec: bl #0x81324c
003749f0: mov r0, r4
003749f4: ldr r1, [sp, #0x1c]
003749f8: bl #0x81324c
003749fc: mov r0, r4
00374a00: ldr r1, [sp, #0x24]
00374a04: bl #0x81324c
00374a08: mov r0, r4
00374a0c: ldr r1, [sp, #0x14]
00374a10: bl #0x81324c
00374a14: mov r0, r4
00374a18: ldr r1, [sp, #0x2c]
00374a1c: bl #0x81324c
00374a20: mov r0, r4
00374a24: ldr r1, [sp, #8]
00374a28: bl #0x81324c
00374a2c: mov r0, r4
00374a30: ldr r1, [sp, #0x3c]
00374a34: bl #0x81324c
00374a38: mov r0, r4
00374a3c: ldr r1, [sp, #0x10]
00374a40: bl #0x81324c
00374a44: mov r0, r4
00374a48: ldr r1, [sp]
00374a4c: bl #0x81324c
00374a50: mov r0, r4
00374a54: ldr r1, [sp, #4]
00374a58: bl #0x81324c
00374a5c: mov r0, r4
00374a60: ldr r1, [sp, #0xc]
00374a64: bl #0x81324c
00374a68: mov r6, #0
00374a6c: mov r7, #0x28
00374a70: mul r1, r7, r6
00374a74: mov r0, r4
00374a78: add r1, r1, #0x540
00374a7c: add r1, r1, #8
00374a80: add r6, r6, #1
00374a84: add r1, r4, r1
00374a88: bl #0x81324c
00374a8c: cmp r6, #7
00374a90: bne #0x374a70
00374a94: mov r0, r4
00374a98: bl #0x373bdc
00374a9c: ldr r1, [sp, #0x28]
00374aa0: ldr r2, [sp, #0x84]
00374aa4: mov r0, r4
00374aa8: ldr r3, [r5, r1]
00374aac: ldr r3, [r3]
00374ab0: cmp r2, r3
00374ab4: bne #0x374af0
00374ab8: add sp, sp, #0x8c
00374abc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00374ac0: mov r0, r6
00374ac4: bl #0x30e76c
00374ac8: cmp r0, #0
00374acc: beq #0x374990
00374ad0: ldr r3, [pc, #0x48]
00374ad4: ldr r0, [r5, r3]
00374ad8: ldr r3, [pc, #0x44]
00374adc: ldr r1, [r5, r3]
00374ae0: bl #0x8102c0
00374ae4: mov r0, r6
00374ae8: bl #0x30ea3c
00374aec: b #0x374990
00374af0: bl #0x30e310

# 0x374b28 _ZN13PlayerManagerC1Ev
00374b28: ldr r3, [pc, #0xac]
00374b2c: ldr r2, [pc, #0xac]
00374b30: push {r4, r5, r6, lr}
00374b34: add r3, pc, r3
00374b38: ldr r2, [r3, r2]
00374b3c: mov r4, r0
00374b40: mov r5, #0
00374b44: add r2, r2, #8
00374b48: str r2, [r0], #8
00374b4c: bl #0x37418c
00374b50: mov r3, r4
00374b54: mov r2, #0
00374b58: str r5, [r4, #0x694]
00374b5c: mvn r1, #0
00374b60: strb r5, [r3, #0x690]!
00374b64: str r3, [r4, #0x69c]
00374b68: str r1, [r4, #0x6cc]
00374b6c: str r2, [r4, #0x6dc]
00374b70: str r3, [r4, #0x698]
00374b74: str r5, [r4, #0x6a0]
00374b78: str r5, [r4, #0x6a8]
00374b7c: str r5, [r4, #0x6ac]
00374b80: str r5, [r4, #0x6b0]
00374b84: str r5, [r4, #0x6b4]
00374b88: str r5, [r4, #0x6b8]
00374b8c: str r5, [r4, #0x6bc]
00374b90: str r5, [r4, #0x6c0]
00374b94: str r5, [r4, #0x6c4]
00374b98: strb r5, [r4, #0x6c9]
00374b9c: strb r5, [r4, #0x6ca]
00374ba0: strb r5, [r4, #0x6cb]
00374ba4: strb r5, [r4, #0x6d0]
00374ba8: str r2, [r4, #0x6d4]
00374bac: str r2, [r4, #0x6d8]
00374bb0: add r0, r4, #0x6e0
00374bb4: bl #0x316d3c
00374bb8: strb r5, [r4, #0x71b]
00374bbc: strb r5, [r4, #0x710]
00374bc0: strb r5, [r4, #0x711]
00374bc4: str r5, [r4, #0x714]
00374bc8: strb r5, [r4, #0x718]
00374bcc: strb r5, [r4, #0x719]
00374bd0: strb r5, [r4, #0x71a]
00374bd4: mov r0, r4
00374bd8: pop {r4, r5, r6, pc}
00374bdc: rsbeq pc, r1, ip, asr pc
00374be0: andeq r4, r0, ip, lsr #8

# 0x374be4 _ZN13PlayerManagerC2Ev
00374be4: ldr r3, [pc, #0xac]
00374be8: ldr r2, [pc, #0xac]
00374bec: push {r4, r5, r6, lr}
00374bf0: add r3, pc, r3
00374bf4: ldr r2, [r3, r2]
00374bf8: mov r4, r0
00374bfc: mov r5, #0
00374c00: add r2, r2, #8
00374c04: str r2, [r0], #8
00374c08: bl #0x37418c
00374c0c: mov r3, r4
00374c10: mov r2, #0
00374c14: str r5, [r4, #0x694]
00374c18: mvn r1, #0
00374c1c: strb r5, [r3, #0x690]!
00374c20: str r3, [r4, #0x69c]
00374c24: str r1, [r4, #0x6cc]
00374c28: str r2, [r4, #0x6dc]
00374c2c: str r3, [r4, #0x698]
00374c30: str r5, [r4, #0x6a0]
00374c34: str r5, [r4, #0x6a8]
00374c38: str r5, [r4, #0x6ac]
00374c3c: str r5, [r4, #0x6b0]
00374c40: str r5, [r4, #0x6b4]
00374c44: str r5, [r4, #0x6b8]
00374c48: str r5, [r4, #0x6bc]
00374c4c: str r5, [r4, #0x6c0]
00374c50: str r5, [r4, #0x6c4]
00374c54: strb r5, [r4, #0x6c9]
00374c58: strb r5, [r4, #0x6ca]
00374c5c: strb r5, [r4, #0x6cb]
00374c60: strb r5, [r4, #0x6d0]
00374c64: str r2, [r4, #0x6d4]
00374c68: str r2, [r4, #0x6d8]
00374c6c: add r0, r4, #0x6e0
00374c70: bl #0x316d3c
00374c74: strb r5, [r4, #0x71b]
00374c78: strb r5, [r4, #0x710]
00374c7c: strb r5, [r4, #0x711]
00374c80: str r5, [r4, #0x714]
00374c84: strb r5, [r4, #0x718]
00374c88: strb r5, [r4, #0x719]
00374c8c: strb r5, [r4, #0x71a]
00374c90: mov r0, r4
00374c94: pop {r4, r5, r6, pc}
00374c98: rsbeq pc, r1, r0, lsr #29
00374c9c: andeq r4, r0, ip, lsr #8

# 0x374cc0 _ZN10PlayerInfoC2Ev
00374cc0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00374cc4: ldr r5, [pc, #0x95c]
00374cc8: ldr r1, [pc, #0x95c]
00374ccc: sub sp, sp, #0x8c
00374cd0: add r5, pc, r5
00374cd4: ldr r3, [r5, r1]
00374cd8: mov r4, r0
00374cdc: str r1, [sp, #0x24]
00374ce0: ldr r3, [r3]
00374ce4: ldr r7, [pc, #0x944]
00374ce8: mov r8, #0
00374cec: str r3, [sp, #0x84]
00374cf0: bl #0x80fc28
00374cf4: ldr r3, [pc, #0x938]
00374cf8: ldr r2, [r4, #0x2a8]
00374cfc: ldr r0, [r5, r7]
00374d00: ldr r3, [r5, r3]
00374d04: cmp r2, #0
00374d08: mov sb, #0
00374d0c: mov r2, #0
00374d10: add r3, r3, #8
00374d14: mov ip, #0x290
00374d18: strd r8, sb, [r4, ip]
00374d1c: mvn r1, #0
00374d20: str r3, [r4]
00374d24: str r2, [r4, #0x2a0]
00374d28: strb r2, [r4, #0x2a4]
00374d2c: add r0, r0, #8
00374d30: mov r3, #8
00374d34: addeq r2, r4, #0x288
00374d38: str r3, [r4, #0x28c]
00374d3c: str r1, [r4, #0x29c]
00374d40: str r0, [r4, #0x288]
00374d44: str r1, [r4, #0x298]
00374d48: streq r2, [sp, #0x2c]
00374d4c: beq #0x374d64
00374d50: add r3, r4, #0x288
00374d54: str r3, [sp, #0x2c]
00374d58: str r2, [r4, #0x2a8]
00374d5c: ldr r0, [sp, #0x2c]
00374d60: bl #0x814f84
00374d64: ldr r3, [pc, #0x8cc]
00374d68: ldr r1, [pc, #0x8cc]
00374d6c: add r6, sp, #0x6c
00374d70: ldr r3, [r5, r3]
00374d74: add r0, r4, #0x2b0
00374d78: add r2, sp, #0x68
00374d7c: add r3, r3, #8
00374d80: str r3, [r4, #0x288]
00374d84: add r1, pc, r1
00374d88: str r0, [sp, #0x1c]
00374d8c: mov r0, r6
00374d90: bl #0x3140ec
00374d94: mov r1, r6
00374d98: ldr r0, [sp, #0x1c]
00374d9c: bl #0x371bec
00374da0: mov r0, r6
00374da4: bl #0x318254
00374da8: ldr r3, [r4, #0x308]
00374dac: ldr r1, [r5, r7]
00374db0: mov r0, #0x2f0
00374db4: cmp r3, #0
00374db8: add r1, r1, #8
00374dbc: mov r8, #0
00374dc0: mov sb, #0
00374dc4: strd r8, sb, [r4, r0]
00374dc8: mvn r2, #0
00374dcc: mov r3, #0
00374dd0: str r1, [r4, #0x2e8]
00374dd4: mov r0, #0x10
00374dd8: addeq r1, r4, #0x2e8
00374ddc: str r0, [r4, #0x2ec]
00374de0: str r2, [r4, #0x2fc]
00374de4: str r2, [r4, #0x2f8]
00374de8: str r3, [r4, #0x300]
00374dec: strb r3, [r4, #0x304]
00374df0: streq r1, [sp, #0x40]
00374df4: beq #0x374e0c
00374df8: add r2, r4, #0x2e8
00374dfc: str r2, [sp, #0x40]
00374e00: str r3, [r4, #0x308]
00374e04: ldr r0, [sp, #0x40]
00374e08: bl #0x814f84
00374e0c: ldr r6, [pc, #0x82c]
00374e10: ldr r3, [r4, #0x330]
00374e14: ldr r1, [r5, r7]
00374e18: ldr r0, [r5, r6]
00374e1c: cmp r3, #0
00374e20: mov r8, #0
00374e24: mov r3, #0
00374e28: add r0, r0, #8
00374e2c: mov sb, #0
00374e30: mov ip, #0x318
00374e34: strd r8, sb, [r4, ip]
00374e38: mvn r2, #0
00374e3c: str r0, [r4, #0x2e8]
00374e40: str r3, [r4, #0x328]
00374e44: strb r3, [r4, #0x32c]
00374e48: add r1, r1, #8
00374e4c: mov r0, #0x10
00374e50: addeq r3, r4, #0x310
00374e54: str r0, [r4, #0x314]
00374e58: str r2, [r4, #0x324]
00374e5c: str r1, [r4, #0x310]
00374e60: str r2, [r4, #0x320]
00374e64: streq r3, [sp, #0x48]
00374e68: beq #0x374e80
00374e6c: add r0, r4, #0x310
00374e70: str r0, [sp, #0x48]
00374e74: str r3, [r4, #0x330]
00374e78: ldr r0, [sp, #0x48]
00374e7c: bl #0x814f84
00374e80: ldr r3, [r4, #0x358]
00374e84: ldr r0, [r5, r6]
00374e88: ldr r1, [r5, r7]
00374e8c: cmp r3, #0
00374e90: add r0, r0, #8
00374e94: add r1, r1, #8
00374e98: mov r8, #0
00374e9c: mov sb, #0
00374ea0: mov ip, #0x340
00374ea4: strd r8, sb, [r4, ip]
00374ea8: mvn r2, #0
00374eac: mov r3, #0
00374eb0: str r0, [r4, #0x310]
00374eb4: str r1, [r4, #0x338]
00374eb8: mov r0, #0x10
00374ebc: addeq r1, r4, #0x338
00374ec0: str r0, [r4, #0x33c]
00374ec4: str r2, [r4, #0x34c]
00374ec8: str r2, [r4, #0x348]
00374ecc: str r3, [r4, #0x350]
00374ed0: strb r3, [r4, #0x354]
00374ed4: streq r1, [sp, #0x30]
00374ed8: beq #0x374ef0
00374edc: add r2, r4, #0x338
00374ee0: str r2, [sp, #0x30]
00374ee4: str r3, [r4, #0x358]
00374ee8: ldr r0, [sp, #0x30]
00374eec: bl #0x814f84
00374ef0: ldr r3, [r4, #0x380]
00374ef4: ldr r0, [r5, r6]
00374ef8: ldr r1, [r5, r7]
00374efc: cmp r3, #0
00374f00: add r0, r0, #8
00374f04: mov r3, #0
00374f08: mov r8, #0
00374f0c: mov sb, #0
00374f10: mov ip, #0x368
00374f14: strd r8, sb, [r4, ip]
00374f18: mvn r2, #0
00374f1c: str r0, [r4, #0x338]
00374f20: str r3, [r4, #0x378]
00374f24: strb r3, [r4, #0x37c]
00374f28: add r1, r1, #8
00374f2c: mov r0, #0x10
00374f30: addeq r3, r4, #0x360
00374f34: str r0, [r4, #0x364]
00374f38: str r2, [r4, #0x374]
00374f3c: str r1, [r4, #0x360]
00374f40: str r2, [r4, #0x370]
00374f44: streq r3, [sp, #0x38]
00374f48: beq #0x374f60
00374f4c: add r0, r4, #0x360
00374f50: str r0, [sp, #0x38]
00374f54: str r3, [r4, #0x380]
00374f58: ldr r0, [sp, #0x38]
00374f5c: bl #0x814f84
00374f60: ldr r3, [r4, #0x3a8]
00374f64: ldr r0, [r5, r6]
00374f68: ldr r1, [r5, r7]
00374f6c: cmn r3, #1
00374f70: add r0, r0, #8
00374f74: add r1, r1, #8
00374f78: mov r8, #0
00374f7c: mov sb, #0
00374f80: mov ip, #0x390
00374f84: strd r8, sb, [r4, ip]
00374f88: mvn r3, #0
00374f8c: mov r2, #0
00374f90: str r0, [r4, #0x360]
00374f94: str r1, [r4, #0x388]
00374f98: mov r0, #0x10
00374f9c: addeq r1, r4, #0x388
00374fa0: str r0, [r4, #0x38c]
00374fa4: strb r2, [r4, #0x3a4]
00374fa8: str r3, [r4, #0x398]
00374fac: str r3, [r4, #0x39c]
00374fb0: str r2, [r4, #0x3a0]
00374fb4: streq r1, [sp, #0x3c]
00374fb8: beq #0x374fd0
00374fbc: add r2, r4, #0x388
00374fc0: str r2, [sp, #0x3c]
00374fc4: str r3, [r4, #0x3a8]
00374fc8: ldr r0, [sp, #0x3c]
00374fcc: bl #0x814f84
00374fd0: ldr r3, [r5, r6]
00374fd4: add r0, r4, #0x3b0
00374fd8: str r0, [sp, #0x28]
00374fdc: add r3, r3, #8
00374fe0: str r3, [r4, #0x388]
00374fe4: mov r8, #0
00374fe8: ldr r0, [sp, #0x28]
00374fec: add r1, sp, #0x60
00374ff0: str r8, [sp, #0x60]
00374ff4: str r8, [sp, #0x64]
00374ff8: bl #0x370550
00374ffc: ldr r0, [sp, #0x60]
00375000: cmp r0, r8
00375004: beq #0x375010
00375008: bl #0x310440
0037500c: str r8, [sp, #0x60]
00375010: add r1, r4, #0x3d8
00375014: mov r8, #0
00375018: str r1, [sp, #0x18]
0037501c: mov r0, r1
00375020: add r1, sp, #0x58
00375024: str r8, [sp, #0x58]
00375028: str r8, [sp, #0x5c]
0037502c: bl #0x370608
00375030: ldr r0, [sp, #0x58]
00375034: cmp r0, r8
00375038: beq #0x375044
0037503c: bl #0x310440
00375040: str r8, [sp, #0x58]
00375044: add r2, r4, #0x400
00375048: mov r8, #0
0037504c: mov r0, r2
00375050: add r1, sp, #0x50
00375054: str r2, [sp, #0x20]
00375058: str r8, [sp, #0x50]
0037505c: str r8, [sp, #0x54]
00375060: bl #0x3706c0
00375064: ldr r0, [sp, #0x50]
00375068: cmp r0, r8
0037506c: beq #0x375078
00375070: bl #0x310440
00375074: str r8, [sp, #0x50]
00375078: ldr r3, [r4, #0x448]
0037507c: ldr r1, [r5, r7]
00375080: mov r0, #0x430
00375084: cmp r3, #0
00375088: mov r8, #0
0037508c: mov r3, #0
00375090: mov sb, #0
00375094: strd r8, sb, [r4, r0]
00375098: str r3, [r4, #0x440]
0037509c: strb r3, [r4, #0x444]
003750a0: addeq r3, r4, #0x420
003750a4: mvn r2, #0
003750a8: add r1, r1, #8
003750ac: mov r0, #0x20
003750b0: addeq r3, r3, #8
003750b4: str r0, [r4, #0x42c]
003750b8: str r2, [r4, #0x43c]
003750bc: str r1, [r4, #0x428]
003750c0: str r2, [r4, #0x438]
003750c4: streq r3, [sp, #0x10]
003750c8: beq #0x3750e4
003750cc: add r0, r4, #0x420
003750d0: add r0, r0, #8
003750d4: str r0, [sp, #0x10]
003750d8: str r3, [r4, #0x448]
003750dc: ldr r0, [sp, #0x10]
003750e0: bl #0x814f84
003750e4: ldr r8, [pc, #0x558]
003750e8: ldr r3, [r4, #0x470]
003750ec: ldr r1, [r5, r7]
003750f0: ldr r0, [r5, r8]
003750f4: cmp r3, #0
003750f8: add r1, r1, #8
003750fc: add r0, r0, #8
00375100: mov sl, #0
00375104: mov fp, #0
00375108: movw ip, #0x458
0037510c: strd sl, fp, [r4, ip]
00375110: mvn r2, #0
00375114: mov r3, #0
00375118: str r0, [r4, #0x428]
0037511c: str r1, [r4, #0x450]
00375120: mov r0, #0x20
00375124: addeq r1, r4, #0x450
00375128: str r0, [r4, #0x454]
0037512c: str r2, [r4, #0x464]
00375130: str r2, [r4, #0x460]
00375134: str r3, [r4, #0x468]
00375138: strb r3, [r4, #0x46c]
0037513c: streq r1, [sp, #0x44]
00375140: beq #0x375158
00375144: add r2, r4, #0x450
00375148: str r2, [sp, #0x44]
0037514c: str r3, [r4, #0x470]
00375150: ldr r0, [sp, #0x44]
00375154: bl #0x814f84
00375158: ldr r3, [r4, #0x498]
0037515c: ldr r0, [r5, r8]
00375160: ldr r1, [r5, r7]
00375164: cmp r3, #0
00375168: mov sl, #0
0037516c: mov r3, #0
00375170: mov fp, #0
00375174: mov ip, #0x480
00375178: strd sl, fp, [r4, ip]
0037517c: add r0, r0, #8
00375180: str r3, [r4, #0x490]
00375184: strb r3, [r4, #0x494]
00375188: addeq r3, r4, #0x470
0037518c: mvn r2, #0
00375190: str r0, [r4, #0x450]
00375194: add r1, r1, #8
00375198: mov r0, #0x20
0037519c: addeq r3, r3, #8
003751a0: str r0, [r4, #0x47c]
003751a4: str r2, [r4, #0x48c]
003751a8: str r1, [r4, #0x478]
003751ac: str r2, [r4, #0x488]
003751b0: streq r3, [sp, #4]
003751b4: beq #0x3751d0
003751b8: add r0, r4, #0x470
003751bc: add r0, r0, #8
003751c0: str r0, [sp, #4]
003751c4: str r3, [r4, #0x498]
003751c8: ldr r0, [sp, #4]
003751cc: bl #0x814f84
003751d0: ldr r3, [r4, #0x4c0]
003751d4: ldr r0, [r5, r8]
003751d8: ldr r1, [r5, r7]
003751dc: cmp r3, #0
003751e0: add r0, r0, #8
003751e4: add r1, r1, #8
003751e8: mov sl, #0
003751ec: mov fp, #0
003751f0: movw ip, #0x4a8
003751f4: strd sl, fp, [r4, ip]
003751f8: mvn r2, #0
003751fc: mov r3, #0
00375200: str r0, [r4, #0x478]
00375204: str r1, [r4, #0x4a0]
00375208: mov r0, #0x20
0037520c: addeq r1, r4, #0x4a0
00375210: str r0, [r4, #0x4a4]
00375214: str r2, [r4, #0x4b4]
00375218: str r2, [r4, #0x4b0]
0037521c: str r3, [r4, #0x4b8]
00375220: strb r3, [r4, #0x4bc]
00375224: streq r1, [sp, #0x34]
00375228: beq #0x375240
0037522c: add r2, r4, #0x4a0
00375230: str r2, [sp, #0x34]
00375234: str r3, [r4, #0x4c0]
00375238: ldr r0, [sp, #0x34]
0037523c: bl #0x814f84
00375240: ldr sl, [pc, #0x400]
00375244: ldrb r3, [r4, #0x4e5]
00375248: ldr r0, [r5, r8]
0037524c: ldr r1, [r5, sl]
00375250: cmp r3, #0
00375254: mov r8, #0
00375258: mov r3, #0
0037525c: mov sb, #0
00375260: mov ip, #0x4d0
00375264: strd r8, sb, [r4, ip]
00375268: add r0, r0, #8
0037526c: str r3, [r4, #0x4e0]
00375270: strb r3, [r4, #0x4e4]
00375274: addeq r3, r4, #0x4c0
00375278: mvn r2, #0
0037527c: str r0, [r4, #0x4a0]
00375280: add r1, r1, #8
00375284: mov r0, #1
00375288: addeq r3, r3, #8
0037528c: str r0, [r4, #0x4cc]
00375290: str r2, [r4, #0x4dc]
00375294: str r1, [r4, #0x4c8]
00375298: str r2, [r4, #0x4d8]
0037529c: streq r3, [sp, #0xc]
003752a0: beq #0x3752bc
003752a4: add r0, r4, #0x4c0
003752a8: add r0, r0, #8
003752ac: str r0, [sp, #0xc]
003752b0: strb r3, [r4, #0x4e5]
003752b4: ldr r0, [sp, #0xc]
003752b8: bl #0x814f84
003752bc: ldr r8, [pc, #0x388]
003752c0: ldrb r3, [r4, #0x505]
003752c4: ldr r1, [r5, sl]
003752c8: ldr r0, [r5, r8]
003752cc: cmp r3, #0
003752d0: add sb, r1, #8
003752d4: add lr, r0, #8
003752d8: mov r1, #0
003752dc: mov r0, #0
003752e0: mov ip, #0x4f0
003752e4: strd r0, r1, [r4, ip]
003752e8: addeq r1, r4, #0x4e0
003752ec: mvn r2, #0
003752f0: mov r3, #0
003752f4: mov r0, #1
003752f8: addeq r1, r1, #8
003752fc: str lr, [r4, #0x4c8]
00375300: str r0, [r4, #0x4ec]
00375304: str r2, [r4, #0x4fc]
00375308: str sb, [r4, #0x4e8]
0037530c: str r2, [r4, #0x4f8]
00375310: str r3, [r4, #0x500]
00375314: strb r3, [r4, #0x504]
00375318: streq r1, [sp, #0x14]
0037531c: beq #0x375338
00375320: add r2, r4, #0x4e0
00375324: add r2, r2, #8
00375328: str r2, [sp, #0x14]
0037532c: strb r3, [r4, #0x505]
00375330: ldr r0, [sp, #0x14]
00375334: bl #0x814f84
00375338: ldrb r3, [r4, #0x525]
0037533c: ldr r0, [r5, r8]
00375340: ldr r1, [r5, sl]
00375344: cmp r3, #0
00375348: add lr, r0, #8
0037534c: add sb, r1, #8
00375350: mov r0, #0
00375354: mov r1, #0
00375358: mov ip, #0x510
0037535c: strd r0, r1, [r4, ip]
00375360: addeq r1, r4, #0x500
00375364: mvn r2, #0
00375368: mov r3, #0
0037536c: mov r0, #1
00375370: addeq r1, r1, #8
00375374: str lr, [r4, #0x4e8]
00375378: str r0, [r4, #0x50c]
0037537c: str r2, [r4, #0x51c]
00375380: str sb, [r4, #0x508]
00375384: str r2, [r4, #0x518]
00375388: str r3, [r4, #0x520]
0037538c: strb r3, [r4, #0x524]
00375390: streq r1, [sp]
00375394: beq #0x3753b0
00375398: add r2, r4, #0x500
0037539c: add r2, r2, #8
003753a0: str r2, [sp]
003753a4: strb r3, [r4, #0x525]
003753a8: ldr r0, [sp]
003753ac: bl #0x814f84
003753b0: ldrb r3, [r4, #0x545]
003753b4: ldr r0, [r5, r8]
003753b8: ldr r1, [r5, sl]
003753bc: cmp r3, #0
003753c0: mov sl, #0
003753c4: mov r3, #0
003753c8: mov fp, #0
003753cc: mov ip, #0x530
003753d0: strd sl, fp, [r4, ip]
003753d4: add r0, r0, #8
003753d8: str r3, [r4, #0x540]
003753dc: strb r3, [r4, #0x544]
003753e0: addeq r3, r4, #0x520
003753e4: mvn r2, #0
003753e8: str r0, [r4, #0x508]
003753ec: add r1, r1, #8
003753f0: mov r0, #1
003753f4: addeq r3, r3, #8
003753f8: str r0, [r4, #0x52c]
003753fc: str r2, [r4, #0x53c]
00375400: str r1, [r4, #0x528]
00375404: str r2, [r4, #0x538]
00375408: streq r3, [sp, #8]
0037540c: beq #0x375428
00375410: add r0, r4, #0x520
00375414: add r0, r0, #8
00375418: str r0, [sp, #8]
0037541c: strb r3, [r4, #0x545]
00375420: ldr r0, [sp, #8]
00375424: bl #0x814f84
00375428: ldr r3, [r5, r8]
0037542c: ldr r2, [r5, r7]
00375430: add r7, r4, #0x540
00375434: add r3, r3, #8
00375438: add r2, r2, #8
0037543c: str r3, [r4, #0x528]
00375440: add fp, r4, #0x660
00375444: str r4, [sp, #0x4c]
00375448: add r7, r7, #8
0037544c: mov sb, #0x10
00375450: mvn sl, #0
00375454: mov r8, #0
00375458: mov r4, r2
0037545c: ldr r3, [r7, #0x20]
00375460: mov r0, #0
00375464: mov r1, #0
00375468: cmp r3, #0
0037546c: str sb, [r7, #4]
00375470: strd r0, r1, [r7, #8]
00375474: str sl, [r7, #0x10]
00375478: str sl, [r7, #0x14]
0037547c: str r8, [r7, #0x18]
00375480: strb r8, [r7, #0x1c]
00375484: str r4, [r7]
00375488: beq #0x375498
0037548c: str r8, [r7, #0x20]
00375490: mov r0, r7
00375494: bl #0x814f84
00375498: ldr r3, [r5, r6]
0037549c: add r3, r3, #8
003754a0: str r3, [r7], #0x28
003754a4: cmp r7, fp
003754a8: bne #0x37545c
003754ac: ldr r6, [pc, #0x19c]
003754b0: ldr r4, [sp, #0x4c]
003754b4: add r6, pc, r6
003754b8: ldr r3, [r6]
003754bc: tst r3, #1
003754c0: beq #0x3755f4
003754c4: ldr r1, [sp, #0x2c]
003754c8: mov r0, r4
003754cc: bl #0x81324c
003754d0: mov r0, r4
003754d4: ldr r1, [sp, #0x48]
003754d8: bl #0x81324c
003754dc: mov r0, r4
003754e0: ldr r1, [sp, #0x30]
003754e4: bl #0x81324c
003754e8: mov r0, r4
003754ec: ldr r1, [sp, #0x38]
003754f0: bl #0x81324c
003754f4: mov r0, r4
003754f8: ldr r1, [sp, #0x1c]
003754fc: bl #0x81324c
00375500: mov r0, r4
00375504: ldr r1, [sp, #0x3c]
00375508: bl #0x81324c
0037550c: mov r0, r4
00375510: ldr r1, [sp, #0x40]
00375514: bl #0x81324c
00375518: mov r0, r4
0037551c: ldr r1, [sp, #0x28]
00375520: bl #0x81324c
00375524: mov r0, r4
00375528: ldr r1, [sp, #0x18]
0037552c: bl #0x81324c
00375530: mov r0, r4
00375534: ldr r1, [sp, #0x20]
00375538: bl #0x81324c
0037553c: mov r0, r4
00375540: ldr r1, [sp, #0x10]
00375544: bl #0x81324c
00375548: mov r0, r4
0037554c: ldr r1, [sp, #0x44]
00375550: bl #0x81324c
00375554: mov r0, r4
00375558: ldr r1, [sp, #4]
0037555c: bl #0x81324c
00375560: mov r0, r4
00375564: ldr r1, [sp, #0x34]
00375568: bl #0x81324c
0037556c: mov r0, r4
00375570: ldr r1, [sp, #0xc]
00375574: bl #0x81324c
00375578: mov r0, r4
0037557c: ldr r1, [sp, #0x14]
00375580: bl #0x81324c
00375584: mov r0, r4
00375588: ldr r1, [sp]
0037558c: bl #0x81324c
00375590: mov r0, r4
00375594: ldr r1, [sp, #8]
00375598: bl #0x81324c
0037559c: mov r6, #0
003755a0: mov r7, #0x28
003755a4: mul r1, r7, r6
003755a8: mov r0, r4
003755ac: add r1, r1, #0x540
003755b0: add r1, r1, #8
003755b4: add r6, r6, #1
003755b8: add r1, r4, r1
003755bc: bl #0x81324c
003755c0: cmp r6, #7
003755c4: bne #0x3755a4
003755c8: mov r0, r4
003755cc: bl #0x373bdc
003755d0: ldr r1, [sp, #0x24]
003755d4: ldr r2, [sp, #0x84]
003755d8: mov r0, r4
003755dc: ldr r3, [r5, r1]
003755e0: ldr r3, [r3]
003755e4: cmp r2, r3
003755e8: bne #0x375624
003755ec: add sp, sp, #0x8c
003755f0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003755f4: mov r0, r6
003755f8: bl #0x30e76c
003755fc: cmp r0, #0
00375600: beq #0x3754c4
00375604: ldr r3, [pc, #0x48]
00375608: ldr r0, [r5, r3]
0037560c: ldr r3, [pc, #0x44]
00375610: ldr r1, [r5, r3]
00375614: bl #0x8102c0
00375618: mov r0, r6
0037561c: bl #0x30ea3c
00375620: b #0x3754c4
00375624: bl #0x30e310
00375628: rsbeq pc, r1, r0, asr #27
0037562c: andeq r4, r0, ip, lsr #1
00375630: andeq r2, r0, r4, lsl #19
00375634: andeq r2, r0, r4, ror sp
00375638: andeq r1, r0, r0, asr r5
0037563c: subseq r6, r5, r4, lsl #21
00375640: andeq r3, r0, ip, lsr r5
00375644: andeq r1, r0, r8, asr #1
00375648: andeq r3, r0, r8, lsl r0
0037564c: andeq r0, r0, r8, asr #21

# 0x37565c _ZN13PlayerManager18ReviveLocalPlayersEb
0037565c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00375660: mov r3, #0
00375664: sub sp, sp, #0x64
00375668: str r0, [sp, #0x20]
0037566c: strb r3, [r0, #0x711]
00375670: str r3, [r0, #0x714]
00375674: str r1, [sp, #0x2c]
00375678: bl #0x7fd794
0037567c: ldrb r3, [r0, #5]
00375680: ldr fp, [pc, #0x44c]
00375684: cmp r3, #0
00375688: add fp, pc, fp
0037568c: beq #0x375abc
00375690: ldr r0, [pc, #0x440]
00375694: str r0, [sp, #0x10]
00375698: ldr r0, [sp, #0x10]
0037569c: ldr r3, [pc, #0x438]
003756a0: ldr r1, [pc, #0x438]
003756a4: ldr sl, [fp, r0]
003756a8: str r3, [sp, #0x28]
003756ac: ldr r3, [fp, r3]
003756b0: add r1, pc, r1
003756b4: mov r0, sl
003756b8: ldr r4, [r3]
003756bc: bl #0x320e44
003756c0: bl #0x30e964
003756c4: mov r1, #1
003756c8: mov r2, r0
003756cc: mov r0, r4
003756d0: bl #0x369da0
003756d4: ldr r3, [pc, #0x408]
003756d8: add r2, sp, #0x5c
003756dc: ldr r0, [sl, #0x40]
003756e0: add r3, pc, r3
003756e4: str r3, [sp, #0x14]
003756e8: ldr r3, [pc, #0x3f8]
003756ec: mov r1, #0
003756f0: str r2, [sp, #0x24]
003756f4: add r3, pc, r3
003756f8: str r3, [sp, #0x18]
003756fc: str fp, [sp, #0x1c]
00375700: bl #0x36ead0
00375704: mov r8, #0
00375708: cmp r8, r0
0037570c: add r5, sp, #0x48
00375710: bge #0x3758e8
00375714: ldr r0, [sl, #0x40]
00375718: mov r1, r8
0037571c: mov r2, #0
00375720: bl #0x36e478
00375724: ldr r6, [r0, #0x660]
00375728: cmp r6, #0
0037572c: beq #0x3758d0
00375730: add r0, r6, #0x560
00375734: bl #0x3e0af8
00375738: ldr r3, [sl, #0x38]
0037573c: add r4, sp, #0x54
00375740: str r4, [sp, #0x54]
00375744: str r4, [sp, #0x58]
00375748: ldr r7, [r3, #0x14]
0037574c: add sb, r3, #0xc
00375750: mov fp, #0xc
00375754: cmp sb, r7
00375758: beq #0x3757ec
0037575c: ldr r1, [r7, #0x2c]
00375760: cmp r1, #0
00375764: beq #0x3757c0
00375768: mov r0, r5
0037576c: bl #0x33dd2c
00375770: mov r0, r5
00375774: bl #0x33ff54
00375778: subs r3, r0, #0
0037577c: beq #0x3757c0
00375780: str r3, [sp, #0xc]
00375784: bl #0x3a3094
00375788: cmp r0, #0
0037578c: ldr r3, [sp, #0xc]
00375790: beq #0x375930
00375794: ldr r0, [sp, #0x24]
00375798: str r3, [sp, #0xc]
0037579c: str fp, [sp, #0x5c]
003757a0: bl #0x708ec0
003757a4: ldr r3, [sp, #0xc]
003757a8: str r3, [r0, #8]
003757ac: ldr r3, [sp, #0x58]
003757b0: str r4, [r0]
003757b4: str r3, [r0, #4]
003757b8: str r0, [r3]
003757bc: str r0, [sp, #0x58]
003757c0: ldr r3, [r7, #0xc]
003757c4: cmp r3, #0
003757c8: bne #0x3757d4
003757cc: b #0x375948
003757d0: mov r3, r2
003757d4: ldr r2, [r3, #8]
003757d8: cmp r2, #0
003757dc: bne #0x3757d0
003757e0: mov r7, r3
003757e4: cmp sb, r7
003757e8: bne #0x37575c
003757ec: bl #0x7fd794
003757f0: ldrb r3, [r0, #5]
003757f4: cmp r3, #0
003757f8: bne #0x375a1c
003757fc: movw r3, #0x1474
00375800: ldr r3, [r6, r3]
00375804: ldr r0, [sp, #0x1c]
00375808: ldr r2, [pc, #0x2dc]
0037580c: str r3, [sp, #0x3c]
00375810: movw r3, #0x1478
00375814: ldr r3, [r6, r3]
00375818: add sb, sp, #0x3c
0037581c: ldr r1, [r0, r2]
00375820: str r3, [sp, #0x40]
00375824: movw r3, #0x147c
00375828: ldr r3, [r6, r3]
0037582c: mov r0, sb
00375830: str r3, [sp, #0x44]
00375834: bl #0x312b6c
00375838: cmp r0, #0
0037583c: beq #0x37597c
00375840: movw r3, #0x14a4
00375844: mov r7, #0
00375848: str r7, [r6, r3]
0037584c: mov r2, #1
00375850: mov r0, r6
00375854: mov r1, r7
00375858: bl #0x3a59ac
0037585c: add r0, r6, #0x4f0
00375860: mov r1, r7
00375864: add r0, r0, #0xc
00375868: add r6, r6, #0x37c
0037586c: bl #0x3c1a00
00375870: mov r0, r6
00375874: bl #0x3fc690
00375878: ldr r2, [sp, #0x10]
0037587c: ldr r3, [sp, #0x1c]
00375880: mov sb, r0
00375884: ldr r1, [sp, #0x14]
00375888: ldr r7, [r3, r2]
0037588c: ldr r2, [sp, #0x18]
00375890: ldr r0, [r7, #0x2c]
00375894: bl #0x4c4bdc
00375898: cmp sb, r0
0037589c: blt #0x3759fc
003758a0: ldr r0, [sp, #0x54]
003758a4: cmp r0, r4
003758a8: bne #0x3758b4
003758ac: b #0x3758c8
003758b0: mov r0, r6
003758b4: ldr r6, [r0]
003758b8: mov r1, #0xc
003758bc: bl #0x708f00
003758c0: cmp r6, r4
003758c4: bne #0x3758b0
003758c8: str r4, [sp, #0x58]
003758cc: str r4, [sp, #0x54]
003758d0: ldr r0, [sl, #0x40]
003758d4: mov r1, #0
003758d8: bl #0x36ead0
003758dc: add r8, r8, #1
003758e0: cmp r8, r0
003758e4: blt #0x375714
003758e8: ldr r0, [sp, #0x28]
003758ec: ldr fp, [sp, #0x1c]
003758f0: mov r1, #2
003758f4: ldr r4, [fp, r0]
003758f8: ldr r0, [r4]
003758fc: bl #0x3698f0
00375900: mov r0, sl
00375904: ldr r4, [r4]
00375908: bl #0x31f594
0037590c: mov ip, #0x3e8
00375910: ldr r1, [r0, #0x11c]
00375914: mov r2, #1
00375918: mov r0, r4
0037591c: mov r3, #0
00375920: str ip, [sp]
00375924: bl #0x36bd78
00375928: add sp, sp, #0x64
0037592c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00375930: ldr r2, [r3, #0x418]
00375934: cmp r6, r2
00375938: beq #0x375794
0037593c: ldr r3, [r7, #0xc]
00375940: cmp r3, #0
00375944: bne #0x3757d4
00375948: ldr r2, [r7, #4]
0037594c: ldr r1, [r2, #0xc]
00375950: cmp r7, r1
00375954: bne #0x375970
00375958: mov r7, r2
0037595c: ldr r2, [r2, #4]
00375960: ldr r3, [r2, #0xc]
00375964: cmp r3, r7
00375968: beq #0x375958
0037596c: ldr r3, [r7, #0xc]
00375970: cmp r3, r2
00375974: movne r7, r2
00375978: b #0x375754
0037597c: mov r2, #1
00375980: mov r0, r6
00375984: mov r1, sb
00375988: bl #0x393db4
0037598c: mov r1, sb
00375990: mov r0, r6
00375994: bl #0x393ae4
00375998: mov r0, r6
0037599c: bl #0x3935dc
003759a0: ldr r1, [r0]
003759a4: mov r7, r0
003759a8: ldr r0, [sp, #0x3c]
003759ac: bl #0x30eba4
003759b0: str r0, [sp, #0x3c]
003759b4: ldr r1, [r7, #4]
003759b8: ldr r0, [sp, #0x40]
003759bc: bl #0x30eba4
003759c0: str r0, [sp, #0x40]
003759c4: ldr r1, [r7, #8]
003759c8: ldr r0, [sp, #0x44]
003759cc: bl #0x30eba4
003759d0: ldr r7, [sp, #0x54]
003759d4: str r0, [sp, #0x44]
003759d8: b #0x3759f0
003759dc: ldr r0, [r7, #8]
003759e0: mov r1, sb
003759e4: mov r2, #1
003759e8: bl #0x393db4
003759ec: ldr r7, [r7]
003759f0: cmp r7, r4
003759f4: bne #0x3759dc
003759f8: b #0x375840
003759fc: ldr r1, [sp, #0x14]
00375a00: ldr r0, [r7, #0x2c]
00375a04: ldr r2, [sp, #0x18]
00375a08: bl #0x4c4bdc
00375a0c: mov r1, r0
00375a10: mov r0, r6
00375a14: bl #0x3ffc40
00375a18: b #0x3758a0
00375a1c: ldr r0, [sp, #0x20]
00375a20: bl #0x36f074
00375a24: cmp r0, #0
00375a28: beq #0x375a38
00375a2c: ldr r3, [sp, #0x2c]
00375a30: cmp r3, #0
00375a34: bne #0x3757fc
00375a38: ldr r0, [sp, #0x20]
00375a3c: bl #0x36e09c
00375a40: ldr r3, [r0]
00375a44: mov r7, r0
00375a48: mov lr, pc
00375a4c: ldr pc, [r3, #0x5c]
00375a50: cmp r0, #0
00375a54: beq #0x375840
00375a58: ldr r3, [r7, #0x660]
00375a5c: cmp r3, #0
00375a60: beq #0x375840
00375a64: ldr r2, [r3, #0x160]
00375a68: ldr r0, [sp, #0x1c]
00375a6c: ldr r1, [pc, #0x78]
00375a70: str r2, [sp, #0x30]
00375a74: ldr r2, [r3, #0x164]
00375a78: add r7, sp, #0x30
00375a7c: ldr r1, [r0, r1]
00375a80: str r2, [sp, #0x34]
00375a84: ldr r3, [r3, #0x168]
00375a88: mov r0, r7
00375a8c: str r3, [sp, #0x38]
00375a90: bl #0x312b6c
00375a94: cmp r0, #0
00375a98: bne #0x375840
00375a9c: mov r0, r6
00375aa0: mov r1, r7
00375aa4: mov r2, #1
00375aa8: bl #0x393db4
00375aac: mov r0, r6
00375ab0: mov r1, r7
00375ab4: bl #0x393ae4
00375ab8: b #0x375840
00375abc: ldr r2, [pc, #0x14]
00375ac0: ldr r0, [fp, r2]
00375ac4: str r2, [sp, #0x10]
00375ac8: bl #0x31f594
00375acc: bl #0x3f0428
00375ad0: b #0x375698
00375ad4: rsbeq pc, r1, r8, lsl #8
00375ad8: strdeq r3, r4, [r0], -r4
00375adc: andeq r0, r0, r4, lsr #27
00375ae0: subseq fp, r4, r0, lsr #18
00375ae4: subseq ip, r4, r0, ror r0
00375ae8: subseq ip, r4, r4, ror #2
00375aec: andeq r3, r0, ip, lsr #30

# 0x375af0 _ZN13PlayerManager19_HandleGlobalDeathsEv
00375af0: push {r4, r5, r6, r7, r8, lr}
00375af4: ldr r6, [pc, #0x398]
00375af8: ldr r3, [r0, #0x714]
00375afc: sub sp, sp, #0x10
00375b00: mov r5, r0
00375b04: add r6, pc, r6
00375b08: cmp r3, #5
00375b0c: addls pc, pc, r3, lsl #2
00375b10: b #0x375b78
00375b14: b #0x375be4
00375b18: b #0x375b78
00375b1c: b #0x375c68
00375b20: b #0x375db4
00375b24: b #0x375dcc
00375b28: b #0x375b80
00375b2c: mov r0, r5
00375b30: mov r2, #1
00375b34: bl #0x36e478
00375b38: ldr r3, [r0, #0x660]
00375b3c: cmp r3, #0
00375b40: beq #0x375e7c
00375b44: movw r2, #0x14e8
00375b48: ldr r3, [r3, r2]
00375b4c: cmp r3, #0
00375b50: beq #0x375b78
00375b54: ldrb r2, [r3, #0x14]
00375b58: ldr r3, [pc, #0x338]
00375b5c: and r2, r2, #1
00375b60: ldr r1, [r6, r3]
00375b64: ldr r3, [r1, #0x38]
00375b68: ldrb r3, [r3, #0x160]
00375b6c: and r3, r2, r3
00375b70: cmp r3, #0
00375b74: bne #0x375dc0
00375b78: add sp, sp, #0x10
00375b7c: pop {r4, r5, r6, r7, r8, pc}
00375b80: ldr r3, [pc, #0x310]
00375b84: ldr r7, [pc, #0x310]
00375b88: ldr r4, [r6, r3]
00375b8c: add r7, pc, r7
00375b90: ldr r6, [r7, #4]
00375b94: mov r0, r4
00375b98: bl #0x31f66c
00375b9c: rsb r0, r0, r6
00375ba0: cmp r0, #0
00375ba4: str r0, [r7, #4]
00375ba8: bge #0x375b78
00375bac: ldr r1, [pc, #0x2ec]
00375bb0: ldr r0, [r4, #0x54]
00375bb4: add r1, pc, r1
00375bb8: bl #0x42e2b0
00375bbc: ldr r1, [pc, #0x2e0]
00375bc0: ldr r0, [r4, #0x54]
00375bc4: add r1, pc, r1
00375bc8: bl #0x431924
00375bcc: mov r0, r5
00375bd0: mov r1, #1
00375bd4: bl #0x37565c
00375bd8: mov r3, #0
00375bdc: strb r3, [r5, #0x718]
00375be0: b #0x375b78
00375be4: ldr r3, [pc, #0x2ac]
00375be8: ldr r4, [pc, #0x2b8]
00375bec: mov r7, #1
00375bf0: ldr r8, [r6, r3]
00375bf4: str r7, [r0, #0x714]
00375bf8: add r4, pc, r4
00375bfc: mov r1, r4
00375c00: ldr r0, [r8, #0x54]
00375c04: bl #0x42d1f0
00375c08: bl #0x41f3f4
00375c0c: cmp r0, #0
00375c10: bne #0x375c54
00375c14: bl #0x42ca8c
00375c18: ldr r3, [r0, #0xf4]
00375c1c: mov r1, r7
00375c20: mov r0, r3
00375c24: ldr r3, [r3]
00375c28: mov lr, pc
00375c2c: ldr pc, [r3, #0x38]
00375c30: ldr r0, [r8, #0x54]
00375c34: mov r1, r4
00375c38: bl #0x431924
00375c3c: ldr r3, [pc, #0x268]
00375c40: mov r1, r7
00375c44: mov r2, #0
00375c48: ldr r3, [r6, r3]
00375c4c: ldr r0, [r3]
00375c50: bl #0x369da0
00375c54: mov r3, #0
00375c58: strb r3, [r5, #0x711]
00375c5c: bl #0x41edd0
00375c60: bl #0x41d570
00375c64: b #0x375b78
00375c68: mov r1, #0
00375c6c: mov r2, r1
00375c70: bl #0x36e478
00375c74: ldr r3, [r0, #0x3a8]
00375c78: mov r4, r0
00375c7c: cmp r3, #0
00375c80: blt #0x375c9c
00375c84: mvn r1, #0
00375c88: bl #0x370bf4
00375c8c: mov r0, r4
00375c90: bl #0x80f23c
00375c94: subs r1, r0, #0
00375c98: beq #0x375de8
00375c9c: mov r4, #0
00375ca0: mov r7, #1
00375ca4: mov r0, r5
00375ca8: bl #0x36d7a8
00375cac: cmp r4, r0
00375cb0: mov r1, r4
00375cb4: mov r2, #0
00375cb8: mov r0, r5
00375cbc: bge #0x375d08
00375cc0: bl #0x36e744
00375cc4: ldr r3, [r0, #0x660]
00375cc8: mov r2, #0
00375ccc: mov r1, r4
00375cd0: cmp r3, r2
00375cd4: mov r0, r5
00375cd8: add r4, r4, #1
00375cdc: beq #0x375ca4
00375ce0: bl #0x36e744
00375ce4: ldr r3, [r0, #0x3a8]
00375ce8: mov r0, r5
00375cec: and r7, r7, r3, lsr #31
00375cf0: bl #0x36d7a8
00375cf4: cmp r4, r0
00375cf8: mov r1, r4
00375cfc: mov r2, #0
00375d00: mov r0, r5
00375d04: blt #0x375cc0
00375d08: cmp r7, r2
00375d0c: beq #0x375b78
00375d10: mov r3, #3
00375d14: mov r1, r2
00375d18: str r3, [r5, #0x714]
00375d1c: mov r2, #1
00375d20: bl #0x36e478
00375d24: ldr r7, [r0, #0x660]
00375d28: mov r0, r5
00375d2c: bl #0x36f074
00375d30: cmp r0, #0
00375d34: beq #0x375d48
00375d38: cmp r7, #0
00375d3c: beq #0x375d48
00375d40: mov r0, r7
00375d44: bl #0x3bb7d0
00375d48: mov r0, r5
00375d4c: bl #0x36f074
00375d50: cmp r0, #0
00375d54: beq #0x375b78
00375d58: ldr r4, [pc, #0x138]
00375d5c: ldr r0, [r6, r4]
00375d60: bl #0x31f594
00375d64: bl #0x3f0428
00375d68: cmp r7, #0
00375d6c: beq #0x375da4
00375d70: movw r3, #0x1474
00375d74: ldr r3, [r7, r3]
00375d78: mov r0, r7
00375d7c: add r1, sp, #4
00375d80: str r3, [sp, #4]
00375d84: movw r3, #0x1478
00375d88: ldr r3, [r7, r3]
00375d8c: mov r2, #1
00375d90: str r3, [sp, #8]
00375d94: movw r3, #0x147c
00375d98: ldr r3, [r7, r3]
00375d9c: str r3, [sp, #0xc]
00375da0: bl #0x393db4
00375da4: ldr r3, [r6, r4]
00375da8: ldr r0, [r3, #0x38]
00375dac: bl #0x347fd0
00375db0: b #0x375b78
00375db4: bl #0x36f074
00375db8: subs r1, r0, #0
00375dbc: beq #0x375b2c
00375dc0: mov r3, #4
00375dc4: str r3, [r5, #0x714]
00375dc8: b #0x375b78
00375dcc: ldr r3, [pc, #0xdc]
00375dd0: mov r2, #5
00375dd4: str r2, [r0, #0x714]
00375dd8: add r3, pc, r3
00375ddc: mov r2, #0x7d0
00375de0: str r2, [r3, #4]
00375de4: b #0x375b78
00375de8: mov r0, r5
00375dec: mov r2, #1
00375df0: bl #0x36e478
00375df4: ldr r7, [r0, #0x660]
00375df8: cmp r7, #0
00375dfc: beq #0x375e74
00375e00: ldr r4, [pc, #0x90]
00375e04: ldr r8, [r6, r4]
00375e08: mov r0, r8
00375e0c: bl #0x31f594
00375e10: bl #0x3ef614
00375e14: cmp r0, #0
00375e18: beq #0x375e38
00375e1c: mov r0, r8
00375e20: bl #0x31f594
00375e24: bl #0x3ef614
00375e28: mov r2, #1
00375e2c: add r1, r0, #0x160
00375e30: mov r0, r7
00375e34: bl #0x393db4
00375e38: mov r0, r7
00375e3c: bl #0x3bb7d0
00375e40: ldr r0, [r6, r4]
00375e44: mov r2, #0
00375e48: ldr r3, [r0, #0x38]
00375e4c: strb r2, [r3, #0x160]
00375e50: bl #0x31f594
00375e54: cmp r0, #0
00375e58: beq #0x375e64
00375e5c: ldr r0, [r0, #0x194]
00375e60: bl #0x47992c
00375e64: ldr r3, [r6, r4]
00375e68: ldr r0, [r3, #0x38]
00375e6c: bl #0x3406ac
00375e70: b #0x375c9c
00375e74: ldr r4, [pc, #0x1c]
00375e78: b #0x375e40
00375e7c: ldr r3, [pc, #0x14]
00375e80: ldr r3, [r6, r3]
00375e84: ldr r3, [r3, #0x38]
00375e88: ldrb r3, [r3, #0x160]
00375e8c: and r3, r3, #1
00375e90: b #0x375b70
00375e94: rsbeq lr, r1, ip, lsl #31
00375e98: strdeq r3, r4, [r0], -r4

# 0x375eb4 _ZN13PlayerManager18_CheckGlobalDeathsEv
00375eb4: ldr r3, [pc, #0x120]
00375eb8: ldr r2, [pc, #0x120]
00375ebc: push {r4, r5, r6, r7, r8, lr}
00375ec0: add r3, pc, r3
00375ec4: mov r5, r0
00375ec8: ldr r0, [r3, r2]
00375ecc: bl #0x31f594
00375ed0: cmp r0, #0
00375ed4: beq #0x375ee4
00375ed8: ldr r3, [r0, #0x130]
00375edc: cmp r3, #0x26
00375ee0: beq #0x375ee8
00375ee4: pop {r4, r5, r6, r7, r8, pc}
00375ee8: bl #0x7fd794
00375eec: ldrb r3, [r0, #5]
00375ef0: cmp r3, #0
00375ef4: beq #0x375ee4
00375ef8: mov r0, r5
00375efc: bl #0x36d7a8
00375f00: mov r4, #0
00375f04: subs r6, r0, #0
00375f08: movne r6, #1
00375f0c: mov r0, r5
00375f10: bl #0x36d7a8
00375f14: cmp r4, r0
00375f18: mov r1, r4
00375f1c: mov r2, #0
00375f20: mov r0, r5
00375f24: bge #0x375f84
00375f28: bl #0x36e744
00375f2c: ldr r3, [r0, #0x660]
00375f30: subs r0, r3, #0
00375f34: moveq r6, r3
00375f38: beq #0x375f54
00375f3c: ldr r3, [r3]
00375f40: mov lr, pc
00375f44: ldr pc, [r3, #0x34]
00375f48: subs r7, r0, #0
00375f4c: andne r6, r6, #1
00375f50: beq #0x375f5c
00375f54: add r4, r4, #1
00375f58: b #0x375f0c
00375f5c: mov r1, r4
00375f60: mov r0, r5
00375f64: mov r2, r7
00375f68: bl #0x36e744
00375f6c: ldr r3, [r0, #0x3a8]
00375f70: and r6, r6, #1
00375f74: add r4, r4, #1
00375f78: cmp r3, #0
00375f7c: movlt r6, r7
00375f80: b #0x375f0c
00375f84: cmp r6, r2
00375f88: beq #0x375fc0
00375f8c: ldr r3, [r5, #0x714]
00375f90: cmp r3, #0
00375f94: bne #0x375fcc
00375f98: bl #0x80b1bc
00375f9c: mov r4, r0
00375fa0: ldr r0, [pc, #0x3c]
00375fa4: mov r1, #1
00375fa8: add r0, pc, r0
00375fac: bl #0x80a244
00375fb0: mov r1, r0
00375fb4: mov r0, r4
00375fb8: bl #0x80e2a4
00375fbc: b #0x375fcc
00375fc0: ldr r3, [r5, #0x714]
00375fc4: cmp r3, r2
00375fc8: beq #0x375fd8
00375fcc: mov r0, r5
00375fd0: pop {r4, r5, r6, r7, r8, lr}
00375fd4: b #0x375af0
00375fd8: pop {r4, r5, r6, r7, r8, pc}

# 0x375fe8 _ZN13PlayerManager18_HandleLocalDeathsEv
00375fe8: push {r4, r5, r6, r7, r8, lr}
00375fec: mov r5, r0
00375ff0: bl #0x7fd794
00375ff4: ldrb r3, [r0, #5]
00375ff8: ldr r4, [pc, #0xf0]
00375ffc: cmp r3, #0
00376000: add r4, pc, r4
00376004: beq #0x376058
00376008: ldr r6, [r5, #0x714]
0037600c: cmp r6, #0
00376010: bne #0x376080
00376014: ldrb r3, [r5, #0x711]
00376018: cmp r3, #0
0037601c: beq #0x3760d4
00376020: mov r1, r6
00376024: mov r2, r6
00376028: mov r0, r5
0037602c: bl #0x36e478
00376030: ldr r3, [pc, #0xbc]
00376034: ldr r7, [r0, #0x3a8]
00376038: mov r8, r0
0037603c: ldr r0, [r4, r3]
00376040: bl #0x31f66c
00376044: subs r1, r7, r0
00376048: bmi #0x3760ac
0037604c: mov r0, r8
00376050: pop {r4, r5, r6, r7, r8, lr}
00376054: b #0x370bf4
00376058: ldr r3, [pc, #0x94]
0037605c: ldr r5, [pc, #0x94]
00376060: ldr r6, [r4, r3]
00376064: add r5, pc, r5
00376068: mov r1, r5
0037606c: ldr r0, [r6, #0x54]
00376070: bl #0x42d1f0
00376074: bl #0x41f3f4
00376078: cmp r0, #0
0037607c: beq #0x376084
00376080: pop {r4, r5, r6, r7, r8, pc}
00376084: ldr r0, [r6, #0x54]
00376088: mov r1, r5
0037608c: bl #0x431924
00376090: ldr r3, [pc, #0x64]
00376094: mov r1, #1
00376098: mov r2, #0
0037609c: ldr r3, [r4, r3]
003760a0: ldr r0, [r3]
003760a4: pop {r4, r5, r6, r7, r8, lr}
003760a8: b #0x369da0
003760ac: mvn r1, #0
003760b0: mov r0, r8
003760b4: strb r6, [r5, #0x711]
003760b8: bl #0x370bf4
003760bc: bl #0x41edd0
003760c0: bl #0x41d570
003760c4: mov r0, r5
003760c8: mov r1, r6
003760cc: pop {r4, r5, r6, r7, r8, lr}
003760d0: b #0x37565c
003760d4: mov r3, #1
003760d8: strb r3, [r5, #0x711]
003760dc: mov r0, r5
003760e0: bl #0x370c9c
003760e4: bl #0x41edd0
003760e8: pop {r4, r5, r6, r7, r8, lr}
003760ec: b #0x41d658
003760f0: mlseq r1, r0, sl, lr
003760f4: strdeq r3, r4, [r0], -r4
003760f8: subseq r8, r4, r4, asr #25
003760fc: andeq r0, r0, r4, lsr #27

# 0x376100 _ZN13PlayerManager17_CheckLocalDeathsEv
00376100: ldr r3, [pc, #0x78]
00376104: ldr r2, [pc, #0x78]
00376108: push {r4, lr}
0037610c: add r3, pc, r3
00376110: mov r4, r0
00376114: ldr r0, [r3, r2]
00376118: bl #0x31f594
0037611c: cmp r0, #0
00376120: beq #0x376130
00376124: ldr r3, [r0, #0x130]
00376128: cmp r3, #0x26
0037612c: beq #0x376134
00376130: pop {r4, pc}
00376134: mov r1, #0
00376138: mov r0, r4
0037613c: mov r2, r1
00376140: bl #0x36e478
00376144: ldr r3, [r0, #0x660]
00376148: cmp r3, #0
0037614c: beq #0x376130
00376150: mov r0, r3
00376154: ldr r3, [r3]
00376158: mov lr, pc
0037615c: ldr pc, [r3, #0x34]
00376160: cmp r0, #0
00376164: beq #0x376130
00376168: ldr r3, [r4, #0x6c4]
0037616c: cmp r3, #0
00376170: ble #0x376130
00376174: mov r0, r4
00376178: pop {r4, lr}
0037617c: b #0x375fe8
00376180: rsbeq lr, r1, r4, lsl #19
00376184: strdeq r3, r4, [r0], -r4

# 0x376380 _ZN13PlayerManager25OnHostChangedNotificationEi
00376380: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00376384: ldr r2, [pc, #0x4e0]
00376388: sub sp, sp, #0x4a0
0037638c: sub sp, sp, #0xc
00376390: str r2, [sp, #0x10]
00376394: ldr r4, [pc, #0x4d4]
00376398: ldr ip, [sp, #0x10]
0037639c: mov r2, #0
003763a0: add r4, pc, r4
003763a4: ldr r3, [r4, ip]
003763a8: mov sb, r0
003763ac: ldr r3, [r3]
003763b0: str r3, [sp, #0x4a4]
003763b4: bl #0x36dfb0
003763b8: mov r6, r0
003763bc: bl #0x80f23c
003763c0: cmp r0, #0
003763c4: bne #0x3763e8
003763c8: ldr r3, [pc, #0x4a4]
003763cc: ldr r3, [r4, r3]
003763d0: ldr r3, [r3]
003763d4: cmp r3, #2
003763d8: streq r0, [r0]
003763dc: beq #0x3763e8
003763e0: cmp r3, #1
003763e4: beq #0x3767f8
003763e8: ldr r7, [pc, #0x488]
003763ec: ldr r0, [r4, r7]
003763f0: bl #0x31f594
003763f4: cmp r0, #0
003763f8: movne r3, #1
003763fc: strbne r3, [sb, #0x719]
00376400: ldr r3, [r6]
00376404: mov r0, r6
00376408: mov lr, pc
0037640c: ldr pc, [r3, #0x50]
00376410: cmp r0, #0
00376414: beq #0x376424
00376418: ldrb r5, [r6, #0x4e5]
0037641c: cmp r5, #0
00376420: beq #0x376648
00376424: ldr fp, [r6, #0x660]
00376428: cmp fp, #0
0037642c: beq #0x37674c
00376430: add r5, sp, #0x3e0
00376434: mov r0, r5
00376438: mov r1, #0x10
0037643c: str r5, [sp, #0x3f0]
00376440: str r5, [sp, #0x3f4]
00376444: bl #0x31167c
00376448: ldr r3, [sp, #0x3f0]
0037644c: add r8, sp, #0x480
00376450: add r8, r8, #0xc
00376454: mov sl, #0
00376458: strb sl, [r3]
0037645c: mov r0, r8
00376460: mov r1, #0x10
00376464: str r8, [sp, #0x49c]
00376468: str r8, [sp, #0x4a0]
0037646c: bl #0x31167c
00376470: ldr r3, [sp, #0x49c]
00376474: ldr r7, [r4, r7]
00376478: ldr r1, [pc, #0x3fc]
0037647c: strb sl, [r3]
00376480: ldr r2, [pc, #0x3f8]
00376484: ldr r3, [r7, #0x34]
00376488: ldr r0, [r7, #0x2c]
0037648c: add r2, pc, r2
00376490: add r1, pc, r1
00376494: str r3, [sp, #0xc]
00376498: bl #0x4c4bdc
0037649c: ldr r3, [sp, #0xc]
003764a0: mov r1, r0
003764a4: mov r0, r3
003764a8: bl #0x508edc
003764ac: ldr r7, [r7, #0x34]
003764b0: mov ip, r0
003764b4: mov r0, fp
003764b8: str ip, [sp, #0xc]
003764bc: str r7, [sp, #0x14]
003764c0: bl #0x3bb7e8
003764c4: add fp, sp, #0x470
003764c8: add fp, fp, #4
003764cc: add r7, sp, #0x450
003764d0: mov r1, r0
003764d4: add r7, r7, #0xc
003764d8: add r2, sp, #0x3dc
003764dc: mov r0, fp
003764e0: bl #0x3140ec
003764e4: mov r3, sl
003764e8: mov r0, r7
003764ec: ldr r1, [sp, #0x14]
003764f0: mov r2, fp
003764f4: bl #0x507c0c
003764f8: ldr ip, [sp, #0xc]
003764fc: ldr r3, [sp, #0x470]
00376500: mov r1, r8
00376504: mov r2, ip
00376508: ldr r0, [sp, #0x14]
0037650c: bl #0x508ef4
00376510: mov r0, r7
00376514: ldr r7, [pc, #0x368]
00376518: bl #0x318254
0037651c: mov r0, fp
00376520: bl #0x318254
00376524: ldr r2, [sp, #0x49c]
00376528: ldr r1, [sp, #0x4a0]
0037652c: mov r0, r5
00376530: bl #0x3109e0
00376534: ldr r7, [r4, r7]
00376538: mov r1, r5
0037653c: add sl, r7, #4
00376540: mov r0, sl
00376544: bl #0x376338
00376548: ldm sl, {r0, r1, r2, r3}
0037654c: add ip, sp, #0x3b4
00376550: stm ip, {r0, r1, r2, r3}
00376554: add r0, r7, #0x14
00376558: mov r1, ip
0037655c: bl #0x36d314
00376560: cmp r0, #1
00376564: beq #0x376664
00376568: bl #0x800f8c
0037656c: ldr r3, [r0]
00376570: mov lr, pc
00376574: ldr pc, [r3, #0xa4]
00376578: add r7, sp, #0x18
0037657c: add fp, sp, #0x440
00376580: mov r1, r0
00376584: add r6, r6, #0x2d0
00376588: add fp, fp, #4
0037658c: mov r0, r7
00376590: add sl, sp, #0x420
00376594: bl #0x819050
00376598: add sl, sl, #0xc
0037659c: mov r1, r6
003765a0: mov r0, fp
003765a4: bl #0x32b918
003765a8: mov r1, r6
003765ac: mov r0, sl
003765b0: ldr r6, [sp, #0x458]
003765b4: bl #0x32b918
003765b8: ldr r0, [sp, #0x43c]
003765bc: ldr r3, [sp, #0x440]
003765c0: mov r2, r6
003765c4: mov r1, #3
003765c8: rsb r3, r3, r0
003765cc: mov r0, r7
003765d0: bl #0x818cb0
003765d4: mov r0, sl
003765d8: bl #0x318254
003765dc: mov r0, fp
003765e0: bl #0x318254
003765e4: bl #0x800f8c
003765e8: ldr r3, [r0]
003765ec: mov lr, pc
003765f0: ldr pc, [r3, #0xa4]
003765f4: mov r1, r7
003765f8: bl #0x81892c
003765fc: mov r0, r7
00376600: bl #0x818c14
00376604: mov r0, r8
00376608: bl #0x318254
0037660c: mov r0, r5
00376610: bl #0x318254
00376614: ldr r3, [sb, #0x714]
00376618: cmp r3, #3
0037661c: moveq r3, #2
00376620: streq r3, [sb, #0x714]
00376624: ldr r2, [sp, #0x10]
00376628: ldr r3, [r4, r2]
0037662c: ldr r2, [sp, #0x4a4]
00376630: ldr r3, [r3]
00376634: cmp r2, r3
00376638: bne #0x376868
0037663c: add sp, sp, #0xac
00376640: add sp, sp, #0x400
00376644: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00376648: mov r0, r6
0037664c: mov r1, r5
00376650: bl #0x370908
00376654: mvn r3, #0
00376658: str r3, [sb, #0x6cc]
0037665c: strb r5, [sb, #0x6cb]
00376660: b #0x376424
00376664: ldr r3, [pc, #0x21c]
00376668: ldr r3, [r4, r3]
0037666c: ldr fp, [r3]
00376670: bl #0x42ca8c
00376674: bl #0x42cb8c
00376678: subs sl, r0, #0
0037667c: beq #0x376568
00376680: ldr r7, [pc, #0x204]
00376684: ldr r3, [r4, r7]
00376688: ldr r2, [r3, #0x2c]
0037668c: cmp r2, #0
00376690: beq #0x3766cc
00376694: ldr r0, [r3, #0x28]
00376698: ldrb r3, [r0, #4]
0037669c: cmp r3, #0
003766a0: bne #0x3766e8
003766a4: ldr r1, [r0]
003766a8: sub r1, r1, #1
003766ac: cmp r1, #0
003766b0: str r1, [r0]
003766b4: bne #0x3766bc
003766b8: bl #0x752b38
003766bc: ldr r3, [r4, r7]
003766c0: mov r2, #0
003766c4: str r2, [r3, #0x2c]
003766c8: str r2, [r3, #0x28]
003766cc: ldr r3, [pc, #0x1bc]
003766d0: ldr r0, [r4, r7]
003766d4: mov r2, sl
003766d8: ldr r1, [r4, r3]
003766dc: mov r3, #0
003766e0: ldr r1, [r1]
003766e4: bl #0x427ca0
003766e8: ldr r0, [r4, r7]
003766ec: bl #0x427d50
003766f0: mov r2, #0
003766f4: mov r1, r0
003766f8: mov r3, #0
003766fc: add r0, sp, #0x3d0
00376700: mov ip, #0
00376704: strd r2, r3, [r0]
00376708: strb ip, [sp, #0x3c4]
0037670c: mov ip, #2
00376710: strb ip, [sp, #0x3c5]
00376714: mov ip, #0
00376718: str ip, [sp, #0x3c8]
0037671c: ldr ip, [sp, #0x3d4]
00376720: add r7, sp, #0x3c4
00376724: mov r0, sl
00376728: str ip, [r7, #8]
0037672c: mov r2, fp
00376730: mov ip, #1
00376734: mov r3, r7
00376738: str ip, [sp]
0037673c: bl #0x7abe0c
00376740: mov r0, r7
00376744: bl #0x797124
00376748: b #0x376568
0037674c: mov r0, sb
00376750: bl #0x36f074
00376754: cmp r0, #0
00376758: bne #0x37682c
0037675c: bl #0x800f8c
00376760: ldr r3, [r0]
00376764: mov lr, pc
00376768: ldr pc, [r3, #0xa4]
0037676c: add r5, sp, #0x18
00376770: add r8, sp, #0x410
00376774: mov r1, r0
00376778: add r6, r6, #0x2d0
0037677c: add r8, r8, #4
00376780: mov r0, r5
00376784: bl #0x819050
00376788: add r7, sp, #0x3fc
0037678c: mov r1, r6
00376790: mov r0, r8
00376794: bl #0x32b918
00376798: mov r1, r6
0037679c: mov r0, r7
003767a0: ldr r6, [sp, #0x428]
003767a4: bl #0x32b918
003767a8: ldr r0, [sp, #0x40c]
003767ac: ldr r3, [sp, #0x410]
003767b0: mov r2, r6
003767b4: mov r1, #3
003767b8: rsb r3, r3, r0
003767bc: mov r0, r5
003767c0: bl #0x818cb0
003767c4: mov r0, r7
003767c8: bl #0x318254
003767cc: mov r0, r8
003767d0: bl #0x318254
003767d4: bl #0x800f8c
003767d8: ldr r3, [r0]
003767dc: mov lr, pc
003767e0: ldr pc, [r3, #0xa4]
003767e4: mov r1, r5
003767e8: bl #0x81892c
003767ec: mov r0, r5
003767f0: bl #0x818c14
003767f4: b #0x376614
003767f8: ldr r0, [pc, #0x94]
003767fc: ldr r1, [pc, #0x94]
00376800: ldr r2, [pc, #0x94]
00376804: ldr r0, [r4, r0]
00376808: ldr r3, [pc, #0x90]
0037680c: movw ip, #0x82f
00376810: add r1, pc, r1
00376814: add r2, pc, r2
00376818: add r3, pc, r3
0037681c: add r0, r0, #0xa8
00376820: str ip, [sp]
00376824: bl #0x30e004
00376828: b #0x3763e8
0037682c: ldr r5, [r4, r7]
00376830: ldr r0, [r5, #0x54]
00376834: bl #0x42cb80
00376838: cmp r0, #0
0037683c: beq #0x37675c
00376840: ldr r0, [r5, #0x54]
00376844: bl #0x42cb80
00376848: ldr r1, [pc, #0x54]
0037684c: ldr r2, [pc, #0x54]
00376850: mov r3, fp
00376854: add r1, pc, r1
00376858: add r2, pc, r2
0037685c: str fp, [sp]
00376860: bl #0x7ad7e8
00376864: b #0x37675c
00376868: bl #0x30e310
0037686c: andeq r4, r0, ip, lsr #1

# 0x3777d0 _ZN10PlayerInfoC1ERKS_
003777d0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003777d4: ldr r7, [pc, #0x71c]
003777d8: sub sp, sp, #0x2c
003777dc: mov r4, r0
003777e0: mov r5, r1
003777e4: bl #0x377448
003777e8: ldr r2, [pc, #0x70c]
003777ec: ldr r3, [pc, #0x70c]
003777f0: add r7, pc, r7
003777f4: ldr r6, [r7, r2]
003777f8: ldr r3, [r7, r3]
003777fc: mov r2, #0x290
00377800: add r6, r6, #8
00377804: add r3, r3, #8
00377808: str r6, [r4, #0x288]
0037780c: str r3, [r4]
00377810: ldr r3, [r5, #0x28c]
00377814: ldr ip, [pc, #0x6e8]
00377818: str r3, [r4, #0x28c]
0037781c: ldrd r0, r1, [r5, r2]
00377820: strd r0, r1, [r4, r2]
00377824: ldr r3, [r5, #0x298]
00377828: ldr r2, [pc, #0x6d8]
0037782c: ldr ip, [r7, ip]
00377830: str r3, [r4, #0x298]
00377834: ldr r3, [r5, #0x29c]
00377838: ldr r8, [r7, r2]
0037783c: add ip, ip, #8
00377840: str r3, [r4, #0x29c]
00377844: ldr r1, [r5, #0x2a0]
00377848: add r8, r8, #8
0037784c: mov r2, #0x2b8
00377850: str r1, [r4, #0x2a0]
00377854: ldrb r0, [r5, #0x2a4]
00377858: str r8, [r4, #0x288]
0037785c: ldr r3, [pc, #0x6a8]
00377860: strb r0, [r4, #0x2a4]
00377864: ldr r0, [r5, #0x2a8]
00377868: str r6, [r4, #0x2b0]
0037786c: str ip, [r4, #0x288]
00377870: str r0, [r4, #0x2a8]
00377874: ldr ip, [r5, #0x2b4]
00377878: add r1, r5, #0x2d0
0037787c: add r0, r4, #0x2d0
00377880: str ip, [r4, #0x2b4]
00377884: ldrd sl, fp, [r5, r2]
00377888: strd sl, fp, [r4, r2]
0037788c: ldr r2, [r5, #0x2c0]
00377890: str r2, [r4, #0x2c0]
00377894: ldr r2, [r5, #0x2c4]
00377898: str r2, [r4, #0x2c4]
0037789c: ldr r2, [r5, #0x2c8]
003778a0: str r2, [r4, #0x2c8]
003778a4: ldrb r2, [r5, #0x2cc]
003778a8: strb r2, [r4, #0x2cc]
003778ac: ldr r3, [r7, r3]
003778b0: add r3, r3, #8
003778b4: str r3, [r4, #0x2b0]
003778b8: bl #0x32b918
003778bc: ldr r3, [pc, #0x64c]
003778c0: str r6, [r4, #0x2e8]
003778c4: mov r2, #0x2f0
003778c8: ldr r3, [r7, r3]
003778cc: ldr r1, [pc, #0x640]
003778d0: mov ip, #0x318
003778d4: add r3, r3, #8
003778d8: str r3, [r4, #0x2b0]
003778dc: ldr r3, [r5, #0x2ec]
003778e0: ldr sb, [r7, r1]
003778e4: mov r1, #0x340
003778e8: str r3, [r4, #0x2ec]
003778ec: ldrd sl, fp, [r5, r2]
003778f0: strd sl, fp, [r4, r2]
003778f4: ldr r3, [r5, #0x2f8]
003778f8: ldr fp, [pc, #0x618]
003778fc: add sb, sb, #8
00377900: str r3, [r4, #0x2f8]
00377904: ldr r0, [r5, #0x2fc]
00377908: mov r2, #0x368
0037790c: mov r3, #0x390
00377910: str r0, [r4, #0x2fc]
00377914: ldr lr, [r5, #0x300]
00377918: str fp, [sp, #0x1c]
0037791c: add r0, r4, #0x3d0
00377920: str lr, [r4, #0x300]
00377924: ldrb lr, [r5, #0x304]
00377928: str r8, [r4, #0x2e8]
0037792c: strb lr, [r4, #0x304]
00377930: ldr lr, [r5, #0x308]
00377934: str r6, [r4, #0x310]
00377938: str sb, [r4, #0x2e8]
0037793c: str lr, [r4, #0x308]
00377940: ldr lr, [r5, #0x314]
00377944: str lr, [r4, #0x314]
00377948: ldrd sl, fp, [r5, ip]
0037794c: strd sl, fp, [r4, ip]
00377950: ldr ip, [r5, #0x320]
00377954: str ip, [r4, #0x320]
00377958: ldr ip, [r5, #0x324]
0037795c: str ip, [r4, #0x324]
00377960: ldr ip, [r5, #0x328]
00377964: str ip, [r4, #0x328]
00377968: ldrb ip, [r5, #0x32c]
0037796c: strb ip, [r4, #0x32c]
00377970: str r8, [r4, #0x310]
00377974: ldr ip, [r5, #0x330]
00377978: str r6, [r4, #0x338]
0037797c: str sb, [r4, #0x310]
00377980: str ip, [r4, #0x330]
00377984: ldr ip, [r5, #0x33c]
00377988: str ip, [r4, #0x33c]
0037798c: ldrd sl, fp, [r5, r1]
00377990: strd sl, fp, [r4, r1]
00377994: ldr r1, [r5, #0x348]
00377998: str r1, [r4, #0x348]
0037799c: ldr r1, [r5, #0x34c]
003779a0: str r1, [r4, #0x34c]
003779a4: ldr r1, [r5, #0x350]
003779a8: str r1, [r4, #0x350]
003779ac: ldrb r1, [r5, #0x354]
003779b0: str r8, [r4, #0x338]
003779b4: strb r1, [r4, #0x354]
003779b8: ldr r1, [r5, #0x358]
003779bc: str r6, [r4, #0x360]
003779c0: str sb, [r4, #0x338]
003779c4: str r1, [r4, #0x358]
003779c8: ldr r1, [r5, #0x364]
003779cc: str r1, [r4, #0x364]
003779d0: ldrd sl, fp, [r5, r2]
003779d4: strd sl, fp, [r4, r2]
003779d8: ldr r2, [r5, #0x370]
003779dc: mov r1, #0x3b8
003779e0: str r2, [r4, #0x370]
003779e4: ldr r2, [r5, #0x374]
003779e8: str r2, [r4, #0x374]
003779ec: ldr r2, [r5, #0x378]
003779f0: str r2, [r4, #0x378]
003779f4: ldrb r2, [r5, #0x37c]
003779f8: strb r2, [r4, #0x37c]
003779fc: str r8, [r4, #0x360]
00377a00: ldr r2, [r5, #0x380]
00377a04: str r6, [r4, #0x388]
00377a08: str sb, [r4, #0x360]
00377a0c: str r2, [r4, #0x380]
00377a10: ldr fp, [sp, #0x1c]
00377a14: ldr r2, [r5, #0x38c]
00377a18: ldr fp, [r7, fp]
00377a1c: str fp, [sp, #0x1c]
00377a20: str r2, [r4, #0x38c]
00377a24: ldrd sl, fp, [r5, r3]
00377a28: strd sl, fp, [r4, r3]
00377a2c: ldr r3, [r5, #0x398]
00377a30: ldr ip, [sp, #0x1c]
00377a34: mov sl, #0
00377a38: str r3, [r4, #0x398]
00377a3c: ldr r3, [r5, #0x39c]
00377a40: add fp, ip, #8
00377a44: str r3, [r4, #0x39c]
00377a48: ldr r3, [r5, #0x3a0]
00377a4c: str r3, [r4, #0x3a0]
00377a50: ldrb r3, [r5, #0x3a4]
00377a54: str r8, [r4, #0x388]
00377a58: strb r3, [r4, #0x3a4]
00377a5c: ldr r3, [r5, #0x3a8]
00377a60: str r6, [r4, #0x3b0]
00377a64: str sb, [r4, #0x388]
00377a68: str r3, [r4, #0x3a8]
00377a6c: ldr r3, [r5, #0x3b4]
00377a70: str r3, [r4, #0x3b4]
00377a74: ldrd r2, r3, [r5, r1]
00377a78: strd r2, r3, [r4, r1]
00377a7c: ldr r3, [r5, #0x3c0]
00377a80: mov r2, #0
00377a84: str r3, [r4, #0x3c0]
00377a88: ldr r3, [r5, #0x3c4]
00377a8c: str r3, [r4, #0x3c4]
00377a90: ldr r3, [r5, #0x3c8]
00377a94: str r3, [r4, #0x3c8]
00377a98: ldrb r3, [r5, #0x3cc]
00377a9c: str fp, [r4, #0x3b0]
00377aa0: str r2, [r4, #0x3d0]
00377aa4: strb r3, [r4, #0x3cc]
00377aa8: str r2, [r4, #0x3d4]
00377aac: ldr r1, [r5, #0x3d0]
00377ab0: ldr r2, [r5, #0x3d4]
00377ab4: bl #0x36ddac
00377ab8: ldr r3, [pc, #0x45c]
00377abc: str r6, [r4, #0x3d8]
00377ac0: mov r1, #0x3e0
00377ac4: ldr r3, [r7, r3]
00377ac8: add r0, r4, #0x3f8
00377acc: add r3, r3, #8
00377ad0: str r3, [r4, #0x3b0]
00377ad4: ldr r3, [r5, #0x3dc]
00377ad8: str r3, [r4, #0x3dc]
00377adc: ldrd r2, r3, [r5, r1]
00377ae0: strd r2, r3, [r4, r1]
00377ae4: ldr r3, [r5, #0x3e8]
00377ae8: str r3, [r4, #0x3e8]
00377aec: ldr r3, [r5, #0x3ec]
00377af0: str r3, [r4, #0x3ec]
00377af4: ldr r3, [r5, #0x3f0]
00377af8: str r3, [r4, #0x3f0]
00377afc: ldrb r3, [r5, #0x3f4]
00377b00: str fp, [r4, #0x3d8]
00377b04: str sl, [r4, #0x3f8]
00377b08: strb r3, [r4, #0x3f4]
00377b0c: str sl, [r4, #0x3fc]
00377b10: ldr r1, [r5, #0x3f8]
00377b14: ldr r2, [r5, #0x3fc]
00377b18: bl #0x36ddac
00377b1c: ldr r3, [pc, #0x3fc]
00377b20: str r6, [r4, #0x400]
00377b24: movw r1, #0x408
00377b28: ldr r3, [r7, r3]
00377b2c: add r0, r4, #0x420
00377b30: add r3, r3, #8
00377b34: str r3, [r4, #0x3d8]
00377b38: ldr r3, [r5, #0x404]
00377b3c: str r3, [r4, #0x404]
00377b40: ldrd r2, r3, [r5, r1]
00377b44: strd r2, r3, [r4, r1]
00377b48: ldr r3, [r5, #0x410]
00377b4c: str r3, [r4, #0x410]
00377b50: ldr r3, [r5, #0x414]
00377b54: str r3, [r4, #0x414]
00377b58: ldr r3, [r5, #0x418]
00377b5c: str r3, [r4, #0x418]
00377b60: ldrb r3, [r5, #0x41c]
00377b64: str fp, [r4, #0x400]
00377b68: str sl, [r4, #0x424]
00377b6c: strb r3, [r4, #0x41c]
00377b70: str sl, [r4, #0x420]
00377b74: ldr r1, [r5, #0x420]
00377b78: ldr r2, [r5, #0x424]
00377b7c: bl #0x36ddac
00377b80: ldr r2, [pc, #0x39c]
00377b84: str r6, [r4, #0x428]
00377b88: mov r1, #0x430
00377b8c: ldr r2, [r7, r2]
00377b90: ldr r0, [pc, #0x390]
00377b94: ldr fp, [pc, #0x390]
00377b98: add r2, r2, #8
00377b9c: str r2, [r4, #0x400]
00377ba0: ldr r2, [r5, #0x42c]
00377ba4: ldr fp, [r7, fp]
00377ba8: movw ip, #0x458
00377bac: str r2, [r4, #0x42c]
00377bb0: ldrd r2, r3, [r5, r1]
00377bb4: strd r2, r3, [r4, r1]
00377bb8: ldr r2, [r5, #0x438]
00377bbc: ldr r3, [pc, #0x36c]
00377bc0: add fp, fp, #8
00377bc4: str r2, [r4, #0x438]
00377bc8: ldr r2, [r5, #0x43c]
00377bcc: str r3, [sp, #0x1c]
00377bd0: mov sl, #0x480
00377bd4: str r2, [r4, #0x43c]
00377bd8: ldr r2, [r5, #0x440]
00377bdc: str r0, [sp, #0x24]
00377be0: str r2, [r4, #0x440]
00377be4: ldrb r2, [r5, #0x444]
00377be8: str r8, [r4, #0x428]
00377bec: strb r2, [r4, #0x444]
00377bf0: ldr r2, [r5, #0x448]
00377bf4: str fp, [r4, #0x428]
00377bf8: str r6, [r4, #0x450]
00377bfc: str r2, [r4, #0x448]
00377c00: ldr r1, [r5, #0x454]
00377c04: add r2, r5, #0x540
00377c08: add r2, r2, #8
00377c0c: str r1, [r4, #0x454]
00377c10: ldrd r0, r1, [r5, ip]
00377c14: strd r0, r1, [r4, ip]
00377c18: ldr r3, [r5, #0x460]
00377c1c: str r2, [sp, #0xc]
00377c20: mov r0, r8
00377c24: str r3, [r4, #0x460]
00377c28: ldr r3, [r5, #0x464]
00377c2c: str r6, [sp, #4]
00377c30: add r1, r4, #0x680
00377c34: str r3, [r4, #0x464]
00377c38: ldr r3, [r5, #0x468]
00377c3c: add r1, r1, #8
00377c40: str r3, [r4, #0x468]
00377c44: ldrb r2, [r5, #0x46c]
00377c48: add r3, r4, #0x570
00377c4c: str r3, [sp, #8]
00377c50: str r8, [r4, #0x450]
00377c54: strb r2, [r4, #0x46c]
00377c58: ldr r3, [r5, #0x470]
00377c5c: str r6, [r4, #0x478]
00377c60: str fp, [r4, #0x450]
00377c64: str r3, [r4, #0x470]
00377c68: ldr r3, [r5, #0x47c]
00377c6c: str r3, [r4, #0x47c]
00377c70: ldrd r2, r3, [r5, sl]
00377c74: strd r2, r3, [r4, sl]
00377c78: ldr sl, [r5, #0x488]
00377c7c: movw r3, #0x4a8
00377c80: str sl, [r4, #0x488]
00377c84: ldr sl, [r5, #0x48c]
00377c88: str sl, [r4, #0x48c]
00377c8c: ldr sl, [r5, #0x490]
00377c90: str sl, [r4, #0x490]
00377c94: ldrb sl, [r5, #0x494]
00377c98: str r8, [r4, #0x478]
00377c9c: strb sl, [r4, #0x494]
00377ca0: ldr sl, [r5, #0x498]
00377ca4: str fp, [r4, #0x478]
00377ca8: str r6, [r4, #0x4a0]
00377cac: str sl, [r4, #0x498]
00377cb0: ldr sl, [r5, #0x4a4]
00377cb4: str sl, [r4, #0x4a4]
00377cb8: movw sl, #0x4a8
00377cbc: ldrd r2, r3, [r3, r5]
00377cc0: strd r2, r3, [r4, sl]
00377cc4: ldr sl, [r5, #0x4b0]
00377cc8: mov r2, #0x4d0
00377ccc: str sl, [r4, #0x4b0]
00377cd0: ldr sl, [r5, #0x4b4]
00377cd4: str sl, [r4, #0x4b4]
00377cd8: ldr sl, [r5, #0x4b8]
00377cdc: str sl, [r4, #0x4b8]
00377ce0: ldrb sl, [r5, #0x4bc]
00377ce4: str r8, [r4, #0x4a0]
00377ce8: strb sl, [r4, #0x4bc]
00377cec: ldr r8, [r5, #0x4c0]
00377cf0: str r6, [r4, #0x4c8]
00377cf4: str fp, [r4, #0x4a0]
00377cf8: str r8, [r4, #0x4c0]
00377cfc: ldr ip, [sp, #0x24]
00377d00: ldr fp, [sp, #0x1c]
00377d04: ldr r8, [r5, #0x4cc]
00377d08: ldr fp, [r7, fp]
00377d0c: ldr r7, [r7, ip]
00377d10: mov ip, #0x4f0
00377d14: str fp, [sp, #0x1c]
00377d18: str r7, [sp, #0x24]
00377d1c: str r8, [r4, #0x4cc]
00377d20: ldrd sl, fp, [r5, r2]
00377d24: strd sl, fp, [r4, r2]
00377d28: ldr sl, [r5, #0x4d8]
00377d2c: ldr r3, [sp, #0x1c]
00377d30: ldr fp, [sp, #0x24]
00377d34: str sl, [r4, #0x4d8]
00377d38: ldr sl, [r5, #0x4dc]
00377d3c: add r8, r3, #8
00377d40: add r7, fp, #8
00377d44: str sl, [r4, #0x4dc]
00377d48: ldr sl, [r5, #0x4e0]
00377d4c: mov r2, #0x510
00377d50: mov r3, #0x530
00377d54: str sl, [r4, #0x4e0]
00377d58: ldrb sl, [r5, #0x4e4]
00377d5c: str r8, [r4, #0x4c8]
00377d60: strb sl, [r4, #0x4e4]
00377d64: ldrb sl, [r5, #0x4e5]
00377d68: str r6, [r4, #0x4e8]
00377d6c: str r7, [r4, #0x4c8]
00377d70: strb sl, [r4, #0x4e5]
00377d74: ldr sl, [r5, #0x4ec]
00377d78: str sl, [r4, #0x4ec]
00377d7c: ldrd sl, fp, [r5, ip]
00377d80: strd sl, fp, [r4, ip]
00377d84: ldr sl, [r5, #0x4f8]
00377d88: str sl, [r4, #0x4f8]
00377d8c: ldr sl, [r5, #0x4fc]
00377d90: str sl, [r4, #0x4fc]
00377d94: ldr sl, [r5, #0x500]
00377d98: str sl, [r4, #0x500]
00377d9c: ldrb sl, [r5, #0x504]
00377da0: str r8, [r4, #0x4e8]
00377da4: strb sl, [r4, #0x504]
00377da8: ldrb sl, [r5, #0x505]
00377dac: str r6, [r4, #0x508]
00377db0: str r7, [r4, #0x4e8]
00377db4: strb sl, [r4, #0x505]
00377db8: ldr sl, [r5, #0x50c]
00377dbc: str sl, [r4, #0x50c]
00377dc0: ldrd sl, fp, [r5, r2]
00377dc4: strd sl, fp, [r4, r2]
00377dc8: ldr sl, [r5, #0x518]
00377dcc: str sl, [r4, #0x518]
00377dd0: ldr sl, [r5, #0x51c]
00377dd4: str sl, [r4, #0x51c]
00377dd8: ldr sl, [r5, #0x520]
00377ddc: str sl, [r4, #0x520]
00377de0: ldrb sl, [r5, #0x524]
00377de4: str r8, [r4, #0x508]
00377de8: strb sl, [r4, #0x524]
00377dec: ldrb sl, [r5, #0x525]
00377df0: str r7, [r4, #0x508]
00377df4: str r6, [r4, #0x528]
00377df8: strb sl, [r4, #0x525]
00377dfc: ldr r6, [r5, #0x52c]
00377e00: str r6, [r4, #0x52c]
00377e04: ldrd sl, fp, [r5, r3]
00377e08: strd sl, fp, [r4, r3]
00377e0c: ldr r6, [r5, #0x538]
00377e10: str r6, [r4, #0x538]
00377e14: ldr r6, [r5, #0x53c]
00377e18: str r6, [r4, #0x53c]
00377e1c: ldr r6, [r5, #0x540]
00377e20: str r6, [r4, #0x540]
00377e24: ldrb r6, [r5, #0x544]
00377e28: str r8, [r4, #0x528]
00377e2c: strb r6, [r4, #0x544]
00377e30: ldrb r6, [r5, #0x545]
00377e34: str r7, [r4, #0x528]
00377e38: strb r6, [r4, #0x545]
00377e3c: ldr r2, [sp, #0xc]
00377e40: ldr r3, [sp, #8]
00377e44: ldr ip, [sp, #4]
00377e48: str ip, [r3, #-0x28]
00377e4c: ldr r6, [r2, #4]
00377e50: str r6, [r3, #-0x24]
00377e54: ldrd r6, r7, [r2, #8]
00377e58: strd r6, r7, [r3, #-0x20]
00377e5c: ldr r6, [r2, #0x10]
00377e60: str r6, [r3, #-0x18]
00377e64: ldr r6, [r2, #0x14]
00377e68: str r6, [r3, #-0x14]
00377e6c: ldr r6, [r2, #0x18]
00377e70: str r6, [r3, #-0x10]
00377e74: ldrb r6, [r2, #0x1c]
00377e78: str r0, [r3, #-0x28]
00377e7c: strb r6, [r3, #-0xc]
00377e80: ldr r6, [r2, #0x20]
00377e84: str sb, [r3, #-0x28]
00377e88: add r2, r2, #0x28
00377e8c: str r6, [r3, #-8]
00377e90: add r3, r3, #0x28
00377e94: cmp r3, r1
00377e98: bne #0x377e48
00377e9c: ldr r3, [r5, #0x660]
00377ea0: mov r0, r4
00377ea4: str r3, [r4, #0x660]
00377ea8: ldr r3, [r5, #0x664]
00377eac: str r3, [r4, #0x664]
00377eb0: ldr r3, [r5, #0x668]
00377eb4: str r3, [r4, #0x668]
00377eb8: ldrb r3, [r5, #0x66c]
00377ebc: strb r3, [r4, #0x66c]
00377ec0: ldr r3, [r5, #0x670]
00377ec4: str r3, [r4, #0x670]
00377ec8: ldr r3, [r5, #0x674]
00377ecc: str r3, [r4, #0x674]
00377ed0: ldr r3, [r5, #0x678]
00377ed4: str r3, [r4, #0x678]
00377ed8: ldr r3, [r5, #0x67c]
00377edc: str r3, [r4, #0x67c]
00377ee0: ldr r3, [r5, #0x680]
00377ee4: str r3, [r4, #0x680]
00377ee8: ldr r3, [r5, #0x684]
00377eec: str r3, [r4, #0x684]
00377ef0: add sp, sp, #0x2c
00377ef4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00377ef8: rsbeq sp, r1, r0, lsr #5
00377efc: andeq r1, r0, r8, lsr #1
00377f00: andeq r2, r0, r4, ror sp
00377f04: andeq r1, r0, r0, asr r5
00377f08: andeq r2, r0, r4, lsl #19
00377f0c: andeq r3, r0, r0, lsr lr
00377f10: andeq r1, r0, r4, asr #26
00377f14: andeq r3, r0, ip, lsr r5
00377f18: andeq r2, r0, ip, ror #21
00377f1c: andeq r2, r0, r8, asr #10
00377f20: strdeq r1, r2, [r0], -r8
00377f24: andeq r2, r0, ip, lsr #17
00377f28: andeq r0, r0, r8, asr #21
00377f2c: andeq r1, r0, r8, asr #1
00377f30: andeq r3, r0, r8, lsl r0

# 0x378808 _ZN10PlayerInfoaSERKS_
00378808: push {r4, r5, r6, r7, r8, lr}
0037880c: mov r4, r0
00378810: mov r6, r1
00378814: bl #0x378708
00378818: add r0, r4, #0x288
0037881c: add r1, r6, #0x2a8
00378820: ldr r3, [r4, #0x288]
00378824: mov lr, pc
00378828: ldr pc, [r3, #0x1c]
0037882c: add r0, r4, #0x2b0
00378830: add r1, r6, #0x2d0
00378834: ldr r3, [r4, #0x2b0]
00378838: mov lr, pc
0037883c: ldr pc, [r3, #0x1c]
00378840: add r0, r4, #0x2e8
00378844: add r1, r6, #0x308
00378848: ldr r3, [r4, #0x2e8]
0037884c: mov lr, pc
00378850: ldr pc, [r3, #0x1c]
00378854: add r0, r4, #0x310
00378858: add r1, r6, #0x330
0037885c: ldr r3, [r4, #0x310]
00378860: mov lr, pc
00378864: ldr pc, [r3, #0x1c]
00378868: add r0, r4, #0x338
0037886c: add r1, r6, #0x358
00378870: ldr r3, [r4, #0x338]
00378874: mov lr, pc
00378878: ldr pc, [r3, #0x1c]
0037887c: add r0, r4, #0x360
00378880: add r1, r6, #0x380
00378884: ldr r3, [r4, #0x360]
00378888: mov lr, pc
0037888c: ldr pc, [r3, #0x1c]
00378890: add r0, r4, #0x388
00378894: add r1, r6, #0x3a8
00378898: ldr r3, [r4, #0x388]
0037889c: mov lr, pc
003788a0: ldr pc, [r3, #0x1c]
003788a4: add r0, r4, #0x3b0
003788a8: add r1, r6, #0x3d0
003788ac: ldr r3, [r4, #0x3b0]
003788b0: mov lr, pc
003788b4: ldr pc, [r3, #0x1c]
003788b8: add r0, r4, #0x3d8
003788bc: add r1, r6, #0x3f8
003788c0: ldr r3, [r4, #0x3d8]
003788c4: mov lr, pc
003788c8: ldr pc, [r3, #0x1c]
003788cc: add r0, r4, #0x400
003788d0: add r1, r6, #0x420
003788d4: ldr r3, [r4, #0x400]
003788d8: mov lr, pc
003788dc: ldr pc, [r3, #0x1c]
003788e0: add r0, r4, #0x420
003788e4: add r1, r6, #0x440
003788e8: add r0, r0, #8
003788ec: add r1, r1, #8
003788f0: ldr r3, [r4, #0x428]
003788f4: mov lr, pc
003788f8: ldr pc, [r3, #0x1c]
003788fc: add r0, r4, #0x450
00378900: add r1, r6, #0x470
00378904: ldr r3, [r4, #0x450]
00378908: mov lr, pc
0037890c: ldr pc, [r3, #0x1c]
00378910: add r0, r4, #0x470
00378914: add r1, r6, #0x490
00378918: add r0, r0, #8
0037891c: add r1, r1, #8
00378920: ldr r3, [r4, #0x478]
00378924: mov lr, pc
00378928: ldr pc, [r3, #0x1c]
0037892c: add r0, r4, #0x4a0
00378930: add r1, r6, #0x4c0
00378934: ldr r3, [r4, #0x4a0]
00378938: mov lr, pc
0037893c: ldr pc, [r3, #0x1c]
00378940: add r0, r4, #0x4c0
00378944: add r1, r6, #0x4e0
00378948: add r0, r0, #8
0037894c: add r1, r1, #5
00378950: ldr r3, [r4, #0x4c8]
00378954: mov lr, pc
00378958: ldr pc, [r3, #0x1c]
0037895c: add r0, r4, #0x4e0
00378960: add r1, r6, #0x500
00378964: add r0, r0, #8
00378968: add r1, r1, #5
0037896c: ldr r3, [r4, #0x4e8]
00378970: mov lr, pc
00378974: ldr pc, [r3, #0x1c]
00378978: add r0, r4, #0x500
0037897c: add r1, r6, #0x520
00378980: add r0, r0, #8
00378984: add r1, r1, #5
00378988: ldr r3, [r4, #0x508]
0037898c: mov lr, pc
00378990: ldr pc, [r3, #0x1c]
00378994: add r8, r6, #0x540
00378998: add r0, r4, #0x520
0037899c: add r1, r8, #5
003789a0: add r0, r0, #8
003789a4: ldr r3, [r4, #0x528]
003789a8: mov lr, pc
003789ac: ldr pc, [r3, #0x1c]
003789b0: add r5, r4, #0x540
003789b4: add r5, r5, #8
003789b8: add r8, r8, #8
003789bc: mov r7, #0
003789c0: add r1, r8, r7
003789c4: ldr r3, [r5]
003789c8: mov r0, r5
003789cc: add r1, r1, #0x20
003789d0: add r7, r7, #0x28
003789d4: mov lr, pc
003789d8: ldr pc, [r3, #0x1c]
003789dc: cmp r7, #0x118
003789e0: add r5, r5, #0x28
003789e4: bne #0x3789c0
003789e8: ldr r3, [r6, #0x660]
003789ec: mov r0, r4
003789f0: str r3, [r4, #0x660]
003789f4: ldr r3, [r6, #0x664]
003789f8: str r3, [r4, #0x664]
003789fc: ldr r3, [r6, #0x668]
00378a00: str r3, [r4, #0x668]
00378a04: ldrb r3, [r6, #0x66c]
00378a08: strb r3, [r4, #0x66c]
00378a0c: ldr r3, [r6, #0x670]
00378a10: str r3, [r4, #0x670]
00378a14: ldr r3, [r6, #0x674]
00378a18: str r3, [r4, #0x674]
00378a1c: ldr r3, [r6, #0x678]
00378a20: str r3, [r4, #0x678]
00378a24: ldr r3, [r6, #0x67c]
00378a28: str r3, [r4, #0x67c]
00378a2c: ldr r3, [r6, #0x680]
00378a30: str r3, [r4, #0x680]
00378a34: ldr r3, [r6, #0x684]
00378a38: str r3, [r4, #0x684]
00378a3c: pop {r4, r5, r6, r7, r8, pc}

# 0x378a40 _ZN13PlayerManager10_AddPlayerEiiib
00378a40: push {r4, r5, r6, r7, r8, sb, sl, lr}
00378a44: ldr r4, [pc, #0x140]
00378a48: ldr r5, [pc, #0x140]
00378a4c: sub sp, sp, #0x690
00378a50: sub sp, sp, #8
00378a54: add r4, pc, r4
00378a58: str r1, [sp, #4]
00378a5c: ldr r1, [r4, r5]
00378a60: mov r7, r2
00378a64: mov r8, r3
00378a68: ldr r2, [r1]
00378a6c: mov r6, r0
00378a70: ldrb sl, [sp, #0x6b8]
00378a74: str r2, [sp, #0x694]
00378a78: bl #0x7fd794
00378a7c: ldrb r3, [r0, #5]
00378a80: cmp r3, #0
00378a84: bne #0x378abc
00378a88: mov r0, r6
00378a8c: ldr r1, [sp, #4]
00378a90: bl #0x36d280
00378a94: cmp r0, #0
00378a98: beq #0x378b40
00378a9c: ldr r3, [r4, r5]
00378aa0: ldr r2, [sp, #0x694]
00378aa4: ldr r3, [r3]
00378aa8: cmp r2, r3
00378aac: bne #0x378b88
00378ab0: add sp, sp, #0x298
00378ab4: add sp, sp, #0x400
00378ab8: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00378abc: bl #0x320e98
00378ac0: ldrb r3, [r0, #0x24]
00378ac4: cmp r3, #0
00378ac8: beq #0x378a88
00378acc: bl #0x800f8c
00378ad0: ldr r3, [r0]
00378ad4: mov lr, pc
00378ad8: ldr pc, [r3, #0x64]
00378adc: cmp r0, #0
00378ae0: beq #0x378a88
00378ae4: bl #0x8100dc
00378ae8: bl #0x8100e0
00378aec: cmp r0, #0
00378af0: beq #0x378a88
00378af4: ldr r1, [sp, #4]
00378af8: mov r2, #0
00378afc: mov r0, r6
00378b00: bl #0x36dfb0
00378b04: ldr r3, [r0]
00378b08: mov sb, r0
00378b0c: mov lr, pc
00378b10: ldr pc, [r3, #0x5c]
00378b14: cmp r0, #0
00378b18: beq #0x378a9c
00378b1c: strb sl, [sb, #0x66c]
00378b20: str r8, [sb, #0x668]
00378b24: ldr r3, [sp, #4]
00378b28: mov r0, sb
00378b2c: str r7, [sb, #0x674]
00378b30: str r3, [sb, #0x670]
00378b34: mov r1, #1
00378b38: bl #0x81366c
00378b3c: b #0x378b7c
00378b40: add sb, sp, #8
00378b44: mov r0, sb
00378b48: bl #0x37418c
00378b4c: ldr r3, [sp, #4]
00378b50: sub r1, sb, #4
00378b54: add r0, r6, #0x690
00378b58: str r3, [sp, #0x678]
00378b5c: strb sl, [sp, #0x674]
00378b60: str r8, [sp, #0x670]
00378b64: str r7, [sp, #0x67c]
00378b68: bl #0x378544
00378b6c: mov r1, sb
00378b70: bl #0x378808
00378b74: mov r0, sb
00378b78: bl #0x371294
00378b7c: mov r0, r6
00378b80: bl #0x36ed0c
00378b84: b #0x378a9c
00378b88: bl #0x30e310
00378b8c: rsbeq ip, r1, ip, lsr r0
00378b90: andeq r4, r0, ip, lsr #1

# 0x378b94 _ZN13PlayerManager23_CheckRemoteControllersEv
00378b94: push {r4, r5, r6, r7, r8, sl, lr}
00378b98: sub sp, sp, #0xc
00378b9c: mov r5, r0
00378ba0: bl #0x7fd794
00378ba4: ldrb r3, [r0, #5]
00378ba8: cmp r3, #0
00378bac: bne #0x378bb8
00378bb0: add sp, sp, #0xc
00378bb4: pop {r4, r5, r6, r7, r8, sl, pc}
00378bb8: bl #0x320e98
00378bbc: ldrb r3, [r0, #0x24]
00378bc0: cmp r3, #0
00378bc4: beq #0x378bb0
00378bc8: bl #0x800f8c
00378bcc: ldr r3, [r0]
00378bd0: mov lr, pc
00378bd4: ldr pc, [r3, #0x64]
00378bd8: cmp r0, #0
00378bdc: beq #0x378bb0
00378be0: bl #0x8100dc
00378be4: bl #0x8100e0
00378be8: cmp r0, #0
00378bec: beq #0x378bb0
00378bf0: bl #0x800f8c
00378bf4: ldr r3, [r0]
00378bf8: mov lr, pc
00378bfc: ldr pc, [r3, #0x6c]
00378c00: ldr r4, [r5, #0x6a8]
00378c04: ldr r3, [r5, #0x6ac]
00378c08: mov r6, r0
00378c0c: cmp r3, r4
00378c10: beq #0x378bb0
00378c14: mov r7, #0
00378c18: ldr r8, [r4]
00378c1c: bl #0x8100dc
00378c20: mov r1, r8
00378c24: mov r2, #0
00378c28: bl #0x8101e0
00378c2c: ldr sl, [r0, #0x1a0]
00378c30: mov r1, r8
00378c34: mov r2, #0
00378c38: cmp r6, sl
00378c3c: mov r0, r5
00378c40: add r4, r4, #4
00378c44: beq #0x378c70
00378c48: bl #0x36dfb0
00378c4c: ldr r3, [r0, #0x670]
00378c50: mov r1, r8
00378c54: mov r2, sl
00378c58: cmp r8, r3
00378c5c: mov r0, r5
00378c60: mvn r3, #0
00378c64: beq #0x378c70
00378c68: str r7, [sp]
00378c6c: bl #0x378a40
00378c70: ldr r3, [r5, #0x6ac]
00378c74: cmp r4, r3
00378c78: bne #0x378c18
00378c7c: b #0x378bb0

# 0x378c80 _ZN13PlayerManager22_CheckLocalControllersEv
00378c80: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00378c84: sub sp, sp, #0x4c
00378c88: mov sb, r0
00378c8c: bl #0x34dda4
00378c90: mov r5, r0
00378c94: bl #0x34d754
00378c98: add r3, sp, #0x34
00378c9c: str r3, [sp, #0x1c]
00378ca0: ldr r3, [pc, #0x2fc]
00378ca4: ldr lr, [sp, #0x1c]
00378ca8: ldr r1, [pc, #0x2f8]
00378cac: add r3, pc, r3
00378cb0: str r3, [sp, #0x28]
00378cb4: ldr r3, [pc, #0x2f0]
00378cb8: ldr r2, [pc, #0x2f0]
00378cbc: cmp r0, #1
00378cc0: movlt r0, #1
00378cc4: add r3, pc, r3
00378cc8: add lr, lr, #4
00378ccc: add r1, pc, r1
00378cd0: str r2, [sp, #0x20]
00378cd4: str r0, [sp, #0x14]
00378cd8: str r3, [sp, #0x2c]
00378cdc: mov r4, #0
00378ce0: str lr, [sp, #0x24]
00378ce4: str r1, [sp, #0x18]
00378ce8: b #0x378d24
00378cec: cmp r7, #0
00378cf0: bne #0x378e38
00378cf4: mov r2, r7
00378cf8: mov r0, sb
00378cfc: mov r1, r8
00378d00: bl #0x36dfb0
00378d04: ldr r3, [r0, #0x664]
00378d08: mov r6, r0
00378d0c: cmn r3, #1
00378d10: beq #0x378e58
00378d14: ldr r0, [sp, #0x14]
00378d18: add r4, r4, #1
00378d1c: cmp r4, r0
00378d20: bhs #0x378e30
00378d24: ldr r3, [r5]
00378d28: mov r1, r4
00378d2c: mov r0, r5
00378d30: mov lr, pc
00378d34: ldr pc, [r3, #8]
00378d38: ldrb r6, [r0, #0x758]
00378d3c: mov sl, r0
00378d40: bl #0x7fd794
00378d44: ldrb r3, [r0, #5]
00378d48: cmp r4, #0
00378d4c: moveq r6, #1
00378d50: cmp r3, #0
00378d54: bne #0x378d98
00378d58: mov r0, sb
00378d5c: mov r1, r4
00378d60: bl #0x36d280
00378d64: eor r0, r0, #1
00378d68: uxtb r7, r0
00378d6c: mov r8, r4
00378d70: mvn fp, #0
00378d74: cmp r6, #0
00378d78: bne #0x378cec
00378d7c: cmp r7, #0
00378d80: bne #0x378d14
00378d84: mov r1, r8
00378d88: mov r2, r7
00378d8c: mov r0, sb
00378d90: bl #0x36dfb0
00378d94: b #0x378d14
00378d98: bl #0x320e98
00378d9c: ldrb r3, [r0, #0x24]
00378da0: cmp r3, #0
00378da4: beq #0x378d58
00378da8: bl #0x800f8c
00378dac: ldr r3, [r0]
00378db0: mov lr, pc
00378db4: ldr pc, [r3, #0x64]
00378db8: cmp r0, #0
00378dbc: beq #0x378d58
00378dc0: bl #0x8100dc
00378dc4: bl #0x8100e0
00378dc8: cmp r0, #0
00378dcc: beq #0x378d58
00378dd0: bl #0x8100dc
00378dd4: mov r1, r4
00378dd8: bl #0x8105dc
00378ddc: ldr r2, [r0]
00378de0: str r0, [sp, #0xc]
00378de4: mov lr, pc
00378de8: ldr pc, [r2, #0x5c]
00378dec: ldr r3, [sp, #0xc]
00378df0: cmp r0, #0
00378df4: mvneq fp, #0
00378df8: ldrne r8, [r3, #0x178]
00378dfc: ldr ip, [r3, #0x670]
00378e00: moveq r8, fp
00378e04: ldrne fp, [r3, #0x1a0]
00378e08: subs r7, r8, ip
00378e0c: movne r7, #1
00378e10: cmp r7, #0
00378e14: bne #0x378f38
00378e18: cmn r8, #1
00378e1c: bne #0x378d74
00378e20: ldr r0, [sp, #0x14]
00378e24: add r4, r4, #1
00378e28: cmp r4, r0
00378e2c: blo #0x378d24
00378e30: add sp, sp, #0x4c
00378e34: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00378e38: mov ip, #1
00378e3c: mov r1, r8
00378e40: mov r2, fp
00378e44: mov r0, sb
00378e48: mov r3, r4
00378e4c: str ip, [sp]
00378e50: bl #0x378a40
00378e54: b #0x378d14
00378e58: ldrb r3, [r0, #0x240]
00378e5c: cmp r3, #2
00378e60: beq #0x378f90
00378e64: ldr r1, [sl, #0x1e0]
00378e68: ldr r0, [sl, #0x1e4]
00378e6c: bl #0x30eba4
00378e70: mov r1, #0x3f800000
00378e74: bl #0x30eba4
00378e78: mov r1, #0x3f000000
00378e7c: bl #0x30ed6c
00378e80: mov r1, r0
00378e84: ldr r0, [sl, #0x1d8]
00378e88: bl #0x30e4b4
00378e8c: cmp r0, #0
00378e90: beq #0x378f28
00378e94: ldrb r3, [sl, #0x1e8]
00378e98: cmp r3, #0
00378e9c: bne #0x378d14
00378ea0: ldr r3, [sp, #0x18]
00378ea4: ldr r2, [sp, #0x20]
00378ea8: ldr r0, [r3, r2]
00378eac: bl #0x31f594
00378eb0: cmp r0, #0
00378eb4: beq #0x378d14
00378eb8: bl #0x42ca8c
00378ebc: bl #0x42cb8c
00378ec0: mov r7, #2
00378ec4: mov r8, r0
00378ec8: mov r3, #0
00378ecc: ldr r0, [r6, #0x67c]
00378ed0: strb r3, [sp, #0x34]
00378ed4: strb r7, [sp, #0x35]
00378ed8: bl #0x30ed30
00378edc: strd r0, r1, [sp, #0x40]
00378ee0: ldr ip, [sp, #0x40]
00378ee4: ldr lr, [sp, #0x24]
00378ee8: ldr r1, [sp, #0x28]
00378eec: ldr r2, [sp, #0x2c]
00378ef0: ldr r3, [sp, #0x1c]
00378ef4: str ip, [lr]
00378ef8: ldr ip, [sp, #0x44]
00378efc: mov r0, r8
00378f00: str ip, [lr, #4]
00378f04: mov ip, #1
00378f08: str ip, [sp]
00378f0c: bl #0x7ad7e8
00378f10: ldr r0, [sp, #0x1c]
00378f14: bl #0x797124
00378f18: mov r0, r6
00378f1c: mov r1, r7
00378f20: bl #0x36f40c
00378f24: b #0x378d14
00378f28: ldrb r3, [sl, #0x1e8]
00378f2c: cmp r3, #0
00378f30: beq #0x378d14
00378f34: b #0x378ea0
00378f38: mov r1, #0
00378f3c: mov r2, r1
00378f40: mov r0, sb
00378f44: str r3, [sp, #0xc]
00378f48: str ip, [sp, #0x10]
00378f4c: bl #0x36e478
00378f50: ldr ip, [sp, #0x10]
00378f54: ldr r2, [r0, #0x670]
00378f58: ldr r3, [sp, #0xc]
00378f5c: cmp ip, r2
00378f60: bne #0x378e18
00378f64: ldr r2, [r3, #0x664]
00378f68: cmn r2, #1
00378f6c: bne #0x378e18
00378f70: ldr r1, [sp, #0x18]
00378f74: ldr r0, [sp, #0x20]
00378f78: ldr r2, [r1, r0]
00378f7c: ldr r2, [r2, #0x4c]
00378f80: ldr r2, [r2, #8]
00378f84: cmn r2, #1
00378f88: strne r2, [r3, #0x664]
00378f8c: b #0x378e18
00378f90: mov r2, r8
00378f94: mov r0, sb
00378f98: mov r1, r4
00378f9c: bl #0x36f4b4
00378fa0: b #0x378d14
00378fa4: ldrsheq r8, [r4], #-0xac
00378fa8: rsbeq fp, r1, r4, asr #27
00378fac: subseq r8, r4, r4, lsl #24
00378fb0: strdeq r3, r4, [r0], -r4

# 0x378fb4 _ZN13PlayerManager6UpdateEv
00378fb4: push {r4, r5, r6, r7, r8, lr}
00378fb8: mov r5, r0
00378fbc: bl #0x36d7a8
00378fc0: ldr r8, [pc, #0xb4]
00378fc4: subs r6, r0, #0
00378fc8: add r8, pc, r8
00378fcc: ble #0x37900c
00378fd0: mov r4, #0
00378fd4: mov r7, r4
00378fd8: mov r1, r4
00378fdc: mov r0, r5
00378fe0: mov r2, #0
00378fe4: bl #0x36e744
00378fe8: ldr r3, [r0, #0x660]
00378fec: add r4, r4, #1
00378ff0: cmp r3, #0
00378ff4: beq #0x379004
00378ff8: ldrb r3, [r3, #0x81]
00378ffc: cmp r3, #0
00379000: strne r7, [r0, #0x660]
00379004: cmp r4, r6
00379008: bne #0x378fd8
0037900c: ldrb r3, [r5, #0x71b]
00379010: cmp r3, #0
00379014: bne #0x379058
00379018: mov r0, r5
0037901c: bl #0x37193c
00379020: mov r0, r5
00379024: bl #0x378c80
00379028: mov r0, r5
0037902c: bl #0x378b94
00379030: mov r0, r5
00379034: bl #0x37280c
00379038: mov r0, r5
0037903c: bl #0x376100
00379040: mov r0, r5
00379044: bl #0x375eb4
00379048: ldr r3, [pc, #0x30]
0037904c: ldr r0, [r8, r3]
00379050: pop {r4, r5, r6, r7, r8, lr}
00379054: b #0x3790d8
00379058: mov r1, #0
0037905c: mov r0, r5
00379060: mov r2, r1
00379064: bl #0x36e478
00379068: ldrb r3, [r0, #0x4e5]
0037906c: cmp r3, #0
00379070: movne r3, #0
00379074: strbne r3, [r5, #0x71b]
00379078: b #0x379018
0037907c: rsbeq fp, r1, r8, asr #21
00379080: andeq r2, r0, r4, lsl r7

# 0x3b3d38 _ZN9Character18SafeGetCharPropsIdEv
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

# 0x3bb7fc _ZNK9Character17SG_GetPlayerClassEv
003bb7fc: movw r3, #0x14e8
003bb800: ldr r3, [r0, r3]
003bb804: cmp r3, #0
003bb808: mvneq r0, #0
003bb80c: ldrne r0, [r3, #0x34]
003bb810: bx lr

# 0x3bb814 _ZN9Character17SG_SetPlayerClassEi
003bb814: movw r3, #0x14e8
003bb818: ldr r3, [r0, r3]
003bb81c: cmp r3, #0
003bb820: strne r1, [r3, #0x34]
003bb824: bx lr

# 0x3bc4d0 _ZN9Character7SG_LoadEi
003bc4d0: movw r3, #0x14e8
003bc4d4: ldr r0, [r0, r3]
003bc4d8: cmp r0, #0
003bc4dc: bxeq lr
003bc4e0: b #0x465430
