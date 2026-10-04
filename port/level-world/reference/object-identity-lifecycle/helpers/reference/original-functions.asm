
# _ZN13ObjectManager15GetObjectByNameEPKcibS1_
0034aca0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034aca4: ldr      r5, [pc, #0x38c]
0034aca8: ldr      ip, [pc, #0x38c]
0034acac: sub      sp, sp, #0x74
0034acb0: add      r5, pc, r5
0034acb4: str      ip, [sp, #0x14]
0034acb8: ldr      ip, [r5, ip]
0034acbc: str      r1, [sp, #0x18]
0034acc0: ldr      r1, [pc, #0x378]
0034acc4: ldr      ip, [ip]
0034acc8: str      r0, [sp, #0xc]
0034accc: mov      r4, r2
0034acd0: mov      r0, r2
0034acd4: add      r1, pc, r1
0034acd8: mov      r2, #6
0034acdc: str      ip, [sp, #0x6c]
0034ace0: mov      r7, r3
0034ace4: bl       #0x30ec7c
0034ace8: ldrb     lr, [sp, #0x98]
0034acec: cmp      r0, #0
0034acf0: ldr      r6, [sp, #0x9c]
0034acf4: str      lr, [sp, #0x24]
0034acf8: beq      #0x34ad18
0034acfc: ldr      r1, [pc, #0x340]
0034ad00: mov      r0, r4
0034ad04: mov      r2, #0x10
0034ad08: add      r1, pc, r1
0034ad0c: bl       #0x30ec7c
0034ad10: cmp      r0, #0
0034ad14: bne      #0x34af98
0034ad18: mvn      r7, #0
0034ad1c: ldr      r1, [pc, #0x324]
0034ad20: mov      r0, r4
0034ad24: add      r1, pc, r1
0034ad28: bl       #0x30e31c
0034ad2c: cmp      r0, #0
0034ad30: beq      #0x34afb8
0034ad34: add      lr, sp, #0x40
0034ad38: mov      r0, lr
0034ad3c: str      lr, [sp, #0x20]
0034ad40: bl       #0x33f50c
0034ad44: ldr      r3, [pc, #0x300]
0034ad48: ldr      r0, [sp, #0x18]
0034ad4c: ldr      sb, [pc, #0x2fc]
0034ad50: ldr      r2, [pc, #0x2fc]
0034ad54: add      r3, pc, r3
0034ad58: ldr      r6, [r0, #0x14]
0034ad5c: str      r3, [sp, #8]
0034ad60: add      r3, sp, #0x34
0034ad64: add      sb, pc, sb
0034ad68: add      r8, r0, #0xc
0034ad6c: str      r2, [sp, #0x10]
0034ad70: add      sl, sp, #0x28
0034ad74: str      r3, [sp, #0x1c]
0034ad78: cmp      r6, r8
0034ad7c: beq      #0x34ae98
0034ad80: ldr      r3, [r6, #0x2c]
0034ad84: cmp      r3, #0
0034ad88: beq      #0x34ae6c
0034ad8c: cmn      r7, #1
0034ad90: beq      #0x34adac
0034ad94: ldr      r2, [r3, #0x64]
0034ad98: cmp      r7, r2
0034ad9c: beq      #0x34adac
0034ada0: ldrb     r3, [r3, #0x87]
0034ada4: cmp      r3, #0
0034ada8: beq      #0x34ae6c
0034adac: ldr      r0, [r6, #0x28]
0034adb0: mov      r1, r4
0034adb4: bl       #0x30e31c
0034adb8: cmp      r0, #0
0034adbc: bne      #0x34ae10
0034adc0: ldr      r0, [sp, #0x20]
0034adc4: ldr      r1, [r6, #0x10]
0034adc8: add      r2, r0, #4
0034adcc: ldr      r0, [r2], #4
0034add0: ldr      r3, [sp, #0xc]
0034add4: ldr      r2, [r2]
0034add8: str      r1, [r3], #4
0034addc: ldr      ip, [sp, #0xc]
0034ade0: str      r2, [r3, #4]
0034ade4: str      r0, [ip, #4]
0034ade8: str      r1, [sp, #0x40]
0034adec: ldr      r0, [sp, #0x14]
0034adf0: ldr      r2, [sp, #0x6c]
0034adf4: ldr      r3, [r5, r0]
0034adf8: ldr      r0, [sp, #0xc]
0034adfc: ldr      r3, [r3]
0034ae00: cmp      r2, r3
0034ae04: bne      #0x34b034
0034ae08: add      sp, sp, #0x74
0034ae0c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034ae10: mov      r1, sb
0034ae14: mov      r0, r4
0034ae18: bl       #0x30e31c
0034ae1c: subs     r1, r0, #0
0034ae20: beq      #0x34af60
0034ae24: ldr      r1, [sp, #8]
0034ae28: mov      r0, r4
0034ae2c: bl       #0x30e31c
0034ae30: subs     r1, r0, #0
0034ae34: bne      #0x34ae6c
0034ae38: ldr      lr, [sp, #0x10]
0034ae3c: mov      r2, #1
0034ae40: ldr      r3, [r5, lr]
0034ae44: ldr      r0, [r3, #0x40]
0034ae48: bl       #0x36e478
0034ae4c: ldr      r1, [r6, #0x2c]
0034ae50: ldr      fp, [r0, #0x660]
0034ae54: mov      r0, sl
0034ae58: bl       #0x33dd2c
0034ae5c: mov      r0, sl
0034ae60: bl       #0x33ff54
0034ae64: cmp      fp, r0
0034ae68: beq      #0x34adc0
0034ae6c: ldr      r3, [r6, #0xc]
0034ae70: cmp      r3, #0
0034ae74: bne      #0x34ae80
0034ae78: b        #0x34af2c
0034ae7c: mov      r3, r2
0034ae80: ldr      r2, [r3, #8]
0034ae84: cmp      r2, #0
0034ae88: bne      #0x34ae7c
0034ae8c: mov      r6, r3
0034ae90: cmp      r6, r8
0034ae94: bne      #0x34ad80
0034ae98: ldr      lr, [sp, #0x24]
0034ae9c: cmp      lr, #0
0034aea0: bne      #0x34afd8
0034aea4: ldr      r3, [pc, #0x1ac]
0034aea8: add      r4, sp, #0x54
0034aeac: ldr      r6, [r5, r3]
0034aeb0: mov      r0, r6
0034aeb4: bl       #0x337888
0034aeb8: ldr      r1, [pc, #0x19c]
0034aebc: add      r2, sp, #0x50
0034aec0: mov      r0, r4
0034aec4: add      r1, pc, r1
0034aec8: bl       #0x3140ec
0034aecc: mov      r0, r6
0034aed0: mov      r1, r4
0034aed4: bl       #0x337a88
0034aed8: ldr      r0, [sp, #0x68]
0034aedc: cmp      r0, r4
0034aee0: beq      #0x34af00
0034aee4: cmp      r0, #0
0034aee8: beq      #0x34af00
0034aeec: ldr      r1, [sp, #0x54]
0034aef0: rsb      r1, r0, r1
0034aef4: cmp      r1, #0x80
0034aef8: bhi      #0x34b02c
0034aefc: bl       #0x708f00
0034af00: ldr      r0, [sp, #0x20]
0034af04: ldr      r3, [sp, #0xc]
0034af08: add      r2, r0, #4
0034af0c: ldr      r1, [r2], #4
0034af10: ldr      r0, [sp, #0x40]
0034af14: ldr      r2, [r2]
0034af18: str      r0, [r3], #4
0034af1c: ldr      ip, [sp, #0xc]
0034af20: str      r2, [r3, #4]
0034af24: str      r1, [ip, #4]
0034af28: b        #0x34adec
0034af2c: ldr      r2, [r6, #4]
0034af30: ldr      r1, [r2, #0xc]
0034af34: cmp      r6, r1
0034af38: bne      #0x34af54
0034af3c: mov      r6, r2
0034af40: ldr      r2, [r2, #4]
0034af44: ldr      r3, [r2, #0xc]
0034af48: cmp      r3, r6
0034af4c: beq      #0x34af3c
0034af50: ldr      r3, [r6, #0xc]
0034af54: cmp      r2, r3
0034af58: movne    r6, r2
0034af5c: b        #0x34ad78
0034af60: ldr      ip, [sp, #0x10]
0034af64: mov      r2, #1
0034af68: ldr      r3, [r5, ip]
0034af6c: ldr      r0, [r3, #0x40]
0034af70: bl       #0x36e478
0034af74: ldr      r1, [r6, #0x2c]
0034af78: ldr      fp, [r0, #0x660]
0034af7c: ldr      r0, [sp, #0x1c]
0034af80: bl       #0x33dd2c
0034af84: ldr      r0, [sp, #0x1c]
0034af88: bl       #0x33ff54
0034af8c: cmp      fp, r0
0034af90: bne      #0x34ae24
0034af94: b        #0x34adc0
0034af98: ldr      r1, [pc, #0xc0]
0034af9c: mov      r0, r4
0034afa0: mov      r2, #0xb
0034afa4: add      r1, pc, r1
0034afa8: bl       #0x30ec7c
0034afac: cmp      r0, #0
0034afb0: bne      #0x34ad1c
0034afb4: b        #0x34ad18
0034afb8: ldr      ip, [sp, #0x24]
0034afbc: ldr      r1, [sp, #0x18]
0034afc0: mov      r2, r6
0034afc4: mov      r3, r7
0034afc8: ldr      r0, [sp, #0xc]
0034afcc: str      ip, [sp]
0034afd0: bl       #0x34b064
0034afd4: b        #0x34adec
0034afd8: ldr      r0, [sp, #0x18]
0034afdc: ldr      r2, [sp, #0x18]
0034afe0: add      r1, sp, #0x70
0034afe4: ldr      r3, [r0, #0x4c]
0034afe8: mov      r0, r6
0034afec: str      r3, [r1, #-0x24]!
0034aff0: add      r3, r3, #1
0034aff4: str      r3, [r2, #0x4c]
0034aff8: bl       #0x34952c
0034affc: mov      r6, r0
0034b000: mov      r0, r4
0034b004: bl       #0x30de54
0034b008: mov      r1, r4
0034b00c: add      r2, r4, r0
0034b010: mov      r0, r6
0034b014: bl       #0x3109e0
0034b018: ldr      r3, [sp, #0x20]
0034b01c: ldr      r1, [sp, #0x4c]
0034b020: add      r2, r3, #4
0034b024: ldr      r0, [r2], #4
0034b028: b        #0x34add0
0034b02c: bl       #0x310440
0034b030: b        #0x34af00
0034b034: bl       #0x30e310
0034b038: rsbeq    sb, r4, r0, ror #27
0034b03c: andeq    r4, r0, ip, lsr #1
0034b040: ldrsheq  r5, [r7], #-0x64
0034b044: subseq   r5, r7, r0, asr #12
0034b048: ldrheq   r5, [r7], #-0x6c
0034b04c: subseq   r5, r7, ip, ror r6
0034b050: subseq   r5, r7, r4, ror #12
0034b054: strdeq   r3, r4, [r0], -r4
0034b058: andeq    r0, r0, r4, lsl #17
0034b05c: subseq   r5, r7, r4, lsr r5
0034b060: subseq   r5, r7, ip, lsr #8

# _ZNK9Character11IsCharacterEv
003a2e1c: mov      r0, #1
003a2e20: bx       lr

# _ZN6CharAI15RemoveFromGroupEv
003d2ff8: push     {r4, lr}
003d2ffc: ldr      r3, [r0, #0x34]
003d3000: sub      sp, sp, #0x18
003d3004: mov      r4, r0
003d3008: cmp      r3, #0
003d300c: beq      #0x3d307c
003d3010: ldr      r2, [r0, #0x38]
003d3014: cmp      r2, #3
003d3018: addls    pc, pc, r2, lsl #2
003d301c: b        #0x3d307c
003d3020: b        #0x3d30d4
003d3024: b        #0x3d3084
003d3028: b        #0x3d3030
003d302c: b        #0x3d30d4
003d3030: add      r2, sp, #0x18
003d3034: ldm      r3, {r0, r1}
003d3038: str      r4, [r2, #-0x18]!
003d303c: add      r3, sp, #0xc
003d3040: mov      r2, sp
003d3044: bl       #0x3d2504
003d3048: ldr      r4, [r4, #0x34]
003d304c: ldr      r3, [r4, #4]
003d3050: cmp      r0, r3
003d3054: beq      #0x3d307c
003d3058: add      r1, r0, #4
003d305c: cmp      r3, r1
003d3060: beq      #0x3d3074
003d3064: subs     r2, r3, r1
003d3068: beq      #0x3d3074
003d306c: bl       #0x30df38
003d3070: ldr      r3, [r4, #4]
003d3074: sub      r3, r3, #4
003d3078: str      r3, [r4, #4]
003d307c: add      sp, sp, #0x18
003d3080: pop      {r4, pc}
003d3084: add      r2, sp, #0x18
003d3088: ldr      r1, [r3, #0x10]
003d308c: ldr      r0, [r3, #0xc]
003d3090: str      r4, [r2, #-0x14]!
003d3094: add      r3, sp, #0x10
003d3098: bl       #0x3d2504
003d309c: ldr      r4, [r4, #0x34]
003d30a0: ldr      r3, [r4, #0x10]
003d30a4: cmp      r0, r3
003d30a8: beq      #0x3d307c
003d30ac: add      r1, r0, #4
003d30b0: cmp      r3, r1
003d30b4: beq      #0x3d30c8
003d30b8: subs     r2, r3, r1
003d30bc: beq      #0x3d30c8
003d30c0: bl       #0x30df38
003d30c4: ldr      r3, [r4, #0x10]
003d30c8: sub      r3, r3, #4
003d30cc: str      r3, [r4, #0x10]
003d30d0: b        #0x3d307c
003d30d4: add      r2, sp, #0x18
003d30d8: ldr      r1, [r3, #0x1c]
003d30dc: ldr      r0, [r3, #0x18]
003d30e0: str      r4, [r2, #-0x10]!
003d30e4: add      r3, sp, #0x14
003d30e8: bl       #0x3d2504
003d30ec: ldr      r4, [r4, #0x34]
003d30f0: ldr      r3, [r4, #0x1c]
003d30f4: cmp      r0, r3
003d30f8: beq      #0x3d307c
003d30fc: add      r1, r0, #4
003d3100: cmp      r3, r1
003d3104: beq      #0x3d3118
003d3108: subs     r2, r3, r1
003d310c: beq      #0x3d3118
003d3110: bl       #0x30df38
003d3114: ldr      r3, [r4, #0x1c]
003d3118: sub      r3, r3, #4
003d311c: str      r3, [r4, #0x1c]
003d3120: b        #0x3d307c

# _ZN13ObjectManager18RemoveNoRoomObjectEP10GameObject
003462b8: push     {r4, lr}
003462bc: ldr      r3, [r0, #0x88]!
003462c0: mov      r4, r1
003462c4: cmp      r3, r0
003462c8: beq      #0x3462e8
003462cc: ldr      r2, [r3, #8]
003462d0: cmp      r2, r4
003462d4: beq      #0x3462e8
003462d8: ldr      r3, [r3]
003462dc: cmp      r0, r3
003462e0: bne      #0x3462cc
003462e4: mov      r3, r0
003462e8: cmp      r0, r3
003462ec: beq      #0x346310
003462f0: ldm      r3, {r2, ip}
003462f4: mov      r0, r3
003462f8: mov      r1, #0xc
003462fc: str      r2, [ip]
00346300: str      ip, [r2, #4]
00346304: bl       #0x708f00
00346308: mov      r3, #0
0034630c: strb     r3, [r4, #0x2f8]
00346310: pop      {r4, pc}

# _ZN13ObjectManager21AssignObjectNetworkIdEP10ObjectBase
003431c0: push     {r4, r5, r6, r7, r8, lr}
003431c4: mov      r5, r0
003431c8: mov      r4, r1
003431cc: bl       #0x7fd794
003431d0: ldrb     r3, [r0, #5]
003431d4: ldr      r6, [pc, #0x110]
003431d8: cmp      r3, #0
003431dc: add      r6, pc, r6
003431e0: beq      #0x343274
003431e4: ldr      r7, [r4, #0x44]
003431e8: ldr      r1, [pc, #0x100]
003431ec: mov      r0, r7
003431f0: add      r1, pc, r1
003431f4: bl       #0x30ebd4
003431f8: cmp      r0, r7
003431fc: beq      #0x3432d0
00343200: ldr      r3, [r4]
00343204: mov      r0, r4
00343208: mov      lr, pc
0034320c: ldr      pc, [r3, #0x24]
00343210: cmp      r0, #0
00343214: bne      #0x343278
00343218: ldr      r0, [r5, #0x13c]
0034321c: add      r3, r0, #1
00343220: str      r3, [r5, #0x13c]
00343224: add      r0, r0, #5
00343228: cmp      r0, #4
0034322c: str      r0, [r4, #0x108]
00343230: bgt      #0x3432b4
00343234: add      r6, r5, #0x100
00343238: mov      r0, r6
0034323c: bl       #0x343168
00343240: str      r4, [r0, #8]
00343244: ldr      r3, [r5, #0x104]
00343248: str      r6, [r0]
0034324c: str      r3, [r0, #4]
00343250: str      r0, [r3]
00343254: str      r0, [r5, #0x104]
00343258: ldr      r3, [r4, #0x100]
0034325c: cmp      r3, #0
00343260: beq      #0x343274
00343264: ldr      r3, [r5, #0x54]
00343268: add      r3, r3, #1
0034326c: str      r3, [r5, #0x54]
00343270: pop      {r4, r5, r6, r7, r8, pc}
00343274: pop      {r4, r5, r6, r7, r8, pc}
00343278: movw     r3, #0x14e4
0034327c: ldrb     r3, [r4, r3]
00343280: cmp      r3, #0
00343284: beq      #0x3432a0
00343288: ldr      r0, [r5, #0x140]
0034328c: add      r3, r0, #1
00343290: add      r0, r0, #0x2700
00343294: str      r3, [r5, #0x140]
00343298: add      r0, r0, #0x11
0034329c: b        #0x343228
003432a0: mov      r0, r4
003432a4: bl       #0x3a30ac
003432a8: cmp      r0, #0
003432ac: beq      #0x343218
003432b0: b        #0x343288
003432b4: ldr      r3, [pc, #0x38]
003432b8: mov      r1, #1
003432bc: ldr      r3, [r6, r3]
003432c0: ldr      r3, [r3]
003432c4: str      r3, [r4, #0xfc]
003432c8: bl       #0x33ff90
003432cc: b        #0x343234
003432d0: add      r0, r0, #0x10
003432d4: bl       #0x30e094
003432d8: ldr      r3, [r5, #0x138]
003432dc: add      r0, r0, #1
003432e0: add      r3, r3, #1
003432e4: str      r3, [r5, #0x138]
003432e8: b        #0x343228
003432ec: strhteq  r1, [r5], #-0x84
003432f0: subseq   sp, r7, r8, asr r1
003432f4: andeq    r0, r0, r0, lsl fp

# _ZNK10ObjectBase11IsCharacterEv
0033dcd0: mov      r0, #0
0033dcd4: bx       lr

# _ZNK10GameObject12IsGameObjectEv
00340054: mov      r0, #1
00340058: bx       lr
