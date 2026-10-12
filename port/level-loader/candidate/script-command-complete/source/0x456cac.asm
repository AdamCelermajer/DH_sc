00456cac 10402de9 push {r4, lr}
00456cb0 0010a0e3 mov r1, #0
00456cb4 1800a0e3 mov r0, #0x18
00456cb8 2ce6faeb bl #0x310570
00456cbc 20409fe5 ldr r4, [pc, #0x20]
00456cc0 20209fe5 ldr r2, [pc, #0x20]
00456cc4 0010a0e3 mov r1, #0
00456cc8 04408fe0 add r4, pc, r4
00456ccc 022094e7 ldr r2, [r4, r2]
00456cd0 141080e5 str r1, [r0, #0x14]
00456cd4 0c1080e5 str r1, [r0, #0xc]
00456cd8 082082e2 add r2, r2, #8
00456cdc 002080e5 str r2, [r0]
00456ce0 1080bde8 pop {r4, pc}
00456ce4 c8dd5300 subseq sp, r3, r8, asr #27
00456ce8 34290000 andeq r2, r0, r4, lsr sb
