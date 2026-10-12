00457390 10402de9 push {r4, lr}
00457394 0010a0e3 mov r1, #0
00457398 1400a0e3 mov r0, #0x14
0045739c 73e4faeb bl #0x310570
004573a0 2c409fe5 ldr r4, [pc, #0x2c]
004573a4 2c109fe5 ldr r1, [pc, #0x2c]
004573a8 0020a0e3 mov r2, #0
004573ac 04408fe0 add r4, pc, r4
004573b0 011094e7 ldr r1, [r4, r1]
004573b4 102080e5 str r2, [r0, #0x10]
004573b8 042080e5 str r2, [r0, #4]
004573bc 081081e2 add r1, r1, #8
004573c0 001080e5 str r1, [r0]
004573c4 082080e5 str r2, [r0, #8]
004573c8 0c20c0e5 strb r2, [r0, #0xc]
004573cc 0d20c0e5 strb r2, [r0, #0xd]
004573d0 1080bde8 pop {r4, pc}
004573d4 e4d65300 subseq sp, r3, r4, ror #13
004573d8 043a0000 andeq r3, r0, r4, lsl #20
