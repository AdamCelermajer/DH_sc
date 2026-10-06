# _ZN15VoxSoundManager11ResumeMusicEi
0036c164 push     {r4, r5, r6, r7, r8, sl, lr}
0036c168 ldr      r4, [pc, #0xa4]
0036c16c ldr      r5, [pc, #0xa4]
0036c170 ldr      r2, [pc, #0xa4]
0036c174 add      r4, pc, r4
0036c178 ldr      r3, [r4, r5]
0036c17c ldr      r8, [r4, r2]
0036c180 sub      sp, sp, #0x2c
0036c184 ldr      r3, [r3]
0036c188 mov      sl, r0
0036c18c mov      r0, r8
0036c190 str      r3, [sp, #0x24]
0036c194 mov      r6, r1
0036c198 bl       #0x337888
0036c19c ldr      r1, [pc, #0x7c]
0036c1a0 add      r7, sp, #0xc
0036c1a4 add      r2, sp, #8
0036c1a8 add      r1, pc, r1
0036c1ac mov      r0, r7
0036c1b0 bl       #0x3140ec
0036c1b4 mov      r0, r8
0036c1b8 mov      r1, r7
0036c1bc bl       #0x337a88
0036c1c0 mov      r8, r0
0036c1c4 mov      r0, r7
0036c1c8 bl       #0x3139ac
0036c1cc cmp      r8, #0
0036c1d0 bne      #0x36c1f4
0036c1d4 ldr      r1, [sl, #0x24]
0036c1d8 cmn      r1, #1
0036c1dc beq      #0x36c1f4
0036c1e0 ldrb     r2, [sl, #0x30]
0036c1e4 mov      r0, sl
0036c1e8 mov      r3, #1
0036c1ec str      r6, [sp]
0036c1f0 bl       #0x36bd78
0036c1f4 ldr      r3, [r4, r5]
0036c1f8 ldr      r2, [sp, #0x24]
0036c1fc ldr      r3, [r3]
0036c200 cmp      r2, r3
0036c204 bne      #0x36c210
0036c208 add      sp, sp, #0x2c
0036c20c pop      {r4, r5, r6, r7, r8, sl, pc}
0036c210 bl       #0x30e310
0036c214 rsbeq    r8, r2, ip, lsl sb
0036c218 andeq    r4, r0, ip, lsr #1
0036c21c andeq    r0, r0, r4, lsl #17
0036c220 subseq   r4, r5, r0, asr #30
