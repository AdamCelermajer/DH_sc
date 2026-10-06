# 0x33dd2c _ZN10ObjectBase9GetHandleEv
0033dd2c: ldr r3, [pc, #0x34]
0033dd30: ldr r2, [pc, #0x34]
0033dd34: push {r4, lr}
0033dd38: add r3, pc, r3
0033dd3c: ldr r2, [r3, r2]
0033dd40: ldr ip, [r1, #0x2c]
0033dd44: mov r4, r0
0033dd48: ldr lr, [r2, #0x38]
0033dd4c: mov r2, #0xc
0033dd50: ldr r3, [lr, #0x78]
0033dd54: str r3, [ip, #8]
0033dd58: ldr r1, [r1, #0x2c]
0033dd5c: bl #0x30df38
0033dd60: mov r0, r4
0033dd64: pop {r4, pc}
0033dd68: rsbeq r6, r5, r8, asr sp
0033dd6c: strdeq r3, r4, [r0], -r4

# 0x33ddc8 _ZN10ObjectBase9SetEnableEb
0033ddc8: ldrb r2, [r0, #0x8a]
0033ddcc: push {r4, lr}
0033ddd0: cmp r2, r1
0033ddd4: beq #0x33ddf0
0033ddd8: cmp r1, #0
0033dddc: strb r1, [r0, #0x8a]
0033dde0: bne #0x33ddf4
0033dde4: ldr r3, [r0]
0033dde8: mov lr, pc
0033ddec: ldr pc, [r3, #0x48]
0033ddf0: pop {r4, pc}
0033ddf4: ldr r3, [r0]
0033ddf8: mov lr, pc
0033ddfc: ldr pc, [r3, #0x44]
0033de00: pop {r4, pc}

# 0x33e404 _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
0033e404: ldr ip, [pc, #0x98]
0033e408: push {r4, r5, r6, r7, r8, sl, lr}
0033e40c: ldr lr, [pc, #0x94]
0033e410: add ip, pc, ip
0033e414: sub sp, sp, #0x2c
0033e418: ldr r5, [ip, lr]
0033e41c: add r4, sp, #0xc
0033e420: mov r7, r0
0033e424: ldr lr, [r5]
0033e428: mov r6, r1
0033e42c: mov sl, r2
0033e430: ldr r1, [r3, #0x14]
0033e434: ldr r2, [r3, #0x10]
0033e438: mov r0, r4
0033e43c: str lr, [sp, #0x24]
0033e440: str r4, [sp, #0x1c]
0033e444: str r4, [sp, #0x20]
0033e448: bl #0x3116e8
0033e44c: mov r1, #0
0033e450: mov r0, #0x38
0033e454: bl #0x310570
0033e458: mov r3, sl
0033e45c: mov r8, r0
0033e460: mov r1, r7
0033e464: mov r2, r6
0033e468: str r4, [sp]
0033e46c: bl #0x33e380
0033e470: mov r2, r8
0033e474: mov r0, r7
0033e478: mov r1, r6
0033e47c: bl #0x513ce4
0033e480: mov r0, r4
0033e484: bl #0x3139ac
0033e488: ldr r2, [sp, #0x24]
0033e48c: ldr r3, [r5]
0033e490: cmp r2, r3
0033e494: bne #0x33e4a0
0033e498: add sp, sp, #0x2c
0033e49c: pop {r4, r5, r6, r7, r8, sl, pc}
0033e4a0: bl #0x30e310
0033e4a4: rsbeq r6, r5, r0, lsl #13
0033e4a8: andeq r4, r0, ip, lsr #1

# 0x33e4ac _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_
0033e4ac: push {r4, r5, r6, r7, r8, sl, lr}
0033e4b0: mov r6, r0
0033e4b4: sub sp, sp, #0xc
0033e4b8: mov r5, r1
0033e4bc: mov r0, #0x24
0033e4c0: mov r1, #0
0033e4c4: mov sl, r3
0033e4c8: mov r7, r2
0033e4cc: bl #0x310570
0033e4d0: ldr r4, [pc, #0x54]
0033e4d4: ldr r3, [pc, #0x54]
0033e4d8: mov r8, r0
0033e4dc: add r4, pc, r4
0033e4e0: ldr r3, [r4, r3]
0033e4e4: mov r1, r5
0033e4e8: add r2, sp, #4
0033e4ec: add r3, r3, #8
0033e4f0: str r3, [r0], #8
0033e4f4: bl #0x3140ec
0033e4f8: ldr r3, [pc, #0x34]
0033e4fc: rsb r7, r6, r7
0033e500: str r7, [r8, #4]
0033e504: ldr r3, [r4, r3]
0033e508: strb sl, [r8, #0x20]
0033e50c: mov r0, r6
0033e510: add r3, r3, #8
0033e514: str r3, [r8]
0033e518: mov r1, r5
0033e51c: mov r2, r8
0033e520: bl #0x513ce4
0033e524: add sp, sp, #0xc
0033e528: pop {r4, r5, r6, r7, r8, sl, pc}
0033e52c: strhteq r6, [r5], #-0x54
0033e530: andeq r2, r0, r0, lsr r3
0033e534: andeq r3, r0, ip, asr #28

# 0x33e6d4 _ZN10ObjectBase19TestEnableConditionEb
0033e6d4: push {r4, r5, r6, r7, r8, lr}
0033e6d8: ldr r4, [pc, #0xe0]
0033e6dc: ldr r6, [pc, #0xe0]
0033e6e0: mov r5, r0
0033e6e4: add r4, pc, r4
0033e6e8: ldr r3, [r4, r6]
0033e6ec: mov r7, r1
0033e6f0: mov r2, #1
0033e6f4: ldr r0, [r3, #0x40]
0033e6f8: mov r1, #0
0033e6fc: bl #0x36e478
0033e700: ldr r3, [r0, #0x660]
0033e704: cmp r3, #0
0033e708: beq #0x33e728
0033e70c: movw r2, #0x14e8
0033e710: ldr r3, [r3, r2]
0033e714: cmp r3, #0
0033e718: beq #0x33e768
0033e71c: ldrb r3, [r3, #0x14]
0033e720: cmp r3, #0
0033e724: beq #0x33e768
0033e728: ldr r0, [r4, r6]
0033e72c: bl #0x31f594
0033e730: cmp r0, #0
0033e734: beq #0x33e770
0033e738: ldr r3, [r5, #0xec]
0033e73c: ldr r2, [r0, #0x118]
0033e740: ldrb r1, [r5, #0xf1]
0033e744: cmn r3, #1
0033e748: moveq r3, #0
0033e74c: cmp r3, r2
0033e750: ble #0x33e7b8
0033e754: mov r0, r5
0033e758: mov r1, #0
0033e75c: bl #0x33ddc8
0033e760: ldrb r0, [r5, #0x8a]
0033e764: pop {r4, r5, r6, r7, r8, pc}
0033e768: ldrb r0, [r5, #0x8a]
0033e76c: pop {r4, r5, r6, r7, r8, pc}
0033e770: ldrb r1, [r5, #0xf1]
0033e774: eor r1, r1, #1
0033e778: cmp r1, #0
0033e77c: beq #0x33e754
0033e780: add r4, r5, #0x8c
0033e784: mov r0, r4
0033e788: bl #0x33e5f8
0033e78c: cmp r0, #0
0033e790: beq #0x33e754
0033e794: mov r0, r5
0033e798: mov r1, #1
0033e79c: bl #0x33ddc8
0033e7a0: cmp r7, #0
0033e7a4: beq #0x33e760
0033e7a8: mov r0, r4
0033e7ac: mov r1, #1
0033e7b0: bl #0x33dd24
0033e7b4: b #0x33e760
0033e7b8: eor r1, r1, #1
0033e7bc: b #0x33e778
0033e7c0: rsbeq r6, r5, ip, lsr #7
0033e7c4: strdeq r3, r4, [r0], -r4

# 0x33ef7c _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
0033ef7c: ldr r3, [pc, #0x80]
0033ef80: ldr ip, [pc, #0x80]
0033ef84: push {r4, r5, r6, r7, r8, lr}
0033ef88: add r3, pc, r3
0033ef8c: ldr r5, [r3, ip]
0033ef90: sub sp, sp, #0x20
0033ef94: add r4, sp, #4
0033ef98: ldr ip, [r5]
0033ef9c: mov r6, r0
0033efa0: mov r7, r1
0033efa4: mov r0, r4
0033efa8: mov r1, #0x10
0033efac: str ip, [sp, #0x1c]
0033efb0: mov r8, r2
0033efb4: str r4, [sp, #0x14]
0033efb8: str r4, [sp, #0x18]
0033efbc: bl #0x31167c
0033efc0: ldr r3, [sp, #0x14]
0033efc4: mov r2, #0
0033efc8: mov r1, r7
0033efcc: strb r2, [r3]
0033efd0: mov r0, r6
0033efd4: mov r2, r8
0033efd8: mov r3, r4
0033efdc: bl #0x33e404
0033efe0: mov r0, r4
0033efe4: bl #0x3139ac
0033efe8: ldr r2, [sp, #0x1c]
0033efec: ldr r3, [r5]
0033eff0: cmp r2, r3
0033eff4: bne #0x33f000
0033eff8: add sp, sp, #0x20
0033effc: pop {r4, r5, r6, r7, r8, pc}
0033f000: bl #0x30e310
0033f004: rsbeq r5, r5, r8, lsl #22
0033f008: andeq r4, r0, ip, lsr #1

# 0x33f00c _ZThn4_N10ObjectBase17DeclarePropertiesEv
0033f00c: sub r0, r0, #4
0033f010: b #0x33f014

# 0x33f014 _ZN10ObjectBase17DeclarePropertiesEv
0033f014: push {r4, r5, r6, lr}
0033f018: ldr r1, [pc, #0x10c]
0033f01c: mov r4, r0
0033f020: add r5, r0, #4
0033f024: mov r0, r5
0033f028: add r2, r4, #0x84
0033f02c: ldrb r3, [r4, #0x84]
0033f030: add r1, pc, r1
0033f034: bl #0x33e4ac
0033f038: ldr r1, [pc, #0xf0]
0033f03c: mov r3, #1
0033f040: mov r0, r5
0033f044: add r2, r4, #0x80
0033f048: add r1, pc, r1
0033f04c: bl #0x33e4ac
0033f050: ldr r1, [pc, #0xdc]
0033f054: mov r0, r5
0033f058: add r2, r4, #0x30
0033f05c: add r1, pc, r1
0033f060: bl #0x33ef7c
0033f064: ldr r1, [pc, #0xcc]
0033f068: mov r0, r5
0033f06c: add r2, r4, #0x48
0033f070: add r1, pc, r1
0033f074: bl #0x33ef7c
0033f078: ldr r1, [pc, #0xbc]
0033f07c: mov r0, r5
0033f080: add r2, r4, #0x68
0033f084: add r1, pc, r1
0033f088: bl #0x33ef7c
0033f08c: ldr r1, [pc, #0xac]
0033f090: mov r0, r5
0033f094: add r2, r4, #0x83
0033f098: add r1, pc, r1
0033f09c: mov r3, #0
0033f0a0: bl #0x33e4ac
0033f0a4: ldr r1, [pc, #0x98]
0033f0a8: mov r3, #0
0033f0ac: mov r0, r5
0033f0b0: add r2, r4, #0x87
0033f0b4: add r1, pc, r1
0033f0b8: bl #0x33e4ac
0033f0bc: ldr r1, [pc, #0x84]
0033f0c0: mov r0, r5
0033f0c4: add r2, r4, #0x90
0033f0c8: add r1, pc, r1
0033f0cc: bl #0x33ef7c
0033f0d0: ldr r1, [pc, #0x74]
0033f0d4: mov r0, r5
0033f0d8: add r2, r4, #0xb4
0033f0dc: add r1, pc, r1
0033f0e0: bl #0x33ef7c
0033f0e4: ldr r1, [pc, #0x64]
0033f0e8: mov r0, r5
0033f0ec: add r2, r4, #0xd4
0033f0f0: add r1, pc, r1
0033f0f4: bl #0x33ef7c
0033f0f8: ldr r1, [pc, #0x54]
0033f0fc: mov r0, r5
0033f100: add r2, r4, #0xf0
0033f104: add r1, pc, r1
0033f108: mov r3, #0
0033f10c: bl #0x33e4ac
0033f110: ldr r1, [pc, #0x40]
0033f114: mov r0, r5
0033f118: add r2, r4, #0xf1
0033f11c: add r1, pc, r1
0033f120: mov r3, #0
0033f124: pop {r4, r5, r6, lr}
0033f128: b #0x33e4ac
0033f12c: subseq r1, r8, r8, lsr r1
0033f130: subseq r1, r8, r8, lsr #2
0033f134: subseq r2, sl, ip, lsl #1
0033f138: subseq r1, r8, r8, lsl #2
0033f13c: subseq r1, r8, r4, lsl #2
0033f140: subseq r1, r8, r0, lsl #2
0033f144: ldrsheq r1, [r8], #-4
0033f148: ldrsheq r1, [r8], #-0
0033f14c: subseq r1, r8, ip, ror #1
0033f150: subseq r1, r8, r8, ror #1
0033f154: subseq r1, r8, r4, ror #1
0033f158: subseq r1, r8, r4, ror #1

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

# 0x33f50c _ZN12ObjectHandleC1Ev
0033f50c: mov r2, #0
0033f510: mvn r1, #0
0033f514: str r1, [r0, #8]
0033f518: str r2, [r0, #4]
0033f51c: str r2, [r0]
0033f520: bx lr

# 0x33f524 _ZN12ObjectHandleC1EP10ObjectBase
0033f524: push {r4, r5, lr}
0033f528: mov r3, #0
0033f52c: mvn r2, #0
0033f530: cmp r1, #0
0033f534: sub sp, sp, #0x14
0033f538: mov r4, r0
0033f53c: str r3, [r0, #4]
0033f540: str r2, [r0, #8]
0033f544: str r3, [r0]
0033f548: beq #0x33f56c
0033f54c: mov r0, sp
0033f550: bl #0x33dd2c
0033f554: ldm sp, {r0, r1, r2}
0033f558: mov r3, r4
0033f55c: str r0, [r3], #4
0033f560: mov r5, sp
0033f564: str r1, [r4, #4]
0033f568: str r2, [r3, #4]
0033f56c: mov r0, r4
0033f570: add sp, sp, #0x14
0033f574: pop {r4, r5, pc}

# 0x33f578 _ZN12ObjectHandleC2EP10ObjectBase
0033f578: push {r4, r5, lr}
0033f57c: mov r3, #0
0033f580: mvn r2, #0
0033f584: cmp r1, #0
0033f588: sub sp, sp, #0x14
0033f58c: mov r4, r0
0033f590: str r3, [r0, #4]
0033f594: str r2, [r0, #8]
0033f598: str r3, [r0]
0033f59c: beq #0x33f5c0
0033f5a0: mov r0, sp
0033f5a4: bl #0x33dd2c
0033f5a8: ldm sp, {r0, r1, r2}
0033f5ac: mov r3, r4
0033f5b0: str r0, [r3], #4
0033f5b4: mov r5, sp
0033f5b8: str r1, [r4, #4]
0033f5bc: str r2, [r3, #4]
0033f5c0: mov r0, r4
0033f5c4: add sp, sp, #0x14
0033f5c8: pop {r4, r5, pc}

# 0x33fc88 _ZNSt3mapIi14ObjectListItemSt4lessIiESaISt4pairIKiS0_EEEixIjEERS0_RKT_
0033fc88: push {r4, r5, r6, r7, r8, sb, sl, lr}
0033fc8c: ldr r5, [pc, #0x124]
0033fc90: ldr sb, [pc, #0x124]
0033fc94: ldr r4, [r0, #4]
0033fc98: add r5, pc, r5
0033fc9c: ldr r3, [r5, sb]
0033fca0: sub sp, sp, #0x48
0033fca4: cmp r4, #0
0033fca8: ldr r3, [r3]
0033fcac: mov r8, r0
0033fcb0: str r3, [sp, #0x44]
0033fcb4: beq #0x33fda8
0033fcb8: ldr r7, [r1]
0033fcbc: mov r2, r0
0033fcc0: b #0x33fccc
0033fcc4: mov r2, r4
0033fcc8: mov r4, r3
0033fccc: ldr r3, [r4, #0x10]
0033fcd0: cmp r3, r7
0033fcd4: ldrlt r3, [r4, #0xc]
0033fcd8: ldrge r3, [r4, #8]
0033fcdc: movlt r4, r2
0033fce0: cmp r3, #0
0033fce4: bne #0x33fcc4
0033fce8: cmp r8, r4
0033fcec: beq #0x33fd20
0033fcf0: ldr r3, [r4, #0x10]
0033fcf4: mov r0, r4
0033fcf8: cmp r3, r7
0033fcfc: bgt #0x33fd20
0033fd00: ldr r3, [r5, sb]
0033fd04: ldr r2, [sp, #0x44]
0033fd08: add r0, r0, #0x14
0033fd0c: ldr r3, [r3]
0033fd10: cmp r2, r3
0033fd14: bne #0x33fdb4
0033fd18: add sp, sp, #0x48
0033fd1c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0033fd20: add r6, sp, #0x28
0033fd24: mov r0, r6
0033fd28: mov r1, #0x10
0033fd2c: str r6, [sp, #0x38]
0033fd30: str r6, [sp, #0x3c]
0033fd34: bl #0x31167c
0033fd38: ldr r2, [sp, #0x38]
0033fd3c: mov r3, #0
0033fd40: add sl, sp, #0x48
0033fd44: strb r3, [r2]
0033fd48: str r7, [sl, #-0x40]!
0033fd4c: add r7, sl, #4
0033fd50: mov r0, r7
0033fd54: ldr r1, [sp, #0x3c]
0033fd58: ldr r2, [sp, #0x38]
0033fd5c: str r3, [sp, #0x40]
0033fd60: str r7, [sp, #0x1c]
0033fd64: str r7, [sp, #0x20]
0033fd68: bl #0x3116e8
0033fd6c: ldr ip, [sp, #0x40]
0033fd70: mov r1, r8
0033fd74: mov r3, sl
0033fd78: mov r2, sp
0033fd7c: add r0, sp, #4
0033fd80: str ip, [sp, #0x24]
0033fd84: str r4, [sp]
0033fd88: bl #0x33f914
0033fd8c: ldr r4, [sp, #4]
0033fd90: mov r0, r7
0033fd94: bl #0x3139ac
0033fd98: mov r0, r6
0033fd9c: bl #0x3139ac
0033fda0: mov r0, r4
0033fda4: b #0x33fd00
0033fda8: ldr r7, [r1]
0033fdac: mov r4, r0
0033fdb0: b #0x33fce8
0033fdb4: bl #0x30e310

# 0x34024c _ZN13ObjectManager6Draw2DEv
0034024c: bx lr

# 0x340250 _ZN13ObjectManager18DoOnlineStateFlushEv
00340250: ldr r3, [r0, #0x100]!
00340254: mov r1, #0
00340258: b #0x340268
0034025c: ldr r2, [r3, #8]
00340260: strb r1, [r2, #0x119]
00340264: ldr r3, [r3]
00340268: cmp r0, r3
0034026c: bne #0x34025c
00340270: bx lr

# 0x340274 _ZN13ObjectManager20DoRemoteUpdateUpdateEf
00340274: push {r4, r5, r6, r7, r8, sb, sl, lr}
00340278: mov r7, r0
0034027c: ldr r5, [r7, #0x100]!
00340280: mov r8, r1
00340284: mvn sb, #0
00340288: cmp r7, r5
0034028c: mov sl, #0
00340290: beq #0x3402f0
00340294: ldr r4, [r5, #8]
00340298: ldr r3, [r4]
0034029c: mov r0, r4
003402a0: mov lr, pc
003402a4: ldr pc, [r3, #0x54]
003402a8: mov r1, #0x42000000
003402ac: cmp r0, #0
003402b0: add r1, r1, #0x480000
003402b4: beq #0x3402e4
003402b8: ldr r6, [r4, #0x114]
003402bc: mov r0, r6
003402c0: bl #0x30e4b4
003402c4: cmp r0, #0
003402c8: mov r1, r6
003402cc: mov r0, r8
003402d0: strne sl, [r4, #0x114]
003402d4: strne sb, [r4, #0x110]
003402d8: bne #0x3402e4
003402dc: bl #0x30eba4
003402e0: str r0, [r4, #0x114]
003402e4: ldr r5, [r5]
003402e8: cmp r7, r5
003402ec: bne #0x340294
003402f0: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x3402f4 _ZN13ObjectManager24GetNetworkIdByObjectBaseEPK10ObjectBase
003402f4: cmp r1, #0
003402f8: mvneq r0, #0
003402fc: ldrne r0, [r1, #0x108]
00340300: bx lr

# 0x340304 _ZN13ObjectManager19DBG_DumpRoomObjectsEv
00340304: ldr r1, [r0, #0x80]!
00340308: cmp r1, r0
0034030c: bxeq lr
00340310: ldr r2, [r1, #8]
00340314: ldr r3, [r2]
00340318: cmp r3, r2
0034031c: beq #0x34032c
00340320: ldr r3, [r3]
00340324: cmp r2, r3
00340328: bne #0x340320
0034032c: ldr r1, [r1]
00340330: cmp r0, r1
00340334: bne #0x340310
00340338: bx lr

# 0x34064c _ZN13ObjectManager12DoCharAIInitEv
0034064c: push {r4, r5, r6, lr}
00340650: mov r6, r0
00340654: ldr r4, [r6, #0x2c]!
00340658: cmp r6, r4
0034065c: beq #0x34068c
00340660: ldr r5, [r4, #8]
00340664: subs r0, r5, #0
00340668: beq #0x340680
0034066c: ldr r3, [r5]
00340670: mov lr, pc
00340674: ldr pc, [r3, #0x24]
00340678: cmp r0, #0
0034067c: bne #0x340690
00340680: ldr r4, [r4]
00340684: cmp r6, r4
00340688: bne #0x340660
0034068c: pop {r4, r5, r6, pc}
00340690: add r5, r5, #0x3c8
00340694: mov r0, r5
00340698: bl #0x3cfd7c
0034069c: mov r0, r5
003406a0: bl #0x3cfde4
003406a4: ldr r4, [r4]
003406a8: b #0x340684

# 0x3406ac _ZN13ObjectManager12ObjectReInitEv
003406ac: push {r4, r5, r6, lr}
003406b0: mov r6, r0
003406b4: ldr r5, [r6, #0x2c]!
003406b8: mov r1, #0
003406bc: cmp r6, r5
003406c0: beq #0x34070c
003406c4: ldr r4, [r5, #8]
003406c8: cmp r4, #0
003406cc: add r0, r4, #0x8c
003406d0: beq #0x3406fc
003406d4: bl #0x33dd24
003406d8: mov r1, #0
003406dc: add r0, r4, #0xb0
003406e0: bl #0x33dd24
003406e4: ldr r3, [r4]
003406e8: mov r0, r4
003406ec: mov lr, pc
003406f0: ldr pc, [r3, #0x24]
003406f4: cmp r0, #0
003406f8: bne #0x340710
003406fc: ldr r5, [r5]
00340700: cmp r6, r5
00340704: mov r1, #0
00340708: bne #0x3406c4
0034070c: pop {r4, r5, r6, pc}
00340710: add r4, r4, #0x3c8
00340714: mov r0, r4
00340718: bl #0x3cfd7c
0034071c: mov r0, r4
00340720: bl #0x3cfde4
00340724: ldr r5, [r5]
00340728: b #0x340700

# 0x34072c _ZN13ObjectManager24DeleteRandomOnlineObjectEv
0034072c: push {r4, r5, r6, lr}
00340730: mov r3, r0
00340734: ldr r4, [r3, #0x100]!
00340738: mov r5, r0
0034073c: cmp r4, r3
00340740: mvneq r0, #3
00340744: beq #0x340760
00340748: mov r0, #0
0034074c: ldr r4, [r4]
00340750: add r0, r0, #1
00340754: cmp r3, r4
00340758: bne #0x34074c
0034075c: sub r0, r0, #4
00340760: mov r1, #0
00340764: bl #0x33ff90
00340768: ldr r3, [r5, #0x100]
0034076c: add r1, r0, #4
00340770: mov r2, #0
00340774: b #0x34078c
00340778: cmp r2, r1
0034077c: ldr r0, [r3, #8]
00340780: beq #0x340798
00340784: ldr r3, [r3]
00340788: add r2, r2, #1
0034078c: cmp r4, r3
00340790: bne #0x340778
00340794: pop {r4, r5, r6, pc}
00340798: pop {r4, r5, r6, lr}
0034079c: b #0x33ddb4

# 0x3407a0 _ZN13ObjectManager26GetObjectHandleByNetworkIdEi
003407a0: push {r4, lr}
003407a4: ldr r3, [r1, #0x100]!
003407a8: mov r4, r0
003407ac: cmp r1, r3
003407b0: beq #0x3407d8
003407b4: ldr r0, [r3, #8]
003407b8: cmp r0, #0
003407bc: beq #0x3407cc
003407c0: ldr ip, [r0, #0x108]
003407c4: cmp r2, ip
003407c8: beq #0x3407ec
003407cc: ldr r3, [r3]
003407d0: cmp r1, r3
003407d4: bne #0x3407b4
003407d8: mov r0, r4
003407dc: mov r1, #0
003407e0: bl #0x33f524
003407e4: mov r0, r4
003407e8: pop {r4, pc}
003407ec: mov r1, r0
003407f0: mov r0, r4
003407f4: bl #0x33dd2c
003407f8: mov r0, r4
003407fc: pop {r4, pc}

# 0x340800 _Z14GetNewInstanceI9CharacterEP10ObjectBasev
00340800: push {r4, lr}
00340804: mov r1, #0
00340808: movw r0, #0x1f90
0034080c: bl #0x310570
00340810: mov r1, #0
00340814: mov r4, r0
00340818: bl #0x3aa1b4
0034081c: mov r0, r4
00340820: pop {r4, pc}

# 0x340824 _Z14GetNewInstanceI4DoorEP10ObjectBasev
00340824: push {r4, lr}
00340828: mov r1, #0
0034082c: mov r0, #0x6d0
00340830: bl #0x310570
00340834: mov r1, #2
00340838: mov r4, r0
0034083c: bl #0x3e8274
00340840: mov r0, r4
00340844: pop {r4, pc}

# 0x340848 _ZN13ObjectManager13CanSendUpdateEP10ObjectBase
00340848: push {r4, r5, lr}
0034084c: subs r5, r1, #0
00340850: sub sp, sp, #0x14
00340854: beq #0x3408d4
00340858: ldr r3, [r5]
0034085c: mov r0, r5
00340860: mov lr, pc
00340864: ldr pc, [r3, #0x54]
00340868: cmp r0, #0
0034086c: bne #0x3408d4
00340870: ldr r3, [r5, #0x100]
00340874: cmp r3, #0
00340878: beq #0x3408d4
0034087c: add r4, sp, #4
00340880: mov r0, r4
00340884: mov r1, r5
00340888: bl #0x33dd2c
0034088c: mov r0, r4
00340890: bl #0x33ff54
00340894: subs r4, r0, #0
00340898: beq #0x3408a8
0034089c: ldr r3, [r4, #0x11c]
003408a0: cmp r3, #0
003408a4: ble #0x3408e0
003408a8: mov r0, #1
003408ac: b #0x3408d8
003408b0: add r0, r4, #0x4f0
003408b4: add r0, r0, #0xc
003408b8: bl #0x3c01c0
003408bc: cmp r0, #0
003408c0: beq #0x34092c
003408c4: mov r0, r4
003408c8: bl #0x3a5248
003408cc: cmp r0, #0
003408d0: beq #0x34092c
003408d4: mov r0, #0
003408d8: add sp, sp, #0x14
003408dc: pop {r4, r5, pc}
003408e0: ldr r3, [r4]
003408e4: mov lr, pc
003408e8: ldr pc, [r3, #0x148]
003408ec: cmp r0, #0
003408f0: beq #0x3408d4
003408f4: mov r3, #0x1480
003408f8: ldrb r3, [r4, r3]
003408fc: cmp r3, #0
00340900: bne #0x3408d4
00340904: mov r0, r4
00340908: bl #0x3a30c4
0034090c: cmp r0, #0
00340910: bne #0x3408d4
00340914: ldr r3, [r4]
00340918: mov r0, r4
0034091c: mov lr, pc
00340920: ldr pc, [r3, #0x34]
00340924: cmp r0, #0
00340928: bne #0x3408b0
0034092c: mov r0, r4
00340930: ldr r3, [r4]
00340934: mov lr, pc
00340938: ldr pc, [r3, #0x28]
0034093c: eor r0, r0, #1
00340940: uxtb r0, r0
00340944: b #0x3408d8

# 0x340948 _ZN13ObjectManager22IsRemotePlayerOfMemberEiP9Character
00340948: push {r4, r5, r6, r7, r8, lr}
0034094c: ldr r6, [pc, #0x98]
00340950: subs r7, r2, #0
00340954: mov r4, r1
00340958: add r6, pc, r6
0034095c: beq #0x3409dc
00340960: bl #0x8100dc
00340964: mov r1, r4
00340968: bl #0x812d50
0034096c: ldr r3, [r0]
00340970: ldr r2, [r0, #4]
00340974: mov r5, r0
00340978: rsb r2, r3, r2
0034097c: lsrs r2, r2, #2
00340980: beq #0x3409dc
00340984: ldr r1, [pc, #0x64]
00340988: mov r2, #0
0034098c: mov r4, r2
00340990: ldr r6, [r6, r1]
00340994: ldr r1, [r3, r2, lsl #2]
00340998: ldr r0, [r6, #0x40]
0034099c: mov r2, #0
003409a0: bl #0x36dfb0
003409a4: ldr r3, [r0, #0x660]
003409a8: add r4, r4, #1
003409ac: mov r2, r4
003409b0: cmp r3, #0
003409b4: beq #0x3409c8
003409b8: ldr r3, [r3, #0x108]
003409bc: ldr r1, [r7, #0x108]
003409c0: cmp r1, r3
003409c4: beq #0x3409e4
003409c8: ldr r3, [r5]
003409cc: ldr r1, [r5, #4]
003409d0: rsb r1, r3, r1
003409d4: cmp r4, r1, asr #2
003409d8: blo #0x340994
003409dc: mov r0, #0
003409e0: pop {r4, r5, r6, r7, r8, pc}
003409e4: mov r0, #1
003409e8: pop {r4, r5, r6, r7, r8, pc}
003409ec: rsbeq r4, r5, r8, lsr r1
003409f0: strdeq r3, r4, [r0], -r4

# 0x3409f4 _ZN13ObjectManager20IsObjectSerializableEP10ObjectBasei
003409f4: push {r4, r5, r6, r7, r8, lr}
003409f8: ldr r4, [pc, #0x1bc]
003409fc: subs r5, r1, #0
00340a00: sub sp, sp, #0x10
00340a04: mov r7, r0
00340a08: add r4, pc, r4
00340a0c: mov r6, r2
00340a10: beq #0x340afc
00340a14: bl #0x7fd794
00340a18: bl #0x7fd5b4
00340a1c: cmp r0, #0
00340a20: beq #0x340a40
00340a24: ldr r3, [r5, #0x110]
00340a28: cmn r3, #1
00340a2c: beq #0x340a40
00340a30: cmp r6, r3
00340a34: beq #0x340a40
00340a38: mov r0, #1
00340a3c: b #0x340b00
00340a40: ldrb r3, [r5, #0x119]
00340a44: cmp r3, #0
00340a48: beq #0x340afc
00340a4c: ldr r3, [r5, #0x100]
00340a50: cmp r3, #0
00340a54: beq #0x340afc
00340a58: ldr r3, [r5]
00340a5c: mov r0, r5
00340a60: mov lr, pc
00340a64: ldr pc, [r3, #0x54]
00340a68: cmp r0, #0
00340a6c: bne #0x340afc
00340a70: ldrb r3, [r5, #0x118]
00340a74: cmp r3, #0
00340a78: bne #0x340afc
00340a7c: ldr r3, [r5, #0x110]
00340a80: cmp r6, r3
00340a84: beq #0x340afc
00340a88: add r8, sp, #4
00340a8c: mov r0, r8
00340a90: mov r1, r5
00340a94: bl #0x33dd2c
00340a98: mov r0, r8
00340a9c: bl #0x33ff54
00340aa0: subs r8, r0, #0
00340aa4: beq #0x340a38
00340aa8: ldr r3, [r8, #0x11c]
00340aac: cmp r3, #0
00340ab0: ble #0x340b08
00340ab4: ldr r0, [r5, #0x100]
00340ab8: bl #0x81347c
00340abc: ldr r3, [r8, #0x11c]
00340ac0: cmp r3, #0
00340ac4: subgt r3, r3, #1
00340ac8: strgt r3, [r8, #0x11c]
00340acc: movgt r0, #1
00340ad0: bgt #0x340b00
00340ad4: b #0x340a38
00340ad8: add r0, r8, #0x4f0
00340adc: add r0, r0, #0xc
00340ae0: bl #0x3c01c0
00340ae4: cmp r0, #0
00340ae8: beq #0x340b54
00340aec: mov r0, r8
00340af0: bl #0x3a5248
00340af4: cmp r0, #0
00340af8: beq #0x340b54
00340afc: mov r0, #0
00340b00: add sp, sp, #0x10
00340b04: pop {r4, r5, r6, r7, r8, pc}
00340b08: ldr r3, [r8]
00340b0c: mov lr, pc
00340b10: ldr pc, [r3, #0x148]
00340b14: cmp r0, #0
00340b18: beq #0x340afc
00340b1c: mov r3, #0x1480
00340b20: ldrb r3, [r8, r3]
00340b24: cmp r3, #0
00340b28: bne #0x340afc
00340b2c: mov r0, r8
00340b30: bl #0x3a30c4
00340b34: cmp r0, #0
00340b38: bne #0x340afc
00340b3c: ldr r3, [r8]
00340b40: mov r0, r8
00340b44: mov lr, pc
00340b48: ldr pc, [r3, #0x34]
00340b4c: cmp r0, #0
00340b50: bne #0x340ad8
00340b54: ldr r3, [r8]
00340b58: mov r0, r8
00340b5c: mov lr, pc
00340b60: ldr pc, [r3, #0x28]
00340b64: cmp r0, #0
00340b68: beq #0x340a38
00340b6c: bl #0x800f8c
00340b70: bl #0x7fe4e0
00340b74: subs r2, r0, #0
00340b78: beq #0x340b98
00340b7c: mov r0, r7
00340b80: mov r1, r6
00340b84: mov r2, r8
00340b88: bl #0x340948
00340b8c: eor r0, r0, #1
00340b90: uxtb r0, r0
00340b94: b #0x340b00
00340b98: ldr r3, [pc, #0x20]
00340b9c: mov r1, r8
00340ba0: ldr r3, [r4, r3]
00340ba4: ldr r0, [r3, #0x40]
00340ba8: bl #0x36eea8
00340bac: ldrb r3, [r0, #0x66c]
00340bb0: cmp r3, #1
00340bb4: beq #0x340b7c
00340bb8: b #0x340afc
00340bbc: rsbeq r4, r5, r8, lsl #1
00340bc0: strdeq r3, r4, [r0], -r4

# 0x340bc4 _ZN13ObjectManager18NetworkUnInitLevelEv
00340bc4: push {r4, lr}
00340bc8: mov r4, r0
00340bcc: mov r0, #3
00340bd0: bl #0x8152c0
00340bd4: mov r3, #0
00340bd8: strb r3, [r4, #0x1ac]
00340bdc: pop {r4, pc}

# 0x340be0 _ZN13ObjectManager16NetworkInitLevelEv
00340be0: push {r4, lr}
00340be4: sub sp, sp, #8
00340be8: mov r4, r0
00340bec: bl #0x7fd794
00340bf0: ldrb r3, [r0, #5]
00340bf4: ldr r0, [pc, #0x44]
00340bf8: cmp r3, #0
00340bfc: add r0, pc, r0
00340c00: beq #0x340c30
00340c04: ldr r3, [pc, #0x38]
00340c08: ldr ip, [pc, #0x38]
00340c0c: ldr r1, [r0, r3]
00340c10: ldr r3, [pc, #0x34]
00340c14: ldr ip, [r0, ip]
00340c18: ldr r2, [r0, r3]
00340c1c: ldr r3, [pc, #0x2c]
00340c20: str ip, [sp]
00340c24: ldr r3, [r0, r3]
00340c28: mov r0, #3
00340c2c: bl #0x815258
00340c30: mov r3, #1
00340c34: strb r3, [r4, #0x1ac]
00340c38: add sp, sp, #8
00340c3c: pop {r4, pc}
00340c40: mlseq r5, r4, lr, r3
00340c44: andeq r1, r0, ip, ror ip
00340c48: andeq r2, r0, r0, asr #20
00340c4c: andeq r1, r0, ip, asr r6
00340c50: andeq r0, r0, r4, asr #17

# 0x340c54 _ZN13ObjectManager14GetObjectByPtrEP10ObjectBase
00340c54: push {r4, r5, r6, lr}
00340c58: mov r6, r2
00340c5c: sub sp, sp, #0x10
00340c60: mov r4, r0
00340c64: bl #0x33f50c
00340c68: cmp r6, #0
00340c6c: beq #0x340c9c
00340c70: mov r1, r6
00340c74: mov r0, sp
00340c78: bl #0x33dd2c
00340c7c: ldr r0, [sp]
00340c80: ldr r1, [sp, #8]
00340c84: ldr r2, [sp, #4]
00340c88: mov r3, r4
00340c8c: str r0, [r3], #4
00340c90: mov r5, sp
00340c94: str r1, [r3, #4]
00340c98: str r2, [r4, #4]
00340c9c: mov r0, r4
00340ca0: add sp, sp, #0x10
00340ca4: pop {r4, r5, r6, pc}

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

# 0x340ccc _Z14GetNewInstanceI12SoundEmitterEP10ObjectBasev
00340ccc: push {r4, lr}
00340cd0: mov r1, #0
00340cd4: mov r0, #0x3a0
00340cd8: bl #0x310570
00340cdc: mov r1, #0x14
00340ce0: mov r4, r0
00340ce4: bl #0x39551c
00340ce8: mov r0, r4
00340cec: pop {r4, pc}

# 0x340cf0 _Z14GetNewInstanceI19LaserTypeProjectileEP10ObjectBasev
00340cf0: push {r4, lr}
00340cf4: mov r1, #0
00340cf8: mov r0, #0x3e4
00340cfc: bl #0x310570
00340d00: mov r1, #0xa
00340d04: mov r4, r0
00340d08: bl #0x3e4b6c
00340d0c: mov r0, r4
00340d10: pop {r4, pc}

# 0x340d14 _Z14GetNewInstanceI10ProjectileEP10ObjectBasev
00340d14: push {r4, lr}
00340d18: mov r1, #0
00340d1c: mov r0, #0x3d4
00340d20: bl #0x310570
00340d24: mov r1, #9
00340d28: mov r4, r0
00340d2c: bl #0x3e5e40
00340d30: mov r0, r4
00340d34: pop {r4, pc}

# 0x340d38 _Z14GetNewInstanceI10ItemObjectEP10ObjectBasev
00340d38: push {r4, lr}
00340d3c: mov r1, #0
00340d40: mov r0, #0x3d0
00340d44: bl #0x310570
00340d48: mov r1, #3
00340d4c: mov r4, r0
00340d50: bl #0x3ec324
00340d54: mov r0, r4
00340d58: pop {r4, pc}

# 0x340d5c _Z14GetNewInstanceI21DestructibleContainerEP10ObjectBasev
00340d5c: push {r4, lr}
00340d60: mov r1, #0
00340d64: movw r0, #0x6f8
00340d68: bl #0x310570
00340d6c: mov r1, #1
00340d70: mov r4, r0
00340d74: bl #0x3a14bc
00340d78: mov r0, r4
00340d7c: pop {r4, pc}

# 0x340d80 _Z14GetNewInstanceI13SlotContainerEP10ObjectBasev
00340d80: push {r4, lr}
00340d84: mov r1, #0
00340d88: movw r0, #0x748
00340d8c: bl #0x310570
00340d90: mov r1, #8
00340d94: mov r4, r0
00340d98: bl #0x3a2bcc
00340d9c: mov r0, r4
00340da0: pop {r4, pc}

# 0x340da4 _Z14GetNewInstanceI17OpenableContainerEP10ObjectBasev
00340da4: push {r4, lr}
00340da8: mov r1, #0
00340dac: movw r0, #0x718
00340db0: bl #0x310570
00340db4: mov r1, #7
00340db8: mov r4, r0
00340dbc: bl #0x3a1cb0
00340dc0: mov r0, r4
00340dc4: pop {r4, pc}

# 0x340dc8 _Z14GetNewInstanceI14LiftableObjectEP10ObjectBasev
00340dc8: push {r4, lr}
00340dcc: mov r1, #0
00340dd0: mov r0, #0x3a8
00340dd4: bl #0x310570
00340dd8: mov r1, #6
00340ddc: mov r4, r0
00340de0: bl #0x3eec88
00340de4: mov r0, r4
00340de8: pop {r4, pc}

# 0x340dec _Z14GetNewInstanceI9SpawnSpotEP10ObjectBasev
00340dec: push {r4, lr}
00340df0: mov r1, #0
00340df4: mov r0, #0x38c
00340df8: bl #0x310570
00340dfc: mov r1, #0x14
00340e00: mov r4, r0
00340e04: bl #0x3ea8e4
00340e08: mov r0, r4
00340e0c: pop {r4, pc}

# 0x340e10 _Z14GetNewInstanceI10SpawnPointEP10ObjectBasev
00340e10: push {r4, lr}
00340e14: mov r1, #0
00340e18: mov r0, #0x394
00340e1c: bl #0x310570
00340e20: mov r1, #0xd
00340e24: mov r4, r0
00340e28: bl #0x3ea388
00340e2c: mov r0, r4
00340e30: pop {r4, pc}

# 0x340e34 _Z14GetNewInstanceI14ProjectileTrapEP10ObjectBasev
00340e34: push {r4, lr}
00340e38: mov r1, #0
00340e3c: mov r0, #0x410
00340e40: bl #0x310570
00340e44: mov r1, #0x11
00340e48: mov r4, r0
00340e4c: bl #0x39d280
00340e50: mov r0, r4
00340e54: pop {r4, pc}

# 0x340e58 _Z14GetNewInstanceI9TimerTrapEP10ObjectBasev
00340e58: push {r4, lr}
00340e5c: mov r1, #0
00340e60: movw r0, #0x408
00340e64: bl #0x310570
00340e68: mov r1, #0x10
00340e6c: mov r4, r0
00340e70: bl #0x39da54
00340e74: mov r0, r4
00340e78: pop {r4, pc}

# 0x340e7c _Z14GetNewInstanceI11TriggerTrapEP10ObjectBasev
00340e7c: push {r4, lr}
00340e80: mov r1, #0
00340e84: movw r0, #0x404
00340e88: bl #0x310570
00340e8c: mov r1, #0xf
00340e90: mov r4, r0
00340e94: bl #0x39ecd4
00340e98: mov r0, r4
00340e9c: pop {r4, pc}

# 0x340ea0 _Z14GetNewInstanceI20TriggerZoneExitLevelEP10ObjectBasev
00340ea0: push {r4, lr}
00340ea4: mov r1, #0
00340ea8: mov r0, #0x820
00340eac: bl #0x310570
00340eb0: mov r1, #0xe
00340eb4: mov r4, r0
00340eb8: bl #0x39c9a4
00340ebc: mov r0, r4
00340ec0: pop {r4, pc}

# 0x340ec4 _Z14GetNewInstanceI11TriggerZoneEP10ObjectBasev
00340ec4: push {r4, lr}
00340ec8: mov r1, #0
00340ecc: movw r0, #0x7d8
00340ed0: bl #0x310570
00340ed4: mov r1, #0x14
00340ed8: mov r4, r0
00340edc: bl #0x39b890
00340ee0: mov r0, r4
00340ee4: pop {r4, pc}

# 0x340ee8 _Z14GetNewInstanceI12TriggerPlateEP10ObjectBasev
00340ee8: push {r4, lr}
00340eec: mov r1, #0
00340ef0: mov r0, #0x780
00340ef4: bl #0x310570
00340ef8: mov r1, #0x12
00340efc: mov r4, r0
00340f00: bl #0x39af04
00340f04: mov r0, r4
00340f08: pop {r4, pc}

# 0x340f0c _Z14GetNewInstanceI13TriggerObjectEP10ObjectBasev
00340f0c: push {r4, lr}
00340f10: mov r1, #0
00340f14: mov r0, #0x790
00340f18: bl #0x310570
00340f1c: mov r4, r0
00340f20: bl #0x399df0
00340f24: mov r0, r4
00340f28: pop {r4, pc}

# 0x340f2c _Z14GetNewInstanceI14CheckpointZoneEP10ObjectBasev
00340f2c: push {r4, lr}
00340f30: mov r1, #0
00340f34: mov r0, #0x3a0
00340f38: bl #0x310570
00340f3c: mov r1, #0xc
00340f40: mov r4, r0
00340f44: bl #0x39594c
00340f48: mov r0, r4
00340f4c: pop {r4, pc}

# 0x340f50 _Z14GetNewInstanceI15QuestMoveInZoneEP10ObjectBasev
00340f50: push {r4, lr}
00340f54: mov r1, #0
00340f58: mov r0, #0x388
00340f5c: bl #0x310570
00340f60: mov r1, #0x14
00340f64: mov r4, r0
00340f68: bl #0x396330
00340f6c: mov r0, r4
00340f70: pop {r4, pc}

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

# 0x340fbc _Z14GetNewInstanceI9BillboardEP10ObjectBasev
00340fbc: push {r4, lr}
00340fc0: mov r1, #0
00340fc4: mov r0, #0x17c
00340fc8: bl #0x310570
00340fcc: mov r1, #0x14
00340fd0: mov r4, r0
00340fd4: bl #0x38815c
00340fd8: mov r0, r4
00340fdc: pop {r4, pc}

# 0x340fe0 _Z14GetNewInstanceI6ColBoxEP10ObjectBasev
00340fe0: push {r4, r5, r6, lr}
00340fe4: mov r1, #0
00340fe8: mov r0, #0x380
00340fec: bl #0x310570
00340ff0: ldr r5, [pc, #0x4c]
00340ff4: mov r1, #0x14
00340ff8: mov r4, r0
00340ffc: bl #0x38c398
00341000: ldr r3, [pc, #0x40]
00341004: add r5, pc, r5
00341008: mov r2, #0
0034100c: ldr r3, [r5, r3]
00341010: str r2, [r4, #0x37c]
00341014: str r2, [r4, #0x374]
00341018: add r1, r3, #0xe4
0034101c: add r0, r3, #8
00341020: add r3, r3, #0xd8
00341024: str r3, [r4, #4]
00341028: mov r3, #1
0034102c: str r0, [r4]
00341030: str r1, [r4, #0x24]
00341034: strb r3, [r4, #0x84]
00341038: str r2, [r4, #0x378]
0034103c: mov r0, r4
00341040: pop {r4, r5, r6, pc}
00341044: rsbeq r3, r5, ip, lsl #21
00341048: andeq r1, r0, r4, lsr #25

# 0x34104c _Z14GetNewInstanceI5FloorEP10ObjectBasev
0034104c: push {r4, r5, r6, lr}
00341050: mov r1, #0
00341054: mov r0, #0x374
00341058: bl #0x310570
0034105c: ldr r5, [pc, #0x38]
00341060: mov r1, #0x14
00341064: mov r4, r0
00341068: bl #0x38c398
0034106c: ldr r3, [pc, #0x2c]
00341070: add r5, pc, r5
00341074: mov r2, #1
00341078: ldr r3, [r5, r3]
0034107c: strb r2, [r4, #0x84]
00341080: mov r0, r4
00341084: add r2, r3, #0xe4
00341088: add r1, r3, #8
0034108c: add r3, r3, #0xd8
00341090: stm r4, {r1, r3}
00341094: str r2, [r4, #0x24]
00341098: pop {r4, r5, r6, pc}
0034109c: rsbeq r3, r5, r0, lsr #20
003410a0: andeq r0, r0, r4, ror sl

# 0x3410a4 _Z14GetNewInstanceI5DummyEP10ObjectBasev
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

# 0x34115c _Z14GetNewInstanceI10LightPointEP10ObjectBasev
0034115c: push {r4, lr}
00341160: mov r1, #0
00341164: mov r0, #0x1b4
00341168: bl #0x310570
0034116c: mov r4, r0
00341170: bl #0x40bdd8
00341174: mov r0, r4
00341178: pop {r4, pc}

# 0x34163c _ZN13ObjectManager11UpdateRoomsEv
0034163c: push {r4, r5, r6, lr}
00341640: mov r4, r0
00341644: mov r5, r0
00341648: ldr r0, [pc, #0x44]
0034164c: add r0, pc, r0
00341650: bl #0x3136b4
00341654: mov r3, #0
00341658: str r3, [r4, #0xf8]
0034165c: ldr r4, [r5, #0x24]!
00341660: b #0x34167c
00341664: ldr r3, [r4, #8]
00341668: mov r0, r3
0034166c: ldr r3, [r3]
00341670: mov lr, pc
00341674: ldr pc, [r3, #0x2c]
00341678: ldr r4, [r4]
0034167c: cmp r5, r4
00341680: bne #0x341664
00341684: ldr r0, [pc, #0xc]
00341688: add r0, pc, r0
0034168c: pop {r4, r5, r6, lr}
00341690: b #0x3136b8
00341694: subseq lr, r7, ip, lsr #24
00341698: ldrsheq lr, [r7], #-0xb0

# 0x34169c _ZN13ObjectManager22ForceEverythingVisibleEv
0034169c: push {r4, r5, r6, lr}
003416a0: mov r3, #0
003416a4: str r3, [r0, #0xf8]
003416a8: mov r5, r0
003416ac: ldr r4, [r5, #0x24]!
003416b0: b #0x3416c0
003416b4: ldr r0, [r4, #8]
003416b8: bl #0x396d24
003416bc: ldr r4, [r4]
003416c0: cmp r5, r4
003416c4: bne #0x3416b4
003416c8: pop {r4, r5, r6, pc}

# 0x3419a8 _ZN13ObjectManager23GetDynamicFogColorAtPosERK7Point3DIfEf
003419a8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003419ac: mov r5, r1
003419b0: mov r1, #0x42000000
003419b4: sub sp, sp, #0x14
003419b8: mov r4, r0
003419bc: add r1, r1, #0xc80000
003419c0: mov r0, r3
003419c4: mov r7, r2
003419c8: bl #0x30ed6c
003419cc: ldr r2, [pc, #0x1f0]
003419d0: mov r3, #0
003419d4: mov fp, r0
003419d8: str r2, [sp, #8]
003419dc: add r2, r5, #0x68
003419e0: str r2, [sp, #4]
003419e4: str r3, [r4, #8]
003419e8: str r3, [r4]
003419ec: str r3, [r4, #4]
003419f0: ldr r3, [sp, #8]
003419f4: ldr r2, [pc, #0x1cc]
003419f8: add r3, pc, r3
003419fc: str r3, [sp, #8]
00341a00: ldr r6, [r5, #0x68]
00341a04: ldr r3, [sp, #4]
00341a08: str r2, [sp, #0xc]
00341a0c: cmp r3, r6
00341a10: beq #0x341ab8
00341a14: ldr r5, [r6, #8]
00341a18: ldr r1, [r7]
00341a1c: ldr r0, [r5, #0x160]
00341a20: bl #0x30e3ac
00341a24: ldr r1, [r7, #4]
00341a28: mov sl, r0
00341a2c: ldr r0, [r5, #0x164]
00341a30: bl #0x30e3ac
00341a34: ldr r1, [r7, #8]
00341a38: mov sb, r0
00341a3c: ldr r0, [r5, #0x168]
00341a40: bl #0x30e3ac
00341a44: mov r1, sl
00341a48: mov r8, r0
00341a4c: mov r0, sl
00341a50: bl #0x30ed6c
00341a54: mov r1, sb
00341a58: mov sl, r0
00341a5c: mov r0, sb
00341a60: bl #0x30ed6c
00341a64: mov r1, r0
00341a68: mov r0, sl
00341a6c: bl #0x30eba4
00341a70: mov r1, r8
00341a74: mov sl, r0
00341a78: mov r0, r8
00341a7c: bl #0x30ed6c
00341a80: mov r1, r0
00341a84: mov r0, sl
00341a88: bl #0x30eba4
00341a8c: bl #0x30e124
00341a90: mov r8, r0
00341a94: mov r1, r8
00341a98: mov r0, fp
00341a9c: bl #0x30e4b4
00341aa0: cmp r0, #0
00341aa4: bne #0x341b30
00341aa8: ldr r6, [r6]
00341aac: ldr r3, [sp, #4]
00341ab0: cmp r3, r6
00341ab4: bne #0x341a14
00341ab8: ldr r6, [r4]
00341abc: mov r1, #0x43000000
00341ac0: add r1, r1, #0x7f0000
00341ac4: mov r0, r6
00341ac8: bl #0x30e2f8
00341acc: ldr r5, [r4, #4]
00341ad0: cmp r0, #0
00341ad4: movne r6, #0x43000000
00341ad8: addne r6, r6, #0x7f0000
00341adc: mov r1, #0x43000000
00341ae0: mov r0, r5
00341ae4: str r6, [r4]
00341ae8: add r1, r1, #0x7f0000
00341aec: bl #0x30e2f8
00341af0: ldr r6, [r4, #8]
00341af4: cmp r0, #0
00341af8: movne r5, #0x43000000
00341afc: addne r5, r5, #0x7f0000
00341b00: mov r1, #0x43000000
00341b04: mov r0, r6
00341b08: str r5, [r4, #4]
00341b0c: add r1, r1, #0x7f0000
00341b10: bl #0x30e2f8
00341b14: cmp r0, #0
00341b18: movne r6, #0x43000000
00341b1c: addne r6, r6, #0x7f0000
00341b20: str r6, [r4, #8]
00341b24: mov r0, r4
00341b28: add sp, sp, #0x14
00341b2c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00341b30: ldr r2, [sp, #8]
00341b34: ldr r3, [sp, #0xc]
00341b38: add r0, r5, #0x3f0
00341b3c: ldr r1, [r2, r3]
00341b40: bl #0x312b6c
00341b44: cmp r0, #0
00341b48: mov r1, r8
00341b4c: mov r0, fp
00341b50: bne #0x341aa8
00341b54: bl #0x30e3ac
00341b58: mov r1, fp
00341b5c: bl #0x30ec94
00341b60: ldr r1, [r5, #0x3f4]
00341b64: mov r8, r0
00341b68: bl #0x30ed6c
00341b6c: ldr r1, [r5, #0x3f8]
00341b70: mov sl, r0
00341b74: mov r0, r8
00341b78: bl #0x30ed6c
00341b7c: ldr r1, [r5, #0x3f0]
00341b80: mov sb, r0
00341b84: mov r0, r8
00341b88: bl #0x30ed6c
00341b8c: mov r1, r0
00341b90: ldr r0, [r4]
00341b94: bl #0x30eba4
00341b98: mov r1, sl
00341b9c: str r0, [r4]
00341ba0: ldr r0, [r4, #4]
00341ba4: bl #0x30eba4
00341ba8: mov r1, sb
00341bac: str r0, [r4, #4]
00341bb0: ldr r0, [r4, #8]
00341bb4: bl #0x30eba4
00341bb8: str r0, [r4, #8]
00341bbc: ldr r6, [r6]
00341bc0: b #0x341aac
00341bc4: mlseq r5, r8, r0, r3
00341bc8: muleq r0, r8, r4

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

# 0x3426b0 _ZNK13ObjectManager16GetObjectsByTypeEPKcRSt4listIP9CharacterSaIS4_EE
003426b0: push {r4, r5, r6, r7, r8, sl, lr}
003426b4: sub sp, sp, #0x14
003426b8: add r5, sp, #4
003426bc: mov r6, r0
003426c0: mov r0, r5
003426c4: mov r4, r1
003426c8: mov r7, r2
003426cc: add r8, r6, #0xc
003426d0: bl #0x33f50c
003426d4: ldr r6, [r6, #0x14]
003426d8: cmp r8, r6
003426dc: beq #0x342764
003426e0: ldr r0, [r6, #0x2c]
003426e4: cmp r0, #0
003426e8: beq #0x342738
003426ec: add r0, r0, #4
003426f0: bl #0x510b4c
003426f4: mov r1, r4
003426f8: bl #0x30e31c
003426fc: cmp r0, #0
00342700: bne #0x342738
00342704: ldr r3, [r6, #0x10]
00342708: mov r0, r5
0034270c: str r3, [sp, #4]
00342710: bl #0x33ff54
00342714: mov sl, r0
00342718: mov r0, r7
0034271c: bl #0x342690
00342720: str sl, [r0, #8]
00342724: ldr r3, [r7, #4]
00342728: str r7, [r0]
0034272c: str r3, [r0, #4]
00342730: str r0, [r3]
00342734: str r0, [r7, #4]
00342738: ldr r2, [r6, #0xc]
0034273c: cmp r2, #0
00342740: bne #0x34274c
00342744: b #0x34276c
00342748: mov r2, r3
0034274c: ldr r3, [r2, #8]
00342750: cmp r3, #0
00342754: bne #0x342748
00342758: mov r6, r2
0034275c: cmp r8, r6
00342760: bne #0x3426e0
00342764: add sp, sp, #0x14
00342768: pop {r4, r5, r6, r7, r8, sl, pc}
0034276c: ldr r3, [r6, #4]
00342770: ldr r1, [r3, #0xc]
00342774: cmp r6, r1
00342778: bne #0x342794
0034277c: mov r6, r3
00342780: ldr r3, [r3, #4]
00342784: ldr r2, [r3, #0xc]
00342788: cmp r2, r6
0034278c: beq #0x34277c
00342790: ldr r2, [r6, #0xc]
00342794: cmp r3, r2
00342798: movne r6, r3
0034279c: b #0x3426d8

# 0x3427a0 _ZN13ObjectManager14AddRoomObjectsEPSt4listIP10GameObjectSaIS2_EE
003427a0: push {r4, r5, r6, r7, lr}
003427a4: ldr r6, [pc, #0x120]
003427a8: subs r5, r1, #0
003427ac: sub sp, sp, #0x14
003427b0: mov r7, r0
003427b4: add r6, pc, r6
003427b8: beq #0x342844
003427bc: mov r4, r7
003427c0: ldr r3, [r4, #0x80]!
003427c4: cmp r3, r4
003427c8: beq #0x342814
003427cc: ldr r2, [r3, #8]
003427d0: cmp r5, r2
003427d4: beq #0x3427e8
003427d8: ldr r3, [r3]
003427dc: cmp r4, r3
003427e0: bne #0x3427cc
003427e4: mov r3, r4
003427e8: cmp r3, r4
003427ec: beq #0x342814
003427f0: ldr r3, [pc, #0xd8]
003427f4: ldr r3, [r6, r3]
003427f8: ldr r3, [r3]
003427fc: cmp r3, #2
00342800: moveq r3, #0
00342804: streq r3, [r3]
00342808: beq #0x342814
0034280c: cmp r3, #1
00342810: beq #0x342898
00342814: mov r3, #0xc
00342818: add r0, sp, #0x10
0034281c: str r3, [r0, #-4]!
00342820: bl #0x708ec0
00342824: str r5, [r0, #8]
00342828: ldr r3, [r7, #0x84]
0034282c: str r4, [r0]
00342830: str r3, [r0, #4]
00342834: str r0, [r3]
00342838: str r0, [r7, #0x84]
0034283c: add sp, sp, #0x14
00342840: pop {r4, r5, r6, r7, pc}
00342844: ldr r3, [pc, #0x84]
00342848: ldr r3, [r6, r3]
0034284c: ldr r3, [r3]
00342850: cmp r3, #2
00342854: streq r5, [r5]
00342858: beq #0x3427bc
0034285c: cmp r3, #1
00342860: bne #0x3427bc
00342864: ldr r0, [pc, #0x68]
00342868: ldr r1, [pc, #0x68]
0034286c: ldr r2, [pc, #0x68]
00342870: ldr r0, [r6, r0]
00342874: ldr r3, [pc, #0x64]
00342878: movw ip, #0x909
0034287c: add r1, pc, r1
00342880: add r2, pc, r2
00342884: add r3, pc, r3
00342888: add r0, r0, #0xa8
0034288c: str ip, [sp]
00342890: bl #0x30e004
00342894: b #0x3427bc
00342898: ldr r0, [pc, #0x34]
0034289c: ldr r1, [pc, #0x40]
003428a0: ldr r2, [pc, #0x40]
003428a4: ldr r0, [r6, r0]
003428a8: ldr r3, [pc, #0x3c]
003428ac: movw ip, #0x90a
003428b0: add r1, pc, r1
003428b4: add r2, pc, r2
003428b8: add r3, pc, r3
003428bc: add r0, r0, #0xa8
003428c0: str ip, [sp]
003428c4: bl #0x30e004
003428c8: b #0x342814

# 0x342f30 _ZN13ObjectManager13IsPacketValidEii
00342f30: push {r4, r5, r6, r7, lr}
00342f34: ldr ip, [r0, #0x14c]
00342f38: sub sp, sp, #0x34
00342f3c: mov r6, r0
00342f40: cmp ip, #0
00342f44: mov r4, r1
00342f48: mov r7, r2
00342f4c: add r5, r0, #0x148
00342f50: beq #0x343030
00342f54: mov r1, r5
00342f58: mov r3, ip
00342f5c: b #0x342f64
00342f60: mov r3, r2
00342f64: ldr r2, [r3, #0x10]
00342f68: cmp r2, r4
00342f6c: ldrlt r2, [r3, #0xc]
00342f70: ldrge r2, [r3, #8]
00342f74: movlt r3, r1
00342f78: mov r1, r3
00342f7c: cmp r2, #0
00342f80: bne #0x342f60
00342f84: cmp r5, r3
00342f88: beq #0x3430bc
00342f8c: ldr r2, [r3, #0x10]
00342f90: cmp r2, r4
00342f94: bgt #0x343030
00342f98: cmp r5, r3
00342f9c: beq #0x3430bc
00342fa0: cmp ip, #0
00342fa4: movne r2, r5
00342fa8: bne #0x342fb4
00342fac: b #0x343140
00342fb0: mov ip, r3
00342fb4: ldr r3, [ip, #0x10]
00342fb8: cmp r3, r4
00342fbc: ldrlt r3, [ip, #0xc]
00342fc0: ldrge r3, [ip, #8]
00342fc4: movlt ip, r2
00342fc8: mov r2, ip
00342fcc: cmp r3, #0
00342fd0: bne #0x342fb0
00342fd4: cmp r5, ip
00342fd8: beq #0x342fec
00342fdc: ldr r2, [ip, #0x10]
00342fe0: mov r3, ip
00342fe4: cmp r2, r4
00342fe8: ble #0x343014
00342fec: add r3, sp, #8
00342ff0: mov lr, #0
00342ff4: add r0, sp, #0x20
00342ff8: mov r1, r5
00342ffc: add r2, sp, #0x24
00343000: str lr, [sp, #0xc]
00343004: str ip, [sp, #0x24]
00343008: str r4, [sp, #8]
0034300c: bl #0x342bbc
00343010: ldr r3, [sp, #0x20]
00343014: ldr r1, [r3, #0x14]
00343018: mov r0, r7
0034301c: bl #0x8153a8
00343020: cmp r0, #0
00343024: bne #0x343038
00343028: add sp, sp, #0x34
0034302c: pop {r4, r5, r6, r7, pc}
00343030: mov r3, r5
00343034: b #0x342f98
00343038: ldr ip, [r6, #0x14c]
0034303c: cmp ip, #0
00343040: moveq ip, r5
00343044: beq #0x343074
00343048: mov r2, r5
0034304c: b #0x343054
00343050: mov ip, r3
00343054: ldr r3, [ip, #0x10]
00343058: cmp r3, r4
0034305c: ldrlt r3, [ip, #0xc]
00343060: ldrge r3, [ip, #8]
00343064: movlt ip, r2
00343068: mov r2, ip
0034306c: cmp r3, #0
00343070: bne #0x343050
00343074: cmp r5, ip
00343078: beq #0x34308c
0034307c: ldr r2, [ip, #0x10]
00343080: mov r3, ip
00343084: cmp r2, r4
00343088: ble #0x3430b0
0034308c: mov r3, sp
00343090: mov lr, #0
00343094: mov r1, r5
00343098: add r0, sp, #0x18
0034309c: add r2, sp, #0x1c
003430a0: stm sp, {r4, lr}
003430a4: str ip, [sp, #0x1c]
003430a8: bl #0x342bbc
003430ac: ldr r3, [sp, #0x18]
003430b0: str r7, [r3, #0x14]
003430b4: mov r0, #1
003430b8: b #0x343028
003430bc: cmp ip, #0
003430c0: moveq ip, r5
003430c4: beq #0x3430f4
003430c8: mov r2, r5
003430cc: b #0x3430d4
003430d0: mov ip, r3
003430d4: ldr r3, [ip, #0x10]
003430d8: cmp r3, r4
003430dc: ldrlt r3, [ip, #0xc]
003430e0: ldrge r3, [ip, #8]
003430e4: movlt ip, r2
003430e8: mov r2, ip
003430ec: cmp r3, #0
003430f0: bne #0x3430d0
003430f4: cmp r5, ip
003430f8: beq #0x34310c
003430fc: ldr r2, [ip, #0x10]
00343100: mov r3, ip
00343104: cmp r2, r4
00343108: ble #0x3430b0
0034310c: add r0, sp, #0x28
00343110: add r3, sp, #0x10
00343114: mov lr, #0
00343118: mov r1, r5
0034311c: add r2, sp, #0x2c
00343120: str r4, [sp, #0x10]
00343124: str lr, [sp, #0x14]
00343128: str ip, [sp, #0x2c]
0034312c: bl #0x342bbc
00343130: ldr r3, [sp, #0x28]
00343134: mov r0, #1
00343138: str r7, [r3, #0x14]
0034313c: b #0x343028
00343140: mov ip, r5
00343144: b #0x342fd4

# 0x343188 _ZN13ObjectManager29AddOrphanRenderObjectToDeleteEP10ObjectBase
00343188: push {r4, r5, r6, lr}
0034318c: subs r6, r1, #0
00343190: mov r4, r0
00343194: beq #0x3431bc
00343198: add r5, r0, #4
0034319c: mov r0, r5
003431a0: bl #0x343168
003431a4: str r6, [r0, #8]
003431a8: ldr r3, [r4, #8]
003431ac: str r5, [r0]
003431b0: str r3, [r0, #4]
003431b4: str r0, [r3]
003431b8: str r0, [r4, #8]
003431bc: pop {r4, r5, r6, pc}

# 0x3431c0 _ZN13ObjectManager21AssignObjectNetworkIdEP10ObjectBase
003431c0: push {r4, r5, r6, r7, r8, lr}
003431c4: mov r5, r0
003431c8: mov r4, r1
003431cc: bl #0x7fd794
003431d0: ldrb r3, [r0, #5]
003431d4: ldr r6, [pc, #0x110]
003431d8: cmp r3, #0
003431dc: add r6, pc, r6
003431e0: beq #0x343274
003431e4: ldr r7, [r4, #0x44]
003431e8: ldr r1, [pc, #0x100]
003431ec: mov r0, r7
003431f0: add r1, pc, r1
003431f4: bl #0x30ebd4
003431f8: cmp r0, r7
003431fc: beq #0x3432d0
00343200: ldr r3, [r4]
00343204: mov r0, r4
00343208: mov lr, pc
0034320c: ldr pc, [r3, #0x24]
00343210: cmp r0, #0
00343214: bne #0x343278
00343218: ldr r0, [r5, #0x13c]
0034321c: add r3, r0, #1
00343220: str r3, [r5, #0x13c]
00343224: add r0, r0, #5
00343228: cmp r0, #4
0034322c: str r0, [r4, #0x108]
00343230: bgt #0x3432b4
00343234: add r6, r5, #0x100
00343238: mov r0, r6
0034323c: bl #0x343168
00343240: str r4, [r0, #8]
00343244: ldr r3, [r5, #0x104]
00343248: str r6, [r0]
0034324c: str r3, [r0, #4]
00343250: str r0, [r3]
00343254: str r0, [r5, #0x104]
00343258: ldr r3, [r4, #0x100]
0034325c: cmp r3, #0
00343260: beq #0x343274
00343264: ldr r3, [r5, #0x54]
00343268: add r3, r3, #1
0034326c: str r3, [r5, #0x54]
00343270: pop {r4, r5, r6, r7, r8, pc}
00343274: pop {r4, r5, r6, r7, r8, pc}
00343278: movw r3, #0x14e4
0034327c: ldrb r3, [r4, r3]
00343280: cmp r3, #0
00343284: beq #0x3432a0
00343288: ldr r0, [r5, #0x140]
0034328c: add r3, r0, #1
00343290: add r0, r0, #0x2700
00343294: str r3, [r5, #0x140]
00343298: add r0, r0, #0x11
0034329c: b #0x343228
003432a0: mov r0, r4
003432a4: bl #0x3a30ac
003432a8: cmp r0, #0
003432ac: beq #0x343218
003432b0: b #0x343288
003432b4: ldr r3, [pc, #0x38]
003432b8: mov r1, #1
003432bc: ldr r3, [r6, r3]
003432c0: ldr r3, [r3]
003432c4: str r3, [r4, #0xfc]
003432c8: bl #0x33ff90
003432cc: b #0x343234
003432d0: add r0, r0, #0x10
003432d4: bl #0x30e094
003432d8: ldr r3, [r5, #0x138]
003432dc: add r0, r0, #1
003432e0: add r3, r3, #1
003432e4: str r3, [r5, #0x138]
003432e8: b #0x343228
003432ec: strhteq r1, [r5], #-0x84
003432f0: subseq sp, r7, r8, asr r1
003432f4: andeq r0, r0, r0, lsl fp

# 0x3432f8 _ZN13ObjectManager15MarkForDeletionEP10ObjectBase
003432f8: push {r4, r5, lr}
003432fc: mov r4, r0
00343300: ldr r3, [r4, #0x3c]!
00343304: sub sp, sp, #0xc
00343308: mov r5, r0
0034330c: cmp r3, r4
00343310: beq #0x343330
00343314: ldr r2, [r3, #8]
00343318: cmp r2, r1
0034331c: beq #0x343330
00343320: ldr r3, [r3]
00343324: cmp r4, r3
00343328: bne #0x343314
0034332c: mov r3, r4
00343330: cmp r4, r3
00343334: beq #0x343340
00343338: add sp, sp, #0xc
0034333c: pop {r4, r5, pc}
00343340: mov r0, r4
00343344: str r1, [sp, #4]
00343348: bl #0x343168
0034334c: ldr r1, [sp, #4]
00343350: str r1, [r0, #8]
00343354: ldr r3, [r5, #0x40]
00343358: str r4, [r0]
0034335c: str r3, [r0, #4]
00343360: str r0, [r3]
00343364: str r0, [r5, #0x40]
00343368: b #0x343338

# 0x34336c _ZNK13ObjectManager16GetLightBaseListERSt4listIP9LightBaseSaIS2_EE
0034336c: push {r4, r5, r6, r7, r8, sb, sl, lr}
00343370: ldr r7, [pc, #0xd4]
00343374: ldr r4, [r0, #0x14]
00343378: sub sp, sp, #8
0034337c: mov r6, r1
00343380: add r5, r0, #0xc
00343384: add r7, pc, r7
00343388: mov sl, #0xc
0034338c: add r8, sp, #4
00343390: cmp r5, r4
00343394: beq #0x343410
00343398: ldr r0, [r4, #0x2c]
0034339c: cmp r0, #0
003433a0: beq #0x3433e4
003433a4: add r0, r0, #4
003433a8: bl #0x510b4c
003433ac: mov r1, r7
003433b0: bl #0x30e31c
003433b4: cmp r0, #0
003433b8: bne #0x3433e4
003433bc: mov r0, r8
003433c0: ldr sb, [r4, #0x2c]
003433c4: str sl, [sp, #4]
003433c8: bl #0x708ec0
003433cc: str sb, [r0, #8]
003433d0: ldr r3, [r6, #4]
003433d4: str r6, [r0]
003433d8: str r3, [r0, #4]
003433dc: str r0, [r3]
003433e0: str r0, [r6, #4]
003433e4: ldr r2, [r4, #0xc]
003433e8: cmp r2, #0
003433ec: bne #0x3433f8
003433f0: b #0x343418
003433f4: mov r2, r3
003433f8: ldr r3, [r2, #8]
003433fc: cmp r3, #0
00343400: bne #0x3433f4
00343404: mov r4, r2
00343408: cmp r5, r4
0034340c: bne #0x343398
00343410: add sp, sp, #8
00343414: pop {r4, r5, r6, r7, r8, sb, sl, pc}
00343418: ldr r3, [r4, #4]
0034341c: ldr r1, [r3, #0xc]
00343420: cmp r4, r1
00343424: bne #0x343440
00343428: mov r4, r3
0034342c: ldr r3, [r3, #4]
00343430: ldr r2, [r3, #0xc]
00343434: cmp r2, r4
00343438: beq #0x343428
0034343c: ldr r2, [r4, #0xc]
00343440: cmp r2, r3
00343444: movne r4, r3
00343448: b #0x343390
0034344c: ldrsbeq ip, [r7], #-0xfc

# 0x344184 _ZN13ObjectManager15AddNoRoomObjectEP10GameObject
00344184: push {r4, r5, lr}
00344188: ldrb r3, [r1, #0x2f8]
0034418c: sub sp, sp, #0xc
00344190: mov r4, r1
00344194: cmp r3, #0
00344198: mov r5, r0
0034419c: bne #0x3441d0
003441a0: mov r3, #0xc
003441a4: add r0, sp, #8
003441a8: str r3, [r0, #-4]!
003441ac: bl #0x708ec0
003441b0: str r4, [r0, #8]
003441b4: ldr r3, [r5, #0x8c]
003441b8: add r2, r5, #0x88
003441bc: stm r0, {r2, r3}
003441c0: str r0, [r3]
003441c4: mov r3, #1
003441c8: str r0, [r5, #0x8c]
003441cc: strb r3, [r4, #0x2f8]
003441d0: add sp, sp, #0xc
003441d4: pop {r4, r5, pc}

# 0x3454dc _ZN13ObjectManager27FlushAllOrphanRenderObjectsEv
003454dc: push {r4, r5, r6, lr}
003454e0: mov r5, r0
003454e4: ldr r4, [r5, #4]!
003454e8: mov r6, #0
003454ec: cmp r5, r4
003454f0: beq #0x345520
003454f4: ldr r3, [r4, #8]
003454f8: cmp r3, #0
003454fc: beq #0x345514
00345500: mov r0, r3
00345504: ldr r3, [r3]
00345508: mov lr, pc
0034550c: ldr pc, [r3, #4]
00345510: str r6, [r4, #8]
00345514: ldr r4, [r4]
00345518: cmp r5, r4
0034551c: bne #0x3454f4
00345520: mov r0, r5
00345524: pop {r4, r5, r6, lr}
00345528: b #0x34526c

# 0x34552c _ZN13ObjectManager8InitPostEv
0034552c: push {r4, r5, r6, r7, lr}
00345530: ldr r5, [pc, #0x374]
00345534: sub sp, sp, #0x24
00345538: mov r4, r0
0034553c: add r5, pc, r5
00345540: ldr r6, [r5, #0x1c]
00345544: ands r6, r6, #1
00345548: beq #0x345620
0034554c: ldr r5, [pc, #0x35c]
00345550: add r5, pc, r5
00345554: ldr r6, [r5, #0x24]
00345558: ands r6, r6, #1
0034555c: beq #0x345644
00345560: add r6, sp, #0x10
00345564: mov r0, r6
00345568: bl #0x33f50c
0034556c: ldr r2, [r4, #0x7c]
00345570: cmp r2, #0
00345574: bne #0x34559c
00345578: ldr r3, [pc, #0x334]
0034557c: ldr r2, [r4, #0x68]
00345580: add r3, pc, r3
00345584: str r2, [r3, #0x28]
00345588: ldr r2, [r4, #0x14]
0034558c: str r2, [r3, #0x20]
00345590: ldr r2, [r4, #0x7c]
00345594: add r2, r2, #1
00345598: str r2, [r4, #0x7c]
0034559c: ldr r5, [pc, #0x314]
003455a0: add r1, r4, #0x68
003455a4: add r5, pc, r5
003455a8: ldr r3, [r5, #0x28]
003455ac: cmp r1, r3
003455b0: beq #0x345744
003455b4: cmp r2, #1
003455b8: beq #0x345880
003455bc: ldr r5, [pc, #0x2f8]
003455c0: add r1, r4, #0xc
003455c4: add r5, pc, r5
003455c8: ldr r3, [r5, #0x20]
003455cc: cmp r1, r3
003455d0: beq #0x345708
003455d4: cmp r2, #3
003455d8: beq #0x3457a0
003455dc: cmp r2, #4
003455e0: beq #0x345668
003455e4: ldr r2, [r3, #0xc]
003455e8: cmp r2, #0
003455ec: bne #0x3455f8
003455f0: b #0x34576c
003455f4: mov r2, r3
003455f8: ldr r3, [r2, #8]
003455fc: cmp r3, #0
00345600: bne #0x3455f4
00345604: mov r3, r2
00345608: ldr r2, [pc, #0x2b0]
0034560c: mov r0, #0
00345610: add r2, pc, r2
00345614: str r3, [r2, #0x20]
00345618: add sp, sp, #0x24
0034561c: pop {r4, r5, r6, r7, pc}
00345620: add r7, r5, #0x1c
00345624: mov r0, r7
00345628: bl #0x30e76c
0034562c: cmp r0, #0
00345630: beq #0x34554c
00345634: str r6, [r5, #0x20]
00345638: mov r0, r7
0034563c: bl #0x30ea3c
00345640: b #0x34554c
00345644: add r7, r5, #0x24
00345648: mov r0, r7
0034564c: bl #0x30e76c
00345650: cmp r0, #0
00345654: beq #0x345560
00345658: str r6, [r5, #0x28]
0034565c: mov r0, r7
00345660: bl #0x30ea3c
00345664: b #0x345560
00345668: ldr r5, [r3, #0x2c]
0034566c: cmp r5, #0
00345670: beq #0x3455e4
00345674: ldr r0, [pc, #0x248]
00345678: ldr r1, [r5, #0x5c]
0034567c: add r0, pc, r0
00345680: bl #0x30e6e8
00345684: cmp r0, #0
00345688: bne #0x345840
0034568c: mov r3, #0xc
00345690: add r0, sp, #0x20
00345694: str r3, [r0, #-4]!
00345698: bl #0x708ec0
0034569c: str r5, [r0, #8]
003456a0: ldr r3, [r4, #0x28]
003456a4: add r2, r4, #0x24
003456a8: stm r0, {r2, r3}
003456ac: str r0, [r3]
003456b0: str r0, [r4, #0x28]
003456b4: mov r0, r5
003456b8: bl #0x396c44
003456bc: ldrb r3, [r5, #0xac]
003456c0: cmp r3, #0
003456c4: bne #0x345818
003456c8: ldr r3, [r5, #0xa8]
003456cc: cmp r3, #0
003456d0: beq #0x345818
003456d4: add r6, r4, #0x44
003456d8: mov r0, r6
003456dc: bl #0x343168
003456e0: str r5, [r0, #8]
003456e4: ldr r2, [r4, #0x48]
003456e8: ldr r3, [pc, #0x1d8]
003456ec: str r6, [r0]
003456f0: str r2, [r0, #4]
003456f4: add r3, pc, r3
003456f8: str r0, [r2]
003456fc: str r0, [r4, #0x48]
00345700: ldr r3, [r3, #0x20]
00345704: b #0x3455e4
00345708: add r2, r2, #1
0034570c: cmp r2, #4
00345710: str r2, [r4, #0x7c]
00345714: movne r0, #1
00345718: bne #0x345618
0034571c: ldr r3, [r4, #0x14]
00345720: add r0, r4, #0x2c
00345724: str r3, [r5, #0x20]
00345728: bl #0x34526c
0034572c: add r0, r4, #0x44
00345730: bl #0x34526c
00345734: add r0, r4, #0x34
00345738: bl #0x34526c
0034573c: mov r0, #0
00345740: b #0x345618
00345744: cmp r2, #1
00345748: bne #0x3455bc
0034574c: ldr r3, [r4, #0x14]
00345750: mov r2, #2
00345754: str r2, [r4, #0x7c]
00345758: str r3, [r5, #0x20]
0034575c: ldr r2, [r4, #0x7c]
00345760: add r2, r2, #1
00345764: str r2, [r4, #0x7c]
00345768: b #0x3455bc
0034576c: ldr r1, [r3, #4]
00345770: ldr r0, [r1, #0xc]
00345774: cmp r0, r3
00345778: bne #0x345794
0034577c: mov r3, r1
00345780: ldr r1, [r1, #4]
00345784: ldr r2, [r1, #0xc]
00345788: cmp r3, r2
0034578c: beq #0x34577c
00345790: ldr r2, [r3, #0xc]
00345794: cmp r1, r2
00345798: movne r3, r1
0034579c: b #0x345608
003457a0: add r4, sp, #4
003457a4: ldr r1, [r3, #0x2c]
003457a8: mov r0, r4
003457ac: bl #0x33f524
003457b0: ldr r2, [sp, #8]
003457b4: add r6, r6, #4
003457b8: ldr r3, [sp, #4]
003457bc: str r2, [r6], #4
003457c0: ldr r2, [r4, #8]
003457c4: add r4, sp, #0x10
003457c8: mov r0, r4
003457cc: mov r1, #0
003457d0: str r2, [r6]
003457d4: str r3, [sp, #0x10]
003457d8: bl #0x33fdc0
003457dc: cmp r0, #0
003457e0: beq #0x345810
003457e4: mov r1, #1
003457e8: mov r0, r4
003457ec: bl #0x33fdc0
003457f0: ldr r3, [r0]
003457f4: mov lr, pc
003457f8: ldr pc, [r3, #0x1c]
003457fc: mov r1, #1
00345800: mov r0, r4
00345804: bl #0x33fdc0
00345808: mov r1, #0
0034580c: bl #0x33e6d4
00345810: ldr r3, [r5, #0x20]
00345814: b #0x3455e4
00345818: ldrb r3, [r5, #0xd0]
0034581c: cmp r3, #0
00345820: bne #0x34589c
00345824: ldr r3, [r5, #0xcc]
00345828: cmp r3, #0
0034582c: bne #0x3456d4
00345830: ldr r3, [pc, #0x94]
00345834: add r3, pc, r3
00345838: ldr r3, [r3, #0x20]
0034583c: b #0x3455e4
00345840: ldr r3, [r5]
00345844: mov r0, r5
00345848: mov lr, pc
0034584c: ldr pc, [r3, #0x38]
00345850: cmp r0, #0
00345854: beq #0x3456bc
00345858: add r6, r4, #0x2c
0034585c: mov r0, r6
00345860: bl #0x343168
00345864: str r5, [r0, #8]
00345868: ldr r3, [r4, #0x30]
0034586c: str r6, [r0]
00345870: str r3, [r0, #4]
00345874: str r0, [r3]
00345878: str r0, [r4, #0x30]
0034587c: b #0x3456bc
00345880: ldr r0, [r3, #8]
00345884: bl #0x38a88c
00345888: ldr r3, [r5, #0x28]
0034588c: ldr r3, [r3]
00345890: str r3, [r5, #0x28]
00345894: ldr r2, [r4, #0x7c]
00345898: b #0x3455bc
0034589c: ldr r3, [pc, #0x2c]
003458a0: add r3, pc, r3
003458a4: ldr r3, [r3, #0x20]
003458a8: b #0x3455e4
003458ac: rsbeq ip, r5, r0, lsl #18
003458b0: rsbeq ip, r5, ip, ror #17
003458b4: strhteq ip, [r5], #-0x8c
003458b8: mlseq r5, r8, r8, ip
003458bc: rsbeq ip, r5, r8, ror r8
003458c0: rsbeq ip, r5, ip, lsr #16
003458c4: ldrsheq sl, [r7], #-0xc4
003458c8: rsbeq ip, r5, r8, asr #14
003458cc: rsbeq ip, r5, r8, lsl #12
003458d0: mlseq r5, ip, r5, ip

# 0x345954 _ZN13ObjectManager19HandleNoRoomObjectsEv
00345954: push {r4, r5, r6, r7, r8, lr}
00345958: add r5, r0, #0x88
0034595c: mov r7, r0
00345960: mov r0, r5
00345964: bl #0x345914
00345968: ldr r4, [r7, #0x14]
0034596c: add r6, r7, #0xc
00345970: cmp r6, r4
00345974: beq #0x3459d4
00345978: ldr r8, [r4, #0x2c]
0034597c: cmp r8, #0
00345980: beq #0x3459a8
00345984: ldr r3, [r8]
00345988: mov r0, r8
0034598c: mov lr, pc
00345990: ldr pc, [r3, #0x20]
00345994: cmp r0, #0
00345998: beq #0x3459a8
0034599c: ldr r3, [r8, #0x2f4]
003459a0: cmp r3, #0
003459a4: beq #0x345a0c
003459a8: ldr r2, [r4, #0xc]
003459ac: cmp r2, #0
003459b0: bne #0x3459bc
003459b4: b #0x3459d8
003459b8: mov r2, r3
003459bc: ldr r3, [r2, #8]
003459c0: cmp r3, #0
003459c4: bne #0x3459b8
003459c8: mov r4, r2
003459cc: cmp r6, r4
003459d0: bne #0x345978
003459d4: pop {r4, r5, r6, r7, r8, pc}
003459d8: ldr r3, [r4, #4]
003459dc: ldr r1, [r3, #0xc]
003459e0: cmp r4, r1
003459e4: bne #0x345a00
003459e8: mov r4, r3
003459ec: ldr r3, [r3, #4]
003459f0: ldr r2, [r3, #0xc]
003459f4: cmp r2, r4
003459f8: beq #0x3459e8
003459fc: ldr r2, [r4, #0xc]
00345a00: cmp r3, r2
00345a04: movne r4, r3
00345a08: b #0x345970
00345a0c: ldr r3, [r7, #0x88]
00345a10: cmp r3, r5
00345a14: beq #0x345a3c
00345a18: ldr r2, [r3, #8]
00345a1c: cmp r8, r2
00345a20: beq #0x345a34
00345a24: ldr r3, [r3]
00345a28: cmp r5, r3
00345a2c: bne #0x345a18
00345a30: mov r3, r5
00345a34: cmp r3, r5
00345a38: bne #0x3459a8
00345a3c: mov r1, r8
00345a40: mov r0, r7
00345a44: bl #0x344184
00345a48: b #0x3459a8

# 0x345bcc _ZNK13ObjectManager19GetNumObjectsByTypeEPKc
00345bcc: push {r4, r5, lr}
00345bd0: sub sp, sp, #0xc
00345bd4: mov r2, sp
00345bd8: str sp, [sp]
00345bdc: str sp, [sp, #4]
00345be0: bl #0x3426b0
00345be4: ldr r3, [sp]
00345be8: mov r4, sp
00345bec: cmp r3, r4
00345bf0: moveq r5, #0
00345bf4: beq #0x345c0c
00345bf8: mov r5, #0
00345bfc: ldr r3, [r3]
00345c00: add r5, r5, #1
00345c04: cmp r3, r4
00345c08: bne #0x345bfc
00345c0c: mov r0, sp
00345c10: bl #0x345b8c
00345c14: mov r0, r5
00345c18: add sp, sp, #0xc
00345c1c: pop {r4, r5, pc}

# 0x345c20 _ZN13ObjectManager15GetObjectByTypeEPKc
00345c20: push {r4, r5, lr}
00345c24: sub sp, sp, #0xc
00345c28: mov r5, r0
00345c2c: mov r0, r1
00345c30: mov r1, r2
00345c34: mov r2, sp
00345c38: str sp, [sp]
00345c3c: str sp, [sp, #4]
00345c40: bl #0x3426b0
00345c44: ldr r2, [sp]
00345c48: mov r4, sp
00345c4c: cmp r2, r4
00345c50: beq #0x345c84
00345c54: mov r3, r2
00345c58: ldr r3, [r3]
00345c5c: cmp r3, r4
00345c60: bne #0x345c58
00345c64: ldr r1, [r2, #8]
00345c68: mov r0, r5
00345c6c: bl #0x33f524
00345c70: mov r0, sp
00345c74: bl #0x345b8c
00345c78: mov r0, r5
00345c7c: add sp, sp, #0xc
00345c80: pop {r4, r5, pc}
00345c84: mov r0, r5
00345c88: mov r1, #0
00345c8c: bl #0x33f524
00345c90: b #0x345c70

# 0x345e30 _ZN13ObjectManager22AskNetResendByObjectIdEi
00345e30: push {r4, r5, r6, lr}
00345e34: sub sp, sp, #0x20
00345e38: add r6, sp, #4
00345e3c: mov r4, r1
00345e40: mov r2, r4
00345e44: mov r1, r0
00345e48: mov r5, r0
00345e4c: mov r0, r6
00345e50: bl #0x3407a0
00345e54: mov r0, r6
00345e58: bl #0x33fee4
00345e5c: ldr r3, [r0, #0x110]
00345e60: add r6, sp, #0x20
00345e64: add r5, r5, #0x194
00345e68: str r3, [r6, #-8]!
00345e6c: mov r0, r5
00345e70: mov r1, r6
00345e74: bl #0x345d04
00345e78: ldr r3, [r0, #4]
00345e7c: cmp r3, #0
00345e80: beq #0x345ed8
00345e84: mov r1, r0
00345e88: sxth ip, r4
00345e8c: b #0x345e94
00345e90: mov r3, r2
00345e94: ldrsh r2, [r3, #0x10]
00345e98: cmp r2, ip
00345e9c: ldrlt r2, [r3, #0xc]
00345ea0: ldrge r2, [r3, #8]
00345ea4: movlt r3, r1
00345ea8: mov r1, r3
00345eac: cmp r2, #0
00345eb0: bne #0x345e90
00345eb4: cmp r0, r3
00345eb8: beq #0x345ee0
00345ebc: ldrsh r2, [r3, #0x10]
00345ec0: cmp r2, ip
00345ec4: bgt #0x345ed8
00345ec8: cmp r0, r3
00345ecc: beq #0x345ee0
00345ed0: add sp, sp, #0x20
00345ed4: pop {r4, r5, r6, pc}
00345ed8: mov r3, r0
00345edc: b #0x345ec8
00345ee0: mov r1, r6
00345ee4: mov r0, r5
00345ee8: bl #0x345d04
00345eec: add r2, sp, #0x1e
00345ef0: mov r1, r0
00345ef4: add r0, sp, #0x10
00345ef8: strh r4, [sp, #0x1e]
00345efc: bl #0x344994
00345f00: b #0x345ed0

# 0x345fbc _ZN13ObjectManager14DelRoomObjectsEPSt4listIP10GameObjectSaIS2_EE
00345fbc: push {r4, r5, r6, lr}
00345fc0: ldr r3, [pc, #0xb0]
00345fc4: subs r4, r1, #0
00345fc8: sub sp, sp, #8
00345fcc: mov r5, r0
00345fd0: add r3, pc, r3
00345fd4: beq #0x346024
00345fd8: ldr r0, [r5, #0x80]!
00345fdc: cmp r5, r0
00345fe0: beq #0x346000
00345fe4: ldr r3, [r0, #8]
00345fe8: ldr r6, [r0]
00345fec: cmp r4, r3
00345ff0: beq #0x346008
00345ff4: mov r0, r6
00345ff8: cmp r5, r0
00345ffc: bne #0x345fe4
00346000: add sp, sp, #8
00346004: pop {r4, r5, r6, pc}
00346008: ldr r3, [r0, #4]
0034600c: mov r1, #0xc
00346010: str r6, [r3]
00346014: str r3, [r6, #4]
00346018: bl #0x708f00
0034601c: mov r0, r6
00346020: b #0x345ff8
00346024: ldr r2, [pc, #0x50]
00346028: ldr r2, [r3, r2]
0034602c: ldr r2, [r2]
00346030: cmp r2, #2
00346034: streq r4, [r4]
00346038: beq #0x345fd8
0034603c: cmp r2, #1
00346040: bne #0x345fd8
00346044: ldr r0, [pc, #0x34]
00346048: ldr r1, [pc, #0x34]
0034604c: ldr r2, [pc, #0x34]
00346050: ldr r0, [r3, r0]
00346054: ldr r3, [pc, #0x30]
00346058: movw ip, #0x912
0034605c: add r1, pc, r1
00346060: add r2, pc, r2
00346064: add r3, pc, r3
00346068: add r0, r0, #0xa8
0034606c: str ip, [sp]
00346070: bl #0x30e004
00346074: b #0x345fd8
00346078: rsbeq lr, r4, r0, asr #21
0034607c: andeq r3, r0, r0, asr #19
00346080: andeq r1, r0, r0, asr #19
00346084: subseq r8, r7, ip, ror r3
00346088: subseq sp, r7, r8, lsr #31
0034608c: subseq sl, r7, r4, lsr r2

# 0x3460cc _ZN13ObjectManager34ProcessNextGameObjectToStartUpdateEv
003460cc: push {r4, lr}
003460d0: mov r3, r0
003460d4: mov r4, r0
003460d8: ldr r0, [r3, #0x90]!
003460dc: cmp r0, r3
003460e0: beq #0x346118
003460e4: ldr r3, [r0, #8]
003460e8: cmp r3, #0
003460ec: beq #0x3460fc
003460f0: mov r0, r3
003460f4: bl #0x38c710
003460f8: ldr r0, [r4, #0x90]
003460fc: ldr r3, [r0]
00346100: ldr r2, [r0, #4]
00346104: mov r1, #0xc
00346108: str r3, [r2]
0034610c: str r2, [r3, #4]
00346110: pop {r4, lr}
00346114: b #0x708f00
00346118: pop {r4, pc}

# 0x346178 _ZN13ObjectManager25ProcessAcknowledgedPacketEii
00346178: push {r4, r5, r6, r7, r8, lr}
0034617c: mov r7, r0
00346180: ldr r4, [r7, #0x100]!
00346184: mov r5, r1
00346188: mov r6, r2
0034618c: cmp r7, r4
00346190: mov r8, r0
00346194: mov r1, r5
00346198: mov r2, r6
0034619c: beq #0x3461d4
003461a0: ldr r3, [r4, #8]
003461a4: ldr r3, [r3, #0x100]
003461a8: cmp r3, #0
003461ac: mov r0, r3
003461b0: beq #0x3461c0
003461b4: ldr r3, [r3]
003461b8: mov lr, pc
003461bc: ldr pc, [r3, #0x40]
003461c0: ldr r4, [r4]
003461c4: mov r1, r5
003461c8: mov r2, r6
003461cc: cmp r7, r4
003461d0: bne #0x3461a0
003461d4: ldr r3, [r8, #0x9c]
003461d8: add r8, r8, #0x98
003461dc: cmp r3, #0
003461e0: beq #0x346278
003461e4: mov r1, r8
003461e8: b #0x3461f0
003461ec: mov r3, r2
003461f0: ldr r2, [r3, #0x10]
003461f4: cmp r2, r5
003461f8: ldrlt r2, [r3, #0xc]
003461fc: ldrge r2, [r3, #8]
00346200: movlt r3, r1
00346204: mov r1, r3
00346208: cmp r2, #0
0034620c: bne #0x3461ec
00346210: cmp r8, r3
00346214: beq #0x346280
00346218: ldr r2, [r3, #0x10]
0034621c: cmp r2, r5
00346220: bgt #0x346278
00346224: cmp r8, r3
00346228: beq #0x346280
0034622c: ldr r0, [r3, #0x14]!
00346230: cmp r0, r3
00346234: beq #0x346280
00346238: ldr r2, [r0, #8]
0034623c: cmp r2, r6
00346240: beq #0x346254
00346244: ldr r0, [r0]
00346248: cmp r3, r0
0034624c: bne #0x346238
00346250: mov r0, r3
00346254: cmp r0, r3
00346258: beq #0x346280
0034625c: ldr r3, [r0]
00346260: ldr r2, [r0, #4]
00346264: mov r1, #0xc
00346268: str r3, [r2]
0034626c: str r2, [r3, #4]
00346270: pop {r4, r5, r6, r7, r8, lr}
00346274: b #0x708f00
00346278: mov r3, r8
0034627c: b #0x346224
00346280: pop {r4, r5, r6, r7, r8, pc}

# 0x346284 _ZN13ObjectManager26sProcessAcknowledgedPacketEii
00346284: ldr r3, [pc, #0x24]
00346288: ldr r2, [pc, #0x24]
0034628c: mov ip, r0
00346290: add r3, pc, r3
00346294: ldr r0, [r3, r2]
00346298: mov r2, r1
0034629c: ldr r0, [r0, #0x38]
003462a0: cmp r0, #0
003462a4: bxeq lr
003462a8: mov r1, ip
003462ac: b #0x346178
003462b0: rsbeq lr, r4, r0, lsl #16
003462b4: strdeq r3, r4, [r0], -r4

# 0x3462b8 _ZN13ObjectManager18RemoveNoRoomObjectEP10GameObject
003462b8: push {r4, lr}
003462bc: ldr r3, [r0, #0x88]!
003462c0: mov r4, r1
003462c4: cmp r3, r0
003462c8: beq #0x3462e8
003462cc: ldr r2, [r3, #8]
003462d0: cmp r2, r4
003462d4: beq #0x3462e8
003462d8: ldr r3, [r3]
003462dc: cmp r0, r3
003462e0: bne #0x3462cc
003462e4: mov r3, r0
003462e8: cmp r0, r3
003462ec: beq #0x346310
003462f0: ldm r3, {r2, ip}
003462f4: mov r0, r3
003462f8: mov r1, #0xc
003462fc: str r2, [ip]
00346300: str ip, [r2, #4]
00346304: bl #0x708f00
00346308: mov r3, #0
0034630c: strb r3, [r4, #0x2f8]
00346310: pop {r4, pc}

# 0x346510 _ZN13ObjectManager26SerializeGameObjectNetDataEiiR12NetBitStream
00346510: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00346514: ldr r5, [pc, #0xb48]
00346518: sub sp, sp, #0xbc
0034651c: str r0, [sp, #0xc]
00346520: ldr r0, [pc, #0xb40]
00346524: add r5, pc, r5
00346528: mov fp, r3
0034652c: ldr r4, [r5, r0]
00346530: str r1, [sp, #0x34]
00346534: str r2, [sp, #0x20]
00346538: mov r0, r4
0034653c: bl #0x31f594
00346540: ldr r3, [sp, #0xc]
00346544: ldrb r1, [r0, #0x3c]
00346548: mov r2, #8
0034654c: add r3, r3, #0x164
00346550: mov r0, fp
00346554: str r3, [sp, #8]
00346558: bl #0x80e4e8
0034655c: ldr r2, [sp, #0xc]
00346560: add r7, sp, #0x34
00346564: add ip, sp, #0x94
00346568: add r2, r2, #0x17c
0034656c: mov r1, r7
00346570: ldr r0, [sp, #8]
00346574: str ip, [sp, #0x18]
00346578: str r2, [sp, #0x10]
0034657c: str ip, [sp, #0x94]
00346580: str ip, [sp, #0x98]
00346584: bl #0x345d04
00346588: mov r1, r7
0034658c: ldr r0, [sp, #0x10]
00346590: bl #0x345d04
00346594: ldr r0, [r0, #0x10]
00346598: str r0, [sp, #0x2c]
0034659c: ldr r0, [r4, #0x40]
003465a0: bl #0x36f074
003465a4: cmp r0, #0
003465a8: beq #0x346768
003465ac: ldr r1, [sp, #0xc]
003465b0: ldr r3, [r1, #0x128]!
003465b4: cmp r3, r1
003465b8: beq #0x3465e4
003465bc: ldr r0, [sp, #0x34]
003465c0: ldr r2, [r3, #8]
003465c4: cmp r2, r0
003465c8: beq #0x3465e4
003465cc: ldr r3, [r3]
003465d0: cmp r1, r3
003465d4: beq #0x346b20
003465d8: ldr r2, [r3, #8]
003465dc: cmp r2, r0
003465e0: bne #0x3465cc
003465e4: cmp r1, r3
003465e8: beq #0x346768
003465ec: ldr r3, [sp, #0xc]
003465f0: mov r1, r7
003465f4: add r0, r3, #0x98
003465f8: bl #0x345a8c
003465fc: mov r4, r0
00346600: bl #0x3441d8
00346604: ldr ip, [sp, #0x20]
00346608: mov r3, r0
0034660c: mov r1, #1
00346610: str ip, [r0, #8]
00346614: ldr r2, [r4, #4]
00346618: str r4, [r0]
0034661c: mov r0, fp
00346620: str r2, [r3, #4]
00346624: str r3, [r2]
00346628: str r3, [r4, #4]
0034662c: bl #0x80e42c
00346630: ldr r1, [sp, #0xc]
00346634: ldr ip, [r1, #0xb4]
00346638: add r1, r1, #0xb0
0034663c: cmp ip, #0
00346640: ldreq lr, [sp, #0x34]
00346644: moveq ip, r1
00346648: beq #0x34667c
0034664c: ldr lr, [sp, #0x34]
00346650: mov r2, r1
00346654: b #0x34665c
00346658: mov ip, r3
0034665c: ldr r3, [ip, #0x10]
00346660: cmp r3, lr
00346664: ldrlt r3, [ip, #0xc]
00346668: ldrge r3, [ip, #8]
0034666c: movlt ip, r2
00346670: mov r2, ip
00346674: cmp r3, #0
00346678: bne #0x346658
0034667c: cmp r1, ip
00346680: beq #0x346694
00346684: ldr r2, [ip, #0x10]
00346688: mov r3, ip
0034668c: cmp r2, lr
00346690: ble #0x3466b8
00346694: add r3, sp, #0x84
00346698: str ip, [sp, #0xac]
0034669c: add r0, sp, #0xb0
003466a0: mov ip, #0
003466a4: add r2, sp, #0xac
003466a8: str lr, [sp, #0x84]
003466ac: strh ip, [sp, #0x88]
003466b0: bl #0x34371c
003466b4: ldr r3, [sp, #0xb0]
003466b8: ldrb r1, [r3, #0x14]
003466bc: mov r0, fp
003466c0: mov r2, #8
003466c4: bl #0x80e4e8
003466c8: ldr r1, [sp, #0xc]
003466cc: ldr ip, [r1, #0xe4]
003466d0: add r1, r1, #0xe0
003466d4: cmp ip, #0
003466d8: ldreq lr, [sp, #0x34]
003466dc: moveq ip, r1
003466e0: beq #0x346714
003466e4: ldr lr, [sp, #0x34]
003466e8: mov r2, r1
003466ec: b #0x3466f4
003466f0: mov ip, r3
003466f4: ldr r3, [ip, #0x10]
003466f8: cmp lr, r3
003466fc: ldrgt r3, [ip, #0xc]
00346700: ldrle r3, [ip, #8]
00346704: movgt ip, r2
00346708: mov r2, ip
0034670c: cmp r3, #0
00346710: bne #0x3466f0
00346714: cmp r1, ip
00346718: beq #0x34672c
0034671c: ldr r2, [ip, #0x10]
00346720: mov r3, ip
00346724: cmp r2, lr
00346728: ble #0x346750
0034672c: add r3, sp, #0x7c
00346730: str ip, [sp, #0xa4]
00346734: add r0, sp, #0xa8
00346738: mov ip, #0
0034673c: add r2, sp, #0xa4
00346740: str lr, [sp, #0x7c]
00346744: strh ip, [sp, #0x80]
00346748: bl #0x34371c
0034674c: ldr r3, [sp, #0xa8]
00346750: ldrh r2, [r3, #0x14]
00346754: mov r1, #1
00346758: str r1, [sp, #0x1c]
0034675c: add r2, r2, r1
00346760: strh r2, [r3, #0x14]
00346764: b #0x34677c
00346768: mov r1, #0
0034676c: mov r0, fp
00346770: bl #0x80e42c
00346774: mov r1, #0
00346778: str r1, [sp, #0x1c]
0034677c: ldr r2, [sp, #0xc]
00346780: mov r1, r7
00346784: add r2, r2, #0x108
00346788: mov r0, r2
0034678c: str r2, [sp, #0x24]
00346790: bl #0x3452ac
00346794: ldr r2, [fp, #0x10]
00346798: mov r4, r0
0034679c: ldr r6, [r0]
003467a0: ands r3, r2, #7
003467a4: lsr r1, r2, #3
003467a8: movne r3, #1
003467ac: rsb r1, r1, #0x570
003467b0: rsb r1, r3, r1
003467b4: cmp r1, #0
003467b8: ble #0x347030
003467bc: add r8, sp, #0x3c
003467c0: mov r0, r8
003467c4: bl #0x80e908
003467c8: cmp r6, r4
003467cc: bne #0x346e04
003467d0: ldr r3, [sp, #0x1c]
003467d4: cmp r3, #0
003467d8: bne #0x346e50
003467dc: ldr sb, [sp, #0xc]
003467e0: ldr sl, [sp, #0xc]
003467e4: ldr r6, [sb, #0x100]!
003467e8: str fp, [sp, #0x14]
003467ec: str r8, [sp, #0x28]
003467f0: cmp r6, sb
003467f4: ldr fp, [sp, #8]
003467f8: beq #0x3468a8
003467fc: mov r1, r7
00346800: mov r0, fp
00346804: ldr r8, [r6, #8]
00346808: bl #0x345d04
0034680c: ldr r4, [r0, #4]
00346810: mov r5, r0
00346814: ldr r1, [r8, #0x108]
00346818: cmp r4, #0
0034681c: beq #0x346ad0
00346820: sxth r1, r1
00346824: mov r2, r0
00346828: b #0x346830
0034682c: mov r4, r3
00346830: ldrsh r3, [r4, #0x10]
00346834: cmp r3, r1
00346838: ldrlt r3, [r4, #0xc]
0034683c: ldrge r3, [r4, #8]
00346840: movlt r4, r2
00346844: mov r2, r4
00346848: cmp r3, #0
0034684c: bne #0x34682c
00346850: cmp r5, r4
00346854: beq #0x346864
00346858: ldrsh r3, [r4, #0x10]
0034685c: cmp r3, r1
00346860: bgt #0x346ad0
00346864: mov r1, r8
00346868: mov r0, sl
0034686c: ldr r2, [sp, #0x34]
00346870: bl #0x3409f4
00346874: subs r1, r0, #0
00346878: beq #0x346aec
0034687c: ldr r0, [sp, #0x18]
00346880: bl #0x343168
00346884: str r8, [r0, #8]
00346888: ldr r3, [sp, #0x98]
0034688c: ldr r2, [sp, #0x18]
00346890: stm r0, {r2, r3}
00346894: str r0, [r3]
00346898: str r0, [sp, #0x98]
0034689c: ldr r6, [r6]
003468a0: cmp r6, sb
003468a4: bne #0x3467fc
003468a8: ldr r3, [sp, #0x18]
003468ac: ldr r5, [sp, #0x94]
003468b0: ldr fp, [sp, #0x14]
003468b4: ldr r8, [sp, #0x28]
003468b8: cmp r5, r3
003468bc: moveq r0, r3
003468c0: beq #0x3468dc
003468c4: ldr r2, [sp, #0x18]
003468c8: mov r3, r5
003468cc: ldr r3, [r3]
003468d0: cmp r3, r2
003468d4: bne #0x3468cc
003468d8: ldr r0, [sp, #0x18]
003468dc: add sl, sp, #0x8c
003468e0: mov sb, #0
003468e4: add ip, sp, #0xb4
003468e8: str fp, [sp, #0x28]
003468ec: str sl, [sp, #0x8c]
003468f0: str sl, [sp, #0x90]
003468f4: mov r3, sb
003468f8: str sb, [sp, #0x14]
003468fc: str ip, [sp, #0x30]
00346900: mov fp, r0
00346904: b #0x34693c
00346908: mov r0, sl
0034690c: str r3, [sp, #4]
00346910: bl #0x343168
00346914: str r4, [r0, #8]
00346918: ldr r2, [sp, #0x90]
0034691c: ldr r3, [sp, #4]
00346920: str sl, [r0]
00346924: str r2, [r0, #4]
00346928: mov r6, r3
0034692c: str r0, [r2]
00346930: str r0, [sp, #0x90]
00346934: ldr r5, [r5]
00346938: mov r3, r6
0034693c: cmp r5, fp
00346940: beq #0x346b78
00346944: cmp sb, #0
00346948: ldr r4, [r5, #8]
0034694c: bne #0x346908
00346950: mov r0, r8
00346954: str r3, [sp, #4]
00346958: bl #0x80e410
0034695c: ldr r6, [r4, #0x108]
00346960: ldr r3, [sp, #4]
00346964: rsb r3, r3, r6
00346968: cmp r3, #1
0034696c: beq #0x346b68
00346970: mov r0, r8
00346974: mov r1, #1
00346978: bl #0x80e42c
0034697c: mov r0, r8
00346980: mov r1, r6
00346984: mov r2, #0x10
00346988: bl #0x80e5dc
0034698c: mov r2, #8
00346990: mov r0, r8
00346994: ldrb r1, [r4, #0xf8]
00346998: bl #0x80e4e8
0034699c: ldr r0, [sp, #8]
003469a0: mov r1, r7
003469a4: bl #0x345d04
003469a8: ldr r3, [r0, #4]
003469ac: cmp r3, #0
003469b0: beq #0x346b30
003469b4: mov r1, r0
003469b8: sxth ip, r6
003469bc: b #0x3469c4
003469c0: mov r3, r2
003469c4: ldrsh r2, [r3, #0x10]
003469c8: cmp r2, ip
003469cc: ldrlt r2, [r3, #0xc]
003469d0: ldrge r2, [r3, #8]
003469d4: movlt r3, r1
003469d8: mov r1, r3
003469dc: cmp r2, #0
003469e0: bne #0x3469c0
003469e4: cmp r0, r3
003469e8: beq #0x346a74
003469ec: ldrsh r2, [r3, #0x10]
003469f0: cmp r2, ip
003469f4: bgt #0x346b30
003469f8: cmp r0, r3
003469fc: beq #0x346a74
00346a00: ldr r0, [r4, #0x100]
00346a04: bl #0x81347c
00346a08: mov r1, r7
00346a0c: ldr r0, [sp, #8]
00346a10: bl #0x345d04
00346a14: ldr r3, [r0, #4]
00346a18: uxth ip, r6
00346a1c: cmp r3, #0
00346a20: sxthne ip, ip
00346a24: movne r1, r0
00346a28: bne #0x346a34
00346a2c: b #0x346a74
00346a30: mov r3, r2
00346a34: ldrsh r2, [r3, #0x10]
00346a38: cmp r2, ip
00346a3c: ldrlt r2, [r3, #0xc]
00346a40: ldrge r2, [r3, #8]
00346a44: movlt r3, r1
00346a48: mov r1, r3
00346a4c: cmp r2, #0
00346a50: bne #0x346a30
00346a54: cmp r0, r3
00346a58: beq #0x346a74
00346a5c: ldrsh r2, [r3, #0x10]
00346a60: cmp r2, ip
00346a64: bgt #0x346a74
00346a68: ldr r1, [sp, #0x30]
00346a6c: str r3, [sp, #0xb4]
00346a70: bl #0x346090
00346a74: ldr r3, [r4]
00346a78: mov r0, r4
00346a7c: ldr r1, [sp, #0x1c]
00346a80: mov lr, pc
00346a84: ldr pc, [r3, #0x4c]
00346a88: ldr r0, [r4, #0x100]
00346a8c: mov r1, #1
00346a90: bl #0x81366c
00346a94: ldr r3, [r4, #0x100]
00346a98: mov r1, r8
00346a9c: ldr r2, [sp, #0x34]
00346aa0: mov r0, r3
00346aa4: ldr ip, [r3]
00346aa8: ldr r3, [sp, #0x20]
00346aac: mov lr, pc
00346ab0: ldr pc, [ip, #8]
00346ab4: ldr r3, [sp, #0x58]
00346ab8: cmp r3, #0
00346abc: bne #0x346b38
00346ac0: ldr r1, [sp, #0x14]
00346ac4: add r1, r1, #1
00346ac8: str r1, [sp, #0x14]
00346acc: b #0x346934
00346ad0: mov r1, r8
00346ad4: mov r0, sl
00346ad8: ldr r2, [sp, #0x34]
00346adc: bl #0x3409f4
00346ae0: subs r1, r0, #0
00346ae4: mov r4, r5
00346ae8: bne #0x34687c
00346aec: cmp r5, r4
00346af0: beq #0x346b08
00346af4: ldr r3, [r8, #0x100]
00346af8: cmp r3, #0
00346afc: bne #0x34687c
00346b00: ldr r6, [r6]
00346b04: b #0x3468a0
00346b08: ldr r0, [r8, #0x100]
00346b0c: cmp r0, #0
00346b10: beq #0x34689c
00346b14: bl #0x81366c
00346b18: ldr r6, [r6]
00346b1c: b #0x3468a0
00346b20: mov r3, r1
00346b24: cmp r1, r3
00346b28: bne #0x3465ec
00346b2c: b #0x346768
00346b30: mov r3, r0
00346b34: b #0x3469f8
00346b38: mov r0, r8
00346b3c: bl #0x80e83c
00346b40: mov r0, sl
00346b44: bl #0x343168
00346b48: str r4, [r0, #8]
00346b4c: ldr r3, [sp, #0x90]
00346b50: mov sb, #1
00346b54: str sl, [r0]
00346b58: str r3, [r0, #4]
00346b5c: str r0, [r3]
00346b60: str r0, [sp, #0x90]
00346b64: b #0x346934
00346b68: mov r0, r8
00346b6c: mov r1, sb
00346b70: bl #0x80e42c
00346b74: b #0x34698c
00346b78: ldr fp, [sp, #0x28]
00346b7c: mov r2, #0x10
00346b80: ldr r1, [sp, #0x14]
00346b84: mov r0, fp
00346b88: bl #0x80e5dc
00346b8c: mov r0, fp
00346b90: mov r1, r8
00346b94: bl #0x80ed54
00346b98: mov r1, r7
00346b9c: ldr r0, [sp, #0x24]
00346ba0: bl #0x3452ac
00346ba4: bl #0x34526c
00346ba8: cmp sb, #0
00346bac: bne #0x346e24
00346bb0: ldr r5, [sp, #0xc]
00346bb4: ldr r0, [r5, #0x128]!
00346bb8: cmp r5, r0
00346bbc: beq #0x346be0
00346bc0: ldr r3, [r0, #8]
00346bc4: ldr r2, [sp, #0x34]
00346bc8: ldr r4, [r0]
00346bcc: cmp r2, r3
00346bd0: beq #0x346d30
00346bd4: mov r0, r4
00346bd8: cmp r5, r0
00346bdc: bne #0x346bc0
00346be0: ldr r2, [sp, #0x1c]
00346be4: cmp r2, #0
00346be8: beq #0x346d80
00346bec: mov r1, #1
00346bf0: mov r0, fp
00346bf4: bl #0x80e42c
00346bf8: ldr r3, [sp, #0xc]
00346bfc: ldr ip, [r3, #0xe4]
00346c00: add r1, r3, #0xe0
00346c04: cmp ip, #0
00346c08: ldreq lr, [sp, #0x34]
00346c0c: moveq ip, r1
00346c10: beq #0x346c44
00346c14: ldr lr, [sp, #0x34]
00346c18: mov r2, r1
00346c1c: b #0x346c24
00346c20: mov ip, r3
00346c24: ldr r3, [ip, #0x10]
00346c28: cmp r3, lr
00346c2c: ldrlt r3, [ip, #0xc]
00346c30: ldrge r3, [ip, #8]
00346c34: movlt ip, r2
00346c38: mov r2, ip
00346c3c: cmp r3, #0
00346c40: bne #0x346c20
00346c44: cmp r1, ip
00346c48: beq #0x347000
00346c4c: ldr r2, [ip, #0x10]
00346c50: mov r3, ip
00346c54: cmp r2, lr
00346c58: bgt #0x347000
00346c5c: ldrb r1, [r3, #0x14]
00346c60: mov r0, fp
00346c64: mov r2, #8
00346c68: bl #0x80e4e8
00346c6c: ldr r1, [sp, #0x2c]
00346c70: cmp r1, #0
00346c74: ble #0x346d98
00346c78: mov r0, fp
00346c7c: mov r1, #1
00346c80: bl #0x80e42c
00346c84: mov r0, fp
00346c88: ldr r1, [sp, #0x2c]
00346c8c: mov r2, #0x10
00346c90: bl #0x80e5dc
00346c94: ldr r0, [sp, #0x10]
00346c98: mov r1, r7
00346c9c: bl #0x345d04
00346ca0: ldr r4, [r0, #8]
00346ca4: ldr r0, [sp, #0x10]
00346ca8: mov r1, r7
00346cac: bl #0x345d04
00346cb0: cmp r4, r0
00346cb4: beq #0x346d00
00346cb8: mov r2, #0x10
00346cbc: mov r0, fp
00346cc0: ldrsh r1, [r4, #0x10]
00346cc4: bl #0x80e5dc
00346cc8: ldr r2, [r4, #0xc]
00346ccc: cmp r2, #0
00346cd0: bne #0x346cdc
00346cd4: b #0x346d4c
00346cd8: mov r2, r3
00346cdc: ldr r3, [r2, #8]
00346ce0: cmp r3, #0
00346ce4: bne #0x346cd8
00346ce8: ldr r0, [sp, #0x10]
00346cec: mov r1, r7
00346cf0: mov r4, r2
00346cf4: bl #0x345d04
00346cf8: cmp r4, r0
00346cfc: bne #0x346cb8
00346d00: ldr r2, [sp, #0xc]
00346d04: ldrb r3, [r2, #0xfd]
00346d08: cmp r3, #0
00346d0c: bne #0x346db4
00346d10: mov r0, sl
00346d14: bl #0x34526c
00346d18: mov r0, r8
00346d1c: bl #0x80e790
00346d20: ldr r0, [sp, #0x18]
00346d24: bl #0x34526c
00346d28: add sp, sp, #0xbc
00346d2c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00346d30: ldr r3, [r0, #4]
00346d34: mov r1, #0xc
00346d38: str r4, [r3]
00346d3c: str r3, [r4, #4]
00346d40: bl #0x708f00
00346d44: mov r0, r4
00346d48: b #0x346bd8
00346d4c: ldr r3, [r4, #4]
00346d50: ldr r1, [r3, #0xc]
00346d54: cmp r4, r1
00346d58: bne #0x346d74
00346d5c: mov r4, r3
00346d60: ldr r3, [r3, #4]
00346d64: ldr r2, [r3, #0xc]
00346d68: cmp r4, r2
00346d6c: beq #0x346d5c
00346d70: ldr r2, [r4, #0xc]
00346d74: cmp r3, r2
00346d78: movne r4, r3
00346d7c: b #0x346ca4
00346d80: ldr r1, [sp, #0x1c]
00346d84: mov r0, fp
00346d88: bl #0x80e42c
00346d8c: ldr r1, [sp, #0x2c]
00346d90: cmp r1, #0
00346d94: bgt #0x346c78
00346d98: mov r0, fp
00346d9c: mov r1, #0
00346da0: bl #0x80e42c
00346da4: ldr r2, [sp, #0xc]
00346da8: ldrb r3, [r2, #0xfd]
00346dac: cmp r3, #0
00346db0: beq #0x346d10
00346db4: bl #0x7fbd74
00346db8: mov r2, #0
00346dbc: mov r1, r0
00346dc0: add r0, sp, #0x5c
00346dc4: bl #0x7fd284
00346dc8: ldr r3, [sp, #0x60]
00346dcc: ldr r2, [r3, #-4]
00346dd0: ldr r3, [sp, #0x34]
00346dd4: cmp r2, r3
00346dd8: beq #0x34704c
00346ddc: ldr r0, [sp, #0x5c]
00346de0: cmp r0, #0
00346de4: beq #0x346d10
00346de8: ldr r1, [sp, #0x64]
00346dec: rsb r1, r0, r1
00346df0: bic r1, r1, #3
00346df4: cmp r1, #0x80
00346df8: bhi #0x347028
00346dfc: bl #0x708f00
00346e00: b #0x346d10
00346e04: mov r1, r7
00346e08: ldr r0, [sp, #0x24]
00346e0c: bl #0x3452ac
00346e10: mov r1, r7
00346e14: ldr r5, [r0]
00346e18: ldr r0, [sp, #0x24]
00346e1c: bl #0x3452ac
00346e20: b #0x3468dc
00346e24: mov r1, r7
00346e28: ldr r0, [sp, #0x24]
00346e2c: bl #0x3452ac
00346e30: mov r1, sl
00346e34: bl #0x346474
00346e38: mov r0, sl
00346e3c: bl #0x34526c
00346e40: mov r0, fp
00346e44: mov r1, #0
00346e48: bl #0x80e42c
00346e4c: b #0x346c6c
00346e50: bl #0x800f8c
00346e54: ldr r3, [r0]
00346e58: mov lr, pc
00346e5c: ldr pc, [r3, #0x6c]
00346e60: ldr r3, [pc, #0x204]
00346e64: str r0, [sp, #0x28]
00346e68: ldr r5, [r5, r3]
00346e6c: ldr r2, [r5, #0x1c]
00346e70: ldr r3, [r5, #0x18]
00346e74: rsb r3, r3, r2
00346e78: asr r3, r3, #2
00346e7c: add r6, r3, r3, lsl #2
00346e80: add r6, r6, r6, lsl #4
00346e84: add r6, r6, r6, lsl #8
00346e88: add r6, r6, r6, lsl #16
00346e8c: add r6, r3, r6, lsl #1
00346e90: cmp r6, #0
00346e94: ble #0x346f1c
00346e98: ldr r3, [pc, #0x1d0]
00346e9c: mov r4, #0
00346ea0: add r3, pc, r3
00346ea4: str r3, [sp, #0x14]
00346ea8: b #0x346eb8
00346eac: add r4, r4, #1
00346eb0: cmp r4, r6
00346eb4: beq #0x346f1c
00346eb8: mov r1, r4
00346ebc: mov r0, r5
00346ec0: bl #0x455bec
00346ec4: cmp r0, #0
00346ec8: beq #0x346eac
00346ecc: mov r1, r4
00346ed0: mov r0, r5
00346ed4: bl #0x455c40
00346ed8: mov sb, r0
00346edc: bl #0x80b1bc
00346ee0: mov r1, #1
00346ee4: mov sl, r0
00346ee8: ldr r0, [sp, #0x14]
00346eec: bl #0x80a244
00346ef0: mov ip, #4
00346ef4: str r4, [r0, #0x54]
00346ef8: str sb, [r0, #0x58]
00346efc: str ip, [r0, #0x50]
00346f00: mov r1, r0
00346f04: ldr r2, [sp, #0x34]
00346f08: mov r0, sl
00346f0c: add r4, r4, #1
00346f10: bl #0x80e23c
00346f14: cmp r4, r6
00346f18: bne #0x346eb8
00346f1c: ldr sb, [sp, #0xc]
00346f20: add r6, sp, #0x68
00346f24: mov sl, r8
00346f28: ldr r4, [sb, #0x100]!
00346f2c: mov r0, r6
00346f30: cmp sb, r4
00346f34: beq #0x346fd0
00346f38: ldr r5, [r4, #8]
00346f3c: subs r1, r5, #0
00346f40: beq #0x346fc0
00346f44: ldr r3, [r5, #0x100]
00346f48: cmp r3, #0
00346f4c: beq #0x346fc0
00346f50: bl #0x33dd2c
00346f54: mov r0, r6
00346f58: bl #0x33ff54
00346f5c: subs r8, r0, #0
00346f60: beq #0x346fa0
00346f64: ldr r3, [r8]
00346f68: mov lr, pc
00346f6c: ldr pc, [r3, #0x28]
00346f70: cmp r0, #0
00346f74: ldr r1, [sp, #0x28]
00346f78: mov r2, r8
00346f7c: ldr r0, [sp, #0xc]
00346f80: beq #0x346f90
00346f84: bl #0x340948
00346f88: cmp r0, #0
00346f8c: beq #0x346fc0
00346f90: mov r0, r8
00346f94: bl #0x3a30c4
00346f98: cmp r0, #0
00346f9c: bne #0x346fc0
00346fa0: ldr r0, [sp, #0x18]
00346fa4: bl #0x343168
00346fa8: str r5, [r0, #8]
00346fac: ldr r3, [sp, #0x98]
00346fb0: ldr r2, [sp, #0x18]
00346fb4: stm r0, {r2, r3}
00346fb8: str r0, [r3]
00346fbc: str r0, [sp, #0x98]
00346fc0: ldr r4, [r4]
00346fc4: mov r0, r6
00346fc8: cmp sb, r4
00346fcc: bne #0x346f38
00346fd0: ldr r1, [sp, #0x18]
00346fd4: ldr r5, [sp, #0x94]
00346fd8: mov r8, sl
00346fdc: cmp r5, r1
00346fe0: moveq r0, r1
00346fe4: beq #0x3468dc
00346fe8: ldr r2, [sp, #0x18]
00346fec: mov r3, r5
00346ff0: ldr r3, [r3]
00346ff4: cmp r3, r2
00346ff8: bne #0x346ff0
00346ffc: b #0x3468d8
00347000: add r3, sp, #0x74
00347004: str ip, [sp, #0x9c]
00347008: add r0, sp, #0xa0
0034700c: mov ip, #0
00347010: add r2, sp, #0x9c
00347014: str lr, [sp, #0x74]
00347018: strh ip, [sp, #0x78]
0034701c: bl #0x34371c
00347020: ldr r3, [sp, #0xa0]
00347024: b #0x346c5c
00347028: bl #0x310440
0034702c: b #0x346d10
00347030: mov r0, fp
00347034: mov r1, #0
00347038: mov r2, #0x10
0034703c: bl #0x80e5dc
00347040: ldr r0, [sp, #0x18]
00347044: bl #0x34526c
00347048: b #0x346d28
0034704c: ldr r0, [sp, #0xc]
00347050: bl #0x340250
00347054: ldr ip, [sp, #0xc]
00347058: mov r3, #0
0034705c: strb r3, [ip, #0xfd]
00347060: b #0x346ddc
00347064: rsbeq lr, r4, ip, ror #10
00347068: strdeq r3, r4, [r0], -r4
0034706c: andeq r1, r0, r0, lsr #20
00347070: subseq r8, r7, r0, lsr #32

# 0x347074 _ZN13ObjectManager16sWritePacketDataEiiR12NetBitStream
00347074: ldr r3, [pc, #0x7c]
00347078: push {r4, r5, r6, r7, r8, lr}
0034707c: mov r7, r0
00347080: ldr r0, [pc, #0x74]
00347084: add r3, pc, r3
00347088: mov r6, r1
0034708c: ldr r4, [r3, r0]
00347090: mov r5, r2
00347094: mov r0, r4
00347098: bl #0x31f594
0034709c: cmp r0, #0
003470a0: beq #0x3470e4
003470a4: ldr r3, [r0, #0x130]
003470a8: cmp r3, #0x23
003470ac: ble #0x3470e4
003470b0: ldr r3, [r4, #0x38]
003470b4: cmp r3, #0
003470b8: beq #0x3470e4
003470bc: mov r0, r5
003470c0: mov r1, #1
003470c4: bl #0x80e42c
003470c8: ldr r0, [r4, #0x38]
003470cc: mov r1, r7
003470d0: mov r2, r6
003470d4: mov r3, r5
003470d8: bl #0x346510
003470dc: mov r0, #1
003470e0: pop {r4, r5, r6, r7, r8, pc}
003470e4: mov r0, r5
003470e8: mov r1, #0
003470ec: bl #0x80e42c
003470f0: mov r0, #0
003470f4: pop {r4, r5, r6, r7, r8, pc}
003470f8: rsbeq sp, r4, ip, lsl #20
003470fc: strdeq r3, r4, [r0], -r4

# 0x347100 _ZN13ObjectManager19InitModulesFogColorERKSt6vectorI7Point3DIfESaIS2_EEi
00347100: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00347104: ldr ip, [r1]
00347108: ldr r3, [r1, #4]
0034710c: mov r6, r0
00347110: ldr r5, [pc, #0x23c]
00347114: rsb r3, ip, r3
00347118: asr r3, r3, #2
0034711c: add r5, pc, r5
00347120: add r0, r3, r3, lsl #2
00347124: sub sp, sp, #0x34
00347128: add r0, r0, r0, lsl #4
0034712c: add r0, r0, r0, lsl #8
00347130: add r0, r0, r0, lsl #16
00347134: add r3, r3, r0, lsl #1
00347138: cmp r3, #0
0034713c: bne #0x347148
00347140: add sp, sp, #0x34
00347144: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00347148: add r3, sp, #0x20
0034714c: str r3, [sp, #0xc]
00347150: mov r0, r3
00347154: mov r3, #0x64
00347158: mul r3, r3, r2
0034715c: str r3, [sp, #4]
00347160: bl #0x3424b8
00347164: ldr r1, [sp, #0x24]
00347168: ldr r0, [sp, #0x20]
0034716c: bl #0x3411c4
00347170: mov r3, r6
00347174: ldr r4, [r3, #0x68]!
00347178: mov r1, #0
0034717c: str r1, [sp, #0x14]
00347180: cmp r4, r3
00347184: str r1, [sp, #0x18]
00347188: str r1, [sp, #0x1c]
0034718c: beq #0x3471a0
00347190: ldr r4, [r4]
00347194: add r1, r1, #1
00347198: cmp r3, r4
0034719c: bne #0x347190
003471a0: mov r7, #0
003471a4: add r0, sp, #0x14
003471a8: add r2, sp, #0x2c
003471ac: str r7, [sp, #0x2c]
003471b0: bl #0x346430
003471b4: ldr r3, [r6, #0x68]
003471b8: ldr r1, [sp, #0x14]
003471bc: b #0x3471d0
003471c0: ldr r2, [r3, #8]
003471c4: str r2, [r1, r7]
003471c8: ldr r3, [r3]
003471cc: add r7, r7, #4
003471d0: cmp r4, r3
003471d4: bne #0x3471c0
003471d8: ldr r3, [r6, #0x68]
003471dc: ldr r0, [sp, #0x14]
003471e0: ldr r1, [sp, #0x18]
003471e4: ldr r4, [r3, #8]
003471e8: mov r2, r4
003471ec: bl #0x342314
003471f0: ldr r2, [sp, #0x24]
003471f4: ldr r3, [sp, #0x20]
003471f8: ldr fp, [sp, #0x18]
003471fc: ldr r7, [sp, #0x14]
00347200: rsb r3, r3, r2
00347204: asr r3, r3, #2
00347208: cmp fp, r7
0034720c: add r2, r3, r3, lsl #2
00347210: add r2, r2, r2, lsl #4
00347214: add r2, r2, r2, lsl #8
00347218: add r2, r2, r2, lsl #16
0034721c: add r2, r3, r2, lsl #1
00347220: str r2, [sp, #8]
00347224: beq #0x347318
00347228: ldr r3, [pc, #0x128]
0034722c: str r3, [sp]
00347230: ldr r6, [r7]
00347234: ldr r0, [r4, #0x160]
00347238: add r7, r7, #4
0034723c: ldr r1, [r6, #0x160]
00347240: bl #0x30e3ac
00347244: ldr r1, [r6, #0x164]
00347248: mov sl, r0
0034724c: ldr r0, [r4, #0x164]
00347250: bl #0x30e3ac
00347254: ldr r1, [r6, #0x168]
00347258: mov sb, r0
0034725c: ldr r0, [r4, #0x168]
00347260: bl #0x30e3ac
00347264: mov r1, sl
00347268: mov r8, r0
0034726c: mov r0, sl
00347270: bl #0x30ed6c
00347274: mov r1, sb
00347278: mov sl, r0
0034727c: mov r0, sb
00347280: bl #0x30ed6c
00347284: mov r1, r0
00347288: mov r0, sl
0034728c: bl #0x30eba4
00347290: mov r1, r8
00347294: mov sl, r0
00347298: mov r0, r8
0034729c: bl #0x30ed6c
003472a0: mov r1, r0
003472a4: mov r0, sl
003472a8: bl #0x30eba4
003472ac: bl #0x30e124
003472b0: ldr r3, [sp]
003472b4: mov sl, r0
003472b8: add r0, r6, #0x3f0
003472bc: ldr r1, [r5, r3]
003472c0: ldr r8, [sp, #0x20]
003472c4: bl #0x312b6c
003472c8: cmp r0, #0
003472cc: mov r0, sl
003472d0: beq #0x34730c
003472d4: bl #0x30e4cc
003472d8: ldr r1, [sp, #4]
003472dc: bl #0x30e2a4
003472e0: ldr r1, [sp, #8]
003472e4: bl #0x30e904
003472e8: mov r3, #0xc
003472ec: mul r1, r3, r1
003472f0: ldr r3, [r8, r1]
003472f4: add r8, r8, r1
003472f8: str r3, [r6, #0x3f0]
003472fc: ldr r3, [r8, #4]
00347300: str r3, [r6, #0x3f4]
00347304: ldr r3, [r8, #8]
00347308: str r3, [r6, #0x3f8]
0034730c: cmp r7, fp
00347310: bne #0x347230
00347314: ldr fp, [sp, #0x14]
00347318: cmp fp, #0
0034731c: beq #0x34733c
00347320: ldr r1, [sp, #0x1c]
00347324: rsb r1, fp, r1
00347328: bic r1, r1, #3
0034732c: cmp r1, #0x80
00347330: bhi #0x347348
00347334: mov r0, fp
00347338: bl #0x708f00
0034733c: ldr r0, [sp, #0xc]
00347340: bl #0x34611c
00347344: b #0x347140
00347348: mov r0, fp
0034734c: bl #0x310440
00347350: b #0x34733c
00347354: rsbeq sp, r4, r4, ror sb
00347358: muleq r0, r8, r4

# 0x34735c _ZN13ObjectManager21LoadGameObjectNetDataEiiR12NetBitStream
0034735c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00347360: ldr ip, [pc, #0xa08]
00347364: ldr lr, [pc, #0xa08]
00347368: sub sp, sp, #0x94
0034736c: add ip, pc, ip
00347370: str r0, [sp, #0xc]
00347374: ldr r0, [ip, lr]
00347378: str lr, [sp, #0x18]
0034737c: str ip, [sp, #0x10]
00347380: str r1, [sp, #0x44]
00347384: mov r7, r2
00347388: mov r4, r3
0034738c: bl #0x31f594
00347390: subs r5, r0, #0
00347394: beq #0x3473b0
00347398: mov r0, r4
0034739c: mov r1, #8
003473a0: bl #0x80e564
003473a4: ldr r3, [r5, #0x3c]
003473a8: cmp r0, r3
003473ac: beq #0x3473b8
003473b0: add sp, sp, #0x94
003473b4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003473b8: add r6, sp, #0x74
003473bc: mov r0, r4
003473c0: str r6, [sp, #0x74]
003473c4: str r6, [sp, #0x78]
003473c8: bl #0x80e498
003473cc: cmp r0, #0
003473d0: streq r0, [sp, #0x1c]
003473d4: movweq sl, #0xffff
003473d8: bne #0x3479ec
003473dc: mov r0, r4
003473e0: mov r1, #0x10
003473e4: bl #0x80e630
003473e8: subs r5, r0, #0
003473ec: beq #0x34776c
003473f0: mov r2, r7
003473f4: ldr r0, [sp, #0xc]
003473f8: ldr r1, [sp, #0x44]
003473fc: bl #0x342f30
00347400: ldr r3, [sp, #0x1c]
00347404: str r0, [sp, #0x14]
00347408: cmp r3, #0
0034740c: addeq fp, sp, #0x44
00347410: bne #0x347bd4
00347414: cmp r5, #0
00347418: ble #0x347d1c
0034741c: ldr r3, [pc, #0x954]
00347420: ldr ip, [sp, #0xc]
00347424: ldr lr, [sp, #0xc]
00347428: str r3, [sp, #0x40]
0034742c: ldr r3, [pc, #0x948]
00347430: ldr r1, [pc, #0x948]
00347434: ldr r2, [pc, #0x948]
00347438: add r3, pc, r3
0034743c: str r3, [sp, #0x34]
00347440: ldr r3, [pc, #0x940]
00347444: mov r7, #0
00347448: add ip, ip, #0x17c
0034744c: add r3, pc, r3
00347450: str r3, [sp, #0x38]
00347454: ldr r3, [pc, #0x930]
00347458: add lr, lr, #0x194
0034745c: str fp, [sp, #0x24]
00347460: add r3, pc, r3
00347464: str r1, [sp, #0x2c]
00347468: str r2, [sp, #0x30]
0034746c: str r3, [sp, #0x3c]
00347470: str ip, [sp, #0x20]
00347474: str lr, [sp, #0x28]
00347478: mov r8, r7
0034747c: add sb, sp, #0x48
00347480: mov sl, r5
00347484: mov fp, r6
00347488: mov r0, r4
0034748c: bl #0x80e498
00347490: cmp r0, #0
00347494: addeq r7, r7, #1
00347498: bne #0x347798
0034749c: mov r1, #8
003474a0: mov r0, r4
003474a4: bl #0x80e564
003474a8: ldr r1, [sp, #0xc]
003474ac: mov r2, r7
003474b0: mov r6, r0
003474b4: mov r0, sb
003474b8: bl #0x3407a0
003474bc: mov r0, sb
003474c0: mov r1, #0
003474c4: bl #0x33fdc0
003474c8: subs r5, r0, #0
003474cc: beq #0x3474e0
003474d0: ldrb r3, [r5, #0xf8]
003474d4: cmp r3, r6
003474d8: moveq r3, #0
003474dc: beq #0x347524
003474e0: sub r6, r6, #1
003474e4: cmp r6, #3
003474e8: addls pc, pc, r6, lsl #2
003474ec: b #0x3476ec
003474f0: b #0x3476cc
003474f4: b #0x34774c
003474f8: b #0x3476ac
003474fc: b #0x347500
00347500: mov r1, #0
00347504: movw r0, #0x718
00347508: bl #0x310570
0034750c: mov r3, #1
00347510: mov r1, #0x14
00347514: mov r2, #0
00347518: mov r5, r0
0034751c: bl #0x398f44
00347520: mov r3, #1
00347524: ldr ip, [r5, #0x104]
00347528: cmp ip, #0
0034752c: beq #0x347558
00347530: cmp r3, #0
00347534: bne #0x3477ac
00347538: ldr lr, [sp, #0x14]
0034753c: cmp lr, #0
00347540: bne #0x347800
00347544: mov r0, ip
00347548: ldr r3, [ip]
0034754c: mov r1, r4
00347550: mov lr, pc
00347554: ldr pc, [r3, #0x1c]
00347558: add r8, r8, #1
0034755c: cmp sl, r8
00347560: bne #0x347488
00347564: mov r6, fp
00347568: ldr fp, [sp, #0x24]
0034756c: ldr r0, [sp, #0x20]
00347570: mov r1, fp
00347574: bl #0x345d04
00347578: ldr r3, [r0, #0x10]
0034757c: mov r5, r0
00347580: cmp r3, #0
00347584: bne #0x347bb8
00347588: ldr r0, [sp, #0x28]
0034758c: mov r1, fp
00347590: bl #0x345d04
00347594: ldr r3, [r0, #0x10]
00347598: mov r5, r0
0034759c: cmp r3, #0
003475a0: bne #0x347c94
003475a4: ldr r5, [sp, #0x74]
003475a8: add r7, sp, #0x6c
003475ac: ldr r8, [sp, #0x20]
003475b0: b #0x3475d4
003475b4: mov r1, fp
003475b8: mov r0, r8
003475bc: bl #0x345d04
003475c0: add r2, r5, #8
003475c4: mov r1, r0
003475c8: mov r0, r7
003475cc: bl #0x344994
003475d0: ldr r5, [r5]
003475d4: cmp r5, r6
003475d8: bne #0x3475b4
003475dc: mov r0, r4
003475e0: bl #0x80e498
003475e4: cmp r0, #0
003475e8: bne #0x347c5c
003475ec: ldr r0, [sp, #0x1c]
003475f0: cmp r0, #0
003475f4: bne #0x347c04
003475f8: mov r0, r4
003475fc: bl #0x80e498
00347600: cmp r0, #0
00347604: beq #0x347680
00347608: ldr r0, [sp, #0x14]
0034760c: cmp r0, #0
00347610: beq #0x347680
00347614: mov r0, r4
00347618: mov r1, #0x10
0034761c: bl #0x80e630
00347620: subs r7, r0, #0
00347624: ble #0x347680
00347628: ldr r1, [sp, #0xc]
0034762c: mov r5, #0
00347630: add sl, sp, #0x64
00347634: add r8, r1, #0x164
00347638: add sb, sp, #0x8e
0034763c: str r6, [sp, #0xc]
00347640: mov r1, #0x10
00347644: mov r0, r4
00347648: bl #0x80e630
0034764c: mov r1, fp
00347650: mov r6, r0
00347654: mov r0, r8
00347658: bl #0x345d04
0034765c: add r5, r5, #1
00347660: mov r1, r0
00347664: mov r2, sb
00347668: mov r0, sl
0034766c: strh r6, [sp, #0x8e]
00347670: bl #0x344994
00347674: cmp r7, r5
00347678: bne #0x347640
0034767c: ldr r6, [sp, #0xc]
00347680: ldr r0, [sp, #0x74]
00347684: cmp r0, r6
00347688: bne #0x347694
0034768c: b #0x3473b0
00347690: mov r0, r4
00347694: ldr r4, [r0]
00347698: mov r1, #0xc
0034769c: bl #0x708f00
003476a0: cmp r4, r6
003476a4: bne #0x347690
003476a8: b #0x3473b0
003476ac: mov r1, #0
003476b0: mov r0, #0x6d0
003476b4: bl #0x310570
003476b8: mov r1, #2
003476bc: mov r5, r0
003476c0: bl #0x3e8274
003476c4: mov r3, #1
003476c8: b #0x347524
003476cc: mov r1, #0
003476d0: movw r0, #0x1f90
003476d4: bl #0x310570
003476d8: mov r1, #0
003476dc: mov r5, r0
003476e0: bl #0x3aa1b4
003476e4: mov r3, #1
003476e8: b #0x347524
003476ec: ldr r1, [sp, #0x10]
003476f0: ldr r0, [sp, #0x2c]
003476f4: ldr r3, [r1, r0]
003476f8: ldr r6, [r3]
003476fc: cmp r6, #2
00347700: moveq r3, #0
00347704: streq r3, [r3]
00347708: moveq r3, #1
0034770c: beq #0x347524
00347710: cmp r6, #1
00347714: movne r3, #1
00347718: bne #0x347524
0034771c: ldr r3, [sp, #0x10]
00347720: ldr r2, [sp, #0x30]
00347724: mov ip, #0x860
00347728: ldr r1, [sp, #0x34]
0034772c: ldr r0, [r3, r2]
00347730: ldr r3, [sp, #0x3c]
00347734: ldr r2, [sp, #0x38]
00347738: add r0, r0, #0xa8
0034773c: str ip, [sp]
00347740: bl #0x30e004
00347744: mov r3, r6
00347748: b #0x347524
0034774c: mov r1, #0
00347750: mov r0, #0x6f0
00347754: bl #0x310570
00347758: mov r1, #0x14
0034775c: mov r5, r0
00347760: bl #0x3a06dc
00347764: mov r3, #1
00347768: b #0x347524
0034776c: ldr r0, [sp, #0x74]
00347770: cmp r0, r6
00347774: bne #0x347780
00347778: b #0x3473b0
0034777c: mov r0, r4
00347780: ldr r4, [r0]
00347784: mov r1, #0xc
00347788: bl #0x708f00
0034778c: cmp r4, r6
00347790: bne #0x34777c
00347794: b #0x3473b0
00347798: mov r0, r4
0034779c: mov r1, #0x10
003477a0: bl #0x80e630
003477a4: mov r7, r0
003477a8: b #0x34749c
003477ac: mov r0, ip
003477b0: ldr r3, [ip]
003477b4: mov r1, r4
003477b8: mov lr, pc
003477bc: ldr pc, [r3, #0x1c]
003477c0: ldr r2, [sp, #0x14]
003477c4: cmp r2, #0
003477c8: beq #0x3477ec
003477cc: mov r0, fp
003477d0: bl #0x343148
003477d4: strh r7, [r0, #8]
003477d8: ldr r3, [sp, #0x78]
003477dc: str fp, [r0]
003477e0: str r3, [r0, #4]
003477e4: str r0, [r3]
003477e8: str r0, [sp, #0x78]
003477ec: mov r0, r5
003477f0: ldr r3, [r5]
003477f4: mov lr, pc
003477f8: ldr pc, [r3, #4]
003477fc: b #0x347558
00347800: mov r1, r4
00347804: ldr r2, [sp, #0x44]
00347808: mov r0, ip
0034780c: ldr ip, [ip]
00347810: mov lr, pc
00347814: ldr pc, [ip, #0x14]
00347818: mov r6, r0
0034781c: bl #0x7fd794
00347820: bl #0x7fd5b4
00347824: cmp r0, #0
00347828: bne #0x347974
0034782c: ldr r3, [r5]
00347830: mov r0, r5
00347834: ldr r1, [sp, #0x1c]
00347838: mov lr, pc
0034783c: ldr pc, [r3, #0x50]
00347840: ldr r2, [sp, #0x40]
00347844: ldr ip, [sp, #0x10]
00347848: ldr r3, [ip, r2]
0034784c: ldr r2, [sp, #0x44]
00347850: ldr r3, [r3]
00347854: str r2, [r5, #0x110]
00347858: mov r2, #0
0034785c: cmp r6, r3
00347860: str r2, [r5, #0x114]
00347864: beq #0x3478ec
00347868: ldr r0, [sp, #0x20]
0034786c: ldr r1, [sp, #0x24]
00347870: bl #0x345d04
00347874: ldr r3, [r0, #4]
00347878: cmp r3, #0
0034787c: beq #0x347cf8
00347880: mov r1, r0
00347884: sxth ip, r7
00347888: b #0x347890
0034788c: mov r3, r2
00347890: ldrsh r2, [r3, #0x10]
00347894: cmp r2, ip
00347898: ldrlt r2, [r3, #0xc]
0034789c: ldrge r2, [r3, #8]
003478a0: movlt r3, r1
003478a4: mov r1, r3
003478a8: cmp r2, #0
003478ac: bne #0x34788c
003478b0: cmp r0, r3
003478b4: beq #0x3478ec
003478b8: ldrsh r2, [r3, #0x10]
003478bc: cmp r2, ip
003478c0: bgt #0x347cf8
003478c4: cmp r0, r3
003478c8: beq #0x3478ec
003478cc: mov r0, fp
003478d0: bl #0x343148
003478d4: strh r7, [r0, #8]
003478d8: ldr r3, [sp, #0x78]
003478dc: str fp, [r0]
003478e0: str r3, [r0, #4]
003478e4: str r0, [r3]
003478e8: str r0, [sp, #0x78]
003478ec: ldr r0, [sp, #0x28]
003478f0: ldr r1, [sp, #0x24]
003478f4: bl #0x345d04
003478f8: ldr r3, [r0, #4]
003478fc: cmp r3, #0
00347900: beq #0x347bb0
00347904: mov r1, r0
00347908: sxth ip, r7
0034790c: b #0x347914
00347910: mov r3, r2
00347914: ldrsh r2, [r3, #0x10]
00347918: cmp r2, ip
0034791c: ldrlt r2, [r3, #0xc]
00347920: ldrge r2, [r3, #8]
00347924: movlt r3, r1
00347928: mov r1, r3
0034792c: cmp r2, #0
00347930: bne #0x347910
00347934: cmp r0, r3
00347938: beq #0x347558
0034793c: ldrsh r2, [r3, #0x10]
00347940: cmp r2, ip
00347944: bgt #0x347bb0
00347948: cmp r0, r3
0034794c: beq #0x347558
00347950: mov r0, fp
00347954: bl #0x343148
00347958: strh r7, [r0, #8]
0034795c: ldr r3, [sp, #0x78]
00347960: str fp, [r0]
00347964: str r3, [r0, #4]
00347968: str r0, [r3]
0034796c: str r0, [sp, #0x78]
00347970: b #0x347558
00347974: ldr r3, [r5, #0x110]
00347978: cmn r3, #1
0034797c: beq #0x347d00
00347980: ldr r2, [sp, #0x44]
00347984: cmp r2, r3
00347988: beq #0x34782c
0034798c: ldr r3, [r5]
00347990: mov r0, r5
00347994: mov lr, pc
00347998: ldr pc, [r3, #0x24]
0034799c: cmp r0, #0
003479a0: beq #0x347558
003479a4: ldr r3, [r5]
003479a8: mov r0, r5
003479ac: mov lr, pc
003479b0: ldr pc, [r3, #0x28]
003479b4: cmp r0, #0
003479b8: beq #0x347558
003479bc: ldr r1, [sp, #0x10]
003479c0: ldr r0, [sp, #0x18]
003479c4: mov r2, #1
003479c8: ldr r3, [r1, r0]
003479cc: mov r1, #0
003479d0: ldr r0, [r3, #0x40]
003479d4: bl #0x36e478
003479d8: ldr r3, [r0, #0x660]
003479dc: ldr r3, [r3, #0x108]
003479e0: cmp r7, r3
003479e4: bne #0x34782c
003479e8: b #0x347558
003479ec: mov r0, r4
003479f0: mov r1, #8
003479f4: bl #0x80e564
003479f8: mov r8, r0
003479fc: ldr r0, [sp, #0xc]
00347a00: mov sl, r8
00347a04: ldr ip, [r0, #0xb4]
00347a08: add r5, r0, #0xb0
00347a0c: cmp ip, #0
00347a10: beq #0x347ba8
00347a14: ldr r0, [sp, #0x44]
00347a18: mov r1, r5
00347a1c: mov r3, ip
00347a20: b #0x347a28
00347a24: mov r3, r2
00347a28: ldr r2, [r3, #0x10]
00347a2c: cmp r2, r0
00347a30: ldrlt r2, [r3, #0xc]
00347a34: ldrge r2, [r3, #8]
00347a38: movlt r3, r1
00347a3c: mov r1, r3
00347a40: cmp r2, #0
00347a44: bne #0x347a24
00347a48: cmp r5, r3
00347a4c: beq #0x347d50
00347a50: ldr r2, [r3, #0x10]
00347a54: cmp r2, r0
00347a58: bgt #0x347ba8
00347a5c: cmp r5, r3
00347a60: beq #0x347d50
00347a64: cmp ip, #0
00347a68: beq #0x347d44
00347a6c: ldr lr, [sp, #0x44]
00347a70: mov r2, r5
00347a74: b #0x347a7c
00347a78: mov ip, r3
00347a7c: ldr r3, [ip, #0x10]
00347a80: cmp r3, lr
00347a84: ldrlt r3, [ip, #0xc]
00347a88: ldrge r3, [ip, #8]
00347a8c: movlt ip, r2
00347a90: mov r2, ip
00347a94: cmp r3, #0
00347a98: bne #0x347a78
00347a9c: cmp r5, ip
00347aa0: beq #0x347ab4
00347aa4: ldr r2, [ip, #0x10]
00347aa8: mov r3, ip
00347aac: cmp r2, lr
00347ab0: ble #0x347adc
00347ab4: add r3, sp, #0x5c
00347ab8: str ip, [sp, #0x84]
00347abc: add r0, sp, #0x88
00347ac0: mov ip, #0
00347ac4: mov r1, r5
00347ac8: add r2, sp, #0x84
00347acc: str lr, [sp, #0x5c]
00347ad0: strh ip, [sp, #0x60]
00347ad4: bl #0x34371c
00347ad8: ldr r3, [sp, #0x88]
00347adc: ldrsh r3, [r3, #0x14]
00347ae0: sxth sb, r8
00347ae4: cmp r3, sb
00347ae8: blt #0x347cb0
00347aec: ldr lr, [sp, #0xc]
00347af0: ldr ip, [lr, #0xb4]
00347af4: cmp ip, #0
00347af8: beq #0x347d38
00347afc: ldr lr, [sp, #0x44]
00347b00: mov r2, r5
00347b04: b #0x347b0c
00347b08: mov ip, r3
00347b0c: ldr r3, [ip, #0x10]
00347b10: cmp r3, lr
00347b14: ldrlt r3, [ip, #0xc]
00347b18: ldrge r3, [ip, #8]
00347b1c: movlt ip, r2
00347b20: mov r2, ip
00347b24: cmp r3, #0
00347b28: bne #0x347b08
00347b2c: cmp r5, ip
00347b30: beq #0x347b44
00347b34: ldr r2, [ip, #0x10]
00347b38: mov r3, ip
00347b3c: cmp r2, lr
00347b40: ble #0x347b6c
00347b44: add r3, sp, #0x54
00347b48: str ip, [sp, #0x7c]
00347b4c: mov r1, r5
00347b50: mov ip, #0
00347b54: add r0, sp, #0x80
00347b58: add r2, sp, #0x7c
00347b5c: str lr, [sp, #0x54]
00347b60: strh ip, [sp, #0x58]
00347b64: bl #0x34371c
00347b68: ldr r3, [sp, #0x80]
00347b6c: ldrsh r3, [r3, #0x14]
00347b70: cmp r3, sb
00347b74: movne lr, #1
00347b78: strne lr, [sp, #0x1c]
00347b7c: bne #0x3473dc
00347b80: ldr r1, [sp, #0xc]
00347b84: add r0, r1, #0xe0
00347b88: add r1, sp, #0x44
00347b8c: bl #0x343a90
00347b90: ldrh r3, [r0]
00347b94: mov r2, #1
00347b98: str r2, [sp, #0x1c]
00347b9c: add r3, r3, r2
00347ba0: strh r3, [r0]
00347ba4: b #0x3473dc
00347ba8: mov r3, r5
00347bac: b #0x347a5c
00347bb0: mov r3, r0
00347bb4: b #0x347948
00347bb8: ldr r1, [r0, #4]
00347bbc: bl #0x345ccc
00347bc0: mov r3, #0
00347bc4: str r3, [r5, #0x10]
00347bc8: stmib r5, {r3, r5}
00347bcc: str r5, [r5, #0xc]
00347bd0: b #0x347588
00347bd4: ldr ip, [sp, #0xc]
00347bd8: add fp, sp, #0x44
00347bdc: mov r1, fp
00347be0: add r0, ip, #0xb0
00347be4: bl #0x343a90
00347be8: ldrsh r3, [r0]
00347bec: sxth sl, sl
00347bf0: cmp r3, sl
00347bf4: movne sl, #0
00347bf8: moveq sl, #1
00347bfc: str sl, [sp, #0x14]
00347c00: b #0x347414
00347c04: ldr r1, [sp, #0xc]
00347c08: add r5, r1, #0xc8
00347c0c: mov r0, r5
00347c10: mov r1, fp
00347c14: bl #0x343a90
00347c18: ldrsh r3, [r0]
00347c1c: cmp r3, #0
00347c20: blt #0x3475f8
00347c24: ldr r2, [sp, #0xc]
00347c28: mov r1, fp
00347c2c: add r0, r2, #0xe0
00347c30: bl #0x343a90
00347c34: mov r1, fp
00347c38: ldrh r7, [r0]
00347c3c: mov r0, r5
00347c40: bl #0x343a90
00347c44: ldrh r3, [r0]
00347c48: cmp r7, r3
00347c4c: ldreq ip, [sp, #0xc]
00347c50: moveq r3, #1
00347c54: strbeq r3, [ip, #0x160]
00347c58: b #0x3475f8
00347c5c: ldr lr, [sp, #0xc]
00347c60: mov r1, #8
00347c64: mov r0, r4
00347c68: add r5, lr, #0xc8
00347c6c: bl #0x80e564
00347c70: mov r1, fp
00347c74: mov r7, r0
00347c78: mov r0, r5
00347c7c: bl #0x343a90
00347c80: mov r0, r5
00347c84: mov r1, fp
00347c88: bl #0x343a90
00347c8c: strh r7, [r0]
00347c90: b #0x3475ec
00347c94: ldr r1, [r0, #4]
00347c98: bl #0x345ccc
00347c9c: mov r3, #0
00347ca0: str r3, [r5, #0x10]
00347ca4: stmib r5, {r3, r5}
00347ca8: str r5, [r5, #0xc]
00347cac: b #0x3475a4
00347cb0: add fp, sp, #0x44
00347cb4: mov r1, fp
00347cb8: mov r0, r5
00347cbc: bl #0x343a90
00347cc0: strh r8, [r0]
00347cc4: ldr lr, [sp, #0xc]
00347cc8: mov r1, fp
00347ccc: add r0, lr, #0xe0
00347cd0: bl #0x343a90
00347cd4: mov r1, #0
00347cd8: strh r1, [r0]
00347cdc: ldr r2, [sp, #0xc]
00347ce0: mov r1, fp
00347ce4: add r0, r2, #0xc8
00347ce8: bl #0x343a90
00347cec: mvn r3, #0
00347cf0: strh r3, [r0]
00347cf4: b #0x347aec
00347cf8: mov r3, r0
00347cfc: b #0x3478c4
00347d00: ldr r0, [sp, #0xc]
00347d04: mov r1, r5
00347d08: bl #0x340848
00347d0c: cmp r0, #0
00347d10: beq #0x34782c
00347d14: ldr r3, [r5, #0x110]
00347d18: b #0x347980
00347d1c: ldr lr, [sp, #0xc]
00347d20: ldr r0, [sp, #0xc]
00347d24: add lr, lr, #0x17c
00347d28: add r0, r0, #0x194
00347d2c: str lr, [sp, #0x20]
00347d30: str r0, [sp, #0x28]
00347d34: b #0x34756c
00347d38: ldr lr, [sp, #0x44]
00347d3c: mov ip, r5
00347d40: b #0x347b2c
00347d44: ldr lr, [sp, #0x44]
00347d48: mov ip, r5
00347d4c: b #0x347a9c
00347d50: add r1, sp, #0x44
00347d54: mov r0, r5
00347d58: bl #0x343a90
00347d5c: mvn r1, #0
00347d60: strh r1, [r0]
00347d64: ldr r2, [sp, #0xc]
00347d68: ldr ip, [r2, #0xb4]
00347d6c: b #0x347a64
00347d70: rsbeq sp, r4, r4, lsr #14
00347d74: strdeq r3, r4, [r0], -r4
00347d78: andeq r2, r0, ip, asr lr
00347d7c: subseq r6, r7, r0, lsr #31
00347d80: andeq r3, r0, r0, asr #19
00347d84: andeq r1, r0, r0, asr #19
00347d88: subseq r7, r7, ip, lsl r1
00347d8c: subseq r8, r7, r8, lsr lr

# 0x347d90 _ZN13ObjectManager15sReadPacketDataEiiR12NetBitStream
00347d90: push {r4, r5, r6, lr}
00347d94: mov r6, r0
00347d98: mov r0, r2
00347d9c: mov r4, r2
00347da0: mov r5, r1
00347da4: bl #0x80e498
00347da8: ldr r3, [pc, #0x34]
00347dac: cmp r0, #0
00347db0: add r3, pc, r3
00347db4: beq #0x347de0
00347db8: ldr r2, [pc, #0x28]
00347dbc: ldr r3, [r3, r2]
00347dc0: ldr r0, [r3, #0x38]
00347dc4: cmp r0, #0
00347dc8: beq #0x347de0
00347dcc: mov r1, r6
00347dd0: mov r2, r5
00347dd4: mov r3, r4
00347dd8: pop {r4, r5, r6, lr}
00347ddc: b #0x34735c
00347de0: pop {r4, r5, r6, pc}
00347de4: rsbeq ip, r4, r0, ror #25
00347de8: strdeq r3, r4, [r0], -r4

# 0x347dec _ZN13ObjectManager16IsOnlineDeferredEP10ObjectBase
00347dec: push {r4, r5, r6, r7, lr}
00347df0: sub sp, sp, #0x14
00347df4: mov r5, r1
00347df8: mov r7, r0
00347dfc: bl #0x800f8c
00347e00: ldr r3, [r0]
00347e04: mov r1, r0
00347e08: add r0, sp, #4
00347e0c: mov lr, pc
00347e10: ldr pc, [r3, #0x88]
00347e14: ldr r6, [sp, #4]
00347e18: ldr r3, [sp, #8]
00347e1c: mov r0, r6
00347e20: cmp r6, r3
00347e24: beq #0x347e90
00347e28: add r7, r7, #0x108
00347e2c: mov r1, r6
00347e30: mov r0, r7
00347e34: bl #0x3452ac
00347e38: mov r1, r6
00347e3c: ldr r4, [r0]
00347e40: mov r0, r7
00347e44: bl #0x3452ac
00347e48: cmp r4, r0
00347e4c: beq #0x347e68
00347e50: ldr r3, [r4, #8]
00347e54: cmp r5, r3
00347e58: beq #0x347e68
00347e5c: ldr r4, [r4]
00347e60: cmp r0, r4
00347e64: bne #0x347e50
00347e68: mov r0, r7
00347e6c: mov r1, r6
00347e70: bl #0x3452ac
00347e74: cmp r4, r0
00347e78: bne #0x347ec0
00347e7c: ldr r3, [sp, #8]
00347e80: add r6, r6, #4
00347e84: cmp r6, r3
00347e88: bne #0x347e2c
00347e8c: ldr r0, [sp, #4]
00347e90: mov r4, #0
00347e94: cmp r0, #0
00347e98: beq #0x347eb4
00347e9c: ldr r1, [sp, #0xc]
00347ea0: rsb r1, r0, r1
00347ea4: bic r1, r1, #3
00347ea8: cmp r1, #0x80
00347eac: bhi #0x347ecc
00347eb0: bl #0x708f00
00347eb4: mov r0, r4
00347eb8: add sp, sp, #0x14
00347ebc: pop {r4, r5, r6, r7, pc}
00347ec0: mov r4, #1
00347ec4: ldr r0, [sp, #4]
00347ec8: b #0x347e94
00347ecc: bl #0x310440
00347ed0: b #0x347eb4

# 0x347fd0 _ZN13ObjectManager15ForceFullUpdateEv
00347fd0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00347fd4: sub sp, sp, #0xa4
00347fd8: mov r5, r0
00347fdc: bl #0x7fd794
00347fe0: ldrb r3, [r0, #5]
00347fe4: ldr r6, [pc, #0x650]
00347fe8: cmp r3, #0
00347fec: add r6, pc, r6
00347ff0: beq #0x3482f0
00347ff4: ldr r3, [r5, #0x118]
00347ff8: cmp r3, #0
00347ffc: bne #0x3483a8
00348000: bl #0x320e98
00348004: ldr r3, [r0, #0x34]
00348008: sub r3, r3, #3
0034800c: cmp r3, #1
00348010: bls #0x3483e0
00348014: bl #0x800f8c
00348018: mov r1, r0
0034801c: ldr r3, [r0]
00348020: add r0, sp, #0x20
00348024: mov lr, pc
00348028: ldr pc, [r3, #0x88]
0034802c: ldr r4, [sp, #0x20]
00348030: ldr r0, [sp, #0x24]
00348034: cmp r4, r0
00348038: beq #0x3482d0
0034803c: ldr r3, [pc, #0x5fc]
00348040: add ip, sp, #0x64
00348044: add r2, sp, #0x2c
00348048: ldr fp, [r6, r3]
0034804c: add r3, sp, #0x68
00348050: stm sp, {r3, ip}
00348054: add r3, sp, #0x70
00348058: add ip, sp, #0x6c
0034805c: str r2, [sp, #8]
00348060: str r3, [sp, #0xc]
00348064: str ip, [sp, #0x10]
00348068: add r2, sp, #0x34
0034806c: add r3, sp, #0x80
00348070: add ip, sp, #0x7c
00348074: add r4, r4, #4
00348078: add sb, r5, #0x128
0034807c: add r7, r5, #0xb0
00348080: add r8, r5, #0xe0
00348084: add sl, r5, #0xc8
00348088: str r2, [sp, #0x14]
0034808c: str r3, [sp, #0x18]
00348090: str ip, [sp, #0x1c]
00348094: mov r6, r5
00348098: ldr r0, [fp, #0x40]
0034809c: bl #0x36e09c
003480a0: ldr r2, [r4, #-4]
003480a4: ldr r3, [r0, #0x1a0]
003480a8: cmp r2, r3
003480ac: beq #0x3482b8
003480b0: mov r0, sb
003480b4: bl #0x3441d8
003480b8: ldr r3, [r4, #-4]
003480bc: str r3, [r0, #8]
003480c0: ldr r3, [r6, #0x12c]
003480c4: str sb, [r0]
003480c8: str r3, [r0, #4]
003480cc: str r0, [r3]
003480d0: ldr ip, [r6, #0xb4]
003480d4: str r0, [r6, #0x12c]
003480d8: cmp ip, #0
003480dc: beq #0x348390
003480e0: ldr r5, [r4, #-4]
003480e4: mov r1, r7
003480e8: mov r3, ip
003480ec: b #0x3480f4
003480f0: mov r3, r2
003480f4: ldr r2, [r3, #0x10]
003480f8: cmp r2, r5
003480fc: ldrlt r2, [r3, #0xc]
00348100: ldrge r2, [r3, #8]
00348104: movlt r3, r1
00348108: mov r1, r3
0034810c: cmp r2, #0
00348110: bne #0x3480f0
00348114: cmp r7, r3
00348118: beq #0x3482f8
0034811c: ldr r2, [r3, #0x10]
00348120: cmp r2, r5
00348124: movgt r3, r7
00348128: cmp r7, r3
0034812c: beq #0x3482f8
00348130: cmp ip, #0
00348134: movne r2, r7
00348138: bne #0x348144
0034813c: b #0x348600
00348140: mov ip, r3
00348144: ldr r3, [ip, #0x10]
00348148: cmp r5, r3
0034814c: ldrgt r3, [ip, #0xc]
00348150: ldrle r3, [ip, #8]
00348154: movgt ip, r2
00348158: mov r2, ip
0034815c: cmp r3, #0
00348160: bne #0x348140
00348164: cmp r7, ip
00348168: beq #0x34817c
0034816c: ldr r2, [ip, #0x10]
00348170: mov r3, ip
00348174: cmp r5, r2
00348178: bge #0x3481a4
0034817c: add r3, sp, #0x3c
00348180: str ip, [sp, #0x74]
00348184: add r0, sp, #0x78
00348188: mov ip, #0
0034818c: mov r1, r7
00348190: add r2, sp, #0x74
00348194: str r5, [sp, #0x3c]
00348198: strh ip, [sp, #0x40]
0034819c: bl #0x34371c
003481a0: ldr r3, [sp, #0x78]
003481a4: ldrh r2, [r3, #0x14]
003481a8: add r2, r2, #1
003481ac: strh r2, [r3, #0x14]
003481b0: ldr ip, [r6, #0xe4]
003481b4: cmp ip, #0
003481b8: beq #0x348384
003481bc: ldr r5, [r4, #-4]
003481c0: mov r2, r8
003481c4: b #0x3481cc
003481c8: mov ip, r3
003481cc: ldr r3, [ip, #0x10]
003481d0: cmp r5, r3
003481d4: ldrgt r3, [ip, #0xc]
003481d8: ldrle r3, [ip, #8]
003481dc: movgt ip, r2
003481e0: mov r2, ip
003481e4: cmp r3, #0
003481e8: bne #0x3481c8
003481ec: cmp r8, ip
003481f0: beq #0x348204
003481f4: ldr r2, [ip, #0x10]
003481f8: mov r3, ip
003481fc: cmp r2, r5
00348200: ble #0x34822c
00348204: ldr r3, [sp, #0x14]
00348208: str ip, [sp, #0x6c]
0034820c: ldr r0, [sp, #0xc]
00348210: mov ip, #0
00348214: mov r1, r8
00348218: ldr r2, [sp, #0x10]
0034821c: str r5, [sp, #0x34]
00348220: strh ip, [sp, #0x38]
00348224: bl #0x34371c
00348228: ldr r3, [sp, #0x70]
0034822c: mov r2, #0
00348230: strh r2, [r3, #0x14]
00348234: ldr ip, [r6, #0xcc]
00348238: cmp ip, #0
0034823c: beq #0x34839c
00348240: ldr r5, [r4, #-4]
00348244: mov r2, sl
00348248: b #0x348250
0034824c: mov ip, r3
00348250: ldr r3, [ip, #0x10]
00348254: cmp r3, r5
00348258: ldrlt r3, [ip, #0xc]
0034825c: ldrge r3, [ip, #8]
00348260: movlt ip, r2
00348264: mov r2, ip
00348268: cmp r3, #0
0034826c: bne #0x34824c
00348270: cmp sl, ip
00348274: beq #0x348288
00348278: ldr r2, [ip, #0x10]
0034827c: mov r3, ip
00348280: cmp r5, r2
00348284: bge #0x3482b0
00348288: ldr r3, [sp, #8]
0034828c: str ip, [sp, #0x64]
00348290: ldr r0, [sp]
00348294: mov ip, #0
00348298: mov r1, sl
0034829c: ldr r2, [sp, #4]
003482a0: str r5, [sp, #0x2c]
003482a4: strh ip, [sp, #0x30]
003482a8: bl #0x34371c
003482ac: ldr r3, [sp, #0x68]
003482b0: mvn r2, #0
003482b4: strh r2, [r3, #0x14]
003482b8: ldr r3, [sp, #0x24]
003482bc: mov r2, r4
003482c0: add r4, r4, #4
003482c4: cmp r2, r3
003482c8: bne #0x348098
003482cc: ldr r0, [sp, #0x20]
003482d0: cmp r0, #0
003482d4: beq #0x3482f0
003482d8: ldr r1, [sp, #0x28]
003482dc: rsb r1, r0, r1
003482e0: bic r1, r1, #3
003482e4: cmp r1, #0x80
003482e8: bhi #0x348608
003482ec: bl #0x708f00
003482f0: add sp, sp, #0xa4
003482f4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003482f8: cmp ip, #0
003482fc: moveq ip, r7
00348300: beq #0x348330
00348304: mov r2, r7
00348308: b #0x348310
0034830c: mov ip, r3
00348310: ldr r3, [ip, #0x10]
00348314: cmp r5, r3
00348318: ldrgt r3, [ip, #0xc]
0034831c: ldrle r3, [ip, #8]
00348320: movgt ip, r2
00348324: mov r2, ip
00348328: cmp r3, #0
0034832c: bne #0x34830c
00348330: cmp r7, ip
00348334: beq #0x348348
00348338: ldr r2, [ip, #0x10]
0034833c: mov r3, ip
00348340: cmp r5, r2
00348344: bge #0x348370
00348348: add r3, sp, #0x44
0034834c: str ip, [sp, #0x7c]
00348350: ldr r0, [sp, #0x18]
00348354: mov ip, #0
00348358: mov r1, r7
0034835c: ldr r2, [sp, #0x1c]
00348360: str r5, [sp, #0x44]
00348364: strh ip, [sp, #0x48]
00348368: bl #0x34371c
0034836c: ldr r3, [sp, #0x80]
00348370: mov r2, #0
00348374: strh r2, [r3, #0x14]
00348378: ldr ip, [r6, #0xe4]
0034837c: cmp ip, #0
00348380: bne #0x3481bc
00348384: ldr r5, [r4, #-4]
00348388: mov ip, r8
0034838c: b #0x3481ec
00348390: ldr r5, [r4, #-4]
00348394: mov r3, r7
00348398: b #0x348128
0034839c: ldr r5, [r4, #-4]
003483a0: mov ip, sl
003483a4: b #0x348270
003483a8: add r4, r5, #0x108
003483ac: mov r0, r4
003483b0: ldr r1, [r5, #0x10c]
003483b4: bl #0x3458d4
003483b8: mov r3, #0
003483bc: str r3, [r5, #0x118]
003483c0: str r3, [r5, #0x10c]
003483c4: str r4, [r5, #0x114]
003483c8: str r4, [r5, #0x110]
003483cc: bl #0x320e98
003483d0: ldr r3, [r0, #0x34]
003483d4: sub r3, r3, #3
003483d8: cmp r3, #1
003483dc: bhi #0x348014
003483e0: add r4, r5, #0x128
003483e4: mov r0, r4
003483e8: bl #0x3441d8
003483ec: mov r3, #1
003483f0: str r3, [r0, #8]
003483f4: ldr r3, [r5, #0x12c]
003483f8: str r4, [r0]
003483fc: add r1, r5, #0xb0
00348400: str r3, [r0, #4]
00348404: str r0, [r3]
00348408: ldr ip, [r5, #0xb4]
0034840c: str r0, [r5, #0x12c]
00348410: cmp ip, #0
00348414: beq #0x3485f8
00348418: mov r4, r1
0034841c: mov r3, ip
00348420: b #0x348428
00348424: mov r3, r2
00348428: ldr r2, [r3, #0x10]
0034842c: cmp r2, #0
00348430: ldrle r2, [r3, #0xc]
00348434: ldrgt r2, [r3, #8]
00348438: movle r3, r4
0034843c: mov r4, r3
00348440: cmp r2, #0
00348444: bne #0x348424
00348448: cmp r1, r3
0034844c: beq #0x348610
00348450: ldr r2, [r3, #0x10]
00348454: cmp r2, #1
00348458: bgt #0x3485f8
0034845c: cmp r1, r3
00348460: beq #0x348610
00348464: cmp ip, #0
00348468: movne r2, r1
0034846c: bne #0x348478
00348470: b #0x348634
00348474: mov ip, r3
00348478: ldr r3, [ip, #0x10]
0034847c: cmp r3, #0
00348480: ldrle r3, [ip, #0xc]
00348484: ldrgt r3, [ip, #8]
00348488: movle ip, r2
0034848c: mov r2, ip
00348490: cmp r3, #0
00348494: bne #0x348474
00348498: cmp r1, ip
0034849c: beq #0x3484b0
003484a0: ldr r2, [ip, #0x10]
003484a4: mov r3, ip
003484a8: cmp r2, #1
003484ac: ble #0x3484d8
003484b0: add r3, sp, #0x5c
003484b4: mov lr, #1
003484b8: str ip, [sp, #0x94]
003484bc: add r0, sp, #0x98
003484c0: mov ip, #0
003484c4: add r2, sp, #0x94
003484c8: str lr, [sp, #0x5c]
003484cc: strh ip, [sp, #0x60]
003484d0: bl #0x34371c
003484d4: ldr r3, [sp, #0x98]
003484d8: ldrh r2, [r3, #0x14]
003484dc: add r2, r2, #1
003484e0: strh r2, [r3, #0x14]
003484e4: ldr ip, [r5, #0xe4]
003484e8: add r1, r5, #0xe0
003484ec: cmp ip, #0
003484f0: moveq ip, r1
003484f4: beq #0x348524
003484f8: mov r2, r1
003484fc: b #0x348504
00348500: mov ip, r3
00348504: ldr r3, [ip, #0x10]
00348508: cmp r3, #0
0034850c: ldrle r3, [ip, #0xc]
00348510: ldrgt r3, [ip, #8]
00348514: movle ip, r2
00348518: mov r2, ip
0034851c: cmp r3, #0
00348520: bne #0x348500
00348524: cmp r1, ip
00348528: beq #0x34853c
0034852c: ldr r2, [ip, #0x10]
00348530: mov r3, ip
00348534: cmp r2, #1
00348538: ble #0x348564
0034853c: add r3, sp, #0x54
00348540: mov lr, #1
00348544: str ip, [sp, #0x8c]
00348548: add r0, sp, #0x90
0034854c: mov ip, #0
00348550: add r2, sp, #0x8c
00348554: str lr, [sp, #0x54]
00348558: strh ip, [sp, #0x58]
0034855c: bl #0x34371c
00348560: ldr r3, [sp, #0x90]
00348564: mov r2, #0
00348568: strh r2, [r3, #0x14]
0034856c: ldr ip, [r5, #0xcc]
00348570: add r1, r5, #0xc8
00348574: cmp ip, #0
00348578: moveq ip, r1
0034857c: beq #0x3485ac
00348580: mov r2, r1
00348584: b #0x34858c
00348588: mov ip, r3
0034858c: ldr r3, [ip, #0x10]
00348590: cmp r3, #0
00348594: ldrle r3, [ip, #0xc]
00348598: ldrgt r3, [ip, #8]
0034859c: movle ip, r2
003485a0: mov r2, ip
003485a4: cmp r3, #0
003485a8: bne #0x348588
003485ac: cmp r1, ip
003485b0: beq #0x3485c4
003485b4: ldr r2, [ip, #0x10]
003485b8: mov r3, ip
003485bc: cmp r2, #1
003485c0: ble #0x3485ec
003485c4: add r3, sp, #0x4c
003485c8: mov lr, #1
003485cc: str ip, [sp, #0x84]
003485d0: add r0, sp, #0x88
003485d4: mov ip, #0
003485d8: add r2, sp, #0x84
003485dc: str lr, [sp, #0x4c]
003485e0: strh ip, [sp, #0x50]
003485e4: bl #0x34371c
003485e8: ldr r3, [sp, #0x88]
003485ec: mvn r2, #0
003485f0: strh r2, [r3, #0x14]
003485f4: b #0x3482f0
003485f8: mov r3, r1
003485fc: b #0x34845c
00348600: mov ip, r7
00348604: b #0x348164
00348608: bl #0x310440
0034860c: b #0x3482f0
00348610: add r3, sp, #0xa0
00348614: mov r2, #1
00348618: str r2, [r3, #-4]!
0034861c: mov r0, r1
00348620: mov r1, r3
00348624: bl #0x343a90
00348628: mov r2, #0
0034862c: strh r2, [r0]
00348630: b #0x3484e4
00348634: mov ip, r1
00348638: b #0x348498
0034863c: rsbeq ip, r4, r4, lsr #21
00348640: strdeq r3, r4, [r0], -r4

# 0x348644 _ZN13ObjectManager17ProcessLostPacketEii
00348644: push {r4, r5, r6, lr}
00348648: ldr r3, [r0, #0x9c]
0034864c: mov r4, r0
00348650: add r5, r0, #0x98
00348654: cmp r3, #0
00348658: beq #0x3486f0
0034865c: mov r6, r5
00348660: mov r0, r3
00348664: b #0x34866c
00348668: mov r0, ip
0034866c: ldr ip, [r0, #0x10]
00348670: cmp ip, r1
00348674: ldrlt ip, [r0, #0xc]
00348678: ldrge ip, [r0, #8]
0034867c: movlt r0, r6
00348680: mov r6, r0
00348684: cmp ip, #0
00348688: bne #0x348668
0034868c: cmp r5, r0
00348690: beq #0x3486f8
00348694: ldr ip, [r0, #0x10]
00348698: cmp ip, r1
0034869c: bgt #0x3486f0
003486a0: cmp r5, r0
003486a4: beq #0x3486f8
003486a8: ldr r1, [r0, #0x14]!
003486ac: cmp r1, r0
003486b0: beq #0x3486f8
003486b4: ldr ip, [r1, #8]
003486b8: cmp ip, r2
003486bc: beq #0x3486d0
003486c0: ldr r1, [r1]
003486c4: cmp r0, r1
003486c8: bne #0x3486b4
003486cc: mov r1, r0
003486d0: cmp r1, r0
003486d4: beq #0x348720
003486d8: ldr r2, [r4, #0xa8]
003486dc: cmp r2, #0
003486e0: bne #0x3486fc
003486e4: mov r0, r4
003486e8: pop {r4, r5, r6, lr}
003486ec: b #0x347fd0
003486f0: mov r0, r5
003486f4: b #0x3486a0
003486f8: pop {r4, r5, r6, pc}
003486fc: mov r1, r3
00348700: mov r0, r5
00348704: bl #0x345b4c
00348708: mov r3, #0
0034870c: str r5, [r4, #0xa4]
00348710: str r3, [r4, #0xa8]
00348714: str r5, [r4, #0xa0]
00348718: str r3, [r4, #0x9c]
0034871c: b #0x3486e4
00348720: pop {r4, r5, r6, pc}

# 0x348724 _ZN13ObjectManager18sProcessLostPacketEii
00348724: ldr r3, [pc, #0x24]
00348728: ldr r2, [pc, #0x24]
0034872c: mov ip, r0
00348730: add r3, pc, r3
00348734: ldr r0, [r3, r2]
00348738: mov r2, r1
0034873c: ldr r0, [r0, #0x38]
00348740: cmp r0, #0
00348744: bxeq lr
00348748: mov r1, ip
0034874c: b #0x348644
00348750: rsbeq ip, r4, r0, ror #6
00348754: strdeq r3, r4, [r0], -r4

# 0x348988 _ZN13ObjectManager6Draw3DEv
00348988: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034898c: ldr r5, [pc, #0x4fc]
00348990: ldr r2, [pc, #0x4fc]
00348994: sub sp, sp, #0x104
00348998: add r5, pc, r5
0034899c: ldr r3, [r5, r2]
003489a0: add r4, sp, #0x1c
003489a4: mov r6, #0
003489a8: ldr r3, [r3]
003489ac: ldr fp, [pc, #0x4e4]
003489b0: mov r7, r0
003489b4: mov r1, r6
003489b8: str r2, [sp, #0x10]
003489bc: mov r0, r4
003489c0: mov r2, #0x40
003489c4: str r3, [sp, #0xfc]
003489c8: bl #0x30e460
003489cc: mov r1, r6
003489d0: mov r2, #0x40
003489d4: mov r0, r4
003489d8: bl #0x30e460
003489dc: ldr r2, [r5, fp]
003489e0: mov r3, #0x3f800000
003489e4: mov r1, #1
003489e8: ldr r2, [r2, #0x10]
003489ec: str r3, [sp, #0x58]
003489f0: str r3, [sp, #0x1c]
003489f4: str r3, [sp, #0x30]
003489f8: str r3, [sp, #0x44]
003489fc: strb r1, [sp, #0x5c]
00348a00: ldr r3, [r2, #0x10]
00348a04: mov r2, r4
00348a08: add r6, r7, #0xc
00348a0c: mov r0, r3
00348a10: ldr r3, [r3]
00348a14: mov lr, pc
00348a18: ldr pc, [r3, #0x6c]
00348a1c: ldr r4, [r7, #0x14]
00348a20: cmp r6, r4
00348a24: beq #0x348a7c
00348a28: ldr r3, [r4, #0x2c]
00348a2c: cmp r3, #0
00348a30: beq #0x348a50
00348a34: ldrb r2, [r3, #0x80]
00348a38: cmp r2, #0
00348a3c: beq #0x348a50
00348a40: mov r0, r3
00348a44: ldr r3, [r3]
00348a48: mov lr, pc
00348a4c: ldr pc, [r3, #0x30]
00348a50: ldr r2, [r4, #0xc]
00348a54: cmp r2, #0
00348a58: bne #0x348a64
00348a5c: b #0x348e3c
00348a60: mov r2, r3
00348a64: ldr r3, [r2, #8]
00348a68: cmp r3, #0
00348a6c: bne #0x348a60
00348a70: mov r4, r2
00348a74: cmp r6, r4
00348a78: bne #0x348a28
00348a7c: ldr r3, [pc, #0x418]
00348a80: add r4, sp, #0xe4
00348a84: ldr r6, [r5, r3]
00348a88: mov r0, r6
00348a8c: bl #0x337888
00348a90: ldr r1, [pc, #0x408]
00348a94: add r2, sp, #0xe0
00348a98: mov r0, r4
00348a9c: add r1, pc, r1
00348aa0: bl #0x3140ec
00348aa4: mov r0, r6
00348aa8: mov r1, r4
00348aac: bl #0x337a88
00348ab0: mov r6, r0
00348ab4: ldr r0, [sp, #0xf8]
00348ab8: cmp r0, r4
00348abc: beq #0x348adc
00348ac0: cmp r0, #0
00348ac4: beq #0x348adc
00348ac8: ldr r1, [sp, #0xe4]
00348acc: rsb r1, r0, r1
00348ad0: cmp r1, #0x80
00348ad4: bhi #0x348e70
00348ad8: bl #0x708f00
00348adc: cmp r6, #0
00348ae0: beq #0x348e1c
00348ae4: ldr r3, [r5, fp]
00348ae8: ldr r3, [r3, #0x10]
00348aec: ldr r6, [r3, #0x10]
00348af0: movw r3, #0xffff
00348af4: ldr r4, [r6, #0xdc]
00348af8: ldrh r2, [r4, #0x2e]
00348afc: cmp r2, r3
00348b00: beq #0x348e78
00348b04: add r3, sp, #0xdc
00348b08: mov r0, r3
00348b0c: str r3, [sp, #0x14]
00348b10: mov r1, r4
00348b14: mov r3, #1
00348b18: bl #0x5dd0e4
00348b1c: ldr r0, [sp, #0xdc]
00348b20: cmp r0, #0
00348b24: moveq r2, #0xff
00348b28: beq #0x348b34
00348b2c: bl #0x5c5d34
00348b30: mov r2, r0
00348b34: mov r3, #0
00348b38: mov r0, r6
00348b3c: ldr r1, [sp, #0x14]
00348b40: bl #0x5ad368
00348b44: mvn r2, #0
00348b48: mov r3, #0
00348b4c: strb r2, [sp, #0xd9]
00348b50: mov r2, #0x7f
00348b54: strb r3, [sp, #0xda]
00348b58: strb r2, [sp, #0xdb]
00348b5c: strb r3, [sp, #0xd8]
00348b60: ldr r4, [r7, #0x24]!
00348b64: add r8, sp, #0x60
00348b68: mov sb, r5
00348b6c: b #0x348bc0
00348b70: ldr r2, [r4, #8]
00348b74: ldr r3, [r6]
00348b78: mov r0, r6
00348b7c: ldr r1, [r2, #0x13c]
00348b80: ldr sl, [r2, #0x12c]
00348b84: ldr r5, [r2, #0x130]
00348b88: ldr lr, [r2, #0x134]
00348b8c: ldr ip, [r2, #0x138]
00348b90: ldr r2, [r2, #0x140]
00348b94: ldr r3, [r3, #0x28]
00348b98: str r1, [sp, #0x70]
00348b9c: str r2, [sp, #0x74]
00348ba0: str sl, [sp, #0x60]
00348ba4: str r5, [sp, #0x64]
00348ba8: str lr, [sp, #0x68]
00348bac: str ip, [sp, #0x6c]
00348bb0: mov r1, r8
00348bb4: ldr r2, [sp, #0xd8]
00348bb8: blx r3
00348bbc: ldr r4, [r4]
00348bc0: cmp r7, r4
00348bc4: bne #0x348b70
00348bc8: ldr r0, [sb, fp]
00348bcc: bl #0x31f594
00348bd0: ldr r3, [r0, #0x128]
00348bd4: mov sl, #0
00348bd8: mvn fp, #0
00348bdc: ldr r3, [r3, #8]
00348be0: add r7, sp, #0xcc
00348be4: mov r5, sb
00348be8: mov r0, r3
00348bec: ldr r3, [r3]
00348bf0: mov lr, pc
00348bf4: ldr pc, [r3, #0x144]
00348bf8: mov ip, #0x7f
00348bfc: mov r8, r0
00348c00: strb ip, [sp, #0xdb]
00348c04: add r3, r8, #0x4c
00348c08: add ip, r8, #0x2c
00348c0c: strb sl, [sp, #0xda]
00348c10: strb sl, [sp, #0xd9]
00348c14: strb fp, [sp, #0xd8]
00348c18: str ip, [sp, #0xc]
00348c1c: str r3, [sp, #8]
00348c20: add sb, r8, #0xc
00348c24: mov r0, r6
00348c28: add r1, r8, #0x6c
00348c2c: ldr r3, [r6]
00348c30: ldr r2, [sp, #0xd8]
00348c34: mov r4, #0
00348c38: mov lr, pc
00348c3c: ldr pc, [r3, #0x28]
00348c40: mov ip, #0x7f
00348c44: mov r3, r7
00348c48: ldr r1, [sp, #8]
00348c4c: ldr r2, [sp, #0xc]
00348c50: mov r0, sb
00348c54: strb ip, [sp, #0xdb]
00348c58: add r6, sp, #0xd8
00348c5c: strb fp, [sp, #0xda]
00348c60: strb sl, [sp, #0xd9]
00348c64: strb sl, [sp, #0xd8]
00348c68: str r4, [sp, #0xcc]
00348c6c: str r4, [sp, #0xd0]
00348c70: str r4, [sp, #0xd4]
00348c74: bl #0x3415d8
00348c78: add r2, r8, #0x3c
00348c7c: mov r0, r7
00348c80: str r2, [sp, #4]
00348c84: add r7, sp, #0xc0
00348c88: mov r1, r6
00348c8c: mov r2, #0x64
00348c90: bl #0x340154
00348c94: mov r3, r7
00348c98: ldr r2, [sp, #4]
00348c9c: ldr r1, [sp, #8]
00348ca0: mov r0, sb
00348ca4: str r4, [sp, #0xc0]
00348ca8: str r4, [sp, #0xc4]
00348cac: str r4, [sp, #0xc8]
00348cb0: bl #0x3415d8
00348cb4: add r3, r8, #0x5c
00348cb8: mov r0, r7
00348cbc: mov r1, r6
00348cc0: add r7, sp, #0xb4
00348cc4: mov r2, #0x64
00348cc8: str r3, [sp]
00348ccc: bl #0x340154
00348cd0: mov r3, r7
00348cd4: ldr r2, [sp, #0xc]
00348cd8: ldr r1, [sp]
00348cdc: mov r0, sb
00348ce0: str r4, [sp, #0xb4]
00348ce4: str r4, [sp, #0xb8]
00348ce8: str r4, [sp, #0xbc]
00348cec: bl #0x3415d8
00348cf0: mov r0, r7
00348cf4: mov r1, r6
00348cf8: add r7, sp, #0xa8
00348cfc: mov r2, #0x64
00348d00: bl #0x340154
00348d04: mov r3, r7
00348d08: ldm sp, {r1, r2}
00348d0c: mov r0, sb
00348d10: add r8, r8, #0x1c
00348d14: str r4, [sp, #0xa8]
00348d18: str r4, [sp, #0xac]
00348d1c: str r4, [sp, #0xb0]
00348d20: bl #0x3415d8
00348d24: mov r0, r7
00348d28: mov r1, r6
00348d2c: add r7, sp, #0x9c
00348d30: mov r2, #0x64
00348d34: bl #0x340154
00348d38: mov ip, #0x7f
00348d3c: mov r3, r7
00348d40: ldr r2, [sp, #0xc]
00348d44: ldr r1, [sp, #8]
00348d48: mov r0, r8
00348d4c: strb ip, [sp, #0xdb]
00348d50: strb fp, [sp, #0xd9]
00348d54: strb sl, [sp, #0xd8]
00348d58: strb fp, [sp, #0xda]
00348d5c: str r4, [sp, #0x9c]
00348d60: str r4, [sp, #0xa0]
00348d64: str r4, [sp, #0xa4]
00348d68: bl #0x3415d8
00348d6c: mov r0, r7
00348d70: mov r1, r6
00348d74: add r7, sp, #0x90
00348d78: mov r2, #0x64
00348d7c: bl #0x340154
00348d80: mov r3, r7
00348d84: ldr r1, [sp, #8]
00348d88: ldr r2, [sp, #4]
00348d8c: mov r0, r8
00348d90: str r4, [sp, #0x90]
00348d94: str r4, [sp, #0x94]
00348d98: str r4, [sp, #0x98]
00348d9c: bl #0x3415d8
00348da0: mov r0, r7
00348da4: mov r1, r6
00348da8: add r7, sp, #0x84
00348dac: mov r2, #0x64
00348db0: bl #0x340154
00348db4: mov r3, r7
00348db8: ldr r2, [sp, #0xc]
00348dbc: ldr r1, [sp]
00348dc0: mov r0, r8
00348dc4: str r4, [sp, #0x84]
00348dc8: str r4, [sp, #0x88]
00348dcc: str r4, [sp, #0x8c]
00348dd0: bl #0x3415d8
00348dd4: mov r0, r7
00348dd8: mov r1, r6
00348ddc: mov r2, #0x64
00348de0: add r7, sp, #0x78
00348de4: bl #0x340154
00348de8: ldm sp, {r1, r2}
00348dec: mov r3, r7
00348df0: mov r0, r8
00348df4: str r4, [sp, #0x80]
00348df8: str r4, [sp, #0x78]
00348dfc: str r4, [sp, #0x7c]
00348e00: bl #0x3415d8
00348e04: mov r0, r7
00348e08: mov r1, r6
00348e0c: mov r2, #0x64
00348e10: bl #0x340154
00348e14: ldr r0, [sp, #0x14]
00348e18: bl #0x310be8
00348e1c: ldr r2, [sp, #0x10]
00348e20: ldr r3, [r5, r2]
00348e24: ldr r2, [sp, #0xfc]
00348e28: ldr r3, [r3]
00348e2c: cmp r2, r3
00348e30: bne #0x348e8c
00348e34: add sp, sp, #0x104
00348e38: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00348e3c: ldr r3, [r4, #4]
00348e40: ldr r1, [r3, #0xc]
00348e44: cmp r1, r4
00348e48: bne #0x348e64
00348e4c: mov r4, r3
00348e50: ldr r3, [r3, #4]
00348e54: ldr r2, [r3, #0xc]
00348e58: cmp r2, r4
00348e5c: beq #0x348e4c
00348e60: ldr r2, [r4, #0xc]
00348e64: cmp r2, r3
00348e68: movne r4, r3
00348e6c: b #0x348a20
00348e70: bl #0x310440
00348e74: b #0x348adc
00348e78: mov r0, r4
00348e7c: mov r1, #1
00348e80: bl #0x5d8b28
00348e84: mov r2, r0
00348e88: b #0x348b04
00348e8c: bl #0x30e310

# 0x348ea4 _ZN13ObjectManager6RemoveE12ObjectHandle
00348ea4: push {r4, r5, r6, r7, r8, sb, sl, lr}
00348ea8: sub sp, sp, #0x20
00348eac: add r4, sp, #4
00348eb0: mov r5, r0
00348eb4: mov r0, r4
00348eb8: stm r4, {r1, r2, r3}
00348ebc: bl #0x33fee4
00348ec0: subs r7, r0, #0
00348ec4: beq #0x348f34
00348ec8: ldr r0, [r7, #0x2f4]
00348ecc: cmp r0, #0
00348ed0: beq #0x348edc
00348ed4: mov r1, r7
00348ed8: bl #0x3968cc
00348edc: mov r0, r5
00348ee0: mov r1, r7
00348ee4: bl #0x3462b8
00348ee8: mov r2, r5
00348eec: ldr r0, [r2, #0x90]!
00348ef0: cmp r0, r2
00348ef4: beq #0x348f14
00348ef8: ldr r3, [r0, #8]
00348efc: cmp r7, r3
00348f00: beq #0x348f14
00348f04: ldr r0, [r0]
00348f08: cmp r2, r0
00348f0c: bne #0x348ef8
00348f10: mov r0, r2
00348f14: cmp r2, r0
00348f18: beq #0x348f34
00348f1c: ldr r3, [r0]
00348f20: ldr r2, [r0, #4]
00348f24: mov r1, #0xc
00348f28: str r3, [r2]
00348f2c: str r2, [r3, #4]
00348f30: bl #0x708f00
00348f34: mov r0, r4
00348f38: mov r1, #0
00348f3c: bl #0x33fdc0
00348f40: mov r8, r5
00348f44: mov sl, r0
00348f48: ldr r0, [r8, #0x2c]!
00348f4c: cmp r8, r0
00348f50: beq #0x348f70
00348f54: ldr r3, [r0, #8]
00348f58: ldr r6, [r0]
00348f5c: cmp sl, r3
00348f60: beq #0x3490ec
00348f64: mov r0, r6
00348f68: cmp r8, r0
00348f6c: bne #0x348f54
00348f70: mov r0, r4
00348f74: bl #0x33ff54
00348f78: mov r8, r5
00348f7c: mov sl, r0
00348f80: ldr r0, [r8, #0x70]!
00348f84: cmp r8, r0
00348f88: beq #0x348fa8
00348f8c: ldr r3, [r0, #8]
00348f90: ldr r6, [r0]
00348f94: cmp sl, r3
00348f98: beq #0x349108
00348f9c: mov r0, r6
00348fa0: cmp r8, r0
00348fa4: bne #0x348f8c
00348fa8: mov r0, r4
00348fac: bl #0x33ff54
00348fb0: subs sl, r0, #0
00348fb4: beq #0x348fec
00348fb8: mov r8, r5
00348fbc: ldr r0, [r8, #0x60]!
00348fc0: cmp r8, r0
00348fc4: beq #0x348fe4
00348fc8: ldr r3, [r0, #8]
00348fcc: ldr r6, [r0]
00348fd0: cmp sl, r3
00348fd4: beq #0x349124
00348fd8: mov r0, r6
00348fdc: cmp r8, r0
00348fe0: bne #0x348fc8
00348fe4: add r0, sl, #0x3c8
00348fe8: bl #0x3d2ff8
00348fec: mov r0, r4
00348ff0: mov r1, #0
00348ff4: bl #0x33fdc0
00348ff8: subs r8, r0, #0
00348ffc: beq #0x34900c
00349000: ldr r3, [r8, #0xf4]
00349004: cmp r3, #5
00349008: beq #0x349200
0034900c: bl #0x7fd794
00349010: ldrb r3, [r0, #5]
00349014: cmp r3, #0
00349018: addeq sl, r5, #0xc
0034901c: beq #0x349058
00349020: mov sb, r5
00349024: ldr r6, [sb, #0x100]!
00349028: add sl, r5, #0xc
0034902c: b #0x349048
00349030: ldr r8, [r6, #8]
00349034: bl #0x33fc88
00349038: ldr r3, [r0, #0x18]
0034903c: cmp r8, r3
00349040: beq #0x349180
00349044: ldr r6, [r6]
00349048: cmp r6, sb
0034904c: mov r0, sl
00349050: mov r1, r4
00349054: bne #0x349030
00349058: ldr r3, [r5, #0x50]
0034905c: sub r3, r3, #1
00349060: str r3, [r5, #0x50]
00349064: ldrb r3, [r7, #0x2fc]
00349068: cmp r3, #0
0034906c: beq #0x349140
00349070: mov r1, r4
00349074: mov r0, sl
00349078: bl #0x33fc88
0034907c: ldr r1, [r0, #0x18]
00349080: mov r0, r5
00349084: bl #0x343188
00349088: ldr r3, [r5, #0x10]
0034908c: ldr r0, [sp, #4]
00349090: cmp r3, #0
00349094: beq #0x3490d8
00349098: mov r1, sl
0034909c: b #0x3490a4
003490a0: mov r3, r2
003490a4: ldr r2, [r3, #0x10]
003490a8: cmp r0, r2
003490ac: ldrgt r2, [r3, #0xc]
003490b0: ldrle r2, [r3, #8]
003490b4: movgt r3, r1
003490b8: mov r1, r3
003490bc: cmp r2, #0
003490c0: bne #0x3490a0
003490c4: cmp sl, r3
003490c8: beq #0x3490d8
003490cc: ldr r2, [r3, #0x10]
003490d0: cmp r0, r2
003490d4: bge #0x34916c
003490d8: ldr r3, [r5, #0x78]
003490dc: add r3, r3, #1
003490e0: str r3, [r5, #0x78]
003490e4: add sp, sp, #0x20
003490e8: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003490ec: ldr r3, [r0, #4]
003490f0: mov r1, #0xc
003490f4: str r6, [r3]
003490f8: str r3, [r6, #4]
003490fc: bl #0x708f00
00349100: mov r0, r6
00349104: b #0x348f68
00349108: ldr r3, [r0, #4]
0034910c: mov r1, #0xc
00349110: str r6, [r3]
00349114: str r3, [r6, #4]
00349118: bl #0x708f00
0034911c: mov r0, r6
00349120: b #0x348fa0
00349124: ldr r3, [r0, #4]
00349128: mov r1, #0xc
0034912c: str r6, [r3]
00349130: str r3, [r6, #4]
00349134: bl #0x708f00
00349138: mov r0, r6
0034913c: b #0x348fdc
00349140: mov r1, r4
00349144: mov r0, sl
00349148: bl #0x33fc88
0034914c: ldr r3, [r0, #0x18]
00349150: cmp r3, #0
00349154: beq #0x349088
00349158: mov r0, r3
0034915c: ldr r3, [r3]
00349160: mov lr, pc
00349164: ldr pc, [r3, #4]
00349168: b #0x349088
0034916c: add r1, sp, #0x20
00349170: str r3, [r1, #-4]!
00349174: mov r0, sl
00349178: bl #0x347f58
0034917c: b #0x3490d8
00349180: add sb, sp, #0x10
00349184: mov r1, r8
00349188: mov r0, sb
0034918c: bl #0x33dd2c
00349190: mov r0, sb
00349194: bl #0x33ff54
00349198: subs r3, r0, #0
0034919c: beq #0x3491b4
003491a0: ldr r3, [r3]
003491a4: mov lr, pc
003491a8: ldr pc, [r3, #0x28]
003491ac: cmp r0, #0
003491b0: bne #0x3491e0
003491b4: ldr sb, [r8, #0x108]
003491b8: add r8, r5, #0x120
003491bc: mov r0, r8
003491c0: bl #0x343148
003491c4: uxth sb, sb
003491c8: strh sb, [r0, #8]
003491cc: ldr r3, [r5, #0x124]
003491d0: str r8, [r0]
003491d4: str r3, [r0, #4]
003491d8: str r0, [r3]
003491dc: str r0, [r5, #0x124]
003491e0: ldr r3, [r6]
003491e4: ldr r2, [r6, #4]
003491e8: mov r0, r6
003491ec: mov r1, #0xc
003491f0: str r3, [r2]
003491f4: str r2, [r3, #4]
003491f8: bl #0x708f00
003491fc: b #0x349058
00349200: mov sl, r5
00349204: ldr r0, [sl, #0x68]!
00349208: cmp sl, r0
0034920c: beq #0x34900c
00349210: ldr r3, [r0, #8]
00349214: ldr r6, [r0]
00349218: cmp r8, r3
0034921c: movne r0, r6
00349220: bne #0x349208
00349224: ldr r3, [r0, #4]
00349228: mov r1, #0xc
0034922c: str r6, [r3]
00349230: str r3, [r6, #4]
00349234: bl #0x708f00
00349238: mov r0, r6
0034923c: b #0x349208

# 0x349240 _ZN13ObjectManager10FakeRemoveE12ObjectHandle
00349240: push {r4, r5, r6, r7, r8, lr}
00349244: sub sp, sp, #0x18
00349248: add r4, sp, #4
0034924c: mov r5, r0
00349250: mov r0, r4
00349254: stm r4, {r1, r2, r3}
00349258: bl #0x33fee4
0034925c: mov r6, r0
00349260: ldr r0, [r0, #0x2f4]
00349264: mov r3, #1
00349268: strb r3, [r6, #0x81]
0034926c: cmp r0, #0
00349270: beq #0x34927c
00349274: mov r1, r6
00349278: bl #0x3968cc
0034927c: mov r0, r5
00349280: mov r1, r6
00349284: bl #0x3462b8
00349288: mov r2, r5
0034928c: ldr r0, [r2, #0x90]!
00349290: cmp r0, r2
00349294: beq #0x3492b4
00349298: ldr r3, [r0, #8]
0034929c: cmp r6, r3
003492a0: beq #0x3492b4
003492a4: ldr r0, [r0]
003492a8: cmp r2, r0
003492ac: bne #0x349298
003492b0: mov r0, r2
003492b4: cmp r2, r0
003492b8: beq #0x3492d4
003492bc: ldr r3, [r0]
003492c0: ldr r2, [r0, #4]
003492c4: mov r1, #0xc
003492c8: str r3, [r2]
003492cc: str r2, [r3, #4]
003492d0: bl #0x708f00
003492d4: mov r0, r4
003492d8: mov r1, #0
003492dc: bl #0x33fdc0
003492e0: mov r7, r5
003492e4: mov r8, r0
003492e8: ldr r0, [r7, #0x2c]!
003492ec: cmp r7, r0
003492f0: beq #0x349310
003492f4: ldr r3, [r0, #8]
003492f8: ldr r6, [r0]
003492fc: cmp r8, r3
00349300: beq #0x349468
00349304: mov r0, r6
00349308: cmp r7, r0
0034930c: bne #0x3492f4
00349310: mov r0, r4
00349314: bl #0x33ff54
00349318: mov r7, r5
0034931c: mov r8, r0
00349320: ldr r0, [r7, #0x70]!
00349324: cmp r7, r0
00349328: beq #0x349348
0034932c: ldr r3, [r0, #8]
00349330: ldr r6, [r0]
00349334: cmp r8, r3
00349338: beq #0x349484
0034933c: mov r0, r6
00349340: cmp r7, r0
00349344: bne #0x34932c
00349348: mov r0, r4
0034934c: bl #0x33ff54
00349350: subs r8, r0, #0
00349354: beq #0x349394
00349358: mov r7, r5
0034935c: ldr r0, [r7, #0x60]!
00349360: cmp r7, r0
00349364: beq #0x349384
00349368: ldr r3, [r0, #8]
0034936c: ldr r6, [r0]
00349370: cmp r8, r3
00349374: beq #0x3494a0
00349378: mov r0, r6
0034937c: cmp r7, r0
00349380: bne #0x349368
00349384: add r0, r8, #0x3c8
00349388: bl #0x3d2ff8
0034938c: mov r0, r8
00349390: bl #0x3a66f8
00349394: mov r0, r4
00349398: mov r1, #0
0034939c: bl #0x33fdc0
003493a0: subs r7, r0, #0
003493a4: beq #0x3493b4
003493a8: ldr r3, [r7, #0xf4]
003493ac: cmp r3, #5
003493b0: beq #0x3494ec
003493b4: mov r0, r4
003493b8: mov r1, #0
003493bc: bl #0x33fdc0
003493c0: mov r7, r5
003493c4: mov r8, r0
003493c8: ldr r0, [r7, #0x100]!
003493cc: cmp r7, r0
003493d0: beq #0x3493f0
003493d4: ldr r3, [r0, #8]
003493d8: ldr r6, [r0]
003493dc: cmp r8, r3
003493e0: beq #0x3494bc
003493e4: mov r0, r6
003493e8: cmp r7, r0
003493ec: bne #0x3493d4
003493f0: ldr r3, [r5, #0x10]
003493f4: ldr r0, [sp, #4]
003493f8: cmp r3, #0
003493fc: addeq r6, r5, #0xc
00349400: beq #0x349448
00349404: add r6, r5, #0xc
00349408: mov r1, r6
0034940c: b #0x349414
00349410: mov r3, r2
00349414: ldr r2, [r3, #0x10]
00349418: cmp r0, r2
0034941c: ldrgt r2, [r3, #0xc]
00349420: ldrle r2, [r3, #8]
00349424: movgt r3, r1
00349428: mov r1, r3
0034942c: cmp r2, #0
00349430: bne #0x349410
00349434: cmp r6, r3
00349438: beq #0x349448
0034943c: ldr r2, [r3, #0x10]
00349440: cmp r0, r2
00349444: bge #0x3494d8
00349448: mov r1, r4
0034944c: mov r0, r6
00349450: bl #0x33fc88
00349454: ldr r1, [r0, #0x18]
00349458: mov r0, r5
0034945c: bl #0x343188
00349460: add sp, sp, #0x18
00349464: pop {r4, r5, r6, r7, r8, pc}
00349468: ldr r3, [r0, #4]
0034946c: mov r1, #0xc
00349470: str r6, [r3]
00349474: str r3, [r6, #4]
00349478: bl #0x708f00
0034947c: mov r0, r6
00349480: b #0x349308
00349484: ldr r3, [r0, #4]
00349488: mov r1, #0xc
0034948c: str r6, [r3]
00349490: str r3, [r6, #4]
00349494: bl #0x708f00
00349498: mov r0, r6
0034949c: b #0x349340
003494a0: ldr r3, [r0, #4]
003494a4: mov r1, #0xc
003494a8: str r6, [r3]
003494ac: str r3, [r6, #4]
003494b0: bl #0x708f00
003494b4: mov r0, r6
003494b8: b #0x34937c
003494bc: ldr r3, [r0, #4]
003494c0: mov r1, #0xc
003494c4: str r6, [r3]
003494c8: str r3, [r6, #4]
003494cc: bl #0x708f00
003494d0: mov r0, r6
003494d4: b #0x3493e8
003494d8: add r1, sp, #0x18
003494dc: str r3, [r1, #-4]!
003494e0: mov r0, r6
003494e4: bl #0x347f58
003494e8: b #0x349448
003494ec: mov r8, r5
003494f0: ldr r0, [r8, #0x68]!
003494f4: cmp r8, r0
003494f8: beq #0x3493b4
003494fc: ldr r3, [r0, #8]
00349500: ldr r6, [r0]
00349504: cmp r7, r3
00349508: movne r0, r6
0034950c: bne #0x3494f4
00349510: ldr r3, [r0, #4]
00349514: mov r1, #0xc
00349518: str r6, [r3]
0034951c: str r3, [r6, #4]
00349520: bl #0x708f00
00349524: mov r0, r6
00349528: b #0x3494f4

# 0x3496b8 _ZN13ObjectManager5FlushEv
003496b8: push {r4, r5, r6, r7, r8, lr}
003496bc: mov r4, r0
003496c0: sub sp, sp, #8
003496c4: add r5, r0, #0x60
003496c8: ldr r6, [r0, #0x60]
003496cc: b #0x3496e4
003496d0: ldr r0, [r6, #8]
003496d4: bl #0x38ba6c
003496d8: ldr r0, [r6, #8]
003496dc: bl #0x33ddb4
003496e0: ldr r6, [r6]
003496e4: cmp r5, r6
003496e8: bne #0x3496d0
003496ec: ldr r6, [r4, #0x60]
003496f0: b #0x349708
003496f4: ldr r0, [r6, #8]
003496f8: cmp r0, #0
003496fc: addne r0, r0, #0x3c8
00349700: bl #0x3cb34c
00349704: ldr r6, [r6]
00349708: cmp r5, r6
0034970c: bne #0x3496f4
00349710: ldr r6, [r4, #0x14]
00349714: add r7, r4, #0xc
00349718: mov r8, #0
0034971c: cmp r6, r7
00349720: beq #0x3497a0
00349724: ldr r3, [r6, #0x2c]
00349728: cmp r3, #0
0034972c: beq #0x349744
00349730: mov r0, r3
00349734: ldr r3, [r3]
00349738: mov lr, pc
0034973c: ldr pc, [r3, #4]
00349740: str r8, [r6, #0x2c]
00349744: ldr r2, [r6, #0xc]
00349748: cmp r2, #0
0034974c: beq #0x349768
00349750: mov r6, r2
00349754: ldr r3, [r6, #8]
00349758: cmp r3, #0
0034975c: beq #0x34971c
00349760: mov r6, r3
00349764: b #0x349754
00349768: ldr r3, [r6, #4]
0034976c: ldr r1, [r3, #0xc]
00349770: cmp r6, r1
00349774: bne #0x349790
00349778: mov r6, r3
0034977c: ldr r3, [r3, #4]
00349780: ldr r2, [r3, #0xc]
00349784: cmp r2, r6
00349788: beq #0x349778
0034978c: ldr r2, [r6, #0xc]
00349790: cmp r2, r3
00349794: movne r6, r3
00349798: cmp r6, r7
0034979c: bne #0x349724
003497a0: add r0, r4, #0x90
003497a4: bl #0x345914
003497a8: ldr r3, [r4, #0x1c]
003497ac: cmp r3, #0
003497b0: bne #0x349af0
003497b4: mov r0, r5
003497b8: mov r6, r4
003497bc: bl #0x345b8c
003497c0: ldr r0, [r6, #0x68]!
003497c4: cmp r0, r6
003497c8: bne #0x3497d4
003497cc: b #0x3497ec
003497d0: mov r0, r5
003497d4: ldr r5, [r0]
003497d8: mov r1, #0xc
003497dc: bl #0x708f00
003497e0: cmp r5, r6
003497e4: bne #0x3497d0
003497e8: mov r0, r6
003497ec: str r0, [r4, #0x6c]
003497f0: str r0, [r4, #0x68]
003497f4: mov r6, r4
003497f8: add r0, r4, #0x100
003497fc: bl #0x34526c
00349800: ldr r0, [r6, #0x120]!
00349804: cmp r6, r0
00349808: bne #0x349814
0034980c: b #0x349828
00349810: mov r0, r5
00349814: ldr r5, [r0]
00349818: mov r1, #0xc
0034981c: bl #0x708f00
00349820: cmp r6, r5
00349824: bne #0x349810
00349828: ldr r3, [r4, #0x118]
0034982c: str r6, [r4, #0x124]
00349830: str r6, [r4, #0x120]
00349834: cmp r3, #0
00349838: bne #0x349ac8
0034983c: ldr r3, [r4, #0x174]
00349840: cmp r3, #0
00349844: beq #0x34986c
00349848: add r5, r4, #0x164
0034984c: mov r0, r5
00349850: ldr r1, [r4, #0x168]
00349854: bl #0x345f04
00349858: mov r3, #0
0034985c: str r5, [r4, #0x170]
00349860: str r3, [r4, #0x174]
00349864: str r5, [r4, #0x16c]
00349868: str r3, [r4, #0x168]
0034986c: ldr r3, [r4, #0x18c]
00349870: cmp r3, #0
00349874: beq #0x34989c
00349878: add r5, r4, #0x17c
0034987c: mov r0, r5
00349880: ldr r1, [r4, #0x180]
00349884: bl #0x345f04
00349888: mov r3, #0
0034988c: str r5, [r4, #0x188]
00349890: str r3, [r4, #0x18c]
00349894: str r5, [r4, #0x184]
00349898: str r3, [r4, #0x180]
0034989c: ldr r3, [r4, #0x1a4]
003498a0: cmp r3, #0
003498a4: beq #0x3498cc
003498a8: add r5, r4, #0x194
003498ac: mov r0, r5
003498b0: ldr r1, [r4, #0x198]
003498b4: bl #0x345f04
003498b8: mov r3, #0
003498bc: str r5, [r4, #0x1a0]
003498c0: str r3, [r4, #0x1a4]
003498c4: str r5, [r4, #0x19c]
003498c8: str r3, [r4, #0x198]
003498cc: add r0, r4, #0x128
003498d0: bl #0x345a4c
003498d4: mov r6, r4
003498d8: add r0, r4, #0x130
003498dc: bl #0x345a4c
003498e0: ldr r0, [r6, #0x80]!
003498e4: cmp r6, r0
003498e8: bne #0x3498f4
003498ec: b #0x349908
003498f0: mov r0, r5
003498f4: ldr r5, [r0]
003498f8: mov r1, #0xc
003498fc: bl #0x708f00
00349900: cmp r6, r5
00349904: bne #0x3498f0
00349908: add r5, r4, #0x88
0034990c: mov r0, r5
00349910: str r6, [r4, #0x84]
00349914: str r6, [r4, #0x80]
00349918: bl #0x345914
0034991c: mov r1, r5
00349920: mov r0, r4
00349924: bl #0x3427a0
00349928: ldr r3, [r4, #0x158]
0034992c: mov r5, #0
00349930: str r5, [r4, #0x138]
00349934: cmp r3, r5
00349938: str r5, [r4, #0x13c]
0034993c: str r5, [r4, #0x140]
00349940: bne #0x349aa4
00349944: add r1, sp, #8
00349948: mov r5, #0
0034994c: str r5, [r1, #-4]!
00349950: mov r0, r7
00349954: strb r5, [r4, #0x160]
00349958: bl #0x34952c
0034995c: mov r3, #1
00349960: str r3, [r4, #0x4c]
00349964: add r0, r4, #0x3c
00349968: str r5, [r4, #0x58]
0034996c: str r5, [r4, #0x50]
00349970: str r5, [r4, #0x54]
00349974: bl #0x34526c
00349978: add r0, r4, #0x2c
0034997c: bl #0x34526c
00349980: add r0, r4, #0x44
00349984: bl #0x34526c
00349988: add r0, r4, #0x34
0034998c: bl #0x34526c
00349990: mov r6, r4
00349994: add r0, r4, #0x70
00349998: bl #0x345b8c
0034999c: ldr r0, [r6, #0x24]!
003499a0: cmp r0, r6
003499a4: bne #0x3499b0
003499a8: b #0x3499c8
003499ac: mov r0, r5
003499b0: ldr r5, [r0]
003499b4: mov r1, #0xc
003499b8: bl #0x708f00
003499bc: cmp r5, r6
003499c0: bne #0x3499ac
003499c4: mov r0, r6
003499c8: mov r5, #0
003499cc: str r0, [r4, #0x28]
003499d0: str r0, [r4, #0x24]
003499d4: str r5, [r4, #0x7c]
003499d8: mov r0, r4
003499dc: bl #0x3454dc
003499e0: ldr r3, [r4, #0xa8]
003499e4: cmp r3, r5
003499e8: beq #0x349a0c
003499ec: add r6, r4, #0x98
003499f0: mov r0, r6
003499f4: ldr r1, [r4, #0x9c]
003499f8: bl #0x345b4c
003499fc: str r6, [r4, #0xa4]
00349a00: str r5, [r4, #0xa8]
00349a04: str r6, [r4, #0xa0]
00349a08: str r5, [r4, #0x9c]
00349a0c: ldr r3, [r4, #0xc0]
00349a10: cmp r3, #0
00349a14: beq #0x349a3c
00349a18: add r5, r4, #0xb0
00349a1c: mov r0, r5
00349a20: ldr r1, [r4, #0xb4]
00349a24: bl #0x345f84
00349a28: mov r3, #0
00349a2c: str r5, [r4, #0xbc]
00349a30: str r3, [r4, #0xc0]
00349a34: str r5, [r4, #0xb8]
00349a38: str r3, [r4, #0xb4]
00349a3c: ldr r3, [r4, #0xd8]
00349a40: cmp r3, #0
00349a44: beq #0x349a6c
00349a48: add r5, r4, #0xc8
00349a4c: mov r0, r5
00349a50: ldr r1, [r4, #0xcc]
00349a54: bl #0x345f84
00349a58: mov r3, #0
00349a5c: str r5, [r4, #0xd4]
00349a60: str r3, [r4, #0xd8]
00349a64: str r5, [r4, #0xd0]
00349a68: str r3, [r4, #0xcc]
00349a6c: ldr r3, [r4, #0xf0]
00349a70: cmp r3, #0
00349a74: beq #0x349a9c
00349a78: add r5, r4, #0xe0
00349a7c: mov r0, r5
00349a80: ldr r1, [r4, #0xe4]
00349a84: bl #0x345f84
00349a88: mov r3, #0
00349a8c: str r3, [r4, #0xf0]
00349a90: str r5, [r4, #0xec]
00349a94: str r5, [r4, #0xe8]
00349a98: str r3, [r4, #0xe4]
00349a9c: add sp, sp, #8
00349aa0: pop {r4, r5, r6, r7, r8, pc}
00349aa4: add r6, r4, #0x148
00349aa8: mov r0, r6
00349aac: ldr r1, [r4, #0x14c]
00349ab0: bl #0x345c94
00349ab4: str r6, [r4, #0x154]
00349ab8: str r5, [r4, #0x158]
00349abc: str r6, [r4, #0x150]
00349ac0: str r5, [r4, #0x14c]
00349ac4: b #0x349944
00349ac8: add r5, r4, #0x108
00349acc: mov r0, r5
00349ad0: ldr r1, [r4, #0x10c]
00349ad4: bl #0x3458d4
00349ad8: mov r3, #0
00349adc: str r5, [r4, #0x114]
00349ae0: str r3, [r4, #0x118]
00349ae4: str r5, [r4, #0x110]
00349ae8: str r3, [r4, #0x10c]
00349aec: b #0x34983c
00349af0: mov r0, r7
00349af4: ldr r1, [r4, #0x10]
00349af8: bl #0x347ed4
00349afc: mov r3, #0
00349b00: str r3, [r4, #0x1c]
00349b04: str r7, [r4, #0x14]
00349b08: str r3, [r4, #0x10]
00349b0c: str r7, [r4, #0x18]
00349b10: b #0x3497b4

# 0x349b14 _ZN13ObjectManagerD1Ev
00349b14: ldr r3, [pc, #0x34c]
00349b18: ldr r2, [pc, #0x34c]
00349b1c: push {r4, r5, r6, lr}
00349b20: add r3, pc, r3
00349b24: ldr r2, [r3, r2]
00349b28: mov r4, r0
00349b2c: add r2, r2, #8
00349b30: str r2, [r0]
00349b34: bl #0x3496b8
00349b38: ldr r3, [r4, #0x1a4]
00349b3c: cmp r3, #0
00349b40: beq #0x349b68
00349b44: add r5, r4, #0x194
00349b48: mov r0, r5
00349b4c: ldr r1, [r4, #0x198]
00349b50: bl #0x345f04
00349b54: mov r3, #0
00349b58: str r5, [r4, #0x1a0]
00349b5c: str r3, [r4, #0x1a4]
00349b60: str r5, [r4, #0x19c]
00349b64: str r3, [r4, #0x198]
00349b68: ldr r3, [r4, #0x18c]
00349b6c: cmp r3, #0
00349b70: beq #0x349b98
00349b74: add r5, r4, #0x17c
00349b78: mov r0, r5
00349b7c: ldr r1, [r4, #0x180]
00349b80: bl #0x345f04
00349b84: mov r3, #0
00349b88: str r5, [r4, #0x188]
00349b8c: str r3, [r4, #0x18c]
00349b90: str r5, [r4, #0x184]
00349b94: str r3, [r4, #0x180]
00349b98: ldr r3, [r4, #0x174]
00349b9c: cmp r3, #0
00349ba0: beq #0x349bc8
00349ba4: add r5, r4, #0x164
00349ba8: mov r0, r5
00349bac: ldr r1, [r4, #0x168]
00349bb0: bl #0x345f04
00349bb4: mov r3, #0
00349bb8: str r5, [r4, #0x170]
00349bbc: str r3, [r4, #0x174]
00349bc0: str r5, [r4, #0x16c]
00349bc4: str r3, [r4, #0x168]
00349bc8: ldr r3, [r4, #0x158]
00349bcc: cmp r3, #0
00349bd0: bne #0x349e40
00349bd4: add r0, r4, #0x130
00349bd8: bl #0x345a4c
00349bdc: add r0, r4, #0x128
00349be0: bl #0x345a4c
00349be4: ldr r0, [r4, #0x120]
00349be8: add r6, r4, #0x120
00349bec: cmp r0, r6
00349bf0: bne #0x349bfc
00349bf4: b #0x349c14
00349bf8: mov r0, r5
00349bfc: ldr r5, [r0]
00349c00: mov r1, #0xc
00349c04: bl #0x708f00
00349c08: cmp r5, r6
00349c0c: bne #0x349bf8
00349c10: mov r0, r6
00349c14: str r0, [r4, #0x120]
00349c18: str r0, [r6, #4]
00349c1c: ldr r3, [r4, #0x118]
00349c20: cmp r3, #0
00349c24: bne #0x349e18
00349c28: add r0, r4, #0x100
00349c2c: bl #0x34526c
00349c30: ldr r3, [r4, #0xf0]
00349c34: cmp r3, #0
00349c38: beq #0x349c60
00349c3c: add r5, r4, #0xe0
00349c40: mov r0, r5
00349c44: ldr r1, [r4, #0xe4]
00349c48: bl #0x345f84
00349c4c: mov r3, #0
00349c50: str r5, [r4, #0xec]
00349c54: str r3, [r4, #0xf0]
00349c58: str r5, [r4, #0xe8]
00349c5c: str r3, [r4, #0xe4]
00349c60: ldr r3, [r4, #0xd8]
00349c64: cmp r3, #0
00349c68: beq #0x349c90
00349c6c: add r5, r4, #0xc8
00349c70: mov r0, r5
00349c74: ldr r1, [r4, #0xcc]
00349c78: bl #0x345f84
00349c7c: mov r3, #0
00349c80: str r5, [r4, #0xd4]
00349c84: str r3, [r4, #0xd8]
00349c88: str r5, [r4, #0xd0]
00349c8c: str r3, [r4, #0xcc]
00349c90: ldr r3, [r4, #0xc0]
00349c94: cmp r3, #0
00349c98: beq #0x349cc0
00349c9c: add r5, r4, #0xb0
00349ca0: mov r0, r5
00349ca4: ldr r1, [r4, #0xb4]
00349ca8: bl #0x345f84
00349cac: mov r3, #0
00349cb0: str r5, [r4, #0xbc]
00349cb4: str r3, [r4, #0xc0]
00349cb8: str r5, [r4, #0xb8]
00349cbc: str r3, [r4, #0xb4]
00349cc0: ldr r3, [r4, #0xa8]
00349cc4: cmp r3, #0
00349cc8: bne #0x349df0
00349ccc: add r0, r4, #0x90
00349cd0: bl #0x345914
00349cd4: add r0, r4, #0x88
00349cd8: bl #0x345914
00349cdc: ldr r0, [r4, #0x80]
00349ce0: add r6, r4, #0x80
00349ce4: cmp r0, r6
00349ce8: bne #0x349cf4
00349cec: b #0x349d0c
00349cf0: mov r0, r5
00349cf4: ldr r5, [r0]
00349cf8: mov r1, #0xc
00349cfc: bl #0x708f00
00349d00: cmp r5, r6
00349d04: bne #0x349cf0
00349d08: mov r0, r6
00349d0c: str r0, [r4, #0x80]
00349d10: str r0, [r6, #4]
00349d14: add r0, r4, #0x70
00349d18: bl #0x345b8c
00349d1c: ldr r0, [r4, #0x68]
00349d20: add r6, r4, #0x68
00349d24: cmp r6, r0
00349d28: bne #0x349d34
00349d2c: b #0x349d48
00349d30: mov r0, r5
00349d34: ldr r5, [r0]
00349d38: mov r1, #0xc
00349d3c: bl #0x708f00
00349d40: cmp r6, r5
00349d44: bne #0x349d30
00349d48: str r6, [r4, #0x68]
00349d4c: add r0, r4, #0x60
00349d50: str r6, [r6, #4]
00349d54: bl #0x345b8c
00349d58: add r0, r4, #0x44
00349d5c: bl #0x34526c
00349d60: add r0, r4, #0x3c
00349d64: bl #0x34526c
00349d68: add r0, r4, #0x34
00349d6c: bl #0x34526c
00349d70: add r0, r4, #0x2c
00349d74: bl #0x34526c
00349d78: ldr r0, [r4, #0x24]
00349d7c: add r6, r4, #0x24
00349d80: cmp r0, r6
00349d84: bne #0x349d90
00349d88: b #0x349da8
00349d8c: mov r0, r5
00349d90: ldr r5, [r0]
00349d94: mov r1, #0xc
00349d98: bl #0x708f00
00349d9c: cmp r5, r6
00349da0: bne #0x349d8c
00349da4: mov r0, r6
00349da8: str r0, [r4, #0x24]
00349dac: str r0, [r6, #4]
00349db0: ldr r3, [r4, #0x1c]
00349db4: cmp r3, #0
00349db8: beq #0x349de0
00349dbc: add r5, r4, #0xc
00349dc0: mov r0, r5
00349dc4: ldr r1, [r4, #0x10]
00349dc8: bl #0x347ed4
00349dcc: mov r3, #0
00349dd0: str r5, [r4, #0x18]
00349dd4: str r3, [r4, #0x1c]
00349dd8: str r5, [r4, #0x14]
00349ddc: str r3, [r4, #0x10]
00349de0: add r0, r4, #4
00349de4: bl #0x34526c
00349de8: mov r0, r4
00349dec: pop {r4, r5, r6, pc}
00349df0: add r5, r4, #0x98
00349df4: mov r0, r5
00349df8: ldr r1, [r4, #0x9c]
00349dfc: bl #0x345b4c
00349e00: mov r3, #0
00349e04: str r5, [r4, #0xa4]
00349e08: str r3, [r4, #0xa8]
00349e0c: str r5, [r4, #0xa0]
00349e10: str r3, [r4, #0x9c]
00349e14: b #0x349ccc
00349e18: add r5, r4, #0x108
00349e1c: mov r0, r5
00349e20: ldr r1, [r4, #0x10c]
00349e24: bl #0x3458d4
00349e28: mov r3, #0
00349e2c: str r5, [r4, #0x114]
00349e30: str r3, [r4, #0x118]
00349e34: str r5, [r4, #0x110]
00349e38: str r3, [r4, #0x10c]
00349e3c: b #0x349c28
00349e40: add r5, r4, #0x148
00349e44: mov r0, r5
00349e48: ldr r1, [r4, #0x14c]
00349e4c: bl #0x345c94
00349e50: mov r3, #0
00349e54: str r5, [r4, #0x154]
00349e58: str r3, [r4, #0x158]
00349e5c: str r5, [r4, #0x150]
00349e60: str r3, [r4, #0x14c]
00349e64: b #0x349bd4
00349e68: rsbeq sl, r4, r0, ror pc
00349e6c: andeq r1, r0, ip, asr r1

# 0x349e70 _ZN13ObjectManagerD0Ev
00349e70: push {r4, lr}
00349e74: mov r4, r0
00349e78: bl #0x349b14
00349e7c: mov r0, r4
00349e80: bl #0x310440
00349e84: mov r0, r4
00349e88: pop {r4, pc}

# 0x349e8c _ZN13ObjectManagerD2Ev
00349e8c: ldr r3, [pc, #0x34c]
00349e90: ldr r2, [pc, #0x34c]
00349e94: push {r4, r5, r6, lr}
00349e98: add r3, pc, r3
00349e9c: ldr r2, [r3, r2]
00349ea0: mov r4, r0
00349ea4: add r2, r2, #8
00349ea8: str r2, [r0]
00349eac: bl #0x3496b8
00349eb0: ldr r3, [r4, #0x1a4]
00349eb4: cmp r3, #0
00349eb8: beq #0x349ee0
00349ebc: add r5, r4, #0x194
00349ec0: mov r0, r5
00349ec4: ldr r1, [r4, #0x198]
00349ec8: bl #0x345f04
00349ecc: mov r3, #0
00349ed0: str r5, [r4, #0x1a0]
00349ed4: str r3, [r4, #0x1a4]
00349ed8: str r5, [r4, #0x19c]
00349edc: str r3, [r4, #0x198]
00349ee0: ldr r3, [r4, #0x18c]
00349ee4: cmp r3, #0
00349ee8: beq #0x349f10
00349eec: add r5, r4, #0x17c
00349ef0: mov r0, r5
00349ef4: ldr r1, [r4, #0x180]
00349ef8: bl #0x345f04
00349efc: mov r3, #0
00349f00: str r5, [r4, #0x188]
00349f04: str r3, [r4, #0x18c]
00349f08: str r5, [r4, #0x184]
00349f0c: str r3, [r4, #0x180]
00349f10: ldr r3, [r4, #0x174]
00349f14: cmp r3, #0
00349f18: beq #0x349f40
00349f1c: add r5, r4, #0x164
00349f20: mov r0, r5
00349f24: ldr r1, [r4, #0x168]
00349f28: bl #0x345f04
00349f2c: mov r3, #0
00349f30: str r5, [r4, #0x170]
00349f34: str r3, [r4, #0x174]
00349f38: str r5, [r4, #0x16c]
00349f3c: str r3, [r4, #0x168]
00349f40: ldr r3, [r4, #0x158]
00349f44: cmp r3, #0
00349f48: bne #0x34a1b8
00349f4c: add r0, r4, #0x130
00349f50: bl #0x345a4c
00349f54: add r0, r4, #0x128
00349f58: bl #0x345a4c
00349f5c: ldr r0, [r4, #0x120]
00349f60: add r6, r4, #0x120
00349f64: cmp r0, r6
00349f68: bne #0x349f74
00349f6c: b #0x349f8c
00349f70: mov r0, r5
00349f74: ldr r5, [r0]
00349f78: mov r1, #0xc
00349f7c: bl #0x708f00
00349f80: cmp r5, r6
00349f84: bne #0x349f70
00349f88: mov r0, r6
00349f8c: str r0, [r4, #0x120]
00349f90: str r0, [r6, #4]
00349f94: ldr r3, [r4, #0x118]
00349f98: cmp r3, #0
00349f9c: bne #0x34a190
00349fa0: add r0, r4, #0x100
00349fa4: bl #0x34526c
00349fa8: ldr r3, [r4, #0xf0]
00349fac: cmp r3, #0
00349fb0: beq #0x349fd8
00349fb4: add r5, r4, #0xe0
00349fb8: mov r0, r5
00349fbc: ldr r1, [r4, #0xe4]
00349fc0: bl #0x345f84
00349fc4: mov r3, #0
00349fc8: str r5, [r4, #0xec]
00349fcc: str r3, [r4, #0xf0]
00349fd0: str r5, [r4, #0xe8]
00349fd4: str r3, [r4, #0xe4]
00349fd8: ldr r3, [r4, #0xd8]
00349fdc: cmp r3, #0
00349fe0: beq #0x34a008
00349fe4: add r5, r4, #0xc8
00349fe8: mov r0, r5
00349fec: ldr r1, [r4, #0xcc]
00349ff0: bl #0x345f84
00349ff4: mov r3, #0
00349ff8: str r5, [r4, #0xd4]
00349ffc: str r3, [r4, #0xd8]
0034a000: str r5, [r4, #0xd0]
0034a004: str r3, [r4, #0xcc]
0034a008: ldr r3, [r4, #0xc0]
0034a00c: cmp r3, #0
0034a010: beq #0x34a038
0034a014: add r5, r4, #0xb0
0034a018: mov r0, r5
0034a01c: ldr r1, [r4, #0xb4]
0034a020: bl #0x345f84
0034a024: mov r3, #0
0034a028: str r5, [r4, #0xbc]
0034a02c: str r3, [r4, #0xc0]
0034a030: str r5, [r4, #0xb8]
0034a034: str r3, [r4, #0xb4]
0034a038: ldr r3, [r4, #0xa8]
0034a03c: cmp r3, #0
0034a040: bne #0x34a168
0034a044: add r0, r4, #0x90
0034a048: bl #0x345914
0034a04c: add r0, r4, #0x88
0034a050: bl #0x345914
0034a054: ldr r0, [r4, #0x80]
0034a058: add r6, r4, #0x80
0034a05c: cmp r0, r6
0034a060: bne #0x34a06c
0034a064: b #0x34a084
0034a068: mov r0, r5
0034a06c: ldr r5, [r0]
0034a070: mov r1, #0xc
0034a074: bl #0x708f00
0034a078: cmp r5, r6
0034a07c: bne #0x34a068
0034a080: mov r0, r6
0034a084: str r0, [r4, #0x80]
0034a088: str r0, [r6, #4]
0034a08c: add r0, r4, #0x70
0034a090: bl #0x345b8c
0034a094: ldr r0, [r4, #0x68]
0034a098: add r6, r4, #0x68
0034a09c: cmp r6, r0
0034a0a0: bne #0x34a0ac
0034a0a4: b #0x34a0c0
0034a0a8: mov r0, r5
0034a0ac: ldr r5, [r0]
0034a0b0: mov r1, #0xc
0034a0b4: bl #0x708f00
0034a0b8: cmp r6, r5
0034a0bc: bne #0x34a0a8
0034a0c0: str r6, [r4, #0x68]
0034a0c4: add r0, r4, #0x60
0034a0c8: str r6, [r6, #4]
0034a0cc: bl #0x345b8c
0034a0d0: add r0, r4, #0x44
0034a0d4: bl #0x34526c
0034a0d8: add r0, r4, #0x3c
0034a0dc: bl #0x34526c
0034a0e0: add r0, r4, #0x34
0034a0e4: bl #0x34526c
0034a0e8: add r0, r4, #0x2c
0034a0ec: bl #0x34526c
0034a0f0: ldr r0, [r4, #0x24]
0034a0f4: add r6, r4, #0x24
0034a0f8: cmp r0, r6
0034a0fc: bne #0x34a108
0034a100: b #0x34a120
0034a104: mov r0, r5
0034a108: ldr r5, [r0]
0034a10c: mov r1, #0xc
0034a110: bl #0x708f00
0034a114: cmp r5, r6
0034a118: bne #0x34a104
0034a11c: mov r0, r6
0034a120: str r0, [r4, #0x24]
0034a124: str r0, [r6, #4]
0034a128: ldr r3, [r4, #0x1c]
0034a12c: cmp r3, #0
0034a130: beq #0x34a158
0034a134: add r5, r4, #0xc
0034a138: mov r0, r5
0034a13c: ldr r1, [r4, #0x10]
0034a140: bl #0x347ed4
0034a144: mov r3, #0
0034a148: str r5, [r4, #0x18]
0034a14c: str r3, [r4, #0x1c]
0034a150: str r5, [r4, #0x14]
0034a154: str r3, [r4, #0x10]
0034a158: add r0, r4, #4
0034a15c: bl #0x34526c
0034a160: mov r0, r4
0034a164: pop {r4, r5, r6, pc}
0034a168: add r5, r4, #0x98
0034a16c: mov r0, r5
0034a170: ldr r1, [r4, #0x9c]
0034a174: bl #0x345b4c
0034a178: mov r3, #0
0034a17c: str r5, [r4, #0xa4]
0034a180: str r3, [r4, #0xa8]
0034a184: str r5, [r4, #0xa0]
0034a188: str r3, [r4, #0x9c]
0034a18c: b #0x34a044
0034a190: add r5, r4, #0x108
0034a194: mov r0, r5
0034a198: ldr r1, [r4, #0x10c]
0034a19c: bl #0x3458d4
0034a1a0: mov r3, #0
0034a1a4: str r5, [r4, #0x114]
0034a1a8: str r3, [r4, #0x118]
0034a1ac: str r5, [r4, #0x110]
0034a1b0: str r3, [r4, #0x10c]
0034a1b4: b #0x349fa0
0034a1b8: add r5, r4, #0x148
0034a1bc: mov r0, r5
0034a1c0: ldr r1, [r4, #0x14c]
0034a1c4: bl #0x345c94
0034a1c8: mov r3, #0
0034a1cc: str r5, [r4, #0x154]
0034a1d0: str r3, [r4, #0x158]
0034a1d4: str r5, [r4, #0x150]
0034a1d8: str r3, [r4, #0x14c]
0034a1dc: b #0x349f4c

# 0x34a1e8 _ZN13ObjectManagerC1Ev
0034a1e8: ldr r2, [pc, #0x20c]
0034a1ec: ldr ip, [pc, #0x20c]
0034a1f0: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034a1f4: add r2, pc, r2
0034a1f8: ldr ip, [r2, ip]
0034a1fc: mov r1, r0
0034a200: mov r3, #0
0034a204: add ip, ip, #8
0034a208: str ip, [r1], #4
0034a20c: add r5, r0, #0x60
0034a210: mov ip, r0
0034a214: str r1, [r0, #8]
0034a218: str r1, [r0, #4]
0034a21c: str r3, [r0, #0x10]
0034a220: strb r3, [ip, #0xc]!
0034a224: str r5, [r0, #0x60]
0034a228: add r5, r0, #0x80
0034a22c: str r5, [r0, #0x84]
0034a230: ldr r5, [r0, #0x60]
0034a234: add lr, r0, #0x70
0034a238: str lr, [r0, #0x70]
0034a23c: str r5, [r0, #0x64]
0034a240: ldr r5, [r0, #0x70]
0034a244: add sb, r0, #0x24
0034a248: add sl, r0, #0x2c
0034a24c: add r8, r0, #0x34
0034a250: add r7, r0, #0x3c
0034a254: add r6, r0, #0x44
0034a258: add fp, r0, #0x68
0034a25c: add lr, r0, #0x88
0034a260: str lr, [r0, #0x88]
0034a264: str ip, [r0, #0x18]
0034a268: str sb, [r0, #0x28]
0034a26c: str sl, [r0, #0x30]
0034a270: str r8, [r0, #0x38]
0034a274: str r7, [r0, #0x40]
0034a278: str r6, [r0, #0x48]
0034a27c: str fp, [r0, #0x6c]
0034a280: str r5, [r0, #0x74]
0034a284: str ip, [r0, #0x14]
0034a288: str sb, [r0, #0x24]
0034a28c: str sl, [r0, #0x2c]
0034a290: str r8, [r0, #0x34]
0034a294: str r7, [r0, #0x3c]
0034a298: str r6, [r0, #0x44]
0034a29c: str r3, [r0, #0x1c]
0034a2a0: str r3, [r0, #0x4c]
0034a2a4: str r3, [r0, #0x50]
0034a2a8: str r3, [r0, #0x54]
0034a2ac: str r3, [r0, #0x58]
0034a2b0: str r3, [r0, #0x5c]
0034a2b4: ldr ip, [r0, #0x84]
0034a2b8: ldr r5, [r0, #0x88]
0034a2bc: mov r1, r0
0034a2c0: add lr, r0, #0x90
0034a2c4: str fp, [r0, #0x68]
0034a2c8: str ip, [r0, #0x80]
0034a2cc: str r5, [r0, #0x8c]
0034a2d0: str lr, [r0, #0x94]
0034a2d4: str lr, [r0, #0x90]
0034a2d8: str r3, [r0, #0x78]
0034a2dc: str r3, [r0, #0x7c]
0034a2e0: str r3, [r0, #0x9c]
0034a2e4: mov ip, r0
0034a2e8: strb r3, [r1, #0x98]!
0034a2ec: str r1, [r0, #0xa4]
0034a2f0: str r1, [r0, #0xa0]
0034a2f4: str r3, [r0, #0xa8]
0034a2f8: str r3, [r0, #0xb4]
0034a2fc: mov r1, r0
0034a300: strb r3, [ip, #0xb0]!
0034a304: str ip, [r0, #0xbc]
0034a308: str ip, [r0, #0xb8]
0034a30c: str r3, [r0, #0xc0]
0034a310: mov ip, r0
0034a314: str r3, [r0, #0xcc]
0034a318: strb r3, [r1, #0xc8]!
0034a31c: str r1, [r0, #0xd4]
0034a320: str r1, [r0, #0xd0]
0034a324: add lr, r0, #0x100
0034a328: mov r1, r0
0034a32c: str r3, [r0, #0xd8]
0034a330: str r3, [r0, #0xe4]
0034a334: strb r3, [ip, #0xe0]!
0034a338: add r6, r0, #0x120
0034a33c: add r5, r0, #0x128
0034a340: str ip, [r0, #0xec]
0034a344: str lr, [r0, #0x104]
0034a348: str ip, [r0, #0xe8]
0034a34c: str lr, [r0, #0x100]
0034a350: mov ip, r0
0034a354: add lr, r0, #0x130
0034a358: str r3, [r0, #0xf0]
0034a35c: str r3, [r0, #0xf8]
0034a360: str r3, [r0, #0x10c]
0034a364: strb r3, [r1, #0x108]!
0034a368: str r1, [r0, #0x114]
0034a36c: str r1, [r0, #0x110]
0034a370: str r6, [r0, #0x120]
0034a374: str r6, [r0, #0x124]
0034a378: str r5, [r0, #0x12c]
0034a37c: str lr, [r0, #0x134]
0034a380: str r5, [r0, #0x128]
0034a384: str lr, [r0, #0x130]
0034a388: str r3, [r0, #0x118]
0034a38c: str r3, [r0, #0x14c]
0034a390: mov r1, r0
0034a394: strb r3, [ip, #0x148]!
0034a398: str ip, [r0, #0x154]
0034a39c: str ip, [r0, #0x150]
0034a3a0: str r3, [r0, #0x158]
0034a3a4: mov ip, r0
0034a3a8: str r3, [r0, #0x168]
0034a3ac: strb r3, [r1, #0x164]!
0034a3b0: str r1, [r0, #0x170]
0034a3b4: str r1, [r0, #0x16c]
0034a3b8: str r3, [r0, #0x174]
0034a3bc: mov r1, r0
0034a3c0: str r3, [r0, #0x180]
0034a3c4: strb r3, [ip, #0x17c]!
0034a3c8: str ip, [r0, #0x188]
0034a3cc: str ip, [r0, #0x184]
0034a3d0: str r3, [r0, #0x18c]
0034a3d4: str r3, [r0, #0x198]
0034a3d8: strb r3, [r1, #0x194]!
0034a3dc: mov r4, r0
0034a3e0: str r1, [r0, #0x1a0]
0034a3e4: str r1, [r0, #0x19c]
0034a3e8: strb r3, [r0, #0x1ac]
0034a3ec: str r3, [r0, #0x1a4]
0034a3f0: bl #0x3496b8
0034a3f4: mov r0, r4
0034a3f8: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034a3fc: mlseq r4, ip, r8, sl
0034a400: andeq r1, r0, ip, asr r1

# 0x34a404 _ZN13ObjectManagerC2Ev
0034a404: ldr r2, [pc, #0x20c]
0034a408: ldr ip, [pc, #0x20c]
0034a40c: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034a410: add r2, pc, r2
0034a414: ldr ip, [r2, ip]
0034a418: mov r1, r0
0034a41c: mov r3, #0
0034a420: add ip, ip, #8
0034a424: str ip, [r1], #4
0034a428: add r5, r0, #0x60
0034a42c: mov ip, r0
0034a430: str r1, [r0, #8]
0034a434: str r1, [r0, #4]
0034a438: str r3, [r0, #0x10]
0034a43c: strb r3, [ip, #0xc]!
0034a440: str r5, [r0, #0x60]
0034a444: add r5, r0, #0x80
0034a448: str r5, [r0, #0x84]
0034a44c: ldr r5, [r0, #0x60]
0034a450: add lr, r0, #0x70
0034a454: str lr, [r0, #0x70]
0034a458: str r5, [r0, #0x64]
0034a45c: ldr r5, [r0, #0x70]
0034a460: add sb, r0, #0x24
0034a464: add sl, r0, #0x2c
0034a468: add r8, r0, #0x34
0034a46c: add r7, r0, #0x3c
0034a470: add r6, r0, #0x44
0034a474: add fp, r0, #0x68
0034a478: add lr, r0, #0x88
0034a47c: str lr, [r0, #0x88]
0034a480: str ip, [r0, #0x18]
0034a484: str sb, [r0, #0x28]
0034a488: str sl, [r0, #0x30]
0034a48c: str r8, [r0, #0x38]
0034a490: str r7, [r0, #0x40]
0034a494: str r6, [r0, #0x48]
0034a498: str fp, [r0, #0x6c]
0034a49c: str r5, [r0, #0x74]
0034a4a0: str ip, [r0, #0x14]
0034a4a4: str sb, [r0, #0x24]
0034a4a8: str sl, [r0, #0x2c]
0034a4ac: str r8, [r0, #0x34]
0034a4b0: str r7, [r0, #0x3c]
0034a4b4: str r6, [r0, #0x44]
0034a4b8: str r3, [r0, #0x1c]
0034a4bc: str r3, [r0, #0x4c]
0034a4c0: str r3, [r0, #0x50]
0034a4c4: str r3, [r0, #0x54]
0034a4c8: str r3, [r0, #0x58]
0034a4cc: str r3, [r0, #0x5c]
0034a4d0: ldr ip, [r0, #0x84]
0034a4d4: ldr r5, [r0, #0x88]
0034a4d8: mov r1, r0
0034a4dc: add lr, r0, #0x90
0034a4e0: str fp, [r0, #0x68]
0034a4e4: str ip, [r0, #0x80]
0034a4e8: str r5, [r0, #0x8c]
0034a4ec: str lr, [r0, #0x94]
0034a4f0: str lr, [r0, #0x90]
0034a4f4: str r3, [r0, #0x78]
0034a4f8: str r3, [r0, #0x7c]
0034a4fc: str r3, [r0, #0x9c]
0034a500: mov ip, r0
0034a504: strb r3, [r1, #0x98]!
0034a508: str r1, [r0, #0xa4]
0034a50c: str r1, [r0, #0xa0]
0034a510: str r3, [r0, #0xa8]
0034a514: str r3, [r0, #0xb4]
0034a518: mov r1, r0
0034a51c: strb r3, [ip, #0xb0]!
0034a520: str ip, [r0, #0xbc]
0034a524: str ip, [r0, #0xb8]
0034a528: str r3, [r0, #0xc0]
0034a52c: mov ip, r0
0034a530: str r3, [r0, #0xcc]
0034a534: strb r3, [r1, #0xc8]!
0034a538: str r1, [r0, #0xd4]
0034a53c: str r1, [r0, #0xd0]
0034a540: add lr, r0, #0x100
0034a544: mov r1, r0
0034a548: str r3, [r0, #0xd8]
0034a54c: str r3, [r0, #0xe4]
0034a550: strb r3, [ip, #0xe0]!
0034a554: add r6, r0, #0x120
0034a558: add r5, r0, #0x128
0034a55c: str ip, [r0, #0xec]
0034a560: str lr, [r0, #0x104]
0034a564: str ip, [r0, #0xe8]
0034a568: str lr, [r0, #0x100]
0034a56c: mov ip, r0
0034a570: add lr, r0, #0x130
0034a574: str r3, [r0, #0xf0]
0034a578: str r3, [r0, #0xf8]
0034a57c: str r3, [r0, #0x10c]
0034a580: strb r3, [r1, #0x108]!
0034a584: str r1, [r0, #0x114]
0034a588: str r1, [r0, #0x110]
0034a58c: str r6, [r0, #0x120]
0034a590: str r6, [r0, #0x124]
0034a594: str r5, [r0, #0x12c]
0034a598: str lr, [r0, #0x134]
0034a59c: str r5, [r0, #0x128]
0034a5a0: str lr, [r0, #0x130]
0034a5a4: str r3, [r0, #0x118]
0034a5a8: str r3, [r0, #0x14c]
0034a5ac: mov r1, r0
0034a5b0: strb r3, [ip, #0x148]!
0034a5b4: str ip, [r0, #0x154]
0034a5b8: str ip, [r0, #0x150]
0034a5bc: str r3, [r0, #0x158]
0034a5c0: mov ip, r0
0034a5c4: str r3, [r0, #0x168]
0034a5c8: strb r3, [r1, #0x164]!
0034a5cc: str r1, [r0, #0x170]
0034a5d0: str r1, [r0, #0x16c]
0034a5d4: str r3, [r0, #0x174]
0034a5d8: mov r1, r0
0034a5dc: str r3, [r0, #0x180]
0034a5e0: strb r3, [ip, #0x17c]!
0034a5e4: str ip, [r0, #0x188]
0034a5e8: str ip, [r0, #0x184]
0034a5ec: str r3, [r0, #0x18c]
0034a5f0: str r3, [r0, #0x198]
0034a5f4: strb r3, [r1, #0x194]!
0034a5f8: mov r4, r0
0034a5fc: str r1, [r0, #0x1a0]
0034a600: str r1, [r0, #0x19c]
0034a604: strb r3, [r0, #0x1ac]
0034a608: str r3, [r0, #0x1a4]
0034a60c: bl #0x3496b8
0034a610: mov r0, r4
0034a614: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034a618: rsbeq sl, r4, r0, lsl #13
0034a61c: andeq r1, r0, ip, asr r1

# 0x34a620 _ZN13ObjectManager6UpdateEf
0034a620: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034a624: ldr r7, [pc, #0x5c4]
0034a628: ldr sl, [pc, #0x5c4]
0034a62c: mov r5, r0
0034a630: add r7, pc, r7
0034a634: ldr r3, [r7, sl]
0034a638: ldr r0, [pc, #0x5b8]
0034a63c: sub sp, sp, #0xb4
0034a640: ldr r3, [r3]
0034a644: add r0, pc, r0
0034a648: mov r4, r1
0034a64c: str r3, [sp, #0xac]
0034a650: bl #0x3136b4
0034a654: bl #0x7fd794
0034a658: ldrb r3, [r0, #5]
0034a65c: cmp r3, #0
0034a660: movne r3, #1
0034a664: strbne r3, [r5, #0xfd]
0034a668: strbne r3, [r5, #0xfc]
0034a66c: ldr r3, [pc, #0x588]
0034a670: ldr r0, [r7, r3]
0034a674: bl #0x31f594
0034a678: cmp r0, #0
0034a67c: beq #0x34a68c
0034a680: ldrb r3, [r0, #0x144]
0034a684: cmp r3, #0
0034a688: bne #0x34aa8c
0034a68c: mov r0, r5
0034a690: bl #0x34163c
0034a694: mov r1, r4
0034a698: mov r0, r5
0034a69c: bl #0x340274
0034a6a0: mov r4, r5
0034a6a4: mov r0, r5
0034a6a8: bl #0x3460cc
0034a6ac: ldr ip, [r4, #0x34]!
0034a6b0: cmp ip, r4
0034a6b4: addeq r8, r5, #0x2c
0034a6b8: beq #0x34a700
0034a6bc: mov r3, ip
0034a6c0: ldr r3, [r3]
0034a6c4: cmp r4, r3
0034a6c8: bne #0x34a6c0
0034a6cc: add r8, r5, #0x2c
0034a6d0: mov r0, r8
0034a6d4: str ip, [sp, #0x68]
0034a6d8: add r1, sp, #0x64
0034a6dc: add ip, sp, #0x70
0034a6e0: add r2, sp, #0x68
0034a6e4: add r3, sp, #0x6c
0034a6e8: str ip, [sp]
0034a6ec: str r8, [sp, #0x64]
0034a6f0: str r4, [sp, #0x6c]
0034a6f4: bl #0x345428
0034a6f8: mov r0, r4
0034a6fc: bl #0x34526c
0034a700: mov r6, r5
0034a704: ldr r4, [r6, #0x3c]!
0034a708: cmp r4, r6
0034a70c: beq #0x34a760
0034a710: mov r3, r4
0034a714: ldr r3, [r3]
0034a718: cmp r6, r3
0034a71c: bne #0x34a714
0034a720: add r2, sp, #0x54
0034a724: cmp r4, r6
0034a728: add sb, sp, #0x48
0034a72c: str r2, [sp, #0xc]
0034a730: beq #0x34a760
0034a734: ldr r1, [r4, #8]
0034a738: ldrb r3, [r1, #0x29]
0034a73c: cmp r3, #0
0034a740: beq #0x34ab14
0034a744: ldrb r3, [r1, #0x82]
0034a748: cmp r3, #0
0034a74c: bne #0x34ab20
0034a750: ldr fp, [r4]
0034a754: mov r4, fp
0034a758: cmp r4, r6
0034a75c: bne #0x34a734
0034a760: mov sb, r5
0034a764: ldr r6, [sb, #0x44]!
0034a768: cmp sb, r6
0034a76c: beq #0x34a7b0
0034a770: ldr r4, [r6, #8]
0034a774: ldrb r3, [r4, #0xac]
0034a778: cmp r3, #0
0034a77c: bne #0x34a9c0
0034a780: ldr r3, [r4, #0xa8]
0034a784: cmp r3, #0
0034a788: beq #0x34a9c0
0034a78c: mov r1, #1
0034a790: mov r0, r4
0034a794: bl #0x33e6d4
0034a798: mov r0, r4
0034a79c: mov r1, #1
0034a7a0: bl #0x33e61c
0034a7a4: ldr r6, [r6]
0034a7a8: cmp sb, r6
0034a7ac: bne #0x34a770
0034a7b0: mov r3, #0
0034a7b4: str r3, [r5, #0x58]
0034a7b8: str r3, [r5, #0x5c]
0034a7bc: ldr r4, [r5, #0x2c]
0034a7c0: add r3, sp, #0x3c
0034a7c4: str r3, [sp, #0xc]
0034a7c8: ldr r3, [pc, #0x430]
0034a7cc: add ip, sp, #0x24
0034a7d0: str ip, [sp, #0x10]
0034a7d4: add r2, sp, #0x30
0034a7d8: add ip, sp, #0x60
0034a7dc: cmp r8, r4
0034a7e0: add r6, r5, #0x70
0034a7e4: str r2, [sp, #0x14]
0034a7e8: str r3, [sp, #0x18]
0034a7ec: str ip, [sp, #0x1c]
0034a7f0: beq #0x34a8dc
0034a7f4: ldr sb, [r4, #8]
0034a7f8: cmp sb, #0
0034a7fc: beq #0x34a8cc
0034a800: ldr r3, [sb]
0034a804: mov r0, sb
0034a808: mov lr, pc
0034a80c: ldr pc, [r3, #0x24]
0034a810: cmp r0, #0
0034a814: bne #0x34aa64
0034a818: ldrb r3, [sb, #0x85]
0034a81c: cmp r3, #0
0034a820: beq #0x34aa10
0034a824: ldrb r3, [sb, #0x8a]
0034a828: cmp r3, #0
0034a82c: beq #0x34aa10
0034a830: ldrb fp, [sb, #0x81]
0034a834: cmp fp, #0
0034a838: bne #0x34a9e0
0034a83c: ldr r3, [sb]
0034a840: mov r0, sb
0034a844: strb fp, [sb, #0x88]
0034a848: mov lr, pc
0034a84c: ldr pc, [r3, #0x2c]
0034a850: ldr r0, [sp, #0xc]
0034a854: mov r1, sb
0034a858: bl #0x33dd2c
0034a85c: ldr r0, [sp, #0xc]
0034a860: mov r1, fp
0034a864: bl #0x33fdc0
0034a868: cmp r0, #0
0034a86c: beq #0x34a8cc
0034a870: ldrb r3, [sb, #0x88]
0034a874: ldrb r2, [sb, #0x89]
0034a878: cmp r2, r3
0034a87c: beq #0x34a8cc
0034a880: cmp r3, #0
0034a884: strb r3, [sb, #0x89]
0034a888: bne #0x34aad4
0034a88c: mov r1, sb
0034a890: ldr r0, [sp, #0x10]
0034a894: bl #0x33dd2c
0034a898: ldr r0, [sp, #0x10]
0034a89c: bl #0x33ff54
0034a8a0: mov fp, r0
0034a8a4: ldr r0, [r5, #0x70]
0034a8a8: cmp r6, r0
0034a8ac: beq #0x34a8cc
0034a8b0: ldr r3, [r0, #8]
0034a8b4: ldr sb, [r0]
0034a8b8: cmp fp, r3
0034a8bc: beq #0x34aab8
0034a8c0: mov r0, sb
0034a8c4: cmp r6, r0
0034a8c8: bne #0x34a8b0
0034a8cc: ldr sb, [r4]
0034a8d0: mov r4, sb
0034a8d4: cmp r8, r4
0034a8d8: bne #0x34a7f4
0034a8dc: ldr r5, [pc, #0x320]
0034a8e0: add r4, sp, #0x94
0034a8e4: ldr r6, [r7, r5]
0034a8e8: mov r0, r6
0034a8ec: bl #0x337888
0034a8f0: ldr r1, [pc, #0x310]
0034a8f4: add r2, sp, #0x78
0034a8f8: mov r0, r4
0034a8fc: add r1, pc, r1
0034a900: bl #0x3140ec
0034a904: mov r0, r6
0034a908: mov r1, r4
0034a90c: mov r2, #0
0034a910: bl #0x337ddc
0034a914: ldr r0, [sp, #0xa8]
0034a918: cmp r0, r4
0034a91c: beq #0x34a93c
0034a920: cmp r0, #0
0034a924: beq #0x34a93c
0034a928: ldr r1, [sp, #0x94]
0034a92c: rsb r1, r0, r1
0034a930: cmp r1, #0x80
0034a934: bhi #0x34abdc
0034a938: bl #0x708f00
0034a93c: ldr r5, [r7, r5]
0034a940: add r4, sp, #0x7c
0034a944: mov r0, r5
0034a948: bl #0x337888
0034a94c: ldr r1, [pc, #0x2b8]
0034a950: add r2, sp, #0x74
0034a954: mov r0, r4
0034a958: add r1, pc, r1
0034a95c: bl #0x3140ec
0034a960: mov r0, r5
0034a964: mov r1, r4
0034a968: mov r2, #0
0034a96c: bl #0x337ddc
0034a970: ldr r0, [sp, #0x90]
0034a974: cmp r0, r4
0034a978: beq #0x34a998
0034a97c: cmp r0, #0
0034a980: beq #0x34a998
0034a984: ldr r1, [sp, #0x7c]
0034a988: rsb r1, r0, r1
0034a98c: cmp r1, #0x80
0034a990: bhi #0x34abe4
0034a994: bl #0x708f00
0034a998: ldr r0, [pc, #0x270]
0034a99c: add r0, pc, r0
0034a9a0: bl #0x3136b8
0034a9a4: ldr r3, [r7, sl]
0034a9a8: ldr r2, [sp, #0xac]
0034a9ac: ldr r3, [r3]
0034a9b0: cmp r2, r3
0034a9b4: bne #0x34abec
0034a9b8: add sp, sp, #0xb4
0034a9bc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034a9c0: ldrb r3, [r4, #0xd0]
0034a9c4: cmp r3, #0
0034a9c8: bne #0x34a7a4
0034a9cc: ldr r3, [r4, #0xcc]
0034a9d0: cmp r3, #0
0034a9d4: bne #0x34a78c
0034a9d8: ldr r6, [r6]
0034a9dc: b #0x34a7a8
0034a9e0: mov r1, sb
0034a9e4: mov r0, r5
0034a9e8: bl #0x3432f8
0034a9ec: ldr sb, [r4]
0034a9f0: ldr r3, [r4, #4]
0034a9f4: mov r0, r4
0034a9f8: mov r1, #0xc
0034a9fc: str sb, [r3]
0034aa00: str r3, [sb, #4]
0034aa04: bl #0x708f00
0034aa08: mov r4, sb
0034aa0c: b #0x34a8d4
0034aa10: bl #0x7fd794
0034aa14: ldrb r3, [r0, #5]
0034aa18: cmp r3, #0
0034aa1c: bne #0x34aa70
0034aa20: mov r2, #0
0034aa24: strb r2, [sb, #0x86]
0034aa28: ldr r3, [sb]
0034aa2c: mov r0, sb
0034aa30: mov lr, pc
0034aa34: ldr pc, [r3, #0x24]
0034aa38: cmp r0, #0
0034aa3c: beq #0x34a8cc
0034aa40: ldr ip, [sp, #0x18]
0034aa44: mov r0, sb
0034aa48: ldr r1, [sp, #0x1c]
0034aa4c: ldr r3, [r7, ip]
0034aa50: mov r2, #0
0034aa54: str r3, [sp, #0x60]
0034aa58: bl #0x3a7b24
0034aa5c: ldr sb, [r4]
0034aa60: b #0x34a8d0
0034aa64: mov r0, sb
0034aa68: bl #0x3a4344
0034aa6c: b #0x34a818
0034aa70: ldr r3, [sb]
0034aa74: mov r0, sb
0034aa78: mov lr, pc
0034aa7c: ldr pc, [r3, #0x54]
0034aa80: cmp r0, #0
0034aa84: beq #0x34aa20
0034aa88: b #0x34a830
0034aa8c: ldrb r3, [r0, #0x198]
0034aa90: cmp r3, #0
0034aa94: bne #0x34a68c
0034aa98: bl #0x7fd794
0034aa9c: ldrb r3, [r0, #5]
0034aaa0: cmp r3, #0
0034aaa4: bne #0x34a68c
0034aaa8: ldr r0, [pc, #0x164]
0034aaac: add r0, pc, r0
0034aab0: bl #0x3136b8
0034aab4: b #0x34a9a4
0034aab8: ldr r3, [r0, #4]
0034aabc: mov r1, #0xc
0034aac0: str sb, [r3]
0034aac4: str r3, [sb, #4]
0034aac8: bl #0x708f00
0034aacc: mov r0, sb
0034aad0: b #0x34a8c4
0034aad4: mov r1, sb
0034aad8: ldr r0, [sp, #0x14]
0034aadc: bl #0x33dd2c
0034aae0: ldr r0, [sp, #0x14]
0034aae4: bl #0x33ff54
0034aae8: mov sb, r0
0034aaec: mov r0, r6
0034aaf0: bl #0x342690
0034aaf4: str sb, [r0, #8]
0034aaf8: ldr r3, [r5, #0x74]
0034aafc: str r6, [r0]
0034ab00: str r3, [r0, #4]
0034ab04: str r0, [r3]
0034ab08: str r0, [r5, #0x74]
0034ab0c: ldr sb, [r4]
0034ab10: b #0x34a8d0
0034ab14: ldrb r3, [r1, #0x82]
0034ab18: cmp r3, #0
0034ab1c: beq #0x34ab30
0034ab20: sub r3, r3, #1
0034ab24: strb r3, [r1, #0x82]
0034ab28: ldr fp, [r4]
0034ab2c: b #0x34a754
0034ab30: mov r0, r5
0034ab34: bl #0x347dec
0034ab38: cmp r0, #0
0034ab3c: bne #0x34ab98
0034ab40: ldr r3, [r4, #8]
0034ab44: mov r0, r3
0034ab48: ldr r3, [r3]
0034ab4c: mov lr, pc
0034ab50: ldr pc, [r3, #0x24]
0034ab54: cmp r0, #0
0034ab58: bne #0x34aba0
0034ab5c: ldr r1, [r4, #8]
0034ab60: mov r0, sb
0034ab64: bl #0x33f524
0034ab68: ldm sb, {r1, r2, r3}
0034ab6c: mov r0, r5
0034ab70: bl #0x348ea4
0034ab74: ldr fp, [r4]
0034ab78: ldr r3, [r4, #4]
0034ab7c: mov r0, r4
0034ab80: mov r1, #0xc
0034ab84: str fp, [r3]
0034ab88: str r3, [fp, #4]
0034ab8c: bl #0x708f00
0034ab90: mov r4, fp
0034ab94: b #0x34a758
0034ab98: ldr r1, [r4, #8]
0034ab9c: b #0x34a744
0034aba0: ldr r3, [r4, #8]
0034aba4: mov r0, r3
0034aba8: ldr r3, [r3]
0034abac: mov lr, pc
0034abb0: ldr pc, [r3, #0x28]
0034abb4: cmp r0, #0
0034abb8: beq #0x34ab5c
0034abbc: ldr r1, [r4, #8]
0034abc0: ldr r0, [sp, #0xc]
0034abc4: bl #0x33f524
0034abc8: ldr ip, [sp, #0xc]
0034abcc: mov r0, r5
0034abd0: ldm ip, {r1, r2, r3}
0034abd4: bl #0x349240
0034abd8: b #0x34ab74
0034abdc: bl #0x310440
0034abe0: b #0x34a93c
0034abe4: bl #0x310440
0034abe8: b #0x34a998
0034abec: bl #0x30e310
0034abf0: rsbeq sl, r4, r0, ror #8
0034abf4: andeq r4, r0, ip, lsr #1
0034abf8: subseq r5, r7, ip, lsr sp
0034abfc: strdeq r3, r4, [r0], -r4
0034ac00: andeq r1, r0, r4, lsr r1
0034ac04: andeq r0, r0, r4, lsl #17

# 0x34ac18 _ZN10ObjectBase7SetNameEPKc
0034ac18: push {r4, r5, r6, lr}
0034ac1c: mov r6, r0
0034ac20: mov r0, r1
0034ac24: mov r4, r1
0034ac28: bl #0x30de54
0034ac2c: ldr r5, [pc, #0x60]
0034ac30: add r2, r4, r0
0034ac34: mov r1, r4
0034ac38: add r0, r6, #0x30
0034ac3c: bl #0x3109e0
0034ac40: ldr r3, [pc, #0x50]
0034ac44: add r5, pc, r5
0034ac48: ldr r1, [r6, #0x2c]
0034ac4c: ldr r3, [r5, r3]
0034ac50: ldr r0, [r3, #0x38]
0034ac54: add r0, r0, #0xc
0034ac58: bl #0x33fc88
0034ac5c: cmp r4, #0
0034ac60: mov r5, r0
0034ac64: beq #0x34ac84
0034ac68: mov r0, r4
0034ac6c: bl #0x30de54
0034ac70: add r2, r4, r0
0034ac74: mov r0, r5
0034ac78: mov r1, r4
0034ac7c: pop {r4, r5, r6, lr}
0034ac80: b #0x3109e0
0034ac84: ldr r2, [pc, #0x10]
0034ac88: add r2, pc, r2
0034ac8c: mov r4, r2
0034ac90: b #0x34ac74
0034ac94: rsbeq sb, r4, ip, asr #28
0034ac98: strdeq r3, r4, [r0], -r4
0034ac9c: subseq r0, r8, r0, lsl #23

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

# 0x34b064 _ZN13ObjectManager22GetHighestThreatPlayerEPKcib
0034b064: push {r4, r5, r6, r7, r8, lr}
0034b068: ldr r4, [pc, #0xf0]
0034b06c: ldr r5, [pc, #0xf0]
0034b070: sub sp, sp, #0x38
0034b074: add r4, pc, r4
0034b078: ldr lr, [r4, r5]
0034b07c: add r6, sp, #0xc
0034b080: mov ip, #0
0034b084: ldr lr, [lr]
0034b088: mov r7, r0
0034b08c: mov r0, r6
0034b090: str ip, [sp, #4]
0034b094: str lr, [sp, #0x34]
0034b098: str ip, [sp]
0034b09c: mov r8, r1
0034b0a0: bl #0x34aca0
0034b0a4: mov r0, r6
0034b0a8: bl #0x33ff54
0034b0ac: add r0, r0, #0x3c8
0034b0b0: bl #0x3d4a18
0034b0b4: subs r2, r0, #0
0034b0b8: beq #0x34b0e8
0034b0bc: mov r1, r8
0034b0c0: mov r0, r7
0034b0c4: bl #0x340c54
0034b0c8: ldr r3, [r4, r5]
0034b0cc: ldr r2, [sp, #0x34]
0034b0d0: mov r0, r7
0034b0d4: ldr r3, [r3]
0034b0d8: cmp r2, r3
0034b0dc: bne #0x34b15c
0034b0e0: add sp, sp, #0x38
0034b0e4: pop {r4, r5, r6, r7, r8, pc}
0034b0e8: ldr r3, [pc, #0x78]
0034b0ec: add r6, sp, #0x1c
0034b0f0: ldr r8, [r4, r3]
0034b0f4: mov r0, r8
0034b0f8: bl #0x337888
0034b0fc: ldr r1, [pc, #0x68]
0034b100: add r2, sp, #0x18
0034b104: mov r0, r6
0034b108: add r1, pc, r1
0034b10c: bl #0x3140ec
0034b110: mov r0, r8
0034b114: mov r1, r6
0034b118: bl #0x337a88
0034b11c: ldr r0, [sp, #0x30]
0034b120: cmp r0, r6
0034b124: beq #0x34b144
0034b128: cmp r0, #0
0034b12c: beq #0x34b144
0034b130: ldr r1, [sp, #0x1c]
0034b134: rsb r1, r0, r1
0034b138: cmp r1, #0x80
0034b13c: bhi #0x34b154
0034b140: bl #0x708f00
0034b144: mov r0, r7
0034b148: mov r1, #0
0034b14c: bl #0x33f524
0034b150: b #0x34b0c8
0034b154: bl #0x310440
0034b158: b #0x34b144
0034b15c: bl #0x30e310
0034b160: rsbeq sb, r4, ip, lsl sl
0034b164: andeq r4, r0, ip, lsr #1
0034b168: andeq r0, r0, r4, lsl #17
0034b16c: subseq r5, r7, r8, lsl #6

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

# 0x34b520 _ZN13ObjectManager12GetNewObjectEPKcS1_ib
0034b520: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b524: sub sp, sp, #0x64
0034b528: str r3, [sp, #0x18]
0034b52c: ldr r3, [pc, #0x1c8]
0034b530: ldr r7, [pc, #0x1c8]
0034b534: ldr sl, [pc, #0x1c8]
0034b538: add r3, pc, r3
0034b53c: str r3, [sp, #0x24]
0034b540: ldr r3, [pc, #0x1c0]
0034b544: add r7, pc, r7
0034b548: ldrb lr, [sp, #0x8c]
0034b54c: ldr ip, [r7, sl]
0034b550: add r3, pc, r3
0034b554: str r3, [sp, #0x28]
0034b558: ldr r3, [pc, #0x1ac]
0034b55c: ldr ip, [ip]
0034b560: str lr, [sp, #0x1c]
0034b564: ldr r5, [pc, #0x1a4]
0034b568: ldr lr, [pc, #0x1a4]
0034b56c: add r3, pc, r3
0034b570: ldr sb, [pc, #0x1a0]
0034b574: str lr, [sp, #0x20]
0034b578: str ip, [sp, #0x5c]
0034b57c: mov r8, r0
0034b580: str r1, [sp, #0x14]
0034b584: mov r6, r2
0034b588: add r5, pc, r5
0034b58c: str r3, [sp, #0x2c]
0034b590: mov r4, #0
0034b594: b #0x34b5a4
0034b598: add r4, r4, #8
0034b59c: cmp r4, #0x108
0034b5a0: beq #0x34b668
0034b5a4: ldr fp, [r5, r4]
0034b5a8: mov r0, r6
0034b5ac: mov r1, fp
0034b5b0: bl #0x30e31c
0034b5b4: cmp r0, #0
0034b5b8: bne #0x34b598
0034b5bc: add r3, r5, r4
0034b5c0: mov lr, pc
0034b5c4: ldr pc, [r3, #4]
0034b5c8: cmp r0, #0
0034b5cc: beq #0x34b61c
0034b5d0: str fp, [r0, #0x20]
0034b5d4: ldr ip, [sp, #0x88]
0034b5d8: mov r2, r0
0034b5dc: ldr r1, [sp, #0x14]
0034b5e0: str ip, [sp, #4]
0034b5e4: ldr ip, [sp, #0x1c]
0034b5e8: ldr r3, [sp, #0x18]
0034b5ec: mov r0, r8
0034b5f0: str r6, [sp]
0034b5f4: str ip, [sp, #8]
0034b5f8: bl #0x34b270
0034b5fc: ldr r3, [r7, sl]
0034b600: ldr r2, [sp, #0x5c]
0034b604: mov r0, r8
0034b608: ldr r3, [r3]
0034b60c: cmp r2, r3
0034b610: bne #0x34b6f8
0034b614: add sp, sp, #0x64
0034b618: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034b61c: ldr r3, [r7, sb]
0034b620: ldr r3, [r3]
0034b624: cmp r3, #2
0034b628: streq r0, [r0]
0034b62c: beq #0x34b598
0034b630: cmp r3, #1
0034b634: bne #0x34b598
0034b638: ldr r3, [sp, #0x20]
0034b63c: movw ip, #0x51e
0034b640: ldr r1, [sp, #0x24]
0034b644: ldr r0, [r7, r3]
0034b648: ldr r2, [sp, #0x28]
0034b64c: ldr r3, [sp, #0x2c]
0034b650: add r0, r0, #0xa8
0034b654: add r4, r4, #8
0034b658: str ip, [sp]
0034b65c: bl #0x30e004
0034b660: cmp r4, #0x108
0034b664: bne #0x34b5a4
0034b668: ldr r3, [pc, #0xac]
0034b66c: add r4, sp, #0x44
0034b670: ldr r5, [r7, r3]
0034b674: mov r0, r5
0034b678: bl #0x337888
0034b67c: ldr r1, [pc, #0x9c]
0034b680: add r2, sp, #0x40
0034b684: mov r0, r4
0034b688: add r1, pc, r1
0034b68c: bl #0x3140ec
0034b690: mov r0, r5
0034b694: mov r1, r4
0034b698: bl #0x337a88
0034b69c: ldr r0, [sp, #0x58]
0034b6a0: cmp r0, r4
0034b6a4: beq #0x34b6c4
0034b6a8: cmp r0, #0
0034b6ac: beq #0x34b6c4
0034b6b0: ldr r1, [sp, #0x44]
0034b6b4: rsb r1, r0, r1
0034b6b8: cmp r1, #0x80
0034b6bc: bhi #0x34b6f0
0034b6c0: bl #0x708f00
0034b6c4: add r4, sp, #0x34
0034b6c8: mov r0, r4
0034b6cc: bl #0x33f50c
0034b6d0: ldr r0, [sp, #0x34]
0034b6d4: ldr r1, [r4, #8]
0034b6d8: ldr r2, [sp, #0x38]
0034b6dc: mov r3, r8
0034b6e0: str r0, [r3], #4
0034b6e4: str r1, [r3, #4]
0034b6e8: str r2, [r8, #4]
0034b6ec: b #0x34b5fc
0034b6f0: bl #0x310440
0034b6f4: b #0x34b6c4
0034b6f8: bl #0x30e310
0034b6fc: subseq r2, r7, r0, lsr #29
0034b700: rsbeq sb, r4, ip, asr #10
0034b704: andeq r4, r0, ip, lsr #1
0034b708: subseq r4, r7, r0, asr #25
0034b70c: subseq r4, r7, ip, lsr #26
0034b710: rsbeq r1, r1, r0, ror r2
0034b714: andeq r1, r0, r0, asr #19
0034b718: andeq r3, r0, r0, asr #19
0034b71c: andeq r0, r0, r4, lsl #17
0034b720: subseq r4, r7, r0, ror sp

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

# 0x34b868 _ZN13ObjectManager11LoadFromXMLEP12TiXmlElementPKcRK7Point3DIfEi
0034b868: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b86c: ldr r4, [pc, #0x338]
0034b870: ldr r5, [pc, #0x338]
0034b874: subs r7, r1, #0
0034b878: add r4, pc, r4
0034b87c: ldr r1, [r4, r5]
0034b880: mov sl, r2
0034b884: sub sp, sp, #0x25c
0034b888: ldr r2, [r1]
0034b88c: mov fp, r0
0034b890: str r3, [sp, #0xc]
0034b894: str r2, [sp, #0x254]
0034b898: beq #0x34bb30
0034b89c: ldr r1, [pc, #0x310]
0034b8a0: mov r0, r7
0034b8a4: add r1, pc, r1
0034b8a8: bl #0x514c70
0034b8ac: ldr r1, [pc, #0x304]
0034b8b0: mov r8, r0
0034b8b4: mov r0, r7
0034b8b8: add r1, pc, r1
0034b8bc: bl #0x514c70
0034b8c0: subs sb, r0, #0
0034b8c4: beq #0x34badc
0034b8c8: cmp r8, #0
0034b8cc: beq #0x34badc
0034b8d0: add r6, sp, #0x2c
0034b8d4: mov r0, r6
0034b8d8: bl #0x33f50c
0034b8dc: cmp sl, #0
0034b8e0: beq #0x34b8f8
0034b8e4: mov r0, sl
0034b8e8: mov r1, r8
0034b8ec: bl #0x30e31c
0034b8f0: cmp r0, #0
0034b8f4: bne #0x34badc
0034b8f8: ldr r1, [pc, #0x2bc]
0034b8fc: mov r0, sb
0034b900: add r1, pc, r1
0034b904: bl #0x30e31c
0034b908: cmp r0, #0
0034b90c: beq #0x34baf8
0034b910: add sl, sp, #0x13c
0034b914: mov r1, sb
0034b918: mov r0, sl
0034b91c: bl #0x30eae4
0034b920: ldr r1, [pc, #0x298]
0034b924: mov r2, sl
0034b928: add r0, sp, #0x3c
0034b92c: add r1, pc, r1
0034b930: mov r3, #0x53
0034b934: bl #0x30eae4
0034b938: ldr ip, [sp, #0x280]
0034b93c: add sl, sp, #0x10
0034b940: mov r3, sb
0034b944: mov r1, fp
0034b948: mov r0, sl
0034b94c: mov r2, r8
0034b950: mov sb, #1
0034b954: str ip, [sp]
0034b958: str sb, [sp, #4]
0034b95c: bl #0x34b520
0034b960: ldr r1, [sp, #0x14]
0034b964: add r3, r6, #4
0034b968: ldr r2, [sp, #0x10]
0034b96c: str r1, [r3], #4
0034b970: ldr ip, [sl, #8]
0034b974: add r6, sp, #0x2c
0034b978: mov r0, r6
0034b97c: mov r1, #0
0034b980: str ip, [r3]
0034b984: str r2, [sp, #0x2c]
0034b988: bl #0x33fdc0
0034b98c: cmp r0, #0
0034b990: beq #0x34badc
0034b994: mov r1, sb
0034b998: mov r0, r6
0034b99c: bl #0x33fdc0
0034b9a0: add r0, r0, #4
0034b9a4: bl #0x513d78
0034b9a8: ldr r1, [pc, #0x214]
0034b9ac: mov r0, r7
0034b9b0: add r1, pc, r1
0034b9b4: bl #0x514c70
0034b9b8: subs fp, r0, #0
0034b9bc: beq #0x34ba18
0034b9c0: mov r1, sb
0034b9c4: mov r0, r6
0034b9c8: bl #0x33fdc0
0034b9cc: add sb, sp, #0x23c
0034b9d0: add sl, r0, #4
0034b9d4: mov r1, fp
0034b9d8: add r2, sp, #0x38
0034b9dc: mov r0, sb
0034b9e0: bl #0x3140ec
0034b9e4: mov r0, sl
0034b9e8: mov r1, sb
0034b9ec: bl #0x513fec
0034b9f0: ldr r0, [sp, #0x250]
0034b9f4: cmp r0, sb
0034b9f8: beq #0x34ba18
0034b9fc: cmp r0, #0
0034ba00: beq #0x34ba18
0034ba04: ldr r1, [sp, #0x23c]
0034ba08: rsb r1, r0, r1
0034ba0c: cmp r1, #0x80
0034ba10: bhi #0x34bba0
0034ba14: bl #0x708f00
0034ba18: mov r1, #1
0034ba1c: mov r0, r6
0034ba20: bl #0x33fdc0
0034ba24: add r0, r0, #4
0034ba28: bl #0x5136ec
0034ba2c: mov r1, #1
0034ba30: mov r0, r6
0034ba34: bl #0x33fdc0
0034ba38: mov r1, r7
0034ba3c: add r0, r0, #4
0034ba40: bl #0x513a00
0034ba44: ldr r1, [pc, #0x17c]
0034ba48: mov r0, r8
0034ba4c: add r1, pc, r1
0034ba50: bl #0x30e31c
0034ba54: cmp r0, #0
0034ba58: beq #0x34bb84
0034ba5c: mov r1, #1
0034ba60: mov r0, r6
0034ba64: bl #0x33fdc0
0034ba68: ldr r3, [r0]
0034ba6c: mov lr, pc
0034ba70: ldr pc, [r3, #0x20]
0034ba74: cmp r0, #0
0034ba78: beq #0x34badc
0034ba7c: mov r0, r6
0034ba80: bl #0x33fee4
0034ba84: ldr r3, [sp, #0xc]
0034ba88: mov r8, r0
0034ba8c: ldr r0, [r0, #0x164]
0034ba90: ldr r1, [r3, #4]
0034ba94: bl #0x30eba4
0034ba98: ldr ip, [sp, #0xc]
0034ba9c: mov r7, r0
0034baa0: ldr r0, [r8, #0x168]
0034baa4: ldr r1, [ip, #8]
0034baa8: bl #0x30eba4
0034baac: ldr r3, [sp, #0xc]
0034bab0: mov r6, r0
0034bab4: ldr r0, [r8, #0x160]
0034bab8: ldr r1, [r3]
0034babc: bl #0x30eba4
0034bac0: add r1, sp, #0x20
0034bac4: str r0, [sp, #0x20]
0034bac8: mov r2, #1
0034bacc: mov r0, r8
0034bad0: str r7, [sp, #0x24]
0034bad4: str r6, [sp, #0x28]
0034bad8: bl #0x393db4
0034badc: ldr r3, [r4, r5]
0034bae0: ldr r2, [sp, #0x254]
0034bae4: ldr r3, [r3]
0034bae8: cmp r2, r3
0034baec: bne #0x34bba8
0034baf0: add sp, sp, #0x25c
0034baf4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034baf8: add sl, sp, #0x13c
0034bafc: mov r1, sb
0034bb00: mov r0, sl
0034bb04: bl #0x30eae4
0034bb08: ldr r1, [pc, #0xbc]
0034bb0c: add sb, sp, #0x3c
0034bb10: mov r2, sl
0034bb14: add r1, pc, r1
0034bb18: mov r0, sb
0034bb1c: mov r3, #0x53
0034bb20: bl #0x30eae4
0034bb24: mvn ip, #0
0034bb28: str ip, [sp, #0x280]
0034bb2c: b #0x34b938
0034bb30: ldr r3, [pc, #0x98]
0034bb34: ldr r3, [r4, r3]
0034bb38: ldr r3, [r3]
0034bb3c: cmp r3, #2
0034bb40: streq r7, [r7]
0034bb44: beq #0x34badc
0034bb48: cmp r3, #1
0034bb4c: bne #0x34badc
0034bb50: ldr r0, [pc, #0x7c]
0034bb54: ldr r1, [pc, #0x7c]
0034bb58: ldr r2, [pc, #0x7c]
0034bb5c: ldr r0, [r4, r0]
0034bb60: ldr r3, [pc, #0x78]
0034bb64: mov ip, #0x20c
0034bb68: add r1, pc, r1
0034bb6c: add r2, pc, r2
0034bb70: add r3, pc, r3
0034bb74: add r0, r0, #0xa8
0034bb78: str ip, [sp]
0034bb7c: bl #0x30e004
0034bb80: b #0x34badc
0034bb84: mov r0, r6
0034bb88: mov r1, #1
0034bb8c: bl #0x33fdc0
0034bb90: ldr r3, [r0]
0034bb94: mov lr, pc
0034bb98: ldr pc, [r3, #0x1c]
0034bb9c: b #0x34ba5c
0034bba0: bl #0x310440
0034bba4: b #0x34ba18
0034bba8: bl #0x30e310
0034bbac: rsbeq sb, r4, r8, lsl r2
0034bbb0: andeq r4, r0, ip, lsr #1
0034bbb4: ldrsbeq r4, [r7], #-0x84
0034bbb8: subseq r5, sb, r0, lsr r8
0034bbbc: subseq r4, r7, r0, asr #22
0034bbc0: subseq r4, r7, ip, lsr #22
0034bbc4: ldrheq r4, [r7], #-0xa0
0034bbc8: subseq r4, r7, r4, lsr #20
0034bbcc: subseq r4, r7, r4, asr #18
0034bbd0: andeq r3, r0, r0, asr #19
0034bbd4: andeq r1, r0, r0, asr #19
0034bbd8: subseq r2, r7, r0, ror r8
0034bbdc: subseq r4, r7, ip, asr #17
0034bbe0: subseq r4, r7, r8, lsr #14

# 0x34bbe4 _ZN13ObjectManager11LoadFromXMLEP12TiXmlElementPKc
0034bbe4: str lr, [sp, #-4]!
0034bbe8: sub sp, sp, #0x1c
0034bbec: mov ip, #0
0034bbf0: mvn lr, #0
0034bbf4: add r3, sp, #0xc
0034bbf8: str ip, [sp, #0x14]
0034bbfc: str lr, [sp]
0034bc00: str ip, [sp, #0xc]
0034bc04: str ip, [sp, #0x10]
0034bc08: bl #0x34b868
0034bc0c: add sp, sp, #0x1c
0034bc10: ldm sp!, {pc}

# 0x387d84 _ZN9BillboardC2EN10ObjectBase6GO_IDSE
00387d84: push {r4, r5, r6, lr}
00387d88: ldr r5, [pc, #0xa8]
00387d8c: mov r4, r0
00387d90: bl #0x33f310
00387d94: ldr r3, [pc, #0xa0]
00387d98: add r5, pc, r5
00387d9c: add r2, r4, #0x120
00387da0: ldr r3, [r5, r3]
00387da4: mov r0, r2
00387da8: str r2, [r4, #0x130]
00387dac: add ip, r3, #8
00387db0: add r1, r3, #0x78
00387db4: add r3, r3, #0x6c
00387db8: str ip, [r4]
00387dbc: str r3, [r4, #4]
00387dc0: str r1, [r4, #0x24]
00387dc4: str r2, [r4, #0x134]
00387dc8: mov r1, #0x10
00387dcc: bl #0x31167c
00387dd0: ldr r2, [r4, #0x130]
00387dd4: mov r5, #0
00387dd8: add r3, r4, #0x138
00387ddc: strb r5, [r2]
00387de0: mov r0, r3
00387de4: str r3, [r4, #0x148]
00387de8: str r3, [r4, #0x14c]
00387dec: mov r1, #0x10
00387df0: bl #0x31167c
00387df4: ldr r2, [r4, #0x148]
00387df8: mov r3, #0
00387dfc: add r0, r4, #0x168
00387e00: strb r5, [r2]
00387e04: str r3, [r4, #0x160]
00387e08: str r3, [r4, #0x150]
00387e0c: str r3, [r4, #0x154]
00387e10: str r3, [r4, #0x158]
00387e14: str r3, [r4, #0x15c]
00387e18: strb r5, [r4, #0x164]
00387e1c: bl #0x33f50c
00387e20: mov r3, #1
00387e24: str r5, [r4, #0x178]
00387e28: strb r3, [r4, #0x85]
00387e2c: strb r5, [r4, #0x174]
00387e30: mov r0, r4
00387e34: pop {r4, r5, r6, pc}

# 0x38815c _ZN9BillboardC1EN10ObjectBase6GO_IDSE
0038815c: push {r4, r5, r6, lr}
00388160: ldr r5, [pc, #0xa8]
00388164: mov r4, r0
00388168: bl #0x33f310
0038816c: ldr r3, [pc, #0xa0]
00388170: add r5, pc, r5
00388174: add r2, r4, #0x120
00388178: ldr r3, [r5, r3]
0038817c: mov r0, r2
00388180: str r2, [r4, #0x130]
00388184: add ip, r3, #8
00388188: add r1, r3, #0x78
0038818c: add r3, r3, #0x6c
00388190: str ip, [r4]
00388194: str r3, [r4, #4]
00388198: str r1, [r4, #0x24]
0038819c: str r2, [r4, #0x134]
003881a0: mov r1, #0x10
003881a4: bl #0x31167c
003881a8: ldr r2, [r4, #0x130]
003881ac: mov r5, #0
003881b0: add r3, r4, #0x138
003881b4: strb r5, [r2]
003881b8: mov r0, r3
003881bc: str r3, [r4, #0x148]
003881c0: str r3, [r4, #0x14c]
003881c4: mov r1, #0x10
003881c8: bl #0x31167c
003881cc: ldr r2, [r4, #0x148]
003881d0: mov r3, #0
003881d4: add r0, r4, #0x168
003881d8: strb r5, [r2]
003881dc: str r3, [r4, #0x160]
003881e0: str r3, [r4, #0x150]
003881e4: str r3, [r4, #0x154]
003881e8: str r3, [r4, #0x158]
003881ec: str r3, [r4, #0x15c]
003881f0: strb r5, [r4, #0x164]
003881f4: bl #0x33f50c
003881f8: mov r3, #1
003881fc: str r5, [r4, #0x178]
00388200: strb r3, [r4, #0x85]
00388204: strb r5, [r4, #0x174]
00388208: mov r0, r4
0038820c: pop {r4, r5, r6, pc}
00388210: rsbeq ip, r0, r0, lsr #18
00388214: muleq r0, r4, r0

# 0x3883dc _ZNK13AnimatedDecor10IsAnimatedEv
003883dc: mov r0, #1
003883e0: bx lr

# 0x3884f8 _ZNK13AnimatedDecor9IsZonableEv
003884f8: b #0x38ab60

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

# 0x389894 _ZN11PropertyMap11AddPropertyI7Point3DIfEEEvPKcRT_S5_
00389894: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00389898: mov r7, r0
0038989c: sub sp, sp, #0xc
003898a0: mov r6, r1
003898a4: mov r0, #0x2c
003898a8: mov r1, #0
003898ac: ldr fp, [r3, #8]
003898b0: ldr r8, [r3]
003898b4: ldr sb, [r3, #4]
003898b8: mov sl, r2
003898bc: bl #0x310570
003898c0: ldr r5, [pc, #0x5c]
003898c4: ldr r3, [pc, #0x5c]
003898c8: mov r4, r0
003898cc: add r5, pc, r5
003898d0: ldr r3, [r5, r3]
003898d4: mov r1, r6
003898d8: add r2, sp, #4
003898dc: add r3, r3, #8
003898e0: str r3, [r0], #8
003898e4: bl #0x3140ec
003898e8: ldr r3, [pc, #0x3c]
003898ec: rsb sl, r7, sl
003898f0: str sl, [r4, #4]
003898f4: ldr r3, [r5, r3]
003898f8: str r8, [r4, #0x20]
003898fc: str sb, [r4, #0x24]
00389900: add r3, r3, #8
00389904: str r3, [r4]
00389908: str fp, [r4, #0x28]
0038990c: mov r0, r7
00389910: mov r1, r6
00389914: mov r2, r4
00389918: bl #0x513ce4
0038991c: add sp, sp, #0xc
00389920: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00389924: rsbeq fp, r0, r4, asr #3
00389928: andeq r2, r0, r0, lsr r3
0038992c: andeq r0, r0, r4, asr #22

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

# 0x389fa8 _ZN6ModuleC1EN10ObjectBase6GO_IDSE
00389fa8: push {r4, r5, r6, r7, r8, lr}
00389fac: ldr r6, [pc, #0x13c]
00389fb0: mov r4, r0
00389fb4: bl #0x38c398
00389fb8: ldr r3, [pc, #0x134]
00389fbc: add r6, pc, r6
00389fc0: mov r7, #1
00389fc4: ldr r3, [r6, r3]
00389fc8: add r2, r4, #0x378
00389fcc: mov r0, r2
00389fd0: add ip, r3, #8
00389fd4: add r1, r3, #0xe4
00389fd8: add r3, r3, #0xd8
00389fdc: str ip, [r4]
00389fe0: str r3, [r4, #4]
00389fe4: str r1, [r4, #0x24]
00389fe8: str r2, [r4, #0x388]
00389fec: str r2, [r4, #0x38c]
00389ff0: strb r7, [r4, #0x375]
00389ff4: strb r7, [r4, #0x376]
00389ff8: strb r7, [r4, #0x84]
00389ffc: mov r1, #0x10
0038a000: bl #0x31167c
0038a004: ldr r2, [r4, #0x388]
0038a008: mov r5, #0
0038a00c: add r3, r4, #0x390
0038a010: strb r5, [r2]
0038a014: mov r0, r3
0038a018: str r3, [r4, #0x3a0]
0038a01c: str r3, [r4, #0x3a4]
0038a020: mov r1, #0x10
0038a024: bl #0x31167c
0038a028: ldr r2, [r4, #0x3a0]
0038a02c: add r3, r4, #0x3a8
0038a030: mov r0, r3
0038a034: strb r5, [r2]
0038a038: mov r1, #0x10
0038a03c: str r3, [r4, #0x3b8]
0038a040: str r3, [r4, #0x3bc]
0038a044: bl #0x31167c
0038a048: ldr r2, [r4, #0x3b8]
0038a04c: add r3, r4, #0x3c0
0038a050: mov r0, r3
0038a054: strb r5, [r2]
0038a058: mov r1, #0x10
0038a05c: str r3, [r4, #0x3d0]
0038a060: str r3, [r4, #0x3d4]
0038a064: bl #0x31167c
0038a068: ldr r2, [r4, #0x3d0]
0038a06c: add r3, r4, #0x3d8
0038a070: mov r0, r3
0038a074: strb r5, [r2]
0038a078: mov r1, #0x10
0038a07c: str r3, [r4, #0x3e8]
0038a080: str r3, [r4, #0x3ec]
0038a084: bl #0x31167c
0038a088: ldr r3, [pc, #0x68]
0038a08c: ldr r2, [r4, #0x3e8]
0038a090: add r0, r4, #0x400
0038a094: ldr r3, [r6, r3]
0038a098: strb r5, [r2]
0038a09c: ldr r2, [r3]
0038a0a0: str r2, [r4, #0x3f0]
0038a0a4: ldr r2, [r3, #4]
0038a0a8: str r2, [r4, #0x3f4]
0038a0ac: ldr r3, [r3, #8]
0038a0b0: strb r5, [r4, #0x3fc]
0038a0b4: str r3, [r4, #0x3f8]
0038a0b8: bl #0x33f50c
0038a0bc: ldr r3, [pc, #0x38]
0038a0c0: str r5, [r4, #0x418]
0038a0c4: strb r7, [r4, #0x84]
0038a0c8: ldr r3, [r6, r3]
0038a0cc: str r5, [r4, #0x410]
0038a0d0: str r5, [r4, #0x414]
0038a0d4: strb r7, [r4, #0x28]
0038a0d8: ldr r2, [r3]
0038a0dc: mov r0, r4
0038a0e0: add r1, r2, r7
0038a0e4: str r2, [r4, #0x40c]
0038a0e8: str r1, [r3]
0038a0ec: pop {r4, r5, r6, r7, r8, pc}

# 0x38a100 _ZN6ModuleC2EN10ObjectBase6GO_IDSE
0038a100: push {r4, r5, r6, r7, r8, lr}
0038a104: ldr r6, [pc, #0x13c]
0038a108: mov r4, r0
0038a10c: bl #0x38c398
0038a110: ldr r3, [pc, #0x134]
0038a114: add r6, pc, r6
0038a118: mov r7, #1
0038a11c: ldr r3, [r6, r3]
0038a120: add r2, r4, #0x378
0038a124: mov r0, r2
0038a128: add ip, r3, #8
0038a12c: add r1, r3, #0xe4
0038a130: add r3, r3, #0xd8
0038a134: str ip, [r4]
0038a138: str r3, [r4, #4]
0038a13c: str r1, [r4, #0x24]
0038a140: str r2, [r4, #0x388]
0038a144: str r2, [r4, #0x38c]
0038a148: strb r7, [r4, #0x375]
0038a14c: strb r7, [r4, #0x376]
0038a150: strb r7, [r4, #0x84]
0038a154: mov r1, #0x10
0038a158: bl #0x31167c
0038a15c: ldr r2, [r4, #0x388]
0038a160: mov r5, #0
0038a164: add r3, r4, #0x390
0038a168: strb r5, [r2]
0038a16c: mov r0, r3
0038a170: str r3, [r4, #0x3a0]
0038a174: str r3, [r4, #0x3a4]
0038a178: mov r1, #0x10
0038a17c: bl #0x31167c
0038a180: ldr r2, [r4, #0x3a0]
0038a184: add r3, r4, #0x3a8
0038a188: mov r0, r3
0038a18c: strb r5, [r2]
0038a190: mov r1, #0x10
0038a194: str r3, [r4, #0x3b8]
0038a198: str r3, [r4, #0x3bc]
0038a19c: bl #0x31167c
0038a1a0: ldr r2, [r4, #0x3b8]
0038a1a4: add r3, r4, #0x3c0
0038a1a8: mov r0, r3
0038a1ac: strb r5, [r2]
0038a1b0: mov r1, #0x10
0038a1b4: str r3, [r4, #0x3d0]
0038a1b8: str r3, [r4, #0x3d4]
0038a1bc: bl #0x31167c
0038a1c0: ldr r2, [r4, #0x3d0]
0038a1c4: add r3, r4, #0x3d8
0038a1c8: mov r0, r3
0038a1cc: strb r5, [r2]
0038a1d0: mov r1, #0x10
0038a1d4: str r3, [r4, #0x3e8]
0038a1d8: str r3, [r4, #0x3ec]
0038a1dc: bl #0x31167c
0038a1e0: ldr r3, [pc, #0x68]
0038a1e4: ldr r2, [r4, #0x3e8]
0038a1e8: add r0, r4, #0x400
0038a1ec: ldr r3, [r6, r3]
0038a1f0: strb r5, [r2]
0038a1f4: ldr r2, [r3]
0038a1f8: str r2, [r4, #0x3f0]
0038a1fc: ldr r2, [r3, #4]
0038a200: str r2, [r4, #0x3f4]
0038a204: ldr r3, [r3, #8]
0038a208: strb r5, [r4, #0x3fc]
0038a20c: str r3, [r4, #0x3f8]
0038a210: bl #0x33f50c
0038a214: ldr r3, [pc, #0x38]
0038a218: str r5, [r4, #0x418]
0038a21c: strb r7, [r4, #0x84]
0038a220: ldr r3, [r6, r3]
0038a224: str r5, [r4, #0x410]
0038a228: str r5, [r4, #0x414]
0038a22c: strb r7, [r4, #0x28]
0038a230: ldr r2, [r3]
0038a234: mov r0, r4
0038a238: add r1, r2, r7
0038a23c: str r2, [r4, #0x40c]
0038a240: str r1, [r3]
0038a244: pop {r4, r5, r6, r7, r8, pc}
0038a248: rsbeq sl, r0, ip, ror sb
0038a24c: muleq r0, r0, r1
0038a250: muleq r0, r8, r4
0038a254: andeq r2, r0, ip, lsr #16

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

# 0x38c130 _ZN10GameObjectC1EN10ObjectBase6GO_IDSE
0038c130: push {r4, r5, r6, r7, lr}
0038c134: ldr r6, [pc, #0x254]
0038c138: sub sp, sp, #0xc
0038c13c: mov r4, r0
0038c140: bl #0x33f310
0038c144: ldr r2, [pc, #0x248]
0038c148: add r6, pc, r6
0038c14c: mov r3, #0
0038c150: ldr r2, [r6, r2]
0038c154: mov r5, #0
0038c158: str r3, [r4, #0x120]
0038c15c: add r1, r2, #0xe4
0038c160: add r0, r2, #8
0038c164: add r2, r2, #0xd8
0038c168: str r2, [r4, #4]
0038c16c: str r1, [r4, #0x24]
0038c170: str r0, [r4]
0038c174: str r3, [r4, #0x124]
0038c178: str r3, [r4, #0x128]
0038c17c: str r3, [r4, #0x12c]
0038c180: str r3, [r4, #0x130]
0038c184: str r3, [r4, #0x134]
0038c188: str r3, [r4, #0x138]
0038c18c: str r3, [r4, #0x13c]
0038c190: str r3, [r4, #0x140]
0038c194: str r3, [r4, #0x144]
0038c198: str r3, [r4, #0x148]
0038c19c: str r3, [r4, #0x14c]
0038c1a0: str r3, [r4, #0x150]
0038c1a4: str r3, [r4, #0x154]
0038c1a8: str r3, [r4, #0x158]
0038c1ac: str r3, [r4, #0x160]
0038c1b0: str r3, [r4, #0x164]
0038c1b4: str r3, [r4, #0x168]
0038c1b8: str r3, [r4, #0x16c]
0038c1bc: str r3, [r4, #0x170]
0038c1c0: str r3, [r4, #0x174]
0038c1c4: str r3, [r4, #0x178]
0038c1c8: str r3, [r4, #0x184]
0038c1cc: str r3, [r4, #0x188]
0038c1d0: str r3, [r4, #0x18c]
0038c1d4: str r3, [r4, #0x190]
0038c1d8: str r3, [r4, #0x194]
0038c1dc: strb r5, [r4, #0x15c]
0038c1e0: str r5, [r4, #0x180]
0038c1e4: add r0, r4, #0x1c8
0038c1e8: str r3, [r4, #0x198]
0038c1ec: str r3, [r4, #0x1c0]
0038c1f0: str r3, [r4, #0x19c]
0038c1f4: str r3, [r4, #0x1a0]
0038c1f8: str r3, [r4, #0x1a4]
0038c1fc: str r3, [r4, #0x1a8]
0038c200: str r3, [r4, #0x1ac]
0038c204: str r3, [r4, #0x1b0]
0038c208: strb r5, [r4, #0x1b4]
0038c20c: strb r5, [r4, #0x1b5]
0038c210: str r3, [r4, #0x1b8]
0038c214: str r3, [r4, #0x1bc]
0038c218: strb r5, [r4, #0x1c4]
0038c21c: bl #0x524644
0038c220: add r3, r4, #0x278
0038c224: mvn r7, #0
0038c228: mov r2, #0x64
0038c22c: str r2, [r4, #0x274]
0038c230: mov r0, r3
0038c234: str r3, [r4, #0x288]
0038c238: str r3, [r4, #0x28c]
0038c23c: str r5, [r4, #0x26c]
0038c240: str r7, [r4, #0x270]
0038c244: mov r1, #0x10
0038c248: bl #0x31167c
0038c24c: ldr r2, [r4, #0x288]
0038c250: add r3, r4, #0x290
0038c254: mov r0, r3
0038c258: strb r5, [r2]
0038c25c: mov r1, #0x10
0038c260: str r3, [r4, #0x2a0]
0038c264: str r3, [r4, #0x2a4]
0038c268: bl #0x31167c
0038c26c: ldr r2, [r4, #0x2a0]
0038c270: add r3, r4, #0x2a8
0038c274: mov r0, r3
0038c278: strb r5, [r2]
0038c27c: mov r1, #0x10
0038c280: str r3, [r4, #0x2b8]
0038c284: str r3, [r4, #0x2bc]
0038c288: bl #0x31167c
0038c28c: ldr r2, [r4, #0x2b8]
0038c290: add r3, r4, #0x2c0
0038c294: mov r0, r3
0038c298: strb r5, [r2]
0038c29c: mov r1, #0x10
0038c2a0: str r3, [r4, #0x2d0]
0038c2a4: str r3, [r4, #0x2d4]
0038c2a8: bl #0x31167c
0038c2ac: ldr r3, [r4, #0x2d0]
0038c2b0: mov ip, #1
0038c2b4: add r6, r4, #0x304
0038c2b8: strb r5, [r3]
0038c2bc: mov r2, ip
0038c2c0: strb ip, [r4, #0x2ee]
0038c2c4: strb ip, [r4, #0x2fb]
0038c2c8: str r5, [r4, #0x2d8]
0038c2cc: str r5, [r4, #0x2dc]
0038c2d0: str r5, [r4, #0x2e0]
0038c2d4: str r5, [r4, #0x2e4]
0038c2d8: str r5, [r4, #0x2e8]
0038c2dc: strb r5, [r4, #0x2ec]
0038c2e0: strb r5, [r4, #0x2ed]
0038c2e4: strb r5, [r4, #0x2ef]
0038c2e8: strb r5, [r4, #0x2f0]
0038c2ec: str r5, [r4, #0x2f4]
0038c2f0: strb r5, [r4, #0x2f8]
0038c2f4: strb r5, [r4, #0x2f9]
0038c2f8: strb r5, [r4, #0x2fa]
0038c2fc: strb r5, [r4, #0x2fc]
0038c300: str r5, [r4, #0x300]
0038c304: mov r3, r5
0038c308: mov r1, r5
0038c30c: mov r0, r6
0038c310: str ip, [sp]
0038c314: bl #0x4a2730
0038c318: add r3, r4, #0x358
0038c31c: mov r0, r3
0038c320: str r3, [r4, #0x368]
0038c324: str r3, [r4, #0x36c]
0038c328: mov r1, #0x10
0038c32c: bl #0x31167c
0038c330: ldr r1, [r4, #0x368]
0038c334: mov r2, #0xc2000000
0038c338: mov r3, #0x42000000
0038c33c: strb r5, [r1]
0038c340: add r2, r2, #0xc80000
0038c344: add r3, r3, #0xc80000
0038c348: mov r1, #0x370
0038c34c: strh r7, [r4, r1]
0038c350: mov r0, r4
0038c354: str r2, [r4, #0x14c]
0038c358: str r3, [r4, #0x158]
0038c35c: str r2, [r4, #0x144]
0038c360: str r2, [r4, #0x148]
0038c364: str r3, [r4, #0x150]
0038c368: str r3, [r4, #0x154]
0038c36c: strb r5, [r4, #0x373]
0038c370: strb r5, [r4, #0x372]
0038c374: bl #0x38aac8
0038c378: mov r0, r6
0038c37c: mov r1, r4
0038c380: bl #0x4a191c
0038c384: mov r0, r4
0038c388: add sp, sp, #0xc
0038c38c: pop {r4, r5, r6, r7, pc}
0038c390: rsbeq r8, r0, r8, asr #18
0038c394: andeq r2, r0, r0, ror sp

# 0x38c398 _ZN10GameObjectC2EN10ObjectBase6GO_IDSE
0038c398: push {r4, r5, r6, r7, lr}
0038c39c: ldr r6, [pc, #0x254]
0038c3a0: sub sp, sp, #0xc
0038c3a4: mov r4, r0
0038c3a8: bl #0x33f310
0038c3ac: ldr r2, [pc, #0x248]
0038c3b0: add r6, pc, r6
0038c3b4: mov r3, #0
0038c3b8: ldr r2, [r6, r2]
0038c3bc: mov r5, #0
0038c3c0: str r3, [r4, #0x120]
0038c3c4: add r1, r2, #0xe4
0038c3c8: add r0, r2, #8
0038c3cc: add r2, r2, #0xd8
0038c3d0: str r2, [r4, #4]
0038c3d4: str r1, [r4, #0x24]
0038c3d8: str r0, [r4]
0038c3dc: str r3, [r4, #0x124]
0038c3e0: str r3, [r4, #0x128]
0038c3e4: str r3, [r4, #0x12c]
0038c3e8: str r3, [r4, #0x130]
0038c3ec: str r3, [r4, #0x134]
0038c3f0: str r3, [r4, #0x138]
0038c3f4: str r3, [r4, #0x13c]
0038c3f8: str r3, [r4, #0x140]
0038c3fc: str r3, [r4, #0x144]
0038c400: str r3, [r4, #0x148]
0038c404: str r3, [r4, #0x14c]
0038c408: str r3, [r4, #0x150]
0038c40c: str r3, [r4, #0x154]
0038c410: str r3, [r4, #0x158]
0038c414: str r3, [r4, #0x160]
0038c418: str r3, [r4, #0x164]
0038c41c: str r3, [r4, #0x168]
0038c420: str r3, [r4, #0x16c]
0038c424: str r3, [r4, #0x170]
0038c428: str r3, [r4, #0x174]
0038c42c: str r3, [r4, #0x178]
0038c430: str r3, [r4, #0x184]
0038c434: str r3, [r4, #0x188]
0038c438: str r3, [r4, #0x18c]
0038c43c: str r3, [r4, #0x190]
0038c440: str r3, [r4, #0x194]
0038c444: strb r5, [r4, #0x15c]
0038c448: str r5, [r4, #0x180]
0038c44c: add r0, r4, #0x1c8
0038c450: str r3, [r4, #0x198]
0038c454: str r3, [r4, #0x1c0]
0038c458: str r3, [r4, #0x19c]
0038c45c: str r3, [r4, #0x1a0]
0038c460: str r3, [r4, #0x1a4]
0038c464: str r3, [r4, #0x1a8]
0038c468: str r3, [r4, #0x1ac]
0038c46c: str r3, [r4, #0x1b0]
0038c470: strb r5, [r4, #0x1b4]
0038c474: strb r5, [r4, #0x1b5]
0038c478: str r3, [r4, #0x1b8]
0038c47c: str r3, [r4, #0x1bc]
0038c480: strb r5, [r4, #0x1c4]
0038c484: bl #0x524644
0038c488: add r3, r4, #0x278
0038c48c: mvn r7, #0
0038c490: mov r2, #0x64
0038c494: str r2, [r4, #0x274]
0038c498: mov r0, r3
0038c49c: str r3, [r4, #0x288]
0038c4a0: str r3, [r4, #0x28c]
0038c4a4: str r5, [r4, #0x26c]
0038c4a8: str r7, [r4, #0x270]
0038c4ac: mov r1, #0x10
0038c4b0: bl #0x31167c
0038c4b4: ldr r2, [r4, #0x288]
0038c4b8: add r3, r4, #0x290
0038c4bc: mov r0, r3
0038c4c0: strb r5, [r2]
0038c4c4: mov r1, #0x10
0038c4c8: str r3, [r4, #0x2a0]
0038c4cc: str r3, [r4, #0x2a4]
0038c4d0: bl #0x31167c
0038c4d4: ldr r2, [r4, #0x2a0]
0038c4d8: add r3, r4, #0x2a8
0038c4dc: mov r0, r3
0038c4e0: strb r5, [r2]
0038c4e4: mov r1, #0x10
0038c4e8: str r3, [r4, #0x2b8]
0038c4ec: str r3, [r4, #0x2bc]
0038c4f0: bl #0x31167c
0038c4f4: ldr r2, [r4, #0x2b8]
0038c4f8: add r3, r4, #0x2c0
0038c4fc: mov r0, r3
0038c500: strb r5, [r2]
0038c504: mov r1, #0x10
0038c508: str r3, [r4, #0x2d0]
0038c50c: str r3, [r4, #0x2d4]
0038c510: bl #0x31167c
0038c514: ldr r3, [r4, #0x2d0]
0038c518: mov ip, #1
0038c51c: add r6, r4, #0x304
0038c520: strb r5, [r3]
0038c524: mov r2, ip
0038c528: strb ip, [r4, #0x2ee]
0038c52c: strb ip, [r4, #0x2fb]
0038c530: str r5, [r4, #0x2d8]
0038c534: str r5, [r4, #0x2dc]
0038c538: str r5, [r4, #0x2e0]
0038c53c: str r5, [r4, #0x2e4]
0038c540: str r5, [r4, #0x2e8]
0038c544: strb r5, [r4, #0x2ec]
0038c548: strb r5, [r4, #0x2ed]
0038c54c: strb r5, [r4, #0x2ef]
0038c550: strb r5, [r4, #0x2f0]
0038c554: str r5, [r4, #0x2f4]
0038c558: strb r5, [r4, #0x2f8]
0038c55c: strb r5, [r4, #0x2f9]
0038c560: strb r5, [r4, #0x2fa]
0038c564: strb r5, [r4, #0x2fc]
0038c568: str r5, [r4, #0x300]
0038c56c: mov r3, r5
0038c570: mov r1, r5
0038c574: mov r0, r6
0038c578: str ip, [sp]
0038c57c: bl #0x4a2730
0038c580: add r3, r4, #0x358
0038c584: mov r0, r3
0038c588: str r3, [r4, #0x368]
0038c58c: str r3, [r4, #0x36c]
0038c590: mov r1, #0x10
0038c594: bl #0x31167c
0038c598: ldr r1, [r4, #0x368]
0038c59c: mov r2, #0xc2000000
0038c5a0: mov r3, #0x42000000
0038c5a4: strb r5, [r1]
0038c5a8: add r2, r2, #0xc80000
0038c5ac: add r3, r3, #0xc80000
0038c5b0: mov r1, #0x370
0038c5b4: strh r7, [r4, r1]
0038c5b8: mov r0, r4
0038c5bc: str r2, [r4, #0x14c]
0038c5c0: str r3, [r4, #0x158]
0038c5c4: str r2, [r4, #0x144]
0038c5c8: str r2, [r4, #0x148]
0038c5cc: str r3, [r4, #0x150]
0038c5d0: str r3, [r4, #0x154]
0038c5d4: strb r5, [r4, #0x373]
0038c5d8: strb r5, [r4, #0x372]
0038c5dc: bl #0x38aac8
0038c5e0: mov r0, r6
0038c5e4: mov r1, r4
0038c5e8: bl #0x4a191c
0038c5ec: mov r0, r4
0038c5f0: add sp, sp, #0xc
0038c5f4: pop {r4, r5, r6, r7, pc}
0038c5f8: rsbeq r8, r0, r0, ror #13
0038c5fc: andeq r2, r0, r0, ror sp

# 0x38cee0 _ZThn4_N10GameObject17DeclarePropertiesEv
0038cee0: sub r0, r0, #4
0038cee4: b #0x38cee8

# 0x38cee8 _ZN10GameObject17DeclarePropertiesEv
0038cee8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038ceec: ldr r6, [pc, #0x208]
0038cef0: ldr r3, [pc, #0x208]
0038cef4: sub sp, sp, #0x4c
0038cef8: add r6, pc, r6
0038cefc: ldr sb, [r6, r3]
0038cf00: add r5, r0, #4
0038cf04: mov r4, r0
0038cf08: ldr r3, [sb]
0038cf0c: add fp, r0, #0x274
0038cf10: ldr r8, [pc, #0x1ec]
0038cf14: str r3, [sp, #0x44]
0038cf18: bl #0x33f014
0038cf1c: ldr r3, [pc, #0x1e4]
0038cf20: ldr r1, [pc, #0x1e4]
0038cf24: mov r0, r5
0038cf28: ldr r7, [r6, r3]
0038cf2c: add r1, pc, r1
0038cf30: add r2, r4, #0x160
0038cf34: ldr ip, [r7]
0038cf38: ldr lr, [r7, #4]
0038cf3c: ldr sl, [r7, #8]
0038cf40: add r3, sp, #0x18
0038cf44: str ip, [sp, #0x18]
0038cf48: str lr, [sp, #0x1c]
0038cf4c: str sl, [sp, #0x20]
0038cf50: bl #0x389894
0038cf54: ldr r1, [pc, #0x1b4]
0038cf58: ldr lr, [r7]
0038cf5c: ldr ip, [r7, #4]
0038cf60: ldr sl, [r7, #8]
0038cf64: add r3, sp, #0xc
0038cf68: add r1, pc, r1
0038cf6c: mov r0, r5
0038cf70: add r2, r4, #0x16c
0038cf74: str lr, [sp, #0xc]
0038cf78: str ip, [sp, #0x10]
0038cf7c: str sl, [sp, #0x14]
0038cf80: bl #0x389894
0038cf84: ldr r1, [pc, #0x188]
0038cf88: mov r0, r5
0038cf8c: add r2, r4, #0x290
0038cf90: add r1, pc, r1
0038cf94: bl #0x33ef7c
0038cf98: ldr r1, [pc, #0x178]
0038cf9c: mov r0, r5
0038cfa0: add r2, r4, #0x2a8
0038cfa4: add r1, pc, r1
0038cfa8: bl #0x33ef7c
0038cfac: ldr r1, [pc, #0x168]
0038cfb0: mov r0, r5
0038cfb4: add r2, r4, #0x2c0
0038cfb8: add r1, pc, r1
0038cfbc: bl #0x33ef7c
0038cfc0: ldr r1, [pc, #0x158]
0038cfc4: mov ip, #0x3f800000
0038cfc8: mov r0, r5
0038cfcc: add r1, pc, r1
0038cfd0: add r2, r4, #0x120
0038cfd4: mov r3, sp
0038cfd8: str ip, [sp, #8]
0038cfdc: str ip, [sp]
0038cfe0: str ip, [sp, #4]
0038cfe4: bl #0x389894
0038cfe8: ldr r1, [pc, #0x134]
0038cfec: add r2, r4, #0x2ec
0038cff0: add r2, r2, #1
0038cff4: add r1, pc, r1
0038cff8: mov r0, r5
0038cffc: mov r3, #0
0038d000: bl #0x33e4ac
0038d004: ldr r1, [pc, #0x11c]
0038d008: mov r3, #0
0038d00c: add r2, r4, #0x15c
0038d010: add r1, pc, r1
0038d014: mov r0, r5
0038d018: bl #0x33e4ac
0038d01c: mov r1, #0
0038d020: mov r0, #0x24
0038d024: bl #0x310570
0038d028: ldr r3, [pc, #0xfc]
0038d02c: add r8, pc, r8
0038d030: mov sl, r0
0038d034: ldr r3, [r6, r3]
0038d038: mov r1, r8
0038d03c: add r2, sp, #0x24
0038d040: add r3, r3, #8
0038d044: str r3, [r0], #8
0038d048: bl #0x3140ec
0038d04c: ldr r3, [pc, #0xdc]
0038d050: mov r2, #0x64
0038d054: rsb fp, r5, fp
0038d058: ldr r3, [r6, r3]
0038d05c: str r2, [sl, #0x20]
0038d060: mov r1, r8
0038d064: add r3, r3, #8
0038d068: str r3, [sl]
0038d06c: mov r2, sl
0038d070: mov r0, r5
0038d074: str fp, [sl, #4]
0038d078: bl #0x513ce4
0038d07c: ldr r1, [pc, #0xb0]
0038d080: add r7, sp, #0x2c
0038d084: add r2, sp, #0x28
0038d088: add r1, pc, r1
0038d08c: mov r0, r7
0038d090: bl #0x3140ec
0038d094: ldr r1, [pc, #0x9c]
0038d098: mov r3, r7
0038d09c: add r2, r4, #0x278
0038d0a0: add r1, pc, r1
0038d0a4: mov r0, r5
0038d0a8: bl #0x33e404
0038d0ac: mov r0, r7
0038d0b0: bl #0x318254
0038d0b4: ldr r1, [pc, #0x80]
0038d0b8: mov r0, r5
0038d0bc: add r2, r4, #0x358
0038d0c0: add r1, pc, r1
0038d0c4: bl #0x33ef7c
0038d0c8: ldr r1, [pc, #0x70]
0038d0cc: add r2, r4, #0x60
0038d0d0: mov r3, #1
0038d0d4: mov r0, r5
0038d0d8: add r1, pc, r1
0038d0dc: bl #0x33e4ac
0038d0e0: ldr r2, [sp, #0x44]
0038d0e4: ldr r3, [sb]
0038d0e8: cmp r2, r3
0038d0ec: bne #0x38d0f8
0038d0f0: add sp, sp, #0x4c
0038d0f4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0038d0f8: bl #0x30e310
0038d0fc: mlseq r0, r8, fp, r7
0038d100: andeq r4, r0, ip, lsr #1
0038d104: subseq r5, r3, r4, lsr #9
0038d108: andeq r3, r0, ip, lsr #30
0038d10c: ldrsheq r5, [r3], #-0x2c
0038d110: subseq r5, r3, r0, lsr #10
0038d114: subseq ip, r3, r8, lsl #18
0038d118: ldrsheq r5, [r3], #-0x44
0038d11c: ldrsheq r5, [r3], #-0x40
0038d120: ldrheq r8, [r5], #-0x14
0038d124: ldrheq r5, [r3], #-0x4c
0038d128: ldrheq r5, [r3], #-0x40
0038d12c: andeq r2, r0, r0, lsr r3
0038d130: muleq r0, r0, r5
0038d134: subseq r5, r3, r8, asr r4
0038d138: subseq r5, r3, r0, asr r4
0038d13c: subseq r5, r3, r0, asr #8
0038d140: subseq r5, r3, r8, lsr r4

# 0x38e680 _ZNSt6vectorIP10GameObjectSaIS1_EEC1ERKS3_
0038e680: push {r4, r5, lr}
0038e684: mov r5, r1
0038e688: ldr r3, [r5]
0038e68c: ldr r1, [r1, #4]
0038e690: sub sp, sp, #0xc
0038e694: mov r4, r0
0038e698: rsb r1, r3, r1
0038e69c: mov ip, #0
0038e6a0: asr r1, r1, #2
0038e6a4: add r2, sp, #8
0038e6a8: str r1, [r2, #-4]!
0038e6ac: str ip, [r4]
0038e6b0: str ip, [r4, #4]
0038e6b4: str ip, [r0, #8]!
0038e6b8: bl #0x38e610
0038e6bc: ldr r2, [sp, #4]
0038e6c0: str r0, [r4]
0038e6c4: str r0, [r4, #4]
0038e6c8: add r2, r0, r2, lsl #2
0038e6cc: str r2, [r4, #8]
0038e6d0: ldm r5, {r1, r2}
0038e6d4: mov r3, r0
0038e6d8: cmp r1, r2
0038e6dc: beq #0x38e6f0
0038e6e0: rsb r5, r1, r2
0038e6e4: mov r2, r5
0038e6e8: bl #0x30e868
0038e6ec: add r3, r0, r5
0038e6f0: str r3, [r4, #4]
0038e6f4: mov r0, r4
0038e6f8: add sp, sp, #0xc
0038e6fc: pop {r4, r5, pc}

# 0x38ebf0 _ZN10GameObject8_GetNameERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038ebf0: mov r0, r1
0038ebf4: ldr r1, [r2, #0x44]
0038ebf8: b #0x37c8cc

# 0x39503c _ZN11PropertyMap11AddPropertyIfEEvPKcRT_S3_
0039503c: push {r4, r5, r6, r7, r8, sl, lr}
00395040: mov r6, r0
00395044: sub sp, sp, #0xc
00395048: mov r5, r1
0039504c: mov r0, #0x24
00395050: mov r1, #0
00395054: mov sl, r3
00395058: mov r7, r2
0039505c: bl #0x310570
00395060: ldr r4, [pc, #0x54]
00395064: ldr r3, [pc, #0x54]
00395068: mov r8, r0
0039506c: add r4, pc, r4
00395070: ldr r3, [r4, r3]
00395074: mov r1, r5
00395078: add r2, sp, #4
0039507c: add r3, r3, #8
00395080: str r3, [r0], #8
00395084: bl #0x3140ec
00395088: ldr r3, [pc, #0x34]
0039508c: rsb r7, r6, r7
00395090: str r7, [r8, #4]
00395094: ldr r3, [r4, r3]
00395098: str sl, [r8, #0x20]
0039509c: mov r0, r6
003950a0: add r3, r3, #8
003950a4: str r3, [r8]
003950a8: mov r1, r5
003950ac: mov r2, r8
003950b0: bl #0x513ce4
003950b4: add sp, sp, #0xc
003950b8: pop {r4, r5, r6, r7, r8, sl, pc}
003950bc: subseq pc, pc, r4, lsr #20
003950c0: andeq r2, r0, r0, lsr r3
003950c4: strheq r2, [r0], -ip

# 0x39551c _ZN12SoundEmitterC1EN10ObjectBase6GO_IDSE
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

# 0x3955a8 _ZN12SoundEmitterC2EN10ObjectBase6GO_IDSE
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

# 0x398878 _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_
00398878: push {r4, r5, r6, r7, r8, sl, lr}
0039887c: mov r6, r0
00398880: sub sp, sp, #0xc
00398884: mov r5, r1
00398888: mov r0, #0x24
0039888c: mov r1, #0
00398890: mov sl, r3
00398894: mov r7, r2
00398898: bl #0x310570
0039889c: ldr r4, [pc, #0x54]
003988a0: ldr r3, [pc, #0x54]
003988a4: mov r8, r0
003988a8: add r4, pc, r4
003988ac: ldr r3, [r4, r3]
003988b0: mov r1, r5
003988b4: add r2, sp, #4
003988b8: add r3, r3, #8
003988bc: str r3, [r0], #8
003988c0: bl #0x3140ec
003988c4: ldr r3, [pc, #0x34]
003988c8: rsb r7, r6, r7
003988cc: str r7, [r8, #4]
003988d0: ldr r3, [r4, r3]
003988d4: str sl, [r8, #0x20]
003988d8: mov r0, r6
003988dc: add r3, r3, #8
003988e0: str r3, [r8]
003988e4: mov r1, r5
003988e8: mov r2, r8
003988ec: bl #0x513ce4
003988f0: add sp, sp, #0xc
003988f4: pop {r4, r5, r6, r7, r8, sl, pc}
003988f8: subseq ip, pc, r8, ror #3
003988fc: andeq r2, r0, r0, lsr r3
00398900: muleq r0, r0, r5

# 0x398f44 _ZN7TriggerC1EN10ObjectBase6GO_IDSEbb
00398f44: push {r4, r5, r6, r7, r8, lr}
00398f48: ldr r5, [pc, #0x7c]
00398f4c: mov r4, r0
00398f50: bl #0x397f28
00398f54: ldr r2, [pc, #0x74]
00398f58: add r5, pc, r5
00398f5c: mov r3, #0
00398f60: ldr r2, [r5, r2]
00398f64: mov r6, #1
00398f68: add r8, r4, #0x3c8
00398f6c: add r1, r2, #0xf4
00398f70: add r0, r2, #8
00398f74: add r2, r2, #0xe8
00398f78: str r3, [r4, #0x3c0]
00398f7c: str r3, [r4, #0x3ac]
00398f80: strb r3, [r4, #0x3b0]
00398f84: str r3, [r4, #0x3b4]
00398f88: str r3, [r4, #0x3b8]
00398f8c: strb r3, [r4, #0x3bc]
00398f90: stm r4, {r0, r2}
00398f94: str r1, [r4, #0x24]
00398f98: add r7, r4, #0x570
00398f9c: str r6, [r4, #0x3a8]
00398fa0: mov r0, r8
00398fa4: bl #0x398da4
00398fa8: mov r0, r7
00398fac: bl #0x398da4
00398fb0: mov r3, #4
00398fb4: str r8, [r4, #0x100]
00398fb8: str r7, [r4, #0x104]
00398fbc: strb r6, [r4, #0x28]
00398fc0: strb r3, [r4, #0xf8]
00398fc4: mov r0, r4
00398fc8: pop {r4, r5, r6, r7, r8, pc}
00398fcc: subseq fp, pc, r8, lsr fp
00398fd0: strdeq r1, r2, [r0], -r0

# 0x398fd4 _ZN7TriggerC2EN10ObjectBase6GO_IDSEbb
00398fd4: push {r4, r5, r6, r7, r8, lr}
00398fd8: ldr r5, [pc, #0x7c]
00398fdc: mov r4, r0
00398fe0: bl #0x397f28
00398fe4: ldr r2, [pc, #0x74]
00398fe8: add r5, pc, r5
00398fec: mov r3, #0
00398ff0: ldr r2, [r5, r2]
00398ff4: mov r6, #1
00398ff8: add r8, r4, #0x3c8
00398ffc: add r1, r2, #0xf4
00399000: add r0, r2, #8
00399004: add r2, r2, #0xe8
00399008: str r3, [r4, #0x3c0]
0039900c: str r3, [r4, #0x3ac]
00399010: strb r3, [r4, #0x3b0]
00399014: str r3, [r4, #0x3b4]
00399018: str r3, [r4, #0x3b8]
0039901c: strb r3, [r4, #0x3bc]
00399020: stm r4, {r0, r2}
00399024: str r1, [r4, #0x24]
00399028: add r7, r4, #0x570
0039902c: str r6, [r4, #0x3a8]
00399030: mov r0, r8
00399034: bl #0x398da4
00399038: mov r0, r7
0039903c: bl #0x398da4
00399040: mov r3, #4
00399044: str r8, [r4, #0x100]
00399048: str r7, [r4, #0x104]
0039904c: strb r6, [r4, #0x28]
00399050: strb r3, [r4, #0xf8]
00399054: mov r0, r4
00399058: pop {r4, r5, r6, r7, r8, pc}
0039905c: subseq fp, pc, r8, lsr #21
00399060: strdeq r1, r2, [r0], -r0

# 0x39af04 _ZN12TriggerPlateC1EN10ObjectBase6GO_IDSE
0039af04: push {r4, r5, r6, lr}
0039af08: mov r2, #1
0039af0c: mov r3, #0
0039af10: ldr r5, [pc, #0xc0]
0039af14: mov r4, r0
0039af18: bl #0x398fd4
0039af1c: ldr r2, [pc, #0xb8]
0039af20: add r5, pc, r5
0039af24: add r3, r4, #0x710
0039af28: ldr r2, [r5, r2]
0039af2c: add r3, r3, #8
0039af30: mov r0, r3
0039af34: add ip, r2, #8
0039af38: add r1, r2, #0x110
0039af3c: add r2, r2, #0x104
0039af40: str ip, [r4]
0039af44: str r2, [r4, #4]
0039af48: str r1, [r4, #0x24]
0039af4c: str r3, [r4, #0x728]
0039af50: str r3, [r4, #0x72c]
0039af54: mov r1, #0x10
0039af58: bl #0x31167c
0039af5c: ldr r2, [r4, #0x728]
0039af60: mov r5, #0
0039af64: add r3, r4, #0x730
0039af68: strb r5, [r2]
0039af6c: add r3, r3, #8
0039af70: mvn r6, #0
0039af74: mov r2, #1
0039af78: str r2, [r4, #0x734]
0039af7c: mov r0, r3
0039af80: str r3, [r4, #0x748]
0039af84: str r3, [r4, #0x74c]
0039af88: str r6, [r4, #0x730]
0039af8c: mov r1, #0x10
0039af90: bl #0x31167c
0039af94: ldr r2, [r4, #0x748]
0039af98: add r3, r4, #0x750
0039af9c: mov r0, r3
0039afa0: strb r5, [r2]
0039afa4: mov r1, #0x10
0039afa8: str r3, [r4, #0x760]
0039afac: str r3, [r4, #0x764]
0039afb0: bl #0x31167c
0039afb4: ldr r3, [r4, #0x760]
0039afb8: mov r0, r4
0039afbc: strb r5, [r3]
0039afc0: str r6, [r4, #0x770]
0039afc4: strb r5, [r4, #0x778]
0039afc8: str r6, [r4, #0x768]
0039afcc: str r6, [r4, #0x76c]
0039afd0: str r5, [r4, #0x774]
0039afd4: pop {r4, r5, r6, pc}
0039afd8: subseq sb, pc, r0, ror fp
0039afdc: andeq r2, r0, r8, asr r1

# 0x39afe0 _ZN12TriggerPlateC2EN10ObjectBase6GO_IDSE
0039afe0: push {r4, r5, r6, lr}
0039afe4: mov r2, #1
0039afe8: mov r3, #0
0039afec: ldr r5, [pc, #0xc0]
0039aff0: mov r4, r0
0039aff4: bl #0x398fd4
0039aff8: ldr r2, [pc, #0xb8]
0039affc: add r5, pc, r5
0039b000: add r3, r4, #0x710
0039b004: ldr r2, [r5, r2]
0039b008: add r3, r3, #8
0039b00c: mov r0, r3
0039b010: add ip, r2, #8
0039b014: add r1, r2, #0x110
0039b018: add r2, r2, #0x104
0039b01c: str ip, [r4]
0039b020: str r2, [r4, #4]
0039b024: str r1, [r4, #0x24]
0039b028: str r3, [r4, #0x728]
0039b02c: str r3, [r4, #0x72c]
0039b030: mov r1, #0x10
0039b034: bl #0x31167c
0039b038: ldr r2, [r4, #0x728]
0039b03c: mov r5, #0
0039b040: add r3, r4, #0x730
0039b044: strb r5, [r2]
0039b048: add r3, r3, #8
0039b04c: mvn r6, #0
0039b050: mov r2, #1
0039b054: str r2, [r4, #0x734]
0039b058: mov r0, r3
0039b05c: str r3, [r4, #0x748]
0039b060: str r3, [r4, #0x74c]
0039b064: str r6, [r4, #0x730]
0039b068: mov r1, #0x10
0039b06c: bl #0x31167c
0039b070: ldr r2, [r4, #0x748]
0039b074: add r3, r4, #0x750
0039b078: mov r0, r3
0039b07c: strb r5, [r2]
0039b080: mov r1, #0x10
0039b084: str r3, [r4, #0x760]
0039b088: str r3, [r4, #0x764]
0039b08c: bl #0x31167c
0039b090: ldr r3, [r4, #0x760]
0039b094: mov r0, r4
0039b098: strb r5, [r3]
0039b09c: str r6, [r4, #0x770]
0039b0a0: strb r5, [r4, #0x778]
0039b0a4: str r6, [r4, #0x768]
0039b0a8: str r6, [r4, #0x76c]
0039b0ac: str r5, [r4, #0x774]
0039b0b0: pop {r4, r5, r6, pc}

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

# 0x39bf10 _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_.clone.2
0039bf10: push {r4, r5, r6, r7, r8, lr}
0039bf14: mov r7, r0
0039bf18: sub sp, sp, #8
0039bf1c: mov r6, r1
0039bf20: mov r0, #0x24
0039bf24: mov r1, #0
0039bf28: mov r8, r2
0039bf2c: bl #0x310570
0039bf30: ldr r5, [pc, #0x58]
0039bf34: ldr r3, [pc, #0x58]
0039bf38: mov r4, r0
0039bf3c: add r5, pc, r5
0039bf40: ldr r3, [r5, r3]
0039bf44: mov r1, r6
0039bf48: add r2, sp, #4
0039bf4c: add r3, r3, #8
0039bf50: str r3, [r0], #8
0039bf54: bl #0x3140ec
0039bf58: ldr r3, [pc, #0x38]
0039bf5c: rsb r8, r7, r8
0039bf60: mvn r2, #0
0039bf64: ldr r3, [r5, r3]
0039bf68: str r2, [r4, #0x20]
0039bf6c: str r8, [r4, #4]
0039bf70: add r3, r3, #8
0039bf74: str r3, [r4]
0039bf78: mov r0, r7
0039bf7c: mov r1, r6
0039bf80: mov r2, r4
0039bf84: bl #0x513ce4
0039bf88: add sp, sp, #8
0039bf8c: pop {r4, r5, r6, r7, r8, pc}
0039bf90: subseq r8, pc, r4, asr fp
0039bf94: andeq r2, r0, r0, lsr r3
0039bf98: muleq r0, r0, r5

# 0x39c83c _ZN11PropertyMap11AddPropertyIiEEvPKcRT_S3_.clone.1
0039c83c: push {r4, r5, r6, r7, r8, lr}
0039c840: mov r7, r0
0039c844: sub sp, sp, #8
0039c848: mov r6, r1
0039c84c: mov r0, #0x24
0039c850: mov r1, #0
0039c854: mov r8, r2
0039c858: bl #0x310570
0039c85c: ldr r5, [pc, #0x58]
0039c860: ldr r3, [pc, #0x58]
0039c864: mov r4, r0
0039c868: add r5, pc, r5
0039c86c: ldr r3, [r5, r3]
0039c870: mov r1, r6
0039c874: add r2, sp, #4
0039c878: add r3, r3, #8
0039c87c: str r3, [r0], #8
0039c880: bl #0x3140ec
0039c884: ldr r3, [pc, #0x38]
0039c888: rsb r8, r7, r8
0039c88c: mvn r2, #0
0039c890: ldr r3, [r5, r3]
0039c894: str r2, [r4, #0x20]
0039c898: str r8, [r4, #4]
0039c89c: add r3, r3, #8
0039c8a0: str r3, [r4]
0039c8a4: mov r0, r7
0039c8a8: mov r1, r6
0039c8ac: mov r2, r4
0039c8b0: bl #0x513ce4
0039c8b4: add sp, sp, #8
0039c8b8: pop {r4, r5, r6, r7, r8, pc}
0039c8bc: subseq r8, pc, r8, lsr #4
0039c8c0: andeq r2, r0, r0, lsr r3
0039c8c4: muleq r0, r0, r5

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

# 0x39d280 _ZN14ProjectileTrapC1EN10ObjectBase6GO_IDSE
0039d280: push {r4, r5, r6, lr}
0039d284: ldr r5, [pc, #0x2c]
0039d288: mov r4, r0
0039d28c: bl #0x39da9c
0039d290: ldr r3, [pc, #0x24]
0039d294: add r5, pc, r5
0039d298: mov r0, r4
0039d29c: ldr r3, [r5, r3]
0039d2a0: add r2, r3, #0x118
0039d2a4: add r1, r3, #8
0039d2a8: add r3, r3, #0x10c
0039d2ac: stm r4, {r1, r3}
0039d2b0: str r2, [r4, #0x24]
0039d2b4: pop {r4, r5, r6, pc}
0039d2b8: ldrsheq r7, [pc], #-0x7c
0039d2bc: andeq r1, r0, ip, ror #16

# 0x39d2c0 _ZN14ProjectileTrapC2EN10ObjectBase6GO_IDSE
0039d2c0: push {r4, r5, r6, lr}
0039d2c4: ldr r5, [pc, #0x2c]
0039d2c8: mov r4, r0
0039d2cc: bl #0x39da9c
0039d2d0: ldr r3, [pc, #0x24]
0039d2d4: add r5, pc, r5
0039d2d8: mov r0, r4
0039d2dc: ldr r3, [r5, r3]
0039d2e0: add r2, r3, #0x118
0039d2e4: add r1, r3, #8
0039d2e8: add r3, r3, #0x10c
0039d2ec: stm r4, {r1, r3}
0039d2f0: str r2, [r4, #0x24]
0039d2f4: pop {r4, r5, r6, pc}
0039d2f8: ldrheq r7, [pc], #-0x7c
0039d2fc: andeq r1, r0, ip, ror #16

# 0x39da54 _ZN9TimerTrapC1EN10ObjectBase6GO_IDSE
0039da54: push {r4, r5, r6, lr}
0039da58: ldr r5, [pc, #0x34]
0039da5c: mov r4, r0
0039da60: bl #0x39e670
0039da64: ldr r3, [pc, #0x2c]
0039da68: add r5, pc, r5
0039da6c: mov r2, #0
0039da70: ldr r3, [r5, r3]
0039da74: str r2, [r4, #0x404]
0039da78: mov r0, r4
0039da7c: add r2, r3, #0x118
0039da80: add r1, r3, #8
0039da84: add r3, r3, #0x10c
0039da88: stm r4, {r1, r3}
0039da8c: str r2, [r4, #0x24]
0039da90: pop {r4, r5, r6, pc}
0039da94: subseq r7, pc, r8, lsr #32
0039da98: andeq r2, r0, ip, ror #11

# 0x39da9c _ZN9TimerTrapC2EN10ObjectBase6GO_IDSE
0039da9c: push {r4, r5, r6, lr}
0039daa0: ldr r5, [pc, #0x34]
0039daa4: mov r4, r0
0039daa8: bl #0x39e670
0039daac: ldr r3, [pc, #0x2c]
0039dab0: add r5, pc, r5
0039dab4: mov r2, #0
0039dab8: ldr r3, [r5, r3]
0039dabc: str r2, [r4, #0x404]
0039dac0: mov r0, r4
0039dac4: add r2, r3, #0x118
0039dac8: add r1, r3, #8
0039dacc: add r3, r3, #0x10c
0039dad0: stm r4, {r1, r3}
0039dad4: str r2, [r4, #0x24]
0039dad8: pop {r4, r5, r6, pc}
0039dadc: subseq r6, pc, r0, ror #31
0039dae0: andeq r2, r0, ip, ror #11

# 0x39e670 _ZN11TriggerTrapC2EN10ObjectBase6GO_IDSE
0039e670: push {r4, r5, r6, lr}
0039e674: mov r2, #1
0039e678: mov r3, #0
0039e67c: ldr r5, [pc, #0xac]
0039e680: mov r4, r0
0039e684: bl #0x397f28
0039e688: ldr r3, [pc, #0xa4]
0039e68c: add r5, pc, r5
0039e690: add r2, r4, #0x3a8
0039e694: ldr r3, [r5, r3]
0039e698: mov r0, r2
0039e69c: str r2, [r4, #0x3b8]
0039e6a0: add ip, r3, #8
0039e6a4: add r1, r3, #0x114
0039e6a8: add r3, r3, #0x108
0039e6ac: str r3, [r4, #4]
0039e6b0: str r1, [r4, #0x24]
0039e6b4: str r2, [r4, #0x3bc]
0039e6b8: str ip, [r4]
0039e6bc: mov r1, #0x10
0039e6c0: bl #0x31167c
0039e6c4: ldr r2, [r4, #0x3b8]
0039e6c8: mov r3, #0
0039e6cc: mov r1, r4
0039e6d0: mvn r0, #0
0039e6d4: strb r3, [r2]
0039e6d8: str r0, [r4, #0x3c0]
0039e6dc: mov r2, r4
0039e6e0: strb r3, [r4, #0x3c4]
0039e6e4: str r3, [r4, #0x3cc]
0039e6e8: strb r3, [r1, #0x3c8]!
0039e6ec: str r1, [r4, #0x3d4]
0039e6f0: str r1, [r4, #0x3d0]
0039e6f4: str r3, [r4, #0x3d8]
0039e6f8: mov r1, #1
0039e6fc: str r3, [r4, #0x3e4]
0039e700: strb r3, [r2, #0x3e0]!
0039e704: str r2, [r4, #0x3ec]
0039e708: str r0, [r4, #0x3fc]
0039e70c: strb r3, [r4, #0x401]
0039e710: strb r1, [r4, #0x85]
0039e714: str r2, [r4, #0x3e8]
0039e718: str r3, [r4, #0x3f0]
0039e71c: str r3, [r4, #0x3f8]
0039e720: strb r3, [r4, #0x400]
0039e724: strb r1, [r4, #0x84]
0039e728: mov r0, r4
0039e72c: pop {r4, r5, r6, pc}
0039e730: subseq r6, pc, r4, lsl #8
0039e734: andeq r1, r0, r8, asr #27

# 0x39ecd4 _ZN11TriggerTrapC1EN10ObjectBase6GO_IDSE
0039ecd4: push {r4, r5, r6, lr}
0039ecd8: mov r2, #1
0039ecdc: mov r3, #0
0039ece0: ldr r5, [pc, #0xac]
0039ece4: mov r4, r0
0039ece8: bl #0x397f28
0039ecec: ldr r3, [pc, #0xa4]
0039ecf0: add r5, pc, r5
0039ecf4: add r2, r4, #0x3a8
0039ecf8: ldr r3, [r5, r3]
0039ecfc: mov r0, r2
0039ed00: str r2, [r4, #0x3b8]
0039ed04: add ip, r3, #8
0039ed08: add r1, r3, #0x114
0039ed0c: add r3, r3, #0x108
0039ed10: str r3, [r4, #4]
0039ed14: str r1, [r4, #0x24]
0039ed18: str r2, [r4, #0x3bc]
0039ed1c: str ip, [r4]
0039ed20: mov r1, #0x10
0039ed24: bl #0x31167c
0039ed28: ldr r2, [r4, #0x3b8]
0039ed2c: mov r3, #0
0039ed30: mov r1, r4
0039ed34: mvn r0, #0
0039ed38: strb r3, [r2]
0039ed3c: str r0, [r4, #0x3c0]
0039ed40: mov r2, r4
0039ed44: strb r3, [r4, #0x3c4]
0039ed48: str r3, [r4, #0x3cc]
0039ed4c: strb r3, [r1, #0x3c8]!
0039ed50: str r1, [r4, #0x3d4]
0039ed54: str r1, [r4, #0x3d0]
0039ed58: str r3, [r4, #0x3d8]
0039ed5c: mov r1, #1
0039ed60: str r3, [r4, #0x3e4]
0039ed64: strb r3, [r2, #0x3e0]!
0039ed68: str r2, [r4, #0x3ec]
0039ed6c: str r0, [r4, #0x3fc]
0039ed70: strb r3, [r4, #0x401]
0039ed74: strb r1, [r4, #0x85]
0039ed78: str r2, [r4, #0x3e8]
0039ed7c: str r3, [r4, #0x3f0]
0039ed80: str r3, [r4, #0x3f8]
0039ed84: strb r3, [r4, #0x400]
0039ed88: strb r1, [r4, #0x84]
0039ed8c: mov r0, r4
0039ed90: pop {r4, r5, r6, pc}
0039ed94: subseq r5, pc, r0, lsr #27
0039ed98: andeq r1, r0, r8, asr #27

# 0x39fc2c _ZN7PODecorC1EP13PhysicalWorldP10GameObjectb.clone.2
0039fc2c: push {r4, r5, lr}
0039fc30: mov lr, #1
0039fc34: sub sp, sp, #0x24
0039fc38: mov r5, #2
0039fc3c: mov ip, #0
0039fc40: mov r3, lr
0039fc44: str r5, [sp, #0x10]
0039fc48: ldr r4, [pc, #0x40]
0039fc4c: movw r5, #0xffff
0039fc50: str r5, [sp, #0x14]
0039fc54: str ip, [sp, #0xc]
0039fc58: mov r5, r0
0039fc5c: str ip, [sp]
0039fc60: str ip, [sp, #4]
0039fc64: str ip, [sp, #8]
0039fc68: str lr, [sp, #0x18]
0039fc6c: bl #0x46f2f0
0039fc70: ldr r3, [pc, #0x1c]
0039fc74: add r4, pc, r4
0039fc78: mov r0, r5
0039fc7c: ldr r3, [r4, r3]
0039fc80: add r3, r3, #8
0039fc84: str r3, [r5]
0039fc88: add sp, sp, #0x24
0039fc8c: pop {r4, r5, pc}
0039fc90: subseq r4, pc, ip, lsl lr
0039fc94: andeq r2, r0, r8, lsl r4

# 0x3a06dc _ZN9ContainerC1EN10ObjectBase6GO_IDSE
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

# 0x3a0788 _ZN9ContainerC2EN10ObjectBase6GO_IDSE
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

# 0x3a14bc _ZN21DestructibleContainerC1EN10ObjectBase6GO_IDSE
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

# 0x3a1508 _ZN21DestructibleContainerC2EN10ObjectBase6GO_IDSE
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

# 0x3a1634 _ZNK17OpenableContainer9GetScriptEv
003a1634: ldr r2, [r0, #0x374]
003a1638: ldr r3, [pc, #0x28]
003a163c: cmn r2, #1
003a1640: add r3, pc, r3
003a1644: moveq r0, #0
003a1648: bxeq lr
003a164c: ldr r1, [pc, #0x18]
003a1650: ldr r3, [r3, r1]
003a1654: mov r1, #0x28
003a1658: ldr r3, [r3]
003a165c: mla r2, r1, r2, r3
003a1660: ldr r0, [r2, #0x18]
003a1664: bx lr
003a1668: subseq r3, pc, r0, asr r4
003a166c: ldrdeq r3, r4, [r0], -ip

# 0x3a1670 _ZNK17OpenableContainer9GetVisualEv
003a1670: ldr r0, [r0, #0x374]
003a1674: ldr r3, [pc, #0x24]
003a1678: cmn r0, #1
003a167c: add r3, pc, r3
003a1680: bxeq lr
003a1684: ldr r2, [pc, #0x18]
003a1688: ldr r3, [r3, r2]
003a168c: mov r2, #0x28
003a1690: ldr r3, [r3]
003a1694: mla r0, r2, r0, r3
003a1698: ldr r0, [r0, #0x24]
003a169c: bx lr
003a16a0: subseq r3, pc, r4, lsl r4
003a16a4: ldrdeq r3, r4, [r0], -ip

# 0x3a16a8 _ZNK17OpenableContainer8IsLockedEv
003a16a8: ldr r3, [r0, #0x394]
003a16ac: sub r3, r3, #3
003a16b0: cmp r3, #1
003a16b4: movls r0, #0
003a16b8: bxls lr
003a16bc: ldr r0, [r0, #0x710]
003a16c0: adds r0, r0, #1
003a16c4: movne r0, #1
003a16c8: bx lr

# 0x3a16cc _ZNK17OpenableContainer18GetInteractionTypeEP10GameObject
003a16cc: mov r0, #0
003a16d0: bx lr

# 0x3a177c _ZNK17OpenableContainer11KeepPhysicsEv
003a177c: push {r4, lr}
003a1780: ldr r0, [r0, #0x38c]
003a1784: bl #0x3a1708
003a1788: ldr r4, [pc, #0x2c]
003a178c: cmn r0, #1
003a1790: add r4, pc, r4
003a1794: beq #0x3a17b4
003a1798: ldr r3, [pc, #0x20]
003a179c: mov r2, #0x28
003a17a0: ldr r3, [r4, r3]
003a17a4: ldr r3, [r3]
003a17a8: mla r0, r2, r0, r3
003a17ac: ldrb r0, [r0, #0xc]
003a17b0: pop {r4, pc}
003a17b4: mov r0, #0
003a17b8: pop {r4, pc}
003a17bc: subseq r3, pc, r0, lsl #6
003a17c0: ldrdeq r3, r4, [r0], -ip

# 0x3a17c4 _ZNK17OpenableContainer8GetSoundEv
003a17c4: push {r4, lr}
003a17c8: ldr r0, [r0, #0x38c]
003a17cc: bl #0x3a1708
003a17d0: ldr r4, [pc, #0x24]
003a17d4: cmn r0, #1
003a17d8: add r4, pc, r4
003a17dc: beq #0x3a17f8
003a17e0: ldr r3, [pc, #0x18]
003a17e4: mov r2, #0x28
003a17e8: ldr r3, [r4, r3]
003a17ec: ldr r3, [r3]
003a17f0: mla r0, r2, r0, r3
003a17f4: ldr r0, [r0, #8]
003a17f8: pop {r4, pc}
003a17fc: ldrheq r3, [pc], #-0x28
003a1800: ldrdeq r3, r4, [r0], -ip

# 0x3a1804 _ZNK17OpenableContainer7GetLootEv
003a1804: push {r4, lr}
003a1808: ldr r0, [r0, #0x38c]
003a180c: bl #0x3a1708
003a1810: ldr r4, [pc, #0x24]
003a1814: cmn r0, #1
003a1818: add r4, pc, r4
003a181c: beq #0x3a1838
003a1820: ldr r3, [pc, #0x18]
003a1824: mov r2, #0x28
003a1828: ldr r3, [r4, r3]
003a182c: ldr r3, [r3]
003a1830: mla r0, r2, r0, r3
003a1834: ldr r0, [r0, #0x10]
003a1838: pop {r4, pc}
003a183c: subseq r3, pc, r8, ror r2
003a1840: ldrdeq r3, r4, [r0], -ip

# 0x3a1844 _ZNK17OpenableContainer9GetDataIdEv
003a1844: ldr r3, [r0, #0x388]
003a1848: ldr r0, [r0, #0x38c]
003a184c: cmp r3, r0
003a1850: beq #0x3a1858
003a1854: b #0x3a1708
003a1858: mvn r0, #0
003a185c: bx lr

# 0x3a1860 _ZN17OpenableContainer6UnlockEP9Character
003a1860: cmp r1, #0
003a1864: push {r4, r5, r6, lr}
003a1868: mov r4, r0
003a186c: beq #0x3a18a4
003a1870: add r5, r1, #0x37c
003a1874: mov r0, r5
003a1878: ldr r1, [r4, #0x710]
003a187c: bl #0x3fd15c
003a1880: cmp r0, #0
003a1884: beq #0x3a18a4
003a1888: ldrsh r3, [r0, #0x50]
003a188c: ldr r2, [r4, #0x708]
003a1890: cmp r2, r3
003a1894: bgt #0x3a18a4
003a1898: ldrb r3, [r4, #0x70c]
003a189c: cmp r3, #0
003a18a0: bne #0x3a18ac
003a18a4: mov r0, #0
003a18a8: pop {r4, r5, r6, pc}
003a18ac: ldr r1, [r4, #0x710]
003a18b0: mov r0, r5
003a18b4: pop {r4, r5, r6, lr}
003a18b8: b #0x3fe658

# 0x3a18bc _ZN17OpenableContainer12TryUnlockingEP10GameObject
003a18bc: push {r4, r5, r6, lr}
003a18c0: mov r5, r1
003a18c4: mov r4, r0
003a18c8: bl #0x3a16a8
003a18cc: cmp r0, #0
003a18d0: bne #0x3a18dc
003a18d4: mov r0, #1
003a18d8: pop {r4, r5, r6, pc}
003a18dc: ldr r3, [r5]
003a18e0: mov r0, r5
003a18e4: mov lr, pc
003a18e8: ldr pc, [r3, #0x24]
003a18ec: cmp r0, #0
003a18f0: movne r1, r5
003a18f4: moveq r1, #0
003a18f8: mov r0, r4
003a18fc: pop {r4, r5, r6, lr}
003a1900: b #0x3a1860

# 0x3a1904 _ZN17OpenableContainer8InteractEP10GameObject
003a1904: push {r4, r5, r6, r7, r8, sb, sl, lr}
003a1908: sub sp, sp, #0x30
003a190c: mov r6, r0
003a1910: mov r5, r1
003a1914: bl #0x3a18bc
003a1918: ldr r4, [pc, #0x208]
003a191c: cmp r0, #0
003a1920: add r4, pc, r4
003a1924: bne #0x3a1930
003a1928: add sp, sp, #0x30
003a192c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003a1930: ldr r7, [pc, #0x1f4]
003a1934: ldr r0, [r4, r7]
003a1938: bl #0x31f594
003a193c: subs r8, r0, #0
003a1940: beq #0x3a1acc
003a1944: ldr sl, [r4, r7]
003a1948: ldr r0, [sl, #0x40]
003a194c: bl #0x36f074
003a1950: cmp r0, #0
003a1954: bne #0x3a1a48
003a1958: mov r0, r6
003a195c: mov r1, r5
003a1960: add r6, sp, #0x24
003a1964: bl #0x3a0b38
003a1968: mov r1, r5
003a196c: mov r0, r6
003a1970: bl #0x33dd2c
003a1974: mov r0, r6
003a1978: bl #0x33ff54
003a197c: subs r5, r0, #0
003a1980: beq #0x3a1928
003a1984: ldr r3, [r5]
003a1988: mov lr, pc
003a198c: ldr pc, [r3, #0x28]
003a1990: cmp r0, #0
003a1994: beq #0x3a1928
003a1998: add r6, r5, #0x560
003a199c: mov r0, r6
003a19a0: mov r1, #0xda
003a19a4: mov r2, #1
003a19a8: bl #0x3e0798
003a19ac: ldr r3, [pc, #0x17c]
003a19b0: mov r0, r6
003a19b4: mov r1, #0xda
003a19b8: ldr r3, [r4, r3]
003a19bc: mov r2, #0
003a19c0: ldr r6, [r3]
003a19c4: bl #0x3df6e0
003a19c8: cmp r0, #0x63
003a19cc: ble #0x3a1928
003a19d0: ldr r3, [r4, r7]
003a19d4: mov r1, r5
003a19d8: ldr r0, [r3, #0x40]
003a19dc: bl #0x36effc
003a19e0: cmp r0, #0
003a19e4: beq #0x3a1928
003a19e8: ldr r3, [pc, #0x144]
003a19ec: ldr r3, [r4, r3]
003a19f0: ldr r8, [r3]
003a19f4: cmp r8, #0
003a19f8: beq #0x3a1b20
003a19fc: ldr r3, [pc, #0x134]
003a1a00: ldr r7, [pc, #0x134]
003a1a04: mov r5, #0
003a1a08: ldr r3, [r4, r3]
003a1a0c: add r7, pc, r7
003a1a10: ldr r4, [r3]
003a1a14: b #0x3a1a24
003a1a18: add r5, r5, #1
003a1a1c: cmp r5, r8
003a1a20: beq #0x3a1b20
003a1a24: ldr r1, [r4, r5, lsl #2]
003a1a28: mov r0, r7
003a1a2c: bl #0x30e31c
003a1a30: cmp r0, #0
003a1a34: bne #0x3a1a18
003a1a38: mov r1, r5
003a1a3c: mov r0, r6
003a1a40: bl #0x3813b8
003a1a44: b #0x3a1928
003a1a48: ldr r3, [r6]
003a1a4c: mov r0, r6
003a1a50: mov lr, pc
003a1a54: ldr pc, [r3, #0xd0]
003a1a58: ldr r1, [pc, #0xe0]
003a1a5c: ldr r2, [pc, #0xe0]
003a1a60: mov sb, r0
003a1a64: add r1, pc, r1
003a1a68: ldr r0, [sl, #0x2c]
003a1a6c: add r2, pc, r2
003a1a70: ldr sl, [r6, #0x64]
003a1a74: bl #0x4c4bdc
003a1a78: ldr r2, [pc, #0xc8]
003a1a7c: add r1, sp, #0x30
003a1a80: mov r3, #0
003a1a84: ldr r2, [r4, r2]
003a1a88: str r0, [sp, #0xc]
003a1a8c: mov r0, r8
003a1a90: add r2, r2, #8
003a1a94: str r2, [r1, #-0x28]!
003a1a98: mvn r2, #0
003a1a9c: strb r3, [sp, #0x19]
003a1aa0: strb r3, [sp, #0x18]
003a1aa4: str sl, [sp, #0x14]
003a1aa8: str r2, [sp, #0x1c]
003a1aac: str sb, [sp, #0x20]
003a1ab0: str r5, [sp, #0x10]
003a1ab4: bl #0x339090
003a1ab8: ldr r3, [pc, #0x8c]
003a1abc: ldr r3, [r4, r3]
003a1ac0: add r3, r3, #8
003a1ac4: str r3, [sp, #8]
003a1ac8: b #0x3a1958
003a1acc: ldr r3, [pc, #0x7c]
003a1ad0: ldr r3, [r4, r3]
003a1ad4: ldr r3, [r3]
003a1ad8: cmp r3, #2
003a1adc: streq r8, [r8]
003a1ae0: beq #0x3a1944
003a1ae4: cmp r3, #1
003a1ae8: bne #0x3a1944
003a1aec: ldr r0, [pc, #0x60]
003a1af0: ldr r1, [pc, #0x60]
003a1af4: ldr r2, [pc, #0x60]
003a1af8: ldr r0, [r4, r0]
003a1afc: ldr r3, [pc, #0x5c]
003a1b00: mov ip, #0xf9
003a1b04: add r1, pc, r1
003a1b08: add r2, pc, r2
003a1b0c: add r3, pc, r3
003a1b10: add r0, r0, #0xa8
003a1b14: str ip, [sp]
003a1b18: bl #0x30e004
003a1b1c: b #0x3a1944
003a1b20: mvn r1, #0
003a1b24: b #0x3a1a3c
003a1b28: subseq r3, pc, r0, ror r1
003a1b2c: strdeq r3, r4, [r0], -r4
003a1b30: andeq r1, r0, r0, ror sp
003a1b34: strdeq r0, r1, [r0], -ip
003a1b38: andeq r1, r0, ip, lsr #32
003a1b3c: subseq r1, r2, ip, ror r6
003a1b40: subseq r0, r2, r4, lsl #30
003a1b44: subseq r1, r2, ip, lsl #12
003a1b48: strdeq r2, r3, [r0], -r8
003a1b4c: strheq r0, [r0], -r0
003a1b50: andeq r3, r0, r0, asr #19
003a1b54: andeq r1, r0, r0, asr #19
003a1b58: ldrsbeq ip, [r1], #-0x84
003a1b5c: subseq sp, r6, r0, asr lr
003a1b60: subseq r1, r2, r4, lsl r5

# 0x3a1b64 _ZN17OpenableContainer8InitPostEv
003a1b64: push {r4, r5, r6, r7, r8, lr}
003a1b68: mov r8, r0
003a1b6c: bl #0x39f910
003a1b70: ldr r5, [r8, #0x704]
003a1b74: ldr r2, [r8, #0x700]
003a1b78: ldr r3, [pc, #0x60]
003a1b7c: cmp r5, r2
003a1b80: add r3, pc, r3
003a1b84: beq #0x3a1bd4
003a1b88: ldr r2, [pc, #0x54]
003a1b8c: ldr r2, [r3, r2]
003a1b90: ldr r6, [r2]
003a1b94: cmp r6, #0
003a1b98: beq #0x3a1bd8
003a1b9c: ldr r2, [pc, #0x44]
003a1ba0: mov r4, #0
003a1ba4: ldr r3, [r3, r2]
003a1ba8: ldr r7, [r3]
003a1bac: b #0x3a1bbc
003a1bb0: add r4, r4, #1
003a1bb4: cmp r4, r6
003a1bb8: beq #0x3a1bd8
003a1bbc: ldr r1, [r7, r4, lsl #2]
003a1bc0: mov r0, r5
003a1bc4: bl #0x30e31c
003a1bc8: cmp r0, #0
003a1bcc: bne #0x3a1bb0
003a1bd0: str r4, [r8, #0x710]
003a1bd4: pop {r4, r5, r6, r7, r8, pc}
003a1bd8: mvn r4, #0
003a1bdc: b #0x3a1bd0
003a1be0: subseq r2, pc, r0, lsl pc
003a1be4: andeq r0, r0, r0, ror #26
003a1be8: andeq r1, r0, r4, asr ip

# 0x3a1bec _ZThn36_N17OpenableContainerD1Ev
003a1bec: sub r0, r0, #0x24
003a1bf0: b #0x3a1bf4

# 0x3a1bf4 _ZN17OpenableContainerD1Ev
003a1bf4: ldr r2, [pc, #0x3c]
003a1bf8: ldr r3, [pc, #0x3c]
003a1bfc: push {r4, lr}
003a1c00: add r2, pc, r2
003a1c04: ldr r3, [r2, r3]
003a1c08: mov r4, r0
003a1c0c: add r0, r0, #0x6f0
003a1c10: add r2, r3, #0x100
003a1c14: add r1, r3, #8
003a1c18: add r3, r3, #0xf4
003a1c1c: stm r4, {r1, r3}
003a1c20: str r2, [r4, #0x24]
003a1c24: bl #0x3139ac
003a1c28: mov r0, r4
003a1c2c: bl #0x3a0598
003a1c30: mov r0, r4
003a1c34: pop {r4, pc}

# 0x3a1c40 _ZThn36_N17OpenableContainerD0Ev
003a1c40: sub r0, r0, #0x24
003a1c44: b #0x3a1c48

# 0x3a1c48 _ZN17OpenableContainerD0Ev
003a1c48: push {r4, lr}
003a1c4c: mov r4, r0
003a1c50: bl #0x3a1bf4
003a1c54: mov r0, r4
003a1c58: bl #0x310440
003a1c5c: mov r0, r4
003a1c60: pop {r4, pc}

# 0x3a1c64 _ZN17OpenableContainerD2Ev
003a1c64: ldr r2, [pc, #0x3c]
003a1c68: ldr r3, [pc, #0x3c]
003a1c6c: push {r4, lr}
003a1c70: add r2, pc, r2
003a1c74: ldr r3, [r2, r3]
003a1c78: mov r4, r0
003a1c7c: add r0, r0, #0x6f0
003a1c80: add r2, r3, #0x100
003a1c84: add r1, r3, #8
003a1c88: add r3, r3, #0xf4
003a1c8c: stm r4, {r1, r3}
003a1c90: str r2, [r4, #0x24]
003a1c94: bl #0x3139ac
003a1c98: mov r0, r4
003a1c9c: bl #0x3a0598
003a1ca0: mov r0, r4
003a1ca4: pop {r4, pc}
003a1ca8: subseq r2, pc, r0, lsr #28
003a1cac: strdeq r0, r1, [r0], -ip

# 0x3a1cb0 _ZN17OpenableContainerC1EN10ObjectBase6GO_IDSE
003a1cb0: push {r4, r5, r6, lr}
003a1cb4: ldr r5, [pc, #0x68]
003a1cb8: mov r4, r0
003a1cbc: bl #0x3a0788
003a1cc0: ldr r3, [pc, #0x60]
003a1cc4: add r5, pc, r5
003a1cc8: add r2, r4, #0x6f0
003a1ccc: ldr r3, [r5, r3]
003a1cd0: mov r0, r2
003a1cd4: str r2, [r4, #0x700]
003a1cd8: add ip, r3, #8
003a1cdc: add r1, r3, #0x100
003a1ce0: add r3, r3, #0xf4
003a1ce4: str r3, [r4, #4]
003a1ce8: str r1, [r4, #0x24]
003a1cec: str r2, [r4, #0x704]
003a1cf0: str ip, [r4]
003a1cf4: mov r1, #0x10
003a1cf8: bl #0x31167c
003a1cfc: ldr r2, [r4, #0x700]
003a1d00: mov r1, #0
003a1d04: mov r3, #1
003a1d08: strb r1, [r2]
003a1d0c: mvn r2, #0
003a1d10: strb r3, [r4, #0x70c]
003a1d14: str r2, [r4, #0x710]
003a1d18: str r3, [r4, #0x708]
003a1d1c: mov r0, r4
003a1d20: pop {r4, r5, r6, pc}
003a1d24: subseq r2, pc, ip, asr #27
003a1d28: strdeq r0, r1, [r0], -ip

# 0x3a1d2c _ZN17OpenableContainerC2EN10ObjectBase6GO_IDSE
003a1d2c: push {r4, r5, r6, lr}
003a1d30: ldr r5, [pc, #0x68]
003a1d34: mov r4, r0
003a1d38: bl #0x3a0788
003a1d3c: ldr r3, [pc, #0x60]
003a1d40: add r5, pc, r5
003a1d44: add r2, r4, #0x6f0
003a1d48: ldr r3, [r5, r3]
003a1d4c: mov r0, r2
003a1d50: str r2, [r4, #0x700]
003a1d54: add ip, r3, #8
003a1d58: add r1, r3, #0x100
003a1d5c: add r3, r3, #0xf4
003a1d60: str r3, [r4, #4]
003a1d64: str r1, [r4, #0x24]
003a1d68: str r2, [r4, #0x704]
003a1d6c: str ip, [r4]
003a1d70: mov r1, #0x10
003a1d74: bl #0x31167c
003a1d78: ldr r2, [r4, #0x700]
003a1d7c: mov r1, #0
003a1d80: mov r3, #1
003a1d84: strb r1, [r2]
003a1d88: mvn r2, #0
003a1d8c: strb r3, [r4, #0x70c]
003a1d90: str r2, [r4, #0x710]
003a1d94: str r3, [r4, #0x708]
003a1d98: mov r0, r4
003a1d9c: pop {r4, r5, r6, pc}
003a1da0: subseq r2, pc, r0, asr sp
003a1da4: strdeq r0, r1, [r0], -ip

# 0x3a1e84 _ZThn4_N17OpenableContainer17DeclarePropertiesEv
003a1e84: sub r0, r0, #4
003a1e88: b #0x3a1e8c

# 0x3a1e8c _ZN17OpenableContainer17DeclarePropertiesEv
003a1e8c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a1e90: ldr r4, [pc, #0x1d4]
003a1e94: ldr r3, [pc, #0x1d4]
003a1e98: sub sp, sp, #0x4c
003a1e9c: add r4, pc, r4
003a1ea0: ldr ip, [r4, r3]
003a1ea4: add r7, sp, #0x2c
003a1ea8: mov fp, r0
003a1eac: ldr r3, [ip]
003a1eb0: str ip, [sp]
003a1eb4: mov sb, #0
003a1eb8: str r3, [sp, #0x44]
003a1ebc: bl #0x3a083c
003a1ec0: mov r0, r7
003a1ec4: mov r1, #0x10
003a1ec8: str r7, [sp, #0x3c]
003a1ecc: str r7, [sp, #0x40]
003a1ed0: bl #0x31167c
003a1ed4: ldr r3, [sp, #0x3c]
003a1ed8: add r8, sp, #0x14
003a1edc: mov r0, r8
003a1ee0: strb sb, [r3]
003a1ee4: ldr r2, [sp, #0x3c]
003a1ee8: ldr r1, [sp, #0x40]
003a1eec: str r8, [sp, #0x24]
003a1ef0: str r8, [sp, #0x28]
003a1ef4: bl #0x3116e8
003a1ef8: mov r1, sb
003a1efc: mov r0, #0x38
003a1f00: bl #0x310570
003a1f04: ldr r3, [pc, #0x168]
003a1f08: ldr sl, [pc, #0x168]
003a1f0c: mov r6, r0
003a1f10: ldr r3, [r4, r3]
003a1f14: add sl, pc, sl
003a1f18: mov r1, sl
003a1f1c: add r3, r3, #8
003a1f20: str r3, [r0], #8
003a1f24: add r2, sp, #0x10
003a1f28: str r3, [sp, #4]
003a1f2c: bl #0x3140ec
003a1f30: ldr r2, [pc, #0x144]
003a1f34: add r5, fp, #4
003a1f38: add r1, fp, #0x6f0
003a1f3c: ldr r2, [r4, r2]
003a1f40: mov r0, r6
003a1f44: rsb r1, r5, r1
003a1f48: add r2, r2, #8
003a1f4c: str r1, [r6, #4]
003a1f50: str r2, [r0], #0x20
003a1f54: str r0, [r6, #0x30]
003a1f58: str r0, [r6, #0x34]
003a1f5c: ldr r1, [sp, #0x28]
003a1f60: ldr r2, [sp, #0x24]
003a1f64: bl #0x3116e8
003a1f68: mov r2, r6
003a1f6c: mov r1, sl
003a1f70: mov r0, r5
003a1f74: bl #0x513ce4
003a1f78: mov r0, r8
003a1f7c: bl #0x3139ac
003a1f80: mov r0, r7
003a1f84: bl #0x3139ac
003a1f88: mov r1, sb
003a1f8c: mov r0, #0x24
003a1f90: bl #0x310570
003a1f94: ldr r7, [pc, #0xe4]
003a1f98: ldr r3, [sp, #4]
003a1f9c: mov r6, r0
003a1fa0: add r7, pc, r7
003a1fa4: str r3, [r0], #8
003a1fa8: mov r1, r7
003a1fac: add r2, sp, #0xc
003a1fb0: str r3, [sp, #4]
003a1fb4: bl #0x3140ec
003a1fb8: ldr r2, [pc, #0xc4]
003a1fbc: add fp, fp, #0x700
003a1fc0: add r1, fp, #8
003a1fc4: ldr r2, [r4, r2]
003a1fc8: rsb r1, r5, r1
003a1fcc: mov r8, #1
003a1fd0: add r2, r2, #8
003a1fd4: str r1, [r6, #4]
003a1fd8: str r2, [r6]
003a1fdc: mov r1, r7
003a1fe0: mov r2, r6
003a1fe4: str r8, [r6, #0x20]
003a1fe8: mov r0, r5
003a1fec: bl #0x513ce4
003a1ff0: mov r1, sb
003a1ff4: mov r0, #0x24
003a1ff8: bl #0x310570
003a1ffc: ldr r7, [pc, #0x84]
003a2000: ldr r3, [sp, #4]
003a2004: mov r6, r0
003a2008: add r7, pc, r7
003a200c: str r3, [r0], #8
003a2010: mov r1, r7
003a2014: add r2, sp, #8
003a2018: bl #0x3140ec
003a201c: ldr r3, [pc, #0x68]
003a2020: add fp, fp, #0xc
003a2024: rsb fp, r5, fp
003a2028: ldr r3, [r4, r3]
003a202c: mov r2, r6
003a2030: str fp, [r6, #4]
003a2034: add r3, r3, #8
003a2038: str r3, [r6]
003a203c: strb r8, [r6, #0x20]
003a2040: mov r0, r5
003a2044: mov r1, r7
003a2048: bl #0x513ce4
003a204c: ldr ip, [sp]
003a2050: ldr r2, [sp, #0x44]
003a2054: ldr r3, [ip]
003a2058: cmp r2, r3
003a205c: bne #0x3a2068
003a2060: add sp, sp, #0x4c
003a2064: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a2068: bl #0x30e310
003a206c: ldrsheq r2, [pc], #-0xb4
003a2070: andeq r4, r0, ip, lsr #1
003a2074: andeq r2, r0, r0, lsr r3
003a2078: subseq r1, r2, r4, lsl #3
003a207c: muleq r0, r4, r4
003a2080: subseq r1, r2, r8, lsl #2
003a2084: muleq r0, r0, r5
003a2088: subseq r1, r2, r8, lsr #1
003a208c: andeq r3, r0, ip, asr #28

# 0x3a2bcc _ZN13SlotContainerC1EN10ObjectBase6GO_IDSE
003a2bcc: push {r4, r5, r6, lr}
003a2bd0: ldr r5, [pc, #0x74]
003a2bd4: mov r4, r0
003a2bd8: bl #0x3a1d2c
003a2bdc: ldr r2, [pc, #0x6c]
003a2be0: add r5, pc, r5
003a2be4: add r3, r4, #0x710
003a2be8: ldr r2, [r5, r2]
003a2bec: add r3, r3, #4
003a2bf0: mov r0, r3
003a2bf4: add ip, r2, #8
003a2bf8: add r1, r2, #0x100
003a2bfc: add r2, r2, #0xf4
003a2c00: str r2, [r4, #4]
003a2c04: str r1, [r4, #0x24]
003a2c08: str r3, [r4, #0x724]
003a2c0c: str r3, [r4, #0x728]
003a2c10: str ip, [r4]
003a2c14: mov r1, #0x10
003a2c18: bl #0x31167c
003a2c1c: ldr r1, [r4, #0x724]
003a2c20: mov r3, #0
003a2c24: mvn r2, #0
003a2c28: strb r3, [r1]
003a2c2c: mov r0, r4
003a2c30: str r2, [r4, #0x730]
003a2c34: str r3, [r4, #0x740]
003a2c38: str r2, [r4, #0x72c]
003a2c3c: str r3, [r4, #0x734]
003a2c40: str r3, [r4, #0x738]
003a2c44: str r3, [r4, #0x73c]
003a2c48: pop {r4, r5, r6, pc}
003a2c4c: ldrheq r1, [pc], #-0xe0
003a2c50: andeq r4, r0, r4, asr #19

# 0x3a2c54 _ZN13SlotContainerC2EN10ObjectBase6GO_IDSE
003a2c54: push {r4, r5, r6, lr}
003a2c58: ldr r5, [pc, #0x74]
003a2c5c: mov r4, r0
003a2c60: bl #0x3a1d2c
003a2c64: ldr r2, [pc, #0x6c]
003a2c68: add r5, pc, r5
003a2c6c: add r3, r4, #0x710
003a2c70: ldr r2, [r5, r2]
003a2c74: add r3, r3, #4
003a2c78: mov r0, r3
003a2c7c: add ip, r2, #8
003a2c80: add r1, r2, #0x100
003a2c84: add r2, r2, #0xf4
003a2c88: str r2, [r4, #4]
003a2c8c: str r1, [r4, #0x24]
003a2c90: str r3, [r4, #0x724]
003a2c94: str r3, [r4, #0x728]
003a2c98: str ip, [r4]
003a2c9c: mov r1, #0x10
003a2ca0: bl #0x31167c
003a2ca4: ldr r1, [r4, #0x724]
003a2ca8: mov r3, #0
003a2cac: mvn r2, #0
003a2cb0: strb r3, [r1]
003a2cb4: mov r0, r4
003a2cb8: str r2, [r4, #0x730]
003a2cbc: str r3, [r4, #0x740]
003a2cc0: str r2, [r4, #0x72c]
003a2cc4: str r3, [r4, #0x734]
003a2cc8: str r3, [r4, #0x738]
003a2ccc: str r3, [r4, #0x73c]
003a2cd0: pop {r4, r5, r6, pc}
003a2cd4: subseq r1, pc, r8, lsr #28
003a2cd8: andeq r4, r0, r4, asr #19

# 0x3a6a24 _ZN9Character18NetStructCharacterC1Ev
003a6a24: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a6a28: ldr r5, [pc, #0x4b4]
003a6a2c: sub sp, sp, #0x2c
003a6a30: mov r4, r0
003a6a34: bl #0x8138f4
003a6a38: ldr r3, [pc, #0x4a8]
003a6a3c: add r5, pc, r5
003a6a40: mov r6, r4
003a6a44: ldr r3, [r5, r3]
003a6a48: mov r7, #0
003a6a4c: add r2, r4, #0x228
003a6a50: add r3, r3, #8
003a6a54: str r3, [r6], #0x130
003a6a58: mov r1, r7
003a6a5c: mov r0, r6
003a6a60: ldr r8, [pc, #0x484]
003a6a64: str r2, [sp, #4]
003a6a68: bl #0x3a681c
003a6a6c: add r3, r4, #0x320
003a6a70: mov r1, r7
003a6a74: ldr r0, [sp, #4]
003a6a78: str r3, [sp, #0x14]
003a6a7c: bl #0x3a681c
003a6a80: mov r1, r7
003a6a84: ldr r0, [sp, #0x14]
003a6a88: bl #0x3a6920
003a6a8c: mov sl, #0
003a6a90: mov r0, #0x420
003a6a94: ldr r1, [r5, r8]
003a6a98: mov fp, #0
003a6a9c: strd sl, fp, [r4, r0]
003a6aa0: mvn r2, #0
003a6aa4: mov r3, #0
003a6aa8: add r1, r1, #8
003a6aac: mov r0, #0x10
003a6ab0: str r0, [r4, #0x41c]
003a6ab4: str r1, [r4, #0x418]
003a6ab8: ldr r0, [r4, #0x438]
003a6abc: str r2, [r4, #0x42c]
003a6ac0: strb r3, [r4, #0x434]
003a6ac4: str r2, [r4, #0x428]
003a6ac8: str r3, [r4, #0x430]
003a6acc: mov r1, r7
003a6ad0: bl #0x30df8c
003a6ad4: cmp r0, #0
003a6ad8: addne sb, r4, #0x410
003a6adc: addne sb, sb, #8
003a6ae0: strne sb, [sp]
003a6ae4: bne #0x3a6b00
003a6ae8: add sl, r4, #0x410
003a6aec: add sl, sl, #8
003a6af0: str sl, [sp]
003a6af4: str r7, [r4, #0x438]
003a6af8: ldr r0, [sp]
003a6afc: bl #0x814f84
003a6b00: ldr r7, [pc, #0x3e8]
003a6b04: ldr r1, [r5, r8]
003a6b08: movw ip, #0x448
003a6b0c: ldr r0, [r5, r7]
003a6b10: add sl, r1, #8
003a6b14: mov r1, #0
003a6b18: add lr, r0, #8
003a6b1c: mov r0, #0
003a6b20: strd r0, r1, [r4, ip]
003a6b24: mvn r2, #0
003a6b28: mov r3, #0
003a6b2c: mov r0, #0x10
003a6b30: mov r8, #0
003a6b34: mov r1, r8
003a6b38: str r0, [r4, #0x444]
003a6b3c: str lr, [r4, #0x418]
003a6b40: ldr r0, [r4, #0x460]
003a6b44: str r2, [r4, #0x454]
003a6b48: strb r3, [r4, #0x45c]
003a6b4c: str sl, [r4, #0x440]
003a6b50: str r2, [r4, #0x450]
003a6b54: str r3, [r4, #0x458]
003a6b58: bl #0x30df8c
003a6b5c: cmp r0, #0
003a6b60: addne r1, r4, #0x440
003a6b64: strne r1, [sp, #0x24]
003a6b68: bne #0x3a6b80
003a6b6c: add r2, r4, #0x440
003a6b70: str r2, [sp, #0x24]
003a6b74: str r8, [r4, #0x460]
003a6b78: ldr r0, [sp, #0x24]
003a6b7c: bl #0x814f84
003a6b80: ldr fp, [pc, #0x36c]
003a6b84: ldr r3, [r4, #0x488]
003a6b88: ldr r0, [r5, r7]
003a6b8c: ldr r1, [r5, fp]
003a6b90: cmp r3, #0
003a6b94: mov r8, #0
003a6b98: add r0, r0, #8
003a6b9c: mov ip, #0x470
003a6ba0: mov sb, #0
003a6ba4: strd r8, sb, [r4, ip]
003a6ba8: mvn r2, #0
003a6bac: mov r3, #0
003a6bb0: add r1, r1, #8
003a6bb4: str r0, [r4, #0x440]
003a6bb8: addeq r8, r4, #0x460
003a6bbc: mov r0, #0x20
003a6bc0: str r0, [r4, #0x46c]
003a6bc4: str r2, [r4, #0x47c]
003a6bc8: str r1, [r4, #0x468]
003a6bcc: str r2, [r4, #0x478]
003a6bd0: str r3, [r4, #0x480]
003a6bd4: strb r3, [r4, #0x484]
003a6bd8: addeq r8, r8, #8
003a6bdc: beq #0x3a6bf4
003a6be0: add r8, r4, #0x460
003a6be4: add r8, r8, #8
003a6be8: str r3, [r4, #0x488]
003a6bec: mov r0, r8
003a6bf0: bl #0x814f84
003a6bf4: ldr r7, [pc, #0x2fc]
003a6bf8: ldr r3, [r4, #0x4b0]
003a6bfc: ldr r1, [r5, fp]
003a6c00: ldr r0, [r5, r7]
003a6c04: cmp r3, #0
003a6c08: add sl, r1, #8
003a6c0c: add lr, r0, #8
003a6c10: mov r1, #0
003a6c14: mov r0, #0
003a6c18: movw ip, #0x498
003a6c1c: strd r0, r1, [r4, ip]
003a6c20: mvn r2, #0
003a6c24: mov r3, #0
003a6c28: mov r0, #0x20
003a6c2c: addeq r1, r4, #0x490
003a6c30: str lr, [r4, #0x468]
003a6c34: str r0, [r4, #0x494]
003a6c38: str r2, [r4, #0x4a4]
003a6c3c: str sl, [r4, #0x490]
003a6c40: str r2, [r4, #0x4a0]
003a6c44: str r3, [r4, #0x4a8]
003a6c48: strb r3, [r4, #0x4ac]
003a6c4c: streq r1, [sp, #0x20]
003a6c50: beq #0x3a6c68
003a6c54: add r2, r4, #0x490
003a6c58: str r2, [sp, #0x20]
003a6c5c: str r3, [r4, #0x4b0]
003a6c60: ldr r0, [sp, #0x20]
003a6c64: bl #0x814f84
003a6c68: ldr r1, [pc, #0x28c]
003a6c6c: ldr r3, [r4, #0x4d8]
003a6c70: ldr r0, [r5, r7]
003a6c74: ldr r1, [r5, r1]
003a6c78: cmp r3, #0
003a6c7c: mov sl, #0
003a6c80: add r7, r1, #8
003a6c84: mov fp, #0
003a6c88: mov ip, #0x4c0
003a6c8c: strd sl, fp, [r4, ip]
003a6c90: add lr, r0, #8
003a6c94: mvn r2, #0
003a6c98: mov r3, #0
003a6c9c: str r7, [r4, #0x4b8]
003a6ca0: mov r0, #0x10
003a6ca4: addeq r7, r4, #0x4b0
003a6ca8: str lr, [r4, #0x490]
003a6cac: str r0, [r4, #0x4bc]
003a6cb0: str r2, [r4, #0x4cc]
003a6cb4: str r2, [r4, #0x4c8]
003a6cb8: str r3, [r4, #0x4d0]
003a6cbc: strb r3, [r4, #0x4d4]
003a6cc0: addeq r7, r7, #8
003a6cc4: beq #0x3a6cdc
003a6cc8: add r7, r4, #0x4b0
003a6ccc: add r7, r7, #8
003a6cd0: str r3, [r4, #0x4d8]
003a6cd4: mov r0, r7
003a6cd8: bl #0x814f84
003a6cdc: ldr ip, [pc, #0x21c]
003a6ce0: ldr r2, [pc, #0x21c]
003a6ce4: ldrb r3, [r4, #0x4fd]
003a6ce8: ldr ip, [r5, ip]
003a6cec: ldr r0, [r5, r2]
003a6cf0: cmp r3, #0
003a6cf4: mov fp, #0
003a6cf8: add ip, ip, #8
003a6cfc: movw lr, #0x4e8
003a6d00: mov sl, #0
003a6d04: strd sl, fp, [r4, lr]
003a6d08: mvn r1, #0
003a6d0c: mov r3, #0
003a6d10: str ip, [r4, #0x4b8]
003a6d14: add r0, r0, #8
003a6d18: mov ip, #1
003a6d1c: addeq fp, r4, #0x4e0
003a6d20: str ip, [r4, #0x4e4]
003a6d24: str r1, [r4, #0x4f4]
003a6d28: str r0, [r4, #0x4e0]
003a6d2c: str r1, [r4, #0x4f0]
003a6d30: str r3, [r4, #0x4f8]
003a6d34: strb r3, [r4, #0x4fc]
003a6d38: streq fp, [sp, #0x1c]
003a6d3c: beq #0x3a6d5c
003a6d40: add r0, r4, #0x4e0
003a6d44: str r0, [sp, #0x1c]
003a6d48: strb r3, [r4, #0x4fd]
003a6d4c: ldr r0, [sp, #0x1c]
003a6d50: str r2, [sp, #0x10]
003a6d54: bl #0x814f84
003a6d58: ldr r2, [sp, #0x10]
003a6d5c: ldr r3, [pc, #0x1a4]
003a6d60: ldrb r1, [r4, #0x51d]
003a6d64: ldr ip, [r5, r2]
003a6d68: ldr lr, [r5, r3]
003a6d6c: movw sb, #0x508
003a6d70: mov sl, #0
003a6d74: add lr, lr, #8
003a6d78: mov fp, #0
003a6d7c: strd sl, fp, [r4, sb]
003a6d80: cmp r1, #0
003a6d84: mvn r0, #0
003a6d88: mov r1, #0
003a6d8c: add ip, ip, #8
003a6d90: str lr, [r4, #0x4e0]
003a6d94: mov lr, #1
003a6d98: str lr, [r4, #0x504]
003a6d9c: str r0, [r4, #0x514]
003a6da0: str ip, [r4, #0x500]
003a6da4: str r0, [r4, #0x510]
003a6da8: str r1, [r4, #0x518]
003a6dac: strb r1, [r4, #0x51c]
003a6db0: addeq sb, r4, #0x500
003a6db4: beq #0x3a6dd8
003a6db8: add sb, r4, #0x500
003a6dbc: strb r1, [r4, #0x51d]
003a6dc0: mov r0, sb
003a6dc4: str r2, [sp, #0x10]
003a6dc8: str r3, [sp, #0xc]
003a6dcc: bl #0x814f84
003a6dd0: ldr r3, [sp, #0xc]
003a6dd4: ldr r2, [sp, #0x10]
003a6dd8: ldr ip, [r5, r3]
003a6ddc: ldr r0, [r5, r2]
003a6de0: ldrb r2, [r4, #0x53d]
003a6de4: add ip, ip, #8
003a6de8: mov sl, #0
003a6dec: movw lr, #0x528
003a6df0: mov fp, #0
003a6df4: strd sl, fp, [r4, lr]
003a6df8: cmp r2, #0
003a6dfc: mvn r1, #0
003a6e00: mov r2, #0
003a6e04: add r0, r0, #8
003a6e08: str ip, [r4, #0x500]
003a6e0c: mov ip, #1
003a6e10: str ip, [r4, #0x524]
003a6e14: str r1, [r4, #0x534]
003a6e18: str r0, [r4, #0x520]
003a6e1c: str r1, [r4, #0x530]
003a6e20: str r2, [r4, #0x538]
003a6e24: strb r2, [r4, #0x53c]
003a6e28: addeq sl, r4, #0x520
003a6e2c: beq #0x3a6e48
003a6e30: add sl, r4, #0x520
003a6e34: strb r2, [r4, #0x53d]
003a6e38: mov r0, sl
003a6e3c: str r3, [sp, #0xc]
003a6e40: bl #0x814f84
003a6e44: ldr r3, [sp, #0xc]
003a6e48: ldr r3, [r5, r3]
003a6e4c: mov r1, r6
003a6e50: mov r0, r4
003a6e54: add r3, r3, #8
003a6e58: str r3, [r4, #0x520]
003a6e5c: bl #0x81324c
003a6e60: mov r0, r4
003a6e64: ldr r1, [sp, #4]
003a6e68: bl #0x81324c
003a6e6c: mov r0, r4
003a6e70: ldr r1, [sp, #0x14]
003a6e74: bl #0x81324c
003a6e78: mov r0, r4
003a6e7c: ldr r1, [sp]
003a6e80: bl #0x81324c
003a6e84: mov r0, r4
003a6e88: ldr r1, [sp, #0x24]
003a6e8c: bl #0x81324c
003a6e90: mov r0, r4
003a6e94: mov r1, r8
003a6e98: bl #0x81324c
003a6e9c: mov r0, r4
003a6ea0: ldr r1, [sp, #0x20]
003a6ea4: bl #0x81324c
003a6ea8: mov r0, r4
003a6eac: mov r1, r7
003a6eb0: bl #0x81324c
003a6eb4: mov r0, r4
003a6eb8: ldr r1, [sp, #0x1c]
003a6ebc: bl #0x81324c
003a6ec0: mov r0, r4
003a6ec4: mov r1, sb
003a6ec8: bl #0x81324c
003a6ecc: mov r0, r4
003a6ed0: mov r1, sl
003a6ed4: bl #0x81324c
003a6ed8: mov r0, r4
003a6edc: add sp, sp, #0x2c
003a6ee0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a6ee4: subseq lr, lr, r4, asr r0
003a6ee8: strdeq r4, r5, [r0], -r8
003a6eec: strheq r4, [r0], -r0
003a6ef0: muleq r0, r8, r2
003a6ef4: andeq r4, r0, r8, rrx
003a6ef8: strdeq r3, r4, [r0], -r0
003a6efc: andeq r2, r0, r4, lsl #19
003a6f00: andeq r3, r0, ip, lsr r5
003a6f04: andeq r3, r0, r8, lsl r0
003a6f08: andeq r0, r0, r8, asr #21

# 0x3a92b4 _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_.clone.21
003a92b4: push {r4, r5, r6, r7, r8, lr}
003a92b8: mov r7, r0
003a92bc: sub sp, sp, #8
003a92c0: mov r6, r1
003a92c4: mov r0, #0x24
003a92c8: mov r1, #0
003a92cc: mov r8, r2
003a92d0: bl #0x310570
003a92d4: ldr r5, [pc, #0x58]
003a92d8: ldr r3, [pc, #0x58]
003a92dc: mov r4, r0
003a92e0: add r5, pc, r5
003a92e4: ldr r3, [r5, r3]
003a92e8: mov r1, r6
003a92ec: add r2, sp, #4
003a92f0: add r3, r3, #8
003a92f4: str r3, [r0], #8
003a92f8: bl #0x3140ec
003a92fc: ldr r3, [pc, #0x38]
003a9300: rsb r8, r7, r8
003a9304: mov r2, #1
003a9308: ldr r3, [r5, r3]
003a930c: strb r2, [r4, #0x20]
003a9310: str r8, [r4, #4]
003a9314: add r3, r3, #8
003a9318: str r3, [r4]
003a931c: mov r0, r7
003a9320: mov r1, r6
003a9324: mov r2, r4
003a9328: bl #0x513ce4
003a932c: add sp, sp, #8
003a9330: pop {r4, r5, r6, r7, r8, pc}
003a9334: ldrheq fp, [lr], #-0x70
003a9338: andeq r2, r0, r0, lsr r3
003a933c: andeq r3, r0, ip, asr #28

# 0x3a9340 _ZN9CharacterC2EN10ObjectBase6GO_IDSE
003a9340: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a9344: add ip, r0, #0x374
003a9348: sub sp, sp, #0x3c
003a934c: mov r4, r0
003a9350: str ip, [sp, #0xc]
003a9354: bl #0x38c398
003a9358: ldr ip, [sp, #0xc]
003a935c: add r5, r4, #0x4f0
003a9360: add r5, r5, #0xc
003a9364: mov r0, ip
003a9368: bl #0x404db8
003a936c: add r0, r4, #0x3b4
003a9370: str r0, [sp, #0x20]
003a9374: add r0, r4, #0x37c
003a9378: bl #0x3ff330
003a937c: add r2, r4, #0x490
003a9380: add r1, r4, #0x3c8
003a9384: add r2, r2, #0xc
003a9388: ldr r0, [sp, #0x20]
003a938c: str r1, [sp, #0x1c]
003a9390: str r2, [sp, #0x14]
003a9394: bl #0x3dbb0c
003a9398: ldr r0, [sp, #0x1c]
003a939c: bl #0x3cebf0
003a93a0: ldr r0, [sp, #0x14]
003a93a4: bl #0x3c8ff4
003a93a8: add r3, r4, #0x560
003a93ac: mov r0, r5
003a93b0: str r3, [sp, #0x18]
003a93b4: ldr sb, [pc, #0x50c]
003a93b8: bl #0x3c1b58
003a93bc: ldr r0, [sp, #0x18]
003a93c0: bl #0x3df084
003a93c4: ldr lr, [pc, #0x500]
003a93c8: add sb, pc, sb
003a93cc: mov r8, #0
003a93d0: ldr lr, [sb, lr]
003a93d4: mov fp, #1
003a93d8: mvn r6, #0
003a93dc: add sl, lr, #0x324
003a93e0: str sl, [sp, #0x34]
003a93e4: add sl, lr, #0x180
003a93e8: str sl, [sp, #0x10]
003a93ec: add sl, lr, #0x1f4
003a93f0: str sl, [sp, #0x24]
003a93f4: add sl, lr, #0x220
003a93f8: str sl, [sp, #0x28]
003a93fc: add sl, lr, #0x230
003a9400: str sl, [sp, #0x2c]
003a9404: add r0, lr, #8
003a9408: add r1, lr, #0x15c
003a940c: add r2, lr, #0x168
003a9410: add sl, lr, #0x304
003a9414: str sl, [sp, #0x30]
003a9418: stm r4, {r0, r1}
003a941c: str r2, [r4, #0x24]
003a9420: ldr r0, [sp, #0x10]
003a9424: add lr, lr, #0x314
003a9428: add r7, r4, #0x1380
003a942c: str r0, [r4, #0x374]
003a9430: ldr r1, [sp, #0x24]
003a9434: add r3, r7, #0x18
003a9438: movw sl, #0x13a8
003a943c: str r1, [r4, #0x37c]
003a9440: ldr r2, [sp, #0x28]
003a9444: add r7, r7, #0x30
003a9448: str r2, [r4, #0x3b4]
003a944c: ldr r0, [sp, #0x2c]
003a9450: str r0, [r4, #0x3c8]
003a9454: ldr r1, [sp, #0x30]
003a9458: str lr, [r4, #0x4fc]
003a945c: mov r0, r3
003a9460: str r1, [r4, #0x49c]
003a9464: ldr r2, [sp, #0x34]
003a9468: mov r1, #0x10
003a946c: str r2, [r4, #0x560]
003a9470: movw r2, #0x1394
003a9474: strb r8, [r4, r2]
003a9478: movw r2, #0x1395
003a947c: strb r8, [r4, r2]
003a9480: movw r2, #0x1396
003a9484: strb fp, [r4, r2]
003a9488: movw r2, #0x1397
003a948c: strb r6, [r4, r2]
003a9490: movw r2, #0x13ac
003a9494: str r3, [r4, r2]
003a9498: str r3, [r4, sl]
003a949c: bl #0x31167c
003a94a0: ldr r3, [r4, sl]
003a94a4: mov sl, #0x13c0
003a94a8: mov r0, r7
003a94ac: strb r8, [r3]
003a94b0: movw r3, #0x13c4
003a94b4: str r7, [r4, r3]
003a94b8: mov r1, #0x10
003a94bc: str r7, [r4, sl]
003a94c0: bl #0x31167c
003a94c4: ldr r2, [r4, sl]
003a94c8: add r7, r4, sl
003a94cc: add r3, r7, #0xc
003a94d0: strb r8, [r2]
003a94d4: movw r2, #0x13c8
003a94d8: strh r6, [r4, r2]
003a94dc: movw r2, #0x13ca
003a94e0: strh r6, [r4, r2]
003a94e4: movw sl, #0x13dc
003a94e8: movw r2, #0x13e0
003a94ec: str r3, [r4, r2]
003a94f0: mov r0, r3
003a94f4: str r3, [r4, sl]
003a94f8: mov r1, #0x10
003a94fc: bl #0x31167c
003a9500: ldr r3, [r4, sl]
003a9504: add r7, r7, #0x28
003a9508: movw sl, #0x13f8
003a950c: strb r8, [r3]
003a9510: movw r3, #0x13e4
003a9514: strb fp, [r4, r3]
003a9518: movw r3, #0x13fc
003a951c: str r7, [r4, r3]
003a9520: mov r0, r7
003a9524: str r7, [r4, sl]
003a9528: mov r1, #0x10
003a952c: bl #0x31167c
003a9530: ldr r3, [r4, sl]
003a9534: add r7, r4, #0x1400
003a9538: movw sl, #0x1410
003a953c: strb r8, [r3]
003a9540: movw r3, #0x1414
003a9544: str r7, [r4, r3]
003a9548: mov r0, r7
003a954c: str r7, [r4, sl]
003a9550: mov r1, #0x10
003a9554: bl #0x31167c
003a9558: ldr r3, [r4, sl]
003a955c: add r7, r7, #0x18
003a9560: movw sl, #0x1428
003a9564: strb r8, [r3]
003a9568: movw r3, #0x142c
003a956c: str r7, [r4, r3]
003a9570: mov r0, r7
003a9574: str r7, [r4, sl]
003a9578: mov r1, #0x10
003a957c: bl #0x31167c
003a9580: ldr r2, [r4, sl]
003a9584: mov r3, #0
003a9588: mov r1, #0xbf000000
003a958c: strb r8, [r2]
003a9590: movw r2, #0x14a8
003a9594: strb r6, [r4, r2]
003a9598: movw r2, #0x1430
003a959c: strb fp, [r4, r2]
003a95a0: movw r2, #0x1434
003a95a4: str r8, [r4, r2]
003a95a8: movw r2, #0x1438
003a95ac: str r8, [r4, r2]
003a95b0: movw r2, #0x1448
003a95b4: strb fp, [r4, r2]
003a95b8: movw r2, #0x1449
003a95bc: strb r8, [r4, r2]
003a95c0: movw r2, #0x144c
003a95c4: str r8, [r4, r2]
003a95c8: movw r2, #0x1450
003a95cc: str r3, [r4, r2]
003a95d0: movw r2, #0x1454
003a95d4: str r3, [r4, r2]
003a95d8: movw r2, #0x1458
003a95dc: str r3, [r4, r2]
003a95e0: movw r2, #0x145c
003a95e4: str r3, [r4, r2]
003a95e8: movw r2, #0x1460
003a95ec: str r3, [r4, r2]
003a95f0: movw r2, #0x1464
003a95f4: str r3, [r4, r2]
003a95f8: movw r2, #0x1468
003a95fc: str r3, [r4, r2]
003a9600: movw r2, #0x146c
003a9604: str r3, [r4, r2]
003a9608: movw r2, #0x1470
003a960c: str r3, [r4, r2]
003a9610: movw r2, #0x1474
003a9614: str r3, [r4, r2]
003a9618: movw r2, #0x1478
003a961c: str r3, [r4, r2]
003a9620: movw r2, #0x147c
003a9624: str r3, [r4, r2]
003a9628: mov r2, #0x1480
003a962c: strb r8, [r4, r2]
003a9630: movw r2, #0x1481
003a9634: strb r8, [r4, r2]
003a9638: movw r2, #0x1484
003a963c: str r8, [r4, r2]
003a9640: movw r2, #0x1488
003a9644: str r8, [r4, r2]
003a9648: movw r2, #0x148c
003a964c: str r8, [r4, r2]
003a9650: movw r2, #0x1490
003a9654: str r8, [r4, r2]
003a9658: movw r2, #0x1494
003a965c: str r8, [r4, r2]
003a9660: movw r2, #0x1498
003a9664: str r6, [r4, r2]
003a9668: movw r2, #0x149c
003a966c: str r8, [r4, r2]
003a9670: movw r2, #0x14a0
003a9674: str r8, [r4, r2]
003a9678: movw r2, #0x14a4
003a967c: str r8, [r4, r2]
003a9680: movw r2, #0x14aa
003a9684: strh r8, [r4, r2]
003a9688: movw r2, #0x14ac
003a968c: strb r8, [r4, r2]
003a9690: movw r2, #0x14d8
003a9694: str r3, [r4, r2]
003a9698: add r1, r1, #0x800000
003a969c: movw r2, #0x14fc
003a96a0: str r1, [r4, r2]
003a96a4: movw r2, #0x1504
003a96a8: str r6, [r4, r2]
003a96ac: movw r2, #0x14ad
003a96b0: strb r8, [r4, r2]
003a96b4: movw r2, #0x14b0
003a96b8: str r3, [r4, r2]
003a96bc: movw r2, #0x14b4
003a96c0: str r3, [r4, r2]
003a96c4: movw r2, #0x14b8
003a96c8: str r3, [r4, r2]
003a96cc: movw r2, #0x14bc
003a96d0: str r3, [r4, r2]
003a96d4: mov r2, #0x14c0
003a96d8: str r3, [r4, r2]
003a96dc: movw r2, #0x14c4
003a96e0: str r3, [r4, r2]
003a96e4: movw r3, #0x14c8
003a96e8: strb r8, [r4, r3]
003a96ec: movw r3, #0x14ca
003a96f0: strh r6, [r4, r3]
003a96f4: movw r3, #0x14cc
003a96f8: str r8, [r4, r3]
003a96fc: movw r3, #0x14d0
003a9700: strh r8, [r4, r3]
003a9704: movw r3, #0x14d4
003a9708: str r8, [r4, r3]
003a970c: movw r3, #0x14dc
003a9710: strb r8, [r4, r3]
003a9714: movw r3, #0x14e4
003a9718: strb r8, [r4, r3]
003a971c: movw r3, #0x14e5
003a9720: strb r8, [r4, r3]
003a9724: movw r3, #0x14e8
003a9728: str r8, [r4, r3]
003a972c: movw r3, #0x14ec
003a9730: str r8, [r4, r3]
003a9734: add r7, r4, #0x1500
003a9738: movw r3, #0x14f0
003a973c: add r0, r4, #0x1a40
003a9740: strb r8, [r4, r3]
003a9744: add r0, r0, #8
003a9748: mov r3, #0x1500
003a974c: add r7, r7, #8
003a9750: str r6, [r4, r3]
003a9754: str r0, [sp, #0x10]
003a9758: mov r0, r7
003a975c: bl #0x3a6a24
003a9760: ldr r0, [sp, #0x10]
003a9764: bl #0x3a6a24
003a9768: add r0, r4, #0x304
003a976c: mov r1, r4
003a9770: strb fp, [r4, #0x28]
003a9774: bl #0x4a191c
003a9778: strb fp, [r4, #0x1c4]
003a977c: strb fp, [r4, #0x85]
003a9780: mov r0, #0x10
003a9784: mov r1, r8
003a9788: bl #0x310570
003a978c: ldr r3, [pc, #0x13c]
003a9790: ldr ip, [sp, #0xc]
003a9794: mov r6, r0
003a9798: ldr r3, [sb, r3]
003a979c: cmp ip, r8
003a97a0: strb r8, [r6, #0xa]
003a97a4: add r3, r3, #8
003a97a8: str r8, [r0, #0xc]
003a97ac: stm r0, {r3, ip}
003a97b0: strb r8, [r6, #8]
003a97b4: strb r8, [r6, #9]
003a97b8: beq #0x3a986c
003a97bc: mov r0, ip
003a97c0: mov r1, r6
003a97c4: bl #0x404e10
003a97c8: ldr r3, [r4, #0x378]
003a97cc: ldr r0, [sp, #0x20]
003a97d0: mov r1, r4
003a97d4: str r4, [r3, #0xc]
003a97d8: bl #0x3db480
003a97dc: ldr r0, [sp, #0x1c]
003a97e0: mov r1, r4
003a97e4: bl #0x3cb7c0
003a97e8: ldr r0, [sp, #0x14]
003a97ec: mov r1, r4
003a97f0: bl #0x3c9890
003a97f4: mov r0, r5
003a97f8: mov r1, r4
003a97fc: bl #0x3c1600
003a9800: ldr r0, [sp, #0x18]
003a9804: mov r1, r4
003a9808: bl #0x3dec0c
003a980c: mov r6, #0
003a9810: str r4, [r4, #0x380]
003a9814: mov r1, r6
003a9818: mov r0, r5
003a981c: add r6, r6, #1
003a9820: bl #0x3c7318
003a9824: cmp r6, #0x14
003a9828: bne #0x3a9814
003a982c: mov r1, #0
003a9830: movw r2, #0x14e0
003a9834: str r1, [r4, r2]
003a9838: mvn r3, #0
003a983c: movw r2, #0x14f4
003a9840: str r3, [r4, r2]
003a9844: str r7, [r4, #0x100]
003a9848: ldr sl, [sp, #0x10]
003a984c: movw r2, #0x14f8
003a9850: mov r0, r4
003a9854: str sl, [r4, #0x104]
003a9858: str r3, [r4, r2]
003a985c: mov r3, #1
003a9860: strb r3, [r4, #0xf8]
003a9864: add sp, sp, #0x3c
003a9868: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a986c: ldr r3, [pc, #0x60]
003a9870: ldr r3, [sb, r3]
003a9874: ldr r3, [r3]
003a9878: cmp r3, #2
003a987c: streq ip, [r4, #0x374]
003a9880: beq #0x3a97bc
003a9884: cmp r3, #1
003a9888: bne #0x3a97bc
003a988c: ldr r0, [pc, #0x44]
003a9890: ldr r1, [pc, #0x44]
003a9894: ldr r2, [pc, #0x44]
003a9898: ldr r0, [sb, r0]
003a989c: ldr r3, [pc, #0x40]
003a98a0: mov lr, #0x44
003a98a4: add r1, pc, r1
003a98a8: add r0, r0, #0xa8
003a98ac: add r2, pc, r2
003a98b0: add r3, pc, r3
003a98b4: str ip, [sp, #0xc]
003a98b8: str lr, [sp]
003a98bc: bl #0x30e004
003a98c0: ldr ip, [sp, #0xc]
003a98c4: b #0x3a97bc
003a98c8: subseq fp, lr, r8, asr #13
003a98cc: andeq r2, r0, r8, lsl #28
003a98d0: andeq r2, r0, r4, lsr #21
003a98d4: andeq r3, r0, r0, asr #19
003a98d8: andeq r1, r0, r0, asr #19
003a98dc: subseq r4, r1, r4, lsr fp
003a98e0: subseq sb, r1, r4, lsl ip
003a98e4: subseq sb, r1, r0, lsr #24

# 0x3a9fdc _ZThn4_N9Character17DeclarePropertiesEv
003a9fdc: sub r0, r0, #4
003a9fe0: b #0x3a9fe4

# 0x3a9fe4 _ZN9Character17DeclarePropertiesEv
003a9fe4: push {r4, r5, r6, r7, r8, sb, sl, lr}
003a9fe8: sub sp, sp, #8
003a9fec: mov r7, r0
003a9ff0: bl #0x38cee8
003a9ff4: ldr r1, [pc, #0x180]
003a9ff8: add r4, r7, #4
003a9ffc: add r5, r7, #0x1380
003aa000: mov r0, r4
003aa004: add r2, r5, #0x30
003aa008: add r1, pc, r1
003aa00c: bl #0x33ef7c
003aa010: ldr r1, [pc, #0x168]
003aa014: add r2, r5, #0x18
003aa018: mov r0, r4
003aa01c: add r1, pc, r1
003aa020: bl #0x33ef7c
003aa024: ldr r1, [pc, #0x158]
003aa028: add r5, r7, #0x13c0
003aa02c: mov r0, r4
003aa030: add r2, r5, #0xc
003aa034: add r1, pc, r1
003aa038: bl #0x33ef7c
003aa03c: ldr r1, [pc, #0x144]
003aa040: mov r0, r4
003aa044: add r2, r5, #0x24
003aa048: add r1, pc, r1
003aa04c: bl #0x3a92b4
003aa050: ldr r1, [pc, #0x134]
003aa054: add r2, r5, #0x28
003aa058: mov r0, r4
003aa05c: add r1, pc, r1
003aa060: bl #0x33ef7c
003aa064: ldr r1, [pc, #0x124]
003aa068: add r7, r7, #0x1400
003aa06c: mov r0, r4
003aa070: mov r2, r7
003aa074: add r1, pc, r1
003aa078: bl #0x33ef7c
003aa07c: ldr r1, [pc, #0x110]
003aa080: mov r0, r4
003aa084: add r2, r7, #0x18
003aa088: add r1, pc, r1
003aa08c: bl #0x33ef7c
003aa090: ldr r1, [pc, #0x100]
003aa094: add r2, r7, #0x30
003aa098: mov r0, r4
003aa09c: add r1, pc, r1
003aa0a0: bl #0x3a92b4
003aa0a4: mov r1, #0
003aa0a8: mov r0, #0x28
003aa0ac: bl #0x310570
003aa0b0: ldr r5, [pc, #0xe4]
003aa0b4: ldr sb, [pc, #0xe4]
003aa0b8: ldr r8, [pc, #0xe4]
003aa0bc: add r5, pc, r5
003aa0c0: ldr sb, [r5, sb]
003aa0c4: add r8, pc, r8
003aa0c8: mov r6, r0
003aa0cc: add sb, sb, #8
003aa0d0: mov r1, r8
003aa0d4: add r2, sp, #4
003aa0d8: str sb, [r0], #8
003aa0dc: bl #0x3140ec
003aa0e0: ldr r3, [pc, #0xc0]
003aa0e4: add r2, r7, #0x34
003aa0e8: mov sl, #0
003aa0ec: ldr r3, [r5, r3]
003aa0f0: rsb r2, r4, r2
003aa0f4: str r2, [r6, #4]
003aa0f8: add r3, r3, #8
003aa0fc: str r3, [r6]
003aa100: mov r2, r6
003aa104: mov r1, r8
003aa108: str sl, [r6, #0x20]
003aa10c: str sl, [r6, #0x24]
003aa110: mov r0, r4
003aa114: bl #0x513ce4
003aa118: mov r1, sl
003aa11c: mov r0, #0x24
003aa120: bl #0x310570
003aa124: ldr r8, [pc, #0x80]
003aa128: mov r6, r0
003aa12c: mov r2, sp
003aa130: add r8, pc, r8
003aa134: mov r1, r8
003aa138: str sb, [r0], #8
003aa13c: bl #0x3140ec
003aa140: ldr r3, [pc, #0x68]
003aa144: add r7, r7, #0x3c
003aa148: rsb r7, r4, r7
003aa14c: ldr r3, [r5, r3]
003aa150: str r7, [r6, #4]
003aa154: mov r0, r4
003aa158: add r3, r3, #8
003aa15c: str r3, [r6]
003aa160: mov r3, #0
003aa164: str r3, [r6, #0x20]
003aa168: mov r1, r8
003aa16c: mov r2, r6
003aa170: bl #0x513ce4
003aa174: add sp, sp, #8
003aa178: pop {r4, r5, r6, r7, r8, sb, sl, pc}

# 0x3aa1b4 _ZN9CharacterC1EN10ObjectBase6GO_IDSE
003aa1b4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8: add ip, r0, #0x374
003aa1bc: sub sp, sp, #0x3c
003aa1c0: mov r4, r0
003aa1c4: str ip, [sp, #0xc]
003aa1c8: bl #0x38c398
003aa1cc: ldr ip, [sp, #0xc]
003aa1d0: add r5, r4, #0x4f0
003aa1d4: add r5, r5, #0xc
003aa1d8: mov r0, ip
003aa1dc: bl #0x404db8
003aa1e0: add r0, r4, #0x3b4
003aa1e4: str r0, [sp, #0x20]
003aa1e8: add r0, r4, #0x37c
003aa1ec: bl #0x3ff330
003aa1f0: add r2, r4, #0x490
003aa1f4: add r1, r4, #0x3c8
003aa1f8: add r2, r2, #0xc
003aa1fc: ldr r0, [sp, #0x20]
003aa200: str r1, [sp, #0x1c]
003aa204: str r2, [sp, #0x14]
003aa208: bl #0x3dbb0c
003aa20c: ldr r0, [sp, #0x1c]
003aa210: bl #0x3cebf0
003aa214: ldr r0, [sp, #0x14]
003aa218: bl #0x3c8ff4
003aa21c: add r3, r4, #0x560
003aa220: mov r0, r5
003aa224: str r3, [sp, #0x18]
003aa228: ldr sb, [pc, #0x50c]
003aa22c: bl #0x3c1b58
003aa230: ldr r0, [sp, #0x18]
003aa234: bl #0x3df084
003aa238: ldr lr, [pc, #0x500]
003aa23c: add sb, pc, sb
003aa240: mov r8, #0
003aa244: ldr lr, [sb, lr]
003aa248: mov fp, #1
003aa24c: mvn r6, #0
003aa250: add sl, lr, #0x324
003aa254: str sl, [sp, #0x34]
003aa258: add sl, lr, #0x180
003aa25c: str sl, [sp, #0x10]
003aa260: add sl, lr, #0x1f4
003aa264: str sl, [sp, #0x24]
003aa268: add sl, lr, #0x220
003aa26c: str sl, [sp, #0x28]
003aa270: add sl, lr, #0x230
003aa274: str sl, [sp, #0x2c]
003aa278: add r0, lr, #8
003aa27c: add r1, lr, #0x15c
003aa280: add r2, lr, #0x168
003aa284: add sl, lr, #0x304
003aa288: str sl, [sp, #0x30]
003aa28c: stm r4, {r0, r1}
003aa290: str r2, [r4, #0x24]
003aa294: ldr r0, [sp, #0x10]
003aa298: add lr, lr, #0x314
003aa29c: add r7, r4, #0x1380
003aa2a0: str r0, [r4, #0x374]
003aa2a4: ldr r1, [sp, #0x24]
003aa2a8: add r3, r7, #0x18
003aa2ac: movw sl, #0x13a8
003aa2b0: str r1, [r4, #0x37c]
003aa2b4: ldr r2, [sp, #0x28]
003aa2b8: add r7, r7, #0x30
003aa2bc: str r2, [r4, #0x3b4]
003aa2c0: ldr r0, [sp, #0x2c]
003aa2c4: str r0, [r4, #0x3c8]
003aa2c8: ldr r1, [sp, #0x30]
003aa2cc: str lr, [r4, #0x4fc]
003aa2d0: mov r0, r3
003aa2d4: str r1, [r4, #0x49c]
003aa2d8: ldr r2, [sp, #0x34]
003aa2dc: mov r1, #0x10
003aa2e0: str r2, [r4, #0x560]
003aa2e4: movw r2, #0x1394
003aa2e8: strb r8, [r4, r2]
003aa2ec: movw r2, #0x1395
003aa2f0: strb r8, [r4, r2]
003aa2f4: movw r2, #0x1396
003aa2f8: strb fp, [r4, r2]
003aa2fc: movw r2, #0x1397
003aa300: strb r6, [r4, r2]
003aa304: movw r2, #0x13ac
003aa308: str r3, [r4, r2]
003aa30c: str r3, [r4, sl]
003aa310: bl #0x31167c
003aa314: ldr r3, [r4, sl]
003aa318: mov sl, #0x13c0
003aa31c: mov r0, r7
003aa320: strb r8, [r3]
003aa324: movw r3, #0x13c4
003aa328: str r7, [r4, r3]
003aa32c: mov r1, #0x10
003aa330: str r7, [r4, sl]
003aa334: bl #0x31167c
003aa338: ldr r2, [r4, sl]
003aa33c: add r7, r4, sl
003aa340: add r3, r7, #0xc
003aa344: strb r8, [r2]
003aa348: movw r2, #0x13c8
003aa34c: strh r6, [r4, r2]
003aa350: movw r2, #0x13ca
003aa354: strh r6, [r4, r2]
003aa358: movw sl, #0x13dc
003aa35c: movw r2, #0x13e0
003aa360: str r3, [r4, r2]
003aa364: mov r0, r3
003aa368: str r3, [r4, sl]
003aa36c: mov r1, #0x10
003aa370: bl #0x31167c
003aa374: ldr r3, [r4, sl]
003aa378: add r7, r7, #0x28
003aa37c: movw sl, #0x13f8
003aa380: strb r8, [r3]
003aa384: movw r3, #0x13e4
003aa388: strb fp, [r4, r3]
003aa38c: movw r3, #0x13fc
003aa390: str r7, [r4, r3]
003aa394: mov r0, r7
003aa398: str r7, [r4, sl]
003aa39c: mov r1, #0x10
003aa3a0: bl #0x31167c
003aa3a4: ldr r3, [r4, sl]
003aa3a8: add r7, r4, #0x1400
003aa3ac: movw sl, #0x1410
003aa3b0: strb r8, [r3]
003aa3b4: movw r3, #0x1414
003aa3b8: str r7, [r4, r3]
003aa3bc: mov r0, r7
003aa3c0: str r7, [r4, sl]
003aa3c4: mov r1, #0x10
003aa3c8: bl #0x31167c
003aa3cc: ldr r3, [r4, sl]
003aa3d0: add r7, r7, #0x18
003aa3d4: movw sl, #0x1428
003aa3d8: strb r8, [r3]
003aa3dc: movw r3, #0x142c
003aa3e0: str r7, [r4, r3]
003aa3e4: mov r0, r7
003aa3e8: str r7, [r4, sl]
003aa3ec: mov r1, #0x10
003aa3f0: bl #0x31167c
003aa3f4: ldr r2, [r4, sl]
003aa3f8: mov r3, #0
003aa3fc: mov r1, #0xbf000000
003aa400: strb r8, [r2]
003aa404: movw r2, #0x14a8
003aa408: strb r6, [r4, r2]
003aa40c: movw r2, #0x1430
003aa410: strb fp, [r4, r2]
003aa414: movw r2, #0x1434
003aa418: str r8, [r4, r2]
003aa41c: movw r2, #0x1438
003aa420: str r8, [r4, r2]
003aa424: movw r2, #0x1448
003aa428: strb fp, [r4, r2]
003aa42c: movw r2, #0x1449
003aa430: strb r8, [r4, r2]
003aa434: movw r2, #0x144c
003aa438: str r8, [r4, r2]
003aa43c: movw r2, #0x1450
003aa440: str r3, [r4, r2]
003aa444: movw r2, #0x1454
003aa448: str r3, [r4, r2]
003aa44c: movw r2, #0x1458
003aa450: str r3, [r4, r2]
003aa454: movw r2, #0x145c
003aa458: str r3, [r4, r2]
003aa45c: movw r2, #0x1460
003aa460: str r3, [r4, r2]
003aa464: movw r2, #0x1464
003aa468: str r3, [r4, r2]
003aa46c: movw r2, #0x1468
003aa470: str r3, [r4, r2]
003aa474: movw r2, #0x146c
003aa478: str r3, [r4, r2]
003aa47c: movw r2, #0x1470
003aa480: str r3, [r4, r2]
003aa484: movw r2, #0x1474
003aa488: str r3, [r4, r2]
003aa48c: movw r2, #0x1478
003aa490: str r3, [r4, r2]
003aa494: movw r2, #0x147c
003aa498: str r3, [r4, r2]
003aa49c: mov r2, #0x1480
003aa4a0: strb r8, [r4, r2]
003aa4a4: movw r2, #0x1481
003aa4a8: strb r8, [r4, r2]
003aa4ac: movw r2, #0x1484
003aa4b0: str r8, [r4, r2]
003aa4b4: movw r2, #0x1488
003aa4b8: str r8, [r4, r2]
003aa4bc: movw r2, #0x148c
003aa4c0: str r8, [r4, r2]
003aa4c4: movw r2, #0x1490
003aa4c8: str r8, [r4, r2]
003aa4cc: movw r2, #0x1494
003aa4d0: str r8, [r4, r2]
003aa4d4: movw r2, #0x1498
003aa4d8: str r6, [r4, r2]
003aa4dc: movw r2, #0x149c
003aa4e0: str r8, [r4, r2]
003aa4e4: movw r2, #0x14a0
003aa4e8: str r8, [r4, r2]
003aa4ec: movw r2, #0x14a4
003aa4f0: str r8, [r4, r2]
003aa4f4: movw r2, #0x14aa
003aa4f8: strh r8, [r4, r2]
003aa4fc: movw r2, #0x14ac
003aa500: strb r8, [r4, r2]
003aa504: movw r2, #0x14d8
003aa508: str r3, [r4, r2]
003aa50c: add r1, r1, #0x800000
003aa510: movw r2, #0x14fc
003aa514: str r1, [r4, r2]
003aa518: movw r2, #0x1504
003aa51c: str r6, [r4, r2]
003aa520: movw r2, #0x14ad
003aa524: strb r8, [r4, r2]
003aa528: movw r2, #0x14b0
003aa52c: str r3, [r4, r2]
003aa530: movw r2, #0x14b4
003aa534: str r3, [r4, r2]
003aa538: movw r2, #0x14b8
003aa53c: str r3, [r4, r2]
003aa540: movw r2, #0x14bc
003aa544: str r3, [r4, r2]
003aa548: mov r2, #0x14c0
003aa54c: str r3, [r4, r2]
003aa550: movw r2, #0x14c4
003aa554: str r3, [r4, r2]
003aa558: movw r3, #0x14c8
003aa55c: strb r8, [r4, r3]
003aa560: movw r3, #0x14ca
003aa564: strh r6, [r4, r3]
003aa568: movw r3, #0x14cc
003aa56c: str r8, [r4, r3]
003aa570: movw r3, #0x14d0
003aa574: strh r8, [r4, r3]
003aa578: movw r3, #0x14d4
003aa57c: str r8, [r4, r3]
003aa580: movw r3, #0x14dc
003aa584: strb r8, [r4, r3]
003aa588: movw r3, #0x14e4
003aa58c: strb r8, [r4, r3]
003aa590: movw r3, #0x14e5
003aa594: strb r8, [r4, r3]
003aa598: movw r3, #0x14e8
003aa59c: str r8, [r4, r3]
003aa5a0: movw r3, #0x14ec
003aa5a4: str r8, [r4, r3]
003aa5a8: add r7, r4, #0x1500
003aa5ac: movw r3, #0x14f0
003aa5b0: add r0, r4, #0x1a40
003aa5b4: strb r8, [r4, r3]
003aa5b8: add r0, r0, #8
003aa5bc: mov r3, #0x1500
003aa5c0: add r7, r7, #8
003aa5c4: str r6, [r4, r3]
003aa5c8: str r0, [sp, #0x10]
003aa5cc: mov r0, r7
003aa5d0: bl #0x3a6a24
003aa5d4: ldr r0, [sp, #0x10]
003aa5d8: bl #0x3a6a24
003aa5dc: add r0, r4, #0x304
003aa5e0: mov r1, r4
003aa5e4: strb fp, [r4, #0x28]
003aa5e8: bl #0x4a191c
003aa5ec: strb fp, [r4, #0x1c4]
003aa5f0: strb fp, [r4, #0x85]
003aa5f4: mov r0, #0x10
003aa5f8: mov r1, r8
003aa5fc: bl #0x310570
003aa600: ldr r3, [pc, #0x13c]
003aa604: ldr ip, [sp, #0xc]
003aa608: mov r6, r0
003aa60c: ldr r3, [sb, r3]
003aa610: cmp ip, r8
003aa614: strb r8, [r6, #0xa]
003aa618: add r3, r3, #8
003aa61c: str r8, [r0, #0xc]
003aa620: stm r0, {r3, ip}
003aa624: strb r8, [r6, #8]
003aa628: strb r8, [r6, #9]
003aa62c: beq #0x3aa6e0
003aa630: mov r0, ip
003aa634: mov r1, r6
003aa638: bl #0x404e10
003aa63c: ldr r3, [r4, #0x378]
003aa640: ldr r0, [sp, #0x20]
003aa644: mov r1, r4
003aa648: str r4, [r3, #0xc]
003aa64c: bl #0x3db480
003aa650: ldr r0, [sp, #0x1c]
003aa654: mov r1, r4
003aa658: bl #0x3cb7c0
003aa65c: ldr r0, [sp, #0x14]
003aa660: mov r1, r4
003aa664: bl #0x3c9890
003aa668: mov r0, r5
003aa66c: mov r1, r4
003aa670: bl #0x3c1600
003aa674: ldr r0, [sp, #0x18]
003aa678: mov r1, r4
003aa67c: bl #0x3dec0c
003aa680: mov r6, #0
003aa684: str r4, [r4, #0x380]
003aa688: mov r1, r6
003aa68c: mov r0, r5
003aa690: add r6, r6, #1
003aa694: bl #0x3c7318
003aa698: cmp r6, #0x14
003aa69c: bne #0x3aa688
003aa6a0: mov r1, #0
003aa6a4: movw r2, #0x14e0
003aa6a8: str r1, [r4, r2]
003aa6ac: mvn r3, #0
003aa6b0: movw r2, #0x14f4
003aa6b4: str r3, [r4, r2]
003aa6b8: str r7, [r4, #0x100]
003aa6bc: ldr sl, [sp, #0x10]
003aa6c0: movw r2, #0x14f8
003aa6c4: mov r0, r4
003aa6c8: str sl, [r4, #0x104]
003aa6cc: str r3, [r4, r2]
003aa6d0: mov r3, #1
003aa6d4: strb r3, [r4, #0xf8]
003aa6d8: add sp, sp, #0x3c
003aa6dc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0: ldr r3, [pc, #0x60]
003aa6e4: ldr r3, [sb, r3]
003aa6e8: ldr r3, [r3]
003aa6ec: cmp r3, #2
003aa6f0: streq ip, [r4, #0x374]
003aa6f4: beq #0x3aa630
003aa6f8: cmp r3, #1
003aa6fc: bne #0x3aa630
003aa700: ldr r0, [pc, #0x44]
003aa704: ldr r1, [pc, #0x44]
003aa708: ldr r2, [pc, #0x44]
003aa70c: ldr r0, [sb, r0]
003aa710: ldr r3, [pc, #0x40]
003aa714: mov lr, #0x44
003aa718: add r1, pc, r1
003aa71c: add r0, r0, #0xa8
003aa720: add r2, pc, r2
003aa724: add r3, pc, r3
003aa728: str ip, [sp, #0xc]
003aa72c: str lr, [sp]
003aa730: bl #0x30e004
003aa734: ldr ip, [sp, #0xc]
003aa738: b #0x3aa630
003aa73c: subseq sl, lr, r4, asr r8
003aa740: andeq r2, r0, r8, lsl #28
003aa744: andeq r2, r0, r4, lsr #21
003aa748: andeq r3, r0, r0, asr #19
003aa74c: andeq r1, r0, r0, asr #19
003aa750: subseq r3, r1, r0, asr #25
003aa754: subseq r8, r1, r0, lsr #27
003aa758: subseq r8, r1, ip, lsr #27

# 0x3b4010 _ZN11POCharacterC2EP13PhysicalWorldP10GameObjectbsttb.clone.2
003b4010: push {r4, r5, r6, r7, lr}
003b4014: sub sp, sp, #0x24
003b4018: ldrh r5, [sp, #0x38]
003b401c: ldrh lr, [sp, #0x3c]
003b4020: ldrb r6, [sp, #0x40]
003b4024: mov ip, #0
003b4028: str r3, [sp, #0xc]
003b402c: mov r7, #1
003b4030: mov r3, ip
003b4034: ldr r4, [pc, #0x44]
003b4038: str r5, [sp, #0x10]
003b403c: str lr, [sp, #0x14]
003b4040: mov r5, r0
003b4044: str ip, [sp, #4]
003b4048: str ip, [sp, #0x18]
003b404c: str r7, [sp]
003b4050: str r6, [sp, #8]
003b4054: bl #0x46f2f0
003b4058: ldr r3, [pc, #0x24]
003b405c: add r4, pc, r4
003b4060: mov r0, r5
003b4064: ldr r3, [r4, r3]
003b4068: add r3, r3, #8
003b406c: str r3, [r5]
003b4070: bl #0x46eb20
003b4074: mov r0, r5
003b4078: add sp, sp, #0x24
003b407c: pop {r4, r5, r6, r7, pc}
003b4080: subseq r0, lr, r4, lsr sl
003b4084: andeq r1, r0, ip, lsr #4

# 0x3ccf9c _ZNSt6vectorIP9CharacterSaIS1_EEC1Ej.clone.3
003ccf9c: push {r4, lr}
003ccfa0: sub sp, sp, #8
003ccfa4: mov r4, r0
003ccfa8: mov r1, #0
003ccfac: add r2, sp, #8
003ccfb0: str r1, [r2, #-4]!
003ccfb4: str r1, [r4]
003ccfb8: str r1, [r4, #4]
003ccfbc: str r1, [r0, #8]!
003ccfc0: bl #0x3ccf2c
003ccfc4: ldr r3, [sp, #4]
003ccfc8: str r0, [r4]
003ccfcc: str r0, [r4, #4]
003ccfd0: add r0, r0, r3, lsl #2
003ccfd4: str r0, [r4, #8]
003ccfd8: mov r0, r4
003ccfdc: add sp, sp, #8
003ccfe0: pop {r4, pc}

# 0x3cde2c _ZN17CharAISkillScriptC1EP9CharacterPKcj
003cde2c: push {r4, r5, r6, r7, r8, sl, lr}
003cde30: ldr r5, [pc, #0x11c]
003cde34: ldr ip, [pc, #0x11c]
003cde38: mov r4, r0
003cde3c: add r5, pc, r5
003cde40: ldr ip, [r5, ip]
003cde44: add r7, r0, #0xc
003cde48: mov sl, r1
003cde4c: add ip, ip, #8
003cde50: str ip, [r0]
003cde54: sub sp, sp, #0xc
003cde58: stmib r4, {r1, r2}
003cde5c: mov r0, r7
003cde60: mov r8, r3
003cde64: mov r6, r2
003cde68: bl #0x3192b4
003cde6c: mvn r3, #0
003cde70: cmp sl, #0
003cde74: str r3, [r4, #0x18]
003cde78: str r8, [r4, #0x14]
003cde7c: beq #0x3cdeac
003cde80: cmp r6, #0
003cde84: beq #0x3cdf00
003cde88: mov r1, r6
003cde8c: mov r0, r7
003cde90: bl #0x39ec10
003cde94: mov r0, r7
003cde98: mov r1, r8
003cde9c: bl #0x3cdd78
003cdea0: mov r0, r4
003cdea4: add sp, sp, #0xc
003cdea8: pop {r4, r5, r6, r7, r8, sl, pc}
003cdeac: ldr r3, [pc, #0xa8]
003cdeb0: ldr r3, [r5, r3]
003cdeb4: ldr r3, [r3]
003cdeb8: cmp r3, #2
003cdebc: streq sl, [sl]
003cdec0: beq #0x3cde80
003cdec4: cmp r3, #1
003cdec8: bne #0x3cde80
003cdecc: ldr r0, [pc, #0x8c]
003cded0: ldr r1, [pc, #0x8c]
003cded4: ldr r2, [pc, #0x8c]
003cded8: ldr r0, [r5, r0]
003cdedc: ldr r3, [pc, #0x88]
003cdee0: mov ip, #0x2e
003cdee4: add r1, pc, r1
003cdee8: add r2, pc, r2
003cdeec: add r3, pc, r3
003cdef0: add r0, r0, #0xa8
003cdef4: str ip, [sp]
003cdef8: bl #0x30e004
003cdefc: b #0x3cde80
003cdf00: ldr r3, [pc, #0x54]
003cdf04: ldr r3, [r5, r3]
003cdf08: ldr r3, [r3]
003cdf0c: cmp r3, #2
003cdf10: streq r6, [r6]
003cdf14: beq #0x3cde88
003cdf18: cmp r3, #1
003cdf1c: bne #0x3cde88
003cdf20: ldr r0, [pc, #0x38]
003cdf24: ldr r1, [pc, #0x44]
003cdf28: ldr r2, [pc, #0x44]
003cdf2c: ldr r0, [r5, r0]
003cdf30: ldr r3, [pc, #0x40]
003cdf34: mov ip, #0x2e
003cdf38: add r1, pc, r1
003cdf3c: add r2, pc, r2
003cdf40: add r3, pc, r3
003cdf44: add r0, r0, #0xa8
003cdf48: str ip, [sp]
003cdf4c: bl #0x30e004
003cdf50: b #0x3cde88
003cdf54: subseq r6, ip, r4, asr ip
003cdf58: andeq r0, r0, ip, lsr #23
003cdf5c: andeq r3, r0, r0, asr #19
003cdf60: andeq r1, r0, r0, asr #19
003cdf64: strdeq r0, r1, [pc], #-0x44
003cdf68: subeq r7, pc, r0, lsl r4
003cdf6c: subeq r7, pc, r4, lsl r4
003cdf70: subeq r0, pc, r0, lsr #9
003cdf74: subseq r3, r1, ip, lsr #3
003cdf78: subeq r7, pc, r0, asr #7

# 0x3d2d60 _ZNSt6vectorIP9CharacterSaIS1_EEC1ERKS3_
003d2d60: push {r4, r5, lr}
003d2d64: mov r5, r1
003d2d68: ldr r3, [r5]
003d2d6c: ldr r1, [r1, #4]
003d2d70: sub sp, sp, #0xc
003d2d74: mov r4, r0
003d2d78: rsb r1, r3, r1
003d2d7c: mov ip, #0
003d2d80: asr r1, r1, #2
003d2d84: add r2, sp, #8
003d2d88: str r1, [r2, #-4]!
003d2d8c: str ip, [r4]
003d2d90: str ip, [r4, #4]
003d2d94: str ip, [r0, #8]!
003d2d98: bl #0x3ccf2c
003d2d9c: ldr r2, [sp, #4]
003d2da0: str r0, [r4]
003d2da4: str r0, [r4, #4]
003d2da8: add r2, r0, r2, lsl #2
003d2dac: str r2, [r4, #8]
003d2db0: ldm r5, {r1, r2}
003d2db4: mov r3, r0
003d2db8: cmp r1, r2
003d2dbc: beq #0x3d2dd0
003d2dc0: rsb r5, r1, r2
003d2dc4: mov r2, r5
003d2dc8: bl #0x30e868
003d2dcc: add r3, r0, r5
003d2dd0: str r3, [r4, #4]
003d2dd4: mov r0, r4
003d2dd8: add sp, sp, #0xc
003d2ddc: pop {r4, r5, pc}

# 0x3e4b6c _ZN19LaserTypeProjectileC1EN10ObjectBase6GO_IDSE
003e4b6c: push {r4, r5, r6, lr}
003e4b70: ldr r5, [pc, #0x48]
003e4b74: mov r4, r0
003e4b78: bl #0x3e5edc
003e4b7c: ldr r3, [pc, #0x40]
003e4b80: add r5, pc, r5
003e4b84: mov r2, #0
003e4b88: ldr r3, [r5, r3]
003e4b8c: str r2, [r4, #0x3dc]
003e4b90: str r2, [r4, #0x3d4]
003e4b94: add r1, r3, #0xf0
003e4b98: add r0, r3, #8
003e4b9c: add r3, r3, #0xe4
003e4ba0: str r3, [r4, #4]
003e4ba4: mvn r3, #0
003e4ba8: str r0, [r4]
003e4bac: str r1, [r4, #0x24]
003e4bb0: str r3, [r4, #0x3e0]
003e4bb4: str r2, [r4, #0x3d8]
003e4bb8: mov r0, r4
003e4bbc: pop {r4, r5, r6, pc}
003e4bc0: subseq pc, sl, r0, lsl pc
003e4bc4: andeq r2, r0, r4, ror #21

# 0x3e4bc8 _ZN19LaserTypeProjectileC2EN10ObjectBase6GO_IDSE
003e4bc8: push {r4, r5, r6, lr}
003e4bcc: ldr r5, [pc, #0x48]
003e4bd0: mov r4, r0
003e4bd4: bl #0x3e5edc
003e4bd8: ldr r3, [pc, #0x40]
003e4bdc: add r5, pc, r5
003e4be0: mov r2, #0
003e4be4: ldr r3, [r5, r3]
003e4be8: str r2, [r4, #0x3dc]
003e4bec: str r2, [r4, #0x3d4]
003e4bf0: add r1, r3, #0xf0
003e4bf4: add r0, r3, #8
003e4bf8: add r3, r3, #0xe4
003e4bfc: str r3, [r4, #4]
003e4c00: mvn r3, #0
003e4c04: str r0, [r4]
003e4c08: str r1, [r4, #0x24]
003e4c0c: str r3, [r4, #0x3e0]
003e4c10: str r2, [r4, #0x3d8]
003e4c14: mov r0, r4
003e4c18: pop {r4, r5, r6, pc}
003e4c1c: ldrheq pc, [sl], #-0xe4
003e4c20: andeq r2, r0, r4, ror #21

# 0x3e5e40 _ZN10ProjectileC1EN10ObjectBase6GO_IDSE
003e5e40: push {r4, r5, r6, lr}
003e5e44: ldr r5, [pc, #0x88]
003e5e48: mov r4, r0
003e5e4c: bl #0x38c398
003e5e50: ldr r1, [pc, #0x80]
003e5e54: add r5, pc, r5
003e5e58: mov r2, #0
003e5e5c: ldr r1, [r5, r1]
003e5e60: mov r3, #0
003e5e64: str r2, [r4, #0x3c8]
003e5e68: add r0, r1, #0xf0
003e5e6c: add ip, r1, #8
003e5e70: add r1, r1, #0xe4
003e5e74: str r1, [r4, #4]
003e5e78: mvn r1, #0
003e5e7c: str r1, [r4, #0x374]
003e5e80: mov r1, #1
003e5e84: str r0, [r4, #0x24]
003e5e88: str ip, [r4]
003e5e8c: strb r3, [r4, #0x3d1]
003e5e90: strb r1, [r4, #0x85]
003e5e94: str r3, [r4, #0x378]
003e5e98: str r3, [r4, #0x380]
003e5e9c: str r3, [r4, #0x384]
003e5ea0: str r2, [r4, #0x388]
003e5ea4: str r2, [r4, #0x38c]
003e5ea8: str r2, [r4, #0x390]
003e5eac: str r2, [r4, #0x394]
003e5eb0: str r2, [r4, #0x398]
003e5eb4: str r2, [r4, #0x39c]
003e5eb8: str r3, [r4, #0x3a4]
003e5ebc: str r3, [r4, #0x3b8]
003e5ec0: str r2, [r4, #0x3c4]
003e5ec4: str r3, [r4, #0x3cc]
003e5ec8: strb r3, [r4, #0x3d0]
003e5ecc: mov r0, r4
003e5ed0: pop {r4, r5, r6, pc}
003e5ed4: subseq lr, sl, ip, lsr ip
003e5ed8: andeq r3, r0, r4, lsr #10

# 0x3e5edc _ZN10ProjectileC2EN10ObjectBase6GO_IDSE
003e5edc: push {r4, r5, r6, lr}
003e5ee0: ldr r5, [pc, #0x88]
003e5ee4: mov r4, r0
003e5ee8: bl #0x38c398
003e5eec: ldr r1, [pc, #0x80]
003e5ef0: add r5, pc, r5
003e5ef4: mov r2, #0
003e5ef8: ldr r1, [r5, r1]
003e5efc: mov r3, #0
003e5f00: str r2, [r4, #0x3c8]
003e5f04: add r0, r1, #0xf0
003e5f08: add ip, r1, #8
003e5f0c: add r1, r1, #0xe4
003e5f10: str r1, [r4, #4]
003e5f14: mvn r1, #0
003e5f18: str r1, [r4, #0x374]
003e5f1c: mov r1, #1
003e5f20: str r0, [r4, #0x24]
003e5f24: str ip, [r4]
003e5f28: strb r3, [r4, #0x3d1]
003e5f2c: strb r1, [r4, #0x85]
003e5f30: str r3, [r4, #0x378]
003e5f34: str r3, [r4, #0x380]
003e5f38: str r3, [r4, #0x384]
003e5f3c: str r2, [r4, #0x388]
003e5f40: str r2, [r4, #0x38c]
003e5f44: str r2, [r4, #0x390]
003e5f48: str r2, [r4, #0x394]
003e5f4c: str r2, [r4, #0x398]
003e5f50: str r2, [r4, #0x39c]
003e5f54: str r3, [r4, #0x3a4]
003e5f58: str r3, [r4, #0x3b8]
003e5f5c: str r2, [r4, #0x3c4]
003e5f60: str r3, [r4, #0x3cc]
003e5f64: strb r3, [r4, #0x3d0]
003e5f68: mov r0, r4
003e5f6c: pop {r4, r5, r6, pc}
003e5f70: subseq lr, sl, r0, lsr #23
003e5f74: andeq r3, r0, r4, lsr #10

# 0x3e7ffc _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_.clone.5
003e7ffc: push {r4, r5, r6, r7, r8, lr}
003e8000: mov r7, r0
003e8004: sub sp, sp, #8
003e8008: mov r6, r1
003e800c: mov r0, #0x24
003e8010: mov r1, #0
003e8014: mov r8, r2
003e8018: bl #0x310570
003e801c: ldr r5, [pc, #0x58]
003e8020: ldr r3, [pc, #0x58]
003e8024: mov r4, r0
003e8028: add r5, pc, r5
003e802c: ldr r3, [r5, r3]
003e8030: mov r1, r6
003e8034: add r2, sp, #4
003e8038: add r3, r3, #8
003e803c: str r3, [r0], #8
003e8040: bl #0x3140ec
003e8044: ldr r3, [pc, #0x38]
003e8048: rsb r8, r7, r8
003e804c: mov r2, #1
003e8050: ldr r3, [r5, r3]
003e8054: strb r2, [r4, #0x20]
003e8058: str r8, [r4, #4]
003e805c: add r3, r3, #8
003e8060: str r3, [r4]
003e8064: mov r0, r7
003e8068: mov r1, r6
003e806c: mov r2, r4
003e8070: bl #0x513ce4
003e8074: add sp, sp, #8
003e8078: pop {r4, r5, r6, r7, r8, pc}
003e807c: subseq ip, sl, r8, ror #20
003e8080: andeq r2, r0, r0, lsr r3
003e8084: andeq r3, r0, ip, asr #28

# 0x3e8274 _ZN4DoorC1EN10ObjectBase6GO_IDSE
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

# 0x3e8324 _ZN4DoorC2EN10ObjectBase6GO_IDSE
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

# 0x3ea388 _ZN10SpawnPointC1EN10ObjectBase6GO_IDSE
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

# 0x3ea3fc _ZN10SpawnPointC2EN10ObjectBase6GO_IDSE
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

# 0x3ea8e4 _ZN9SpawnSpotC1EN10ObjectBase6GO_IDSE
003ea8e4: push {r4, r5, r6, lr}
003ea8e8: ldr r5, [pc, #0x54]
003ea8ec: mov r4, r0
003ea8f0: bl #0x38c398
003ea8f4: ldr r3, [pc, #0x4c]
003ea8f8: add r5, pc, r5
003ea8fc: add r2, r4, #0x374
003ea900: ldr r3, [r5, r3]
003ea904: mov r0, r2
003ea908: str r2, [r4, #0x384]
003ea90c: add ip, r3, #8
003ea910: add r1, r3, #0xe4
003ea914: add r3, r3, #0xd8
003ea918: str r3, [r4, #4]
003ea91c: str r1, [r4, #0x24]
003ea920: str r2, [r4, #0x388]
003ea924: str ip, [r4]
003ea928: mov r1, #0x10
003ea92c: bl #0x31167c
003ea930: ldr r3, [r4, #0x384]
003ea934: mov r2, #0
003ea938: mov r0, r4
003ea93c: strb r2, [r3]
003ea940: pop {r4, r5, r6, pc}

# 0x3ea94c _ZN9SpawnSpotC2EN10ObjectBase6GO_IDSE
003ea94c: push {r4, r5, r6, lr}
003ea950: ldr r5, [pc, #0x54]
003ea954: mov r4, r0
003ea958: bl #0x38c398
003ea95c: ldr r3, [pc, #0x4c]
003ea960: add r5, pc, r5
003ea964: add r2, r4, #0x374
003ea968: ldr r3, [r5, r3]
003ea96c: mov r0, r2
003ea970: str r2, [r4, #0x384]
003ea974: add ip, r3, #8
003ea978: add r1, r3, #0xe4
003ea97c: add r3, r3, #0xd8
003ea980: str r3, [r4, #4]
003ea984: str r1, [r4, #0x24]
003ea988: str r2, [r4, #0x388]
003ea98c: str ip, [r4]
003ea990: mov r1, #0x10
003ea994: bl #0x31167c
003ea998: ldr r3, [r4, #0x384]
003ea99c: mov r2, #0
003ea9a0: mov r0, r4
003ea9a4: strb r2, [r3]
003ea9a8: pop {r4, r5, r6, pc}
003ea9ac: subseq sl, sl, r0, lsr r1
003ea9b0: andeq r4, r0, r4, lsl #17

# 0x3ec324 _ZN10ItemObjectC1EN10ObjectBase6GO_IDSE
003ec324: push {r4, r5, r6, r7, r8, lr}
003ec328: mov r4, r0
003ec32c: ldr r5, [pc, #0x90]
003ec330: bl #0x38c398
003ec334: add r0, r4, #0x374
003ec338: bl #0x3ff330
003ec33c: ldr r3, [pc, #0x84]
003ec340: add r5, pc, r5
003ec344: mvn r1, #0
003ec348: ldr r3, [r5, r3]
003ec34c: mov r7, #0x3c0
003ec350: strh r1, [r4, r7]
003ec354: add r0, r3, #0xfc
003ec358: add r6, r3, #8
003ec35c: add ip, r3, #0xd8
003ec360: add r3, r3, #0xe4
003ec364: str r3, [r4, #0x24]
003ec368: mov r3, #0x3ac
003ec36c: str r0, [r4, #0x374]
003ec370: stm r4, {r6, ip}
003ec374: strh r1, [r4, r3]
003ec378: mov r3, #0x40000000
003ec37c: add r3, r3, #0x200000
003ec380: str r3, [r4, #0x3b0]
003ec384: mov r3, #0x3b4
003ec388: strh r1, [r4, r3]
003ec38c: movw r3, #0x3b6
003ec390: strh r1, [r4, r3]
003ec394: mov r3, #0x3b8
003ec398: strh r1, [r4, r3]
003ec39c: mov r2, #0
003ec3a0: mov r3, #1
003ec3a4: strb r3, [r4, #0x85]
003ec3a8: strb r2, [r4, #0x2ee]
003ec3ac: str r2, [r4, #0x3bc]
003ec3b0: str r2, [r4, #0x3c4]
003ec3b4: str r2, [r4, #0x3c8]
003ec3b8: str r2, [r4, #0x3cc]
003ec3bc: mov r0, r4
003ec3c0: pop {r4, r5, r6, r7, r8, pc}
003ec3c4: subseq r8, sl, r0, asr r7
003ec3c8: andeq r2, r0, r8, lsr #12

# 0x3ec3cc _ZN10ItemObjectC2EN10ObjectBase6GO_IDSE
003ec3cc: push {r4, r5, r6, r7, r8, lr}
003ec3d0: mov r4, r0
003ec3d4: ldr r5, [pc, #0x90]
003ec3d8: bl #0x38c398
003ec3dc: add r0, r4, #0x374
003ec3e0: bl #0x3ff330
003ec3e4: ldr r3, [pc, #0x84]
003ec3e8: add r5, pc, r5
003ec3ec: mvn r1, #0
003ec3f0: ldr r3, [r5, r3]
003ec3f4: mov r7, #0x3c0
003ec3f8: strh r1, [r4, r7]
003ec3fc: add r0, r3, #0xfc
003ec400: add r6, r3, #8
003ec404: add ip, r3, #0xd8
003ec408: add r3, r3, #0xe4
003ec40c: str r3, [r4, #0x24]
003ec410: mov r3, #0x3ac
003ec414: str r0, [r4, #0x374]
003ec418: stm r4, {r6, ip}
003ec41c: strh r1, [r4, r3]
003ec420: mov r3, #0x40000000
003ec424: add r3, r3, #0x200000
003ec428: str r3, [r4, #0x3b0]
003ec42c: mov r3, #0x3b4
003ec430: strh r1, [r4, r3]
003ec434: movw r3, #0x3b6
003ec438: strh r1, [r4, r3]
003ec43c: mov r3, #0x3b8
003ec440: strh r1, [r4, r3]
003ec444: mov r2, #0
003ec448: mov r3, #1
003ec44c: strb r3, [r4, #0x85]
003ec450: strb r2, [r4, #0x2ee]
003ec454: str r2, [r4, #0x3bc]
003ec458: str r2, [r4, #0x3c4]
003ec45c: str r2, [r4, #0x3c8]
003ec460: str r2, [r4, #0x3cc]
003ec464: mov r0, r4
003ec468: pop {r4, r5, r6, r7, r8, pc}
003ec46c: subseq r8, sl, r8, lsr #13
003ec470: andeq r2, r0, r8, lsr #12

# 0x3eec88 _ZN14LiftableObjectC1EN10ObjectBase6GO_IDSE
003eec88: push {r4, r5, r6, lr}
003eec8c: ldr r5, [pc, #0x74]
003eec90: mov r4, r0
003eec94: bl #0x38c398
003eec98: ldr r3, [pc, #0x6c]
003eec9c: add r5, pc, r5
003eeca0: add r2, r4, #0x378
003eeca4: ldr r3, [r5, r3]
003eeca8: mov r0, r2
003eecac: str r2, [r4, #0x388]
003eecb0: add ip, r3, #8
003eecb4: add r1, r3, #0xe8
003eecb8: add r3, r3, #0xdc
003eecbc: str r3, [r4, #4]
003eecc0: str r1, [r4, #0x24]
003eecc4: str r2, [r4, #0x38c]
003eecc8: str ip, [r4]
003eeccc: mov r1, #0x10
003eecd0: bl #0x31167c
003eecd4: ldr r1, [r4, #0x388]
003eecd8: mov r3, #0
003eecdc: mov r2, #0
003eece0: strb r3, [r1]
003eece4: mov r0, r4
003eece8: str r2, [r4, #0x3a0]
003eecec: strb r3, [r4, #0x84]
003eecf0: str r3, [r4, #0x390]
003eecf4: strb r3, [r4, #0x394]
003eecf8: str r2, [r4, #0x398]
003eecfc: str r2, [r4, #0x39c]
003eed00: strb r3, [r4, #0x3a4]
003eed04: pop {r4, r5, r6, pc}
003eed08: ldrsheq r5, [sl], #-0xd4
003eed0c: andeq r2, r0, r4, lsr #7

# 0x3eed10 _ZN14LiftableObjectC2EN10ObjectBase6GO_IDSE
003eed10: push {r4, r5, r6, lr}
003eed14: ldr r5, [pc, #0x74]
003eed18: mov r4, r0
003eed1c: bl #0x38c398
003eed20: ldr r3, [pc, #0x6c]
003eed24: add r5, pc, r5
003eed28: add r2, r4, #0x378
003eed2c: ldr r3, [r5, r3]
003eed30: mov r0, r2
003eed34: str r2, [r4, #0x388]
003eed38: add ip, r3, #8
003eed3c: add r1, r3, #0xe8
003eed40: add r3, r3, #0xdc
003eed44: str r3, [r4, #4]
003eed48: str r1, [r4, #0x24]
003eed4c: str r2, [r4, #0x38c]
003eed50: str ip, [r4]
003eed54: mov r1, #0x10
003eed58: bl #0x31167c
003eed5c: ldr r1, [r4, #0x388]
003eed60: mov r3, #0
003eed64: mov r2, #0
003eed68: strb r3, [r1]
003eed6c: mov r0, r4
003eed70: str r2, [r4, #0x3a0]
003eed74: strb r3, [r4, #0x84]
003eed78: str r3, [r4, #0x390]
003eed7c: strb r3, [r4, #0x394]
003eed80: str r2, [r4, #0x398]
003eed84: str r2, [r4, #0x39c]
003eed88: strb r3, [r4, #0x3a4]
003eed8c: pop {r4, r5, r6, pc}
003eed90: subseq r5, sl, ip, ror #26
003eed94: andeq r2, r0, r4, lsr #7

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

# 0x40aba4 _ZN9LightBaseC1EN10ObjectBase6GO_IDSE
0040aba4: push {r4, r5, r6, lr}
0040aba8: ldr r5, [pc, #0xa0]
0040abac: mov r4, r0
0040abb0: bl #0x33f310
0040abb4: ldr r1, [pc, #0x98]
0040abb8: add r5, pc, r5
0040abbc: mov r3, #0
0040abc0: ldr r1, [r5, r1]
0040abc4: add r2, r4, #0x168
0040abc8: mov r6, #0
0040abcc: add ip, r1, #8
0040abd0: add r0, r1, #0x84
0040abd4: add r1, r1, #0x78
0040abd8: str r1, [r4, #4]
0040abdc: str r0, [r4, #0x24]
0040abe0: str r3, [r4, #0x160]
0040abe4: mov r0, r2
0040abe8: str r3, [r4, #0x124]
0040abec: str r3, [r4, #0x128]
0040abf0: str r3, [r4, #0x12c]
0040abf4: str r3, [r4, #0x134]
0040abf8: str r3, [r4, #0x138]
0040abfc: str r3, [r4, #0x13c]
0040ac00: str r3, [r4, #0x140]
0040ac04: str r3, [r4, #0x144]
0040ac08: str r3, [r4, #0x148]
0040ac0c: str r3, [r4, #0x14c]
0040ac10: str r3, [r4, #0x150]
0040ac14: str r3, [r4, #0x154]
0040ac18: str r3, [r4, #0x158]
0040ac1c: str r3, [r4, #0x15c]
0040ac20: str ip, [r4]
0040ac24: str r6, [r4, #0x120]
0040ac28: str r2, [r4, #0x178]
0040ac2c: str r2, [r4, #0x17c]
0040ac30: mov r1, #0x10
0040ac34: bl #0x31167c
0040ac38: ldr r3, [r4, #0x178]
0040ac3c: mov r0, r4
0040ac40: strb r6, [r3]
0040ac44: mov r3, #1
0040ac48: strb r3, [r4, #0x85]
0040ac4c: pop {r4, r5, r6, pc}
0040ac50: ldrsbeq sb, [r8], #-0xe8
0040ac54: andeq r0, r0, r0, ror #15

# 0x40ac58 _ZN9LightBaseC2EN10ObjectBase6GO_IDSE
0040ac58: push {r4, r5, r6, lr}
0040ac5c: ldr r5, [pc, #0xa0]
0040ac60: mov r4, r0
0040ac64: bl #0x33f310
0040ac68: ldr r1, [pc, #0x98]
0040ac6c: add r5, pc, r5
0040ac70: mov r3, #0
0040ac74: ldr r1, [r5, r1]
0040ac78: add r2, r4, #0x168
0040ac7c: mov r6, #0
0040ac80: add ip, r1, #8
0040ac84: add r0, r1, #0x84
0040ac88: add r1, r1, #0x78
0040ac8c: str r1, [r4, #4]
0040ac90: str r0, [r4, #0x24]
0040ac94: str r3, [r4, #0x160]
0040ac98: mov r0, r2
0040ac9c: str r3, [r4, #0x124]
0040aca0: str r3, [r4, #0x128]
0040aca4: str r3, [r4, #0x12c]
0040aca8: str r3, [r4, #0x134]
0040acac: str r3, [r4, #0x138]
0040acb0: str r3, [r4, #0x13c]
0040acb4: str r3, [r4, #0x140]
0040acb8: str r3, [r4, #0x144]
0040acbc: str r3, [r4, #0x148]
0040acc0: str r3, [r4, #0x14c]
0040acc4: str r3, [r4, #0x150]
0040acc8: str r3, [r4, #0x154]
0040accc: str r3, [r4, #0x158]
0040acd0: str r3, [r4, #0x15c]
0040acd4: str ip, [r4]
0040acd8: str r6, [r4, #0x120]
0040acdc: str r2, [r4, #0x178]
0040ace0: str r2, [r4, #0x17c]
0040ace4: mov r1, #0x10
0040ace8: bl #0x31167c
0040acec: ldr r3, [r4, #0x178]
0040acf0: mov r0, r4
0040acf4: strb r6, [r3]
0040acf8: mov r3, #1
0040acfc: strb r3, [r4, #0x85]
0040ad00: pop {r4, r5, r6, pc}
0040ad04: subseq sb, r8, r4, lsr #28
0040ad08: andeq r0, r0, r0, ror #15

# 0x46ef68 _ZN14PhysicalObjectC1EP13PhysicalWorldP10GameObjectbbbbstti
0046ef68: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046ef6c: ldr r5, [pc, #0x360]
0046ef70: ldr r4, [pc, #0x360]
0046ef74: sub sp, sp, #0xb4
0046ef78: add r5, pc, r5
0046ef7c: str r4, [sp, #0x10]
0046ef80: ldr lr, [r5, r4]
0046ef84: ldr ip, [pc, #0x350]
0046ef88: ldr r4, [pc, #0x350]
0046ef8c: ldrb sb, [sp, #0xd8]
0046ef90: ldr ip, [r5, ip]
0046ef94: ldr r4, [r5, r4]
0046ef98: ldr lr, [lr]
0046ef9c: mov r7, #0
0046efa0: str r4, [sp, #0xc]
0046efa4: add ip, ip, #8
0046efa8: mov r4, r0
0046efac: mov sl, #0
0046efb0: str r1, [r0, #4]
0046efb4: str ip, [r0]
0046efb8: str r2, [r4, #8]
0046efbc: str sl, [r0, #0xc]
0046efc0: strb sb, [r0, #0x10]
0046efc4: str r7, [r0, #0x14]
0046efc8: str r7, [r0, #0x18]
0046efcc: str r7, [r0, #0x1c]
0046efd0: strb r7, [r0, #0x26]
0046efd4: strb r7, [r0, #0x27]
0046efd8: ldrb ip, [sp, #0xdc]
0046efdc: mov r6, r2
0046efe0: str lr, [sp, #0xac]
0046efe4: ldrh r2, [sp, #0xe8]
0046efe8: ldrb lr, [sp, #0xe0]
0046efec: str r3, [sp, #0x1c]
0046eff0: ldrh r3, [sp, #0xec]
0046eff4: ldr r0, [sp, #0xc]
0046eff8: str ip, [sp, #0x20]
0046effc: str lr, [sp, #0x24]
0046f000: str r3, [sp, #0x18]
0046f004: ldrsh r8, [sp, #0xe4]
0046f008: str r2, [sp, #0x14]
0046f00c: bl #0x337888
0046f010: ldr r1, [pc, #0x2cc]
0046f014: add fp, sp, #0x94
0046f018: add r2, sp, #0x90
0046f01c: add r1, pc, r1
0046f020: mov r0, fp
0046f024: bl #0x3140ec
0046f028: mov r1, fp
0046f02c: ldr r0, [sp, #0xc]
0046f030: bl #0x337a88
0046f034: mov r3, r0
0046f038: mov r0, fp
0046f03c: str r3, [sp, #8]
0046f040: bl #0x3139ac
0046f044: ldr r3, [sp, #8]
0046f048: mvn r2, #0x298
0046f04c: sub r2, r2, #1
0046f050: cmp r3, r7
0046f054: movne r8, r2
0046f058: cmp r6, r7
0046f05c: beq #0x46f1d0
0046f060: cmp sb, r7
0046f064: bne #0x46f1f4
0046f068: ldr r3, [pc, #0x278]
0046f06c: mov r2, #1
0046f070: ldr r1, [r6, #0x12c]
0046f074: ldr r3, [r5, r3]
0046f078: ldr r0, [r6, #0x138]
0046f07c: str r2, [sp, #0x30]
0046f080: add ip, r3, #8
0046f084: movw r3, #0xcccd
0046f088: movt r3, #0x3e4c
0046f08c: strh r2, [sp, #0x46]
0046f090: mvn r2, #0
0046f094: str ip, [sp, #0x2c]
0046f098: strh r2, [sp, #0x48]
0046f09c: str r3, [sp, #0x38]
0046f0a0: str sl, [sp, #0x40]
0046f0a4: str sb, [sp, #0x8c]
0046f0a8: str sb, [sp, #0x34]
0046f0ac: str sl, [sp, #0x3c]
0046f0b0: strh sb, [sp, #0x4a]
0046f0b4: strb sb, [sp, #0x44]
0046f0b8: bl #0x30e3ac
0046f0bc: movw r1, #0xd70a
0046f0c0: movt r1, #0x3c23
0046f0c4: bl #0x30ed6c
0046f0c8: ldr r1, [r6, #0x130]
0046f0cc: mov sb, r0
0046f0d0: ldr r0, [r6, #0x13c]
0046f0d4: bl #0x30e3ac
0046f0d8: movw r1, #0xd70a
0046f0dc: movt r1, #0x3c23
0046f0e0: bl #0x30ed6c
0046f0e4: movw r1, #0xd70a
0046f0e8: mov r7, r0
0046f0ec: movt r1, #0x3c23
0046f0f0: ldr r0, [r6, #0x160]
0046f0f4: bl #0x30ed6c
0046f0f8: movw r1, #0xd70a
0046f0fc: movt r1, #0x3c23
0046f100: mov sl, r0
0046f104: ldr r0, [r6, #0x164]
0046f108: bl #0x30ed6c
0046f10c: mov r1, #0x3f000000
0046f110: mov r6, r0
0046f114: mov r0, sb
0046f118: bl #0x30ed6c
0046f11c: mov r1, #0x3f000000
0046f120: mov r3, r0
0046f124: mov r0, r7
0046f128: str r3, [sp, #8]
0046f12c: bl #0x30ed6c
0046f130: ldr r3, [sp, #8]
0046f134: add fp, sp, #0x2c
0046f138: mov r2, r0
0046f13c: mov r1, r3
0046f140: mov r0, fp
0046f144: bl #0x7e44b8
0046f148: mov r1, r7
0046f14c: mov r0, sb
0046f150: bl #0x30e70c
0046f154: cmp r0, #0
0046f158: moveq r7, sb
0046f15c: mov r0, r7
0046f160: mov r1, #0x3f000000
0046f164: bl #0x30ed6c
0046f168: ldr r3, [pc, #0x17c]
0046f16c: str r0, [r4, #0xc]
0046f170: mov ip, fp
0046f174: ldr r3, [r5, r3]
0046f178: add r3, r3, #8
0046f17c: str r3, [sp, #0x2c]
0046f180: strh r8, [r4, #0x24]
0046f184: ldr lr, [sp, #0x14]
0046f188: mov r1, ip
0046f18c: mov r3, r6
0046f190: strh lr, [r4, #0x20]
0046f194: ldr r2, [sp, #0x18]
0046f198: mov r0, r4
0046f19c: strh r2, [r4, #0x22]
0046f1a0: ldr lr, [sp, #0x20]
0046f1a4: strh r8, [ip, #0x1e]
0046f1a8: mov r2, sl
0046f1ac: strb lr, [ip, #0x18]
0046f1b0: ldr lr, [sp, #0x14]
0046f1b4: strh lr, [ip, #0x1a]
0046f1b8: ldr lr, [sp, #0x18]
0046f1bc: strh lr, [ip, #0x1c]
0046f1c0: ldr ip, [sp, #0x1c]
0046f1c4: ldr lr, [sp, #0x24]
0046f1c8: stm sp, {ip, lr}
0046f1cc: bl #0x46edf0
0046f1d0: ldr ip, [sp, #0x10]
0046f1d4: ldr r2, [sp, #0xac]
0046f1d8: mov r0, r4
0046f1dc: ldr r3, [r5, ip]
0046f1e0: ldr r3, [r3]
0046f1e4: cmp r2, r3
0046f1e8: bne #0x46f2d0
0046f1ec: add sp, sp, #0xb4
0046f1f0: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f1f4: movw r3, #0xcccd
0046f1f8: ldr r1, [r6, #0x12c]
0046f1fc: ldr r0, [r6, #0x138]
0046f200: movt r3, #0x3e4c
0046f204: mov ip, #1
0046f208: mvn lr, #0
0046f20c: str r3, [sp, #0x38]
0046f210: strh ip, [sp, #0x46]
0046f214: strh lr, [sp, #0x48]
0046f218: strb r7, [sp, #0x44]
0046f21c: str r7, [sp, #0x30]
0046f220: str sl, [sp, #0x50]
0046f224: str r7, [sp, #0x34]
0046f228: str sl, [sp, #0x3c]
0046f22c: str sl, [sp, #0x40]
0046f230: strh r7, [sp, #0x4a]
0046f234: str sl, [sp, #0x4c]
0046f238: bl #0x30e3ac
0046f23c: movw r1, #0xd70a
0046f240: movt r1, #0x3c23
0046f244: bl #0x30ed6c
0046f248: ldr r1, [r6, #0x130]
0046f24c: mov sb, r0
0046f250: ldr r0, [r6, #0x13c]
0046f254: bl #0x30e3ac
0046f258: movw r1, #0xd70a
0046f25c: movt r1, #0x3c23
0046f260: bl #0x30ed6c
0046f264: movw r1, #0xd70a
0046f268: mov r7, r0
0046f26c: movt r1, #0x3c23
0046f270: ldr r0, [r6, #0x160]
0046f274: bl #0x30ed6c
0046f278: movw r1, #0xd70a
0046f27c: movt r1, #0x3c23
0046f280: mov sl, r0
0046f284: ldr r0, [r6, #0x164]
0046f288: bl #0x30ed6c
0046f28c: mov r1, r7
0046f290: mov r6, r0
0046f294: mov r0, sb
0046f298: bl #0x30e70c
0046f29c: cmp r0, #0
0046f2a0: moveq r7, sb
0046f2a4: mov r0, r7
0046f2a8: mov r1, #0x3f000000
0046f2ac: bl #0x30ed6c
0046f2b0: ldr r3, [pc, #0x34]
0046f2b4: add ip, sp, #0xb0
0046f2b8: str r0, [r4, #0xc]
0046f2bc: ldr r3, [r5, r3]
0046f2c0: str r0, [sp, #0x54]
0046f2c4: add r3, r3, #8
0046f2c8: str r3, [ip, #-0x84]!
0046f2cc: b #0x46f180
0046f2d0: bl #0x30e310
0046f2d4: subseq r5, r2, r8, lsl fp
0046f2d8: andeq r4, r0, ip, lsr #1
0046f2dc: andeq r2, r0, r4, lsl #9
0046f2e0: andeq r0, r0, r4, lsl #17
0046f2e4: subeq lr, r5, ip, asr #11
0046f2e8: ldrdeq r4, r5, [r0], -ip
0046f2ec: andeq r3, r0, r8, lsl r8

# 0x46f2f0 _ZN14PhysicalObjectC2EP13PhysicalWorldP10GameObjectbbbbstti
0046f2f0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046f2f4: ldr r5, [pc, #0x360]
0046f2f8: ldr r4, [pc, #0x360]
0046f2fc: sub sp, sp, #0xb4
0046f300: add r5, pc, r5
0046f304: str r4, [sp, #0x10]
0046f308: ldr lr, [r5, r4]
0046f30c: ldr ip, [pc, #0x350]
0046f310: ldr r4, [pc, #0x350]
0046f314: ldrb sb, [sp, #0xd8]
0046f318: ldr ip, [r5, ip]
0046f31c: ldr r4, [r5, r4]
0046f320: ldr lr, [lr]
0046f324: mov r7, #0
0046f328: str r4, [sp, #0xc]
0046f32c: add ip, ip, #8
0046f330: mov r4, r0
0046f334: mov sl, #0
0046f338: str r1, [r0, #4]
0046f33c: str ip, [r0]
0046f340: str r2, [r4, #8]
0046f344: str sl, [r0, #0xc]
0046f348: strb sb, [r0, #0x10]
0046f34c: str r7, [r0, #0x14]
0046f350: str r7, [r0, #0x18]
0046f354: str r7, [r0, #0x1c]
0046f358: strb r7, [r0, #0x26]
0046f35c: strb r7, [r0, #0x27]
0046f360: ldrb ip, [sp, #0xdc]
0046f364: mov r6, r2
0046f368: str lr, [sp, #0xac]
0046f36c: ldrh r2, [sp, #0xe8]
0046f370: ldrb lr, [sp, #0xe0]
0046f374: str r3, [sp, #0x1c]
0046f378: ldrh r3, [sp, #0xec]
0046f37c: ldr r0, [sp, #0xc]
0046f380: str ip, [sp, #0x20]
0046f384: str lr, [sp, #0x24]
0046f388: str r3, [sp, #0x18]
0046f38c: ldrsh r8, [sp, #0xe4]
0046f390: str r2, [sp, #0x14]
0046f394: bl #0x337888
0046f398: ldr r1, [pc, #0x2cc]
0046f39c: add fp, sp, #0x94
0046f3a0: add r2, sp, #0x90
0046f3a4: add r1, pc, r1
0046f3a8: mov r0, fp
0046f3ac: bl #0x3140ec
0046f3b0: mov r1, fp
0046f3b4: ldr r0, [sp, #0xc]
0046f3b8: bl #0x337a88
0046f3bc: mov r3, r0
0046f3c0: mov r0, fp
0046f3c4: str r3, [sp, #8]
0046f3c8: bl #0x3139ac
0046f3cc: ldr r3, [sp, #8]
0046f3d0: mvn r2, #0x298
0046f3d4: sub r2, r2, #1
0046f3d8: cmp r3, r7
0046f3dc: movne r8, r2
0046f3e0: cmp r6, r7
0046f3e4: beq #0x46f558
0046f3e8: cmp sb, r7
0046f3ec: bne #0x46f57c
0046f3f0: ldr r3, [pc, #0x278]
0046f3f4: mov r2, #1
0046f3f8: ldr r1, [r6, #0x12c]
0046f3fc: ldr r3, [r5, r3]
0046f400: ldr r0, [r6, #0x138]
0046f404: str r2, [sp, #0x30]
0046f408: add ip, r3, #8
0046f40c: movw r3, #0xcccd
0046f410: movt r3, #0x3e4c
0046f414: strh r2, [sp, #0x46]
0046f418: mvn r2, #0
0046f41c: str ip, [sp, #0x2c]
0046f420: strh r2, [sp, #0x48]
0046f424: str r3, [sp, #0x38]
0046f428: str sl, [sp, #0x40]
0046f42c: str sb, [sp, #0x8c]
0046f430: str sb, [sp, #0x34]
0046f434: str sl, [sp, #0x3c]
0046f438: strh sb, [sp, #0x4a]
0046f43c: strb sb, [sp, #0x44]
0046f440: bl #0x30e3ac
0046f444: movw r1, #0xd70a
0046f448: movt r1, #0x3c23
0046f44c: bl #0x30ed6c
0046f450: ldr r1, [r6, #0x130]
0046f454: mov sb, r0
0046f458: ldr r0, [r6, #0x13c]
0046f45c: bl #0x30e3ac
0046f460: movw r1, #0xd70a
0046f464: movt r1, #0x3c23
0046f468: bl #0x30ed6c
0046f46c: movw r1, #0xd70a
0046f470: mov r7, r0
0046f474: movt r1, #0x3c23
0046f478: ldr r0, [r6, #0x160]
0046f47c: bl #0x30ed6c
0046f480: movw r1, #0xd70a
0046f484: movt r1, #0x3c23
0046f488: mov sl, r0
0046f48c: ldr r0, [r6, #0x164]
0046f490: bl #0x30ed6c
0046f494: mov r1, #0x3f000000
0046f498: mov r6, r0
0046f49c: mov r0, sb
0046f4a0: bl #0x30ed6c
0046f4a4: mov r1, #0x3f000000
0046f4a8: mov r3, r0
0046f4ac: mov r0, r7
0046f4b0: str r3, [sp, #8]
0046f4b4: bl #0x30ed6c
0046f4b8: ldr r3, [sp, #8]
0046f4bc: add fp, sp, #0x2c
0046f4c0: mov r2, r0
0046f4c4: mov r1, r3
0046f4c8: mov r0, fp
0046f4cc: bl #0x7e44b8
0046f4d0: mov r1, r7
0046f4d4: mov r0, sb
0046f4d8: bl #0x30e70c
0046f4dc: cmp r0, #0
0046f4e0: moveq r7, sb
0046f4e4: mov r0, r7
0046f4e8: mov r1, #0x3f000000
0046f4ec: bl #0x30ed6c
0046f4f0: ldr r3, [pc, #0x17c]
0046f4f4: str r0, [r4, #0xc]
0046f4f8: mov ip, fp
0046f4fc: ldr r3, [r5, r3]
0046f500: add r3, r3, #8
0046f504: str r3, [sp, #0x2c]
0046f508: strh r8, [r4, #0x24]
0046f50c: ldr lr, [sp, #0x14]
0046f510: mov r1, ip
0046f514: mov r3, r6
0046f518: strh lr, [r4, #0x20]
0046f51c: ldr r2, [sp, #0x18]
0046f520: mov r0, r4
0046f524: strh r2, [r4, #0x22]
0046f528: ldr lr, [sp, #0x20]
0046f52c: strh r8, [ip, #0x1e]
0046f530: mov r2, sl
0046f534: strb lr, [ip, #0x18]
0046f538: ldr lr, [sp, #0x14]
0046f53c: strh lr, [ip, #0x1a]
0046f540: ldr lr, [sp, #0x18]
0046f544: strh lr, [ip, #0x1c]
0046f548: ldr ip, [sp, #0x1c]
0046f54c: ldr lr, [sp, #0x24]
0046f550: stm sp, {ip, lr}
0046f554: bl #0x46edf0
0046f558: ldr ip, [sp, #0x10]
0046f55c: ldr r2, [sp, #0xac]
0046f560: mov r0, r4
0046f564: ldr r3, [r5, ip]
0046f568: ldr r3, [r3]
0046f56c: cmp r2, r3
0046f570: bne #0x46f658
0046f574: add sp, sp, #0xb4
0046f578: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046f57c: movw r3, #0xcccd
0046f580: ldr r1, [r6, #0x12c]
0046f584: ldr r0, [r6, #0x138]
0046f588: movt r3, #0x3e4c
0046f58c: mov ip, #1
0046f590: mvn lr, #0
0046f594: str r3, [sp, #0x38]
0046f598: strh ip, [sp, #0x46]
0046f59c: strh lr, [sp, #0x48]
0046f5a0: strb r7, [sp, #0x44]
0046f5a4: str r7, [sp, #0x30]
0046f5a8: str sl, [sp, #0x50]
0046f5ac: str r7, [sp, #0x34]
0046f5b0: str sl, [sp, #0x3c]
0046f5b4: str sl, [sp, #0x40]
0046f5b8: strh r7, [sp, #0x4a]
0046f5bc: str sl, [sp, #0x4c]
0046f5c0: bl #0x30e3ac
0046f5c4: movw r1, #0xd70a
0046f5c8: movt r1, #0x3c23
0046f5cc: bl #0x30ed6c
0046f5d0: ldr r1, [r6, #0x130]
0046f5d4: mov sb, r0
0046f5d8: ldr r0, [r6, #0x13c]
0046f5dc: bl #0x30e3ac
0046f5e0: movw r1, #0xd70a
0046f5e4: movt r1, #0x3c23
0046f5e8: bl #0x30ed6c
0046f5ec: movw r1, #0xd70a
0046f5f0: mov r7, r0
0046f5f4: movt r1, #0x3c23
0046f5f8: ldr r0, [r6, #0x160]
0046f5fc: bl #0x30ed6c
0046f600: movw r1, #0xd70a
0046f604: movt r1, #0x3c23
0046f608: mov sl, r0
0046f60c: ldr r0, [r6, #0x164]
0046f610: bl #0x30ed6c
0046f614: mov r1, r7
0046f618: mov r6, r0
0046f61c: mov r0, sb
0046f620: bl #0x30e70c
0046f624: cmp r0, #0
0046f628: moveq r7, sb
0046f62c: mov r0, r7
0046f630: mov r1, #0x3f000000
0046f634: bl #0x30ed6c
0046f638: ldr r3, [pc, #0x34]
0046f63c: add ip, sp, #0xb0
0046f640: str r0, [r4, #0xc]
0046f644: ldr r3, [r5, r3]
0046f648: str r0, [sp, #0x54]
0046f64c: add r3, r3, #8
0046f650: str r3, [ip, #-0x84]!
0046f654: b #0x46f508
0046f658: bl #0x30e310

# 0x472a0c _ZN12VisualObjectC1EP10GameObjectRKSsS3_
00472a0c: push {r4, r5, r6, r7, r8, sl, lr}
00472a10: ldr r6, [pc, #0x234]
00472a14: ldr r5, [pc, #0x234]
00472a18: mov ip, #0xbf000000
00472a1c: add r6, pc, r6
00472a20: ldr r5, [r6, r5]
00472a24: mov lr, #0
00472a28: add ip, ip, #0x800000
00472a2c: mov r7, r1
00472a30: add r1, r5, #8
00472a34: mov r5, #0
00472a38: mov r8, r3
00472a3c: sub sp, sp, #0xc
00472a40: str r1, [r0]
00472a44: str lr, [r0, #0x24]
00472a48: str ip, [r0, #0x74]
00472a4c: str lr, [r0, #0x10]
00472a50: str lr, [r0, #0x14]
00472a54: str lr, [r0, #0x18]
00472a58: str lr, [r0, #0x1c]
00472a5c: str lr, [r0, #0x20]
00472a60: str ip, [r0, #0x58]
00472a64: str ip, [r0, #0x5c]
00472a68: str ip, [r0, #0x60]
00472a6c: str ip, [r0, #0x64]
00472a70: str ip, [r0, #0x68]
00472a74: str r7, [r0, #4]
00472a78: str r5, [r0, #8]
00472a7c: str r5, [r0, #0xc]
00472a80: strb r5, [r0, #0x28]
00472a84: str r5, [r0, #0x2c]
00472a88: str r5, [r0, #0x30]
00472a8c: str r5, [r0, #0x34]
00472a90: str r5, [r0, #0x38]
00472a94: strb r5, [r0, #0x3c]
00472a98: str r5, [r0, #0x40]
00472a9c: str r5, [r0, #0x44]
00472aa0: str r5, [r0, #0x48]
00472aa4: str r5, [r0, #0x4c]
00472aa8: str r5, [r0, #0x50]
00472aac: str r5, [r0, #0x54]
00472ab0: strb r5, [r0, #0x6c]
00472ab4: strb r5, [r0, #0x7c]
00472ab8: strb r5, [r0, #0x7d]
00472abc: strb r5, [r0, #0x7e]
00472ac0: strb r5, [r0, #0x7f]
00472ac4: str r5, [r0, #0x80]
00472ac8: str r5, [r0, #0x84]
00472acc: str r5, [r0, #0x88]
00472ad0: str r5, [r0, #0x8c]
00472ad4: str r5, [r0, #0x90]
00472ad8: str r5, [r0, #0x94]
00472adc: str r5, [r0, #0x9c]
00472ae0: str r5, [r0, #0xa0]
00472ae4: str r5, [r0, #0xa4]
00472ae8: strb r5, [r0, #0xa9]
00472aec: mov sl, r2
00472af0: mov r4, r0
00472af4: bl #0x50a564
00472af8: ldr ip, [r8, #0x10]
00472afc: ldr r2, [r8, #0x14]
00472b00: ldr r1, [sl, #0x14]
00472b04: mov r3, r5
00472b08: cmp ip, r2
00472b0c: moveq r2, r5
00472b10: mvn ip, #0x80000000
00472b14: str ip, [sp]
00472b18: bl #0x50a504
00472b1c: cmp r0, r5
00472b20: str r0, [r4, #8]
00472b24: beq #0x472c40
00472b28: mov r1, r7
00472b2c: mov r0, r4
00472b30: bl #0x47295c
00472b34: ldr r0, [r4, #8]
00472b38: bl #0x35c854
00472b3c: mov r0, r4
00472b40: bl #0x4718f0
00472b44: ldr r3, [pc, #0x108]
00472b48: ldr r1, [r4, #8]
00472b4c: ldr r5, [r6, r3]
00472b50: ldr r3, [r5, #0x10]
00472b54: ldr r3, [r3, #0x1c]
00472b58: ldr r3, [r3, #4]
00472b5c: mov r0, r3
00472b60: ldr r3, [r3]
00472b64: mov lr, pc
00472b68: ldr pc, [r3, #0x5c]
00472b6c: ldr r3, [r5, #0x10]
00472b70: ldr r0, [r3, #0x1c]
00472b74: bl #0x350ee0
00472b78: ldr r3, [r5, #0x10]
00472b7c: ldr r2, [pc, #0xd4]
00472b80: ldr r1, [r4, #8]
00472b84: ldr r0, [r3, #0x1c]
00472b88: add r2, pc, r2
00472b8c: mov r3, #1
00472b90: bl #0x35a0e4
00472b94: subs r2, r0, #0
00472b98: beq #0x472c08
00472b9c: mov r3, #1
00472ba0: strb r3, [r4, #0x28]
00472ba4: ldr r3, [r5, #0x10]
00472ba8: movw r1, #0x6164
00472bac: movt r1, #0x6d65
00472bb0: ldr r3, [r3, #0x1c]
00472bb4: mov r0, r3
00472bb8: ldr r3, [r3]
00472bbc: mov lr, pc
00472bc0: ldr pc, [r3, #0x1c]
00472bc4: cmp r0, #0
00472bc8: str r0, [r4, #0xc]
00472bcc: beq #0x472bec
00472bd0: ldr r3, [r0]
00472bd4: ldr r3, [r3, #-0xc]
00472bd8: add r0, r0, r3
00472bdc: ldr r3, [r0, #4]
00472be0: add r3, r3, #1
00472be4: str r3, [r0, #4]
00472be8: ldr r0, [r4, #0xc]
00472bec: mov r1, #0
00472bf0: strb r1, [r0, #0x138]
00472bf4: ldr r3, [r4, #0xc]
00472bf8: mov r0, r3
00472bfc: ldr r3, [r3]
00472c00: mov lr, pc
00472c04: ldr pc, [r3, #0x48]
00472c08: mov r0, r4
00472c0c: bl #0x47211c
00472c10: mov r0, r4
00472c14: bl #0x470a54
00472c18: mov r1, #0
00472c1c: mov r0, #8
00472c20: bl #0x310570
00472c24: ldr r1, [r4, #8]
00472c28: mov r5, r0
00472c2c: mov r2, #0
00472c30: bl #0x474d30
00472c34: mov r0, r4
00472c38: mov r1, r5
00472c3c: bl #0x470a84
00472c40: mov r0, r4
00472c44: add sp, sp, #0xc
00472c48: pop {r4, r5, r6, r7, r8, sl, pc}
00472c4c: subseq r2, r2, r4, ror r0
00472c50: muleq r0, r4, lr
00472c54: strdeq r3, r4, [r0], -r4
00472c58: subeq sl, r5, r0, lsr #22

# 0x472c5c _ZN12VisualObjectC2EP10GameObjectRKSsS3_
00472c5c: push {r4, r5, r6, r7, r8, sl, lr}
00472c60: ldr r6, [pc, #0x234]
00472c64: ldr r5, [pc, #0x234]
00472c68: mov ip, #0xbf000000
00472c6c: add r6, pc, r6
00472c70: ldr r5, [r6, r5]
00472c74: mov lr, #0
00472c78: add ip, ip, #0x800000
00472c7c: mov r7, r1
00472c80: add r1, r5, #8
00472c84: mov r5, #0
00472c88: mov r8, r3
00472c8c: sub sp, sp, #0xc
00472c90: str r1, [r0]
00472c94: str lr, [r0, #0x24]
00472c98: str ip, [r0, #0x74]
00472c9c: str lr, [r0, #0x10]
00472ca0: str lr, [r0, #0x14]
00472ca4: str lr, [r0, #0x18]
00472ca8: str lr, [r0, #0x1c]
00472cac: str lr, [r0, #0x20]
00472cb0: str ip, [r0, #0x58]
00472cb4: str ip, [r0, #0x5c]
00472cb8: str ip, [r0, #0x60]
00472cbc: str ip, [r0, #0x64]
00472cc0: str ip, [r0, #0x68]
00472cc4: str r7, [r0, #4]
00472cc8: str r5, [r0, #8]
00472ccc: str r5, [r0, #0xc]
00472cd0: strb r5, [r0, #0x28]
00472cd4: str r5, [r0, #0x2c]
00472cd8: str r5, [r0, #0x30]
00472cdc: str r5, [r0, #0x34]
00472ce0: str r5, [r0, #0x38]
00472ce4: strb r5, [r0, #0x3c]
00472ce8: str r5, [r0, #0x40]
00472cec: str r5, [r0, #0x44]
00472cf0: str r5, [r0, #0x48]
00472cf4: str r5, [r0, #0x4c]
00472cf8: str r5, [r0, #0x50]
00472cfc: str r5, [r0, #0x54]
00472d00: strb r5, [r0, #0x6c]
00472d04: strb r5, [r0, #0x7c]
00472d08: strb r5, [r0, #0x7d]
00472d0c: strb r5, [r0, #0x7e]
00472d10: strb r5, [r0, #0x7f]
00472d14: str r5, [r0, #0x80]
00472d18: str r5, [r0, #0x84]
00472d1c: str r5, [r0, #0x88]
00472d20: str r5, [r0, #0x8c]
00472d24: str r5, [r0, #0x90]
00472d28: str r5, [r0, #0x94]
00472d2c: str r5, [r0, #0x9c]
00472d30: str r5, [r0, #0xa0]
00472d34: str r5, [r0, #0xa4]
00472d38: strb r5, [r0, #0xa9]
00472d3c: mov sl, r2
00472d40: mov r4, r0
00472d44: bl #0x50a564
00472d48: ldr ip, [r8, #0x10]
00472d4c: ldr r2, [r8, #0x14]
00472d50: ldr r1, [sl, #0x14]
00472d54: mov r3, r5
00472d58: cmp ip, r2
00472d5c: moveq r2, r5
00472d60: mvn ip, #0x80000000
00472d64: str ip, [sp]
00472d68: bl #0x50a504
00472d6c: cmp r0, r5
00472d70: str r0, [r4, #8]
00472d74: beq #0x472e90
00472d78: mov r1, r7
00472d7c: mov r0, r4
00472d80: bl #0x47295c
00472d84: ldr r0, [r4, #8]
00472d88: bl #0x35c854
00472d8c: mov r0, r4
00472d90: bl #0x4718f0
00472d94: ldr r3, [pc, #0x108]
00472d98: ldr r1, [r4, #8]
00472d9c: ldr r5, [r6, r3]
00472da0: ldr r3, [r5, #0x10]
00472da4: ldr r3, [r3, #0x1c]
00472da8: ldr r3, [r3, #4]
00472dac: mov r0, r3
00472db0: ldr r3, [r3]
00472db4: mov lr, pc
00472db8: ldr pc, [r3, #0x5c]
00472dbc: ldr r3, [r5, #0x10]
00472dc0: ldr r0, [r3, #0x1c]
00472dc4: bl #0x350ee0
00472dc8: ldr r3, [r5, #0x10]
00472dcc: ldr r2, [pc, #0xd4]
00472dd0: ldr r1, [r4, #8]
00472dd4: ldr r0, [r3, #0x1c]
00472dd8: add r2, pc, r2
00472ddc: mov r3, #1
00472de0: bl #0x35a0e4
00472de4: subs r2, r0, #0
00472de8: beq #0x472e58
00472dec: mov r3, #1
00472df0: strb r3, [r4, #0x28]
00472df4: ldr r3, [r5, #0x10]
00472df8: movw r1, #0x6164
00472dfc: movt r1, #0x6d65
00472e00: ldr r3, [r3, #0x1c]
00472e04: mov r0, r3
00472e08: ldr r3, [r3]
00472e0c: mov lr, pc
00472e10: ldr pc, [r3, #0x1c]
00472e14: cmp r0, #0
00472e18: str r0, [r4, #0xc]
00472e1c: beq #0x472e3c
00472e20: ldr r3, [r0]
00472e24: ldr r3, [r3, #-0xc]
00472e28: add r0, r0, r3
00472e2c: ldr r3, [r0, #4]
00472e30: add r3, r3, #1
00472e34: str r3, [r0, #4]
00472e38: ldr r0, [r4, #0xc]
00472e3c: mov r1, #0
00472e40: strb r1, [r0, #0x138]
00472e44: ldr r3, [r4, #0xc]
00472e48: mov r0, r3
00472e4c: ldr r3, [r3]
00472e50: mov lr, pc
00472e54: ldr pc, [r3, #0x48]
00472e58: mov r0, r4
00472e5c: bl #0x47211c
00472e60: mov r0, r4
00472e64: bl #0x470a54
00472e68: mov r1, #0
00472e6c: mov r0, #8
00472e70: bl #0x310570
00472e74: ldr r1, [r4, #8]
00472e78: mov r5, r0
00472e7c: mov r2, #0
00472e80: bl #0x474d30
00472e84: mov r0, r4
00472e88: mov r1, r5
00472e8c: bl #0x470a84
00472e90: mov r0, r4
00472e94: add sp, sp, #0xc
00472e98: pop {r4, r5, r6, r7, r8, sl, pc}
00472e9c: subseq r1, r2, r4, lsr #28
00472ea0: muleq r0, r4, lr
00472ea4: strdeq r3, r4, [r0], -r4
00472ea8: ldrdeq sl, fp, [r5], #-0x80

# 0x4770d0 _ZN10AnchorBaseC1EP10GameObjectNS_10AnchorTypeE
004770d0: ldr r3, [pc, #0xac]
004770d4: push {r4, lr}
004770d8: ldr lr, [pc, #0xa8]
004770dc: add r3, pc, r3
004770e0: mov ip, #0
004770e4: ldr lr, [r3, lr]
004770e8: str r2, [r0, #4]
004770ec: cmp r1, #0
004770f0: add lr, lr, #8
004770f4: mov r2, #1
004770f8: sub sp, sp, #8
004770fc: mov r4, r0
00477100: str lr, [r0]
00477104: str ip, [r0, #0x14]
00477108: strb r2, [r0, #0x18]
0047710c: str r1, [r0, #8]
00477110: str ip, [r0, #0xc]
00477114: str ip, [r0, #0x10]
00477118: beq #0x477130
0047711c: mov r0, r4
00477120: bl #0x477074
00477124: mov r0, r4
00477128: add sp, sp, #8
0047712c: pop {r4, pc}
00477130: ldr r2, [pc, #0x54]
00477134: ldr r2, [r3, r2]
00477138: ldr r2, [r2]
0047713c: cmp r2, #2
00477140: streq r1, [r1]
00477144: beq #0x47711c
00477148: cmp r2, #1
0047714c: bne #0x47711c
00477150: ldr r0, [pc, #0x38]
00477154: ldr r1, [pc, #0x38]
00477158: ldr r2, [pc, #0x38]
0047715c: ldr r0, [r3, r0]
00477160: ldr r3, [pc, #0x34]
00477164: mov ip, #0x18
00477168: add r1, pc, r1
0047716c: add r2, pc, r2
00477170: add r3, pc, r3
00477174: add r0, r0, #0xa8
00477178: str ip, [sp]
0047717c: bl #0x30e004
00477180: b #0x47711c
00477184: ldrheq sp, [r1], #-0x94
00477188: strheq r1, [r0], -ip
0047718c: andeq r3, r0, r0, asr #19
00477190: andeq r1, r0, r0, asr #19
00477194: subeq r7, r4, r0, ror r2
00477198: subeq fp, r4, ip, lsr #4
0047719c: subeq r6, r5, r8, ror r7

# 0x4771a0 _ZN10AnchorBaseC2EP10GameObjectNS_10AnchorTypeE
004771a0: ldr r3, [pc, #0xac]
004771a4: push {r4, lr}
004771a8: ldr lr, [pc, #0xa8]
004771ac: add r3, pc, r3
004771b0: mov ip, #0
004771b4: ldr lr, [r3, lr]
004771b8: str r2, [r0, #4]
004771bc: cmp r1, #0
004771c0: add lr, lr, #8
004771c4: mov r2, #1
004771c8: sub sp, sp, #8
004771cc: mov r4, r0
004771d0: str lr, [r0]
004771d4: str ip, [r0, #0x14]
004771d8: strb r2, [r0, #0x18]
004771dc: str r1, [r0, #8]
004771e0: str ip, [r0, #0xc]
004771e4: str ip, [r0, #0x10]
004771e8: beq #0x477200
004771ec: mov r0, r4
004771f0: bl #0x477074
004771f4: mov r0, r4
004771f8: add sp, sp, #8
004771fc: pop {r4, pc}
00477200: ldr r2, [pc, #0x54]
00477204: ldr r2, [r3, r2]
00477208: ldr r2, [r2]
0047720c: cmp r2, #2
00477210: streq r1, [r1]
00477214: beq #0x4771ec
00477218: cmp r2, #1
0047721c: bne #0x4771ec
00477220: ldr r0, [pc, #0x38]
00477224: ldr r1, [pc, #0x38]
00477228: ldr r2, [pc, #0x38]
0047722c: ldr r0, [r3, r0]
00477230: ldr r3, [pc, #0x34]
00477234: mov ip, #0x18
00477238: add r1, pc, r1
0047723c: add r2, pc, r2
00477240: add r3, pc, r3
00477244: add r0, r0, #0xa8
00477248: str ip, [sp]
0047724c: bl #0x30e004
00477250: b #0x4771ec
00477254: subseq sp, r1, r4, ror #17
00477258: strheq r1, [r0], -ip
0047725c: andeq r3, r0, r0, asr #19
00477260: andeq r1, r0, r0, asr #19
00477264: subeq r7, r4, r0, lsr #3
00477268: subeq fp, r4, ip, asr r1
0047726c: subeq r6, r5, r8, lsr #13

# 0x477a80 _ZN13AnchorForwardC1EP10GameObjectfffN10AnchorBase10AnchorTypeE
00477a80: push {r4, r5, r6, r7, r8, sl, lr}
00477a84: sub sp, sp, #0x1c
00477a88: mov r6, r2
00477a8c: ldr r5, [pc, #0x20c]
00477a90: ldr r2, [sp, #0x3c]
00477a94: mov r4, r0
00477a98: mov r8, r3
00477a9c: mov r7, r1
00477aa0: bl #0x4771a0
00477aa4: ldr r3, [pc, #0x1f8]
00477aa8: add r5, pc, r5
00477aac: mov r1, #0
00477ab0: ldr r3, [r5, r3]
00477ab4: mov sl, #0
00477ab8: mov r0, r6
00477abc: add r3, r3, #8
00477ac0: str r3, [r4]
00477ac4: ldr r3, [sp, #0x38]
00477ac8: str r6, [r4, #0x1c]
00477acc: str r8, [r4, #0x20]
00477ad0: str r3, [r4, #0x24]
00477ad4: mov r3, #5
00477ad8: str r3, [r4, #0x3c]
00477adc: str sl, [r4, #0x28]
00477ae0: str r1, [r4, #0x2c]
00477ae4: str r1, [r4, #0x30]
00477ae8: str r1, [r4, #0x34]
00477aec: strb sl, [r4, #0x38]
00477af0: str r1, [r4, #0x40]
00477af4: str r1, [r4, #0x44]
00477af8: str r1, [r4, #0x48]
00477afc: strb sl, [r4, #0x4c]
00477b00: str sl, [r4, #0x50]
00477b04: str r1, [r4, #0x54]
00477b08: str r1, [r4, #0x58]
00477b0c: str r1, [r4, #0x5c]
00477b10: str r1, [r4, #0x60]
00477b14: bl #0x30e4b4
00477b18: cmp r0, sl
00477b1c: bne #0x477b40
00477b20: ldr r3, [pc, #0x180]
00477b24: ldr r3, [r5, r3]
00477b28: ldr r3, [r3]
00477b2c: cmp r3, #2
00477b30: streq sl, [sl]
00477b34: beq #0x477b40
00477b38: cmp r3, #1
00477b3c: beq #0x477c34
00477b40: mov r0, r8
00477b44: mov r1, #0
00477b48: bl #0x30e4b4
00477b4c: cmp r0, #0
00477b50: bne #0x477b78
00477b54: ldr r3, [pc, #0x14c]
00477b58: ldr r3, [r5, r3]
00477b5c: ldr r3, [r3]
00477b60: cmp r3, #2
00477b64: moveq r3, #0
00477b68: streq r3, [r3]
00477b6c: beq #0x477b78
00477b70: cmp r3, #1
00477b74: beq #0x477c6c
00477b78: ldr r6, [r4, #0x24]
00477b7c: mov r1, #0
00477b80: mov r0, r6
00477b84: bl #0x30e4b4
00477b88: cmp r0, #0
00477b8c: beq #0x477bdc
00477b90: mov r0, r6
00477b94: mov r1, #0x3f800000
00477b98: bl #0x30e9ac
00477b9c: cmp r0, #0
00477ba0: beq #0x477bdc
00477ba4: cmp r7, #0
00477ba8: beq #0x477bc8
00477bac: add r5, sp, #0xc
00477bb0: mov r1, r7
00477bb4: mov r0, r5
00477bb8: bl #0x33dd2c
00477bbc: mov r0, r5
00477bc0: bl #0x33ff54
00477bc4: str r0, [r4, #0x28]
00477bc8: mov r0, r4
00477bcc: bl #0x4779dc
00477bd0: mov r0, r4
00477bd4: add sp, sp, #0x1c
00477bd8: pop {r4, r5, r6, r7, r8, sl, pc}
00477bdc: ldr r3, [pc, #0xc4]
00477be0: ldr r3, [r5, r3]
00477be4: ldr r3, [r3]
00477be8: cmp r3, #2
00477bec: moveq r3, #0
00477bf0: streq r3, [r3]
00477bf4: beq #0x477ba4
00477bf8: cmp r3, #1
00477bfc: bne #0x477ba4
00477c00: ldr r0, [pc, #0xa4]
00477c04: ldr r1, [pc, #0xa4]
00477c08: ldr r2, [pc, #0xa4]
00477c0c: ldr r0, [r5, r0]
00477c10: ldr r3, [pc, #0xa0]
00477c14: mov ip, #0x32
00477c18: add r1, pc, r1
00477c1c: add r2, pc, r2
00477c20: add r3, pc, r3
00477c24: add r0, r0, #0xa8
00477c28: str ip, [sp]
00477c2c: bl #0x30e004
00477c30: b #0x477ba4
00477c34: ldr r0, [pc, #0x70]
00477c38: ldr r1, [pc, #0x7c]
00477c3c: ldr r2, [pc, #0x7c]
00477c40: ldr r0, [r5, r0]
00477c44: ldr r3, [pc, #0x78]
00477c48: mov ip, #0x30
00477c4c: add r1, pc, r1
00477c50: add r0, r0, #0xa8
00477c54: add r2, pc, r2
00477c58: add r3, pc, r3
00477c5c: str ip, [sp]
00477c60: bl #0x30e004
00477c64: ldr r8, [r4, #0x20]
00477c68: b #0x477b40
00477c6c: ldr r0, [pc, #0x38]
00477c70: ldr r1, [pc, #0x50]
00477c74: ldr r2, [pc, #0x50]
00477c78: ldr r0, [r5, r0]
00477c7c: ldr r3, [pc, #0x4c]
00477c80: mov ip, #0x31
00477c84: add r1, pc, r1
00477c88: add r2, pc, r2
00477c8c: add r3, pc, r3
00477c90: add r0, r0, #0xa8
00477c94: str ip, [sp]
00477c98: bl #0x30e004
00477c9c: b #0x477b78
00477ca0: subseq ip, r1, r8, ror #31
00477ca4: muleq r0, r8, r3
00477ca8: andeq r3, r0, r0, asr #19
00477cac: andeq r1, r0, r0, asr #19
00477cb0: subeq r6, r4, r0, asr #15
00477cb4: subeq r5, r5, r4, lsr #27
00477cb8: subeq r5, r5, r0, lsr sp
00477cbc: subeq r6, r4, ip, lsl #15
00477cc0: subeq r5, r5, r4, ror #25
00477cc4: strdeq r5, r6, [r5], #-0xc8
00477cc8: subeq r6, r4, r4, asr r7
00477ccc: subeq r5, r5, r0, lsr #26
00477cd0: subeq r5, r5, r4, asr #25

# 0x477cd4 _ZN13AnchorForwardC2EP10GameObjectfffN10AnchorBase10AnchorTypeE
00477cd4: push {r4, r5, r6, r7, r8, sl, lr}
00477cd8: sub sp, sp, #0x1c
00477cdc: mov r6, r2
00477ce0: ldr r5, [pc, #0x20c]
00477ce4: ldr r2, [sp, #0x3c]
00477ce8: mov r4, r0
00477cec: mov r8, r3
00477cf0: mov r7, r1
00477cf4: bl #0x4771a0
00477cf8: ldr r3, [pc, #0x1f8]
00477cfc: add r5, pc, r5
00477d00: mov r1, #0
00477d04: ldr r3, [r5, r3]
00477d08: mov sl, #0
00477d0c: mov r0, r6
00477d10: add r3, r3, #8
00477d14: str r3, [r4]
00477d18: ldr r3, [sp, #0x38]
00477d1c: str r6, [r4, #0x1c]
00477d20: str r8, [r4, #0x20]
00477d24: str r3, [r4, #0x24]
00477d28: mov r3, #5
00477d2c: str r3, [r4, #0x3c]
00477d30: str sl, [r4, #0x28]
00477d34: str r1, [r4, #0x2c]
00477d38: str r1, [r4, #0x30]
00477d3c: str r1, [r4, #0x34]
00477d40: strb sl, [r4, #0x38]
00477d44: str r1, [r4, #0x40]
00477d48: str r1, [r4, #0x44]
00477d4c: str r1, [r4, #0x48]
00477d50: strb sl, [r4, #0x4c]
00477d54: str sl, [r4, #0x50]
00477d58: str r1, [r4, #0x54]
00477d5c: str r1, [r4, #0x58]
00477d60: str r1, [r4, #0x5c]
00477d64: str r1, [r4, #0x60]
00477d68: bl #0x30e4b4
00477d6c: cmp r0, sl
00477d70: bne #0x477d94
00477d74: ldr r3, [pc, #0x180]
00477d78: ldr r3, [r5, r3]
00477d7c: ldr r3, [r3]
00477d80: cmp r3, #2
00477d84: streq sl, [sl]
00477d88: beq #0x477d94
00477d8c: cmp r3, #1
00477d90: beq #0x477e88
00477d94: mov r0, r8
00477d98: mov r1, #0
00477d9c: bl #0x30e4b4
00477da0: cmp r0, #0
00477da4: bne #0x477dcc
00477da8: ldr r3, [pc, #0x14c]
00477dac: ldr r3, [r5, r3]
00477db0: ldr r3, [r3]
00477db4: cmp r3, #2
00477db8: moveq r3, #0
00477dbc: streq r3, [r3]
00477dc0: beq #0x477dcc
00477dc4: cmp r3, #1
00477dc8: beq #0x477ec0
00477dcc: ldr r6, [r4, #0x24]
00477dd0: mov r1, #0
00477dd4: mov r0, r6
00477dd8: bl #0x30e4b4
00477ddc: cmp r0, #0
00477de0: beq #0x477e30
00477de4: mov r0, r6
00477de8: mov r1, #0x3f800000
00477dec: bl #0x30e9ac
00477df0: cmp r0, #0
00477df4: beq #0x477e30
00477df8: cmp r7, #0
00477dfc: beq #0x477e1c
00477e00: add r5, sp, #0xc
00477e04: mov r1, r7
00477e08: mov r0, r5
00477e0c: bl #0x33dd2c
00477e10: mov r0, r5
00477e14: bl #0x33ff54
00477e18: str r0, [r4, #0x28]
00477e1c: mov r0, r4
00477e20: bl #0x4779dc
00477e24: mov r0, r4
00477e28: add sp, sp, #0x1c
00477e2c: pop {r4, r5, r6, r7, r8, sl, pc}
00477e30: ldr r3, [pc, #0xc4]
00477e34: ldr r3, [r5, r3]
00477e38: ldr r3, [r3]
00477e3c: cmp r3, #2
00477e40: moveq r3, #0
00477e44: streq r3, [r3]
00477e48: beq #0x477df8
00477e4c: cmp r3, #1
00477e50: bne #0x477df8
00477e54: ldr r0, [pc, #0xa4]
00477e58: ldr r1, [pc, #0xa4]
00477e5c: ldr r2, [pc, #0xa4]
00477e60: ldr r0, [r5, r0]
00477e64: ldr r3, [pc, #0xa0]
00477e68: mov ip, #0x32
00477e6c: add r1, pc, r1
00477e70: add r2, pc, r2
00477e74: add r3, pc, r3
00477e78: add r0, r0, #0xa8
00477e7c: str ip, [sp]
00477e80: bl #0x30e004
00477e84: b #0x477df8
00477e88: ldr r0, [pc, #0x70]
00477e8c: ldr r1, [pc, #0x7c]
00477e90: ldr r2, [pc, #0x7c]
00477e94: ldr r0, [r5, r0]
00477e98: ldr r3, [pc, #0x78]
00477e9c: mov ip, #0x30
00477ea0: add r1, pc, r1
00477ea4: add r0, r0, #0xa8
00477ea8: add r2, pc, r2
00477eac: add r3, pc, r3
00477eb0: str ip, [sp]
00477eb4: bl #0x30e004
00477eb8: ldr r8, [r4, #0x20]
00477ebc: b #0x477d94
00477ec0: ldr r0, [pc, #0x38]
00477ec4: ldr r1, [pc, #0x50]
00477ec8: ldr r2, [pc, #0x50]
00477ecc: ldr r0, [r5, r0]
00477ed0: ldr r3, [pc, #0x4c]
00477ed4: mov ip, #0x31
00477ed8: add r1, pc, r1
00477edc: add r2, pc, r2
00477ee0: add r3, pc, r3
00477ee4: add r0, r0, #0xa8
00477ee8: str ip, [sp]
00477eec: bl #0x30e004
00477ef0: b #0x477dcc

# 0x478578 _ZN11AnchorGroupC1EP10GameObjectN10AnchorBase10AnchorTypeE
00478578: push {r4, r5, r6, lr}
0047857c: ldr r4, [pc, #0x28]
00478580: mov r5, r0
00478584: bl #0x4771a0
00478588: ldr r3, [pc, #0x20]
0047858c: add r4, pc, r4
00478590: mov r0, r5
00478594: ldr r3, [r4, r3]
00478598: add r3, r3, #8
0047859c: str r3, [r5]
004785a0: bl #0x478520
004785a4: mov r0, r5
004785a8: pop {r4, r5, r6, pc}
004785ac: subseq ip, r1, r4, lsl #10
004785b0: andeq r4, r0, r0, lsl #5

# 0x4785b4 _ZN11AnchorGroupC2EP10GameObjectN10AnchorBase10AnchorTypeE
004785b4: push {r4, r5, r6, lr}
004785b8: ldr r4, [pc, #0x28]
004785bc: mov r5, r0
004785c0: bl #0x4771a0
004785c4: ldr r3, [pc, #0x20]
004785c8: add r4, pc, r4
004785cc: mov r0, r5
004785d0: ldr r3, [r4, r3]
004785d4: add r3, r3, #8
004785d8: str r3, [r5]
004785dc: bl #0x478520
004785e0: mov r0, r5
004785e4: pop {r4, r5, r6, pc}
004785e8: subseq ip, r1, r8, asr #9
004785ec: andeq r4, r0, r0, lsl #5

# 0x4a191c _ZN14ObjectSearcher10TargetList12SetRefObjectEP10GameObject
004a191c: cmp r1, #0
004a1920: push {r4, lr}
004a1924: mov r4, r0
004a1928: beq #0x4a194c
004a192c: str r1, [r0, #0x2c]
004a1930: ldr r3, [r1]
004a1934: mov r0, r1
004a1938: mov lr, pc
004a193c: ldr pc, [r3, #0x24]
004a1940: cmp r0, #0
004a1944: ldrne r3, [r4, #0x2c]
004a1948: strne r3, [r4, #0x30]
004a194c: pop {r4, pc}

# 0x4a2730 _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
004a2730: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a2734: ldr r6, [pc, #0x10c]
004a2738: ldr r7, [pc, #0x10c]
004a273c: mov r5, #0
004a2740: add r6, pc, r6
004a2744: str r5, [r0]
004a2748: str r5, [r0, #4]
004a274c: str r5, [r0, #8]
004a2750: str r5, [r0, #0xc]
004a2754: str r5, [r0, #0x10]
004a2758: str r5, [r0, #0x14]
004a275c: str r5, [r0, #0x18]
004a2760: str r5, [r0, #0x1c]
004a2764: str r5, [r0, #0x20]
004a2768: str r5, [r0, #0x24]
004a276c: mov r4, r0
004a2770: mov r8, r2
004a2774: mov sl, r3
004a2778: mov fp, r1
004a277c: ldr sb, [sp, #0x28]
004a2780: bl #0x4a2240
004a2784: ldr r2, [r6, r7]
004a2788: mov r3, r4
004a278c: str r8, [r4, #0x34]
004a2790: str r2, [r4, #0x28]
004a2794: str sl, [r4, #0x38]
004a2798: str r5, [r4, #0x2c]
004a279c: str r5, [r4, #0x30]
004a27a0: str r5, [r4, #0x40]
004a27a4: strb r5, [r3, #0x3c]!
004a27a8: ldr r1, [r4, #0x10]
004a27ac: ldr r2, [r4]
004a27b0: str r3, [r4, #0x48]
004a27b4: str r5, [r4, #0x4c]
004a27b8: cmp r1, r2
004a27bc: str r3, [r4, #0x44]
004a27c0: beq #0x4a27dc
004a27c4: mov r0, r4
004a27c8: bl #0x38fb18
004a27cc: ldr r2, [r4, #0x10]
004a27d0: ldr r3, [r4]
004a27d4: cmp r2, r3
004a27d8: bne #0x4a27c4
004a27dc: cmp sb, #1
004a27e0: beq #0x4a2808
004a27e4: cmp sb, #2
004a27e8: beq #0x4a2828
004a27ec: ldr r3, [r6, r7]
004a27f0: mov r0, r4
004a27f4: mov r1, fp
004a27f8: str r3, [r4, #0x28]
004a27fc: bl #0x4a191c
004a2800: mov r0, r4
004a2804: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2808: ldr r3, [pc, #0x40]
004a280c: mov r0, r4
004a2810: mov r1, fp
004a2814: ldr r3, [r6, r3]
004a2818: str r3, [r4, #0x28]
004a281c: bl #0x4a191c
004a2820: mov r0, r4
004a2824: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2828: ldr r3, [pc, #0x24]
004a282c: mov r0, r4
004a2830: mov r1, fp
004a2834: ldr r3, [r6, r3]
004a2838: str r3, [r4, #0x28]
004a283c: bl #0x4a191c
004a2840: mov r0, r4
004a2844: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2848: subeq r2, pc, r0, asr r3
004a284c: andeq r2, r0, r4, ror ip
004a2850: andeq r4, r0, r4, asr #21
004a2854: ldrdeq r1, r2, [r0], -r4

# 0x4a348c _ZN14ObjectSearcher10TargetListC2EP10GameObjectiii
004a348c: push {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a3490: ldr r6, [pc, #0x10c]
004a3494: ldr r7, [pc, #0x10c]
004a3498: mov r5, #0
004a349c: add r6, pc, r6
004a34a0: str r5, [r0]
004a34a4: str r5, [r0, #4]
004a34a8: str r5, [r0, #8]
004a34ac: str r5, [r0, #0xc]
004a34b0: str r5, [r0, #0x10]
004a34b4: str r5, [r0, #0x14]
004a34b8: str r5, [r0, #0x18]
004a34bc: str r5, [r0, #0x1c]
004a34c0: str r5, [r0, #0x20]
004a34c4: str r5, [r0, #0x24]
004a34c8: mov r4, r0
004a34cc: mov r8, r2
004a34d0: mov sl, r3
004a34d4: mov fp, r1
004a34d8: ldr sb, [sp, #0x28]
004a34dc: bl #0x4a2240
004a34e0: ldr r2, [r6, r7]
004a34e4: mov r3, r4
004a34e8: str r8, [r4, #0x34]
004a34ec: str r2, [r4, #0x28]
004a34f0: str sl, [r4, #0x38]
004a34f4: str r5, [r4, #0x2c]
004a34f8: str r5, [r4, #0x30]
004a34fc: str r5, [r4, #0x40]
004a3500: strb r5, [r3, #0x3c]!
004a3504: ldr r1, [r4, #0x10]
004a3508: ldr r2, [r4]
004a350c: str r3, [r4, #0x48]
004a3510: str r5, [r4, #0x4c]
004a3514: cmp r1, r2
004a3518: str r3, [r4, #0x44]
004a351c: beq #0x4a3538
004a3520: mov r0, r4
004a3524: bl #0x38fb18
004a3528: ldr r2, [r4, #0x10]
004a352c: ldr r3, [r4]
004a3530: cmp r2, r3
004a3534: bne #0x4a3520
004a3538: cmp sb, #1
004a353c: beq #0x4a3564
004a3540: cmp sb, #2
004a3544: beq #0x4a3584
004a3548: ldr r3, [r6, r7]
004a354c: mov r0, r4
004a3550: mov r1, fp
004a3554: str r3, [r4, #0x28]
004a3558: bl #0x4a191c
004a355c: mov r0, r4
004a3560: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a3564: ldr r3, [pc, #0x40]
004a3568: mov r0, r4
004a356c: mov r1, fp
004a3570: ldr r3, [r6, r3]
004a3574: str r3, [r4, #0x28]
004a3578: bl #0x4a191c
004a357c: mov r0, r4
004a3580: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a3584: ldr r3, [pc, #0x24]
004a3588: mov r0, r4
004a358c: mov r1, fp
004a3590: ldr r3, [r6, r3]
004a3594: str r3, [r4, #0x28]
004a3598: bl #0x4a191c
004a359c: mov r0, r4
004a35a0: pop {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a35a4: strdeq r1, r2, [pc], #-0x54
004a35a8: andeq r2, r0, r4, ror ip
004a35ac: andeq r4, r0, r4, asr #21
004a35b0: ldrdeq r1, r2, [r0], -r4

# 0x4ae234 _ZN7Structs19GetMemberIDByStringINS_17OpenableContainerEEEiPKc
004ae234: ldr r3, [pc, #0x48]
004ae238: ldr r2, [pc, #0x48]
004ae23c: push {r4, r5, r6, lr}
004ae240: add r3, pc, r3
004ae244: mov r6, r0
004ae248: ldr r5, [r3, r2]
004ae24c: mov r4, #0
004ae250: ldr r1, [r5, #0x14]
004ae254: mov r0, r6
004ae258: bl #0x30e31c
004ae25c: cmp r0, #0
004ae260: beq #0x4ae27c
004ae264: add r4, r4, #1
004ae268: cmp r4, #8
004ae26c: add r5, r5, #0x18
004ae270: bne #0x4ae250
004ae274: mvn r0, #0
004ae278: pop {r4, r5, r6, pc}
004ae27c: mov r0, r4
004ae280: pop {r4, r5, r6, pc}
004ae284: subeq r6, lr, r0, asr r8
004ae288: strheq r0, [r0], -ip

# 0x4d9ee8 _ZN7Structs17OpenableContainer8finalizeEv
004d9ee8: push {r4, lr}
004d9eec: mov r4, r0
004d9ef0: ldr r0, [r0, #0x18]
004d9ef4: cmp r0, #0
004d9ef8: beq #0x4d9f0c
004d9efc: bl #0x310440
004d9f00: mov r3, #0
004d9f04: str r3, [r4, #0x14]
004d9f08: str r3, [r4, #0x18]
004d9f0c: pop {r4, pc}

# 0x4d9f10 _ZN7Structs17OpenableContainerD1Ev
004d9f10: push {r4, lr}
004d9f14: ldr r3, [pc, #0x2c]
004d9f18: ldr r2, [pc, #0x2c]
004d9f1c: mov r4, r0
004d9f20: add r3, pc, r3
004d9f24: ldr r0, [r0, #0x18]
004d9f28: ldr r2, [r3, r2]
004d9f2c: cmp r0, #0
004d9f30: add r2, r2, #8
004d9f34: str r2, [r4]
004d9f38: beq #0x4d9f40
004d9f3c: bl #0x310440
004d9f40: mov r0, r4
004d9f44: pop {r4, pc}
004d9f48: subeq sl, fp, r0, ror fp
004d9f4c: muleq r0, r4, sb

# 0x4d9f50 _ZN7Structs17OpenableContainerD0Ev
004d9f50: push {r4, lr}
004d9f54: mov r4, r0
004d9f58: bl #0x4d9f10
004d9f5c: mov r0, r4
004d9f60: bl #0x310440
004d9f64: mov r0, r4
004d9f68: pop {r4, pc}

# 0x4d9f6c _ZN7Structs17OpenableContainerD2Ev
004d9f6c: push {r4, lr}
004d9f70: ldr r3, [pc, #0x2c]
004d9f74: ldr r2, [pc, #0x2c]
004d9f78: mov r4, r0
004d9f7c: add r3, pc, r3
004d9f80: ldr r0, [r0, #0x18]
004d9f84: ldr r2, [r3, r2]
004d9f88: cmp r0, #0
004d9f8c: add r2, r2, #8
004d9f90: str r2, [r4]
004d9f94: beq #0x4d9f9c
004d9f98: bl #0x310440
004d9f9c: mov r0, r4
004d9fa0: pop {r4, pc}
004d9fa4: subeq sl, fp, r4, lsl fp
004d9fa8: muleq r0, r4, sb

# 0x4fdb90 _ZN7Structs17OpenableContainer4readEP11IStreamBase
004fdb90: push {r4, r5, r6, lr}
004fdb94: mov r4, r0
004fdb98: sub sp, sp, #8
004fdb9c: mov r0, r1
004fdba0: mov r5, r1
004fdba4: add r1, r4, #4
004fdba8: bl #0x459090
004fdbac: mov r3, #1
004fdbb0: cmp r3, #0
004fdbb4: str r3, [sp, #4]
004fdbb8: bne #0x4fdbfc
004fdbbc: add r3, r4, #5
004fdbc0: add r2, r4, #6
004fdbc4: ldrb r0, [r2, #1]
004fdbc8: ldrb r1, [r3, #-1]
004fdbcc: cmp r3, r2
004fdbd0: eor r1, r0, r1
004fdbd4: strb r1, [r3, #-1]
004fdbd8: ldrb r0, [r2, #1]
004fdbdc: eor r1, r1, r0
004fdbe0: strb r1, [r2, #1]
004fdbe4: ldrb r0, [r3, #-1]
004fdbe8: sub r2, r2, #1
004fdbec: eor r1, r1, r0
004fdbf0: strb r1, [r3, #-1]
004fdbf4: add r3, r3, #1
004fdbf8: blo #0x4fdbc4
004fdbfc: mov r0, r5
004fdc00: add r1, r4, #8
004fdc04: bl #0x459090
004fdc08: mov r3, #1
004fdc0c: cmp r3, #0
004fdc10: str r3, [sp, #4]
004fdc14: bne #0x4fdc58
004fdc18: add r3, r4, #9
004fdc1c: add r2, r4, #0xa
004fdc20: ldrb r0, [r2, #1]
004fdc24: ldrb r1, [r3, #-1]
004fdc28: cmp r3, r2
004fdc2c: eor r1, r0, r1
004fdc30: strb r1, [r3, #-1]
004fdc34: ldrb r0, [r2, #1]
004fdc38: eor r1, r1, r0
004fdc3c: strb r1, [r2, #1]
004fdc40: ldrb r0, [r3, #-1]
004fdc44: sub r2, r2, #1
004fdc48: eor r1, r1, r0
004fdc4c: strb r1, [r3, #-1]
004fdc50: add r3, r3, #1
004fdc54: blo #0x4fdc20
004fdc58: add r1, r4, #0xc
004fdc5c: mov r0, r5
004fdc60: bl #0x4db89c
004fdc64: mov r0, r5
004fdc68: add r1, r4, #0x10
004fdc6c: bl #0x459090
004fdc70: mov r3, #1
004fdc74: cmp r3, #0
004fdc78: str r3, [sp, #4]
004fdc7c: bne #0x4fdcc0
004fdc80: add r3, r4, #0x11
004fdc84: add r2, r4, #0x12
004fdc88: ldrb r0, [r2, #1]
004fdc8c: ldrb r1, [r3, #-1]
004fdc90: cmp r3, r2
004fdc94: eor r1, r0, r1
004fdc98: strb r1, [r3, #-1]
004fdc9c: ldrb r0, [r2, #1]
004fdca0: eor r1, r1, r0
004fdca4: strb r1, [r2, #1]
004fdca8: ldrb r0, [r3, #-1]
004fdcac: sub r2, r2, #1
004fdcb0: eor r1, r1, r0
004fdcb4: strb r1, [r3, #-1]
004fdcb8: add r3, r3, #1
004fdcbc: blo #0x4fdc88
004fdcc0: mov r0, r5
004fdcc4: add r1, r4, #0x14
004fdcc8: bl #0x3df1a0
004fdccc: mov r3, #1
004fdcd0: cmp r3, #0
004fdcd4: str r3, [sp, #4]
004fdcd8: bne #0x4fdd1c
004fdcdc: add r3, r4, #0x15
004fdce0: add r2, r4, #0x16
004fdce4: ldrb r0, [r2, #1]
004fdce8: ldrb r1, [r3, #-1]
004fdcec: cmp r3, r2
004fdcf0: eor r1, r0, r1
004fdcf4: strb r1, [r3, #-1]
004fdcf8: ldrb r0, [r2, #1]
004fdcfc: eor r1, r1, r0
004fdd00: strb r1, [r2, #1]
004fdd04: ldrb r0, [r3, #-1]
004fdd08: sub r2, r2, #1
004fdd0c: eor r1, r1, r0
004fdd10: strb r1, [r3, #-1]
004fdd14: add r3, r3, #1
004fdd18: blo #0x4fdce4
004fdd1c: ldr r0, [r4, #0x18]
004fdd20: cmp r0, #0
004fdd24: beq #0x4fdd2c
004fdd28: bl #0x310440
004fdd2c: ldr r0, [r4, #0x14]
004fdd30: mov r1, #1
004fdd34: mov r6, #0
004fdd38: add r0, r0, r1
004fdd3c: bl #0x31056c
004fdd40: ldr r2, [r4, #0x14]
004fdd44: mov r1, r0
004fdd48: str r0, [r4, #0x18]
004fdd4c: mov r3, r6
004fdd50: mov r0, r5
004fdd54: bl #0x317454
004fdd58: ldr r3, [r4, #0x14]
004fdd5c: ldr r2, [r4, #0x18]
004fdd60: mov r0, r5
004fdd64: add r1, r4, #0x1c
004fdd68: strb r6, [r2, r3]
004fdd6c: bl #0x459090
004fdd70: mov r3, #1
004fdd74: cmp r3, r6
004fdd78: str r3, [sp, #4]
004fdd7c: bne #0x4fddc0
004fdd80: add r3, r4, #0x1d
004fdd84: add r2, r4, #0x1e
004fdd88: ldrb r0, [r2, #1]
004fdd8c: ldrb r1, [r3, #-1]
004fdd90: cmp r3, r2
004fdd94: eor r1, r0, r1
004fdd98: strb r1, [r3, #-1]
004fdd9c: ldrb r0, [r2, #1]
004fdda0: eor r1, r1, r0
004fdda4: strb r1, [r2, #1]
004fdda8: ldrb r0, [r3, #-1]
004fddac: sub r2, r2, #1
004fddb0: eor r1, r1, r0
004fddb4: strb r1, [r3, #-1]
004fddb8: add r3, r3, #1
004fddbc: blo #0x4fdd88
004fddc0: mov r0, r5
004fddc4: add r1, r4, #0x20
004fddc8: bl #0x459090
004fddcc: mov r3, #1
004fddd0: cmp r3, #0
004fddd4: str r3, [sp, #4]
004fddd8: bne #0x4fde1c
004fdddc: add r3, r4, #0x21
004fdde0: add r2, r4, #0x22
004fdde4: ldrb r0, [r2, #1]
004fdde8: ldrb r1, [r3, #-1]
004fddec: cmp r3, r2
004fddf0: eor r1, r0, r1
004fddf4: strb r1, [r3, #-1]
004fddf8: ldrb r0, [r2, #1]
004fddfc: eor r1, r1, r0
004fde00: strb r1, [r2, #1]
004fde04: ldrb r0, [r3, #-1]
004fde08: sub r2, r2, #1
004fde0c: eor r1, r1, r0
004fde10: strb r1, [r3, #-1]
004fde14: add r3, r3, #1
004fde18: blo #0x4fdde4
004fde1c: mov r0, r5
004fde20: add r1, r4, #0x24
004fde24: bl #0x459090
004fde28: mov r3, #1
004fde2c: cmp r3, #0
004fde30: str r3, [sp, #4]
004fde34: bne #0x4fde78
004fde38: add r3, r4, #0x26
004fde3c: add r4, r4, #0x25
004fde40: ldrb r1, [r3, #1]
004fde44: ldrb r2, [r4, #-1]
004fde48: cmp r4, r3
004fde4c: eor r2, r1, r2
004fde50: strb r2, [r4, #-1]
004fde54: ldrb r1, [r3, #1]
004fde58: eor r2, r2, r1
004fde5c: strb r2, [r3, #1]
004fde60: ldrb r1, [r4, #-1]
004fde64: sub r3, r3, #1
004fde68: eor r2, r2, r1
004fde6c: strb r2, [r4, #-1]
004fde70: add r4, r4, #1
004fde74: blo #0x4fde40
004fde78: add sp, sp, #8
004fde7c: pop {r4, r5, r6, pc}

# 0x510b4c _ZN11PropertyMap16GetThisClassNameEv
00510b4c: push {r4, lr}
00510b50: mov r4, r0
00510b54: ldr r0, [r0, #0x1c]
00510b58: ldr r3, [pc, #0x6c]
00510b5c: sub sp, sp, #8
00510b60: cmp r0, #0
00510b64: add r3, pc, r3
00510b68: beq #0x510b74
00510b6c: add sp, sp, #8
00510b70: pop {r4, pc}
00510b74: ldr r2, [pc, #0x54]
00510b78: ldr r2, [r3, r2]
00510b7c: ldr r2, [r2]
00510b80: cmp r2, #2
00510b84: streq r0, [r0]
00510b88: beq #0x510b6c
00510b8c: cmp r2, #1
00510b90: bne #0x510b6c
00510b94: ldr r0, [pc, #0x38]
00510b98: ldr r1, [pc, #0x38]
00510b9c: ldr r2, [pc, #0x38]
00510ba0: ldr r0, [r3, r0]
00510ba4: ldr r3, [pc, #0x34]
00510ba8: mov ip, #0x92
00510bac: add r1, pc, r1
00510bb0: add r0, r0, #0xa8
00510bb4: add r2, pc, r2
00510bb8: add r3, pc, r3
00510bbc: str ip, [sp]
00510bc0: bl #0x30e004
00510bc4: ldr r0, [r4, #0x1c]
00510bc8: b #0x510b6c
00510bcc: subeq r3, r8, ip, lsr #30
00510bd0: andeq r3, r0, r0, asr #19
00510bd4: andeq r1, r0, r0, asr #19
00510bd8: eorseq sp, sl, ip, lsr #16
00510bdc: eorseq fp, ip, ip, lsr #8
00510be0: eorseq fp, ip, r0, asr #8

# 0x510dec _ZN11PropertyMap19DestroyPropertyMapsEv
00510dec: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00510df0: ldr sl, [pc, #0x1dc]
00510df4: ldr r3, [pc, #0x1dc]
00510df8: sub sp, sp, #0xc
00510dfc: add sl, pc, sl
00510e00: ldr r3, [sl, r3]
00510e04: ldr sb, [pc, #0x1d0]
00510e08: str r3, [sp, #4]
00510e0c: ldr r4, [r3, #8]
00510e10: ldr r3, [sp, #4]
00510e14: cmp r4, r3
00510e18: beq #0x510ef0
00510e1c: ldr r8, [r4, #0x30]
00510e20: add fp, r4, #0x28
00510e24: cmp fp, r8
00510e28: beq #0x510ec0
00510e2c: ldr r5, [r8, #0x30]
00510e30: add r7, r8, #0x28
00510e34: cmp r7, r5
00510e38: beq #0x510e90
00510e3c: ldr r6, [r5, #0x28]
00510e40: cmp r6, #0
00510e44: beq #0x510e64
00510e48: ldr r3, [sl, sb]
00510e4c: mov r0, r6
00510e50: add r3, r3, #8
00510e54: str r3, [r0], #8
00510e58: bl #0x318254
00510e5c: mov r0, r6
00510e60: bl #0x310440
00510e64: ldr r2, [r5, #0xc]
00510e68: cmp r2, #0
00510e6c: bne #0x510e78
00510e70: b #0x510f04
00510e74: mov r2, r3
00510e78: ldr r3, [r2, #8]
00510e7c: cmp r3, #0
00510e80: bne #0x510e74
00510e84: mov r5, r2
00510e88: cmp r7, r5
00510e8c: bne #0x510e3c
00510e90: ldr r1, [r8, #0xc]
00510e94: cmp r1, #0
00510e98: mov r2, r1
00510e9c: bne #0x510ea8
00510ea0: b #0x510f38
00510ea4: mov r2, r3
00510ea8: ldr r3, [r2, #8]
00510eac: cmp r3, #0
00510eb0: bne #0x510ea4
00510eb4: mov r8, r2
00510eb8: cmp fp, r8
00510ebc: bne #0x510e2c
00510ec0: ldr r2, [r4, #0xc]
00510ec4: cmp r2, #0
00510ec8: bne #0x510ed4
00510ecc: b #0x510f6c
00510ed0: mov r2, r3
00510ed4: ldr r3, [r2, #8]
00510ed8: cmp r3, #0
00510edc: bne #0x510ed0
00510ee0: mov r4, r2
00510ee4: ldr r3, [sp, #4]
00510ee8: cmp r4, r3
00510eec: bne #0x510e1c
00510ef0: ldr r3, [r4, #0x10]
00510ef4: cmp r3, #0
00510ef8: bne #0x510fb4
00510efc: add sp, sp, #0xc
00510f00: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00510f04: ldr r3, [r5, #4]
00510f08: ldr r1, [r3, #0xc]
00510f0c: cmp r5, r1
00510f10: bne #0x510f2c
00510f14: mov r5, r3
00510f18: ldr r3, [r3, #4]
00510f1c: ldr r2, [r3, #0xc]
00510f20: cmp r2, r5
00510f24: beq #0x510f14
00510f28: ldr r2, [r5, #0xc]
00510f2c: cmp r3, r2
00510f30: movne r5, r3
00510f34: b #0x510e34
00510f38: ldr r3, [r8, #4]
00510f3c: ldr r2, [r3, #0xc]
00510f40: cmp r8, r2
00510f44: bne #0x510f60
00510f48: mov r8, r3
00510f4c: ldr r3, [r3, #4]
00510f50: ldr r2, [r3, #0xc]
00510f54: cmp r2, r8
00510f58: beq #0x510f48
00510f5c: ldr r1, [r8, #0xc]
00510f60: cmp r3, r1
00510f64: movne r8, r3
00510f68: b #0x510e24
00510f6c: ldr r3, [r4, #4]
00510f70: ldr r2, [r3, #0xc]
00510f74: cmp r4, r2
00510f78: movne r2, r4
00510f7c: beq #0x510f88
00510f80: b #0x510fa0
00510f84: mov r3, r1
00510f88: ldr r1, [r3, #4]
00510f8c: ldr r2, [r1, #0xc]
00510f90: cmp r2, r3
00510f94: beq #0x510f84
00510f98: mov r2, r3
00510f9c: mov r3, r1
00510fa0: ldr r1, [r2, #0xc]
00510fa4: cmp r3, r1
00510fa8: movne r2, r3
00510fac: mov r4, r2
00510fb0: b #0x510ee4
00510fb4: mov r0, r4
00510fb8: ldr r1, [r4, #4]
00510fbc: bl #0x510d78
00510fc0: mov r3, #0
00510fc4: str r3, [r4, #0x10]
00510fc8: stmib r4, {r3, r4}
00510fcc: str r4, [r4, #0xc]
00510fd0: b #0x510efc
00510fd4: umaaleq r3, r8, r4, ip
00510fd8: andeq r2, r0, r8, ror #20
00510fdc: andeq r2, r0, r0, lsr r3

# 0x5124d4 _ZNSt3mapISsS_ISsS_ISsP8PropertySt4lessISsESaISt4pairIKSsS1_EEES3_SaIS4_IS5_S8_EEES3_SaIS4_IS5_SB_EEEixIPKcEERSB_RKT_.clone.2
005124d4: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005124d8: ldr r4, [pc, #0x184]
005124dc: ldr sb, [pc, #0x184]
005124e0: ldr fp, [pc, #0x184]
005124e4: add r4, pc, r4
005124e8: ldr r3, [r4, sb]
005124ec: ldr r7, [r4, fp]
005124f0: mov r6, r0
005124f4: ldr r3, [r3]
005124f8: sub sp, sp, #0x9c
005124fc: mov r0, r7
00512500: mov r1, r6
00512504: str r3, [sp, #0x94]
00512508: bl #0x511638
0051250c: cmp r0, r7
00512510: mov r5, r0
00512514: beq #0x5125ac
00512518: add r7, sp, #0x64
0051251c: ldr r1, [r6]
00512520: add r2, sp, #0x2c
00512524: mov r0, r7
00512528: bl #0x3140ec
0051252c: ldr r3, [sp, #0x78]
00512530: ldr r1, [r5, #0x24]
00512534: ldr r8, [r5, #0x20]
00512538: ldr sl, [sp, #0x74]
0051253c: mov r0, r3
00512540: rsb r8, r1, r8
00512544: rsb sl, r3, sl
00512548: cmp r8, sl
0051254c: movlt r2, r8
00512550: movge r2, sl
00512554: bl #0x30e5e0
00512558: cmp r0, #0
0051255c: mov r3, r5
00512560: bne #0x5125a0
00512564: cmp sl, r8
00512568: blt #0x5125a4
0051256c: mov r0, r7
00512570: str r3, [sp, #4]
00512574: bl #0x318254
00512578: ldr r3, [sp, #4]
0051257c: ldr r1, [r4, sb]
00512580: ldr r2, [sp, #0x94]
00512584: add r0, r3, #0x28
00512588: ldr r3, [r1]
0051258c: cmp r2, r3
00512590: bne #0x51259c
00512594: add sp, sp, #0x9c
00512598: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0051259c: bl #0x30e310
005125a0: bge #0x51256c
005125a4: mov r0, r7
005125a8: bl #0x318254
005125ac: add sl, sp, #0x7c
005125b0: add r2, sp, #0x30
005125b4: add r7, sp, #0x34
005125b8: mov r8, #0
005125bc: ldr r1, [r6]
005125c0: mov r0, sl
005125c4: add r6, sp, #0x98
005125c8: bl #0x3140ec
005125cc: strb r8, [r6, #-0x8c]!
005125d0: mov r1, sl
005125d4: mov r0, r7
005125d8: str r8, [sp, #0x10]
005125dc: str r6, [sp, #0x14]
005125e0: str r6, [sp, #0x18]
005125e4: str r8, [sp, #0x1c]
005125e8: bl #0x32b918
005125ec: mov r1, r6
005125f0: add r0, r7, #0x18
005125f4: bl #0x511390
005125f8: mov r3, r7
005125fc: ldr r1, [r4, fp]
00512600: add r0, sp, #0x24
00512604: add r2, sp, #0x28
00512608: str r5, [sp, #0x28]
0051260c: bl #0x511fd0
00512610: mov r0, r7
00512614: ldr r5, [sp, #0x24]
00512618: bl #0x510d20
0051261c: ldr r3, [sp, #0x1c]
00512620: cmp r3, r8
00512624: bne #0x512638
00512628: mov r0, sl
0051262c: bl #0x318254
00512630: mov r3, r5
00512634: b #0x51257c
00512638: mov r0, r6
0051263c: ldr r1, [sp, #0x10]
00512640: bl #0x510ce0
00512644: mov r0, sl
00512648: str r6, [sp, #0x18]
0051264c: str r8, [sp, #0x1c]
00512650: str r6, [sp, #0x14]
00512654: str r8, [sp, #0x10]
00512658: bl #0x318254
0051265c: mov r3, r5
00512660: b #0x51257c
00512664: subeq r2, r8, ip, lsr #11
00512668: andeq r4, r0, ip, lsr #1
0051266c: andeq r2, r0, r8, ror #20

# 0x5134c0 _ZN11PropertyMap14GetPropertyMapEv
005134c0: push {r4, lr}
005134c4: sub sp, sp, #8
005134c8: mov r4, r0
005134cc: bl #0x510b4c
005134d0: add r3, sp, #8
005134d4: str r0, [r3, #-4]!
005134d8: mov r0, r3
005134dc: bl #0x5124d4
005134e0: add r1, r4, #4
005134e4: bl #0x513318
005134e8: add sp, sp, #8
005134ec: pop {r4, pc}

# 0x5134f0 _ZN11PropertyMap19SavePropertiesToXMLEP12TiXmlElementPKc
005134f0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005134f4: ldr r3, [pc, #0x16c]
005134f8: ldr r4, [pc, #0x16c]
005134fc: sub sp, sp, #0x54
00513500: add r3, pc, r3
00513504: str r3, [sp, #4]
00513508: ldr r3, [r3, r4]
0051350c: cmp r2, #0
00513510: str r4, [sp, #8]
00513514: ldr r3, [r3]
00513518: mov r8, r0
0051351c: str r1, [sp, #0xc]
00513520: str r3, [sp, #0x4c]
00513524: movne r4, r2
00513528: beq #0x513658
0051352c: ldr ip, [sp, #0xc]
00513530: cmp ip, #0
00513534: beq #0x513600
00513538: mov r1, #0
0051353c: mov r0, #0x8c
00513540: bl #0x310570
00513544: mov r1, r4
00513548: mov sl, r0
0051354c: bl #0x51745c
00513550: mov r0, r8
00513554: bl #0x5134c0
00513558: ldr r4, [r0, #8]
0051355c: mov r7, r0
00513560: add r6, sp, #0x1c
00513564: add sb, sp, #0x18
00513568: add r5, sp, #0x34
0051356c: add fp, sp, #0x10
00513570: cmp r7, r4
00513574: beq #0x5135f4
00513578: ldr r3, [r4, #0x28]
0051357c: cmp r3, #0
00513580: beq #0x5135c8
00513584: str r3, [sp, #0x10]
00513588: str r8, [sp, #0x14]
0051358c: ldr r1, [r3, #0x1c]
00513590: mov r2, sb
00513594: mov r0, r6
00513598: bl #0x3140ec
0051359c: mov r0, r5
005135a0: mov r1, fp
005135a4: bl #0x510fe0
005135a8: mov r0, sl
005135ac: mov r1, r6
005135b0: mov r2, r5
005135b4: bl #0x516400
005135b8: mov r0, r5
005135bc: bl #0x318254
005135c0: mov r0, r6
005135c4: bl #0x318254
005135c8: ldr r2, [r4, #0xc]
005135cc: cmp r2, #0
005135d0: bne #0x5135dc
005135d4: b #0x513624
005135d8: mov r2, r3
005135dc: ldr r3, [r2, #8]
005135e0: cmp r3, #0
005135e4: bne #0x5135d8
005135e8: mov r4, r2
005135ec: cmp r7, r4
005135f0: bne #0x513578
005135f4: ldr r0, [sp, #0xc]
005135f8: mov r1, sl
005135fc: bl #0x515964
00513600: ldr r2, [sp, #4]
00513604: ldr r1, [sp, #8]
00513608: ldr r3, [r2, r1]
0051360c: ldr r2, [sp, #0x4c]
00513610: ldr r3, [r3]
00513614: cmp r2, r3
00513618: bne #0x513664
0051361c: add sp, sp, #0x54
00513620: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00513624: ldr r3, [r4, #4]
00513628: ldr r1, [r3, #0xc]
0051362c: cmp r4, r1
00513630: bne #0x51364c
00513634: mov r4, r3
00513638: ldr r3, [r3, #4]
0051363c: ldr r2, [r3, #0xc]
00513640: cmp r2, r4
00513644: beq #0x513634
00513648: ldr r2, [r4, #0xc]
0051364c: cmp r2, r3
00513650: movne r4, r3
00513654: b #0x513570
00513658: ldr r4, [pc, #0x10]
0051365c: add r4, pc, r4
00513660: b #0x51352c
00513664: bl #0x30e310
00513668: umaaleq r1, r8, r0, r5
0051366c: andeq r4, r0, ip, lsr #1
00513670: eorseq ip, sl, ip, lsl #24

# 0x513674 _ZN11PropertyMap14DumpPropertiesEv
00513674: push {r4, lr}
00513678: bl #0x5134c0
0051367c: ldr r3, [r0, #8]
00513680: cmp r0, r3
00513684: beq #0x5136b4
00513688: ldr r2, [r3, #0xc]
0051368c: cmp r2, #0
00513690: bne #0x51369c
00513694: b #0x5136b8
00513698: mov r2, r3
0051369c: ldr r3, [r2, #8]
005136a0: cmp r3, #0
005136a4: bne #0x513698
005136a8: mov r3, r2
005136ac: cmp r0, r3
005136b0: bne #0x513688
005136b4: pop {r4, pc}
005136b8: ldr r1, [r3, #4]
005136bc: ldr ip, [r1, #0xc]
005136c0: cmp r3, ip
005136c4: bne #0x5136e0
005136c8: mov r3, r1
005136cc: ldr r1, [r1, #4]
005136d0: ldr r2, [r1, #0xc]
005136d4: cmp r2, r3
005136d8: beq #0x5136c8
005136dc: ldr r2, [r3, #0xc]
005136e0: cmp r1, r2
005136e4: movne r3, r1
005136e8: b #0x513680

# 0x5136ec _ZN11PropertyMap21LoadDefaultPropertiesEv
005136ec: push {r4, r5, r6, r7, r8, sb, sl, lr}
005136f0: ldr r4, [pc, #0x108]
005136f4: ldr r6, [pc, #0x108]
005136f8: sub sp, sp, #0x20
005136fc: add r4, pc, r4
00513700: ldr r3, [r4, r6]
00513704: add sb, r0, #4
00513708: mov r8, r0
0051370c: ldr r3, [r3]
00513710: add r5, sp, #4
00513714: str r3, [sp, #0x1c]
00513718: bl #0x5134c0
0051371c: mov r1, sb
00513720: mov sl, r0
00513724: mov r0, r5
00513728: bl #0x32b918
0051372c: ldr r7, [sl, #8]
00513730: cmp sl, r7
00513734: beq #0x51378c
00513738: ldr r3, [r7, #0x28]
0051373c: cmp r3, #0
00513740: beq #0x513760
00513744: cmp r8, #0
00513748: beq #0x513760
0051374c: mov r0, r3
00513750: mov r1, r8
00513754: ldr r3, [r3]
00513758: mov lr, pc
0051375c: ldr pc, [r3, #0xc]
00513760: ldr r2, [r7, #0xc]
00513764: cmp r2, #0
00513768: bne #0x513774
0051376c: b #0x5137c8
00513770: mov r2, r3
00513774: ldr r3, [r2, #8]
00513778: cmp r3, #0
0051377c: bne #0x513770
00513780: mov r7, r2
00513784: cmp sl, r7
00513788: bne #0x513738
0051378c: cmp sb, r5
00513790: beq #0x5137a4
00513794: mov r0, sb
00513798: ldr r1, [sp, #0x18]
0051379c: ldr r2, [sp, #0x14]
005137a0: bl #0x3109e0
005137a4: mov r0, r5
005137a8: bl #0x318254
005137ac: ldr r3, [r4, r6]
005137b0: ldr r2, [sp, #0x1c]
005137b4: ldr r3, [r3]
005137b8: cmp r2, r3
005137bc: bne #0x5137fc
005137c0: add sp, sp, #0x20
005137c4: pop {r4, r5, r6, r7, r8, sb, sl, pc}
005137c8: ldr r3, [r7, #4]
005137cc: ldr r1, [r3, #0xc]
005137d0: cmp r7, r1
005137d4: bne #0x5137f0
005137d8: mov r7, r3
005137dc: ldr r3, [r3, #4]
005137e0: ldr r2, [r3, #0xc]
005137e4: cmp r2, r7
005137e8: beq #0x5137d8
005137ec: ldr r2, [r7, #0xc]
005137f0: cmp r2, r3
005137f4: movne r7, r3
005137f8: b #0x513730
005137fc: bl #0x30e310
00513800: umaaleq r1, r8, r4, r3
00513804: andeq r4, r0, ip, lsr #1

# 0x513808 _ZN11PropertyMap7GetPropEPKc
00513808: push {r4, lr}
0051380c: sub sp, sp, #8
00513810: add r4, sp, #8
00513814: str r1, [r4, #-4]!
00513818: bl #0x5134c0
0051381c: mov r1, r4
00513820: bl #0x512ce0
00513824: ldr r0, [r0]
00513828: add sp, sp, #8
0051382c: pop {r4, pc}

# 0x513830 _ZN11PropertyMap20SetTemplateParameterEPKcS1_
00513830: push {r4, lr}
00513834: mov r4, r2
00513838: bl #0x513808
0051383c: subs r3, r0, #0
00513840: beq #0x513854
00513844: ldr r3, [r3]
00513848: mov r1, r4
0051384c: mov lr, pc
00513850: ldr pc, [r3, #0x10]
00513854: pop {r4, pc}

# 0x513858 _ZN11PropertyMap11GetPropertyEPKc
00513858: push {r4, r5, r6, lr}
0051385c: mov r5, r1
00513860: mov r4, r0
00513864: mov r1, r2
00513868: mov r0, r5
0051386c: bl #0x513808
00513870: stm r4, {r0, r5}
00513874: mov r0, r4
00513878: pop {r4, r5, r6, pc}

# 0x51387c _ZN11PropertyMap11SetPropertyEPKcS1_
0051387c: push {r4, lr}
00513880: mov r3, r1
00513884: sub sp, sp, #8
00513888: mov r1, r0
0051388c: mov r4, r2
00513890: mov r0, sp
00513894: mov r2, r3
00513898: bl #0x513858
0051389c: cmp r4, #0
005138a0: ldr r3, [sp]
005138a4: ldr r1, [sp, #4]
005138a8: beq #0x5138d8
005138ac: cmp r1, #0
005138b0: beq #0x5138d0
005138b4: cmp r3, #0
005138b8: beq #0x5138d0
005138bc: mov r0, r3
005138c0: mov r2, r4
005138c4: ldr r3, [r3]
005138c8: mov lr, pc
005138cc: ldr pc, [r3, #4]
005138d0: add sp, sp, #8
005138d4: pop {r4, pc}
005138d8: cmp r1, #0
005138dc: beq #0x5138d0
005138e0: cmp r3, #0
005138e4: beq #0x5138d0
005138e8: mov r0, r3
005138ec: ldr r3, [r3]
005138f0: mov lr, pc
005138f4: ldr pc, [r3, #0xc]
005138f8: b #0x5138d0

# 0x5138fc _ZN11PropertyMap15ClonePropertiesERS_
005138fc: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00513900: ldr sb, [pc, #0xf0]
00513904: ldr fp, [pc, #0xf0]
00513908: sub sp, sp, #0x24
0051390c: add sb, pc, sb
00513910: ldr r3, [sb, fp]
00513914: mov sl, r0
00513918: mov r0, r1
0051391c: ldr r3, [r3]
00513920: mov r8, r1
00513924: add r5, sp, #4
00513928: str r3, [sp, #0x1c]
0051392c: bl #0x5134c0
00513930: ldr r4, [r0, #8]
00513934: mov r7, r0
00513938: cmp r7, r4
0051393c: beq #0x5139a4
00513940: ldr r3, [r4, #0x28]
00513944: ldr r6, [r4, #0x24]
00513948: mov r2, r8
0051394c: mov r1, r3
00513950: mov r0, r5
00513954: ldr r3, [r3]
00513958: mov lr, pc
0051395c: ldr pc, [r3]
00513960: ldr r2, [sp, #0x18]
00513964: mov r0, sl
00513968: mov r1, r6
0051396c: bl #0x51387c
00513970: mov r0, r5
00513974: bl #0x318254
00513978: ldr r2, [r4, #0xc]
0051397c: cmp r2, #0
00513980: bne #0x51398c
00513984: b #0x5139c0
00513988: mov r2, r3
0051398c: ldr r3, [r2, #8]
00513990: cmp r3, #0
00513994: bne #0x513988
00513998: mov r4, r2
0051399c: cmp r7, r4
005139a0: bne #0x513940
005139a4: ldr r3, [sb, fp]
005139a8: ldr r2, [sp, #0x1c]
005139ac: ldr r3, [r3]
005139b0: cmp r2, r3
005139b4: bne #0x5139f4
005139b8: add sp, sp, #0x24
005139bc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005139c0: ldr r3, [r4, #4]
005139c4: ldr r1, [r3, #0xc]
005139c8: cmp r4, r1
005139cc: bne #0x5139e8
005139d0: mov r4, r3
005139d4: ldr r3, [r3, #4]
005139d8: ldr r2, [r3, #0xc]
005139dc: cmp r2, r4
005139e0: beq #0x5139d0
005139e4: ldr r2, [r4, #0xc]
005139e8: cmp r2, r3
005139ec: movne r4, r3
005139f0: b #0x513938
005139f4: bl #0x30e310
005139f8: subeq r1, r8, r4, lsl #3
005139fc: andeq r4, r0, ip, lsr #1

# 0x513a00 _ZN11PropertyMap20LoadOverridesFromXMLEP12TiXmlElement
00513a00: push {r4, r5, r6, r7, r8, lr}
00513a04: subs r7, r1, #0
00513a08: mov r6, r0
00513a0c: beq #0x513a70
00513a10: bl #0x5134c0
00513a14: ldr r4, [r0, #8]
00513a18: mov r5, r0
00513a1c: cmp r5, r4
00513a20: beq #0x513a70
00513a24: ldr r8, [r4, #0x24]
00513a28: mov r0, r7
00513a2c: mov r1, r8
00513a30: bl #0x514c70
00513a34: mov r1, r8
00513a38: mov r2, r0
00513a3c: mov r0, r6
00513a40: bl #0x51387c
00513a44: ldr r2, [r4, #0xc]
00513a48: cmp r2, #0
00513a4c: bne #0x513a58
00513a50: b #0x513a74
00513a54: mov r2, r3
00513a58: ldr r3, [r2, #8]
00513a5c: cmp r3, #0
00513a60: bne #0x513a54
00513a64: mov r4, r2
00513a68: cmp r5, r4
00513a6c: bne #0x513a24
00513a70: pop {r4, r5, r6, r7, r8, pc}
00513a74: ldr r3, [r4, #4]
00513a78: ldr r1, [r3, #0xc]
00513a7c: cmp r4, r1
00513a80: bne #0x513a9c
00513a84: mov r4, r3
00513a88: ldr r3, [r3, #4]
00513a8c: ldr r2, [r3, #0xc]
00513a90: cmp r2, r4
00513a94: beq #0x513a84
00513a98: ldr r2, [r4, #0xc]
00513a9c: cmp r2, r3
00513aa0: movne r4, r3
00513aa4: b #0x513a1c

# 0x513aa8 _ZNSt3mapISsS_ISsP8PropertySt4lessISsESaISt4pairIKSsS1_EEES3_SaIS4_IS5_S8_EEEixIA1_cEERS8_RKT_.clone.3
00513aa8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00513aac: ldr r5, [pc, #0x21c]
00513ab0: ldr r2, [pc, #0x21c]
00513ab4: sub sp, sp, #0xbc
00513ab8: add r5, pc, r5
00513abc: ldr r3, [r5, r2]
00513ac0: str r2, [sp, #8]
00513ac4: ldr r6, [r0, #4]
00513ac8: ldr r3, [r3]
00513acc: mov sl, r0
00513ad0: cmp r6, #0
00513ad4: str r3, [sp, #0xb4]
00513ad8: moveq r4, r0
00513adc: beq #0x513b78
00513ae0: ldr fp, [pc, #0x1f0]
00513ae4: add r3, sp, #0x30
00513ae8: mov r4, r0
00513aec: add r7, sp, #0x6c
00513af0: add fp, pc, fp
00513af4: str r3, [sp, #0xc]
00513af8: mov r1, fp
00513afc: ldr r2, [sp, #0xc]
00513b00: mov r0, r7
00513b04: bl #0x3140ec
00513b08: ldr r3, [r6, #0x24]
00513b0c: ldr r1, [sp, #0x80]
00513b10: ldr r8, [r6, #0x20]
00513b14: ldr sb, [sp, #0x7c]
00513b18: mov r0, r3
00513b1c: rsb r8, r3, r8
00513b20: rsb sb, r1, sb
00513b24: cmp sb, r8
00513b28: movlt r2, sb
00513b2c: movge r2, r8
00513b30: bl #0x30e5e0
00513b34: subs r3, r0, #0
00513b38: bne #0x513b50
00513b3c: cmp r8, sb
00513b40: mvnlt r3, #0
00513b44: blt #0x513b50
00513b48: movle r3, #0
00513b4c: movgt r3, #1
00513b50: mov r0, r7
00513b54: str r3, [sp, #4]
00513b58: bl #0x318254
00513b5c: ldr r3, [sp, #4]
00513b60: cmp r3, #0
00513b64: movge r4, r6
00513b68: ldrlt r6, [r6, #0xc]
00513b6c: ldrge r6, [r6, #8]
00513b70: cmp r6, #0
00513b74: bne #0x513af8
00513b78: cmp sl, r4
00513b7c: beq #0x513c14
00513b80: ldr r1, [pc, #0x154]
00513b84: add r6, sp, #0x84
00513b88: add r2, sp, #0x34
00513b8c: add r1, pc, r1
00513b90: mov r0, r6
00513b94: bl #0x3140ec
00513b98: ldr r3, [sp, #0x98]
00513b9c: ldr r1, [r4, #0x24]
00513ba0: ldr r7, [r4, #0x20]
00513ba4: ldr r8, [sp, #0x94]
00513ba8: mov r0, r3
00513bac: rsb r7, r1, r7
00513bb0: rsb r8, r3, r8
00513bb4: cmp r7, r8
00513bb8: movlt r2, r7
00513bbc: movge r2, r8
00513bc0: bl #0x30e5e0
00513bc4: cmp r0, #0
00513bc8: mov sb, r4
00513bcc: bne #0x513c08
00513bd0: cmp r8, r7
00513bd4: blt #0x513c0c
00513bd8: mov r0, r6
00513bdc: bl #0x318254
00513be0: ldr r2, [sp, #8]
00513be4: add r0, sb, #0x28
00513be8: ldr r3, [r5, r2]
00513bec: ldr r2, [sp, #0xb4]
00513bf0: ldr r3, [r3]
00513bf4: cmp r2, r3
00513bf8: bne #0x513c04
00513bfc: add sp, sp, #0xbc
00513c00: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00513c04: bl #0x30e310
00513c08: bge #0x513bd8
00513c0c: mov r0, r6
00513c10: bl #0x318254
00513c14: ldr r1, [pc, #0xc4]
00513c18: add sb, sp, #0x9c
00513c1c: add r2, sp, #0x38
00513c20: add r6, sp, #0xb8
00513c24: add r7, sp, #0x3c
00513c28: mov r8, #0
00513c2c: add r1, pc, r1
00513c30: mov r0, sb
00513c34: bl #0x3140ec
00513c38: strb r8, [r6, #-0xa8]!
00513c3c: mov r1, sb
00513c40: mov r0, r7
00513c44: str r8, [sp, #0x14]
00513c48: str r6, [sp, #0x18]
00513c4c: str r6, [sp, #0x1c]
00513c50: str r8, [sp, #0x20]
00513c54: bl #0x32b918
00513c58: mov r1, r6
00513c5c: add r0, r7, #0x18
00513c60: bl #0x511130
00513c64: mov r3, r7
00513c68: mov r1, sl
00513c6c: add r0, sp, #0x28
00513c70: add r2, sp, #0x2c
00513c74: str r4, [sp, #0x2c]
00513c78: bl #0x512e14
00513c7c: mov r0, r7
00513c80: ldr r4, [sp, #0x28]
00513c84: bl #0x510c98
00513c88: ldr r3, [sp, #0x20]
00513c8c: cmp r3, r8
00513c90: bne #0x513ca4
00513c94: mov r0, sb
00513c98: bl #0x318254
00513c9c: mov sb, r4
00513ca0: b #0x513be0
00513ca4: mov r0, r6
00513ca8: ldr r1, [sp, #0x14]
00513cac: bl #0x510c58
00513cb0: mov r0, sb
00513cb4: str r6, [sp, #0x1c]
00513cb8: str r8, [sp, #0x20]
00513cbc: str r6, [sp, #0x18]
00513cc0: str r8, [sp, #0x14]
00513cc4: mov sb, r4
00513cc8: bl #0x318254
00513ccc: b #0x513be0
00513cd0: ldrdeq r0, r1, [r8], #-0xf8
00513cd4: andeq r4, r0, ip, lsr #1
00513cd8: eorseq r7, fp, r8, lsl sp
00513cdc: eorseq r7, fp, ip, ror ip
00513ce0: ldrsbteq r7, [fp], -ip

# 0x513ce4 _ZN11PropertyMap11AddPropertyEPKcP8Property
00513ce4: push {r4, r5, r6, r7, r8, lr}
00513ce8: sub sp, sp, #0x10
00513cec: str r1, [sp, #4]
00513cf0: mov r7, r2
00513cf4: bl #0x510b4c
00513cf8: add r3, sp, #0x10
00513cfc: str r0, [r3, #-4]!
00513d00: mov r0, r3
00513d04: bl #0x5124d4
00513d08: bl #0x513aa8
00513d0c: add r5, sp, #4
00513d10: mov r1, r5
00513d14: mov r6, r0
00513d18: bl #0x511e60
00513d1c: ldr r4, [pc, #0x4c]
00513d20: cmp r0, r6
00513d24: add r4, pc, r4
00513d28: beq #0x513d58
00513d2c: ldr r8, [r0, #0x28]
00513d30: cmp r8, #0
00513d34: beq #0x513d58
00513d38: ldr r3, [pc, #0x34]
00513d3c: mov r0, r8
00513d40: ldr r3, [r4, r3]
00513d44: add r3, r3, #8
00513d48: str r3, [r0], #8
00513d4c: bl #0x318254
00513d50: mov r0, r8
00513d54: bl #0x310440
00513d58: mov r0, r6
00513d5c: mov r1, r5
00513d60: bl #0x512ce0
00513d64: str r7, [r0]
00513d68: add sp, sp, #0x10
00513d6c: pop {r4, r5, r6, r7, r8, pc}
00513d70: subeq r0, r8, ip, ror #26
00513d74: andeq r2, r0, r0, lsr r3

# 0x513d78 _ZN11PropertyMap14InitPropertiesEv
00513d78: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00513d7c: ldr fp, [pc, #0x250]
00513d80: ldr r2, [pc, #0x250]
00513d84: ldr r3, [pc, #0x250]
00513d88: sub sp, sp, #0x84
00513d8c: add fp, pc, fp
00513d90: str r3, [sp, #4]
00513d94: ldr r3, [fp, r2]
00513d98: str r2, [sp, #8]
00513d9c: str r0, [sp, #0xc]
00513da0: ldr r3, [r3]
00513da4: str r3, [sp, #0x7c]
00513da8: bl #0x510b4c
00513dac: ldr r2, [sp, #4]
00513db0: mov sl, r0
00513db4: ldr r8, [fp, r2]
00513db8: ldr r4, [r8, #4]
00513dbc: cmp r4, #0
00513dc0: beq #0x513ee0
00513dc4: add r7, sp, #0x4c
00513dc8: add sb, sp, #0x18
00513dcc: b #0x513dd8
00513dd0: mov r8, r4
00513dd4: mov r4, r3
00513dd8: mov r1, sl
00513ddc: mov r2, sb
00513de0: mov r0, r7
00513de4: bl #0x3140ec
00513de8: ldr r3, [r4, #0x24]
00513dec: ldr r1, [sp, #0x60]
00513df0: ldr r6, [r4, #0x20]
00513df4: ldr r5, [sp, #0x5c]
00513df8: mov r0, r3
00513dfc: rsb r6, r3, r6
00513e00: rsb r5, r1, r5
00513e04: cmp r5, r6
00513e08: movlt r2, r5
00513e0c: movge r2, r6
00513e10: bl #0x30e5e0
00513e14: subs r3, r0, #0
00513e18: bne #0x513e30
00513e1c: cmp r6, r5
00513e20: mvnlt r3, #0
00513e24: blt #0x513e30
00513e28: movle r3, #0
00513e2c: movgt r3, #1
00513e30: mov r0, r7
00513e34: str r3, [sp]
00513e38: bl #0x318254
00513e3c: ldr r3, [sp]
00513e40: cmp r3, #0
00513e44: ldrlt r3, [r4, #0xc]
00513e48: ldrge r3, [r4, #8]
00513e4c: movlt r4, r8
00513e50: cmp r3, #0
00513e54: bne #0x513dd0
00513e58: ldr r2, [sp, #4]
00513e5c: mov r8, r4
00513e60: ldr r3, [fp, r2]
00513e64: cmp r4, r3
00513e68: beq #0x513ee0
00513e6c: add r5, sp, #0x34
00513e70: mov r1, sl
00513e74: add r2, sp, #0x14
00513e78: mov r0, r5
00513e7c: bl #0x3140ec
00513e80: ldr r3, [sp, #0x48]
00513e84: ldr r1, [r4, #0x24]
00513e88: ldr r6, [r4, #0x20]
00513e8c: ldr r7, [sp, #0x44]
00513e90: mov r0, r3
00513e94: rsb r6, r1, r6
00513e98: rsb r7, r3, r7
00513e9c: cmp r6, r7
00513ea0: movlt r2, r6
00513ea4: movge r2, r7
00513ea8: bl #0x30e5e0
00513eac: subs r8, r0, #0
00513eb0: bne #0x513ec8
00513eb4: cmp r7, r6
00513eb8: mvnlt r8, #0
00513ebc: blt #0x513ec8
00513ec0: movle r8, #0
00513ec4: movgt r8, #1
00513ec8: mov r0, r5
00513ecc: bl #0x318254
00513ed0: cmp r8, #0
00513ed4: ldrlt r3, [sp, #4]
00513ed8: movge r8, r4
00513edc: ldrlt r8, [fp, r3]
00513ee0: ldr r2, [sp, #4]
00513ee4: ldr r3, [fp, r2]
00513ee8: cmp r8, r3
00513eec: beq #0x513f10
00513ef0: ldr r2, [sp, #8]
00513ef4: ldr r3, [fp, r2]
00513ef8: ldr r2, [sp, #0x7c]
00513efc: ldr r3, [r3]
00513f00: cmp r2, r3
00513f04: bne #0x513fd0
00513f08: add sp, sp, #0x84
00513f0c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00513f10: add r4, sp, #0x64
00513f14: mov r0, r4
00513f18: mov r1, #0x10
00513f1c: str r4, [sp, #0x74]
00513f20: str r4, [sp, #0x78]
00513f24: bl #0x31167c
00513f28: ldr r3, [sp, #0x74]
00513f2c: add r7, sp, #0x1c
00513f30: mov r5, #0
00513f34: strb r5, [r3]
00513f38: mov r1, r4
00513f3c: mov r0, r7
00513f40: bl #0x32b918
00513f44: mov r1, r5
00513f48: mov r0, #0x38
00513f4c: bl #0x310570
00513f50: ldr r3, [pc, #0x88]
00513f54: ldr r6, [pc, #0x88]
00513f58: mov r5, r0
00513f5c: ldr r3, [fp, r3]
00513f60: add r6, pc, r6
00513f64: mov r1, r6
00513f68: add r3, r3, #8
00513f6c: add r2, sp, #0x10
00513f70: str r3, [r0], #8
00513f74: bl #0x3140ec
00513f78: ldr r3, [pc, #0x68]
00513f7c: mov r0, r5
00513f80: mov r2, #4
00513f84: ldr r3, [fp, r3]
00513f88: str r2, [r5, #4]
00513f8c: mov r1, r7
00513f90: add r3, r3, #8
00513f94: str r3, [r0], #0x20
00513f98: bl #0x32b918
00513f9c: mov r1, r6
00513fa0: mov r2, r5
00513fa4: ldr r0, [sp, #0xc]
00513fa8: bl #0x513ce4
00513fac: mov r0, r7
00513fb0: bl #0x318254
00513fb4: mov r0, r4
00513fb8: bl #0x318254
00513fbc: ldr r0, [sp, #0xc]
00513fc0: ldr r3, [r0]
00513fc4: mov lr, pc
00513fc8: ldr pc, [r3]
00513fcc: b #0x513ef0
00513fd0: bl #0x30e310
00513fd4: subeq r0, r8, r4, lsl #26
00513fd8: andeq r4, r0, ip, lsr #1
00513fdc: andeq r2, r0, r8, ror #20
00513fe0: andeq r2, r0, r0, lsr r3
00513fe4: eorseq ip, sl, r0, lsl #10
00513fe8: muleq r0, r4, r4

# 0x513fec _ZN11PropertyMap11SetTemplateERKSs
00513fec: push {r4, r5, r6, r7, r8, sl, lr}
00513ff0: ldr r3, [r1, #0x14]
00513ff4: ldr r2, [r1, #0x10]
00513ff8: ldr r7, [pc, #0x164]
00513ffc: sub sp, sp, #0x14
00514000: cmp r3, r2
00514004: mov r4, r0
00514008: add r7, pc, r7
0051400c: beq #0x514040
00514010: add r0, r0, #4
00514014: cmp r1, r0
00514018: beq #0x514024
0051401c: mov r1, r3
00514020: bl #0x3109e0
00514024: mov r0, r4
00514028: bl #0x5134c0
0051402c: ldr r8, [r0, #0x10]
00514030: cmp r8, #0
00514034: beq #0x514048
00514038: mov r0, r4
0051403c: bl #0x51419c
00514040: add sp, sp, #0x14
00514044: pop {r4, r5, r6, r7, r8, sl, pc}
00514048: mov r0, r4
0051404c: bl #0x510b4c
00514050: add r3, sp, #0x10
00514054: str r0, [r3, #-4]!
00514058: mov r0, r3
0051405c: bl #0x5124d4
00514060: bl #0x513aa8
00514064: mov r5, r0
00514068: mov r0, r4
0051406c: bl #0x5134c0
00514070: ldr r3, [r0, #0x10]
00514074: mov r6, r0
00514078: cmp r3, #0
0051407c: bne #0x514110
00514080: ldr r7, [r5, #8]
00514084: cmp r5, r7
00514088: beq #0x514038
0051408c: add r1, r7, #0x10
00514090: mov r0, r6
00514094: ldr r8, [r7, #0x28]
00514098: bl #0x512b74
0051409c: ldr r3, [r8]
005140a0: mov sl, r0
005140a4: mov r0, r8
005140a8: mov lr, pc
005140ac: ldr pc, [r3, #0x14]
005140b0: str r0, [sl]
005140b4: ldr r2, [r7, #0xc]
005140b8: cmp r2, #0
005140bc: bne #0x5140c8
005140c0: b #0x5140dc
005140c4: mov r2, r3
005140c8: ldr r3, [r2, #8]
005140cc: cmp r3, #0
005140d0: bne #0x5140c4
005140d4: mov r7, r2
005140d8: b #0x514084
005140dc: ldr r3, [r7, #4]
005140e0: ldr r1, [r3, #0xc]
005140e4: cmp r7, r1
005140e8: bne #0x514104
005140ec: mov r7, r3
005140f0: ldr r3, [r3, #4]
005140f4: ldr r2, [r3, #0xc]
005140f8: cmp r2, r7
005140fc: beq #0x5140ec
00514100: ldr r2, [r7, #0xc]
00514104: cmp r2, r3
00514108: movne r7, r3
0051410c: b #0x514084
00514110: ldr r3, [pc, #0x50]
00514114: ldr r3, [r7, r3]
00514118: ldr r3, [r3]
0051411c: cmp r3, #2
00514120: streq r8, [r8]
00514124: beq #0x514080
00514128: cmp r3, #1
0051412c: bne #0x514080
00514130: ldr r0, [pc, #0x34]
00514134: ldr r1, [pc, #0x34]
00514138: ldr r2, [pc, #0x34]
0051413c: ldr r0, [r7, r0]
00514140: ldr r3, [pc, #0x30]
00514144: mov ip, #0x70
00514148: add r1, pc, r1
0051414c: add r2, pc, r2
00514150: add r3, pc, r3
00514154: add r0, r0, #0xa8
00514158: str ip, [sp]
0051415c: bl #0x30e004
00514160: b #0x514080
00514164: subeq r0, r8, r8, lsl #21
00514168: andeq r3, r0, r0, asr #19
0051416c: andeq r1, r0, r0, asr #19
00514170: mlaseq sl, r0, r2, sl
00514174: ldrshteq r7, [ip], -ip
00514178: eorseq r7, ip, r8, lsr #29

# 0x51419c _ZN11PropertyMap12LoadTemplateEv
0051419c: ldr r3, [pc, #0x68]
005141a0: ldr r2, [pc, #0x68]
005141a4: str lr, [sp, #-4]!
005141a8: add r3, pc, r3
005141ac: ldr r2, [r3, r2]
005141b0: sub sp, sp, #0xc
005141b4: ldr r2, [r2]
005141b8: cmp r2, #2
005141bc: moveq r3, #0
005141c0: streq r3, [r3]
005141c4: beq #0x5141d0
005141c8: cmp r2, #1
005141cc: beq #0x5141d8
005141d0: add sp, sp, #0xc
005141d4: ldm sp!, {pc}
005141d8: ldr r0, [pc, #0x34]
005141dc: ldr r1, [pc, #0x34]
005141e0: ldr r2, [pc, #0x34]
005141e4: ldr r0, [r3, r0]
005141e8: ldr r3, [pc, #0x30]
005141ec: mov ip, #9
005141f0: add r1, pc, r1
005141f4: add r2, pc, r2
005141f8: add r3, pc, r3
005141fc: add r0, r0, #0xa8
00514200: str ip, [sp]
00514204: bl #0x30e004
00514208: b #0x5141d0
0051420c: subeq r0, r8, r8, ror #17
00514210: andeq r3, r0, r0, asr #19
00514214: andeq r1, r0, r0, asr #19
00514218: eorseq sl, sl, r8, ror #3
0051421c: eorseq sl, sl, r4, ror r3
00514220: eorseq r7, ip, r8, ror #28

# 0x524644 _ZN8PFObjectC1Ev
00524644: ldr r3, [pc, #0xf4]
00524648: ldr r1, [pc, #0xf4]
0052464c: push {r4, r5, r6, r7, r8, lr}
00524650: add r3, pc, r3
00524654: ldr lr, [r3, r1]
00524658: mov r1, #8
0052465c: str r1, [r0, #4]
00524660: mov r2, #0
00524664: mov ip, #0
00524668: mov r5, #0x3f800000
0052466c: mov r1, #2
00524670: str ip, [r0]
00524674: str ip, [r0, #0xc]
00524678: str ip, [r0, #0x10]
0052467c: str r2, [r0, #0x18]
00524680: str r2, [r0, #0x1c]
00524684: str r2, [r0, #0x20]
00524688: str r1, [r0, #0x14]
0052468c: str r5, [r0, #8]
00524690: ldr r6, [lr]
00524694: mov r4, r0
00524698: ldr r0, [pc, #0xa8]
0052469c: str r6, [r4, #0x24]
005246a0: ldr r6, [lr, #4]
005246a4: ldr r0, [r3, r0]
005246a8: ldr r1, [pc, #0x9c]
005246ac: str r6, [r4, #0x28]
005246b0: ldr r7, [lr, #8]
005246b4: add r6, r4, #0x38
005246b8: add lr, r4, #0x8c
005246bc: add r0, r0, #8
005246c0: add r1, pc, r1
005246c4: str r0, [r4, #0x4c]
005246c8: str r2, [r4, #0x34]
005246cc: str r2, [r4, #0x40]
005246d0: str r2, [r4, #0x44]
005246d4: str r2, [r4, #0x48]
005246d8: str ip, [r4, #0x50]
005246dc: str ip, [r4, #0x54]
005246e0: str r2, [r4, #0x5c]
005246e4: str r2, [r4, #0x60]
005246e8: str r2, [r4, #0x64]
005246ec: str r2, [r4, #0x68]
005246f0: mov r0, lr
005246f4: str r7, [r4, #0x2c]
005246f8: str r6, [r4, #0x3c]
005246fc: str r5, [r4, #0x58]
00524700: str r5, [r4, #0x30]
00524704: str r6, [r4, #0x38]
00524708: add r1, r1, #1
0052470c: str r2, [r4, #0x6c]
00524710: str r2, [r4, #0x70]
00524714: str ip, [r4, #0x7c]
00524718: str r2, [r4, #0x88]
0052471c: str r2, [r4, #0x74]
00524720: str r2, [r4, #0x78]
00524724: str r2, [r4, #0x80]
00524728: str r2, [r4, #0x84]
0052472c: str lr, [r4, #0x9c]
00524730: str lr, [r4, #0xa0]
00524734: bl #0x5244e8
00524738: mov r0, r4
0052473c: pop {r4, r5, r6, r7, r8, pc}
00524740: subeq r0, r7, r0, asr #8
00524744: andeq r4, r0, r0, asr #6
00524748: andeq r1, r0, r8, ror r2
0052474c: eorseq lr, ip, r0, lsr #6
