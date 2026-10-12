0045f0a8 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045f0ac 34429fe5 ldr r4, [pc, #0x234]
0045f0b0 34b29fe5 ldr fp, [pc, #0x234]
0045f0b4 34c29fe5 ldr ip, [pc, #0x234]
0045f0b8 04408fe0 add r4, pc, r4
0045f0bc 0b3094e7 ldr r3, [r4, fp]
0045f0c0 0c8094e7 ldr r8, [r4, ip]
0045f0c4 4cd04de2 sub sp, sp, #0x4c
0045f0c8 003093e5 ldr r3, [r3]
0045f0cc 0c108de5 str r1, [sp, #0xc]
0045f0d0 0060a0e1 mov r6, r0
0045f0d4 44308de5 str r3, [sp, #0x44]
0045f0d8 0800a0e1 mov r0, r8
0045f0dc 0290a0e1 mov sb, r2
0045f0e0 0c7096e5 ldr r7, [r6, #0xc]
0045f0e4 e761fbeb bl #0x337888
0045f0e8 04129fe5 ldr r1, [pc, #0x204]
0045f0ec 2c508de2 add r5, sp, #0x2c
0045f0f0 28208de2 add r2, sp, #0x28
0045f0f4 01108fe0 add r1, pc, r1
0045f0f8 0500a0e1 mov r0, r5
0045f0fc f4a19fe5 ldr sl, [pc, #0x1f4]
0045f100 f9d3faeb bl #0x3140ec
0045f104 0510a0e1 mov r1, r5
0045f108 0800a0e1 mov r0, r8
0045f10c 5d62fbeb bl #0x337a88
0045f110 0500a0e1 mov r0, r5
0045f114 4ee4faeb bl #0x318254
0045f118 0a3094e7 ldr r3, [r4, sl]
0045f11c 1c808de2 add r8, sp, #0x1c
0045f120 182097e5 ldr r2, [r7, #0x18]
0045f124 381093e5 ldr r1, [r3, #0x38]
0045f128 0050a0e3 mov r5, #0
0045f12c 0930a0e1 mov r3, sb
0045f130 0800a0e1 mov r0, r8
0045f134 00508de5 str r5, [sp]
0045f138 04508de5 str r5, [sp, #4]
0045f13c d7aefbeb bl #0x34aca0
0045f140 0510a0e1 mov r1, r5
0045f144 0800a0e1 mov r0, r8
0045f148 1c83fbeb bl #0x33fdc0
0045f14c 005050e2 subs r5, r0, #0
0045f150 5600001a bne #0x45f2b0
0045f154 0a3094e7 ldr r3, [r4, sl]
0045f158 10808de2 add r8, sp, #0x10
0045f15c 0c2097e5 ldr r2, [r7, #0xc]
0045f160 381093e5 ldr r1, [r3, #0x38]
0045f164 00a0a0e3 mov sl, #0
0045f168 0930a0e1 mov r3, sb
0045f16c 0800a0e1 mov r0, r8
0045f170 00a08de5 str sl, [sp]
0045f174 04a08de5 str sl, [sp, #4]
0045f178 c8aefbeb bl #0x34aca0
0045f17c 0a10a0e1 mov r1, sl
0045f180 0800a0e1 mov r0, r8
0045f184 0d83fbeb bl #0x33fdc0
0045f188 000050e3 cmp r0, #0
0045f18c 00a0a001 moveq sl, r0
0045f190 3e00001a bne #0x45f290
0045f194 1c30d7e5 ldrb r3, [r7, #0x1c]
0045f198 000055e3 cmp r5, #0
0045f19c 145086e5 str r5, [r6, #0x14]
0045f1a0 1030c6e5 strb r3, [r6, #0x10]
0045f1a4 8430d515 ldrbne r3, [r5, #0x84]
0045f1a8 0530a001 moveq r3, r5
0045f1ac 000050e3 cmp r0, #0
0045f1b0 1830c6e5 strb r3, [r6, #0x18]
0045f1b4 1000c605 strbeq r0, [r6, #0x10]
0045f1b8 2100000a beq #0x45f244
0045f1bc 1030d7e5 ldrb r3, [r7, #0x10]
0045f1c0 000053e3 cmp r3, #0
0045f1c4 3d00001a bne #0x45f2c0
0045f1c8 0c209de5 ldr r2, [sp, #0xc]
0045f1cc 783395e5 ldr r3, [r5, #0x378]
0045f1d0 000052e3 cmp r2, #0
0045f1d4 0120a0e3 mov r2, #1
0045f1d8 0920c3e5 strb r2, [r3, #9]
0045f1dc 2400001a bne #0x45f274
0045f1e0 1830d6e5 ldrb r3, [r6, #0x18]
0045f1e4 000053e3 cmp r3, #0
0045f1e8 0800000a beq #0x45f210
0045f1ec 1c30d7e5 ldrb r3, [r7, #0x1c]
0045f1f0 000053e3 cmp r3, #0
0045f1f4 0f00000a beq #0x45f238
0045f1f8 0c109de5 ldr r1, [sp, #0xc]
0045f1fc 0500a0e1 mov r0, r5
0045f200 0120a0e1 mov r2, r1
0045f204 7bd6fceb bl #0x394bf8
0045f208 0c309de5 ldr r3, [sp, #0xc]
0045f20c 8430c5e5 strb r3, [r5, #0x84]
0045f210 780395e5 ldr r0, [r5, #0x378]
0045f214 0a10a0e1 mov r1, sl
0045f218 c898feeb bl #0x405540
0045f21c 0520a0e1 mov r2, r5
0045f220 0032b2e5 ldr r3, [r2, #0x200]!
0045f224 020053e1 cmp r3, r2
0045f228 0f00000a beq #0x45f26c
0045f22c 003093e5 ldr r3, [r3]
0045f230 030052e1 cmp r2, r3
0045f234 fcffff1a bne #0x45f22c
0045f238 783395e5 ldr r3, [r5, #0x378]
0045f23c 0020a0e3 mov r2, #0
0045f240 0920c3e5 strb r2, [r3, #9]
0045f244 0800a0e1 mov r0, r8
0045f248 0010a0e3 mov r1, #0
0045f24c db82fbeb bl #0x33fdc0
0045f250 0b3094e7 ldr r3, [r4, fp]
0045f254 44209de5 ldr r2, [sp, #0x44]
0045f258 003093e5 ldr r3, [r3]
0045f25c 030052e1 cmp r2, r3
0045f260 1f00001a bne #0x45f2e4
0045f264 4cd08de2 add sp, sp, #0x4c
0045f268 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045f26c 0030a0e3 mov r3, #0
0045f270 1030c6e5 strb r3, [r6, #0x10]
0045f274 161e8ae2 add r1, sl, #0x160
0045f278 780395e5 ldr r0, [r5, #0x378]
0045f27c 2598feeb bl #0x405318
0045f280 783395e5 ldr r3, [r5, #0x378]
0045f284 0020a0e3 mov r2, #0
0045f288 0920c3e5 strb r2, [r3, #9]
0045f28c ecffffea b #0x45f244
0045f290 0800a0e1 mov r0, r8
0045f294 1283fbeb bl #0x33fee4
0045f298 000050e3 cmp r0, #0
0045f29c 00005513 cmpne r5, #0
0045f2a0 00a0a0e1 mov sl, r0
0045f2a4 0000a003 moveq r0, #0
0045f2a8 0100a013 movne r0, #1
0045f2ac b8ffffea b #0x45f194
0045f2b0 0800a0e1 mov r0, r8
0045f2b4 2683fbeb bl #0x33ff54
0045f2b8 0050a0e1 mov r5, r0
0045f2bc a4ffffea b #0x45f154
0045f2c0 0500a0e1 mov r0, r5
0045f2c4 b9d5fceb bl #0x3949b0
0045f2c8 0c209de5 ldr r2, [sp, #0xc]
0045f2cc 783395e5 ldr r3, [r5, #0x378]
0045f2d0 000052e3 cmp r2, #0
0045f2d4 0120a0e3 mov r2, #1
0045f2d8 0920c3e5 strb r2, [r3, #9]
0045f2dc bfffff0a beq #0x45f1e0
0045f2e0 e3ffffea b #0x45f274
0045f2e4 09bcfaeb bl #0x30e310
0045f2e8 d8595300 ldrsbeq r5, [r3], #-0x98
0045f2ec ac400000 andeq r4, r0, ip, lsr #1
0045f2f0 84080000 andeq r0, r0, r4, lsl #17
0045f2f4 8cdf4600 subeq sp, r6, ip, lsl #31
0045f2f8 f4370000 strdeq r3, r4, [r0], -r4
