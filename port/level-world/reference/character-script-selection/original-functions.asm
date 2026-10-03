
# _ZN6CharAI15SetScriptByNameEPKc
003ceeb0: push     {r4, r5, r6, r7, r8, sl, lr}
003ceeb4: ldr      r4, [pc, #0x16c]
003ceeb8: ldr      r5, [pc, #0x16c]
003ceebc: mov      r6, r1
003ceec0: add      r4, pc, r4
003ceec4: ldr      r3, [r4, r5]
003ceec8: ldr      r1, [pc, #0x160]
003ceecc: sub      sp, sp, #0x24
003ceed0: ldr      r3, [r3]
003ceed4: mov      r7, r0
003ceed8: add      r1, pc, r1
003ceedc: mov      r0, r6
003ceee0: mov      r2, #2
003ceee4: str      r3, [sp, #0x1c]
003ceee8: bl       #0x30ec7c
003ceeec: subs     r8, r0, #0
003ceef0: bne      #0x3cef64
003ceef4: ldr      r1, [pc, #0x138]
003ceef8: add      r6, r6, #2
003ceefc: mov      r0, r6
003cef00: add      r1, pc, r1
003cef04: bl       #0x30e31c
003cef08: cmp      r0, #0
003cef0c: beq      #0x3cf00c
003cef10: ldr      r1, [pc, #0x120]
003cef14: mov      r0, r6
003cef18: add      r1, pc, r1
003cef1c: bl       #0x30e31c
003cef20: cmp      r0, #0
003cef24: beq      #0x3ceff8
003cef28: ldr      r1, [pc, #0x10c]
003cef2c: mov      r0, r6
003cef30: add      r1, pc, r1
003cef34: bl       #0x30e31c
003cef38: cmp      r0, #0
003cef3c: beq      #0x3cf018
003cef40: ldr      r1, [pc, #0xf8]
003cef44: mov      r0, r6
003cef48: add      r1, pc, r1
003cef4c: bl       #0x30e31c
003cef50: cmp      r0, #0
003cef54: bne      #0x3cefe8
003cef58: mov      r0, r7
003cef5c: bl       #0x3cce14
003cef60: b        #0x3cefcc
003cef64: mov      r0, r7
003cef68: bl       #0x3ccaf4
003cef6c: ldr      r3, [pc, #0xd0]
003cef70: add      r8, sp, #4
003cef74: ldr      sl, [r4, r3]
003cef78: mov      r0, sl
003cef7c: bl       #0x337888
003cef80: ldr      r1, [pc, #0xc0]
003cef84: mov      r2, sp
003cef88: mov      r0, r8
003cef8c: add      r1, pc, r1
003cef90: bl       #0x3140ec
003cef94: mov      r0, sl
003cef98: mov      r1, r8
003cef9c: bl       #0x337a88
003cefa0: ldr      r0, [sp, #0x18]
003cefa4: cmp      r0, r8
003cefa8: beq      #0x3cefc8
003cefac: cmp      r0, #0
003cefb0: beq      #0x3cefc8
003cefb4: ldr      r1, [sp, #4]
003cefb8: rsb      r1, r0, r1
003cefbc: cmp      r1, #0x80
003cefc0: bhi      #0x3cf004
003cefc4: bl       #0x708f00
003cefc8: str      r6, [r7, #0x30]
003cefcc: ldr      r3, [r4, r5]
003cefd0: ldr      r2, [sp, #0x1c]
003cefd4: ldr      r3, [r3]
003cefd8: cmp      r2, r3
003cefdc: bne      #0x3cf024
003cefe0: add      sp, sp, #0x24
003cefe4: pop      {r4, r5, r6, r7, r8, sl, pc}
003cefe8: mov      r0, r7
003cefec: bl       #0x3cce14
003ceff0: str      r8, [r7, #0x30]
003ceff4: b        #0x3cefcc
003ceff8: mov      r0, r7
003ceffc: bl       #0x3ccfe4
003cf000: b        #0x3cefcc
003cf004: bl       #0x310440
003cf008: b        #0x3cefc8
003cf00c: mov      r0, r7
003cf010: bl       #0x3ccbe4
003cf014: b        #0x3cefcc
003cf018: mov      r0, r7
003cf01c: bl       #0x3cccf8
003cf020: b        #0x3cefcc
003cf024: bl       #0x30e310
003cf028: ldrsbeq  r5, [ip], #-0xb0
003cf02c: andeq    r4, r0, ip, lsr #1
003cf030: strheq   r6, [pc], #-0x48
003cf034: umaaleq  r6, pc, r8, r4
003cf038: umaaleq  r6, pc, r0, r4
003cf03c: subeq    r6, pc, r8, lsl #9
003cf040: subeq    r6, pc, r8, ror r4
003cf044: andeq    r0, r0, r4, lsl #17
003cf048: subeq    r6, pc, ip, asr #7

# _ZN6CharAI16StepCreateScriptEv
003cf04c: push     {r4, r5, r6, r7, r8, sl, lr}
003cf050: ldr      r4, [pc, #0x17c]
003cf054: ldr      r6, [pc, #0x17c]
003cf058: ldr      r2, [pc, #0x17c]
003cf05c: add      r4, pc, r4
003cf060: ldr      r3, [r4, r6]
003cf064: ldr      r2, [r4, r2]
003cf068: sub      sp, sp, #0x44
003cf06c: ldr      r3, [r3]
003cf070: mov      r5, r0
003cf074: ldr      r0, [r0, #4]
003cf078: ldr      r7, [r2]
003cf07c: str      r3, [sp, #0x3c]
003cf080: bl       #0x3a2fec
003cf084: mov      r3, #0x44
003cf088: mla      r7, r3, r0, r7
003cf08c: ldr      r3, [r7, #0x28]
003cf090: cmp      r3, #0
003cf094: beq      #0x3cf124
003cf098: ldr      r3, [pc, #0x140]
003cf09c: add      r8, sp, #0x24
003cf0a0: ldr      sl, [r4, r3]
003cf0a4: mov      r0, sl
003cf0a8: bl       #0x337888
003cf0ac: ldr      r1, [pc, #0x130]
003cf0b0: add      r2, sp, #8
003cf0b4: mov      r0, r8
003cf0b8: add      r1, pc, r1
003cf0bc: bl       #0x3140ec
003cf0c0: mov      r0, sl
003cf0c4: mov      r1, r8
003cf0c8: bl       #0x337a88
003cf0cc: ldr      r0, [sp, #0x38]
003cf0d0: cmp      r0, r8
003cf0d4: beq      #0x3cf0f4
003cf0d8: cmp      r0, #0
003cf0dc: beq      #0x3cf0f4
003cf0e0: ldr      r1, [sp, #0x24]
003cf0e4: rsb      r1, r0, r1
003cf0e8: cmp      r1, #0x80
003cf0ec: bhi      #0x3cf1c8
003cf0f0: bl       #0x708f00
003cf0f4: ldr      r1, [r7, #0x2c]
003cf0f8: mov      r0, r5
003cf0fc: bl       #0x3ceeb0
003cf100: mov      r3, #1
003cf104: strb     r3, [r5, #0x2c]
003cf108: ldr      r3, [r4, r6]
003cf10c: ldr      r2, [sp, #0x3c]
003cf110: ldr      r3, [r3]
003cf114: cmp      r2, r3
003cf118: bne      #0x3cf1d0
003cf11c: add      sp, sp, #0x44
003cf120: pop      {r4, r5, r6, r7, r8, sl, pc}
003cf124: ldr      r3, [pc, #0xb4]
003cf128: add      r7, sp, #0xc
003cf12c: ldr      r8, [r4, r3]
003cf130: mov      r0, r8
003cf134: bl       #0x337888
003cf138: ldr      r1, [pc, #0xa8]
003cf13c: add      r2, sp, #4
003cf140: mov      r0, r7
003cf144: add      r1, pc, r1
003cf148: bl       #0x3140ec
003cf14c: mov      r0, r8
003cf150: mov      r1, r7
003cf154: bl       #0x337a88
003cf158: ldr      r0, [sp, #0x20]
003cf15c: cmp      r0, r7
003cf160: beq      #0x3cf180
003cf164: cmp      r0, #0
003cf168: beq      #0x3cf180
003cf16c: ldr      r1, [sp, #0xc]
003cf170: rsb      r1, r0, r1
003cf174: cmp      r1, #0x80
003cf178: bhi      #0x3cf1c0
003cf17c: bl       #0x708f00
003cf180: ldr      r3, [r5, #4]
003cf184: ldr      r1, [pc, #0x60]
003cf188: ldr      r0, [r3, #0x5c]
003cf18c: add      r1, pc, r1
003cf190: bl       #0x30e31c
003cf194: cmp      r0, #0
003cf198: beq      #0x3cf1b4
003cf19c: mov      r0, r5
003cf1a0: bl       #0x3cce14
003cf1a4: mov      r3, #0
003cf1a8: str      r3, [r5, #0x30]
003cf1ac: strb     r3, [r5, #0x2c]
003cf1b0: b        #0x3cf108
003cf1b4: mov      r0, r5
003cf1b8: bl       #0x3cd11c
003cf1bc: b        #0x3cf1a4
003cf1c0: bl       #0x310440
003cf1c4: b        #0x3cf180
003cf1c8: bl       #0x310440
003cf1cc: b        #0x3cf0f4
003cf1d0: bl       #0x30e310
003cf1d4: subseq   r5, ip, r4, lsr sl
003cf1d8: andeq    r4, r0, ip, lsr #1
003cf1dc: andeq    r0, r0, r8, asr r7
003cf1e0: andeq    r0, r0, r4, lsl #17
003cf1e4: subeq    r4, pc, r8, asr sp
003cf1e8: subeq    r4, pc, ip, asr #25
003cf1ec: subeq    r1, pc, ip, lsr r2

# _ZN6CharAI9SetScriptI10AISDefaultEEvv
003cce14: push     {r4, r5, r6, lr}
003cce18: ldr      r3, [r0, #4]
003cce1c: ldr      r5, [pc, #0xe8]
003cce20: sub      sp, sp, #8
003cce24: cmp      r3, #0
003cce28: mov      r4, r0
003cce2c: add      r5, pc, r5
003cce30: beq      #0x3cceb8
003cce34: ldr      r3, [r4, #0x20]
003cce38: cmp      r3, #0
003cce3c: beq      #0x3cce74
003cce40: ldr      r3, [r4]
003cce44: mov      r0, r4
003cce48: mov      lr, pc
003cce4c: ldr      pc, [r3, #0x14]
003cce50: ldr      r3, [r4, #0x20]
003cce54: cmp      r3, #0
003cce58: beq      #0x3cce74
003cce5c: mov      r0, r3
003cce60: ldr      r3, [r3]
003cce64: mov      lr, pc
003cce68: ldr      pc, [r3, #4]
003cce6c: mov      r3, #0
003cce70: str      r3, [r4, #0x20]
003cce74: mov      r1, #0
003cce78: mov      r0, #0xc4
003cce7c: bl       #0x310570
003cce80: mov      r1, #1
003cce84: mov      r6, r0
003cce88: bl       #0x3d8fb0
003cce8c: ldr      r3, [pc, #0x7c]
003cce90: mov      r2, #0
003cce94: str      r2, [r6, #0xc0]
003cce98: ldr      r3, [r5, r3]
003cce9c: str      r2, [r6, #0xb8]
003ccea0: str      r2, [r6, #0xbc]
003ccea4: add      r3, r3, #8
003ccea8: str      r3, [r6]
003cceac: str      r6, [r4, #0x20]
003cceb0: add      sp, sp, #8
003cceb4: pop      {r4, r5, r6, pc}
003cceb8: ldr      r2, [pc, #0x54]
003ccebc: ldr      r2, [r5, r2]
003ccec0: ldr      r2, [r2]
003ccec4: cmp      r2, #2
003ccec8: streq    r3, [r3]
003ccecc: beq      #0x3cce34
003cced0: cmp      r2, #1
003cced4: bne      #0x3cce34
003cced8: ldr      r0, [pc, #0x38]
003ccedc: ldr      r1, [pc, #0x38]
003ccee0: ldr      r2, [pc, #0x38]
003ccee4: ldr      r0, [r5, r0]
003ccee8: ldr      r3, [pc, #0x34]
003cceec: movw     ip, #0x2a1
003ccef0: add      r1, pc, r1
003ccef4: add      r2, pc, r2
003ccef8: add      r3, pc, r3
003ccefc: add      r0, r0, #0xa8
003ccf00: str      ip, [sp]
003ccf04: bl       #0x30e004
003ccf08: b        #0x3cce34
003ccf0c: subseq   r7, ip, r4, ror #24
003ccf10: andeq    r2, r0, ip, lsr #21
003ccf14: andeq    r3, r0, r0, asr #19
003ccf18: andeq    r1, r0, r0, asr #19
003ccf1c: subeq    r1, pc, r8, ror #9
003ccf20: subeq    r8, pc, ip, asr r3
003ccf24: subeq    r8, pc, r8, lsr #7

# _ZN6CharAI9SetScriptI10AISMonsterEEvv
003ccbe4: push     {r4, r5, r6, lr}
003ccbe8: ldr      r3, [r0, #4]
003ccbec: ldr      r5, [pc, #0xe8]
003ccbf0: sub      sp, sp, #8
003ccbf4: cmp      r3, #0
003ccbf8: mov      r4, r0
003ccbfc: add      r5, pc, r5
003ccc00: beq      #0x3ccc88
003ccc04: ldr      r3, [r4, #0x20]
003ccc08: cmp      r3, #0
003ccc0c: beq      #0x3ccc44
003ccc10: ldr      r3, [r4]
003ccc14: mov      r0, r4
003ccc18: mov      lr, pc
003ccc1c: ldr      pc, [r3, #0x14]
003ccc20: ldr      r3, [r4, #0x20]
003ccc24: cmp      r3, #0
003ccc28: beq      #0x3ccc44
003ccc2c: mov      r0, r3
003ccc30: ldr      r3, [r3]
003ccc34: mov      lr, pc
003ccc38: ldr      pc, [r3, #4]
003ccc3c: mov      r3, #0
003ccc40: str      r3, [r4, #0x20]
003ccc44: mov      r1, #0
003ccc48: mov      r0, #0xc4
003ccc4c: bl       #0x310570
003ccc50: mov      r1, #1
003ccc54: mov      r6, r0
003ccc58: bl       #0x3d8fb0
003ccc5c: ldr      r3, [pc, #0x7c]
003ccc60: mov      r2, #0
003ccc64: str      r2, [r6, #0xc0]
003ccc68: ldr      r3, [r5, r3]
003ccc6c: str      r2, [r6, #0xb8]
003ccc70: str      r2, [r6, #0xbc]
003ccc74: add      r3, r3, #8
003ccc78: str      r3, [r6]
003ccc7c: str      r6, [r4, #0x20]
003ccc80: add      sp, sp, #8
003ccc84: pop      {r4, r5, r6, pc}
003ccc88: ldr      r2, [pc, #0x54]
003ccc8c: ldr      r2, [r5, r2]
003ccc90: ldr      r2, [r2]
003ccc94: cmp      r2, #2
003ccc98: streq    r3, [r3]
003ccc9c: beq      #0x3ccc04
003ccca0: cmp      r2, #1
003ccca4: bne      #0x3ccc04
003ccca8: ldr      r0, [pc, #0x38]
003cccac: ldr      r1, [pc, #0x38]
003cccb0: ldr      r2, [pc, #0x38]
003cccb4: ldr      r0, [r5, r0]
003cccb8: ldr      r3, [pc, #0x34]
003cccbc: movw     ip, #0x2a1
003cccc0: add      r1, pc, r1
003cccc4: add      r2, pc, r2
003cccc8: add      r3, pc, r3
003ccccc: add      r0, r0, #0xa8
003cccd0: str      ip, [sp]
003cccd4: bl       #0x30e004
003cccd8: b        #0x3ccc04

# _ZN6CharAI9SetScriptI15AISPlayerIPhoneEEvv
003ccfe4: push     {r4, r5, r6, r7, lr}
003ccfe8: ldr      r3, [r0, #4]
003ccfec: ldr      r5, [pc, #0x108]
003ccff0: sub      sp, sp, #0xc
003ccff4: cmp      r3, #0
003ccff8: mov      r4, r0
003ccffc: add      r5, pc, r5
003cd000: beq      #0x3cd0a8
003cd004: ldr      r3, [r4, #0x20]
003cd008: cmp      r3, #0
003cd00c: beq      #0x3cd044
003cd010: ldr      r3, [r4]
003cd014: mov      r0, r4
003cd018: mov      lr, pc
003cd01c: ldr      pc, [r3, #0x14]
003cd020: ldr      r3, [r4, #0x20]
003cd024: cmp      r3, #0
003cd028: beq      #0x3cd044
003cd02c: mov      r0, r3
003cd030: ldr      r3, [r3]
003cd034: mov      lr, pc
003cd038: ldr      pc, [r3, #4]
003cd03c: mov      r3, #0
003cd040: str      r3, [r4, #0x20]
003cd044: mov      r1, #0
003cd048: mov      r0, #0xd8
003cd04c: bl       #0x310570
003cd050: mov      r1, #1
003cd054: mov      r6, r0
003cd058: bl       #0x3d8fb0
003cd05c: ldr      r3, [pc, #0x9c]
003cd060: mov      r7, #0
003cd064: mov      r0, r6
003cd068: ldr      r3, [r5, r3]
003cd06c: str      r7, [r6, #0xb8]
003cd070: str      r7, [r6, #0xbc]
003cd074: add      r3, r3, #8
003cd078: str      r7, [r6, #0xc0]
003cd07c: str      r3, [r0], #0xc4
003cd080: bl       #0x3ccf9c
003cd084: ldr      r3, [pc, #0x78]
003cd088: str      r7, [r6, #0xd4]
003cd08c: str      r7, [r6, #0xd0]
003cd090: ldr      r3, [r5, r3]
003cd094: add      r3, r3, #8
003cd098: str      r3, [r6]
003cd09c: str      r6, [r4, #0x20]
003cd0a0: add      sp, sp, #0xc
003cd0a4: pop      {r4, r5, r6, r7, pc}
003cd0a8: ldr      r2, [pc, #0x58]
003cd0ac: ldr      r2, [r5, r2]
003cd0b0: ldr      r2, [r2]
003cd0b4: cmp      r2, #2
003cd0b8: streq    r3, [r3]
003cd0bc: beq      #0x3cd004
003cd0c0: cmp      r2, #1
003cd0c4: bne      #0x3cd004
003cd0c8: ldr      r0, [pc, #0x3c]
003cd0cc: ldr      r1, [pc, #0x3c]
003cd0d0: ldr      r2, [pc, #0x3c]
003cd0d4: ldr      r0, [r5, r0]
003cd0d8: ldr      r3, [pc, #0x38]
003cd0dc: movw     ip, #0x2a1
003cd0e0: add      r1, pc, r1
003cd0e4: add      r2, pc, r2
003cd0e8: add      r3, pc, r3
003cd0ec: add      r0, r0, #0xa8
003cd0f0: str      ip, [sp]
003cd0f4: bl       #0x30e004
003cd0f8: b        #0x3cd004

# _ZN6CharAI9SetScriptI8AISFaeryEEvv
003cccf8: push     {r4, r5, r6, lr}
003cccfc: ldr      r3, [r0, #4]
003ccd00: ldr      r5, [pc, #0xf0]
003ccd04: sub      sp, sp, #8
003ccd08: cmp      r3, #0
003ccd0c: mov      r4, r0
003ccd10: add      r5, pc, r5
003ccd14: beq      #0x3ccda4
003ccd18: ldr      r3, [r4, #0x20]
003ccd1c: cmp      r3, #0
003ccd20: beq      #0x3ccd58
003ccd24: ldr      r3, [r4]
003ccd28: mov      r0, r4
003ccd2c: mov      lr, pc
003ccd30: ldr      pc, [r3, #0x14]
003ccd34: ldr      r3, [r4, #0x20]
003ccd38: cmp      r3, #0
003ccd3c: beq      #0x3ccd58
003ccd40: mov      r0, r3
003ccd44: ldr      r3, [r3]
003ccd48: mov      lr, pc
003ccd4c: ldr      pc, [r3, #4]
003ccd50: mov      r3, #0
003ccd54: str      r3, [r4, #0x20]
003ccd58: mov      r1, #0
003ccd5c: mov      r0, #0xc8
003ccd60: bl       #0x310570
003ccd64: mov      r1, #1
003ccd68: mov      r6, r0
003ccd6c: bl       #0x3d8fb0
003ccd70: ldr      r2, [pc, #0x84]
003ccd74: mov      r3, #0
003ccd78: mvn      r1, #0
003ccd7c: ldr      r2, [r5, r2]
003ccd80: str      r3, [r6, #0xc0]
003ccd84: str      r1, [r6, #0xc4]
003ccd88: add      r2, r2, #8
003ccd8c: str      r2, [r6]
003ccd90: str      r3, [r6, #0xb8]
003ccd94: str      r3, [r6, #0xbc]
003ccd98: str      r6, [r4, #0x20]
003ccd9c: add      sp, sp, #8
003ccda0: pop      {r4, r5, r6, pc}
003ccda4: ldr      r2, [pc, #0x54]
003ccda8: ldr      r2, [r5, r2]
003ccdac: ldr      r2, [r2]
003ccdb0: cmp      r2, #2
003ccdb4: streq    r3, [r3]
003ccdb8: beq      #0x3ccd18
003ccdbc: cmp      r2, #1
003ccdc0: bne      #0x3ccd18
003ccdc4: ldr      r0, [pc, #0x38]
003ccdc8: ldr      r1, [pc, #0x38]
003ccdcc: ldr      r2, [pc, #0x38]
003ccdd0: ldr      r0, [r5, r0]
003ccdd4: ldr      r3, [pc, #0x34]
003ccdd8: movw     ip, #0x2a1
003ccddc: add      r1, pc, r1
003ccde0: add      r2, pc, r2
003ccde4: add      r3, pc, r3
003ccde8: add      r0, r0, #0xa8
003ccdec: str      ip, [sp]
003ccdf0: bl       #0x30e004
003ccdf4: b        #0x3ccd18
003ccdf8: subseq   r7, ip, r0, lsl #27
003ccdfc: andeq    r4, r0, r4, lsr sb
003cce00: andeq    r3, r0, r0, asr #19
003cce04: andeq    r1, r0, r0, asr #19
003cce08: strdeq   r1, r2, [pc], #-0x5c
003cce0c: subeq    r8, pc, r0, ror r4
003cce10: strheq   r8, [pc], #-0x4c

# _ZN6CharAI9SetScriptI11AISExternalEEvv
003ccaf4: push     {r4, r5, lr}
003ccaf8: ldr      r2, [r0, #4]
003ccafc: ldr      r3, [pc, #0xc8]
003ccb00: sub      sp, sp, #0xc
003ccb04: cmp      r2, #0
003ccb08: mov      r4, r0
003ccb0c: add      r3, pc, r3
003ccb10: beq      #0x3ccb78
003ccb14: ldr      r3, [r4, #0x20]
003ccb18: cmp      r3, #0
003ccb1c: beq      #0x3ccb54
003ccb20: ldr      r3, [r4]
003ccb24: mov      r0, r4
003ccb28: mov      lr, pc
003ccb2c: ldr      pc, [r3, #0x14]
003ccb30: ldr      r3, [r4, #0x20]
003ccb34: cmp      r3, #0
003ccb38: beq      #0x3ccb54
003ccb3c: mov      r0, r3
003ccb40: ldr      r3, [r3]
003ccb44: mov      lr, pc
003ccb48: ldr      pc, [r3, #4]
003ccb4c: mov      r3, #0
003ccb50: str      r3, [r4, #0x20]
003ccb54: mov      r1, #0
003ccb58: mov      r0, #0xc4
003ccb5c: bl       #0x310570
003ccb60: mov      r1, #1
003ccb64: mov      r5, r0
003ccb68: bl       #0x3dd0e4
003ccb6c: str      r5, [r4, #0x20]
003ccb70: add      sp, sp, #0xc
003ccb74: pop      {r4, r5, pc}
003ccb78: ldr      r1, [pc, #0x50]
003ccb7c: ldr      r1, [r3, r1]
003ccb80: ldr      r1, [r1]
003ccb84: cmp      r1, #2
003ccb88: streq    r2, [r2]
003ccb8c: beq      #0x3ccb14
003ccb90: cmp      r1, #1
003ccb94: bne      #0x3ccb14
003ccb98: ldr      r0, [pc, #0x34]
003ccb9c: ldr      r1, [pc, #0x34]
003ccba0: ldr      r2, [pc, #0x34]
003ccba4: ldr      r0, [r3, r0]
003ccba8: ldr      r3, [pc, #0x30]
003ccbac: movw     ip, #0x2a1
003ccbb0: add      r1, pc, r1
003ccbb4: add      r2, pc, r2
003ccbb8: add      r3, pc, r3
003ccbbc: add      r0, r0, #0xa8
003ccbc0: str      ip, [sp]
003ccbc4: bl       #0x30e004
003ccbc8: b        #0x3ccb14
003ccbcc: subseq   r7, ip, r4, lsl #31
003ccbd0: andeq    r3, r0, r0, asr #19
003ccbd4: andeq    r1, r0, r0, asr #19
003ccbd8: subeq    r1, pc, r8, lsr #16
003ccbdc: umaaleq  r8, pc, ip, r6
003ccbe0: subeq    r8, pc, r8, ror #13

# _ZN6CharAI9SetScriptI9AISPlayerEEvv
003cd11c: push     {r4, r5, r6, r7, lr}
003cd120: ldr      r3, [r0, #4]
003cd124: ldr      r5, [pc, #0xf8]
003cd128: sub      sp, sp, #0xc
003cd12c: cmp      r3, #0
003cd130: mov      r4, r0
003cd134: add      r5, pc, r5
003cd138: beq      #0x3cd1d0
003cd13c: ldr      r3, [r4, #0x20]
003cd140: cmp      r3, #0
003cd144: beq      #0x3cd17c
003cd148: ldr      r3, [r4]
003cd14c: mov      r0, r4
003cd150: mov      lr, pc
003cd154: ldr      pc, [r3, #0x14]
003cd158: ldr      r3, [r4, #0x20]
003cd15c: cmp      r3, #0
003cd160: beq      #0x3cd17c
003cd164: mov      r0, r3
003cd168: ldr      r3, [r3]
003cd16c: mov      lr, pc
003cd170: ldr      pc, [r3, #4]
003cd174: mov      r3, #0
003cd178: str      r3, [r4, #0x20]
003cd17c: mov      r1, #0
003cd180: mov      r0, #0xd8
003cd184: bl       #0x310570
003cd188: mov      r1, #1
003cd18c: mov      r6, r0
003cd190: bl       #0x3d8fb0
003cd194: ldr      r3, [pc, #0x8c]
003cd198: mov      r7, #0
003cd19c: mov      r0, r6
003cd1a0: ldr      r3, [r5, r3]
003cd1a4: str      r7, [r6, #0xb8]
003cd1a8: str      r7, [r6, #0xbc]
003cd1ac: add      r3, r3, #8
003cd1b0: str      r7, [r6, #0xc0]
003cd1b4: str      r3, [r0], #0xc4
003cd1b8: bl       #0x3ccf9c
003cd1bc: str      r7, [r6, #0xd4]
003cd1c0: str      r7, [r6, #0xd0]
003cd1c4: str      r6, [r4, #0x20]
003cd1c8: add      sp, sp, #0xc
003cd1cc: pop      {r4, r5, r6, r7, pc}
003cd1d0: ldr      r2, [pc, #0x54]
003cd1d4: ldr      r2, [r5, r2]
003cd1d8: ldr      r2, [r2]
003cd1dc: cmp      r2, #2
003cd1e0: streq    r3, [r3]
003cd1e4: beq      #0x3cd13c
003cd1e8: cmp      r2, #1
003cd1ec: bne      #0x3cd13c
003cd1f0: ldr      r0, [pc, #0x38]
003cd1f4: ldr      r1, [pc, #0x38]
003cd1f8: ldr      r2, [pc, #0x38]
003cd1fc: ldr      r0, [r5, r0]
003cd200: ldr      r3, [pc, #0x34]
003cd204: movw     ip, #0x2a1
003cd208: add      r1, pc, r1
003cd20c: add      r2, pc, r2
003cd210: add      r3, pc, r3
003cd214: add      r0, r0, #0xa8
003cd218: str      ip, [sp]
003cd21c: bl       #0x30e004
003cd220: b        #0x3cd13c
003cd224: subseq   r7, ip, ip, asr sb
003cd228: andeq    r1, r0, r0, lsl fp
003cd22c: andeq    r3, r0, r0, asr #19
003cd230: andeq    r1, r0, r0, asr #19
003cd234: ldrdeq   r1, r2, [pc], #-0x10
003cd238: subeq    r8, pc, r4, asr #32
003cd23c: umaaleq  r8, pc, r0, r0
