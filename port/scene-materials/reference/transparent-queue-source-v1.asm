
material order 0035364c
0035364c push {r4, r5, r6, lr}
00353650 mov r5, r0
00353654 bl #0x5c5d34
00353658 ldr r3, [r5, #0x10]
0035365c mov r4, r0
00353660 lsr r3, r3, r0
00353664 tst r3, #1
00353668 beq #0x353678
0035366c mov r0, r5
00353670 mov r1, r4
00353674 bl #0x5c5fd8
00353678 ldr r3, [r5, #0x18]
0035367c ldr r0, [r3, r4, lsl #2]
00353680 pop {r4, r5, r6, pc}
00353684 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00353688 ldr ip, [r0, #0x10]
0035368c sub sp, sp, #0x14
00353690 mov r6, r0
00353694 lsr ip, ip, r1
00353698 tst ip, #1
0035369c mov r7, r1
003536a0 mov r4, r2
003536a4 mov r5, r3
003536a8 beq #0x3536b0
003536ac bl #0x5c5fd8
003536b0 ldr r2, [r4, #0x10]
003536b4 ldr r3, [r6, #0x18]
003536b8 lsr r2, r2, r5
003536bc tst r2, #1
003536c0 ldr r8, [r3, r7, lsl #2]
003536c4 beq #0x3536d4
003536c8 mov r0, r4
003536cc mov r1, r5
003536d0 bl #0x5c5fd8
003536d4 ldr r3, [r4, #0x18]
003536d8 ldr r3, [r3, r5, lsl #2]
003536dc cmp r8, r3
003536e0 beq #0x3536f0
003536e4 mov r0, #0
003536e8 add sp, sp, #0x14
003536ec pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003536f0 ldr r2, [r6, #4]
003536f4 ldr r1, [r4, #4]
003536f8 mov r3, #0xc
003536fc ldr r2, [r2, #0x18]
00353700 ldr r1, [r1, #0x18]
00353704 mla r2, r3, r7, r2
00353708 mla r3, r3, r5, r1
0035370c ldrb sb, [r2, #4]
00353710 ldrb r1, [r3, #4]
00353714 cmp r1, sb
00353718 bne #0x3536e4
0035371c cmp sb, #0
00353720 beq #0x353794
00353724 ldr r8, [r2, #8]
00353728 ldr sl, [r3, #8]
0035372c ldr r2, [r8, #0x20]
00353730 ldr r3, [sl, #0x20]
00353734 cmp r2, r3
00353738 bne #0x3536e4
0035373c sub r3, sb, #1
00353740 uxtb r3, r3
00353744 mov fp, #0x34
00353748 mla r3, r3, fp, fp
0035374c mov r1, sl
00353750 str r3, [sp, #0xc]
00353754 mov r0, r8
00353758 mov r2, #0x20
0035375c bl #0x30e5e0
00353760 cmp r0, #0
00353764 bne #0x3536e4
00353768 ldr r3, [sp, #0xc]
0035376c add r0, r8, fp
00353770 add r1, sl, fp
00353774 cmp fp, r3
00353778 beq #0x353794
0035377c ldr r2, [r0, #0x20]
00353780 ldr r3, [r1, #0x20]
00353784 add fp, fp, #0x34
00353788 cmp r2, r3
0035378c bne #0x3536e4
00353790 b #0x353758
00353794 mov r0, r6
00353798 mov r1, r7
0035379c mov r2, sb
003537a0 mov r3, r4
003537a4 str r5, [sp]
003537a8 bl #0x5ca72c
003537ac b #0x3536e8
003537b0 push {r4, r5, r6, lr}
003537b4 mov r4, r1
003537b8 mov r6, r0
003537bc bl #0x5c5d34
003537c0 mov r5, r0
003537c4 mov r0, r4
003537c8 bl #0x5c5d34
003537cc mov r1, r5
003537d0 mov r3, r0
003537d4 mov r2, r4
003537d8 mov r0, r6
003537dc pop {r4, r5, r6, lr}
003537e0 b #0x353684
003537e4 push {r4, r5, r6, r7, r8, lr}
003537e8 sub sp, sp, #8
003537ec mov r5, r1
003537f0 mov r7, r0
003537f4 bl #0x5c5d34
003537f8 mov r6, r0
003537fc mov r0, r5
00353800 bl #0x5c5d34
00353804 ldr r3, [r7, #0x10]
00353808 mov r4, r0
0035380c lsr r3, r3, r6
00353810 tst r3, #1
00353814 beq #0x353824
00353818 mov r0, r7
0035381c mov r1, r6
00353820 bl #0x5c5fd8
00353824 ldr r2, [r5, #0x10]
00353828 ldr r3, [r7, #0x18]
0035382c lsr r2, r2, r4
00353830 tst r2, #1
00353834 ldr r8, [r3, r6, lsl #2]
00353838 beq #0x353848
0035383c mov r0, r5
00353840 mov r1, r4
00353844 bl #0x5c5fd8
00353848 ldr r3, [r5, #0x18]
0035384c ldr r3, [r3, r4, lsl #2]
00353850 cmp r8, r3
00353854 beq #0x353868
00353858 movhs r0, #0
0035385c movlo r0, #1
00353860 add sp, sp, #8
00353864 pop {r4, r5, r6, r7, r8, pc}
00353868 ldr r2, [r7, #4]
0035386c ldr r1, [r5, #4]
00353870 mov r3, #0xc
00353874 ldr r2, [r2, #0x18]
00353878 ldr r1, [r1, #0x18]
0035387c mla r2, r3, r6, r2
00353880 mla r3, r3, r4, r1
00353884 ldrb r2, [r2, #4]
00353888 ldrb r3, [r3, #4]
0035388c cmp r2, r3
00353890 bne #0x353858
00353894 mov r0, r7
00353898 mov r1, r6
0035389c mov r3, r5
003538a0 str r4, [sp]
003538a4 bl #0x5ca3fc
003538a8 b #0x353860

queue comparator 003538ac
003538ac push {r4, r5, r6, r7, lr}
003538b0 ldr r3, [r1, #8]
003538b4 sub sp, sp, #0xc
003538b8 mov r4, r1
003538bc cmp r3, #0
003538c0 str r3, [sp, #4]
003538c4 ldrne r2, [r3]
003538c8 mov r5, r0
003538cc addne r2, r2, #1
003538d0 strne r2, [r3]
003538d4 ldr r2, [r0, #0xc]
003538d8 ldr r3, [r1, #0xc]
003538dc cmp r2, r3
003538e0 bgt #0x353998
003538e4 beq #0x353900
003538e8 mov r5, #0
003538ec add r0, sp, #4
003538f0 bl #0x351e3c
003538f4 mov r0, r5
003538f8 add sp, sp, #0xc
003538fc pop {r4, r5, r6, r7, pc}
00353900 ldr r7, [r0, #0x10]
00353904 ldr r6, [r1, #0x10]
00353908 mov r0, r7
0035390c mov r1, r6
00353910 bl #0x30e2f8
00353914 cmp r0, #0
00353918 bne #0x353998
0035391c mov r0, r7
00353920 mov r1, r6
00353924 bl #0x30df8c
00353928 cmp r0, #0
0035392c beq #0x3538e8
00353930 ldr r0, [r5, #8]
00353934 cmp r0, #0
00353938 beq #0x3539c4
0035393c ldr r1, [sp, #4]
00353940 cmp r1, #0
00353944 beq #0x3539b4
00353948 bl #0x3537b0
0035394c cmp r0, #0
00353950 beq #0x3539a0
00353954 ldr r3, [r5]
00353958 ldr r1, [r5, #4]
0035395c mov r0, r3
00353960 ldr r3, [r3]
00353964 mov lr, pc
00353968 ldr pc, [r3, #0x20]
0035396c ldr r3, [r4]
00353970 mov r5, r0
00353974 ldr r1, [r4, #4]
00353978 mov r0, r3
0035397c ldr r3, [r3]
00353980 mov lr, pc
00353984 ldr pc, [r3, #0x20]
00353988 cmp r5, r0
0035398c movge r5, #0
00353990 movlt r5, #1
00353994 b #0x3538ec
00353998 mov r5, #1
0035399c b #0x3538ec
003539a0 ldr r0, [r5, #8]
003539a4 ldr r1, [sp, #4]
003539a8 bl #0x3537e4
003539ac mov r5, r0
003539b0 b #0x3538ec
003539b4 cmp r0, r1
003539b8 movhs r5, #0
003539bc movlo r5, #1
003539c0 b #0x3538ec
003539c4 ldr r1, [sp, #4]
003539c8 cmp r1, #0
003539cc bne #0x3539b4
003539d0 ldr r5, [r5]
003539d4 ldr r3, [r4]
003539d8 cmp r5, r3
003539dc movhs r5, #0
003539e0 movlo r5, #1
003539e4 b #0x3538ec

queue entry 00354e8c
00354e8c push {r4, r5, r6, r7, r8, lr}
00354e90 ldr r3, [r3]
00354e94 sub sp, sp, #8
00354e98 mov r4, r0
00354e9c str r3, [sp, #4]
00354ea0 cmp r3, #0
00354ea4 ldrne r0, [r3]
00354ea8 ldr r5, [sp, #0x24]
00354eac mov r6, r2
00354eb0 addne r0, r0, #1
00354eb4 ldr r2, [sp, #0x28]
00354eb8 strne r0, [r3]
00354ebc ldr r3, [sp, #4]
00354ec0 str r1, [r4]
00354ec4 ldr r1, [sp, #0x20]
00354ec8 cmp r3, #0
00354ecc stmib r4, {r1, r3}
00354ed0 ldrne r1, [r3]
00354ed4 addne r1, r1, #1
00354ed8 strne r1, [r3]
00354edc cmn r2, #0x80000001
00354ee0 strne r2, [r4, #0xc]
00354ee4 beq #0x354fa0
00354ee8 add r0, sp, #4
00354eec bl #0x310be8
00354ef0 cmp r5, #0
00354ef4 beq #0x354fbc
00354ef8 ldr r1, [r6]
00354efc ldr r0, [r5]
00354f00 bl #0x30e3ac
00354f04 ldr r1, [r6, #4]
00354f08 mov r8, r0
00354f0c ldr r0, [r5, #4]
00354f10 bl #0x30e3ac
00354f14 ldr r1, [r6, #8]
00354f18 mov r7, r0
00354f1c ldr r0, [r5, #8]
00354f20 bl #0x30e3ac
00354f24 mov r1, r8
00354f28 mov r6, r0
00354f2c mov r0, r8
00354f30 bl #0x30ed6c
00354f34 mov r1, r7
00354f38 mov r5, r0
00354f3c mov r0, r7
00354f40 bl #0x30ed6c
00354f44 mov r1, r0
00354f48 mov r0, r5
00354f4c bl #0x30eba4
00354f50 mov r1, r6
00354f54 mov r5, r0
00354f58 mov r0, r6
00354f5c bl #0x30ed6c
00354f60 mov r1, r0
00354f64 mov r0, r5
00354f68 bl #0x30eba4
00354f6c ldr r3, [r4]
00354f70 mov r5, r0
00354f74 mov r0, r3
00354f78 ldr r3, [r3]
00354f7c mov lr, pc
00354f80 ldr pc, [r3, #0xd0]
00354f84 mov r1, r0
00354f88 mov r0, r5
00354f8c bl #0x30eba4
00354f90 str r0, [r4, #0x10]
00354f94 mov r0, r4
00354f98 add sp, sp, #8
00354f9c pop {r4, r5, r6, r7, r8, pc}
00354fa0 ldr r3, [r4]
00354fa4 mov r0, r3
00354fa8 ldr r3, [r3]
00354fac mov lr, pc
00354fb0 ldr pc, [r3, #0xd8]
00354fb4 str r0, [r4, #0xc]
00354fb8 b #0x354ee8
00354fbc ldr r3, [r4]
00354fc0 mov r0, r3
00354fc4 ldr r3, [r3]
00354fc8 mov lr, pc
00354fcc ldr pc, [r3, #0x38]
00354fd0 ldr r1, [r6]
00354fd4 mov r5, r0
00354fd8 ldr r0, [r0, #0x30]
00354fdc bl #0x30e3ac
00354fe0 ldr r1, [r6, #4]
00354fe4 mov r8, r0
00354fe8 ldr r0, [r5, #0x34]
00354fec bl #0x30e3ac
00354ff0 ldr r1, [r6, #8]
00354ff4 mov r7, r0

registerNodeForRendering 0058f348
0058f348 push {r4, r5, r6, r7, r8, sl, lr}
0058f34c sub sp, sp, #0xd4
0058f350 ldr ip, [sp, #0xf0]
0058f354 mov r4, r0
0058f358 mov r5, r3
0058f35c ldr r8, [sp, #0xf4]
0058f360 ldr r6, [sp, #0xf8]
0058f364 cmp ip, #8
0058f368 addls pc, pc, ip, lsl #2
0058f36c b #0x58f43c
0058f370 b #0x58f468
0058f374 b #0x58f4d8
0058f378 b #0x58f528
0058f37c b #0x58f55c
0058f380 b #0x58f610
0058f384 b #0x58f71c
0058f388 b #0x58f6b4
0058f38c b #0x58f6e8
0058f390 b #0x58f394
0058f394 ldrb r3, [r0, #0x28a]
0058f398 cmp r3, #0
0058f39c beq #0x58f750
0058f3a0 ldr r7, [r2]
0058f3a4 add r4, r0, #0x78
0058f3a8 cmp r7, #0
0058f3ac streq r1, [sp, #0x50]
0058f3b0 streq r5, [sp, #0x54]
0058f3b4 streq r7, [sp, #0x58]
0058f3b8 beq #0x58f3e0
0058f3bc ldr r3, [r7]
0058f3c0 add r3, r3, #1
0058f3c4 str r3, [r7]
0058f3c8 str r1, [sp, #0x50]
0058f3cc str r5, [sp, #0x54]
0058f3d0 str r7, [sp, #0x58]
0058f3d4 ldr r3, [r7]
0058f3d8 add r3, r3, #1
0058f3dc str r3, [r7]
0058f3e0 cmn r6, #0x80000001
0058f3e4 strne r6, [sp, #0x5c]
0058f3e8 beq #0x58f820
0058f3ec mov r0, r4
0058f3f0 add r1, sp, #0x50
0058f3f4 bl #0x352058
0058f3f8 ldr r4, [sp, #0x58]
0058f3fc cmp r4, #0
0058f400 beq #0x58f428
0058f404 ldr r3, [r4]
0058f408 sub r3, r3, #1
0058f40c cmp r3, #0
0058f410 str r3, [r4]
0058f414 bne #0x58f428
0058f418 mov r0, r4
0058f41c bl #0x5cbf78
0058f420 mov r0, r4
0058f424 bl #0x30e2b0
0058f428 cmp r7, #0
0058f42c beq #0x58f50c
0058f430 mov r0, r7
0058f434 bl #0x589de8
0058f438 b #0x58f50c
0058f43c ldr r3, [pc, #0x540]
0058f440 mov r0, #0
0058f444 add r3, pc, r3
0058f448 ldr r2, [r3]
0058f44c ldr r1, [r3, #4]
0058f450 add r2, r2, #1
0058f454 add r1, r1, #1
0058f458 str r1, [r3, #4]
0058f45c str r2, [r3]
0058f460 add sp, sp, #0xd4
0058f464 pop {r4, r5, r6, r7, r8, sl, pc}
0058f468 ldr ip, [r0, #0x3c]
0058f46c ldr r3, [r0, #0x40]
0058f470 rsb r6, ip, r3
0058f474 asrs r6, r6, #3
0058f478 beq #0x58f4a8
0058f47c ldr r2, [ip]
0058f480 cmp r2, r1
0058f484 movne r2, #0
0058f488 bne #0x58f49c
0058f48c b #0x58f43c
0058f490 ldr r0, [ip, r2, lsl #3]
0058f494 cmp r0, r1
0058f498 beq #0x58f43c
0058f49c add r2, r2, #1
0058f4a0 cmp r2, r6
0058f4a4 bne #0x58f490
0058f4a8 ldr r2, [r4, #0x44]
0058f4ac str r5, [sp, #0xa8]
0058f4b0 str r1, [sp, #0xa4]
0058f4b4 cmp r3, r2
0058f4b8 beq #0x58f83c
0058f4bc str r1, [r3]
0058f4c0 ldr r2, [sp, #0xa8]
0058f4c4 str r2, [r3, #4]
0058f4c8 ldr r3, [r4, #0x40]
0058f4cc add r3, r3, #8
0058f4d0 str r3, [r4, #0x40]
0058f4d4 b #0x58f50c
0058f4d8 add r6, sp, #0x70
0058f4dc mov r0, r6
0058f4e0 add r2, r4, #0xe8
0058f4e4 bl #0x350cf4
0058f4e8 ldr ip, [r4, #0x4c]
0058f4ec ldr r3, [r4, #0x50]
0058f4f0 cmp ip, r3
0058f4f4 beq #0x58f960
0058f4f8 ldm r6, {r0, r1, r2, r3}
0058f4fc stm ip, {r0, r1, r2, r3}
0058f500 ldr r3, [r4, #0x4c]
0058f504 add r3, r3, #0x10
0058f508 str r3, [r4, #0x4c]
0058f50c ldr r3, [pc, #0x474]
0058f510 mov r0, #1
0058f514 add r3, pc, r3
0058f518 ldr r2, [r3]
0058f51c add r2, r2, r0
0058f520 str r2, [r3]
0058f524 b #0x58f460
0058f528 ldr r3, [r0, #0x70]
0058f52c ldr r2, [r0, #0x74]
0058f530 str r5, [sp, #0xa0]
0058f534 str r1, [sp, #0x9c]
0058f538 cmp r3, r2
0058f53c beq #0x58f91c
0058f540 str r1, [r3]
0058f544 ldr r2, [sp, #0xa0]
0058f548 str r2, [r3, #4]
0058f54c ldr r3, [r0, #0x70]
0058f550 add r3, r3, #8
0058f554 str r3, [r0, #0x70]
0058f558 b #0x58f50c
0058f55c ldr r7, [r2]
0058f560 cmp r7, #0
0058f564 beq #0x58f87c
0058f568 mov r0, r7
0058f56c str r1, [sp, #0x14]
0058f570 str r2, [sp, #0x10]
0058f574 bl #0x5c5d34
0058f578 ldr r3, [r7, #4]
0058f57c mov ip, #0xc
0058f580 ldr r1, [sp, #0x14]
0058f584 ldr r3, [r3, #0x18]
0058f588 ldr r2, [sp, #0x10]
0058f58c mla r3, ip, r0, r3
0058f590 ldr r3, [r3, #8]
0058f594 ldr r3, [r3, #4]
0058f598 tst r3, #0x10000
0058f59c bne #0x58f7b8
0058f5a0 ldr r7, [r2]
0058f5a4 add r4, r4, #0x78
0058f5a8 cmp r7, #0
0058f5ac beq #0x58f880
0058f5b0 ldr r3, [r7]
0058f5b4 add r3, r3, #1
0058f5b8 str r3, [r7]
0058f5bc str r1, [sp, #0x40]
0058f5c0 str r5, [sp, #0x44]
0058f5c4 str r7, [sp, #0x48]
0058f5c8 ldr r3, [r7]
0058f5cc add r3, r3, #1
0058f5d0 str r3, [r7]
0058f5d4 cmn r6, #0x80000001
0058f5d8 strne r6, [sp, #0x4c]
0058f5dc beq #0x58f894
0058f5e0 mov r0, r4
0058f5e4 add r1, sp, #0x40
0058f5e8 bl #0x352058
0058f5ec ldr r0, [sp, #0x48]
0058f5f0 cmp r0, #0
0058f5f4 beq #0x58f5fc
0058f5f8 bl #0x589de8
0058f5fc cmp r7, #0
0058f600 beq #0x58f50c
0058f604 mov r0, r7
0058f608 bl #0x589de8
0058f60c b #0x58f50c
0058f610 ldr r7, [r2]
0058f614 cmp r7, #0
0058f618 streq r1, [sp, #0x60]
0058f61c streq r3, [sp, #0x64]
0058f620 streq r7, [sp, #0x68]
0058f624 beq #0x58f64c
0058f628 ldr r3, [r7]
0058f62c add r3, r3, #1
0058f630 str r3, [r7]
0058f634 str r1, [sp, #0x60]
0058f638 str r5, [sp, #0x64]
0058f63c str r7, [sp, #0x68]
0058f640 ldr r3, [r7]
0058f644 add r3, r3, #1
0058f648 str r3, [r7]
0058f64c cmn r6, #0x80000001
0058f650 strne r6, [sp, #0x6c]
0058f654 beq #0x58f860
0058f658 ldr r1, [r4, #0x7c]
0058f65c ldr r3, [r4, #0x80]
0058f660 cmp r1, r3
0058f664 beq #0x58f940
0058f668 ldr r3, [sp, #0x60]
0058f66c str r3, [r1]
0058f670 ldr r3, [sp, #0x64]
0058f674 str r3, [r1, #4]
0058f678 ldr r3, [sp, #0x68]
0058f67c str r3, [r1, #8]
0058f680 cmp r3, #0
0058f684 ldrne r2, [r3]
0058f688 addne r2, r2, #1
0058f68c strne r2, [r3]
0058f690 ldr r3, [sp, #0x6c]
0058f694 str r3, [r1, #0xc]
0058f698 ldr r3, [r4, #0x7c]
0058f69c add r3, r3, #0x10
0058f6a0 str r3, [r4, #0x7c]
0058f6a4 ldr r0, [sp, #0x68]
0058f6a8 cmp r0, #0
0058f6ac bne #0x58f5f8
0058f6b0 b #0x58f5fc
0058f6b4 ldr r3, [r0, #0x64]
0058f6b8 ldr r2, [r0, #0x68]
0058f6bc str r5, [sp, #0x90]
0058f6c0 str r1, [sp, #0x8c]
0058f6c4 cmp r3, r2
0058f6c8 beq #0x58f8d4
0058f6cc str r1, [r3]
0058f6d0 ldr r2, [sp, #0x90]
0058f6d4 str r2, [r3, #4]
0058f6d8 ldr r3, [r0, #0x64]
0058f6dc add r3, r3, #8
0058f6e0 str r3, [r0, #0x64]
0058f6e4 b #0x58f50c
0058f6e8 ldr r3, [r0, #0x34]
0058f6ec ldr r2, [r0, #0x38]
0058f6f0 str r5, [sp, #0x88]
0058f6f4 str r1, [sp, #0x84]
0058f6f8 cmp r3, r2
0058f6fc beq #0x58f8b0
0058f700 str r1, [r3]
0058f704 ldr r2, [sp, #0x88]
0058f708 str r2, [r3, #4]
0058f70c ldr r3, [r0, #0x34]
0058f710 add r3, r3, #8
0058f714 str r3, [r0, #0x34]
0058f718 b #0x58f50c
0058f71c ldr r3, [r0, #0x58]
0058f720 ldr r2, [r0, #0x5c]
0058f724 str r5, [sp, #0x98]
0058f728 str r1, [sp, #0x94]
0058f72c cmp r3, r2
0058f730 beq #0x58f8f8
0058f734 str r1, [r3]
0058f738 ldr r2, [sp, #0x98]
0058f73c str r2, [r3, #4]
0058f740 ldr r3, [r0, #0x58]
0058f744 add r3, r3, #8
0058f748 str r3, [r0, #0x58]
0058f74c b #0x58f50c
0058f750 ldr r3, [r2]
0058f754 add r7, r0, #0x84
0058f758 add r2, r0, #0xe8
0058f75c cmp r3, #0
0058f760 str r3, [sp, #0xb0]
0058f764 ldrne r0, [r3]
0058f768 add r4, sp, #0x2c
0058f76c addne r0, r0, #1
0058f770 strne r0, [r3]
0058f774 add r3, sp, #0xb0
0058f778 mov r0, r4
0058f77c stm sp, {r5, r8}
0058f780 str r6, [sp, #8]
0058f784 bl #0x354e8c
0058f788 mov r0, r7
0058f78c mov r1, r4
0058f790 bl #0x356b68
0058f794 ldr r0, [sp, #0x34]
0058f798 cmp r0, #0
0058f79c beq #0x58f7a4
0058f7a0 bl #0x589de8
0058f7a4 ldr r0, [sp, #0xb0]
0058f7a8 cmp r0, #0
0058f7ac beq #0x58f50c
0058f7b0 bl #0x589de8
0058f7b4 b #0x58f50c
0058f7b8 ldrb r3, [r4, #0x28a]
0058f7bc cmp r3, #0
0058f7c0 bne #0x58f5a0
0058f7c4 ldr r3, [r2]
0058f7c8 add r7, sp, #0x18
0058f7cc add r2, r4, #0xe8
0058f7d0 cmp r3, #0
0058f7d4 str r3, [sp, #0xac]
0058f7d8 ldrne r0, [r3]
0058f7dc add sl, r4, #0x84
0058f7e0 add r4, sp, #0xac
0058f7e4 addne r0, r0, #1
0058f7e8 strne r0, [r3]
0058f7ec mov r3, r4
0058f7f0 mov r0, r7
0058f7f4 stm sp, {r5, r8}
0058f7f8 str r6, [sp, #8]
0058f7fc bl #0x354e8c
0058f800 mov r0, sl
0058f804 mov r1, r7
0058f808 bl #0x356b68
0058f80c mov r0, r7
0058f810 bl #0x58d710
0058f814 mov r0, r4
0058f818 bl #0x310be8
0058f81c b #0x58f50c
0058f820 ldr r3, [sp, #0x50]
0058f824 mov r0, r3
0058f828 ldr r3, [r3]
0058f82c mov lr, pc
0058f830 ldr pc, [r3, #0xd8]
0058f834 str r0, [sp, #0x5c]
0058f838 b #0x58f3ec
0058f83c mov ip, #1
0058f840 mov r1, r3
0058f844 add r0, r4, #0x3c
0058f848 add r2, sp, #0xa4
0058f84c add r3, sp, #0xc8
0058f850 str ip, [sp, #4]
0058f854 str ip, [sp]
0058f858 bl #0x351afc
0058f85c b #0x58f50c
0058f860 ldr r3, [sp, #0x60]
0058f864 mov r0, r3
0058f868 ldr r3, [r3]
0058f86c mov lr, pc
0058f870 ldr pc, [r3, #0xd8]
0058f874 str r0, [sp, #0x6c]
0058f878 b #0x58f658
0058f87c add r4, r0, #0x78
0058f880 mov r3, #0
0058f884 str r1, [sp, #0x40]
0058f888 str r5, [sp, #0x44]
0058f88c str r3, [sp, #0x48]
0058f890 b #0x58f5d4
0058f894 ldr r3, [sp, #0x40]
0058f898 mov r0, r3
0058f89c ldr r3, [r3]
0058f8a0 mov lr, pc
0058f8a4 ldr pc, [r3, #0xd8]
0058f8a8 str r0, [sp, #0x4c]
0058f8ac b #0x58f5e0
0058f8b0 mov ip, #1
0058f8b4 mov r1, r3
0058f8b8 add r0, r0, #0x30
0058f8bc add r2, sp, #0x84
0058f8c0 add r3, sp, #0xb4
0058f8c4 str ip, [sp, #4]
0058f8c8 str ip, [sp]
0058f8cc bl #0x351afc
0058f8d0 b #0x58f50c
0058f8d4 mov ip, #1
0058f8d8 mov r1, r3
0058f8dc add r0, r0, #0x60
0058f8e0 add r2, sp, #0x8c
0058f8e4 add r3, sp, #0xb8
0058f8e8 str ip, [sp, #4]
0058f8ec str ip, [sp]
0058f8f0 bl #0x351c9c
0058f8f4 b #0x58f50c
0058f8f8 mov ip, #1
0058f8fc mov r1, r3
0058f900 add r0, r0, #0x54
0058f904 add r2, sp, #0x94
0058f908 add r3, sp, #0xbc
0058f90c str ip, [sp, #4]
0058f910 str ip, [sp]
0058f914 bl #0x351c9c
0058f918 b #0x58f50c
0058f91c mov ip, #1
0058f920 mov r1, r3
0058f924 add r0, r0, #0x6c
0058f928 add r2, sp, #0x9c
0058f92c add r3, sp, #0xc4
0058f930 str ip, [sp, #4]
0058f934 str ip, [sp]
0058f938 bl #0x351afc
0058f93c b #0x58f50c
0058f940 mov ip, #1
0058f944 add r0, r4, #0x78
0058f948 add r2, sp, #0x60
0058f94c add r3, sp, #0xc0
0058f950 str ip, [sp, #4]
0058f954 str ip, [sp]
0058f958 bl #0x351e84
0058f95c b #0x58f6a4
0058f960 mov r1, ip
0058f964 add r0, r4, #0x48
0058f968 mov ip, #1
0058f96c mov r2, r6
0058f970 add r3, sp, #0xcc
0058f974 str ip, [sp, #4]
0058f978 str ip, [sp]
0058f97c bl #0x351920
0058f980 b #0x58f50c
0058f984 subeq r7, r6, ip, lsr #8
0058f988 subeq r7, r6, ip, asr r3
0058f98c push {r4, r5, r6, r7, r8, lr}
0058f990 subs r4, r1, #0
0058f994 mov r6, r0
0058f998 beq #0x58fa00
0058f99c mov r0, r6
0058f9a0 ldr r1, [r4, #0xc]
