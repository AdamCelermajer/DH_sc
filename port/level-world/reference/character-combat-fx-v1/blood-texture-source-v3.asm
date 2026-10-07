
setTexture 005b26f0
005b26f0 push {r4, r5, r6, r7, r8, lr}
005b26f4 mov r4, r0
005b26f8 ldr r0, [r0, #0x4c]
005b26fc mov r5, r1
005b2700 mov r6, r2
005b2704 cmp r1, r0
005b2708 mov r7, r3
005b270c bhs #0x5b277c
005b2710 add r3, r3, #0x21
005b2714 add r3, r4, r3, lsl #5
005b2718 ldr r8, [r3, r1, lsl #2]
005b271c cmp r8, r2
005b2720 beq #0x5b2784
005b2724 cmp r2, #0
005b2728 str r2, [r3, r5, lsl #2]
005b272c beq #0x5b27a4
005b2730 ldr r3, [r4, #0x84]
005b2734 ldr r2, [r4, #0x268]
005b2738 add r3, r3, #1
005b273c cmp r1, r2
005b2740 str r3, [r4, #0x84]
005b2744 beq #0x5b2758
005b2748 add r0, r1, #0x8400
005b274c add r0, r0, #0xc0
005b2750 bl #0x30e1e4
005b2754 str r5, [r4, #0x268]
005b2758 ldrb r1, [r6, #0x3f]
005b275c and r1, r1, #8
005b2760 uxtb r1, r1
005b2764 cmp r1, #0
005b2768 bne #0x5b27ac
005b276c mov r0, r6
005b2770 bl #0x5fde9c
005b2774 mov r0, #1
005b2778 pop {r4, r5, r6, r7, r8, pc}
005b277c mov r0, #0
005b2780 pop {r4, r5, r6, r7, r8, pc}
005b2784 cmp r8, #0
005b2788 beq #0x5b27a4
005b278c ldrh r3, [r8, #0x40]
005b2790 bic r3, r3, #2
005b2794 lsl r3, r3, #0x13
005b2798 lsr r3, r3, #0x13
005b279c cmp r3, #0
005b27a0 bne #0x5b27d8
005b27a4 mov r0, #1
005b27a8 pop {r4, r5, r6, r7, r8, pc}
005b27ac ldr r3, [pc, #0x54]
005b27b0 ldr r1, [r6, #0x54]
005b27b4 add r3, pc, r3
005b27b8 add r3, r3, #0xa4
005b27bc ldr r0, [r3, r7, lsl #2]
005b27c0 bl #0x30e7c0
005b27c4 mov r0, r6
005b27c8 mov r1, #0
005b27cc bl #0x5b044c
005b27d0 mov r0, #1
005b27d4 pop {r4, r5, r6, r7, r8, pc}
005b27d8 ldr r3, [r4, #0x268]
005b27dc cmp r1, r3
005b27e0 beq #0x5b27f4
005b27e4 add r0, r1, #0x8400
005b27e8 add r0, r0, #0xc0
005b27ec bl #0x30e1e4
005b27f0 str r5, [r4, #0x268]
005b27f4 mov r0, r8
005b27f8 mov r1, #0
005b27fc bl #0x5b044c
005b2800 mov r0, #1
005b2804 pop {r4, r5, r6, r7, r8, pc}
005b2808 eorseq sp, r2, r0, lsl #17

ITextureCtor 005fe404
005fe404 ldr ip, [pc, #0x2cc]
005fe408 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005fe40c ldr lr, [pc, #0x2c8]
005fe410 add ip, pc, ip
005fe414 mov r6, r0
005fe418 ldr lr, [ip, lr]
005fe41c sub sp, sp, #0x24
005fe420 mov r4, #0
005fe424 add lr, lr, #8
005fe428 str r4, [r6, #4]
005fe42c mov r4, r2
005fe430 str lr, [r0], #8
005fe434 add r2, sp, #0x1c
005fe438 mov r5, r3
005fe43c bl #0x32603c
005fe440 ldr r3, [r5, #0x10]
005fe444 str r3, [r6, #0x20]
005fe448 ldr r3, [r5, #0x14]
005fe44c str r3, [r6, #0x24]
005fe450 ldr r3, [r5]
005fe454 cmp r3, #1
005fe458 ldreq r2, [r5, #0x18]
005fe45c mov r3, #0
005fe460 movne r2, #1
005fe464 str r3, [r6, #0x38]
005fe468 str r3, [r6, #0x2c]
005fe46c str r3, [r6, #0x30]
005fe470 mvn r3, #0
005fe474 str r2, [r6, #0x28]
005fe478 str r4, [r6, #0x34]
005fe47c strh r3, [r6, #0x3c]
005fe480 ldrb r0, [r5, #0x1c]
005fe484 cmp r0, #0
005fe488 moveq r2, #1
005fe48c beq #0x5fe524
005fe490 ldr r3, [r5, #0x10]
005fe494 cmp r3, #0
005fe498 mvneq r2, #0
005fe49c beq #0x5fe4b0
005fe4a0 mvn r2, #0
005fe4a4 lsrs r3, r3, #1
005fe4a8 add r2, r2, #1
005fe4ac bne #0x5fe4a4
005fe4b0 ldr r3, [r5, #0x14]
005fe4b4 str r2, [sp, #0x18]
005fe4b8 cmp r3, #0
005fe4bc mvneq r1, #0
005fe4c0 beq #0x5fe4d4
005fe4c4 mvn r1, #0
005fe4c8 lsrs r3, r3, #1
005fe4cc add r1, r1, #1
005fe4d0 bne #0x5fe4c8
005fe4d4 ldr r3, [r5, #0x18]
005fe4d8 str r1, [sp, #0x14]
005fe4dc cmp r3, #0
005fe4e0 mvneq r0, #0
005fe4e4 beq #0x5fe4f8
005fe4e8 mvn r0, #0
005fe4ec lsrs r3, r3, #1
005fe4f0 add r0, r0, #1
005fe4f4 bne #0x5fe4ec
005fe4f8 cmp r1, r2
005fe4fc movhi r2, r1
005fe500 addhi r3, sp, #0x14
005fe504 addls r3, sp, #0x18
005fe508 cmp r2, r0
005fe50c str r0, [sp, #0x10]
005fe510 addlo r3, sp, #0x10
005fe514 ldr r2, [r3]
005fe518 add r2, r2, #1
005fe51c uxtb r2, r2
005fe520 sub r0, r2, #1
005fe524 strb r2, [r6, #0x3e]
005fe528 ldrb r1, [r5, #0x1d]
005fe52c mov r3, #0
005fe530 str r3, [r6, #0x4c]
005fe534 cmp r1, #0
005fe538 moveq ip, r1
005fe53c movne ip, #4
005fe540 strb ip, [r6, #0x3f]
005fe544 movw ip, #0x1ffd
005fe548 strh ip, [r6, #0x40]
005fe54c mov r1, #0
005fe550 mov ip, #0x3f800000
005fe554 str ip, [r6, #0x44]
005fe558 str r3, [r6, #0x48]
005fe55c strb r1, [r6, #0x43]
005fe560 strb r1, [r6, #0x42]
005fe564 ldr r1, [r5]
005fe568 ldr r3, [r6, #0x38]
005fe56c and r1, r1, #3
005fe570 bic r3, r3, #3
005fe574 orr r3, r1, r3
005fe578 str r3, [r6, #0x38]
005fe57c ldr r1, [r5, #8]
005fe580 bic r3, r3, #0xc
005fe584 and r1, r1, #3
005fe588 orr r3, r3, r1, lsl #2
005fe58c str r3, [r6, #0x38]
005fe590 ldr r1, [r5, #0xc]
005fe594 bic r3, r3, #0xc00
005fe598 and r1, r1, #3
005fe59c orr r3, r3, r1, lsl #10
005fe5a0 str r3, [r6, #0x38]
005fe5a4 ldr r1, [r5, #4]
005fe5a8 bic r3, r3, #0x3f0
005fe5ac and r1, r1, #0x3f
005fe5b0 orr r3, r3, r1, lsl #4
005fe5b4 str r3, [r6, #0x38]
005fe5b8 ldrb r1, [r5, #0x1c]
005fe5bc bic r3, r3, #0x3f000
005fe5c0 cmp r1, #0
005fe5c4 movne r1, #0x3000
005fe5c8 moveq r1, #0x1000
005fe5cc orr r3, r3, r1
005fe5d0 orr r3, r3, #0x8000
005fe5d4 bic r3, r3, #0xff00000
005fe5d8 bic r3, r3, #0xc0000
005fe5dc tst r3, #0x70000000
005fe5e0 str r3, [r6, #0x38]
005fe5e4 bicne r3, r3, #0x70000000
005fe5e8 strne r3, [r6, #0x38]
005fe5ec subne r0, r2, #1
005fe5f0 bl #0x30e964
005fe5f4 ldr r3, [r6, #0x38]
005fe5f8 ldrb sl, [r6, #0x3e]
005fe5fc str r0, [r6, #0x50]
005fe600 and r0, r3, #3
005fe604 cmp r0, #2
005fe608 moveq r0, #6
005fe60c movne r0, #1
005fe610 mul r0, sl, r0
005fe614 add r2, sl, #1
005fe618 add r0, r0, #0x1f
005fe61c add r0, r2, r0, lsr #5
005fe620 mov r1, #0
005fe624 lsl r0, r0, #2
005fe628 bl #0x5341a8
005fe62c mov r8, r0
005fe630 ldr r0, [r6, #0x30]
005fe634 str r8, [r6, #0x30]
005fe638 cmp r0, #0
005fe63c beq #0x5fe648
005fe640 bl #0x30e0b8
005fe644 ldr r8, [r6, #0x30]
005fe648 ldmib r5, {r3, fp}
005fe64c cmp fp, #1
005fe650 movne fp, #0
005fe654 moveq fp, #1
005fe658 str r3, [sp, #0xc]
005fe65c cmp sl, #0
005fe660 ldr sb, [r5, #0x18]
005fe664 moveq r7, sl
005fe668 beq #0x5fe6bc
005fe66c mov r4, #0
005fe670 mov r7, r4
005fe674 mov ip, r4
005fe678 str r7, [r8, r4, lsl #2]
005fe67c ldr r1, [r5, #0x10]
005fe680 ldr r2, [r5, #0x14]
005fe684 ldr r0, [sp, #0xc]
005fe688 mov r3, sb
005fe68c str ip, [sp]
005fe690 str fp, [sp, #4]
005fe694 bl #0x5edbec
005fe698 add r4, r4, #1
005fe69c uxtb ip, r4
005fe6a0 cmp ip, sl
005fe6a4 add r7, r7, r0
005fe6a8 blo #0x5fe678
005fe6ac sub sl, sl, #1
005fe6b0 uxtb sl, sl
005fe6b4 add sl, sl, #1
005fe6b8 add r8, r8, sl, lsl #2
005fe6bc mov r0, r6
005fe6c0 str r7, [r8]
005fe6c4 mov r1, #1
005fe6c8 bl #0x5fdae8
005fe6cc mov r0, r6
005fe6d0 add sp, sp, #0x24
005fe6d4 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005fe6d8 eorseq r6, sb, r0, lsl #13
005fe6dc andeq r3, r0, r4, lsl #10

getTextureParameter 005b2558
005b2558 push {r4, lr}
005b255c ldr ip, [r2, #0xc]
005b2560 mov r4, r0
005b2564 ldr r1, [r1, ip]
005b2568 cmp r1, #0
005b256c str r1, [r0]
005b2570 beq #0x5b25d8
005b2574 ldr r0, [r1, #4]
005b2578 add r0, r0, #1
005b257c str r0, [r1, #4]
005b2580 ldr r1, [r4]
005b2584 cmp r1, #0
005b2588 beq #0x5b25d8
005b258c ldrb r1, [r1, #0x3f]
005b2590 tst r1, #0x10
005b2594 beq #0x5b25d0
005b2598 ldr r0, [r3, #0xe0]
005b259c mov r1, #1
005b25a0 ldrb r2, [r2, #6]
005b25a4 sub r2, r2, #0xc
005b25a8 bl #0x5ec1b8
005b25ac subs r3, r0, #0
005b25b0 ldrne r2, [r3, #4]
005b25b4 addne r2, r2, #1
005b25b8 strne r2, [r3, #4]
005b25bc ldr r0, [r4]
005b25c0 str r3, [r4]
005b25c4 cmp r0, #0
005b25c8 beq #0x5b25d0
005b25cc bl #0x31d584
005b25d0 mov r0, r4
005b25d4 pop {r4, pc}
005b25d8 ldr r0, [r3, #0xe0]
005b25dc mov r1, #0
005b25e0 b #0x5b25a0

DriverNonGrouped 005b76c4
005b76c4 push {r4, r5, r6, r7, r8, sl, lr}
005b76c8 ldr r3, [r1]
005b76cc mov r4, r0
005b76d0 mov ip, #0
005b76d4 ubfx r0, r3, #0x10, #1
005b76d8 mov r5, r1
005b76dc bfi ip, r0, #0, #8
005b76e0 ubfx r1, r3, #0x11, #1
005b76e4 ubfx r2, r3, #0x12, #1
005b76e8 bfi ip, r1, #8, #8
005b76ec ldr lr, [r4, #0x1ec]
005b76f0 bfi ip, r2, #0x10, #8
005b76f4 ubfx r3, r3, #0x13, #1
005b76f8 bfi ip, r3, #0x18, #8
005b76fc cmp lr, ip
005b7700 sub sp, sp, #0xc
005b7704 beq #0x5b7710
005b7708 str ip, [r4, #0x1ec]
005b770c bl #0x30decc
005b7710 ldrb r3, [r4, #0x208]
005b7714 ldrb r2, [r4, #0x20b]
005b7718 ldrb r7, [r5, #7]
005b771c ldrb r6, [r5, #6]
005b7720 ldrb r8, [r5, #5]
005b7724 ldrb r0, [r5, #4]
005b7728 ldrb ip, [r4, #0x20a]
005b772c ldrb r1, [r4, #0x209]
005b7730 strb r3, [sp]
005b7734 strb ip, [sp, #2]
005b7738 strb r1, [sp, #1]
005b773c strb r2, [sp, #3]
005b7740 strb r7, [sp, #7]
005b7744 strb r6, [sp, #6]
005b7748 strb r8, [sp, #5]
005b774c strb r0, [sp, #4]
005b7750 ldm sp, {r2, r3}
005b7754 cmp r2, r3
005b7758 beq #0x5b77a8
005b775c strb r6, [r4, #0x20a]
005b7760 strb r8, [r4, #0x209]
005b7764 strb r7, [r4, #0x20b]
005b7768 strb r0, [r4, #0x208]
005b776c bl #0x30e2e0
005b7770 mov sl, r0
005b7774 mov r0, r8
005b7778 bl #0x30e2e0
005b777c mov r8, r0
005b7780 mov r0, r6
005b7784 bl #0x30e2e0
005b7788 mov r6, r0
005b778c mov r0, r7
005b7790 bl #0x30e2e0
005b7794 mov r1, r8
005b7798 mov r3, r0
005b779c mov r2, r6
005b77a0 mov r0, sl
005b77a4 bl #0x30ea00
005b77a8 ldr r6, [r5, #8]
005b77ac ldr r1, [r4, #0x20c]
005b77b0 mov r0, r6
005b77b4 bl #0x30df8c
005b77b8 cmp r0, #0
005b77bc beq #0x5b7878
005b77c0 ldr r6, [r5, #0xc]
005b77c4 ldr r1, [r4, #0x210]
005b77c8 ldr r7, [r5, #0x10]
005b77cc mov r0, r6
005b77d0 bl #0x30df8c
005b77d4 cmp r0, #0
005b77d8 bne #0x5b7858
005b77dc mov r0, r6
005b77e0 mov r1, r7
005b77e4 bl #0x30e9e8
005b77e8 str r6, [r4, #0x210]
005b77ec str r7, [r4, #0x214]
005b77f0 ldr r3, [r5]
005b77f4 ldrb r1, [r4, #0x1c8]
005b77f8 ubfx r2, r3, #0x14, #1
005b77fc cmp r1, r2
005b7800 beq #0x5b781c
005b7804 cmp r2, #0
005b7808 strb r2, [r4, #0x1c8]
005b780c bne #0x5b7888
005b7810 mov r0, #0xbd0
005b7814 bl #0x30df68
005b7818 ldr r3, [r5]
005b781c ldrb r2, [r4, #0x1f8]
005b7820 uxtb r0, r3
005b7824 cmp r2, r0
005b7828 beq #0x5b7838
005b782c strb r0, [r4, #0x1f8]
005b7830 bl #0x30e55c
005b7834 ldr r3, [r5]
005b7838 ldrb r2, [r4, #0x1f9]
005b783c ubfx r0, r3, #8, #8
005b7840 cmp r2, r0
005b7844 beq #0x5b7870
005b7848 strb r0, [r4, #0x1f9]
005b784c add sp, sp, #0xc
005b7850 pop {r4, r5, r6, r7, r8, sl, lr}
005b7854 b #0x30ed48
005b7858 mov r0, r7
005b785c ldr r1, [r4, #0x214]
005b7860 bl #0x30df8c
005b7864 cmp r0, #0
005b7868 bne #0x5b77f0
005b786c b #0x5b77dc
005b7870 add sp, sp, #0xc
005b7874 pop {r4, r5, r6, r7, r8, sl, pc}
005b7878 str r6, [r4, #0x20c]
005b787c mov r0, r6
005b7880 bl #0x30e49c
005b7884 b #0x5b77c0
005b7888 mov r0, #0xbd0
005b788c bl #0x30e544
005b7890 ldr r3, [r5]
005b7894 b #0x5b781c

addRenderPass 005dcf2c
005dcf2c push {r4, r5, r6, r7, r8, lr}
005dcf30 mov r4, r1
005dcf34 ldr r1, [pc, #0x50]
005dcf38 mov r6, r2
005dcf3c mov r5, r3
005dcf40 add r1, pc, r1
005dcf44 mov r7, r0
005dcf48 bl #0x5dc058
005dcf4c cmp r0, #0
005dcf50 beq #0x5dcf78
005dcf54 ldr r8, [r4]
005dcf58 cmp r8, #0
005dcf5c beq #0x5dcf7c
005dcf60 ldr r0, [r7, #0x90]
005dcf64 mov r1, r4
005dcf68 mov r2, r6
005dcf6c mov r3, r5
005dcf70 bl #0x5d93e8
005dcf74 mov r0, #1
005dcf78 pop {r4, r5, r6, r7, r8, pc}
005dcf7c mov r0, r7
005dcf80 bl #0x5dcdd4
005dcf84 mov r0, r8
005dcf88 pop {r4, r5, r6, r7, r8, pc}
005dcf8c ldrhteq r3, [r0], -r8

constructMaterial 0061cca8
0061cca8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061ccac ldr r5, [pc, #0x23c]
0061ccb0 ldr sb, [pc, #0x23c]
0061ccb4 sub sp, sp, #0x7c
0061ccb8 cmp r3, #0
0061ccbc add r5, pc, r5
0061ccc0 str r3, [sp, #0x14]
0061ccc4 ldr r3, [r5, sb]
0061ccc8 mov fp, r0
0061cccc ldr r0, [sp, #0xa0]
0061ccd0 ldr r3, [r3]
0061ccd4 mov r7, r1
0061ccd8 mov r6, r2
0061ccdc str r0, [sp, #0x18]
0061cce0 str r3, [sp, #0x74]
0061cce4 beq #0x61cee0
0061cce8 ldr r3, [r2, #0xd4]
0061ccec add sl, sp, #0x2c
0061ccf0 add r8, sp, #0x44
0061ccf4 ldr r4, [r3, #0x34]
0061ccf8 cmp r4, #0
0061ccfc ldrne r3, [r4, #4]
0061cd00 mov r0, r4
0061cd04 addne r3, r3, #1
0061cd08 strne r3, [r4, #4]
0061cd0c ldr r3, [r4]
0061cd10 mov lr, pc
0061cd14 ldr pc, [r3, #0x2c]
0061cd18 add r3, sp, #0x5c
0061cd1c mov r1, r0
0061cd20 add r2, sp, #0x28
0061cd24 mov r0, r3
0061cd28 str r3, [sp, #0x10]
0061cd2c bl #0x32603c
0061cd30 ldr r3, [r4]
0061cd34 ldr r1, [r7]
0061cd38 add r2, sp, #0x24
0061cd3c ldr r3, [r3, #0x38]
0061cd40 cmp r1, #0
0061cd44 ldrne r1, [r1, #0x20]
0061cd48 mov r0, sl
0061cd4c str r3, [sp, #0xc]
0061cd50 bl #0x32603c
0061cd54 mov r0, r8
0061cd58 mov r1, r4
0061cd5c mov r2, sl
0061cd60 ldr r3, [sp, #0xc]
0061cd64 blx r3
0061cd68 ldr r0, [sp, #0x40]
0061cd6c cmp r0, sl
0061cd70 beq #0x61cd80
0061cd74 cmp r0, #0
0061cd78 beq #0x61cd80
0061cd7c bl #0x310450
0061cd80 ldr r1, [sp, #0x58]
0061cd84 ldr r3, [sp, #0x54]
0061cd88 cmp r1, r3
0061cd8c beq #0x61cea8
0061cd90 ldrsb r3, [r3, #-1]
0061cd94 cmp r3, #0x5c
0061cd98 beq #0x61cdbc
0061cd9c cmp r3, #0x2f
0061cda0 beq #0x61cdbc
0061cda4 ldr r1, [pc, #0x14c]
0061cda8 mov r0, r8
0061cdac add r1, pc, r1
0061cdb0 add r2, r1, #1
0061cdb4 bl #0x320a4c
0061cdb8 ldr r1, [sp, #0x58]
0061cdbc mov r2, #1
0061cdc0 mov r3, r2
0061cdc4 ldr ip, [r4]
0061cdc8 mov r0, r4
0061cdcc mov lr, pc
0061cdd0 ldr pc, [ip, #0x1c]
0061cdd4 str r0, [sp, #0x1c]
0061cdd8 ldr r0, [r7, #4]
0061cddc add sl, sp, #0x20
0061cde0 mov r2, r7
0061cde4 ldr ip, [r0]
0061cde8 mov r1, r0
0061cdec ldr r0, [sp, #0x14]
0061cdf0 mov r3, r6
0061cdf4 str r0, [sp]
0061cdf8 ldr r0, [sp, #0x18]
0061cdfc str r0, [sp, #4]
0061ce00 mov r0, sl
0061ce04 mov lr, pc
0061ce08 ldr pc, [ip, #0x20]
0061ce0c ldr r2, [sp, #0x1c]
0061ce10 cmp r2, #0
0061ce14 beq #0x61ce2c
0061ce18 ldr r3, [r4]
0061ce1c mov r0, r4
0061ce20 ldr r1, [sp, #0x58]
0061ce24 mov lr, pc
0061ce28 ldr pc, [r3, #0x28]
0061ce2c ldr r3, [sp, #0x20]
0061ce30 mov r0, sl
0061ce34 cmp r3, #0
0061ce38 str r3, [fp]
0061ce3c ldrne r2, [r3]
0061ce40 addne r2, r2, #1
0061ce44 strne r2, [r3]
0061ce48 bl #0x310be8
0061ce4c ldr r0, [sp, #0x58]
0061ce50 cmp r0, r8
0061ce54 beq #0x61ce64
0061ce58 cmp r0, #0
0061ce5c beq #0x61ce64
0061ce60 bl #0x310450
0061ce64 ldr r0, [sp, #0x70]
0061ce68 ldr r3, [sp, #0x10]
0061ce6c cmp r0, r3
0061ce70 beq #0x61ce80
0061ce74 cmp r0, #0
0061ce78 beq #0x61ce80
0061ce7c bl #0x310450
0061ce80 mov r0, r4
0061ce84 bl #0x31d584
0061ce88 ldr r3, [r5, sb]
0061ce8c ldr r2, [sp, #0x74]
0061ce90 mov r0, fp
0061ce94 ldr r3, [r3]
0061ce98 cmp r2, r3
0061ce9c bne #0x61ceec
0061cea0 add sp, sp, #0x7c
0061cea4 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061cea8 ldr r0, [r7, #4]
0061ceac add sl, sp, #0x20
0061ceb0 mov r2, r7
0061ceb4 ldr ip, [r0]
0061ceb8 mov r1, r0
0061cebc ldr r0, [sp, #0x14]
0061cec0 mov r3, r6
0061cec4 str r0, [sp]
0061cec8 ldr r0, [sp, #0x18]
0061cecc str r0, [sp, #4]
0061ced0 mov r0, sl
0061ced4 mov lr, pc
0061ced8 ldr pc, [ip, #0x20]
0061cedc b #0x61ce2c
0061cee0 ldr r2, [sp, #0x14]
0061cee4 str r2, [fp]
0061cee8 b #0x61ce88
0061ceec bl #0x30e310
0061cef0 ldrsbteq r7, [r7], -r4
0061cef4 andeq r4, r0, ip, lsr #1
0061cef8 eoreq r3, sl, ip, lsr #29
