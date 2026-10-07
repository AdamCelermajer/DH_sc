# _ZN13TriggerObject8InitPostEv
00399ff8 push     {r4, r5, r6, r7, r8, sl, lr}
00399ffc sub      sp, sp, #0x24
0039a000 mov      r4, r0
0039a004 bl       #0x38bd64
0039a008 ldr      r3, [r4, #0x274]
0039a00c ldr      r5, [pc, #0x33c]
0039a010 cmp      r0, r3
0039a014 add      r5, pc, r5
0039a018 bge      #0x39a2f8
0039a01c ldr      r3, [pc, #0x330]
0039a020 ldr      r8, [r4, #0x72c]
0039a024 ldr      r3, [r5, r3]
0039a028 ldr      r7, [r3]
0039a02c cmp      r7, #0
0039a030 beq      #0x39a30c
0039a034 ldr      r3, [pc, #0x31c]
0039a038 mov      r6, #0
0039a03c ldr      r3, [r5, r3]
0039a040 ldr      sl, [r3]
0039a044 b        #0x39a054
0039a048 add      r6, r6, #1
0039a04c cmp      r6, r7
0039a050 beq      #0x39a30c
0039a054 ldr      r1, [sl, r6, lsl #2]
0039a058 mov      r0, r8
0039a05c bl       #0x30e31c
0039a060 cmp      r0, #0
0039a064 bne      #0x39a048
0039a068 cmn      r6, #1
0039a06c str      r6, [r4, #0x730]
0039a070 beq      #0x39a0c4
0039a074 ldr      r7, [pc, #0x2e0]
0039a078 mov      r2, #0x18
0039a07c ldr      r3, [r5, r7]
0039a080 ldr      r3, [r3]
0039a084 mla      r6, r2, r6, r3
0039a088 ldr      r3, [r6, #0x14]
0039a08c cmn      r3, #1
0039a090 beq      #0x39a0c4
0039a094 ldr      r2, [pc, #0x2c4]
0039a098 mov      r1, #0xc
0039a09c ldr      r2, [r5, r2]
0039a0a0 ldr      r2, [r2]
0039a0a4 mla      r3, r1, r3, r2
0039a0a8 ldr      r6, [r3, #8]
0039a0ac mov      r0, r6
0039a0b0 bl       #0x30de54
0039a0b4 mov      r1, r6
0039a0b8 add      r2, r6, r0
0039a0bc add      r0, r4, #0x290
0039a0c0 bl       #0x3109e0
0039a0c4 ldr      r3, [pc, #0x298]
0039a0c8 ldr      r1, [r4, #0x748]
0039a0cc mov      r2, #0
0039a0d0 ldr      r6, [r5, r3]
0039a0d4 mov      r0, r6
0039a0d8 bl       #0x4591f0
0039a0dc ldr      r1, [r4, #0x764]
0039a0e0 str      r0, [r4, #0x74c]
0039a0e4 mov      r2, #0
0039a0e8 mov      r0, r6
0039a0ec bl       #0x4591f0
0039a0f0 str      r0, [r4, #0x768]
0039a0f4 mov      r0, r4
0039a0f8 bl       #0x398874
0039a0fc ldr      r7, [r4, #0x780]
0039a100 ldr      r3, [r4, #0x77c]
0039a104 cmp      r3, r7
0039a108 beq      #0x39a300
0039a10c ldr      r1, [pc, #0x254]
0039a110 mov      r0, r7
0039a114 add      r1, pc, r1
0039a118 bl       #0x30e31c
0039a11c cmp      r0, #0
0039a120 beq      #0x39a300
0039a124 ldr      r3, [pc, #0x240]
0039a128 ldr      r3, [r5, r3]
0039a12c ldr      r8, [r3]
0039a130 cmp      r8, #0
0039a134 beq      #0x39a1ac
0039a138 ldr      r3, [pc, #0x230]
0039a13c mov      r6, #0
0039a140 ldr      r3, [r5, r3]
0039a144 ldr      sl, [r3]
0039a148 b        #0x39a158
0039a14c add      r6, r6, #1
0039a150 cmp      r6, r8
0039a154 beq      #0x39a1ac
0039a158 ldr      r1, [sl, r6, lsl #2]
0039a15c mov      r0, r7
0039a160 bl       #0x30e31c
0039a164 cmp      r0, #0
0039a168 bne      #0x39a14c
0039a16c cmn      r6, #1
0039a170 beq      #0x39a1ac
0039a174 mov      r1, r0
0039a178 mov      r0, #0xc
0039a17c bl       #0x310570
0039a180 mov      r7, r0
0039a184 bl       #0x4786f0
0039a188 ldr      r3, [pc, #0x1e4]
0039a18c str      r7, [r4, #0x788]
0039a190 mov      r0, r7
0039a194 ldr      r3, [r5, r3]
0039a198 ldr      r3, [r3]
0039a19c add      r6, r3, r6, lsl #4
0039a1a0 ldr      r2, [r6, #4]
0039a1a4 ldr      r1, [r6, #8]
0039a1a8 bl       #0x478914
0039a1ac ldr      r3, [r4, #0x730]
0039a1b0 cmn      r3, #1
0039a1b4 beq      #0x39a2e4
0039a1b8 mov      r0, r4
0039a1bc bl       #0x38ab60
0039a1c0 cmp      r0, #0
0039a1c4 beq      #0x39a2e4
0039a1c8 ldr      r3, [r4, #0x2d8]
0039a1cc cmp      r3, #0
0039a1d0 beq      #0x39a208
0039a1d4 ldrb     r2, [r4, #0x784]
0039a1d8 cmp      r2, #0
0039a1dc bne      #0x39a318
0039a1e0 ldr      ip, [r3, #0x38]
0039a1e4 ldr      r1, [pc, #0x18c]
0039a1e8 mov      r3, r2
0039a1ec mov      r0, ip
0039a1f0 add      r1, pc, r1
0039a1f4 ldr      ip, [ip]
0039a1f8 str      r2, [sp]
0039a1fc mov      r2, #1
0039a200 mov      lr, pc
0039a204 ldr      pc, [ip, #0x20]
0039a208 ldr      r3, [pc, #0x16c]
0039a20c mov      r1, #0
0039a210 mov      r0, #0x28
0039a214 ldr      r3, [r5, r3]
0039a218 mov      r6, r1
0039a21c ldr      r8, [r3, #0x44]
0039a220 bl       #0x310570
0039a224 mov      ip, #1
0039a228 mov      lr, #2
0039a22c mov      r3, ip
0039a230 mov      r1, r8
0039a234 mov      r2, r4
0039a238 str      lr, [sp, #0x10]
0039a23c movw     lr, #0xffff
0039a240 mov      r7, r0
0039a244 str      lr, [sp, #0x14]
0039a248 str      ip, [sp, #0x18]
0039a24c str      r6, [sp]
0039a250 str      r6, [sp, #4]
0039a254 str      r6, [sp, #8]
0039a258 str      r6, [sp, #0xc]
0039a25c bl       #0x46f2f0
0039a260 ldr      r3, [pc, #0x118]
0039a264 mov      r0, r4
0039a268 mov      r1, r7
0039a26c ldr      r3, [r5, r3]
0039a270 mov      r2, r6
0039a274 add      r3, r3, #8
0039a278 str      r3, [r7]
0039a27c bl       #0x394bf8
0039a280 ldr      r3, [pc, #0xfc]
0039a284 ldr      r3, [r5, r3]
0039a288 ldr      r0, [r3]
0039a28c cmp      r0, r6
0039a290 beq      #0x39a348
0039a294 ldr      r7, [pc, #0xc0]
0039a298 ldr      r3, [r4, #0x730]
0039a29c mov      r1, #0x18
0039a2a0 ldr      r2, [r5, r7]
0039a2a4 ldr      r2, [r2]
0039a2a8 mla      r3, r1, r3, r2
0039a2ac ldr      r1, [r3, #0x10]
0039a2b0 bl       #0x3699fc
0039a2b4 ldr      r2, [r5, r7]
0039a2b8 ldr      r3, [r4, #0x730]
0039a2bc mov      r1, #0x18
0039a2c0 ldr      r2, [r2]
0039a2c4 mov      r0, r4
0039a2c8 mla      r3, r1, r3, r2
0039a2cc ldr      r2, [pc, #0xb4]
0039a2d0 ldr      r1, [r3, #0xc]
0039a2d4 add      r2, pc, r2
0039a2d8 add      sp, sp, #0x24
0039a2dc pop      {r4, r5, r6, r7, r8, sl, lr}
0039a2e0 b        #0x38ef60
0039a2e4 mov      r0, r4
0039a2e8 ldr      r3, [r4]
0039a2ec mov      r1, #0
0039a2f0 mov      lr, pc
0039a2f4 ldr      pc, [r3, #0x40]
0039a2f8 add      sp, sp, #0x24
0039a2fc pop      {r4, r5, r6, r7, r8, sl, pc}
0039a300 mov      r3, #1
0039a304 strb     r3, [r4, #0x784]
0039a308 b        #0x39a1ac
0039a30c mvn      r3, #0
0039a310 str      r3, [r4, #0x730]
0039a314 b        #0x39a0c4
0039a318 ldr      ip, [r3, #0x38]
0039a31c ldr      r1, [pc, #0x68]
0039a320 mov      r2, #0
0039a324 mov      r3, r2
0039a328 mov      r0, ip
0039a32c add      r1, pc, r1
0039a330 ldr      ip, [ip]
0039a334 str      r2, [sp]
0039a338 mov      r2, #1
0039a33c mov      lr, pc
0039a340 ldr      pc, [ip, #0x20]
0039a344 b        #0x39a208
0039a348 ldr      r7, [pc, #0xc]
0039a34c b        #0x39a2b4
0039a350 subseq   sl, pc, ip, ror sl
0039a354 andeq    r0, r0, r8, lsr #25
0039a358 andeq    r4, r0, r8, asr r2
0039a35c andeq    r0, r0, ip, lsl lr
0039a360 andeq    r1, r0, r8, lsr #25
0039a364 andeq    r1, r0, r0, lsr #20
0039a368 subseq   r0, r4, ip, lsl #11
0039a36c andeq    r2, r0, r4, ror #30
0039a370 andeq    r3, r0, r4, asr r5
0039a374 andeq    r2, r0, r0, asr #16
0039a378 subseq   r8, r2, r8, asr #17
0039a37c strdeq   r3, r4, [r0], -r4
0039a380 andeq    r2, r0, r8, lsl r4
0039a384 andeq    r0, r0, r4, lsr #27
0039a388 subseq   r8, r2, ip, lsr #17
0039a38c subseq   r7, r2, r4, lsl #31
