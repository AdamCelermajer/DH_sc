00458a28 10402de9 push {r4, lr}
00458a2c 0010a0e3 mov r1, #0
00458a30 1000a0e3 mov r0, #0x10
00458a34 cddefaeb bl #0x310570
00458a38 28409fe5 ldr r4, [pc, #0x28]
00458a3c 28209fe5 ldr r2, [pc, #0x28]
00458a40 0010a0e3 mov r1, #0
00458a44 04408fe0 add r4, pc, r4
00458a48 022094e7 ldr r2, [r4, r2]
00458a4c 0c1080e5 str r1, [r0, #0xc]
00458a50 0410c0e5 strb r1, [r0, #4]
00458a54 082082e2 add r2, r2, #8
00458a58 002080e5 str r2, [r0]
00458a5c 0020e0e3 mvn r2, #0
00458a60 082080e5 str r2, [r0, #8]
00458a64 1080bde8 pop {r4, pc}
00458a68 4cc05300 subseq ip, r3, ip, asr #32
00458a6c f0490000 strdeq r4, r5, [r0], -r0
