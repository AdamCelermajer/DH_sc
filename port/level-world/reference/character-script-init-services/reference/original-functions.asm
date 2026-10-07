
# _ZN9Character9_InitHpMpEv
003b3a70: push     {r4, lr}
003b3a74: mvn      r1, #0
003b3a78: mov      r4, r0
003b3a7c: bl       #0x3bdca4
003b3a80: mov      r0, r4
003b3a84: mvn      r1, #0
003b3a88: pop      {r4, lr}
003b3a8c: b        #0x3bdbb8

# _ZN6CharAI15UpdateAllSkillsEv
003d8894: push     {r4, r5, r6, lr}
003d8898: mov      r5, r0
003d889c: ldr      r0, [r0, #4]
003d88a0: add      r0, r0, #0x4f0
003d88a4: add      r0, r0, #0xc
003d88a8: bl       #0x3c02e8
003d88ac: cmp      r0, #0
003d88b0: beq      #0x3d88b8
003d88b4: pop      {r4, r5, r6, pc}
003d88b8: ldr      r0, [r5, #4]
003d88bc: add      r0, r0, #0x4f0
003d88c0: add      r0, r0, #0xc
003d88c4: bl       #0x3c0334
003d88c8: subs     r4, r0, #0
003d88cc: bne      #0x3d88b4
003d88d0: ldr      r3, [r5, #0xb4]
003d88d4: ldr      r6, [r5, #0xb8]
003d88d8: rsb      r6, r3, r6
003d88dc: asrs     r6, r6, #2
003d88e0: bne      #0x3d88ec
003d88e4: b        #0x3d8908
003d88e8: ldr      r3, [r5, #0xb4]
003d88ec: ldr      r0, [r3, r4, lsl #2]
003d88f0: add      r4, r4, #1
003d88f4: cmp      r0, #0
003d88f8: beq      #0x3d8900
003d88fc: bl       #0x3dabd0
003d8900: cmp      r4, r6
003d8904: bne      #0x3d88e8
003d8908: ldr      r3, [r5, #0xc0]
003d890c: ldr      r6, [r5, #0xc4]
003d8910: rsb      r6, r3, r6
003d8914: asrs     r6, r6, #2
003d8918: beq      #0x3d88b4
003d891c: mov      r4, #0
003d8920: b        #0x3d8928
003d8924: ldr      r3, [r5, #0xc0]
003d8928: ldr      r0, [r3, r4, lsl #2]
003d892c: add      r4, r4, #1
003d8930: cmp      r0, #0
003d8934: beq      #0x3d893c
003d8938: bl       #0x3dabd0
003d893c: cmp      r4, r6
003d8940: bne      #0x3d8924
003d8944: pop      {r4, r5, r6, pc}

# _ZN6CharAI18SetSkillsAndSpellsEv
003ce044: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ce048: ldr      r6, [pc, #0x714]
003ce04c: ldr      r2, [pc, #0x714]
003ce050: sub      sp, sp, #0xa4
003ce054: add      r6, pc, r6
003ce058: str      r2, [sp, #0x14]
003ce05c: ldr      r2, [r6, r2]
003ce060: ldr      r3, [r0, #0x1c]
003ce064: mov      r4, r0
003ce068: ldr      r2, [r2]
003ce06c: cmp      r3, #0
003ce070: str      r2, [sp, #0x9c]
003ce074: beq      #0x3ce6ac
003ce078: ldr      fp, [pc, #0x6ec]
003ce07c: add      r5, sp, #0x84
003ce080: ldr      r7, [r6, fp]
003ce084: mov      r0, r7
003ce088: bl       #0x337888
003ce08c: ldr      r1, [pc, #0x6dc]
003ce090: add      r2, sp, #0x50
003ce094: mov      r0, r5
003ce098: add      r1, pc, r1
003ce09c: bl       #0x3140ec
003ce0a0: mov      r0, r7
003ce0a4: mov      r1, r5
003ce0a8: bl       #0x337a88
003ce0ac: ldr      r0, [sp, #0x98]
003ce0b0: cmp      r0, r5
003ce0b4: beq      #0x3ce0d4
003ce0b8: cmp      r0, #0
003ce0bc: beq      #0x3ce0d4
003ce0c0: ldr      r1, [sp, #0x84]
003ce0c4: rsb      r1, r0, r1
003ce0c8: cmp      r1, #0x80
003ce0cc: bhi      #0x3ce694
003ce0d0: bl       #0x708f00
003ce0d4: ldr      r3, [r4, #0x1c]
003ce0d8: add      r8, sp, #0x6c
003ce0dc: str      r8, [sp, #0x7c]
003ce0e0: str      r8, [sp, #0x80]
003ce0e4: ldr      r2, [r3, #0x78]
003ce0e8: ldr      r1, [r3, #0x7c]
003ce0ec: mov      r0, r8
003ce0f0: bl       #0x3116e8
003ce0f4: ldr      r1, [pc, #0x678]
003ce0f8: ldr      r0, [r4, #0x1c]
003ce0fc: add      r1, pc, r1
003ce100: add      r0, r0, #0x68
003ce104: add      r2, r1, #0x14
003ce108: bl       #0x3109e0
003ce10c: ldr      r5, [r4, #0xb8]
003ce110: ldr      r3, [r4, #0xb4]
003ce114: rsb      r5, r3, r5
003ce118: asrs     r5, r5, #2
003ce11c: beq      #0x3ce448
003ce120: ldr      r5, [r4, #0xc4]
003ce124: ldr      r3, [r4, #0xc0]
003ce128: rsb      r5, r3, r5
003ce12c: asrs     r5, r5, #2
003ce130: beq      #0x3ce204
003ce134: ldr      r0, [r4, #0x1c]
003ce138: add      r0, r0, #0x68
003ce13c: cmp      r0, r8
003ce140: beq      #0x3ce150
003ce144: ldr      r1, [sp, #0x80]
003ce148: ldr      r2, [sp, #0x7c]
003ce14c: bl       #0x3109e0
003ce150: ldr      r7, [r6, fp]
003ce154: add      r5, sp, #0x54
003ce158: mov      r0, r7
003ce15c: bl       #0x337888
003ce160: ldr      r1, [pc, #0x610]
003ce164: add      r2, sp, #0x4c
003ce168: mov      r0, r5
003ce16c: add      r1, pc, r1
003ce170: bl       #0x3140ec
003ce174: mov      r0, r7
003ce178: mov      r1, r5
003ce17c: bl       #0x337a88
003ce180: ldr      r0, [sp, #0x68]
003ce184: cmp      r0, r5
003ce188: beq      #0x3ce1a8
003ce18c: cmp      r0, #0
003ce190: beq      #0x3ce1a8
003ce194: ldr      r1, [sp, #0x54]
003ce198: rsb      r1, r0, r1
003ce19c: cmp      r1, #0x80
003ce1a0: bhi      #0x3ce69c
003ce1a4: bl       #0x708f00
003ce1a8: ldr      r3, [r4, #0x1c]
003ce1ac: mov      r0, r3
003ce1b0: ldr      r3, [r3]
003ce1b4: mov      lr, pc
003ce1b8: ldr      pc, [r3, #0xcc]
003ce1bc: ldr      r0, [sp, #0x80]
003ce1c0: cmp      r0, r8
003ce1c4: beq      #0x3ce1e4
003ce1c8: cmp      r0, #0
003ce1cc: beq      #0x3ce1e4
003ce1d0: ldr      r1, [sp, #0x6c]
003ce1d4: rsb      r1, r0, r1
003ce1d8: cmp      r1, #0x80
003ce1dc: bhi      #0x3ce6a4
003ce1e0: bl       #0x708f00
003ce1e4: ldr      r2, [sp, #0x14]
003ce1e8: ldr      r3, [r6, r2]
003ce1ec: ldr      r2, [sp, #0x9c]
003ce1f0: ldr      r3, [r3]
003ce1f4: cmp      r2, r3
003ce1f8: bne      #0x3ce760
003ce1fc: add      sp, sp, #0xa4
003ce200: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ce204: add      r2, r4, #0xc0
003ce208: ldr      r0, [r4, #4]
003ce20c: str      r2, [sp, #0x18]
003ce210: bl       #0x3ae5dc
003ce214: add      sb, sp, #0x2c
003ce218: ldr      r1, [r0, #4]
003ce21c: mov      r7, r0
003ce220: ldr      r0, [sp, #0x18]
003ce224: bl       #0x3cd584
003ce228: mov      r0, sb
003ce22c: bl       #0x3192b4
003ce230: ldr      r1, [pc, #0x544]
003ce234: mov      r0, sb
003ce238: add      r1, pc, r1
003ce23c: bl       #0x39ec10
003ce240: mov      r0, sb
003ce244: mvn      r1, #0
003ce248: bl       #0x3cdd78
003ce24c: ldr      r3, [r7, #4]
003ce250: cmp      r3, #0
003ce254: beq      #0x3ce414
003ce258: ldr      r3, [pc, #0x520]
003ce25c: str      r6, [sp, #0x24]
003ce260: mov      sl, r8
003ce264: str      r3, [sp, #0x20]
003ce268: ldr      r3, [pc, #0x514]
003ce26c: add      r3, pc, r3
003ce270: str      r3, [sp, #8]
003ce274: ldr      r3, [pc, #0x50c]
003ce278: add      r3, pc, r3
003ce27c: str      r3, [sp, #0xc]
003ce280: ldr      r3, [pc, #0x504]
003ce284: add      r3, pc, r3
003ce288: str      r3, [sp, #0x10]
003ce28c: ldr      r3, [pc, #0x4fc]
003ce290: add      r3, pc, r3
003ce294: str      r3, [sp, #0x1c]
003ce298: b        #0x3ce3bc
003ce29c: ldr      r0, [r4, #0x1c]
003ce2a0: ldr      r1, [sp, #8]
003ce2a4: bl       #0x37b574
003ce2a8: ldr      r6, [sp, #0x30]
003ce2ac: ldm      r6, {r0, r3}
003ce2b0: rsb      r3, r0, r3
003ce2b4: asr      r3, r3, #4
003ce2b8: add      r2, r3, r3, lsl #3
003ce2bc: add      r2, r2, r2, lsl #6
003ce2c0: add      r2, r3, r2, lsl #3
003ce2c4: add      r2, r2, r2, lsl #15
003ce2c8: add      r2, r3, r2, lsl #3
003ce2cc: cmp      r2, #0
003ce2d0: bne      #0x3ce2e4
003ce2d4: ldr      r2, [sp, #0x20]
003ce2d8: add      r0, pc, r2
003ce2dc: bl       #0x708eb0
003ce2e0: ldr      r0, [r6]
003ce2e4: ldr      r1, [r8, #0x18]
003ce2e8: bl       #0x31c46c
003ce2ec: ldr      r6, [sp, #0x30]
003ce2f0: ldm      r6, {r0, r3}
003ce2f4: rsb      r3, r0, r3
003ce2f8: asr      r3, r3, #4
003ce2fc: add      r2, r3, r3, lsl #3
003ce300: add      r2, r2, r2, lsl #6
003ce304: add      r2, r3, r2, lsl #3
003ce308: add      r2, r2, r2, lsl #15
003ce30c: add      r2, r3, r2, lsl #3
003ce310: rsb      r2, r2, #0
003ce314: cmp      r2, #1
003ce318: bhi      #0x3ce328
003ce31c: ldr      r0, [sp, #0x1c]
003ce320: bl       #0x708eb0
003ce324: ldr      r0, [r6]
003ce328: mov      r1, #0xbf000000
003ce32c: add      r0, r0, #0x70
003ce330: add      r1, r1, #0x800000
003ce334: bl       #0x31b5e8
003ce338: ldr      r0, [r4, #0x1c]
003ce33c: ldr      r1, [sp, #0xc]
003ce340: mov      r2, sb
003ce344: bl       #0x37c41c
003ce348: ldr      r0, [r4, #0x1c]
003ce34c: ldr      r1, [r8, #0x18]
003ce350: bl       #0x37b574
003ce354: cmp      r0, #0
003ce358: beq      #0x3ce420
003ce35c: mov      r1, #0
003ce360: mov      r0, #0x1c
003ce364: bl       #0x310570
003ce368: ldr      r1, [r4, #4]
003ce36c: mvn      r3, #0
003ce370: ldr      r2, [r8, #0x18]
003ce374: mov      r6, r0
003ce378: bl       #0x3cde2c
003ce37c: ldr      r1, [r4, #0xc4]
003ce380: ldr      r3, [r4, #0xc8]
003ce384: str      r6, [sp, #0x3c]
003ce388: cmp      r1, r3
003ce38c: beq      #0x3ce730
003ce390: str      r6, [r1]
003ce394: ldr      r3, [r4, #0xc4]
003ce398: add      r3, r3, #4
003ce39c: str      r3, [r4, #0xc4]
003ce3a0: ldr      r0, [r4, #0x1c]
003ce3a4: ldr      r1, [sp, #0x10]
003ce3a8: bl       #0x37c514
003ce3ac: ldr      r3, [r7, #4]
003ce3b0: add      r5, r5, #1
003ce3b4: cmp      r3, r5
003ce3b8: bls      #0x3ce40c
003ce3bc: ldr      r0, [r4, #4]
003ce3c0: mov      r1, r5
003ce3c4: bl       #0x3aeac0
003ce3c8: ldr      r3, [r0, #0x14]
003ce3cc: mov      r8, r0
003ce3d0: cmp      r3, #0
003ce3d4: bne      #0x3ce29c
003ce3d8: ldr      r1, [r4, #0xc4]
003ce3dc: ldr      r2, [r4, #0xc8]
003ce3e0: str      r3, [sp, #0x34]
003ce3e4: cmp      r1, r2
003ce3e8: beq      #0x3ce700
003ce3ec: str      r3, [r1]
003ce3f0: ldr      r3, [r4, #0xc4]
003ce3f4: add      r5, r5, #1
003ce3f8: add      r3, r3, #4
003ce3fc: str      r3, [r4, #0xc4]
003ce400: ldr      r3, [r7, #4]
003ce404: cmp      r3, r5
003ce408: bhi      #0x3ce3bc
003ce40c: ldr      r6, [sp, #0x24]
003ce410: mov      r8, sl
003ce414: mov      r0, sb
003ce418: bl       #0x319228
003ce41c: b        #0x3ce134
003ce420: ldr      r1, [r4, #0xc4]
003ce424: ldr      r3, [r4, #0xc8]
003ce428: str      r0, [sp, #0x38]
003ce42c: cmp      r1, r3
003ce430: beq      #0x3ce750
003ce434: str      r0, [r1]
003ce438: ldr      r3, [r4, #0xc4]
003ce43c: add      r3, r3, #4
003ce440: str      r3, [r4, #0xc4]
003ce444: b        #0x3ce3a0
003ce448: add      r3, r4, #0xb4
003ce44c: ldr      r0, [r4, #4]
003ce450: str      r3, [sp, #0x18]
003ce454: bl       #0x3bc5fc
003ce458: add      sb, sp, #0x2c
003ce45c: ldr      r1, [r0, #4]
003ce460: mov      r7, r0
003ce464: ldr      r0, [sp, #0x18]
003ce468: bl       #0x3cd584
003ce46c: mov      r0, sb
003ce470: bl       #0x3192b4
003ce474: ldr      r1, [pc, #0x318]
003ce478: mov      r0, sb
003ce47c: add      r1, pc, r1
003ce480: bl       #0x39ec10
003ce484: mov      r0, sb
003ce488: mvn      r1, #0
003ce48c: bl       #0x3cdd78
003ce490: ldr      r3, [r7, #4]
003ce494: cmp      r3, #0
003ce498: beq      #0x3ce660
003ce49c: ldr      r3, [pc, #0x2f4]
003ce4a0: ldr      r2, [pc, #0x2f4]
003ce4a4: str      r6, [sp, #0x24]
003ce4a8: add      r3, pc, r3
003ce4ac: str      r3, [sp, #8]
003ce4b0: ldr      r3, [pc, #0x2e8]
003ce4b4: str      r2, [sp, #0x20]
003ce4b8: mov      sl, r8
003ce4bc: add      r3, pc, r3
003ce4c0: str      r3, [sp, #0xc]
003ce4c4: ldr      r3, [pc, #0x2d8]
003ce4c8: add      r3, pc, r3
003ce4cc: str      r3, [sp, #0x10]
003ce4d0: ldr      r3, [pc, #0x2d0]
003ce4d4: add      r3, pc, r3
003ce4d8: str      r3, [sp, #0x1c]
003ce4dc: b        #0x3ce608
003ce4e0: ldr      r0, [r4, #0x1c]
003ce4e4: ldr      r1, [sp, #8]
003ce4e8: bl       #0x37b574
003ce4ec: ldr      r6, [sp, #0x30]
003ce4f0: ldm      r6, {r0, r3}
003ce4f4: rsb      r3, r0, r3
003ce4f8: asr      r3, r3, #4
003ce4fc: add      r2, r3, r3, lsl #3
003ce500: add      r2, r2, r2, lsl #6
003ce504: add      r2, r3, r2, lsl #3
003ce508: add      r2, r2, r2, lsl #15
003ce50c: add      r2, r3, r2, lsl #3
003ce510: cmp      r2, #0
003ce514: bne      #0x3ce528
003ce518: ldr      r3, [sp, #0x20]
003ce51c: add      r0, pc, r3
003ce520: bl       #0x708eb0
003ce524: ldr      r0, [r6]
003ce528: ldr      r1, [r8, #0x28]
003ce52c: bl       #0x31c46c
003ce530: ldr      r6, [sp, #0x30]
003ce534: ldm      r6, {r2, r3}
003ce538: rsb      r3, r2, r3
003ce53c: asr      r3, r3, #4
003ce540: add      r1, r3, r3, lsl #3
003ce544: add      r1, r1, r1, lsl #6
003ce548: add      r1, r3, r1, lsl #3
003ce54c: add      r1, r1, r1, lsl #15
003ce550: add      r1, r3, r1, lsl #3
003ce554: rsb      r1, r1, #0
003ce558: cmp      r1, #1
003ce55c: bhi      #0x3ce56c
003ce560: ldr      r0, [sp, #0x1c]
003ce564: bl       #0x708eb0
003ce568: ldr      r2, [r6]
003ce56c: mov      r0, r5
003ce570: add      r6, r2, #0x70
003ce574: bl       #0x30e964
003ce578: mov      r1, r0
003ce57c: mov      r0, r6
003ce580: bl       #0x31b5e8
003ce584: ldr      r0, [r4, #0x1c]
003ce588: ldr      r1, [sp, #0xc]
003ce58c: mov      r2, sb
003ce590: bl       #0x37c41c
003ce594: ldr      r0, [r4, #0x1c]
003ce598: ldr      r1, [r8, #0x28]
003ce59c: bl       #0x37b574
003ce5a0: cmp      r0, #0
003ce5a4: beq      #0x3ce66c
003ce5a8: mov      r1, #0
003ce5ac: mov      r0, #0x1c
003ce5b0: bl       #0x310570
003ce5b4: ldr      r1, [r4, #4]
003ce5b8: mov      r3, r5
003ce5bc: ldr      r2, [r8, #0x28]
003ce5c0: mov      r6, r0
003ce5c4: bl       #0x3cde2c
003ce5c8: ldr      r1, [r4, #0xb8]
003ce5cc: ldr      r3, [r4, #0xbc]
003ce5d0: str      r6, [sp, #0x48]
003ce5d4: cmp      r1, r3
003ce5d8: beq      #0x3ce740
003ce5dc: str      r6, [r1]
003ce5e0: ldr      r3, [r4, #0xb8]
003ce5e4: add      r3, r3, #4
003ce5e8: str      r3, [r4, #0xb8]
003ce5ec: ldr      r0, [r4, #0x1c]
003ce5f0: ldr      r1, [sp, #0x10]
003ce5f4: bl       #0x37c514
003ce5f8: ldr      r3, [r7, #4]
003ce5fc: add      r5, r5, #1
003ce600: cmp      r3, r5
003ce604: bls      #0x3ce658
003ce608: ldr      r0, [r4, #4]
003ce60c: mov      r1, r5
003ce610: bl       #0x3bc784
003ce614: ldr      r3, [r0, #0x24]
003ce618: mov      r8, r0
003ce61c: cmp      r3, #0
003ce620: bne      #0x3ce4e0
003ce624: ldr      r1, [r4, #0xb8]
003ce628: ldr      r2, [r4, #0xbc]
003ce62c: str      r3, [sp, #0x40]
003ce630: cmp      r1, r2
003ce634: beq      #0x3ce710
003ce638: str      r3, [r1]
003ce63c: ldr      r3, [r4, #0xb8]
003ce640: add      r5, r5, #1
003ce644: add      r3, r3, #4
003ce648: str      r3, [r4, #0xb8]
003ce64c: ldr      r3, [r7, #4]
003ce650: cmp      r3, r5
003ce654: bhi      #0x3ce608
003ce658: ldr      r6, [sp, #0x24]
003ce65c: mov      r8, sl
003ce660: mov      r0, sb
003ce664: bl       #0x319228
003ce668: b        #0x3ce120
003ce66c: ldr      r1, [r4, #0xb8]
003ce670: ldr      r3, [r4, #0xbc]
003ce674: str      r0, [sp, #0x44]
003ce678: cmp      r1, r3
003ce67c: beq      #0x3ce720
003ce680: str      r0, [r1]
003ce684: ldr      r3, [r4, #0xb8]
003ce688: add      r3, r3, #4
003ce68c: str      r3, [r4, #0xb8]
003ce690: b        #0x3ce5ec
003ce694: bl       #0x310440
003ce698: b        #0x3ce0d4
003ce69c: bl       #0x310440
003ce6a0: b        #0x3ce1a8
003ce6a4: bl       #0x310440
003ce6a8: b        #0x3ce1e4
003ce6ac: ldr      r2, [pc, #0xf8]
003ce6b0: ldr      r2, [r6, r2]
003ce6b4: ldr      r2, [r2]
003ce6b8: cmp      r2, #2
003ce6bc: streq    r3, [r3]
003ce6c0: beq      #0x3ce078
003ce6c4: cmp      r2, #1
003ce6c8: bne      #0x3ce078
003ce6cc: ldr      r0, [pc, #0xdc]
003ce6d0: ldr      r1, [pc, #0xdc]
003ce6d4: ldr      r2, [pc, #0xdc]
003ce6d8: ldr      r0, [r6, r0]
003ce6dc: ldr      r3, [pc, #0xd8]
003ce6e0: mov      ip, #0x294
003ce6e4: add      r1, pc, r1
003ce6e8: add      r2, pc, r2
003ce6ec: add      r3, pc, r3
003ce6f0: add      r0, r0, #0xa8
003ce6f4: str      ip, [sp]
003ce6f8: bl       #0x30e004
003ce6fc: b        #0x3ce078
003ce700: ldr      r0, [sp, #0x18]
003ce704: add      r2, sp, #0x34
003ce708: bl       #0x3cd89c
003ce70c: b        #0x3ce3ac
003ce710: ldr      r0, [sp, #0x18]
003ce714: add      r2, sp, #0x40
003ce718: bl       #0x3cd89c
003ce71c: b        #0x3ce5f8
003ce720: ldr      r0, [sp, #0x18]
003ce724: add      r2, sp, #0x44
003ce728: bl       #0x3cd89c
003ce72c: b        #0x3ce5ec
003ce730: ldr      r0, [sp, #0x18]
003ce734: add      r2, sp, #0x3c
003ce738: bl       #0x3cd89c
003ce73c: b        #0x3ce3a0
003ce740: ldr      r0, [sp, #0x18]
003ce744: add      r2, sp, #0x48
003ce748: bl       #0x3cd89c
003ce74c: b        #0x3ce5ec
003ce750: ldr      r0, [sp, #0x18]
003ce754: add      r2, sp, #0x38
003ce758: bl       #0x3cd89c
003ce75c: b        #0x3ce3a0
003ce760: bl       #0x30e310
003ce764: subseq   r6, ip, ip, lsr sl
003ce768: andeq    r4, r0, ip, lsr #1
003ce76c: andeq    r0, r0, r4, lsl #17
003ce770: subeq    r7, pc, r0, asr #5
003ce774: subeq    r5, pc, ip, ror #10
003ce778: subeq    r7, pc, ip, ror #3
003ce77c: ldrdeq   sp, lr, [pc], #-0x50
003ce780: umaaleq  r0, pc, r0, r1
003ce784: subeq    r6, pc, ip, lsr #31
003ce788: subeq    r7, pc, r8, lsl #2
003ce78c: strdeq   r7, r8, [pc], #-0xc
003ce790: ldrdeq   r0, r1, [pc], #-0x18
003ce794: subeq    sp, pc, ip, lsl #7
003ce798: subeq    r6, pc, r0, ror sp
003ce79c: subeq    pc, lr, ip, asr #30
003ce7a0: subeq    r6, pc, r4, asr #29
003ce7a4: strheq   r6, [pc], #-0xe8
003ce7a8: umaaleq  pc, lr, r4, pc
003ce7ac: andeq    r3, r0, r0, asr #19
003ce7b0: andeq    r1, r0, r0, asr #19
