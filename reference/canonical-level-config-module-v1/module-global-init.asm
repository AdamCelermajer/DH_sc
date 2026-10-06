_GLOBAL__I_.._.._sources_Game_Objects_Block.cpp
003888dc push     {r4, r5, r6, lr}
003888e0 ldr      r4, [pc, #0x110]
003888e4 ldr      r3, [pc, #0x110]
003888e8 ldr      r2, [pc, #0x110]
003888ec add      r4, pc, r4
003888f0 ldr      ip, [r4, r3]
003888f4 ldr      r3, [pc, #0x108]
003888f8 ldr      r2, [r4, r2]
003888fc ldr      r5, [ip]
00388900 mov      r1, #0xbf000000
00388904 mov      r0, #0x3f000000
00388908 add      r1, r1, #0x800000
0038890c add      r3, pc, r3
00388910 tst      r5, #1
00388914 str      r0, [r3, #8]
00388918 str      r1, [r2, #8]
0038891c str      r0, [r3]
00388920 str      r0, [r3, #4]
00388924 str      r1, [r2]
00388928 str      r1, [r2, #4]
0038892c beq      #0x3889c4
00388930 ldr      r3, [pc, #0xd0]
00388934 ldr      r3, [r4, r3]
00388938 ldr      r2, [r3]
0038893c tst      r2, #1
00388940 beq      #0x388990
00388944 ldr      r3, [pc, #0xc0]
00388948 ldr      r3, [r4, r3]
0038894c ldr      r2, [r3]
00388950 tst      r2, #1
00388954 beq      #0x38895c
00388958 pop      {r4, r5, r6, pc}
0038895c mov      r2, #1
00388960 str      r2, [r3]
00388964 ldr      r3, [pc, #0xa4]
00388968 ldr      r5, [r4, r3]
0038896c mov      r0, r5
00388970 bl       #0x522e2c ; _ZN7PFWorldC1Ev
00388974 ldr      r3, [pc, #0x98]
00388978 mov      r0, r5
0038897c ldr      r1, [r4, r3]
00388980 ldr      r3, [pc, #0x90]
00388984 ldr      r2, [r4, r3]
00388988 pop      {r4, r5, r6, lr}
0038898c b        #0x30e304
00388990 mov      r2, #1
00388994 str      r2, [r3]
00388998 ldr      r3, [pc, #0x7c]
0038899c ldr      r5, [r4, r3]
003889a0 mov      r0, r5
003889a4 bl       #0x32d79c ; _ZN11ApplicationC1Ev
003889a8 ldr      r3, [pc, #0x70]
003889ac mov      r0, r5
003889b0 ldr      r1, [r4, r3]
003889b4 ldr      r3, [pc, #0x5c]
003889b8 ldr      r2, [r4, r3]
003889bc bl       #0x30e304
003889c0 b        #0x388944
003889c4 mov      r3, #1
003889c8 str      r3, [ip]
003889cc ldr      r3, [pc, #0x50]
003889d0 ldr      r5, [r4, r3]
003889d4 mov      r0, r5
003889d8 bl       #0x3790a8 ; _ZN17PlayerStatManagerC1Ev
003889dc ldr      r3, [pc, #0x44]
003889e0 mov      r0, r5
003889e4 ldr      r1, [r4, r3]
003889e8 ldr      r3, [pc, #0x28]
003889ec ldr      r2, [r4, r3]
003889f0 bl       #0x30e304
003889f4 b        #0x388930
003889f8 rsbeq    ip, r0, r4, lsr #3
003889fc strdeq   r0, r1, [r0], -r4
00388a00 muleq    r0, r8, r4
00388a04 rsbeq    sb, r1, r0, lsr #27
00388a08 andeq    r0, r0, ip, lsr #31
00388a0c andeq    r0, r0, r0, ror r6
00388a10 andeq    r1, r0, r4, lsl #4
00388a14 andeq    r1, r0, r0, lsr #4
00388a18 muleq    r0, r0, r8
00388a1c strdeq   r3, r4, [r0], -r4
00388a20 andeq    r0, r0, r0, asr #17
00388a24 andeq    r2, r0, r4, lsl r7
00388a28 muleq    r0, ip, r5
