00457cb4 10402de9 push {r4, lr}
00457cb8 0010a0e3 mov r1, #0
00457cbc 1400a0e3 mov r0, #0x14
00457cc0 2ae2faeb bl #0x310570
00457cc4 2c409fe5 ldr r4, [pc, #0x2c]
00457cc8 2c109fe5 ldr r1, [pc, #0x2c]
00457ccc 0020a0e3 mov r2, #0
00457cd0 04408fe0 add r4, pc, r4
00457cd4 011094e7 ldr r1, [r4, r1]
00457cd8 102080e5 str r2, [r0, #0x10]
00457cdc 0420c0e5 strb r2, [r0, #4]
00457ce0 081081e2 add r1, r1, #8
00457ce4 001080e5 str r1, [r0]
00457ce8 0010e0e3 mvn r1, #0
00457cec 081080e5 str r1, [r0, #8]
00457cf0 0c2080e5 str r2, [r0, #0xc]
00457cf4 1080bde8 pop {r4, pc}
00457cf8 c0cd5300 subseq ip, r3, r0, asr #27
00457cfc 8c050000 andeq r0, r0, ip, lsl #11
