00457c6c 10402de9 push {r4, lr}
00457c70 0010a0e3 mov r1, #0
00457c74 1000a0e3 mov r0, #0x10
00457c78 3ce2faeb bl #0x310570
00457c7c 28409fe5 ldr r4, [pc, #0x28]
00457c80 28209fe5 ldr r2, [pc, #0x28]
00457c84 0010a0e3 mov r1, #0
00457c88 04408fe0 add r4, pc, r4
00457c8c 022094e7 ldr r2, [r4, r2]
00457c90 0c1080e5 str r1, [r0, #0xc]
00457c94 0410c0e5 strb r1, [r0, #4]
00457c98 082082e2 add r2, r2, #8
00457c9c 002080e5 str r2, [r0]
00457ca0 0020e0e3 mvn r2, #0
00457ca4 082080e5 str r2, [r0, #8]
00457ca8 1080bde8 pop {r4, pc}
00457cac 08ce5300 subseq ip, r3, r8, lsl #28
00457cb0 bc170000 strheq r1, [r0], -ip
