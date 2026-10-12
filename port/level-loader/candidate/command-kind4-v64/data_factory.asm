0x4575e4 10402de9 push {r4, lr}
0x4575e8 0010a0e3 mov r1, #0
0x4575ec 2000a0e3 mov r0, #0x20
0x4575f0 dee3faeb bl #0x310570
0x4575f4 20409fe5 ldr r4, [pc, #0x20]
0x4575f8 20209fe5 ldr r2, [pc, #0x20]
0x4575fc 0010a0e3 mov r1, #0
0x457600 04408fe0 add r4, pc, r4
0x457604 022094e7 ldr r2, [r4, r2]
0x457608 141080e5 str r1, [r0, #0x14]
0x45760c 0c1080e5 str r1, [r0, #0xc]
0x457610 082082e2 add r2, r2, #8
0x457614 002080e5 str r2, [r0]
0x457618 1080bde8 pop {r4, pc}
