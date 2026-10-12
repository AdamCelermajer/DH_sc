004574a0 10402de9 push {r4, lr}
004574a4 0010a0e3 mov r1, #0
004574a8 0800a0e3 mov r0, #8
004574ac 2fe4faeb bl #0x310570
004574b0 1c409fe5 ldr r4, [pc, #0x1c]
004574b4 1c309fe5 ldr r3, [pc, #0x1c]
004574b8 0010a0e3 mov r1, #0
004574bc 04408fe0 add r4, pc, r4
004574c0 033094e7 ldr r3, [r4, r3]
004574c4 041080e5 str r1, [r0, #4]
004574c8 083083e2 add r3, r3, #8
004574cc 003080e5 str r3, [r0]
004574d0 1080bde8 pop {r4, pc}
004574d4 d4d55300 ldrsbeq sp, [r3], #-0x54
004574d8 bc2b0000 strheq r2, [r0], -ip
