0045fa9c f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045faa0 20419fe5 ldr r4, [pc, #0x120]
0045faa4 20519fe5 ldr r5, [pc, #0x120]
0045faa8 44d04de2 sub sp, sp, #0x44
0045faac 04408fe0 add r4, pc, r4
0045fab0 053094e7 ldr r3, [r4, r5]
0045fab4 007051e2 subs r7, r1, #0
0045fab8 02a0a0e1 mov sl, r2
0045fabc 003093e5 ldr r3, [r3]
0045fac0 3c308de5 str r3, [sp, #0x3c]
0045fac4 0600000a beq #0x45fae4
0045fac8 053094e7 ldr r3, [r4, r5]
0045facc 3c209de5 ldr r2, [sp, #0x3c]
0045fad0 003093e5 ldr r3, [r3]
0045fad4 030052e1 cmp r2, r3
0045fad8 3900001a bne #0x45fbc4
0045fadc 44d08de2 add sp, sp, #0x44
0045fae0 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045fae4 e4309fe5 ldr r3, [pc, #0xe4]
0045fae8 0c6090e5 ldr r6, [r0, #0xc]
0045faec 24808de2 add r8, sp, #0x24
0045faf0 03b094e7 ldr fp, [r4, r3]
0045faf4 14908de2 add sb, sp, #0x14
0045faf8 0b00a0e1 mov r0, fp
0045fafc 615ffbeb bl #0x337888
0045fb00 cc109fe5 ldr r1, [pc, #0xcc]
0045fb04 20208de2 add r2, sp, #0x20
0045fb08 0800a0e1 mov r0, r8
0045fb0c 01108fe0 add r1, pc, r1
0045fb10 75d1faeb bl #0x3140ec
0045fb14 0810a0e1 mov r1, r8
0045fb18 0b00a0e1 mov r0, fp
0045fb1c d95ffbeb bl #0x337a88
0045fb20 0800a0e1 mov r0, r8
0045fb24 cae1faeb bl #0x318254
0045fb28 a8109fe5 ldr r1, [pc, #0xa8]
0045fb2c 202096e5 ldr r2, [r6, #0x20]
0045fb30 0a30a0e1 mov r3, sl
0045fb34 011094e7 ldr r1, [r4, r1]
0045fb38 0900a0e1 mov r0, sb
0045fb3c 381091e5 ldr r1, [r1, #0x38]
0045fb40 00708de5 str r7, [sp]
0045fb44 04708de5 str r7, [sp, #4]
0045fb48 54acfbeb bl #0x34aca0
0045fb4c 0710a0e1 mov r1, r7
0045fb50 0900a0e1 mov r0, sb
0045fb54 08b096e5 ldr fp, [r6, #8]
0045fb58 9880fbeb bl #0x33fdc0
0045fb5c 00a050e2 subs sl, r0, #0
0045fb60 1300001a bne #0x45fbb4
0045fb64 140096e5 ldr r0, [r6, #0x14]
0045fb68 7dbbfaeb bl #0x30e964
0045fb6c 0070a0e1 mov r7, r0
0045fb70 180096e5 ldr r0, [r6, #0x18]
0045fb74 7abbfaeb bl #0x30e964
0045fb78 0080a0e1 mov r8, r0
0045fb7c 100096e5 ldr r0, [r6, #0x10]
0045fb80 77bbfaeb bl #0x30e964
0045fb84 50309fe5 ldr r3, [pc, #0x50]
0045fb88 08008de5 str r0, [sp, #8]
0045fb8c 00c0a0e3 mov ip, #0
0045fb90 030094e7 ldr r0, [r4, r3]
0045fb94 0b10a0e1 mov r1, fp
0045fb98 0a30a0e1 mov r3, sl
0045fb9c 08208de2 add r2, sp, #8
0045fba0 0c708de5 str r7, [sp, #0xc]
0045fba4 10808de5 str r8, [sp, #0x10]
0045fba8 00c08de5 str ip, [sp]
0045fbac 58d800eb bl #0x495d14
0045fbb0 c4ffffea b #0x45fac8
0045fbb4 0900a0e1 mov r0, sb
0045fbb8 c980fbeb bl #0x33fee4
0045fbbc 00a0a0e1 mov sl, r0
0045fbc0 e7ffffea b #0x45fb64
0045fbc4 d1b9faeb bl #0x30e310
0045fbc8 e44f5300 subseq r4, r3, r4, ror #31
0045fbcc ac400000 andeq r4, r0, ip, lsr #1
0045fbd0 84080000 andeq r0, r0, r4, lsl #17
0045fbd4 74d54600 subeq sp, r6, r4, ror r5
0045fbd8 f4370000 strdeq r3, r4, [r0], -r4
0045fbdc 081b0000 andeq r1, r0, r8, lsl #22
