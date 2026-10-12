00460a58 70402de9 push {r4, r5, r6, lr}
00460a5c a8609fe5 ldr r6, [pc, #0xa8]
00460a60 83feffeb bl #0x460474
00460a64 a4309fe5 ldr r3, [pc, #0xa4]
00460a68 06608fe0 add r6, pc, r6
00460a6c 034096e7 ldr r4, [r6, r3]
00460a70 142094e5 ldr r2, [r4, #0x14]
00460a74 043094e5 ldr r3, [r4, #4]
00460a78 030052e1 cmp r2, r3
00460a7c 0600000a beq #0x460a9c
00460a80 045084e2 add r5, r4, #4
00460a84 0500a0e1 mov r0, r5
00460a88 198dfceb bl #0x383ef4
00460a8c 142094e5 ldr r2, [r4, #0x14]
00460a90 043094e5 ldr r3, [r4, #4]
00460a94 030052e1 cmp r2, r3
00460a98 f9ffff1a bne #0x460a84
00460a9c 74feffeb bl #0x460474
00460aa0 6c309fe5 ldr r3, [pc, #0x6c]
00460aa4 034096e7 ldr r4, [r6, r3]
00460aa8 142094e5 ldr r2, [r4, #0x14]
00460aac 043094e5 ldr r3, [r4, #4]
00460ab0 030052e1 cmp r2, r3
00460ab4 0600000a beq #0x460ad4
00460ab8 045084e2 add r5, r4, #4
00460abc 0500a0e1 mov r0, r5
00460ac0 928cfceb bl #0x383d10
00460ac4 142094e5 ldr r2, [r4, #0x14]
00460ac8 043094e5 ldr r3, [r4, #4]
00460acc 030052e1 cmp r2, r3
00460ad0 f9ffff1a bne #0x460abc
00460ad4 3c309fe5 ldr r3, [pc, #0x3c]
00460ad8 034096e7 ldr r4, [r6, r3]
00460adc 142094e5 ldr r2, [r4, #0x14]
00460ae0 043094e5 ldr r3, [r4, #4]
00460ae4 030052e1 cmp r2, r3
00460ae8 0600000a beq #0x460b08
00460aec 045084e2 add r5, r4, #4
00460af0 0500a0e1 mov r0, r5
00460af4 ab8cfceb bl #0x383da8
00460af8 142094e5 ldr r2, [r4, #0x14]
00460afc 043094e5 ldr r3, [r4, #4]
00460b00 030052e1 cmp r2, r3
00460b04 f9ffff1a bne #0x460af0
00460b08 7080bde8 pop {r4, r5, r6, pc}
00460b0c 28405300 subseq r4, r3, r8, lsr #32
00460b10 bc250000 strheq r2, [r0], -ip
00460b14 ec140000 andeq r1, r0, ip, ror #9
00460b18 d80f0000 ldrdeq r0, r1, [r0], -r8
