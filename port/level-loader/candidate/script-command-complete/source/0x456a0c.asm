00456a0c 10402de9 push {r4, lr}
00456a10 0010a0e3 mov r1, #0
00456a14 1000a0e3 mov r0, #0x10
00456a18 d4e6faeb bl #0x310570
00456a1c 1c409fe5 ldr r4, [pc, #0x1c]
00456a20 1c309fe5 ldr r3, [pc, #0x1c]
00456a24 0010a0e3 mov r1, #0
00456a28 04408fe0 add r4, pc, r4
00456a2c 033094e7 ldr r3, [r4, r3]
00456a30 0c1080e5 str r1, [r0, #0xc]
00456a34 083083e2 add r3, r3, #8
00456a38 003080e5 str r3, [r0]
00456a3c 1080bde8 pop {r4, pc}
00456a40 68e05300 subseq lr, r3, r8, rrx
00456a44 c41f0000 andeq r1, r0, r4, asr #31
