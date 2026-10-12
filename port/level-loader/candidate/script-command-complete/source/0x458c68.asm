00458c68 10402de9 push {r4, lr}
00458c6c 0010a0e3 mov r1, #0
00458c70 1000a0e3 mov r0, #0x10
00458c74 3ddefaeb bl #0x310570
00458c78 28409fe5 ldr r4, [pc, #0x28]
00458c7c 28209fe5 ldr r2, [pc, #0x28]
00458c80 0010a0e3 mov r1, #0
00458c84 04408fe0 add r4, pc, r4
00458c88 022094e7 ldr r2, [r4, r2]
00458c8c 0c1080e5 str r1, [r0, #0xc]
00458c90 0410c0e5 strb r1, [r0, #4]
00458c94 082082e2 add r2, r2, #8
00458c98 002080e5 str r2, [r0]
00458c9c 0020e0e3 mvn r2, #0
00458ca0 082080e5 str r2, [r0, #8]
00458ca4 1080bde8 pop {r4, pc}
00458ca8 0cbe5300 subseq fp, r3, ip, lsl #28
00458cac f03a0000 strdeq r3, r4, [r0], -r0
