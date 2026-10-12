004587a0 10402de9 push {r4, lr}
004587a4 0010a0e3 mov r1, #0
004587a8 1000a0e3 mov r0, #0x10
004587ac 6fdffaeb bl #0x310570
004587b0 28409fe5 ldr r4, [pc, #0x28]
004587b4 28209fe5 ldr r2, [pc, #0x28]
004587b8 0010a0e3 mov r1, #0
004587bc 04408fe0 add r4, pc, r4
004587c0 022094e7 ldr r2, [r4, r2]
004587c4 0c1080e5 str r1, [r0, #0xc]
004587c8 0410c0e5 strb r1, [r0, #4]
004587cc 082082e2 add r2, r2, #8
004587d0 002080e5 str r2, [r0]
004587d4 0020e0e3 mvn r2, #0
004587d8 082080e5 str r2, [r0, #8]
004587dc 1080bde8 pop {r4, pc}
004587e0 d4c25300 ldrsbeq ip, [r3], #-0x24
004587e4 b40f0000 strheq r0, [r0], -r4
