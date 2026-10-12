0045dda0 f0472de9 push {r4, r5, r6, r7, r8, sb, sl, lr}
0045dda4 08419fe5 ldr r4, [pc, #0x108]
0045dda8 08519fe5 ldr r5, [pc, #0x108]
0045ddac 38d04de2 sub sp, sp, #0x38
0045ddb0 04408fe0 add r4, pc, r4
0045ddb4 053094e7 ldr r3, [r4, r5]
0045ddb8 006051e2 subs r6, r1, #0
0045ddbc 02a0a0e1 mov sl, r2
0045ddc0 003093e5 ldr r3, [r3]
0045ddc4 34308de5 str r3, [sp, #0x34]
0045ddc8 0600000a beq #0x45dde8
0045ddcc 053094e7 ldr r3, [r4, r5]
0045ddd0 34209de5 ldr r2, [sp, #0x34]
0045ddd4 003093e5 ldr r3, [r3]
0045ddd8 030052e1 cmp r2, r3
0045dddc 3300001a bne #0x45deb0
0045dde0 38d08de2 add sp, sp, #0x38
0045dde4 f087bde8 pop {r4, r5, r6, r7, r8, sb, sl, pc}
0045dde8 cc309fe5 ldr r3, [pc, #0xcc]
0045ddec 0c9090e5 ldr sb, [r0, #0xc]
0045ddf0 1c708de2 add r7, sp, #0x1c
0045ddf4 038094e7 ldr r8, [r4, r3]
0045ddf8 0800a0e1 mov r0, r8
0045ddfc a166fbeb bl #0x337888
0045de00 b8109fe5 ldr r1, [pc, #0xb8]
0045de04 18208de2 add r2, sp, #0x18
0045de08 0700a0e1 mov r0, r7
0045de0c 01108fe0 add r1, pc, r1
0045de10 b5d8faeb bl #0x3140ec
0045de14 0710a0e1 mov r1, r7
0045de18 0800a0e1 mov r0, r8
0045de1c 1967fbeb bl #0x337a88
0045de20 0700a0e1 mov r0, r7
0045de24 0ae9faeb bl #0x318254
0045de28 0c8099e5 ldr r8, [sb, #0xc]
0045de2c 90109fe5 ldr r1, [pc, #0x90]
0045de30 0800a0e1 mov r0, r8
0045de34 01108fe0 add r1, pc, r1
0045de38 2ac2faeb bl #0x30e6e8
0045de3c 000050e3 cmp r0, #0
0045de40 0400001a bne #0x45de58
0045de44 7c309fe5 ldr r3, [pc, #0x7c]
0045de48 0120a0e3 mov r2, #1
0045de4c 033094e7 ldr r3, [r4, r3]
0045de50 0020c3e5 strb r2, [r3]
0045de54 dcffffea b #0x45ddcc
0045de58 6c309fe5 ldr r3, [pc, #0x6c]
0045de5c 0c708de2 add r7, sp, #0xc
0045de60 0820a0e1 mov r2, r8
0045de64 031094e7 ldr r1, [r4, r3]
0045de68 0700a0e1 mov r0, r7
0045de6c 0a30a0e1 mov r3, sl
0045de70 381091e5 ldr r1, [r1, #0x38]
0045de74 00608de5 str r6, [sp]
0045de78 04608de5 str r6, [sp, #4]
0045de7c 87b3fbeb bl #0x34aca0
0045de80 0700a0e1 mov r0, r7
0045de84 0610a0e1 mov r1, r6
0045de88 cc87fbeb bl #0x33fdc0
0045de8c 000050e3 cmp r0, #0
0045de90 cdffff0a beq #0x45ddcc
0045de94 0700a0e1 mov r0, r7
0045de98 2d88fbeb bl #0x33ff54
0045de9c 000050e3 cmp r0, #0
0045dea0 78339015 ldrne r3, [r0, #0x378]
0045dea4 0120a013 movne r2, #1
0045dea8 0820c315 strbne r2, [r3, #8]
0045deac c6ffffea b #0x45ddcc
0045deb0 16c1faeb bl #0x30e310
0045deb4 e06c5300 subseq r6, r3, r0, ror #25
0045deb8 ac400000 andeq r4, r0, ip, lsr #1
0045debc 84080000 andeq r0, r0, r4, lsl #17
0045dec0 74f24600 subeq pc, r6, r4, ror r2
0045dec4 14b14600 subeq fp, r6, r4, lsl r1
0045dec8 50360000 andeq r3, r0, r0, asr r6
0045decc f4370000 strdeq r3, r4, [r0], -r4
