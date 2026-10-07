_ZN9Character6UpdateEv 0x3abe98
003abe98: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003abe9c: ldr r5, [pc, #0xe54]
003abea0: ldr r6, [pc, #0xe54]
003abea4: mov r4, r0
003abea8: add r5, pc, r5
003abeac: ldr r3, [r5, r6]
003abeb0: ldr r0, [pc, #0xe48]
003abeb4: ldr r7, [pc, #0xe48]
003abeb8: ldr r3, [r3]
003abebc: sub sp, sp, #0x9c
003abec0: add r0, pc, r0
003abec4: str r3, [sp, #0x94]
003abec8: bl #0x3136b4
003abecc: ldr sl, [r5, r7]
003abed0: add r8, sp, #0x7c
003abed4: mov r0, sl
003abed8: bl #0x337888
003abedc: ldr r1, [pc, #0xe24]
003abee0: add r2, sp, #0x48
003abee4: mov r0, r8
003abee8: add r1, pc, r1
003abeec: bl #0x3140ec
003abef0: mov r0, sl
003abef4: mov r1, r8
003abef8: bl #0x337a88
003abefc: mov sl, r0
003abf00: mov r0, r8
003abf04: bl #0x318254
003abf08: cmp sl, #0
003abf0c: bne #0x3ac300
003abf10: ldr sl, [r5, r7]
003abf14: add r8, sp, #0x64
003abf18: mov r0, sl
003abf1c: bl #0x337888
003abf20: ldr r1, [pc, #0xde4]
003abf24: add r2, sp, #0x44
003abf28: mov r0, r8
003abf2c: add r1, pc, r1
003abf30: bl #0x3140ec
003abf34: mov r0, sl
003abf38: mov r1, r8
003abf3c: bl #0x337a88
003abf40: mov sl, r0
003abf44: mov r0, r8
003abf48: bl #0x318254
003abf4c: cmp sl, #0
003abf50: bne #0x3ac2d8
003abf54: ldr r3, [r4]
003abf58: mov r0, r4
003abf5c: mov lr, pc
003abf60: ldr pc, [r3, #0x148]
003abf64: cmp r0, #0
003abf68: beq #0x3ac204
003abf6c: ldr r0, [pc, #0xd9c]
003abf70: add r8, r4, #0x4f0
003abf74: add r8, r8, #0xc
003abf78: add r0, pc, r0
003abf7c: bl #0x3136b4
003abf80: mov r0, r8
003abf84: bl #0x3c01ac
003abf88: cmp r0, #0
003abf8c: bne #0x3ac348
003abf90: ldr sl, [pc, #0xd7c]
003abf94: add r2, r4, #0x3c8
003abf98: str r2, [sp, #8]
003abf9c: ldr r0, [pc, #0xd74]
003abfa0: add sb, sp, #0x4c
003abfa4: add r0, pc, r0
003abfa8: bl #0x3136b8
003abfac: ldr fp, [r5, sl]
003abfb0: ldr r3, [fp, #0x38]
003abfb4: ldr r2, [r3, #0x5c]
003abfb8: add r2, r2, #1
003abfbc: str r2, [r3, #0x5c]
003abfc0: ldr r3, [r4, #0x378]
003abfc4: mov r0, r3
003abfc8: ldr r3, [r3]
003abfcc: mov lr, pc
003abfd0: ldr pc, [r3, #8]
003abfd4: ldr r7, [r5, r7]
003abfd8: mov r0, r7
003abfdc: bl #0x337888
003abfe0: ldr r1, [pc, #0xd34]
003abfe4: add r2, sp, #0x40
003abfe8: mov r0, sb
003abfec: add r1, pc, r1
003abff0: bl #0x3140ec
003abff4: mov r0, r7
003abff8: mov r1, sb
003abffc: bl #0x337a88
003ac000: mov r7, r0
003ac004: mov r0, sb
003ac008: bl #0x318254
003ac00c: cmp r7, #0
003ac010: bne #0x3ac618
003ac014: movw r3, #0x14e0
003ac018: ldr r0, [r4, r3]
003ac01c: cmp r0, #0
003ac020: beq #0x3ac028
003ac024: bl #0x317ae4
003ac028: add r0, r4, #0x3b4
003ac02c: bl #0x3db640
003ac030: ldr r0, [sp, #8]
003ac034: bl #0x3cfbf4
003ac038: mov r0, r8
003ac03c: bl #0x3c628c
003ac040: add r0, r4, #0x490
003ac044: add r0, r0, #0xc
003ac048: bl #0x3caf3c
003ac04c: movw r7, #0x14fc
003ac050: mov r0, r4
003ac054: bl #0x38cbe8
003ac058: ldr r8, [r4, r7]
003ac05c: mov r1, #0
003ac060: mov r0, r8
003ac064: bl #0x30e2f8
003ac068: cmp r0, #0
003ac06c: bne #0x3ac894
003ac070: ldr r3, [pc, #0xca8]
003ac074: ldr r3, [r5, r3]
003ac078: ldrb r3, [r3]
003ac07c: cmp r3, #0
003ac080: bne #0x3ac22c
003ac084: ldr r3, [pc, #0xc98]
003ac088: ldr r3, [r5, r3]
003ac08c: ldrb r3, [r3]
003ac090: cmp r3, #0
003ac094: bne #0x3ac22c
003ac098: ldr r3, [r4]
003ac09c: mov r0, r4
003ac0a0: mov lr, pc
003ac0a4: ldr pc, [r3, #0x28]
003ac0a8: cmp r0, #0
003ac0ac: beq #0x3ac27c
003ac0b0: mov r0, r4
003ac0b4: bl #0x3abb9c
003ac0b8: mov r0, r4
003ac0bc: bl #0x3a469c
003ac0c0: ldr r3, [r5, sl]
003ac0c4: ldr r3, [r3, #0x74]
003ac0c8: tst r3, #7
003ac0cc: beq #0x3ac888
003ac0d0: mov r3, #0x1480
003ac0d4: ldrb r3, [r4, r3]
003ac0d8: cmp r3, #0
003ac0dc: beq #0x3ac740
003ac0e0: movw r3, #0x1494
003ac0e4: ldr r3, [r4, r3]
003ac0e8: cmp r3, #0
003ac0ec: beq #0x3ac10c
003ac0f0: movw r2, #0x1498
003ac0f4: ldr r2, [r4, r2]
003ac0f8: cmp r2, #0
003ac0fc: blt #0x3ac10c
003ac100: ldr r0, [r3, r2, lsl #2]
003ac104: mov r1, #0
003ac108: bl #0x492ef0
003ac10c: movw r3, #0x149c
003ac110: ldr r0, [r4, r3]
003ac114: cmp r0, #0
003ac118: beq #0x3ac124
003ac11c: mov r1, #0
003ac120: bl #0x492ef0
003ac124: bl #0x3a42f4
003ac128: cmp r0, #0
003ac12c: beq #0x3ac148
003ac130: movw r3, #0x14a0
003ac134: ldr r0, [r4, r3]
003ac138: cmp r0, #0
003ac13c: beq #0x3ac148
003ac140: mov r1, #1
003ac144: bl #0x492ef0
003ac148: ldr r3, [pc, #0xbd8]
003ac14c: ldr r0, [r5, r3]
003ac150: bl #0x455c54
003ac154: cmp r0, #0
003ac158: bne #0x3ac1f4
003ac15c: mov r7, #0x1500
003ac160: ldr r1, [r4, r7]
003ac164: cmn r1, #1
003ac168: beq #0x3ac17c
003ac16c: mov r0, r4
003ac170: bl #0x3aef68
003ac174: mvn r3, #0
003ac178: str r3, [r4, r7]
003ac17c: movw r7, #0x1504
003ac180: ldr r3, [r4, r7]
003ac184: cmn r3, #1
003ac188: beq #0x3ac1f4
003ac18c: ldr sb, [r5, sl]
003ac190: mov r1, r4
003ac194: ldr r0, [sb, #0x40]
003ac198: bl #0x36effc
003ac19c: cmp r0, #0
003ac1a0: beq #0x3ac1f4
003ac1a4: bl #0x413e90
003ac1a8: ldr r1, [pc, #0xb7c]
003ac1ac: ldr sl, [r4, r7]
003ac1b0: add r1, pc, r1
003ac1b4: bl #0x414678
003ac1b8: ldr r1, [pc, #0xb70]
003ac1bc: ldr r2, [pc, #0xb70]
003ac1c0: mov r8, r0
003ac1c4: add r1, pc, r1
003ac1c8: add r2, pc, r2
003ac1cc: ldr r0, [sb, #0x2c]
003ac1d0: bl #0x4c4bdc
003ac1d4: asr sl, sl, #8
003ac1d8: mov r3, r0
003ac1dc: mov r1, sl
003ac1e0: mov r0, r4
003ac1e4: mov r2, r8
003ac1e8: bl #0x3af0c8
003ac1ec: mvn r3, #0
003ac1f0: str r3, [r4, r7]
003ac1f4: ldr r0, [pc, #0xb3c]
003ac1f8: add r0, pc, r0
003ac1fc: bl #0x3136b8
003ac200: b #0x3ac210
003ac204: ldr r0, [pc, #0xb30]
003ac208: add r0, pc, r0
003ac20c: bl #0x3136b8
003ac210: ldr r3, [r5, r6]
003ac214: ldr r2, [sp, #0x94]
003ac218: ldr r3, [r3]
003ac21c: cmp r2, r3
003ac220: bne #0x3aca98
003ac224: add sp, sp, #0x9c
003ac228: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ac22c: mov r0, r4
003ac230: bl #0x3a3094
003ac234: cmp r0, #0
003ac238: beq #0x3ac098
003ac23c: ldr r3, [pc, #0xafc]
003ac240: ldr r1, [r4, #0x164]
003ac244: ldr r0, [r4, #0x160]
003ac248: ldr r3, [r5, r3]
003ac24c: ldr r2, [r4, #0x168]
003ac250: str r1, [r3, #4]
003ac254: mov r1, #0
003ac258: str r0, [r3]
003ac25c: str r1, [r3, #0xc]
003ac260: str r2, [r3, #8]
003ac264: ldr r3, [r4]
003ac268: mov r0, r4
003ac26c: mov lr, pc
003ac270: ldr pc, [r3, #0x28]
003ac274: cmp r0, #0
003ac278: bne #0x3ac0b0
003ac27c: mov r0, r4
003ac280: bl #0x3a30c4
003ac284: cmp r0, #0
003ac288: bne #0x3ac574
003ac28c: movw r3, #0x14e4
003ac290: ldrb r3, [r4, r3]
003ac294: cmp r3, #0
003ac298: bne #0x3ac900
003ac29c: bl #0x7fd794
003ac2a0: ldrb r3, [r0, #5]
003ac2a4: cmp r3, #0
003ac2a8: beq #0x3ac0c0
003ac2ac: ldrb r3, [r4, #0x118]
003ac2b0: cmp r3, #0
003ac2b4: beq #0x3ac0c0
003ac2b8: ldr r3, [r4, #0x520]
003ac2bc: tst r3, #0x100
003ac2c0: beq #0x3ac0c0
003ac2c4: ldr r3, [r4, #0x110]
003ac2c8: cmn r3, #1
003ac2cc: moveq r3, #0
003ac2d0: strbeq r3, [r4, #0x118]
003ac2d4: b #0x3ac0c0
003ac2d8: ldr r3, [r4]
003ac2dc: mov r0, r4
003ac2e0: mov lr, pc
003ac2e4: ldr pc, [r3, #0x28]
003ac2e8: cmp r0, #0
003ac2ec: beq #0x3abf54
003ac2f0: add r0, r4, #0x37c
003ac2f4: mov r1, #0x32
003ac2f8: bl #0x3ffc40
003ac2fc: b #0x3abf54
003ac300: ldr sl, [pc, #0xa0c]
003ac304: mov r1, #0
003ac308: mov r2, r1
003ac30c: ldr r3, [r5, sl]
003ac310: ldr r0, [r3, #0x40]
003ac314: bl #0x36e744
003ac318: ldr r3, [r0, #0x660]
003ac31c: cmp r4, r3
003ac320: bne #0x3abf10
003ac324: movw r3, #0x1088
003ac328: ldr r2, [r4, r3]
003ac32c: cmp r2, #0
003ac330: ble #0x3ac940
003ac334: sub r2, r2, #0xc8
003ac338: add r0, r4, #0x560
003ac33c: mov r1, #0x24
003ac340: bl #0x3e07a0
003ac344: b #0x3abf10
003ac348: mov r0, r8
003ac34c: bl #0x3c01ac
003ac350: cmp r0, #0xc
003ac354: beq #0x3ac608
003ac358: mov r0, r8
003ac35c: bl #0x3c01ac
003ac360: cmp r0, #2
003ac364: beq #0x3abf90
003ac368: mov r3, #0x1480
003ac36c: ldrb r3, [r4, r3]
003ac370: cmp r3, #0
003ac374: bne #0x3ac608
003ac378: ldr r3, [r4, #0x3e4]
003ac37c: cmp r3, #0
003ac380: bne #0x3abf90
003ac384: ldr sl, [pc, #0x988]
003ac388: ldr r0, [r5, sl]
003ac38c: bl #0x31f594
003ac390: ldr r3, [r0, #0x3c]
003ac394: cmp r3, #0x1d
003ac398: beq #0x3aca48
003ac39c: ldr sb, [pc, #0x9a0]
003ac3a0: ldr r3, [r5, sb]
003ac3a4: ldr r3, [r3, #0x10]
003ac3a8: cmp r3, #8
003ac3ac: movls r3, #0
003ac3b0: movhi r3, #1
003ac3b4: cmp r3, #0
003ac3b8: beq #0x3ac3f4
003ac3bc: ldr r3, [r5, sb]
003ac3c0: ldr fp, [r3, #8]
003ac3c4: cmp fp, r3
003ac3c8: beq #0x3ac3f4
003ac3cc: ldr r3, [fp, #0x14]
003ac3d0: mov r1, #0
003ac3d4: mov r2, #1
003ac3d8: ldr r0, [r3, #0x378]
003ac3dc: bl #0x40570c
003ac3e0: add r1, sp, #0x98
003ac3e4: ldr r0, [fp, #0x14]
003ac3e8: mov r2, #1
003ac3ec: str fp, [r1, #-0x5c]!
003ac3f0: bl #0x3a7b24
003ac3f4: add r3, r4, #0x3c8
003ac3f8: mov r0, r3
003ac3fc: mov r1, #1
003ac400: str r3, [sp, #8]
003ac404: bl #0x3cf3a4
003ac408: cmp r0, #0
003ac40c: beq #0x3abf9c
003ac410: mov r0, r4
003ac414: bl #0x3a3064
003ac418: cmp r0, #0
003ac41c: beq #0x3abf9c
003ac420: mov r0, r4
003ac424: bl #0x3a3144
003ac428: cmp r0, #0
003ac42c: bne #0x3abf9c
003ac430: mov r0, r4
003ac434: bl #0x3a3158
003ac438: cmp r0, #0
003ac43c: bne #0x3abf9c
003ac440: bl #0x7fd794
003ac444: ldrb r3, [r0, #5]
003ac448: cmp r3, #0
003ac44c: bne #0x3abf9c
003ac450: ldrb r3, [r4, #0x3ec]
003ac454: cmp r3, #0
003ac458: beq #0x3abf9c
003ac45c: bl #0x60b0cc
003ac460: ldr r2, [r5, sb]
003ac464: ldr ip, [r2, #4]
003ac468: cmp ip, #0
003ac46c: moveq ip, r2
003ac470: bne #0x3ac480
003ac474: b #0x3ac4b4
003ac478: mov r2, ip
003ac47c: mov ip, r3
003ac480: ldr r3, [ip, #0x10]
003ac484: cmp r0, r3
003ac488: ldrgt r3, [ip, #0xc]
003ac48c: ldrle r3, [ip, #8]
003ac490: movgt ip, r2
003ac494: cmp r3, #0
003ac498: bne #0x3ac478
003ac49c: ldr r3, [r5, sb]
003ac4a0: cmp ip, r3
003ac4a4: beq #0x3ac4b4
003ac4a8: ldr r3, [ip, #0x10]
003ac4ac: cmp r0, r3
003ac4b0: bge #0x3ac56c
003ac4b4: ldr r3, [r5, sb]
003ac4b8: mov fp, #0
003ac4bc: str r0, [sp, #0x30]
003ac4c0: ldr lr, [r3, #8]
003ac4c4: str fp, [sp, #0x34]
003ac4c8: cmp lr, ip
003ac4cc: beq #0x3acae4
003ac4d0: cmp ip, r3
003ac4d4: beq #0x3acc50
003ac4d8: ldrb r3, [ip]
003ac4dc: cmp r3, #0
003ac4e0: bne #0x3ac4f8
003ac4e4: ldr r3, [ip, #4]
003ac4e8: ldr r3, [r3, #4]
003ac4ec: cmp r3, ip
003ac4f0: ldreq lr, [ip, #0xc]
003ac4f4: beq #0x3ac518
003ac4f8: ldr lr, [ip, #8]
003ac4fc: cmp lr, #0
003ac500: bne #0x3ac50c
003ac504: b #0x3acc7c
003ac508: mov lr, r3
003ac50c: ldr r3, [lr, #0xc]
003ac510: cmp r3, #0
003ac514: bne #0x3ac508
003ac518: ldr r2, [ip, #0x10]
003ac51c: cmp r0, r2
003ac520: movge fp, #0
003ac524: movlt fp, #1
003ac528: cmp fp, #0
003ac52c: str r2, [sp, #0xc]
003ac530: beq #0x3aca9c
003ac534: ldr r3, [lr, #0x10]
003ac538: cmp r0, r3
003ac53c: ble #0x3aca9c
003ac540: ldr r3, [lr, #0xc]
003ac544: cmp r3, #0
003ac548: movne r1, ip
003ac54c: beq #0x3acbf8
003ac550: mov ip, #0
003ac554: add r0, sp, #0x38
003ac558: add r2, sp, #0x30
003ac55c: mov r3, r1
003ac560: str ip, [sp]
003ac564: bl #0x3aa77c
003ac568: ldr ip, [sp, #0x38]
003ac56c: str r4, [ip, #0x14]
003ac570: b #0x3abf9c
003ac574: ldrb r3, [r4, #0x8a]
003ac578: cmp r3, #0
003ac57c: beq #0x3ac28c
003ac580: ldrb r3, [r4, #0x2fa]
003ac584: cmp r3, #0
003ac588: bne #0x3ac5e0
003ac58c: movw r7, #0x1484
003ac590: ldr r3, [r4, r7]
003ac594: cmp r3, #0
003ac598: bne #0x3ac0c0
003ac59c: ldr r3, [pc, #0x7a4]
003ac5a0: mov r1, #0x8b
003ac5a4: mov r2, r4
003ac5a8: ldr r0, [r5, r3]
003ac5ac: bl #0x495430
003ac5b0: cmp r0, #0
003ac5b4: str r0, [r4, r7]
003ac5b8: beq #0x3ac0c0
003ac5bc: bl #0x49267c
003ac5c0: ldr r3, [r0]
003ac5c4: mov lr, pc
003ac5c8: ldr pc, [r3, #0x44]
003ac5cc: mov r1, #1
003ac5d0: ldr r3, [r0]
003ac5d4: mov lr, pc
003ac5d8: ldr pc, [r3, #0x40]
003ac5dc: b #0x3ac0c0
003ac5e0: movw r3, #0x1484
003ac5e4: ldr r3, [r4, r3]
003ac5e8: cmp r3, #0
003ac5ec: beq #0x3ac0c0
003ac5f0: ldr r3, [pc, #0x750]
003ac5f4: add r1, r4, #0x1480
003ac5f8: add r1, r1, #4
003ac5fc: ldr r0, [r5, r3]
003ac600: bl #0x494978
003ac604: b #0x3ac0c0
003ac608: add r3, r4, #0x3c8
003ac60c: ldr sl, [pc, #0x700]
003ac610: str r3, [sp, #8]
003ac614: b #0x3abf9c
003ac618: bl #0x7fd794
003ac61c: ldrb r3, [r0, #5]
003ac620: cmp r3, #0
003ac624: beq #0x3ac014
003ac628: ldr r3, [r4]
003ac62c: mov r0, r4
003ac630: mov lr, pc
003ac634: ldr pc, [r3, #0x28]
003ac638: cmp r0, #0
003ac63c: beq #0x3ac014
003ac640: ldr r0, [fp, #0x40]
003ac644: mov r1, r4
003ac648: mov r2, #0
003ac64c: bl #0x36eea8
003ac650: ldrb r3, [r0, #0x66c]
003ac654: cmp r3, #0
003ac658: beq #0x3ac014
003ac65c: mov r1, r4
003ac660: ldr r0, [fp, #0x40]
003ac664: mov r2, #0
003ac668: bl #0x36eea8
003ac66c: ldr r1, [r0, #0x678]
003ac670: cmp r1, #0
003ac674: ble #0x3ac014
003ac678: ldr r0, [fp, #0x40]
003ac67c: sub r1, r1, #1
003ac680: mov r2, #0
003ac684: bl #0x36e744
003ac688: ldr r7, [r0, #0x660]
003ac68c: cmp r7, #0
003ac690: beq #0x3ac014
003ac694: ldr sb, [pc, #0x6b0]
003ac698: mov r0, r4
003ac69c: add sb, pc, sb
003ac6a0: ldr r2, [sb, #0x24]
003ac6a4: ldr r3, [sb, #0x28]
003ac6a8: add r2, r2, #1
003ac6ac: add r3, r3, #1
003ac6b0: str r2, [sb, #0x24]
003ac6b4: str r3, [sb, #0x28]
003ac6b8: bl #0x3bd110
003ac6bc: cmp r0, #0x45
003ac6c0: ble #0x3aca18
003ac6c4: movw r3, #0x14a4
003ac6c8: ldr r0, [r4, r3]
003ac6cc: cmp r0, #0
003ac6d0: beq #0x3ac6f4
003ac6d4: movw r3, #0x14ac
003ac6d8: ldrb r3, [r4, r3]
003ac6dc: cmp r3, #0
003ac6e0: beq #0x3ac978
003ac6e4: movw r3, #0x14a8
003ac6e8: ldrsb r3, [r4, r3]
003ac6ec: cmp r3, #8
003ac6f0: beq #0x3ac990
003ac6f4: add fp, r7, #0x160
003ac6f8: add sb, r4, #0x160
003ac6fc: mov r1, sb
003ac700: mov r0, fp
003ac704: bl #0x3a4118
003ac708: mov r1, #0x43000000
003ac70c: add r1, r1, #0x960000
003ac710: bl #0x30e2f8
003ac714: cmp r0, #0
003ac718: bne #0x3aca70
003ac71c: ldr r3, [pc, #0x62c]
003ac720: add r3, pc, r3
003ac724: ldr r2, [r3, #0x28]
003ac728: cmp r2, #0x3e8
003ac72c: bgt #0x3aca80
003ac730: ldr r0, [r4, #0x378]
003ac734: mov r1, #0
003ac738: bl #0x4053d0
003ac73c: b #0x3ac014
003ac740: ldr r3, [r5, sl]
003ac744: mov r1, r4
003ac748: ldr r0, [r3, #0x40]
003ac74c: bl #0x36effc
003ac750: cmp r0, #0
003ac754: beq #0x3ac0e0
003ac758: movw r3, #0x1494
003ac75c: ldr r3, [r4, r3]
003ac760: cmp r3, #0
003ac764: beq #0x3ac820
003ac768: ldr r7, [r4, #0x40c]
003ac76c: cmp r7, #0
003ac770: beq #0x3ac8b4
003ac774: cmp r4, r7
003ac778: beq #0x3ac8b4
003ac77c: movw r3, #0x149c
003ac780: ldr r0, [r4, r3]
003ac784: cmp r0, #0
003ac788: beq #0x3ac794
003ac78c: mov r1, #0
003ac790: bl #0x492ef0
003ac794: ldr r3, [r7]
003ac798: mov r0, r7
003ac79c: mov r1, r4
003ac7a0: mov lr, pc
003ac7a4: ldr pc, [r3, #0x90]
003ac7a8: subs r8, r0, #0
003ac7ac: blt #0x3ac820
003ac7b0: cmp r8, #1
003ac7b4: beq #0x3ac930
003ac7b8: movw sb, #0x1494
003ac7bc: ldr r3, [r4, sb]
003ac7c0: ldr r0, [r3, r8, lsl #2]
003ac7c4: cmp r0, #0
003ac7c8: beq #0x3ac820
003ac7cc: str r7, [r0, #0x28]
003ac7d0: mov r1, #1
003ac7d4: bl #0x492aa0
003ac7d8: movw r3, #0x1498
003ac7dc: ldr r3, [r4, r3]
003ac7e0: cmp r8, r3
003ac7e4: beq #0x3ac820
003ac7e8: cmp r3, #0
003ac7ec: blt #0x3ac808
003ac7f0: ldr r2, [r4, sb]
003ac7f4: ldr r0, [r2, r3, lsl #2]
003ac7f8: cmp r0, #0
003ac7fc: beq #0x3ac80c
003ac800: mov r1, #0
003ac804: bl #0x492ef0
003ac808: ldr r2, [r4, sb]
003ac80c: ldr r0, [r2, r8, lsl #2]
003ac810: mov r1, #1
003ac814: bl #0x492ef0
003ac818: movw r3, #0x1498
003ac81c: str r8, [r4, r3]
003ac820: movw r3, #0x149c
003ac824: ldr r0, [r4, r3]
003ac828: cmp r0, #0
003ac82c: beq #0x3ac124
003ac830: ldr r3, [r4, #0x378]
003ac834: ldrb r2, [r3, #9]
003ac838: cmp r2, #0
003ac83c: bne #0x3ac860
003ac840: ldr r2, [pc, #0x50c]
003ac844: ldr r2, [r5, r2]
003ac848: ldrb r2, [r2]
003ac84c: cmp r2, #0
003ac850: bne #0x3ac11c
003ac854: ldrb r3, [r3, #8]
003ac858: cmp r3, #0
003ac85c: bne #0x3ac11c
003ac860: ldrb r3, [r4, #0x1b5]
003ac864: cmp r3, #0
003ac868: beq #0x3ac11c
003ac86c: ldr r0, [r5, sl]
003ac870: bl #0x320e74
003ac874: cmp r0, #0
003ac878: beq #0x3ac124
003ac87c: movw r3, #0x149c
003ac880: ldr r0, [r4, r3]
003ac884: b #0x3ac11c
003ac888: mov r0, r4
003ac88c: bl #0x3a4470
003ac890: b #0x3ac0d0
003ac894: ldr r0, [r5, sl]
003ac898: bl #0x31f66c
003ac89c: bl #0x30e2e0
003ac8a0: mov r1, r0
003ac8a4: mov r0, r8
003ac8a8: bl #0x30e3ac
003ac8ac: str r0, [r4, r7]
003ac8b0: b #0x3ac070
003ac8b4: movw r2, #0x14a4
003ac8b8: ldr r7, [r4, r2]
003ac8bc: cmp r7, #0
003ac8c0: bne #0x3ac8f4
003ac8c4: movw r7, #0x1498
003ac8c8: ldr r2, [r4, r7]
003ac8cc: cmp r2, #0
003ac8d0: blt #0x3ac820
003ac8d4: ldr r0, [r3, r2, lsl #2]
003ac8d8: cmp r0, #0
003ac8dc: beq #0x3ac820
003ac8e0: mov r1, #0
003ac8e4: bl #0x492ef0
003ac8e8: mvn r3, #0
003ac8ec: str r3, [r4, r7]
003ac8f0: b #0x3ac820
003ac8f4: cmp r4, r7
003ac8f8: beq #0x3ac8c4
003ac8fc: b #0x3ac77c
003ac900: bl #0x7fd794
003ac904: ldrb r3, [r0, #5]
003ac908: cmp r3, #0
003ac90c: beq #0x3ac29c
003ac910: movw r2, #0x14e5
003ac914: ldrb r3, [r4, r2]
003ac918: cmp r3, #0
003ac91c: moveq r1, #1
003ac920: strbeq r1, [r4, r2]
003ac924: strbeq r3, [r4, #0x118]
003ac928: beq #0x3ac0c0
003ac92c: b #0x3ac29c
003ac930: mov r0, r7
003ac934: mov r1, r4
003ac938: bl #0x3ec048
003ac93c: b #0x3ac7b8
003ac940: mov r1, #0x24
003ac944: add r0, r4, #0x560
003ac948: mov r2, #0
003ac94c: bl #0x3e07a0
003ac950: ldr r3, [r4]
003ac954: mov r0, r4
003ac958: mov lr, pc
003ac95c: ldr pc, [r3, #0x34]
003ac960: subs r1, r0, #0
003ac964: bne #0x3abf10
003ac968: ldr r0, [r4, #0x378]
003ac96c: mov r2, r1
003ac970: bl #0x40570c
003ac974: b #0x3abf10
003ac978: movw r3, #0x14a8
003ac97c: ldrsb r3, [r4, r3]
003ac980: cmp r3, #1
003ac984: beq #0x3ac9c8
003ac988: cmn r3, #2
003ac98c: beq #0x3ac9c8
003ac990: add sb, r4, #0x160
003ac994: mov r1, sb
003ac998: add r0, r0, #0x160
003ac99c: bl #0x3a4118
003ac9a0: mov r1, #0x43000000
003ac9a4: add r1, r1, #0x960000
003ac9a8: bl #0x30e2f8
003ac9ac: subs r1, r0, #0
003ac9b0: beq #0x3aca64
003ac9b4: movw r3, #0x14a4
003ac9b8: ldr r1, [r4, r3]
003ac9bc: ldr r0, [r4, #0x378]
003ac9c0: bl #0x4053d0
003ac9c4: b #0x3ac014
003ac9c8: ldr r3, [r0, #0x3bc]
003ac9cc: cmp r4, r3
003ac9d0: bne #0x3ac6f4
003ac9d4: add r0, r4, #0x37c
003ac9d8: bl #0x3fe330
003ac9dc: cmp r0, #0
003ac9e0: bne #0x3ac6f4
003ac9e4: add fp, r7, #0x160
003ac9e8: add sb, r4, #0x160
003ac9ec: mov r1, sb
003ac9f0: mov r0, fp
003ac9f4: bl #0x3a4118
003ac9f8: mov r1, #0x43000000
003ac9fc: add r1, r1, #0xfa0000
003aca00: bl #0x30e70c
003aca04: cmp r0, #0
003aca08: beq #0x3ac6fc
003aca0c: movw r3, #0x14a4
003aca10: ldr r0, [r4, r3]
003aca14: b #0x3ac994
003aca18: mov r0, r4
003aca1c: bl #0x3a5990
003aca20: cmp r0, #0
003aca24: beq #0x3ac6c4
003aca28: ldr r3, [sb, #0x24]
003aca2c: cmp r3, #0x64
003aca30: ble #0x3ac6c4
003aca34: ldr r0, [r4, #0x378]
003aca38: bl #0x4056b0
003aca3c: mov r3, #0
003aca40: str r3, [sb, #0x24]
003aca44: b #0x3ac014
003aca48: ldr sb, [pc, #0x2f4]
003aca4c: ldr r3, [r5, sb]
003aca50: ldr r3, [r3, #0x10]
003aca54: cmp r3, #0x18
003aca58: movls r3, #0
003aca5c: movhi r3, #1
003aca60: b #0x3ac3b4
003aca64: ldr r0, [r4, #0x378]
003aca68: bl #0x4057fc
003aca6c: b #0x3ac014
003aca70: mov r1, r7
003aca74: ldr r0, [r4, #0x378]
003aca78: bl #0x4053d0
003aca7c: b #0x3ac014
003aca80: str r0, [r3, #0x28]
003aca84: ldr r3, [r4]
003aca88: mov r0, r4
003aca8c: mov lr, pc
003aca90: ldr pc, [r3, #0x138]
003aca94: b #0x3ac730
003aca98: bl #0x30e310
003aca9c: ldr r3, [ip, #0xc]
003acaa0: cmp r3, #0
003acaa4: beq #0x3acb7c
003acaa8: mov r1, r3
003acaac: b #0x3acab4
003acab0: mov r1, r2
003acab4: ldr r2, [r1, #8]
003acab8: cmp r2, #0
003acabc: bne #0x3acab0
003acac0: cmp fp, #0
003acac4: beq #0x3acb28
003acac8: add r0, sp, #0x10
003acacc: add r1, sp, #0x30
003acad0: bl #0x3aa8b4
003acad4: ldr r3, [sp, #0x10]
003acad8: str r3, [sp, #0x38]
003acadc: ldr ip, [sp, #0x38]
003acae0: b #0x3ac56c
003acae4: ldr r3, [r3, #0x10]
003acae8: cmp r3, fp
003acaec: beq #0x3acc34
003acaf0: ldr r3, [ip, #0x10]
003acaf4: cmp r0, r3
003acaf8: blt #0x3acc14
003acafc: ble #0x3acb70
003acb00: ldr r3, [ip, #0xc]
003acb04: cmp r3, #0
003acb08: beq #0x3acbac
003acb0c: mov r1, r3
003acb10: b #0x3acb18
003acb14: mov r1, r2
003acb18: ldr r2, [r1, #8]
003acb1c: cmp r2, #0
003acb20: bne #0x3acb14
003acb24: b #0x3acbd8
003acb28: ldr r2, [sp, #0xc]
003acb2c: cmp r0, r2
003acb30: ble #0x3acb70
003acb34: ldr r2, [r5, sb]
003acb38: cmp r1, r2
003acb3c: beq #0x3acb4c
003acb40: ldr r2, [r1, #0x10]
003acb44: cmp r0, r2
003acb48: bge #0x3acac8
003acb4c: cmp r3, #0
003acb50: bne #0x3ac550
003acb54: mov r1, ip
003acb58: add r0, sp, #0x38
003acb5c: add r2, sp, #0x30
003acb60: str ip, [sp]
003acb64: bl #0x3aa77c
003acb68: ldr ip, [sp, #0x38]
003acb6c: b #0x3ac56c
003acb70: str ip, [sp, #0x38]
003acb74: ldr ip, [sp, #0x38]
003acb78: b #0x3ac56c
003acb7c: ldr r1, [ip, #4]
003acb80: mov r2, ip
003acb84: b #0x3acb90
003acb88: mov r2, r1
003acb8c: ldr r1, [r1, #4]
003acb90: ldr lr, [r1, #0xc]
003acb94: cmp lr, r2
003acb98: beq #0x3acb88
003acb9c: ldr lr, [r2, #0xc]
003acba0: cmp r1, lr
003acba4: moveq r1, r2
003acba8: b #0x3acac0
003acbac: ldr r1, [ip, #4]
003acbb0: mov r2, ip
003acbb4: ldr ip, [r1, #0xc]
003acbb8: cmp ip, r2
003acbbc: bne #0x3acbcc
003acbc0: mov r2, r1
003acbc4: ldr r1, [r1, #4]
003acbc8: b #0x3acbb4
003acbcc: ldr ip, [r2, #0xc]
003acbd0: cmp r1, ip
003acbd4: moveq r1, r2
003acbd8: ldr r2, [r5, sb]
003acbdc: cmp r1, r2
003acbe0: beq #0x3accd8
003acbe4: ldr r2, [r1, #0x10]
003acbe8: cmp r0, r2
003acbec: bge #0x3accbc
003acbf0: cmp r3, #0
003acbf4: bne #0x3ac550
003acbf8: mov r1, lr
003acbfc: add r0, sp, #0x38
003acc00: add r2, sp, #0x30
003acc04: str lr, [sp]
003acc08: bl #0x3aa77c
003acc0c: ldr ip, [sp, #0x38]
003acc10: b #0x3ac56c
003acc14: mov r1, ip
003acc18: mov r3, ip
003acc1c: add r0, sp, #0x38
003acc20: add r2, sp, #0x30
003acc24: str fp, [sp]
003acc28: bl #0x3aa77c
003acc2c: ldr ip, [sp, #0x38]
003acc30: b #0x3ac56c
003acc34: add r0, sp, #0x28
003acc38: add r1, sp, #0x30
003acc3c: bl #0x3aa8b4
003acc40: ldr r3, [sp, #0x28]
003acc44: str r3, [sp, #0x38]
003acc48: ldr ip, [sp, #0x38]
003acc4c: b #0x3ac56c
003acc50: ldr r1, [ip, #0xc]
003acc54: ldr r3, [r1, #0x10]
003acc58: cmp r0, r3
003acc5c: ble #0x3acca0
003acc60: mov r3, fp
003acc64: add r0, sp, #0x38
003acc68: add r2, sp, #0x30
003acc6c: str ip, [sp]
003acc70: bl #0x3aa77c
003acc74: ldr ip, [sp, #0x38]
003acc78: b #0x3ac56c
003acc7c: ldr lr, [ip, #4]
003acc80: mov r3, ip
003acc84: b #0x3acc90
003acc88: mov r3, lr
003acc8c: ldr lr, [lr, #4]
003acc90: ldr r2, [lr, #8]
003acc94: cmp r2, r3
003acc98: beq #0x3acc88
003acc9c: b #0x3ac518
003acca0: add r0, sp, #0x18
003acca4: add r1, sp, #0x30
003acca8: bl #0x3aa8b4
003accac: ldr r3, [sp, #0x18]
003accb0: str r3, [sp, #0x38]
003accb4: ldr ip, [sp, #0x38]
003accb8: b #0x3ac56c
003accbc: add r0, sp, #0x20
003accc0: add r1, sp, #0x30
003accc4: bl #0x3aa8b4
003accc8: ldr r3, [sp, #0x20]
003acccc: str r3, [sp, #0x38]
003accd0: ldr ip, [sp, #0x38]
003accd4: b #0x3ac56c
003accd8: mov r1, lr
003accdc: add r0, sp, #0x38
003acce0: add r2, sp, #0x30
003acce4: mov r3, #0
003acce8: str lr, [sp]
003accec: bl #0x3aa77c
003accf0: ldr ip, [sp, #0x38]
003accf4: b #0x3ac56c
003accf8: subseq r8, lr, r8, ror #23
003accfc: andeq r4, r0, ip, lsr #1
003acd00: ldrsbeq r7, [r1], #-0x70
003acd04: andeq r0, r0, r4, lsl #17
003acd08: subseq r7, r1, r0, asr #15
003acd0c: subseq r4, r1, ip, lsl #9
003acd10: subseq r7, r1, r0, asr #14
003acd14: strdeq r3, r4, [r0], -r4
003acd18: subseq r7, r1, r4, lsl r7
003acd1c: subseq r7, r1, ip, ror #13
003acd20: andeq r4, r0, r8, lsr #9
003acd24: andeq r2, r0, r4, ror r4
003acd28: andeq r1, r0, r0, lsr #20
003acd2c: subseq r7, r1, r0, lsr r5
003acd30: subseq r7, r1, ip, lsr #10
003acd34: subseq r7, r1, r0, asr #10
_ZN9Character16UpdateSpotTargetEv 0x3a469c
003a469c: push {r4, r5, r6, lr}
003a46a0: movw r6, #0x14cc
003a46a4: ldr r2, [r0, r6]
003a46a8: ldr r5, [pc, #0x12c]
003a46ac: mov r4, r0
003a46b0: cmp r2, #0
003a46b4: add r5, pc, r5
003a46b8: beq #0x3a47b0
003a46bc: movw r3, #0x14c8
003a46c0: ldrb r3, [r0, r3]
003a46c4: cmp r3, #0
003a46c8: beq #0x3a4720
003a46cc: ldr r3, [pc, #0x10c]
003a46d0: add r1, r4, #0x1480
003a46d4: add r1, r1, #0x30
003a46d8: ldr r0, [r5, r3]
003a46dc: mov r2, #0
003a46e0: bl #0x525d84
003a46e4: cmp r0, #0
003a46e8: bne #0x3a4760
003a46ec: movw r3, #0x14bc
003a46f0: ldr r1, [r4, r3]
003a46f4: mov r3, #0x14c0
003a46f8: ldr r2, [r4, r3]
003a46fc: movw r3, #0x14c4
003a4700: ldr r0, [r4, r3]
003a4704: movw r3, #0x14b8
003a4708: str r0, [r4, r3]
003a470c: movw r3, #0x14b0
003a4710: str r1, [r4, r3]
003a4714: movw r3, #0x14b4
003a4718: str r2, [r4, r3]
003a471c: pop {r4, r5, r6, pc}
003a4720: ldr r3, [pc, #0xbc]
003a4724: add r1, r0, #0x14c0
003a4728: add r1, r1, #0xc
003a472c: ldr r0, [r5, r3]
003a4730: bl #0x494978
003a4734: ldr r0, [r4, r6]
003a4738: cmp r0, #0
003a473c: beq #0x3a471c
003a4740: ldr r3, [pc, #0x98]
003a4744: add r1, r4, #0x1480
003a4748: add r1, r1, #0x30
003a474c: ldr r0, [r5, r3]
003a4750: mov r2, #0
003a4754: bl #0x525d84
003a4758: cmp r0, #0
003a475c: beq #0x3a46ec
003a4760: movw r3, #0x14cc
003a4764: ldr r0, [r4, r3]
003a4768: movw r3, #0x14b0
003a476c: ldr r1, [r4, r3]
003a4770: movw r3, #0x14b4
003a4774: ldr r2, [r4, r3]
003a4778: movw r3, #0x14b8
003a477c: ldr r3, [r4, r3]
003a4780: movw ip, #0x14bc
003a4784: str r1, [r4, ip]
003a4788: mov ip, #0x14c0
003a478c: str r2, [r4, ip]
003a4790: movw ip, #0x14c4
003a4794: str r3, [r4, ip]
003a4798: str r1, [r0, #0x34]
003a479c: mov r1, #0
003a47a0: str r2, [r0, #0x38]
003a47a4: str r3, [r0, #0x3c]
003a47a8: pop {r4, r5, r6, lr}
003a47ac: b #0x492aa0
003a47b0: movw r3, #0x14c8
003a47b4: ldrb r3, [r0, r3]
003a47b8: cmp r3, #0
003a47bc: beq #0x3a471c
003a47c0: movw r3, #0x14ca
003a47c4: ldrsh r1, [r0, r3]
003a47c8: ldr r3, [pc, #0x14]
003a47cc: ldr r0, [r5, r3]
003a47d0: bl #0x495430
003a47d4: str r0, [r4, r6]
003a47d8: b #0x3a4738
003a47dc: ldrsbeq r0, [pc], #-0x3c
003a47e0: andeq r1, r0, r4, lsl #4
003a47e4: andeq r1, r0, r8, lsl #22
_ZN15VisualFXManager10_GetAnimFXEi 0x494ad4
00494ad4: push {r4, r5, r6, r7, r8, sl, lr}
00494ad8: ldr sl, [pc, #0x118]
00494adc: subs r7, r1, #0
00494ae0: sub sp, sp, #0xc
00494ae4: add sl, pc, sl
00494ae8: bge #0x494afc
00494aec: mov r5, #0
00494af0: mov r0, r5
00494af4: add sp, sp, #0xc
00494af8: pop {r4, r5, r6, r7, r8, sl, pc}
00494afc: ldr r3, [pc, #0xf8]
00494b00: ldr r3, [sl, r3]
00494b04: ldr r3, [r3]
00494b08: cmp r7, r3
00494b0c: bge #0x494aec
00494b10: ldr r3, [r0, #0x28]
00494b14: mov r8, #0x18
00494b18: mla r8, r8, r7, r3
00494b1c: ldr r2, [r8, #8]
00494b20: ldr r3, [r8, #4]
00494b24: rsb r3, r3, r2
00494b28: asrs r3, r3, #2
00494b2c: bne #0x494bd0
00494b30: mov r6, r8
00494b34: ldr r4, [r6, #0x10]!
00494b38: cmp r4, r6
00494b3c: beq #0x494b58
00494b40: ldr r4, [r4]
00494b44: add r3, r3, #1
00494b48: cmp r6, r4
00494b4c: bne #0x494b40
00494b50: cmp r3, #5
00494b54: bhi #0x494aec
00494b58: mov r1, #0
00494b5c: mov r0, #0x54
00494b60: bl #0x310570
00494b64: mov r1, r7
00494b68: mov r5, r0
00494b6c: bl #0x492374
00494b70: ldr r3, [pc, #0x88]
00494b74: mov r0, #0xc
00494b78: ldr r2, [pc, #0x84]
00494b7c: ldr r1, [sl, r3]
00494b80: mvn ip, #0
00494b84: mov r3, #0x3f800000
00494b88: ldr r1, [r1]
00494b8c: add r2, pc, r2
00494b90: mla r7, r0, r7, r1
00494b94: mov r0, r5
00494b98: ldr r1, [r7, #8]
00494b9c: str ip, [sp]
00494ba0: mov ip, #1
00494ba4: str ip, [sp, #4]
00494ba8: bl #0x49296c
00494bac: mov r0, r6
00494bb0: bl #0x494ab4
00494bb4: str r5, [r0, #8]
00494bb8: ldr r3, [r8, #0x14]
00494bbc: str r4, [r0]
00494bc0: str r3, [r0, #4]
00494bc4: str r0, [r3]
00494bc8: str r0, [r8, #0x14]
00494bcc: b #0x494af0
00494bd0: ldr r5, [r2, #-4]
00494bd4: mov r1, #0x3f800000
00494bd8: add r4, r8, #0x10
00494bdc: ldr r0, [r5, #0x2c]
00494be0: bl #0x472708
00494be4: ldr r3, [r8, #8]
00494be8: mov r0, r4
00494bec: sub r3, r3, #4
00494bf0: str r3, [r8, #8]
00494bf4: b #0x494bb0
00494bf8: subeq pc, pc, ip, lsr #31
00494bfc: andeq r0, r0, r8, lsl #23
00494c00: strheq r1, [r0], -r8
00494c04: subeq r6, r3, ip, ror ip
_ZN10AnimatedFXC2Ei 0x4922e0
004922e0: push {r4, r5}
004922e4: ldr r4, [pc, #0x80]
004922e8: ldr r2, [pc, #0x80]
004922ec: str r1, [r0, #8]
004922f0: add r4, pc, r4
004922f4: ldr r2, [r4, r2]
004922f8: mvn r1, #0
004922fc: str r1, [r0, #0x1c]
00492300: mov r1, #0x3f800000
00492304: mov ip, #0
00492308: add r5, r2, #8
0049230c: str r1, [r0, #0x20]
00492310: mov r2, #0
00492314: mov r1, #1
00492318: str r2, [r0, #0x50]
0049231c: str r5, [r0]
00492320: strb r1, [r0, #0x24]
00492324: str ip, [r0, #0x48]
00492328: str r2, [r0, #0xc]
0049232c: str r2, [r0, #0x10]
00492330: str r2, [r0, #0x14]
00492334: strb r2, [r0, #0x18]
00492338: str r2, [r0, #0x28]
0049233c: str r2, [r0, #0x2c]
00492340: strb r2, [r0, #0x30]
00492344: strb r2, [r0, #0x31]
00492348: strb r2, [r0, #0x32]
0049234c: str ip, [r0, #0x34]
00492350: str ip, [r0, #0x38]
00492354: str ip, [r0, #0x3c]
00492358: str ip, [r0, #0x40]
0049235c: str ip, [r0, #0x44]
00492360: strb r2, [r0, #0x4c]
00492364: pop {r4, r5}
00492368: bx lr
0049236c: subseq r2, r0, r0, lsr #15
00492370: andeq r3, r0, ip, lsl r1
_ZN9Character8InitPostEv 0x3b4d60
003b4d60: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4d64: ldr r5, [pc, #0x828]
003b4d68: ldr r6, [pc, #0x828]
003b4d6c: movw r3, #0x1394
003b4d70: add r5, pc, r5
003b4d74: ldr r2, [r5, r6]
003b4d78: ldrb r7, [r0, r3]
003b4d7c: sub sp, sp, #0xfc
003b4d80: ldr r2, [r2]
003b4d84: cmp r7, #0
003b4d88: mov r4, r0
003b4d8c: str r2, [sp, #0xf4]
003b4d90: beq #0x3b4db0
003b4d94: ldr r3, [r5, r6]
003b4d98: ldr r2, [sp, #0xf4]
003b4d9c: ldr r3, [r3]
003b4da0: cmp r2, r3
003b4da4: bne #0x3b5590
003b4da8: add sp, sp, #0xfc
003b4dac: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4db0: mov r2, #1
003b4db4: strb r2, [r0, r3]
003b4db8: bl #0x38bd64
003b4dbc: ldr r3, [r4, #0x274]
003b4dc0: cmp r0, r3
003b4dc4: bge #0x3b4d94
003b4dc8: ldr fp, [pc, #0x7cc]
003b4dcc: add r8, sp, #0xdc
003b4dd0: ldr sl, [r5, fp]
003b4dd4: mov r0, sl
003b4dd8: bl #0x337888
003b4ddc: ldr r1, [pc, #0x7bc]
003b4de0: add r2, sp, #0x48
003b4de4: mov r0, r8
003b4de8: add r1, pc, r1
003b4dec: bl #0x3140ec
003b4df0: mov r1, r8
003b4df4: mov r0, sl
003b4df8: bl #0x337a88
003b4dfc: mov r0, r8
003b4e00: bl #0x318254
003b4e04: movw r3, #0x13fc
003b4e08: ldr r2, [r4, r3]
003b4e0c: movw r3, #0x13f8
003b4e10: ldr r3, [r4, r3]
003b4e14: cmp r3, r2
003b4e18: beq #0x3b4e6c
003b4e1c: ldr r1, [pc, #0x780]
003b4e20: add r8, sp, #0x24
003b4e24: ldr r3, [r4, #0x64]
003b4e28: ldr r1, [r5, r1]
003b4e2c: mov r0, r8
003b4e30: ldr r1, [r1, #0x38]
003b4e34: str r7, [sp]
003b4e38: str r7, [sp, #4]
003b4e3c: bl #0x34aca0
003b4e40: mov r0, r8
003b4e44: mov r1, r7
003b4e48: bl #0x33fdc0
003b4e4c: cmp r0, #0
003b4e50: beq #0x3b4e6c
003b4e54: mov r0, r8
003b4e58: bl #0x33fee4
003b4e5c: subs r1, r0, #0
003b4e60: beq #0x3b4e6c
003b4e64: ldr r0, [r4, #0x378]
003b4e68: bl #0x405540
003b4e6c: mov r0, r4
003b4e70: movw r7, #0x13c8
003b4e74: bl #0x3b3d38
003b4e78: ldrsh r3, [r4, r7]
003b4e7c: cmn r3, #1
003b4e80: movweq r1, #0xffff
003b4e84: beq #0x3b4ec4
003b4e88: ldr sl, [r5, fp]
003b4e8c: add r8, sp, #0xc4
003b4e90: mov r0, sl
003b4e94: bl #0x337888
003b4e98: ldr r1, [pc, #0x708]
003b4e9c: add r2, sp, #0x44
003b4ea0: mov r0, r8
003b4ea4: add r1, pc, r1
003b4ea8: bl #0x3140ec
003b4eac: mov r1, r8
003b4eb0: mov r0, sl
003b4eb4: bl #0x337a88
003b4eb8: mov r0, r8
003b4ebc: bl #0x318254
003b4ec0: ldrh r1, [r4, r7]
003b4ec4: add r7, r4, #0x560
003b4ec8: sxth r1, r1
003b4ecc: mov r0, r7
003b4ed0: bl #0x3df2a4
003b4ed4: mov r0, r7
003b4ed8: mov r1, #1
003b4edc: bl #0x3e0810
003b4ee0: mov r0, r4
003b4ee4: bl #0x3a54d4
003b4ee8: subs r8, r0, #0
003b4eec: beq #0x3b4f04
003b4ef0: bl #0x30de54
003b4ef4: mov r1, r8
003b4ef8: add r2, r8, r0
003b4efc: add r0, r4, #0x290
003b4f00: bl #0x3109e0
003b4f04: ldr sl, [r5, fp]
003b4f08: add r8, sp, #0xac
003b4f0c: mov r0, sl
003b4f10: bl #0x337888
003b4f14: ldr r1, [pc, #0x690]
003b4f18: add r2, sp, #0x40
003b4f1c: mov r0, r8
003b4f20: add r1, pc, r1
003b4f24: bl #0x3140ec
003b4f28: mov r1, r8
003b4f2c: mov r0, sl
003b4f30: bl #0x337a88
003b4f34: mov r0, r8
003b4f38: bl #0x318254
003b4f3c: ldr r0, [r4, #0x59c]
003b4f40: bl #0x30e964
003b4f44: movw r1, #0x74bc
003b4f48: movt r1, #0x3c13
003b4f4c: bl #0x30ed6c
003b4f50: str r0, [r4, #0x120]
003b4f54: ldr r0, [r4, #0x5a0]
003b4f58: bl #0x30e964
003b4f5c: movw r1, #0x74bc
003b4f60: movt r1, #0x3c13
003b4f64: bl #0x30ed6c
003b4f68: str r0, [r4, #0x124]
003b4f6c: ldr r0, [r4, #0x5a4]
003b4f70: bl #0x30e964
003b4f74: movw r1, #0xd70a
003b4f78: movt r1, #0x3c23
003b4f7c: bl #0x30ed6c
003b4f80: str r0, [r4, #0x128]
003b4f84: mov r0, r4
003b4f88: bl #0x38be5c
003b4f8c: mov r0, r4
003b4f90: bl #0x38ab60
003b4f94: subs r1, r0, #0
003b4f98: beq #0x3b5488
003b4f9c: mov r1, #2
003b4fa0: mov r0, r4
003b4fa4: bl #0x3bc4d0
003b4fa8: ldr r3, [pc, #0x600]
003b4fac: mov r0, r4
003b4fb0: ldr r3, [r5, r3]
003b4fb4: ldr r8, [r3]
003b4fb8: bl #0x3a2fec
003b4fbc: mov r3, #0x44
003b4fc0: mla r8, r3, r0, r8
003b4fc4: ldr r3, [r4]
003b4fc8: mov r0, r4
003b4fcc: mov lr, pc
003b4fd0: ldr pc, [r3, #0x28]
003b4fd4: cmp r0, #0
003b4fd8: bne #0x3b4ff4
003b4fdc: ldrb r3, [r8, #0x10]
003b4fe0: cmp r3, #0
003b4fe4: beq #0x3b4ff4
003b4fe8: mov r3, #1
003b4fec: strb r3, [r4, #0x3ec]
003b4ff0: b #0x3b5014
003b4ff4: ldr r3, [r4]
003b4ff8: mov r0, r4
003b4ffc: mov lr, pc
003b5000: ldr pc, [r3, #0x28]
003b5004: cmp r0, #0
003b5008: bne #0x3b54a4
003b500c: add r0, r4, #0x3c8
003b5010: bl #0x3cf1f0
003b5014: movw r3, #0x1488
003b5018: ldr r2, [pc, #0x594]
003b501c: ldr r1, [r4, r3]
003b5020: ldr r3, [r8, #0x30]
003b5024: str r2, [sp, #0xc]
003b5028: mov r2, r4
003b502c: add r1, r1, r3
003b5030: ldr r3, [sp, #0xc]
003b5034: ldr sb, [pc, #0x57c]
003b5038: add sl, sp, #0x94
003b503c: ldr r0, [r5, r3]
003b5040: bl #0x495430
003b5044: movw r3, #0x1484
003b5048: str r0, [r4, r3]
003b504c: mov r0, r4
003b5050: bl #0x3b4738
003b5054: ldr r8, [r5, fp]
003b5058: add sb, pc, sb
003b505c: mov r0, r8
003b5060: bl #0x337888
003b5064: add r2, sp, #0x3c
003b5068: mov r0, sl
003b506c: mov r1, sb
003b5070: bl #0x3140ec
003b5074: mov r1, sl
003b5078: mov r0, r8
003b507c: bl #0x337a88
003b5080: mov r0, sl
003b5084: bl #0x318254
003b5088: add r0, r4, #0x490
003b508c: add r0, r0, #0xc
003b5090: bl #0x3c9f4c
003b5094: add sl, sp, #0x7c
003b5098: mov r0, r8
003b509c: bl #0x337888
003b50a0: add r2, sp, #0x38
003b50a4: mov r0, sl
003b50a8: mov r1, sb
003b50ac: bl #0x3140ec
003b50b0: mov r1, sl
003b50b4: mov r0, r8
003b50b8: bl #0x337a88
003b50bc: mov r0, sl
003b50c0: bl #0x318254
003b50c4: mov r0, r4
003b50c8: bl #0x3b3b00
003b50cc: add sl, sp, #0x64
003b50d0: mov r0, r8
003b50d4: bl #0x337888
003b50d8: add r2, sp, #0x34
003b50dc: mov r1, sb
003b50e0: mov r0, sl
003b50e4: bl #0x3140ec
003b50e8: mov r1, sl
003b50ec: mov r0, r8
003b50f0: bl #0x337a88
003b50f4: mov r0, sl
003b50f8: bl #0x318254
003b50fc: ldr r3, [r4]
003b5100: mov r0, r4
003b5104: mov lr, pc
003b5108: ldr pc, [r3, #0x28]
003b510c: cmp r0, #0
003b5110: beq #0x3b538c
003b5114: ldr r3, [r4, #0x2d8]
003b5118: cmp r3, #0
003b511c: beq #0x3b512c
003b5120: ldr r0, [r3, #8]
003b5124: mov r1, #0
003b5128: bl #0x59719c
003b512c: ldr r2, [pc, #0x470]
003b5130: mov r0, r4
003b5134: mov r1, #4
003b5138: str r2, [sp, #0x10]
003b513c: bl #0x3bc4d0
003b5140: ldr r3, [sp, #0x10]
003b5144: ldr r0, [r5, r3]
003b5148: bl #0x31f594
003b514c: cmp r0, #0
003b5150: beq #0x3b5160
003b5154: ldr r1, [r0, #0x118]
003b5158: mov r0, r4
003b515c: bl #0x3bb950
003b5160: ldr r2, [sp, #0x10]
003b5164: mov r1, r4
003b5168: ldr r3, [r5, r2]
003b516c: ldr r0, [r3, #0x40]
003b5170: bl #0x36effc
003b5174: cmp r0, #0
003b5178: bne #0x3b54dc
003b517c: movw r3, #0x13c8
003b5180: ldrsh r1, [r4, r3]
003b5184: mov r0, r7
003b5188: bl #0x3df2a4
003b518c: mov r0, r7
003b5190: bl #0x3df480
003b5194: mov r0, r7
003b5198: mov r1, #1
003b519c: bl #0x3e0810
003b51a0: ldr r2, [sp, #0x10]
003b51a4: mov r1, r4
003b51a8: ldr r3, [r5, r2]
003b51ac: ldr r0, [r3, #0x40]
003b51b0: bl #0x36effc
003b51b4: cmp r0, #0
003b51b8: bne #0x3b54d0
003b51bc: movw r3, #0xc9ff
003b51c0: movt r3, #0x3b9a
003b51c4: str r3, [r4, #0x3a4]
003b51c8: mov r2, #0
003b51cc: mov r0, r7
003b51d0: mov r1, #0xc2
003b51d4: bl #0x3df6e0
003b51d8: ldr r2, [sp, #0xc]
003b51dc: bic r0, r0, r0, asr #31
003b51e0: strb r0, [r4, #0x3a8]
003b51e4: ldr r3, [r5, r2]
003b51e8: ldr r2, [r3, #0x1c]
003b51ec: ldr r3, [r3, #0x20]
003b51f0: rsb r3, r2, r3
003b51f4: asr r3, r3, #3
003b51f8: add r2, r3, r3, lsl #2
003b51fc: add r2, r2, r2, lsl #4
003b5200: add r2, r2, r2, lsl #8
003b5204: add r2, r2, r2, lsl #16
003b5208: add r3, r3, r2, lsl #1
003b520c: cmp r3, #0
003b5210: beq #0x3b538c
003b5214: mov r0, #0x24
003b5218: mov r1, #0
003b521c: bl #0x31056c
003b5220: movw r3, #0x1494
003b5224: str r0, [r4, r3]
003b5228: ldr r3, [pc, #0x38c]
003b522c: mov sb, r0
003b5230: ldr r3, [r5, r3]
003b5234: ldr sl, [r3]
003b5238: cmp sl, #0
003b523c: beq #0x3b550c
003b5240: ldr r3, [pc, #0x378]
003b5244: ldr r2, [pc, #0x378]
003b5248: str r7, [sp, #0x18]
003b524c: ldr r3, [r5, r3]
003b5250: add r2, pc, r2
003b5254: mov r8, #0
003b5258: ldr r3, [r3]
003b525c: str r0, [sp, #0x14]
003b5260: mov sb, r2
003b5264: mov r7, r3
003b5268: b #0x3b5278
003b526c: add r8, r8, #1
003b5270: cmp r8, sl
003b5274: beq #0x3b5504
003b5278: mov r0, sb
003b527c: ldr r1, [r7, r8, lsl #2]
003b5280: bl #0x30e31c
003b5284: cmp r0, #0
003b5288: bne #0x3b526c
003b528c: ldr sb, [sp, #0x14]
003b5290: ldr r7, [sp, #0x18]
003b5294: mov r1, r8
003b5298: ldr r3, [sp, #0xc]
003b529c: str r7, [sp, #0x14]
003b52a0: str fp, [sp, #0x18]
003b52a4: ldr r2, [r5, r3]
003b52a8: ldr r3, [pc, #0x318]
003b52ac: str r6, [sp, #0x1c]
003b52b0: mov r8, #0
003b52b4: movw sl, #0x1494
003b52b8: mov r7, r1
003b52bc: mov r6, r2
003b52c0: mov fp, r3
003b52c4: b #0x3b52cc
003b52c8: ldr sb, [r4, sl]
003b52cc: mov r0, r6
003b52d0: add r1, r7, r8
003b52d4: mov r2, #0
003b52d8: bl #0x495430
003b52dc: str r0, [sb, r8, lsl #2]
003b52e0: ldr r3, [r4, sl]
003b52e4: ldr r3, [r3, r8, lsl #2]
003b52e8: cmp r3, #0
003b52ec: beq #0x3b5350
003b52f0: ldr r2, [r5, fp]
003b52f4: mov r0, r3
003b52f8: mov r1, #0
003b52fc: ldr lr, [r2]
003b5300: ldr ip, [r2, #4]
003b5304: ldr r2, [r2, #8]
003b5308: str lr, [r3, #0x34]
003b530c: str ip, [r3, #0x38]
003b5310: str r2, [r3, #0x3c]
003b5314: bl #0x492aa0
003b5318: ldr r3, [r4, sl]
003b531c: mov r1, #0
003b5320: ldr r0, [r3, r8, lsl #2]
003b5324: bl #0x492ef0
003b5328: ldr r3, [r4, sl]
003b532c: ldr r0, [r3, r8, lsl #2]
003b5330: bl #0x49267c
003b5334: ldr r3, [r0]
003b5338: mov lr, pc
003b533c: ldr pc, [r3, #0x44]
003b5340: mov r1, #1
003b5344: ldr r3, [r0]
003b5348: mov lr, pc
003b534c: ldr pc, [r3, #0x40]
003b5350: add r8, r8, #1
003b5354: cmp r8, #9
003b5358: bne #0x3b52c8
003b535c: ldr r2, [sp, #0x10]
003b5360: mov r1, r4
003b5364: ldr r7, [sp, #0x14]
003b5368: ldr r3, [r5, r2]
003b536c: ldr fp, [sp, #0x18]
003b5370: ldr r6, [sp, #0x1c]
003b5374: ldr r0, [r3, #0x40]
003b5378: bl #0x36effc
003b537c: cmp r0, #0
003b5380: bne #0x3b5514
003b5384: mov r0, r4
003b5388: bl #0x3a41a0
003b538c: add r8, r4, #0xff0
003b5390: add r8, r8, #4
003b5394: mov r1, r8
003b5398: mov r2, #0xd2
003b539c: mov r0, r7
003b53a0: bl #0x3dedb4
003b53a4: bl #0x30e964
003b53a8: mov r3, #0x1440
003b53ac: str r0, [r4, r3]
003b53b0: mov r2, #0xd3
003b53b4: mov r1, r8
003b53b8: mov r0, r7
003b53bc: bl #0x3dedb4
003b53c0: bl #0x30e964
003b53c4: movw r3, #0x1444
003b53c8: str r0, [r4, r3]
003b53cc: add r1, r4, #0x160
003b53d0: mov r0, r4
003b53d4: bl #0x3a58f4
003b53d8: add r1, r4, #0x1440
003b53dc: mov r0, r4
003b53e0: add r1, r1, #0x10
003b53e4: mov r2, #1
003b53e8: bl #0x393db4
003b53ec: ldr ip, [r4, #0x16c]
003b53f0: ldr r0, [r4, #0x2d8]
003b53f4: ldr r1, [r4, #0x170]
003b53f8: ldr r2, [r4, #0x174]
003b53fc: movw r3, #0x145c
003b5400: str ip, [r4, r3]
003b5404: movw r3, #0x1460
003b5408: str r1, [r4, r3]
003b540c: cmp r0, #0
003b5410: movw r3, #0x1464
003b5414: str r2, [r4, r3]
003b5418: beq #0x3b5420
003b541c: bl #0x470a54
003b5420: mov r1, #0
003b5424: mov r2, #1
003b5428: mov r0, r4
003b542c: bl #0x3a59ac
003b5430: mov r0, r4
003b5434: bl #0x3b3a70
003b5438: ldrb r1, [r4, #0x3ec]
003b543c: cmp r1, #0
003b5440: beq #0x3b54c4
003b5444: mov r0, r4
003b5448: bl #0x3d37d0
003b544c: ldr r7, [r5, fp]
003b5450: add r4, sp, #0x4c
003b5454: mov r0, r7
003b5458: bl #0x337888
003b545c: ldr r1, [pc, #0x168]
003b5460: add r2, sp, #0x30
003b5464: mov r0, r4
003b5468: add r1, pc, r1
003b546c: bl #0x3140ec
003b5470: mov r0, r7
003b5474: mov r1, r4
003b5478: bl #0x337a88
003b547c: mov r0, r4
003b5480: bl #0x318254
003b5484: b #0x3b4d94
003b5488: mov r0, r4
003b548c: ldr r3, [r4]
003b5490: mov lr, pc
003b5494: ldr pc, [r3, #0x40]
003b5498: mov r0, r4
003b549c: bl #0x33ddb4
003b54a0: b #0x3b4d94
003b54a4: ldr r3, [pc, #0xf8]
003b54a8: mov r1, r4
003b54ac: ldr r3, [r5, r3]
003b54b0: ldr r0, [r3, #0x40]
003b54b4: bl #0x36effc
003b54b8: cmp r0, #0
003b54bc: bne #0x3b500c
003b54c0: b #0x3b4fe8
003b54c4: add r0, r4, #0x3c8
003b54c8: bl #0x3ce7c0
003b54cc: b #0x3b5444
003b54d0: mov r0, r4
003b54d4: bl #0x3b3a90
003b54d8: b #0x3b51bc
003b54dc: mov r0, r4
003b54e0: bl #0x3b395c
003b54e4: mov r0, r7
003b54e8: bl #0x3defac
003b54ec: movw r3, #0x14e8
003b54f0: ldr r0, [r4, r3]
003b54f4: cmp r0, #0
003b54f8: beq #0x3b517c
003b54fc: bl #0x4679e8
003b5500: b #0x3b517c
003b5504: ldr sb, [sp, #0x14]
003b5508: ldr r7, [sp, #0x18]
003b550c: mvn r1, #0
003b5510: b #0x3b5298
003b5514: ldr r3, [sp, #0xc]
003b5518: mov r2, #0
003b551c: movw r8, #0x149c
003b5520: ldr r0, [r5, r3]
003b5524: ldr r3, [pc, #0xa4]
003b5528: ldr r3, [r5, r3]
003b552c: ldr r3, [r3]
003b5530: ldr r1, [r3, #0x88]
003b5534: bl #0x495430
003b5538: cmp r0, #0
003b553c: str r0, [r4, r8]
003b5540: beq #0x3b5384
003b5544: mov r2, #0
003b5548: str r2, [r0, #0x3c]
003b554c: str r2, [r0, #0x34]
003b5550: str r2, [r0, #0x38]
003b5554: mov r1, #0
003b5558: bl #0x492aa0
003b555c: mov r1, #0
003b5560: ldr r0, [r4, r8]
003b5564: bl #0x492ef0
003b5568: ldr r0, [r4, r8]
003b556c: bl #0x49267c
003b5570: ldr r3, [r0]
003b5574: mov lr, pc
003b5578: ldr pc, [r3, #0x44]
003b557c: mov r1, #1
003b5580: ldr r3, [r0]
003b5584: mov lr, pc
003b5588: ldr pc, [r3, #0x40]
003b558c: b #0x3b5384
003b5590: bl #0x30e310
003b5594: subseq pc, sp, r0, lsr #26
003b5598: andeq r4, r0, ip, lsr #1
003b559c: andeq r0, r0, r4, lsl #17
003b55a0: subseq pc, r0, r8, lsr #32
003b55a4: strdeq r3, r4, [r0], -r4
003b55a8: subseq lr, r0, ip, ror #30
003b55ac: ldrsheq lr, [r0], #-0xe0
003b55b0: andeq r0, r0, r8, asr r7
003b55b4: andeq r1, r0, r8, lsl #22
003b55b8: ldrheq lr, [r0], #-0xd8
003b55bc: andeq r0, r0, r4, asr #13
003b55c0: muleq r0, r4, r2
003b55c4: subseq lr, r0, r0, ror #24
003b55c8: andeq r3, r0, ip, lsr #30
003b55cc: subseq lr, r0, r8, lsr #19
003b55d0: andeq r3, r0, r8, asr #5
_ZN15VisualFXManager10GrabAnimFXEiP10GameObject 0x495430
00495430: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495434: ldr r4, [pc, #0x260]
00495438: ldr r5, [pc, #0x260]
0049543c: ldr ip, [pc, #0x260]
00495440: add r4, pc, r4
00495444: ldr r3, [r4, r5]
00495448: ldr sl, [r4, ip]
0049544c: sub sp, sp, #0x84
00495450: ldr r3, [r3]
00495454: mov r8, r0
00495458: mov r0, sl
0049545c: str r3, [sp, #0x7c]
00495460: mov r7, r1
00495464: mov sb, r2
00495468: bl #0x337888
0049546c: ldr r1, [pc, #0x234]
00495470: add r6, sp, #0x64
00495474: add r2, sp, #0x60
00495478: add r1, pc, r1
0049547c: mov r0, r6
00495480: bl #0x3140ec
00495484: mov r0, sl
00495488: mov r1, r6
0049548c: bl #0x337ec8
00495490: mov sl, r0
00495494: ldr r0, [sp, #0x78]
00495498: cmp r0, r6
0049549c: beq #0x4954bc
004954a0: cmp r0, #0
004954a4: beq #0x4954bc
004954a8: ldr r1, [sp, #0x64]
004954ac: rsb r1, r0, r1
004954b0: cmp r1, #0x80
004954b4: bhi #0x4954e8
004954b8: bl #0x708f00
004954bc: cmp sl, #0
004954c0: bne #0x4954f4
004954c4: mov r6, #0
004954c8: ldr r3, [r4, r5]
004954cc: ldr r2, [sp, #0x7c]
004954d0: mov r0, r6
004954d4: ldr r3, [r3]
004954d8: cmp r2, r3
004954dc: bne #0x495698
004954e0: add sp, sp, #0x84
004954e4: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004954e8: bl #0x310440
004954ec: cmp sl, #0
004954f0: beq #0x4954c4
004954f4: cmp r7, #0
004954f8: blt #0x4954c4
004954fc: ldr r3, [pc, #0x1a8]
00495500: ldr r3, [r4, r3]
00495504: ldr r3, [r3]
00495508: cmp r7, r3
0049550c: bge #0x4954c4
00495510: mov sl, #0x18
00495514: ldr fp, [r8, #0x1c]
00495518: mul sl, sl, r7
0049551c: mov r0, r8
00495520: ldr r3, [fp, sl]
00495524: add r1, fp, sl
00495528: str r1, [sp, #0x14]
0049552c: ldr r3, [r3, #0x10]
00495530: ldr r1, [r3, #4]
00495534: bl #0x494ad4
00495538: subs r6, r0, #0
0049553c: beq #0x4954c8
00495540: ldr r2, [fp, sl]
00495544: ldr r3, [r2, #0x14]
00495548: cmp r3, #2
0049554c: movne r3, #0
00495550: beq #0x495684
00495554: ldr r1, [pc, #0x154]
00495558: mov ip, #0
0049555c: ldr r2, [r2, #8]
00495560: ldr r0, [r4, r1]
00495564: mov r1, r7
00495568: add r7, sp, #0x1c
0049556c: ldr lr, [r0, #4]
00495570: ldr fp, [r0, #8]
00495574: ldr sl, [r0]
00495578: str ip, [sp, #4]
0049557c: add ip, sp, #0x54
00495580: str ip, [sp, #8]
00495584: mov r0, r8
00495588: add ip, sp, #0x48
0049558c: str lr, [sp, #0x4c]
00495590: str ip, [sp, #0xc]
00495594: str lr, [sp, #0x58]
00495598: str sl, [sp, #0x48]
0049559c: str sl, [sp, #0x54]
004955a0: str fp, [sp, #0x50]
004955a4: str fp, [sp, #0x5c]
004955a8: str sb, [sp]
004955ac: bl #0x4935b8
004955b0: ldr r1, [sp, #0x14]
004955b4: mov sl, r0
004955b8: mov r0, r7
004955bc: bl #0x493924
004955c0: mov r1, r8
004955c4: mov r3, sl
004955c8: mov r2, r7
004955cc: add r0, sp, #0x34
004955d0: bl #0x4933e4
004955d4: ldr r2, [sp, #0x14]
004955d8: mov r0, r7
004955dc: add r7, r2, #0x10
004955e0: bl #0x4940d8
004955e4: mov r0, r7
004955e8: bl #0x493814
004955ec: str sl, [r0, #8]
004955f0: ldr r1, [sp, #0x14]
004955f4: mov r3, r0
004955f8: ldr r2, [r1, #0x14]
004955fc: str r7, [r0]
00495600: mov r0, r6
00495604: str r2, [r3, #4]
00495608: str r3, [r2]
0049560c: str r3, [r1, #0x14]
00495610: str sb, [r6, #0x28]
00495614: mov r1, #1
00495618: bl #0x492aa0
0049561c: mov r0, r6
00495620: mov r1, #1
00495624: bl #0x492aa0
00495628: mov r0, r6
0049562c: mov r1, #1
00495630: bl #0x492694
00495634: mov r0, r6
00495638: bl #0x492744
0049563c: ldr lr, [sp, #0x38]
00495640: ldr r0, [pc, #0x6c]
00495644: ldrb r1, [sp, #0x34]
00495648: str lr, [sp]
0049564c: mvn lr, #0
00495650: ldr ip, [r4, r0]
00495654: str lr, [sp, #4]
00495658: ldr lr, [sp, #0x44]
0049565c: mov r0, r6
00495660: ldrb r2, [sp, #0x35]
00495664: ldrb r3, [sp, #0x36]
00495668: str lr, [sp, #8]
0049566c: str ip, [sp, #0xc]
00495670: bl #0x492e8c
00495674: mov r0, r6
00495678: mov r1, #1
0049567c: bl #0x492ef0
00495680: b #0x4954c8
00495684: ldr r0, [r2, #0xc]
00495688: bl #0x493780
0049568c: ldr r2, [fp, sl]
00495690: mov r3, r0
00495694: b #0x495554
00495698: bl #0x30e310
0049569c: subeq pc, pc, r0, asr r6
004956a0: andeq r4, r0, ip, lsr #1
004956a4: andeq r0, r0, r4, lsl #17
_ZN9Character22UpdateObjectOfInterestEv 0x3abb9c
003abb9c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003abba0: movw r5, #0x14a4
003abba4: ldr r3, [r0, r5]
003abba8: ldr r6, [pc, #0x2cc]
003abbac: mov r1, #0
003abbb0: cmp r3, #0
003abbb4: movw r2, #0x14ad
003abbb8: strb r1, [r0, r2]
003abbbc: sub sp, sp, #0x74
003abbc0: mov r4, r0
003abbc4: add r6, pc, r6
003abbc8: beq #0x3abbf4
003abbcc: mov r0, r3
003abbd0: mov r1, r4
003abbd4: ldr r3, [r3]
003abbd8: mov lr, pc
003abbdc: ldr pc, [r3, #0x8c]
003abbe0: cmp r0, #0
003abbe4: bne #0x3abc28
003abbe8: mov r2, #0
003abbec: movw r3, #0x14a4
003abbf0: str r2, [r4, r3]
003abbf4: ldr r7, [pc, #0x284]
003abbf8: movw sl, #0x14aa
003abbfc: ldrh r5, [r4, sl]
003abc00: ldr r0, [r6, r7]
003abc04: bl #0x31f66c
003abc08: rsb r5, r0, r5
003abc0c: uxth r5, r5
003abc10: sxth r3, r5
003abc14: cmp r3, #0
003abc18: strh r5, [r4, sl]
003abc1c: ble #0x3abc48
003abc20: add sp, sp, #0x74
003abc24: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003abc28: ldr r3, [r4, r5]
003abc2c: ldrb r3, [r3, #0x81]
003abc30: cmp r3, #0
003abc34: beq #0x3abbf4
003abc38: mov r2, #0
003abc3c: movw r3, #0x14a4
003abc40: str r2, [r4, r3]
003abc44: b #0x3abbf4
003abc48: mvn r0, #0
003abc4c: movw r3, #0x14a8
003abc50: strb r0, [r4, r3]
003abc54: mov r3, #0x1f4
003abc58: strh r3, [r4, sl]
003abc5c: mov r2, #0
003abc60: movw r3, #0x14ac
003abc64: strb r2, [r4, r3]
003abc68: mov r8, #1
003abc6c: movw r1, #0x14a4
003abc70: add r5, sp, #8
003abc74: ldr fp, [r4, r1]
003abc78: mov r3, r2
003abc7c: str r2, [r4, r1]
003abc80: mov r0, r5
003abc84: mov r2, r8
003abc88: mov r1, r4
003abc8c: str r8, [sp]
003abc90: bl #0x4a2730
003abc94: mov r3, #0x59
003abc98: str r3, [sp, #0x3c]
003abc9c: ldr r2, [sp, #0x18]
003abca0: ldr r3, [sp, #8]
003abca4: str r8, [sp, #0x40]
003abca8: cmp r2, r3
003abcac: beq #0x3abcc8
003abcb0: mov r0, r5
003abcb4: bl #0x38fb18
003abcb8: ldr r3, [sp, #8]
003abcbc: ldr r2, [sp, #0x18]
003abcc0: cmp r2, r3
003abcc4: bne #0x3abcb0
003abcc8: ldr r3, [pc, #0x1b4]
003abccc: mov r0, r4
003abcd0: ldr r3, [r6, r3]
003abcd4: str r3, [sp, #0x30]
003abcd8: bl #0x3935dc
003abcdc: ldr r7, [r6, r7]
003abce0: ldr r1, [pc, #0x1a0]
003abce4: ldr r2, [pc, #0x1a0]
003abce8: mov r8, r0
003abcec: add r1, pc, r1
003abcf0: add r2, pc, r2
003abcf4: ldr r0, [r7, #0x2c]
003abcf8: bl #0x4c4bdc
003abcfc: bl #0x30e964
003abd00: ldr r3, [pc, #0x188]
003abd04: ldr lr, [r7, #0x38]
003abd08: mov r2, r0
003abd0c: ldr r3, [r6, r3]
003abd10: add ip, lr, #0x80
003abd14: str ip, [sp, #0x60]
003abd18: add r3, r3, #8
003abd1c: str r3, [sp, #0x5c]
003abd20: ldr lr, [lr, #0x80]
003abd24: movw r3, #0xfdb
003abd28: str ip, [sp, #0x68]
003abd2c: mov ip, #0
003abd30: movt r3, #0x40c9
003abd34: str ip, [sp, #0x6c]
003abd38: mov r1, r8
003abd3c: add ip, sp, #0x5c
003abd40: mov r0, r5
003abd44: str lr, [sp, #0x64]
003abd48: str ip, [sp]
003abd4c: bl #0x4a33c8
003abd50: ldr r3, [pc, #0x13c]
003abd54: ldr r7, [sp, #8]
003abd58: ldr r2, [sp, #0x18]
003abd5c: ldr r3, [r6, r3]
003abd60: cmp r2, r7
003abd64: add r3, r3, #8
003abd68: str r3, [sp, #0x5c]
003abd6c: movwne r6, #0x14a4
003abd70: movwne sb, #0x14a8
003abd74: movwne sl, #0x14ac
003abd78: bne #0x3abde8
003abd7c: b #0x3abe68
003abd80: str r3, [r4, r6]
003abd84: ldr r3, [r7]
003abd88: mov r1, r4
003abd8c: mov r0, r3
003abd90: ldr r3, [r3]
003abd94: mov lr, pc
003abd98: ldr pc, [r3, #0x90]
003abd9c: uxtb r0, r0
003abda0: strb r0, [r4, sb]
003abda4: ldr r3, [r7, #0xc]
003abda8: sxtb r0, r0
003abdac: and r3, r3, #1
003abdb0: cmp r3, #0
003abdb4: strb r3, [r4, sl]
003abdb8: bne #0x3abe74
003abdbc: cmp r0, #0
003abdc0: cmpne r0, #2
003abdc4: beq #0x3abe74
003abdc8: cmp r0, #1
003abdcc: beq #0x3abe34
003abdd0: mov r0, r5
003abdd4: bl #0x38fb18
003abdd8: ldr r7, [sp, #8]
003abddc: ldr r3, [sp, #0x18]
003abde0: cmp r3, r7
003abde4: beq #0x3abe68
003abde8: ldr r3, [r7]
003abdec: movw r8, #0x14a4
003abdf0: cmp r4, r3
003abdf4: beq #0x3abdd0
003abdf8: ldr r2, [r4, r6]
003abdfc: cmp r2, #0
003abe00: beq #0x3abd80
003abe04: ldr r2, [r7, #0xc]
003abe08: tst r2, #1
003abe0c: bne #0x3abd80
003abe10: mov r0, r3
003abe14: mov r1, r4
003abe18: ldr r3, [r3]
003abe1c: mov lr, pc
003abe20: ldr pc, [r3, #0x90]
003abe24: cmp r0, #1
003abe28: bne #0x3abdd0
003abe2c: ldr r3, [r7]
003abe30: b #0x3abd80
003abe34: ldr r3, [r4, r6]
003abe38: ldr r2, [r3, #0x3bc]
003abe3c: cmp r4, r2
003abe40: bne #0x3abdd0
003abe44: cmp r3, #0
003abe48: beq #0x3abe5c
003abe4c: cmp fp, r3
003abe50: movne r2, #1
003abe54: movwne r3, #0x14ad
003abe58: strbne r2, [r4, r3]
003abe5c: mov r0, r5
003abe60: bl #0x38d18c
003abe64: b #0x3abc20
003abe68: movw r3, #0x14a4
003abe6c: ldr r3, [r4, r3]
003abe70: b #0x3abe44
003abe74: ldr r3, [r4, r8]
003abe78: b #0x3abe44
003abe7c: subseq r8, lr, ip, asr #29
003abe80: strdeq r3, r4, [r0], -r4
003abe84: ldrdeq r1, r2, [r0], -r4
003abe88: subseq r5, r1, r4, ror #20
_ZN9Character9InitFinalEv 0x3b4978
003b4978: push {r4, r5, r6, r7, r8, lr}
003b497c: ldr r4, [pc, #0x22c]
003b4980: ldr r6, [pc, #0x22c]
003b4984: movw r3, #0x1395
003b4988: add r4, pc, r4
003b498c: ldr r2, [r4, r6]
003b4990: ldrb r1, [r0, r3]
003b4994: sub sp, sp, #0x48
003b4998: ldr r2, [r2]
003b499c: cmp r1, #0
003b49a0: mov r5, r0
003b49a4: str r2, [sp, #0x44]
003b49a8: beq #0x3b49c8
003b49ac: ldr r3, [r4, r6]
003b49b0: ldr r2, [sp, #0x44]
003b49b4: ldr r3, [r3]
003b49b8: cmp r2, r3
003b49bc: bne #0x3b4bac
003b49c0: add sp, sp, #0x48
003b49c4: pop {r4, r5, r6, r7, r8, pc}
003b49c8: mov r2, #1
003b49cc: strb r2, [r0, r3]
003b49d0: bl #0x38bd64
003b49d4: ldr r3, [r5, #0x274]
003b49d8: cmp r0, r3
003b49dc: bge #0x3b49ac
003b49e0: mov r0, r5
003b49e4: bl #0x38cd48
003b49e8: mov r0, r5
003b49ec: bl #0x3a3094
003b49f0: cmp r0, #0
003b49f4: beq #0x3b4b98
003b49f8: ldr r3, [pc, #0x1b8]
003b49fc: mov r1, #0
003b4a00: mov r2, #1
003b4a04: ldr r3, [r4, r3]
003b4a08: ldr r0, [r3, #0x40]
003b4a0c: bl #0x36e478
003b4a10: ldr r7, [r0, #0x660]
003b4a14: mov r3, #0
003b4a18: str r3, [sp, #8]
003b4a1c: cmp r7, #0
003b4a20: str r3, [sp]
003b4a24: str r3, [sp, #4]
003b4a28: beq #0x3b4a88
003b4a2c: mov r1, sp
003b4a30: mov r0, r7
003b4a34: bl #0x393ae4
003b4a38: mov r0, r7
003b4a3c: bl #0x3935dc
003b4a40: ldr r1, [r0]
003b4a44: mov r7, r0
003b4a48: ldr r0, [sp]
003b4a4c: bl #0x30eba4
003b4a50: str r0, [sp]
003b4a54: ldr r1, [r7, #4]
003b4a58: ldr r0, [sp, #4]
003b4a5c: bl #0x30eba4
003b4a60: str r0, [sp, #4]
003b4a64: ldr r1, [r7, #8]
003b4a68: ldr r0, [sp, #8]
003b4a6c: bl #0x30eba4
003b4a70: mov r1, sp
003b4a74: str r0, [sp, #8]
003b4a78: mov r2, #1
003b4a7c: mov r0, r5
003b4a80: mov r8, sp
003b4a84: bl #0x393db4
003b4a88: ldr r3, [r5, #0x2d8]
003b4a8c: cmp r3, #0
003b4a90: beq #0x3b4af8
003b4a94: ldr r3, [r5]
003b4a98: mov r0, r5
003b4a9c: mov lr, pc
003b4aa0: ldr pc, [r3, #0x28]
003b4aa4: cmp r0, #0
003b4aa8: beq #0x3b4b7c
003b4aac: ldr r3, [pc, #0x104]
003b4ab0: ldr r1, [pc, #0x104]
003b4ab4: add r7, sp, #0x2c
003b4ab8: ldr r3, [r4, r3]
003b4abc: add r1, pc, r1
003b4ac0: add r2, sp, #0x10
003b4ac4: ldr r3, [r3, #0x10]
003b4ac8: mov r0, r7
003b4acc: ldr r8, [r3, #0x1c]
003b4ad0: bl #0x3140ec
003b4ad4: add r8, r8, #0x294
003b4ad8: mov r0, r8
003b4adc: mov r1, r7
003b4ae0: bl #0x40c3cc
003b4ae4: mov r8, r0
003b4ae8: mov r0, r7
003b4aec: bl #0x318254
003b4af0: ldr r3, [r5, #0x2d8]
003b4af4: str r8, [r3, #0x40]
003b4af8: add r7, r5, #0x3c8
003b4afc: mov r0, r7
003b4b00: ldr r3, [r5, #0x3c8]
003b4b04: mov lr, pc
003b4b08: ldr pc, [r3, #0x10]
003b4b0c: ldr r3, [r5]
003b4b10: mov r0, r5
003b4b14: mov lr, pc
003b4b18: ldr pc, [r3, #0x28]
003b4b1c: cmp r0, #0
003b4b20: beq #0x3b49ac
003b4b24: ldr r3, [pc, #0x8c]
003b4b28: mov r1, r5
003b4b2c: ldr r3, [r4, r3]
003b4b30: ldr r0, [r3, #0x40]
003b4b34: bl #0x36effc
003b4b38: cmp r0, #0
003b4b3c: beq #0x3b49ac
003b4b40: mov r0, r7
003b4b44: add r7, r5, #0x560
003b4b48: bl #0x3d8894
003b4b4c: mov r0, r7
003b4b50: mov r1, #1
003b4b54: bl #0x3e0810
003b4b58: mov r0, r7
003b4b5c: mov r1, #0xc2
003b4b60: mov r2, #0
003b4b64: bl #0x3df6e0
003b4b68: bic r0, r0, r0, asr #31
003b4b6c: strb r0, [r5, #0x3a8]
003b4b70: mov r0, r5
003b4b74: bl #0x3bc4a8
003b4b78: b #0x3b49ac
003b4b7c: ldr r3, [pc, #0x34]
003b4b80: ldr r1, [pc, #0x38]
003b4b84: add r7, sp, #0x14
003b4b88: ldr r3, [r4, r3]
003b4b8c: add r1, pc, r1
003b4b90: add r2, sp, #0xc
003b4b94: b #0x3b4ac4
003b4b98: mov r0, r5
003b4b9c: bl #0x3a307c
003b4ba0: cmp r0, #0
003b4ba4: beq #0x3b4a88
003b4ba8: b #0x3b49f8
003b4bac: bl #0x30e310
003b4bb0: subseq r0, lr, r8, lsl #2
003b4bb4: andeq r4, r0, ip, lsr #1
003b4bb8: strdeq r3, r4, [r0], -r4
003b4bbc: subseq pc, r0, ip, ror r3
_ZN10AnimatedFX10SetVisibleEb 0x492ef0
00492ef0: push {r4, lr}
00492ef4: ldrb r2, [r0, #0x24]
00492ef8: mov r4, r0
00492efc: cmp r2, r1
00492f00: beq #0x492f38
00492f04: ldr r0, [r0, #0x2c]
00492f08: strb r1, [r4, #0x24]
00492f0c: cmp r0, #0
00492f10: beq #0x492f28
00492f14: bl #0x471368
00492f18: ldr r3, [r4, #0x2c]
00492f1c: ldrb r2, [r4, #0x24]
00492f20: ldr r3, [r3, #8]
00492f24: strb r2, [r3, #0x200]
00492f28: mov r0, r4
00492f2c: mov r1, #0
00492f30: pop {r4, lr}
00492f34: b #0x492aa0
00492f38: pop {r4, pc}
_ZN9Character13UpdateStateFXEv 0x3a4470
003a4470: push {r4, r5, r6, r7, r8, lr}
003a4474: movw r2, #0x1490
003a4478: ldr r3, [r0]
003a447c: mov r4, r0
003a4480: ldr r6, [r0, r2]
003a4484: mov lr, pc
003a4488: ldr pc, [r3, #0x34]
003a448c: ldr r5, [pc, #0x1f8]
003a4490: cmp r0, #0
003a4494: add r5, pc, r5
003a4498: bne #0x3a45a8
003a449c: cmp r6, #4
003a44a0: ble #0x3a45ac
003a44a4: mvn r7, #0
003a44a8: mov r6, #0
003a44ac: movw r3, #0x1490
003a44b0: ldr r2, [r4, r3]
003a44b4: cmp r2, r6
003a44b8: beq #0x3a45a8
003a44bc: cmn r7, #1
003a44c0: str r6, [r4, r3]
003a44c4: beq #0x3a4674
003a44c8: movw r3, #0x148c
003a44cc: ldr r3, [r4, r3]
003a44d0: cmp r3, #0
003a44d4: beq #0x3a4654
003a44d8: ldr r6, [pc, #0x1b0]
003a44dc: add r1, r4, #0x1480
003a44e0: add r1, r1, #0xc
003a44e4: ldr r0, [r5, r6]
003a44e8: bl #0x494978
003a44ec: ldr r0, [r5, r6]
003a44f0: mov r1, r7
003a44f4: mov r2, #0
003a44f8: bl #0x495430
003a44fc: movw r6, #0x148c
003a4500: cmp r0, #0
003a4504: str r0, [r4, r6]
003a4508: beq #0x3a45a8
003a450c: str r4, [r0, #0x28]
003a4510: mov r1, #1
003a4514: bl #0x492aa0
003a4518: ldr r2, [pc, #0x174]
003a451c: ldr r3, [r4, r6]
003a4520: mov r1, #0
003a4524: ldr r2, [r5, r2]
003a4528: mov r0, r3
003a452c: ldr lr, [r2]
003a4530: ldr ip, [r2, #4]
003a4534: ldr r2, [r2, #8]
003a4538: str lr, [r3, #0x34]
003a453c: str ip, [r3, #0x38]
003a4540: str r2, [r3, #0x3c]
003a4544: bl #0x492aa0
003a4548: ldr r0, [r4, r6]
003a454c: mov r1, #1
003a4550: bl #0x492ef0
003a4554: ldr r0, [r4, r6]
003a4558: bl #0x49267c
003a455c: cmp r0, #0
003a4560: beq #0x3a45a8
003a4564: ldr r0, [r4, r6]
003a4568: bl #0x49267c
003a456c: ldr r3, [r0]
003a4570: mov lr, pc
003a4574: ldr pc, [r3, #0x44]
003a4578: cmp r0, #0
003a457c: beq #0x3a45a8
003a4580: ldr r0, [r4, r6]
003a4584: bl #0x49267c
003a4588: ldr r3, [r0]
003a458c: mov lr, pc
003a4590: ldr pc, [r3, #0x44]
003a4594: mov r1, #1
003a4598: ldr r3, [r0]
003a459c: mov lr, pc
003a45a0: ldr pc, [r3, #0x40]
003a45a4: pop {r4, r5, r6, r7, r8, pc}
003a45a8: pop {r4, r5, r6, r7, r8, pc}
003a45ac: add r7, r4, #0x4f0
003a45b0: add r7, r7, #0xc
003a45b4: mov r0, r7
003a45b8: mov r1, #1
003a45bc: bl #0x3c0378
003a45c0: cmp r0, #0
003a45c4: beq #0x3a45d4
003a45c8: ldr r3, [r4, #0x520]
003a45cc: tst r3, #0x800
003a45d0: bne #0x3a465c
003a45d4: cmp r6, #3
003a45d8: bgt #0x3a44a4
003a45dc: mov r0, r7
003a45e0: mov r1, #1
003a45e4: bl #0x3c034c
003a45e8: cmp r0, #0
003a45ec: beq #0x3a461c
003a45f0: ldr r3, [r4, #0x520]
003a45f4: tst r3, #0x400
003a45f8: beq #0x3a461c
003a45fc: ldr r3, [pc, #0x94]
003a4600: mov r6, #3
003a4604: ldr r3, [r5, r3]
003a4608: ldr r3, [r3]
003a460c: ldr r7, [r3, #0x74]
003a4610: cmn r7, #1
003a4614: bne #0x3a44ac
003a4618: b #0x3a44a8
003a461c: cmp r6, #1
003a4620: bgt #0x3a44a4
003a4624: add r0, r4, #0x560
003a4628: bl #0x3de6c4
003a462c: mov r1, #0x3f800000
003a4630: bl #0x30e70c
003a4634: cmp r0, #0
003a4638: beq #0x3a44a4
003a463c: ldr r3, [pc, #0x54]
003a4640: mov r6, #1
003a4644: ldr r3, [r5, r3]
003a4648: ldr r3, [r3]
003a464c: ldr r7, [r3, #0x7c]
003a4650: b #0x3a4610
003a4654: ldr r6, [pc, #0x34]
003a4658: b #0x3a44ec
003a465c: ldr r3, [pc, #0x34]
003a4660: mov r6, #4
003a4664: ldr r3, [r5, r3]
003a4668: ldr r3, [r3]
003a466c: ldr r7, [r3, #0x80]
003a4670: b #0x3a4610
003a4674: ldr r3, [pc, #0x14]
003a4678: add r1, r4, #0x1480
003a467c: add r1, r1, #0xc
003a4680: ldr r0, [r5, r3]
003a4684: pop {r4, r5, r6, r7, r8, lr}
003a4688: b #0x494978
003a468c: ldrsheq r0, [pc], #-0x5c
003a4690: andeq r1, r0, r8, lsl #22
003a4694: andeq r3, r0, ip, lsr #30
003a4698: andeq r3, r0, r8, asr #5
_ZN6CharAI12AI_SetTargetEP10GameObjectb 0x3d6890
003d6890: push {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr r5, [pc, #0x204]
003d6898: ldr r7, [pc, #0x204]
003d689c: sub sp, sp, #0x78
003d68a0: add r5, pc, r5
003d68a4: ldr r3, [r5, r7]
003d68a8: mov r4, r0
003d68ac: cmp r2, #0
003d68b0: ldr r3, [r3]
003d68b4: mov r6, r1
003d68b8: str r1, [r4, #0x3c]
003d68bc: str r3, [sp, #0x74]
003d68c0: bne #0x3d6a20
003d68c4: ldr r3, [r0, #0x40]
003d68c8: ldr sb, [pc, #0x1d8]
003d68cc: add r8, sp, #0x5c
003d68d0: cmp r3, r1
003d68d4: ldrne r1, [r0, #4]
003d68d8: ldr sl, [r5, sb]
003d68dc: movwne r3, #0x14d0
003d68e0: strhne r2, [r1, r3]
003d68e4: mov r0, sl
003d68e8: bl #0x337888
003d68ec: ldr r1, [pc, #0x1b8]
003d68f0: add r2, sp, #0x10
003d68f4: mov r0, r8
003d68f8: add r1, pc, r1
003d68fc: bl #0x3140ec
003d6900: mov r0, sl
003d6904: mov r1, r8
003d6908: bl #0x337a88
003d690c: mov sl, r0
003d6910: ldr r0, [sp, #0x70]
003d6914: cmp r0, r8
003d6918: beq #0x3d6938
003d691c: cmp r0, #0
003d6920: beq #0x3d6938
003d6924: ldr r1, [sp, #0x5c]
003d6928: rsb r1, r0, r1
003d692c: cmp r1, #0x80
003d6930: bhi #0x3d6a4c
003d6934: bl #0x708f00
003d6938: cmp sl, #0
003d693c: beq #0x3d69a4
003d6940: ldr r3, [r4, #0x40]
003d6944: cmp r3, r6
003d6948: beq #0x3d69a4
003d694c: subs r3, r3, #0
003d6950: movne r3, #1
003d6954: subs r2, r6, #0
003d6958: movne r2, #1
003d695c: tst r2, r3
003d6960: bne #0x3d6a28
003d6964: cmp r3, #0
003d6968: beq #0x3d6a18
003d696c: ldr sl, [r5, sb]
003d6970: add r8, sp, #0x2c
003d6974: mov r0, sl
003d6978: bl #0x337888
003d697c: ldr r1, [pc, #0x12c]
003d6980: add r2, sp, #8
003d6984: mov r0, r8
003d6988: add r1, pc, r1
003d698c: bl #0x3140ec
003d6990: mov r0, sl
003d6994: mov r1, r8
003d6998: bl #0x337a88
003d699c: mov r0, r8
003d69a0: bl #0x318254
003d69a4: cmp r6, #0
003d69a8: str r6, [r4, #0x40]
003d69ac: beq #0x3d69fc
003d69b0: ldr r0, [r4, #4]
003d69b4: bl #0x3a2fec
003d69b8: ldr r2, [r4, #0x44]
003d69bc: ldr r3, [r4, #0x40]
003d69c0: cmp r3, r2
003d69c4: movne r2, #0
003d69c8: strbne r2, [r4, #0x4c]
003d69cc: movne r2, r3
003d69d0: str r2, [r4, #0x44]
003d69d4: mov r0, r3
003d69d8: ldr r3, [r3]
003d69dc: mov lr, pc
003d69e0: ldr pc, [r3, #0x34]
003d69e4: eor r0, r0, #1
003d69e8: strb r0, [r4, #0x48]
003d69ec: ldr r1, [r4, #0x40]
003d69f0: mov r0, r4
003d69f4: bl #0x3d4ed8
003d69f8: strb r0, [r4, #0x49]
003d69fc: ldr r3, [r5, r7]
003d6a00: ldr r2, [sp, #0x74]
003d6a04: ldr r3, [r3]
003d6a08: cmp r2, r3
003d6a0c: bne #0x3d6a9c
003d6a10: add sp, sp, #0x78
003d6a14: pop {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp r2, #0
003d6a1c: bne #0x3d6a5c
003d6a20: str r6, [r4, #0x40]
003d6a24: b #0x3d69fc
003d6a28: ldr sl, [r5, sb]
003d6a2c: add r8, sp, #0x44
003d6a30: mov r0, sl
003d6a34: bl #0x337888
003d6a38: ldr r1, [pc, #0x74]
003d6a3c: add r2, sp, #0xc
003d6a40: mov r0, r8
003d6a44: add r1, pc, r1
003d6a48: b #0x3d698c
003d6a4c: bl #0x310440
003d6a50: cmp sl, #0
003d6a54: beq #0x3d69a4
003d6a58: b #0x3d6940
003d6a5c: ldr sl, [r5, sb]
003d6a60: add r8, sp, #0x14
003d6a64: mov r0, sl
003d6a68: bl #0x337888
003d6a6c: ldr r1, [pc, #0x44]
003d6a70: add r2, sp, #4
003d6a74: mov r0, r8
003d6a78: add r1, pc, r1
003d6a7c: bl #0x3140ec
003d6a80: mov r1, r8
003d6a84: mov r0, sl
003d6a88: bl #0x337a88
003d6a8c: mov r0, r8
003d6a90: bl #0x318254
003d6a94: str r6, [r4, #0x40]
003d6a98: b #0x3d69b0
003d6a9c: bl #0x30e310
003d6aa0: ldrsheq lr, [fp], #-0x10
003d6aa4: andeq r4, r0, ip, lsr #1
003d6aa8: andeq r0, r0, r4, lsl #17
003d6aac: subeq lr, lr, r0, ror #27
003d6ab0: subeq lr, lr, r8, ror #26
003d6ab4: subeq lr, lr, ip, lsr #25
003d6ab8: subeq lr, lr, r8, ror ip
_ZN10GameObject20UpdateTargetPositionEv 0x393d74
00393d74: push {r4, lr}
00393d78: ldr r1, [r0, #0x180]
00393d7c: sub sp, sp, #0x10
00393d80: mov r4, r0
00393d84: cmp r1, #0
00393d88: beq #0x393dac
00393d8c: add r0, sp, #4
00393d90: bl #0x597180
00393d94: ldr r2, [sp, #8]
00393d98: ldr r3, [sp, #0xc]
00393d9c: ldr r1, [sp, #4]
00393da0: str r2, [r4, #0x188]
00393da4: str r3, [r4, #0x18c]
00393da8: str r1, [r4, #0x184]
00393dac: add sp, sp, #0x10
00393db0: pop {r4, pc}
_ZN9Character20_EnableSpotTargetingERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv 0x3b7144
003b7144: push {r4, r5, r6, lr}
003b7148: ldr r3, [r0, #4]
003b714c: ldr r4, [pc, #0x248]
003b7150: sub sp, sp, #8
003b7154: ldr r1, [r3, #4]
003b7158: ldr ip, [r3]
003b715c: add r4, pc, r4
003b7160: mov r5, r0
003b7164: rsb r3, ip, r1
003b7168: asr r3, r3, #4
003b716c: add r1, r3, r3, lsl #3
003b7170: add r1, r1, r1, lsl #6
003b7174: add r1, r3, r1, lsl #3
003b7178: add r1, r1, r1, lsl #15
003b717c: add r3, r3, r1, lsl #3
003b7180: cmp r3, #0
003b7184: bne #0x3b7190
003b7188: add sp, sp, #8
003b718c: pop {r4, r5, r6, pc}
003b7190: ldr r6, [ip, #4]
003b7194: cmp r6, #1
003b7198: bne #0x3b7188
003b719c: mov r1, #0
003b71a0: str r2, [sp, #4]
003b71a4: bl #0x37baf8
003b71a8: bl #0x31bc80
003b71ac: cmp r0, #0
003b71b0: ldr r2, [sp, #4]
003b71b4: beq #0x3b722c
003b71b8: ldr r1, [r5, #4]
003b71bc: ldr r3, [r1]
003b71c0: ldr r1, [r1, #4]
003b71c4: rsb r3, r3, r1
003b71c8: asr r3, r3, #4
003b71cc: add r1, r3, r3, lsl #3
003b71d0: add r1, r1, r1, lsl #6
003b71d4: add r1, r3, r1, lsl #3
003b71d8: add r1, r1, r1, lsl #15
003b71dc: add r3, r3, r1, lsl #3
003b71e0: rsb r3, r3, #0
003b71e4: cmp r3, #1
003b71e8: bls #0x3b7188
003b71ec: mov r0, r5
003b71f0: mov r1, r6
003b71f4: bl #0x37baf8
003b71f8: ldr r3, [r0, #4]
003b71fc: cmp r3, #3
003b7200: bne #0x3b7188
003b7204: mov r1, r6
003b7208: mov r0, r5
003b720c: bl #0x37baf8
003b7210: bl #0x38d798
003b7214: ldr r3, [pc, #0x184]
003b7218: ldr r2, [sp, #4]
003b721c: ldr r3, [r4, r3]
003b7220: ldr r3, [r3]
003b7224: cmp r0, r3
003b7228: bhs #0x3b7188
003b722c: mov r1, #0
003b7230: mov r0, r5
003b7234: str r2, [sp, #4]
003b7238: bl #0x37baf8
003b723c: bl #0x31bc80
003b7240: cmp r0, #0
003b7244: ldr r2, [sp, #4]
003b7248: beq #0x3b72e8
003b724c: ldr r3, [r5, #4]
003b7250: ldm r3, {r1, r3}
003b7254: rsb r3, r1, r3
003b7258: asr r3, r3, #4
003b725c: add r1, r3, r3, lsl #3
003b7260: add r1, r1, r1, lsl #6
003b7264: add r1, r3, r1, lsl #3
003b7268: add r1, r1, r1, lsl #15
003b726c: add r3, r3, r1, lsl #3
003b7270: rsb r3, r3, #0
003b7274: cmp r3, #2
003b7278: bhi #0x3b7300
003b727c: mov r1, #1
003b7280: mov r0, r5
003b7284: str r2, [sp, #4]
003b7288: bl #0x37baf8
003b728c: bl #0x38d798
003b7290: ldr r2, [sp, #4]
003b7294: movw r4, #0x14ca
003b7298: ldr ip, [r2, #0x160]
003b729c: ldr r1, [r2, #0x164]
003b72a0: ldr r3, [r2, #0x168]
003b72a4: strh r0, [r2, r4]
003b72a8: movw r0, #0x14c8
003b72ac: mov r4, #1
003b72b0: strb r4, [r2, r0]
003b72b4: movw r0, #0x14bc
003b72b8: str ip, [r2, r0]
003b72bc: mov r0, #0x14c0
003b72c0: str r1, [r2, r0]
003b72c4: movw r0, #0x14c4
003b72c8: str r3, [r2, r0]
003b72cc: movw r0, #0x14b0
003b72d0: str ip, [r2, r0]
003b72d4: movw r0, #0x14b4
003b72d8: str r1, [r2, r0]
003b72dc: movw r1, #0x14b8
003b72e0: str r3, [r2, r1]
003b72e4: b #0x3b7188
003b72e8: movw r3, #0x14c8
003b72ec: strb r0, [r2, r3]
003b72f0: mvn r1, #0
003b72f4: movw r3, #0x14ca
003b72f8: strh r1, [r2, r3]
003b72fc: b #0x3b7188
003b7300: mov r0, r5
003b7304: mov r1, #2
003b7308: str r2, [sp, #4]
003b730c: bl #0x37baf8
003b7310: ldr r3, [r0, #4]
003b7314: ldr r2, [sp, #4]
003b7318: cmp r3, #7
003b731c: bne #0x3b727c
003b7320: mov r1, #2
003b7324: mov r0, r5
003b7328: bl #0x37baf8
003b732c: bl #0x31b5a0
003b7330: mov r1, #1
003b7334: mov r4, r0
003b7338: mov r0, r5
003b733c: bl #0x37baf8
003b7340: bl #0x38d798
003b7344: ldr r2, [sp, #4]
003b7348: mov r1, #1
003b734c: movw r3, #0x14c8
003b7350: strb r1, [r2, r3]
003b7354: ldr ip, [r4, #0x160]
003b7358: movw r3, #0x14b0
003b735c: str ip, [r2, r3]
003b7360: ldr r1, [r4, #0x164]
003b7364: movw r3, #0x14b4
003b7368: str r1, [r2, r3]
003b736c: ldr r3, [r4, #0x168]
003b7370: movw r4, #0x14ca
003b7374: strh r0, [r2, r4]
003b7378: movw r0, #0x14bc
003b737c: str ip, [r2, r0]
003b7380: mov r0, #0x14c0
003b7384: str r1, [r2, r0]
003b7388: movw r1, #0x14c4
003b738c: str r3, [r2, r1]
003b7390: movw r1, #0x14b8
003b7394: str r3, [r2, r1]
003b7398: b #0x3b7188
003b739c: subseq sp, sp, r4, lsr sb
003b73a0: andeq r0, r0, r4, asr #13
_ZN9CharacterC2EN10ObjectBase6GO_IDSE 0x3a9340
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
