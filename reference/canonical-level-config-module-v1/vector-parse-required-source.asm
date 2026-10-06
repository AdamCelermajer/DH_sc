_Z10_DEBUG_OUTPKcz
00324114 push     {r0, r1, r2, r3}
00324118 ldr      r3, [pc, #0x88]
0032411c push     {r4, r5, lr}
00324120 ldr      r2, [pc, #0x84]
00324124 add      r3, pc, r3
00324128 sub      sp, sp, #0x400
0032412c ldr      r5, [r3, r2]
00324130 sub      sp, sp, #0xc
00324134 add      r4, sp, #8
00324138 ldr      lr, [r5]
0032413c add      ip, sp, #0x410
00324140 add      ip, ip, #0xc
00324144 sub      r4, r4, #4
00324148 mov      r2, ip
0032414c ldr      r1, [sp, #0x418]
00324150 mov      r0, r4
00324154 str      lr, [sp, #0x404]
00324158 str      ip, [sp]
0032415c bl       #0x30e268
00324160 ldr      r0, [pc, #0x48]
00324164 mov      r1, r4
00324168 add      r0, pc, r0
0032416c bl       #0x30de84
00324170 ldr      r0, [pc, #0x3c]
00324174 mov      r1, r4
00324178 add      r0, pc, r0
0032417c bl       #0x533560 ; appDebugLog
00324180 ldr      r2, [sp, #0x404]
00324184 ldr      r3, [r5]
00324188 cmp      r2, r3
0032418c bne      #0x3241a4
00324190 add      sp, sp, #0xc
00324194 add      sp, sp, #0x400
00324198 pop      {r4, r5, lr}
0032419c add      sp, sp, #0x10
003241a0 bx       lr
003241a4 bl       #0x30e310
003241a8 rsbeq    r0, r7, ip, ror #18
003241ac andeq    r4, r0, ip, lsr #1
003241b0 subseq   sl, ip, r8, lsl #25
003241b4 subseq   sl, sb, r0, lsl #25
