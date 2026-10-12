0045ded0 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045ded4 38419fe5 ldr r4, [pc, #0x138]
0045ded8 38619fe5 ldr r6, [pc, #0x138]
0045dedc 38119fe5 ldr r1, [pc, #0x138]
0045dee0 04408fe0 add r4, pc, r4
0045dee4 063094e7 ldr r3, [r4, r6]
0045dee8 018094e7 ldr r8, [r4, r1]
0045deec 38d04de2 sub sp, sp, #0x38
0045def0 003093e5 ldr r3, [r3]
0045def4 0290a0e1 mov sb, r2
0045def8 1c708de2 add r7, sp, #0x1c
0045defc 34308de5 str r3, [sp, #0x34]
0045df00 0ca090e5 ldr sl, [r0, #0xc]
0045df04 0800a0e1 mov r0, r8
0045df08 5e66fbeb bl #0x337888
0045df0c 0c119fe5 ldr r1, [pc, #0x10c]
0045df10 18208de2 add r2, sp, #0x18
0045df14 0700a0e1 mov r0, r7
0045df18 01108fe0 add r1, pc, r1
0045df1c 72d8faeb bl #0x3140ec
0045df20 0710a0e1 mov r1, r7
0045df24 0800a0e1 mov r0, r8
0045df28 d666fbeb bl #0x337a88
0045df2c 0700a0e1 mov r0, r7
0045df30 c7e8faeb bl #0x318254
0045df34 e8109fe5 ldr r1, [pc, #0xe8]
0045df38 0c508de2 add r5, sp, #0xc
0045df3c 0c209ae5 ldr r2, [sl, #0xc]
0045df40 011094e7 ldr r1, [r4, r1]
0045df44 0070a0e3 mov r7, #0
0045df48 0930a0e1 mov r3, sb
0045df4c 381091e5 ldr r1, [r1, #0x38]
0045df50 0500a0e1 mov r0, r5
0045df54 00708de5 str r7, [sp]
0045df58 04708de5 str r7, [sp, #4]
0045df5c 4fb3fbeb bl #0x34aca0
0045df60 0500a0e1 mov r0, r5
0045df64 0710a0e1 mov r1, r7
0045df68 9487fbeb bl #0x33fdc0
0045df6c 070050e1 cmp r0, r7
0045df70 0600001a bne #0x45df90
0045df74 063094e7 ldr r3, [r4, r6]
0045df78 34209de5 ldr r2, [sp, #0x34]
0045df7c 003093e5 ldr r3, [r3]
0045df80 030052e1 cmp r2, r3
0045df84 2100001a bne #0x45e010
0045df88 38d08de2 add sp, sp, #0x38
0045df8c f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045df90 0500a0e1 mov r0, r5
0045df94 ee87fbeb bl #0x33ff54
0045df98 005050e2 subs r5, r0, #0
0045df9c f4ffff0a beq #0x45df74
0045dfa0 df7f85e2 add r7, r5, #0x37c
0045dfa4 0700a0e1 mov r0, r7
0045dfa8 0110a0e3 mov r1, #1
0045dfac a287feeb bl #0x3ffe3c
0045dfb0 0010a0e1 mov r1, r0
0045dfb4 0700a0e1 mov r0, r7
0045dfb8 9f79feeb bl #0x3fc63c
0045dfbc f43401e3 movw r3, #0x14f4
0045dfc0 030085e7 str r0, [r5, r3]
0045dfc4 0210a0e3 mov r1, #2
0045dfc8 0700a0e1 mov r0, r7
0045dfcc 9a87feeb bl #0x3ffe3c
0045dfd0 0010a0e1 mov r1, r0
0045dfd4 0700a0e1 mov r0, r7
0045dfd8 9779feeb bl #0x3fc63c
0045dfdc f83401e3 movw r3, #0x14f8
0045dfe0 030085e7 str r0, [r5, r3]
0045dfe4 003095e5 ldr r3, [r5]
0045dfe8 0500a0e1 mov r0, r5
0045dfec 0110a0e3 mov r1, #1
0045dff0 0fe0a0e1 mov lr, pc
0045dff4 44f193e5 ldr pc, [r3, #0x144]
0045dff8 0500a0e1 mov r0, r5
0045dffc 003095e5 ldr r3, [r5]
0045e000 0210a0e3 mov r1, #2
0045e004 0fe0a0e1 mov lr, pc
0045e008 44f193e5 ldr pc, [r3, #0x144]
0045e00c d8ffffea b #0x45df74
0045e010 bec0faeb bl #0x30e310
0045e014 b06b5300 ldrheq r6, [r3], #-0xb0
0045e018 ac400000 andeq r4, r0, ip, lsr #1
0045e01c 84080000 andeq r0, r0, r4, lsl #17
0045e020 68f14600 subeq pc, r6, r8, ror #2
0045e024 f4370000 strdeq r3, r4, [r0], -r4
