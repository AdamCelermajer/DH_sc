# _ZN15VoxSoundManager14CrossfadeMusicEii
0036baa0 push     {r4, r5, r6, r7, r8, sb, sl, lr}
0036baa4 ldr      r5, [pc, #0x178]
0036baa8 ldr      r3, [pc, #0x178]
0036baac sub      sp, sp, #0x30
0036bab0 add      r5, pc, r5
0036bab4 ldr      r3, [r5, r3]
0036bab8 mov      r4, r0
0036babc ldrb     r3, [r3]
0036bac0 cmp      r3, #0
0036bac4 bne      #0x36bbf8
0036bac8 cmn      r2, #1
0036bacc beq      #0x36bbf8
0036bad0 ldrb     r3, [r0, #0x31]
0036bad4 cmp      r3, #0
0036bad8 ldr      r3, [pc, #0x14c]
0036badc moveq    sl, r1
0036bae0 movne    r8, r1
0036bae4 ldr      r3, [r5, r3]
0036bae8 movne    sl, r2
0036baec mov      r1, #0xc
0036baf0 ldr      r3, [r3]
0036baf4 moveq    r8, r2
0036baf8 mla      r2, r1, sl, r3
0036bafc mla      r3, r1, r8, r3
0036bb00 ldr      r7, [r2, #4]
0036bb04 ldr      sb, [r3, #4]
0036bb08 str      r7, [r3, #4]
0036bb0c ldr      r2, [r2, #8]
0036bb10 str      r2, [r3, #8]
0036bb14 ldr      r3, [r0, #8]
0036bb18 ldr      r2, [r3, sb, lsl #2]
0036bb1c cmp      r2, #0
0036bb20 beq      #0x36bc00
0036bb24 ldr      r2, [r3, r7, lsl #2]
0036bb28 cmp      r2, #0
0036bb2c beq      #0x36bc10
0036bb30 ldr      r2, [pc, #0xf8]
0036bb34 mvn      r0, #0
0036bb38 mvn      r1, #0
0036bb3c ldr      r2, [r5, r2]
0036bb40 strd     r0, r1, [sp, #0x10]
0036bb44 mov      r6, #0
0036bb48 add      r5, sp, #0x30
0036bb4c add      r2, r2, #8
0036bb50 str      r2, [r5, #-0x28]!
0036bb54 str      r6, [sp, #0x18]
0036bb58 str      r6, [sp, #0x1c]
0036bb5c str      r6, [sp, #0x20]
0036bb60 str      r6, [sp, #0x24]
0036bb64 str      r6, [sp, #0x28]
0036bb68 ldr      r1, [r3, sb, lsl #2]
0036bb6c mov      r2, r5
0036bb70 mov      r3, #1
0036bb74 ldr      r0, [r4]
0036bb78 bl       #0x862548
0036bb7c mov      r1, r5
0036bb80 ldr      r0, [r4]
0036bb84 bl       #0x862000
0036bb88 mov      r1, r8
0036bb8c mov      sb, r0
0036bb90 mov      r2, #0x7d0
0036bb94 mov      r0, r4
0036bb98 bl       #0x369fec
0036bb9c mov      ip, #2
0036bba0 mov      r1, sl
0036bba4 mov      r2, #1
0036bba8 mov      r3, #0x7d0
0036bbac mov      r0, r4
0036bbb0 str      ip, [sp]
0036bbb4 str      r6, [sp, #4]
0036bbb8 bl       #0x36b80c
0036bbbc ldr      r1, [r4, #8]
0036bbc0 mov      r3, #1
0036bbc4 mov      r2, r5
0036bbc8 ldr      r1, [r1, r7, lsl #2]
0036bbcc ldr      r0, [r4]
0036bbd0 bl       #0x862548
0036bbd4 ldr      r0, [r4]
0036bbd8 mov      r2, sb
0036bbdc mov      r1, r5
0036bbe0 bl       #0x861fd8
0036bbe4 ldrb     r3, [r4, #0x31]
0036bbe8 mov      r0, r5
0036bbec eor      r3, r3, #1
0036bbf0 strb     r3, [r4, #0x31]
0036bbf4 bl       #0x8683ac
0036bbf8 add      sp, sp, #0x30
0036bbfc pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0036bc00 mov      r1, sb
0036bc04 bl       #0x3699fc
0036bc08 ldr      r3, [r4, #8]
0036bc0c b        #0x36bb24
0036bc10 mov      r0, r4
0036bc14 mov      r1, r7
0036bc18 bl       #0x3699fc
0036bc1c ldr      r3, [r4, #8]
0036bc20 b        #0x36bb30
0036bc24 rsbeq    r8, r2, r0, ror #31
0036bc28 andeq    r3, r0, r0, lsr fp
0036bc2c andeq    r3, r0, ip, lsr lr
0036bc30 andeq    r2, r0, r8, lsr #28
