00458d40 10402de9 push {r4, lr}
00458d44 0010a0e3 mov r1, #0
00458d48 1400a0e3 mov r0, #0x14
00458d4c 07defaeb bl #0x310570
00458d50 2c409fe5 ldr r4, [pc, #0x2c]
00458d54 2c109fe5 ldr r1, [pc, #0x2c]
00458d58 0020a0e3 mov r2, #0
00458d5c 04408fe0 add r4, pc, r4
00458d60 011094e7 ldr r1, [r4, r1]
00458d64 102080e5 str r2, [r0, #0x10]
00458d68 0420c0e5 strb r2, [r0, #4]
00458d6c 081081e2 add r1, r1, #8
00458d70 001080e5 str r1, [r0]
00458d74 0010e0e3 mvn r1, #0
00458d78 081080e5 str r1, [r0, #8]
00458d7c 0c2080e5 str r2, [r0, #0xc]
00458d80 1080bde8 pop {r4, pc}
00458d84 34bd5300 subseq fp, r3, r4, lsr sp
00458d88 90170000 muleq r0, r0, r7
