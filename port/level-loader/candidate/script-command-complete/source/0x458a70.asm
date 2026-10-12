00458a70 10402de9 push {r4, lr}
00458a74 0010a0e3 mov r1, #0
00458a78 1000a0e3 mov r0, #0x10
00458a7c bbdefaeb bl #0x310570
00458a80 28409fe5 ldr r4, [pc, #0x28]
00458a84 28209fe5 ldr r2, [pc, #0x28]
00458a88 0010a0e3 mov r1, #0
00458a8c 04408fe0 add r4, pc, r4
00458a90 022094e7 ldr r2, [r4, r2]
00458a94 0c1080e5 str r1, [r0, #0xc]
00458a98 0410c0e5 strb r1, [r0, #4]
00458a9c 082082e2 add r2, r2, #8
00458aa0 002080e5 str r2, [r0]
00458aa4 0020e0e3 mvn r2, #0
00458aa8 082080e5 str r2, [r0, #8]
00458aac 1080bde8 pop {r4, pc}
00458ab0 04c05300 subseq ip, r3, r4
00458ab4 c42f0000 andeq r2, r0, r4, asr #31
