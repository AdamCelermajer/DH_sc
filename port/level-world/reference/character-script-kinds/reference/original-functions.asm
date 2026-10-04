
# _ZN10AISDefault21OnTargetInRangedRangeEv
003dc560: push     {r4, r5, r6, lr}
003dc564: mov      r4, r0
003dc568: ldr      r0, [r0, #0x98]
003dc56c: sub      sp, sp, #0x10
003dc570: mov      r1, #0
003dc574: add      r0, r0, #0x3c8
003dc578: bl       #0x3d574c
003dc57c: subs     r5, r0, #0
003dc580: bne      #0x3dc5a0
003dc584: ldr      r0, [r4, #0x98]
003dc588: add      r0, r0, #0x3c8
003dc58c: bl       #0x3d49d0
003dc590: cmp      r0, #0
003dc594: bne      #0x3dc5c0
003dc598: add      sp, sp, #0x10
003dc59c: pop      {r4, r5, r6, pc}
003dc5a0: ldr      r3, [r4, #0x98]
003dc5a4: ldr      r0, [r3, #0x378]
003dc5a8: bl       #0x40559c
003dc5ac: ldr      r3, [r4, #0x98]
003dc5b0: ldr      r1, [r3, #0x408]
003dc5b4: ldr      r0, [r3, #0x378]
003dc5b8: bl       #0x405b04
003dc5bc: b        #0x3dc598
003dc5c0: ldr      r0, [r4, #0x98]
003dc5c4: mov      r1, r5
003dc5c8: add      r0, r0, #0x4f0
003dc5cc: add      r0, r0, #0xc
003dc5d0: bl       #0x3c0260
003dc5d4: cmp      r0, #0
003dc5d8: beq      #0x3dc630
003dc5dc: ldr      r3, [r4, #0x98]
003dc5e0: ldr      r6, [r3, #0x378]
003dc5e4: add      r5, sp, #4
003dc5e8: ldr      r1, [r3, #0x408]
003dc5ec: mov      r0, r5
003dc5f0: bl       #0x38b228
003dc5f4: mov      r0, r6
003dc5f8: mov      r1, r5
003dc5fc: bl       #0x4054e4
003dc600: ldr      r0, [r4, #0x98]
003dc604: mov      r2, r0
003dc608: ldr      r3, [r2, #0x200]!
003dc60c: cmp      r3, r2
003dc610: beq      #0x3dc66c
003dc614: ldr      r3, [r3]
003dc618: cmp      r2, r3
003dc61c: beq      #0x3dc598
003dc620: ldr      r3, [r3]
003dc624: cmp      r2, r3
003dc628: bne      #0x3dc614
003dc62c: b        #0x3dc598
003dc630: ldr      r0, [r4, #0x98]
003dc634: add      r0, r0, #0x4f0
003dc638: add      r0, r0, #0xc
003dc63c: bl       #0x3c02d0
003dc640: cmp      r0, #0
003dc644: bne      #0x3dc5dc
003dc648: ldr      r3, [r4, #0x98]
003dc64c: mov      r1, r3
003dc650: ldr      r2, [r1, #0x200]!
003dc654: cmp      r2, r1
003dc658: beq      #0x3dc5e0
003dc65c: ldr      r2, [r2]
003dc660: cmp      r1, r2
003dc664: bne      #0x3dc65c
003dc668: b        #0x3dc598
003dc66c: mov      r1, #0
003dc670: mov      r2, r1
003dc674: add      r0, r0, #0x3c8
003dc678: bl       #0x3d6890
003dc67c: ldr      r0, [r4, #0x98]
003dc680: add      r0, r0, #0x3c8
003dc684: bl       #0x3d49c4
003dc688: ldr      r3, [r4, #0x98]
003dc68c: mov      r2, #0
003dc690: strb     r2, [r3, #0x412]
003dc694: b        #0x3dc598

# _ZN11AISExternal6OnInitEv
003dcea4: push     {r4, lr}
003dcea8: mov      r4, r0
003dceac: bl       #0x3dbe78
003dceb0: ldr      r1, [pc, #0xc]
003dceb4: mov      r0, r4
003dceb8: add      r1, pc, r1
003dcebc: pop      {r4, lr}
003dcec0: b        #0x37c514
003dcec4: subeq    r8, lr, r0, ror #24

# _ZN11AISExternalC1Eb
003dd0e4: push     {r4, r5, r6, lr}
003dd0e8: ldr      r5, [pc, #0x30]
003dd0ec: mov      r4, r0
003dd0f0: bl       #0x3d8fb0
003dd0f4: ldr      r2, [pc, #0x28]
003dd0f8: add      r5, pc, r5
003dd0fc: mov      r3, #0
003dd100: ldr      r2, [r5, r2]
003dd104: str      r3, [r4, #0xc0]
003dd108: str      r3, [r4, #0xb8]
003dd10c: add      r2, r2, #8
003dd110: str      r2, [r4]
003dd114: str      r3, [r4, #0xbc]
003dd118: mov      r0, r4
003dd11c: pop      {r4, r5, r6, pc}

# _ZN10AISDefault7OnTimerEPv
003dbedc: bx       lr

# _ZN10AISDefault6OnInitEv
003dbe78: bx       lr

# _ZN6CharAI16StepCreateScriptEv
003cf04c: push     {r4, r5, r6, r7, r8, sl, lr}
003cf050: ldr      r4, [pc, #0x17c]
003cf054: ldr      r6, [pc, #0x17c]
003cf058: ldr      r2, [pc, #0x17c]
003cf05c: add      r4, pc, r4
003cf060: ldr      r3, [r4, r6]
003cf064: ldr      r2, [r4, r2]
003cf068: sub      sp, sp, #0x44
003cf06c: ldr      r3, [r3]
003cf070: mov      r5, r0
003cf074: ldr      r0, [r0, #4]
003cf078: ldr      r7, [r2]
003cf07c: str      r3, [sp, #0x3c]
003cf080: bl       #0x3a2fec
003cf084: mov      r3, #0x44
003cf088: mla      r7, r3, r0, r7
003cf08c: ldr      r3, [r7, #0x28]
003cf090: cmp      r3, #0
003cf094: beq      #0x3cf124
003cf098: ldr      r3, [pc, #0x140]
003cf09c: add      r8, sp, #0x24
003cf0a0: ldr      sl, [r4, r3]
003cf0a4: mov      r0, sl
003cf0a8: bl       #0x337888
003cf0ac: ldr      r1, [pc, #0x130]
003cf0b0: add      r2, sp, #8
003cf0b4: mov      r0, r8
003cf0b8: add      r1, pc, r1
003cf0bc: bl       #0x3140ec
003cf0c0: mov      r0, sl
003cf0c4: mov      r1, r8
003cf0c8: bl       #0x337a88
003cf0cc: ldr      r0, [sp, #0x38]
003cf0d0: cmp      r0, r8
003cf0d4: beq      #0x3cf0f4
003cf0d8: cmp      r0, #0
003cf0dc: beq      #0x3cf0f4
003cf0e0: ldr      r1, [sp, #0x24]
003cf0e4: rsb      r1, r0, r1
003cf0e8: cmp      r1, #0x80
003cf0ec: bhi      #0x3cf1c8
003cf0f0: bl       #0x708f00
003cf0f4: ldr      r1, [r7, #0x2c]
003cf0f8: mov      r0, r5
003cf0fc: bl       #0x3ceeb0
003cf100: mov      r3, #1
003cf104: strb     r3, [r5, #0x2c]
003cf108: ldr      r3, [r4, r6]
003cf10c: ldr      r2, [sp, #0x3c]
003cf110: ldr      r3, [r3]
003cf114: cmp      r2, r3
003cf118: bne      #0x3cf1d0
003cf11c: add      sp, sp, #0x44
003cf120: pop      {r4, r5, r6, r7, r8, sl, pc}
003cf124: ldr      r3, [pc, #0xb4]
003cf128: add      r7, sp, #0xc
003cf12c: ldr      r8, [r4, r3]
003cf130: mov      r0, r8
003cf134: bl       #0x337888
003cf138: ldr      r1, [pc, #0xa8]
003cf13c: add      r2, sp, #4
003cf140: mov      r0, r7
003cf144: add      r1, pc, r1
003cf148: bl       #0x3140ec
003cf14c: mov      r0, r8
003cf150: mov      r1, r7
003cf154: bl       #0x337a88
003cf158: ldr      r0, [sp, #0x20]
003cf15c: cmp      r0, r7
003cf160: beq      #0x3cf180
003cf164: cmp      r0, #0
003cf168: beq      #0x3cf180
003cf16c: ldr      r1, [sp, #0xc]
003cf170: rsb      r1, r0, r1
003cf174: cmp      r1, #0x80
003cf178: bhi      #0x3cf1c0
003cf17c: bl       #0x708f00
003cf180: ldr      r3, [r5, #4]
003cf184: ldr      r1, [pc, #0x60]
003cf188: ldr      r0, [r3, #0x5c]
003cf18c: add      r1, pc, r1
003cf190: bl       #0x30e31c
003cf194: cmp      r0, #0
003cf198: beq      #0x3cf1b4
003cf19c: mov      r0, r5
003cf1a0: bl       #0x3cce14
003cf1a4: mov      r3, #0
003cf1a8: str      r3, [r5, #0x30]
003cf1ac: strb     r3, [r5, #0x2c]
003cf1b0: b        #0x3cf108
003cf1b4: mov      r0, r5
003cf1b8: bl       #0x3cd11c
003cf1bc: b        #0x3cf1a4
003cf1c0: bl       #0x310440
003cf1c4: b        #0x3cf180
003cf1c8: bl       #0x310440
003cf1cc: b        #0x3cf0f4
003cf1d0: bl       #0x30e310
003cf1d4: subseq   r5, ip, r4, lsr sl
003cf1d8: andeq    r4, r0, ip, lsr #1
003cf1dc: andeq    r0, r0, r8, asr r7
003cf1e0: andeq    r0, r0, r4, lsl #17
003cf1e4: subeq    r4, pc, r8, asr sp
003cf1e8: subeq    r4, pc, ip, asr #25
003cf1ec: subeq    r1, pc, ip, lsr r2

# _ZN11AISExternal11OnTerminateEv
003dce34: ldr      r1, [pc, #4]
003dce38: add      r1, pc, r1
003dce3c: b        #0x37c514
003dce40: subeq    r8, lr, r0, lsr #25

# _ZN10AISDefault11OnPreAttackEi
003dbef8: bx       lr

# _ZN11AISExternal18OnTargetOutOfSightEv
003dce14: ldr      r1, [pc, #4]
003dce18: add      r1, pc, r1
003dce1c: b        #0x37c514
003dce20: subeq    r8, lr, r8, lsr #25

# _ZN10AISDefault16OnNeutralSpottedEP9Character
003dbe9c: bx       lr

# _ZN10AISDefault14OnCollisionEndEP10GameObjectb
003dbf40: ldr      r3, [pc, #0x18]
003dbf44: ldr      r2, [pc, #0x18]
003dbf48: add      r3, pc, r3
003dbf4c: ldr      r2, [r3, r2]
003dbf50: ldr      r3, [r2]
003dbf54: sub      r3, r3, #1
003dbf58: str      r3, [r2]
003dbf5c: bx       lr
003dbf60: subseq   r8, fp, r8, asr #22
003dbf64: andeq    r1, r0, r8, asr #12

# _ZN10AISDefault13OnPreInteractEv
003dbef0: mov      r0, #1
003dbef4: bx       lr

# _ZN10AISDefault13OnFearExpiredEv
003dbee4: bx       lr

# _ZN10AISDefault18OnMasterOutOfSightEv
003dbec4: bx       lr

# _ZN11AISExternal11OnInitFinalEv
003dce44: ldr      r1, [pc, #4]
003dce48: add      r1, pc, r1
003dce4c: b        #0x37c514
003dce50: subeq    r8, lr, r0, lsr #25

# _ZN11AISExternal15OnTargetInSightEv
003dce04: ldr      r1, [pc, #4]
003dce08: add      r1, pc, r1
003dce0c: b        #0x37c514
003dce10: subeq    r8, lr, r8, lsr #25

# _ZN10AISDefault18OnMasterOutOfRangeEv
003dbecc: bx       lr

# _ZN10AISDefault6OnDiedEP10GameObject
003dbe90: bx       lr

# _ZN10AISDefault20OnMasterInMeleeRangeEv
003dbed8: bx       lr

# _ZN10AISDefault15OnTargetRevivedEv
003dbeb0: bx       lr

# _ZN10AISDefault15OnProjectileHitEP10GameObject
003dbefc: bx       lr

# _ZN10AISDefault15OnCombatResultsEP10GameObjectPv
003dbf04: bx       lr

# _ZN10AISDefaultD0Ev
003dc968: ldr      r3, [pc, #0x2c]
003dc96c: ldr      r2, [pc, #0x2c]
003dc970: push     {r4, lr}
003dc974: add      r3, pc, r3
003dc978: ldr      r2, [r3, r2]
003dc97c: mov      r4, r0
003dc980: add      r2, r2, #8
003dc984: str      r2, [r0]
003dc988: bl       #0x3d92f0
003dc98c: mov      r0, r4
003dc990: bl       #0x310440
003dc994: mov      r0, r4
003dc998: pop      {r4, pc}
003dc99c: subseq   r8, fp, ip, lsl r1
003dc9a0: andeq    r2, r0, ip, lsr #21

# _ZN11AISExternal18OnMasterOutOfRangeEv
003dcd48: ldr      r3, [r0, #0xb8]
003dcd4c: tst      r3, #0x40
003dcd50: bxeq     lr
003dcd54: ldr      r1, [pc, #4]
003dcd58: add      r1, pc, r1
003dcd5c: b        #0x37c514
003dcd60: subeq    r8, lr, r8, lsr #25

# _ZN11AISExternal21OnTargetInRangedRangeEv
003dcdcc: ldr      r3, [r0, #0xb8]
003dcdd0: tst      r3, #8
003dcdd4: bxeq     lr
003dcdd8: ldr      r1, [pc, #4]
003dcddc: add      r1, pc, r1
003dcde0: b        #0x37c514
003dcde4: subeq    r8, lr, r4, lsr #25

# _ZN10AISDefault15OnCombatResultsEP9CharacterS1_Pv
003dc9a8: push     {r4, r5, r6, r7, r8, lr}
003dc9ac: sub      sp, sp, #8
003dc9b0: mov      r7, r1
003dc9b4: mov      r5, r0
003dc9b8: mov      r0, sp
003dc9bc: mov      r6, r3
003dc9c0: mov      r8, r2
003dc9c4: bl       #0x3192b4
003dc9c8: mov      r0, sp
003dc9cc: mov      r1, r7
003dc9d0: bl       #0x386f28
003dc9d4: mov      r0, sp
003dc9d8: mov      r1, r8
003dc9dc: bl       #0x386f28
003dc9e0: ldrb     r3, [r6, #0x18]
003dc9e4: mov      r4, sp
003dc9e8: tst      r3, #3
003dc9ec: beq      #0x3dca0c
003dc9f0: ldr      r3, [r5, #0xb8]
003dc9f4: tst      r3, #0x1000
003dc9f8: bne      #0x3dca30
003dc9fc: mov      r0, sp
003dca00: bl       #0x319228
003dca04: add      sp, sp, #8
003dca08: pop      {r4, r5, r6, r7, r8, pc}
003dca0c: ldr      r3, [r5, #0xb8]
003dca10: tst      r3, #0x800
003dca14: beq      #0x3dc9fc
003dca18: ldr      r1, [pc, #0x28]
003dca1c: mov      r0, r5
003dca20: mov      r2, sp
003dca24: add      r1, pc, r1
003dca28: bl       #0x37c41c
003dca2c: b        #0x3dc9fc
003dca30: ldr      r1, [pc, #0x14]
003dca34: mov      r0, r5
003dca38: mov      r2, sp
003dca3c: add      r1, pc, r1
003dca40: bl       #0x37c41c
003dca44: b        #0x3dc9fc
003dca48: subeq    r8, lr, ip, lsr pc
003dca4c: subeq    r8, lr, r4, lsr pc

# _ZN11AISExternal12OnMasterDiedEv
003dcd84: ldr      r1, [pc, #4]
003dcd88: add      r1, pc, r1
003dcd8c: b        #0x37c514
003dcd90: strheq   r8, [lr], #-0xc8

# _ZN8AISFaery20OnMasterInCloseRangeEv
003de2d0: push     {r4, lr}
003de2d4: ldr      r3, [r0]
003de2d8: mov      lr, pc
003de2dc: ldr      pc, [r3, #0x7c]
003de2e0: pop      {r4, pc}

# _ZN10AISDefault12OnTargetDiedEv
003dbeac: bx       lr

# _ZN8AISFaery15OnFriendSpottedEP9Character
003de4ec: push     {r4, r5, r6, lr}
003de4f0: mov      r4, r0
003de4f4: ldr      r0, [r0, #0x98]
003de4f8: mov      r5, r1
003de4fc: ldr      r3, [r0, #0x418]
003de500: cmp      r3, #0
003de504: beq      #0x3de50c
003de508: pop      {r4, r5, r6, pc}
003de50c: ldr      r3, [r1, #0x420]
003de510: cmp      r3, #0
003de514: bne      #0x3de508
003de518: add      r0, r0, #0x3c8
003de51c: bl       #0x3d4d80
003de520: ldr      r3, [r4, #0x98]
003de524: mov      r0, r5
003de528: mvn      r1, #0
003de52c: str      r3, [r5, #0x420]
003de530: bl       #0x3bb98c
003de534: mov      r1, r0
003de538: mov      r0, r5
003de53c: pop      {r4, r5, r6, lr}
003de540: b        #0x3ae99c

# _ZN10AISMonster15OnTargetRevivedEv
003dd490: bx       lr

# _ZN10AISDefault13OnStunExpiredEv
003dbee0: bx       lr

# _ZN11AISExternal20OnMasterInCloseRangeEv
003dcd10: ldr      r3, [r0, #0xb8]
003dcd14: tst      r3, #0x100
003dcd18: bxeq     lr
003dcd1c: ldr      r1, [pc, #4]
003dcd20: add      r1, pc, r1
003dcd24: b        #0x37c514
003dcd28: strheq   r8, [lr], #-0xc0

# _ZN10AISDefault7OnAggroEP9Character
003dbea4: bx       lr

# _ZN10AISDefault11OnEndOfAnimEv
003dbeec: bx       lr

# _ZN10AISDefault15OnMasterInSightEv
003dbec8: bx       lr

# _ZN10AISMonster12OnTargetDiedEv
003dd550: push     {r4, lr}
003dd554: ldr      r3, [r0, #0x98]
003dd558: mov      r4, r0
003dd55c: ldr      r0, [r3, #0x378]
003dd560: bl       #0x40559c
003dd564: ldr      r0, [r4, #0x98]
003dd568: mov      r1, #0
003dd56c: mov      r2, r1
003dd570: add      r0, r0, #0x3c8
003dd574: bl       #0x3d6890
003dd578: ldr      r0, [r4, #0x98]
003dd57c: add      r0, r0, #0x3c8
003dd580: pop      {r4, lr}
003dd584: b        #0x3d49c4

# _ZN6CharAI9SetScriptI10AISDefaultEEvv
003cce14: push     {r4, r5, r6, lr}
003cce18: ldr      r3, [r0, #4]
003cce1c: ldr      r5, [pc, #0xe8]
003cce20: sub      sp, sp, #8
003cce24: cmp      r3, #0
003cce28: mov      r4, r0
003cce2c: add      r5, pc, r5
003cce30: beq      #0x3cceb8
003cce34: ldr      r3, [r4, #0x20]
003cce38: cmp      r3, #0
003cce3c: beq      #0x3cce74
003cce40: ldr      r3, [r4]
003cce44: mov      r0, r4
003cce48: mov      lr, pc
003cce4c: ldr      pc, [r3, #0x14]
003cce50: ldr      r3, [r4, #0x20]
003cce54: cmp      r3, #0
003cce58: beq      #0x3cce74
003cce5c: mov      r0, r3
003cce60: ldr      r3, [r3]
003cce64: mov      lr, pc
003cce68: ldr      pc, [r3, #4]
003cce6c: mov      r3, #0
003cce70: str      r3, [r4, #0x20]
003cce74: mov      r1, #0
003cce78: mov      r0, #0xc4
003cce7c: bl       #0x310570
003cce80: mov      r1, #1
003cce84: mov      r6, r0
003cce88: bl       #0x3d8fb0
003cce8c: ldr      r3, [pc, #0x7c]
003cce90: mov      r2, #0
003cce94: str      r2, [r6, #0xc0]
003cce98: ldr      r3, [r5, r3]
003cce9c: str      r2, [r6, #0xb8]
003ccea0: str      r2, [r6, #0xbc]
003ccea4: add      r3, r3, #8
003ccea8: str      r3, [r6]
003cceac: str      r6, [r4, #0x20]
003cceb0: add      sp, sp, #8
003cceb4: pop      {r4, r5, r6, pc}
003cceb8: ldr      r2, [pc, #0x54]
003ccebc: ldr      r2, [r5, r2]
003ccec0: ldr      r2, [r2]
003ccec4: cmp      r2, #2
003ccec8: streq    r3, [r3]
003ccecc: beq      #0x3cce34
003cced0: cmp      r2, #1
003cced4: bne      #0x3cce34
003cced8: ldr      r0, [pc, #0x38]
003ccedc: ldr      r1, [pc, #0x38]
003ccee0: ldr      r2, [pc, #0x38]
003ccee4: ldr      r0, [r5, r0]
003ccee8: ldr      r3, [pc, #0x34]
003cceec: movw     ip, #0x2a1
003ccef0: add      r1, pc, r1
003ccef4: add      r2, pc, r2
003ccef8: add      r3, pc, r3
003ccefc: add      r0, r0, #0xa8
003ccf00: str      ip, [sp]
003ccf04: bl       #0x30e004
003ccf08: b        #0x3cce34
003ccf0c: subseq   r7, ip, r4, ror #24
003ccf10: andeq    r2, r0, ip, lsr #21
003ccf14: andeq    r3, r0, r0, asr #19
003ccf18: andeq    r1, r0, r0, asr #19
003ccf1c: subeq    r1, pc, r8, ror #9
003ccf20: subeq    r8, pc, ip, asr r3
003ccf24: subeq    r8, pc, r8, lsr #7

# _ZN11AISExternal8OnUpdateEv
003dce64: push     {r4, lr}
003dce68: mov      r4, r0
003dce6c: bl       #0x3dc798
003dce70: ldr      r3, [r4, #0xb8]
003dce74: tst      r3, #1
003dce78: beq      #0x3dce8c
003dce7c: ldr      r1, [pc, #0x1c]
003dce80: mov      r0, r4
003dce84: add      r1, pc, r1
003dce88: bl       #0x37c514
003dce8c: mov      r0, r4
003dce90: bl       #0x3d8eb4
003dce94: mov      r0, r4
003dce98: pop      {r4, lr}
003dce9c: b        #0x3d8ea0
003dcea0: subeq    r8, lr, r4, lsl #25

# _ZN8AISFaery18OnMasterOutOfRangeEv
003de450: push     {r4, r5, r6, r7, r8, lr}
003de454: ldr      r3, [r0, #0x98]
003de458: sub      sp, sp, #0x10
003de45c: mov      r4, r0
003de460: ldr      r0, [r3, #0x418]
003de464: bl       #0x3935dc
003de468: mov      r5, r0
003de46c: ldr      r0, [r4, #0x98]
003de470: bl       #0x3935dc
003de474: ldr      r1, [r0, #4]
003de478: mov      r6, r0
003de47c: ldr      r0, [r5, #4]
003de480: bl       #0x30e3ac
003de484: ldr      r1, [r6, #8]
003de488: mov      r8, r0
003de48c: ldr      r0, [r5, #8]
003de490: bl       #0x30e3ac
003de494: ldr      r1, [r6]
003de498: mov      r7, r0
003de49c: ldr      r0, [r5]
003de4a0: bl       #0x30e3ac
003de4a4: ldr      r3, [r4, #0x98]
003de4a8: str      r0, [sp, #4]
003de4ac: str      r8, [sp, #8]
003de4b0: str      r7, [sp, #0xc]
003de4b4: ldr      r0, [r3, #0x378]
003de4b8: add      r1, sp, #4
003de4bc: bl       #0x405374
003de4c0: add      sp, sp, #0x10
003de4c4: pop      {r4, r5, r6, r7, r8, pc}

# _ZN11AISExternal21OnMasterInRangedRangeEv
003dcd2c: ldr      r3, [r0, #0xb8]
003dcd30: tst      r3, #0x80
003dcd34: bxeq     lr
003dcd38: ldr      r1, [pc, #4]
003dcd3c: add      r1, pc, r1
003dcd40: b        #0x37c514
003dcd44: subeq    r8, lr, ip, lsr #25

# _ZN6CharAI15SetScriptByNameEPKc
003ceeb0: push     {r4, r5, r6, r7, r8, sl, lr}
003ceeb4: ldr      r4, [pc, #0x16c]
003ceeb8: ldr      r5, [pc, #0x16c]
003ceebc: mov      r6, r1
003ceec0: add      r4, pc, r4
003ceec4: ldr      r3, [r4, r5]
003ceec8: ldr      r1, [pc, #0x160]
003ceecc: sub      sp, sp, #0x24
003ceed0: ldr      r3, [r3]
003ceed4: mov      r7, r0
003ceed8: add      r1, pc, r1
003ceedc: mov      r0, r6
003ceee0: mov      r2, #2
003ceee4: str      r3, [sp, #0x1c]
003ceee8: bl       #0x30ec7c
003ceeec: subs     r8, r0, #0
003ceef0: bne      #0x3cef64
003ceef4: ldr      r1, [pc, #0x138]
003ceef8: add      r6, r6, #2
003ceefc: mov      r0, r6
003cef00: add      r1, pc, r1
003cef04: bl       #0x30e31c
003cef08: cmp      r0, #0
003cef0c: beq      #0x3cf00c
003cef10: ldr      r1, [pc, #0x120]
003cef14: mov      r0, r6
003cef18: add      r1, pc, r1
003cef1c: bl       #0x30e31c
003cef20: cmp      r0, #0
003cef24: beq      #0x3ceff8
003cef28: ldr      r1, [pc, #0x10c]
003cef2c: mov      r0, r6
003cef30: add      r1, pc, r1
003cef34: bl       #0x30e31c
003cef38: cmp      r0, #0
003cef3c: beq      #0x3cf018
003cef40: ldr      r1, [pc, #0xf8]
003cef44: mov      r0, r6
003cef48: add      r1, pc, r1
003cef4c: bl       #0x30e31c
003cef50: cmp      r0, #0
003cef54: bne      #0x3cefe8
003cef58: mov      r0, r7
003cef5c: bl       #0x3cce14
003cef60: b        #0x3cefcc
003cef64: mov      r0, r7
003cef68: bl       #0x3ccaf4
003cef6c: ldr      r3, [pc, #0xd0]
003cef70: add      r8, sp, #4
003cef74: ldr      sl, [r4, r3]
003cef78: mov      r0, sl
003cef7c: bl       #0x337888
003cef80: ldr      r1, [pc, #0xc0]
003cef84: mov      r2, sp
003cef88: mov      r0, r8
003cef8c: add      r1, pc, r1
003cef90: bl       #0x3140ec
003cef94: mov      r0, sl
003cef98: mov      r1, r8
003cef9c: bl       #0x337a88
003cefa0: ldr      r0, [sp, #0x18]
003cefa4: cmp      r0, r8
003cefa8: beq      #0x3cefc8
003cefac: cmp      r0, #0
003cefb0: beq      #0x3cefc8
003cefb4: ldr      r1, [sp, #4]
003cefb8: rsb      r1, r0, r1
003cefbc: cmp      r1, #0x80
003cefc0: bhi      #0x3cf004
003cefc4: bl       #0x708f00
003cefc8: str      r6, [r7, #0x30]
003cefcc: ldr      r3, [r4, r5]
003cefd0: ldr      r2, [sp, #0x1c]
003cefd4: ldr      r3, [r3]
003cefd8: cmp      r2, r3
003cefdc: bne      #0x3cf024
003cefe0: add      sp, sp, #0x24
003cefe4: pop      {r4, r5, r6, r7, r8, sl, pc}
003cefe8: mov      r0, r7
003cefec: bl       #0x3cce14
003ceff0: str      r8, [r7, #0x30]
003ceff4: b        #0x3cefcc
003ceff8: mov      r0, r7
003ceffc: bl       #0x3ccfe4
003cf000: b        #0x3cefcc
003cf004: bl       #0x310440
003cf008: b        #0x3cefc8
003cf00c: mov      r0, r7
003cf010: bl       #0x3ccbe4
003cf014: b        #0x3cefcc
003cf018: mov      r0, r7
003cf01c: bl       #0x3cccf8
003cf020: b        #0x3cefcc
003cf024: bl       #0x30e310
003cf028: ldrsbeq  r5, [ip], #-0xb0
003cf02c: andeq    r4, r0, ip, lsr #1
003cf030: strheq   r6, [pc], #-0x48
003cf034: umaaleq  r6, pc, r8, r4
003cf038: umaaleq  r6, pc, r0, r4
003cf03c: subeq    r6, pc, r8, lsl #9
003cf040: subeq    r6, pc, r8, ror r4
003cf044: andeq    r0, r0, r4, lsl #17
003cf048: subeq    r6, pc, ip, asr #7

# _ZN10AISDefault9OnDeAggroEP9Character
003dbea8: bx       lr

# _ZN10AISDefaultD1Ev
003dbf6c: ldr      r3, [pc, #0x24]
003dbf70: ldr      r2, [pc, #0x24]
003dbf74: push     {r4, lr}
003dbf78: add      r3, pc, r3
003dbf7c: ldr      r2, [r3, r2]
003dbf80: mov      r4, r0
003dbf84: add      r2, r2, #8
003dbf88: str      r2, [r0]
003dbf8c: bl       #0x3d92f0
003dbf90: mov      r0, r4
003dbf94: pop      {r4, pc}
003dbf98: subseq   r8, fp, r8, lsl fp
003dbf9c: andeq    r2, r0, ip, lsr #21

# _ZN11AISExternal14OnEnemySpottedEP9Character
003dd2f4: push     {r4, r5, r6, lr}
003dd2f8: sub      sp, sp, #8
003dd2fc: mov      r5, r0
003dd300: mov      r6, r1
003dd304: mov      r0, sp
003dd308: bl       #0x3192b4
003dd30c: mov      r0, sp
003dd310: mov      r1, r6
003dd314: bl       #0x386f28
003dd318: ldr      r1, [pc, #0x20]
003dd31c: mov      r0, r5
003dd320: mov      r2, sp
003dd324: add      r1, pc, r1
003dd328: bl       #0x37c41c
003dd32c: mov      r0, sp
003dd330: mov      r4, sp
003dd334: bl       #0x319228
003dd338: add      sp, sp, #8
003dd33c: pop      {r4, r5, r6, pc}
003dd340: subeq    r8, lr, ip, lsr #16

# _ZN11AISExternal18OnTargetOutOfRangeEv
003dcde8: ldr      r3, [r0, #0xb8]
003dcdec: tst      r3, #4
003dcdf0: bxeq     lr
003dcdf4: ldr      r1, [pc, #4]
003dcdf8: add      r1, pc, r1
003dcdfc: b        #0x37c514
003dce00: subeq    r8, lr, r0, lsr #25

# _ZN10AISDefault20OnTargetInMeleeRangeEv
003dc284: push     {r4, r5, r6, lr}
003dc288: mov      r4, r0
003dc28c: ldr      r0, [r0, #0x98]
003dc290: mov      r1, #0
003dc294: add      r0, r0, #0x3c8
003dc298: bl       #0x3d574c
003dc29c: subs     r5, r0, #0
003dc2a0: bne      #0x3dc2e0
003dc2a4: ldr      r0, [r4, #0x98]
003dc2a8: ldr      r1, [r0, #0x408]
003dc2ac: add      r0, r0, #0x3c8
003dc2b0: bl       #0x3d4f98
003dc2b4: cmp      r0, #0
003dc2b8: bne      #0x3dc2c0
003dc2bc: pop      {r4, r5, r6, pc}
003dc2c0: ldr      r3, [r4, #0x98]
003dc2c4: ldr      r0, [r3, #0x378]
003dc2c8: bl       #0x40559c
003dc2cc: ldr      r0, [r4, #0x98]
003dc2d0: mov      r1, r5
003dc2d4: add      r0, r0, #0x3c8
003dc2d8: pop      {r4, r5, r6, lr}
003dc2dc: b        #0x3cff34
003dc2e0: ldr      r3, [r4, #0x98]
003dc2e4: ldr      r0, [r3, #0x378]
003dc2e8: bl       #0x40559c
003dc2ec: ldr      r3, [r4, #0x98]
003dc2f0: ldr      r1, [r3, #0x408]
003dc2f4: ldr      r0, [r3, #0x378]
003dc2f8: pop      {r4, r5, r6, lr}
003dc2fc: b        #0x405b04

# _ZN10AISDefault21OnMasterInRangedRangeEv
003dbed0: bx       lr

# _ZN10AISMonsterD1Ev
003dd498: ldr      r3, [pc, #0x24]
003dd49c: ldr      r2, [pc, #0x24]
003dd4a0: push     {r4, lr}
003dd4a4: add      r3, pc, r3
003dd4a8: ldr      r2, [r3, r2]
003dd4ac: mov      r4, r0
003dd4b0: add      r2, r2, #8
003dd4b4: str      r2, [r0]
003dd4b8: bl       #0x3d92f0
003dd4bc: mov      r0, r4
003dd4c0: pop      {r4, pc}
003dd4c4: subseq   r7, fp, ip, ror #11
003dd4c8: andeq    r2, r0, ip, lsr #21

# _ZN10AISDefault6OnKillEP9Character
003dbf00: bx       lr

# _ZN10AISMonster18OnTargetOutOfSightEv
003dd500: push     {r4, r5, r6, lr}
003dd504: ldr      r3, [r0, #0x98]
003dd508: mov      r4, r0
003dd50c: ldr      r0, [r3, #0x408]
003dd510: cmp      r0, #0
003dd514: beq      #0x3dd530
003dd518: ldr      r5, [r3, #0x378]
003dd51c: bl       #0x3935dc
003dd520: mov      r1, r0
003dd524: mov      r0, r5
003dd528: bl       #0x4054e4
003dd52c: ldr      r3, [r4, #0x98]
003dd530: mov      r1, #0
003dd534: add      r0, r3, #0x3c8
003dd538: mov      r2, r1
003dd53c: bl       #0x3d6890
003dd540: ldr      r0, [r4, #0x98]
003dd544: add      r0, r0, #0x3c8
003dd548: pop      {r4, r5, r6, lr}
003dd54c: b        #0x3d49c4

# _ZN10AISDefault11OnTerminateEv
003dbe84: bx       lr

# _ZN10AISDefault11OnInitFinalEv
003dbe80: bx       lr

# _ZN8AISFaery8OnUpdateEv
003de544: push     {r4, r5, r6, r7, r8, lr}
003de548: mov      r4, r0
003de54c: bl       #0x3dc798
003de550: ldr      r3, [r4, #0x98]
003de554: ldr      r5, [r3, #0x418]
003de558: cmp      r5, #0
003de55c: beq      #0x3de56c
003de560: ldr      r6, [r3, #0x2d8]
003de564: cmp      r6, #0
003de568: bne      #0x3de570
003de56c: pop      {r4, r5, r6, r7, r8, pc}
003de570: mov      r0, r5
003de574: mvn      r1, #0
003de578: ldr      r7, [r4, #0xc4]
003de57c: bl       #0x3bb98c
003de580: cmp      r7, r0
003de584: beq      #0x3de56c
003de588: mov      r0, r5
003de58c: mvn      r1, #0
003de590: bl       #0x3bb98c
003de594: mov      r1, #0
003de598: mov      r2, r0
003de59c: str      r0, [r4, #0xc4]
003de5a0: mov      r0, r6
003de5a4: pop      {r4, r5, r6, r7, r8, lr}
003de5a8: b        #0x470e18

# _ZN11AISExternalD0Ev
003dd094: push     {r4, lr}
003dd098: mov      r4, r0
003dd09c: bl       #0x3dd060
003dd0a0: mov      r0, r4
003dd0a4: bl       #0x310440
003dd0a8: mov      r0, r4
003dd0ac: pop      {r4, pc}

# _ZN8AISFaeryD0Ev
003de688: ldr      r3, [pc, #0x2c]
003de68c: ldr      r2, [pc, #0x2c]
003de690: push     {r4, lr}
003de694: add      r3, pc, r3
003de698: ldr      r2, [r3, r2]
003de69c: mov      r4, r0
003de6a0: add      r2, r2, #8
003de6a4: str      r2, [r0]
003de6a8: bl       #0x3d92f0
003de6ac: mov      r0, r4
003de6b0: bl       #0x310440
003de6b4: mov      r0, r4
003de6b8: pop      {r4, pc}
003de6bc: ldrsheq  r6, [fp], #-0x3c
003de6c0: andeq    r2, r0, ip, lsr #21

# _ZN10AISDefault12OnMasterDiedEv
003dbebc: bx       lr

# _ZN11AISExternal20OnTargetInMeleeRangeEv
003dcd94: ldr      r3, [r0, #0xb8]
003dcd98: tst      r3, #0x20
003dcd9c: bxeq     lr
003dcda0: ldr      r1, [pc, #4]
003dcda4: add      r1, pc, r1
003dcda8: b        #0x37c514
003dcdac: subeq    r8, lr, ip, lsr #25

# _ZN10AISDefault14OnStateChangedEii
003dbe8c: bx       lr

# _ZN10AISDefault9OnRevivedEP10GameObject
003dbe94: bx       lr

# _ZN6CharAI9SetScriptI10AISMonsterEEvv
003ccbe4: push     {r4, r5, r6, lr}
003ccbe8: ldr      r3, [r0, #4]
003ccbec: ldr      r5, [pc, #0xe8]
003ccbf0: sub      sp, sp, #8
003ccbf4: cmp      r3, #0
003ccbf8: mov      r4, r0
003ccbfc: add      r5, pc, r5
003ccc00: beq      #0x3ccc88
003ccc04: ldr      r3, [r4, #0x20]
003ccc08: cmp      r3, #0
003ccc0c: beq      #0x3ccc44
003ccc10: ldr      r3, [r4]
003ccc14: mov      r0, r4
003ccc18: mov      lr, pc
003ccc1c: ldr      pc, [r3, #0x14]
003ccc20: ldr      r3, [r4, #0x20]
003ccc24: cmp      r3, #0
003ccc28: beq      #0x3ccc44
003ccc2c: mov      r0, r3
003ccc30: ldr      r3, [r3]
003ccc34: mov      lr, pc
003ccc38: ldr      pc, [r3, #4]
003ccc3c: mov      r3, #0
003ccc40: str      r3, [r4, #0x20]
003ccc44: mov      r1, #0
003ccc48: mov      r0, #0xc4
003ccc4c: bl       #0x310570
003ccc50: mov      r1, #1
003ccc54: mov      r6, r0
003ccc58: bl       #0x3d8fb0
003ccc5c: ldr      r3, [pc, #0x7c]
003ccc60: mov      r2, #0
003ccc64: str      r2, [r6, #0xc0]
003ccc68: ldr      r3, [r5, r3]
003ccc6c: str      r2, [r6, #0xb8]
003ccc70: str      r2, [r6, #0xbc]
003ccc74: add      r3, r3, #8
003ccc78: str      r3, [r6]
003ccc7c: str      r6, [r4, #0x20]
003ccc80: add      sp, sp, #8
003ccc84: pop      {r4, r5, r6, pc}
003ccc88: ldr      r2, [pc, #0x54]
003ccc8c: ldr      r2, [r5, r2]
003ccc90: ldr      r2, [r2]
003ccc94: cmp      r2, #2
003ccc98: streq    r3, [r3]
003ccc9c: beq      #0x3ccc04
003ccca0: cmp      r2, #1
003ccca4: bne      #0x3ccc04
003ccca8: ldr      r0, [pc, #0x38]
003cccac: ldr      r1, [pc, #0x38]
003cccb0: ldr      r2, [pc, #0x38]
003cccb4: ldr      r0, [r5, r0]
003cccb8: ldr      r3, [pc, #0x34]
003cccbc: movw     ip, #0x2a1
003cccc0: add      r1, pc, r1
003cccc4: add      r2, pc, r2
003cccc8: add      r3, pc, r3
003ccccc: add      r0, r0, #0xa8
003cccd0: str      ip, [sp]
003cccd4: bl       #0x30e004
003cccd8: b        #0x3ccc04

# _ZN10AISDefault20OnTargetInCloseRangeEv
003dc300: push     {r4, r5, r6, r7, r8, sl, lr}
003dc304: mov      r4, r0
003dc308: ldr      r0, [r0, #0x98]
003dc30c: sub      sp, sp, #0x1c
003dc310: mov      r1, #0
003dc314: add      r0, r0, #0x3c8
003dc318: bl       #0x3d574c
003dc31c: subs     r6, r0, #0
003dc320: beq      #0x3dc340
003dc324: ldr      r0, [r4, #0x98]
003dc328: add      r0, r0, #0x3c8
003dc32c: bl       #0x3d49d0
003dc330: cmp      r0, #0
003dc334: bne      #0x3dc37c
003dc338: add      sp, sp, #0x1c
003dc33c: pop      {r4, r5, r6, r7, r8, sl, pc}
003dc340: ldr      r0, [r4, #0x98]
003dc344: ldr      r1, [r0, #0x408]
003dc348: add      r0, r0, #0x3c8
003dc34c: bl       #0x3d4f98
003dc350: subs     r5, r0, #0
003dc354: bne      #0x3dc46c
003dc358: ldr      r3, [r4, #0x98]
003dc35c: mov      r1, r3
003dc360: ldr      r2, [r1, #0x200]!
003dc364: cmp      r2, r1
003dc368: beq      #0x3dc4b8
003dc36c: ldr      r2, [r2]
003dc370: cmp      r1, r2
003dc374: bne      #0x3dc36c
003dc378: b        #0x3dc338
003dc37c: ldr      r0, [r4, #0x98]
003dc380: mov      r1, #0
003dc384: add      r0, r0, #0x4f0
003dc388: add      r0, r0, #0xc
003dc38c: bl       #0x3c0260
003dc390: cmp      r0, #0
003dc394: beq      #0x3dc524
003dc398: ldr      r0, [r4, #0x98]
003dc39c: bl       #0x3935dc
003dc3a0: ldr      r3, [r4, #0x98]
003dc3a4: mov      r5, r0
003dc3a8: ldr      r0, [r3, #0x408]
003dc3ac: bl       #0x3935dc
003dc3b0: mov      r6, r0
003dc3b4: ldr      r1, [r0]
003dc3b8: ldr      r0, [r5]
003dc3bc: bl       #0x30e3ac
003dc3c0: ldr      r1, [r6, #4]
003dc3c4: mov      sl, r0
003dc3c8: ldr      r0, [r5, #4]
003dc3cc: bl       #0x30e3ac
003dc3d0: ldr      r1, [r6, #8]
003dc3d4: mov      r7, r0
003dc3d8: ldr      r0, [r5, #8]
003dc3dc: bl       #0x30e3ac
003dc3e0: ldr      r3, [r4, #0x98]
003dc3e4: mov      r6, r0
003dc3e8: mov      r0, r3
003dc3ec: ldr      r8, [r3, #0x378]
003dc3f0: bl       #0x3935dc
003dc3f4: mov      r5, r0
003dc3f8: ldr      r1, [r5, #4]
003dc3fc: mov      r0, r7
003dc400: bl       #0x30eba4
003dc404: ldr      r1, [r5, #8]
003dc408: mov      r7, r0
003dc40c: mov      r0, r6
003dc410: bl       #0x30eba4
003dc414: ldr      r1, [r5]
003dc418: mov      r6, r0
003dc41c: mov      r0, sl
003dc420: bl       #0x30eba4
003dc424: add      r1, sp, #0xc
003dc428: str      r0, [sp, #0xc]
003dc42c: mov      r0, r8
003dc430: str      r7, [sp, #0x10]
003dc434: str      r6, [sp, #0x14]
003dc438: bl       #0x4054e4
003dc43c: ldr      r0, [r4, #0x98]
003dc440: mov      r2, r0
003dc444: ldr      r3, [r2, #0x200]!
003dc448: cmp      r3, r2
003dc44c: beq      #0x3dc48c
003dc450: ldr      r3, [r3]
003dc454: cmp      r2, r3
003dc458: beq      #0x3dc338
003dc45c: ldr      r3, [r3]
003dc460: cmp      r2, r3
003dc464: bne      #0x3dc450
003dc468: b        #0x3dc338
003dc46c: ldr      r3, [r4, #0x98]
003dc470: ldr      r0, [r3, #0x378]
003dc474: bl       #0x40559c
003dc478: ldr      r0, [r4, #0x98]
003dc47c: mov      r1, r6
003dc480: add      r0, r0, #0x3c8
003dc484: bl       #0x3cff34
003dc488: b        #0x3dc338
003dc48c: mov      r1, #0
003dc490: mov      r2, r1
003dc494: add      r0, r0, #0x3c8
003dc498: bl       #0x3d6890
003dc49c: ldr      r0, [r4, #0x98]
003dc4a0: add      r0, r0, #0x3c8
003dc4a4: bl       #0x3d49c4
003dc4a8: ldr      r3, [r4, #0x98]
003dc4ac: mov      r2, #0
003dc4b0: strb     r2, [r3, #0x412]
003dc4b4: b        #0x3dc338
003dc4b8: ldr      r7, [r3, #0x378]
003dc4bc: ldr      r1, [r3, #0x408]
003dc4c0: mov      r0, sp
003dc4c4: bl       #0x38b228
003dc4c8: mov      r0, r7
003dc4cc: mov      r1, sp
003dc4d0: bl       #0x4054e4
003dc4d4: ldr      r0, [r4, #0x98]
003dc4d8: mov      r6, sp
003dc4dc: mov      r2, r0
003dc4e0: ldr      r3, [r2, #0x200]!
003dc4e4: cmp      r3, r2
003dc4e8: beq      #0x3dc4fc
003dc4ec: ldr      r3, [r3]
003dc4f0: cmp      r2, r3
003dc4f4: bne      #0x3dc4ec
003dc4f8: b        #0x3dc338
003dc4fc: add      r0, r0, #0x3c8
003dc500: mov      r1, r5
003dc504: mov      r2, r5
003dc508: bl       #0x3d6890
003dc50c: ldr      r0, [r4, #0x98]
003dc510: add      r0, r0, #0x3c8
003dc514: bl       #0x3d49c4
003dc518: ldr      r3, [r4, #0x98]
003dc51c: strb     r5, [r3, #0x412]
003dc520: b        #0x3dc338
003dc524: ldr      r0, [r4, #0x98]
003dc528: add      r0, r0, #0x4f0
003dc52c: add      r0, r0, #0xc
003dc530: bl       #0x3c02d0
003dc534: cmp      r0, #0
003dc538: bne      #0x3dc398
003dc53c: ldr      r0, [r4, #0x98]
003dc540: mov      r2, r0
003dc544: ldr      r3, [r2, #0x200]!
003dc548: cmp      r3, r2
003dc54c: beq      #0x3dc39c
003dc550: ldr      r3, [r3]
003dc554: cmp      r2, r3
003dc558: bne      #0x3dc550
003dc55c: b        #0x3dc338

# _ZN10AISDefault14OnEnemySpottedEP9Character
003dbea0: bx       lr

# _ZN11AISExternal7InitVCBEv
003dcec8: push     {r4, r5, r6, lr}
003dcecc: mov      r4, r0
003dced0: bl       #0x3dc7d8
003dced4: ldr      r1, [pc, #0x15c]
003dced8: mov      r0, r4
003dcedc: ldr      r5, [r4, #0xb8]
003dcee0: add      r1, pc, r1
003dcee4: bl       #0x37c2a0
003dcee8: ldr      r1, [pc, #0x14c]
003dceec: orr      r5, r0, r5
003dcef0: str      r5, [r4, #0xb8]
003dcef4: add      r1, pc, r1
003dcef8: mov      r0, r4
003dcefc: bl       #0x37c2a0
003dcf00: ldr      r1, [pc, #0x138]
003dcf04: cmp      r0, #0
003dcf08: movne    r0, #2
003dcf0c: moveq    r0, #0
003dcf10: orr      r5, r0, r5
003dcf14: add      r1, pc, r1
003dcf18: str      r5, [r4, #0xb8]
003dcf1c: mov      r0, r4
003dcf20: bl       #0x37c2a0
003dcf24: ldr      r1, [pc, #0x118]
003dcf28: cmp      r0, #0
003dcf2c: movne    r0, #4
003dcf30: moveq    r0, #0
003dcf34: orr      r5, r0, r5
003dcf38: add      r1, pc, r1
003dcf3c: str      r5, [r4, #0xb8]
003dcf40: mov      r0, r4
003dcf44: bl       #0x37c2a0
003dcf48: ldr      r1, [pc, #0xf8]
003dcf4c: cmp      r0, #0
003dcf50: movne    r0, #8
003dcf54: moveq    r0, #0
003dcf58: orr      r5, r0, r5
003dcf5c: add      r1, pc, r1
003dcf60: str      r5, [r4, #0xb8]
003dcf64: mov      r0, r4
003dcf68: bl       #0x37c2a0
003dcf6c: ldr      r1, [pc, #0xd8]
003dcf70: cmp      r0, #0
003dcf74: movne    r0, #0x10
003dcf78: moveq    r0, #0
003dcf7c: orr      r5, r0, r5
003dcf80: add      r1, pc, r1
003dcf84: str      r5, [r4, #0xb8]
003dcf88: mov      r0, r4
003dcf8c: bl       #0x37c2a0
003dcf90: ldr      r1, [pc, #0xb8]
003dcf94: cmp      r0, #0
003dcf98: movne    r0, #0x20
003dcf9c: moveq    r0, #0
003dcfa0: orr      r5, r0, r5
003dcfa4: add      r1, pc, r1
003dcfa8: str      r5, [r4, #0xb8]
003dcfac: mov      r0, r4
003dcfb0: bl       #0x37c2a0
003dcfb4: ldr      r1, [pc, #0x98]
003dcfb8: cmp      r0, #0
003dcfbc: movne    r0, #0x40
003dcfc0: moveq    r0, #0
003dcfc4: orr      r5, r0, r5
003dcfc8: add      r1, pc, r1
003dcfcc: str      r5, [r4, #0xb8]
003dcfd0: mov      r0, r4
003dcfd4: bl       #0x37c2a0
003dcfd8: ldr      r1, [pc, #0x78]
003dcfdc: cmp      r0, #0
003dcfe0: movne    r0, #0x80
003dcfe4: moveq    r0, #0
003dcfe8: orr      r5, r0, r5
003dcfec: add      r1, pc, r1
003dcff0: str      r5, [r4, #0xb8]
003dcff4: mov      r0, r4
003dcff8: bl       #0x37c2a0
003dcffc: ldr      r1, [pc, #0x58]
003dd000: cmp      r0, #0
003dd004: movne    r0, #0x100
003dd008: moveq    r0, #0
003dd00c: orr      r5, r0, r5
003dd010: str      r5, [r4, #0xb8]
003dd014: add      r1, pc, r1
003dd018: mov      r0, r4
003dd01c: bl       #0x37c2a0
003dd020: cmp      r0, #0
003dd024: movne    r0, #0x200
003dd028: moveq    r0, #0
003dd02c: orr      r5, r0, r5
003dd030: str      r5, [r4, #0xb8]
003dd034: pop      {r4, r5, r6, pc}
003dd038: subeq    r8, lr, r8, lsr #24
003dd03c: subeq    r8, lr, ip, lsr #24
003dd040: subeq    r8, lr, r4, lsl #23
003dd044: subeq    r8, lr, r8, asr #22
003dd048: subeq    r8, lr, ip, lsl #22
003dd04c: ldrdeq   r8, sb, [lr], #-0xa0
003dd050: subeq    r8, lr, ip, asr sl
003dd054: subeq    r8, lr, r0, lsr #20
003dd058: subeq    r8, lr, r4, ror #19
003dd05c: subeq    r8, lr, r4, lsr #19

# _ZN10AISDefault10OnInitPostEv
003dbe7c: bx       lr

# _ZN11AISExternal9OnRevivedEP10GameObject
003dd3f0: push     {r4, r5, r6, lr}
003dd3f4: sub      sp, sp, #8
003dd3f8: mov      r5, r0
003dd3fc: mov      r6, r1
003dd400: mov      r0, sp
003dd404: bl       #0x3192b4
003dd408: mov      r0, sp
003dd40c: mov      r1, r6
003dd410: bl       #0x386f28
003dd414: ldr      r1, [pc, #0x20]
003dd418: mov      r0, r5
003dd41c: mov      r2, sp
003dd420: add      r1, pc, r1
003dd424: bl       #0x37c41c
003dd428: mov      r0, sp
003dd42c: mov      r4, sp
003dd430: bl       #0x319228
003dd434: add      sp, sp, #8
003dd438: pop      {r4, r5, r6, pc}
003dd43c: subeq    r8, lr, r8, asr r7

# _ZN11AISExternal20OnMasterInMeleeRangeEv
003dccf4: ldr      r3, [r0, #0xb8]
003dccf8: tst      r3, #0x200
003dccfc: bxeq     lr
003dcd00: ldr      r1, [pc, #4]
003dcd04: add      r1, pc, r1
003dcd08: b        #0x37c514
003dcd0c: strheq   r8, [lr], #-0xc4

# _ZN10AISDefault7InitVCBEv
003dc7d8: ldr      r1, [pc, #0x4c]
003dc7dc: mov      r3, #0
003dc7e0: push     {r4, r5, r6, lr}
003dc7e4: add      r1, pc, r1
003dc7e8: str      r3, [r0, #0xb8]
003dc7ec: mov      r4, r0
003dc7f0: bl       #0x37c2a0
003dc7f4: ldr      r1, [pc, #0x34]
003dc7f8: cmp      r0, #0
003dc7fc: movne    r5, #0x800
003dc800: moveq    r5, #0
003dc804: str      r5, [r4, #0xb8]
003dc808: add      r1, pc, r1
003dc80c: mov      r0, r4
003dc810: bl       #0x37c2a0
003dc814: cmp      r0, #0
003dc818: movne    r0, #0x1000
003dc81c: moveq    r0, #0
003dc820: orr      r5, r0, r5
003dc824: str      r5, [r4, #0xb8]
003dc828: pop      {r4, r5, r6, pc}
003dc82c: subeq    sb, lr, ip, ror r1
003dc830: subeq    sb, lr, r8, ror #2

# _ZN11AISExternal16OnNeutralSpottedEP9Character
003dd344: push     {r4, r5, r6, lr}
003dd348: sub      sp, sp, #8
003dd34c: mov      r5, r0
003dd350: mov      r6, r1
003dd354: mov      r0, sp
003dd358: bl       #0x3192b4
003dd35c: mov      r0, sp
003dd360: mov      r1, r6
003dd364: bl       #0x386f28
003dd368: ldr      r1, [pc, #0x20]
003dd36c: mov      r0, r5
003dd370: mov      r2, sp
003dd374: add      r1, pc, r1
003dd378: bl       #0x37c41c
003dd37c: mov      r0, sp
003dd380: mov      r4, sp
003dd384: bl       #0x319228
003dd388: add      sp, sp, #8
003dd38c: pop      {r4, r5, r6, pc}
003dd390: subeq    r8, lr, ip, ror #15

# _ZN10AISDefault8OnAttackEiib
003dc12c: push     {r4, r5, r6, r7, lr}
003dc130: mov      r4, r0
003dc134: ldr      r0, [r0, #0x98]
003dc138: sub      sp, sp, #0x34
003dc13c: mov      r6, r3
003dc140: add      r0, r0, #0x3c8
003dc144: bl       #0x3d5450
003dc148: subs     r5, r0, #0
003dc14c: beq      #0x3dc18c
003dc150: add      r7, sp, #8
003dc154: ldr      r1, [r4, #0x98]
003dc158: mov      r3, r6
003dc15c: mov      r2, r5
003dc160: mov      r6, #0
003dc164: mov      r0, r7
003dc168: str      r6, [sp]
003dc16c: bl       #0x3b3368
003dc170: mov      r0, r7
003dc174: ldr      r1, [r4, #0x98]
003dc178: mov      r2, r5
003dc17c: mov      r3, r6
003dc180: bl       #0x3b10b4
003dc184: add      sp, sp, #0x34
003dc188: pop      {r4, r5, r6, r7, pc}
003dc18c: ldr      r1, [r4, #0x98]
003dc190: ldr      r3, [r1, #0x408]
003dc194: cmp      r3, #0
003dc198: beq      #0x3dc184
003dc19c: mov      r0, r3
003dc1a0: ldr      r3, [r3]
003dc1a4: mov      lr, pc
003dc1a8: ldr      pc, [r3, #0x98]
003dc1ac: b        #0x3dc184

# _ZN10AISDefault20OnAttackDelayExpiredEv
003dbee8: bx       lr

# _ZN11AISExternal15OnCombatResultsEP10GameObjectPv
003dd250: push     {r4, r5, r6, lr}
003dd254: sub      sp, sp, #8
003dd258: mov      r6, r2
003dd25c: mov      r5, r0
003dd260: mov      r0, sp
003dd264: bl       #0x3192b4
003dd268: mov      r0, sp
003dd26c: mov      r1, r6
003dd270: bl       #0x31a46c
003dd274: ldr      r1, [pc, #0x20]
003dd278: mov      r0, r5
003dd27c: mov      r2, sp
003dd280: add      r1, pc, r1
003dd284: bl       #0x37c41c
003dd288: mov      r0, sp
003dd28c: mov      r4, sp
003dd290: bl       #0x319228
003dd294: add      sp, sp, #8
003dd298: pop      {r4, r5, r6, pc}
003dd29c: strheq   r8, [lr], #-0x80

# _ZN6CharAI9SetScriptI11AISExternalEEvv
003ccaf4: push     {r4, r5, lr}
003ccaf8: ldr      r2, [r0, #4]
003ccafc: ldr      r3, [pc, #0xc8]
003ccb00: sub      sp, sp, #0xc
003ccb04: cmp      r2, #0
003ccb08: mov      r4, r0
003ccb0c: add      r3, pc, r3
003ccb10: beq      #0x3ccb78
003ccb14: ldr      r3, [r4, #0x20]
003ccb18: cmp      r3, #0
003ccb1c: beq      #0x3ccb54
003ccb20: ldr      r3, [r4]
003ccb24: mov      r0, r4
003ccb28: mov      lr, pc
003ccb2c: ldr      pc, [r3, #0x14]
003ccb30: ldr      r3, [r4, #0x20]
003ccb34: cmp      r3, #0
003ccb38: beq      #0x3ccb54
003ccb3c: mov      r0, r3
003ccb40: ldr      r3, [r3]
003ccb44: mov      lr, pc
003ccb48: ldr      pc, [r3, #4]
003ccb4c: mov      r3, #0
003ccb50: str      r3, [r4, #0x20]
003ccb54: mov      r1, #0
003ccb58: mov      r0, #0xc4
003ccb5c: bl       #0x310570
003ccb60: mov      r1, #1
003ccb64: mov      r5, r0
003ccb68: bl       #0x3dd0e4
003ccb6c: str      r5, [r4, #0x20]
003ccb70: add      sp, sp, #0xc
003ccb74: pop      {r4, r5, pc}
003ccb78: ldr      r1, [pc, #0x50]
003ccb7c: ldr      r1, [r3, r1]
003ccb80: ldr      r1, [r1]
003ccb84: cmp      r1, #2
003ccb88: streq    r2, [r2]
003ccb8c: beq      #0x3ccb14
003ccb90: cmp      r1, #1
003ccb94: bne      #0x3ccb14
003ccb98: ldr      r0, [pc, #0x34]
003ccb9c: ldr      r1, [pc, #0x34]
003ccba0: ldr      r2, [pc, #0x34]
003ccba4: ldr      r0, [r3, r0]
003ccba8: ldr      r3, [pc, #0x30]
003ccbac: movw     ip, #0x2a1
003ccbb0: add      r1, pc, r1
003ccbb4: add      r2, pc, r2
003ccbb8: add      r3, pc, r3
003ccbbc: add      r0, r0, #0xa8
003ccbc0: str      ip, [sp]
003ccbc4: bl       #0x30e004
003ccbc8: b        #0x3ccb14
003ccbcc: subseq   r7, ip, r4, lsl #31
003ccbd0: andeq    r3, r0, r0, asr #19
003ccbd4: andeq    r1, r0, r0, asr #19
003ccbd8: subeq    r1, pc, r8, lsr #16
003ccbdc: umaaleq  r8, pc, ip, r6
003ccbe0: subeq    r8, pc, r8, ror #13

# _ZN11AISExternal15OnFriendSpottedEP9Character
003dd394: push     {r4, r5, r6, lr}
003dd398: ldr      r3, [r0, #0xb8]
003dd39c: sub      sp, sp, #8
003dd3a0: mov      r5, r0
003dd3a4: tst      r3, #2
003dd3a8: mov      r6, r1
003dd3ac: beq      #0x3dd3e4
003dd3b0: mov      r0, sp
003dd3b4: bl       #0x3192b4
003dd3b8: mov      r0, sp
003dd3bc: mov      r1, r6
003dd3c0: bl       #0x386f28
003dd3c4: ldr      r1, [pc, #0x20]
003dd3c8: mov      r0, r5
003dd3cc: mov      r2, sp
003dd3d0: add      r1, pc, r1
003dd3d4: bl       #0x37c41c
003dd3d8: mov      r0, sp
003dd3dc: mov      r4, sp
003dd3e0: bl       #0x319228
003dd3e4: add      sp, sp, #8
003dd3e8: pop      {r4, r5, r6, pc}
003dd3ec: subeq    r8, lr, r0, asr r7

# _ZN11AISExternalD1Ev
003dd060: ldr      r3, [pc, #0x24]
003dd064: ldr      r2, [pc, #0x24]
003dd068: push     {r4, lr}
003dd06c: add      r3, pc, r3
003dd070: ldr      r2, [r3, r2]
003dd074: mov      r4, r0
003dd078: add      r2, r2, #8
003dd07c: str      r2, [r0]
003dd080: bl       #0x3d92f0
003dd084: mov      r0, r4
003dd088: pop      {r4, pc}
003dd08c: subseq   r7, fp, r4, lsr #20
003dd090: andeq    r2, r0, ip, lsr #21

# _ZN10AISDefault13OnScriptTimerEj
003dcc80: push     {r4, r5, r6, lr}
003dcc84: sub      sp, sp, #8
003dcc88: mov      r5, r0
003dcc8c: mov      r6, r1
003dcc90: mov      r0, sp
003dcc94: bl       #0x3192b4
003dcc98: mov      r0, sp
003dcc9c: mov      r1, r6
003dcca0: bl       #0x3cdd78
003dcca4: ldr      r1, [pc, #0x20]
003dcca8: mov      r0, r5
003dccac: mov      r2, sp
003dccb0: add      r1, pc, r1
003dccb4: bl       #0x37c41c
003dccb8: mov      r0, sp
003dccbc: mov      r4, sp
003dccc0: bl       #0x319228
003dccc4: add      sp, sp, #8
003dccc8: pop      {r4, r5, r6, pc}
003dcccc: strdeq   r8, sb, [lr], #-0xc0

# _ZN8AISFaeryD1Ev
003de2e4: ldr      r3, [pc, #0x24]
003de2e8: ldr      r2, [pc, #0x24]
003de2ec: push     {r4, lr}
003de2f0: add      r3, pc, r3
003de2f4: ldr      r2, [r3, r2]
003de2f8: mov      r4, r0
003de2fc: add      r2, r2, #8
003de300: str      r2, [r0]
003de304: bl       #0x3d92f0
003de308: mov      r0, r4
003de30c: pop      {r4, pc}
003de310: subseq   r6, fp, r0, lsr #15
003de314: andeq    r2, r0, ip, lsr #21

# _ZN11AISExternal11OnEndOfAnimEv
003dccd0: push     {r4, lr}
003dccd4: mov      r4, r0
003dccd8: bl       #0x3dbeec
003dccdc: ldr      r1, [pc, #0xc]
003dcce0: mov      r0, r4
003dcce4: add      r1, pc, r1
003dcce8: pop      {r4, lr}
003dccec: b        #0x37c514
003dccf0: subeq    r8, lr, r4, asr #25

# _ZN11AISExternal15OnProjectileHitEP10GameObject
003dd2a0: push     {r4, r5, r6, lr}
003dd2a4: sub      sp, sp, #8
003dd2a8: mov      r5, r0
003dd2ac: mov      r6, r1
003dd2b0: bl       #0x3dbefc
003dd2b4: mov      r0, sp
003dd2b8: bl       #0x3192b4
003dd2bc: mov      r0, sp
003dd2c0: mov      r1, r6
003dd2c4: bl       #0x386f28
003dd2c8: ldr      r1, [pc, #0x20]
003dd2cc: mov      r0, r5
003dd2d0: mov      r2, sp
003dd2d4: add      r1, pc, r1
003dd2d8: bl       #0x37c41c
003dd2dc: mov      r0, sp
003dd2e0: mov      r4, sp
003dd2e4: bl       #0x319228
003dd2e8: add      sp, sp, #8
003dd2ec: pop      {r4, r5, r6, pc}
003dd2f0: subeq    r8, lr, ip, ror #16

# _ZN10AISDefault18OnCollisionPersistEP10GameObjectb
003dbfa0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003dbfa4: mov      r4, r0
003dbfa8: ldr      r0, [r0, #0x98]
003dbfac: mov      r5, r1
003dbfb0: mov      r1, #0
003dbfb4: add      r0, r0, #0x4f0
003dbfb8: add      r0, r0, #0xc
003dbfbc: mov      r7, r2
003dbfc0: bl       #0x3c029c
003dbfc4: ldr      r6, [pc, #0x158]
003dbfc8: cmp      r0, #0
003dbfcc: add      r6, pc, r6
003dbfd0: beq      #0x3dc004
003dbfd4: ldr      r3, [r4, #0x98]
003dbfd8: ldr      sl, [r3, #0x408]
003dbfdc: cmp      r5, sl
003dbfe0: beq      #0x3dc004
003dbfe4: ldr      r3, [r5]
003dbfe8: mov      r0, r5
003dbfec: mov      lr, pc
003dbff0: ldr      pc, [r3, #0x24]
003dbff4: cmp      r0, #0
003dbff8: bne      #0x3dc060
003dbffc: cmp      r7, #0
003dc000: bne      #0x3dc008
003dc004: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dc008: ldr      r3, [r5]
003dc00c: mov      r0, r5
003dc010: mov      lr, pc
003dc014: ldr      pc, [r3, #0x24]
003dc018: cmp      r0, #0
003dc01c: beq      #0x3dc0dc
003dc020: ldr      r2, [pc, #0x100]
003dc024: ldr      r3, [r4, #0xc0]
003dc028: ldr      r0, [r6, r2]
003dc02c: ldr      r2, [r0, #0x74]
003dc030: cmp      r3, r2
003dc034: beq      #0x3dc004
003dc038: ldr      r3, [r4, #0x98]
003dc03c: ldrb     r3, [r3, #0x3e0]
003dc040: cmp      r3, #0
003dc044: bne      #0x3dc004
003dc048: str      r2, [r4, #0xc0]
003dc04c: ldr      r5, [r4, #0xbc]
003dc050: bl       #0x31f66c
003dc054: add      r0, r0, r5
003dc058: str      r0, [r4, #0xbc]
003dc05c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dc060: ldr      r2, [r4, #0x98]
003dc064: mov      r0, r2
003dc068: ldr      r3, [r2]
003dc06c: ldr      sb, [r2, #0x418]
003dc070: mov      lr, pc
003dc074: ldr      pc, [r3, #0x28]
003dc078: subs     r8, r0, #0
003dc07c: bne      #0x3dc09c
003dc080: rsbs     r3, sb, #1
003dc084: movlo    r3, #0
003dc088: cmp      sb, sl
003dc08c: moveq    sl, r3
003dc090: orrne    sl, r3, #1
003dc094: cmp      sl, #0
003dc098: bne      #0x3dc0f4
003dc09c: ldr      r3, [r4, #0x98]
003dc0a0: mov      r0, r3
003dc0a4: ldr      r3, [r3]
003dc0a8: mov      lr, pc
003dc0ac: ldr      pc, [r3, #0x28]
003dc0b0: cmp      r0, #0
003dc0b4: beq      #0x3dbffc
003dc0b8: ldr      r0, [r4, #0x98]
003dc0bc: mov      r1, r5
003dc0c0: add      r0, r0, #0x3c8
003dc0c4: bl       #0x3d574c
003dc0c8: cmp      r0, #0
003dc0cc: beq      #0x3dbffc
003dc0d0: ldr      r0, [r4, #0x98]
003dc0d4: bl       #0x3bc6b8
003dc0d8: b        #0x3dbffc
003dc0dc: ldr      r3, [r5, #0xf4]
003dc0e0: cmp      r3, #0x15
003dc0e4: beq      #0x3dc020
003dc0e8: cmp      r3, #2
003dc0ec: bne      #0x3dc004
003dc0f0: b        #0x3dc020
003dc0f4: ldr      r0, [r4, #0x98]
003dc0f8: mov      r1, r5
003dc0fc: add      r0, r0, #0x3c8
003dc100: bl       #0x3d574c
003dc104: cmp      r0, #0
003dc108: beq      #0x3dc09c
003dc10c: ldr      r0, [r4, #0x98]
003dc110: mov      r1, r5
003dc114: mov      r2, r8
003dc118: add      r0, r0, #0x3c8
003dc11c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003dc120: b        #0x3d6890
003dc124: subseq   r8, fp, r4, asr #21
003dc128: strdeq   r3, r4, [r0], -r4

# _ZN11AISExternal10OnInitPostEv
003dce54: ldr      r1, [pc, #4]
003dce58: add      r1, pc, r1
003dce5c: b        #0x37c514
003dce60: subeq    r8, lr, r0, lsr #25

# _ZN10AISDefault10OnInteractEv
003dc1b0: push     {r4, lr}
003dc1b4: ldr      r3, [r0, #0x98]
003dc1b8: mov      r4, r0
003dc1bc: ldr      r3, [r3, #0x408]
003dc1c0: cmp      r3, #0
003dc1c4: beq      #0x3dc20c
003dc1c8: mov      r0, r3
003dc1cc: ldr      r3, [r3]
003dc1d0: mov      lr, pc
003dc1d4: ldr      pc, [r3, #0x24]
003dc1d8: cmp      r0, #0
003dc1dc: bne      #0x3dc210
003dc1e0: ldr      r1, [r4, #0x98]
003dc1e4: ldr      r3, [r1, #0x408]
003dc1e8: mov      r0, r3
003dc1ec: ldr      r3, [r3]
003dc1f0: mov      lr, pc
003dc1f4: ldr      pc, [r3, #0x98]
003dc1f8: ldr      r3, [r4, #0x98]
003dc1fc: ldr      r3, [r3, #0x408]
003dc200: ldrb     r2, [r3, #0x2fa]
003dc204: cmp      r2, #0
003dc208: beq      #0x3dc22c
003dc20c: pop      {r4, pc}
003dc210: ldr      r0, [r4, #0x98]
003dc214: add      r0, r0, #0x3c8
003dc218: bl       #0x3d5450
003dc21c: bl       #0x3a3064
003dc220: cmp      r0, #0
003dc224: bne      #0x3dc20c
003dc228: b        #0x3dc1e0
003dc22c: mov      r0, r3
003dc230: ldr      r3, [r3]
003dc234: mov      lr, pc
003dc238: ldr      pc, [r3, #0x24]
003dc23c: cmp      r0, #0
003dc240: bne      #0x3dc268
003dc244: ldr      r0, [r4, #0x98]
003dc248: mov      r1, #0
003dc24c: mov      r2, r1
003dc250: add      r0, r0, #0x3c8
003dc254: bl       #0x3d6890
003dc258: ldr      r0, [r4, #0x98]
003dc25c: add      r0, r0, #0x3c8
003dc260: pop      {r4, lr}
003dc264: b        #0x3d49c4
003dc268: ldr      r0, [r4, #0x98]
003dc26c: add      r0, r0, #0x3c8
003dc270: bl       #0x3d5450
003dc274: bl       #0x3a30c4
003dc278: cmp      r0, #0
003dc27c: bne      #0x3dc20c
003dc280: b        #0x3dc244

# _ZN10AISDefault16OnCollisionBeginEP10GameObjectb
003dbf08: ldr      r3, [pc, #0x28]
003dbf0c: ldr      ip, [pc, #0x28]
003dbf10: push     {r4, lr}
003dbf14: add      r3, pc, r3
003dbf18: ldr      ip, [r3, ip]
003dbf1c: ldr      r3, [ip]
003dbf20: add      r3, r3, #1
003dbf24: str      r3, [ip]
003dbf28: ldr      r3, [r0]
003dbf2c: mov      lr, pc
003dbf30: ldr      pc, [r3, #0xc0]
003dbf34: pop      {r4, pc}
003dbf38: subseq   r8, fp, ip, ror fp
003dbf3c: andeq    r1, r0, r8, asr #12

# _ZN10AISDefault11OnAnimEventEPKc
003dca50: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dca54: sub      sp, sp, #0x3c
003dca58: add      r5, sp, #0x30
003dca5c: mov      r6, r0
003dca60: mov      r7, r1
003dca64: mov      r0, r5
003dca68: bl       #0x3192b4
003dca6c: mov      r0, r5
003dca70: mov      r1, r7
003dca74: bl       #0x39ec10
003dca78: ldr      r3, [r6, #0x98]
003dca7c: mov      r0, r5
003dca80: ldr      r4, [pc, #0x1d0]
003dca84: ldr      r1, [r3, #0x4f4]
003dca88: bl       #0x3cdd78
003dca8c: ldr      r1, [pc, #0x1c8]
003dca90: mov      r0, r6
003dca94: mov      r2, r5
003dca98: add      r1, pc, r1
003dca9c: bl       #0x37c41c
003dcaa0: ldr      r1, [pc, #0x1b8]
003dcaa4: mov      r0, r7
003dcaa8: add      r4, pc, r4
003dcaac: add      r1, pc, r1
003dcab0: bl       #0x30e31c
003dcab4: cmp      r0, #0
003dcab8: beq      #0x3dcad4
003dcabc: ldr      r1, [pc, #0x1a0]
003dcac0: mov      r0, r7
003dcac4: add      r1, pc, r1
003dcac8: bl       #0x30e31c
003dcacc: cmp      r0, #0
003dcad0: bne      #0x3dcc0c
003dcad4: ldr      r1, [pc, #0x18c]
003dcad8: mov      r3, #0
003dcadc: mov      r0, r7
003dcae0: add      r1, pc, r1
003dcae4: str      r3, [sp, #0x2c]
003dcae8: str      r3, [sp, #0x24]
003dcaec: str      r3, [sp, #0x28]
003dcaf0: bl       #0x30e31c
003dcaf4: cmp      r0, #0
003dcaf8: beq      #0x3dcc1c
003dcafc: add      r0, sp, #0xc
003dcb00: ldr      r1, [r6, #0x98]
003dcb04: bl       #0x3a57f4
003dcb08: ldr      r3, [sp, #0xc]
003dcb0c: str      r3, [sp, #0x24]
003dcb10: ldr      r3, [sp, #0x10]
003dcb14: str      r3, [sp, #0x28]
003dcb18: ldr      r3, [sp, #0x14]
003dcb1c: str      r3, [sp, #0x2c]
003dcb20: ldr      r0, [r6, #0x98]
003dcb24: bl       #0x3a3300
003dcb28: ldr      r7, [pc, #0x13c]
003dcb2c: mov      ip, #0
003dcb30: add      fp, sp, #0x24
003dcb34: mov      r1, r0
003dcb38: mov      r3, ip
003dcb3c: mov      r2, fp
003dcb40: ldr      r0, [r4, r7]
003dcb44: str      ip, [sp]
003dcb48: bl       #0x495d14
003dcb4c: ldr      r2, [r6, #0x98]
003dcb50: movw     r3, #0x1014
003dcb54: ldr      r3, [r2, r3]
003dcb58: cmp      r3, #0
003dcb5c: blt      #0x3dcc44
003dcb60: ldr      r1, [pc, #0x108]
003dcb64: ldr      r1, [r4, r1]
003dcb68: ldr      r1, [r1]
003dcb6c: cmp      r3, r1
003dcb70: bge      #0x3dcc44
003dcb74: ldr      r1, [pc, #0xf8]
003dcb78: mov      r0, #0x18
003dcb7c: ldr      r1, [r4, r1]
003dcb80: ldr      r1, [r1]
003dcb84: mla      r3, r0, r3, r1
003dcb88: ldrb     r3, [r3, #0x14]
003dcb8c: ldr      sl, [r2, #0x1d8]
003dcb90: cmp      sl, #0
003dcb94: ldrne    sl, [sl, #0x3c]
003dcb98: cmp      r3, #0
003dcb9c: beq      #0x3dcc0c
003dcba0: cmp      sl, #0
003dcba4: beq      #0x3dcc0c
003dcba8: ldr      r3, [pc, #0xc8]
003dcbac: ldr      r3, [r4, r3]
003dcbb0: ldr      sb, [r3]
003dcbb4: cmp      sb, #0
003dcbb8: ble      #0x3dcc0c
003dcbbc: ldr      r3, [pc, #0xb8]
003dcbc0: mov      r8, #0
003dcbc4: ldr      r3, [r4, r3]
003dcbc8: ldr      r6, [r3]
003dcbcc: b        #0x3dcbdc
003dcbd0: cmp      r8, sb
003dcbd4: add      r6, r6, #0x20
003dcbd8: beq      #0x3dcc0c
003dcbdc: ldr      r0, [r6, #0xc]
003dcbe0: mov      r1, sl
003dcbe4: bl       #0x30e31c
003dcbe8: subs     ip, r0, #0
003dcbec: add      r8, r8, #1
003dcbf0: bne      #0x3dcbd0
003dcbf4: ldr      r1, [r6, #4]
003dcbf8: ldr      r0, [r4, r7]
003dcbfc: mov      r2, fp
003dcc00: mov      r3, ip
003dcc04: str      ip, [sp]
003dcc08: bl       #0x495d14
003dcc0c: mov      r0, r5
003dcc10: bl       #0x319228
003dcc14: add      sp, sp, #0x3c
003dcc18: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003dcc1c: add      r0, sp, #0x18
003dcc20: ldr      r1, [r6, #0x98]
003dcc24: bl       #0x3a5874
003dcc28: ldr      r3, [sp, #0x18]
003dcc2c: str      r3, [sp, #0x24]
003dcc30: ldr      r3, [sp, #0x1c]
003dcc34: str      r3, [sp, #0x28]
003dcc38: ldr      r3, [sp, #0x20]
003dcc3c: str      r3, [sp, #0x2c]
003dcc40: b        #0x3dcb20
003dcc44: ldr      r3, [pc, #0x28]
003dcc48: ldr      r3, [r4, r3]
003dcc4c: ldr      r3, [r3]
003dcc50: ldrb     r3, [r3, #0x14]
003dcc54: b        #0x3dcb8c
003dcc58: subseq   r7, fp, r8, ror #31
003dcc5c: umaaleq  r6, lr, r8, r4
003dcc60: ldrdeq   r8, sb, [lr], #-0xe4
003dcc64: subeq    r8, lr, ip, asr #29
003dcc68: subeq    r8, lr, r0, lsr #29
003dcc6c: andeq    r1, r0, r8, lsl #22
003dcc70: andeq    r1, r0, ip, ror #3
003dcc74: andeq    r0, r0, r4, asr #30
003dcc78: andeq    r1, r0, ip, lsr #23
003dcc7c: strheq   r3, [r0], -r4

# _ZN10AISDefault15OnMasterRevivedEv
003dbec0: bx       lr

# _ZN10AISDefault17OnCollisionResultEP10GameObjectb
003dbf68: bx       lr

# _ZN10AISMonster15OnTargetInSightEv
003dd494: bx       lr

# _ZN6CharAI9SetScriptI8AISFaeryEEvv
003cccf8: push     {r4, r5, r6, lr}
003cccfc: ldr      r3, [r0, #4]
003ccd00: ldr      r5, [pc, #0xf0]
003ccd04: sub      sp, sp, #8
003ccd08: cmp      r3, #0
003ccd0c: mov      r4, r0
003ccd10: add      r5, pc, r5
003ccd14: beq      #0x3ccda4
003ccd18: ldr      r3, [r4, #0x20]
003ccd1c: cmp      r3, #0
003ccd20: beq      #0x3ccd58
003ccd24: ldr      r3, [r4]
003ccd28: mov      r0, r4
003ccd2c: mov      lr, pc
003ccd30: ldr      pc, [r3, #0x14]
003ccd34: ldr      r3, [r4, #0x20]
003ccd38: cmp      r3, #0
003ccd3c: beq      #0x3ccd58
003ccd40: mov      r0, r3
003ccd44: ldr      r3, [r3]
003ccd48: mov      lr, pc
003ccd4c: ldr      pc, [r3, #4]
003ccd50: mov      r3, #0
003ccd54: str      r3, [r4, #0x20]
003ccd58: mov      r1, #0
003ccd5c: mov      r0, #0xc8
003ccd60: bl       #0x310570
003ccd64: mov      r1, #1
003ccd68: mov      r6, r0
003ccd6c: bl       #0x3d8fb0
003ccd70: ldr      r2, [pc, #0x84]
003ccd74: mov      r3, #0
003ccd78: mvn      r1, #0
003ccd7c: ldr      r2, [r5, r2]
003ccd80: str      r3, [r6, #0xc0]
003ccd84: str      r1, [r6, #0xc4]
003ccd88: add      r2, r2, #8
003ccd8c: str      r2, [r6]
003ccd90: str      r3, [r6, #0xb8]
003ccd94: str      r3, [r6, #0xbc]
003ccd98: str      r6, [r4, #0x20]
003ccd9c: add      sp, sp, #8
003ccda0: pop      {r4, r5, r6, pc}
003ccda4: ldr      r2, [pc, #0x54]
003ccda8: ldr      r2, [r5, r2]
003ccdac: ldr      r2, [r2]
003ccdb0: cmp      r2, #2
003ccdb4: streq    r3, [r3]
003ccdb8: beq      #0x3ccd18
003ccdbc: cmp      r2, #1
003ccdc0: bne      #0x3ccd18
003ccdc4: ldr      r0, [pc, #0x38]
003ccdc8: ldr      r1, [pc, #0x38]
003ccdcc: ldr      r2, [pc, #0x38]
003ccdd0: ldr      r0, [r5, r0]
003ccdd4: ldr      r3, [pc, #0x34]
003ccdd8: movw     ip, #0x2a1
003ccddc: add      r1, pc, r1
003ccde0: add      r2, pc, r2
003ccde4: add      r3, pc, r3
003ccde8: add      r0, r0, #0xa8
003ccdec: str      ip, [sp]
003ccdf0: bl       #0x30e004
003ccdf4: b        #0x3ccd18
003ccdf8: subseq   r7, ip, r0, lsl #27
003ccdfc: andeq    r4, r0, r4, lsr sb
003cce00: andeq    r3, r0, r0, asr #19
003cce04: andeq    r1, r0, r0, asr #19
003cce08: strdeq   r1, r2, [pc], #-0x5c
003cce0c: subeq    r8, pc, r0, ror r4
003cce10: strheq   r8, [pc], #-0x4c

# _ZN8AISFaery21OnMasterInRangedRangeEv
003de444: ldr      r3, [r0, #0x98]
003de448: ldr      r0, [r3, #0x378]
003de44c: b        #0x40559c

# _ZN10AISMonsterD0Ev
003dd7b0: ldr      r3, [pc, #0x2c]
003dd7b4: ldr      r2, [pc, #0x2c]
003dd7b8: push     {r4, lr}
003dd7bc: add      r3, pc, r3
003dd7c0: ldr      r2, [r3, r2]
003dd7c4: mov      r4, r0
003dd7c8: add      r2, r2, #8
003dd7cc: str      r2, [r0]
003dd7d0: bl       #0x3d92f0
003dd7d4: mov      r0, r4
003dd7d8: bl       #0x310440
003dd7dc: mov      r0, r4
003dd7e0: pop      {r4, pc}
003dd7e4: ldrsbeq  r7, [fp], #-0x24
003dd7e8: andeq    r2, r0, ip, lsr #21

# _ZN10AISDefault5OnMsgEiPKv
003dbe88: bx       lr

# _ZN11AISExternalC2Eb
003dd128: push     {r4, r5, r6, lr}
003dd12c: ldr      r5, [pc, #0x30]
003dd130: mov      r4, r0
003dd134: bl       #0x3d8fb0
003dd138: ldr      r2, [pc, #0x28]
003dd13c: add      r5, pc, r5
003dd140: mov      r3, #0
003dd144: ldr      r2, [r5, r2]
003dd148: str      r3, [r4, #0xc0]
003dd14c: str      r3, [r4, #0xb8]
003dd150: add      r2, r2, #8
003dd154: str      r2, [r4]
003dd158: str      r3, [r4, #0xbc]
003dd15c: mov      r0, r4
003dd160: pop      {r4, r5, r6, pc}
003dd164: subseq   r7, fp, r4, asr sb
003dd168: andeq    r0, r0, r4, ror #18

# _ZN10AISDefault15OnTargetInSightEv
003dbeb8: bx       lr

# _ZN11AISExternal18OnMasterOutOfSightEv
003dcd74: ldr      r1, [pc, #4]
003dcd78: add      r1, pc, r1
003dcd7c: b        #0x37c514
003dcd80: strheq   r8, [lr], #-0xc0

# _ZN10AISDefault15OnFriendSpottedEP9Character
003dbe98: bx       lr

# _ZN11AISExternal15OnMasterInSightEv
003dcd64: ldr      r1, [pc, #4]
003dcd68: add      r1, pc, r1
003dcd6c: b        #0x37c514
003dcd70: strheq   r8, [lr], #-0xc0

# _ZN10AISMonster8OnUpdateEv
003dd588: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003dd58c: mov      r5, r0
003dd590: bl       #0x3dc798
003dd594: ldr      r0, [r5, #0x98]
003dd598: mov      r1, #0
003dd59c: ldr      r4, [pc, #0x128]
003dd5a0: add      r0, r0, #0x4f0
003dd5a4: add      r0, r0, #0xc
003dd5a8: bl       #0x3c0260
003dd5ac: cmp      r0, #0
003dd5b0: add      r4, pc, r4
003dd5b4: beq      #0x3dd5c8
003dd5b8: ldr      r0, [r5, #0x98]
003dd5bc: ldr      r3, [r0, #0x408]
003dd5c0: cmp      r3, #0
003dd5c4: beq      #0x3dd5cc
003dd5c8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003dd5cc: add      r1, r0, #0x1440
003dd5d0: add      r1, r1, #0x10
003dd5d4: mov      r2, #0x3f800000
003dd5d8: bl       #0x38ac0c
003dd5dc: cmp      r0, #0
003dd5e0: bne      #0x3dd5c8
003dd5e4: ldr      r6, [r5, #0x98]
003dd5e8: mov      r0, r6
003dd5ec: bl       #0x3935dc
003dd5f0: movw     r3, #0x1450
003dd5f4: mov      r7, r0
003dd5f8: ldr      r1, [r7]
003dd5fc: ldr      r0, [r6, r3]
003dd600: bl       #0x30e3ac
003dd604: movw     r3, #0x1454
003dd608: ldr      r1, [r7, #4]
003dd60c: mov      sl, r0
003dd610: ldr      r0, [r6, r3]
003dd614: bl       #0x30e3ac
003dd618: movw     r3, #0x1458
003dd61c: ldr      r1, [r7, #8]
003dd620: mov      r8, r0
003dd624: ldr      r0, [r6, r3]
003dd628: bl       #0x30e3ac
003dd62c: ldr      r3, [pc, #0x9c]
003dd630: mov      r7, r0
003dd634: ldr      r0, [r5, #0x98]
003dd638: ldr      r3, [r4, r3]
003dd63c: ldr      r4, [r3]
003dd640: bl       #0x3a2fec
003dd644: mov      r3, #0x44
003dd648: mla      r4, r3, r0, r4
003dd64c: ldr      r0, [r4, #0x1c]
003dd650: mov      r1, r0
003dd654: bl       #0x30ed6c
003dd658: mov      r1, sl
003dd65c: mov      r4, r0
003dd660: mov      r0, sl
003dd664: bl       #0x30ed6c
003dd668: mov      r1, r8
003dd66c: mov      r6, r0
003dd670: mov      r0, r8
003dd674: bl       #0x30ed6c
003dd678: mov      r1, r0
003dd67c: mov      r0, r6
003dd680: bl       #0x30eba4
003dd684: mov      r1, r7
003dd688: mov      r6, r0
003dd68c: mov      r0, r7
003dd690: bl       #0x30ed6c
003dd694: mov      r1, r0
003dd698: mov      r0, r6
003dd69c: bl       #0x30eba4
003dd6a0: mov      r1, r0
003dd6a4: mov      r0, r4
003dd6a8: bl       #0x30e70c
003dd6ac: cmp      r0, #0
003dd6b0: beq      #0x3dd5c8
003dd6b4: ldr      r3, [r5, #0x98]
003dd6b8: ldr      r0, [r3, #0x378]
003dd6bc: add      r1, r3, #0x1440
003dd6c0: add      r1, r1, #0x10
003dd6c4: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003dd6c8: b        #0x4054e4
003dd6cc: subseq   r7, fp, r0, ror #9
003dd6d0: andeq    r0, r0, r8, asr r7

# _ZN8AISFaery20OnMasterInMeleeRangeEv
003de318: push     {r4, r5, r6, r7, r8, lr}
003de31c: mov      r5, r0
003de320: ldr      r0, [r0, #0x98]
003de324: sub      sp, sp, #0x18
003de328: mov      r1, #0
003de32c: add      r0, r0, #0x4f0
003de330: add      r0, r0, #0xc
003de334: bl       #0x3c0260
003de338: ldr      r4, [pc, #0xfc]
003de33c: cmp      r0, #0
003de340: add      r4, pc, r4
003de344: beq      #0x3de434
003de348: ldr      r3, [r5, #0x98]
003de34c: ldr      r6, [r3, #0x418]
003de350: cmp      r6, #0
003de354: beq      #0x3de434
003de358: add      r0, r6, #0x4f0
003de35c: add      r0, r0, #0xc
003de360: mov      r1, #0
003de364: bl       #0x3c0260
003de368: cmp      r0, #0
003de36c: beq      #0x3de434
003de370: mov      r3, #0
003de374: mov      r0, r6
003de378: add      r1, sp, #0xc
003de37c: str      r3, [sp, #0x14]
003de380: str      r3, [sp, #0xc]
003de384: str      r3, [sp, #0x10]
003de388: bl       #0x393ae4
003de38c: ldr      r3, [pc, #0xac]
003de390: ldr      r0, [sp, #0xc]
003de394: ldr      r8, [sp, #0x10]
003de398: ldr      r3, [r4, r3]
003de39c: add      r0, r0, #0x80000000
003de3a0: add      r8, r8, #0x80000000
003de3a4: ldr      r4, [r3]
003de3a8: ldr      r7, [sp, #0x14]
003de3ac: mov      r1, r4
003de3b0: bl       #0x30ed6c
003de3b4: mov      r1, r4
003de3b8: str      r0, [sp, #0xc]
003de3bc: mov      r0, r8
003de3c0: bl       #0x30ed6c
003de3c4: add      r7, r7, #0x80000000
003de3c8: mov      r1, r4
003de3cc: str      r0, [sp, #0x10]
003de3d0: mov      r0, r7
003de3d4: bl       #0x30ed6c
003de3d8: ldr      r3, [r5, #0x98]
003de3dc: str      r0, [sp, #0x14]
003de3e0: mov      r0, r6
003de3e4: ldr      r7, [r3, #0x378]
003de3e8: bl       #0x3935dc
003de3ec: ldr      r1, [sp, #0x10]
003de3f0: mov      r4, r0
003de3f4: ldr      r0, [r0, #4]
003de3f8: bl       #0x30eba4
003de3fc: ldr      r1, [sp, #0x14]
003de400: mov      r6, r0
003de404: ldr      r0, [r4, #8]
003de408: bl       #0x30eba4
003de40c: ldr      r1, [sp, #0xc]
003de410: mov      r5, r0
003de414: ldr      r0, [r4]
003de418: bl       #0x30eba4
003de41c: mov      r1, sp
003de420: str      r0, [sp]
003de424: mov      r0, r7
003de428: str      r6, [sp, #4]
003de42c: str      r5, [sp, #8]
003de430: bl       #0x40542c
003de434: add      sp, sp, #0x18
003de438: pop      {r4, r5, r6, r7, r8, pc}
003de43c: subseq   r6, fp, r0, asr r7
003de440: ldrdeq   r2, r3, [r0], -ip

# _ZN10AISDefault8OnUpdateEv
003dc798: push     {r4, lr}
003dc79c: ldr      r3, [r0, #0xbc]
003dc7a0: mov      r4, r0
003dc7a4: cmp      r3, #0xc7
003dc7a8: bhi      #0x3dc7b0
003dc7ac: pop      {r4, pc}
003dc7b0: ldr      r0, [r0, #0x98]
003dc7b4: mov      r3, #0
003dc7b8: str      r3, [r4, #0xbc]
003dc7bc: add      r0, r0, #0x3c8
003dc7c0: mov      r1, #0x3e8
003dc7c4: bl       #0x3cb748
003dc7c8: ldr      r3, [r4, #0x98]
003dc7cc: ldr      r0, [r3, #0x378]
003dc7d0: pop      {r4, lr}
003dc7d4: b        #0x40559c

# _ZN10AISDefault18OnTargetOutOfSightEv
003dbeb4: bx       lr

# _ZN11AISExternal12OnTargetDiedEv
003dce24: ldr      r1, [pc, #4]
003dce28: add      r1, pc, r1
003dce2c: b        #0x37c514
003dce30: strheq   r4, [lr], #-0xd8

# _ZN11AISExternalD2Ev
003dd0b0: ldr      r3, [pc, #0x24]
003dd0b4: ldr      r2, [pc, #0x24]
003dd0b8: push     {r4, lr}
003dd0bc: add      r3, pc, r3
003dd0c0: ldr      r2, [r3, r2]
003dd0c4: mov      r4, r0
003dd0c8: add      r2, r2, #8
003dd0cc: str      r2, [r0]
003dd0d0: bl       #0x3d92f0
003dd0d4: mov      r0, r4
003dd0d8: pop      {r4, pc}
003dd0dc: ldrsbeq  r7, [fp], #-0x94
003dd0e0: andeq    r2, r0, ip, lsr #21

# _ZN10AISDefault20OnMasterInCloseRangeEv
003dbed4: bx       lr

# _ZN10AISMonster14OnEnemySpottedEP9Character
003dd4cc: push     {r4, lr}
003dd4d0: mov      r4, r0
003dd4d4: ldr      r0, [r0, #0x98]
003dd4d8: ldr      r2, [r0, #0x408]
003dd4dc: cmp      r2, #0
003dd4e0: beq      #0x3dd4e8
003dd4e4: pop      {r4, pc}
003dd4e8: add      r0, r0, #0x3c8
003dd4ec: bl       #0x3d6890
003dd4f0: ldr      r3, [r4, #0x98]
003dd4f4: mov      r2, #1
003dd4f8: strb     r2, [r3, #0x412]
003dd4fc: pop      {r4, pc}

# _ZN11AISExternal6OnDiedEP10GameObject
003dd440: push     {r4, r5, r6, lr}
003dd444: sub      sp, sp, #8
003dd448: mov      r5, r0
003dd44c: mov      r6, r1
003dd450: mov      r0, sp
003dd454: bl       #0x3192b4
003dd458: mov      r0, sp
003dd45c: mov      r1, r6
003dd460: bl       #0x386f28
003dd464: ldr      r1, [pc, #0x20]
003dd468: mov      r0, r5
003dd46c: mov      r2, sp
003dd470: add      r1, pc, r1
003dd474: bl       #0x37c41c
003dd478: mov      r0, sp
003dd47c: mov      r4, sp
003dd480: bl       #0x319228
003dd484: add      sp, sp, #8
003dd488: pop      {r4, r5, r6, pc}
003dd48c: subeq    r8, lr, r8, lsl r7

# _ZN10AISDefault18OnTargetOutOfRangeEv
003dc698: push     {r4, r5, r6, lr}
003dc69c: mov      r4, r0
003dc6a0: ldr      r0, [r0, #0x98]
003dc6a4: sub      sp, sp, #0x10
003dc6a8: add      r0, r0, #0x3c8
003dc6ac: bl       #0x3d49d0
003dc6b0: cmp      r0, #0
003dc6b4: bne      #0x3dc6c0
003dc6b8: add      sp, sp, #0x10
003dc6bc: pop      {r4, r5, r6, pc}
003dc6c0: ldr      r0, [r4, #0x98]
003dc6c4: mov      r1, #0
003dc6c8: add      r0, r0, #0x4f0
003dc6cc: add      r0, r0, #0xc
003dc6d0: bl       #0x3c0260
003dc6d4: cmp      r0, #0
003dc6d8: beq      #0x3dc730
003dc6dc: ldr      r3, [r4, #0x98]
003dc6e0: ldr      r6, [r3, #0x378]
003dc6e4: add      r5, sp, #4
003dc6e8: ldr      r1, [r3, #0x408]
003dc6ec: mov      r0, r5
003dc6f0: bl       #0x38b228
003dc6f4: mov      r0, r6
003dc6f8: mov      r1, r5
003dc6fc: bl       #0x4054e4
003dc700: ldr      r0, [r4, #0x98]
003dc704: mov      r2, r0
003dc708: ldr      r3, [r2, #0x200]!
003dc70c: cmp      r3, r2
003dc710: beq      #0x3dc76c
003dc714: ldr      r3, [r3]
003dc718: cmp      r2, r3
003dc71c: beq      #0x3dc6b8
003dc720: ldr      r3, [r3]
003dc724: cmp      r2, r3
003dc728: bne      #0x3dc714
003dc72c: b        #0x3dc6b8
003dc730: ldr      r0, [r4, #0x98]
003dc734: add      r0, r0, #0x4f0
003dc738: add      r0, r0, #0xc
003dc73c: bl       #0x3c02d0
003dc740: cmp      r0, #0
003dc744: bne      #0x3dc6dc
003dc748: ldr      r3, [r4, #0x98]
003dc74c: mov      r1, r3
003dc750: ldr      r2, [r1, #0x200]!
003dc754: cmp      r2, r1
003dc758: beq      #0x3dc6e0
003dc75c: ldr      r2, [r2]
003dc760: cmp      r1, r2
003dc764: bne      #0x3dc75c
003dc768: b        #0x3dc6b8
003dc76c: mov      r1, #0
003dc770: mov      r2, r1
003dc774: add      r0, r0, #0x3c8
003dc778: bl       #0x3d6890
003dc77c: ldr      r0, [r4, #0x98]
003dc780: add      r0, r0, #0x3c8
003dc784: bl       #0x3d49c4
003dc788: ldr      r3, [r4, #0x98]
003dc78c: mov      r2, #0
003dc790: strb     r2, [r3, #0x412]
003dc794: b        #0x3dc6b8

# _ZN8AISFaery18OnMasterOutOfSightEv
003de4c8: push     {r4, lr}
003de4cc: ldr      r3, [r0, #0x98]
003de4d0: ldr      r0, [r3, #0x418]
003de4d4: ldr      r4, [r3, #0x378]
003de4d8: bl       #0x3935dc
003de4dc: mov      r1, r0
003de4e0: mov      r0, r4
003de4e4: pop      {r4, lr}
003de4e8: b        #0x405318

# _ZN11AISExternal20OnTargetInCloseRangeEv
003dcdb0: ldr      r3, [r0, #0xb8]
003dcdb4: tst      r3, #0x10
003dcdb8: bxeq     lr
003dcdbc: ldr      r1, [pc, #4]
003dcdc0: add      r1, pc, r1
003dcdc4: b        #0x37c514
003dcdc8: subeq    r8, lr, r8, lsr #25
