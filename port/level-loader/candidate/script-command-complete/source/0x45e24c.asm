0045e24c f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045e250 08419fe5 ldr r4, [pc, #0x108]
0045e254 08519fe5 ldr r5, [pc, #0x108]
0045e258 3cd04de2 sub sp, sp, #0x3c
0045e25c 04408fe0 add r4, pc, r4
0045e260 053094e7 ldr r3, [r4, r5]
0045e264 006051e2 subs r6, r1, #0
0045e268 02a0a0e1 mov sl, r2
0045e26c 003093e5 ldr r3, [r3]
0045e270 34308de5 str r3, [sp, #0x34]
0045e274 0600000a beq #0x45e294
0045e278 053094e7 ldr r3, [r4, r5]
0045e27c 34209de5 ldr r2, [sp, #0x34]
0045e280 003093e5 ldr r3, [r3]
0045e284 030052e1 cmp r2, r3
0045e288 3300001a bne #0x45e35c
0045e28c 3cd08de2 add sp, sp, #0x3c
0045e290 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045e294 cc309fe5 ldr r3, [pc, #0xcc]
0045e298 0c7090e5 ldr r7, [r0, #0xc]
0045e29c 1c808de2 add r8, sp, #0x1c
0045e2a0 03b094e7 ldr fp, [r4, r3]
0045e2a4 0c908de2 add sb, sp, #0xc
0045e2a8 0b00a0e1 mov r0, fp
0045e2ac 7565fbeb bl #0x337888
0045e2b0 b4109fe5 ldr r1, [pc, #0xb4]
0045e2b4 18208de2 add r2, sp, #0x18
0045e2b8 0800a0e1 mov r0, r8
0045e2bc 01108fe0 add r1, pc, r1
0045e2c0 89d7faeb bl #0x3140ec
0045e2c4 0810a0e1 mov r1, r8
0045e2c8 0b00a0e1 mov r0, fp
0045e2cc ed65fbeb bl #0x337a88
0045e2d0 0800a0e1 mov r0, r8
0045e2d4 dee7faeb bl #0x318254
0045e2d8 90109fe5 ldr r1, [pc, #0x90]
0045e2dc 142097e5 ldr r2, [r7, #0x14]
0045e2e0 0a30a0e1 mov r3, sl
0045e2e4 011094e7 ldr r1, [r4, r1]
0045e2e8 0900a0e1 mov r0, sb
0045e2ec 381091e5 ldr r1, [r1, #0x38]
0045e2f0 00608de5 str r6, [sp]
0045e2f4 04608de5 str r6, [sp, #4]
0045e2f8 68b2fbeb bl #0x34aca0
0045e2fc 0900a0e1 mov r0, sb
0045e300 0610a0e1 mov r1, r6
0045e304 ad86fbeb bl #0x33fdc0
0045e308 000050e3 cmp r0, #0
0045e30c d9ffff0a beq #0x45e278
0045e310 0900a0e1 mov r0, sb
0045e314 f286fbeb bl #0x33fee4
0045e318 000050e3 cmp r0, #0
0045e31c d5ffff0a beq #0x45e278
0045e320 d83290e5 ldr r3, [r0, #0x2d8]
0045e324 000053e3 cmp r3, #0
0045e328 d2ffff0a beq #0x45e278
0045e32c 38c093e5 ldr ip, [r3, #0x38]
0045e330 00005ce3 cmp ip, #0
0045e334 cfffff0a beq #0x45e278
0045e338 0c20d7e5 ldrb r2, [r7, #0xc]
0045e33c 081097e5 ldr r1, [r7, #8]
0045e340 0c00a0e1 mov r0, ip
0045e344 0630a0e1 mov r3, r6
0045e348 00c09ce5 ldr ip, [ip]
0045e34c 00608de5 str r6, [sp]
0045e350 0fe0a0e1 mov lr, pc
0045e354 1cf09ce5 ldr pc, [ip, #0x1c]
0045e358 c6ffffea b #0x45e278
0045e35c ebbffaeb bl #0x30e310
0045e360 34685300 subseq r6, r3, r4, lsr r8
0045e364 ac400000 andeq r4, r0, ip, lsr #1
0045e368 84080000 andeq r0, r0, r4, lsl #17
0045e36c c4ed4600 subeq lr, r6, r4, asr #27
0045e370 f4370000 strdeq r3, r4, [r0], -r4
