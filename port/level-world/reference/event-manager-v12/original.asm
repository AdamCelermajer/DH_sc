
_ZN30ObjectiveTemplate_InteractWithIN7Structs19v2QuestTriggerPlateE15QE_TriggerPlateE11handleEventEPK6IEventPK12EventManager 0x47f194 148
0047f194: push {r4, lr}
0047f198: ldr r3, [r1, #0x18]
0047f19c: ldr r2, [r0, #0x24]
0047f1a0: mov r4, r0
0047f1a4: ldr r0, [r0, #0xc]
0047f1a8: cmp r2, r3
0047f1ac: beq #0x47f1b8
0047f1b0: mov r0, #0
0047f1b4: pop {r4, pc}
0047f1b8: ldrb r3, [r1, #0x11]
0047f1bc: cmp r3, #0
0047f1c0: bne #0x47f210
0047f1c4: ldr r3, [r4, #0x20]
0047f1c8: add r3, r3, #1
0047f1cc: str r3, [r4, #0x20]
0047f1d0: mov r3, #1
0047f1d4: strb r3, [r1, #0x10]
0047f1d8: ldr r3, [r4, #0x20]
0047f1dc: str r3, [r1, #0x14]
0047f1e0: ldr r3, [r4, #0x20]
0047f1e4: ldr r2, [r0, #0x28]
0047f1e8: cmp r2, r3
0047f1ec: bgt #0x47f1b0
0047f1f0: mov r0, r4
0047f1f4: bl #0x47ba10
0047f1f8: mov r0, r4
0047f1fc: ldr r3, [r4]
0047f200: mov lr, pc
0047f204: ldr pc, [r3, #0x1c]
0047f208: mov r0, #0
0047f20c: pop {r4, pc}
0047f210: ldr r3, [r1, #0x14]
0047f214: ldr r2, [r4, #0x20]
0047f218: cmp r2, r3
0047f21c: strlt r3, [r4, #0x20]
0047f220: blt #0x47f1e4
0047f224: b #0x47f1b0

_ZN16GameEventManagerD1Ev 0x479bec 56
00479bec: push {r4, lr}
00479bf0: mov r4, r0
00479bf4: bl #0x479968
00479bf8: ldr r3, [r4]
00479bfc: cmp r3, #0
00479c00: beq #0x479c1c
00479c04: ldr r2, [r4, #8]
00479c08: mov r1, r3
00479c0c: add r0, r4, #8
00479c10: rsb r3, r3, r2
00479c14: asr r2, r3, #2
00479c18: bl #0x479bd0
00479c1c: mov r0, r4
00479c20: pop {r4, pc}

_ZN16GameEventManagerD2Ev 0x479f34 56
00479f34: push {r4, lr}
00479f38: mov r4, r0
00479f3c: bl #0x479968
00479f40: ldr r3, [r4]
00479f44: cmp r3, #0
00479f48: beq #0x479f64
00479f4c: ldr r2, [r4, #8]
00479f50: mov r1, r3
00479f54: add r0, r4, #8
00479f58: rsb r3, r3, r2
00479f5c: asr r2, r3, #2
00479f60: bl #0x479bd0
00479f64: mov r0, r4
00479f68: pop {r4, pc}

_ZN31ObjectiveTemplate_KillCharacterIN7Structs18v2QuestKillEnemiesE14QE_KillEnemies14TestCharPropId18Objective_SavedQtyE11handleEventEPK6IEventPK12EventManager 0x47f100 148
0047f100: push {r4, lr}
0047f104: ldr r3, [r0, #0x24]
0047f108: ldr r2, [r1, #0x18]
0047f10c: mov r4, r0
0047f110: ldr r3, [r3, #0x20]
0047f114: cmp r3, r2
0047f118: beq #0x47f124
0047f11c: mov r0, #0
0047f120: pop {r4, pc}
0047f124: ldrb r3, [r1, #0x11]
0047f128: cmp r3, #0
0047f12c: bne #0x47f17c
0047f130: ldr r3, [r0, #0x20]
0047f134: add r3, r3, #1
0047f138: str r3, [r0, #0x20]
0047f13c: mov r3, #1
0047f140: strb r3, [r1, #0x10]
0047f144: ldr r3, [r0, #0x20]
0047f148: str r3, [r1, #0x14]
0047f14c: ldr r3, [r0, #0x20]
0047f150: ldr r2, [r4, #0x2c]
0047f154: cmp r2, r3
0047f158: bgt #0x47f11c
0047f15c: mov r0, r4
0047f160: bl #0x47ba10
0047f164: mov r0, r4
0047f168: ldr r3, [r4]
0047f16c: mov lr, pc
0047f170: ldr pc, [r3, #0x1c]
0047f174: mov r0, #0
0047f178: pop {r4, pc}
0047f17c: ldr r3, [r1, #0x14]
0047f180: ldr r2, [r0, #0x20]
0047f184: cmp r2, r3
0047f188: strlt r3, [r0, #0x20]
0047f18c: blt #0x47f150
0047f190: b #0x47f11c

_ZN30ObjectiveTemplate_InteractWithIN7Structs24v2QuestDestroyGameObjectE20QE_DestroyGameObjectE11handleEventEPK6IEventPK12EventManager 0x47f3e4 148
0047f3e4: push {r4, lr}
0047f3e8: ldr r3, [r1, #0x18]
0047f3ec: ldr r2, [r0, #0x24]
0047f3f0: mov r4, r0
0047f3f4: ldr r0, [r0, #0xc]
0047f3f8: cmp r2, r3
0047f3fc: beq #0x47f408
0047f400: mov r0, #0
0047f404: pop {r4, pc}
0047f408: ldrb r3, [r1, #0x11]
0047f40c: cmp r3, #0
0047f410: bne #0x47f460
0047f414: ldr r3, [r4, #0x20]
0047f418: add r3, r3, #1
0047f41c: str r3, [r4, #0x20]
0047f420: mov r3, #1
0047f424: strb r3, [r1, #0x10]
0047f428: ldr r3, [r4, #0x20]
0047f42c: str r3, [r1, #0x14]
0047f430: ldr r3, [r4, #0x20]
0047f434: ldr r2, [r0, #0x28]
0047f438: cmp r2, r3
0047f43c: bgt #0x47f400
0047f440: mov r0, r4
0047f444: bl #0x47ba10
0047f448: mov r0, r4
0047f44c: ldr r3, [r4]
0047f450: mov lr, pc
0047f454: ldr pc, [r3, #0x1c]
0047f458: mov r0, #0
0047f45c: pop {r4, pc}
0047f460: ldr r3, [r1, #0x14]
0047f464: ldr r2, [r4, #0x20]
0047f468: cmp r2, r3
0047f46c: strlt r3, [r4, #0x20]
0047f470: blt #0x47f434
0047f474: b #0x47f400

_ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIN12EventManager12ReceiverInfoESaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE8_M_eraseEPNS_18_Rb_tree_node_baseE 0x338438 116
00338438: push {r4, r5, r6, r7, r8, lr}
0033843c: subs r6, r1, #0
00338440: mov r8, r0
00338444: beq #0x3384a8
00338448: mov r0, r8
0033844c: ldr r1, [r6, #0xc]
00338450: bl #0x338438
00338454: ldr r3, [r6, #0x14]
00338458: add r5, r6, #0x14
0033845c: ldr r7, [r6, #8]
00338460: cmp r3, r5
00338464: beq #0x33848c
00338468: mov r0, r3
0033846c: b #0x338474
00338470: mov r0, r4
00338474: ldr r4, [r0]
00338478: mov r1, #0x14
0033847c: bl #0x708f00
00338480: cmp r4, r5
00338484: bne #0x338470
00338488: mov r3, r5
0033848c: mov r0, r6
00338490: str r3, [r5, #4]
00338494: str r3, [r5]
00338498: mov r1, #0x1c
0033849c: bl #0x708f00
003384a0: subs r6, r7, #0
003384a4: bne #0x338448
003384a8: pop {r4, r5, r6, r7, r8, pc}

_ZN15v2EmuController8_onEventEPK13EvMouseButtonPK12EventManager 0x405f48 376
00405f48: push {r4, r5, r6, r7, r8, lr}
00405f4c: ldr r3, [r1, #8]
00405f50: ldr r7, [pc, #0x160]
00405f54: sub sp, sp, #0x40
00405f58: cmp r3, #0
00405f5c: mov r8, r1
00405f60: mov r4, r0
00405f64: add r7, pc, r7
00405f68: bne #0x405fe4
00405f6c: ldrb r3, [r1, #0xc]
00405f70: cmp r3, #0
00405f74: bne #0x405fe4
00405f78: ldr r3, [r0, #0x18]
00405f7c: and r2, r3, #3
00405f80: cmp r2, #3
00405f84: beq #0x40604c
00405f88: tst r3, #1
00405f8c: bne #0x405ff0
00405f90: tst r3, #2
00405f94: beq #0x405fe4
00405f98: ldrsh r0, [r1, #0x10]
00405f9c: mov r3, #0
00405fa0: str r3, [sp, #0xc]
00405fa4: str r3, [sp, #4]
00405fa8: str r3, [sp, #8]
00405fac: bl #0x30e964
00405fb0: mov r6, r0
00405fb4: ldrsh r0, [r8, #0xe]
00405fb8: bl #0x30e964
00405fbc: ldr r3, [pc, #0xf8]
00405fc0: add r5, sp, #4
00405fc4: str r0, [sp, #0x28]
00405fc8: add r1, sp, #0x28
00405fcc: ldr r0, [r7, r3]
00405fd0: mov r2, r5
00405fd4: str r6, [sp, #0x2c]
00405fd8: bl #0x525884
00405fdc: cmp r0, #0
00405fe0: bne #0x4060a8
00405fe4: mov r0, #0
00405fe8: add sp, sp, #0x40
00405fec: pop {r4, r5, r6, r7, r8, pc}
00405ff0: ldrsh r0, [r1, #0x10]
00405ff4: mov r3, #0
00405ff8: str r3, [sp, #0x18]
00405ffc: str r3, [sp, #0x10]
00406000: str r3, [sp, #0x14]
00406004: bl #0x30e964
00406008: mov r6, r0
0040600c: ldrsh r0, [r8, #0xe]
00406010: bl #0x30e964
00406014: ldr r3, [pc, #0xa0]
00406018: add r5, sp, #0x10
0040601c: str r0, [sp, #0x30]
00406020: add r1, sp, #0x30
00406024: ldr r0, [r7, r3]
00406028: mov r2, r5
0040602c: str r6, [sp, #0x34]
00406030: bl #0x525884
00406034: cmp r0, #0
00406038: beq #0x405fe4
0040603c: mov r0, r4
00406040: mov r1, r5
00406044: bl #0x405318
00406048: b #0x405fe4
0040604c: ldrsh r0, [r1, #0x10]
00406050: mov r3, #0
00406054: str r3, [sp, #0x24]
00406058: str r3, [sp, #0x1c]
0040605c: str r3, [sp, #0x20]
00406060: bl #0x30e964
00406064: mov r6, r0
00406068: ldrsh r0, [r8, #0xe]
0040606c: bl #0x30e964
00406070: ldr r3, [pc, #0x44]
00406074: add r5, sp, #0x1c
00406078: str r0, [sp, #0x38]
0040607c: add r1, sp, #0x38
00406080: ldr r0, [r7, r3]
00406084: mov r2, r5
00406088: str r6, [sp, #0x3c]
0040608c: bl #0x525884
00406090: cmp r0, #0
00406094: beq #0x405fe4
00406098: mov r0, r4
0040609c: mov r1, r5
004060a0: bl #0x4054e4
004060a4: b #0x405fe4
004060a8: mov r0, r4
004060ac: mov r1, r5
004060b0: bl #0x405260
004060b4: b #0x405fe4
004060b8: subseq lr, r8, ip, lsr #22
004060bc: andeq r1, r0, r4, lsl #4

_ZNK6IEvent10getEventIDEv 0x31d8dc 8
0031d8dc: ldr r0, [r0, #4]
0031d8e0: bx lr

_ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIN12EventManager12ReceiverInfoESaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE9_M_insertEPNS_18_Rb_tree_node_baseERKSA_SI_SI_ 0x3386c4 216
003386c4: cmp r1, r2
003386c8: push {r4, r5, r6, r7, r8, lr}
003386cc: mov r4, r1
003386d0: mov r5, r2
003386d4: mov r6, r0
003386d8: beq #0x338768
003386dc: ldr r2, [sp, #0x1c]
003386e0: cmp r2, #0
003386e4: beq #0x338730
003386e8: mov r1, r3
003386ec: mov r0, r4
003386f0: bl #0x33862c
003386f4: str r0, [r5, #0xc]
003386f8: ldr r3, [r4, #0xc]
003386fc: mov r7, r0
00338700: cmp r5, r3
00338704: beq #0x338760
00338708: mov r0, r7
0033870c: str r5, [r7, #4]
00338710: add r1, r4, #4
00338714: bl #0x313760
00338718: ldr r3, [r4, #0x10]
0033871c: mov r0, r6
00338720: add r3, r3, #1
00338724: str r3, [r4, #0x10]
00338728: str r7, [r6]
0033872c: pop {r4, r5, r6, r7, r8, pc}
00338730: ldr r2, [sp, #0x18]
00338734: cmp r2, #0
00338738: beq #0x338788
0033873c: mov r1, r3
00338740: mov r0, r4
00338744: bl #0x33862c
00338748: str r0, [r5, #8]
0033874c: ldr r3, [r4, #8]
00338750: mov r7, r0
00338754: cmp r5, r3
00338758: streq r0, [r4, #8]
0033875c: b #0x338708
00338760: str r7, [r4, #0xc]
00338764: b #0x338708
00338768: mov r1, r3
0033876c: mov r0, r4
00338770: bl #0x33862c
00338774: mov r7, r0
00338778: str r0, [r4, #8]
0033877c: str r0, [r4, #4]
00338780: str r0, [r4, #0xc]
00338784: b #0x338708
00338788: ldr r1, [r3]
0033878c: ldr r2, [r5, #0x10]
00338790: cmp r1, r2
00338794: bge #0x3386e8
00338798: b #0x33873c

_ZN12EventManager13DelayedDetachEiP14IEventReceiver 0x3382a4 208
003382a4: push {r4, r5, r6, lr}
003382a8: ldr r4, [r0, #0xc]
003382ac: sub sp, sp, #8
003382b0: mov r5, r0
003382b4: cmp r4, #0
003382b8: add ip, r0, #8
003382bc: beq #0x338338
003382c0: mov r0, ip
003382c4: b #0x3382cc
003382c8: mov r4, r3
003382cc: ldr r3, [r4, #0x10]
003382d0: cmp r3, r1
003382d4: ldrlt r3, [r4, #0xc]
003382d8: ldrge r3, [r4, #8]
003382dc: movlt r4, r0
003382e0: mov r0, r4
003382e4: cmp r3, #0
003382e8: bne #0x3382c8
003382ec: cmp ip, r4
003382f0: beq #0x33832c
003382f4: ldr r3, [r4, #0x10]
003382f8: cmp r3, r1
003382fc: bgt #0x338338
00338300: cmp ip, r4
00338304: beq #0x33832c
00338308: mov r1, r4
0033830c: ldr r6, [r1, #0x14]!
00338310: b #0x338324
00338314: ldr r3, [r6, #8]
00338318: cmp r3, r2
0033831c: beq #0x338340
00338320: ldr r6, [r6]
00338324: cmp r1, r6
00338328: bne #0x338314
0033832c: mov r0, #0
00338330: add sp, sp, #8
00338334: pop {r4, r5, r6, pc}
00338338: mov r4, ip
0033833c: b #0x338300
00338340: mov r3, #0x10
00338344: add r0, sp, #8
00338348: str r3, [r0, #-4]!
0033834c: bl #0x708ec0
00338350: str r4, [r0, #8]
00338354: str r6, [r0, #0xc]
00338358: ldr r3, [r5, #0x2c]
0033835c: add r2, r5, #0x28
00338360: stm r0, {r2, r3}
00338364: str r0, [r3]
00338368: str r0, [r5, #0x2c]
0033836c: mov r0, #1
00338370: b #0x338330

_ZNSt3mapIiSt4listIN12EventManager12ReceiverInfoESaIS2_EESt4lessIiESaISt4pairIKiS4_EEEixIiEERS4_RKT_ 0x338c98 264
00338c98: push {r4, r5, r6, r7, r8, lr}
00338c9c: ldr ip, [r0, #4]
00338ca0: sub sp, sp, #0x20
00338ca4: cmp ip, #0
00338ca8: beq #0x338d94
00338cac: ldr r1, [r1]
00338cb0: mov r2, r0
00338cb4: b #0x338cbc
00338cb8: mov ip, r3
00338cbc: ldr r3, [ip, #0x10]
00338cc0: cmp r3, r1
00338cc4: ldrlt r3, [ip, #0xc]
00338cc8: ldrge r3, [ip, #8]
00338ccc: movlt ip, r2
00338cd0: mov r2, ip
00338cd4: cmp r3, #0
00338cd8: bne #0x338cb8
00338cdc: cmp r0, ip
00338ce0: beq #0x338cf4
00338ce4: ldr r3, [ip, #0x10]
00338ce8: mov r8, ip
00338cec: cmp r1, r3
00338cf0: bge #0x338d88
00338cf4: add r5, sp, #0x20
00338cf8: str r1, [r5, #-0x1c]!
00338cfc: add r6, r5, #4
00338d00: mov r1, r0
00338d04: add r4, sp, #0x10
00338d08: add r0, sp, #0x1c
00338d0c: add r2, sp, #0x18
00338d10: mov r3, r5
00338d14: str ip, [sp, #0x18]
00338d18: str r4, [sp, #0x10]
00338d1c: str r4, [sp, #0x14]
00338d20: str r6, [sp, #8]
00338d24: str r6, [sp, #0xc]
00338d28: bl #0x338924
00338d2c: ldr r0, [sp, #8]
00338d30: ldr r8, [sp, #0x1c]
00338d34: cmp r0, r6
00338d38: bne #0x338d44
00338d3c: b #0x338d58
00338d40: mov r0, r7
00338d44: ldr r7, [r0]
00338d48: mov r1, #0x14
00338d4c: bl #0x708f00
00338d50: cmp r7, r6
00338d54: bne #0x338d40
00338d58: ldr r0, [sp, #0x10]
00338d5c: cmp r0, r4
00338d60: beq #0x338d88
00338d64: add r5, r5, #4
00338d68: str r5, [sp, #0xc]
00338d6c: str r5, [sp, #8]
00338d70: ldr r5, [r0]
00338d74: mov r1, #0x14
00338d78: bl #0x708f00
00338d7c: cmp r5, r4
00338d80: mov r0, r5
00338d84: bne #0x338d70
00338d88: add r0, r8, #0x14
00338d8c: add sp, sp, #0x20
00338d90: pop {r4, r5, r6, r7, r8, pc}
00338d94: ldr r1, [r1]
00338d98: mov ip, r0
00338d9c: b #0x338cdc

_ZN12EventManager6UpdateEd 0x33900c 132
0033900c: push {r4, r5, r6, lr}
00339010: mov r6, r0
00339014: ldr r3, [r6, #0x20]
00339018: add r4, r0, #0x20
0033901c: cmp r3, r4
00339020: beq #0x339084
00339024: mov r2, r3
00339028: ldr r2, [r2]
0033902c: cmp r4, r2
00339030: bne #0x339028
00339034: ldr r5, [r3, #8]
00339038: mov r0, r6
0033903c: mov r1, r5
00339040: bl #0x338ebc
00339044: cmp r5, #0
00339048: beq #0x33905c
0033904c: mov r0, r5
00339050: ldr r3, [r5]
00339054: mov lr, pc
00339058: ldr pc, [r3, #4]
0033905c: ldr r0, [r6, #0x20]
00339060: mov r1, #0xc
00339064: ldr r3, [r0]
00339068: ldr r2, [r0, #4]
0033906c: str r3, [r2]
00339070: str r2, [r3, #4]
00339074: bl #0x708f00
00339078: ldr r3, [r6, #0x20]
0033907c: cmp r3, r4
00339080: bne #0x339024
00339084: mov r0, r6
00339088: pop {r4, r5, r6, lr}
0033908c: b #0x3383f4

_ZN12EventManagerD2Ev 0x338590 124
00338590: ldr r3, [pc, #0x6c]
00338594: ldr r2, [pc, #0x6c]
00338598: push {r4, r5, r6, lr}
0033859c: add r3, pc, r3
003385a0: ldr r2, [r3, r2]
003385a4: mov r5, r0
003385a8: mov r4, r0
003385ac: add r2, r2, #8
003385b0: str r2, [r5], #0x20
003385b4: mov r0, r5
003385b8: bl #0x338374
003385bc: add r0, r4, #0x28
003385c0: bl #0x3383b4
003385c4: mov r0, r5
003385c8: bl #0x338374
003385cc: ldr r3, [r4, #0x18]
003385d0: cmp r3, #0
003385d4: beq #0x3385fc
003385d8: add r5, r4, #8
003385dc: mov r0, r5
003385e0: ldr r1, [r4, #0xc]
003385e4: bl #0x338438
003385e8: mov r3, #0
003385ec: str r5, [r4, #0x14]
003385f0: str r3, [r4, #0x18]
003385f4: str r5, [r4, #0x10]
003385f8: str r3, [r4, #0xc]
003385fc: mov r0, r4
00338600: pop {r4, r5, r6, pc}

_ZNK12EventManager10RaiseAsyncERK6IEvent 0x339090 4
00339090: b #0x338ebc

_ZN30ObjectiveTemplate_InteractWithIN7Structs21v2QuestOpenGameObjectE17QE_OpenGameObjectE11handleEventEPK6IEventPK12EventManager 0x47f478 148
0047f478: push {r4, lr}
0047f47c: ldr r3, [r1, #0x18]
0047f480: ldr r2, [r0, #0x24]
0047f484: mov r4, r0
0047f488: ldr r0, [r0, #0xc]
0047f48c: cmp r2, r3
0047f490: beq #0x47f49c
0047f494: mov r0, #0
0047f498: pop {r4, pc}
0047f49c: ldrb r3, [r1, #0x11]
0047f4a0: cmp r3, #0
0047f4a4: bne #0x47f4f4
0047f4a8: ldr r3, [r4, #0x20]
0047f4ac: add r3, r3, #1
0047f4b0: str r3, [r4, #0x20]
0047f4b4: mov r3, #1
0047f4b8: strb r3, [r1, #0x10]
0047f4bc: ldr r3, [r4, #0x20]
0047f4c0: str r3, [r1, #0x14]
0047f4c4: ldr r3, [r4, #0x20]
0047f4c8: ldr r2, [r0, #0x28]
0047f4cc: cmp r2, r3
0047f4d0: bgt #0x47f494
0047f4d4: mov r0, r4
0047f4d8: bl #0x47ba10
0047f4dc: mov r0, r4
0047f4e0: ldr r3, [r4]
0047f4e4: mov lr, pc
0047f4e8: ldr pc, [r3, #0x1c]
0047f4ec: mov r0, #0
0047f4f0: pop {r4, pc}
0047f4f4: ldr r3, [r1, #0x14]
0047f4f8: ldr r2, [r4, #0x20]
0047f4fc: cmp r2, r3
0047f500: strlt r3, [r4, #0x20]
0047f504: blt #0x47f4c8
0047f508: b #0x47f494

_ZN6glitch7IDevice16setEventReceiverEPNS_14IEventReceiverE 0x6714c8 68
006714c8: push {r4, r5, r6, lr}
006714cc: mov r4, r0
006714d0: ldr r0, [r0, #0x2c]
006714d4: mov r5, r1
006714d8: str r1, [r4, #0x28]
006714dc: cmp r0, #0
006714e0: beq #0x6714e8
006714e4: bl #0x6a093c
006714e8: ldr r3, [r4, #0x18]
006714ec: cmp r3, #0
006714f0: beq #0x671508
006714f4: mov r0, r3
006714f8: mov r1, r5
006714fc: ldr r3, [r3]
00671500: mov lr, pc
00671504: ldr pc, [r3, #0x34]
00671508: pop {r4, r5, r6, pc}

_ZN31ObjectiveTemplate_KillCharacterIN7Structs19v2QuestClearEnemiesE15QE_ClearEnemies14TestCharPropId18Objective_SavedQtyE11handleEventEPK6IEventPK12EventManager 0x47f228 148
0047f228: push {r4, lr}
0047f22c: ldr r3, [r0, #0x24]
0047f230: ldr r2, [r1, #0x18]
0047f234: mov r4, r0
0047f238: ldr r3, [r3, #0x24]
0047f23c: cmp r3, r2
0047f240: beq #0x47f24c
0047f244: mov r0, #0
0047f248: pop {r4, pc}
0047f24c: ldrb r3, [r1, #0x11]
0047f250: cmp r3, #0
0047f254: bne #0x47f2a4
0047f258: ldr r3, [r0, #0x20]
0047f25c: add r3, r3, #1
0047f260: str r3, [r0, #0x20]
0047f264: mov r3, #1
0047f268: strb r3, [r1, #0x10]
0047f26c: ldr r3, [r0, #0x20]
0047f270: str r3, [r1, #0x14]
0047f274: ldr r3, [r0, #0x20]
0047f278: ldr r2, [r4, #0x2c]
0047f27c: cmp r2, r3
0047f280: bgt #0x47f244
0047f284: mov r0, r4
0047f288: bl #0x47ba10
0047f28c: mov r0, r4
0047f290: ldr r3, [r4]
0047f294: mov lr, pc
0047f298: ldr pc, [r3, #0x1c]
0047f29c: mov r0, #0
0047f2a0: pop {r4, pc}
0047f2a4: ldr r3, [r1, #0x14]
0047f2a8: ldr r2, [r0, #0x20]
0047f2ac: cmp r2, r3
0047f2b0: strlt r3, [r0, #0x20]
0047f2b4: blt #0x47f278
0047f2b8: b #0x47f244

_ZN20Objective_GatherLoot11handleEventEPK6IEventPK12EventManager 0x47ba5c 136
0047ba5c: push {r4, lr}
0047ba60: ldr r3, [r0, #0xc]
0047ba64: ldr r2, [r1, #0x18]
0047ba68: sub sp, sp, #8
0047ba6c: ldr r3, [r3, #0x24]
0047ba70: mov r4, r0
0047ba74: cmp r3, r2
0047ba78: beq #0x47ba88
0047ba7c: mov r0, #0
0047ba80: add sp, sp, #8
0047ba84: pop {r4, pc}
0047ba88: ldr r2, [r0, #0x10]
0047ba8c: ldr r3, [r1, #8]
0047ba90: cmp r2, r3
0047ba94: bne #0x47ba7c
0047ba98: str r1, [sp, #4]
0047ba9c: bl #0x47aba4
0047baa0: ldr r1, [sp, #4]
0047baa4: ldrb r3, [r1, #0x11]
0047baa8: cmp r3, #0
0047baac: moveq r3, #1
0047bab0: strbeq r3, [r1, #0x10]
0047bab4: ldreq r3, [r4, #0x20]
0047bab8: ldrne r3, [r1, #0x14]
0047babc: streq r3, [r1, #0x14]
0047bac0: ldr r2, [r4, #0xc]
0047bac4: strne r3, [r4, #0x20]
0047bac8: ldreq r3, [r4, #0x20]
0047bacc: ldr r2, [r2, #0x28]
0047bad0: cmp r2, r3
0047bad4: bgt #0x47ba7c
0047bad8: mov r0, r4
0047badc: bl #0x47ba10
0047bae0: b #0x47ba7c

_ZThn4_N12MenuWorldMap12InputHandler7onEventEPK6IEventPK12EventManager 0x4372b4 8
004372b4: sub r0, r0, #4
004372b8: b #0x4372bc

_ZN7Console7onEventEPK6IEventPK12EventManager 0x335b14 1152
00335b14: push {r4, r5, r6, lr}
00335b18: mov r5, r0
00335b1c: mov r4, r1
00335b20: bl #0x33271c
00335b24: ldrb r3, [r5, #4]
00335b28: ldr r6, [pc, #0x45c]
00335b2c: cmp r3, #0
00335b30: add r6, pc, r6
00335b34: bne #0x335b40
00335b38: mov r0, #0
00335b3c: pop {r4, r5, r6, pc}
00335b40: ldr r3, [r4]
00335b44: mov r0, r4
00335b48: mov lr, pc
00335b4c: ldr pc, [r3, #8]
00335b50: cmp r0, #0
00335b54: bne #0x335d00
00335b58: ldrb r3, [r4, #0x10]
00335b5c: cmp r3, #0
00335b60: beq #0x335b38
00335b64: ldr r1, [r4, #0xc]
00335b68: sub r1, r1, #0x30
00335b6c: cmp r1, #0x5a
00335b70: addls pc, pc, r1, lsl #2
00335b74: b #0x335b38
00335b78: b #0x335ee4
00335b7c: b #0x335ee4
00335b80: b #0x335ee4
00335b84: b #0x335ee4
00335b88: b #0x335ee4
00335b8c: b #0x335ee4
00335b90: b #0x335ee4
00335b94: b #0x335ee4
00335b98: b #0x335ee4
00335b9c: b #0x335ee4
00335ba0: b #0x335b38
00335ba4: b #0x335b38
00335ba8: b #0x335b38
00335bac: b #0x335b38
00335bb0: b #0x335b38
00335bb4: b #0x335b38
00335bb8: b #0x335b38
00335bbc: b #0x335b38
00335bc0: b #0x335b38
00335bc4: b #0x335b38
00335bc8: b #0x335b38
00335bcc: b #0x335b38
00335bd0: b #0x335b38
00335bd4: b #0x335b38
00335bd8: b #0x335b38
00335bdc: b #0x335b38
00335be0: b #0x335b38
00335be4: b #0x335b38
00335be8: b #0x335b38
00335bec: b #0x335b38
00335bf0: b #0x335b38
00335bf4: b #0x335b38
00335bf8: b #0x335b38
00335bfc: b #0x335b38
00335c00: b #0x335b38
00335c04: b #0x335b38
00335c08: b #0x335b38
00335c0c: b #0x335b38
00335c10: b #0x335b38
00335c14: b #0x335b38
00335c18: b #0x335b38
00335c1c: b #0x335b38
00335c20: b #0x335b38
00335c24: b #0x335b38
00335c28: b #0x335eb0
00335c2c: b #0x335cf8
00335c30: b #0x335cf8
00335c34: b #0x335b38
00335c38: b #0x335b38
00335c3c: b #0x335b38
00335c40: b #0x335b38
00335c44: b #0x335b38
00335c48: b #0x335b38
00335c4c: b #0x335b38
00335c50: b #0x335b38
00335c54: b #0x335b38
00335c58: b #0x335b38
00335c5c: b #0x335b38
00335c60: b #0x335b38
00335c64: b #0x335b38
00335c68: b #0x335b38
00335c6c: b #0x335b38
00335c70: b #0x335b38
00335c74: b #0x335b38
00335c78: b #0x335b38
00335c7c: b #0x335b38
00335c80: b #0x335b38
00335c84: b #0x335b38
00335c88: b #0x335b38
00335c8c: b #0x335b38
00335c90: b #0x335b38
00335c94: b #0x335b38
00335c98: b #0x335b38
00335c9c: b #0x335b38
00335ca0: b #0x335b38
00335ca4: b #0x335b38
00335ca8: b #0x335b38
00335cac: b #0x335b38
00335cb0: b #0x335b38
00335cb4: b #0x335b38
00335cb8: b #0x335b38
00335cbc: b #0x335b38
00335cc0: b #0x335b38
00335cc4: b #0x335b38
00335cc8: b #0x335b38
00335ccc: b #0x335b38
00335cd0: b #0x335b38
00335cd4: b #0x335e68
00335cd8: b #0x335e38
00335cdc: b #0x335ce4
00335ce0: b #0x335e84
00335ce4: ldr r3, [r5, #8]
00335ce8: cmp r3, #0
00335cec: subne r3, r3, #1
00335cf0: strne r3, [r5, #8]
00335cf4: bne #0x335df4
00335cf8: mov r0, #1
00335cfc: pop {r4, r5, r6, pc}
00335d00: ldr r3, [r4]
00335d04: mov r0, r4
00335d08: mov lr, pc
00335d0c: ldr pc, [r3, #8]
00335d10: cmp r0, #4
00335d14: bne #0x335d58
00335d18: ldrb r3, [r4, #0x10]
00335d1c: cmp r3, #0
00335d20: beq #0x335e0c
00335d24: ldr r3, [r5, #0x8c]
00335d28: cmp r3, #0
00335d2c: blt #0x335f5c
00335d30: ldr r2, [r4, #0xc]
00335d34: cmp r3, r2
00335d38: beq #0x335cf8
00335d3c: mov r3, #0
00335d40: strb r3, [r5, #0x86]
00335d44: mvn r3, #0
00335d48: str r3, [r5, #0x8c]
00335d4c: bl #0x3833e4
00335d50: mov r0, #1
00335d54: pop {r4, r5, r6, pc}
00335d58: ldr r3, [r4]
00335d5c: mov r0, r4
00335d60: mov lr, pc
00335d64: ldr pc, [r3, #8]
00335d68: cmp r0, #5
00335d6c: bne #0x335b38
00335d70: mov r2, #0
00335d74: strb r2, [r5, #0x86]
00335d78: ldrh r6, [r4, #0xa]
00335d7c: ldrsh r3, [r5, #0x84]
00335d80: ldrh r4, [r4, #8]
00335d84: sxth r1, r6
00335d88: rsb r3, r3, r1
00335d8c: add r1, r3, #0xf
00335d90: cmp r3, r2
00335d94: movlt r3, r1
00335d98: asrs r3, r3, #4
00335d9c: bne #0x335f00
00335da0: ldrsh r2, [r5, #0x82]
00335da4: sxth r1, r4
00335da8: movw r3, #0x6667
00335dac: rsb r2, r2, r1
00335db0: movt r3, #0x6666
00335db4: smull r1, r3, r3, r2
00335db8: asr r2, r2, #0x1f
00335dbc: rsb r3, r2, r3, asr #4
00335dc0: cmp r3, #0
00335dc4: beq #0x335cf8
00335dc8: ldr r2, [r5, #8]
00335dcc: strh r6, [r5, #0x84]
00335dd0: strh r4, [r5, #0x82]
00335dd4: add r3, r3, r2
00335dd8: cmp r3, #0
00335ddc: movle r3, #0
00335de0: strle r3, [r5, #8]
00335de4: ble #0x335df4
00335de8: cmp r3, #0x13
00335dec: movhs r3, #0x13
00335df0: str r3, [r5, #8]
00335df4: mov r3, #0
00335df8: mov r0, r5
00335dfc: str r3, [r5, #0xc]
00335e00: bl #0x334b1c
00335e04: mov r0, #1
00335e08: pop {r4, r5, r6, pc}
00335e0c: ldrb r2, [r5, #0x86]
00335e10: mvn r1, #0
00335e14: str r1, [r5, #0x8c]
00335e18: cmp r2, #0
00335e1c: beq #0x335cf8
00335e20: mov r0, r5
00335e24: strb r3, [r5, #0x86]
00335e28: mov r1, #1
00335e2c: bl #0x334d1c
00335e30: mov r0, #1
00335e34: pop {r4, r5, r6, pc}
00335e38: ldr r3, [r5, #0xc]
00335e3c: mov r0, r5
00335e40: add r3, r3, #1
00335e44: str r3, [r5, #0xc]
00335e48: bl #0x3307ac
00335e4c: ldr r3, [r5, #0xc]
00335e50: sub r2, r0, #1
00335e54: mov r0, #1
00335e58: cmp r3, r2
00335e5c: strls r3, [r5, #0xc]
00335e60: strhi r2, [r5, #0xc]
00335e64: pop {r4, r5, r6, pc}
00335e68: ldr r3, [r5, #0xc]
00335e6c: cmp r3, #0
00335e70: beq #0x335cf8
00335e74: sub r3, r3, #1
00335e78: str r3, [r5, #0xc]
00335e7c: mov r0, #1
00335e80: pop {r4, r5, r6, pc}
00335e84: ldr r3, [r5, #8]
00335e88: mov r2, #0
00335e8c: mov r0, r5
00335e90: add r3, r3, #1
00335e94: cmp r3, #0x13
00335e98: movhs r3, #0x13
00335e9c: str r2, [r5, #0xc]
00335ea0: str r3, [r5, #8]
00335ea4: bl #0x334b1c
00335ea8: mov r0, #1
00335eac: pop {r4, r5, r6, pc}
00335eb0: ldr r3, [pc, #0xd8]
00335eb4: ldr r0, [r6, r3]
00335eb8: bl #0x31f594
00335ebc: subs r3, r0, #0
00335ec0: beq #0x335cf8
00335ec4: ldr r2, [r3, #0x130]
00335ec8: cmp r2, #0x24
00335ecc: bne #0x335cf8
00335ed0: ldr r2, [r3, #0x130]
00335ed4: mov r0, #1
00335ed8: add r2, r2, r0
00335edc: str r2, [r3, #0x130]
00335ee0: pop {r4, r5, r6, pc}
00335ee4: ldrb r3, [r5, #4]
00335ee8: cmp r3, #0
00335eec: beq #0x335b38
00335ef0: mov r0, r5
00335ef4: bl #0x334d1c
00335ef8: mov r0, #1
00335efc: pop {r4, r5, r6, pc}
00335f00: ldr r1, [r5, #0xc]
00335f04: strh r6, [r5, #0x84]
00335f08: strh r4, [r5, #0x82]
00335f0c: add r3, r3, r1
00335f10: cmp r3, #0
00335f14: ble #0x335f80
00335f18: str r3, [r5, #0xc]
00335f1c: mov r0, r5
00335f20: bl #0x3307ac
00335f24: ldrsh r2, [r5, #0x82]
00335f28: sxth r1, r4
00335f2c: movw r3, #0x6667
00335f30: rsb r2, r2, r1
00335f34: movt r3, #0x6666
00335f38: smull r1, r3, r3, r2
00335f3c: ldr r1, [r5, #0xc]
00335f40: sub r0, r0, #1
00335f44: asr r2, r2, #0x1f
00335f48: cmp r1, r0
00335f4c: strls r1, [r5, #0xc]
00335f50: strhi r0, [r5, #0xc]
00335f54: rsb r3, r2, r3, asr #4
00335f58: b #0x335dc0
00335f5c: ldr r3, [r4, #0xc]
00335f60: mov r0, #1
00335f64: strb r0, [r5, #0x86]
00335f68: str r3, [r5, #0x8c]
00335f6c: ldrh r1, [r4, #8]
00335f70: strh r1, [r5, #0x82]
00335f74: ldrh r4, [r4, #0xa]
00335f78: strh r4, [r5, #0x84]
00335f7c: pop {r4, r5, r6, pc}
00335f80: str r2, [r5, #0xc]
00335f84: mov r0, #1
00335f88: pop {r4, r5, r6, pc}
00335f8c: rsbeq lr, r5, r0, ror #30
00335f90: strdeq r3, r4, [r0], -r4

_ZN16OnScreenKeyboard7onEventEPK6IEventPK12EventManager 0x51b444 324
0051b444: push {r4, r5, r6, lr}
0051b448: mov r0, r1
0051b44c: ldr r3, [r1]
0051b450: mov r5, r1
0051b454: mov lr, pc
0051b458: ldr pc, [r3, #8]
0051b45c: ldr r4, [pc, #0x108]
0051b460: subs r2, r0, #0
0051b464: add r4, pc, r4
0051b468: bne #0x51b4f4
0051b46c: ldr r3, [r5, #0xc]
0051b470: cmp r3, #0xd
0051b474: beq #0x51b4e0
0051b478: cmp r3, #8
0051b47c: beq #0x51b4fc
0051b480: ldr r3, [r5, #8]
0051b484: cmp r3, #0
0051b488: beq #0x51b4ec
0051b48c: ldrb r2, [r5, #0x10]
0051b490: cmp r2, #0
0051b494: beq #0x51b4ec
0051b498: ldr r2, [pc, #0xd0]
0051b49c: ldr r1, [r4, r2]
0051b4a0: ldr r2, [pc, #0xcc]
0051b4a4: ldr r0, [r1]
0051b4a8: ldr r2, [r4, r2]
0051b4ac: sub r0, r0, #1
0051b4b0: ldr r1, [r2]
0051b4b4: cmp r0, r1
0051b4b8: bls #0x51b4ec
0051b4bc: ldr ip, [pc, #0xb4]
0051b4c0: mov r0, #1
0051b4c4: ldr ip, [r4, ip]
0051b4c8: ldr ip, [ip]
0051b4cc: strb r3, [ip, r1]
0051b4d0: ldr r3, [r2]
0051b4d4: add r3, r3, r0
0051b4d8: str r3, [r2]
0051b4dc: pop {r4, r5, r6, pc}
0051b4e0: ldrb r1, [r5, #0x10]
0051b4e4: cmp r1, #0
0051b4e8: beq #0x51b53c
0051b4ec: mov r0, #1
0051b4f0: pop {r4, r5, r6, pc}
0051b4f4: mov r0, #0
0051b4f8: pop {r4, r5, r6, pc}
0051b4fc: ldrb r3, [r5, #0x10]
0051b500: cmp r3, #0
0051b504: beq #0x51b4ec
0051b508: ldr r3, [pc, #0x64]
0051b50c: ldr r3, [r4, r3]
0051b510: ldr r1, [r3]
0051b514: cmp r1, #0
0051b518: beq #0x51b4ec
0051b51c: sub r1, r1, #1
0051b520: str r1, [r3]
0051b524: ldr r3, [pc, #0x4c]
0051b528: mov r0, #1
0051b52c: ldr r3, [r4, r3]
0051b530: ldr r3, [r3]
0051b534: strb r2, [r3, r1]
0051b538: pop {r4, r5, r6, pc}
0051b53c: ldr r3, [pc, #0x38]
0051b540: ldr r2, [pc, #0x38]
0051b544: ldr r3, [r4, r3]
0051b548: ldr r2, [r4, r2]
0051b54c: ldr r0, [r3, #0x14]
0051b550: bl #0x33811c
0051b554: ldr r2, [pc, #0x28]
0051b558: mov r3, #1
0051b55c: mov r0, r3
0051b560: ldr r2, [r4, r2]
0051b564: strb r3, [r2]
0051b568: pop {r4, r5, r6, pc}
0051b56c: subeq sb, r7, ip, lsr #12
0051b570: muleq r0, r0, fp
0051b574: strheq r1, [r0], -r0
0051b578: andeq r2, r0, r4, ror #13
0051b57c: strdeq r3, r4, [r0], -r4
0051b580: andeq r2, r0, r0, lsl #22
0051b584: andeq r2, r0, r0, lsr sb

_ZN11Application25RegisterForIrrlichtEventsEPN6glitch14IEventReceiverE 0x32fa7c 56
0032fa7c: push {r4, r5, r6, lr}
0032fa80: subs r6, r1, #0
0032fa84: mov r4, r0
0032fa88: beq #0x32fab0
0032fa8c: add r5, r0, #8
0032fa90: mov r0, r5
0032fa94: bl #0x32fa5c
0032fa98: str r6, [r0, #8]
0032fa9c: ldr r3, [r4, #0xc]
0032faa0: str r5, [r0]
0032faa4: str r3, [r0, #4]
0032faa8: str r0, [r3]
0032faac: str r0, [r4, #0xc]
0032fab0: pop {r4, r5, r6, pc}

_ZN14IEventReceiverD1Ev 0x32fdbc 4
0032fdbc: bx lr

_ZN16GameEventManager6UnloadEv 0x479968 84
00479968: push {r4, r5, r6, lr}
0047996c: ldr r4, [r0]
00479970: ldr r3, [r0, #4]
00479974: mov r6, r0
00479978: cmp r4, r3
0047997c: beq #0x4799b8
00479980: ldr r5, [r4]
00479984: add r4, r4, #4
00479988: cmp r5, #0
0047998c: beq #0x4799a4
00479990: mov r0, r5
00479994: bl #0x47957c
00479998: mov r0, r5
0047999c: bl #0x310440
004799a0: ldr r3, [r6, #4]
004799a4: cmp r4, r3
004799a8: bne #0x479980
004799ac: ldr r3, [r6]
004799b0: cmp r4, r3
004799b4: strne r3, [r6, #4]
004799b8: pop {r4, r5, r6, pc}

_ZN31ObjectiveTemplate_KillCharacterIN7Structs25v2QuestClearEnemyTemplateE21QE_ClearEnemyTemplate16TestCharTemplate18Objective_SavedQtyE11handleEventEPK6IEventPK12EventManager 0x47f350 148
0047f350: push {r4, lr}
0047f354: ldr r3, [r0, #0x24]
0047f358: ldr r2, [r1, #0x18]
0047f35c: mov r4, r0
0047f360: ldr r3, [r3, #0x24]
0047f364: cmp r3, r2
0047f368: beq #0x47f374
0047f36c: mov r0, #0
0047f370: pop {r4, pc}
0047f374: ldrb r3, [r1, #0x11]
0047f378: cmp r3, #0
0047f37c: bne #0x47f3cc
0047f380: ldr r3, [r0, #0x20]
0047f384: add r3, r3, #1
0047f388: str r3, [r0, #0x20]
0047f38c: mov r3, #1
0047f390: strb r3, [r1, #0x10]
0047f394: ldr r3, [r0, #0x20]
0047f398: str r3, [r1, #0x14]
0047f39c: ldr r3, [r0, #0x20]
0047f3a0: ldr r2, [r4, #0x2c]
0047f3a4: cmp r2, r3
0047f3a8: bgt #0x47f36c
0047f3ac: mov r0, r4
0047f3b0: bl #0x47ba10
0047f3b4: mov r0, r4
0047f3b8: ldr r3, [r4]
0047f3bc: mov lr, pc
0047f3c0: ldr pc, [r3, #0x1c]
0047f3c4: mov r0, #0
0047f3c8: pop {r4, pc}
0047f3cc: ldr r3, [r1, #0x14]
0047f3d0: ldr r2, [r0, #0x20]
0047f3d4: cmp r2, r3
0047f3d8: strlt r3, [r0, #0x20]
0047f3dc: blt #0x47f3a0
0047f3e0: b #0x47f36c

_ZN31ObjectiveTemplate_KillCharacterIN7Structs24v2QuestKillEnemyTemplateE20QE_KillEnemyTemplate16TestCharTemplate18Objective_SavedQtyE11handleEventEPK6IEventPK12EventManager 0x47f2bc 148
0047f2bc: push {r4, lr}
0047f2c0: ldr r3, [r0, #0x24]
0047f2c4: ldr r2, [r1, #0x18]
0047f2c8: mov r4, r0
0047f2cc: ldr r3, [r3, #0x20]
0047f2d0: cmp r3, r2
0047f2d4: beq #0x47f2e0
0047f2d8: mov r0, #0
0047f2dc: pop {r4, pc}
0047f2e0: ldrb r3, [r1, #0x11]
0047f2e4: cmp r3, #0
0047f2e8: bne #0x47f338
0047f2ec: ldr r3, [r0, #0x20]
0047f2f0: add r3, r3, #1
0047f2f4: str r3, [r0, #0x20]
0047f2f8: mov r3, #1
0047f2fc: strb r3, [r1, #0x10]
0047f300: ldr r3, [r0, #0x20]
0047f304: str r3, [r1, #0x14]
0047f308: ldr r3, [r0, #0x20]
0047f30c: ldr r2, [r4, #0x2c]
0047f310: cmp r2, r3
0047f314: bgt #0x47f2d8
0047f318: mov r0, r4
0047f31c: bl #0x47ba10
0047f320: mov r0, r4
0047f324: ldr r3, [r4]
0047f328: mov lr, pc
0047f32c: ldr pc, [r3, #0x1c]
0047f330: mov r0, #0
0047f334: pop {r4, pc}
0047f338: ldr r3, [r1, #0x14]
0047f33c: ldr r2, [r0, #0x20]
0047f340: cmp r2, r3
0047f344: strlt r3, [r0, #0x20]
0047f348: blt #0x47f30c
0047f34c: b #0x47f2d8

_ZNSt4priv10_List_baseIN12EventManager17DelayedDetachInfoESaIS2_EE5clearEv 0x3383b4 64
003383b4: push {r4, r5, r6, lr}
003383b8: mov r5, r0
003383bc: ldr r0, [r0]
003383c0: cmp r0, r5
003383c4: bne #0x3383d0
003383c8: b #0x3383e8
003383cc: mov r0, r4
003383d0: ldr r4, [r0]
003383d4: mov r1, #0x10
003383d8: bl #0x708f00
003383dc: cmp r4, r5
003383e0: bne #0x3383cc
003383e4: mov r0, r5
003383e8: str r0, [r5, #4]
003383ec: str r0, [r5]
003383f0: pop {r4, r5, r6, pc}

_ZN12EventManager6AttachEiP14IEventReceiveri 0x338da0 284
00338da0: push {r4, r5, r6, r7, lr}
00338da4: ldr r4, [r0, #0xc]
00338da8: sub sp, sp, #0xc
00338dac: str r1, [sp, #4]
00338db0: cmp r4, #0
00338db4: mov r5, r2
00338db8: mov r6, r3
00338dbc: add r0, r0, #8
00338dc0: beq #0x338e6c
00338dc4: mov r2, r0
00338dc8: b #0x338dd0
00338dcc: mov r4, r3
00338dd0: ldr r3, [r4, #0x10]
00338dd4: cmp r3, r1
00338dd8: ldrlt r3, [r4, #0xc]
00338ddc: ldrge r3, [r4, #8]
00338de0: movlt r4, r2
00338de4: mov r2, r4
00338de8: cmp r3, #0
00338dec: bne #0x338dcc
00338df0: cmp r0, r4
00338df4: beq #0x338e7c
00338df8: ldr r3, [r4, #0x10]
00338dfc: cmp r3, r1
00338e00: bgt #0x338e6c
00338e04: cmp r0, r4
00338e08: beq #0x338e7c
00338e0c: mov r2, r4
00338e10: ldr r7, [r2, #0x14]!
00338e14: b #0x338e28
00338e18: ldr r3, [r7, #8]
00338e1c: cmp r5, r3
00338e20: beq #0x338e74
00338e24: ldr r7, [r7]
00338e28: cmp r7, r2
00338e2c: bne #0x338e18
00338e30: mov r0, r7
00338e34: bl #0x33860c
00338e38: mov r2, #0
00338e3c: strb r2, [r0, #0x10]
00338e40: str r6, [r0, #0xc]
00338e44: str r5, [r0, #8]
00338e48: ldr r2, [r4, #0x18]
00338e4c: mov r3, r0
00338e50: str r7, [r0]
00338e54: str r2, [r3, #4]
00338e58: mov r0, #1
00338e5c: str r3, [r2]
00338e60: str r3, [r4, #0x18]
00338e64: add sp, sp, #0xc
00338e68: pop {r4, r5, r6, r7, pc}
00338e6c: mov r4, r0
00338e70: b #0x338e04
00338e74: mov r0, #0
00338e78: b #0x338e64
00338e7c: add r1, sp, #4
00338e80: bl #0x338c98
00338e84: mov r4, r0
00338e88: bl #0x33860c
00338e8c: mov r2, #0
00338e90: strb r2, [r0, #0x10]
00338e94: str r6, [r0, #0xc]
00338e98: str r5, [r0, #8]
00338e9c: ldr r2, [r4, #4]
00338ea0: mov r3, r0
00338ea4: str r4, [r0]
00338ea8: str r2, [r3, #4]
00338eac: mov r0, #1
00338eb0: str r3, [r2]
00338eb4: str r3, [r4, #4]
00338eb8: b #0x338e64

_ZN6glitch7CLoggerC2EPNS_14IEventReceiverE 0x6a08b4 60
006a08b4: ldr r2, [pc, #0x2c]
006a08b8: ldr ip, [pc, #0x2c]
006a08bc: str r4, [sp, #-4]!
006a08c0: add r2, pc, r2
006a08c4: ldr ip, [r2, ip]
006a08c8: mov r4, #1
006a08cc: str r1, [r0, #0xc]
006a08d0: add ip, ip, #8
006a08d4: str ip, [r0]
006a08d8: str r4, [r0, #8]
006a08dc: str r4, [r0, #4]
006a08e0: ldm sp!, {r4}
006a08e4: bx lr

_ZN11ZoomHandler7onEventEPK6IEventPK12EventManager 0x382bdc 1192
00382bdc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00382be0: ldr r3, [r0, #0x20]
00382be4: sub sp, sp, #0x1c
00382be8: mov r5, r0
00382bec: cmp r3, #0
00382bf0: mov r4, r1
00382bf4: beq #0x382c04
00382bf8: ldrb r3, [r3, #0x85]
00382bfc: cmp r3, #0
00382c00: bne #0x382c14
00382c04: mov r5, #0
00382c08: mov r0, r5
00382c0c: add sp, sp, #0x1c
00382c10: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00382c14: ldr r3, [r1]
00382c18: mov r0, r1
00382c1c: mov lr, pc
00382c20: ldr pc, [r3, #8]
00382c24: cmp r0, #4
00382c28: bne #0x382d18
00382c2c: ldrb r3, [r4, #0x10]
00382c30: cmp r3, #0
00382c34: bne #0x382c9c
00382c38: ldr r3, [r5, #0xc]
00382c3c: cmp r3, #0
00382c40: beq #0x382d04
00382c44: add r0, r5, #8
00382c48: ldr ip, [r4, #0xc]
00382c4c: mov r1, r0
00382c50: b #0x382c58
00382c54: mov r3, r2
00382c58: ldr r2, [r3, #0x10]
00382c5c: cmp r2, ip
00382c60: ldrlt r2, [r3, #0xc]
00382c64: ldrge r2, [r3, #8]
00382c68: movlt r3, r1
00382c6c: mov r1, r3
00382c70: cmp r2, #0
00382c74: bne #0x382c54
00382c78: cmp r0, r3
00382c7c: beq #0x382d04
00382c80: ldr r2, [r3, #0x10]
00382c84: cmp r2, ip
00382c88: bgt #0x382d04
00382c8c: add r1, sp, #0x18
00382c90: str r3, [r1, #-4]!
00382c94: bl #0x3824a0
00382c98: b #0x382d04
00382c9c: ldr r3, [r5, #0x20]
00382ca0: ldrb r3, [r3, #0x85]
00382ca4: cmp r3, #0
00382ca8: bne #0x382f18
00382cac: add r7, r4, #0xc
00382cb0: add r6, r5, #8
00382cb4: mov r1, r7
00382cb8: mov r0, r6
00382cbc: bl #0x382b34
00382cc0: ldrh r3, [r4, #8]
00382cc4: mov r1, r7
00382cc8: strh r3, [r0]
00382ccc: mov r0, r6
00382cd0: bl #0x382b34
00382cd4: ldrh r3, [r4, #0xa]
00382cd8: mov r1, r7
00382cdc: strh r3, [r0, #2]
00382ce0: mov r0, r6
00382ce4: bl #0x382b34
00382ce8: ldrh r3, [r4, #8]
00382cec: mov r1, r7
00382cf0: strh r3, [r0, #4]
00382cf4: mov r0, r6
00382cf8: bl #0x382b34
00382cfc: ldrh r4, [r4, #0xa]
00382d00: strh r4, [r0, #6]
00382d04: ldr r5, [r5, #0x18]
00382d08: cmp r5, #1
00382d0c: movls r5, #0
00382d10: movhi r5, #1
00382d14: b #0x382c08
00382d18: ldr r3, [r4]
00382d1c: mov r0, r4
00382d20: mov lr, pc
00382d24: ldr pc, [r3, #8]
00382d28: cmp r0, #5
00382d2c: bne #0x382c04
00382d30: add r7, r5, #8
00382d34: add r8, r4, #0xc
00382d38: mov r0, r7
00382d3c: mov r1, r8
00382d40: bl #0x382b34
00382d44: ldrsh r3, [r0]
00382d48: cmp r3, #0
00382d4c: beq #0x382f44
00382d50: mov r1, r8
00382d54: mov r0, r7
00382d58: bl #0x382b34
00382d5c: ldrh r3, [r4, #8]
00382d60: mov r1, r8
00382d64: strh r3, [r0, #4]
00382d68: mov r0, r7
00382d6c: bl #0x382b34
00382d70: ldrh r3, [r4, #0xa]
00382d74: strh r3, [r0, #6]
00382d78: ldr r3, [r5, #0x18]
00382d7c: cmp r3, #1
00382d80: bls #0x382f88
00382d84: ldr r6, [r5, #0x10]
00382d88: ldrsh r0, [r6, #0x14]
00382d8c: bl #0x30e964
00382d90: str r0, [sp, #0xc]
00382d94: ldrsh r0, [r6, #0x16]
00382d98: bl #0x30e964
00382d9c: mov fp, r0
00382da0: ldrsh r0, [r6, #0x18]
00382da4: bl #0x30e964
00382da8: str r0, [sp, #8]
00382dac: ldrsh r0, [r6, #0x1a]
00382db0: bl #0x30e964
00382db4: mov r3, r0
00382db8: ldr r0, [r6, #0xc]
00382dbc: cmp r0, #0
00382dc0: mov r1, r0
00382dc4: bne #0x382dd0
00382dc8: b #0x383044
00382dcc: mov r1, r2
00382dd0: ldr r2, [r1, #8]
00382dd4: cmp r2, #0
00382dd8: bne #0x382dcc
00382ddc: mov r6, r1
00382de0: ldrsh r0, [r6, #0x14]
00382de4: str r3, [sp, #4]
00382de8: bl #0x30e964
00382dec: mov sl, r0
00382df0: ldrsh r0, [r6, #0x16]
00382df4: bl #0x30e964
00382df8: mov r1, sl
00382dfc: mov sb, r0
00382e00: ldr r0, [sp, #0xc]
00382e04: bl #0x30e3ac
00382e08: mov r1, sb
00382e0c: mov sl, r0
00382e10: mov r0, fp
00382e14: bl #0x30e3ac
00382e18: mov r1, sl
00382e1c: mov sb, r0
00382e20: mov r0, sl
00382e24: bl #0x30ed6c
00382e28: mov r1, sb
00382e2c: mov sl, r0
00382e30: mov r0, sb
00382e34: bl #0x30ed6c
00382e38: mov r1, r0
00382e3c: mov r0, sl
00382e40: bl #0x30eba4
00382e44: bl #0x30e8a4
00382e48: bl #0x30e1c0
00382e4c: bl #0x30e6a0
00382e50: mov sl, r0
00382e54: ldrsh r0, [r6, #0x18]
00382e58: bl #0x30e964
00382e5c: mov sb, r0
00382e60: ldrsh r0, [r6, #0x1a]
00382e64: bl #0x30e964
00382e68: mov r1, sb
00382e6c: mov r6, r0
00382e70: ldr r0, [sp, #8]
00382e74: bl #0x30e3ac
00382e78: ldr r3, [sp, #4]
00382e7c: mov sb, r0
00382e80: mov r1, r6
00382e84: mov r0, r3
00382e88: bl #0x30e3ac
00382e8c: mov r1, sb
00382e90: mov fp, r0
00382e94: mov r0, sb
00382e98: bl #0x30ed6c
00382e9c: mov r1, fp
00382ea0: mov r6, r0
00382ea4: mov r0, fp
00382ea8: bl #0x30ed6c
00382eac: mov r1, r0
00382eb0: mov r0, r6
00382eb4: bl #0x30eba4
00382eb8: bl #0x30e8a4
00382ebc: bl #0x30e1c0
00382ec0: ldr r6, [r5, #0x20]
00382ec4: bl #0x30e6a0
00382ec8: mov r1, sl
00382ecc: bl #0x30e3ac
00382ed0: ldr r1, [r5, #0x28]
00382ed4: ldr r5, [r6, #0x88]
00382ed8: bl #0x30ed6c
00382edc: mov r1, r5
00382ee0: bl #0x30eba4
00382ee4: mov r5, #1
00382ee8: str r0, [r6, #0x88]
00382eec: mov r1, r8
00382ef0: mov r0, r7
00382ef4: bl #0x382b34
00382ef8: ldrh r3, [r4, #8]
00382efc: mov r1, r8
00382f00: strh r3, [r0]
00382f04: mov r0, r7
00382f08: bl #0x382b34
00382f0c: ldrh r4, [r4, #0xa]
00382f10: strh r4, [r0, #2]
00382f14: b #0x382c08
00382f18: bl #0x453da4
00382f1c: ldrb r3, [r0, #0x1e4]
00382f20: cmp r3, #0
00382f24: bne #0x382d04
00382f28: bl #0x453da4
00382f2c: ldrsh r1, [r4, #8]
00382f30: ldrsh r2, [r4, #0xa]
00382f34: bl #0x4533ec
00382f38: cmp r0, #0
00382f3c: bne #0x382cac
00382f40: b #0x382d04
00382f44: mov r0, r7
00382f48: mov r1, r8
00382f4c: bl #0x382b34
00382f50: ldrsh r3, [r0, #2]
00382f54: cmp r3, #0
00382f58: bne #0x382d50
00382f5c: mov r1, r8
00382f60: mov r0, r7
00382f64: bl #0x382b34
00382f68: ldrh r3, [r4, #8]
00382f6c: mov r1, r8
00382f70: strh r3, [r0]
00382f74: mov r0, r7
00382f78: bl #0x382b34
00382f7c: ldrh r3, [r4, #0xa]
00382f80: strh r3, [r0, #2]
00382f84: b #0x382d50
00382f88: ldr r3, [r5, #0x20]
00382f8c: ldrb r3, [r3, #0x85]
00382f90: cmp r3, #0
00382f94: moveq r5, r3
00382f98: beq #0x382eec
00382f9c: mov r1, r8
00382fa0: mov r0, r7
00382fa4: bl #0x382b34
00382fa8: mov r6, r0
00382fac: ldrsh r0, [r0]
00382fb0: bl #0x30e964
00382fb4: mov fp, r0
00382fb8: ldrsh r0, [r6, #2]
00382fbc: bl #0x30e964
00382fc0: mov r6, r0
00382fc4: ldrsh r0, [r4, #8]
00382fc8: bl #0x30e964
00382fcc: mov sl, r0
00382fd0: ldrsh r0, [r4, #0xa]
00382fd4: bl #0x30e964
00382fd8: mov r1, fp
00382fdc: mov sb, r0
00382fe0: mov r0, sl
00382fe4: bl #0x30e3ac
00382fe8: mov r1, #0x42000000
00382fec: add r1, r1, #0x480000
00382ff0: bl #0x30ed6c
00382ff4: mov r1, r6
00382ff8: mov sl, r0
00382ffc: mov r0, sb
00383000: bl #0x30e3ac
00383004: mov r1, #0x42000000
00383008: add r1, r1, #0x480000
0038300c: add r0, r0, #0x80000000
00383010: bl #0x30ed6c
00383014: ldr r5, [r5, #0x20]
00383018: mov r1, r0
0038301c: ldr r0, [r5, #0x9c]
00383020: bl #0x30e3ac
00383024: mov r1, sl
00383028: mov r6, r0
0038302c: ldr r0, [r5, #0x98]
00383030: bl #0x30e3ac
00383034: str r6, [r5, #0x9c]
00383038: str r0, [r5, #0x98]
0038303c: mov r5, #0
00383040: b #0x382eec
00383044: ldr r2, [r6, #4]
00383048: ldr r1, [r2, #0xc]
0038304c: cmp r1, r6
00383050: beq #0x38305c
00383054: b #0x383078
00383058: mov r2, r1
0038305c: ldr r1, [r2, #4]
00383060: ldr r0, [r1, #0xc]
00383064: cmp r2, r0
00383068: beq #0x383058
0038306c: mov r6, r2
00383070: ldr r0, [r2, #0xc]
00383074: mov r2, r1
00383078: cmp r2, r0
0038307c: movne r6, r2
00383080: b #0x382de0

_ZN15v2EmuController7onEventEPK6IEventPK12EventManager 0x406450 96
00406450: push {r4, r5, r6, lr}
00406454: mov r6, r0
00406458: ldr r3, [r1]
0040645c: mov r0, r1
00406460: mov r4, r1
00406464: mov r5, r2
00406468: mov lr, pc
0040646c: ldr pc, [r3, #8]
00406470: cmp r0, #0
00406474: bne #0x40648c
00406478: mov r0, r6
0040647c: mov r1, r4
00406480: mov r2, r5
00406484: pop {r4, r5, r6, lr}
00406488: b #0x4060c0
0040648c: cmp r0, #2
00406490: beq #0x40649c
00406494: mov r0, #0
00406498: pop {r4, r5, r6, pc}
0040649c: mov r0, r6
004064a0: mov r1, r4
004064a4: mov r2, r5
004064a8: pop {r4, r5, r6, lr}
004064ac: b #0x405f48

_ZN16GameEventManager6ReInitEv 0x47992c 60
0047992c: push {r4, r5, r6, lr}
00479930: ldr r4, [r0]
00479934: ldr r3, [r0, #4]
00479938: mov r6, r0
0047993c: cmp r4, r3
00479940: beq #0x479964
00479944: ldr r5, [r4], #4
00479948: mov r0, r5
0047994c: bl #0x479538
00479950: mov r0, r5
00479954: bl #0x4794fc
00479958: ldr r3, [r6, #4]
0047995c: cmp r4, r3
00479960: bne #0x479944
00479964: pop {r4, r5, r6, pc}

_ZN6glitch3gui15CGUIEnvironment20setUserEventReceiverEPNS_14IEventReceiverE 0x535768 8
00535768: str r1, [r0, #0x1c4]
0053576c: bx lr

_ZNSt4priv10_List_baseIPK6IEventSaIS3_EE5clearEv 0x338374 64
00338374: push {r4, r5, r6, lr}
00338378: mov r5, r0
0033837c: ldr r0, [r0]
00338380: cmp r0, r5
00338384: bne #0x338390
00338388: b #0x3383a8
0033838c: mov r0, r4
00338390: ldr r4, [r0]
00338394: mov r1, #0xc
00338398: bl #0x708f00
0033839c: cmp r4, r5
003383a0: bne #0x33838c
003383a4: mov r0, r5
003383a8: str r0, [r5, #4]
003383ac: str r0, [r5]
003383b0: pop {r4, r5, r6, pc}

_ZN16GameEventManager4LoadEv 0x479e5c 216
00479e5c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00479e60: ldm r0, {r3, sl}
00479e64: ldr r4, [pc, #0xb8]
00479e68: sub sp, sp, #0x14
00479e6c: rsb sl, r3, sl
00479e70: asrs sl, sl, #2
00479e74: mov r5, r0
00479e78: add r4, pc, r4
00479e7c: beq #0x479e88
00479e80: add sp, sp, #0x14
00479e84: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00479e88: ldr r3, [pc, #0x98]
00479e8c: add r2, sp, #0x10
00479e90: str sl, [r2, #-4]!
00479e94: ldr r3, [r4, r3]
00479e98: ldr r6, [r3]
00479e9c: mov r1, r6
00479ea0: bl #0x479e18
00479ea4: cmp r6, #0
00479ea8: beq #0x479e80
00479eac: ldr r3, [pc, #0x78]
00479eb0: ldr r2, [pc, #0x78]
00479eb4: mov r7, sl
00479eb8: ldr fp, [r4, r3]
00479ebc: str r2, [sp, #4]
00479ec0: mov r1, #0
00479ec4: mov r0, #0x20
00479ec8: ldr sb, [fp]
00479ecc: bl #0x310570
00479ed0: mov r8, r0
00479ed4: bl #0x4795ac
00479ed8: ldr r2, [sp, #4]
00479edc: add sb, sb, sl
00479ee0: mov r1, sb
00479ee4: ldr r3, [r4, r2]
00479ee8: mov r0, r8
00479eec: add sl, sl, #0x18
00479ef0: ldr r3, [r3]
00479ef4: ldr r3, [r3, r7, lsl #2]
00479ef8: str r7, [r8, #4]
00479efc: str r3, [r8, #8]
00479f00: bl #0x479568
00479f04: mov r0, r8
00479f08: bl #0x479538
00479f0c: ldr r3, [r5]
00479f10: str r8, [r3, r7, lsl #2]
00479f14: add r7, r7, #1
00479f18: cmp r7, r6
00479f1c: bne #0x479ec0
00479f20: b #0x479e80
00479f24: subseq sl, r1, r8, lsl ip
00479f28: andeq r0, r0, r0, ror #16
00479f2c: muleq r0, r8, ip
00479f30: ldrdeq r3, r4, [r0], -r0

_ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIN12EventManager12ReceiverInfoESaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE13insert_uniqueERKSA_ 0x33879c 392
0033879c: push {r4, r5, r6, lr}
003387a0: ldr ip, [r1, #4]
003387a4: sub sp, sp, #0x10
003387a8: mov r4, r0
003387ac: cmp ip, #0
003387b0: mov r3, r2
003387b4: moveq ip, r1
003387b8: beq #0x338814
003387bc: ldr r6, [r2]
003387c0: b #0x3387c8
003387c4: mov ip, r2
003387c8: ldr r0, [ip, #0x10]
003387cc: mov r5, #1
003387d0: cmp r0, r6
003387d4: ldrgt r2, [ip, #8]
003387d8: ldrle r2, [ip, #0xc]
003387dc: movle r5, #0
003387e0: cmp r2, #0
003387e4: bne #0x3387c4
003387e8: cmp r5, #0
003387ec: moveq r5, ip
003387f0: bne #0x338814
003387f4: cmp r6, r0
003387f8: movle r3, #0
003387fc: strle r5, [r4]
00338800: strble r3, [r4, #4]
00338804: bgt #0x33887c
00338808: mov r0, r4
0033880c: add sp, sp, #0x10
00338810: pop {r4, r5, r6, pc}
00338814: ldr r2, [r1, #8]
00338818: cmp ip, r2
0033881c: beq #0x3388fc
00338820: ldrb r2, [ip]
00338824: cmp r2, #0
00338828: bne #0x33883c
0033882c: ldr r2, [ip, #4]
00338830: ldr r2, [r2, #4]
00338834: cmp ip, r2
00338838: beq #0x3388e8
0033883c: ldr r0, [ip, #8]
00338840: cmp r0, #0
00338844: bne #0x338850
00338848: b #0x3388a8
0033884c: mov r0, r2
00338850: ldr r2, [r0, #0xc]
00338854: cmp r2, #0
00338858: bne #0x33884c
0033885c: ldr r6, [r3]
00338860: mov r5, r0
00338864: ldr r0, [r0, #0x10]
00338868: cmp r6, r0
0033886c: movle r3, #0
00338870: strle r5, [r4]
00338874: strble r3, [r4, #4]
00338878: ble #0x338808
0033887c: mov r2, ip
00338880: add r0, sp, #8
00338884: mov ip, #0
00338888: str ip, [sp, #4]
0033888c: str ip, [sp]
00338890: bl #0x3386c4
00338894: ldr r3, [sp, #8]
00338898: mov r2, #1
0033889c: strb r2, [r4, #4]
003388a0: str r3, [r4]
003388a4: b #0x338808
003388a8: ldr r2, [ip, #4]
003388ac: ldr r0, [r2, #8]
003388b0: cmp ip, r0
003388b4: movne r5, r2
003388b8: ldrne r6, [r3]
003388bc: ldrne r0, [r2, #0x10]
003388c0: beq #0x3388cc
003388c4: b #0x3387f4
003388c8: mov r2, r5
003388cc: ldr r5, [r2, #4]
003388d0: ldr r0, [r5, #8]
003388d4: cmp r0, r2
003388d8: beq #0x3388c8
003388dc: ldr r6, [r3]
003388e0: ldr r0, [r5, #0x10]
003388e4: b #0x3387f4
003388e8: ldr r2, [ip, #0xc]
003388ec: ldr r6, [r3]
003388f0: mov r5, r2
003388f4: ldr r0, [r2, #0x10]
003388f8: b #0x3387f4
003388fc: mov r2, ip
00338900: mov lr, #0
00338904: add r0, sp, #0xc
00338908: stm sp, {ip, lr}
0033890c: bl #0x3386c4
00338910: ldr r3, [sp, #0xc]
00338914: mov r2, #1
00338918: strb r2, [r4, #4]
0033891c: str r3, [r4]
00338920: b #0x338808

_ZN12EventManager6DetachEiP14IEventReceiver 0x33811c 172
0033811c: push {r4, lr}
00338120: ldr r3, [r0, #0xc]
00338124: add r0, r0, #8
00338128: cmp r3, #0
0033812c: beq #0x3381a0
00338130: mov r4, r0
00338134: b #0x33813c
00338138: mov r3, ip
0033813c: ldr ip, [r3, #0x10]
00338140: cmp ip, r1
00338144: ldrlt ip, [r3, #0xc]
00338148: ldrge ip, [r3, #8]
0033814c: movlt r3, r4
00338150: mov r4, r3
00338154: cmp ip, #0
00338158: bne #0x338138
0033815c: cmp r0, r3
00338160: beq #0x338198
00338164: ldr ip, [r3, #0x10]
00338168: cmp ip, r1
0033816c: bgt #0x3381a0
00338170: cmp r0, r3
00338174: ldrne r0, [r3, #0x14]!
00338178: bne #0x338190
0033817c: b #0x338198
00338180: ldr r1, [r0, #8]
00338184: cmp r1, r2
00338188: beq #0x3381a8
0033818c: ldr r0, [r0]
00338190: cmp r3, r0
00338194: bne #0x338180
00338198: mov r0, #0
0033819c: pop {r4, pc}
003381a0: mov r3, r0
003381a4: b #0x338170
003381a8: ldr r3, [r0]
003381ac: ldr r2, [r0, #4]
003381b0: mov r1, #0x14
003381b4: str r3, [r2]
003381b8: str r2, [r3, #4]
003381bc: bl #0x708f00
003381c0: mov r0, #1
003381c4: pop {r4, pc}

_ZN11MenuManager7onEventEPK6IEventPK12EventManager 0x430ad4 2388
00430ad4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00430ad8: ldr r4, [pc, #0x888]
00430adc: ldr r6, [pc, #0x888]
00430ae0: mov r5, r0
00430ae4: add r4, pc, r4
00430ae8: ldr r3, [r4, r6]
00430aec: ldr r0, [pc, #0x87c]
00430af0: ldr sb, [pc, #0x87c]
00430af4: ldr r3, [r3]
00430af8: sub sp, sp, #0xc4
00430afc: add r0, pc, r0
00430b00: str r3, [sp, #0xbc]
00430b04: mov r7, r1
00430b08: bl #0x3136b4
00430b0c: ldr sl, [r4, sb]
00430b10: add r8, sp, #0xa4
00430b14: mov r0, sl
00430b18: bl #0x337888
00430b1c: ldr r1, [pc, #0x854]
00430b20: add r2, sp, #0x70
00430b24: mov r0, r8
00430b28: add r1, pc, r1
00430b2c: bl #0x3140ec
00430b30: mov r0, sl
00430b34: mov r1, r8
00430b38: bl #0x337a88
00430b3c: cmp r0, #0
00430b40: beq #0x430b7c
00430b44: mov r0, r8
00430b48: bl #0x318254
00430b4c: mov r5, #0
00430b50: ldr r0, [pc, #0x824]
00430b54: add r0, pc, r0
00430b58: bl #0x3136b8
00430b5c: ldr r3, [r4, r6]
00430b60: ldr r2, [sp, #0xbc]
00430b64: mov r0, r5
00430b68: ldr r3, [r3]
00430b6c: cmp r2, r3
00430b70: bne #0x43135c
00430b74: add sp, sp, #0xc4
00430b78: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00430b7c: mov r0, sl
00430b80: bl #0x337888
00430b84: ldr r1, [pc, #0x7f4]
00430b88: add fp, sp, #0x8c
00430b8c: add r2, sp, #0x6c
00430b90: add r1, pc, r1
00430b94: mov r0, fp
00430b98: bl #0x3140ec
00430b9c: mov r1, fp
00430ba0: mov r0, sl
00430ba4: bl #0x337a88
00430ba8: mov sl, r0
00430bac: mov r0, fp
00430bb0: bl #0x318254
00430bb4: mov r0, r8
00430bb8: bl #0x318254
00430bbc: cmp sl, #0
00430bc0: bne #0x431360
00430bc4: cmp r7, #0
00430bc8: str sl, [r5, #0xac]
00430bcc: str r7, [r5, #0xb4]
00430bd0: beq #0x431360
00430bd4: mov r3, #0
00430bd8: add r1, sp, #0x64
00430bdc: add r2, sp, #0x60
00430be0: mov r0, r5
00430be4: str r3, [sp, #0x44]
00430be8: str r3, [sp, #0x40]
00430bec: str r3, [sp, #0x3c]
00430bf0: str sl, [sp, #0x48]
00430bf4: str sl, [sp, #0x64]
00430bf8: str sl, [sp, #0x60]
00430bfc: bl #0x42cbbc
00430c00: ldr r0, [sp, #0x64]
00430c04: bl #0x30e964
00430c08: str r0, [sp, #0x3c]
00430c0c: ldr r0, [sp, #0x60]
00430c10: bl #0x30e964
00430c14: str r0, [sp, #0x40]
00430c18: ldr r3, [r7]
00430c1c: mov r0, r7
00430c20: mov lr, pc
00430c24: ldr pc, [r3, #8]
00430c28: cmp r0, #4
00430c2c: bne #0x430c54
00430c30: ldr r8, [r7, #0xc]
00430c34: ldrb r3, [r7, #0x10]
00430c38: cmp r8, #3
00430c3c: str r3, [sp, #0x48]
00430c40: bls #0x430d60
00430c44: mov r3, #0
00430c48: str r3, [r5, #0xb4]
00430c4c: mov r5, #1
00430c50: b #0x430b50
00430c54: ldr r3, [r7]
00430c58: mov r0, r7
00430c5c: mov lr, pc
00430c60: ldr pc, [r3, #8]
00430c64: cmp r0, #5
00430c68: beq #0x430d4c
00430c6c: ldr r3, [r7]
00430c70: mov r0, r7
00430c74: mov lr, pc
00430c78: ldr pc, [r3, #8]
00430c7c: cmp r0, #7
00430c80: beq #0x430db4
00430c84: mov r8, #0
00430c88: mov r7, #0
00430c8c: add sb, sp, #0x3c
00430c90: ldr sl, [pc, #0x6ec]
00430c94: b #0x430cd4
00430c98: ldr r0, [r4, sl]
00430c9c: bl #0x31f684
00430ca0: cmp r0, #0
00430ca4: bne #0x430cc8
00430ca8: cmp r7, #2
00430cac: movne r3, #0
00430cb0: moveq r3, #1
00430cb4: cmp r8, #0
00430cb8: movle r2, #0
00430cbc: andgt r2, r3, #1
00430cc0: cmp r2, #0
00430cc4: beq #0x430d84
00430cc8: add r7, r7, #1
00430ccc: cmp r7, #4
00430cd0: beq #0x430d38
00430cd4: ldr r3, [r5, #0xf4]
00430cd8: add r2, r3, r7, lsl #2
00430cdc: ldr fp, [r2, #0x134]
00430ce0: cmp fp, #0
00430ce4: beq #0x430cc8
00430ce8: cmp r7, #3
00430cec: bne #0x430c98
00430cf0: ldr r3, [r3, #0x13c]
00430cf4: cmp r3, #0
00430cf8: beq #0x430d08
00430cfc: ldr r3, [r3, #0x118]
00430d00: cmp r3, #0
00430d04: bne #0x430d38
00430d08: ldr r3, [r4, sl]
00430d0c: mov r0, r3
00430d10: str r3, [sp, #8]
00430d14: bl #0x31f594
00430d18: cmp r0, #0
00430d1c: ldr r3, [sp, #8]
00430d20: beq #0x430d38
00430d24: mov r0, r3
00430d28: bl #0x31f594
00430d2c: ldrb r3, [r0, #0x198]
00430d30: cmp r3, #0
00430d34: bne #0x430d78
00430d38: ldr r3, [r5, #0xac]
00430d3c: mov r2, #0
00430d40: str r2, [r5, #0xb4]
00430d44: mov r5, r3
00430d48: b #0x430b50
00430d4c: mov r3, #1
00430d50: str r3, [sp, #0x48]
00430d54: ldr r8, [r7, #0xc]
00430d58: cmp r8, #3
00430d5c: bhi #0x430c44
00430d60: cmp r8, #0
00430d64: beq #0x430c88
00430d68: ldrb r3, [r5, #0x110]
00430d6c: cmp r3, #0
00430d70: beq #0x430c88
00430d74: b #0x430c44
00430d78: cmp r7, #2
00430d7c: movne r3, #0
00430d80: moveq r3, #1
00430d84: cmp r3, #0
00430d88: bne #0x4312cc
00430d8c: ldr r3, [fp]
00430d90: mov r0, fp
00430d94: mov r1, sb
00430d98: mov r2, r8
00430d9c: mov lr, pc
00430da0: ldr pc, [r3, #0x14]
00430da4: ldr r3, [r5, #0xac]
00430da8: cmp r3, #1
00430dac: bne #0x430cc8
00430db0: b #0x430d3c
00430db4: ldr r1, [pc, #0x5cc]
00430db8: add fp, sp, #0x18
00430dbc: mov r0, fp
00430dc0: add r1, pc, r1
00430dc4: mov r8, #2
00430dc8: strb sl, [sp, #0x18]
00430dcc: strb sl, [sp, #0x19]
00430dd0: bl #0x797350
00430dd4: ldr r0, [r7, #0xc]
00430dd8: strb sl, [sp, #0x24]
00430ddc: strb r8, [sp, #0x25]
00430de0: bl #0x30ed30
00430de4: ldr r3, [r7, #0x10]
00430de8: strd r0, r1, [sp, #0x28]
00430dec: mov r0, r3
00430df0: strb sl, [sp, #0x30]
00430df4: strb r8, [sp, #0x31]
00430df8: bl #0x30ed30
00430dfc: strd r0, r1, [sp, #0x58]
00430e00: ldr r2, [sp, #0x58]
00430e04: ldr r3, [r7, #8]
00430e08: str r2, [sp, #0x34]
00430e0c: ldr r2, [sp, #0x5c]
00430e10: str r2, [fp, #0x20]
00430e14: cmp r3, #0x2c
00430e18: addls pc, pc, r3, lsl #2
00430e1c: b #0x431210
00430e20: b #0x4311fc
00430e24: b #0x4311e8
00430e28: b #0x4311d4
00430e2c: b #0x4311c0
00430e30: b #0x4311ac
00430e34: b #0x431198
00430e38: b #0x431184
00430e3c: b #0x431170
00430e40: b #0x43115c
00430e44: b #0x431148
00430e48: b #0x431134
00430e4c: b #0x431120
00430e50: b #0x43110c
00430e54: b #0x4310f8
00430e58: b #0x4310e4
00430e5c: b #0x4310d0
00430e60: b #0x431210
00430e64: b #0x431210
00430e68: b #0x431210
00430e6c: b #0x4310bc
00430e70: b #0x431210
00430e74: b #0x431210
00430e78: b #0x4310a8
00430e7c: b #0x431210
00430e80: b #0x431210
00430e84: b #0x431210
00430e88: b #0x431210
00430e8c: b #0x431210
00430e90: b #0x431210
00430e94: b #0x431210
00430e98: b #0x431210
00430e9c: b #0x431210
00430ea0: b #0x431210
00430ea4: b #0x431210
00430ea8: b #0x431210
00430eac: b #0x431210
00430eb0: b #0x431210
00430eb4: b #0x431094
00430eb8: b #0x431080
00430ebc: b #0x43106c
00430ec0: b #0x431058
00430ec4: b #0x431044
00430ec8: b #0x431030
00430ecc: b #0x43101c
00430ed0: b #0x430ed4
00430ed4: ldr r1, [pc, #0x4b0]
00430ed8: mov r0, fp
00430edc: add r1, pc, r1
00430ee0: bl #0x797350
00430ee4: ldr sl, [r4, sb]
00430ee8: add r8, sp, #0x74
00430eec: ldr r7, [pc, #0x49c]
00430ef0: mov r0, sl
00430ef4: bl #0x337888
00430ef8: ldr r1, [pc, #0x494]
00430efc: add r2, sp, #0x68
00430f00: mov r0, r8
00430f04: add r1, pc, r1
00430f08: bl #0x3140ec
00430f0c: mov r1, r8
00430f10: mov r0, sl
00430f14: bl #0x337a88
00430f18: mov r0, r8
00430f1c: bl #0x318254
00430f20: ldr r0, [pc, #0x470]
00430f24: add r7, pc, r7
00430f28: add r0, pc, r0
00430f2c: bl #0x3136b4
00430f30: ldr r8, [r7, #0x11c]
00430f34: ands r8, r8, #1
00430f38: beq #0x4312f0
00430f3c: ldr r1, [pc, #0x458]
00430f40: ldr r2, [pc, #0x458]
00430f44: ldr r3, [pc, #0x458]
00430f48: ldr r7, [pc, #0x458]
00430f4c: add r1, pc, r1
00430f50: add r2, pc, r2
00430f54: add r3, pc, r3
00430f58: add r7, pc, r7
00430f5c: add r1, r1, #0x1c
00430f60: add r2, r2, #0x1c
00430f64: add r3, r3, #0x1c
00430f68: add r7, r7, #0x1c
00430f6c: str r1, [sp, #0xc]
00430f70: str r2, [sp, #0x10]
00430f74: str r3, [sp, #0x14]
00430f78: mov r8, #0
00430f7c: mov sl, r4
00430f80: ldr r3, [r5, #0xf4]
00430f84: add r3, r3, r8, lsl #2
00430f88: ldr r4, [r3, #0x134]
00430f8c: cmp r4, #0
00430f90: beq #0x430fe0
00430f94: ldr r3, [r7, #0x2c]
00430f98: cmp r3, #0
00430f9c: beq #0x431268
00430fa0: ldr r3, [r7, #0x28]
00430fa4: ldrb sb, [r3, #4]
00430fa8: cmp sb, #0
00430fac: beq #0x431248
00430fb0: ldr r3, [sp, #0xc]
00430fb4: mov r0, #0x30
00430fb8: mla r0, r0, r8, r3
00430fbc: bl #0x427d50
00430fc0: ldr r2, [pc, #0x3e4]
00430fc4: mov r1, r0
00430fc8: mov ip, #3
00430fcc: mov r0, r4
00430fd0: add r2, pc, r2
00430fd4: mov r3, fp
00430fd8: str ip, [sp]
00430fdc: bl #0x7abe0c
00430fe0: add r8, r8, #1
00430fe4: cmp r8, #4
00430fe8: add r7, r7, #0x30
00430fec: bne #0x430f80
00430ff0: ldr r0, [pc, #0x3b8]
00430ff4: mov r4, sl
00430ff8: add r0, pc, r0
00430ffc: bl #0x3136b8
00431000: add r0, fp, #0x18
00431004: bl #0x797124
00431008: add r0, fp, #0xc
0043100c: bl #0x797124
00431010: mov r0, fp
00431014: bl #0x797124
00431018: b #0x430c84
0043101c: ldr r1, [pc, #0x390]
00431020: mov r0, fp
00431024: add r1, pc, r1
00431028: bl #0x797350
0043102c: b #0x430ee4
00431030: ldr r1, [pc, #0x380]
00431034: mov r0, fp
00431038: add r1, pc, r1
0043103c: bl #0x797350
00431040: b #0x430ee4
00431044: ldr r1, [pc, #0x370]
00431048: mov r0, fp
0043104c: add r1, pc, r1
00431050: bl #0x797350
00431054: b #0x430ee4
00431058: ldr r1, [pc, #0x360]
0043105c: mov r0, fp
00431060: add r1, pc, r1
00431064: bl #0x797350
00431068: b #0x430ee4
0043106c: ldr r1, [pc, #0x350]
00431070: mov r0, fp
00431074: add r1, pc, r1
00431078: bl #0x797350
0043107c: b #0x430ee4
00431080: ldr r1, [pc, #0x340]
00431084: mov r0, fp
00431088: add r1, pc, r1
0043108c: bl #0x797350
00431090: b #0x430ee4
00431094: ldr r1, [pc, #0x330]
00431098: mov r0, fp
0043109c: add r1, pc, r1
004310a0: bl #0x797350
004310a4: b #0x430ee4
004310a8: ldr r1, [pc, #0x320]
004310ac: mov r0, fp
004310b0: add r1, pc, r1
004310b4: bl #0x797350
004310b8: b #0x430ee4
004310bc: ldr r1, [pc, #0x310]
004310c0: mov r0, fp
004310c4: add r1, pc, r1
004310c8: bl #0x797350
004310cc: b #0x430ee4
004310d0: ldr r1, [pc, #0x300]
004310d4: mov r0, fp
004310d8: add r1, pc, r1
004310dc: bl #0x797350
004310e0: b #0x430ee4
004310e4: ldr r1, [pc, #0x2f0]
004310e8: mov r0, fp
004310ec: add r1, pc, r1
004310f0: bl #0x797350
004310f4: b #0x430ee4
004310f8: ldr r1, [pc, #0x2e0]
004310fc: mov r0, fp
00431100: add r1, pc, r1
00431104: bl #0x797350
00431108: b #0x430ee4
0043110c: ldr r1, [pc, #0x2d0]
00431110: mov r0, fp
00431114: add r1, pc, r1
00431118: bl #0x797350
0043111c: b #0x430ee4
00431120: ldr r1, [pc, #0x2c0]
00431124: mov r0, fp
00431128: add r1, pc, r1
0043112c: bl #0x797350
00431130: b #0x430ee4
00431134: ldr r1, [pc, #0x2b0]
00431138: mov r0, fp
0043113c: add r1, pc, r1
00431140: bl #0x797350
00431144: b #0x430ee4
00431148: ldr r1, [pc, #0x2a0]
0043114c: mov r0, fp
00431150: add r1, pc, r1
00431154: bl #0x797350
00431158: b #0x430ee4
0043115c: ldr r1, [pc, #0x290]
00431160: mov r0, fp
00431164: add r1, pc, r1
00431168: bl #0x797350
0043116c: b #0x430ee4
00431170: ldr r1, [pc, #0x280]
00431174: mov r0, fp
00431178: add r1, pc, r1
0043117c: bl #0x797350
00431180: b #0x430ee4
00431184: ldr r1, [pc, #0x270]
00431188: mov r0, fp
0043118c: add r1, pc, r1
00431190: bl #0x797350
00431194: b #0x430ee4
00431198: ldr r1, [pc, #0x260]
0043119c: mov r0, fp
004311a0: add r1, pc, r1
004311a4: bl #0x797350
004311a8: b #0x430ee4
004311ac: ldr r1, [pc, #0x250]
004311b0: mov r0, fp
004311b4: add r1, pc, r1
004311b8: bl #0x797350
004311bc: b #0x430ee4
004311c0: ldr r1, [pc, #0x240]
004311c4: mov r0, fp
004311c8: add r1, pc, r1
004311cc: bl #0x797350
004311d0: b #0x430ee4
004311d4: ldr r1, [pc, #0x230]
004311d8: mov r0, fp
004311dc: add r1, pc, r1
004311e0: bl #0x797350
004311e4: b #0x430ee4
004311e8: ldr r1, [pc, #0x220]
004311ec: mov r0, fp
004311f0: add r1, pc, r1
004311f4: bl #0x797350
004311f8: b #0x430ee4
004311fc: ldr r1, [pc, #0x210]
00431200: mov r0, fp
00431204: add r1, pc, r1
00431208: bl #0x797350
0043120c: b #0x430ee4
00431210: ldr r1, [pc, #0x200]
00431214: add r7, sp, #0x4c
00431218: mov r3, #0
0043121c: mov r0, r7
00431220: add r1, pc, r1
00431224: strb r3, [sp, #0x4d]
00431228: strb r3, [sp, #0x4c]
0043122c: bl #0x797350
00431230: mov r0, fp
00431234: mov r1, r7
00431238: bl #0x79773c
0043123c: mov r0, r7
00431240: bl #0x797124
00431244: b #0x430ee4
00431248: mov r0, #0x30
0043124c: mul r0, r0, r8
00431250: ldr r3, [sp, #0x14]
00431254: add r0, r0, #0x28
00431258: mov r1, sb
0043125c: add r0, r3, r0
00431260: bl #0x41fe84
00431264: str sb, [r7, #0x2c]
00431268: mov ip, #0x30
0043126c: mul ip, ip, r8
00431270: ldr r3, [sp, #0x10]
00431274: ldr r1, [pc, #0x1a0]
00431278: mov r2, r4
0043127c: add r0, r3, ip
00431280: add r1, pc, r1
00431284: mov r3, #0
00431288: str ip, [sp, #8]
0043128c: bl #0x427ca0
00431290: ldr r3, [r7, #0x2c]
00431294: ldr ip, [sp, #8]
00431298: cmp r3, #0
0043129c: beq #0x430fe0
004312a0: ldr r3, [r7, #0x28]
004312a4: ldrb sb, [r3, #4]
004312a8: cmp sb, #0
004312ac: bne #0x430fb0
004312b0: ldr r3, [sp, #0x10]
004312b4: add r0, ip, #0x28
004312b8: mov r1, sb
004312bc: add r0, r3, r0
004312c0: bl #0x41fe84
004312c4: str sb, [r7, #0x2c]
004312c8: b #0x430fe0
004312cc: bl #0x7fd794
004312d0: ldrb r3, [r0, #5]
004312d4: cmp r3, #0
004312d8: bne #0x431348
004312dc: ldr r3, [r4, sl]
004312e0: ldrb r3, [r3, #0xab]
004312e4: cmp r3, #0
004312e8: bne #0x430d8c
004312ec: b #0x430cc8
004312f0: add sl, r7, #0x11c
004312f4: mov r0, sl
004312f8: bl #0x30e76c
004312fc: cmp r0, #0
00431300: beq #0x430f3c
00431304: add r0, r7, #0x1c
00431308: bl #0x41aeec
0043130c: add r0, r7, #0x4c
00431310: bl #0x41aeec
00431314: add r0, r7, #0x7c
00431318: bl #0x41aeec
0043131c: add r0, r7, #0xac
00431320: bl #0x41aeec
00431324: mov r0, sl
00431328: bl #0x30ea3c
0043132c: ldr r3, [pc, #0xec]
00431330: ldr r1, [pc, #0xec]
00431334: mov r0, r8
00431338: ldr r2, [r4, r3]
0043133c: add r1, pc, r1
00431340: bl #0x30e304
00431344: b #0x430f3c
00431348: bl #0x320e98
0043134c: ldrb r3, [r0, #0x28]
00431350: cmp r3, #0
00431354: bne #0x430cc8
00431358: b #0x430d8c
0043135c: bl #0x30e310
00431360: mov r5, #0
00431364: b #0x430b50
00431368: subseq r3, r6, ip, lsr #31
0043136c: andeq r4, r0, ip, lsr #1
00431370: subeq sl, sb, r4, asr #16
00431374: andeq r0, r0, r4, lsl #17
00431378: subeq pc, r8, r8, lsr #6
0043137c: subeq sl, sb, ip, ror #15
00431380: subeq pc, r8, r0, ror #5
00431384: strdeq r3, r4, [r0], -r4
00431388: subeq sl, sb, r8, asr #20
0043138c: umaaleq sl, sb, r4, r5
00431390: subseq r4, r7, r4, ror r2
00431394: ldrdeq sb, sl, [sb], #-0x14
00431398: subeq sl, sb, r8, ror r5
0043139c: subseq r4, r7, ip, asr #4
004313a0: subseq r4, r7, r8, asr #4
004313a4: subseq r4, r7, r4, asr #4
004313a8: subseq r4, r7, r0, asr #4
004313ac: subeq sl, sb, r0, lsl #10
004313b0: subeq sl, sb, r8, lsr #9
004313b4: subeq sl, sb, ip, lsr r4
004313b8: subeq sl, sb, r8, lsl r4
004313bc: strdeq sl, fp, [sb], #-0x34
004313c0: ldrdeq sl, fp, [sb], #-0x30
004313c4: subeq sl, sb, ip, lsr #7
004313c8: subeq sl, sb, r8, lsl #7
004313cc: subeq sl, sb, r4, ror #6
004313d0: subeq sl, sb, r0, ror #7
004313d4: strheq sl, [sb], #-0x3c
004313d8: subeq sl, sb, r8, lsl r3
004313dc: strdeq sl, fp, [sb], #-0x24
004313e0: ldrdeq sl, fp, [sb], #-0x20
004313e4: subeq sl, sb, ip, lsr #5
004313e8: subeq sl, sb, r8, lsl #5
004313ec: subeq sl, sb, ip, ror #4
004313f0: subeq sl, sb, r0, asr r2
004313f4: subeq sl, sb, r4, lsr r2
004313f8: subeq sl, sb, r8, lsl r2
004313fc: strdeq sl, fp, [sb], #-0x1c
00431400: subeq sl, sb, r0, ror #3
00431404: subeq sl, sb, r4, asr #3
00431408: subeq sl, sb, r8, lsr #3
0043140c: subeq sl, sb, ip, lsl #3
00431410: subeq sl, sb, r0, ror r1
00431414: subeq sl, sb, r4, asr r1
00431418: subeq sl, sb, r8, ror #11
0043141c: subeq r1, sb, r0, lsl #31
00431420: muleq r0, r0, r8

_ZN12EventManager5FlushEv 0x3384ac 76
003384ac: push {r4, r5, r6, lr}
003384b0: mov r4, r0
003384b4: add r0, r0, #0x20
003384b8: bl #0x338374
003384bc: ldr r3, [r4, #0x18]
003384c0: cmp r3, #0
003384c4: beq #0x3384ec
003384c8: add r5, r4, #8
003384cc: mov r0, r5
003384d0: ldr r1, [r4, #0xc]
003384d4: bl #0x338438
003384d8: mov r3, #0
003384dc: str r5, [r4, #0x14]
003384e0: str r3, [r4, #0x18]
003384e4: str r5, [r4, #0x10]
003384e8: str r3, [r4, #0xc]
003384ec: add r0, r4, #0x28
003384f0: pop {r4, r5, r6, lr}
003384f4: b #0x3383b4

_ZN16GameEventManagerC1Ev 0x479d08 20
00479d08: push {r4, lr}
00479d0c: mov r4, r0
00479d10: bl #0x479cac
00479d14: mov r0, r4
00479d18: pop {r4, pc}

_ZN14IEventReceiverD0Ev 0x330674 52
00330674: ldr r3, [pc, #0x24]
00330678: ldr r2, [pc, #0x24]
0033067c: push {r4, lr}
00330680: add r3, pc, r3
00330684: ldr r2, [r3, r2]
00330688: mov r4, r0
0033068c: add r2, r2, #8
00330690: str r2, [r0]
00330694: bl #0x310440
00330698: mov r0, r4
0033069c: pop {r4, pc}
003306a0: rsbeq r4, r6, r0, lsl r4
003306a4: andeq r0, r0, r0, asr #22

_ZN6glitch7collada14IEventsManagerD0Ev 0x60f4d0 20
0060f4d0: push {r4, lr}
0060f4d4: mov r4, r0
0060f4d8: bl #0x30e2b0
0060f4dc: mov r0, r4
0060f4e0: pop {r4, pc}

_ZN16GameEventManager15LoopOnAllEventsEM9GameEventFvvE 0x479718 100
00479718: push {r4, r5, r6, r7, r8, lr}
0047971c: ldr r4, [r0]
00479720: ldr r3, [r0, #4]
00479724: sub sp, sp, #8
00479728: mov r5, r0
0047972c: cmp r4, r3
00479730: stm sp, {r1, r2}
00479734: mov r7, r1
00479738: beq #0x479774
0047973c: asr r6, r2, #1
00479740: and r8, r2, #1
00479744: ldr r0, [r4]
00479748: cmp r8, #0
0047974c: moveq r3, r7
00479750: ldrne r3, [r0, r6]
00479754: addeq r0, r0, r6
00479758: addne r0, r0, r6
0047975c: ldrne r3, [r3, r7]
00479760: blx r3
00479764: ldr r3, [r5, #4]
00479768: add r4, r4, #4
0047976c: cmp r4, r3
00479770: bne #0x479744
00479774: add sp, sp, #8
00479778: pop {r4, r5, r6, r7, r8, pc}

_ZN6glitch7CLoggerC1EPNS_14IEventReceiverE 0x6a08f0 60
006a08f0: ldr r2, [pc, #0x2c]
006a08f4: ldr ip, [pc, #0x2c]
006a08f8: str r4, [sp, #-4]!
006a08fc: add r2, pc, r2
006a0900: ldr ip, [r2, ip]
006a0904: mov r4, #1
006a0908: str r1, [r0, #0xc]
006a090c: add ip, ip, #8
006a0910: str ip, [r0]
006a0914: str r4, [r0, #8]
006a0918: str r4, [r0, #4]
006a091c: ldm sp!, {r4}
006a0920: bx lr
006a0924: mlaeq pc, r4, r1, r4
006a0928: andeq r4, r0, ip, asr #13

_ZNK12EventManager5RaiseERK6IEvent 0x338ebc 336
00338ebc: push {r4, r5, r6, r7, r8, lr}
00338ec0: mov r6, r0
00338ec4: ldr r3, [r1]
00338ec8: mov r0, r1
00338ecc: sub sp, sp, #8
00338ed0: mov r7, r1
00338ed4: mov lr, pc
00338ed8: ldr pc, [r3, #8]
00338edc: ldr r8, [r6, #0xc]
00338ee0: add r1, r6, #8
00338ee4: cmp r8, #0
00338ee8: beq #0x339004
00338eec: mov r2, r1
00338ef0: b #0x338ef8
00338ef4: mov r8, r3
00338ef8: ldr r3, [r8, #0x10]
00338efc: cmp r0, r3
00338f00: ldrgt r3, [r8, #0xc]
00338f04: ldrle r3, [r8, #8]
00338f08: movgt r8, r2
00338f0c: mov r2, r8
00338f10: cmp r3, #0
00338f14: bne #0x338ef4
00338f18: cmp r1, r8
00338f1c: beq #0x338ffc
00338f20: ldr r3, [r8, #0x10]
00338f24: cmp r0, r3
00338f28: blt #0x339004
00338f2c: cmp r1, r8
00338f30: beq #0x338ffc
00338f34: str sp, [sp]
00338f38: str sp, [sp, #4]
00338f3c: ldr r5, [r8, #0x14]!
00338f40: mov r4, sp
00338f44: cmp r5, r8
00338f48: moveq r5, sp
00338f4c: beq #0x338fc4
00338f50: mov r0, r4
00338f54: bl #0x33860c
00338f58: ldr r3, [r5, #8]
00338f5c: str r3, [r0, #8]
00338f60: ldr r3, [r5, #0xc]
00338f64: str r3, [r0, #0xc]
00338f68: ldrb r3, [r5, #0x10]
00338f6c: strb r3, [r0, #0x10]
00338f70: ldr r3, [sp, #4]
00338f74: str r4, [r0]
00338f78: str r3, [r0, #4]
00338f7c: str r0, [r3]
00338f80: str r0, [sp, #4]
00338f84: ldr r5, [r5]
00338f88: cmp r8, r5
00338f8c: bne #0x338f50
00338f90: ldr r5, [sp]
00338f94: mov r1, r7
00338f98: mov r2, r6
00338f9c: cmp r5, r4
00338fa0: beq #0x338fd4
00338fa4: ldr r3, [r5, #8]
00338fa8: mov r0, r3
00338fac: ldr r3, [r3]
00338fb0: mov lr, pc
00338fb4: ldr pc, [r3, #8]
00338fb8: cmp r0, #1
00338fbc: beq #0x338fd4
00338fc0: ldr r5, [r5]
00338fc4: cmp r5, r4
00338fc8: mov r1, r7
00338fcc: mov r2, r6
00338fd0: bne #0x338fa4
00338fd4: ldr r0, [sp]
00338fd8: cmp r0, r4
00338fdc: bne #0x338fe8
00338fe0: b #0x338ffc
00338fe4: mov r0, r5
00338fe8: ldr r5, [r0]
00338fec: mov r1, #0x14
00338ff0: bl #0x708f00
00338ff4: cmp r5, r4
00338ff8: bne #0x338fe4
00338ffc: add sp, sp, #8
00339000: pop {r4, r5, r6, r7, r8, pc}
00339004: mov r8, r1
00339008: b #0x338f2c

_ZNK16GameEventManager14GetEventByNameEPKc 0x479848 128
00479848: ldr r3, [pc, #0x6c]
0047984c: ldr r2, [pc, #0x6c]
00479850: push {r4, r5, r6, r7, r8, lr}
00479854: add r3, pc, r3
00479858: ldr r2, [r3, r2]
0047985c: mov r8, r0
00479860: mov r6, r1
00479864: ldr r5, [r2]
00479868: cmp r5, #0
0047986c: beq #0x4798ac
00479870: ldr r2, [pc, #0x4c]
00479874: mov r4, #0
00479878: ldr r3, [r3, r2]
0047987c: ldr r7, [r3]
00479880: b #0x479890
00479884: add r4, r4, #1
00479888: cmp r4, r5
0047988c: beq #0x4798ac
00479890: ldr r1, [r7, r4, lsl #2]
00479894: mov r0, r6
00479898: bl #0x30e31c
0047989c: cmp r0, #0
004798a0: bne #0x479884
004798a4: mov r1, r4
004798a8: b #0x4798b0
004798ac: mvn r1, #0
004798b0: mov r0, r8
004798b4: pop {r4, r5, r6, r7, r8, lr}
004798b8: b #0x4796f0
004798bc: subseq fp, r1, ip, lsr r2
004798c0: andeq r0, r0, r0, ror #16
004798c4: ldrdeq r3, r4, [r0], -r0

_ZN23Objective_EventReceiver27CheckMustSendThroughNetworkEPK6IEvent 0x47ac94 84
0047ac94: push {r4, r5, r6, lr}
0047ac98: mov r4, r1
0047ac9c: bl #0x7fd794
0047aca0: ldrb r3, [r0, #5]
0047aca4: cmp r3, #0
0047aca8: beq #0x47ace4
0047acac: ldrb r5, [r4, #0x11]
0047acb0: cmp r5, #0
0047acb4: bne #0x47ace4
0047acb8: ldrb r3, [r4, #0x10]
0047acbc: cmp r3, #0
0047acc0: beq #0x47ace4
0047acc4: bl #0x80b1bc
0047acc8: mov r6, r0
0047accc: mov r0, r4
0047acd0: bl #0x47ac58
0047acd4: mov r1, r0
0047acd8: mov r0, r6
0047acdc: bl #0x80e2a4
0047ace0: strb r5, [r4, #0x10]
0047ace4: pop {r4, r5, r6, pc}

_ZN6glitch7collada14IEventsManagerD1Ev 0x60e028 4
0060e028: bx lr

_ZNK16GameEventManager12GetEventByIDEi 0x4796f0 40
004796f0: cmp r1, #0
004796f4: blt #0x479710
004796f8: ldr r2, [r0, #4]
004796fc: ldr r3, [r0]
00479700: rsb r2, r3, r2
00479704: cmp r1, r2, asr #2
00479708: ldrlt r0, [r3, r1, lsl #2]
0047970c: bxlt lr
00479710: mov r0, #0
00479714: bx lr

_ZN6glitch12createDeviceENS_5video13E_DRIVER_TYPEERKNS_4core11dimension2dIiEEjbbbPNS_14IEventReceiverE 0x5340ac 204
005340ac: push {r4, r5, r6, r7, r8, lr}
005340b0: ldr ip, [pc, #0xbc]
005340b4: sub sp, sp, #0x58
005340b8: ldr r6, [r1, #4]
005340bc: add ip, pc, ip
005340c0: ldr r7, [r1]
005340c4: str ip, [sp, #0x3c]
005340c8: mov ip, #0x500000
005340cc: str ip, [sp, #0x40]
005340d0: mov ip, #0xa0000
005340d4: ldrb r5, [sp, #0x70]
005340d8: ldrb r4, [sp, #0x74]
005340dc: str ip, [sp, #0x44]
005340e0: strb r3, [sp, #0xe]
005340e4: mov ip, #0x20000
005340e8: ldr r3, [sp, #0x78]
005340ec: str ip, [sp, #0x48]
005340f0: mov r8, #0x10
005340f4: mov ip, #0x2000
005340f8: mov r1, #0
005340fc: str r0, [sp]
00534100: mvn lr, #0
00534104: strb r8, [sp, #0xd]
00534108: str ip, [sp, #0x4c]
0053410c: mov r8, #0x40000
00534110: mov ip, #0x3f800000
00534114: mov r0, sp
00534118: str r8, [sp, #0x20]
0053411c: str lr, [sp, #0x38]
00534120: str ip, [sp, #0x50]
00534124: str r1, [sp, #0x54]
00534128: str r7, [sp, #4]
0053412c: str r6, [sp, #8]
00534130: strb r2, [sp, #0xc]
00534134: strb r5, [sp, #0xf]
00534138: strb r4, [sp, #0x10]
0053413c: str r3, [sp, #0x28]
00534140: strb r1, [sp, #0x11]
00534144: str r1, [sp, #0x14]
00534148: str r1, [sp, #0x18]
0053414c: str r1, [sp, #0x1c]
00534150: strb r1, [sp, #0x24]
00534154: strb r1, [sp, #0x25]
00534158: strb r1, [sp, #0x26]
0053415c: str r1, [sp, #0x2c]
00534160: strb r1, [sp, #0x30]
00534164: str lr, [sp, #0x34]
00534168: bl #0x6a0748
0053416c: add sp, sp, #0x58
00534170: pop {r4, r5, r6, r7, r8, pc}
00534174: eorseq sb, sl, r4, lsl #23

_ZThn24_N23Objective_EventReceiver7onEventEPK6IEventPK12EventManager 0x47ace8 8
0047ace8: sub r0, r0, #0x18
0047acec: b #0x47acf0

_ZN12EventManagerD0Ev 0x338574 28
00338574: push {r4, lr}
00338578: mov r4, r0
0033857c: bl #0x3384f8
00338580: mov r0, r4
00338584: bl #0x310440
00338588: mov r0, r4
0033858c: pop {r4, pc}

_ZN11HUDControls7onEventEPK6IEventPK12EventManager 0x418258 212
00418258: ldr r3, [pc, #0xc0]
0041825c: push {r4, r5, r6, lr}
00418260: ldr r2, [pc, #0xbc]
00418264: mov r4, r1
00418268: ldr r1, [pc, #0xb8]
0041826c: add r3, pc, r3
00418270: mov r5, r0
00418274: add r1, pc, r1
00418278: ldr r0, [r3, r2]
0041827c: bl #0x320e44
00418280: cmp r0, #0
00418284: bne #0x418294
00418288: ldrb r3, [r5, #9]
0041828c: cmp r3, #0
00418290: beq #0x41829c
00418294: mov r0, #0
00418298: pop {r4, r5, r6, pc}
0041829c: ldrb r3, [r5, #0x84]
004182a0: cmp r3, #0
004182a4: bne #0x418294
004182a8: ldr r3, [r4]
004182ac: mov r0, r4
004182b0: mov lr, pc
004182b4: ldr pc, [r3, #8]
004182b8: cmp r0, #4
004182bc: bne #0x4182f4
004182c0: ldrb r3, [r4, #0x10]
004182c4: ldrh r1, [r4, #8]
004182c8: ldrh r2, [r4, #0xa]
004182cc: cmp r3, #0
004182d0: mvneq r3, #0
004182d4: sxthne r1, r1
004182d8: sxthne r2, r2
004182dc: strne r2, [r5, #0x80]
004182e0: strne r1, [r5, #0x7c]
004182e4: streq r3, [r5, #0x80]
004182e8: streq r3, [r5, #0x7c]
004182ec: mov r0, #0
004182f0: pop {r4, r5, r6, pc}
004182f4: ldr r3, [r4]
004182f8: mov r0, r4
004182fc: mov lr, pc
00418300: ldr pc, [r3, #8]
00418304: cmp r0, #5
00418308: ldrsheq r2, [r4, #8]
0041830c: ldrsheq r3, [r4, #0xa]
00418310: mov r0, #0
00418314: streq r2, [r5, #0x7c]
00418318: streq r3, [r5, #0x80]
0041831c: pop {r4, r5, r6, pc}
00418320: subseq ip, r7, r4, lsr #16
00418324: strdeq r3, r4, [r0], -r4
00418328: subeq r6, sl, ip, asr #19

_ZNSt4priv10_List_baseIPN6glitch14IEventReceiverESaIS3_EE5clearEv 0x328f00 64
00328f00: push {r4, r5, r6, lr}
00328f04: mov r5, r0
00328f08: ldr r0, [r0]
00328f0c: cmp r0, r5
00328f10: bne #0x328f1c
00328f14: b #0x328f34
00328f18: mov r0, r4
00328f1c: ldr r4, [r0]
00328f20: mov r1, #0xc
00328f24: bl #0x708f00
00328f28: cmp r4, r5
00328f2c: bne #0x328f18
00328f30: mov r0, r5
00328f34: str r0, [r5, #4]
00328f38: str r0, [r5]
00328f3c: pop {r4, r5, r6, pc}

_ZN12EventManagerC1Ev 0x3380bc 96
003380bc: ldr r1, [pc, #0x50]
003380c0: push {r4, r5}
003380c4: ldr r4, [pc, #0x4c]
003380c8: add r1, pc, r1
003380cc: mov ip, #0
003380d0: ldr r4, [r1, r4]
003380d4: mov r2, r0
003380d8: add r5, r0, #0x20
003380dc: add r4, r4, #8
003380e0: str r4, [r0]
003380e4: str ip, [r0, #0xc]
003380e8: add r4, r0, #0x28
003380ec: strb ip, [r2, #8]!
003380f0: str r2, [r0, #0x14]
003380f4: str r4, [r0, #0x2c]
003380f8: str ip, [r0, #0x18]
003380fc: str r5, [r0, #0x24]
00338100: str r2, [r0, #0x10]
00338104: str r5, [r0, #0x20]
00338108: str r4, [r0, #0x28]
0033810c: pop {r4, r5}
00338110: bx lr
00338114: rsbeq ip, r5, r8, asr #19
00338118: ldrdeq r4, r5, [r0], -r4

_ZN16GameEventManager7CompileEv 0x47977c 52
0047977c: ldr r1, [pc, #0x24]
00479780: ldr r3, [pc, #0x24]
00479784: mov ip, #0
00479788: add r1, pc, r1
0047978c: ldr r3, [r1, r3]
00479790: sub sp, sp, #8
00479794: mov r2, ip
00479798: mov r1, r3
0047979c: stm sp, {r3, ip}
004797a0: add sp, sp, #8
004797a4: b #0x479718
004797a8: subseq fp, r1, r8, lsl #6
004797ac: strdeq r1, r2, [r0], -r4

_ZN14CameraOverview7onEventEPK6IEventPK12EventManager 0x410930 404
00410930: push {r4, r5, r6, lr}
00410934: mov r4, r0
00410938: ldr r3, [r1]
0041093c: mov r0, r1
00410940: mov r5, r1
00410944: mov lr, pc
00410948: ldr pc, [r3, #8]
0041094c: cmp r0, #0
00410950: bne #0x4109e4
00410954: ldrb r3, [r5, #0x10]
00410958: cmp r3, #0
0041095c: beq #0x4109d4
00410960: ldr r3, [r5, #0xc]
00410964: sub r3, r3, #0x41
00410968: cmp r3, #0x17
0041096c: addls pc, pc, r3, lsl #2
00410970: b #0x4109e4
00410974: b #0x410a2c
00410978: b #0x4109e4
0041097c: b #0x4109e4
00410980: b #0x410a48
00410984: b #0x410a64
00410988: b #0x4109e4
0041098c: b #0x4109e4
00410990: b #0x4109e4
00410994: b #0x4109e4
00410998: b #0x4109e4
0041099c: b #0x4109e4
004109a0: b #0x4109e4
004109a4: b #0x4109e4
004109a8: b #0x4109e4
004109ac: b #0x4109e4
004109b0: b #0x4109e4
004109b4: b #0x410a78
004109b8: b #0x4109e4
004109bc: b #0x410a8c
004109c0: b #0x4109e4
004109c4: b #0x4109e4
004109c8: b #0x4109e4
004109cc: b #0x410aa8
004109d0: b #0x410a14
004109d4: ldr r3, [r5, #0xc]
004109d8: sub r3, r3, #0x41
004109dc: cmp r3, #0x17
004109e0: bls #0x4109ec
004109e4: mov r0, #0
004109e8: pop {r4, r5, r6, pc}
004109ec: mov r0, #1
004109f0: lsl r2, r0, r3
004109f4: movw r3, #0x19
004109f8: movt r3, #0xc5
004109fc: and r3, r2, r3
00410a00: cmp r3, #0
00410a04: beq #0x4109e4
00410a08: mov r3, #0
00410a0c: str r3, [r4, #0x24]
00410a10: pop {r4, r5, r6, pc}
00410a14: mov r3, #0
00410a18: mov r0, #1
00410a1c: str r3, [r4, #0x24]
00410a20: str r3, [r4, #0x1c]
00410a24: str r3, [r4, #0x20]
00410a28: pop {r4, r5, r6, pc}
00410a2c: mov r1, #0x42000000
00410a30: ldr r0, [r4, #0x1c]
00410a34: add r1, r1, #0xc80000
00410a38: bl #0x30e3ac
00410a3c: str r0, [r4, #0x1c]
00410a40: mov r0, #1
00410a44: pop {r4, r5, r6, pc}
00410a48: mov r1, #0x42000000
00410a4c: ldr r0, [r4, #0x1c]
00410a50: add r1, r1, #0xc80000
00410a54: bl #0x30eba4
00410a58: str r0, [r4, #0x1c]
00410a5c: mov r0, #1
00410a60: pop {r4, r5, r6, pc}
00410a64: mov r3, #0x42000000
00410a68: add r3, r3, #0xc80000
00410a6c: mov r0, #1
00410a70: str r3, [r4, #0x24]
00410a74: pop {r4, r5, r6, pc}
00410a78: mov r3, #0xc2000000
00410a7c: add r3, r3, #0xc80000
00410a80: mov r0, #1
00410a84: str r3, [r4, #0x24]
00410a88: pop {r4, r5, r6, pc}
00410a8c: mov r1, #0x42000000
00410a90: ldr r0, [r4, #0x20]
00410a94: add r1, r1, #0xc80000
00410a98: bl #0x30e3ac
00410a9c: str r0, [r4, #0x20]
00410aa0: mov r0, #1
00410aa4: pop {r4, r5, r6, pc}
00410aa8: mov r1, #0x42000000
00410aac: ldr r0, [r4, #0x20]
00410ab0: add r1, r1, #0xc80000
00410ab4: bl #0x30eba4
00410ab8: str r0, [r4, #0x20]
00410abc: mov r0, #1
00410ac0: pop {r4, r5, r6, pc}

_ZN6glitch14IEventReceiverD0Ev 0x31fd8c 52
0031fd8c: ldr r3, [pc, #0x24]
0031fd90: ldr r2, [pc, #0x24]
0031fd94: push {r4, lr}
0031fd98: add r3, pc, r3
0031fd9c: ldr r2, [r3, r2]
0031fda0: mov r4, r0
0031fda4: add r2, r2, #8
0031fda8: str r2, [r0]
0031fdac: bl #0x310440
0031fdb0: mov r0, r4
0031fdb4: pop {r4, pc}

_ZN12EventManagerD1Ev 0x3384f8 124
003384f8: ldr r3, [pc, #0x6c]
003384fc: ldr r2, [pc, #0x6c]
00338500: push {r4, r5, r6, lr}
00338504: add r3, pc, r3
00338508: ldr r2, [r3, r2]
0033850c: mov r5, r0
00338510: mov r4, r0
00338514: add r2, r2, #8
00338518: str r2, [r5], #0x20
0033851c: mov r0, r5
00338520: bl #0x338374
00338524: add r0, r4, #0x28
00338528: bl #0x3383b4
0033852c: mov r0, r5
00338530: bl #0x338374
00338534: ldr r3, [r4, #0x18]
00338538: cmp r3, #0
0033853c: beq #0x338564
00338540: add r5, r4, #8
00338544: mov r0, r5
00338548: ldr r1, [r4, #0xc]
0033854c: bl #0x338438
00338550: mov r3, #0
00338554: str r5, [r4, #0x14]
00338558: str r3, [r4, #0x18]
0033855c: str r5, [r4, #0x10]
00338560: str r3, [r4, #0xc]
00338564: mov r0, r4
00338568: pop {r4, r5, r6, pc}
0033856c: rsbeq ip, r5, ip, lsl #11
00338570: ldrdeq r4, r5, [r0], -r4

_ZN15v2EmuController8_onEventEPK10EvKeyboardPK12EventManager 0x4060c0 904
004060c0: push {r4, r5, lr}
004060c4: mov r4, r1
004060c8: ldr r1, [r1, #0xc]
004060cc: sub sp, sp, #0x14
004060d0: mov r5, r0
004060d4: sub r3, r1, #0x31
004060d8: cmp r3, #0x61
004060dc: addls pc, pc, r3, lsl #2
004060e0: b #0x406308
004060e4: b #0x406330
004060e8: b #0x406330
004060ec: b #0x406330
004060f0: b #0x406330
004060f4: b #0x406308
004060f8: b #0x406308
004060fc: b #0x406308
00406100: b #0x406308
00406104: b #0x406308
00406108: b #0x406308
0040610c: b #0x406308
00406110: b #0x406308
00406114: b #0x406308
00406118: b #0x406308
0040611c: b #0x406308
00406120: b #0x406308
00406124: b #0x406308
00406128: b #0x406308
0040612c: b #0x406308
00406130: b #0x406308
00406134: b #0x406308
00406138: b #0x406308
0040613c: b #0x406308
00406140: b #0x406308
00406144: b #0x406308
00406148: b #0x406308
0040614c: b #0x406308
00406150: b #0x406308
00406154: b #0x406344
00406158: b #0x40635c
0040615c: b #0x406308
00406160: b #0x406308
00406164: b #0x406308
00406168: b #0x406308
0040616c: b #0x406308
00406170: b #0x406308
00406174: b #0x406308
00406178: b #0x406308
0040617c: b #0x406308
00406180: b #0x406308
00406184: b #0x406308
00406188: b #0x406308
0040618c: b #0x406308
00406190: b #0x406308
00406194: b #0x406308
00406198: b #0x406308
0040619c: b #0x406374
004061a0: b #0x406374
004061a4: b #0x406374
004061a8: b #0x4063ac
004061ac: b #0x406308
004061b0: b #0x406308
004061b4: b #0x406308
004061b8: b #0x406308
004061bc: b #0x406308
004061c0: b #0x406308
004061c4: b #0x406308
004061c8: b #0x406308
004061cc: b #0x406308
004061d0: b #0x406308
004061d4: b #0x406308
004061d8: b #0x406308
004061dc: b #0x406308
004061e0: b #0x406308
004061e4: b #0x406308
004061e8: b #0x406308
004061ec: b #0x406308
004061f0: b #0x406308
004061f4: b #0x406308
004061f8: b #0x406308
004061fc: b #0x406308
00406200: b #0x406308
00406204: b #0x406308
00406208: b #0x406308
0040620c: b #0x406308
00406210: b #0x406308
00406214: b #0x406308
00406218: b #0x406308
0040621c: b #0x406308
00406220: b #0x406308
00406224: b #0x406308
00406228: b #0x406308
0040622c: b #0x406308
00406230: b #0x406308
00406234: b #0x406308
00406238: b #0x406308
0040623c: b #0x40626c
00406240: b #0x4063c4
00406244: b #0x4063e0
00406248: b #0x4063fc
0040624c: b #0x406308
00406250: b #0x406308
00406254: b #0x406308
00406258: b #0x406308
0040625c: b #0x406308
00406260: b #0x406308
00406264: b #0x406418
00406268: b #0x406314
0040626c: ldrb r3, [r4, #0x10]
00406270: ldr r4, [r0, #0x14]
00406274: cmp r3, #0
00406278: orrne r4, r4, #9
0040627c: biceq r4, r4, #9
00406280: str r4, [r0, #0x14]
00406284: mov r3, #0
00406288: tst r4, #1
0040628c: str r3, [sp, #0xc]
00406290: str r3, [sp, #4]
00406294: str r3, [sp, #8]
00406298: beq #0x4062a8
0040629c: mov r3, #0xc3000000
004062a0: add r3, r3, #0x480000
004062a4: str r3, [sp, #4]
004062a8: tst r4, #2
004062ac: beq #0x4062c4
004062b0: mov r1, #0x43000000
004062b4: ldr r0, [sp, #4]
004062b8: add r1, r1, #0x480000
004062bc: bl #0x30eba4
004062c0: str r0, [sp, #4]
004062c4: tst r4, #4
004062c8: beq #0x4062e0
004062cc: mov r1, #0x43000000
004062d0: ldr r0, [sp, #8]
004062d4: add r1, r1, #0x480000
004062d8: bl #0x30e3ac
004062dc: str r0, [sp, #8]
004062e0: tst r4, #8
004062e4: beq #0x4062fc
004062e8: mov r1, #0x43000000
004062ec: ldr r0, [sp, #8]
004062f0: add r1, r1, #0x480000
004062f4: bl #0x30eba4
004062f8: str r0, [sp, #8]
004062fc: mov r0, r5
00406300: add r1, sp, #4
00406304: bl #0x405374
00406308: mov r0, #0
0040630c: add sp, sp, #0x14
00406310: pop {r4, r5, pc}
00406314: ldrb r3, [r4, #0x10]
00406318: cmp r3, #0
0040631c: ldr r3, [r0, #0x18]
00406320: orrne r3, r3, #0x12
00406324: biceq r3, r3, #0x12
00406328: str r3, [r0, #0x18]
0040632c: b #0x406308
00406330: ldrb r3, [r4, #0x10]
00406334: cmp r3, #0
00406338: beq #0x406308
0040633c: bl #0x4056b0
00406340: b #0x406308
00406344: ldrb r3, [r4, #0x10]
00406348: cmp r3, #0
0040634c: beq #0x406308
00406350: mov r1, #0
00406354: bl #0x4057fc
00406358: b #0x406308
0040635c: ldrb r3, [r4, #0x10]
00406360: cmp r3, #0
00406364: beq #0x406308
00406368: mov r1, #0
0040636c: bl #0x405b04
00406370: b #0x406308
00406374: ldr r0, [r0, #0xc]
00406378: cmp r0, #0
0040637c: beq #0x406308
00406380: sub r1, r1, #0x5f
00406384: bl #0x3bbe68
00406388: cmn r0, #1
0040638c: mov r1, r0
00406390: beq #0x406308
00406394: ldrb r3, [r4, #0x10]
00406398: cmp r3, #0
0040639c: beq #0x40643c
004063a0: mov r0, r5
004063a4: bl #0x405a20
004063a8: b #0x406308
004063ac: ldrb r1, [r4, #0x10]
004063b0: cmp r1, #0
004063b4: beq #0x406434
004063b8: mov r1, #0
004063bc: bl #0x4055f8
004063c0: b #0x406308
004063c4: ldrb r3, [r4, #0x10]
004063c8: ldr r4, [r0, #0x14]
004063cc: cmp r3, #0
004063d0: orrne r4, r4, #6
004063d4: biceq r4, r4, #6
004063d8: str r4, [r0, #0x14]
004063dc: b #0x406284
004063e0: ldrb r3, [r4, #0x10]
004063e4: ldr r4, [r0, #0x14]
004063e8: cmp r3, #0
004063ec: orrne r4, r4, #5
004063f0: biceq r4, r4, #5
004063f4: str r4, [r0, #0x14]
004063f8: b #0x406284
004063fc: ldrb r3, [r4, #0x10]
00406400: ldr r4, [r0, #0x14]
00406404: cmp r3, #0
00406408: orrne r4, r4, #0xa
0040640c: biceq r4, r4, #0xa
00406410: str r4, [r0, #0x14]
00406414: b #0x406284
00406418: ldrb r3, [r4, #0x10]
0040641c: cmp r3, #0
00406420: ldr r3, [r0, #0x18]
00406424: orrne r3, r3, #9
00406428: biceq r3, r3, #9
0040642c: str r3, [r0, #0x18]
00406430: b #0x406308
00406434: bl #0x405654
00406438: b #0x406308
0040643c: mov r0, r5
00406440: bl #0x405954
00406444: b #0x406308

_ZN16GameEventManagerC2Ev 0x479cf4 20
00479cf4: push {r4, lr}
00479cf8: mov r4, r0
00479cfc: bl #0x479cac
00479d00: mov r0, r4
00479d04: pop {r4, pc}

_ZN30ObjectiveTemplate_InteractWithIN7Structs23v2QuestPickedUpLiftableE19QE_PickedUpLiftableE11handleEventEPK6IEventPK12EventManager 0x47f5a0 148
0047f5a0: push {r4, lr}
0047f5a4: ldr r3, [r1, #0x18]
0047f5a8: ldr r2, [r0, #0x24]
0047f5ac: mov r4, r0
0047f5b0: ldr r0, [r0, #0xc]
0047f5b4: cmp r2, r3
0047f5b8: beq #0x47f5c4
0047f5bc: mov r0, #0
0047f5c0: pop {r4, pc}
0047f5c4: ldrb r3, [r1, #0x11]
0047f5c8: cmp r3, #0
0047f5cc: bne #0x47f61c
0047f5d0: ldr r3, [r4, #0x20]
0047f5d4: add r3, r3, #1
0047f5d8: str r3, [r4, #0x20]
0047f5dc: mov r3, #1
0047f5e0: strb r3, [r1, #0x10]
0047f5e4: ldr r3, [r4, #0x20]
0047f5e8: str r3, [r1, #0x14]
0047f5ec: ldr r3, [r4, #0x20]
0047f5f0: ldr r2, [r0, #0x28]
0047f5f4: cmp r2, r3
0047f5f8: bgt #0x47f5bc
0047f5fc: mov r0, r4
0047f600: bl #0x47ba10
0047f604: mov r0, r4
0047f608: ldr r3, [r4]
0047f60c: mov lr, pc
0047f610: ldr pc, [r3, #0x1c]
0047f614: mov r0, #0
0047f618: pop {r4, pc}
0047f61c: ldr r3, [r1, #0x14]
0047f620: ldr r2, [r4, #0x20]
0047f624: cmp r2, r3
0047f628: strlt r3, [r4, #0x20]
0047f62c: blt #0x47f5f0
0047f630: b #0x47f5bc

_ZThn16_N15v2EmuController7onEventEPK6IEventPK12EventManager 0x406448 8
00406448: sub r0, r0, #0x10
0040644c: b #0x406450

_ZN6glitch14IEventReceiverD1Ev 0x31d8cc 4
0031d8cc: bx lr

_ZN12EventManagerC2Ev 0x33805c 96
0033805c: ldr r1, [pc, #0x50]
00338060: push {r4, r5}
00338064: ldr r4, [pc, #0x4c]
00338068: add r1, pc, r1
0033806c: mov ip, #0
00338070: ldr r4, [r1, r4]
00338074: mov r2, r0
00338078: add r5, r0, #0x20
0033807c: add r4, r4, #8
00338080: str r4, [r0]
00338084: str ip, [r0, #0xc]
00338088: add r4, r0, #0x28
0033808c: strb ip, [r2, #8]!
00338090: str r2, [r0, #0x14]
00338094: str r4, [r0, #0x2c]
00338098: str ip, [r0, #0x18]
0033809c: str r5, [r0, #0x24]
003380a0: str r2, [r0, #0x10]
003380a4: str r5, [r0, #0x20]
003380a8: str r4, [r0, #0x28]
003380ac: pop {r4, r5}
003380b0: bx lr
003380b4: rsbeq ip, r5, r8, lsr #20
003380b8: ldrdeq r4, r5, [r0], -r4

_ZN6CharAI12RaiseAIEventEiPv 0x3cbb34 1764
003cbb34: ldr r3, [pc, #0x6d4]
003cbb38: push {r4, r5, r6, r7, r8, lr}
003cbb3c: add r3, pc, r3
003cbb40: mov r4, r1
003cbb44: mov r5, r0
003cbb48: mov r6, r2
003cbb4c: cmp r1, #0x3f
003cbb50: addls pc, pc, r1, lsl #2
003cbb54: b #0x3cbcfc
003cbb58: b #0x3cbc58
003cbb5c: b #0x3cbe90
003cbb60: b #0x3cbe98
003cbb64: b #0x3cbe2c
003cbb68: b #0x3cbcfc
003cbb6c: b #0x3cbcfc
003cbb70: b #0x3cbcfc
003cbb74: b #0x3cbcfc
003cbb78: b #0x3cbcfc
003cbb7c: b #0x3cbcfc
003cbb80: b #0x3cbcfc
003cbb84: b #0x3cbcfc
003cbb88: b #0x3cbcfc
003cbb8c: b #0x3cbcfc
003cbb90: b #0x3cbcfc
003cbb94: b #0x3cbcfc
003cbb98: b #0x3cbcfc
003cbb9c: b #0x3cbcfc
003cbba0: b #0x3cbcfc
003cbba4: b #0x3cbcfc
003cbba8: b #0x3cbcfc
003cbbac: b #0x3cbcfc
003cbbb0: b #0x3cbcfc
003cbbb4: b #0x3cbcfc
003cbbb8: b #0x3cbcfc
003cbbbc: b #0x3cbcfc
003cbbc0: b #0x3cbcfc
003cbbc4: b #0x3cbcfc
003cbbc8: b #0x3cbcfc
003cbbcc: b #0x3cbcfc
003cbbd0: b #0x3cbcfc
003cbbd4: b #0x3cbcfc
003cbbd8: b #0x3cbcfc
003cbbdc: b #0x3cbcfc
003cbbe0: b #0x3cbe40
003cbbe4: b #0x3cbe64
003cbbe8: b #0x3cbcfc
003cbbec: b #0x3cbcfc
003cbbf0: b #0x3cbcfc
003cbbf4: b #0x3cbcfc
003cbbf8: b #0x3cbe80
003cbbfc: b #0x3cbc78
003cbc00: b #0x3cbcfc
003cbc04: b #0x3cbcfc
003cbc08: b #0x3cbcfc
003cbc0c: b #0x3cbcfc
003cbc10: b #0x3cbcfc
003cbc14: b #0x3cbcfc
003cbc18: b #0x3cbc5c
003cbc1c: b #0x3cbc8c
003cbc20: b #0x3cbc98
003cbc24: b #0x3cbca4
003cbc28: b #0x3cbcac
003cbc2c: b #0x3cbcbc
003cbc30: b #0x3cbcfc
003cbc34: b #0x3cbcfc
003cbc38: b #0x3cbcfc
003cbc3c: b #0x3cbcfc
003cbc40: b #0x3cbcfc
003cbc44: b #0x3cbcfc
003cbc48: b #0x3cbcfc
003cbc4c: b #0x3cbcfc
003cbc50: b #0x3cbcfc
003cbc54: b #0x3cbcf0
003cbc58: movw r4, #0xc351
003cbc5c: ldr r0, [r5, #4]
003cbc60: add r0, r0, #0x4f0
003cbc64: add r0, r0, #0xc
003cbc68: mov r1, r4
003cbc6c: mov r2, r6
003cbc70: pop {r4, r5, r6, r7, r8, lr}
003cbc74: b #0x3c5684
003cbc78: ldr r3, [r0]
003cbc7c: mov r1, r2
003cbc80: mov lr, pc
003cbc84: ldr pc, [r3, #0x80]
003cbc88: b #0x3cbc5c
003cbc8c: mov r3, #0
003cbc90: strb r3, [r0, #0x18]
003cbc94: pop {r4, r5, r6, r7, r8, pc}
003cbc98: mov r3, #1
003cbc9c: strb r3, [r0, #0x4a]
003cbca0: pop {r4, r5, r6, r7, r8, pc}
003cbca4: bl #0x3cb77c
003cbca8: pop {r4, r5, r6, r7, r8, pc}
003cbcac: ldr r0, [r0, #4]
003cbcb0: add r0, r0, #0x560
003cbcb4: bl #0x3df3f0
003cbcb8: pop {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr r3, [r0]
003cbcc0: cmp r2, #0
003cbcc4: mvneq r1, #0
003cbcc8: ldr r4, [r3, #0x90]
003cbccc: beq #0x3cbce4
003cbcd0: mov r0, r2
003cbcd4: ldr r3, [r2]
003cbcd8: mov lr, pc
003cbcdc: ldr pc, [r3]
003cbce0: mov r1, r0
003cbce4: mov r0, r5
003cbce8: blx r4
003cbcec: pop {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr r0, [r0, #4]
003cbcf4: bl #0x394a3c
003cbcf8: b #0x3cbc5c
003cbcfc: ldr r0, [r0, #4]
003cbd00: ldr r1, [r0, #0x378]
003cbd04: ldrb r2, [r1, #9]
003cbd08: cmp r2, #0
003cbd0c: bne #0x3cbd30
003cbd10: ldr r2, [pc, #0x4fc]
003cbd14: ldr r3, [r3, r2]
003cbd18: ldrb r3, [r3]
003cbd1c: cmp r3, #0
003cbd20: bne #0x3cbc5c
003cbd24: ldrb r3, [r1, #8]
003cbd28: cmp r3, #0
003cbd2c: bne #0x3cbc5c
003cbd30: sub r3, r4, #4
003cbd34: cmp r3, #0x3a
003cbd38: addls pc, pc, r3, lsl #2
003cbd3c: b #0x3cbc60
003cbd40: b #0x3cc1f8
003cbd44: b #0x3cbc60
003cbd48: b #0x3cbc60
003cbd4c: b #0x3cc1e0
003cbd50: b #0x3cc1c8
003cbd54: b #0x3cc1ac
003cbd58: b #0x3cc198
003cbd5c: b #0x3cc184
003cbd60: b #0x3cc170
003cbd64: b #0x3cc15c
003cbd68: b #0x3cc148
003cbd6c: b #0x3cc134
003cbd70: b #0x3cc120
003cbd74: b #0x3cc10c
003cbd78: b #0x3cc0f8
003cbd7c: b #0x3cc0e4
003cbd80: b #0x3cc0d0
003cbd84: b #0x3cc0bc
003cbd88: b #0x3cc0a8
003cbd8c: b #0x3cc094
003cbd90: b #0x3cc080
003cbd94: b #0x3cc06c
003cbd98: b #0x3cbc60
003cbd9c: b #0x3cbc60
003cbda0: b #0x3cbc60
003cbda4: b #0x3cc044
003cbda8: b #0x3cc038
003cbdac: b #0x3cc02c
003cbdb0: b #0x3cc020
003cbdb4: b #0x3cc014
003cbdb8: b #0x3cbc60
003cbdbc: b #0x3cbc60
003cbdc0: b #0x3cc004
003cbdc4: b #0x3cbff4
003cbdc8: b #0x3cbfe4
003cbdcc: b #0x3cbfd4
003cbdd0: b #0x3cbc60
003cbdd4: b #0x3cbc60
003cbdd8: b #0x3cbfbc
003cbddc: b #0x3cbfa4
003cbde0: b #0x3cbf8c
003cbde4: b #0x3cbc60
003cbde8: b #0x3cbc60
003cbdec: b #0x3cbc60
003cbdf0: b #0x3cbc60
003cbdf4: b #0x3cbc60
003cbdf8: b #0x3cbc60
003cbdfc: b #0x3cbc60
003cbe00: b #0x3cbc60
003cbe04: b #0x3cbc60
003cbe08: b #0x3cbc60
003cbe0c: b #0x3cbf70
003cbe10: b #0x3cbf54
003cbe14: b #0x3cbf38
003cbe18: b #0x3cbf1c
003cbe1c: b #0x3cbf00
003cbe20: b #0x3cbee4
003cbe24: b #0x3cbec8
003cbe28: b #0x3cbeac
003cbe2c: mov r1, r2
003cbe30: ldr r3, [r5]
003cbe34: mov lr, pc
003cbe38: ldr pc, [r3, #0x28]
003cbe3c: pop {r4, r5, r6, r7, r8, pc}
003cbe40: bl #0x3d3aec
003cbe44: ldr r3, [r5]
003cbe48: mov r7, r0
003cbe4c: mov r0, r5
003cbe50: mov lr, pc
003cbe54: ldr pc, [r3, #0x98]
003cbe58: cmp r7, #0
003cbe5c: bne #0x3cbc5c
003cbe60: pop {r4, r5, r6, r7, r8, pc}
003cbe64: bl #0x3d3ae4
003cbe68: ldr r3, [r5]
003cbe6c: mov r7, r0
003cbe70: mov r0, r5
003cbe74: mov lr, pc
003cbe78: ldr pc, [r3, #0x98]
003cbe7c: b #0x3cbe58
003cbe80: mov r1, r2
003cbe84: bl #0x3d4434
003cbe88: mov r7, r0
003cbe8c: b #0x3cbe58
003cbe90: movw r4, #0xc352
003cbe94: b #0x3cbc5c
003cbe98: ldr r3, [r0]
003cbe9c: mov r1, r2
003cbea0: mov lr, pc
003cbea4: ldr pc, [r3, #0x24]
003cbea8: b #0x3cbc5c
003cbeac: mov r0, r5
003cbeb0: mov r1, r6
003cbeb4: ldr r3, [r5]
003cbeb8: mov r2, #0
003cbebc: mov lr, pc
003cbec0: ldr pc, [r3, #0xc8]
003cbec4: pop {r4, r5, r6, r7, r8, pc}
003cbec8: mov r0, r5
003cbecc: mov r1, r6
003cbed0: ldr r3, [r5]
003cbed4: mov r2, #1
003cbed8: mov lr, pc
003cbedc: ldr pc, [r3, #0xc8]
003cbee0: pop {r4, r5, r6, r7, r8, pc}
003cbee4: mov r0, r5
003cbee8: mov r1, r6
003cbeec: ldr r3, [r5]
003cbef0: mov r2, #0
003cbef4: mov lr, pc
003cbef8: ldr pc, [r3, #0xc4]
003cbefc: pop {r4, r5, r6, r7, r8, pc}
003cbf00: mov r0, r5
003cbf04: mov r1, r6
003cbf08: ldr r3, [r5]
003cbf0c: mov r2, #1
003cbf10: mov lr, pc
003cbf14: ldr pc, [r3, #0xc4]
003cbf18: pop {r4, r5, r6, r7, r8, pc}
003cbf1c: mov r0, r5
003cbf20: mov r1, r6
003cbf24: ldr r3, [r5]
003cbf28: mov r2, #0
003cbf2c: mov lr, pc
003cbf30: ldr pc, [r3, #0xc0]
003cbf34: pop {r4, r5, r6, r7, r8, pc}
003cbf38: mov r0, r5
003cbf3c: mov r1, r6
003cbf40: ldr r3, [r5]
003cbf44: mov r2, #1
003cbf48: mov lr, pc
003cbf4c: ldr pc, [r3, #0xc0]
003cbf50: pop {r4, r5, r6, r7, r8, pc}
003cbf54: mov r0, r5
003cbf58: mov r1, r6
003cbf5c: ldr r3, [r5]
003cbf60: mov r2, #0
003cbf64: mov lr, pc
003cbf68: ldr pc, [r3, #0xbc]
003cbf6c: pop {r4, r5, r6, r7, r8, pc}
003cbf70: mov r0, r5
003cbf74: mov r1, r6
003cbf78: ldr r3, [r5]
003cbf7c: mov r2, #1
003cbf80: mov lr, pc
003cbf84: ldr pc, [r3, #0xbc]
003cbf88: pop {r4, r5, r6, r7, r8, pc}
003cbf8c: mov r0, r5
003cbf90: ldr r3, [r5]
003cbf94: mov lr, pc
003cbf98: ldr pc, [r3, #0x88]
003cbf9c: ldr r0, [r5, #4]
003cbfa0: b #0x3cbc60
003cbfa4: mov r0, r5
003cbfa8: ldr r3, [r5]
003cbfac: mov lr, pc
003cbfb0: ldr pc, [r3, #0x84]
003cbfb4: ldr r0, [r5, #4]
003cbfb8: b #0x3cbc60
003cbfbc: mov r0, r5
003cbfc0: ldr r3, [r5]
003cbfc4: mov lr, pc
003cbfc8: ldr pc, [r3, #0x8c]
003cbfcc: ldr r0, [r5, #4]
003cbfd0: b #0x3cbc60
003cbfd4: mov r0, r5
003cbfd8: bl #0x3d3ff8
003cbfdc: mov r7, r0
003cbfe0: b #0x3cbe58
003cbfe4: mov r0, r5
003cbfe8: bl #0x3d4204
003cbfec: mov r7, r0
003cbff0: b #0x3cbe58
003cbff4: mov r0, r5
003cbff8: bl #0x3d3d30
003cbffc: mov r7, r0
003cc000: b #0x3cbe58
003cc004: mov r0, r5
003cc008: bl #0x3d3d4c
003cc00c: mov r7, r0
003cc010: b #0x3cbe58
003cc014: mov r0, r5
003cc018: pop {r4, r5, r6, r7, r8, lr}
003cc01c: b #0x3d8b28
003cc020: mov r0, r5
003cc024: pop {r4, r5, r6, r7, r8, lr}
003cc028: b #0x3d8038
003cc02c: mov r0, r5
003cc030: pop {r4, r5, r6, r7, r8, lr}
003cc034: b #0x3d8b7c
003cc038: mov r0, r5
003cc03c: pop {r4, r5, r6, r7, r8, lr}
003cc040: b #0x3d808c
003cc044: ldr r3, [r5]
003cc048: add r0, r0, #0x4f0
003cc04c: add r0, r0, #0xc
003cc050: ldr r4, [r3, #0x20]
003cc054: bl #0x3c01ac
003cc058: mov r1, r6
003cc05c: mov r2, r0
003cc060: mov r0, r5
003cc064: blx r4
003cc068: pop {r4, r5, r6, r7, r8, pc}
003cc06c: mov r0, r5
003cc070: ldr r3, [r5]
003cc074: mov lr, pc
003cc078: ldr pc, [r3, #0x7c]
003cc07c: pop {r4, r5, r6, r7, r8, pc}
003cc080: mov r0, r5
003cc084: ldr r3, [r5]
003cc088: mov lr, pc
003cc08c: ldr pc, [r3, #0x78]
003cc090: pop {r4, r5, r6, r7, r8, pc}
003cc094: mov r0, r5
003cc098: ldr r3, [r5]
003cc09c: mov lr, pc
003cc0a0: ldr pc, [r3, #0x74]
003cc0a4: pop {r4, r5, r6, r7, r8, pc}
003cc0a8: mov r0, r5
003cc0ac: ldr r3, [r5]
003cc0b0: mov lr, pc
003cc0b4: ldr pc, [r3, #0x70]
003cc0b8: pop {r4, r5, r6, r7, r8, pc}
003cc0bc: mov r0, r5
003cc0c0: ldr r3, [r5]
003cc0c4: mov lr, pc
003cc0c8: ldr pc, [r3, #0x6c]
003cc0cc: pop {r4, r5, r6, r7, r8, pc}
003cc0d0: mov r0, r5
003cc0d4: ldr r3, [r5]
003cc0d8: mov lr, pc
003cc0dc: ldr pc, [r3, #0x68]
003cc0e0: pop {r4, r5, r6, r7, r8, pc}
003cc0e4: mov r0, r5
003cc0e8: ldr r3, [r5]
003cc0ec: mov lr, pc
003cc0f0: ldr pc, [r3, #0x64]
003cc0f4: pop {r4, r5, r6, r7, r8, pc}
003cc0f8: mov r0, r5
003cc0fc: ldr r3, [r5]
003cc100: mov lr, pc
003cc104: ldr pc, [r3, #0x60]
003cc108: pop {r4, r5, r6, r7, r8, pc}
003cc10c: mov r0, r5
003cc110: ldr r3, [r5]
003cc114: mov lr, pc
003cc118: ldr pc, [r3, #0x5c]
003cc11c: pop {r4, r5, r6, r7, r8, pc}
003cc120: mov r0, r5
003cc124: ldr r3, [r5]
003cc128: mov lr, pc
003cc12c: ldr pc, [r3, #0x58]
003cc130: pop {r4, r5, r6, r7, r8, pc}
003cc134: mov r0, r5
003cc138: ldr r3, [r5]
003cc13c: mov lr, pc
003cc140: ldr pc, [r3, #0x54]
003cc144: pop {r4, r5, r6, r7, r8, pc}
003cc148: mov r0, r5
003cc14c: ldr r3, [r5]
003cc150: mov lr, pc
003cc154: ldr pc, [r3, #0x50]
003cc158: pop {r4, r5, r6, r7, r8, pc}
003cc15c: mov r0, r5
003cc160: ldr r3, [r5]
003cc164: mov lr, pc
003cc168: ldr pc, [r3, #0x4c]
003cc16c: pop {r4, r5, r6, r7, r8, pc}
003cc170: mov r0, r5
003cc174: ldr r3, [r5]
003cc178: mov lr, pc
003cc17c: ldr pc, [r3, #0x48]
003cc180: pop {r4, r5, r6, r7, r8, pc}
003cc184: mov r0, r5
003cc188: ldr r3, [r5]
003cc18c: mov lr, pc
003cc190: ldr pc, [r3, #0x44]
003cc194: pop {r4, r5, r6, r7, r8, pc}
003cc198: mov r0, r5
003cc19c: ldr r3, [r5]
003cc1a0: mov lr, pc
003cc1a4: ldr pc, [r3, #0x40]
003cc1a8: pop {r4, r5, r6, r7, r8, pc}
003cc1ac: mov r0, r5
003cc1b0: ldr r3, [r5]
003cc1b4: mov r1, r6
003cc1b8: mov lr, pc
003cc1bc: ldr pc, [r3, #0x34]
003cc1c0: ldr r0, [r5, #4]
003cc1c4: b #0x3cbc60
003cc1c8: mov r0, r5
003cc1cc: mov r1, r6
003cc1d0: ldr r3, [r5]
003cc1d4: mov lr, pc
003cc1d8: ldr pc, [r3, #0x30]
003cc1dc: pop {r4, r5, r6, r7, r8, pc}
003cc1e0: mov r0, r5
003cc1e4: mov r1, r6
003cc1e8: ldr r3, [r5]
003cc1ec: mov lr, pc
003cc1f0: ldr pc, [r3, #0x2c]
003cc1f4: pop {r4, r5, r6, r7, r8, pc}
003cc1f8: mov r0, r5
003cc1fc: mov r1, r6
003cc200: ldr r3, [r5]
003cc204: mov lr, pc
003cc208: ldr pc, [r3, #0xb0]
003cc20c: pop {r4, r5, r6, r7, r8, pc}
003cc210: subseq r8, ip, r4, asr pc
003cc214: andeq r3, r0, r0, asr r6

_ZN30ObjectiveTemplate_InteractWithIN7Structs16v2QuestTalkToNPCE12QE_TalkToNPCE11handleEventEPK6IEventPK12EventManager 0x47f50c 148
0047f50c: push {r4, lr}
0047f510: ldr r3, [r1, #0x18]
0047f514: ldr r2, [r0, #0x24]
0047f518: mov r4, r0
0047f51c: ldr r0, [r0, #0xc]
0047f520: cmp r2, r3
0047f524: beq #0x47f530
0047f528: mov r0, #0
0047f52c: pop {r4, pc}
0047f530: ldrb r3, [r1, #0x11]
0047f534: cmp r3, #0
0047f538: bne #0x47f588
0047f53c: ldr r3, [r4, #0x20]
0047f540: add r3, r3, #1
0047f544: str r3, [r4, #0x20]
0047f548: mov r3, #1
0047f54c: strb r3, [r1, #0x10]
0047f550: ldr r3, [r4, #0x20]
0047f554: str r3, [r1, #0x14]
0047f558: ldr r3, [r4, #0x20]
0047f55c: ldr r2, [r0, #0x28]
0047f560: cmp r2, r3
0047f564: bgt #0x47f528
0047f568: mov r0, r4
0047f56c: bl #0x47ba10
0047f570: mov r0, r4
0047f574: ldr r3, [r4]
0047f578: mov lr, pc
0047f57c: ldr pc, [r3, #0x1c]
0047f580: mov r0, #0
0047f584: pop {r4, pc}
0047f588: ldr r3, [r1, #0x14]
0047f58c: ldr r2, [r4, #0x20]
0047f590: cmp r2, r3
0047f594: strlt r3, [r4, #0x20]
0047f598: blt #0x47f55c
0047f59c: b #0x47f528

_ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIN12EventManager12ReceiverInfoESaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE13insert_uniqueENS_17_Rb_tree_iteratorISA_SE_EERKSA_ 0x338924 884
00338924: push {r4, r5, r6, r7, r8, sl, lr}
00338928: ldr r4, [r2]
0033892c: ldr r2, [r1, #8]
00338930: sub sp, sp, #0x2c
00338934: mov r5, r1
00338938: cmp r4, r2
0033893c: mov r7, r0
00338940: mov r6, r3
00338944: beq #0x338ab4
00338948: cmp r4, r1
0033894c: beq #0x338b34
00338950: ldrb r3, [r4]
00338954: cmp r3, #0
00338958: beq #0x338a48
0033895c: ldr ip, [r4, #8]
00338960: cmp ip, #0
00338964: bne #0x338970
00338968: b #0x338a68
0033896c: mov ip, r3
00338970: ldr r3, [ip, #0xc]
00338974: cmp r3, #0
00338978: bne #0x33896c
0033897c: ldr r2, [r6]
00338980: ldr r0, [r4, #0x10]
00338984: cmp r2, r0
00338988: movge r1, #0
0033898c: movlt r1, #1
00338990: cmp r1, #0
00338994: bne #0x338a08
00338998: ldr r8, [r4, #0xc]
0033899c: cmp r8, #0
003389a0: beq #0x338b9c
003389a4: mov ip, r8
003389a8: b #0x3389b0
003389ac: mov ip, r3
003389b0: ldr r3, [ip, #8]
003389b4: cmp r3, #0
003389b8: bne #0x3389ac
003389bc: cmp r1, #0
003389c0: bne #0x338a98
003389c4: cmp r2, r0
003389c8: ble #0x338b5c
003389cc: cmp r5, ip
003389d0: beq #0x3389e0
003389d4: ldr r3, [ip, #0x10]
003389d8: cmp r2, r3
003389dc: bge #0x338a98
003389e0: cmp r8, #0
003389e4: bne #0x338b14
003389e8: mov r1, r5
003389ec: mov r2, r4
003389f0: mov r3, r6
003389f4: mov r0, r7
003389f8: str r8, [sp]
003389fc: str r4, [sp, #4]
00338a00: bl #0x3386c4
00338a04: b #0x338a3c
00338a08: ldr r3, [ip, #0x10]
00338a0c: cmp r2, r3
00338a10: ble #0x338998
00338a14: ldr lr, [ip, #0xc]
00338a18: cmp lr, #0
00338a1c: beq #0x338b7c
00338a20: mov ip, #0
00338a24: mov r1, r5
00338a28: mov r2, r4
00338a2c: mov r3, r6
00338a30: mov r0, r7
00338a34: stm sp, {r4, ip}
00338a38: bl #0x3386c4
00338a3c: mov r0, r7
00338a40: add sp, sp, #0x2c
00338a44: pop {r4, r5, r6, r7, r8, sl, pc}
00338a48: ldr r3, [r4, #4]
00338a4c: ldr r3, [r3, #4]
00338a50: cmp r4, r3
00338a54: ldreq ip, [r4, #0xc]
00338a58: beq #0x33897c
00338a5c: ldr ip, [r4, #8]
00338a60: cmp ip, #0
00338a64: bne #0x338970
00338a68: ldr ip, [r4, #4]
00338a6c: ldr r3, [ip, #8]
00338a70: cmp r4, r3
00338a74: beq #0x338a80
00338a78: b #0x33897c
00338a7c: mov ip, r3
00338a80: ldr r3, [ip, #4]
00338a84: ldr r2, [r3, #8]
00338a88: cmp r2, ip
00338a8c: beq #0x338a7c
00338a90: mov ip, r3
00338a94: b #0x33897c
00338a98: mov r1, r5
00338a9c: mov r2, r6
00338aa0: add r0, sp, #8
00338aa4: bl #0x33879c
00338aa8: ldr r3, [sp, #8]
00338aac: str r3, [r7]
00338ab0: b #0x338a3c
00338ab4: ldr r2, [r1, #0x10]
00338ab8: cmp r2, #0
00338abc: beq #0x338c0c
00338ac0: ldr r2, [r3]
00338ac4: ldr ip, [r4, #0x10]
00338ac8: cmp r2, ip
00338acc: blt #0x338c24
00338ad0: ble #0x338b5c
00338ad4: ldr lr, [r4, #0xc]
00338ad8: cmp lr, #0
00338adc: beq #0x338bd4
00338ae0: mov ip, lr
00338ae4: b #0x338aec
00338ae8: mov ip, r3
00338aec: ldr r3, [ip, #8]
00338af0: cmp r3, #0
00338af4: bne #0x338ae8
00338af8: cmp r5, ip
00338afc: beq #0x338c74
00338b00: ldr r3, [ip, #0x10]
00338b04: cmp r2, r3
00338b08: bge #0x338c38
00338b0c: cmp lr, #0
00338b10: beq #0x338c54
00338b14: mov lr, #0
00338b18: mov r1, r5
00338b1c: mov r2, ip
00338b20: mov r3, r6
00338b24: mov r0, r7
00338b28: stm sp, {ip, lr}
00338b2c: bl #0x3386c4
00338b30: b #0x338a3c
00338b34: ldr r2, [r4, #0xc]
00338b38: ldr ip, [r3]
00338b3c: ldr lr, [r2, #0x10]
00338b40: cmp lr, ip
00338b44: bge #0x338b64
00338b48: mov ip, #0
00338b4c: str ip, [sp]
00338b50: str r4, [sp, #4]
00338b54: bl #0x3386c4
00338b58: b #0x338a3c
00338b5c: str r4, [r7]
00338b60: b #0x338a3c
00338b64: mov r2, r3
00338b68: add r0, sp, #0x10
00338b6c: bl #0x33879c
00338b70: ldr r3, [sp, #0x10]
00338b74: str r3, [r7]
00338b78: b #0x338a3c
00338b7c: mov r1, r5
00338b80: mov r2, ip
00338b84: mov r3, r6
00338b88: mov r0, r7
00338b8c: str lr, [sp]
00338b90: str ip, [sp, #4]
00338b94: bl #0x3386c4
00338b98: b #0x338a3c
00338b9c: ldr r3, [r4, #4]
00338ba0: ldr ip, [r3, #0xc]
00338ba4: cmp r4, ip
00338ba8: movne ip, r4
00338bac: bne #0x338bc4
00338bb0: mov ip, r3
00338bb4: ldr r3, [r3, #4]
00338bb8: ldr sl, [r3, #0xc]
00338bbc: cmp ip, sl
00338bc0: beq #0x338bb0
00338bc4: ldr sl, [ip, #0xc]
00338bc8: cmp r3, sl
00338bcc: movne ip, r3
00338bd0: b #0x3389bc
00338bd4: ldr r3, [r4, #4]
00338bd8: ldr r1, [r3, #0xc]
00338bdc: cmp r4, r1
00338be0: movne ip, r4
00338be4: bne #0x338bfc
00338be8: mov ip, r3
00338bec: ldr r3, [r3, #4]
00338bf0: ldr r1, [r3, #0xc]
00338bf4: cmp r1, ip
00338bf8: beq #0x338be8
00338bfc: ldr r1, [ip, #0xc]
00338c00: cmp r3, r1
00338c04: movne ip, r3
00338c08: b #0x338af8
00338c0c: mov r2, r3
00338c10: add r0, sp, #0x20
00338c14: bl #0x33879c
00338c18: ldr r3, [sp, #0x20]
00338c1c: str r3, [r7]
00338c20: b #0x338a3c
00338c24: mov ip, #0
00338c28: mov r2, r4
00338c2c: stm sp, {r4, ip}
00338c30: bl #0x3386c4
00338c34: b #0x338a3c
00338c38: mov r1, r5
00338c3c: mov r2, r6
00338c40: add r0, sp, #0x18
00338c44: bl #0x33879c
00338c48: ldr r3, [sp, #0x18]
00338c4c: str r3, [r7]
00338c50: b #0x338a3c
00338c54: mov r1, r5
00338c58: mov r2, r4
00338c5c: mov r3, r6
00338c60: mov r0, r7
00338c64: str lr, [sp]
00338c68: str r4, [sp, #4]
00338c6c: bl #0x3386c4
00338c70: b #0x338a3c
00338c74: mov ip, #0
00338c78: mov r1, r5
00338c7c: mov r2, r4
00338c80: mov r3, r6
00338c84: mov r0, r7
00338c88: str ip, [sp]
00338c8c: str r4, [sp, #4]
00338c90: bl #0x3386c4
00338c94: b #0x338a3c

_ZN23Objective_EventReceiver7onEventEPK6IEventPK12EventManager 0x47acf0 204
0047acf0: push {r4, r5, r6, lr}
0047acf4: ldrb r4, [r0, #8]
0047acf8: ldr r3, [pc, #0xa4]
0047acfc: sub sp, sp, #8
0047ad00: cmp r4, #0
0047ad04: mov r5, r0
0047ad08: mov r6, r1
0047ad0c: add r3, pc, r3
0047ad10: bne #0x47ad44
0047ad14: ldr r2, [pc, #0x8c]
0047ad18: ldr r2, [r3, r2]
0047ad1c: ldr r2, [r2]
0047ad20: cmp r2, #2
0047ad24: streq r4, [r4]
0047ad28: beq #0x47ad38
0047ad2c: cmp r2, #1
0047ad30: beq #0x47ad70
0047ad34: mov r4, #0
0047ad38: mov r0, r4
0047ad3c: add sp, sp, #8
0047ad40: pop {r4, r5, r6, pc}
0047ad44: ldrb r3, [r0, #0x14]
0047ad48: cmp r3, #0
0047ad4c: bne #0x47ad34
0047ad50: ldr r3, [r0]
0047ad54: mov lr, pc
0047ad58: ldr pc, [r3, #0x38]
0047ad5c: mov r1, r6
0047ad60: mov r4, r0
0047ad64: mov r0, r5
0047ad68: bl #0x47ac94
0047ad6c: b #0x47ad38
0047ad70: ldr r0, [pc, #0x34]
0047ad74: ldr r1, [pc, #0x34]
0047ad78: ldr r2, [pc, #0x34]
0047ad7c: ldr r0, [r3, r0]
0047ad80: ldr r3, [pc, #0x30]
0047ad84: mov ip, #0xd5
0047ad88: add r1, pc, r1
0047ad8c: add r2, pc, r2
0047ad90: add r3, pc, r3
0047ad94: add r0, r0, #0xa8
0047ad98: str ip, [sp]
0047ad9c: bl #0x30e004
0047ada0: b #0x47ad38
0047ada4: subseq sb, r1, r4, lsl #27
0047ada8: andeq r3, r0, r0, asr #19
0047adac: andeq r1, r0, r0, asr #19
0047adb0: subeq r3, r4, r0, asr r6
0047adb4: ldrdeq r3, r4, [r4], #-0x7c
0047adb8: subeq r2, r5, r8, asr lr

_ZN11Application27UnRegisterForIrrlichtEventsEPN6glitch14IEventReceiverE 0x329310 88
00329310: push {r4, r5, r6, lr}
00329314: subs r6, r1, #0
00329318: beq #0x329348
0032931c: mov r5, r0
00329320: ldr r0, [r5, #8]!
00329324: cmp r5, r0
00329328: beq #0x329348
0032932c: ldr r3, [r0, #8]
00329330: ldr r4, [r0]
00329334: cmp r6, r3
00329338: beq #0x32934c
0032933c: mov r0, r4
00329340: cmp r5, r0
00329344: bne #0x32932c
00329348: pop {r4, r5, r6, pc}
0032934c: ldr r3, [r0, #4]
00329350: mov r1, #0xc
00329354: str r4, [r3]
00329358: str r3, [r4, #4]
0032935c: bl #0x708f00
00329360: mov r0, r4
00329364: b #0x329340

_ZN12EventManager17DropDelayedDetachEv 0x3383f4 68
003383f4: push {r4, r5, r6, lr}
003383f8: mov r5, r0
003383fc: ldr r4, [r5, #0x28]!
00338400: b #0x338424
00338404: ldr r0, [r4, #0xc]
00338408: mov r1, #0x14
0033840c: ldr r3, [r0]
00338410: ldr r2, [r0, #4]
00338414: str r3, [r2]
00338418: str r2, [r3, #4]
0033841c: bl #0x708f00
00338420: ldr r4, [r4]
00338424: cmp r5, r4
00338428: bne #0x338404
0033842c: mov r0, r5
00338430: pop {r4, r5, r6, lr}
00338434: b #0x3383b4

_ZN6IEventD1Ev 0x31d8d8 4
0031d8d8: bx lr

_ZNK16GameEventManager15LoopOnAllEventsEM9GameEventKFvvE 0x4797b0 100
004797b0: push {r4, r5, r6, r7, r8, lr}
004797b4: ldr r4, [r0]
004797b8: ldr r3, [r0, #4]
004797bc: sub sp, sp, #8
004797c0: mov r5, r0
004797c4: cmp r4, r3
004797c8: stm sp, {r1, r2}
004797cc: mov r7, r1
004797d0: beq #0x47980c
004797d4: asr r6, r2, #1
004797d8: and r8, r2, #1
004797dc: ldr r0, [r4]
004797e0: cmp r8, #0
004797e4: moveq r3, r7
004797e8: ldrne r3, [r0, r6]
004797ec: addeq r0, r0, r6
004797f0: addne r0, r0, r6
004797f4: ldrne r3, [r3, r7]
004797f8: blx r3
004797fc: ldr r3, [r5, #4]
00479800: add r4, r4, #4
00479804: cmp r4, r3
00479808: bne #0x4797dc
0047980c: add sp, sp, #8
00479810: pop {r4, r5, r6, r7, r8, pc}

_ZN19GS_InterruptLoading7onEventEPK6IEventPK12EventManager 0x417424 8
00417424: mov r0, #0
00417428: bx lr

_ZNSt4priv8_Rb_treeIiSt4lessIiESt4pairIKiSt4listIN12EventManager12ReceiverInfoESaIS7_EEENS_10_Select1stISA_EENS_11_MapTraitsTISA_EESaISA_EE14_M_create_nodeERKSA_ 0x33862c 152
0033862c: push {r4, r5, r6, r7, lr}
00338630: sub sp, sp, #0xc
00338634: mov r3, #0x1c
00338638: add r0, sp, #8
0033863c: str r3, [r0, #-4]!
00338640: mov r6, r1
00338644: bl #0x708ec0
00338648: ldr r3, [r6]
0033864c: add r5, r0, #0x14
00338650: str r5, [r0, #0x14]
00338654: str r3, [r0, #0x10]
00338658: str r5, [r0, #0x18]
0033865c: ldr r4, [r6, #4]!
00338660: mov r7, r0
00338664: cmp r4, r6
00338668: beq #0x3386ac
0033866c: mov r0, r5
00338670: bl #0x33860c
00338674: ldr r3, [r4, #8]
00338678: str r3, [r0, #8]
0033867c: ldr r3, [r4, #0xc]
00338680: str r3, [r0, #0xc]
00338684: ldrb r3, [r4, #0x10]
00338688: strb r3, [r0, #0x10]
0033868c: ldr r3, [r5, #4]
00338690: str r5, [r0]
00338694: str r3, [r0, #4]
00338698: str r0, [r3]
0033869c: str r0, [r5, #4]
003386a0: ldr r4, [r4]
003386a4: cmp r6, r4
003386a8: bne #0x33866c
003386ac: mov r3, #0
003386b0: str r3, [r7, #0xc]
003386b4: str r3, [r7, #8]
003386b8: mov r0, r7
003386bc: add sp, sp, #0xc
003386c0: pop {r4, r5, r6, r7, pc}

_ZN20Objective_MoveInZone11handleEventEPK6IEventPK12EventManager 0x47bcd4 172
0047bcd4: push {r4, r5, r6, r7, r8, lr}
0047bcd8: ldr r2, [r1, #0x18]
0047bcdc: ldr r3, [r0, #0x20]
0047bce0: mov r4, r0
0047bce4: mov r5, r1
0047bce8: cmp r2, r3
0047bcec: ldr r7, [r0, #0xc]
0047bcf0: ldr r6, [r1, #8]
0047bcf4: beq #0x47bd00
0047bcf8: mov r0, #0
0047bcfc: pop {r4, r5, r6, r7, r8, pc}
0047bd00: cmp r6, #0
0047bd04: beq #0x47bd70
0047bd08: ldr r8, [r7, #0x24]
0047bd0c: cmn r8, #1
0047bd10: beq #0x47bd50
0047bd14: mov r0, r6
0047bd18: bl #0x3b3d38
0047bd1c: cmp r0, r8
0047bd20: bne #0x47bcf8
0047bd24: mov r0, r4
0047bd28: bl #0x47ba10
0047bd2c: ldr r3, [r4]
0047bd30: mov r0, r4
0047bd34: mov lr, pc
0047bd38: ldr pc, [r3, #0x1c]
0047bd3c: ldrb r3, [r5, #0x11]
0047bd40: cmp r3, #0
0047bd44: moveq r3, #1
0047bd48: strbeq r3, [r5, #0x10]
0047bd4c: b #0x47bcf8
0047bd50: ldr r3, [r6]
0047bd54: mov r0, r6
0047bd58: mov lr, pc
0047bd5c: ldr pc, [r3, #0x28]
0047bd60: cmp r0, #0
0047bd64: bne #0x47bd24
0047bd68: ldr r8, [r7, #0x24]
0047bd6c: b #0x47bd14
0047bd70: ldrb r3, [r1, #0x11]
0047bd74: cmp r3, #0
0047bd78: bne #0x47bd24
0047bd7c: b #0x47bd08

_ZN6IEventD0Ev 0x31fc20 52
0031fc20: ldr r3, [pc, #0x24]
0031fc24: ldr r2, [pc, #0x24]
0031fc28: push {r4, lr}
0031fc2c: add r3, pc, r3
0031fc30: ldr r2, [r3, r2]
0031fc34: mov r4, r0
0031fc38: add r2, r2, #8
0031fc3c: str r2, [r0]
0031fc40: bl #0x310440
0031fc44: mov r0, r4
0031fc48: pop {r4, pc}
0031fc4c: rsbeq r4, r7, r4, ror #28
0031fc50: strheq r0, [r0], -r0

_ZN30ObjectiveTemplate_InteractWithIN7Structs16v2QuestTriggerOnE12QE_TriggerOnE11handleEventEPK6IEventPK12EventManager 0x47f634 148
0047f634: push {r4, lr}
0047f638: ldr r3, [r1, #0x18]
0047f63c: ldr r2, [r0, #0x24]
0047f640: mov r4, r0
0047f644: ldr r0, [r0, #0xc]
0047f648: cmp r2, r3
0047f64c: beq #0x47f658
0047f650: mov r0, #0
0047f654: pop {r4, pc}
0047f658: ldrb r3, [r1, #0x11]
0047f65c: cmp r3, #0
0047f660: bne #0x47f6b0
0047f664: ldr r3, [r4, #0x20]
0047f668: add r3, r3, #1
0047f66c: str r3, [r4, #0x20]
0047f670: mov r3, #1
0047f674: strb r3, [r1, #0x10]
0047f678: ldr r3, [r4, #0x20]
0047f67c: str r3, [r1, #0x14]
0047f680: ldr r3, [r4, #0x20]
0047f684: ldr r2, [r0, #0x28]
0047f688: cmp r2, r3
0047f68c: bgt #0x47f650
0047f690: mov r0, r4
0047f694: bl #0x47ba10
0047f698: mov r0, r4
0047f69c: ldr r3, [r4]
0047f6a0: mov lr, pc
0047f6a4: ldr pc, [r3, #0x1c]
0047f6a8: mov r0, #0
0047f6ac: pop {r4, pc}
0047f6b0: ldr r3, [r1, #0x14]
0047f6b4: ldr r2, [r4, #0x20]
0047f6b8: cmp r2, r3
0047f6bc: strlt r3, [r4, #0x20]
0047f6c0: blt #0x47f684
0047f6c4: b #0x47f650

_ZN12MenuWorldMap12InputHandler7onEventEPK6IEventPK12EventManager 0x4372bc 1640
004372bc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004372c0: mov r7, r0
004372c4: sub sp, sp, #0x74
004372c8: ldr r3, [r1]
004372cc: mov r0, r1
004372d0: mov r5, r1
004372d4: mov lr, pc
004372d8: ldr pc, [r3, #8]
004372dc: cmp r0, #4
004372e0: beq #0x4372fc
004372e4: cmp r0, #5
004372e8: movne r8, #0
004372ec: beq #0x437370
004372f0: mov r0, r8
004372f4: add sp, sp, #0x74
004372f8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004372fc: ldrb r3, [r5, #0x10]
00437300: cmp r3, #0
00437304: bne #0x4377e4
00437308: ldr r3, [r7, #0xc]
0043730c: cmp r3, #0
00437310: beq #0x43735c
00437314: add r6, r7, #8
00437318: ldr r0, [r5, #0xc]
0043731c: mov r1, r6
00437320: b #0x437328
00437324: mov r3, r2
00437328: ldr r2, [r3, #0x10]
0043732c: cmp r2, r0
00437330: ldrlt r2, [r3, #0xc]
00437334: ldrge r2, [r3, #8]
00437338: movlt r3, r1
0043733c: mov r1, r3
00437340: cmp r2, #0
00437344: bne #0x437324
00437348: cmp r6, r3
0043734c: beq #0x43735c
00437350: ldr r2, [r3, #0x10]
00437354: cmp r2, r0
00437358: ble #0x4377c0
0043735c: ldr r8, [r7, #0x18]
00437360: cmp r8, #1
00437364: movls r8, #0
00437368: movhi r8, #1
0043736c: b #0x4372f0
00437370: ldr lr, [r7, #0xc]
00437374: add r4, r5, #0xc
00437378: add r6, r7, #8
0043737c: cmp lr, #0
00437380: beq #0x437918
00437384: ldr ip, [r5, #0xc]
00437388: mov r2, r6
0043738c: b #0x437394
00437390: mov lr, r3
00437394: ldr r3, [lr, #0x10]
00437398: cmp r3, ip
0043739c: ldrlt r3, [lr, #0xc]
004373a0: ldrge r3, [lr, #8]
004373a4: movlt lr, r2
004373a8: mov r2, lr
004373ac: cmp r3, #0
004373b0: bne #0x437390
004373b4: cmp r6, lr
004373b8: beq #0x437788
004373bc: ldr r2, [lr, #0x10]
004373c0: mov r3, lr
004373c4: cmp ip, r2
004373c8: blt #0x437788
004373cc: ldrsh r3, [r3, #0x14]
004373d0: cmp r3, #0
004373d4: beq #0x437854
004373d8: ldr lr, [r7, #0xc]
004373dc: cmp lr, #0
004373e0: beq #0x437900
004373e4: ldr ip, [r5, #0xc]
004373e8: mov r2, r6
004373ec: b #0x4373f4
004373f0: mov lr, r3
004373f4: ldr r3, [lr, #0x10]
004373f8: cmp r3, ip
004373fc: ldrlt r3, [lr, #0xc]
00437400: ldrge r3, [lr, #8]
00437404: movlt lr, r2
00437408: mov r2, lr
0043740c: cmp r3, #0
00437410: bne #0x4373f0
00437414: cmp r6, lr
00437418: beq #0x437750
0043741c: ldr r2, [lr, #0x10]
00437420: mov r3, lr
00437424: cmp ip, r2
00437428: blt #0x437750
0043742c: ldrh r2, [r5, #8]
00437430: strh r2, [r3, #0x18]
00437434: ldr lr, [r7, #0xc]
00437438: cmp lr, #0
0043743c: beq #0x43790c
00437440: ldr ip, [r5, #0xc]
00437444: mov r2, r6
00437448: b #0x437450
0043744c: mov lr, r3
00437450: ldr r3, [lr, #0x10]
00437454: cmp r3, ip
00437458: ldrlt r3, [lr, #0xc]
0043745c: ldrge r3, [lr, #8]
00437460: movlt lr, r2
00437464: mov r2, lr
00437468: cmp r3, #0
0043746c: bne #0x43744c
00437470: cmp r6, lr
00437474: beq #0x437718
00437478: ldr r2, [lr, #0x10]
0043747c: mov r3, lr
00437480: cmp ip, r2
00437484: blt #0x437718
00437488: ldrh r2, [r5, #0xa]
0043748c: strh r2, [r3, #0x1a]
00437490: ldr r3, [r7, #0x18]
00437494: cmp r3, #1
00437498: bls #0x437898
0043749c: ldr r4, [r7, #0x10]
004374a0: ldrsh r0, [r4, #0x14]
004374a4: bl #0x30e964
004374a8: str r0, [sp, #4]
004374ac: ldrsh r0, [r4, #0x16]
004374b0: bl #0x30e964
004374b4: mov fp, r0
004374b8: ldrsh r0, [r4, #0x18]
004374bc: bl #0x30e964
004374c0: mov r8, r0
004374c4: ldrsh r0, [r4, #0x1a]
004374c8: bl #0x30e964
004374cc: ldr r1, [r4, #0xc]
004374d0: mov r3, r0
004374d4: cmp r1, #0
004374d8: bne #0x4374e4
004374dc: b #0x4378b4
004374e0: mov r1, r2
004374e4: ldr r2, [r1, #8]
004374e8: cmp r2, #0
004374ec: bne #0x4374e0
004374f0: mov r4, r1
004374f4: ldrsh r0, [r4, #0x14]
004374f8: str r3, [sp]
004374fc: bl #0x30e964
00437500: mov sl, r0
00437504: ldrsh r0, [r4, #0x16]
00437508: bl #0x30e964
0043750c: mov r1, sl
00437510: mov sb, r0
00437514: ldr r0, [sp, #4]
00437518: bl #0x30e3ac
0043751c: mov r1, sb
00437520: mov sl, r0
00437524: mov r0, fp
00437528: bl #0x30e3ac
0043752c: mov r1, sl
00437530: mov sb, r0
00437534: mov r0, sl
00437538: bl #0x30ed6c
0043753c: mov r1, sb
00437540: mov sl, r0
00437544: mov r0, sb
00437548: bl #0x30ed6c
0043754c: mov r1, r0
00437550: mov r0, sl
00437554: bl #0x30eba4
00437558: bl #0x30e8a4
0043755c: bl #0x30e1c0
00437560: bl #0x30e6a0
00437564: mov sl, r0
00437568: ldrsh r0, [r4, #0x18]
0043756c: bl #0x30e964
00437570: mov fp, r0
00437574: ldrsh r0, [r4, #0x1a]
00437578: bl #0x30e964
0043757c: mov r1, fp
00437580: mov sb, r0
00437584: mov r0, r8
00437588: bl #0x30e3ac
0043758c: ldr r3, [sp]
00437590: mov r4, r0
00437594: mov r1, sb
00437598: mov r0, r3
0043759c: bl #0x30e3ac
004375a0: mov r1, r4
004375a4: mov r8, r0
004375a8: mov r0, r4
004375ac: bl #0x30ed6c
004375b0: mov r1, r8
004375b4: mov r4, r0
004375b8: mov r0, r8
004375bc: bl #0x30ed6c
004375c0: mov r1, r0
004375c4: mov r0, r4
004375c8: bl #0x30eba4
004375cc: bl #0x30e8a4
004375d0: bl #0x30e1c0
004375d4: bl #0x30e6a0
004375d8: mov r1, sl
004375dc: bl #0x30e3ac
004375e0: mov r1, r0
004375e4: mov r0, r7
004375e8: bl #0x435d74
004375ec: mov r8, #1
004375f0: ldr r4, [r7, #0xc]
004375f4: cmp r4, #0
004375f8: beq #0x4378e8
004375fc: ldr ip, [r5, #0xc]
00437600: mov r2, r6
00437604: b #0x43760c
00437608: mov r4, r3
0043760c: ldr r3, [r4, #0x10]
00437610: cmp r3, ip
00437614: ldrlt r3, [r4, #0xc]
00437618: ldrge r3, [r4, #8]
0043761c: movlt r4, r2
00437620: mov r2, r4
00437624: cmp r3, #0
00437628: bne #0x437608
0043762c: cmp r6, r4
00437630: beq #0x4376e0
00437634: ldr r2, [r4, #0x10]
00437638: mov r3, r4
0043763c: cmp ip, r2
00437640: blt #0x4376e0
00437644: ldrh r2, [r5, #8]
00437648: strh r2, [r3, #0x14]
0043764c: ldr r4, [r7, #0xc]
00437650: cmp r4, #0
00437654: beq #0x4378f4
00437658: ldr ip, [r5, #0xc]
0043765c: mov r2, r6
00437660: b #0x437668
00437664: mov r4, r3
00437668: ldr r3, [r4, #0x10]
0043766c: cmp r3, ip
00437670: ldrlt r3, [r4, #0xc]
00437674: ldrge r3, [r4, #8]
00437678: movlt r4, r2
0043767c: mov r2, r4
00437680: cmp r3, #0
00437684: bne #0x437664
00437688: cmp r6, r4
0043768c: beq #0x4376a0
00437690: ldr r2, [r4, #0x10]
00437694: mov r3, r4
00437698: cmp ip, r2
0043769c: bge #0x4376d4
004376a0: add r3, sp, #8
004376a4: str ip, [sp, #8]
004376a8: mov r1, r6
004376ac: mov ip, #0
004376b0: add r0, sp, #0x48
004376b4: add r2, sp, #0x44
004376b8: str r4, [sp, #0x44]
004376bc: strh ip, [sp, #0x12]
004376c0: strh ip, [sp, #0x10]
004376c4: strh ip, [sp, #0xe]
004376c8: strh ip, [sp, #0xc]
004376cc: bl #0x436940
004376d0: ldr r3, [sp, #0x48]
004376d4: ldrh r5, [r5, #0xa]
004376d8: strh r5, [r3, #0x16]
004376dc: b #0x4372f0
004376e0: add r3, sp, #0x14
004376e4: str ip, [sp, #0x14]
004376e8: add r0, sp, #0x50
004376ec: mov ip, #0
004376f0: mov r1, r6
004376f4: add r2, sp, #0x4c
004376f8: str r4, [sp, #0x4c]
004376fc: strh ip, [sp, #0x1e]
00437700: strh ip, [sp, #0x1c]
00437704: strh ip, [sp, #0x1a]
00437708: strh ip, [sp, #0x18]
0043770c: bl #0x436940
00437710: ldr r3, [sp, #0x50]
00437714: b #0x437644
00437718: add r3, sp, #0x20
0043771c: str ip, [sp, #0x20]
00437720: add r0, sp, #0x58
00437724: mov ip, #0
00437728: mov r1, r6
0043772c: add r2, sp, #0x54
00437730: str lr, [sp, #0x54]
00437734: strh ip, [sp, #0x2a]
00437738: strh ip, [sp, #0x28]
0043773c: strh ip, [sp, #0x26]
00437740: strh ip, [sp, #0x24]
00437744: bl #0x436940
00437748: ldr r3, [sp, #0x58]
0043774c: b #0x437488
00437750: add r3, sp, #0x2c
00437754: str ip, [sp, #0x2c]
00437758: add r0, sp, #0x60
0043775c: mov ip, #0
00437760: mov r1, r6
00437764: add r2, sp, #0x5c
00437768: str lr, [sp, #0x5c]
0043776c: strh ip, [sp, #0x36]
00437770: strh ip, [sp, #0x34]
00437774: strh ip, [sp, #0x32]
00437778: strh ip, [sp, #0x30]
0043777c: bl #0x436940
00437780: ldr r3, [sp, #0x60]
00437784: b #0x43742c
00437788: add r3, sp, #0x38
0043778c: str ip, [sp, #0x38]
00437790: add r0, sp, #0x68
00437794: mov ip, #0
00437798: mov r1, r6
0043779c: add r2, sp, #0x64
004377a0: str lr, [sp, #0x64]
004377a4: strh ip, [sp, #0x42]
004377a8: strh ip, [sp, #0x40]
004377ac: strh ip, [sp, #0x3e]
004377b0: strh ip, [sp, #0x3c]
004377b4: bl #0x436940
004377b8: ldr r3, [sp, #0x68]
004377bc: b #0x4373cc
004377c0: add r1, sp, #0x70
004377c4: str r3, [r1, #-4]!
004377c8: mov r0, r6
004377cc: bl #0x436e04
004377d0: ldr r8, [r7, #0x18]
004377d4: cmp r8, #1
004377d8: movls r8, #0
004377dc: movhi r8, #1
004377e0: b #0x4372f0
004377e4: add r4, r7, #8
004377e8: add r6, r5, #0xc
004377ec: mov r1, r6
004377f0: mov r0, r4
004377f4: bl #0x436cb4
004377f8: ldrh r2, [r5, #8]
004377fc: mov r1, r6
00437800: strh r2, [r0]
00437804: mov r0, r4
00437808: bl #0x436cb4
0043780c: ldrh r3, [r5, #0xa]
00437810: mov r1, r6
00437814: strh r3, [r0, #2]
00437818: mov r0, r4
0043781c: bl #0x436cb4
00437820: ldrh ip, [r5, #8]
00437824: mov r1, r6
00437828: strh ip, [r0, #4]
0043782c: mov r0, r4
00437830: bl #0x436cb4
00437834: ldrh r2, [r5, #0xa]
00437838: mov r1, #1
0043783c: strh r2, [r0, #6]
00437840: ldrsh r3, [r5, #0xa]
00437844: mov r0, r7
00437848: ldrsh r2, [r5, #8]
0043784c: bl #0x435a5c
00437850: b #0x43735c
00437854: mov r0, r6
00437858: mov r1, r4
0043785c: bl #0x436cb4
00437860: ldrsh r3, [r0, #2]
00437864: cmp r3, #0
00437868: bne #0x4373d8
0043786c: mov r1, r4
00437870: mov r0, r6
00437874: bl #0x436cb4
00437878: ldrh r2, [r5, #8]
0043787c: mov r1, r4
00437880: strh r2, [r0]
00437884: mov r0, r6
00437888: bl #0x436cb4
0043788c: ldrh r3, [r5, #0xa]
00437890: strh r3, [r0, #2]
00437894: b #0x4373d8
00437898: mov r0, r7
0043789c: mov r1, #0
004378a0: ldrsh r2, [r5, #8]
004378a4: ldrsh r3, [r5, #0xa]
004378a8: bl #0x435a5c
004378ac: mov r8, #0
004378b0: b #0x4375f0
004378b4: ldr r2, [r4, #4]
004378b8: ldr r0, [r2, #0xc]
004378bc: cmp r4, r0
004378c0: bne #0x4378dc
004378c4: mov r4, r2
004378c8: ldr r2, [r2, #4]
004378cc: ldr r1, [r2, #0xc]
004378d0: cmp r1, r4
004378d4: beq #0x4378c4
004378d8: ldr r1, [r4, #0xc]
004378dc: cmp r2, r1
004378e0: movne r4, r2
004378e4: b #0x4374f4
004378e8: ldr ip, [r5, #0xc]
004378ec: mov r4, r6
004378f0: b #0x43762c
004378f4: ldr ip, [r5, #0xc]
004378f8: mov r4, r6
004378fc: b #0x437688
00437900: ldr ip, [r5, #0xc]
00437904: mov lr, r6
00437908: b #0x437414
0043790c: ldr ip, [r5, #0xc]
00437910: mov lr, r6
00437914: b #0x437470
00437918: ldr ip, [r5, #0xc]
0043791c: mov lr, r6
00437920: b #0x4373b4

_ZN16GameEventManager6UpdateEv 0x4798c8 100
004798c8: push {r4, r5, r6, lr}
004798cc: ldr r5, [pc, #0x4c]
004798d0: sub sp, sp, #8
004798d4: mov r6, r0
004798d8: add r5, pc, r5
004798dc: mov r0, r5
004798e0: ldr r4, [pc, #0x3c]
004798e4: bl #0x3136b4
004798e8: ldr r2, [pc, #0x38]
004798ec: add r4, pc, r4
004798f0: mov r3, #0
004798f4: ldr ip, [r4, r2]
004798f8: mov r0, r6
004798fc: mov r2, r3
00479900: mov r1, ip
00479904: str ip, [sp]
00479908: str r3, [sp, #4]
0047990c: bl #0x479718
00479910: mov r0, r5
00479914: add sp, sp, #8
00479918: pop {r4, r5, r6, lr}
0047991c: b #0x3136b8
00479920: subeq r4, r5, r8, ror #4
00479924: subseq fp, r1, r4, lsr #3
00479928: andeq r4, r0, r4, ror #1

_ZN6glitch7CLogger11setReceiverEPNS_14IEventReceiverE 0x6a093c 8
006a093c: str r1, [r0, #0xc]
006a0940: bx lr

_ZThn12_N14CameraOverview7onEventEPK6IEventPK12EventManager 0x410928 8
00410928: sub r0, r0, #0xc
0041092c: b #0x410930

_ZN7Console16onEventNotOpenedEPK6IEventPK12EventManager 0x33271c 4528
0033271c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00332720: ldr r4, [pc, #0xe9c]
00332724: ldr r5, [pc, #0xe9c]
00332728: ldr r7, [pc, #0xe9c]
0033272c: add r4, pc, r4
00332730: ldr r3, [r4, r5]
00332734: ldr sl, [r4, r7]
00332738: sub sp, sp, #0x1fc
0033273c: ldr r3, [r3]
00332740: mov r6, r0
00332744: mov r0, sl
00332748: mov r8, r1
0033274c: str r3, [sp, #0x1f4]
00332750: bl #0x31f594
00332754: cmp r0, #0
00332758: beq #0x332768
0033275c: ldr r3, [r0, #0x130]
00332760: cmp r3, #0x26
00332764: beq #0x332788
00332768: ldr r3, [r4, r5]
0033276c: ldr r2, [sp, #0x1f4]
00332770: mov r0, #0
00332774: ldr r3, [r3]
00332778: cmp r2, r3
0033277c: bne #0x33389c
00332780: add sp, sp, #0x1fc
00332784: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00332788: ldr r3, [r8]
0033278c: mov r0, r8
00332790: mov lr, pc
00332794: ldr pc, [r3, #8]
00332798: cmp r0, #0
0033279c: bne #0x332768
003327a0: ldr r3, [r8, #0xc]
003327a4: cmp r3, #0x57
003327a8: beq #0x332abc
003327ac: cmp r3, #0x53
003327b0: beq #0x332b10
003327b4: cmp r3, #0x41
003327b8: beq #0x332b48
003327bc: cmp r3, #0x44
003327c0: beq #0x332b7c
003327c4: cmp r3, #0x58
003327c8: beq #0x332bb0
003327cc: ldr r3, [r8]
003327d0: mov r0, r8
003327d4: mov lr, pc
003327d8: ldr pc, [r3, #8]
003327dc: cmp r0, #0
003327e0: bne #0x332768
003327e4: ldrb r3, [r8, #0x10]
003327e8: cmp r3, #0
003327ec: beq #0x332768
003327f0: ldr r3, [r8, #0xc]
003327f4: sub r3, r3, #0x30
003327f8: cmp r3, #0xad
003327fc: addls pc, pc, r3, lsl #2
00332800: b #0x332768
00332804: b #0x332f08
00332808: b #0x332768
0033280c: b #0x332768
00332810: b #0x332768
00332814: b #0x332768
00332818: b #0x332768
0033281c: b #0x332768
00332820: b #0x332768
00332824: b #0x332768
00332828: b #0x332ea0
0033282c: b #0x332768
00332830: b #0x332768
00332834: b #0x332768
00332838: b #0x332768
0033283c: b #0x332768
00332840: b #0x332768
00332844: b #0x332768
00332848: b #0x332768
0033284c: b #0x332768
00332850: b #0x332768
00332854: b #0x332768
00332858: b #0x332768
0033285c: b #0x332bf0
00332860: b #0x332768
00332864: b #0x332768
00332868: b #0x332768
0033286c: b #0x332768
00332870: b #0x332e80
00332874: b #0x332768
00332878: b #0x332768
0033287c: b #0x332768
00332880: b #0x332768
00332884: b #0x332e0c
00332888: b #0x332768
0033288c: b #0x332e00
00332890: b #0x332768
00332894: b #0x332768
00332898: b #0x332de0
0033289c: b #0x332768
003328a0: b #0x332768
003328a4: b #0x332768
003328a8: b #0x332768
003328ac: b #0x332768
003328b0: b #0x332768
003328b4: b #0x332768
003328b8: b #0x332dd4
003328bc: b #0x332dc0
003328c0: b #0x332768
003328c4: b #0x332768
003328c8: b #0x332768
003328cc: b #0x332768
003328d0: b #0x332768
003328d4: b #0x332768
003328d8: b #0x332768
003328dc: b #0x332768
003328e0: b #0x332768
003328e4: b #0x332768
003328e8: b #0x332768
003328ec: b #0x332768
003328f0: b #0x332768
003328f4: b #0x332768
003328f8: b #0x332768
003328fc: b #0x332768
00332900: b #0x332768
00332904: b #0x332768
00332908: b #0x332768
0033290c: b #0x332768
00332910: b #0x332768
00332914: b #0x332768
00332918: b #0x332768
0033291c: b #0x332768
00332920: b #0x332768
00332924: b #0x332768
00332928: b #0x332768
0033292c: b #0x332768
00332930: b #0x332768
00332934: b #0x332768
00332938: b #0x332768
0033293c: b #0x332768
00332940: b #0x332768
00332944: b #0x332768
00332948: b #0x332768
0033294c: b #0x332768
00332950: b #0x332c94
00332954: b #0x333330
00332958: b #0x33322c
0033295c: b #0x333100
00332960: b #0x332768
00332964: b #0x332768
00332968: b #0x332768
0033296c: b #0x332768
00332970: b #0x3330c8
00332974: b #0x333090
00332978: b #0x332768
0033297c: b #0x332768
00332980: b #0x333000
00332984: b #0x332f70
00332988: b #0x332768
0033298c: b #0x332768
00332990: b #0x332768
00332994: b #0x332768
00332998: b #0x332768
0033299c: b #0x332768
003329a0: b #0x332768
003329a4: b #0x332768
003329a8: b #0x332768
003329ac: b #0x332768
003329b0: b #0x332768
003329b4: b #0x332768
003329b8: b #0x332768
003329bc: b #0x332768
003329c0: b #0x332768
003329c4: b #0x332768
003329c8: b #0x332768
003329cc: b #0x332768
003329d0: b #0x332768
003329d4: b #0x332768
003329d8: b #0x332768
003329dc: b #0x332768
003329e0: b #0x332768
003329e4: b #0x332768
003329e8: b #0x332768
003329ec: b #0x332768
003329f0: b #0x332768
003329f4: b #0x332768
003329f8: b #0x332768
003329fc: b #0x332768
00332a00: b #0x332768
00332a04: b #0x332768
00332a08: b #0x332768
00332a0c: b #0x332768
00332a10: b #0x332768
00332a14: b #0x332768
00332a18: b #0x332768
00332a1c: b #0x332768
00332a20: b #0x332768
00332a24: b #0x332768
00332a28: b #0x332768
00332a2c: b #0x332768
00332a30: b #0x332768
00332a34: b #0x332768
00332a38: b #0x332768
00332a3c: b #0x332768
00332a40: b #0x332768
00332a44: b #0x332768
00332a48: b #0x332768
00332a4c: b #0x332768
00332a50: b #0x332768
00332a54: b #0x332768
00332a58: b #0x332768
00332a5c: b #0x332768
00332a60: b #0x332768
00332a64: b #0x332768
00332a68: b #0x332768
00332a6c: b #0x332768
00332a70: b #0x332768
00332a74: b #0x332768
00332a78: b #0x332768
00332a7c: b #0x332768
00332a80: b #0x332768
00332a84: b #0x332768
00332a88: b #0x332768
00332a8c: b #0x332768
00332a90: b #0x332768
00332a94: b #0x332768
00332a98: b #0x332768
00332a9c: b #0x332768
00332aa0: b #0x332768
00332aa4: b #0x332768
00332aa8: b #0x332768
00332aac: b #0x332768
00332ab0: b #0x332f4c
00332ab4: b #0x332768
00332ab8: b #0x332ee4
00332abc: mov r0, sl
00332ac0: bl #0x31f594
00332ac4: cmp r0, #0
00332ac8: beq #0x332b40
00332acc: ldr sl, [r0, #0x128]
00332ad0: cmp sl, #0
00332ad4: beq #0x332b40
00332ad8: mov r1, #0x43000000
00332adc: ldr r0, [sl, #0x38]
00332ae0: add r1, r1, #0x480000
00332ae4: bl #0x30e3ac
00332ae8: ldr r3, [sl, #8]
00332aec: str r0, [sl, #0x38]
00332af0: movw r1, #0x5000
00332af4: mov r0, r3
00332af8: movt r1, #0x4743
00332afc: ldr r3, [r3]
00332b00: mov lr, pc
00332b04: ldr pc, [r3, #0x134]
00332b08: ldr r3, [r8, #0xc]
00332b0c: b #0x3327b4
00332b10: mov r0, sl
00332b14: bl #0x31f594
00332b18: cmp r0, #0
00332b1c: beq #0x332b40
00332b20: ldr sl, [r0, #0x128]
00332b24: cmp sl, #0
00332b28: beq #0x332b40
00332b2c: mov r1, #0x43000000
00332b30: ldr r0, [sl, #0x38]
00332b34: add r1, r1, #0x480000
00332b38: bl #0x30eba4
00332b3c: str r0, [sl, #0x38]
00332b40: ldr r3, [r8, #0xc]
00332b44: b #0x3327b4
00332b48: ldr r0, [r4, r7]
00332b4c: bl #0x31f594
00332b50: cmp r0, #0
00332b54: beq #0x3327cc
00332b58: ldr sl, [r0, #0x128]
00332b5c: cmp sl, #0
00332b60: beq #0x3327cc
00332b64: mov r1, #0x43000000
00332b68: ldr r0, [sl, #0x3c]
00332b6c: add r1, r1, #0x480000
00332b70: bl #0x30e3ac
00332b74: str r0, [sl, #0x3c]
00332b78: b #0x3327cc
00332b7c: ldr r0, [r4, r7]
00332b80: bl #0x31f594
00332b84: cmp r0, #0
00332b88: beq #0x3327cc
00332b8c: ldr sl, [r0, #0x128]
00332b90: cmp sl, #0
00332b94: beq #0x3327cc
00332b98: mov r1, #0x43000000
00332b9c: ldr r0, [sl, #0x3c]
00332ba0: add r1, r1, #0x480000
00332ba4: bl #0x30eba4
00332ba8: str r0, [sl, #0x3c]
00332bac: b #0x3327cc
00332bb0: ldr r0, [r4, r7]
00332bb4: bl #0x31f594
00332bb8: cmp r0, #0
00332bbc: beq #0x3327cc
00332bc0: ldr r3, [r0, #0x128]
00332bc4: cmp r3, #0
00332bc8: beq #0x3327cc
00332bcc: ldr r2, [pc, #0x9fc]
00332bd0: ldr r2, [r4, r2]
00332bd4: ldr r1, [r2]
00332bd8: str r1, [r3, #0x38]
00332bdc: ldr r1, [r2, #4]
00332be0: str r1, [r3, #0x3c]
00332be4: ldr r2, [r2, #8]
00332be8: str r2, [r3, #0x40]
00332bec: b #0x3327cc
00332bf0: ldr r3, [r4, r7]
00332bf4: ldr r2, [r6, #0x7c]
00332bf8: ldr r3, [r3, #0x10]
00332bfc: cmp r2, #0
00332c00: ldr sl, [r3, #0x1c]
00332c04: beq #0x333604
00332c08: bl #0x330740
00332c0c: ldr r1, [pc, #0x9c0]
00332c10: add r8, sp, #0x1c4
00332c14: mov sb, r0
00332c18: add r2, sp, #0x6c
00332c1c: add r1, pc, r1
00332c20: mov r0, r8
00332c24: bl #0x3140ec
00332c28: mov r1, r8
00332c2c: mov r2, #0
00332c30: mov r0, sb
00332c34: bl #0x337ddc
00332c38: mov r0, r8
00332c3c: bl #0x3139ac
00332c40: ldr r3, [r6, #0x7c]
00332c44: ldr r8, [r4, r7]
00332c48: cmp r3, #0
00332c4c: moveq r1, r3
00332c50: addne r1, r3, #0x130
00332c54: mov r0, r8
00332c58: bl #0x329310
00332c5c: ldr r3, [sl, #4]
00332c60: ldr r1, [r6, #0x7c]
00332c64: mov r0, r3
00332c68: ldr r3, [r3]
00332c6c: mov lr, pc
00332c70: ldr pc, [r3, #0x60]
00332c74: mov r3, #0
00332c78: str r3, [r6, #0x7c]
00332c7c: mov r0, r8
00332c80: bl #0x31f594
00332c84: ldr r3, [r0, #0x128]
00332c88: mov r0, sl
00332c8c: ldr r1, [r3, #8]
00332c90: bl #0x5890c0
00332c94: bl #0x330740
00332c98: ldr r8, [pc, #0x938]
00332c9c: add sb, sp, #0x194
00332ca0: mov r3, r0
00332ca4: add r8, pc, r8
00332ca8: mov r1, r8
00332cac: add r2, sp, #0x64
00332cb0: mov r0, sb
00332cb4: str r3, [sp]
00332cb8: bl #0x3140ec
00332cbc: bl #0x330740
00332cc0: add sl, sp, #0x1ac
00332cc4: mov fp, r0
00332cc8: add r2, sp, #0x68
00332ccc: mov r1, r8
00332cd0: mov r0, sl
00332cd4: bl #0x3140ec
00332cd8: mov r1, sl
00332cdc: mov r0, fp
00332ce0: bl #0x337a88
00332ce4: ldr r3, [sp]
00332ce8: eor r2, r0, #1
00332cec: mov r1, sb
00332cf0: uxtb r2, r2
00332cf4: mov r0, r3
00332cf8: bl #0x337ddc
00332cfc: mov r0, sl
00332d00: bl #0x3139ac
00332d04: mov r0, sb
00332d08: bl #0x3139ac
00332d0c: ldr fp, [r4, r7]
00332d10: add sb, sp, #0x17c
00332d14: ldr r3, [fp, #0x10]
00332d18: ldr sl, [r3, #0x1c]
00332d1c: bl #0x330740
00332d20: mov r1, r8
00332d24: mov r3, r0
00332d28: add r2, sp, #0x60
00332d2c: mov r0, sb
00332d30: str r3, [sp]
00332d34: bl #0x3140ec
00332d38: ldr r3, [sp]
00332d3c: mov r1, sb
00332d40: mov r0, r3
00332d44: bl #0x337a88
00332d48: mov r8, r0
00332d4c: mov r0, sb
00332d50: bl #0x3139ac
00332d54: cmp r8, #0
00332d58: bne #0x3333ec
00332d5c: ldr r3, [r6, #0x7c]
00332d60: ldr r2, [fp, #0x38]
00332d64: ldr r7, [r4, r7]
00332d68: cmp r3, #0
00332d6c: add r2, r2, #0x60
00332d70: moveq r1, r3
00332d74: addne r1, r3, #0x130
00332d78: str r2, [r6, #0x74]
00332d7c: mov r0, r7
00332d80: bl #0x329310
00332d84: ldr r3, [sl, #4]
00332d88: ldr r1, [r6, #0x7c]
00332d8c: mov r0, r3
00332d90: ldr r3, [r3]
00332d94: mov lr, pc
00332d98: ldr pc, [r3, #0x60]
00332d9c: mov r0, r7
00332da0: bl #0x31f594
00332da4: ldr r3, [r0, #0x128]
00332da8: mov r0, sl
00332dac: ldr r1, [r3, #8]
00332db0: bl #0x5890c0
00332db4: mov r3, #0
00332db8: str r3, [r6, #0x7c]
00332dbc: b #0x332768
00332dc0: ldr r1, [pc, #0x814]
00332dc4: mov r0, r6
00332dc8: add r1, pc, r1
00332dcc: bl #0x3325cc
00332dd0: b #0x332768
00332dd4: mov r0, r6
00332dd8: bl #0x3306a8
00332ddc: b #0x332768
00332de0: bl #0x7fd794
00332de4: ldrb r3, [r0, #5]
00332de8: cmp r3, #0
00332dec: beq #0x332768
00332df0: ldr r3, [r4, r7]
00332df4: ldr r0, [r3, #0x38]
00332df8: bl #0x347fd0
00332dfc: b #0x332768
00332e00: mov r0, r6
00332e04: bl #0x3309d4
00332e08: b #0x332768
00332e0c: bl #0x330740
00332e10: ldr r6, [pc, #0x7c8]
00332e14: add r7, sp, #0xec
00332e18: add r2, sp, #0x48
00332e1c: add r6, pc, r6
00332e20: mov r1, r6
00332e24: mov sb, r0
00332e28: mov r0, r7
00332e2c: bl #0x3140ec
00332e30: bl #0x330740
00332e34: add r8, sp, #0x104
00332e38: add r2, sp, #0x4c
00332e3c: mov sl, r0
00332e40: mov r1, r6
00332e44: mov r0, r8
00332e48: bl #0x3140ec
00332e4c: mov r1, r8
00332e50: mov r0, sl
00332e54: bl #0x337a88
00332e58: eor r2, r0, #1
00332e5c: uxtb r2, r2
00332e60: mov r0, sb
00332e64: mov r1, r7
00332e68: bl #0x337ddc
00332e6c: mov r0, r8
00332e70: bl #0x3139ac
00332e74: mov r0, r7
00332e78: bl #0x3139ac
00332e7c: b #0x332768
00332e80: bl #0x7fd794
00332e84: ldrb r3, [r0, #5]
00332e88: cmp r3, #0
00332e8c: beq #0x332768
00332e90: ldr r3, [r4, r7]
00332e94: ldr r0, [r3, #0x38]
00332e98: bl #0x34072c
00332e9c: b #0x332768
00332ea0: ldr r0, [r4, r7]
00332ea4: bl #0x31f594
00332ea8: subs r3, r0, #0
00332eac: ldrne r3, [r3, #0x128]
00332eb0: ldr r6, [r3, #8]
00332eb4: ldr r3, [r6]
00332eb8: mov r0, r6
00332ebc: ldr r7, [r3, #0x13c]
00332ec0: mov lr, pc
00332ec4: ldr pc, [r3, #0x128]
00332ec8: movw r1, #0xfa35
00332ecc: movt r1, #0x3c8e
00332ed0: bl #0x30e3ac
00332ed4: mov r1, r0
00332ed8: mov r0, r6
00332edc: blx r7
00332ee0: b #0x332768
00332ee4: ldr r0, [r4, r7]
00332ee8: bl #0x31f594
00332eec: cmp r0, #0
00332ef0: beq #0x332768
00332ef4: ldr r0, [r0, #0x128]
00332ef8: cmp r0, #0
00332efc: beq #0x332768
00332f00: bl #0x40f45c
00332f04: b #0x332768
00332f08: ldr r0, [r4, r7]
00332f0c: bl #0x31f594
00332f10: subs r3, r0, #0
00332f14: ldrne r3, [r3, #0x128]
00332f18: ldr r6, [r3, #8]
00332f1c: ldr r3, [r6]
00332f20: mov r0, r6
00332f24: ldr r7, [r3, #0x13c]
00332f28: mov lr, pc
00332f2c: ldr pc, [r3, #0x128]
00332f30: movw r1, #0xfa35
00332f34: movt r1, #0x3c8e
00332f38: bl #0x30eba4
00332f3c: mov r1, r0
00332f40: mov r0, r6
00332f44: blx r7
00332f48: b #0x332768
00332f4c: ldr r0, [r4, r7]
00332f50: bl #0x31f594
00332f54: cmp r0, #0
00332f58: beq #0x332768
00332f5c: ldr r0, [r0, #0x12c]
00332f60: cmp r0, #0
00332f64: beq #0x332768
00332f68: bl #0x40f45c
00332f6c: b #0x332768
00332f70: bl #0x330740
00332f74: ldr r1, [pc, #0x668]
00332f78: add r8, sp, #0x11c
00332f7c: mov sl, r0
00332f80: add r2, sp, #0x50
00332f84: add r1, pc, r1
00332f88: mov r0, r8
00332f8c: bl #0x3140ec
00332f90: mov r0, sl
00332f94: mov r1, r8
00332f98: bl #0x337a88
00332f9c: mov sl, r0
00332fa0: mov r0, r8
00332fa4: bl #0x3139ac
00332fa8: cmp sl, #0
00332fac: bne #0x333584
00332fb0: ldr r0, [r4, r7]
00332fb4: bl #0x31f594
00332fb8: cmp r0, #0
00332fbc: beq #0x332768
00332fc0: ldr r3, [r0, #0x128]
00332fc4: cmp r3, #0
00332fc8: beq #0x332768
00332fcc: ldr r6, [r3, #8]
00332fd0: ldr r3, [r6]
00332fd4: mov r0, r6
00332fd8: ldr r7, [r3, #0x130]
00332fdc: mov lr, pc
00332fe0: ldr pc, [r3, #0x11c]
00332fe4: mov r1, #0x42000000
00332fe8: add r1, r1, #0xc80000
00332fec: bl #0x30e3ac
00332ff0: mov r1, r0
00332ff4: mov r0, r6
00332ff8: blx r7
00332ffc: b #0x332768
00333000: bl #0x330740
00333004: ldr r1, [pc, #0x5dc]
00333008: add r8, sp, #0x134
0033300c: mov sl, r0
00333010: add r2, sp, #0x54
00333014: add r1, pc, r1
00333018: mov r0, r8
0033301c: bl #0x3140ec
00333020: mov r0, sl
00333024: mov r1, r8
00333028: bl #0x337a88
0033302c: mov sl, r0
00333030: mov r0, r8
00333034: bl #0x3139ac
00333038: cmp sl, #0
0033303c: bne #0x333598
00333040: ldr r0, [r4, r7]
00333044: bl #0x31f594
00333048: cmp r0, #0
0033304c: beq #0x332768
00333050: ldr r3, [r0, #0x128]
00333054: cmp r3, #0
00333058: beq #0x332768
0033305c: ldr r6, [r3, #8]
00333060: ldr r3, [r6]
00333064: mov r0, r6
00333068: ldr r7, [r3, #0x130]
0033306c: mov lr, pc
00333070: ldr pc, [r3, #0x11c]
00333074: mov r1, #0x42000000
00333078: add r1, r1, #0xc80000
0033307c: bl #0x30eba4
00333080: mov r1, r0
00333084: mov r0, r6
00333088: blx r7
0033308c: b #0x332768
00333090: ldr r0, [r4, r7]
00333094: bl #0x31f594
00333098: cmp r0, #0
0033309c: beq #0x332768
003330a0: ldr r3, [r0, #0x128]
003330a4: cmp r3, #0
003330a8: beq #0x332768
003330ac: ldr r6, [r3, #8]
003330b0: ldr r3, [r6]
003330b4: mov r0, r6
003330b8: ldr r7, [r3, #0x134]
003330bc: mov lr, pc
003330c0: ldr pc, [r3, #0x120]
003330c4: b #0x332fe4
003330c8: ldr r0, [r4, r7]
003330cc: bl #0x31f594
003330d0: cmp r0, #0
003330d4: beq #0x332768
003330d8: ldr r3, [r0, #0x128]
003330dc: cmp r3, #0
003330e0: beq #0x332768
003330e4: ldr r6, [r3, #8]
003330e8: ldr r3, [r6]
003330ec: mov r0, r6
003330f0: ldr r7, [r3, #0x134]
003330f4: mov lr, pc
003330f8: ldr pc, [r3, #0x120]
003330fc: b #0x333074
00333100: bl #0x330740
00333104: ldr r1, [pc, #0x4e0]
00333108: add r8, sp, #0xd4
0033310c: mov sl, r0
00333110: add r2, sp, #0x44
00333114: add r1, pc, r1
00333118: mov r0, r8
0033311c: bl #0x3140ec
00333120: mov r0, sl
00333124: mov r1, r8
00333128: bl #0x337a88
0033312c: mov sl, r0
00333130: mov r0, r8
00333134: bl #0x3139ac
00333138: cmp sl, #0
0033313c: beq #0x333198
00333140: ldr r3, [r6, #0x74]
00333144: ldr r2, [r4, r7]
00333148: ldr r3, [r3]
0033314c: str r3, [r6, #0x74]
00333150: ldr r2, [r2, #0x38]
00333154: add r1, r2, #0x60
00333158: cmp r3, r1
0033315c: beq #0x3338a0
00333160: mov r1, #0
00333164: mov r0, r6
00333168: bl #0x33026c
0033316c: mov r0, r6
00333170: bl #0x331b68
00333174: ldr r3, [r6, #0x74]
00333178: add r7, sp, #0xbc
0033317c: mov r0, r7
00333180: ldr r3, [r3, #8]
00333184: ldr r3, [r3, #0x2d8]
00333188: ldr r1, [r3, #8]
0033318c: bl #0x51049c
00333190: mov r0, r7
00333194: bl #0x3139ac
00333198: ldr r2, [r6, #0x48]
0033319c: ldr r3, [r6, #0x4c]
003331a0: rsb r3, r2, r3
003331a4: lsrs r3, r3, #2
003331a8: beq #0x332768
003331ac: ldr r3, [r6, #0x54]
003331b0: cmp r3, #0
003331b4: beq #0x3335b8
003331b8: mov r0, r3
003331bc: mov r1, #0
003331c0: ldr r3, [r3]
003331c4: mov lr, pc
003331c8: ldr pc, [r3, #0x48]
003331cc: ldr r1, [r6, #0x4c]
003331d0: add r3, sp, #0x3c
003331d4: ldr r0, [r6, #0x48]
003331d8: add r2, r6, #0x54
003331dc: bl #0x3302bc
003331e0: ldr r3, [r6, #0x48]
003331e4: cmp r0, r3
003331e8: ldreq r3, [r6, #0x4c]
003331ec: ldrne r1, [r0, #-4]
003331f0: ldreq r1, [r3, #-4]
003331f4: str r1, [r6, #0x54]
003331f8: add r7, sp, #0xa4
003331fc: add r6, r6, #0x58
00333200: mov r0, r7
00333204: bl #0x51049c
00333208: cmp r6, r7
0033320c: beq #0x333220
00333210: mov r0, r6
00333214: ldr r1, [sp, #0xb8]
00333218: ldr r2, [sp, #0xb4]
0033321c: bl #0x3109e0
00333220: mov r0, r7
00333224: bl #0x3139ac
00333228: b #0x332768
0033322c: bl #0x330740
00333230: ldr r1, [pc, #0x3b8]
00333234: add r8, sp, #0x8c
00333238: mov sl, r0
0033323c: add r2, sp, #0x40
00333240: add r1, pc, r1
00333244: mov r0, r8
00333248: bl #0x3140ec
0033324c: mov r0, sl
00333250: mov r1, r8
00333254: bl #0x337a88
00333258: mov sl, r0
0033325c: mov r0, r8
00333260: bl #0x3139ac
00333264: cmp sl, #0
00333268: beq #0x3332a0
0033326c: ldr r2, [r4, r7]
00333270: ldr r3, [r6, #0x74]
00333274: mov r0, r6
00333278: ldr r2, [r2, #0x38]
0033327c: ldr r1, [r2, #0x60]
00333280: cmp r1, r3
00333284: ldreq r3, [r2, #0x64]
00333288: ldrne r3, [r3, #4]
0033328c: mov r1, #0
00333290: str r3, [r6, #0x74]
00333294: bl #0x33026c
00333298: mov r0, r6
0033329c: bl #0x331b68
003332a0: ldr r1, [r6, #0x4c]
003332a4: ldr r0, [r6, #0x48]
003332a8: rsb r3, r0, r1
003332ac: lsrs r3, r3, #2
003332b0: beq #0x332768
003332b4: ldr r3, [r6, #0x54]
003332b8: cmp r3, #0
003332bc: beq #0x3335ac
003332c0: add r2, r6, #0x54
003332c4: add r3, sp, #0x38
003332c8: bl #0x3302bc
003332cc: ldr r3, [r6, #0x54]
003332d0: mov r1, #0
003332d4: mov r7, r0
003332d8: mov r0, r3
003332dc: ldr r3, [r3]
003332e0: mov lr, pc
003332e4: ldr pc, [r3, #0x48]
003332e8: ldr r3, [r6, #0x4c]
003332ec: add r2, r7, #4
003332f0: cmp r2, r3
003332f4: ldreq r3, [r6, #0x48]
003332f8: ldrne r1, [r7, #4]
003332fc: ldreq r1, [r3]
00333300: str r1, [r6, #0x54]
00333304: add r7, sp, #0x74
00333308: add r6, r6, #0x58
0033330c: mov r0, r7
00333310: bl #0x51049c
00333314: cmp r6, r7
00333318: beq #0x333220
0033331c: mov r0, r6
00333320: ldr r1, [sp, #0x88]
00333324: ldr r2, [sp, #0x84]
00333328: bl #0x3109e0
0033332c: b #0x333220
00333330: bl #0x330740
00333334: ldr r7, [pc, #0x2b8]
00333338: add r8, sp, #0x14c
0033333c: add r2, sp, #0x58
00333340: add r7, pc, r7
00333344: mov r1, r7
00333348: mov fp, r0
0033334c: mov r0, r8
00333350: bl #0x3140ec
00333354: bl #0x330740
00333358: add sl, sp, #0x164
0033335c: mov r1, r7
00333360: add r2, sp, #0x5c
00333364: mov sb, r0
00333368: mov r0, sl
0033336c: bl #0x3140ec
00333370: mov r1, sl
00333374: mov r0, sb
00333378: bl #0x337a88
0033337c: eor r2, r0, #1
00333380: mov r1, r8
00333384: uxtb r2, r2
00333388: mov r0, fp
0033338c: bl #0x337ddc
00333390: mov r0, sl
00333394: bl #0x3139ac
00333398: mov r0, r8
0033339c: bl #0x3139ac
003333a0: ldr r7, [r6, #0x48]
003333a4: ldr r8, [r6, #0x4c]
003333a8: cmp r7, r8
003333ac: beq #0x3333e0
003333b0: ldr r3, [r7], #4
003333b4: mov r1, #1
003333b8: mov r0, r3
003333bc: ldr r3, [r3]
003333c0: mov lr, pc
003333c4: ldr pc, [r3, #0x48]
003333c8: cmp r7, r8
003333cc: bne #0x3333b0
003333d0: ldr r3, [r6, #0x48]
003333d4: ldr r2, [r6, #0x4c]
003333d8: cmp r3, r2
003333dc: strne r3, [r6, #0x4c]
003333e0: mov r3, #0
003333e4: str r3, [r6, #0x54]
003333e8: b #0x332768
003333ec: ldr r3, [fp, #0x38]
003333f0: mov r0, r6
003333f4: mov r1, #0
003333f8: ldr r3, [r3, #0x60]
003333fc: str r3, [r6, #0x74]
00333400: bl #0x33026c
00333404: ldr r1, [pc, #0x1ec]
00333408: ldr r2, [sl, #4]
0033340c: ldr r3, [sl]
00333410: add r1, pc, r1
00333414: mov r0, sl
00333418: mov lr, pc
0033341c: ldr pc, [r3, #0x6c]
00333420: str r0, [r6, #0x7c]
00333424: bl #0x597094
00333428: ldr r3, [r0]
0033342c: mov r1, #0x42000000
00333430: add r1, r1, #0xc80000
00333434: ldr r8, [r3, #8]
00333438: mov r0, r8
0033343c: ldr r3, [r8]
00333440: mov lr, pc
00333444: ldr pc, [r3, #0x60]
00333448: movw r1, #0x4000
0033344c: mov r0, r8
00333450: ldr r3, [r8]
00333454: movt r1, #0x459c
00333458: mov lr, pc
0033345c: ldr pc, [r3, #0x58]
00333460: ldr r3, [r6, #0x7c]
00333464: movw r1, #0x5000
00333468: movt r1, #0x4743
0033346c: mov r0, r3
00333470: ldr r3, [r3]
00333474: mov lr, pc
00333478: ldr pc, [r3, #0x134]
0033347c: ldr r3, [r6, #0x7c]
00333480: ldr r7, [r4, r7]
00333484: cmp r3, #0
00333488: moveq r1, r3
0033348c: addne r1, r3, #0x130
00333490: mov r0, r7
00333494: bl #0x32fa7c
00333498: mov r0, r7
0033349c: bl #0x31f594
003334a0: ldr r3, [r0, #0x128]
003334a4: add r0, sp, #0x14
003334a8: ldr r7, [r3, #8]
003334ac: mov r1, r7
003334b0: bl #0x597180
003334b4: ldr r3, [r7]
003334b8: mov r0, r7
003334bc: mov lr, pc
003334c0: ldr pc, [r3, #0x108]
003334c4: mov r0, r6
003334c8: bl #0x331b68
003334cc: ldr r3, [r6, #0x7c]
003334d0: mov r1, #0
003334d4: mov r0, r3
003334d8: ldr r3, [r3]
003334dc: mov lr, pc
003334e0: ldr pc, [r3, #0xb8]
003334e4: ldr sb, [r6, #0x7c]
003334e8: ldr r3, [r7]
003334ec: mov r0, r7
003334f0: ldr r2, [sb]
003334f4: ldr r8, [r2, #0x114]
003334f8: mov lr, pc
003334fc: ldr pc, [r3, #0x118]
00333500: mov r1, r0
00333504: mov r0, sb
00333508: blx r8
0033350c: ldr sb, [r6, #0x7c]
00333510: ldr r3, [r7]
00333514: mov r0, r7
00333518: ldr r2, [sb]
0033351c: ldr r8, [r2, #0x13c]
00333520: mov lr, pc
00333524: ldr pc, [r3, #0x128]
00333528: mov r1, r0
0033352c: mov r0, sb
00333530: blx r8
00333534: ldr r8, [r6, #0x7c]
00333538: ldr r3, [r7]
0033353c: mov r0, r7
00333540: ldr r2, [r8]
00333544: ldr r7, [r2, #0x138]
00333548: mov lr, pc
0033354c: ldr pc, [r3, #0x124]
00333550: mov r1, r0
00333554: mov r0, r8
00333558: blx r7
0033355c: ldr r3, [r6, #0x7c]
00333560: mov r1, #1
00333564: mov r0, r3
00333568: ldr r3, [r3]
0033356c: mov lr, pc
00333570: ldr pc, [r3, #0x148]
00333574: mov r0, sl
00333578: ldr r1, [r6, #0x7c]
0033357c: bl #0x5890c0
00333580: b #0x332768
00333584: ldr r1, [r6, #0x70]
00333588: mov r0, r6
0033358c: sub r1, r1, #1
00333590: bl #0x33026c
00333594: b #0x332768
00333598: ldr r1, [r6, #0x70]
0033359c: mov r0, r6
003335a0: add r1, r1, #1
003335a4: bl #0x33026c
003335a8: b #0x332768
003335ac: ldr r1, [r1, #-4]
003335b0: str r1, [r6, #0x54]
003335b4: b #0x333304
003335b8: ldr r1, [r2]
003335bc: str r1, [r6, #0x54]
003335c0: b #0x3331f8
003335c4: rsbeq r2, r6, r4, ror #6
003335c8: andeq r4, r0, ip, lsr #1
003335cc: strdeq r3, r4, [r0], -r4
003335d0: andeq r3, r0, ip, lsr #30
003335d4: subseq ip, r8, ip, lsl #26
003335d8: subseq ip, r8, r4, asr #5

_ZNK16GameEventManager28DBG_TraceDetailedInformationEv 0x479814 52
00479814: ldr r1, [pc, #0x24]
00479818: ldr r3, [pc, #0x24]
0047981c: mov ip, #0
00479820: add r1, pc, r1
00479824: ldr r3, [r1, r3]
00479828: sub sp, sp, #8
0047982c: mov r2, ip
00479830: mov r1, r3
00479834: stm sp, {r3, ip}
00479838: add sp, sp, #8
0047983c: b #0x4797b0
00479840: subseq fp, r1, r0, ror r2
00479844: strheq r4, [r0], -r0

_ZNSaINSt4priv10_List_nodeIPN6glitch14IEventReceiverEEEE8allocateEjPKv.clone.20 0x32fa5c 32
0032fa5c: str lr, [sp, #-4]!
0032fa60: sub sp, sp, #0xc
0032fa64: add r0, sp, #8
0032fa68: mov r3, #0xc
0032fa6c: str r3, [r0, #-4]!
0032fa70: bl #0x708ec0
0032fa74: add sp, sp, #0xc
0032fa78: ldm sp!, {pc}

_ZNSt4listIPN6glitch14IEventReceiverESaIS2_EE6resizeEjRKS2_.clone.30 0x32fc60 64
0032fc60: push {r4, r5, r6, lr}
0032fc64: mov r5, r0
0032fc68: ldr r0, [r0]
0032fc6c: cmp r0, r5
0032fc70: beq #0x32fc9c
0032fc74: b #0x32fc7c
0032fc78: mov r0, r4
0032fc7c: ldr r4, [r0]
0032fc80: ldr r3, [r0, #4]
0032fc84: mov r1, #0xc
0032fc88: str r4, [r3]
0032fc8c: str r3, [r4, #4]
0032fc90: bl #0x708f00
0032fc94: cmp r5, r4
0032fc98: bne #0x32fc78
0032fc9c: pop {r4, r5, r6, pc}

_GLOBAL__I_.._.._sources_Core_EventManager_EventManager.cpp 0x3381c8 220
003381c8: push {r4, r5, r6, lr}
003381cc: ldr r4, [pc, #0xac]
003381d0: ldr r2, [pc, #0xac]
003381d4: ldr r3, [pc, #0xac]
003381d8: add r4, pc, r4
003381dc: ldr r1, [r4, r2]
003381e0: add r3, pc, r3
003381e4: mov r2, #0x3f000000
003381e8: ldr r0, [r1]
003381ec: str r2, [r3, #8]
003381f0: str r2, [r3]
003381f4: tst r0, #1
003381f8: str r2, [r3, #4]
003381fc: beq #0x33824c
00338200: ldr r3, [pc, #0x84]
00338204: ldr r3, [r4, r3]
00338208: ldr r2, [r3]
0033820c: tst r2, #1
00338210: beq #0x338218
00338214: pop {r4, r5, r6, pc}
00338218: mov r2, #1
0033821c: str r2, [r3]
00338220: ldr r3, [pc, #0x68]
00338224: ldr r5, [r4, r3]
00338228: mov r0, r5
0033822c: bl #0x32d79c
00338230: ldr r3, [pc, #0x5c]
00338234: mov r0, r5
00338238: ldr r1, [r4, r3]
0033823c: ldr r3, [pc, #0x54]
00338240: ldr r2, [r4, r3]
00338244: pop {r4, r5, r6, lr}
00338248: b #0x30e304
0033824c: mov r3, #1
00338250: str r3, [r1]
00338254: ldr r3, [pc, #0x40]
00338258: ldr r5, [r4, r3]
0033825c: mov r0, r5
00338260: bl #0x3790a8
00338264: ldr r3, [pc, #0x34]
00338268: mov r0, r5
0033826c: ldr r1, [r4, r3]
00338270: ldr r3, [pc, #0x20]
00338274: ldr r2, [r4, r3]
00338278: bl #0x30e304
0033827c: b #0x338200
00338280: strhteq ip, [r5], #-0x88
00338284: strdeq r0, r1, [r0], -r4
00338288: rsbeq sb, r6, r4, ror #22
0033828c: andeq r0, r0, ip, lsr #31
00338290: strdeq r3, r4, [r0], -r4
00338294: andeq r0, r0, r0, asr #17
00338298: muleq r0, r0, r8
0033829c: andeq r2, r0, r4, lsl r7
003382a0: muleq r0, ip, r5

_ZNSaINSt4priv10_List_nodeIN12EventManager12ReceiverInfoEEEE8allocateEjPKv.clone.1 0x33860c 32
0033860c: str lr, [sp, #-4]!
00338610: sub sp, sp, #0xc
00338614: add r0, sp, #8
00338618: mov r3, #0x14
0033861c: str r3, [r0, #-4]!
00338620: bl #0x708ec0
00338624: add sp, sp, #0xc
00338628: ldm sp!, {pc}

_GLOBAL__I_.._.._sources_Game_Progression_GameEventManager.cpp 0x479c24 136
00479c24: push {r4, r5, r6, lr}
00479c28: ldr r4, [pc, #0x64]
00479c2c: ldr r2, [pc, #0x64]
00479c30: ldr r3, [pc, #0x64]
00479c34: add r4, pc, r4
00479c38: ldr r1, [r4, r2]
00479c3c: add r3, pc, r3
00479c40: mov r2, #0x3f000000
00479c44: ldr r0, [r1]
00479c48: str r2, [r3, #8]
00479c4c: str r2, [r3]
00479c50: tst r0, #1
00479c54: str r2, [r3, #4]
00479c58: beq #0x479c60
00479c5c: pop {r4, r5, r6, pc}
00479c60: mov r3, #1
00479c64: str r3, [r1]
00479c68: ldr r3, [pc, #0x30]
00479c6c: ldr r5, [r4, r3]
00479c70: mov r0, r5
00479c74: bl #0x32d79c
00479c78: ldr r3, [pc, #0x24]
00479c7c: mov r0, r5
00479c80: ldr r1, [r4, r3]
00479c84: ldr r3, [pc, #0x1c]
00479c88: ldr r2, [r4, r3]
00479c8c: pop {r4, r5, r6, lr}
00479c90: b #0x30e304
00479c94: subseq sl, r1, ip, asr lr
00479c98: andeq r0, r0, ip, lsr #31
00479c9c: subseq ip, r2, r0, ror #10
00479ca0: strdeq r3, r4, [r0], -r4
00479ca4: andeq r0, r0, r0, asr #17
00479ca8: muleq r0, r0, r8