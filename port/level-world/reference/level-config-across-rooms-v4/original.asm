# 0x310454 _Z11CustomAllocj
00310454: push {r4, r5, lr}
00310458: sub sp, sp, #0xc
0031045c: bl #0x30e6f4
00310460: ldr r5, [pc, #0x74]
00310464: subs r4, r0, #0
00310468: add r5, pc, r5
0031046c: beq #0x31047c
00310470: mov r0, r4
00310474: add sp, sp, #0xc
00310478: pop {r4, r5, pc}
0031047c: ldr r0, [pc, #0x5c]
00310480: add r0, pc, r0
00310484: bl #0x31041c
00310488: ldr r3, [pc, #0x54]
0031048c: ldr r3, [r5, r3]
00310490: ldr r3, [r3]
00310494: cmp r3, #2
00310498: streq r4, [r4]
0031049c: beq #0x310470
003104a0: cmp r3, #1
003104a4: bne #0x310470
003104a8: ldr r0, [pc, #0x38]
003104ac: ldr r1, [pc, #0x38]
003104b0: ldr r2, [pc, #0x38]
003104b4: ldr r0, [r5, r0]
003104b8: ldr r3, [pc, #0x34]
003104bc: mov ip, #0x3a
003104c0: add r1, pc, r1
003104c4: add r2, pc, r2
003104c8: add r3, pc, r3
003104cc: add r0, r0, #0xa8
003104d0: str ip, [sp]
003104d4: bl #0x30e004
003104d8: b #0x310470
003104dc: rsbeq r4, r8, r8, lsr #12
003104e0: subseq sp, sl, r8, asr #30
003104e4: andeq r3, r0, r0, asr #19
003104e8: andeq r1, r0, r0, asr #19
003104ec: subseq sp, sl, r8, lsl pc
003104f0: subseq sp, sl, r4, lsr pc
003104f4: subseq sp, sl, r8, lsr pc

# 0x310570 _Znwj15MemoryHintState
00310570: b #0x310454

# 0x310648 _Z8VoxAllocjN3vox10VoxMemHintE
00310648: subs r3, r1, #0
0031064c: mov r1, r0
00310650: ble #0x31065c
00310654: mov r0, r3
00310658: b #0x3105b4
0031065c: b #0x310454

# 0x33f15c _ZN10ObjectBaseC1ENS_6GO_IDSE
0033f15c: push {r4, r5, r6, r7, r8, lr}
0033f160: ldr r6, [pc, #0x198]
0033f164: ldr r2, [pc, #0x198]
0033f168: ldr r3, [pc, #0x198]
0033f16c: add r6, pc, r6
0033f170: ldr r2, [r6, r2]
0033f174: ldr r3, [r6, r3]
0033f178: mov r4, r0
0033f17c: add r2, r2, #8
0033f180: add r0, r3, #8
0033f184: add r3, r4, #8
0033f188: str r2, [r4]
0033f18c: str r0, [r4, #4]
0033f190: mov r7, r1
0033f194: mov r0, r3
0033f198: str r3, [r4, #0x18]
0033f19c: str r3, [r4, #0x1c]
0033f1a0: mov r1, #0x10
0033f1a4: bl #0x31167c
0033f1a8: ldr r2, [pc, #0x15c]
0033f1ac: ldr r1, [r4, #0x18]
0033f1b0: mov r5, #0
0033f1b4: ldr r2, [r6, r2]
0033f1b8: strb r5, [r1]
0033f1bc: add r3, r4, #0x30
0033f1c0: add r1, r2, #0x74
0033f1c4: add r0, r2, #8
0033f1c8: add r2, r2, #0x68
0033f1cc: stm r4, {r0, r2}
0033f1d0: str r1, [r4, #0x24]
0033f1d4: mov r0, r3
0033f1d8: str r5, [r4, #0x20]
0033f1dc: strb r5, [r4, #0x28]
0033f1e0: strb r5, [r4, #0x29]
0033f1e4: str r5, [r4, #0x2c]
0033f1e8: str r3, [r4, #0x40]
0033f1ec: str r3, [r4, #0x44]
0033f1f0: mov r1, #0x10
0033f1f4: bl #0x31167c
0033f1f8: ldr r2, [r4, #0x40]
0033f1fc: add r3, r4, #0x48
0033f200: mov r0, r3
0033f204: strb r5, [r2]
0033f208: mov r1, #0x10
0033f20c: str r3, [r4, #0x58]
0033f210: str r3, [r4, #0x5c]
0033f214: bl #0x31167c
0033f218: ldr r2, [r4, #0x58]
0033f21c: add r3, r4, #0x68
0033f220: mvn r6, #0
0033f224: strb r5, [r2]
0033f228: mov r1, #0x10
0033f22c: mov r0, r3
0033f230: strb r5, [r4, #0x60]
0033f234: str r3, [r4, #0x78]
0033f238: str r3, [r4, #0x7c]
0033f23c: str r6, [r4, #0x64]
0033f240: bl #0x31167c
0033f244: ldr r3, [r4, #0x78]
0033f248: add r0, r4, #0x8c
0033f24c: strb r5, [r3]
0033f250: mov r3, #1
0033f254: strb r3, [r4, #0x8a]
0033f258: strb r5, [r4, #0x81]
0033f25c: strb r5, [r4, #0x84]
0033f260: strb r5, [r4, #0x85]
0033f264: strb r5, [r4, #0x86]
0033f268: strb r5, [r4, #0x88]
0033f26c: strb r5, [r4, #0x89]
0033f270: bl #0x33ed7c
0033f274: add r0, r4, #0xb0
0033f278: bl #0x33ed7c
0033f27c: add r3, r4, #0xd4
0033f280: mov r0, r3
0033f284: str r3, [r4, #0xe4]
0033f288: str r3, [r4, #0xe8]
0033f28c: mov r1, #0x10
0033f290: bl #0x31167c
0033f294: ldr r3, [r4, #0xe4]
0033f298: mov r1, r5
0033f29c: mov r0, #0xc
0033f2a0: strb r5, [r3]
0033f2a4: mov r3, #0
0033f2a8: str r3, [r4, #0x114]
0033f2ac: strb r5, [r4, #0xf0]
0033f2b0: strb r5, [r4, #0xf1]
0033f2b4: strb r5, [r4, #0xf8]
0033f2b8: str r5, [r4, #0xfc]
0033f2bc: str r5, [r4, #0x100]
0033f2c0: str r5, [r4, #0x104]
0033f2c4: strb r5, [r4, #0x10c]
0033f2c8: strb r5, [r4, #0x118]
0033f2cc: strb r5, [r4, #0x119]
0033f2d0: str r5, [r4, #0x11c]
0033f2d4: str r7, [r4, #0xf4]
0033f2d8: str r6, [r4, #0x110]
0033f2dc: str r6, [r4, #0xec]
0033f2e0: str r6, [r4, #0x108]
0033f2e4: bl #0x310570
0033f2e8: mov r5, r0
0033f2ec: bl #0x33f50c
0033f2f0: str r5, [r4, #0x2c]
0033f2f4: mov r0, r4
0033f2f8: str r4, [r5, #4]
0033f2fc: pop {r4, r5, r6, r7, r8, pc}
0033f300: rsbeq r5, r5, r4, lsr #18
0033f304: andeq r1, r0, ip, lsl #1
0033f308: ldrdeq r3, r4, [r0], -ip
0033f30c: andeq r3, r0, r4, lsl #23

# 0x33f310 _ZN10ObjectBaseC2ENS_6GO_IDSE
0033f310: push {r4, r5, r6, r7, r8, lr}
0033f314: ldr r6, [pc, #0x198]
0033f318: ldr r2, [pc, #0x198]
0033f31c: ldr r3, [pc, #0x198]
0033f320: add r6, pc, r6
0033f324: ldr r2, [r6, r2]
0033f328: ldr r3, [r6, r3]
0033f32c: mov r4, r0
0033f330: add r2, r2, #8
0033f334: add r0, r3, #8
0033f338: add r3, r4, #8
0033f33c: str r2, [r4]
0033f340: str r0, [r4, #4]
0033f344: mov r7, r1
0033f348: mov r0, r3
0033f34c: str r3, [r4, #0x18]
0033f350: str r3, [r4, #0x1c]
0033f354: mov r1, #0x10
0033f358: bl #0x31167c
0033f35c: ldr r2, [pc, #0x15c]
0033f360: ldr r1, [r4, #0x18]
0033f364: mov r5, #0
0033f368: ldr r2, [r6, r2]
0033f36c: strb r5, [r1]
0033f370: add r3, r4, #0x30
0033f374: add r1, r2, #0x74
0033f378: add r0, r2, #8
0033f37c: add r2, r2, #0x68
0033f380: stm r4, {r0, r2}
0033f384: str r1, [r4, #0x24]
0033f388: mov r0, r3
0033f38c: str r5, [r4, #0x20]
0033f390: strb r5, [r4, #0x28]
0033f394: strb r5, [r4, #0x29]
0033f398: str r5, [r4, #0x2c]
0033f39c: str r3, [r4, #0x40]
0033f3a0: str r3, [r4, #0x44]
0033f3a4: mov r1, #0x10
0033f3a8: bl #0x31167c
0033f3ac: ldr r2, [r4, #0x40]
0033f3b0: add r3, r4, #0x48
0033f3b4: mov r0, r3
0033f3b8: strb r5, [r2]
0033f3bc: mov r1, #0x10
0033f3c0: str r3, [r4, #0x58]
0033f3c4: str r3, [r4, #0x5c]
0033f3c8: bl #0x31167c
0033f3cc: ldr r2, [r4, #0x58]
0033f3d0: add r3, r4, #0x68
0033f3d4: mvn r6, #0
0033f3d8: strb r5, [r2]
0033f3dc: mov r1, #0x10
0033f3e0: mov r0, r3
0033f3e4: strb r5, [r4, #0x60]
0033f3e8: str r3, [r4, #0x78]
0033f3ec: str r3, [r4, #0x7c]
0033f3f0: str r6, [r4, #0x64]
0033f3f4: bl #0x31167c
0033f3f8: ldr r3, [r4, #0x78]
0033f3fc: add r0, r4, #0x8c
0033f400: strb r5, [r3]
0033f404: mov r3, #1
0033f408: strb r3, [r4, #0x8a]
0033f40c: strb r5, [r4, #0x81]
0033f410: strb r5, [r4, #0x84]
0033f414: strb r5, [r4, #0x85]
0033f418: strb r5, [r4, #0x86]
0033f41c: strb r5, [r4, #0x88]
0033f420: strb r5, [r4, #0x89]
0033f424: bl #0x33ed7c
0033f428: add r0, r4, #0xb0
0033f42c: bl #0x33ed7c
0033f430: add r3, r4, #0xd4
0033f434: mov r0, r3
0033f438: str r3, [r4, #0xe4]
0033f43c: str r3, [r4, #0xe8]
0033f440: mov r1, #0x10
0033f444: bl #0x31167c
0033f448: ldr r3, [r4, #0xe4]
0033f44c: mov r1, r5
0033f450: mov r0, #0xc
0033f454: strb r5, [r3]
0033f458: mov r3, #0
0033f45c: str r3, [r4, #0x114]
0033f460: strb r5, [r4, #0xf0]
0033f464: strb r5, [r4, #0xf1]
0033f468: strb r5, [r4, #0xf8]
0033f46c: str r5, [r4, #0xfc]
0033f470: str r5, [r4, #0x100]
0033f474: str r5, [r4, #0x104]
0033f478: strb r5, [r4, #0x10c]
0033f47c: strb r5, [r4, #0x118]
0033f480: strb r5, [r4, #0x119]
0033f484: str r5, [r4, #0x11c]
0033f488: str r7, [r4, #0xf4]
0033f48c: str r6, [r4, #0x110]
0033f490: str r6, [r4, #0xec]
0033f494: str r6, [r4, #0x108]
0033f498: bl #0x310570
0033f49c: mov r5, r0
0033f4a0: bl #0x33f50c
0033f4a4: str r5, [r4, #0x2c]
0033f4a8: mov r0, r4
0033f4ac: str r4, [r5, #4]
0033f4b0: pop {r4, r5, r6, r7, r8, pc}
0033f4b4: rsbeq r5, r5, r0, ror r7
0033f4b8: andeq r1, r0, ip, lsl #1
0033f4bc: ldrdeq r3, r4, [r0], -ip
0033f4c0: andeq r3, r0, r4, lsl #23

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

# 0x34aca0 _ZN13ObjectManager15GetObjectByNameEPKcibS1_
0034aca0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034aca4: ldr r5, [pc, #0x38c]
0034aca8: ldr ip, [pc, #0x38c]
0034acac: sub sp, sp, #0x74
0034acb0: add r5, pc, r5
0034acb4: str ip, [sp, #0x14]
0034acb8: ldr ip, [r5, ip]
0034acbc: str r1, [sp, #0x18]
0034acc0: ldr r1, [pc, #0x378]
0034acc4: ldr ip, [ip]
0034acc8: str r0, [sp, #0xc]
0034accc: mov r4, r2
0034acd0: mov r0, r2
0034acd4: add r1, pc, r1
0034acd8: mov r2, #6
0034acdc: str ip, [sp, #0x6c]
0034ace0: mov r7, r3
0034ace4: bl #0x30ec7c
0034ace8: ldrb lr, [sp, #0x98]
0034acec: cmp r0, #0
0034acf0: ldr r6, [sp, #0x9c]
0034acf4: str lr, [sp, #0x24]
0034acf8: beq #0x34ad18
0034acfc: ldr r1, [pc, #0x340]
0034ad00: mov r0, r4
0034ad04: mov r2, #0x10
0034ad08: add r1, pc, r1
0034ad0c: bl #0x30ec7c
0034ad10: cmp r0, #0
0034ad14: bne #0x34af98
0034ad18: mvn r7, #0
0034ad1c: ldr r1, [pc, #0x324]
0034ad20: mov r0, r4
0034ad24: add r1, pc, r1
0034ad28: bl #0x30e31c
0034ad2c: cmp r0, #0
0034ad30: beq #0x34afb8
0034ad34: add lr, sp, #0x40
0034ad38: mov r0, lr
0034ad3c: str lr, [sp, #0x20]
0034ad40: bl #0x33f50c
0034ad44: ldr r3, [pc, #0x300]
0034ad48: ldr r0, [sp, #0x18]
0034ad4c: ldr sb, [pc, #0x2fc]
0034ad50: ldr r2, [pc, #0x2fc]
0034ad54: add r3, pc, r3
0034ad58: ldr r6, [r0, #0x14]
0034ad5c: str r3, [sp, #8]
0034ad60: add r3, sp, #0x34
0034ad64: add sb, pc, sb
0034ad68: add r8, r0, #0xc
0034ad6c: str r2, [sp, #0x10]
0034ad70: add sl, sp, #0x28
0034ad74: str r3, [sp, #0x1c]
0034ad78: cmp r6, r8
0034ad7c: beq #0x34ae98
0034ad80: ldr r3, [r6, #0x2c]
0034ad84: cmp r3, #0
0034ad88: beq #0x34ae6c
0034ad8c: cmn r7, #1
0034ad90: beq #0x34adac
0034ad94: ldr r2, [r3, #0x64]
0034ad98: cmp r7, r2
0034ad9c: beq #0x34adac
0034ada0: ldrb r3, [r3, #0x87]
0034ada4: cmp r3, #0
0034ada8: beq #0x34ae6c
0034adac: ldr r0, [r6, #0x28]
0034adb0: mov r1, r4
0034adb4: bl #0x30e31c
0034adb8: cmp r0, #0
0034adbc: bne #0x34ae10
0034adc0: ldr r0, [sp, #0x20]
0034adc4: ldr r1, [r6, #0x10]
0034adc8: add r2, r0, #4
0034adcc: ldr r0, [r2], #4
0034add0: ldr r3, [sp, #0xc]
0034add4: ldr r2, [r2]
0034add8: str r1, [r3], #4
0034addc: ldr ip, [sp, #0xc]
0034ade0: str r2, [r3, #4]
0034ade4: str r0, [ip, #4]
0034ade8: str r1, [sp, #0x40]
0034adec: ldr r0, [sp, #0x14]
0034adf0: ldr r2, [sp, #0x6c]
0034adf4: ldr r3, [r5, r0]
0034adf8: ldr r0, [sp, #0xc]
0034adfc: ldr r3, [r3]
0034ae00: cmp r2, r3
0034ae04: bne #0x34b034
0034ae08: add sp, sp, #0x74
0034ae0c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034ae10: mov r1, sb
0034ae14: mov r0, r4
0034ae18: bl #0x30e31c
0034ae1c: subs r1, r0, #0
0034ae20: beq #0x34af60
0034ae24: ldr r1, [sp, #8]
0034ae28: mov r0, r4
0034ae2c: bl #0x30e31c
0034ae30: subs r1, r0, #0
0034ae34: bne #0x34ae6c
0034ae38: ldr lr, [sp, #0x10]
0034ae3c: mov r2, #1
0034ae40: ldr r3, [r5, lr]
0034ae44: ldr r0, [r3, #0x40]
0034ae48: bl #0x36e478
0034ae4c: ldr r1, [r6, #0x2c]
0034ae50: ldr fp, [r0, #0x660]
0034ae54: mov r0, sl
0034ae58: bl #0x33dd2c
0034ae5c: mov r0, sl
0034ae60: bl #0x33ff54
0034ae64: cmp fp, r0
0034ae68: beq #0x34adc0
0034ae6c: ldr r3, [r6, #0xc]
0034ae70: cmp r3, #0
0034ae74: bne #0x34ae80
0034ae78: b #0x34af2c
0034ae7c: mov r3, r2
0034ae80: ldr r2, [r3, #8]
0034ae84: cmp r2, #0
0034ae88: bne #0x34ae7c
0034ae8c: mov r6, r3
0034ae90: cmp r6, r8
0034ae94: bne #0x34ad80
0034ae98: ldr lr, [sp, #0x24]
0034ae9c: cmp lr, #0
0034aea0: bne #0x34afd8
0034aea4: ldr r3, [pc, #0x1ac]
0034aea8: add r4, sp, #0x54
0034aeac: ldr r6, [r5, r3]
0034aeb0: mov r0, r6
0034aeb4: bl #0x337888
0034aeb8: ldr r1, [pc, #0x19c]
0034aebc: add r2, sp, #0x50
0034aec0: mov r0, r4
0034aec4: add r1, pc, r1
0034aec8: bl #0x3140ec
0034aecc: mov r0, r6
0034aed0: mov r1, r4
0034aed4: bl #0x337a88
0034aed8: ldr r0, [sp, #0x68]
0034aedc: cmp r0, r4
0034aee0: beq #0x34af00
0034aee4: cmp r0, #0
0034aee8: beq #0x34af00
0034aeec: ldr r1, [sp, #0x54]
0034aef0: rsb r1, r0, r1
0034aef4: cmp r1, #0x80
0034aef8: bhi #0x34b02c
0034aefc: bl #0x708f00
0034af00: ldr r0, [sp, #0x20]
0034af04: ldr r3, [sp, #0xc]
0034af08: add r2, r0, #4
0034af0c: ldr r1, [r2], #4
0034af10: ldr r0, [sp, #0x40]
0034af14: ldr r2, [r2]
0034af18: str r0, [r3], #4
0034af1c: ldr ip, [sp, #0xc]
0034af20: str r2, [r3, #4]
0034af24: str r1, [ip, #4]
0034af28: b #0x34adec
0034af2c: ldr r2, [r6, #4]
0034af30: ldr r1, [r2, #0xc]
0034af34: cmp r6, r1
0034af38: bne #0x34af54
0034af3c: mov r6, r2
0034af40: ldr r2, [r2, #4]
0034af44: ldr r3, [r2, #0xc]
0034af48: cmp r3, r6
0034af4c: beq #0x34af3c
0034af50: ldr r3, [r6, #0xc]
0034af54: cmp r2, r3
0034af58: movne r6, r2
0034af5c: b #0x34ad78
0034af60: ldr ip, [sp, #0x10]
0034af64: mov r2, #1
0034af68: ldr r3, [r5, ip]
0034af6c: ldr r0, [r3, #0x40]
0034af70: bl #0x36e478
0034af74: ldr r1, [r6, #0x2c]
0034af78: ldr fp, [r0, #0x660]
0034af7c: ldr r0, [sp, #0x1c]
0034af80: bl #0x33dd2c
0034af84: ldr r0, [sp, #0x1c]
0034af88: bl #0x33ff54
0034af8c: cmp fp, r0
0034af90: bne #0x34ae24
0034af94: b #0x34adc0
0034af98: ldr r1, [pc, #0xc0]
0034af9c: mov r0, r4
0034afa0: mov r2, #0xb
0034afa4: add r1, pc, r1
0034afa8: bl #0x30ec7c
0034afac: cmp r0, #0
0034afb0: bne #0x34ad1c
0034afb4: b #0x34ad18
0034afb8: ldr ip, [sp, #0x24]
0034afbc: ldr r1, [sp, #0x18]
0034afc0: mov r2, r6
0034afc4: mov r3, r7
0034afc8: ldr r0, [sp, #0xc]
0034afcc: str ip, [sp]
0034afd0: bl #0x34b064
0034afd4: b #0x34adec
0034afd8: ldr r0, [sp, #0x18]
0034afdc: ldr r2, [sp, #0x18]
0034afe0: add r1, sp, #0x70
0034afe4: ldr r3, [r0, #0x4c]
0034afe8: mov r0, r6
0034afec: str r3, [r1, #-0x24]!
0034aff0: add r3, r3, #1
0034aff4: str r3, [r2, #0x4c]
0034aff8: bl #0x34952c
0034affc: mov r6, r0
0034b000: mov r0, r4
0034b004: bl #0x30de54
0034b008: mov r1, r4
0034b00c: add r2, r4, r0
0034b010: mov r0, r6
0034b014: bl #0x3109e0
0034b018: ldr r3, [sp, #0x20]
0034b01c: ldr r1, [sp, #0x4c]
0034b020: add r2, r3, #4
0034b024: ldr r0, [r2], #4
0034b028: b #0x34add0
0034b02c: bl #0x310440
0034b030: b #0x34af00
0034b034: bl #0x30e310
0034b038: rsbeq sb, r4, r0, ror #27
0034b03c: andeq r4, r0, ip, lsr #1
0034b040: ldrsheq r5, [r7], #-0x64
0034b044: subseq r5, r7, r0, asr #12
0034b048: ldrheq r5, [r7], #-0x6c
0034b04c: subseq r5, r7, ip, ror r6
0034b050: subseq r5, r7, r4, ror #12
0034b054: strdeq r3, r4, [r0], -r4
0034b058: andeq r0, r0, r4, lsl #17
0034b05c: subseq r5, r7, r4, lsr r5
0034b060: subseq r5, r7, ip, lsr #8

# 0x34b270 _ZN13ObjectManager3AddEP10ObjectBasePKcS3_ib
0034b270: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b274: ldr r4, [pc, #0x27c]
0034b278: sub sp, sp, #0x2c
0034b27c: subs r6, r2, #0
0034b280: add r4, pc, r4
0034b284: mov r5, r0
0034b288: mov r8, r1
0034b28c: mov r7, r3
0034b290: ldr fp, [sp, #0x50]
0034b294: ldr sl, [sp, #0x54]
0034b298: ldrb sb, [sp, #0x58]
0034b29c: beq #0x34b304
0034b2a0: cmp r7, #0
0034b2a4: beq #0x34b358
0034b2a8: mov r4, #0
0034b2ac: mov ip, #1
0034b2b0: mov r2, r7
0034b2b4: mov r3, sl
0034b2b8: mov r0, r5
0034b2bc: mov r1, r8
0034b2c0: str ip, [sp]
0034b2c4: str r4, [sp, #4]
0034b2c8: bl #0x34aca0
0034b2cc: mov r0, r5
0034b2d0: mov r1, r4
0034b2d4: bl #0x33fdc0
0034b2d8: cmp r0, r4
0034b2dc: beq #0x34b3ac
0034b2e0: cmp r6, r4
0034b2e4: beq #0x34b2f8
0034b2e8: mov r0, r6
0034b2ec: ldr r3, [r6]
0034b2f0: mov lr, pc
0034b2f4: ldr pc, [r3, #4]
0034b2f8: mov r0, r5
0034b2fc: add sp, sp, #0x2c
0034b300: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034b304: ldr r3, [pc, #0x1f0]
0034b308: ldr r3, [r4, r3]
0034b30c: ldr r3, [r3]
0034b310: cmp r3, #2
0034b314: streq r6, [r6]
0034b318: beq #0x34b2a0
0034b31c: cmp r3, #1
0034b320: bne #0x34b2a0
0034b324: ldr r0, [pc, #0x1d4]
0034b328: ldr r1, [pc, #0x1d4]
0034b32c: ldr r2, [pc, #0x1d4]
0034b330: ldr r0, [r4, r0]
0034b334: ldr r3, [pc, #0x1d0]
0034b338: movw ip, #0x428
0034b33c: add r1, pc, r1
0034b340: add r2, pc, r2
0034b344: add r3, pc, r3
0034b348: add r0, r0, #0xa8
0034b34c: str ip, [sp]
0034b350: bl #0x30e004
0034b354: b #0x34b2a0
0034b358: ldr r3, [pc, #0x19c]
0034b35c: ldr r3, [r4, r3]
0034b360: ldr r3, [r3]
0034b364: cmp r3, #2
0034b368: streq r7, [r7]
0034b36c: beq #0x34b2a8
0034b370: cmp r3, #1
0034b374: bne #0x34b2a8
0034b378: ldr r0, [pc, #0x180]
0034b37c: ldr r1, [pc, #0x18c]
0034b380: ldr r2, [pc, #0x18c]
0034b384: ldr r0, [r4, r0]
0034b388: ldr r3, [pc, #0x188]
0034b38c: movw ip, #0x429
0034b390: add r1, pc, r1
0034b394: add r2, pc, r2
0034b398: add r3, pc, r3
0034b39c: add r0, r0, #0xa8
0034b3a0: str ip, [sp]
0034b3a4: bl #0x30e004
0034b3a8: b #0x34b2a8
0034b3ac: mov r1, r5
0034b3b0: add r0, r8, #0xc
0034b3b4: bl #0x33fc88
0034b3b8: str r6, [r0, #0x18]
0034b3bc: ldr r3, [r8, #0x50]
0034b3c0: mov r2, r5
0034b3c4: mov r1, r7
0034b3c8: add r3, r3, #1
0034b3cc: str r3, [r8, #0x50]
0034b3d0: str r6, [r5, #4]
0034b3d4: ldr ip, [r6, #0x2c]
0034b3d8: ldr lr, [r2], #4
0034b3dc: mov r0, r6
0034b3e0: mov r3, ip
0034b3e4: str lr, [r3], #4
0034b3e8: ldr lr, [r5, #4]
0034b3ec: add r4, sp, #0x18
0034b3f0: str lr, [ip, #4]
0034b3f4: ldr r2, [r2, #4]
0034b3f8: str r2, [r3, #4]
0034b3fc: bl #0x34ac18
0034b400: mov r0, fp
0034b404: bl #0x30de54
0034b408: mov r1, fp
0034b40c: add r2, fp, r0
0034b410: add r0, r6, #0x48
0034b414: bl #0x3109e0
0034b418: mov r1, r6
0034b41c: mov r0, r4
0034b420: str sl, [r6, #0x64]
0034b424: bl #0x33dd2c
0034b428: mov r0, r4
0034b42c: bl #0x33ff54
0034b430: subs r7, r0, #0
0034b434: beq #0x34b45c
0034b438: add r4, r8, #0x60
0034b43c: mov r0, r4
0034b440: bl #0x342690
0034b444: str r7, [r0, #8]
0034b448: ldr r3, [r8, #0x64]
0034b44c: str r4, [r0]
0034b450: str r3, [r0, #4]
0034b454: str r0, [r3]
0034b458: str r0, [r8, #0x64]
0034b45c: add r4, sp, #0xc
0034b460: mov r0, r4
0034b464: mov r1, r6
0034b468: bl #0x33dd2c
0034b46c: mov r0, r4
0034b470: mov r1, #0
0034b474: bl #0x33fdc0
0034b478: subs r4, r0, #0
0034b47c: beq #0x34b48c
0034b480: ldr r3, [r4, #0xf4]
0034b484: cmp r3, #5
0034b488: beq #0x34b4a4
0034b48c: cmp sb, #0
0034b490: beq #0x34b2f8
0034b494: mov r0, r8
0034b498: mov r1, r6
0034b49c: bl #0x3431c0
0034b4a0: b #0x34b2f8
0034b4a4: ldr r0, [r6, #0x5c]
0034b4a8: ldr r2, [r6, #0x58]
0034b4ac: rsb r2, r0, r2
0034b4b0: cmp r2, #6
0034b4b4: bne #0x34b48c
0034b4b8: ldr r1, [pc, #0x5c]
0034b4bc: add r1, pc, r1
0034b4c0: bl #0x30e5e0
0034b4c4: cmp r0, #0
0034b4c8: bne #0x34b48c
0034b4cc: mov r3, #0xc
0034b4d0: add r0, sp, #0x28
0034b4d4: str r3, [r0, #-4]!
0034b4d8: bl #0x708ec0
0034b4dc: str r4, [r0, #8]
0034b4e0: ldr r3, [r8, #0x6c]
0034b4e4: add r2, r8, #0x68
0034b4e8: stm r0, {r2, r3}
0034b4ec: str r0, [r3]
0034b4f0: str r0, [r8, #0x6c]
0034b4f4: b #0x34b48c
0034b4f8: rsbeq sb, r4, r0, lsl r8
0034b4fc: andeq r3, r0, r0, asr #19
0034b500: andeq r1, r0, r0, asr #19

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