00456f58 10402de9 push {r4, lr}
00456f5c 0010a0e3 mov r1, #0
00456f60 1000a0e3 mov r0, #0x10
00456f64 81e5faeb bl #0x310570
00456f68 1c409fe5 ldr r4, [pc, #0x1c]
00456f6c 1c309fe5 ldr r3, [pc, #0x1c]
00456f70 0010a0e3 mov r1, #0
00456f74 04408fe0 add r4, pc, r4
00456f78 033094e7 ldr r3, [r4, r3]
00456f7c 0c1080e5 str r1, [r0, #0xc]
00456f80 083083e2 add r3, r3, #8
00456f84 003080e5 str r3, [r0]
00456f88 1080bde8 pop {r4, pc}
00456f8c 1cdb5300 subseq sp, r3, ip, lsl fp
00456f90 c8050000 andeq r0, r0, r8, asr #11
