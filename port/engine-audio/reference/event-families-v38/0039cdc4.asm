# _ZN14ProjectileTrap14SpecificUpdateEv
0039cdc4 push     {r4, r5, r6, r7, r8, sl, lr}
0039cdc8 ldrb     r3, [r0, #0x40c]
0039cdcc ldr      r5, [pc, #0x130]
0039cdd0 sub      sp, sp, #0x24
0039cdd4 cmp      r3, #0
0039cdd8 mov      r4, r0
0039cddc add      r5, pc, r5
0039cde0 beq      #0x39ce1c
0039cde4 ldr      r3, [r0, #0x3c0]
0039cde8 cmp      r3, #0
0039cdec blt      #0x39ce10
0039cdf0 ldr      r3, [pc, #0x110]
0039cdf4 ldr      r6, [r0, #0x404]
0039cdf8 ldr      r0, [r5, r3]
0039cdfc bl       #0x31f66c
0039ce00 rsb      r0, r0, r6
0039ce04 cmp      r0, #0
0039ce08 str      r0, [r4, #0x404]
0039ce0c ble      #0x39ce30
0039ce10 ldrb     r8, [r4, #0x3c4]
0039ce14 cmp      r8, #0
0039ce18 bne      #0x39ce24
0039ce1c add      sp, sp, #0x24
0039ce20 pop      {r4, r5, r6, r7, r8, sl, pc}
0039ce24 mov      r0, r4
0039ce28 bl       #0x39f168
0039ce2c b        #0x39ce1c
0039ce30 ldrb     r8, [r4, #0x3c4]
0039ce34 cmp      r8, #0
0039ce38 movne    r3, #0
0039ce3c strne    r3, [r4, #0x404]
0039ce40 bne      #0x39ce14
0039ce44 ldr      r3, [r4, #0x3c0]
0039ce48 cmp      r3, #0
0039ce4c blt      #0x39ceb0
0039ce50 ldr      r2, [pc, #0xb4]
0039ce54 ldr      r3, [r4]
0039ce58 mov      r0, r4
0039ce5c ldr      r2, [r5, r2]
0039ce60 ldr      sl, [r2]
0039ce64 mov      lr, pc
0039ce68 ldr      pc, [r3, #0xec]
0039ce6c ldr      lr, [r4, #0x168]
0039ce70 ldr      r6, [r4, #0x164]
0039ce74 ldr      r7, [r4, #0x160]
0039ce78 mov      ip, #0xbf000000
0039ce7c add      ip, ip, #0x800000
0039ce80 mov      r1, r0
0039ce84 str      lr, [sp, #0x1c]
0039ce88 mov      r0, sl
0039ce8c mov      lr, #1
0039ce90 mov      r3, r8
0039ce94 add      r2, sp, #0x14
0039ce98 str      r7, [sp, #0x14]
0039ce9c str      r6, [sp, #0x18]
0039cea0 str      lr, [sp]
0039cea4 str      ip, [sp, #8]
0039cea8 str      ip, [sp, #4]
0039ceac bl       #0x36b5d8
0039ceb0 ldr      r3, [r4]
0039ceb4 mov      r0, r4
0039ceb8 mov      lr, pc
0039cebc ldr      pc, [r3, #0xf8]
0039cec0 ldr      r2, [pc, #0x48]
0039cec4 ldr      r3, [pc, #0x48]
0039cec8 mov      ip, #0
0039cecc ldr      lr, [r5, r2]
0039ced0 ldr      r3, [r5, r3]
0039ced4 str      r0, [r4, #0x404]
0039ced8 ldr      r1, [r4, #0x408]
0039cedc mov      r0, r3
0039cee0 str      lr, [sp, #4]
0039cee4 mov      r3, ip
0039cee8 mov      lr, #1
0039ceec mov      r2, r4
0039cef0 str      lr, [sp, #0xc]
0039cef4 str      ip, [sp]
0039cef8 str      ip, [sp, #8]
0039cefc bl       #0x3e701c
0039cf00 b        #0x39ce10
0039cf04 ldrheq   r7, [pc], #-0xc4
0039cf08 strdeq   r3, r4, [r0], -r4
0039cf0c andeq    r0, r0, r4, lsr #27
0039cf10 ldrdeq   r3, r4, [r0], -ip
0039cf14 andeq    r0, r0, ip, asr #16
