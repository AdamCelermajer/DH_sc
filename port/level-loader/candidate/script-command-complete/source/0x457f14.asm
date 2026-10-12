00457f14 10402de9 push {r4, lr}
00457f18 0010a0e3 mov r1, #0
00457f1c 1000a0e3 mov r0, #0x10
00457f20 92e1faeb bl #0x310570
00457f24 28409fe5 ldr r4, [pc, #0x28]
00457f28 28209fe5 ldr r2, [pc, #0x28]
00457f2c 0010a0e3 mov r1, #0
00457f30 04408fe0 add r4, pc, r4
00457f34 022094e7 ldr r2, [r4, r2]
00457f38 0c1080e5 str r1, [r0, #0xc]
00457f3c 0410c0e5 strb r1, [r0, #4]
00457f40 082082e2 add r2, r2, #8
00457f44 002080e5 str r2, [r0]
00457f48 0020e0e3 mvn r2, #0
00457f4c 082080e5 str r2, [r0, #8]
00457f50 1080bde8 pop {r4, pc}
00457f54 60cb5300 subseq ip, r3, r0, ror #22
00457f58 c42c0000 andeq r2, r0, r4, asr #25
