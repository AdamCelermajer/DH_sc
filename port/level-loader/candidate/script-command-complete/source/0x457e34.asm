00457e34 10402de9 push {r4, lr}
00457e38 0010a0e3 mov r1, #0
00457e3c 1800a0e3 mov r0, #0x18
00457e40 cae1faeb bl #0x310570
00457e44 30409fe5 ldr r4, [pc, #0x30]
00457e48 30109fe5 ldr r1, [pc, #0x30]
00457e4c 0020a0e3 mov r2, #0
00457e50 04408fe0 add r4, pc, r4
00457e54 011094e7 ldr r1, [r4, r1]
00457e58 1420c0e5 strb r2, [r0, #0x14]
00457e5c 0420c0e5 strb r2, [r0, #4]
00457e60 081081e2 add r1, r1, #8
00457e64 001080e5 str r1, [r0]
00457e68 0010e0e3 mvn r1, #0
00457e6c 081080e5 str r1, [r0, #8]
00457e70 0c2080e5 str r2, [r0, #0xc]
00457e74 102080e5 str r2, [r0, #0x10]
00457e78 1080bde8 pop {r4, pc}
00457e7c 40cc5300 subseq ip, r3, r0, asr #24
00457e80 2c090000 andeq r0, r0, ip, lsr #18
