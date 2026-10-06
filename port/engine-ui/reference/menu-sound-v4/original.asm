# 0x8624f8 _ZN3vox9VoxEngine7IsReadyERNS_10DataHandleE
008624f8: ldr r3, [pc, #0x18]
008624fc: ldr r2, [pc, #0x18]
00862500: add r3, pc, r3
00862504: ldr r2, [r3, r2]
00862508: ldr r0, [r2]
0086250c: cmp r0, #0
00862510: bxeq lr
00862514: b #0x868978
00862518: mulseq r3, r0, r5
0086251c: muleq r0, ip, r7

# 0x8627e0 _ZN3vox9VoxEngine14LoadDataSourceEiPviS1_i
008627e0: ldr r1, [pc, #0x68]
008627e4: ldr ip, [pc, #0x68]
008627e8: push {r4, lr}
008627ec: add r1, pc, r1
008627f0: ldr ip, [r1, ip]
008627f4: sub sp, sp, #0x10
008627f8: mov r4, r0
008627fc: ldr r1, [ip]
00862800: cmp r1, #0
00862804: beq #0x862830
00862808: ldr ip, [sp, #0x18]
0086280c: str ip, [sp]
00862810: ldr ip, [sp, #0x1c]
00862814: str ip, [sp, #4]
00862818: ldr ip, [sp, #0x20]
0086281c: str ip, [sp, #8]
00862820: bl #0x86b144
00862824: mov r0, r4
00862828: add sp, sp, #0x10
0086282c: pop {r4, pc}
00862830: mvn r2, #0
00862834: mvn r3, #0
00862838: str r1, [sp, #0xc]
0086283c: str r1, [sp]
00862840: str r1, [sp, #4]
00862844: str r1, [sp, #8]
00862848: bl #0x868d54
0086284c: b #0x862824
00862850: andseq r2, r3, r4, lsr #5
00862854: muleq r0, ip, r7

# 0x36b80c _ZN15VoxSoundManager4PlayEibiib
0036b80c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b810: ldr r4, [pc, #0x268]
0036b814: ldr r5, [pc, #0x268]
0036b818: ldr lr, [pc, #0x268]
0036b81c: add r4, pc, r4
0036b820: ldr ip, [r4, r5]
0036b824: ldr r6, [r4, lr]
0036b828: sub sp, sp, #0x8c
0036b82c: ldr ip, [ip]
0036b830: mov r7, r0
0036b834: mov r0, r6
0036b838: str r3, [sp, #0x14]
0036b83c: str ip, [sp, #0x84]
0036b840: mov sl, r1
0036b844: mov fp, r2
0036b848: ldrb sb, [sp, #0xb4]
0036b84c: bl #0x337888
0036b850: ldr r1, [pc, #0x234]
0036b854: add r8, sp, #0x6c
0036b858: add r2, sp, #0x68
0036b85c: add r1, pc, r1
0036b860: mov r0, r8
0036b864: bl #0x3140ec
0036b868: mov r0, r6
0036b86c: mov r1, r8
0036b870: bl #0x337a88
0036b874: mov r6, r0
0036b878: mov r0, r8
0036b87c: bl #0x3139ac
0036b880: cmp r6, #0
0036b884: bne #0x36b9e0
0036b888: cmp sl, #0
0036b88c: blt #0x36b9e0
0036b890: cmp sb, #0
0036b894: beq #0x36ba20
0036b898: ldr r3, [pc, #0x1f0]
0036b89c: ldr r3, [r4, r3]
0036b8a0: ldrb r3, [r3]
0036b8a4: cmp r3, #0
0036b8a8: bne #0x36ba00
0036b8ac: ldr r3, [pc, #0x1e0]
0036b8b0: mov r2, #0xc
0036b8b4: add r1, sp, #0x60
0036b8b8: ldr r3, [r4, r3]
0036b8bc: add ip, sp, #0x5c
0036b8c0: add r8, r7, #0x64
0036b8c4: ldr r3, [r3]
0036b8c8: mov r0, r8
0036b8cc: mla sl, r2, sl, r3
0036b8d0: add r3, sp, #0x58
0036b8d4: ldr r6, [sl, #4]
0036b8d8: add r2, sp, #0x50
0036b8dc: stm sp, {r1, ip}
0036b8e0: mov r1, r6
0036b8e4: add ip, sp, #0x54
0036b8e8: str ip, [sp, #8]
0036b8ec: bl #0x8896f4
0036b8f0: ldr r3, [r7, #8]
0036b8f4: ldr r1, [r3, r6, lsl #2]
0036b8f8: cmp r1, #0
0036b8fc: beq #0x36ba5c
0036b900: ldr r0, [r7]
0036b904: bl #0x8624f8
0036b908: cmp r0, #0
0036b90c: beq #0x36b9e0
0036b910: ldr r0, [sp, #0x14]
0036b914: bl #0x30e964
0036b918: mov r1, #0x44000000
0036b91c: add r1, r1, #0x7a0000
0036b920: bl #0x30ec94
0036b924: ldr r3, [r7, #8]
0036b928: ldr r2, [sp, #0x5c]
0036b92c: mov sl, r0
0036b930: ldr r1, [r3, r6, lsl #2]
0036b934: ldr r0, [r7]
0036b938: bl #0x862618
0036b93c: add ip, sp, #0x67
0036b940: str ip, [sp]
0036b944: add ip, sp, #0x44
0036b948: mov r1, r6
0036b94c: mov r0, r8
0036b950: add r2, sp, #0x4c
0036b954: add r3, sp, #0x48
0036b958: str ip, [sp, #4]
0036b95c: add ip, sp, #0x40
0036b960: str ip, [sp, #8]
0036b964: bl #0x889894
0036b968: ldr r3, [r7, #8]
0036b96c: ldr r1, [r7]
0036b970: mov r8, #0
0036b974: ldr r2, [r3, r6, lsl #2]
0036b978: add r6, sp, #0x18
0036b97c: ldr r3, [sp, #0x4c]
0036b980: mov r0, r6
0036b984: str r8, [sp]
0036b988: bl #0x862438
0036b98c: ldr r0, [r7]
0036b990: mov r1, r6
0036b994: mov r2, r8
0036b998: mov r3, #1
0036b99c: bl #0x861d60
0036b9a0: ldr r3, [sp, #0x40]
0036b9a4: mov r2, r8
0036b9a8: ldr r0, [r7]
0036b9ac: mov r1, r6
0036b9b0: bl #0x861950
0036b9b4: ldr r3, [sp, #0xb0]
0036b9b8: sub r3, r3, #1
0036b9bc: cmp r3, #0x1d
0036b9c0: bls #0x36ba48
0036b9c4: ldr r0, [r7]
0036b9c8: mov r3, sl
0036b9cc: mov r1, r6
0036b9d0: ldrb r2, [sp, #0x67]
0036b9d4: bl #0x8621c0
0036b9d8: mov r0, r6
0036b9dc: bl #0x8683ac
0036b9e0: ldr r3, [r4, r5]
0036b9e4: ldr r2, [sp, #0x84]
0036b9e8: mov r0, #0
0036b9ec: ldr r3, [r3]
0036b9f0: cmp r2, r3
0036b9f4: bne #0x36ba7c
0036b9f8: add sp, sp, #0x8c
0036b9fc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036ba00: ldr r3, [pc, #0x90]
0036ba04: mov r0, sl
0036ba08: mov r2, fp
0036ba0c: ldr r1, [r4, r3]
0036ba10: mov r3, #2
0036ba14: ldr r1, [r1]
0036ba18: bl #0x531348
0036ba1c: b #0x36b9e0
0036ba20: bl #0x7fd794
0036ba24: ldrb r3, [r0, #5]
0036ba28: cmp r3, #0
0036ba2c: beq #0x36b898
0036ba30: ldr r3, [pc, #0x64]
0036ba34: ldr r3, [r4, r3]
0036ba38: ldrb r3, [r3]
0036ba3c: cmp r3, #0
0036ba40: bne #0x36b9e0
0036ba44: b #0x36b898
0036ba48: ldr r0, [r7]
0036ba4c: mov r1, r6
0036ba50: ldr r2, [sp, #0x48]
0036ba54: bl #0x862058
0036ba58: b #0x36b9c4
0036ba5c: mov r1, r6
0036ba60: mov r0, r7
0036ba64: bl #0x3699fc
0036ba68: ldr r3, [r7, #8]
0036ba6c: ldr r1, [r3, r6, lsl #2]
0036ba70: cmp r1, #0
0036ba74: beq #0x36b9e0
0036ba78: b #0x36b900
0036ba7c: bl #0x30e310
0036ba80: rsbeq sb, r2, r4, ror r2
0036ba84: andeq r4, r0, ip, lsr #1
0036ba88: andeq r0, r0, r4, lsl #17
0036ba8c: subseq r5, r5, ip, lsl #17
0036ba90: andeq r3, r0, r0, lsr fp
0036ba94: andeq r3, r0, ip, lsr lr
0036ba98: andeq r0, r0, r0, lsl #13
0036ba9c: andeq r2, r0, r0, lsr #31

# 0x531230 nativeLoadSound
00531230: push {r4, lr}
00531234: ldr lr, [pc, #0x44]
00531238: ldr r2, [pc, #0x44]
0053123c: ldr r3, [pc, #0x44]
00531240: add lr, pc, lr
00531244: ldr ip, [lr, r2]
00531248: add r3, pc, r3
0053124c: ldr r4, [r3]
00531250: ldr ip, [ip]
00531254: sub sp, sp, #8
00531258: ldr r2, [r3, #4]
0053125c: mov r3, r0
00531260: mov r0, ip
00531264: ldr ip, [ip]
00531268: str r1, [sp]
0053126c: mov r1, r4
00531270: mov lr, pc
00531274: ldr pc, [ip, #0x234]
00531278: add sp, sp, #8
0053127c: pop {r4, pc}
00531280: subeq r3, r6, r0, asr r8
00531284: muleq r0, r4, sl
00531288: ldrdeq r5, r6, [ip], #-0x14

# 0x86f5fc _ZN3vox8VoxUtils24LoadDataSourceFromFileExEPKciNS_21VoxSourceLoadingFlagsEi
0086f5fc: push {r4, r5, r6, r7, r8, lr}
0086f600: mov r7, r3
0086f604: sub sp, sp, #0x10
0086f608: mov r4, r0
0086f60c: mov r8, r1
0086f610: mov r6, r2
0086f614: ldr r5, [sp, #0x28]
0086f618: bl #0x862b30
0086f61c: tst r7, #0x10000
0086f620: bne #0x86f654
0086f624: tst r7, #1
0086f628: bne #0x86f680
0086f62c: cmp r7, #2
0086f630: beq #0x86f698
0086f634: mov r1, r8
0086f638: mov r2, r6
0086f63c: mov r3, r5
0086f640: mov r0, r4
0086f644: bl #0x86f558
0086f648: mov r0, r4
0086f64c: add sp, sp, #0x10
0086f650: pop {r4, r5, r6, r7, r8, pc}
0086f654: uxth r7, r7
0086f658: mov r1, r0
0086f65c: mov ip, #0
0086f660: mov r3, r8
0086f664: mov r0, r4
0086f668: mov r2, #1
0086f66c: stm sp, {r6, ip}
0086f670: str r5, [sp, #8]
0086f674: str r7, [sp, #0xc]
0086f678: bl #0x862760
0086f67c: b #0x86f648
0086f680: mov r1, r8
0086f684: mov r2, r6
0086f688: mov r3, r5
0086f68c: mov r0, r4
0086f690: bl #0x86f3b4
0086f694: b #0x86f648
0086f698: mov r1, r8
0086f69c: mov r2, r6
0086f6a0: mov r3, r5
0086f6a4: mov r0, r4
0086f6a8: bl #0x86f5a0
0086f6ac: b #0x86f648

# 0x888c34 PlayMenuDependency__ZN3vox11StreamCFileC2EPKc
00888c34: ldr r3, [pc, #0x70]
00888c38: ldr r2, [pc, #0x70]
00888c3c: push {r4, r5, r6, r7, r8, lr}
00888c40: add r3, pc, r3
00888c44: ldr r2, [r3, r2]
00888c48: mov r5, r0
00888c4c: mov r6, #0
00888c50: add r2, r2, #8
00888c54: str r6, [r0, #4]
00888c58: str r2, [r5], #8
00888c5c: mov r4, r0
00888c60: str r5, [r0, #0x18]
00888c64: str r5, [r0, #0x1c]
00888c68: mov r0, r5
00888c6c: mov r7, r1
00888c70: bl #0x888c30
00888c74: ldr r3, [r4, #0x18]
00888c78: cmp r7, r6
00888c7c: strb r6, [r3]
00888c80: beq #0x888ca4
00888c84: mov r0, r7
00888c88: bl #0x30de54
00888c8c: mov r1, r7
00888c90: add r2, r7, r0
00888c94: mov r0, r5
00888c98: bl #0x888b70
00888c9c: mov r0, r4
00888ca0: bl #0x888a60
00888ca4: mov r0, r4
00888ca8: pop {r4, r5, r6, r7, r8, pc}
00888cac: andseq fp, r0, r0, asr lr
00888cb0: andeq r3, r0, r0, lsr r8

# 0x5315bc nativeUnloadSound
005315bc: push {r4, lr}
005315c0: ldr lr, [pc, #0x44]
005315c4: ldr r2, [pc, #0x44]
005315c8: ldr r3, [pc, #0x44]
005315cc: add lr, pc, lr
005315d0: ldr ip, [lr, r2]
005315d4: add r3, pc, r3
005315d8: ldr r4, [r3]
005315dc: ldr ip, [ip]
005315e0: sub sp, sp, #8
005315e4: ldr r2, [r3, #0x2c]
005315e8: mov r3, r0
005315ec: mov r0, ip
005315f0: ldr ip, [ip]
005315f4: str r1, [sp]
005315f8: mov r1, r4
005315fc: mov lr, pc
00531600: ldr pc, [ip, #0x234]
00531604: add sp, sp, #8
00531608: pop {r4, pc}
0053160c: subeq r3, r6, r4, asr #9
00531610: muleq r0, r4, sl
00531614: subeq r4, ip, r8, asr #28

# 0x36bc34 _ZN15VoxSoundManager8PlayMenuEibii
0036bc34: str lr, [sp, #-4]!
0036bc38: mov ip, #1
0036bc3c: sub sp, sp, #0xc
0036bc40: str ip, [sp, #4]
0036bc44: ldr ip, [sp, #0x10]
0036bc48: str ip, [sp]
0036bc4c: bl #0x36b80c
0036bc50: add sp, sp, #0xc
0036bc54: ldm sp!, {pc}

# 0x8896f4 _ZNK3vox15VoxSoundPackXML17GetDataSourceInfoEiRPKcRNS_11FormatTypesERiS6_RNS_21VoxSourceLoadingFlagsE
008896f4: cmp r1, #0
008896f8: push {r4, r5}
008896fc: blt #0x889728
00889700: ldm r0, {r4, ip}
00889704: rsb ip, r4, ip
00889708: asr ip, ip, #2
0088970c: lsl r5, ip, #4
00889710: rsb r5, ip, r5
00889714: add r5, r5, r5, lsl #8
00889718: add r5, r5, r5, lsl #16
0088971c: add ip, ip, r5, lsl #4
00889720: cmp r1, ip
00889724: blt #0x889738
00889728: mov r1, #0
0088972c: mov r0, r1
00889730: pop {r4, r5}
00889734: bx lr
00889738: mov ip, #0x44
0088973c: mul ip, ip, r1
00889740: ldr r5, [r4, ip]
00889744: add r4, r4, ip
00889748: cmp r1, r5
0088974c: bne #0x889728
00889750: ldr r4, [r4, #0xc]
00889754: mov r1, #1
00889758: str r4, [r2]
0088975c: ldr r2, [r0]
00889760: add r2, r2, ip
00889764: ldrsb r2, [r2, #0x14]
00889768: str r2, [r3]
0088976c: ldr r3, [r0]
00889770: add r3, r3, ip
00889774: ldrsb r2, [r3, #0x16]
00889778: ldr r3, [sp, #8]
0088977c: str r2, [r3]
00889780: ldr r3, [r0]
00889784: add r3, r3, ip
00889788: ldrsb r2, [r3, #0x15]
0088978c: ldr r3, [sp, #0xc]
00889790: str r2, [r3]
00889794: ldr r3, [r0]
00889798: add ip, r3, ip
0088979c: ldr r2, [ip, #0x10]
008897a0: ldr r3, [sp, #0x10]
008897a4: str r2, [r3]
008897a8: b #0x88972c

# 0x862b30 _ZN3vox9VoxEngine12GetVoxEngineEv
00862b30: ldr r3, [pc, #0x40]
00862b34: ldr r2, [pc, #0x40]
00862b38: push {r4, r5, r6, lr}
00862b3c: add r3, pc, r3
00862b40: ldr r4, [r3, r2]
00862b44: ldr r5, [r4]
00862b48: cmp r5, #0
00862b4c: beq #0x862b58
00862b50: mov r0, r5
00862b54: pop {r4, r5, r6, pc}
00862b58: mov r1, r5
00862b5c: mov r0, #0x20
00862b60: bl #0x310648
00862b64: mov r5, r0
00862b68: bl #0x862abc
00862b6c: str r5, [r4]
00862b70: mov r0, r5
00862b74: pop {r4, r5, r6, pc}
00862b78: andseq r1, r3, r4, asr pc
00862b7c: andeq r2, r0, r8, asr #25

# 0x888aec PlayMenuDependency__ZN3vox11StreamCFile15CreateNewCursorEv
00888aec: push {r4, r5, r6, lr}
00888af0: ldr r3, [r0, #4]
00888af4: ldr r4, [pc, #0x6c]
00888af8: mov r5, r0
00888afc: cmp r3, #0
00888b00: add r4, pc, r4
00888b04: ble #0x888b60
00888b08: mov r1, #0
00888b0c: movw r0, #0x801c
00888b10: bl #0x310648
00888b14: ldr r2, [pc, #0x50]
00888b18: mov r3, #0
00888b1c: movw r1, #0x8018
00888b20: ldr r2, [r4, r2]
00888b24: str r3, [r0, r1]
00888b28: str r5, [r0, #4]
00888b2c: add r2, r2, #8
00888b30: str r2, [r0]
00888b34: mvn r2, #0
00888b38: str r2, [r0, #0xc]
00888b3c: movw r2, #0x8010
00888b40: str r3, [r0, r2]
00888b44: movw r2, #0x8014
00888b48: str r3, [r0, r2]
00888b4c: mov r6, r0
00888b50: str r3, [r0, #8]
00888b54: bl #0x888728
00888b58: mov r0, r6
00888b5c: pop {r4, r5, r6, pc}
00888b60: mov r0, #0
00888b64: pop {r4, r5, r6, pc}
00888b68: mulseq r0, r0, pc
00888b6c: andeq r1, r0, r0, lsl #4

# 0x36921c _ZN15VoxSoundManager11UnloadSoundEi
0036921c: ldr r3, [pc, #0x6c]
00369220: ldr r2, [pc, #0x6c]
00369224: push {r4, r5, lr}
00369228: add r3, pc, r3
0036922c: ldr r2, [r3, r2]
00369230: sub sp, sp, #0xc
00369234: mov r4, r0
00369238: ldrb r5, [r2]
0036923c: cmp r5, #0
00369240: bne #0x369288
00369244: cmp r1, #0
00369248: blt #0x369288
0036924c: ldr r3, [r0, #0x1c]
00369250: cmp r1, r3
00369254: bge #0x369288
00369258: ldr r3, [r0, #8]
0036925c: ldr r3, [r3, r1, lsl #2]
00369260: cmp r3, #0
00369264: beq #0x369288
00369268: mov r0, r3
0036926c: ldr r3, [r3]
00369270: str r1, [sp, #4]
00369274: mov lr, pc
00369278: ldr pc, [r3, #4]
0036927c: ldr r3, [r4, #8]
00369280: ldr r1, [sp, #4]
00369284: str r5, [r3, r1, lsl #2]
00369288: add sp, sp, #0xc
0036928c: pop {r4, r5, pc}
00369290: rsbeq fp, r2, r8, ror #16
00369294: andeq r3, r0, r0, lsr fp

# 0x888728 PlayMenuDependency__ZN3vox17StreamCFileCursor4InitEv
00888728: push {r4, lr}
0088872c: ldr r3, [r0, #4]
00888730: mov r4, r0
00888734: cmp r3, #0
00888738: beq #0x888748
0088873c: ldr r2, [r0, #8]
00888740: cmp r2, #0
00888744: beq #0x88874c
00888748: pop {r4, pc}
0088874c: ldr r1, [r3, #0x1c]
00888750: ldr r3, [r3, #0x20]
00888754: cmp r1, #0
00888758: beq #0x888748
0088875c: mov r0, r3
00888760: mov r2, #6
00888764: ldr r3, [r3]
00888768: mov lr, pc
0088876c: ldr pc, [r3, #8]
00888770: str r0, [r4, #8]
00888774: pop {r4, pc}

# 0x86f3b4 _ZN3vox8VoxUtils27LoadDataSourceFromFileToRAMEPKcii
0086f3b4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0086f3b8: sub sp, sp, #0x2c
0086f3bc: mov r5, r1
0086f3c0: str r2, [sp, #0x10]
0086f3c4: str r3, [sp, #0x14]
0086f3c8: mov r6, r0
0086f3cc: bl #0x862b30
0086f3d0: mov fp, r0
0086f3d4: bl #0x8945a4
0086f3d8: ldr r4, [pc, #0x170]
0086f3dc: subs r8, r0, #0
0086f3e0: add r4, pc, r4
0086f3e4: beq #0x86f510
0086f3e8: mov r1, r5
0086f3ec: ldr r3, [r8]
0086f3f0: mov r2, #6
0086f3f4: mov lr, pc
0086f3f8: ldr pc, [r3, #8]
0086f3fc: subs r5, r0, #0
0086f400: beq #0x86f510
0086f404: mov r1, #0
0086f408: mov r2, #2
0086f40c: ldr r3, [r5]
0086f410: mov lr, pc
0086f414: ldr pc, [r3, #0xc]
0086f418: ldr r3, [r5]
0086f41c: mov r0, r5
0086f420: mov lr, pc
0086f424: ldr pc, [r3, #0x10]
0086f428: subs r7, r0, #0
0086f42c: ble #0x86f4fc
0086f430: mov r1, #0
0086f434: mov r2, r1
0086f438: ldr r3, [r5]
0086f43c: mov r0, r5
0086f440: mov lr, pc
0086f444: ldr pc, [r3, #0xc]
0086f448: mov r0, r7
0086f44c: bl #0x3104f8
0086f450: subs sl, r0, #0
0086f454: beq #0x86f4fc
0086f458: mov r4, #0
0086f45c: movw sb, #0xffff
0086f460: b #0x86f468
0086f464: add r4, r4, r0
0086f468: rsb r3, r4, r7
0086f46c: cmp r3, sb
0086f470: ldrle ip, [r5]
0086f474: movle r0, r5
0086f478: addle r1, sl, r4
0086f47c: movle r2, #1
0086f480: addgt r1, sl, r4
0086f484: ldrgt ip, [r5]
0086f488: movgt r0, r5
0086f48c: movgt r2, #1
0086f490: movgt r3, #0x10000
0086f494: mov lr, pc
0086f498: ldr pc, [ip, #8]
0086f49c: cmp r0, #0
0086f4a0: bgt #0x86f464
0086f4a4: mov r1, r5
0086f4a8: ldr r3, [r8]
0086f4ac: mov r0, r8
0086f4b0: mov lr, pc
0086f4b4: ldr pc, [r3, #0xc]
0086f4b8: mov lr, #1
0086f4bc: strb lr, [sp, #0x25]
0086f4c0: ldr lr, [sp, #0x10]
0086f4c4: mov ip, #0
0086f4c8: mov r1, fp
0086f4cc: str lr, [sp]
0086f4d0: ldr lr, [sp, #0x14]
0086f4d4: mov r2, ip
0086f4d8: mov r0, r6
0086f4dc: add r3, sp, #0x1c
0086f4e0: str sl, [sp, #0x1c]
0086f4e4: str r7, [sp, #0x20]
0086f4e8: str lr, [sp, #8]
0086f4ec: strb ip, [sp, #0x24]
0086f4f0: str ip, [sp, #4]
0086f4f4: bl #0x8627e0
0086f4f8: b #0x86f544
0086f4fc: mov r0, r8
0086f500: mov r1, r5
0086f504: ldr r3, [r8]
0086f508: mov lr, pc
0086f50c: ldr pc, [r3, #0xc]
0086f510: ldr r2, [pc, #0x3c]
0086f514: mvn r0, #0
0086f518: mvn r1, #0
0086f51c: ldr r2, [r4, r2]
0086f520: strd r0, r1, [r6, #8]
0086f524: mov r3, #0
0086f528: add r2, r2, #8
0086f52c: str r3, [r6, #0x20]
0086f530: str r2, [r6]
0086f534: str r3, [r6, #0x10]
0086f538: str r3, [r6, #0x14]
0086f53c: str r3, [r6, #0x18]
0086f540: str r3, [r6, #0x1c]
0086f544: mov r0, r6
0086f548: add sp, sp, #0x2c
0086f54c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0086f550: ldrheq r5, [r2], -r0
0086f554: andeq r1, r0, r4, lsr #19

# 0x8947d4 PlayMenuDependency__ZN3vox9openStdIOEPKcNS_17VoxFileAccessModeE
008947d4: cmp r1, #0xb
008947d8: addls pc, pc, r1, lsl #2
008947dc: b #0x89481c
008947e0: b #0x894824
008947e4: b #0x894830
008947e8: b #0x89483c
008947ec: b #0x894848
008947f0: b #0x894854
008947f4: b #0x894860
008947f8: b #0x89486c
008947fc: b #0x894878
00894800: b #0x894884
00894804: b #0x894890
00894808: b #0x89489c
0089480c: b #0x894810
00894810: ldr r1, [pc, #0x90]
00894814: add r1, pc, r1
00894818: b #0x30e508
0089481c: mov r0, #0
00894820: bx lr
00894824: ldr r1, [pc, #0x80]
00894828: add r1, pc, r1
0089482c: b #0x30e508
00894830: ldr r1, [pc, #0x78]
00894834: add r1, pc, r1
00894838: b #0x30e508
0089483c: ldr r1, [pc, #0x70]
00894840: add r1, pc, r1
00894844: b #0x30e508
00894848: ldr r1, [pc, #0x68]
0089484c: add r1, pc, r1
00894850: b #0x30e508
00894854: ldr r1, [pc, #0x60]
00894858: add r1, pc, r1
0089485c: b #0x30e508
00894860: ldr r1, [pc, #0x58]
00894864: add r1, pc, r1
00894868: b #0x30e508
0089486c: ldr r1, [pc, #0x50]
00894870: add r1, pc, r1
00894874: b #0x30e508
00894878: ldr r1, [pc, #0x48]
0089487c: add r1, pc, r1
00894880: b #0x30e508
00894884: ldr r1, [pc, #0x40]
00894888: add r1, pc, r1
0089488c: b #0x30e508
00894890: ldr r1, [pc, #0x38]
00894894: add r1, pc, r1
00894898: b #0x30e508
0089489c: ldr r1, [pc, #0x30]
008948a0: add r1, pc, r1
008948a4: b #0x30e508
008948a8: andeq fp, r2, ip, ror pc
008948ac: andeq sb, r4, r0, lsl #30
008948b0: andeq sl, r2, ip, ror r7
008948b4: andeq sp, r2, r0, ror #11
008948b8: andeq sp, r7, r4, lsr #3
008948bc: andeq sp, r7, r0, lsr #3
008948c0: muleq r7, ip, r1
008948c4: andeq fp, r2, r0, lsr pc
008948c8: andeq sl, r4, ip, lsl r7
008948cc: andeq sl, r4, r8, lsl r7
008948d0: andeq sp, r7, r4, ror r1
008948d4: strdeq fp, ip, [r2], -r8

# 0x888a60 PlayMenuDependency__ZN3vox11StreamCFile4InitEv
00888a60: push {r4, r5, r6, lr}
00888a64: mov r5, #0
00888a68: str r5, [r0, #4]
00888a6c: mov r4, r0
00888a70: bl #0x8945a4
00888a74: ldr r1, [r4, #0x1c]
00888a78: ldr r2, [r4, #0x18]
00888a7c: str r0, [r4, #0x20]
00888a80: cmp r2, r1
00888a84: beq #0x888ae8
00888a88: cmp r0, r5
00888a8c: beq #0x888ae8
00888a90: ldr r3, [r0]
00888a94: mov r2, #6
00888a98: mov lr, pc
00888a9c: ldr pc, [r3, #8]
00888aa0: subs r6, r0, #0
00888aa4: beq #0x888ae8
00888aa8: mov r1, r5
00888aac: mov r2, #2
00888ab0: ldr r3, [r6]
00888ab4: mov lr, pc
00888ab8: ldr pc, [r3, #0xc]
00888abc: ldr r3, [r6]
00888ac0: mov r0, r6
00888ac4: mov lr, pc
00888ac8: ldr pc, [r3, #0x10]
00888acc: ldr r3, [r4, #0x20]
00888ad0: str r0, [r4, #4]
00888ad4: mov r1, r6
00888ad8: mov r0, r3
00888adc: ldr r3, [r3]
00888ae0: mov lr, pc
00888ae4: ldr pc, [r3, #0xc]
00888ae8: pop {r4, r5, r6, pc}

# 0x3b3b00 _ZN9Character11_InitSoundsEv
003b3b00: push {r4, r5, r6, r7, r8, lr}
003b3b04: bl #0x3a32d0
003b3b08: ldr r6, [pc, #0xf0]
003b3b0c: ldr r7, [pc, #0xf0]
003b3b10: mov r4, r0
003b3b14: add r6, pc, r6
003b3b18: ldr r3, [r6, r7]
003b3b1c: ldr r0, [r3]
003b3b20: cmp r0, #0
003b3b24: beq #0x3b3bfc
003b3b28: ldr r3, [r4, #0xc]
003b3b2c: cmp r3, #0
003b3b30: beq #0x3b3b60
003b3b34: mov r5, #0
003b3b38: b #0x3b3b44
003b3b3c: ldr r3, [r6, r7]
003b3b40: ldr r0, [r3]
003b3b44: ldr r3, [r4, #0x10]
003b3b48: ldr r1, [r3, r5, lsl #2]
003b3b4c: bl #0x3699fc
003b3b50: ldr r3, [r4, #0xc]
003b3b54: add r5, r5, #1
003b3b58: cmp r3, r5
003b3b5c: bhi #0x3b3b3c
003b3b60: ldr r3, [r4, #4]
003b3b64: cmp r3, #0
003b3b68: beq #0x3b3b94
003b3b6c: ldr r8, [r6, r7]
003b3b70: mov r5, #0
003b3b74: ldr r3, [r4, #8]
003b3b78: ldr r0, [r8]
003b3b7c: ldr r1, [r3, r5, lsl #2]
003b3b80: bl #0x3699fc
003b3b84: ldr r3, [r4, #4]
003b3b88: add r5, r5, #1
003b3b8c: cmp r3, r5
003b3b90: bhi #0x3b3b74
003b3b94: ldr r3, [r4, #0x14]
003b3b98: cmp r3, #0
003b3b9c: beq #0x3b3bc8
003b3ba0: ldr r8, [r6, r7]
003b3ba4: mov r5, #0
003b3ba8: ldr r3, [r4, #0x18]
003b3bac: ldr r0, [r8]
003b3bb0: ldr r1, [r3, r5, lsl #2]
003b3bb4: bl #0x3699fc
003b3bb8: ldr r3, [r4, #0x14]
003b3bbc: add r5, r5, #1
003b3bc0: cmp r3, r5
003b3bc4: bhi #0x3b3ba8
003b3bc8: ldr r3, [r4, #0x1c]
003b3bcc: cmp r3, #0
003b3bd0: beq #0x3b3bfc
003b3bd4: ldr r6, [r6, r7]
003b3bd8: mov r5, #0
003b3bdc: ldr r3, [r4, #0x20]
003b3be0: ldr r0, [r6]
003b3be4: ldr r1, [r3, r5, lsl #2]
003b3be8: bl #0x3699fc
003b3bec: ldr r3, [r4, #0x1c]
003b3bf0: add r5, r5, #1
003b3bf4: cmp r3, r5
003b3bf8: bhi #0x3b3bdc
003b3bfc: pop {r4, r5, r6, r7, r8, pc}
003b3c00: subseq r0, lr, ip, ror pc
003b3c04: andeq r0, r0, r4, lsr #27

# 0x868978 _ZN3vox17VoxEngineInternal7IsReadyERNS_10DataHandleE
00868978: push {r4, r5, r6, lr}
0086897c: add r4, r0, #0x54
00868980: mov r5, r0
00868984: mov r6, r1
00868988: mov r0, r4
0086898c: bl #0x893548
00868990: mov r0, r5
00868994: mov r1, r6
00868998: bl #0x8686bc
0086899c: subs r5, r0, #0
008689a0: beq #0x8689ac
008689a4: bl #0x8658cc
008689a8: mov r5, r0
008689ac: mov r0, r4
008689b0: bl #0x89351c
008689b4: mov r0, r5
008689b8: pop {r4, r5, r6, pc}

# 0x3699fc _ZN15VoxSoundManager9LoadSoundEi
003699fc: push {r4, r5, r6, r7, r8, sl, lr}
00369a00: ldr r4, [pc, #0x11c]
00369a04: ldr r3, [pc, #0x11c]
00369a08: ldr r5, [pc, #0x11c]
00369a0c: add r4, pc, r4
00369a10: ldr r2, [r4, r3]
00369a14: ldr r3, [r4, r5]
00369a18: sub sp, sp, #0x22c
00369a1c: ldrb r2, [r2]
00369a20: ldr r3, [r3]
00369a24: mov r7, r0
00369a28: cmp r2, #0
00369a2c: mov r6, r1
00369a30: str r3, [sp, #0x224]
00369a34: bne #0x369a4c
00369a38: cmp r1, #0
00369a3c: blt #0x369a4c
00369a40: ldr r3, [r0, #0x1c]
00369a44: cmp r1, r3
00369a48: ble #0x369a68
00369a4c: ldr r3, [r4, r5]
00369a50: ldr r2, [sp, #0x224]
00369a54: ldr r3, [r3]
00369a58: cmp r2, r3
00369a5c: bne #0x369b20
00369a60: add sp, sp, #0x22c
00369a64: pop {r4, r5, r6, r7, r8, sl, pc}
00369a68: add ip, sp, #0x20
00369a6c: str ip, [sp]
00369a70: add ip, sp, #0x1c
00369a74: add r3, sp, #0x18
00369a78: str ip, [sp, #4]
00369a7c: add r0, r0, #0x64
00369a80: add ip, sp, #0x14
00369a84: add r2, sp, #0x10
00369a88: str ip, [sp, #8]
00369a8c: bl #0x8896f4
00369a90: ldr r3, [r7, #0x1c]
00369a94: cmp r6, r3
00369a98: bgt #0x369a4c
00369a9c: ldr r3, [r7, #8]
00369aa0: ldr r3, [r3, r6, lsl #2]
00369aa4: cmp r3, #0
00369aa8: bne #0x369a4c
00369aac: ldr r3, [pc, #0x7c]
00369ab0: add sl, sp, #0x24
00369ab4: mov r0, sl
00369ab8: ldr r3, [r4, r3]
00369abc: ldr r1, [r3]
00369ac0: bl #0x30e520
00369ac4: mov r0, sl
00369ac8: bl #0x30de54
00369acc: ldr r1, [pc, #0x60]
00369ad0: mov r2, #0xd
00369ad4: add r0, sl, r0
00369ad8: add r1, pc, r1
00369adc: bl #0x30e868
00369ae0: ldr r1, [sp, #0x10]
00369ae4: mov r0, sl
00369ae8: bl #0x30ed90
00369aec: mov r1, #4
00369af0: mov r0, #0x28
00369af4: bl #0x310570
00369af8: ldr ip, [sp, #0x20]
00369afc: ldr r3, [sp, #0x14]
00369b00: mov r1, sl
00369b04: ldr r2, [sp, #0x18]
00369b08: mov r8, r0
00369b0c: str ip, [sp]
00369b10: bl #0x86f5fc
00369b14: ldr r3, [r7, #8]
00369b18: str r8, [r3, r6, lsl #2]
00369b1c: b #0x369a4c
00369b20: bl #0x30e310
00369b24: rsbeq fp, r2, r4, lsl #1
00369b28: andeq r3, r0, r0, lsr fp
00369b2c: andeq r4, r0, ip, lsr #1
00369b30: andeq r0, r0, r0, lsl #12

# 0x8658cc _ZN3vox7DataObj7IsReadyEv
008658cc: push {r4, r5, r6, lr}
008658d0: add r4, r0, #0x58
008658d4: mov r5, r0
008658d8: mov r0, r4
008658dc: bl #0x89347c
008658e0: ldr r3, [r5, #0x50]
008658e4: mov r0, r4
008658e8: rsbs r4, r3, #1
008658ec: movlo r4, #0
008658f0: bl #0x893478
008658f4: mov r0, r4
008658f8: pop {r4, r5, r6, pc}

# 0x86f5a0 _ZN3vox8VoxUtils27LoadDataSourceFromFileAsRAWEPKcii
0086f5a0: push {r4, r5, r6, r7, r8, sl, lr}
0086f5a4: sub sp, sp, #0x2c
0086f5a8: mov r5, r0
0086f5ac: mov r8, r1
0086f5b0: mov r7, r2
0086f5b4: mov sl, r3
0086f5b8: bl #0x862b30
0086f5bc: mov r3, sl
0086f5c0: mov r6, r0
0086f5c4: mov r1, r8
0086f5c8: mov r2, r7
0086f5cc: mov r0, sp
0086f5d0: bl #0x86f558
0086f5d4: mov r0, r5
0086f5d8: mov r1, r6
0086f5dc: mov r2, sp
0086f5e0: bl #0x862700
0086f5e4: mov r0, sp
0086f5e8: bl #0x86ac28
0086f5ec: mov r4, sp
0086f5f0: mov r0, r5
0086f5f4: add sp, sp, #0x2c
0086f5f8: pop {r4, r5, r6, r7, r8, sl, pc}

# 0x8686bc _ZN3vox17VoxEngineInternal13GetDataObjectERNS_10DataHandleE
008686bc: push {r4, r5, r6, r7, lr}
008686c0: sub sp, sp, #0xc
008686c4: ldr r3, [r1]
008686c8: mov r5, r0
008686cc: mov r2, sp
008686d0: mov r0, r1
008686d4: mov r4, r1
008686d8: add r1, sp, #4
008686dc: mov lr, pc
008686e0: ldr pc, [r3, #0x10]
008686e4: ldr r3, [sp]
008686e8: add r3, r3, #0x154
008686ec: ldr r2, [r5, r3, lsl #2]
008686f0: ldr r3, [sp, #4]
008686f4: cmp r2, r3
008686f8: beq #0x868750
008686fc: ldr r3, [r4]
00868700: mov r0, r4
00868704: mov lr, pc
00868708: ldr pc, [r3, #8]
0086870c: mov r2, r0
00868710: mov r3, r1
00868714: add r0, r5, #8
00868718: bl #0x863144
0086871c: subs r7, r0, #0
00868720: beq #0x86876c
00868724: ldr r2, [r7, #0x14]
00868728: ldr r3, [r4]
0086872c: mov r0, r4
00868730: add r1, r2, #0x154
00868734: ldr r1, [r5, r1, lsl #2]
00868738: str r2, [sp]
0086873c: mov lr, pc
00868740: ldr pc, [r3, #0x14]
00868744: mov r0, r7
00868748: add sp, sp, #0xc
0086874c: pop {r4, r5, r6, r7, pc}
00868750: ldr r3, [r4]
00868754: mov r0, r4
00868758: mov lr, pc
0086875c: ldr pc, [r3, #0xc]
00868760: subs r7, r0, #0
00868764: bne #0x868744
00868768: b #0x8686fc
0086876c: add r6, r5, #0x60
00868770: mov r0, r6
00868774: bl #0x893548
00868778: ldr r3, [r4]
0086877c: mov r0, r4
00868780: mov lr, pc
00868784: ldr pc, [r3, #8]
00868788: mov r2, r0
0086878c: mov r3, r1
00868790: add r0, r5, #0x28
00868794: bl #0x863144
00868798: mov r7, r0
0086879c: mov r0, r6
008687a0: bl #0x89351c
008687a4: cmp r7, #0
008687a8: bne #0x868724
008687ac: b #0x868744

# 0x86b144 _ZN3vox17VoxEngineInternal14LoadDataSourceEiPviS1_i
0086b144: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0086b148: ldr r6, [pc, #0x2c0]
0086b14c: sub sp, sp, #0x54
0086b150: cmp r2, #0
0086b154: add r6, pc, r6
0086b158: mov r7, r0
0086b15c: mov r4, r1
0086b160: ldr r5, [sp, #0x78]
0086b164: blt #0x86b1b8
0086b168: ldr r1, [r1, #0x4c4]
0086b16c: cmp r2, r1
0086b170: bge #0x86b1b8
0086b174: add r2, r4, r2, lsl #2
0086b178: ldr r2, [r2, #0x444]
0086b17c: cmp r2, #0
0086b180: beq #0x86b1b8
0086b184: mov r0, r3
0086b188: blx r2
0086b18c: subs r8, r0, #0
0086b190: beq #0x86b1b8
0086b194: cmp r5, #0
0086b198: blt #0x86b1a8
0086b19c: ldr r3, [r4, #0x548]
0086b1a0: cmp r5, r3
0086b1a4: blt #0x86b1e8
0086b1a8: mov r0, r8
0086b1ac: bl #0x86366c
0086b1b0: mov r0, r8
0086b1b4: bl #0x310444
0086b1b8: mov r1, #0
0086b1bc: mov r0, r7
0086b1c0: mvn r2, #0
0086b1c4: mvn r3, #0
0086b1c8: str r1, [sp, #0xc]
0086b1cc: str r1, [sp]
0086b1d0: str r1, [sp, #4]
0086b1d4: str r1, [sp, #8]
0086b1d8: bl #0x868d54
0086b1dc: mov r0, r7
0086b1e0: add sp, sp, #0x54
0086b1e4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0086b1e8: add r5, r5, #0x130
0086b1ec: add r5, r5, #2
0086b1f0: ldr r3, [r4, r5, lsl #2]
0086b1f4: cmp r3, #0
0086b1f8: beq #0x86b1a8
0086b1fc: ldr r0, [sp, #0x7c]
0086b200: blx r3
0086b204: subs sl, r0, #0
0086b208: beq #0x86b1a8
0086b20c: ldr r3, [r8]
0086b210: mov r0, r8
0086b214: mov lr, pc
0086b218: ldr pc, [r3, #0x14]
0086b21c: subs sb, r0, #0
0086b220: beq #0x86b3e4
0086b224: ldr r3, [sl]
0086b228: mov r0, sl
0086b22c: mov r1, sb
0086b230: mov lr, pc
0086b234: ldr pc, [r3, #0x10]
0086b238: subs r3, r0, #0
0086b23c: beq #0x86b3d0
0086b240: ldr ip, [r3, #0x10]
0086b244: ldr fp, [r3, #4]
0086b248: ldr r2, [sl]
0086b24c: str ip, [sp, #0x14]
0086b250: ldr ip, [r3, #0xc]
0086b254: mov r1, r3
0086b258: mov r0, sl
0086b25c: str ip, [sp, #0x18]
0086b260: ldr r3, [r3, #8]
0086b264: str r3, [sp, #0x1c]
0086b268: mov lr, pc
0086b26c: ldr pc, [r2, #0x14]
0086b270: cmp fp, #0
0086b274: ble #0x86b3d0
0086b278: mov r0, r4
0086b27c: bl #0x869314
0086b280: strd r0, r1, [sp, #0x20]
0086b284: mov r1, #0
0086b288: mov r0, #0x60
0086b28c: bl #0x310648
0086b290: ldr r2, [pc, #0x17c]
0086b294: mov r5, r0
0086b298: mov r3, #0
0086b29c: ldr r2, [r6, r2]
0086b2a0: ldrd r0, r1, [sp, #0x20]
0086b2a4: str r3, [r5, #0x10]
0086b2a8: add r2, r2, #8
0086b2ac: strd r0, r1, [r5, #8]
0086b2b0: str r2, [r5]
0086b2b4: add r0, r5, #0x18
0086b2b8: str r3, [sp, #0x10]
0086b2bc: bl #0x8935d0
0086b2c0: ldr ip, [sp, #0x80]
0086b2c4: ldr r2, [pc, #0x14c]
0086b2c8: add r1, r5, #0x40
0086b2cc: str ip, [r5, #0x1c]
0086b2d0: ldr r2, [r6, r2]
0086b2d4: ldr ip, [sp, #0x14]
0086b2d8: mvn r0, #0
0086b2dc: add r2, r2, #8
0086b2e0: str ip, [r5, #0x34]
0086b2e4: str r2, [r5]
0086b2e8: ldr r2, [sp, #0x18]
0086b2ec: str r2, [r5, #0x30]
0086b2f0: ldr ip, [sp, #0x1c]
0086b2f4: str r1, [r5, #0x44]
0086b2f8: str r0, [r5, #0x48]
0086b2fc: str ip, [r5, #0x2c]
0086b300: str fp, [r5, #0x28]
0086b304: ldr r3, [sp, #0x10]
0086b308: str r0, [r5, #0x24]
0086b30c: str r1, [r5, #0x40]
0086b310: str r3, [r5, #0x50]
0086b314: str r3, [r5, #0x20]
0086b318: strb r3, [r5, #0x4c]
0086b31c: strb r3, [r5, #0x4d]
0086b320: str r8, [r5, #0x38]
0086b324: str sl, [r5, #0x3c]
0086b328: add r0, r5, #0x58
0086b32c: bl #0x8935d0
0086b330: mov r1, sb
0086b334: ldr r3, [r8]
0086b338: mov r0, r8
0086b33c: mov lr, pc
0086b340: ldr pc, [r3, #0x18]
0086b344: cmp r5, #0
0086b348: beq #0x86b3e4
0086b34c: ldr r3, [r4, #0x590]
0086b350: add r8, sp, #0x28
0086b354: str r3, [r5, #0x14]
0086b358: ldr r1, [r4, #0x590]
0086b35c: ldr r3, [pc, #0xb8]
0086b360: add r0, r1, #0x154
0086b364: ldr ip, [r4, r0, lsl #2]
0086b368: ldr lr, [r6, r3]
0086b36c: mov r0, r8
0086b370: ldrd r2, r3, [r5, #8]
0086b374: str lr, [sp]
0086b378: str ip, [sp, #8]
0086b37c: str r1, [sp, #0xc]
0086b380: str r5, [sp, #4]
0086b384: bl #0x868d54
0086b388: ldr r3, [r4, #0x590]
0086b38c: add r6, r4, #0x60
0086b390: mov r0, r6
0086b394: add r3, r3, #1
0086b398: and r3, r3, #0xf
0086b39c: str r3, [r4, #0x590]
0086b3a0: bl #0x8934ac
0086b3a4: mov r1, r5
0086b3a8: add r0, r4, #0x28
0086b3ac: bl #0x8648f4
0086b3b0: mov r0, r6
0086b3b4: bl #0x893480
0086b3b8: mov r0, r7
0086b3bc: mov r1, r8
0086b3c0: bl #0x868e7c
0086b3c4: mov r0, r8
0086b3c8: bl #0x86ac28
0086b3cc: b #0x86b1dc
0086b3d0: mov r1, sb
0086b3d4: ldr r3, [r8]
0086b3d8: mov r0, r8
0086b3dc: mov lr, pc
0086b3e0: ldr pc, [r3, #0x18]
0086b3e4: mov r0, r8
0086b3e8: bl #0x86366c
0086b3ec: mov r0, r8
0086b3f0: bl #0x310444
0086b3f4: ldr r3, [sl]
0086b3f8: mov r0, sl
0086b3fc: mov lr, pc
0086b400: ldr pc, [r3]
0086b404: mov r0, sl
0086b408: bl #0x310444
0086b40c: b #0x86b1b8
0086b410: andseq sb, r2, ip, lsr sb
0086b414: andeq r4, r0, r4, lsr r7
0086b418: strdeq r1, r2, [r0], -r0
0086b41c: muleq r0, r8, r8

# 0x53128c nativeLoadSoundBig
0053128c: push {r4, lr}
00531290: ldr lr, [pc, #0x34]
00531294: ldr r3, [pc, #0x34]
00531298: ldr r1, [pc, #0x34]
0053129c: add lr, pc, lr
005312a0: ldr r2, [lr, r3]
005312a4: add r1, pc, r1
005312a8: mov r3, r0
005312ac: ldr ip, [r2]
005312b0: ldr r2, [r1, #8]
005312b4: ldr r1, [r1]
005312b8: mov r0, ip
005312bc: ldr ip, [ip]
005312c0: mov lr, pc
005312c4: ldr pc, [ip, #0x234]
005312c8: pop {r4, pc}
005312cc: strdeq r3, r4, [r6], #-0x74
005312d0: muleq r0, r4, sl
005312d4: subeq r5, ip, r8, ror r1

# 0x888f98 PlayMenuDependency__ZN3vox18StreamCFileFactoryEPv
00888f98: push {r4, r5, r6, lr}
00888f9c: mov r1, #0
00888fa0: mov r5, r0
00888fa4: mov r0, #0x24
00888fa8: bl #0x310648
00888fac: mov r1, r5
00888fb0: mov r4, r0
00888fb4: bl #0x888f18
00888fb8: mov r0, r4
00888fbc: pop {r4, r5, r6, pc}

# 0x43ae10 _Z17NativePlaySoundFXRKN7gameswf7fn_callE
0043ae10: push {r4, lr}
0043ae14: ldr r3, [r0, #0x10]
0043ae18: ldr r4, [pc, #0x74]
0043ae1c: sub sp, sp, #8
0043ae20: cmp r3, #1
0043ae24: add r4, pc, r4
0043ae28: beq #0x43ae34
0043ae2c: add sp, sp, #8
0043ae30: pop {r4, pc}
0043ae34: ldr r3, [r0, #0xc]
0043ae38: ldr r2, [r0, #0x14]
0043ae3c: mov r0, #0xc
0043ae40: ldr r3, [r3]
0043ae44: mla r0, r0, r2, r3
0043ae48: ldrb r3, [r0, #1]
0043ae4c: sub r3, r3, #3
0043ae50: uxtb r3, r3
0043ae54: cmp r3, #1
0043ae58: bhi #0x43ae2c
0043ae5c: bl #0x796f5c
0043ae60: bl #0x37ba84
0043ae64: cmn r0, #1
0043ae68: beq #0x43ae2c
0043ae6c: ldr r3, [pc, #0x24]
0043ae70: mov ip, #0
0043ae74: mov r1, r0
0043ae78: ldr lr, [r4, r3]
0043ae7c: mov r2, ip
0043ae80: mov r3, ip
0043ae84: ldr r0, [lr]
0043ae88: str ip, [sp]
0043ae8c: bl #0x36bc34
0043ae90: b #0x43ae2c
0043ae94: subseq sb, r5, ip, ror #24
0043ae98: andeq r0, r0, r4, lsr #27

# 0x888f18 PlayMenuDependency__ZN3vox11StreamCFileC1EPKc
00888f18: ldr r3, [pc, #0x70]
00888f1c: ldr r2, [pc, #0x70]
00888f20: push {r4, r5, r6, r7, r8, lr}
00888f24: add r3, pc, r3
00888f28: ldr r2, [r3, r2]
00888f2c: mov r5, r0
00888f30: mov r6, #0
00888f34: add r2, r2, #8
00888f38: str r6, [r0, #4]
00888f3c: str r2, [r5], #8
00888f40: mov r4, r0
00888f44: str r5, [r0, #0x18]
00888f48: str r5, [r0, #0x1c]
00888f4c: mov r0, r5
00888f50: mov r7, r1
00888f54: bl #0x888c30
00888f58: ldr r3, [r4, #0x18]
00888f5c: cmp r7, r6
00888f60: strb r6, [r3]
00888f64: beq #0x888f88
00888f68: mov r0, r7
00888f6c: bl #0x30de54
00888f70: mov r1, r7
00888f74: add r2, r7, r0
00888f78: mov r0, r5
00888f7c: bl #0x888b70
00888f80: mov r0, r4
00888f84: bl #0x888a60
00888f88: mov r0, r4
00888f8c: pop {r4, r5, r6, r7, r8, pc}
00888f90: andseq fp, r0, ip, ror #22
00888f94: andeq r3, r0, r0, lsr r8

# 0x86f558 _ZN3vox8VoxUtils22LoadDataSourceFromFileEPKcii
0086f558: push {r4, r5, r6, r7, lr}
0086f55c: sub sp, sp, #0x14
0086f560: mov r4, r0
0086f564: mov r7, r1
0086f568: mov r6, r2
0086f56c: mov r5, r3
0086f570: bl #0x862b30
0086f574: mov ip, #0
0086f578: mov r1, r0
0086f57c: mov r3, r7
0086f580: mov r0, r4
0086f584: mov r2, #1
0086f588: stm sp, {r6, ip}
0086f58c: str r5, [sp, #8]
0086f590: bl #0x8627e0
0086f594: mov r0, r4
0086f598: add sp, sp, #0x14
0086f59c: pop {r4, r5, r6, r7, pc}

# 0x531618 nativeUnloadSoundBig
00531618: push {r4, lr}
0053161c: ldr lr, [pc, #0x34]
00531620: ldr r3, [pc, #0x34]
00531624: ldr r1, [pc, #0x34]
00531628: add lr, pc, lr
0053162c: ldr r2, [lr, r3]
00531630: add r1, pc, r1
00531634: mov r3, r0
00531638: ldr ip, [r2]
0053163c: ldr r2, [r1, #0x30]
00531640: ldr r1, [r1]
00531644: mov r0, ip
00531648: ldr ip, [ip]
0053164c: mov lr, pc
00531650: ldr pc, [ip, #0x234]
00531654: pop {r4, pc}
00531658: subeq r3, r6, r8, ror #8
0053165c: muleq r0, r4, sl
00531660: subeq r4, ip, ip, ror #27

# 0x894330 PlayMenuDependency__ZN3vox19FileSystemInterface8OpenFileEPcNS_17VoxFileAccessModeE
00894330: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00894334: ldr r5, [pc, #0x254]
00894338: ldr r7, [pc, #0x254]
0089433c: mov r8, r1
00894340: add r5, pc, r5
00894344: ldr r3, [r5, r7]
00894348: ldr r1, [pc, #0x248]
0089434c: sub sp, sp, #0x34
00894350: ldr r3, [r3]
00894354: add r6, sp, #0x14
00894358: mov r4, r0
0089435c: str r2, [sp, #4]
00894360: add r1, pc, r1
00894364: add r2, sp, #0x10
00894368: mov r0, r6
0089436c: str r3, [sp, #0x2c]
00894370: bl #0x86f338
00894374: mov r2, r4
00894378: ldr r3, [r2, #0xc]!
0089437c: cmp r3, r2
00894380: beq #0x8943b0
00894384: ldr r3, [r3]
00894388: cmp r2, r3
0089438c: bne #0x894384
00894390: ldr r3, [r4, #0x10]
00894394: add r2, r3, #8
00894398: cmp r6, r2
0089439c: beq #0x8943b0
008943a0: ldr r2, [r3, #0x18]
008943a4: mov r0, r6
008943a8: ldr r1, [r3, #0x1c]
008943ac: bl #0x888b70
008943b0: mov r0, r8
008943b4: bl #0x30de54
008943b8: mov r1, r8
008943bc: add r2, r8, r0
008943c0: mov r0, r6
008943c4: bl #0x871718
008943c8: ldr r3, [r4, #8]
008943cc: cmp r3, #0
008943d0: beq #0x8943e0
008943d4: ldrb r2, [r4, #4]
008943d8: cmp r2, #0
008943dc: bne #0x894508
008943e0: ldr fp, [pc, #0x1b4]
008943e4: ldr r3, [r5, fp]
008943e8: ldr r0, [sp, #0x28]
008943ec: ldr r1, [sp, #4]
008943f0: mov lr, pc
008943f4: ldr pc, [r3, #0x10]
008943f8: subs sl, r0, #0
008943fc: moveq r8, sl
00894400: beq #0x894430
00894404: mov r0, #0xc
00894408: mov r1, #0
0089440c: bl #0x310648
00894410: ldr r3, [pc, #0x188]
00894414: mov r2, #0
00894418: mov r8, r0
0089441c: ldr r3, [r5, r3]
00894420: str r2, [r0, #8]
00894424: str sl, [r0, #4]
00894428: add r3, r3, #8
0089442c: str r3, [r0]
00894430: ldr r3, [r4, #8]
00894434: cmp r3, #0
00894438: beq #0x894448
0089443c: ldrb sb, [r4, #4]
00894440: cmp sb, #0
00894444: beq #0x894498
00894448: subs r3, sl, #0
0089444c: movne r3, #1
00894450: cmp r8, #0
00894454: movne r3, #0
00894458: cmp r3, #0
0089445c: bne #0x894574
00894460: ldr r0, [sp, #0x28]
00894464: cmp r0, r6
00894468: beq #0x894478
0089446c: cmp r0, #0
00894470: beq #0x894478
00894474: bl #0x310444
00894478: ldr r3, [r5, r7]
0089447c: ldr r2, [sp, #0x2c]
00894480: mov r0, r8
00894484: ldr r3, [r3]
00894488: cmp r2, r3
0089448c: bne #0x89458c
00894490: add sp, sp, #0x34
00894494: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00894498: cmp sl, #0
0089449c: bne #0x894448
008944a0: mov r0, r3
008944a4: ldr ip, [r3]
008944a8: ldr r1, [sp, #0x28]
008944ac: add r2, sp, #0xc
008944b0: add r3, sp, #8
008944b4: mov lr, pc
008944b8: ldr pc, [ip, #8]
008944bc: cmp r0, #0
008944c0: beq #0x894460
008944c4: ldr r2, [r4, #8]
008944c8: ldr r1, [sp, #4]
008944cc: ldr r3, [r5, fp]
008944d0: ldr r0, [r2, #0x1c]
008944d4: mov lr, pc
008944d8: ldr pc, [r3, #0x10]
008944dc: subs sl, r0, #0
008944e0: beq #0x894460
008944e4: mov r1, sb
008944e8: mov r0, #0x18
008944ec: bl #0x310648
008944f0: mov r1, sl
008944f4: ldr r2, [sp, #0xc]
008944f8: ldr r3, [sp, #8]
008944fc: mov r8, r0
00894500: bl #0x893d70
00894504: b #0x894448
00894508: mov r0, r3
0089450c: ldr ip, [r3]
00894510: ldr r1, [sp, #0x28]
00894514: add r2, sp, #8
00894518: add r3, sp, #0xc
0089451c: mov lr, pc
00894520: ldr pc, [ip, #8]
00894524: cmp r0, #0
00894528: beq #0x8943e0
0089452c: ldr r3, [r4, #8]
00894530: ldr fp, [pc, #0x64]
00894534: ldr r1, [sp, #4]
00894538: ldr r0, [r3, #0x1c]
0089453c: ldr r3, [r5, fp]
00894540: mov lr, pc
00894544: ldr pc, [r3, #0x10]
00894548: subs sl, r0, #0
0089454c: beq #0x8943e4
00894550: mov r1, #0
00894554: mov r0, #0x18
00894558: bl #0x310648
0089455c: mov r1, sl
00894560: ldr r2, [sp, #8]
00894564: ldr r3, [sp, #0xc]
00894568: mov r8, r0
0089456c: bl #0x893d70
00894570: b #0x894430
00894574: ldr r3, [r5, fp]
00894578: mov r0, sl
0089457c: mov lr, pc
00894580: ldr pc, [r3, #0x14]
00894584: mov r8, #0
00894588: b #0x894460
0089458c: bl #0x30e310
00894590: andseq r0, r0, r0, asr r7
00894594: andeq r4, r0, ip, lsr #1
00894598: andeq r7, r3, r8, lsr #9
0089459c: andeq r1, r0, r8, ror r5
008945a0: andeq r2, r0, ip, lsl sl
