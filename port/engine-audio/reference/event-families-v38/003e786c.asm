# _ZN4Door4OpenEb
003e786c push     {r4, r5, r6, lr}
003e7870 ldrb     r3, [r0, #0x3ac]
003e7874 ldr      r5, [pc, #0x11c]
003e7878 sub      sp, sp, #0x20
003e787c cmp      r3, #0
003e7880 mov      r4, r0
003e7884 add      r5, pc, r5
003e7888 bne      #0x3e7924
003e788c ldr      r3, [r0, #0x3a8]
003e7890 cmp      r3, #0
003e7894 bne      #0x3e7924
003e7898 ldr      r6, [r0, #0x2d8]
003e789c cmp      r6, #0
003e78a0 beq      #0x3e78ac
003e78a4 cmp      r1, #0
003e78a8 beq      #0x3e792c
003e78ac mov      r0, r4
003e78b0 mov      r1, #0
003e78b4 bl       #0x3e7788
003e78b8 ldr      r3, [r4, #0x3a0]
003e78bc cmn      r3, #1
003e78c0 beq      #0x3e7924
003e78c4 ldr      r2, [pc, #0xd0]
003e78c8 ldr      r1, [pc, #0xd0]
003e78cc ldr      lr, [r4, #0x168]
003e78d0 ldr      r2, [r5, r2]
003e78d4 ldr      r1, [r5, r1]
003e78d8 ldr      r6, [r4, #0x160]
003e78dc ldr      r2, [r2]
003e78e0 ldr      r0, [r1]
003e78e4 mov      r1, #0x18
003e78e8 mla      r3, r1, r3, r2
003e78ec ldr      r5, [r4, #0x164]
003e78f0 mov      ip, #0xbf000000
003e78f4 ldr      r1, [r3, #0x10]
003e78f8 add      ip, ip, #0x800000
003e78fc str      lr, [sp, #0x1c]
003e7900 add      r2, sp, #0x14
003e7904 mov      lr, #1
003e7908 mov      r3, #0
003e790c str      r6, [sp, #0x14]
003e7910 str      r5, [sp, #0x18]
003e7914 str      lr, [sp]
003e7918 str      ip, [sp, #8]
003e791c str      ip, [sp, #4]
003e7920 bl       #0x36b5d8
003e7924 add      sp, sp, #0x20
003e7928 pop      {r4, r5, r6, pc}
003e792c ldr      r3, [r0]
003e7930 mov      lr, pc
003e7934 ldr      pc, [r3, #0xc4]
003e7938 cmp      r0, #0
003e793c beq      #0x3e7958
003e7940 ldrb     r3, [r4, #0x2ee]
003e7944 cmp      r3, #0
003e7948 beq      #0x3e7958
003e794c ldrb     r3, [r4, #0x2f0]
003e7950 cmp      r3, #0
003e7954 beq      #0x3e78ac
003e7958 mov      r2, #3
003e795c str      r2, [r4, #0x3a8]
003e7960 mov      r3, #0
003e7964 mov      r2, #1
003e7968 strb     r2, [r4, #0x3ac]
003e796c strb     r3, [r4, #0x85]
003e7970 ldr      ip, [r6, #0x38]
003e7974 ldr      r1, [pc, #0x28]
003e7978 mov      r2, r3
003e797c mov      r0, ip
003e7980 add      r1, pc, r1
003e7984 ldr      ip, [ip]
003e7988 str      r3, [sp]
003e798c mov      lr, pc
003e7990 ldr      pc, [ip, #0x20]
003e7994 b        #0x3e78b8
003e7998 subseq   sp, sl, ip, lsl #4
003e799c andeq    r1, r0, r8, lsl #28
003e79a0 andeq    r0, r0, r4, lsr #27
003e79a4 subeq    sb, sp, r8, asr #25
