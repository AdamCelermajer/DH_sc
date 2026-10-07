# _ZN15VoxSoundManager9PlayEventEiRKN6glitch4core8vector3dIfEEff
0036b420 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b424 ldr      r4, [pc, #0x190]
0036b428 ldr      r6, [pc, #0x190]
0036b42c ldr      lr, [pc, #0x190]
0036b430 add      r4, pc, r4
0036b434 ldr      ip, [r4, r6]
0036b438 ldr      r5, [r4, lr]
0036b43c sub      sp, sp, #0x7c
0036b440 ldr      ip, [ip]
0036b444 mov      r7, r0
0036b448 mov      r0, r5
0036b44c mov      fp, r3
0036b450 str      ip, [sp, #0x74]
0036b454 mov      sl, r1
0036b458 mov      sb, r2
0036b45c bl       #0x337888
0036b460 ldr      r1, [pc, #0x160]
0036b464 add      r8, sp, #0x5c
0036b468 add      r2, sp, #0x40
0036b46c add      r1, pc, r1
0036b470 mov      r0, r8
0036b474 bl       #0x3140ec
0036b478 mov      r1, r8
0036b47c mov      r0, r5
0036b480 bl       #0x337a88
0036b484 mov      r2, r0
0036b488 mov      r0, r8
0036b48c str      r2, [sp, #0x18]
0036b490 bl       #0x3139ac
0036b494 ldr      r2, [sp, #0x18]
0036b498 cmp      r2, #0
0036b49c bne      #0x36b57c
0036b4a0 cmp      sl, #0
0036b4a4 blt      #0x36b57c
0036b4a8 ldr      r3, [pc, #0x11c]
0036b4ac ldr      r3, [r4, r3]
0036b4b0 ldrb     r3, [r3]
0036b4b4 cmp      r3, #0
0036b4b8 bne      #0x36b59c
0036b4bc add      r3, r7, #0x64
0036b4c0 mov      r0, r3
0036b4c4 mov      r1, sl
0036b4c8 add      r2, sp, #0x38
0036b4cc str      r3, [sp, #0x1c]
0036b4d0 bl       #0x88bb6c
0036b4d4 ldr      r3, [sp, #0x38]
0036b4d8 cmp      r3, #0
0036b4dc blt      #0x36b57c
0036b4e0 mov      r0, r5
0036b4e4 bl       #0x337888
0036b4e8 ldr      r1, [pc, #0xe0]
0036b4ec add      r8, sp, #0x44
0036b4f0 add      r2, sp, #0x3c
0036b4f4 add      r1, pc, r1
0036b4f8 mov      r0, r8
0036b4fc bl       #0x3140ec
0036b500 mov      r1, r8
0036b504 mov      r0, r5
0036b508 bl       #0x337a88
0036b50c mov      r0, r8
0036b510 bl       #0x3139ac
0036b514 add      ip, sp, #0x34
0036b518 str      ip, [sp]
0036b51c add      ip, sp, #0x30
0036b520 add      r2, sp, #0x24
0036b524 ldr      r1, [sp, #0x38]
0036b528 add      r3, sp, #0x2c
0036b52c str      ip, [sp, #4]
0036b530 ldr      r0, [sp, #0x1c]
0036b534 add      ip, sp, #0x28
0036b538 str      ip, [sp, #8]
0036b53c bl       #0x8896f4
0036b540 ldr      ip, [sp, #0x34]
0036b544 mov      r0, r7
0036b548 ldr      r1, [sp, #0x38]
0036b54c str      ip, [sp]
0036b550 ldr      ip, [sp, #0x30]
0036b554 ldr      r2, [sp, #0x24]
0036b558 ldr      r3, [sp, #0x2c]
0036b55c str      ip, [sp, #4]
0036b560 ldr      ip, [sp, #0x28]
0036b564 str      sb, [sp, #0xc]
0036b568 str      fp, [sp, #0x10]
0036b56c str      ip, [sp, #8]
0036b570 ldr      ip, [sp, #0xa0]
0036b574 str      ip, [sp, #0x14]
0036b578 bl       #0x36a7c0
0036b57c ldr      r3, [r4, r6]
0036b580 ldr      r2, [sp, #0x74]
0036b584 mov      r0, #0
0036b588 ldr      r3, [r3]
0036b58c cmp      r2, r3
0036b590 bne      #0x36b5b8
0036b594 add      sp, sp, #0x7c
0036b598 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036b59c ldr      r3, [pc, #0x30]
0036b5a0 mov      r0, sl
0036b5a4 ldr      r1, [r4, r3]
0036b5a8 mov      r3, #2
0036b5ac ldr      r1, [r1]
0036b5b0 bl       #0x531348
0036b5b4 b        #0x36b57c
0036b5b8 bl       #0x30e310
0036b5bc rsbeq    sb, r2, r0, ror #12
0036b5c0 andeq    r4, r0, ip, lsr #1
0036b5c4 andeq    r0, r0, r4, lsl #17
0036b5c8 subseq   r5, r5, ip, ror ip
0036b5cc andeq    r3, r0, r0, lsr fp
0036b5d0 subseq   r5, r5, ip, lsl #24
0036b5d4 andeq    r0, r0, r0, lsl #13
