004565c0 10402de9 push {r4, lr}
004565c4 0010a0e3 mov r1, #0
004565c8 0800a0e3 mov r0, #8
004565cc e7e7faeb bl #0x310570
004565d0 1c409fe5 ldr r4, [pc, #0x1c]
004565d4 1c309fe5 ldr r3, [pc, #0x1c]
004565d8 0010a0e3 mov r1, #0
004565dc 04408fe0 add r4, pc, r4
004565e0 033094e7 ldr r3, [r4, r3]
004565e4 041080e5 str r1, [r0, #4]
004565e8 083083e2 add r3, r3, #8
004565ec 003080e5 str r3, [r0]
004565f0 1080bde8 pop {r4, pc}
004565f4 b4e45300 ldrheq lr, [r3], #-0x44
004565f8 8c3a0000 andeq r3, r0, ip, lsl #21
