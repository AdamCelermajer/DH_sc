0045da8c f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045da90 dc409fe5 ldr r4, [pc, #0xdc]
0045da94 dc809fe5 ldr r8, [pc, #0xdc]
0045da98 dc109fe5 ldr r1, [pc, #0xdc]
0045da9c 04408fe0 add r4, pc, r4
0045daa0 083094e7 ldr r3, [r4, r8]
0045daa4 01a094e7 ldr sl, [r4, r1]
0045daa8 38d04de2 sub sp, sp, #0x38
0045daac 003093e5 ldr r3, [r3]
0045dab0 0290a0e1 mov sb, r2
0045dab4 1c508de2 add r5, sp, #0x1c
0045dab8 34308de5 str r3, [sp, #0x34]
0045dabc 0c7090e5 ldr r7, [r0, #0xc]
0045dac0 0a00a0e1 mov r0, sl
0045dac4 6f67fbeb bl #0x337888
0045dac8 b0109fe5 ldr r1, [pc, #0xb0]
0045dacc 18208de2 add r2, sp, #0x18
0045dad0 0500a0e1 mov r0, r5
0045dad4 01108fe0 add r1, pc, r1
0045dad8 83d9faeb bl #0x3140ec
0045dadc 0510a0e1 mov r1, r5
0045dae0 0a00a0e1 mov r0, sl
0045dae4 e767fbeb bl #0x337a88
0045dae8 0500a0e1 mov r0, r5
0045daec d8e9faeb bl #0x318254
0045daf0 8c109fe5 ldr r1, [pc, #0x8c]
0045daf4 0c608de2 add r6, sp, #0xc
0045daf8 102097e5 ldr r2, [r7, #0x10]
0045dafc 011094e7 ldr r1, [r4, r1]
0045db00 0050a0e3 mov r5, #0
0045db04 0930a0e1 mov r3, sb
0045db08 381091e5 ldr r1, [r1, #0x38]
0045db0c 0600a0e1 mov r0, r6
0045db10 00508de5 str r5, [sp]
0045db14 04508de5 str r5, [sp, #4]
0045db18 60b4fbeb bl #0x34aca0
0045db1c 0600a0e1 mov r0, r6
0045db20 0510a0e1 mov r1, r5
0045db24 a588fbeb bl #0x33fdc0
0045db28 050050e1 cmp r0, r5
0045db2c 0600001a bne #0x45db4c
0045db30 083094e7 ldr r3, [r4, r8]
0045db34 34209de5 ldr r2, [sp, #0x34]
0045db38 003093e5 ldr r3, [r3]
0045db3c 030052e1 cmp r2, r3
0045db40 0a00001a bne #0x45db70
0045db44 38d08de2 add sp, sp, #0x38
0045db48 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045db4c 0600a0e1 mov r0, r6
0045db50 ff88fbeb bl #0x33ff54
0045db54 003050e2 subs r3, r0, #0
0045db58 f4ffff0a beq #0x45db30
0045db5c 003093e5 ldr r3, [r3]
0045db60 081097e5 ldr r1, [r7, #8]
0045db64 0fe0a0e1 mov lr, pc
0045db68 44f193e5 ldr pc, [r3, #0x144]
0045db6c efffffea b #0x45db30
0045db70 e6c1faeb bl #0x30e310
0045db74 f46f5300 ldrsheq r6, [r3], #-0xf4
0045db78 ac400000 andeq r4, r0, ip, lsr #1
0045db7c 84080000 andeq r0, r0, r4, lsl #17
0045db80 acf54600 subeq pc, r6, ip, lsr #11
0045db84 f4370000 strdeq r3, r4, [r0], -r4
