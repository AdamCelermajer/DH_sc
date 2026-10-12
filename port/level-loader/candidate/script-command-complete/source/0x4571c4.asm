004571c4 10402de9 push {r4, lr}
004571c8 0010a0e3 mov r1, #0
004571cc 2800a0e3 mov r0, #0x28
004571d0 e6e4faeb bl #0x310570
004571d4 2c409fe5 ldr r4, [pc, #0x2c]
004571d8 2c109fe5 ldr r1, [pc, #0x2c]
004571dc 2c209fe5 ldr r2, [pc, #0x2c]
004571e0 04408fe0 add r4, pc, r4
004571e4 011094e7 ldr r1, [r4, r1]
004571e8 022094e7 ldr r2, [r4, r2]
004571ec 00c0a0e3 mov ip, #0
004571f0 081081e2 add r1, r1, #8
004571f4 082082e2 add r2, r2, #8
004571f8 20c080e5 str ip, [r0, #0x20]
004571fc 001080e5 str r1, [r0]
00457200 0c2080e5 str r2, [r0, #0xc]
00457204 1080bde8 pop {r4, pc}
00457208 b0d85300 ldrheq sp, [r3], #-0x80
0045720c cc250000 andeq r2, r0, ip, asr #11
00457210 f84b0000 strdeq r4, r5, [r0], -r8
