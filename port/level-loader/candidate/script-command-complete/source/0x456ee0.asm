00456ee0 10402de9 push {r4, lr}
00456ee4 0010a0e3 mov r1, #0
00456ee8 1400a0e3 mov r0, #0x14
00456eec 9fe5faeb bl #0x310570
00456ef0 1c409fe5 ldr r4, [pc, #0x1c]
00456ef4 1c309fe5 ldr r3, [pc, #0x1c]
00456ef8 0010a0e3 mov r1, #0
00456efc 04408fe0 add r4, pc, r4
00456f00 033094e7 ldr r3, [r4, r3]
00456f04 101080e5 str r1, [r0, #0x10]
00456f08 083083e2 add r3, r3, #8
00456f0c 003080e5 str r3, [r0]
00456f10 1080bde8 pop {r4, pc}
