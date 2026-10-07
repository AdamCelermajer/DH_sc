# _ZN12CharAnimator18_PlayItemSwooshSFXEP12ItemInstance
003c94b8 push     {r4, r5, r6, r7, r8, lr}
003c94bc ldr      r5, [pc, #0x90]
003c94c0 cmp      r1, #0
003c94c4 sub      sp, sp, #0x20
003c94c8 mov      r4, r0
003c94cc add      r5, pc, r5
003c94d0 beq      #0x3c9548
003c94d4 mov      r0, r1
003c94d8 bl       #0x3f9e08
003c94dc ldr      r7, [r0, #0x14]
003c94e0 cmn      r7, #1
003c94e4 beq      #0x3c9548
003c94e8 ldr      r3, [pc, #0x68]
003c94ec ldr      r0, [r4, #4]
003c94f0 mov      r4, #1
003c94f4 ldr      r3, [r5, r3]
003c94f8 ldr      r8, [r3]
003c94fc bl       #0x3935dc
003c9500 ldr      r6, [r0]
003c9504 ldr      r5, [r0, #4]
003c9508 ldr      lr, [r0, #8]
003c950c mov      ip, #0xbf000000
003c9510 add      ip, ip, #0x800000
003c9514 mov      r0, r8
003c9518 mov      r1, r7
003c951c add      r2, sp, #0x14
003c9520 mov      r3, #0
003c9524 str      r6, [sp, #0x14]
003c9528 str      r5, [sp, #0x18]
003c952c str      lr, [sp, #0x1c]
003c9530 str      ip, [sp, #8]
003c9534 str      r4, [sp]
003c9538 str      ip, [sp, #4]
003c953c bl       #0x36b5d8
003c9540 mov      r0, r4
003c9544 b        #0x3c954c
003c9548 mov      r0, #0
003c954c add      sp, sp, #0x20
003c9550 pop      {r4, r5, r6, r7, r8, pc}
003c9554 subseq   fp, ip, r4, asr #11
003c9558 andeq    r0, r0, r4, lsr #27
