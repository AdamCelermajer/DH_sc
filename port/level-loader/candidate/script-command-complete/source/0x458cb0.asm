00458cb0 10402de9 push {r4, lr}
00458cb4 0010a0e3 mov r1, #0
00458cb8 1000a0e3 mov r0, #0x10
00458cbc 2bdefaeb bl #0x310570
00458cc0 28409fe5 ldr r4, [pc, #0x28]
00458cc4 28209fe5 ldr r2, [pc, #0x28]
00458cc8 0010a0e3 mov r1, #0
00458ccc 04408fe0 add r4, pc, r4
00458cd0 022094e7 ldr r2, [r4, r2]
00458cd4 0c1080e5 str r1, [r0, #0xc]
00458cd8 0410c0e5 strb r1, [r0, #4]
00458cdc 082082e2 add r2, r2, #8
00458ce0 002080e5 str r2, [r0]
00458ce4 0020e0e3 mvn r2, #0
00458ce8 082080e5 str r2, [r0, #8]
00458cec 1080bde8 pop {r4, pc}
00458cf0 c4bd5300 subseq fp, r3, r4, asr #27
00458cf4 48480000 andeq r4, r0, r8, asr #16
