# 0x3180a4 _GLOBAL__I_.._.._sources_Utils_UserProperties.cpp
003180a4: ldr r3, [pc, #0x14]
003180a8: mov r2, #0x3f000000
003180ac: add r3, pc, r3
003180b0: str r2, [r3, #8]
003180b4: str r2, [r3]
003180b8: str r2, [r3, #4]
003180bc: bx lr
003180c0: mlseq r8, ip, r8, r7

# 0x318178 _ZN14UserPropertiesD1Ev
00318178: push {r4, r5, r6, lr}
0031817c: ldr r3, [pc, #0x4c]
00318180: ldr r2, [pc, #0x4c]
00318184: ldr r1, [r0, #0x14]
00318188: add r3, pc, r3
0031818c: ldr r2, [r3, r2]
00318190: cmp r1, #0
00318194: mov r4, r0
00318198: add r2, r2, #8
0031819c: str r2, [r0]
003181a0: beq #0x3181c8
003181a4: add r5, r0, #4
003181a8: mov r0, r5
003181ac: ldr r1, [r4, #8]
003181b0: bl #0x3180c4
003181b4: mov r3, #0
003181b8: str r5, [r4, #0x10]
003181bc: str r3, [r4, #0x14]
003181c0: str r5, [r4, #0xc]
003181c4: str r3, [r4, #8]
003181c8: mov r0, r4
003181cc: pop {r4, r5, r6, pc}
003181d0: rsbeq ip, r7, r8, lsl #18
003181d4: andeq r2, r0, r0, lsl r8

# 0x3181d8 _ZN14UserPropertiesD0Ev
003181d8: push {r4, lr}
003181dc: mov r4, r0
003181e0: bl #0x318178
003181e4: mov r0, r4
003181e8: bl #0x310440
003181ec: mov r0, r4
003181f0: pop {r4, pc}

# 0x3181f4 _ZN14UserPropertiesD2Ev
003181f4: push {r4, r5, r6, lr}
003181f8: ldr r3, [pc, #0x4c]
003181fc: ldr r2, [pc, #0x4c]
00318200: ldr r1, [r0, #0x14]
00318204: add r3, pc, r3
00318208: ldr r2, [r3, r2]
0031820c: cmp r1, #0
00318210: mov r4, r0
00318214: add r2, r2, #8
00318218: str r2, [r0]
0031821c: beq #0x318244
00318220: add r5, r0, #4
00318224: mov r0, r5
00318228: ldr r1, [r4, #8]
0031822c: bl #0x3180c4
00318230: mov r3, #0
00318234: str r5, [r4, #0x10]
00318238: str r3, [r4, #0x14]
0031823c: str r5, [r4, #0xc]
00318240: str r3, [r4, #8]
00318244: mov r0, r4
00318248: pop {r4, r5, r6, pc}
0031824c: rsbeq ip, r7, ip, lsl #17
00318250: andeq r2, r0, r0, lsl r8

# 0x318e18 _ZN14UserProperties11AddPropertyEPKcS1_
00318e18: push {r4, r5, lr}
00318e1c: sub sp, sp, #0xc
00318e20: add r3, sp, #8
00318e24: str r1, [r3, #-4]!
00318e28: mov r1, r3
00318e2c: add r0, r0, #4
00318e30: mov r4, r2
00318e34: bl #0x318cb4
00318e38: mov r5, r0
00318e3c: mov r0, r4
00318e40: bl #0x30de54
00318e44: mov r1, r4
00318e48: add r2, r4, r0
00318e4c: mov r0, r5
00318e50: bl #0x3109e0
00318e54: add sp, sp, #0xc
00318e58: pop {r4, r5, pc}

# 0x318e5c _ZN14UserProperties14_ParseKeyValueEPcS0_
00318e5c: push {r4, r5, r6, r7, r8, sb, sl, lr}
00318e60: ldrb r3, [r1]
00318e64: ldr ip, [pc, #0x168]
00318e68: mov r7, r0
00318e6c: cmp r3, #0
00318e70: add ip, pc, ip
00318e74: mov sl, r2
00318e78: beq #0x318f84
00318e7c: ldr r2, [pc, #0x154]
00318e80: mov r8, r1
00318e84: ldr r2, [ip, r2]
00318e88: ldr r0, [r2]
00318e8c: sxtb r2, r3
00318e90: cmn r2, #1
00318e94: uxtab r1, r0, r2
00318e98: beq #0x318f88
00318e9c: ldrb r2, [r1, #1]
00318ea0: tst r2, #7
00318ea4: beq #0x318f88
00318ea8: cmp r3, #0
00318eac: beq #0x318f84
00318eb0: ldrb r4, [r8, #1]
00318eb4: add r5, r8, #1
00318eb8: cmp r4, #0
00318ebc: beq #0x318f10
00318ec0: sxtb r3, r4
00318ec4: cmn r3, #1
00318ec8: beq #0x318f10
00318ecc: uxtab r3, r0, r3
00318ed0: ldrb r3, [r3, #1]
00318ed4: tst r3, #7
00318ed8: movne r5, r8
00318edc: beq #0x318f10
00318ee0: ldrb r4, [r5, #2]
00318ee4: cmp r4, #0
00318ee8: sxtb r3, r4
00318eec: beq #0x318f98
00318ef0: cmn r3, #1
00318ef4: uxtab r2, r0, r3
00318ef8: beq #0x318f98
00318efc: ldrb r3, [r2, #1]
00318f00: add r5, r5, #1
00318f04: tst r3, #7
00318f08: bne #0x318ee0
00318f0c: add r5, r5, #1
00318f10: mov r3, #0
00318f14: cmp sl, #0
00318f18: strb r3, [r5]
00318f1c: beq #0x318fb8
00318f20: ldr r6, [pc, #0xb4]
00318f24: mov r0, sl
00318f28: add r6, pc, r6
00318f2c: mov r1, r6
00318f30: bl #0x30ebd4
00318f34: cmp r0, #0
00318f38: beq #0x318fa0
00318f3c: add sb, r0, #3
00318f40: mov r1, r6
00318f44: mov r0, sb
00318f48: bl #0x30ebd4
00318f4c: cmp sb, #0
00318f50: cmpne r0, #0
00318f54: mov r6, r0
00318f58: movne r3, #0
00318f5c: moveq r3, #1
00318f60: beq #0x318fa0
00318f64: strb r3, [r0]
00318f68: mov r1, r8
00318f6c: mov r0, r7
00318f70: mov r2, sb
00318f74: bl #0x318e18
00318f78: mov r3, #0x25
00318f7c: strb r3, [r6]
00318f80: strb r4, [r5]
00318f84: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00318f88: ldrb r3, [r8, #1]!
00318f8c: cmp r3, #0
00318f90: bne #0x318e8c
00318f94: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00318f98: add r5, r5, #2
00318f9c: b #0x318f10
00318fa0: mov r0, r7
00318fa4: mov r1, r8
00318fa8: mov r2, sl
00318fac: bl #0x318e18
00318fb0: strb r4, [r5]
00318fb4: b #0x318f84
00318fb8: ldr r2, [pc, #0x20]
00318fbc: mov r0, r7
00318fc0: mov r1, r8
00318fc4: add r2, pc, r2
00318fc8: bl #0x318e18
00318fcc: strb r4, [r5]
00318fd0: b #0x318f84
00318fd4: rsbeq fp, r7, r0, lsr #24
00318fd8: ldrdeq r1, r2, [r0], -ip
00318fdc: subseq r5, sl, r8, asr #16
00318fe0: subseq r2, fp, r4, asr #16

# 0x318fe4 _ZN14UserProperties10_ParseLineEPc
00318fe4: push {r4, r5, r6, lr}
00318fe8: mov r5, r1
00318fec: mov r6, r0
00318ff0: mov r1, #0x3d
00318ff4: mov r0, r5
00318ff8: bl #0x30ec28
00318ffc: subs r4, r0, #0
00319000: beq #0x319028
00319004: mov r3, #0
00319008: mov r2, r4
0031900c: strb r3, [r2], #1
00319010: mov r0, r6
00319014: mov r1, r5
00319018: bl #0x318e5c
0031901c: mov r3, #0x3d
00319020: strb r3, [r4]
00319024: pop {r4, r5, r6, pc}
00319028: mov r0, r6
0031902c: mov r1, r5
00319030: mov r2, r4
00319034: pop {r4, r5, r6, lr}
00319038: b #0x318e5c

# 0x31903c _ZN14UserProperties16_ParsePropertiesEPKc
0031903c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00319040: ldr r4, [pc, #0xf4]
00319044: ldr r5, [pc, #0xf4]
00319048: sub sp, sp, #0x2c
0031904c: add r4, pc, r4
00319050: ldr r3, [r4, r5]
00319054: subs r2, r1, #0
00319058: mov r6, r0
0031905c: ldr r3, [r3]
00319060: str r3, [sp, #0x24]
00319064: beq #0x3190e4
00319068: add r7, sp, #0xc
0031906c: mov r0, r7
00319070: add r2, sp, #8
00319074: bl #0x3140ec
00319078: mov fp, #0xa
0031907c: ldr sl, [sp, #0x20]
00319080: mov sb, #0
00319084: b #0x3190a0
00319088: mov r1, sl
0031908c: strb sb, [r8]
00319090: mov r0, r6
00319094: bl #0x318fe4
00319098: add sl, r8, #1
0031909c: strb fp, [r8]
003190a0: mov r0, sl
003190a4: mov r1, #0xa
003190a8: bl #0x30ec28
003190ac: subs r8, r0, #0
003190b0: bne #0x319088
003190b4: mov r0, r6
003190b8: mov r1, sl
003190bc: bl #0x318fe4
003190c0: mov r0, r7
003190c4: bl #0x318254
003190c8: ldr r3, [r4, r5]
003190cc: ldr r2, [sp, #0x24]
003190d0: ldr r3, [r3]
003190d4: cmp r2, r3
003190d8: bne #0x319138
003190dc: add sp, sp, #0x2c
003190e0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003190e4: ldr r3, [pc, #0x58]
003190e8: ldr r3, [r4, r3]
003190ec: ldr r3, [r3]
003190f0: cmp r3, #2
003190f4: streq r2, [r2]
003190f8: beq #0x3190c8
003190fc: cmp r3, #1
00319100: bne #0x3190c8
00319104: ldr r0, [pc, #0x3c]
00319108: ldr r1, [pc, #0x3c]
0031910c: ldr r2, [pc, #0x3c]
00319110: ldr r0, [r4, r0]
00319114: ldr r3, [pc, #0x38]
00319118: mov ip, #0x25
0031911c: add r1, pc, r1
00319120: add r2, pc, r2
00319124: add r3, pc, r3
00319128: add r0, r0, #0xa8
0031912c: str ip, [sp]
00319130: bl #0x30e004
00319134: b #0x3190c8
00319138: bl #0x30e310
0031913c: rsbeq fp, r7, r4, asr #20
00319140: andeq r4, r0, ip, lsr #1
00319144: andeq r3, r0, r0, asr #19
00319148: andeq r1, r0, r0, asr #19
0031914c: ldrheq r5, [sl], #-0x2c
00319150: subseq r5, sl, r8, asr r6
00319154: subseq r5, sl, ip, asr r6

# 0x319158 _ZN14UserPropertiesC1EPKc
00319158: ldr r2, [pc, #0x48]
0031915c: push {r4, r5, r6, lr}
00319160: ldr r5, [pc, #0x44]
00319164: add r2, pc, r2
00319168: mov ip, #0
0031916c: ldr r5, [r2, r5]
00319170: mov r3, r0
00319174: str ip, [r0, #8]
00319178: add r5, r5, #8
0031917c: str r5, [r0]
00319180: cmp r1, ip
00319184: strb ip, [r3, #4]!
00319188: mov r4, r0
0031918c: str r3, [r0, #0x10]
00319190: str ip, [r0, #0x14]
00319194: str r3, [r0, #0xc]
00319198: beq #0x3191a0
0031919c: bl #0x31903c
003191a0: mov r0, r4
003191a4: pop {r4, r5, r6, pc}
003191a8: rsbeq fp, r7, ip, lsr #18
003191ac: andeq r2, r0, r0, lsl r8

# 0x3191b0 _ZN14UserPropertiesC2EPKc
003191b0: ldr r2, [pc, #0x48]
003191b4: push {r4, r5, r6, lr}
003191b8: ldr r5, [pc, #0x44]
003191bc: add r2, pc, r2
003191c0: mov ip, #0
003191c4: ldr r5, [r2, r5]
003191c8: mov r3, r0
003191cc: str ip, [r0, #8]
003191d0: add r5, r5, #8
003191d4: str r5, [r0]
003191d8: cmp r1, ip
003191dc: strb ip, [r3, #4]!
003191e0: mov r4, r0
003191e4: str r3, [r0, #0x10]
003191e8: str ip, [r0, #0x14]
003191ec: str r3, [r0, #0xc]
003191f0: beq #0x3191f8
003191f4: bl #0x31903c
003191f8: mov r0, r4
003191fc: pop {r4, r5, r6, pc}

# 0x31f594 _ZNK11Application15GetCurrentLevelEv
0031f594: ldr r3, [pc, #0x10]
0031f598: ldr r2, [pc, #0x10]
0031f59c: add r3, pc, r3
0031f5a0: ldr r2, [r3, r2]
0031f5a4: ldr r0, [r2]
0031f5a8: bx lr

# 0x340ca8 _Z14GetNewInstanceI11LevelConfigEP10ObjectBasev
00340ca8: push {r4, lr}
00340cac: mov r1, #0
00340cb0: mov r0, #0x318
00340cb4: bl #0x310570
00340cb8: mov r1, #4
00340cbc: mov r4, r0
00340cc0: bl #0x3f51a8
00340cc4: mov r0, r4
00340cc8: pop {r4, pc}

# 0x340f74 _Z14GetNewInstanceI8RoomZoneEP10ObjectBasev
00340f74: push {r4, lr}
00340f78: mov r1, #0
00340f7c: mov r0, #0x39c
00340f80: bl #0x310570
00340f84: mov r1, #0xb
00340f88: mov r4, r0
00340f8c: bl #0x396558
00340f90: mov r0, r4
00340f94: pop {r4, pc}

# 0x340f98 _Z14GetNewInstanceI6ModuleEP10ObjectBasev
00340f98: push {r4, lr}
00340f9c: mov r1, #0
00340fa0: movw r0, #0x41c
00340fa4: bl #0x310570
00340fa8: mov r1, #5
00340fac: mov r4, r0
00340fb0: bl #0x389fa8
00340fb4: mov r0, r4
00340fb8: pop {r4, pc}

# 0x34b724 _ZN13ObjectManager5SpawnEPKcS1_bb
0034b724: push {r4, r5, r6, r7, r8, lr}
0034b728: sub sp, sp, #8
0034b72c: ldrb ip, [sp, #0x24]
0034b730: mvn lr, #0
0034b734: mov r4, r0
0034b738: str lr, [sp]
0034b73c: str ip, [sp, #4]
0034b740: mov r6, r1
0034b744: mov r5, r2
0034b748: mov r7, r3
0034b74c: ldrb r8, [sp, #0x20]
0034b750: bl #0x34b520
0034b754: mov r0, r4
0034b758: mov r1, #0
0034b75c: bl #0x33fdc0
0034b760: cmp r0, #0
0034b764: beq #0x34b7f4
0034b768: mov r1, #1
0034b76c: mov r0, r4
0034b770: bl #0x33fdc0
0034b774: add r0, r0, #4
0034b778: bl #0x513d78
0034b77c: mov r1, #1
0034b780: mov r0, r4
0034b784: bl #0x33fdc0
0034b788: add r0, r0, #4
0034b78c: bl #0x5136ec
0034b790: mov r1, #1
0034b794: mov r0, r4
0034b798: bl #0x33fdc0
0034b79c: mov r1, r7
0034b7a0: bl #0x34ac18
0034b7a4: mov r1, #1
0034b7a8: mov r0, r4
0034b7ac: bl #0x33fdc0
0034b7b0: mov r7, r0
0034b7b4: mov r0, r5
0034b7b8: bl #0x30de54
0034b7bc: mov r1, r5
0034b7c0: add r2, r5, r0
0034b7c4: add r0, r7, #0x48
0034b7c8: bl #0x3109e0
0034b7cc: cmp r8, #0
0034b7d0: beq #0x34b838
0034b7d4: mov r1, #1
0034b7d8: mov r0, r4
0034b7dc: bl #0x33fdc0
0034b7e0: ldr r3, [r0]
0034b7e4: mov lr, pc
0034b7e8: ldr pc, [r3, #0x38]
0034b7ec: cmp r0, #0
0034b7f0: bne #0x34b800
0034b7f4: mov r0, r4
0034b7f8: add sp, sp, #8
0034b7fc: pop {r4, r5, r6, r7, r8, pc}
0034b800: mov r1, #0
0034b804: mov r0, r4
0034b808: bl #0x33fdc0
0034b80c: add r5, r6, #0x34
0034b810: mov r7, r0
0034b814: mov r0, r5
0034b818: bl #0x343168
0034b81c: str r7, [r0, #8]
0034b820: ldr r3, [r6, #0x38]
0034b824: str r5, [r0]
0034b828: str r3, [r0, #4]
0034b82c: str r0, [r3]
0034b830: str r0, [r6, #0x38]
0034b834: b #0x34b7f4
0034b838: mov r1, #1
0034b83c: mov r0, r4
0034b840: bl #0x33fdc0
0034b844: ldr r3, [r0]
0034b848: mov lr, pc
0034b84c: ldr pc, [r3, #0x1c]
0034b850: mov r0, r4
0034b854: mov r1, #1
0034b858: bl #0x33fdc0
0034b85c: mov r1, #1
0034b860: bl #0x33e6d4
0034b864: b #0x34b7d4

# 0x350e5c _ZN12SceneManager12SearchByTypeEPN6glitch5scene10ISceneNodeERSt6vectorIS3_NS0_4core10SAllocatorIS3_LNS0_6memory13E_MEMORY_HINTE0EEEENS1_17E_SCENE_NODE_TYPEE
00350e5c: subs ip, r1, #0
00350e60: push {r4, lr}
00350e64: beq #0x350e7c
00350e68: mov r1, r3
00350e6c: ldr r4, [r0]
00350e70: mov r3, ip
00350e74: mov lr, pc
00350e78: ldr pc, [r4, #0x20]
00350e7c: pop {r4, pc}

# 0x3524a0 _ZN12SceneManager12AddNodeToMapEPN6glitch5scene10ISceneNodeE
003524a0: push {r4, r5, r6, r7, lr}
003524a4: ldr r6, [r0, #0x28c]
003524a8: sub sp, sp, #0x14
003524ac: mov r5, r0
003524b0: cmp r6, #0
003524b4: mov r4, r1
003524b8: beq #0x352550
003524bc: ldr r3, [r4]
003524c0: mov r0, r4
003524c4: ldr r3, [r3, #-0xc]
003524c8: add r3, r4, r3
003524cc: ldr r2, [r3, #4]
003524d0: add r2, r2, #1
003524d4: str r2, [r3, #4]
003524d8: bl #0x597290
003524dc: cmp r0, #0
003524e0: beq #0x352518
003524e4: ldr r3, [r4]
003524e8: add r6, sp, #4
003524ec: mov r0, r6
003524f0: mov r1, r4
003524f4: ldr r7, [r3, #0xa4]
003524f8: bl #0x597180
003524fc: mov r0, r4
00352500: mov r1, r6
00352504: blx r7
00352508: ldr r3, [r4]
0035250c: mov r0, r4
00352510: mov lr, pc
00352514: ldr pc, [r3, #0x68]
00352518: mov r0, r4
0035251c: bl #0x50f220
00352520: ldr r3, [r5, #0x28c]
00352524: mov r1, r4
00352528: mov r0, r3
0035252c: ldr r3, [r3]
00352530: mov lr, pc
00352534: ldr pc, [r3, #0x5c]
00352538: ldr r3, [r4]
0035253c: ldr r0, [r3, #-0xc]
00352540: add r0, r4, r0
00352544: bl #0x31d584
00352548: add sp, sp, #0x14
0035254c: pop {r4, r5, r6, r7, pc}
00352550: mov r1, r6
00352554: mov r0, #0x150
00352558: bl #0x5341ac
0035255c: mvn r1, #0
00352560: mov r7, r0
00352564: bl #0x5839d8
00352568: ldr r3, [r5, #4]
0035256c: str r7, [r5, #0x28c]
00352570: mov r1, r7
00352574: mov r0, r3
00352578: ldr r3, [r3]
0035257c: mov lr, pc
00352580: ldr pc, [r3, #0x5c]
00352584: ldr r3, [r5, #0x28c]
00352588: ldr r2, [r3]
0035258c: ldr r0, [r2, #-0xc]
00352590: add r0, r3, r0
00352594: bl #0x31d584
00352598: ldr r3, [r5, #0x28c]
0035259c: mov r1, r6
003525a0: mov r0, r3
003525a4: ldr r3, [r3]
003525a8: mov lr, pc
003525ac: ldr pc, [r3, #0x48]
003525b0: b #0x3524bc

# 0x3596f8 _ZN12SceneManager9LoadSceneEPKcS1_bb
003596f8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003596fc: ldr r4, [pc, #0x308]
00359700: ldr r7, [pc, #0x308]
00359704: subs r5, r2, #0
00359708: add r4, pc, r4
0035970c: ldr r2, [r4, r7]
00359710: sub sp, sp, #0x3c
00359714: str r0, [sp, #8]
00359718: ldr r2, [r2]
0035971c: mov r6, r1
00359720: mov fp, r3
00359724: ldrb sb, [sp, #0x60]
00359728: str r2, [sp, #0x34]
0035972c: beq #0x35973c
00359730: ldrsb r3, [r5]
00359734: cmp r3, #0
00359738: bne #0x359990
0035973c: ldr r3, [pc, #0x2d0]
00359740: ldr r8, [pc, #0x2d0]
00359744: mov r1, r6
00359748: ldr r0, [r4, r3]
0035974c: mov r2, #1
00359750: ldr r3, [r4, r8]
00359754: ldr r0, [r0, #0x10]
00359758: ldr r0, [r0, #0x10]
0035975c: bl #0x61bbd4
00359760: mov sl, r0
00359764: cmp sl, #0
00359768: beq #0x3597b0
0035976c: mov r0, r6
00359770: bl #0x30de54
00359774: mov r1, r6
00359778: add r2, r6, r0
0035977c: add r0, sl, #0x1bc
00359780: bl #0x3109e0
00359784: cmp r5, #0
00359788: add r3, sl, #0x1d4
0035978c: beq #0x3599f8
00359790: mov r0, r5
00359794: str r3, [sp, #4]
00359798: bl #0x30de54
0035979c: ldr r3, [sp, #4]
003597a0: add r2, r5, r0
003597a4: mov r1, r5
003597a8: mov r0, r3
003597ac: bl #0x3109e0
003597b0: cmp sb, #0
003597b4: beq #0x3597c8
003597b8: cmp sl, #0
003597bc: beq #0x3597c8
003597c0: mov r0, sl
003597c4: bl #0x35cc0c
003597c8: subs sb, r5, #0
003597cc: movne sb, #1
003597d0: cmp sl, #0
003597d4: cmpne r5, #0
003597d8: bne #0x359954
003597dc: cmp sl, #0
003597e0: beq #0x3598a0
003597e4: mov r0, sl
003597e8: mov r1, #2
003597ec: bl #0x59719c
003597f0: cmp sb, #0
003597f4: beq #0x359898
003597f8: ldr r8, [sl, #0xf4]
003597fc: cmp r8, #0
00359800: subne r8, r8, #4
00359804: ldr r3, [r8]
00359808: mov r0, r8
0035980c: mov lr, pc
00359810: ldr pc, [r3, #0x24]
00359814: ldr r1, [pc, #0x200]
00359818: add r1, pc, r1
0035981c: bl #0x30ebd4
00359820: cmp r0, #0
00359824: beq #0x359898
00359828: ldr r5, [r8, #0xf4]!
0035982c: cmp r5, r8
00359830: beq #0x359898
00359834: ldr r3, [pc, #0x1e4]
00359838: ldr sb, [pc, #0x1e4]
0035983c: add r3, pc, r3
00359840: str r3, [sp, #0xc]
00359844: ldr r3, [pc, #0x1dc]
00359848: add sb, pc, sb
0035984c: add r3, pc, r3
00359850: str r3, [sp, #0x10]
00359854: ldr r3, [pc, #0x1d0]
00359858: add r3, pc, r3
0035985c: str r3, [sp, #0x14]
00359860: cmp r5, #0
00359864: moveq r6, r5
00359868: subne r6, r5, #4
0035986c: ldr r3, [r6]
00359870: mov r0, r6
00359874: ldr r5, [r5]
00359878: mov lr, pc
0035987c: ldr pc, [r3, #0x24]
00359880: mov r1, sb
00359884: bl #0x30ebd4
00359888: cmp r0, #0
0035988c: beq #0x3598e0
00359890: cmp r8, r5
00359894: bne #0x359860
00359898: cmp fp, #0
0035989c: bne #0x3598c0
003598a0: ldr r3, [r4, r7]
003598a4: ldr r2, [sp, #0x34]
003598a8: mov r0, sl
003598ac: ldr r3, [r3]
003598b0: cmp r2, r3
003598b4: bne #0x359a08
003598b8: add sp, sp, #0x3c
003598bc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003598c0: ldr r2, [sp, #8]
003598c4: mov r1, sl
003598c8: ldr r3, [r2, #4]
003598cc: mov r0, r3
003598d0: ldr r3, [r3]
003598d4: mov lr, pc
003598d8: ldr pc, [r3, #0x5c]
003598dc: b #0x3598a0
003598e0: ldr r3, [r6]
003598e4: mov r0, r6
003598e8: mov lr, pc
003598ec: ldr pc, [r3, #0x24]
003598f0: ldr r1, [sp, #0xc]
003598f4: bl #0x30ebd4
003598f8: cmp r0, #0
003598fc: bne #0x359890
00359900: ldr r3, [r6]
00359904: mov r0, r6
00359908: mov lr, pc
0035990c: ldr pc, [r3, #0x24]
00359910: ldr r1, [sp, #0x10]
00359914: bl #0x30ebd4
00359918: cmp r0, #0
0035991c: bne #0x359890
00359920: ldr r3, [r6]
00359924: mov r0, r6
00359928: mov lr, pc
0035992c: ldr pc, [r3, #0x24]
00359930: ldr r1, [sp, #0x14]
00359934: bl #0x30ebd4
00359938: cmp r0, #0
0035993c: bne #0x359890
00359940: mov r0, r6
00359944: ldr r3, [r6]
00359948: mov lr, pc
0035994c: ldr pc, [r3, #0x68]
00359950: b #0x359890
00359954: mov r0, r6
00359958: ldr r1, [r4, r8]
0035995c: bl #0x61967c
00359960: subs r5, r0, #0
00359964: beq #0x3597dc
00359968: mov r0, sl
0035996c: ldr r3, [sl]
00359970: mov r1, r5
00359974: mov lr, pc
00359978: ldr pc, [r3, #0x6c]
0035997c: ldr r3, [r5]
00359980: ldr r0, [r3, #-0xc]
00359984: add r0, r5, r0
00359988: bl #0x31d584
0035998c: b #0x3597dc
00359990: add ip, sp, #0x1c
00359994: mov r1, r5
00359998: add r2, sp, #0x18
0035999c: mov r0, ip
003599a0: str ip, [sp, #4]
003599a4: bl #0x3140ec
003599a8: ldr r1, [pc, #0x80]
003599ac: ldr ip, [sp, #4]
003599b0: ldr r8, [pc, #0x60]
003599b4: add r1, pc, r1
003599b8: mov r0, ip
003599bc: add r2, r1, #5
003599c0: bl #0x310804
003599c4: ldr r3, [pc, #0x48]
003599c8: mov r1, r6
003599cc: ldr r2, [sp, #0x30]
003599d0: ldr r0, [r4, r3]
003599d4: ldr r3, [r4, r8]
003599d8: ldr r0, [r0, #0x10]
003599dc: ldr r0, [r0, #0x10]
003599e0: bl #0x61c878
003599e4: ldr ip, [sp, #4]
003599e8: mov sl, r0
003599ec: mov r0, ip
003599f0: bl #0x318254
003599f4: b #0x359764
003599f8: ldr r2, [pc, #0x34]
003599fc: add r2, pc, r2
00359a00: mov r1, r2
00359a04: b #0x3597a8
00359a08: bl #0x30e310
00359a0c: rsbeq fp, r3, r8, lsl #7
00359a10: andeq r4, r0, ip, lsr #1
00359a14: strdeq r3, r4, [r0], -r4
00359a18: andeq r0, r0, ip, lsr #26
00359a1c: subseq r7, r6, r0, ror #7
00359a20: ldrsbeq r7, [r6], #-0x34
00359a24: ldrheq r7, [r6], #-0x38
00359a28: subseq r7, r6, ip, asr #7
00359a2c: subseq r7, r6, r8, asr #7
00359a30: subseq r7, r6, ip, lsr r2
00359a34: subseq r1, r7, ip, lsl #28

# 0x35a0e4 _ZN12SceneManager12SearchByNameEPN6glitch5scene10ISceneNodeEPKcb
0035a0e4: push {r4, r5, r6, r7, r8, sl, lr}
0035a0e8: ldr r4, [pc, #0x90]
0035a0ec: ldr r6, [pc, #0x90]
0035a0f0: cmp r2, #0
0035a0f4: cmpne r1, #0
0035a0f8: add r4, pc, r4
0035a0fc: ldr ip, [r4, r6]
0035a100: mov r5, r1
0035a104: sub sp, sp, #0x24
0035a108: ldr ip, [ip]
0035a10c: moveq r1, #0
0035a110: movne r1, #1
0035a114: mov sl, r0
0035a118: mov r8, r3
0035a11c: str ip, [sp, #0x1c]
0035a120: moveq r5, r1
0035a124: beq #0x35a15c
0035a128: add r7, sp, #4
0035a12c: mov r1, r2
0035a130: mov r0, r7
0035a134: mov r2, sp
0035a138: bl #0x3140ec
0035a13c: mov r1, r5
0035a140: mov r0, sl
0035a144: mov r2, r7
0035a148: mov r3, r8
0035a14c: bl #0x352b74
0035a150: mov r5, r0
0035a154: mov r0, r7
0035a158: bl #0x318254
0035a15c: ldr r3, [r4, r6]
0035a160: ldr r2, [sp, #0x1c]
0035a164: mov r0, r5
0035a168: ldr r3, [r3]
0035a16c: cmp r2, r3
0035a170: bne #0x35a17c
0035a174: add sp, sp, #0x24
0035a178: pop {r4, r5, r6, r7, r8, sl, pc}
0035a17c: bl #0x30e310
0035a180: mlseq r3, r8, sb, sl
0035a184: andeq r4, r0, ip, lsr #1

# 0x35a8e0 _ZNK6glitch5scene10ISceneNode25getAbsoluteTransformationEv
0035a8e0: add r0, r0, #0x24
0035a8e4: bx lr

# 0x37ba84 _ZN6Arrays19GetMemberIDByStringINS_6SoundsEEEiPKc
0037ba84: ldr r3, [pc, #0x60]
0037ba88: ldr r2, [pc, #0x60]
0037ba8c: push {r4, r5, r6, r7, r8, lr}
0037ba90: add r3, pc, r3
0037ba94: ldr r2, [r3, r2]
0037ba98: mov r6, r0
0037ba9c: ldr r5, [r2]
0037baa0: cmp r5, #0
0037baa4: beq #0x37bae4
0037baa8: ldr r2, [pc, #0x44]
0037baac: mov r4, #0
0037bab0: ldr r3, [r3, r2]
0037bab4: ldr r7, [r3]
0037bab8: b #0x37bac8
0037babc: add r4, r4, #1
0037bac0: cmp r4, r5
0037bac4: beq #0x37bae4
0037bac8: ldr r1, [r7, r4, lsl #2]
0037bacc: mov r0, r6
0037bad0: bl #0x30e31c
0037bad4: cmp r0, #0
0037bad8: bne #0x37babc
0037badc: mov r0, r4
0037bae0: pop {r4, r5, r6, r7, r8, pc}
0037bae4: mvn r0, #0
0037bae8: pop {r4, r5, r6, r7, r8, pc}
0037baec: rsbeq sb, r1, r0
0037baf0: andeq r3, r0, r8, lsr sp
0037baf4: andeq r3, r0, r8, lsr #19

# 0x385ed8 _ZN7GSLevelC1Ev
00385ed8: ldr r2, [pc, #0x48]
00385edc: ldr ip, [pc, #0x48]
00385ee0: mov r3, r0
00385ee4: add r2, pc, r2
00385ee8: ldr ip, [r2, ip]
00385eec: push {r4, lr}
00385ef0: add ip, ip, #8
00385ef4: mov r4, r0
00385ef8: str ip, [r3], #4
00385efc: mov r0, r3
00385f00: str r3, [r4, #0x14]
00385f04: str r3, [r4, #0x18]
00385f08: mov r1, #0x10
00385f0c: bl #0x31167c
00385f10: ldr r2, [r4, #0x14]
00385f14: mov r3, #0
00385f18: mov r0, r4
00385f1c: strb r3, [r2]
00385f20: str r3, [r4, #0x34]
00385f24: pop {r4, pc}
00385f28: rsbeq lr, r0, ip, lsr #23
00385f2c: ldrdeq r0, r1, [r0], -r8

# 0x385f30 _ZN7GSLevelC2Ev
00385f30: ldr r2, [pc, #0x48]
00385f34: ldr ip, [pc, #0x48]
00385f38: mov r3, r0
00385f3c: add r2, pc, r2
00385f40: ldr ip, [r2, ip]
00385f44: push {r4, lr}
00385f48: add ip, ip, #8
00385f4c: mov r4, r0
00385f50: str ip, [r3], #4
00385f54: mov r0, r3
00385f58: str r3, [r4, #0x14]
00385f5c: str r3, [r4, #0x18]
00385f60: mov r1, #0x10
00385f64: bl #0x31167c
00385f68: ldr r2, [r4, #0x14]
00385f6c: mov r3, #0
00385f70: mov r0, r4
00385f74: strb r3, [r2]
00385f78: str r3, [r4, #0x34]
00385f7c: pop {r4, pc}
00385f80: rsbeq lr, r0, r4, asr fp
00385f84: ldrdeq r0, r1, [r0], -r8

# 0x3860bc _ZN7GSLevel4DtorEPK12StateMachine
003860bc: push {r4, r5, r6, lr}
003860c0: mov r5, r0
003860c4: ldr r0, [r0, #0x34]
003860c8: ldr r4, [pc, #0x68]
003860cc: cmp r0, #0
003860d0: add r4, pc, r4
003860d4: beq #0x386124
003860d8: bl #0x3f0690
003860dc: bl #0x42ca8c
003860e0: ldr r1, [pc, #0x54]
003860e4: add r1, pc, r1
003860e8: bl #0x42d1f0
003860ec: subs r3, r0, #0
003860f0: beq #0x386100
003860f4: ldr r3, [r3]
003860f8: mov lr, pc
003860fc: ldr pc, [r3, #0x10]
00386100: ldr r3, [r5, #0x34]
00386104: cmp r3, #0
00386108: beq #0x386124
0038610c: mov r0, r3
00386110: ldr r3, [r3]
00386114: mov lr, pc
00386118: ldr pc, [r3, #4]
0038611c: mov r3, #0
00386120: str r3, [r5, #0x34]
00386124: ldr r3, [pc, #0x14]
00386128: mov r2, #0
0038612c: ldr r3, [r4, r3]
00386130: str r2, [r3]
00386134: pop {r4, r5, r6, pc}
00386138: rsbeq lr, r0, r0, asr #19
0038613c: subseq fp, r3, r4, asr #13
00386140: andeq r1, r0, r4, ror #26

# 0x386190 _ZN7GSLevel4CtorEPK12StateMachine
00386190: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00386194: ldr r5, [pc, #0x128]
00386198: ldr r3, [pc, #0x128]
0038619c: mov r4, r0
003861a0: add r5, pc, r5
003861a4: sub sp, sp, #0x1c
003861a8: ldr r0, [r5, r3]
003861ac: bl #0x475664
003861b0: mov r1, #0
003861b4: mov r0, #0x1ac
003861b8: ldr sb, [r4, #0x18]
003861bc: bl #0x310570
003861c0: ldrb lr, [r4, #0x2c]
003861c4: ldrb ip, [r4, #0x2d]
003861c8: ldr fp, [r4, #0x24]
003861cc: ldr r7, [r4, #0x28]
003861d0: ldr r8, [r4, #0x30]
003861d4: ldr sl, [r4, #0x40]
003861d8: ldr r2, [r4, #0x1c]
003861dc: ldr r3, [r4, #0x20]
003861e0: mov r1, sb
003861e4: str lr, [sp, #8]
003861e8: str ip, [sp, #0xc]
003861ec: mov r6, r0
003861f0: str fp, [sp]
003861f4: str r7, [sp, #4]
003861f8: str r8, [sp, #0x10]
003861fc: str sl, [sp, #0x14]
00386200: bl #0x3f3128
00386204: ldr r2, [pc, #0xc0]
00386208: mov r3, #1
0038620c: str r3, [r4, #0x38]
00386210: ldr r2, [r5, r2]
00386214: str r6, [r4, #0x34]
00386218: str r6, [r2]
0038621c: strb r3, [r4, #0x3c]
00386220: bl #0x42ca8c
00386224: ldr r1, [pc, #0xa4]
00386228: add r1, pc, r1
0038622c: bl #0x42d1f0
00386230: mov r4, r0
00386234: bl #0x7fd794
00386238: ldrb r3, [r0, #5]
0038623c: cmp r3, #0
00386240: bne #0x38628c
00386244: cmp r4, #0
00386248: beq #0x386284
0038624c: bl #0x42ca8c
00386250: mov r1, r4
00386254: bl #0x4317e8
00386258: add r0, r4, #0x48
0038625c: ldr r5, [r4, #4]
00386260: bl #0x386144
00386264: ldr r2, [pc, #0x68]
00386268: mov ip, #0
0038626c: ldr r1, [r4, #0x4c]
00386270: mov r0, r5
00386274: add r2, pc, r2
00386278: mov r3, ip
0038627c: str ip, [sp]
00386280: bl #0x7abe0c
00386284: add sp, sp, #0x1c
00386288: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038628c: bl #0x320e98
00386290: ldr r3, [r0, #0x34]
00386294: cmp r3, #3
00386298: bne #0x386244
0038629c: bl #0x42ca8c
003862a0: ldr r3, [r0, #0xf4]
003862a4: mov r1, r4
003862a8: mov r0, r3
003862ac: ldr r3, [r3]
003862b0: mov lr, pc
003862b4: ldr pc, [r3, #0x40]
003862b8: cmp r0, #0
003862bc: bne #0x386284
003862c0: b #0x386244

# 0x386818 _ZN7GSLevel9LoadLevelEPKcijjjbbii
00386818: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038681c: sub sp, sp, #0xc
00386820: mov sl, r3
00386824: mov r6, r0
00386828: stm sp, {r1, r2}
0038682c: bl #0x30de54
00386830: ldr r5, [pc, #0x74]
00386834: ldr r4, [pc, #0x74]
00386838: add r2, r6, r0
0038683c: add r5, pc, r5
00386840: ldr r4, [r5, r4]
00386844: mov r1, r6
00386848: ldr r8, [sp, #0x30]
0038684c: add r0, r4, #4
00386850: ldr sb, [sp, #0x3c]
00386854: ldr fp, [sp, #0x40]
00386858: ldrb r7, [sp, #0x34]
0038685c: ldrb r6, [sp, #0x38]
00386860: bl #0x3109e0
00386864: ldr r3, [sp, #4]
00386868: ldr ip, [sp]
0038686c: mov r1, r4
00386870: str r3, [r4, #0x20]
00386874: ldr r3, [pc, #0x38]
00386878: mov r2, #0
0038687c: str ip, [r4, #0x1c]
00386880: ldr r3, [r5, r3]
00386884: str sl, [r4, #0x24]
00386888: str r8, [r4, #0x28]
0038688c: ldr r0, [r3, #0x18]
00386890: strb r7, [r4, #0x2c]
00386894: strb r6, [r4, #0x2d]
00386898: str sb, [r4, #0x30]
0038689c: str fp, [r4, #0x40]
003868a0: add sp, sp, #0xc
003868a4: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003868a8: b #0x33a388
003868ac: rsbeq lr, r0, r4, asr r2
003868b0: andeq r2, r0, r0, lsl #5
003868b4: strdeq r3, r4, [r0], -r4

# 0x388218 _ZN6PFRoom17ExtendBoundingBoxERK4aabbIfE
00388218: push {r4, r5, r6, r7, r8, lr}
0038821c: ldr r6, [r1]
00388220: ldr r7, [r0, #0x3c]
00388224: mov r4, r0
00388228: mov r5, r1
0038822c: mov r0, r6
00388230: mov r1, r7
00388234: bl #0x30e70c
00388238: cmp r0, #0
0038823c: moveq r6, r7
00388240: str r6, [r4, #0x3c]
00388244: ldr r7, [r5, #4]
00388248: ldr r8, [r4, #0x40]
0038824c: mov r0, r7
00388250: mov r1, r8
00388254: bl #0x30e70c
00388258: cmp r0, #0
0038825c: moveq r7, r8
00388260: str r7, [r4, #0x40]
00388264: ldr r8, [r4, #0x44]
00388268: ldr r7, [r5, #8]
0038826c: mov r1, r8
00388270: mov r0, r7
00388274: bl #0x30e70c
00388278: cmp r0, #0
0038827c: moveq r7, r8
00388280: str r7, [r4, #0x44]
00388284: ldr r8, [r4, #0x48]
00388288: ldr r7, [r5, #0xc]
0038828c: mov r0, r8
00388290: mov r1, r7
00388294: bl #0x30e70c
00388298: cmp r0, #0
0038829c: moveq r7, r8
003882a0: str r7, [r4, #0x48]
003882a4: ldr r8, [r4, #0x4c]
003882a8: ldr r7, [r5, #0x10]
003882ac: mov r0, r8
003882b0: mov r1, r7
003882b4: bl #0x30e70c
003882b8: cmp r0, #0
003882bc: moveq r7, r8
003882c0: str r7, [r4, #0x4c]
003882c4: ldr r7, [r5, #0x14]
003882c8: ldr r5, [r4, #0x50]
003882cc: mov r1, r7
003882d0: mov r0, r5
003882d4: bl #0x30e70c
003882d8: cmp r0, #0
003882dc: moveq r7, r5
003882e0: ldr r5, [r4, #0x20]
003882e4: str r7, [r4, #0x50]
003882e8: mov r0, r6
003882ec: ldr r7, [r5, #0x14]
003882f0: mov r1, r7
003882f4: bl #0x30e70c
003882f8: cmp r0, #0
003882fc: moveq r6, r7
00388300: str r6, [r5, #0x14]
00388304: ldr r6, [r4, #0x40]
00388308: ldr r7, [r5, #0x18]
0038830c: mov r0, r6
00388310: mov r1, r7
00388314: bl #0x30e70c
00388318: cmp r0, #0
0038831c: moveq r6, r7
00388320: str r6, [r5, #0x18]
00388324: ldr r6, [r4, #0x44]
00388328: ldr r7, [r5, #0x1c]
0038832c: mov r0, r6
00388330: mov r1, r7
00388334: bl #0x30e70c
00388338: cmp r0, #0
0038833c: moveq r6, r7
00388340: str r6, [r5, #0x1c]
00388344: ldr r6, [r4, #0x48]
00388348: ldr r7, [r5, #0x20]
0038834c: mov r1, r6
00388350: mov r0, r7
00388354: bl #0x30e70c
00388358: cmp r0, #0
0038835c: moveq r6, r7
00388360: str r6, [r5, #0x20]
00388364: ldr r6, [r4, #0x4c]
00388368: ldr r7, [r5, #0x24]
0038836c: mov r1, r6
00388370: mov r0, r7
00388374: bl #0x30e70c
00388378: cmp r0, #0
0038837c: moveq r6, r7
00388380: str r6, [r5, #0x24]
00388384: ldr r4, [r4, #0x50]
00388388: ldr r6, [r5, #0x28]
0038838c: mov r1, r4
00388390: mov r0, r6
00388394: bl #0x30e70c
00388398: cmp r0, #0
0038839c: moveq r4, r6
003883a0: str r4, [r5, #0x28]
003883a4: pop {r4, r5, r6, r7, r8, pc}

# 0x388b20 _ZN6Module8InitPostEv
00388b20: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00388b24: ldr r4, [pc, #0x114]
00388b28: ldr r5, [pc, #0x114]
00388b2c: sub sp, sp, #0x54
00388b30: add r4, pc, r4
00388b34: ldr r3, [r4, r5]
00388b38: mov r8, r0
00388b3c: ldr r3, [r3]
00388b40: str r3, [sp, #0x4c]
00388b44: bl #0x388a98
00388b48: ldr r3, [r8, #0x2d8]
00388b4c: cmp r3, #0
00388b50: beq #0x388be0
00388b54: ldr r3, [r3, #8]
00388b58: ldr r7, [pc, #0xe8]
00388b5c: add sl, sp, #0x18
00388b60: mov r0, r3
00388b64: ldr r3, [r3]
00388b68: mov lr, pc
00388b6c: ldr pc, [r3, #0x34]
00388b70: ldr r3, [pc, #0xd4]
00388b74: ldr r1, [pc, #0xd4]
00388b78: add r7, pc, r7
00388b7c: ldr r3, [r4, r3]
00388b80: ldr r2, [r7, #0xc]
00388b84: add r1, pc, r1
00388b88: mov fp, r0
00388b8c: mov r0, sl
00388b90: ldr sb, [r3, #0x38]
00388b94: bl #0x30eae4
00388b98: ldr r3, [r7, #0xc]
00388b9c: ldr r2, [pc, #0xb0]
00388ba0: add r6, sp, #0xc
00388ba4: add r3, r3, #1
00388ba8: str r3, [r7, #0xc]
00388bac: mov ip, #1
00388bb0: mov r7, #0
00388bb4: add r2, pc, r2
00388bb8: mov r3, sl
00388bbc: mov r1, sb
00388bc0: mov r0, r6
00388bc4: stm sp, {r7, ip}
00388bc8: bl #0x34b724
00388bcc: mov r1, r7
00388bd0: mov r0, r6
00388bd4: bl #0x33fdc0
00388bd8: subs r7, r0, #0
00388bdc: bne #0x388bfc
00388be0: ldr r3, [r4, r5]
00388be4: ldr r2, [sp, #0x4c]
00388be8: ldr r3, [r3]
00388bec: cmp r2, r3
00388bf0: bne #0x388c3c
00388bf4: add sp, sp, #0x54
00388bf8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00388bfc: ldr r3, [r7, #0xf4]
00388c00: cmp r3, #0xb
00388c04: bne #0x388be0
00388c08: mov r2, r6
00388c0c: ldr ip, [r2], #4
00388c10: ldr r1, [r6, #4]
00388c14: add r3, r8, #0x400
00388c18: ldr r2, [r2, #4]
00388c1c: add r3, r3, #4
00388c20: str ip, [r8, #0x400]
00388c24: str r1, [r3], #4
00388c28: str r2, [r3]
00388c2c: mov r1, fp
00388c30: bl #0x397594
00388c34: str r8, [r7, #0x38c]
00388c38: b #0x388be0
00388c3c: bl #0x30e310
00388c40: rsbeq fp, r0, r0, ror #30
00388c44: andeq r4, r0, ip, lsr #1
00388c48: rsbeq sb, r1, r4, lsr fp
00388c4c: strdeq r3, r4, [r0], -r4
00388c50: subseq sb, r3, r4, asr #13
00388c54: ldrheq r7, [r3], #-0x7c

# 0x38a38c _ZNK6Module11_ChooseXmlsERSsS0_
0038a38c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038a390: ldr fp, [pc, #0x4d8]
0038a394: ldr r3, [pc, #0x4d8]
0038a398: sub sp, sp, #0x234
0038a39c: add fp, pc, fp
0038a3a0: str r3, [sp, #0x14]
0038a3a4: ldr r3, [fp, r3]
0038a3a8: add r6, r0, #0x378
0038a3ac: cmp r6, r1
0038a3b0: ldr r3, [r3]
0038a3b4: mov sl, r0
0038a3b8: str r1, [sp, #0xc]
0038a3bc: str r2, [sp, #0x10]
0038a3c0: str r3, [sp, #0x22c]
0038a3c4: beq #0x38a3d8
0038a3c8: mov r0, r1
0038a3cc: ldr r2, [sl, #0x388]
0038a3d0: ldr r1, [sl, #0x38c]
0038a3d4: bl #0x3109e0
0038a3d8: ldr r0, [sp, #0x10]
0038a3dc: add sb, sl, #0x390
0038a3e0: cmp sb, r0
0038a3e4: beq #0x38a3f4
0038a3e8: ldr r1, [sl, #0x3a4]
0038a3ec: ldr r2, [sl, #0x3a0]
0038a3f0: bl #0x3109e0
0038a3f4: ldr r2, [sl, #0x3bc]
0038a3f8: ldr r3, [sl, #0x3b8]
0038a3fc: cmp r2, r3
0038a400: beq #0x38a67c
0038a404: ldr r2, [sl, #0x3d4]
0038a408: ldr r3, [sl, #0x3d0]
0038a40c: cmp r2, r3
0038a410: beq #0x38a67c
0038a414: add r4, sp, #0x214
0038a418: mov r7, #0
0038a41c: mov r0, r4
0038a420: mov r1, #0x10
0038a424: str r7, [sp, #0x38]
0038a428: str r7, [sp, #0x3c]
0038a42c: str r7, [sp, #0x40]
0038a430: str r7, [sp, #0x2c]
0038a434: str r7, [sp, #0x30]
0038a438: str r7, [sp, #0x34]
0038a43c: str r7, [sp, #0x20]
0038a440: str r7, [sp, #0x24]
0038a444: str r7, [sp, #0x28]
0038a448: str r4, [sp, #0x224]
0038a44c: str r4, [sp, #0x228]
0038a450: bl #0x31167c
0038a454: ldr r3, [sp, #0x224]
0038a458: add r5, sp, #0x17c
0038a45c: mov r0, r5
0038a460: strb r7, [r3]
0038a464: add r1, sl, #0x3a8
0038a468: bl #0x389a6c
0038a46c: add r8, sp, #0x38
0038a470: mov r0, r5
0038a474: mov r1, r4
0038a478: bl #0x38a258
0038a47c: ldr r3, [r0]
0038a480: ldr r3, [r3, #-0xc]
0038a484: add r0, r0, r3
0038a488: ldr r3, [r0, #8]
0038a48c: tst r3, #5
0038a490: beq #0x38a69c
0038a494: mov r1, r6
0038a498: mov r0, r8
0038a49c: add r6, sp, #0xe4
0038a4a0: bl #0x32bad8
0038a4a4: mov r0, r6
0038a4a8: add r1, sl, #0x3c0
0038a4ac: bl #0x389a6c
0038a4b0: add r7, sp, #0x2c
0038a4b4: mov r0, r6
0038a4b8: mov r1, r4
0038a4bc: bl #0x38a258
0038a4c0: ldr r3, [r0]
0038a4c4: ldr r3, [r3, #-0xc]
0038a4c8: add r0, r0, r3
0038a4cc: ldr r3, [r0, #8]
0038a4d0: tst r3, #5
0038a4d4: beq #0x38a6ac
0038a4d8: mov r1, sb
0038a4dc: mov r0, r7
0038a4e0: bl #0x32bad8
0038a4e4: ldr r2, [sl, #0x3ec]
0038a4e8: ldr r3, [sl, #0x3e8]
0038a4ec: cmp r2, r3
0038a4f0: beq #0x38a754
0038a4f4: add sb, sp, #0x4c
0038a4f8: add r1, sl, #0x3d8
0038a4fc: mov r0, sb
0038a500: bl #0x389a6c
0038a504: add r1, sp, #0x20
0038a508: add r2, sp, #0x44
0038a50c: mov sl, #0
0038a510: str r1, [sp, #0x18]
0038a514: str r2, [sp, #0x1c]
0038a518: mov r0, sb
0038a51c: mov r1, r4
0038a520: bl #0x38a258
0038a524: ldr r3, [r0]
0038a528: ldr r3, [r3, #-0xc]
0038a52c: add r0, r0, r3
0038a530: ldr r3, [r0, #8]
0038a534: tst r3, #5
0038a538: beq #0x38a6bc
0038a53c: ldr r1, [sp, #0x24]
0038a540: ldr r3, [sp, #0x28]
0038a544: rsb sl, sl, #0x64
0038a548: str sl, [sp, #0x48]
0038a54c: cmp r1, r3
0038a550: beq #0x38a85c
0038a554: str sl, [r1]
0038a558: ldr r3, [sp, #0x24]
0038a55c: add r3, r3, #4
0038a560: str r3, [sp, #0x24]
0038a564: mov r0, sb
0038a568: bl #0x389344
0038a56c: ldr r1, [sp, #0x24]
0038a570: ldr r2, [sp, #0x3c]
0038a574: ldr r3, [sp, #0x38]
0038a578: rsb r3, r3, r2
0038a57c: asr r3, r3, #3
0038a580: add sb, r3, r3, lsl #2
0038a584: add sb, sb, sb, lsl #4
0038a588: add sb, sb, sb, lsl #8
0038a58c: add sb, sb, sb, lsl #16
0038a590: add sb, r3, sb, lsl #1
0038a594: ldr r3, [sp, #0x20]
0038a598: rsb r1, r3, r1
0038a59c: cmp sb, r1, asr #2
0038a5a0: beq #0x38a7f0
0038a5a4: ldr r3, [pc, #0x2cc]
0038a5a8: ldr r3, [fp, r3]
0038a5ac: ldr r3, [r3]
0038a5b0: cmp r3, #2
0038a5b4: moveq r3, #0
0038a5b8: streq r3, [r3]
0038a5bc: beq #0x38a5c8
0038a5c0: cmp r3, #1
0038a5c4: beq #0x38a828
0038a5c8: mov r0, #0x64
0038a5cc: bl #0x388c58
0038a5d0: ldr sl, [sp, #0x38]
0038a5d4: ldr r3, [sp, #0x3c]
0038a5d8: rsb r3, sl, r3
0038a5dc: asr r3, r3, #3
0038a5e0: add lr, r3, r3, lsl #2
0038a5e4: add lr, lr, lr, lsl #4
0038a5e8: add lr, lr, lr, lsl #8
0038a5ec: add lr, lr, lr, lsl #16
0038a5f0: adds lr, r3, lr, lsl #1
0038a5f4: beq #0x38a630
0038a5f8: ldr ip, [sp, #0x20]
0038a5fc: ldr r2, [ip]
0038a600: cmp r0, r2
0038a604: movlt sb, #0
0038a608: blt #0x38a70c
0038a60c: mov r3, #0
0038a610: b #0x38a624
0038a614: ldr r1, [ip, r3, lsl #2]
0038a618: add r2, r2, r1
0038a61c: cmp r2, r0
0038a620: bgt #0x38a704
0038a624: add r3, r3, #1
0038a628: cmp r3, lr
0038a62c: bne #0x38a614
0038a630: mov r0, r6
0038a634: bl #0x389344
0038a638: mov r0, r5
0038a63c: bl #0x389344
0038a640: mov r0, r4
0038a644: bl #0x3139ac
0038a648: ldr r0, [sp, #0x20]
0038a64c: cmp r0, #0
0038a650: beq #0x38a66c
0038a654: ldr r1, [sp, #0x28]
0038a658: rsb r1, r0, r1
0038a65c: bic r1, r1, #3
0038a660: cmp r1, #0x80
0038a664: bhi #0x38a820
0038a668: bl #0x708f00
0038a66c: mov r0, r7
0038a670: bl #0x313f30
0038a674: mov r0, r8
0038a678: bl #0x313f30
0038a67c: ldr r0, [sp, #0x14]
0038a680: ldr r2, [sp, #0x22c]
0038a684: ldr r3, [fp, r0]
0038a688: ldr r3, [r3]
0038a68c: cmp r2, r3
0038a690: bne #0x38a86c
0038a694: add sp, sp, #0x234
0038a698: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038a69c: mov r0, r8
0038a6a0: mov r1, r4
0038a6a4: bl #0x32bad8
0038a6a8: b #0x38a470
0038a6ac: mov r0, r7
0038a6b0: mov r1, r4
0038a6b4: bl #0x32bad8
0038a6b8: b #0x38a4b4
0038a6bc: ldr r0, [sp, #0x228]
0038a6c0: bl #0x30e094
0038a6c4: ldr r1, [sp, #0x24]
0038a6c8: ldr r3, [sp, #0x28]
0038a6cc: str r0, [sp, #0x44]
0038a6d0: cmp r1, r3
0038a6d4: beq #0x38a6f4
0038a6d8: str r0, [r1]
0038a6dc: ldr r3, [sp, #0x24]
0038a6e0: add r3, r3, #4
0038a6e4: str r3, [sp, #0x24]
0038a6e8: ldr r3, [sp, #0x44]
0038a6ec: add sl, sl, r3
0038a6f0: b #0x38a518
0038a6f4: ldr r0, [sp, #0x18]
0038a6f8: ldr r2, [sp, #0x1c]
0038a6fc: bl #0x389628
0038a700: b #0x38a6e8
0038a704: mov sb, #0x18
0038a708: mul sb, sb, r3
0038a70c: ldr r1, [sp, #0xc]
0038a710: add sl, sl, sb
0038a714: cmp r1, sl
0038a718: beq #0x38a72c
0038a71c: mov r0, r1
0038a720: ldr r2, [sl, #0x10]
0038a724: ldr r1, [sl, #0x14]
0038a728: bl #0x3109e0
0038a72c: ldr r3, [sp, #0x2c]
0038a730: ldr r2, [sp, #0x10]
0038a734: add sb, r3, sb
0038a738: cmp r2, sb
0038a73c: beq #0x38a630
0038a740: mov r0, r2
0038a744: ldr r1, [sb, #0x14]
0038a748: ldr r2, [sb, #0x10]
0038a74c: bl #0x3109e0
0038a750: b #0x38a630
0038a754: ldr r2, [sp, #0x3c]
0038a758: ldr r3, [sp, #0x38]
0038a75c: mov r0, #0x64
0038a760: rsb r3, r3, r2
0038a764: asr r3, r3, #3
0038a768: add sb, r3, r3, lsl #2
0038a76c: add sb, sb, sb, lsl #4
0038a770: add sb, sb, sb, lsl #8
0038a774: add sb, sb, sb, lsl #16
0038a778: add sb, r3, sb, lsl #1
0038a77c: mov r1, sb
0038a780: bl #0x30e2a4
0038a784: cmp sb, #0
0038a788: str r0, [sp, #0x44]
0038a78c: ldrle r1, [sp, #0x24]
0038a790: ble #0x38a594
0038a794: add r3, sp, #0x20
0038a798: add r0, sp, #0x44
0038a79c: ldr r1, [sp, #0x24]
0038a7a0: mov sl, #0
0038a7a4: str r3, [sp, #0x18]
0038a7a8: str r0, [sp, #0x1c]
0038a7ac: b #0x38a7d0
0038a7b0: ldr r3, [sp, #0x44]
0038a7b4: str r3, [r1]
0038a7b8: ldr r1, [sp, #0x24]
0038a7bc: add r1, r1, #4
0038a7c0: str r1, [sp, #0x24]
0038a7c4: add sl, sl, #1
0038a7c8: cmp sl, sb
0038a7cc: beq #0x38a570
0038a7d0: ldr r3, [sp, #0x28]
0038a7d4: cmp r3, r1
0038a7d8: bne #0x38a7b0
0038a7dc: ldr r0, [sp, #0x18]
0038a7e0: ldr r2, [sp, #0x1c]
0038a7e4: bl #0x389628
0038a7e8: ldr r1, [sp, #0x24]
0038a7ec: b #0x38a7c4
0038a7f0: ldr r2, [sp, #0x30]
0038a7f4: ldr r3, [sp, #0x2c]
0038a7f8: rsb r3, r3, r2
0038a7fc: asr r3, r3, #3
0038a800: add r2, r3, r3, lsl #2
0038a804: add r2, r2, r2, lsl #4
0038a808: add r2, r2, r2, lsl #8
0038a80c: add r2, r2, r2, lsl #16
0038a810: add r3, r3, r2, lsl #1
0038a814: cmp sb, r3
0038a818: bne #0x38a5a4
0038a81c: b #0x38a5c8
0038a820: bl #0x310440
0038a824: b #0x38a66c
0038a828: ldr r0, [pc, #0x4c]
0038a82c: ldr r1, [pc, #0x4c]
0038a830: ldr r2, [pc, #0x4c]
0038a834: ldr r0, [fp, r0]
0038a838: ldr r3, [pc, #0x48]
0038a83c: mov ip, #0xbd
0038a840: add r1, pc, r1
0038a844: add r2, pc, r2
0038a848: add r3, pc, r3
0038a84c: add r0, r0, #0xa8
0038a850: str ip, [sp]
0038a854: bl #0x30e004
0038a858: b #0x38a5c8
0038a85c: add r0, sp, #0x20
0038a860: add r2, sp, #0x48
0038a864: bl #0x389628
0038a868: b #0x38a564
0038a86c: bl #0x30e310

# 0x38a88c _ZNK6Module10LoadModuleEv
0038a88c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038a890: ldr sb, [pc, #0x1fc]
0038a894: ldr r3, [pc, #0x1fc]
0038a898: ldr fp, [pc, #0x1fc]
0038a89c: add sb, pc, sb
0038a8a0: ldr r2, [sb, r3]
0038a8a4: ldr r3, [sb, fp]
0038a8a8: sub sp, sp, #0x84
0038a8ac: ldr r4, [r2]
0038a8b0: ldr r3, [r3]
0038a8b4: mov r5, r0
0038a8b8: cmp r4, #0
0038a8bc: str r3, [sp, #0x7c]
0038a8c0: beq #0x38aa3c
0038a8c4: mov r0, r4
0038a8c8: ldr r1, [r5, #0x40c]
0038a8cc: bl #0x3ef278
0038a8d0: ldr r3, [r5, #0x160]
0038a8d4: add r6, sp, #0x64
0038a8d8: mov r0, r6
0038a8dc: str r3, [r4, #0x160]
0038a8e0: ldr r3, [r5, #0x164]
0038a8e4: mov r1, #0x10
0038a8e8: add r7, sp, #0x4c
0038a8ec: str r3, [r4, #0x164]
0038a8f0: ldr r3, [r5, #0x168]
0038a8f4: mov r8, #0
0038a8f8: str r3, [r4, #0x168]
0038a8fc: str r6, [sp, #0x74]
0038a900: str r6, [sp, #0x78]
0038a904: bl #0x31167c
0038a908: ldr r3, [sp, #0x74]
0038a90c: mov r0, r7
0038a910: mov r1, #0x10
0038a914: strb r8, [r3]
0038a918: str r7, [sp, #0x5c]
0038a91c: str r7, [sp, #0x60]
0038a920: bl #0x31167c
0038a924: ldr r3, [sp, #0x5c]
0038a928: mov r2, r7
0038a92c: mov r0, r5
0038a930: strb r8, [r3]
0038a934: mov r1, r6
0038a938: bl #0x38a38c
0038a93c: ldr r3, [sp, #0x74]
0038a940: ldr r2, [sp, #0x78]
0038a944: cmp r2, r3
0038a948: beq #0x38a998
0038a94c: ldr r8, [pc, #0x14c]
0038a950: add r5, sp, #0x34
0038a954: add sl, sp, #0x18
0038a958: add r8, pc, r8
0038a95c: mov r1, r8
0038a960: mov r2, sl
0038a964: mov r0, r5
0038a968: bl #0x3140ec
0038a96c: mov r1, r6
0038a970: mov r2, r5
0038a974: mov r0, r4
0038a978: bl #0x3f3b40
0038a97c: mov r3, r0
0038a980: mov r0, r5
0038a984: str r3, [sp, #0xc]
0038a988: bl #0x3139ac
0038a98c: ldr r3, [sp, #0xc]
0038a990: cmp r3, #0
0038a994: beq #0x38a95c
0038a998: ldr r3, [sp, #0x5c]
0038a99c: ldr r2, [sp, #0x60]
0038a9a0: cmp r2, r3
0038a9a4: beq #0x38a9f4
0038a9a8: ldr r8, [pc, #0xf4]
0038a9ac: add r5, sp, #0x1c
0038a9b0: add sl, sp, #0x14
0038a9b4: add r8, pc, r8
0038a9b8: mov r1, r8
0038a9bc: mov r2, sl
0038a9c0: mov r0, r5
0038a9c4: bl #0x3140ec
0038a9c8: mov r1, r7
0038a9cc: mov r2, r5
0038a9d0: mov r0, r4
0038a9d4: bl #0x3f3b40
0038a9d8: mov r3, r0
0038a9dc: mov r0, r5
0038a9e0: str r3, [sp, #0xc]
0038a9e4: bl #0x3139ac
0038a9e8: ldr r3, [sp, #0xc]
0038a9ec: cmp r3, #0
0038a9f0: beq #0x38a9b8
0038a9f4: mov r3, #0
0038a9f8: str r3, [r4, #0x168]
0038a9fc: str r3, [r4, #0x160]
0038aa00: str r3, [r4, #0x164]
0038aa04: mvn r1, #0
0038aa08: mov r0, r4
0038aa0c: bl #0x3ef278
0038aa10: mov r0, r7
0038aa14: bl #0x3139ac
0038aa18: mov r0, r6
0038aa1c: bl #0x3139ac
0038aa20: ldr r3, [sb, fp]
0038aa24: ldr r2, [sp, #0x7c]
0038aa28: ldr r3, [r3]
0038aa2c: cmp r2, r3
0038aa30: bne #0x38aa90
0038aa34: add sp, sp, #0x84
0038aa38: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038aa3c: ldr r3, [pc, #0x64]
0038aa40: ldr r3, [sb, r3]
0038aa44: ldr r3, [r3]
0038aa48: cmp r3, #2
0038aa4c: streq r4, [r4]
0038aa50: beq #0x38a8c4
0038aa54: cmp r3, #1
0038aa58: bne #0x38a8c4
0038aa5c: ldr r0, [pc, #0x48]
0038aa60: ldr r1, [pc, #0x48]
0038aa64: ldr r2, [pc, #0x48]
0038aa68: ldr r0, [sb, r0]
0038aa6c: ldr r3, [pc, #0x44]
0038aa70: mov ip, #0x54
0038aa74: add r1, pc, r1
0038aa78: add r2, pc, r2
0038aa7c: add r3, pc, r3
0038aa80: add r0, r0, #0xa8
0038aa84: str ip, [sp]
0038aa88: bl #0x30e004
0038aa8c: b #0x38a8c4
0038aa90: bl #0x30e310

# 0x38aac8 _ZN10GameObject18UpdateAbsoluteAABBEv
0038aac8: push {r4, r5, r6, r7, r8, sb, sl, lr}
0038aacc: mov r4, r0
0038aad0: ldr sl, [r4, #0x148]
0038aad4: ldr r0, [r0, #0x144]
0038aad8: ldr r8, [r4, #0x14c]
0038aadc: ldr r7, [r4, #0x150]
0038aae0: ldr r6, [r4, #0x154]
0038aae4: ldr r5, [r4, #0x158]
0038aae8: ldr r1, [r4, #0x160]
0038aaec: str r0, [r4, #0x12c]
0038aaf0: str sl, [r4, #0x130]
0038aaf4: str r8, [r4, #0x134]
0038aaf8: str r7, [r4, #0x138]
0038aafc: str r6, [r4, #0x13c]
0038ab00: str r5, [r4, #0x140]
0038ab04: bl #0x30eba4
0038ab08: ldr r1, [r4, #0x164]
0038ab0c: str r0, [r4, #0x12c]
0038ab10: mov r0, sl
0038ab14: bl #0x30eba4
0038ab18: ldr r1, [r4, #0x168]
0038ab1c: str r0, [r4, #0x130]
0038ab20: mov r0, r8
0038ab24: bl #0x30eba4
0038ab28: ldr r1, [r4, #0x160]
0038ab2c: str r0, [r4, #0x134]
0038ab30: mov r0, r7
0038ab34: bl #0x30eba4
0038ab38: ldr r1, [r4, #0x164]
0038ab3c: str r0, [r4, #0x138]
0038ab40: mov r0, r6
0038ab44: bl #0x30eba4
0038ab48: ldr r1, [r4, #0x168]
0038ab4c: str r0, [r4, #0x13c]
0038ab50: mov r0, r5
0038ab54: bl #0x30eba4
0038ab58: str r0, [r4, #0x140]
0038ab5c: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x393600 _ZN10GameObject14SetDestinationERK7Point3DIfE
00393600: ldr r3, [r1]
00393604: str r3, [r0, #0x1a8]
00393608: ldr r3, [r1, #4]
0039360c: str r3, [r0, #0x1ac]
00393610: ldr r3, [r1, #8]
00393614: str r3, [r0, #0x1b0]
00393618: bx lr

# 0x393db4 _ZN10GameObject11SetPositionERK7Point3DIfEb
00393db4: push {r4, r5, r6, r7, r8, sb, sl, lr}
00393db8: ldr r5, [r0, #0x2e0]
00393dbc: mov r4, r0
00393dc0: mov r6, r1
00393dc4: cmp r5, #0
00393dc8: mov r7, r2
00393dcc: beq #0x393e2c
00393dd0: ldr r1, [r0, #0x164]
00393dd4: ldr r0, [r6, #4]
00393dd8: bl #0x30e3ac
00393ddc: ldr r1, [r4, #0x168]
00393de0: mov sl, r0
00393de4: ldr r0, [r6, #8]
00393de8: bl #0x30e3ac
00393dec: ldr r1, [r4, #0x160]
00393df0: mov r8, r0
00393df4: ldr r0, [r6]
00393df8: bl #0x30e3ac
00393dfc: mov r1, r0
00393e00: ldr r0, [r5, #0xc]
00393e04: bl #0x30eba4
00393e08: mov r1, sl
00393e0c: str r0, [r5, #0xc]
00393e10: ldr r0, [r5, #0x10]
00393e14: bl #0x30eba4
00393e18: mov r1, r8
00393e1c: str r0, [r5, #0x10]
00393e20: ldr r0, [r5, #0x14]
00393e24: bl #0x30eba4
00393e28: str r0, [r5, #0x14]
00393e2c: ldr r3, [r6]
00393e30: mov r0, r4
00393e34: str r3, [r4, #0x160]
00393e38: ldr r3, [r6, #4]
00393e3c: str r3, [r4, #0x164]
00393e40: ldr r3, [r6, #8]
00393e44: str r3, [r4, #0x168]
00393e48: bl #0x38aac8
00393e4c: ldr r0, [r4, #0x2dc]
00393e50: cmp r0, #0
00393e54: beq #0x393e64
00393e58: ldr r1, [r4, #0x160]
00393e5c: ldr r2, [r4, #0x164]
00393e60: bl #0x46ea80
00393e64: ldr r0, [r4, #0x2d8]
00393e68: cmp r0, #0
00393e6c: beq #0x393e74
00393e70: bl #0x470cb8
00393e74: cmp r7, #0
00393e78: bne #0x393e80
00393e7c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00393e80: mov r0, r4
00393e84: mov r1, r6
00393e88: pop {r4, r5, r6, r7, r8, sb, sl, lr}
00393e8c: b #0x393600

# 0x39594c _ZN14CheckpointZoneC1EN10ObjectBase6GO_IDSE
0039594c: mov r2, #1
00395950: push {r4, r5, r6, lr}
00395954: mov r3, r2
00395958: ldr r5, [pc, #0x4c]
0039595c: mov r4, r0
00395960: bl #0x397ca0
00395964: ldr r3, [pc, #0x44]
00395968: add r5, pc, r5
0039596c: mov r1, #0
00395970: ldr r3, [r5, r3]
00395974: mov r2, r4
00395978: str r1, [r4, #0x38c]
0039597c: add r0, r3, #0xf4
00395980: add ip, r3, #8
00395984: add r3, r3, #0xe8
00395988: str r0, [r4, #0x24]
0039598c: str ip, [r4]
00395990: str r3, [r4, #4]
00395994: strb r1, [r2, #0x388]!
00395998: str r2, [r4, #0x394]
0039599c: str r1, [r4, #0x398]
003959a0: str r2, [r4, #0x390]
003959a4: mov r0, r4
003959a8: pop {r4, r5, r6, pc}
003959ac: subseq pc, pc, r8, lsr #2
003959b0: andeq r2, r0, r4, ror #28

# 0x3959b4 _ZN14CheckpointZoneC2EN10ObjectBase6GO_IDSE
003959b4: mov r2, #1
003959b8: push {r4, r5, r6, lr}
003959bc: mov r3, r2
003959c0: ldr r5, [pc, #0x4c]
003959c4: mov r4, r0
003959c8: bl #0x397ca0
003959cc: ldr r3, [pc, #0x44]
003959d0: add r5, pc, r5
003959d4: mov r1, #0
003959d8: ldr r3, [r5, r3]
003959dc: mov r2, r4
003959e0: str r1, [r4, #0x38c]
003959e4: add r0, r3, #0xf4
003959e8: add ip, r3, #8
003959ec: add r3, r3, #0xe8
003959f0: str r0, [r4, #0x24]
003959f4: str ip, [r4]
003959f8: str r3, [r4, #4]
003959fc: strb r1, [r2, #0x388]!
00395a00: str r2, [r4, #0x394]
00395a04: str r1, [r4, #0x398]
00395a08: str r2, [r4, #0x390]
00395a0c: mov r0, r4
00395a10: pop {r4, r5, r6, pc}
00395a14: subseq pc, pc, r0, asr #1
00395a18: andeq r2, r0, r4, ror #28

# 0x396330 _ZN15QuestMoveInZoneC1EN10ObjectBase6GO_IDSE
00396330: push {r4, r5, r6, lr}
00396334: mov r2, #1
00396338: mov r3, #0
0039633c: ldr r5, [pc, #0x2c]
00396340: mov r4, r0
00396344: bl #0x397ca0
00396348: ldr r3, [pc, #0x24]
0039634c: add r5, pc, r5
00396350: mov r0, r4
00396354: ldr r3, [r5, r3]
00396358: add r2, r3, #0xf4
0039635c: add r1, r3, #8
00396360: add r3, r3, #0xe8
00396364: stm r4, {r1, r3}
00396368: str r2, [r4, #0x24]
0039636c: pop {r4, r5, r6, pc}
00396370: subseq lr, pc, r4, asr #14
00396374: andeq r2, r0, r4, ror lr

# 0x396378 _ZN15QuestMoveInZoneC2EN10ObjectBase6GO_IDSE
00396378: push {r4, r5, r6, lr}
0039637c: mov r2, #1
00396380: mov r3, #0
00396384: ldr r5, [pc, #0x2c]
00396388: mov r4, r0
0039638c: bl #0x397ca0
00396390: ldr r3, [pc, #0x24]
00396394: add r5, pc, r5
00396398: mov r0, r4
0039639c: ldr r3, [r5, r3]
003963a0: add r2, r3, #0xf4
003963a4: add r1, r3, #8
003963a8: add r3, r3, #0xe8
003963ac: stm r4, {r1, r3}
003963b0: str r2, [r4, #0x24]
003963b4: pop {r4, r5, r6, pc}
003963b8: ldrsheq lr, [pc], #-0x6c
003963bc: andeq r2, r0, r4, ror lr

# 0x396548 _ZN8RoomZone8InitPostEv
00396548: b #0x39771c

# 0x396558 _ZN8RoomZoneC1EN10ObjectBase6GO_IDSE
00396558: push {r4, r5, r6, lr}
0039655c: mov r2, #0
00396560: mov r3, #1
00396564: ldr r5, [pc, #0x50]
00396568: mov r4, r0
0039656c: bl #0x397ca0
00396570: ldr r3, [pc, #0x48]
00396574: add r5, pc, r5
00396578: mov r1, #1
0039657c: ldr r3, [r5, r3]
00396580: add r2, r4, #0x394
00396584: strb r1, [r4, #0x389]
00396588: add r0, r3, #0xf4
0039658c: add ip, r3, #8
00396590: add r3, r3, #0xe8
00396594: str r3, [r4, #4]
00396598: mov r3, #0
0039659c: str r0, [r4, #0x24]
003965a0: str ip, [r4]
003965a4: strb r3, [r4, #0x390]
003965a8: str r2, [r4, #0x398]
003965ac: strb r1, [r4, #0x388]
003965b0: str r2, [r4, #0x394]
003965b4: mov r0, r4
003965b8: pop {r4, r5, r6, pc}
003965bc: subseq lr, pc, ip, lsl r5
003965c0: andeq r2, r0, r4, ror r7

# 0x3965c4 _ZN8RoomZoneC2EN10ObjectBase6GO_IDSE
003965c4: push {r4, r5, r6, lr}
003965c8: mov r2, #0
003965cc: mov r3, #1
003965d0: ldr r5, [pc, #0x50]
003965d4: mov r4, r0
003965d8: bl #0x397ca0
003965dc: ldr r3, [pc, #0x48]
003965e0: add r5, pc, r5
003965e4: mov r1, #1
003965e8: ldr r3, [r5, r3]
003965ec: add r2, r4, #0x394
003965f0: strb r1, [r4, #0x389]
003965f4: add r0, r3, #0xf4
003965f8: add ip, r3, #8
003965fc: add r3, r3, #0xe8
00396600: str r3, [r4, #4]
00396604: mov r3, #0
00396608: str r0, [r4, #0x24]
0039660c: str ip, [r4]
00396610: strb r3, [r4, #0x390]
00396614: str r2, [r4, #0x398]
00396618: strb r1, [r4, #0x388]
0039661c: str r2, [r4, #0x394]
00396620: mov r0, r4
00396624: pop {r4, r5, r6, pc}
00396628: ldrheq lr, [pc], #-0x40
0039662c: andeq r2, r0, r4, ror r7

# 0x397594 _ZN4Zone19InitWithBoundingBoxERKN6glitch4core8aabbox3dIfEE
00397594: push {r4, r5, r6, r7, r8, sl, lr}
00397598: mov r8, r1
0039759c: sub sp, sp, #0x34
003975a0: ldr r1, [r1, #4]
003975a4: mov r4, r0
003975a8: ldr r0, [r8, #0x10]
003975ac: bl #0x30e3ac
003975b0: ldr r1, [r8, #8]
003975b4: mov r6, r0
003975b8: ldr r0, [r8, #0x14]
003975bc: bl #0x30e3ac
003975c0: ldr r1, [r8]
003975c4: mov r5, r0
003975c8: ldr r0, [r8, #0xc]
003975cc: bl #0x30e3ac
003975d0: str r6, [r4, #0x378]
003975d4: str r5, [r4, #0x37c]
003975d8: str r0, [r4, #0x374]
003975dc: ldr r3, [r8]
003975e0: ldr r5, [pc, #0x128]
003975e4: str r3, [r4, #0x144]
003975e8: ldr r3, [r8, #4]
003975ec: add r5, pc, r5
003975f0: str r3, [r4, #0x148]
003975f4: ldr r3, [r8, #8]
003975f8: str r3, [r4, #0x14c]
003975fc: ldr r3, [r8, #0xc]
00397600: str r3, [r4, #0x150]
00397604: ldr r3, [r8, #0x10]
00397608: str r3, [r4, #0x154]
0039760c: ldr r3, [r8, #0x14]
00397610: str r3, [r4, #0x158]
00397614: ldr r1, [r8, #0x10]
00397618: ldr r0, [r8, #4]
0039761c: bl #0x30eba4
00397620: mov r1, #0x3f000000
00397624: bl #0x30ed6c
00397628: ldr r1, [r8, #0x14]
0039762c: mov r7, r0
00397630: ldr r0, [r8, #8]
00397634: bl #0x30eba4
00397638: mov r1, #0x3f000000
0039763c: bl #0x30ed6c
00397640: ldr r1, [r8, #0xc]
00397644: mov r6, r0
00397648: ldr r0, [r8]
0039764c: bl #0x30eba4
00397650: mov r1, #0x3f000000
00397654: bl #0x30ed6c
00397658: add r1, sp, #0x24
0039765c: str r0, [sp, #0x24]
00397660: mov r2, #1
00397664: mov r0, r4
00397668: str r7, [sp, #0x28]
0039766c: str r6, [sp, #0x2c]
00397670: bl #0x393db4
00397674: ldrb r3, [r4, #0x380]
00397678: cmp r3, #0
0039767c: beq #0x397708
00397680: ldr r3, [pc, #0x8c]
00397684: mov r1, #0
00397688: mov r0, #0x28
0039768c: ldr r3, [r5, r3]
00397690: mov r6, r1
00397694: ldr sl, [r3, #0x44]
00397698: bl #0x310570
0039769c: ldrb r8, [r4, #0x381]
003976a0: mov ip, #1
003976a4: movw r3, #0x51e
003976a8: cmp r8, r6
003976ac: mvn lr, #4
003976b0: moveq r8, r3
003976b4: movne r8, #4
003976b8: mov r1, sl
003976bc: mov r3, ip
003976c0: mov r2, r4
003976c4: str lr, [sp, #0xc]
003976c8: mov lr, #0x800
003976cc: mov r7, r0
003976d0: str lr, [sp, #0x10]
003976d4: str r8, [sp, #0x14]
003976d8: stm sp, {r6, ip}
003976dc: str r6, [sp, #8]
003976e0: str r6, [sp, #0x18]
003976e4: bl #0x46f2f0
003976e8: ldr r3, [pc, #0x28]
003976ec: mov r0, r4
003976f0: mov r1, r7
003976f4: ldr r3, [r5, r3]
003976f8: mov r2, r6
003976fc: add r3, r3, #8
00397700: str r3, [r7]
00397704: bl #0x394bf8
00397708: add sp, sp, #0x34
0039770c: pop {r4, r5, r6, r7, r8, sl, pc}
00397710: subseq sp, pc, r4, lsr #9
00397714: strdeq r3, r4, [r0], -r4
00397718: andeq r3, r0, r0, lsl #13

# 0x39771c _ZN4Zone8InitPostEv
0039771c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00397720: sub sp, sp, #0x5c
00397724: mov r4, r0
00397728: bl #0x38bd64
0039772c: ldr r3, [r4, #0x274]
00397730: ldr r5, [pc, #0x3d4]
00397734: cmp r0, r3
00397738: add r5, pc, r5
0039773c: blt #0x397748
00397740: add sp, sp, #0x5c
00397744: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00397748: mov r0, r4
0039774c: bl #0x38be5c
00397750: ldr r0, [r4, #0x374]
00397754: ldr r1, [r4, #0x120]
00397758: bl #0x30ed6c
0039775c: ldr r1, [r4, #0x124]
00397760: mov r8, r0
00397764: str r0, [r4, #0x374]
00397768: ldr r0, [r4, #0x378]
0039776c: bl #0x30ed6c
00397770: ldr r1, [r4, #0x128]
00397774: mov r7, r0
00397778: str r0, [r4, #0x378]
0039777c: ldr r0, [r4, #0x37c]
00397780: bl #0x30ed6c
00397784: mov r1, #0x3f000000
00397788: mov r6, r0
0039778c: str r0, [r4, #0x37c]
00397790: mov r0, r8
00397794: bl #0x30ed6c
00397798: add r3, r0, #0x80000000
0039779c: str r3, [r4, #0x144]
003977a0: str r0, [r4, #0x150]
003977a4: mov r1, #0x3f000000
003977a8: mov r0, r7
003977ac: bl #0x30ed6c
003977b0: add r3, r0, #0x80000000
003977b4: str r3, [r4, #0x148]
003977b8: str r0, [r4, #0x154]
003977bc: mov r1, #0x3f000000
003977c0: mov r0, r6
003977c4: bl #0x30ed6c
003977c8: add r3, r0, #0x80000000
003977cc: str r0, [r4, #0x158]
003977d0: str r3, [r4, #0x14c]
003977d4: mov r1, r4
003977d8: ldr r3, [r1], #0x144
003977dc: mov r0, r4
003977e0: mov r2, #0
003977e4: mov lr, pc
003977e8: ldr pc, [r3, #0x9c]
003977ec: ldrb r3, [r4, #0x380]
003977f0: cmp r3, #0
003977f4: bne #0x3978bc
003977f8: ldr r6, [r4, #0x384]
003977fc: cmp r6, #0
00397800: bne #0x397740
00397804: ldr r0, [r4, #0x2d8]
00397808: cmp r0, #0
0039780c: beq #0x397740
00397810: ldr r1, [pc, #0x2f8]
00397814: add r1, pc, r1
00397818: bl #0x470a18
0039781c: cmp r0, #0
00397820: str r0, [r4, #0x384]
00397824: beq #0x397740
00397828: ldr r2, [r0]
0039782c: ldr r1, [pc, #0x2e0]
00397830: movw r3, #0x6164
00397834: ldr r2, [r2, #-0xc]
00397838: ldr r1, [r5, r1]
0039783c: movt r3, #0x6d65
00397840: add r0, r0, r2
00397844: ldr ip, [r0, #4]
00397848: add r2, sp, #0x44
0039784c: add ip, ip, #1
00397850: str ip, [r0, #4]
00397854: ldr r0, [r1, #0x10]
00397858: ldr r1, [r4, #0x384]
0039785c: ldr r0, [r0, #0x1c]
00397860: str r6, [sp, #0x44]
00397864: str r6, [sp, #0x48]
00397868: str r6, [sp, #0x4c]
0039786c: bl #0x350e5c
00397870: ldr r0, [sp, #0x44]
00397874: ldr r3, [sp, #0x48]
00397878: mov r2, r0
0039787c: rsb r3, r0, r3
00397880: asr r3, r3, #2
00397884: cmp r3, #1
00397888: beq #0x397994
0039788c: ldr r3, [pc, #0x284]
00397890: ldr r3, [r5, r3]
00397894: ldr r3, [r3]
00397898: cmp r3, #2
0039789c: streq r6, [r6]
003978a0: beq #0x3978ac
003978a4: cmp r3, #1
003978a8: beq #0x397948
003978ac: cmp r0, #0
003978b0: beq #0x397740
003978b4: bl #0x310450
003978b8: b #0x397740
003978bc: ldr r3, [pc, #0x250]
003978c0: mov r1, #0
003978c4: mov r0, #0x28
003978c8: ldr r3, [r5, r3]
003978cc: mov r6, r1
003978d0: ldr sl, [r3, #0x44]
003978d4: bl #0x310570
003978d8: ldrb r8, [r4, #0x381]
003978dc: mov ip, #1
003978e0: movw r3, #0x51e
003978e4: cmp r8, r6
003978e8: mvn lr, #4
003978ec: moveq r8, r3
003978f0: movne r8, #4
003978f4: mov r1, sl
003978f8: mov r3, ip
003978fc: mov r2, r4
00397900: str lr, [sp, #0xc]
00397904: mov lr, #0x800
00397908: mov r7, r0
0039790c: str lr, [sp, #0x10]
00397910: str r8, [sp, #0x14]
00397914: stm sp, {r6, ip}
00397918: str r6, [sp, #8]
0039791c: str r6, [sp, #0x18]
00397920: bl #0x46f2f0
00397924: ldr r3, [pc, #0x1f0]
00397928: mov r1, r7
0039792c: mov r2, r6
00397930: ldr r3, [r5, r3]
00397934: mov r0, r4
00397938: add r3, r3, #8
0039793c: str r3, [r7]
00397940: bl #0x394bf8
00397944: b #0x3977f8
00397948: ldr r0, [pc, #0x1d0]
0039794c: ldr r1, [pc, #0x1d0]
00397950: ldr r2, [pc, #0x1d0]
00397954: ldr r0, [r5, r0]
00397958: ldr r3, [pc, #0x1cc]
0039795c: add r2, pc, r2
00397960: mov ip, #0x60
00397964: add r3, pc, r3
00397968: add r1, pc, r1
0039796c: add r0, r0, #0xa8
00397970: str ip, [sp]
00397974: bl #0x30e004
00397978: ldr r2, [sp, #0x44]
0039797c: ldr r3, [sp, #0x48]
00397980: mov r0, r2
00397984: rsb r3, r2, r3
00397988: asr r3, r3, #2
0039798c: cmp r3, #1
00397990: bne #0x3978ac
00397994: ldr r5, [r2]
00397998: cmp r5, #0
0039799c: moveq r0, r2
003979a0: beq #0x3978ac
003979a4: ldr r2, [r4, #0x168]
003979a8: ldr r3, [r5]
003979ac: ldr r8, [r4, #0x160]
003979b0: ldr r6, [r4, #0x164]
003979b4: mov r0, r5
003979b8: str r2, [sp, #0x24]
003979bc: mov lr, pc
003979c0: ldr pc, [r3, #0x30]
003979c4: ldr r1, [r0]
003979c8: mov r3, r0
003979cc: mov r0, r8
003979d0: str r1, [sp, #0x2c]
003979d4: ldr fp, [r3, #4]
003979d8: str fp, [sp, #0x30]
003979dc: ldr sb, [r3, #8]
003979e0: str sb, [sp, #0x34]
003979e4: ldr sl, [r3, #0xc]
003979e8: str sl, [sp, #0x38]
003979ec: ldr r7, [r3, #0x10]
003979f0: str r7, [sp, #0x3c]
003979f4: ldr r3, [r3, #0x14]
003979f8: str r3, [sp, #0x20]
003979fc: bl #0x30eba4
00397a00: mov r1, fp
00397a04: str r0, [sp, #0x2c]
00397a08: mov r0, r6
00397a0c: bl #0x30eba4
00397a10: mov r1, sb
00397a14: str r0, [sp, #0x30]
00397a18: ldr r0, [sp, #0x24]
00397a1c: bl #0x30eba4
00397a20: mov r1, sl
00397a24: str r0, [sp, #0x34]
00397a28: mov r0, r8
00397a2c: bl #0x30eba4
00397a30: mov r1, r7
00397a34: str r0, [sp, #0x38]
00397a38: mov r0, r6
00397a3c: bl #0x30eba4
00397a40: ldr r3, [sp, #0x20]
00397a44: str r0, [sp, #0x3c]
00397a48: ldr r0, [sp, #0x24]
00397a4c: mov r1, r3
00397a50: bl #0x30eba4
00397a54: add r1, sp, #0x2c
00397a58: str r0, [sp, #0x40]
00397a5c: mov r0, r4
00397a60: bl #0x397594
00397a64: mov r0, r5
00397a68: ldr r3, [r5]
00397a6c: mov r1, #0
00397a70: mov lr, pc
00397a74: ldr pc, [r3, #0x48]
00397a78: mov r1, r5
00397a7c: ldr r3, [r5]
00397a80: add r0, sp, #0x54
00397a84: mov lr, pc
00397a88: ldr pc, [r3, #0xf8]
00397a8c: ldr r3, [sp, #0x54]
00397a90: mov r1, #0
00397a94: mov r0, #0xac
00397a98: cmp r3, #0
00397a9c: str r3, [sp, #0x50]
00397aa0: ldrne r2, [r3, #4]
00397aa4: addne r2, r2, #1
00397aa8: strne r2, [r3, #4]
00397aac: bl #0x5341ac
00397ab0: add r1, sp, #0x50
00397ab4: mov r2, #0
00397ab8: mov r3, #1
00397abc: mov r5, r0
00397ac0: bl #0x595ec8
00397ac4: ldr r0, [sp, #0x50]
00397ac8: cmp r0, #0
00397acc: beq #0x397ad4
00397ad0: bl #0x31d584
00397ad4: ldr r0, [sp, #0x54]
00397ad8: cmp r0, #0
00397adc: beq #0x397ae4
00397ae0: bl #0x31d584
00397ae4: ldr r3, [r4, #0x384]
00397ae8: mov r1, r5
00397aec: mov r0, r3
00397af0: ldr r3, [r3]
00397af4: mov lr, pc
00397af8: ldr pc, [r3, #0xb4]
00397afc: mov r0, r5
00397b00: bl #0x31d584
00397b04: ldr r0, [sp, #0x44]
00397b08: b #0x3978ac
00397b0c: subseq sp, pc, r8, asr r3
00397b10: subseq fp, r2, ip, lsr r2
00397b14: strdeq r3, r4, [r0], -r4
00397b18: andeq r3, r0, r0, asr #19
00397b1c: andeq r3, r0, r0, lsl #13
00397b20: andeq r1, r0, r0, asr #19
00397b24: subseq r6, r2, r0, ror sl
00397b28: subseq fp, r2, r4, lsl #2
00397b2c: subseq fp, r2, r4, lsl #1

# 0x397c2c _ZN4ZoneC1EN10ObjectBase6GO_IDSEbb
00397c2c: push {r4, r5, r6, r7, r8, lr}
00397c30: ldr r4, [pc, #0x60]
00397c34: mov r6, r0
00397c38: mov r5, r2
00397c3c: mov r7, r3
00397c40: bl #0x38c398
00397c44: ldr r3, [pc, #0x50]
00397c48: add r4, pc, r4
00397c4c: mov r2, #0
00397c50: ldr r3, [r4, r3]
00397c54: str r2, [r6, #0x37c]
00397c58: strb r5, [r6, #0x380]
00397c5c: add r1, r3, #0xf4
00397c60: add r0, r3, #8
00397c64: add r3, r3, #0xe8
00397c68: str r3, [r6, #4]
00397c6c: mov r3, #0
00397c70: str r3, [r6, #0x384]
00397c74: mov r3, #1
00397c78: str r0, [r6]
00397c7c: str r1, [r6, #0x24]
00397c80: strb r7, [r6, #0x381]
00397c84: strb r3, [r6, #0x84]
00397c88: str r2, [r6, #0x374]
00397c8c: str r2, [r6, #0x378]
00397c90: mov r0, r6
00397c94: pop {r4, r5, r6, r7, r8, pc}
00397c98: subseq ip, pc, r8, asr #28
00397c9c: andeq r0, r0, r4, lsr sp

# 0x397ca0 _ZN4ZoneC2EN10ObjectBase6GO_IDSEbb
00397ca0: push {r4, r5, r6, r7, r8, lr}
00397ca4: ldr r4, [pc, #0x60]
00397ca8: mov r6, r0
00397cac: mov r5, r2
00397cb0: mov r7, r3
00397cb4: bl #0x38c398
00397cb8: ldr r3, [pc, #0x50]
00397cbc: add r4, pc, r4
00397cc0: mov r2, #0
00397cc4: ldr r3, [r4, r3]
00397cc8: str r2, [r6, #0x37c]
00397ccc: strb r5, [r6, #0x380]
00397cd0: add r1, r3, #0xf4
00397cd4: add r0, r3, #8
00397cd8: add r3, r3, #0xe8
00397cdc: str r3, [r6, #4]
00397ce0: mov r3, #0
00397ce4: str r3, [r6, #0x384]
00397ce8: mov r3, #1
00397cec: str r0, [r6]
00397cf0: str r1, [r6, #0x24]
00397cf4: strb r7, [r6, #0x381]
00397cf8: strb r3, [r6, #0x84]
00397cfc: str r2, [r6, #0x374]
00397d00: str r2, [r6, #0x378]
00397d04: mov r0, r6
00397d08: pop {r4, r5, r6, r7, r8, pc}
00397d0c: ldrsbeq ip, [pc], #-0xd4
00397d10: andeq r0, r0, r4, lsr sp

# 0x397ec0 _ZN6ZoneExC1EN10ObjectBase6GO_IDSEbb
00397ec0: push {r4, r5, r6, lr}
00397ec4: ldr r5, [pc, #0x54]
00397ec8: mov r4, r0
00397ecc: bl #0x397ca0
00397ed0: ldr r2, [pc, #0x4c]
00397ed4: add r5, pc, r5
00397ed8: mov r3, #0
00397edc: ldr r2, [r5, r2]
00397ee0: mov r1, r4
00397ee4: str r3, [r4, #0x38c]
00397ee8: add r0, r2, #0xf4
00397eec: add ip, r2, #8
00397ef0: add r2, r2, #0xe8
00397ef4: str r0, [r4, #0x24]
00397ef8: str ip, [r4]
00397efc: str r2, [r4, #4]
00397f00: strb r3, [r1, #0x388]!
00397f04: str r1, [r4, #0x394]
00397f08: str r3, [r4, #0x3a4]
00397f0c: str r1, [r4, #0x390]
00397f10: str r3, [r4, #0x398]
00397f14: str r3, [r4, #0x3a0]
00397f18: mov r0, r4
00397f1c: pop {r4, r5, r6, pc}
00397f20: ldrheq ip, [pc], #-0xbc
00397f24: strheq r3, [r0], -r4

# 0x397f28 _ZN6ZoneExC2EN10ObjectBase6GO_IDSEbb
00397f28: push {r4, r5, r6, lr}
00397f2c: ldr r5, [pc, #0x54]
00397f30: mov r4, r0
00397f34: bl #0x397ca0
00397f38: ldr r2, [pc, #0x4c]
00397f3c: add r5, pc, r5
00397f40: mov r3, #0
00397f44: ldr r2, [r5, r2]
00397f48: mov r1, r4
00397f4c: str r3, [r4, #0x38c]
00397f50: add r0, r2, #0xf4
00397f54: add ip, r2, #8
00397f58: add r2, r2, #0xe8
00397f5c: str r0, [r4, #0x24]
00397f60: str ip, [r4]
00397f64: str r2, [r4, #4]
00397f68: strb r3, [r1, #0x388]!
00397f6c: str r1, [r4, #0x394]
00397f70: str r3, [r4, #0x3a4]
00397f74: str r1, [r4, #0x390]
00397f78: str r3, [r4, #0x398]
00397f7c: str r3, [r4, #0x3a0]
00397f80: mov r0, r4
00397f84: pop {r4, r5, r6, pc}
00397f88: subseq ip, pc, r4, asr fp
00397f8c: strheq r3, [r0], -r4

# 0x39b890 _ZN11TriggerZoneC1EN10ObjectBase6GO_IDSE
0039b890: push {r4, r5, r6, r7, lr}
0039b894: mov r3, #1
0039b898: sub sp, sp, #0x1c
0039b89c: mov r2, #0
0039b8a0: ldr r5, [pc, #0xdc]
0039b8a4: mov r4, r0
0039b8a8: bl #0x398fd4
0039b8ac: ldr r3, [pc, #0xd4]
0039b8b0: add r5, pc, r5
0039b8b4: ldr r6, [pc, #0xd0]
0039b8b8: ldr r3, [r5, r3]
0039b8bc: add r0, r4, #0x720
0039b8c0: add r6, pc, r6
0039b8c4: add r2, r3, #0xf4
0039b8c8: add r1, r3, #8
0039b8cc: add r3, r3, #0xe8
0039b8d0: str r3, [r4, #4]
0039b8d4: mvn r7, #0
0039b8d8: str r1, [r4]
0039b8dc: str r2, [r4, #0x24]
0039b8e0: mov r1, r6
0039b8e4: add r2, sp, #0x14
0039b8e8: add r0, r0, #4
0039b8ec: bl #0x3140ec
0039b8f0: mov r1, r6
0039b8f4: add r2, sp, #0x10
0039b8f8: str r7, [r4, #0x73c]
0039b8fc: add r0, r4, #0x740
0039b900: bl #0x3140ec
0039b904: add r0, r4, #0x750
0039b908: mov r1, r6
0039b90c: add r2, sp, #0xc
0039b910: str r7, [r4, #0x758]
0039b914: add r0, r0, #0xc
0039b918: bl #0x3140ec
0039b91c: add r0, r4, #0x770
0039b920: mov r1, r6
0039b924: add r2, sp, #8
0039b928: str r7, [r4, #0x774]
0039b92c: add r0, r0, #8
0039b930: bl #0x3140ec
0039b934: mov r0, r4
0039b938: str r7, [r0, #0x790]!
0039b93c: mov r1, r6
0039b940: add r2, sp, #4
0039b944: add r0, r0, #4
0039b948: bl #0x3140ec
0039b94c: mov r3, #0
0039b950: add r0, r4, #0x7b0
0039b954: str r7, [r4, #0x7ac]
0039b958: str r3, [r4, #0x7b8]
0039b95c: str r3, [r4, #0x7b0]
0039b960: strb r3, [r4, #0x7b4]
0039b964: strb r3, [r4, #0x7b5]
0039b968: mov r1, r6
0039b96c: mov r2, sp
0039b970: add r0, r0, #0xc
0039b974: bl #0x3140ec
0039b978: mov r0, r4
0039b97c: add sp, sp, #0x1c
0039b980: pop {r4, r5, r6, r7, pc}
0039b984: subseq sb, pc, r0, ror #3
0039b988: andeq r3, r0, r8, lsl ip
0039b98c: subseq pc, r2, r8, asr #30

# 0x39b990 _ZN11TriggerZoneC2EN10ObjectBase6GO_IDSE
0039b990: push {r4, r5, r6, r7, lr}
0039b994: mov r3, #1
0039b998: sub sp, sp, #0x1c
0039b99c: mov r2, #0
0039b9a0: ldr r5, [pc, #0xdc]
0039b9a4: mov r4, r0
0039b9a8: bl #0x398fd4
0039b9ac: ldr r3, [pc, #0xd4]
0039b9b0: add r5, pc, r5
0039b9b4: ldr r6, [pc, #0xd0]
0039b9b8: ldr r3, [r5, r3]
0039b9bc: add r0, r4, #0x720
0039b9c0: add r6, pc, r6
0039b9c4: add r2, r3, #0xf4
0039b9c8: add r1, r3, #8
0039b9cc: add r3, r3, #0xe8
0039b9d0: str r3, [r4, #4]
0039b9d4: mvn r7, #0
0039b9d8: str r1, [r4]
0039b9dc: str r2, [r4, #0x24]
0039b9e0: mov r1, r6
0039b9e4: add r2, sp, #0x14
0039b9e8: add r0, r0, #4
0039b9ec: bl #0x3140ec
0039b9f0: mov r1, r6
0039b9f4: add r2, sp, #0x10
0039b9f8: str r7, [r4, #0x73c]
0039b9fc: add r0, r4, #0x740
0039ba00: bl #0x3140ec
0039ba04: add r0, r4, #0x750
0039ba08: mov r1, r6
0039ba0c: add r2, sp, #0xc
0039ba10: str r7, [r4, #0x758]
0039ba14: add r0, r0, #0xc
0039ba18: bl #0x3140ec
0039ba1c: add r0, r4, #0x770
0039ba20: mov r1, r6
0039ba24: add r2, sp, #8
0039ba28: str r7, [r4, #0x774]
0039ba2c: add r0, r0, #8
0039ba30: bl #0x3140ec
0039ba34: mov r0, r4
0039ba38: str r7, [r0, #0x790]!
0039ba3c: mov r1, r6
0039ba40: add r2, sp, #4
0039ba44: add r0, r0, #4
0039ba48: bl #0x3140ec
0039ba4c: mov r3, #0
0039ba50: add r0, r4, #0x7b0
0039ba54: str r7, [r4, #0x7ac]
0039ba58: str r3, [r4, #0x7b8]
0039ba5c: str r3, [r4, #0x7b0]
0039ba60: strb r3, [r4, #0x7b4]
0039ba64: strb r3, [r4, #0x7b5]
0039ba68: mov r1, r6
0039ba6c: mov r2, sp
0039ba70: add r0, r0, #0xc
0039ba74: bl #0x3140ec
0039ba78: mov r0, r4
0039ba7c: add sp, sp, #0x1c
0039ba80: pop {r4, r5, r6, r7, pc}
0039ba84: subseq sb, pc, r0, ror #1
0039ba88: andeq r3, r0, r8, lsl ip
0039ba8c: subseq pc, r2, r8, asr #28

# 0x39c9a4 _ZN20TriggerZoneExitLevelC1EN10ObjectBase6GO_IDSE
0039c9a4: push {r4, r5, r6, lr}
0039c9a8: ldr r5, [pc, #0x88]
0039c9ac: mov r4, r0
0039c9b0: bl #0x39b990
0039c9b4: ldr r2, [pc, #0x80]
0039c9b8: add r5, pc, r5
0039c9bc: add r3, r4, #0x7d0
0039c9c0: ldr r2, [r5, r2]
0039c9c4: add r3, r3, #0xc
0039c9c8: str r3, [r4, #0x7ec]
0039c9cc: add r1, r2, #0xf4
0039c9d0: add r0, r2, #8
0039c9d4: add r2, r2, #0xe8
0039c9d8: str r2, [r4, #4]
0039c9dc: mvn r2, #0
0039c9e0: str r0, [r4]
0039c9e4: str r1, [r4, #0x24]
0039c9e8: str r2, [r4, #0x7d4]
0039c9ec: mov r0, r3
0039c9f0: str r3, [r4, #0x7f0]
0039c9f4: mov r1, #0x10
0039c9f8: bl #0x31167c
0039c9fc: ldr r2, [r4, #0x7ec]
0039ca00: add r3, r4, #0x800
0039ca04: mov r5, #0
0039ca08: strb r5, [r2]
0039ca0c: mov r0, r3
0039ca10: str r3, [r4, #0x810]
0039ca14: str r3, [r4, #0x814]
0039ca18: mov r1, #0x10
0039ca1c: bl #0x31167c
0039ca20: ldr r3, [r4, #0x810]
0039ca24: mov r0, r4
0039ca28: strb r5, [r3]
0039ca2c: strb r5, [r4, #0x819]
0039ca30: strb r5, [r4, #0x818]
0039ca34: pop {r4, r5, r6, pc}
0039ca38: ldrsbeq r8, [pc], #-8
0039ca3c: strdeq r4, r5, [r0], -r4

# 0x39ca40 _ZN20TriggerZoneExitLevelC2EN10ObjectBase6GO_IDSE
0039ca40: push {r4, r5, r6, lr}
0039ca44: ldr r5, [pc, #0x88]
0039ca48: mov r4, r0
0039ca4c: bl #0x39b990
0039ca50: ldr r2, [pc, #0x80]
0039ca54: add r5, pc, r5
0039ca58: add r3, r4, #0x7d0
0039ca5c: ldr r2, [r5, r2]
0039ca60: add r3, r3, #0xc
0039ca64: str r3, [r4, #0x7ec]
0039ca68: add r1, r2, #0xf4
0039ca6c: add r0, r2, #8
0039ca70: add r2, r2, #0xe8
0039ca74: str r2, [r4, #4]
0039ca78: mvn r2, #0
0039ca7c: str r0, [r4]
0039ca80: str r1, [r4, #0x24]
0039ca84: str r2, [r4, #0x7d4]
0039ca88: mov r0, r3
0039ca8c: str r3, [r4, #0x7f0]
0039ca90: mov r1, #0x10
0039ca94: bl #0x31167c
0039ca98: ldr r2, [r4, #0x7ec]
0039ca9c: add r3, r4, #0x800
0039caa0: mov r5, #0
0039caa4: strb r5, [r2]
0039caa8: mov r0, r3
0039caac: str r3, [r4, #0x810]
0039cab0: str r3, [r4, #0x814]
0039cab4: mov r1, #0x10
0039cab8: bl #0x31167c
0039cabc: ldr r3, [r4, #0x810]
0039cac0: mov r0, r4
0039cac4: strb r5, [r3]
0039cac8: strb r5, [r4, #0x819]
0039cacc: strb r5, [r4, #0x818]
0039cad0: pop {r4, r5, r6, pc}
0039cad4: subseq r8, pc, ip, lsr r0
0039cad8: strdeq r4, r5, [r0], -r4

# 0x3ef184 _ZN11LevelConfig6UpdateEv
003ef184: bx lr

# 0x3ef188 _ZNK11LevelConfig4DrawEv
003ef188: bx lr

# 0x3ef4a8 _ZNK5Level14GetLevelConfigEv
003ef4a8: push {r4, lr}
003ef4ac: mov r4, r0
003ef4b0: ldr r0, [r0, #0x38]
003ef4b4: ldr r3, [pc, #0x6c]
003ef4b8: sub sp, sp, #8
003ef4bc: cmp r0, #0
003ef4c0: add r3, pc, r3
003ef4c4: beq #0x3ef4d0
003ef4c8: add sp, sp, #8
003ef4cc: pop {r4, pc}
003ef4d0: ldr r2, [pc, #0x54]
003ef4d4: ldr r2, [r3, r2]
003ef4d8: ldr r2, [r2]
003ef4dc: cmp r2, #2
003ef4e0: streq r0, [r0]
003ef4e4: beq #0x3ef4c8
003ef4e8: cmp r2, #1
003ef4ec: bne #0x3ef4c8
003ef4f0: ldr r0, [pc, #0x38]
003ef4f4: ldr r1, [pc, #0x38]
003ef4f8: ldr r2, [pc, #0x38]
003ef4fc: ldr r0, [r3, r0]
003ef500: ldr r3, [pc, #0x34]
003ef504: mov ip, #0x1dc
003ef508: add r1, pc, r1
003ef50c: add r0, r0, #0xa8
003ef510: add r2, pc, r2
003ef514: add r3, pc, r3
003ef518: str ip, [sp]
003ef51c: bl #0x30e004
003ef520: ldr r0, [r4, #0x38]
003ef524: b #0x3ef4c8
003ef528: ldrsbeq r5, [sl], #-0x50
003ef52c: andeq r3, r0, r0, asr #19
003ef530: andeq r1, r0, r0, asr #19
003ef534: ldrdeq lr, pc, [ip], #-0xe0
003ef538: subeq r6, sp, r0, ror #13
003ef53c: subeq r2, sp, r4, asr #14

# 0x3f150c _ZN5Level14SetLevelConfigEP11LevelConfig
003f150c: push {r4, r5, lr}
003f1510: ldr r5, [pc, #0x178]
003f1514: cmp r1, #0
003f1518: str r1, [r0, #0x38]
003f151c: sub sp, sp, #0xc
003f1520: mov r4, r0
003f1524: add r5, pc, r5
003f1528: beq #0x3f1588
003f152c: ldr r0, [r1, #0x17c]
003f1530: bl #0x37ba84
003f1534: ldr r3, [r4, #0x38]
003f1538: str r0, [r4, #0x11c]
003f153c: cmp r3, #0
003f1540: beq #0x3f1638
003f1544: ldr r0, [r3, #0x1c4]
003f1548: ldr r2, [r3, #0x1c0]
003f154c: cmp r0, r2
003f1550: beq #0x3f1560
003f1554: bl #0x37ba84
003f1558: ldr r3, [r4, #0x38]
003f155c: str r0, [r4, #0x124]
003f1560: cmp r3, #0
003f1564: beq #0x3f15e0
003f1568: ldr r2, [r3, #0x190]
003f156c: ldr r0, [r3, #0x194]
003f1570: cmp r0, r2
003f1574: beq #0x3f1580
003f1578: bl #0x37ba84
003f157c: str r0, [r4, #0x120]
003f1580: add sp, sp, #0xc
003f1584: pop {r4, r5, pc}
003f1588: ldr r3, [pc, #0x104]
003f158c: ldr r3, [r5, r3]
003f1590: ldr r3, [r3]
003f1594: cmp r3, #2
003f1598: streq r1, [r1]
003f159c: beq #0x3f152c
003f15a0: cmp r3, #1
003f15a4: bne #0x3f152c
003f15a8: ldr r0, [pc, #0xe8]
003f15ac: ldr r1, [pc, #0xe8]
003f15b0: ldr r2, [pc, #0xe8]
003f15b4: ldr r0, [r5, r0]
003f15b8: ldr r3, [pc, #0xe4]
003f15bc: add r1, pc, r1
003f15c0: mov ip, #0x1dc
003f15c4: add r0, r0, #0xa8
003f15c8: add r2, pc, r2
003f15cc: add r3, pc, r3
003f15d0: str ip, [sp]
003f15d4: bl #0x30e004
003f15d8: ldr r1, [r4, #0x38]
003f15dc: b #0x3f152c
003f15e0: ldr r2, [pc, #0xac]
003f15e4: ldr r2, [r5, r2]
003f15e8: ldr r2, [r2]
003f15ec: cmp r2, #2
003f15f0: streq r3, [r3]
003f15f4: beq #0x3f1568
003f15f8: cmp r2, #1
003f15fc: bne #0x3f1568
003f1600: ldr r0, [pc, #0x90]
003f1604: ldr r1, [pc, #0x9c]
003f1608: ldr r2, [pc, #0x9c]
003f160c: ldr r0, [r5, r0]
003f1610: ldr r3, [pc, #0x98]
003f1614: mov ip, #0x1dc
003f1618: add r1, pc, r1
003f161c: add r3, pc, r3
003f1620: add r0, r0, #0xa8
003f1624: add r2, pc, r2
003f1628: str ip, [sp]
003f162c: bl #0x30e004
003f1630: ldr r3, [r4, #0x38]
003f1634: b #0x3f1568
003f1638: ldr r2, [pc, #0x54]
003f163c: ldr r2, [r5, r2]
003f1640: ldr r2, [r2]
003f1644: cmp r2, #2
003f1648: streq r3, [r3]
003f164c: beq #0x3f1544
003f1650: cmp r2, #1
003f1654: bne #0x3f1544
003f1658: ldr r0, [pc, #0x38]
003f165c: ldr r1, [pc, #0x50]
003f1660: ldr r2, [pc, #0x50]
003f1664: ldr r0, [r5, r0]
003f1668: ldr r3, [pc, #0x4c]
003f166c: mov ip, #0x1dc
003f1670: add r1, pc, r1
003f1674: add r3, pc, r3
003f1678: add r0, r0, #0xa8
003f167c: add r2, pc, r2
003f1680: str ip, [sp]
003f1684: bl #0x30e004
003f1688: ldr r3, [r4, #0x38]
003f168c: b #0x3f1544
003f1690: subseq r3, sl, ip, ror #10
003f1694: andeq r3, r0, r0, asr #19
003f1698: andeq r1, r0, r0, asr #19
003f169c: subeq ip, ip, ip, lsl lr
003f16a0: subeq r4, sp, r8, lsr #12
003f16a4: subeq r0, sp, ip, lsl #13
003f16a8: subeq ip, ip, r0, asr #27
003f16ac: subeq r4, sp, ip, asr #11
003f16b0: subeq r0, sp, ip, lsr r6
003f16b4: subeq ip, ip, r8, ror #26
003f16b8: subeq r4, sp, r4, ror r5
003f16bc: subeq r0, sp, r4, ror #11

# 0x3f20c0 _ZThn36_N11LevelConfigD1Ev
003f20c0: sub r0, r0, #0x24
003f20c4: b #0x3f20c8

# 0x3f20c8 _ZN11LevelConfigD1Ev
003f20c8: ldr r2, [pc, #0xb4]
003f20cc: ldr r3, [pc, #0xb4]
003f20d0: push {r4, lr}
003f20d4: add r2, pc, r2
003f20d8: ldr r3, [r2, r3]
003f20dc: mov r4, r0
003f20e0: add r0, r0, #0x300
003f20e4: add r2, r3, #0x74
003f20e8: add r1, r3, #8
003f20ec: add r3, r3, #0x68
003f20f0: stm r4, {r1, r3}
003f20f4: str r2, [r4, #0x24]
003f20f8: bl #0x3139ac
003f20fc: add r0, r4, #0x2e8
003f2100: bl #0x3139ac
003f2104: add r0, r4, #0x2d0
003f2108: bl #0x3139ac
003f210c: add r0, r4, #0x2b8
003f2110: bl #0x3139ac
003f2114: add r0, r4, #0x27c
003f2118: bl #0x3139ac
003f211c: add r0, r4, #0x264
003f2120: bl #0x3139ac
003f2124: add r0, r4, #0x24c
003f2128: bl #0x3139ac
003f212c: add r0, r4, #0x234
003f2130: bl #0x3139ac
003f2134: add r0, r4, #0x204
003f2138: bl #0x34611c
003f213c: add r0, r4, #0x1b0
003f2140: bl #0x3139ac
003f2144: add r0, r4, #0x198
003f2148: bl #0x3139ac
003f214c: add r0, r4, #0x180
003f2150: bl #0x3139ac
003f2154: add r0, r4, #0x168
003f2158: bl #0x3139ac
003f215c: add r0, r4, #0x150
003f2160: bl #0x3139ac
003f2164: add r0, r4, #0x138
003f2168: bl #0x3139ac
003f216c: add r0, r4, #0x120
003f2170: bl #0x3139ac
003f2174: mov r0, r4
003f2178: bl #0x33e998
003f217c: mov r0, r4
003f2180: pop {r4, pc}
003f2184: ldrheq r2, [sl], #-0x9c
003f2188: andeq r2, r0, r8, lsl sb

# 0x3f218c _ZThn36_N11LevelConfigD0Ev
003f218c: sub r0, r0, #0x24
003f2190: b #0x3f2194

# 0x3f2194 _ZN11LevelConfigD0Ev
003f2194: push {r4, lr}
003f2198: mov r4, r0
003f219c: bl #0x3f20c8
003f21a0: mov r0, r4
003f21a4: bl #0x310440
003f21a8: mov r0, r4
003f21ac: pop {r4, pc}

# 0x3f21b0 _ZN11LevelConfigD2Ev
003f21b0: ldr r2, [pc, #0xb4]
003f21b4: ldr r3, [pc, #0xb4]
003f21b8: push {r4, lr}
003f21bc: add r2, pc, r2
003f21c0: ldr r3, [r2, r3]
003f21c4: mov r4, r0
003f21c8: add r0, r0, #0x300
003f21cc: add r2, r3, #0x74
003f21d0: add r1, r3, #8
003f21d4: add r3, r3, #0x68
003f21d8: stm r4, {r1, r3}
003f21dc: str r2, [r4, #0x24]
003f21e0: bl #0x3139ac
003f21e4: add r0, r4, #0x2e8
003f21e8: bl #0x3139ac
003f21ec: add r0, r4, #0x2d0
003f21f0: bl #0x3139ac
003f21f4: add r0, r4, #0x2b8
003f21f8: bl #0x3139ac
003f21fc: add r0, r4, #0x27c
003f2200: bl #0x3139ac
003f2204: add r0, r4, #0x264
003f2208: bl #0x3139ac
003f220c: add r0, r4, #0x24c
003f2210: bl #0x3139ac
003f2214: add r0, r4, #0x234
003f2218: bl #0x3139ac
003f221c: add r0, r4, #0x204
003f2220: bl #0x34611c
003f2224: add r0, r4, #0x1b0
003f2228: bl #0x3139ac
003f222c: add r0, r4, #0x198
003f2230: bl #0x3139ac
003f2234: add r0, r4, #0x180
003f2238: bl #0x3139ac
003f223c: add r0, r4, #0x168
003f2240: bl #0x3139ac
003f2244: add r0, r4, #0x150
003f2248: bl #0x3139ac
003f224c: add r0, r4, #0x138
003f2250: bl #0x3139ac
003f2254: add r0, r4, #0x120
003f2258: bl #0x3139ac
003f225c: mov r0, r4
003f2260: bl #0x33e998
003f2264: mov r0, r4
003f2268: pop {r4, pc}
003f226c: ldrsbeq r2, [sl], #-0x84
003f2270: andeq r2, r0, r8, lsl sb

# 0x3f28ec _ZN11LevelConfig8InitPostEv
003f28ec: push {r4, r5, r6, r7, r8, sb, sl, lr}
003f28f0: ldr r5, [pc, #0x1c8]
003f28f4: ldr r8, [pc, #0x1c8]
003f28f8: ldrb r2, [r0, #0x29c]
003f28fc: add r5, pc, r5
003f2900: ldr r3, [r5, r8]
003f2904: sub sp, sp, #0x78
003f2908: cmp r2, #0
003f290c: ldr r3, [r3]
003f2910: mov r4, r0
003f2914: str r3, [sp, #0x74]
003f2918: bne #0x3f2a50
003f291c: ldr r2, [r0, #0x25c]
003f2920: ldr r3, [r0, #0x260]
003f2924: mov r1, #1
003f2928: strb r1, [r0, #0x29c]
003f292c: cmp r2, r3
003f2930: beq #0x3f2a6c
003f2934: ldr r2, [r4, #0x28c]
003f2938: ldr r3, [r4, #0x290]
003f293c: cmp r2, r3
003f2940: beq #0x3f2aa8
003f2944: ldr r2, [r4, #0x2c8]
003f2948: ldr r3, [r4, #0x2cc]
003f294c: cmp r2, r3
003f2950: beq #0x3f2a94
003f2954: ldr r2, [r4, #0x2e0]
003f2958: ldr r3, [r4, #0x2e4]
003f295c: cmp r2, r3
003f2960: beq #0x3f2a80
003f2964: add r0, r4, #0x1cc
003f2968: bl #0x3ef1b8
003f296c: ldr r3, [pc, #0x154]
003f2970: ldr r7, [pc, #0x154]
003f2974: add sb, sp, #0x5c
003f2978: ldr r6, [r5, r3]
003f297c: add r7, pc, r7
003f2980: add sl, sp, #0x44
003f2984: mov r0, r6
003f2988: bl #0x337888
003f298c: add r2, sp, #0x10
003f2990: mov r1, r7
003f2994: mov r0, sb
003f2998: bl #0x3140ec
003f299c: mov r1, sb
003f29a0: mov r0, r6
003f29a4: bl #0x337a88
003f29a8: mov r0, sb
003f29ac: bl #0x3139ac
003f29b0: mov r0, r6
003f29b4: bl #0x337888
003f29b8: add r2, sp, #0xc
003f29bc: mov r0, sl
003f29c0: mov r1, r7
003f29c4: bl #0x3140ec
003f29c8: mov r1, sl
003f29cc: mov r0, r6
003f29d0: bl #0x337a88
003f29d4: mov r0, sl
003f29d8: bl #0x3139ac
003f29dc: add sl, sp, #0x2c
003f29e0: mov r0, r6
003f29e4: bl #0x337888
003f29e8: add r2, sp, #8
003f29ec: mov r0, sl
003f29f0: mov r1, r7
003f29f4: bl #0x3140ec
003f29f8: mov r1, sl
003f29fc: mov r0, r6
003f2a00: bl #0x337a88
003f2a04: mov r0, sl
003f2a08: bl #0x3139ac
003f2a0c: add sl, sp, #0x14
003f2a10: mov r0, r6
003f2a14: bl #0x337888
003f2a18: add r2, sp, #4
003f2a1c: mov r1, r7
003f2a20: mov r0, sl
003f2a24: bl #0x3140ec
003f2a28: mov r1, sl
003f2a2c: mov r0, r6
003f2a30: bl #0x337a88
003f2a34: mov r0, sl
003f2a38: bl #0x3139ac
003f2a3c: ldr r3, [pc, #0x8c]
003f2a40: ldr r0, [r5, r3]
003f2a44: bl #0x31f594
003f2a48: mov r1, r4
003f2a4c: bl #0x3f150c
003f2a50: ldr r3, [r5, r8]
003f2a54: ldr r2, [sp, #0x74]
003f2a58: ldr r3, [r3]
003f2a5c: cmp r2, r3
003f2a60: bne #0x3f2abc
003f2a64: add sp, sp, #0x78
003f2a68: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003f2a6c: ldr r1, [pc, #0x60]
003f2a70: add r0, r0, #0x24c
003f2a74: add r1, pc, r1
003f2a78: bl #0x33076c
003f2a7c: b #0x3f2934
003f2a80: ldr r1, [pc, #0x50]
003f2a84: add r0, r4, #0x2d0
003f2a88: add r1, pc, r1
003f2a8c: bl #0x33076c
003f2a90: b #0x3f2964
003f2a94: ldr r1, [pc, #0x40]
003f2a98: add r0, r4, #0x2b8
003f2a9c: add r1, pc, r1
003f2aa0: bl #0x33076c
003f2aa4: b #0x3f2954
003f2aa8: ldr r1, [pc, #0x30]
003f2aac: add r0, r4, #0x27c
003f2ab0: add r1, pc, r1
003f2ab4: bl #0x33076c
003f2ab8: b #0x3f2944
003f2abc: bl #0x30e310

# 0x3f3128 _ZN5LevelC1EPKcijjjbbii
003f3128: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f312c: ldr ip, [pc, #0x360]
003f3130: sub sp, sp, #0x420
003f3134: sub sp, sp, #0xc
003f3138: str ip, [sp, #0x1c]
003f313c: ldr sb, [pc, #0x354]
003f3140: mov ip, r3
003f3144: ldr r3, [sp, #0x1c]
003f3148: add sb, pc, sb
003f314c: mov r4, r0
003f3150: ldr lr, [sb, r3]
003f3154: mov r3, r2
003f3158: mov r8, r1
003f315c: ldr r2, [lr]
003f3160: ldrb fp, [sp, #0x458]
003f3164: str r3, [sp, #0x18]
003f3168: str ip, [sp, #0x14]
003f316c: str r2, [sp, #0x424]
003f3170: ldrb sl, [sp, #0x45c]
003f3174: bl #0x33805c
003f3178: ldr r2, [pc, #0x31c]
003f317c: mov r6, #0
003f3180: mvn r5, #0
003f3184: ldr r2, [sb, r2]
003f3188: add r7, r4, #0x44
003f318c: mov r1, r6
003f3190: add r2, r2, #8
003f3194: str r2, [r4]
003f3198: str r6, [r4, #0x30]
003f319c: str r6, [r4, #0x38]
003f31a0: str r5, [r4, #0x3c]
003f31a4: str r5, [r4, #0x40]
003f31a8: mov r0, r7
003f31ac: bl #0x37c584
003f31b0: ldr r2, [sp, #0x450]
003f31b4: mov r1, r8
003f31b8: add r0, r4, #0xf8
003f31bc: str r2, [r4, #0xdc]
003f31c0: ldr r2, [sp, #0x454]
003f31c4: strb fp, [r4, #0xf1]
003f31c8: strb sl, [r4, #0xf2]
003f31cc: str r2, [r4, #0xe0]
003f31d0: mov r2, #1
003f31d4: str r2, [r4, #0xe4]
003f31d8: add r2, sp, #0x28
003f31dc: sub r2, r2, #8
003f31e0: str r6, [r4, #0xec]
003f31e4: strb r6, [r4, #0xf0]
003f31e8: strb r6, [r4, #0xf3]
003f31ec: strb r6, [r4, #0xf4]
003f31f0: strb r6, [r4, #0xf5]
003f31f4: bl #0x3140ec
003f31f8: ldr r3, [sp, #0x18]
003f31fc: ldr r2, [pc, #0x29c]
003f3200: mov r1, #0
003f3204: str r3, [r4, #0x110]
003f3208: ldr ip, [sp, #0x14]
003f320c: add r2, pc, r2
003f3210: add r0, r4, #0xac
003f3214: str ip, [r4, #0x114]
003f3218: ldr r3, [sp, #0x464]
003f321c: str r6, [r4, #0x130]
003f3220: str r1, [r4, #0x1a4]
003f3224: str r6, [r4, #0x134]
003f3228: str r1, [r4, #0x160]
003f322c: str r6, [r4, #0x138]
003f3230: str r1, [r4, #0x164]
003f3234: str r1, [r4, #0x168]
003f3238: str r1, [r4, #0x19c]
003f323c: str r1, [r4, #0x1a0]
003f3240: str r3, [r4, #0x118]
003f3244: str r5, [r4, #0x11c]
003f3248: str r5, [r4, #0x120]
003f324c: str r5, [r4, #0x124]
003f3250: str r6, [r4, #0x128]
003f3254: str r6, [r4, #0x12c]
003f3258: str r6, [r4, #0x13c]
003f325c: str r6, [r4, #0x140]
003f3260: strb r6, [r4, #0x144]
003f3264: strb r6, [r4, #0x145]
003f3268: str r5, [r4, #0x148]
003f326c: str r6, [r4, #0x14c]
003f3270: str r6, [r4, #0x150]
003f3274: str r6, [r4, #0x154]
003f3278: str r6, [r4, #0x158]
003f327c: str r6, [r4, #0x15c]
003f3280: str r6, [r4, #0x194]
003f3284: strb r6, [r4, #0x198]
003f3288: strb r6, [r4, #0x1a8]
003f328c: ldr r3, [r2]
003f3290: ldr r1, [pc, #0x20c]
003f3294: ldr fp, [pc, #0x20c]
003f3298: add r3, r3, #1
003f329c: add r1, pc, r1
003f32a0: str r3, [r2]
003f32a4: add r2, r1, #0xd
003f32a8: bl #0x3109e0
003f32ac: ldr r1, [pc, #0x1f8]
003f32b0: mov r0, r7
003f32b4: add r1, pc, r1
003f32b8: bl #0x37b574
003f32bc: ldr r1, [pc, #0x1ec]
003f32c0: mov r0, r7
003f32c4: add r1, pc, r1
003f32c8: bl #0x37b574
003f32cc: ldr r2, [pc, #0x1e0]
003f32d0: ldr r3, [sb, fp]
003f32d4: str r5, [r4, #0x18c]
003f32d8: ldr r2, [sb, r2]
003f32dc: strb r6, [r4, #0x16c]
003f32e0: str r6, [r2]
003f32e4: strb r6, [r4, #0xe8]
003f32e8: ldr r3, [r3]
003f32ec: cmp r3, r6
003f32f0: beq #0x3f3370
003f32f4: ldr r3, [pc, #0x1bc]
003f32f8: add r5, sp, #0x28
003f32fc: sub r5, r5, #4
003f3300: ldr sl, [sb, r3]
003f3304: mov r7, r6
003f3308: b #0x3f3324
003f330c: ldr r3, [sb, fp]
003f3310: add r7, r7, #1
003f3314: add r6, r6, #0x48
003f3318: ldr r3, [r3]
003f331c: cmp r3, r7
003f3320: bls #0x3f3370
003f3324: ldr r8, [sl]
003f3328: mov r0, r5
003f332c: add r8, r8, r6
003f3330: ldr r1, [r8, #0x20]
003f3334: bl #0x30e520
003f3338: mov r0, r5
003f333c: mov r1, #0
003f3340: mvn r2, #0
003f3344: bl #0x34e414
003f3348: ldr r0, [r4, #0x10c]
003f334c: mov r1, r5
003f3350: bl #0x30ebd4
003f3354: cmp r0, #0
003f3358: beq #0x3f330c
003f335c: ldrb r3, [r8, #0x14]
003f3360: str r7, [r4, #0x3c]
003f3364: strb r3, [r4, #0xe8]
003f3368: ldr r3, [r8, #0x10]
003f336c: str r3, [r4, #0x40]
003f3370: bl #0x7fd794
003f3374: ldrb r3, [r0, #5]
003f3378: cmp r3, #0
003f337c: bne #0x3f342c
003f3380: ldr r3, [r4, #0x3c]
003f3384: cmn r3, #1
003f3388: beq #0x3f3404
003f338c: ldr ip, [sp, #0x460]
003f3390: cmn ip, #1
003f3394: beq #0x3f33c8
003f3398: ldr r3, [r4, #0x40]
003f339c: mov r2, #1
003f33a0: strb r2, [r4, #0xf5]
003f33a4: orrs ip, ip, r3
003f33a8: beq #0x3f33c0
003f33ac: ldr r2, [sp, #0x460]
003f33b0: cmp r2, r3
003f33b4: beq #0x3f33c8
003f33b8: cmp r3, #0
003f33bc: beq #0x3f33c8
003f33c0: mov r3, #1
003f33c4: strb r3, [r4, #0xf3]
003f33c8: mov r1, #0
003f33cc: mov r0, #0x3c
003f33d0: bl #0x310570
003f33d4: ldr ip, [r4, #0x118]
003f33d8: ldr lr, [r4, #0x3c]
003f33dc: ldr r2, [r4, #0x114]
003f33e0: ldr r3, [r4, #0x40]
003f33e4: mov r5, r0
003f33e8: str ip, [sp, #4]
003f33ec: mov r1, r4
003f33f0: mov ip, #0
003f33f4: str lr, [sp]
003f33f8: str ip, [sp, #8]
003f33fc: bl #0x462934
003f3400: str r5, [r4, #0xec]
003f3404: ldr ip, [sp, #0x1c]
003f3408: ldr r2, [sp, #0x424]
003f340c: mov r0, r4
003f3410: ldr r3, [sb, ip]
003f3414: ldr r3, [r3]
003f3418: cmp r2, r3
003f341c: bne #0x3f3490
003f3420: add sp, sp, #0x2c
003f3424: add sp, sp, #0x400
003f3428: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f342c: ldr r5, [pc, #0x88]
003f3430: ldr r3, [sb, r5]
003f3434: ldr r0, [r3, #0x40]
003f3438: bl #0x36f074
003f343c: cmp r0, #0
003f3440: beq #0x3f345c
003f3444: ldr r3, [sb, r5]
003f3448: ldr r3, [r3, #0x40]
003f344c: ldrb r3, [r3, #0x719]
003f3450: cmp r3, #0
003f3454: beq #0x3f3380
003f3458: b #0x3f3470
003f345c: bl #0x320e98
003f3460: ldr r3, [r0, #0x34]
003f3464: sub r3, r3, #3
003f3468: cmp r3, #1
003f346c: bls #0x3f347c
003f3470: mov r3, #0
003f3474: strb r3, [r4, #0xf1]
003f3478: b #0x3f3380
003f347c: bl #0x800f8c
003f3480: bl #0x81f524
003f3484: cmp r0, #0
003f3488: beq #0x3f3470
003f348c: b #0x3f3444
003f3490: bl #0x30e310
003f3494: andeq r4, r0, ip, lsr #1
003f3498: subseq r1, sl, r8, asr #18
003f349c: andeq r0, r0, r4, ror r7
003f34a0: ldrsbeq pc, [sl], #-0xe4
003f34a4: subeq r3, sp, ip, asr #8
003f34a8: andeq r1, r0, r0, asr #17
003f34ac: subeq r3, sp, r4, asr #8
003f34b0: subeq r3, sp, ip, asr #8
003f34b4: andeq r2, r0, ip, lsr #16
003f34b8: andeq r0, r0, r4, ror r8
003f34bc: strdeq r3, r4, [r0], -r4

# 0x3f34c0 _ZN5LevelC2EPKcijjjbbii
003f34c0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f34c4: ldr ip, [pc, #0x360]
003f34c8: sub sp, sp, #0x420
003f34cc: sub sp, sp, #0xc
003f34d0: str ip, [sp, #0x1c]
003f34d4: ldr sb, [pc, #0x354]
003f34d8: mov ip, r3
003f34dc: ldr r3, [sp, #0x1c]
003f34e0: add sb, pc, sb
003f34e4: mov r4, r0
003f34e8: ldr lr, [sb, r3]
003f34ec: mov r3, r2
003f34f0: mov r8, r1
003f34f4: ldr r2, [lr]
003f34f8: ldrb fp, [sp, #0x458]
003f34fc: str r3, [sp, #0x18]
003f3500: str ip, [sp, #0x14]
003f3504: str r2, [sp, #0x424]
003f3508: ldrb sl, [sp, #0x45c]
003f350c: bl #0x33805c
003f3510: ldr r2, [pc, #0x31c]
003f3514: mov r6, #0
003f3518: mvn r5, #0
003f351c: ldr r2, [sb, r2]
003f3520: add r7, r4, #0x44
003f3524: mov r1, r6
003f3528: add r2, r2, #8
003f352c: str r2, [r4]
003f3530: str r6, [r4, #0x30]
003f3534: str r6, [r4, #0x38]
003f3538: str r5, [r4, #0x3c]
003f353c: str r5, [r4, #0x40]
003f3540: mov r0, r7
003f3544: bl #0x37c584
003f3548: ldr r2, [sp, #0x450]
003f354c: mov r1, r8
003f3550: add r0, r4, #0xf8
003f3554: str r2, [r4, #0xdc]
003f3558: ldr r2, [sp, #0x454]
003f355c: strb fp, [r4, #0xf1]
003f3560: strb sl, [r4, #0xf2]
003f3564: str r2, [r4, #0xe0]
003f3568: mov r2, #1
003f356c: str r2, [r4, #0xe4]
003f3570: add r2, sp, #0x28
003f3574: sub r2, r2, #8
003f3578: str r6, [r4, #0xec]
003f357c: strb r6, [r4, #0xf0]
003f3580: strb r6, [r4, #0xf3]
003f3584: strb r6, [r4, #0xf4]
003f3588: strb r6, [r4, #0xf5]
003f358c: bl #0x3140ec
003f3590: ldr r3, [sp, #0x18]
003f3594: ldr r2, [pc, #0x29c]
003f3598: mov r1, #0
003f359c: str r3, [r4, #0x110]
003f35a0: ldr ip, [sp, #0x14]
003f35a4: add r2, pc, r2
003f35a8: add r0, r4, #0xac
003f35ac: str ip, [r4, #0x114]
003f35b0: ldr r3, [sp, #0x464]
003f35b4: str r6, [r4, #0x130]
003f35b8: str r1, [r4, #0x1a4]
003f35bc: str r6, [r4, #0x134]
003f35c0: str r1, [r4, #0x160]
003f35c4: str r6, [r4, #0x138]
003f35c8: str r1, [r4, #0x164]
003f35cc: str r1, [r4, #0x168]
003f35d0: str r1, [r4, #0x19c]
003f35d4: str r1, [r4, #0x1a0]
003f35d8: str r3, [r4, #0x118]
003f35dc: str r5, [r4, #0x11c]
003f35e0: str r5, [r4, #0x120]
003f35e4: str r5, [r4, #0x124]
003f35e8: str r6, [r4, #0x128]
003f35ec: str r6, [r4, #0x12c]
003f35f0: str r6, [r4, #0x13c]
003f35f4: str r6, [r4, #0x140]
003f35f8: strb r6, [r4, #0x144]
003f35fc: strb r6, [r4, #0x145]
003f3600: str r5, [r4, #0x148]
003f3604: str r6, [r4, #0x14c]
003f3608: str r6, [r4, #0x150]
003f360c: str r6, [r4, #0x154]
003f3610: str r6, [r4, #0x158]
003f3614: str r6, [r4, #0x15c]
003f3618: str r6, [r4, #0x194]
003f361c: strb r6, [r4, #0x198]
003f3620: strb r6, [r4, #0x1a8]
003f3624: ldr r3, [r2]
003f3628: ldr r1, [pc, #0x20c]
003f362c: ldr fp, [pc, #0x20c]
003f3630: add r3, r3, #1
003f3634: add r1, pc, r1
003f3638: str r3, [r2]
003f363c: add r2, r1, #0xd
003f3640: bl #0x3109e0
003f3644: ldr r1, [pc, #0x1f8]
003f3648: mov r0, r7
003f364c: add r1, pc, r1
003f3650: bl #0x37b574
003f3654: ldr r1, [pc, #0x1ec]
003f3658: mov r0, r7
003f365c: add r1, pc, r1
003f3660: bl #0x37b574
003f3664: ldr r2, [pc, #0x1e0]
003f3668: ldr r3, [sb, fp]
003f366c: str r5, [r4, #0x18c]
003f3670: ldr r2, [sb, r2]
003f3674: strb r6, [r4, #0x16c]
003f3678: str r6, [r2]
003f367c: strb r6, [r4, #0xe8]
003f3680: ldr r3, [r3]
003f3684: cmp r3, r6
003f3688: beq #0x3f3708
003f368c: ldr r3, [pc, #0x1bc]
003f3690: add r5, sp, #0x28
003f3694: sub r5, r5, #4
003f3698: ldr sl, [sb, r3]
003f369c: mov r7, r6
003f36a0: b #0x3f36bc
003f36a4: ldr r3, [sb, fp]
003f36a8: add r7, r7, #1
003f36ac: add r6, r6, #0x48
003f36b0: ldr r3, [r3]
003f36b4: cmp r3, r7
003f36b8: bls #0x3f3708
003f36bc: ldr r8, [sl]
003f36c0: mov r0, r5
003f36c4: add r8, r8, r6
003f36c8: ldr r1, [r8, #0x20]
003f36cc: bl #0x30e520
003f36d0: mov r0, r5
003f36d4: mov r1, #0
003f36d8: mvn r2, #0
003f36dc: bl #0x34e414
003f36e0: ldr r0, [r4, #0x10c]
003f36e4: mov r1, r5
003f36e8: bl #0x30ebd4
003f36ec: cmp r0, #0
003f36f0: beq #0x3f36a4
003f36f4: ldrb r3, [r8, #0x14]
003f36f8: str r7, [r4, #0x3c]
003f36fc: strb r3, [r4, #0xe8]
003f3700: ldr r3, [r8, #0x10]
003f3704: str r3, [r4, #0x40]
003f3708: bl #0x7fd794
003f370c: ldrb r3, [r0, #5]
003f3710: cmp r3, #0
003f3714: bne #0x3f37c4
003f3718: ldr r3, [r4, #0x3c]
003f371c: cmn r3, #1
003f3720: beq #0x3f379c
003f3724: ldr ip, [sp, #0x460]
003f3728: cmn ip, #1
003f372c: beq #0x3f3760
003f3730: ldr r3, [r4, #0x40]
003f3734: mov r2, #1
003f3738: strb r2, [r4, #0xf5]
003f373c: orrs ip, ip, r3
003f3740: beq #0x3f3758
003f3744: ldr r2, [sp, #0x460]
003f3748: cmp r2, r3
003f374c: beq #0x3f3760
003f3750: cmp r3, #0
003f3754: beq #0x3f3760
003f3758: mov r3, #1
003f375c: strb r3, [r4, #0xf3]
003f3760: mov r1, #0
003f3764: mov r0, #0x3c
003f3768: bl #0x310570
003f376c: ldr ip, [r4, #0x118]
003f3770: ldr lr, [r4, #0x3c]
003f3774: ldr r2, [r4, #0x114]
003f3778: ldr r3, [r4, #0x40]
003f377c: mov r5, r0
003f3780: str ip, [sp, #4]
003f3784: mov r1, r4
003f3788: mov ip, #0
003f378c: str lr, [sp]
003f3790: str ip, [sp, #8]
003f3794: bl #0x462934
003f3798: str r5, [r4, #0xec]
003f379c: ldr ip, [sp, #0x1c]
003f37a0: ldr r2, [sp, #0x424]
003f37a4: mov r0, r4
003f37a8: ldr r3, [sb, ip]
003f37ac: ldr r3, [r3]
003f37b0: cmp r2, r3
003f37b4: bne #0x3f3828
003f37b8: add sp, sp, #0x2c
003f37bc: add sp, sp, #0x400
003f37c0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f37c4: ldr r5, [pc, #0x88]
003f37c8: ldr r3, [sb, r5]
003f37cc: ldr r0, [r3, #0x40]
003f37d0: bl #0x36f074
003f37d4: cmp r0, #0
003f37d8: beq #0x3f37f4
003f37dc: ldr r3, [sb, r5]
003f37e0: ldr r3, [r3, #0x40]
003f37e4: ldrb r3, [r3, #0x719]
003f37e8: cmp r3, #0
003f37ec: beq #0x3f3718
003f37f0: b #0x3f3808
003f37f4: bl #0x320e98
003f37f8: ldr r3, [r0, #0x34]
003f37fc: sub r3, r3, #3
003f3800: cmp r3, #1
003f3804: bls #0x3f3814
003f3808: mov r3, #0
003f380c: strb r3, [r4, #0xf1]
003f3810: b #0x3f3718
003f3814: bl #0x800f8c
003f3818: bl #0x81f524
003f381c: cmp r0, #0
003f3820: beq #0x3f3808
003f3824: b #0x3f37dc
003f3828: bl #0x30e310
003f382c: andeq r4, r0, ip, lsr #1
003f3830: ldrheq r1, [sl], #-0x50
003f3834: andeq r0, r0, r4, ror r7
003f3838: subseq pc, sl, ip, lsr fp
003f383c: strheq r3, [sp], #-4
003f3840: andeq r1, r0, r0, asr #17
003f3844: subeq r3, sp, ip, lsr #1
003f3848: strheq r3, [sp], #-4
003f384c: andeq r2, r0, ip, lsr #16
003f3850: andeq r0, r0, r4, ror r8
003f3854: strdeq r3, r4, [r0], -r4

# 0x3f4910 _ZN11LevelConfigC2EN10ObjectBase6GO_IDSE
003f4910: push {r4, r5, r6, lr}
003f4914: ldr r5, [pc, #0x270]
003f4918: mov r4, r0
003f491c: bl #0x33f310
003f4920: ldr r3, [pc, #0x268]
003f4924: add r5, pc, r5
003f4928: add r2, r4, #0x120
003f492c: ldr r3, [r5, r3]
003f4930: mov r0, r2
003f4934: str r2, [r4, #0x130]
003f4938: add ip, r3, #8
003f493c: add r1, r3, #0x74
003f4940: add r3, r3, #0x68
003f4944: str ip, [r4]
003f4948: str r3, [r4, #4]
003f494c: str r1, [r4, #0x24]
003f4950: str r2, [r4, #0x134]
003f4954: mov r1, #0x10
003f4958: bl #0x31167c
003f495c: ldr r2, [r4, #0x130]
003f4960: mov r5, #0
003f4964: add r3, r4, #0x138
003f4968: strb r5, [r2]
003f496c: mov r0, r3
003f4970: str r3, [r4, #0x148]
003f4974: str r3, [r4, #0x14c]
003f4978: mov r1, #0x10
003f497c: bl #0x31167c
003f4980: ldr r2, [r4, #0x148]
003f4984: add r3, r4, #0x150
003f4988: mov r0, r3
003f498c: strb r5, [r2]
003f4990: mov r1, #0x10
003f4994: str r3, [r4, #0x160]
003f4998: str r3, [r4, #0x164]
003f499c: bl #0x31167c
003f49a0: ldr r2, [r4, #0x160]
003f49a4: add r3, r4, #0x168
003f49a8: mov r0, r3
003f49ac: strb r5, [r2]
003f49b0: mov r1, #0x10
003f49b4: str r3, [r4, #0x178]
003f49b8: str r3, [r4, #0x17c]
003f49bc: bl #0x31167c
003f49c0: ldr r2, [r4, #0x178]
003f49c4: add r3, r4, #0x180
003f49c8: mov r0, r3
003f49cc: strb r5, [r2]
003f49d0: mov r1, #0x10
003f49d4: str r3, [r4, #0x190]
003f49d8: str r3, [r4, #0x194]
003f49dc: bl #0x31167c
003f49e0: ldr r2, [r4, #0x190]
003f49e4: add r3, r4, #0x198
003f49e8: mov r0, r3
003f49ec: strb r5, [r2]
003f49f0: mov r1, #0x10
003f49f4: str r3, [r4, #0x1a8]
003f49f8: str r3, [r4, #0x1ac]
003f49fc: bl #0x31167c
003f4a00: ldr r2, [r4, #0x1a8]
003f4a04: add r3, r4, #0x1b0
003f4a08: mov r0, r3
003f4a0c: strb r5, [r2]
003f4a10: mov r1, #0x10
003f4a14: str r3, [r4, #0x1c0]
003f4a18: str r3, [r4, #0x1c4]
003f4a1c: bl #0x31167c
003f4a20: ldr r1, [r4, #0x1c0]
003f4a24: mov r3, #0
003f4a28: add r2, r4, #0x234
003f4a2c: strb r5, [r1]
003f4a30: mov r0, r2
003f4a34: str r3, [r4, #0x22c]
003f4a38: str r3, [r4, #0x1cc]
003f4a3c: str r3, [r4, #0x1d0]
003f4a40: str r3, [r4, #0x1d4]
003f4a44: str r3, [r4, #0x1e0]
003f4a48: str r3, [r4, #0x1e4]
003f4a4c: str r3, [r4, #0x1e8]
003f4a50: str r3, [r4, #0x1ec]
003f4a54: str r3, [r4, #0x1f0]
003f4a58: str r3, [r4, #0x1f4]
003f4a5c: str r3, [r4, #0x1f8]
003f4a60: str r3, [r4, #0x1fc]
003f4a64: str r3, [r4, #0x200]
003f4a68: str r3, [r4, #0x218]
003f4a6c: str r3, [r4, #0x21c]
003f4a70: str r3, [r4, #0x220]
003f4a74: str r3, [r4, #0x224]
003f4a78: str r3, [r4, #0x228]
003f4a7c: str r2, [r4, #0x244]
003f4a80: str r2, [r4, #0x248]
003f4a84: str r5, [r4, #0x204]
003f4a88: str r5, [r4, #0x208]
003f4a8c: str r5, [r4, #0x20c]
003f4a90: mov r1, #0x10
003f4a94: bl #0x31167c
003f4a98: ldr r2, [r4, #0x244]
003f4a9c: add r3, r4, #0x24c
003f4aa0: mov r0, r3
003f4aa4: strb r5, [r2]
003f4aa8: mov r1, #0x10
003f4aac: str r3, [r4, #0x25c]
003f4ab0: str r3, [r4, #0x260]
003f4ab4: bl #0x31167c
003f4ab8: ldr r2, [r4, #0x25c]
003f4abc: add r3, r4, #0x264
003f4ac0: mov r0, r3
003f4ac4: strb r5, [r2]
003f4ac8: mov r1, #0x10
003f4acc: str r3, [r4, #0x274]
003f4ad0: str r3, [r4, #0x278]
003f4ad4: bl #0x31167c
003f4ad8: ldr r2, [r4, #0x274]
003f4adc: add r3, r4, #0x27c
003f4ae0: mov r0, r3
003f4ae4: strb r5, [r2]
003f4ae8: mov r1, #0x10
003f4aec: str r3, [r4, #0x28c]
003f4af0: str r3, [r4, #0x290]
003f4af4: bl #0x31167c
003f4af8: ldr r2, [r4, #0x28c]
003f4afc: add r3, r4, #0x2b8
003f4b00: mov r0, r3
003f4b04: strb r5, [r2]
003f4b08: mov r1, #0x10
003f4b0c: str r3, [r4, #0x2c8]
003f4b10: str r3, [r4, #0x2cc]
003f4b14: strb r5, [r4, #0x29c]
003f4b18: bl #0x31167c
003f4b1c: ldr r2, [r4, #0x2c8]
003f4b20: add r3, r4, #0x2d0
003f4b24: mov r0, r3
003f4b28: strb r5, [r2]
003f4b2c: mov r1, #0x10
003f4b30: str r3, [r4, #0x2e0]
003f4b34: str r3, [r4, #0x2e4]
003f4b38: bl #0x31167c
003f4b3c: ldr r2, [r4, #0x2e0]
003f4b40: add r3, r4, #0x2e8
003f4b44: mov r0, r3
003f4b48: strb r5, [r2]
003f4b4c: mov r1, #0x10
003f4b50: str r3, [r4, #0x2f8]
003f4b54: str r3, [r4, #0x2fc]
003f4b58: bl #0x31167c
003f4b5c: ldr r2, [r4, #0x2f8]
003f4b60: add r3, r4, #0x300
003f4b64: mov r0, r3
003f4b68: strb r5, [r2]
003f4b6c: mov r1, #0x10
003f4b70: str r3, [r4, #0x310]
003f4b74: str r3, [r4, #0x314]
003f4b78: bl #0x31167c
003f4b7c: ldr r3, [r4, #0x310]
003f4b80: mov r0, r4
003f4b84: strb r5, [r3]
003f4b88: pop {r4, r5, r6, pc}
003f4b8c: subseq r0, sl, ip, ror #2
003f4b90: andeq r2, r0, r8, lsl sb

# 0x3f4b94 _ZThn4_N11LevelConfig17DeclarePropertiesEv
003f4b94: sub r0, r0, #4
003f4b98: b #0x3f4b9c

# 0x3f4b9c _ZN11LevelConfig17DeclarePropertiesEv
003f4b9c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003f4ba0: ldr sb, [pc, #0x54c]
003f4ba4: ldr r3, [pc, #0x54c]
003f4ba8: ldr r1, [pc, #0x54c]
003f4bac: add sb, pc, sb
003f4bb0: ldr ip, [sb, r3]
003f4bb4: mov r4, r0
003f4bb8: add r5, r0, #4
003f4bbc: ldr r3, [ip]
003f4bc0: sub sp, sp, #0x13c
003f4bc4: add r1, pc, r1
003f4bc8: mov r0, r5
003f4bcc: add r2, r4, #0x150
003f4bd0: str ip, [sp, #4]
003f4bd4: str r3, [sp, #0x134]
003f4bd8: bl #0x33ef7c
003f4bdc: ldr r1, [pc, #0x51c]
003f4be0: mov r0, r5
003f4be4: add r2, r4, #0x24c
003f4be8: add r1, pc, r1
003f4bec: bl #0x33ef7c
003f4bf0: ldr r1, [pc, #0x50c]
003f4bf4: add r6, sp, #0x11c
003f4bf8: add r2, sp, #0x88
003f4bfc: add r1, pc, r1
003f4c00: mov r0, r6
003f4c04: bl #0x3140ec
003f4c08: ldr r1, [pc, #0x4f8]
003f4c0c: mov r3, r6
003f4c10: add r2, r4, #0x264
003f4c14: add r1, pc, r1
003f4c18: mov r0, r5
003f4c1c: bl #0x33e404
003f4c20: mov r0, r6
003f4c24: bl #0x3139ac
003f4c28: ldr r1, [pc, #0x4dc]
003f4c2c: mov r0, r5
003f4c30: add r2, r4, #0x27c
003f4c34: add r1, pc, r1
003f4c38: bl #0x33ef7c
003f4c3c: ldr r1, [pc, #0x4cc]
003f4c40: mov r0, r5
003f4c44: add r2, r4, #0x294
003f4c48: add r1, pc, r1
003f4c4c: mov r3, #0x258
003f4c50: bl #0x398878
003f4c54: ldr r1, [pc, #0x4b8]
003f4c58: movw r3, #0x2710
003f4c5c: mov r0, r5
003f4c60: add r1, pc, r1
003f4c64: add r2, r4, #0x298
003f4c68: bl #0x398878
003f4c6c: ldr r1, [pc, #0x4a4]
003f4c70: mov r0, r5
003f4c74: add r2, r4, #0x234
003f4c78: add r1, pc, r1
003f4c7c: bl #0x33ef7c
003f4c80: ldr r1, [pc, #0x494]
003f4c84: mov r0, r5
003f4c88: add r2, r4, #0x168
003f4c8c: add r1, pc, r1
003f4c90: bl #0x33ef7c
003f4c94: ldr r1, [pc, #0x484]
003f4c98: mov r0, r5
003f4c9c: add r2, r4, #0x198
003f4ca0: add r1, pc, r1
003f4ca4: bl #0x33ef7c
003f4ca8: ldr r1, [pc, #0x474]
003f4cac: mov r0, r5
003f4cb0: add r2, r4, #0x1b0
003f4cb4: add r1, pc, r1
003f4cb8: bl #0x33ef7c
003f4cbc: ldr r1, [pc, #0x464]
003f4cc0: mov r0, r5
003f4cc4: add r2, r4, #0x180
003f4cc8: add r1, pc, r1
003f4ccc: bl #0x33ef7c
003f4cd0: ldr r1, [pc, #0x454]
003f4cd4: mov r0, r5
003f4cd8: add r2, r4, #0x1c8
003f4cdc: add r1, pc, r1
003f4ce0: mov r3, #1
003f4ce4: bl #0x33e4ac
003f4ce8: ldr r1, [pc, #0x440]
003f4cec: mov r6, #0x43000000
003f4cf0: add r6, r6, #0x7f0000
003f4cf4: mov r0, r5
003f4cf8: add r1, pc, r1
003f4cfc: add r2, r4, #0x1cc
003f4d00: add r3, sp, #0x60
003f4d04: str r6, [sp, #0x60]
003f4d08: str r6, [sp, #0x64]
003f4d0c: str r6, [sp, #0x68]
003f4d10: bl #0x389894
003f4d14: ldr r1, [pc, #0x418]
003f4d18: mov r0, r5
003f4d1c: add r2, r4, #0x1d8
003f4d20: add r1, pc, r1
003f4d24: mov r3, #1
003f4d28: bl #0x398878
003f4d2c: ldr r1, [pc, #0x404]
003f4d30: mov r0, r5
003f4d34: add r2, r4, #0x1dc
003f4d38: add r1, pc, r1
003f4d3c: mov r3, #0
003f4d40: bl #0x398878
003f4d44: ldr r1, [pc, #0x3f0]
003f4d48: mov r0, r5
003f4d4c: add r2, r4, #0x1e0
003f4d50: add r1, pc, r1
003f4d54: add r3, sp, #0x54
003f4d58: str r6, [sp, #0x5c]
003f4d5c: str r6, [sp, #0x54]
003f4d60: str r6, [sp, #0x58]
003f4d64: bl #0x389894
003f4d68: ldr r1, [pc, #0x3d0]
003f4d6c: mov r6, #0
003f4d70: mov lr, #0x3f800000
003f4d74: add r1, pc, r1
003f4d78: mov r0, r5
003f4d7c: add r2, r4, #0x1f8
003f4d80: add r3, sp, #0x48
003f4d84: str lr, [sp, #0x50]
003f4d88: str r6, [sp, #0x48]
003f4d8c: str r6, [sp, #0x4c]
003f4d90: bl #0x389894
003f4d94: ldr r1, [pc, #0x3a8]
003f4d98: mov r0, r5
003f4d9c: add r2, r4, #0x1ec
003f4da0: add r1, pc, r1
003f4da4: add r3, sp, #0x3c
003f4da8: str r6, [sp, #0x3c]
003f4dac: str r6, [sp, #0x40]
003f4db0: str r6, [sp, #0x44]
003f4db4: bl #0x389894
003f4db8: ldr r1, [pc, #0x388]
003f4dbc: movw lr, #0x2400
003f4dc0: movt lr, #0xc974
003f4dc4: mov r0, r5
003f4dc8: add r1, pc, r1
003f4dcc: add r2, r4, #0x218
003f4dd0: add r3, sp, #0x30
003f4dd4: str lr, [sp, #0x38]
003f4dd8: str lr, [sp, #0x30]
003f4ddc: str lr, [sp, #0x34]
003f4de0: bl #0x389894
003f4de4: ldr r1, [pc, #0x360]
003f4de8: mov r0, r5
003f4dec: add r2, r4, #0x224
003f4df0: add r1, pc, r1
003f4df4: add r3, sp, #0x24
003f4df8: str r6, [sp, #0x24]
003f4dfc: str r6, [sp, #0x28]
003f4e00: str r6, [sp, #0x2c]
003f4e04: bl #0x389894
003f4e08: ldr r1, [pc, #0x340]
003f4e0c: mov r3, r6
003f4e10: mov r0, r5
003f4e14: add r1, pc, r1
003f4e18: add r2, r4, #0x230
003f4e1c: bl #0x39503c
003f4e20: ldr r1, [pc, #0x32c]
003f4e24: add r6, sp, #0x104
003f4e28: add r2, sp, #0x84
003f4e2c: add r1, pc, r1
003f4e30: mov r0, r6
003f4e34: bl #0x3140ec
003f4e38: ldr r1, [pc, #0x318]
003f4e3c: mov r3, r6
003f4e40: add r2, r4, #0x120
003f4e44: add r1, pc, r1
003f4e48: mov r0, r5
003f4e4c: bl #0x33e404
003f4e50: mov r0, r6
003f4e54: bl #0x3139ac
003f4e58: ldr r1, [pc, #0x2fc]
003f4e5c: add r7, sp, #0xec
003f4e60: add r2, sp, #0x80
003f4e64: add r1, pc, r1
003f4e68: mov r0, r7
003f4e6c: bl #0x3140ec
003f4e70: ldr r1, [pc, #0x2e8]
003f4e74: ldr r6, [pc, #0x2e8]
003f4e78: mov r3, r7
003f4e7c: add r2, r4, #0x138
003f4e80: add r1, pc, r1
003f4e84: mov r0, r5
003f4e88: bl #0x33e404
003f4e8c: add r6, pc, r6
003f4e90: mov r0, r7
003f4e94: add r7, sp, #0xd4
003f4e98: bl #0x3139ac
003f4e9c: mov r1, r6
003f4ea0: add r2, sp, #0x7c
003f4ea4: mov r0, r7
003f4ea8: bl #0x3140ec
003f4eac: ldr r1, [pc, #0x2b4]
003f4eb0: mov r3, r7
003f4eb4: add r2, r4, #0x2b8
003f4eb8: add r1, pc, r1
003f4ebc: mov r0, r5
003f4ec0: bl #0x33e404
003f4ec4: mov r0, r7
003f4ec8: add r7, sp, #0xbc
003f4ecc: bl #0x3139ac
003f4ed0: mov r1, r6
003f4ed4: add r2, sp, #0x78
003f4ed8: mov r0, r7
003f4edc: bl #0x3140ec
003f4ee0: ldr r1, [pc, #0x284]
003f4ee4: mov r3, r7
003f4ee8: add r2, r4, #0x2d0
003f4eec: add r1, pc, r1
003f4ef0: mov r0, r5
003f4ef4: bl #0x33e404
003f4ef8: mov r0, r7
003f4efc: add r7, sp, #0xa4
003f4f00: bl #0x3139ac
003f4f04: mov r1, r6
003f4f08: add r2, sp, #0x74
003f4f0c: mov r0, r7
003f4f10: bl #0x3140ec
003f4f14: ldr r1, [pc, #0x254]
003f4f18: mov r3, r7
003f4f1c: add r2, r4, #0x300
003f4f20: add r1, pc, r1
003f4f24: mov r0, r5
003f4f28: bl #0x33e404
003f4f2c: mov r0, r7
003f4f30: bl #0x3139ac
003f4f34: ldr r1, [pc, #0x238]
003f4f38: add r6, sp, #0x8c
003f4f3c: add r2, sp, #0x70
003f4f40: add r1, pc, r1
003f4f44: mov r0, r6
003f4f48: bl #0x3140ec
003f4f4c: ldr r1, [pc, #0x224]
003f4f50: mov r3, r6
003f4f54: add r2, r4, #0x2e8
003f4f58: add r1, pc, r1
003f4f5c: mov r0, r5
003f4f60: bl #0x33e404
003f4f64: add sl, sp, #0xc
003f4f68: mov r0, r6
003f4f6c: add fp, sp, #0x18
003f4f70: bl #0x3139ac
003f4f74: mov r6, #0
003f4f78: mov r1, fp
003f4f7c: mov r0, sl
003f4f80: str r6, [sp, #0x18]
003f4f84: str r6, [sp, #0x1c]
003f4f88: str r6, [sp, #0x20]
003f4f8c: bl #0x3424b8
003f4f90: mov r1, r6
003f4f94: mov r0, #0x2c
003f4f98: bl #0x310570
003f4f9c: ldr r3, [pc, #0x1d8]
003f4fa0: ldr r8, [pc, #0x1d8]
003f4fa4: mov r7, r0
003f4fa8: ldr r3, [sb, r3]
003f4fac: add r8, pc, r8
003f4fb0: mov r1, r8
003f4fb4: add r3, r3, #8
003f4fb8: str r3, [r0], #8
003f4fbc: add r2, sp, #0x6c
003f4fc0: bl #0x3140ec
003f4fc4: ldr r3, [pc, #0x1b8]
003f4fc8: add r2, r4, #0x204
003f4fcc: rsb r2, r5, r2
003f4fd0: ldr r3, [sb, r3]
003f4fd4: mov r0, r7
003f4fd8: str r2, [r7, #4]
003f4fdc: add r3, r3, #8
003f4fe0: str r3, [r0], #0x20
003f4fe4: mov r1, sl
003f4fe8: bl #0x3424b8
003f4fec: mov r1, r8
003f4ff0: mov r2, r7
003f4ff4: mov r0, r5
003f4ff8: bl #0x513ce4
003f4ffc: mov r0, sl
003f5000: bl #0x34611c
003f5004: mov r0, fp
003f5008: bl #0x34611c
003f500c: ldr r1, [pc, #0x174]
003f5010: mov r3, #0x43000000
003f5014: mov r0, r5
003f5018: add r2, r4, #0x210
003f501c: add r1, pc, r1
003f5020: add r3, r3, #0x480000
003f5024: bl #0x39503c
003f5028: ldr r1, [pc, #0x15c]
003f502c: mov r3, #0x42000000
003f5030: mov r0, r5
003f5034: add r2, r4, #0x214
003f5038: add r1, pc, r1
003f503c: add r3, r3, #0xc80000
003f5040: bl #0x39503c
003f5044: ldr r1, [pc, #0x144]
003f5048: mov r0, r5
003f504c: add r2, r4, #0x2a0
003f5050: add r1, pc, r1
003f5054: mov r3, #0x500000
003f5058: bl #0x398878
003f505c: ldr r1, [pc, #0x130]
003f5060: mov r0, r5
003f5064: add r2, r4, #0x2a4
003f5068: add r1, pc, r1
003f506c: mov r3, #0xa0000
003f5070: bl #0x398878
003f5074: ldr r1, [pc, #0x11c]
003f5078: mov r0, r5
003f507c: add r2, r4, #0x2a8
003f5080: add r1, pc, r1
003f5084: mov r3, r6
003f5088: bl #0x33e4ac
003f508c: ldr r1, [pc, #0x108]
003f5090: mov r0, r5
003f5094: add r2, r4, #0x2ac
003f5098: add r1, pc, r1
003f509c: mov r3, #0x500000
003f50a0: bl #0x398878
003f50a4: ldr r1, [pc, #0xf4]
003f50a8: mov r0, r5
003f50ac: add r2, r4, #0x2b0
003f50b0: add r1, pc, r1
003f50b4: mov r3, #0xa0000
003f50b8: bl #0x398878
003f50bc: ldr r1, [pc, #0xe0]
003f50c0: add r2, r4, #0x2b4
003f50c4: mov r3, r6
003f50c8: mov r0, r5
003f50cc: add r1, pc, r1
003f50d0: bl #0x33e4ac
003f50d4: ldr ip, [sp, #4]
003f50d8: ldr r2, [sp, #0x134]
003f50dc: ldr r3, [ip]
003f50e0: cmp r2, r3
003f50e4: bne #0x3f50f0
003f50e8: add sp, sp, #0x13c
003f50ec: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003f50f0: bl #0x30e310
003f50f4: subseq pc, sb, r4, ror #29
003f50f8: andeq r4, r0, ip, lsr #1
003f50fc: subeq r1, sp, ip, lsl #25
003f5100: subeq r1, sp, r8, ror ip
003f5104: subeq r1, sp, r4, ror ip
003f5108: subeq r1, sp, r4, ror #24
003f510c: subeq r1, sp, r4, asr ip
003f5110: subeq r1, sp, r0, asr ip
003f5114: subeq r1, sp, r8, asr #24
003f5118: subeq r1, sp, r0, asr #24
003f511c: subeq r1, sp, ip, asr #24
003f5120: subeq r1, sp, r0, lsr #24
003f5124: subeq r1, sp, ip, lsl ip
003f5128: subeq r1, sp, r8, lsl ip
003f512c: subeq r1, sp, r4, lsl ip
003f5130: subeq r1, sp, r0, lsl ip
003f5134: strdeq r1, r2, [sp], #-0xb8
003f5138: strdeq r1, r2, [sp], #-0xb0
003f513c: strdeq sp, lr, [ip], #-0x50
003f5140: strheq r1, [sp], #-0xbc
003f5144: subeq r1, sp, r8, lsr #23
003f5148: umaaleq r1, sp, r0, fp
003f514c: subeq r1, sp, r8, ror fp
003f5150: subeq r1, sp, r4, ror #22
003f5154: subeq fp, ip, r4, asr #12
003f5158: subeq fp, ip, r4, lsr r3
003f515c: subeq r1, sp, r4, lsr #22
003f5160: subeq ip, lr, r8, ror #4
003f5164: subeq r6, sp, ip, ror sb
003f5168: subeq r1, sp, r0, ror #21
003f516c: strheq r1, [sp], #-0xac
003f5170: umaaleq r1, sp, r8, sl
003f5174: subeq r1, sp, r8, lsl #21
003f5178: subeq r1, sp, r8, lsl #21
003f517c: andeq r2, r0, r0, lsr r3
003f5180: subeq r1, sp, r4, asr #20
003f5184: muleq r0, r8, r6
003f5188: subeq r1, sp, r4, ror #19
003f518c: subeq r1, sp, r0, ror #19
003f5190: ldrdeq r1, r2, [sp], #-0x98
003f5194: ldrdeq r1, r2, [sp], #-0x98
003f5198: ldrdeq r1, r2, [sp], #-0x98
003f519c: ldrdeq r1, r2, [sp], #-0x90
003f51a0: ldrdeq r1, r2, [sp], #-0x90
003f51a4: subeq r1, sp, ip, asr #19

# 0x3f51a8 _ZN11LevelConfigC1EN10ObjectBase6GO_IDSE
003f51a8: push {r4, r5, r6, lr}
003f51ac: ldr r5, [pc, #0x270]
003f51b0: mov r4, r0
003f51b4: bl #0x33f310
003f51b8: ldr r3, [pc, #0x268]
003f51bc: add r5, pc, r5
003f51c0: add r2, r4, #0x120
003f51c4: ldr r3, [r5, r3]
003f51c8: mov r0, r2
003f51cc: str r2, [r4, #0x130]
003f51d0: add ip, r3, #8
003f51d4: add r1, r3, #0x74
003f51d8: add r3, r3, #0x68
003f51dc: str ip, [r4]
003f51e0: str r3, [r4, #4]
003f51e4: str r1, [r4, #0x24]
003f51e8: str r2, [r4, #0x134]
003f51ec: mov r1, #0x10
003f51f0: bl #0x31167c
003f51f4: ldr r2, [r4, #0x130]
003f51f8: mov r5, #0
003f51fc: add r3, r4, #0x138
003f5200: strb r5, [r2]
003f5204: mov r0, r3
003f5208: str r3, [r4, #0x148]
003f520c: str r3, [r4, #0x14c]
003f5210: mov r1, #0x10
003f5214: bl #0x31167c
003f5218: ldr r2, [r4, #0x148]
003f521c: add r3, r4, #0x150
003f5220: mov r0, r3
003f5224: strb r5, [r2]
003f5228: mov r1, #0x10
003f522c: str r3, [r4, #0x160]
003f5230: str r3, [r4, #0x164]
003f5234: bl #0x31167c
003f5238: ldr r2, [r4, #0x160]
003f523c: add r3, r4, #0x168
003f5240: mov r0, r3
003f5244: strb r5, [r2]
003f5248: mov r1, #0x10
003f524c: str r3, [r4, #0x178]
003f5250: str r3, [r4, #0x17c]
003f5254: bl #0x31167c
003f5258: ldr r2, [r4, #0x178]
003f525c: add r3, r4, #0x180
003f5260: mov r0, r3
003f5264: strb r5, [r2]
003f5268: mov r1, #0x10
003f526c: str r3, [r4, #0x190]
003f5270: str r3, [r4, #0x194]
003f5274: bl #0x31167c
003f5278: ldr r2, [r4, #0x190]
003f527c: add r3, r4, #0x198
003f5280: mov r0, r3
003f5284: strb r5, [r2]
003f5288: mov r1, #0x10
003f528c: str r3, [r4, #0x1a8]
003f5290: str r3, [r4, #0x1ac]
003f5294: bl #0x31167c
003f5298: ldr r2, [r4, #0x1a8]
003f529c: add r3, r4, #0x1b0
003f52a0: mov r0, r3
003f52a4: strb r5, [r2]
003f52a8: mov r1, #0x10
003f52ac: str r3, [r4, #0x1c0]
003f52b0: str r3, [r4, #0x1c4]
003f52b4: bl #0x31167c
003f52b8: ldr r1, [r4, #0x1c0]
003f52bc: mov r3, #0
003f52c0: add r2, r4, #0x234
003f52c4: strb r5, [r1]
003f52c8: mov r0, r2
003f52cc: str r3, [r4, #0x22c]
003f52d0: str r3, [r4, #0x1cc]
003f52d4: str r3, [r4, #0x1d0]
003f52d8: str r3, [r4, #0x1d4]
003f52dc: str r3, [r4, #0x1e0]
003f52e0: str r3, [r4, #0x1e4]
003f52e4: str r3, [r4, #0x1e8]
003f52e8: str r3, [r4, #0x1ec]
003f52ec: str r3, [r4, #0x1f0]
003f52f0: str r3, [r4, #0x1f4]
003f52f4: str r3, [r4, #0x1f8]
003f52f8: str r3, [r4, #0x1fc]
003f52fc: str r3, [r4, #0x200]
003f5300: str r3, [r4, #0x218]
003f5304: str r3, [r4, #0x21c]
003f5308: str r3, [r4, #0x220]
003f530c: str r3, [r4, #0x224]
003f5310: str r3, [r4, #0x228]
003f5314: str r2, [r4, #0x244]
003f5318: str r2, [r4, #0x248]
003f531c: str r5, [r4, #0x204]
003f5320: str r5, [r4, #0x208]
003f5324: str r5, [r4, #0x20c]
003f5328: mov r1, #0x10
003f532c: bl #0x31167c
003f5330: ldr r2, [r4, #0x244]
003f5334: add r3, r4, #0x24c
003f5338: mov r0, r3
003f533c: strb r5, [r2]
003f5340: mov r1, #0x10
003f5344: str r3, [r4, #0x25c]
003f5348: str r3, [r4, #0x260]
003f534c: bl #0x31167c
003f5350: ldr r2, [r4, #0x25c]
003f5354: add r3, r4, #0x264
003f5358: mov r0, r3
003f535c: strb r5, [r2]
003f5360: mov r1, #0x10
003f5364: str r3, [r4, #0x274]
003f5368: str r3, [r4, #0x278]
003f536c: bl #0x31167c
003f5370: ldr r2, [r4, #0x274]
003f5374: add r3, r4, #0x27c
003f5378: mov r0, r3
003f537c: strb r5, [r2]
003f5380: mov r1, #0x10
003f5384: str r3, [r4, #0x28c]
003f5388: str r3, [r4, #0x290]
003f538c: bl #0x31167c
003f5390: ldr r2, [r4, #0x28c]
003f5394: add r3, r4, #0x2b8
003f5398: mov r0, r3
003f539c: strb r5, [r2]
003f53a0: mov r1, #0x10
003f53a4: str r3, [r4, #0x2c8]
003f53a8: str r3, [r4, #0x2cc]
003f53ac: strb r5, [r4, #0x29c]
003f53b0: bl #0x31167c
003f53b4: ldr r2, [r4, #0x2c8]
003f53b8: add r3, r4, #0x2d0
003f53bc: mov r0, r3
003f53c0: strb r5, [r2]
003f53c4: mov r1, #0x10
003f53c8: str r3, [r4, #0x2e0]
003f53cc: str r3, [r4, #0x2e4]
003f53d0: bl #0x31167c
003f53d4: ldr r2, [r4, #0x2e0]
003f53d8: add r3, r4, #0x2e8
003f53dc: mov r0, r3
003f53e0: strb r5, [r2]
003f53e4: mov r1, #0x10
003f53e8: str r3, [r4, #0x2f8]
003f53ec: str r3, [r4, #0x2fc]
003f53f0: bl #0x31167c
003f53f4: ldr r2, [r4, #0x2f8]
003f53f8: add r3, r4, #0x300
003f53fc: mov r0, r3
003f5400: strb r5, [r2]
003f5404: mov r1, #0x10
003f5408: str r3, [r4, #0x310]
003f540c: str r3, [r4, #0x314]
003f5410: bl #0x31167c
003f5414: ldr r3, [r4, #0x310]
003f5418: mov r0, r4
003f541c: strb r5, [r3]
003f5420: pop {r4, r5, r6, pc}
003f5424: ldrsbeq pc, [sb], #-0x84
003f5428: andeq r2, r0, r8, lsl sb

# 0x40ff70 _ZN11CameraLevelC1Ev
0040ff70: push {r4, r5, r6, lr}
0040ff74: ldr r6, [pc, #0xa8]
0040ff78: mov r4, r0
0040ff7c: bl #0x411cdc
0040ff80: ldr r2, [pc, #0xa0]
0040ff84: add r6, pc, r6
0040ff88: mov r5, #0
0040ff8c: ldr r2, [r6, r2]
0040ff90: add r3, r4, #0x4c
0040ff94: mov r0, r3
0040ff98: add r2, r2, #8
0040ff9c: str r2, [r4]
0040ffa0: str r3, [r4, #0x5c]
0040ffa4: str r3, [r4, #0x60]
0040ffa8: str r5, [r4, #0x44]
0040ffac: str r5, [r4, #0x48]
0040ffb0: mov r1, #0x10
0040ffb4: bl #0x31167c
0040ffb8: ldr r2, [r4, #0x5c]
0040ffbc: add r3, r4, #0x64
0040ffc0: mov r0, r3
0040ffc4: strb r5, [r2]
0040ffc8: mov r1, #0x10
0040ffcc: str r3, [r4, #0x74]
0040ffd0: str r3, [r4, #0x78]
0040ffd4: bl #0x31167c
0040ffd8: ldr r2, [r4, #0x74]
0040ffdc: mov r3, #0
0040ffe0: mov r0, r4
0040ffe4: strb r5, [r2]
0040ffe8: mvn r2, #0
0040ffec: str r2, [r4, #0x7c]
0040fff0: mov r2, #0x3f800000
0040fff4: str r2, [r4, #0x8c]
0040fff8: str r3, [r4, #0xa0]
0040fffc: strb r5, [r4, #0xa4]
00410000: strb r5, [r4, #0x84]
00410004: strb r5, [r4, #0x85]
00410008: strb r5, [r4, #0x86]
0041000c: str r3, [r4, #0x88]
00410010: str r3, [r4, #0x90]
00410014: str r3, [r4, #0x94]
00410018: str r3, [r4, #0x98]
0041001c: str r3, [r4, #0x9c]
00410020: pop {r4, r5, r6, pc}
00410024: subseq r4, r8, ip, lsl #22
00410028: andeq r4, r0, r8, lsr #4

# 0x41002c _ZN11CameraLevelC2Ev
0041002c: push {r4, r5, r6, lr}
00410030: ldr r6, [pc, #0xa8]
00410034: mov r4, r0
00410038: bl #0x411cdc
0041003c: ldr r2, [pc, #0xa0]
00410040: add r6, pc, r6
00410044: mov r5, #0
00410048: ldr r2, [r6, r2]
0041004c: add r3, r4, #0x4c
00410050: mov r0, r3
00410054: add r2, r2, #8
00410058: str r2, [r4]
0041005c: str r3, [r4, #0x5c]
00410060: str r3, [r4, #0x60]
00410064: str r5, [r4, #0x44]
00410068: str r5, [r4, #0x48]
0041006c: mov r1, #0x10
00410070: bl #0x31167c
00410074: ldr r2, [r4, #0x5c]
00410078: add r3, r4, #0x64
0041007c: mov r0, r3
00410080: strb r5, [r2]
00410084: mov r1, #0x10
00410088: str r3, [r4, #0x74]
0041008c: str r3, [r4, #0x78]
00410090: bl #0x31167c
00410094: ldr r2, [r4, #0x74]
00410098: mov r3, #0
0041009c: mov r0, r4
004100a0: strb r5, [r2]
004100a4: mvn r2, #0
004100a8: str r2, [r4, #0x7c]
004100ac: mov r2, #0x3f800000
004100b0: str r2, [r4, #0x8c]
004100b4: str r3, [r4, #0xa0]
004100b8: strb r5, [r4, #0xa4]
004100bc: strb r5, [r4, #0x84]
004100c0: strb r5, [r4, #0x85]
004100c4: strb r5, [r4, #0x86]
004100c8: str r3, [r4, #0x88]
004100cc: str r3, [r4, #0x90]
004100d0: str r3, [r4, #0x94]
004100d4: str r3, [r4, #0x98]
004100d8: str r3, [r4, #0x9c]
004100dc: pop {r4, r5, r6, pc}
004100e0: subseq r4, r8, r0, asr sl
004100e4: andeq r4, r0, r8, lsr #4

# 0x47211c _ZN12VisualObject11CalcMeshBoxEv
0047211c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00472120: ldr r2, [pc, #0x5d4]
00472124: ldr r3, [r0, #0xc]
00472128: sub sp, sp, #0x64
0047212c: add r2, pc, r2
00472130: cmp r3, #0
00472134: str r2, [sp, #8]
00472138: mov r4, r0
0047213c: beq #0x472408
00472140: mov r0, r3
00472144: ldr r3, [r3]
00472148: mov lr, pc
0047214c: ldr pc, [r3, #0x30]
00472150: ldr r1, [r0]
00472154: mov r3, r0
00472158: ldr r2, [r4, #0xc]
0047215c: str r1, [r4, #0x10]
00472160: ldr r1, [r0, #4]
00472164: mov r0, r2
00472168: str r1, [r4, #0x14]
0047216c: ldr r3, [r3, #8]
00472170: str r3, [r4, #0x18]
00472174: ldr r3, [r2]
00472178: mov lr, pc
0047217c: ldr pc, [r3, #0x30]
00472180: ldr r2, [r0, #0xc]
00472184: mov r3, r0
00472188: ldr r0, [r4, #0xc]
0047218c: str r2, [r4, #0x1c]
00472190: ldr r2, [r3, #0x10]
00472194: str r2, [r4, #0x20]
00472198: ldr r3, [r3, #0x14]
0047219c: str r3, [r4, #0x24]
004721a0: bl #0x597290
004721a4: ldr r3, [r0]
004721a8: mov lr, pc
004721ac: ldr pc, [r3, #0x90]
004721b0: mov r3, r0
004721b4: ldr r1, [r0]
004721b8: ldr r0, [r4, #0x10]
004721bc: ldr r6, [r3, #4]
004721c0: ldr r5, [r3, #8]
004721c4: bl #0x30ed6c
004721c8: mov r1, r6
004721cc: str r0, [r4, #0x10]
004721d0: ldr r0, [r4, #0x14]
004721d4: bl #0x30ed6c
004721d8: mov r1, r5
004721dc: str r0, [r4, #0x14]
004721e0: ldr r0, [r4, #0x18]
004721e4: bl #0x30ed6c
004721e8: str r0, [r4, #0x18]
004721ec: ldr r0, [r4, #0xc]
004721f0: bl #0x597290
004721f4: ldr r3, [r0]
004721f8: mov lr, pc
004721fc: ldr pc, [r3, #0x90]
00472200: mov r3, r0
00472204: ldr r1, [r0]
00472208: ldr r0, [r4, #0x1c]
0047220c: ldr r6, [r3, #4]
00472210: ldr r5, [r3, #8]
00472214: bl #0x30ed6c
00472218: mov r1, r6
0047221c: str r0, [r4, #0x1c]
00472220: ldr r0, [r4, #0x20]
00472224: bl #0x30ed6c
00472228: mov r1, r5
0047222c: str r0, [r4, #0x20]
00472230: ldr r0, [r4, #0x24]
00472234: bl #0x30ed6c
00472238: str r0, [r4, #0x24]
0047223c: ldr r3, [r4, #8]
00472240: add r5, sp, #0x10
00472244: mov r6, #0
00472248: mov r0, r3
0047224c: ldr r3, [r3]
00472250: mov lr, pc
00472254: ldr pc, [r3, #0x40]
00472258: mov r2, #0x41
0047225c: mov r1, r0
00472260: mov r0, r5
00472264: strb r6, [sp, #0x50]
00472268: bl #0x30e868
0047226c: mov r3, #0
00472270: mov r1, r5
00472274: add r0, r4, #0x10
00472278: str r3, [sp, #0x48]
0047227c: str r3, [sp, #0x40]
00472280: str r3, [sp, #0x44]
00472284: strb r6, [sp, #0x50]
00472288: bl #0x312da8
0047228c: mov r1, r5
00472290: add r0, r4, #0x1c
00472294: bl #0x312da8
00472298: ldr r7, [r4, #0x1c]
0047229c: ldr r5, [r4, #0x10]
004722a0: mov r0, r7
004722a4: mov r1, r5
004722a8: bl #0x30e70c
004722ac: mov r1, r5
004722b0: cmp r0, r6
004722b4: mov r0, r7
004722b8: movne fp, r7
004722bc: moveq fp, r5
004722c0: bl #0x30e2f8
004722c4: cmp r0, #0
004722c8: ldr r6, [r4, #0x20]
004722cc: moveq r7, r5
004722d0: ldr r5, [r4, #0x14]
004722d4: mov r0, r6
004722d8: str r7, [r4, #0x1c]
004722dc: mov r1, r5
004722e0: str fp, [r4, #0x10]
004722e4: bl #0x30e70c
004722e8: mov r1, r5
004722ec: cmp r0, #0
004722f0: mov r0, r6
004722f4: movne sb, r6
004722f8: moveq sb, r5
004722fc: bl #0x30e2f8
00472300: cmp r0, #0
00472304: ldr r8, [r4, #0x18]
00472308: moveq r6, r5
0047230c: ldr r5, [r4, #0x24]
00472310: mov r1, r8
00472314: str r6, [r4, #0x20]
00472318: mov r0, r5
0047231c: str sb, [r4, #0x14]
00472320: bl #0x30e70c
00472324: mov r1, r8
00472328: cmp r0, #0
0047232c: mov r0, r5
00472330: movne sl, r5
00472334: moveq sl, r8
00472338: bl #0x30e2f8
0047233c: cmp r0, #0
00472340: moveq r5, r8
00472344: mov r1, fp
00472348: str r5, [r4, #0x24]
0047234c: mov r0, r7
00472350: str sl, [r4, #0x18]
00472354: bl #0x30e3ac
00472358: mov r1, #0x3f000000
0047235c: bl #0x30ed6c
00472360: mov r1, sb
00472364: mov r7, r0
00472368: mov r0, r6
0047236c: bl #0x30e3ac
00472370: mov r1, #0x3f000000
00472374: bl #0x30ed6c
00472378: mov r1, sl
0047237c: mov r8, r0
00472380: mov r0, r5
00472384: bl #0x30e3ac
00472388: mov r1, #0x3f000000
0047238c: bl #0x30ed6c
00472390: ldr ip, [sp, #8]
00472394: ldr r3, [pc, #0x364]
00472398: mov r6, r0
0047239c: mov r1, r7
004723a0: ldr r5, [ip, r3]
004723a4: ldr r0, [r5]
004723a8: bl #0x30e3ac
004723ac: str r0, [r4, #0x10]
004723b0: ldr r1, [r5]
004723b4: mov r0, r7
004723b8: bl #0x30eba4
004723bc: str r0, [r4, #0x1c]
004723c0: ldr r0, [r5, #4]
004723c4: mov r1, r8
004723c8: bl #0x30e3ac
004723cc: str r0, [r4, #0x14]
004723d0: ldr r1, [r5, #4]
004723d4: mov r0, r8
004723d8: bl #0x30eba4
004723dc: str r0, [r4, #0x20]
004723e0: ldr r0, [r5, #8]
004723e4: mov r1, r6
004723e8: bl #0x30e3ac
004723ec: str r0, [r4, #0x18]
004723f0: ldr r1, [r5, #8]
004723f4: mov r0, r6
004723f8: bl #0x30eba4
004723fc: str r0, [r4, #0x24]
00472400: add sp, sp, #0x64
00472404: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00472408: ldr ip, [sp, #8]
0047240c: ldr r0, [pc, #0x2f0]
00472410: mvn r1, #0x80000000
00472414: sub r1, r1, #0x800000
00472418: ldr r5, [ip, r0]
0047241c: mvn r2, #0x800000
00472420: str r1, [r4, #0x18]
00472424: str r1, [r4, #0x10]
00472428: str r1, [r4, #0x14]
0047242c: str r2, [r4, #0x24]
00472430: str r2, [r4, #0x1c]
00472434: str r2, [r4, #0x20]
00472438: ldr r2, [r5, #0x10]
0047243c: str r3, [sp, #0x5c]
00472440: str r3, [sp, #0x54]
00472444: str r3, [sp, #0x58]
00472448: ldr r3, [r2, #0x1c]
0047244c: add r6, sp, #0x54
00472450: movw r1, #0x6164
00472454: mov r0, r3
00472458: ldr ip, [r3]
0047245c: movt r1, #0x7365
00472460: ldr r3, [r4, #8]
00472464: mov r2, r6
00472468: mov lr, pc
0047246c: ldr pc, [ip, #0x20]
00472470: ldr r0, [sp, #0x54]
00472474: ldr r3, [sp, #0x58]
00472478: rsb r3, r0, r3
0047247c: asrs r3, r3, #2
00472480: str r3, [sp, #0xc]
00472484: beq #0x472690
00472488: ldr r2, [sp, #0xc]
0047248c: cmp r2, #0
00472490: beq #0x472680
00472494: mov r5, #0
00472498: b #0x4724a0
0047249c: ldr r0, [sp, #0x54]
004724a0: ldr r3, [r0, r5, lsl #2]
004724a4: mov r0, r3
004724a8: ldr r3, [r3]
004724ac: mov lr, pc
004724b0: ldr pc, [r3, #0x30]
004724b4: ldr r3, [sp, #0x54]
004724b8: ldr sl, [r0, #8]
004724bc: ldr fp, [r0]
004724c0: ldr r3, [r3, r5, lsl #2]
004724c4: ldr sb, [r0, #4]
004724c8: mov r0, r3
004724cc: ldr r3, [r3]
004724d0: mov lr, pc
004724d4: ldr pc, [r3, #0x30]
004724d8: ldr r2, [sp, #0x54]
004724dc: mov r3, r0
004724e0: ldr r6, [r0, #0x14]
004724e4: ldr r8, [r0, #0xc]
004724e8: ldr r0, [r2, r5, lsl #2]
004724ec: ldr r7, [r3, #0x10]
004724f0: bl #0x597290
004724f4: cmp r0, #0
004724f8: beq #0x4725bc
004724fc: ldr r3, [sp, #0x54]
00472500: ldr r0, [r3, r5, lsl #2]
00472504: bl #0x597290
00472508: ldr r3, [r0]
0047250c: mov lr, pc
00472510: ldr pc, [r3, #0x90]
00472514: mov r2, r0
00472518: ldr r3, [r2, #4]
0047251c: ldr r2, [r2, #8]
00472520: ldr r1, [r0]
00472524: mov r0, fp
00472528: stm sp, {r2, r3}
0047252c: bl #0x30ed6c
00472530: ldr r3, [sp, #4]
00472534: mov fp, r0
00472538: mov r0, sb
0047253c: mov r1, r3
00472540: bl #0x30ed6c
00472544: ldr r2, [sp]
00472548: mov sb, r0
0047254c: mov r0, sl
00472550: mov r1, r2
00472554: bl #0x30ed6c
00472558: ldr r3, [sp, #0x54]
0047255c: mov sl, r0
00472560: ldr r0, [r3, r5, lsl #2]
00472564: bl #0x597290
00472568: ldr r3, [r0]
0047256c: mov lr, pc
00472570: ldr pc, [r3, #0x90]
00472574: mov r2, r0
00472578: ldr r3, [r2, #4]
0047257c: ldr r2, [r2, #8]
00472580: ldr r1, [r0]
00472584: mov r0, r8
00472588: stm sp, {r2, r3}
0047258c: bl #0x30ed6c
00472590: ldr r3, [sp, #4]
00472594: mov r8, r0
00472598: mov r0, r7
0047259c: mov r1, r3
004725a0: bl #0x30ed6c
004725a4: ldr r2, [sp]
004725a8: mov r7, r0
004725ac: mov r0, r6
004725b0: mov r1, r2
004725b4: bl #0x30ed6c
004725b8: mov r6, r0
004725bc: ldr r3, [r4, #0x10]
004725c0: mov r1, fp
004725c4: add r5, r5, #1
004725c8: mov r0, r3
004725cc: str r3, [sp, #4]
004725d0: bl #0x30e2f8
004725d4: cmp r0, #0
004725d8: ldr r3, [sp, #4]
004725dc: movne r3, fp
004725e0: ldr fp, [r4, #0x14]
004725e4: str r3, [r4, #0x10]
004725e8: mov r1, sb
004725ec: mov r0, fp
004725f0: bl #0x30e2f8
004725f4: cmp r0, #0
004725f8: movne fp, sb
004725fc: ldr sb, [r4, #0x18]
00472600: mov r1, sl
00472604: str fp, [r4, #0x14]
00472608: mov r0, sb
0047260c: bl #0x30e2f8
00472610: cmp r0, #0
00472614: movne sb, sl
00472618: ldr sl, [r4, #0x1c]
0047261c: mov r1, r8
00472620: str sb, [r4, #0x18]
00472624: mov r0, sl
00472628: bl #0x30e70c
0047262c: cmp r0, #0
00472630: movne sl, r8
00472634: ldr r8, [r4, #0x20]
00472638: mov r1, r7
0047263c: str sl, [r4, #0x1c]
00472640: mov r0, r8
00472644: bl #0x30e70c
00472648: cmp r0, #0
0047264c: movne r8, r7
00472650: ldr r7, [r4, #0x24]
00472654: str r8, [r4, #0x20]
00472658: mov r1, r6
0047265c: mov r0, r7
00472660: bl #0x30e70c
00472664: ldr r3, [sp, #0xc]
00472668: cmp r0, #0
0047266c: movne r7, r6
00472670: cmp r5, r3
00472674: str r7, [r4, #0x24]
00472678: bne #0x47249c
0047267c: ldr r0, [sp, #0x54]
00472680: cmp r0, #0
00472684: beq #0x47223c
00472688: bl #0x310450
0047268c: b #0x47223c
00472690: ldr r3, [r5, #0x10]
00472694: movw r1, #0x6164
00472698: movt r1, #0x6d65
0047269c: ldr ip, [r3, #0x1c]
004726a0: mov r2, r6
004726a4: ldr r3, [r4, #8]
004726a8: mov r0, ip
004726ac: ldr ip, [ip]
004726b0: mov lr, pc
004726b4: ldr pc, [ip, #0x20]
004726b8: ldr r0, [sp, #0x54]
004726bc: ldr r3, [sp, #0x58]
004726c0: rsb r3, r0, r3
004726c4: asrs r3, r3, #2
004726c8: str r3, [sp, #0xc]
004726cc: bne #0x472488
004726d0: mov r3, #0
004726d4: cmp r0, #0
004726d8: str r3, [r4, #0x24]
004726dc: str r3, [r4, #0x10]
004726e0: str r3, [r4, #0x14]
004726e4: str r3, [r4, #0x18]
004726e8: str r3, [r4, #0x1c]
004726ec: str r3, [r4, #0x20]
004726f0: beq #0x472400
004726f4: bl #0x310450
004726f8: b #0x472400
004726fc: subseq r2, r2, r4, ror #18
00472700: andeq r3, r0, ip, lsr #30
00472704: strdeq r3, r4, [r0], -r4

# 0x47295c _ZN12VisualObject9SetParentEP10GameObject
0047295c: push {r4, r5, r6, lr}
00472960: ldr r4, [pc, #0x9c]
00472964: cmp r1, #0
00472968: str r1, [r0, #4]
0047296c: mov r5, r0
00472970: add r4, pc, r4
00472974: beq #0x4729c8
00472978: ldrb r3, [r1, #0x84]
0047297c: cmp r3, #0
00472980: bne #0x4729a4
00472984: mov r0, r5
00472988: bl #0x38ba74
0047298c: ldr r3, [pc, #0x74]
00472990: ldr r0, [r5, #8]
00472994: mov r1, #1
00472998: ldr r2, [r4, r3]
0047299c: pop {r4, r5, r6, lr}
004729a0: b #0x50e484
004729a4: mov r0, r1
004729a8: ldr r3, [r1]
004729ac: mov lr, pc
004729b0: ldr pc, [r3, #0x80]
004729b4: cmp r0, #0
004729b8: beq #0x4729cc
004729bc: ldr r3, [r5, #4]
004729c0: cmp r3, #0
004729c4: bne #0x472984
004729c8: pop {r4, r5, r6, pc}
004729cc: mov r0, r5
004729d0: bl #0x38ba74
004729d4: ldr r6, [r5, #8]
004729d8: ldr r3, [r6]
004729dc: mov r0, r6
004729e0: ldr r4, [r3, #0xa4]
004729e4: mov lr, pc
004729e8: ldr pc, [r3, #0xa0]
004729ec: mov r1, r0
004729f0: mov r0, r6
004729f4: blx r4
004729f8: ldr r0, [r5, #8]
004729fc: pop {r4, r5, r6, lr}
00472a00: b #0x50f220
00472a04: subseq r2, r2, r0, lsr #2
00472a08: andeq r3, r0, r4, ror r0

# 0x50e46c _Z19setOnAnimateEnabledPN6glitch5scene10ISceneNodeEb
0050e46c: ldr r3, [r0, #0x11c]
0050e470: cmp r1, #0
0050e474: orrne r3, r3, #0x200
0050e478: biceq r3, r3, #0x200
0050e47c: str r3, [r0, #0x11c]
0050e480: bx lr

# 0x50eeb4 _ZN6glitch4core10quaternionaSERKNS0_8CMatrix4IfEE
0050eeb4: push {r4, r5, r6, r7, r8, sb, sl, lr}
0050eeb8: ldr r7, [r1]
0050eebc: ldr r6, [r1, #0x14]
0050eec0: ldr r8, [r1, #0x28]
0050eec4: mov r4, r1
0050eec8: mov r5, r0
0050eecc: mov r1, r6
0050eed0: mov r0, r7
0050eed4: bl #0x30eba4
0050eed8: mov r1, r8
0050eedc: bl #0x30eba4
0050eee0: mov r1, #0
0050eee4: mov sl, r0
0050eee8: bl #0x30e2f8
0050eeec: cmp r0, #0
0050eef0: bne #0x50f0dc
0050eef4: mov r0, r7
0050eef8: mov r1, r6
0050eefc: bl #0x30e2f8
0050ef00: cmp r0, #0
0050ef04: beq #0x50efa8
0050ef08: mov r0, r7
0050ef0c: mov r1, r8
0050ef10: bl #0x30e2f8
0050ef14: cmp r0, #0
0050ef18: beq #0x50efa8
0050ef1c: mov r1, #0x3f800000
0050ef20: mov r0, r7
0050ef24: bl #0x30eba4
0050ef28: mov r1, r6
0050ef2c: bl #0x30e3ac
0050ef30: mov r1, r8
0050ef34: bl #0x30e3ac
0050ef38: bl #0x30e124
0050ef3c: mov r1, #0x3f000000
0050ef40: mov r6, r0
0050ef44: bl #0x30ed6c
0050ef48: mov r1, r6
0050ef4c: str r0, [r5]
0050ef50: mov r0, #0x3f000000
0050ef54: bl #0x30ec94
0050ef58: ldr r1, [r4, #0x10]
0050ef5c: mov r6, r0
0050ef60: ldr r0, [r4, #4]
0050ef64: bl #0x30eba4
0050ef68: mov r1, r6
0050ef6c: bl #0x30ed6c
0050ef70: str r0, [r5, #4]
0050ef74: ldr r1, [r4, #8]
0050ef78: ldr r0, [r4, #0x20]
0050ef7c: bl #0x30eba4
0050ef80: mov r1, r6
0050ef84: bl #0x30ed6c
0050ef88: str r0, [r5, #8]
0050ef8c: ldr r1, [r4, #0x18]
0050ef90: ldr r0, [r4, #0x24]
0050ef94: bl #0x30e3ac
0050ef98: mov r1, r6
0050ef9c: bl #0x30ed6c
0050efa0: str r0, [r5, #0xc]
0050efa4: b #0x50f044
0050efa8: mov r0, r6
0050efac: mov r1, r8
0050efb0: bl #0x30e2f8
0050efb4: cmp r0, #0
0050efb8: bne #0x50f050
0050efbc: mov r1, #0x3f800000
0050efc0: mov r0, r8
0050efc4: bl #0x30eba4
0050efc8: mov r1, r7
0050efcc: bl #0x30e3ac
0050efd0: mov r1, r6
0050efd4: bl #0x30e3ac
0050efd8: bl #0x30e124
0050efdc: mov r1, #0x3f000000
0050efe0: mov r6, r0
0050efe4: bl #0x30ed6c
0050efe8: mov r1, r6
0050efec: str r0, [r5, #8]
0050eff0: mov r0, #0x3f000000
0050eff4: bl #0x30ec94
0050eff8: ldr r1, [r4, #0x20]
0050effc: mov r6, r0
0050f000: ldr r0, [r4, #8]
0050f004: bl #0x30eba4
0050f008: mov r1, r6
0050f00c: bl #0x30ed6c
0050f010: str r0, [r5]
0050f014: ldr r1, [r4, #0x24]
0050f018: ldr r0, [r4, #0x18]
0050f01c: bl #0x30eba4
0050f020: mov r1, r6
0050f024: bl #0x30ed6c
0050f028: str r0, [r5, #4]
0050f02c: ldr r1, [r4, #4]
0050f030: ldr r0, [r4, #0x10]
0050f034: bl #0x30e3ac
0050f038: mov r1, r6
0050f03c: bl #0x30ed6c
0050f040: str r0, [r5, #0xc]
0050f044: mov r0, r5
0050f048: pop {r4, r5, r6, r7, r8, sb, sl, lr}
0050f04c: b #0x35c8f0
0050f050: mov r0, r6
0050f054: mov r1, #0x3f800000
0050f058: bl #0x30eba4
0050f05c: mov r1, r7
0050f060: bl #0x30e3ac
0050f064: mov r1, r8
0050f068: bl #0x30e3ac
0050f06c: bl #0x30e124
0050f070: mov r1, #0x3f000000
0050f074: mov r6, r0
0050f078: bl #0x30ed6c
0050f07c: mov r1, r6
0050f080: str r0, [r5, #4]
0050f084: mov r0, #0x3f000000
0050f088: bl #0x30ec94
0050f08c: ldr r1, [r4, #0x10]
0050f090: mov r6, r0
0050f094: ldr r0, [r4, #4]
0050f098: bl #0x30eba4
0050f09c: mov r1, r6
0050f0a0: bl #0x30ed6c
0050f0a4: str r0, [r5]
0050f0a8: ldr r1, [r4, #0x24]
0050f0ac: ldr r0, [r4, #0x18]
0050f0b0: bl #0x30eba4
0050f0b4: mov r1, r6
0050f0b8: bl #0x30ed6c
0050f0bc: str r0, [r5, #8]
0050f0c0: ldr r1, [r4, #0x20]
0050f0c4: ldr r0, [r4, #8]
0050f0c8: bl #0x30e3ac
0050f0cc: mov r1, r6
0050f0d0: bl #0x30ed6c
0050f0d4: str r0, [r5, #0xc]
0050f0d8: b #0x50f044
0050f0dc: mov r1, #0x3f800000
0050f0e0: mov r0, sl
0050f0e4: bl #0x30eba4
0050f0e8: bl #0x30e124
0050f0ec: mov r1, #0x3f000000
0050f0f0: mov r6, r0
0050f0f4: bl #0x30ed6c
0050f0f8: mov r1, r6
0050f0fc: str r0, [r5, #0xc]
0050f100: mov r0, #0x3f000000
0050f104: bl #0x30ec94
0050f108: ldr r1, [r4, #0x18]
0050f10c: mov r6, r0
0050f110: ldr r0, [r4, #0x24]
0050f114: bl #0x30e3ac
0050f118: mov r1, r6
0050f11c: bl #0x30ed6c
0050f120: str r0, [r5]
0050f124: ldr r1, [r4, #0x20]
0050f128: ldr r0, [r4, #8]
0050f12c: bl #0x30e3ac
0050f130: mov r1, r6
0050f134: bl #0x30ed6c
0050f138: str r0, [r5, #4]
0050f13c: ldr r1, [r4, #4]
0050f140: ldr r0, [r4, #0x10]
0050f144: bl #0x30e3ac
0050f148: mov r1, r6
0050f14c: bl #0x30ed6c
0050f150: str r0, [r5, #8]
0050f154: b #0x50f044

# 0x50f220 _Z14OptimizeStaticPN6glitch5scene10ISceneNodeE
0050f220: push {r4, r5, r6, lr}
0050f224: mov r1, #0
0050f228: mov r4, r0
0050f22c: sub sp, sp, #0x20
0050f230: ldr r3, [r0]
0050f234: mov lr, pc
0050f238: ldr pc, [r3, #0xb8]
0050f23c: ldr r3, [r4]
0050f240: add r5, sp, #0x14
0050f244: mov r0, r5
0050f248: mov r1, r4
0050f24c: ldr r6, [r3, #0xa4]
0050f250: bl #0x597180
0050f254: mov r1, r5
0050f258: mov r0, r4
0050f25c: blx r6
0050f260: ldr r3, [r4]
0050f264: mov r0, r4
0050f268: add r5, sp, #4
0050f26c: ldr r6, [r3, #0x9c]
0050f270: mov lr, pc
0050f274: ldr pc, [r3, #0x38]
0050f278: mov r1, r0
0050f27c: mov r0, r5
0050f280: bl #0x50eeb4
0050f284: mov r0, r4
0050f288: mov r1, r5
0050f28c: blx r6
0050f290: ldr r3, [r4, #0x11c]
0050f294: mov r5, r4
0050f298: bic r3, r3, #0x200
0050f29c: str r3, [r4, #0x11c]
0050f2a0: ldr r4, [r5, #0xf4]!
0050f2a4: cmp r4, r5
0050f2a8: beq #0x50f2c8
0050f2ac: cmp r4, #0
0050f2b0: moveq r0, r4
0050f2b4: subne r0, r4, #4
0050f2b8: bl #0x50f220
0050f2bc: ldr r4, [r4]
0050f2c0: cmp r5, r4
0050f2c4: bne #0x50f2ac
0050f2c8: add sp, sp, #0x20
0050f2cc: pop {r4, r5, r6, pc}

# 0x50f89c _Z17CopyMeshSceneNodePN6glitch5scene14IMeshSceneNodeE
0050f89c: push {r4, r5, r6, r7, r8, lr}
0050f8a0: sub sp, sp, #0x18
0050f8a4: mov r4, r0
0050f8a8: add r6, sp, #0x10
0050f8ac: mov r0, r6
0050f8b0: mov r1, r4
0050f8b4: ldr r3, [r4]
0050f8b8: ldr r5, [pc, #0x110]
0050f8bc: mov lr, pc
0050f8c0: ldr pc, [r3, #0xf8]
0050f8c4: ldr r3, [pc, #0x108]
0050f8c8: add r5, pc, r5
0050f8cc: add r0, sp, #0x14
0050f8d0: ldr r3, [r5, r3]
0050f8d4: mov ip, #0
0050f8d8: mov r1, r6
0050f8dc: ldr r2, [r3, #0x10]
0050f8e0: mvn r3, #0
0050f8e4: ldr r2, [r2, #0x10]
0050f8e8: str ip, [sp]
0050f8ec: bl #0x59b2a4
0050f8f0: ldr r0, [sp, #0x10]
0050f8f4: cmp r0, #0
0050f8f8: beq #0x50f900
0050f8fc: bl #0x31d584
0050f900: ldr r3, [sp, #0x14]
0050f904: mov r0, r4
0050f908: cmp r3, #0
0050f90c: str r3, [sp, #0xc]
0050f910: ldrne r2, [r3, #4]
0050f914: addne r2, r2, #1
0050f918: strne r2, [r3, #4]
0050f91c: ldr r3, [r4]
0050f920: mov lr, pc
0050f924: ldr pc, [r3, #0xa0]
0050f928: ldr r3, [r4]
0050f92c: mov r8, r0
0050f930: mov r0, r4
0050f934: mov lr, pc
0050f938: ldr pc, [r3, #0x98]
0050f93c: ldr r3, [r4]
0050f940: mov r7, r0
0050f944: mov r0, r4
0050f948: mov lr, pc
0050f94c: ldr pc, [r3, #0x90]
0050f950: mov r1, #0
0050f954: mov r6, r0
0050f958: mov r0, #0x140
0050f95c: bl #0x5341ac
0050f960: mov r3, r8
0050f964: add r1, sp, #0xc
0050f968: mvn r2, #0
0050f96c: mov r5, r0
0050f970: str r7, [sp]
0050f974: str r6, [sp, #4]
0050f978: bl #0x585118
0050f97c: ldr r0, [sp, #0xc]
0050f980: cmp r0, #0
0050f984: beq #0x50f98c
0050f988: bl #0x31d584
0050f98c: ldr r3, [r5]
0050f990: mov r0, r4
0050f994: ldr r4, [r3, #0x28]
0050f998: bl #0x597290
0050f99c: ldr r3, [r0]
0050f9a0: mov lr, pc
0050f9a4: ldr pc, [r3, #0x24]
0050f9a8: mov r1, r0
0050f9ac: mov r0, r5
0050f9b0: blx r4
0050f9b4: ldr r0, [sp, #0x14]
0050f9b8: cmp r0, #0
0050f9bc: beq #0x50f9c4
0050f9c0: bl #0x31d584
0050f9c4: mov r0, r5
0050f9c8: add sp, sp, #0x18
0050f9cc: pop {r4, r5, r6, r7, r8, pc}
0050f9d0: subeq r5, r8, r8, asr #3
0050f9d4: strdeq r3, r4, [r0], -r4

# 0x51cfb8 _ZNKSt4priv8_Rb_treeISsSt4lessISsESt4pairIKSsSsENS_10_Select1stIS5_EENS_11_MapTraitsTIS5_EESaIS5_EE7_M_findIPKcEEPNS_18_Rb_tree_node_baseERKT_
0051cfb8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0051cfbc: ldr r2, [pc, #0x1e8]
0051cfc0: ldr r3, [pc, #0x1e8]
0051cfc4: sub sp, sp, #0x54
0051cfc8: add r2, pc, r2
0051cfcc: str r3, [sp, #0xc]
0051cfd0: ldr r3, [r2, r3]
0051cfd4: str r2, [sp, #4]
0051cfd8: str r0, [sp, #8]
0051cfdc: ldr r5, [r0, #4]
0051cfe0: ldr r3, [r3]
0051cfe4: mov sb, r1
0051cfe8: cmp r5, #0
0051cfec: str r3, [sp, #0x4c]
0051cff0: beq #0x51d18c
0051cff4: mov sl, r0
0051cff8: add r0, sp, #0x18
0051cffc: add r8, sp, #0x34
0051d000: str r0, [sp]
0051d004: b #0x51d028
0051d008: mov r0, r4
0051d00c: bl #0x708f00
0051d010: cmp fp, #0
0051d014: movge sl, r5
0051d018: ldrlt r5, [r5, #0xc]
0051d01c: ldrge r5, [r5, #8]
0051d020: cmp r5, #0
0051d024: beq #0x51d0c4
0051d028: ldr r1, [sb]
0051d02c: ldr r2, [sp]
0051d030: mov r0, r8
0051d034: bl #0x3140ec
0051d038: ldr r3, [r5, #0x24]
0051d03c: ldr r4, [sp, #0x48]
0051d040: ldr r7, [r5, #0x20]
0051d044: ldr r6, [sp, #0x44]
0051d048: mov r0, r3
0051d04c: rsb r7, r3, r7
0051d050: rsb r6, r4, r6
0051d054: cmp r6, r7
0051d058: movlt r2, r6
0051d05c: movge r2, r7
0051d060: mov r1, r4
0051d064: bl #0x30e5e0
0051d068: subs fp, r0, #0
0051d06c: bne #0x51d084
0051d070: cmp r7, r6
0051d074: mvnlt fp, #0
0051d078: blt #0x51d084
0051d07c: movle fp, #0
0051d080: movgt fp, #1
0051d084: cmp r4, r8
0051d088: beq #0x51d010
0051d08c: cmp r4, #0
0051d090: beq #0x51d010
0051d094: ldr r1, [sp, #0x34]
0051d098: rsb r1, r4, r1
0051d09c: cmp r1, #0x80
0051d0a0: bls #0x51d008
0051d0a4: mov r0, r4
0051d0a8: bl #0x310440
0051d0ac: cmp fp, #0
0051d0b0: movge sl, r5
0051d0b4: ldrlt r5, [r5, #0xc]
0051d0b8: ldrge r5, [r5, #8]
0051d0bc: cmp r5, #0
0051d0c0: bne #0x51d028
0051d0c4: ldr r1, [sp, #8]
0051d0c8: cmp sl, r1
0051d0cc: beq #0x51d14c
0051d0d0: add r5, sp, #0x1c
0051d0d4: ldr r1, [sb]
0051d0d8: add r2, sp, #0x14
0051d0dc: mov r0, r5
0051d0e0: bl #0x3140ec
0051d0e4: ldr r3, [sl, #0x24]
0051d0e8: ldr r4, [sp, #0x30]
0051d0ec: ldr r7, [sl, #0x20]
0051d0f0: ldr r6, [sp, #0x2c]
0051d0f4: mov r1, r3
0051d0f8: rsb r7, r3, r7
0051d0fc: rsb r6, r4, r6
0051d100: cmp r7, r6
0051d104: movlt r2, r7
0051d108: movge r2, r6
0051d10c: mov r0, r4
0051d110: bl #0x30e5e0
0051d114: subs r8, r0, #0
0051d118: beq #0x51d174
0051d11c: cmp r4, r5
0051d120: beq #0x51d144
0051d124: cmp r4, #0
0051d128: beq #0x51d144
0051d12c: ldr r1, [sp, #0x1c]
0051d130: rsb r1, r4, r1
0051d134: cmp r1, #0x80
0051d138: bhi #0x51d194
0051d13c: mov r0, r4
0051d140: bl #0x708f00
0051d144: cmp r8, #0
0051d148: blt #0x51d18c
0051d14c: ldr r0, [sp, #4]
0051d150: ldr r2, [sp, #0xc]
0051d154: ldr r3, [r0, r2]
0051d158: ldr r2, [sp, #0x4c]
0051d15c: mov r0, sl
0051d160: ldr r3, [r3]
0051d164: cmp r2, r3
0051d168: bne #0x51d1a8
0051d16c: add sp, sp, #0x54
0051d170: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0051d174: cmp r6, r7
0051d178: mvnlt r8, #0
0051d17c: blt #0x51d11c
0051d180: movle r8, #0
0051d184: movgt r8, #1
0051d188: b #0x51d11c
0051d18c: ldr sl, [sp, #8]
0051d190: b #0x51d14c
0051d194: mov r0, r4
0051d198: bl #0x310440
0051d19c: cmp r8, #0
0051d1a0: bge #0x51d14c
0051d1a4: b #0x51d18c
0051d1a8: bl #0x30e310
0051d1ac: subeq r7, r7, r8, asr #21
0051d1b0: andeq r4, r0, ip, lsr #1

# 0x51d1b4 _ZN7PFFloorC1EPKcP6PFRoomP13PFGOuterGraphP13PFGInnerGraphj
0051d1b4: push {r4, r5, r6, r7, r8, lr}
0051d1b8: ldr r5, [pc, #0x218]
0051d1bc: ldr ip, [pc, #0x218]
0051d1c0: sub sp, sp, #0x10
0051d1c4: add r5, pc, r5
0051d1c8: ldr ip, [r5, ip]
0051d1cc: mov r4, r0
0051d1d0: mov r6, r2
0051d1d4: add ip, ip, #8
0051d1d8: add r2, sp, #0xc
0051d1dc: str ip, [r0], #4
0051d1e0: mov r8, r3
0051d1e4: bl #0x3140ec
0051d1e8: str r6, [r4, #0x1c]
0051d1ec: ldr r3, [sp, #0x2c]
0051d1f0: mov r7, #0
0051d1f4: add r0, r4, #0x28
0051d1f8: str r3, [r4, #0x20]
0051d1fc: str r0, [r4, #0x38]
0051d200: str r0, [r4, #0x3c]
0051d204: mov r1, #0x10
0051d208: str r7, [r4, #0x24]
0051d20c: bl #0x31167c
0051d210: ldr r0, [r4, #0x38]
0051d214: mov r3, #0
0051d218: mov r1, r4
0051d21c: strb r7, [r0]
0051d220: str r3, [r4, #0x64]
0051d224: ldr r0, [sp, #0x28]
0051d228: mov r2, r4
0051d22c: str r3, [r4, #0x44]
0051d230: str r3, [r4, #0x48]
0051d234: str r3, [r4, #0x4c]
0051d238: str r3, [r4, #0x50]
0051d23c: str r3, [r4, #0x54]
0051d240: str r3, [r4, #0x58]
0051d244: str r3, [r4, #0x5c]
0051d248: str r3, [r4, #0x60]
0051d24c: str r0, [r4, #0x74]
0051d250: str r7, [r4, #0x40]
0051d254: str r7, [r4, #0x68]
0051d258: str r7, [r4, #0x6c]
0051d25c: str r8, [r4, #0x70]
0051d260: str r7, [r4, #0x7c]
0051d264: strb r7, [r1, #0x78]!
0051d268: str r1, [r4, #0x84]
0051d26c: str r1, [r4, #0x80]
0051d270: str r7, [r4, #0x88]
0051d274: str r7, [r4, #0x94]
0051d278: strb r7, [r2, #0x90]!
0051d27c: str r2, [r4, #0x9c]
0051d280: str r2, [r4, #0x98]
0051d284: str r7, [r4, #0xa0]
0051d288: str r7, [r4, #0xa8]
0051d28c: str r7, [r4, #0xac]
0051d290: str r7, [r4, #0xb0]
0051d294: str r7, [r4, #0xb4]
0051d298: str r7, [r4, #0xb8]
0051d29c: str r7, [r4, #0xbc]
0051d2a0: str r7, [r4, #0xc0]
0051d2a4: ldr r3, [r4, #0x1c]
0051d2a8: str r7, [r4, #0xc4]
0051d2ac: str r7, [r4, #0xc8]
0051d2b0: cmp r3, r7
0051d2b4: beq #0x51d32c
0051d2b8: cmp r8, #0
0051d2bc: beq #0x51d384
0051d2c0: ldr r3, [r4, #0x74]
0051d2c4: cmp r3, #0
0051d2c8: beq #0x51d2d8
0051d2cc: mov r0, r4
0051d2d0: add sp, sp, #0x10
0051d2d4: pop {r4, r5, r6, r7, r8, pc}
0051d2d8: ldr r2, [pc, #0x100]
0051d2dc: ldr r2, [r5, r2]
0051d2e0: ldr r2, [r2]
0051d2e4: cmp r2, #2
0051d2e8: streq r3, [r3]
0051d2ec: beq #0x51d2cc
0051d2f0: cmp r2, #1
0051d2f4: bne #0x51d2cc
0051d2f8: ldr r0, [pc, #0xe4]
0051d2fc: ldr r1, [pc, #0xe4]
0051d300: ldr r2, [pc, #0xe4]
0051d304: ldr r0, [r5, r0]
0051d308: ldr r3, [pc, #0xe0]
0051d30c: mov ip, #0x24
0051d310: add r1, pc, r1
0051d314: add r2, pc, r2
0051d318: add r3, pc, r3
0051d31c: add r0, r0, #0xa8
0051d320: str ip, [sp]
0051d324: bl #0x30e004
0051d328: b #0x51d2cc
0051d32c: ldr r2, [pc, #0xac]
0051d330: ldr r2, [r5, r2]
0051d334: ldr r2, [r2]
0051d338: cmp r2, #2
0051d33c: streq r3, [r3]
0051d340: beq #0x51d2b8
0051d344: cmp r2, #1
0051d348: bne #0x51d2b8
0051d34c: ldr r0, [pc, #0x90]
0051d350: ldr r1, [pc, #0x9c]
0051d354: ldr r2, [pc, #0x9c]
0051d358: ldr r0, [r5, r0]
0051d35c: ldr r3, [pc, #0x98]
0051d360: mov ip, #0x22
0051d364: add r1, pc, r1
0051d368: add r0, r0, #0xa8
0051d36c: add r2, pc, r2
0051d370: add r3, pc, r3
0051d374: str ip, [sp]
0051d378: bl #0x30e004
0051d37c: ldr r8, [r4, #0x70]
0051d380: b #0x51d2b8
0051d384: ldr r3, [pc, #0x54]
0051d388: ldr r3, [r5, r3]
0051d38c: ldr r3, [r3]
0051d390: cmp r3, #2
0051d394: streq r8, [r8]
0051d398: beq #0x51d2c0
0051d39c: cmp r3, #1
0051d3a0: bne #0x51d2c0
0051d3a4: ldr r0, [pc, #0x38]
0051d3a8: ldr r1, [pc, #0x50]
0051d3ac: ldr r2, [pc, #0x50]
0051d3b0: ldr r0, [r5, r0]
0051d3b4: ldr r3, [pc, #0x4c]
0051d3b8: mov ip, #0x23
0051d3bc: add r1, pc, r1
0051d3c0: add r2, pc, r2
0051d3c4: add r3, pc, r3
0051d3c8: add r0, r0, #0xa8
0051d3cc: str ip, [sp]
0051d3d0: bl #0x30e004
0051d3d4: b #0x51d2c0
0051d3d8: subeq r7, r7, ip, asr #17
0051d3dc: andeq r4, r0, r0, lsr #7
0051d3e0: andeq r3, r0, r0, asr #19
0051d3e4: andeq r1, r0, r0, asr #19
0051d3e8: eorseq r1, sl, r8, asr #1
0051d3ec: eorseq pc, fp, r4, lsl r6
0051d3f0: ldrhteq pc, [fp], -r8
0051d3f4: eorseq r1, sl, r4, ror r0
0051d3f8: eorseq pc, fp, ip, asr r5
0051d3fc: eorseq pc, fp, r0, ror #10
0051d400: eorseq r1, sl, ip, lsl r0
0051d404: eorseq pc, fp, r8, asr r5
0051d408: eorseq pc, fp, ip, lsl #10

# 0x51d40c _ZN7PFFloorC2EPKcP6PFRoomP13PFGOuterGraphP13PFGInnerGraphj
0051d40c: push {r4, r5, r6, r7, r8, lr}
0051d410: ldr r5, [pc, #0x218]
0051d414: ldr ip, [pc, #0x218]
0051d418: sub sp, sp, #0x10
0051d41c: add r5, pc, r5
0051d420: ldr ip, [r5, ip]
0051d424: mov r4, r0
0051d428: mov r6, r2
0051d42c: add ip, ip, #8
0051d430: add r2, sp, #0xc
0051d434: str ip, [r0], #4
0051d438: mov r8, r3
0051d43c: bl #0x3140ec
0051d440: str r6, [r4, #0x1c]
0051d444: ldr r3, [sp, #0x2c]
0051d448: mov r7, #0
0051d44c: add r0, r4, #0x28
0051d450: str r3, [r4, #0x20]
0051d454: str r0, [r4, #0x38]
0051d458: str r0, [r4, #0x3c]
0051d45c: mov r1, #0x10
0051d460: str r7, [r4, #0x24]
0051d464: bl #0x31167c
0051d468: ldr r0, [r4, #0x38]
0051d46c: mov r3, #0
0051d470: mov r1, r4
0051d474: strb r7, [r0]
0051d478: str r3, [r4, #0x64]
0051d47c: ldr r0, [sp, #0x28]
0051d480: mov r2, r4
0051d484: str r3, [r4, #0x44]
0051d488: str r3, [r4, #0x48]
0051d48c: str r3, [r4, #0x4c]
0051d490: str r3, [r4, #0x50]
0051d494: str r3, [r4, #0x54]
0051d498: str r3, [r4, #0x58]
0051d49c: str r3, [r4, #0x5c]
0051d4a0: str r3, [r4, #0x60]
0051d4a4: str r0, [r4, #0x74]
0051d4a8: str r7, [r4, #0x40]
0051d4ac: str r7, [r4, #0x68]
0051d4b0: str r7, [r4, #0x6c]
0051d4b4: str r8, [r4, #0x70]
0051d4b8: str r7, [r4, #0x7c]
0051d4bc: strb r7, [r1, #0x78]!
0051d4c0: str r1, [r4, #0x84]
0051d4c4: str r1, [r4, #0x80]
0051d4c8: str r7, [r4, #0x88]
0051d4cc: str r7, [r4, #0x94]
0051d4d0: strb r7, [r2, #0x90]!
0051d4d4: str r2, [r4, #0x9c]
0051d4d8: str r2, [r4, #0x98]
0051d4dc: str r7, [r4, #0xa0]
0051d4e0: str r7, [r4, #0xa8]
0051d4e4: str r7, [r4, #0xac]
0051d4e8: str r7, [r4, #0xb0]
0051d4ec: str r7, [r4, #0xb4]
0051d4f0: str r7, [r4, #0xb8]
0051d4f4: str r7, [r4, #0xbc]
0051d4f8: str r7, [r4, #0xc0]
0051d4fc: ldr r3, [r4, #0x1c]
0051d500: str r7, [r4, #0xc4]
0051d504: str r7, [r4, #0xc8]
0051d508: cmp r3, r7
0051d50c: beq #0x51d584
0051d510: cmp r8, #0
0051d514: beq #0x51d5dc
0051d518: ldr r3, [r4, #0x74]
0051d51c: cmp r3, #0
0051d520: beq #0x51d530
0051d524: mov r0, r4
0051d528: add sp, sp, #0x10
0051d52c: pop {r4, r5, r6, r7, r8, pc}
0051d530: ldr r2, [pc, #0x100]
0051d534: ldr r2, [r5, r2]
0051d538: ldr r2, [r2]
0051d53c: cmp r2, #2
0051d540: streq r3, [r3]
0051d544: beq #0x51d524
0051d548: cmp r2, #1
0051d54c: bne #0x51d524
0051d550: ldr r0, [pc, #0xe4]
0051d554: ldr r1, [pc, #0xe4]
0051d558: ldr r2, [pc, #0xe4]
0051d55c: ldr r0, [r5, r0]
0051d560: ldr r3, [pc, #0xe0]
0051d564: mov ip, #0x24
0051d568: add r1, pc, r1
0051d56c: add r2, pc, r2
0051d570: add r3, pc, r3
0051d574: add r0, r0, #0xa8
0051d578: str ip, [sp]
0051d57c: bl #0x30e004
0051d580: b #0x51d524
0051d584: ldr r2, [pc, #0xac]
0051d588: ldr r2, [r5, r2]
0051d58c: ldr r2, [r2]
0051d590: cmp r2, #2
0051d594: streq r3, [r3]
0051d598: beq #0x51d510
0051d59c: cmp r2, #1
0051d5a0: bne #0x51d510
0051d5a4: ldr r0, [pc, #0x90]
0051d5a8: ldr r1, [pc, #0x9c]
0051d5ac: ldr r2, [pc, #0x9c]
0051d5b0: ldr r0, [r5, r0]
0051d5b4: ldr r3, [pc, #0x98]
0051d5b8: mov ip, #0x22
0051d5bc: add r1, pc, r1
0051d5c0: add r0, r0, #0xa8
0051d5c4: add r2, pc, r2
0051d5c8: add r3, pc, r3
0051d5cc: str ip, [sp]
0051d5d0: bl #0x30e004
0051d5d4: ldr r8, [r4, #0x70]
0051d5d8: b #0x51d510
0051d5dc: ldr r3, [pc, #0x54]
0051d5e0: ldr r3, [r5, r3]
0051d5e4: ldr r3, [r3]
0051d5e8: cmp r3, #2
0051d5ec: streq r8, [r8]
0051d5f0: beq #0x51d518
0051d5f4: cmp r3, #1
0051d5f8: bne #0x51d518
0051d5fc: ldr r0, [pc, #0x38]
0051d600: ldr r1, [pc, #0x50]
0051d604: ldr r2, [pc, #0x50]
0051d608: ldr r0, [r5, r0]
0051d60c: ldr r3, [pc, #0x4c]
0051d610: mov ip, #0x23
0051d614: add r1, pc, r1
0051d618: add r2, pc, r2
0051d61c: add r3, pc, r3
0051d620: add r0, r0, #0xa8
0051d624: str ip, [sp]
0051d628: bl #0x30e004
0051d62c: b #0x51d518
0051d630: subeq r7, r7, r4, ror r6
0051d634: andeq r4, r0, r0, lsr #7
0051d638: andeq r3, r0, r0, asr #19
0051d63c: andeq r1, r0, r0, asr #19
0051d640: eorseq r0, sl, r0, ror lr
0051d644: ldrhteq pc, [fp], -ip
0051d648: eorseq pc, fp, r0, ror #6
0051d64c: eorseq r0, sl, ip, lsl lr
0051d650: eorseq pc, fp, r4, lsl #6
0051d654: eorseq pc, fp, r8, lsl #6
0051d658: eorseq r0, sl, r4, asr #27
0051d65c: eorseq pc, fp, r0, lsl #6
0051d660: ldrhteq pc, [fp], -r4

# 0x520b40 _ZN7PFFloor12_LoadNavMeshEPN6glitch5scene14IMeshSceneNodeE
00520b40: push {r4, r5, r6, r7, r8, sb, sl, lr}
00520b44: mov r4, r0
00520b48: sub sp, sp, #0x38
00520b4c: mov r0, r1
00520b50: mov r5, r1
00520b54: bl #0x597290
00520b58: ldr r3, [r0]
00520b5c: mov lr, pc
00520b60: ldr pc, [r3, #0xac]
00520b64: ldr sl, [pc, #0x400]
00520b68: add r8, sp, #8
00520b6c: mov r1, r0
00520b70: add r7, sp, #0x38
00520b74: add sl, pc, sl
00520b78: mov r0, r8
00520b7c: bl #0x319158
00520b80: add sb, r8, #4
00520b84: str sl, [r7, #-8]!
00520b88: mov r0, sb
00520b8c: mov r1, r7
00520b90: bl #0x51cfb8
00520b94: ldr r6, [pc, #0x3d4]
00520b98: cmp sb, r0
00520b9c: add r6, pc, r6
00520ba0: beq #0x520bd4
00520ba4: mov r1, r7
00520ba8: mov r0, sb
00520bac: str sl, [sp, #0x30]
00520bb0: bl #0x51cfb8
00520bb4: ldr sb, [r0, #0x3c]
00520bb8: add sl, r4, #0x28
00520bbc: mov r0, sb
00520bc0: bl #0x30de54
00520bc4: mov r1, sb
00520bc8: add r2, sb, r0
00520bcc: mov r0, sl
00520bd0: bl #0x3109e0
00520bd4: ldr sb, [r4, #0x3c]
00520bd8: ldr r1, [pc, #0x394]
00520bdc: mov r0, sb
00520be0: add r1, pc, r1
00520be4: bl #0x30ebd4
00520be8: ldr sl, [r4, #0x24]
00520bec: ldr r1, [pc, #0x384]
00520bf0: cmp r0, #0
00520bf4: orrne sl, sl, #0x1000000
00520bf8: strne sl, [r4, #0x24]
00520bfc: add r1, pc, r1
00520c00: mov r0, sb
00520c04: bl #0x30ebd4
00520c08: ldr r1, [pc, #0x36c]
00520c0c: cmp r0, #0
00520c10: orrne sl, sl, #0x2000000
00520c14: strne sl, [r4, #0x24]
00520c18: add r1, pc, r1
00520c1c: mov r0, sb
00520c20: bl #0x30ebd4
00520c24: ldr r1, [pc, #0x354]
00520c28: cmp r0, #0
00520c2c: orrne sl, sl, #1
00520c30: strne sl, [r4, #0x24]
00520c34: add r1, pc, r1
00520c38: mov r0, sb
00520c3c: bl #0x30ebd4
00520c40: cmp r0, #0
00520c44: orrne sl, sl, #2
00520c48: strne sl, [r4, #0x24]
00520c4c: tst sl, #0x3000000
00520c50: ldrne r3, [r4, #0x20]
00520c54: mov r0, r5
00520c58: orrne r3, r3, #0x7000000
00520c5c: strne r3, [r4, #0x20]
00520c60: bl #0x597290
00520c64: cmp r0, #0
00520c68: beq #0x520f18
00520c6c: mov r0, r5
00520c70: bl #0x597290
00520c74: cmp r0, #0
00520c78: beq #0x520ca0
00520c7c: ldr r3, [r5]
00520c80: add r6, sp, #0x24
00520c84: mov r0, r6
00520c88: mov r1, r5
00520c8c: ldr sl, [r3, #0xa4]
00520c90: bl #0x597180
00520c94: mov r0, r5
00520c98: mov r1, r6
00520c9c: blx sl
00520ca0: mov r0, r5
00520ca4: bl #0x50f89c
00520ca8: str r0, [r4, #0x40]
00520cac: mov r1, #0
00520cb0: mov r0, r5
00520cb4: ldr r3, [r5]
00520cb8: mov lr, pc
00520cbc: ldr pc, [r3, #0x48]
00520cc0: mov r0, r5
00520cc4: ldr r3, [r5]
00520cc8: mov lr, pc
00520ccc: ldr pc, [r3, #0x68]
00520cd0: ldr r3, [r4, #0x40]
00520cd4: mov r0, r3
00520cd8: ldr r3, [r3]
00520cdc: mov lr, pc
00520ce0: ldr pc, [r3, #0xa0]
00520ce4: ldr r1, [r0]
00520ce8: mov r3, r0
00520cec: ldr r2, [r4, #0x40]
00520cf0: str r1, [r4, #0x5c]
00520cf4: ldr r1, [r0, #4]
00520cf8: mov r0, r2
00520cfc: str r1, [r4, #0x60]
00520d00: ldr r3, [r3, #8]
00520d04: str r3, [r4, #0x64]
00520d08: ldr r3, [r2]
00520d0c: mov lr, pc
00520d10: ldr pc, [r3, #0x34]
00520d14: ldr r3, [r0]
00520d18: mov r1, #0x44000000
00520d1c: add r1, r1, #0x7a0000
00520d20: str r3, [r4, #0x44]
00520d24: ldr r3, [r0, #4]
00520d28: str r3, [r4, #0x48]
00520d2c: ldr r5, [r0, #8]
00520d30: str r5, [r4, #0x4c]
00520d34: ldr r3, [r0, #0xc]
00520d38: str r3, [r4, #0x50]
00520d3c: ldr r3, [r0, #0x10]
00520d40: str r3, [r4, #0x54]
00520d44: ldr r0, [r0, #0x14]
00520d48: bl #0x30eba4
00520d4c: mov r1, #0x44000000
00520d50: str r0, [r4, #0x58]
00520d54: add r1, r1, #0x7a0000
00520d58: mov r0, r5
00520d5c: bl #0x30e3ac
00520d60: ldr r3, [r4, #0x40]
00520d64: str r0, [r4, #0x4c]
00520d68: add r0, sp, #0x34
00520d6c: mov r1, r3
00520d70: ldr r3, [r3]
00520d74: mov lr, pc
00520d78: ldr pc, [r3, #0xf8]
00520d7c: mov r1, #0
00520d80: mov r0, #0xb8
00520d84: ldr r5, [sp, #0x34]
00520d88: bl #0x5341ac
00520d8c: ldr r2, [r4, #0x40]
00520d90: mov ip, #1
00520d94: mov r1, r5
00520d98: mov r3, #0xf
00520d9c: mov r6, r0
00520da0: str ip, [sp]
00520da4: bl #0x588654
00520da8: ldr r0, [sp, #0x34]
00520dac: cmp r0, #0
00520db0: beq #0x520db8
00520db4: bl #0x31d584
00520db8: ldr r3, [r4, #0x40]
00520dbc: mov r1, r6
00520dc0: mov r0, r3
00520dc4: ldr r3, [r3]
00520dc8: mov lr, pc
00520dcc: ldr pc, [r3, #0xb4]
00520dd0: mov r0, r6
00520dd4: bl #0x31d584
00520dd8: ldr r3, [r6]
00520ddc: mov r0, r6
00520de0: mov lr, pc
00520de4: ldr pc, [r3, #0xc]
00520de8: cmp r0, #0
00520dec: mov sl, r0
00520df0: str r0, [sp, #0x30]
00520df4: movle r5, #0
00520df8: ble #0x520e7c
00520dfc: mov r0, #0x24
00520e00: mov r1, #0
00520e04: mul r0, r0, sl
00520e08: bl #0x31056c
00520e0c: mov r2, #0
00520e10: mov r5, r0
00520e14: mov r3, r0
00520e18: mov r1, #0
00520e1c: b #0x520e24
00520e20: add r3, r3, #0x24
00520e24: add r1, r1, #1
00520e28: cmp sl, r1
00520e2c: str r2, [r3]
00520e30: str r2, [r3, #4]
00520e34: str r2, [r3, #8]
00520e38: str r2, [r3, #0xc]
00520e3c: str r2, [r3, #0x10]
00520e40: str r2, [r3, #0x14]
00520e44: str r2, [r3, #0x18]
00520e48: str r2, [r3, #0x1c]
00520e4c: str r2, [r3, #0x20]
00520e50: bne #0x520e20
00520e54: mov r1, #0
00520e58: ldr ip, [r6]
00520e5c: mov r0, r6
00520e60: str r1, [sp]
00520e64: ldr r2, [sp, #0x30]
00520e68: mov r3, r7
00520e6c: mov r1, r5
00520e70: mov lr, pc
00520e74: ldr pc, [ip, #0x10]
00520e78: ldr sl, [sp, #0x30]
00520e7c: mov r2, sl
00520e80: mov r0, r4
00520e84: mov r1, r5
00520e88: bl #0x520588
00520e8c: ldr r3, [sp, #0x30]
00520e90: str r5, [r4, #0x68]
00520e94: cmp r3, #0
00520e98: str r3, [r4, #0x6c]
00520e9c: beq #0x520f08
00520ea0: mov r6, #0
00520ea4: mov r7, r6
00520ea8: b #0x520eb0
00520eac: ldr r5, [r4, #0x68]
00520eb0: add r5, r5, r6
00520eb4: ldr r0, [r5, #8]
00520eb8: mov r1, #0x3f800000
00520ebc: bl #0x30eba4
00520ec0: str r0, [r5, #8]
00520ec4: ldr r5, [r4, #0x68]
00520ec8: mov r1, #0x3f800000
00520ecc: add r7, r7, #1
00520ed0: add r5, r5, r6
00520ed4: ldr r0, [r5, #0x14]
00520ed8: bl #0x30eba4
00520edc: str r0, [r5, #0x14]
00520ee0: ldr r5, [r4, #0x68]
00520ee4: mov r1, #0x3f800000
00520ee8: add r5, r5, r6
00520eec: ldr r0, [r5, #0x20]
00520ef0: bl #0x30eba4
00520ef4: str r0, [r5, #0x20]
00520ef8: ldr r3, [r4, #0x6c]
00520efc: add r6, r6, #0x24
00520f00: cmp r3, r7
00520f04: bhi #0x520eac
00520f08: mov r0, r8
00520f0c: bl #0x318178
00520f10: add sp, sp, #0x38
00520f14: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00520f18: ldr r3, [pc, #0x64]
00520f1c: ldr r3, [r6, r3]
00520f20: ldr r3, [r3]
00520f24: cmp r3, #2
00520f28: streq r0, [r0]
00520f2c: beq #0x520c6c
00520f30: cmp r3, #1
00520f34: bne #0x520c6c
00520f38: ldr r0, [pc, #0x48]
00520f3c: ldr r1, [pc, #0x48]
00520f40: ldr r2, [pc, #0x48]
00520f44: ldr r0, [r6, r0]
00520f48: ldr r3, [pc, #0x44]
00520f4c: mov ip, #0x8a
00520f50: add r1, pc, r1
00520f54: add r2, pc, r2
00520f58: add r3, pc, r3
00520f5c: add r0, r0, #0xa8
00520f60: str ip, [sp]
00520f64: bl #0x30e004
00520f68: b #0x520c6c
00520f6c: eorseq fp, fp, r4, asr #27
00520f70: strdeq r3, r4, [r7], #-0xe4
00520f74: eorseq sl, lr, r0, lsl fp
00520f78: eorseq fp, fp, ip, asr #26
00520f7c: eorseq fp, fp, r8, lsr sp
00520f80: eorseq fp, fp, r4, lsr #26
00520f84: andeq r3, r0, r0, asr #19
00520f88: andeq r1, r0, r0, asr #19
00520f8c: eorseq sp, sb, r8, lsl #9
00520f90: eorseq fp, fp, ip, lsl #20
00520f94: eorseq fp, fp, r8, ror sb

# 0x521540 _ZN6PFRoomC1EPKcjP7PFWorldP13PFGOuterGraphP13PFGInnerGraph
00521540: push {r4, r5, r6, r7, r8, lr}
00521544: ldr r5, [pc, #0x198]
00521548: ldr ip, [pc, #0x198]
0052154c: sub sp, sp, #0x10
00521550: add r5, pc, r5
00521554: ldr ip, [r5, ip]
00521558: mov r4, r0
0052155c: mov r6, r2
00521560: add ip, ip, #8
00521564: str ip, [r0], #4
00521568: add r2, sp, #0xc
0052156c: mov r8, r3
00521570: ldr r7, [sp, #0x28]
00521574: bl #0x3140ec
00521578: str r6, [r4, #0x1c]
0052157c: ldr r1, [sp, #0x2c]
00521580: mov r3, #0
00521584: mov r2, #0
00521588: cmp r8, #0
0052158c: str r1, [r4, #0x2c]
00521590: str r2, [r4, #0x38]
00521594: str r3, [r4, #0x50]
00521598: str r8, [r4, #0x20]
0052159c: str r2, [r4, #0x24]
005215a0: str r7, [r4, #0x28]
005215a4: str r2, [r4, #0x30]
005215a8: str r2, [r4, #0x34]
005215ac: str r3, [r4, #0x3c]
005215b0: str r3, [r4, #0x40]
005215b4: str r3, [r4, #0x44]
005215b8: str r3, [r4, #0x48]
005215bc: str r3, [r4, #0x4c]
005215c0: beq #0x521638
005215c4: cmp r7, #0
005215c8: beq #0x521690
005215cc: ldr r3, [r4, #0x2c]
005215d0: cmp r3, #0
005215d4: beq #0x5215e4
005215d8: mov r0, r4
005215dc: add sp, sp, #0x10
005215e0: pop {r4, r5, r6, r7, r8, pc}
005215e4: ldr r2, [pc, #0x100]
005215e8: ldr r2, [r5, r2]
005215ec: ldr r2, [r2]
005215f0: cmp r2, #2
005215f4: streq r3, [r3]
005215f8: beq #0x5215d8
005215fc: cmp r2, #1
00521600: bne #0x5215d8
00521604: ldr r0, [pc, #0xe4]
00521608: ldr r1, [pc, #0xe4]
0052160c: ldr r2, [pc, #0xe4]
00521610: ldr r0, [r5, r0]
00521614: ldr r3, [pc, #0xe0]
00521618: mov ip, #0x1e
0052161c: add r1, pc, r1
00521620: add r2, pc, r2
00521624: add r3, pc, r3
00521628: add r0, r0, #0xa8
0052162c: str ip, [sp]
00521630: bl #0x30e004
00521634: b #0x5215d8
00521638: ldr r3, [pc, #0xac]
0052163c: ldr r3, [r5, r3]
00521640: ldr r3, [r3]
00521644: cmp r3, #2
00521648: streq r8, [r8]
0052164c: beq #0x5215c4
00521650: cmp r3, #1
00521654: bne #0x5215c4
00521658: ldr r0, [pc, #0x90]
0052165c: ldr r1, [pc, #0x9c]
00521660: ldr r2, [pc, #0x9c]
00521664: ldr r0, [r5, r0]
00521668: ldr r3, [pc, #0x98]
0052166c: mov ip, #0x1c
00521670: add r1, pc, r1
00521674: add r0, r0, #0xa8
00521678: add r2, pc, r2
0052167c: add r3, pc, r3
00521680: str ip, [sp]
00521684: bl #0x30e004
00521688: ldr r7, [r4, #0x28]
0052168c: b #0x5215c4
00521690: ldr r3, [pc, #0x54]
00521694: ldr r3, [r5, r3]
00521698: ldr r3, [r3]
0052169c: cmp r3, #2
005216a0: streq r7, [r7]
005216a4: beq #0x5215cc
005216a8: cmp r3, #1
005216ac: bne #0x5215cc
005216b0: ldr r0, [pc, #0x38]
005216b4: ldr r1, [pc, #0x50]
005216b8: ldr r2, [pc, #0x50]
005216bc: ldr r0, [r5, r0]
005216c0: ldr r3, [pc, #0x4c]
005216c4: mov ip, #0x1d
005216c8: add r1, pc, r1
005216cc: add r2, pc, r2
005216d0: add r3, pc, r3
005216d4: add r0, r0, #0xa8
005216d8: str ip, [sp]
005216dc: bl #0x30e004
005216e0: b #0x5215cc
005216e4: subeq r3, r7, r0, asr #10
005216e8: andeq r3, r0, r4, lsr #18
005216ec: andeq r3, r0, r0, asr #19
005216f0: andeq r1, r0, r0, asr #19
005216f4: ldrhteq ip, [sb], -ip
005216f8: eorseq fp, fp, r8, lsl #6
005216fc: eorseq fp, fp, ip, asr r3
00521700: eorseq ip, sb, r8, ror #26
00521704: eorseq fp, fp, r0, lsl #6
00521708: eorseq fp, fp, r4, lsl #6
0052170c: eorseq ip, sb, r0, lsl sp
00521710: eorseq fp, fp, ip, asr #4
00521714: ldrhteq fp, [fp], -r0

# 0x521718 _ZN6PFRoomC2EPKcjP7PFWorldP13PFGOuterGraphP13PFGInnerGraph
00521718: push {r4, r5, r6, r7, r8, lr}
0052171c: ldr r5, [pc, #0x198]
00521720: ldr ip, [pc, #0x198]
00521724: sub sp, sp, #0x10
00521728: add r5, pc, r5
0052172c: ldr ip, [r5, ip]
00521730: mov r4, r0
00521734: mov r6, r2
00521738: add ip, ip, #8
0052173c: str ip, [r0], #4
00521740: add r2, sp, #0xc
00521744: mov r8, r3
00521748: ldr r7, [sp, #0x28]
0052174c: bl #0x3140ec
00521750: str r6, [r4, #0x1c]
00521754: ldr r1, [sp, #0x2c]
00521758: mov r3, #0
0052175c: mov r2, #0
00521760: cmp r8, #0
00521764: str r1, [r4, #0x2c]
00521768: str r2, [r4, #0x38]
0052176c: str r3, [r4, #0x50]
00521770: str r8, [r4, #0x20]
00521774: str r2, [r4, #0x24]
00521778: str r7, [r4, #0x28]
0052177c: str r2, [r4, #0x30]
00521780: str r2, [r4, #0x34]
00521784: str r3, [r4, #0x3c]
00521788: str r3, [r4, #0x40]
0052178c: str r3, [r4, #0x44]
00521790: str r3, [r4, #0x48]
00521794: str r3, [r4, #0x4c]
00521798: beq #0x521810
0052179c: cmp r7, #0
005217a0: beq #0x521868
005217a4: ldr r3, [r4, #0x2c]
005217a8: cmp r3, #0
005217ac: beq #0x5217bc
005217b0: mov r0, r4
005217b4: add sp, sp, #0x10
005217b8: pop {r4, r5, r6, r7, r8, pc}
005217bc: ldr r2, [pc, #0x100]
005217c0: ldr r2, [r5, r2]
005217c4: ldr r2, [r2]
005217c8: cmp r2, #2
005217cc: streq r3, [r3]
005217d0: beq #0x5217b0
005217d4: cmp r2, #1
005217d8: bne #0x5217b0
005217dc: ldr r0, [pc, #0xe4]
005217e0: ldr r1, [pc, #0xe4]
005217e4: ldr r2, [pc, #0xe4]
005217e8: ldr r0, [r5, r0]
005217ec: ldr r3, [pc, #0xe0]
005217f0: mov ip, #0x1e
005217f4: add r1, pc, r1
005217f8: add r2, pc, r2
005217fc: add r3, pc, r3
00521800: add r0, r0, #0xa8
00521804: str ip, [sp]
00521808: bl #0x30e004
0052180c: b #0x5217b0
00521810: ldr r3, [pc, #0xac]
00521814: ldr r3, [r5, r3]
00521818: ldr r3, [r3]
0052181c: cmp r3, #2
00521820: streq r8, [r8]
00521824: beq #0x52179c
00521828: cmp r3, #1
0052182c: bne #0x52179c
00521830: ldr r0, [pc, #0x90]
00521834: ldr r1, [pc, #0x9c]
00521838: ldr r2, [pc, #0x9c]
0052183c: ldr r0, [r5, r0]
00521840: ldr r3, [pc, #0x98]
00521844: mov ip, #0x1c
00521848: add r1, pc, r1
0052184c: add r0, r0, #0xa8
00521850: add r2, pc, r2
00521854: add r3, pc, r3
00521858: str ip, [sp]
0052185c: bl #0x30e004
00521860: ldr r7, [r4, #0x28]
00521864: b #0x52179c
00521868: ldr r3, [pc, #0x54]
0052186c: ldr r3, [r5, r3]
00521870: ldr r3, [r3]
00521874: cmp r3, #2
00521878: streq r7, [r7]
0052187c: beq #0x5217a4
00521880: cmp r3, #1
00521884: bne #0x5217a4
00521888: ldr r0, [pc, #0x38]
0052188c: ldr r1, [pc, #0x50]
00521890: ldr r2, [pc, #0x50]
00521894: ldr r0, [r5, r0]
00521898: ldr r3, [pc, #0x4c]
0052189c: mov ip, #0x1d
005218a0: add r1, pc, r1
005218a4: add r2, pc, r2
005218a8: add r3, pc, r3
005218ac: add r0, r0, #0xa8
005218b0: str ip, [sp]
005218b4: bl #0x30e004
005218b8: b #0x5217a4
005218bc: subeq r3, r7, r8, ror #6
005218c0: andeq r3, r0, r4, lsr #18
005218c4: andeq r3, r0, r0, asr #19
005218c8: andeq r1, r0, r0, asr #19
005218cc: eorseq ip, sb, r4, ror #23
005218d0: eorseq fp, fp, r0, lsr r1
005218d4: eorseq fp, fp, r4, lsl #3
005218d8: mlaseq sb, r0, fp, ip
005218dc: eorseq fp, fp, r8, lsr #2
005218e0: eorseq fp, fp, ip, lsr #2
005218e4: eorseq ip, sb, r8, lsr fp
005218e8: eorseq fp, fp, r4, ror r0
005218ec: ldrsbteq fp, [fp], -r8

# 0x521eb8 _ZN6PFRoom10_LoadFloorEPN6glitch5scene14IMeshSceneNodeEPKc
00521eb8: push {r4, r5, r6, r7, r8, sb, sl, lr}
00521ebc: ldr r6, [pc, #0x360]
00521ec0: ldr r7, [pc, #0x360]
00521ec4: sub sp, sp, #0x30
00521ec8: add r6, pc, r6
00521ecc: ldr r3, [r6, r7]
00521ed0: subs sb, r1, #0
00521ed4: mov r4, r0
00521ed8: ldr r3, [r3]
00521edc: mov r8, r2
00521ee0: str r3, [sp, #0x2c]
00521ee4: beq #0x522120
00521ee8: mov r1, #0
00521eec: mov r0, #0xcc
00521ef0: bl #0x310570
00521ef4: ldr ip, [r4, #0x2c]
00521ef8: ldr r3, [r4, #0x28]
00521efc: mov r1, r8
00521f00: str ip, [sp]
00521f04: mov r2, r4
00521f08: mov ip, #1
00521f0c: mov r5, r0
00521f10: str ip, [sp, #4]
00521f14: bl #0x51d1b4
00521f18: ldr r8, [r4, #0x34]
00521f1c: ldr r3, [r4, #0x38]
00521f20: cmp r8, r3
00521f24: beq #0x522174
00521f28: str r5, [r8]
00521f2c: ldr r3, [r4, #0x34]
00521f30: add r3, r3, #4
00521f34: str r3, [r4, #0x34]
00521f38: ldr r3, [pc, #0x2ec]
00521f3c: add r8, sp, #0x14
00521f40: ldr sl, [r6, r3]
00521f44: mov r0, sl
00521f48: bl #0x337888
00521f4c: ldr r1, [pc, #0x2dc]
00521f50: add r2, sp, #0x10
00521f54: mov r0, r8
00521f58: add r1, pc, r1
00521f5c: bl #0x3140ec
00521f60: mov r0, sl
00521f64: mov r1, r8
00521f68: bl #0x337a88
00521f6c: mov sl, r0
00521f70: ldr r0, [sp, #0x28]
00521f74: cmp r0, r8
00521f78: beq #0x521f98
00521f7c: cmp r0, #0
00521f80: beq #0x521f98
00521f84: ldr r1, [sp, #0x14]
00521f88: rsb r1, r0, r1
00521f8c: cmp r1, #0x80
00521f90: bhi #0x522118
00521f94: bl #0x708f00
00521f98: cmp sl, #0
00521f9c: beq #0x5220c0
00521fa0: bl #0x60b0cc
00521fa4: mov r0, r5
00521fa8: mov r1, sb
00521fac: bl #0x520b40
00521fb0: bl #0x60b0cc
00521fb4: ldr r2, [r4, #0x34]
00521fb8: ldr r3, [r4, #0x30]
00521fbc: rsb r3, r3, r2
00521fc0: asr r3, r3, #2
00521fc4: cmp r3, #1
00521fc8: beq #0x5220e4
00521fcc: ldr r8, [r5, #0x44]
00521fd0: ldr sl, [r4, #0x3c]
00521fd4: mov r0, r8
00521fd8: mov r1, sl
00521fdc: bl #0x30e70c
00521fe0: cmp r0, #0
00521fe4: moveq r8, sl
00521fe8: str r8, [r4, #0x3c]
00521fec: ldr r8, [r5, #0x48]
00521ff0: ldr sl, [r4, #0x40]
00521ff4: mov r0, r8
00521ff8: mov r1, sl
00521ffc: bl #0x30e70c
00522000: cmp r0, #0
00522004: moveq r8, sl
00522008: str r8, [r4, #0x40]
0052200c: ldr r8, [r5, #0x4c]
00522010: ldr sl, [r4, #0x44]
00522014: mov r0, r8
00522018: mov r1, sl
0052201c: bl #0x30e70c
00522020: cmp r0, #0
00522024: moveq r8, sl
00522028: str r8, [r4, #0x44]
0052202c: ldr r8, [r5, #0x50]
00522030: ldr sl, [r4, #0x48]
00522034: mov r1, r8
00522038: mov r0, sl
0052203c: bl #0x30e70c
00522040: cmp r0, #0
00522044: moveq r8, sl
00522048: str r8, [r4, #0x48]
0052204c: ldr r8, [r5, #0x54]
00522050: ldr sl, [r4, #0x4c]
00522054: mov r1, r8
00522058: mov r0, sl
0052205c: bl #0x30e70c
00522060: cmp r0, #0
00522064: moveq r8, sl
00522068: str r8, [r4, #0x4c]
0052206c: ldr sl, [r4, #0x50]
00522070: ldr r8, [r5, #0x58]
00522074: mov r0, sl
00522078: mov r1, r8
0052207c: bl #0x30e70c
00522080: cmp r0, #0
00522084: moveq r8, sl
00522088: str r8, [r4, #0x50]
0052208c: ldr r3, [pc, #0x1a0]
00522090: ldr r1, [r5, #0x40]
00522094: ldr r3, [r6, r3]
00522098: ldr r3, [r3, #0x10]
0052209c: ldr r0, [r3, #0x1c]
005220a0: bl #0x3524a0
005220a4: ldr r3, [r6, r7]
005220a8: ldr r2, [sp, #0x2c]
005220ac: ldr r3, [r3]
005220b0: cmp r2, r3
005220b4: bne #0x522220
005220b8: add sp, sp, #0x30
005220bc: pop {r4, r5, r6, r7, r8, sb, sl, pc}
005220c0: mov r1, sb
005220c4: mov r0, r5
005220c8: bl #0x520b40
005220cc: ldr r2, [r4, #0x34]
005220d0: ldr r3, [r4, #0x30]
005220d4: rsb r3, r3, r2
005220d8: asr r3, r3, #2
005220dc: cmp r3, #1
005220e0: bne #0x521fcc
005220e4: ldr r3, [r5, #0x44]
005220e8: str r3, [r4, #0x3c]
005220ec: ldr r3, [r5, #0x48]
005220f0: str r3, [r4, #0x40]
005220f4: ldr r3, [r5, #0x4c]
005220f8: str r3, [r4, #0x44]
005220fc: ldr r3, [r5, #0x50]
00522100: str r3, [r4, #0x48]
00522104: ldr r3, [r5, #0x54]
00522108: str r3, [r4, #0x4c]
0052210c: ldr r3, [r5, #0x58]
00522110: str r3, [r4, #0x50]
00522114: b #0x52208c
00522118: bl #0x310440
0052211c: b #0x521f98
00522120: ldr r3, [pc, #0x110]
00522124: ldr r3, [r6, r3]
00522128: ldr r3, [r3]
0052212c: cmp r3, #2
00522130: streq sb, [sb]
00522134: beq #0x521ee8
00522138: cmp r3, #1
0052213c: bne #0x521ee8
00522140: ldr r0, [pc, #0xf4]
00522144: ldr r1, [pc, #0xf4]
00522148: ldr r2, [pc, #0xf4]
0052214c: ldr r0, [r6, r0]
00522150: ldr r3, [pc, #0xf0]
00522154: mov ip, #0x35
00522158: add r1, pc, r1
0052215c: add r2, pc, r2
00522160: add r3, pc, r3
00522164: add r0, r0, #0xa8
00522168: str ip, [sp]
0052216c: bl #0x30e004
00522170: b #0x521ee8
00522174: ldr r3, [r4, #0x30]
00522178: rsb r3, r3, r8
0052217c: asr r3, r3, #2
00522180: cmp r3, #1
00522184: addhs r1, r3, r3
00522188: addlo r1, r3, #1
0052218c: cmn r1, #0xc0000001
00522190: bls #0x5221fc
00522194: mvn r1, #0xc0000000
00522198: add r2, sp, #0x30
0052219c: str r1, [r2, #-0x24]!
005221a0: add r0, r4, #0x38
005221a4: bl #0x521d40
005221a8: ldr r1, [r4, #0x30]
005221ac: mov sl, r0
005221b0: subs r8, r8, r1
005221b4: moveq r8, r0
005221b8: bne #0x522210
005221bc: str r5, [r8], #4
005221c0: ldr r0, [r4, #0x30]
005221c4: ldr r1, [r4, #0x38]
005221c8: cmp r0, #0
005221cc: beq #0x5221e4
005221d0: rsb r1, r0, r1
005221d4: bic r1, r1, #3
005221d8: cmp r1, #0x80
005221dc: bhi #0x522208
005221e0: bl #0x708f00
005221e4: ldr r3, [sp, #0xc]
005221e8: str sl, [r4, #0x30]
005221ec: str r8, [r4, #0x34]
005221f0: add sl, sl, r3, lsl #2
005221f4: str sl, [r4, #0x38]
005221f8: b #0x521f38
005221fc: cmp r3, r1
00522200: bls #0x522198
00522204: b #0x522194
00522208: bl #0x310440
0052220c: b #0x5221e4
00522210: mov r2, r8
00522214: bl #0x30df38
00522218: add r8, r0, r8
0052221c: b #0x5221bc
00522220: bl #0x30e310
00522224: subeq r2, r7, r8, asr #23
00522228: andeq r4, r0, ip, lsr #1
0052222c: andeq r0, r0, r4, lsl #17
00522230: mlaseq fp, r8, sl, sl
00522234: strdeq r3, r4, [r0], -r4
00522238: andeq r3, r0, r0, asr #19
0052223c: andeq r1, r0, r0, asr #19
00522240: eorseq ip, sb, r0, lsl #5
00522244: eorseq sl, fp, ip, lsl #17
00522248: eorseq sl, fp, r0, lsr #16

# 0x522844 _ZN7PFWorld5_InitEv
00522844: push {r4, r5, r6, lr}
00522848: ldr r3, [r0, #4]
0052284c: ldr r4, [pc, #0x220]
00522850: sub sp, sp, #8
00522854: cmp r3, #0
00522858: mov r5, r0
0052285c: add r4, pc, r4
00522860: beq #0x522884
00522864: ldr r3, [r0, #0x44]
00522868: cmp r3, #0
0052286c: beq #0x5229b8
00522870: ldr r3, [r5, #0x48]
00522874: cmp r3, #0
00522878: beq #0x522964
0052287c: add sp, sp, #8
00522880: pop {r4, r5, r6, pc}
00522884: ldr r2, [r0, #0x44]
00522888: cmp r2, #0
0052288c: beq #0x5228b0
00522890: ldr r2, [pc, #0x1e0]
00522894: ldr r2, [r4, r2]
00522898: ldr r2, [r2]
0052289c: cmp r2, #2
005228a0: streq r3, [r3]
005228a4: beq #0x5228b0
005228a8: cmp r2, #1
005228ac: beq #0x522a0c
005228b0: ldr r3, [r5, #0x48]
005228b4: cmp r3, #0
005228b8: beq #0x5228e0
005228bc: ldr r3, [pc, #0x1b4]
005228c0: ldr r3, [r4, r3]
005228c4: ldr r3, [r3]
005228c8: cmp r3, #2
005228cc: moveq r3, #0
005228d0: streq r3, [r3]
005228d4: beq #0x5228e0
005228d8: cmp r3, #1
005228dc: beq #0x522a40
005228e0: mov r1, #0
005228e4: mov r0, #0x1c
005228e8: bl #0x310570
005228ec: ldr r2, [pc, #0x188]
005228f0: mov r6, #0
005228f4: mov r3, r0
005228f8: ldr r2, [r4, r2]
005228fc: str r6, [r0, #8]
00522900: strb r6, [r3, #4]!
00522904: add r2, r2, #8
00522908: str r3, [r0, #0x10]
0052290c: str r2, [r0]
00522910: str r3, [r0, #0xc]
00522914: str r6, [r0, #0x14]
00522918: mov r1, r6
0052291c: str r0, [r5, #0x44]
00522920: mov r0, #0x20
00522924: bl #0x310570
00522928: ldr r2, [pc, #0x150]
0052292c: mov r3, r0
00522930: str r6, [r0, #8]
00522934: ldr r2, [r4, r2]
00522938: strb r6, [r3, #4]!
0052293c: str r3, [r0, #0x10]
00522940: str r3, [r0, #0xc]
00522944: add r2, r2, #8
00522948: mov r3, #1
0052294c: str r2, [r0]
00522950: str r6, [r0, #0x1c]
00522954: str r6, [r0, #0x14]
00522958: str r3, [r5, #4]
0052295c: str r0, [r5, #0x48]
00522960: b #0x52287c
00522964: ldr r2, [pc, #0x10c]
00522968: ldr r2, [r4, r2]
0052296c: ldr r2, [r2]
00522970: cmp r2, #2
00522974: streq r3, [r3]
00522978: beq #0x52287c
0052297c: cmp r2, #1
00522980: bne #0x52287c
00522984: ldr r0, [pc, #0xf8]
00522988: ldr r1, [pc, #0xf8]
0052298c: ldr r2, [pc, #0xf8]
00522990: ldr r0, [r4, r0]
00522994: ldr r3, [pc, #0xf4]
00522998: mov ip, #0x2f
0052299c: add r1, pc, r1
005229a0: add r2, pc, r2
005229a4: add r3, pc, r3
005229a8: add r0, r0, #0xa8
005229ac: str ip, [sp]
005229b0: bl #0x30e004
005229b4: b #0x52287c
005229b8: ldr r2, [pc, #0xb8]
005229bc: ldr r2, [r4, r2]
005229c0: ldr r2, [r2]
005229c4: cmp r2, #2
005229c8: streq r3, [r3]
005229cc: beq #0x522870
005229d0: cmp r2, #1
005229d4: bne #0x522870
005229d8: ldr r0, [pc, #0xa4]
005229dc: ldr r1, [pc, #0xb0]
005229e0: ldr r2, [pc, #0xb0]
005229e4: ldr r0, [r4, r0]
005229e8: ldr r3, [pc, #0xac]
005229ec: mov ip, #0x2e
005229f0: add r1, pc, r1
005229f4: add r2, pc, r2
005229f8: add r3, pc, r3
005229fc: add r0, r0, #0xa8
00522a00: str ip, [sp]
00522a04: bl #0x30e004
00522a08: b #0x522870
00522a0c: ldr r0, [pc, #0x70]
00522a10: ldr r1, [pc, #0x88]
00522a14: ldr r2, [pc, #0x88]
00522a18: ldr r0, [r4, r0]
00522a1c: ldr r3, [pc, #0x84]
00522a20: mov ip, #0x33
00522a24: add r1, pc, r1
00522a28: add r2, pc, r2
00522a2c: add r3, pc, r3
00522a30: add r0, r0, #0xa8
00522a34: str ip, [sp]
00522a38: bl #0x30e004
00522a3c: b #0x5228b0
00522a40: ldr r0, [pc, #0x3c]
00522a44: ldr r1, [pc, #0x60]
00522a48: ldr r2, [pc, #0x60]
00522a4c: ldr r0, [r4, r0]
00522a50: ldr r3, [pc, #0x5c]
00522a54: mov ip, #0x34
00522a58: add r1, pc, r1
00522a5c: add r2, pc, r2
00522a60: add r3, pc, r3
00522a64: add r0, r0, #0xa8
00522a68: str ip, [sp]
00522a6c: bl #0x30e004
00522a70: b #0x5228e0
00522a74: subeq r2, r7, r4, lsr r2
00522a78: andeq r3, r0, r0, asr #19
00522a7c: andeq r2, r0, ip, lsr #12
00522a80: muleq r0, ip, sp
00522a84: andeq r1, r0, r0, asr #19
00522a88: eorseq fp, sb, ip, lsr sl
00522a8c: eorseq sb, fp, r8, lsl #31
00522a90: eorseq sl, fp, ip, rrx
00522a94: eorseq fp, sb, r8, ror #19
00522a98: eorseq sb, fp, r4, lsr #30
00522a9c: eorseq sl, fp, r8, lsl r0
00522aa0: ldrhteq fp, [sb], -r4
00522aa4: eorseq sl, fp, r0, lsr r0
00522aa8: eorseq sb, fp, r4, ror #31
00522aac: eorseq fp, sb, r0, lsl #19
00522ab0: eorseq sl, fp, ip
00522ab4: ldrhteq sb, [fp], -r0

# 0x5239e8 _ZN7PFWorld16_AddExitPositionEPN6glitch5scene10ISceneNodeEPKc
005239e8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005239ec: mov r5, r0
005239f0: ldr r3, [r1]
005239f4: sub sp, sp, #0x14
005239f8: mov r0, r1
005239fc: mov r4, r1
00523a00: mov lr, pc
00523a04: ldr pc, [r3, #0x24]
00523a08: ldr r1, [pc, #0x1f4]
00523a0c: add r1, pc, r1
00523a10: bl #0x30ebd4
00523a14: cmp r0, #0
00523a18: movne sb, #0
00523a1c: beq #0x523b80
00523a20: mov r0, sp
00523a24: mov r1, r4
00523a28: bl #0x597180
00523a2c: ldr r3, [r5, #0x88]
00523a30: ldr r6, [r5, #0x8c]
00523a34: ldr sl, [sp]
00523a38: ldr r8, [sp, #4]
00523a3c: cmp r3, r6
00523a40: ldr r7, [sp, #8]
00523a44: beq #0x523a7c
00523a48: str r7, [r3, #0xc]
00523a4c: str sb, [r3]
00523a50: str sl, [r3, #4]
00523a54: str r8, [r3, #8]
00523a58: ldr r3, [r5, #0x88]
00523a5c: add r3, r3, #0x10
00523a60: str r3, [r5, #0x88]
00523a64: mov r0, r4
00523a68: ldr r3, [r4]
00523a6c: mov lr, pc
00523a70: ldr pc, [r3, #0x68]
00523a74: add sp, sp, #0x14
00523a78: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00523a7c: ldr r3, [r5, #0x84]
00523a80: rsb r3, r3, r6
00523a84: asr r3, r3, #4
00523a88: cmp r3, #1
00523a8c: addhs r1, r3, r3
00523a90: addlo r1, r3, #1
00523a94: cmn r1, #0xf0000001
00523a98: bhi #0x523b78
00523a9c: cmp r3, r1
00523aa0: bhi #0x523b78
00523aa4: add r2, sp, #0x10
00523aa8: str r1, [r2, #-4]!
00523aac: add r0, r5, #0x8c
00523ab0: bl #0x522b94
00523ab4: ldr r2, [r5, #0x84]
00523ab8: mov fp, r0
00523abc: rsb r6, r2, r6
00523ac0: asr r6, r6, #4
00523ac4: cmp r6, #0
00523ac8: movle r6, r0
00523acc: ble #0x523b10
00523ad0: add r2, r2, #0x10
00523ad4: add r3, r0, #0x10
00523ad8: mov r1, r6
00523adc: ldr r0, [r2, #-0x10]
00523ae0: subs r1, r1, #1
00523ae4: str r0, [r3, #-0x10]
00523ae8: ldr r0, [r2, #-0xc]
00523aec: str r0, [r3, #-0xc]
00523af0: ldr r0, [r2, #-8]
00523af4: str r0, [r3, #-8]
00523af8: ldr r0, [r2, #-4]
00523afc: add r2, r2, #0x10
00523b00: str r0, [r3, #-4]
00523b04: add r3, r3, #0x10
00523b08: bne #0x523adc
00523b0c: add r6, fp, r6, lsl #4
00523b10: str sb, [r6]
00523b14: str sl, [r6, #4]
00523b18: str r8, [r6, #8]
00523b1c: str r7, [r6, #0xc]
00523b20: ldr r3, [r5, #0x88]
00523b24: ldr r0, [r5, #0x84]
00523b28: add r6, r6, #0x10
00523b2c: ldr r1, [r5, #0x8c]
00523b30: cmp r3, r0
00523b34: subne r2, r3, #0x10
00523b38: rsbne r2, r0, r2
00523b3c: mvnne r2, r2, lsr #4
00523b40: addne r3, r3, r2, lsl #4
00523b44: cmp r3, #0
00523b48: beq #0x523b60
00523b4c: rsb r1, r3, r1
00523b50: bic r1, r1, #0xf
00523b54: cmp r1, #0x80
00523b58: bhi #0x523bfc
00523b5c: bl #0x708f00
00523b60: ldr r3, [sp, #0xc]
00523b64: str fp, [r5, #0x84]
00523b68: str r6, [r5, #0x88]
00523b6c: add fp, fp, r3, lsl #4
00523b70: str fp, [r5, #0x8c]
00523b74: b #0x523a64
00523b78: mvn r1, #0xf0000000
00523b7c: b #0x523aa4
00523b80: ldr r3, [r4]
00523b84: mov r0, r4
00523b88: mov lr, pc
00523b8c: ldr pc, [r3, #0x24]
00523b90: ldr r1, [pc, #0x70]
00523b94: add r1, pc, r1
00523b98: bl #0x30ebd4
00523b9c: cmp r0, #0
00523ba0: movne sb, #1
00523ba4: bne #0x523a20
00523ba8: ldr r3, [r4]
00523bac: mov r0, r4
00523bb0: mov lr, pc
00523bb4: ldr pc, [r3, #0x24]
00523bb8: ldr r1, [pc, #0x4c]
00523bbc: add r1, pc, r1
00523bc0: bl #0x30ebd4
00523bc4: cmp r0, #0
00523bc8: movne sb, #2
00523bcc: bne #0x523a20
00523bd0: ldr r3, [r4]
00523bd4: mov r0, r4
00523bd8: mov lr, pc
00523bdc: ldr pc, [r3, #0x24]
00523be0: ldr r1, [pc, #0x28]
00523be4: add r1, pc, r1
00523be8: bl #0x30ebd4
00523bec: cmp r0, #0
00523bf0: movne sb, #3
00523bf4: moveq sb, #0
00523bf8: b #0x523a20
00523bfc: bl #0x310440
00523c00: b #0x523b60
00523c04: eorseq sb, fp, ip, rrx
00523c08: eorseq r8, fp, ip, ror #29
00523c0c: eorseq r8, fp, ip, asr #29
00523c10: eorseq r8, fp, ip, lsr #29

# 0x523c14 _ZN7PFWorld8LoadRoomEPN6glitch5scene10ISceneNodeEjPKc
00523c14: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00523c18: mov r4, r0
00523c1c: sub sp, sp, #0x2c
00523c20: mov r7, r3
00523c24: mov r6, r1
00523c28: mov r8, r2
00523c2c: bl #0x522844
00523c30: ldr r3, [r4, #4]
00523c34: ldr r0, [pc, #0x540]
00523c38: cmp r3, #1
00523c3c: add r0, pc, r0
00523c40: str r0, [sp, #8]
00523c44: beq #0x523c6c
00523c48: ldr r3, [pc, #0x530]
00523c4c: ldr r3, [r0, r3]
00523c50: ldr r3, [r3]
00523c54: cmp r3, #2
00523c58: moveq r3, #0
00523c5c: streq r3, [r3]
00523c60: beq #0x523c6c
00523c64: cmp r3, #1
00523c68: beq #0x524010
00523c6c: cmp r6, #0
00523c70: beq #0x524048
00523c74: mov r1, #0
00523c78: mov r0, #0x54
00523c7c: bl #0x310570
00523c80: ldr lr, [r4, #0x44]
00523c84: ldr ip, [r4, #0x48]
00523c88: mov r1, r7
00523c8c: mov r3, r4
00523c90: mov r2, r8
00523c94: mov r5, r0
00523c98: str lr, [sp]
00523c9c: str ip, [sp, #4]
00523ca0: bl #0x521540
00523ca4: ldr r7, [r4, #0xc]
00523ca8: ldr r3, [r4, #0x10]
00523cac: cmp r7, r3
00523cb0: beq #0x5240d0
00523cb4: str r5, [r7]
00523cb8: ldr r3, [r4, #0xc]
00523cbc: add r3, r3, #4
00523cc0: str r3, [r4, #0xc]
00523cc4: ldr ip, [pc, #0x4b8]
00523cc8: ldr r0, [sp, #8]
00523ccc: movw r3, #0x6164
00523cd0: str ip, [sp, #0x14]
00523cd4: ldr r2, [r0, ip]
00523cd8: mov sl, #0
00523cdc: mov r1, r6
00523ce0: ldr r0, [r2, #0x10]
00523ce4: movt r3, #0x6d65
00523ce8: add r2, sp, #0x18
00523cec: ldr r0, [r0, #0x1c]
00523cf0: str sl, [sp, #0x18]
00523cf4: str sl, [sp, #0x1c]
00523cf8: str sl, [sp, #0x20]
00523cfc: bl #0x350e5c
00523d00: ldr r6, [sp, #0x18]
00523d04: ldr r7, [sp, #0x1c]
00523d08: cmp r6, r7
00523d0c: beq #0x5240a4
00523d10: ldr r3, [pc, #0x470]
00523d14: ldr r8, [pc, r3]
00523d18: b #0x523d3c
00523d1c: mov r0, sb
00523d20: mov r1, r8
00523d24: bl #0x30ebd4
00523d28: cmp r0, #0
00523d2c: ldrne sl, [r6]
00523d30: add r6, r6, #4
00523d34: cmp r6, r7
00523d38: beq #0x523da8
00523d3c: ldr r3, [r6]
00523d40: mov r0, r3
00523d44: ldr r3, [r3]
00523d48: mov lr, pc
00523d4c: ldr pc, [r3, #0x24]
00523d50: ldrb r3, [r0]
00523d54: mov sb, r0
00523d58: cmp r3, #0
00523d5c: bne #0x523d1c
00523d60: ldr r0, [r6]
00523d64: bl #0x597290
00523d68: cmp r0, #0
00523d6c: beq #0x523d1c
00523d70: ldr r0, [r6]
00523d74: bl #0x597290
00523d78: ldr r3, [r0]
00523d7c: mov lr, pc
00523d80: ldr pc, [r3, #0x24]
00523d84: mov sb, r0
00523d88: mov r0, sb
00523d8c: mov r1, r8
00523d90: bl #0x30ebd4
00523d94: cmp r0, #0
00523d98: ldrne sl, [r6]
00523d9c: add r6, r6, #4
00523da0: cmp r6, r7
00523da4: bne #0x523d3c
00523da8: ldr r7, [sp, #0x18]
00523dac: ldr fp, [sp, #0x1c]
00523db0: cmp r7, fp
00523db4: beq #0x5240a4
00523db8: ldr r3, [pc, #0x3cc]
00523dbc: mov sb, #0
00523dc0: mov r8, r4
00523dc4: add r3, pc, r3
00523dc8: ldr r1, [r3, #8]
00523dcc: ldr r3, [r3, #4]
00523dd0: str r1, [sp, #0xc]
00523dd4: str r3, [sp, #0x10]
00523dd8: b #0x523e58
00523ddc: ldr r1, [sp, #0x10]
00523de0: mov r0, r4
00523de4: bl #0x30ebd4
00523de8: cmp r0, #0
00523dec: mov r2, r4
00523df0: mov r0, r5
00523df4: beq #0x523e28
00523df8: ldr r1, [r7]
00523dfc: bl #0x521eb8
00523e00: cmp sl, #0
00523e04: add sb, sb, #1
00523e08: beq #0x523eb4
00523e0c: ldr ip, [sp, #8]
00523e10: ldr r2, [sp, #0x14]
00523e14: mov r1, sl
00523e18: ldr r3, [ip, r2]
00523e1c: ldr r3, [r3, #0x10]
00523e20: ldr r0, [r3, #0x1c]
00523e24: bl #0x3524a0
00523e28: ldr r1, [sp, #0xc]
00523e2c: mov r0, r4
00523e30: bl #0x30ebd4
00523e34: cmp r0, #0
00523e38: add r7, r7, #4
00523e3c: mov r1, r6
00523e40: mov r2, r4
00523e44: mov r0, r8
00523e48: beq #0x523e50
00523e4c: bl #0x5239e8
00523e50: cmp r7, fp
00523e54: beq #0x523edc
00523e58: ldr r6, [r7]
00523e5c: ldr r3, [r6]
00523e60: mov r0, r6
00523e64: mov lr, pc
00523e68: ldr pc, [r3, #0x24]
00523e6c: ldrb r3, [r0]
00523e70: mov r4, r0
00523e74: cmp r3, #0
00523e78: bne #0x523ddc
00523e7c: ldr r0, [r7]
00523e80: bl #0x597290
00523e84: cmp r0, #0
00523e88: beq #0x523ddc
00523e8c: ldr r0, [r7]
00523e90: bl #0x597290
00523e94: ldr r3, [r0]
00523e98: mov lr, pc
00523e9c: ldr pc, [r3, #0x24]
00523ea0: mov r4, r0
00523ea4: mov r0, r6
00523ea8: bl #0x597290
00523eac: mov r6, r0
00523eb0: b #0x523ddc
00523eb4: ldr r1, [sp, #8]
00523eb8: ldr r0, [sp, #0x14]
00523ebc: ldr r3, [r5, #0x34]
00523ec0: ldr r2, [r1, r0]
00523ec4: ldr r3, [r3, #-4]
00523ec8: ldr r2, [r2, #0x10]
00523ecc: ldr r1, [r3, #0x40]
00523ed0: ldr r0, [r2, #0x1c]
00523ed4: bl #0x3524a0
00523ed8: b #0x523e28
00523edc: cmp sb, #0
00523ee0: mov r4, r8
00523ee4: beq #0x5240a4
00523ee8: ldr r2, [r8, #0xc]
00523eec: ldr r3, [r8, #8]
00523ef0: rsb r3, r3, r2
00523ef4: asr r3, r3, #2
00523ef8: cmp r3, #1
00523efc: beq #0x523fdc
00523f00: ldr r6, [r5, #0x3c]
00523f04: ldr r7, [r8, #0x14]
00523f08: mov r0, r6
00523f0c: mov r1, r7
00523f10: bl #0x30e70c
00523f14: cmp r0, #0
00523f18: moveq r6, r7
00523f1c: str r6, [r8, #0x14]
00523f20: ldr r6, [r5, #0x40]
00523f24: ldr r7, [r8, #0x18]
00523f28: mov r0, r6
00523f2c: mov r1, r7
00523f30: bl #0x30e70c
00523f34: cmp r0, #0
00523f38: moveq r6, r7
00523f3c: str r6, [r8, #0x18]
00523f40: ldr r6, [r5, #0x44]
00523f44: ldr r7, [r8, #0x1c]
00523f48: mov r0, r6
00523f4c: mov r1, r7
00523f50: bl #0x30e70c
00523f54: cmp r0, #0
00523f58: moveq r6, r7
00523f5c: str r6, [r8, #0x1c]
00523f60: ldr r6, [r5, #0x48]
00523f64: ldr r7, [r8, #0x20]
00523f68: mov r1, r6
00523f6c: mov r0, r7
00523f70: bl #0x30e70c
00523f74: cmp r0, #0
00523f78: moveq r6, r7
00523f7c: str r6, [r8, #0x20]
00523f80: ldr r6, [r5, #0x4c]
00523f84: ldr r7, [r8, #0x24]
00523f88: mov r1, r6
00523f8c: mov r0, r7
00523f90: bl #0x30e70c
00523f94: cmp r0, #0
00523f98: moveq r6, r7
00523f9c: str r6, [r8, #0x24]
00523fa0: ldr r7, [r8, #0x28]
00523fa4: ldr r6, [r5, #0x50]
00523fa8: mov r0, r7
00523fac: mov r1, r6
00523fb0: bl #0x30e70c
00523fb4: cmp r0, #0
00523fb8: moveq r6, r7
00523fbc: str r6, [r8, #0x28]
00523fc0: ldr r0, [sp, #0x18]
00523fc4: cmp r0, #0
00523fc8: beq #0x523fd0
00523fcc: bl #0x310450
00523fd0: mov r0, r5
00523fd4: add sp, sp, #0x2c
00523fd8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00523fdc: ldr r3, [r5, #0x3c]
00523fe0: str r3, [r8, #0x14]
00523fe4: ldr r3, [r5, #0x40]
00523fe8: str r3, [r8, #0x18]
00523fec: ldr r3, [r5, #0x44]
00523ff0: str r3, [r8, #0x1c]
00523ff4: ldr r3, [r5, #0x48]
00523ff8: str r3, [r8, #0x20]
00523ffc: ldr r3, [r5, #0x4c]
00524000: str r3, [r8, #0x24]
00524004: ldr r3, [r5, #0x50]
00524008: str r3, [r8, #0x28]
0052400c: b #0x523fc0
00524010: ldr r1, [sp, #8]
00524014: ldr r0, [pc, #0x174]
00524018: ldr r2, [pc, #0x174]
0052401c: ldr r3, [pc, #0x174]
00524020: ldr r0, [r1, r0]
00524024: ldr r1, [pc, #0x170]
00524028: mov ip, #0x5c
0052402c: add r2, pc, r2
00524030: add r1, pc, r1
00524034: add r3, pc, r3
00524038: add r0, r0, #0xa8
0052403c: str ip, [sp]
00524040: bl #0x30e004
00524044: b #0x523c6c
00524048: ldr r2, [sp, #8]
0052404c: ldr r3, [pc, #0x12c]
00524050: ldr r3, [r2, r3]
00524054: ldr r3, [r3]
00524058: cmp r3, #2
0052405c: streq r6, [r6]
00524060: beq #0x523c74
00524064: cmp r3, #1
00524068: bne #0x523c74
0052406c: ldr r3, [sp, #8]
00524070: ldr r0, [pc, #0x118]
00524074: ldr r1, [pc, #0x124]
00524078: ldr r2, [pc, #0x124]
0052407c: ldr r0, [r3, r0]
00524080: ldr r3, [pc, #0x120]
00524084: mov ip, #0x62
00524088: add r1, pc, r1
0052408c: add r2, pc, r2
00524090: add r3, pc, r3
00524094: add r0, r0, #0xa8
00524098: str ip, [sp]
0052409c: bl #0x30e004
005240a0: b #0x523c74
005240a4: ldr r3, [r4, #0xc]
005240a8: cmp r5, #0
005240ac: sub r3, r3, #4
005240b0: str r3, [r4, #0xc]
005240b4: beq #0x523fc0
005240b8: mov r0, r5
005240bc: ldr r3, [r5]
005240c0: mov lr, pc
005240c4: ldr pc, [r3, #4]
005240c8: mov r5, #0
005240cc: b #0x523fc0
005240d0: ldr r3, [r4, #8]
005240d4: rsb r3, r3, r7
005240d8: asr r3, r3, #2
005240dc: cmp r3, #1
005240e0: addhs r1, r3, r3
005240e4: addlo r1, r3, #1
005240e8: cmn r1, #0xc0000001
005240ec: bls #0x524158
005240f0: mvn r1, #0xc0000000
005240f4: add r2, sp, #0x28
005240f8: str r1, [r2, #-4]!
005240fc: add r0, r4, #0x10
00524100: bl #0x522c04
00524104: ldr r1, [r4, #8]
00524108: mov r8, r0
0052410c: subs r7, r7, r1
00524110: moveq r7, r0
00524114: bne #0x52416c
00524118: str r5, [r7], #4
0052411c: ldr r0, [r4, #8]
00524120: ldr r3, [r4, #0x10]
00524124: cmp r0, #0
00524128: beq #0x524140
0052412c: rsb r3, r0, r3
00524130: bic r1, r3, #3
00524134: cmp r1, #0x80
00524138: bhi #0x524164
0052413c: bl #0x708f00
00524140: ldr r3, [sp, #0x24]
00524144: str r8, [r4, #8]
00524148: str r7, [r4, #0xc]
0052414c: add r8, r8, r3, lsl #2
00524150: str r8, [r4, #0x10]
00524154: b #0x523cc4
00524158: cmp r3, r1
0052415c: bls #0x5240f4
00524160: b #0x5240f0
00524164: bl #0x310440
00524168: b #0x524140
0052416c: mov r2, r7
00524170: bl #0x30df38
00524174: add r7, r0, r7
00524178: b #0x524118
0052417c: subeq r0, r7, r4, asr lr
00524180: andeq r3, r0, r0, asr #19
00524184: strdeq r3, r4, [r0], -r4
00524188: subeq r2, r3, r8, lsl lr
0052418c: subeq r2, r3, r8, ror #26
00524190: andeq r1, r0, r0, asr #19
00524194: eorseq r8, fp, ip, ror #20
00524198: ldrsbteq r8, [fp], -ip
0052419c: eorseq sl, sb, r8, lsr #7
005241a0: eorseq sl, sb, r0, asr r3
005241a4: eorseq r8, fp, ip, asr sb
005241a8: eorseq r8, fp, r0, lsl #19

# 0x5839d8 _ZN6glitch5scene15CEmptySceneNodeC1Ei
005839d8: push {r4, r5, r6, r7, lr}
005839dc: ldr r6, [pc, #0xdc]
005839e0: ldr r3, [pc, #0xdc]
005839e4: ldr r2, [pc, #0xdc]
005839e8: add r6, pc, r6
005839ec: ldr r3, [r6, r3]
005839f0: ldr r2, [r6, r2]
005839f4: mov lr, #1
005839f8: ldr ip, [r3, #0x18]
005839fc: add r2, r2, #8
00583a00: str r2, [r0, #0x148]
00583a04: str ip, [r0]
00583a08: str lr, [r0, #0x14c]
00583a0c: ldr lr, [ip, #-0xc]
00583a10: ldr r7, [r3, #0x1c]
00583a14: sub sp, sp, #0x34
00583a18: mov ip, #0
00583a1c: str r7, [r0, lr]
00583a20: add lr, sp, #8
00583a24: mov r5, #0x3f800000
00583a28: mov r2, r1
00583a2c: str lr, [sp]
00583a30: add r1, r3, #4
00583a34: add lr, sp, #0x18
00583a38: add r3, sp, #0x24
00583a3c: mov r4, r0
00583a40: str ip, [sp, #0x10]
00583a44: str lr, [sp, #4]
00583a48: str ip, [sp, #0x24]
00583a4c: str ip, [sp, #0x28]
00583a50: str ip, [sp, #0x2c]
00583a54: str ip, [sp, #8]
00583a58: str ip, [sp, #0xc]
00583a5c: str r5, [sp, #0x14]
00583a60: str r5, [sp, #0x18]
00583a64: str r5, [sp, #0x1c]
00583a68: str r5, [sp, #0x20]
00583a6c: bl #0x5990c0
00583a70: ldr r3, [pc, #0x54]
00583a74: mov r2, #0xbf000000
00583a78: add r2, r2, #0x800000
00583a7c: ldr r3, [r6, r3]
00583a80: mov r0, r4
00583a84: str r5, [r4, #0x144]
00583a88: add r1, r3, #0x120
00583a8c: add r3, r3, #0x1c
00583a90: str r1, [r4, #0x148]
00583a94: str r2, [r4, #0x138]
00583a98: str r2, [r4, #0x130]
00583a9c: str r3, [r4]
00583aa0: str r2, [r4, #0x134]
00583aa4: str r5, [r4, #0x13c]
00583aa8: str r5, [r4, #0x140]
00583aac: mov r1, #0
00583ab0: bl #0x59719c
00583ab4: mov r0, r4
00583ab8: add sp, sp, #0x34
00583abc: pop {r4, r5, r6, r7, pc}
00583ac0: subeq r1, r1, r8, lsr #1
00583ac4: andeq r4, r0, r8, lsl r1
00583ac8: andeq r2, r0, r4, asr #22
00583acc: andeq r4, r0, r8, ror #7

# 0x585118 _ZN6glitch5scene14CMeshSceneNodeC1ERKN5boost13intrusive_ptrINS0_5IMeshEEEiRKNS_4core8vector3dIfEERKNS8_10quaternionESC_
00585118: push {r4, r5, r6, r7, lr}
0058511c: ldr r6, [pc, #0xe8]
00585120: ldr lr, [pc, #0xe8]
00585124: ldr ip, [pc, #0xe8]
00585128: add r6, pc, r6
0058512c: ldr r5, [r6, lr]
00585130: ldr ip, [r6, ip]
00585134: mov r7, #1
00585138: ldr lr, [r5, #0x24]
0058513c: add ip, ip, #8
00585140: str r7, [r0, #0x13c]
00585144: str lr, [r0]
00585148: str ip, [r0, #0x138]
0058514c: ldr ip, [lr, #-0xc]
00585150: ldr lr, [r5, #0x28]
00585154: sub sp, sp, #0xc
00585158: mov r7, r1
0058515c: str lr, [r0, ip]
00585160: ldr ip, [sp, #0x20]
00585164: add r1, r5, #8
00585168: mov r4, r0
0058516c: str ip, [sp]
00585170: ldr ip, [sp, #0x24]
00585174: str ip, [sp, #4]
00585178: bl #0x5990c0
0058517c: ldr r2, [r5, #4]
00585180: ldr r1, [r5, #0x14]
00585184: ldr r3, [pc, #0x8c]
00585188: str r2, [r4]
0058518c: ldr r2, [r2, #-0x1c]
00585190: ldr r3, [r6, r3]
00585194: ldr ip, [r5, #0x18]
00585198: str r1, [r4, r2]
0058519c: ldr r0, [r4]
005851a0: mov r2, #0
005851a4: add r1, r3, #0x128
005851a8: ldr r0, [r0, #-0xc]
005851ac: add r3, r3, #0x1c
005851b0: str ip, [r4, r0]
005851b4: str r2, [r4, #0x130]
005851b8: str r3, [r4]
005851bc: str r1, [r4, #0x138]
005851c0: str r2, [r4, #0x134]
005851c4: ldr r3, [r7]
005851c8: cmp r3, r2
005851cc: streq r3, [r4, #0x130]
005851d0: beq #0x5851f4
005851d4: ldr r2, [r3, #4]
005851d8: add r2, r2, #1
005851dc: str r2, [r3, #4]
005851e0: ldr r0, [r4, #0x130]
005851e4: str r3, [r4, #0x130]
005851e8: cmp r0, #0
005851ec: beq #0x5851f4
005851f0: bl #0x31d584
005851f4: mov r0, r4
005851f8: mov r1, #2
005851fc: bl #0x59719c
00585200: mov r0, r4
00585204: add sp, sp, #0xc
00585208: pop {r4, r5, r6, r7, pc}
0058520c: subeq pc, r0, r8, ror #18
00585210: andeq r0, r0, r0, lsr sl
00585214: andeq r2, r0, r4, asr #22
00585218: andeq r3, r0, r8, lsr #23

# 0x596ec4 _ZN6glitch5scene10ISceneNode10setVisibleEb
00596ec4: push {r4, r5, r6, lr}
00596ec8: ldrb r3, [r0, #0x120]
00596ecc: mov r5, r0
00596ed0: cmp r3, r1
00596ed4: beq #0x596f40
00596ed8: ldr r3, [r0, #0x11c]
00596edc: cmp r1, #0
00596ee0: strb r1, [r0, #0x120]
00596ee4: and r2, r3, #1
00596ee8: bne #0x596f44
00596eec: bic r3, r3, #1
00596ef0: str r3, [r5, #0x11c]
00596ef4: and r3, r3, #1
00596ef8: cmp r2, r3
00596efc: beq #0x596f40
00596f00: mov r6, r5
00596f04: ldr r4, [r6, #0xf4]!
00596f08: b #0x596f34
00596f0c: ldr r1, [r5, #0x11c]
00596f10: cmp r4, #0
00596f14: moveq r3, r4
00596f18: subne r3, r4, #4
00596f1c: mov r0, r3
00596f20: and r1, r1, #1
00596f24: ldr r3, [r3]
00596f28: mov lr, pc
00596f2c: ldr pc, [r3, #0xec]
00596f30: ldr r4, [r4]
00596f34: cmp r6, r4
00596f38: bne #0x596f0c
00596f3c: pop {r4, r5, r6, pc}
00596f40: pop {r4, r5, r6, pc}
00596f44: ldrb r1, [r0, #0x121]
00596f48: cmp r1, #0
00596f4c: beq #0x596eec
00596f50: orr r3, r3, #1
00596f54: str r3, [r0, #0x11c]
00596f58: b #0x596ef4

# 0x597004 _ZN6glitch5scene10ISceneNode11removeChildEPS1_
00597004: ldr r3, [r1, #0xec]
00597008: push {r4, lr}
0059700c: cmp r3, r0
00597010: beq #0x59701c
00597014: mov r0, #0
00597018: pop {r4, pc}
0059701c: ldr r2, [r1, #4]
00597020: add ip, r1, #4
00597024: cmp r2, #0
00597028: ldrne r0, [r1, #8]
0059702c: strne r2, [r0]
00597030: strne r0, [r2, #4]
00597034: ldr lr, [r3, #0xf0]
00597038: mov r2, #0
0059703c: sub r0, ip, #4
00597040: sub lr, lr, #1
00597044: str lr, [r3, #0xf0]
00597048: str r2, [r1, #8]
0059704c: str r2, [r1, #4]
00597050: str r2, [r0, #0xec]
00597054: ldr r3, [ip, #-4]
00597058: ldr r3, [r3, #-0xc]
0059705c: add r0, r0, r3
00597060: bl #0x31d584
00597064: mov r0, #1
00597068: pop {r4, pc}

# 0x5970f4 _ZN6glitch5scene10ISceneNode11setRotationERKNS_4core10quaternionE
005970f4: ldr r3, [r1]
005970f8: ldr r2, [r0, #0x11c]
005970fc: str r3, [r0, #0xb8]
00597100: ldr r3, [r1, #4]
00597104: orr r2, r2, #4
00597108: str r3, [r0, #0xbc]
0059710c: ldr r3, [r1, #8]
00597110: str r3, [r0, #0xc0]
00597114: ldr r3, [r1, #0xc]
00597118: str r2, [r0, #0x11c]
0059711c: str r3, [r0, #0xc4]
00597120: bx lr

# 0x59712c _ZN6glitch5scene10ISceneNode11setPositionERKNS_4core8vector3dIfEE
0059712c: ldr r3, [r1]
00597130: ldr r2, [r0, #0x11c]
00597134: str r3, [r0, #0xac]
00597138: ldr r3, [r1, #4]
0059713c: orr r2, r2, #8
00597140: str r3, [r0, #0xb0]
00597144: ldr r3, [r1, #8]
00597148: str r2, [r0, #0x11c]
0059714c: str r3, [r0, #0xb4]
00597150: bx lr

# 0x597180 _ZNK6glitch5scene10ISceneNode19getAbsolutePositionEv
00597180: ldr ip, [r1, #0x54]
00597184: ldr r2, [r1, #0x58]
00597188: ldr r1, [r1, #0x5c]
0059718c: str ip, [r0]
00597190: str r2, [r0, #4]
00597194: str r1, [r0, #8]
00597198: bx lr

# 0x5971e0 _ZN6glitch5scene10ISceneNode9setParentEPS1_
005971e0: push {r4, r5, r6, lr}
005971e4: ldr r3, [r0]
005971e8: mov r4, r0
005971ec: mov r5, r1
005971f0: ldr r3, [r3, #-0xc]
005971f4: add r3, r0, r3
005971f8: ldr r2, [r3, #4]
005971fc: add r2, r2, #1
00597200: str r2, [r3, #4]
00597204: ldr r3, [r0]
00597208: mov lr, pc
0059720c: ldr pc, [r3, #0x68]
00597210: ldr r3, [r4, #0x11c]
00597214: cmp r5, #0
00597218: str r5, [r4, #0xec]
0059721c: orr r3, r3, #0x40
00597220: str r3, [r4, #0x11c]
00597224: beq #0x597240
00597228: ldr r1, [r5, #0x110]
0059722c: ldr r3, [r4, #0x110]
00597230: cmp r3, r1
00597234: beq #0x597240
00597238: mov r0, r4
0059723c: bl #0x588e30
00597240: ldr r3, [r4]
00597244: ldr r0, [r3, #-0xc]
00597248: add r0, r4, r0
0059724c: pop {r4, r5, r6, lr}
00597250: b #0x31d584

# 0x597c60 _ZN6glitch5scene10ISceneNode22updateAbsolutePositionEb
00597c60: push {r4, r5, r6, lr}
00597c64: ldr r3, [r0, #0xec]
00597c68: mov r4, r0
00597c6c: mov r5, r1
00597c70: cmp r3, #0
00597c74: beq #0x597d18
00597c78: ldr r2, [r3, #0x11c]
00597c7c: tst r2, #0x20
00597c80: bne #0x597cd0
00597c84: ldr r2, [r0, #0x11c]
00597c88: tst r2, #0x5e
00597c8c: bne #0x597cd0
00597c90: cmp r5, #0
00597c94: ldrne r5, [r4, #0xf4]!
00597c98: bne #0x597cc4
00597c9c: b #0x597ccc
00597ca0: cmp r5, #0
00597ca4: moveq r3, r5
00597ca8: subne r3, r5, #4
00597cac: mov r0, r3
00597cb0: mov r1, #1
00597cb4: ldr r3, [r3]
00597cb8: mov lr, pc
00597cbc: ldr pc, [r3, #0xb8]
00597cc0: ldr r5, [r5]
00597cc4: cmp r4, r5
00597cc8: bne #0x597ca0
00597ccc: pop {r4, r5, r6, pc}
00597cd0: mov r0, r3
00597cd4: ldr r3, [r3]
00597cd8: mov lr, pc
00597cdc: ldr pc, [r3, #0x38]
00597ce0: ldr r3, [r4]
00597ce4: mov r6, r0
00597ce8: mov r0, r4
00597cec: mov lr, pc
00597cf0: ldr pc, [r3, #0x40]
00597cf4: add r2, r4, #0x24
00597cf8: mov r1, r0
00597cfc: mov r0, r6
00597d00: bl #0x597884
00597d04: ldr r3, [r4, #0x11c]
00597d08: orr r3, r3, #0x120
00597d0c: bic r3, r3, #0x50
00597d10: str r3, [r4, #0x11c]
00597d14: b #0x597c90
00597d18: ldr r3, [r0, #0x11c]
00597d1c: tst r3, #0x5e
00597d20: beq #0x597c90
00597d24: mov r6, r0
00597d28: ldr r3, [r6], #0x24
00597d2c: mov lr, pc
00597d30: ldr pc, [r3, #0x40]
00597d34: mov r2, #0x41
00597d38: mov r1, r0
00597d3c: mov r0, r6
00597d40: bl #0x30e868
00597d44: ldr r3, [r4, #0x11c]
00597d48: orr r3, r3, #0x120
00597d4c: bic r3, r3, #0x50
00597d50: str r3, [r4, #0x11c]
00597d54: b #0x597c90

# 0x598864 _ZN6glitch5scene10ISceneNode8addChildEPS1_
00598864: cmp r1, r0
00598868: cmpne r1, #0
0059886c: push {r4, r5, r6, lr}
00598870: mov r5, r0
00598874: mov r4, r1
00598878: bne #0x598880
0059887c: pop {r4, r5, r6, pc}
00598880: ldr r3, [r1]
00598884: mov r0, r1
00598888: ldr r3, [r3, #-0xc]
0059888c: add r3, r1, r3
00598890: ldr r2, [r3, #4]
00598894: add r2, r2, #1
00598898: str r2, [r3, #4]
0059889c: ldr r3, [r1]
005988a0: mov lr, pc
005988a4: ldr pc, [r3, #0x68]
005988a8: ldr r2, [r5, #0xf8]
005988ac: add r3, r4, #4
005988b0: add r1, r5, #0xf4
005988b4: str r2, [r4, #8]
005988b8: str r3, [r2]
005988bc: str r3, [r5, #0xf8]
005988c0: str r1, [r4, #4]
005988c4: ldr r3, [r5, #0xf0]
005988c8: mov r0, r4
005988cc: mov r1, r5
005988d0: add r3, r3, #1
005988d4: str r3, [r5, #0xf0]
005988d8: bl #0x5971e0
005988dc: ldr r0, [r5, #0x110]
005988e0: cmp r0, #0
005988e4: beq #0x5988ec
005988e8: bl #0x5890b4
005988ec: ldr r1, [r5, #0x11c]
005988f0: mov r0, r4
005988f4: ldr r3, [r4]
005988f8: and r1, r1, #1
005988fc: mov lr, pc
00598900: ldr pc, [r3, #0xec]
00598904: pop {r4, r5, r6, pc}

# 0x598908 _ZNK6glitch5scene10ISceneNode25getRelativeTransformationEv
00598908: push {r4, r5, r6, lr}
0059890c: ldr r3, [r0, #0x11c]
00598910: sub sp, sp, #0x48
00598914: mov r4, r0
00598918: tst r3, #0xe
0059891c: addeq r5, r0, #0x68
00598920: beq #0x598958
00598924: ands r2, r3, #6
00598928: bne #0x598964
0059892c: ldr ip, [r0, #0xac]
00598930: ldr r1, [r4, #0xb4]
00598934: ldr r0, [r0, #0xb0]
00598938: add r5, r4, #0x68
0059893c: strb r2, [r4, #0xa8]
00598940: str ip, [r4, #0x98]
00598944: str r0, [r4, #0x9c]
00598948: str r1, [r4, #0xa0]
0059894c: bic r3, r3, #0xe
00598950: orr r3, r3, #0x10
00598954: str r3, [r4, #0x11c]
00598958: mov r0, r5
0059895c: add sp, sp, #0x48
00598960: pop {r4, r5, r6, pc}
00598964: add r6, sp, #4
00598968: mov r3, #0
0059896c: add r5, r0, #0x68
00598970: mov r1, r6
00598974: add r0, r0, #0xb8
00598978: strb r3, [sp, #0x44]
0059897c: bl #0x5602d0
00598980: mov r1, r6
00598984: mov r2, #0x41
00598988: mov r0, r5
0059898c: bl #0x30e868
00598990: ldr r0, [r4, #0xc8]
00598994: mov r1, #0x3f800000
00598998: bl #0x30df8c
0059899c: cmp r0, #0
005989a0: beq #0x5989b8
005989a4: ldr r0, [r4, #0xcc]
005989a8: mov r1, #0x3f800000
005989ac: bl #0x30df8c
005989b0: cmp r0, #0
005989b4: bne #0x5989ec
005989b8: mov r0, r5
005989bc: add r1, r4, #0xc8
005989c0: bl #0x597788
005989c4: ldr r3, [r4, #0xb4]
005989c8: ldr r1, [r4, #0xac]
005989cc: ldr r2, [r4, #0xb0]
005989d0: mov r0, #0
005989d4: str r3, [r4, #0xa0]
005989d8: strb r0, [r4, #0xa8]
005989dc: str r1, [r4, #0x98]
005989e0: str r2, [r4, #0x9c]
005989e4: ldr r3, [r4, #0x11c]
005989e8: b #0x59894c
005989ec: ldr r0, [r4, #0xd0]
005989f0: mov r1, #0x3f800000
005989f4: bl #0x30df8c
005989f8: cmp r0, #0
005989fc: bne #0x5989c4
00598a00: b #0x5989b8

# 0x5990c0 _ZN6glitch5scene10ISceneNodeC2EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
005990c0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005990c4: mov r5, #0
005990c8: sub sp, sp, #0x14
005990cc: mov r8, r1
005990d0: str r5, [r0, #4]
005990d4: str r5, [r0, #8]
005990d8: mov r4, r0
005990dc: mov sl, r3
005990e0: ldr r7, [sp, #0x3c]
005990e4: str r2, [sp, #0xc]
005990e8: bl #0x6a118c
005990ec: ldr r2, [r8]
005990f0: add r3, r4, #0xc
005990f4: mov r0, r3
005990f8: str r2, [r4]
005990fc: ldr r1, [r8, #4]
00599100: ldr r2, [r2, #-0x1c]
00599104: mov sb, #0x40
00599108: mov r6, #0x3f800000
0059910c: str r1, [r4, r2]
00599110: ldr r2, [r4]
00599114: ldr r1, [r8, #8]
00599118: mov r8, #1
0059911c: ldr r2, [r2, #-0xc]
00599120: str r1, [r4, r2]
00599124: str r3, [r4, #0x1c]
00599128: str r3, [r4, #0x20]
0059912c: bl #0x598ee0
00599130: ldr r3, [r4, #0x1c]
00599134: mov r1, r5
00599138: mov r2, sb
0059913c: strb r5, [r3]
00599140: add r0, r4, #0x24
00599144: strb r5, [r4, #0x64]
00599148: bl #0x30e460
0059914c: mov r2, sb
00599150: mov r1, r5
00599154: str r6, [r4, #0x24]
00599158: str r6, [r4, #0x38]
0059915c: str r6, [r4, #0x4c]
00599160: str r6, [r4, #0x60]
00599164: strb r8, [r4, #0x64]
00599168: strb r5, [r4, #0xa8]
0059916c: add r0, r4, #0x68
00599170: bl #0x30e460
00599174: str r6, [r4, #0x68]
00599178: str r6, [r4, #0x7c]
0059917c: str r6, [r4, #0x90]
00599180: str r6, [r4, #0xa4]
00599184: strb r8, [r4, #0xa8]
00599188: ldr r3, [sl]
0059918c: add r2, r4, #0xb8
00599190: str r2, [sp, #4]
00599194: str r3, [r4, #0xac]
00599198: ldr r3, [sl, #4]
0059919c: mov ip, #0xbf000000
005991a0: add ip, ip, #0x800000
005991a4: str r3, [r4, #0xb0]
005991a8: ldr r3, [sl, #8]
005991ac: add lr, r4, #0xfc
005991b0: add sb, r4, #0xf4
005991b4: str r3, [r4, #0xb4]
005991b8: ldr fp, [sp, #0x38]
005991bc: add sl, r4, #0x104
005991c0: ldm fp, {r0, r1, r2, r3}
005991c4: ldr fp, [sp, #4]
005991c8: stm fp, {r0, r1, r2, r3}
005991cc: ldr r3, [r7]
005991d0: mov r0, r4
005991d4: mov r1, r5
005991d8: str r3, [r4, #0xc8]
005991dc: ldr r3, [r7, #4]
005991e0: str r3, [r4, #0xcc]
005991e4: ldr r3, [r7, #8]
005991e8: str ip, [r4, #0xdc]
005991ec: str ip, [r4, #0xd4]
005991f0: str r3, [r4, #0xd0]
005991f4: str ip, [r4, #0xd8]
005991f8: str r6, [r4, #0xe8]
005991fc: str r6, [r4, #0xe0]
00599200: str r6, [r4, #0xe4]
00599204: str r5, [r4, #0xec]
00599208: str r5, [r4, #0xf0]
0059920c: str sb, [r4, #0xf4]
00599210: str sb, [r4, #0xf8]
00599214: str lr, [r4, #0x100]
00599218: str sl, [r4, #0x108]
0059921c: ldr r2, [sp, #0xc]
00599220: movw r3, #0x60f
00599224: str r3, [r4, #0x11c]
00599228: mov r3, #0
0059922c: str r2, [r4, #0x10c]
00599230: strb r8, [r4, #0x121]
00599234: str r3, [r4, #0x128]
00599238: str lr, [r4, #0xfc]
0059923c: str sl, [r4, #0x104]
00599240: str r5, [r4, #0x110]
00599244: str r5, [r4, #0x114]
00599248: str r5, [r4, #0x118]
0059924c: strb r8, [r4, #0x120]
00599250: str r5, [r4, #0x124]
00599254: str r5, [r4, #0x12c]
00599258: bl #0x597c60
0059925c: mov r0, r4
00599260: add sp, sp, #0x14
00599264: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# 0x599268 _ZN6glitch5scene10ISceneNodeC1EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
00599268: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059926c: ldr r6, [pc, #0x1c0]
00599270: ldr lr, [pc, #0x1c0]
00599274: ldr ip, [pc, #0x1c0]
00599278: add r6, pc, r6
0059927c: ldr lr, [r6, lr]
00599280: ldr ip, [r6, ip]
00599284: mov sl, #1
00599288: ldr r5, [lr, #0xc]
0059928c: add ip, ip, #8
00599290: str sl, [r0, #0x134]
00599294: str r5, [r0]
00599298: str ip, [r0, #0x130]
0059929c: ldr ip, [r5, #-0xc]
005992a0: ldr lr, [lr, #0x10]
005992a4: mov r5, #0
005992a8: sub sp, sp, #0xc
005992ac: str lr, [r0, ip]
005992b0: str r5, [r0, #4]
005992b4: str r5, [r0, #8]
005992b8: mov r4, r0
005992bc: mov r7, r2
005992c0: str r3, [sp]
005992c4: ldr r8, [sp, #0x30]
005992c8: str r1, [sp, #4]
005992cc: bl #0x6a118c
005992d0: ldr r2, [pc, #0x168]
005992d4: add r1, r4, #0xc
005992d8: mov r0, r1
005992dc: ldr r2, [r6, r2]
005992e0: str r1, [r4, #0x1c]
005992e4: str r1, [r4, #0x20]
005992e8: add r1, r2, #0x120
005992ec: add r2, r2, #0x1c
005992f0: str r2, [r4]
005992f4: str r1, [r4, #0x130]
005992f8: bl #0x598ee0
005992fc: ldr r2, [r4, #0x1c]
00599300: mov sb, #0x40
00599304: mov r6, #0x3f800000
00599308: strb r5, [r2]
0059930c: mov r1, r5
00599310: mov r2, sb
00599314: strb r5, [r4, #0x64]
00599318: add r0, r4, #0x24
0059931c: bl #0x30e460
00599320: mov r2, sb
00599324: mov r1, r5
00599328: str r6, [r4, #0x24]
0059932c: str r6, [r4, #0x38]
00599330: str r6, [r4, #0x4c]
00599334: str r6, [r4, #0x60]
00599338: strb sl, [r4, #0x64]
0059933c: strb r5, [r4, #0xa8]
00599340: add r0, r4, #0x68
00599344: bl #0x30e460
00599348: str r6, [r4, #0x68]
0059934c: str r6, [r4, #0x7c]
00599350: str r6, [r4, #0x90]
00599354: str r6, [r4, #0xa4]
00599358: strb sl, [r4, #0xa8]
0059935c: ldr r2, [r7]
00599360: add fp, r4, #0xb8
00599364: mov ip, #0xbf000000
00599368: str r2, [r4, #0xac]
0059936c: ldr r2, [r7, #4]
00599370: add ip, ip, #0x800000
00599374: add lr, r4, #0xfc
00599378: str r2, [r4, #0xb0]
0059937c: ldr r2, [r7, #8]
00599380: add sb, r4, #0xf4
00599384: add r7, r4, #0x104
00599388: str r2, [r4, #0xb4]
0059938c: ldr r3, [sp]
00599390: ldm r3, {r0, r1, r2, r3}
00599394: stm fp, {r0, r1, r2, r3}
00599398: ldr r3, [r8]
0059939c: mov r0, r4
005993a0: mov r1, r5
005993a4: str r3, [r4, #0xc8]
005993a8: ldr r3, [r8, #4]
005993ac: str r3, [r4, #0xcc]
005993b0: ldr r3, [r8, #8]
005993b4: str ip, [r4, #0xdc]
005993b8: str ip, [r4, #0xd4]
005993bc: str r3, [r4, #0xd0]
005993c0: str ip, [r4, #0xd8]
005993c4: str r6, [r4, #0xe8]
005993c8: str r6, [r4, #0xe0]
005993cc: str r6, [r4, #0xe4]
005993d0: str r5, [r4, #0xec]
005993d4: str r5, [r4, #0xf0]
005993d8: str sb, [r4, #0xf4]
005993dc: str sb, [r4, #0xf8]
005993e0: str lr, [r4, #0x100]
005993e4: str r7, [r4, #0x108]
005993e8: ldr r3, [sp, #4]
005993ec: strb sl, [r4, #0x121]
005993f0: str lr, [r4, #0xfc]
005993f4: str r3, [r4, #0x10c]
005993f8: movw r3, #0x60f
005993fc: str r3, [r4, #0x11c]
00599400: mov r3, #0
00599404: str r3, [r4, #0x128]
00599408: str r7, [r4, #0x104]
0059940c: str r5, [r4, #0x110]
00599410: str r5, [r4, #0x114]
00599414: str r5, [r4, #0x118]
00599418: strb sl, [r4, #0x120]
0059941c: str r5, [r4, #0x124]
00599420: str r5, [r4, #0x12c]
00599424: bl #0x597c60
00599428: mov r0, r4
0059942c: add sp, sp, #0xc
00599430: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00599434: eorseq fp, pc, r8, lsl r8
00599438: andeq r4, r0, r4, lsr #23
0059943c: andeq r2, r0, r4, asr #22
00599440: andeq r2, r0, r0, lsl #3

# 0x60e54c _ZNK6glitch7collada16CColladaDatabase14getVisualSceneEi
0060e54c: ldr r3, [r0]
0060e550: ldr r3, [r3, #0x24]
0060e554: ldr r3, [r3, #0x20]
0060e558: ldr r2, [r3, #0x98]
0060e55c: cmp r2, #0
0060e560: ldrgt r0, [r3, #0x9c]
0060e564: movle r0, #0
0060e568: addgt r0, r0, r1, lsl #4
0060e56c: bx lr

# 0x61b2f4 _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS0_5SNodeEPNS0_14CRootSceneNodeE
0061b2f4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061b2f8: subs r4, r2, #0
0061b2fc: sub sp, sp, #0x44
0061b300: mov r8, r0
0061b304: mov fp, r1
0061b308: mov sl, r3
0061b30c: moveq r6, r4
0061b310: beq #0x61b55c
0061b314: ldr r3, [r4, #0x4c]
0061b318: cmp r3, #0
0061b31c: beq #0x61b874
0061b320: ldr r3, [r0, #4]
0061b324: mov r1, r0
0061b328: mov r0, r3
0061b32c: ldr r3, [r3]
0061b330: mov lr, pc
0061b334: ldr pc, [r3, #0x40]
0061b338: mov r6, r0
0061b33c: ldr r1, [r4, #0x40]
0061b340: cmp r1, #0
0061b344: ble #0x61b434
0061b348: add r3, sp, #0x38
0061b34c: add ip, sp, #0x3c
0061b350: mov r5, #0
0061b354: str r3, [sp, #8]
0061b358: str ip, [sp, #0xc]
0061b35c: mov r7, r6
0061b360: ldr r2, [r4, #0x44]
0061b364: lsl r6, r5, #3
0061b368: ldr r3, [r2, r5, lsl #3]
0061b36c: add r2, r2, r6
0061b370: sub r3, r3, #1
0061b374: cmp r3, #0xc
0061b378: addls pc, pc, r3, lsl #2
0061b37c: b #0x61b424
0061b380: b #0x61b84c
0061b384: b #0x61b720
0061b388: b #0x61b68c
0061b38c: b #0x61b664
0061b390: b #0x61b424
0061b394: b #0x61b424
0061b398: b #0x61b424
0061b39c: b #0x61b424
0061b3a0: b #0x61b804
0061b3a4: b #0x61b3b4
0061b3a8: b #0x61b828
0061b3ac: b #0x61b614
0061b3b0: b #0x61b568
0061b3b4: ldr r1, [r2, #4]
0061b3b8: mov r0, r8
0061b3bc: mov r2, fp
0061b3c0: mov r3, sl
0061b3c4: bl #0x61a608
0061b3c8: subs sb, r0, #0
0061b3cc: beq #0x61b600
0061b3d0: ldr r2, [r4, #0x44]
0061b3d4: ldr r3, [sb]
0061b3d8: add r6, r2, r6
0061b3dc: ldr r2, [r6, #4]
0061b3e0: ldr r1, [r2, #0x14]
0061b3e4: mov lr, pc
0061b3e8: ldr pc, [r3, #0xd4]
0061b3ec: mov r0, sb
0061b3f0: ldr r3, [sb]
0061b3f4: mov lr, pc
0061b3f8: ldr pc, [r3, #0x104]
0061b3fc: mov r0, r7
0061b400: ldr r3, [r7]
0061b404: mov r1, sb
0061b408: mov lr, pc
0061b40c: ldr pc, [r3, #0x5c]
0061b410: ldr r3, [sb]
0061b414: ldr r0, [r3, #-0xc]
0061b418: add r0, sb, r0
0061b41c: bl #0x31d584
0061b420: ldr r1, [r4, #0x40]
0061b424: add r5, r5, #1
0061b428: cmp r5, r1
0061b42c: blt #0x61b360
0061b430: mov r6, r7
0061b434: mov r0, r6
0061b438: ldr r1, [r4, #4]
0061b43c: ldr r3, [r6]
0061b440: mov lr, pc
0061b444: ldr pc, [r3, #0x28]
0061b448: ldr r3, [r6]
0061b44c: ldr r2, [r4, #0xc]
0061b450: mov r0, r6
0061b454: ldr r3, [r3, #0xa4]
0061b458: str r2, [sp, #0x2c]
0061b45c: ldr r2, [r4, #0x10]
0061b460: add r1, sp, #0x2c
0061b464: str r2, [sp, #0x30]
0061b468: ldr r2, [r4, #0x14]
0061b46c: str r2, [sp, #0x34]
0061b470: blx r3
0061b474: ldr r3, [r6]
0061b478: ldr r2, [r4, #0x18]
0061b47c: mov r0, r6
0061b480: ldr r3, [r3, #0x9c]
0061b484: str r2, [sp, #0x10]
0061b488: ldr r2, [r4, #0x1c]
0061b48c: add r1, sp, #0x10
0061b490: str r2, [sp, #0x14]
0061b494: ldr r2, [r4, #0x20]
0061b498: str r2, [sp, #0x18]
0061b49c: ldr r2, [r4, #0x24]
0061b4a0: str r2, [sp, #0x1c]
0061b4a4: blx r3
0061b4a8: ldr r3, [r6]
0061b4ac: ldr r2, [r4, #0x28]
0061b4b0: mov r0, r6
0061b4b4: ldr r3, [r3, #0x94]
0061b4b8: str r2, [sp, #0x20]
0061b4bc: ldr r2, [r4, #0x2c]
0061b4c0: add r1, sp, #0x20
0061b4c4: str r2, [sp, #0x24]
0061b4c8: ldr r2, [r4, #0x30]
0061b4cc: str r2, [sp, #0x28]
0061b4d0: blx r3
0061b4d4: ldr r1, [r4, #0x34]
0061b4d8: ldr r3, [r6]
0061b4dc: mov r0, r6
0061b4e0: subs r1, r1, #0
0061b4e4: movne r1, #1
0061b4e8: mov lr, pc
0061b4ec: ldr pc, [r3, #0x48]
0061b4f0: ldr r3, [r4, #0x38]
0061b4f4: cmp r3, #0
0061b4f8: ble #0x61b55c
0061b4fc: mov r5, #0
0061b500: mov r7, r5
0061b504: mov sb, r8
0061b508: ldr r2, [r4, #0x3c]
0061b50c: mov r3, sl
0061b510: mov r1, fp
0061b514: add r2, r2, r5
0061b518: mov r0, sb
0061b51c: bl #0x61b2f4
0061b520: ldr r3, [r6]
0061b524: mov r8, r0
0061b528: mov r1, r0
0061b52c: mov r0, r6
0061b530: mov lr, pc
0061b534: ldr pc, [r3, #0x5c]
0061b538: ldr r3, [r8]
0061b53c: add r7, r7, #1
0061b540: add r5, r5, #0x50
0061b544: ldr r0, [r3, #-0xc]
0061b548: add r0, r8, r0
0061b54c: bl #0x31d584
0061b550: ldr r3, [r4, #0x38]
0061b554: cmp r7, r3
0061b558: blt #0x61b508
0061b55c: mov r0, r6
0061b560: add sp, sp, #0x44
0061b564: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061b568: ldr r2, [r2, #4]
0061b56c: ldr r0, [sp, #8]
0061b570: mov r1, r8
0061b574: mov r3, sl
0061b578: bl #0x60e6f0
0061b57c: ldr r3, [r8, #4]
0061b580: mov r1, r8
0061b584: ldr r2, [sp, #8]
0061b588: mov r0, r3
0061b58c: ldr ip, [r3]
0061b590: ldr r3, [r4, #0x48]
0061b594: mov lr, pc
0061b598: ldr pc, [ip, #0x50]
0061b59c: subs sb, r0, #0
0061b5a0: beq #0x61b5f0
0061b5a4: ldr r2, [r4, #0x44]
0061b5a8: ldr r3, [sb]
0061b5ac: add r6, r2, r6
0061b5b0: ldr r2, [r6, #4]
0061b5b4: ldr r1, [r2, #0x14]
0061b5b8: mov lr, pc
0061b5bc: ldr pc, [r3, #0xd4]
0061b5c0: mov r0, sb
0061b5c4: mov r1, #2
0061b5c8: bl #0x59719c
0061b5cc: mov r0, r7
0061b5d0: ldr r3, [r7]
0061b5d4: mov r1, sb
0061b5d8: mov lr, pc
0061b5dc: ldr pc, [r3, #0x5c]
0061b5e0: ldr r3, [sb]
0061b5e4: ldr r0, [r3, #-0xc]
0061b5e8: add r0, sb, r0
0061b5ec: bl #0x31d584
0061b5f0: ldr r0, [sp, #0x38]
0061b5f4: cmp r0, #0
0061b5f8: beq #0x61b600
0061b5fc: bl #0x31d584
0061b600: ldr r1, [r4, #0x40]
0061b604: add r5, r5, #1
0061b608: cmp r5, r1
0061b60c: blt #0x61b360
0061b610: b #0x61b430
0061b614: ldr r1, [r2, #4]
0061b618: mov r0, r8
0061b61c: mov r2, sl
0061b620: bl #0x61a994
0061b624: subs r6, r0, #0
0061b628: beq #0x61b600
0061b62c: mov r1, r6
0061b630: mov r0, r7
0061b634: ldr r3, [r7]
0061b638: mov lr, pc
0061b63c: ldr pc, [r3, #0x5c]
0061b640: ldr r3, [r6]
0061b644: add r5, r5, #1
0061b648: ldr r0, [r3, #-0xc]
0061b64c: add r0, r6, r0
0061b650: bl #0x31d584
0061b654: ldr r1, [r4, #0x40]
0061b658: cmp r5, r1
0061b65c: blt #0x61b360
0061b660: b #0x61b430
0061b664: ldr r3, [r2, #4]
0061b668: mov r0, r8
0061b66c: mov r2, sl
0061b670: ldr r1, [r3, #4]
0061b674: add r1, r1, #1
0061b678: bl #0x61b24c
0061b67c: subs r6, r0, #0
0061b680: bne #0x61b62c
0061b684: ldr r1, [r4, #0x40]
0061b688: b #0x61b604
0061b68c: ldr r3, [r2, #4]
0061b690: ldr r0, [sp, #0xc]
0061b694: mov r1, r8
0061b698: mov r2, fp
0061b69c: str sl, [sp]
0061b6a0: bl #0x61aeb8
0061b6a4: ldr r0, [sp, #0x3c]
0061b6a8: cmp r0, #0
0061b6ac: str r0, [sp, #0x38]
0061b6b0: ldrne r3, [r0, #4]
0061b6b4: addne r3, r3, #1
0061b6b8: strne r3, [r0, #4]
0061b6bc: ldrne r0, [sp, #0x3c]
0061b6c0: cmp r0, #0
0061b6c4: beq #0x61b6cc
0061b6c8: bl #0x31d584
0061b6cc: ldr r3, [sp, #0x38]
0061b6d0: cmp r3, #0
0061b6d4: beq #0x61b600
0061b6d8: ldr r3, [r8, #4]
0061b6dc: mov r1, r8
0061b6e0: ldr r2, [sp, #8]
0061b6e4: mov r0, r3
0061b6e8: ldr ip, [r3]
0061b6ec: ldr r3, [r4, #0x48]
0061b6f0: mov lr, pc
0061b6f4: ldr pc, [ip, #0x48]
0061b6f8: subs sb, r0, #0
0061b6fc: beq #0x61b7f0
0061b700: ldr r2, [r4, #0x44]
0061b704: ldr r3, [sb]
0061b708: add r6, r2, r6
0061b70c: ldr r2, [r6, #4]
0061b710: ldr r1, [r2, #0x14]
0061b714: mov lr, pc
0061b718: ldr pc, [r3, #0xd4]
0061b71c: b #0x61b7cc
0061b720: ldr r3, [r2, #4]
0061b724: mov ip, #1
0061b728: ldr r0, [sp, #8]
0061b72c: mov r1, r8
0061b730: mov r2, fp
0061b734: stm sp, {sl, ip}
0061b738: bl #0x61ace8
0061b73c: ldr r3, [sp, #0x38]
0061b740: mov r0, r3
0061b744: ldr r3, [r3]
0061b748: mov lr, pc
0061b74c: ldr pc, [r3, #0x30]
0061b750: cmp r0, #2
0061b754: beq #0x61b894
0061b758: ldr r3, [sp, #0x38]
0061b75c: mov r0, r3
0061b760: ldr r3, [r3]
0061b764: mov lr, pc
0061b768: ldr pc, [r3, #0x30]
0061b76c: cmp r0, #3
0061b770: beq #0x61b894
0061b774: ldr r3, [r8, #4]
0061b778: mov r1, r8
0061b77c: ldr r2, [sp, #8]
0061b780: mov r0, r3
0061b784: ldr ip, [r3]
0061b788: ldr r3, [r4, #0x48]
0061b78c: mov lr, pc
0061b790: ldr pc, [ip, #0x48]
0061b794: mov sb, r0
0061b798: cmp sb, #0
0061b79c: beq #0x61b7f0
0061b7a0: ldr r2, [r4, #0x44]
0061b7a4: mov r0, sb
0061b7a8: ldr r3, [sb]
0061b7ac: add r6, r2, r6
0061b7b0: ldr r2, [r6, #4]
0061b7b4: ldr r1, [r2, #0x14]
0061b7b8: mov lr, pc
0061b7bc: ldr pc, [r3, #0xd4]
0061b7c0: mov r0, sb
0061b7c4: mov r1, #2
0061b7c8: bl #0x59719c
0061b7cc: mov r0, r7
0061b7d0: ldr r3, [r7]
0061b7d4: mov r1, sb
0061b7d8: mov lr, pc
0061b7dc: ldr pc, [r3, #0x5c]
0061b7e0: ldr r3, [sb]
0061b7e4: ldr r0, [r3, #-0xc]
0061b7e8: add r0, sb, r0
0061b7ec: bl #0x31d584
0061b7f0: ldr r0, [sp, #0x38]
0061b7f4: cmp r0, #0
0061b7f8: bne #0x61b41c
0061b7fc: ldr r1, [r4, #0x40]
0061b800: b #0x61b604
0061b804: ldr r1, [r2, #4]
0061b808: mov r0, r8
0061b80c: mov r2, fp
0061b810: mov r3, sl
0061b814: bl #0x61a888
0061b818: subs sb, r0, #0
0061b81c: bne #0x61b3d0
0061b820: ldr r1, [r4, #0x40]
0061b824: b #0x61b604
0061b828: ldr r1, [r2, #4]
0061b82c: mov r0, r8
0061b830: mov r2, fp
0061b834: mov r3, sl
0061b838: bl #0x61a77c
0061b83c: subs r6, r0, #0
0061b840: bne #0x61b62c
0061b844: ldr r1, [r4, #0x40]
0061b848: b #0x61b604
0061b84c: ldr r3, [r2, #4]
0061b850: mov r0, r8
0061b854: mov r2, sl
0061b858: ldr r1, [r3, #4]
0061b85c: add r1, r1, #1
0061b860: bl #0x61b2d0
0061b864: subs r6, r0, #0
0061b868: bne #0x61b62c
0061b86c: ldr r1, [r4, #0x40]
0061b870: b #0x61b604
0061b874: ldr r3, [r0, #4]
0061b878: mov r1, r0
0061b87c: mov r0, r3
0061b880: ldr r3, [r3]
0061b884: mov lr, pc
0061b888: ldr pc, [r3, #0x3c]
0061b88c: mov r6, r0
0061b890: b #0x61b33c
0061b894: ldr r3, [r8, #4]
0061b898: mov r1, r8
0061b89c: ldr r2, [sp, #8]
0061b8a0: mov r0, r3
0061b8a4: ldr ip, [r3]
0061b8a8: ldr r3, [r4, #0x48]
0061b8ac: mov lr, pc
0061b8b0: ldr pc, [ip, #0x4c]
0061b8b4: mov sb, r0
0061b8b8: b #0x61b798

# 0x61be14 _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPNS0_5SNodeE
0061be14: push {r4, r5, r6, r7, r8, lr}
0061be18: subs r6, r2, #0
0061be1c: mov r5, r0
0061be20: mov r7, r1
0061be24: beq #0x61be90
0061be28: ldr r3, [r0, #4]
0061be2c: mov r1, r0
0061be30: mov r0, r3
0061be34: ldr r3, [r3]
0061be38: mov lr, pc
0061be3c: ldr pc, [r3, #0x54]
0061be40: mov r4, r0
0061be44: mov r2, r6
0061be48: mov r1, r7
0061be4c: mov r3, r4
0061be50: mov r0, r5
0061be54: bl #0x61b2f4
0061be58: ldr r3, [r4]
0061be5c: mov r1, r0
0061be60: mov r5, r0
0061be64: mov r0, r4
0061be68: mov lr, pc
0061be6c: ldr pc, [r3, #0x5c]
0061be70: mov r0, r4
0061be74: bl #0x65b3dc
0061be78: ldr r3, [r5]
0061be7c: ldr r0, [r3, #-0xc]
0061be80: add r0, r5, r0
0061be84: bl #0x31d584
0061be88: mov r0, r4
0061be8c: pop {r4, r5, r6, r7, r8, pc}
0061be90: mov r0, r6
0061be94: pop {r4, r5, r6, r7, r8, pc}

# 0x61c214 _ZNK6glitch7collada16CColladaDatabase7getNodeEPKcRNS0_5SNodeE
0061c214: push {r4, r5, r6, r7, r8, sb, sl, lr}
0061c218: mov r7, r0
0061c21c: ldr r0, [r2]
0061c220: mov r4, r2
0061c224: mov r8, r1
0061c228: bl #0x30e31c
0061c22c: cmp r0, #0
0061c230: bne #0x61c23c
0061c234: mov r0, r4
0061c238: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0061c23c: ldr sl, [r4, #0x38]
0061c240: cmp sl, #0
0061c244: ble #0x61c288
0061c248: mov r5, #0
0061c24c: mov r6, r5
0061c250: b #0x61c260
0061c254: cmp r6, sl
0061c258: add r5, r5, #0x50
0061c25c: beq #0x61c288
0061c260: ldr r2, [r4, #0x3c]
0061c264: mov r0, r7
0061c268: mov r1, r8
0061c26c: add r2, r2, r5
0061c270: bl #0x61c214
0061c274: cmp r0, #0
0061c278: add r6, r6, #1
0061c27c: beq #0x61c254
0061c280: mov r4, r0
0061c284: b #0x61c234
0061c288: mov r4, #0
0061c28c: b #0x61c234

# 0x61c290 _ZNK6glitch7collada16CColladaDatabase7getNodeEPKc
0061c290: push {r4, r5, r6, r7, r8, sb, sl, lr}
0061c294: mov r8, r1
0061c298: mov r1, #0
0061c29c: mov r7, r0
0061c2a0: bl #0x60e54c
0061c2a4: subs r6, r0, #0
0061c2a8: bne #0x61c2b4
0061c2ac: mov r0, #0
0061c2b0: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0061c2b4: ldr sl, [r6, #8]
0061c2b8: cmp sl, #0
0061c2bc: ble #0x61c2ac
0061c2c0: mov r4, #0
0061c2c4: mov r5, r4
0061c2c8: ldr r2, [r6, #0xc]
0061c2cc: mov r0, r7
0061c2d0: mov r1, r8
0061c2d4: add r2, r2, r4
0061c2d8: bl #0x61c214
0061c2dc: cmp r0, #0
0061c2e0: add r5, r5, #1
0061c2e4: bne #0x61c2b0
0061c2e8: cmp r5, sl
0061c2ec: add r4, r4, #0x50
0061c2f0: bne #0x61c2c8
0061c2f4: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x61c6f4 _ZNK6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPKc
0061c6f4: push {r4, r5, r6, lr}
0061c6f8: mov r4, r1
0061c6fc: mov r1, r2
0061c700: mov r5, r0
0061c704: bl #0x61c290
0061c708: mov r1, r4
0061c70c: mov r2, r0
0061c710: mov r0, r5
0061c714: pop {r4, r5, r6, lr}
0061c718: b #0x61be14

# 0x61c878 _ZN6glitch7collada16CColladaDatabase13constructNodeEPNS_5video12IVideoDriverEPKcS6_PNS0_15CColladaFactoryE
0061c878: push {r4, r5, r6, r7, r8, sl, lr}
0061c87c: ldr r4, [pc, #0x90]
0061c880: ldr r5, [pc, #0x90]
0061c884: mov sl, r2
0061c888: add r4, pc, r4
0061c88c: ldr r6, [r4, r5]
0061c890: mov r2, #0
0061c894: mov r8, r0
0061c898: sub sp, sp, #0xc
0061c89c: mov r7, r3
0061c8a0: ldr r0, [r6]
0061c8a4: mov r3, r2
0061c8a8: bl #0x65ac5c
0061c8ac: cmp r0, #0
0061c8b0: moveq r8, r0
0061c8b4: beq #0x61c908
0061c8b8: ldr r3, [r6]
0061c8bc: mov r2, #0
0061c8c0: mov r1, r8
0061c8c4: ldrb r6, [r3, #0x28]
0061c8c8: strb r2, [r3, #0x28]
0061c8cc: stm sp, {r0, r7}
0061c8d0: ldr r3, [r0, #4]
0061c8d4: mov r7, sp
0061c8d8: cmp r3, r2
0061c8dc: addne r3, r3, #1
0061c8e0: strne r3, [r0, #4]
0061c8e4: mov r2, sl
0061c8e8: mov r0, sp
0061c8ec: bl #0x61c6f4
0061c8f0: mov r8, r0
0061c8f4: mov r0, sp
0061c8f8: bl #0x619474
0061c8fc: ldr r3, [r4, r5]
0061c900: ldr r3, [r3]
0061c904: strb r6, [r3, #0x28]
0061c908: mov r0, r8
0061c90c: add sp, sp, #0xc
0061c910: pop {r4, r5, r6, r7, r8, sl, pc}
0061c914: eorseq r8, r7, r8, lsl #4
0061c918: andeq r4, r0, r8, asr #8
