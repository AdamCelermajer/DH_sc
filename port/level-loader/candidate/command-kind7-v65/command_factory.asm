0x458b48 10402de9 push {r4, lr}
0x458b4c 0010a0e3 mov r1, #0
0x458b50 1000a0e3 mov r0, #0x10
0x458b54 85defaeb bl #0x310570
0x458b58 28409fe5 ldr r4, [pc, #0x28]
0x458b5c 28209fe5 ldr r2, [pc, #0x28]
0x458b60 0010a0e3 mov r1, #0
0x458b64 04408fe0 add r4, pc, r4
0x458b68 022094e7 ldr r2, [r4, r2]
0x458b6c 0c1080e5 str r1, [r0, #0xc]
0x458b70 0410c0e5 strb r1, [r0, #4]
0x458b74 082082e2 add r2, r2, #8
0x458b78 002080e5 str r2, [r0]
0x458b7c 0020e0e3 mvn r2, #0
0x458b80 082080e5 str r2, [r0, #8]
0x458b84 1080bde8 pop {r4, pc}
