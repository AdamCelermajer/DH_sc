# 0x3109e0 _ZNSs9_M_assignEPKcS0_
003109e0: push {r4, r5, r6, r7, r8, lr}
003109e4: mov r4, r0
003109e8: ldr r3, [r0, #0x10]
003109ec: ldr r0, [r0, #0x14]
003109f0: rsb r5, r1, r2
003109f4: mov r6, r2
003109f8: rsb r2, r0, r3
003109fc: cmp r5, r2
00310a00: mov r7, r1
00310a04: bhi #0x310a3c
00310a08: cmp r5, #0
00310a0c: bne #0x310a5c
00310a10: add r2, r0, r5
00310a14: cmp r2, r3
00310a18: beq #0x310a34
00310a1c: ldrb r1, [r3]
00310a20: rsb r3, r3, r2
00310a24: strb r1, [r0, r5]
00310a28: ldr r2, [r4, #0x10]
00310a2c: add r3, r2, r3
00310a30: str r3, [r4, #0x10]
00310a34: mov r0, r4
00310a38: pop {r4, r5, r6, r7, r8, pc}
00310a3c: cmp r2, #0
00310a40: bne #0x310a7c
00310a44: add r1, r7, r2
00310a48: mov r0, r4
00310a4c: mov r2, r6
00310a50: bl #0x310804
00310a54: mov r0, r4
00310a58: pop {r4, r5, r6, r7, r8, pc}
00310a5c: mov r2, r5
00310a60: bl #0x30e868
00310a64: ldr r0, [r4, #0x14]
00310a68: ldr r3, [r4, #0x10]
00310a6c: add r2, r0, r5
00310a70: cmp r2, r3
00310a74: bne #0x310a1c
00310a78: b #0x310a34
00310a7c: bl #0x30e868
00310a80: ldr r3, [r4, #0x14]
00310a84: ldr r2, [r4, #0x10]
00310a88: mov r0, r4
00310a8c: rsb r2, r3, r2
00310a90: add r1, r7, r2
00310a94: mov r2, r6
00310a98: bl #0x310804
00310a9c: b #0x310a54

# 0x31167c _ZNSt4priv12_String_baseIcSaIcEE17_M_allocate_blockEj
0031167c: push {r4, lr}
00311680: cmp r1, #0
00311684: sub sp, sp, #8
00311688: mov r4, r0
0031168c: beq #0x3116c8
00311690: cmp r1, #0x10
00311694: bls #0x3116c0
00311698: cmp r1, #0x80
0031169c: str r1, [sp, #4]
003116a0: bhi #0x3116d8
003116a4: add r0, sp, #4
003116a8: bl #0x708ec0
003116ac: ldr r3, [sp, #4]
003116b0: str r0, [r4, #0x14]
003116b4: str r0, [r4, #0x10]
003116b8: add r0, r0, r3
003116bc: str r0, [r4]
003116c0: add sp, sp, #8
003116c4: pop {r4, pc}
003116c8: ldr r0, [pc, #0x14]
003116cc: add r0, pc, r0
003116d0: bl #0x708e40
003116d4: b #0x3116c0
003116d8: mov r0, r1
003116dc: bl #0x310454
003116e0: b #0x3116ac
003116e4: subseq ip, sl, ip, lsl #27

# 0x3410fc _Z14GetNewInstanceI5DecorEP10ObjectBasev
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

# 0x342600 _Z14GetNewInstanceI13AnimatedDecorEP10ObjectBasev
00342600: push {r4, r5, r6, lr}
00342604: mov r1, #0
00342608: mov r0, #0x394
0034260c: bl #0x310570
00342610: ldr r6, [pc, #0x70]
00342614: mov r1, #0x14
00342618: mov r4, r0
0034261c: bl #0x38c398
00342620: ldr r3, [pc, #0x64]
00342624: add r6, pc, r6
00342628: add r2, r4, #0x37c
0034262c: ldr r3, [r6, r3]
00342630: mov r5, #1
00342634: mov r0, r2
00342638: add ip, r3, #8
0034263c: add r1, r3, #0xe4
00342640: add r3, r3, #0xd8
00342644: str r3, [r4, #4]
00342648: str r1, [r4, #0x24]
0034264c: str r2, [r4, #0x38c]
00342650: str r2, [r4, #0x390]
00342654: str ip, [r4]
00342658: strb r5, [r4, #0x375]
0034265c: strb r5, [r4, #0x376]
00342660: strb r5, [r4, #0x84]
00342664: mov r1, #0x10
00342668: bl #0x31167c
0034266c: ldr r2, [r4, #0x38c]
00342670: mov r3, #0
00342674: mov r0, r4
00342678: strb r3, [r2]
0034267c: strb r5, [r4, #0x84]
00342680: strb r3, [r4, #0x375]
00342684: pop {r4, r5, r6, pc}
00342688: rsbeq r2, r5, ip, ror #8
0034268c: andeq r1, r0, r8, lsr sp

# 0x3883d4 _ZNK5Decor11IsUpdatableEv
003883d4: mov r0, #0
003883d8: bx lr

# 0x3883dc _ZNK13AnimatedDecor10IsAnimatedEv
003883dc: mov r0, #1
003883e0: bx lr

# 0x388420 _ZThn36_N5DecorD1Ev
00388420: sub r0, r0, #0x24
00388424: b #0x388428

# 0x388428 _ZN5DecorD1Ev
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

# 0x3884f8 _ZNK13AnimatedDecor9IsZonableEv
003884f8: b #0x38ab60

# 0x388730 _ZN5Decor12LoadFloorMapEv
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

# 0x388a2c _ZN7PODecorC1EP13PhysicalWorldP10GameObjectb.clone.2
00388a2c: push {r4, r5, lr}
00388a30: mov lr, #1
00388a34: sub sp, sp, #0x24
00388a38: mov r5, #2
00388a3c: mov ip, #0
00388a40: mov r3, lr
00388a44: str r5, [sp, #0x10]
00388a48: ldr r4, [pc, #0x40]
00388a4c: movw r5, #0xffff
00388a50: str r5, [sp, #0x14]
00388a54: str ip, [sp, #0xc]
00388a58: mov r5, r0
00388a5c: str ip, [sp]
00388a60: str ip, [sp, #4]
00388a64: str ip, [sp, #8]
00388a68: str lr, [sp, #0x18]
00388a6c: bl #0x46f2f0
00388a70: ldr r3, [pc, #0x1c]
00388a74: add r4, pc, r4
00388a78: mov r0, r5
00388a7c: ldr r3, [r4, r3]
00388a80: add r3, r3, #8
00388a84: str r3, [r5]
00388a88: add sp, sp, #0x24
00388a8c: pop {r4, r5, pc}
00388a90: rsbeq ip, r0, ip, lsl r0
00388a94: andeq r2, r0, r8, lsl r4

# 0x388a98 _ZN5Decor8InitPostEv
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

# 0x388c58 _ZN6Random9GetRandomEib.clone.3
00388c58: push {r4, lr}
00388c5c: ldr r4, [pc, #0x7c]
00388c60: cmp r0, #0
00388c64: add r4, pc, r4
00388c68: beq #0x388cc8
00388c6c: ldr r2, [pc, #0x70]
00388c70: mov r1, r0
00388c74: movw r0, #0xe6ab
00388c78: ldr r2, [r4, r2]
00388c7c: movw r3, #0xdb17
00388c80: movt r3, #0x2b52
00388c84: ldr lr, [r2]
00388c88: movw ip, #0xf26b
00388c8c: movt ip, #0xda
00388c90: mul r0, r0, lr
00388c94: add r0, r0, #0x2b000
00388c98: add r0, r0, #0x3fc
00388c9c: add r0, r0, #1
00388ca0: umull lr, r3, r3, r0
00388ca4: rsb lr, r3, r0
00388ca8: add r3, r3, lr, lsr #1
00388cac: lsr r3, r3, #0x17
00388cb0: mls r3, ip, r3, r0
00388cb4: mov r0, r3
00388cb8: str r3, [r2]
00388cbc: bl #0x30eb2c
00388cc0: eor r0, r1, r1, asr #31
00388cc4: sub r0, r0, r1, asr #31
00388cc8: ldr r3, [pc, #0x18]
00388ccc: ldr r3, [r4, r3]
00388cd0: ldr r2, [r3]
00388cd4: add r2, r2, #1
00388cd8: str r2, [r3]
00388cdc: pop {r4, pc}
00388ce0: rsbeq fp, r0, ip, lsr #28
00388ce4: muleq r0, r4, ip
00388ce8: andeq r1, r0, r8, lsl #1

# 0x388cec _ZN13AnimatedDecor19__CallbackRandomAllEPN6glitch5scene19ITimelineControllerEPv
00388cec: push {r4, r5, lr}
00388cf0: ldr r3, [r1, #0x2d8]
00388cf4: sub sp, sp, #0xc
00388cf8: mov r5, r1
00388cfc: ldr r3, [r3, #0x38]
00388d00: mov r1, #0
00388d04: ldr r4, [pc, #0xa8]
00388d08: mov r0, r3
00388d0c: ldr r3, [r3]
00388d10: mov lr, pc
00388d14: ldr pc, [r3, #0x10]
00388d18: sub r0, r0, #1
00388d1c: bl #0x388c58
00388d20: ldr r3, [r5, #0x2d8]
00388d24: mov lr, #0
00388d28: mov r1, r0
00388d2c: ldr ip, [r3, #0x38]
00388d30: mov r2, lr
00388d34: mov r3, lr
00388d38: mov r0, ip
00388d3c: ldr ip, [ip]
00388d40: str lr, [sp]
00388d44: mov lr, pc
00388d48: ldr pc, [ip, #0x1c]
00388d4c: cmp r0, #0
00388d50: add r4, pc, r4
00388d54: bne #0x388d78
00388d58: ldr r3, [pc, #0x58]
00388d5c: ldr r3, [r4, r3]
00388d60: ldr r3, [r3]
00388d64: cmp r3, #2
00388d68: streq r0, [r0]
00388d6c: beq #0x388d78
00388d70: cmp r3, #1
00388d74: beq #0x388d80
00388d78: add sp, sp, #0xc
00388d7c: pop {r4, r5, pc}
00388d80: ldr r0, [pc, #0x34]
00388d84: ldr r1, [pc, #0x34]
00388d88: ldr r2, [pc, #0x34]
00388d8c: ldr r0, [r4, r0]
00388d90: ldr r3, [pc, #0x30]
00388d94: movw ip, #0x159
00388d98: add r1, pc, r1
00388d9c: add r2, pc, r2
00388da0: add r3, pc, r3
00388da4: add r0, r0, #0xa8
00388da8: str ip, [sp]
00388dac: bl #0x30e004
00388db0: b #0x388d78
00388db4: rsbeq fp, r0, r0, asr #26
00388db8: andeq r3, r0, r0, asr #19
00388dbc: andeq r1, r0, r0, asr #19
00388dc0: subseq r5, r3, r0, asr #12
00388dc4: subseq sb, r3, r4, asr #9
00388dc8: ldrsbeq sb, [r3], #-0x40

# 0x388ea8 _ZThn36_N5DecorD0Ev
00388ea8: sub r0, r0, #0x24
00388eac: b #0x388eb0

# 0x388eb0 _ZN5DecorD0Ev
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

# 0x389090 _ZThn36_N13AnimatedDecorD1Ev
00389090: sub r0, r0, #0x24
00389094: b #0x389098

# 0x389098 _ZN13AnimatedDecorD1Ev
00389098: push {r4, r5, r6, lr}
0038909c: ldr r5, [pc, #0x54]
003890a0: ldr r3, [pc, #0x54]
003890a4: mov r4, r0
003890a8: add r5, pc, r5
003890ac: ldr r3, [r5, r3]
003890b0: add r0, r0, #0x37c
003890b4: add r2, r3, #0xe4
003890b8: add r1, r3, #8
003890bc: add r3, r3, #0xd8
003890c0: stm r4, {r1, r3}
003890c4: str r2, [r4, #0x24]
003890c8: bl #0x3139ac
003890cc: ldr r3, [pc, #0x2c]
003890d0: mov r0, r4
003890d4: ldr r3, [r5, r3]
003890d8: add r2, r3, #0xe4
003890dc: add r1, r3, #8
003890e0: add r3, r3, #0xd8
003890e4: stm r4, {r1, r3}
003890e8: str r2, [r4, #0x24]
003890ec: bl #0x38d378
003890f0: mov r0, r4
003890f4: pop {r4, r5, r6, pc}
003890f8: rsbeq fp, r0, r8, ror #19
003890fc: andeq r1, r0, r8, lsr sp
00389100: andeq r2, r0, ip, lsl #22

# 0x389104 _ZThn36_N13AnimatedDecorD0Ev
00389104: sub r0, r0, #0x24
00389108: b #0x38910c

# 0x38910c _ZN13AnimatedDecorD0Ev
0038910c: push {r4, lr}
00389110: mov r4, r0
00389114: bl #0x389098
00389118: mov r0, r4
0038911c: bl #0x310440
00389120: mov r0, r4
00389124: pop {r4, pc}

# 0x389128 _ZN13AnimatedDecor8InitPostEv
00389128: push {r4, r5, r6, r7, r8, lr}
0038912c: mov r3, #1
00389130: strb r3, [r0, #0x10c]
00389134: sub sp, sp, #8
00389138: mov r4, r0
0038913c: bl #0x388a98
00389140: mov r0, r4
00389144: bl #0x38ab60
00389148: ldr r5, [pc, #0x1d8]
0038914c: subs r1, r0, #0
00389150: add r5, pc, r5
00389154: beq #0x3892bc
00389158: ldr r8, [r4, #0x2d8]
0038915c: cmp r8, #0
00389160: beq #0x38923c
00389164: ldr r7, [r4, #0x390]
00389168: ldr r3, [r4, #0x38c]
0038916c: cmp r3, r7
00389170: beq #0x389308
00389174: ldr r1, [pc, #0x1b0]
00389178: mov r0, r7
0038917c: add r1, pc, r1
00389180: bl #0x30e6e8
00389184: subs r6, r0, #0
00389188: beq #0x389244
0038918c: ldr r3, [r8, #0x38]
00389190: mov r1, r7
00389194: mov r2, #0
00389198: mov r0, r3
0038919c: ldr r3, [r3]
003891a0: mov lr, pc
003891a4: ldr pc, [r3, #0x14]
003891a8: cmp r0, #0
003891ac: bne #0x3892d0
003891b0: ldr r3, [r4, #0x2d8]
003891b4: mov lr, #0
003891b8: mov r1, lr
003891bc: ldr ip, [r3, #0x38]
003891c0: mov r2, #1
003891c4: mov r3, lr
003891c8: mov r0, ip
003891cc: ldr ip, [ip]
003891d0: str lr, [sp]
003891d4: mov lr, pc
003891d8: ldr pc, [ip, #0x1c]
003891dc: ldr r0, [r4, #0x2d8]
003891e0: bl #0x470a54
003891e4: ldr r3, [r4, #0x2d8]
003891e8: ldrb r3, [r3, #0x28]
003891ec: cmp r3, #0
003891f0: beq #0x38922c
003891f4: ldr r3, [pc, #0x134]
003891f8: mov r1, #0
003891fc: mov r0, #0x28
00389200: ldr r3, [r5, r3]
00389204: ldr r6, [r3, #0x44]
00389208: bl #0x310570
0038920c: mov r1, r6
00389210: mov r5, r0
00389214: mov r2, r4
00389218: bl #0x388a2c
0038921c: mov r0, r4
00389220: mov r1, r5
00389224: mov r2, #0
00389228: bl #0x394bf8
0038922c: mov r0, r4
00389230: ldr r3, [r4]
00389234: mov lr, pc
00389238: ldr pc, [r3, #0x2c]
0038923c: add sp, sp, #8
00389240: pop {r4, r5, r6, r7, r8, pc}
00389244: ldr r3, [r8, #0x38]
00389248: mov r1, r6
0038924c: mov r0, r3
00389250: ldr r3, [r3]
00389254: mov lr, pc
00389258: ldr pc, [r3, #0x10]
0038925c: sub r0, r0, #1
00389260: bl #0x388c58
00389264: ldr r3, [r4, #0x2d8]
00389268: mov r1, r0
0038926c: mov r2, r6
00389270: ldr ip, [r3, #0x38]
00389274: mov r3, r6
00389278: mov r0, ip
0038927c: ldr ip, [ip]
00389280: str r6, [sp]
00389284: mov lr, pc
00389288: ldr pc, [ip, #0x1c]
0038928c: ldr r2, [r4, #0x2d8]
00389290: mov r3, r6
00389294: ldr ip, [r2, #0x38]
00389298: ldr r2, [pc, #0x94]
0038929c: mov r0, ip
003892a0: ldr r1, [r5, r2]
003892a4: ldr ip, [ip]
003892a8: mov r2, r4
003892ac: str r4, [sp]
003892b0: mov lr, pc
003892b4: ldr pc, [ip, #0x2c]
003892b8: b #0x3891dc
003892bc: mov r0, r4
003892c0: ldr r3, [r4]
003892c4: mov lr, pc
003892c8: ldr pc, [r3, #0x40]
003892cc: b #0x38923c
003892d0: ldr r3, [r4, #0x2d8]
003892d4: mov r2, #0
003892d8: ldr r1, [r4, #0x390]
003892dc: ldr ip, [r3, #0x38]
003892e0: mov r3, r2
003892e4: mov r0, ip
003892e8: ldr ip, [ip]
003892ec: str r2, [sp]
003892f0: mov r2, #1
003892f4: mov lr, pc
003892f8: ldr pc, [ip, #0x20]
003892fc: cmp r0, #0
00389300: bne #0x3891dc
00389304: b #0x3891b0
00389308: ldr r1, [pc, #0x28]
0038930c: add r0, r4, #0x37c
00389310: add r1, pc, r1
00389314: add r2, r1, #4
00389318: bl #0x3109e0
0038931c: ldr r8, [r4, #0x2d8]
00389320: ldr r7, [r4, #0x390]
00389324: b #0x389174
00389328: rsbeq fp, r0, r0, asr #18
0038932c: subseq sb, r3, ip, lsr r1
00389330: strdeq r3, r4, [r0], -r4
00389334: ldrdeq r3, r4, [r0], -r0
00389338: subseq r8, r3, r0, lsr #31

# 0x3899c8 _ZThn4_N5Decor17DeclarePropertiesEv
003899c8: sub r0, r0, #4
003899cc: b #0x3899d0

# 0x3899d0 _ZN5Decor17DeclarePropertiesEv
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

# 0x389dcc _ZThn4_N13AnimatedDecor17DeclarePropertiesEv
00389dcc: sub r0, r0, #4
00389dd0: b #0x389dd4

# 0x389dd4 _ZN13AnimatedDecor17DeclarePropertiesEv
00389dd4: push {r4, lr}
00389dd8: mov r4, r0
00389ddc: bl #0x3899d0
00389de0: ldr r1, [pc, #0x10]
00389de4: add r2, r4, #0x37c
00389de8: add r0, r4, #4
00389dec: add r1, pc, r1
00389df0: pop {r4, lr}
00389df4: b #0x33ef7c
00389df8: subseq r8, r3, r4, lsl r5

# 0x38ab60 _ZNK10GameObject13MeetConditionEv
0038ab60: mov r0, #1
0038ab64: bx lr
