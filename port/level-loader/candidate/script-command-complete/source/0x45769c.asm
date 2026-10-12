0045769c 10402de9 push {r4, lr}
004576a0 0010a0e3 mov r1, #0
004576a4 0800a0e3 mov r0, #8
004576a8 b0e3faeb bl #0x310570
004576ac 1c409fe5 ldr r4, [pc, #0x1c]
004576b0 1c309fe5 ldr r3, [pc, #0x1c]
004576b4 0010a0e3 mov r1, #0
004576b8 04408fe0 add r4, pc, r4
004576bc 033094e7 ldr r3, [r4, r3]
004576c0 041080e5 str r1, [r0, #4]
004576c4 083083e2 add r3, r3, #8
004576c8 003080e5 str r3, [r0]
004576cc 1080bde8 pop {r4, pc}
004576d0 d8d35300 ldrsbeq sp, [r3], #-0x38
004576d4 c0120000 andeq r1, r0, r0, asr #5
