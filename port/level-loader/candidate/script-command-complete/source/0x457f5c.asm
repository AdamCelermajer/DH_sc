00457f5c 10402de9 push {r4, lr}
00457f60 0010a0e3 mov r1, #0
00457f64 1000a0e3 mov r0, #0x10
00457f68 80e1faeb bl #0x310570
00457f6c 28409fe5 ldr r4, [pc, #0x28]
00457f70 28209fe5 ldr r2, [pc, #0x28]
00457f74 0010a0e3 mov r1, #0
00457f78 04408fe0 add r4, pc, r4
00457f7c 022094e7 ldr r2, [r4, r2]
00457f80 0c1080e5 str r1, [r0, #0xc]
00457f84 0410c0e5 strb r1, [r0, #4]
00457f88 082082e2 add r2, r2, #8
00457f8c 002080e5 str r2, [r0]
00457f90 0020e0e3 mvn r2, #0
00457f94 082080e5 str r2, [r0, #8]
00457f98 1080bde8 pop {r4, pc}
00457f9c 18cb5300 subseq ip, r3, r8, lsl fp
00457fa0 70350000 andeq r3, r0, r0, ror r5
