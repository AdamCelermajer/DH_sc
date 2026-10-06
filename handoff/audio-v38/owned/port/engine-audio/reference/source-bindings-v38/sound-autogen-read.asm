00502af4 push {r4, r5, lr}
00502af8 mov r4, r0
00502afc sub sp, sp, #0xc
00502b00 mov r0, r1
00502b04 mov r5, r1
00502b08 add r1, r4, #4
00502b0c bl #0x459090
00502b10 mov r3, #1
00502b14 cmp r3, #0
00502b18 str r3, [sp, #4]
00502b1c bne #0x502b60
00502b20 add r3, r4, #5
00502b24 add r2, r4, #6
00502b28 ldrb r0, [r2, #1]
00502b2c ldrb r1, [r3, #-1]
00502b30 cmp r3, r2
00502b34 eor r1, r0, r1
00502b38 strb r1, [r3, #-1]
00502b3c ldrb r0, [r2, #1]
00502b40 eor r1, r1, r0
00502b44 strb r1, [r2, #1]
00502b48 ldrb r0, [r3, #-1]
00502b4c sub r2, r2, #1
00502b50 eor r1, r1, r0
00502b54 strb r1, [r3, #-1]
00502b58 add r3, r3, #1
00502b5c blo #0x502b28
00502b60 mov r0, r5
00502b64 add r1, r4, #8
00502b68 bl #0x459090
00502b6c mov r3, #1
00502b70 cmp r3, #0
00502b74 str r3, [sp, #4]
00502b78 bne #0x502bbc
00502b7c add r3, r4, #0xa
00502b80 add r4, r4, #9
00502b84 ldrb r1, [r3, #1]
00502b88 ldrb r2, [r4, #-1]
00502b8c cmp r3, r4
00502b90 eor r2, r1, r2
00502b94 strb r2, [r4, #-1]
00502b98 ldrb r1, [r3, #1]
00502b9c eor r2, r2, r1
00502ba0 strb r2, [r3, #1]
00502ba4 ldrb r1, [r4, #-1]
00502ba8 sub r3, r3, #1
00502bac eor r2, r2, r1
00502bb0 strb r2, [r4, #-1]
00502bb4 add r4, r4, #1
00502bb8 bhi #0x502b84
00502bbc add sp, sp, #0xc
00502bc0 pop {r4, r5, pc}
