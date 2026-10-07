00530ba8 push     {r4, r5, r6, lr}
00530bac ldr      r4, [pc, #0x280]
00530bb0 ldr      r3, [pc, #0x280]
00530bb4 mov      r5, #0
00530bb8 add      r4, pc, r4
00530bbc ldr      r3, [r4, r3]
00530bc0 sub      sp, sp, #0x10
00530bc4 str      r5, [r3]
00530bc8 bl       #0x531af8
00530bcc ldr      r3, [pc, #0x268]
00530bd0 ldr      lr, [pc, #0x268]
00530bd4 ldr      r6, [r4, r3]
00530bd8 ldr      r3, [pc, #0x264]
00530bdc ldr      lr, [r4, lr]
00530be0 ldr      r1, [r4, r3]
00530be4 ldr      r3, [pc, #0x25c]
00530be8 strb     r5, [lr]
00530bec str      r0, [r1]
00530bf0 ldr      ip, [r4, r3]
00530bf4 ldr      r3, [pc, #0x250]
00530bf8 ldr      r0, [pc, #0x250]
00530bfc ldr      r1, [r6]
00530c00 ldr      r2, [r4, r3]
00530c04 ldr      r3, [pc, #0x248]
00530c08 add      r0, pc, r0
00530c0c str      r5, [r2]
00530c10 ldr      r3, [r4, r3]
00530c14 strb     r5, [ip]
00530c18 strb     r5, [r3]
00530c1c bl       #0x324114
00530c20 ldr      r3, [r6]
00530c24 movw     r2, #0x356
00530c28 cmp      r3, r2
00530c2c beq      #0x530db8
00530c30 cmp      r3, #0x3c0
00530c34 beq      #0x530da0
00530c38 cmp      r3, #0x320
00530c3c beq      #0x530d60
00530c40 ldr      r3, [pc, #0x210]
00530c44 add      r3, pc, r3
00530c48 ldr      r1, [r3, #0x20]
00530c4c ldr      r2, [r3, #0x24]
00530c50 stm      r3, {r1, r2}
00530c54 ldr      r1, [pc, #0x200]
00530c58 mov      ip, #0
00530c5c mov      r3, ip
00530c60 add      r1, pc, r1
00530c64 mov      r2, #0x10
00530c68 mov      r0, #1
00530c6c str      ip, [sp]
00530c70 str      ip, [sp, #4]
00530c74 str      ip, [sp, #8]
00530c78 bl       #0x5340ac
00530c7c ldr      r1, [pc, #0x1dc]
00530c80 ldr      r2, [pc, #0x1dc]
00530c84 mov      r3, r0
00530c88 ldr      r5, [r4, r1]
00530c8c ldr      r1, [pc, #0x1d4]
00530c90 ldr      r2, [r4, r2]
00530c94 str      r3, [r5]
00530c98 ldr      ip, [r4, r1]
00530c9c mov      r0, r2
00530ca0 mov      r1, r3
00530ca4 str      r2, [ip]
00530ca8 bl       #0x32fab4
00530cac ldr      r3, [pc, #0x1b8]
00530cb0 ldr      r2, [r5]
00530cb4 ldr      r1, [r4, r3]
00530cb8 ldr      r3, [pc, #0x1b0]
00530cbc ldr      r0, [r4, r3]
00530cc0 mvn      r3, #0
00530cc4 str      r3, [r1]
00530cc8 str      r3, [r0]
00530ccc ldr      r3, [pc, #0x1a0]
00530cd0 ldr      r2, [r2, #0x10]
00530cd4 ldr      r3, [r4, r3]
00530cd8 str      r2, [r3]
00530cdc bl       #0x532d10
00530ce0 cmp      r0, #5
00530ce4 addls    pc, pc, r0, lsl #2
00530ce8 b        #0x530d14
00530cec b        #0x530e20
00530cf0 b        #0x530e0c
00530cf4 b        #0x530df8
00530cf8 b        #0x530de4
00530cfc b        #0x530dd0
00530d00 b        #0x530d04
00530d04 ldr      r3, [pc, #0x16c]
00530d08 mov      r2, #1
00530d0c ldr      r3, [r4, r3]
00530d10 strb     r2, [r3]
00530d14 bl       #0x532d58
00530d18 cmp      r0, #2
00530d1c beq      #0x530d8c
00530d20 cmp      r0, #0x63
00530d24 beq      #0x530d78
00530d28 cmp      r0, #1
00530d2c bne      #0x530d4c
00530d30 ldr      r0, [pc, #0x144]
00530d34 add      r0, pc, r0
00530d38 bl       #0x324114
00530d3c ldr      r3, [pc, #0x13c]
00530d40 mov      r2, #0
00530d44 ldr      r3, [r4, r3]
00530d48 strb     r2, [r3]
00530d4c ldr      r0, [pc, #0x130]
00530d50 add      r0, pc, r0
00530d54 add      sp, sp, #0x10
00530d58 pop      {r4, r5, r6, lr}
00530d5c b        #0x324114
00530d60 ldr      r3, [pc, #0x120]
00530d64 add      r3, pc, r3
00530d68 ldr      r1, [r3, #8]
00530d6c ldr      r2, [r3, #0xc]
00530d70 stm      r3, {r1, r2}
00530d74 b        #0x530c54
00530d78 ldr      r3, [pc, #0x10c]
00530d7c mov      r2, #1
00530d80 ldr      r3, [r4, r3]
00530d84 strb     r2, [r3]
00530d88 b        #0x530d4c
00530d8c ldr      r3, [pc, #0xec]
00530d90 mov      r2, #0
00530d94 ldr      r3, [r4, r3]
00530d98 strb     r2, [r3]
00530d9c b        #0x530d4c
00530da0 ldr      r3, [pc, #0xe8]
00530da4 add      r3, pc, r3
00530da8 ldr      r1, [r3, #0x18]
00530dac ldr      r2, [r3, #0x1c]
00530db0 stm      r3, {r1, r2}
00530db4 b        #0x530c54
00530db8 ldr      r3, [pc, #0xd4]
00530dbc add      r3, pc, r3
00530dc0 ldr      r1, [r3, #0x10]
00530dc4 ldr      r2, [r3, #0x14]
00530dc8 stm      r3, {r1, r2}
00530dcc b        #0x530c54
00530dd0 ldr      r3, [pc, #0xc0]
00530dd4 mov      r2, #1
00530dd8 ldr      r3, [r4, r3]
00530ddc strb     r2, [r3]
00530de0 b        #0x530d14
00530de4 ldr      r3, [pc, #0xb0]
00530de8 mov      r2, #1
00530dec ldr      r3, [r4, r3]
00530df0 strb     r2, [r3]
00530df4 b        #0x530d14
00530df8 ldr      r3, [pc, #0xa0]
00530dfc mov      r2, #1
00530e00 ldr      r3, [r4, r3]
00530e04 strb     r2, [r3]
00530e08 b        #0x530d14
00530e0c ldr      r3, [pc, #0x90]
00530e10 mov      r2, #1
00530e14 ldr      r3, [r4, r3]
00530e18 strb     r2, [r3]
00530e1c b        #0x530d14
00530e20 ldr      r3, [pc, #0x80]
00530e24 mov      r2, #1
00530e28 ldr      r3, [r4, r3]
00530e2c strb     r2, [r3]
00530e30 b        #0x530d14
00530e34 ldrdeq   r3, r4, [r6], #-0xe8
00530e38 andeq    r1, r0, r8, asr #14
00530e3c andeq    r2, r0, r4, asr #11
00530e40 andeq    r2, r0, r8, lsr #13
00530e44 andeq    r0, r0, r0, lsl #12
00530e48 andeq    r4, r0, r0, ror #2
00530e4c ldrdeq   r2, r3, [r0], -r4
00530e50 eorseq   ip, sl, r8, lsr #16
00530e54 andeq    r4, r0, r8, lsr #18
00530e58 subeq    r5, ip, r4, ror #14
00530e5c subeq    r5, ip, r8, asr #14
00530e60 andeq    r1, r0, r8, ror #29
00530e64 strdeq   r3, r4, [r0], -r4
00530e68 andeq    r4, r0, r4, lsr #13
00530e6c strdeq   r3, r4, [r0], -r4
00530e70 andeq    r3, r0, r0, lsr #30
00530e74 andeq    r3, r0, r0, ror #14
00530e78 andeq    r4, r0, r8, lsr #9
00530e7c eorseq   ip, sl, ip, lsr #14
00530e80 andeq    r3, r0, r0, lsr fp
00530e84 eorseq   ip, sl, r0, lsr r7
00530e88 subeq    r5, ip, r4, asr #12
00530e8c andeq    r1, r0, r0, ror #13
00530e90 subeq    r5, ip, r4, lsl #12
00530e94 subeq    r5, ip, ip, ror #11
00530e98 ldrdeq   r3, r4, [r0], -r0
00530e9c andeq    r0, r0, r4, lsl #31
00530ea0 andeq    r4, r0, r8, asr r4
00530ea4 strheq   r1, [r0], -r0
00530ea8 andeq    r2, r0, r8, ror #14