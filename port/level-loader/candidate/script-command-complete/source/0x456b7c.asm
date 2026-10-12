00456b7c 10402de9 push {r4, lr}
00456b80 0010a0e3 mov r1, #0
00456b84 1800a0e3 mov r0, #0x18
00456b88 78e6faeb bl #0x310570
00456b8c 20409fe5 ldr r4, [pc, #0x20]
00456b90 20209fe5 ldr r2, [pc, #0x20]
00456b94 0010a0e3 mov r1, #0
00456b98 04408fe0 add r4, pc, r4
00456b9c 022094e7 ldr r2, [r4, r2]
00456ba0 141080e5 str r1, [r0, #0x14]
00456ba4 0c1080e5 str r1, [r0, #0xc]
00456ba8 082082e2 add r2, r2, #8
00456bac 002080e5 str r2, [r0]
00456bb0 1080bde8 pop {r4, pc}
00456bb4 f8de5300 ldrsheq sp, [r3], #-0xe8
00456bb8 30080000 andeq r0, r0, r0, lsr r8
