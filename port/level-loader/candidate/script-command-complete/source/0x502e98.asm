00502e98 30402de9 push {r4, r5, lr}
00502e9c 0040a0e1 mov r4, r0
00502ea0 0cd04de2 sub sp, sp, #0xc
00502ea4 0150a0e1 mov r5, r1
00502ea8 5ef2ffeb bl #0x4ff828
00502eac 0500a0e1 mov r0, r5
00502eb0 081084e2 add r1, r4, #8
00502eb4 7558fdeb bl #0x459090
00502eb8 0130a0e3 mov r3, #1
00502ebc 000053e3 cmp r3, #0
00502ec0 04308de5 str r3, [sp, #4]
00502ec4 0f00001a bne #0x502f08
00502ec8 0a3084e2 add r3, r4, #0xa
00502ecc 094084e2 add r4, r4, #9
00502ed0 0110d3e5 ldrb r1, [r3, #1]
00502ed4 012054e5 ldrb r2, [r4, #-1]
00502ed8 030054e1 cmp r4, r3
00502edc 022021e0 eor r2, r1, r2
00502ee0 012044e5 strb r2, [r4, #-1]
00502ee4 0110d3e5 ldrb r1, [r3, #1]
00502ee8 012022e0 eor r2, r2, r1
00502eec 0120c3e5 strb r2, [r3, #1]
00502ef0 011054e5 ldrb r1, [r4, #-1]
00502ef4 013043e2 sub r3, r3, #1
00502ef8 012022e0 eor r2, r2, r1
00502efc 012044e5 strb r2, [r4, #-1]
00502f00 014084e2 add r4, r4, #1
00502f04 f1ffff3a blo #0x502ed0
00502f08 0cd08de2 add sp, sp, #0xc
00502f0c 3080bde8 pop {r4, r5, pc}
