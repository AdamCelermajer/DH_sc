004598d8 18309fe5 ldr r3, [pc, #0x18]
004598dc 18209fe5 ldr r2, [pc, #0x18]
004598e0 10402de9 push {r4, lr}
004598e4 03308fe0 add r3, pc, r3
004598e8 020093e7 ldr r0, [r3, r2]
004598ec 2817fbeb bl #0x31f594
004598f0 1040bde8 pop {r4, lr}
004598f4 5456feea b #0x3ef24c
004598f8 acb15300 subseq fp, r3, ip, lsr #3
004598fc f4370000 strdeq r3, r4, [r0], -r4
