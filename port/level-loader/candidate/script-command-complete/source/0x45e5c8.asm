0045e5c8 f04f2de9 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0045e5cc a8429fe5 ldr r4, [pc, #0x2a8]
0045e5d0 a8929fe5 ldr sb, [pc, #0x2a8]
0045e5d4 a8129fe5 ldr r1, [pc, #0x2a8]
0045e5d8 04408fe0 add r4, pc, r4
0045e5dc 093094e7 ldr r3, [r4, sb]
0045e5e0 016094e7 ldr r6, [r4, r1]
0045e5e4 74d04de2 sub sp, sp, #0x74
0045e5e8 003093e5 ldr r3, [r3]
0045e5ec 02a0a0e1 mov sl, r2
0045e5f0 54508de2 add r5, sp, #0x54
0045e5f4 6c308de5 str r3, [sp, #0x6c]
0045e5f8 0c7090e5 ldr r7, [r0, #0xc]
0045e5fc 0600a0e1 mov r0, r6
0045e600 a064fbeb bl #0x337888
0045e604 7c129fe5 ldr r1, [pc, #0x27c]
0045e608 50208de2 add r2, sp, #0x50
0045e60c 0500a0e1 mov r0, r5
0045e610 01108fe0 add r1, pc, r1
0045e614 70b29fe5 ldr fp, [pc, #0x270]
0045e618 b3d6faeb bl #0x3140ec
0045e61c 0510a0e1 mov r1, r5
0045e620 0600a0e1 mov r0, r6
0045e624 1765fbeb bl #0x337a88
0045e628 0500a0e1 mov r0, r5
0045e62c 08e7faeb bl #0x318254
0045e630 0b6094e7 ldr r6, [r4, fp]
0045e634 44808de2 add r8, sp, #0x44
0045e638 142097e5 ldr r2, [r7, #0x14]
0045e63c 381096e5 ldr r1, [r6, #0x38]
0045e640 0050a0e3 mov r5, #0
0045e644 0a30a0e1 mov r3, sl
0045e648 0800a0e1 mov r0, r8
0045e64c 00508de5 str r5, [sp]
0045e650 04508de5 str r5, [sp, #4]
0045e654 91b1fbeb bl #0x34aca0
0045e658 0800a0e1 mov r0, r8
0045e65c 2086fbeb bl #0x33fee4
0045e660 0c2097e5 ldr r2, [r7, #0xc]
0045e664 38708de2 add r7, sp, #0x38
0045e668 381096e5 ldr r1, [r6, #0x38]
0045e66c 0080a0e1 mov r8, r0
0045e670 0a30a0e1 mov r3, sl
0045e674 0700a0e1 mov r0, r7
0045e678 00508de5 str r5, [sp]
0045e67c 04508de5 str r5, [sp, #4]
0045e680 86b1fbeb bl #0x34aca0
0045e684 0700a0e1 mov r0, r7
0045e688 1586fbeb bl #0x33fee4
0045e68c 050050e1 cmp r0, r5
0045e690 05005811 cmpne r8, r5
0045e694 0070a0e1 mov r7, r0
0045e698 0600001a bne #0x45e6b8
0045e69c 093094e7 ldr r3, [r4, sb]
0045e6a0 6c209de5 ldr r2, [sp, #0x6c]
0045e6a4 003093e5 ldr r3, [r3]
0045e6a8 030052e1 cmp r2, r3
0045e6ac 7100001a bne #0x45e878
0045e6b0 74d08de2 add sp, sp, #0x74
0045e6b4 f08fbde8 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0045e6b8 2ca08de2 add sl, sp, #0x2c
0045e6bc 0a00a0e1 mov r0, sl
0045e6c0 0810a0e1 mov r1, r8
0045e6c4 987dfbeb bl #0x33dd2c
0045e6c8 0a00a0e1 mov r0, sl
0045e6cc 2086fbeb bl #0x33ff54
0045e6d0 00a050e2 subs sl, r0, #0
0045e6d4 0900000a beq #0x45e700
0045e6d8 4f0e8ae2 add r0, sl, #0x4f0
0045e6dc 0c0080e2 add r0, r0, #0xc
0045e6e0 0510a0e1 mov r1, r5
0045e6e4 c58cfdeb bl #0x3c1a00
0045e6e8 0a00a0e1 mov r0, sl
0045e6ec 00309ae5 ldr r3, [sl]
0045e6f0 0fe0a0e1 mov lr, pc
0045e6f4 28f093e5 ldr pc, [r3, #0x28]
0045e6f8 050050e1 cmp r0, r5
0045e6fc 0600001a bne #0x45e71c
0045e700 0800a0e1 mov r0, r8
0045e704 161e87e2 add r1, r7, #0x160
0045e708 0120a0e3 mov r2, #1
0045e70c a8d5fceb bl #0x393db4
0045e710 0800a0e1 mov r0, r8
0045e714 ddd5fceb bl #0x393e90
0045e718 dfffffea b #0x45e69c
0045e71c 0520a0e1 mov r2, r5
0045e720 400096e5 ldr r0, [r6, #0x40]
0045e724 0110a0e3 mov r1, #1
0045e728 0540fceb bl #0x36e744
0045e72c 605690e5 ldr r5, [r0, #0x660]
0045e730 000055e3 cmp r5, #0
0045e734 1400000a beq #0x45e78c
0045e738 640197e5 ldr r0, [r7, #0x164]
0045e73c 0010a0e3 mov r1, #0
0045e740 17c1faeb bl #0x30eba4
0045e744 0010a0e3 mov r1, #0
0045e748 0060a0e1 mov r6, r0
0045e74c 680197e5 ldr r0, [r7, #0x168]
0045e750 13c1faeb bl #0x30eba4
0045e754 c314a0e3 mov r1, #0xc3000000
0045e758 00a0a0e1 mov sl, r0
0045e75c 121781e2 add r1, r1, #0x480000
0045e760 600197e5 ldr r0, [r7, #0x160]
0045e764 0ec1faeb bl #0x30eba4
0045e768 20108de2 add r1, sp, #0x20
0045e76c 20008de5 str r0, [sp, #0x20]
0045e770 0120a0e3 mov r2, #1
0045e774 0500a0e1 mov r0, r5
0045e778 24608de5 str r6, [sp, #0x24]
0045e77c 28a08de5 str sl, [sp, #0x28]
0045e780 8bd5fceb bl #0x393db4
0045e784 0500a0e1 mov r0, r5
0045e788 c0d5fceb bl #0x393e90
0045e78c 0b3094e7 ldr r3, [r4, fp]
0045e790 0210a0e3 mov r1, #2
0045e794 0020a0e3 mov r2, #0
0045e798 400093e5 ldr r0, [r3, #0x40]
0045e79c e83ffceb bl #0x36e744
0045e7a0 605690e5 ldr r5, [r0, #0x660]
0045e7a4 000055e3 cmp r5, #0
0045e7a8 1400000a beq #0x45e800
0045e7ac 640197e5 ldr r0, [r7, #0x164]
0045e7b0 0010a0e3 mov r1, #0
0045e7b4 fac0faeb bl #0x30eba4
0045e7b8 0010a0e3 mov r1, #0
0045e7bc 0060a0e1 mov r6, r0
0045e7c0 680197e5 ldr r0, [r7, #0x168]
0045e7c4 f6c0faeb bl #0x30eba4
0045e7c8 4314a0e3 mov r1, #0x43000000
0045e7cc 00a0a0e1 mov sl, r0
0045e7d0 121781e2 add r1, r1, #0x480000
0045e7d4 600197e5 ldr r0, [r7, #0x160]
0045e7d8 f1c0faeb bl #0x30eba4
0045e7dc 14108de2 add r1, sp, #0x14
0045e7e0 14008de5 str r0, [sp, #0x14]
0045e7e4 0120a0e3 mov r2, #1
0045e7e8 0500a0e1 mov r0, r5
0045e7ec 18608de5 str r6, [sp, #0x18]
0045e7f0 1ca08de5 str sl, [sp, #0x1c]
0045e7f4 6ed5fceb bl #0x393db4
0045e7f8 0500a0e1 mov r0, r5
0045e7fc a3d5fceb bl #0x393e90
0045e800 0b3094e7 ldr r3, [r4, fp]
0045e804 0310a0e3 mov r1, #3
0045e808 0020a0e3 mov r2, #0
0045e80c 400093e5 ldr r0, [r3, #0x40]
0045e810 cb3ffceb bl #0x36e744
0045e814 605690e5 ldr r5, [r0, #0x660]
0045e818 000055e3 cmp r5, #0
0045e81c b7ffff0a beq #0x45e700
0045e820 c314a0e3 mov r1, #0xc3000000
0045e824 640197e5 ldr r0, [r7, #0x164]
0045e828 121781e2 add r1, r1, #0x480000
0045e82c dcc0faeb bl #0x30eba4
0045e830 0010a0e3 mov r1, #0
0045e834 0060a0e1 mov r6, r0
0045e838 680197e5 ldr r0, [r7, #0x168]
0045e83c d8c0faeb bl #0x30eba4
0045e840 0010a0e3 mov r1, #0
0045e844 00a0a0e1 mov sl, r0
0045e848 600197e5 ldr r0, [r7, #0x160]
0045e84c d4c0faeb bl #0x30eba4
0045e850 08108de2 add r1, sp, #8
0045e854 08008de5 str r0, [sp, #8]
0045e858 0120a0e3 mov r2, #1
0045e85c 0500a0e1 mov r0, r5
0045e860 0c608de5 str r6, [sp, #0xc]
0045e864 10a08de5 str sl, [sp, #0x10]
0045e868 51d5fceb bl #0x393db4
0045e86c 0500a0e1 mov r0, r5
0045e870 86d5fceb bl #0x393e90
0045e874 a1ffffea b #0x45e700
0045e878 a4befaeb bl #0x30e310
0045e87c b8645300 ldrheq r6, [r3], #-0x48
0045e880 ac400000 andeq r4, r0, ip, lsr #1
0045e884 84080000 andeq r0, r0, r4, lsl #17
0045e888 70ea4600 subeq lr, r6, r0, ror sl
0045e88c f4370000 strdeq r3, r4, [r0], -r4
