00457290 10402de9 push {r4, lr}
00457294 0010a0e3 mov r1, #0
00457298 0800a0e3 mov r0, #8
0045729c b3e4faeb bl #0x310570
004572a0 1c409fe5 ldr r4, [pc, #0x1c]
004572a4 1c309fe5 ldr r3, [pc, #0x1c]
004572a8 0010a0e3 mov r1, #0
004572ac 04408fe0 add r4, pc, r4
004572b0 033094e7 ldr r3, [r4, r3]
004572b4 041080e5 str r1, [r0, #4]
004572b8 083083e2 add r3, r3, #8
004572bc 003080e5 str r3, [r0]
004572c0 1080bde8 pop {r4, pc}
004572c4 e4d75300 subseq sp, r3, r4, ror #15
004572c8 343b0000 andeq r3, r0, r4, lsr fp
