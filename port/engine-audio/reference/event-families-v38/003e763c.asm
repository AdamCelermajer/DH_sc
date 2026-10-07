# _ZN4Door5CloseEb
003e763c push     {r4, r5, r6, lr}
003e7640 ldrb     r3, [r0, #0x3ac]
003e7644 ldr      r4, [pc, #0x120]
003e7648 sub      sp, sp, #0x20
003e764c cmp      r3, #0
003e7650 mov      r5, r0
003e7654 add      r4, pc, r4
003e7658 bne      #0x3e7668
003e765c ldr      r3, [r0, #0x3a8]
003e7660 cmp      r3, #1
003e7664 beq      #0x3e7670
003e7668 add      sp, sp, #0x20
003e766c pop      {r4, r5, r6, pc}
003e7670 ldr      r6, [r0, #0x2d8]
003e7674 cmp      r6, #0
003e7678 beq      #0x3e76b0
003e767c cmp      r1, #0
003e7680 bne      #0x3e76b0
003e7684 ldr      r3, [r0]
003e7688 mov      lr, pc
003e768c ldr      pc, [r3, #0xc4]
003e7690 cmp      r0, #0
003e7694 beq      #0x3e772c
003e7698 ldrb     r3, [r5, #0x2ee]
003e769c cmp      r3, #0
003e76a0 beq      #0x3e772c
003e76a4 ldrb     r3, [r5, #0x2f0]
003e76a8 cmp      r3, #0
003e76ac bne      #0x3e772c
003e76b0 mov      r0, r5
003e76b4 mov      r1, #0
003e76b8 bl       #0x3e7598
003e76bc ldr      r3, [r5, #0x3a0]
003e76c0 cmn      r3, #1
003e76c4 beq      #0x3e7668
003e76c8 ldr      r2, [pc, #0xa0]
003e76cc ldr      r1, [pc, #0xa0]
003e76d0 ldr      lr, [r5, #0x168]
003e76d4 ldr      r2, [r4, r2]
003e76d8 ldr      r1, [r4, r1]
003e76dc ldr      r6, [r5, #0x160]
003e76e0 ldr      r2, [r2]
003e76e4 ldr      r0, [r1]
003e76e8 mov      r1, #0x18
003e76ec mla      r3, r1, r3, r2
003e76f0 ldr      r4, [r5, #0x164]
003e76f4 mov      ip, #0xbf000000
003e76f8 ldr      r1, [r3, #0xc]
003e76fc add      ip, ip, #0x800000
003e7700 str      lr, [sp, #0x1c]
003e7704 add      r2, sp, #0x14
003e7708 mov      lr, #1
003e770c mov      r3, #0
003e7710 str      r6, [sp, #0x14]
003e7714 str      r4, [sp, #0x18]
003e7718 str      lr, [sp]
003e771c str      ip, [sp, #8]
003e7720 str      ip, [sp, #4]
003e7724 bl       #0x36b5d8
003e7728 b        #0x3e7668
003e772c mov      r2, #2
003e7730 str      r2, [r5, #0x3a8]
003e7734 mov      r3, #0
003e7738 mov      r2, #1
003e773c strb     r2, [r5, #0x3ac]
003e7740 strb     r3, [r5, #0x85]
003e7744 ldr      ip, [r6, #0x38]
003e7748 ldr      r1, [pc, #0x28]
003e774c mov      r2, r3
003e7750 mov      r0, ip
003e7754 add      r1, pc, r1
003e7758 ldr      ip, [ip]
003e775c str      r3, [sp]
003e7760 mov      lr, pc
003e7764 ldr      pc, [ip, #0x20]
003e7768 b        #0x3e76bc
003e776c subseq   sp, sl, ip, lsr r4
003e7770 andeq    r1, r0, r8, lsl #28
003e7774 andeq    r0, r0, r4, lsr #27
003e7778 subeq    lr, sp, ip, ror #18
