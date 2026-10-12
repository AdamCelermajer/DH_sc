004563d0 10402de9 push {r4, lr}
004563d4 0010a0e3 mov r1, #0
004563d8 1400a0e3 mov r0, #0x14
004563dc 63e8faeb bl #0x310570
004563e0 1c409fe5 ldr r4, [pc, #0x1c]
004563e4 1c309fe5 ldr r3, [pc, #0x1c]
004563e8 0010a0e3 mov r1, #0
004563ec 04408fe0 add r4, pc, r4
004563f0 033094e7 ldr r3, [r4, r3]
004563f4 0c1080e5 str r1, [r0, #0xc]
004563f8 083083e2 add r3, r3, #8
004563fc 003080e5 str r3, [r0]
00456400 1080bde8 pop {r4, pc}
00456404 a4e65300 subseq lr, r3, r4, lsr #13
00456408 683c0000 andeq r3, r0, r8, ror #24
