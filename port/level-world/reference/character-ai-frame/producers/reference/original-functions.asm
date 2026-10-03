
# _ZN9Character6UpdateEv
003abe98: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003abe9c: ldr      r5, [pc, #0xe54]
003abea0: ldr      r6, [pc, #0xe54]
003abea4: mov      r4, r0
003abea8: add      r5, pc, r5
003abeac: ldr      r3, [r5, r6]
003abeb0: ldr      r0, [pc, #0xe48]
003abeb4: ldr      r7, [pc, #0xe48]
003abeb8: ldr      r3, [r3]
003abebc: sub      sp, sp, #0x9c
003abec0: add      r0, pc, r0
003abec4: str      r3, [sp, #0x94]
003abec8: bl       #0x3136b4
003abecc: ldr      sl, [r5, r7]
003abed0: add      r8, sp, #0x7c
003abed4: mov      r0, sl
003abed8: bl       #0x337888
003abedc: ldr      r1, [pc, #0xe24]
003abee0: add      r2, sp, #0x48
003abee4: mov      r0, r8
003abee8: add      r1, pc, r1
003abeec: bl       #0x3140ec
003abef0: mov      r0, sl
003abef4: mov      r1, r8
003abef8: bl       #0x337a88
003abefc: mov      sl, r0
003abf00: mov      r0, r8
003abf04: bl       #0x318254
003abf08: cmp      sl, #0
003abf0c: bne      #0x3ac300
003abf10: ldr      sl, [r5, r7]
003abf14: add      r8, sp, #0x64
003abf18: mov      r0, sl
003abf1c: bl       #0x337888
003abf20: ldr      r1, [pc, #0xde4]
003abf24: add      r2, sp, #0x44
003abf28: mov      r0, r8
003abf2c: add      r1, pc, r1
003abf30: bl       #0x3140ec
003abf34: mov      r0, sl
003abf38: mov      r1, r8
003abf3c: bl       #0x337a88
003abf40: mov      sl, r0
003abf44: mov      r0, r8
003abf48: bl       #0x318254
003abf4c: cmp      sl, #0
003abf50: bne      #0x3ac2d8
003abf54: ldr      r3, [r4]
003abf58: mov      r0, r4
003abf5c: mov      lr, pc
003abf60: ldr      pc, [r3, #0x148]
003abf64: cmp      r0, #0
003abf68: beq      #0x3ac204
003abf6c: ldr      r0, [pc, #0xd9c]
003abf70: add      r8, r4, #0x4f0
003abf74: add      r8, r8, #0xc
003abf78: add      r0, pc, r0
003abf7c: bl       #0x3136b4
003abf80: mov      r0, r8
003abf84: bl       #0x3c01ac
003abf88: cmp      r0, #0
003abf8c: bne      #0x3ac348
003abf90: ldr      sl, [pc, #0xd7c]
003abf94: add      r2, r4, #0x3c8
003abf98: str      r2, [sp, #8]
003abf9c: ldr      r0, [pc, #0xd74]
003abfa0: add      sb, sp, #0x4c
003abfa4: add      r0, pc, r0
003abfa8: bl       #0x3136b8
003abfac: ldr      fp, [r5, sl]
003abfb0: ldr      r3, [fp, #0x38]
003abfb4: ldr      r2, [r3, #0x5c]
003abfb8: add      r2, r2, #1
003abfbc: str      r2, [r3, #0x5c]
003abfc0: ldr      r3, [r4, #0x378]
003abfc4: mov      r0, r3
003abfc8: ldr      r3, [r3]
003abfcc: mov      lr, pc
003abfd0: ldr      pc, [r3, #8]
003abfd4: ldr      r7, [r5, r7]
003abfd8: mov      r0, r7
003abfdc: bl       #0x337888
003abfe0: ldr      r1, [pc, #0xd34]
003abfe4: add      r2, sp, #0x40
003abfe8: mov      r0, sb
003abfec: add      r1, pc, r1
003abff0: bl       #0x3140ec
003abff4: mov      r0, r7
003abff8: mov      r1, sb
003abffc: bl       #0x337a88
003ac000: mov      r7, r0
003ac004: mov      r0, sb
003ac008: bl       #0x318254
003ac00c: cmp      r7, #0
003ac010: bne      #0x3ac618
003ac014: movw     r3, #0x14e0
003ac018: ldr      r0, [r4, r3]
003ac01c: cmp      r0, #0
003ac020: beq      #0x3ac028
003ac024: bl       #0x317ae4
003ac028: add      r0, r4, #0x3b4
003ac02c: bl       #0x3db640
003ac030: ldr      r0, [sp, #8]
003ac034: bl       #0x3cfbf4
003ac038: mov      r0, r8
003ac03c: bl       #0x3c628c
003ac040: add      r0, r4, #0x490
003ac044: add      r0, r0, #0xc
003ac048: bl       #0x3caf3c
003ac04c: movw     r7, #0x14fc
003ac050: mov      r0, r4
003ac054: bl       #0x38cbe8
003ac058: ldr      r8, [r4, r7]
003ac05c: mov      r1, #0
003ac060: mov      r0, r8
003ac064: bl       #0x30e2f8
003ac068: cmp      r0, #0
003ac06c: bne      #0x3ac894
003ac070: ldr      r3, [pc, #0xca8]
003ac074: ldr      r3, [r5, r3]
003ac078: ldrb     r3, [r3]
003ac07c: cmp      r3, #0
003ac080: bne      #0x3ac22c
003ac084: ldr      r3, [pc, #0xc98]
003ac088: ldr      r3, [r5, r3]
003ac08c: ldrb     r3, [r3]
003ac090: cmp      r3, #0
003ac094: bne      #0x3ac22c
003ac098: ldr      r3, [r4]
003ac09c: mov      r0, r4
003ac0a0: mov      lr, pc
003ac0a4: ldr      pc, [r3, #0x28]
003ac0a8: cmp      r0, #0
003ac0ac: beq      #0x3ac27c
003ac0b0: mov      r0, r4
003ac0b4: bl       #0x3abb9c
003ac0b8: mov      r0, r4
003ac0bc: bl       #0x3a469c
003ac0c0: ldr      r3, [r5, sl]
003ac0c4: ldr      r3, [r3, #0x74]
003ac0c8: tst      r3, #7
003ac0cc: beq      #0x3ac888
003ac0d0: mov      r3, #0x1480
003ac0d4: ldrb     r3, [r4, r3]
003ac0d8: cmp      r3, #0
003ac0dc: beq      #0x3ac740
003ac0e0: movw     r3, #0x1494
003ac0e4: ldr      r3, [r4, r3]
003ac0e8: cmp      r3, #0
003ac0ec: beq      #0x3ac10c
003ac0f0: movw     r2, #0x1498
003ac0f4: ldr      r2, [r4, r2]
003ac0f8: cmp      r2, #0
003ac0fc: blt      #0x3ac10c
003ac100: ldr      r0, [r3, r2, lsl #2]
003ac104: mov      r1, #0
003ac108: bl       #0x492ef0
003ac10c: movw     r3, #0x149c
003ac110: ldr      r0, [r4, r3]
003ac114: cmp      r0, #0
003ac118: beq      #0x3ac124
003ac11c: mov      r1, #0
003ac120: bl       #0x492ef0
003ac124: bl       #0x3a42f4
003ac128: cmp      r0, #0
003ac12c: beq      #0x3ac148
003ac130: movw     r3, #0x14a0
003ac134: ldr      r0, [r4, r3]
003ac138: cmp      r0, #0
003ac13c: beq      #0x3ac148
003ac140: mov      r1, #1
003ac144: bl       #0x492ef0
003ac148: ldr      r3, [pc, #0xbd8]
003ac14c: ldr      r0, [r5, r3]
003ac150: bl       #0x455c54
003ac154: cmp      r0, #0
003ac158: bne      #0x3ac1f4
003ac15c: mov      r7, #0x1500
003ac160: ldr      r1, [r4, r7]
003ac164: cmn      r1, #1
003ac168: beq      #0x3ac17c
003ac16c: mov      r0, r4
003ac170: bl       #0x3aef68
003ac174: mvn      r3, #0
003ac178: str      r3, [r4, r7]
003ac17c: movw     r7, #0x1504
003ac180: ldr      r3, [r4, r7]
003ac184: cmn      r3, #1
003ac188: beq      #0x3ac1f4
003ac18c: ldr      sb, [r5, sl]
003ac190: mov      r1, r4
003ac194: ldr      r0, [sb, #0x40]
003ac198: bl       #0x36effc
003ac19c: cmp      r0, #0
003ac1a0: beq      #0x3ac1f4
003ac1a4: bl       #0x413e90
003ac1a8: ldr      r1, [pc, #0xb7c]
003ac1ac: ldr      sl, [r4, r7]
003ac1b0: add      r1, pc, r1
003ac1b4: bl       #0x414678
003ac1b8: ldr      r1, [pc, #0xb70]
003ac1bc: ldr      r2, [pc, #0xb70]
003ac1c0: mov      r8, r0
003ac1c4: add      r1, pc, r1
003ac1c8: add      r2, pc, r2
003ac1cc: ldr      r0, [sb, #0x2c]
003ac1d0: bl       #0x4c4bdc
003ac1d4: asr      sl, sl, #8
003ac1d8: mov      r3, r0
003ac1dc: mov      r1, sl
003ac1e0: mov      r0, r4
003ac1e4: mov      r2, r8
003ac1e8: bl       #0x3af0c8
003ac1ec: mvn      r3, #0
003ac1f0: str      r3, [r4, r7]
003ac1f4: ldr      r0, [pc, #0xb3c]
003ac1f8: add      r0, pc, r0
003ac1fc: bl       #0x3136b8
003ac200: b        #0x3ac210
003ac204: ldr      r0, [pc, #0xb30]
003ac208: add      r0, pc, r0
003ac20c: bl       #0x3136b8
003ac210: ldr      r3, [r5, r6]
003ac214: ldr      r2, [sp, #0x94]
003ac218: ldr      r3, [r3]
003ac21c: cmp      r2, r3
003ac220: bne      #0x3aca98
003ac224: add      sp, sp, #0x9c
003ac228: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ac22c: mov      r0, r4
003ac230: bl       #0x3a3094
003ac234: cmp      r0, #0
003ac238: beq      #0x3ac098
003ac23c: ldr      r3, [pc, #0xafc]
003ac240: ldr      r1, [r4, #0x164]
003ac244: ldr      r0, [r4, #0x160]
003ac248: ldr      r3, [r5, r3]
003ac24c: ldr      r2, [r4, #0x168]
003ac250: str      r1, [r3, #4]
003ac254: mov      r1, #0
003ac258: str      r0, [r3]
003ac25c: str      r1, [r3, #0xc]
003ac260: str      r2, [r3, #8]
003ac264: ldr      r3, [r4]
003ac268: mov      r0, r4
003ac26c: mov      lr, pc
003ac270: ldr      pc, [r3, #0x28]
003ac274: cmp      r0, #0
003ac278: bne      #0x3ac0b0
003ac27c: mov      r0, r4
003ac280: bl       #0x3a30c4
003ac284: cmp      r0, #0
003ac288: bne      #0x3ac574
003ac28c: movw     r3, #0x14e4
003ac290: ldrb     r3, [r4, r3]
003ac294: cmp      r3, #0
003ac298: bne      #0x3ac900
003ac29c: bl       #0x7fd794
003ac2a0: ldrb     r3, [r0, #5]
003ac2a4: cmp      r3, #0
003ac2a8: beq      #0x3ac0c0
003ac2ac: ldrb     r3, [r4, #0x118]
003ac2b0: cmp      r3, #0
003ac2b4: beq      #0x3ac0c0
003ac2b8: ldr      r3, [r4, #0x520]
003ac2bc: tst      r3, #0x100
003ac2c0: beq      #0x3ac0c0
003ac2c4: ldr      r3, [r4, #0x110]
003ac2c8: cmn      r3, #1
003ac2cc: moveq    r3, #0
003ac2d0: strbeq   r3, [r4, #0x118]
003ac2d4: b        #0x3ac0c0
003ac2d8: ldr      r3, [r4]
003ac2dc: mov      r0, r4
003ac2e0: mov      lr, pc
003ac2e4: ldr      pc, [r3, #0x28]
003ac2e8: cmp      r0, #0
003ac2ec: beq      #0x3abf54
003ac2f0: add      r0, r4, #0x37c
003ac2f4: mov      r1, #0x32
003ac2f8: bl       #0x3ffc40
003ac2fc: b        #0x3abf54
003ac300: ldr      sl, [pc, #0xa0c]
003ac304: mov      r1, #0
003ac308: mov      r2, r1
003ac30c: ldr      r3, [r5, sl]
003ac310: ldr      r0, [r3, #0x40]
003ac314: bl       #0x36e744
003ac318: ldr      r3, [r0, #0x660]
003ac31c: cmp      r4, r3
003ac320: bne      #0x3abf10
003ac324: movw     r3, #0x1088
003ac328: ldr      r2, [r4, r3]
003ac32c: cmp      r2, #0
003ac330: ble      #0x3ac940
003ac334: sub      r2, r2, #0xc8
003ac338: add      r0, r4, #0x560
003ac33c: mov      r1, #0x24
003ac340: bl       #0x3e07a0
003ac344: b        #0x3abf10
003ac348: mov      r0, r8
003ac34c: bl       #0x3c01ac
003ac350: cmp      r0, #0xc
003ac354: beq      #0x3ac608
003ac358: mov      r0, r8
003ac35c: bl       #0x3c01ac
003ac360: cmp      r0, #2
003ac364: beq      #0x3abf90
003ac368: mov      r3, #0x1480
003ac36c: ldrb     r3, [r4, r3]
003ac370: cmp      r3, #0
003ac374: bne      #0x3ac608
003ac378: ldr      r3, [r4, #0x3e4]
003ac37c: cmp      r3, #0
003ac380: bne      #0x3abf90
003ac384: ldr      sl, [pc, #0x988]
003ac388: ldr      r0, [r5, sl]
003ac38c: bl       #0x31f594
003ac390: ldr      r3, [r0, #0x3c]
003ac394: cmp      r3, #0x1d
003ac398: beq      #0x3aca48
003ac39c: ldr      sb, [pc, #0x9a0]
003ac3a0: ldr      r3, [r5, sb]
003ac3a4: ldr      r3, [r3, #0x10]
003ac3a8: cmp      r3, #8
003ac3ac: movls    r3, #0
003ac3b0: movhi    r3, #1
003ac3b4: cmp      r3, #0
003ac3b8: beq      #0x3ac3f4
003ac3bc: ldr      r3, [r5, sb]
003ac3c0: ldr      fp, [r3, #8]
003ac3c4: cmp      fp, r3
003ac3c8: beq      #0x3ac3f4
003ac3cc: ldr      r3, [fp, #0x14]
003ac3d0: mov      r1, #0
003ac3d4: mov      r2, #1
003ac3d8: ldr      r0, [r3, #0x378]
003ac3dc: bl       #0x40570c
003ac3e0: add      r1, sp, #0x98
003ac3e4: ldr      r0, [fp, #0x14]
003ac3e8: mov      r2, #1
003ac3ec: str      fp, [r1, #-0x5c]!
003ac3f0: bl       #0x3a7b24
003ac3f4: add      r3, r4, #0x3c8
003ac3f8: mov      r0, r3
003ac3fc: mov      r1, #1
003ac400: str      r3, [sp, #8]
003ac404: bl       #0x3cf3a4
003ac408: cmp      r0, #0
003ac40c: beq      #0x3abf9c
003ac410: mov      r0, r4
003ac414: bl       #0x3a3064
003ac418: cmp      r0, #0
003ac41c: beq      #0x3abf9c
003ac420: mov      r0, r4
003ac424: bl       #0x3a3144
003ac428: cmp      r0, #0
003ac42c: bne      #0x3abf9c
003ac430: mov      r0, r4
003ac434: bl       #0x3a3158
003ac438: cmp      r0, #0
003ac43c: bne      #0x3abf9c
003ac440: bl       #0x7fd794
003ac444: ldrb     r3, [r0, #5]
003ac448: cmp      r3, #0
003ac44c: bne      #0x3abf9c
003ac450: ldrb     r3, [r4, #0x3ec]
003ac454: cmp      r3, #0
003ac458: beq      #0x3abf9c
003ac45c: bl       #0x60b0cc
003ac460: ldr      r2, [r5, sb]
003ac464: ldr      ip, [r2, #4]
003ac468: cmp      ip, #0
003ac46c: moveq    ip, r2
003ac470: bne      #0x3ac480
003ac474: b        #0x3ac4b4
003ac478: mov      r2, ip
003ac47c: mov      ip, r3
003ac480: ldr      r3, [ip, #0x10]
003ac484: cmp      r0, r3
003ac488: ldrgt    r3, [ip, #0xc]
003ac48c: ldrle    r3, [ip, #8]
003ac490: movgt    ip, r2
003ac494: cmp      r3, #0
003ac498: bne      #0x3ac478
003ac49c: ldr      r3, [r5, sb]
003ac4a0: cmp      ip, r3
003ac4a4: beq      #0x3ac4b4
003ac4a8: ldr      r3, [ip, #0x10]
003ac4ac: cmp      r0, r3
003ac4b0: bge      #0x3ac56c
003ac4b4: ldr      r3, [r5, sb]
003ac4b8: mov      fp, #0
003ac4bc: str      r0, [sp, #0x30]
003ac4c0: ldr      lr, [r3, #8]
003ac4c4: str      fp, [sp, #0x34]
003ac4c8: cmp      lr, ip
003ac4cc: beq      #0x3acae4
003ac4d0: cmp      ip, r3
003ac4d4: beq      #0x3acc50
003ac4d8: ldrb     r3, [ip]
003ac4dc: cmp      r3, #0
003ac4e0: bne      #0x3ac4f8
003ac4e4: ldr      r3, [ip, #4]
003ac4e8: ldr      r3, [r3, #4]
003ac4ec: cmp      r3, ip
003ac4f0: ldreq    lr, [ip, #0xc]
003ac4f4: beq      #0x3ac518
003ac4f8: ldr      lr, [ip, #8]
003ac4fc: cmp      lr, #0
003ac500: bne      #0x3ac50c
003ac504: b        #0x3acc7c
003ac508: mov      lr, r3
003ac50c: ldr      r3, [lr, #0xc]
003ac510: cmp      r3, #0
003ac514: bne      #0x3ac508
003ac518: ldr      r2, [ip, #0x10]
003ac51c: cmp      r0, r2
003ac520: movge    fp, #0
003ac524: movlt    fp, #1
003ac528: cmp      fp, #0
003ac52c: str      r2, [sp, #0xc]
003ac530: beq      #0x3aca9c
003ac534: ldr      r3, [lr, #0x10]
003ac538: cmp      r0, r3
003ac53c: ble      #0x3aca9c
003ac540: ldr      r3, [lr, #0xc]
003ac544: cmp      r3, #0
003ac548: movne    r1, ip
003ac54c: beq      #0x3acbf8
003ac550: mov      ip, #0
003ac554: add      r0, sp, #0x38
003ac558: add      r2, sp, #0x30
003ac55c: mov      r3, r1
003ac560: str      ip, [sp]
003ac564: bl       #0x3aa77c
003ac568: ldr      ip, [sp, #0x38]
003ac56c: str      r4, [ip, #0x14]
003ac570: b        #0x3abf9c
003ac574: ldrb     r3, [r4, #0x8a]
003ac578: cmp      r3, #0
003ac57c: beq      #0x3ac28c
003ac580: ldrb     r3, [r4, #0x2fa]
003ac584: cmp      r3, #0
003ac588: bne      #0x3ac5e0
003ac58c: movw     r7, #0x1484
003ac590: ldr      r3, [r4, r7]
003ac594: cmp      r3, #0
003ac598: bne      #0x3ac0c0
003ac59c: ldr      r3, [pc, #0x7a4]
003ac5a0: mov      r1, #0x8b
003ac5a4: mov      r2, r4
003ac5a8: ldr      r0, [r5, r3]
003ac5ac: bl       #0x495430
003ac5b0: cmp      r0, #0
003ac5b4: str      r0, [r4, r7]
003ac5b8: beq      #0x3ac0c0
003ac5bc: bl       #0x49267c
003ac5c0: ldr      r3, [r0]
003ac5c4: mov      lr, pc
003ac5c8: ldr      pc, [r3, #0x44]
003ac5cc: mov      r1, #1
003ac5d0: ldr      r3, [r0]
003ac5d4: mov      lr, pc
003ac5d8: ldr      pc, [r3, #0x40]
003ac5dc: b        #0x3ac0c0
003ac5e0: movw     r3, #0x1484
003ac5e4: ldr      r3, [r4, r3]
003ac5e8: cmp      r3, #0
003ac5ec: beq      #0x3ac0c0
003ac5f0: ldr      r3, [pc, #0x750]
003ac5f4: add      r1, r4, #0x1480
003ac5f8: add      r1, r1, #4
003ac5fc: ldr      r0, [r5, r3]
003ac600: bl       #0x494978
003ac604: b        #0x3ac0c0
003ac608: add      r3, r4, #0x3c8
003ac60c: ldr      sl, [pc, #0x700]
003ac610: str      r3, [sp, #8]
003ac614: b        #0x3abf9c
003ac618: bl       #0x7fd794
003ac61c: ldrb     r3, [r0, #5]
003ac620: cmp      r3, #0
003ac624: beq      #0x3ac014
003ac628: ldr      r3, [r4]
003ac62c: mov      r0, r4
003ac630: mov      lr, pc
003ac634: ldr      pc, [r3, #0x28]
003ac638: cmp      r0, #0
003ac63c: beq      #0x3ac014
003ac640: ldr      r0, [fp, #0x40]
003ac644: mov      r1, r4
003ac648: mov      r2, #0
003ac64c: bl       #0x36eea8
003ac650: ldrb     r3, [r0, #0x66c]
003ac654: cmp      r3, #0
003ac658: beq      #0x3ac014
003ac65c: mov      r1, r4
003ac660: ldr      r0, [fp, #0x40]
003ac664: mov      r2, #0
003ac668: bl       #0x36eea8
003ac66c: ldr      r1, [r0, #0x678]
003ac670: cmp      r1, #0
003ac674: ble      #0x3ac014
003ac678: ldr      r0, [fp, #0x40]
003ac67c: sub      r1, r1, #1
003ac680: mov      r2, #0
003ac684: bl       #0x36e744
003ac688: ldr      r7, [r0, #0x660]
003ac68c: cmp      r7, #0
003ac690: beq      #0x3ac014
003ac694: ldr      sb, [pc, #0x6b0]
003ac698: mov      r0, r4
003ac69c: add      sb, pc, sb
003ac6a0: ldr      r2, [sb, #0x24]
003ac6a4: ldr      r3, [sb, #0x28]
003ac6a8: add      r2, r2, #1
003ac6ac: add      r3, r3, #1
003ac6b0: str      r2, [sb, #0x24]
003ac6b4: str      r3, [sb, #0x28]
003ac6b8: bl       #0x3bd110
003ac6bc: cmp      r0, #0x45
003ac6c0: ble      #0x3aca18
003ac6c4: movw     r3, #0x14a4
003ac6c8: ldr      r0, [r4, r3]
003ac6cc: cmp      r0, #0
003ac6d0: beq      #0x3ac6f4
003ac6d4: movw     r3, #0x14ac
003ac6d8: ldrb     r3, [r4, r3]
003ac6dc: cmp      r3, #0
003ac6e0: beq      #0x3ac978
003ac6e4: movw     r3, #0x14a8
003ac6e8: ldrsb    r3, [r4, r3]
003ac6ec: cmp      r3, #8
003ac6f0: beq      #0x3ac990
003ac6f4: add      fp, r7, #0x160
003ac6f8: add      sb, r4, #0x160
003ac6fc: mov      r1, sb
003ac700: mov      r0, fp
003ac704: bl       #0x3a4118
003ac708: mov      r1, #0x43000000
003ac70c: add      r1, r1, #0x960000
003ac710: bl       #0x30e2f8
003ac714: cmp      r0, #0
003ac718: bne      #0x3aca70
003ac71c: ldr      r3, [pc, #0x62c]
003ac720: add      r3, pc, r3
003ac724: ldr      r2, [r3, #0x28]
003ac728: cmp      r2, #0x3e8
003ac72c: bgt      #0x3aca80
003ac730: ldr      r0, [r4, #0x378]
003ac734: mov      r1, #0
003ac738: bl       #0x4053d0
003ac73c: b        #0x3ac014
003ac740: ldr      r3, [r5, sl]
003ac744: mov      r1, r4
003ac748: ldr      r0, [r3, #0x40]
003ac74c: bl       #0x36effc
003ac750: cmp      r0, #0
003ac754: beq      #0x3ac0e0
003ac758: movw     r3, #0x1494
003ac75c: ldr      r3, [r4, r3]
003ac760: cmp      r3, #0
003ac764: beq      #0x3ac820
003ac768: ldr      r7, [r4, #0x40c]
003ac76c: cmp      r7, #0
003ac770: beq      #0x3ac8b4
003ac774: cmp      r4, r7
003ac778: beq      #0x3ac8b4
003ac77c: movw     r3, #0x149c
003ac780: ldr      r0, [r4, r3]
003ac784: cmp      r0, #0
003ac788: beq      #0x3ac794
003ac78c: mov      r1, #0
003ac790: bl       #0x492ef0
003ac794: ldr      r3, [r7]
003ac798: mov      r0, r7
003ac79c: mov      r1, r4
003ac7a0: mov      lr, pc
003ac7a4: ldr      pc, [r3, #0x90]
003ac7a8: subs     r8, r0, #0
003ac7ac: blt      #0x3ac820
003ac7b0: cmp      r8, #1
003ac7b4: beq      #0x3ac930
003ac7b8: movw     sb, #0x1494
003ac7bc: ldr      r3, [r4, sb]
003ac7c0: ldr      r0, [r3, r8, lsl #2]
003ac7c4: cmp      r0, #0
003ac7c8: beq      #0x3ac820
003ac7cc: str      r7, [r0, #0x28]
003ac7d0: mov      r1, #1
003ac7d4: bl       #0x492aa0
003ac7d8: movw     r3, #0x1498
003ac7dc: ldr      r3, [r4, r3]
003ac7e0: cmp      r8, r3
003ac7e4: beq      #0x3ac820
003ac7e8: cmp      r3, #0
003ac7ec: blt      #0x3ac808
003ac7f0: ldr      r2, [r4, sb]
003ac7f4: ldr      r0, [r2, r3, lsl #2]
003ac7f8: cmp      r0, #0
003ac7fc: beq      #0x3ac80c
003ac800: mov      r1, #0
003ac804: bl       #0x492ef0
003ac808: ldr      r2, [r4, sb]
003ac80c: ldr      r0, [r2, r8, lsl #2]
003ac810: mov      r1, #1
003ac814: bl       #0x492ef0
003ac818: movw     r3, #0x1498
003ac81c: str      r8, [r4, r3]
003ac820: movw     r3, #0x149c
003ac824: ldr      r0, [r4, r3]
003ac828: cmp      r0, #0
003ac82c: beq      #0x3ac124
003ac830: ldr      r3, [r4, #0x378]
003ac834: ldrb     r2, [r3, #9]
003ac838: cmp      r2, #0
003ac83c: bne      #0x3ac860
003ac840: ldr      r2, [pc, #0x50c]
003ac844: ldr      r2, [r5, r2]
003ac848: ldrb     r2, [r2]
003ac84c: cmp      r2, #0
003ac850: bne      #0x3ac11c
003ac854: ldrb     r3, [r3, #8]
003ac858: cmp      r3, #0
003ac85c: bne      #0x3ac11c
003ac860: ldrb     r3, [r4, #0x1b5]
003ac864: cmp      r3, #0
003ac868: beq      #0x3ac11c
003ac86c: ldr      r0, [r5, sl]
003ac870: bl       #0x320e74
003ac874: cmp      r0, #0
003ac878: beq      #0x3ac124
003ac87c: movw     r3, #0x149c
003ac880: ldr      r0, [r4, r3]
003ac884: b        #0x3ac11c
003ac888: mov      r0, r4
003ac88c: bl       #0x3a4470
003ac890: b        #0x3ac0d0
003ac894: ldr      r0, [r5, sl]
003ac898: bl       #0x31f66c
003ac89c: bl       #0x30e2e0
003ac8a0: mov      r1, r0
003ac8a4: mov      r0, r8
003ac8a8: bl       #0x30e3ac
003ac8ac: str      r0, [r4, r7]
003ac8b0: b        #0x3ac070
003ac8b4: movw     r2, #0x14a4
003ac8b8: ldr      r7, [r4, r2]
003ac8bc: cmp      r7, #0
003ac8c0: bne      #0x3ac8f4
003ac8c4: movw     r7, #0x1498
003ac8c8: ldr      r2, [r4, r7]
003ac8cc: cmp      r2, #0
003ac8d0: blt      #0x3ac820
003ac8d4: ldr      r0, [r3, r2, lsl #2]
003ac8d8: cmp      r0, #0
003ac8dc: beq      #0x3ac820
003ac8e0: mov      r1, #0
003ac8e4: bl       #0x492ef0
003ac8e8: mvn      r3, #0
003ac8ec: str      r3, [r4, r7]
003ac8f0: b        #0x3ac820
003ac8f4: cmp      r4, r7
003ac8f8: beq      #0x3ac8c4
003ac8fc: b        #0x3ac77c
003ac900: bl       #0x7fd794
003ac904: ldrb     r3, [r0, #5]
003ac908: cmp      r3, #0
003ac90c: beq      #0x3ac29c
003ac910: movw     r2, #0x14e5
003ac914: ldrb     r3, [r4, r2]
003ac918: cmp      r3, #0
003ac91c: moveq    r1, #1
003ac920: strbeq   r1, [r4, r2]
003ac924: strbeq   r3, [r4, #0x118]
003ac928: beq      #0x3ac0c0
003ac92c: b        #0x3ac29c
003ac930: mov      r0, r7
003ac934: mov      r1, r4
003ac938: bl       #0x3ec048
003ac93c: b        #0x3ac7b8
003ac940: mov      r1, #0x24
003ac944: add      r0, r4, #0x560
003ac948: mov      r2, #0
003ac94c: bl       #0x3e07a0
003ac950: ldr      r3, [r4]
003ac954: mov      r0, r4
003ac958: mov      lr, pc
003ac95c: ldr      pc, [r3, #0x34]
003ac960: subs     r1, r0, #0
003ac964: bne      #0x3abf10
003ac968: ldr      r0, [r4, #0x378]
003ac96c: mov      r2, r1
003ac970: bl       #0x40570c
003ac974: b        #0x3abf10
003ac978: movw     r3, #0x14a8
003ac97c: ldrsb    r3, [r4, r3]
003ac980: cmp      r3, #1
003ac984: beq      #0x3ac9c8
003ac988: cmn      r3, #2
003ac98c: beq      #0x3ac9c8
003ac990: add      sb, r4, #0x160
003ac994: mov      r1, sb
003ac998: add      r0, r0, #0x160
003ac99c: bl       #0x3a4118
003ac9a0: mov      r1, #0x43000000
003ac9a4: add      r1, r1, #0x960000
003ac9a8: bl       #0x30e2f8
003ac9ac: subs     r1, r0, #0
003ac9b0: beq      #0x3aca64
003ac9b4: movw     r3, #0x14a4
003ac9b8: ldr      r1, [r4, r3]
003ac9bc: ldr      r0, [r4, #0x378]
003ac9c0: bl       #0x4053d0
003ac9c4: b        #0x3ac014
003ac9c8: ldr      r3, [r0, #0x3bc]
003ac9cc: cmp      r4, r3
003ac9d0: bne      #0x3ac6f4
003ac9d4: add      r0, r4, #0x37c
003ac9d8: bl       #0x3fe330
003ac9dc: cmp      r0, #0
003ac9e0: bne      #0x3ac6f4
003ac9e4: add      fp, r7, #0x160
003ac9e8: add      sb, r4, #0x160
003ac9ec: mov      r1, sb
003ac9f0: mov      r0, fp
003ac9f4: bl       #0x3a4118
003ac9f8: mov      r1, #0x43000000
003ac9fc: add      r1, r1, #0xfa0000
003aca00: bl       #0x30e70c
003aca04: cmp      r0, #0
003aca08: beq      #0x3ac6fc
003aca0c: movw     r3, #0x14a4
003aca10: ldr      r0, [r4, r3]
003aca14: b        #0x3ac994
003aca18: mov      r0, r4
003aca1c: bl       #0x3a5990
003aca20: cmp      r0, #0
003aca24: beq      #0x3ac6c4
003aca28: ldr      r3, [sb, #0x24]
003aca2c: cmp      r3, #0x64
003aca30: ble      #0x3ac6c4
003aca34: ldr      r0, [r4, #0x378]
003aca38: bl       #0x4056b0
003aca3c: mov      r3, #0
003aca40: str      r3, [sb, #0x24]
003aca44: b        #0x3ac014
003aca48: ldr      sb, [pc, #0x2f4]
003aca4c: ldr      r3, [r5, sb]
003aca50: ldr      r3, [r3, #0x10]
003aca54: cmp      r3, #0x18
003aca58: movls    r3, #0
003aca5c: movhi    r3, #1
003aca60: b        #0x3ac3b4
003aca64: ldr      r0, [r4, #0x378]
003aca68: bl       #0x4057fc
003aca6c: b        #0x3ac014
003aca70: mov      r1, r7
003aca74: ldr      r0, [r4, #0x378]
003aca78: bl       #0x4053d0
003aca7c: b        #0x3ac014
003aca80: str      r0, [r3, #0x28]
003aca84: ldr      r3, [r4]
003aca88: mov      r0, r4
003aca8c: mov      lr, pc
003aca90: ldr      pc, [r3, #0x138]
003aca94: b        #0x3ac730
003aca98: bl       #0x30e310
003aca9c: ldr      r3, [ip, #0xc]
003acaa0: cmp      r3, #0
003acaa4: beq      #0x3acb7c
003acaa8: mov      r1, r3
003acaac: b        #0x3acab4
003acab0: mov      r1, r2
003acab4: ldr      r2, [r1, #8]
003acab8: cmp      r2, #0
003acabc: bne      #0x3acab0
003acac0: cmp      fp, #0
003acac4: beq      #0x3acb28
003acac8: add      r0, sp, #0x10
003acacc: add      r1, sp, #0x30
003acad0: bl       #0x3aa8b4
003acad4: ldr      r3, [sp, #0x10]
003acad8: str      r3, [sp, #0x38]
003acadc: ldr      ip, [sp, #0x38]
003acae0: b        #0x3ac56c
003acae4: ldr      r3, [r3, #0x10]
003acae8: cmp      r3, fp
003acaec: beq      #0x3acc34
003acaf0: ldr      r3, [ip, #0x10]
003acaf4: cmp      r0, r3
003acaf8: blt      #0x3acc14
003acafc: ble      #0x3acb70
003acb00: ldr      r3, [ip, #0xc]
003acb04: cmp      r3, #0
003acb08: beq      #0x3acbac
003acb0c: mov      r1, r3
003acb10: b        #0x3acb18
003acb14: mov      r1, r2
003acb18: ldr      r2, [r1, #8]
003acb1c: cmp      r2, #0
003acb20: bne      #0x3acb14
003acb24: b        #0x3acbd8
003acb28: ldr      r2, [sp, #0xc]
003acb2c: cmp      r0, r2
003acb30: ble      #0x3acb70
003acb34: ldr      r2, [r5, sb]
003acb38: cmp      r1, r2
003acb3c: beq      #0x3acb4c
003acb40: ldr      r2, [r1, #0x10]
003acb44: cmp      r0, r2
003acb48: bge      #0x3acac8
003acb4c: cmp      r3, #0
003acb50: bne      #0x3ac550
003acb54: mov      r1, ip
003acb58: add      r0, sp, #0x38
003acb5c: add      r2, sp, #0x30
003acb60: str      ip, [sp]
003acb64: bl       #0x3aa77c
003acb68: ldr      ip, [sp, #0x38]
003acb6c: b        #0x3ac56c
003acb70: str      ip, [sp, #0x38]
003acb74: ldr      ip, [sp, #0x38]
003acb78: b        #0x3ac56c
003acb7c: ldr      r1, [ip, #4]
003acb80: mov      r2, ip
003acb84: b        #0x3acb90
003acb88: mov      r2, r1
003acb8c: ldr      r1, [r1, #4]
003acb90: ldr      lr, [r1, #0xc]
003acb94: cmp      lr, r2
003acb98: beq      #0x3acb88
003acb9c: ldr      lr, [r2, #0xc]
003acba0: cmp      r1, lr
003acba4: moveq    r1, r2
003acba8: b        #0x3acac0
003acbac: ldr      r1, [ip, #4]
003acbb0: mov      r2, ip
003acbb4: ldr      ip, [r1, #0xc]
003acbb8: cmp      ip, r2
003acbbc: bne      #0x3acbcc
003acbc0: mov      r2, r1
003acbc4: ldr      r1, [r1, #4]
003acbc8: b        #0x3acbb4
003acbcc: ldr      ip, [r2, #0xc]
003acbd0: cmp      r1, ip
003acbd4: moveq    r1, r2
003acbd8: ldr      r2, [r5, sb]
003acbdc: cmp      r1, r2
003acbe0: beq      #0x3accd8
003acbe4: ldr      r2, [r1, #0x10]
003acbe8: cmp      r0, r2
003acbec: bge      #0x3accbc
003acbf0: cmp      r3, #0
003acbf4: bne      #0x3ac550
003acbf8: mov      r1, lr
003acbfc: add      r0, sp, #0x38
003acc00: add      r2, sp, #0x30
003acc04: str      lr, [sp]
003acc08: bl       #0x3aa77c
003acc0c: ldr      ip, [sp, #0x38]
003acc10: b        #0x3ac56c
003acc14: mov      r1, ip
003acc18: mov      r3, ip
003acc1c: add      r0, sp, #0x38
003acc20: add      r2, sp, #0x30
003acc24: str      fp, [sp]
003acc28: bl       #0x3aa77c
003acc2c: ldr      ip, [sp, #0x38]
003acc30: b        #0x3ac56c
003acc34: add      r0, sp, #0x28
003acc38: add      r1, sp, #0x30
003acc3c: bl       #0x3aa8b4
003acc40: ldr      r3, [sp, #0x28]
003acc44: str      r3, [sp, #0x38]
003acc48: ldr      ip, [sp, #0x38]
003acc4c: b        #0x3ac56c
003acc50: ldr      r1, [ip, #0xc]
003acc54: ldr      r3, [r1, #0x10]
003acc58: cmp      r0, r3
003acc5c: ble      #0x3acca0
003acc60: mov      r3, fp
003acc64: add      r0, sp, #0x38
003acc68: add      r2, sp, #0x30
003acc6c: str      ip, [sp]
003acc70: bl       #0x3aa77c
003acc74: ldr      ip, [sp, #0x38]
003acc78: b        #0x3ac56c
003acc7c: ldr      lr, [ip, #4]
003acc80: mov      r3, ip
003acc84: b        #0x3acc90
003acc88: mov      r3, lr
003acc8c: ldr      lr, [lr, #4]
003acc90: ldr      r2, [lr, #8]
003acc94: cmp      r2, r3
003acc98: beq      #0x3acc88
003acc9c: b        #0x3ac518
003acca0: add      r0, sp, #0x18
003acca4: add      r1, sp, #0x30
003acca8: bl       #0x3aa8b4
003accac: ldr      r3, [sp, #0x18]
003accb0: str      r3, [sp, #0x38]
003accb4: ldr      ip, [sp, #0x38]
003accb8: b        #0x3ac56c
003accbc: add      r0, sp, #0x20
003accc0: add      r1, sp, #0x30
003accc4: bl       #0x3aa8b4
003accc8: ldr      r3, [sp, #0x20]
003acccc: str      r3, [sp, #0x38]
003accd0: ldr      ip, [sp, #0x38]
003accd4: b        #0x3ac56c
003accd8: mov      r1, lr
003accdc: add      r0, sp, #0x38
003acce0: add      r2, sp, #0x30
003acce4: mov      r3, #0
003acce8: str      lr, [sp]
003accec: bl       #0x3aa77c
003accf0: ldr      ip, [sp, #0x38]
003accf4: b        #0x3ac56c
003accf8: subseq   r8, lr, r8, ror #23
003accfc: andeq    r4, r0, ip, lsr #1
003acd00: ldrsbeq  r7, [r1], #-0x70
003acd04: andeq    r0, r0, r4, lsl #17
003acd08: subseq   r7, r1, r0, asr #15
003acd0c: subseq   r4, r1, ip, lsl #9
003acd10: subseq   r7, r1, r0, asr #14
003acd14: strdeq   r3, r4, [r0], -r4
003acd18: subseq   r7, r1, r4, lsl r7
003acd1c: subseq   r7, r1, ip, ror #13
003acd20: andeq    r4, r0, r8, lsr #9
003acd24: andeq    r2, r0, r4, ror r4
003acd28: andeq    r1, r0, r0, lsr #20
003acd2c: subseq   r7, r1, r0, lsr r5
003acd30: subseq   r7, r1, ip, lsr #10
003acd34: subseq   r7, r1, r0, asr #10

# _ZN6CharAI14AI_PauseUpdateEj
003cb748: str      lr, [sp, #-4]!
003cb74c: ldr      r3, [r0, #4]
003cb750: mov      ip, #0
003cb754: mov      r2, #1
003cb758: strb     r2, [r0, #0x18]
003cb75c: sub      sp, sp, #0xc
003cb760: add      r0, r3, #0x3b4
003cb764: mov      r2, ip
003cb768: mov      r3, #0x31
003cb76c: str      ip, [sp]
003cb770: bl       #0x3dbe24
003cb774: add      sp, sp, #0xc
003cb778: ldm      sp!, {pc}

# _ZN11Application9ComputeDtEv
00320da4: push     {r4, r5, r6, lr}
00320da8: mov      r4, r0
00320dac: bl       #0x60b0cc
00320db0: ldr      r3, [r4, #0x88]
00320db4: str      r0, [r4, #0x88]
00320db8: rsb      r0, r3, r0
00320dbc: str      r0, [r4, #0x8c]
00320dc0: bl       #0x30e2e0
00320dc4: ldr      r1, [r4, #0x9c]
00320dc8: bl       #0x30ed6c
00320dcc: bl       #0x30e4cc
00320dd0: str      r0, [r4, #0x8c]
00320dd4: bl       #0x30e2e0
00320dd8: ldr      r1, [r4, #0x98]
00320ddc: bl       #0x30ed6c
00320de0: ldr      r1, [r4, #0x94]
00320de4: bl       #0x30eba4
00320de8: mov      r5, r0
00320dec: bl       #0x30e4cc
00320df0: str      r0, [r4, #0x90]
00320df4: bl       #0x30e2e0
00320df8: mov      r1, r0
00320dfc: mov      r0, r5
00320e00: bl       #0x30e3ac
00320e04: str      r0, [r4, #0x94]
00320e08: pop      {r4, r5, r6, pc}

# _ZN11Application7_UpdateEi
0032c438: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0032c43c: ldr      r4, [pc, #0x818]
0032c440: ldr      r2, [pc, #0x818]
0032c444: mov      r5, r0
0032c448: add      r4, pc, r4
0032c44c: ldr      r3, [r4, r2]
0032c450: ldr      r0, [pc, #0x80c]
0032c454: sub      sp, sp, #0x1bc
0032c458: ldr      r3, [r3]
0032c45c: add      r0, pc, r0
0032c460: str      r2, [sp, #8]
0032c464: mov      r7, r1
0032c468: str      r3, [sp, #0x1b4]
0032c46c: bl       #0x3136b4
0032c470: ldr      r3, [pc, #0x7f0]
0032c474: ldr      r6, [pc, #0x7f0]
0032c478: add      r8, sp, #0x19c
0032c47c: ldr      r0, [r4, r3]
0032c480: bl       #0x52e784
0032c484: ldr      sl, [r4, r6]
0032c488: mov      r0, sl
0032c48c: bl       #0x337888
0032c490: ldr      r1, [pc, #0x7d8]
0032c494: add      r2, sp, #0x48
0032c498: mov      r0, r8
0032c49c: add      r1, pc, r1
0032c4a0: bl       #0x3140ec
0032c4a4: mov      r0, sl
0032c4a8: mov      r1, r8
0032c4ac: mov      r2, #0
0032c4b0: bl       #0x337ddc
0032c4b4: mov      r0, r8
0032c4b8: bl       #0x3139ac
0032c4bc: ldrb     r3, [r5, #0xaa]
0032c4c0: cmp      r3, #0
0032c4c4: bne      #0x32cb5c
0032c4c8: ldr      r3, [pc, #0x7a4]
0032c4cc: ldr      r3, [r4, r3]
0032c4d0: ldrb     r3, [r3]
0032c4d4: cmp      r3, #0
0032c4d8: bne      #0x32c81c
0032c4dc: ldr      r8, [pc, #0x794]
0032c4e0: ldr      fp, [pc, #0x794]
0032c4e4: mov      r0, r5
0032c4e8: bl       #0x31f594
0032c4ec: mov      r1, #1
0032c4f0: mov      sl, r0
0032c4f4: mov      r0, r5
0032c4f8: bl       #0x321000
0032c4fc: cmp      r0, #0
0032c500: bne      #0x32c854
0032c504: ldr      r3, [pc, #0x774]
0032c508: ldr      r3, [r4, r3]
0032c50c: ldrb     r3, [r3]
0032c510: cmp      r3, #0
0032c514: beq      #0x32c878
0032c518: ldr      r3, [r4, r8]
0032c51c: ldr      r3, [r3]
0032c520: cmp      r3, #0x11
0032c524: beq      #0x32cc34
0032c528: ldr      r0, [r5, #4]
0032c52c: ldrb     r3, [r0, #8]
0032c530: cmp      r3, #0
0032c534: bne      #0x32cb74
0032c538: bl       #0x3cb2f0
0032c53c: ldr      r3, [r4, fp]
0032c540: ldr      r0, [r3, #0x40]
0032c544: bl       #0x378fb4
0032c548: mov      r0, r7
0032c54c: bl       #0x30ed30
0032c550: mov      sb, r1
0032c554: mov      r2, r0
0032c558: mov      r3, r1
0032c55c: mov      r8, r0
0032c560: ldr      r0, [r5, #0x14]
0032c564: bl       #0x33900c
0032c568: ldr      r0, [r5, #0x18]
0032c56c: mov      r3, sb
0032c570: mov      r2, r8
0032c574: bl       #0x33a7f4
0032c578: ldr      r3, [pc, #0x704]
0032c57c: ldr      r3, [r4, r3]
0032c580: ldr      r0, [r3]
0032c584: cmp      r0, #0
0032c588: beq      #0x32c590
0032c58c: bl       #0x36934c
0032c590: ldr      r0, [r5, #0x20]
0032c594: cmp      r0, #0
0032c598: beq      #0x32c5a8
0032c59c: mov      r2, r8
0032c5a0: mov      r3, sb
0032c5a4: bl       #0x33b334
0032c5a8: ldr      r0, [r5, #0x24]
0032c5ac: cmp      r0, #0
0032c5b0: beq      #0x32c5c0
0032c5b4: mov      r2, r8
0032c5b8: mov      r3, sb
0032c5bc: bl       #0x33d744
0032c5c0: bl       #0x34dda4
0032c5c4: mov      r8, r0
0032c5c8: mov      r0, r7
0032c5cc: bl       #0x30e964
0032c5d0: ldr      r7, [r8]
0032c5d4: mov      r1, r0
0032c5d8: mov      r0, r8
0032c5dc: mov      lr, pc
0032c5e0: ldr      pc, [r7, #0xc]
0032c5e4: mov      r0, r5
0032c5e8: bl       #0x321164
0032c5ec: ldr      r7, [pc, #0x694]
0032c5f0: ldr      sl, [r4, r6]
0032c5f4: add      r8, sp, #0x184
0032c5f8: add      r7, pc, r7
0032c5fc: mov      r0, sl
0032c600: bl       #0x337888
0032c604: add      r2, sp, #0x44
0032c608: mov      r1, r7
0032c60c: mov      r0, r8
0032c610: bl       #0x3140ec
0032c614: mov      r1, r8
0032c618: mov      r0, sl
0032c61c: bl       #0x337a88
0032c620: mov      sb, r0
0032c624: mov      r0, r8
0032c628: bl       #0x3139ac
0032c62c: cmp      sb, #0
0032c630: bne      #0x32cb20
0032c634: ldr      sl, [r4, r6]
0032c638: ldr      r7, [pc, #0x64c]
0032c63c: add      r8, sp, #0x154
0032c640: mov      r0, sl
0032c644: add      r7, pc, r7
0032c648: bl       #0x337888
0032c64c: add      r2, sp, #0x3c
0032c650: mov      r1, r7
0032c654: mov      r0, r8
0032c658: bl       #0x3140ec
0032c65c: mov      r1, r8
0032c660: mov      r0, sl
0032c664: bl       #0x337a88
0032c668: mov      sb, r0
0032c66c: mov      r0, r8
0032c670: bl       #0x3139ac
0032c674: cmp      sb, #0
0032c678: bne      #0x32cae0
0032c67c: ldr      sl, [r4, r6]
0032c680: ldr      r7, [pc, #0x608]
0032c684: add      r8, sp, #0x124
0032c688: mov      r0, sl
0032c68c: add      r7, pc, r7
0032c690: bl       #0x337888
0032c694: add      r2, sp, #0x34
0032c698: mov      r1, r7
0032c69c: mov      r0, r8
0032c6a0: bl       #0x3140ec
0032c6a4: mov      r1, r8
0032c6a8: mov      r0, sl
0032c6ac: bl       #0x337a88
0032c6b0: mov      sb, r0
0032c6b4: mov      r0, r8
0032c6b8: bl       #0x3139ac
0032c6bc: cmp      sb, #0
0032c6c0: bne      #0x32ca9c
0032c6c4: ldr      sl, [r4, r6]
0032c6c8: ldr      r7, [pc, #0x5c4]
0032c6cc: add      r8, sp, #0xf4
0032c6d0: mov      r0, sl
0032c6d4: add      r7, pc, r7
0032c6d8: bl       #0x337888
0032c6dc: add      r2, sp, #0x2c
0032c6e0: mov      r1, r7
0032c6e4: mov      r0, r8
0032c6e8: bl       #0x3140ec
0032c6ec: mov      r1, r8
0032c6f0: mov      r0, sl
0032c6f4: bl       #0x337a88
0032c6f8: mov      sb, r0
0032c6fc: mov      r0, r8
0032c700: bl       #0x3139ac
0032c704: cmp      sb, #0
0032c708: bne      #0x32ca44
0032c70c: ldr      sl, [r4, r6]
0032c710: ldr      r7, [pc, #0x580]
0032c714: add      r8, sp, #0xc4
0032c718: mov      r0, sl
0032c71c: add      r7, pc, r7
0032c720: bl       #0x337888
0032c724: add      r2, sp, #0x24
0032c728: mov      r1, r7
0032c72c: mov      r0, r8
0032c730: bl       #0x3140ec
0032c734: mov      r1, r8
0032c738: mov      r0, sl
0032c73c: bl       #0x337a88
0032c740: mov      sb, r0
0032c744: mov      r0, r8
0032c748: bl       #0x3139ac
0032c74c: cmp      sb, #0
0032c750: bne      #0x32c9ec
0032c754: ldr      sl, [r4, r6]
0032c758: ldr      r7, [pc, #0x53c]
0032c75c: add      r8, sp, #0x94
0032c760: mov      r0, sl
0032c764: add      r7, pc, r7
0032c768: bl       #0x337888
0032c76c: add      r2, sp, #0x1c
0032c770: mov      r1, r7
0032c774: mov      r0, r8
0032c778: bl       #0x3140ec
0032c77c: mov      r1, r8
0032c780: mov      r0, sl
0032c784: bl       #0x337a88
0032c788: mov      sb, r0
0032c78c: mov      r0, r8
0032c790: bl       #0x3139ac
0032c794: cmp      sb, #0
0032c798: bne      #0x32c99c
0032c79c: ldr      r8, [r4, r6]
0032c7a0: ldr      r6, [pc, #0x4f8]
0032c7a4: add      r7, sp, #0x64
0032c7a8: mov      r0, r8
0032c7ac: add      r6, pc, r6
0032c7b0: bl       #0x337888
0032c7b4: add      r2, sp, #0x14
0032c7b8: mov      r1, r6
0032c7bc: mov      r0, r7
0032c7c0: bl       #0x3140ec
0032c7c4: mov      r1, r7
0032c7c8: mov      r0, r8
0032c7cc: bl       #0x337a88
0032c7d0: mov      sl, r0
0032c7d4: mov      r0, r7
0032c7d8: bl       #0x3139ac
0032c7dc: cmp      sl, #0
0032c7e0: bne      #0x32c92c
0032c7e4: ldr      r3, [r5, #0x74]
0032c7e8: ldr      r0, [pc, #0x4b4]
0032c7ec: add      r3, r3, #1
0032c7f0: str      r3, [r5, #0x74]
0032c7f4: add      r0, pc, r0
0032c7f8: bl       #0x3136b8
0032c7fc: ldr      r2, [sp, #8]
0032c800: ldr      r3, [r4, r2]
0032c804: ldr      r2, [sp, #0x1b4]
0032c808: ldr      r3, [r3]
0032c80c: cmp      r2, r3
0032c810: bne      #0x32cc58
0032c814: add      sp, sp, #0x1bc
0032c818: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0032c81c: ldr      r8, [pc, #0x454]
0032c820: ldr      r3, [r4, r8]
0032c824: ldr      r3, [r3]
0032c828: cmp      r3, #0xa
0032c82c: beq      #0x32cb7c
0032c830: mov      r0, r5
0032c834: bl       #0x31f594
0032c838: mov      r1, #1
0032c83c: mov      sl, r0
0032c840: mov      r0, r5
0032c844: bl       #0x321000
0032c848: cmp      r0, #0
0032c84c: ldr      fp, [pc, #0x428]
0032c850: beq      #0x32c504
0032c854: bl       #0x7fd794
0032c858: ldrb     r3, [r0, #5]
0032c85c: cmp      r3, #0
0032c860: beq      #0x32c518
0032c864: ldr      r3, [pc, #0x414]
0032c868: ldr      r3, [r4, r3]
0032c86c: ldrb     r3, [r3]
0032c870: cmp      r3, #0
0032c874: bne      #0x32c518
0032c878: mov      r0, r5
0032c87c: bl       #0x31f5b4
0032c880: cmp      r0, #0
0032c884: beq      #0x32c518
0032c888: mov      r0, r5
0032c88c: bl       #0x31f5e8
0032c890: cmp      r0, #0
0032c894: bne      #0x32c518
0032c898: cmp      sl, #0
0032c89c: beq      #0x32c8b8
0032c8a0: ldrb     r3, [sl, #0x144]
0032c8a4: cmp      r3, #0
0032c8a8: beq      #0x32c518
0032c8ac: ldrb     r3, [sl, #0x1a8]
0032c8b0: cmp      r3, #0
0032c8b4: bne      #0x32c518
0032c8b8: ldr      r3, [r4, fp]
0032c8bc: ldr      sl, [pc, #0x3e4]
0032c8c0: mov      sb, #1
0032c8c4: ldr      r3, [r3, #0x10]
0032c8c8: add      sl, pc, sl
0032c8cc: mov      r0, sl
0032c8d0: ldr      r3, [r3, #0x1c]
0032c8d4: str      r3, [sp, #0xc]
0032c8d8: bl       #0x3136b4
0032c8dc: bl       #0x35c058
0032c8e0: ldr      r0, [sp, #0xc]
0032c8e4: bl       #0x35c51c
0032c8e8: ldr      r3, [sp, #0xc]
0032c8ec: mov      r0, r7
0032c8f0: strb     sb, [r3, #0x250]
0032c8f4: ldr      r2, [sp, #0xc]
0032c8f8: ldr      r3, [r2]
0032c8fc: str      r3, [sp, #4]
0032c900: bl       #0x30e964
0032c904: mov      r2, #0
0032c908: mov      r1, r0
0032c90c: ldr      r3, [sp, #4]
0032c910: ldr      r0, [sp, #0xc]
0032c914: mov      lr, pc
0032c918: ldr      pc, [r3, #0x60]
0032c91c: strb     sb, [r5, #0xa9]
0032c920: mov      r0, sl
0032c924: bl       #0x3136b8
0032c928: b        #0x32c518
0032c92c: add      r7, sp, #0x4c
0032c930: mov      r0, r8
0032c934: bl       #0x337888
0032c938: mov      r1, r6
0032c93c: add      r2, sp, #0x10
0032c940: mov      r0, r7
0032c944: bl       #0x3140ec
0032c948: mov      r1, r7
0032c94c: mov      r2, #0
0032c950: mov      r0, r8
0032c954: bl       #0x337ddc
0032c958: mov      r0, r7
0032c95c: bl       #0x3139ac
0032c960: mov      r0, r5
0032c964: bl       #0x31f594
0032c968: subs     r6, r0, #0
0032c96c: beq      #0x32c7e4
0032c970: ldr      r3, [r4, fp]
0032c974: ldr      r1, [pc, #0x330]
0032c978: ldr      r2, [pc, #0x330]
0032c97c: ldr      r0, [r3, #0x2c]
0032c980: add      r1, pc, r1
0032c984: add      r2, pc, r2
0032c988: bl       #0x4c4bdc
0032c98c: mov      r1, r0
0032c990: mov      r0, r6
0032c994: bl       #0x3ef728
0032c998: b        #0x32c7e4
0032c99c: add      r8, sp, #0x7c
0032c9a0: mov      r0, sl
0032c9a4: bl       #0x337888
0032c9a8: mov      r1, r7
0032c9ac: add      r2, sp, #0x18
0032c9b0: mov      r0, r8
0032c9b4: bl       #0x3140ec
0032c9b8: mov      r1, r8
0032c9bc: mov      r2, #0
0032c9c0: mov      r0, sl
0032c9c4: bl       #0x337ddc
0032c9c8: mov      r0, r8
0032c9cc: bl       #0x3139ac
0032c9d0: mov      r0, r5
0032c9d4: bl       #0x31f594
0032c9d8: cmp      r0, #0
0032c9dc: beq      #0x32c79c
0032c9e0: mvn      r1, #0
0032c9e4: bl       #0x3ef728
0032c9e8: b        #0x32c79c
0032c9ec: ldr      r3, [r4, fp]
0032c9f0: ldr      r2, [pc, #0x2bc]
0032c9f4: mov      r1, #0
0032c9f8: ldr      r3, [r3, #0x10]
0032c9fc: ldr      r2, [r4, r2]
0032ca00: add      r8, sp, #0xac
0032ca04: ldr      r3, [r3, #0x1c]
0032ca08: ldr      r0, [r3, #4]
0032ca0c: bl       #0x51073c
0032ca10: mov      r0, sl
0032ca14: bl       #0x337888
0032ca18: mov      r1, r7
0032ca1c: add      r2, sp, #0x20
0032ca20: mov      r0, r8
0032ca24: bl       #0x3140ec
0032ca28: mov      r0, sl
0032ca2c: mov      r1, r8
0032ca30: mov      r2, #0
0032ca34: bl       #0x337ddc
0032ca38: mov      r0, r8
0032ca3c: bl       #0x3139ac
0032ca40: b        #0x32c754
0032ca44: ldr      r3, [r4, fp]
0032ca48: ldr      r2, [pc, #0x268]
0032ca4c: mov      r1, #0
0032ca50: ldr      r3, [r3, #0x10]
0032ca54: ldr      r2, [r4, r2]
0032ca58: add      r8, sp, #0xdc
0032ca5c: ldr      r3, [r3, #0x1c]
0032ca60: ldr      r0, [r3, #4]
0032ca64: bl       #0x50e7c4
0032ca68: mov      r0, sl
0032ca6c: bl       #0x337888
0032ca70: mov      r1, r7
0032ca74: add      r2, sp, #0x28
0032ca78: mov      r0, r8
0032ca7c: bl       #0x3140ec
0032ca80: mov      r0, sl
0032ca84: mov      r1, r8
0032ca88: mov      r2, #0
0032ca8c: bl       #0x337ddc
0032ca90: mov      r0, r8
0032ca94: bl       #0x3139ac
0032ca98: b        #0x32c70c
0032ca9c: ldr      r3, [r4, fp]
0032caa0: add      r8, sp, #0x10c
0032caa4: ldr      r0, [r3, #0x38]
0032caa8: bl       #0x340304
0032caac: mov      r0, sl
0032cab0: bl       #0x337888
0032cab4: mov      r1, r7
0032cab8: add      r2, sp, #0x30
0032cabc: mov      r0, r8
0032cac0: bl       #0x3140ec
0032cac4: mov      r0, sl
0032cac8: mov      r1, r8
0032cacc: mov      r2, #0
0032cad0: bl       #0x337ddc
0032cad4: mov      r0, r8
0032cad8: bl       #0x3139ac
0032cadc: b        #0x32c6c4
0032cae0: bl       #0x50e2b8
0032cae4: add      r8, sp, #0x13c
0032cae8: bl       #0x50e228
0032caec: mov      r0, sl
0032caf0: bl       #0x337888
0032caf4: mov      r1, r7
0032caf8: add      r2, sp, #0x38
0032cafc: mov      r0, r8
0032cb00: bl       #0x3140ec
0032cb04: mov      r0, sl
0032cb08: mov      r1, r8
0032cb0c: mov      r2, #0
0032cb10: bl       #0x337ddc
0032cb14: mov      r0, r8
0032cb18: bl       #0x3139ac
0032cb1c: b        #0x32c67c
0032cb20: bl       #0x50e228
0032cb24: add      r8, sp, #0x16c
0032cb28: mov      r0, sl
0032cb2c: bl       #0x337888
0032cb30: mov      r1, r7
0032cb34: add      r2, sp, #0x40
0032cb38: mov      r0, r8
0032cb3c: bl       #0x3140ec
0032cb40: mov      r0, sl
0032cb44: mov      r1, r8
0032cb48: mov      r2, #0
0032cb4c: bl       #0x337ddc
0032cb50: mov      r0, r8
0032cb54: bl       #0x3139ac
0032cb58: b        #0x32c634
0032cb5c: mov      r3, #0
0032cb60: strb     r3, [r5, #0xaa]
0032cb64: mov      r0, r5
0032cb68: mov      r1, #3
0032cb6c: bl       #0x32c1f4
0032cb70: b        #0x32c4c8
0032cb74: bl       #0x317ef8
0032cb78: b        #0x32c538
0032cb7c: mov      r3, #0
0032cb80: mov      r0, #0x10
0032cb84: str      r3, [sp, #4]
0032cb88: bl       #0x310454
0032cb8c: ldr      r3, [sp, #4]
0032cb90: mov      fp, #0x42000000
0032cb94: add      fp, fp, #0xc80000
0032cb98: str      r3, [r0]
0032cb9c: str      r3, [r0, #4]
0032cba0: str      fp, [r0, #8]
0032cba4: str      fp, [r0, #0xc]
0032cba8: mov      sb, r0
0032cbac: mov      r0, #0x10
0032cbb0: str      r3, [sp, #4]
0032cbb4: bl       #0x310454
0032cbb8: ldr      r3, [sp, #4]
0032cbbc: str      fp, [r0, #8]
0032cbc0: str      fp, [r0, #4]
0032cbc4: str      r3, [r0]
0032cbc8: mov      r3, #0x43000000
0032cbcc: add      r3, r3, #0x480000
0032cbd0: str      r3, [r0, #0xc]
0032cbd4: ldr      r3, [r5, #0x20]
0032cbd8: mov      sl, r0
0032cbdc: mov      r1, sb
0032cbe0: mov      r0, r3
0032cbe4: ldr      r3, [r3]
0032cbe8: mov      lr, pc
0032cbec: ldr      pc, [r3, #0x30]
0032cbf0: cmp      r0, #0
0032cbf4: bne      #0x32cc3c
0032cbf8: ldr      r3, [r5, #0x20]
0032cbfc: mov      r1, sl
0032cc00: mov      r0, r3
0032cc04: ldr      r3, [r3]
0032cc08: mov      lr, pc
0032cc0c: ldr      pc, [r3, #0x30]
0032cc10: cmp      r0, #0
0032cc14: beq      #0x32c830
0032cc18: ldr      fp, [pc, #0x5c]
0032cc1c: ldr      r1, [pc, #0x98]
0032cc20: ldr      r3, [r4, fp]
0032cc24: add      r1, pc, r1
0032cc28: ldr      r0, [r3, #0x4c]
0032cc2c: bl       #0x46d578
0032cc30: b        #0x32c4e4
0032cc34: bl       #0x314734
0032cc38: b        #0x32c538
0032cc3c: ldr      fp, [pc, #0x38]
0032cc40: ldr      r1, [pc, #0x78]
0032cc44: ldr      r3, [r4, fp]
0032cc48: add      r1, pc, r1
0032cc4c: ldr      r0, [r3, #0x4c]
0032cc50: bl       #0x46d578
0032cc54: b        #0x32c4e4
0032cc58: bl       #0x30e310
0032cc5c: rsbeq    r8, r6, r8, asr #12
0032cc60: andeq    r4, r0, ip, lsr #1
0032cc64: subseq   r2, sb, r4, asr sp
0032cc68: andeq    r1, r0, r4, ror r4
0032cc6c: andeq    r0, r0, r4, lsl #17
0032cc70: subseq   r2, sb, ip, lsr #26
0032cc74: strheq   r3, [r0], -r0
0032cc78: andeq    r3, r0, r0, asr r8
0032cc7c: strdeq   r3, r4, [r0], -r4
0032cc80: andeq    r2, r0, r0, lsr #31
0032cc84: andeq    r0, r0, r4, lsr #27
0032cc88: subseq   r2, sb, r0, lsl ip
0032cc8c: subseq   r2, sb, r4, ror #23
0032cc90: ldrheq   r2, [sb], #-0xbc
0032cc94: subseq   r2, sb, ip, lsl #23
0032cc98: subseq   r2, sb, r4, ror #22
0032cc9c: subseq   r2, sb, r4, lsr fp
0032cca0: subseq   r2, sb, ip, lsl #22
0032cca4: ldrheq   r2, [sb], #-0x9c
0032cca8: subseq   r2, sb, r8, lsr #18
0032ccac: subseq   r2, sb, r8, asr sb
0032ccb0: subseq   r2, sb, r4, ror #18
0032ccb4: andeq    r2, r0, ip, lsl #28
0032ccb8: andeq    r1, r0, r4, lsr #13
0032ccbc: subseq   r2, sb, r4, asr #11

# _ZN11Application5GetDtEv
0031f66c: ldr      r0, [r0, #0x8c]
0031f670: bx       lr

# _ZN11Application6UpdateEv
0032ccc4: push     {r4, r5, r6, r7, r8, sl, lr}
0032ccc8: ldr      r5, [pc, #0x2d4]
0032cccc: ldr      r6, [pc, #0x2d4]
0032ccd0: ldrb     r2, [r0, #0xa4]
0032ccd4: add      r5, pc, r5
0032ccd8: ldr      r3, [r5, r6]
0032ccdc: sub      sp, sp, #0x2c
0032cce0: cmp      r2, #0
0032cce4: ldr      r3, [r3]
0032cce8: mov      r4, r0
0032ccec: str      r3, [sp, #0x24]
0032ccf0: beq      #0x32cd10
0032ccf4: ldr      r3, [r5, r6]
0032ccf8: ldr      r2, [sp, #0x24]
0032ccfc: ldr      r3, [r3]
0032cd00: cmp      r2, r3
0032cd04: bne      #0x32cfa0
0032cd08: add      sp, sp, #0x2c
0032cd0c: pop      {r4, r5, r6, r7, r8, sl, pc}
0032cd10: bl       #0x7fd794
0032cd14: ldrb     r3, [r0, #5]
0032cd18: cmp      r3, #0
0032cd1c: bne      #0x32cea8
0032cd20: ldrb     r3, [r4, #0x7a]
0032cd24: cmp      r3, #0
0032cd28: movne    r3, #0
0032cd2c: strbne   r3, [r4, #0x7a]
0032cd30: ldr      r3, [r4, #0x10]
0032cd34: ldr      r3, [r3, #0x20]
0032cd38: mov      r0, r3
0032cd3c: ldr      r3, [r3]
0032cd40: mov      lr, pc
0032cd44: ldr      pc, [r3, #0xc]
0032cd48: ldr      r3, [r4, #0x70]
0032cd4c: mov      r7, r0
0032cd50: rsb      r3, r3, r0
0032cd54: cmp      r3, #0x7d0
0032cd58: bhi      #0x32cf00
0032cd5c: ldr      r0, [r4, #0x28]
0032cd60: cmp      r0, #0
0032cd64: beq      #0x32cd80
0032cd68: ldr      r3, [pc, #0x23c]
0032cd6c: add      r3, pc, r3
0032cd70: ldr      r2, [r3, #4]
0032cd74: rsb      r2, r2, r7
0032cd78: cmp      r2, #0xfa0
0032cd7c: bgt      #0x32ce9c
0032cd80: str      r7, [r4, #0x70]
0032cd84: bl       #0x7fd794
0032cd88: ldrb     r3, [r0, #5]
0032cd8c: cmp      r3, #0
0032cd90: bne      #0x32cec0
0032cd94: ldr      r0, [r4, #0x20]
0032cd98: bl       #0x33c568
0032cd9c: mov      r0, r4
0032cda0: bl       #0x320da4
0032cda4: ldr      r0, [r4, #0x8c]
0032cda8: bl       #0x30e2e0
0032cdac: mov      r7, r0
0032cdb0: mov      r1, r0
0032cdb4: mov      r0, #0x44000000
0032cdb8: add      r0, r0, #0x7a0000
0032cdbc: bl       #0x30ec94
0032cdc0: ldr      r3, [pc, #0x1e8]
0032cdc4: mov      r1, r7
0032cdc8: mov      sl, r0
0032cdcc: ldr      r8, [r5, r3]
0032cdd0: add      r7, sp, #0xc
0032cdd4: mov      r0, r8
0032cdd8: bl       #0x31177c
0032cddc: ldr      r1, [r4, #0x8c]
0032cde0: mov      r0, r4
0032cde4: bl       #0x32c438
0032cde8: mov      r0, r4
0032cdec: bl       #0x32ade8
0032cdf0: ldr      r1, [pc, #0x1bc]
0032cdf4: add      r2, sp, #8
0032cdf8: mov      r0, r7
0032cdfc: add      r1, pc, r1
0032ce00: bl       #0x3140ec
0032ce04: mov      ip, #0x42000000
0032ce08: mov      r3, #0
0032ce0c: add      ip, ip, #0x700000
0032ce10: mov      r2, sl
0032ce14: mov      r1, r7
0032ce18: mov      r0, r8
0032ce1c: str      ip, [sp]
0032ce20: bl       #0x312734
0032ce24: mov      r0, r7
0032ce28: bl       #0x3139ac
0032ce2c: bl       #0x7fd794
0032ce30: ldrb     r3, [r0, #5]
0032ce34: cmp      r3, #0
0032ce38: bne      #0x32ceb4
0032ce3c: bl       #0x38174c
0032ce40: cmp      r0, #0
0032ce44: beq      #0x32ccf4
0032ce48: ldr      r3, [pc, #0x168]
0032ce4c: add      r3, pc, r3
0032ce50: ldr      r2, [r3, #8]
0032ce54: ldr      r0, [r3, #0xc]
0032ce58: add      r2, r2, #1
0032ce5c: str      r2, [r3, #8]
0032ce60: ldr      r1, [r4, #0x8c]
0032ce64: cmp      r2, #0xa
0032ce68: add      r2, r0, r1
0032ce6c: str      r2, [r3, #0xc]
0032ce70: beq      #0x32cf10
0032ce74: ldr      r1, [r3, #0x10]
0032ce78: cmp      r1, #0
0032ce7c: ble      #0x32ccf4
0032ce80: ldr      r3, [r4, #0x10]
0032ce84: mov      r2, #0
0032ce88: mov      r0, r3
0032ce8c: ldr      r3, [r3]
0032ce90: mov      lr, pc
0032ce94: ldr      pc, [r3, #0x10]
0032ce98: b        #0x32ccf4
0032ce9c: str      r7, [r3, #4]
0032cea0: bl       #0x339100
0032cea4: b        #0x32cd80
0032cea8: bl       #0x7fd794
0032ceac: bl       #0x7fd914
0032ceb0: b        #0x32cd20
0032ceb4: bl       #0x7fd794
0032ceb8: bl       #0x7fd634
0032cebc: b        #0x32ce3c
0032cec0: ldr      r7, [pc, #0xf4]
0032cec4: add      r7, pc, r7
0032cec8: mov      r0, r7
0032cecc: bl       #0x3136b4
0032ced0: bl       #0x7fd794
0032ced4: mov      r8, r0
0032ced8: ldr      r0, [r4, #0x8c]
0032cedc: bl       #0x30e2e0
0032cee0: mov      r1, r0
0032cee4: mov      r0, r8
0032cee8: bl       #0x824f34
0032ceec: bl       #0x320e98
0032cef0: bl       #0x4a039c
0032cef4: mov      r0, r7
0032cef8: bl       #0x3136b8
0032cefc: b        #0x32cd94
0032cf00: str      r0, [r4, #0x70]
0032cf04: mov      r0, r4
0032cf08: bl       #0x320da4
0032cf0c: b        #0x32ccf4
0032cf10: movw     r1, #0x6667
0032cf14: movt     r1, #0x6666
0032cf18: smull    r0, r1, r1, r2
0032cf1c: ldr      r0, [r3, #0x10]
0032cf20: asr      r2, r2, #0x1f
0032cf24: rsb      r1, r2, r1, asr #2
0032cf28: rsb      r1, r0, r1
0032cf2c: cmp      r1, #0xf
0032cf30: rsble    r1, r1, #0x10
0032cf34: strle    r1, [r3, #0x10]
0032cf38: ble      #0x32cf74
0032cf3c: cmp      r1, #0x20
0032cf40: rsble    r1, r1, #0x21
0032cf44: strle    r1, [r3, #0x10]
0032cf48: ble      #0x32cf74
0032cf4c: cmp      r1, #0x31
0032cf50: rsble    r1, r1, #0x32
0032cf54: strle    r1, [r3, #0x10]
0032cf58: ble      #0x32cf74
0032cf5c: mov      r2, #0
0032cf60: str      r2, [r3, #0xc]
0032cf64: str      r2, [r3, #0x10]
0032cf68: str      r2, [r3, #8]
0032cf6c: mov      r1, #5
0032cf70: b        #0x32cf90
0032cf74: ldr      r3, [pc, #0x44]
0032cf78: mov      r2, #0
0032cf7c: cmp      r1, #4
0032cf80: add      r3, pc, r3
0032cf84: str      r2, [r3, #0xc]
0032cf88: str      r2, [r3, #8]
0032cf8c: ble      #0x32cf6c
0032cf90: ldr      r3, [pc, #0x2c]
0032cf94: add      r3, pc, r3
0032cf98: str      r1, [r3, #0x10]
0032cf9c: b        #0x32ce80
0032cfa0: bl       #0x30e310
0032cfa4: strhteq  r7, [r6], #-0xdc
0032cfa8: andeq    r4, r0, ip, lsr #1
0032cfac: rsbeq    r2, r7, r8, lsl #25
0032cfb0: strdeq   r0, r1, [r0], -ip
0032cfb4: subseq   r2, sb, r4, lsl #10
0032cfb8: rsbeq    r2, r7, r8, lsr #23
0032cfbc: subseq   r2, sb, ip, lsr #8
0032cfc0: rsbeq    r2, r7, r4, ror sl
0032cfc4: rsbeq    r2, r7, r0, ror #20

# _ZN10CharTimers6UpdateEv
003db640: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003db644: ldr      r8, [pc, #0x23c]
003db648: ldr      r3, [pc, #0x23c]
003db64c: ldr      r1, [pc, #0x23c]
003db650: add      r8, pc, r8
003db654: ldr      r2, [r8, r3]
003db658: ldr      r3, [r8, r1]
003db65c: sub      sp, sp, #0x54
003db660: ldrb     sl, [r2, #0x30]
003db664: ldr      r3, [r3]
003db668: str      r1, [sp, #0x10]
003db66c: cmp      sl, #0
003db670: mov      r6, r0
003db674: str      r3, [sp, #0x4c]
003db678: beq      #0x3db69c
003db67c: ldr      r2, [sp, #0x10]
003db680: ldr      r3, [r8, r2]
003db684: ldr      r2, [sp, #0x4c]
003db688: ldr      r3, [r3]
003db68c: cmp      r2, r3
003db690: bne      #0x3db884
003db694: add      sp, sp, #0x54
003db698: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003db69c: ldr      r3, [pc, #0x1f0]
003db6a0: ldr      r0, [r8, r3]
003db6a4: bl       #0x31f66c
003db6a8: str      r0, [sp, #0x14]
003db6ac: ldr      r4, [r6, #8]
003db6b0: ldr      r3, [r6, #0xc]
003db6b4: rsb      r3, r4, r3
003db6b8: asrs     r3, r3, #5
003db6bc: str      r3, [sp, #8]
003db6c0: beq      #0x3db67c
003db6c4: ldr      r2, [pc, #0x1cc]
003db6c8: ldr      r3, [pc, #0x1cc]
003db6cc: mov      sb, r8
003db6d0: str      r2, [sp]
003db6d4: ldr      r2, [pc, #0x1c4]
003db6d8: add      r3, pc, r3
003db6dc: add      r3, r3, #0x13
003db6e0: add      r2, pc, r2
003db6e4: add      r2, r2, #0x13
003db6e8: str      r2, [sp, #4]
003db6ec: str      r3, [sp, #0xc]
003db6f0: b        #0x3db708
003db6f4: ldr      r1, [sp, #8]
003db6f8: add      sl, sl, #1
003db6fc: cmp      sl, r1
003db700: beq      #0x3db87c
003db704: ldr      r4, [r6, #8]
003db708: lsl      r7, sl, #5
003db70c: add      r4, r4, r7
003db710: ldrb     r3, [r4, #0x14]
003db714: cmp      r3, #0
003db718: beq      #0x3db6f4
003db71c: ldrb     r2, [r4, #0x15]
003db720: cmp      r2, #0
003db724: bne      #0x3db6f4
003db728: ldr      r2, [r4, #0x10]
003db72c: ldr      r1, [sp, #0x14]
003db730: add      r5, sp, #0x1c
003db734: add      r8, sp, #0x34
003db738: add      r2, r2, r1
003db73c: str      r2, [r4, #0x10]
003db740: cmp      r3, #0
003db744: beq      #0x3db6f4
003db748: ldr      r2, [r4, #0x10]
003db74c: ldr      r3, [r4, #0xc]
003db750: cmp      r2, r3
003db754: blo      #0x3db6f4
003db758: cmp      r3, #0
003db75c: beq      #0x3db6f4
003db760: ldr      r1, [r4, #8]
003db764: cmp      r1, #0
003db768: strbeq   r1, [r4, #0x14]
003db76c: beq      #0x3db780
003db770: rsb      r3, r3, r2
003db774: subgt    r1, r1, #1
003db778: str      r3, [r4, #0x10]
003db77c: strgt    r1, [r4, #8]
003db780: ldr      r3, [r4, #0x18]
003db784: cmn      r3, #1
003db788: beq      #0x3db7fc
003db78c: ldr      r3, [sp]
003db790: ldr      fp, [sb, r3]
003db794: mov      r0, fp
003db798: bl       #0x337888
003db79c: mov      r0, r5
003db7a0: ldr      r1, [sp, #4]
003db7a4: str      r5, [sp, #0x2c]
003db7a8: str      r5, [sp, #0x30]
003db7ac: bl       #0x3db5f0
003db7b0: mov      r0, fp
003db7b4: mov      r1, r5
003db7b8: bl       #0x337a88
003db7bc: ldr      r0, [sp, #0x30]
003db7c0: cmp      r0, r5
003db7c4: beq      #0x3db7e4
003db7c8: cmp      r0, #0
003db7cc: beq      #0x3db7e4
003db7d0: ldr      r1, [sp, #0x1c]
003db7d4: rsb      r1, r0, r1
003db7d8: cmp      r1, #0x80
003db7dc: bhi      #0x3db86c
003db7e0: bl       #0x708f00
003db7e4: ldmib    r6, {r0, r2}
003db7e8: ldr      r1, [r4, #0x18]
003db7ec: add      r2, r2, r7
003db7f0: bl       #0x3a4d5c
003db7f4: ldrb     r3, [r4, #0x14]
003db7f8: b        #0x3db740
003db7fc: ldr      r2, [sp]
003db800: ldr      fp, [sb, r2]
003db804: mov      r0, fp
003db808: bl       #0x337888
003db80c: mov      r0, r8
003db810: ldr      r1, [sp, #0xc]
003db814: str      r8, [sp, #0x44]
003db818: str      r8, [sp, #0x48]
003db81c: bl       #0x3db5f0
003db820: mov      r0, fp
003db824: mov      r1, r8
003db828: bl       #0x337a88
003db82c: ldr      r0, [sp, #0x48]
003db830: cmp      r0, r8
003db834: beq      #0x3db854
003db838: cmp      r0, #0
003db83c: beq      #0x3db854
003db840: ldr      r1, [sp, #0x34]
003db844: rsb      r1, r0, r1
003db848: cmp      r1, #0x80
003db84c: bhi      #0x3db874
003db850: bl       #0x708f00
003db854: ldmib    r6, {r0, r2}
003db858: mov      r1, #0x29
003db85c: add      r2, r2, r7
003db860: bl       #0x3a4d5c
003db864: ldrb     r3, [r4, #0x14]
003db868: b        #0x3db740
003db86c: bl       #0x310440
003db870: b        #0x3db7e4
003db874: bl       #0x310440
003db878: b        #0x3db854
003db87c: mov      r8, sb
003db880: b        #0x3db67c
003db884: bl       #0x30e310
003db888: subseq   sb, fp, r0, asr #8
003db88c: andeq    r1, r0, r0, lsr #20
003db890: andeq    r4, r0, ip, lsr #1
003db894: strdeq   r3, r4, [r0], -r4
003db898: andeq    r0, r0, r4, lsl #17
003db89c: subeq    sl, lr, r0, ror r2
003db8a0: subeq    sl, lr, r8, ror #4

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

# _ZN6CharAI12RaiseAIEventEiPv
003cbb34: ldr      r3, [pc, #0x6d4]
003cbb38: push     {r4, r5, r6, r7, r8, lr}
003cbb3c: add      r3, pc, r3
003cbb40: mov      r4, r1
003cbb44: mov      r5, r0
003cbb48: mov      r6, r2
003cbb4c: cmp      r1, #0x3f
003cbb50: addls    pc, pc, r1, lsl #2
003cbb54: b        #0x3cbcfc
003cbb58: b        #0x3cbc58
003cbb5c: b        #0x3cbe90
003cbb60: b        #0x3cbe98
003cbb64: b        #0x3cbe2c
003cbb68: b        #0x3cbcfc
003cbb6c: b        #0x3cbcfc
003cbb70: b        #0x3cbcfc
003cbb74: b        #0x3cbcfc
003cbb78: b        #0x3cbcfc
003cbb7c: b        #0x3cbcfc
003cbb80: b        #0x3cbcfc
003cbb84: b        #0x3cbcfc
003cbb88: b        #0x3cbcfc
003cbb8c: b        #0x3cbcfc
003cbb90: b        #0x3cbcfc
003cbb94: b        #0x3cbcfc
003cbb98: b        #0x3cbcfc
003cbb9c: b        #0x3cbcfc
003cbba0: b        #0x3cbcfc
003cbba4: b        #0x3cbcfc
003cbba8: b        #0x3cbcfc
003cbbac: b        #0x3cbcfc
003cbbb0: b        #0x3cbcfc
003cbbb4: b        #0x3cbcfc
003cbbb8: b        #0x3cbcfc
003cbbbc: b        #0x3cbcfc
003cbbc0: b        #0x3cbcfc
003cbbc4: b        #0x3cbcfc
003cbbc8: b        #0x3cbcfc
003cbbcc: b        #0x3cbcfc
003cbbd0: b        #0x3cbcfc
003cbbd4: b        #0x3cbcfc
003cbbd8: b        #0x3cbcfc
003cbbdc: b        #0x3cbcfc
003cbbe0: b        #0x3cbe40
003cbbe4: b        #0x3cbe64
003cbbe8: b        #0x3cbcfc
003cbbec: b        #0x3cbcfc
003cbbf0: b        #0x3cbcfc
003cbbf4: b        #0x3cbcfc
003cbbf8: b        #0x3cbe80
003cbbfc: b        #0x3cbc78
003cbc00: b        #0x3cbcfc
003cbc04: b        #0x3cbcfc
003cbc08: b        #0x3cbcfc
003cbc0c: b        #0x3cbcfc
003cbc10: b        #0x3cbcfc
003cbc14: b        #0x3cbcfc
003cbc18: b        #0x3cbc5c
003cbc1c: b        #0x3cbc8c
003cbc20: b        #0x3cbc98
003cbc24: b        #0x3cbca4
003cbc28: b        #0x3cbcac
003cbc2c: b        #0x3cbcbc
003cbc30: b        #0x3cbcfc
003cbc34: b        #0x3cbcfc
003cbc38: b        #0x3cbcfc
003cbc3c: b        #0x3cbcfc
003cbc40: b        #0x3cbcfc
003cbc44: b        #0x3cbcfc
003cbc48: b        #0x3cbcfc
003cbc4c: b        #0x3cbcfc
003cbc50: b        #0x3cbcfc
003cbc54: b        #0x3cbcf0
003cbc58: movw     r4, #0xc351
003cbc5c: ldr      r0, [r5, #4]
003cbc60: add      r0, r0, #0x4f0
003cbc64: add      r0, r0, #0xc
003cbc68: mov      r1, r4
003cbc6c: mov      r2, r6
003cbc70: pop      {r4, r5, r6, r7, r8, lr}
003cbc74: b        #0x3c5684
003cbc78: ldr      r3, [r0]
003cbc7c: mov      r1, r2
003cbc80: mov      lr, pc
003cbc84: ldr      pc, [r3, #0x80]
003cbc88: b        #0x3cbc5c
003cbc8c: mov      r3, #0
003cbc90: strb     r3, [r0, #0x18]
003cbc94: pop      {r4, r5, r6, r7, r8, pc}
003cbc98: mov      r3, #1
003cbc9c: strb     r3, [r0, #0x4a]
003cbca0: pop      {r4, r5, r6, r7, r8, pc}
003cbca4: bl       #0x3cb77c
003cbca8: pop      {r4, r5, r6, r7, r8, pc}
003cbcac: ldr      r0, [r0, #4]
003cbcb0: add      r0, r0, #0x560
003cbcb4: bl       #0x3df3f0
003cbcb8: pop      {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr      r3, [r0]
003cbcc0: cmp      r2, #0
003cbcc4: mvneq    r1, #0
003cbcc8: ldr      r4, [r3, #0x90]
003cbccc: beq      #0x3cbce4
003cbcd0: mov      r0, r2
003cbcd4: ldr      r3, [r2]
003cbcd8: mov      lr, pc
003cbcdc: ldr      pc, [r3]
003cbce0: mov      r1, r0
003cbce4: mov      r0, r5
003cbce8: blx      r4
003cbcec: pop      {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr      r0, [r0, #4]
003cbcf4: bl       #0x394a3c
003cbcf8: b        #0x3cbc5c
003cbcfc: ldr      r0, [r0, #4]
003cbd00: ldr      r1, [r0, #0x378]
003cbd04: ldrb     r2, [r1, #9]
003cbd08: cmp      r2, #0
003cbd0c: bne      #0x3cbd30
003cbd10: ldr      r2, [pc, #0x4fc]
003cbd14: ldr      r3, [r3, r2]
003cbd18: ldrb     r3, [r3]
003cbd1c: cmp      r3, #0
003cbd20: bne      #0x3cbc5c
003cbd24: ldrb     r3, [r1, #8]
003cbd28: cmp      r3, #0
003cbd2c: bne      #0x3cbc5c
003cbd30: sub      r3, r4, #4
003cbd34: cmp      r3, #0x3a
003cbd38: addls    pc, pc, r3, lsl #2
003cbd3c: b        #0x3cbc60
003cbd40: b        #0x3cc1f8
003cbd44: b        #0x3cbc60
003cbd48: b        #0x3cbc60
003cbd4c: b        #0x3cc1e0
003cbd50: b        #0x3cc1c8
003cbd54: b        #0x3cc1ac
003cbd58: b        #0x3cc198
003cbd5c: b        #0x3cc184
003cbd60: b        #0x3cc170
003cbd64: b        #0x3cc15c
003cbd68: b        #0x3cc148
003cbd6c: b        #0x3cc134
003cbd70: b        #0x3cc120
003cbd74: b        #0x3cc10c
003cbd78: b        #0x3cc0f8
003cbd7c: b        #0x3cc0e4
003cbd80: b        #0x3cc0d0
003cbd84: b        #0x3cc0bc
003cbd88: b        #0x3cc0a8
003cbd8c: b        #0x3cc094
003cbd90: b        #0x3cc080
003cbd94: b        #0x3cc06c
003cbd98: b        #0x3cbc60
003cbd9c: b        #0x3cbc60
003cbda0: b        #0x3cbc60
003cbda4: b        #0x3cc044
003cbda8: b        #0x3cc038
003cbdac: b        #0x3cc02c
003cbdb0: b        #0x3cc020
003cbdb4: b        #0x3cc014
003cbdb8: b        #0x3cbc60
003cbdbc: b        #0x3cbc60
003cbdc0: b        #0x3cc004
003cbdc4: b        #0x3cbff4
003cbdc8: b        #0x3cbfe4
003cbdcc: b        #0x3cbfd4
003cbdd0: b        #0x3cbc60
003cbdd4: b        #0x3cbc60
003cbdd8: b        #0x3cbfbc
003cbddc: b        #0x3cbfa4
003cbde0: b        #0x3cbf8c
003cbde4: b        #0x3cbc60
003cbde8: b        #0x3cbc60
003cbdec: b        #0x3cbc60
003cbdf0: b        #0x3cbc60
003cbdf4: b        #0x3cbc60
003cbdf8: b        #0x3cbc60
003cbdfc: b        #0x3cbc60
003cbe00: b        #0x3cbc60
003cbe04: b        #0x3cbc60
003cbe08: b        #0x3cbc60
003cbe0c: b        #0x3cbf70
003cbe10: b        #0x3cbf54
003cbe14: b        #0x3cbf38
003cbe18: b        #0x3cbf1c
003cbe1c: b        #0x3cbf00
003cbe20: b        #0x3cbee4
003cbe24: b        #0x3cbec8
003cbe28: b        #0x3cbeac
003cbe2c: mov      r1, r2
003cbe30: ldr      r3, [r5]
003cbe34: mov      lr, pc
003cbe38: ldr      pc, [r3, #0x28]
003cbe3c: pop      {r4, r5, r6, r7, r8, pc}
003cbe40: bl       #0x3d3aec
003cbe44: ldr      r3, [r5]
003cbe48: mov      r7, r0
003cbe4c: mov      r0, r5
003cbe50: mov      lr, pc
003cbe54: ldr      pc, [r3, #0x98]
003cbe58: cmp      r7, #0
003cbe5c: bne      #0x3cbc5c
003cbe60: pop      {r4, r5, r6, r7, r8, pc}
003cbe64: bl       #0x3d3ae4
003cbe68: ldr      r3, [r5]
003cbe6c: mov      r7, r0
003cbe70: mov      r0, r5
003cbe74: mov      lr, pc
003cbe78: ldr      pc, [r3, #0x98]
003cbe7c: b        #0x3cbe58
003cbe80: mov      r1, r2
003cbe84: bl       #0x3d4434
003cbe88: mov      r7, r0
003cbe8c: b        #0x3cbe58
003cbe90: movw     r4, #0xc352
003cbe94: b        #0x3cbc5c
003cbe98: ldr      r3, [r0]
003cbe9c: mov      r1, r2
003cbea0: mov      lr, pc
003cbea4: ldr      pc, [r3, #0x24]
003cbea8: b        #0x3cbc5c
003cbeac: mov      r0, r5
003cbeb0: mov      r1, r6
003cbeb4: ldr      r3, [r5]
003cbeb8: mov      r2, #0
003cbebc: mov      lr, pc
003cbec0: ldr      pc, [r3, #0xc8]
003cbec4: pop      {r4, r5, r6, r7, r8, pc}
003cbec8: mov      r0, r5
003cbecc: mov      r1, r6
003cbed0: ldr      r3, [r5]
003cbed4: mov      r2, #1
003cbed8: mov      lr, pc
003cbedc: ldr      pc, [r3, #0xc8]
003cbee0: pop      {r4, r5, r6, r7, r8, pc}
003cbee4: mov      r0, r5
003cbee8: mov      r1, r6
003cbeec: ldr      r3, [r5]
003cbef0: mov      r2, #0
003cbef4: mov      lr, pc
003cbef8: ldr      pc, [r3, #0xc4]
003cbefc: pop      {r4, r5, r6, r7, r8, pc}
003cbf00: mov      r0, r5
003cbf04: mov      r1, r6
003cbf08: ldr      r3, [r5]
003cbf0c: mov      r2, #1
003cbf10: mov      lr, pc
003cbf14: ldr      pc, [r3, #0xc4]
003cbf18: pop      {r4, r5, r6, r7, r8, pc}
003cbf1c: mov      r0, r5
003cbf20: mov      r1, r6
003cbf24: ldr      r3, [r5]
003cbf28: mov      r2, #0
003cbf2c: mov      lr, pc
003cbf30: ldr      pc, [r3, #0xc0]
003cbf34: pop      {r4, r5, r6, r7, r8, pc}
003cbf38: mov      r0, r5
003cbf3c: mov      r1, r6
003cbf40: ldr      r3, [r5]
003cbf44: mov      r2, #1
003cbf48: mov      lr, pc
003cbf4c: ldr      pc, [r3, #0xc0]
003cbf50: pop      {r4, r5, r6, r7, r8, pc}
003cbf54: mov      r0, r5
003cbf58: mov      r1, r6
003cbf5c: ldr      r3, [r5]
003cbf60: mov      r2, #0
003cbf64: mov      lr, pc
003cbf68: ldr      pc, [r3, #0xbc]
003cbf6c: pop      {r4, r5, r6, r7, r8, pc}
003cbf70: mov      r0, r5
003cbf74: mov      r1, r6
003cbf78: ldr      r3, [r5]
003cbf7c: mov      r2, #1
003cbf80: mov      lr, pc
003cbf84: ldr      pc, [r3, #0xbc]
003cbf88: pop      {r4, r5, r6, r7, r8, pc}
003cbf8c: mov      r0, r5
003cbf90: ldr      r3, [r5]
003cbf94: mov      lr, pc
003cbf98: ldr      pc, [r3, #0x88]
003cbf9c: ldr      r0, [r5, #4]
003cbfa0: b        #0x3cbc60
003cbfa4: mov      r0, r5
003cbfa8: ldr      r3, [r5]
003cbfac: mov      lr, pc
003cbfb0: ldr      pc, [r3, #0x84]
003cbfb4: ldr      r0, [r5, #4]
003cbfb8: b        #0x3cbc60
003cbfbc: mov      r0, r5
003cbfc0: ldr      r3, [r5]
003cbfc4: mov      lr, pc
003cbfc8: ldr      pc, [r3, #0x8c]
003cbfcc: ldr      r0, [r5, #4]
003cbfd0: b        #0x3cbc60
003cbfd4: mov      r0, r5
003cbfd8: bl       #0x3d3ff8
003cbfdc: mov      r7, r0
003cbfe0: b        #0x3cbe58
003cbfe4: mov      r0, r5
003cbfe8: bl       #0x3d4204
003cbfec: mov      r7, r0
003cbff0: b        #0x3cbe58
003cbff4: mov      r0, r5
003cbff8: bl       #0x3d3d30
003cbffc: mov      r7, r0
003cc000: b        #0x3cbe58
003cc004: mov      r0, r5
003cc008: bl       #0x3d3d4c
003cc00c: mov      r7, r0
003cc010: b        #0x3cbe58
003cc014: mov      r0, r5
003cc018: pop      {r4, r5, r6, r7, r8, lr}
003cc01c: b        #0x3d8b28
003cc020: mov      r0, r5
003cc024: pop      {r4, r5, r6, r7, r8, lr}
003cc028: b        #0x3d8038
003cc02c: mov      r0, r5
003cc030: pop      {r4, r5, r6, r7, r8, lr}
003cc034: b        #0x3d8b7c
003cc038: mov      r0, r5
003cc03c: pop      {r4, r5, r6, r7, r8, lr}
003cc040: b        #0x3d808c
003cc044: ldr      r3, [r5]
003cc048: add      r0, r0, #0x4f0
003cc04c: add      r0, r0, #0xc
003cc050: ldr      r4, [r3, #0x20]
003cc054: bl       #0x3c01ac
003cc058: mov      r1, r6
003cc05c: mov      r2, r0
003cc060: mov      r0, r5
003cc064: blx      r4
003cc068: pop      {r4, r5, r6, r7, r8, pc}
003cc06c: mov      r0, r5
003cc070: ldr      r3, [r5]
003cc074: mov      lr, pc
003cc078: ldr      pc, [r3, #0x7c]
003cc07c: pop      {r4, r5, r6, r7, r8, pc}
003cc080: mov      r0, r5
003cc084: ldr      r3, [r5]
003cc088: mov      lr, pc
003cc08c: ldr      pc, [r3, #0x78]
003cc090: pop      {r4, r5, r6, r7, r8, pc}
003cc094: mov      r0, r5
003cc098: ldr      r3, [r5]
003cc09c: mov      lr, pc
003cc0a0: ldr      pc, [r3, #0x74]
003cc0a4: pop      {r4, r5, r6, r7, r8, pc}
003cc0a8: mov      r0, r5
003cc0ac: ldr      r3, [r5]
003cc0b0: mov      lr, pc
003cc0b4: ldr      pc, [r3, #0x70]
003cc0b8: pop      {r4, r5, r6, r7, r8, pc}
003cc0bc: mov      r0, r5
003cc0c0: ldr      r3, [r5]
003cc0c4: mov      lr, pc
003cc0c8: ldr      pc, [r3, #0x6c]
003cc0cc: pop      {r4, r5, r6, r7, r8, pc}
003cc0d0: mov      r0, r5
003cc0d4: ldr      r3, [r5]
003cc0d8: mov      lr, pc
003cc0dc: ldr      pc, [r3, #0x68]
003cc0e0: pop      {r4, r5, r6, r7, r8, pc}
003cc0e4: mov      r0, r5
003cc0e8: ldr      r3, [r5]
003cc0ec: mov      lr, pc
003cc0f0: ldr      pc, [r3, #0x64]
003cc0f4: pop      {r4, r5, r6, r7, r8, pc}
003cc0f8: mov      r0, r5
003cc0fc: ldr      r3, [r5]
003cc100: mov      lr, pc
003cc104: ldr      pc, [r3, #0x60]
003cc108: pop      {r4, r5, r6, r7, r8, pc}
003cc10c: mov      r0, r5
003cc110: ldr      r3, [r5]
003cc114: mov      lr, pc
003cc118: ldr      pc, [r3, #0x5c]
003cc11c: pop      {r4, r5, r6, r7, r8, pc}
003cc120: mov      r0, r5
003cc124: ldr      r3, [r5]
003cc128: mov      lr, pc
003cc12c: ldr      pc, [r3, #0x58]
003cc130: pop      {r4, r5, r6, r7, r8, pc}
003cc134: mov      r0, r5
003cc138: ldr      r3, [r5]
003cc13c: mov      lr, pc
003cc140: ldr      pc, [r3, #0x54]
003cc144: pop      {r4, r5, r6, r7, r8, pc}
003cc148: mov      r0, r5
003cc14c: ldr      r3, [r5]
003cc150: mov      lr, pc
003cc154: ldr      pc, [r3, #0x50]
003cc158: pop      {r4, r5, r6, r7, r8, pc}
003cc15c: mov      r0, r5
003cc160: ldr      r3, [r5]
003cc164: mov      lr, pc
003cc168: ldr      pc, [r3, #0x4c]
003cc16c: pop      {r4, r5, r6, r7, r8, pc}
003cc170: mov      r0, r5
003cc174: ldr      r3, [r5]
003cc178: mov      lr, pc
003cc17c: ldr      pc, [r3, #0x48]
003cc180: pop      {r4, r5, r6, r7, r8, pc}
003cc184: mov      r0, r5
003cc188: ldr      r3, [r5]
003cc18c: mov      lr, pc
003cc190: ldr      pc, [r3, #0x44]
003cc194: pop      {r4, r5, r6, r7, r8, pc}
003cc198: mov      r0, r5
003cc19c: ldr      r3, [r5]
003cc1a0: mov      lr, pc
003cc1a4: ldr      pc, [r3, #0x40]
003cc1a8: pop      {r4, r5, r6, r7, r8, pc}
003cc1ac: mov      r0, r5
003cc1b0: ldr      r3, [r5]
003cc1b4: mov      r1, r6
003cc1b8: mov      lr, pc
003cc1bc: ldr      pc, [r3, #0x34]
003cc1c0: ldr      r0, [r5, #4]
003cc1c4: b        #0x3cbc60
003cc1c8: mov      r0, r5
003cc1cc: mov      r1, r6
003cc1d0: ldr      r3, [r5]
003cc1d4: mov      lr, pc
003cc1d8: ldr      pc, [r3, #0x30]
003cc1dc: pop      {r4, r5, r6, r7, r8, pc}
003cc1e0: mov      r0, r5
003cc1e4: mov      r1, r6
003cc1e8: ldr      r3, [r5]
003cc1ec: mov      lr, pc
003cc1f0: ldr      pc, [r3, #0x2c]
003cc1f4: pop      {r4, r5, r6, r7, r8, pc}
003cc1f8: mov      r0, r5
003cc1fc: mov      r1, r6
003cc200: ldr      r3, [r5]
003cc204: mov      lr, pc
003cc208: ldr      pc, [r3, #0xb0]
003cc20c: pop      {r4, r5, r6, r7, r8, pc}
003cc210: subseq   r8, ip, r4, asr pc
003cc214: andeq    r3, r0, r0, asr r6
