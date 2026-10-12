00457fa4 10402de9 push {r4, lr}
00457fa8 0010a0e3 mov r1, #0
00457fac 1000a0e3 mov r0, #0x10
00457fb0 6ee1faeb bl #0x310570
00457fb4 28409fe5 ldr r4, [pc, #0x28]
00457fb8 28209fe5 ldr r2, [pc, #0x28]
00457fbc 0010a0e3 mov r1, #0
00457fc0 04408fe0 add r4, pc, r4
00457fc4 022094e7 ldr r2, [r4, r2]
00457fc8 0c1080e5 str r1, [r0, #0xc]
00457fcc 0410c0e5 strb r1, [r0, #4]
00457fd0 082082e2 add r2, r2, #8
00457fd4 002080e5 str r2, [r0]
00457fd8 0020e0e3 mvn r2, #0
00457fdc 082080e5 str r2, [r0, #8]
00457fe0 1080bde8 pop {r4, pc}
00457fe4 d0ca5300 ldrsbeq ip, [r3], #-0xa0
00457fe8 d8090000 ldrdeq r0, r1, [r0], -r8
