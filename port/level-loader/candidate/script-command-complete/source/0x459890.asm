00459890 0c2090e5 ldr r2, [r0, #0xc]
00459894 10309fe5 ldr r3, [pc, #0x10]
00459898 081092e5 ldr r1, [r2, #8]
0045989c 0c209fe5 ldr r2, [pc, #0xc]
004598a0 03308fe0 add r3, pc, r3
004598a4 020093e7 ldr r0, [r3, r2]
004598a8 6ceb00ea b #0x494660
004598ac f0b15300 ldrsheq fp, [r3], #-0x10
004598b0 081b0000 andeq r1, r0, r8, lsl #22
