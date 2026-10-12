00456e64 10402de9 push {r4, lr}
00456e68 0010a0e3 mov r1, #0
00456e6c 0800a0e3 mov r0, #8
00456e70 bee5faeb bl #0x310570
00456e74 1c409fe5 ldr r4, [pc, #0x1c]
00456e78 1c309fe5 ldr r3, [pc, #0x1c]
00456e7c 0010a0e3 mov r1, #0
00456e80 04408fe0 add r4, pc, r4
00456e84 033094e7 ldr r3, [r4, r3]
00456e88 041080e5 str r1, [r0, #4]
00456e8c 083083e2 add r3, r3, #8
00456e90 003080e5 str r3, [r0]
00456e94 1080bde8 pop {r4, pc}
00456e98 10dc5300 subseq sp, r3, r0, lsl ip
00456e9c 284c0000 andeq r4, r0, r8, lsr #24
