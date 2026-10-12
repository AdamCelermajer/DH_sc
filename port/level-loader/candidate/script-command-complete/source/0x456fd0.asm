00456fd0 10402de9 push {r4, lr}
00456fd4 0010a0e3 mov r1, #0
00456fd8 0c00a0e3 mov r0, #0xc
00456fdc 63e5faeb bl #0x310570
00456fe0 20409fe5 ldr r4, [pc, #0x20]
00456fe4 20209fe5 ldr r2, [pc, #0x20]
00456fe8 0010a0e3 mov r1, #0
00456fec 04408fe0 add r4, pc, r4
00456ff0 022094e7 ldr r2, [r4, r2]
00456ff4 081080e5 str r1, [r0, #8]
00456ff8 041080e5 str r1, [r0, #4]
00456ffc 082082e2 add r2, r2, #8
00457000 002080e5 str r2, [r0]
00457004 1080bde8 pop {r4, pc}
00457008 a4da5300 subseq sp, r3, r4, lsr #21
0045700c c01e0000 andeq r1, r0, r0, asr #29
