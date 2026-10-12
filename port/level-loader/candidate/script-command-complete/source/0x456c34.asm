00456c34 10402de9 push {r4, lr}
00456c38 0010a0e3 mov r1, #0
00456c3c 1000a0e3 mov r0, #0x10
00456c40 4ae6faeb bl #0x310570
00456c44 1c409fe5 ldr r4, [pc, #0x1c]
00456c48 1c309fe5 ldr r3, [pc, #0x1c]
00456c4c 0010a0e3 mov r1, #0
00456c50 04408fe0 add r4, pc, r4
00456c54 033094e7 ldr r3, [r4, r3]
00456c58 0c1080e5 str r1, [r0, #0xc]
00456c5c 083083e2 add r3, r3, #8
00456c60 003080e5 str r3, [r0]
00456c64 1080bde8 pop {r4, pc}
00456c68 40de5300 subseq sp, r3, r0, asr #28
00456c6c 840c0000 andeq r0, r0, r4, lsl #25
