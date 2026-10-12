00458b90 10402de9 push {r4, lr}
00458b94 0010a0e3 mov r1, #0
00458b98 1000a0e3 mov r0, #0x10
00458b9c 73defaeb bl #0x310570
00458ba0 28409fe5 ldr r4, [pc, #0x28]
00458ba4 28209fe5 ldr r2, [pc, #0x28]
00458ba8 0010a0e3 mov r1, #0
00458bac 04408fe0 add r4, pc, r4
00458bb0 022094e7 ldr r2, [r4, r2]
00458bb4 0c1080e5 str r1, [r0, #0xc]
00458bb8 0410c0e5 strb r1, [r0, #4]
00458bbc 082082e2 add r2, r2, #8
00458bc0 002080e5 str r2, [r0]
00458bc4 0020e0e3 mvn r2, #0
00458bc8 082080e5 str r2, [r0, #8]
00458bcc 1080bde8 pop {r4, pc}
00458bd0 e4be5300 subseq fp, r3, r4, ror #29
00458bd4 84170000 andeq r1, r0, r4, lsl #15
