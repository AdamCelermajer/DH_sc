# _ZN6CharAI15OnTargetInSightEv
003d22e4 push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d22e8 ldr      r4, [pc, #0x108]
003d22ec ldr      r6, [pc, #0x108]
003d22f0 ldr      r2, [pc, #0x108]
003d22f4 add      r4, pc, r4
003d22f8 ldr      r3, [r4, r6]
003d22fc ldr      r8, [r4, r2]
003d2300 sub      sp, sp, #0x40
003d2304 ldr      r3, [r3]
003d2308 mov      r5, r0
003d230c mov      r0, r8
003d2310 str      r3, [sp, #0x3c]
003d2314 bl       #0x337888
003d2318 ldr      r1, [pc, #0xe4]
003d231c add      r7, sp, #0x24
003d2320 add      r2, sp, #0x20
003d2324 mov      r0, r7
003d2328 add      r1, pc, r1
003d232c bl       #0x3140ec
003d2330 mov      r1, r7
003d2334 mov      r0, r8
003d2338 bl       #0x337a88
003d233c mov      r0, r7
003d2340 bl       #0x3139ac
003d2344 ldr      r3, [pc, #0xbc]
003d2348 ldr      r0, [r5, #4]
003d234c ldr      r3, [r4, r3]
003d2350 ldr      r7, [r3]
003d2354 bl       #0x3a2fec
003d2358 mov      r3, #0x44
003d235c mla      r7, r3, r0, r7
003d2360 ldr      r3, [pc, #0xa4]
003d2364 ldr      r0, [r5, #4]
003d2368 ldr      sl, [r7, #0x24]
003d236c ldr      r3, [r4, r3]
003d2370 ldr      sb, [r3]
003d2374 bl       #0x3935dc
003d2378 ldr      lr, [r0, #4]
003d237c ldr      r7, [r0]
003d2380 ldr      r8, [r0, #8]
003d2384 mov      ip, #0xbf000000
003d2388 add      ip, ip, #0x800000
003d238c mov      r3, #0
003d2390 str      lr, [sp, #0x18]
003d2394 mov      r0, sb
003d2398 mov      lr, #1
003d239c mov      r1, sl
003d23a0 add      r2, sp, #0x14
003d23a4 str      r7, [sp, #0x14]
003d23a8 str      r8, [sp, #0x1c]
003d23ac str      lr, [sp]
003d23b0 str      ip, [sp, #8]
003d23b4 str      ip, [sp, #4]
003d23b8 bl       #0x36b5d8
003d23bc ldr      r3, [r5, #0x1c]
003d23c0 cmp      r3, #0
003d23c4 beq      #0x3d23d8
003d23c8 mov      r0, r3
003d23cc ldr      r3, [r3]
003d23d0 mov      lr, pc
003d23d4 ldr      pc, [r3, #0x4c]
003d23d8 ldr      r3, [r4, r6]
003d23dc ldr      r2, [sp, #0x3c]
003d23e0 ldr      r3, [r3]
003d23e4 cmp      r2, r3
003d23e8 bne      #0x3d23f4
003d23ec add      sp, sp, #0x40
003d23f0 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d23f4 bl       #0x30e310
