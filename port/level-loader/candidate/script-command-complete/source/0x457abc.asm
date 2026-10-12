00457abc 10402de9 push {r4, lr}
00457ac0 0010a0e3 mov r1, #0
00457ac4 1000a0e3 mov r0, #0x10
00457ac8 a8e2faeb bl #0x310570
00457acc 28409fe5 ldr r4, [pc, #0x28]
00457ad0 28209fe5 ldr r2, [pc, #0x28]
00457ad4 0010a0e3 mov r1, #0
00457ad8 04408fe0 add r4, pc, r4
00457adc 022094e7 ldr r2, [r4, r2]
00457ae0 0c1080e5 str r1, [r0, #0xc]
00457ae4 0410c0e5 strb r1, [r0, #4]
00457ae8 082082e2 add r2, r2, #8
00457aec 002080e5 str r2, [r0]
00457af0 0020e0e3 mvn r2, #0
00457af4 082080e5 str r2, [r0, #8]
00457af8 1080bde8 pop {r4, pc}
00457afc b8cf5300 ldrheq ip, [r3], #-0xf8
00457b00 d8480000 ldrdeq r4, r5, [r0], -r8
