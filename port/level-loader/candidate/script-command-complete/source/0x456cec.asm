00456cec 10402de9 push {r4, lr}
00456cf0 0010a0e3 mov r1, #0
00456cf4 2000a0e3 mov r0, #0x20
00456cf8 1ce6faeb bl #0x310570
00456cfc 20409fe5 ldr r4, [pc, #0x20]
00456d00 20209fe5 ldr r2, [pc, #0x20]
00456d04 0010a0e3 mov r1, #0
00456d08 04408fe0 add r4, pc, r4
00456d0c 022094e7 ldr r2, [r4, r2]
00456d10 181080e5 str r1, [r0, #0x18]
00456d14 0c1080e5 str r1, [r0, #0xc]
00456d18 082082e2 add r2, r2, #8
00456d1c 002080e5 str r2, [r0]
00456d20 1080bde8 pop {r4, pc}
00456d24 88dd5300 subseq sp, r3, r8, lsl #27
00456d28 241b0000 andeq r1, r0, r4, lsr #22
