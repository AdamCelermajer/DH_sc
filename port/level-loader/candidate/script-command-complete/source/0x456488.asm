00456488 10402de9 push {r4, lr}
0045648c 0010a0e3 mov r1, #0
00456490 0800a0e3 mov r0, #8
00456494 35e8faeb bl #0x310570
00456498 1c409fe5 ldr r4, [pc, #0x1c]
0045649c 1c309fe5 ldr r3, [pc, #0x1c]
004564a0 0010a0e3 mov r1, #0
004564a4 04408fe0 add r4, pc, r4
004564a8 033094e7 ldr r3, [r4, r3]
004564ac 041080e5 str r1, [r0, #4]
004564b0 083083e2 add r3, r3, #8
004564b4 003080e5 str r3, [r0]
004564b8 1080bde8 pop {r4, pc}
004564bc ece55300 subseq lr, r3, ip, ror #11
004564c0 000c0000 andeq r0, r0, r0, lsl #24
