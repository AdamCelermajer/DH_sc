00456f94 10402de9 push {r4, lr}
00456f98 0010a0e3 mov r1, #0
00456f9c 1400a0e3 mov r0, #0x14
00456fa0 72e5faeb bl #0x310570
00456fa4 1c409fe5 ldr r4, [pc, #0x1c]
00456fa8 1c309fe5 ldr r3, [pc, #0x1c]
00456fac 0010a0e3 mov r1, #0
00456fb0 04408fe0 add r4, pc, r4
00456fb4 033094e7 ldr r3, [r4, r3]
00456fb8 101080e5 str r1, [r0, #0x10]
00456fbc 083083e2 add r3, r3, #8
00456fc0 003080e5 str r3, [r0]
00456fc4 1080bde8 pop {r4, pc}
00456fc8 e0da5300 subseq sp, r3, r0, ror #21
00456fcc f8250000 strdeq r2, r3, [r0], -r8
