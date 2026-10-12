004577a4 10402de9 push {r4, lr}
004577a8 0010a0e3 mov r1, #0
004577ac 1000a0e3 mov r0, #0x10
004577b0 6ee3faeb bl #0x310570
004577b4 28409fe5 ldr r4, [pc, #0x28]
004577b8 28209fe5 ldr r2, [pc, #0x28]
004577bc 0010a0e3 mov r1, #0
004577c0 04408fe0 add r4, pc, r4
004577c4 022094e7 ldr r2, [r4, r2]
004577c8 0c1080e5 str r1, [r0, #0xc]
004577cc 0410c0e5 strb r1, [r0, #4]
004577d0 082082e2 add r2, r2, #8
004577d4 002080e5 str r2, [r0]
004577d8 0020e0e3 mvn r2, #0
004577dc 082080e5 str r2, [r0, #8]
004577e0 1080bde8 pop {r4, pc}
004577e4 d0d25300 ldrsbeq sp, [r3], #-0x20
004577e8 d42e0000 ldrdeq r2, r3, [r0], -r4
