0x458c20 10402de9 push {r4, lr}
0x458c24 0010a0e3 mov r1, #0
0x458c28 1000a0e3 mov r0, #0x10
0x458c2c 4fdefaeb bl #0x310570
0x458c30 28409fe5 ldr r4, [pc, #0x28]
0x458c34 28209fe5 ldr r2, [pc, #0x28]
0x458c38 0010a0e3 mov r1, #0
0x458c3c 04408fe0 add r4, pc, r4
0x458c40 022094e7 ldr r2, [r4, r2]
0x458c44 0c1080e5 str r1, [r0, #0xc]
0x458c48 0410c0e5 strb r1, [r0, #4]
0x458c4c 082082e2 add r2, r2, #8
0x458c50 002080e5 str r2, [r0]
0x458c54 0020e0e3 mvn r2, #0
0x458c58 082080e5 str r2, [r0, #8]
0x458c5c 1080bde8 pop {r4, pc}
