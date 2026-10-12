00457de4 10402de9 push {r4, lr}
00457de8 0010a0e3 mov r1, #0
00457dec 1800a0e3 mov r0, #0x18
00457df0 dee1faeb bl #0x310570
00457df4 30409fe5 ldr r4, [pc, #0x30]
00457df8 30109fe5 ldr r1, [pc, #0x30]
00457dfc 0020a0e3 mov r2, #0
00457e00 04408fe0 add r4, pc, r4
00457e04 011094e7 ldr r1, [r4, r1]
00457e08 1420c0e5 strb r2, [r0, #0x14]
00457e0c 0420c0e5 strb r2, [r0, #4]
00457e10 081081e2 add r1, r1, #8
00457e14 001080e5 str r1, [r0]
00457e18 0010e0e3 mvn r1, #0
00457e1c 081080e5 str r1, [r0, #8]
00457e20 0c2080e5 str r2, [r0, #0xc]
00457e24 102080e5 str r2, [r0, #0x10]
00457e28 1080bde8 pop {r4, pc}
