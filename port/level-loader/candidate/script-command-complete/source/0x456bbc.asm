00456bbc 10402de9 push {r4, lr}
00456bc0 0010a0e3 mov r1, #0
00456bc4 2000a0e3 mov r0, #0x20
00456bc8 68e6faeb bl #0x310570
00456bcc 1c409fe5 ldr r4, [pc, #0x1c]
00456bd0 1c309fe5 ldr r3, [pc, #0x1c]
00456bd4 0010a0e3 mov r1, #0
00456bd8 04408fe0 add r4, pc, r4
00456bdc 033094e7 ldr r3, [r4, r3]
00456be0 181080e5 str r1, [r0, #0x18]
00456be4 083083e2 add r3, r3, #8
00456be8 003080e5 str r3, [r0]
00456bec 1080bde8 pop {r4, pc}
00456bf0 b8de5300 ldrheq sp, [r3], #-0xe8
00456bf4 c8410000 andeq r4, r0, r8, asr #3
