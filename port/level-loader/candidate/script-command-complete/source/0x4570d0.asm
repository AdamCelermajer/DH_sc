004570d0 10402de9 push {r4, lr}
004570d4 0010a0e3 mov r1, #0
004570d8 1000a0e3 mov r0, #0x10
004570dc 23e5faeb bl #0x310570
004570e0 1c409fe5 ldr r4, [pc, #0x1c]
004570e4 1c309fe5 ldr r3, [pc, #0x1c]
004570e8 0010a0e3 mov r1, #0
004570ec 04408fe0 add r4, pc, r4
004570f0 033094e7 ldr r3, [r4, r3]
004570f4 0c1080e5 str r1, [r0, #0xc]
004570f8 083083e2 add r3, r3, #8
004570fc 003080e5 str r3, [r0]
00457100 1080bde8 pop {r4, pc}
00457104 a4d95300 subseq sp, r3, r4, lsr #19
00457108 88230000 andeq r2, r0, r8, lsl #7
