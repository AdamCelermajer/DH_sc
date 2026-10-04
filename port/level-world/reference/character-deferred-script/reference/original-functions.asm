
# _ZN6CharAI11OnInitFinalEv
003d0ba4: push     {r4, lr}
003d0ba8: ldr      r3, [r0, #0x1c]
003d0bac: cmp      r3, #0
003d0bb0: beq      #0x3d0bc4
003d0bb4: mov      r0, r3
003d0bb8: ldr      r3, [r3]
003d0bbc: mov      lr, pc
003d0bc0: ldr      pc, [r3, #0x10]
003d0bc4: pop      {r4, pc}

# _ZN6CharAI17LoadScriptProcessEv
003cf1f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003cf1f4: ldr      r3, [r0, #0x28]
003cf1f8: ldr      r6, [pc, #0x18c]
003cf1fc: sub      sp, sp, #0xc
003cf200: cmp      r3, #6
003cf204: mov      r4, r0
003cf208: add      r6, pc, r6
003cf20c: bgt      #0x3cf2a8
003cf210: ldrb     r2, [r0, #0x24]
003cf214: cmp      r2, #0
003cf218: bne      #0x3cf348
003cf21c: mov      r5, #6
003cf220: ldr      r8, [pc, #0x168]
003cf224: ldr      sl, [pc, #0x168]
003cf228: ldr      sb, [pc, #0x168]
003cf22c: ldr      r7, [pc, #0x168]
003cf230: ldr      fp, [pc, #0x168]
003cf234: add      r8, pc, r8
003cf238: add      sl, pc, sl
003cf23c: add      sb, pc, sb
003cf240: cmp      r3, #6
003cf244: addls    pc, pc, r3, lsl #2
003cf248: b        #0x3cf300
003cf24c: b        #0x3cf2f0
003cf250: b        #0x3cf2e0
003cf254: b        #0x3cf2d0
003cf258: b        #0x3cf2c0
003cf25c: b        #0x3cf2b0
003cf260: b        #0x3cf28c
003cf264: b        #0x3cf268
003cf268: ldr      r2, [r4, #0x20]
003cf26c: ldr      r3, [r4, #0x28]
003cf270: str      r2, [r4, #0x1c]
003cf274: add      r3, r3, #1
003cf278: cmp      r5, #0
003cf27c: str      r3, [r4, #0x28]
003cf280: beq      #0x3cf2a8
003cf284: sub      r5, r5, #1
003cf288: b        #0x3cf240
003cf28c: mov      r0, r4
003cf290: bl       #0x3cb314
003cf294: ldr      r3, [r4, #0x28]
003cf298: cmp      r5, #0
003cf29c: add      r3, r3, #1
003cf2a0: str      r3, [r4, #0x28]
003cf2a4: bne      #0x3cf284
003cf2a8: add      sp, sp, #0xc
003cf2ac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003cf2b0: mov      r0, r4
003cf2b4: bl       #0x3cdf7c
003cf2b8: ldr      r3, [r4, #0x28]
003cf2bc: b        #0x3cf274
003cf2c0: mov      r0, r4
003cf2c4: bl       #0x3cc218
003cf2c8: ldr      r3, [r4, #0x28]
003cf2cc: b        #0x3cf274
003cf2d0: mov      r0, r4
003cf2d4: bl       #0x3cc26c
003cf2d8: ldr      r3, [r4, #0x28]
003cf2dc: b        #0x3cf274
003cf2e0: mov      r0, r4
003cf2e4: bl       #0x3cc278
003cf2e8: ldr      r3, [r4, #0x28]
003cf2ec: b        #0x3cf274
003cf2f0: mov      r0, r4
003cf2f4: bl       #0x3cf04c
003cf2f8: ldr      r3, [r4, #0x28]
003cf2fc: b        #0x3cf274
003cf300: ldr      r2, [r6, r7]
003cf304: ldr      r2, [r2]
003cf308: cmp      r2, #2
003cf30c: moveq    r2, #0
003cf310: streq    r2, [r2]
003cf314: beq      #0x3cf274
003cf318: cmp      r2, #1
003cf31c: bne      #0x3cf274
003cf320: ldr      r0, [r6, fp]
003cf324: mov      r3, sb
003cf328: movw     ip, #0x22b
003cf32c: mov      r1, r8
003cf330: mov      r2, sl
003cf334: add      r0, r0, #0xa8
003cf338: str      ip, [sp]
003cf33c: bl       #0x30e004
003cf340: ldr      r3, [r4, #0x28]
003cf344: b        #0x3cf274
003cf348: ldr      r3, [r0, #4]
003cf34c: mov      r0, r3
003cf350: ldr      r3, [r3]
003cf354: mov      lr, pc
003cf358: ldr      pc, [r3, #0x28]
003cf35c: cmp      r0, #0
003cf360: ldrne    r3, [r4, #0x28]
003cf364: bne      #0x3cf21c
003cf368: ldr      r1, [r4, #0x28]
003cf36c: mov      r0, r4
003cf370: rsb      r1, r1, #7
003cf374: bl       #0x3cb854
003cf378: cmp      r0, #0
003cf37c: ble      #0x3cf2a8
003cf380: sub      r5, r0, #1
003cf384: ldr      r3, [r4, #0x28]
003cf388: b        #0x3cf220
003cf38c: subseq   r5, ip, r8, lsl #17
003cf390: subeq    pc, lr, r4, lsr #3
003cf394: subeq    pc, lr, r0, lsr r3
003cf398: subeq    r5, pc, ip, ror pc
003cf39c: andeq    r3, r0, r0, asr #19
003cf3a0: andeq    r1, r0, r0, asr #19

# _ZN6CharAI10OnInitPostEv
003d0b80: push     {r4, lr}
003d0b84: ldr      r3, [r0, #0x1c]
003d0b88: cmp      r3, #0
003d0b8c: beq      #0x3d0ba0
003d0b90: mov      r0, r3
003d0b94: ldr      r3, [r3]
003d0b98: mov      lr, pc
003d0b9c: ldr      pc, [r3, #0xc]
003d0ba0: pop      {r4, pc}

# _ZN6CharAI17InitScriptProcessEb
003ce7c0: push     {r4, r5, r6, lr}
003ce7c4: mov      r4, r0
003ce7c8: ldr      r0, [r0, #4]
003ce7cc: mov      r5, r1
003ce7d0: bl       #0x3b3a70
003ce7d4: mov      r0, r4
003ce7d8: bl       #0x3ce044
003ce7dc: mov      r0, r4
003ce7e0: bl       #0x3d8894
003ce7e4: ldr      r3, [r4]
003ce7e8: mov      r0, r4
003ce7ec: mov      lr, pc
003ce7f0: ldr      pc, [r3, #0xc]
003ce7f4: cmp      r5, #0
003ce7f8: beq      #0x3ce80c
003ce7fc: mov      r0, r4
003ce800: ldr      r3, [r4]
003ce804: mov      lr, pc
003ce808: ldr      pc, [r3, #0x10]
003ce80c: pop      {r4, r5, r6, pc}

# _ZN6CharAI14StepInitScriptEv
003cb314: push     {r4, lr}
003cb318: mov      r4, r0
003cb31c: ldr      r3, [r0]
003cb320: mov      lr, pc
003cb324: ldr      pc, [r3, #8]
003cb328: ldr      r3, [r4, #0x30]
003cb32c: cmp      r3, #0
003cb330: beq      #0x3cb348
003cb334: ldr      r3, [r4, #0x20]
003cb338: mov      r0, r3
003cb33c: ldr      r3, [r3]
003cb340: mov      lr, pc
003cb344: ldr      pc, [r3, #0xcc]
003cb348: pop      {r4, pc}

# _ZN6CharAI22LoadNInitScriptProcessEb
003cf3a4: push     {r4, lr}
003cf3a8: ldr      r3, [r0, #0x1c]
003cf3ac: sub      sp, sp, #8
003cf3b0: mov      r4, r0
003cf3b4: cmp      r3, #0
003cf3b8: beq      #0x3cf3c8
003cf3bc: mov      r0, #0
003cf3c0: add      sp, sp, #8
003cf3c4: pop      {r4, pc}
003cf3c8: str      r1, [sp, #4]
003cf3cc: bl       #0x3cf1f0
003cf3d0: ldr      r3, [r4, #0x1c]
003cf3d4: ldr      r1, [sp, #4]
003cf3d8: cmp      r3, #0
003cf3dc: beq      #0x3cf3bc
003cf3e0: mov      r0, r4
003cf3e4: bl       #0x3ce7c0
003cf3e8: mov      r0, #1
003cf3ec: b        #0x3cf3c0
