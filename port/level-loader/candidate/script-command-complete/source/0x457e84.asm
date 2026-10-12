00457e84 10402de9 push {r4, lr}
00457e88 0010a0e3 mov r1, #0
00457e8c 1000a0e3 mov r0, #0x10
00457e90 b6e1faeb bl #0x310570
00457e94 28409fe5 ldr r4, [pc, #0x28]
00457e98 28209fe5 ldr r2, [pc, #0x28]
00457e9c 0010a0e3 mov r1, #0
00457ea0 04408fe0 add r4, pc, r4
00457ea4 022094e7 ldr r2, [r4, r2]
00457ea8 0c1080e5 str r1, [r0, #0xc]
00457eac 0410c0e5 strb r1, [r0, #4]
00457eb0 082082e2 add r2, r2, #8
00457eb4 002080e5 str r2, [r0]
00457eb8 0020e0e3 mvn r2, #0
00457ebc 082080e5 str r2, [r0, #8]
00457ec0 1080bde8 pop {r4, pc}
00457ec4 f0cb5300 ldrsheq ip, [r3], #-0xb0
00457ec8 541b0000 andeq r1, r0, r4, asr fp
