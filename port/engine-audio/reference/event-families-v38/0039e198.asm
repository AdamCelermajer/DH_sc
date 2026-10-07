# _ZN11TriggerTrap8ActivateEv
0039e198 push     {r4, r5, r6, r7, lr}
0039e19c ldr      r6, [r0, #0x2d8]
0039e1a0 ldr      r5, [pc, #0xe4]
0039e1a4 sub      sp, sp, #0x24
0039e1a8 cmp      r6, #0
0039e1ac mov      r4, r0
0039e1b0 add      r5, pc, r5
0039e1b4 beq      #0x39e25c
0039e1b8 mov      r3, #1
0039e1bc strb     r3, [r0, #0x3c4]
0039e1c0 ldr      ip, [r6, #0x38]
0039e1c4 ldr      r1, [pc, #0xc4]
0039e1c8 mov      r3, #0
0039e1cc mov      r0, ip
0039e1d0 mov      r2, r3
0039e1d4 ldr      ip, [ip]
0039e1d8 add      r1, pc, r1
0039e1dc str      r3, [sp]
0039e1e0 mov      lr, pc
0039e1e4 ldr      pc, [ip, #0x20]
0039e1e8 ldr      r3, [r4, #0x3c0]
0039e1ec cmp      r3, #0
0039e1f0 blt      #0x39e254
0039e1f4 ldr      r2, [pc, #0x98]
0039e1f8 ldr      r3, [r4]
0039e1fc mov      r0, r4
0039e200 ldr      r2, [r5, r2]
0039e204 ldr      r7, [r2]
0039e208 mov      lr, pc
0039e20c ldr      pc, [r3, #0xec]
0039e210 ldr      lr, [r4, #0x168]
0039e214 ldr      r6, [r4, #0x160]
0039e218 ldr      r5, [r4, #0x164]
0039e21c mov      ip, #0xbf000000
0039e220 add      ip, ip, #0x800000
0039e224 mov      r1, r0
0039e228 str      lr, [sp, #0x1c]
0039e22c mov      r0, r7
0039e230 mov      lr, #1
0039e234 add      r2, sp, #0x14
0039e238 mov      r3, #0
0039e23c str      r6, [sp, #0x14]
0039e240 str      r5, [sp, #0x18]
0039e244 str      lr, [sp]
0039e248 str      ip, [sp, #8]
0039e24c str      ip, [sp, #4]
0039e250 bl       #0x36b5d8
0039e254 add      sp, sp, #0x24
0039e258 pop      {r4, r5, r6, r7, pc}
0039e25c ldr      r3, [r0, #0x3f0]
0039e260 cmp      r3, #0
0039e264 beq      #0x39e1e8
0039e268 add      r7, r0, #0x3e0
0039e26c mov      r0, r7
0039e270 ldr      r1, [r4, #0x3e4]
0039e274 bl       #0x39861c
0039e278 str      r7, [r4, #0x3ec]
0039e27c str      r6, [r4, #0x3f0]
0039e280 str      r7, [r4, #0x3e8]
0039e284 str      r6, [r4, #0x3e4]
0039e288 b        #0x39e1e8
0039e28c subseq   r6, pc, r0, ror #17
0039e290 subseq   r4, r2, r0, lsl #18
0039e294 andeq    r0, r0, r4, lsr #27
