# _ZN9Container8InitPostEv
0039f910 push     {r4, r5, r6, r7, r8, lr}
0039f914 sub      sp, sp, #8
0039f918 mov      r4, r0
0039f91c bl       #0x38bd64
0039f920 ldr      r3, [r4, #0x274]
0039f924 ldr      r5, [pc, #0x200]
0039f928 cmp      r0, r3
0039f92c add      r5, pc, r5
0039f930 blt      #0x39f93c
0039f934 add      sp, sp, #8
0039f938 pop      {r4, r5, r6, r7, r8, pc}
0039f93c ldr      r3, [r4]
0039f940 mov      r0, r4
0039f944 mov      lr, pc
0039f948 ldr      pc, [r3, #0xd0]
0039f94c ldr      r3, [r4]
0039f950 str      r0, [r4, #0x374]
0039f954 mov      r0, r4
0039f958 mov      lr, pc
0039f95c ldr      pc, [r3, #0xcc]
0039f960 cmn      r0, #1
0039f964 beq      #0x39f9a4
0039f968 ldr      r3, [r4, #0x374]
0039f96c cmn      r3, #1
0039f970 beq      #0x39f9a4
0039f974 ldr      r3, [pc, #0x1b4]
0039f978 mov      r2, #0xc
0039f97c ldr      r3, [r5, r3]
0039f980 ldr      r3, [r3]
0039f984 mla      r0, r2, r0, r3
0039f988 ldr      r6, [r0, #8]
0039f98c mov      r0, r6
0039f990 bl       #0x30de54
0039f994 mov      r1, r6
0039f998 add      r2, r6, r0
0039f99c add      r0, r4, #0x290
0039f9a0 bl       #0x3109e0
0039f9a4 mov      r0, r4
0039f9a8 bl       #0x38be5c
0039f9ac mov      r0, r4
0039f9b0 bl       #0x38ab60
0039f9b4 subs     r1, r0, #0
0039f9b8 beq      #0x39faf8
0039f9bc ldr      r8, [r4, #0x2d8]
0039f9c0 cmp      r8, #0
0039f9c4 beq      #0x39fa38
0039f9c8 ldr      r3, [pc, #0x164]
0039f9cc ldr      r6, [r8, #0x38]
0039f9d0 mov      r2, r4
0039f9d4 ldr      r1, [r5, r3]
0039f9d8 ldr      r3, [pc, #0x158]
0039f9dc ldr      ip, [r6]
0039f9e0 mov      r0, r6
0039f9e4 ldr      r3, [r5, r3]
0039f9e8 str      r4, [sp]
0039f9ec mov      lr, pc
0039f9f0 ldr      pc, [ip, #0x2c]
0039f9f4 ldr      r1, [pc, #0x140]
0039f9f8 mov      r7, #0
0039f9fc ldr      ip, [r6]
0039fa00 add      r1, pc, r1
0039fa04 str      r7, [sp]
0039fa08 mov      r0, r6
0039fa0c mov      r2, r7
0039fa10 mov      r3, r7
0039fa14 mov      lr, pc
0039fa18 ldr      pc, [ip, #0x20]
0039fa1c cmp      r0, #0
0039fa20 beq      #0x39fa94
0039fa24 mov      r1, r7
0039fa28 mov      r0, r4
0039fa2c bl       #0x39f3cc
0039fa30 mov      r0, r8
0039fa34 bl       #0x470a54
0039fa38 ldr      r3, [pc, #0x100]
0039fa3c ldr      r3, [r5, r3]
0039fa40 ldr      r5, [r3]
0039fa44 cmp      r5, #0
0039fa48 beq      #0x39fa68
0039fa4c ldr      r3, [r4]
0039fa50 mov      r0, r4
0039fa54 mov      lr, pc
0039fa58 ldr      pc, [r3, #0xd8]
0039fa5c mov      r1, r0
0039fa60 mov      r0, r5
0039fa64 bl       #0x3699fc
0039fa68 ldr      r3, [r4]
0039fa6c mov      r0, r4
0039fa70 mov      lr, pc
0039fa74 ldr      pc, [r3, #0xc8]
0039fa78 ldr      r2, [pc, #0xc4]
0039fa7c mov      r1, r0
0039fa80 mov      r0, r4
0039fa84 add      r2, pc, r2
0039fa88 add      sp, sp, #8
0039fa8c pop      {r4, r5, r6, r7, r8, lr}
0039fa90 b        #0x38ef60
0039fa94 ldr      r1, [pc, #0xac]
0039fa98 mov      r2, r0
0039fa9c ldr      ip, [r6]
0039faa0 mov      r3, r2
0039faa4 str      r0, [sp]
0039faa8 add      r1, pc, r1
0039faac mov      r0, r6
0039fab0 mov      lr, pc
0039fab4 ldr      pc, [ip, #0x20]
0039fab8 subs     r3, r0, #0
0039fabc bne      #0x39fb1c
0039fac0 ldr      r1, [pc, #0x84]
0039fac4 ldr      ip, [r6]
0039fac8 mov      r2, r3
0039facc mov      r0, r6
0039fad0 add      r1, pc, r1
0039fad4 str      r3, [sp]
0039fad8 mov      lr, pc
0039fadc ldr      pc, [ip, #0x20]
0039fae0 cmp      r0, #0
0039fae4 beq      #0x39fa30
0039fae8 mov      r0, r4
0039faec mov      r1, #2
0039faf0 bl       #0x39f3cc
0039faf4 b        #0x39fa30
0039faf8 mov      r0, r4
0039fafc ldr      r3, [r4]
0039fb00 mov      lr, pc
0039fb04 ldr      pc, [r3, #0x40]
0039fb08 mov      r0, r4
0039fb0c mov      r1, #4
0039fb10 add      sp, sp, #8
0039fb14 pop      {r4, r5, r6, r7, r8, lr}
0039fb18 b        #0x39f3cc
0039fb1c mov      r0, r4
0039fb20 mov      r1, #1
0039fb24 bl       #0x39f3cc
0039fb28 b        #0x39fa30
0039fb2c subseq   r5, pc, r4, ror #2
0039fb30 andeq    r1, r0, r8, lsr #25
0039fb34 andeq    r4, r0, r0, lsl #24
0039fb38 andeq    r1, r0, ip, ror #30
0039fb3c subseq   r3, r2, r0, asr r5
0039fb40 andeq    r0, r0, r4, lsr #27
0039fb44 ldrsheq  r3, [r2], #-0xc
0039fb48 subseq   r3, r2, r0, lsl #8
0039fb4c subseq   r2, r2, r0, ror #15
