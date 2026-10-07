# _ZN15VoxSoundManager18SetInSafeZoneMusicEb
0036c014 push     {r4, r5, r6, r7, lr}
0036c018 ldr      r6, [pc, #0x124]
0036c01c ldr      r3, [pc, #0x124]
0036c020 sub      sp, sp, #0xc
0036c024 add      r6, pc, r6
0036c028 ldr      r3, [r6, r3]
0036c02c mov      r4, r0
0036c030 mov      r7, r1
0036c034 ldrb     r5, [r3]
0036c038 cmp      r5, #0
0036c03c bne      #0x36c0c0
0036c040 cmp      r1, #0
0036c044 beq      #0x36c08c
0036c048 ldr      r3, [pc, #0xfc]
0036c04c ldr      r0, [r6, r3]
0036c050 bl       #0x31f594
0036c054 ldr      r1, [r0, #0x120]
0036c058 cmp      r1, #0
0036c05c blt      #0x36c0f0
0036c060 ldrb     r3, [r4, #0x31]
0036c064 mov      r2, #1
0036c068 strb     r2, [r4, #0x32]
0036c06c cmp      r3, #0
0036c070 beq      #0x36c0b8
0036c074 mov      ip, #0x7d0
0036c078 mov      r0, r4
0036c07c mov      r3, r5
0036c080 str      ip, [sp]
0036c084 bl       #0x36bd78
0036c088 b        #0x36c0b8
0036c08c ldr      r3, [pc, #0xb8]
0036c090 strb     r1, [r4, #0x32]
0036c094 ldr      r0, [r6, r3]
0036c098 bl       #0x31f594
0036c09c mov      ip, #0x7d0
0036c0a0 ldr      r1, [r0, #0x11c]
0036c0a4 mov      r3, r7
0036c0a8 mov      r0, r4
0036c0ac mov      r2, #1
0036c0b0 str      ip, [sp]
0036c0b4 bl       #0x36bd78
0036c0b8 add      sp, sp, #0xc
0036c0bc pop      {r4, r5, r6, r7, pc}
0036c0c0 ldr      r3, [pc, #0x84]
0036c0c4 strb     r1, [r4, #0x32]
0036c0c8 ldr      r0, [r6, r3]
0036c0cc bl       #0x31f594
0036c0d0 mov      ip, #0x7d0
0036c0d4 ldr      r1, [r0, #0x11c]
0036c0d8 mov      r2, #1
0036c0dc mov      r0, r4
0036c0e0 mov      r3, #0
0036c0e4 str      ip, [sp]
0036c0e8 bl       #0x36bd78
0036c0ec b        #0x36c0b8
0036c0f0 ldr      r3, [pc, #0x58]
0036c0f4 ldr      r3, [r6, r3]
0036c0f8 ldr      r3, [r3]
0036c0fc cmp      r3, #2
0036c100 streq    r5, [r5]
0036c104 beq      #0x36c0b8
0036c108 cmp      r3, #1
0036c10c bne      #0x36c0b8
0036c110 ldr      r0, [pc, #0x3c]
0036c114 ldr      r1, [pc, #0x3c]
0036c118 ldr      r2, [pc, #0x3c]
0036c11c ldr      r0, [r6, r0]
0036c120 ldr      r3, [pc, #0x38]
0036c124 mov      ip, #0x5a0
0036c128 add      r1, pc, r1
0036c12c add      r2, pc, r2
0036c130 add      r3, pc, r3
0036c134 add      r0, r0, #0xa8
0036c138 str      ip, [sp]
0036c13c bl       #0x30e004
0036c140 b        #0x36c0b8
0036c144 rsbeq    r8, r2, ip, ror #20
0036c148 andeq    r3, r0, r0, lsr fp
0036c14c strdeq   r3, r4, [r0], -r4
0036c150 andeq    r3, r0, r0, asr #19
0036c154 andeq    r1, r0, r0, asr #19
0036c158 ldrheq   r2, [r5], #-0x20
0036c15c subseq   r2, r5, ip, lsr r4
0036c160 subseq   r4, r5, r0, ror #31
