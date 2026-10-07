
SOURCE 003afee0
003afee0 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003afee4 ldr r4, [pc, #0x2b8]
003afee8 ldr r5, [pc, #0x2b8]
003afeec sub sp, sp, #0x6c
003afef0 add r4, pc, r4
003afef4 ldr r3, [r4, r5]
003afef8 mov sb, r0
003afefc mov r0, r1
003aff00 ldr r3, [r3]
003aff04 mov r6, r2
003aff08 add r8, sp, #0x4c
003aff0c str r3, [sp, #0x64]
003aff10 bl #0x3a32d0
003aff14 mov fp, r0
003aff18 mov r0, r6
003aff1c bl #0x3a32d0
003aff20 ldr r3, [pc, #0x284]
003aff24 mov r7, r0
003aff28 ldr sl, [r4, r3]
003aff2c mov r0, sl
003aff30 bl #0x337888
003aff34 ldr r1, [pc, #0x274]
003aff38 add r2, sp, #0x48
003aff3c mov r0, r8
003aff40 add r1, pc, r1
003aff44 bl #0x3140ec
003aff48 mov r0, sl
003aff4c mov r1, r8
003aff50 bl #0x337a88
003aff54 mov sl, r0
003aff58 mov r0, r8
003aff5c bl #0x3139ac
003aff60 cmp sl, #0
003aff64 beq #0x3aff84
003aff68 ldr r3, [r4, r5]
003aff6c ldr r2, [sp, #0x64]
003aff70 ldr r3, [r3]
003aff74 cmp r2, r3
003aff78 bne #0x3b01a0
003aff7c add sp, sp, #0x6c
003aff80 pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aff84 ldr r3, [r6]
003aff88 mov r0, r6
003aff8c mov lr, pc
003aff90 ldr pc, [r3, #0x34]
003aff94 subs r8, r0, #0
003aff98 bne #0x3b0034
003aff9c ldr r3, [sb]
003affa0 cmp r3, #0
003affa4 ble #0x3aff68
003affa8 ldr r0, [r7, #0xc]
003affac cmp r0, #0
003affb0 bne #0x3b013c
003affb4 ldrb r8, [r7, #0x24]
003affb8 cmp r8, #0
003affbc beq #0x3b0058
003affc0 ldr r0, [fp, #0x14]
003affc4 cmp r0, #0
003affc8 beq #0x3aff68
003affcc ldr r3, [pc, #0x1e0]
003affd0 ldr r7, [fp, #0x18]
003affd4 ldr r3, [r4, r3]
003affd8 ldr sl, [r3]
003affdc bl #0x3af6d8
003affe0 ldr r8, [r7, r0, lsl #2]
003affe4 mov r0, r6
003affe8 bl #0x3935dc
003affec ldr r7, [r0]
003afff0 ldr r6, [r0, #4]
003afff4 ldr lr, [r0, #8]
003afff8 mov ip, #0xbf000000
003afffc add ip, ip, #0x800000
003b0000 mov r0, sl
003b0004 mov r1, r8
003b0008 add r2, sp, #0x24
003b000c mov r3, #0
003b0010 str r7, [sp, #0x24]
003b0014 str r6, [sp, #0x28]
003b0018 str lr, [sp, #0x2c]
003b001c mov lr, #1
003b0020 str lr, [sp]
003b0024 str ip, [sp, #8]
003b0028 str ip, [sp, #4]
003b002c bl #0x36b5d8
003b0030 b #0x3aff68
003b0034 ldr r0, [r7, #4]
003b0038 cmp r0, #0
003b003c bne #0x3b00c4
003b0040 ldr r3, [sb]
003b0044 cmp r3, #0
003b0048 ble #0x3aff68
003b004c ldrb r8, [r7, #0x24]
003b0050 cmp r8, #0
003b0054 bne #0x3affc0
003b0058 ldrb r3, [r7, #0x25]
003b005c cmp r3, #0
003b0060 beq #0x3aff68
003b0064 ldr r0, [fp, #0x1c]
003b0068 cmp r0, #0
003b006c beq #0x3aff68
003b0070 ldr r3, [pc, #0x13c]
003b0074 ldr r7, [fp, #0x20]
003b0078 ldr r3, [r4, r3]
003b007c ldr sb, [r3]
003b0080 bl #0x3af6d8
003b0084 ldr sl, [r7, r0, lsl #2]
003b0088 mov r0, r6
003b008c bl #0x3935dc
003b0090 ldr r6, [r0]
003b0094 ldr lr, [r0, #4]
003b0098 ldr r7, [r0, #8]
003b009c mov ip, #0xbf000000
003b00a0 add ip, ip, #0x800000
003b00a4 mov r0, sb
003b00a8 mov r1, sl
003b00ac mov r3, r8
003b00b0 add r2, sp, #0x18
003b00b4 str r6, [sp, #0x18]
003b00b8 str lr, [sp, #0x1c]
003b00bc str r7, [sp, #0x20]
003b00c0 b #0x3b001c
003b00c4 ldr r3, [pc, #0xe8]
003b00c8 ldr r8, [r7, #8]
003b00cc ldr r3, [r4, r3]
003b00d0 ldr r3, [r3]
003b00d4 str r3, [sp, #0x10]
003b00d8 bl #0x3af6d8
003b00dc ldr r8, [r8, r0, lsl #2]
003b00e0 mov r0, r6
003b00e4 bl #0x3935dc
003b00e8 ldr r2, [r0, #4]
003b00ec ldr lr, [r0]
003b00f0 ldr r3, [sp, #0x10]
003b00f4 str r2, [sp, #0x14]
003b00f8 ldr r0, [r0, #8]
003b00fc str lr, [sp, #0x3c]
003b0100 ldr lr, [sp, #0x14]
003b0104 mov ip, #0xbf000000
003b0108 str r0, [sp, #0x44]
003b010c add ip, ip, #0x800000
003b0110 mov r0, r3
003b0114 mov r1, r8
003b0118 mov r3, sl
003b011c add r2, sp, #0x3c
003b0120 str lr, [sp, #0x40]
003b0124 mov lr, #1
003b0128 str lr, [sp]
003b012c str ip, [sp, #8]
003b0130 str ip, [sp, #4]
003b0134 bl #0x36b5d8
003b0138 b #0x3b0040
003b013c ldr r3, [pc, #0x70]
003b0140 ldr sl, [r7, #0x10]
003b0144 ldr r3, [r4, r3]
003b0148 ldr r3, [r3]
003b014c str r3, [sp, #0x10]
003b0150 bl #0x3af6d8
003b0154 ldr sl, [sl, r0, lsl #2]
003b0158 mov r0, r6
003b015c bl #0x3935dc
003b0160 ldr r2, [r0, #4]
003b0164 ldr lr, [r0]
003b0168 ldr r3, [sp, #0x10]
003b016c str r2, [sp, #0x14]
003b0170 ldr r0, [r0, #8]
003b0174 str lr, [sp, #0x30]
003b0178 ldr lr, [sp, #0x14]
003b017c mov ip, #0xbf000000
003b0180 str r0, [sp, #0x38]
003b0184 add ip, ip, #0x800000
003b0188 mov r0, r3
003b018c mov r1, sl
003b0190 mov r3, r8
003b0194 add r2, sp, #0x30
003b0198 str lr, [sp, #0x34]
003b019c b #0x3b0124
003b01a0 bl #0x30e310
003b01a4 subseq r4, lr, r0, lsr #23
003b01a8 andeq r4, r0, ip, lsr #1
003b01ac andeq r0, r0, r4, lsl #17
003b01b0 subseq r3, r1, r0, lsr #25
003b01b4 andeq r0, r0, r4, lsr #27

SOURCE 003afd38
003afd38 push {r4, r5, r6, r7, r8, sb, sl, lr}
003afd3c ldr r4, [pc, #0x188]
003afd40 ldr r5, [pc, #0x188]
003afd44 sub sp, sp, #0x48
003afd48 add r4, pc, r4
003afd4c ldr r3, [r4, r5]
003afd50 mov sb, r0
003afd54 mov r0, r2
003afd58 ldr r3, [r3]
003afd5c mov r8, r2
003afd60 add r6, sp, #0x2c
003afd64 str r3, [sp, #0x44]
003afd68 bl #0x3a32d0
003afd6c ldr r3, [pc, #0x160]
003afd70 mov sl, r0
003afd74 ldr r7, [r4, r3]
003afd78 mov r0, r7
003afd7c bl #0x337888
003afd80 ldr r1, [pc, #0x150]
003afd84 add r2, sp, #0x28
003afd88 mov r0, r6
003afd8c add r1, pc, r1
003afd90 bl #0x3140ec
003afd94 mov r0, r7
003afd98 mov r1, r6
003afd9c bl #0x337a88
003afda0 mov r7, r0
003afda4 mov r0, r6
003afda8 bl #0x3139ac
003afdac cmp r7, #0
003afdb0 beq #0x3afdd0
003afdb4 ldr r3, [r4, r5]
003afdb8 ldr r2, [sp, #0x44]
003afdbc ldr r3, [r3]
003afdc0 cmp r2, r3
003afdc4 bne #0x3afec8
003afdc8 add sp, sp, #0x48
003afdcc pop {r4, r5, r6, r7, r8, sb, sl, pc}
003afdd0 ldr r3, [r8]
003afdd4 mov r0, r8
003afdd8 mov lr, pc
003afddc ldr pc, [r3, #0x34]
003afde0 subs r6, r0, #0
003afde4 bne #0x3afe68
003afde8 ldr r3, [sb]
003afdec cmp r3, #0
003afdf0 ble #0x3afdb4
003afdf4 ldr r0, [sl, #0xc]
003afdf8 cmp r0, #0
003afdfc beq #0x3afdb4
003afe00 ldr r3, [pc, #0xd4]
003afe04 ldr r7, [sl, #0x10]
003afe08 ldr r3, [r4, r3]
003afe0c ldr sb, [r3]
003afe10 bl #0x3af6d8
003afe14 ldr sl, [r7, r0, lsl #2]
003afe18 mov r0, r8
003afe1c bl #0x3935dc
003afe20 ldr r7, [r0]
003afe24 ldr lr, [r0, #4]
003afe28 ldr r8, [r0, #8]
003afe2c mov ip, #0xbf000000
003afe30 add ip, ip, #0x800000
003afe34 mov r0, sb
003afe38 mov r1, sl
003afe3c mov r3, r6
003afe40 add r2, sp, #0x10
003afe44 str r7, [sp, #0x10]
003afe48 str lr, [sp, #0x14]
003afe4c str r8, [sp, #0x18]
003afe50 mov lr, #1
003afe54 str lr, [sp]
003afe58 str ip, [sp, #8]
003afe5c str ip, [sp, #4]
003afe60 bl #0x36b5d8
003afe64 b #0x3afdb4
003afe68 ldr r0, [sl, #4]
003afe6c cmp r0, #0
003afe70 beq #0x3afdb4
003afe74 ldr r3, [pc, #0x60]
003afe78 ldr r6, [sl, #8]
003afe7c ldr r3, [r4, r3]
003afe80 ldr sb, [r3]
003afe84 bl #0x3af6d8
003afe88 ldr sl, [r6, r0, lsl #2]
003afe8c mov r0, r8
003afe90 bl #0x3935dc
003afe94 ldr r6, [r0]
003afe98 ldr lr, [r0, #4]
003afe9c ldr r8, [r0, #8]
003afea0 mov ip, #0xbf000000
003afea4 add ip, ip, #0x800000
003afea8 mov r0, sb
003afeac mov r1, sl
003afeb0 mov r3, r7
003afeb4 add r2, sp, #0x1c
003afeb8 str r6, [sp, #0x1c]
003afebc str lr, [sp, #0x20]
003afec0 str r8, [sp, #0x24]
003afec4 b #0x3afe50
003afec8 bl #0x30e310
003afecc subseq r4, lr, r8, asr #26
003afed0 andeq r4, r0, ip, lsr #1
003afed4 andeq r0, r0, r4, lsl #17
003afed8 subseq r3, r1, r4, asr lr
003afedc andeq r0, r0, r4, lsr #27

SOURCE 003a3294
003a3294 movw r3, #0x1018
003a3298 ldr r0, [r0, r3]
003a329c ldr r3, [pc, #0x24]
003a32a0 cmp r0, #0
003a32a4 add r3, pc, r3
003a32a8 blt #0x3a32c0
003a32ac ldr r2, [pc, #0x18]
003a32b0 ldr r3, [r3, r2]
003a32b4 ldr r3, [r3]
003a32b8 cmp r0, r3
003a32bc bxlt lr
003a32c0 mov r0, #2
003a32c4 bx lr
003a32c8 subseq r1, pc, ip, ror #15
003a32cc andeq r2, r0, r0, lsr #20

SOURCE 003a32d0
003a32d0 ldr r3, [pc, #0x20]
003a32d4 ldr r2, [pc, #0x20]
003a32d8 push {r4, lr}
003a32dc add r3, pc, r3
003a32e0 ldr r2, [r3, r2]
003a32e4 ldr r4, [r2]
003a32e8 bl #0x3a3294
003a32ec mov r3, #0x28
003a32f0 mla r0, r3, r0, r4
003a32f4 pop {r4, pc}
003a32f8 ldrheq r1, [pc], #-0x74
003a32fc andeq r1, r0, r0, lsr #23

SOURCE 003af6d8
003af6d8 push {r4, lr}
003af6dc ldr r4, [pc, #0x7c]
003af6e0 cmp r0, #0
003af6e4 add r4, pc, r4
003af6e8 beq #0x3af748
003af6ec ldr r2, [pc, #0x70]
003af6f0 mov r1, r0
003af6f4 movw r0, #0xe6ab
003af6f8 ldr r2, [r4, r2]
003af6fc movw r3, #0xdb17
003af700 movt r3, #0x2b52
003af704 ldr lr, [r2]
003af708 movw ip, #0xf26b
003af70c movt ip, #0xda
003af710 mul r0, r0, lr
003af714 add r0, r0, #0x2b000
003af718 add r0, r0, #0x3fc
003af71c add r0, r0, #1
003af720 umull lr, r3, r3, r0
003af724 rsb lr, r3, r0
003af728 add r3, r3, lr, lsr #1
003af72c lsr r3, r3, #0x17
003af730 mls r3, ip, r3, r0
003af734 mov r0, r3
003af738 str r3, [r2]
003af73c bl #0x30eb2c
003af740 eor r0, r1, r1, asr #31
003af744 sub r0, r0, r1, asr #31
003af748 ldr r3, [pc, #0x18]
003af74c ldr r3, [r4, r3]
003af750 ldr r2, [r3]
003af754 add r2, r2, #1
003af758 str r2, [r3]
003af75c pop {r4, pc}
003af760 subseq r5, lr, ip, lsr #7
003af764 muleq r0, r4, ip
003af768 andeq r1, r0, r8, lsl #1

SOURCE 0036b5d8
0036b5d8 push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0036b5dc ldr r4, [pc, #0x200]
0036b5e0 ldr r5, [pc, #0x200]
0036b5e4 ldr r7, [pc, #0x200]
0036b5e8 add r4, pc, r4
0036b5ec ldr ip, [r4, r5]
0036b5f0 ldr r6, [r4, r7]
0036b5f4 sub sp, sp, #0x74
0036b5f8 ldr ip, [ip]
0036b5fc mov sb, r0
0036b600 mov r0, r6
0036b604 str r3, [sp, #0x1c]
0036b608 str ip, [sp, #0x6c]
0036b60c mov sl, r1
0036b610 mov fp, r2
0036b614 bl #0x337888
0036b618 ldr r1, [pc, #0x1d0]
0036b61c add r8, sp, #0x54
0036b620 add r2, sp, #0x38
0036b624 add r1, pc, r1
0036b628 mov r0, r8
0036b62c bl #0x3140ec
0036b630 mov r0, r6
0036b634 mov r1, r8
0036b638 bl #0x337a88
0036b63c mov r6, r0
0036b640 mov r0, r8
0036b644 bl #0x3139ac
0036b648 cmp r6, #0
0036b64c beq #0x36b670
0036b650 ldr r3, [r4, r5]
0036b654 ldr r2, [sp, #0x6c]
0036b658 mov r0, #0
0036b65c ldr r3, [r3]
0036b660 cmp r2, r3
0036b664 bne #0x36b7e0
0036b668 add sp, sp, #0x74
0036b66c pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0036b670 ldr r3, [pc, #0x17c]
0036b674 ldr r0, [r4, r3]
0036b678 bl #0x31f594
0036b67c cmp r0, #0
0036b680 beq #0x36b650
0036b684 ldr r3, [r0, #0x130]
0036b688 cmp r3, #0x26
0036b68c bne #0x36b650
0036b690 bl #0x7fd794
0036b694 ldrb r3, [r0, #5]
0036b698 cmp r3, #0
0036b69c beq #0x36b6b4
0036b6a0 ldr r3, [pc, #0x150]
0036b6a4 ldr r3, [r4, r3]
0036b6a8 ldrb r3, [r3]
0036b6ac cmp r3, #0
0036b6b0 bne #0x36b650
0036b6b4 cmp sl, #0
0036b6b8 blt #0x36b650
0036b6bc ldr r3, [pc, #0x138]
0036b6c0 ldr r3, [r4, r3]
0036b6c4 ldrb r3, [r3]
0036b6c8 cmp r3, #0
0036b6cc bne #0x36b79c
0036b6d0 ldr r3, [pc, #0x128]
0036b6d4 mov r2, #0xc
0036b6d8 ldr r3, [r4, r3]
0036b6dc ldr r3, [r3]
0036b6e0 mla sl, r2, sl, r3
0036b6e4 ldr r3, [sl, #8]
0036b6e8 ldr r8, [sl, #4]
0036b6ec cmp r3, #1
0036b6f0 beq #0x36b7bc
0036b6f4 add ip, sp, #0x30
0036b6f8 str ip, [sp]
0036b6fc add ip, sp, #0x2c
0036b700 add r3, sp, #0x28
0036b704 mov r1, r8
0036b708 add r2, sp, #0x20
0036b70c str ip, [sp, #4]
0036b710 add r0, sb, #0x64
0036b714 add ip, sp, #0x24
0036b718 str ip, [sp, #8]
0036b71c bl #0x8896f4
0036b720 ldr r7, [r4, r7]
0036b724 add r6, sp, #0x3c
0036b728 mov r0, r7
0036b72c bl #0x337888
0036b730 ldr r1, [pc, #0xcc]
0036b734 add r2, sp, #0x34
0036b738 mov r0, r6
0036b73c add r1, pc, r1
0036b740 bl #0x3140ec
0036b744 mov r1, r6
0036b748 mov r0, r7
0036b74c bl #0x337a88
0036b750 mov r0, r6
0036b754 bl #0x3139ac
0036b758 ldr ip, [sp, #0x30]
0036b75c mov r0, sb
0036b760 mov r1, r8
0036b764 str ip, [sp]
0036b768 ldr ip, [sp, #0x2c]
0036b76c ldr r2, [sp, #0x20]
0036b770 ldr r3, [sp, #0x28]
0036b774 str ip, [sp, #4]
0036b778 ldr ip, [sp, #0x24]
0036b77c str fp, [sp, #0xc]
0036b780 str ip, [sp, #8]
0036b784 ldr ip, [sp, #0x9c]
0036b788 str ip, [sp, #0x10]
0036b78c ldr ip, [sp, #0xa0]
0036b790 str ip, [sp, #0x14]
0036b794 bl #0x36a7c0
0036b798 b #0x36b650
0036b79c ldr r3, [pc, #0x64]
0036b7a0 mov r0, sl
0036b7a4 ldr r2, [sp, #0x1c]
0036b7a8 ldr r1, [r4, r3]
0036b7ac mov r3, #2
0036b7b0 ldr r1, [r1]
0036b7b4 bl #0x531348
0036b7b8 b #0x36b650
0036b7bc mov ip, #0xbf000000
0036b7c0 add ip, ip, #0x800000
0036b7c4 mov r0, sb
0036b7c8 mov r1, r8
0036b7cc mov r2, fp
0036b7d0 mov r3, ip
0036b7d4 str ip, [sp]
0036b7d8 bl #0x36b420
0036b7dc b #0x36b650
0036b7e0 bl #0x30e310
0036b7e4 rsbeq sb, r2, r8, lsr #9
0036b7e8 andeq r4, r0, ip, lsr #1
0036b7ec andeq r0, r0, r4, lsl #17
0036b7f0 subseq r5, r5, r4, asr #21
0036b7f4 strdeq r3, r4, [r0], -r4
0036b7f8 andeq r2, r0, r0, lsr #31
0036b7fc andeq r3, r0, r0, lsr fp
0036b800 andeq r3, r0, ip, lsr lr
0036b804 subseq r5, r5, r4, asr #19
0036b808 andeq r0, r0, r0, lsl #13
