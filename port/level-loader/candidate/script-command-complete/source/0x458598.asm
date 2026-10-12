00458598 10402de9 push {r4, lr}
0045859c 0010a0e3 mov r1, #0
004585a0 1000a0e3 mov r0, #0x10
004585a4 f1dffaeb bl #0x310570
004585a8 28409fe5 ldr r4, [pc, #0x28]
004585ac 28209fe5 ldr r2, [pc, #0x28]
004585b0 0010a0e3 mov r1, #0
004585b4 04408fe0 add r4, pc, r4
004585b8 022094e7 ldr r2, [r4, r2]
004585bc 0c1080e5 str r1, [r0, #0xc]
004585c0 0410c0e5 strb r1, [r0, #4]
004585c4 082082e2 add r2, r2, #8
004585c8 002080e5 str r2, [r0]
004585cc 0020e0e3 mvn r2, #0
004585d0 082080e5 str r2, [r0, #8]
004585d4 1080bde8 pop {r4, pc}
004585d8 dcc45300 ldrsbeq ip, [r3], #-0x4c
004585dc 60160000 andeq r1, r0, r0, ror #12
