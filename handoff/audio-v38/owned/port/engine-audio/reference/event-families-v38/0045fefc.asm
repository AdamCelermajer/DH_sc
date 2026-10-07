# _ZN16Script_PlaySound7ExecuteEbi
0045fefc push     {r4, r5, r6, r7, r8, lr}
0045ff00 ldr      r4, [pc, #0xe8]
0045ff04 ldr      r5, [pc, #0xe8]
0045ff08 sub      sp, sp, #0x28
0045ff0c add      r4, pc, r4
0045ff10 ldr      r3, [r4, r5]
0045ff14 cmp      r1, #0
0045ff18 ldr      r3, [r3]
0045ff1c str      r3, [sp, #0x24]
0045ff20 ldr      r6, [r0, #0xc]
0045ff24 beq      #0x45ff34
0045ff28 ldrb     r3, [r6, #0xc]
0045ff2c cmp      r3, #0
0045ff30 beq      #0x45ffa0
0045ff34 ldr      r3, [pc, #0xbc]
0045ff38 add      r7, sp, #0xc
0045ff3c ldr      r8, [r4, r3]
0045ff40 mov      r0, r8
0045ff44 bl       #0x337888
0045ff48 ldr      r1, [pc, #0xac]
0045ff4c add      r2, sp, #8
0045ff50 mov      r0, r7
0045ff54 add      r1, pc, r1
0045ff58 bl       #0x3140ec
0045ff5c mov      r1, r7
0045ff60 mov      r0, r8
0045ff64 bl       #0x337a88
0045ff68 mov      r0, r7
0045ff6c bl       #0x318254
0045ff70 ldrb     ip, [r6, #0xc]
0045ff74 cmp      ip, #0
0045ff78 bne      #0x45ffbc
0045ff7c ldr      r2, [pc, #0x7c]
0045ff80 ldr      r3, [r6, #8]
0045ff84 ldr      r1, [r6, #0x10]
0045ff88 ldr      r0, [r4, r2]
0045ff8c ldrb     r2, [r6, #0xd]
0045ff90 ldr      r0, [r0]
0045ff94 str      ip, [sp, #4]
0045ff98 str      ip, [sp]
0045ff9c bl       #0x36b80c
0045ffa0 ldr      r3, [r4, r5]
0045ffa4 ldr      r2, [sp, #0x24]
0045ffa8 ldr      r3, [r3]
0045ffac cmp      r2, r3
0045ffb0 bne      #0x45ffec
0045ffb4 add      sp, sp, #0x28
0045ffb8 pop      {r4, r5, r6, r7, r8, pc}
0045ffbc ldr      r2, [pc, #0x3c]
0045ffc0 ldr      r3, [r6, #8]
0045ffc4 ldr      r1, [r6, #0x10]
0045ffc8 ldr      r0, [r4, r2]
0045ffcc mov      ip, #0x7d0
0045ffd0 ldrb     r2, [r6, #0xd]
0045ffd4 ldr      r0, [r0]
0045ffd8 subs     r3, r3, #0
0045ffdc movne    r3, #1
0045ffe0 str      ip, [sp]
0045ffe4 bl       #0x36bd78
0045ffe8 b        #0x45ffa0
0045ffec bl       #0x30e310
0045fff0 subseq   r4, r3, r4, lsl #23
0045fff4 andeq    r4, r0, ip, lsr #1
0045fff8 andeq    r0, r0, r4, lsl #17
0045fffc subeq    sp, r6, ip, lsr #2
00460000 andeq    r0, r0, r4, lsr #27
