# _ZN9LuaScript10_PlayMusicERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e420 push     {r4, r5, r6, r7, lr}
0037e424 ldr      r5, [r0, #4]
0037e428 mov      r6, r0
0037e42c ldr      r4, [pc, #0x128]
0037e430 ldm      r5, {r0, r3}
0037e434 add      r4, pc, r4
0037e438 sub      sp, sp, #0xc
0037e43c rsb      r3, r0, r3
0037e440 asr      r3, r3, #4
0037e444 add      r2, r3, r3, lsl #3
0037e448 add      r2, r2, r2, lsl #6
0037e44c add      r2, r3, r2, lsl #3
0037e450 add      r2, r2, r2, lsl #15
0037e454 add      r3, r3, r2, lsl #3
0037e458 cmp      r3, #0
0037e45c bne      #0x37e470
0037e460 ldr      r0, [pc, #0xf8]
0037e464 add      r0, pc, r0
0037e468 bl       #0x708eb0
0037e46c ldr      r0, [r5]
0037e470 bl       #0x31c49c
0037e474 bl       #0x37ba84
0037e478 cmn      r0, #1
0037e47c mov      r5, r0
0037e480 beq      #0x37e504
0037e484 ldr      r7, [r6, #4]
0037e488 ldr      r2, [pc, #0xd4]
0037e48c ldm      r7, {r0, r3}
0037e490 ldr      r2, [r4, r2]
0037e494 rsb      r3, r0, r3
0037e498 asr      r3, r3, #4
0037e49c ldr      r6, [r2]
0037e4a0 add      r2, r3, r3, lsl #3
0037e4a4 add      r2, r2, r2, lsl #6
0037e4a8 add      r2, r3, r2, lsl #3
0037e4ac add      r2, r2, r2, lsl #15
0037e4b0 add      r3, r3, r2, lsl #3
0037e4b4 rsb      r3, r3, #0
0037e4b8 cmp      r3, #1
0037e4bc bls      #0x37e50c
0037e4c0 add      r0, r0, #0x70
0037e4c4 bl       #0x31bbf0
0037e4c8 bl       #0x30e4cc
0037e4cc mov      r1, r5
0037e4d0 str      r0, [sp]
0037e4d4 mov      r2, #1
0037e4d8 mov      r0, r6
0037e4dc mov      r3, #0
0037e4e0 bl       #0x36bd78
0037e4e4 ldr      r3, [pc, #0x7c]
0037e4e8 ldr      r0, [r4, r3]
0037e4ec bl       #0x31f594
0037e4f0 cmp      r0, #0
0037e4f4 beq      #0x37e504
0037e4f8 ldr      r3, [r0, #0x11c]
0037e4fc cmp      r5, r3
0037e500 beq      #0x37e520
0037e504 add      sp, sp, #0xc
0037e508 pop      {r4, r5, r6, r7, pc}
0037e50c ldr      r0, [pc, #0x58]
0037e510 add      r0, pc, r0
0037e514 bl       #0x708eb0
0037e518 ldr      r0, [r7]
0037e51c b        #0x37e4c0
0037e520 ldrb     r3, [r6, #0x31]
0037e524 cmp      r3, #0
0037e528 bne      #0x37e544
0037e52c ldr      r1, [pc, #0x3c]
0037e530 mov      r0, r6
0037e534 add      r1, pc, r1
0037e538 add      sp, sp, #0xc
0037e53c pop      {r4, r5, r6, r7, lr}
0037e540 b        #0x369514
0037e544 ldr      r1, [pc, #0x28]
0037e548 mov      r0, r6
0037e54c add      r1, pc, r1
0037e550 add      sp, sp, #0xc
0037e554 pop      {r4, r5, r6, r7, lr}
0037e558 b        #0x369514
0037e55c rsbeq    r6, r1, ip, asr r6
0037e560 subseq   r0, r4, r4
0037e564 andeq    r0, r0, r4, lsr #27
0037e568 strdeq   r3, r4, [r0], -r4
0037e56c subseq   pc, r3, r8, asr pc
0037e570 ldrsheq  r3, [r4], #-0x64
0037e574 ldrsbeq  r3, [r4], #-0x64
