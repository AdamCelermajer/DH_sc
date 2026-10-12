00456a48 10402de9 push {r4, lr}
00456a4c 0010a0e3 mov r1, #0
00456a50 1000a0e3 mov r0, #0x10
00456a54 c5e6faeb bl #0x310570
00456a58 1c409fe5 ldr r4, [pc, #0x1c]
00456a5c 1c309fe5 ldr r3, [pc, #0x1c]
00456a60 0010a0e3 mov r1, #0
00456a64 04408fe0 add r4, pc, r4
00456a68 033094e7 ldr r3, [r4, r3]
00456a6c 0c1080e5 str r1, [r0, #0xc]
00456a70 083083e2 add r3, r3, #8
00456a74 003080e5 str r3, [r0]
00456a78 1080bde8 pop {r4, pc}
00456a7c 2ce05300 subseq lr, r3, ip, lsr #32
00456a80 20160000 andeq r1, r0, r0, lsr #12
