# _ZN21Script_PlayLevelMusic7ExecuteEbi
0045fd20 push     {r4, r5, r6, r7, r8, lr}
0045fd24 ldr      r4, [pc, #0xe0]
0045fd28 ldr      r6, [pc, #0xe0]
0045fd2c ldr      r2, [pc, #0xe0]
0045fd30 add      r4, pc, r4
0045fd34 ldr      r3, [r4, r6]
0045fd38 ldr      r7, [r4, r2]
0045fd3c sub      sp, sp, #0x28
0045fd40 ldr      r3, [r3]
0045fd44 add      r5, sp, #0xc
0045fd48 str      r3, [sp, #0x24]
0045fd4c ldr      r8, [r0, #0xc]
0045fd50 mov      r0, r7
0045fd54 bl       #0x337888
0045fd58 ldr      r1, [pc, #0xb8]
0045fd5c add      r2, sp, #8
0045fd60 mov      r0, r5
0045fd64 add      r1, pc, r1
0045fd68 bl       #0x3140ec
0045fd6c mov      r1, r5
0045fd70 mov      r0, r7
0045fd74 bl       #0x337a88
0045fd78 mov      r0, r5
0045fd7c bl       #0x318254
0045fd80 ldr      r3, [pc, #0x94]
0045fd84 ldr      r0, [r4, r3]
0045fd88 bl       #0x31f594
0045fd8c cmp      r0, #0
0045fd90 beq      #0x45fdd8
0045fd94 ldr      r3, [pc, #0x84]
0045fd98 ldr      r1, [r0, #0x11c]
0045fd9c ldr      r0, [r8, #8]
0045fda0 ldr      r3, [r4, r3]
0045fda4 mov      r2, #1
0045fda8 ldr      r5, [r3]
0045fdac mov      r3, #0
0045fdb0 str      r0, [sp]
0045fdb4 mov      r0, r5
0045fdb8 bl       #0x36bd78
0045fdbc ldrb     r3, [r5, #0x31]
0045fdc0 cmp      r3, #0
0045fdc4 bne      #0x45fdf4
0045fdc8 ldr      r1, [pc, #0x54]
0045fdcc mov      r0, r5
0045fdd0 add      r1, pc, r1
0045fdd4 bl       #0x369514
0045fdd8 ldr      r3, [r4, r6]
0045fddc ldr      r2, [sp, #0x24]
0045fde0 ldr      r3, [r3]
0045fde4 cmp      r2, r3
0045fde8 bne      #0x45fe08
0045fdec add      sp, sp, #0x28
0045fdf0 pop      {r4, r5, r6, r7, r8, pc}
0045fdf4 ldr      r1, [pc, #0x2c]
0045fdf8 mov      r0, r5
0045fdfc add      r1, pc, r1
0045fe00 bl       #0x369514
0045fe04 b        #0x45fdd8
0045fe08 bl       #0x30e310
0045fe0c subseq   r4, r3, r0, ror #26
0045fe10 andeq    r4, r0, ip, lsr #1
0045fe14 andeq    r0, r0, r4, lsl #17
0045fe18 subeq    sp, r6, ip, lsl r3
0045fe1c strdeq   r3, r4, [r0], -r4
0045fe20 andeq    r0, r0, r4, lsr #27
0045fe24 subeq    r1, r6, r8, asr lr
0045fe28 subeq    r1, r6, r4, lsr #28
