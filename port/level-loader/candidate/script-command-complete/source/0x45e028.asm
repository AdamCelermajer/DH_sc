0045e028 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045e02c dc409fe5 ldr r4, [pc, #0xdc]
0045e030 dc609fe5 ldr r6, [pc, #0xdc]
0045e034 dc109fe5 ldr r1, [pc, #0xdc]
0045e038 04408fe0 add r4, pc, r4
0045e03c 063094e7 ldr r3, [r4, r6]
0045e040 018094e7 ldr r8, [r4, r1]
0045e044 38d04de2 sub sp, sp, #0x38
0045e048 003093e5 ldr r3, [r3]
0045e04c 0290a0e1 mov sb, r2
0045e050 1c708de2 add r7, sp, #0x1c
0045e054 34308de5 str r3, [sp, #0x34]
0045e058 0ca090e5 ldr sl, [r0, #0xc]
0045e05c 0800a0e1 mov r0, r8
0045e060 0866fbeb bl #0x337888
0045e064 b0109fe5 ldr r1, [pc, #0xb0]
0045e068 18208de2 add r2, sp, #0x18
0045e06c 0700a0e1 mov r0, r7
0045e070 01108fe0 add r1, pc, r1
0045e074 1cd8faeb bl #0x3140ec
0045e078 0710a0e1 mov r1, r7
0045e07c 0800a0e1 mov r0, r8
0045e080 8066fbeb bl #0x337a88
0045e084 0700a0e1 mov r0, r7
0045e088 71e8faeb bl #0x318254
0045e08c 8c109fe5 ldr r1, [pc, #0x8c]
0045e090 0c508de2 add r5, sp, #0xc
0045e094 0c209ae5 ldr r2, [sl, #0xc]
0045e098 011094e7 ldr r1, [r4, r1]
0045e09c 0070a0e3 mov r7, #0
0045e0a0 0930a0e1 mov r3, sb
0045e0a4 381091e5 ldr r1, [r1, #0x38]
0045e0a8 0500a0e1 mov r0, r5
0045e0ac 00708de5 str r7, [sp]
0045e0b0 04708de5 str r7, [sp, #4]
0045e0b4 f9b2fbeb bl #0x34aca0
0045e0b8 0500a0e1 mov r0, r5
0045e0bc 0710a0e1 mov r1, r7
0045e0c0 3e87fbeb bl #0x33fdc0
0045e0c4 070050e1 cmp r0, r7
0045e0c8 0600001a bne #0x45e0e8
0045e0cc 063094e7 ldr r3, [r4, r6]
0045e0d0 34209de5 ldr r2, [sp, #0x34]
0045e0d4 003093e5 ldr r3, [r3]
0045e0d8 030052e1 cmp r2, r3
0045e0dc 0a00001a bne #0x45e10c
0045e0e0 38d08de2 add sp, sp, #0x38
0045e0e4 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045e0e8 0500a0e1 mov r0, r5
0045e0ec 7c87fbeb bl #0x33fee4
0045e0f0 003050e2 subs r3, r0, #0
0045e0f4 f4ffff0a beq #0x45e0cc
0045e0f8 003093e5 ldr r3, [r3]
0045e0fc 0710a0e1 mov r1, r7
0045e100 0fe0a0e1 mov lr, pc
0045e104 40f093e5 ldr pc, [r3, #0x40]
0045e108 efffffea b #0x45e0cc
0045e10c 7fc0faeb bl #0x30e310
0045e110 586a5300 subseq r6, r3, r8, asr sl
0045e114 ac400000 andeq r4, r0, ip, lsr #1
0045e118 84080000 andeq r0, r0, r4, lsl #17
0045e11c 10f04600 subeq pc, r6, r0, lsl r0
0045e120 f4370000 strdeq r3, r4, [r0], -r4
