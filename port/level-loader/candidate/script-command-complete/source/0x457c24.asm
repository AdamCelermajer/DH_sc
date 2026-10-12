00457c24 10402de9 push {r4, lr}
00457c28 0010a0e3 mov r1, #0
00457c2c 1000a0e3 mov r0, #0x10
00457c30 4ee2faeb bl #0x310570
00457c34 28409fe5 ldr r4, [pc, #0x28]
00457c38 28209fe5 ldr r2, [pc, #0x28]
00457c3c 0010a0e3 mov r1, #0
00457c40 04408fe0 add r4, pc, r4
00457c44 022094e7 ldr r2, [r4, r2]
00457c48 0c1080e5 str r1, [r0, #0xc]
00457c4c 0410c0e5 strb r1, [r0, #4]
00457c50 082082e2 add r2, r2, #8
00457c54 002080e5 str r2, [r0]
00457c58 0020e0e3 mvn r2, #0
00457c5c 082080e5 str r2, [r0, #8]
00457c60 1080bde8 pop {r4, pc}
00457c64 50ce5300 subseq ip, r3, r0, asr lr
00457c68 c0450000 andeq r4, r0, r0, asr #11
