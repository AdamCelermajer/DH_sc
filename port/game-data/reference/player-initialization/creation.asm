
# _ZN14PlayerSavegameC1Ejib
004655ac: ldr      r3, [pc, #0x100]
004655b0: push     {r4, r5, r6, lr}
004655b4: ldr      lr, [pc, #0xfc]
004655b8: add      r3, pc, r3
004655bc: mov      r4, r0
004655c0: ldr      lr, [r3, lr]
004655c4: mov      r5, #0
004655c8: add      ip, r0, #0x18
004655cc: add      lr, lr, #8
004655d0: str      lr, [r0]
004655d4: str      r1, [r0, #4]
004655d8: mov      r0, ip
004655dc: str      ip, [r4, #0x28]
004655e0: str      ip, [r4, #0x2c]
004655e4: mov      r1, #0x10
004655e8: str      r5, [r4, #8]
004655ec: strb     r5, [r4, #0xc]
004655f0: str      r5, [r4, #0x10]
004655f4: strb     r5, [r4, #0x14]
004655f8: mov      r6, r2
004655fc: bl       #0x31167c
00465600: ldr      r3, [r4, #0x28]
00465604: add      r0, r4, #0xb8
00465608: strb     r5, [r3]
0046560c: mov      r3, #1
00465610: str      r3, [r4, #0x30]
00465614: mvn      r3, #0
00465618: str      r3, [r4, #0x34]
0046561c: str      r5, [r4, #0x3c]
00465620: str      r5, [r4, #0x80]
00465624: str      r5, [r4, #0x84]
00465628: str      r5, [r4, #0x88]
0046562c: str      r5, [r4, #0x8c]
00465630: str      r5, [r4, #0x90]
00465634: bl       #0x46b0a4
00465638: add      r0, r4, #0x118
0046563c: bl       #0x46b0a4
00465640: add      r2, r4, #0x17c
00465644: mov      r3, r5
00465648: str      r3, [r2, r5]
0046564c: add      r1, r2, r5
00465650: add      r5, r5, #8
00465654: cmp      r5, #0x18
00465658: str      r3, [r1, #4]
0046565c: bne      #0x465648
00465660: mov      r1, r3
00465664: strb     r3, [r4, #0x194]
00465668: mov      r2, r1
0046566c: mov      r3, r4
00465670: add      r1, r1, #1
00465674: cmp      r1, #3
00465678: str      r2, [r3, #0x94]
0046567c: str      r2, [r3, #0xa0]
00465680: str      r2, [r3, #0xac]
00465684: str      r2, [r3, #0x40]
00465688: str      r2, [r3, #0x68]
0046568c: str      r2, [r3, #0x74]
00465690: str      r2, [r3, #0x5c]
00465694: str      r2, [r3, #0x50]
00465698: add      r3, r3, #4
0046569c: bne      #0x465670
004656a0: mov      r0, r4
004656a4: mov      r1, r6
004656a8: bl       #0x465430
004656ac: mov      r0, r4
004656b0: pop      {r4, r5, r6, pc}
004656b4: ldrsbeq  pc, [r2], #-0x48
004656b8: strheq   r4, [r0], -ip

# _ZN9Character12CreatePlayerEjjjPKcbb
003acea4: push     {r4, r5, r6, r7, lr}
003acea8: ldr      ip, [pc, #0xcc]
003aceac: ldr      r2, [pc, #0xcc]
003aceb0: sub      sp, sp, #0x1c
003aceb4: add      ip, pc, ip
003aceb8: ldr      r1, [ip, r2]
003acebc: ldr      r2, [pc, #0xc0]
003acec0: add      r4, sp, #0xc
003acec4: ldr      r1, [r1, #0x38]
003acec8: mov      lr, #1
003acecc: add      r2, pc, r2
003aced0: mov      r7, r0
003aced4: mov      r0, r4
003aced8: str      lr, [sp, #4]
003acedc: str      lr, [sp]
003acee0: ldrb     r6, [sp, #0x30]
003acee4: ldrb     r5, [sp, #0x34]
003acee8: bl       #0x34b724
003aceec: mov      r0, r4
003acef0: bl       #0x33ff54
003acef4: subs     r4, r0, #0
003acef8: beq      #0x3acf50
003acefc: ldr      r3, [r4]
003acf00: mov      lr, pc
003acf04: ldr      pc, [r3, #0x28]
003acf08: cmp      r0, #0
003acf0c: bne      #0x3acf70
003acf10: cmp      r6, #0
003acf14: moveq    r0, r4
003acf18: movne    r0, r4
003acf1c: moveq    r1, r7
003acf20: mvnne    r1, #0
003acf24: bl       #0x3bb740
003acf28: mov      r0, r4
003acf2c: ldr      r3, [r4]
003acf30: mov      lr, pc
003acf34: ldr      pc, [r3, #0x1c]
003acf38: ldr      r3, [r4]
003acf3c: mov      r0, r4
003acf40: mov      lr, pc
003acf44: ldr      pc, [r3, #0x58]
003acf48: cmp      r5, #0
003acf4c: beq      #0x3acf5c
003acf50: mov      r0, r4
003acf54: add      sp, sp, #0x1c
003acf58: pop      {r4, r5, r6, r7, pc}
003acf5c: add      r0, r4, #0x4f0
003acf60: add      r0, r0, #0xc
003acf64: mov      r1, r5
003acf68: bl       #0x3c1a00
003acf6c: b        #0x3acf50
003acf70: mov      r0, r4
003acf74: bl       #0x3b36b0
003acf78: b        #0x3acf10
003acf7c: ldrsbeq  r7, [lr], #-0xbc
003acf80: strdeq   r3, r4, [r0], -r4
003acf84: subseq   r3, r1, ip, ror #11

# _ZN9Character17SG_SetPlayerClassEi
003bb814: movw     r3, #0x14e8
003bb818: ldr      r3, [r0, r3]
003bb81c: cmp      r3, #0
003bb820: strne    r1, [r3, #0x34]
003bb824: bx       lr

# _ZN14PlayerSavegame17__LoadPlayerClassEP11IStreamBasePv
00469d88: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00469d8c: ldr      r8, [pc, #0xc8]
00469d90: ldr      sb, [pc, #0xc8]
00469d94: sub      sp, sp, #0x24
00469d98: add      r8, pc, r8
00469d9c: ldr      r3, [r8, sb]
00469da0: add      sl, sp, #4
00469da4: mov      r5, r0
00469da8: ldr      r3, [r3]
00469dac: mov      r0, sl
00469db0: mov      fp, r1
00469db4: mov      r1, #0x10
00469db8: str      r3, [sp, #0x1c]
00469dbc: str      sl, [sp, #0x14]
00469dc0: str      sl, [sp, #0x18]
00469dc4: bl       #0x31167c
00469dc8: ldr      r3, [sp, #0x14]
00469dcc: mov      r4, #0
00469dd0: mov      r0, r5
00469dd4: strb     r4, [r3]
00469dd8: mov      r1, sl
00469ddc: bl       #0x461da8
00469de0: ldr      r3, [pc, #0x7c]
00469de4: ldr      r6, [sp, #0x18]
00469de8: ldr      r3, [r8, r3]
00469dec: ldr      r5, [r3]
00469df0: cmp      r5, r4
00469df4: beq      #0x469e50
00469df8: ldr      r3, [pc, #0x68]
00469dfc: ldr      r3, [r8, r3]
00469e00: ldr      r7, [r3]
00469e04: b        #0x469e14
00469e08: add      r4, r4, #1
00469e0c: cmp      r4, r5
00469e10: beq      #0x469e50
00469e14: mov      r0, r6
00469e18: ldr      r1, [r7, r4, lsl #2]
00469e1c: bl       #0x30e31c
00469e20: cmp      r0, #0
00469e24: bne      #0x469e08
00469e28: str      r4, [fp, #0x34]
00469e2c: mov      r0, sl
00469e30: bl       #0x3139ac
00469e34: ldr      r3, [r8, sb]
00469e38: ldr      r2, [sp, #0x1c]
00469e3c: ldr      r3, [r3]
00469e40: cmp      r2, r3
00469e44: bne      #0x469e58
00469e48: add      sp, sp, #0x24
00469e4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00469e50: mvn      r4, #0
00469e54: b        #0x469e28
00469e58: bl       #0x30e310
00469e5c: ldrsheq  sl, [r2], #-0xc8
00469e60: andeq    r4, r0, ip, lsr #1
00469e64: andeq    r4, r0, r4, lsl #4
00469e68: andeq    r3, r0, r8, lsl #24

# _ZN14PlayerSavegameC2Ev
00465c40: ldr      r3, [pc, #0x150]
00465c44: ldr      r1, [pc, #0x150]
00465c48: push     {r4, r5, r6, r7, r8, lr}
00465c4c: add      r3, pc, r3
00465c50: ldr      r1, [r3, r1]
00465c54: mov      r4, r0
00465c58: mov      r5, #0
00465c5c: add      r2, r0, #0x18
00465c60: add      r1, r1, #8
00465c64: mvn      r6, #0
00465c68: str      r1, [r0]
00465c6c: sub      sp, sp, #0x18
00465c70: mov      r0, r2
00465c74: str      r2, [r4, #0x28]
00465c78: str      r2, [r4, #0x2c]
00465c7c: mov      r1, #0x10
00465c80: str      r6, [r4, #4]
00465c84: str      r5, [r4, #8]
00465c88: strb     r5, [r4, #0xc]
00465c8c: str      r5, [r4, #0x10]
00465c90: strb     r5, [r4, #0x14]
00465c94: bl       #0x31167c
00465c98: ldr      r3, [r4, #0x28]
00465c9c: add      r0, r4, #0xb8
00465ca0: strb     r5, [r3]
00465ca4: str      r6, [r4, #0x34]
00465ca8: str      r5, [r4, #0x30]
00465cac: str      r5, [r4, #0x3c]
00465cb0: str      r5, [r4, #0x80]
00465cb4: str      r5, [r4, #0x84]
00465cb8: str      r5, [r4, #0x88]
00465cbc: str      r5, [r4, #0x8c]
00465cc0: str      r5, [r4, #0x90]
00465cc4: bl       #0x46b0a4
00465cc8: add      r0, r4, #0x118
00465ccc: bl       #0x46b0a4
00465cd0: mov      r3, r5
00465cd4: str      r5, [r4, #0x178]
00465cd8: add      r1, r4, #0x17c
00465cdc: mov      r2, r5
00465ce0: str      r2, [r1, r3]
00465ce4: add      r0, r1, r3
00465ce8: add      r3, r3, #8
00465cec: cmp      r3, #0x18
00465cf0: str      r2, [r0, #4]
00465cf4: bne      #0x465ce0
00465cf8: mov      r5, r2
00465cfc: strb     r2, [r4, #0x194]
00465d00: add      r8, r4, #0x88
00465d04: mov      r6, sp
00465d08: mov      r7, r2
00465d0c: mov      r0, r8
00465d10: mov      r1, sp
00465d14: str      r7, [sp, #4]
00465d18: strb     r7, [sp]
00465d1c: str      r6, [sp, #8]
00465d20: str      r6, [sp, #0xc]
00465d24: str      r7, [sp, #0x10]
00465d28: bl       #0x465a48
00465d2c: ldr      r3, [sp, #0x10]
00465d30: add      r5, r5, #1
00465d34: cmp      r3, #0
00465d38: beq      #0x465d48
00465d3c: mov      r0, sp
00465d40: ldr      r1, [sp, #4]
00465d44: bl       #0x345c94
00465d48: cmp      r5, #2
00465d4c: bne      #0x465d0c
00465d50: mov      r1, #0
00465d54: mov      r3, r4
00465d58: mov      r2, r1
00465d5c: add      r1, r1, #1
00465d60: cmp      r1, #3
00465d64: str      r2, [r3, #0x94]
00465d68: str      r2, [r3, #0xa0]
00465d6c: str      r2, [r3, #0xac]
00465d70: str      r2, [r3, #0x40]
00465d74: str      r2, [r3, #0x68]
00465d78: str      r2, [r3, #0x74]
00465d7c: str      r2, [r3, #0x5c]
00465d80: str      r2, [r3, #0x50]
00465d84: add      r3, r3, #4
00465d88: bne      #0x465d5c
00465d8c: mov      r0, r4
00465d90: add      sp, sp, #0x18
00465d94: pop      {r4, r5, r6, r7, r8, pc}
00465d98: subseq   lr, r2, r4, asr #28
00465d9c: strheq   r4, [r0], -ip

# _ZN9Character7InitAllEv
003b35f0: push     {r4, lr}
003b35f4: mov      r4, r0
003b35f8: ldr      r3, [r0]
003b35fc: mov      lr, pc
003b3600: ldr      pc, [r3, #0x1c]
003b3604: mov      r0, r4
003b3608: ldr      r3, [r4]
003b360c: mov      lr, pc
003b3610: ldr      pc, [r3, #0x58]
003b3614: pop      {r4, pc}

# _ZN9Character12CreatePlayerEPS_jjPKc
003acd58: push     {r4, r5, r6, lr}
003acd5c: ldr      r2, [pc, #0x120]
003acd60: subs     r5, r0, #0
003acd64: sub      sp, sp, #0x18
003acd68: mov      r6, r3
003acd6c: add      r2, pc, r2
003acd70: beq      #0x3ace24
003acd74: ldr      r1, [pc, #0x10c]
003acd78: add      r4, sp, #0xc
003acd7c: mov      ip, #1
003acd80: ldr      r1, [r2, r1]
003acd84: ldr      r2, [pc, #0x100]
003acd88: mov      r0, r4
003acd8c: ldr      r1, [r1, #0x38]
003acd90: add      r2, pc, r2
003acd94: str      ip, [sp, #4]
003acd98: str      ip, [sp]
003acd9c: bl       #0x34b724
003acda0: mov      r0, r4
003acda4: bl       #0x33ff54
003acda8: subs     r4, r0, #0
003acdac: beq      #0x3ace18
003acdb0: add      r1, r5, #4
003acdb4: add      r0, r4, #4
003acdb8: bl       #0x5138fc
003acdbc: mov      r0, r6
003acdc0: bl       #0x30de54
003acdc4: mov      r1, r6
003acdc8: add      r2, r6, r0
003acdcc: add      r0, r4, #0x30
003acdd0: bl       #0x3109e0
003acdd4: mov      r0, r5
003acdd8: bl       #0x3bb728
003acddc: mov      r1, r0
003acde0: mov      r0, r4
003acde4: bl       #0x3bb740
003acde8: mov      r0, r4
003acdec: ldr      r3, [r4]
003acdf0: mov      lr, pc
003acdf4: ldr      pc, [r3, #0x1c]
003acdf8: mov      r0, r4
003acdfc: ldr      r3, [r4]
003ace00: mov      lr, pc
003ace04: ldr      pc, [r3, #0x58]
003ace08: add      r0, r4, #0x4f0
003ace0c: add      r0, r0, #0xc
003ace10: mov      r1, #0
003ace14: bl       #0x3c1a00
003ace18: mov      r0, r4
003ace1c: add      sp, sp, #0x18
003ace20: pop      {r4, r5, r6, pc}
003ace24: ldr      r3, [pc, #0x64]
003ace28: ldr      r3, [r2, r3]
003ace2c: ldr      r3, [r3]
003ace30: cmp      r3, #2
003ace34: streq    r5, [r5]
003ace38: moveq    r4, r5
003ace3c: beq      #0x3ace18
003ace40: cmp      r3, #1
003ace44: movne    r4, r5
003ace48: bne      #0x3ace18
003ace4c: ldr      r0, [pc, #0x40]
003ace50: ldr      r1, [pc, #0x40]
003ace54: ldr      r3, [pc, #0x40]
003ace58: ldr      r0, [r2, r0]
003ace5c: ldr      r2, [pc, #0x3c]
003ace60: mov      ip, #0x3a
003ace64: add      r1, pc, r1
003ace68: add      r0, r0, #0xa8
003ace6c: add      r2, pc, r2
003ace70: add      r3, pc, r3
003ace74: str      ip, [sp]
003ace78: mov      r4, r5
003ace7c: bl       #0x30e004
003ace80: b        #0x3ace18
003ace84: subseq   r7, lr, r4, lsr #26
003ace88: strdeq   r3, r4, [r0], -r4
003ace8c: subseq   r3, r1, r8, lsr #14
003ace90: andeq    r3, r0, r0, asr #19
003ace94: andeq    r1, r0, r0, asr #19
003ace98: subseq   r1, r1, r4, ror r5
003ace9c: subseq   r6, r1, r8, lsr #17
003acea0: subseq   r6, r1, r4, lsr #17

# _ZN9Character24InitializePlayerSavegameEv
003b36b0: push     {r4, r5, r6, lr}
003b36b4: mov      r1, #0
003b36b8: mov      r4, r0
003b36bc: mov      r0, #0x198
003b36c0: bl       #0x310570
003b36c4: mov      r5, r0
003b36c8: bl       #0x465ae0
003b36cc: movw     r3, #0x14e8
003b36d0: mov      r0, r4
003b36d4: mov      r1, r4
003b36d8: str      r5, [r4, r3]
003b36dc: pop      {r4, r5, r6, lr}
003b36e0: b        #0x3bb754
