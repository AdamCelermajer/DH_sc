
# _ZN12CharAIScript12SetCharacterEP9Character
003d90f8: push     {r4, r5, lr}
003d90fc: ldr      r3, [pc, #0x98]
003d9100: subs     r4, r1, #0
003d9104: sub      sp, sp, #0xc
003d9108: mov      r5, r0
003d910c: add      r3, pc, r3
003d9110: beq      #0x3d9148
003d9114: str      r4, [r5, #0x98]
003d9118: mov      r0, r4
003d911c: add      r1, r5, #0x10
003d9120: ldr      r3, [r4]
003d9124: mov      lr, pc
003d9128: ldr      pc, [r3, #0xc]
003d912c: ldr      r1, [pc, #0x6c]
003d9130: add      r0, r5, #0x68
003d9134: add      r1, pc, r1
003d9138: add      r2, r1, #0x10
003d913c: add      sp, sp, #0xc
003d9140: pop      {r4, r5, lr}
003d9144: b        #0x3109e0
003d9148: ldr      r2, [pc, #0x54]
003d914c: ldr      r2, [r3, r2]
003d9150: ldr      r2, [r2]
003d9154: cmp      r2, #2
003d9158: streq    r4, [r4]
003d915c: beq      #0x3d9114
003d9160: cmp      r2, #1
003d9164: bne      #0x3d9114
003d9168: ldr      r0, [pc, #0x38]
003d916c: ldr      r1, [pc, #0x38]
003d9170: ldr      r2, [pc, #0x38]
003d9174: ldr      r0, [r3, r0]
003d9178: ldr      r3, [pc, #0x34]
003d917c: mov      ip, #0x83
003d9180: add      r1, pc, r1
003d9184: add      r2, pc, r2
003d9188: add      r3, pc, r3
003d918c: add      r0, r0, #0xa8
003d9190: str      ip, [sp]
003d9194: bl       #0x30e004
003d9198: b        #0x3d9114
003d919c: subseq   fp, fp, r4, lsl #19
003d91a0: subeq    sl, lr, ip, lsl r5
003d91a4: andeq    r3, r0, r0, asr #19
003d91a8: andeq    r1, r0, r0, asr #19
003d91ac: subeq    r5, lr, r8, asr r2
003d91b0: subseq   r8, r1, r4, lsl #30
003d91b4: subeq    ip, lr, r0, ror r6

# _ZN6CharAI16StepSetCharacterEv
003cc26c: ldr      r1, [r0, #4]
003cc270: ldr      r0, [r0, #0x20]
003cc274: b        #0x3d90f8

# _ZN6CharAI14StepLoadCommonEv
003cc218: ldrb     r3, [r0, #0x2c]
003cc21c: cmp      r3, #0
003cc220: bxeq     lr
003cc224: ldr      r1, [pc, #8]
003cc228: ldr      r0, [r0, #0x20]
003cc22c: add      r1, pc, r1
003cc230: b        #0x37b574
003cc234: subeq    r8, pc, ip, ror #31

# _ZN6CharAI16StepBindFunctionEv
003cc278: ldr      r0, [r0, #0x20]
003cc27c: b        #0x3d8f2c

# _ZN6CharAI22StepQueryAvailableStepEi
003cb854: push     {r4, r5, lr}
003cb858: ldr      r4, [pc, #0x8c]
003cb85c: subs     r5, r1, #0
003cb860: sub      sp, sp, #0xc
003cb864: add      r4, pc, r4
003cb868: blt      #0x3cb894
003cb86c: ldr      r3, [pc, #0x7c]
003cb870: ldr      r3, [r4, r3]
003cb874: ldr      r2, [r3]
003cb878: cmp      r5, r2
003cb87c: movlt    r0, r5
003cb880: movge    r0, r2
003cb884: rsb      r2, r0, r2
003cb888: str      r2, [r3]
003cb88c: add      sp, sp, #0xc
003cb890: pop      {r4, r5, pc}
003cb894: ldr      r3, [pc, #0x58]
003cb898: ldr      r3, [r4, r3]
003cb89c: ldr      r3, [r3]
003cb8a0: cmp      r3, #2
003cb8a4: moveq    r3, #0
003cb8a8: streq    r3, [r3]
003cb8ac: beq      #0x3cb86c
003cb8b0: cmp      r3, #1
003cb8b4: bne      #0x3cb86c
003cb8b8: ldr      r0, [pc, #0x38]
003cb8bc: ldr      r1, [pc, #0x38]
003cb8c0: ldr      r2, [pc, #0x38]
003cb8c4: ldr      r0, [r4, r0]
003cb8c8: ldr      r3, [pc, #0x34]
003cb8cc: movw     ip, #0x161
003cb8d0: add      r1, pc, r1
003cb8d4: add      r2, pc, r2
003cb8d8: add      r3, pc, r3
003cb8dc: add      r0, r0, #0xa8
003cb8e0: str      ip, [sp]
003cb8e4: bl       #0x30e004
003cb8e8: b        #0x3cb86c
003cb8ec: subseq   sb, ip, ip, lsr #4
003cb8f0: andeq    r4, r0, ip, lsr #20
003cb8f4: andeq    r3, r0, r0, asr #19
003cb8f8: andeq    r1, r0, r0, asr #19
003cb8fc: subeq    r2, pc, r8, lsl #22
003cb900: subeq    sb, pc, r4, lsr sb
003cb904: subeq    sb, pc, r0, ror #17
