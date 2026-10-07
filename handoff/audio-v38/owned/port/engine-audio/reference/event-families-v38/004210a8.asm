# _ZN8MenuBase12FS_PlayMusicEPKcS1_Pv
004210a8 push     {r4, r5, r6, r7, r8, lr}
004210ac ldr      r4, [pc, #0xa0]
004210b0 subs     r6, r1, #0
004210b4 sub      sp, sp, #8
004210b8 add      r4, pc, r4
004210bc beq      #0x421148
004210c0 ldrsb    r3, [r6]
004210c4 cmp      r3, #0
004210c8 beq      #0x421148
004210cc ldr      r3, [pc, #0x84]
004210d0 ldr      r3, [r4, r3]
004210d4 ldr      r7, [r3]
004210d8 cmp      r7, #0
004210dc beq      #0x421148
004210e0 ldr      r3, [pc, #0x74]
004210e4 mov      r5, #0
004210e8 ldr      r3, [r4, r3]
004210ec ldr      r8, [r3]
004210f0 b        #0x421100
004210f4 add      r5, r5, #1
004210f8 cmp      r5, r7
004210fc beq      #0x421148
00421100 ldr      r1, [r8, r5, lsl #2]
00421104 mov      r0, r6
00421108 bl       #0x30e31c
0042110c cmp      r0, #0
00421110 bne      #0x4210f4
00421114 cmn      r5, #1
00421118 beq      #0x421148
0042111c ldr      r2, [pc, #0x3c]
00421120 mov      r3, r0
00421124 mov      ip, #0x7d0
00421128 ldr      r0, [r4, r2]
0042112c mov      r1, r5
00421130 mov      r2, #1
00421134 ldr      r0, [r0]
00421138 str      ip, [sp]
0042113c bl       #0x36bd78
00421140 mov      r0, #1
00421144 b        #0x42114c
00421148 mov      r0, #0
0042114c add      sp, sp, #8
00421150 pop      {r4, r5, r6, r7, r8, pc}
00421154 ldrsbeq  r3, [r7], #-0x98
00421158 andeq    r3, r0, r8, lsr sp
0042115c andeq    r3, r0, r8, lsr #19
00421160 andeq    r0, r0, r4, lsr #27
