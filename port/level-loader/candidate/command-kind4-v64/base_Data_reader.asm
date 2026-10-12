0x4ff828 10402de9 push {r4, lr}
0x4ff82c 0040a0e1 mov r4, r0
0x4ff830 08d04de2 sub sp, sp, #8
0x4ff834 0100a0e1 mov r0, r1
0x4ff838 041084e2 add r1, r4, #4
0x4ff83c 1366fdeb bl #0x459090
0x4ff840 0130a0e3 mov r3, #1
0x4ff844 000053e3 cmp r3, #0
0x4ff848 04308de5 str r3, [sp, #4]
0x4ff84c 0f00001a bne #0x4ff890
0x4ff850 063084e2 add r3, r4, #6
0x4ff854 054084e2 add r4, r4, #5
0x4ff858 0110d3e5 ldrb r1, [r3, #1]
0x4ff85c 012054e5 ldrb r2, [r4, #-1]
0x4ff860 030054e1 cmp r4, r3
0x4ff864 022021e0 eor r2, r1, r2
0x4ff868 012044e5 strb r2, [r4, #-1]
0x4ff86c 0110d3e5 ldrb r1, [r3, #1]
0x4ff870 012022e0 eor r2, r2, r1
0x4ff874 0120c3e5 strb r2, [r3, #1]
0x4ff878 011054e5 ldrb r1, [r4, #-1]
0x4ff87c 013043e2 sub r3, r3, #1
0x4ff880 012022e0 eor r2, r2, r1
0x4ff884 012044e5 strb r2, [r4, #-1]
0x4ff888 014084e2 add r4, r4, #1
0x4ff88c f1ffff3a blo #0x4ff858
0x4ff890 08d08de2 add sp, sp, #8
0x4ff894 1080bde8 pop {r4, pc}
