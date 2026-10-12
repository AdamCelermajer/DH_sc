0045ec50 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045ec54 4c429fe5 ldr r4, [pc, #0x24c]
0045ec58 4cb29fe5 ldr fp, [pc, #0x24c]
0045ec5c 4c129fe5 ldr r1, [pc, #0x24c]
0045ec60 04408fe0 add r4, pc, r4
0045ec64 0b3094e7 ldr r3, [r4, fp]
0045ec68 017094e7 ldr r7, [r4, r1]
0045ec6c 54d04de2 sub sp, sp, #0x54
0045ec70 003093e5 ldr r3, [r3]
0045ec74 0290a0e1 mov sb, r2
0045ec78 34508de2 add r5, sp, #0x34
0045ec7c 4c308de5 str r3, [sp, #0x4c]
0045ec80 0c6090e5 ldr r6, [r0, #0xc]
0045ec84 0700a0e1 mov r0, r7
0045ec88 fe62fbeb bl #0x337888
0045ec8c 20129fe5 ldr r1, [pc, #0x220]
0045ec90 30208de2 add r2, sp, #0x30
0045ec94 0500a0e1 mov r0, r5
0045ec98 01108fe0 add r1, pc, r1
0045ec9c 14a29fe5 ldr sl, [pc, #0x214]
0045eca0 11d5faeb bl #0x3140ec
0045eca4 0510a0e1 mov r1, r5
0045eca8 0700a0e1 mov r0, r7
0045ecac 7563fbeb bl #0x337a88
0045ecb0 0500a0e1 mov r0, r5
0045ecb4 66e5faeb bl #0x318254
0045ecb8 0a3094e7 ldr r3, [r4, sl]
0045ecbc 24808de2 add r8, sp, #0x24
0045ecc0 142096e5 ldr r2, [r6, #0x14]
0045ecc4 381093e5 ldr r1, [r3, #0x38]
0045ecc8 0050a0e3 mov r5, #0
0045eccc 0930a0e1 mov r3, sb
0045ecd0 0800a0e1 mov r0, r8
0045ecd4 00508de5 str r5, [sp]
0045ecd8 04508de5 str r5, [sp, #4]
0045ecdc efaffbeb bl #0x34aca0
0045ece0 0800a0e1 mov r0, r8
0045ece4 0510a0e1 mov r1, r5
0045ece8 3484fbeb bl #0x33fdc0
0045ecec 007050e2 subs r7, r0, #0
0045ecf0 4d00001a bne #0x45ee2c
0045ecf4 18508de2 add r5, sp, #0x18
0045ecf8 0010a0e3 mov r1, #0
0045ecfc 0500a0e1 mov r0, r5
0045ed00 0782fbeb bl #0x33f524
0045ed04 0c8096e5 ldr r8, [r6, #0xc]
0045ed08 ac119fe5 ldr r1, [pc, #0x1ac]
0045ed0c 0800a0e1 mov r0, r8
0045ed10 01108fe0 add r1, pc, r1
0045ed14 80bdfaeb bl #0x30e31c
0045ed18 000050e3 cmp r0, #0
0045ed1c 3400001a bne #0x45edf4
0045ed20 00008de5 str r0, [sp]
0045ed24 0a3094e7 ldr r3, [r4, sl]
0045ed28 14c096e5 ldr ip, [r6, #0x14]
0045ed2c 08608de2 add r6, sp, #8
0045ed30 381093e5 ldr r1, [r3, #0x38]
0045ed34 0820a0e1 mov r2, r8
0045ed38 0930a0e1 mov r3, sb
0045ed3c 0600a0e1 mov r0, r6
0045ed40 04c08de5 str ip, [sp, #4]
0045ed44 d5affbeb bl #0x34aca0
0045ed48 083096e5 ldr r3, [r6, #8]
0045ed4c 0500a0e1 mov r0, r5
0045ed50 0010a0e3 mov r1, #0
0045ed54 083085e5 str r3, [r5, #8]
0045ed58 08309de5 ldr r3, [sp, #8]
0045ed5c 18308de5 str r3, [sp, #0x18]
0045ed60 0c309de5 ldr r3, [sp, #0xc]
0045ed64 1c308de5 str r3, [sp, #0x1c]
0045ed68 1484fbeb bl #0x33fdc0
0045ed6c 006050e2 subs r6, r0, #0
0045ed70 2900001a bne #0x45ee1c
0045ed74 000057e3 cmp r7, #0
0045ed78 1300000a beq #0x45edcc
0045ed7c 0500a0e1 mov r0, r5
0045ed80 0010a0e3 mov r1, #0
0045ed84 0d84fbeb bl #0x33fdc0
0045ed88 000050e3 cmp r0, #0
0045ed8c 0e00000a beq #0x45edcc
0045ed90 003097e5 ldr r3, [r7]
0045ed94 0700a0e1 mov r0, r7
0045ed98 0fe0a0e1 mov lr, pc
0045ed9c 28f093e5 ldr pc, [r3, #0x28]
0045eda0 000050e3 cmp r0, #0
0045eda4 2400001a bne #0x45ee3c
0045eda8 783397e5 ldr r3, [r7, #0x378]
0045edac 0120a0e3 mov r2, #1
0045edb0 0610a0e1 mov r1, r6
0045edb4 0920c3e5 strb r2, [r3, #9]
0045edb8 780397e5 ldr r0, [r7, #0x378]
0045edbc 3e99feeb bl #0x4052bc
0045edc0 783397e5 ldr r3, [r7, #0x378]
0045edc4 0020a0e3 mov r2, #0
0045edc8 0920c3e5 strb r2, [r3, #9]
0045edcc 0500a0e1 mov r0, r5
0045edd0 0010a0e3 mov r1, #0
0045edd4 f983fbeb bl #0x33fdc0
0045edd8 0b3094e7 ldr r3, [r4, fp]
0045eddc 4c209de5 ldr r2, [sp, #0x4c]
0045ede0 003093e5 ldr r3, [r3]
0045ede4 030052e1 cmp r2, r3
0045ede8 2d00001a bne #0x45eea4
0045edec 54d08de2 add sp, sp, #0x54
0045edf0 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045edf4 0a3094e7 ldr r3, [r4, sl]
0045edf8 00c0a0e3 mov ip, #0
0045edfc 08608de2 add r6, sp, #8
0045ee00 381093e5 ldr r1, [r3, #0x38]
0045ee04 0820a0e1 mov r2, r8
0045ee08 0930a0e1 mov r3, sb
0045ee0c 0600a0e1 mov r0, r6
0045ee10 04c08de5 str ip, [sp, #4]
0045ee14 00c08de5 str ip, [sp]
0045ee18 c9ffffea b #0x45ed44
0045ee1c 0500a0e1 mov r0, r5
0045ee20 2f84fbeb bl #0x33fee4
0045ee24 0060a0e1 mov r6, r0
0045ee28 d1ffffea b #0x45ed74
0045ee2c 0800a0e1 mov r0, r8
0045ee30 4784fbeb bl #0x33ff54
0045ee34 0070a0e1 mov r7, r0
0045ee38 adffffea b #0x45ecf4
0045ee3c 0aa094e7 ldr sl, [r4, sl]
0045ee40 0a00a0e1 mov r0, sl
0045ee44 d201fbeb bl #0x31f594
0045ee48 000050e3 cmp r0, #0
0045ee4c d5ffff0a beq #0x45eda8
0045ee50 0180a0e3 mov r8, #1
0045ee54 40009ae5 ldr r0, [sl, #0x40]
0045ee58 0810a0e1 mov r1, r8
0045ee5c 0020a0e3 mov r2, #0
0045ee60 373efceb bl #0x36e744
0045ee64 609690e5 ldr sb, [r0, #0x660]
0045ee68 000059e3 cmp sb, #0
0045ee6c 0800000a beq #0x45ee94
0045ee70 783399e5 ldr r3, [sb, #0x378]
0045ee74 0120a0e3 mov r2, #1
0045ee78 0610a0e1 mov r1, r6
0045ee7c 0920c3e5 strb r2, [r3, #9]
0045ee80 780399e5 ldr r0, [sb, #0x378]
0045ee84 0c99feeb bl #0x4052bc
0045ee88 783399e5 ldr r3, [sb, #0x378]
0045ee8c 0020a0e3 mov r2, #0
0045ee90 0920c3e5 strb r2, [r3, #9]
0045ee94 018088e2 add r8, r8, #1
0045ee98 040058e3 cmp r8, #4
0045ee9c ecffff1a bne #0x45ee54
0045eea0 c0ffffea b #0x45eda8
0045eea4 19bdfaeb bl #0x30e310
0045eea8 305e5300 subseq r5, r3, r0, lsr lr
0045eeac ac400000 andeq r4, r0, ip, lsr #1
0045eeb0 84080000 andeq r0, r0, r4, lsl #17
0045eeb4 e8e34600 subeq lr, r6, r8, ror #7
0045eeb8 f4370000 strdeq r3, r4, [r0], -r4
0045eebc d0164600 ldrdeq r1, r2, [r6], #-0x60
