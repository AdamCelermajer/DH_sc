00456394 10402de9 push {r4, lr}
00456398 0010a0e3 mov r1, #0
0045639c 0800a0e3 mov r0, #8
004563a0 72e8faeb bl #0x310570
004563a4 1c409fe5 ldr r4, [pc, #0x1c]
004563a8 1c309fe5 ldr r3, [pc, #0x1c]
004563ac 0010a0e3 mov r1, #0
004563b0 04408fe0 add r4, pc, r4
004563b4 033094e7 ldr r3, [r4, r3]
004563b8 041080e5 str r1, [r0, #4]
004563bc 083083e2 add r3, r3, #8
004563c0 003080e5 str r3, [r0]
004563c4 1080bde8 pop {r4, pc}
004563c8 e0e65300 subseq lr, r3, r0, ror #13
004563cc 6c060000 andeq r0, r0, ip, ror #12
