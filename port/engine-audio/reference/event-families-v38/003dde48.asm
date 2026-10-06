# _ZN9AISPlayer9OnDeAggroEP9Character
003dde48 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dde4c ldr      r4, [pc, #0x264]
003dde50 ldr      r6, [pc, #0x264]
003dde54 ldr      r7, [pc, #0x264]
003dde58 add      r4, pc, r4
003dde5c ldr      r3, [r4, r6]
003dde60 sub      sp, sp, #0x4c
003dde64 mov      sb, r1
003dde68 ldr      r3, [r3]
003dde6c mov      r5, r0
003dde70 add      r8, sp, #0x2c
003dde74 str      r3, [sp, #0x44]
003dde78 bl       #0x3dbea8
003dde7c ldr      sl, [r4, r7]
003dde80 mov      r0, sl
003dde84 bl       #0x337888
003dde88 ldr      r1, [pc, #0x234]
003dde8c add      r2, sp, #0x10
003dde90 mov      r0, r8
003dde94 add      r1, pc, r1
003dde98 bl       #0x3140ec
003dde9c mov      r0, sl
003ddea0 mov      r1, r8
003ddea4 bl       #0x337a88
003ddea8 mov      sl, r0
003ddeac ldr      r0, [sp, #0x40]
003ddeb0 cmp      r0, r8
003ddeb4 beq      #0x3dded4
003ddeb8 cmp      r0, #0
003ddebc beq      #0x3dded4
003ddec0 ldr      r1, [sp, #0x2c]
003ddec4 rsb      r1, r0, r1
003ddec8 cmp      r1, #0x80
003ddecc bhi      #0x3de054
003dded0 bl       #0x708f00
003dded4 cmp      sl, #0
003dded8 bne      #0x3de030
003ddedc ldr      r3, [r5, #0xd0]
003ddee0 sub      r3, r3, #1
003ddee4 str      r3, [r5, #0xd0]
003ddee8 bl       #0x7fd794
003ddeec ldrb     r3, [r0, #5]
003ddef0 cmp      r3, #0
003ddef4 bne      #0x3de010
003ddef8 ldr      r8, [pc, #0x1c8]
003ddefc ldr      r3, [pc, #0x1c8]
003ddf00 ldr      fp, [r4, r8]
003ddf04 ldr      r3, [r4, r3]
003ddf08 mov      r0, fp
003ddf0c ldr      r8, [r3]
003ddf10 bl       #0x31f594
003ddf14 mov      r0, sb
003ddf18 ldr      sl, [r5, #0xd4]
003ddf1c bl       #0x3a3024
003ddf20 ldr      r3, [r0, #0x14]
003ddf24 rsb      sl, r3, sl
003ddf28 str      sl, [r5, #0xd4]
003ddf2c ldrb     r3, [r8, #0x31]
003ddf30 cmp      r3, #0
003ddf34 bne      #0x3ddf64
003ddf38 cmp      sl, #0
003ddf3c bne      #0x3ddf64
003ddf40 ldr      r1, [pc, #0x188]
003ddf44 mov      r0, r8
003ddf48 mov      sb, #1
003ddf4c add      r1, pc, r1
003ddf50 bl       #0x369514
003ddf54 ldrb     r3, [r8, #0x32]
003ddf58 strb     sb, [r8, #0x31]
003ddf5c cmp      r3, #0
003ddf60 bne      #0x3de084
003ddf64 ldr      r3, [r5, #0xd0]
003ddf68 cmp      r3, #0
003ddf6c beq      #0x3de05c
003ddf70 ldr      r8, [r4, r7]
003ddf74 add      r7, sp, #0x14
003ddf78 mov      r0, r8
003ddf7c bl       #0x337888
003ddf80 ldr      r1, [pc, #0x14c]
003ddf84 add      r2, sp, #0xc
003ddf88 mov      r0, r7
003ddf8c add      r1, pc, r1
003ddf90 bl       #0x3140ec
003ddf94 mov      r0, r8
003ddf98 mov      r1, r7
003ddf9c bl       #0x337a88
003ddfa0 mov      r8, r0
003ddfa4 ldr      r0, [sp, #0x28]
003ddfa8 cmp      r0, r7
003ddfac beq      #0x3ddfcc
003ddfb0 cmp      r0, #0
003ddfb4 beq      #0x3ddfcc
003ddfb8 ldr      r1, [sp, #0x14]
003ddfbc rsb      r1, r0, r1
003ddfc0 cmp      r1, #0x80
003ddfc4 bhi      #0x3de07c
003ddfc8 bl       #0x708f00
003ddfcc cmp      r8, #0
003ddfd0 beq      #0x3ddff4
003ddfd4 ldr      r0, [pc, #0xfc]
003ddfd8 ldr      r1, [pc, #0xfc]
003ddfdc ldr      r3, [r5, #0xd4]
003ddfe0 ldr      r0, [r4, r0]
003ddfe4 add      r1, pc, r1
003ddfe8 ldr      r2, [r5, #0xd0]
003ddfec add      r0, r0, #0xa8
003ddff0 bl       #0x30e004
003ddff4 ldr      r3, [r4, r6]
003ddff8 ldr      r2, [sp, #0x44]
003ddffc ldr      r3, [r3]
003de000 cmp      r2, r3
003de004 bne      #0x3de0b4
003de008 add      sp, sp, #0x4c
003de00c pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003de010 ldr      r8, [pc, #0xb0]
003de014 ldr      r1, [r5, #0x98]
003de018 ldr      r3, [r4, r8]
003de01c ldr      r0, [r3, #0x40]
003de020 bl       #0x36effc
003de024 cmp      r0, #0
003de028 beq      #0x3ddff4
003de02c b        #0x3ddefc
003de030 ldr      r0, [pc, #0xa0]
003de034 ldr      r1, [pc, #0xa4]
003de038 ldr      r2, [r5, #0xd0]
003de03c ldr      r0, [r4, r0]
003de040 add      r1, pc, r1
003de044 ldr      r3, [r5, #0xd4]
003de048 add      r0, r0, #0xa8
003de04c bl       #0x30e004
003de050 b        #0x3ddedc
003de054 bl       #0x310440
003de058 b        #0x3dded4
003de05c ldrb     r3, [r8, #0x32]
003de060 cmp      r3, #0
003de064 bne      #0x3ddf70
003de068 ldr      r3, [r5, #0xc4]
003de06c ldr      r2, [r5, #0xc8]
003de070 cmp      r3, r2
003de074 strne    r3, [r5, #0xc8]
003de078 b        #0x3ddf70
003de07c bl       #0x310440
003de080 b        #0x3ddfcc
003de084 mov      r0, fp
003de088 bl       #0x31f594
003de08c ldr      r1, [r0, #0x120]
003de090 cmp      r1, #0
003de094 blt      #0x3ddf64
003de098 mov      ip, #0x7d0
003de09c mov      r2, sb
003de0a0 mov      r3, sl
003de0a4 mov      r0, r8
003de0a8 str      ip, [sp]
003de0ac bl       #0x36bd78
003de0b0 b        #0x3ddf64
003de0b4 bl       #0x30e310
003de0b8 subseq   r6, fp, r8, lsr ip
003de0bc andeq    r4, r0, ip, lsr #1
003de0c0 andeq    r0, r0, r4, lsl #17
003de0c4 subeq    r7, lr, r4, lsl sp
003de0c8 strdeq   r3, r4, [r0], -r4
003de0cc andeq    r0, r0, r4, lsr #27
003de0d0 ldrdeq   r3, r4, [lr], #-0xc4
003de0d4 subeq    r7, lr, ip, lsl ip
003de0d8 andeq    r1, r0, r0, asr #19
003de0dc umaaleq  r7, lr, r4, ip
003de0e0 subeq    r7, lr, r0, lsl #24
