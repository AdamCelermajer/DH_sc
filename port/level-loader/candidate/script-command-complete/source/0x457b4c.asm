00457b4c 10402de9 push {r4, lr}
00457b50 0010a0e3 mov r1, #0
00457b54 1000a0e3 mov r0, #0x10
00457b58 84e2faeb bl #0x310570
00457b5c 28409fe5 ldr r4, [pc, #0x28]
00457b60 28209fe5 ldr r2, [pc, #0x28]
00457b64 0010a0e3 mov r1, #0
00457b68 04408fe0 add r4, pc, r4
00457b6c 022094e7 ldr r2, [r4, r2]
00457b70 0c1080e5 str r1, [r0, #0xc]
00457b74 0410c0e5 strb r1, [r0, #4]
00457b78 082082e2 add r2, r2, #8
00457b7c 002080e5 str r2, [r0]
00457b80 0020e0e3 mvn r2, #0
00457b84 082080e5 str r2, [r0, #8]
00457b88 1080bde8 pop {r4, pc}
00457b8c 28cf5300 subseq ip, r3, r8, lsr #30
00457b90 ac3d0000 andeq r3, r0, ip, lsr #27
