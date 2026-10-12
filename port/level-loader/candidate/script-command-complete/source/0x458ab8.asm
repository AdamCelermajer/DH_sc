00458ab8 10402de9 push {r4, lr}
00458abc 0010a0e3 mov r1, #0
00458ac0 1000a0e3 mov r0, #0x10
00458ac4 a9defaeb bl #0x310570
00458ac8 28409fe5 ldr r4, [pc, #0x28]
00458acc 28209fe5 ldr r2, [pc, #0x28]
00458ad0 0010a0e3 mov r1, #0
00458ad4 04408fe0 add r4, pc, r4
00458ad8 022094e7 ldr r2, [r4, r2]
00458adc 0c1080e5 str r1, [r0, #0xc]
00458ae0 0410c0e5 strb r1, [r0, #4]
00458ae4 082082e2 add r2, r2, #8
00458ae8 002080e5 str r2, [r0]
00458aec 0020e0e3 mvn r2, #0
00458af0 082080e5 str r2, [r0, #8]
00458af4 1080bde8 pop {r4, pc}
00458af8 bcbf5300 ldrheq fp, [r3], #-0xfc
00458afc 2c320000 andeq r3, r0, ip, lsr #4
