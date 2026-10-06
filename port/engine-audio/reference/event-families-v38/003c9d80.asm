# _ZN12CharAnimator13_AddAnimTableEiijjj
003c9d80 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003c9d84 ldr      r4, [pc, #0x19c]
003c9d88 ldr      sl, [pc, #0x19c]
003c9d8c mov      sb, r1
003c9d90 add      r4, pc, r4
003c9d94 ldr      ip, [r4, sl]
003c9d98 sub      sp, sp, #0x3c
003c9d9c cmp      r2, #0
003c9da0 ldr      r1, [ip]
003c9da4 mov      fp, r0
003c9da8 ldr      r0, [sp, #0x60]
003c9dac str      r1, [sp, #0x34]
003c9db0 blt      #0x3c9de8
003c9db4 ldr      r1, [pc, #0x174]
003c9db8 add      r2, r3, r2
003c9dbc ldr      r1, [r4, r1]
003c9dc0 ldr      r1, [r1]
003c9dc4 cmp      r2, r1
003c9dc8 bge      #0x3c9de8
003c9dcc ldr      r8, [sp, #0x64]
003c9dd0 and      r8, r8, r0
003c9dd4 cmp      r8, r0
003c9dd8 cmpne    r3, #0
003c9ddc moveq    r8, #0
003c9de0 movne    r8, #1
003c9de4 beq      #0x3c9e04
003c9de8 ldr      r3, [r4, sl]
003c9dec ldr      r2, [sp, #0x34]
003c9df0 ldr      r3, [r3]
003c9df4 cmp      r2, r3
003c9df8 bne      #0x3c9f24
003c9dfc add      sp, sp, #0x3c
003c9e00 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003c9e04 ldr      r3, [pc, #0x128]
003c9e08 ldr      r1, [pc, #0x128]
003c9e0c mov      r7, #0x14
003c9e10 ldr      r3, [r4, r3]
003c9e14 ldr      r6, [r4, r1]
003c9e18 add      r5, sp, #0x1c
003c9e1c ldr      r3, [r3]
003c9e20 mov      r0, r6
003c9e24 mla      r7, r7, r2, r3
003c9e28 bl       #0x337888
003c9e2c ldr      r1, [pc, #0x108]
003c9e30 add      r2, sp, #0x18
003c9e34 mov      r0, r5
003c9e38 add      r1, pc, r1
003c9e3c bl       #0x3140ec
003c9e40 mov      r1, r5
003c9e44 mov      r0, r6
003c9e48 bl       #0x337a88
003c9e4c mov      r0, r5
003c9e50 bl       #0x3139ac
003c9e54 ldr      r3, [r7, #8]
003c9e58 cmp      r3, #0
003c9e5c beq      #0x3c9de8
003c9e60 ldr      r2, [pc, #0xd8]
003c9e64 ldr      r3, [pc, #0xd8]
003c9e68 mov      r5, r8
003c9e6c str      r2, [sp, #0xc]
003c9e70 ldr      r2, [pc, #0xd0]
003c9e74 str      r3, [sp, #0x10]
003c9e78 mov      r6, r8
003c9e7c str      r2, [sp, #0x14]
003c9e80 b        #0x3c9ef0
003c9e84 ldr      r2, [r3, #8]
003c9e88 ldr      r3, [sp, #0xc]
003c9e8c mov      r1, sb
003c9e90 ldr      r0, [r4, r3]
003c9e94 bl       #0x47653c
003c9e98 ldr      r2, [sp, #0x10]
003c9e9c ldr      r3, [r4, r2]
003c9ea0 ldr      r0, [r3]
003c9ea4 cmp      r0, #0
003c9ea8 beq      #0x3c9ebc
003c9eac ldr      r3, [r7, #0xc]
003c9eb0 add      r3, r3, r5
003c9eb4 ldr      r1, [r3, #0x2c]
003c9eb8 bl       #0x3699fc
003c9ebc ldr      r3, [r7, #0xc]
003c9ec0 add      r3, r3, r5
003c9ec4 ldr      r1, [r3, #0x18]
003c9ec8 cmp      r1, #0
003c9ecc blt      #0x3c9edc
003c9ed0 ldr      r3, [sp, #0x14]
003c9ed4 ldr      r0, [r4, r3]
003c9ed8 bl       #0x4967e8
003c9edc ldr      r3, [r7, #8]
003c9ee0 add      r6, r6, #1
003c9ee4 add      r5, r5, #0x38
003c9ee8 cmp      r3, r6
003c9eec bls      #0x3c9de8
003c9ef0 ldr      r3, [r7, #0xc]
003c9ef4 add      r3, r3, r5
003c9ef8 ldr      r2, [r3, #0x28]
003c9efc cmp      r2, #0
003c9f00 beq      #0x3c9e84
003c9f04 ldr      r2, [r3, #8]
003c9f08 mov      r0, fp
003c9f0c mov      r1, sb
003c9f10 mov      r3, r8
003c9f14 str      r8, [sp]
003c9f18 str      r8, [sp, #4]
003c9f1c bl       #0x3c9d80
003c9f20 b        #0x3c9edc
003c9f24 bl       #0x30e310
003c9f28 subseq   sl, ip, r0, lsl #26
003c9f2c andeq    r4, r0, ip, lsr #1
003c9f30 andeq    r2, r0, r8, asr #20
003c9f34 andeq    r3, r0, ip, ror ip
003c9f38 andeq    r0, r0, r4, lsl #17
003c9f3c ldrdeq   sb, sl, [pc], #-0xf8
003c9f40 andeq    r4, r0, r8, lsr r8
003c9f44 andeq    r0, r0, r4, lsr #27
003c9f48 andeq    r1, r0, r8, lsl #22
