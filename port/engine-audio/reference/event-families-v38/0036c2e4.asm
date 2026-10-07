# _ZN15VoxSoundManager10InitializeEv
0036c2e4 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036c2e8 ldr      sb, [pc, #0x3fc]
0036c2ec ldr      fp, [pc, #0x3fc]
0036c2f0 mov      r5, r0
0036c2f4 add      sb, pc, sb
0036c2f8 ldr      r3, [sb, fp]
0036c2fc ldr      r0, [pc, #0x3f0]
0036c300 sub      sp, sp, #0x7c
0036c304 ldr      r3, [r3]
0036c308 add      r0, pc, r0
0036c30c add      r4, sp, #0x5c
0036c310 str      r3, [sp, #0x74]
0036c314 bl       #0x324114
0036c318 ldr      r1, [pc, #0x3d8]
0036c31c mov      r3, #0
0036c320 add      r6, r5, #0x38
0036c324 strb     r3, [r5, #0x33]
0036c328 add      r1, pc, r1
0036c32c mov      r0, r4
0036c330 add      r2, sp, #0x40
0036c334 bl       #0x3140ec
0036c338 cmp      r6, r4
0036c33c beq      #0x36c350
0036c340 mov      r0, r6
0036c344 ldr      r1, [sp, #0x70]
0036c348 ldr      r2, [sp, #0x6c]
0036c34c bl       #0x3109e0
0036c350 mov      r0, r4
0036c354 bl       #0x3139ac
0036c358 ldr      r0, [pc, #0x39c]
0036c35c add      r0, pc, r0
0036c360 bl       #0x381744
0036c364 ldr      r3, [pc, #0x394]
0036c368 ldr      r1, [pc, #0x394]
0036c36c cmp      r0, #0
0036c370 ldr      r4, [sb, r3]
0036c374 strbeq   r0, [r5, #0x33]
0036c378 add      r1, pc, r1
0036c37c mov      r0, r4
0036c380 bl       #0x320e44
0036c384 bl       #0x30e964
0036c388 mov      r1, #1
0036c38c mov      r2, r0
0036c390 mov      r0, r5
0036c394 bl       #0x369da0
0036c398 ldr      r1, [pc, #0x368]
0036c39c mov      r0, r4
0036c3a0 add      r1, pc, r1
0036c3a4 bl       #0x320e44
0036c3a8 bl       #0x30e964
0036c3ac mov      r1, #2
0036c3b0 mov      r2, r0
0036c3b4 mov      r0, r5
0036c3b8 bl       #0x369da0
0036c3bc ldr      r1, [pc, #0x348]
0036c3c0 ldr      r0, [r5]
0036c3c4 add      r1, pc, r1
0036c3c8 bl       #0x861a08
0036c3cc mov      r1, #2
0036c3d0 mov      r2, #4
0036c3d4 ldr      r0, [r5]
0036c3d8 bl       #0x861b38
0036c3dc ldr      r3, [r5, #0x7c]
0036c3e0 ldr      r2, [r5, #0x80]
0036c3e4 rsb      r2, r3, r2
0036c3e8 asr      r3, r2, #3
0036c3ec add      r1, r3, r3, lsl #1
0036c3f0 add      r1, r1, r1, lsl #4
0036c3f4 add      r1, r1, r1, lsl #8
0036c3f8 add      r1, r1, r1, lsl #16
0036c3fc add      r1, r3, r1, lsl #2
0036c400 cmp      r1, #8
0036c404 bgt      #0x36c5bc
0036c408 cmp      r2, #0x4f
0036c40c ble      #0x36c488
0036c410 add      r7, r5, #0x64
0036c414 mov      r4, #1
0036c418 add      r6, sp, #0x38
0036c41c add      r8, sp, #0x34
0036c420 add      sl, sp, #0x30
0036c424 mov      r1, r4
0036c428 mov      r2, r6
0036c42c mov      r3, r8
0036c430 mov      r0, r7
0036c434 str      sl, [sp]
0036c438 bl       #0x889498
0036c43c ldr      ip, [sp, #0x30]
0036c440 ldr      r0, [r5]
0036c444 ldr      r2, [sp, #0x38]
0036c448 ldr      r3, [sp, #0x34]
0036c44c mov      r1, r4
0036c450 str      ip, [sp]
0036c454 bl       #0x862858
0036c458 ldr      r2, [r5, #0x80]
0036c45c ldr      r3, [r5, #0x7c]
0036c460 add      r4, r4, #1
0036c464 rsb      r3, r3, r2
0036c468 asr      r3, r3, #3
0036c46c add      r2, r3, r3, lsl #1
0036c470 add      r2, r2, r2, lsl #4
0036c474 add      r2, r2, r2, lsl #8
0036c478 add      r2, r2, r2, lsl #16
0036c47c add      r3, r3, r2, lsl #2
0036c480 cmp      r4, r3
0036c484 blt      #0x36c424
0036c488 ldr      r3, [pc, #0x280]
0036c48c ldr      r3, [sb, r3]
0036c490 ldrb     r3, [r3]
0036c494 cmp      r3, #0
0036c498 beq      #0x36c4c4
0036c49c ldr      r0, [pc, #0x270]
0036c4a0 add      r0, pc, r0
0036c4a4 bl       #0x324114
0036c4a8 ldr      r3, [sb, fp]
0036c4ac ldr      r2, [sp, #0x74]
0036c4b0 ldr      r3, [r3]
0036c4b4 cmp      r2, r3
0036c4b8 bne      #0x36c6e8
0036c4bc add      sp, sp, #0x7c
0036c4c0 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036c4c4 bl       #0x38174c
0036c4c8 cmp      r0, #0
0036c4cc beq      #0x36c5dc
0036c4d0 ldr      r4, [pc, #0x240]
0036c4d4 add      r8, r5, #0x64
0036c4d8 mov      r0, r8
0036c4dc add      r4, pc, r4
0036c4e0 mov      r1, r4
0036c4e4 bl       #0x88a490
0036c4e8 subs     sl, r0, #0
0036c4ec ble      #0x36c49c
0036c4f0 ldr      r2, [pc, #0x224]
0036c4f4 ldr      r3, [pc, #0x224]
0036c4f8 str      fp, [sp, #0x1c]
0036c4fc str      r2, [sp, #0xc]
0036c500 add      r2, r5, #0x14
0036c504 str      r2, [sp, #0x20]
0036c508 add      r2, sp, #0x2c
0036c50c str      r2, [sp, #0x10]
0036c510 add      r2, sp, #0x3c
0036c514 add      r3, pc, r3
0036c518 str      r2, [sp, #0x14]
0036c51c add      r2, sp, #0x28
0036c520 str      r4, [sp, #0x18]
0036c524 mov      r7, #0
0036c528 add      r6, sp, #0x44
0036c52c str      r2, [sp, #0x24]
0036c530 mov      fp, r3
0036c534 ldr      r1, [sp, #0x18]
0036c538 ldr      r2, [sp, #0x10]
0036c53c mov      r0, r8
0036c540 bl       #0x88bdac
0036c544 ldr      r3, [sp, #0xc]
0036c548 ldr      r4, [sb, r3]
0036c54c mov      r0, r4
0036c550 bl       #0x337888
0036c554 ldr      r2, [sp, #0x14]
0036c558 mov      r1, fp
0036c55c mov      r0, r6
0036c560 bl       #0x3140ec
0036c564 mov      r0, r4
0036c568 mov      r1, r6
0036c56c bl       #0x337a88
0036c570 mov      r0, r6
0036c574 bl       #0x3139ac
0036c578 ldr      r4, [r5, #0x10]
0036c57c ldr      r3, [r5, #0x14]
0036c580 cmp      r4, r3
0036c584 beq      #0x36c61c
0036c588 ldr      r3, [sp, #0x2c]
0036c58c str      r3, [r4]
0036c590 ldr      r3, [r5, #0x10]
0036c594 add      r3, r3, #4
0036c598 str      r3, [r5, #0x10]
0036c59c add      r7, r7, #1
0036c5a0 mov      r0, r5
0036c5a4 ldr      r1, [sp, #0x2c]
0036c5a8 bl       #0x3699fc
0036c5ac cmp      r7, sl
0036c5b0 bne      #0x36c534
0036c5b4 ldr      fp, [sp, #0x1c]
0036c5b8 b        #0x36c49c
0036c5bc ldr      r0, [pc, #0x160]
0036c5c0 mov      r2, #8
0036c5c4 add      r0, pc, r0
0036c5c8 bl       #0x30de84
0036c5cc ldr      r2, [r5, #0x80]
0036c5d0 ldr      r3, [r5, #0x7c]
0036c5d4 rsb      r2, r3, r2
0036c5d8 b        #0x36c408
0036c5dc ldr      r3, [pc, #0x144]
0036c5e0 ldr      r3, [sb, r3]
0036c5e4 ldrb     r3, [r3]
0036c5e8 cmp      r3, #0
0036c5ec bne      #0x36c4d0
0036c5f0 ldr      r3, [pc, #0x134]
0036c5f4 ldr      r3, [sb, r3]
0036c5f8 ldrb     r3, [r3]
0036c5fc cmp      r3, #0
0036c600 bne      #0x36c4d0
0036c604 ldr      r3, [pc, #0x124]
0036c608 ldr      r3, [sb, r3]
0036c60c ldrb     r3, [r3]
0036c610 cmp      r3, #0
0036c614 beq      #0x36c49c
0036c618 b        #0x36c4d0
0036c61c ldr      r2, [r5, #0xc]
0036c620 rsb      r2, r2, r4
0036c624 asr      r2, r2, #2
0036c628 cmp      r2, #1
0036c62c addhs    r3, r2, r2
0036c630 addlo    r3, r2, #1
0036c634 cmn      r3, #0xc0000001
0036c638 bhi      #0x36c6b8
0036c63c cmp      r2, r3
0036c640 bhi      #0x36c6b8
0036c644 mov      r1, r3
0036c648 ldr      r0, [sp, #0x20]
0036c64c ldr      r2, [sp, #0x24]
0036c650 str      r3, [sp, #0x28]
0036c654 bl       #0x35fd5c
0036c658 ldr      r1, [r5, #0xc]
0036c65c mov      ip, r0
0036c660 subs     r4, r4, r1
0036c664 moveq    r4, r0
0036c668 bne      #0x36c6d0
0036c66c ldr      r3, [sp, #0x2c]
0036c670 str      r3, [r4], #4
0036c674 ldr      r0, [r5, #0xc]
0036c678 ldr      r3, [r5, #0x14]
0036c67c cmp      r0, #0
0036c680 beq      #0x36c6a0
0036c684 rsb      r3, r0, r3
0036c688 bic      r1, r3, #3
0036c68c cmp      r1, #0x80
0036c690 bhi      #0x36c6c0
0036c694 str      ip, [sp, #8]
0036c698 bl       #0x708f00
0036c69c ldr      ip, [sp, #8]
0036c6a0 ldr      r3, [sp, #0x28]
0036c6a4 str      ip, [r5, #0xc]
0036c6a8 str      r4, [r5, #0x10]
0036c6ac add      r3, ip, r3, lsl #2
0036c6b0 str      r3, [r5, #0x14]
0036c6b4 b        #0x36c59c
0036c6b8 mvn      r3, #0xc0000000
0036c6bc b        #0x36c644
0036c6c0 str      ip, [sp, #8]
0036c6c4 bl       #0x310440
0036c6c8 ldr      ip, [sp, #8]
0036c6cc b        #0x36c6a0
0036c6d0 mov      r2, r4
0036c6d4 str      r0, [sp, #8]
0036c6d8 bl       #0x30df38
0036c6dc ldr      ip, [sp, #8]
0036c6e0 add      r4, r0, r4
0036c6e4 b        #0x36c66c
0036c6e8 bl       #0x30e310
0036c6ec mlseq    r2, ip, r7, r8
0036c6f0 andeq    r4, r0, ip, lsr #1
0036c6f4 subseq   r4, r5, r0, ror #28
0036c6f8 subseq   pc, r5, r0, ror #9
0036c6fc subseq   r4, r5, ip, asr #28
0036c700 strdeq   r3, r4, [r0], -r4
0036c704 subseq   r4, r5, r8, asr ip
0036c708 subseq   r4, r5, r0, lsl lr
0036c70c ldrsheq  r4, [r5], #-0xdc
0036c710 andeq    r3, r0, r0, lsr fp
0036c714 subseq   r4, r5, r0, ror #27
