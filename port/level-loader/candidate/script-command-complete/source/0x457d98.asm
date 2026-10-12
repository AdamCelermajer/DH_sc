00457d98 10402de9 push {r4, lr}
00457d9c 0010a0e3 mov r1, #0
00457da0 1400a0e3 mov r0, #0x14
00457da4 f1e1faeb bl #0x310570
00457da8 2c409fe5 ldr r4, [pc, #0x2c]
00457dac 2c109fe5 ldr r1, [pc, #0x2c]
00457db0 0020a0e3 mov r2, #0
00457db4 04408fe0 add r4, pc, r4
00457db8 011094e7 ldr r1, [r4, r1]
00457dbc 102080e5 str r2, [r0, #0x10]
00457dc0 0420c0e5 strb r2, [r0, #4]
00457dc4 081081e2 add r1, r1, #8
00457dc8 001080e5 str r1, [r0]
00457dcc 0010e0e3 mvn r1, #0
00457dd0 081080e5 str r1, [r0, #8]
00457dd4 0c2080e5 str r2, [r0, #0xc]
00457dd8 1080bde8 pop {r4, pc}
00457ddc dccc5300 ldrsbeq ip, [r3], #-0xcc
00457de0 58400000 andeq r4, r0, r8, asr r0
