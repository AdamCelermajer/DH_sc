00456afc 10402de9 push {r4, lr}
00456b00 0010a0e3 mov r1, #0
00456b04 1800a0e3 mov r0, #0x18
00456b08 98e6faeb bl #0x310570
00456b0c 20409fe5 ldr r4, [pc, #0x20]
00456b10 20209fe5 ldr r2, [pc, #0x20]
00456b14 0010a0e3 mov r1, #0
00456b18 04408fe0 add r4, pc, r4
00456b1c 022094e7 ldr r2, [r4, r2]
00456b20 141080e5 str r1, [r0, #0x14]
00456b24 0c1080e5 str r1, [r0, #0xc]
00456b28 082082e2 add r2, r2, #8
00456b2c 002080e5 str r2, [r0]
00456b30 1080bde8 pop {r4, pc}
00456b34 78df5300 subseq sp, r3, r8, ror pc
00456b38 00170000 andeq r1, r0, r0, lsl #14
