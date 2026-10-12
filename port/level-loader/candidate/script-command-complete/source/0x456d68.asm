00456d68 10402de9 push {r4, lr}
00456d6c 0010a0e3 mov r1, #0
00456d70 1c00a0e3 mov r0, #0x1c
00456d74 fde5faeb bl #0x310570
00456d78 20409fe5 ldr r4, [pc, #0x20]
00456d7c 20209fe5 ldr r2, [pc, #0x20]
00456d80 0010a0e3 mov r1, #0
00456d84 04408fe0 add r4, pc, r4
00456d88 022094e7 ldr r2, [r4, r2]
00456d8c 141080e5 str r1, [r0, #0x14]
00456d90 0c1080e5 str r1, [r0, #0xc]
00456d94 082082e2 add r2, r2, #8
00456d98 002080e5 str r2, [r0]
00456d9c 1080bde8 pop {r4, pc}
00456da0 0cdd5300 subseq sp, r3, ip, lsl #26
00456da4 64240000 andeq r2, r0, r4, ror #8
