00458c20 10402de9 push {r4, lr}
00458c24 0010a0e3 mov r1, #0
00458c28 1000a0e3 mov r0, #0x10
00458c2c 4fdefaeb bl #0x310570
00458c30 28409fe5 ldr r4, [pc, #0x28]
00458c34 28209fe5 ldr r2, [pc, #0x28]
00458c38 0010a0e3 mov r1, #0
00458c3c 04408fe0 add r4, pc, r4
00458c40 022094e7 ldr r2, [r4, r2]
00458c44 0c1080e5 str r1, [r0, #0xc]
00458c48 0410c0e5 strb r1, [r0, #4]
00458c4c 082082e2 add r2, r2, #8
00458c50 002080e5 str r2, [r0]
00458c54 0020e0e3 mvn r2, #0
00458c58 082080e5 str r2, [r0, #8]
00458c5c 1080bde8 pop {r4, pc}
00458c60 54be5300 subseq fp, r3, r4, asr lr
00458c64 90370000 muleq r0, r0, r7
