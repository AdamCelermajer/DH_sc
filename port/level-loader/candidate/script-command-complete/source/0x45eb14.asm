0045eb14 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045eb18 1c419fe5 ldr r4, [pc, #0x11c]
0045eb1c 1c519fe5 ldr r5, [pc, #0x11c]
0045eb20 1c119fe5 ldr r1, [pc, #0x11c]
0045eb24 04408fe0 add r4, pc, r4
0045eb28 053094e7 ldr r3, [r4, r5]
0045eb2c 016094e7 ldr r6, [r4, r1]
0045eb30 10719fe5 ldr r7, [pc, #0x110]
0045eb34 003093e5 ldr r3, [r3]
0045eb38 50d04de2 sub sp, sp, #0x50
0045eb3c 34808de2 add r8, sp, #0x34
0045eb40 07708fe0 add r7, pc, r7
0045eb44 00a0a0e1 mov sl, r0
0045eb48 0600a0e1 mov r0, r6
0045eb4c 4c308de5 str r3, [sp, #0x4c]
0045eb50 0290a0e1 mov sb, r2
0045eb54 4b63fbeb bl #0x337888
0045eb58 18208de2 add r2, sp, #0x18
0045eb5c 0710a0e1 mov r1, r7
0045eb60 0800a0e1 mov r0, r8
0045eb64 60d5faeb bl #0x3140ec
0045eb68 0810a0e1 mov r1, r8
0045eb6c 0600a0e1 mov r0, r6
0045eb70 c463fbeb bl #0x337a88
0045eb74 0800a0e1 mov r0, r8
0045eb78 b5e5faeb bl #0x318254
0045eb7c 1c808de2 add r8, sp, #0x1c
0045eb80 0600a0e1 mov r0, r6
0045eb84 0ca09ae5 ldr sl, [sl, #0xc]
0045eb88 3e63fbeb bl #0x337888
0045eb8c 14208de2 add r2, sp, #0x14
0045eb90 0710a0e1 mov r1, r7
0045eb94 0800a0e1 mov r0, r8
0045eb98 53d5faeb bl #0x3140ec
0045eb9c 0810a0e1 mov r1, r8
0045eba0 0600a0e1 mov r0, r6
0045eba4 b763fbeb bl #0x337a88
0045eba8 0800a0e1 mov r0, r8
0045ebac a8e5faeb bl #0x318254
0045ebb0 94309fe5 ldr r3, [pc, #0x94]
0045ebb4 08608de2 add r6, sp, #8
0045ebb8 0c209ae5 ldr r2, [sl, #0xc]
0045ebbc 031094e7 ldr r1, [r4, r3]
0045ebc0 0070a0e3 mov r7, #0
0045ebc4 0930a0e1 mov r3, sb
0045ebc8 381091e5 ldr r1, [r1, #0x38]
0045ebcc 0600a0e1 mov r0, r6
0045ebd0 00708de5 str r7, [sp]
0045ebd4 04708de5 str r7, [sp, #4]
0045ebd8 30b0fbeb bl #0x34aca0
0045ebdc 0600a0e1 mov r0, r6
0045ebe0 0710a0e1 mov r1, r7
0045ebe4 7584fbeb bl #0x33fdc0
0045ebe8 070050e1 cmp r0, r7
0045ebec 0600001a bne #0x45ec0c
0045ebf0 053094e7 ldr r3, [r4, r5]
0045ebf4 4c209de5 ldr r2, [sp, #0x4c]
0045ebf8 003093e5 ldr r3, [r3]
0045ebfc 030052e1 cmp r2, r3
0045ec00 0c00001a bne #0x45ec38
0045ec04 50d08de2 add sp, sp, #0x50
0045ec08 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045ec0c 0600a0e1 mov r0, r6
0045ec10 b384fbeb bl #0x33fee4
0045ec14 006050e2 subs r6, r0, #0
0045ec18 f4ffff0a beq #0x45ebf0
0045ec1c 77b6fceb bl #0x38c600
0045ec20 0600a0e1 mov r0, r6
0045ec24 003096e5 ldr r3, [r6]
0045ec28 0110a0e3 mov r1, #1
0045ec2c 0fe0a0e1 mov lr, pc
0045ec30 40f093e5 ldr pc, [r3, #0x40]
0045ec34 edffffea b #0x45ebf0
0045ec38 b4bdfaeb bl #0x30e310
0045ec3c 6c5f5300 subseq r5, r3, ip, ror #30
0045ec40 ac400000 andeq r4, r0, ip, lsr #1
0045ec44 84080000 andeq r0, r0, r4, lsl #17
0045ec48 40e54600 subeq lr, r6, r0, asr #10
0045ec4c f4370000 strdeq r3, r4, [r0], -r4
