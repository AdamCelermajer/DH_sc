004575a0 10402de9 push {r4, lr}
004575a4 0010a0e3 mov r1, #0
004575a8 1000a0e3 mov r0, #0x10
004575ac efe3faeb bl #0x310570
004575b0 24409fe5 ldr r4, [pc, #0x24]
004575b4 24109fe5 ldr r1, [pc, #0x24]
004575b8 0020a0e3 mov r2, #0
004575bc 04408fe0 add r4, pc, r4
004575c0 011094e7 ldr r1, [r4, r1]
004575c4 0c20c0e5 strb r2, [r0, #0xc]
004575c8 042080e5 str r2, [r0, #4]
004575cc 081081e2 add r1, r1, #8
004575d0 001080e5 str r1, [r0]
004575d4 082080e5 str r2, [r0, #8]
004575d8 1080bde8 pop {r4, pc}
004575dc d4d45300 ldrsbeq sp, [r3], #-0x44
004575e0 84210000 andeq r2, r0, r4, lsl #3
