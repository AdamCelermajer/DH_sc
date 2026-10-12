00457d4c 10402de9 push {r4, lr}
00457d50 0010a0e3 mov r1, #0
00457d54 1400a0e3 mov r0, #0x14
00457d58 04e2faeb bl #0x310570
00457d5c 2c409fe5 ldr r4, [pc, #0x2c]
00457d60 2c109fe5 ldr r1, [pc, #0x2c]
00457d64 0020a0e3 mov r2, #0
00457d68 04408fe0 add r4, pc, r4
00457d6c 011094e7 ldr r1, [r4, r1]
00457d70 102080e5 str r2, [r0, #0x10]
00457d74 0420c0e5 strb r2, [r0, #4]
00457d78 081081e2 add r1, r1, #8
00457d7c 001080e5 str r1, [r0]
00457d80 0010e0e3 mvn r1, #0
00457d84 081080e5 str r1, [r0, #8]
00457d88 0c2080e5 str r2, [r0, #0xc]
00457d8c 1080bde8 pop {r4, pc}
00457d90 28cd5300 subseq ip, r3, r8, lsr #26
00457d94 141d0000 andeq r1, r0, r4, lsl sp
