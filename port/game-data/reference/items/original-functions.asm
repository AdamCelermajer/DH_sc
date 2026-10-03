
# _ZN6Arrays9ItemTable4readEP11IStreamBase
004ba12c: push     {r4, r5, r6, r7, r8, sl, lr}
004ba130: sub      sp, sp, #0xc
004ba134: mov      sl, r0
004ba138: bl       #0x313a90
004ba13c: ldr      r6, [pc, #0x128]
004ba140: mov      r3, #1
004ba144: cmp      r3, #0
004ba148: str      r0, [sp, #4]
004ba14c: str      r3, [sp]
004ba150: add      r6, pc, r6
004ba154: bne      #0x4ba19c
004ba158: add      r3, sp, #4
004ba15c: add      r2, r3, #2
004ba160: add      r3, r3, #1
004ba164: ldrb     r0, [r2, #1]
004ba168: ldrb     r1, [r3, #-1]
004ba16c: cmp      r2, r3
004ba170: eor      r1, r0, r1
004ba174: strb     r1, [r3, #-1]
004ba178: ldrb     r0, [r2, #1]
004ba17c: eor      r1, r1, r0
004ba180: strb     r1, [r2, #1]
004ba184: ldrb     r0, [r3, #-1]
004ba188: sub      r2, r2, #1
004ba18c: eor      r1, r1, r0
004ba190: strb     r1, [r3, #-1]
004ba194: add      r3, r3, #1
004ba198: bhi      #0x4ba164
004ba19c: bl       #0x4a5f80
004ba1a0: ldr      r7, [pc, #0xc8]
004ba1a4: ldr      r4, [sp, #4]
004ba1a8: mov      r5, #0xa4
004ba1ac: ldr      r3, [r6, r7]
004ba1b0: mul      r0, r5, r4
004ba1b4: str      r4, [r3]
004ba1b8: add      r0, r0, #8
004ba1bc: mov      r1, #1
004ba1c0: bl       #0x31056c
004ba1c4: cmp      r4, #0
004ba1c8: str      r5, [r0]
004ba1cc: str      r4, [r0, #4]
004ba1d0: add      r3, r0, #8
004ba1d4: beq      #0x4ba208
004ba1d8: ldr      r1, [pc, #0x94]
004ba1dc: mov      r2, #0
004ba1e0: ldr      ip, [r6, r1]
004ba1e4: mov      r1, r2
004ba1e8: add      ip, ip, #8
004ba1ec: add      r2, r2, #1
004ba1f0: cmp      r2, r4
004ba1f4: str      r1, [r0, #0x10]
004ba1f8: str      ip, [r0, #8]
004ba1fc: str      r1, [r0, #0x58]
004ba200: add      r0, r0, #0xa4
004ba204: bne      #0x4ba1ec
004ba208: ldr      r2, [r6, r7]
004ba20c: ldr      r8, [pc, #0x64]
004ba210: ldr      r1, [r2]
004ba214: ldr      r2, [r6, r8]
004ba218: cmp      r1, #0
004ba21c: str      r3, [r2]
004ba220: beq      #0x4ba264
004ba224: mov      r4, #0
004ba228: mov      r5, r4
004ba22c: b        #0x4ba238
004ba230: ldr      r3, [r6, r8]
004ba234: ldr      r3, [r3]
004ba238: add      r0, r3, r4
004ba23c: mov      r1, sl
004ba240: ldr      r3, [r3, r4]
004ba244: mov      lr, pc
004ba248: ldr      pc, [r3, #0xc]
004ba24c: ldr      r3, [r6, r7]
004ba250: add      r5, r5, #1
004ba254: add      r4, r4, #0xa4
004ba258: ldr      r3, [r3]
004ba25c: cmp      r3, r5
004ba260: bhi      #0x4ba230
004ba264: add      sp, sp, #0xc
004ba268: pop      {r4, r5, r6, r7, r8, sl, pc}
004ba26c: subeq    sl, sp, r0, asr #18
004ba270: andeq    r0, r0, r0, ror #26
004ba274: andeq    r4, r0, r4, asr r1
004ba278: andeq    r2, r0, ip, ror #16

# _ZN12StreamReader6readAsIbEEvP11IStreamBasePT_
004db89c: str      lr, [sp, #-4]!
004db8a0: mov      r3, #0
004db8a4: sub      sp, sp, #0xc
004db8a8: ldr      ip, [r0]
004db8ac: mov      r2, #1
004db8b0: mov      lr, pc
004db8b4: ldr      pc, [ip, #0x18]
004db8b8: ldr      r3, [pc, #0x74]
004db8bc: cmp      r0, #1
004db8c0: add      r3, pc, r3
004db8c4: beq      #0x4db8f4
004db8c8: ldr      r2, [pc, #0x68]
004db8cc: ldr      r2, [r3, r2]
004db8d0: ldr      r2, [r2]
004db8d4: cmp      r2, #2
004db8d8: moveq    r3, #0
004db8dc: streq    r3, [r3]
004db8e0: beq      #0x4db8ec
004db8e4: cmp      r2, #1
004db8e8: beq      #0x4db900
004db8ec: add      sp, sp, #0xc
004db8f0: ldm      sp!, {pc}
004db8f4: cmp      r1, #0
004db8f8: beq      #0x4db8ec
004db8fc: b        #0x4db8c8
004db900: ldr      r0, [pc, #0x34]
004db904: ldr      r1, [pc, #0x34]
004db908: ldr      r2, [pc, #0x34]
004db90c: ldr      r0, [r3, r0]
004db910: ldr      r3, [pc, #0x30]
004db914: mov      ip, #0x50
004db918: add      r1, pc, r1
004db91c: add      r2, pc, r2
004db920: add      r3, pc, r3
004db924: add      r0, r0, #0xa8
004db928: str      ip, [sp]
004db92c: bl       #0x30e004
004db930: b        #0x4db8ec
004db934: ldrdeq   sb, sl, [fp], #-0x10
004db938: andeq    r3, r0, r0, asr #19
004db93c: andeq    r1, r0, r0, asr #19
004db940: eorseq   r2, lr, r0, asr #21
004db944: eorseq   r2, lr, r4, ror #23
004db948: eorseq   r4, lr, r0, lsr #8

# _ZN7Structs9Inventory4readEP11IStreamBase
004dbb5c: push     {r4, r5, lr}
004dbb60: add      r4, r0, #4
004dbb64: sub      sp, sp, #0xc
004dbb68: mov      r5, r0
004dbb6c: mov      r0, r1
004dbb70: mov      r1, r4
004dbb74: bl       #0x4dbaac
004dbb78: mov      r3, #1
004dbb7c: cmp      r3, #0
004dbb80: str      r3, [sp, #4]
004dbb84: bne      #0x4dbbc4
004dbb88: add      r5, r5, #5
004dbb8c: ldrb     r2, [r4, #1]
004dbb90: ldrb     r3, [r5, #-1]
004dbb94: cmp      r5, r4
004dbb98: eor      r3, r2, r3
004dbb9c: strb     r3, [r5, #-1]
004dbba0: ldrb     r2, [r4, #1]
004dbba4: eor      r3, r3, r2
004dbba8: strb     r3, [r4, #1]
004dbbac: ldrb     r2, [r5, #-1]
004dbbb0: sub      r4, r4, #1
004dbbb4: eor      r3, r3, r2
004dbbb8: strb     r3, [r5, #-1]
004dbbbc: add      r5, r5, #1
004dbbc0: blo      #0x4dbb8c
004dbbc4: add      sp, sp, #0xc
004dbbc8: pop      {r4, r5, pc}

# _ZN6Arrays19GetMemberIDByStringINS_9ItemTableEEEiPKc
004ad5f8: ldr      r3, [pc, #0x60]
004ad5fc: ldr      r2, [pc, #0x60]
004ad600: push     {r4, r5, r6, r7, r8, lr}
004ad604: add      r3, pc, r3
004ad608: ldr      r2, [r3, r2]
004ad60c: mov      r6, r0
004ad610: ldr      r5, [r2]
004ad614: cmp      r5, #0
004ad618: beq      #0x4ad658
004ad61c: ldr      r2, [pc, #0x44]
004ad620: mov      r4, #0
004ad624: ldr      r3, [r3, r2]
004ad628: ldr      r7, [r3]
004ad62c: b        #0x4ad63c
004ad630: add      r4, r4, #1
004ad634: cmp      r4, r5
004ad638: beq      #0x4ad658
004ad63c: ldr      r1, [r7, r4, lsl #2]
004ad640: mov      r0, r6
004ad644: bl       #0x30e31c
004ad648: cmp      r0, #0
004ad64c: bne      #0x4ad630
004ad650: mov      r0, r4
004ad654: pop      {r4, r5, r6, r7, r8, pc}
004ad658: mvn      r0, #0
004ad65c: pop      {r4, r5, r6, r7, r8, pc}
004ad660: subeq    r7, lr, ip, lsl #9
004ad664: andeq    r0, r0, r0, ror #26
004ad668: andeq    r1, r0, r4, asr ip

# _ZNK9Character14CanRangeAttackEv
003a4d3c: movw     r3, #0x1078
003a4d40: ldr      r3, [r0, r3]
003a4d44: cmn      r3, #1
003a4d48: beq      #0x3a4d54
003a4d4c: mov      r0, #1
003a4d50: bx       lr
003a4d54: add      r0, r0, #0x37c
003a4d58: b        #0x400014

# _ZN6Arrays8ItemList4readEP11IStreamBase
004ba27c: push     {r4, r5, r6, r7, r8, sl, lr}
004ba280: sub      sp, sp, #0xc
004ba284: mov      sl, r0
004ba288: bl       #0x313a90
004ba28c: ldr      r6, [pc, #0x124]
004ba290: mov      r3, #1
004ba294: cmp      r3, #0
004ba298: str      r0, [sp, #4]
004ba29c: str      r3, [sp]
004ba2a0: add      r6, pc, r6
004ba2a4: bne      #0x4ba2ec
004ba2a8: add      r3, sp, #4
004ba2ac: add      r2, r3, #2
004ba2b0: add      r3, r3, #1
004ba2b4: ldrb     r0, [r2, #1]
004ba2b8: ldrb     r1, [r3, #-1]
004ba2bc: cmp      r2, r3
004ba2c0: eor      r1, r0, r1
004ba2c4: strb     r1, [r3, #-1]
004ba2c8: ldrb     r0, [r2, #1]
004ba2cc: eor      r1, r1, r0
004ba2d0: strb     r1, [r2, #1]
004ba2d4: ldrb     r0, [r3, #-1]
004ba2d8: sub      r2, r2, #1
004ba2dc: eor      r1, r1, r0
004ba2e0: strb     r1, [r3, #-1]
004ba2e4: add      r3, r3, #1
004ba2e8: bhi      #0x4ba2b4
004ba2ec: bl       #0x4a6100
004ba2f0: ldr      r7, [pc, #0xc4]
004ba2f4: ldr      r4, [sp, #4]
004ba2f8: mov      r5, #0xc
004ba2fc: ldr      r3, [r6, r7]
004ba300: mul      r0, r5, r4
004ba304: str      r4, [r3]
004ba308: add      r0, r0, #8
004ba30c: mov      r1, #1
004ba310: bl       #0x31056c
004ba314: cmp      r4, #0
004ba318: str      r5, [r0]
004ba31c: str      r4, [r0, #4]
004ba320: add      r3, r0, #8
004ba324: beq      #0x4ba354
004ba328: ldr      r1, [pc, #0x90]
004ba32c: mov      r2, #0
004ba330: mov      ip, r2
004ba334: ldr      r1, [r6, r1]
004ba338: add      r1, r1, #8
004ba33c: add      r2, r2, #1
004ba340: cmp      r2, r4
004ba344: str      r1, [r0, #8]
004ba348: str      ip, [r0, #0x10]
004ba34c: add      r0, r0, #0xc
004ba350: bne      #0x4ba33c
004ba354: ldr      r2, [r6, r7]
004ba358: ldr      r8, [pc, #0x64]
004ba35c: ldr      r1, [r2]
004ba360: ldr      r2, [r6, r8]
004ba364: cmp      r1, #0
004ba368: str      r3, [r2]
004ba36c: beq      #0x4ba3b0
004ba370: mov      r4, #0
004ba374: mov      r5, r4
004ba378: b        #0x4ba384
004ba37c: ldr      r3, [r6, r8]
004ba380: ldr      r3, [r3]
004ba384: add      r0, r3, r4
004ba388: mov      r1, sl
004ba38c: ldr      r3, [r3, r4]
004ba390: mov      lr, pc
004ba394: ldr      pc, [r3, #0xc]
004ba398: ldr      r3, [r6, r7]
004ba39c: add      r5, r5, #1
004ba3a0: add      r4, r4, #0xc
004ba3a4: ldr      r3, [r3]
004ba3a8: cmp      r3, r5
004ba3ac: bhi      #0x4ba37c
004ba3b0: add      sp, sp, #0xc
004ba3b4: pop      {r4, r5, r6, r7, r8, sl, pc}
004ba3b8: strdeq   sl, fp, [sp], #-0x70
004ba3bc: ldrdeq   r2, r3, [r0], -r0
004ba3c0: andeq    r3, r0, r8, lsr #16
004ba3c4: andeq    r1, r0, r8, lsr #13

# _ZNK13ItemInventory15HasRangedWeaponEv
003fffa4: push     {r4, r5, r6, lr}
003fffa8: mov      r1, #1
003fffac: mov      r4, r0
003fffb0: bl       #0x3fc6a8
003fffb4: mov      r5, #0xc
003fffb8: mul      r5, r5, r0
003fffbc: ldr      r3, [r4, #0x14]
003fffc0: ldr      r3, [r3, r5]
003fffc4: ldr      r0, [r3, #4]
003fffc8: cmp      r0, #0
003fffcc: beq      #0x400008
003fffd0: ldr      r0, [r0]
003fffd4: bl       #0x3f9e08
003fffd8: ldr      r3, [r0, #0x58]
003fffdc: cmp      r3, #4
003fffe0: beq      #0x40000c
003fffe4: ldr      r3, [r4, #0x14]
003fffe8: ldr      r3, [r3, r5]
003fffec: ldr      r3, [r3, #4]
003ffff0: ldr      r0, [r3]
003ffff4: bl       #0x3f9e08
003ffff8: ldr      r0, [r0, #0x58]
003ffffc: cmp      r0, #5
00400000: movne    r0, #0
00400004: moveq    r0, #1
00400008: pop      {r4, r5, r6, pc}
0040000c: mov      r0, #1
00400010: pop      {r4, r5, r6, pc}

# _ZN6Arrays21DropTilePriorityTable4readEP11IStreamBase
004ba4fc: push     {r4, r5, r6, r7, r8, sl, lr}
004ba500: sub      sp, sp, #0xc
004ba504: mov      sl, r0
004ba508: bl       #0x313a90
004ba50c: ldr      r6, [pc, #0x124]
004ba510: mov      r3, #1
004ba514: cmp      r3, #0
004ba518: str      r0, [sp, #4]
004ba51c: str      r3, [sp]
004ba520: add      r6, pc, r6
004ba524: bne      #0x4ba56c
004ba528: add      r3, sp, #4
004ba52c: add      r2, r3, #2
004ba530: add      r3, r3, #1
004ba534: ldrb     r0, [r2, #1]
004ba538: ldrb     r1, [r3, #-1]
004ba53c: cmp      r2, r3
004ba540: eor      r1, r0, r1
004ba544: strb     r1, [r3, #-1]
004ba548: ldrb     r0, [r2, #1]
004ba54c: eor      r1, r1, r0
004ba550: strb     r1, [r2, #1]
004ba554: ldrb     r0, [r3, #-1]
004ba558: sub      r2, r2, #1
004ba55c: eor      r1, r1, r0
004ba560: strb     r1, [r3, #-1]
004ba564: add      r3, r3, #1
004ba568: bhi      #0x4ba534
004ba56c: bl       #0x4a63f4
004ba570: ldr      r7, [pc, #0xc4]
004ba574: ldr      r4, [sp, #4]
004ba578: mov      r5, #0xc
004ba57c: ldr      r3, [r6, r7]
004ba580: mul      r0, r5, r4
004ba584: str      r4, [r3]
004ba588: add      r0, r0, #8
004ba58c: mov      r1, #1
004ba590: bl       #0x31056c
004ba594: cmp      r4, #0
004ba598: str      r5, [r0]
004ba59c: str      r4, [r0, #4]
004ba5a0: add      r3, r0, #8
004ba5a4: beq      #0x4ba5d4
004ba5a8: ldr      r1, [pc, #0x90]
004ba5ac: mov      r2, #0
004ba5b0: mov      ip, r2
004ba5b4: ldr      r1, [r6, r1]
004ba5b8: add      r1, r1, #8
004ba5bc: add      r2, r2, #1
004ba5c0: cmp      r2, r4
004ba5c4: str      r1, [r0, #8]
004ba5c8: str      ip, [r0, #0x10]
004ba5cc: add      r0, r0, #0xc
004ba5d0: bne      #0x4ba5bc
004ba5d4: ldr      r2, [r6, r7]
004ba5d8: ldr      r8, [pc, #0x64]
004ba5dc: ldr      r1, [r2]
004ba5e0: ldr      r2, [r6, r8]
004ba5e4: cmp      r1, #0
004ba5e8: str      r3, [r2]
004ba5ec: beq      #0x4ba630
004ba5f0: mov      r4, #0
004ba5f4: mov      r5, r4
004ba5f8: b        #0x4ba604
004ba5fc: ldr      r3, [r6, r8]
004ba600: ldr      r3, [r3]
004ba604: add      r0, r3, r4
004ba608: mov      r1, sl
004ba60c: ldr      r3, [r3, r4]
004ba610: mov      lr, pc
004ba614: ldr      pc, [r3, #0xc]
004ba618: ldr      r3, [r6, r7]
004ba61c: add      r5, r5, #1
004ba620: add      r4, r4, #0xc
004ba624: ldr      r3, [r3]
004ba628: cmp      r3, r5
004ba62c: bhi      #0x4ba5fc
004ba630: add      sp, sp, #0xc
004ba634: pop      {r4, r5, r6, r7, r8, sl, pc}
004ba638: subeq    sl, sp, r0, ror r5
004ba63c: strheq   r4, [r0], -r4
004ba640: andeq    r1, r0, r8, lsr r8
004ba644: strdeq   r3, r4, [r0], -r0

# _ZN6Arrays9ItemTable13finalizeNamesEv
004a5ee4: push     {r4, r5, r6, r7, r8, lr}
004a5ee8: ldr      r5, [pc, #0x84]
004a5eec: ldr      r6, [pc, #0x84]
004a5ef0: add      r5, pc, r5
004a5ef4: ldr      r3, [r5, r6]
004a5ef8: ldr      r3, [r3]
004a5efc: cmp      r3, #0
004a5f00: beq      #0x4a5f70
004a5f04: ldr      r7, [pc, #0x70]
004a5f08: ldr      r2, [r5, r7]
004a5f0c: ldr      r2, [r2]
004a5f10: cmp      r2, #0
004a5f14: beq      #0x4a5f5c
004a5f18: mov      r4, #0
004a5f1c: b        #0x4a5f28
004a5f20: ldr      r3, [r5, r6]
004a5f24: ldr      r3, [r3]
004a5f28: ldr      r0, [r3, r4, lsl #2]
004a5f2c: add      r4, r4, #1
004a5f30: cmp      r0, #0
004a5f34: beq      #0x4a5f44
004a5f38: bl       #0x310440
004a5f3c: ldr      r3, [r5, r6]
004a5f40: ldr      r3, [r3]
004a5f44: ldr      r2, [r5, r7]
004a5f48: ldr      r2, [r2]
004a5f4c: cmp      r2, r4
004a5f50: bhi      #0x4a5f20
004a5f54: cmp      r3, #0
004a5f58: beq      #0x4a5f64
004a5f5c: mov      r0, r3
004a5f60: bl       #0x310440
004a5f64: ldr      r3, [r5, r6]
004a5f68: mov      r2, #0
004a5f6c: str      r2, [r3]
004a5f70: pop      {r4, r5, r6, r7, r8, pc}
004a5f74: subeq    lr, lr, r0, lsr #23
004a5f78: andeq    r1, r0, r4, asr ip
004a5f7c: andeq    r0, r0, r0, ror #26

# _ZN12StreamReader6readAsIsEEvP11IStreamBasePT_
004dbaac: str      lr, [sp, #-4]!
004dbab0: mov      r3, #0
004dbab4: sub      sp, sp, #0xc
004dbab8: ldr      ip, [r0]
004dbabc: mov      r2, #2
004dbac0: mov      lr, pc
004dbac4: ldr      pc, [ip, #0x18]
004dbac8: ldr      r3, [pc, #0x74]
004dbacc: cmp      r0, #2
004dbad0: add      r3, pc, r3
004dbad4: beq      #0x4dbb04
004dbad8: ldr      r2, [pc, #0x68]
004dbadc: ldr      r2, [r3, r2]
004dbae0: ldr      r2, [r2]
004dbae4: cmp      r2, #2
004dbae8: moveq    r3, #0
004dbaec: streq    r3, [r3]
004dbaf0: beq      #0x4dbafc
004dbaf4: cmp      r2, #1
004dbaf8: beq      #0x4dbb10
004dbafc: add      sp, sp, #0xc
004dbb00: ldm      sp!, {pc}
004dbb04: cmp      r1, #0
004dbb08: beq      #0x4dbafc
004dbb0c: b        #0x4dbad8
004dbb10: ldr      r0, [pc, #0x34]
004dbb14: ldr      r1, [pc, #0x34]
004dbb18: ldr      r2, [pc, #0x34]
004dbb1c: ldr      r0, [r3, r0]
004dbb20: ldr      r3, [pc, #0x30]
004dbb24: mov      ip, #0x50
004dbb28: add      r1, pc, r1
004dbb2c: add      r2, pc, r2
004dbb30: add      r3, pc, r3
004dbb34: add      r0, r0, #0xa8
004dbb38: str      ip, [sp]
004dbb3c: bl       #0x30e004
004dbb40: b        #0x4dbafc
004dbb44: subeq    r8, fp, r0, asr #31
004dbb48: andeq    r3, r0, r0, asr #19
004dbb4c: andeq    r1, r0, r0, asr #19
004dbb50: ldrhteq  r2, [lr], -r0
004dbb54: ldrsbteq r2, [lr], -r4
004dbb58: eorseq   r4, lr, r0, lsl r2

# _ZN6Arrays9ItemTable8finalizeEv
004a5f80: push     {r4, r5, r6, r7, r8, lr}
004a5f84: ldr      r5, [pc, #0xcc]
004a5f88: ldr      r7, [pc, #0xcc]
004a5f8c: add      r5, pc, r5
004a5f90: ldr      r3, [r5, r7]
004a5f94: ldr      r3, [r3]
004a5f98: cmp      r3, #0
004a5f9c: beq      #0x4a6054
004a5fa0: ldr      r8, [pc, #0xb8]
004a5fa4: ldr      r2, [r5, r8]
004a5fa8: ldr      r2, [r2]
004a5fac: cmp      r2, #0
004a5fb0: beq      #0x4a6000
004a5fb4: mov      r4, #0
004a5fb8: mov      r6, r4
004a5fbc: b        #0x4a5fc8
004a5fc0: ldr      r3, [r5, r7]
004a5fc4: ldr      r3, [r3]
004a5fc8: add      r0, r3, r4
004a5fcc: ldr      r3, [r3, r4]
004a5fd0: mov      lr, pc
004a5fd4: ldr      pc, [r3, #8]
004a5fd8: ldr      r3, [r5, r8]
004a5fdc: add      r6, r6, #1
004a5fe0: add      r4, r4, #0xa4
004a5fe4: ldr      r3, [r3]
004a5fe8: cmp      r3, r6
004a5fec: bhi      #0x4a5fc0
004a5ff0: ldr      r3, [r5, r7]
004a5ff4: ldr      r3, [r3]
004a5ff8: cmp      r3, #0
004a5ffc: beq      #0x4a6048
004a6000: ldr      r2, [r3, #-4]
004a6004: mov      r0, #0xa4
004a6008: mla      r0, r0, r2, r3
004a600c: cmp      r3, r0
004a6010: bne      #0x4a601c
004a6014: b        #0x4a6040
004a6018: mov      r0, r4
004a601c: sub      r4, r0, #0xa4
004a6020: ldr      r3, [r0, #-0xa4]
004a6024: mov      r0, r4
004a6028: mov      lr, pc
004a602c: ldr      pc, [r3]
004a6030: ldr      r3, [r5, r7]
004a6034: ldr      r0, [r3]
004a6038: cmp      r0, r4
004a603c: bne      #0x4a6018
004a6040: sub      r0, r0, #8
004a6044: bl       #0x310440
004a6048: ldr      r3, [r5, r7]
004a604c: mov      r2, #0
004a6050: str      r2, [r3]
004a6054: pop      {r4, r5, r6, r7, r8, pc}
004a6058: subeq    lr, lr, r4, lsl #22
004a605c: andeq    r2, r0, ip, ror #16
004a6060: andeq    r0, r0, r0, ror #26

# _ZN12StreamReader12readStringExEP11IStreamBasePcy
00317454: push     {r4, r5, r6, lr}
00317458: ldr      ip, [r0]
0031745c: mov      r4, r2
00317460: mov      r5, r3
00317464: mov      lr, pc
00317468: ldr      pc, [ip, #0x18]
0031746c: cmp      r0, r4
00317470: mov      r0, #0
00317474: beq      #0x317480
00317478: and      r0, r0, #1
0031747c: pop      {r4, r5, r6, pc}
00317480: cmp      r1, r5
00317484: moveq    r0, #1
00317488: and      r0, r0, #1
0031748c: pop      {r4, r5, r6, pc}

# _ZNK12ItemInstance7GetItemEv
003f9e08: ldr      r3, [pc, #0x1c]
003f9e0c: ldr      r1, [pc, #0x1c]
003f9e10: ldr      r2, [r0, #4]
003f9e14: add      r3, pc, r3
003f9e18: ldr      r1, [r3, r1]
003f9e1c: mov      r0, #0xa4
003f9e20: ldr      r3, [r1]
003f9e24: mla      r0, r0, r2, r3
003f9e28: bx       lr
003f9e2c: subseq   sl, sb, ip, ror ip
003f9e30: andeq    r2, r0, ip, ror #16

# _ZN7Structs4Item4readEP11IStreamBase
004fc068: push     {r4, r5, r6, lr}
004fc06c: mov      r4, r0
004fc070: sub      sp, sp, #8
004fc074: mov      r5, r1
004fc078: bl       #0x4ec47c
004fc07c: mov      r0, r5
004fc080: add      r1, r4, #0x44
004fc084: bl       #0x459090
004fc088: mov      r3, #1
004fc08c: cmp      r3, #0
004fc090: str      r3, [sp, #4]
004fc094: bne      #0x4fc0d8
004fc098: add      r3, r4, #0x45
004fc09c: add      r2, r4, #0x46
004fc0a0: ldrb     r0, [r2, #1]
004fc0a4: ldrb     r1, [r3, #-1]
004fc0a8: cmp      r3, r2
004fc0ac: eor      r1, r0, r1
004fc0b0: strb     r1, [r3, #-1]
004fc0b4: ldrb     r0, [r2, #1]
004fc0b8: eor      r1, r1, r0
004fc0bc: strb     r1, [r2, #1]
004fc0c0: ldrb     r0, [r3, #-1]
004fc0c4: sub      r2, r2, #1
004fc0c8: eor      r1, r1, r0
004fc0cc: strb     r1, [r3, #-1]
004fc0d0: add      r3, r3, #1
004fc0d4: blo      #0x4fc0a0
004fc0d8: mov      r0, r5
004fc0dc: add      r1, r4, #0x48
004fc0e0: bl       #0x459090
004fc0e4: mov      r3, #1
004fc0e8: cmp      r3, #0
004fc0ec: str      r3, [sp, #4]
004fc0f0: bne      #0x4fc134
004fc0f4: add      r3, r4, #0x49
004fc0f8: add      r2, r4, #0x4a
004fc0fc: ldrb     r0, [r2, #1]
004fc100: ldrb     r1, [r3, #-1]
004fc104: cmp      r3, r2
004fc108: eor      r1, r0, r1
004fc10c: strb     r1, [r3, #-1]
004fc110: ldrb     r0, [r2, #1]
004fc114: eor      r1, r1, r0
004fc118: strb     r1, [r2, #1]
004fc11c: ldrb     r0, [r3, #-1]
004fc120: sub      r2, r2, #1
004fc124: eor      r1, r1, r0
004fc128: strb     r1, [r3, #-1]
004fc12c: add      r3, r3, #1
004fc130: blo      #0x4fc0fc
004fc134: mov      r0, r5
004fc138: add      r1, r4, #0x4c
004fc13c: bl       #0x3df1a0
004fc140: mov      r3, #1
004fc144: cmp      r3, #0
004fc148: str      r3, [sp, #4]
004fc14c: bne      #0x4fc190
004fc150: add      r3, r4, #0x4d
004fc154: add      r2, r4, #0x4e
004fc158: ldrb     r0, [r2, #1]
004fc15c: ldrb     r1, [r3, #-1]
004fc160: cmp      r3, r2
004fc164: eor      r1, r0, r1
004fc168: strb     r1, [r3, #-1]
004fc16c: ldrb     r0, [r2, #1]
004fc170: eor      r1, r1, r0
004fc174: strb     r1, [r2, #1]
004fc178: ldrb     r0, [r3, #-1]
004fc17c: sub      r2, r2, #1
004fc180: eor      r1, r1, r0
004fc184: strb     r1, [r3, #-1]
004fc188: add      r3, r3, #1
004fc18c: blo      #0x4fc158
004fc190: ldr      r0, [r4, #0x50]
004fc194: cmp      r0, #0
004fc198: beq      #0x4fc1a0
004fc19c: bl       #0x310440
004fc1a0: ldr      r0, [r4, #0x4c]
004fc1a4: mov      r1, #1
004fc1a8: mov      r6, #0
004fc1ac: add      r0, r0, r1
004fc1b0: bl       #0x31056c
004fc1b4: ldr      r2, [r4, #0x4c]
004fc1b8: mov      r1, r0
004fc1bc: str      r0, [r4, #0x50]
004fc1c0: mov      r3, r6
004fc1c4: mov      r0, r5
004fc1c8: bl       #0x317454
004fc1cc: ldr      r3, [r4, #0x4c]
004fc1d0: ldr      r2, [r4, #0x50]
004fc1d4: mov      r0, r5
004fc1d8: add      r1, r4, #0x54
004fc1dc: strb     r6, [r2, r3]
004fc1e0: bl       #0x459090
004fc1e4: mov      r3, #1
004fc1e8: cmp      r3, r6
004fc1ec: str      r3, [sp, #4]
004fc1f0: bne      #0x4fc234
004fc1f4: add      r3, r4, #0x55
004fc1f8: add      r2, r4, #0x56
004fc1fc: ldrb     r0, [r2, #1]
004fc200: ldrb     r1, [r3, #-1]
004fc204: cmp      r3, r2
004fc208: eor      r1, r0, r1
004fc20c: strb     r1, [r3, #-1]
004fc210: ldrb     r0, [r2, #1]
004fc214: eor      r1, r1, r0
004fc218: strb     r1, [r2, #1]
004fc21c: ldrb     r0, [r3, #-1]
004fc220: sub      r2, r2, #1
004fc224: eor      r1, r1, r0
004fc228: strb     r1, [r3, #-1]
004fc22c: add      r3, r3, #1
004fc230: blo      #0x4fc1fc
004fc234: mov      r0, r5
004fc238: add      r1, r4, #0x58
004fc23c: bl       #0x459090
004fc240: mov      r3, #1
004fc244: cmp      r3, #0
004fc248: str      r3, [sp, #4]
004fc24c: bne      #0x4fc290
004fc250: add      r3, r4, #0x59
004fc254: add      r2, r4, #0x5a
004fc258: ldrb     r0, [r2, #1]
004fc25c: ldrb     r1, [r3, #-1]
004fc260: cmp      r3, r2
004fc264: eor      r1, r0, r1
004fc268: strb     r1, [r3, #-1]
004fc26c: ldrb     r0, [r2, #1]
004fc270: eor      r1, r1, r0
004fc274: strb     r1, [r2, #1]
004fc278: ldrb     r0, [r3, #-1]
004fc27c: sub      r2, r2, #1
004fc280: eor      r1, r1, r0
004fc284: strb     r1, [r3, #-1]
004fc288: add      r3, r3, #1
004fc28c: blo      #0x4fc258
004fc290: mov      r0, r5
004fc294: add      r1, r4, #0x5c
004fc298: bl       #0x459090
004fc29c: mov      r3, #1
004fc2a0: cmp      r3, #0
004fc2a4: str      r3, [sp, #4]
004fc2a8: bne      #0x4fc2ec
004fc2ac: add      r3, r4, #0x5d
004fc2b0: add      r2, r4, #0x5e
004fc2b4: ldrb     r0, [r2, #1]
004fc2b8: ldrb     r1, [r3, #-1]
004fc2bc: cmp      r3, r2
004fc2c0: eor      r1, r0, r1
004fc2c4: strb     r1, [r3, #-1]
004fc2c8: ldrb     r0, [r2, #1]
004fc2cc: eor      r1, r1, r0
004fc2d0: strb     r1, [r2, #1]
004fc2d4: ldrb     r0, [r3, #-1]
004fc2d8: sub      r2, r2, #1
004fc2dc: eor      r1, r1, r0
004fc2e0: strb     r1, [r3, #-1]
004fc2e4: add      r3, r3, #1
004fc2e8: blo      #0x4fc2b4
004fc2ec: mov      r0, r5
004fc2f0: add      r1, r4, #0x60
004fc2f4: bl       #0x459090
004fc2f8: mov      r3, #1
004fc2fc: cmp      r3, #0
004fc300: str      r3, [sp, #4]
004fc304: bne      #0x4fc348
004fc308: add      r3, r4, #0x61
004fc30c: add      r2, r4, #0x62
004fc310: ldrb     r0, [r2, #1]
004fc314: ldrb     r1, [r3, #-1]
004fc318: cmp      r3, r2
004fc31c: eor      r1, r0, r1
004fc320: strb     r1, [r3, #-1]
004fc324: ldrb     r0, [r2, #1]
004fc328: eor      r1, r1, r0
004fc32c: strb     r1, [r2, #1]
004fc330: ldrb     r0, [r3, #-1]
004fc334: sub      r2, r2, #1
004fc338: eor      r1, r1, r0
004fc33c: strb     r1, [r3, #-1]
004fc340: add      r3, r3, #1
004fc344: blo      #0x4fc310
004fc348: mov      r0, r5
004fc34c: add      r1, r4, #0x64
004fc350: bl       #0x459090
004fc354: mov      r3, #1
004fc358: cmp      r3, #0
004fc35c: str      r3, [sp, #4]
004fc360: bne      #0x4fc3a4
004fc364: add      r3, r4, #0x65
004fc368: add      r2, r4, #0x66
004fc36c: ldrb     r0, [r2, #1]
004fc370: ldrb     r1, [r3, #-1]
004fc374: cmp      r3, r2
004fc378: eor      r1, r0, r1
004fc37c: strb     r1, [r3, #-1]
004fc380: ldrb     r0, [r2, #1]
004fc384: eor      r1, r1, r0
004fc388: strb     r1, [r2, #1]
004fc38c: ldrb     r0, [r3, #-1]
004fc390: sub      r2, r2, #1
004fc394: eor      r1, r1, r0
004fc398: strb     r1, [r3, #-1]
004fc39c: add      r3, r3, #1
004fc3a0: blo      #0x4fc36c
004fc3a4: mov      r0, r5
004fc3a8: add      r1, r4, #0x68
004fc3ac: bl       #0x459090
004fc3b0: mov      r3, #1
004fc3b4: cmp      r3, #0
004fc3b8: str      r3, [sp, #4]
004fc3bc: bne      #0x4fc400
004fc3c0: add      r3, r4, #0x69
004fc3c4: add      r2, r4, #0x6a
004fc3c8: ldrb     r0, [r2, #1]
004fc3cc: ldrb     r1, [r3, #-1]
004fc3d0: cmp      r3, r2
004fc3d4: eor      r1, r0, r1
004fc3d8: strb     r1, [r3, #-1]
004fc3dc: ldrb     r0, [r2, #1]
004fc3e0: eor      r1, r1, r0
004fc3e4: strb     r1, [r2, #1]
004fc3e8: ldrb     r0, [r3, #-1]
004fc3ec: sub      r2, r2, #1
004fc3f0: eor      r1, r1, r0
004fc3f4: strb     r1, [r3, #-1]
004fc3f8: add      r3, r3, #1
004fc3fc: blo      #0x4fc3c8
004fc400: mov      r0, r5
004fc404: add      r1, r4, #0x6c
004fc408: bl       #0x459090
004fc40c: mov      r3, #1
004fc410: cmp      r3, #0
004fc414: str      r3, [sp, #4]
004fc418: bne      #0x4fc45c
004fc41c: add      r3, r4, #0x6d
004fc420: add      r2, r4, #0x6e
004fc424: ldrb     r0, [r2, #1]
004fc428: ldrb     r1, [r3, #-1]
004fc42c: cmp      r3, r2
004fc430: eor      r1, r0, r1
004fc434: strb     r1, [r3, #-1]
004fc438: ldrb     r0, [r2, #1]
004fc43c: eor      r1, r1, r0
004fc440: strb     r1, [r2, #1]
004fc444: ldrb     r0, [r3, #-1]
004fc448: sub      r2, r2, #1
004fc44c: eor      r1, r1, r0
004fc450: strb     r1, [r3, #-1]
004fc454: add      r3, r3, #1
004fc458: blo      #0x4fc424
004fc45c: mov      r0, r5
004fc460: add      r1, r4, #0x70
004fc464: bl       #0x459090
004fc468: mov      r3, #1
004fc46c: cmp      r3, #0
004fc470: str      r3, [sp, #4]
004fc474: bne      #0x4fc4b8
004fc478: add      r3, r4, #0x71
004fc47c: add      r2, r4, #0x72
004fc480: ldrb     r0, [r2, #1]
004fc484: ldrb     r1, [r3, #-1]
004fc488: cmp      r3, r2
004fc48c: eor      r1, r0, r1
004fc490: strb     r1, [r3, #-1]
004fc494: ldrb     r0, [r2, #1]
004fc498: eor      r1, r1, r0
004fc49c: strb     r1, [r2, #1]
004fc4a0: ldrb     r0, [r3, #-1]
004fc4a4: sub      r2, r2, #1
004fc4a8: eor      r1, r1, r0
004fc4ac: strb     r1, [r3, #-1]
004fc4b0: add      r3, r3, #1
004fc4b4: blo      #0x4fc480
004fc4b8: mov      r0, r5
004fc4bc: add      r1, r4, #0x74
004fc4c0: bl       #0x459090
004fc4c4: mov      r3, #1
004fc4c8: cmp      r3, #0
004fc4cc: str      r3, [sp, #4]
004fc4d0: bne      #0x4fc514
004fc4d4: add      r3, r4, #0x75
004fc4d8: add      r2, r4, #0x76
004fc4dc: ldrb     r0, [r2, #1]
004fc4e0: ldrb     r1, [r3, #-1]
004fc4e4: cmp      r3, r2
004fc4e8: eor      r1, r0, r1
004fc4ec: strb     r1, [r3, #-1]
004fc4f0: ldrb     r0, [r2, #1]
004fc4f4: eor      r1, r1, r0
004fc4f8: strb     r1, [r2, #1]
004fc4fc: ldrb     r0, [r3, #-1]
004fc500: sub      r2, r2, #1
004fc504: eor      r1, r1, r0
004fc508: strb     r1, [r3, #-1]
004fc50c: add      r3, r3, #1
004fc510: blo      #0x4fc4dc
004fc514: mov      r0, r5
004fc518: add      r1, r4, #0x78
004fc51c: bl       #0x459090
004fc520: mov      r3, #1
004fc524: cmp      r3, #0
004fc528: str      r3, [sp, #4]
004fc52c: bne      #0x4fc570
004fc530: add      r3, r4, #0x79
004fc534: add      r2, r4, #0x7a
004fc538: ldrb     r0, [r2, #1]
004fc53c: ldrb     r1, [r3, #-1]
004fc540: cmp      r3, r2
004fc544: eor      r1, r0, r1
004fc548: strb     r1, [r3, #-1]
004fc54c: ldrb     r0, [r2, #1]
004fc550: eor      r1, r1, r0
004fc554: strb     r1, [r2, #1]
004fc558: ldrb     r0, [r3, #-1]
004fc55c: sub      r2, r2, #1
004fc560: eor      r1, r1, r0
004fc564: strb     r1, [r3, #-1]
004fc568: add      r3, r3, #1
004fc56c: blo      #0x4fc538
004fc570: mov      r0, r5
004fc574: add      r1, r4, #0x7c
004fc578: bl       #0x459090
004fc57c: mov      r3, #1
004fc580: cmp      r3, #0
004fc584: str      r3, [sp, #4]
004fc588: bne      #0x4fc5cc
004fc58c: add      r3, r4, #0x7d
004fc590: add      r2, r4, #0x7e
004fc594: ldrb     r0, [r2, #1]
004fc598: ldrb     r1, [r3, #-1]
004fc59c: cmp      r3, r2
004fc5a0: eor      r1, r0, r1
004fc5a4: strb     r1, [r3, #-1]
004fc5a8: ldrb     r0, [r2, #1]
004fc5ac: eor      r1, r1, r0
004fc5b0: strb     r1, [r2, #1]
004fc5b4: ldrb     r0, [r3, #-1]
004fc5b8: sub      r2, r2, #1
004fc5bc: eor      r1, r1, r0
004fc5c0: strb     r1, [r3, #-1]
004fc5c4: add      r3, r3, #1
004fc5c8: blo      #0x4fc594
004fc5cc: mov      r0, r5
004fc5d0: add      r1, r4, #0x80
004fc5d4: bl       #0x459090
004fc5d8: mov      r3, #1
004fc5dc: cmp      r3, #0
004fc5e0: str      r3, [sp, #4]
004fc5e4: bne      #0x4fc628
004fc5e8: add      r3, r4, #0x81
004fc5ec: add      r2, r4, #0x82
004fc5f0: ldrb     r0, [r2, #1]
004fc5f4: ldrb     r1, [r3, #-1]
004fc5f8: cmp      r3, r2
004fc5fc: eor      r1, r0, r1
004fc600: strb     r1, [r3, #-1]
004fc604: ldrb     r0, [r2, #1]
004fc608: eor      r1, r1, r0
004fc60c: strb     r1, [r2, #1]
004fc610: ldrb     r0, [r3, #-1]
004fc614: sub      r2, r2, #1
004fc618: eor      r1, r1, r0
004fc61c: strb     r1, [r3, #-1]
004fc620: add      r3, r3, #1
004fc624: blo      #0x4fc5f0
004fc628: mov      r0, r5
004fc62c: add      r1, r4, #0x84
004fc630: bl       #0x459090
004fc634: mov      r3, #1
004fc638: cmp      r3, #0
004fc63c: str      r3, [sp, #4]
004fc640: bne      #0x4fc684
004fc644: add      r3, r4, #0x85
004fc648: add      r2, r4, #0x86
004fc64c: ldrb     r0, [r2, #1]
004fc650: ldrb     r1, [r3, #-1]
004fc654: cmp      r3, r2
004fc658: eor      r1, r0, r1
004fc65c: strb     r1, [r3, #-1]
004fc660: ldrb     r0, [r2, #1]
004fc664: eor      r1, r1, r0
004fc668: strb     r1, [r2, #1]
004fc66c: ldrb     r0, [r3, #-1]
004fc670: sub      r2, r2, #1
004fc674: eor      r1, r1, r0
004fc678: strb     r1, [r3, #-1]
004fc67c: add      r3, r3, #1
004fc680: blo      #0x4fc64c
004fc684: mov      r0, r5
004fc688: add      r1, r4, #0x88
004fc68c: bl       #0x459090
004fc690: mov      r3, #1
004fc694: cmp      r3, #0
004fc698: str      r3, [sp, #4]
004fc69c: bne      #0x4fc6e0
004fc6a0: add      r3, r4, #0x89
004fc6a4: add      r2, r4, #0x8a
004fc6a8: ldrb     r0, [r2, #1]
004fc6ac: ldrb     r1, [r3, #-1]
004fc6b0: cmp      r3, r2
004fc6b4: eor      r1, r0, r1
004fc6b8: strb     r1, [r3, #-1]
004fc6bc: ldrb     r0, [r2, #1]
004fc6c0: eor      r1, r1, r0
004fc6c4: strb     r1, [r2, #1]
004fc6c8: ldrb     r0, [r3, #-1]
004fc6cc: sub      r2, r2, #1
004fc6d0: eor      r1, r1, r0
004fc6d4: strb     r1, [r3, #-1]
004fc6d8: add      r3, r3, #1
004fc6dc: blo      #0x4fc6a8
004fc6e0: mov      r0, r5
004fc6e4: add      r1, r4, #0x8c
004fc6e8: bl       #0x459090
004fc6ec: mov      r3, #1
004fc6f0: cmp      r3, #0
004fc6f4: str      r3, [sp, #4]
004fc6f8: bne      #0x4fc73c
004fc6fc: add      r3, r4, #0x8d
004fc700: add      r2, r4, #0x8e
004fc704: ldrb     r0, [r2, #1]
004fc708: ldrb     r1, [r3, #-1]
004fc70c: cmp      r3, r2
004fc710: eor      r1, r0, r1
004fc714: strb     r1, [r3, #-1]
004fc718: ldrb     r0, [r2, #1]
004fc71c: eor      r1, r1, r0
004fc720: strb     r1, [r2, #1]
004fc724: ldrb     r0, [r3, #-1]
004fc728: sub      r2, r2, #1
004fc72c: eor      r1, r1, r0
004fc730: strb     r1, [r3, #-1]
004fc734: add      r3, r3, #1
004fc738: blo      #0x4fc704
004fc73c: mov      r0, r5
004fc740: add      r1, r4, #0x90
004fc744: bl       #0x459090
004fc748: mov      r3, #1
004fc74c: cmp      r3, #0
004fc750: str      r3, [sp, #4]
004fc754: bne      #0x4fc798
004fc758: add      r3, r4, #0x91
004fc75c: add      r2, r4, #0x92
004fc760: ldrb     r0, [r2, #1]
004fc764: ldrb     r1, [r3, #-1]
004fc768: cmp      r3, r2
004fc76c: eor      r1, r0, r1
004fc770: strb     r1, [r3, #-1]
004fc774: ldrb     r0, [r2, #1]
004fc778: eor      r1, r1, r0
004fc77c: strb     r1, [r2, #1]
004fc780: ldrb     r0, [r3, #-1]
004fc784: sub      r2, r2, #1
004fc788: eor      r1, r1, r0
004fc78c: strb     r1, [r3, #-1]
004fc790: add      r3, r3, #1
004fc794: blo      #0x4fc760
004fc798: mov      r0, r5
004fc79c: add      r1, r4, #0x94
004fc7a0: bl       #0x459090
004fc7a4: mov      r3, #1
004fc7a8: cmp      r3, #0
004fc7ac: str      r3, [sp, #4]
004fc7b0: bne      #0x4fc7f4
004fc7b4: add      r3, r4, #0x95
004fc7b8: add      r2, r4, #0x96
004fc7bc: ldrb     r0, [r2, #1]
004fc7c0: ldrb     r1, [r3, #-1]
004fc7c4: cmp      r3, r2
004fc7c8: eor      r1, r0, r1
004fc7cc: strb     r1, [r3, #-1]
004fc7d0: ldrb     r0, [r2, #1]
004fc7d4: eor      r1, r1, r0
004fc7d8: strb     r1, [r2, #1]
004fc7dc: ldrb     r0, [r3, #-1]
004fc7e0: sub      r2, r2, #1
004fc7e4: eor      r1, r1, r0
004fc7e8: strb     r1, [r3, #-1]
004fc7ec: add      r3, r3, #1
004fc7f0: blo      #0x4fc7bc
004fc7f4: mov      r0, r5
004fc7f8: add      r1, r4, #0x98
004fc7fc: bl       #0x459090
004fc800: mov      r3, #1
004fc804: cmp      r3, #0
004fc808: str      r3, [sp, #4]
004fc80c: bne      #0x4fc850
004fc810: add      r3, r4, #0x99
004fc814: add      r2, r4, #0x9a
004fc818: ldrb     r0, [r2, #1]
004fc81c: ldrb     r1, [r3, #-1]
004fc820: cmp      r3, r2
004fc824: eor      r1, r0, r1
004fc828: strb     r1, [r3, #-1]
004fc82c: ldrb     r0, [r2, #1]
004fc830: eor      r1, r1, r0
004fc834: strb     r1, [r2, #1]
004fc838: ldrb     r0, [r3, #-1]
004fc83c: sub      r2, r2, #1
004fc840: eor      r1, r1, r0
004fc844: strb     r1, [r3, #-1]
004fc848: add      r3, r3, #1
004fc84c: blo      #0x4fc818
004fc850: mov      r0, r5
004fc854: add      r1, r4, #0x9c
004fc858: bl       #0x459090
004fc85c: mov      r3, #1
004fc860: cmp      r3, #0
004fc864: str      r3, [sp, #4]
004fc868: bne      #0x4fc8ac
004fc86c: add      r3, r4, #0x9d
004fc870: add      r2, r4, #0x9e
004fc874: ldrb     r0, [r2, #1]
004fc878: ldrb     r1, [r3, #-1]
004fc87c: cmp      r3, r2
004fc880: eor      r1, r0, r1
004fc884: strb     r1, [r3, #-1]
004fc888: ldrb     r0, [r2, #1]
004fc88c: eor      r1, r1, r0
004fc890: strb     r1, [r2, #1]
004fc894: ldrb     r0, [r3, #-1]
004fc898: sub      r2, r2, #1
004fc89c: eor      r1, r1, r0
004fc8a0: strb     r1, [r3, #-1]
004fc8a4: add      r3, r3, #1
004fc8a8: blo      #0x4fc874
004fc8ac: mov      r0, r5
004fc8b0: add      r1, r4, #0xa0
004fc8b4: bl       #0x459090
004fc8b8: mov      r3, #1
004fc8bc: cmp      r3, #0
004fc8c0: str      r3, [sp, #4]
004fc8c4: bne      #0x4fc908
004fc8c8: add      r3, r4, #0xa2
004fc8cc: add      r4, r4, #0xa1
004fc8d0: ldrb     r1, [r3, #1]
004fc8d4: ldrb     r2, [r4, #-1]
004fc8d8: cmp      r4, r3
004fc8dc: eor      r2, r1, r2
004fc8e0: strb     r2, [r4, #-1]
004fc8e4: ldrb     r1, [r3, #1]
004fc8e8: eor      r2, r2, r1
004fc8ec: strb     r2, [r3, #1]
004fc8f0: ldrb     r1, [r4, #-1]
004fc8f4: sub      r3, r3, #1
004fc8f8: eor      r2, r2, r1
004fc8fc: strb     r2, [r4, #-1]
004fc900: add      r4, r4, #1
004fc904: blo      #0x4fc8d0
004fc908: add      sp, sp, #8
004fc90c: pop      {r4, r5, r6, pc}

# _ZN12StreamReader6readAsIjEEvP11IStreamBasePT_
003df1a0: str      lr, [sp, #-4]!
003df1a4: mov      r3, #0
003df1a8: sub      sp, sp, #0xc
003df1ac: ldr      ip, [r0]
003df1b0: mov      r2, #4
003df1b4: mov      lr, pc
003df1b8: ldr      pc, [ip, #0x18]
003df1bc: ldr      r3, [pc, #0x74]
003df1c0: cmp      r0, #4
003df1c4: add      r3, pc, r3
003df1c8: beq      #0x3df1f8
003df1cc: ldr      r2, [pc, #0x68]
003df1d0: ldr      r2, [r3, r2]
003df1d4: ldr      r2, [r2]
003df1d8: cmp      r2, #2
003df1dc: moveq    r3, #0
003df1e0: streq    r3, [r3]
003df1e4: beq      #0x3df1f0
003df1e8: cmp      r2, #1
003df1ec: beq      #0x3df204
003df1f0: add      sp, sp, #0xc
003df1f4: ldm      sp!, {pc}
003df1f8: cmp      r1, #0
003df1fc: beq      #0x3df1f0
003df200: b        #0x3df1cc
003df204: ldr      r0, [pc, #0x34]
003df208: ldr      r1, [pc, #0x34]
003df20c: ldr      r2, [pc, #0x34]
003df210: ldr      r0, [r3, r0]
003df214: ldr      r3, [pc, #0x30]
003df218: mov      ip, #0x50
003df21c: add      r1, pc, r1
003df220: add      r2, pc, r2
003df224: add      r3, pc, r3
003df228: add      r0, r0, #0xa8
003df22c: str      ip, [sp]
003df230: bl       #0x30e004
003df234: b        #0x3df1f0
003df238: subseq   r5, fp, ip, asr #17
003df23c: andeq    r3, r0, r0, asr #19
003df240: andeq    r1, r0, r0, asr #19
003df244: strheq   pc, [sp], #-0x1c
003df248: subeq    pc, sp, r0, ror #5
003df24c: subeq    r0, lr, ip, lsl fp

# _ZN6Arrays8ItemList9skipNamesEP11IStreamBase
004b4a54: b        #0x4b48bc

# _ZN7Structs17ItemListEntryList4readEP11IStreamBase
004dc790: push     {r4, r5, r6, r7, r8, lr}
004dc794: mov      r5, r0
004dc798: sub      sp, sp, #8
004dc79c: mov      r0, r1
004dc7a0: mov      r7, r1
004dc7a4: ldr      r6, [pc, #0x148]
004dc7a8: add      r1, r5, #4
004dc7ac: bl       #0x3df1a0
004dc7b0: mov      r3, #1
004dc7b4: cmp      r3, #0
004dc7b8: str      r3, [sp, #4]
004dc7bc: add      r6, pc, r6
004dc7c0: bne      #0x4dc804
004dc7c4: add      r3, r5, #5
004dc7c8: add      r2, r5, #6
004dc7cc: ldrb     r0, [r2, #1]
004dc7d0: ldrb     r1, [r3, #-1]
004dc7d4: cmp      r3, r2
004dc7d8: eor      r1, r0, r1
004dc7dc: strb     r1, [r3, #-1]
004dc7e0: ldrb     r0, [r2, #1]
004dc7e4: eor      r1, r1, r0
004dc7e8: strb     r1, [r2, #1]
004dc7ec: ldrb     r0, [r3, #-1]
004dc7f0: sub      r2, r2, #1
004dc7f4: eor      r1, r1, r0
004dc7f8: strb     r1, [r3, #-1]
004dc7fc: add      r3, r3, #1
004dc800: blo      #0x4dc7cc
004dc804: ldr      r3, [r5, #8]
004dc808: cmp      r3, #0
004dc80c: beq      #0x4dc854
004dc810: ldr      r2, [r3, #-4]
004dc814: mov      r0, #0xc
004dc818: mla      r0, r0, r2, r3
004dc81c: cmp      r3, r0
004dc820: bne      #0x4dc82c
004dc824: b        #0x4dc84c
004dc828: mov      r0, r4
004dc82c: sub      r4, r0, #0xc
004dc830: ldr      r3, [r0, #-0xc]
004dc834: mov      r0, r4
004dc838: mov      lr, pc
004dc83c: ldr      pc, [r3]
004dc840: ldr      r0, [r5, #8]
004dc844: cmp      r0, r4
004dc848: bne      #0x4dc828
004dc84c: sub      r0, r0, #8
004dc850: bl       #0x310440
004dc854: ldr      r4, [r5, #4]
004dc858: mov      r8, #0xc
004dc85c: mov      r1, #1
004dc860: mul      r0, r8, r4
004dc864: add      r0, r0, #8
004dc868: bl       #0x31056c
004dc86c: cmp      r4, #0
004dc870: str      r8, [r0]
004dc874: str      r4, [r0, #4]
004dc878: add      r3, r0, #8
004dc87c: beq      #0x4dc8a4
004dc880: ldr      r1, [pc, #0x70]
004dc884: mov      r2, #0
004dc888: ldr      r1, [r6, r1]
004dc88c: add      r1, r1, #8
004dc890: add      r2, r2, #1
004dc894: cmp      r2, r4
004dc898: str      r1, [r0, #8]
004dc89c: add      r0, r0, #0xc
004dc8a0: bne      #0x4dc890
004dc8a4: ldr      r2, [r5, #4]
004dc8a8: str      r3, [r5, #8]
004dc8ac: cmp      r2, #0
004dc8b0: beq      #0x4dc8ec
004dc8b4: mov      r4, #0
004dc8b8: mov      r6, r4
004dc8bc: b        #0x4dc8c4
004dc8c0: ldr      r3, [r5, #8]
004dc8c4: add      r0, r3, r4
004dc8c8: mov      r1, r7
004dc8cc: ldr      r3, [r3, r4]
004dc8d0: mov      lr, pc
004dc8d4: ldr      pc, [r3, #0xc]
004dc8d8: ldr      r3, [r5, #4]
004dc8dc: add      r6, r6, #1
004dc8e0: add      r4, r4, #0xc
004dc8e4: cmp      r3, r6
004dc8e8: bhi      #0x4dc8c0
004dc8ec: add      sp, sp, #8
004dc8f0: pop      {r4, r5, r6, r7, r8, pc}
004dc8f4: ldrdeq   r8, sb, [fp], #-0x24
004dc8f8: andeq    r3, r0, r8, lsr #2

# _ZN12StreamReader6readAsIaEEvP11IStreamBasePT_
004db9fc: str      lr, [sp, #-4]!
004dba00: mov      r3, #0
004dba04: sub      sp, sp, #0xc
004dba08: ldr      ip, [r0]
004dba0c: mov      r2, #1
004dba10: mov      lr, pc
004dba14: ldr      pc, [ip, #0x18]
004dba18: ldr      r3, [pc, #0x74]
004dba1c: cmp      r0, #1
004dba20: add      r3, pc, r3
004dba24: beq      #0x4dba54
004dba28: ldr      r2, [pc, #0x68]
004dba2c: ldr      r2, [r3, r2]
004dba30: ldr      r2, [r2]
004dba34: cmp      r2, #2
004dba38: moveq    r3, #0
004dba3c: streq    r3, [r3]
004dba40: beq      #0x4dba4c
004dba44: cmp      r2, #1
004dba48: beq      #0x4dba60
004dba4c: add      sp, sp, #0xc
004dba50: ldm      sp!, {pc}
004dba54: cmp      r1, #0
004dba58: beq      #0x4dba4c
004dba5c: b        #0x4dba28
004dba60: ldr      r0, [pc, #0x34]
004dba64: ldr      r1, [pc, #0x34]
004dba68: ldr      r2, [pc, #0x34]
004dba6c: ldr      r0, [r3, r0]
004dba70: ldr      r3, [pc, #0x30]
004dba74: mov      ip, #0x50
004dba78: add      r1, pc, r1
004dba7c: add      r2, pc, r2
004dba80: add      r3, pc, r3
004dba84: add      r0, r0, #0xa8
004dba88: str      ip, [sp]
004dba8c: bl       #0x30e004
004dba90: b        #0x4dba4c
004dba94: subeq    sb, fp, r0, ror r0
004dba98: andeq    r3, r0, r0, asr #19
004dba9c: andeq    r1, r0, r0, asr #19
004dbaa0: eorseq   r2, lr, r0, ror #18
004dbaa4: eorseq   r2, lr, r4, lsl #21
004dbaa8: eorseq   r4, lr, r0, asr #5

# _ZN12StreamReader6readAsIfEEvP11IStreamBasePT_
004db94c: str      lr, [sp, #-4]!
004db950: mov      r3, #0
004db954: sub      sp, sp, #0xc
004db958: ldr      ip, [r0]
004db95c: mov      r2, #4
004db960: mov      lr, pc
004db964: ldr      pc, [ip, #0x18]
004db968: ldr      r3, [pc, #0x74]
004db96c: cmp      r0, #4
004db970: add      r3, pc, r3
004db974: beq      #0x4db9a4
004db978: ldr      r2, [pc, #0x68]
004db97c: ldr      r2, [r3, r2]
004db980: ldr      r2, [r2]
004db984: cmp      r2, #2
004db988: moveq    r3, #0
004db98c: streq    r3, [r3]
004db990: beq      #0x4db99c
004db994: cmp      r2, #1
004db998: beq      #0x4db9b0
004db99c: add      sp, sp, #0xc
004db9a0: ldm      sp!, {pc}
004db9a4: cmp      r1, #0
004db9a8: beq      #0x4db99c
004db9ac: b        #0x4db978
004db9b0: ldr      r0, [pc, #0x34]
004db9b4: ldr      r1, [pc, #0x34]
004db9b8: ldr      r2, [pc, #0x34]
004db9bc: ldr      r0, [r3, r0]
004db9c0: ldr      r3, [pc, #0x30]
004db9c4: mov      ip, #0x50
004db9c8: add      r1, pc, r1
004db9cc: add      r2, pc, r2
004db9d0: add      r3, pc, r3
004db9d4: add      r0, r0, #0xa8
004db9d8: str      ip, [sp]
004db9dc: bl       #0x30e004
004db9e0: b        #0x4db99c
004db9e4: subeq    sb, fp, r0, lsr #2
004db9e8: andeq    r3, r0, r0, asr #19
004db9ec: andeq    r1, r0, r0, asr #19
004db9f0: eorseq   r2, lr, r0, lsl sl
004db9f4: eorseq   r2, lr, r4, lsr fp
004db9f8: eorseq   r4, lr, r0, ror r3

# _ZN7Structs8ItemBase4readEP11IStreamBase
004ec47c: push     {r4, r5, r6, lr}
004ec480: mov      r4, r0
004ec484: sub      sp, sp, #8
004ec488: mov      r0, r1
004ec48c: mov      r5, r1
004ec490: add      r1, r4, #4
004ec494: bl       #0x3df1a0
004ec498: mov      r3, #1
004ec49c: cmp      r3, #0
004ec4a0: str      r3, [sp, #4]
004ec4a4: bne      #0x4ec4e8
004ec4a8: add      r3, r4, #5
004ec4ac: add      r2, r4, #6
004ec4b0: ldrb     r0, [r2, #1]
004ec4b4: ldrb     r1, [r3, #-1]
004ec4b8: cmp      r3, r2
004ec4bc: eor      r1, r0, r1
004ec4c0: strb     r1, [r3, #-1]
004ec4c4: ldrb     r0, [r2, #1]
004ec4c8: eor      r1, r1, r0
004ec4cc: strb     r1, [r2, #1]
004ec4d0: ldrb     r0, [r3, #-1]
004ec4d4: sub      r2, r2, #1
004ec4d8: eor      r1, r1, r0
004ec4dc: strb     r1, [r3, #-1]
004ec4e0: add      r3, r3, #1
004ec4e4: blo      #0x4ec4b0
004ec4e8: ldr      r0, [r4, #8]
004ec4ec: cmp      r0, #0
004ec4f0: beq      #0x4ec4f8
004ec4f4: bl       #0x310440
004ec4f8: ldr      r0, [r4, #4]
004ec4fc: mov      r1, #1
004ec500: mov      r6, #0
004ec504: add      r0, r0, r1
004ec508: bl       #0x31056c
004ec50c: ldr      r2, [r4, #4]
004ec510: mov      r1, r0
004ec514: str      r0, [r4, #8]
004ec518: mov      r3, r6
004ec51c: mov      r0, r5
004ec520: bl       #0x317454
004ec524: ldr      r3, [r4, #4]
004ec528: ldr      r2, [r4, #8]
004ec52c: mov      r0, r5
004ec530: add      r1, r4, #0xc
004ec534: strb     r6, [r2, r3]
004ec538: bl       #0x459090
004ec53c: mov      r3, #1
004ec540: cmp      r3, r6
004ec544: str      r3, [sp, #4]
004ec548: bne      #0x4ec58c
004ec54c: add      r3, r4, #0xd
004ec550: add      r2, r4, #0xe
004ec554: ldrb     r0, [r2, #1]
004ec558: ldrb     r1, [r3, #-1]
004ec55c: cmp      r3, r2
004ec560: eor      r1, r0, r1
004ec564: strb     r1, [r3, #-1]
004ec568: ldrb     r0, [r2, #1]
004ec56c: eor      r1, r1, r0
004ec570: strb     r1, [r2, #1]
004ec574: ldrb     r0, [r3, #-1]
004ec578: sub      r2, r2, #1
004ec57c: eor      r1, r1, r0
004ec580: strb     r1, [r3, #-1]
004ec584: add      r3, r3, #1
004ec588: blo      #0x4ec554
004ec58c: mov      r0, r5
004ec590: add      r1, r4, #0x10
004ec594: bl       #0x459090
004ec598: mov      r3, #1
004ec59c: cmp      r3, #0
004ec5a0: str      r3, [sp, #4]
004ec5a4: bne      #0x4ec5e8
004ec5a8: add      r3, r4, #0x11
004ec5ac: add      r2, r4, #0x12
004ec5b0: ldrb     r0, [r2, #1]
004ec5b4: ldrb     r1, [r3, #-1]
004ec5b8: cmp      r3, r2
004ec5bc: eor      r1, r0, r1
004ec5c0: strb     r1, [r3, #-1]
004ec5c4: ldrb     r0, [r2, #1]
004ec5c8: eor      r1, r1, r0
004ec5cc: strb     r1, [r2, #1]
004ec5d0: ldrb     r0, [r3, #-1]
004ec5d4: sub      r2, r2, #1
004ec5d8: eor      r1, r1, r0
004ec5dc: strb     r1, [r3, #-1]
004ec5e0: add      r3, r3, #1
004ec5e4: blo      #0x4ec5b0
004ec5e8: mov      r0, r5
004ec5ec: add      r1, r4, #0x14
004ec5f0: bl       #0x459090
004ec5f4: mov      r3, #1
004ec5f8: cmp      r3, #0
004ec5fc: str      r3, [sp, #4]
004ec600: bne      #0x4ec644
004ec604: add      r3, r4, #0x15
004ec608: add      r2, r4, #0x16
004ec60c: ldrb     r0, [r2, #1]
004ec610: ldrb     r1, [r3, #-1]
004ec614: cmp      r3, r2
004ec618: eor      r1, r0, r1
004ec61c: strb     r1, [r3, #-1]
004ec620: ldrb     r0, [r2, #1]
004ec624: eor      r1, r1, r0
004ec628: strb     r1, [r2, #1]
004ec62c: ldrb     r0, [r3, #-1]
004ec630: sub      r2, r2, #1
004ec634: eor      r1, r1, r0
004ec638: strb     r1, [r3, #-1]
004ec63c: add      r3, r3, #1
004ec640: blo      #0x4ec60c
004ec644: mov      r0, r5
004ec648: add      r1, r4, #0x18
004ec64c: bl       #0x459090
004ec650: mov      r3, #1
004ec654: cmp      r3, #0
004ec658: str      r3, [sp, #4]
004ec65c: bne      #0x4ec6a0
004ec660: add      r3, r4, #0x19
004ec664: add      r2, r4, #0x1a
004ec668: ldrb     r0, [r2, #1]
004ec66c: ldrb     r1, [r3, #-1]
004ec670: cmp      r3, r2
004ec674: eor      r1, r0, r1
004ec678: strb     r1, [r3, #-1]
004ec67c: ldrb     r0, [r2, #1]
004ec680: eor      r1, r1, r0
004ec684: strb     r1, [r2, #1]
004ec688: ldrb     r0, [r3, #-1]
004ec68c: sub      r2, r2, #1
004ec690: eor      r1, r1, r0
004ec694: strb     r1, [r3, #-1]
004ec698: add      r3, r3, #1
004ec69c: blo      #0x4ec668
004ec6a0: add      r1, r4, #0x1c
004ec6a4: mov      r0, r5
004ec6a8: bl       #0x4db89c
004ec6ac: mov      r0, r5
004ec6b0: add      r1, r4, #0x20
004ec6b4: bl       #0x4db94c
004ec6b8: mov      r3, #1
004ec6bc: cmp      r3, #0
004ec6c0: str      r3, [sp, #4]
004ec6c4: bne      #0x4ec708
004ec6c8: add      r3, r4, #0x21
004ec6cc: add      r2, r4, #0x22
004ec6d0: ldrb     r0, [r2, #1]
004ec6d4: ldrb     r1, [r3, #-1]
004ec6d8: cmp      r3, r2
004ec6dc: eor      r1, r0, r1
004ec6e0: strb     r1, [r3, #-1]
004ec6e4: ldrb     r0, [r2, #1]
004ec6e8: eor      r1, r1, r0
004ec6ec: strb     r1, [r2, #1]
004ec6f0: ldrb     r0, [r3, #-1]
004ec6f4: sub      r2, r2, #1
004ec6f8: eor      r1, r1, r0
004ec6fc: strb     r1, [r3, #-1]
004ec700: add      r3, r3, #1
004ec704: blo      #0x4ec6d0
004ec708: mov      r0, r5
004ec70c: add      r1, r4, #0x24
004ec710: bl       #0x4db94c
004ec714: mov      r3, #1
004ec718: cmp      r3, #0
004ec71c: str      r3, [sp, #4]
004ec720: bne      #0x4ec764
004ec724: add      r3, r4, #0x25
004ec728: add      r2, r4, #0x26
004ec72c: ldrb     r0, [r2, #1]
004ec730: ldrb     r1, [r3, #-1]
004ec734: cmp      r3, r2
004ec738: eor      r1, r0, r1
004ec73c: strb     r1, [r3, #-1]
004ec740: ldrb     r0, [r2, #1]
004ec744: eor      r1, r1, r0
004ec748: strb     r1, [r2, #1]
004ec74c: ldrb     r0, [r3, #-1]
004ec750: sub      r2, r2, #1
004ec754: eor      r1, r1, r0
004ec758: strb     r1, [r3, #-1]
004ec75c: add      r3, r3, #1
004ec760: blo      #0x4ec72c
004ec764: mov      r0, r5
004ec768: add      r1, r4, #0x28
004ec76c: bl       #0x4db94c
004ec770: mov      r3, #1
004ec774: cmp      r3, #0
004ec778: str      r3, [sp, #4]
004ec77c: bne      #0x4ec7c0
004ec780: add      r3, r4, #0x29
004ec784: add      r2, r4, #0x2a
004ec788: ldrb     r0, [r2, #1]
004ec78c: ldrb     r1, [r3, #-1]
004ec790: cmp      r3, r2
004ec794: eor      r1, r0, r1
004ec798: strb     r1, [r3, #-1]
004ec79c: ldrb     r0, [r2, #1]
004ec7a0: eor      r1, r1, r0
004ec7a4: strb     r1, [r2, #1]
004ec7a8: ldrb     r0, [r3, #-1]
004ec7ac: sub      r2, r2, #1
004ec7b0: eor      r1, r1, r0
004ec7b4: strb     r1, [r3, #-1]
004ec7b8: add      r3, r3, #1
004ec7bc: blo      #0x4ec788
004ec7c0: mov      r0, r5
004ec7c4: add      r1, r4, #0x2c
004ec7c8: bl       #0x4db94c
004ec7cc: mov      r3, #1
004ec7d0: cmp      r3, #0
004ec7d4: str      r3, [sp, #4]
004ec7d8: bne      #0x4ec81c
004ec7dc: add      r3, r4, #0x2d
004ec7e0: add      r2, r4, #0x2e
004ec7e4: ldrb     r0, [r2, #1]
004ec7e8: ldrb     r1, [r3, #-1]
004ec7ec: cmp      r3, r2
004ec7f0: eor      r1, r0, r1
004ec7f4: strb     r1, [r3, #-1]
004ec7f8: ldrb     r0, [r2, #1]
004ec7fc: eor      r1, r1, r0
004ec800: strb     r1, [r2, #1]
004ec804: ldrb     r0, [r3, #-1]
004ec808: sub      r2, r2, #1
004ec80c: eor      r1, r1, r0
004ec810: strb     r1, [r3, #-1]
004ec814: add      r3, r3, #1
004ec818: blo      #0x4ec7e4
004ec81c: mov      r0, r5
004ec820: add      r1, r4, #0x30
004ec824: bl       #0x4db94c
004ec828: mov      r3, #1
004ec82c: cmp      r3, #0
004ec830: str      r3, [sp, #4]
004ec834: bne      #0x4ec878
004ec838: add      r3, r4, #0x31
004ec83c: add      r2, r4, #0x32
004ec840: ldrb     r0, [r2, #1]
004ec844: ldrb     r1, [r3, #-1]
004ec848: cmp      r3, r2
004ec84c: eor      r1, r0, r1
004ec850: strb     r1, [r3, #-1]
004ec854: ldrb     r0, [r2, #1]
004ec858: eor      r1, r1, r0
004ec85c: strb     r1, [r2, #1]
004ec860: ldrb     r0, [r3, #-1]
004ec864: sub      r2, r2, #1
004ec868: eor      r1, r1, r0
004ec86c: strb     r1, [r3, #-1]
004ec870: add      r3, r3, #1
004ec874: blo      #0x4ec840
004ec878: mov      r0, r5
004ec87c: add      r1, r4, #0x34
004ec880: bl       #0x4db94c
004ec884: mov      r3, #1
004ec888: cmp      r3, #0
004ec88c: str      r3, [sp, #4]
004ec890: bne      #0x4ec8d4
004ec894: add      r3, r4, #0x35
004ec898: add      r2, r4, #0x36
004ec89c: ldrb     r0, [r2, #1]
004ec8a0: ldrb     r1, [r3, #-1]
004ec8a4: cmp      r3, r2
004ec8a8: eor      r1, r0, r1
004ec8ac: strb     r1, [r3, #-1]
004ec8b0: ldrb     r0, [r2, #1]
004ec8b4: eor      r1, r1, r0
004ec8b8: strb     r1, [r2, #1]
004ec8bc: ldrb     r0, [r3, #-1]
004ec8c0: sub      r2, r2, #1
004ec8c4: eor      r1, r1, r0
004ec8c8: strb     r1, [r3, #-1]
004ec8cc: add      r3, r3, #1
004ec8d0: blo      #0x4ec89c
004ec8d4: mov      r0, r5
004ec8d8: add      r1, r4, #0x38
004ec8dc: bl       #0x4db94c
004ec8e0: mov      r3, #1
004ec8e4: cmp      r3, #0
004ec8e8: str      r3, [sp, #4]
004ec8ec: bne      #0x4ec930
004ec8f0: add      r3, r4, #0x39
004ec8f4: add      r2, r4, #0x3a
004ec8f8: ldrb     r0, [r2, #1]
004ec8fc: ldrb     r1, [r3, #-1]
004ec900: cmp      r3, r2
004ec904: eor      r1, r0, r1
004ec908: strb     r1, [r3, #-1]
004ec90c: ldrb     r0, [r2, #1]
004ec910: eor      r1, r1, r0
004ec914: strb     r1, [r2, #1]
004ec918: ldrb     r0, [r3, #-1]
004ec91c: sub      r2, r2, #1
004ec920: eor      r1, r1, r0
004ec924: strb     r1, [r3, #-1]
004ec928: add      r3, r3, #1
004ec92c: blo      #0x4ec8f8
004ec930: mov      r0, r5
004ec934: add      r1, r4, #0x3c
004ec938: bl       #0x4db94c
004ec93c: mov      r3, #1
004ec940: cmp      r3, #0
004ec944: str      r3, [sp, #4]
004ec948: bne      #0x4ec98c
004ec94c: add      r3, r4, #0x3d
004ec950: add      r2, r4, #0x3e
004ec954: ldrb     r0, [r2, #1]
004ec958: ldrb     r1, [r3, #-1]
004ec95c: cmp      r3, r2
004ec960: eor      r1, r0, r1
004ec964: strb     r1, [r3, #-1]
004ec968: ldrb     r0, [r2, #1]
004ec96c: eor      r1, r1, r0
004ec970: strb     r1, [r2, #1]
004ec974: ldrb     r0, [r3, #-1]
004ec978: sub      r2, r2, #1
004ec97c: eor      r1, r1, r0
004ec980: strb     r1, [r3, #-1]
004ec984: add      r3, r3, #1
004ec988: blo      #0x4ec954
004ec98c: mov      r0, r5
004ec990: add      r1, r4, #0x40
004ec994: bl       #0x4db94c
004ec998: mov      r3, #1
004ec99c: cmp      r3, #0
004ec9a0: str      r3, [sp, #4]
004ec9a4: bne      #0x4ec9e8
004ec9a8: add      r3, r4, #0x42
004ec9ac: add      r4, r4, #0x41
004ec9b0: ldrb     r1, [r3, #1]
004ec9b4: ldrb     r2, [r4, #-1]
004ec9b8: cmp      r4, r3
004ec9bc: eor      r2, r1, r2
004ec9c0: strb     r2, [r4, #-1]
004ec9c4: ldrb     r1, [r3, #1]
004ec9c8: eor      r2, r2, r1
004ec9cc: strb     r2, [r3, #1]
004ec9d0: ldrb     r1, [r4, #-1]
004ec9d4: sub      r3, r3, #1
004ec9d8: eor      r2, r2, r1
004ec9dc: strb     r2, [r4, #-1]
004ec9e0: add      r4, r4, #1
004ec9e4: blo      #0x4ec9b0
004ec9e8: add      sp, sp, #8
004ec9ec: pop      {r4, r5, r6, pc}

# _ZNK13ItemInventory18GetCurrentEquipSetEi
003fc6a8: cmp      r1, #0
003fc6ac: blt      #0x3fc6c0
003fc6b0: sub      r1, r1, #1
003fc6b4: cmp      r1, #1
003fc6b8: movhi    r0, #0
003fc6bc: bxhi     lr
003fc6c0: ldrsb    r0, [r0, #0x2e]
003fc6c4: bx       lr

# _ZN7Structs13ItemListEntry4readEP11IStreamBase
004ed280: push     {r4, r5, r6, lr}
004ed284: mov      r4, r0
004ed288: sub      sp, sp, #8
004ed28c: mov      r0, r1
004ed290: mov      r5, r1
004ed294: add      r1, r4, #4
004ed298: bl       #0x459090
004ed29c: mov      r3, #1
004ed2a0: cmp      r3, #0
004ed2a4: str      r3, [sp, #4]
004ed2a8: bne      #0x4ed2ec
004ed2ac: add      r3, r4, #5
004ed2b0: add      r2, r4, #6
004ed2b4: ldrb     r0, [r2, #1]
004ed2b8: ldrb     r1, [r3, #-1]
004ed2bc: cmp      r3, r2
004ed2c0: eor      r1, r0, r1
004ed2c4: strb     r1, [r3, #-1]
004ed2c8: ldrb     r0, [r2, #1]
004ed2cc: eor      r1, r1, r0
004ed2d0: strb     r1, [r2, #1]
004ed2d4: ldrb     r0, [r3, #-1]
004ed2d8: sub      r2, r2, #1
004ed2dc: eor      r1, r1, r0
004ed2e0: strb     r1, [r3, #-1]
004ed2e4: add      r3, r3, #1
004ed2e8: blo      #0x4ed2b4
004ed2ec: add      r6, r4, #8
004ed2f0: mov      r0, r5
004ed2f4: mov      r1, r6
004ed2f8: bl       #0x4dbaac
004ed2fc: mov      r3, #1
004ed300: cmp      r3, #0
004ed304: str      r3, [sp, #4]
004ed308: bne      #0x4ed348
004ed30c: add      r3, r4, #9
004ed310: ldrb     r1, [r6, #1]
004ed314: ldrb     r2, [r3, #-1]
004ed318: cmp      r3, r6
004ed31c: eor      r2, r1, r2
004ed320: strb     r2, [r3, #-1]
004ed324: ldrb     r1, [r6, #1]
004ed328: eor      r2, r2, r1
004ed32c: strb     r2, [r6, #1]
004ed330: ldrb     r1, [r3, #-1]
004ed334: sub      r6, r6, #1
004ed338: eor      r2, r2, r1
004ed33c: strb     r2, [r3, #-1]
004ed340: add      r3, r3, #1
004ed344: blo      #0x4ed310
004ed348: mov      r0, r5
004ed34c: add      r1, r4, #0xa
004ed350: bl       #0x4db9fc
004ed354: add      sp, sp, #8
004ed358: pop      {r4, r5, r6, pc}

# _ZN6Arrays14InventoryTable4readEP11IStreamBase
004ba3c8: push     {r4, r5, r6, r7, r8, lr}
004ba3cc: sub      sp, sp, #8
004ba3d0: mov      r8, r0
004ba3d4: bl       #0x313a90
004ba3d8: ldr      r5, [pc, #0x10c]
004ba3dc: mov      r3, #1
004ba3e0: cmp      r3, #0
004ba3e4: str      r0, [sp, #4]
004ba3e8: str      r3, [sp]
004ba3ec: add      r5, pc, r5
004ba3f0: bne      #0x4ba438
004ba3f4: add      r3, sp, #4
004ba3f8: add      r2, r3, #2
004ba3fc: add      r3, r3, #1
004ba400: ldrb     r0, [r2, #1]
004ba404: ldrb     r1, [r3, #-1]
004ba408: cmp      r2, r3
004ba40c: eor      r1, r0, r1
004ba410: strb     r1, [r3, #-1]
004ba414: ldrb     r0, [r2, #1]
004ba418: eor      r1, r1, r0
004ba41c: strb     r1, [r2, #1]
004ba420: ldrb     r0, [r3, #-1]
004ba424: sub      r2, r2, #1
004ba428: eor      r1, r1, r0
004ba42c: strb     r1, [r3, #-1]
004ba430: add      r3, r3, #1
004ba434: bhi      #0x4ba400
004ba438: ldr      r6, [pc, #0xb0]
004ba43c: bl       #0x4a6280
004ba440: ldr      r4, [sp, #4]
004ba444: ldr      r3, [r5, r6]
004ba448: mov      r1, #1
004ba44c: add      r0, r4, r1
004ba450: str      r4, [r3]
004ba454: lsl      r0, r0, #3
004ba458: bl       #0x31056c
004ba45c: mov      r3, #8
004ba460: cmp      r4, #0
004ba464: stm      r0, {r3, r4}
004ba468: add      r3, r0, r3
004ba46c: beq      #0x4ba490
004ba470: ldr      r1, [pc, #0x7c]
004ba474: mov      r2, #0
004ba478: ldr      r1, [r5, r1]
004ba47c: add      r1, r1, #8
004ba480: add      r2, r2, #1
004ba484: cmp      r2, r4
004ba488: str      r1, [r0, #8]!
004ba48c: bne      #0x4ba480
004ba490: ldr      r2, [r5, r6]
004ba494: ldr      r7, [pc, #0x5c]
004ba498: ldr      r1, [r2]
004ba49c: ldr      r2, [r5, r7]
004ba4a0: cmp      r1, #0
004ba4a4: str      r3, [r2]
004ba4a8: beq      #0x4ba4e4
004ba4ac: mov      r4, #0
004ba4b0: b        #0x4ba4bc
004ba4b4: ldr      r3, [r5, r7]
004ba4b8: ldr      r3, [r3]
004ba4bc: add      r0, r3, r4, lsl #3
004ba4c0: mov      r1, r8
004ba4c4: ldr      r3, [r3, r4, lsl #3]
004ba4c8: mov      lr, pc
004ba4cc: ldr      pc, [r3, #0xc]
004ba4d0: ldr      r3, [r5, r6]
004ba4d4: add      r4, r4, #1
004ba4d8: ldr      r3, [r3]
004ba4dc: cmp      r3, r4
004ba4e0: bhi      #0x4ba4b4
004ba4e4: add      sp, sp, #8
004ba4e8: pop      {r4, r5, r6, r7, r8, pc}
004ba4ec: subeq    sl, sp, r4, lsr #13
004ba4f0: andeq    r0, r0, ip, ror #15
004ba4f4: andeq    r2, r0, r4, ror #23
004ba4f8: muleq    r0, r0, fp

# _ZN6Arrays21DropTilePriorityTable9skipNamesEP11IStreamBase
004b53f4: b        #0x4b525c

# _ZN6Arrays14InventoryTable9skipNamesEP11IStreamBase
004b5258: b        #0x4b50c0

# _ZN6Arrays9ItemTable9readNamesEP11IStreamBase
004b4724: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004b4728: mov      r7, r0
004b472c: sub      sp, sp, #0x1c
004b4730: bl       #0x4a5ee4
004b4734: mov      r0, r7
004b4738: bl       #0x313a90
004b473c: ldr      r6, [pc, #0x16c]
004b4740: mov      r3, #1
004b4744: cmp      r3, #0
004b4748: add      r6, pc, r6
004b474c: str      r0, [sp, #0x14]
004b4750: str      r3, [sp, #0xc]
004b4754: bne      #0x4b47a4
004b4758: add      r3, sp, #0x14
004b475c: add      r2, r3, #2
004b4760: add      r3, r3, #1
004b4764: ldrb     r0, [r2, #1]
004b4768: ldrb     r1, [r3, #-1]
004b476c: cmp      r2, r3
004b4770: mov      r4, r2
004b4774: eor      r1, r0, r1
004b4778: strb     r1, [r3, #-1]
004b477c: ldrb     r0, [r2, #1]
004b4780: eor      r1, r1, r0
004b4784: strb     r1, [r2, #1]
004b4788: ldrb     r0, [r3, #-1]
004b478c: sub      r2, r2, #1
004b4790: eor      r1, r1, r0
004b4794: strb     r1, [r3, #-1]
004b4798: add      r3, r3, #1
004b479c: bhi      #0x4b4764
004b47a0: ldr      r0, [sp, #0x14]
004b47a4: ldr      r3, [pc, #0x108]
004b47a8: ldr      r3, [r6, r3]
004b47ac: ldr      r3, [r3]
004b47b0: cmp      r3, r0
004b47b4: beq      #0x4b47c0
004b47b8: add      sp, sp, #0x1c
004b47bc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004b47c0: lsl      r0, r0, #2
004b47c4: mov      r1, #1
004b47c8: bl       #0x31056c
004b47cc: ldr      sb, [pc, #0xe4]
004b47d0: ldr      r2, [sp, #0x14]
004b47d4: ldr      r3, [r6, sb]
004b47d8: cmp      r2, #0
004b47dc: str      r0, [r3]
004b47e0: beq      #0x4b47b8
004b47e4: add      sl, sp, #0x10
004b47e8: mov      r8, #1
004b47ec: add      r1, sl, r8
004b47f0: add      r3, sl, #2
004b47f4: mov      r4, #0
004b47f8: stm      sp, {r1, r3}
004b47fc: mov      r0, r7
004b4800: mov      r1, sl
004b4804: bl       #0x3df1a0
004b4808: cmp      r8, #0
004b480c: str      r8, [sp, #0xc]
004b4810: bne      #0x4b4854
004b4814: ldr      r3, [sp]
004b4818: ldr      r2, [sp, #4]
004b481c: ldrb     r0, [r2, #1]
004b4820: ldrb     r1, [r3, #-1]
004b4824: cmp      r2, r3
004b4828: eor      r1, r0, r1
004b482c: strb     r1, [r3, #-1]
004b4830: ldrb     r0, [r2, #1]
004b4834: eor      r1, r1, r0
004b4838: strb     r1, [r2, #1]
004b483c: ldrb     r0, [r3, #-1]
004b4840: sub      r2, r2, #1
004b4844: eor      r1, r1, r0
004b4848: strb     r1, [r3, #-1]
004b484c: add      r3, r3, #1
004b4850: bhi      #0x4b481c
004b4854: ldr      r0, [sp, #0x10]
004b4858: ldr      r5, [r6, sb]
004b485c: mov      r1, #1
004b4860: add      r0, r0, r1
004b4864: ldr      fp, [r5]
004b4868: bl       #0x31056c
004b486c: str      r0, [fp, r4, lsl #2]
004b4870: ldr      r3, [r5]
004b4874: ldr      r2, [sp, #0x10]
004b4878: mov      r0, r7
004b487c: ldr      r1, [r3, r4, lsl #2]
004b4880: mov      r3, #0
004b4884: bl       #0x317454
004b4888: ldr      r3, [r5]
004b488c: mov      r1, #0
004b4890: ldr      r2, [r3, r4, lsl #2]
004b4894: ldr      r3, [sp, #0x10]
004b4898: add      r4, r4, #1
004b489c: strb     r1, [r2, r3]
004b48a0: ldr      r3, [sp, #0x14]
004b48a4: cmp      r3, r4
004b48a8: bhi      #0x4b47fc
004b48ac: b        #0x4b47b8
004b48b0: subeq    r0, lr, r8, asr #6
004b48b4: andeq    r0, r0, r0, ror #26
004b48b8: andeq    r1, r0, r4, asr ip

# _ZN12StreamReader6readAsIjEET_P11IStreamBase
00313a90: str      lr, [sp, #-4]!
00313a94: sub      sp, sp, #0x14
00313a98: mov      r3, #0
00313a9c: ldr      ip, [r0]
00313aa0: add      r1, sp, #0xc
00313aa4: mov      r2, #4
00313aa8: mov      lr, pc
00313aac: ldr      pc, [ip, #0x18]
00313ab0: ldr      r3, [pc, #0x78]
00313ab4: cmp      r0, #4
00313ab8: add      r3, pc, r3
00313abc: beq      #0x313af0
00313ac0: ldr      r2, [pc, #0x6c]
00313ac4: ldr      r2, [r3, r2]
00313ac8: ldr      r2, [r2]
00313acc: cmp      r2, #2
00313ad0: moveq    r3, #0
00313ad4: streq    r3, [r3]
00313ad8: beq      #0x313ae4
00313adc: cmp      r2, #1
00313ae0: beq      #0x313afc
00313ae4: ldr      r0, [sp, #0xc]
00313ae8: add      sp, sp, #0x14
00313aec: ldm      sp!, {pc}
00313af0: cmp      r1, #0
00313af4: beq      #0x313ae4
00313af8: b        #0x313ac0
00313afc: ldr      r0, [pc, #0x34]
00313b00: ldr      r1, [pc, #0x34]
00313b04: ldr      r2, [pc, #0x34]
00313b08: ldr      r0, [r3, r0]
00313b0c: ldr      r3, [pc, #0x30]
00313b10: mov      ip, #0x44
00313b14: add      r1, pc, r1
00313b18: add      r2, pc, r2
00313b1c: add      r3, pc, r3
00313b20: add      r0, r0, #0xa8
00313b24: str      ip, [sp]
00313b28: bl       #0x30e004
00313b2c: b        #0x313ae4

# _ZN12StreamReader6readAsIiEEvP11IStreamBasePT_
00459090: str      lr, [sp, #-4]!
00459094: mov      r3, #0
00459098: sub      sp, sp, #0xc
0045909c: ldr      ip, [r0]
004590a0: mov      r2, #4
004590a4: mov      lr, pc
004590a8: ldr      pc, [ip, #0x18]
004590ac: ldr      r3, [pc, #0x74]
004590b0: cmp      r0, #4
004590b4: add      r3, pc, r3
004590b8: beq      #0x4590e8
004590bc: ldr      r2, [pc, #0x68]
004590c0: ldr      r2, [r3, r2]
004590c4: ldr      r2, [r2]
004590c8: cmp      r2, #2
004590cc: moveq    r3, #0
004590d0: streq    r3, [r3]
004590d4: beq      #0x4590e0
004590d8: cmp      r2, #1
004590dc: beq      #0x4590f4
004590e0: add      sp, sp, #0xc
004590e4: ldm      sp!, {pc}
004590e8: cmp      r1, #0
004590ec: beq      #0x4590e0
004590f0: b        #0x4590bc
004590f4: ldr      r0, [pc, #0x34]
004590f8: ldr      r1, [pc, #0x34]
004590fc: ldr      r2, [pc, #0x34]
00459100: ldr      r0, [r3, r0]
00459104: ldr      r3, [pc, #0x30]
00459108: mov      ip, #0x50
0045910c: add      r1, pc, r1
00459110: add      r2, pc, r2
00459114: add      r3, pc, r3
00459118: add      r0, r0, #0xa8
0045911c: str      ip, [sp]
00459120: bl       #0x30e004
00459124: b        #0x4590e0
00459128: ldrsbeq  fp, [r3], #-0x9c
0045912c: andeq    r3, r0, r0, asr #19
00459130: andeq    r1, r0, r0, asr #19
00459134: subeq    r5, r6, ip, asr #5
00459138: strdeq   r5, r6, [r6], #-0x30
0045913c: subeq    r6, r6, ip, lsr #24

# _ZNK13ItemInventory14CanRangeAttackEv
00400014: b        #0x3fffa4
