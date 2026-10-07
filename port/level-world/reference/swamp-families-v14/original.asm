
_ZThn36_N21DestructibleContainerD1Ev 0x3a1410 8
003a1410: sub r0, r0, #0x24
003a1414: b #0x3a1418

_ZNK5Dummy9IsZonableEv 0x34013c 8
0034013c: mov r0, #0
00340140: bx lr

_ZThn36_N4Door11DeserializeEP11IStreamBase 0x3e7a64 8
003e7a64: sub r0, r0, #0x24
003e7a68: b #0x3e7a6c

_ZN7Structs13TriggerObjectD0Ev 0x4d9d04 28
004d9d04: push {r4, lr}
004d9d08: mov r4, r0
004d9d0c: bl #0x4d9cc4
004d9d10: mov r0, r4
004d9d14: bl #0x310440
004d9d18: mov r0, r4
004d9d1c: pop {r4, pc}

_ZN7Structs4DoorD2Ev 0x4da0f4 64
004da0f4: push {r4, lr}
004da0f8: ldr r3, [pc, #0x2c]
004da0fc: ldr r2, [pc, #0x2c]
004da100: mov r4, r0
004da104: add r3, pc, r3
004da108: ldr r0, [r0, #8]
004da10c: ldr r2, [r3, r2]
004da110: cmp r0, #0
004da114: add r2, r2, #8
004da118: str r2, [r4]
004da11c: beq #0x4da124
004da120: bl #0x310440
004da124: mov r0, r4
004da128: pop {r4, pc}
004da12c: subeq sl, fp, ip, lsl #19
004da130: muleq r0, ip, sb

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

_ZThn36_N14CheckpointZoneD0Ev 0x395bac 8
00395bac: sub r0, r0, #0x24
00395bb0: b #0x395bb4

_ZN4Door8DisabledEv 0x3e7b94 68
003e7b94: push {r4, lr}
003e7b98: mov r4, r0
003e7b9c: bl #0x38ba04
003e7ba0: ldr r2, [r4, #0x3a8]
003e7ba4: ldr r3, [pc, #0x24]
003e7ba8: cmp r2, #1
003e7bac: add r3, pc, r3
003e7bb0: beq #0x3e7bcc
003e7bb4: ldr r0, [pc, #0x18]
003e7bb8: add r1, r4, #0x1c8
003e7bbc: mov r2, #0
003e7bc0: ldr r0, [r3, r0]
003e7bc4: pop {r4, lr}
003e7bc8: b #0x5252ec
003e7bcc: pop {r4, pc}
003e7bd0: subseq ip, sl, r4, ror #29
003e7bd4: andeq r1, r0, r4, lsl #4

_ZNK13TriggerObject13IsInteractiveEP10GameObject 0x399490 24
00399490: push {r4, lr}
00399494: mov r4, r0
00399498: bl #0x3987ac
0039949c: cmp r0, #0
003994a0: ldrbne r0, [r4, #0x784]
003994a4: pop {r4, pc}

_ZThn36_N4Door9SerializeEP11IStreamBase 0x3e7b6c 8
003e7b6c: sub r0, r0, #0x24
003e7b70: b #0x3e7b74

_ZN11TriggerZone10HideMarkerEv 0x39b2f4 96
0039b2f4: push {r4, r5, r6, r7, r8, lr}
0039b2f8: ldr r3, [r0, #0x7b0]
0039b2fc: ldr r4, [pc, #0x48]
0039b300: mov r5, r0
0039b304: cmp r3, #0
0039b308: add r4, pc, r4
0039b30c: beq #0x39b348
0039b310: mov r6, #0
0039b314: mov r0, r3
0039b318: str r6, [r3, #0x28]
0039b31c: mov r1, #1
0039b320: mov r7, r5
0039b324: bl #0x492aa0
0039b328: ldr r0, [r7, #0x7b0]!
0039b32c: mov r1, r6
0039b330: bl #0x492ef0
0039b334: ldr r3, [pc, #0x14]
0039b338: mov r1, r7
0039b33c: ldr r0, [r4, r3]
0039b340: bl #0x494978
0039b344: str r6, [r5, #0x7b0]
0039b348: pop {r4, r5, r6, r7, r8, pc}
0039b34c: subseq sb, pc, r8, lsl #15
0039b350: andeq r1, r0, r8, lsl #22

_ZN10SpawnPointD1Ev 0x3ea2cc 76
003ea2cc: ldr r2, [pc, #0x3c]
003ea2d0: ldr r3, [pc, #0x3c]
003ea2d4: push {r4, lr}
003ea2d8: add r2, pc, r2
003ea2dc: ldr r3, [r2, r3]
003ea2e0: mov r4, r0
003ea2e4: add r0, r0, #0x378
003ea2e8: add r2, r3, #0xe4
003ea2ec: add r1, r3, #8
003ea2f0: add r3, r3, #0xd8
003ea2f4: stm r4, {r1, r3}
003ea2f8: str r2, [r4, #0x24]
003ea2fc: bl #0x3139ac
003ea300: mov r0, r4
003ea304: bl #0x38d378
003ea308: mov r0, r4
003ea30c: pop {r4, pc}
003ea310: ldrheq sl, [sl], #-0x78
003ea314: muleq r0, ip, r0

_ZNK5Dummy13IsInteractiveEP10GameObject 0x34014c 8
0034014c: mov r0, #0
00340150: bx lr

_ZN4Door10__CallbackEPN6glitch5scene19ITimelineControllerEPv 0x3e8568 240
003e8568: push {r4, r5, r6, r7, r8, lr}
003e856c: ldr r4, [pc, #0xd4]
003e8570: ldr r8, [pc, #0xd4]
003e8574: ldr r2, [pc, #0xd4]
003e8578: add r4, pc, r4
003e857c: ldr r3, [r4, r8]
003e8580: ldr r7, [r4, r2]
003e8584: sub sp, sp, #0x20
003e8588: ldr r3, [r3]
003e858c: mov r0, r7
003e8590: mov r5, r1
003e8594: str r3, [sp, #0x1c]
003e8598: bl #0x337888
003e859c: ldr r1, [pc, #0xb0]
003e85a0: add r6, sp, #4
003e85a4: mov r2, sp
003e85a8: add r1, pc, r1
003e85ac: mov r0, r6
003e85b0: bl #0x3140ec
003e85b4: mov r0, r7
003e85b8: mov r1, r6
003e85bc: bl #0x337a88
003e85c0: ldr r0, [sp, #0x18]
003e85c4: cmp r0, r6
003e85c8: beq #0x3e85e8
003e85cc: cmp r0, #0
003e85d0: beq #0x3e85e8
003e85d4: ldr r1, [sp, #4]
003e85d8: rsb r1, r0, r1
003e85dc: cmp r1, #0x80
003e85e0: bhi #0x3e863c
003e85e4: bl #0x708f00
003e85e8: ldr r3, [r5, #0x3a8]
003e85ec: mov r1, #0
003e85f0: mov r2, #1
003e85f4: cmp r3, #2
003e85f8: strb r2, [r5, #0x85]
003e85fc: strb r1, [r5, #0x3ac]
003e8600: beq #0x3e8630
003e8604: cmp r3, #3
003e8608: bne #0x3e8614
003e860c: mov r0, r5
003e8610: bl #0x3e7788
003e8614: ldr r3, [r4, r8]
003e8618: ldr r2, [sp, #0x1c]
003e861c: ldr r3, [r3]
003e8620: cmp r2, r3
003e8624: bne #0x3e8644
003e8628: add sp, sp, #0x20
003e862c: pop {r4, r5, r6, r7, r8, pc}
003e8630: mov r0, r5
003e8634: bl #0x3e7598
003e8638: b #0x3e8614
003e863c: bl #0x310440
003e8640: b #0x3e85e8
003e8644: bl #0x30e310
003e8648: subseq ip, sl, r8, lsl r5
003e864c: andeq r4, r0, ip, lsr #1
003e8650: andeq r0, r0, r4, lsl #17
003e8654: subeq sp, sp, r0, asr #22

_ZNK4Door10IsAnimatedEv 0x3e74a0 8
003e74a0: mov r0, #1
003e74a4: bx lr

_ZN13TriggerObjectD1Ev 0x39993c 148
0039993c: push {r4, r5, r6, lr}
00399940: ldr r2, [pc, #0x80]
00399944: ldr r3, [pc, #0x80]
00399948: ldr r5, [r0, #0x788]
0039994c: add r2, pc, r2
00399950: ldr r3, [r2, r3]
00399954: cmp r5, #0
00399958: mov r4, r0
0039995c: add r2, r3, #0xf4
00399960: add r1, r3, #8
00399964: add r3, r3, #0xe8
00399968: stm r0, {r1, r3}
0039996c: str r2, [r0, #0x24]
00399970: beq #0x39998c
00399974: mov r0, r5
00399978: bl #0x478eac
0039997c: mov r0, r5
00399980: bl #0x310440
00399984: mov r3, #0
00399988: str r3, [r4, #0x788]
0039998c: add r0, r4, #0x760
00399990: add r0, r0, #0xc
00399994: bl #0x318254
00399998: add r0, r4, #0x750
0039999c: bl #0x318254
003999a0: add r0, r4, #0x730
003999a4: add r0, r0, #4
003999a8: bl #0x318254
003999ac: add r0, r4, #0x710
003999b0: add r0, r0, #8
003999b4: bl #0x318254
003999b8: mov r0, r4
003999bc: bl #0x399214
003999c0: mov r0, r4
003999c4: pop {r4, r5, r6, pc}
003999c8: subseq fp, pc, r4, asr #2
003999cc: andeq r2, r0, r4, ror #22

_ZThn4_N5Decor17DeclarePropertiesEv 0x3899c8 8
003899c8: sub r0, r0, #4
003899cc: b #0x3899d0

_ZThn36_N10SpawnPointD1Ev 0x3ea2c4 8
003ea2c4: sub r0, r0, #0x24
003ea2c8: b #0x3ea2cc

_ZThn36_N5DecorD1Ev 0x388420 8
00388420: sub r0, r0, #0x24
00388424: b #0x388428

_ZN5Decor17DeclarePropertiesEv 0x3899d0 156
003899d0: push {r4, r5, r6, r7, lr}
003899d4: sub sp, sp, #0xc
003899d8: mov r7, r0
003899dc: bl #0x38cee8
003899e0: mov r1, #0
003899e4: mov r0, #0x24
003899e8: bl #0x310570
003899ec: ldr r5, [pc, #0x68]
003899f0: ldr r3, [pc, #0x68]
003899f4: ldr r6, [pc, #0x68]
003899f8: add r5, pc, r5
003899fc: ldr r3, [r5, r3]
00389a00: add r6, pc, r6
00389a04: mov r4, r0
00389a08: add r3, r3, #8
00389a0c: mov r1, r6
00389a10: add r2, sp, #4
00389a14: str r3, [r0], #8
00389a18: bl #0x3140ec
00389a1c: ldr r3, [pc, #0x44]
00389a20: add r2, r7, #0x374
00389a24: add r0, r7, #4
00389a28: ldr r3, [r5, r3]
00389a2c: add r2, r2, #2
00389a30: rsb r2, r0, r2
00389a34: add r3, r3, #8
00389a38: str r3, [r4]
00389a3c: mov r3, #1
00389a40: str r2, [r4, #4]
00389a44: strb r3, [r4, #0x20]
00389a48: mov r1, r6
00389a4c: mov r2, r4
00389a50: bl #0x513ce4
00389a54: add sp, sp, #0xc
00389a58: pop {r4, r5, r6, r7, pc}
00389a5c: mlseq r0, r8, r0, fp
00389a60: andeq r2, r0, r0, lsr r3
00389a64: ldrsheq r8, [r3], #-0x80
00389a68: andeq r3, r0, ip, asr #28

_ZN14CheckpointZoneC1EN10ObjectBase6GO_IDSE 0x39594c 104
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

_ZN7Structs19GetMemberIDByStringINS_4DoorEEEiPKc 0x4ae53c 88
004ae53c: ldr r3, [pc, #0x48]
004ae540: ldr r2, [pc, #0x48]
004ae544: push {r4, r5, r6, lr}
004ae548: add r3, pc, r3
004ae54c: mov r6, r0
004ae550: ldr r5, [r3, r2]
004ae554: mov r4, #0
004ae558: ldr r1, [r5, #0x14]
004ae55c: mov r0, r6
004ae560: bl #0x30e31c
004ae564: cmp r0, #0
004ae568: beq #0x4ae584
004ae56c: add r4, r4, #1
004ae570: cmp r4, #4
004ae574: add r5, r5, #0x18
004ae578: bne #0x4ae558
004ae57c: mvn r0, #0
004ae580: pop {r4, r5, r6, pc}
004ae584: mov r0, r4
004ae588: pop {r4, r5, r6, pc}
004ae58c: subeq r6, lr, r8, asr #10
004ae590: andeq r2, r0, ip, lsr #1

_ZN10SpawnPoint11PlaceObjectEP10GameObject 0x3ea22c 96
003ea22c: push {r4, r5, r6, lr}
003ea230: mov r5, r1
003ea234: mov r4, r0
003ea238: add r1, r0, #0x160
003ea23c: mov r2, #1
003ea240: mov r0, r5
003ea244: bl #0x393db4
003ea248: add r1, r4, #0x16c
003ea24c: mov r0, r5
003ea250: bl #0x3938a0
003ea254: ldr r1, [r4, #0x390]
003ea258: ldr r3, [pc, #0x24]
003ea25c: cmn r1, #1
003ea260: add r3, pc, r3
003ea264: beq #0x3ea280
003ea268: ldr r0, [pc, #0x18]
003ea26c: ldr r2, [r4, #0x64]
003ea270: ldr r0, [r3, r0]
003ea274: mov r3, #0
003ea278: pop {r4, r5, r6, lr}
003ea27c: b #0x4605c0
003ea280: pop {r4, r5, r6, pc}
003ea284: subseq sl, sl, r0, lsr r8
003ea288: andeq r1, r0, r0, lsr #20

_ZThn4_N4Door17DeclarePropertiesEv 0x3e8988 8
003e8988: sub r0, r0, #4
003e898c: b #0x3e8990

_ZN11TriggerZoneD0Ev 0x39b7e8 28
0039b7e8: push {r4, lr}
0039b7ec: mov r4, r0
0039b7f0: bl #0x39b754
0039b7f4: mov r0, r4
0039b7f8: bl #0x310440
0039b7fc: mov r0, r4
0039b800: pop {r4, pc}

_ZNK10SpawnPoint11IsUpdatableEv 0x3ea20c 8
003ea20c: mov r0, #0
003ea210: bx lr

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

_ZNK13TriggerObject11IsUpdatableEv 0x399318 8
00399318: mov r0, #1
0039931c: bx lr

_Z14GetNewInstanceI14CheckpointZoneEP10ObjectBasev 0x340f2c 36
00340f2c: push {r4, lr}
00340f30: mov r1, #0
00340f34: mov r0, #0x3a0
00340f38: bl #0x310570
00340f3c: mov r1, #0xc
00340f40: mov r4, r0
00340f44: bl #0x39594c
00340f48: mov r0, r4
00340f4c: pop {r4, pc}

_ZN12SoundEmitterD0Ev 0x395474 28
00395474: push {r4, lr}
00395478: mov r4, r0
0039547c: bl #0x3953e0
00395480: mov r0, r4
00395484: bl #0x310440
00395488: mov r0, r4
0039548c: pop {r4, pc}

_ZN15QuestMoveInZone17OnCollisionBeginsEP10GameObject 0x396120 304
00396120: push {r4, r5, r6, r7, r8, lr}
00396124: ldr r4, [pc, #0xfc]
00396128: subs r5, r1, #0
0039612c: sub sp, sp, #0x28
00396130: mov r6, r0
00396134: add r4, pc, r4
00396138: beq #0x3961d4
0039613c: ldr r3, [r5]
00396140: mov r0, r5
00396144: mov lr, pc
00396148: ldr pc, [r3, #0x24]
0039614c: cmp r0, #0
00396150: bne #0x39615c
00396154: add sp, sp, #0x28
00396158: pop {r4, r5, r6, r7, r8, pc}
0039615c: ldr r3, [pc, #0xc8]
00396160: ldr r8, [r4, r3]
00396164: mov r0, r8
00396168: bl #0x31f594
0039616c: subs r7, r0, #0
00396170: beq #0x396154
00396174: ldr r1, [pc, #0xb4]
00396178: ldr r2, [pc, #0xb4]
0039617c: ldr r0, [r8, #0x2c]
00396180: add r1, pc, r1
00396184: add r2, pc, r2
00396188: ldr r8, [r6, #0x64]
0039618c: bl #0x4c4bdc
00396190: ldr r2, [pc, #0xa0]
00396194: mov r3, #0
00396198: str r0, [sp, #0x10]
0039619c: ldr r2, [r4, r2]
003961a0: mvn ip, #0
003961a4: mov r0, r7
003961a8: add r2, r2, #8
003961ac: add r1, sp, #0xc
003961b0: str r5, [sp, #0x14]
003961b4: str r8, [sp, #0x18]
003961b8: strb r3, [sp, #0x1d]
003961bc: str ip, [sp, #0x20]
003961c0: str r2, [sp, #0xc]
003961c4: str r6, [sp, #0x24]
003961c8: strb r3, [sp, #0x1c]
003961cc: bl #0x339090
003961d0: b #0x396154
003961d4: ldr r3, [pc, #0x60]
003961d8: ldr r3, [r4, r3]
003961dc: ldr r3, [r3]
003961e0: cmp r3, #2
003961e4: streq r5, [r5]
003961e8: beq #0x39613c
003961ec: cmp r3, #1
003961f0: bne #0x39613c
003961f4: ldr r0, [pc, #0x44]
003961f8: ldr r1, [pc, #0x44]
003961fc: ldr r2, [pc, #0x44]
00396200: ldr r0, [r4, r0]
00396204: ldr r3, [pc, #0x40]
00396208: mov ip, #0x26
0039620c: add r1, pc, r1
00396210: add r2, pc, r2
00396214: add r3, pc, r3
00396218: add r0, r0, #0xa8
0039621c: str ip, [sp]
00396220: bl #0x30e004
00396224: b #0x39613c
00396228: subseq lr, pc, ip, asr sb
0039622c: strdeq r3, r4, [r0], -r4
00396230: subseq ip, r2, r8, ror #15
00396234: ldrsheq ip, [r2], #-0x7c
00396238: andeq r2, r0, r0, asr #31
0039623c: andeq r3, r0, r0, asr #19
00396240: andeq r1, r0, r0, asr #19
00396244: subseq r8, r2, ip, asr #3
00396248: subseq ip, r2, r8, lsl #3
0039624c: subseq ip, r2, r4, lsl #14

_ZN11TriggerZoneD1Ev 0x39b754 140
0039b754: ldr r2, [pc, #0x7c]
0039b758: ldr r3, [pc, #0x7c]
0039b75c: push {r4, lr}
0039b760: add r2, pc, r2
0039b764: ldr r3, [r2, r3]
0039b768: mov r4, r0
0039b76c: add r2, r3, #0xf4
0039b770: add r1, r3, #8
0039b774: add r3, r3, #0xe8
0039b778: stm r0, {r1, r3}
0039b77c: str r2, [r0, #0x24]
0039b780: bl #0x39b2f4
0039b784: add r0, r4, #0x7b0
0039b788: add r0, r0, #0xc
0039b78c: bl #0x3139ac
0039b790: add r0, r4, #0x790
0039b794: add r0, r0, #4
0039b798: bl #0x3139ac
0039b79c: add r0, r4, #0x770
0039b7a0: add r0, r0, #8
0039b7a4: bl #0x3139ac
0039b7a8: add r0, r4, #0x750
0039b7ac: add r0, r0, #0xc
0039b7b0: bl #0x3139ac
0039b7b4: add r0, r4, #0x740
0039b7b8: bl #0x3139ac
0039b7bc: add r0, r4, #0x720
0039b7c0: add r0, r0, #4
0039b7c4: bl #0x3139ac
0039b7c8: mov r0, r4
0039b7cc: bl #0x399214
0039b7d0: mov r0, r4
0039b7d4: pop {r4, pc}
0039b7d8: subseq sb, pc, r0, lsr r3
0039b7dc: andeq r3, r0, r8, lsl ip

_ZN11TriggerZone10ShowMarkerEv 0x39b354 260
0039b354: push {r4, r5, lr}
0039b358: ldr r1, [r0, #0x7ac]
0039b35c: ldr r5, [pc, #0xd8]
0039b360: sub sp, sp, #0xc
0039b364: cmn r1, #1
0039b368: mov r4, r0
0039b36c: add r5, pc, r5
0039b370: beq #0x39b3fc
0039b374: ldr r3, [r0, #0x7b0]
0039b378: cmp r3, #0
0039b37c: beq #0x39b3a4
0039b380: ldr r3, [pc, #0xb8]
0039b384: ldr r3, [r5, r3]
0039b388: ldr r3, [r3]
0039b38c: cmp r3, #2
0039b390: moveq r3, #0
0039b394: streq r3, [r3]
0039b398: beq #0x39b3a4
0039b39c: cmp r3, #1
0039b3a0: beq #0x39b404
0039b3a4: ldr r3, [pc, #0x98]
0039b3a8: mov r2, #0
0039b3ac: ldr r0, [r5, r3]
0039b3b0: bl #0x495430
0039b3b4: cmp r0, #0
0039b3b8: str r0, [r4, #0x7b0]
0039b3bc: beq #0x39b3fc
0039b3c0: str r4, [r0, #0x28]
0039b3c4: mov r1, #1
0039b3c8: bl #0x492aa0
0039b3cc: mov r1, #1
0039b3d0: ldr r0, [r4, #0x7b0]
0039b3d4: bl #0x492ef0
0039b3d8: ldr r0, [r4, #0x7b0]
0039b3dc: bl #0x49267c
0039b3e0: ldr r3, [r0]
0039b3e4: mov lr, pc
0039b3e8: ldr pc, [r3, #0x44]
0039b3ec: mov r1, #1
0039b3f0: ldr r3, [r0]
0039b3f4: mov lr, pc
0039b3f8: ldr pc, [r3, #0x40]
0039b3fc: add sp, sp, #0xc
0039b400: pop {r4, r5, pc}
0039b404: ldr r0, [pc, #0x3c]
0039b408: ldr r1, [pc, #0x3c]
0039b40c: ldr r2, [pc, #0x3c]
0039b410: ldr r0, [r5, r0]
0039b414: ldr r3, [pc, #0x38]
0039b418: add r1, pc, r1
0039b41c: movw ip, #0x12a
0039b420: add r0, r0, #0xa8
0039b424: add r2, pc, r2
0039b428: add r3, pc, r3
0039b42c: str ip, [sp]
0039b430: bl #0x30e004
0039b434: ldr r1, [r4, #0x7ac]
0039b438: b #0x39b3a4
0039b43c: subseq sb, pc, r4, lsr #14
0039b440: andeq r3, r0, r0, asr #19
0039b444: andeq r1, r0, r8, lsl #22
0039b448: andeq r1, r0, r0, asr #19
0039b44c: subseq r2, r2, r0, asr #31
0039b450: subseq r7, r2, ip, lsl r8
0039b454: subseq r7, r2, r8, lsr r8

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

_ZN4Door5CloseEb 0x3e763c 320
003e763c: push {r4, r5, r6, lr}
003e7640: ldrb r3, [r0, #0x3ac]
003e7644: ldr r4, [pc, #0x120]
003e7648: sub sp, sp, #0x20
003e764c: cmp r3, #0
003e7650: mov r5, r0
003e7654: add r4, pc, r4
003e7658: bne #0x3e7668
003e765c: ldr r3, [r0, #0x3a8]
003e7660: cmp r3, #1
003e7664: beq #0x3e7670
003e7668: add sp, sp, #0x20
003e766c: pop {r4, r5, r6, pc}
003e7670: ldr r6, [r0, #0x2d8]
003e7674: cmp r6, #0
003e7678: beq #0x3e76b0
003e767c: cmp r1, #0
003e7680: bne #0x3e76b0
003e7684: ldr r3, [r0]
003e7688: mov lr, pc
003e768c: ldr pc, [r3, #0xc4]
003e7690: cmp r0, #0
003e7694: beq #0x3e772c
003e7698: ldrb r3, [r5, #0x2ee]
003e769c: cmp r3, #0
003e76a0: beq #0x3e772c
003e76a4: ldrb r3, [r5, #0x2f0]
003e76a8: cmp r3, #0
003e76ac: bne #0x3e772c
003e76b0: mov r0, r5
003e76b4: mov r1, #0
003e76b8: bl #0x3e7598
003e76bc: ldr r3, [r5, #0x3a0]
003e76c0: cmn r3, #1
003e76c4: beq #0x3e7668
003e76c8: ldr r2, [pc, #0xa0]
003e76cc: ldr r1, [pc, #0xa0]
003e76d0: ldr lr, [r5, #0x168]
003e76d4: ldr r2, [r4, r2]
003e76d8: ldr r1, [r4, r1]
003e76dc: ldr r6, [r5, #0x160]
003e76e0: ldr r2, [r2]
003e76e4: ldr r0, [r1]
003e76e8: mov r1, #0x18
003e76ec: mla r3, r1, r3, r2
003e76f0: ldr r4, [r5, #0x164]
003e76f4: mov ip, #0xbf000000
003e76f8: ldr r1, [r3, #0xc]
003e76fc: add ip, ip, #0x800000
003e7700: str lr, [sp, #0x1c]
003e7704: add r2, sp, #0x14
003e7708: mov lr, #1
003e770c: mov r3, #0
003e7710: str r6, [sp, #0x14]
003e7714: str r4, [sp, #0x18]
003e7718: str lr, [sp]
003e771c: str ip, [sp, #8]
003e7720: str ip, [sp, #4]
003e7724: bl #0x36b5d8
003e7728: b #0x3e7668
003e772c: mov r2, #2
003e7730: str r2, [r5, #0x3a8]
003e7734: mov r3, #0
003e7738: mov r2, #1
003e773c: strb r2, [r5, #0x3ac]
003e7740: strb r3, [r5, #0x85]
003e7744: ldr ip, [r6, #0x38]
003e7748: ldr r1, [pc, #0x28]
003e774c: mov r2, r3
003e7750: mov r0, ip
003e7754: add r1, pc, r1
003e7758: ldr ip, [ip]
003e775c: str r3, [sp]
003e7760: mov lr, pc
003e7764: ldr pc, [ip, #0x20]
003e7768: b #0x3e76bc
003e776c: subseq sp, sl, ip, lsr r4
003e7770: andeq r1, r0, r8, lsl #28
003e7774: andeq r0, r0, r4, lsr #27
003e7778: subeq lr, sp, ip, ror #18

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

_ZN15QuestMoveInZoneC2EN10ObjectBase6GO_IDSE 0x396378 72
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

_ZThn4_N21DestructibleContainer17DeclarePropertiesEv 0x3a1268 8
003a1268: sub r0, r0, #4
003a126c: b #0x3a1270

_ZN14CheckpointZoneD2Ev 0x395bd0 116
00395bd0: push {r4, r5, r6, lr}
00395bd4: ldr r2, [pc, #0x60]
00395bd8: ldr r3, [pc, #0x60]
00395bdc: ldr r1, [r0, #0x398]
00395be0: add r2, pc, r2
00395be4: ldr r3, [r2, r3]
00395be8: cmp r1, #0
00395bec: mov r4, r0
00395bf0: add r2, r3, #0xf4
00395bf4: add r1, r3, #8
00395bf8: add r3, r3, #0xe8
00395bfc: stm r0, {r1, r3}
00395c00: str r2, [r0, #0x24]
00395c04: beq #0x395c2c
00395c08: add r5, r0, #0x388
00395c0c: mov r0, r5
00395c10: ldr r1, [r4, #0x38c]
00395c14: bl #0x395af8
00395c18: mov r3, #0
00395c1c: str r5, [r4, #0x394]
00395c20: str r3, [r4, #0x398]
00395c24: str r5, [r4, #0x390]
00395c28: str r3, [r4, #0x38c]
00395c2c: mov r0, r4
00395c30: bl #0x397bc4
00395c34: mov r0, r4
00395c38: pop {r4, r5, r6, pc}
00395c3c: ldrheq lr, [pc], #-0xe0
00395c40: andeq r2, r0, r4, ror #28

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

_ZN13TriggerObject6UpdateEv 0x3994a8 208
003994a8: push {r4, r5, lr}
003994ac: mov r4, r0
003994b0: sub sp, sp, #0xc
003994b4: ldr r5, [r0, #0x3b8]
003994b8: bl #0x398838
003994bc: ldrb r3, [r4, #0x784]
003994c0: cmp r3, #0
003994c4: beq #0x39951c
003994c8: cmp r5, #0
003994cc: ble #0x3994e0
003994d0: mov r0, r4
003994d4: bl #0x3987ac
003994d8: cmp r0, #0
003994dc: bne #0x39952c
003994e0: ldr r0, [r4, #0x2d8]
003994e4: cmp r0, #0
003994e8: beq #0x3994f0
003994ec: bl #0x38ba74
003994f0: ldr r1, [r4, #0x2e4]
003994f4: cmp r1, #0
003994f8: beq #0x399514
003994fc: ldr r3, [r4]
00399500: mov r0, r4
00399504: mov lr, pc
00399508: ldr pc, [r3, #0x98]
0039950c: mov r3, #0
00399510: str r3, [r4, #0x2e4]
00399514: add sp, sp, #0xc
00399518: pop {r4, r5, pc}
0039951c: mov r0, r4
00399520: bl #0x3993bc
00399524: ldr r0, [r4, #0x2d8]
00399528: b #0x3994e4
0039952c: ldr r0, [r4, #0x2d8]
00399530: cmp r0, #0
00399534: beq #0x399568
00399538: ldr ip, [r0, #0x38]
0039953c: ldr r1, [pc, #0x30]
00399540: mov r2, #0
00399544: mov r0, ip
00399548: mov r3, r2
0039954c: ldr ip, [ip]
00399550: add r1, pc, r1
00399554: str r2, [sp]
00399558: mov r2, #1
0039955c: mov lr, pc
00399560: ldr pc, [ip, #0x20]
00399564: ldr r0, [r4, #0x2d8]
00399568: mov r3, #0
0039956c: strb r3, [r4, #0x373]
00399570: b #0x3994e4
00399574: subseq r8, r2, r0, ror #26

_ZN4Door4OpenEb 0x3e786c 316
003e786c: push {r4, r5, r6, lr}
003e7870: ldrb r3, [r0, #0x3ac]
003e7874: ldr r5, [pc, #0x11c]
003e7878: sub sp, sp, #0x20
003e787c: cmp r3, #0
003e7880: mov r4, r0
003e7884: add r5, pc, r5
003e7888: bne #0x3e7924
003e788c: ldr r3, [r0, #0x3a8]
003e7890: cmp r3, #0
003e7894: bne #0x3e7924
003e7898: ldr r6, [r0, #0x2d8]
003e789c: cmp r6, #0
003e78a0: beq #0x3e78ac
003e78a4: cmp r1, #0
003e78a8: beq #0x3e792c
003e78ac: mov r0, r4
003e78b0: mov r1, #0
003e78b4: bl #0x3e7788
003e78b8: ldr r3, [r4, #0x3a0]
003e78bc: cmn r3, #1
003e78c0: beq #0x3e7924
003e78c4: ldr r2, [pc, #0xd0]
003e78c8: ldr r1, [pc, #0xd0]
003e78cc: ldr lr, [r4, #0x168]
003e78d0: ldr r2, [r5, r2]
003e78d4: ldr r1, [r5, r1]
003e78d8: ldr r6, [r4, #0x160]
003e78dc: ldr r2, [r2]
003e78e0: ldr r0, [r1]
003e78e4: mov r1, #0x18
003e78e8: mla r3, r1, r3, r2
003e78ec: ldr r5, [r4, #0x164]
003e78f0: mov ip, #0xbf000000
003e78f4: ldr r1, [r3, #0x10]
003e78f8: add ip, ip, #0x800000
003e78fc: str lr, [sp, #0x1c]
003e7900: add r2, sp, #0x14
003e7904: mov lr, #1
003e7908: mov r3, #0
003e790c: str r6, [sp, #0x14]
003e7910: str r5, [sp, #0x18]
003e7914: str lr, [sp]
003e7918: str ip, [sp, #8]
003e791c: str ip, [sp, #4]
003e7920: bl #0x36b5d8
003e7924: add sp, sp, #0x20
003e7928: pop {r4, r5, r6, pc}
003e792c: ldr r3, [r0]
003e7930: mov lr, pc
003e7934: ldr pc, [r3, #0xc4]
003e7938: cmp r0, #0
003e793c: beq #0x3e7958
003e7940: ldrb r3, [r4, #0x2ee]
003e7944: cmp r3, #0
003e7948: beq #0x3e7958
003e794c: ldrb r3, [r4, #0x2f0]
003e7950: cmp r3, #0
003e7954: beq #0x3e78ac
003e7958: mov r2, #3
003e795c: str r2, [r4, #0x3a8]
003e7960: mov r3, #0
003e7964: mov r2, #1
003e7968: strb r2, [r4, #0x3ac]
003e796c: strb r3, [r4, #0x85]
003e7970: ldr ip, [r6, #0x38]
003e7974: ldr r1, [pc, #0x28]
003e7978: mov r2, r3
003e797c: mov r0, ip
003e7980: add r1, pc, r1
003e7984: ldr ip, [ip]
003e7988: str r3, [sp]
003e798c: mov lr, pc
003e7990: ldr pc, [ip, #0x20]
003e7994: b #0x3e78b8
003e7998: subseq sp, sl, ip, lsl #4
003e799c: andeq r1, r0, r8, lsl #28
003e79a0: andeq r0, r0, r4, lsr #27
003e79a4: subeq sb, sp, r8, asr #25

_ZNK13TriggerObject18GetInteractionTypeEP10GameObject 0x399330 56
00399330: ldr r0, [r0, #0x730]
00399334: ldr r3, [pc, #0x24]
00399338: cmn r0, #1
0039933c: add r3, pc, r3
00399340: bxeq lr
00399344: ldr r2, [pc, #0x18]
00399348: ldr r3, [r3, r2]
0039934c: mov r2, #0x18
00399350: ldr r3, [r3]
00399354: mla r0, r2, r0, r3
00399358: ldr r0, [r0, #4]
0039935c: bx lr
00399360: subseq fp, pc, r4, asr r7
00399364: andeq r0, r0, ip, lsl lr

_ZN4Door9InitFinalEv 0x3e7c1c 92
003e7c1c: push {r4, lr}
003e7c20: mov r4, r0
003e7c24: bl #0x38bd64
003e7c28: ldr r3, [r4, #0x274]
003e7c2c: cmp r0, r3
003e7c30: blt #0x3e7c38
003e7c34: pop {r4, pc}
003e7c38: mov r0, r4
003e7c3c: bl #0x38cd48
003e7c40: mov r0, r4
003e7c44: bl #0x38ab60
003e7c48: cmp r0, #0
003e7c4c: beq #0x3e7c34
003e7c50: ldrb r1, [r4, #0x3a4]
003e7c54: cmp r1, #0
003e7c58: bne #0x3e7c68
003e7c5c: mov r0, r4
003e7c60: pop {r4, lr}
003e7c64: b #0x3e7598
003e7c68: mov r0, r4
003e7c6c: mov r1, #0
003e7c70: pop {r4, lr}
003e7c74: b #0x3e7788

_ZN4Door8InitPostEv 0x3e7da8 596
003e7da8: push {r4, r5, r6, r7, r8, sl, lr}
003e7dac: sub sp, sp, #0x24
003e7db0: mov r4, r0
003e7db4: bl #0x38bd64
003e7db8: ldr r3, [r4, #0x274]
003e7dbc: ldr r5, [pc, #0x210]
003e7dc0: cmp r0, r3
003e7dc4: add r5, pc, r5
003e7dc8: bge #0x3e7f44
003e7dcc: ldr r3, [pc, #0x204]
003e7dd0: ldr r8, [r4, #0x39c]
003e7dd4: ldr r3, [r5, r3]
003e7dd8: ldr r7, [r3]
003e7ddc: cmp r7, #0
003e7de0: beq #0x3e7f4c
003e7de4: ldr r3, [pc, #0x1f0]
003e7de8: mov r6, #0
003e7dec: ldr r3, [r5, r3]
003e7df0: ldr sl, [r3]
003e7df4: b #0x3e7e04
003e7df8: add r6, r6, #1
003e7dfc: cmp r6, r7
003e7e00: beq #0x3e7f4c
003e7e04: ldr r1, [sl, r6, lsl #2]
003e7e08: mov r0, r8
003e7e0c: bl #0x30e31c
003e7e10: cmp r0, #0
003e7e14: bne #0x3e7df8
003e7e18: cmn r6, #1
003e7e1c: str r6, [r4, #0x3a0]
003e7e20: beq #0x3e7e74
003e7e24: ldr r3, [pc, #0x1b4]
003e7e28: mov r2, #0x18
003e7e2c: ldr r3, [r5, r3]
003e7e30: ldr r3, [r3]
003e7e34: mla r6, r2, r6, r3
003e7e38: ldr r3, [r6, #0x14]
003e7e3c: cmn r3, #1
003e7e40: beq #0x3e7e74
003e7e44: ldr r2, [pc, #0x198]
003e7e48: mov r1, #0xc
003e7e4c: ldr r2, [r5, r2]
003e7e50: ldr r2, [r2]
003e7e54: mla r3, r1, r3, r2
003e7e58: ldr r6, [r3, #8]
003e7e5c: mov r0, r6
003e7e60: bl #0x30de54
003e7e64: mov r1, r6
003e7e68: add r2, r6, r0
003e7e6c: add r0, r4, #0x290
003e7e70: bl #0x3109e0
003e7e74: mov r0, r4
003e7e78: bl #0x39771c
003e7e7c: mov r0, r4
003e7e80: bl #0x38ab60
003e7e84: subs r1, r0, #0
003e7e88: beq #0x3e7f34
003e7e8c: ldr r6, [r4, #0x2d8]
003e7e90: cmp r6, #0
003e7e94: beq #0x3e7ecc
003e7e98: ldr r3, [pc, #0x148]
003e7e9c: ldr r2, [r6, #0x38]
003e7ea0: ldr r1, [r5, r3]
003e7ea4: ldr r3, [pc, #0x140]
003e7ea8: mov r0, r2
003e7eac: ldr ip, [r2]
003e7eb0: ldr r3, [r5, r3]
003e7eb4: str r4, [sp]
003e7eb8: mov r2, r4
003e7ebc: mov lr, pc
003e7ec0: ldr pc, [ip, #0x2c]
003e7ec4: mov r0, r6
003e7ec8: bl #0x470a54
003e7ecc: ldrb r3, [r4, #0x3a5]
003e7ed0: cmp r3, #0
003e7ed4: bne #0x3e7f58
003e7ed8: ldr r3, [r4, #0x3a0]
003e7edc: cmn r3, #1
003e7ee0: beq #0x3e7f44
003e7ee4: ldr r2, [pc, #0x104]
003e7ee8: ldr r6, [r5, r2]
003e7eec: ldr r0, [r6]
003e7ef0: cmp r0, #0
003e7ef4: beq #0x3e7f44
003e7ef8: ldr r2, [pc, #0xe0]
003e7efc: mov r7, #0x18
003e7f00: ldr r5, [r5, r2]
003e7f04: ldr r2, [r5]
003e7f08: mla r3, r7, r3, r2
003e7f0c: ldr r1, [r3, #0x10]
003e7f10: bl #0x3699fc
003e7f14: ldr r2, [r4, #0x3a0]
003e7f18: ldr r3, [r5]
003e7f1c: ldr r0, [r6]
003e7f20: mla r7, r7, r2, r3
003e7f24: ldr r1, [r7, #0xc]
003e7f28: add sp, sp, #0x24
003e7f2c: pop {r4, r5, r6, r7, r8, sl, lr}
003e7f30: b #0x3699fc
003e7f34: mov r0, r4
003e7f38: ldr r3, [r4]
003e7f3c: mov lr, pc
003e7f40: ldr pc, [r3, #0x40]
003e7f44: add sp, sp, #0x24
003e7f48: pop {r4, r5, r6, r7, r8, sl, pc}
003e7f4c: mvn r3, #0
003e7f50: str r3, [r4, #0x3a0]
003e7f54: b #0x3e7e74
003e7f58: ldr r3, [pc, #0x94]
003e7f5c: mov r1, #0
003e7f60: mov r0, #0x28
003e7f64: ldr r3, [r5, r3]
003e7f68: mov r6, r1
003e7f6c: ldr r8, [r3, #0x44]
003e7f70: bl #0x310570
003e7f74: mov ip, #1
003e7f78: mov lr, #2
003e7f7c: mov r1, r8
003e7f80: mov r3, ip
003e7f84: mov r2, r4
003e7f88: str lr, [sp, #0x10]
003e7f8c: movw lr, #0xffff
003e7f90: mov r7, r0
003e7f94: str lr, [sp, #0x14]
003e7f98: str r6, [sp]
003e7f9c: str r6, [sp, #4]
003e7fa0: str r6, [sp, #8]
003e7fa4: str r6, [sp, #0xc]
003e7fa8: str ip, [sp, #0x18]
003e7fac: bl #0x46f2f0
003e7fb0: ldr r3, [pc, #0x40]
003e7fb4: mov r1, r7
003e7fb8: mov r2, r6
003e7fbc: ldr r3, [r5, r3]
003e7fc0: mov r0, r4
003e7fc4: add r3, r3, #8
003e7fc8: str r3, [r7]
003e7fcc: bl #0x394bf8
003e7fd0: b #0x3e7ed8
003e7fd4: subseq ip, sl, ip, asr #25
003e7fd8: strdeq r1, r2, [r0], -r4
003e7fdc: andeq r4, r0, r8, lsl #9
003e7fe0: andeq r1, r0, r8, lsl #28
003e7fe4: andeq r1, r0, r8, lsr #25
003e7fe8: andeq r0, r0, r8, asr #20
003e7fec: andeq r3, r0, r4, lsl #21
003e7ff0: andeq r0, r0, r4, lsr #27
003e7ff4: strdeq r3, r4, [r0], -r4
003e7ff8: andeq r2, r0, r8, lsl r4

_ZN11TriggerZoneC2EN10ObjectBase6GO_IDSE 0x39b990 256
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

_ZN11IStreamBase7writeAsIN4Door10DoorStatesEEEvRKT_ 0x3e7abc 176
003e7abc: str lr, [sp, #-4]!
003e7ac0: mov r3, #0
003e7ac4: sub sp, sp, #0xc
003e7ac8: ldr ip, [r0]
003e7acc: mov r2, #4
003e7ad0: mov lr, pc
003e7ad4: ldr pc, [ip, #0x1c]
003e7ad8: ldr r3, [pc, #0x74]
003e7adc: cmp r0, #4
003e7ae0: add r3, pc, r3
003e7ae4: beq #0x3e7b14
003e7ae8: ldr r2, [pc, #0x68]
003e7aec: ldr r2, [r3, r2]
003e7af0: ldr r2, [r2]
003e7af4: cmp r2, #2
003e7af8: moveq r3, #0
003e7afc: streq r3, [r3]
003e7b00: beq #0x3e7b0c
003e7b04: cmp r2, #1
003e7b08: beq #0x3e7b20
003e7b0c: add sp, sp, #0xc
003e7b10: ldm sp!, {pc}
003e7b14: cmp r1, #0
003e7b18: beq #0x3e7b0c
003e7b1c: b #0x3e7ae8
003e7b20: ldr r0, [pc, #0x34]
003e7b24: ldr r1, [pc, #0x34]
003e7b28: ldr r2, [pc, #0x34]
003e7b2c: ldr r0, [r3, r0]
003e7b30: ldr r3, [pc, #0x30]
003e7b34: mov ip, #0x4d
003e7b38: add r1, pc, r1
003e7b3c: add r2, pc, r2
003e7b40: add r3, pc, r3
003e7b44: add r0, r0, #0xa8
003e7b48: str ip, [sp]
003e7b4c: bl #0x30e004
003e7b50: b #0x3e7b0c
003e7b54: ldrheq ip, [sl], #-0xf0
003e7b58: andeq r3, r0, r0, asr #19
003e7b5c: andeq r1, r0, r0, asr #19
003e7b60: subeq r6, sp, r0, lsr #17
003e7b64: subeq r6, sp, r4, ror #18
003e7b68: ldrdeq r6, r7, [sp], #-0x98

_ZN13TriggerObjectD2Ev 0x3999f4 148
003999f4: push {r4, r5, r6, lr}
003999f8: ldr r2, [pc, #0x80]
003999fc: ldr r3, [pc, #0x80]
00399a00: ldr r5, [r0, #0x788]
00399a04: add r2, pc, r2
00399a08: ldr r3, [r2, r3]
00399a0c: cmp r5, #0
00399a10: mov r4, r0
00399a14: add r2, r3, #0xf4
00399a18: add r1, r3, #8
00399a1c: add r3, r3, #0xe8
00399a20: stm r0, {r1, r3}
00399a24: str r2, [r0, #0x24]
00399a28: beq #0x399a44
00399a2c: mov r0, r5
00399a30: bl #0x478eac
00399a34: mov r0, r5
00399a38: bl #0x310440
00399a3c: mov r3, #0
00399a40: str r3, [r4, #0x788]
00399a44: add r0, r4, #0x760
00399a48: add r0, r0, #0xc
00399a4c: bl #0x318254
00399a50: add r0, r4, #0x750
00399a54: bl #0x318254
00399a58: add r0, r4, #0x730
00399a5c: add r0, r0, #4
00399a60: bl #0x318254
00399a64: add r0, r4, #0x710
00399a68: add r0, r0, #8
00399a6c: bl #0x318254
00399a70: mov r0, r4
00399a74: bl #0x399214
00399a78: mov r0, r4
00399a7c: pop {r4, r5, r6, pc}
00399a80: subseq fp, pc, ip, lsl #1
00399a84: andeq r2, r0, r4, ror #22

_Z14GetNewInstanceI11TriggerZoneEP10ObjectBasev 0x340ec4 36
00340ec4: push {r4, lr}
00340ec8: mov r1, #0
00340ecc: movw r0, #0x7d8
00340ed0: bl #0x310570
00340ed4: mov r1, #0x14
00340ed8: mov r4, r0
00340edc: bl #0x39b890
00340ee0: mov r0, r4
00340ee4: pop {r4, pc}

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

_ZN4Door13NetStructDoorC1Ev 0x3e80d4 416
003e80d4: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e80d8: ldr r7, [pc, #0x184]
003e80dc: mov r4, r0
003e80e0: bl #0x8138f4
003e80e4: ldr r3, [pc, #0x17c]
003e80e8: ldr r5, [pc, #0x17c]
003e80ec: add r7, pc, r7
003e80f0: ldr r3, [r7, r3]
003e80f4: ldrb r2, [r4, #0x14d]
003e80f8: ldr r0, [r7, r5]
003e80fc: add r3, r3, #8
003e8100: mov sb, #0
003e8104: mov r8, #0
003e8108: mov ip, #0x138
003e810c: strd r8, sb, [r4, ip]
003e8110: cmp r2, #0
003e8114: mvn r1, #0
003e8118: mov r2, #0
003e811c: add r0, r0, #8
003e8120: str r3, [r4]
003e8124: mov r3, #1
003e8128: str r3, [r4, #0x134]
003e812c: str r1, [r4, #0x144]
003e8130: str r0, [r4, #0x130]
003e8134: str r1, [r4, #0x140]
003e8138: str r2, [r4, #0x148]
003e813c: strb r2, [r4, #0x14c]
003e8140: addeq sb, r4, #0x130
003e8144: beq #0x3e8158
003e8148: add sb, r4, #0x130
003e814c: strb r2, [r4, #0x14d]
003e8150: mov r0, sb
003e8154: bl #0x814f84
003e8158: ldr r8, [pc, #0x110]
003e815c: ldrb r3, [r4, #0x16d]
003e8160: ldr r1, [r7, r5]
003e8164: ldr r0, [r7, r8]
003e8168: mov ip, #0x158
003e816c: mov sl, #0
003e8170: add r0, r0, #8
003e8174: mov fp, #0
003e8178: strd sl, fp, [r4, ip]
003e817c: cmp r3, #0
003e8180: mvn r2, #0
003e8184: mov r3, #0
003e8188: add r1, r1, #8
003e818c: str r0, [r4, #0x130]
003e8190: mov r0, #1
003e8194: str r0, [r4, #0x154]
003e8198: str r2, [r4, #0x164]
003e819c: str r1, [r4, #0x150]
003e81a0: str r2, [r4, #0x160]
003e81a4: str r3, [r4, #0x168]
003e81a8: strb r3, [r4, #0x16c]
003e81ac: addeq r6, r4, #0x150
003e81b0: beq #0x3e81c4
003e81b4: add r6, r4, #0x150
003e81b8: strb r3, [r4, #0x16d]
003e81bc: mov r0, r6
003e81c0: bl #0x814f84
003e81c4: ldr r0, [r7, r8]
003e81c8: ldrb r3, [r4, #0x18d]
003e81cc: ldr r1, [r7, r5]
003e81d0: add r0, r0, #8
003e81d4: mov ip, #0x178
003e81d8: mov sl, #0
003e81dc: mov fp, #0
003e81e0: strd sl, fp, [r4, ip]
003e81e4: cmp r3, #0
003e81e8: mvn r2, #0
003e81ec: mov r3, #0
003e81f0: add r1, r1, #8
003e81f4: str r0, [r4, #0x150]
003e81f8: mov r0, #1
003e81fc: str r0, [r4, #0x174]
003e8200: str r2, [r4, #0x184]
003e8204: str r1, [r4, #0x170]
003e8208: str r2, [r4, #0x180]
003e820c: str r3, [r4, #0x188]
003e8210: strb r3, [r4, #0x18c]
003e8214: addeq r5, r4, #0x170
003e8218: beq #0x3e822c
003e821c: add r5, r4, #0x170
003e8220: strb r3, [r4, #0x18d]
003e8224: mov r0, r5
003e8228: bl #0x814f84
003e822c: ldr r3, [r7, r8]
003e8230: mov r1, sb
003e8234: mov r0, r4
003e8238: add r3, r3, #8
003e823c: str r3, [r4, #0x170]
003e8240: bl #0x81324c
003e8244: mov r0, r4
003e8248: mov r1, r6
003e824c: bl #0x81324c
003e8250: mov r0, r4
003e8254: mov r1, r5
003e8258: bl #0x81324c
003e825c: mov r0, r4
003e8260: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e8264: subseq ip, sl, r4, lsr #19
003e8268: andeq r2, r0, r0, lsr #32
003e826c: andeq r3, r0, r8, lsl r0
003e8270: andeq r0, r0, r8, asr #21

_ZThn36_N11TriggerZoneD1Ev 0x39b74c 8
0039b74c: sub r0, r0, #0x24
0039b750: b #0x39b754

_ZN4Door26InterpretIncomingNetStructEb 0x3e7828 68
003e7828: cmp r1, #0
003e782c: bxeq lr
003e7830: ldrb r1, [r0, #0x68d]
003e7834: cmp r1, #0
003e7838: beq #0x3e7858
003e783c: ldr r1, [r0, #0x3a8]
003e7840: cmp r1, #1
003e7844: cmpne r1, #3
003e7848: movne r1, #0
003e784c: moveq r1, #1
003e7850: bxeq lr
003e7854: b #0x3e7788
003e7858: ldr r3, [r0, #0x3a8]
003e785c: cmp r3, #1
003e7860: cmpne r3, #3
003e7864: bxne lr
003e7868: b #0x3e7598

_ZN7Structs13TriggerObject4readEP11IStreamBase 0x4fd448 464
004fd448: push {r4, r5, r6, lr}
004fd44c: mov r4, r0
004fd450: sub sp, sp, #8
004fd454: mov r0, r1
004fd458: mov r5, r1
004fd45c: add r1, r4, #4
004fd460: bl #0x459090
004fd464: mov r3, #1
004fd468: cmp r3, #0
004fd46c: str r3, [sp, #4]
004fd470: bne #0x4fd4b4
004fd474: add r3, r4, #5
004fd478: add r2, r4, #6
004fd47c: ldrb r0, [r2, #1]
004fd480: ldrb r1, [r3, #-1]
004fd484: cmp r3, r2
004fd488: eor r1, r0, r1
004fd48c: strb r1, [r3, #-1]
004fd490: ldrb r0, [r2, #1]
004fd494: eor r1, r1, r0
004fd498: strb r1, [r2, #1]
004fd49c: ldrb r0, [r3, #-1]
004fd4a0: sub r2, r2, #1
004fd4a4: eor r1, r1, r0
004fd4a8: strb r1, [r3, #-1]
004fd4ac: add r3, r3, #1
004fd4b0: blo #0x4fd47c
004fd4b4: mov r0, r5
004fd4b8: add r1, r4, #8
004fd4bc: bl #0x3df1a0
004fd4c0: mov r3, #1
004fd4c4: cmp r3, #0
004fd4c8: str r3, [sp, #4]
004fd4cc: bne #0x4fd510
004fd4d0: add r3, r4, #9
004fd4d4: add r2, r4, #0xa
004fd4d8: ldrb r0, [r2, #1]
004fd4dc: ldrb r1, [r3, #-1]
004fd4e0: cmp r3, r2
004fd4e4: eor r1, r0, r1
004fd4e8: strb r1, [r3, #-1]
004fd4ec: ldrb r0, [r2, #1]
004fd4f0: eor r1, r1, r0
004fd4f4: strb r1, [r2, #1]
004fd4f8: ldrb r0, [r3, #-1]
004fd4fc: sub r2, r2, #1
004fd500: eor r1, r1, r0
004fd504: strb r1, [r3, #-1]
004fd508: add r3, r3, #1
004fd50c: blo #0x4fd4d8
004fd510: ldr r0, [r4, #0xc]
004fd514: cmp r0, #0
004fd518: beq #0x4fd520
004fd51c: bl #0x310440
004fd520: ldr r0, [r4, #8]
004fd524: mov r1, #1
004fd528: mov r6, #0
004fd52c: add r0, r0, r1
004fd530: bl #0x31056c
004fd534: ldr r2, [r4, #8]
004fd538: mov r1, r0
004fd53c: str r0, [r4, #0xc]
004fd540: mov r3, r6
004fd544: mov r0, r5
004fd548: bl #0x317454
004fd54c: ldr r3, [r4, #8]
004fd550: ldr r2, [r4, #0xc]
004fd554: mov r0, r5
004fd558: add r1, r4, #0x10
004fd55c: strb r6, [r2, r3]
004fd560: bl #0x459090
004fd564: mov r3, #1
004fd568: cmp r3, r6
004fd56c: str r3, [sp, #4]
004fd570: bne #0x4fd5b4
004fd574: add r3, r4, #0x11
004fd578: add r2, r4, #0x12
004fd57c: ldrb r0, [r2, #1]
004fd580: ldrb r1, [r3, #-1]
004fd584: cmp r2, r3
004fd588: eor r1, r0, r1
004fd58c: strb r1, [r3, #-1]
004fd590: ldrb r0, [r2, #1]
004fd594: eor r1, r1, r0
004fd598: strb r1, [r2, #1]
004fd59c: ldrb r0, [r3, #-1]
004fd5a0: sub r2, r2, #1
004fd5a4: eor r1, r1, r0
004fd5a8: strb r1, [r3, #-1]
004fd5ac: add r3, r3, #1
004fd5b0: bhi #0x4fd57c
004fd5b4: mov r0, r5
004fd5b8: add r1, r4, #0x14
004fd5bc: bl #0x459090
004fd5c0: mov r3, #1
004fd5c4: cmp r3, #0
004fd5c8: str r3, [sp, #4]
004fd5cc: bne #0x4fd610
004fd5d0: add r3, r4, #0x16
004fd5d4: add r4, r4, #0x15
004fd5d8: ldrb r1, [r3, #1]
004fd5dc: ldrb r2, [r4, #-1]
004fd5e0: cmp r3, r4
004fd5e4: eor r2, r1, r2
004fd5e8: strb r2, [r4, #-1]
004fd5ec: ldrb r1, [r3, #1]
004fd5f0: eor r2, r2, r1
004fd5f4: strb r2, [r3, #1]
004fd5f8: ldrb r1, [r4, #-1]
004fd5fc: sub r3, r3, #1
004fd600: eor r2, r2, r1
004fd604: strb r2, [r4, #-1]
004fd608: add r4, r4, #1
004fd60c: bhi #0x4fd5d8
004fd610: add sp, sp, #8
004fd614: pop {r4, r5, r6, pc}

_ZN13TriggerObject8InteractEP10GameObject 0x399af0 632
00399af0: push {r4, r5, r6, r7, r8, lr}
00399af4: ldr r3, [r0, #0x300]
00399af8: ldr r5, [pc, #0x22c]
00399afc: sub sp, sp, #0x40
00399b00: cmp r3, #0
00399b04: mov r4, r0
00399b08: mov r7, r1
00399b0c: add r5, pc, r5
00399b10: beq #0x399b20
00399b14: bl #0x3987ac
00399b18: cmp r0, #0
00399b1c: bne #0x399ca0
00399b20: mov r0, r4
00399b24: bl #0x398794
00399b28: ldr r3, [r4, #0x2d8]
00399b2c: cmp r3, #0
00399b30: beq #0x399b5c
00399b34: ldr ip, [r3, #0x38]
00399b38: ldr r1, [pc, #0x1f0]
00399b3c: mov r3, #0
00399b40: mov r0, ip
00399b44: mov r2, r3
00399b48: ldr ip, [ip]
00399b4c: add r1, pc, r1
00399b50: str r3, [sp]
00399b54: mov lr, pc
00399b58: ldr pc, [ip, #0x20]
00399b5c: ldr r3, [r4, #0x730]
00399b60: cmn r3, #1
00399b64: beq #0x399bc8
00399b68: ldr r2, [pc, #0x1c4]
00399b6c: ldr r1, [pc, #0x1c4]
00399b70: ldr lr, [r4, #0x168]
00399b74: ldr r2, [r5, r2]
00399b78: ldr r1, [r5, r1]
00399b7c: ldr r6, [r4, #0x164]
00399b80: ldr r2, [r2]
00399b84: ldr r0, [r1]
00399b88: mov r1, #0x18
00399b8c: mla r3, r1, r3, r2
00399b90: ldr r7, [r4, #0x160]
00399b94: mov ip, #0xbf000000
00399b98: ldr r1, [r3, #0x10]
00399b9c: add ip, ip, #0x800000
00399ba0: str lr, [sp, #0x34]
00399ba4: add r2, sp, #0x2c
00399ba8: mov lr, #1
00399bac: mov r3, #0
00399bb0: str r7, [sp, #0x2c]
00399bb4: str r6, [sp, #0x30]
00399bb8: str lr, [sp]
00399bbc: str ip, [sp, #8]
00399bc0: str ip, [sp, #4]
00399bc4: bl #0x36b5d8
00399bc8: ldr r1, [r4, #0x768]
00399bcc: cmn r1, #1
00399bd0: beq #0x399bf8
00399bd4: ldr r3, [r4, #0x3b4]
00399bd8: tst r3, #1
00399bdc: beq #0x399bf8
00399be0: ldr r3, [pc, #0x154]
00399be4: ldr r2, [r4, #0x64]
00399be8: ldr r0, [r5, r3]
00399bec: mov r3, #0
00399bf0: bl #0x4605c0
00399bf4: b #0x399c98
00399bf8: ldr r1, [r4, #0x74c]
00399bfc: cmn r1, #1
00399c00: beq #0x399c18
00399c04: ldr r3, [pc, #0x130]
00399c08: ldr r2, [r4, #0x64]
00399c0c: ldr r0, [r5, r3]
00399c10: mov r3, #0
00399c14: bl #0x4605c0
00399c18: ldr r6, [pc, #0x120]
00399c1c: ldr r0, [r5, r6]
00399c20: bl #0x31f594
00399c24: subs r7, r0, #0
00399c28: beq #0x399cd8
00399c2c: ldr r3, [r5, r6]
00399c30: ldr r1, [pc, #0x10c]
00399c34: ldr r2, [pc, #0x10c]
00399c38: ldr r0, [r3, #0x2c]
00399c3c: add r1, pc, r1
00399c40: add r2, pc, r2
00399c44: ldr r6, [r4, #0x730]
00399c48: ldr r8, [r4, #0x64]
00399c4c: bl #0x4c4bdc
00399c50: ldr r2, [pc, #0xf4]
00399c54: add r1, sp, #0x40
00399c58: mov r3, #0
00399c5c: ldr r2, [r5, r2]
00399c60: str r0, [sp, #0x14]
00399c64: mov r0, r7
00399c68: add r2, r2, #8
00399c6c: str r2, [r1, #-0x30]!
00399c70: mvn r2, #0
00399c74: strb r3, [sp, #0x21]
00399c78: str r3, [sp, #0x18]
00399c7c: strb r3, [sp, #0x20]
00399c80: str r8, [sp, #0x1c]
00399c84: str r2, [sp, #0x24]
00399c88: str r6, [sp, #0x28]
00399c8c: bl #0x339090
00399c90: mov r3, #1
00399c94: strb r3, [r4, #0x373]
00399c98: add sp, sp, #0x40
00399c9c: pop {r4, r5, r6, r7, r8, pc}
00399ca0: add r6, sp, #0x38
00399ca4: mov r0, r6
00399ca8: bl #0x3192b4
00399cac: mov r0, r6
00399cb0: mov r1, r7
00399cb4: bl #0x386f28
00399cb8: ldr r1, [pc, #0x90]
00399cbc: ldr r0, [r4, #0x300]
00399cc0: mov r2, r6
00399cc4: add r1, pc, r1
00399cc8: bl #0x37c41c
00399ccc: mov r0, r6
00399cd0: bl #0x319228
00399cd4: b #0x399b20
00399cd8: ldr r3, [pc, #0x74]
00399cdc: ldr r3, [r5, r3]
00399ce0: ldr r3, [r3]
00399ce4: cmp r3, #2
00399ce8: streq r7, [r7]
00399cec: beq #0x399c2c
00399cf0: cmp r3, #1
00399cf4: bne #0x399c2c
00399cf8: ldr r0, [pc, #0x58]
00399cfc: ldr r1, [pc, #0x58]
00399d00: ldr r2, [pc, #0x58]
00399d04: ldr r0, [r5, r0]
00399d08: ldr r3, [pc, #0x54]
00399d0c: movw ip, #0x11e
00399d10: add r1, pc, r1
00399d14: add r2, pc, r2
00399d18: add r3, pc, r3
00399d1c: add r0, r0, #0xa8
00399d20: str ip, [sp]
00399d24: bl #0x30e004
00399d28: b #0x399c2c
00399d2c: subseq sl, pc, r4, lsl #31
00399d30: subseq r8, r2, ip, lsl #31
00399d34: andeq r0, r0, ip, lsl lr
00399d38: andeq r0, r0, r4, lsr #27
00399d3c: andeq r1, r0, r0, lsr #20
00399d40: strdeq r3, r4, [r0], -r4
00399d44: subseq r8, r2, ip, lsr #26
00399d48: subseq r8, r2, r8, lsl #30
00399d4c: andeq r3, r0, r8, lsr #13
00399d50: subseq r8, r2, r4, lsr #28
00399d54: andeq r3, r0, r0, asr #19
00399d58: andeq r1, r0, r0, asr #19
00399d5c: subseq r4, r2, r8, asr #13
00399d60: subseq r5, r7, r4, asr #24
00399d64: subseq r8, r2, r0, ror #27

_ZN14CheckpointZoneC2EN10ObjectBase6GO_IDSE 0x3959b4 104
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

_ZN14CheckpointZoneD0Ev 0x395bb4 28
00395bb4: push {r4, lr}
00395bb8: mov r4, r0
00395bbc: bl #0x395b38
00395bc0: mov r0, r4
00395bc4: bl #0x310440
00395bc8: mov r0, r4
00395bcc: pop {r4, pc}

_ZN15QuestMoveInZoneC1EN10ObjectBase6GO_IDSE 0x396330 72
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

_ZThn36_N21DestructibleContainerD0Ev 0x3a1458 8
003a1458: sub r0, r0, #0x24
003a145c: b #0x3a1460

_ZN7Structs4DoorD1Ev 0x4da098 64
004da098: push {r4, lr}
004da09c: ldr r3, [pc, #0x2c]
004da0a0: ldr r2, [pc, #0x2c]
004da0a4: mov r4, r0
004da0a8: add r3, pc, r3
004da0ac: ldr r0, [r0, #8]
004da0b0: ldr r2, [r3, r2]
004da0b4: cmp r0, #0
004da0b8: add r2, r2, #8
004da0bc: str r2, [r4]
004da0c0: beq #0x4da0c8
004da0c4: bl #0x310440
004da0c8: mov r0, r4
004da0cc: pop {r4, pc}
004da0d0: subeq sl, fp, r8, ror #19
004da0d4: muleq r0, ip, sb

_ZN12SoundEmitter8InitPostEv 0x395144 124
00395144: ldr r3, [pc, #0x68]
00395148: ldr r2, [pc, #0x68]
0039514c: push {r4, r5, r6, r7, r8, lr}
00395150: add r3, pc, r3
00395154: ldr r2, [r3, r2]
00395158: mov r8, r0
0039515c: ldr r6, [r0, #0x38c]
00395160: ldr r5, [r2]
00395164: cmp r5, #0
00395168: beq #0x3951a8
0039516c: ldr r2, [pc, #0x48]
00395170: mov r4, #0
00395174: ldr r3, [r3, r2]
00395178: ldr r7, [r3]
0039517c: b #0x39518c
00395180: add r4, r4, #1
00395184: cmp r4, r5
00395188: beq #0x3951a8
0039518c: ldr r1, [r7, r4, lsl #2]
00395190: mov r0, r6
00395194: bl #0x30e31c
00395198: cmp r0, #0
0039519c: bne #0x395180
003951a0: str r4, [r8, #0x390]
003951a4: pop {r4, r5, r6, r7, r8, pc}
003951a8: mvn r4, #0
003951ac: str r4, [r8, #0x390]
003951b0: pop {r4, r5, r6, r7, r8, pc}
003951b4: subseq pc, pc, r0, asr #18
003951b8: andeq r3, r0, r8, lsr sp
003951bc: andeq r3, r0, r8, lsr #19

_ZN15QuestMoveInZoneD0Ev 0x3962d4 28
003962d4: push {r4, lr}
003962d8: mov r4, r0
003962dc: bl #0x39628c
003962e0: mov r0, r4
003962e4: bl #0x310440
003962e8: mov r0, r4
003962ec: pop {r4, pc}

_ZN21DestructibleContainer17DeclarePropertiesEv 0x3a1270 4
003a1270: b #0x3a083c

_ZNK11TriggerZone21IsDoorClosedActivatedEv 0x39b27c 36
0039b27c: ldr r0, [r0, #0x7b8]
0039b280: cmp r0, #0
0039b284: bxeq lr
0039b288: ldr r0, [r0, #0x3a8]
0039b28c: cmp r0, #1
0039b290: cmpne r0, #3
0039b294: movne r0, #0
0039b298: moveq r0, #1
0039b29c: bx lr

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

_Z14GetNewInstanceI13TriggerObjectEP10ObjectBasev 0x340f0c 32
00340f0c: push {r4, lr}
00340f10: mov r1, #0
00340f14: mov r0, #0x790
00340f18: bl #0x310570
00340f1c: mov r4, r0
00340f20: bl #0x399df0
00340f24: mov r0, r4
00340f28: pop {r4, pc}

_ZThn4_N12SoundEmitter17DeclarePropertiesEv 0x395710 8
00395710: sub r0, r0, #4
00395714: b #0x395718

_ZN13TriggerObject8InitPostEv 0x399ff8 920
00399ff8: push {r4, r5, r6, r7, r8, sl, lr}
00399ffc: sub sp, sp, #0x24
0039a000: mov r4, r0
0039a004: bl #0x38bd64
0039a008: ldr r3, [r4, #0x274]
0039a00c: ldr r5, [pc, #0x33c]
0039a010: cmp r0, r3
0039a014: add r5, pc, r5
0039a018: bge #0x39a2f8
0039a01c: ldr r3, [pc, #0x330]
0039a020: ldr r8, [r4, #0x72c]
0039a024: ldr r3, [r5, r3]
0039a028: ldr r7, [r3]
0039a02c: cmp r7, #0
0039a030: beq #0x39a30c
0039a034: ldr r3, [pc, #0x31c]
0039a038: mov r6, #0
0039a03c: ldr r3, [r5, r3]
0039a040: ldr sl, [r3]
0039a044: b #0x39a054
0039a048: add r6, r6, #1
0039a04c: cmp r6, r7
0039a050: beq #0x39a30c
0039a054: ldr r1, [sl, r6, lsl #2]
0039a058: mov r0, r8
0039a05c: bl #0x30e31c
0039a060: cmp r0, #0
0039a064: bne #0x39a048
0039a068: cmn r6, #1
0039a06c: str r6, [r4, #0x730]
0039a070: beq #0x39a0c4
0039a074: ldr r7, [pc, #0x2e0]
0039a078: mov r2, #0x18
0039a07c: ldr r3, [r5, r7]
0039a080: ldr r3, [r3]
0039a084: mla r6, r2, r6, r3
0039a088: ldr r3, [r6, #0x14]
0039a08c: cmn r3, #1
0039a090: beq #0x39a0c4
0039a094: ldr r2, [pc, #0x2c4]
0039a098: mov r1, #0xc
0039a09c: ldr r2, [r5, r2]
0039a0a0: ldr r2, [r2]
0039a0a4: mla r3, r1, r3, r2
0039a0a8: ldr r6, [r3, #8]
0039a0ac: mov r0, r6
0039a0b0: bl #0x30de54
0039a0b4: mov r1, r6
0039a0b8: add r2, r6, r0
0039a0bc: add r0, r4, #0x290
0039a0c0: bl #0x3109e0
0039a0c4: ldr r3, [pc, #0x298]
0039a0c8: ldr r1, [r4, #0x748]
0039a0cc: mov r2, #0
0039a0d0: ldr r6, [r5, r3]
0039a0d4: mov r0, r6
0039a0d8: bl #0x4591f0
0039a0dc: ldr r1, [r4, #0x764]
0039a0e0: str r0, [r4, #0x74c]
0039a0e4: mov r2, #0
0039a0e8: mov r0, r6
0039a0ec: bl #0x4591f0
0039a0f0: str r0, [r4, #0x768]
0039a0f4: mov r0, r4
0039a0f8: bl #0x398874
0039a0fc: ldr r7, [r4, #0x780]
0039a100: ldr r3, [r4, #0x77c]
0039a104: cmp r3, r7
0039a108: beq #0x39a300
0039a10c: ldr r1, [pc, #0x254]
0039a110: mov r0, r7
0039a114: add r1, pc, r1
0039a118: bl #0x30e31c
0039a11c: cmp r0, #0
0039a120: beq #0x39a300
0039a124: ldr r3, [pc, #0x240]
0039a128: ldr r3, [r5, r3]
0039a12c: ldr r8, [r3]
0039a130: cmp r8, #0
0039a134: beq #0x39a1ac
0039a138: ldr r3, [pc, #0x230]
0039a13c: mov r6, #0
0039a140: ldr r3, [r5, r3]
0039a144: ldr sl, [r3]
0039a148: b #0x39a158
0039a14c: add r6, r6, #1
0039a150: cmp r6, r8
0039a154: beq #0x39a1ac
0039a158: ldr r1, [sl, r6, lsl #2]
0039a15c: mov r0, r7
0039a160: bl #0x30e31c
0039a164: cmp r0, #0
0039a168: bne #0x39a14c
0039a16c: cmn r6, #1
0039a170: beq #0x39a1ac
0039a174: mov r1, r0
0039a178: mov r0, #0xc
0039a17c: bl #0x310570
0039a180: mov r7, r0
0039a184: bl #0x4786f0
0039a188: ldr r3, [pc, #0x1e4]
0039a18c: str r7, [r4, #0x788]
0039a190: mov r0, r7
0039a194: ldr r3, [r5, r3]
0039a198: ldr r3, [r3]
0039a19c: add r6, r3, r6, lsl #4
0039a1a0: ldr r2, [r6, #4]
0039a1a4: ldr r1, [r6, #8]
0039a1a8: bl #0x478914
0039a1ac: ldr r3, [r4, #0x730]
0039a1b0: cmn r3, #1
0039a1b4: beq #0x39a2e4
0039a1b8: mov r0, r4
0039a1bc: bl #0x38ab60
0039a1c0: cmp r0, #0
0039a1c4: beq #0x39a2e4
0039a1c8: ldr r3, [r4, #0x2d8]
0039a1cc: cmp r3, #0
0039a1d0: beq #0x39a208
0039a1d4: ldrb r2, [r4, #0x784]
0039a1d8: cmp r2, #0
0039a1dc: bne #0x39a318
0039a1e0: ldr ip, [r3, #0x38]
0039a1e4: ldr r1, [pc, #0x18c]
0039a1e8: mov r3, r2
0039a1ec: mov r0, ip
0039a1f0: add r1, pc, r1
0039a1f4: ldr ip, [ip]
0039a1f8: str r2, [sp]
0039a1fc: mov r2, #1
0039a200: mov lr, pc
0039a204: ldr pc, [ip, #0x20]
0039a208: ldr r3, [pc, #0x16c]
0039a20c: mov r1, #0
0039a210: mov r0, #0x28
0039a214: ldr r3, [r5, r3]
0039a218: mov r6, r1
0039a21c: ldr r8, [r3, #0x44]
0039a220: bl #0x310570
0039a224: mov ip, #1
0039a228: mov lr, #2
0039a22c: mov r3, ip
0039a230: mov r1, r8
0039a234: mov r2, r4
0039a238: str lr, [sp, #0x10]
0039a23c: movw lr, #0xffff
0039a240: mov r7, r0
0039a244: str lr, [sp, #0x14]
0039a248: str ip, [sp, #0x18]
0039a24c: str r6, [sp]
0039a250: str r6, [sp, #4]
0039a254: str r6, [sp, #8]
0039a258: str r6, [sp, #0xc]
0039a25c: bl #0x46f2f0
0039a260: ldr r3, [pc, #0x118]
0039a264: mov r0, r4
0039a268: mov r1, r7
0039a26c: ldr r3, [r5, r3]
0039a270: mov r2, r6
0039a274: add r3, r3, #8
0039a278: str r3, [r7]
0039a27c: bl #0x394bf8
0039a280: ldr r3, [pc, #0xfc]
0039a284: ldr r3, [r5, r3]
0039a288: ldr r0, [r3]
0039a28c: cmp r0, r6
0039a290: beq #0x39a348
0039a294: ldr r7, [pc, #0xc0]
0039a298: ldr r3, [r4, #0x730]
0039a29c: mov r1, #0x18
0039a2a0: ldr r2, [r5, r7]
0039a2a4: ldr r2, [r2]
0039a2a8: mla r3, r1, r3, r2
0039a2ac: ldr r1, [r3, #0x10]
0039a2b0: bl #0x3699fc
0039a2b4: ldr r2, [r5, r7]
0039a2b8: ldr r3, [r4, #0x730]
0039a2bc: mov r1, #0x18
0039a2c0: ldr r2, [r2]
0039a2c4: mov r0, r4
0039a2c8: mla r3, r1, r3, r2
0039a2cc: ldr r2, [pc, #0xb4]
0039a2d0: ldr r1, [r3, #0xc]
0039a2d4: add r2, pc, r2
0039a2d8: add sp, sp, #0x24
0039a2dc: pop {r4, r5, r6, r7, r8, sl, lr}
0039a2e0: b #0x38ef60
0039a2e4: mov r0, r4
0039a2e8: ldr r3, [r4]
0039a2ec: mov r1, #0
0039a2f0: mov lr, pc
0039a2f4: ldr pc, [r3, #0x40]
0039a2f8: add sp, sp, #0x24
0039a2fc: pop {r4, r5, r6, r7, r8, sl, pc}
0039a300: mov r3, #1
0039a304: strb r3, [r4, #0x784]
0039a308: b #0x39a1ac
0039a30c: mvn r3, #0
0039a310: str r3, [r4, #0x730]
0039a314: b #0x39a0c4
0039a318: ldr ip, [r3, #0x38]
0039a31c: ldr r1, [pc, #0x68]
0039a320: mov r2, #0
0039a324: mov r3, r2
0039a328: mov r0, ip
0039a32c: add r1, pc, r1
0039a330: ldr ip, [ip]
0039a334: str r2, [sp]
0039a338: mov r2, #1
0039a33c: mov lr, pc
0039a340: ldr pc, [ip, #0x20]
0039a344: b #0x39a208
0039a348: ldr r7, [pc, #0xc]
0039a34c: b #0x39a2b4
0039a350: subseq sl, pc, ip, ror sl
0039a354: andeq r0, r0, r8, lsr #25
0039a358: andeq r4, r0, r8, asr r2
0039a35c: andeq r0, r0, ip, lsl lr
0039a360: andeq r1, r0, r8, lsr #25
0039a364: andeq r1, r0, r0, lsr #20
0039a368: subseq r0, r4, ip, lsl #11
0039a36c: andeq r2, r0, r4, ror #30
0039a370: andeq r3, r0, r4, asr r5
0039a374: andeq r2, r0, r0, asr #16
0039a378: subseq r8, r2, r8, asr #17
0039a37c: strdeq r3, r4, [r0], -r4
0039a380: andeq r2, r0, r8, lsl r4
0039a384: andeq r0, r0, r4, lsr #27
0039a388: subseq r8, r2, ip, lsr #17
0039a38c: subseq r7, r2, r4, lsl #31

_ZN11TriggerZone11setUpdatingEb 0x39b26c 16
0039b26c: cmp r1, #0
0039b270: strb r1, [r0, #0x85]
0039b274: strbeq r1, [r0, #0x7b4]
0039b278: bx lr

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

_ZN7Structs4DoorD0Ev 0x4da0d8 28
004da0d8: push {r4, lr}
004da0dc: mov r4, r0
004da0e0: bl #0x4da098
004da0e4: mov r0, r4
004da0e8: bl #0x310440
004da0ec: mov r0, r4
004da0f0: pop {r4, pc}

_Z14GetNewInstanceI10SpawnPointEP10ObjectBasev 0x340e10 36
00340e10: push {r4, lr}
00340e14: mov r1, #0
00340e18: mov r0, #0x394
00340e1c: bl #0x310570
00340e20: mov r1, #0xd
00340e24: mov r4, r0
00340e28: bl #0x3ea388
00340e2c: mov r0, r4
00340e30: pop {r4, pc}

_ZN21DestructibleContainerD0Ev 0x3a1460 28
003a1460: push {r4, lr}
003a1464: mov r4, r0
003a1468: bl #0x3a1418
003a146c: mov r0, r4
003a1470: bl #0x310440
003a1474: mov r0, r4
003a1478: pop {r4, pc}

_ZN13TriggerObjectC2Ev 0x399ef4 260
00399ef4: push {r4, r5, r6, lr}
00399ef8: mov r2, #0
00399efc: mov r3, #1
00399f00: mov r1, #0x14
00399f04: ldr r5, [pc, #0xe4]
00399f08: mov r4, r0
00399f0c: bl #0x398fd4
00399f10: ldr r2, [pc, #0xdc]
00399f14: add r5, pc, r5
00399f18: add r3, r4, #0x710
00399f1c: ldr r2, [r5, r2]
00399f20: add r3, r3, #8
00399f24: mov r0, r3
00399f28: add ip, r2, #8
00399f2c: add r1, r2, #0xf4
00399f30: add r2, r2, #0xe8
00399f34: str ip, [r4]
00399f38: str r2, [r4, #4]
00399f3c: str r1, [r4, #0x24]
00399f40: str r3, [r4, #0x728]
00399f44: str r3, [r4, #0x72c]
00399f48: mov r1, #0x10
00399f4c: bl #0x31167c
00399f50: ldr r2, [r4, #0x728]
00399f54: mov r5, #0
00399f58: mvn r6, #0
00399f5c: mov r3, r4
00399f60: strb r5, [r2]
00399f64: str r6, [r3, #0x730]!
00399f68: add r3, r3, #4
00399f6c: mov r0, r3
00399f70: str r3, [r4, #0x744]
00399f74: str r3, [r4, #0x748]
00399f78: mov r1, #0x10
00399f7c: bl #0x31167c
00399f80: ldr r2, [r4, #0x744]
00399f84: add r3, r4, #0x750
00399f88: mov r0, r3
00399f8c: strb r5, [r2]
00399f90: mov r1, #0x10
00399f94: str r3, [r4, #0x760]
00399f98: str r3, [r4, #0x764]
00399f9c: str r6, [r4, #0x74c]
00399fa0: bl #0x31167c
00399fa4: mov r3, r4
00399fa8: ldr r2, [r3, #0x760]!
00399fac: mov r1, #0x10
00399fb0: add r3, r3, #0xc
00399fb4: strb r5, [r2]
00399fb8: mov r0, r3
00399fbc: str r3, [r4, #0x77c]
00399fc0: str r3, [r4, #0x780]
00399fc4: bl #0x31167c
00399fc8: ldr r2, [r4, #0x77c]
00399fcc: mov r3, #1
00399fd0: mov r0, r4
00399fd4: strb r5, [r2]
00399fd8: strb r5, [r4, #0x84]
00399fdc: strb r3, [r4, #0x85]
00399fe0: strb r5, [r4, #0x784]
00399fe4: str r5, [r4, #0x788]
00399fe8: strb r3, [r4, #0x28]
00399fec: pop {r4, r5, r6, pc}
00399ff0: subseq sl, pc, ip, ror fp
00399ff4: andeq r2, r0, r4, ror #22

_ZN4Door6UpdateEv 0x3e7560 56
003e7560: push {r4, lr}
003e7564: mov r3, #0x370
003e7568: ldrsh r3, [r0, r3]
003e756c: mov r4, r0
003e7570: cmp r3, #0
003e7574: blt #0x3e757c
003e7578: bl #0x38ae2c
003e757c: ldr r0, [r4, #0x2d8]
003e7580: cmp r0, #0
003e7584: beq #0x3e758c
003e7588: bl #0x38ba74
003e758c: mov r0, r4
003e7590: pop {r4, lr}
003e7594: b #0x38b8b8

_ZN12SoundEmitter17DeclarePropertiesEv 0x395718 552
00395718: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0039571c: ldr r6, [pc, #0x1e8]
00395720: ldr r3, [pc, #0x1e8]
00395724: sub sp, sp, #0x4c
00395728: add r6, pc, r6
0039572c: ldr ip, [r6, r3]
00395730: add r4, r0, #4
00395734: mov r5, r0
00395738: ldr r3, [ip]
0039573c: add sl, r0, #0x374
00395740: str ip, [sp, #4]
00395744: str r3, [sp, #0x44]
00395748: bl #0x38cee8
0039574c: mov r1, #0
00395750: mov r0, #0x24
00395754: bl #0x310570
00395758: ldr fp, [pc, #0x1b4]
0039575c: ldr r8, [pc, #0x1b4]
00395760: mov r7, r0
00395764: ldr fp, [r6, fp]
00395768: add r8, pc, r8
0039576c: mov r1, r8
00395770: add fp, fp, #8
00395774: add r2, sp, #0x10
00395778: str fp, [r0], #8
0039577c: bl #0x3140ec
00395780: ldr r3, [pc, #0x194]
00395784: rsb sl, r4, sl
00395788: mov r2, #1
0039578c: ldr r3, [r6, r3]
00395790: add sb, sp, #0x2c
00395794: str sl, [r7, #4]
00395798: add r3, r3, #8
0039579c: str r3, [r7]
003957a0: strb r2, [r7, #0x20]
003957a4: mov r1, r8
003957a8: mov r2, r7
003957ac: mov r0, r4
003957b0: bl #0x513ce4
003957b4: mov r0, sb
003957b8: mov r1, #0x10
003957bc: str sb, [sp, #0x3c]
003957c0: str sb, [sp, #0x40]
003957c4: bl #0x31167c
003957c8: ldr r3, [sp, #0x3c]
003957cc: mov r7, #0
003957d0: add r8, sp, #0x14
003957d4: strb r7, [r3]
003957d8: ldr r2, [sp, #0x3c]
003957dc: mov r0, r8
003957e0: ldr r1, [sp, #0x40]
003957e4: str r8, [sp, #0x24]
003957e8: str r8, [sp, #0x28]
003957ec: bl #0x3116e8
003957f0: mov r1, r7
003957f4: mov r0, #0x38
003957f8: bl #0x310570
003957fc: ldr sl, [pc, #0x11c]
00395800: mov r7, r0
00395804: add r2, sp, #0xc
00395808: add sl, pc, sl
0039580c: mov r1, sl
00395810: str fp, [r0], #8
00395814: bl #0x3140ec
00395818: ldr r3, [pc, #0x104]
0039581c: add r2, r5, #0x378
00395820: mov r0, r7
00395824: ldr r3, [r6, r3]
00395828: rsb r2, r4, r2
0039582c: str r2, [r7, #4]
00395830: add r3, r3, #8
00395834: str r3, [r0], #0x20
00395838: str r0, [r7, #0x30]
0039583c: str r0, [r7, #0x34]
00395840: ldr r1, [sp, #0x28]
00395844: ldr r2, [sp, #0x24]
00395848: bl #0x3116e8
0039584c: mov r1, sl
00395850: mov r2, r7
00395854: mov r0, r4
00395858: bl #0x513ce4
0039585c: mov r0, r8
00395860: bl #0x3139ac
00395864: mov r0, sb
00395868: bl #0x3139ac
0039586c: ldr r3, [pc, #0xb4]
00395870: ldr r7, [pc, #0xb4]
00395874: ldr r2, [pc, #0xb4]
00395878: ldr r6, [r6, r3]
0039587c: add r7, pc, r7
00395880: add r2, pc, r2
00395884: mov r1, r7
00395888: ldr r0, [r6, #0x2c]
0039588c: bl #0x4c4bdc
00395890: bl #0x30e964
00395894: ldr r8, [pc, #0x98]
00395898: add sl, r5, #0x394
0039589c: mov r3, r0
003958a0: add r8, pc, r8
003958a4: mov r1, r8
003958a8: mov r0, r4
003958ac: mov r2, sl
003958b0: bl #0x39503c
003958b4: ldr r2, [pc, #0x7c]
003958b8: mov r1, r7
003958bc: ldr r0, [r6, #0x2c]
003958c0: add r2, pc, r2
003958c4: bl #0x4c4bdc
003958c8: bl #0x30e964
003958cc: ldr r8, [pc, #0x68]
003958d0: add r5, r5, #0x398
003958d4: mov r3, r0
003958d8: add r8, pc, r8
003958dc: mov r2, r5
003958e0: mov r0, r4
003958e4: mov r1, r8
003958e8: bl #0x39503c
003958ec: ldr ip, [sp, #4]
003958f0: ldr r2, [sp, #0x44]
003958f4: ldr r3, [ip]
003958f8: cmp r2, r3
003958fc: bne #0x395908
00395900: add sp, sp, #0x4c
00395904: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00395908: bl #0x30e310
0039590c: subseq pc, pc, r8, ror #6
00395910: andeq r4, r0, ip, lsr #1
00395914: andeq r2, r0, r0, lsr r3
00395918: subseq sp, r2, r8, lsr #2
0039591c: andeq r3, r0, ip, asr #28

_ZN11TriggerZone8InitPostEv 0x39c0a8 544
0039c0a8: push {r4, r5, r6, r7, r8, sl, lr}
0039c0ac: mov r4, r0
0039c0b0: sub sp, sp, #0x1c
0039c0b4: bl #0x398874
0039c0b8: ldr r3, [r4, #0x734]
0039c0bc: ldr r1, [r4, #0x738]
0039c0c0: ldr r5, [pc, #0x1ec]
0039c0c4: cmp r3, r1
0039c0c8: mvn r3, #0
0039c0cc: str r3, [r4, #0x73c]
0039c0d0: add r5, pc, r5
0039c0d4: beq #0x39c0ec
0039c0d8: ldr r3, [pc, #0x1d8]
0039c0dc: mov r2, #0
0039c0e0: ldr r0, [r5, r3]
0039c0e4: bl #0x4591f0
0039c0e8: str r0, [r4, #0x73c]
0039c0ec: ldr r1, [r4, #0x754]
0039c0f0: ldr r3, [r4, #0x750]
0039c0f4: mvn r2, #0
0039c0f8: str r2, [r4, #0x758]
0039c0fc: cmp r3, r1
0039c100: beq #0x39c118
0039c104: ldr r3, [pc, #0x1ac]
0039c108: mov r2, #0
0039c10c: ldr r0, [r5, r3]
0039c110: bl #0x4591f0
0039c114: str r0, [r4, #0x758]
0039c118: ldr r1, [r4, #0x770]
0039c11c: ldr r3, [r4, #0x76c]
0039c120: mvn r2, #0
0039c124: str r2, [r4, #0x774]
0039c128: cmp r3, r1
0039c12c: beq #0x39c144
0039c130: ldr r3, [pc, #0x180]
0039c134: mov r2, #0
0039c138: ldr r0, [r5, r3]
0039c13c: bl #0x4591f0
0039c140: str r0, [r4, #0x774]
0039c144: ldr r1, [r4, #0x78c]
0039c148: ldr r3, [r4, #0x788]
0039c14c: mvn r2, #0
0039c150: str r2, [r4, #0x790]
0039c154: cmp r3, r1
0039c158: beq #0x39c170
0039c15c: ldr r3, [pc, #0x154]
0039c160: mov r2, #0
0039c164: ldr r0, [r5, r3]
0039c168: bl #0x4591f0
0039c16c: str r0, [r4, #0x790]
0039c170: ldr r7, [r4, #0x7a8]
0039c174: ldr r3, [r4, #0x7a4]
0039c178: mvn r2, #0
0039c17c: str r2, [r4, #0x7ac]
0039c180: cmp r3, r7
0039c184: beq #0x39c1d4
0039c188: ldr r3, [pc, #0x12c]
0039c18c: ldr r3, [r5, r3]
0039c190: ldr r8, [r3]
0039c194: cmp r8, #0
0039c198: beq #0x39c2a8
0039c19c: ldr r3, [pc, #0x11c]
0039c1a0: mov r6, #0
0039c1a4: ldr r3, [r5, r3]
0039c1a8: ldr sl, [r3]
0039c1ac: b #0x39c1bc
0039c1b0: add r6, r6, #1
0039c1b4: cmp r6, r8
0039c1b8: beq #0x39c2a8
0039c1bc: ldr r1, [sl, r6, lsl #2]
0039c1c0: mov r0, r7
0039c1c4: bl #0x30e31c
0039c1c8: cmp r0, #0
0039c1cc: bne #0x39c1b0
0039c1d0: str r6, [r4, #0x7ac]
0039c1d4: ldr r2, [r4, #0x7d0]
0039c1d8: ldr r1, [r4, #0x7cc]
0039c1dc: ldr r3, [r4, #0x3ac]
0039c1e0: cmp r2, r1
0039c1e4: str r3, [r4, #0x3b8]
0039c1e8: beq #0x39c250
0039c1ec: ldr r3, [pc, #0xd0]
0039c1f0: add r7, sp, #0xc
0039c1f4: mov r6, #0
0039c1f8: ldr r1, [r5, r3]
0039c1fc: mov r0, r7
0039c200: mvn r3, #0
0039c204: ldr r1, [r1, #0x38]
0039c208: str r6, [sp]
0039c20c: str r6, [sp, #4]
0039c210: bl #0x34aca0
0039c214: mov r0, r7
0039c218: mov r1, r6
0039c21c: bl #0x33fdc0
0039c220: cmp r0, r6
0039c224: beq #0x39c250
0039c228: mov r0, r7
0039c22c: mov r1, r6
0039c230: bl #0x33fdc0
0039c234: cmp r0, #0
0039c238: beq #0x39c248
0039c23c: ldr r3, [r0, #0xf4]
0039c240: cmp r3, #2
0039c244: beq #0x39c24c
0039c248: mov r0, #0
0039c24c: str r0, [r4, #0x7b8]
0039c250: ldr r3, [r4, #0x774]
0039c254: cmn r3, #1
0039c258: beq #0x39c26c
0039c25c: mov r3, #0
0039c260: strb r3, [r4, #0x3bc]
0039c264: add sp, sp, #0x1c
0039c268: pop {r4, r5, r6, r7, r8, sl, pc}
0039c26c: ldr r3, [r4, #0x790]
0039c270: cmn r3, #1
0039c274: bne #0x39c25c
0039c278: ldr r3, [r4, #0x73c]
0039c27c: cmn r3, #1
0039c280: beq #0x39c25c
0039c284: cmp r3, #0
0039c288: blt #0x39c25c
0039c28c: ldr r2, [pc, #0x24]
0039c290: mov r1, #0xc
0039c294: ldr r2, [r5, r2]
0039c298: ldr r2, [r2, #0x18]
0039c29c: mla r3, r1, r3, r2
0039c2a0: ldrb r3, [r3, #4]
0039c2a4: b #0x39c260
0039c2a8: mvn r6, #0
0039c2ac: str r6, [r4, #0x7ac]
0039c2b0: b #0x39c1d4
0039c2b4: subseq r8, pc, r0, asr #19
0039c2b8: andeq r1, r0, r0, lsr #20
0039c2bc: andeq r0, r0, r4, asr #13
0039c2c0: muleq r0, r4, r2
0039c2c4: strdeq r3, r4, [r0], -r4

_Z14GetNewInstanceI5DummyEP10ObjectBasev 0x3410a4 88
003410a4: push {r4, r5, r6, lr}
003410a8: mov r1, #0
003410ac: mov r0, #0x374
003410b0: bl #0x310570
003410b4: ldr r5, [pc, #0x38]
003410b8: mov r1, #0x14
003410bc: mov r4, r0
003410c0: bl #0x38c398
003410c4: ldr r3, [pc, #0x2c]
003410c8: add r5, pc, r5
003410cc: mov r2, #1
003410d0: ldr r3, [r5, r3]
003410d4: strb r2, [r4, #0x84]
003410d8: mov r0, r4
003410dc: add r2, r3, #0xe4
003410e0: add r1, r3, #8
003410e4: add r3, r3, #0xd8
003410e8: stm r4, {r1, r3}
003410ec: str r2, [r4, #0x24]
003410f0: pop {r4, r5, r6, pc}
003410f4: rsbeq r3, r5, r8, asr #19
003410f8: andeq r3, r0, r8, ror lr

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

_ZThn36_N13TriggerObject9SerializeEP11IStreamBase 0x39969c 8
0039969c: sub r0, r0, #0x24
003996a0: b #0x3996a4

_ZN10SpawnPoint17DeclarePropertiesEv 0x3ea554 400
003ea554: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ea558: ldr r4, [pc, #0x168]
003ea55c: ldr r3, [pc, #0x168]
003ea560: sub sp, sp, #0x4c
003ea564: add r4, pc, r4
003ea568: ldr r3, [r4, r3]
003ea56c: add sl, r0, #4
003ea570: mov sb, r0
003ea574: ldr r2, [r3]
003ea578: add r8, r0, #0x374
003ea57c: str r3, [sp, #4]
003ea580: str r2, [sp, #0x44]
003ea584: bl #0x38cee8
003ea588: mov r1, #0
003ea58c: mov r0, #0x24
003ea590: bl #0x310570
003ea594: ldr fp, [pc, #0x134]
003ea598: ldr r7, [pc, #0x134]
003ea59c: mov r5, r0
003ea5a0: ldr fp, [r4, fp]
003ea5a4: add r7, pc, r7
003ea5a8: mov r1, r7
003ea5ac: add fp, fp, #8
003ea5b0: add r2, sp, #0x10
003ea5b4: str fp, [r0], #8
003ea5b8: bl #0x3140ec
003ea5bc: ldr r2, [pc, #0x114]
003ea5c0: rsb r8, sl, r8
003ea5c4: mvn r1, #0
003ea5c8: ldr r2, [r4, r2]
003ea5cc: add r6, sp, #0x2c
003ea5d0: str r8, [r5, #4]
003ea5d4: add r2, r2, #8
003ea5d8: str r1, [r5, #0x20]
003ea5dc: str r2, [r5]
003ea5e0: mov r1, r7
003ea5e4: mov r2, r5
003ea5e8: mov r0, sl
003ea5ec: bl #0x513ce4
003ea5f0: mov r0, r6
003ea5f4: mov r1, #0x10
003ea5f8: str r6, [sp, #0x3c]
003ea5fc: str r6, [sp, #0x40]
003ea600: bl #0x31167c
003ea604: ldr r2, [sp, #0x3c]
003ea608: mov r5, #0
003ea60c: add r7, sp, #0x14
003ea610: strb r5, [r2]
003ea614: ldr r2, [sp, #0x3c]
003ea618: mov r0, r7
003ea61c: ldr r1, [sp, #0x40]
003ea620: str r7, [sp, #0x24]
003ea624: str r7, [sp, #0x28]
003ea628: bl #0x3116e8
003ea62c: mov r1, r5
003ea630: mov r0, #0x38
003ea634: bl #0x310570
003ea638: ldr r8, [pc, #0x9c]
003ea63c: mov r5, r0
003ea640: add r2, sp, #0xc
003ea644: add r8, pc, r8
003ea648: mov r1, r8
003ea64c: str fp, [r0], #8
003ea650: bl #0x3140ec
003ea654: ldr r2, [pc, #0x84]
003ea658: add sb, sb, #0x378
003ea65c: mov r0, r5
003ea660: ldr r2, [r4, r2]
003ea664: rsb sb, sl, sb
003ea668: str sb, [r5, #4]
003ea66c: add r2, r2, #8
003ea670: str r2, [r0], #0x20
003ea674: str r0, [r5, #0x30]
003ea678: str r0, [r5, #0x34]
003ea67c: ldr r1, [sp, #0x28]
003ea680: ldr r2, [sp, #0x24]
003ea684: bl #0x3116e8
003ea688: mov r2, r5
003ea68c: mov r1, r8
003ea690: mov r0, sl
003ea694: bl #0x513ce4
003ea698: mov r0, r7
003ea69c: bl #0x3139ac
003ea6a0: mov r0, r6
003ea6a4: bl #0x3139ac
003ea6a8: ldr r3, [sp, #4]
003ea6ac: ldr r2, [sp, #0x44]
003ea6b0: ldr r3, [r3]
003ea6b4: cmp r2, r3
003ea6b8: bne #0x3ea6c4
003ea6bc: add sp, sp, #0x4c
003ea6c0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ea6c4: bl #0x30e310
003ea6c8: subseq sl, sl, ip, lsr #10
003ea6cc: andeq r4, r0, ip, lsr #1
003ea6d0: andeq r2, r0, r0, lsr r3
003ea6d4: subeq r8, sp, ip, asr #15
003ea6d8: muleq r0, r0, r5
003ea6dc: umaaleq r7, sp, r4, r3
003ea6e0: muleq r0, r4, r4

_ZNK10SpawnPoint9IsZonableEv 0x3ea214 8
003ea214: mov r0, #0
003ea218: bx lr

_ZN7Structs4Door4readEP11IStreamBase 0x4fe050 464
004fe050: push {r4, r5, r6, lr}
004fe054: mov r4, r0
004fe058: sub sp, sp, #8
004fe05c: mov r0, r1
004fe060: mov r5, r1
004fe064: add r1, r4, #4
004fe068: bl #0x3df1a0
004fe06c: mov r3, #1
004fe070: cmp r3, #0
004fe074: str r3, [sp, #4]
004fe078: bne #0x4fe0bc
004fe07c: add r3, r4, #5
004fe080: add r2, r4, #6
004fe084: ldrb r0, [r2, #1]
004fe088: ldrb r1, [r3, #-1]
004fe08c: cmp r3, r2
004fe090: eor r1, r0, r1
004fe094: strb r1, [r3, #-1]
004fe098: ldrb r0, [r2, #1]
004fe09c: eor r1, r1, r0
004fe0a0: strb r1, [r2, #1]
004fe0a4: ldrb r0, [r3, #-1]
004fe0a8: sub r2, r2, #1
004fe0ac: eor r1, r1, r0
004fe0b0: strb r1, [r3, #-1]
004fe0b4: add r3, r3, #1
004fe0b8: blo #0x4fe084
004fe0bc: ldr r0, [r4, #8]
004fe0c0: cmp r0, #0
004fe0c4: beq #0x4fe0cc
004fe0c8: bl #0x310440
004fe0cc: ldr r0, [r4, #4]
004fe0d0: mov r1, #1
004fe0d4: mov r6, #0
004fe0d8: add r0, r0, r1
004fe0dc: bl #0x31056c
004fe0e0: ldr r2, [r4, #4]
004fe0e4: mov r1, r0
004fe0e8: str r0, [r4, #8]
004fe0ec: mov r3, r6
004fe0f0: mov r0, r5
004fe0f4: bl #0x317454
004fe0f8: ldr r3, [r4, #4]
004fe0fc: ldr r2, [r4, #8]
004fe100: mov r0, r5
004fe104: add r1, r4, #0xc
004fe108: strb r6, [r2, r3]
004fe10c: bl #0x459090
004fe110: mov r3, #1
004fe114: cmp r3, r6
004fe118: str r3, [sp, #4]
004fe11c: bne #0x4fe160
004fe120: add r3, r4, #0xd
004fe124: add r2, r4, #0xe
004fe128: ldrb r0, [r2, #1]
004fe12c: ldrb r1, [r3, #-1]
004fe130: cmp r3, r2
004fe134: eor r1, r0, r1
004fe138: strb r1, [r3, #-1]
004fe13c: ldrb r0, [r2, #1]
004fe140: eor r1, r1, r0
004fe144: strb r1, [r2, #1]
004fe148: ldrb r0, [r3, #-1]
004fe14c: sub r2, r2, #1
004fe150: eor r1, r1, r0
004fe154: strb r1, [r3, #-1]
004fe158: add r3, r3, #1
004fe15c: blo #0x4fe128
004fe160: mov r0, r5
004fe164: add r1, r4, #0x10
004fe168: bl #0x459090
004fe16c: mov r3, #1
004fe170: cmp r3, #0
004fe174: str r3, [sp, #4]
004fe178: bne #0x4fe1bc
004fe17c: add r3, r4, #0x11
004fe180: add r2, r4, #0x12
004fe184: ldrb r0, [r2, #1]
004fe188: ldrb r1, [r3, #-1]
004fe18c: cmp r2, r3
004fe190: eor r1, r0, r1
004fe194: strb r1, [r3, #-1]
004fe198: ldrb r0, [r2, #1]
004fe19c: eor r1, r1, r0
004fe1a0: strb r1, [r2, #1]
004fe1a4: ldrb r0, [r3, #-1]
004fe1a8: sub r2, r2, #1
004fe1ac: eor r1, r1, r0
004fe1b0: strb r1, [r3, #-1]
004fe1b4: add r3, r3, #1
004fe1b8: bhi #0x4fe184
004fe1bc: mov r0, r5
004fe1c0: add r1, r4, #0x14
004fe1c4: bl #0x459090
004fe1c8: mov r3, #1
004fe1cc: cmp r3, #0
004fe1d0: str r3, [sp, #4]
004fe1d4: bne #0x4fe218
004fe1d8: add r3, r4, #0x16
004fe1dc: add r4, r4, #0x15
004fe1e0: ldrb r1, [r3, #1]
004fe1e4: ldrb r2, [r4, #-1]
004fe1e8: cmp r3, r4
004fe1ec: eor r2, r1, r2
004fe1f0: strb r2, [r4, #-1]
004fe1f4: ldrb r1, [r3, #1]
004fe1f8: eor r2, r2, r1
004fe1fc: strb r2, [r3, #1]
004fe200: ldrb r1, [r4, #-1]
004fe204: sub r3, r3, #1
004fe208: eor r2, r2, r1
004fe20c: strb r2, [r4, #-1]
004fe210: add r4, r4, #1
004fe214: bhi #0x4fe1e0
004fe218: add sp, sp, #8
004fe21c: pop {r4, r5, r6, pc}

_ZN15QuestMoveInZoneD2Ev 0x3962f0 64
003962f0: ldr r2, [pc, #0x30]
003962f4: ldr r3, [pc, #0x30]
003962f8: push {r4, lr}
003962fc: add r2, pc, r2
00396300: ldr r3, [r2, r3]
00396304: mov r4, r0
00396308: add r2, r3, #0xf4
0039630c: add r1, r3, #8
00396310: add r3, r3, #0xe8
00396314: stm r0, {r1, r3}
00396318: str r2, [r0, #0x24]
0039631c: bl #0x397bc4
00396320: mov r0, r4
00396324: pop {r4, pc}

_ZNK11TriggerZone4DrawEv 0x39bd70 416
0039bd70: push {r4, r5, r6, r7, r8, sb, sl, lr}
0039bd74: ldr r4, [pc, #0x180]
0039bd78: ldr r6, [pc, #0x180]
0039bd7c: ldr r2, [pc, #0x180]
0039bd80: add r4, pc, r4
0039bd84: ldr r3, [r4, r6]
0039bd88: ldr r8, [r4, r2]
0039bd8c: sub sp, sp, #0x40
0039bd90: ldr r3, [r3]
0039bd94: mov r5, r0
0039bd98: mov r0, r8
0039bd9c: str r3, [sp, #0x3c]
0039bda0: bl #0x337888
0039bda4: ldr r1, [pc, #0x15c]
0039bda8: add r7, sp, #0x24
0039bdac: add r2, sp, #0x20
0039bdb0: add r1, pc, r1
0039bdb4: mov r0, r7
0039bdb8: bl #0x3140ec
0039bdbc: mov r0, r8
0039bdc0: mov r1, r7
0039bdc4: bl #0x337a88
0039bdc8: mov r8, r0
0039bdcc: mov r0, r7
0039bdd0: bl #0x3139ac
0039bdd4: cmp r8, #0
0039bdd8: beq #0x39bde8
0039bddc: ldr r3, [r5, #0x3b4]
0039bde0: cmp r3, #0
0039bde4: beq #0x39be04
0039bde8: ldr r3, [r4, r6]
0039bdec: ldr r2, [sp, #0x3c]
0039bdf0: ldr r3, [r3]
0039bdf4: cmp r2, r3
0039bdf8: bne #0x39bef8
0039bdfc: add sp, sp, #0x40
0039be00: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0039be04: mov r0, r5
0039be08: bl #0x38ab60
0039be0c: cmp r0, #0
0039be10: beq #0x39bde8
0039be14: ldr sl, [pc, #0xf0]
0039be18: ldr r3, [r4, sl]
0039be1c: ldr r3, [r3, #0x10]
0039be20: ldr r8, [r3, #0x10]
0039be24: movw r3, #0xffff
0039be28: ldr sb, [r8, #0xdc]
0039be2c: ldrh r2, [sb, #0x2e]
0039be30: cmp r2, r3
0039be34: bne #0x39be48
0039be38: mov r0, sb
0039be3c: mov r1, #1
0039be40: bl #0x5d8b28
0039be44: mov r2, r0
0039be48: add r7, sp, #0x1c
0039be4c: mov r0, r7
0039be50: mov r1, sb
0039be54: mov r3, #1
0039be58: bl #0x5dd0e4
0039be5c: ldr r0, [sp, #0x1c]
0039be60: cmp r0, #0
0039be64: moveq r2, #0xff
0039be68: beq #0x39be74
0039be6c: bl #0x5c5d34
0039be70: mov r2, r0
0039be74: mov r0, r8
0039be78: mov r1, r7
0039be7c: mov r3, #0
0039be80: bl #0x5ad368
0039be84: ldr r3, [r4, sl]
0039be88: ldr sb, [r5, #0x12c]
0039be8c: ldr sl, [r5, #0x130]
0039be90: ldr r3, [r3, #0x10]
0039be94: ldr r8, [r5, #0x134]
0039be98: ldr lr, [r5, #0x138]
0039be9c: ldr r0, [r3, #0x10]
0039bea0: ldr ip, [r5, #0x13c]
0039bea4: ldr r1, [r5, #0x140]
0039bea8: ldr r3, [r0]
0039beac: mvn r2, #0
0039beb0: mov r5, #0
0039beb4: ldr r3, [r3, #0x28]
0039beb8: strb r5, [sp, #0x18]
0039bebc: strb r2, [sp, #0x1b]
0039bec0: strb r2, [sp, #0x19]
0039bec4: strb r2, [sp, #0x1a]
0039bec8: str r1, [sp, #0x14]
0039becc: str sb, [sp]
0039bed0: str sl, [sp, #4]
0039bed4: str r8, [sp, #8]
0039bed8: str lr, [sp, #0xc]
0039bedc: str ip, [sp, #0x10]
0039bee0: mov r1, sp
0039bee4: ldr r2, [sp, #0x18]
0039bee8: blx r3
0039beec: mov r0, r7
0039bef0: bl #0x310be8
0039bef4: b #0x39bde8
0039bef8: bl #0x30e310
0039befc: subseq r8, pc, r0, lsl sp
0039bf00: andeq r4, r0, ip, lsr #1
0039bf04: andeq r0, r0, r4, lsl #17

_ZN4Door6OpenedEb 0x3e7788 160
003e7788: push {r4, r5, r6, lr}
003e778c: ldr r3, [r0, #0x2d8]
003e7790: ldr r5, [pc, #0x84]
003e7794: mov r2, #1
003e7798: cmp r3, #0
003e779c: add r5, pc, r5
003e77a0: sub sp, sp, #8
003e77a4: mov r4, r0
003e77a8: mov lr, r1
003e77ac: str r2, [r0, #0x3a8]
003e77b0: ldr r6, [r0, #0x2dc]
003e77b4: beq #0x3e77c0
003e77b8: cmp r1, #0
003e77bc: beq #0x3e77f4
003e77c0: cmp r6, #0
003e77c4: beq #0x3e77d0
003e77c8: mov r0, r6
003e77cc: bl #0x46eb70
003e77d0: ldr r3, [pc, #0x48]
003e77d4: add r1, r4, #0x1c8
003e77d8: mov r2, #0
003e77dc: ldr r0, [r5, r3]
003e77e0: bl #0x5252ec
003e77e4: mov r3, #1
003e77e8: strb r3, [r4, #0x373]
003e77ec: add sp, sp, #8
003e77f0: pop {r4, r5, r6, pc}
003e77f4: ldr ip, [r3, #0x38]
003e77f8: ldr r1, [pc, #0x24]
003e77fc: mov r3, lr
003e7800: mov r0, ip
003e7804: add r1, pc, r1
003e7808: ldr ip, [ip]
003e780c: str lr, [sp]
003e7810: mov lr, pc
003e7814: ldr pc, [ip, #0x20]
003e7818: b #0x3e77c0
003e781c: ldrsheq sp, [sl], #-0x24
003e7820: andeq r1, r0, r4, lsl #4
003e7824: ldrdeq lr, pc, [sp], #-0x84

_ZThn36_N13TriggerObjectD0Ev 0x3999d0 8
003999d0: sub r0, r0, #0x24
003999d4: b #0x3999d8

_ZN12SoundEmitterD1Ev 0x3953e0 140
003953e0: push {r4, lr}
003953e4: ldr r3, [pc, #0x74]
003953e8: ldr r2, [pc, #0x74]
003953ec: mov r4, r0
003953f0: add r3, pc, r3
003953f4: ldrb r0, [r0, #0x39c]
003953f8: ldr r2, [r3, r2]
003953fc: cmp r0, #0
00395400: add r1, r2, #0xe4
00395404: add r0, r2, #8
00395408: add r2, r2, #0xd8
0039540c: stm r4, {r0, r2}
00395410: str r1, [r4, #0x24]
00395414: bne #0x395430
00395418: add r0, r4, #0x378
0039541c: bl #0x3139ac
00395420: mov r0, r4
00395424: bl #0x38d378
00395428: mov r0, r4
0039542c: pop {r4, pc}
00395430: ldr r0, [pc, #0x30]
00395434: ldr r1, [r4, #0x390]
00395438: mov r2, #0
0039543c: ldr r3, [r3, r0]
00395440: ldr r0, [r3]
00395444: bl #0x369fec
00395448: add r0, r4, #0x378
0039544c: bl #0x3139ac
00395450: mov r0, r4
00395454: bl #0x38d378
00395458: mov r0, r4
0039545c: pop {r4, pc}
00395460: subseq pc, pc, r0, lsr #13
00395464: andeq r1, r0, r8, asr fp
00395468: andeq r0, r0, r4, lsr #27

_ZN11TriggerZoneD2Ev 0x39b804 140
0039b804: ldr r2, [pc, #0x7c]
0039b808: ldr r3, [pc, #0x7c]
0039b80c: push {r4, lr}
0039b810: add r2, pc, r2
0039b814: ldr r3, [r2, r3]
0039b818: mov r4, r0
0039b81c: add r2, r3, #0xf4
0039b820: add r1, r3, #8
0039b824: add r3, r3, #0xe8
0039b828: stm r0, {r1, r3}
0039b82c: str r2, [r0, #0x24]
0039b830: bl #0x39b2f4
0039b834: add r0, r4, #0x7b0
0039b838: add r0, r0, #0xc
0039b83c: bl #0x3139ac
0039b840: add r0, r4, #0x790
0039b844: add r0, r0, #4
0039b848: bl #0x3139ac
0039b84c: add r0, r4, #0x770
0039b850: add r0, r0, #8
0039b854: bl #0x3139ac
0039b858: add r0, r4, #0x750
0039b85c: add r0, r0, #0xc
0039b860: bl #0x3139ac
0039b864: add r0, r4, #0x740
0039b868: bl #0x3139ac
0039b86c: add r0, r4, #0x720
0039b870: add r0, r0, #4
0039b874: bl #0x3139ac
0039b878: mov r0, r4
0039b87c: bl #0x399214
0039b880: mov r0, r4
0039b884: pop {r4, pc}
0039b888: subseq sb, pc, r0, lsl #5
0039b88c: andeq r3, r0, r8, lsl ip

_ZN4Door9SerializeEP11IStreamBase 0x3e7b74 32
003e7b74: push {r4, r5, r6, lr}
003e7b78: mov r4, r0
003e7b7c: mov r5, r1
003e7b80: bl #0x38b9e4
003e7b84: mov r0, r5
003e7b88: add r1, r4, #0x3a8
003e7b8c: pop {r4, r5, r6, lr}
003e7b90: b #0x3e7abc

_ZN7Structs13TriggerObject8finalizeEv 0x4d9c9c 40
004d9c9c: push {r4, lr}
004d9ca0: mov r4, r0
004d9ca4: ldr r0, [r0, #0xc]
004d9ca8: cmp r0, #0
004d9cac: beq #0x4d9cc0
004d9cb0: bl #0x310440
004d9cb4: mov r3, #0
004d9cb8: str r3, [r4, #8]
004d9cbc: str r3, [r4, #0xc]
004d9cc0: pop {r4, pc}

_ZNK10SpawnPoint10IsAnimatedEv 0x3ea21c 8
003ea21c: mov r0, #0
003ea220: bx lr

_ZN4DoorD2Ev 0x3e8658 320
003e8658: push {r4, r5, r6, r7, r8, sb, sl, lr}
003e865c: ldr r5, [pc, #0x124]
003e8660: ldr r3, [pc, #0x124]
003e8664: ldr r6, [pc, #0x124]
003e8668: ldr r7, [pc, #0x124]
003e866c: add r5, pc, r5
003e8670: ldr ip, [r0, #0x65c]
003e8674: ldr r3, [r5, r3]
003e8678: ldr r2, [r5, r6]
003e867c: ldr r1, [r5, r7]
003e8680: mov r4, r0
003e8684: add r2, r2, #8
003e8688: add r0, r3, #0xf4
003e868c: cmp ip, #0
003e8690: add r1, r1, #8
003e8694: add ip, r3, #8
003e8698: add r3, r3, #0xe8
003e869c: str ip, [r4]
003e86a0: str r3, [r4, #4]
003e86a4: str r0, [r4, #0x24]
003e86a8: str r2, [r4, #0x670]
003e86ac: str r1, [r4, #0x540]
003e86b0: str r2, [r4, #0x6b0]
003e86b4: str r2, [r4, #0x690]
003e86b8: add r8, r4, #0x540
003e86bc: beq #0x3e86e4
003e86c0: add sl, r8, #0x10c
003e86c4: mov r0, sl
003e86c8: ldr r1, [r8, #0x110]
003e86cc: bl #0x370fd0
003e86d0: mov r3, #0
003e86d4: str sl, [r4, #0x654]
003e86d8: str r3, [r8, #0x110]
003e86dc: str sl, [r4, #0x658]
003e86e0: str r3, [r4, #0x65c]
003e86e4: ldr r2, [r5, r7]
003e86e8: ldr r3, [r5, r6]
003e86ec: ldr r1, [r4, #0x4cc]
003e86f0: add r2, r2, #8
003e86f4: add r3, r3, #8
003e86f8: cmp r1, #0
003e86fc: str r3, [r4, #0x4e0]
003e8700: str r2, [r4, #0x3b0]
003e8704: str r3, [r4, #0x520]
003e8708: str r3, [r4, #0x500]
003e870c: add r5, r4, #0x3b0
003e8710: beq #0x3e8738
003e8714: add r6, r5, #0x10c
003e8718: mov r0, r6
003e871c: ldr r1, [r5, #0x110]
003e8720: bl #0x370fd0
003e8724: mov r3, #0
003e8728: str r6, [r4, #0x4c4]
003e872c: str r3, [r5, #0x110]
003e8730: str r6, [r4, #0x4c8]
003e8734: str r3, [r4, #0x4cc]
003e8738: add r3, r4, #0x388
003e873c: ldr r0, [r3, #0x14]
003e8740: cmp r0, r3
003e8744: beq #0x3e8764
003e8748: cmp r0, #0
003e874c: beq #0x3e8764
003e8750: ldr r1, [r4, #0x388]
003e8754: rsb r1, r0, r1
003e8758: cmp r1, #0x80
003e875c: bhi #0x3e8774
003e8760: bl #0x708f00
003e8764: mov r0, r4
003e8768: bl #0x397bc4
003e876c: mov r0, r4
003e8770: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003e8774: bl #0x310440
003e8778: mov r0, r4
003e877c: bl #0x397bc4
003e8780: mov r0, r4
003e8784: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003e8788: subseq ip, sl, r4, lsr #8
003e878c: andeq r4, r0, r4, lsl #19
003e8790: andeq r1, r0, r8, lsr #1
003e8794: andeq r4, r0, r4, asr #7

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

_ZN10SpawnPointC1EN10ObjectBase6GO_IDSE 0x3ea388 116
003ea388: push {r4, r5, r6, lr}
003ea38c: ldr r5, [pc, #0x60]
003ea390: mov r4, r0
003ea394: bl #0x38c398
003ea398: ldr r3, [pc, #0x58]
003ea39c: add r5, pc, r5
003ea3a0: add r2, r4, #0x378
003ea3a4: ldr r3, [r5, r3]
003ea3a8: mvn r6, #0
003ea3ac: mov r0, r2
003ea3b0: add ip, r3, #8
003ea3b4: add r1, r3, #0xe4
003ea3b8: add r3, r3, #0xd8
003ea3bc: str r3, [r4, #4]
003ea3c0: str r1, [r4, #0x24]
003ea3c4: str r2, [r4, #0x388]
003ea3c8: str r2, [r4, #0x38c]
003ea3cc: str ip, [r4]
003ea3d0: str r6, [r4, #0x374]
003ea3d4: mov r1, #0x10
003ea3d8: bl #0x31167c
003ea3dc: ldr r3, [r4, #0x388]
003ea3e0: mov r2, #0
003ea3e4: mov r0, r4
003ea3e8: strb r2, [r3]
003ea3ec: str r6, [r4, #0x390]
003ea3f0: pop {r4, r5, r6, pc}
003ea3f4: ldrsheq sl, [sl], #-0x64
003ea3f8: muleq r0, ip, r0

_ZThn36_N11TriggerZoneD0Ev 0x39b7e0 8
0039b7e0: sub r0, r0, #0x24
0039b7e4: b #0x39b7e8

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

_ZN15QuestMoveInZoneD1Ev 0x39628c 64
0039628c: ldr r2, [pc, #0x30]
00396290: ldr r3, [pc, #0x30]
00396294: push {r4, lr}
00396298: add r2, pc, r2
0039629c: ldr r3, [r2, r3]
003962a0: mov r4, r0
003962a4: add r2, r3, #0xf4
003962a8: add r1, r3, #8
003962ac: add r3, r3, #0xe8
003962b0: stm r0, {r1, r3}
003962b4: str r2, [r0, #0x24]
003962b8: bl #0x397bc4
003962bc: mov r0, r4
003962c0: pop {r4, pc}
003962c4: ldrsheq lr, [pc], #-0x78
003962c8: andeq r2, r0, r4, ror lr

_ZN4DoorC2EN10ObjectBase6GO_IDSE 0x3e8324 176
003e8324: push {r4, r5, r6, r7, r8, lr}
003e8328: mov r2, #0
003e832c: mov r3, #1
003e8330: ldr r5, [pc, #0x94]
003e8334: mov r4, r0
003e8338: bl #0x397ca0
003e833c: ldr r3, [pc, #0x8c]
003e8340: add r5, pc, r5
003e8344: add r2, r4, #0x388
003e8348: ldr r3, [r5, r3]
003e834c: mov r0, r2
003e8350: str r2, [r4, #0x398]
003e8354: add ip, r3, #8
003e8358: add r1, r3, #0xf4
003e835c: add r3, r3, #0xe8
003e8360: str ip, [r4]
003e8364: str r3, [r4, #4]
003e8368: str r1, [r4, #0x24]
003e836c: str r2, [r4, #0x39c]
003e8370: mov r1, #0x10
003e8374: bl #0x31167c
003e8378: ldr r2, [r4, #0x398]
003e837c: mov r3, #0
003e8380: mov r7, #1
003e8384: add r6, r4, #0x3b0
003e8388: strb r3, [r2]
003e838c: add r5, r4, #0x540
003e8390: strb r3, [r4, #0x3ac]
003e8394: strb r3, [r4, #0x3a4]
003e8398: str r3, [r4, #0x3a8]
003e839c: strb r7, [r4, #0x3a5]
003e83a0: mov r0, r6
003e83a4: bl #0x3e80d4
003e83a8: mov r0, r5
003e83ac: bl #0x3e80d4
003e83b0: mov r3, #3
003e83b4: strb r7, [r4, #0x28]
003e83b8: str r6, [r4, #0x100]
003e83bc: str r5, [r4, #0x104]
003e83c0: strb r3, [r4, #0xf8]
003e83c4: mov r0, r4
003e83c8: pop {r4, r5, r6, r7, r8, pc}
003e83cc: subseq ip, sl, r0, asr r7
003e83d0: andeq r4, r0, r4, lsl #19

_Z14GetNewInstanceI12SoundEmitterEP10ObjectBasev 0x340ccc 36
00340ccc: push {r4, lr}
00340cd0: mov r1, #0
00340cd4: mov r0, #0x3a0
00340cd8: bl #0x310570
00340cdc: mov r1, #0x14
00340ce0: mov r4, r0
00340ce4: bl #0x39551c
00340ce8: mov r0, r4
00340cec: pop {r4, pc}

_ZN10SpawnPointC2EN10ObjectBase6GO_IDSE 0x3ea3fc 116
003ea3fc: push {r4, r5, r6, lr}
003ea400: ldr r5, [pc, #0x60]
003ea404: mov r4, r0
003ea408: bl #0x38c398
003ea40c: ldr r3, [pc, #0x58]
003ea410: add r5, pc, r5
003ea414: add r2, r4, #0x378
003ea418: ldr r3, [r5, r3]
003ea41c: mvn r6, #0
003ea420: mov r0, r2
003ea424: add ip, r3, #8
003ea428: add r1, r3, #0xe4
003ea42c: add r3, r3, #0xd8
003ea430: str r3, [r4, #4]
003ea434: str r1, [r4, #0x24]
003ea438: str r2, [r4, #0x388]
003ea43c: str r2, [r4, #0x38c]
003ea440: str ip, [r4]
003ea444: str r6, [r4, #0x374]
003ea448: mov r1, #0x10
003ea44c: bl #0x31167c
003ea450: ldr r3, [r4, #0x388]
003ea454: mov r2, #0
003ea458: mov r0, r4
003ea45c: strb r2, [r3]
003ea460: str r6, [r4, #0x390]
003ea464: pop {r4, r5, r6, pc}
003ea468: subseq sl, sl, r0, lsl #13
003ea46c: muleq r0, ip, r0

_ZN4Door6ClosedEb 0x3e7598 164
003e7598: push {r4, r5, r6, lr}
003e759c: ldr r3, [r0, #0x2d8]
003e75a0: ldr r5, [pc, #0x88]
003e75a4: mov r2, #0
003e75a8: cmp r3, #0
003e75ac: str r2, [r0, #0x3a8]
003e75b0: add r5, pc, r5
003e75b4: sub sp, sp, #8
003e75b8: mov r4, r0
003e75bc: mov r2, r1
003e75c0: ldr r6, [r0, #0x2dc]
003e75c4: beq #0x3e75d0
003e75c8: cmp r1, #0
003e75cc: beq #0x3e7604
003e75d0: cmp r6, #0
003e75d4: beq #0x3e75e0
003e75d8: mov r0, r6
003e75dc: bl #0x46ebe4
003e75e0: ldr r3, [pc, #0x4c]
003e75e4: add r1, r4, #0x1c8
003e75e8: mov r2, #1
003e75ec: ldr r0, [r5, r3]
003e75f0: bl #0x5252ec
003e75f4: mov r3, #0
003e75f8: strb r3, [r4, #0x373]
003e75fc: add sp, sp, #8
003e7600: pop {r4, r5, r6, pc}
003e7604: ldr ip, [r3, #0x38]
003e7608: mov r3, r1
003e760c: ldr r1, [pc, #0x24]
003e7610: mov r0, ip
003e7614: ldr ip, [ip]
003e7618: add r1, pc, r1
003e761c: str r2, [sp]
003e7620: mov r2, #1
003e7624: mov lr, pc
003e7628: ldr pc, [ip, #0x20]
003e762c: b #0x3e75d0
003e7630: subseq sp, sl, r0, ror #9
003e7634: andeq r1, r0, r4, lsl #4
003e7638: strheq lr, [sp], #-0xa0

_Z14GetNewInstanceI15QuestMoveInZoneEP10ObjectBasev 0x340f50 36
00340f50: push {r4, lr}
00340f54: mov r1, #0
00340f58: mov r0, #0x388
00340f5c: bl #0x310570
00340f60: mov r1, #0x14
00340f64: mov r4, r0
00340f68: bl #0x396330
00340f6c: mov r0, r4
00340f70: pop {r4, pc}

_ZNK4Door11IsUpdatableEv 0x3e7498 8
003e7498: mov r0, #1
003e749c: bx lr

_ZNK4Door13getUDTypeNameEv 0x3e74b8 16
003e74b8: ldr r0, [pc, #4]
003e74bc: add r0, pc, r0
003e74c0: bx lr
003e74c4: subeq sb, sp, r4, lsr #1

_ZN13TriggerObject9SerializeEP11IStreamBase 0x3996a4 4
003996a4: b #0x398978

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

_ZThn36_N12SoundEmitterD1Ev 0x3953d8 8
003953d8: sub r0, r0, #0x24
003953dc: b #0x3953e0

_ZThn36_N5DecorD0Ev 0x388ea8 8
00388ea8: sub r0, r0, #0x24
00388eac: b #0x388eb0

_ZN7Structs4Door8finalizeEv 0x4da070 40
004da070: push {r4, lr}
004da074: mov r4, r0
004da078: ldr r0, [r0, #8]
004da07c: cmp r0, #0
004da080: beq #0x4da094
004da084: bl #0x310440
004da088: mov r3, #0
004da08c: str r3, [r4, #4]
004da090: str r3, [r4, #8]
004da094: pop {r4, pc}

_ZN5Decor12LoadFloorMapEv 0x388730 128
00388730: push {r4, lr}
00388734: ldr r2, [r0, #0x2d8]
00388738: ldr r3, [pc, #0x68]
0038873c: mov r4, r0
00388740: cmp r2, #0
00388744: add r3, pc, r3
00388748: beq #0x388758
0038874c: ldrb r1, [r0, #0x375]
00388750: cmp r1, #0
00388754: bne #0x38875c
00388758: pop {r4, pc}
0038875c: ldr r1, [r2, #8]
00388760: ldr r2, [r0, #0x64]
00388764: ldr r0, [pc, #0x40]
00388768: ldr r0, [r3, r0]
0038876c: ldr r3, [r4, #0x44]
00388770: bl #0x523c14
00388774: cmp r0, #0
00388778: beq #0x38879c
0038877c: ldrb r3, [r4, #0x376]
00388780: add r1, r4, #0x12c
00388784: cmp r3, #0
00388788: ldr r3, [r0, #0x24]
0038878c: orrne r3, r3, #1
00388790: biceq r3, r3, #1
00388794: str r3, [r0, #0x24]
00388798: bl #0x388218
0038879c: mov r3, #0
003887a0: strb r3, [r4, #0x375]
003887a4: pop {r4, pc}
003887a8: rsbeq ip, r0, ip, asr #6
003887ac: andeq r1, r0, r4, lsl #4

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

_ZN4Door11DeserializeEP11IStreamBase 0x3e7a6c 80
003e7a6c: push {r4, r5, r6, lr}
003e7a70: mov r4, r0
003e7a74: mov r5, r1
003e7a78: bl #0x38b91c
003e7a7c: mov r0, r5
003e7a80: add r1, r4, #0x3a8
003e7a84: bl #0x3e79b4
003e7a88: ldr r3, [r4, #0x3a8]
003e7a8c: cmp r3, #1
003e7a90: beq #0x3e7aac
003e7a94: cmp r3, #3
003e7a98: beq #0x3e7aac
003e7a9c: mov r0, r4
003e7aa0: mov r1, #0
003e7aa4: pop {r4, r5, r6, lr}
003e7aa8: b #0x3e7598
003e7aac: mov r0, r4
003e7ab0: mov r1, #0
003e7ab4: pop {r4, r5, r6, lr}
003e7ab8: b #0x3e7788

_ZThn36_N5DummyD0Ev 0x341958 8
00341958: sub r0, r0, #0x24
0034195c: b #0x341960

_ZN10SpawnPoint8InitPostEv 0x3ea28c 56
003ea28c: push {r4, r5, r6, lr}
003ea290: ldr r4, [pc, #0x24]
003ea294: mov r5, r0
003ea298: bl #0x38be5c
003ea29c: ldr r3, [pc, #0x1c]
003ea2a0: add r4, pc, r4
003ea2a4: ldr r1, [r5, #0x38c]
003ea2a8: ldr r0, [r4, r3]
003ea2ac: mov r2, #0
003ea2b0: bl #0x4591f0
003ea2b4: str r0, [r5, #0x390]
003ea2b8: pop {r4, r5, r6, pc}
003ea2bc: ldrsheq sl, [sl], #-0x70
003ea2c0: andeq r1, r0, r0, lsr #20

_Z14GetNewInstanceI5DecorEP10ObjectBasev 0x3410fc 96
003410fc: push {r4, r5, r6, lr}
00341100: mov r1, #0
00341104: mov r0, #0x378
00341108: bl #0x310570
0034110c: ldr r5, [pc, #0x40]
00341110: mov r1, #0x14
00341114: mov r4, r0
00341118: bl #0x38c398
0034111c: ldr r3, [pc, #0x34]
00341120: add r5, pc, r5
00341124: mov r2, #1
00341128: ldr r3, [r5, r3]
0034112c: strb r2, [r4, #0x84]
00341130: strb r2, [r4, #0x375]
00341134: add r1, r3, #0xe4
00341138: add r0, r3, #8
0034113c: add r3, r3, #0xd8
00341140: stm r4, {r0, r3}
00341144: str r1, [r4, #0x24]
00341148: strb r2, [r4, #0x376]
0034114c: mov r0, r4
00341150: pop {r4, r5, r6, pc}
00341154: rsbeq r3, r5, r0, ror sb
00341158: andeq r2, r0, ip, lsl #22

_ZNK12SoundEmitter11IsUpdatableEv 0x394ecc 8
00394ecc: mov r0, #1
00394ed0: bx lr

_ZNK4Door13IsInteractiveEP10GameObject 0x3e74a8 8
003e74a8: mov r0, #0
003e74ac: bx lr

_ZNK10SpawnPoint13IsInteractiveEP10GameObject 0x3ea224 8
003ea224: mov r0, #0
003ea228: bx lr

_ZThn36_N12SoundEmitterD0Ev 0x39546c 8
0039546c: sub r0, r0, #0x24
00395470: b #0x395474

_ZN12SoundEmitterD2Ev 0x395490 140
00395490: push {r4, lr}
00395494: ldr r3, [pc, #0x74]
00395498: ldr r2, [pc, #0x74]
0039549c: mov r4, r0
003954a0: add r3, pc, r3
003954a4: ldrb r0, [r0, #0x39c]
003954a8: ldr r2, [r3, r2]
003954ac: cmp r0, #0
003954b0: add r1, r2, #0xe4
003954b4: add r0, r2, #8
003954b8: add r2, r2, #0xd8
003954bc: stm r4, {r0, r2}
003954c0: str r1, [r4, #0x24]
003954c4: bne #0x3954e0
003954c8: add r0, r4, #0x378
003954cc: bl #0x3139ac
003954d0: mov r0, r4
003954d4: bl #0x38d378
003954d8: mov r0, r4
003954dc: pop {r4, pc}
003954e0: ldr r0, [pc, #0x30]
003954e4: ldr r1, [r4, #0x390]
003954e8: mov r2, #0
003954ec: ldr r3, [r3, r0]
003954f0: ldr r0, [r3]
003954f4: bl #0x369fec
003954f8: add r0, r4, #0x378
003954fc: bl #0x3139ac
00395500: mov r0, r4
00395504: bl #0x38d378
00395508: mov r0, r4
0039550c: pop {r4, pc}
00395510: ldrsheq pc, [pc], #-0x50
00395514: andeq r1, r0, r8, asr fp
00395518: andeq r0, r0, r4, lsr #27

_ZThn36_N13TriggerObject11DeserializeEP11IStreamBase 0x399578 8
00399578: sub r0, r0, #0x24
0039957c: b #0x399580

_ZN13TriggerObject19TestInteractiveCondEv 0x3993bc 212
003993bc: ldr r3, [pc, #0xc0]
003993c0: ldr r2, [pc, #0xc0]
003993c4: push {r4, lr}
003993c8: add r3, pc, r3
003993cc: mov r4, r0
003993d0: ldr r0, [r3, r2]
003993d4: sub sp, sp, #8
003993d8: mov r1, #0
003993dc: mov r2, #1
003993e0: ldr r0, [r0, #0x40]
003993e4: bl #0x36e478
003993e8: ldr r3, [r0, #0x660]
003993ec: cmp r3, #0
003993f0: beq #0x399410
003993f4: movw r2, #0x14e8
003993f8: ldr r3, [r3, r2]
003993fc: cmp r3, #0
00399400: beq #0x399474
00399404: ldrb r3, [r3, #0x14]
00399408: cmp r3, #0
0039940c: beq #0x399474
00399410: ldr r0, [r4, #0x788]
00399414: cmp r0, #0
00399418: beq #0x39942c
0039941c: bl #0x478704
00399420: cmp r0, #0
00399424: strbeq r0, [r4, #0x784]
00399428: beq #0x39947c
0039942c: ldr r3, [r4, #0x2d8]
00399430: mov r2, #1
00399434: strb r2, [r4, #0x784]
00399438: cmp r3, #0
0039943c: beq #0x399468
00399440: ldr ip, [r3, #0x38]
00399444: ldr r1, [pc, #0x40]
00399448: mov r3, #0
0039944c: mov r0, ip
00399450: mov r2, r3
00399454: ldr ip, [ip]
00399458: add r1, pc, r1
0039945c: str r3, [sp]
00399460: mov lr, pc
00399464: ldr pc, [ip, #0x20]
00399468: mov r3, #0
0039946c: strb r3, [r4, #0x373]
00399470: b #0x39947c
00399474: mov r3, #0
00399478: strb r3, [r4, #0x784]
0039947c: add sp, sp, #8
00399480: pop {r4, pc}
00399484: subseq fp, pc, r8, asr #13
00399488: strdeq r3, r4, [r0], -r4
0039948c: subseq sb, r2, r8, asr r6

_ZNK12SoundEmitter4DrawEv 0x394ec0 4
00394ec0: bx lr

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

_ZN12SoundEmitter6UpdateEv 0x3951c0 536
003951c0: push {r4, r5, r6, r7, r8, sl, lr}
003951c4: ldr r4, [pc, #0x200]
003951c8: ldr r3, [pc, #0x200]
003951cc: sub sp, sp, #0x2c
003951d0: add r4, pc, r4
003951d4: ldr r6, [r4, r3]
003951d8: mov r5, r0
003951dc: mov r0, r6
003951e0: bl #0x31f594
003951e4: cmp r0, #0
003951e8: beq #0x39521c
003951ec: ldr r0, [r6, #0x40]
003951f0: mov r1, #0
003951f4: mov r2, #1
003951f8: bl #0x36e478
003951fc: ldr r3, [r0, #0x660]
00395200: cmp r3, #0
00395204: beq #0x39521c
00395208: mov r0, r6
0039520c: bl #0x31f594
00395210: ldr r3, [r0, #0x130]
00395214: cmp r3, #0x26
00395218: beq #0x395224
0039521c: add sp, sp, #0x2c
00395220: pop {r4, r5, r6, r7, r8, sl, pc}
00395224: mov r2, #1
00395228: ldr r0, [r6, #0x40]
0039522c: mov r1, #0
00395230: bl #0x36e478
00395234: ldr r3, [r0, #0x660]
00395238: ldr r0, [r5, #0x160]
0039523c: ldr r1, [r3, #0x160]
00395240: str r1, [sp, #0x1c]
00395244: ldr r7, [r3, #0x164]
00395248: str r7, [sp, #0x20]
0039524c: ldr r6, [r3, #0x168]
00395250: str r6, [sp, #0x24]
00395254: bl #0x30e3ac
00395258: mov r1, r7
0039525c: mov sl, r0
00395260: ldr r0, [r5, #0x164]
00395264: bl #0x30e3ac
00395268: mov r1, r6
0039526c: mov r8, r0
00395270: ldr r0, [r5, #0x168]
00395274: bl #0x30e3ac
00395278: mov r1, sl
0039527c: mov r7, r0
00395280: mov r0, sl
00395284: bl #0x30ed6c
00395288: mov r1, r8
0039528c: mov r6, r0
00395290: mov r0, r8
00395294: bl #0x30ed6c
00395298: mov r1, r0
0039529c: mov r0, r6
003952a0: bl #0x30eba4
003952a4: mov r1, r7
003952a8: mov r6, r0
003952ac: mov r0, r7
003952b0: bl #0x30ed6c
003952b4: mov r1, r0
003952b8: mov r0, r6
003952bc: bl #0x30eba4
003952c0: bl #0x30e8a4
003952c4: bl #0x30e1c0
003952c8: bl #0x30e6a0
003952cc: ldrb r3, [r5, #0x39c]
003952d0: mov r7, r0
003952d4: cmp r3, #0
003952d8: beq #0x395344
003952dc: ldr r6, [r5, #0x398]
003952e0: mov r1, r0
003952e4: mov r0, r6
003952e8: bl #0x30e9ac
003952ec: cmp r0, #0
003952f0: beq #0x39521c
003952f4: mov r0, r6
003952f8: mov r1, #0
003952fc: bl #0x30e2f8
00395300: cmp r0, #0
00395304: beq #0x39521c
00395308: mov r3, #0
0039530c: strb r3, [r5, #0x39c]
00395310: ldr r3, [pc, #0xbc]
00395314: ldr r3, [r4, r3]
00395318: ldr r0, [r3]
0039531c: cmp r0, #0
00395320: beq #0x395348
00395324: ldr r1, [r5, #0x390]
00395328: add r3, sp, #0x1c
0039532c: mov r2, #0xfa
00395330: str r6, [sp]
00395334: bl #0x36a218
00395338: ldrb r3, [r5, #0x39c]
0039533c: cmp r3, #0
00395340: bne #0x39521c
00395344: ldr r6, [r5, #0x398]
00395348: mov r1, r7
0039534c: mov r0, r6
00395350: bl #0x30e2f8
00395354: cmp r0, #0
00395358: beq #0x39521c
0039535c: mov r0, r6
00395360: mov r1, #0
00395364: bl #0x30e2f8
00395368: cmp r0, #0
0039536c: beq #0x39521c
00395370: ldr r3, [pc, #0x5c]
00395374: mov ip, #1
00395378: strb ip, [r5, #0x39c]
0039537c: ldr r3, [r4, r3]
00395380: ldr r0, [r3]
00395384: cmp r0, #0
00395388: beq #0x39521c
0039538c: ldrb r3, [r5, #0x374]
00395390: ldr r1, [r5, #0x390]
00395394: ldr r6, [r5, #0x164]
00395398: ldr r4, [r5, #0x168]
0039539c: ldr r5, [r5, #0x160]
003953a0: mov lr, #0xbf000000
003953a4: add lr, lr, #0x800000
003953a8: add r2, sp, #0x10
003953ac: str r5, [sp, #0x10]
003953b0: str r6, [sp, #0x14]
003953b4: str r4, [sp, #0x18]
003953b8: str ip, [sp]
003953bc: str lr, [sp, #8]
003953c0: str lr, [sp, #4]
003953c4: bl #0x36b5d8
003953c8: b #0x39521c
003953cc: subseq pc, pc, r0, asr #17
003953d0: strdeq r3, r4, [r0], -r4
003953d4: andeq r0, r0, r4, lsr #27

_ZN4DoorD1Ev 0x3e8824 320
003e8824: push {r4, r5, r6, r7, r8, sb, sl, lr}
003e8828: ldr r5, [pc, #0x124]
003e882c: ldr r3, [pc, #0x124]
003e8830: ldr r6, [pc, #0x124]
003e8834: ldr r7, [pc, #0x124]
003e8838: add r5, pc, r5
003e883c: ldr ip, [r0, #0x65c]
003e8840: ldr r3, [r5, r3]
003e8844: ldr r2, [r5, r6]
003e8848: ldr r1, [r5, r7]
003e884c: mov r4, r0
003e8850: add r2, r2, #8
003e8854: add r0, r3, #0xf4
003e8858: cmp ip, #0
003e885c: add r1, r1, #8
003e8860: add ip, r3, #8
003e8864: add r3, r3, #0xe8
003e8868: str ip, [r4]
003e886c: str r3, [r4, #4]
003e8870: str r0, [r4, #0x24]
003e8874: str r2, [r4, #0x670]
003e8878: str r1, [r4, #0x540]
003e887c: str r2, [r4, #0x6b0]
003e8880: str r2, [r4, #0x690]
003e8884: add r8, r4, #0x540
003e8888: beq #0x3e88b0
003e888c: add sl, r8, #0x10c
003e8890: mov r0, sl
003e8894: ldr r1, [r8, #0x110]
003e8898: bl #0x370fd0
003e889c: mov r3, #0
003e88a0: str sl, [r4, #0x654]
003e88a4: str r3, [r8, #0x110]
003e88a8: str sl, [r4, #0x658]
003e88ac: str r3, [r4, #0x65c]
003e88b0: ldr r2, [r5, r7]
003e88b4: ldr r3, [r5, r6]
003e88b8: ldr r1, [r4, #0x4cc]
003e88bc: add r2, r2, #8
003e88c0: add r3, r3, #8
003e88c4: cmp r1, #0
003e88c8: str r3, [r4, #0x4e0]
003e88cc: str r2, [r4, #0x3b0]
003e88d0: str r3, [r4, #0x520]
003e88d4: str r3, [r4, #0x500]
003e88d8: add r5, r4, #0x3b0
003e88dc: beq #0x3e8904
003e88e0: add r6, r5, #0x10c
003e88e4: mov r0, r6
003e88e8: ldr r1, [r5, #0x110]
003e88ec: bl #0x370fd0
003e88f0: mov r3, #0
003e88f4: str r6, [r4, #0x4c4]
003e88f8: str r3, [r5, #0x110]
003e88fc: str r6, [r4, #0x4c8]
003e8900: str r3, [r4, #0x4cc]
003e8904: add r3, r4, #0x388
003e8908: ldr r0, [r3, #0x14]
003e890c: cmp r0, r3
003e8910: beq #0x3e8930
003e8914: cmp r0, #0
003e8918: beq #0x3e8930
003e891c: ldr r1, [r4, #0x388]
003e8920: rsb r1, r0, r1
003e8924: cmp r1, #0x80
003e8928: bhi #0x3e8940
003e892c: bl #0x708f00
003e8930: mov r0, r4
003e8934: bl #0x397bc4
003e8938: mov r0, r4
003e893c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003e8940: bl #0x310440
003e8944: mov r0, r4
003e8948: bl #0x397bc4
003e894c: mov r0, r4
003e8950: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003e8954: subseq ip, sl, r8, asr r2
003e8958: andeq r4, r0, r4, lsl #19
003e895c: andeq r1, r0, r8, lsr #1
003e8960: andeq r4, r0, r4, asr #7

_ZN13TriggerObject17DeclarePropertiesEv 0x399d70 128
00399d70: push {r4, r5, r6, lr}
00399d74: mov r4, r0
00399d78: bl #0x398a88
00399d7c: ldr r1, [pc, #0x5c]
00399d80: add r5, r4, #4
00399d84: add r2, r4, #0x710
00399d88: mov r0, r5
00399d8c: add r1, pc, r1
00399d90: add r2, r2, #8
00399d94: bl #0x33ef7c
00399d98: ldr r1, [pc, #0x44]
00399d9c: add r2, r4, #0x730
00399da0: mov r0, r5
00399da4: add r2, r2, #4
00399da8: add r1, pc, r1
00399dac: bl #0x33ef7c
00399db0: ldr r1, [pc, #0x30]
00399db4: mov r0, r5
00399db8: add r2, r4, #0x750
00399dbc: add r1, pc, r1
00399dc0: bl #0x33ef7c
00399dc4: ldr r1, [pc, #0x20]
00399dc8: add r2, r4, #0x760
00399dcc: mov r0, r5
00399dd0: add r1, pc, r1
00399dd4: add r2, r2, #0xc
00399dd8: pop {r4, r5, r6, lr}
00399ddc: b #0x33ef7c
00399de0: subseq r8, r2, ip, asr #27
00399de4: subseq r7, r2, r0, lsr ip
00399de8: subseq r8, r2, r4, lsr #27
00399dec: subseq r8, r2, r0, lsr #27

_ZThn36_N14CheckpointZoneD1Ev 0x395b30 8
00395b30: sub r0, r0, #0x24
00395b34: b #0x395b38

_ZN10SpawnPointD2Ev 0x3ea33c 76
003ea33c: ldr r2, [pc, #0x3c]
003ea340: ldr r3, [pc, #0x3c]
003ea344: push {r4, lr}
003ea348: add r2, pc, r2
003ea34c: ldr r3, [r2, r3]
003ea350: mov r4, r0
003ea354: add r0, r0, #0x378
003ea358: add r2, r3, #0xe4
003ea35c: add r1, r3, #8
003ea360: add r3, r3, #0xd8
003ea364: stm r4, {r1, r3}
003ea368: str r2, [r4, #0x24]
003ea36c: bl #0x3139ac
003ea370: mov r0, r4
003ea374: bl #0x38d378
003ea378: mov r0, r4
003ea37c: pop {r4, pc}
003ea380: subseq sl, sl, r8, asr #14
003ea384: muleq r0, ip, r0

_ZN13TriggerObject26InterpretIncomingNetStructEb 0x39939c 32
0039939c: push {r4, r5, r6, lr}
003993a0: mov r4, r1
003993a4: mov r5, r0
003993a8: bl #0x3987dc
003993ac: cmp r4, #0
003993b0: movne r3, #0
003993b4: strbne r3, [r5, #0x784]
003993b8: pop {r4, r5, r6, pc}

_ZN4Door7EnabledEv 0x3e7bd8 68
003e7bd8: push {r4, lr}
003e7bdc: mov r4, r0
003e7be0: bl #0x38ba38
003e7be4: ldr r2, [r4, #0x3a8]
003e7be8: ldr r3, [pc, #0x24]
003e7bec: cmp r2, #1
003e7bf0: add r3, pc, r3
003e7bf4: beq #0x3e7c10
003e7bf8: ldr r0, [pc, #0x18]
003e7bfc: add r1, r4, #0x1c8
003e7c00: mov r2, #1
003e7c04: ldr r0, [r3, r0]
003e7c08: pop {r4, lr}
003e7c0c: b #0x5252ec
003e7c10: pop {r4, pc}
003e7c14: subseq ip, sl, r0, lsr #29
003e7c18: andeq r1, r0, r4, lsl #4

_ZN4Door15__EventCallbackERKN6glitch7collada15STriggeredEventEPv 0x3e8450 280
003e8450: push {r4, r5, r6, r7, r8, sl, lr}
003e8454: ldr r4, [pc, #0xf4]
003e8458: ldr r6, [pc, #0xf4]
003e845c: ldr r2, [pc, #0xf4]
003e8460: add r4, pc, r4
003e8464: ldr r3, [r4, r6]
003e8468: ldr r7, [r4, r2]
003e846c: sub sp, sp, #0x24
003e8470: ldr r3, [r3]
003e8474: mov r8, r0
003e8478: mov r0, r7
003e847c: str r3, [sp, #0x1c]
003e8480: mov sl, r1
003e8484: bl #0x337888
003e8488: ldr r1, [pc, #0xcc]
003e848c: add r5, sp, #4
003e8490: mov r2, sp
003e8494: add r1, pc, r1
003e8498: mov r0, r5
003e849c: bl #0x3140ec
003e84a0: mov r0, r7
003e84a4: mov r1, r5
003e84a8: bl #0x337a88
003e84ac: ldr r0, [sp, #0x18]
003e84b0: cmp r0, r5
003e84b4: beq #0x3e84d4
003e84b8: cmp r0, #0
003e84bc: beq #0x3e84d4
003e84c0: ldr r1, [sp, #4]
003e84c4: rsb r1, r0, r1
003e84c8: cmp r1, #0x80
003e84cc: bhi #0x3e8544
003e84d0: bl #0x708f00
003e84d4: ldr r5, [r8, #4]
003e84d8: ldr r1, [pc, #0x80]
003e84dc: mov r0, r5
003e84e0: add r1, pc, r1
003e84e4: bl #0x30e31c
003e84e8: cmp r0, #0
003e84ec: beq #0x3e8534
003e84f0: ldr r1, [pc, #0x6c]
003e84f4: mov r0, r5
003e84f8: add r1, pc, r1
003e84fc: bl #0x30e31c
003e8500: cmp r0, #0
003e8504: beq #0x3e8524
003e8508: ldr r3, [r4, r6]
003e850c: ldr r2, [sp, #0x1c]
003e8510: ldr r3, [r3]
003e8514: cmp r2, r3
003e8518: bne #0x3e854c
003e851c: add sp, sp, #0x24
003e8520: pop {r4, r5, r6, r7, r8, sl, pc}
003e8524: mov r0, sl
003e8528: mov r1, #1
003e852c: bl #0x3e7598
003e8530: b #0x3e8508
003e8534: mov r0, sl
003e8538: mov r1, #1
003e853c: bl #0x3e7788
003e8540: b #0x3e8508
003e8544: bl #0x310440
003e8548: b #0x3e84d4
003e854c: bl #0x30e310
003e8550: subseq ip, sl, r0, lsr r6
003e8554: andeq r4, r0, ip, lsr #1
003e8558: andeq r0, r0, r4, lsl #17
003e855c: subeq sp, sp, r4, asr ip
003e8560: subeq sl, sp, r8, lsr #21
003e8564: subeq sl, sp, r0, asr #16

_ZN5DummyD1Ev 0x341184 64
00341184: ldr r2, [pc, #0x30]
00341188: ldr r3, [pc, #0x30]
0034118c: push {r4, lr}
00341190: add r2, pc, r2
00341194: ldr r3, [r2, r3]
00341198: mov r4, r0
0034119c: add r2, r3, #0xe4
003411a0: add r1, r3, #8
003411a4: add r3, r3, #0xd8
003411a8: stm r0, {r1, r3}
003411ac: str r2, [r0, #0x24]
003411b0: bl #0x38d378
003411b4: mov r0, r4
003411b8: pop {r4, pc}
003411bc: rsbeq r3, r5, r0, lsl #18
003411c0: andeq r3, r0, r8, ror lr

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

_ZThn4_N13TriggerObject17DeclarePropertiesEv 0x399d68 8
00399d68: sub r0, r0, #4
00399d6c: b #0x399d70

_ZNK13TriggerObject9IsZonableEv 0x399320 8
00399320: mov r0, #1
00399324: bx lr

_ZN4Door25PopulateOutgoingNetStructEb 0x3e8088 76
003e8088: cmp r1, #0
003e808c: push {r4, lr}
003e8090: mov r4, r0
003e8094: bne #0x3e809c
003e8098: pop {r4, pc}
003e809c: ldr r3, [r0, #0x3a8]
003e80a0: ldrb r2, [r0, #0x4fd]
003e80a4: cmp r3, #1
003e80a8: cmpne r3, #3
003e80ac: movne r3, #0
003e80b0: moveq r3, #1
003e80b4: cmp r2, r3
003e80b8: beq #0x3e80c8
003e80bc: strb r3, [r0, #0x4fd]
003e80c0: add r0, r0, #0x4e0
003e80c4: bl #0x814f84
003e80c8: add r0, r4, #0x3b0
003e80cc: pop {r4, lr}
003e80d0: b #0x81347c

_ZN4Door5_OpenERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3e79a8 12
003e79a8: mov r0, r2
003e79ac: mov r1, #0
003e79b0: b #0x3e786c

_ZN4Door13NetStructDoorD0Ev 0x3e8798 132
003e8798: push {r4, r5, r6, lr}
003e879c: ldr r3, [pc, #0x6c]
003e87a0: ldr r2, [pc, #0x6c]
003e87a4: ldr r1, [pc, #0x6c]
003e87a8: add r3, pc, r3
003e87ac: mov r4, r0
003e87b0: ldr r1, [r3, r1]
003e87b4: ldr r0, [r0, #0x11c]
003e87b8: ldr r2, [r3, r2]
003e87bc: add r1, r1, #8
003e87c0: cmp r0, #0
003e87c4: add r2, r2, #8
003e87c8: str r2, [r4, #0x130]
003e87cc: str r1, [r4]
003e87d0: str r2, [r4, #0x170]
003e87d4: str r2, [r4, #0x150]
003e87d8: beq #0x3e8800
003e87dc: add r5, r4, #0x10c
003e87e0: mov r0, r5
003e87e4: ldr r1, [r4, #0x110]
003e87e8: bl #0x370fd0
003e87ec: mov r3, #0
003e87f0: str r5, [r4, #0x118]
003e87f4: str r3, [r4, #0x11c]
003e87f8: str r5, [r4, #0x114]
003e87fc: str r3, [r4, #0x110]
003e8800: mov r0, r4
003e8804: bl #0x310440
003e8808: mov r0, r4
003e880c: pop {r4, r5, r6, pc}
003e8810: subseq ip, sl, r8, ror #5
003e8814: andeq r1, r0, r8, lsr #1
003e8818: andeq r4, r0, r4, asr #7

_ZN5Decor8InitPostEv 0x388a98 136
00388a98: push {r4, r5, r6, lr}
00388a9c: mov r4, r0
00388aa0: bl #0x38be5c
00388aa4: ldr r0, [r4, #0x2d8]
00388aa8: ldr r5, [pc, #0x68]
00388aac: cmp r0, #0
00388ab0: add r5, pc, r5
00388ab4: beq #0x388b14
00388ab8: bl #0x470a54
00388abc: ldr r3, [r4, #0x2d8]
00388ac0: ldrb r3, [r3, #0x28]
00388ac4: cmp r3, #0
00388ac8: bne #0x388ad8
00388acc: mov r0, r4
00388ad0: pop {r4, r5, r6, lr}
00388ad4: b #0x388730
00388ad8: ldr r3, [pc, #0x3c]
00388adc: mov r1, #0
00388ae0: mov r0, #0x28
00388ae4: ldr r3, [r5, r3]
00388ae8: ldr r6, [r3, #0x44]
00388aec: bl #0x310570
00388af0: mov r1, r6
00388af4: mov r5, r0
00388af8: mov r2, r4
00388afc: bl #0x388a2c
00388b00: mov r0, r4
00388b04: mov r1, r5
00388b08: mov r2, #0
00388b0c: bl #0x394bf8
00388b10: b #0x388acc
00388b14: pop {r4, r5, r6, pc}
00388b18: rsbeq fp, r0, r0, ror #31
00388b1c: strdeq r3, r4, [r0], -r4

_ZN13TriggerObjectC1Ev 0x399df0 260
00399df0: push {r4, r5, r6, lr}
00399df4: mov r2, #0
00399df8: mov r3, #1
00399dfc: mov r1, #0x14
00399e00: ldr r5, [pc, #0xe4]
00399e04: mov r4, r0
00399e08: bl #0x398fd4
00399e0c: ldr r2, [pc, #0xdc]
00399e10: add r5, pc, r5
00399e14: add r3, r4, #0x710
00399e18: ldr r2, [r5, r2]
00399e1c: add r3, r3, #8
00399e20: mov r0, r3
00399e24: add ip, r2, #8
00399e28: add r1, r2, #0xf4
00399e2c: add r2, r2, #0xe8
00399e30: str ip, [r4]
00399e34: str r2, [r4, #4]
00399e38: str r1, [r4, #0x24]
00399e3c: str r3, [r4, #0x728]
00399e40: str r3, [r4, #0x72c]
00399e44: mov r1, #0x10
00399e48: bl #0x31167c
00399e4c: ldr r2, [r4, #0x728]
00399e50: mov r5, #0
00399e54: mvn r6, #0
00399e58: mov r3, r4
00399e5c: strb r5, [r2]
00399e60: str r6, [r3, #0x730]!
00399e64: add r3, r3, #4
00399e68: mov r0, r3
00399e6c: str r3, [r4, #0x744]
00399e70: str r3, [r4, #0x748]
00399e74: mov r1, #0x10
00399e78: bl #0x31167c
00399e7c: ldr r2, [r4, #0x744]
00399e80: add r3, r4, #0x750
00399e84: mov r0, r3
00399e88: strb r5, [r2]
00399e8c: mov r1, #0x10
00399e90: str r3, [r4, #0x760]
00399e94: str r3, [r4, #0x764]
00399e98: str r6, [r4, #0x74c]
00399e9c: bl #0x31167c
00399ea0: mov r3, r4
00399ea4: ldr r2, [r3, #0x760]!
00399ea8: mov r1, #0x10
00399eac: add r3, r3, #0xc
00399eb0: strb r5, [r2]
00399eb4: mov r0, r3
00399eb8: str r3, [r4, #0x77c]
00399ebc: str r3, [r4, #0x780]
00399ec0: bl #0x31167c
00399ec4: ldr r2, [r4, #0x77c]
00399ec8: mov r3, #1
00399ecc: mov r0, r4
00399ed0: strb r5, [r2]
00399ed4: strb r5, [r4, #0x84]
00399ed8: strb r3, [r4, #0x85]
00399edc: strb r5, [r4, #0x784]
00399ee0: str r5, [r4, #0x788]
00399ee4: strb r3, [r4, #0x28]
00399ee8: pop {r4, r5, r6, pc}
00399eec: subseq sl, pc, r0, lsl #25
00399ef0: andeq r2, r0, r4, ror #22

_ZThn36_N4DoorD0Ev 0x3e8964 8
003e8964: sub r0, r0, #0x24
003e8968: b #0x3e896c

_ZN11TriggerZone17DeclarePropertiesEv 0x39bfa4 260
0039bfa4: push {r4, r5, r6, lr}
0039bfa8: mov r5, r0
0039bfac: bl #0x398a88
0039bfb0: ldr r1, [pc, #0xcc]
0039bfb4: add r4, r5, #4
0039bfb8: add r6, r5, #0x710
0039bfbc: mov r0, r4
0039bfc0: add r2, r6, #8
0039bfc4: add r1, pc, r1
0039bfc8: bl #0x39bf10
0039bfcc: ldr r1, [pc, #0xb4]
0039bfd0: add r2, r6, #0xc
0039bfd4: mov r0, r4
0039bfd8: add r1, pc, r1
0039bfdc: bl #0x39bf10
0039bfe0: ldr r1, [pc, #0xa4]
0039bfe4: add r6, r5, #0x720
0039bfe8: mov r0, r4
0039bfec: mov r2, r6
0039bff0: add r1, pc, r1
0039bff4: bl #0x39bf10
0039bff8: ldr r1, [pc, #0x90]
0039bffc: add r2, r6, #4
0039c000: mov r0, r4
0039c004: add r1, pc, r1
0039c008: bl #0x33ef7c
0039c00c: ldr r1, [pc, #0x80]
0039c010: mov r0, r4
0039c014: add r2, r5, #0x740
0039c018: add r1, pc, r1
0039c01c: bl #0x33ef7c
0039c020: ldr r1, [pc, #0x70]
0039c024: add r2, r5, #0x750
0039c028: mov r0, r4
0039c02c: add r2, r2, #0xc
0039c030: add r1, pc, r1
0039c034: bl #0x33ef7c
0039c038: ldr r1, [pc, #0x5c]
0039c03c: add r2, r5, #0x770
0039c040: mov r0, r4
0039c044: add r2, r2, #8
0039c048: add r1, pc, r1
0039c04c: bl #0x33ef7c
0039c050: ldr r1, [pc, #0x48]
0039c054: add r2, r5, #0x790
0039c058: mov r0, r4
0039c05c: add r2, r2, #4
0039c060: add r1, pc, r1
0039c064: bl #0x33ef7c
0039c068: ldr r1, [pc, #0x34]
0039c06c: add r2, r5, #0x7b0
0039c070: mov r0, r4
0039c074: add r1, pc, r1
0039c078: add r2, r2, #0xc
0039c07c: pop {r4, r5, r6, lr}
0039c080: b #0x33ef7c
0039c084: subseq r6, r2, ip, ror #25
0039c088: subseq r6, r2, r0, ror #25
0039c08c: ldrsbeq r6, [r2], #-0xc8
0039c090: ldrsbeq r5, [r2], #-0x94
0039c094: ldrheq r6, [r2], #-0xc8
0039c098: ldrheq r6, [r2], #-0xc0
0039c09c: ldrheq r6, [r2], #-0xc0
0039c0a0: ldrheq r6, [r2], #-0xc8
0039c0a4: ldrheq r6, [r2], #-0xcc

_ZN11IStreamBase6readAsIN4Door10DoorStatesEEEvRT_ 0x3e79b4 176
003e79b4: str lr, [sp, #-4]!
003e79b8: mov r3, #0
003e79bc: sub sp, sp, #0xc
003e79c0: ldr ip, [r0]
003e79c4: mov r2, #4
003e79c8: mov lr, pc
003e79cc: ldr pc, [ip, #0x18]
003e79d0: ldr r3, [pc, #0x74]
003e79d4: cmp r0, #4
003e79d8: add r3, pc, r3
003e79dc: beq #0x3e7a0c
003e79e0: ldr r2, [pc, #0x68]
003e79e4: ldr r2, [r3, r2]
003e79e8: ldr r2, [r2]
003e79ec: cmp r2, #2
003e79f0: moveq r3, #0
003e79f4: streq r3, [r3]
003e79f8: beq #0x3e7a04
003e79fc: cmp r2, #1
003e7a00: beq #0x3e7a18
003e7a04: add sp, sp, #0xc
003e7a08: ldm sp!, {pc}
003e7a0c: cmp r1, #0
003e7a10: beq #0x3e7a04
003e7a14: b #0x3e79e0
003e7a18: ldr r0, [pc, #0x34]
003e7a1c: ldr r1, [pc, #0x34]
003e7a20: ldr r2, [pc, #0x34]
003e7a24: ldr r0, [r3, r0]
003e7a28: ldr r3, [pc, #0x30]
003e7a2c: mov ip, #0x45
003e7a30: add r1, pc, r1
003e7a34: add r2, pc, r2
003e7a38: add r3, pc, r3
003e7a3c: add r0, r0, #0xa8
003e7a40: str ip, [sp]
003e7a44: bl #0x30e004
003e7a48: b #0x3e7a04
003e7a4c: ldrheq sp, [sl], #-8
003e7a50: andeq r3, r0, r0, asr #19
003e7a54: andeq r1, r0, r0, asr #19
003e7a58: subeq r6, sp, r8, lsr #19
003e7a5c: subeq r6, sp, ip, asr #21
003e7a60: subeq r6, sp, r0, ror #21

_ZThn36_N10SpawnPointD0Ev 0x3ea318 8
003ea318: sub r0, r0, #0x24
003ea31c: b #0x3ea320

_ZThn4_N10SpawnPoint17DeclarePropertiesEv 0x3ea54c 8
003ea54c: sub r0, r0, #4
003ea550: b #0x3ea554

_ZNK5Decor11IsUpdatableEv 0x3883d4 8
003883d4: mov r0, #0
003883d8: bx lr

_ZNK11TriggerZone23SafeStartScriptOnlyOnceEi 0x39b2a0 84
0039b2a0: ldr r3, [pc, #0x44]
0039b2a4: cmn r1, #1
0039b2a8: push {r4, r5, r6, lr}
0039b2ac: add r3, pc, r3
0039b2b0: mov r4, r1
0039b2b4: mov r6, r0
0039b2b8: beq #0x39b2e8
0039b2bc: ldr r2, [pc, #0x2c]
0039b2c0: ldr r5, [r3, r2]
0039b2c4: mov r0, r5
0039b2c8: bl #0x455bec
0039b2cc: subs r3, r0, #0
0039b2d0: bne #0x39b2e8
0039b2d4: ldr r2, [r6, #0x64]
0039b2d8: mov r0, r5
0039b2dc: mov r1, r4
0039b2e0: pop {r4, r5, r6, lr}
0039b2e4: b #0x4605c0
0039b2e8: pop {r4, r5, r6, pc}
0039b2ec: subseq sb, pc, r4, ror #15
0039b2f0: andeq r1, r0, r0, lsr #20

_ZN4Door6_CloseERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3e777c 12
003e777c: mov r0, r2
003e7780: mov r1, #0
003e7784: b #0x3e763c

_ZThn36_N13TriggerObjectD1Ev 0x399934 8
00399934: sub r0, r0, #0x24
00399938: b #0x39993c

_ZThn4_N11TriggerZone17DeclarePropertiesEv 0x39bf9c 8
0039bf9c: sub r0, r0, #4
0039bfa0: b #0x39bfa4

_Z14GetNewInstanceI4DoorEP10ObjectBasev 0x340824 36
00340824: push {r4, lr}
00340828: mov r1, #0
0034082c: mov r0, #0x6d0
00340830: bl #0x310570
00340834: mov r1, #2
00340838: mov r4, r0
0034083c: bl #0x3e8274
00340840: mov r0, r4
00340844: pop {r4, pc}

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

_ZN10SpawnPointD0Ev 0x3ea320 28
003ea320: push {r4, lr}
003ea324: mov r4, r0
003ea328: bl #0x3ea2cc
003ea32c: mov r0, r4
003ea330: bl #0x310440
003ea334: mov r0, r4
003ea338: pop {r4, pc}

_ZThn36_N4DoorD1Ev 0x3e881c 8
003e881c: sub r0, r0, #0x24
003e8820: b #0x3e8824

_ZN11TriggerZoneC1EN10ObjectBase6GO_IDSE 0x39b890 256
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

_ZN7Structs13TriggerObjectD1Ev 0x4d9cc4 64
004d9cc4: push {r4, lr}
004d9cc8: ldr r3, [pc, #0x2c]
004d9ccc: ldr r2, [pc, #0x2c]
004d9cd0: mov r4, r0
004d9cd4: add r3, pc, r3
004d9cd8: ldr r0, [r0, #0xc]
004d9cdc: ldr r2, [r3, r2]
004d9ce0: cmp r0, #0
004d9ce4: add r2, r2, #8
004d9ce8: str r2, [r4]
004d9cec: beq #0x4d9cf4
004d9cf0: bl #0x310440
004d9cf4: mov r0, r4
004d9cf8: pop {r4, pc}
004d9cfc: strheq sl, [fp], #-0xdc
004d9d00: strdeq r1, r2, [r0], -r4

_ZN5DecorD0Ev 0x388eb0 72
00388eb0: ldr r2, [pc, #0x38]
00388eb4: ldr r3, [pc, #0x38]
00388eb8: push {r4, lr}
00388ebc: add r2, pc, r2
00388ec0: ldr r3, [r2, r3]
00388ec4: mov r4, r0
00388ec8: add r2, r3, #0xe4
00388ecc: add r1, r3, #8
00388ed0: add r3, r3, #0xd8
00388ed4: stm r0, {r1, r3}
00388ed8: str r2, [r0, #0x24]
00388edc: bl #0x38d378
00388ee0: mov r0, r4
00388ee4: bl #0x310440
00388ee8: mov r0, r4
00388eec: pop {r4, pc}

_ZThn36_N5DummyD1Ev 0x34117c 8
0034117c: sub r0, r0, #0x24
00341180: b #0x341184

_ZN11TriggerZone6UpdateEv 0x39b458 756
0039b458: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0039b45c: ldr r4, [pc, #0x2e0]
0039b460: ldr r6, [pc, #0x2e0]
0039b464: sub sp, sp, #4
0039b468: add r4, pc, r4
0039b46c: ldr r3, [r4, r6]
0039b470: mov r5, r0
0039b474: mov r1, #0
0039b478: ldr r7, [r3, #0x40]
0039b47c: mov r2, #1
0039b480: mov r0, r7
0039b484: bl #0x36e478
0039b488: ldr r3, [r0, #0x660]
0039b48c: cmp r3, #0
0039b490: beq #0x39b4ac
0039b494: mov r2, #0x1480
0039b498: ldrb r3, [r3, r2]
0039b49c: cmp r3, #0
0039b4a0: beq #0x39b4ac
0039b4a4: add sp, sp, #4
0039b4a8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0039b4ac: mov r0, r5
0039b4b0: bl #0x39b27c
0039b4b4: cmp r0, #0
0039b4b8: bne #0x39b4a4
0039b4bc: mov r0, r5
0039b4c0: bl #0x3987ac
0039b4c4: cmp r0, #0
0039b4c8: beq #0x39b4a4
0039b4cc: mov r0, r5
0039b4d0: bl #0x38ab60
0039b4d4: cmp r0, #0
0039b4d8: beq #0x39b4a4
0039b4dc: bl #0x7fd794
0039b4e0: ldrb r3, [r0, #5]
0039b4e4: cmp r3, #0
0039b4e8: bne #0x39b654
0039b4ec: mov r0, r5
0039b4f0: bl #0x398838
0039b4f4: ldr r7, [r7, #0x6c4]
0039b4f8: bl #0x7fd794
0039b4fc: ldrb r3, [r0, #5]
0039b500: cmp r3, #0
0039b504: bne #0x39b6b4
0039b508: mov r0, r5
0039b50c: bl #0x3987f8
0039b510: mov sb, r0
0039b514: str r0, [r5, #0x3c0]
0039b518: ldr r3, [r5, #0x774]
0039b51c: ldr sl, [r5, #0x790]
0039b520: cmn r3, #1
0039b524: movne fp, r3
0039b528: ldreq fp, [r5, #0x73c]
0039b52c: cmn sl, #1
0039b530: ldreq sl, [r5, #0x758]
0039b534: cmn r3, #1
0039b538: beq #0x39b6d4
0039b53c: cmp sb, r7
0039b540: movne r8, #0
0039b544: moveq r8, #1
0039b548: ldr r3, [r4, r6]
0039b54c: mov r1, #0
0039b550: mov r2, #1
0039b554: ldr r0, [r3, #0x40]
0039b558: bl #0x36e478
0039b55c: rsbs r6, sb, #1
0039b560: movlo r6, #0
0039b564: cmp r8, #0
0039b568: ldr r4, [r0, #0x660]
0039b56c: beq #0x39b670
0039b570: ldrb r3, [r5, #0x3bc]
0039b574: cmp r3, #0
0039b578: beq #0x39b678
0039b57c: cmp r4, #0
0039b580: moveq r6, r4
0039b584: beq #0x39b5d4
0039b588: mov r0, r5
0039b58c: mov r1, r4
0039b590: bl #0x38b518
0039b594: cmp r0, #0
0039b598: bne #0x39b670
0039b59c: ldrb r3, [r5, #0x3bc]
0039b5a0: mov r8, r0
0039b5a4: cmp r3, #0
0039b5a8: beq #0x39b5cc
0039b5ac: cmp r4, #0
0039b5b0: beq #0x39b64c
0039b5b4: mov r1, r4
0039b5b8: mov r0, r5
0039b5bc: bl #0x38b518
0039b5c0: cmp r0, #0
0039b5c4: moveq r6, #1
0039b5c8: bne #0x39b64c
0039b5cc: cmp r8, #0
0039b5d0: bne #0x39b678
0039b5d4: ldrb r3, [r5, #0x7b4]
0039b5d8: cmp r3, #0
0039b5dc: bne #0x39b684
0039b5e0: cmp r7, #1
0039b5e4: ble #0x39b4a4
0039b5e8: ldrb r3, [r5, #0x3bc]
0039b5ec: cmp r3, #0
0039b5f0: bne #0x39b4a4
0039b5f4: ldr r3, [r5, #0x774]
0039b5f8: cmn r3, #1
0039b5fc: beq #0x39b4a4
0039b600: ldr r1, [r5, #0x73c]
0039b604: cmn r1, #1
0039b608: beq #0x39b4a4
0039b60c: ldrb r3, [r5, #0x7b5]
0039b610: cmp sb, #0
0039b614: movle sb, #0
0039b618: movgt sb, #1
0039b61c: cmp r3, #0
0039b620: bne #0x39b6e4
0039b624: cmp sb, #0
0039b628: beq #0x39b4a4
0039b62c: mov r0, r5
0039b630: bl #0x39b2a0
0039b634: mov r3, #1
0039b638: mov r0, r5
0039b63c: strb r3, [r5, #0x7b5]
0039b640: add sp, sp, #4
0039b644: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0039b648: b #0x39b354
0039b64c: mov r6, #0
0039b650: b #0x39b5cc
0039b654: ldr r3, [r5]
0039b658: mov r0, r5
0039b65c: mov lr, pc
0039b660: ldr pc, [r3, #0x54]
0039b664: cmp r0, #0
0039b668: bne #0x39b4a4
0039b66c: b #0x39b4ec
0039b670: ldrb r3, [r5, #0x3bc]
0039b674: b #0x39b5a4
0039b678: ldrb r3, [r5, #0x7b4]
0039b67c: cmp r3, #0
0039b680: beq #0x39b70c
0039b684: cmp r6, #0
0039b688: beq #0x39b5e0
0039b68c: mov r3, #0
0039b690: cmn sl, #1
0039b694: strb r3, [r5, #0x7b4]
0039b698: beq #0x39b5e0
0039b69c: mov r0, r5
0039b6a0: mov r1, sl
0039b6a4: bl #0x39b2a0
0039b6a8: mov r0, r5
0039b6ac: bl #0x398794
0039b6b0: b #0x39b5e0
0039b6b4: ldr r3, [r5]
0039b6b8: mov r0, r5
0039b6bc: mov lr, pc
0039b6c0: ldr pc, [r3, #0x54]
0039b6c4: cmp r0, #0
0039b6c8: ldrne sb, [r5, #0x3c0]
0039b6cc: bne #0x39b518
0039b6d0: b #0x39b508
0039b6d4: cmp sb, #0
0039b6d8: movle r8, #0
0039b6dc: movgt r8, #1
0039b6e0: b #0x39b548
0039b6e4: cmp sb, #0
0039b6e8: bne #0x39b4a4
0039b6ec: mov r0, r5
0039b6f0: strb sb, [r5, #0x7b5]
0039b6f4: ldr r1, [r5, #0x758]
0039b6f8: bl #0x39b2a0
0039b6fc: mov r0, r5
0039b700: add sp, sp, #4
0039b704: pop {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0039b708: b #0x39b2f4
0039b70c: mov r3, #1
0039b710: strb r3, [r5, #0x7b4]
0039b714: mov r1, fp
0039b718: mov r0, r5
0039b71c: bl #0x39b2a0
0039b720: ldr r3, [r5, #0x3ac]
0039b724: mov r0, r5
0039b728: str r3, [r5, #0x3b8]
0039b72c: bl #0x39b2f4
0039b730: cmn sl, #1
0039b734: bne #0x39b5d4
0039b738: mov r0, r5
0039b73c: bl #0x398794
0039b740: b #0x39b5d4
0039b744: subseq sb, pc, r8, lsr #12
0039b748: strdeq r3, r4, [r0], -r4

_ZN14CheckpointZoneD1Ev 0x395b38 116
00395b38: push {r4, r5, r6, lr}
00395b3c: ldr r2, [pc, #0x60]
00395b40: ldr r3, [pc, #0x60]
00395b44: ldr r1, [r0, #0x398]
00395b48: add r2, pc, r2
00395b4c: ldr r3, [r2, r3]
00395b50: cmp r1, #0
00395b54: mov r4, r0
00395b58: add r2, r3, #0xf4
00395b5c: add r1, r3, #8
00395b60: add r3, r3, #0xe8
00395b64: stm r0, {r1, r3}
00395b68: str r2, [r0, #0x24]
00395b6c: beq #0x395b94
00395b70: add r5, r0, #0x388
00395b74: mov r0, r5
00395b78: ldr r1, [r4, #0x38c]
00395b7c: bl #0x395af8
00395b80: mov r3, #0
00395b84: str r5, [r4, #0x394]
00395b88: str r3, [r4, #0x398]
00395b8c: str r5, [r4, #0x390]
00395b90: str r3, [r4, #0x38c]
00395b94: mov r0, r4
00395b98: bl #0x397bc4
00395b9c: mov r0, r4
00395ba0: pop {r4, r5, r6, pc}
00395ba4: subseq lr, pc, r8, asr #30
00395ba8: andeq r2, r0, r4, ror #28

_ZN12SoundEmitterC2EN10ObjectBase6GO_IDSE 0x3955a8 140
003955a8: push {r4, r5, r6, lr}
003955ac: ldr r5, [pc, #0x78]
003955b0: mov r4, r0
003955b4: bl #0x38c398
003955b8: ldr r3, [pc, #0x70]
003955bc: add r5, pc, r5
003955c0: add r2, r4, #0x378
003955c4: ldr r3, [r5, r3]
003955c8: mov r0, r2
003955cc: str r2, [r4, #0x388]
003955d0: add ip, r3, #8
003955d4: add r1, r3, #0xe4
003955d8: add r3, r3, #0xd8
003955dc: str r3, [r4, #4]
003955e0: str r1, [r4, #0x24]
003955e4: str r2, [r4, #0x38c]
003955e8: str ip, [r4]
003955ec: mov r1, #0x10
003955f0: bl #0x31167c
003955f4: ldr r1, [r4, #0x388]
003955f8: mov r2, #0
003955fc: mov r3, #0xbf000000
00395600: strb r2, [r1]
00395604: add r3, r3, #0x800000
00395608: mvn r1, #0
0039560c: strb r2, [r4, #0x39c]
00395610: mov r2, #1
00395614: str r1, [r4, #0x390]
00395618: str r3, [r4, #0x398]
0039561c: strb r2, [r4, #0x85]
00395620: str r3, [r4, #0x394]
00395624: mov r0, r4
00395628: pop {r4, r5, r6, pc}
0039562c: ldrsbeq pc, [pc], #-0x44
00395630: andeq r1, r0, r8, asr fp

_ZN5DecorD1Ev 0x388428 64
00388428: ldr r2, [pc, #0x30]
0038842c: ldr r3, [pc, #0x30]
00388430: push {r4, lr}
00388434: add r2, pc, r2
00388438: ldr r3, [r2, r3]
0038843c: mov r4, r0
00388440: add r2, r3, #0xe4
00388444: add r1, r3, #8
00388448: add r3, r3, #0xd8
0038844c: stm r0, {r1, r3}
00388450: str r2, [r0, #0x24]
00388454: bl #0x38d378
00388458: mov r0, r4
0038845c: pop {r4, pc}
00388460: rsbeq ip, r0, ip, asr r6
00388464: andeq r2, r0, ip, lsl #22

_ZN13TriggerObject11DeserializeEP11IStreamBase 0x399580 284
00399580: push {r4, lr}
00399584: mov r4, r0
00399588: sub sp, sp, #8
0039958c: bl #0x39890c
00399590: mov r0, r4
00399594: bl #0x3993bc
00399598: ldr r3, [r4, #0x2d8]
0039959c: cmp r3, #0
003995a0: beq #0x3995ec
003995a4: mov r0, r4
003995a8: bl #0x3987ac
003995ac: subs lr, r0, #0
003995b0: beq #0x399628
003995b4: ldrb lr, [r4, #0x784]
003995b8: cmp lr, #0
003995bc: bne #0x3995f4
003995c0: ldr r2, [r4, #0x2d8]
003995c4: ldr r1, [pc, #0xc0]
003995c8: mov r3, lr
003995cc: ldr ip, [r2, #0x38]
003995d0: add r1, pc, r1
003995d4: mov r2, #1
003995d8: mov r0, ip
003995dc: ldr ip, [ip]
003995e0: str lr, [sp]
003995e4: mov lr, pc
003995e8: ldr pc, [ip, #0x20]
003995ec: add sp, sp, #8
003995f0: pop {r4, pc}
003995f4: ldr r3, [r4, #0x2d8]
003995f8: ldr r1, [pc, #0x90]
003995fc: mov r2, #0
00399600: ldr ip, [r3, #0x38]
00399604: add r1, pc, r1
00399608: mov r3, r2
0039960c: mov r0, ip
00399610: ldr ip, [ip]
00399614: str r2, [sp]
00399618: mov r2, #1
0039961c: mov lr, pc
00399620: ldr pc, [ip, #0x20]
00399624: b #0x3995ec
00399628: ldr r2, [r4, #0x2d8]
0039962c: ldr r1, [pc, #0x60]
00399630: mov r3, lr
00399634: ldr ip, [r2, #0x38]
00399638: add r1, pc, r1
0039963c: mov r2, #1
00399640: mov r0, ip
00399644: ldr ip, [ip]
00399648: str lr, [sp]
0039964c: mov lr, pc
00399650: ldr pc, [ip, #0x20]
00399654: subs ip, r0, #0
00399658: bne #0x3995ec
0039965c: ldr r3, [r4, #0x2d8]
00399660: ldr r1, [pc, #0x30]
00399664: mov r2, ip
00399668: ldr lr, [r3, #0x38]
0039966c: add r1, pc, r1
00399670: mov r3, ip
00399674: ldr r4, [lr]
00399678: mov r0, lr
0039967c: str ip, [sp]
00399680: mov lr, pc
00399684: ldr pc, [r4, #0x20]
00399688: b #0x3995ec
0039968c: subseq sb, r2, r8, ror #9
00399690: subseq r8, r2, ip, lsr #25

_ZN13TriggerObjectD0Ev 0x3999d8 28
003999d8: push {r4, lr}
003999dc: mov r4, r0
003999e0: bl #0x39993c
003999e4: mov r0, r4
003999e8: bl #0x310440
003999ec: mov r0, r4
003999f0: pop {r4, pc}

_ZN4DoorD0Ev 0x3e896c 28
003e896c: push {r4, lr}
003e8970: mov r4, r0
003e8974: bl #0x3e8824
003e8978: mov r0, r4
003e897c: bl #0x310440
003e8980: mov r0, r4
003e8984: pop {r4, pc}

_ZN4Door17DeclarePropertiesEv 0x3e8990 428
003e8990: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e8994: ldr r4, [pc, #0x184]
003e8998: ldr fp, [pc, #0x184]
003e899c: sub sp, sp, #0x3c
003e89a0: add r4, pc, r4
003e89a4: ldr r3, [r4, fp]
003e89a8: add r7, sp, #0x1c
003e89ac: mov sb, r0
003e89b0: ldr r3, [r3]
003e89b4: mov r5, #0
003e89b8: add r8, sp, #4
003e89bc: str r3, [sp, #0x34]
003e89c0: bl #0x397df8
003e89c4: mov r0, r7
003e89c8: mov r1, #0x10
003e89cc: str r7, [sp, #0x2c]
003e89d0: str r7, [sp, #0x30]
003e89d4: bl #0x31167c
003e89d8: ldr r3, [sp, #0x2c]
003e89dc: mov r0, r8
003e89e0: ldr sl, [pc, #0x140]
003e89e4: strb r5, [r3]
003e89e8: ldr r2, [sp, #0x2c]
003e89ec: ldr r1, [sp, #0x30]
003e89f0: str r8, [sp, #0x14]
003e89f4: str r8, [sp, #0x18]
003e89f8: bl #0x3116e8
003e89fc: mov r1, r5
003e8a00: mov r0, #0x38
003e8a04: bl #0x310570
003e8a08: ldr r3, [pc, #0x11c]
003e8a0c: add sl, pc, sl
003e8a10: mov r5, r0
003e8a14: ldr r3, [r4, r3]
003e8a18: mov r1, sl
003e8a1c: mov r2, sp
003e8a20: add r3, r3, #8
003e8a24: str r3, [r0], #8
003e8a28: bl #0x3140ec
003e8a2c: ldr r3, [pc, #0xfc]
003e8a30: add r6, sb, #4
003e8a34: add r2, sb, #0x388
003e8a38: ldr r3, [r4, r3]
003e8a3c: mov r0, r5
003e8a40: rsb r2, r6, r2
003e8a44: add r3, r3, #8
003e8a48: str r2, [r5, #4]
003e8a4c: str r3, [r0], #0x20
003e8a50: str r0, [r5, #0x30]
003e8a54: str r0, [r5, #0x34]
003e8a58: ldr r1, [sp, #0x18]
003e8a5c: ldr r2, [sp, #0x14]
003e8a60: bl #0x3116e8
003e8a64: mov r0, r6
003e8a68: mov r1, sl
003e8a6c: mov r2, r5
003e8a70: bl #0x513ce4
003e8a74: ldr r0, [sp, #0x18]
003e8a78: cmp r0, r8
003e8a7c: beq #0x3e8a9c
003e8a80: cmp r0, #0
003e8a84: beq #0x3e8a9c
003e8a88: ldr r1, [sp, #4]
003e8a8c: rsb r1, r0, r1
003e8a90: cmp r1, #0x80
003e8a94: bhi #0x3e8b14
003e8a98: bl #0x708f00
003e8a9c: ldr r0, [sp, #0x30]
003e8aa0: cmp r0, r7
003e8aa4: beq #0x3e8ac4
003e8aa8: cmp r0, #0
003e8aac: beq #0x3e8ac4
003e8ab0: ldr r1, [sp, #0x1c]
003e8ab4: rsb r1, r0, r1
003e8ab8: cmp r1, #0x80
003e8abc: bhi #0x3e8b0c
003e8ac0: bl #0x708f00
003e8ac4: ldr r1, [pc, #0x68]
003e8ac8: add sb, sb, #0x3a4
003e8acc: mov r0, r6
003e8ad0: add r1, pc, r1
003e8ad4: mov r2, sb
003e8ad8: bl #0x3e7ffc
003e8adc: ldr r1, [pc, #0x54]
003e8ae0: add r2, sb, #1
003e8ae4: mov r0, r6
003e8ae8: add r1, pc, r1
003e8aec: bl #0x3e7ffc
003e8af0: ldr r3, [r4, fp]
003e8af4: ldr r2, [sp, #0x34]
003e8af8: ldr r3, [r3]
003e8afc: cmp r2, r3
003e8b00: bne #0x3e8b1c
003e8b04: add sp, sp, #0x3c
003e8b08: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e8b0c: bl #0x310440
003e8b10: b #0x3e8ac4
003e8b14: bl #0x310440
003e8b18: b #0x3e8a9c
003e8b1c: bl #0x30e310
003e8b20: ldrsheq ip, [sl], #-0
003e8b24: andeq r4, r0, ip, lsr #1
003e8b28: subeq sl, sp, ip, asr #2
003e8b2c: andeq r2, r0, r0, lsr r3
003e8b30: muleq r0, r4, r4
003e8b34: strheq sl, [sp], #-0x48
003e8b38: subeq sp, sp, r0, lsl r6

_ZThn36_N15QuestMoveInZoneD0Ev 0x3962cc 8
003962cc: sub r0, r0, #0x24
003962d0: b #0x3962d4

_ZNK13TriggerObject10IsAnimatedEv 0x399328 8
00399328: mov r0, #1
0039932c: bx lr

_ZN7Structs19GetMemberIDByStringINS_13TriggerObjectEEEiPKc 0x4ae044 88
004ae044: ldr r3, [pc, #0x48]
004ae048: ldr r2, [pc, #0x48]
004ae04c: push {r4, r5, r6, lr}
004ae050: add r3, pc, r3
004ae054: mov r6, r0
004ae058: ldr r5, [r3, r2]
004ae05c: mov r4, #0
004ae060: ldr r1, [r5, #0x14]
004ae064: mov r0, r6
004ae068: bl #0x30e31c
004ae06c: cmp r0, #0
004ae070: beq #0x4ae08c
004ae074: add r4, r4, #1
004ae078: cmp r4, #4
004ae07c: add r5, r5, #0x18
004ae080: bne #0x4ae060
004ae084: mvn r0, #0
004ae088: pop {r4, r5, r6, pc}
004ae08c: mov r0, r4
004ae090: pop {r4, r5, r6, pc}
004ae094: subeq r6, lr, r0, asr #20
004ae098: andeq r3, r0, r4, asr #32

_ZN4Door13NetStructDoorD1Ev 0x3e83d4 124
003e83d4: push {r4, r5, r6, lr}
003e83d8: ldr r3, [pc, #0x64]
003e83dc: ldr r2, [pc, #0x64]
003e83e0: ldr r1, [pc, #0x64]
003e83e4: add r3, pc, r3
003e83e8: mov r4, r0
003e83ec: ldr r1, [r3, r1]
003e83f0: ldr r0, [r0, #0x11c]
003e83f4: ldr r2, [r3, r2]
003e83f8: add r1, r1, #8
003e83fc: cmp r0, #0
003e8400: add r2, r2, #8
003e8404: str r2, [r4, #0x130]
003e8408: str r1, [r4]
003e840c: str r2, [r4, #0x170]
003e8410: str r2, [r4, #0x150]
003e8414: beq #0x3e843c
003e8418: add r5, r4, #0x10c
003e841c: mov r0, r5
003e8420: ldr r1, [r4, #0x110]
003e8424: bl #0x370fd0
003e8428: mov r3, #0
003e842c: str r5, [r4, #0x118]
003e8430: str r3, [r4, #0x11c]
003e8434: str r5, [r4, #0x114]
003e8438: str r3, [r4, #0x110]
003e843c: mov r0, r4
003e8440: pop {r4, r5, r6, pc}
003e8444: subseq ip, sl, ip, lsr #13
003e8448: andeq r1, r0, r8, lsr #1
003e844c: andeq r4, r0, r4, asr #7

_ZNK5Dummy11IsUpdatableEv 0x340134 8
00340134: mov r0, #0
00340138: bx lr

_ZN14CheckpointZone17OnCollisionBeginsEP10GameObject 0x395f04 536
00395f04: push {r4, r5, r6, r7, lr}
00395f08: ldr r4, [pc, #0x1e4]
00395f0c: subs r5, r1, #0
00395f10: sub sp, sp, #0x24
00395f14: mov r6, r0
00395f18: add r4, pc, r4
00395f1c: beq #0x39604c
00395f20: ldr r3, [r5]
00395f24: mov r0, r5
00395f28: mov lr, pc
00395f2c: ldr pc, [r3, #0x24]
00395f30: cmp r0, #0
00395f34: bne #0x395f40
00395f38: add sp, sp, #0x24
00395f3c: pop {r4, r5, r6, r7, pc}
00395f40: add r7, sp, #8
00395f44: mov r1, r5
00395f48: mov r0, r7
00395f4c: bl #0x33dd2c
00395f50: mov r0, r7
00395f54: bl #0x33ff54
00395f58: ldr r3, [pc, #0x198]
00395f5c: str r0, [sp, #0x1c]
00395f60: mov r1, #0
00395f64: ldr r7, [r4, r3]
00395f68: mov r2, #1
00395f6c: ldr r0, [r7, #0x40]
00395f70: bl #0x36e478
00395f74: ldr r5, [r0, #0x660]
00395f78: mov r0, r7
00395f7c: bl #0x31f594
00395f80: subs r7, r0, #0
00395f84: beq #0x3960a0
00395f88: ldr r3, [sp, #0x1c]
00395f8c: mov r0, r3
00395f90: ldr r3, [r3]
00395f94: mov lr, pc
00395f98: ldr pc, [r3, #0x28]
00395f9c: cmp r0, #0
00395fa0: bne #0x396014
00395fa4: ldr r3, [r6, #0x38c]
00395fa8: add r1, r6, #0x388
00395fac: cmp r3, #0
00395fb0: beq #0x395ff8
00395fb4: ldr ip, [sp, #0x1c]
00395fb8: mov r0, r1
00395fbc: b #0x395fc4
00395fc0: mov r3, r2
00395fc4: ldr r2, [r3, #0x10]
00395fc8: cmp ip, r2
00395fcc: ldrhi r2, [r3, #0xc]
00395fd0: ldrls r2, [r3, #8]
00395fd4: movhi r3, r0
00395fd8: mov r0, r3
00395fdc: cmp r2, #0
00395fe0: bne #0x395fc0
00395fe4: cmp r1, r3
00395fe8: beq #0x396004
00395fec: ldr r2, [r3, #0x10]
00395ff0: cmp ip, r2
00395ff4: bhs #0x395ffc
00395ff8: mov r3, r1
00395ffc: cmp r1, r3
00396000: bne #0x395f38
00396004: add r0, sp, #0x14
00396008: add r2, sp, #0x1c
0039600c: bl #0x395d84
00396010: b #0x395f38
00396014: ldr r3, [sp, #0x1c]
00396018: cmp r3, r5
0039601c: bne #0x395fa4
00396020: add r1, r5, #0x1440
00396024: add r4, r6, #0x160
00396028: add r1, r1, #0x28
0039602c: mov r0, r4
00396030: bl #0x312b6c
00396034: subs r2, r0, #0
00396038: bne #0x395fa4
0039603c: mov r0, r7
00396040: mov r1, r4
00396044: bl #0x3f04b4
00396048: b #0x395fa4
0039604c: ldr r3, [pc, #0xa8]
00396050: ldr r3, [r4, r3]
00396054: ldr r3, [r3]
00396058: cmp r3, #2
0039605c: streq r5, [r5]
00396060: beq #0x395f20
00396064: cmp r3, #1
00396068: bne #0x395f20
0039606c: ldr r0, [pc, #0x8c]
00396070: ldr r1, [pc, #0x8c]
00396074: ldr r2, [pc, #0x8c]
00396078: ldr r0, [r4, r0]
0039607c: ldr r3, [pc, #0x88]
00396080: mov ip, #0x26
00396084: add r1, pc, r1
00396088: add r2, pc, r2
0039608c: add r3, pc, r3
00396090: add r0, r0, #0xa8
00396094: str ip, [sp]
00396098: bl #0x30e004
0039609c: b #0x395f20
003960a0: ldr r3, [pc, #0x54]
003960a4: ldr r3, [r4, r3]
003960a8: ldr r3, [r3]
003960ac: cmp r3, #2
003960b0: streq r7, [r7]
003960b4: beq #0x395f88
003960b8: cmp r3, #1
003960bc: bne #0x395f88
003960c0: ldr r0, [pc, #0x38]
003960c4: ldr r1, [pc, #0x44]
003960c8: ldr r2, [pc, #0x44]
003960cc: ldr r0, [r4, r0]
003960d0: ldr r3, [pc, #0x40]
003960d4: mov ip, #0x2e
003960d8: add r1, pc, r1
003960dc: add r2, pc, r2
003960e0: add r3, pc, r3
003960e4: add r0, r0, #0xa8
003960e8: str ip, [sp]
003960ec: bl #0x30e004
003960f0: b #0x395f88
003960f4: subseq lr, pc, r8, ror fp
003960f8: strdeq r3, r4, [r0], -r4
003960fc: andeq r3, r0, r0, asr #19
00396100: andeq r1, r0, r0, asr #19
00396104: subseq r8, r2, r4, asr r3
00396108: subseq ip, r2, r0, lsl r3
0039610c: subseq ip, r2, ip, lsr r8
00396110: subseq r8, r2, r0, lsl #6
00396114: ldrsheq r8, [r4], #-0x3c
00396118: subseq ip, r2, r8, ror #15

_ZN4DoorC1EN10ObjectBase6GO_IDSE 0x3e8274 176
003e8274: push {r4, r5, r6, r7, r8, lr}
003e8278: mov r2, #0
003e827c: mov r3, #1
003e8280: ldr r5, [pc, #0x94]
003e8284: mov r4, r0
003e8288: bl #0x397ca0
003e828c: ldr r3, [pc, #0x8c]
003e8290: add r5, pc, r5
003e8294: add r2, r4, #0x388
003e8298: ldr r3, [r5, r3]
003e829c: mov r0, r2
003e82a0: str r2, [r4, #0x398]
003e82a4: add ip, r3, #8
003e82a8: add r1, r3, #0xf4
003e82ac: add r3, r3, #0xe8
003e82b0: str ip, [r4]
003e82b4: str r3, [r4, #4]
003e82b8: str r1, [r4, #0x24]
003e82bc: str r2, [r4, #0x39c]
003e82c0: mov r1, #0x10
003e82c4: bl #0x31167c
003e82c8: ldr r2, [r4, #0x398]
003e82cc: mov r3, #0
003e82d0: mov r7, #1
003e82d4: add r6, r4, #0x3b0
003e82d8: strb r3, [r2]
003e82dc: add r5, r4, #0x540
003e82e0: strb r3, [r4, #0x3ac]
003e82e4: strb r3, [r4, #0x3a4]
003e82e8: str r3, [r4, #0x3a8]
003e82ec: strb r7, [r4, #0x3a5]
003e82f0: mov r0, r6
003e82f4: bl #0x3e80d4
003e82f8: mov r0, r5
003e82fc: bl #0x3e80d4
003e8300: mov r3, #3
003e8304: strb r7, [r4, #0x28]
003e8308: str r6, [r4, #0x100]
003e830c: str r5, [r4, #0x104]
003e8310: strb r3, [r4, #0xf8]
003e8314: mov r0, r4
003e8318: pop {r4, r5, r6, r7, r8, pc}
003e831c: subseq ip, sl, r0, lsl #16
003e8320: andeq r4, r0, r4, lsl #19

_ZN5DummyD0Ev 0x341960 72
00341960: ldr r2, [pc, #0x38]
00341964: ldr r3, [pc, #0x38]
00341968: push {r4, lr}
0034196c: add r2, pc, r2
00341970: ldr r3, [r2, r3]
00341974: mov r4, r0
00341978: add r2, r3, #0xe4
0034197c: add r1, r3, #8
00341980: add r3, r3, #0xd8
00341984: stm r0, {r1, r3}
00341988: str r2, [r0, #0x24]
0034198c: bl #0x38d378
00341990: mov r0, r4
00341994: bl #0x310440
00341998: mov r0, r4
0034199c: pop {r4, pc}
003419a0: rsbeq r3, r5, r4, lsr #2
003419a4: andeq r3, r0, r8, ror lr

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

_ZN12SoundEmitterC1EN10ObjectBase6GO_IDSE 0x39551c 140
0039551c: push {r4, r5, r6, lr}
00395520: ldr r5, [pc, #0x78]
00395524: mov r4, r0
00395528: bl #0x38c398
0039552c: ldr r3, [pc, #0x70]
00395530: add r5, pc, r5
00395534: add r2, r4, #0x378
00395538: ldr r3, [r5, r3]
0039553c: mov r0, r2
00395540: str r2, [r4, #0x388]
00395544: add ip, r3, #8
00395548: add r1, r3, #0xe4
0039554c: add r3, r3, #0xd8
00395550: str r3, [r4, #4]
00395554: str r1, [r4, #0x24]
00395558: str r2, [r4, #0x38c]
0039555c: str ip, [r4]
00395560: mov r1, #0x10
00395564: bl #0x31167c
00395568: ldr r1, [r4, #0x388]
0039556c: mov r2, #0
00395570: mov r3, #0xbf000000
00395574: strb r2, [r1]
00395578: add r3, r3, #0x800000
0039557c: mvn r1, #0
00395580: strb r2, [r4, #0x39c]
00395584: mov r2, #1
00395588: str r1, [r4, #0x390]
0039558c: str r3, [r4, #0x398]
00395590: strb r2, [r4, #0x85]
00395594: str r3, [r4, #0x394]
00395598: mov r0, r4
0039559c: pop {r4, r5, r6, pc}
003955a0: subseq pc, pc, r0, ror #10
003955a4: andeq r1, r0, r8, asr fp

_ZNK12SoundEmitter9IsZonableEv 0x394ec4 8
00394ec4: mov r0, #1
00394ec8: bx lr

_ZN21DestructibleContainer9DoEffectsEv 0x3a0d68 4
003a0d68: bx lr

_ZN7Structs13TriggerObjectD2Ev 0x4d9d20 64
004d9d20: push {r4, lr}
004d9d24: ldr r3, [pc, #0x2c]
004d9d28: ldr r2, [pc, #0x2c]
004d9d2c: mov r4, r0
004d9d30: add r3, pc, r3
004d9d34: ldr r0, [r0, #0xc]
004d9d38: ldr r2, [r3, r2]
004d9d3c: cmp r0, #0
004d9d40: add r2, r2, #8
004d9d44: str r2, [r4]
004d9d48: beq #0x4d9d50
004d9d4c: bl #0x310440
004d9d50: mov r0, r4
004d9d54: pop {r4, pc}
004d9d58: subeq sl, fp, r0, ror #26
004d9d5c: strdeq r1, r2, [r0], -r4

_ZNK4Door9IsZonableEv 0x3e74b0 8
003e74b0: mov r0, #1
003e74b4: bx lr

_ZNK21DestructibleContainer18GetInteractionTypeEP10GameObject 0x3a0d60 8
003a0d60: mov r0, #8
003a0d64: bx lr

_ZThn36_N15QuestMoveInZoneD1Ev 0x396284 8
00396284: sub r0, r0, #0x24
00396288: b #0x39628c

_ZNK5Dummy10IsAnimatedEv 0x340144 8
00340144: mov r0, #0
00340148: bx lr