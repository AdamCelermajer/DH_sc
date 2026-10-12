00456f1c 10402de9 push {r4, lr}
00456f20 0010a0e3 mov r1, #0
00456f24 1000a0e3 mov r0, #0x10
00456f28 90e5faeb bl #0x310570
00456f2c 1c409fe5 ldr r4, [pc, #0x1c]
00456f30 1c309fe5 ldr r3, [pc, #0x1c]
00456f34 0010a0e3 mov r1, #0
00456f38 04408fe0 add r4, pc, r4
00456f3c 033094e7 ldr r3, [r4, r3]
00456f40 0c1080e5 str r1, [r0, #0xc]
00456f44 083083e2 add r3, r3, #8
00456f48 003080e5 str r3, [r0]
00456f4c 1080bde8 pop {r4, pc}
00456f50 58db5300 subseq sp, r3, r8, asr fp
00456f54 b02d0000 strheq r2, [r0], -r0
