00457b94 10402de9 push {r4, lr}
00457b98 0010a0e3 mov r1, #0
00457b9c 1000a0e3 mov r0, #0x10
00457ba0 72e2faeb bl #0x310570
00457ba4 28409fe5 ldr r4, [pc, #0x28]
00457ba8 28209fe5 ldr r2, [pc, #0x28]
00457bac 0010a0e3 mov r1, #0
00457bb0 04408fe0 add r4, pc, r4
00457bb4 022094e7 ldr r2, [r4, r2]
00457bb8 0c1080e5 str r1, [r0, #0xc]
00457bbc 0410c0e5 strb r1, [r0, #4]
00457bc0 082082e2 add r2, r2, #8
00457bc4 002080e5 str r2, [r0]
00457bc8 0020e0e3 mvn r2, #0
00457bcc 082080e5 str r2, [r0, #8]
00457bd0 1080bde8 pop {r4, pc}
00457bd4 e0ce5300 subseq ip, r3, r0, ror #29
00457bd8 3c3a0000 andeq r3, r0, ip, lsr sl
