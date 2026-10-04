
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

# _ZN12CharAnimator6UpdateEv
003caf3c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003caf40: ldr      r5, [pc, #0x37c]
003caf44: ldr      r6, [pc, #0x37c]
003caf48: mov      r4, r0
003caf4c: add      r5, pc, r5
003caf50: ldr      r3, [r5, r6]
003caf54: ldr      r0, [pc, #0x370]
003caf58: sub      sp, sp, #0x78
003caf5c: ldr      r3, [r3]
003caf60: add      r0, pc, r0
003caf64: str      r3, [sp, #0x74]
003caf68: bl       #0x3136b4
003caf6c: ldr      r3, [r4, #4]
003caf70: ldr      r3, [r3, #0x520]
003caf74: tst      r3, #0x200
003caf78: bne      #0x3cafb8
003caf7c: ldrb     r3, [r4, #0x5c]
003caf80: cmp      r3, #0
003caf84: bne      #0x3cb128
003caf88: ldr      r0, [pc, #0x340]
003caf8c: mov      r3, #0
003caf90: strb     r3, [r4, #0x5c]
003caf94: add      r0, pc, r0
003caf98: bl       #0x3136b8
003caf9c: ldr      r3, [r5, r6]
003cafa0: ldr      r2, [sp, #0x74]
003cafa4: ldr      r3, [r3]
003cafa8: cmp      r2, r3
003cafac: bne      #0x3cb2c0
003cafb0: add      sp, sp, #0x78
003cafb4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003cafb8: ldrb     r3, [r4, #0x5c]
003cafbc: cmp      r3, #0
003cafc0: beq      #0x3cb11c
003cafc4: ldrb     r3, [r4, #0x4a]
003cafc8: mov      r2, #1
003cafcc: strb     r2, [r4, #0x5c]
003cafd0: cmp      r3, #0
003cafd4: movne    r3, #0
003cafd8: strbne   r3, [r4, #0x4a]
003cafdc: strbne   r2, [r4, #0x49]
003cafe0: beq      #0x3cb134
003cafe4: ldr      sl, [r4, #0x2c]
003cafe8: ldr      r3, [pc, #0x2e4]
003cafec: mov      r8, #0xc
003caff0: mla      r8, r8, sl, r4
003caff4: ldr      r3, [r5, r3]
003caff8: ldr      r2, [r8, #8]
003caffc: mov      r7, #0x14
003cb000: ldr      r3, [r3]
003cb004: ldr      r0, [r4, #4]
003cb008: mov      r1, #0x27
003cb00c: mla      r7, r7, r2, r3
003cb010: mov      r2, #0
003cb014: bl       #0x3a4d5c
003cb018: ldr      r3, [r7, #0x10]
003cb01c: cmp      r3, #1
003cb020: bne      #0x3cb144
003cb024: ldr      r3, [r8, #0x10]
003cb028: ldr      r2, [r7, #8]
003cb02c: add      r3, r3, #1
003cb030: cmp      r3, r2
003cb034: beq      #0x3cb144
003cb038: mov      r8, #0xc
003cb03c: mla      r8, r8, sl, r4
003cb040: str      r3, [r8, #0x10]
003cb044: ldr      r2, [r7, #8]
003cb048: cmp      r2, r3
003cb04c: bhi      #0x3cb1e4
003cb050: mov      r2, #0xc
003cb054: mla      r2, r2, sl, r4
003cb058: ldr      r3, [r2, #0xc]
003cb05c: cmp      r3, #0
003cb060: beq      #0x3cb174
003cb064: subgt    r3, r3, #1
003cb068: strgt    r3, [r2, #0xc]
003cb06c: ldr      r3, [pc, #0x264]
003cb070: add      r7, sp, #0x44
003cb074: ldr      r8, [r5, r3]
003cb078: mov      r0, r8
003cb07c: bl       #0x337888
003cb080: ldr      r1, [pc, #0x254]
003cb084: add      r2, sp, #0xc
003cb088: mov      r0, r7
003cb08c: add      r1, pc, r1
003cb090: bl       #0x3140ec
003cb094: mov      r1, r7
003cb098: mov      r0, r8
003cb09c: bl       #0x337a88
003cb0a0: mov      r0, r7
003cb0a4: bl       #0x3139ac
003cb0a8: mov      r2, #0
003cb0ac: ldr      r0, [r4, #4]
003cb0b0: mov      r1, #0x23
003cb0b4: bl       #0x3a4d5c
003cb0b8: ldr      r2, [r4, #0x50]
003cb0bc: mov      r3, #0xc
003cb0c0: mla      r3, r3, sl, r4
003cb0c4: cmn      r2, #1
003cb0c8: ldr      r7, [r3, #0xc]
003cb0cc: beq      #0x3cb2ac
003cb0d0: mov      r3, #0xc
003cb0d4: mla      sl, r3, sl, r4
003cb0d8: cmp      r7, #0
003cb0dc: mvnlt    r3, #0
003cb0e0: strlt    r3, [sl, #0xc]
003cb0e4: strge    r7, [sl, #0xc]
003cb0e8: mov      r3, #0
003cb0ec: strb     r3, [r4, #0x49]
003cb0f0: ldr      r1, [r4, #0x50]
003cb0f4: cmn      r1, #1
003cb0f8: beq      #0x3cb10c
003cb0fc: mov      r0, r4
003cb100: bl       #0x3cacb0
003cb104: mvn      r3, #0
003cb108: str      r3, [r4, #0x50]
003cb10c: ldr      r0, [pc, #0x1cc]
003cb110: add      r0, pc, r0
003cb114: bl       #0x3136b8
003cb118: b        #0x3caf9c
003cb11c: mov      r0, r4
003cb120: bl       #0x3c9168
003cb124: b        #0x3cafc4
003cb128: mov      r0, r4
003cb12c: bl       #0x3c91c8
003cb130: b        #0x3caf88
003cb134: ldrb     r3, [r4, #0x49]
003cb138: cmp      r3, #0
003cb13c: beq      #0x3cb0f0
003cb140: b        #0x3cafe4
003cb144: ldr      r0, [r4, #4]
003cb148: mov      r1, #0x25
003cb14c: mov      r2, #0
003cb150: bl       #0x3a4d5c
003cb154: ldr      r3, [r7, #0x10]
003cb158: cmp      r3, #1
003cb15c: bne      #0x3cb050
003cb160: add      r3, r3, #0xb
003cb164: mla      r3, r3, sl, r4
003cb168: ldr      r3, [r3, #0x10]
003cb16c: add      r3, r3, #1
003cb170: b        #0x3cb038
003cb174: ldr      r3, [r4, #0x2c]
003cb178: cmp      r3, #0
003cb17c: bne      #0x3cb258
003cb180: ldrb     r7, [r4, #0x48]
003cb184: cmp      r7, #0
003cb188: bne      #0x3cb0e8
003cb18c: ldr      r3, [pc, #0x144]
003cb190: add      r8, sp, #0x14
003cb194: ldr      sl, [r5, r3]
003cb198: mov      r0, sl
003cb19c: bl       #0x337888
003cb1a0: ldr      r1, [pc, #0x13c]
003cb1a4: add      r2, sp, #4
003cb1a8: mov      r0, r8
003cb1ac: add      r1, pc, r1
003cb1b0: bl       #0x3140ec
003cb1b4: mov      r1, r8
003cb1b8: mov      r0, sl
003cb1bc: bl       #0x337a88
003cb1c0: mov      r0, r8
003cb1c4: bl       #0x3139ac
003cb1c8: mov      r3, #1
003cb1cc: strb     r3, [r4, #0x48]
003cb1d0: mov      r2, r7
003cb1d4: ldr      r0, [r4, #4]
003cb1d8: mov      r1, #0x22
003cb1dc: bl       #0x3a4d5c
003cb1e0: b        #0x3cb0e8
003cb1e4: ldr      r3, [pc, #0xec]
003cb1e8: add      sl, sp, #0x5c
003cb1ec: ldr      sb, [r5, r3]
003cb1f0: mov      r0, sb
003cb1f4: bl       #0x337888
003cb1f8: ldr      r1, [pc, #0xe8]
003cb1fc: add      r2, sp, #0x10
003cb200: mov      r0, sl
003cb204: add      r1, pc, r1
003cb208: bl       #0x3140ec
003cb20c: mov      r1, sl
003cb210: mov      r0, sb
003cb214: bl       #0x337a88
003cb218: mov      r0, sl
003cb21c: bl       #0x3139ac
003cb220: mov      r1, #0x23
003cb224: ldr      r0, [r4, #4]
003cb228: mov      r2, #0
003cb22c: bl       #0x3a4d5c
003cb230: ldr      r1, [r8, #0x10]
003cb234: ldr      r3, [r7, #8]
003cb238: cmp      r1, r3
003cb23c: bhs      #0x3cb24c
003cb240: mov      r0, r4
003cb244: bl       #0x3ca79c
003cb248: b        #0x3cb0e8
003cb24c: mov      r0, r4
003cb250: bl       #0x3caf3c
003cb254: b        #0x3cb0e8
003cb258: ldr      r3, [pc, #0x78]
003cb25c: add      r7, sp, #0x2c
003cb260: ldr      r8, [r5, r3]
003cb264: mov      r0, r8
003cb268: bl       #0x337888
003cb26c: ldr      r1, [pc, #0x78]
003cb270: add      r2, sp, #8
003cb274: mov      r0, r7
003cb278: add      r1, pc, r1
003cb27c: bl       #0x3140ec
003cb280: mov      r1, r7
003cb284: mov      r0, r8
003cb288: bl       #0x337a88
003cb28c: mov      r0, r7
003cb290: bl       #0x3139ac
003cb294: ldr      r3, [r4, #0x2c]
003cb298: mov      r0, r4
003cb29c: sub      r3, r3, #1
003cb2a0: str      r3, [r4, #0x2c]
003cb2a4: bl       #0x3caf3c
003cb2a8: b        #0x3cb0e8
003cb2ac: ldr      r1, [r3, #8]
003cb2b0: mov      r0, r4
003cb2b4: ldr      r2, [r4, #0x2c]
003cb2b8: bl       #0x3cab38
003cb2bc: b        #0x3cb0d0
003cb2c0: bl       #0x30e310
003cb2c4: subseq   sb, ip, r4, asr #22
003cb2c8: andeq    r4, r0, ip, lsr #1
003cb2cc: subeq    sl, pc, r0, asr #4
003cb2d0: subeq    sl, pc, ip, lsl #4
003cb2d4: andeq    r3, r0, ip, ror ip
003cb2d8: andeq    r0, r0, r4, lsl #17
003cb2dc: subeq    sb, pc, r4, asr #31
003cb2e0: umaaleq  sl, pc, r0, r0
003cb2e4: subeq    sb, pc, r4, lsr #29
003cb2e8: subeq    sb, pc, ip, asr #28
003cb2ec: ldrdeq   sb, sl, [pc], #-0xd8

# _ZN6CSIdle8OnUpdateEiP9CharacterP16CharStateMachine
003c0e80: mov      r0, r1
003c0e84: mov      r1, r2
003c0e88: mov      r2, r3
003c0e8c: b        #0x3c0b78

# _ZN11Application5GetDtEv
0031f66c: ldr      r0, [r0, #0x8c]
0031f670: bx       lr

# _Z20PushProfilingContextPKc
003136b4: bx       lr

# _ZN16CharStateMachine6UpdateEv
003c628c: push     {r4, r5, r6, lr}
003c6290: mov      r4, r0
003c6294: ldr      r0, [pc, #0xe8]
003c6298: sub      sp, sp, #8
003c629c: ldr      r5, [pc, #0xe4]
003c62a0: add      r0, pc, r0
003c62a4: bl       #0x3136b4
003c62a8: ldr      r3, [pc, #0xdc]
003c62ac: add      r5, pc, r5
003c62b0: ldr      r6, [r4, #0x60]
003c62b4: ldr      r0, [r5, r3]
003c62b8: bl       #0x31f66c
003c62bc: ldr      r3, [r4, #0x2c]
003c62c0: add      r0, r0, r6
003c62c4: str      r0, [r4, #0x60]
003c62c8: tst      r3, #2
003c62cc: bne      #0x3c6314
003c62d0: tst      r3, #4
003c62d4: bne      #0x3c6350
003c62d8: ldr      r3, [r4, #0x20]
003c62dc: cmp      r3, #0
003c62e0: beq      #0x3c6300
003c62e4: ldm      r3, {r1, r2}
003c62e8: mov      r3, r4
003c62ec: mov      r0, r2
003c62f0: ldr      ip, [r2]
003c62f4: ldr      r2, [r4, #4]
003c62f8: mov      lr, pc
003c62fc: ldr      pc, [ip, #0x14]
003c6300: ldr      r0, [pc, #0x88]
003c6304: add      r0, pc, r0
003c6308: add      sp, sp, #8
003c630c: pop      {r4, r5, r6, lr}
003c6310: b        #0x3136b8
003c6314: mov      r0, r4
003c6318: mov      r1, #0
003c631c: bl       #0x3c0378
003c6320: subs     ip, r0, #0
003c6324: bne      #0x3c6344
003c6328: ldr      r2, [r4, #0x24]
003c632c: mov      r3, ip
003c6330: mov      r0, r4
003c6334: ubfx     r2, r2, #0xb, #1
003c6338: mvn      r1, #0
003c633c: str      ip, [sp]
003c6340: bl       #0x3c5ffc
003c6344: ldr      r3, [r4, #0x2c]
003c6348: tst      r3, #4
003c634c: beq      #0x3c62d8
003c6350: mov      r0, r4
003c6354: mov      r1, #0
003c6358: bl       #0x3c034c
003c635c: subs     ip, r0, #0
003c6360: bne      #0x3c62d8
003c6364: ldr      r2, [r4, #0x24]
003c6368: mov      r3, ip
003c636c: mov      r0, r4
003c6370: ubfx     r2, r2, #0xa, #1
003c6374: mvn      r1, #0
003c6378: str      ip, [sp]
003c637c: bl       #0x3c6144
003c6380: b        #0x3c62d8
003c6384: subeq    lr, pc, r0, lsr #25
003c6388: subseq   lr, ip, r4, ror #15
003c638c: strdeq   r3, r4, [r0], -r4
003c6390: subeq    lr, pc, ip, lsr ip

# _Z19PopProfilingContextPKc
003136b8: bx       lr

# _ZN10GameObject6UpdateEv
0038cbe8: push     {r4, r5, r6, r7, r8, lr}
0038cbec: ldr      r5, [pc, #0x138]
0038cbf0: ldr      r7, [pc, #0x138]
0038cbf4: mov      r4, r0
0038cbf8: add      r5, pc, r5
0038cbfc: ldr      r3, [r5, r7]
0038cc00: ldr      r0, [pc, #0x12c]
0038cc04: sub      sp, sp, #0x20
0038cc08: ldr      r3, [r3]
0038cc0c: add      r0, pc, r0
0038cc10: add      r6, sp, #4
0038cc14: str      r3, [sp, #0x1c]
0038cc18: bl       #0x3136b4
0038cc1c: ldr      r3, [pc, #0x114]
0038cc20: ldr      r8, [r5, r3]
0038cc24: mov      r0, r8
0038cc28: bl       #0x337888
0038cc2c: ldr      r1, [pc, #0x108]
0038cc30: mov      r2, sp
0038cc34: mov      r0, r6
0038cc38: add      r1, pc, r1
0038cc3c: bl       #0x3140ec
0038cc40: mov      r1, r6
0038cc44: mov      r0, r8
0038cc48: bl       #0x337a88
0038cc4c: mov      r0, r6
0038cc50: bl       #0x318254
0038cc54: ldr      r3, [pc, #0xe4]
0038cc58: ldr      r3, [r5, r3]
0038cc5c: ldr      r3, [r3, #0x38]
0038cc60: ldr      r2, [r3, #0x58]
0038cc64: add      r2, r2, #1
0038cc68: str      r2, [r3, #0x58]
0038cc6c: ldr      r1, [r4, #0x2e4]
0038cc70: cmp      r1, #0
0038cc74: beq      #0x38cc90
0038cc78: ldr      r3, [r4]
0038cc7c: mov      r0, r4
0038cc80: mov      lr, pc
0038cc84: ldr      pc, [r3, #0x98]
0038cc88: mov      r3, #0
0038cc8c: str      r3, [r4, #0x2e4]
0038cc90: ldr      r3, [r4, #0x174]
0038cc94: ldr      lr, [r4, #0x160]
0038cc98: ldr      ip, [r4, #0x164]
0038cc9c: ldr      r1, [r4, #0x16c]
0038cca0: ldr      r2, [r4, #0x170]
0038cca4: ldr      r0, [r4, #0x168]
0038cca8: str      r3, [r4, #0x1a4]
0038ccac: str      lr, [r4, #0x190]
0038ccb0: str      ip, [r4, #0x194]
0038ccb4: str      r1, [r4, #0x19c]
0038ccb8: str      r2, [r4, #0x1a0]
0038ccbc: str      r0, [r4, #0x198]
0038ccc0: mov      r0, r4
0038ccc4: bl       #0x3940c0
0038ccc8: mov      r0, r4
0038cccc: bl       #0x393710
0038ccd0: mov      r0, r4
0038ccd4: bl       #0x3943cc
0038ccd8: mov      r0, r4
0038ccdc: bl       #0x393d74
0038cce0: mov      r0, r4
0038cce4: bl       #0x38b8b8
0038cce8: mov      r3, #0x370
0038ccec: ldrsh    r3, [r4, r3]
0038ccf0: cmp      r3, #0
0038ccf4: blt      #0x38cd00
0038ccf8: mov      r0, r4
0038ccfc: bl       #0x38ae2c
0038cd00: ldr      r0, [pc, #0x3c]
0038cd04: add      r0, pc, r0
0038cd08: bl       #0x3136b8
0038cd0c: ldr      r3, [r5, r7]
0038cd10: ldr      r2, [sp, #0x1c]
0038cd14: ldr      r3, [r3]
0038cd18: cmp      r2, r3
0038cd1c: bne      #0x38cd28
0038cd20: add      sp, sp, #0x20
0038cd24: pop      {r4, r5, r6, r7, r8, pc}
0038cd28: bl       #0x30e310
0038cd2c: mlseq    r0, r8, lr, r7
0038cd30: andeq    r4, r0, ip, lsr #1
0038cd34: subseq   r5, r3, r4, asr r8
0038cd38: andeq    r0, r0, r4, lsl #17
0038cd3c: subseq   r3, r3, r0, ror #14
0038cd40: strdeq   r3, r4, [r0], -r4
0038cd44: subseq   r5, r3, ip, asr r7

# _ZN6CSIdle16IdleCommonUpdateEiP9CharacterP16CharStateMachine
003c0b78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c0b7c: ldrb     r3, [r1, #0x1b5]
003c0b80: ldr      r5, [pc, #0x2e0]
003c0b84: sub      sp, sp, #0x4c
003c0b88: cmp      r3, #0
003c0b8c: mov      r4, r1
003c0b90: add      r5, pc, r5
003c0b94: bne      #0x3c0bb8
003c0b98: ldr      r3, [r1]
003c0b9c: mov      r0, r1
003c0ba0: mov      lr, pc
003c0ba4: ldr      pc, [r3, #0x28]
003c0ba8: cmp      r0, #0
003c0bac: bne      #0x3c0bcc
003c0bb0: add      sp, sp, #0x4c
003c0bb4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c0bb8: mov      r1, #0
003c0bbc: mov      r0, r4
003c0bc0: mov      r2, r1
003c0bc4: bl       #0x3a4d5c
003c0bc8: b        #0x3c0bb0
003c0bcc: bl       #0x7fd794
003c0bd0: ldrb     r6, [r0, #5]
003c0bd4: cmp      r6, #0
003c0bd8: bne      #0x3c0bb0
003c0bdc: ldr      r2, [pc, #0x288]
003c0be0: ldr      r1, [pc, #0x288]
003c0be4: ldr      r7, [r5, r2]
003c0be8: str      r2, [sp, #0x14]
003c0bec: ldr      r2, [pc, #0x280]
003c0bf0: add      r1, pc, r1
003c0bf4: ldr      r0, [r7, #0x2c]
003c0bf8: add      r2, pc, r2
003c0bfc: bl       #0x4c4bdc
003c0c00: ldr      r3, [r4, #0x55c]
003c0c04: mov      sb, r0
003c0c08: cmp      r0, r3
003c0c0c: bhi      #0x3c0bb0
003c0c10: ldr      r8, [r7, #0x40]
003c0c14: mov      r0, r8
003c0c18: bl       #0x36d7a8
003c0c1c: subs     sl, r0, #0
003c0c20: ble      #0x3c0bb0
003c0c24: ldr      r3, [pc, #0x24c]
003c0c28: add      r2, sp, #0x30
003c0c2c: str      r2, [sp, #0x24]
003c0c30: add      r3, pc, r3
003c0c34: str      r3, [sp, #0x28]
003c0c38: ldr      r3, [pc, #0x23c]
003c0c3c: mov      r7, r5
003c0c40: add      r3, pc, r3
003c0c44: str      r3, [sp, #0x2c]
003c0c48: add      r3, sp, #0x3c
003c0c4c: str      r3, [sp, #0x20]
003c0c50: b        #0x3c0c60
003c0c54: add      r6, r6, #1
003c0c58: cmp      r6, sl
003c0c5c: beq      #0x3c0bb0
003c0c60: mov      r0, r8
003c0c64: mov      r1, r6
003c0c68: mov      r2, #0
003c0c6c: bl       #0x36e744
003c0c70: ldr      r5, [r0, #0x660]
003c0c74: cmp      r5, #0
003c0c78: cmpne    r4, r5
003c0c7c: beq      #0x3c0c54
003c0c80: add      r0, r5, #0x4f0
003c0c84: add      r0, r0, #0xc
003c0c88: bl       #0x3c01ac
003c0c8c: cmp      r0, #3
003c0c90: bne      #0x3c0c54
003c0c94: ldr      r3, [r5, #0x55c]
003c0c98: cmp      sb, r3
003c0c9c: bhi      #0x3c0c54
003c0ca0: ldr      r2, [sp, #0x14]
003c0ca4: ldr      r1, [sp, #0x28]
003c0ca8: ldr      r3, [r7, r2]
003c0cac: ldr      r2, [sp, #0x2c]
003c0cb0: ldr      r0, [r3, #0x2c]
003c0cb4: bl       #0x4c4bdc
003c0cb8: bl       #0x30e964
003c0cbc: str      r0, [sp, #0xc]
003c0cc0: ldr      r1, [r5, #0x160]
003c0cc4: ldr      r0, [r4, #0x160]
003c0cc8: bl       #0x30e3ac
003c0ccc: str      r0, [sp, #0x10]
003c0cd0: ldr      r1, [r5, #0x164]
003c0cd4: ldr      r0, [r4, #0x164]
003c0cd8: bl       #0x30e3ac
003c0cdc: str      r0, [sp, #0x18]
003c0ce0: ldr      r1, [r5, #0x168]
003c0ce4: ldr      r0, [r4, #0x168]
003c0ce8: bl       #0x30e3ac
003c0cec: str      r0, [sp, #0x1c]
003c0cf0: ldr      r0, [sp, #0x10]
003c0cf4: mov      r1, r0
003c0cf8: bl       #0x30ed6c
003c0cfc: mov      fp, r0
003c0d00: ldr      r0, [sp, #0x18]
003c0d04: mov      r1, r0
003c0d08: bl       #0x30ed6c
003c0d0c: mov      r1, r0
003c0d10: mov      r0, fp
003c0d14: bl       #0x30eba4
003c0d18: mov      fp, r0
003c0d1c: ldr      r0, [sp, #0x1c]
003c0d20: mov      r1, r0
003c0d24: bl       #0x30ed6c
003c0d28: mov      r1, r0
003c0d2c: mov      r0, fp
003c0d30: bl       #0x30eba4
003c0d34: mov      fp, r0
003c0d38: ldr      r0, [sp, #0xc]
003c0d3c: mov      r1, r0
003c0d40: bl       #0x30ed6c
003c0d44: mov      r1, fp
003c0d48: bl       #0x30e2f8
003c0d4c: cmp      r0, #0
003c0d50: beq      #0x3c0c54
003c0d54: mov      r0, fp
003c0d58: mov      r1, #0
003c0d5c: bl       #0x30e2f8
003c0d60: cmp      r0, #0
003c0d64: beq      #0x3c0c54
003c0d68: mov      r0, fp
003c0d6c: bl       #0x30e124
003c0d70: mov      fp, r0
003c0d74: mov      r1, fp
003c0d78: ldr      r0, [sp, #0xc]
003c0d7c: bl       #0x30e3ac
003c0d80: mov      r1, #0x3f400000
003c0d84: bl       #0x30ed6c
003c0d88: mov      r1, fp
003c0d8c: bl       #0x30ec94
003c0d90: ldr      r1, [sp, #0x10]
003c0d94: mov      fp, r0
003c0d98: bl       #0x30ed6c
003c0d9c: ldr      r1, [sp, #0x18]
003c0da0: str      r0, [sp, #0xc]
003c0da4: mov      r0, fp
003c0da8: bl       #0x30ed6c
003c0dac: ldr      r1, [sp, #0x1c]
003c0db0: str      r0, [sp, #0x10]
003c0db4: mov      r0, fp
003c0db8: bl       #0x30ed6c
003c0dbc: ldr      r1, [r4, #0x164]
003c0dc0: mov      fp, r0
003c0dc4: ldr      r0, [sp, #0x10]
003c0dc8: bl       #0x30eba4
003c0dcc: ldr      r1, [r4, #0x168]
003c0dd0: mov      r2, r0
003c0dd4: mov      r0, fp
003c0dd8: str      r2, [sp, #4]
003c0ddc: bl       #0x30eba4
003c0de0: ldr      r1, [r4, #0x160]
003c0de4: mov      ip, r0
003c0de8: ldr      r0, [sp, #0xc]
003c0dec: str      ip, [sp, #8]
003c0df0: bl       #0x30eba4
003c0df4: ldr      r3, [r4, #0x378]
003c0df8: ldmib    sp, {r2, ip}
003c0dfc: str      r0, [sp, #0x3c]
003c0e00: ldr      r1, [sp, #0x20]
003c0e04: mov      r0, r3
003c0e08: str      r2, [sp, #0x40]
003c0e0c: str      ip, [sp, #0x44]
003c0e10: bl       #0x4054e4
003c0e14: ldr      r0, [r5, #0x164]
003c0e18: ldr      r1, [sp, #0x10]
003c0e1c: bl       #0x30e3ac
003c0e20: mov      r1, fp
003c0e24: mov      r3, r0
003c0e28: ldr      r0, [r5, #0x168]
003c0e2c: str      r3, [sp, #8]
003c0e30: bl       #0x30e3ac
003c0e34: ldr      r1, [sp, #0xc]
003c0e38: mov      fp, r0
003c0e3c: ldr      r0, [r5, #0x160]
003c0e40: bl       #0x30e3ac
003c0e44: ldr      r2, [r5, #0x378]
003c0e48: ldr      r3, [sp, #8]
003c0e4c: str      r0, [sp, #0x30]
003c0e50: ldr      r1, [sp, #0x24]
003c0e54: mov      r0, r2
003c0e58: str      r3, [sp, #0x34]
003c0e5c: str      fp, [sp, #0x38]
003c0e60: bl       #0x4054e4
003c0e64: b        #0x3c0c54
003c0e68: subseq   r3, sp, r0, lsl #30
003c0e6c: strdeq   r3, r4, [r0], -r4
003c0e70: subseq   r0, r0, r0, ror #22
003c0e74: subseq   r3, r0, r0, lsl #31
003c0e78: subseq   r0, r0, r0, lsr #22
003c0e7c: subseq   r3, r0, r8, asr pc

# _ZN13PhysicalWorld6updateEv
0034bd08: push     {r4, r5, r6, lr}
0034bd0c: ldr      r4, [pc, #0x50]
0034bd10: mov      r6, r0
0034bd14: ldr      r5, [pc, #0x4c]
0034bd18: add      r4, pc, r4
0034bd1c: mov      r0, r4
0034bd20: bl       #0x3136b4
0034bd24: ldr      r3, [pc, #0x40]
0034bd28: add      r5, pc, r5
0034bd2c: ldr      r0, [r5, r3]
0034bd30: bl       #0x31f66c
0034bd34: bl       #0x30e2e0
0034bd38: movw     r1, #0x126f
0034bd3c: movt     r1, #0x3a83
0034bd40: bl       #0x30ed6c
0034bd44: ldr      r5, [r6, #0x10]
0034bd48: mov      r1, r0
0034bd4c: mov      r2, #0xa
0034bd50: mov      r0, r5
0034bd54: bl       #0x7e8b1c
0034bd58: mov      r0, r4
0034bd5c: pop      {r4, r5, r6, lr}
0034bd60: b        #0x3136b8
0034bd64: subseq   r4, r7, r0, lsl #18
0034bd68: rsbeq    r8, r4, r8, ror #26
0034bd6c: strdeq   r3, r4, [r0], -r4
