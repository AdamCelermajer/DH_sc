
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
