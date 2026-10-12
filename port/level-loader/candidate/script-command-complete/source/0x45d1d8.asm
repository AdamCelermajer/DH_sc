0045d1d8 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045d1dc dc409fe5 ldr r4, [pc, #0xdc]
0045d1e0 dc509fe5 ldr r5, [pc, #0xdc]
0045d1e4 dc109fe5 ldr r1, [pc, #0xdc]
0045d1e8 04408fe0 add r4, pc, r4
0045d1ec 053094e7 ldr r3, [r4, r5]
0045d1f0 017094e7 ldr r7, [r4, r1]
0045d1f4 38d04de2 sub sp, sp, #0x38
0045d1f8 003093e5 ldr r3, [r3]
0045d1fc 0080a0e1 mov r8, r0
0045d200 0700a0e1 mov r0, r7
0045d204 34308de5 str r3, [sp, #0x34]
0045d208 0290a0e1 mov sb, r2
0045d20c 0ca098e5 ldr sl, [r8, #0xc]
0045d210 9c69fbeb bl #0x337888
0045d214 b0109fe5 ldr r1, [pc, #0xb0]
0045d218 1c608de2 add r6, sp, #0x1c
0045d21c 18208de2 add r2, sp, #0x18
0045d220 01108fe0 add r1, pc, r1
0045d224 0600a0e1 mov r0, r6
0045d228 afdbfaeb bl #0x3140ec
0045d22c 0610a0e1 mov r1, r6
0045d230 0700a0e1 mov r0, r7
0045d234 136afbeb bl #0x337a88
0045d238 0600a0e1 mov r0, r6
0045d23c 04ecfaeb bl #0x318254
0045d240 88309fe5 ldr r3, [pc, #0x88]
0045d244 0c708de2 add r7, sp, #0xc
0045d248 0c209ae5 ldr r2, [sl, #0xc]
0045d24c 031094e7 ldr r1, [r4, r3]
0045d250 0060a0e3 mov r6, #0
0045d254 0930a0e1 mov r3, sb
0045d258 381091e5 ldr r1, [r1, #0x38]
0045d25c 0700a0e1 mov r0, r7
0045d260 00608de5 str r6, [sp]
0045d264 04608de5 str r6, [sp, #4]
0045d268 8cb6fbeb bl #0x34aca0
0045d26c 0700a0e1 mov r0, r7
0045d270 0610a0e1 mov r1, r6
0045d274 d18afbeb bl #0x33fdc0
0045d278 000050e3 cmp r0, #0
0045d27c 0500000a beq #0x45d298
0045d280 f43090e5 ldr r3, [r0, #0xf4]
0045d284 120053e3 cmp r3, #0x12
0045d288 0130a003 moveq r3, #1
0045d28c 10008805 streq r0, [r8, #0x10]
0045d290 7937c005 strbeq r3, [r0, #0x779]
0045d294 0100000a beq #0x45d2a0
0045d298 0030a0e3 mov r3, #0
0045d29c 103088e5 str r3, [r8, #0x10]
0045d2a0 053094e7 ldr r3, [r4, r5]
0045d2a4 34209de5 ldr r2, [sp, #0x34]
0045d2a8 003093e5 ldr r3, [r3]
0045d2ac 030052e1 cmp r2, r3
0045d2b0 0100001a bne #0x45d2bc
0045d2b4 38d08de2 add sp, sp, #0x38
0045d2b8 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045d2bc 13c4faeb bl #0x30e310
0045d2c0 a8785300 subseq r7, r3, r8, lsr #17
0045d2c4 ac400000 andeq r4, r0, ip, lsr #1
0045d2c8 84080000 andeq r0, r0, r4, lsl #17
0045d2cc 60fe4600 subeq pc, r6, r0, ror #28
0045d2d0 f4370000 strdeq r3, r4, [r0], -r4
