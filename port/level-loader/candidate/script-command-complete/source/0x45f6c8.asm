0045f6c8 f0412de9 push {r4, r5, r6, r7, r8, lr}
0045f6cc a4409fe5 ldr r4, [pc, #0xa4]
0045f6d0 a4609fe5 ldr r6, [pc, #0xa4]
0045f6d4 a4209fe5 ldr r2, [pc, #0xa4]
0045f6d8 04408fe0 add r4, pc, r4
0045f6dc 063094e7 ldr r3, [r4, r6]
0045f6e0 027094e7 ldr r7, [r4, r2]
0045f6e4 20d04de2 sub sp, sp, #0x20
0045f6e8 003093e5 ldr r3, [r3]
0045f6ec 04508de2 add r5, sp, #4
0045f6f0 1c308de5 str r3, [sp, #0x1c]
0045f6f4 0c8090e5 ldr r8, [r0, #0xc]
0045f6f8 0700a0e1 mov r0, r7
0045f6fc 6160fbeb bl #0x337888
0045f700 7c109fe5 ldr r1, [pc, #0x7c]
0045f704 0d20a0e1 mov r2, sp
0045f708 0500a0e1 mov r0, r5
0045f70c 01108fe0 add r1, pc, r1
0045f710 75d2faeb bl #0x3140ec
0045f714 0510a0e1 mov r1, r5
0045f718 0700a0e1 mov r0, r7
0045f71c d960fbeb bl #0x337a88
0045f720 0500a0e1 mov r0, r5
0045f724 cae2faeb bl #0x318254
0045f728 58309fe5 ldr r3, [pc, #0x58]
0045f72c 0010a0e3 mov r1, #0
0045f730 0120a0e3 mov r2, #1
0045f734 033094e7 ldr r3, [r4, r3]
0045f738 400093e5 ldr r0, [r3, #0x40]
0045f73c 4d3bfceb bl #0x36e478
0045f740 600690e5 ldr r0, [r0, #0x660]
0045f744 000050e3 cmp r0, #0
0045f748 0200000a beq #0x45f758
0045f74c 0c2098e5 ldr r2, [r8, #0xc]
0045f750 081098e5 ldr r1, [r8, #8]
0045f754 1e3cfdeb bl #0x3ae7d4
0045f758 063094e7 ldr r3, [r4, r6]
0045f75c 1c209de5 ldr r2, [sp, #0x1c]
0045f760 003093e5 ldr r3, [r3]
0045f764 030052e1 cmp r2, r3
0045f768 0100001a bne #0x45f774
0045f76c 20d08de2 add sp, sp, #0x20
0045f770 f081bde8 pop {r4, r5, r6, r7, r8, pc}
0045f774 e5bafaeb bl #0x30e310
0045f778 b8535300 ldrheq r5, [r3], #-0x38
0045f77c ac400000 andeq r4, r0, ip, lsr #1
0045f780 84080000 andeq r0, r0, r4, lsl #17
0045f784 74d94600 subeq sp, r6, r4, ror sb
0045f788 f4370000 strdeq r3, r4, [r0], -r4
