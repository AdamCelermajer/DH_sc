# _ZN11TriggerTrap8InitPostEv
0039dee0 push     {r4, r5, r6, lr}
0039dee4 sub      sp, sp, #8
0039dee8 mov      r4, r0
0039deec bl       #0x38bd64
0039def0 ldr      r3, [r4, #0x274]
0039def4 ldr      r5, [pc, #0x1a0]
0039def8 cmp      r0, r3
0039defc add      r5, pc, r5
0039df00 blt      #0x39df0c
0039df04 add      sp, sp, #8
0039df08 pop      {r4, r5, r6, pc}
0039df0c ldr      r3, [r4]
0039df10 mov      r0, r4
0039df14 mov      lr, pc
0039df18 ldr      pc, [r3, #0xd8]
0039df1c cmn      r0, #1
0039df20 str      r0, [r4, #0x3c0]
0039df24 beq      #0x39df80
0039df28 ldr      r3, [r4]
0039df2c mov      r0, r4
0039df30 mov      lr, pc
0039df34 ldr      pc, [r3, #0xe0]
0039df38 cmn      r0, #1
0039df3c beq      #0x39df80
0039df40 ldr      r2, [pc, #0x158]
0039df44 ldr      r3, [r4]
0039df48 mov      r0, r4
0039df4c ldr      r2, [r5, r2]
0039df50 ldr      r6, [r2]
0039df54 mov      lr, pc
0039df58 ldr      pc, [r3, #0xe0]
0039df5c mov      r3, #0xc
0039df60 mla      r6, r3, r0, r6
0039df64 ldr      r6, [r6, #8]
0039df68 mov      r0, r6
0039df6c bl       #0x30de54
0039df70 mov      r1, r6
0039df74 add      r2, r6, r0
0039df78 add      r0, r4, #0x290
0039df7c bl       #0x3109e0
0039df80 mov      r0, r4
0039df84 bl       #0x39771c
0039df88 mov      r0, r4
0039df8c bl       #0x38ab60
0039df90 subs     r1, r0, #0
0039df94 beq      #0x39e088
0039df98 ldr      r6, [r4, #0x2d8]
0039df9c cmp      r6, #0
0039dfa0 beq      #0x39e02c
0039dfa4 ldr      r3, [pc, #0xf8]
0039dfa8 ldr      r2, [r6, #0x38]
0039dfac ldr      r1, [r5, r3]
0039dfb0 ldr      r3, [pc, #0xf0]
0039dfb4 ldr      ip, [r2]
0039dfb8 mov      r0, r2
0039dfbc ldr      r3, [r5, r3]
0039dfc0 mov      r2, r4
0039dfc4 str      r4, [sp]
0039dfc8 mov      lr, pc
0039dfcc ldr      pc, [ip, #0x2c]
0039dfd0 ldr      ip, [r6, #0x38]
0039dfd4 ldr      r1, [pc, #0xd0]
0039dfd8 mov      r3, #0
0039dfdc mov      r0, ip
0039dfe0 mov      r2, r3
0039dfe4 ldr      ip, [ip]
0039dfe8 add      r1, pc, r1
0039dfec str      r3, [sp]
0039dff0 mov      lr, pc
0039dff4 ldr      pc, [ip, #0x20]
0039dff8 subs     lr, r0, #0
0039dffc bne      #0x39e02c
0039e000 mov      r2, #1
0039e004 strb     r2, [r4, #0x401]
0039e008 ldr      ip, [r6, #0x38]
0039e00c ldr      r1, [pc, #0x9c]
0039e010 mov      r3, lr
0039e014 mov      r0, ip
0039e018 add      r1, pc, r1
0039e01c ldr      ip, [ip]
0039e020 str      lr, [sp]
0039e024 mov      lr, pc
0039e028 ldr      pc, [ip, #0x20]
0039e02c ldr      r3, [pc, #0x80]
0039e030 ldr      r3, [r5, r3]
0039e034 ldr      r5, [r3]
0039e038 cmp      r5, #0
0039e03c beq      #0x39e05c
0039e040 ldr      r3, [r4]
0039e044 mov      r0, r4
0039e048 mov      lr, pc
0039e04c ldr      pc, [r3, #0xec]
0039e050 mov      r1, r0
0039e054 mov      r0, r5
0039e058 bl       #0x3699fc
0039e05c ldr      r3, [r4]
0039e060 mov      r0, r4
0039e064 mov      lr, pc
0039e068 ldr      pc, [r3, #0xdc]
0039e06c ldr      r2, [pc, #0x44]
0039e070 mov      r1, r0
0039e074 mov      r0, r4
0039e078 add      r2, pc, r2
0039e07c add      sp, sp, #8
0039e080 pop      {r4, r5, r6, lr}
0039e084 b        #0x38ef60
0039e088 mov      r0, r4
0039e08c ldr      r3, [r4]
0039e090 mov      lr, pc
0039e094 ldr      pc, [r3, #0x40]
0039e098 b        #0x39df04
