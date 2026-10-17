
# _ZNK13ItemInventory15IsInventoryFullEv
003fe330: push     {r4, r5, r6, r7, r8, lr}
003fe334: ldr      r4, [pc, #0xfc]
003fe338: ldr      r6, [pc, #0xfc]
003fe33c: ldr      r2, [pc, #0xfc]
003fe340: add      r4, pc, r4
003fe344: ldr      r3, [r4, r6]
003fe348: ldr      r7, [r4, r2]
003fe34c: sub      sp, sp, #0x20
003fe350: ldr      r3, [r3]
003fe354: add      r5, sp, #4
003fe358: mov      r8, r0
003fe35c: mov      r0, r7
003fe360: str      r3, [sp, #0x1c]
003fe364: bl       #0x337888
003fe368: mov      r0, r5
003fe36c: mov      r1, #0x12
003fe370: str      r5, [sp, #0x14]
003fe374: str      r5, [sp, #0x18]
003fe378: bl       #0x31167c
003fe37c: ldr      r1, [pc, #0xc0]
003fe380: mov      r2, #0x11
003fe384: ldr      r0, [sp, #0x18]
003fe388: add      r1, pc, r1
003fe38c: bl       #0x30e868
003fe390: add      r3, r0, #0x11
003fe394: str      r3, [sp, #0x14]
003fe398: mov      r3, #0
003fe39c: strb     r3, [r0, #0x11]
003fe3a0: mov      r1, r5
003fe3a4: mov      r0, r7
003fe3a8: bl       #0x337a88
003fe3ac: mov      r7, r0
003fe3b0: ldr      r0, [sp, #0x18]
003fe3b4: cmp      r0, r5
003fe3b8: beq      #0x3fe3d8
003fe3bc: cmp      r0, #0
003fe3c0: beq      #0x3fe3d8
003fe3c4: ldr      r1, [sp, #4]
003fe3c8: rsb      r1, r0, r1
003fe3cc: cmp      r1, #0x80
003fe3d0: bhi      #0x3fe42c
003fe3d4: bl       #0x708f00
003fe3d8: cmp      r7, #0
003fe3dc: bne      #0x3fe40c
003fe3e0: ldrb     r3, [r8, #0x2f]
003fe3e4: cmp      r3, #0
003fe3e8: bne      #0x3fe40c
003fe3ec: ldr      r3, [r8, #8]
003fe3f0: ldr      r0, [r8, #0xc]
003fe3f4: rsb      r0, r3, r0
003fe3f8: asr      r0, r0, #2
003fe3fc: cmp      r0, #0x63
003fe400: movls    r0, #0
003fe404: movhi    r0, #1
003fe408: b        #0x3fe410
003fe40c: mov      r0, #0
003fe410: ldr      r3, [r4, r6]
003fe414: ldr      r2, [sp, #0x1c]
003fe418: ldr      r3, [r3]
003fe41c: cmp      r2, r3
003fe420: bne      #0x3fe434
003fe424: add      sp, sp, #0x20
003fe428: pop      {r4, r5, r6, r7, r8, pc}
003fe42c: bl       #0x310440
003fe430: b        #0x3fe3d8
003fe434: bl       #0x30e310
003fe438: subseq   r6, sb, r0, asr r7
003fe43c: andeq    r4, r0, ip, lsr #1
003fe440: andeq    r0, r0, r4, lsl #17
003fe444: subeq    sb, ip, r0, asr #1

# _ZN12ItemInstance8SetValueEi
003fbc58: str      r1, [r0, #0x54]
003fbc5c: b        #0x3fb754

# _ZNK12ItemInstance12GetNumPowersEv
003f9e80: ldr      r3, [r0, #0x5c]
003f9e84: ldr      r0, [r0, #0x60]
003f9e88: rsb      r0, r3, r0
003f9e8c: asr      r0, r0, #5
003f9e90: bx       lr

# _ZN12ItemInstance8AddPowerEii
003fbc60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003fbc64: ldr      r5, [pc, #0x510]
003fbc68: ldr      r3, [pc, #0x510]
003fbc6c: sub      sp, sp, #0x9c
003fbc70: add      r5, pc, r5
003fbc74: str      r3, [sp, #0xc]
003fbc78: ldr      r3, [r5, r3]
003fbc7c: subs     r7, r1, #0
003fbc80: mov      r4, r0
003fbc84: ldr      r3, [r3]
003fbc88: mov      r6, r2
003fbc8c: str      r3, [sp, #0x94]
003fbc90: blt      #0x3fbefc
003fbc94: ldr      r3, [pc, #0x4e8]
003fbc98: ldr      r3, [r5, r3]
003fbc9c: ldr      r3, [r3]
003fbca0: cmp      r7, r3
003fbca4: bge      #0x3fbefc
003fbca8: cmp      r6, #1
003fbcac: beq      #0x3fc0d8
003fbcb0: cmp      r6, #2
003fbcb4: movne    r6, r7
003fbcb8: movne    r7, r6
003fbcbc: beq      #0x3fc138
003fbcc0: ldr      r3, [pc, #0x4c0]
003fbcc4: mov      r8, #0x28
003fbcc8: add      sb, sp, #0x74
003fbccc: ldr      r3, [r5, r3]
003fbcd0: add      sl, sb, #8
003fbcd4: mov      r0, sl
003fbcd8: ldr      r3, [r3]
003fbcdc: mov      r1, #0x10
003fbce0: add      fp, r4, #0x5c
003fbce4: mla      r8, r8, r6, r3
003fbce8: mov      r6, #0
003fbcec: ldr      r3, [r8, #0x1c]
003fbcf0: str      r7, [sp, #0x74]
003fbcf4: str      sl, [sp, #0x8c]
003fbcf8: str      r3, [sp, #0x78]
003fbcfc: str      sl, [sp, #0x90]
003fbd00: bl       #0x31167c
003fbd04: ldr      r3, [sp, #0x8c]
003fbd08: mov      r1, sb
003fbd0c: mov      r0, fp
003fbd10: strb     r6, [r3]
003fbd14: bl       #0x3fb640
003fbd18: mov      r0, sl
003fbd1c: bl       #0x3139ac
003fbd20: ldr      r3, [r8, #0xc]
003fbd24: cmp      r3, r6
003fbd28: beq      #0x3fc098
003fbd2c: ldr      r1, [pc, #0x458]
003fbd30: str      r6, [sp, #0x24]
003fbd34: str      r6, [sp, #0x28]
003fbd38: ldr      r3, [r5, r1]
003fbd3c: str      r1, [sp, #0x14]
003fbd40: str      r6, [sp, #0x2c]
003fbd44: add      r3, r3, #8
003fbd48: str      r3, [sp, #0x20]
003fbd4c: ldr      r3, [r8, #0xc]
003fbd50: cmp      r3, r6
003fbd54: addeq    r2, sp, #0x20
003fbd58: streq    r2, [sp, #0x10]
003fbd5c: beq      #0x3fbe0c
003fbd60: movw     r2, #0x5555
003fbd64: add      r1, sp, #0x20
003fbd68: orr      r2, r2, r2, lsl #14
003fbd6c: mov      r3, r6
003fbd70: str      r1, [sp, #0x10]
003fbd74: str      r2, [sp, #8]
003fbd78: add      r1, r1, #0xc
003fbd7c: add      r2, sp, #0x30
003fbd80: mov      r7, r3
003fbd84: str      r1, [sp, #0x18]
003fbd88: str      r2, [sp, #0x1c]
003fbd8c: mov      sl, r3
003fbd90: b        #0x3fbd9c
003fbd94: ldr      r6, [sp, #0x28]
003fbd98: ldr      r3, [sp, #0x2c]
003fbd9c: cmp      r6, r3
003fbda0: beq      #0x3fbf54
003fbda4: mov      r3, #0
003fbda8: str      r3, [r6]
003fbdac: str      sl, [r6, #8]
003fbdb0: str      sl, [r6, #4]
003fbdb4: ldr      r6, [sp, #0x28]
003fbdb8: add      r6, r6, #0xc
003fbdbc: str      r6, [sp, #0x28]
003fbdc0: ldr      r3, [r8, #0x10]
003fbdc4: lsl      fp, r7, #4
003fbdc8: sub      sb, r6, #0xc
003fbdcc: add      r3, r3, fp
003fbdd0: ldr      r0, [r3, #8]
003fbdd4: bl       #0x30e964
003fbdd8: mov      r1, #0x3b800000
003fbddc: bl       #0x30ed6c
003fbde0: str      r0, [r6, #-0xc]
003fbde4: ldr      r3, [r8, #0x10]
003fbde8: add      r7, r7, #1
003fbdec: add      fp, r3, fp
003fbdf0: ldr      r3, [fp, #8]
003fbdf4: str      sl, [sb, #8]
003fbdf8: asr      r3, r3, #8
003fbdfc: str      r3, [sb, #4]
003fbe00: ldr      r3, [r8, #0xc]
003fbe04: cmp      r3, r7
003fbe08: bhi      #0x3fbd94
003fbe0c: ldr      r3, [pc, #0x37c]
003fbe10: ldr      r1, [r8, #0x14]
003fbe14: ldr      r7, [r4, #0x60]
003fbe18: ldr      r3, [r5, r3]
003fbe1c: sub      r7, r7, #0x18
003fbe20: ldr      r6, [r3, #0x34]
003fbe24: mov      r0, r6
003fbe28: bl       #0x508edc
003fbe2c: mov      r1, r7
003fbe30: mov      r2, r0
003fbe34: ldr      r3, [sp, #0x10]
003fbe38: mov      r0, r6
003fbe3c: bl       #0x509aec
003fbe40: ldr      r2, [sp, #0x14]
003fbe44: ldr      r1, [sp, #0x10]
003fbe48: ldr      r3, [r5, r2]
003fbe4c: add      r0, r1, #4
003fbe50: add      r3, r3, #8
003fbe54: str      r3, [sp, #0x20]
003fbe58: bl       #0x3fab18
003fbe5c: ldr      r3, [r4, #0x5c]
003fbe60: ldr      r6, [r4, #0x60]
003fbe64: rsb      r6, r3, r6
003fbe68: asr      r6, r6, #5
003fbe6c: subs     r0, r6, #1
003fbe70: beq      #0x3fbedc
003fbe74: sub      r1, r0, #1
003fbe78: add      r1, r3, r1, lsl #5
003fbe7c: add      r0, r3, r0, lsl #5
003fbe80: ldr      r2, [r1, #4]
003fbe84: ldr      r3, [r0, #4]
003fbe88: cmp      r2, r3
003fbe8c: ble      #0x3fbedc
003fbe90: sub      r7, r6, #0xf8000002
003fbe94: sub      r8, r6, #0xf8000003
003fbe98: lsl      r8, r8, #5
003fbe9c: lsl      r7, r7, #5
003fbea0: sub      r6, r6, #2
003fbea4: b        #0x3fbed0
003fbea8: ldr      r1, [r4, #0x5c]
003fbeac: sub      r6, r6, #1
003fbeb0: add      r0, r1, r7
003fbeb4: add      r1, r1, r8
003fbeb8: ldr      r2, [r1, #4]
003fbebc: ldr      r3, [r0, #4]
003fbec0: sub      r8, r8, #0x20
003fbec4: sub      r7, r7, #0x20
003fbec8: cmp      r2, r3
003fbecc: ble      #0x3fbedc
003fbed0: bl       #0x3fb570
003fbed4: cmp      r6, #0
003fbed8: bne      #0x3fbea8
003fbedc: ldr      r2, [sp, #0xc]
003fbee0: ldr      r3, [r5, r2]
003fbee4: ldr      r2, [sp, #0x94]
003fbee8: ldr      r3, [r3]
003fbeec: cmp      r2, r3
003fbef0: bne      #0x3fc178
003fbef4: add      sp, sp, #0x9c
003fbef8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003fbefc: ldr      r3, [pc, #0x290]
003fbf00: ldr      r3, [r5, r3]
003fbf04: ldr      r3, [r3]
003fbf08: cmp      r3, #2
003fbf0c: moveq    r3, #0
003fbf10: streq    r3, [r3]
003fbf14: beq      #0x3fbca8
003fbf18: cmp      r3, #1
003fbf1c: bne      #0x3fbca8
003fbf20: ldr      r0, [pc, #0x270]
003fbf24: ldr      r1, [pc, #0x270]
003fbf28: ldr      r2, [pc, #0x270]
003fbf2c: ldr      r0, [r5, r0]
003fbf30: ldr      r3, [pc, #0x26c]
003fbf34: movw     ip, #0x25e
003fbf38: add      r1, pc, r1
003fbf3c: add      r2, pc, r2
003fbf40: add      r3, pc, r3
003fbf44: add      r0, r0, #0xa8
003fbf48: str      ip, [sp]
003fbf4c: bl       #0x30e004
003fbf50: b        #0x3fbca8
003fbf54: ldr      r3, [sp, #0x24]
003fbf58: ldr      r1, [sp, #8]
003fbf5c: rsb      r3, r3, r6
003fbf60: asr      r3, r3, #2
003fbf64: add      r2, r3, r3, lsl #2
003fbf68: add      r2, r2, r2, lsl #4
003fbf6c: add      r2, r2, r2, lsl #8
003fbf70: add      r2, r2, r2, lsl #16
003fbf74: add      r2, r3, r2, lsl #1
003fbf78: cmp      r2, #1
003fbf7c: addhs    r3, r2, r2
003fbf80: addlo    r3, r2, #1
003fbf84: cmp      r3, r1
003fbf88: bhi      #0x3fc08c
003fbf8c: cmp      r2, r3
003fbf90: bhi      #0x3fc08c
003fbf94: mov      r1, r3
003fbf98: ldr      r0, [sp, #0x18]
003fbf9c: ldr      r2, [sp, #0x1c]
003fbfa0: str      r3, [sp, #0x30]
003fbfa4: bl       #0x3fac54
003fbfa8: ldr      ip, [sp, #0x24]
003fbfac: mov      sb, r0
003fbfb0: rsb      r6, ip, r6
003fbfb4: asr      r3, r6, #2
003fbfb8: add      r6, r3, r3, lsl #2
003fbfbc: add      r6, r6, r6, lsl #4
003fbfc0: add      r6, r6, r6, lsl #8
003fbfc4: add      r6, r6, r6, lsl #16
003fbfc8: add      r6, r3, r6, lsl #1
003fbfcc: cmp      r6, #0
003fbfd0: movle    r3, r0
003fbfd4: ble      #0x3fc01c
003fbfd8: mov      r0, r6
003fbfdc: mov      r3, #0
003fbfe0: ldr      r2, [ip, r3]
003fbfe4: add      r1, ip, r3
003fbfe8: add      r1, r1, #4
003fbfec: str      r2, [sb, r3]
003fbff0: ldr      lr, [r1], #4
003fbff4: add      r2, sb, r3
003fbff8: add      r2, r2, #4
003fbffc: str      lr, [r2], #4
003fc000: ldr      r1, [r1]
003fc004: subs     r0, r0, #1
003fc008: add      r3, r3, #0xc
003fc00c: str      r1, [r2]
003fc010: bne      #0x3fbfe0
003fc014: mov      r2, #0xc
003fc018: mla      r3, r2, r6, sb
003fc01c: str      sl, [r3, #8]
003fc020: str      sl, [r3, #4]
003fc024: mov      r6, r3
003fc028: mov      r3, #0
003fc02c: str      r3, [r6], #0xc
003fc030: ldr      r0, [sp, #0x24]
003fc034: ldr      r3, [sp, #0x2c]
003fc038: cmp      r0, #0
003fc03c: beq      #0x3fc070
003fc040: rsb      r3, r0, r3
003fc044: asr      r3, r3, #2
003fc048: mov      r2, #0xc
003fc04c: add      r1, r3, r3, lsl #2
003fc050: add      r1, r1, r1, lsl #4
003fc054: add      r1, r1, r1, lsl #8
003fc058: add      r1, r1, r1, lsl #16
003fc05c: add      r1, r3, r1, lsl #1
003fc060: mul      r1, r2, r1
003fc064: cmp      r1, #0x80
003fc068: bhi      #0x3fc0d0
003fc06c: bl       #0x708f00
003fc070: ldr      r3, [sp, #0x30]
003fc074: mov      r1, #0xc
003fc078: str      sb, [sp, #0x24]
003fc07c: mla      sb, r1, r3, sb
003fc080: str      r6, [sp, #0x28]
003fc084: str      sb, [sp, #0x2c]
003fc088: b        #0x3fbdc0
003fc08c: movw     r3, #0x5555
003fc090: orr      r3, r3, r3, lsl #14
003fc094: b        #0x3fbf94
003fc098: ldr      r3, [pc, #0xf0]
003fc09c: ldr      r1, [r8, #0x14]
003fc0a0: ldr      r6, [r4, #0x60]
003fc0a4: ldr      r3, [r5, r3]
003fc0a8: sub      r6, r6, #0x18
003fc0ac: ldr      r0, [r3, #0x34]
003fc0b0: bl       #0x508edc
003fc0b4: mov      r7, r0
003fc0b8: bl       #0x30de54
003fc0bc: mov      r1, r7
003fc0c0: add      r2, r7, r0
003fc0c4: mov      r0, r6
003fc0c8: bl       #0x3109e0
003fc0cc: b        #0x3fbe5c
003fc0d0: bl       #0x310440
003fc0d4: b        #0x3fc070
003fc0d8: ldr      r3, [pc, #0xc8]
003fc0dc: add      r8, sp, #0x34
003fc0e0: mov      r0, r8
003fc0e4: ldr      sl, [r5, r3]
003fc0e8: mov      r6, r7
003fc0ec: add      sb, r7, #1
003fc0f0: ldr      r3, [sl]
003fc0f4: ldr      r1, [r3, r7, lsl #2]
003fc0f8: bl       #0x30e520
003fc0fc: mov      r0, r8
003fc100: bl       #0x30de54
003fc104: ldr      r1, [pc, #0xa0]
003fc108: add      r0, r8, r0
003fc10c: mov      r2, #6
003fc110: add      r1, pc, r1
003fc114: bl       #0x30e868
003fc118: ldr      r3, [sl]
003fc11c: mov      r0, r8
003fc120: ldr      r1, [r3, sb, lsl #2]
003fc124: bl       #0x30e31c
003fc128: cmp      r0, #0
003fc12c: moveq    r7, sb
003fc130: moveq    r6, r7
003fc134: b        #0x3fbcc0
003fc138: ldr      r3, [pc, #0x68]
003fc13c: add      r8, sp, #0x34
003fc140: mov      r0, r8
003fc144: ldr      sl, [r5, r3]
003fc148: mov      r6, r7
003fc14c: add      sb, r7, #2
003fc150: ldr      r3, [sl]
003fc154: ldr      r1, [r3, r7, lsl #2]
003fc158: bl       #0x30e520
003fc15c: mov      r0, r8
003fc160: bl       #0x30de54
003fc164: ldr      r1, [pc, #0x44]
003fc168: add      r0, r8, r0
003fc16c: mov      r2, #0xa
003fc170: add      r1, pc, r1
003fc174: b        #0x3fc114
003fc178: bl       #0x30e310
003fc17c: subseq   r8, sb, r0, lsr #28
003fc180: andeq    r4, r0, ip, lsr #1
003fc184: andeq    r1, r0, r8, lsl #3
003fc188: andeq    r3, r0, r8, ror #23
003fc18c: andeq    r4, r0, r8, lsl #1
003fc190: strdeq   r3, r4, [r0], -r4
003fc194: andeq    r3, r0, r0, asr #19
003fc198: andeq    r1, r0, r0, asr #19
003fc19c: subeq    r2, ip, r0, lsr #9
003fc1a0: subeq    fp, ip, r4, lsr #5
003fc1a4: subeq    fp, ip, r8
003fc1a8: andeq    r1, r0, ip, asr #5
003fc1ac: subeq    fp, ip, r0, lsl r1
003fc1b0: strheq   fp, [ip], #-8

# _ZNK12ItemInstanceeqERKS_
003f9d78: str      r4, [sp, #-4]!
003f9d7c: ldr      r2, [r0, #4]
003f9d80: ldr      r3, [r1, #4]
003f9d84: cmp      r2, r3
003f9d88: beq      #0x3f9d98
003f9d8c: mov      r0, #0
003f9d90: ldm      sp!, {r4}
003f9d94: bx       lr
003f9d98: ldr      r2, [r0, #0x60]
003f9d9c: ldr      r4, [r0, #0x5c]
003f9da0: ldr      r3, [r1, #0x60]
003f9da4: ldr      ip, [r1, #0x5c]
003f9da8: rsb      r0, r4, r2
003f9dac: asr      r0, r0, #5
003f9db0: rsb      r3, ip, r3
003f9db4: cmp      r0, r3, asr #5
003f9db8: bne      #0x3f9d8c
003f9dbc: cmp      r0, #0
003f9dc0: beq      #0x3f9df8
003f9dc4: ldr      r3, [ip]
003f9dc8: ldr      r2, [r4]
003f9dcc: cmp      r2, r3
003f9dd0: moveq    r3, #0
003f9dd4: beq      #0x3f9dec
003f9dd8: b        #0x3f9d8c
003f9ddc: ldr      r1, [r4, r3, lsl #5]
003f9de0: ldr      r2, [ip, r3, lsl #5]
003f9de4: cmp      r1, r2
003f9de8: bne      #0x3f9d8c
003f9dec: add      r3, r3, #1
003f9df0: cmp      r3, r0
003f9df4: bne      #0x3f9ddc
003f9df8: mov      r0, #1
003f9dfc: b        #0x3f9d90

# _ZN13ItemInventory16_AddItemInstanceEP12ItemInstancebb
003ff5d4: push     {r4, r5, r6, r7, r8, sl, lr}
003ff5d8: mov      r4, r0
003ff5dc: ldrsb    r0, [r0, #0x2c]
003ff5e0: ldr      r6, [pc, #0x260]
003ff5e4: sub      sp, sp, #0xc
003ff5e8: cmp      r0, #0
003ff5ec: add      r6, pc, r6
003ff5f0: mov      r5, r1
003ff5f4: mov      r8, r2
003ff5f8: mov      r7, r3
003ff5fc: beq      #0x3ff6f0
003ff600: ldr      r3, [r4, #0x24]
003ff604: cmp      r3, #0
003ff608: beq      #0x3ff81c
003ff60c: cmp      r7, #0
003ff610: bne      #0x3ff724
003ff614: mov      r0, r5
003ff618: bl       #0x3f9e58
003ff61c: cmp      r0, #0
003ff620: beq      #0x3ff674
003ff624: cmp      r8, #0
003ff628: bne      #0x3ff674
003ff62c: ldr      r7, [r4, #8]
003ff630: ldr      r2, [r4, #0xc]
003ff634: cmp      r7, r2
003ff638: beq      #0x3ff674
003ff63c: ldr      sl, [r7]
003ff640: mov      r1, r8
003ff644: mov      r0, r4
003ff648: ldr      r3, [sl]
003ff64c: cmp      r3, #0
003ff650: beq      #0x3ff664
003ff654: bl       #0x3fdaf0
003ff658: cmp      r0, #0
003ff65c: beq      #0x3ff75c
003ff660: ldr      r2, [r4, #0xc]
003ff664: add      r7, r7, #4
003ff668: cmp      r7, r2
003ff66c: addne    r8, r8, #1
003ff670: bne      #0x3ff63c
003ff674: mov      r1, #0
003ff678: mov      r0, #8
003ff67c: bl       #0x310570
003ff680: mvn      r3, #0
003ff684: str      r5, [r0]
003ff688: strb     r3, [r0, #5]
003ff68c: strb     r3, [r0, #4]
003ff690: ldr      r1, [r4, #0xc]
003ff694: ldr      r3, [r4, #0x10]
003ff698: str      r0, [sp, #4]
003ff69c: cmp      r1, r3
003ff6a0: beq      #0x3ff838
003ff6a4: str      r0, [r1]
003ff6a8: ldr      r3, [r4, #0xc]
003ff6ac: add      r3, r3, #4
003ff6b0: str      r3, [r4, #0xc]
003ff6b4: ldr      r3, [pc, #0x190]
003ff6b8: mov      r0, r4
003ff6bc: ldr      r3, [r6, r3]
003ff6c0: ldr      r5, [r3]
003ff6c4: bl       #0x3fe330
003ff6c8: cmp      r0, #0
003ff6cc: bne      #0x3ff7a8
003ff6d0: ldr      r3, [r4, #8]
003ff6d4: ldr      r8, [r4, #0xc]
003ff6d8: rsb      r8, r3, r8
003ff6dc: asr      r8, r8, #2
003ff6e0: sub      r8, r8, #1
003ff6e4: mov      r0, r8
003ff6e8: add      sp, sp, #0xc
003ff6ec: pop      {r4, r5, r6, r7, r8, sl, pc}
003ff6f0: mov      r0, r1
003ff6f4: bl       #0x3f9e08
003ff6f8: ldr      r3, [r0, #0x58]
003ff6fc: cmp      r3, #0xe
003ff700: bne      #0x3ff600
003ff704: cmp      r5, #0
003ff708: beq      #0x3ff754
003ff70c: mov      r0, r5
003ff710: ldr      r3, [r5]
003ff714: mov      lr, pc
003ff718: ldr      pc, [r3, #4]
003ff71c: mvn      r8, #0
003ff720: b        #0x3ff6e4
003ff724: mov      r0, r5
003ff728: bl       #0x3f9e08
003ff72c: ldr      r3, [r0, #0x58]
003ff730: cmp      r3, #0xd
003ff734: bne      #0x3ff614
003ff738: mov      r0, r4
003ff73c: ldr      r1, [r5, #0x54]
003ff740: bl       #0x3fe164
003ff744: mov      r0, r5
003ff748: ldr      r3, [r5]
003ff74c: mov      lr, pc
003ff750: ldr      pc, [r3, #4]
003ff754: mvn      r8, #0
003ff758: b        #0x3ff6e4
003ff75c: ldr      r0, [sl]
003ff760: mov      r1, r5
003ff764: bl       #0x3f9d78
003ff768: cmp      r0, #0
003ff76c: beq      #0x3ff660
003ff770: mov      r0, r5
003ff774: ldrsh    r6, [r5, #0x50]
003ff778: bl       #0x3f9e08
003ff77c: ldr      r3, [r0, #0x58]
003ff780: cmp      r3, #0xe
003ff784: beq      #0x3ff7fc
003ff788: ldr      r0, [sl]
003ff78c: mov      r1, r6
003ff790: bl       #0x3fa17c
003ff794: mov      r0, r5
003ff798: ldr      r3, [r5]
003ff79c: mov      lr, pc
003ff7a0: ldr      pc, [r3, #4]
003ff7a4: b        #0x3ff6e4
003ff7a8: ldr      r3, [r4, #4]
003ff7ac: mov      r0, r3
003ff7b0: ldr      r3, [r3]
003ff7b4: mov      lr, pc
003ff7b8: ldr      pc, [r3, #0x28]
003ff7bc: cmp      r0, #0
003ff7c0: beq      #0x3ff6d0
003ff7c4: ldr      r3, [pc, #0x84]
003ff7c8: ldr      r1, [r4, #4]
003ff7cc: ldr      r3, [r6, r3]
003ff7d0: ldr      r0, [r3, #0x40]
003ff7d4: bl       #0x36effc
003ff7d8: cmp      r0, #0
003ff7dc: beq      #0x3ff6d0
003ff7e0: ldr      r0, [pc, #0x6c]
003ff7e4: add      r0, pc, r0
003ff7e8: bl       #0x3a3f70
003ff7ec: mov      r1, r0
003ff7f0: mov      r0, r5
003ff7f4: bl       #0x3813b8
003ff7f8: b        #0x3ff6d0
003ff7fc: ldr      r0, [sl]
003ff800: ldrsb    r1, [r4, #0x2c]
003ff804: ldrsh    r3, [r0, #0x50]
003ff808: rsb      r1, r3, r1
003ff80c: cmp      r1, r6
003ff810: movge    r1, r6
003ff814: biclt    r1, r1, r1, asr #31
003ff818: b        #0x3ff790
003ff81c: mov      r0, r5
003ff820: bl       #0x3f9e08
003ff824: ldr      r3, [r0, #0x58]
003ff828: cmp      r3, #0xe
003ff82c: streq    r5, [r4, #0x24]
003ff830: bne      #0x3ff60c
003ff834: b        #0x3ff614
003ff838: add      r0, r4, #8
003ff83c: add      r2, sp, #4
003ff840: bl       #0x3fef4c
003ff844: b        #0x3ff6b4
003ff848: subseq   r5, sb, r4, lsr #9
003ff84c: andeq    r1, r0, r0, ror sp
003ff850: strdeq   r3, r4, [r0], -r4
003ff854: subeq    r7, ip, ip, ror ip

# _ZNK12ItemInstance10GetPowerIdEj
003fa038: push     {r4, r5, lr}
003fa03c: mov      r4, r0
003fa040: ldr      r2, [r0, #0x5c]
003fa044: ldr      r0, [r0, #0x60]
003fa048: ldr      r3, [pc, #0x7c]
003fa04c: sub      sp, sp, #0xc
003fa050: rsb      r0, r2, r0
003fa054: cmp      r1, r0, asr #5
003fa058: mov      r5, r1
003fa05c: add      r3, pc, r3
003fa060: blo      #0x3fa088
003fa064: ldr      r1, [pc, #0x64]
003fa068: ldr      r1, [r3, r1]
003fa06c: ldr      r1, [r1]
003fa070: cmp      r1, #2
003fa074: moveq    r3, #0
003fa078: streq    r3, [r3]
003fa07c: beq      #0x3fa088
003fa080: cmp      r1, #1
003fa084: beq      #0x3fa094
003fa088: ldr      r0, [r2, r5, lsl #5]
003fa08c: add      sp, sp, #0xc
003fa090: pop      {r4, r5, pc}
003fa094: ldr      r0, [pc, #0x38]
003fa098: ldr      r1, [pc, #0x38]
003fa09c: ldr      r2, [pc, #0x38]
003fa0a0: ldr      r0, [r3, r0]
003fa0a4: ldr      r3, [pc, #0x34]
003fa0a8: add      r2, pc, r2
003fa0ac: movw     ip, #0x2af
003fa0b0: add      r1, pc, r1
003fa0b4: add      r0, r0, #0xa8
003fa0b8: add      r3, pc, r3
003fa0bc: str      ip, [sp]
003fa0c0: bl       #0x30e004
003fa0c4: ldr      r2, [r4, #0x5c]
003fa0c8: b        #0x3fa088
003fa0cc: subseq   sl, sb, r4, lsr sl
003fa0d0: andeq    r3, r0, r0, asr #19
003fa0d4: andeq    r1, r0, r0, asr #19
003fa0d8: subeq    r4, ip, r8, lsr #6
003fa0dc: subeq    ip, ip, r0, lsl #29
003fa0e0: umaaleq  ip, ip, r0, lr
