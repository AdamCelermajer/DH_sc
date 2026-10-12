00456e28 10402de9 push {r4, lr}
00456e2c 0010a0e3 mov r1, #0
00456e30 1400a0e3 mov r0, #0x14
00456e34 cde5faeb bl #0x310570
00456e38 1c409fe5 ldr r4, [pc, #0x1c]
00456e3c 1c309fe5 ldr r3, [pc, #0x1c]
00456e40 0010a0e3 mov r1, #0
00456e44 04408fe0 add r4, pc, r4
00456e48 033094e7 ldr r3, [r4, r3]
00456e4c 101080e5 str r1, [r0, #0x10]
00456e50 083083e2 add r3, r3, #8
00456e54 003080e5 str r3, [r0]
00456e58 1080bde8 pop {r4, pc}
00456e5c 4cdc5300 subseq sp, r3, ip, asr #24
00456e60 40490000 andeq r4, r0, r0, asr #18
