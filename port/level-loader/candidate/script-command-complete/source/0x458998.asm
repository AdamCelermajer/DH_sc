00458998 10402de9 push {r4, lr}
0045899c 0010a0e3 mov r1, #0
004589a0 1000a0e3 mov r0, #0x10
004589a4 f1defaeb bl #0x310570
004589a8 28409fe5 ldr r4, [pc, #0x28]
004589ac 28209fe5 ldr r2, [pc, #0x28]
004589b0 0010a0e3 mov r1, #0
004589b4 04408fe0 add r4, pc, r4
004589b8 022094e7 ldr r2, [r4, r2]
004589bc 0c1080e5 str r1, [r0, #0xc]
004589c0 0410c0e5 strb r1, [r0, #4]
004589c4 082082e2 add r2, r2, #8
004589c8 002080e5 str r2, [r0]
004589cc 0020e0e3 mvn r2, #0
004589d0 082080e5 str r2, [r0, #8]
004589d4 1080bde8 pop {r4, pc}
004589d8 dcc05300 ldrsbeq ip, [r3], #-0xc
004589dc 48400000 andeq r4, r0, r8, asr #32
