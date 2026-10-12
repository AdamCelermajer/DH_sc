00456ac0 10402de9 push {r4, lr}
00456ac4 0010a0e3 mov r1, #0
00456ac8 1000a0e3 mov r0, #0x10
00456acc a7e6faeb bl #0x310570
00456ad0 1c409fe5 ldr r4, [pc, #0x1c]
00456ad4 1c309fe5 ldr r3, [pc, #0x1c]
00456ad8 0010a0e3 mov r1, #0
00456adc 04408fe0 add r4, pc, r4
00456ae0 033094e7 ldr r3, [r4, r3]
00456ae4 0c1080e5 str r1, [r0, #0xc]
00456ae8 083083e2 add r3, r3, #8
00456aec 003080e5 str r3, [r0]
00456af0 1080bde8 pop {r4, pc}
00456af4 b4df5300 ldrheq sp, [r3], #-0xf4
00456af8 84120000 andeq r1, r0, r4, lsl #5
