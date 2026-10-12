004569d0 10402de9 push {r4, lr}
004569d4 0010a0e3 mov r1, #0
004569d8 1400a0e3 mov r0, #0x14
004569dc e3e6faeb bl #0x310570
004569e0 1c409fe5 ldr r4, [pc, #0x1c]
004569e4 1c309fe5 ldr r3, [pc, #0x1c]
004569e8 0010a0e3 mov r1, #0
004569ec 04408fe0 add r4, pc, r4
004569f0 033094e7 ldr r3, [r4, r3]
004569f4 101080e5 str r1, [r0, #0x10]
004569f8 083083e2 add r3, r3, #8
004569fc 003080e5 str r3, [r0]
00456a00 1080bde8 pop {r4, pc}
00456a04 a4e05300 subseq lr, r3, r4, lsr #1
00456a08 603c0000 andeq r3, r0, r0, ror #24
