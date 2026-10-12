00458cf8 10402de9 push {r4, lr}
00458cfc 0010a0e3 mov r1, #0
00458d00 1000a0e3 mov r0, #0x10
00458d04 19defaeb bl #0x310570
00458d08 28409fe5 ldr r4, [pc, #0x28]
00458d0c 28209fe5 ldr r2, [pc, #0x28]
00458d10 0010a0e3 mov r1, #0
00458d14 04408fe0 add r4, pc, r4
00458d18 022094e7 ldr r2, [r4, r2]
00458d1c 0c1080e5 str r1, [r0, #0xc]
00458d20 0410c0e5 strb r1, [r0, #4]
00458d24 082082e2 add r2, r2, #8
00458d28 002080e5 str r2, [r0]
00458d2c 0020e0e3 mvn r2, #0
00458d30 082080e5 str r2, [r0, #8]
00458d34 1080bde8 pop {r4, pc}
00458d38 7cbd5300 subseq fp, r3, ip, ror sp
00458d3c 08410000 andeq r4, r0, r8, lsl #2
