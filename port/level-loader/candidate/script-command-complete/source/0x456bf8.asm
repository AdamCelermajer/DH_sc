00456bf8 10402de9 push {r4, lr}
00456bfc 0010a0e3 mov r1, #0
00456c00 1000a0e3 mov r0, #0x10
00456c04 59e6faeb bl #0x310570
00456c08 1c409fe5 ldr r4, [pc, #0x1c]
00456c0c 1c309fe5 ldr r3, [pc, #0x1c]
00456c10 0010a0e3 mov r1, #0
00456c14 04408fe0 add r4, pc, r4
00456c18 033094e7 ldr r3, [r4, r3]
00456c1c 0c1080e5 str r1, [r0, #0xc]
00456c20 083083e2 add r3, r3, #8
00456c24 003080e5 str r3, [r0]
00456c28 1080bde8 pop {r4, pc}
00456c2c 7cde5300 subseq sp, r3, ip, ror lr
00456c30 502e0000 andeq r2, r0, r0, asr lr
