0x4ffdc0 30402de9 push {r4, r5, lr}
0x4ffdc4 0040a0e1 mov r4, r0
0x4ffdc8 0cd04de2 sub sp, sp, #0xc
0x4ffdcc 0150a0e1 mov r5, r1
0x4ffdd0 94feffeb bl #0x4ff828
0x4ffdd4 0500a0e1 mov r0, r5
0x4ffdd8 081084e2 add r1, r4, #8
0x4ffddc da6effeb bl #0x4db94c
0x4ffde0 0130a0e3 mov r3, #1
0x4ffde4 000053e3 cmp r3, #0
0x4ffde8 04308de5 str r3, [sp, #4]
0x4ffdec 0f00001a bne #0x4ffe30
0x4ffdf0 0a3084e2 add r3, r4, #0xa
0x4ffdf4 094084e2 add r4, r4, #9
0x4ffdf8 0110d3e5 ldrb r1, [r3, #1]
0x4ffdfc 012054e5 ldrb r2, [r4, #-1]
0x4ffe00 030054e1 cmp r4, r3
0x4ffe04 022021e0 eor r2, r1, r2
0x4ffe08 012044e5 strb r2, [r4, #-1]
0x4ffe0c 0110d3e5 ldrb r1, [r3, #1]
0x4ffe10 012022e0 eor r2, r2, r1
0x4ffe14 0120c3e5 strb r2, [r3, #1]
0x4ffe18 011054e5 ldrb r1, [r4, #-1]
0x4ffe1c 013043e2 sub r3, r3, #1
0x4ffe20 012022e0 eor r2, r2, r1
0x4ffe24 012044e5 strb r2, [r4, #-1]
0x4ffe28 014084e2 add r4, r4, #1
0x4ffe2c f1ffff3a blo #0x4ffdf8
0x4ffe30 0cd08de2 add sp, sp, #0xc
0x4ffe34 3080bde8 pop {r4, r5, pc}
