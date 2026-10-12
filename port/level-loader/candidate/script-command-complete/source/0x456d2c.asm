00456d2c 10402de9 push {r4, lr}
00456d30 0010a0e3 mov r1, #0
00456d34 1000a0e3 mov r0, #0x10
00456d38 0ce6faeb bl #0x310570
00456d3c 1c409fe5 ldr r4, [pc, #0x1c]
00456d40 1c309fe5 ldr r3, [pc, #0x1c]
00456d44 0010a0e3 mov r1, #0
00456d48 04408fe0 add r4, pc, r4
00456d4c 033094e7 ldr r3, [r4, r3]
00456d50 0c1080e5 str r1, [r0, #0xc]
00456d54 083083e2 add r3, r3, #8
00456d58 003080e5 str r3, [r0]
00456d5c 1080bde8 pop {r4, pc}
00456d60 48dd5300 subseq sp, r3, r8, asr #26
00456d64 a84a0000 andeq r4, r0, r8, lsr #21
