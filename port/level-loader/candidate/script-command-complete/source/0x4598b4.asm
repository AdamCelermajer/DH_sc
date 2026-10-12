004598b4 0c2090e5 ldr r2, [r0, #0xc]
004598b8 10309fe5 ldr r3, [pc, #0x10]
004598bc 081092e5 ldr r1, [r2, #8]
004598c0 0c209fe5 ldr r2, [pc, #0xc]
004598c4 03308fe0 add r3, pc, r3
004598c8 020093e7 ldr r0, [r3, r2]
004598cc c5f300ea b #0x4967e8
004598d0 ccb15300 subseq fp, r3, ip, asr #3
004598d4 081b0000 andeq r1, r0, r8, lsl #22
