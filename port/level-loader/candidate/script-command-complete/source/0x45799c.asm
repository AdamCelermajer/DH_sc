0045799c 10402de9 push {r4, lr}
004579a0 0010a0e3 mov r1, #0
004579a4 1000a0e3 mov r0, #0x10
004579a8 f0e2faeb bl #0x310570
004579ac 28409fe5 ldr r4, [pc, #0x28]
004579b0 28209fe5 ldr r2, [pc, #0x28]
004579b4 0010a0e3 mov r1, #0
004579b8 04408fe0 add r4, pc, r4
004579bc 022094e7 ldr r2, [r4, r2]
004579c0 0c1080e5 str r1, [r0, #0xc]
004579c4 0410c0e5 strb r1, [r0, #4]
004579c8 082082e2 add r2, r2, #8
004579cc 002080e5 str r2, [r0]
004579d0 0020e0e3 mvn r2, #0
004579d4 082080e5 str r2, [r0, #8]
004579d8 1080bde8 pop {r4, pc}
004579dc d8d05300 ldrsbeq sp, [r3], #-8
004579e0 f8400000 strdeq r4, r5, [r0], -r8
