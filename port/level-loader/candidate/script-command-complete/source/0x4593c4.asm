004593c4 0c2090e5 ldr r2, [r0, #0xc]
004593c8 1c309fe5 ldr r3, [pc, #0x1c]
004593cc 000052e3 cmp r2, #0
004593d0 03308fe0 add r3, pc, r3
004593d4 1eff2f01 bxeq lr
004593d8 081092e5 ldr r1, [r2, #8]
004593dc 0c209fe5 ldr r2, [pc, #0xc]
004593e0 023093e7 ldr r3, [r3, r2]
004593e4 000093e5 ldr r0, [r3]
004593e8 f29ffcea b #0x3813b8
004593ec c0b65300 subseq fp, r3, r0, asr #13
004593f0 701d0000 andeq r1, r0, r0, ror sp
