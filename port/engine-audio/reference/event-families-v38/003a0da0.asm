# _ZN21DestructibleContainer8InteractEP10GameObject
003a0da0 push     {r4, r5, r6, r7, r8, sb, sl, lr}
003a0da4 ldr      r3, [r0, #0x6f4]
003a0da8 ldr      r5, [pc, #0x294]
003a0dac sub      sp, sp, #0x48
003a0db0 cmp      r3, #0
003a0db4 mov      r4, r0
003a0db8 mov      r6, r1
003a0dbc add      r5, pc, r5
003a0dc0 beq      #0x3a0e68
003a0dc4 ldr      r2, [r0, #0x2d8]
003a0dc8 sub      r1, r3, #1
003a0dcc str      r1, [r0, #0x6f4]
003a0dd0 cmp      r2, #0
003a0dd4 beq      #0x3a0e00
003a0dd8 ldr      ip, [r2, #0x38]
003a0ddc ldr      r0, [r0, #0x6f0]
003a0de0 mov      r3, #0
003a0de4 mov      r2, r3
003a0de8 rsb      r1, r1, r0
003a0dec mov      r0, ip
003a0df0 ldr      ip, [ip]
003a0df4 str      r3, [sp]
003a0df8 mov      lr, pc
003a0dfc ldr      pc, [ip, #0x1c]
003a0e00 ldr      r2, [pc, #0x240]
003a0e04 ldr      r3, [r4]
003a0e08 mov      r0, r4
003a0e0c ldr      r2, [r5, r2]
003a0e10 ldr      r7, [r2]
003a0e14 mov      lr, pc
003a0e18 ldr      pc, [r3, #0xd8]
003a0e1c ldr      lr, [r4, #0x168]
003a0e20 ldr      r6, [r4, #0x160]
003a0e24 ldr      r5, [r4, #0x164]
003a0e28 mov      ip, #0xbf000000
003a0e2c add      ip, ip, #0x800000
003a0e30 mov      r1, r0
003a0e34 str      lr, [sp, #0x38]
003a0e38 mov      r0, r7
003a0e3c mov      lr, #1
003a0e40 add      r2, sp, #0x30
003a0e44 mov      r3, #0
003a0e48 str      r6, [sp, #0x30]
003a0e4c str      r5, [sp, #0x34]
003a0e50 str      lr, [sp]
003a0e54 str      ip, [sp, #8]
003a0e58 str      ip, [sp, #4]
003a0e5c bl       #0x36b5d8
003a0e60 add      sp, sp, #0x48
003a0e64 pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003a0e68 ldr      r7, [pc, #0x1dc]
003a0e6c ldr      r0, [r5, r7]
003a0e70 bl       #0x31f594
003a0e74 subs     r8, r0, #0
003a0e78 beq      #0x3a0fe8
003a0e7c ldr      r3, [r4]
003a0e80 mov      r0, r4
003a0e84 mov      lr, pc
003a0e88 ldr      pc, [r3, #0xd0]
003a0e8c ldr      r7, [r5, r7]
003a0e90 ldr      r1, [pc, #0x1b8]
003a0e94 ldr      r2, [pc, #0x1b8]
003a0e98 mov      sb, r0
003a0e9c add      r1, pc, r1
003a0ea0 add      r2, pc, r2
003a0ea4 ldr      r0, [r7, #0x2c]
003a0ea8 ldr      sl, [r4, #0x64]
003a0eac bl       #0x4c4bdc
003a0eb0 ldr      r3, [pc, #0x1a0]
003a0eb4 add      r1, sp, #0x48
003a0eb8 str      r0, [sp, #0x18]
003a0ebc ldr      r3, [r5, r3]
003a0ec0 mov      r0, r8
003a0ec4 mov      r8, #0
003a0ec8 add      r3, r3, #8
003a0ecc str      r3, [r1, #-0x34]!
003a0ed0 mvn      r3, #0
003a0ed4 str      r3, [sp, #0x28]
003a0ed8 str      sl, [sp, #0x20]
003a0edc str      sb, [sp, #0x2c]
003a0ee0 str      r6, [sp, #0x1c]
003a0ee4 strb     r8, [sp, #0x24]
003a0ee8 strb     r8, [sp, #0x25]
003a0eec bl       #0x339090
003a0ef0 ldr      r3, [pc, #0x164]
003a0ef4 mov      r0, r4
003a0ef8 mov      r1, r6
003a0efc ldr      r3, [r5, r3]
003a0f00 add      r4, sp, #0x3c
003a0f04 add      r3, r3, #8
003a0f08 str      r3, [sp, #0x14]
003a0f0c bl       #0x3a0b38
003a0f10 mov      r0, r4
003a0f14 mov      r1, r6
003a0f18 bl       #0x33dd2c
003a0f1c mov      r0, r4
003a0f20 bl       #0x33ff54
003a0f24 subs     r4, r0, #0
003a0f28 beq      #0x3a0e60
003a0f2c ldr      r3, [r4]
003a0f30 mov      lr, pc
003a0f34 ldr      pc, [r3, #0x28]
003a0f38 cmp      r0, r8
003a0f3c beq      #0x3a0e60
003a0f40 add      r6, r4, #0x560
003a0f44 mov      r0, r6
003a0f48 mov      r1, #0xd9
003a0f4c mov      r2, #1
003a0f50 bl       #0x3e0798
003a0f54 ldr      r3, [pc, #0x104]
003a0f58 mov      r0, r6
003a0f5c mov      r1, #0xd9
003a0f60 ldr      r3, [r5, r3]
003a0f64 mov      r2, r8
003a0f68 ldr      r6, [r3]
003a0f6c bl       #0x3df6e0
003a0f70 cmp      r0, #0xc7
003a0f74 ble      #0x3a0e60
003a0f78 ldr      r0, [r7, #0x40]
003a0f7c mov      r1, r4
003a0f80 bl       #0x36effc
003a0f84 cmp      r0, r8
003a0f88 beq      #0x3a0e60
003a0f8c ldr      r3, [pc, #0xd0]
003a0f90 ldr      r3, [r5, r3]
003a0f94 ldr      r7, [r3]
003a0f98 cmp      r7, r8
003a0f9c beq      #0x3a103c
003a0fa0 ldr      r3, [pc, #0xc0]
003a0fa4 ldr      r3, [r5, r3]
003a0fa8 ldr      r5, [pc, #0xbc]
003a0fac ldr      r4, [r3]
003a0fb0 add      r5, pc, r5
003a0fb4 b        #0x3a0fc4
003a0fb8 add      r8, r8, #1
003a0fbc cmp      r8, r7
003a0fc0 beq      #0x3a103c
003a0fc4 ldr      r1, [r4, r8, lsl #2]
003a0fc8 mov      r0, r5
003a0fcc bl       #0x30e31c
003a0fd0 cmp      r0, #0
003a0fd4 bne      #0x3a0fb8
003a0fd8 mov      r1, r8
003a0fdc mov      r0, r6
003a0fe0 bl       #0x3813b8
003a0fe4 b        #0x3a0e60
003a0fe8 ldr      r3, [pc, #0x80]
003a0fec ldr      r3, [r5, r3]
003a0ff0 ldr      r3, [r3]
003a0ff4 cmp      r3, #2
003a0ff8 streq    r8, [r8]
003a0ffc beq      #0x3a0e7c
003a1000 cmp      r3, #1
003a1004 bne      #0x3a0e7c
003a1008 ldr      r0, [pc, #0x64]
003a100c ldr      r1, [pc, #0x64]
003a1010 ldr      r2, [pc, #0x64]
003a1014 ldr      r0, [r5, r0]
003a1018 ldr      r3, [pc, #0x60]
003a101c mov      ip, #0xe7
003a1020 add      r1, pc, r1
003a1024 add      r2, pc, r2
003a1028 add      r3, pc, r3
003a102c add      r0, r0, #0xa8
003a1030 str      ip, [sp]
003a1034 bl       #0x30e004
003a1038 b        #0x3a0e7c
003a103c mvn      r1, #0
003a1040 b        #0x3a0fdc
003a1044 ldrsbeq  r3, [pc], #-0xc4
003a1048 andeq    r0, r0, r4, lsr #27
003a104c strdeq   r3, r4, [r0], -r4
003a1050 subseq   r1, r2, ip, asr #21
003a1054 subseq   r2, r2, r0, asr r1
003a1058 muleq    r0, r0, r6
003a105c strheq   r0, [r0], -r0
003a1060 andeq    r1, r0, r0, ror sp
003a1064 strdeq   r0, r1, [r0], -ip
003a1068 andeq    r1, r0, ip, lsr #32
003a106c subseq   r2, r2, r8, asr r0
003a1070 andeq    r3, r0, r0, asr #19
003a1074 andeq    r1, r0, r0, asr #19
003a1078 ldrheq   sp, [r1], #-0x38
003a107c subseq   lr, r6, r4, lsr sb
003a1080 subseq   r1, r2, r8, ror #30
