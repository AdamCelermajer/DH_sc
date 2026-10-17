_Z20NativeCreateSaveSlotRKN7gameswf7fn_callE 0x43f630
0043f630: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0043f634: ldr r4, [pc, #0x1d4]
0043f638: ldr r5, [pc, #0x1d4]
0043f63c: ldr r2, [r0, #0x10]
0043f640: add r4, pc, r4
0043f644: ldr r3, [r4, r5]
0043f648: sub sp, sp, #0x1a4
0043f64c: cmp r2, #2
0043f650: ldr r3, [r3]
0043f654: mov r6, r0
0043f658: str r3, [sp, #0x19c]
0043f65c: beq #0x43f67c
0043f660: ldr r3, [r4, r5]
0043f664: ldr r2, [sp, #0x19c]
0043f668: ldr r3, [r3]
0043f66c: cmp r2, r3
0043f670: bne #0x43f80c
0043f674: add sp, sp, #0x1a4
0043f678: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0043f67c: ldr r3, [r0, #0xc]
0043f680: ldr r0, [r0, #0x14]
0043f684: mov r7, #0xc
0043f688: ldr r3, [r3]
0043f68c: mla r0, r7, r0, r3
0043f690: bl #0x420a84
0043f694: ldr r3, [r6, #0xc]
0043f698: mov sl, r0
0043f69c: ldr r0, [r6, #0x14]
0043f6a0: ldr r3, [r3]
0043f6a4: sub r0, r0, #1
0043f6a8: mla r0, r7, r0, r3
0043f6ac: bl #0x420a84
0043f6b0: ldrsb r3, [r0]
0043f6b4: cmn r3, #1
0043f6b8: ldr r3, [pc, #0x158]
0043f6bc: addne sb, r0, #1
0043f6c0: ldreq sb, [r0, #0xc]
0043f6c4: ldr r3, [r4, r3]
0043f6c8: ldr r7, [r3]
0043f6cc: cmp r7, #0
0043f6d0: beq #0x43f804
0043f6d4: ldr r3, [pc, #0x140]
0043f6d8: mov r8, #0
0043f6dc: ldr r3, [r4, r3]
0043f6e0: ldr fp, [r3]
0043f6e4: b #0x43f6f4
0043f6e8: add r8, r8, #1
0043f6ec: cmp r8, r7
0043f6f0: beq #0x43f804
0043f6f4: mov r0, sb
0043f6f8: ldr r1, [fp, r8, lsl #2]
0043f6fc: bl #0x30e31c
0043f700: cmp r0, #0
0043f704: bne #0x43f6e8
0043f708: mov r0, r8
0043f70c: bl #0x439c34
0043f710: cmp r0, #0
0043f714: beq #0x43f660
0043f718: bl #0x4660c8
0043f71c: add r7, sp, #4
0043f720: mov r1, r0
0043f724: mov r2, #1
0043f728: mov r3, #0
0043f72c: mov sb, r0
0043f730: mov r0, r7
0043f734: bl #0x4655ac
0043f738: ldrsb r3, [sl]
0043f73c: add r0, r7, #0x18
0043f740: cmn r3, #1
0043f744: addne r1, sl, #1
0043f748: ldreq r1, [sl, #0xc]
0043f74c: bl #0x33076c
0043f750: mov r3, #1
0043f754: str r3, [sp, #0x34]
0043f758: ldr r3, [sp, #0x40]
0043f75c: str r8, [sp, #0x38]
0043f760: mov r8, #0
0043f764: cmp r3, #0
0043f768: movlt r3, #0
0043f76c: strlt r3, [sp, #0x40]
0043f770: ldr r3, [pc, #0xa8]
0043f774: mov r0, r7
0043f778: ldr r3, [r4, r3]
0043f77c: str r8, [r3]
0043f780: bl #0x467718
0043f784: mov r0, r7
0043f788: bl #0x467744
0043f78c: ldr r3, [pc, #0x90]
0043f790: mov r0, #1
0043f794: strb r0, [sp, #0x198]
0043f798: ldr r3, [r4, r3]
0043f79c: ldr ip, [r3]
0043f7a0: cmp ip, r8
0043f7a4: beq #0x43f7d4
0043f7a8: add ip, r7, ip
0043f7ac: mov r2, r7
0043f7b0: mov r3, r7
0043f7b4: mov r1, #0x29
0043f7b8: str r1, [r3, #0x50]
0043f7bc: str r8, [r3, #0x40]
0043f7c0: strb r0, [r2, #0x4c]
0043f7c4: add r2, r2, #1
0043f7c8: cmp r2, ip
0043f7cc: add r3, r3, #4
0043f7d0: bne #0x43f7b8
0043f7d4: mov r0, r7
0043f7d8: bl #0x464b2c
0043f7dc: mov r0, sb
0043f7e0: bl #0x30ed30
0043f7e4: ldr r6, [r6]
0043f7e8: mov r2, r0
0043f7ec: mov r3, r1
0043f7f0: mov r0, r6
0043f7f4: bl #0x797488
0043f7f8: mov r0, r7
0043f7fc: bl #0x46378c
0043f800: b #0x43f660
0043f804: mvn r8, #0
0043f808: b #0x43f708
0043f80c: bl #0x30e310
0043f810: subseq r5, r5, r0, asr r4
0043f814: andeq r4, r0, ip, lsr #1
0043f818: andeq r4, r0, r4, lsl #4
0043f81c: andeq r3, r0, r8, lsl #24
0043f820: muleq r0, ip, sl
0043f824: andeq r3, r0, r0, lsr r5

_ZN14PlayerSavegameC1Ejib 0x4655ac
004655ac: ldr r3, [pc, #0x100]
004655b0: push {r4, r5, r6, lr}
004655b4: ldr lr, [pc, #0xfc]
004655b8: add r3, pc, r3
004655bc: mov r4, r0
004655c0: ldr lr, [r3, lr]
004655c4: mov r5, #0
004655c8: add ip, r0, #0x18
004655cc: add lr, lr, #8
004655d0: str lr, [r0]
004655d4: str r1, [r0, #4]
004655d8: mov r0, ip
004655dc: str ip, [r4, #0x28]
004655e0: str ip, [r4, #0x2c]
004655e4: mov r1, #0x10
004655e8: str r5, [r4, #8]
004655ec: strb r5, [r4, #0xc]
004655f0: str r5, [r4, #0x10]
004655f4: strb r5, [r4, #0x14]
004655f8: mov r6, r2
004655fc: bl #0x31167c
00465600: ldr r3, [r4, #0x28]
00465604: add r0, r4, #0xb8
00465608: strb r5, [r3]
0046560c: mov r3, #1
00465610: str r3, [r4, #0x30]
00465614: mvn r3, #0
00465618: str r3, [r4, #0x34]
0046561c: str r5, [r4, #0x3c]
00465620: str r5, [r4, #0x80]
00465624: str r5, [r4, #0x84]
00465628: str r5, [r4, #0x88]
0046562c: str r5, [r4, #0x8c]
00465630: str r5, [r4, #0x90]
00465634: bl #0x46b0a4
00465638: add r0, r4, #0x118
0046563c: bl #0x46b0a4
00465640: add r2, r4, #0x17c
00465644: mov r3, r5
00465648: str r3, [r2, r5]
0046564c: add r1, r2, r5
00465650: add r5, r5, #8
00465654: cmp r5, #0x18
00465658: str r3, [r1, #4]
0046565c: bne #0x465648
00465660: mov r1, r3
00465664: strb r3, [r4, #0x194]
00465668: mov r2, r1
0046566c: mov r3, r4
00465670: add r1, r1, #1
00465674: cmp r1, #3
00465678: str r2, [r3, #0x94]
0046567c: str r2, [r3, #0xa0]
00465680: str r2, [r3, #0xac]
00465684: str r2, [r3, #0x40]
00465688: str r2, [r3, #0x68]
0046568c: str r2, [r3, #0x74]
00465690: str r2, [r3, #0x5c]
00465694: str r2, [r3, #0x50]
00465698: add r3, r3, #4
0046569c: bne #0x465670
004656a0: mov r0, r4
004656a4: mov r1, r6
004656a8: bl #0x465430
004656ac: mov r0, r4
004656b0: pop {r4, r5, r6, pc}
004656b4: ldrsbeq pc, [r2], #-0x48
004656b8: strheq r4, [r0], -ip

_ZN14PlayerSavegame7SG_LoadEi 0x465430
00465430: push {r4, r5, r6, lr}
00465434: mov r5, r0
00465438: mov r4, r1
0046543c: bl #0x464f4c
00465440: mov r0, r5
00465444: mov r1, r4
00465448: pop {r4, r5, r6, lr}
0046544c: b #0x468574

_ZN14PlayerSavegame22_LoadVolatileQuestsLogEi 0x468574
00468574: push {r4, r5, r6, r7, r8, lr}
00468578: ldr r4, [pc, #0xa4]
0046857c: tst r1, #0x14
00468580: mov r5, r0
00468584: add r4, pc, r4
00468588: bne #0x468590
0046858c: pop {r4, r5, r6, r7, r8, pc}
00468590: bl #0x7fd794
00468594: ldrb r3, [r0, #5]
00468598: cmp r3, #0
0046859c: beq #0x46858c
004685a0: ldr r3, [pc, #0x80]
004685a4: ldr r6, [r4, r3]
004685a8: ldr r0, [r6, #0x40]
004685ac: bl #0x36f074
004685b0: cmp r0, #0
004685b4: ldreq r7, [r6, #0x40]
004685b8: bne #0x468610
004685bc: add r6, r7, #0x6e0
004685c0: ldr r3, [r7, #0x6e0]
004685c4: mov r0, r6
004685c8: mov lr, pc
004685cc: ldr pc, [r3, #8]
004685d0: orrs r1, r0, r1
004685d4: beq #0x46858c
004685d8: ldr r1, [r7, #0x6e0]
004685dc: mov r0, r6
004685e0: mov r2, #0
004685e4: mov r3, #0
004685e8: mov lr, pc
004685ec: ldr pc, [r1, #0x20]
004685f0: ldr r3, [pc, #0x34]
004685f4: add r0, r5, #0x118
004685f8: mov r2, r6
004685fc: ldr r1, [r4, r3]
00468600: mov r3, #0
00468604: ldr r1, [r1]
00468608: pop {r4, r5, r6, r7, r8, lr}
0046860c: b #0x46c48c
00468610: ldr r7, [r6, #0x40]
00468614: ldrb r3, [r7, #0x719]
00468618: cmp r3, #0
0046861c: beq #0x46858c
00468620: b #0x4685bc
00468624: subseq ip, r2, ip, lsl #10
00468628: strdeq r3, r4, [r0], -r4
0046862c: muleq r0, ip, sl

_Z17IsPlayableClassIDi 0x439c34
00439c34: movw r3, #0x107
00439c38: movw r2, #0x145
00439c3c: cmp r0, r3
00439c40: cmpne r0, r2
00439c44: moveq r0, #1
00439c48: bxeq lr
00439c4c: movw r3, #0x122
00439c50: cmp r0, r3
00439c54: movne r0, #0
00439c58: moveq r0, #1
00439c5c: bx lr

_ZN14PlayerSavegame16SG_GenerateSeedsEv 0x467718
00467718: push {r4, lr}
0046771c: mov r4, r0
00467720: bl #0x60b0cc
00467724: add r3, r0, #0x5300
00467728: add r3, r3, #0x7b
0046772c: add r2, r3, #0xfd00
00467730: add r2, r2, #0x2f
00467734: str r2, [r4, #0x64]
00467738: str r0, [r4, #0x5c]
0046773c: str r3, [r4, #0x60]
00467740: pop {r4, pc}

_ZN14PlayerSavegame14SG_SetSaveDateEv 0x467744
00467744: push {r4, lr}
00467748: mov r4, r0
0046774c: mov r0, #0
00467750: bl #0x30e580
00467754: str r0, [r4, #0x38]
00467758: pop {r4, pc}

_ZN14PlayerSavegame7SG_SaveEv 0x464b2c
00464b2c: push {r4, r5, r6, lr}
00464b30: ldr r3, [r0, #8]
00464b34: ldr r6, [pc, #0x158]
00464b38: sub sp, sp, #8
00464b3c: cmp r3, #0
00464b40: mov r4, r0
00464b44: add r6, pc, r6
00464b48: beq #0x464b58
00464b4c: ldrb r3, [r0, #0xc]
00464b50: cmp r3, #0
00464b54: beq #0x464b60
00464b58: add sp, sp, #8
00464b5c: pop {r4, r5, r6, pc}
00464b60: bl #0x7fd794
00464b64: ldrb r3, [r0, #5]
00464b68: cmp r3, #0
00464b6c: bne #0x464ba0
00464b70: mov r3, #1
00464b74: str r3, [r4, #0x178]
00464b78: bl #0x7fd794
00464b7c: ldrb r3, [r0, #5]
00464b80: cmp r3, #0
00464b84: bne #0x464bd4
00464b88: ldr r0, [r4, #8]
00464b8c: bl #0x315fb8
00464b90: mov r0, r4
00464b94: add sp, sp, #8
00464b98: pop {r4, r5, r6, lr}
00464b9c: b #0x4684f0
00464ba0: ldr r3, [pc, #0xf0]
00464ba4: ldr r5, [r6, r3]
00464ba8: ldr r0, [r5, #0x40]
00464bac: bl #0x36f074
00464bb0: cmp r0, #0
00464bb4: beq #0x464b70
00464bb8: ldr r3, [r5, #0x40]
00464bbc: ldrb r3, [r3, #0x719]
00464bc0: cmp r3, #0
00464bc4: bne #0x464b70
00464bc8: mov r3, #2
00464bcc: str r3, [r4, #0x178]
00464bd0: b #0x464b78
00464bd4: ldr r3, [pc, #0xbc]
00464bd8: ldr r5, [r6, r3]
00464bdc: ldr r0, [r5, #0x40]
00464be0: bl #0x36f074
00464be4: cmp r0, #0
00464be8: bne #0x464c74
00464bec: mov r1, #1
00464bf0: mov r0, r4
00464bf4: bl #0x463654
00464bf8: mov r5, r0
00464bfc: mov r1, #1
00464c00: mov r0, r4
00464c04: mov r2, r5
00464c08: bl #0x468630
00464c0c: ldr r0, [r4, #8]
00464c10: ldr r1, [r0, #0x1c]
00464c14: cmp r1, #0
00464c18: beq #0x464c88
00464c1c: bl #0x315fb8
00464c20: mov r0, r4
00464c24: mov r1, #0
00464c28: mov r2, r5
00464c2c: bl #0x468630
00464c30: cmp r5, #0
00464c34: beq #0x464b90
00464c38: ldr r3, [pc, #0x5c]
00464c3c: mov r5, #0
00464c40: ldr r0, [r4, #4]
00464c44: ldr r3, [r6, r3]
00464c48: mov r1, r5
00464c4c: mov ip, #1
00464c50: ldr r3, [r3]
00464c54: mov r2, r5
00464c58: str ip, [sp]
00464c5c: str r5, [sp, #4]
00464c60: bl #0x4626f4
00464c64: mov r1, r5
00464c68: ldr r0, [r4, #4]
00464c6c: bl #0x464a68
00464c70: b #0x464b90
00464c74: ldr r3, [r5, #0x40]
00464c78: ldrb r3, [r3, #0x719]
00464c7c: cmp r3, #0
00464c80: beq #0x464b88
00464c84: b #0x464bec
00464c88: bl #0x315ad0
00464c8c: ldr r0, [r4, #8]
00464c90: b #0x464c1c
00464c94: subseq pc, r2, ip, asr #30
00464c98: strdeq r3, r4, [r0], -r4
00464c9c: muleq r0, ip, sl

_ZN14PlayerSavegame22_SaveVolatileQuestsLogEv 0x4684f0
004684f0: push {r4, r5, r6, lr}
004684f4: mov r5, r0
004684f8: bl #0x7fd794
004684fc: ldrb r3, [r0, #5]
00468500: ldr r4, [pc, #0x60]
00468504: cmp r3, #0
00468508: add r4, pc, r4
0046850c: bne #0x468514
00468510: pop {r4, r5, r6, pc}
00468514: ldr r3, [pc, #0x50]
00468518: ldr r6, [r4, r3]
0046851c: ldr r0, [r6, #0x40]
00468520: bl #0x36f074
00468524: cmp r0, #0
00468528: ldreq r6, [r6, #0x40]
0046852c: beq #0x468540
00468530: ldr r6, [r6, #0x40]
00468534: ldrb r3, [r6, #0x719]
00468538: cmp r3, #0
0046853c: beq #0x468510
00468540: add r6, r6, #0x6e0
00468544: mov r0, r6
00468548: bl #0x316a48
0046854c: ldr r3, [pc, #0x1c]
00468550: add r0, r5, #0x118
00468554: mov r2, r6
00468558: ldr r3, [r4, r3]
0046855c: ldr r1, [r3]
00468560: pop {r4, r5, r6, lr}
00468564: b #0x46c658
00468568: subseq ip, r2, r8, lsl #11
0046856c: strdeq r3, r4, [r0], -r4
00468570: muleq r0, ip, sl

_ZN8Savegame7saveAllEv 0x315fb8
00315fb8: ldr r1, [pc, #0x3dc]
00315fbc: ldr r2, [pc, #0x3dc]
00315fc0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00315fc4: add r1, pc, r1
00315fc8: ldr r3, [r1, r2]
00315fcc: mov sb, r0
00315fd0: ldr r0, [pc, #0x3cc]
00315fd4: ldr r3, [r3]
00315fd8: sub sp, sp, #0x8c
00315fdc: str r1, [sp, #0x14]
00315fe0: add r0, pc, r0
00315fe4: add r1, sp, #0x88
00315fe8: str r2, [sp, #0x20]
00315fec: str r1, [sp, #0x18]
00315ff0: str r3, [sp, #0x84]
00315ff4: bl #0x3136b4
00315ff8: ldr r2, [sp, #0x18]
00315ffc: mov r4, #0
00316000: add fp, sb, #0x20
00316004: str r4, [r2, #-0x24]!
00316008: add r3, r2, #4
0031600c: str r2, [sp, #0x18]
00316010: ldr r2, [sb, #0x14]
00316014: ldr r1, [sb, #0x18]
00316018: mov r0, r3
0031601c: str r3, [sp, #0x78]
00316020: str r3, [sp, #0x7c]
00316024: bl #0x3116e8
00316028: mov r3, #1
0031602c: ldr r0, [sp, #0x18]
00316030: strb r3, [sp, #0x80]
00316034: strb r4, [sp, #0x81]
00316038: bl #0x315110
0031603c: mov r0, #0x30
00316040: bl #0x310454
00316044: add r3, sp, #0x88
00316048: mov r4, r0
0031604c: str r3, [sp, #0x1c]
00316050: bl #0x316d3c
00316054: ldr r0, [sp, #0x1c]
00316058: mvn r3, #0
0031605c: add r7, sp, #0x3c
00316060: str r3, [r0, #-0x48]!
00316064: str r0, [sp, #0x1c]
00316068: ldr r1, [sp, #0x1c]
0031606c: mov r0, r4
00316070: bl #0x3139e0
00316074: ldr r3, [pc, #0x32c]
00316078: ldr r1, [pc, #0x32c]
0031607c: ldr r2, [pc, #0x32c]
00316080: add r3, pc, r3
00316084: str r3, [sp, #0x2c]
00316088: ldr r3, [pc, #0x324]
0031608c: add r3, pc, r3
00316090: str r3, [sp, #0x30]
00316094: ldr r3, [pc, #0x31c]
00316098: add r3, pc, r3
0031609c: str r3, [sp, #0x34]
003160a0: ldr r5, [sb, #0x28]
003160a4: str r1, [sp, #0x24]
003160a8: str r2, [sp, #0x28]
003160ac: cmp r5, fp
003160b0: beq #0x3161a0
003160b4: ldr r3, [r4]
003160b8: mov r0, r4
003160bc: mov lr, pc
003160c0: ldr pc, [r3, #0x30]
003160c4: mov r3, #0
003160c8: strd r0, r1, [sp, #8]
003160cc: mov r1, r7
003160d0: mov r0, r4
003160d4: str r3, [sp, #0x3c]
003160d8: bl #0x3139e0
003160dc: ldr r1, [r5, #0x24]
003160e0: mov r2, #4
003160e4: mov r3, #0
003160e8: mov r0, r4
003160ec: bl #0x317604
003160f0: ldr r3, [r4]
003160f4: mov r0, r4
003160f8: mov lr, pc
003160fc: ldr pc, [r3, #0x30]
00316100: ldr r3, [r5, #0x38]
00316104: mov r8, r0
00316108: cmp r3, #0
0031610c: beq #0x316284
00316110: mov r0, r4
00316114: ldr r1, [r5, #0x3c]
00316118: blx r3
0031611c: ldr r3, [r4]
00316120: mov r0, r4
00316124: mov lr, pc
00316128: ldr pc, [r3, #0x30]
0031612c: ldrd r2, r3, [sp, #8]
00316130: rsb r8, r8, r0
00316134: str r8, [sp, #0x3c]
00316138: mov r6, r0
0031613c: mov sl, r1
00316140: mov r0, r4
00316144: ldr r1, [r4]
00316148: mov lr, pc
0031614c: ldr pc, [r1, #0x2c]
00316150: mov r0, r4
00316154: mov r1, r7
00316158: bl #0x3139e0
0031615c: mov r3, sl
00316160: mov r2, r6
00316164: ldr r1, [r4]
00316168: mov r0, r4
0031616c: mov lr, pc
00316170: ldr pc, [r1, #0x2c]
00316174: ldr r3, [r5, #0xc]
00316178: cmp r3, #0
0031617c: bne #0x316188
00316180: b #0x3162ec
00316184: mov r3, r2
00316188: ldr r2, [r3, #8]
0031618c: cmp r2, #0
00316190: bne #0x316184
00316194: mov r5, r3
00316198: cmp r5, fp
0031619c: bne #0x3160b4
003161a0: ldr r3, [r4]
003161a4: mov r0, r4
003161a8: mov lr, pc
003161ac: ldr pc, [r3, #0x30]
003161b0: mov r2, #0
003161b4: mov r6, r0
003161b8: mov r7, r1
003161bc: mov r3, #0
003161c0: mov r0, r4
003161c4: ldr r1, [r4]
003161c8: mov lr, pc
003161cc: ldr pc, [r1, #0x2c]
003161d0: ldr r3, [sb, #0x30]
003161d4: ldr r1, [sp, #0x1c]
003161d8: mov r0, r4
003161dc: str r3, [sp, #0x40]
003161e0: bl #0x3139e0
003161e4: mov r2, r6
003161e8: mov r3, r7
003161ec: ldr r1, [r4]
003161f0: mov r0, r4
003161f4: mov lr, pc
003161f8: ldr pc, [r1, #0x2c]
003161fc: add r5, sp, #0x88
00316200: mov r0, sb
00316204: mov r1, r4
00316208: bl #0x315ad0
0031620c: str r4, [r5, #-0x44]!
00316210: add r3, r5, #4
00316214: ldr r2, [sb, #0x14]
00316218: ldr r1, [sb, #0x18]
0031621c: mov r0, r3
00316220: str r3, [sp, #0x58]
00316224: str r3, [sp, #0x5c]
00316228: bl #0x3116e8
0031622c: mov r3, #0
00316230: mov r0, r5
00316234: strb r3, [sp, #0x60]
00316238: mov r3, #1
0031623c: strb r3, [sp, #0x61]
00316240: bl #0x315110
00316244: mov r0, r5
00316248: bl #0x313c90
0031624c: ldr r0, [sp, #0x18]
00316250: bl #0x313c90
00316254: ldr r0, [pc, #0x160]
00316258: add r0, pc, r0
0031625c: bl #0x3136b8
00316260: ldr r1, [sp, #0x14]
00316264: ldr r0, [sp, #0x20]
00316268: ldr r2, [sp, #0x84]
0031626c: ldr r3, [r1, r0]
00316270: ldr r3, [r3]
00316274: cmp r2, r3
00316278: bne #0x316398
0031627c: add sp, sp, #0x8c
00316280: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00316284: ldr r6, [sb, #0x1c]
00316288: cmp r6, #0
0031628c: beq #0x316320
00316290: ldrb r3, [r6, #0x2c]
00316294: ldr r2, [r4]
00316298: cmp r3, #0
0031629c: ldr sl, [r2, #0x1c]
003162a0: bne #0x3162c8
003162a4: ldr r1, [sp, #0x14]
003162a8: ldr r0, [sp, #0x24]
003162ac: ldr r2, [r1, r0]
003162b0: ldr r2, [r2]
003162b4: cmp r2, #2
003162b8: streq r3, [r3]
003162bc: beq #0x3162c8
003162c0: cmp r2, #1
003162c4: beq #0x31636c
003162c8: ldr r3, [r6, #0x1c]
003162cc: ldr ip, [r5, #0x28]
003162d0: mov r0, r4
003162d4: ldr r1, [r3]
003162d8: ldr r2, [r5, #0x30]
003162dc: mov r3, #0
003162e0: add r1, r1, ip
003162e4: blx sl
003162e8: b #0x31611c
003162ec: ldr r2, [r5, #4]
003162f0: ldr r1, [r2, #0xc]
003162f4: cmp r5, r1
003162f8: bne #0x316314
003162fc: mov r5, r2
00316300: ldr r2, [r2, #4]
00316304: ldr r3, [r2, #0xc]
00316308: cmp r3, r5
0031630c: beq #0x3162fc
00316310: ldr r3, [r5, #0xc]
00316314: cmp r3, r2
00316318: movne r5, r2
0031631c: b #0x3160ac
00316320: mov r1, r6
00316324: ldr r0, [r5, #0x30]
00316328: bl #0x31056c
0031632c: mov r1, r6
00316330: mov sl, r0
00316334: ldr r2, [r5, #0x30]
00316338: bl #0x30e460
0031633c: mov r3, r6
00316340: ldr ip, [r4]
00316344: mov r0, r4
00316348: mov r1, sl
0031634c: ldr r2, [r5, #0x30]
00316350: mov lr, pc
00316354: ldr pc, [ip, #0x1c]
00316358: cmp sl, #0
0031635c: beq #0x31611c
00316360: mov r0, sl
00316364: bl #0x310440
00316368: b #0x31611c
0031636c: ldr r3, [sp, #0x14]
00316370: ldr r2, [sp, #0x28]
00316374: mov ip, #0x82
00316378: ldr r1, [sp, #0x2c]
0031637c: ldr r0, [r3, r2]
00316380: ldr r2, [sp, #0x30]
00316384: ldr r3, [sp, #0x34]
00316388: add r0, r0, #0xa8
0031638c: str ip, [sp]
00316390: bl #0x30e004
00316394: b #0x3162c8
00316398: bl #0x30e310
0031639c: rsbeq lr, r7, ip, asr #21
003163a0: andeq r4, r0, ip, lsr #1
003163a4: ldrsbeq r8, [sl], #-0x50
003163a8: subseq r8, sl, r8, asr r3
003163ac: andeq r3, r0, r0, asr #19
003163b0: andeq r1, r0, r0, asr #19
003163b4: subseq r8, sl, ip, lsr r5
003163b8: subseq r8, sl, r8, lsr r5
003163bc: subseq r8, sl, r8, asr r3

_ZN6Arrays19GetMemberIDByStringINS_14CharacterTableEEEiPKc 0x3fa188
003fa188: ldr r3, [pc, #0x60]
003fa18c: ldr r2, [pc, #0x60]
003fa190: push {r4, r5, r6, r7, r8, lr}
003fa194: add r3, pc, r3
003fa198: ldr r2, [r3, r2]
003fa19c: mov r6, r0
003fa1a0: ldr r5, [r2]
003fa1a4: cmp r5, #0
003fa1a8: beq #0x3fa1e8
003fa1ac: ldr r2, [pc, #0x44]
003fa1b0: mov r4, #0
003fa1b4: ldr r3, [r3, r2]
003fa1b8: ldr r7, [r3]
003fa1bc: b #0x3fa1cc
003fa1c0: add r4, r4, #1
003fa1c4: cmp r4, r5
003fa1c8: beq #0x3fa1e8
003fa1cc: ldr r1, [r7, r4, lsl #2]
003fa1d0: mov r0, r6
003fa1d4: bl #0x30e31c
003fa1d8: cmp r0, #0
003fa1dc: bne #0x3fa1c0
003fa1e0: mov r0, r4
003fa1e4: pop {r4, r5, r6, r7, r8, pc}
003fa1e8: mvn r0, #0
003fa1ec: pop {r4, r5, r6, r7, r8, pc}
003fa1f0: ldrsheq sl, [sb], #-0x8c
003fa1f4: andeq r4, r0, r4, lsl #4
003fa1f8: andeq r3, r0, r8, lsl #24

_ZN13QuestSavegameC1Ev 0x46b0a4
0046b0a4: ldr r3, [pc, #0x90]
0046b0a8: ldr r2, [pc, #0x90]
0046b0ac: mov r1, #0
0046b0b0: add r3, pc, r3
0046b0b4: ldr r2, [r3, r2]
0046b0b8: push {r4, r5, r6}
0046b0bc: mov ip, r1
0046b0c0: add r5, r0, #0x10
0046b0c4: add r4, r0, #0x1c
0046b0c8: add r2, r2, #8
0046b0cc: str r2, [r0]
0046b0d0: str r1, [r0, #4]
0046b0d4: str r1, [r0, #8]
0046b0d8: str r1, [r0, #0xc]
0046b0dc: str r1, [r0, #0x10]
0046b0e0: mov r2, r0
0046b0e4: str r1, [r5, #8]
0046b0e8: str r1, [r5, #4]
0046b0ec: mov r6, ip
0046b0f0: str r1, [r0, #0x1c]
0046b0f4: mvn r5, #0
0046b0f8: str r1, [r4, #8]
0046b0fc: str r1, [r4, #4]
0046b100: str r1, [r0, #0x5c]
0046b104: mov r4, #1
0046b108: mov r1, r0
0046b10c: add ip, ip, #1
0046b110: cmp ip, #3
0046b114: str r5, [r2, #0x2c]
0046b118: str r5, [r2, #0x38]
0046b11c: str r4, [r2, #0x44]
0046b120: str r4, [r2, #0x50]
0046b124: strb r6, [r1, #0x28]
0046b128: add r2, r2, #4
0046b12c: add r1, r1, #1
0046b130: bne #0x46b10c
0046b134: pop {r4, r5, r6}
0046b138: bx lr
0046b13c: subseq sb, r2, r0, ror #19
0046b140: andeq r3, r0, r4, ror #9

_ZN11IStreamBase7writeAsERKSs 0x461668
00461668: push {r4, r5, lr}
0046166c: ldr r2, [r1, #0x10]
00461670: ldr r3, [r1, #0x14]
00461674: sub sp, sp, #0xc
00461678: mov r5, r1
0046167c: rsb r3, r3, r2
00461680: add r1, sp, #8
00461684: add r3, r3, #1
00461688: str r3, [r1, #-4]!
0046168c: mov r4, r0
00461690: bl #0x38b808
00461694: ldr r2, [sp, #4]
00461698: mov r0, r4
0046169c: ldr r1, [r5, #0x14]
004616a0: asr r3, r2, #0x1f
004616a4: ldr ip, [r4]
004616a8: mov lr, pc
004616ac: ldr pc, [ip, #0x1c]
004616b0: add sp, sp, #0xc
004616b4: pop {r4, r5, pc}

_ZN11IStreamBase7writeAsIiEEvRKT_ 0x38b808
0038b808: str lr, [sp, #-4]!
0038b80c: mov r3, #0
0038b810: sub sp, sp, #0xc
0038b814: ldr ip, [r0]
0038b818: mov r2, #4
0038b81c: mov lr, pc
0038b820: ldr pc, [ip, #0x1c]
0038b824: ldr r3, [pc, #0x74]
0038b828: cmp r0, #4
0038b82c: add r3, pc, r3
0038b830: beq #0x38b860
0038b834: ldr r2, [pc, #0x68]
0038b838: ldr r2, [r3, r2]
0038b83c: ldr r2, [r2]
0038b840: cmp r2, #2
0038b844: moveq r3, #0
0038b848: streq r3, [r3]
0038b84c: beq #0x38b858
0038b850: cmp r2, #1
0038b854: beq #0x38b86c
0038b858: add sp, sp, #0xc
0038b85c: ldm sp!, {pc}
0038b860: cmp r1, #0
0038b864: beq #0x38b858
0038b868: b #0x38b834
0038b86c: ldr r0, [pc, #0x34]
0038b870: ldr r1, [pc, #0x34]
0038b874: ldr r2, [pc, #0x34]
0038b878: ldr r0, [r3, r0]
0038b87c: ldr r3, [pc, #0x30]
0038b880: mov ip, #0x4d
0038b884: add r1, pc, r1
0038b888: add r2, pc, r2
0038b88c: add r3, pc, r3
0038b890: add r0, r0, #0xa8
0038b894: str ip, [sp]
0038b898: bl #0x30e004
0038b89c: b #0x38b858
0038b8a0: rsbeq sb, r0, r4, ror #4
0038b8a4: andeq r3, r0, r0, asr #19
0038b8a8: andeq r1, r0, r0, asr #19
0038b8ac: subseq r2, r3, r4, asr fp
0038b8b0: subseq r2, r3, r8, lsl ip
0038b8b4: subseq r2, r3, ip, lsl #25

_ZN11IStreamBase7writeAsIjEEvRKT_ 0x461770
00461770: str lr, [sp, #-4]!
00461774: mov r3, #0
00461778: sub sp, sp, #0xc
0046177c: ldr ip, [r0]
00461780: mov r2, #4
00461784: mov lr, pc
00461788: ldr pc, [ip, #0x1c]
0046178c: ldr r3, [pc, #0x74]
00461790: cmp r0, #4
00461794: add r3, pc, r3
00461798: beq #0x4617c8
0046179c: ldr r2, [pc, #0x68]
004617a0: ldr r2, [r3, r2]
004617a4: ldr r2, [r2]
004617a8: cmp r2, #2
004617ac: moveq r3, #0
004617b0: streq r3, [r3]
004617b4: beq #0x4617c0
004617b8: cmp r2, #1
004617bc: beq #0x4617d4
004617c0: add sp, sp, #0xc
004617c4: ldm sp!, {pc}
004617c8: cmp r1, #0
004617cc: beq #0x4617c0
004617d0: b #0x46179c
004617d4: ldr r0, [pc, #0x34]
004617d8: ldr r1, [pc, #0x34]
004617dc: ldr r2, [pc, #0x34]
004617e0: ldr r0, [r3, r0]
004617e4: ldr r3, [pc, #0x30]
004617e8: mov ip, #0x4d
004617ec: add r1, pc, r1
004617f0: add r2, pc, r2
004617f4: add r3, pc, r3
004617f8: add r0, r0, #0xa8
004617fc: str ip, [sp]
00461800: bl #0x30e004
00461804: b #0x4617c0
00461808: ldrsheq r3, [r3], #-0x2c
0046180c: andeq r3, r0, r0, asr #19
00461810: andeq r1, r0, r0, asr #19
00461814: subeq ip, r5, ip, ror #23
00461818: strheq ip, [r5], #-0xc0
0046181c: subeq ip, r5, r4, lsr #26

_ZN11IStreamBase7writeAsIbEEvRKT_ 0x33e138
0033e138: str lr, [sp, #-4]!
0033e13c: mov r3, #0
0033e140: sub sp, sp, #0xc
0033e144: ldr ip, [r0]
0033e148: mov r2, #1
0033e14c: mov lr, pc
0033e150: ldr pc, [ip, #0x1c]
0033e154: ldr r3, [pc, #0x74]
0033e158: cmp r0, #1
0033e15c: add r3, pc, r3
0033e160: beq #0x33e190
0033e164: ldr r2, [pc, #0x68]
0033e168: ldr r2, [r3, r2]
0033e16c: ldr r2, [r2]
0033e170: cmp r2, #2
0033e174: moveq r3, #0
0033e178: streq r3, [r3]
0033e17c: beq #0x33e188
0033e180: cmp r2, #1
0033e184: beq #0x33e19c
0033e188: add sp, sp, #0xc
0033e18c: ldm sp!, {pc}
0033e190: cmp r1, #0
0033e194: beq #0x33e188
0033e198: b #0x33e164
0033e19c: ldr r0, [pc, #0x34]
0033e1a0: ldr r1, [pc, #0x34]
0033e1a4: ldr r2, [pc, #0x34]
0033e1a8: ldr r0, [r3, r0]
0033e1ac: ldr r3, [pc, #0x30]
0033e1b0: mov ip, #0x4d
0033e1b4: add r1, pc, r1
0033e1b8: add r2, pc, r2
0033e1bc: add r3, pc, r3
0033e1c0: add r0, r0, #0xa8
0033e1c4: str ip, [sp]
0033e1c8: bl #0x30e004
0033e1cc: b #0x33e188
0033e1d0: rsbeq r6, r5, r4, lsr sb
0033e1d4: andeq r3, r0, r0, asr #19
0033e1d8: andeq r1, r0, r0, asr #19
0033e1dc: subseq r0, r8, r4, lsr #4
0033e1e0: subseq r0, r8, r8, ror #5
0033e1e4: subseq r0, r8, ip, asr r3

_ZN14PlayerSavegame16__SavePlayerNameEP11IStreamBasePv 0x4688c0
004688c0: add r1, r1, #0x18
004688c4: b #0x461668

_ZN14PlayerSavegame17__SavePlayerLevelEP11IStreamBasePv 0x468930
00468930: add r1, r1, #0x30
00468934: b #0x38b808

_ZN14PlayerSavegame17__SavePlayerClassEP11IStreamBasePv 0x4698e4
004698e4: push {r4, r5, r6, r7, r8, lr}
004698e8: ldr r4, [pc, #0xa0]
004698ec: ldr r5, [pc, #0xa0]
004698f0: sub sp, sp, #0x20
004698f4: add r4, pc, r4
004698f8: ldr r3, [r4, r5]
004698fc: mov r7, r0
00469900: ldr r3, [r3]
00469904: str r3, [sp, #0x1c]
00469908: ldr r3, [r1, #0x34]
0046990c: cmp r3, #0
00469910: blt #0x469970
00469914: ldr r2, [pc, #0x7c]
00469918: ldr r2, [r4, r2]
0046991c: ldr r2, [r2]
00469920: cmp r3, r2
00469924: bhi #0x469970
00469928: ldr r2, [pc, #0x6c]
0046992c: add r6, sp, #4
00469930: ldr r2, [r4, r2]
00469934: ldr r2, [r2]
00469938: ldr r8, [r2, r3, lsl #2]
0046993c: str r6, [sp, #0x14]
00469940: str r6, [sp, #0x18]
00469944: mov r0, r8
00469948: bl #0x30de54
0046994c: mov r1, r8
00469950: add r2, r8, r0
00469954: mov r0, r6
00469958: bl #0x3116e8
0046995c: mov r0, r7
00469960: mov r1, r6
00469964: bl #0x461668
00469968: mov r0, r6
0046996c: bl #0x3139ac
00469970: ldr r3, [r4, r5]
00469974: ldr r2, [sp, #0x1c]
00469978: ldr r3, [r3]
0046997c: cmp r2, r3
00469980: bne #0x46998c
00469984: add sp, sp, #0x20
00469988: pop {r4, r5, r6, r7, r8, pc}
0046998c: bl #0x30e310

_ZN14PlayerSavegame21__SaveDifficultyLevelEP11IStreamBasePv 0x4688f8
004688f8: ldr r3, [pc, #0x28]
004688fc: ldr r2, [pc, #0x28]
00468900: push {r4, r5, r6, lr}
00468904: add r3, pc, r3
00468908: mov r4, r1
0046890c: ldr r1, [r3, r2]
00468910: mov r5, r0
00468914: bl #0x38b808
00468918: mov r0, r5
0046891c: add r1, r4, #0x3c
00468920: pop {r4, r5, r6, lr}
00468924: b #0x38b808
00468928: subseq ip, r2, ip, lsl #3
0046892c: muleq r0, ip, sl

_ZN14PlayerSavegame15__SaveLevelNameEP11IStreamBasePv 0x468b20
00468b20: push {r4, r5, r6, lr}
00468b24: mov r5, r1
00468b28: add r1, r1, #0x38
00468b2c: mov r6, r0
00468b30: bl #0x461770
00468b34: mov r4, #0
00468b38: add r1, r4, #0x14
00468b3c: add r1, r5, r1, lsl #2
00468b40: mov r0, r6
00468b44: bl #0x38b808
00468b48: add r1, r5, r4, lsl #2
00468b4c: add r1, r1, #0x5c
00468b50: mov r0, r6
00468b54: bl #0x38b808
00468b58: add r1, r5, r4, lsl #2
00468b5c: add r1, r1, #0xfc
00468b60: add r4, r4, #1
00468b64: mov r0, r6
00468b68: bl #0x38b808
00468b6c: cmp r4, #3
00468b70: bne #0x468b38
00468b74: pop {r4, r5, r6, pc}

_ZN14PlayerSavegame21__SaveLevelEntryPointEP11IStreamBasePv 0x4688c8
004688c8: push {r4, r5, r6, lr}
004688cc: mov r4, r1
004688d0: mov r5, r0
004688d4: add r1, r1, #0x40
004688d8: bl #0x38b808
004688dc: mov r0, r5
004688e0: add r1, r4, #0x44
004688e4: bl #0x38b808
004688e8: mov r0, r5
004688ec: add r1, r4, #0x48
004688f0: pop {r4, r5, r6, lr}
004688f4: b #0x38b808

_ZN14PlayerSavegame19__SaveUseSpawnPointEP11IStreamBasePv 0x4689a8
004689a8: push {r4, r5, r6, lr}
004689ac: mov r4, r1
004689b0: mov r5, r0
004689b4: add r1, r1, #0x4c
004689b8: bl #0x33e138
004689bc: mov r0, r5
004689c0: add r1, r4, #0x4d
004689c4: bl #0x33e138
004689c8: mov r0, r5
004689cc: add r1, r4, #0x4e
004689d0: pop {r4, r5, r6, lr}
004689d4: b #0x33e138
