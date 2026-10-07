
SOURCE 003bf828
003bf828 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003bf82c ldr r4, [pc, #0x73c]
003bf830 ldr r2, [pc, #0x73c]
003bf834 sub sp, sp, #0x174
003bf838 add r4, pc, r4
003bf83c ldr r3, [r4, r2]
003bf840 cmp r1, #0
003bf844 str r2, [sp, #0x1c]
003bf848 ldr r3, [r3]
003bf84c str r1, [sp, #0x14]
003bf850 str r0, [sp, #0xc]
003bf854 str r3, [sp, #0x16c]
003bf858 beq #0x3bfee0
003bf85c ldr r3, [pc, #0x714]
003bf860 ldr ip, [sp, #0x14]
003bf864 mov r2, #0x23
003bf868 ldr r3, [r4, r3]
003bf86c add r1, ip, #0xff0
003bf870 add r0, ip, #0x560
003bf874 ldr r3, [r3]
003bf878 add r1, r1, #4
003bf87c ldr sb, [pc, #0x6f8]
003bf880 str r3, [sp, #0x20]
003bf884 bl #0x3dedb4
003bf888 asr r0, r0, #8
003bf88c bl #0x30e964
003bf890 mov r1, #0
003bf894 str r0, [sp, #0x10]
003bf898 bl #0x30e2f8
003bf89c ldr r5, [r4, sb]
003bf8a0 cmp r0, #0
003bf8a4 moveq r2, #0
003bf8a8 mov r0, r5
003bf8ac streq r2, [sp, #0x10]
003bf8b0 bl #0x337888
003bf8b4 ldr r1, [pc, #0x6c4]
003bf8b8 add r8, sp, #0x154
003bf8bc add r2, sp, #0x78
003bf8c0 mov r0, r8
003bf8c4 add r1, pc, r1
003bf8c8 ldr r6, [pc, #0x6b4]
003bf8cc bl #0x3140ec
003bf8d0 mov r1, r8
003bf8d4 mov r0, r5
003bf8d8 bl #0x337a88
003bf8dc mov r0, r8
003bf8e0 bl #0x318254
003bf8e4 add r7, sp, #0x13c
003bf8e8 add r6, pc, r6
003bf8ec mov r0, r5
003bf8f0 bl #0x337888
003bf8f4 add r2, sp, #0x74
003bf8f8 mov r0, r7
003bf8fc mov r1, r6
003bf900 bl #0x3140ec
003bf904 mov r1, r7
003bf908 mov r0, r5
003bf90c bl #0x337a88
003bf910 mov r0, r7
003bf914 bl #0x318254
003bf918 mov r0, r5
003bf91c bl #0x337888
003bf920 ldr r3, [pc, #0x660]
003bf924 add r7, sp, #0x124
003bf928 add r2, sp, #0x70
003bf92c mov r1, r6
003bf930 mov r0, r7
003bf934 str r3, [sp, #0x2c]
003bf938 bl #0x3140ec
003bf93c mov r1, r7
003bf940 mov r0, r5
003bf944 bl #0x337a88
003bf948 mov r0, r7
003bf94c bl #0x318254
003bf950 ldr ip, [sp, #0x2c]
003bf954 ldr r3, [r4, ip]
003bf958 ldr r8, [r3, #0x40]
003bf95c ldr r7, [r8, #0x6c4]
003bf960 cmp r7, #4
003bf964 bgt #0x3bfcec
003bf968 ldr r2, [sp, #0x20]
003bf96c cmp r7, #0
003bf970 ldr r2, [r2, #0xa0]
003bf974 str r2, [sp, #0x24]
003bf978 ble #0x3bfd20
003bf97c ldr r3, [pc, #0x608]
003bf980 mov r6, #0
003bf984 str r6, [sp, #0x18]
003bf988 str r3, [sp, #0x30]
003bf98c ldr r3, [pc, #0x5fc]
003bf990 mov r5, r6
003bf994 add r3, pc, r3
003bf998 str r3, [sp, #0x28]
003bf99c ldr r3, [pc, #0x5f0]
003bf9a0 add r3, pc, r3
003bf9a4 str r3, [sp, #0x34]
003bf9a8 ldr r3, [pc, #0x5e8]
003bf9ac add r3, pc, r3
003bf9b0 str r3, [sp, #0x38]
003bf9b4 ldr r3, [pc, #0x5e0]
003bf9b8 add r3, pc, r3
003bf9bc str r3, [sp, #0x3c]
003bf9c0 mov r0, r8
003bf9c4 mov r1, r5
003bf9c8 mov r2, #1
003bf9cc bl #0x36e744
003bf9d0 ldr sl, [r0, #0x660]
003bf9d4 cmp sl, #0
003bf9d8 beq #0x3bfde4
003bf9dc mov r1, sl
003bf9e0 ldr r2, [sp, #0x14]
003bf9e4 ldr r0, [sp, #0x10]
003bf9e8 bl #0x3bd918
003bf9ec add fp, sp, #0x44
003bf9f0 mov r1, #0
003bf9f4 str r0, [fp, r6]
003bf9f8 bl #0x30e4b4
003bf9fc cmp r0, #0
003bfa00 beq #0x3bfad8
003bfa04 ldr r2, [sp, #0xc]
003bfa08 cmp r2, #0
003bfa0c beq #0x3bfe78
003bfa10 ldr r1, [r2, #0x160]
003bfa14 ldr r0, [sl, #0x160]
003bfa18 bl #0x30e3ac
003bfa1c ldr ip, [sp, #0xc]
003bfa20 mov r3, r0
003bfa24 ldr r0, [sl, #0x164]
003bfa28 ldr r1, [ip, #0x164]
003bfa2c str r3, [sp, #8]
003bfa30 bl #0x30e3ac
003bfa34 ldr r3, [sp, #8]
003bfa38 mov r2, r0
003bfa3c str r2, [sp, #8]
003bfa40 mov r1, r3
003bfa44 mov r0, r3
003bfa48 bl #0x30ed6c
003bfa4c ldr r2, [sp, #8]
003bfa50 mov r3, r0
003bfa54 str r3, [sp, #8]
003bfa58 mov r1, r2
003bfa5c mov r0, r2
003bfa60 bl #0x30ed6c
003bfa64 ldr r3, [sp, #8]
003bfa68 mov r1, r0
003bfa6c mov r0, r3
003bfa70 bl #0x30eba4
003bfa74 bl #0x30e124
003bfa78 ldr r2, [sp, #0xc]
003bfa7c cmp r2, sl
003bfa80 beq #0x3bfa98
003bfa84 mov r1, r0
003bfa88 ldr r0, [sp, #0x24]
003bfa8c bl #0x30e4b4
003bfa90 cmp r0, #0
003bfa94 beq #0x3bfe30
003bfa98 ldr fp, [r4, sb]
003bfa9c ldr r2, [sp, #0x18]
003bfaa0 add sl, sp, #0x10c
003bfaa4 mov r0, fp
003bfaa8 add r2, r2, #1
003bfaac str r2, [sp, #0x18]
003bfab0 bl #0x337888
003bfab4 add r2, sp, #0x6c
003bfab8 ldr r1, [sp, #0x28]
003bfabc mov r0, sl
003bfac0 bl #0x3140ec
003bfac4 mov r0, fp
003bfac8 mov r1, sl
003bfacc bl #0x337a88
003bfad0 mov r0, sl
003bfad4 bl #0x318254
003bfad8 add r5, r5, #1
003bfadc cmp r5, r7
003bfae0 add r6, r6, #4
003bfae4 bne #0x3bf9c0
003bfae8 ldr r3, [sp, #0x18]
003bfaec cmp r3, #0
003bfaf0 beq #0x3bfd20
003bfaf4 sub r0, r3, #1
003bfaf8 bl #0x30e964
003bfafc ldr ip, [sp, #0x20]
003bfb00 add r6, sp, #0xdc
003bfb04 ldr fp, [pc, #0x494]
003bfb08 ldr r1, [ip, #0xa4]
003bfb0c bl #0x30ed6c
003bfb10 ldr sl, [r4, sb]
003bfb14 str r0, [sp, #0xc]
003bfb18 mov r5, #0
003bfb1c mov r0, sl
003bfb20 bl #0x337888
003bfb24 ldr r1, [pc, #0x478]
003bfb28 add r2, sp, #0x64
003bfb2c mov r0, r6
003bfb30 add r1, pc, r1
003bfb34 bl #0x3140ec
003bfb38 mov r0, sl
003bfb3c mov r1, r6
003bfb40 bl #0x337a88
003bfb44 mov r0, r6
003bfb48 bl #0x318254
003bfb4c ldr r3, [pc, #0x454]
003bfb50 add r2, sp, #0x44
003bfb54 add fp, pc, fp
003bfb58 add r3, pc, r3
003bfb5c str r3, [sp, #0x18]
003bfb60 ldr r3, [pc, #0x444]
003bfb64 str r2, [sp, #0x10]
003bfb68 mov sl, r7
003bfb6c add r3, pc, r3
003bfb70 str r3, [sp, #0x20]
003bfb74 ldr r3, [pc, #0x434]
003bfb78 add r3, pc, r3
003bfb7c str r3, [sp, #0x24]
003bfb80 b #0x3bfbc4
003bfb84 ldr r7, [r4, sb]
003bfb88 add r6, sp, #0xc4
003bfb8c mov r0, r7
003bfb90 bl #0x337888
003bfb94 add r2, sp, #0x60
003bfb98 mov r1, fp
003bfb9c mov r0, r6
003bfba0 bl #0x3140ec
003bfba4 mov r0, r7
003bfba8 mov r1, r6
003bfbac bl #0x337a88
003bfbb0 mov r0, r6
003bfbb4 bl #0x318254
003bfbb8 add r5, r5, #1
003bfbbc cmp r5, sl
003bfbc0 beq #0x3bfd58
003bfbc4 mov r0, r8
003bfbc8 mov r1, r5
003bfbcc mov r2, #1
003bfbd0 bl #0x36e744
003bfbd4 ldr r6, [r0, #0x660]
003bfbd8 cmp r6, #0
003bfbdc beq #0x3bfbb8
003bfbe0 mov r0, #0x42000000
003bfbe4 ldr r1, [sp, #0xc]
003bfbe8 add r0, r0, #0xc80000
003bfbec bl #0x30e3ac
003bfbf0 ldr r3, [sp, #0x10]
003bfbf4 ldr r1, [r3, r5, lsl #2]
003bfbf8 bl #0x30ed6c
003bfbfc mov r1, #0x42000000
003bfc00 add r1, r1, #0xc80000
003bfc04 bl #0x30ec94
003bfc08 mov r1, #0
003bfc0c mov r7, r0
003bfc10 bl #0x30e4b4
003bfc14 cmp r0, #0
003bfc18 beq #0x3bfb84
003bfc1c mov r1, #0x3f800000
003bfc20 mov r0, r7
003bfc24 bl #0x30eba4
003bfc28 bl #0x30e4cc
003bfc2c lsl r7, r0, #8
003bfc30 mov r1, r7
003bfc34 mov r0, r6
003bfc38 mov r2, #1
003bfc3c bl #0x3bf498
003bfc40 cmp r0, #0
003bfc44 beq #0x3bfb84
003bfc48 ldr ip, [sp, #0x2c]
003bfc4c mov r1, r6
003bfc50 ldr ip, [r4, ip]
003bfc54 ldr r0, [ip, #0x40]
003bfc58 str ip, [sp, #0x28]
003bfc5c bl #0x36effc
003bfc60 cmp r0, #0
003bfc64 beq #0x3bfb84
003bfc68 mov r0, r6
003bfc6c bl #0x3bb918
003bfc70 mov r3, r0
003bfc74 ldr r0, [sp, #0x28]
003bfc78 str r3, [sp, #8]
003bfc7c bl #0x31f594
003bfc80 ldr r3, [sp, #8]
003bfc84 ldr r2, [r0, #0x118]
003bfc88 cmp r3, r2
003bfc8c movlt r7, #0x100
003bfc90 bl #0x413e90
003bfc94 mov r1, r7
003bfc98 mov r3, r0
003bfc9c add r0, r6, #0x560
003bfca0 str r3, [sp, #8]
003bfca4 bl #0x3de7ec
003bfca8 ldr r3, [sp, #8]
003bfcac mov r7, r0
003bfcb0 ldr r1, [sp, #0x18]
003bfcb4 mov r0, r3
003bfcb8 bl #0x414678
003bfcbc ldr r2, [sp, #0x28]
003bfcc0 mov r6, r0
003bfcc4 ldr r1, [sp, #0x20]
003bfcc8 ldr r0, [r2, #0x2c]
003bfccc ldr r2, [sp, #0x24]
003bfcd0 bl #0x4c4bdc
003bfcd4 asr r1, r7, #8
003bfcd8 mov r3, r0
003bfcdc mov r2, r6
003bfce0 ldr r0, [sp, #0x14]
003bfce4 bl #0x3af0c8
003bfce8 b #0x3bfb84
003bfcec ldr r3, [pc, #0x298]
003bfcf0 ldr r3, [r4, r3]
003bfcf4 ldr r3, [r3]
003bfcf8 cmp r3, #2
003bfcfc moveq r3, #0
003bfd00 streq r3, [r3]
003bfd04 beq #0x3bfd10
003bfd08 cmp r3, #1
003bfd0c beq #0x3bff38
003bfd10 ldr r3, [sp, #0x20]
003bfd14 ldr r3, [r3, #0xa0]
003bfd18 str r3, [sp, #0x24]
003bfd1c b #0x3bf97c
003bfd20 ldr r6, [r4, sb]
003bfd24 add r5, sp, #0xac
003bfd28 mov r0, r6
003bfd2c bl #0x337888
003bfd30 ldr r1, [pc, #0x27c]
003bfd34 add r2, sp, #0x5c
003bfd38 mov r0, r5
003bfd3c add r1, pc, r1
003bfd40 bl #0x3140ec
003bfd44 mov r0, r6
003bfd48 mov r1, r5
003bfd4c bl #0x337a88
003bfd50 mov r0, r5
003bfd54 bl #0x318254
003bfd58 ldr r5, [r4, sb]
003bfd5c ldr r6, [pc, #0x254]
003bfd60 add r7, sp, #0x94
003bfd64 mov r0, r5
003bfd68 add r6, pc, r6
003bfd6c bl #0x337888
003bfd70 add r2, sp, #0x58
003bfd74 mov r0, r7
003bfd78 mov r1, r6
003bfd7c bl #0x3140ec
003bfd80 mov r1, r7
003bfd84 mov r0, r5
003bfd88 bl #0x337a88
003bfd8c mov r0, r7
003bfd90 bl #0x318254
003bfd94 add r7, sp, #0x7c
003bfd98 mov r0, r5
003bfd9c bl #0x337888
003bfda0 mov r1, r6
003bfda4 add r2, sp, #0x54
003bfda8 mov r0, r7
003bfdac bl #0x3140ec
003bfdb0 mov r0, r5
003bfdb4 mov r1, r7
003bfdb8 bl #0x337a88
003bfdbc mov r0, r7
003bfdc0 bl #0x318254
003bfdc4 ldr ip, [sp, #0x1c]
003bfdc8 ldr r2, [sp, #0x16c]
003bfdcc ldr r3, [r4, ip]
003bfdd0 ldr r3, [r3]
003bfdd4 cmp r2, r3
003bfdd8 bne #0x3bff6c
003bfddc add sp, sp, #0x174
003bfde0 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bfde4 ldr ip, [sp, #0x30]
003bfde8 ldr r3, [r4, ip]
003bfdec ldr r3, [r3]
003bfdf0 cmp r3, #2
003bfdf4 streq sl, [sl]
003bfdf8 beq #0x3bfad8
003bfdfc cmp r3, #1
003bfe00 bne #0x3bfad8
003bfe04 ldr r0, [pc, #0x1b0]
003bfe08 ldr r3, [pc, #0x1b0]
003bfe0c movw ip, #0x103
003bfe10 ldr r0, [r4, r0]
003bfe14 add r3, pc, r3
003bfe18 ldr r1, [sp, #0x38]
003bfe1c ldr r2, [sp, #0x3c]
003bfe20 add r0, r0, #0xa8
003bfe24 str ip, [sp]
003bfe28 bl #0x30e004
003bfe2c b #0x3bfad8
003bfe30 ldr r3, [r4, sb]
003bfe34 mov r2, #0
003bfe38 add sl, sp, #0xf4
003bfe3c mov r0, r3
003bfe40 str r2, [fp, r6]
003bfe44 str r3, [sp, #8]
003bfe48 bl #0x337888
003bfe4c add r2, sp, #0x68
003bfe50 ldr r1, [sp, #0x34]
003bfe54 mov r0, sl
003bfe58 bl #0x3140ec
003bfe5c ldr r3, [sp, #8]
003bfe60 mov r1, sl
003bfe64 mov r0, r3
003bfe68 bl #0x337a88
003bfe6c mov r0, sl
003bfe70 bl #0x318254
003bfe74 b #0x3bfad8
003bfe78 ldr r3, [sp, #0x14]
003bfe7c ldr r0, [sl, #0x160]
003bfe80 ldr r1, [r3, #0x160]
003bfe84 bl #0x30e3ac
003bfe88 ldr ip, [sp, #0x14]
003bfe8c mov r3, r0
003bfe90 ldr r0, [sl, #0x164]
003bfe94 ldr r1, [ip, #0x164]
003bfe98 str r3, [sp, #8]
003bfe9c bl #0x30e3ac
003bfea0 ldr r3, [sp, #8]
003bfea4 mov r2, r0
003bfea8 str r2, [sp, #8]
003bfeac mov r1, r3
003bfeb0 mov r0, r3
003bfeb4 bl #0x30ed6c
003bfeb8 ldr r2, [sp, #8]
003bfebc mov sl, r0
003bfec0 mov r1, r2
003bfec4 mov r0, r2
003bfec8 bl #0x30ed6c
003bfecc mov r1, r0
003bfed0 mov r0, sl
003bfed4 bl #0x30eba4
003bfed8 bl #0x30e124
003bfedc b #0x3bfa84
003bfee0 ldr r3, [pc, #0xa4]
003bfee4 ldr r3, [r4, r3]
003bfee8 ldr r3, [r3]
003bfeec cmp r3, #2
003bfef0 moveq r3, r1
003bfef4 streq r3, [r3]
003bfef8 beq #0x3bfdc4
003bfefc cmp r3, #1
003bff00 bne #0x3bfdc4
003bff04 ldr r0, [pc, #0xb0]
003bff08 ldr r1, [pc, #0xb4]
003bff0c ldr r2, [pc, #0xb4]
003bff10 ldr r0, [r4, r0]
003bff14 ldr r3, [pc, #0xb0]
003bff18 mov ip, #0xdb
003bff1c add r1, pc, r1
003bff20 add r2, pc, r2
003bff24 add r3, pc, r3
003bff28 add r0, r0, #0xa8
003bff2c str ip, [sp]
003bff30 bl #0x30e004
003bff34 b #0x3bfdc4
003bff38 ldr r0, [pc, #0x7c]
003bff3c ldr r1, [pc, #0x8c]
003bff40 ldr r2, [pc, #0x8c]
003bff44 ldr r0, [r4, r0]
003bff48 ldr r3, [pc, #0x88]
003bff4c mov ip, #0xf7
003bff50 add r1, pc, r1
003bff54 add r2, pc, r2
003bff58 add r3, pc, r3
003bff5c add r0, r0, #0xa8
003bff60 str ip, [sp]
003bff64 bl #0x30e004
003bff68 b #0x3bfd10
003bff6c bl #0x30e310
003bff70 subseq r5, sp, r8, asr r2
003bff74 andeq r4, r0, ip, lsr #1
003bff78 andeq r3, r0, r8, asr #5
003bff7c andeq r0, r0, r4, lsl #17
003bff80 subseq r5, r0, ip, lsl #4
003bff84 subseq r5, r0, r0, ror r0
003bff88 strdeq r3, r4, [r0], -r4
003bff8c andeq r3, r0, r0, asr #19
003bff90 subseq r4, r0, r4, asr #31
003bff94 ldrheq r4, [r0], #-0xf8
003bff98 subeq lr, pc, ip, lsr #20
003bff9c subseq r5, r0, r0, ror r1
003bffa0 subseq r4, r0, r4, lsl #28
003bffa4 subseq r4, r0, r8, lsr #28
003bffa8 subseq r4, r0, r0, ror #31
003bffac subseq r3, r0, r4, lsl #23
003bffb0 ldrsbeq r4, [r0], #-0xf0
003bffb4 subseq r4, r0, ip, lsl ip
003bffb8 ldrsheq r4, [r0], #-0xb0
003bffbc andeq r1, r0, r0, asr #19
003bffc0 subseq r4, r0, ip, ror #21
003bffc4 strheq lr, [pc], #-0x4c
003bffc8 subseq r4, r0, r0, lsr #23
003bffcc ldrsbeq r4, [r0], #-0x9c
003bffd0 subeq lr, pc, r8, lsl #9

SOURCE 003bd918
003bd918 push {r4, r5, r6, r7, r8, sl, lr}
003bd91c ldr r4, [pc, #0x13c]
003bd920 ldr r6, [pc, #0x13c]
003bd924 ldr r3, [pc, #0x13c]
003bd928 add r4, pc, r4
003bd92c ldr ip, [r4, r6]
003bd930 ldr r3, [r4, r3]
003bd934 mov r7, r1
003bd938 ldr r1, [ip]
003bd93c sub sp, sp, #0x24
003bd940 mov r5, r0
003bd944 mov r0, r2
003bd948 ldr r8, [r3]
003bd94c str r1, [sp, #0x1c]
003bd950 bl #0x3bd120
003bd954 mov sl, r0
003bd958 mov r0, r7
003bd95c bl #0x3bd120
003bd960 rsb r7, r0, sl
003bd964 ldr r0, [r8, #0x9c]
003bd968 bl #0x30e4cc
003bd96c cmp r7, r0
003bd970 movge r7, r0
003bd974 cmp r7, #0
003bd978 ldr r0, [r8, #0x8c]
003bd97c ldr r3, [r8, #0x90]
003bd980 ldr sl, [r8, #0x98]
003bd984 ldr r8, [r8, #0x94]
003bd988 ble #0x3bda48
003bd98c bl #0x30e4cc
003bd990 mul r0, r0, r7
003bd994 bl #0x30e964
003bd998 mov r1, #0x42000000
003bd99c add r1, r1, #0xc80000
003bd9a0 bl #0x30eba4
003bd9a4 mov r7, r0
003bd9a8 mov r1, r7
003bd9ac mov r0, sl
003bd9b0 bl #0x30e70c
003bd9b4 cmp r0, #0
003bd9b8 moveq r7, sl
003bd9bc mov r1, r7
003bd9c0 mov r0, r8
003bd9c4 bl #0x30e2f8
003bd9c8 cmp r0, #0
003bd9cc moveq r7, r8
003bd9d0 mov r1, #0x42000000
003bd9d4 mov r0, r7
003bd9d8 add r1, r1, #0xc80000
003bd9dc bl #0x30ec94
003bd9e0 mov r1, r5
003bd9e4 bl #0x30ed6c
003bd9e8 ldr r3, [pc, #0x7c]
003bd9ec mov r8, r0
003bd9f0 add r5, sp, #4
003bd9f4 ldr r7, [r4, r3]
003bd9f8 mov r0, r7
003bd9fc bl #0x337888
003bda00 ldr r1, [pc, #0x68]
003bda04 mov r2, sp
003bda08 mov r0, r5
003bda0c add r1, pc, r1
003bda10 bl #0x3140ec
003bda14 mov r1, r5
003bda18 mov r0, r7
003bda1c bl #0x337a88
003bda20 mov r0, r5
003bda24 bl #0x318254
003bda28 ldr r3, [r4, r6]
003bda2c ldr r2, [sp, #0x1c]
003bda30 mov r0, r8
003bda34 ldr r3, [r3]
003bda38 cmp r2, r3
003bda3c bne #0x3bda5c
003bda40 add sp, sp, #0x24
003bda44 pop {r4, r5, r6, r7, r8, sl, pc}
003bda48 moveq r7, #0x42000000
003bda4c addeq r7, r7, #0xc80000
003bda50 beq #0x3bd9a8
003bda54 mov r0, r3
003bda58 b #0x3bd98c
003bda5c bl #0x30e310
003bda60 subseq r7, sp, r8, ror #2
003bda64 andeq r4, r0, ip, lsr #1
003bda68 andeq r3, r0, r8, asr #5
003bda6c andeq r0, r0, r4, lsl #17
003bda70 subseq r6, r0, ip, asr #30

SOURCE 003a5b18
003a5b18 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a5b1c sub sp, sp, #0xb4
003a5b20 ldr r3, [r0]
003a5b24 mov r6, r2
003a5b28 mov r4, r0
003a5b2c mov r7, r1
003a5b30 mov lr, pc
003a5b34 ldr pc, [r3, #0x34]
003a5b38 ldr r5, [pc, #0x664]
003a5b3c subs r2, r0, #0
003a5b40 add r5, pc, r5
003a5b44 beq #0x3a5b50
003a5b48 add sp, sp, #0xb4
003a5b4c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003a5b50 movw r3, #0x1449
003a5b54 mov sl, #1
003a5b58 add r8, r4, #0x560
003a5b5c strb sl, [r4, r3]
003a5b60 mov r0, r8
003a5b64 mov r1, #0x24
003a5b68 bl #0x3e07a0
003a5b6c ldr r3, [r4]
003a5b70 mov r0, r4
003a5b74 mov lr, pc
003a5b78 ldr pc, [r3, #0x28]
003a5b7c cmp r0, #0
003a5b80 beq #0x3a5be0
003a5b84 cmp r6, #0
003a5b88 bne #0x3a5be0
003a5b8c ldr sb, [pc, #0x614]
003a5b90 mov r2, sl
003a5b94 mov r0, r8
003a5b98 mov r1, #0x19
003a5b9c bl #0x3e0798
003a5ba0 ldr r3, [r5, sb]
003a5ba4 mov r1, r4
003a5ba8 ldr r0, [r3, #0x40]
003a5bac bl #0x36effc
003a5bb0 cmp r0, #0
003a5bb4 bne #0x3a602c
003a5bb8 bl #0x7fd794
003a5bbc ldrb r3, [r0, #5]
003a5bc0 cmp r3, #0
003a5bc4 beq #0x3a5b48
003a5bc8 ldr r3, [r5, sb]
003a5bcc mov r1, #0
003a5bd0 mov r2, #1
003a5bd4 ldr r0, [r3, #0x40]
003a5bd8 bl #0x36e478
003a5bdc b #0x3a5b48
003a5be0 ldr sb, [pc, #0x5c0]
003a5be4 ldr r0, [r5, sb]
003a5be8 bl #0x31f594
003a5bec ldr r3, [r0, #0x150]
003a5bf0 cmp r3, #0
003a5bf4 beq #0x3a601c
003a5bf8 cmp r7, #0
003a5bfc beq #0x3a609c
003a5c00 cmp r6, #0
003a5c04 bne #0x3a5b48
003a5c08 movw r3, #0x144c
003a5c0c str r7, [r4, r3]
003a5c10 ldr r3, [pc, #0x594]
003a5c14 ldr r2, [pc, #0x594]
003a5c18 cmp r7, r4
003a5c1c movne fp, #0
003a5c20 moveq fp, #1
003a5c24 add r3, pc, r3
003a5c28 str r3, [sp, #0x24]
003a5c2c ldr r3, [pc, #0x580]
003a5c30 add r8, r4, #0x3c8
003a5c34 str r2, [sp, #0x14]
003a5c38 add r3, pc, r3
003a5c3c str r3, [sp, #0x20]
003a5c40 ldr r3, [pc, #0x570]
003a5c44 mov sl, r7
003a5c48 add r3, pc, r3
003a5c4c str r3, [sp, #0x1c]
003a5c50 ldr r3, [pc, #0x564]
003a5c54 add r3, pc, r3
003a5c58 str r3, [sp, #0x18]
003a5c5c mov r0, r8
003a5c60 bl #0x3d4a10
003a5c64 cmp r6, r0
003a5c68 bge #0x3a5de8
003a5c6c mov ip, #0
003a5c70 str ip, [sp, #0xac]
003a5c74 mov r0, r8
003a5c78 mov ip, #0
003a5c7c mov r1, r6
003a5c80 add r2, sp, #0xac
003a5c84 add r3, sp, #0xa8
003a5c88 str ip, [sp, #0xa8]
003a5c8c bl #0x3d72d0
003a5c90 ldr r7, [sp, #0xac]
003a5c94 cmp r7, #0
003a5c98 beq #0x3a5d40
003a5c9c movw r3, #0x14d4
003a5ca0 ldr r0, [r7, r3]
003a5ca4 cmp r0, #0
003a5ca8 strne r0, [sp, #0xac]
003a5cac beq #0x3a5de0
003a5cb0 mov r1, #4
003a5cb4 mov r2, r4
003a5cb8 bl #0x3a4d5c
003a5cbc ldr r3, [sp, #0xac]
003a5cc0 cmp r3, #0
003a5cc4 beq #0x3a5d40
003a5cc8 mov r0, r3
003a5ccc ldr r3, [r3]
003a5cd0 mov lr, pc
003a5cd4 ldr pc, [r3, #0x28]
003a5cd8 cmp r0, #0
003a5cdc beq #0x3a5d40
003a5ce0 ldr r0, [sp, #0xac]
003a5ce4 movw r3, #0x14a4
003a5ce8 mov r1, #0x17
003a5cec ldr r2, [r0, r3]
003a5cf0 cmp r4, r2
003a5cf4 moveq r2, #0
003a5cf8 streq r2, [r0, r3]
003a5cfc ldreq r0, [sp, #0xac]
003a5d00 mov r2, #1
003a5d04 add r0, r0, #0x560
003a5d08 bl #0x3e0798
003a5d0c ldr r0, [sp, #0xac]
003a5d10 mov r1, #0x18
003a5d14 mov r2, #1
003a5d18 add r0, r0, #0x560
003a5d1c bl #0x3e0798
003a5d20 ldr r3, [r5, sb]
003a5d24 cmp sl, r7
003a5d28 ldr r1, [sp, #0xac]
003a5d2c ldr r0, [r3, #0x40]
003a5d30 moveq fp, #1
003a5d34 bl #0x36effc
003a5d38 cmp r0, #0
003a5d3c bne #0x3a5d48
003a5d40 add r6, r6, #1
003a5d44 b #0x3a5c5c
003a5d48 ldr r2, [sp, #0x14]
003a5d4c ldr r0, [sp, #0xac]
003a5d50 mov r1, #0x17
003a5d54 ldr r3, [r5, r2]
003a5d58 add r0, r0, #0x560
003a5d5c mov r2, #0
003a5d60 ldr r7, [r3]
003a5d64 bl #0x3df6e0
003a5d68 cmp r0, #0x64
003a5d6c beq #0x3a60fc
003a5d70 ldr r0, [sp, #0xac]
003a5d74 mov r1, #0x17
003a5d78 mov r2, #0
003a5d7c add r0, r0, #0x560
003a5d80 bl #0x3df6e0
003a5d84 cmp r0, #0x1f4
003a5d88 beq #0x3a6118
003a5d8c ldr r0, [sp, #0xac]
003a5d90 mov r1, #0x17
003a5d94 mov r2, #0
003a5d98 add r0, r0, #0x560
003a5d9c bl #0x3df6e0
003a5da0 cmp r0, #0x3e8
003a5da4 beq #0x3a60a8
003a5da8 ldr r0, [sp, #0xac]
003a5dac mov r1, #0x17
003a5db0 mov r2, #0
003a5db4 add r0, r0, #0x560
003a5db8 bl #0x3df6e0
003a5dbc cmp r0, #0x7d0
003a5dc0 bne #0x3a5d40
003a5dc4 ldr r0, [sp, #0x24]
003a5dc8 bl #0x3a3f70
003a5dcc mov r1, r0
003a5dd0 mov r0, r7
003a5dd4 bl #0x3813b8
003a5dd8 add r6, r6, #1
003a5ddc b #0x3a5c5c
003a5de0 mov r0, r7
003a5de4 b #0x3a5cb0
003a5de8 cmp fp, #0
003a5dec mov r7, sl
003a5df0 beq #0x3a5e1c
003a5df4 movw r8, #0x144c
003a5df8 ldr r3, [r4, r8]
003a5dfc mov r0, r3
003a5e00 ldr r3, [r3]
003a5e04 mov lr, pc
003a5e08 ldr pc, [r3, #0x24]
003a5e0c cmp r0, #0
003a5e10 bne #0x3a60c4
003a5e14 mov r1, r4
003a5e18 bl #0x3bf828
003a5e1c ldr r3, [r4]
003a5e20 mov r0, r4
003a5e24 mov lr, pc
003a5e28 ldr pc, [r3, #0x54]
003a5e2c cmp r0, #0
003a5e30 bne #0x3a5b48
003a5e34 movw r3, #0x14e4
003a5e38 ldrb r3, [r4, r3]
003a5e3c cmp r3, #0
003a5e40 bne #0x3a5b48
003a5e44 ldr r0, [r5, sb]
003a5e48 bl #0x31f594
003a5e4c cmp r0, #0
003a5e50 str r0, [sp, #0x14]
003a5e54 beq #0x3a6150
003a5e58 ldr r8, [pc, #0x360]
003a5e5c movw r3, #0x13c8
003a5e60 ldr sl, [r5, sb]
003a5e64 ldr r2, [pc, #0x358]
003a5e68 ldrsh ip, [r4, r3]
003a5e6c add r8, pc, r8
003a5e70 ldr r0, [sl, #0x2c]
003a5e74 add r2, pc, r2
003a5e78 mov r1, r8
003a5e7c ldr fp, [r4, #0x64]
003a5e80 str r3, [sp, #0x10]
003a5e84 str ip, [sp, #0xc]
003a5e88 bl #0x4c4bdc
003a5e8c ldr r2, [pc, #0x334]
003a5e90 ldr ip, [sp, #0xc]
003a5e94 mov r6, #0
003a5e98 ldr r2, [r5, r2]
003a5e9c mvn sb, #0
003a5ea0 str r0, [sp, #0x84]
003a5ea4 add r2, r2, #8
003a5ea8 ldr r0, [sp, #0x14]
003a5eac add r1, sp, #0x80
003a5eb0 str fp, [sp, #0x8c]
003a5eb4 ldr fp, [pc, #0x310]
003a5eb8 str r2, [sp, #0x80]
003a5ebc str ip, [sp, #0x98]
003a5ec0 str r7, [sp, #0x88]
003a5ec4 strb r6, [sp, #0x90]
003a5ec8 strb r6, [sp, #0x91]
003a5ecc str sb, [sp, #0x94]
003a5ed0 bl #0x339090
003a5ed4 ldr r3, [sp, #0x10]
003a5ed8 ldr fp, [r5, fp]
003a5edc ldr r2, [pc, #0x2ec]
003a5ee0 ldrsh r3, [r4, r3]
003a5ee4 add fp, fp, #8
003a5ee8 str fp, [sp, #0x80]
003a5eec str r3, [sp, #0x18]
003a5ef0 ldr ip, [r4, #0x64]
003a5ef4 add r2, pc, r2
003a5ef8 mov r1, r8
003a5efc ldr r0, [sl, #0x2c]
003a5f00 str ip, [sp, #0xc]
003a5f04 bl #0x4c4bdc
003a5f08 ldr r3, [pc, #0x2c4]
003a5f0c ldr ip, [sp, #0xc]
003a5f10 str r0, [sp, #0x68]
003a5f14 ldr r3, [r5, r3]
003a5f18 add r1, sp, #0x64
003a5f1c ldr r0, [sp, #0x14]
003a5f20 add r3, r3, #8
003a5f24 str r3, [sp, #0x64]
003a5f28 ldr r3, [sp, #0x18]
003a5f2c str ip, [sp, #0x70]
003a5f30 str r7, [sp, #0x6c]
003a5f34 str r3, [sp, #0x7c]
003a5f38 strb r6, [sp, #0x74]
003a5f3c strb r6, [sp, #0x75]
003a5f40 str sb, [sp, #0x78]
003a5f44 bl #0x339090
003a5f48 movw r2, #0x13ca
003a5f4c ldrsh ip, [r4, r2]
003a5f50 str fp, [sp, #0x64]
003a5f54 cmp ip, sb
003a5f58 beq #0x3a5b48
003a5f5c ldr r2, [pc, #0x274]
003a5f60 ldr r3, [r4, #0x64]
003a5f64 mov r1, r8
003a5f68 ldr r0, [sl, #0x2c]
003a5f6c add r2, pc, r2
003a5f70 str r3, [sp, #0x18]
003a5f74 str ip, [sp, #0xc]
003a5f78 bl #0x4c4bdc
003a5f7c ldr r3, [pc, #0x258]
003a5f80 ldr r2, [sp, #0x18]
003a5f84 ldr ip, [sp, #0xc]
003a5f88 ldr r3, [r5, r3]
003a5f8c str r0, [sp, #0x4c]
003a5f90 add r1, sp, #0x48
003a5f94 ldr r0, [sp, #0x14]
003a5f98 add r3, r3, #8
003a5f9c str r2, [sp, #0x54]
003a5fa0 str r3, [sp, #0x48]
003a5fa4 str ip, [sp, #0x60]
003a5fa8 str r7, [sp, #0x50]
003a5fac strb r6, [sp, #0x58]
003a5fb0 strb r6, [sp, #0x59]
003a5fb4 str sb, [sp, #0x5c]
003a5fb8 bl #0x339090
003a5fbc ldr r2, [pc, #0x21c]
003a5fc0 str fp, [sp, #0x48]
003a5fc4 mov r1, r8
003a5fc8 movw r3, #0x13ca
003a5fcc ldr r0, [sl, #0x2c]
003a5fd0 add r2, pc, r2
003a5fd4 ldrsh r8, [r4, r3]
003a5fd8 ldr r4, [r4, #0x64]
003a5fdc bl #0x4c4bdc
003a5fe0 ldr r3, [pc, #0x1fc]
003a5fe4 str r0, [sp, #0x30]
003a5fe8 add r1, sp, #0x2c
003a5fec ldr r3, [r5, r3]
003a5ff0 ldr r0, [sp, #0x14]
003a5ff4 str r7, [sp, #0x34]
003a5ff8 add r3, r3, #8
003a5ffc str r4, [sp, #0x38]
003a6000 strb r6, [sp, #0x3d]
003a6004 str sb, [sp, #0x40]
003a6008 str r3, [sp, #0x2c]
003a600c str r8, [sp, #0x44]
003a6010 strb r6, [sp, #0x3c]
003a6014 bl #0x339090
003a6018 b #0x3a5b48
003a601c mov r0, r4
003a6020 mov r1, r7
003a6024 bl #0x3a5ae4
003a6028 b #0x3a5bf8
003a602c ldr r3, [pc, #0x17c]
003a6030 mov r0, r8
003a6034 mov r1, #0x19
003a6038 ldr r3, [r5, r3]
003a603c mov r2, r6
003a6040 ldr r4, [r3]
003a6044 bl #0x3df6e0
003a6048 cmp r0, #0xa
003a604c beq #0x3a60e0
003a6050 mov r0, r8
003a6054 mov r1, #0x19
003a6058 mov r2, r6
003a605c bl #0x3df6e0
003a6060 cmp r0, #0x32
003a6064 beq #0x3a6134
003a6068 mov r0, r8
003a606c mov r2, r6
003a6070 mov r1, #0x19
003a6074 bl #0x3df6e0
003a6078 cmp r0, #0x64
003a607c bne #0x3a5bb8
003a6080 ldr r0, [pc, #0x160]
003a6084 add r0, pc, r0
003a6088 bl #0x3a3f70
003a608c mov r1, r0
003a6090 mov r0, r4
003a6094 bl #0x3813b8
003a6098 b #0x3a5bb8
003a609c cmp r6, #0
003a60a0 bne #0x3a5b48
003a60a4 b #0x3a5e1c
003a60a8 ldr r0, [sp, #0x20]
003a60ac bl #0x3a3f70
003a60b0 mov r1, r0
003a60b4 mov r0, r7
003a60b8 bl #0x3813b8
003a60bc add r6, r6, #1
003a60c0 b #0x3a5c5c
003a60c4 add r6, sp, #0x9c
003a60c8 mov r0, r6
003a60cc ldr r1, [r4, r8]
003a60d0 bl #0x33dd2c
003a60d4 mov r0, r6
003a60d8 bl #0x33ff54
003a60dc b #0x3a5e14
003a60e0 ldr r0, [pc, #0x104]
003a60e4 add r0, pc, r0
003a60e8 bl #0x3a3f70
003a60ec mov r1, r0
003a60f0 mov r0, r4
003a60f4 bl #0x3813b8
003a60f8 b #0x3a5bb8
003a60fc ldr r0, [sp, #0x18]
003a6100 bl #0x3a3f70
003a6104 mov r1, r0
003a6108 mov r0, r7
003a610c bl #0x3813b8
003a6110 add r6, r6, #1
003a6114 b #0x3a5c5c
003a6118 ldr r0, [sp, #0x1c]
003a611c bl #0x3a3f70
003a6120 mov r1, r0
003a6124 mov r0, r7
003a6128 bl #0x3813b8
003a612c add r6, r6, #1
003a6130 b #0x3a5c5c
003a6134 ldr r0, [pc, #0xb4]
003a6138 add r0, pc, r0
003a613c bl #0x3a3f70
003a6140 mov r1, r0
003a6144 mov r0, r4
003a6148 bl #0x3813b8
003a614c b #0x3a5bb8
003a6150 ldr r3, [pc, #0x9c]
003a6154 ldr r3, [r5, r3]
003a6158 ldr r3, [r3]
003a615c cmp r3, #2
003a6160 streq r0, [r0]
003a6164 beq #0x3a5e58
003a6168 cmp r3, #1
003a616c bne #0x3a5e58
003a6170 ldr r0, [pc, #0x80]
003a6174 ldr r1, [pc, #0x80]
003a6178 ldr r2, [pc, #0x80]
003a617c ldr r0, [r5, r0]
003a6180 ldr r3, [pc, #0x7c]
003a6184 movw ip, #0x2d3
003a6188 add r1, pc, r1
003a618c add r2, pc, r2
003a6190 add r3, pc, r3
003a6194 add r0, r0, #0xa8
003a6198 str ip, [sp]
003a619c bl #0x30e004
003a61a0 b #0x3a5e58
003a61a4 subseq lr, lr, r0, asr pc
003a61a8 strdeq r3, r4, [r0], -r4
003a61ac subseq sp, r1, ip, lsr r7
003a61b0 andeq r1, r0, r0, ror sp
003a61b4 subseq sp, r1, r8, lsl r7
003a61b8 ldrsheq sp, [r1], #-0x68
003a61bc ldrsbeq sp, [r1], #-0x6c
003a61c0 ldrsheq ip, [r1], #-0xac
003a61c4 ldrsheq sp, [r1], #-0x4c
003a61c8 ldrdeq r3, r4, [r0], -r4
003a61cc strheq r0, [r0], -r0
003a61d0 subseq sp, r1, ip, lsl #9
003a61d4 strheq r4, [r0], -r4
003a61d8 subseq sp, r1, r4, lsr #8
003a61dc andeq r2, r0, r0, ror #29
003a61e0 ldrsbeq sp, [r1], #-0x38
003a61e4 andeq r4, r0, r4, lsr #11

SOURCE 003de7ec
003de7ec ldr r2, [r0, #0xdb8]
003de7f0 movw r3, #0x851f
003de7f4 movt r3, #0x51eb
003de7f8 add r2, r2, #0x6400
003de7fc smull r0, r3, r3, r2
003de800 asr r2, r2, #0x1f
003de804 rsb r3, r2, r3, asr #5
003de808 mul r3, r3, r1
003de80c asr r0, r3, #8
003de810 bx lr

SOURCE 003e087c
003e087c push {r4, r5, r6, lr}
003e0880 mov r4, r0
003e0884 mov r5, r1
003e0888 bl #0x3defbc
003e088c mov r0, r4
003e0890 mov r1, r5
003e0894 bl #0x3df2a4
003e0898 mov r0, r4
003e089c mov r1, #1
003e08a0 pop {r4, r5, r6, lr}
003e08a4 b #0x3e0810

SOURCE 003bf498
003bf498 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003bf49c ldr r4, [pc, #0x344]
003bf4a0 ldr r6, [pc, #0x344]
003bf4a4 ldr sb, [pc, #0x344]
003bf4a8 add r4, pc, r4
003bf4ac ldr r3, [r4, r6]
003bf4b0 sub sp, sp, #0x54
003bf4b4 ldr r7, [pc, #0x338]
003bf4b8 ldr sl, [r4, sb]
003bf4bc str r2, [sp, #8]
003bf4c0 ldr r2, [pc, #0x330]
003bf4c4 ldr r3, [r3]
003bf4c8 add r7, pc, r7
003bf4cc mov r5, r0
003bf4d0 add r2, pc, r2
003bf4d4 mov fp, r1
003bf4d8 ldr r0, [sl, #0x2c]
003bf4dc mov r1, r7
003bf4e0 str r3, [sp, #0x4c]
003bf4e4 bl #0x4c4bdc
003bf4e8 mov r8, r0
003bf4ec mov r0, r5
003bf4f0 bl #0x3bb918
003bf4f4 cmp r0, #1
003bf4f8 beq #0x3bf690
003bf4fc mov r0, r5
003bf500 bl #0x3bb918
003bf504 cmp r0, #2
003bf508 beq #0x3bf6ac
003bf50c add r7, r5, #0x560
003bf510 mov r0, r7
003bf514 mov r1, #0x13
003bf518 mov r2, #0
003bf51c bl #0x3df6e0
003bf520 cmp r8, r0
003bf524 bgt #0x3bf548
003bf528 mov r0, #0
003bf52c ldr r3, [r4, r6]
003bf530 ldr r2, [sp, #0x4c]
003bf534 ldr r3, [r3]
003bf538 cmp r2, r3
003bf53c bne #0x3bf7e4
003bf540 add sp, sp, #0x54
003bf544 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bf548 ldr r3, [r5]
003bf54c mov r0, r5
003bf550 mov lr, pc
003bf554 ldr pc, [r3, #0x28]
003bf558 cmp r0, #0
003bf55c beq #0x3bf528
003bf560 ldr r3, [r5]
003bf564 mov r0, r5
003bf568 mov lr, pc
003bf56c ldr pc, [r3, #0x54]
003bf570 cmp r0, #0
003bf574 bne #0x3bf528
003bf578 ldr r0, [r4, sb]
003bf57c bl #0x31f594
003bf580 ldr r3, [r0, #0x150]
003bf584 cmp r3, #0
003bf588 bne #0x3bf528
003bf58c cmp fp, #0
003bf590 blt #0x3bf790
003bf594 ldr r3, [pc, #0x260]
003bf598 add r8, sp, #0x34
003bf59c ldr sl, [r4, r3]
003bf5a0 str r3, [sp, #0xc]
003bf5a4 mov r0, sl
003bf5a8 bl #0x337888
003bf5ac ldr r1, [pc, #0x24c]
003bf5b0 add r2, sp, #0x18
003bf5b4 mov r0, r8
003bf5b8 add r1, pc, r1
003bf5bc bl #0x3140ec
003bf5c0 mov r0, sl
003bf5c4 mov r1, r8
003bf5c8 bl #0x337a88
003bf5cc mov sl, r0
003bf5d0 mov r0, r8
003bf5d4 bl #0x318254
003bf5d8 cmp sl, #0
003bf5dc bne #0x3bf76c
003bf5e0 mov r0, r5
003bf5e4 bl #0x3bb918
003bf5e8 mov sl, r0
003bf5ec ldr r0, [r4, sb]
003bf5f0 bl #0x31f594
003bf5f4 ldr r3, [r0, #0x118]
003bf5f8 mov r0, r7
003bf5fc add r8, sp, #0x1c
003bf600 cmp sl, r3
003bf604 movlt fp, #0x100
003bf608 mov r1, fp
003bf60c bl #0x3de7ec
003bf610 mov r1, #0x21
003bf614 mov r2, r0
003bf618 mov r0, r7
003bf61c bl #0x3e0708
003bf620 ldr r3, [sp, #0xc]
003bf624 ldr sl, [r4, r3]
003bf628 mov r0, sl
003bf62c bl #0x337888
003bf630 ldr r1, [pc, #0x1cc]
003bf634 add r2, sp, #0x14
003bf638 mov r0, r8
003bf63c add r1, pc, r1
003bf640 bl #0x3140ec
003bf644 mov r1, r8
003bf648 mov r0, sl
003bf64c bl #0x337a88
003bf650 mov r0, r8
003bf654 bl #0x318254
003bf658 mov r1, #0x21
003bf65c mov r0, r7
003bf660 bl #0x3bd130
003bf664 mov r1, #0x22
003bf668 mov r8, r0
003bf66c mov r0, r7
003bf670 bl #0x3bd130
003bf674 cmp r8, r0
003bf678 bge #0x3bf6fc
003bf67c ldr r3, [sp, #8]
003bf680 cmp r3, #0
003bf684 bne #0x3bf6c8
003bf688 mov r0, #1
003bf68c b #0x3bf52c
003bf690 ldr r2, [pc, #0x170]
003bf694 ldr r0, [sl, #0x2c]
003bf698 mov r1, r7
003bf69c add r2, pc, r2
003bf6a0 bl #0x4c4bdc
003bf6a4 mov r8, r0
003bf6a8 b #0x3bf50c
003bf6ac ldr r2, [pc, #0x158]
003bf6b0 ldr r0, [sl, #0x2c]
003bf6b4 mov r1, r7
003bf6b8 add r2, pc, r2
003bf6bc bl #0x4c4bdc
003bf6c0 mov r8, r0
003bf6c4 b #0x3bf50c
003bf6c8 ldr r3, [r4, sb]
003bf6cc mov r1, r5
003bf6d0 mov r2, #0
003bf6d4 ldr r0, [r3, #0x40]
003bf6d8 bl #0x36eea8
003bf6dc ldr r1, [pc, #0x12c]
003bf6e0 ldr r3, [r0, #0x670]
003bf6e4 asr r2, fp, #8
003bf6e8 ldr r0, [r4, r1]
003bf6ec mov r1, #6
003bf6f0 bl #0x3790e0
003bf6f4 mov r0, #1
003bf6f8 b #0x3bf52c
003bf6fc mov r1, #0x21
003bf700 mov r0, r7
003bf704 bl #0x3bd130
003bf708 mov r1, #0x22
003bf70c mov r8, r0
003bf710 mov r0, r7
003bf714 bl #0x3bd130
003bf718 rsb r1, r0, r8
003bf71c asr r1, r1, #8
003bf720 mov r0, r5
003bf724 bl #0x3beb88
003bf728 mov r1, #0x21
003bf72c mov r0, r7
003bf730 bl #0x3bd130
003bf734 mov r1, #0x22
003bf738 mov r8, r0
003bf73c mov r0, r7
003bf740 bl #0x3bd130
003bf744 cmp r8, r0
003bf748 ble #0x3bf67c
003bf74c mov r1, #0x22
003bf750 mov r0, r7
003bf754 bl #0x3bd130
003bf758 mov r1, #0x21
003bf75c mov r2, r0
003bf760 mov r0, r7
003bf764 bl #0x3e07a0
003bf768 b #0x3bf67c
003bf76c mov r1, #0x22
003bf770 mov r0, r7
003bf774 bl #0x3bd130
003bf778 mov r1, #0x21
003bf77c mov fp, r0
003bf780 mov r0, r7
003bf784 bl #0x3bd130
003bf788 rsb fp, r0, fp
003bf78c b #0x3bf5e0
003bf790 ldr r2, [pc, #0x7c]
003bf794 ldr r2, [r4, r2]
003bf798 ldr r2, [r2]
003bf79c cmp r2, #2
003bf7a0 streq r3, [r3]
003bf7a4 beq #0x3bf594
003bf7a8 cmp r2, #1
003bf7ac bne #0x3bf594
003bf7b0 ldr r0, [pc, #0x60]
003bf7b4 ldr r1, [pc, #0x60]
003bf7b8 ldr r2, [pc, #0x60]
003bf7bc ldr r0, [r4, r0]
003bf7c0 ldr r3, [pc, #0x5c]
003bf7c4 movw ip, #0x15e
003bf7c8 add r1, pc, r1
003bf7cc add r2, pc, r2
003bf7d0 add r3, pc, r3
003bf7d4 add r0, r0, #0xa8
003bf7d8 str ip, [sp]
003bf7dc bl #0x30e004
003bf7e0 b #0x3bf594
003bf7e4 bl #0x30e310
003bf7e8 subseq r5, sp, r8, ror #11
003bf7ec andeq r4, r0, ip, lsr #1
003bf7f0 strdeq r3, r4, [r0], -r4
003bf7f4 subseq r2, r0, r8, lsl #5
003bf7f8 ldrheq r5, [r0], #-0x40
003bf7fc andeq r0, r0, r4, lsl #17
003bf800 ldrsheq r5, [r0], #-0x48
003bf804 subseq r5, r0, r4, asr r2
003bf808 ldrsheq r5, [r0], #-0x24
003bf80c subseq r4, r0, r0, ror lr
003bf810 andeq r2, r0, r4, lsl r7
003bf814 andeq r3, r0, r0, asr #19
003bf818 andeq r1, r0, r0, asr #19
003bf81c subeq lr, pc, r0, lsl ip
003bf820 subseq r5, r0, r4, lsr #2
003bf824 subseq r5, r0, r0, lsr r1

SOURCE 003beb88
003beb88 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003beb8c ldr r4, [pc, #0x878]
003beb90 ldr r7, [pc, #0x878]
003beb94 ldr sb, [pc, #0x878]
003beb98 add r4, pc, r4
003beb9c ldr r3, [r4, r7]
003beba0 ldr r6, [pc, #0x870]
003beba4 ldr sl, [r4, sb]
003beba8 ldr r2, [pc, #0x86c]
003bebac ldr r3, [r3]
003bebb0 sub sp, sp, #0x94
003bebb4 add r6, pc, r6
003bebb8 mov r5, r0
003bebbc add r2, pc, r2
003bebc0 str r1, [sp, #0x14]
003bebc4 ldr r0, [sl, #0x2c]
003bebc8 mov r1, r6
003bebcc str r3, [sp, #0x8c]
003bebd0 bl #0x4c4bdc
003bebd4 mov r8, r0
003bebd8 mov r0, r5
003bebdc bl #0x3bb918
003bebe0 cmp r0, #1
003bebe4 beq #0x3bf030
003bebe8 mov r0, r5
003bebec bl #0x3bb918
003bebf0 cmp r0, #2
003bebf4 beq #0x3bf04c
003bebf8 add r6, r5, #0x560
003bebfc mov r0, r6
003bec00 mov r1, #0x13
003bec04 mov r2, #0
003bec08 bl #0x3df6e0
003bec0c cmp r8, r0
003bec10 bgt #0x3bec30
003bec14 ldr r3, [r4, r7]
003bec18 ldr r2, [sp, #0x8c]
003bec1c ldr r3, [r3]
003bec20 cmp r2, r3
003bec24 bne #0x3bf408
003bec28 add sp, sp, #0x94
003bec2c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003bec30 ldr r3, [pc, #0x7e8]
003bec34 add sl, sp, #0x74
003bec38 ldr fp, [r4, r3]
003bec3c mov r0, fp
003bec40 bl #0x337888
003bec44 ldr r1, [pc, #0x7d8]
003bec48 add r2, sp, #0x54
003bec4c mov r0, sl
003bec50 add r1, pc, r1
003bec54 bl #0x3140ec
003bec58 mov r1, sl
003bec5c mov r0, fp
003bec60 bl #0x337a88
003bec64 mov r0, sl
003bec68 bl #0x318254
003bec6c mov r0, r6
003bec70 mov r1, #0x13
003bec74 mov r2, #1
003bec78 bl #0x3e0798
003bec7c mov r2, #0
003bec80 mov r0, r6
003bec84 mov r1, #0x21
003bec88 bl #0x3e0808
003bec8c movw r3, #0x13c8
003bec90 ldrsh r1, [r5, r3]
003bec94 mov r0, r6
003bec98 bl #0x3e087c
003bec9c mov r0, r5
003beca0 mvn r1, #0
003beca4 bl #0x3bdca4
003beca8 mov r0, r5
003becac mvn r1, #0
003becb0 bl #0x3bdbb8
003becb4 mov r2, #0
003becb8 mov r1, #0x13
003becbc mov r0, r6
003becc0 bl #0x3df6e0
003becc4 mov r1, r0
003becc8 mov r0, r5
003beccc bl #0x3bb840
003becd0 mov r0, r5
003becd4 bl #0x3bc4a8
003becd8 ldr r3, [r5]
003becdc mov r0, r5
003bece0 mov lr, pc
003bece4 ldr pc, [r3, #0x28]
003bece8 cmp r0, #0
003becec beq #0x3befe4
003becf0 add sl, sp, #0x58
003becf4 mov r0, sl
003becf8 mov r1, #0x10
003becfc str sl, [sp, #0x68]
003bed00 str sl, [sp, #0x6c]
003bed04 bl #0x31167c
003bed08 ldr r2, [sp, #0x68]
003bed0c mov r1, #0
003bed10 ldr r3, [r4, sb]
003bed14 strb r1, [r2]
003bed18 ldr r2, [pc, #0x708]
003bed1c ldr r1, [pc, #0x708]
003bed20 ldr r0, [r3, #0x2c]
003bed24 add r2, pc, r2
003bed28 add r1, pc, r1
003bed2c ldr fp, [r3, #0x34]
003bed30 bl #0x4c4bdc
003bed34 mov r1, r0
003bed38 mov r0, fp
003bed3c bl #0x508edc
003bed40 mov fp, r0
003bed44 bl #0x30de54
003bed48 ldr r1, [pc, #0x6e0]
003bed4c add r2, fp, r0
003bed50 mov r0, sl
003bed54 str r1, [sp, #0x10]
003bed58 mov r1, fp
003bed5c bl #0x3109e0
003bed60 ldr r2, [sp, #0x10]
003bed64 mov r1, sl
003bed68 ldr r2, [r4, r2]
003bed6c add fp, r2, #4
003bed70 mov r0, fp
003bed74 str r2, [sp, #0x10]
003bed78 bl #0x3be28c
003bed7c ldm fp, {r0, r1, r2, r3}
003bed80 add ip, sp, #0x20
003bed84 stm ip, {r0, r1, r2, r3}
003bed88 ldr r3, [sp, #0x10]
003bed8c mov r1, ip
003bed90 add r0, r3, #0x14
003bed94 bl #0x3bcf8c
003bed98 cmp r0, #1
003bed9c beq #0x3bf22c
003beda0 ldr r1, [pc, #0x68c]
003beda4 ldr r3, [r4, sb]
003beda8 mov r2, #1
003bedac ldr fp, [r4, r1]
003bedb0 str r1, [sp, #0x10]
003bedb4 ldr r1, [pc, #0x67c]
003bedb8 ldr r3, [r3, #0x4c]
003bedbc mov r0, fp
003bedc0 add r1, pc, r1
003bedc4 str r3, [sp, #0x18]
003bedc8 bl #0x4591f0
003bedcc cmn r0, #1
003bedd0 mov r1, r0
003bedd4 beq #0x3bede8
003bedd8 mov r0, fp
003beddc mvn r2, #0
003bede0 mov r3, #0
003bede4 bl #0x4605c0
003bede8 ldr r3, [pc, #0x64c]
003bedec mov r1, #0x87
003bedf0 mov r2, r5
003bedf4 ldr r0, [r4, r3]
003bedf8 mov r3, #0
003bedfc bl #0x495f04
003bee00 mov r0, r6
003bee04 mov r1, #0x13
003bee08 mov r2, #0
003bee0c bl #0x3df6e0
003bee10 cmp r0, #2
003bee14 mov fp, r0
003bee18 beq #0x3bf138
003bee1c cmp r0, #0xc
003bee20 beq #0x3bf1e0
003bee24 ldr r3, [r4, sb]
003bee28 mov r1, r5
003bee2c ldr r0, [r3, #0x40]
003bee30 bl #0x36effc
003bee34 cmp r0, #0
003bee38 beq #0x3befdc
003bee3c sub fp, fp, #0xa
003bee40 cmp fp, #0x5a
003bee44 addls pc, pc, fp, lsl #2
003bee48 b #0x3befdc
003bee4c b #0x3bf108
003bee50 b #0x3befdc
003bee54 b #0x3befdc
003bee58 b #0x3befdc
003bee5c b #0x3befdc
003bee60 b #0x3befdc
003bee64 b #0x3befdc
003bee68 b #0x3befdc
003bee6c b #0x3befdc
003bee70 b #0x3befdc
003bee74 b #0x3bf0f4
003bee78 b #0x3befdc
003bee7c b #0x3befdc
003bee80 b #0x3befdc
003bee84 b #0x3befdc
003bee88 b #0x3befdc
003bee8c b #0x3befdc
003bee90 b #0x3befdc
003bee94 b #0x3befdc
003bee98 b #0x3befdc
003bee9c b #0x3bf0e0
003beea0 b #0x3befdc
003beea4 b #0x3befdc
003beea8 b #0x3befdc
003beeac b #0x3befdc
003beeb0 b #0x3befdc
003beeb4 b #0x3befdc
003beeb8 b #0x3befdc
003beebc b #0x3befdc
003beec0 b #0x3befdc
003beec4 b #0x3bf0cc
003beec8 b #0x3befdc
003beecc b #0x3befdc
003beed0 b #0x3befdc
003beed4 b #0x3befdc
003beed8 b #0x3befdc
003beedc b #0x3befdc
003beee0 b #0x3befdc
003beee4 b #0x3befdc
003beee8 b #0x3befdc
003beeec b #0x3bf0b8
003beef0 b #0x3befdc
003beef4 b #0x3befdc
003beef8 b #0x3befdc
003beefc b #0x3befdc
003bef00 b #0x3befdc
003bef04 b #0x3befdc
003bef08 b #0x3befdc
003bef0c b #0x3befdc
003bef10 b #0x3befdc
003bef14 b #0x3bf0a4
003bef18 b #0x3befdc
003bef1c b #0x3befdc
003bef20 b #0x3befdc
003bef24 b #0x3befdc
003bef28 b #0x3befdc
003bef2c b #0x3befdc
003bef30 b #0x3befdc
003bef34 b #0x3befdc
003bef38 b #0x3befdc
003bef3c b #0x3bf090
003bef40 b #0x3befdc
003bef44 b #0x3befdc
003bef48 b #0x3befdc
003bef4c b #0x3befdc
003bef50 b #0x3befdc
003bef54 b #0x3befdc
003bef58 b #0x3befdc
003bef5c b #0x3befdc
003bef60 b #0x3befdc
003bef64 b #0x3bf07c
003bef68 b #0x3befdc
003bef6c b #0x3befdc
003bef70 b #0x3befdc
003bef74 b #0x3befdc
003bef78 b #0x3befdc
003bef7c b #0x3befdc
003bef80 b #0x3befdc
003bef84 b #0x3befdc
003bef88 b #0x3befdc
003bef8c b #0x3bf068
003bef90 b #0x3befdc
003bef94 b #0x3befdc
003bef98 b #0x3befdc
003bef9c b #0x3befdc
003befa0 b #0x3befdc
003befa4 b #0x3befdc
003befa8 b #0x3befdc
003befac b #0x3befdc
003befb0 b #0x3befdc
003befb4 b #0x3befb8
003befb8 ldr r3, [pc, #0x480]
003befbc ldr r0, [pc, #0x480]
003befc0 ldr r3, [r4, r3]
003befc4 add r0, pc, r0
003befc8 ldr sb, [r3]
003befcc bl #0x3a3f70
003befd0 mov r1, r0
003befd4 mov r0, sb
003befd8 bl #0x3813b8
003befdc mov r0, sl
003befe0 bl #0x318254
003befe4 mov r0, r6
003befe8 mov r1, #0x13
003befec mov r2, #0
003beff0 bl #0x3df6e0
003beff4 cmp r8, r0
003beff8 ble #0x3bec14
003beffc add r1, r5, #0xff0
003bf000 add r1, r1, #4
003bf004 mov r0, r6
003bf008 mov r2, #0x22
003bf00c bl #0x3dedb4
003bf010 ldr r1, [sp, #0x14]
003bf014 cmp r1, r0, asr #8
003bf018 bgt #0x3bf11c
003bf01c mov r0, r6
003bf020 ldr r2, [sp, #0x14]
003bf024 mov r1, #0x21
003bf028 bl #0x3e0798
003bf02c b #0x3bec14
003bf030 ldr r2, [pc, #0x410]
003bf034 ldr r0, [sl, #0x2c]
003bf038 mov r1, r6
003bf03c add r2, pc, r2
003bf040 bl #0x4c4bdc
003bf044 mov r8, r0
003bf048 b #0x3bebf8
003bf04c ldr r2, [pc, #0x3f8]
003bf050 ldr r0, [sl, #0x2c]
003bf054 mov r1, r6
003bf058 add r2, pc, r2
003bf05c bl #0x4c4bdc
003bf060 mov r8, r0
003bf064 b #0x3bebf8
003bf068 ldr r3, [pc, #0x3d0]
003bf06c ldr r0, [pc, #0x3dc]
003bf070 ldr r3, [r4, r3]
003bf074 add r0, pc, r0
003bf078 b #0x3befc8
003bf07c ldr r3, [pc, #0x3bc]
003bf080 ldr r0, [pc, #0x3cc]
003bf084 ldr r3, [r4, r3]
003bf088 add r0, pc, r0
003bf08c b #0x3befc8
003bf090 ldr r3, [pc, #0x3a8]
003bf094 ldr r0, [pc, #0x3bc]
003bf098 ldr r3, [r4, r3]
003bf09c add r0, pc, r0
003bf0a0 b #0x3befc8
003bf0a4 ldr r3, [pc, #0x394]
003bf0a8 ldr r0, [pc, #0x3ac]
003bf0ac ldr r3, [r4, r3]
003bf0b0 add r0, pc, r0
003bf0b4 b #0x3befc8
003bf0b8 ldr r3, [pc, #0x380]
003bf0bc ldr r0, [pc, #0x39c]
003bf0c0 ldr r3, [r4, r3]
003bf0c4 add r0, pc, r0
003bf0c8 b #0x3befc8
003bf0cc ldr r3, [pc, #0x36c]
003bf0d0 ldr r0, [pc, #0x38c]
003bf0d4 ldr r3, [r4, r3]
003bf0d8 add r0, pc, r0
003bf0dc b #0x3befc8
003bf0e0 ldr r3, [pc, #0x358]
003bf0e4 ldr r0, [pc, #0x37c]
003bf0e8 ldr r3, [r4, r3]
003bf0ec add r0, pc, r0
003bf0f0 b #0x3befc8
003bf0f4 ldr r3, [pc, #0x344]
003bf0f8 ldr r0, [pc, #0x36c]
003bf0fc ldr r3, [r4, r3]
003bf100 add r0, pc, r0
003bf104 b #0x3befc8
003bf108 ldr r3, [pc, #0x330]
003bf10c ldr r0, [pc, #0x35c]
003bf110 ldr r3, [r4, r3]
003bf114 add r0, pc, r0
003bf118 b #0x3befc8
003bf11c mov r0, r6
003bf120 mov r1, #0x22
003bf124 bl #0x3bd130
003bf128 asr r0, r0, #8
003bf12c sub r0, r0, #1
003bf130 str r0, [sp, #0x14]
003bf134 b #0x3bf01c
003bf138 bl #0x7fd794
003bf13c ldrb r3, [r0, #5]
003bf140 cmp r3, #0
003bf144 beq #0x3bf380
003bf148 bl #0x7fd794
003bf14c ldrb r3, [r0, #5]
003bf150 cmp r3, #0
003bf154 beq #0x3bf318
003bf158 bl #0x7fd794
003bf15c ldrb r3, [r0, #5]
003bf160 cmp r3, #0
003bf164 bne #0x3bee24
003bf168 mov r0, r5
003bf16c bl #0x3bb8e4
003bf170 cmp r0, #0
003bf174 str r0, [sp, #0x1c]
003bf178 bne #0x3bee24
003bf17c ldr r1, [sp, #0x18]
003bf180 ldrb r3, [r1, #0x2c]
003bf184 cmp r3, #0
003bf188 beq #0x3bee24
003bf18c ldr r2, [sp, #0x10]
003bf190 ldr r1, [pc, #0x2dc]
003bf194 ldr r2, [r4, r2]
003bf198 add r1, pc, r1
003bf19c str r2, [sp, #0x18]
003bf1a0 ldr r0, [sp, #0x18]
003bf1a4 mov r2, #1
003bf1a8 bl #0x4591f0
003bf1ac cmn r0, #1
003bf1b0 str r0, [sp, #0x10]
003bf1b4 beq #0x3bee24
003bf1b8 bl #0x7fd794
003bf1bc ldrb r3, [r0, #5]
003bf1c0 cmp r3, #0
003bf1c4 beq #0x3bee24
003bf1c8 ldr r0, [sp, #0x18]
003bf1cc ldr r1, [sp, #0x10]
003bf1d0 ldr r3, [sp, #0x1c]
003bf1d4 mvn r2, #0
003bf1d8 bl #0x4605c0
003bf1dc b #0x3bee24
003bf1e0 bl #0x42ca8c
003bf1e4 ldr r1, [pc, #0x28c]
003bf1e8 ldr r3, [r0, #0xf4]
003bf1ec ldr r2, [pc, #0x288]
003bf1f0 mov ip, #1
003bf1f4 ldr r0, [r3, #0x138]
003bf1f8 mov lr, #0
003bf1fc add r3, sp, #0x3c
003bf200 add r1, pc, r1
003bf204 add r2, pc, r2
003bf208 str r3, [sp, #0x10]
003bf20c strb lr, [sp, #0x3c]
003bf210 str ip, [sp]
003bf214 strb ip, [sp, #0x3d]
003bf218 strb ip, [sp, #0x40]
003bf21c bl #0x7ad7e8
003bf220 ldr r0, [sp, #0x10]
003bf224 bl #0x797124
003bf228 b #0x3bee24
003bf22c ldr r3, [pc, #0x24c]
003bf230 ldr r3, [r4, r3]
003bf234 ldr r3, [r3]
003bf238 str r3, [sp, #0x18]
003bf23c bl #0x42ca8c
003bf240 bl #0x42cb8c
003bf244 cmp r0, #0
003bf248 str r0, [sp, #0x10]
003bf24c beq #0x3beda0
003bf250 ldr fp, [pc, #0x22c]
003bf254 ldr r3, [r4, fp]
003bf258 ldr r2, [r3, #0x2c]
003bf25c cmp r2, #0
003bf260 beq #0x3bf29c
003bf264 ldr r0, [r3, #0x28]
003bf268 ldrb r3, [r0, #4]
003bf26c cmp r3, #0
003bf270 bne #0x3bf2b8
003bf274 ldr r1, [r0]
003bf278 sub r1, r1, #1
003bf27c cmp r1, #0
003bf280 str r1, [r0]
003bf284 bne #0x3bf28c
003bf288 bl #0x752b38
003bf28c ldr r3, [r4, fp]
003bf290 mov r2, #0
003bf294 str r2, [r3, #0x2c]
003bf298 str r2, [r3, #0x28]
003bf29c ldr r3, [pc, #0x1e4]
003bf2a0 ldr r0, [r4, fp]
003bf2a4 ldr r2, [sp, #0x10]
003bf2a8 ldr r1, [r4, r3]
003bf2ac mov r3, #0
003bf2b0 ldr r1, [r1]
003bf2b4 bl #0x427ca0
003bf2b8 ldr r0, [r4, fp]
003bf2bc bl #0x427d50
003bf2c0 mov ip, #0
003bf2c4 strb ip, [sp, #0x30]
003bf2c8 mov r2, #0
003bf2cc mov r3, #0
003bf2d0 mov ip, #2
003bf2d4 strd r2, r3, [sp, #0x48]
003bf2d8 strb ip, [sp, #0x31]
003bf2dc mov ip, #0
003bf2e0 str ip, [sp, #0x34]
003bf2e4 ldr ip, [sp, #0x4c]
003bf2e8 add fp, sp, #0x30
003bf2ec mov r1, r0
003bf2f0 ldr r2, [sp, #0x18]
003bf2f4 ldr r0, [sp, #0x10]
003bf2f8 mov r3, fp
003bf2fc str ip, [fp, #8]
003bf300 mov ip, #1
003bf304 str ip, [sp]
003bf308 bl #0x7abe0c
003bf30c mov r0, fp
003bf310 bl #0x797124
003bf314 b #0x3beda0
003bf318 mov r0, r5
003bf31c bl #0x3bb8e4
003bf320 subs r3, r0, #0
003bf324 bne #0x3bf158
003bf328 ldr r1, [sp, #0x18]
003bf32c ldrb r2, [r1, #0x29]
003bf330 cmp r2, #0
003bf334 beq #0x3bf158
003bf338 ldr r2, [sp, #0x10]
003bf33c ldr r1, [pc, #0x148]
003bf340 str r3, [sp, #0xc]
003bf344 ldr ip, [r4, r2]
003bf348 add r1, pc, r1
003bf34c mov r2, #1
003bf350 mov r0, ip
003bf354 str ip, [sp, #8]
003bf358 bl #0x4591f0
003bf35c cmn r0, #1
003bf360 mov r1, r0
003bf364 ldr r3, [sp, #0xc]
003bf368 ldr ip, [sp, #8]
003bf36c beq #0x3bf158
003bf370 mov r0, ip
003bf374 mvn r2, #0
003bf378 bl #0x4605c0
003bf37c b #0x3bf158
003bf380 mov r0, r5
003bf384 bl #0x3bb8e4
003bf388 subs r3, r0, #0
003bf38c bne #0x3bf148
003bf390 ldr r1, [sp, #0x18]
003bf394 ldrb r2, [r1, #0x2e]
003bf398 cmp r2, #0
003bf39c beq #0x3bf148
003bf3a0 ldr r2, [sp, #0x10]
003bf3a4 ldr r1, [pc, #0xe4]
003bf3a8 str r3, [sp, #0xc]
003bf3ac ldr ip, [r4, r2]
003bf3b0 add r1, pc, r1
003bf3b4 mov r2, #1
003bf3b8 mov r0, ip
003bf3bc str ip, [sp, #8]
003bf3c0 bl #0x4591f0
003bf3c4 cmn r0, #1
003bf3c8 mov r1, r0
003bf3cc ldr r3, [sp, #0xc]
003bf3d0 ldr ip, [sp, #8]
003bf3d4 beq #0x3bf3e4
003bf3d8 mov r0, ip
003bf3dc mvn r2, #0
003bf3e0 bl #0x4605c0
003bf3e4 ldr r3, [r4, sb]
003bf3e8 mov r1, #0
003bf3ec ldr r2, [r3, #0x4c]
003bf3f0 ldr r3, [pc, #0x9c]
003bf3f4 strb r1, [r2, #0x2e]
003bf3f8 ldr r3, [r4, r3]
003bf3fc ldr r0, [r3]
003bf400 bl #0x317e98
003bf404 b #0x3bf148
003bf408 bl #0x30e310
003bf40c ldrsheq r5, [sp], #-0xe8
003bf410 andeq r4, r0, ip, lsr #1
003bf414 strdeq r3, r4, [r0], -r4

SOURCE 003ddb10
003ddb10 push {r4, r5, r6, lr}
003ddb14 mov r5, r0
003ddb18 sub sp, sp, #8
003ddb1c mov r6, r1
003ddb20 bl #0x3dbf00
003ddb24 ldr r3, [r5, #0xb8]
003ddb28 tst r3, #0x400
003ddb2c beq #0x3ddb64
003ddb30 mov r0, sp
003ddb34 bl #0x3192b4
003ddb38 mov r0, sp
003ddb3c mov r1, r6
003ddb40 bl #0x386f28
003ddb44 ldr r1, [pc, #0x20]
003ddb48 mov r0, r5
003ddb4c mov r2, sp
003ddb50 add r1, pc, r1
003ddb54 bl #0x37c41c
003ddb58 mov r0, sp
003ddb5c mov r4, sp
003ddb60 bl #0x319228
003ddb64 add sp, sp, #8
003ddb68 pop {r4, r5, r6, pc}
003ddb6c subeq r8, lr, r0, asr #32

SOURCE 003d0d80
003d0d80 push {r4, lr}
003d0d84 ldr r3, [r0, #0x1c]
003d0d88 cmp r3, #0
003d0d8c beq #0x3d0da0
003d0d90 mov r0, r3
003d0d94 ldr r3, [r3]
003d0d98 mov lr, pc
003d0d9c ldr pc, [r3, #0xb0]
003d0da0 pop {r4, pc}
