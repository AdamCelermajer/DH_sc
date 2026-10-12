0045cdd0 f0402de9 push {r4, r5, r6, r7, lr}
0045cdd4 88409fe5 ldr r4, [pc, #0x88]
0045cdd8 88609fe5 ldr r6, [pc, #0x88]
0045cddc 88209fe5 ldr r2, [pc, #0x88]
0045cde0 04408fe0 add r4, pc, r4
0045cde4 063094e7 ldr r3, [r4, r6]
0045cde8 027094e7 ldr r7, [r4, r2]
0045cdec 24d04de2 sub sp, sp, #0x24
0045cdf0 003093e5 ldr r3, [r3]
0045cdf4 0700a0e1 mov r0, r7
0045cdf8 04508de2 add r5, sp, #4
0045cdfc 1c308de5 str r3, [sp, #0x1c]
0045ce00 a06afbeb bl #0x337888
0045ce04 64109fe5 ldr r1, [pc, #0x64]
0045ce08 0d20a0e1 mov r2, sp
0045ce0c 0500a0e1 mov r0, r5
0045ce10 01108fe0 add r1, pc, r1
0045ce14 b4dcfaeb bl #0x3140ec
0045ce18 0510a0e1 mov r1, r5
0045ce1c 0700a0e1 mov r0, r7
0045ce20 186bfbeb bl #0x337a88
0045ce24 0500a0e1 mov r0, r5
0045ce28 09edfaeb bl #0x318254
0045ce2c 40309fe5 ldr r3, [pc, #0x40]
0045ce30 030094e7 ldr r0, [r4, r3]
0045ce34 d609fbeb bl #0x31f594
0045ce38 000050e3 cmp r0, #0
0045ce3c 0000000a beq #0x45ce44
0045ce40 d64afeeb bl #0x3ef9a0
0045ce44 063094e7 ldr r3, [r4, r6]
0045ce48 1c209de5 ldr r2, [sp, #0x1c]
0045ce4c 003093e5 ldr r3, [r3]
0045ce50 030052e1 cmp r2, r3
0045ce54 0100001a bne #0x45ce60
0045ce58 24d08de2 add sp, sp, #0x24
0045ce5c f080bde8 pop {r4, r5, r6, r7, pc}
0045ce60 2ac5faeb bl #0x30e310
0045ce64 b07c5300 ldrheq r7, [r3], #-0xc0
0045ce68 ac400000 andeq r4, r0, ip, lsr #1
0045ce6c 84080000 andeq r0, r0, r4, lsl #17
0045ce70 70024700 subeq r0, r7, r0, ror r2
0045ce74 f4370000 strdeq r3, r4, [r0], -r4
