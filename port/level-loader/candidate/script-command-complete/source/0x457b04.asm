00457b04 10402de9 push {r4, lr}
00457b08 0010a0e3 mov r1, #0
00457b0c 1000a0e3 mov r0, #0x10
00457b10 96e2faeb bl #0x310570
00457b14 28409fe5 ldr r4, [pc, #0x28]
00457b18 28209fe5 ldr r2, [pc, #0x28]
00457b1c 0010a0e3 mov r1, #0
00457b20 04408fe0 add r4, pc, r4
00457b24 022094e7 ldr r2, [r4, r2]
00457b28 0c1080e5 str r1, [r0, #0xc]
00457b2c 0410c0e5 strb r1, [r0, #4]
00457b30 082082e2 add r2, r2, #8
00457b34 002080e5 str r2, [r0]
00457b38 0020e0e3 mvn r2, #0
00457b3c 082080e5 str r2, [r0, #8]
00457b40 1080bde8 pop {r4, pc}
00457b44 70cf5300 subseq ip, r3, r0, ror pc
00457b48 7c280000 andeq r2, r0, ip, ror r8
