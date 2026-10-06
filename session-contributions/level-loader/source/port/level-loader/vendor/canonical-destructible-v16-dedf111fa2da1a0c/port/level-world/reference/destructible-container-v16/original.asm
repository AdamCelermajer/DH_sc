
_ZN9Container8InteractEP10GameObject 0x3a0b38 300
003a0b38: push {r4, r5, r6, r7, lr}
003a0b3c: ldr r3, [r0, #0x394]
003a0b40: ldr r5, [pc, #0x110]
003a0b44: sub sp, sp, #0x24
003a0b48: sub r3, r3, #3
003a0b4c: cmp r3, #1
003a0b50: mov r4, r0
003a0b54: add r5, pc, r5
003a0b58: bls #0x3a0c34
003a0b5c: str r1, [r0, #0x398]
003a0b60: mov r1, #4
003a0b64: bl #0x39f3cc
003a0b68: ldr r3, [r4]
003a0b6c: mov r0, r4
003a0b70: mov lr, pc
003a0b74: ldr pc, [r3, #0xdc]
003a0b78: subs r1, r0, #0
003a0b7c: beq #0x3a0c3c
003a0b80: ldr r3, [r4, #0x2d8]
003a0b84: cmp r3, #0
003a0b88: beq #0x3a0c4c
003a0b8c: mov r0, r4
003a0b90: mov r1, #3
003a0b94: bl #0x39f3cc
003a0b98: ldr r2, [r4, #0x2d8]
003a0b9c: ldr r1, [pc, #0xb8]
003a0ba0: mov r3, #0
003a0ba4: ldr ip, [r2, #0x38]
003a0ba8: add r1, pc, r1
003a0bac: mov r2, r3
003a0bb0: mov r0, ip
003a0bb4: ldr ip, [ip]
003a0bb8: str r3, [sp]
003a0bbc: mov lr, pc
003a0bc0: ldr pc, [ip, #0x20]
003a0bc4: ldr r2, [pc, #0x94]
003a0bc8: ldr r3, [r4]
003a0bcc: mov r0, r4
003a0bd0: ldr r2, [r5, r2]
003a0bd4: ldr r7, [r2]
003a0bd8: mov lr, pc
003a0bdc: ldr pc, [r3, #0xd8]
003a0be0: ldr lr, [r4, #0x168]
003a0be4: ldr r5, [r4, #0x164]
003a0be8: ldr r6, [r4, #0x160]
003a0bec: mov ip, #0xbf000000
003a0bf0: add ip, ip, #0x800000
003a0bf4: mov r1, r0
003a0bf8: mov r3, #0
003a0bfc: str lr, [sp, #0x1c]
003a0c00: mov r0, r7
003a0c04: mov lr, #1
003a0c08: add r2, sp, #0x14
003a0c0c: str r6, [sp, #0x14]
003a0c10: str r5, [sp, #0x18]
003a0c14: str lr, [sp]
003a0c18: str ip, [sp, #8]
003a0c1c: str ip, [sp, #4]
003a0c20: bl #0x36b5d8
003a0c24: mov r0, r4
003a0c28: ldr r3, [r4]
003a0c2c: mov lr, pc
003a0c30: ldr pc, [r3, #0x2c]
003a0c34: add sp, sp, #0x24
003a0c38: pop {r4, r5, r6, r7, pc}
003a0c3c: mov r0, r4
003a0c40: mov r2, r1
003a0c44: bl #0x394bf8
003a0c48: b #0x3a0b80
003a0c4c: mov r0, r4
003a0c50: bl #0x3a0a98
003a0c54: b #0x3a0bc4
003a0c58: subseq r3, pc, ip, lsr pc
003a0c5c: subseq r1, r2, r0, lsr pc
003a0c60: andeq r0, r0, r4, lsr #27

_ZThn36_N21DestructibleContainerD1Ev 0x3a1410 8
003a1410: sub r0, r0, #0x24
003a1414: b #0x3a1418

_ZN9Container18NetStructContainerD0Ev 0x3a03a4 132
003a03a4: push {r4, r5, r6, lr}
003a03a8: ldr r3, [pc, #0x6c]
003a03ac: ldr r2, [pc, #0x6c]
003a03b0: ldr r1, [pc, #0x6c]
003a03b4: add r3, pc, r3
003a03b8: mov r4, r0
003a03bc: ldr r1, [r3, r1]
003a03c0: ldr r0, [r0, #0x11c]
003a03c4: ldr r2, [r3, r2]
003a03c8: add r1, r1, #8
003a03cc: cmp r0, #0
003a03d0: add r2, r2, #8
003a03d4: str r2, [r4, #0x130]
003a03d8: str r1, [r4]
003a03dc: str r2, [r4, #0x180]
003a03e0: str r2, [r4, #0x158]
003a03e4: beq #0x3a040c
003a03e8: add r5, r4, #0x10c
003a03ec: mov r0, r5
003a03f0: ldr r1, [r4, #0x110]
003a03f4: bl #0x370fd0
003a03f8: mov r3, #0
003a03fc: str r5, [r4, #0x118]
003a0400: str r3, [r4, #0x11c]
003a0404: str r5, [r4, #0x114]
003a0408: str r3, [r4, #0x110]
003a040c: mov r0, r4
003a0410: bl #0x310440
003a0414: mov r0, r4
003a0418: pop {r4, r5, r6, pc}
003a041c: ldrsbeq r4, [pc], #-0x6c
003a0420: andeq r1, r0, r8, lsr #1
003a0424: andeq r4, r0, r4, asr #7

_ZN9ContainerD2Ev 0x3a0598 324
003a0598: push {r4, r5, r6, r7, r8, sb, sl, lr}
003a059c: ldr r5, [pc, #0x128]
003a05a0: ldr r3, [pc, #0x128]
003a05a4: ldr r7, [pc, #0x128]
003a05a8: ldr r8, [pc, #0x128]
003a05ac: add r5, pc, r5
003a05b0: ldr r3, [r5, r3]
003a05b4: ldr r1, [r5, r7]
003a05b8: ldr r2, [r5, r8]
003a05bc: mov r4, r0
003a05c0: add r1, r1, #8
003a05c4: add r0, r3, #0x100
003a05c8: add ip, r3, #8
003a05cc: add r2, r2, #8
003a05d0: add r3, r3, #0xf4
003a05d4: add sl, r4, #0x540
003a05d8: str ip, [r4]
003a05dc: str r3, [r4, #4]
003a05e0: str r0, [r4, #0x24]
003a05e4: str r1, [r4, #0x678]
003a05e8: str r2, [r4, #0x548]
003a05ec: str r1, [r4, #0x6c8]
003a05f0: str r1, [r4, #0x6a0]
003a05f4: add r6, sl, #8
003a05f8: ldr r3, [r6, #0x11c]
003a05fc: cmp r3, #0
003a0600: beq #0x3a0628
003a0604: add sl, sl, #0x114
003a0608: mov r0, sl
003a060c: ldr r1, [r6, #0x110]
003a0610: bl #0x370fd0
003a0614: mov r3, #0
003a0618: str r3, [r6, #0x11c]
003a061c: str sl, [r6, #0x118]
003a0620: str sl, [r6, #0x114]
003a0624: str r3, [r6, #0x110]
003a0628: ldr r2, [r5, r8]
003a062c: ldr r3, [r5, r7]
003a0630: ldr r1, [r4, #0x4bc]
003a0634: add r2, r2, #8
003a0638: add r3, r3, #8
003a063c: cmp r1, #0
003a0640: str r3, [r4, #0x4d0]
003a0644: str r2, [r4, #0x3a0]
003a0648: str r3, [r4, #0x520]
003a064c: str r3, [r4, #0x4f8]
003a0650: add r5, r4, #0x3a0
003a0654: beq #0x3a067c
003a0658: add r6, r5, #0x10c
003a065c: mov r0, r6
003a0660: ldr r1, [r5, #0x110]
003a0664: bl #0x370fd0
003a0668: mov r3, #0
003a066c: str r6, [r4, #0x4b4]
003a0670: str r3, [r5, #0x110]
003a0674: str r6, [r4, #0x4b8]
003a0678: str r3, [r4, #0x4bc]
003a067c: add r3, r4, #0x378
003a0680: ldr r0, [r3, #0x14]
003a0684: cmp r0, r3
003a0688: beq #0x3a06a8
003a068c: cmp r0, #0
003a0690: beq #0x3a06a8
003a0694: ldr r1, [r4, #0x378]
003a0698: rsb r1, r0, r1
003a069c: cmp r1, #0x80
003a06a0: bhi #0x3a06b8
003a06a4: bl #0x708f00
003a06a8: mov r0, r4
003a06ac: bl #0x38d378
003a06b0: mov r0, r4
003a06b4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003a06b8: bl #0x310440
003a06bc: mov r0, r4
003a06c0: bl #0x38d378
003a06c4: mov r0, r4
003a06c8: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003a06cc: subseq r4, pc, r4, ror #9
003a06d0: ldrdeq r2, r3, [r0], -r4
003a06d4: andeq r1, r0, r8, lsr #1
003a06d8: andeq r4, r0, r4, asr #7

_ZNK9Container10IsAnimatedEv 0x39f37c 8
0039f37c: mov r0, #1
0039f380: bx lr

_ZN7Structs21DestructibleContainerD1Ev 0x4da15c 64
004da15c: push {r4, lr}
004da160: ldr r3, [pc, #0x2c]
004da164: ldr r2, [pc, #0x2c]
004da168: mov r4, r0
004da16c: add r3, pc, r3
004da170: ldr r0, [r0, #0x30]
004da174: ldr r2, [r3, r2]
004da178: cmp r0, #0
004da17c: add r2, r2, #8
004da180: str r2, [r4]
004da184: beq #0x4da18c
004da188: bl #0x310440
004da18c: mov r0, r4
004da190: pop {r4, pc}
004da194: subeq sl, fp, r4, lsr #18
004da198: andeq r3, r0, r4, asr #28

_ZNK9Container10IsObstacleEv 0x39f3a8 8
0039f3a8: mov r0, #1
0039f3ac: bx lr

_ZN9Container8InitPostEv 0x39f910 576
0039f910: push {r4, r5, r6, r7, r8, lr}
0039f914: sub sp, sp, #8
0039f918: mov r4, r0
0039f91c: bl #0x38bd64
0039f920: ldr r3, [r4, #0x274]
0039f924: ldr r5, [pc, #0x200]
0039f928: cmp r0, r3
0039f92c: add r5, pc, r5
0039f930: blt #0x39f93c
0039f934: add sp, sp, #8
0039f938: pop {r4, r5, r6, r7, r8, pc}
0039f93c: ldr r3, [r4]
0039f940: mov r0, r4
0039f944: mov lr, pc
0039f948: ldr pc, [r3, #0xd0]
0039f94c: ldr r3, [r4]
0039f950: str r0, [r4, #0x374]
0039f954: mov r0, r4
0039f958: mov lr, pc
0039f95c: ldr pc, [r3, #0xcc]
0039f960: cmn r0, #1
0039f964: beq #0x39f9a4
0039f968: ldr r3, [r4, #0x374]
0039f96c: cmn r3, #1
0039f970: beq #0x39f9a4
0039f974: ldr r3, [pc, #0x1b4]
0039f978: mov r2, #0xc
0039f97c: ldr r3, [r5, r3]
0039f980: ldr r3, [r3]
0039f984: mla r0, r2, r0, r3
0039f988: ldr r6, [r0, #8]
0039f98c: mov r0, r6
0039f990: bl #0x30de54
0039f994: mov r1, r6
0039f998: add r2, r6, r0
0039f99c: add r0, r4, #0x290
0039f9a0: bl #0x3109e0
0039f9a4: mov r0, r4
0039f9a8: bl #0x38be5c
0039f9ac: mov r0, r4
0039f9b0: bl #0x38ab60
0039f9b4: subs r1, r0, #0
0039f9b8: beq #0x39faf8
0039f9bc: ldr r8, [r4, #0x2d8]
0039f9c0: cmp r8, #0
0039f9c4: beq #0x39fa38
0039f9c8: ldr r3, [pc, #0x164]
0039f9cc: ldr r6, [r8, #0x38]
0039f9d0: mov r2, r4
0039f9d4: ldr r1, [r5, r3]
0039f9d8: ldr r3, [pc, #0x158]
0039f9dc: ldr ip, [r6]
0039f9e0: mov r0, r6
0039f9e4: ldr r3, [r5, r3]
0039f9e8: str r4, [sp]
0039f9ec: mov lr, pc
0039f9f0: ldr pc, [ip, #0x2c]
0039f9f4: ldr r1, [pc, #0x140]
0039f9f8: mov r7, #0
0039f9fc: ldr ip, [r6]
0039fa00: add r1, pc, r1
0039fa04: str r7, [sp]
0039fa08: mov r0, r6
0039fa0c: mov r2, r7
0039fa10: mov r3, r7
0039fa14: mov lr, pc
0039fa18: ldr pc, [ip, #0x20]
0039fa1c: cmp r0, #0
0039fa20: beq #0x39fa94
0039fa24: mov r1, r7
0039fa28: mov r0, r4
0039fa2c: bl #0x39f3cc
0039fa30: mov r0, r8
0039fa34: bl #0x470a54
0039fa38: ldr r3, [pc, #0x100]
0039fa3c: ldr r3, [r5, r3]
0039fa40: ldr r5, [r3]
0039fa44: cmp r5, #0
0039fa48: beq #0x39fa68
0039fa4c: ldr r3, [r4]
0039fa50: mov r0, r4
0039fa54: mov lr, pc
0039fa58: ldr pc, [r3, #0xd8]
0039fa5c: mov r1, r0
0039fa60: mov r0, r5
0039fa64: bl #0x3699fc
0039fa68: ldr r3, [r4]
0039fa6c: mov r0, r4
0039fa70: mov lr, pc
0039fa74: ldr pc, [r3, #0xc8]
0039fa78: ldr r2, [pc, #0xc4]
0039fa7c: mov r1, r0
0039fa80: mov r0, r4
0039fa84: add r2, pc, r2
0039fa88: add sp, sp, #8
0039fa8c: pop {r4, r5, r6, r7, r8, lr}
0039fa90: b #0x38ef60
0039fa94: ldr r1, [pc, #0xac]
0039fa98: mov r2, r0
0039fa9c: ldr ip, [r6]
0039faa0: mov r3, r2
0039faa4: str r0, [sp]
0039faa8: add r1, pc, r1
0039faac: mov r0, r6
0039fab0: mov lr, pc
0039fab4: ldr pc, [ip, #0x20]
0039fab8: subs r3, r0, #0
0039fabc: bne #0x39fb1c
0039fac0: ldr r1, [pc, #0x84]
0039fac4: ldr ip, [r6]
0039fac8: mov r2, r3
0039facc: mov r0, r6
0039fad0: add r1, pc, r1
0039fad4: str r3, [sp]
0039fad8: mov lr, pc
0039fadc: ldr pc, [ip, #0x20]
0039fae0: cmp r0, #0
0039fae4: beq #0x39fa30
0039fae8: mov r0, r4
0039faec: mov r1, #2
0039faf0: bl #0x39f3cc
0039faf4: b #0x39fa30
0039faf8: mov r0, r4
0039fafc: ldr r3, [r4]
0039fb00: mov lr, pc
0039fb04: ldr pc, [r3, #0x40]
0039fb08: mov r0, r4
0039fb0c: mov r1, #4
0039fb10: add sp, sp, #8
0039fb14: pop {r4, r5, r6, r7, r8, lr}
0039fb18: b #0x39f3cc
0039fb1c: mov r0, r4
0039fb20: mov r1, #1
0039fb24: bl #0x39f3cc
0039fb28: b #0x39fa30
0039fb2c: subseq r5, pc, r4, ror #2
0039fb30: andeq r1, r0, r8, lsr #25
0039fb34: andeq r4, r0, r0, lsl #24
0039fb38: andeq r1, r0, ip, ror #30
0039fb3c: subseq r3, r2, r0, asr r5
0039fb40: andeq r0, r0, r4, lsr #27
0039fb44: ldrsheq r3, [r2], #-0xc
0039fb48: subseq r3, r2, r0, lsl #8
0039fb4c: subseq r2, r2, r0, ror #15

_ZN9ContainerC1EN10ObjectBase6GO_IDSE 0x3a06dc 172
003a06dc: push {r4, r5, r6, r7, r8, lr}
003a06e0: ldr r5, [pc, #0x98]
003a06e4: mov r4, r0
003a06e8: bl #0x38c398
003a06ec: ldr r3, [pc, #0x90]
003a06f0: add r5, pc, r5
003a06f4: add r2, r4, #0x378
003a06f8: ldr r3, [r5, r3]
003a06fc: mov r0, r2
003a0700: str r2, [r4, #0x388]
003a0704: add ip, r3, #8
003a0708: add r1, r3, #0x100
003a070c: add r3, r3, #0xf4
003a0710: str ip, [r4]
003a0714: str r2, [r4, #0x38c]
003a0718: str r3, [r4, #4]
003a071c: str r1, [r4, #0x24]
003a0720: mov r1, #0x10
003a0724: bl #0x31167c
003a0728: ldr r3, [r4, #0x388]
003a072c: mov r6, #0
003a0730: mov r7, #2
003a0734: add r8, r4, #0x3a0
003a0738: add r5, r4, #0x540
003a073c: strb r6, [r3]
003a0740: add r5, r5, #8
003a0744: strb r6, [r4, #0x390]
003a0748: str r7, [r4, #0x394]
003a074c: str r6, [r4, #0x398]
003a0750: mov r0, r8
003a0754: bl #0x39ff58
003a0758: mov r0, r5
003a075c: bl #0x39ff58
003a0760: mov r3, #1
003a0764: strb r3, [r4, #0x28]
003a0768: strb r6, [r4, #0x84]
003a076c: str r8, [r4, #0x100]
003a0770: str r5, [r4, #0x104]
003a0774: strb r7, [r4, #0xf8]
003a0778: mov r0, r4
003a077c: pop {r4, r5, r6, r7, r8, pc}
003a0780: subseq r4, pc, r0, lsr #7
003a0784: ldrdeq r2, r3, [r0], -r4

_ZThn36_N9Container11DeserializeEP11IStreamBase 0x39f6e8 8
0039f6e8: sub r0, r0, #0x24
0039f6ec: b #0x39f6f0

_ZNK9Container21GetLootFixedNumPowersEv 0x39f35c 8
0039f35c: mvn r0, #0
0039f360: bx lr

_ZNK9Container8GetSoundEv 0x39f34c 8
0039f34c: mvn r0, #0
0039f350: bx lr

_ZThn36_N9ContainerD1Ev 0x3a0428 8
003a0428: sub r0, r0, #0x24
003a042c: b #0x3a0430

_ZNK9Container19GetObstacleStrengthEv 0x39f3bc 12
0039f3bc: mov r0, #0x41000000
0039f3c0: add r0, r0, #0x200000
0039f3c4: bx lr

_ZNK21DestructibleContainer9GetDataIdEv 0x3a11c0 28
003a11c0: ldr r3, [r0, #0x388]
003a11c4: ldr r0, [r0, #0x38c]
003a11c8: cmp r3, r0
003a11cc: beq #0x3a11d4
003a11d0: b #0x3a1084
003a11d4: mvn r0, #0
003a11d8: bx lr

_ZNK21DestructibleContainer9GetVisualEv 0x3a0d28 56
003a0d28: ldr r0, [r0, #0x374]
003a0d2c: ldr r3, [pc, #0x24]
003a0d30: cmn r0, #1
003a0d34: add r3, pc, r3
003a0d38: bxeq lr
003a0d3c: ldr r2, [pc, #0x18]
003a0d40: ldr r3, [r3, r2]
003a0d44: mov r2, #0x44
003a0d48: ldr r3, [r3]
003a0d4c: mla r0, r2, r0, r3
003a0d50: ldr r0, [r0, #0x3c]
003a0d54: bx lr
003a0d58: subseq r3, pc, ip, asr sp
003a0d5c: andeq r3, r0, r4, asr r8

_ZN7Structs19GetMemberIDByStringINS_21DestructibleContainerEEEiPKc 0x4ae594 88
004ae594: ldr r3, [pc, #0x48]
004ae598: ldr r2, [pc, #0x48]
004ae59c: push {r4, r5, r6, lr}
004ae5a0: add r3, pc, r3
004ae5a4: mov r6, r0
004ae5a8: ldr r5, [r3, r2]
004ae5ac: mov r4, #0
004ae5b0: ldr r1, [r5, #0x14]
004ae5b4: mov r0, r6
004ae5b8: bl #0x30e31c
004ae5bc: cmp r0, #0
004ae5c0: beq #0x4ae5dc
004ae5c4: add r4, r4, #1
004ae5c8: cmp r4, #0xf
004ae5cc: add r5, r5, #0x18
004ae5d0: bne #0x4ae5b0
004ae5d4: mvn r0, #0
004ae5d8: pop {r4, r5, r6, pc}
004ae5dc: mov r0, r4
004ae5e0: pop {r4, r5, r6, pc}
004ae5e4: strdeq r6, r7, [lr], #-0x40
004ae5e8: andeq r3, r0, r4, lsl pc

_Z14GetNewInstanceI21DestructibleContainerEP10ObjectBasev 0x340d5c 36
00340d5c: push {r4, lr}
00340d60: mov r1, #0
00340d64: movw r0, #0x6f8
00340d68: bl #0x310570
00340d6c: mov r1, #1
00340d70: mov r4, r0
00340d74: bl #0x3a14bc
00340d78: mov r0, r4
00340d7c: pop {r4, pc}

_ZThn4_N9Container17DeclarePropertiesEv 0x3a0834 8
003a0834: sub r0, r0, #4
003a0838: b #0x3a083c

_ZNK9Container13IsInteractiveEP10GameObject 0x39f384 36
0039f384: ldrb r3, [r0, #0x81]
0039f388: cmp r3, #0
0039f38c: movne r0, #0
0039f390: bxne lr
0039f394: ldr r0, [r0, #0x394]
0039f398: cmp r0, #2
0039f39c: movne r0, #0
0039f3a0: moveq r0, #1
0039f3a4: bx lr

_ZNK21DestructibleContainer9GetScriptEv 0x3a0cec 60
003a0cec: ldr r2, [r0, #0x374]
003a0cf0: ldr r3, [pc, #0x28]
003a0cf4: cmn r2, #1
003a0cf8: add r3, pc, r3
003a0cfc: moveq r0, #0
003a0d00: bxeq lr
003a0d04: ldr r1, [pc, #0x18]
003a0d08: ldr r3, [r3, r1]
003a0d0c: mov r1, #0x44
003a0d10: ldr r3, [r3]
003a0d14: mla r2, r1, r2, r3
003a0d18: ldr r0, [r2, #0x30]
003a0d1c: bx lr

_ZN7Structs21DestructibleContainerD0Ev 0x4da19c 28
004da19c: push {r4, lr}
004da1a0: mov r4, r0
004da1a4: bl #0x4da15c
004da1a8: mov r0, r4
004da1ac: bl #0x310440
004da1b0: mov r0, r4
004da1b4: pop {r4, pc}

_ZThn4_N21DestructibleContainer17DeclarePropertiesEv 0x3a1268 8
003a1268: sub r0, r0, #4
003a126c: b #0x3a1270

_ZN21DestructibleContainerC1EN10ObjectBase6GO_IDSE 0x3a14bc 76
003a14bc: push {r4, r5, r6, lr}
003a14c0: ldr r5, [pc, #0x38]
003a14c4: mov r4, r0
003a14c8: bl #0x3a0788
003a14cc: ldr r3, [pc, #0x30]
003a14d0: add r5, pc, r5
003a14d4: mov r2, #0
003a14d8: ldr r3, [r5, r3]
003a14dc: str r2, [r4, #0x6f4]
003a14e0: str r2, [r4, #0x6f0]
003a14e4: add r1, r3, #8
003a14e8: add r2, r3, #0x100
003a14ec: add r3, r3, #0xf4
003a14f0: stm r4, {r1, r3}
003a14f4: str r2, [r4, #0x24]
003a14f8: mov r0, r4
003a14fc: pop {r4, r5, r6, pc}
003a1500: subseq r3, pc, r0, asr #11
003a1504: strheq r1, [r0], -r8

_ZThn36_N9ContainerD0Ev 0x3a0574 8
003a0574: sub r0, r0, #0x24
003a0578: b #0x3a057c

_ZNK9Container9GetDataIdEv 0x39f33c 8
0039f33c: mvn r0, #0
0039f340: bx lr

_ZNK21DestructibleContainer11KeepPhysicsEv 0x3a10f8 72
003a10f8: push {r4, lr}
003a10fc: ldr r0, [r0, #0x38c]
003a1100: bl #0x3a1084
003a1104: ldr r4, [pc, #0x2c]
003a1108: cmn r0, #1
003a110c: add r4, pc, r4
003a1110: beq #0x3a1130
003a1114: ldr r3, [pc, #0x20]
003a1118: mov r2, #0x44
003a111c: ldr r3, [r4, r3]
003a1120: ldr r3, [r3]
003a1124: mla r0, r2, r0, r3
003a1128: ldrb r0, [r0, #0x20]
003a112c: pop {r4, pc}
003a1130: mov r0, #0
003a1134: pop {r4, pc}
003a1138: subseq r3, pc, r4, lsl #19
003a113c: andeq r3, r0, r4, asr r8

_ZN9Container6DoOpenEv 0x3a0a98 160
003a0a98: push {r4, r5, r6, lr}
003a0a9c: sub sp, sp, #0x10
003a0aa0: ldr r3, [r0]
003a0aa4: mov r4, r0
003a0aa8: mov lr, pc
003a0aac: ldr pc, [r3, #0xd4]
003a0ab0: ldr r3, [r4]
003a0ab4: mov r5, r0
003a0ab8: mov r0, r4
003a0abc: ldr r6, [r4, #0x398]
003a0ac0: mov lr, pc
003a0ac4: ldr pc, [r3, #0xe0]
003a0ac8: mov ip, #0
003a0acc: mov r3, r0
003a0ad0: mov r2, r6
003a0ad4: mov r0, r5
003a0ad8: mov r1, r4
003a0adc: str ip, [sp]
003a0ae0: bl #0x3ecba0
003a0ae4: ldr r3, [r4, #0x300]
003a0ae8: cmp r3, #0
003a0aec: beq #0x3a0b2c
003a0af0: add r5, sp, #8
003a0af4: mov r0, r5
003a0af8: bl #0x3192b4
003a0afc: ldr r1, [r4, #0x398]
003a0b00: cmp r1, #0
003a0b04: beq #0x3a0b10
003a0b08: mov r0, r5
003a0b0c: bl #0x386f28
003a0b10: ldr r1, [pc, #0x1c]
003a0b14: ldr r0, [r4, #0x300]
003a0b18: mov r2, r5
003a0b1c: add r1, pc, r1
003a0b20: bl #0x37c41c
003a0b24: mov r0, r5
003a0b28: bl #0x319228
003a0b2c: add sp, sp, #0x10
003a0b30: pop {r4, r5, r6, pc}
003a0b34: subseq r2, r2, r4, ror #8

_ZN21DestructibleContainer8InteractEP10GameObject 0x3a0da0 740
003a0da0: push {r4, r5, r6, r7, r8, sb, sl, lr}
003a0da4: ldr r3, [r0, #0x6f4]
003a0da8: ldr r5, [pc, #0x294]
003a0dac: sub sp, sp, #0x48
003a0db0: cmp r3, #0
003a0db4: mov r4, r0
003a0db8: mov r6, r1
003a0dbc: add r5, pc, r5
003a0dc0: beq #0x3a0e68
003a0dc4: ldr r2, [r0, #0x2d8]
003a0dc8: sub r1, r3, #1
003a0dcc: str r1, [r0, #0x6f4]
003a0dd0: cmp r2, #0
003a0dd4: beq #0x3a0e00
003a0dd8: ldr ip, [r2, #0x38]
003a0ddc: ldr r0, [r0, #0x6f0]
003a0de0: mov r3, #0
003a0de4: mov r2, r3
003a0de8: rsb r1, r1, r0
003a0dec: mov r0, ip
003a0df0: ldr ip, [ip]
003a0df4: str r3, [sp]
003a0df8: mov lr, pc
003a0dfc: ldr pc, [ip, #0x1c]
003a0e00: ldr r2, [pc, #0x240]
003a0e04: ldr r3, [r4]
003a0e08: mov r0, r4
003a0e0c: ldr r2, [r5, r2]
003a0e10: ldr r7, [r2]
003a0e14: mov lr, pc
003a0e18: ldr pc, [r3, #0xd8]
003a0e1c: ldr lr, [r4, #0x168]
003a0e20: ldr r6, [r4, #0x160]
003a0e24: ldr r5, [r4, #0x164]
003a0e28: mov ip, #0xbf000000
003a0e2c: add ip, ip, #0x800000
003a0e30: mov r1, r0
003a0e34: str lr, [sp, #0x38]
003a0e38: mov r0, r7
003a0e3c: mov lr, #1
003a0e40: add r2, sp, #0x30
003a0e44: mov r3, #0
003a0e48: str r6, [sp, #0x30]
003a0e4c: str r5, [sp, #0x34]
003a0e50: str lr, [sp]
003a0e54: str ip, [sp, #8]
003a0e58: str ip, [sp, #4]
003a0e5c: bl #0x36b5d8
003a0e60: add sp, sp, #0x48
003a0e64: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003a0e68: ldr r7, [pc, #0x1dc]
003a0e6c: ldr r0, [r5, r7]
003a0e70: bl #0x31f594
003a0e74: subs r8, r0, #0
003a0e78: beq #0x3a0fe8
003a0e7c: ldr r3, [r4]
003a0e80: mov r0, r4
003a0e84: mov lr, pc
003a0e88: ldr pc, [r3, #0xd0]
003a0e8c: ldr r7, [r5, r7]
003a0e90: ldr r1, [pc, #0x1b8]
003a0e94: ldr r2, [pc, #0x1b8]
003a0e98: mov sb, r0
003a0e9c: add r1, pc, r1
003a0ea0: add r2, pc, r2
003a0ea4: ldr r0, [r7, #0x2c]
003a0ea8: ldr sl, [r4, #0x64]
003a0eac: bl #0x4c4bdc
003a0eb0: ldr r3, [pc, #0x1a0]
003a0eb4: add r1, sp, #0x48
003a0eb8: str r0, [sp, #0x18]
003a0ebc: ldr r3, [r5, r3]
003a0ec0: mov r0, r8
003a0ec4: mov r8, #0
003a0ec8: add r3, r3, #8
003a0ecc: str r3, [r1, #-0x34]!
003a0ed0: mvn r3, #0
003a0ed4: str r3, [sp, #0x28]
003a0ed8: str sl, [sp, #0x20]
003a0edc: str sb, [sp, #0x2c]
003a0ee0: str r6, [sp, #0x1c]
003a0ee4: strb r8, [sp, #0x24]
003a0ee8: strb r8, [sp, #0x25]
003a0eec: bl #0x339090
003a0ef0: ldr r3, [pc, #0x164]
003a0ef4: mov r0, r4
003a0ef8: mov r1, r6
003a0efc: ldr r3, [r5, r3]
003a0f00: add r4, sp, #0x3c
003a0f04: add r3, r3, #8
003a0f08: str r3, [sp, #0x14]
003a0f0c: bl #0x3a0b38
003a0f10: mov r0, r4
003a0f14: mov r1, r6
003a0f18: bl #0x33dd2c
003a0f1c: mov r0, r4
003a0f20: bl #0x33ff54
003a0f24: subs r4, r0, #0
003a0f28: beq #0x3a0e60
003a0f2c: ldr r3, [r4]
003a0f30: mov lr, pc
003a0f34: ldr pc, [r3, #0x28]
003a0f38: cmp r0, r8
003a0f3c: beq #0x3a0e60
003a0f40: add r6, r4, #0x560
003a0f44: mov r0, r6
003a0f48: mov r1, #0xd9
003a0f4c: mov r2, #1
003a0f50: bl #0x3e0798
003a0f54: ldr r3, [pc, #0x104]
003a0f58: mov r0, r6
003a0f5c: mov r1, #0xd9
003a0f60: ldr r3, [r5, r3]
003a0f64: mov r2, r8
003a0f68: ldr r6, [r3]
003a0f6c: bl #0x3df6e0
003a0f70: cmp r0, #0xc7
003a0f74: ble #0x3a0e60
003a0f78: ldr r0, [r7, #0x40]
003a0f7c: mov r1, r4
003a0f80: bl #0x36effc
003a0f84: cmp r0, r8
003a0f88: beq #0x3a0e60
003a0f8c: ldr r3, [pc, #0xd0]
003a0f90: ldr r3, [r5, r3]
003a0f94: ldr r7, [r3]
003a0f98: cmp r7, r8
003a0f9c: beq #0x3a103c
003a0fa0: ldr r3, [pc, #0xc0]
003a0fa4: ldr r3, [r5, r3]
003a0fa8: ldr r5, [pc, #0xbc]
003a0fac: ldr r4, [r3]
003a0fb0: add r5, pc, r5
003a0fb4: b #0x3a0fc4
003a0fb8: add r8, r8, #1
003a0fbc: cmp r8, r7
003a0fc0: beq #0x3a103c
003a0fc4: ldr r1, [r4, r8, lsl #2]
003a0fc8: mov r0, r5
003a0fcc: bl #0x30e31c
003a0fd0: cmp r0, #0
003a0fd4: bne #0x3a0fb8
003a0fd8: mov r1, r8
003a0fdc: mov r0, r6
003a0fe0: bl #0x3813b8
003a0fe4: b #0x3a0e60
003a0fe8: ldr r3, [pc, #0x80]
003a0fec: ldr r3, [r5, r3]
003a0ff0: ldr r3, [r3]
003a0ff4: cmp r3, #2
003a0ff8: streq r8, [r8]
003a0ffc: beq #0x3a0e7c
003a1000: cmp r3, #1
003a1004: bne #0x3a0e7c
003a1008: ldr r0, [pc, #0x64]
003a100c: ldr r1, [pc, #0x64]
003a1010: ldr r2, [pc, #0x64]
003a1014: ldr r0, [r5, r0]
003a1018: ldr r3, [pc, #0x60]
003a101c: mov ip, #0xe7
003a1020: add r1, pc, r1
003a1024: add r2, pc, r2
003a1028: add r3, pc, r3
003a102c: add r0, r0, #0xa8
003a1030: str ip, [sp]
003a1034: bl #0x30e004
003a1038: b #0x3a0e7c
003a103c: mvn r1, #0
003a1040: b #0x3a0fdc
003a1044: ldrsbeq r3, [pc], #-0xc4
003a1048: andeq r0, r0, r4, lsr #27
003a104c: strdeq r3, r4, [r0], -r4
003a1050: subseq r1, r2, ip, asr #21
003a1054: subseq r2, r2, r0, asr r1
003a1058: muleq r0, r0, r6
003a105c: strheq r0, [r0], -r0
003a1060: andeq r1, r0, r0, ror sp
003a1064: strdeq r0, r1, [r0], -ip
003a1068: andeq r1, r0, ip, lsr #32
003a106c: subseq r2, r2, r8, asr r0
003a1070: andeq r3, r0, r0, asr #19
003a1074: andeq r1, r0, r0, asr #19
003a1078: ldrheq sp, [r1], #-0x38
003a107c: subseq lr, r6, r4, lsr sb
003a1080: subseq r1, r2, r8, ror #30

_ZN9Container18NetStructContainerC1Ev 0x39ff58 440
0039ff58: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0039ff5c: ldr r5, [pc, #0x190]
0039ff60: mov r4, r0
0039ff64: bl #0x8138f4
0039ff68: ldr r3, [pc, #0x188]
0039ff6c: ldr r6, [pc, #0x188]
0039ff70: add r5, pc, r5
0039ff74: ldr r3, [r5, r3]
0039ff78: ldr r2, [r4, #0x150]
0039ff7c: ldr r0, [r5, r6]
0039ff80: add r3, r3, #8
0039ff84: mov r8, #0
0039ff88: mov sb, #0
0039ff8c: mov ip, #0x138
0039ff90: strd r8, sb, [r4, ip]
0039ff94: cmp r2, #0
0039ff98: mvn r1, #0
0039ff9c: mov r2, #0
0039ffa0: add r0, r0, #8
0039ffa4: str r3, [r4]
0039ffa8: mov r3, #1
0039ffac: str r3, [r4, #0x134]
0039ffb0: str r1, [r4, #0x144]
0039ffb4: str r0, [r4, #0x130]
0039ffb8: str r1, [r4, #0x140]
0039ffbc: str r2, [r4, #0x148]
0039ffc0: strb r2, [r4, #0x14c]
0039ffc4: addeq r8, r4, #0x130
0039ffc8: beq #0x39ffdc
0039ffcc: add r8, r4, #0x130
0039ffd0: str r2, [r4, #0x150]
0039ffd4: mov r0, r8
0039ffd8: bl #0x814f84
0039ffdc: ldr r2, [pc, #0x11c]
0039ffe0: ldr r3, [pc, #0x11c]
0039ffe4: ldr r1, [r4, #0x178]
0039ffe8: ldr r2, [r5, r2]
0039ffec: ldr r3, [r5, r3]
0039fff0: mov sl, #0
0039fff4: add r2, r2, #8
0039fff8: mov fp, #0
0039fffc: mov ip, #0x160
003a0000: strd sl, fp, [r4, ip]
003a0004: cmp r1, #0
003a0008: mvn r0, #0
003a000c: mov r1, #0
003a0010: add r3, r3, #8
003a0014: str r2, [r4, #0x130]
003a0018: mov r2, #0x10
003a001c: str r2, [r4, #0x15c]
003a0020: str r0, [r4, #0x16c]
003a0024: str r3, [r4, #0x158]
003a0028: str r0, [r4, #0x168]
003a002c: str r1, [r4, #0x170]
003a0030: strb r1, [r4, #0x174]
003a0034: addeq r7, r4, #0x158
003a0038: beq #0x3a004c
003a003c: add r7, r4, #0x158
003a0040: str r1, [r4, #0x178]
003a0044: mov r0, r7
003a0048: bl #0x814f84
003a004c: ldr r3, [pc, #0xb4]
003a0050: ldr r2, [r4, #0x1a0]
003a0054: ldr r0, [r5, r6]
003a0058: ldr r3, [r5, r3]
003a005c: mov sl, #0
003a0060: mov fp, #0
003a0064: add r3, r3, #8
003a0068: mov ip, #0x188
003a006c: strd sl, fp, [r4, ip]
003a0070: cmp r2, #0
003a0074: mvn r1, #0
003a0078: mov r2, #0
003a007c: add r0, r0, #8
003a0080: str r3, [r4, #0x158]
003a0084: mov r3, #0x20
003a0088: str r3, [r4, #0x184]
003a008c: str r1, [r4, #0x194]
003a0090: str r0, [r4, #0x180]
003a0094: str r1, [r4, #0x190]
003a0098: str r2, [r4, #0x198]
003a009c: strb r2, [r4, #0x19c]
003a00a0: addeq r6, r4, #0x180
003a00a4: beq #0x3a00b8
003a00a8: add r6, r4, #0x180
003a00ac: str r2, [r4, #0x1a0]
003a00b0: mov r0, r6
003a00b4: bl #0x814f84
003a00b8: ldr r3, [pc, #0x4c]
003a00bc: mov r1, r8
003a00c0: mov r0, r4
003a00c4: ldr r3, [r5, r3]
003a00c8: add r3, r3, #8
003a00cc: str r3, [r4, #0x180]
003a00d0: bl #0x81324c
003a00d4: mov r0, r4
003a00d8: mov r1, r7
003a00dc: bl #0x81324c
003a00e0: mov r0, r4
003a00e4: mov r1, r6
003a00e8: bl #0x81324c
003a00ec: mov r0, r4
003a00f0: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a00f4: subseq r4, pc, r0, lsr #22
003a00f8: strdeq r0, r1, [r0], -r0
003a00fc: andeq r4, r0, r8, rrx
003a0100: strdeq r3, r4, [r0], -r4
003a0104: andeq r2, r0, r4, lsl #19
003a0108: andeq r3, r0, ip, lsr r5
003a010c: strdeq r3, r4, [r0], -r0

_ZThn36_N21DestructibleContainerD0Ev 0x3a1458 8
003a1458: sub r0, r0, #0x24
003a145c: b #0x3a1460

_ZN9Container9InitFinalEv 0x39fc98 160
0039fc98: push {r4, r5, r6, lr}
0039fc9c: mov r4, r0
0039fca0: bl #0x38bd64
0039fca4: ldr r3, [r4, #0x274]
0039fca8: ldr r5, [pc, #0x80]
0039fcac: cmp r0, r3
0039fcb0: add r5, pc, r5
0039fcb4: blt #0x39fcbc
0039fcb8: pop {r4, r5, r6, pc}
0039fcbc: mov r0, r4
0039fcc0: bl #0x38cd48
0039fcc4: ldr r3, [r4, #0x394]
0039fcc8: sub r3, r3, #3
0039fccc: cmp r3, #1
0039fcd0: bls #0x39fce4
0039fcd4: ldr r3, [r4]
0039fcd8: mov r0, r4
0039fcdc: mov lr, pc
0039fce0: ldr pc, [r3, #0x2c]
0039fce4: mov r0, r4
0039fce8: bl #0x38ab60
0039fcec: cmp r0, #0
0039fcf0: beq #0x39fcb8
0039fcf4: ldr r3, [pc, #0x38]
0039fcf8: mov r1, #0
0039fcfc: mov r0, #0x28
0039fd00: ldr r3, [r5, r3]
0039fd04: ldr r6, [r3, #0x44]
0039fd08: bl #0x310570
0039fd0c: mov r1, r6
0039fd10: mov r2, r4
0039fd14: mov r5, r0
0039fd18: bl #0x39fc2c
0039fd1c: mov r0, r4
0039fd20: mov r1, r5
0039fd24: mov r2, #0
0039fd28: pop {r4, r5, r6, lr}
0039fd2c: b #0x394bf8
0039fd30: subseq r4, pc, r0, ror #27
0039fd34: strdeq r3, r4, [r0], -r4

_ZN9Container15__EventCallbackERKN6glitch7collada15STriggeredEventEPv 0x3a0c64 132
003a0c64: push {r4, r5, r6, lr}
003a0c68: mov r6, r1
003a0c6c: ldr r1, [pc, #0x6c]
003a0c70: sub sp, sp, #8
003a0c74: mov r5, r0
003a0c78: add r1, pc, r1
003a0c7c: ldr r0, [r0, #4]
003a0c80: bl #0x30e31c
003a0c84: cmp r0, #0
003a0c88: beq #0x3a0cd4
003a0c8c: ldr r3, [r6, #0x300]
003a0c90: cmp r3, #0
003a0c94: beq #0x3a0ccc
003a0c98: mov r0, sp
003a0c9c: bl #0x3192b4
003a0ca0: mov r0, sp
003a0ca4: ldr r1, [r5, #4]
003a0ca8: bl #0x39ec10
003a0cac: ldr r1, [pc, #0x30]
003a0cb0: ldr r0, [r6, #0x300]
003a0cb4: mov r2, sp
003a0cb8: add r1, pc, r1
003a0cbc: bl #0x37c41c
003a0cc0: mov r0, sp
003a0cc4: mov r4, sp
003a0cc8: bl #0x319228
003a0ccc: add sp, sp, #8
003a0cd0: pop {r4, r5, r6, pc}
003a0cd4: mov r0, r6
003a0cd8: bl #0x3a0a98
003a0cdc: b #0x3a0ccc
003a0ce0: subseq r2, r2, r0, lsl r3
003a0ce4: subseq r2, r2, r8, ror r2

_ZN9Container8SetStateENS_5StateE 0x39f3cc 36
0039f3cc: ldr r3, [r0, #0x2d8]
0039f3d0: cmp r1, #3
0039f3d4: ldr r3, [r3, #8]
0039f3d8: ldr r2, [r3, #0x11c]
0039f3dc: biceq r2, r2, #0x400
0039f3e0: orrne r2, r2, #0x400
0039f3e4: str r2, [r3, #0x11c]
0039f3e8: str r1, [r0, #0x394]
0039f3ec: bx lr

_ZN21DestructibleContainer17DeclarePropertiesEv 0x3a1270 4
003a1270: b #0x3a083c

_ZN21DestructibleContainer15__EventCallbackERKN6glitch7collada15STriggeredEventEPv 0x3a1274 412
003a1274: push {r4, r5, r6, r7, r8, sl, lr}
003a1278: mov r5, r1
003a127c: ldr r7, [r0, #4]
003a1280: ldr r1, [pc, #0x154]
003a1284: sub sp, sp, #0x2c
003a1288: mov r6, r0
003a128c: add r1, pc, r1
003a1290: mov r0, r7
003a1294: bl #0x30e31c
003a1298: ldr r4, [pc, #0x140]
003a129c: cmp r0, #0
003a12a0: add r4, pc, r4
003a12a4: beq #0x3a137c
003a12a8: ldr r1, [pc, #0x134]
003a12ac: mov r0, r7
003a12b0: add r1, pc, r1
003a12b4: bl #0x30e31c
003a12b8: cmp r0, #0
003a12bc: bne #0x3a1368
003a12c0: ldr r7, [pc, #0x120]
003a12c4: ldr r0, [r4, r7]
003a12c8: bl #0x31f594
003a12cc: subs r8, r0, #0
003a12d0: beq #0x3a1388
003a12d4: ldr r3, [r5]
003a12d8: mov r0, r5
003a12dc: mov lr, pc
003a12e0: ldr pc, [r3, #0xd0]
003a12e4: ldr r3, [r4, r7]
003a12e8: ldr r1, [pc, #0xfc]
003a12ec: ldr r2, [pc, #0xfc]
003a12f0: mov sl, r0
003a12f4: add r1, pc, r1
003a12f8: ldr r0, [r3, #0x2c]
003a12fc: add r2, pc, r2
003a1300: ldr r7, [r5, #0x64]
003a1304: bl #0x4c4bdc
003a1308: ldr r2, [pc, #0xe4]
003a130c: add r1, sp, #0x28
003a1310: mov r3, #0
003a1314: ldr r2, [r4, r2]
003a1318: str r0, [sp, #0x10]
003a131c: mov r0, r8
003a1320: add r2, r2, #8
003a1324: str r2, [r1, #-0x1c]!
003a1328: mvn r2, #0
003a132c: strb r3, [sp, #0x1d]
003a1330: str r3, [sp, #0x14]
003a1334: strb r3, [sp, #0x1c]
003a1338: str r7, [sp, #0x18]
003a133c: str r2, [sp, #0x20]
003a1340: str sl, [sp, #0x24]
003a1344: bl #0x339090
003a1348: ldr r3, [pc, #0xa8]
003a134c: mov r0, r6
003a1350: mov r1, r5
003a1354: ldr r3, [r4, r3]
003a1358: add r3, r3, #8
003a135c: str r3, [sp, #0xc]
003a1360: bl #0x3a0c64
003a1364: b #0x3a1374
003a1368: mov r0, r6
003a136c: mov r1, r5
003a1370: bl #0x3a0c64
003a1374: add sp, sp, #0x2c
003a1378: pop {r4, r5, r6, r7, r8, sl, pc}
003a137c: mov r0, r5
003a1380: bl #0x3a0d68
003a1384: b #0x3a1374
003a1388: ldr r3, [pc, #0x6c]
003a138c: ldr r3, [r4, r3]
003a1390: ldr r3, [r3]
003a1394: cmp r3, #2
003a1398: streq r8, [r8]
003a139c: beq #0x3a12d4
003a13a0: cmp r3, #1
003a13a4: bne #0x3a12d4
003a13a8: ldr r0, [pc, #0x50]
003a13ac: ldr r1, [pc, #0x50]
003a13b0: ldr r2, [pc, #0x50]
003a13b4: ldr r0, [r4, r0]
003a13b8: ldr r3, [pc, #0x4c]
003a13bc: mov ip, #0x45
003a13c0: add r1, pc, r1
003a13c4: add r2, pc, r2
003a13c8: add r3, pc, r3
003a13cc: add r0, r0, #0xa8
003a13d0: str ip, [sp]
003a13d4: bl #0x30e004
003a13d8: b #0x3a12d4
003a13dc: subseq pc, r1, r4, lsl fp
003a13e0: ldrsheq r3, [pc], #-0x70
003a13e4: ldrsbeq r1, [r2], #-0xc8
003a13e8: strdeq r3, r4, [r0], -r4
003a13ec: subseq r1, r2, r4, ror r6
003a13f0: ldrsheq r1, [r2], #-0xc4
003a13f4: muleq r0, r0, r6
003a13f8: strheq r0, [r0], -r0
003a13fc: andeq r3, r0, r0, asr #19
003a1400: andeq r1, r0, r0, asr #19
003a1404: subseq sp, r1, r8, lsl r0

_ZN21DestructibleContainer8InitPostEv 0x3a11dc 140
003a11dc: push {r4, r5, lr}
003a11e0: mov r4, r0
003a11e4: sub sp, sp, #0xc
003a11e8: bl #0x39f910
003a11ec: ldr r5, [r4, #0x2d8]
003a11f0: ldr r3, [pc, #0x64]
003a11f4: cmp r5, #0
003a11f8: add r3, pc, r3
003a11fc: beq #0x3a1254
003a1200: ldr r2, [r5, #0x38]
003a1204: ldr r0, [pc, #0x54]
003a1208: ldr r1, [pc, #0x54]
003a120c: ldr ip, [r2]
003a1210: str r4, [sp]
003a1214: ldr r1, [r3, r1]
003a1218: ldr r3, [r3, r0]
003a121c: mov r0, r2
003a1220: mov r2, r4
003a1224: mov lr, pc
003a1228: ldr pc, [ip, #0x2c]
003a122c: ldr r3, [r5, #0x38]
003a1230: mov r1, #0
003a1234: mov r0, r3
003a1238: ldr r3, [r3]
003a123c: mov lr, pc
003a1240: ldr pc, [r3, #0x10]
003a1244: cmp r0, #3
003a1248: subhi r0, r0, #3
003a124c: strhi r0, [r4, #0x6f4]
003a1250: strhi r0, [r4, #0x6f0]
003a1254: add sp, sp, #0xc
003a1258: pop {r4, r5, pc}

_ZN9Container9SerializeEP11IStreamBase 0x39f8e0 48
0039f8e0: push {r4, r5, lr}
0039f8e4: mov r5, r0
0039f8e8: sub sp, sp, #0xc
0039f8ec: mov r4, r1
0039f8f0: bl #0x38b9e4
0039f8f4: ldr r3, [r5, #0x394]
0039f8f8: add r1, sp, #8
0039f8fc: mov r0, r4
0039f900: strb r3, [r1, #-1]!
0039f904: bl #0x39f828
0039f908: add sp, sp, #0xc
0039f90c: pop {r4, r5, pc}

_ZNK9Container17GetObstacleRadiusEv 0x39f3b0 12
0039f3b0: mov r0, #0x43000000
0039f3b4: add r0, r0, #0x160000
0039f3b8: bx lr

_ZN21DestructibleContainerD0Ev 0x3a1460 28
003a1460: push {r4, lr}
003a1464: mov r4, r0
003a1468: bl #0x3a1418
003a146c: mov r0, r4
003a1470: bl #0x310440
003a1474: mov r0, r4
003a1478: pop {r4, pc}

_ZN7Structs21DestructibleContainer4readEP11IStreamBase 0x4fe220 1396
004fe220: push {r4, r5, r6, lr}
004fe224: mov r4, r0
004fe228: sub sp, sp, #8
004fe22c: mov r0, r1
004fe230: mov r5, r1
004fe234: add r1, r4, #4
004fe238: bl #0x459090
004fe23c: mov r3, #1
004fe240: cmp r3, #0
004fe244: str r3, [sp, #4]
004fe248: bne #0x4fe28c
004fe24c: add r3, r4, #5
004fe250: add r2, r4, #6
004fe254: ldrb r0, [r2, #1]
004fe258: ldrb r1, [r3, #-1]
004fe25c: cmp r3, r2
004fe260: eor r1, r0, r1
004fe264: strb r1, [r3, #-1]
004fe268: ldrb r0, [r2, #1]
004fe26c: eor r1, r1, r0
004fe270: strb r1, [r2, #1]
004fe274: ldrb r0, [r3, #-1]
004fe278: sub r2, r2, #1
004fe27c: eor r1, r1, r0
004fe280: strb r1, [r3, #-1]
004fe284: add r3, r3, #1
004fe288: blo #0x4fe254
004fe28c: mov r0, r5
004fe290: add r1, r4, #8
004fe294: bl #0x459090
004fe298: mov r3, #1
004fe29c: cmp r3, #0
004fe2a0: str r3, [sp, #4]
004fe2a4: bne #0x4fe2e8
004fe2a8: add r3, r4, #9
004fe2ac: add r2, r4, #0xa
004fe2b0: ldrb r0, [r2, #1]
004fe2b4: ldrb r1, [r3, #-1]
004fe2b8: cmp r3, r2
004fe2bc: eor r1, r0, r1
004fe2c0: strb r1, [r3, #-1]
004fe2c4: ldrb r0, [r2, #1]
004fe2c8: eor r1, r1, r0
004fe2cc: strb r1, [r2, #1]
004fe2d0: ldrb r0, [r3, #-1]
004fe2d4: sub r2, r2, #1
004fe2d8: eor r1, r1, r0
004fe2dc: strb r1, [r3, #-1]
004fe2e0: add r3, r3, #1
004fe2e4: blo #0x4fe2b0
004fe2e8: mov r0, r5
004fe2ec: add r1, r4, #0xc
004fe2f0: bl #0x459090
004fe2f4: mov r3, #1
004fe2f8: cmp r3, #0
004fe2fc: str r3, [sp, #4]
004fe300: bne #0x4fe344
004fe304: add r3, r4, #0xd
004fe308: add r2, r4, #0xe
004fe30c: ldrb r0, [r2, #1]
004fe310: ldrb r1, [r3, #-1]
004fe314: cmp r3, r2
004fe318: eor r1, r0, r1
004fe31c: strb r1, [r3, #-1]
004fe320: ldrb r0, [r2, #1]
004fe324: eor r1, r1, r0
004fe328: strb r1, [r2, #1]
004fe32c: ldrb r0, [r3, #-1]
004fe330: sub r2, r2, #1
004fe334: eor r1, r1, r0
004fe338: strb r1, [r3, #-1]
004fe33c: add r3, r3, #1
004fe340: blo #0x4fe30c
004fe344: mov r0, r5
004fe348: add r1, r4, #0x10
004fe34c: bl #0x459090
004fe350: mov r3, #1
004fe354: cmp r3, #0
004fe358: str r3, [sp, #4]
004fe35c: bne #0x4fe3a0
004fe360: add r3, r4, #0x11
004fe364: add r2, r4, #0x12
004fe368: ldrb r0, [r2, #1]
004fe36c: ldrb r1, [r3, #-1]
004fe370: cmp r3, r2
004fe374: eor r1, r0, r1
004fe378: strb r1, [r3, #-1]
004fe37c: ldrb r0, [r2, #1]
004fe380: eor r1, r1, r0
004fe384: strb r1, [r2, #1]
004fe388: ldrb r0, [r3, #-1]
004fe38c: sub r2, r2, #1
004fe390: eor r1, r1, r0
004fe394: strb r1, [r3, #-1]
004fe398: add r3, r3, #1
004fe39c: blo #0x4fe368
004fe3a0: mov r0, r5
004fe3a4: add r1, r4, #0x14
004fe3a8: bl #0x459090
004fe3ac: mov r3, #1
004fe3b0: cmp r3, #0
004fe3b4: str r3, [sp, #4]
004fe3b8: bne #0x4fe3fc
004fe3bc: add r3, r4, #0x15
004fe3c0: add r2, r4, #0x16
004fe3c4: ldrb r0, [r2, #1]
004fe3c8: ldrb r1, [r3, #-1]
004fe3cc: cmp r3, r2
004fe3d0: eor r1, r0, r1
004fe3d4: strb r1, [r3, #-1]
004fe3d8: ldrb r0, [r2, #1]
004fe3dc: eor r1, r1, r0
004fe3e0: strb r1, [r2, #1]
004fe3e4: ldrb r0, [r3, #-1]
004fe3e8: sub r2, r2, #1
004fe3ec: eor r1, r1, r0
004fe3f0: strb r1, [r3, #-1]
004fe3f4: add r3, r3, #1
004fe3f8: blo #0x4fe3c4
004fe3fc: mov r0, r5
004fe400: add r1, r4, #0x18
004fe404: bl #0x459090
004fe408: mov r3, #1
004fe40c: cmp r3, #0
004fe410: str r3, [sp, #4]
004fe414: bne #0x4fe458
004fe418: add r3, r4, #0x19
004fe41c: add r2, r4, #0x1a
004fe420: ldrb r0, [r2, #1]
004fe424: ldrb r1, [r3, #-1]
004fe428: cmp r3, r2
004fe42c: eor r1, r0, r1
004fe430: strb r1, [r3, #-1]
004fe434: ldrb r0, [r2, #1]
004fe438: eor r1, r1, r0
004fe43c: strb r1, [r2, #1]
004fe440: ldrb r0, [r3, #-1]
004fe444: sub r2, r2, #1
004fe448: eor r1, r1, r0
004fe44c: strb r1, [r3, #-1]
004fe450: add r3, r3, #1
004fe454: blo #0x4fe420
004fe458: mov r0, r5
004fe45c: add r1, r4, #0x1c
004fe460: bl #0x459090
004fe464: mov r3, #1
004fe468: cmp r3, #0
004fe46c: str r3, [sp, #4]
004fe470: bne #0x4fe4b4
004fe474: add r3, r4, #0x1d
004fe478: add r2, r4, #0x1e
004fe47c: ldrb r0, [r2, #1]
004fe480: ldrb r1, [r3, #-1]
004fe484: cmp r3, r2
004fe488: eor r1, r0, r1
004fe48c: strb r1, [r3, #-1]
004fe490: ldrb r0, [r2, #1]
004fe494: eor r1, r1, r0
004fe498: strb r1, [r2, #1]
004fe49c: ldrb r0, [r3, #-1]
004fe4a0: sub r2, r2, #1
004fe4a4: eor r1, r1, r0
004fe4a8: strb r1, [r3, #-1]
004fe4ac: add r3, r3, #1
004fe4b0: blo #0x4fe47c
004fe4b4: add r1, r4, #0x20
004fe4b8: mov r0, r5
004fe4bc: bl #0x4db89c
004fe4c0: mov r0, r5
004fe4c4: add r1, r4, #0x24
004fe4c8: bl #0x459090
004fe4cc: mov r3, #1
004fe4d0: cmp r3, #0
004fe4d4: str r3, [sp, #4]
004fe4d8: bne #0x4fe51c
004fe4dc: add r3, r4, #0x25
004fe4e0: add r2, r4, #0x26
004fe4e4: ldrb r0, [r2, #1]
004fe4e8: ldrb r1, [r3, #-1]
004fe4ec: cmp r3, r2
004fe4f0: eor r1, r0, r1
004fe4f4: strb r1, [r3, #-1]
004fe4f8: ldrb r0, [r2, #1]
004fe4fc: eor r1, r1, r0
004fe500: strb r1, [r2, #1]
004fe504: ldrb r0, [r3, #-1]
004fe508: sub r2, r2, #1
004fe50c: eor r1, r1, r0
004fe510: strb r1, [r3, #-1]
004fe514: add r3, r3, #1
004fe518: blo #0x4fe4e4
004fe51c: mov r0, r5
004fe520: add r1, r4, #0x28
004fe524: bl #0x459090
004fe528: mov r3, #1
004fe52c: cmp r3, #0
004fe530: str r3, [sp, #4]
004fe534: bne #0x4fe578
004fe538: add r3, r4, #0x29
004fe53c: add r2, r4, #0x2a
004fe540: ldrb r0, [r2, #1]
004fe544: ldrb r1, [r3, #-1]
004fe548: cmp r3, r2
004fe54c: eor r1, r0, r1
004fe550: strb r1, [r3, #-1]
004fe554: ldrb r0, [r2, #1]
004fe558: eor r1, r1, r0
004fe55c: strb r1, [r2, #1]
004fe560: ldrb r0, [r3, #-1]
004fe564: sub r2, r2, #1
004fe568: eor r1, r1, r0
004fe56c: strb r1, [r3, #-1]
004fe570: add r3, r3, #1
004fe574: blo #0x4fe540
004fe578: mov r0, r5
004fe57c: add r1, r4, #0x2c
004fe580: bl #0x3df1a0
004fe584: mov r3, #1
004fe588: cmp r3, #0
004fe58c: str r3, [sp, #4]
004fe590: bne #0x4fe5d4
004fe594: add r3, r4, #0x2d
004fe598: add r2, r4, #0x2e
004fe59c: ldrb r0, [r2, #1]
004fe5a0: ldrb r1, [r3, #-1]
004fe5a4: cmp r3, r2
004fe5a8: eor r1, r0, r1
004fe5ac: strb r1, [r3, #-1]
004fe5b0: ldrb r0, [r2, #1]
004fe5b4: eor r1, r1, r0
004fe5b8: strb r1, [r2, #1]
004fe5bc: ldrb r0, [r3, #-1]
004fe5c0: sub r2, r2, #1
004fe5c4: eor r1, r1, r0
004fe5c8: strb r1, [r3, #-1]
004fe5cc: add r3, r3, #1
004fe5d0: blo #0x4fe59c
004fe5d4: ldr r0, [r4, #0x30]
004fe5d8: cmp r0, #0
004fe5dc: beq #0x4fe5e4
004fe5e0: bl #0x310440
004fe5e4: ldr r0, [r4, #0x2c]
004fe5e8: mov r1, #1
004fe5ec: mov r6, #0
004fe5f0: add r0, r0, r1
004fe5f4: bl #0x31056c
004fe5f8: ldr r2, [r4, #0x2c]
004fe5fc: mov r1, r0
004fe600: str r0, [r4, #0x30]
004fe604: mov r3, r6
004fe608: mov r0, r5
004fe60c: bl #0x317454
004fe610: ldr r3, [r4, #0x2c]
004fe614: ldr r2, [r4, #0x30]
004fe618: mov r0, r5
004fe61c: add r1, r4, #0x34
004fe620: strb r6, [r2, r3]
004fe624: bl #0x459090
004fe628: mov r3, #1
004fe62c: cmp r3, r6
004fe630: str r3, [sp, #4]
004fe634: bne #0x4fe678
004fe638: add r3, r4, #0x35
004fe63c: add r2, r4, #0x36
004fe640: ldrb r0, [r2, #1]
004fe644: ldrb r1, [r3, #-1]
004fe648: cmp r3, r2
004fe64c: eor r1, r0, r1
004fe650: strb r1, [r3, #-1]
004fe654: ldrb r0, [r2, #1]
004fe658: eor r1, r1, r0
004fe65c: strb r1, [r2, #1]
004fe660: ldrb r0, [r3, #-1]
004fe664: sub r2, r2, #1
004fe668: eor r1, r1, r0
004fe66c: strb r1, [r3, #-1]
004fe670: add r3, r3, #1
004fe674: blo #0x4fe640
004fe678: mov r0, r5
004fe67c: add r1, r4, #0x38
004fe680: bl #0x459090
004fe684: mov r3, #1
004fe688: cmp r3, #0
004fe68c: str r3, [sp, #4]
004fe690: bne #0x4fe6d4
004fe694: add r3, r4, #0x39
004fe698: add r2, r4, #0x3a
004fe69c: ldrb r0, [r2, #1]
004fe6a0: ldrb r1, [r3, #-1]
004fe6a4: cmp r3, r2
004fe6a8: eor r1, r0, r1
004fe6ac: strb r1, [r3, #-1]
004fe6b0: ldrb r0, [r2, #1]
004fe6b4: eor r1, r1, r0
004fe6b8: strb r1, [r2, #1]
004fe6bc: ldrb r0, [r3, #-1]
004fe6c0: sub r2, r2, #1
004fe6c4: eor r1, r1, r0
004fe6c8: strb r1, [r3, #-1]
004fe6cc: add r3, r3, #1
004fe6d0: blo #0x4fe69c
004fe6d4: mov r0, r5
004fe6d8: add r1, r4, #0x3c
004fe6dc: bl #0x459090
004fe6e0: mov r3, #1
004fe6e4: cmp r3, #0
004fe6e8: str r3, [sp, #4]
004fe6ec: bne #0x4fe730
004fe6f0: add r3, r4, #0x3d
004fe6f4: add r2, r4, #0x3e
004fe6f8: ldrb r0, [r2, #1]
004fe6fc: ldrb r1, [r3, #-1]
004fe700: cmp r3, r2
004fe704: eor r1, r0, r1
004fe708: strb r1, [r3, #-1]
004fe70c: ldrb r0, [r2, #1]
004fe710: eor r1, r1, r0
004fe714: strb r1, [r2, #1]
004fe718: ldrb r0, [r3, #-1]
004fe71c: sub r2, r2, #1
004fe720: eor r1, r1, r0
004fe724: strb r1, [r3, #-1]
004fe728: add r3, r3, #1
004fe72c: blo #0x4fe6f8
004fe730: mov r0, r5
004fe734: add r1, r4, #0x40
004fe738: bl #0x459090
004fe73c: mov r3, #1
004fe740: cmp r3, #0
004fe744: str r3, [sp, #4]
004fe748: bne #0x4fe78c
004fe74c: add r3, r4, #0x42
004fe750: add r4, r4, #0x41
004fe754: ldrb r1, [r3, #1]
004fe758: ldrb r2, [r4, #-1]
004fe75c: cmp r4, r3
004fe760: eor r2, r1, r2
004fe764: strb r2, [r4, #-1]
004fe768: ldrb r1, [r3, #1]
004fe76c: eor r2, r2, r1
004fe770: strb r2, [r3, #1]
004fe774: ldrb r1, [r4, #-1]
004fe778: sub r3, r3, #1
004fe77c: eor r2, r2, r1
004fe780: strb r2, [r4, #-1]
004fe784: add r4, r4, #1
004fe788: blo #0x4fe754
004fe78c: add sp, sp, #8
004fe790: pop {r4, r5, r6, pc}

_ZN9Container5SpawnEv 0x39f3f0 92
0039f3f0: push {r4, r5, lr}
0039f3f4: ldr r5, [r0, #0x2d8]
0039f3f8: sub sp, sp, #0xc
0039f3fc: cmp r5, #0
0039f400: beq #0x39f440
0039f404: ldr r4, [r0, #0x394]
0039f408: cmp r4, #0
0039f40c: bne #0x39f440
0039f410: mov r1, #1
0039f414: bl #0x39f3cc
0039f418: ldr r3, [r5, #0x38]
0039f41c: ldr r1, [pc, #0x24]
0039f420: mov r2, r4
0039f424: ldr ip, [r3]
0039f428: mov r0, r3
0039f42c: add r1, pc, r1
0039f430: str r4, [sp]
0039f434: mov r3, r4
0039f438: mov lr, pc
0039f43c: ldr pc, [ip, #0x20]
0039f440: add sp, sp, #0xc
0039f444: pop {r4, r5, pc}
0039f448: subseq r3, r2, ip, ror sl

_ZNK9Container9GetVisualEv 0x39f334 8
0039f334: mvn r0, #0
0039f338: bx lr

_ZNK9Container7GetLootEv 0x39f344 8
0039f344: mvn r0, #0
0039f348: bx lr

_ZN9ContainerD0Ev 0x3a057c 28
003a057c: push {r4, lr}
003a0580: mov r4, r0
003a0584: bl #0x3a0430
003a0588: mov r0, r4
003a058c: bl #0x310440
003a0590: mov r0, r4
003a0594: pop {r4, pc}

_ZNK21DestructibleContainer8GetSoundEv 0x3a1140 64
003a1140: push {r4, lr}
003a1144: ldr r0, [r0, #0x38c]
003a1148: bl #0x3a1084
003a114c: ldr r4, [pc, #0x24]
003a1150: cmn r0, #1
003a1154: add r4, pc, r4
003a1158: beq #0x3a1174
003a115c: ldr r3, [pc, #0x18]
003a1160: mov r2, #0x44
003a1164: ldr r3, [r4, r3]
003a1168: ldr r3, [r3]
003a116c: mla r0, r2, r0, r3
003a1170: ldr r0, [r0, #0x1c]
003a1174: pop {r4, pc}
003a1178: subseq r3, pc, ip, lsr sb
003a117c: andeq r3, r0, r4, asr r8

_ZNK9Container6IsDeadEv 0x39f364 24
0039f364: ldr r0, [r0, #0x394]
0039f368: sub r0, r0, #3
0039f36c: cmp r0, #1
0039f370: movhi r0, #0
0039f374: movls r0, #1
0039f378: bx lr

_ZNK21DestructibleContainer7GetLootEv 0x3a1180 64
003a1180: push {r4, lr}
003a1184: ldr r0, [r0, #0x38c]
003a1188: bl #0x3a1084
003a118c: ldr r4, [pc, #0x24]
003a1190: cmn r0, #1
003a1194: add r4, pc, r4
003a1198: beq #0x3a11b4
003a119c: ldr r3, [pc, #0x18]
003a11a0: mov r2, #0x44
003a11a4: ldr r3, [r4, r3]
003a11a8: ldr r3, [r3]
003a11ac: mla r0, r2, r0, r3
003a11b0: ldr r0, [r0, #0x28]
003a11b4: pop {r4, pc}
003a11b8: ldrsheq r3, [pc], #-0x8c
003a11bc: andeq r3, r0, r4, asr r8

_ZNK9Container9IsZonableEv 0x39f534 4
0039f534: b #0x38ab60

_ZN9Container26InterpretIncomingNetStructEb 0x39fd38 544
0039fd38: push {r4, r5, r6, r7, lr}
0039fd3c: ldr r6, [r0, #0x698]
0039fd40: ldr r7, [pc, #0x1fc]
0039fd44: sub sp, sp, #0x1c
0039fd48: cmp r6, #1
0039fd4c: mov r4, r0
0039fd50: mov r5, r1
0039fd54: add r7, pc, r7
0039fd58: beq #0x39fd84
0039fd5c: cmp r5, #0
0039fd60: beq #0x39fd7c
0039fd64: ldr r3, [r4, #0x394]
0039fd68: sub r3, r3, #3
0039fd6c: cmp r3, #1
0039fd70: bls #0x39fe70
0039fd74: ldr r3, [r4, #0x6e8]
0039fd78: str r3, [r4, #0xfc]
0039fd7c: add sp, sp, #0x1c
0039fd80: pop {r4, r5, r6, r7, pc}
0039fd84: ldr r3, [r0, #0x394]
0039fd88: sub r3, r3, #3
0039fd8c: cmp r3, #1
0039fd90: bls #0x39fd5c
0039fd94: ldr r3, [pc, #0x1ac]
0039fd98: add r6, sp, #0xc
0039fd9c: ldr r2, [r0, #0x6c0]
0039fda0: ldr r3, [r7, r3]
0039fda4: mov r0, r6
0039fda8: ldr r1, [r3, #0x38]
0039fdac: bl #0x3407a0
0039fdb0: mov r0, r6
0039fdb4: bl #0x33fee4
0039fdb8: subs r1, r0, #0
0039fdbc: beq #0x39fdc8
0039fdc0: cmp r5, #0
0039fdc4: beq #0x39fe5c
0039fdc8: ldr r3, [r4, #0x394]
0039fdcc: sub r3, r3, #3
0039fdd0: cmp r3, #1
0039fdd4: bls #0x39fe50
0039fdd8: str r1, [r4, #0x398]
0039fddc: mov r0, r4
0039fde0: mov r1, #4
0039fde4: bl #0x39f3cc
0039fde8: ldr r3, [r4]
0039fdec: mov r0, r4
0039fdf0: mov lr, pc
0039fdf4: ldr pc, [r3, #0xdc]
0039fdf8: subs r1, r0, #0
0039fdfc: beq #0x39ff34
0039fe00: ldr r3, [r4, #0x2d8]
0039fe04: cmp r3, #0
0039fe08: beq #0x39fe50
0039fe0c: cmp r5, #0
0039fe10: beq #0x39fef8
0039fe14: mov r0, r4
0039fe18: mov r1, #4
0039fe1c: bl #0x39f3cc
0039fe20: ldr r2, [r4, #0x2d8]
0039fe24: ldr r1, [pc, #0x120]
0039fe28: mov r3, #0
0039fe2c: ldr ip, [r2, #0x38]
0039fe30: add r1, pc, r1
0039fe34: mov r2, r3
0039fe38: mov r0, ip
0039fe3c: ldr ip, [ip]
0039fe40: str r3, [sp]
0039fe44: mov lr, pc
0039fe48: ldr pc, [ip, #0x20]
0039fe4c: b #0x39fd74
0039fe50: cmp r5, #0
0039fe54: bne #0x39fd74
0039fe58: b #0x39fd7c
0039fe5c: mov r0, r4
0039fe60: ldr r3, [r4]
0039fe64: mov lr, pc
0039fe68: ldr pc, [r3, #0x98]
0039fe6c: b #0x39fd7c
0039fe70: cmp r6, #0
0039fe74: bne #0x39fd74
0039fe78: mov r0, r4
0039fe7c: mov r1, #2
0039fe80: bl #0x39f3cc
0039fe84: ldr r3, [pc, #0xbc]
0039fe88: mov r1, r6
0039fe8c: mov r0, #0x28
0039fe90: ldr r3, [r7, r3]
0039fe94: ldr r7, [r3, #0x44]
0039fe98: bl #0x310570
0039fe9c: mov r1, r7
0039fea0: mov r5, r0
0039fea4: mov r2, r4
0039fea8: bl #0x39fc2c
0039feac: mov r2, r6
0039feb0: mov r0, r4
0039feb4: mov r1, r5
0039feb8: bl #0x394bf8
0039febc: mov r0, r4
0039fec0: mov r1, #2
0039fec4: bl #0x39f3cc
0039fec8: ldr r3, [r4, #0x2d8]
0039fecc: ldr r1, [pc, #0x7c]
0039fed0: mov r2, r6
0039fed4: ldr ip, [r3, #0x38]
0039fed8: add r1, pc, r1
0039fedc: mov r3, r6
0039fee0: mov r0, ip
0039fee4: ldr ip, [ip]
0039fee8: str r6, [sp]
0039feec: mov lr, pc
0039fef0: ldr pc, [ip, #0x20]
0039fef4: b #0x39fd74
0039fef8: mov r0, r4
0039fefc: mov r1, #3
0039ff00: bl #0x39f3cc
0039ff04: ldr r3, [r4, #0x2d8]
0039ff08: ldr r1, [pc, #0x44]
0039ff0c: mov r2, r5
0039ff10: ldr ip, [r3, #0x38]
0039ff14: add r1, pc, r1
0039ff18: mov r3, r5
0039ff1c: mov r0, ip
0039ff20: ldr ip, [ip]
0039ff24: str r5, [sp]
0039ff28: mov lr, pc
0039ff2c: ldr pc, [ip, #0x20]
0039ff30: b #0x39fd7c
0039ff34: mov r0, r4
0039ff38: mov r2, r1
0039ff3c: bl #0x394bf8
0039ff40: b #0x39fe00
0039ff44: subseq r4, pc, ip, lsr sp
0039ff48: strdeq r3, r4, [r0], -r4

_ZN12ObjectHandlecvPT_I9ContainerEEv 0x4593f4 180
004593f4: push {r4, r5, lr}
004593f8: mov r1, #0
004593fc: sub sp, sp, #0xc
00459400: bl #0x33fdc0
00459404: ldr r5, [pc, #0x84]
00459408: ldr r3, [pc, #0x84]
0045940c: mov r4, r0
00459410: add r5, pc, r5
00459414: ldr r3, [r5, r3]
00459418: ldr r3, [r3]
0045941c: cmp r3, #2
00459420: moveq r3, #0
00459424: streq r3, [r3]
00459428: beq #0x459434
0045942c: cmp r3, #1
00459430: beq #0x45945c
00459434: cmp r4, #0
00459438: bne #0x459448
0045943c: mov r0, #0
00459440: add sp, sp, #0xc
00459444: pop {r4, r5, pc}
00459448: ldr r3, [r4, #0xf4]
0045944c: cmp r3, #0x15
00459450: moveq r0, r4
00459454: bne #0x45943c
00459458: b #0x459440
0045945c: ldr r0, [pc, #0x34]
00459460: ldr r1, [pc, #0x34]
00459464: ldr r2, [pc, #0x34]
00459468: ldr r0, [r5, r0]
0045946c: ldr r3, [pc, #0x30]
00459470: mov ip, #0x48
00459474: add r1, pc, r1
00459478: add r2, pc, r2
0045947c: add r3, pc, r3
00459480: add r0, r0, #0xa8
00459484: str ip, [sp]
00459488: bl #0x30e004
0045948c: b #0x459434
00459490: subseq fp, r3, r0, lsl #13
00459494: andeq r3, r0, r0, asr #19
00459498: andeq r1, r0, r0, asr #19
0045949c: subeq r4, r6, r4, ror #30
004594a0: subeq r3, r7, r8, lsl #20
004594a4: subeq r3, r7, ip, lsr sl

_ZNK9Container11IsUpdatableEv 0x39f324 8
0039f324: mov r0, #0
0039f328: bx lr

_ZN21DestructibleContainerD1Ev 0x3a1418 64
003a1418: ldr r2, [pc, #0x30]
003a141c: ldr r3, [pc, #0x30]
003a1420: push {r4, lr}
003a1424: add r2, pc, r2
003a1428: ldr r3, [r2, r3]
003a142c: mov r4, r0
003a1430: add r2, r3, #0x100
003a1434: add r1, r3, #8
003a1438: add r3, r3, #0xf4
003a143c: stm r0, {r1, r3}
003a1440: str r2, [r0, #0x24]
003a1444: bl #0x3a0598
003a1448: mov r0, r4
003a144c: pop {r4, pc}
003a1450: subseq r3, pc, ip, ror #12
003a1454: strheq r1, [r0], -r8

_ZThn36_N9Container9SerializeEP11IStreamBase 0x39f8d8 8
0039f8d8: sub r0, r0, #0x24
0039f8dc: b #0x39f8e0

_ZNK9Container11KeepPhysicsEv 0x39f354 8
0039f354: mov r0, #0
0039f358: bx lr

_ZN7Structs21DestructibleContainerD2Ev 0x4da1b8 64
004da1b8: push {r4, lr}
004da1bc: ldr r3, [pc, #0x2c]
004da1c0: ldr r2, [pc, #0x2c]
004da1c4: mov r4, r0
004da1c8: add r3, pc, r3
004da1cc: ldr r0, [r0, #0x30]
004da1d0: ldr r2, [r3, r2]
004da1d4: cmp r0, #0
004da1d8: add r2, r2, #8
004da1dc: str r2, [r4]
004da1e0: beq #0x4da1e8
004da1e4: bl #0x310440
004da1e8: mov r0, r4
004da1ec: pop {r4, pc}
004da1f0: subeq sl, fp, r8, asr #17
004da1f4: andeq r3, r0, r4, asr #28

_ZN21DestructibleContainerC2EN10ObjectBase6GO_IDSE 0x3a1508 76
003a1508: push {r4, r5, r6, lr}
003a150c: ldr r5, [pc, #0x38]
003a1510: mov r4, r0
003a1514: bl #0x3a0788
003a1518: ldr r3, [pc, #0x30]
003a151c: add r5, pc, r5
003a1520: mov r2, #0
003a1524: ldr r3, [r5, r3]
003a1528: str r2, [r4, #0x6f4]
003a152c: str r2, [r4, #0x6f0]
003a1530: add r1, r3, #8
003a1534: add r2, r3, #0x100
003a1538: add r3, r3, #0xf4
003a153c: stm r4, {r1, r3}
003a1540: str r2, [r4, #0x24]
003a1544: mov r0, r4
003a1548: pop {r4, r5, r6, pc}
003a154c: subseq r3, pc, r4, ror r5
003a1550: strheq r1, [r0], -r8

_ZN9Container17DeclarePropertiesEv 0x3a083c 604
003a083c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a0840: ldr r4, [pc, #0x22c]
003a0844: ldr r2, [pc, #0x22c]
003a0848: ldr r3, [pc, #0x22c]
003a084c: sub sp, sp, #0x4c
003a0850: add r4, pc, r4
003a0854: str r3, [sp]
003a0858: ldr r3, [r4, r2]
003a085c: add r5, r0, #4
003a0860: mov sb, r0
003a0864: ldr r3, [r3]
003a0868: str r2, [sp, #4]
003a086c: ldr r7, [pc, #0x20c]
003a0870: str r3, [sp, #0x44]
003a0874: bl #0x38cee8
003a0878: mov r1, #0
003a087c: mov r0, #0x24
003a0880: bl #0x310570
003a0884: ldr r2, [sp]
003a0888: add r7, pc, r7
003a088c: mov r6, r0
003a0890: ldr fp, [r4, r2]
003a0894: mov r1, r7
003a0898: add r2, sp, #0x10
003a089c: add fp, fp, #8
003a08a0: str fp, [r0], #8
003a08a4: bl #0x3140ec
003a08a8: ldr r3, [pc, #0x1d4]
003a08ac: add r2, sb, #0x374
003a08b0: rsb r2, r5, r2
003a08b4: ldr r3, [r4, r3]
003a08b8: str r2, [r6, #4]
003a08bc: mov r1, r7
003a08c0: add r3, r3, #8
003a08c4: str r3, [r6]
003a08c8: mvn r3, #0
003a08cc: str r3, [r6, #0x20]
003a08d0: mov r2, r6
003a08d4: add r7, sp, #0x2c
003a08d8: mov r0, r5
003a08dc: bl #0x513ce4
003a08e0: mov r0, r7
003a08e4: mov r1, #0x10
003a08e8: str r7, [sp, #0x3c]
003a08ec: str r7, [sp, #0x40]
003a08f0: bl #0x31167c
003a08f4: ldr r3, [sp, #0x3c]
003a08f8: mov r6, #0
003a08fc: add r8, sp, #0x14
003a0900: strb r6, [r3]
003a0904: ldr r2, [sp, #0x3c]
003a0908: mov r0, r8
003a090c: ldr r1, [sp, #0x40]
003a0910: str r8, [sp, #0x24]
003a0914: str r8, [sp, #0x28]
003a0918: bl #0x3116e8
003a091c: mov r1, r6
003a0920: mov r0, #0x38
003a0924: bl #0x310570
003a0928: ldr sl, [pc, #0x158]
003a092c: mov r6, r0
003a0930: add r2, sp, #0xc
003a0934: add sl, pc, sl
003a0938: mov r1, sl
003a093c: str fp, [r0], #8
003a0940: bl #0x3140ec
003a0944: ldr r3, [pc, #0x140]
003a0948: add r2, sb, #0x378
003a094c: mov r0, r6
003a0950: ldr r3, [r4, r3]
003a0954: rsb r2, r5, r2
003a0958: str r2, [r6, #4]
003a095c: add r3, r3, #8
003a0960: str r3, [r0], #0x20
003a0964: str r0, [r6, #0x30]
003a0968: str r0, [r6, #0x34]
003a096c: ldr r1, [sp, #0x28]
003a0970: ldr r2, [sp, #0x24]
003a0974: bl #0x3116e8
003a0978: mov r0, r5
003a097c: mov r1, sl
003a0980: mov r2, r6
003a0984: bl #0x513ce4
003a0988: ldr r0, [sp, #0x28]
003a098c: cmp r0, r8
003a0990: beq #0x3a09b0
003a0994: cmp r0, #0
003a0998: beq #0x3a09b0
003a099c: ldr r1, [sp, #0x14]
003a09a0: rsb r1, r0, r1
003a09a4: cmp r1, #0x80
003a09a8: bhi #0x3a0a68
003a09ac: bl #0x708f00
003a09b0: ldr r0, [sp, #0x40]
003a09b4: cmp r0, r7
003a09b8: beq #0x3a09d8
003a09bc: cmp r0, #0
003a09c0: beq #0x3a09d8
003a09c4: ldr r1, [sp, #0x2c]
003a09c8: rsb r1, r0, r1
003a09cc: cmp r1, #0x80
003a09d0: bhi #0x3a0a60
003a09d4: bl #0x708f00
003a09d8: mov r1, #0
003a09dc: mov r0, #0x24
003a09e0: bl #0x310570
003a09e4: ldr r2, [sp]
003a09e8: ldr r7, [pc, #0xa0]
003a09ec: mov r6, r0
003a09f0: ldr r3, [r4, r2]
003a09f4: add r7, pc, r7
003a09f8: mov r1, r7
003a09fc: add r3, r3, #8
003a0a00: str r3, [r0], #8
003a0a04: add r2, sp, #8
003a0a08: bl #0x3140ec
003a0a0c: ldr r3, [pc, #0x80]
003a0a10: add sb, sb, #0x390
003a0a14: rsb sb, r5, sb
003a0a18: ldr r3, [r4, r3]
003a0a1c: mov r2, r6
003a0a20: str sb, [r6, #4]
003a0a24: add r3, r3, #8
003a0a28: str r3, [r6]
003a0a2c: mov r3, #0
003a0a30: strb r3, [r6, #0x20]
003a0a34: mov r0, r5
003a0a38: mov r1, r7
003a0a3c: bl #0x513ce4
003a0a40: ldr r2, [sp, #4]
003a0a44: ldr r3, [r4, r2]
003a0a48: ldr r2, [sp, #0x44]
003a0a4c: ldr r3, [r3]
003a0a50: cmp r2, r3
003a0a54: bne #0x3a0a70
003a0a58: add sp, sp, #0x4c
003a0a5c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a0a60: bl #0x310440
003a0a64: b #0x3a09d8
003a0a68: bl #0x310440
003a0a6c: b #0x3a09b0
003a0a70: bl #0x30e310
003a0a74: subseq r4, pc, r0, asr #4
003a0a78: andeq r4, r0, ip, lsr #1
003a0a7c: andeq r2, r0, r0, lsr r3
003a0a80: ldrsbeq r2, [r2], #-0x20
003a0a84: muleq r0, r0, r5
003a0a88: subseq r2, r2, ip, lsr #12
003a0a8c: muleq r0, r4, r4
003a0a90: subseq r2, r2, ip, ror r5
003a0a94: andeq r3, r0, ip, asr #28

_ZN9ContainerD1Ev 0x3a0430 324
003a0430: push {r4, r5, r6, r7, r8, sb, sl, lr}
003a0434: ldr r5, [pc, #0x128]
003a0438: ldr r3, [pc, #0x128]
003a043c: ldr r7, [pc, #0x128]
003a0440: ldr r8, [pc, #0x128]
003a0444: add r5, pc, r5
003a0448: ldr r3, [r5, r3]
003a044c: ldr r1, [r5, r7]
003a0450: ldr r2, [r5, r8]
003a0454: mov r4, r0
003a0458: add r1, r1, #8
003a045c: add r0, r3, #0x100
003a0460: add ip, r3, #8
003a0464: add r2, r2, #8
003a0468: add r3, r3, #0xf4
003a046c: add sl, r4, #0x540
003a0470: str ip, [r4]
003a0474: str r3, [r4, #4]
003a0478: str r0, [r4, #0x24]
003a047c: str r1, [r4, #0x678]
003a0480: str r2, [r4, #0x548]
003a0484: str r1, [r4, #0x6c8]
003a0488: str r1, [r4, #0x6a0]
003a048c: add r6, sl, #8
003a0490: ldr r3, [r6, #0x11c]
003a0494: cmp r3, #0
003a0498: beq #0x3a04c0
003a049c: add sl, sl, #0x114
003a04a0: mov r0, sl
003a04a4: ldr r1, [r6, #0x110]
003a04a8: bl #0x370fd0
003a04ac: mov r3, #0
003a04b0: str r3, [r6, #0x11c]
003a04b4: str sl, [r6, #0x118]
003a04b8: str sl, [r6, #0x114]
003a04bc: str r3, [r6, #0x110]
003a04c0: ldr r2, [r5, r8]
003a04c4: ldr r3, [r5, r7]
003a04c8: ldr r1, [r4, #0x4bc]
003a04cc: add r2, r2, #8
003a04d0: add r3, r3, #8
003a04d4: cmp r1, #0
003a04d8: str r3, [r4, #0x4d0]
003a04dc: str r2, [r4, #0x3a0]
003a04e0: str r3, [r4, #0x520]
003a04e4: str r3, [r4, #0x4f8]
003a04e8: add r5, r4, #0x3a0
003a04ec: beq #0x3a0514
003a04f0: add r6, r5, #0x10c
003a04f4: mov r0, r6
003a04f8: ldr r1, [r5, #0x110]
003a04fc: bl #0x370fd0
003a0500: mov r3, #0
003a0504: str r6, [r4, #0x4b4]
003a0508: str r3, [r5, #0x110]
003a050c: str r6, [r4, #0x4b8]
003a0510: str r3, [r4, #0x4bc]
003a0514: add r3, r4, #0x378
003a0518: ldr r0, [r3, #0x14]
003a051c: cmp r0, r3
003a0520: beq #0x3a0540
003a0524: cmp r0, #0
003a0528: beq #0x3a0540
003a052c: ldr r1, [r4, #0x378]
003a0530: rsb r1, r0, r1
003a0534: cmp r1, #0x80
003a0538: bhi #0x3a0550
003a053c: bl #0x708f00
003a0540: mov r0, r4
003a0544: bl #0x38d378
003a0548: mov r0, r4
003a054c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003a0550: bl #0x310440
003a0554: mov r0, r4
003a0558: bl #0x38d378
003a055c: mov r0, r4
003a0560: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003a0564: subseq r4, pc, ip, asr #12
003a0568: ldrdeq r2, r3, [r0], -r4
003a056c: andeq r1, r0, r8, lsr #1
003a0570: andeq r4, r0, r4, asr #7

_ZN21DestructibleContainerD2Ev 0x3a147c 64
003a147c: ldr r2, [pc, #0x30]
003a1480: ldr r3, [pc, #0x30]
003a1484: push {r4, lr}
003a1488: add r2, pc, r2
003a148c: ldr r3, [r2, r3]
003a1490: mov r4, r0
003a1494: add r2, r3, #0x100
003a1498: add r1, r3, #8
003a149c: add r3, r3, #0xf4
003a14a0: stm r0, {r1, r3}
003a14a4: str r2, [r0, #0x24]
003a14a8: bl #0x3a0598
003a14ac: mov r0, r4
003a14b0: pop {r4, pc}
003a14b4: subseq r3, pc, r8, lsl #12
003a14b8: strheq r1, [r0], -r8

_ZN9Container11DeserializeEP11IStreamBase 0x39f6f0 312
0039f6f0: push {r4, r5, lr}
0039f6f4: ldr r3, [pc, #0x11c]
0039f6f8: ldr r2, [pc, #0x11c]
0039f6fc: mov r5, r0
0039f700: add r3, pc, r3
0039f704: ldr r0, [r3, r2]
0039f708: sub sp, sp, #0x14
0039f70c: mov r4, r1
0039f710: bl #0x31f594
0039f714: ldrb r3, [r5, #0x390]
0039f718: cmp r3, #0
0039f71c: bne #0x39f740
0039f720: ldrb r3, [r0, #0xf3]
0039f724: cmp r3, #0
0039f728: beq #0x39f734
0039f72c: add sp, sp, #0x14
0039f730: pop {r4, r5, pc}
0039f734: ldrb r3, [r0, #0xf4]
0039f738: cmp r3, #0
0039f73c: bne #0x39f72c
0039f740: mov r1, r4
0039f744: mov r0, r5
0039f748: bl #0x38b91c
0039f74c: mov r0, r4
0039f750: add r1, sp, #0xf
0039f754: bl #0x39f638
0039f758: ldr r3, [r5, #0x2d8]
0039f75c: cmp r3, #0
0039f760: beq #0x39f72c
0039f764: mov r0, r5
0039f768: ldrb r1, [sp, #0xf]
0039f76c: bl #0x39f3cc
0039f770: ldr r3, [r5, #0x394]
0039f774: ldr r4, [r5, #0x2d8]
0039f778: cmp r3, #4
0039f77c: beq #0x39f7bc
0039f780: cmp r3, #2
0039f784: bne #0x39f72c
0039f788: cmp r4, #0
0039f78c: beq #0x39f72c
0039f790: ldr ip, [r4, #0x38]
0039f794: ldr r1, [pc, #0x84]
0039f798: mov r3, #0
0039f79c: mov r2, r3
0039f7a0: mov r0, ip
0039f7a4: add r1, pc, r1
0039f7a8: ldr ip, [ip]
0039f7ac: str r3, [sp]
0039f7b0: mov lr, pc
0039f7b4: ldr pc, [ip, #0x20]
0039f7b8: b #0x39f72c
0039f7bc: ldr r3, [r5]
0039f7c0: mov r0, r5
0039f7c4: mov lr, pc
0039f7c8: ldr pc, [r3, #0xdc]
0039f7cc: subs r1, r0, #0
0039f7d0: beq #0x39f808
0039f7d4: cmp r4, #0
0039f7d8: beq #0x39f72c
0039f7dc: ldr ip, [r4, #0x38]
0039f7e0: ldr r1, [pc, #0x3c]
0039f7e4: mov r3, #0
0039f7e8: mov r2, r3
0039f7ec: mov r0, ip
0039f7f0: add r1, pc, r1
0039f7f4: ldr ip, [ip]
0039f7f8: str r3, [sp]
0039f7fc: mov lr, pc
0039f800: ldr pc, [ip, #0x20]
0039f804: b #0x39f72c
0039f808: mov r0, r5
0039f80c: mov r2, r1
0039f810: bl #0x394bf8
0039f814: b #0x39f7d4

_ZN7Structs21DestructibleContainer8finalizeEv 0x4da134 40
004da134: push {r4, lr}
004da138: mov r4, r0
004da13c: ldr r0, [r0, #0x30]
004da140: cmp r0, #0
004da144: beq #0x4da158
004da148: bl #0x310440
004da14c: mov r3, #0
004da150: str r3, [r4, #0x2c]
004da154: str r3, [r4, #0x30]
004da158: pop {r4, pc}

_ZNK9Container9GetScriptEv 0x39f32c 8
0039f32c: mov r0, #0
0039f330: bx lr

_ZN21DestructibleContainer9DoEffectsEv 0x3a0d68 4
003a0d68: bx lr

_ZN9Container18NetStructContainerD1Ev 0x3a0324 124
003a0324: push {r4, r5, r6, lr}
003a0328: ldr r3, [pc, #0x64]
003a032c: ldr r2, [pc, #0x64]
003a0330: ldr r1, [pc, #0x64]
003a0334: add r3, pc, r3
003a0338: mov r4, r0
003a033c: ldr r1, [r3, r1]
003a0340: ldr r0, [r0, #0x11c]
003a0344: ldr r2, [r3, r2]
003a0348: add r1, r1, #8
003a034c: cmp r0, #0
003a0350: add r2, r2, #8
003a0354: str r2, [r4, #0x130]
003a0358: str r1, [r4]
003a035c: str r2, [r4, #0x180]
003a0360: str r2, [r4, #0x158]
003a0364: beq #0x3a038c
003a0368: add r5, r4, #0x10c
003a036c: mov r0, r5
003a0370: ldr r1, [r4, #0x110]
003a0374: bl #0x370fd0
003a0378: mov r3, #0
003a037c: str r5, [r4, #0x118]
003a0380: str r3, [r4, #0x11c]
003a0384: str r5, [r4, #0x114]
003a0388: str r3, [r4, #0x110]
003a038c: mov r0, r4
003a0390: pop {r4, r5, r6, pc}
003a0394: subseq r4, pc, ip, asr r7
003a0398: andeq r1, r0, r8, lsr #1
003a039c: andeq r4, r0, r4, asr #7

_ZN9ContainerC2EN10ObjectBase6GO_IDSE 0x3a0788 172
003a0788: push {r4, r5, r6, r7, r8, lr}
003a078c: ldr r5, [pc, #0x98]
003a0790: mov r4, r0
003a0794: bl #0x38c398
003a0798: ldr r3, [pc, #0x90]
003a079c: add r5, pc, r5
003a07a0: add r2, r4, #0x378
003a07a4: ldr r3, [r5, r3]
003a07a8: mov r0, r2
003a07ac: str r2, [r4, #0x388]
003a07b0: add ip, r3, #8
003a07b4: add r1, r3, #0x100
003a07b8: add r3, r3, #0xf4
003a07bc: str ip, [r4]
003a07c0: str r2, [r4, #0x38c]
003a07c4: str r3, [r4, #4]
003a07c8: str r1, [r4, #0x24]
003a07cc: mov r1, #0x10
003a07d0: bl #0x31167c
003a07d4: ldr r3, [r4, #0x388]
003a07d8: mov r6, #0
003a07dc: mov r7, #2
003a07e0: add r8, r4, #0x3a0
003a07e4: add r5, r4, #0x540
003a07e8: strb r6, [r3]
003a07ec: add r5, r5, #8
003a07f0: strb r6, [r4, #0x390]
003a07f4: str r7, [r4, #0x394]
003a07f8: str r6, [r4, #0x398]
003a07fc: mov r0, r8
003a0800: bl #0x39ff58
003a0804: mov r0, r5
003a0808: bl #0x39ff58
003a080c: mov r3, #1
003a0810: strb r3, [r4, #0x28]
003a0814: strb r6, [r4, #0x84]
003a0818: str r8, [r4, #0x100]
003a081c: str r5, [r4, #0x104]
003a0820: strb r7, [r4, #0xf8]
003a0824: mov r0, r4
003a0828: pop {r4, r5, r6, r7, r8, pc}
003a082c: ldrsheq r4, [pc], #-0x24
003a0830: ldrdeq r2, r3, [r0], -r4

_ZNK21DestructibleContainer18GetInteractionTypeEP10GameObject 0x3a0d60 8
003a0d60: mov r0, #8
003a0d64: bx lr

_ZN9Container10__CallbackEPN6glitch5scene19ITimelineControllerEPv 0x39f44c 204
0039f44c: push {r4, lr}
0039f450: ldr r3, [r1, #0x394]
0039f454: sub sp, sp, #8
0039f458: mov r4, r1
0039f45c: cmp r3, #1
0039f460: beq #0x39f4d4
0039f464: cmp r3, #3
0039f468: beq #0x39f498
0039f46c: ldr r3, [r0]
0039f470: mov lr, pc
0039f474: ldr pc, [r3, #0x44]
0039f478: cmp r0, #0
0039f47c: ldreq r3, [r4, #0x2d8]
0039f480: ldreq r3, [r3, #8]
0039f484: ldreq r2, [r3, #0x11c]
0039f488: biceq r2, r2, #0x200
0039f48c: streq r2, [r3, #0x11c]
0039f490: add sp, sp, #8
0039f494: pop {r4, pc}
0039f498: mov r0, r1
0039f49c: mov r1, #4
0039f4a0: bl #0x39f3cc
0039f4a4: ldr r2, [r4, #0x2d8]
0039f4a8: ldr r1, [pc, #0x60]
0039f4ac: mov r3, #0
0039f4b0: ldr ip, [r2, #0x38]
0039f4b4: add r1, pc, r1
0039f4b8: mov r2, r3
0039f4bc: mov r0, ip
0039f4c0: ldr ip, [ip]
0039f4c4: str r3, [sp]
0039f4c8: mov lr, pc
0039f4cc: ldr pc, [ip, #0x20]
0039f4d0: b #0x39f490
0039f4d4: mov r0, r1
0039f4d8: mov r1, #2
0039f4dc: bl #0x39f3cc
0039f4e0: ldr r2, [r4, #0x2d8]
0039f4e4: ldr r1, [pc, #0x28]
0039f4e8: mov r3, #0
0039f4ec: ldr ip, [r2, #0x38]
0039f4f0: add r1, pc, r1
0039f4f4: mov r2, r3
0039f4f8: mov r0, ip
0039f4fc: ldr ip, [ip]
0039f500: str r3, [sp]
0039f504: mov lr, pc
0039f508: ldr pc, [ip, #0x20]
0039f50c: b #0x39f490
0039f510: subseq r3, r2, r4, lsl r6
0039f514: subseq r2, r2, r0, asr #27

_ZN9Container25PopulateOutgoingNetStructEb 0x3a0110 532
003a0110: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a0114: ldr r4, [pc, #0x1e8]
003a0118: ldr r6, [pc, #0x1e8]
003a011c: ldr r3, [r0, #0x394]
003a0120: sub sp, sp, #0x7c
003a0124: add r4, pc, r4
003a0128: ldr r2, [sp, #0x70]
003a012c: ldr ip, [r4, r6]
003a0130: sub r3, r3, #3
003a0134: cmp r3, #1
003a0138: movhi r3, #0
003a013c: movls r3, #1
003a0140: cmp r2, r3
003a0144: mov r8, #0
003a0148: mov r2, #0
003a014c: mov r5, r0
003a0150: add ip, ip, #8
003a0154: mvn r0, #0
003a0158: mov lr, #1
003a015c: mov sb, #0
003a0160: strd r8, sb, [sp, #0x58]
003a0164: str lr, [sp, #0x54]
003a0168: str r0, [sp, #0x64]
003a016c: strb r2, [sp, #0x6c]
003a0170: str ip, [sp, #0x50]
003a0174: mov r8, r1
003a0178: str r0, [sp, #0x60]
003a017c: str r2, [sp, #0x68]
003a0180: addeq r7, sp, #0x50
003a0184: beq #0x3a0198
003a0188: add r7, sp, #0x50
003a018c: mov r0, r7
003a0190: str r3, [sp, #0x70]
003a0194: bl #0x814f84
003a0198: ldr r2, [pc, #0x16c]
003a019c: add r1, r7, #0x20
003a01a0: ldr r3, [r5, #0x4d0]
003a01a4: ldr r2, [r4, r2]
003a01a8: add r0, r5, #0x4d0
003a01ac: ldr r7, [pc, #0x15c]
003a01b0: add r2, r2, #8
003a01b4: str r2, [sp, #0x50]
003a01b8: mov lr, pc
003a01bc: ldr pc, [r3, #0x1c]
003a01c0: ldr r3, [pc, #0x14c]
003a01c4: ldr r2, [r4, r7]
003a01c8: ldr r1, [r5, #0x398]
003a01cc: ldr r3, [r4, r3]
003a01d0: add r2, r2, #8
003a01d4: str r2, [sp, #0x50]
003a01d8: ldr r0, [r3, #0x38]
003a01dc: bl #0x3402f4
003a01e0: ldr r2, [pc, #0x130]
003a01e4: ldr r1, [sp, #0x48]
003a01e8: mov r3, r0
003a01ec: ldr r2, [r4, r2]
003a01f0: mvn r0, #0
003a01f4: cmp r3, r1
003a01f8: mov sl, #0
003a01fc: mov r1, #0
003a0200: add r2, r2, #8
003a0204: mov ip, #0x10
003a0208: mov fp, #0
003a020c: strd sl, fp, [sp, #0x30]
003a0210: str ip, [sp, #0x2c]
003a0214: str r0, [sp, #0x3c]
003a0218: strb r1, [sp, #0x44]
003a021c: str r2, [sp, #0x28]
003a0220: str r0, [sp, #0x38]
003a0224: str r1, [sp, #0x40]
003a0228: addeq sl, sp, #0x28
003a022c: beq #0x3a0240
003a0230: add sl, sp, #0x28
003a0234: mov r0, sl
003a0238: str r3, [sp, #0x48]
003a023c: bl #0x814f84
003a0240: ldr r2, [pc, #0xd4]
003a0244: add r0, r5, #0x4f0
003a0248: ldr r3, [r5, #0x4f8]
003a024c: ldr r2, [r4, r2]
003a0250: add r0, r0, #8
003a0254: add r1, sl, #0x20
003a0258: add r2, r2, #8
003a025c: str r2, [sp, #0x28]
003a0260: mov lr, pc
003a0264: ldr pc, [r3, #0x1c]
003a0268: cmp r8, #0
003a026c: beq #0x3a02fc
003a0270: ldr ip, [r4, r7]
003a0274: ldr r3, [r5, #0xfc]
003a0278: ldr r0, [r4, r6]
003a027c: ldr r2, [sp, #0x20]
003a0280: add ip, ip, #8
003a0284: mvn r1, #0
003a0288: cmp r3, r2
003a028c: mov r6, #0
003a0290: mov r2, #0
003a0294: add r0, r0, #8
003a0298: str ip, [sp, #0x28]
003a029c: mov r7, #0
003a02a0: mov ip, #0x20
003a02a4: strd r6, r7, [sp, #8]
003a02a8: str ip, [sp, #4]
003a02ac: str r1, [sp, #0x14]
003a02b0: strb r2, [sp, #0x1c]
003a02b4: str r0, [sp]
003a02b8: str r1, [sp, #0x10]
003a02bc: str r2, [sp, #0x18]
003a02c0: moveq r6, sp
003a02c4: beq #0x3a02d8
003a02c8: mov r0, sp
003a02cc: mov r6, sp
003a02d0: str r3, [sp, #0x20]
003a02d4: bl #0x814f84
003a02d8: ldr r2, [pc, #0x40]
003a02dc: ldr r3, [r5, #0x520]
003a02e0: add r0, r5, #0x520
003a02e4: ldr r2, [r4, r2]
003a02e8: add r1, r6, #0x20
003a02ec: add r2, r2, #8
003a02f0: str r2, [sp]
003a02f4: mov lr, pc
003a02f8: ldr pc, [r3, #0x1c]
003a02fc: add sp, sp, #0x7c
003a0300: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a0304: subseq r4, pc, ip, ror #18
003a0308: andeq r4, r0, r8, rrx
003a030c: strdeq r3, r4, [r0], -r4
003a0310: andeq r1, r0, r8, lsr #1
003a0314: strdeq r3, r4, [r0], -r4
003a0318: andeq r2, r0, r4, lsl #19
003a031c: andeq r3, r0, ip, lsr r5
003a0320: strdeq r3, r4, [r0], -r0