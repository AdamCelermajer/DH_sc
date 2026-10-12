00457d00 10402de9 push {r4, lr}
00457d04 0010a0e3 mov r1, #0
00457d08 1400a0e3 mov r0, #0x14
00457d0c 17e2faeb bl #0x310570
00457d10 2c409fe5 ldr r4, [pc, #0x2c]
00457d14 2c109fe5 ldr r1, [pc, #0x2c]
00457d18 0020a0e3 mov r2, #0
00457d1c 04408fe0 add r4, pc, r4
00457d20 011094e7 ldr r1, [r4, r1]
00457d24 102080e5 str r2, [r0, #0x10]
00457d28 0420c0e5 strb r2, [r0, #4]
00457d2c 081081e2 add r1, r1, #8
00457d30 001080e5 str r1, [r0]
00457d34 0010e0e3 mvn r1, #0
00457d38 081080e5 str r1, [r0, #8]
00457d3c 0c2080e5 str r2, [r0, #0xc]
00457d40 1080bde8 pop {r4, pc}
00457d44 74cd5300 subseq ip, r3, r4, ror sp
00457d48 6c0d0000 andeq r0, r0, ip, ror #26
