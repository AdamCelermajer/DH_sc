# _ZN15VoxSoundManager6Play3DEiRKN6glitch4core8vector3dIfEEbiff
0036b5d8 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b5dc ldr      r4, [pc, #0x200]
0036b5e0 ldr      r5, [pc, #0x200]
0036b5e4 ldr      r7, [pc, #0x200]
0036b5e8 add      r4, pc, r4
0036b5ec ldr      ip, [r4, r5]
0036b5f0 ldr      r6, [r4, r7]
0036b5f4 sub      sp, sp, #0x74
0036b5f8 ldr      ip, [ip]
0036b5fc mov      sb, r0
0036b600 mov      r0, r6
0036b604 str      r3, [sp, #0x1c]
0036b608 str      ip, [sp, #0x6c]
0036b60c mov      sl, r1
0036b610 mov      fp, r2
0036b614 bl       #0x337888
0036b618 ldr      r1, [pc, #0x1d0]
0036b61c add      r8, sp, #0x54
0036b620 add      r2, sp, #0x38
0036b624 add      r1, pc, r1
0036b628 mov      r0, r8
0036b62c bl       #0x3140ec
0036b630 mov      r0, r6
0036b634 mov      r1, r8
0036b638 bl       #0x337a88
0036b63c mov      r6, r0
0036b640 mov      r0, r8
0036b644 bl       #0x3139ac
0036b648 cmp      r6, #0
0036b64c beq      #0x36b670
0036b650 ldr      r3, [r4, r5]
0036b654 ldr      r2, [sp, #0x6c]
0036b658 mov      r0, #0
0036b65c ldr      r3, [r3]
0036b660 cmp      r2, r3
0036b664 bne      #0x36b7e0
0036b668 add      sp, sp, #0x74
0036b66c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036b670 ldr      r3, [pc, #0x17c]
0036b674 ldr      r0, [r4, r3]
0036b678 bl       #0x31f594
0036b67c cmp      r0, #0
0036b680 beq      #0x36b650
0036b684 ldr      r3, [r0, #0x130]
0036b688 cmp      r3, #0x26
0036b68c bne      #0x36b650
0036b690 bl       #0x7fd794
0036b694 ldrb     r3, [r0, #5]
0036b698 cmp      r3, #0
0036b69c beq      #0x36b6b4
0036b6a0 ldr      r3, [pc, #0x150]
0036b6a4 ldr      r3, [r4, r3]
0036b6a8 ldrb     r3, [r3]
0036b6ac cmp      r3, #0
0036b6b0 bne      #0x36b650
0036b6b4 cmp      sl, #0
0036b6b8 blt      #0x36b650
0036b6bc ldr      r3, [pc, #0x138]
0036b6c0 ldr      r3, [r4, r3]
0036b6c4 ldrb     r3, [r3]
0036b6c8 cmp      r3, #0
0036b6cc bne      #0x36b79c
0036b6d0 ldr      r3, [pc, #0x128]
0036b6d4 mov      r2, #0xc
0036b6d8 ldr      r3, [r4, r3]
0036b6dc ldr      r3, [r3]
0036b6e0 mla      sl, r2, sl, r3
0036b6e4 ldr      r3, [sl, #8]
0036b6e8 ldr      r8, [sl, #4]
0036b6ec cmp      r3, #1
0036b6f0 beq      #0x36b7bc
0036b6f4 add      ip, sp, #0x30
0036b6f8 str      ip, [sp]
0036b6fc add      ip, sp, #0x2c
0036b700 add      r3, sp, #0x28
0036b704 mov      r1, r8
0036b708 add      r2, sp, #0x20
0036b70c str      ip, [sp, #4]
0036b710 add      r0, sb, #0x64
0036b714 add      ip, sp, #0x24
0036b718 str      ip, [sp, #8]
0036b71c bl       #0x8896f4
0036b720 ldr      r7, [r4, r7]
0036b724 add      r6, sp, #0x3c
0036b728 mov      r0, r7
0036b72c bl       #0x337888
0036b730 ldr      r1, [pc, #0xcc]
0036b734 add      r2, sp, #0x34
0036b738 mov      r0, r6
0036b73c add      r1, pc, r1
0036b740 bl       #0x3140ec
0036b744 mov      r1, r6
0036b748 mov      r0, r7
0036b74c bl       #0x337a88
0036b750 mov      r0, r6
0036b754 bl       #0x3139ac
0036b758 ldr      ip, [sp, #0x30]
0036b75c mov      r0, sb
0036b760 mov      r1, r8
0036b764 str      ip, [sp]
0036b768 ldr      ip, [sp, #0x2c]
0036b76c ldr      r2, [sp, #0x20]
0036b770 ldr      r3, [sp, #0x28]
0036b774 str      ip, [sp, #4]
0036b778 ldr      ip, [sp, #0x24]
0036b77c str      fp, [sp, #0xc]
0036b780 str      ip, [sp, #8]
0036b784 ldr      ip, [sp, #0x9c]
0036b788 str      ip, [sp, #0x10]
0036b78c ldr      ip, [sp, #0xa0]
0036b790 str      ip, [sp, #0x14]
0036b794 bl       #0x36a7c0
0036b798 b        #0x36b650
0036b79c ldr      r3, [pc, #0x64]
0036b7a0 mov      r0, sl
0036b7a4 ldr      r2, [sp, #0x1c]
0036b7a8 ldr      r1, [r4, r3]
0036b7ac mov      r3, #2
0036b7b0 ldr      r1, [r1]
0036b7b4 bl       #0x531348
0036b7b8 b        #0x36b650
0036b7bc mov      ip, #0xbf000000
0036b7c0 add      ip, ip, #0x800000
0036b7c4 mov      r0, sb
0036b7c8 mov      r1, r8
0036b7cc mov      r2, fp
0036b7d0 mov      r3, ip
0036b7d4 str      ip, [sp]
0036b7d8 bl       #0x36b420
0036b7dc b        #0x36b650
0036b7e0 bl       #0x30e310
0036b7e4 rsbeq    sb, r2, r8, lsr #9
0036b7e8 andeq    r4, r0, ip, lsr #1
0036b7ec andeq    r0, r0, r4, lsl #17
0036b7f0 subseq   r5, r5, r4, asr #21
0036b7f4 strdeq   r3, r4, [r0], -r4
0036b7f8 andeq    r2, r0, r0, lsr #31
0036b7fc andeq    r3, r0, r0, lsr fp
0036b800 andeq    r3, r0, ip, lsr lr
0036b804 subseq   r5, r5, r4, asr #19
0036b808 andeq    r0, r0, r0, lsl #13
