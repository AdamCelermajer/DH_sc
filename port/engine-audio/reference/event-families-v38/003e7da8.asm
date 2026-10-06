# _ZN4Door8InitPostEv
003e7da8 push     {r4, r5, r6, r7, r8, sl, lr}
003e7dac sub      sp, sp, #0x24
003e7db0 mov      r4, r0
003e7db4 bl       #0x38bd64
003e7db8 ldr      r3, [r4, #0x274]
003e7dbc ldr      r5, [pc, #0x210]
003e7dc0 cmp      r0, r3
003e7dc4 add      r5, pc, r5
003e7dc8 bge      #0x3e7f44
003e7dcc ldr      r3, [pc, #0x204]
003e7dd0 ldr      r8, [r4, #0x39c]
003e7dd4 ldr      r3, [r5, r3]
003e7dd8 ldr      r7, [r3]
003e7ddc cmp      r7, #0
003e7de0 beq      #0x3e7f4c
003e7de4 ldr      r3, [pc, #0x1f0]
003e7de8 mov      r6, #0
003e7dec ldr      r3, [r5, r3]
003e7df0 ldr      sl, [r3]
003e7df4 b        #0x3e7e04
003e7df8 add      r6, r6, #1
003e7dfc cmp      r6, r7
003e7e00 beq      #0x3e7f4c
003e7e04 ldr      r1, [sl, r6, lsl #2]
003e7e08 mov      r0, r8
003e7e0c bl       #0x30e31c
003e7e10 cmp      r0, #0
003e7e14 bne      #0x3e7df8
003e7e18 cmn      r6, #1
003e7e1c str      r6, [r4, #0x3a0]
003e7e20 beq      #0x3e7e74
003e7e24 ldr      r3, [pc, #0x1b4]
003e7e28 mov      r2, #0x18
003e7e2c ldr      r3, [r5, r3]
003e7e30 ldr      r3, [r3]
003e7e34 mla      r6, r2, r6, r3
003e7e38 ldr      r3, [r6, #0x14]
003e7e3c cmn      r3, #1
003e7e40 beq      #0x3e7e74
003e7e44 ldr      r2, [pc, #0x198]
003e7e48 mov      r1, #0xc
003e7e4c ldr      r2, [r5, r2]
003e7e50 ldr      r2, [r2]
003e7e54 mla      r3, r1, r3, r2
003e7e58 ldr      r6, [r3, #8]
003e7e5c mov      r0, r6
003e7e60 bl       #0x30de54
003e7e64 mov      r1, r6
003e7e68 add      r2, r6, r0
003e7e6c add      r0, r4, #0x290
003e7e70 bl       #0x3109e0
003e7e74 mov      r0, r4
003e7e78 bl       #0x39771c
003e7e7c mov      r0, r4
003e7e80 bl       #0x38ab60
003e7e84 subs     r1, r0, #0
003e7e88 beq      #0x3e7f34
003e7e8c ldr      r6, [r4, #0x2d8]
003e7e90 cmp      r6, #0
003e7e94 beq      #0x3e7ecc
003e7e98 ldr      r3, [pc, #0x148]
003e7e9c ldr      r2, [r6, #0x38]
003e7ea0 ldr      r1, [r5, r3]
003e7ea4 ldr      r3, [pc, #0x140]
003e7ea8 mov      r0, r2
003e7eac ldr      ip, [r2]
003e7eb0 ldr      r3, [r5, r3]
003e7eb4 str      r4, [sp]
003e7eb8 mov      r2, r4
003e7ebc mov      lr, pc
003e7ec0 ldr      pc, [ip, #0x2c]
003e7ec4 mov      r0, r6
003e7ec8 bl       #0x470a54
003e7ecc ldrb     r3, [r4, #0x3a5]
003e7ed0 cmp      r3, #0
003e7ed4 bne      #0x3e7f58
003e7ed8 ldr      r3, [r4, #0x3a0]
003e7edc cmn      r3, #1
003e7ee0 beq      #0x3e7f44
003e7ee4 ldr      r2, [pc, #0x104]
003e7ee8 ldr      r6, [r5, r2]
003e7eec ldr      r0, [r6]
003e7ef0 cmp      r0, #0
003e7ef4 beq      #0x3e7f44
003e7ef8 ldr      r2, [pc, #0xe0]
003e7efc mov      r7, #0x18
003e7f00 ldr      r5, [r5, r2]
003e7f04 ldr      r2, [r5]
003e7f08 mla      r3, r7, r3, r2
003e7f0c ldr      r1, [r3, #0x10]
003e7f10 bl       #0x3699fc
003e7f14 ldr      r2, [r4, #0x3a0]
003e7f18 ldr      r3, [r5]
003e7f1c ldr      r0, [r6]
003e7f20 mla      r7, r7, r2, r3
003e7f24 ldr      r1, [r7, #0xc]
003e7f28 add      sp, sp, #0x24
003e7f2c pop      {r4, r5, r6, r7, r8, sl, lr}
003e7f30 b        #0x3699fc
003e7f34 mov      r0, r4
003e7f38 ldr      r3, [r4]
003e7f3c mov      lr, pc
003e7f40 ldr      pc, [r3, #0x40]
003e7f44 add      sp, sp, #0x24
003e7f48 pop      {r4, r5, r6, r7, r8, sl, pc}
003e7f4c mvn      r3, #0
003e7f50 str      r3, [r4, #0x3a0]
003e7f54 b        #0x3e7e74
003e7f58 ldr      r3, [pc, #0x94]
003e7f5c mov      r1, #0
003e7f60 mov      r0, #0x28
003e7f64 ldr      r3, [r5, r3]
003e7f68 mov      r6, r1
003e7f6c ldr      r8, [r3, #0x44]
003e7f70 bl       #0x310570
003e7f74 mov      ip, #1
003e7f78 mov      lr, #2
003e7f7c mov      r1, r8
003e7f80 mov      r3, ip
003e7f84 mov      r2, r4
003e7f88 str      lr, [sp, #0x10]
003e7f8c movw     lr, #0xffff
003e7f90 mov      r7, r0
003e7f94 str      lr, [sp, #0x14]
003e7f98 str      r6, [sp]
003e7f9c str      r6, [sp, #4]
003e7fa0 str      r6, [sp, #8]
003e7fa4 str      r6, [sp, #0xc]
003e7fa8 str      ip, [sp, #0x18]
003e7fac bl       #0x46f2f0
003e7fb0 ldr      r3, [pc, #0x40]
003e7fb4 mov      r1, r7
003e7fb8 mov      r2, r6
003e7fbc ldr      r3, [r5, r3]
003e7fc0 mov      r0, r4
003e7fc4 add      r3, r3, #8
003e7fc8 str      r3, [r7]
003e7fcc bl       #0x394bf8
003e7fd0 b        #0x3e7ed8
003e7fd4 subseq   ip, sl, ip, asr #25
003e7fd8 strdeq   r1, r2, [r0], -r4
003e7fdc andeq    r4, r0, r8, lsl #9
003e7fe0 andeq    r1, r0, r8, lsl #28
003e7fe4 andeq    r1, r0, r8, lsr #25
003e7fe8 andeq    r0, r0, r8, asr #20
003e7fec andeq    r3, r0, r4, lsl #21
003e7ff0 andeq    r0, r0, r4, lsr #27
003e7ff4 strdeq   r3, r4, [r0], -r4
003e7ff8 andeq    r2, r0, r8, lsl r4
