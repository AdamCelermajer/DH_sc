# canonical-object-factory-v1/original-source.asm lines 418-510
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

# canonical-object-factory-v1/original-source.asm lines 7152-7224
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

# level-config-module-connection-v2/original-source.asm lines 3646-3690
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

# level-config-module-connection-v2/original-source.asm lines 3974-3988
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

