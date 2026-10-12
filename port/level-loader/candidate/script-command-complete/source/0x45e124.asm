0045e124 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045e128 08419fe5 ldr r4, [pc, #0x108]
0045e12c 08519fe5 ldr r5, [pc, #0x108]
0045e130 3cd04de2 sub sp, sp, #0x3c
0045e134 04408fe0 add r4, pc, r4
0045e138 053094e7 ldr r3, [r4, r5]
0045e13c 006051e2 subs r6, r1, #0
0045e140 02a0a0e1 mov sl, r2
0045e144 003093e5 ldr r3, [r3]
0045e148 34308de5 str r3, [sp, #0x34]
0045e14c 0600000a beq #0x45e16c
0045e150 053094e7 ldr r3, [r4, r5]
0045e154 34209de5 ldr r2, [sp, #0x34]
0045e158 003093e5 ldr r3, [r3]
0045e15c 030052e1 cmp r2, r3
0045e160 3300001a bne #0x45e234
0045e164 3cd08de2 add sp, sp, #0x3c
0045e168 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045e16c cc309fe5 ldr r3, [pc, #0xcc]
0045e170 0c7090e5 ldr r7, [r0, #0xc]
0045e174 1c808de2 add r8, sp, #0x1c
0045e178 03b094e7 ldr fp, [r4, r3]
0045e17c 0c908de2 add sb, sp, #0xc
0045e180 0b00a0e1 mov r0, fp
0045e184 bf65fbeb bl #0x337888
0045e188 b4109fe5 ldr r1, [pc, #0xb4]
0045e18c 18208de2 add r2, sp, #0x18
0045e190 0800a0e1 mov r0, r8
0045e194 01108fe0 add r1, pc, r1
0045e198 d3d7faeb bl #0x3140ec
0045e19c 0810a0e1 mov r1, r8
0045e1a0 0b00a0e1 mov r0, fp
0045e1a4 3766fbeb bl #0x337a88
0045e1a8 0800a0e1 mov r0, r8
0045e1ac 28e8faeb bl #0x318254
0045e1b0 90109fe5 ldr r1, [pc, #0x90]
0045e1b4 182097e5 ldr r2, [r7, #0x18]
0045e1b8 0a30a0e1 mov r3, sl
0045e1bc 011094e7 ldr r1, [r4, r1]
0045e1c0 0900a0e1 mov r0, sb
0045e1c4 381091e5 ldr r1, [r1, #0x38]
0045e1c8 00608de5 str r6, [sp]
0045e1cc 04608de5 str r6, [sp, #4]
0045e1d0 b2b2fbeb bl #0x34aca0
0045e1d4 0900a0e1 mov r0, sb
0045e1d8 0610a0e1 mov r1, r6
0045e1dc f786fbeb bl #0x33fdc0
0045e1e0 000050e3 cmp r0, #0
0045e1e4 d9ffff0a beq #0x45e150
0045e1e8 0900a0e1 mov r0, sb
0045e1ec 3c87fbeb bl #0x33fee4
0045e1f0 000050e3 cmp r0, #0
0045e1f4 d5ffff0a beq #0x45e150
0045e1f8 d83290e5 ldr r3, [r0, #0x2d8]
0045e1fc 000053e3 cmp r3, #0
0045e200 d2ffff0a beq #0x45e150
0045e204 38c093e5 ldr ip, [r3, #0x38]
0045e208 00005ce3 cmp ip, #0
0045e20c cfffff0a beq #0x45e150
0045e210 1020d7e5 ldrb r2, [r7, #0x10]
0045e214 0c1097e5 ldr r1, [r7, #0xc]
0045e218 0c00a0e1 mov r0, ip
0045e21c 0630a0e1 mov r3, r6
0045e220 00c09ce5 ldr ip, [ip]
0045e224 00608de5 str r6, [sp]
0045e228 0fe0a0e1 mov lr, pc
0045e22c 20f09ce5 ldr pc, [ip, #0x20]
0045e230 c6ffffea b #0x45e150
0045e234 35c0faeb bl #0x30e310
0045e238 5c695300 subseq r6, r3, ip, asr sb
0045e23c ac400000 andeq r4, r0, ip, lsr #1
0045e240 84080000 andeq r0, r0, r4, lsl #17
0045e244 ecee4600 subeq lr, r6, ip, ror #29
0045e248 f4370000 strdeq r3, r4, [r0], -r4
