00458b00 10402de9 push {r4, lr}
00458b04 0010a0e3 mov r1, #0
00458b08 1000a0e3 mov r0, #0x10
00458b0c 97defaeb bl #0x310570
00458b10 28409fe5 ldr r4, [pc, #0x28]
00458b14 28209fe5 ldr r2, [pc, #0x28]
00458b18 0010a0e3 mov r1, #0
00458b1c 04408fe0 add r4, pc, r4
00458b20 022094e7 ldr r2, [r4, r2]
00458b24 0c1080e5 str r1, [r0, #0xc]
00458b28 0410c0e5 strb r1, [r0, #4]
00458b2c 082082e2 add r2, r2, #8
00458b30 002080e5 str r2, [r0]
00458b34 0020e0e3 mvn r2, #0
00458b38 082080e5 str r2, [r0, #8]
00458b3c 1080bde8 pop {r4, pc}
00458b40 74bf5300 subseq fp, r3, r4, ror pc
00458b44 8c1a0000 andeq r1, r0, ip, lsl #21
