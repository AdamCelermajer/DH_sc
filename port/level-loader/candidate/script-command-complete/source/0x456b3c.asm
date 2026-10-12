00456b3c 10402de9 push {r4, lr}
00456b40 0010a0e3 mov r1, #0
00456b44 2000a0e3 mov r0, #0x20
00456b48 88e6faeb bl #0x310570
00456b4c 20409fe5 ldr r4, [pc, #0x20]
00456b50 20209fe5 ldr r2, [pc, #0x20]
00456b54 0010a0e3 mov r1, #0
00456b58 04408fe0 add r4, pc, r4
00456b5c 022094e7 ldr r2, [r4, r2]
00456b60 1c1080e5 str r1, [r0, #0x1c]
00456b64 101080e5 str r1, [r0, #0x10]
00456b68 082082e2 add r2, r2, #8
00456b6c 002080e5 str r2, [r0]
00456b70 1080bde8 pop {r4, pc}
00456b74 38df5300 subseq sp, r3, r8, lsr pc
00456b78 882a0000 andeq r2, r0, r8, lsl #21
