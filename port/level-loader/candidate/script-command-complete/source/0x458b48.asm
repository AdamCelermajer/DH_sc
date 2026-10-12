00458b48 10402de9 push {r4, lr}
00458b4c 0010a0e3 mov r1, #0
00458b50 1000a0e3 mov r0, #0x10
00458b54 85defaeb bl #0x310570
00458b58 28409fe5 ldr r4, [pc, #0x28]
00458b5c 28209fe5 ldr r2, [pc, #0x28]
00458b60 0010a0e3 mov r1, #0
00458b64 04408fe0 add r4, pc, r4
00458b68 022094e7 ldr r2, [r4, r2]
00458b6c 0c1080e5 str r1, [r0, #0xc]
00458b70 0410c0e5 strb r1, [r0, #4]
00458b74 082082e2 add r2, r2, #8
00458b78 002080e5 str r2, [r0]
00458b7c 0020e0e3 mvn r2, #0
00458b80 082080e5 str r2, [r0, #8]
00458b84 1080bde8 pop {r4, pc}
00458b88 2cbf5300 subseq fp, r3, ip, lsr #30
00458b8c 282f0000 andeq r2, r0, r8, lsr #30
