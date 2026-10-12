004586c0 10402de9 push {r4, lr}
004586c4 0010a0e3 mov r1, #0
004586c8 1400a0e3 mov r0, #0x14
004586cc a7dffaeb bl #0x310570
004586d0 2c409fe5 ldr r4, [pc, #0x2c]
004586d4 2c109fe5 ldr r1, [pc, #0x2c]
004586d8 0020a0e3 mov r2, #0
004586dc 04408fe0 add r4, pc, r4
004586e0 011094e7 ldr r1, [r4, r1]
004586e4 102080e5 str r2, [r0, #0x10]
004586e8 0420c0e5 strb r2, [r0, #4]
004586ec 081081e2 add r1, r1, #8
004586f0 001080e5 str r1, [r0]
004586f4 0010e0e3 mvn r1, #0
004586f8 081080e5 str r1, [r0, #8]
004586fc 0c2080e5 str r2, [r0, #0xc]
00458700 1080bde8 pop {r4, pc}
00458704 b4c35300 ldrheq ip, [r3], #-0x34
00458708 e4280000 andeq r2, r0, r4, ror #17
