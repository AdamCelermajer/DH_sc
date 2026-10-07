# 0x30f010 _Z8StrToObjPKcRSt6vectorI7Point3DIfESaIS3_EE
0030f010: ldr r0, [pc, #4]
0030f014: add r0, pc, r0
0030f018: b #0x324114
0030f01c: subseq pc, sl, ip, ror #6

# 0x30f020 _Z8StrToObjPKcRb
0030f020: push {r4, lr}
0030f024: mov r4, r1
0030f028: bl #0x30e094
0030f02c: subs r0, r0, #0
0030f030: movne r0, #1
0030f034: strb r0, [r4]
0030f038: pop {r4, pc}

# 0x30f03c _Z8StrToObjPKcRi
0030f03c: push {r4, lr}
0030f040: mov r4, r1
0030f044: bl #0x30e094
0030f048: str r0, [r4]
0030f04c: pop {r4, pc}

# 0x30f050 _Z8StrToObjPKcR7Point2DIiE
0030f050: push {r4, r5, r6, r7, lr}
0030f054: sub sp, sp, #0xc
0030f058: mov r3, #0x2c
0030f05c: mov r7, r0
0030f060: add r4, sp, #8
0030f064: mov r0, #0x100
0030f068: strh r3, [r4, #-4]!
0030f06c: mov r6, r1
0030f070: bl #0x310454
0030f074: mov r1, r7
0030f078: mov r5, r0
0030f07c: bl #0x30e520
0030f080: mov r0, r5
0030f084: mov r1, r4
0030f088: bl #0x30e028
0030f08c: cmp r0, #0
0030f090: beq #0x30f09c
0030f094: bl #0x30e094
0030f098: str r0, [r6]
0030f09c: mov r1, r4
0030f0a0: mov r0, #0
0030f0a4: bl #0x30e028
0030f0a8: cmp r0, #0
0030f0ac: beq #0x30f0b8
0030f0b0: bl #0x30e094
0030f0b4: str r0, [r6, #4]
0030f0b8: mov r0, r5
0030f0bc: bl #0x310440
0030f0c0: add sp, sp, #0xc
0030f0c4: pop {r4, r5, r6, r7, pc}

# 0x30f0c8 _Z8StrToObjPKcRN6glitch4core11dimension2dIiEE
0030f0c8: push {r4, r5, r6, r7, r8, lr}
0030f0cc: sub sp, sp, #8
0030f0d0: mov r6, #0
0030f0d4: mov r3, #0x2c
0030f0d8: add r4, sp, #8
0030f0dc: strh r3, [r4, #-4]!
0030f0e0: mov r5, r1
0030f0e4: mov r1, r0
0030f0e8: mov r0, r6
0030f0ec: bl #0x30e520
0030f0f0: mov r0, r6
0030f0f4: mov r1, r4
0030f0f8: bl #0x30e028
0030f0fc: cmp r0, r6
0030f100: beq #0x30f10c
0030f104: bl #0x30e094
0030f108: mov r8, r0
0030f10c: mov r1, r4
0030f110: mov r0, #0
0030f114: bl #0x30e028
0030f118: cmp r0, #0
0030f11c: beq #0x30f128
0030f120: bl #0x30e094
0030f124: mov r7, r0
0030f128: mov r0, #0
0030f12c: bl #0x310440
0030f130: str r7, [r5, #4]
0030f134: str r8, [r5]
0030f138: add sp, sp, #8
0030f13c: pop {r4, r5, r6, r7, r8, pc}

# 0x30f140 _Z8StrToObjPKcR7Point3DIiE
0030f140: push {r4, r5, r6, r7, lr}
0030f144: sub sp, sp, #0xc
0030f148: mov r3, #0x2c
0030f14c: mov r7, r0
0030f150: add r4, sp, #8
0030f154: mov r0, #0x100
0030f158: strh r3, [r4, #-4]!
0030f15c: mov r5, r1
0030f160: bl #0x310454
0030f164: mov r1, r7
0030f168: mov r6, r0
0030f16c: bl #0x30e520
0030f170: mov r0, r6
0030f174: mov r1, r4
0030f178: bl #0x30e028
0030f17c: cmp r0, #0
0030f180: beq #0x30f194
0030f184: mov r1, #0
0030f188: bl #0x30e6ac
0030f18c: bl #0x30ea24
0030f190: str r0, [r5]
0030f194: mov r0, #0
0030f198: mov r1, r4
0030f19c: bl #0x30e028
0030f1a0: cmp r0, #0
0030f1a4: beq #0x30f1b0
0030f1a8: bl #0x30e094
0030f1ac: str r0, [r5, #4]
0030f1b0: mov r1, r4
0030f1b4: mov r0, #0
0030f1b8: bl #0x30e028
0030f1bc: cmp r0, #0
0030f1c0: beq #0x30f1cc
0030f1c4: bl #0x30e094
0030f1c8: str r0, [r5, #8]
0030f1cc: mov r0, r6
0030f1d0: bl #0x310440
0030f1d4: add sp, sp, #0xc
0030f1d8: pop {r4, r5, r6, r7, pc}

# 0x30f1dc _Z8StrToObjPKcR7Point3DIfE
0030f1dc: push {r4, r5, r6, r7, lr}
0030f1e0: sub sp, sp, #0xc
0030f1e4: mov r3, #0x2c
0030f1e8: mov r7, r0
0030f1ec: add r4, sp, #8
0030f1f0: mov r0, #0x100
0030f1f4: strh r3, [r4, #-4]!
0030f1f8: mov r5, r1
0030f1fc: bl #0x310454
0030f200: mov r1, r7
0030f204: mov r6, r0
0030f208: bl #0x30e520
0030f20c: mov r0, r6
0030f210: mov r1, r4
0030f214: bl #0x30e028
0030f218: cmp r0, #0
0030f21c: beq #0x30f230
0030f220: mov r1, #0
0030f224: bl #0x30e6ac
0030f228: bl #0x30e6a0
0030f22c: str r0, [r5]
0030f230: mov r0, #0
0030f234: mov r1, r4
0030f238: bl #0x30e028
0030f23c: cmp r0, #0
0030f240: beq #0x30f254
0030f244: mov r1, #0
0030f248: bl #0x30e6ac
0030f24c: bl #0x30e6a0
0030f250: str r0, [r5, #4]
0030f254: mov r1, r4
0030f258: mov r0, #0
0030f25c: bl #0x30e028
0030f260: cmp r0, #0
0030f264: beq #0x30f278
0030f268: mov r1, #0
0030f26c: bl #0x30e6ac
0030f270: bl #0x30e6a0
0030f274: str r0, [r5, #8]
0030f278: mov r0, r6
0030f27c: bl #0x310440
0030f280: add sp, sp, #0xc
0030f284: pop {r4, r5, r6, r7, pc}

# 0x30f288 _Z8StrToObjPKcRf
0030f288: push {r4, lr}
0030f28c: mov r4, r1
0030f290: mov r1, #0
0030f294: bl #0x30e6ac
0030f298: bl #0x30e6a0
0030f29c: str r0, [r4]
0030f2a0: pop {r4, pc}

# 0x30f2a4 _Z8StrToObjPKcRN6glitch4core11dimension2dIfEE
0030f2a4: push {r4, r5, r6, r7, r8, sl, lr}
0030f2a8: sub sp, sp, #0xc
0030f2ac: mov r3, #0x2c
0030f2b0: mov r7, r0
0030f2b4: add r4, sp, #8
0030f2b8: mov r0, #0x100
0030f2bc: strh r3, [r4, #-4]!
0030f2c0: mov r5, r1
0030f2c4: bl #0x310454
0030f2c8: mov r1, r7
0030f2cc: mov r6, r0
0030f2d0: bl #0x30e520
0030f2d4: mov r0, r6
0030f2d8: mov r1, r4
0030f2dc: bl #0x30e028
0030f2e0: cmp r0, #0
0030f2e4: beq #0x30f2f8
0030f2e8: mov r1, #0
0030f2ec: bl #0x30e6ac
0030f2f0: bl #0x30e6a0
0030f2f4: mov sl, r0
0030f2f8: mov r1, r4
0030f2fc: mov r0, #0
0030f300: bl #0x30e028
0030f304: cmp r0, #0
0030f308: beq #0x30f31c
0030f30c: mov r1, #0
0030f310: bl #0x30e6ac
0030f314: bl #0x30e6a0
0030f318: mov r8, r0
0030f31c: mov r0, r6
0030f320: bl #0x310440
0030f324: str r8, [r5, #4]
0030f328: str sl, [r5]
0030f32c: add sp, sp, #0xc
0030f330: pop {r4, r5, r6, r7, r8, sl, pc}

# 0x30f334 _Z8StrToObjPKcRN6glitch4core8vector3dIfEE
0030f334: push {r4, r5, r6, r7, lr}
0030f338: sub sp, sp, #0xc
0030f33c: mov r3, #0x2c
0030f340: mov r7, r0
0030f344: add r4, sp, #8
0030f348: mov r0, #0x100
0030f34c: strh r3, [r4, #-4]!
0030f350: mov r5, r1
0030f354: bl #0x310454
0030f358: mov r1, r7
0030f35c: mov r6, r0
0030f360: bl #0x30e520
0030f364: mov r0, r6
0030f368: mov r1, r4
0030f36c: bl #0x30e028
0030f370: cmp r0, #0
0030f374: beq #0x30f388
0030f378: mov r1, #0
0030f37c: bl #0x30e6ac
0030f380: bl #0x30e6a0
0030f384: str r0, [r5]
0030f388: mov r0, #0
0030f38c: mov r1, r4
0030f390: bl #0x30e028
0030f394: cmp r0, #0
0030f398: beq #0x30f3ac
0030f39c: mov r1, #0
0030f3a0: bl #0x30e6ac
0030f3a4: bl #0x30e6a0
0030f3a8: str r0, [r5, #4]
0030f3ac: mov r1, r4
0030f3b0: mov r0, #0
0030f3b4: bl #0x30e028
0030f3b8: cmp r0, #0
0030f3bc: beq #0x30f3d0
0030f3c0: mov r1, #0
0030f3c4: bl #0x30e6ac
0030f3c8: bl #0x30e6a0
0030f3cc: str r0, [r5, #8]
0030f3d0: mov r0, r6
0030f3d4: bl #0x310440
0030f3d8: add sp, sp, #0xc
0030f3dc: pop {r4, r5, r6, r7, pc}

# 0x33de64 _ZN18SimpleTypePropertyIbE14IsDefaultValueEPv
0033de64: ldr r2, [r0, #4]
0033de68: ldrb r3, [r0, #0x20]
0033de6c: ldrb r0, [r1, r2]
0033de70: cmp r0, r3
0033de74: movne r0, #0
0033de78: moveq r0, #1
0033de7c: bx lr

# 0x33de80 _ZN18SimpleTypePropertyIbE17SetToDefaultValueEPv
0033de80: ldrb r2, [r0, #0x20]
0033de84: ldr r3, [r0, #4]
0033de88: strb r2, [r1, r3]
0033de8c: bx lr

# 0x33e218 _ZN18SimpleTypePropertyISsE8ToStringEPv
0033e218: push {r4, lr}
0033e21c: ldr r3, [r1, #4]
0033e220: mov r4, r0
0033e224: str r0, [r4, #0x10]
0033e228: str r0, [r4, #0x14]
0033e22c: add r3, r2, r3
0033e230: ldr r2, [r3, #0x10]
0033e234: ldr r1, [r3, #0x14]
0033e238: bl #0x3116e8
0033e23c: mov r0, r4
0033e240: pop {r4, pc}

# 0x33e298 _ZN18SimpleTypePropertyISsE17SetToDefaultValueEPv
0033e298: mov r3, r0
0033e29c: ldr r0, [r0, #4]
0033e2a0: add r2, r3, #0x20
0033e2a4: add r0, r1, r0
0033e2a8: cmp r0, r2
0033e2ac: bxeq lr
0033e2b0: ldr r2, [r3, #0x30]
0033e2b4: ldr r1, [r3, #0x34]
0033e2b8: b #0x3109e0

# 0x33e358 _ZN18SimpleTypePropertyISsE25SetDefaultValueFromStringEPKc
0033e358: push {r4, r5, r6, lr}
0033e35c: mov r4, r0
0033e360: mov r0, r1
0033e364: mov r5, r1
0033e368: bl #0x30de54
0033e36c: mov r1, r5
0033e370: add r2, r5, r0
0033e374: add r0, r4, #0x20
0033e378: pop {r4, r5, r6, lr}
0033e37c: b #0x3109e0

# 0x33e538 _ZN18SimpleTypePropertyISsE14IsDefaultValueEPv
0033e538: push {r4, lr}
0033e53c: ldr r2, [r0, #4]
0033e540: ldr ip, [r0, #0x30]
0033e544: ldr r3, [r0, #0x34]
0033e548: add r1, r1, r2
0033e54c: ldr r2, [r1, #0x10]
0033e550: ldr r0, [r1, #0x14]
0033e554: rsb ip, r3, ip
0033e558: rsb r2, r0, r2
0033e55c: cmp r2, ip
0033e560: beq #0x33e56c
0033e564: mov r0, #0
0033e568: pop {r4, pc}
0033e56c: mov r1, r3
0033e570: bl #0x30e5e0
0033e574: rsbs r0, r0, #1
0033e578: movlo r0, #0
0033e57c: pop {r4, pc}

# 0x33e580 _ZN18SimpleTypePropertyIbE25SetDefaultValueFromStringEPKc
0033e580: mov r3, r0
0033e584: mov r2, #0
0033e588: strb r2, [r3, #0x20]!
0033e58c: mov r0, r1
0033e590: mov r1, r3
0033e594: b #0x30f020

# 0x33e598 _ZN18SimpleTypePropertyIbE10FromStringEPvPKc
0033e598: ldr r3, [r0, #4]
0033e59c: mov r0, #0
0033e5a0: strb r0, [r1, r3]
0033e5a4: add r1, r1, r3
0033e5a8: mov r0, r2
0033e5ac: b #0x30f020

# 0x33e5b0 _ZN18SimpleTypePropertyIbE8ToStringEPv
0033e5b0: push {r4, r5, r6, r7, lr}
0033e5b4: mov r4, r0
0033e5b8: sub sp, sp, #0xc
0033e5bc: mov r0, #0x100
0033e5c0: mov r7, r2
0033e5c4: mov r6, r1
0033e5c8: bl #0x310454
0033e5cc: ldr r1, [r6, #4]
0033e5d0: mov r5, r0
0033e5d4: add r1, r7, r1
0033e5d8: bl #0x30eeb4
0033e5dc: mov r0, r4
0033e5e0: mov r1, r5
0033e5e4: add r2, sp, #4
0033e5e8: bl #0x3140ec
0033e5ec: mov r0, r4
0033e5f0: add sp, sp, #0xc
0033e5f4: pop {r4, r5, r6, r7, pc}

# 0x33ee34 _ZN18SimpleTypePropertyISsE5CloneEv
0033ee34: push {r4, r5, r6, r7, r8, lr}
0033ee38: mov r1, #0
0033ee3c: mov r7, r0
0033ee40: mov r0, #0x38
0033ee44: bl #0x310570
0033ee48: ldr r5, [pc, #0x7c]
0033ee4c: ldr r2, [pc, #0x7c]
0033ee50: mov r3, r0
0033ee54: add r5, pc, r5
0033ee58: ldr r2, [r5, r2]
0033ee5c: mov r4, r0
0033ee60: mov r1, #0x10
0033ee64: add r2, r2, #8
0033ee68: str r2, [r3], #8
0033ee6c: mov r0, r3
0033ee70: str r3, [r4, #0x18]
0033ee74: str r3, [r4, #0x1c]
0033ee78: bl #0x31167c
0033ee7c: ldr r2, [pc, #0x50]
0033ee80: ldr r1, [r4, #0x18]
0033ee84: mov r3, r4
0033ee88: ldr r2, [r5, r2]
0033ee8c: mov r6, #0
0033ee90: strb r6, [r1]
0033ee94: add r2, r2, #8
0033ee98: str r2, [r3], #0x20
0033ee9c: mov r0, r3
0033eea0: str r3, [r4, #0x30]
0033eea4: str r3, [r4, #0x34]
0033eea8: mov r1, #0x10
0033eeac: bl #0x31167c
0033eeb0: ldr r3, [r4, #0x30]
0033eeb4: mov r1, r7
0033eeb8: mov r0, r4
0033eebc: strb r6, [r3]
0033eec0: bl #0x33e244
0033eec4: mov r0, r4
0033eec8: pop {r4, r5, r6, r7, r8, pc}
0033eecc: rsbeq r5, r5, ip, lsr ip
0033eed0: andeq r2, r0, r0, lsr r3
0033eed4: muleq r0, r4, r4

# 0x33eed8 _ZN18SimpleTypePropertyIbE5CloneEv
0033eed8: push {r4, r5, r6, r7, r8, lr}
0033eedc: mov r1, #0
0033eee0: mov r5, r0
0033eee4: mov r0, #0x24
0033eee8: bl #0x310570
0033eeec: ldr r7, [pc, #0x7c]
0033eef0: ldr r3, [pc, #0x7c]
0033eef4: mov r6, r0
0033eef8: add r7, pc, r7
0033eefc: ldr r3, [r7, r3]
0033ef00: mov r4, r0
0033ef04: mov r1, #0x10
0033ef08: add r3, r3, #8
0033ef0c: str r3, [r6], #8
0033ef10: str r6, [r0, #0x18]
0033ef14: str r6, [r0, #0x1c]
0033ef18: mov r0, r6
0033ef1c: bl #0x31167c
0033ef20: ldr r3, [pc, #0x50]
0033ef24: ldr r2, [r4, #0x18]
0033ef28: mov r1, #0
0033ef2c: ldr r3, [r7, r3]
0033ef30: strb r1, [r2]
0033ef34: add r2, r5, #8
0033ef38: add r3, r3, #8
0033ef3c: str r3, [r4]
0033ef40: ldr r3, [r5, #4]
0033ef44: cmp r6, r2
0033ef48: str r3, [r4, #4]
0033ef4c: beq #0x33ef60
0033ef50: mov r0, r6
0033ef54: ldr r1, [r5, #0x1c]
0033ef58: ldr r2, [r5, #0x18]
0033ef5c: bl #0x3109e0
0033ef60: ldrb r3, [r5, #0x20]
0033ef64: mov r0, r4
0033ef68: strb r3, [r4, #0x20]
0033ef6c: pop {r4, r5, r6, r7, r8, pc}
0033ef70: mlseq r5, r8, fp, r5
0033ef74: andeq r2, r0, r0, lsr r3
0033ef78: andeq r3, r0, ip, asr #28

# 0x33f4c4 _ZN18SimpleTypePropertyISsE10FromStringEPvPKc
0033f4c4: push {r4, r5, r6, lr}
0033f4c8: mov r6, r0
0033f4cc: mov r0, r2
0033f4d0: mov r4, r2
0033f4d4: mov r5, r1
0033f4d8: bl #0x30de54
0033f4dc: ldr r3, [r6, #4]
0033f4e0: add r2, r4, r0
0033f4e4: mov r1, r4
0033f4e8: add r0, r5, r3
0033f4ec: pop {r4, r5, r6, lr}
0033f4f0: b #0x3109e0

# 0x38777c _ZN18SimpleTypePropertyIN6glitch4core11dimension2dIfEEE17SetToDefaultValueEPv
0038777c: ldr r3, [r0, #0x20]
00387780: ldr r2, [r0, #4]
00387784: str r3, [r1, r2]!
00387788: ldr r3, [r0, #0x24]
0038778c: str r3, [r1, #4]
00387790: bx lr

# 0x3877f8 _ZN18SimpleTypePropertyIN6glitch4core8vector3dIfEEE17SetToDefaultValueEPv
003877f8: ldr r3, [r0, #4]
003877fc: ldr r2, [r0, #0x20]
00387800: add ip, r1, r3
00387804: str r2, [r1, r3]
00387808: ldr r3, [r0, #0x24]
0038780c: str r3, [ip, #4]
00387810: ldr r3, [r0, #0x28]
00387814: str r3, [ip, #8]
00387818: bx lr

# 0x387a8c _ZN18SimpleTypePropertyIN6glitch4core8vector3dIfEEE25SetDefaultValueFromStringEPKc
00387a8c: mov r3, r0
00387a90: mov r2, #0
00387a94: mov r0, r1
00387a98: add r1, r3, #0x20
00387a9c: str r2, [r3, #0x28]
00387aa0: str r2, [r3, #0x20]
00387aa4: str r2, [r3, #0x24]
00387aa8: b #0x30f334

# 0x387aac _ZN18SimpleTypePropertyIN6glitch4core8vector3dIfEEE10FromStringEPvPKc
00387aac: ldr ip, [r0, #4]
00387ab0: mov r0, #0
00387ab4: add r3, r1, ip
00387ab8: str r0, [r1, ip]
00387abc: str r0, [r3, #8]
00387ac0: str r0, [r3, #4]
00387ac4: mov r1, r3
00387ac8: mov r0, r2
00387acc: b #0x30f334

# 0x387b18 _ZN18SimpleTypePropertyIN6glitch4core11dimension2dIfEEE25SetDefaultValueFromStringEPKc
00387b18: mov r3, r0
00387b1c: mov r2, #0
00387b20: str r2, [r0, #0x24]
00387b24: str r2, [r3, #0x20]!
00387b28: mov r0, r1
00387b2c: mov r1, r3
00387b30: b #0x30f2a4

# 0x387b34 _ZN18SimpleTypePropertyIN6glitch4core11dimension2dIfEEE10FromStringEPvPKc
00387b34: ldr r0, [r0, #4]
00387b38: mov ip, #0
00387b3c: add r3, r1, r0
00387b40: str ip, [r3, #4]
00387b44: str ip, [r1, r0]
00387b48: mov r0, r2
00387b4c: mov r1, r3
00387b50: b #0x30f2a4

# 0x387e40 _ZN18SimpleTypePropertyIN6glitch4core8vector3dIfEEE5CloneEv
00387e40: push {r4, r5, r6, r7, r8, lr}
00387e44: mov r1, #0
00387e48: mov r5, r0
00387e4c: mov r0, #0x2c
00387e50: bl #0x310570
00387e54: ldr r7, [pc, #0x9c]
00387e58: ldr r3, [pc, #0x9c]
00387e5c: mov r6, r0
00387e60: add r7, pc, r7
00387e64: ldr r3, [r7, r3]
00387e68: mov r4, r0
00387e6c: mov r1, #0x10
00387e70: add r3, r3, #8
00387e74: str r3, [r6], #8
00387e78: str r6, [r0, #0x18]
00387e7c: str r6, [r0, #0x1c]
00387e80: mov r0, r6
00387e84: bl #0x31167c
00387e88: ldr r2, [pc, #0x70]
00387e8c: ldr r1, [r4, #0x18]
00387e90: mov r3, #0
00387e94: ldr r2, [r7, r2]
00387e98: mov r0, #0
00387e9c: strb r0, [r1]
00387ea0: add r2, r2, #8
00387ea4: str r2, [r4]
00387ea8: str r3, [r4, #0x28]
00387eac: str r3, [r4, #0x20]
00387eb0: str r3, [r4, #0x24]
00387eb4: ldr r3, [r5, #4]
00387eb8: add r2, r5, #8
00387ebc: cmp r6, r2
00387ec0: str r3, [r4, #4]
00387ec4: beq #0x387ed8
00387ec8: mov r0, r6
00387ecc: ldr r1, [r5, #0x1c]
00387ed0: ldr r2, [r5, #0x18]
00387ed4: bl #0x3109e0
00387ed8: ldr r3, [r5, #0x20]
00387edc: mov r0, r4
00387ee0: str r3, [r4, #0x20]
00387ee4: ldr r3, [r5, #0x24]
00387ee8: str r3, [r4, #0x24]
00387eec: ldr r3, [r5, #0x28]
00387ef0: str r3, [r4, #0x28]
00387ef4: pop {r4, r5, r6, r7, r8, pc}
00387ef8: rsbeq ip, r0, r0, lsr ip
00387efc: andeq r2, r0, r0, lsr r3
00387f00: muleq r0, r0, fp

# 0x387f04 _ZN18SimpleTypePropertyIN6glitch4core11dimension2dIfEEE5CloneEv
00387f04: push {r4, r5, r6, r7, r8, lr}
00387f08: mov r1, #0
00387f0c: mov r5, r0
00387f10: mov r0, #0x28
00387f14: bl #0x310570
00387f18: ldr r7, [pc, #0x90]
00387f1c: ldr r3, [pc, #0x90]
00387f20: mov r6, r0
00387f24: add r7, pc, r7
00387f28: ldr r3, [r7, r3]
00387f2c: mov r4, r0
00387f30: mov r1, #0x10
00387f34: add r3, r3, #8
00387f38: str r3, [r6], #8
00387f3c: str r6, [r0, #0x18]
00387f40: str r6, [r0, #0x1c]
00387f44: mov r0, r6
00387f48: bl #0x31167c
00387f4c: ldr r3, [pc, #0x64]
00387f50: ldr r1, [r4, #0x18]
00387f54: mov r2, #0
00387f58: ldr r3, [r7, r3]
00387f5c: mov r0, #0
00387f60: strb r0, [r1]
00387f64: add r3, r3, #8
00387f68: str r2, [r4, #0x24]
00387f6c: str r2, [r4, #0x20]
00387f70: str r3, [r4]
00387f74: ldr r3, [r5, #4]
00387f78: add r2, r5, #8
00387f7c: cmp r6, r2
00387f80: str r3, [r4, #4]
00387f84: beq #0x387f98
00387f88: mov r0, r6
00387f8c: ldr r1, [r5, #0x1c]
00387f90: ldr r2, [r5, #0x18]
00387f94: bl #0x3109e0
00387f98: ldr r3, [r5, #0x20]
00387f9c: mov r0, r4
00387fa0: str r3, [r4, #0x20]
00387fa4: ldr r3, [r5, #0x24]
00387fa8: str r3, [r4, #0x24]
00387fac: pop {r4, r5, r6, r7, r8, pc}
00387fb0: rsbeq ip, r0, ip, ror #22
00387fb4: andeq r2, r0, r0, lsr r3
00387fb8: ldrdeq r2, r3, [r0], -r4

# 0x3883fc _ZN18SimpleTypePropertyI7Point3DIfEE17SetToDefaultValueEPv
003883fc: ldr r3, [r0, #4]
00388400: ldr r2, [r0, #0x20]
00388404: add ip, r1, r3
00388408: str r2, [r1, r3]
0038840c: ldr r3, [r0, #0x24]
00388410: str r3, [ip, #4]
00388414: ldr r3, [r0, #0x28]
00388418: str r3, [ip, #8]
0038841c: bx lr

# 0x38865c _ZN18SimpleTypePropertyI7Point3DIfEE25SetDefaultValueFromStringEPKc
0038865c: mov r3, r0
00388660: mov r2, #0
00388664: mov r0, r1
00388668: add r1, r3, #0x20
0038866c: str r2, [r3, #0x28]
00388670: str r2, [r3, #0x20]
00388674: str r2, [r3, #0x24]
00388678: b #0x30f1dc

# 0x38867c _ZN18SimpleTypePropertyI7Point3DIfEE10FromStringEPvPKc
0038867c: ldr ip, [r0, #4]
00388680: mov r0, #0
00388684: add r3, r1, ip
00388688: str r0, [r1, ip]
0038868c: str r0, [r3, #8]
00388690: str r0, [r3, #4]
00388694: mov r1, r3
00388698: mov r0, r2
0038869c: b #0x30f1dc

# 0x389ee4 _ZN18SimpleTypePropertyI7Point3DIfEE5CloneEv
00389ee4: push {r4, r5, r6, r7, r8, lr}
00389ee8: mov r1, #0
00389eec: mov r5, r0
00389ef0: mov r0, #0x2c
00389ef4: bl #0x310570
00389ef8: ldr r7, [pc, #0x9c]
00389efc: ldr r3, [pc, #0x9c]
00389f00: mov r6, r0
00389f04: add r7, pc, r7
00389f08: ldr r3, [r7, r3]
00389f0c: mov r4, r0
00389f10: mov r1, #0x10
00389f14: add r3, r3, #8
00389f18: str r3, [r6], #8
00389f1c: str r6, [r0, #0x18]
00389f20: str r6, [r0, #0x1c]
00389f24: mov r0, r6
00389f28: bl #0x31167c
00389f2c: ldr r2, [pc, #0x70]
00389f30: ldr r1, [r4, #0x18]
00389f34: mov r3, #0
00389f38: ldr r2, [r7, r2]
00389f3c: mov r0, #0
00389f40: strb r0, [r1]
00389f44: add r2, r2, #8
00389f48: str r2, [r4]
00389f4c: str r3, [r4, #0x28]
00389f50: str r3, [r4, #0x20]
00389f54: str r3, [r4, #0x24]
00389f58: ldr r3, [r5, #4]
00389f5c: add r2, r5, #8
00389f60: cmp r6, r2
00389f64: str r3, [r4, #4]
00389f68: beq #0x389f7c
00389f6c: mov r0, r6
00389f70: ldr r1, [r5, #0x1c]
00389f74: ldr r2, [r5, #0x18]
00389f78: bl #0x3109e0
00389f7c: ldr r3, [r5, #0x20]
00389f80: mov r0, r4
00389f84: str r3, [r4, #0x20]
00389f88: ldr r3, [r5, #0x24]
00389f8c: str r3, [r4, #0x24]
00389f90: ldr r3, [r5, #0x28]
00389f94: str r3, [r4, #0x28]
00389f98: pop {r4, r5, r6, r7, r8, pc}
00389f9c: rsbeq sl, r0, ip, lsl #23
00389fa0: andeq r2, r0, r0, lsr r3
00389fa4: andeq r0, r0, r4, asr #22

# 0x38ae00 _ZN18SimpleTypePropertyIiE14IsDefaultValueEPv
0038ae00: ldr r2, [r0, #4]
0038ae04: ldr r3, [r0, #0x20]
0038ae08: ldr r0, [r1, r2]
0038ae0c: cmp r0, r3
0038ae10: movne r0, #0
0038ae14: moveq r0, #1
0038ae18: bx lr

# 0x38ae1c _ZN18SimpleTypePropertyIiE17SetToDefaultValueEPv
0038ae1c: ldr r2, [r0, #0x20]
0038ae20: ldr r3, [r0, #4]
0038ae24: str r2, [r1, r3]
0038ae28: bx lr

# 0x38ba94 _ZN18SimpleTypePropertyIiE25SetDefaultValueFromStringEPKc
0038ba94: mov r3, r0
0038ba98: mov r2, #0
0038ba9c: str r2, [r3, #0x20]!
0038baa0: mov r0, r1
0038baa4: mov r1, r3
0038baa8: b #0x30f03c

# 0x38baac _ZN18SimpleTypePropertyIiE10FromStringEPvPKc
0038baac: ldr r3, [r0, #4]
0038bab0: mov r0, #0
0038bab4: str r0, [r1, r3]
0038bab8: add r1, r1, r3
0038babc: mov r0, r2
0038bac0: b #0x30f03c

# 0x38bac4 _ZN18SimpleTypePropertyIiE8ToStringEPv
0038bac4: push {r4, r5, r6, r7, lr}
0038bac8: mov r4, r0
0038bacc: sub sp, sp, #0xc
0038bad0: mov r0, #0x100
0038bad4: mov r7, r2
0038bad8: mov r6, r1
0038badc: bl #0x310454
0038bae0: ldr r1, [r6, #4]
0038bae4: mov r5, r0
0038bae8: add r1, r7, r1
0038baec: bl #0x30eea0
0038baf0: mov r0, r4
0038baf4: mov r1, r5
0038baf8: add r2, sp, #4
0038bafc: bl #0x3140ec
0038bb00: mov r0, r4
0038bb04: add sp, sp, #0xc
0038bb08: pop {r4, r5, r6, r7, pc}

# 0x38c08c _ZN18SimpleTypePropertyIiE5CloneEv
0038c08c: push {r4, r5, r6, r7, r8, lr}
0038c090: mov r1, #0
0038c094: mov r5, r0
0038c098: mov r0, #0x24
0038c09c: bl #0x310570
0038c0a0: ldr r7, [pc, #0x7c]
0038c0a4: ldr r3, [pc, #0x7c]
0038c0a8: mov r6, r0
0038c0ac: add r7, pc, r7
0038c0b0: ldr r3, [r7, r3]
0038c0b4: mov r4, r0
0038c0b8: mov r1, #0x10
0038c0bc: add r3, r3, #8
0038c0c0: str r3, [r6], #8
0038c0c4: str r6, [r0, #0x18]
0038c0c8: str r6, [r0, #0x1c]
0038c0cc: mov r0, r6
0038c0d0: bl #0x31167c
0038c0d4: ldr r3, [pc, #0x50]
0038c0d8: ldr r2, [r4, #0x18]
0038c0dc: mov r1, #0
0038c0e0: ldr r3, [r7, r3]
0038c0e4: strb r1, [r2]
0038c0e8: add r2, r5, #8
0038c0ec: add r3, r3, #8
0038c0f0: str r3, [r4]
0038c0f4: ldr r3, [r5, #4]
0038c0f8: cmp r6, r2
0038c0fc: str r3, [r4, #4]
0038c100: beq #0x38c114
0038c104: mov r0, r6
0038c108: ldr r1, [r5, #0x1c]
0038c10c: ldr r2, [r5, #0x18]
0038c110: bl #0x3109e0
0038c114: ldr r3, [r5, #0x20]
0038c118: mov r0, r4
0038c11c: str r3, [r4, #0x20]
0038c120: pop {r4, r5, r6, r7, r8, pc}
0038c124: rsbeq r8, r0, r4, ror #19
0038c128: andeq r2, r0, r0, lsr r3
0038c12c: muleq r0, r0, r5

# 0x394ed4 _ZN18SimpleTypePropertyIfE14IsDefaultValueEPv
00394ed4: push {r4, lr}
00394ed8: ldr r2, [r0, #4]
00394edc: mov r3, r0
00394ee0: mov r4, #0
00394ee4: ldr r0, [r1, r2]
00394ee8: ldr r1, [r3, #0x20]
00394eec: bl #0x30df8c
00394ef0: cmp r0, #0
00394ef4: movne r4, #1
00394ef8: and r0, r4, #1
00394efc: pop {r4, pc}

# 0x394f00 _ZN18SimpleTypePropertyIfE17SetToDefaultValueEPv
00394f00: ldr r2, [r0, #0x20]
00394f04: ldr r3, [r0, #4]
00394f08: str r2, [r1, r3]
00394f0c: bx lr

# 0x394f98 _ZN18SimpleTypePropertyIfE5CloneEv
00394f98: push {r4, r5, r6, r7, r8, lr}
00394f9c: mov r1, #0
00394fa0: mov r5, r0
00394fa4: mov r0, #0x24
00394fa8: bl #0x310570
00394fac: ldr r7, [pc, #0x7c]
00394fb0: ldr r3, [pc, #0x7c]
00394fb4: mov r6, r0
00394fb8: add r7, pc, r7
00394fbc: ldr r3, [r7, r3]
00394fc0: mov r4, r0
00394fc4: mov r1, #0x10
00394fc8: add r3, r3, #8
00394fcc: str r3, [r6], #8
00394fd0: str r6, [r0, #0x18]
00394fd4: str r6, [r0, #0x1c]
00394fd8: mov r0, r6
00394fdc: bl #0x31167c
00394fe0: ldr r3, [pc, #0x50]
00394fe4: ldr r2, [r4, #0x18]
00394fe8: mov r1, #0
00394fec: ldr r3, [r7, r3]
00394ff0: strb r1, [r2]
00394ff4: add r2, r5, #8
00394ff8: add r3, r3, #8
00394ffc: str r3, [r4]
00395000: ldr r3, [r5, #4]
00395004: cmp r6, r2
00395008: str r3, [r4, #4]
0039500c: beq #0x395020
00395010: mov r0, r6
00395014: ldr r1, [r5, #0x1c]
00395018: ldr r2, [r5, #0x18]
0039501c: bl #0x3109e0
00395020: ldr r3, [r5, #0x20]
00395024: mov r0, r4
00395028: str r3, [r4, #0x20]
0039502c: pop {r4, r5, r6, r7, r8, pc}
00395030: ldrsbeq pc, [pc], #-0xa8
00395034: andeq r2, r0, r0, lsr r3
00395038: strheq r2, [r0], -ip

# 0x3950c8 _ZN18SimpleTypePropertyIfE25SetDefaultValueFromStringEPKc
003950c8: mov r3, r0
003950cc: mov r2, #0
003950d0: str r2, [r3, #0x20]!
003950d4: mov r0, r1
003950d8: mov r1, r3
003950dc: b #0x30f288

# 0x3950e0 _ZN18SimpleTypePropertyIfE10FromStringEPvPKc
003950e0: ldr r3, [r0, #4]
003950e4: mov ip, #0
003950e8: mov r0, r1
003950ec: str ip, [r0, r3]
003950f0: add r1, r1, r3
003950f4: mov r0, r2
003950f8: b #0x30f288

# 0x3950fc _ZN18SimpleTypePropertyIfE8ToStringEPv
003950fc: push {r4, r5, r6, r7, lr}
00395100: mov r4, r0
00395104: sub sp, sp, #0xc
00395108: mov r0, #0x100
0039510c: mov r7, r2
00395110: mov r6, r1
00395114: bl #0x310454
00395118: ldr r1, [r6, #4]
0039511c: mov r5, r0
00395120: add r1, r7, r1
00395124: bl #0x30eec8
00395128: mov r0, r4
0039512c: mov r1, r5
00395130: add r2, sp, #4
00395134: bl #0x3140ec
00395138: mov r0, r4
0039513c: add sp, sp, #0xc
00395140: pop {r4, r5, r6, r7, pc}

# 0x3a357c _ZN18SimpleTypePropertyI7Point2DIiEE14IsDefaultValueEPv
003a357c: push {r4, r5, r6, r7, r8, lr}
003a3580: ldr r3, [r0, #4]
003a3584: mov r4, r0
003a3588: ldr r5, [r0, #0x24]
003a358c: ldr r0, [r1, r3]
003a3590: add r3, r1, r3
003a3594: ldr r6, [r3, #4]
003a3598: bl #0x30e964
003a359c: mov r7, r0
003a35a0: ldr r0, [r4, #0x20]
003a35a4: bl #0x30e964
003a35a8: mov r1, r0
003a35ac: mov r0, r7
003a35b0: bl #0x30df8c
003a35b4: cmp r0, #0
003a35b8: beq #0x3a35ec
003a35bc: mov r0, r6
003a35c0: bl #0x30e964
003a35c4: mov r4, r0
003a35c8: mov r0, r5
003a35cc: bl #0x30e964
003a35d0: mov r1, r0
003a35d4: mov r0, r4
003a35d8: bl #0x30df8c
003a35dc: cmp r0, #0
003a35e0: mov r0, #0
003a35e4: movne r0, #1
003a35e8: uxtb r0, r0
003a35ec: pop {r4, r5, r6, r7, r8, pc}

# 0x3a35f0 _ZN18SimpleTypePropertyI7Point2DIiEE17SetToDefaultValueEPv
003a35f0: ldr r3, [r0, #4]
003a35f4: ldr r2, [r0, #0x20]
003a35f8: add ip, r1, r3
003a35fc: str r2, [r1, r3]
003a3600: ldr r3, [r0, #0x24]
003a3604: str r3, [ip, #4]
003a3608: bx lr

# 0x3a66bc _ZN18SimpleTypePropertyI7Point2DIiEE25SetDefaultValueFromStringEPKc
003a66bc: mov r3, r0
003a66c0: mov r2, #0
003a66c4: str r2, [r0, #0x24]
003a66c8: mov r0, r1
003a66cc: add r1, r3, #0x20
003a66d0: str r2, [r3, #0x20]
003a66d4: b #0x30f050

# 0x3a66d8 _ZN18SimpleTypePropertyI7Point2DIiEE10FromStringEPvPKc
003a66d8: ldr r0, [r0, #4]
003a66dc: mov ip, #0
003a66e0: add r3, r1, r0
003a66e4: str ip, [r1, r0]
003a66e8: mov r0, r2
003a66ec: mov r1, r3
003a66f0: str ip, [r3, #4]
003a66f4: b #0x30f050

# 0x3a926c _ZN18SimpleTypePropertyI7Point2DIiEE8ToStringEPv
003a926c: push {r4, r5, r6, r7, lr}
003a9270: mov r4, r0
003a9274: sub sp, sp, #0xc
003a9278: mov r0, #0x100
003a927c: mov r7, r2
003a9280: mov r6, r1
003a9284: bl #0x310454
003a9288: ldr r1, [r6, #4]
003a928c: mov r5, r0
003a9290: add r1, r7, r1
003a9294: bl #0x30ee84
003a9298: mov r0, r4
003a929c: mov r1, r5
003a92a0: add r2, sp, #4
003a92a4: bl #0x3140ec
003a92a8: mov r0, r4
003a92ac: add sp, sp, #0xc
003a92b0: pop {r4, r5, r6, r7, pc}

# 0x3a98e8 _ZN18SimpleTypePropertyI7Point2DIiEE5CloneEv
003a98e8: push {r4, r5, r6, r7, r8, lr}
003a98ec: mov r1, #0
003a98f0: mov r5, r0
003a98f4: mov r0, #0x28
003a98f8: bl #0x310570
003a98fc: ldr r7, [pc, #0x8c]
003a9900: ldr r3, [pc, #0x8c]
003a9904: mov r6, r0
003a9908: add r7, pc, r7
003a990c: ldr r3, [r7, r3]
003a9910: mov r4, r0
003a9914: mov r1, #0x10
003a9918: add r3, r3, #8
003a991c: str r3, [r6], #8
003a9920: str r6, [r0, #0x18]
003a9924: str r6, [r0, #0x1c]
003a9928: mov r0, r6
003a992c: bl #0x31167c
003a9930: ldr r2, [pc, #0x60]
003a9934: ldr r1, [r4, #0x18]
003a9938: mov r3, #0
003a993c: ldr r2, [r7, r2]
003a9940: strb r3, [r1]
003a9944: str r3, [r4, #0x24]
003a9948: add r2, r2, #8
003a994c: str r2, [r4]
003a9950: str r3, [r4, #0x20]
003a9954: ldr r3, [r5, #4]
003a9958: add r2, r5, #8
003a995c: cmp r6, r2
003a9960: str r3, [r4, #4]
003a9964: beq #0x3a9978
003a9968: mov r0, r6
003a996c: ldr r1, [r5, #0x1c]
003a9970: ldr r2, [r5, #0x18]
003a9974: bl #0x3109e0
003a9978: ldr r3, [r5, #0x20]
003a997c: mov r0, r4
003a9980: str r3, [r4, #0x20]
003a9984: ldr r3, [r5, #0x24]
003a9988: str r3, [r4, #0x24]
003a998c: pop {r4, r5, r6, r7, r8, pc}
003a9990: subseq fp, lr, r8, lsl #3
003a9994: andeq r2, r0, r0, lsr r3
003a9998: andeq r2, r0, r0, lsr r1

# 0x3f8eb4 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE17SetToDefaultValueEPv
003f8eb4: mov r3, r0
003f8eb8: ldr r0, [r0, #4]
003f8ebc: add r0, r1, r0
003f8ec0: add r1, r3, #0x20
003f8ec4: b #0x3f8c40

# 0x3f8ec8 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE5CloneEv
003f8ec8: push {r4, r5, r6, r7, r8, lr}
003f8ecc: mov r1, #0
003f8ed0: mov r5, r0
003f8ed4: mov r0, #0x2c
003f8ed8: bl #0x310570
003f8edc: ldr r7, [pc, #0x8c]
003f8ee0: ldr r3, [pc, #0x8c]
003f8ee4: mov r6, r0
003f8ee8: add r7, pc, r7
003f8eec: ldr r3, [r7, r3]
003f8ef0: mov r4, r0
003f8ef4: mov r1, #0x10
003f8ef8: add r3, r3, #8
003f8efc: str r3, [r6], #8
003f8f00: str r6, [r0, #0x18]
003f8f04: str r6, [r0, #0x1c]
003f8f08: mov r0, r6
003f8f0c: bl #0x31167c
003f8f10: ldr r2, [pc, #0x60]
003f8f14: ldr r1, [r4, #0x18]
003f8f18: mov r3, #0
003f8f1c: ldr r2, [r7, r2]
003f8f20: strb r3, [r1]
003f8f24: str r3, [r4, #0x28]
003f8f28: add r2, r2, #8
003f8f2c: str r2, [r4]
003f8f30: str r3, [r4, #0x20]
003f8f34: str r3, [r4, #0x24]
003f8f38: ldr r3, [r5, #4]
003f8f3c: add r2, r5, #8
003f8f40: cmp r6, r2
003f8f44: str r3, [r4, #4]
003f8f48: beq #0x3f8f5c
003f8f4c: mov r0, r6
003f8f50: ldr r1, [r5, #0x1c]
003f8f54: ldr r2, [r5, #0x18]
003f8f58: bl #0x3109e0
003f8f5c: add r1, r5, #0x20
003f8f60: add r0, r4, #0x20
003f8f64: bl #0x3f8c40
003f8f68: mov r0, r4
003f8f6c: pop {r4, r5, r6, r7, r8, pc}
003f8f70: subseq fp, sb, r8, lsr #23
003f8f74: andeq r2, r0, r0, lsr r3
003f8f78: muleq r0, r8, r6

# 0x3f8f7c _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE25SetDefaultValueFromStringEPKc
003f8f7c: push {r4, r5, r6, lr}
003f8f80: sub sp, sp, #0x10
003f8f84: add r4, r0, #0x20
003f8f88: add r5, sp, #4
003f8f8c: mov r3, #0
003f8f90: mov r6, r1
003f8f94: mov r0, r4
003f8f98: mov r1, r5
003f8f9c: str r3, [sp, #0xc]
003f8fa0: str r3, [sp, #4]
003f8fa4: str r3, [sp, #8]
003f8fa8: bl #0x3f8c40
003f8fac: mov r0, r5
003f8fb0: bl #0x34611c
003f8fb4: mov r0, r6
003f8fb8: mov r1, r4
003f8fbc: bl #0x30f010
003f8fc0: add sp, sp, #0x10
003f8fc4: pop {r4, r5, r6, pc}

# 0x3f8fc8 _ZN18SimpleTypePropertyISt6vectorI7Point3DIfESaIS2_EEE10FromStringEPvPKc
003f8fc8: push {r4, r5, r6, lr}
003f8fcc: ldr r4, [r0, #4]
003f8fd0: sub sp, sp, #0x10
003f8fd4: add r5, sp, #4
003f8fd8: add r4, r1, r4
003f8fdc: mov r3, #0
003f8fe0: mov r1, r5
003f8fe4: mov r0, r4
003f8fe8: mov r6, r2
003f8fec: str r3, [sp, #0xc]
003f8ff0: str r3, [sp, #4]
003f8ff4: str r3, [sp, #8]
003f8ff8: bl #0x3f8c40
003f8ffc: mov r0, r5
003f9000: bl #0x34611c
003f9004: mov r0, r6
003f9008: mov r1, r4
003f900c: bl #0x30f010
003f9010: add sp, sp, #0x10
003f9014: pop {r4, r5, r6, pc}

# 0x519d30 _ZN12TiXmlElement9ReadValueEPKcP16TiXmlParsingData13TiXmlEncoding
00519d30: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00519d34: sub sp, sp, #0x1c
00519d38: mov r6, r3
00519d3c: mov sl, r1
00519d40: mov sb, r2
00519d44: mov fp, r0
00519d48: bl #0x5144b8
00519d4c: mov r1, r6
00519d50: str r0, [sp, #0x14]
00519d54: mov r0, sl
00519d58: bl #0x518788
00519d5c: ldr r7, [pc, #0x1a4]
00519d60: subs r5, r0, #0
00519d64: add r7, pc, r7
00519d68: beq #0x519e64
00519d6c: ldrb r3, [r5]
00519d70: cmp r3, #0
00519d74: beq #0x519e58
00519d78: ldr r2, [pc, #0x18c]
00519d7c: ldr r8, [pc, #0x18c]
00519d80: add r2, pc, r2
00519d84: str r2, [sp, #0x10]
00519d88: ldr r2, [pc, #0x184]
00519d8c: add r8, pc, r8
00519d90: str r2, [sp, #8]
00519d94: ldr r2, [pc, #0x17c]
00519d98: str r2, [sp, #0xc]
00519d9c: cmp r3, #0x3c
00519da0: beq #0x519e90
00519da4: mov r1, #0
00519da8: mov r0, #0x44
00519dac: bl #0x310570
00519db0: mov r1, #4
00519db4: mov r4, r0
00519db8: bl #0x515e18
00519dbc: ldr r2, [sp, #8]
00519dc0: mov r0, r4
00519dc4: mov r1, r8
00519dc8: ldr r3, [r7, r2]
00519dcc: mov r2, r8
00519dd0: add r3, r3, #8
00519dd4: str r3, [r0], #0x20
00519dd8: bl #0x3109e0
00519ddc: ldr r2, [sp, #0xc]
00519de0: ldr ip, [r4]
00519de4: mov r0, r4
00519de8: ldr r3, [r7, r2]
00519dec: mov r2, #0
00519df0: strb r2, [r4, #0x40]
00519df4: ldrb r3, [r3]
00519df8: mov r2, sb
00519dfc: cmp r3, #0
00519e00: moveq r1, sl
00519e04: movne r1, r5
00519e08: mov r3, r6
00519e0c: mov lr, pc
00519e10: ldr pc, [ip, #0xc]
00519e14: mov sl, r0
00519e18: mov r0, r4
00519e1c: bl #0x518990
00519e20: cmp r0, #0
00519e24: beq #0x519ef0
00519e28: mov r0, r4
00519e2c: ldr r3, [r4]
00519e30: mov lr, pc
00519e34: ldr pc, [r3, #4]
00519e38: mov r0, sl
00519e3c: mov r1, r6
00519e40: bl #0x518788
00519e44: subs r5, r0, #0
00519e48: beq #0x519e64
00519e4c: ldrb r3, [r5]
00519e50: cmp r3, #0
00519e54: bne #0x519d9c
00519e58: mov r0, r5
00519e5c: add sp, sp, #0x1c
00519e60: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00519e64: ldr r3, [sp, #0x14]
00519e68: cmp r3, #0
00519e6c: beq #0x519f00
00519e70: mov r2, #0
00519e74: mov r0, r3
00519e78: mov r1, #6
00519e7c: mov r3, r2
00519e80: str r6, [sp]
00519e84: mov r5, #0
00519e88: bl #0x5194e8
00519e8c: b #0x519e58
00519e90: mov r0, r5
00519e94: ldr r1, [sp, #0x10]
00519e98: mov r2, #0
00519e9c: mov r3, r6
00519ea0: bl #0x51889c
00519ea4: cmp r0, #0
00519ea8: bne #0x519e58
00519eac: mov r0, fp
00519eb0: mov r1, r5
00519eb4: mov r2, r6
00519eb8: bl #0x519b10
00519ebc: subs r4, r0, #0
00519ec0: beq #0x519f00
00519ec4: mov r1, r5
00519ec8: ldr ip, [r4]
00519ecc: mov r2, sb
00519ed0: mov r3, r6
00519ed4: mov lr, pc
00519ed8: ldr pc, [ip, #0xc]
00519edc: mov r1, r4
00519ee0: mov sl, r0
00519ee4: mov r0, fp
00519ee8: bl #0x515964
00519eec: b #0x519e38
00519ef0: mov r1, r4
00519ef4: mov r0, fp
00519ef8: bl #0x515964
00519efc: b #0x519e38
00519f00: mov r5, #0
00519f04: b #0x519e58
00519f08: subeq sl, r7, ip, lsr #26
00519f0c: eorseq r2, ip, r8, lsr #7
00519f10: eorseq r1, fp, ip, ror sl
00519f14: andeq r2, r0, r4, asr #10
00519f18: andeq r4, r0, r4, rrx

# 0x83186c _ZN21GLXPlayerWebComponent25IsNextResponseStringTokenEPKc
0083186c: ldr r3, [pc, #0x80]
00831870: ldr r2, [pc, #0x80]
00831874: push {r4, r5, r6, r7, lr}
00831878: add r3, pc, r3
0083187c: ldr r5, [r3, r2]
00831880: sub sp, sp, #0x10c
00831884: add r4, sp, #4
00831888: ldr ip, [r5]
0083188c: mov r7, r0
00831890: mov r6, r1
00831894: mov r2, #0x100
00831898: mov r1, #0
0083189c: mov r0, r4
008318a0: str ip, [sp, #0x104]
008318a4: bl #0x30e460
008318a8: mov r2, #0x100
008318ac: mov r0, r4
008318b0: mov r1, #0
008318b4: bl #0x82b364
008318b8: mov r1, r4
008318bc: mov r0, r7
008318c0: bl #0x831744
008318c4: mov r0, r6
008318c8: mov r1, r4
008318cc: bl #0x82b34c
008318d0: ldr r2, [sp, #0x104]
008318d4: ldr r3, [r5]
008318d8: rsbs r0, r0, #1
008318dc: movlo r0, #0
008318e0: cmp r2, r3
008318e4: bne #0x8318f0
008318e8: add sp, sp, #0x10c
008318ec: pop {r4, r5, r6, r7, pc}
008318f0: bl #0x30e310
008318f4: andseq r3, r6, r8, lsl r2
008318f8: andeq r4, r0, ip, lsr #1
