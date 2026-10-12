00456da8 10402de9 push {r4, lr}
00456dac 0010a0e3 mov r1, #0
00456db0 1800a0e3 mov r0, #0x18
00456db4 ede5faeb bl #0x310570
00456db8 20409fe5 ldr r4, [pc, #0x20]
00456dbc 20209fe5 ldr r2, [pc, #0x20]
00456dc0 0010a0e3 mov r1, #0
00456dc4 04408fe0 add r4, pc, r4
00456dc8 022094e7 ldr r2, [r4, r2]
00456dcc 141080e5 str r1, [r0, #0x14]
00456dd0 0c1080e5 str r1, [r0, #0xc]
00456dd4 082082e2 add r2, r2, #8
00456dd8 002080e5 str r2, [r0]
00456ddc 1080bde8 pop {r4, pc}
00456de0 ccdc5300 subseq sp, r3, ip, asr #25
00456de4 04360000 andeq r3, r0, r4, lsl #12
