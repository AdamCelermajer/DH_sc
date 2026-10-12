00457a2c 10402de9 push {r4, lr}
00457a30 0010a0e3 mov r1, #0
00457a34 1000a0e3 mov r0, #0x10
00457a38 cce2faeb bl #0x310570
00457a3c 28409fe5 ldr r4, [pc, #0x28]
00457a40 28209fe5 ldr r2, [pc, #0x28]
00457a44 0010a0e3 mov r1, #0
00457a48 04408fe0 add r4, pc, r4
00457a4c 022094e7 ldr r2, [r4, r2]
00457a50 0c1080e5 str r1, [r0, #0xc]
00457a54 0410c0e5 strb r1, [r0, #4]
00457a58 082082e2 add r2, r2, #8
00457a5c 002080e5 str r2, [r0]
00457a60 0020e0e3 mvn r2, #0
00457a64 082080e5 str r2, [r0, #8]
00457a68 1080bde8 pop {r4, pc}
00457a6c 48d05300 subseq sp, r3, r8, asr #32
00457a70 dc0e0000 ldrdeq r0, r1, [r0], -ip
