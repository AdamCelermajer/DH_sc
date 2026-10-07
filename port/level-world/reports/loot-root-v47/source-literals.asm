
# _ZNK9Character11GetCharTypeEv 0x3a3054
003a3054: push {r4, lr}
003a3058: bl #0x3a3024
003a305c: ldr r0, [r0, #0x38]
003a3060: pop {r4, pc}

# _ZN9Character11Ctrl_UseOOIEP10GameObject 0x3ad690
003ad690: push {r4, r5, r6, lr}
003ad694: ldr r3, [r0]
003ad698: mov r4, r0
003ad69c: mov r5, r1
003ad6a0: mov lr, pc
003ad6a4: ldr pc, [r3, #0x34]
003ad6a8: cmp r0, #0
003ad6ac: bne #0x3ad6e4
003ad6b0: cmp r5, #0
003ad6b4: beq #0x3ad6c8
003ad6b8: mov r0, r4
003ad6bc: mov r1, r5
003ad6c0: pop {r4, r5, r6, lr}
003ad6c4: b #0x3ad5ac
003ad6c8: movw r3, #0x14a4
003ad6cc: ldr r3, [r4, r3]
003ad6d0: cmp r3, #0
003ad6d4: beq #0x3ad6e4
003ad6d8: mov r0, r4
003ad6dc: pop {r4, r5, r6, lr}
003ad6e0: b #0x3ad614
003ad6e4: pop {r4, r5, r6, pc}

# _ZN18MenuMessageManagerI19CharMenuTutorialMsgLi1EED0Ev 0x329ec4
00329ec4: ldr r3, [pc, #0x44]
00329ec8: ldr r2, [pc, #0x44]
00329ecc: push {r4, r5, r6, lr}
00329ed0: add r3, pc, r3
00329ed4: ldr r2, [r3, r2]
00329ed8: mov r4, r0
00329edc: mov r6, r0
00329ee0: add r2, r2, #8
00329ee4: add r5, r0, #4
00329ee8: str r2, [r4], #0x2c
00329eec: sub r4, r4, #0x28
00329ef0: mov r0, r4
00329ef4: bl #0x329e10
00329ef8: cmp r5, r4
00329efc: bne #0x329eec
00329f00: mov r0, r6
00329f04: bl #0x310440
00329f08: mov r0, r6
00329f0c: pop {r4, r5, r6, pc}
00329f10: rsbeq sl, r6, r0, asr #23
00329f14: andeq r1, r0, r8, ror r1

# _ZN18MenuMessageManagerI11TutorialMsgLi1EED1Ev 0x329a54
00329a54: ldr r3, [pc, #0x80]
00329a58: ldr r2, [pc, #0x80]
00329a5c: push {r4, r5, r6, lr}
00329a60: add r3, pc, r3
00329a64: ldr r2, [r3, r2]
00329a68: mov r4, r0
00329a6c: mov r6, r0
00329a70: add r2, r2, #8
00329a74: add r5, r0, #4
00329a78: str r2, [r4], #0x2c
00329a7c: ldr r3, [r4, #-0x28]!
00329a80: ldr r1, [r4, #0x10]
00329a84: ldr r2, [r4, #8]
00329a88: ldr r0, [r4, #0xc]
00329a8c: cmp r1, r3
00329a90: beq #0x329ac4
00329a94: add r3, r3, #8
00329a98: cmp r3, r2
00329a9c: beq #0x329ab4
00329aa0: cmp r1, r3
00329aa4: add r3, r3, #8
00329aa8: beq #0x329ac4
00329aac: cmp r2, r3
00329ab0: bne #0x329aa0
00329ab4: ldr r3, [r0, #4]!
00329ab8: cmp r1, r3
00329abc: add r2, r3, #0x80
00329ac0: bne #0x329a94
00329ac4: mov r0, r4
00329ac8: bl #0x3299d0
00329acc: cmp r4, r5
00329ad0: bne #0x329a7c
00329ad4: mov r0, r6
00329ad8: pop {r4, r5, r6, pc}
00329adc: rsbeq fp, r6, r0, lsr r0
00329ae0: andeq r3, r0, r8, ror r1

# _ZN18MenuMessageManagerI19CharMenuTutorialMsgLi1EED1Ev 0x329e78
00329e78: ldr r3, [pc, #0x3c]
00329e7c: ldr r2, [pc, #0x3c]
00329e80: push {r4, r5, r6, lr}
00329e84: add r3, pc, r3
00329e88: ldr r2, [r3, r2]
00329e8c: mov r4, r0
00329e90: mov r6, r0
00329e94: add r2, r2, #8
00329e98: add r5, r0, #4
00329e9c: str r2, [r4], #0x2c
00329ea0: sub r4, r4, #0x28
00329ea4: mov r0, r4
00329ea8: bl #0x329e10
00329eac: cmp r5, r4
00329eb0: bne #0x329ea0
00329eb4: mov r0, r6
00329eb8: pop {r4, r5, r6, pc}
00329ebc: rsbeq sl, r6, ip, lsl #24
00329ec0: andeq r1, r0, r8, ror r1

# _ZN10ItemObject8InteractEP10GameObject 0x3ed144
003ed144: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ed148: ldr r4, [pc, #0x8e0]
003ed14c: ldr r5, [pc, #0x8e0]
003ed150: sub sp, sp, #0x184
003ed154: add r4, pc, r4
003ed158: ldr r3, [r4, r5]
003ed15c: mov r8, r1
003ed160: mov r6, r0
003ed164: ldr r3, [r3]
003ed168: str r3, [sp, #0x17c]
003ed16c: bl #0x3ebffc
003ed170: cmp r0, #0
003ed174: beq #0x3ed194
003ed178: ldr r3, [r4, r5]
003ed17c: ldr r2, [sp, #0x17c]
003ed180: ldr r3, [r3]
003ed184: cmp r2, r3
003ed188: bne #0x3ed8ec
003ed18c: add sp, sp, #0x184
003ed190: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ed194: add r7, sp, #0x3c
003ed198: mov r0, r7
003ed19c: mov r1, r8
003ed1a0: bl #0x33dd2c
003ed1a4: mov r0, r7
003ed1a8: bl #0x33ff54
003ed1ac: subs r7, r0, #0
003ed1b0: beq #0x3ed178
003ed1b4: ldr r3, [r6, #0x3bc]
003ed1b8: cmp r3, #0
003ed1bc: beq #0x3ed1c8
003ed1c0: cmp r7, r3
003ed1c4: bne #0x3ed178
003ed1c8: mov r3, #0x3c0
003ed1cc: ldrsh r8, [r6, r3]
003ed1d0: cmn r8, #1
003ed1d4: beq #0x3ed1fc
003ed1d8: ldr r3, [pc, #0x858]
003ed1dc: mov r1, r7
003ed1e0: mov r2, #0
003ed1e4: ldr r3, [r4, r3]
003ed1e8: ldr r0, [r3, #0x40]
003ed1ec: bl #0x36eea8
003ed1f0: ldr r3, [r0, #0x678]
003ed1f4: cmp r8, r3
003ed1f8: beq #0x3ed654
003ed1fc: ldr r3, [r7]
003ed200: mov r0, r7
003ed204: mov lr, pc
003ed208: ldr pc, [r3, #0x28]
003ed20c: cmp r0, #0
003ed210: beq #0x3ed178
003ed214: add r1, r6, #0x374
003ed218: str r1, [sp, #0x20]
003ed21c: ldr r0, [sp, #0x20]
003ed220: mov r1, #0
003ed224: bl #0x3fc61c
003ed228: ldr fp, [pc, #0x808]
003ed22c: ldr r1, [pc, #0x808]
003ed230: mov r8, r0
003ed234: ldr sl, [r4, fp]
003ed238: add r1, pc, r1
003ed23c: mov r0, sl
003ed240: bl #0x320e44
003ed244: str r0, [sp, #0x1c]
003ed248: mov r0, r8
003ed24c: bl #0x3f9e08
003ed250: ldr sb, [r0, #0x68]
003ed254: mov r0, r8
003ed258: bl #0x3f9e08
003ed25c: cmn sb, #1
003ed260: addeq r2, r7, #0x37c
003ed264: ldr sb, [r0, #0x58]
003ed268: streq r2, [sp, #0x1c]
003ed26c: bne #0x3ed668
003ed270: cmp sb, #0xe
003ed274: beq #0x3ed8f0
003ed278: ldr r3, [pc, #0x7c0]
003ed27c: add r1, sp, #0x124
003ed280: mov r0, r8
003ed284: ldr r3, [r4, r3]
003ed288: str r1, [sp, #0x28]
003ed28c: add sb, sp, #0x16c
003ed290: ldr r2, [r3]
003ed294: add sl, sp, #0x10c
003ed298: str r2, [sp, #0x18]
003ed29c: bl #0x3fa6cc
003ed2a0: ldr r2, [sp, #0x18]
003ed2a4: mov r3, #0xc
003ed2a8: ldr r1, [pc, #0x794]
003ed2ac: mla r3, r3, r0, r2
003ed2b0: add r1, pc, r1
003ed2b4: ldr r2, [r3, #8]
003ed2b8: mov r0, sb
003ed2bc: bl #0x30eae4
003ed2c0: mov r1, sb
003ed2c4: add r2, sp, #0x54
003ed2c8: ldr r0, [sp, #0x28]
003ed2cc: bl #0x3140ec
003ed2d0: ldr r3, [sp, #0x138]
003ed2d4: ldr r1, [sp, #0x134]
003ed2d8: mov r0, sl
003ed2dc: str sl, [sp, #0x11c]
003ed2e0: rsb r1, r3, r1
003ed2e4: add r1, r1, #0xf
003ed2e8: str sl, [sp, #0x120]
003ed2ec: bl #0x31167c
003ed2f0: ldr r1, [pc, #0x750]
003ed2f4: ldr r3, [sp, #0x11c]
003ed2f8: mov ip, #0
003ed2fc: add r1, pc, r1
003ed300: strb ip, [r3]
003ed304: add r2, r1, #0xe
003ed308: add r3, sp, #0x48
003ed30c: mov r0, sl
003ed310: str ip, [sp, #0x14]
003ed314: bl #0x32a78c
003ed318: mov r0, sl
003ed31c: ldr r1, [sp, #0x138]
003ed320: ldr r2, [sp, #0x134]
003ed324: bl #0x310804
003ed328: add r2, sp, #0xdc
003ed32c: str r2, [sp, #0x24]
003ed330: ldr r2, [pc, #0x714]
003ed334: add r3, sp, #0xf4
003ed338: mov r0, r3
003ed33c: mov r1, sl
003ed340: add r2, pc, r2
003ed344: str r3, [sp, #0x18]
003ed348: bl #0x3338cc
003ed34c: ldr r1, [r8, #0x1c]
003ed350: add r2, sp, #0x50
003ed354: ldr r0, [sp, #0x24]
003ed358: bl #0x3140ec
003ed35c: ldr r3, [sp, #0x18]
003ed360: add sb, sp, #0xc4
003ed364: mov r0, sb
003ed368: mov r1, r3
003ed36c: ldr r2, [sp, #0x24]
003ed370: bl #0x3ecf68
003ed374: ldr r2, [pc, #0x6d4]
003ed378: add r1, sp, #0x13c
003ed37c: str r1, [sp, #0x2c]
003ed380: add r2, pc, r2
003ed384: mov r1, sb
003ed388: ldr r0, [sp, #0x2c]
003ed38c: bl #0x3338cc
003ed390: mov r0, sb
003ed394: bl #0x3139ac
003ed398: ldr r0, [sp, #0x24]
003ed39c: bl #0x3139ac
003ed3a0: ldr r3, [sp, #0x18]
003ed3a4: mov r0, r3
003ed3a8: bl #0x3139ac
003ed3ac: mov r0, sl
003ed3b0: bl #0x3139ac
003ed3b4: add sl, sp, #0x58
003ed3b8: ldr r0, [sp, #0x28]
003ed3bc: bl #0x3139ac
003ed3c0: mov r0, sl
003ed3c4: mov r1, #0x10
003ed3c8: str sl, [sp, #0x68]
003ed3cc: str sl, [sp, #0x6c]
003ed3d0: bl #0x31167c
003ed3d4: ldr ip, [sp, #0x14]
003ed3d8: ldr r3, [sp, #0x68]
003ed3dc: mov r0, sl
003ed3e0: strb ip, [r3]
003ed3e4: ldr r1, [sp, #0x150]
003ed3e8: ldr r2, [sp, #0x14c]
003ed3ec: str ip, [sp, #0x14]
003ed3f0: bl #0x3109e0
003ed3f4: ldr r3, [r4, fp]
003ed3f8: ldr ip, [sp, #0x14]
003ed3fc: mov r2, #1
003ed400: ldr r0, [r3, #0x40]
003ed404: mov r1, ip
003ed408: bl #0x36e478
003ed40c: ldr r3, [r0, #0x660]
003ed410: cmp r7, r3
003ed414: beq #0x3eda24
003ed418: mov r0, r8
003ed41c: bl #0x3f9e08
003ed420: ldr r3, [r0, #0x58]
003ed424: cmp r3, #0xd
003ed428: beq #0x3ed43c
003ed42c: bl #0x7fd794
003ed430: ldrb r3, [r0, #5]
003ed434: cmp r3, #0
003ed438: beq #0x3ed9c4
003ed43c: add r3, r7, #0x560
003ed440: str r3, [sp, #0x24]
003ed444: ldr r3, [pc, #0x608]
003ed448: add r8, sp, #0xac
003ed44c: ldr sb, [r4, r3]
003ed450: mov r0, sb
003ed454: bl #0x337888
003ed458: ldr r1, [pc, #0x5f8]
003ed45c: add r2, sp, #0x4c
003ed460: mov r0, r8
003ed464: add r1, pc, r1
003ed468: bl #0x3140ec
003ed46c: mov r1, r8
003ed470: mov r0, sb
003ed474: bl #0x337a88
003ed478: mov r0, r8
003ed47c: bl #0x3139ac
003ed480: mov r3, #1
003ed484: ldr r0, [sp, #0x20]
003ed488: ldr r1, [sp, #0x1c]
003ed48c: mov r2, #0
003ed490: bl #0x3ffa68
003ed494: ldr r0, [sp, #0x24]
003ed498: mov r1, #0xdf
003ed49c: mov r2, #1
003ed4a0: bl #0x3e0798
003ed4a4: ldr r3, [pc, #0x5b0]
003ed4a8: ldr r0, [sp, #0x24]
003ed4ac: mov r1, #0xdf
003ed4b0: ldr r3, [r4, r3]
003ed4b4: mov r2, #0
003ed4b8: ldr r3, [r3]
003ed4bc: str r3, [sp, #0x24]
003ed4c0: bl #0x3df6e0
003ed4c4: cmp r0, #0x12c
003ed4c8: bge #0x3ed918
003ed4cc: mov r0, sl
003ed4d0: bl #0x3139ac
003ed4d4: ldr r0, [sp, #0x2c]
003ed4d8: bl #0x3139ac
003ed4dc: mov r0, r6
003ed4e0: bl #0x3ebffc
003ed4e4: cmp r0, #0
003ed4e8: beq #0x3ed178
003ed4ec: bl #0x7fd794
003ed4f0: ldrb r3, [r0, #5]
003ed4f4: cmp r3, #0
003ed4f8: beq #0x3ed54c
003ed4fc: ldr r3, [r7, #0x378]
003ed500: ldrb r3, [r3, #0xa]
003ed504: cmp r3, #0
003ed508: beq #0x3ed54c
003ed50c: bl #0x80b1bc
003ed510: mov r8, r0
003ed514: ldr r0, [pc, #0x544]
003ed518: mov r1, #1
003ed51c: ldr sl, [r6, #0x108]
003ed520: add r0, pc, r0
003ed524: ldrb sb, [r7, #0x108]
003ed528: bl #0x80a244
003ed52c: uxth sl, sl
003ed530: mov r3, #5
003ed534: mov r1, r0
003ed538: strb sb, [r0, #0x54]
003ed53c: strb r3, [r0, #0x50]
003ed540: strh sl, [r0, #0x52]
003ed544: mov r0, r8
003ed548: bl #0x80e2a4
003ed54c: movw r3, #0x3b6
003ed550: ldrsh r1, [r6, r3]
003ed554: ldr r3, [pc, #0x508]
003ed558: ldr lr, [r6, #0x164]
003ed55c: ldr r8, [r6, #0x168]
003ed560: ldr r3, [r4, r3]
003ed564: ldr sl, [r6, #0x160]
003ed568: mov ip, #0xbf000000
003ed56c: ldr r0, [r3]
003ed570: add ip, ip, #0x800000
003ed574: str lr, [sp, #0x34]
003ed578: add r2, sp, #0x30
003ed57c: mov lr, #1
003ed580: mov r3, #0
003ed584: str r8, [sp, #0x38]
003ed588: str sl, [sp, #0x30]
003ed58c: str lr, [sp]
003ed590: str ip, [sp, #8]
003ed594: str ip, [sp, #4]
003ed598: bl #0x36b5d8
003ed59c: ldr r8, [r6, #0x3c8]
003ed5a0: cmp r8, #0
003ed5a4: beq #0x3ed5c0
003ed5a8: mov r0, r8
003ed5ac: bl #0x498dec
003ed5b0: mov r0, r8
003ed5b4: bl #0x310440
003ed5b8: mov r3, #0
003ed5bc: str r3, [r6, #0x3c8]
003ed5c0: mov r0, r6
003ed5c4: bl #0x3ebccc
003ed5c8: movw r3, #0x14a4
003ed5cc: ldr r2, [r7, r3]
003ed5d0: ldr r0, [pc, #0x490]
003ed5d4: cmp r6, r2
003ed5d8: moveq r2, #0
003ed5dc: streq r2, [r7, r3]
003ed5e0: add r0, pc, r0
003ed5e4: ldr r8, [r0]
003ed5e8: ands r8, r8, #1
003ed5ec: beq #0x3ed874
003ed5f0: ldr r3, [pc, #0x474]
003ed5f4: mov r8, #0
003ed5f8: add r2, r7, #0x160
003ed5fc: add r3, pc, r3
003ed600: ldr r1, [r3, #4]
003ed604: ldr r3, [pc, #0x464]
003ed608: str r8, [sp]
003ed60c: ldr r0, [r4, r3]
003ed610: mov r3, r8
003ed614: bl #0x495d14
003ed618: ldr r3, [pc, #0x454]
003ed61c: mov r1, r6
003ed620: ldr r0, [r4, r3]
003ed624: bl #0x3eac34
003ed628: ldr r3, [r4, fp]
003ed62c: mov r1, r7
003ed630: mov r2, r8
003ed634: ldr r0, [r3, #0x40]
003ed638: bl #0x36eea8
003ed63c: ldr r3, [pc, #0x434]
003ed640: ldr r2, [r0, #0x678]
003ed644: mov r1, #4
003ed648: ldr r0, [r4, r3]
003ed64c: bl #0x3790ec
003ed650: b #0x3ed178
003ed654: mov r3, #0x3b8
003ed658: ldrsh r3, [r6, r3]
003ed65c: cmp r3, #0
003ed660: bgt #0x3ed178
003ed664: b #0x3ed1fc
003ed668: mov r0, r8
003ed66c: bl #0x3f9e80
003ed670: ldr r3, [sp, #0x1c]
003ed674: cmp r0, r3
003ed678: blo #0x3ed70c
003ed67c: add r2, r7, #0x37c
003ed680: mov r0, r2
003ed684: str r2, [sp, #0x1c]
003ed688: bl #0x3fe330
003ed68c: cmp r0, #0
003ed690: beq #0x3ed270
003ed694: add r8, sp, #0x74
003ed698: mov r0, r8
003ed69c: mov r1, #0x10
003ed6a0: str r8, [sp, #0x84]
003ed6a4: str r8, [sp, #0x88]
003ed6a8: bl #0x31167c
003ed6ac: ldr r3, [sp, #0x84]
003ed6b0: mov r2, #0
003ed6b4: ldr r1, [pc, #0x3c0]
003ed6b8: strb r2, [r3]
003ed6bc: ldr r2, [pc, #0x3bc]
003ed6c0: ldr r0, [sl, #0x2c]
003ed6c4: add r1, pc, r1
003ed6c8: add r2, pc, r2
003ed6cc: ldr sl, [sl, #0x34]
003ed6d0: bl #0x4c4bdc
003ed6d4: mov r1, r0
003ed6d8: mov r0, sl
003ed6dc: bl #0x508edc
003ed6e0: mov sl, r0
003ed6e4: bl #0x30de54
003ed6e8: mov r1, sl
003ed6ec: add r2, sl, r0
003ed6f0: mov r0, r8
003ed6f4: bl #0x3109e0
003ed6f8: mov r0, r8
003ed6fc: bl #0x3ecff8
003ed700: mov r0, r8
003ed704: bl #0x3139ac
003ed708: b #0x3ed4dc
003ed70c: add sb, sp, #0x154
003ed710: mov r0, sb
003ed714: mov r1, #0x10
003ed718: str sb, [sp, #0x164]
003ed71c: str sb, [sp, #0x168]
003ed720: bl #0x31167c
003ed724: ldr r3, [sp, #0x164]
003ed728: mov r2, #0
003ed72c: ldr r1, [pc, #0x350]
003ed730: strb r2, [r3]
003ed734: ldr r2, [pc, #0x34c]
003ed738: ldr r3, [sl, #0x34]
003ed73c: add r1, pc, r1
003ed740: add r2, pc, r2
003ed744: ldr r0, [sl, #0x2c]
003ed748: str r3, [sp, #0x18]
003ed74c: bl #0x4c4bdc
003ed750: ldr r3, [sp, #0x18]
003ed754: mov r1, r0
003ed758: mov r0, r3
003ed75c: bl #0x508edc
003ed760: ldr r3, [pc, #0x2d8]
003ed764: str r0, [sp, #0x1c]
003ed768: mov r0, r8
003ed76c: ldr r3, [r4, r3]
003ed770: ldr r3, [r3]
003ed774: str r3, [sp, #0x28]
003ed778: bl #0x3fa6cc
003ed77c: mov r1, r8
003ed780: str r0, [sp, #0x24]
003ed784: mov r2, #1
003ed788: mov r0, r7
003ed78c: bl #0x3a4a3c
003ed790: ldr r1, [sp, #0x1c]
003ed794: mov ip, r0
003ed798: cmp r1, #0
003ed79c: beq #0x3ed7f8
003ed7a0: ldr r2, [sp, #0x24]
003ed7a4: ldr r1, [sp, #0x28]
003ed7a8: mov r3, #0xc
003ed7ac: mla r3, r3, r2, r1
003ed7b0: ldr r1, [pc, #0x2d4]
003ed7b4: ldr r2, [r3, #8]
003ed7b8: add r3, sp, #0x16c
003ed7bc: add r1, pc, r1
003ed7c0: bic r2, r2, #0xff000000
003ed7c4: mov r0, r3
003ed7c8: str r3, [sp, #0x18]
003ed7cc: str ip, [sp, #0x14]
003ed7d0: bl #0x30eae4
003ed7d4: ldr lr, [r8, #0x1c]
003ed7d8: ldr ip, [sp, #0x14]
003ed7dc: ldr r0, [sl, #0x34]
003ed7e0: ldr r2, [sp, #0x1c]
003ed7e4: ldr r3, [sp, #0x18]
003ed7e8: mov r1, sb
003ed7ec: str lr, [sp]
003ed7f0: str ip, [sp, #4]
003ed7f4: bl #0x508ef4
003ed7f8: add r8, sp, #0x90
003ed7fc: mov r0, r8
003ed800: mov r1, #0x10
003ed804: str r8, [sp, #0xa0]
003ed808: str r8, [sp, #0xa4]
003ed80c: bl #0x31167c
003ed810: ldr r3, [sp, #0xa0]
003ed814: mov sl, #0
003ed818: mov r0, r8
003ed81c: strb sl, [r3]
003ed820: ldr r1, [sp, #0x168]
003ed824: ldr r2, [sp, #0x164]
003ed828: bl #0x3109e0
003ed82c: mov r0, r8
003ed830: bl #0x3ecff8
003ed834: mov ip, #1
003ed838: mov r3, sl
003ed83c: mov r1, sl
003ed840: add r2, r7, #0x37c
003ed844: ldr r0, [sp, #0x20]
003ed848: str ip, [sp]
003ed84c: bl #0x3ffa44
003ed850: mov r2, sl
003ed854: mov r1, r0
003ed858: mov r0, r7
003ed85c: bl #0x3a4bec
003ed860: mov r0, r8
003ed864: bl #0x3139ac
003ed868: mov r0, sb
003ed86c: bl #0x3139ac
003ed870: b #0x3ed4dc
003ed874: bl #0x30e76c
003ed878: cmp r0, #0
003ed87c: beq #0x3ed5f0
003ed880: ldr r3, [pc, #0x208]
003ed884: ldr r3, [r4, r3]
003ed888: ldr sl, [r3]
003ed88c: cmp sl, #0
003ed890: beq #0x3ed910
003ed894: ldr r3, [pc, #0x1f8]
003ed898: ldr sb, [pc, #0x1f8]
003ed89c: str r7, [sp, #0x20]
003ed8a0: ldr r3, [r4, r3]
003ed8a4: add sb, pc, sb
003ed8a8: ldr r3, [r3]
003ed8ac: mov r7, r3
003ed8b0: b #0x3ed8c0
003ed8b4: add r8, r8, #1
003ed8b8: cmp r8, sl
003ed8bc: beq #0x3ed90c
003ed8c0: mov r0, sb
003ed8c4: ldr r1, [r7, r8, lsl #2]
003ed8c8: bl #0x30e31c
003ed8cc: cmp r0, #0
003ed8d0: bne #0x3ed8b4
003ed8d4: ldr r7, [sp, #0x20]
003ed8d8: ldr r0, [pc, #0x1bc]
003ed8dc: add r0, pc, r0
003ed8e0: str r8, [r0, #4]
003ed8e4: bl #0x30ea3c
003ed8e8: b #0x3ed5f0
003ed8ec: bl #0x30e310
003ed8f0: ldr r0, [sp, #0x1c]
003ed8f4: bl #0x3fc690
003ed8f8: ldrb r3, [r7, #0x3a8]
003ed8fc: sxtb r3, r3
003ed900: cmp r0, r3
003ed904: bge #0x3ed4dc
003ed908: b #0x3ed278
003ed90c: ldr r7, [sp, #0x20]
003ed910: mvn r8, #0
003ed914: b #0x3ed8d8
003ed918: ldr r3, [r7]
003ed91c: mov r0, r7
003ed920: mov lr, pc
003ed924: ldr pc, [r3, #0x28]
003ed928: cmp r0, #0
003ed92c: beq #0x3ed4cc
003ed930: ldr r3, [r4, fp]
003ed934: mov r1, r7
003ed938: ldr r0, [r3, #0x40]
003ed93c: bl #0x36effc
003ed940: cmp r0, #0
003ed944: beq #0x3ed4cc
003ed948: ldr r3, [pc, #0x150]
003ed94c: ldr r3, [r4, r3]
003ed950: ldr sb, [r3]
003ed954: cmp sb, #0
003ed958: beq #0x3ed9bc
003ed95c: ldr r3, [pc, #0x140]
003ed960: ldr r2, [pc, #0x140]
003ed964: str r7, [sp, #0x1c]
003ed968: ldr r3, [r4, r3]
003ed96c: add r2, pc, r2
003ed970: mov r8, #0
003ed974: ldr r3, [r3]
003ed978: str r2, [sp, #0x20]
003ed97c: mov r7, r3
003ed980: b #0x3ed990
003ed984: add r8, r8, #1
003ed988: cmp r8, sb
003ed98c: beq #0x3ed9b8
003ed990: ldr r0, [sp, #0x20]
003ed994: ldr r1, [r7, r8, lsl #2]
003ed998: bl #0x30e31c
003ed99c: cmp r0, #0
003ed9a0: bne #0x3ed984
003ed9a4: ldr r7, [sp, #0x1c]
003ed9a8: mov r1, r8
003ed9ac: ldr r0, [sp, #0x24]
003ed9b0: bl #0x3813b8
003ed9b4: b #0x3ed4cc
003ed9b8: ldr r7, [sp, #0x1c]
003ed9bc: mvn r1, #0
003ed9c0: b #0x3ed9ac
003ed9c4: mov r0, r7
003ed9c8: bl #0x3bb8e4
003ed9cc: subs sb, r0, #0
003ed9d0: bne #0x3ed43c
003ed9d4: ldr r3, [r4, fp]
003ed9d8: ldr r3, [r3, #0x4c]
003ed9dc: ldrb r3, [r3, #0x2a]
003ed9e0: cmp r3, #0
003ed9e4: beq #0x3ed43c
003ed9e8: ldr r3, [pc, #0xbc]
003ed9ec: ldr r1, [pc, #0xbc]
003ed9f0: mov r2, #1
003ed9f4: ldr r8, [r4, r3]
003ed9f8: add r1, pc, r1
003ed9fc: mov r0, r8
003eda00: bl #0x4591f0
003eda04: cmn r0, #1
003eda08: mov r1, r0
003eda0c: beq #0x3ed43c
003eda10: mov r0, r8
003eda14: mov r3, sb
003eda18: mvn r2, #0
003eda1c: bl #0x4605c0
003eda20: b #0x3ed43c
003eda24: mov r0, sl
003eda28: bl #0x3ecff8
003eda2c: b #0x3ed418
003eda30: subseq r7, sl, ip, lsr sb
003eda34: andeq r4, r0, ip, lsr #1
003eda38: strdeq r3, r4, [r0], -r4
003eda3c: subeq sb, sp, r8, lsr #1
003eda40: andeq r1, r0, r4, asr #4
003eda44: subeq sb, sp, r0, lsl #2
003eda48: subeq sb, sp, r4, lsr r0
003eda4c: subeq sb, sp, r0
003eda50: subeq r8, sp, r8, asr #31
003eda54: andeq r0, r0, r4, lsl #17
003eda58: subeq r8, sp, ip, lsl #30
003eda5c: andeq r1, r0, r0, ror sp
003eda60: subeq r1, sp, r8, ror #19
003eda64: andeq r0, r0, r4, lsr #27
003eda68: ldrheq r5, [fp], #-0xa0

# _ZN9Character20SG_GetGameDifficultyEv 0x3bb8e4
003bb8e4: movw r3, #0x14e8
003bb8e8: ldr r2, [r0, r3]
003bb8ec: ldr r3, [pc, #0x1c]
003bb8f0: cmp r2, #0
003bb8f4: add r3, pc, r3
003bb8f8: mvneq r0, #0
003bb8fc: bxeq lr
003bb900: ldr r2, [pc, #0xc]
003bb904: ldr r3, [r3, r2]
003bb908: ldr r0, [r3]
003bb90c: bx lr

# _ZN12v2Controller10Cmd_UseOOIEP10GameObject 0x4057fc
004057fc: push {r4, r5, r6, r7, r8, lr}
00405800: ldrb r2, [r0, #9]
00405804: ldr r3, [pc, #0x13c]
00405808: mov r4, r0
0040580c: cmp r2, #0
00405810: mov r5, r1
00405814: add r3, pc, r3
00405818: bne #0x405840
0040581c: ldr r2, [pc, #0x128]
00405820: ldr r3, [r3, r2]
00405824: ldrb r3, [r3]
00405828: cmp r3, #0
0040582c: beq #0x405834
00405830: pop {r4, r5, r6, r7, r8, pc}
00405834: ldrb r3, [r0, #8]
00405838: cmp r3, #0
0040583c: bne #0x405830
00405840: bl #0x7fd794
00405844: ldrb r3, [r0, #5]
00405848: cmp r3, #0
0040584c: beq #0x405904
00405850: ldrb r3, [r4, #0xa]
00405854: cmp r3, #0
00405858: beq #0x405904
0040585c: ldr r7, [r4, #0xc]
00405860: cmp r7, #0
00405864: beq #0x405904
00405868: cmp r5, #0
0040586c: movne r6, r5
00405870: beq #0x405934
00405874: ldr r3, [r6]
00405878: mov r0, r6
0040587c: mov lr, pc
00405880: ldr pc, [r3, #0x24]
00405884: cmp r0, #0
00405888: bne #0x405920
0040588c: ldr r3, [r6]
00405890: mov r0, r6
00405894: ldr r1, [r4, #0xc]
00405898: mov lr, pc
0040589c: ldr pc, [r3, #0x90]
004058a0: cmp r0, #1
004058a4: beq #0x405904
004058a8: ldr r3, [r6]
004058ac: mov r0, r6
004058b0: ldr r1, [r4, #0xc]
004058b4: mov lr, pc
004058b8: ldr pc, [r3, #0x90]
004058bc: cmp r0, #8
004058c0: beq #0x405904
004058c4: bl #0x80b1bc
004058c8: mov r8, r0
004058cc: ldr r0, [pc, #0x7c]
004058d0: mov r1, #1
004058d4: ldr r6, [r6, #0x108]
004058d8: add r0, pc, r0
004058dc: ldrb r7, [r7, #0x108]
004058e0: bl #0x80a244
004058e4: uxth r6, r6
004058e8: mov r3, #5
004058ec: mov r1, r0
004058f0: strb r7, [r0, #0x54]
004058f4: strb r3, [r0, #0x50]
004058f8: strh r6, [r0, #0x52]
004058fc: mov r0, r8
00405900: bl #0x80e2a4
00405904: ldr r3, [r4, #4]
00405908: mov r1, r5
0040590c: mov r0, r3
00405910: ldr r3, [r3]
00405914: mov lr, pc
00405918: ldr pc, [r3, #0x50]
0040591c: pop {r4, r5, r6, r7, r8, pc}
00405920: mov r0, r6
00405924: bl #0x3a30c4
00405928: cmp r0, #0
0040592c: bne #0x405904
00405930: b #0x40588c
00405934: movw r3, #0x14a4
00405938: ldr r6, [r7, r3]
0040593c: cmp r6, #0
00405940: beq #0x405904
00405944: b #0x405874
00405948: subseq pc, r8, ip, ror r2
0040594c: andeq r3, r0, r0, asr r6
00405950: subeq sb, fp, r0, lsr r6

# _ZN18MenuMessageManagerI11TutorialMsgLi1EED0Ev 0x329ae4
00329ae4: ldr r3, [pc, #0x88]
00329ae8: ldr r2, [pc, #0x88]
00329aec: push {r4, r5, r6, lr}
00329af0: add r3, pc, r3
00329af4: ldr r2, [r3, r2]
00329af8: mov r4, r0
00329afc: mov r6, r0
00329b00: add r2, r2, #8
00329b04: add r5, r0, #4
00329b08: str r2, [r4], #0x2c
00329b0c: ldr r3, [r4, #-0x28]!
00329b10: ldr r1, [r4, #0x10]
00329b14: ldr r2, [r4, #8]
00329b18: ldr r0, [r4, #0xc]
00329b1c: cmp r1, r3
00329b20: beq #0x329b54
00329b24: add r3, r3, #8
00329b28: cmp r3, r2
00329b2c: beq #0x329b44
00329b30: cmp r1, r3
00329b34: add r3, r3, #8
00329b38: beq #0x329b54
00329b3c: cmp r2, r3
00329b40: bne #0x329b30
00329b44: ldr r3, [r0, #4]!
00329b48: cmp r1, r3
00329b4c: add r2, r3, #0x80
00329b50: bne #0x329b24
00329b54: mov r0, r4
00329b58: bl #0x3299d0
00329b5c: cmp r4, r5
00329b60: bne #0x329b0c
00329b64: mov r0, r6
00329b68: bl #0x310440
00329b6c: mov r0, r6
00329b70: pop {r4, r5, r6, pc}
00329b74: rsbeq sl, r6, r0, lsr #31
00329b78: andeq r3, r0, r8, ror r1

# _ZN15SavegameManager15__saveTutorialsEP11IStreamBasePv 0x46c798
0046c798: push {r4, lr}
0046c79c: mov r2, #0xe
0046c7a0: ldr ip, [r0]
0046c7a4: mov r3, #0
0046c7a8: add r1, r1, #0x29
0046c7ac: mov lr, pc
0046c7b0: ldr pc, [ip, #0x1c]
0046c7b4: pop {r4, pc}

# _ZN9Character6UseOOIEv 0x3ad614
003ad614: push {r4, r5, r6, lr}
003ad618: movw r3, #0x14a4
003ad61c: ldr r3, [r0, r3]
003ad620: mov r4, r0
003ad624: cmp r3, #0
003ad628: beq #0x3ad638
003ad62c: ldr r1, [r0, #0x408]
003ad630: cmp r1, #0
003ad634: beq #0x3ad63c
003ad638: pop {r4, r5, r6, pc}
003ad63c: add r5, r0, #0x4f0
003ad640: add r5, r5, #0xc
003ad644: mov r0, r5
003ad648: bl #0x3c0260
003ad64c: subs r1, r0, #0
003ad650: beq #0x3ad674
003ad654: movw r3, #0x14a4
003ad658: ldr r1, [r4, r3]
003ad65c: add r0, r4, #0x3c8
003ad660: mov r2, #0
003ad664: bl #0x3d6890
003ad668: mov r3, #1
003ad66c: strb r3, [r4, #0x412]
003ad670: b #0x3ad638
003ad674: mov r0, r5
003ad678: bl #0x3c029c
003ad67c: cmp r0, #0
003ad680: beq #0x3ad638
003ad684: b #0x3ad654

# _ZNK9Character8IsPlayerEv 0x3a49f0
003a49f0: push {r4, r5, r6, lr}
003a49f4: mov r5, r0
003a49f8: bl #0x3a3054
003a49fc: cmp r0, #0
003a4a00: beq #0x3a4a14
003a4a04: cmp r0, #1
003a4a08: movne r0, #0
003a4a0c: moveq r0, #1
003a4a10: pop {r4, r5, r6, pc}
003a4a14: ldr r4, [r5, #0x44]
003a4a18: ldr r1, [pc, #0x18]
003a4a1c: mov r0, r4
003a4a20: add r1, pc, r1
003a4a24: bl #0x30ebd4
003a4a28: cmp r4, r0
003a4a2c: movne r0, #0
003a4a30: moveq r0, #1
003a4a34: pop {r4, r5, r6, pc}
003a4a38: subseq lr, r1, r8, asr #14

# _ZN10GameObject7_IsDeadERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ea48
0038ea48: push {r4, lr}
0038ea4c: ldr r3, [r2]
0038ea50: mov r0, r2
0038ea54: mov r4, r1
0038ea58: mov lr, pc
0038ea5c: ldr pc, [r3, #0x34]
0038ea60: mov r3, r0
0038ea64: mov r1, r3
0038ea68: mov r0, r4
0038ea6c: pop {r4, lr}
0038ea70: b #0x37c7e4

# _ZN9Character11ForceUseOOIEP10GameObject 0x3ad5ac
003ad5ac: push {r4, r5, r6, lr}
003ad5b0: subs r5, r1, #0
003ad5b4: mov r4, r0
003ad5b8: beq #0x3ad5c8
003ad5bc: ldr r1, [r0, #0x408]
003ad5c0: cmp r1, #0
003ad5c4: beq #0x3ad5cc
003ad5c8: pop {r4, r5, r6, pc}
003ad5cc: add r6, r0, #0x4f0
003ad5d0: add r6, r6, #0xc
003ad5d4: mov r0, r6
003ad5d8: bl #0x3c0260
003ad5dc: subs r1, r0, #0
003ad5e0: beq #0x3ad600
003ad5e4: mov r1, r5
003ad5e8: add r0, r4, #0x3c8
003ad5ec: mov r2, #0
003ad5f0: bl #0x3d6890
003ad5f4: mov r3, #1
003ad5f8: strb r3, [r4, #0x412]
003ad5fc: b #0x3ad5c8
003ad600: mov r0, r6
003ad604: bl #0x3c029c
003ad608: cmp r0, #0
003ad60c: beq #0x3ad5c8
003ad610: b #0x3ad5e4

# _ZNK10GameObject6IsDeadEv 0x3400ac
003400ac: mov r0, #0
003400b0: bx lr

# _ZN15SavegameManager15__loadTutorialsEP11IStreamBasePv 0x46c778
0046c778: push {r4, lr}
0046c77c: mov r2, #0xe
0046c780: ldr ip, [r0]
0046c784: mov r3, #0
0046c788: add r1, r1, #0x29
0046c78c: mov lr, pc
0046c790: ldr pc, [ip, #0x18]
0046c794: pop {r4, pc}

# _ZN10GameObject12_IsCharacterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x38ea1c
0038ea1c: push {r4, lr}
0038ea20: ldr r3, [r2]
0038ea24: mov r0, r2
0038ea28: mov r4, r1
0038ea2c: mov lr, pc
0038ea30: ldr pc, [r3, #0x24]
0038ea34: mov r3, r0
0038ea38: mov r1, r3
0038ea3c: mov r0, r4
0038ea40: pop {r4, lr}
0038ea44: b #0x37c7e4

# _ZN18MenuMessageManagerI11TutorialMsgLi1EE21FlushEnqueuedMessagesEi.clone.26 0x3f2274
003f2274: push {r4, r5, r6, lr}
003f2278: ldr r5, [pc, #0x7c]
003f227c: ldr r6, [pc, #0x7c]
003f2280: add r5, pc, r5
003f2284: ldr r3, [r5, r6]
003f2288: mov r4, r3
003f228c: ldr r3, [r3, #4]
003f2290: ldr r2, [r4, #0x14]
003f2294: cmp r3, r2
003f2298: beq #0x3f22f8
003f229c: ldr r2, [r4, #0xc]
003f22a0: sub r2, r2, #8
003f22a4: cmp r3, r2
003f22a8: addne r3, r3, #8
003f22ac: strne r3, [r4, #4]
003f22b0: bne #0x3f2290
003f22b4: ldr r0, [r4, #8]
003f22b8: mov r1, #0x80
003f22bc: cmp r0, #0
003f22c0: beq #0x3f22c8
003f22c4: bl #0x708f00
003f22c8: ldr r2, [r5, r6]
003f22cc: ldr r3, [r2, #0x10]
003f22d0: add r1, r3, #4
003f22d4: str r1, [r2, #0x10]
003f22d8: ldr r3, [r3, #4]
003f22dc: add r1, r3, #0x80
003f22e0: str r1, [r2, #0xc]
003f22e4: str r3, [r2, #4]
003f22e8: str r3, [r2, #8]
003f22ec: ldr r2, [r4, #0x14]
003f22f0: cmp r3, r2
003f22f4: bne #0x3f229c
003f22f8: pop {r4, r5, r6, pc}
003f22fc: subseq r2, sl, r0, lsl r8
003f2300: andeq r1, r0, r0, lsl ip

# _ZNK18MenuMessageManagerI19CharMenuTutorialMsgLi1EE6InvokeEPKci.clone.50 0x442734
00442734: push {r4, r5, r6, r7, lr}
00442738: sub sp, sp, #0x24
0044273c: mov r6, r0
00442740: bl #0x42ca8c
00442744: bl #0x42cb8c
00442748: ldr r4, [pc, #0xdc]
0044274c: subs r5, r0, #0
00442750: add r4, pc, r4
00442754: beq #0x4427d8
00442758: ldr r7, [pc, #0xd0]
0044275c: ldr r3, [r4, r7]
00442760: ldr r2, [r3, #0x2c]
00442764: cmp r2, #0
00442768: beq #0x442804
0044276c: ldr r0, [r3, #0x28]
00442770: ldrb r3, [r0, #4]
00442774: cmp r3, #0
00442778: beq #0x4427e0
0044277c: ldr r0, [r4, r7]
00442780: bl #0x427d50
00442784: mov ip, #0
00442788: mov r2, #0
0044278c: mov r3, #0
00442790: strb ip, [sp, #0xc]
00442794: mov ip, #2
00442798: strd r2, r3, [sp, #0x18]
0044279c: strb ip, [sp, #0xd]
004427a0: mov ip, #0
004427a4: str ip, [sp, #0x10]
004427a8: ldr ip, [sp, #0x1c]
004427ac: add r4, sp, #0xc
004427b0: mov r1, r0
004427b4: str ip, [r4, #8]
004427b8: mov r0, r5
004427bc: mov ip, #1
004427c0: mov r2, r6
004427c4: mov r3, r4
004427c8: str ip, [sp]
004427cc: bl #0x7abe0c
004427d0: mov r0, r4
004427d4: bl #0x797124
004427d8: add sp, sp, #0x24
004427dc: pop {r4, r5, r6, r7, pc}
004427e0: ldr r1, [r0]
004427e4: sub r1, r1, #1
004427e8: cmp r1, #0
004427ec: str r1, [r0]
004427f0: beq #0x442824
004427f4: ldr r3, [r4, r7]
004427f8: mov r2, #0
004427fc: str r2, [r3, #0x2c]
00442800: str r2, [r3, #0x28]
00442804: ldr r3, [pc, #0x28]
00442808: ldr r0, [r4, r7]
0044280c: mov r2, r5
00442810: ldr r1, [r4, r3]
00442814: mov r3, #0
00442818: ldr r1, [r1]
0044281c: bl #0x427ca0
00442820: b #0x44277c
00442824: bl #0x752b38
00442828: b #0x4427f4
0044282c: subseq r2, r5, r0, asr #6
00442830: andeq r4, r0, r4, asr r0
00442834: andeq r4, r0, r4, asr #12

# _ZNK18MenuMessageManagerI11TutorialMsgLi1EE6InvokeEPKci.clone.49 0x442a14
00442a14: push {r4, r5, r6, r7, lr}
00442a18: sub sp, sp, #0x24
00442a1c: mov r6, r0
00442a20: bl #0x42ca8c
00442a24: bl #0x42cb8c
00442a28: ldr r4, [pc, #0xdc]
00442a2c: subs r5, r0, #0
00442a30: add r4, pc, r4
00442a34: beq #0x442ab8
00442a38: ldr r7, [pc, #0xd0]
00442a3c: ldr r3, [r4, r7]
00442a40: ldr r2, [r3, #0x2c]
00442a44: cmp r2, #0
00442a48: beq #0x442ae4
00442a4c: ldr r0, [r3, #0x28]
00442a50: ldrb r3, [r0, #4]
00442a54: cmp r3, #0
00442a58: beq #0x442ac0
00442a5c: ldr r0, [r4, r7]
00442a60: bl #0x427d50
00442a64: mov ip, #0
00442a68: mov r2, #0
00442a6c: mov r3, #0
00442a70: strb ip, [sp, #0xc]
00442a74: mov ip, #2
00442a78: strd r2, r3, [sp, #0x18]
00442a7c: strb ip, [sp, #0xd]
00442a80: mov ip, #0
00442a84: str ip, [sp, #0x10]
00442a88: ldr ip, [sp, #0x1c]
00442a8c: add r4, sp, #0xc
00442a90: mov r1, r0
00442a94: str ip, [r4, #8]
00442a98: mov r0, r5
00442a9c: mov ip, #1
00442aa0: mov r2, r6
00442aa4: mov r3, r4
00442aa8: str ip, [sp]
00442aac: bl #0x7abe0c
00442ab0: mov r0, r4
00442ab4: bl #0x797124
00442ab8: add sp, sp, #0x24
00442abc: pop {r4, r5, r6, r7, pc}
00442ac0: ldr r1, [r0]
00442ac4: sub r1, r1, #1
00442ac8: cmp r1, #0
00442acc: str r1, [r0]
00442ad0: beq #0x442b04
00442ad4: ldr r3, [r4, r7]
00442ad8: mov r2, #0
00442adc: str r2, [r3, #0x2c]
00442ae0: str r2, [r3, #0x28]
00442ae4: ldr r3, [pc, #0x28]
00442ae8: ldr r0, [r4, r7]
00442aec: mov r2, r5
00442af0: ldr r1, [r4, r3]
00442af4: mov r3, #0
00442af8: ldr r1, [r1]
00442afc: bl #0x427ca0
00442b00: b #0x442a5c
00442b04: bl #0x752b38
00442b08: b #0x442ad4
00442b0c: subseq r2, r5, r0, rrx
00442b10: andeq r0, r0, r8, lsl #18
00442b14: andeq r3, r0, r4, ror sp

# _ZNK18MenuMessageManagerI11TutorialMsgLi1EE6InvokeEPKci.clone.27 0x45a030
0045a030: push {r4, r5, r6, r7, r8, lr}
0045a034: sub sp, sp, #0x20
0045a038: mov r6, r0
0045a03c: bl #0x42ca8c
0045a040: bl #0x42cb8c
0045a044: ldr r4, [pc, #0xa4]
0045a048: subs r5, r0, #0
0045a04c: add r4, pc, r4
0045a050: beq #0x45a0cc
0045a054: ldr r7, [pc, #0x98]
0045a058: ldr r8, [r4, r7]
0045a05c: add r0, r8, #0x28
0045a060: bl #0x386144
0045a064: ldr r3, [r8, #0x2c]
0045a068: cmp r3, #0
0045a06c: beq #0x45a0d4
0045a070: ldr r0, [r4, r7]
0045a074: bl #0x427d50
0045a078: mov ip, #0
0045a07c: mov r2, #0
0045a080: mov r3, #0
0045a084: strb ip, [sp, #0xc]
0045a088: mov ip, #2
0045a08c: strd r2, r3, [sp, #0x18]
0045a090: strb ip, [sp, #0xd]
0045a094: mov ip, #0
0045a098: str ip, [sp, #0x10]
0045a09c: ldr ip, [sp, #0x1c]
0045a0a0: add r4, sp, #0xc
0045a0a4: mov r1, r0
0045a0a8: str ip, [r4, #8]
0045a0ac: mov r0, r5
0045a0b0: mov ip, #1
0045a0b4: mov r2, r6
0045a0b8: mov r3, r4
0045a0bc: str ip, [sp]
0045a0c0: bl #0x7abe0c
0045a0c4: mov r0, r4
0045a0c8: bl #0x797124
0045a0cc: add sp, sp, #0x20
0045a0d0: pop {r4, r5, r6, r7, r8, pc}
0045a0d4: ldr r2, [pc, #0x1c]
0045a0d8: mov r0, r8
0045a0dc: ldr r1, [r4, r2]
0045a0e0: mov r2, r5
0045a0e4: ldr r1, [r1]
0045a0e8: bl #0x427ca0
0045a0ec: b #0x45a070
0045a0f0: subseq sl, r3, r4, asr #20
0045a0f4: andeq r0, r0, r8, lsl #18
0045a0f8: andeq r3, r0, r4, ror sp

# _ZNK18MenuMessageManagerI19CharMenuTutorialMsgLi1EE6InvokeEPKci.clone.31 0x45a0fc
0045a0fc: push {r4, r5, r6, r7, r8, lr}
0045a100: sub sp, sp, #0x20
0045a104: mov r6, r0
0045a108: bl #0x42ca8c
0045a10c: bl #0x42cb8c
0045a110: ldr r4, [pc, #0xa4]
0045a114: subs r5, r0, #0
0045a118: add r4, pc, r4
0045a11c: beq #0x45a198
0045a120: ldr r7, [pc, #0x98]
0045a124: ldr r8, [r4, r7]
0045a128: add r0, r8, #0x28
0045a12c: bl #0x386144
0045a130: ldr r3, [r8, #0x2c]
0045a134: cmp r3, #0
0045a138: beq #0x45a1a0
0045a13c: ldr r0, [r4, r7]
0045a140: bl #0x427d50
0045a144: mov ip, #0
0045a148: mov r2, #0
0045a14c: mov r3, #0
0045a150: strb ip, [sp, #0xc]
0045a154: mov ip, #2
0045a158: strd r2, r3, [sp, #0x18]
0045a15c: strb ip, [sp, #0xd]
0045a160: mov ip, #0
0045a164: str ip, [sp, #0x10]
0045a168: ldr ip, [sp, #0x1c]
0045a16c: add r4, sp, #0xc
0045a170: mov r1, r0
0045a174: str ip, [r4, #8]
0045a178: mov r0, r5
0045a17c: mov ip, #1
0045a180: mov r2, r6
0045a184: mov r3, r4
0045a188: str ip, [sp]
0045a18c: bl #0x7abe0c
0045a190: mov r0, r4
0045a194: bl #0x797124
0045a198: add sp, sp, #0x20
0045a19c: pop {r4, r5, r6, r7, r8, pc}
0045a1a0: ldr r2, [pc, #0x1c]
0045a1a4: mov r0, r8
0045a1a8: ldr r1, [r4, r2]
0045a1ac: mov r2, r5
0045a1b0: ldr r1, [r1]
0045a1b4: bl #0x427ca0
0045a1b8: b #0x45a13c
0045a1bc: subseq sl, r3, r8, ror sb
0045a1c0: andeq r4, r0, r4, asr r0
0045a1c4: andeq r4, r0, r4, asr #12

# _ZN18MenuMessageManagerI11TutorialMsgLi1EE19SkipEnqueuedMessageEib.clone.30 0x46026c
0046026c: push {r4, r5, r6, lr}
00460270: ldr r4, [pc, #0xb0]
00460274: ldr r5, [pc, #0xb0]
00460278: add r4, pc, r4
0046027c: ldr r3, [r4, r5]
00460280: ldr r2, [r3, #4]
00460284: ldr r1, [r3, #0x14]
00460288: cmp r1, r2
0046028c: beq #0x4602e8
00460290: ldr r1, [r3, #0xc]
00460294: sub r1, r1, #8
00460298: cmp r2, r1
0046029c: addne r2, r2, #8
004602a0: strne r2, [r3, #4]
004602a4: beq #0x4602ec
004602a8: ldr r3, [pc, #0x80]
004602ac: ldr r3, [r4, r3]
004602b0: ldr r0, [r3]
004602b4: cmp r0, #0
004602b8: beq #0x4602c0
004602bc: bl #0x45a030
004602c0: ldr r3, [r4, r5]
004602c4: ldr r2, [r3, #4]
004602c8: ldr r3, [r3, #0x14]
004602cc: cmp r3, r2
004602d0: beq #0x4602e8
004602d4: ldr r3, [pc, #0x58]
004602d8: ldr r3, [r4, r3]
004602dc: ldr r0, [r3]
004602e0: pop {r4, r5, r6, lr}
004602e4: b #0x45a030
004602e8: pop {r4, r5, r6, pc}
004602ec: ldr r0, [r3, #8]
004602f0: cmp r0, #0
004602f4: beq #0x460300
004602f8: mov r1, #0x80
004602fc: bl #0x708f00
00460300: ldr r3, [r4, r5]
00460304: ldr r2, [r3, #0x10]
00460308: add r1, r2, #4
0046030c: str r1, [r3, #0x10]
00460310: ldr r2, [r2, #4]
00460314: add r1, r2, #0x80
00460318: str r2, [r3, #4]
0046031c: str r1, [r3, #0xc]
00460320: str r2, [r3, #8]
00460324: b #0x4602a8
00460328: subseq r4, r3, r8, lsl r8
0046032c: andeq r1, r0, r0, lsl ip
00460330: strheq r1, [r0], -r8
00460334: andeq r3, r0, r8, lsr r3
