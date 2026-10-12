00456a84 10402de9 push {r4, lr}
00456a88 0010a0e3 mov r1, #0
00456a8c 1400a0e3 mov r0, #0x14
00456a90 b6e6faeb bl #0x310570
00456a94 1c409fe5 ldr r4, [pc, #0x1c]
00456a98 1c309fe5 ldr r3, [pc, #0x1c]
00456a9c 0010a0e3 mov r1, #0
00456aa0 04408fe0 add r4, pc, r4
00456aa4 033094e7 ldr r3, [r4, r3]
00456aa8 101080e5 str r1, [r0, #0x10]
00456aac 083083e2 add r3, r3, #8
00456ab0 003080e5 str r3, [r0]
00456ab4 1080bde8 pop {r4, pc}
00456ab8 f0df5300 ldrsheq sp, [r3], #-0xf0
00456abc b0120000 strheq r1, [r0], -r0
