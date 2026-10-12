0045f5cc f0412de9 push {r4, r5, r6, r7, r8, lr}
0045f5d0 dc409fe5 ldr r4, [pc, #0xdc]
0045f5d4 dc609fe5 ldr r6, [pc, #0xdc]
0045f5d8 dc209fe5 ldr r2, [pc, #0xdc]
0045f5dc 04408fe0 add r4, pc, r4
0045f5e0 063094e7 ldr r3, [r4, r6]
0045f5e4 027094e7 ldr r7, [r4, r2]
0045f5e8 20d04de2 sub sp, sp, #0x20
0045f5ec 003093e5 ldr r3, [r3]
0045f5f0 04508de2 add r5, sp, #4
0045f5f4 1c308de5 str r3, [sp, #0x1c]
0045f5f8 0c8090e5 ldr r8, [r0, #0xc]
0045f5fc 0700a0e1 mov r0, r7
0045f600 a060fbeb bl #0x337888
0045f604 b4109fe5 ldr r1, [pc, #0xb4]
0045f608 0d20a0e1 mov r2, sp
0045f60c 0500a0e1 mov r0, r5
0045f610 01108fe0 add r1, pc, r1
0045f614 b4d2faeb bl #0x3140ec
0045f618 0510a0e1 mov r1, r5
0045f61c 0700a0e1 mov r0, r7
0045f620 1861fbeb bl #0x337a88
0045f624 0500a0e1 mov r0, r5
0045f628 09e3faeb bl #0x318254
0045f62c 58780eeb bl #0x7fd794
0045f630 0530d0e5 ldrb r3, [r0, #5]
0045f634 000053e3 cmp r3, #0
0045f638 1100001a bne #0x45f684
0045f63c 80309fe5 ldr r3, [pc, #0x80]
0045f640 033094e7 ldr r3, [r4, r3]
0045f644 400093e5 ldr r0, [r3, #0x40]
0045f648 0010a0e3 mov r1, #0
0045f64c 0120a0e3 mov r2, #1
0045f650 883bfceb bl #0x36e478
0045f654 600690e5 ldr r0, [r0, #0x660]
0045f658 000050e3 cmp r0, #0
0045f65c 0100000a beq #0x45f668
0045f660 081098e5 ldr r1, [r8, #8]
0045f664 ec3bfdeb bl #0x3ae61c
0045f668 063094e7 ldr r3, [r4, r6]
0045f66c 1c209de5 ldr r2, [sp, #0x1c]
0045f670 003093e5 ldr r3, [r3]
0045f674 030052e1 cmp r2, r3
0045f678 0c00001a bne #0x45f6b0
0045f67c 20d08de2 add sp, sp, #0x20
0045f680 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045f684 38309fe5 ldr r3, [pc, #0x38]
0045f688 035094e7 ldr r5, [r4, r3]
0045f68c 400095e5 ldr r0, [r5, #0x40]
0045f690 773efceb bl #0x36f074
0045f694 000050e3 cmp r0, #0
0045f698 f2ffff0a beq #0x45f668
0045f69c 400095e5 ldr r0, [r5, #0x40]
0045f6a0 1937d0e5 ldrb r3, [r0, #0x719]
0045f6a4 000053e3 cmp r3, #0
0045f6a8 eeffff1a bne #0x45f668
0045f6ac e5ffffea b #0x45f648
0045f6b0 16bbfaeb bl #0x30e310
0045f6b4 b4545300 ldrheq r5, [r3], #-0x44
0045f6b8 ac400000 andeq r4, r0, ip, lsr #1
0045f6bc 84080000 andeq r0, r0, r4, lsl #17
0045f6c0 70da4600 subeq sp, r6, r0, ror sl
0045f6c4 f4370000 strdeq r3, r4, [r0], -r4
