# _ZN15VoxSoundManager12RestartMusicEi
0036c224 push     {r4, r5, r6, r7, r8, sl, lr}
0036c228 ldr      r4, [pc, #0xa4]
0036c22c ldr      r5, [pc, #0xa4]
0036c230 ldr      r2, [pc, #0xa4]
0036c234 add      r4, pc, r4
0036c238 ldr      r3, [r4, r5]
0036c23c ldr      r8, [r4, r2]
0036c240 sub      sp, sp, #0x2c
0036c244 ldr      r3, [r3]
0036c248 mov      sl, r0
0036c24c mov      r0, r8
0036c250 str      r3, [sp, #0x24]
0036c254 mov      r6, r1
0036c258 bl       #0x337888
0036c25c ldr      r1, [pc, #0x7c]
0036c260 add      r7, sp, #0xc
0036c264 add      r2, sp, #8
0036c268 add      r1, pc, r1
0036c26c mov      r0, r7
0036c270 bl       #0x3140ec
0036c274 mov      r0, r8
0036c278 mov      r1, r7
0036c27c bl       #0x337a88
0036c280 mov      r8, r0
0036c284 mov      r0, r7
0036c288 bl       #0x3139ac
0036c28c cmp      r8, #0
0036c290 bne      #0x36c2b4
0036c294 ldr      r1, [sl, #0x28]
0036c298 cmn      r1, #1
0036c29c beq      #0x36c2b4
0036c2a0 ldrb     r2, [sl, #0x30]
0036c2a4 mov      r0, sl
0036c2a8 mov      r3, #1
0036c2ac str      r6, [sp]
0036c2b0 bl       #0x36bd78
0036c2b4 ldr      r3, [r4, r5]
0036c2b8 ldr      r2, [sp, #0x24]
0036c2bc ldr      r3, [r3]
0036c2c0 cmp      r2, r3
0036c2c4 bne      #0x36c2d0
0036c2c8 add      sp, sp, #0x2c
0036c2cc pop      {r4, r5, r6, r7, r8, sl, pc}
0036c2d0 bl       #0x30e310
0036c2d4 rsbeq    r8, r2, ip, asr r8
0036c2d8 andeq    r4, r0, ip, lsr #1
0036c2dc andeq    r0, r0, r4, lsl #17
0036c2e0 subseq   r4, r5, r0, lsl #29
