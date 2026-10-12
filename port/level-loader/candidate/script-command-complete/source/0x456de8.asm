00456de8 10402de9 push {r4, lr}
00456dec 0010a0e3 mov r1, #0
00456df0 0c00a0e3 mov r0, #0xc
00456df4 dde5faeb bl #0x310570
00456df8 20409fe5 ldr r4, [pc, #0x20]
00456dfc 20209fe5 ldr r2, [pc, #0x20]
00456e00 0010a0e3 mov r1, #0
00456e04 04408fe0 add r4, pc, r4
00456e08 022094e7 ldr r2, [r4, r2]
00456e0c 081080e5 str r1, [r0, #8]
00456e10 041080e5 str r1, [r0, #4]
00456e14 082082e2 add r2, r2, #8
00456e18 002080e5 str r2, [r0]
00456e1c 1080bde8 pop {r4, pc}
00456e20 8cdc5300 subseq sp, r3, ip, lsl #25
00456e24 38150000 andeq r1, r0, r8, lsr r5
