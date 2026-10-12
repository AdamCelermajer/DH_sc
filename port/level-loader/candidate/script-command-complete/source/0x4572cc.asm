004572cc 10402de9 push {r4, lr}
004572d0 0010a0e3 mov r1, #0
004572d4 0800a0e3 mov r0, #8
004572d8 a4e4faeb bl #0x310570
004572dc 1c409fe5 ldr r4, [pc, #0x1c]
004572e0 1c309fe5 ldr r3, [pc, #0x1c]
004572e4 0010a0e3 mov r1, #0
004572e8 04408fe0 add r4, pc, r4
004572ec 033094e7 ldr r3, [r4, r3]
004572f0 041080e5 str r1, [r0, #4]
004572f4 083083e2 add r3, r3, #8
004572f8 003080e5 str r3, [r0]
004572fc 1080bde8 pop {r4, pc}
00457300 a8d75300 subseq sp, r3, r8, lsr #15
00457304 a0410000 andeq r4, r0, r0, lsr #3
