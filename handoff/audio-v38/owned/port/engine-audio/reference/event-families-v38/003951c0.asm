# _ZN12SoundEmitter6UpdateEv
003951c0 push     {r4, r5, r6, r7, r8, sl, lr}
003951c4 ldr      r4, [pc, #0x200]
003951c8 ldr      r3, [pc, #0x200]
003951cc sub      sp, sp, #0x2c
003951d0 add      r4, pc, r4
003951d4 ldr      r6, [r4, r3]
003951d8 mov      r5, r0
003951dc mov      r0, r6
003951e0 bl       #0x31f594
003951e4 cmp      r0, #0
003951e8 beq      #0x39521c
003951ec ldr      r0, [r6, #0x40]
003951f0 mov      r1, #0
003951f4 mov      r2, #1
003951f8 bl       #0x36e478
003951fc ldr      r3, [r0, #0x660]
00395200 cmp      r3, #0
00395204 beq      #0x39521c
00395208 mov      r0, r6
0039520c bl       #0x31f594
00395210 ldr      r3, [r0, #0x130]
00395214 cmp      r3, #0x26
00395218 beq      #0x395224
0039521c add      sp, sp, #0x2c
00395220 pop      {r4, r5, r6, r7, r8, sl, pc}
00395224 mov      r2, #1
00395228 ldr      r0, [r6, #0x40]
0039522c mov      r1, #0
00395230 bl       #0x36e478
00395234 ldr      r3, [r0, #0x660]
00395238 ldr      r0, [r5, #0x160]
0039523c ldr      r1, [r3, #0x160]
00395240 str      r1, [sp, #0x1c]
00395244 ldr      r7, [r3, #0x164]
00395248 str      r7, [sp, #0x20]
0039524c ldr      r6, [r3, #0x168]
00395250 str      r6, [sp, #0x24]
00395254 bl       #0x30e3ac
00395258 mov      r1, r7
0039525c mov      sl, r0
00395260 ldr      r0, [r5, #0x164]
00395264 bl       #0x30e3ac
00395268 mov      r1, r6
0039526c mov      r8, r0
00395270 ldr      r0, [r5, #0x168]
00395274 bl       #0x30e3ac
00395278 mov      r1, sl
0039527c mov      r7, r0
00395280 mov      r0, sl
00395284 bl       #0x30ed6c
00395288 mov      r1, r8
0039528c mov      r6, r0
00395290 mov      r0, r8
00395294 bl       #0x30ed6c
00395298 mov      r1, r0
0039529c mov      r0, r6
003952a0 bl       #0x30eba4
003952a4 mov      r1, r7
003952a8 mov      r6, r0
003952ac mov      r0, r7
003952b0 bl       #0x30ed6c
003952b4 mov      r1, r0
003952b8 mov      r0, r6
003952bc bl       #0x30eba4
003952c0 bl       #0x30e8a4
003952c4 bl       #0x30e1c0
003952c8 bl       #0x30e6a0
003952cc ldrb     r3, [r5, #0x39c]
003952d0 mov      r7, r0
003952d4 cmp      r3, #0
003952d8 beq      #0x395344
003952dc ldr      r6, [r5, #0x398]
003952e0 mov      r1, r0
003952e4 mov      r0, r6
003952e8 bl       #0x30e9ac
003952ec cmp      r0, #0
003952f0 beq      #0x39521c
003952f4 mov      r0, r6
003952f8 mov      r1, #0
003952fc bl       #0x30e2f8
00395300 cmp      r0, #0
00395304 beq      #0x39521c
00395308 mov      r3, #0
0039530c strb     r3, [r5, #0x39c]
00395310 ldr      r3, [pc, #0xbc]
00395314 ldr      r3, [r4, r3]
00395318 ldr      r0, [r3]
0039531c cmp      r0, #0
00395320 beq      #0x395348
00395324 ldr      r1, [r5, #0x390]
00395328 add      r3, sp, #0x1c
0039532c mov      r2, #0xfa
00395330 str      r6, [sp]
00395334 bl       #0x36a218
00395338 ldrb     r3, [r5, #0x39c]
0039533c cmp      r3, #0
00395340 bne      #0x39521c
00395344 ldr      r6, [r5, #0x398]
00395348 mov      r1, r7
0039534c mov      r0, r6
00395350 bl       #0x30e2f8
00395354 cmp      r0, #0
00395358 beq      #0x39521c
0039535c mov      r0, r6
00395360 mov      r1, #0
00395364 bl       #0x30e2f8
00395368 cmp      r0, #0
0039536c beq      #0x39521c
00395370 ldr      r3, [pc, #0x5c]
00395374 mov      ip, #1
00395378 strb     ip, [r5, #0x39c]
0039537c ldr      r3, [r4, r3]
00395380 ldr      r0, [r3]
00395384 cmp      r0, #0
00395388 beq      #0x39521c
0039538c ldrb     r3, [r5, #0x374]
00395390 ldr      r1, [r5, #0x390]
00395394 ldr      r6, [r5, #0x164]
00395398 ldr      r4, [r5, #0x168]
0039539c ldr      r5, [r5, #0x160]
003953a0 mov      lr, #0xbf000000
003953a4 add      lr, lr, #0x800000
003953a8 add      r2, sp, #0x10
003953ac str      r5, [sp, #0x10]
003953b0 str      r6, [sp, #0x14]
003953b4 str      r4, [sp, #0x18]
003953b8 str      ip, [sp]
003953bc str      lr, [sp, #8]
003953c0 str      lr, [sp, #4]
003953c4 bl       #0x36b5d8
003953c8 b        #0x39521c
003953cc subseq   pc, pc, r0, asr #17
003953d0 strdeq   r3, r4, [r0], -r4
003953d4 andeq    r0, r0, r4, lsr #27
