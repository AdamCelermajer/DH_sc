0045819c 10402de9 push {r4, lr}
004581a0 0010a0e3 mov r1, #0
004581a4 1000a0e3 mov r0, #0x10
004581a8 f0e0faeb bl #0x310570
004581ac 28409fe5 ldr r4, [pc, #0x28]
004581b0 28209fe5 ldr r2, [pc, #0x28]
004581b4 0010a0e3 mov r1, #0
004581b8 04408fe0 add r4, pc, r4
004581bc 022094e7 ldr r2, [r4, r2]
004581c0 0c1080e5 str r1, [r0, #0xc]
004581c4 0410c0e5 strb r1, [r0, #4]
004581c8 082082e2 add r2, r2, #8
004581cc 002080e5 str r2, [r0]
004581d0 0020e0e3 mvn r2, #0
004581d4 082080e5 str r2, [r0, #8]
004581d8 1080bde8 pop {r4, pc}
004581dc d8c85300 ldrsbeq ip, [r3], #-0x88
004581e0 081c0000 andeq r1, r0, r8, lsl #24
