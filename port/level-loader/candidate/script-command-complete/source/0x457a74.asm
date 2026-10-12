00457a74 10402de9 push {r4, lr}
00457a78 0010a0e3 mov r1, #0
00457a7c 1000a0e3 mov r0, #0x10
00457a80 bae2faeb bl #0x310570
00457a84 28409fe5 ldr r4, [pc, #0x28]
00457a88 28209fe5 ldr r2, [pc, #0x28]
00457a8c 0010a0e3 mov r1, #0
00457a90 04408fe0 add r4, pc, r4
00457a94 022094e7 ldr r2, [r4, r2]
00457a98 0c1080e5 str r1, [r0, #0xc]
00457a9c 0410c0e5 strb r1, [r0, #4]
00457aa0 082082e2 add r2, r2, #8
00457aa4 002080e5 str r2, [r0]
00457aa8 0020e0e3 mvn r2, #0
00457aac 082080e5 str r2, [r0, #8]
00457ab0 1080bde8 pop {r4, pc}
00457ab4 00d05300 subseq sp, r3, r0
00457ab8 60190000 andeq r1, r0, r0, ror #18
