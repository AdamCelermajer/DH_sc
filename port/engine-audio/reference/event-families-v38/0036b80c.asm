# _ZN15VoxSoundManager4PlayEibiib
0036b80c push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b810 ldr      r4, [pc, #0x268]
0036b814 ldr      r5, [pc, #0x268]
0036b818 ldr      lr, [pc, #0x268]
0036b81c add      r4, pc, r4
0036b820 ldr      ip, [r4, r5]
0036b824 ldr      r6, [r4, lr]
0036b828 sub      sp, sp, #0x8c
0036b82c ldr      ip, [ip]
0036b830 mov      r7, r0
0036b834 mov      r0, r6
0036b838 str      r3, [sp, #0x14]
0036b83c str      ip, [sp, #0x84]
0036b840 mov      sl, r1
0036b844 mov      fp, r2
0036b848 ldrb     sb, [sp, #0xb4]
0036b84c bl       #0x337888
0036b850 ldr      r1, [pc, #0x234]
0036b854 add      r8, sp, #0x6c
0036b858 add      r2, sp, #0x68
0036b85c add      r1, pc, r1
0036b860 mov      r0, r8
0036b864 bl       #0x3140ec
0036b868 mov      r0, r6
0036b86c mov      r1, r8
0036b870 bl       #0x337a88
0036b874 mov      r6, r0
0036b878 mov      r0, r8
0036b87c bl       #0x3139ac
0036b880 cmp      r6, #0
0036b884 bne      #0x36b9e0
0036b888 cmp      sl, #0
0036b88c blt      #0x36b9e0
0036b890 cmp      sb, #0
0036b894 beq      #0x36ba20
0036b898 ldr      r3, [pc, #0x1f0]
0036b89c ldr      r3, [r4, r3]
0036b8a0 ldrb     r3, [r3]
0036b8a4 cmp      r3, #0
0036b8a8 bne      #0x36ba00
0036b8ac ldr      r3, [pc, #0x1e0]
0036b8b0 mov      r2, #0xc
0036b8b4 add      r1, sp, #0x60
0036b8b8 ldr      r3, [r4, r3]
0036b8bc add      ip, sp, #0x5c
0036b8c0 add      r8, r7, #0x64
0036b8c4 ldr      r3, [r3]
0036b8c8 mov      r0, r8
0036b8cc mla      sl, r2, sl, r3
0036b8d0 add      r3, sp, #0x58
0036b8d4 ldr      r6, [sl, #4]
0036b8d8 add      r2, sp, #0x50
0036b8dc stm      sp, {r1, ip}
0036b8e0 mov      r1, r6
0036b8e4 add      ip, sp, #0x54
0036b8e8 str      ip, [sp, #8]
0036b8ec bl       #0x8896f4
0036b8f0 ldr      r3, [r7, #8]
0036b8f4 ldr      r1, [r3, r6, lsl #2]
0036b8f8 cmp      r1, #0
0036b8fc beq      #0x36ba5c
0036b900 ldr      r0, [r7]
0036b904 bl       #0x8624f8
0036b908 cmp      r0, #0
0036b90c beq      #0x36b9e0
0036b910 ldr      r0, [sp, #0x14]
0036b914 bl       #0x30e964
0036b918 mov      r1, #0x44000000
0036b91c add      r1, r1, #0x7a0000
0036b920 bl       #0x30ec94
0036b924 ldr      r3, [r7, #8]
0036b928 ldr      r2, [sp, #0x5c]
0036b92c mov      sl, r0
0036b930 ldr      r1, [r3, r6, lsl #2]
0036b934 ldr      r0, [r7]
0036b938 bl       #0x862618
0036b93c add      ip, sp, #0x67
0036b940 str      ip, [sp]
0036b944 add      ip, sp, #0x44
0036b948 mov      r1, r6
0036b94c mov      r0, r8
0036b950 add      r2, sp, #0x4c
0036b954 add      r3, sp, #0x48
0036b958 str      ip, [sp, #4]
0036b95c add      ip, sp, #0x40
0036b960 str      ip, [sp, #8]
0036b964 bl       #0x889894
0036b968 ldr      r3, [r7, #8]
0036b96c ldr      r1, [r7]
0036b970 mov      r8, #0
0036b974 ldr      r2, [r3, r6, lsl #2]
0036b978 add      r6, sp, #0x18
0036b97c ldr      r3, [sp, #0x4c]
0036b980 mov      r0, r6
0036b984 str      r8, [sp]
0036b988 bl       #0x862438
0036b98c ldr      r0, [r7]
0036b990 mov      r1, r6
0036b994 mov      r2, r8
0036b998 mov      r3, #1
0036b99c bl       #0x861d60
0036b9a0 ldr      r3, [sp, #0x40]
0036b9a4 mov      r2, r8
0036b9a8 ldr      r0, [r7]
0036b9ac mov      r1, r6
0036b9b0 bl       #0x861950
0036b9b4 ldr      r3, [sp, #0xb0]
0036b9b8 sub      r3, r3, #1
0036b9bc cmp      r3, #0x1d
0036b9c0 bls      #0x36ba48
0036b9c4 ldr      r0, [r7]
0036b9c8 mov      r3, sl
0036b9cc mov      r1, r6
0036b9d0 ldrb     r2, [sp, #0x67]
0036b9d4 bl       #0x8621c0
0036b9d8 mov      r0, r6
0036b9dc bl       #0x8683ac
0036b9e0 ldr      r3, [r4, r5]
0036b9e4 ldr      r2, [sp, #0x84]
0036b9e8 mov      r0, #0
0036b9ec ldr      r3, [r3]
0036b9f0 cmp      r2, r3
0036b9f4 bne      #0x36ba7c
0036b9f8 add      sp, sp, #0x8c
0036b9fc pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036ba00 ldr      r3, [pc, #0x90]
0036ba04 mov      r0, sl
0036ba08 mov      r2, fp
0036ba0c ldr      r1, [r4, r3]
0036ba10 mov      r3, #2
0036ba14 ldr      r1, [r1]
0036ba18 bl       #0x531348
0036ba1c b        #0x36b9e0
0036ba20 bl       #0x7fd794
0036ba24 ldrb     r3, [r0, #5]
0036ba28 cmp      r3, #0
0036ba2c beq      #0x36b898
0036ba30 ldr      r3, [pc, #0x64]
0036ba34 ldr      r3, [r4, r3]
0036ba38 ldrb     r3, [r3]
0036ba3c cmp      r3, #0
0036ba40 bne      #0x36b9e0
0036ba44 b        #0x36b898
0036ba48 ldr      r0, [r7]
0036ba4c mov      r1, r6
0036ba50 ldr      r2, [sp, #0x48]
0036ba54 bl       #0x862058
0036ba58 b        #0x36b9c4
0036ba5c mov      r1, r6
0036ba60 mov      r0, r7
0036ba64 bl       #0x3699fc
0036ba68 ldr      r3, [r7, #8]
0036ba6c ldr      r1, [r3, r6, lsl #2]
0036ba70 cmp      r1, #0
0036ba74 beq      #0x36b9e0
0036ba78 b        #0x36b900
0036ba7c bl       #0x30e310
0036ba80 rsbeq    sb, r2, r4, ror r2
0036ba84 andeq    r4, r0, ip, lsr #1
0036ba88 andeq    r0, r0, r4, lsl #17
0036ba8c subseq   r5, r5, ip, lsl #17
0036ba90 andeq    r3, r0, r0, lsr fp
0036ba94 andeq    r3, r0, ip, lsr lr
0036ba98 andeq    r0, r0, r0, lsl #13
0036ba9c andeq    r2, r0, r0, lsr #31
