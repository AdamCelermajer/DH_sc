
TransferInventoryTo 003ffa68
003ffa68 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ffa6c mov r7, r0
003ffa70 ldr r4, [r0, #8]
003ffa74 ldr r0, [r0, #0xc]
003ffa78 ldr fp, [pc, #0x1a8]
003ffa7c sub sp, sp, #0x44
003ffa80 cmp r0, r4
003ffa84 add fp, pc, fp
003ffa88 mov r6, r1
003ffa8c mov sb, r2
003ffa90 mov r8, r3
003ffa94 beq #0x3ffc10
003ffa98 ldr r3, [pc, #0x18c]
003ffa9c ldr r1, [pc, #0x18c]
003ffaa0 ldr r2, [pc, #0x18c]
003ffaa4 str r3, [sp, #0x1c]
003ffaa8 ldr r3, [pc, #0x188]
003ffaac str r1, [sp, #8]
003ffab0 add r1, sp, #0x24
003ffab4 add r3, pc, r3
003ffab8 str r3, [sp, #0xc]
003ffabc ldr r3, [pc, #0x178]
003ffac0 str r2, [sp, #0x14]
003ffac4 add r4, r4, #4
003ffac8 add r3, pc, r3
003ffacc str r3, [sp, #0x10]
003ffad0 add r5, r6, #0x30
003ffad4 str r1, [sp, #0x18]
003ffad8 ldr r1, [r4, #-4]
003ffadc mov r2, sb
003ffae0 mov r3, r8
003ffae4 ldr r1, [r1]
003ffae8 mov r0, r6
003ffaec bl #0x3ff5d4
003ffaf0 ldr r3, [r4, #-4]
003ffaf4 ldr r0, [r3]
003ffaf8 bl #0x3f9e00
003ffafc ldr r3, [r6, #4]
003ffb00 mov sl, r0
003ffb04 cmp r3, #0
003ffb08 beq #0x3ffbe8
003ffb0c mov r0, r3
003ffb10 ldr r3, [r3]
003ffb14 mov lr, pc
003ffb18 ldr pc, [r3, #0x28]
003ffb1c cmp r0, #0
003ffb20 beq #0x3ffbe8
003ffb24 ldr r3, [r6, #0x30]
003ffb28 cmp r3, r5
003ffb2c beq #0x3ffb4c
003ffb30 ldr r2, [r3, #8]
003ffb34 cmp sl, r2
003ffb38 beq #0x3ffb4c
003ffb3c ldr r3, [r3]
003ffb40 cmp r5, r3
003ffb44 bne #0x3ffb30
003ffb48 mov r3, r5
003ffb4c cmp r5, r3
003ffb50 beq #0x3ffbe8
003ffb54 ldr r3, [sp, #8]
003ffb58 ldr r2, [fp, r3]
003ffb5c mov r0, r2
003ffb60 str r2, [sp, #4]
003ffb64 bl #0x31f594
003ffb68 subs r3, r0, #0
003ffb6c ldr r2, [sp, #4]
003ffb70 beq #0x3ffbe8
003ffb74 ldr ip, [r6, #4]
003ffb78 ldr r0, [r2, #0x2c]
003ffb7c ldr r1, [sp, #0xc]
003ffb80 ldr r2, [sp, #0x10]
003ffb84 str r3, [sp, #4]
003ffb88 str ip, [sp]
003ffb8c bl #0x4c4bdc
003ffb90 ldr r1, [sp, #0x14]
003ffb94 ldr r3, [sp, #4]
003ffb98 ldr ip, [sp]
003ffb9c ldr r2, [fp, r1]
003ffba0 str r0, [sp, #0x28]
003ffba4 ldr r1, [sp, #0x18]
003ffba8 add r2, r2, #8
003ffbac mov r0, r3
003ffbb0 str r2, [sp, #0x24]
003ffbb4 mov r3, #0
003ffbb8 mvn r2, #0
003ffbbc strb r3, [sp, #0x35]
003ffbc0 strb r3, [sp, #0x34]
003ffbc4 str ip, [sp, #0x2c]
003ffbc8 str sl, [sp, #0x3c]
003ffbcc str r2, [sp, #0x30]
003ffbd0 str r2, [sp, #0x38]
003ffbd4 bl #0x339090
003ffbd8 ldr r1, [sp, #0x1c]
003ffbdc ldr r3, [fp, r1]
003ffbe0 add r3, r3, #8
003ffbe4 str r3, [sp, #0x24]
003ffbe8 ldr r0, [r4, #-4]
003ffbec bl #0x310440
003ffbf0 ldr r3, [r7, #0xc]
003ffbf4 mov r2, r4
003ffbf8 add r4, r4, #4
003ffbfc cmp r3, r2
003ffc00 bne #0x3ffad8
003ffc04 ldr r2, [r7, #8]
003ffc08 cmp r3, r2
003ffc0c strne r2, [r7, #0xc]
003ffc10 mov r0, r7
003ffc14 mov r2, r6
003ffc18 ldr r1, [r7, #0x20]
003ffc1c bl #0x3fe1a8
003ffc20 add sp, sp, #0x44
003ffc24 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}

TransferQuantity 003ff858
003ff858 push {r4, r5, r6, r7, r8, sb, sl, lr}
003ff85c mov r4, r0
003ff860 ldr r5, [r0, #0xc]
003ff864 ldr r0, [r0, #8]
003ff868 ldr ip, [pc, #0x1bc]
003ff86c sub sp, sp, #8
003ff870 rsb r0, r0, r5
003ff874 cmp r1, r0, asr #2
003ff878 add ip, pc, ip
003ff87c mov r6, r1
003ff880 mov sb, r2
003ff884 mov r7, r3
003ff888 ldrb sl, [sp, #0x28]
003ff88c ldrb r8, [sp, #0x2c]
003ff890 blo #0x3ff8b8
003ff894 ldr r3, [pc, #0x194]
003ff898 ldr r3, [ip, r3]
003ff89c ldr r3, [r3]
003ff8a0 cmp r3, #2
003ff8a4 moveq r3, #0
003ff8a8 streq r3, [r3]
003ff8ac beq #0x3ff8b8
003ff8b0 cmp r3, #1
003ff8b4 beq #0x3ff9f0
003ff8b8 cmp r7, #0
003ff8bc ble #0x3ffa24
003ff8c0 ldr r5, [r4, #8]
003ff8c4 cmp r6, #0
003ff8c8 addne r5, r5, r6, lsl #2
003ff8cc ldr r3, [r5]
003ff8d0 ldr r0, [r3]
003ff8d4 ldrsh r6, [r0, #0x50]
003ff8d8 cmp r6, r7
003ff8dc bge #0x3ff8f8
003ff8e0 cmp r6, #0
003ff8e4 moveq r7, r6
003ff8e8 bne #0x3ff930
003ff8ec mov r0, r7
003ff8f0 add sp, sp, #8
003ff8f4 pop {r4, r5, r6, r7, r8, sb, sl, pc}
003ff8f8 bl #0x3f9e08
003ff8fc cmp r6, r7
003ff900 beq #0x3ff934
003ff904 ldr r3, [r5]
003ff908 mov r1, r7
003ff90c ldr r0, [r3]
003ff910 bl #0x3fc3e0
003ff914 mov r2, sl
003ff918 mov r1, r0
003ff91c mov r3, r8
003ff920 mov r0, sb
003ff924 add sp, sp, #8
003ff928 pop {r4, r5, r6, r7, r8, sb, sl, lr}
003ff92c b #0x3ff5d4
003ff930 bl #0x3f9e08
003ff934 ldr r3, [r5]
003ff938 ldrsb r1, [r3, #4]
003ff93c cmn r1, #1
003ff940 beq #0x3ff95c
003ff944 ldr r3, [r4]
003ff948 mov r0, r4
003ff94c mov r2, #0
003ff950 mov lr, pc
003ff954 ldr pc, [r3, #0x20]
003ff958 ldr r3, [r5]
003ff95c ldrsb r1, [r3, #5]
003ff960 cmn r1, #1
003ff964 beq #0x3ff980
003ff968 ldr r3, [r4]
003ff96c mov r0, r4
003ff970 mov r2, #1
003ff974 mov lr, pc
003ff978 ldr pc, [r3, #0x20]
003ff97c ldr r3, [r5]
003ff980 ldr r1, [r3]
003ff984 ldr r3, [r4, #0x24]
003ff988 mov r2, sl
003ff98c mov r0, sb
003ff990 cmp r1, r3
003ff994 moveq r3, #0
003ff998 streq r3, [r4, #0x24]
003ff99c ldreq r3, [r5]
003ff9a0 mov r6, r5
003ff9a4 ldreq r1, [r3]
003ff9a8 mov r3, r8
003ff9ac bl #0x3ff5d4
003ff9b0 mov r7, r0
003ff9b4 ldr r0, [r6], #4
003ff9b8 bl #0x310440
003ff9bc ldr r3, [r4, #0xc]
003ff9c0 cmp r6, r3
003ff9c4 beq #0x3ff9e4
003ff9c8 subs r2, r3, r6
003ff9cc moveq r6, r3
003ff9d0 beq #0x3ff9e4
003ff9d4 mov r1, r6
003ff9d8 mov r0, r5
003ff9dc bl #0x30df38
003ff9e0 ldr r6, [r4, #0xc]
003ff9e4 sub r6, r6, #4
003ff9e8 str r6, [r4, #0xc]
003ff9ec b #0x3ff8ec
003ff9f0 ldr r0, [pc, #0x3c]
003ff9f4 ldr r1, [pc, #0x3c]
003ff9f8 ldr r2, [pc, #0x3c]
003ff9fc ldr r0, [ip, r0]
003ffa00 ldr r3, [pc, #0x38]
003ffa04 movw ip, #0x1aa
003ffa08 add r1, pc, r1
003ffa0c add r2, pc, r2
003ffa10 add r3, pc, r3
003ffa14 add r0, r0, #0xa8
003ffa18 str ip, [sp]
003ffa1c bl #0x30e004
003ffa20 b #0x3ff8b8
003ffa24 mvn r7, #0
003ffa28 b #0x3ff8ec
003ffa2c subseq r5, sb, r8, lsl r2
003ffa30 andeq r3, r0, r0, asr #19
003ffa34 andeq r1, r0, r0, asr #19
003ffa38 ldrdeq lr, pc, [fp], #-0x90
003ffa3c subeq r7, ip, ip, ror sb
003ffa40 umaaleq r7, ip, r8, sb

TransferItemTo 003ffa44
003ffa44 str lr, [sp, #-4]!
003ffa48 sub sp, sp, #0xc
003ffa4c ldrb ip, [sp, #0x10]
003ffa50 str r3, [sp]
003ffa54 mvn r3, #0x80000000
003ffa58 str ip, [sp, #4]
003ffa5c bl #0x3ff858
003ffa60 add sp, sp, #0xc
003ffa64 ldm sp!, {pc}

TransferGold 003fe1a8
003fe1a8 push {r4, r5, r6, lr}
003fe1ac mov r4, r0
003fe1b0 mov r0, r2
003fe1b4 mov r5, r1
003fe1b8 bl #0x3fe164
003fe1bc mov r0, r4
003fe1c0 rsb r1, r5, #0
003fe1c4 pop {r4, r5, r6, lr}
003fe1c8 b #0x3fe164

CharacterGetLootTable 003a2fcc
003a2fcc movw r3, #0x101c
003a2fd0 ldr r0, [r0, r3]
003a2fd4 bx lr

CharacterDropLoot 003a5ae4
003a5ae4 push {r4, r5, lr}
003a5ae8 mov r4, r1
003a5aec sub sp, sp, #0xc
003a5af0 mov r5, r0
003a5af4 bl #0x3a2fcc
003a5af8 mov ip, #0
003a5afc mov r1, r5
003a5b00 mov r2, r4
003a5b04 mvn r3, #0
003a5b08 str ip, [sp]
003a5b0c bl #0x3ecba0
003a5b10 add sp, sp, #0xc
003a5b14 pop {r4, r5, pc}

UnregisterQuestGatheringItemId 0047d5fc
0047d5fc push {r4, r5, lr}
0047d600 mov r5, r0
0047d604 ldr r4, [r5, #0x30]!
0047d608 ldr r3, [pc, #0xd4]
0047d60c sub sp, sp, #0xc
0047d610 cmp r4, r5
0047d614 add r3, pc, r3
0047d618 beq #0x47d638
0047d61c ldr r2, [r4, #8]
0047d620 cmp r1, r2
0047d624 beq #0x47d638
0047d628 ldr r4, [r4]
0047d62c cmp r5, r4
0047d630 bne #0x47d61c
0047d634 mov r4, r5
0047d638 cmp r5, r4
0047d63c beq #0x47d67c
0047d640 ldrb r3, [r4, #0xc]
0047d644 sub r3, r3, #1
0047d648 uxtb r3, r3
0047d64c cmp r3, #0
0047d650 strb r3, [r4, #0xc]
0047d654 bne #0x47d6a8
0047d658 ldr r3, [r4]
0047d65c ldr r2, [r4, #4]
0047d660 mov r0, r4
0047d664 mov r1, #0x10
0047d668 str r3, [r2]
0047d66c str r2, [r3, #4]
0047d670 add sp, sp, #0xc
0047d674 pop {r4, r5, lr}
0047d678 b #0x708f00
0047d67c ldr r2, [pc, #0x64]
0047d680 ldr r2, [r3, r2]
0047d684 ldr r2, [r2]
0047d688 cmp r2, #2
0047d68c moveq r3, #0
0047d690 streq r3, [r3]
0047d694 beq #0x47d6a0
0047d698 cmp r2, #1
0047d69c beq #0x47d6b0
0047d6a0 cmp r5, r4
0047d6a4 bne #0x47d640
0047d6a8 add sp, sp, #0xc
0047d6ac pop {r4, r5, pc}
0047d6b0 ldr r0, [pc, #0x34]
0047d6b4 ldr r1, [pc, #0x34]
0047d6b8 ldr r2, [pc, #0x34]
0047d6bc ldr r0, [r3, r0]
0047d6c0 ldr r3, [pc, #0x30]
0047d6c4 movw ip, #0x14f
0047d6c8 add r1, pc, r1
0047d6cc add r2, pc, r2
0047d6d0 add r3, pc, r3
0047d6d4 add r0, r0, #0xa8
0047d6d8 str ip, [sp]
0047d6dc bl #0x30e004
0047d6e0 b #0x47d6a0
0047d6e4 subseq r7, r1, ip, ror r4
0047d6e8 andeq r3, r0, r0, asr #19
0047d6ec andeq r1, r0, r0, asr #19
0047d6f0 subeq r0, r4, r0, lsl sp
0047d6f4 ldrdeq r0, r1, [r5], #-0x7c
0047d6f8 subeq r0, r5, r8, lsl #16

ItemInventoryCtor 003ff200
003ff200 ldr r3, [pc, #0x120]
003ff204 ldr r2, [pc, #0x120]
003ff208 push {r4, r5, r6, r7, r8, sb, sl, lr}
003ff20c add r3, pc, r3
003ff210 ldr r2, [r3, r2]
003ff214 mov r6, r0
003ff218 mov r1, #0
003ff21c add r2, r2, #8
003ff220 str r2, [r6]
003ff224 mvn r2, #0x80000000
003ff228 sub sp, sp, #0x10
003ff22c add r0, r0, #0x30
003ff230 str r2, [r6, #0x28]
003ff234 mvn r2, #0
003ff238 mov r7, r1
003ff23c strb r2, [r6, #0x2c]
003ff240 str r0, [r6, #0x34]
003ff244 str r1, [r6, #4]
003ff248 str r1, [r6, #8]
003ff24c str r1, [r6, #0xc]
003ff250 str r1, [r6, #0x10]
003ff254 str r1, [r6, #0x14]
003ff258 str r1, [r6, #0x18]
003ff25c str r1, [r6, #0x1c]
003ff260 str r1, [r6, #0x20]
003ff264 str r1, [r6, #0x24]
003ff268 strb r1, [r6, #0x2d]
003ff26c strb r1, [r6, #0x2e]
003ff270 strb r1, [r6, #0x2f]
003ff274 str r0, [r6, #0x30]
003ff278 add sl, r6, #0x14
003ff27c mov r8, sp
003ff280 mov r5, r1
003ff284 add sb, sp, #0xc
003ff288 mov r0, sl
003ff28c mov r1, sp
003ff290 str r5, [sp]
003ff294 str r5, [sp, #4]
003ff298 str r5, [sp, #8]
003ff29c bl #0x3ff178
003ff2a0 ldr r0, [sp]
003ff2a4 cmp r0, #0
003ff2a8 beq #0x3ff2c4
003ff2ac ldr r1, [sp, #8]
003ff2b0 rsb r1, r0, r1
003ff2b4 bic r1, r1, #3
003ff2b8 cmp r1, #0x80
003ff2bc bhi #0x3ff320
003ff2c0 bl #0x708f00
003ff2c4 mov r4, #0
003ff2c8 ldr r0, [r6, #0x14]
003ff2cc str r5, [sp, #0xc]
003ff2d0 add r0, r0, r7
003ff2d4 ldmib r0, {r1, r3}
003ff2d8 cmp r1, r3
003ff2dc beq #0x3ff314
003ff2e0 str r5, [r1]
003ff2e4 ldr r3, [r0, #4]
003ff2e8 add r3, r3, #4
003ff2ec str r3, [r0, #4]
003ff2f0 add r4, r4, #1
003ff2f4 cmp r4, #9
003ff2f8 bne #0x3ff2c8
003ff2fc add r7, r7, #0xc
003ff300 cmp r7, #0x18
003ff304 bne #0x3ff288
003ff308 mov r0, r6
003ff30c add sp, sp, #0x10
003ff310 pop {r4, r5, r6, r7, r8, sb, sl, pc}
003ff314 mov r2, sb
003ff318 bl #0x3fef4c
003ff31c b #0x3ff2f0
003ff320 bl #0x310440
003ff324 b #0x3ff2c4
003ff328 subseq r5, sb, r4, lsl #17
003ff32c strdeq r2, r3, [r0], -ip

ItemInteract 003ed144
003ed144 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ed148 ldr r4, [pc, #0x8e0]
003ed14c ldr r5, [pc, #0x8e0]
003ed150 sub sp, sp, #0x184
003ed154 add r4, pc, r4
003ed158 ldr r3, [r4, r5]
003ed15c mov r8, r1
003ed160 mov r6, r0
003ed164 ldr r3, [r3]
003ed168 str r3, [sp, #0x17c]
003ed16c bl #0x3ebffc
003ed170 cmp r0, #0
003ed174 beq #0x3ed194
003ed178 ldr r3, [r4, r5]
003ed17c ldr r2, [sp, #0x17c]
003ed180 ldr r3, [r3]
003ed184 cmp r2, r3
003ed188 bne #0x3ed8ec
003ed18c add sp, sp, #0x184
003ed190 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ed194 add r7, sp, #0x3c
003ed198 mov r0, r7
003ed19c mov r1, r8
003ed1a0 bl #0x33dd2c
003ed1a4 mov r0, r7
003ed1a8 bl #0x33ff54
003ed1ac subs r7, r0, #0
003ed1b0 beq #0x3ed178
003ed1b4 ldr r3, [r6, #0x3bc]
003ed1b8 cmp r3, #0
003ed1bc beq #0x3ed1c8
003ed1c0 cmp r7, r3
003ed1c4 bne #0x3ed178
003ed1c8 mov r3, #0x3c0
003ed1cc ldrsh r8, [r6, r3]
003ed1d0 cmn r8, #1
003ed1d4 beq #0x3ed1fc
003ed1d8 ldr r3, [pc, #0x858]
003ed1dc mov r1, r7
003ed1e0 mov r2, #0
003ed1e4 ldr r3, [r4, r3]
003ed1e8 ldr r0, [r3, #0x40]
003ed1ec bl #0x36eea8
003ed1f0 ldr r3, [r0, #0x678]
003ed1f4 cmp r8, r3
003ed1f8 beq #0x3ed654
003ed1fc ldr r3, [r7]
003ed200 mov r0, r7
003ed204 mov lr, pc
003ed208 ldr pc, [r3, #0x28]
003ed20c cmp r0, #0
003ed210 beq #0x3ed178
003ed214 add r1, r6, #0x374
003ed218 str r1, [sp, #0x20]
003ed21c ldr r0, [sp, #0x20]
003ed220 mov r1, #0
003ed224 bl #0x3fc61c
003ed228 ldr fp, [pc, #0x808]
003ed22c ldr r1, [pc, #0x808]
003ed230 mov r8, r0
003ed234 ldr sl, [r4, fp]
003ed238 add r1, pc, r1
003ed23c mov r0, sl
003ed240 bl #0x320e44
003ed244 str r0, [sp, #0x1c]
003ed248 mov r0, r8
003ed24c bl #0x3f9e08
003ed250 ldr sb, [r0, #0x68]
003ed254 mov r0, r8
003ed258 bl #0x3f9e08
003ed25c cmn sb, #1
003ed260 addeq r2, r7, #0x37c
003ed264 ldr sb, [r0, #0x58]
003ed268 streq r2, [sp, #0x1c]
003ed26c bne #0x3ed668
003ed270 cmp sb, #0xe
003ed274 beq #0x3ed8f0
003ed278 ldr r3, [pc, #0x7c0]
003ed27c add r1, sp, #0x124
003ed280 mov r0, r8
003ed284 ldr r3, [r4, r3]
003ed288 str r1, [sp, #0x28]
003ed28c add sb, sp, #0x16c
003ed290 ldr r2, [r3]
003ed294 add sl, sp, #0x10c
003ed298 str r2, [sp, #0x18]
003ed29c bl #0x3fa6cc
003ed2a0 ldr r2, [sp, #0x18]
003ed2a4 mov r3, #0xc
003ed2a8 ldr r1, [pc, #0x794]
003ed2ac mla r3, r3, r0, r2
003ed2b0 add r1, pc, r1
003ed2b4 ldr r2, [r3, #8]
003ed2b8 mov r0, sb
003ed2bc bl #0x30eae4
003ed2c0 mov r1, sb
003ed2c4 add r2, sp, #0x54
003ed2c8 ldr r0, [sp, #0x28]
003ed2cc bl #0x3140ec
003ed2d0 ldr r3, [sp, #0x138]
003ed2d4 ldr r1, [sp, #0x134]
003ed2d8 mov r0, sl
003ed2dc str sl, [sp, #0x11c]
003ed2e0 rsb r1, r3, r1
003ed2e4 add r1, r1, #0xf
003ed2e8 str sl, [sp, #0x120]
003ed2ec bl #0x31167c
003ed2f0 ldr r1, [pc, #0x750]
003ed2f4 ldr r3, [sp, #0x11c]
003ed2f8 mov ip, #0
003ed2fc add r1, pc, r1
003ed300 strb ip, [r3]
003ed304 add r2, r1, #0xe
003ed308 add r3, sp, #0x48
003ed30c mov r0, sl
003ed310 str ip, [sp, #0x14]
003ed314 bl #0x32a78c
003ed318 mov r0, sl
003ed31c ldr r1, [sp, #0x138]
003ed320 ldr r2, [sp, #0x134]
003ed324 bl #0x310804
003ed328 add r2, sp, #0xdc
003ed32c str r2, [sp, #0x24]
003ed330 ldr r2, [pc, #0x714]
003ed334 add r3, sp, #0xf4
003ed338 mov r0, r3
003ed33c mov r1, sl
003ed340 add r2, pc, r2
003ed344 str r3, [sp, #0x18]
003ed348 bl #0x3338cc
003ed34c ldr r1, [r8, #0x1c]
003ed350 add r2, sp, #0x50
003ed354 ldr r0, [sp, #0x24]
003ed358 bl #0x3140ec
003ed35c ldr r3, [sp, #0x18]
003ed360 add sb, sp, #0xc4
003ed364 mov r0, sb
003ed368 mov r1, r3
003ed36c ldr r2, [sp, #0x24]
003ed370 bl #0x3ecf68
003ed374 ldr r2, [pc, #0x6d4]
003ed378 add r1, sp, #0x13c
003ed37c str r1, [sp, #0x2c]
003ed380 add r2, pc, r2
003ed384 mov r1, sb
003ed388 ldr r0, [sp, #0x2c]
003ed38c bl #0x3338cc
003ed390 mov r0, sb
003ed394 bl #0x3139ac
003ed398 ldr r0, [sp, #0x24]
003ed39c bl #0x3139ac
003ed3a0 ldr r3, [sp, #0x18]
003ed3a4 mov r0, r3
003ed3a8 bl #0x3139ac
003ed3ac mov r0, sl
003ed3b0 bl #0x3139ac
003ed3b4 add sl, sp, #0x58
003ed3b8 ldr r0, [sp, #0x28]
003ed3bc bl #0x3139ac
003ed3c0 mov r0, sl
003ed3c4 mov r1, #0x10
003ed3c8 str sl, [sp, #0x68]
003ed3cc str sl, [sp, #0x6c]
003ed3d0 bl #0x31167c
003ed3d4 ldr ip, [sp, #0x14]
003ed3d8 ldr r3, [sp, #0x68]
003ed3dc mov r0, sl
003ed3e0 strb ip, [r3]
003ed3e4 ldr r1, [sp, #0x150]
003ed3e8 ldr r2, [sp, #0x14c]
003ed3ec str ip, [sp, #0x14]
003ed3f0 bl #0x3109e0
003ed3f4 ldr r3, [r4, fp]
003ed3f8 ldr ip, [sp, #0x14]
003ed3fc mov r2, #1
003ed400 ldr r0, [r3, #0x40]
003ed404 mov r1, ip
003ed408 bl #0x36e478
003ed40c ldr r3, [r0, #0x660]
003ed410 cmp r7, r3
003ed414 beq #0x3eda24
003ed418 mov r0, r8
003ed41c bl #0x3f9e08
003ed420 ldr r3, [r0, #0x58]
003ed424 cmp r3, #0xd
003ed428 beq #0x3ed43c
003ed42c bl #0x7fd794
003ed430 ldrb r3, [r0, #5]
003ed434 cmp r3, #0
003ed438 beq #0x3ed9c4
003ed43c add r3, r7, #0x560
003ed440 str r3, [sp, #0x24]
003ed444 ldr r3, [pc, #0x608]
003ed448 add r8, sp, #0xac
003ed44c ldr sb, [r4, r3]
003ed450 mov r0, sb
003ed454 bl #0x337888
003ed458 ldr r1, [pc, #0x5f8]
003ed45c add r2, sp, #0x4c
003ed460 mov r0, r8
003ed464 add r1, pc, r1
003ed468 bl #0x3140ec
003ed46c mov r1, r8
003ed470 mov r0, sb
003ed474 bl #0x337a88
003ed478 mov r0, r8
003ed47c bl #0x3139ac
003ed480 mov r3, #1
003ed484 ldr r0, [sp, #0x20]
003ed488 ldr r1, [sp, #0x1c]
003ed48c mov r2, #0
003ed490 bl #0x3ffa68
003ed494 ldr r0, [sp, #0x24]
003ed498 mov r1, #0xdf
003ed49c mov r2, #1
003ed4a0 bl #0x3e0798
003ed4a4 ldr r3, [pc, #0x5b0]
003ed4a8 ldr r0, [sp, #0x24]
003ed4ac mov r1, #0xdf
003ed4b0 ldr r3, [r4, r3]
003ed4b4 mov r2, #0
003ed4b8 ldr r3, [r3]
003ed4bc str r3, [sp, #0x24]
003ed4c0 bl #0x3df6e0
003ed4c4 cmp r0, #0x12c
003ed4c8 bge #0x3ed918
003ed4cc mov r0, sl
003ed4d0 bl #0x3139ac
003ed4d4 ldr r0, [sp, #0x2c]
003ed4d8 bl #0x3139ac
003ed4dc mov r0, r6
003ed4e0 bl #0x3ebffc
003ed4e4 cmp r0, #0
003ed4e8 beq #0x3ed178
003ed4ec bl #0x7fd794
003ed4f0 ldrb r3, [r0, #5]
003ed4f4 cmp r3, #0
003ed4f8 beq #0x3ed54c
003ed4fc ldr r3, [r7, #0x378]
003ed500 ldrb r3, [r3, #0xa]
003ed504 cmp r3, #0
003ed508 beq #0x3ed54c
003ed50c bl #0x80b1bc
003ed510 mov r8, r0
003ed514 ldr r0, [pc, #0x544]
003ed518 mov r1, #1
003ed51c ldr sl, [r6, #0x108]
003ed520 add r0, pc, r0
003ed524 ldrb sb, [r7, #0x108]
003ed528 bl #0x80a244
003ed52c uxth sl, sl
003ed530 mov r3, #5
003ed534 mov r1, r0
003ed538 strb sb, [r0, #0x54]
003ed53c strb r3, [r0, #0x50]
003ed540 strh sl, [r0, #0x52]
003ed544 mov r0, r8
003ed548 bl #0x80e2a4
003ed54c movw r3, #0x3b6
003ed550 ldrsh r1, [r6, r3]
003ed554 ldr r3, [pc, #0x508]
003ed558 ldr lr, [r6, #0x164]
003ed55c ldr r8, [r6, #0x168]
003ed560 ldr r3, [r4, r3]
003ed564 ldr sl, [r6, #0x160]
003ed568 mov ip, #0xbf000000
003ed56c ldr r0, [r3]
003ed570 add ip, ip, #0x800000
003ed574 str lr, [sp, #0x34]
003ed578 add r2, sp, #0x30
003ed57c mov lr, #1
003ed580 mov r3, #0
003ed584 str r8, [sp, #0x38]
003ed588 str sl, [sp, #0x30]
003ed58c str lr, [sp]
003ed590 str ip, [sp, #8]
003ed594 str ip, [sp, #4]
003ed598 bl #0x36b5d8
003ed59c ldr r8, [r6, #0x3c8]
003ed5a0 cmp r8, #0
003ed5a4 beq #0x3ed5c0
003ed5a8 mov r0, r8
003ed5ac bl #0x498dec
003ed5b0 mov r0, r8
003ed5b4 bl #0x310440
003ed5b8 mov r3, #0
003ed5bc str r3, [r6, #0x3c8]
003ed5c0 mov r0, r6
003ed5c4 bl #0x3ebccc
003ed5c8 movw r3, #0x14a4
003ed5cc ldr r2, [r7, r3]
003ed5d0 ldr r0, [pc, #0x490]
003ed5d4 cmp r6, r2
003ed5d8 moveq r2, #0
003ed5dc streq r2, [r7, r3]
003ed5e0 add r0, pc, r0
003ed5e4 ldr r8, [r0]
003ed5e8 ands r8, r8, #1
003ed5ec beq #0x3ed874
003ed5f0 ldr r3, [pc, #0x474]
003ed5f4 mov r8, #0
003ed5f8 add r2, r7, #0x160
003ed5fc add r3, pc, r3
003ed600 ldr r1, [r3, #4]
003ed604 ldr r3, [pc, #0x464]
003ed608 str r8, [sp]
003ed60c ldr r0, [r4, r3]
003ed610 mov r3, r8
003ed614 bl #0x495d14
003ed618 ldr r3, [pc, #0x454]
003ed61c mov r1, r6
003ed620 ldr r0, [r4, r3]
003ed624 bl #0x3eac34
003ed628 ldr r3, [r4, fp]
003ed62c mov r1, r7
003ed630 mov r2, r8
003ed634 ldr r0, [r3, #0x40]
003ed638 bl #0x36eea8
003ed63c ldr r3, [pc, #0x434]
003ed640 ldr r2, [r0, #0x678]
003ed644 mov r1, #4
003ed648 ldr r0, [r4, r3]
003ed64c bl #0x3790ec
003ed650 b #0x3ed178
003ed654 mov r3, #0x3b8
003ed658 ldrsh r3, [r6, r3]
003ed65c cmp r3, #0
003ed660 bgt #0x3ed178
003ed664 b #0x3ed1fc
003ed668 mov r0, r8
003ed66c bl #0x3f9e80
003ed670 ldr r3, [sp, #0x1c]
003ed674 cmp r0, r3
003ed678 blo #0x3ed70c
003ed67c add r2, r7, #0x37c
003ed680 mov r0, r2
003ed684 str r2, [sp, #0x1c]
003ed688 bl #0x3fe330
003ed68c cmp r0, #0
003ed690 beq #0x3ed270
003ed694 add r8, sp, #0x74
003ed698 mov r0, r8
003ed69c mov r1, #0x10
003ed6a0 str r8, [sp, #0x84]
003ed6a4 str r8, [sp, #0x88]
003ed6a8 bl #0x31167c
003ed6ac ldr r3, [sp, #0x84]
003ed6b0 mov r2, #0
003ed6b4 ldr r1, [pc, #0x3c0]
003ed6b8 strb r2, [r3]
003ed6bc ldr r2, [pc, #0x3bc]
003ed6c0 ldr r0, [sl, #0x2c]
003ed6c4 add r1, pc, r1
003ed6c8 add r2, pc, r2
003ed6cc ldr sl, [sl, #0x34]
003ed6d0 bl #0x4c4bdc
003ed6d4 mov r1, r0
003ed6d8 mov r0, sl
003ed6dc bl #0x508edc
003ed6e0 mov sl, r0
003ed6e4 bl #0x30de54
003ed6e8 mov r1, sl
003ed6ec add r2, sl, r0
003ed6f0 mov r0, r8
003ed6f4 bl #0x3109e0
003ed6f8 mov r0, r8
003ed6fc bl #0x3ecff8
003ed700 mov r0, r8
003ed704 bl #0x3139ac
003ed708 b #0x3ed4dc
003ed70c add sb, sp, #0x154
003ed710 mov r0, sb
003ed714 mov r1, #0x10
003ed718 str sb, [sp, #0x164]
003ed71c str sb, [sp, #0x168]
003ed720 bl #0x31167c
003ed724 ldr r3, [sp, #0x164]
003ed728 mov r2, #0
003ed72c ldr r1, [pc, #0x350]
003ed730 strb r2, [r3]
003ed734 ldr r2, [pc, #0x34c]
003ed738 ldr r3, [sl, #0x34]
003ed73c add r1, pc, r1
003ed740 add r2, pc, r2
003ed744 ldr r0, [sl, #0x2c]
003ed748 str r3, [sp, #0x18]
003ed74c bl #0x4c4bdc
003ed750 ldr r3, [sp, #0x18]
003ed754 mov r1, r0
003ed758 mov r0, r3
003ed75c bl #0x508edc
003ed760 ldr r3, [pc, #0x2d8]
003ed764 str r0, [sp, #0x1c]
003ed768 mov r0, r8
003ed76c ldr r3, [r4, r3]
003ed770 ldr r3, [r3]
003ed774 str r3, [sp, #0x28]
003ed778 bl #0x3fa6cc
003ed77c mov r1, r8
003ed780 str r0, [sp, #0x24]
003ed784 mov r2, #1
003ed788 mov r0, r7
003ed78c bl #0x3a4a3c
003ed790 ldr r1, [sp, #0x1c]
003ed794 mov ip, r0
003ed798 cmp r1, #0
003ed79c beq #0x3ed7f8
003ed7a0 ldr r2, [sp, #0x24]
003ed7a4 ldr r1, [sp, #0x28]
003ed7a8 mov r3, #0xc
003ed7ac mla r3, r3, r2, r1
003ed7b0 ldr r1, [pc, #0x2d4]
003ed7b4 ldr r2, [r3, #8]
003ed7b8 add r3, sp, #0x16c
003ed7bc add r1, pc, r1
003ed7c0 bic r2, r2, #0xff000000
003ed7c4 mov r0, r3
003ed7c8 str r3, [sp, #0x18]
003ed7cc str ip, [sp, #0x14]
003ed7d0 bl #0x30eae4
003ed7d4 ldr lr, [r8, #0x1c]
003ed7d8 ldr ip, [sp, #0x14]
003ed7dc ldr r0, [sl, #0x34]
003ed7e0 ldr r2, [sp, #0x1c]
003ed7e4 ldr r3, [sp, #0x18]
003ed7e8 mov r1, sb
003ed7ec str lr, [sp]
003ed7f0 str ip, [sp, #4]
003ed7f4 bl #0x508ef4
003ed7f8 add r8, sp, #0x90
003ed7fc mov r0, r8
003ed800 mov r1, #0x10
003ed804 str r8, [sp, #0xa0]
003ed808 str r8, [sp, #0xa4]
003ed80c bl #0x31167c
003ed810 ldr r3, [sp, #0xa0]
003ed814 mov sl, #0
003ed818 mov r0, r8
003ed81c strb sl, [r3]
003ed820 ldr r1, [sp, #0x168]
003ed824 ldr r2, [sp, #0x164]
003ed828 bl #0x3109e0
003ed82c mov r0, r8
003ed830 bl #0x3ecff8
003ed834 mov ip, #1
003ed838 mov r3, sl
003ed83c mov r1, sl
003ed840 add r2, r7, #0x37c
003ed844 ldr r0, [sp, #0x20]
003ed848 str ip, [sp]
003ed84c bl #0x3ffa44
003ed850 mov r2, sl
003ed854 mov r1, r0
003ed858 mov r0, r7
003ed85c bl #0x3a4bec
003ed860 mov r0, r8
003ed864 bl #0x3139ac
003ed868 mov r0, sb
003ed86c bl #0x3139ac
003ed870 b #0x3ed4dc
003ed874 bl #0x30e76c
003ed878 cmp r0, #0
003ed87c beq #0x3ed5f0
003ed880 ldr r3, [pc, #0x208]
003ed884 ldr r3, [r4, r3]
003ed888 ldr sl, [r3]
003ed88c cmp sl, #0
003ed890 beq #0x3ed910
003ed894 ldr r3, [pc, #0x1f8]
003ed898 ldr sb, [pc, #0x1f8]
003ed89c str r7, [sp, #0x20]
003ed8a0 ldr r3, [r4, r3]
003ed8a4 add sb, pc, sb
003ed8a8 ldr r3, [r3]
003ed8ac mov r7, r3
003ed8b0 b #0x3ed8c0
003ed8b4 add r8, r8, #1
003ed8b8 cmp r8, sl
003ed8bc beq #0x3ed90c
003ed8c0 mov r0, sb
003ed8c4 ldr r1, [r7, r8, lsl #2]
003ed8c8 bl #0x30e31c
003ed8cc cmp r0, #0
003ed8d0 bne #0x3ed8b4
003ed8d4 ldr r7, [sp, #0x20]
003ed8d8 ldr r0, [pc, #0x1bc]
003ed8dc add r0, pc, r0
003ed8e0 str r8, [r0, #4]
003ed8e4 bl #0x30ea3c
003ed8e8 b #0x3ed5f0
003ed8ec bl #0x30e310
003ed8f0 ldr r0, [sp, #0x1c]
003ed8f4 bl #0x3fc690
003ed8f8 ldrb r3, [r7, #0x3a8]
003ed8fc sxtb r3, r3
003ed900 cmp r0, r3
003ed904 bge #0x3ed4dc
003ed908 b #0x3ed278
003ed90c ldr r7, [sp, #0x20]
003ed910 mvn r8, #0
003ed914 b #0x3ed8d8
003ed918 ldr r3, [r7]
003ed91c mov r0, r7
003ed920 mov lr, pc
003ed924 ldr pc, [r3, #0x28]
003ed928 cmp r0, #0
003ed92c beq #0x3ed4cc
003ed930 ldr r3, [r4, fp]
003ed934 mov r1, r7
003ed938 ldr r0, [r3, #0x40]
003ed93c bl #0x36effc
003ed940 cmp r0, #0
003ed944 beq #0x3ed4cc
003ed948 ldr r3, [pc, #0x150]
003ed94c ldr r3, [r4, r3]
003ed950 ldr sb, [r3]
003ed954 cmp sb, #0
003ed958 beq #0x3ed9bc
003ed95c ldr r3, [pc, #0x140]
003ed960 ldr r2, [pc, #0x140]
003ed964 str r7, [sp, #0x1c]
003ed968 ldr r3, [r4, r3]
003ed96c add r2, pc, r2
003ed970 mov r8, #0
003ed974 ldr r3, [r3]
003ed978 str r2, [sp, #0x20]
003ed97c mov r7, r3
003ed980 b #0x3ed990
003ed984 add r8, r8, #1
003ed988 cmp r8, sb
003ed98c beq #0x3ed9b8
003ed990 ldr r0, [sp, #0x20]
003ed994 ldr r1, [r7, r8, lsl #2]
003ed998 bl #0x30e31c
003ed99c cmp r0, #0
003ed9a0 bne #0x3ed984
003ed9a4 ldr r7, [sp, #0x1c]
003ed9a8 mov r1, r8
003ed9ac ldr r0, [sp, #0x24]
003ed9b0 bl #0x3813b8
003ed9b4 b #0x3ed4cc
003ed9b8 ldr r7, [sp, #0x1c]
003ed9bc mvn r1, #0
003ed9c0 b #0x3ed9ac
003ed9c4 mov r0, r7
003ed9c8 bl #0x3bb8e4
003ed9cc subs sb, r0, #0
003ed9d0 bne #0x3ed43c
003ed9d4 ldr r3, [r4, fp]
003ed9d8 ldr r3, [r3, #0x4c]
003ed9dc ldrb r3, [r3, #0x2a]
003ed9e0 cmp r3, #0
003ed9e4 beq #0x3ed43c
003ed9e8 ldr r3, [pc, #0xbc]
003ed9ec ldr r1, [pc, #0xbc]
003ed9f0 mov r2, #1
003ed9f4 ldr r8, [r4, r3]
003ed9f8 add r1, pc, r1
003ed9fc mov r0, r8
003eda00 bl #0x4591f0
003eda04 cmn r0, #1
003eda08 mov r1, r0
003eda0c beq #0x3ed43c
003eda10 mov r0, r8
003eda14 mov r3, sb
003eda18 mvn r2, #0
003eda1c bl #0x4605c0
003eda20 b #0x3ed43c
003eda24 mov r0, sl
003eda28 bl #0x3ecff8
003eda2c b #0x3ed418
003eda30 subseq r7, sl, ip, lsr sb
003eda34 andeq r4, r0, ip, lsr #1
003eda38 strdeq r3, r4, [r0], -r4
003eda3c subeq sb, sp, r8, lsr #1
003eda40 andeq r1, r0, r4, asr #4
003eda44 subeq sb, sp, r0, lsl #2
003eda48 subeq sb, sp, r4, lsr r0
003eda4c subeq sb, sp, r0
003eda50 subeq r8, sp, r8, asr #31
003eda54 andeq r0, r0, r4, lsl #17
003eda58 subeq r8, sp, ip, lsl #30
003eda5c andeq r1, r0, r0, ror sp
003eda60 subeq r1, sp, r8, ror #19
003eda64 andeq r0, r0, r4, lsr #27
003eda68 ldrheq r5, [fp], #-0xa0

ItemDespawn 003eac34
003eac34 push {r4, r5, r6, lr}
003eac38 subs r4, r1, #0
003eac3c beq #0x3eac8c
003eac40 mov r3, #0x3ac
003eac44 ldrsh r3, [r4, r3]
003eac48 cmp r3, #0
003eac4c blt #0x3eac8c
003eac50 ldr r1, [r0, #8]
003eac54 ldr r2, [r0, #4]
003eac58 rsb r1, r2, r1
003eac5c cmp r3, r1, asr #4
003eac60 bge #0x3eac8c
003eac64 ldr r2, [r2, r3, lsl #4]
003eac68 mov r3, #0
003eac6c ldr r1, [r2, r3]
003eac70 add r0, r2, r3
003eac74 add r3, r3, #8
003eac78 cmp r4, r1
003eac7c beq #0x3eac90
003eac80 cmp r3, #0x28
003eac84 bne #0x3eac6c
003eac88 pop {r4, r5, r6, pc}
003eac8c pop {r4, r5, r6, pc}
003eac90 mov r5, #0
003eac94 strb r5, [r0, #4]
003eac98 ldr r3, [r4]
003eac9c mov r0, r4
003eaca0 mov r1, r5
003eaca4 mov lr, pc
003eaca8 ldr pc, [r3, #0x40]
003eacac add r0, r4, #0x374
003eacb0 mov r1, #1
003eacb4 bl #0x3fe6bc
003eacb8 mov r0, r4
003eacbc mov r1, r5
