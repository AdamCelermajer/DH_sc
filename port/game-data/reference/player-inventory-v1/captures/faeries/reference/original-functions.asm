
# _ZN14PlayerSavegame21__LoadDifficultyLevelEP11IStreamBasePv
00468968: ldr      r3, [pc, #0x28]
0046896c: ldr      r2, [pc, #0x28]
00468970: push     {r4, r5, r6, lr}
00468974: add      r3, pc, r3
00468978: mov      r4, r1
0046897c: ldr      r1, [r3, r2]
00468980: mov      r5, r0
00468984: bl       #0x38b758
00468988: mov      r0, r5
0046898c: add      r1, r4, #0x3c
00468990: pop      {r4, r5, r6, lr}
00468994: b        #0x38b758
00468998: subseq   ip, r2, ip, lsl r1
0046899c: muleq    r0, ip, sl

# _ZN14PlayerSavegame13__LoadFaeriesEP11IStreamBasePv
004691d0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004691d4: ldr      r3, [pc, #0x138]
004691d8: sub      sp, sp, #0x2c
004691dc: ldr      sl, [pc, #0x134]
004691e0: str      r3, [sp, #0xc]
004691e4: ldr      r3, [pc, #0x130]
004691e8: mov      r7, r1
004691ec: mov      r5, r0
004691f0: str      r3, [sp, #0x10]
004691f4: ldr      r3, [pc, #0x124]
004691f8: mov      r4, r1
004691fc: mov      r6, #0
00469200: add      r3, pc, r3
00469204: str      r3, [sp, #0x14]
00469208: ldr      r3, [pc, #0x114]
0046920c: add      r8, sp, #0x24
00469210: add      sl, pc, sl
00469214: add      r3, pc, r3
00469218: str      r3, [sp, #0x18]
0046921c: ldr      r3, [pc, #0x104]
00469220: add      r3, pc, r3
00469224: str      r3, [sp, #0x1c]
00469228: ldr      r3, [r4, #0x94]
0046922c: cmp      r3, #0
00469230: beq      #0x4692bc
00469234: add      r1, r7, r6, lsl #2
00469238: add      r1, r1, #0xac
0046923c: mov      r0, r5
00469240: bl       #0x38b758
00469244: mov      r0, r5
00469248: mov      r1, r8
0046924c: bl       #0x313b48
00469250: ldr      r3, [r4, #0xa0]
00469254: ldr      r2, [sp, #0x24]
00469258: cmp      r3, r2
0046925c: bne      #0x4692b4
00469260: cmp      r3, #0
00469264: beq      #0x4692a4
00469268: mov      sb, #0
0046926c: ldr      r1, [r4, #0x94]
00469270: lsl      fp, sb, #2
00469274: mov      r0, r5
00469278: add      r1, r1, fp
0046927c: add      r1, r1, #2
00469280: bl       #0x469070
00469284: ldr      r1, [r4, #0x94]
00469288: mov      r0, r5
0046928c: add      sb, sb, #1
00469290: add      r1, r1, fp
00469294: bl       #0x469120
00469298: ldr      r3, [sp, #0x24]
0046929c: cmp      r3, sb
004692a0: bhi      #0x46926c
004692a4: add      r6, r6, #1
004692a8: cmp      r6, #3
004692ac: add      r4, r4, #4
004692b0: bne      #0x469228
004692b4: add      sp, sp, #0x2c
004692b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004692bc: ldr      r1, [sp, #0xc]
004692c0: ldr      r2, [sl, r1]
004692c4: ldr      r2, [r2]
004692c8: cmp      r2, #2
004692cc: beq      #0x46930c
004692d0: cmp      r2, #1
004692d4: bne      #0x4692b4
004692d8: ldr      r3, [sp, #0x10]
004692dc: movw     ip, #0x20d
004692e0: ldr      r1, [sp, #0x14]
004692e4: ldr      r0, [sl, r3]
004692e8: ldr      r2, [sp, #0x18]
004692ec: ldr      r3, [sp, #0x1c]
004692f0: add      r0, r0, #0xa8
004692f4: str      ip, [sp]
004692f8: bl       #0x30e004
004692fc: ldr      r3, [r4, #0x94]
00469300: cmp      r3, #0
00469304: bne      #0x469234
00469308: b        #0x4692b4
0046930c: str      r3, [r3]
00469310: b        #0x4692b4
00469314: andeq    r3, r0, r0, asr #19
00469318: subseq   fp, r2, r0, lsl #17
0046931c: andeq    r1, r0, r0, asr #19
00469320: ldrdeq   r5, r6, [r5], #-0x18
00469324: subeq    r4, r6, ip, ror #5
00469328: subeq    r4, r6, r8, lsl #5

# _ZNK14PlayerSavegame17SG_GetFaerieLevelEji
00466700: push     {r4, r5, r6, r7, lr}
00466704: add      r7, r2, #0x28
00466708: mov      r6, r2
0046670c: ldr      r2, [r0, r7, lsl #2]
00466710: ldr      r3, [pc, #0x94]
00466714: sub      sp, sp, #0xc
00466718: cmp      r2, r1
0046671c: mov      r5, r0
00466720: mov      r4, r1
00466724: add      r3, pc, r3
00466728: bhi      #0x466798
0046672c: ldr      r2, [pc, #0x7c]
00466730: ldr      r2, [r3, r2]
00466734: ldr      r2, [r2]
00466738: cmp      r2, #2
0046673c: moveq    r0, #0
00466740: streq    r0, [r0]
00466744: beq      #0x466754
00466748: cmp      r2, #1
0046674c: beq      #0x46675c
00466750: mov      r0, #0
00466754: add      sp, sp, #0xc
00466758: pop      {r4, r5, r6, r7, pc}
0046675c: ldr      r0, [pc, #0x50]
00466760: ldr      r1, [pc, #0x50]
00466764: ldr      r2, [pc, #0x50]
00466768: ldr      r0, [r3, r0]
0046676c: ldr      r3, [pc, #0x4c]
00466770: mov      ip, #0x128
00466774: add      r1, pc, r1
00466778: add      r3, pc, r3
0046677c: add      r0, r0, #0xa8
00466780: add      r2, pc, r2
00466784: str      ip, [sp]
00466788: bl       #0x30e004
0046678c: ldr      r3, [r5, r7, lsl #2]
00466790: cmp      r4, r3
00466794: bhs      #0x466750
00466798: add      r5, r5, r6, lsl #2
0046679c: ldr      r3, [r5, #0x94]
004667a0: add      r4, r3, r4, lsl #2
004667a4: ldrh     r0, [r4, #2]
004667a8: b        #0x466754
004667ac: subseq   lr, r2, ip, ror #6
004667b0: andeq    r3, r0, r0, asr #19
004667b4: andeq    r1, r0, r0, asr #19
004667b8: subeq    r7, r5, r4, ror #24
004667bc: subeq    r6, r6, r0, ror #21
004667c0: subeq    r6, r6, r8, lsl #22

# _ZN14PlayerSavegame12_InitFaeriesEv
004694c8: push     {r4, r5, r6, r7, r8, lr}
004694cc: mov      r5, #0
004694d0: mov      r4, r0
004694d4: mov      r8, #5
004694d8: mov      r7, r5
004694dc: ldr      r6, [r4, #0x94]
004694e0: cmp      r6, #0
004694e4: beq      #0x4694fc
004694e8: add      r5, r5, #1
004694ec: cmp      r5, #3
004694f0: add      r4, r4, #4
004694f4: bne      #0x4694dc
004694f8: pop      {r4, r5, r6, r7, r8, pc}
004694fc: str      r8, [r4, #0xa0]
00469500: mov      r0, #0x14
00469504: mov      r1, r6
00469508: bl       #0x31056c
0046950c: ldr      r3, [r4, #0xa0]
00469510: str      r0, [r4, #0x94]
00469514: cmp      r3, #0
00469518: bne      #0x469524
0046951c: b        #0x4694e8
00469520: ldr      r0, [r4, #0x94]
00469524: add      r0, r0, r6, lsl #2
00469528: mov      r3, #0
0046952c: strh     r3, [r0, #2]
00469530: ldr      r3, [r4, #0x94]
00469534: strb     r7, [r3, r6, lsl #2]
00469538: ldr      r3, [r4, #0xa0]
0046953c: add      r6, r6, #1
00469540: cmp      r3, r6
00469544: bhi      #0x469520
00469548: b        #0x4694e8

# _ZN14PlayerSavegame17SG_SetFaerieStateEjii
00466588: push     {r4, r5, r6, r7, lr}
0046658c: mov      r6, r3
00466590: add      r3, r3, #0x28
00466594: mov      r5, r0
00466598: ldr      r0, [r0, r3, lsl #2]
0046659c: ldr      r3, [pc, #0x80]
004665a0: sub      sp, sp, #0xc
004665a4: cmp      r0, r1
004665a8: mov      r4, r1
004665ac: add      r3, pc, r3
004665b0: mov      r7, r2
004665b4: bhi      #0x4665dc
004665b8: ldr      r2, [pc, #0x68]
004665bc: ldr      r2, [r3, r2]
004665c0: ldr      r2, [r2]
004665c4: cmp      r2, #2
004665c8: moveq    r3, #0
004665cc: streq    r3, [r3]
004665d0: beq      #0x4665dc
004665d4: cmp      r2, #1
004665d8: beq      #0x4665f0
004665dc: add      r5, r5, r6, lsl #2
004665e0: ldr      r3, [r5, #0x94]
004665e4: strb     r7, [r3, r4, lsl #2]
004665e8: add      sp, sp, #0xc
004665ec: pop      {r4, r5, r6, r7, pc}
004665f0: ldr      r0, [pc, #0x34]
004665f4: ldr      r1, [pc, #0x34]
004665f8: ldr      r2, [pc, #0x34]
004665fc: ldr      r0, [r3, r0]
00466600: ldr      r3, [pc, #0x30]
00466604: movw     ip, #0x14a
00466608: add      r1, pc, r1
0046660c: add      r2, pc, r2
00466610: add      r3, pc, r3
00466614: add      r0, r0, #0xa8
00466618: str      ip, [sp]
0046661c: bl       #0x30e004
00466620: b        #0x4665dc
00466624: subseq   lr, r2, r4, ror #9
00466628: andeq    r3, r0, r0, asr #19
0046662c: andeq    r1, r0, r0, asr #19
00466630: ldrdeq   r7, r8, [r5], #-0xd0
00466634: subeq    r6, r6, r4, asr ip
00466638: subeq    r6, r6, r0, ror ip

# _ZN14PlayerSavegame18__LoadCurrentFaeryEP11IStreamBasePv
00468cd4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00468cd8: ldr      r3, [pc, #0xc0]
00468cdc: sub      sp, sp, #0x14
00468ce0: ldr      r8, [pc, #0xbc]
00468ce4: add      r3, pc, r3
00468ce8: str      r3, [sp, #8]
00468cec: ldr      r3, [pc, #0xb4]
00468cf0: ldr      fp, [pc, #0xb4]
00468cf4: ldr      sl, [pc, #0xb4]
00468cf8: add      r3, pc, r3
00468cfc: ldr      sb, [pc, #0xb0]
00468d00: mov      r6, r1
00468d04: mov      r7, r0
00468d08: add      fp, pc, fp
00468d0c: str      r3, [sp, #0xc]
00468d10: mov      r5, r1
00468d14: mov      r4, #0
00468d18: add      r8, pc, r8
00468d1c: ldr      r3, [r5, #0x94]
00468d20: cmp      r3, #0
00468d24: beq      #0x468d50
00468d28: add      r1, r6, r4, lsl #2
00468d2c: add      r1, r1, #0xac
00468d30: add      r4, r4, #1
00468d34: mov      r0, r7
00468d38: bl       #0x38b758
00468d3c: cmp      r4, #3
00468d40: add      r5, r5, #4
00468d44: bne      #0x468d1c
00468d48: add      sp, sp, #0x14
00468d4c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00468d50: ldr      r2, [r8, sl]
00468d54: ldr      r2, [r2]
00468d58: cmp      r2, #2
00468d5c: beq      #0x468d98
00468d60: cmp      r2, #1
00468d64: bne      #0x468d48
00468d68: ldr      r0, [r8, sb]
00468d6c: ldr      r3, [sp, #0xc]
00468d70: movw     ip, #0x245
00468d74: mov      r1, fp
00468d78: ldr      r2, [sp, #8]
00468d7c: add      r0, r0, #0xa8
00468d80: str      ip, [sp]
00468d84: bl       #0x30e004
00468d88: ldr      r3, [r5, #0x94]
00468d8c: cmp      r3, #0
00468d90: bne      #0x468d28
00468d94: b        #0x468d48
00468d98: str      r3, [r3]
00468d9c: b        #0x468d48
00468da0: subeq    r4, r6, ip, lsl r8
00468da4: subseq   fp, r2, r8, ror sp
00468da8: strheq   r4, [r6], #-0x70
00468dac: ldrdeq   r5, r6, [r5], #-0x60
00468db0: andeq    r3, r0, r0, asr #19
00468db4: andeq    r1, r0, r0, asr #19

# _ZN14PlayerSavegame17SG_SetFaerieLevelEjii
0046663c: push     {r4, r5, r6, r7, r8, lr}
00466640: add      r7, r3, #0x28
00466644: mov      r5, r0
00466648: ldr      r0, [r0, r7, lsl #2]
0046664c: mov      r6, r3
00466650: ldr      r3, [pc, #0x90]
00466654: cmp      r0, r1
00466658: sub      sp, sp, #8
0046665c: mov      r4, r1
00466660: add      r3, pc, r3
00466664: mov      r8, r2
00466668: bhi      #0x4666d4
0046666c: ldr      r2, [pc, #0x78]
00466670: ldr      r2, [r3, r2]
00466674: ldr      r2, [r2]
00466678: cmp      r2, #2
0046667c: moveq    r3, #0
00466680: streq    r3, [r3]
00466684: beq      #0x466690
00466688: cmp      r2, #1
0046668c: beq      #0x466698
00466690: add      sp, sp, #8
00466694: pop      {r4, r5, r6, r7, r8, pc}
00466698: ldr      r0, [pc, #0x50]
0046669c: ldr      r1, [pc, #0x50]
004666a0: ldr      r2, [pc, #0x50]
004666a4: ldr      r0, [r3, r0]
004666a8: ldr      r3, [pc, #0x4c]
004666ac: movw     ip, #0x133
004666b0: add      r1, pc, r1
004666b4: add      r3, pc, r3
004666b8: add      r0, r0, #0xa8
004666bc: add      r2, pc, r2
004666c0: str      ip, [sp]
004666c4: bl       #0x30e004
004666c8: ldr      r3, [r5, r7, lsl #2]
004666cc: cmp      r4, r3
004666d0: bhs      #0x466690
004666d4: add      r5, r5, r6, lsl #2
004666d8: ldr      r3, [r5, #0x94]
004666dc: add      r4, r3, r4, lsl #2
004666e0: strh     r8, [r4, #2]
004666e4: b        #0x466690
004666e8: subseq   lr, r2, r0, lsr r4
004666ec: andeq    r3, r0, r0, asr #19
004666f0: andeq    r1, r0, r0, asr #19
004666f4: subeq    r7, r5, r8, lsr #26
004666f8: subeq    r6, r6, r4, lsr #23
004666fc: subeq    r6, r6, ip, asr #23
