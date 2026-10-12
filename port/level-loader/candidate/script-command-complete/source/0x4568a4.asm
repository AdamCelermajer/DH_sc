004568a4 10402de9 push {r4, lr}
004568a8 0010a0e3 mov r1, #0
004568ac 1000a0e3 mov r0, #0x10
004568b0 2ee7faeb bl #0x310570
004568b4 1c409fe5 ldr r4, [pc, #0x1c]
004568b8 1c309fe5 ldr r3, [pc, #0x1c]
004568bc 0010a0e3 mov r1, #0
004568c0 04408fe0 add r4, pc, r4
004568c4 033094e7 ldr r3, [r4, r3]
004568c8 0c1080e5 str r1, [r0, #0xc]
004568cc 083083e2 add r3, r3, #8
004568d0 003080e5 str r3, [r0]
004568d4 1080bde8 pop {r4, pc}
004568d8 d0e15300 ldrsbeq lr, [r3], #-0x10
004568dc 98260000 muleq r0, r8, r6
