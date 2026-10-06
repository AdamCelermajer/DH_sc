# _ZN9Character18F_ApplyCombatSoundERKNS_12AttackResultEPS_S3_
003afee0 push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003afee4 ldr      r4, [pc, #0x2b8]
003afee8 ldr      r5, [pc, #0x2b8]
003afeec sub      sp, sp, #0x6c
003afef0 add      r4, pc, r4
003afef4 ldr      r3, [r4, r5]
003afef8 mov      sb, r0
003afefc mov      r0, r1
003aff00 ldr      r3, [r3]
003aff04 mov      r6, r2
003aff08 add      r8, sp, #0x4c
003aff0c str      r3, [sp, #0x64]
003aff10 bl       #0x3a32d0
003aff14 mov      fp, r0
003aff18 mov      r0, r6
003aff1c bl       #0x3a32d0
003aff20 ldr      r3, [pc, #0x284]
003aff24 mov      r7, r0
003aff28 ldr      sl, [r4, r3]
003aff2c mov      r0, sl
003aff30 bl       #0x337888
003aff34 ldr      r1, [pc, #0x274]
003aff38 add      r2, sp, #0x48
003aff3c mov      r0, r8
003aff40 add      r1, pc, r1
003aff44 bl       #0x3140ec
003aff48 mov      r0, sl
003aff4c mov      r1, r8
003aff50 bl       #0x337a88
003aff54 mov      sl, r0
003aff58 mov      r0, r8
003aff5c bl       #0x3139ac
003aff60 cmp      sl, #0
003aff64 beq      #0x3aff84
003aff68 ldr      r3, [r4, r5]
003aff6c ldr      r2, [sp, #0x64]
003aff70 ldr      r3, [r3]
003aff74 cmp      r2, r3
003aff78 bne      #0x3b01a0
003aff7c add      sp, sp, #0x6c
003aff80 pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aff84 ldr      r3, [r6]
003aff88 mov      r0, r6
003aff8c mov      lr, pc
003aff90 ldr      pc, [r3, #0x34]
003aff94 subs     r8, r0, #0
003aff98 bne      #0x3b0034
003aff9c ldr      r3, [sb]
003affa0 cmp      r3, #0
003affa4 ble      #0x3aff68
003affa8 ldr      r0, [r7, #0xc]
003affac cmp      r0, #0
003affb0 bne      #0x3b013c
003affb4 ldrb     r8, [r7, #0x24]
003affb8 cmp      r8, #0
003affbc beq      #0x3b0058
003affc0 ldr      r0, [fp, #0x14]
003affc4 cmp      r0, #0
003affc8 beq      #0x3aff68
003affcc ldr      r3, [pc, #0x1e0]
003affd0 ldr      r7, [fp, #0x18]
003affd4 ldr      r3, [r4, r3]
003affd8 ldr      sl, [r3]
003affdc bl       #0x3af6d8
003affe0 ldr      r8, [r7, r0, lsl #2]
003affe4 mov      r0, r6
003affe8 bl       #0x3935dc
003affec ldr      r7, [r0]
003afff0 ldr      r6, [r0, #4]
003afff4 ldr      lr, [r0, #8]
003afff8 mov      ip, #0xbf000000
003afffc add      ip, ip, #0x800000
003b0000 mov      r0, sl
003b0004 mov      r1, r8
003b0008 add      r2, sp, #0x24
003b000c mov      r3, #0
003b0010 str      r7, [sp, #0x24]
003b0014 str      r6, [sp, #0x28]
003b0018 str      lr, [sp, #0x2c]
003b001c mov      lr, #1
003b0020 str      lr, [sp]
003b0024 str      ip, [sp, #8]
003b0028 str      ip, [sp, #4]
003b002c bl       #0x36b5d8
003b0030 b        #0x3aff68
003b0034 ldr      r0, [r7, #4]
003b0038 cmp      r0, #0
003b003c bne      #0x3b00c4
003b0040 ldr      r3, [sb]
003b0044 cmp      r3, #0
003b0048 ble      #0x3aff68
003b004c ldrb     r8, [r7, #0x24]
003b0050 cmp      r8, #0
003b0054 bne      #0x3affc0
003b0058 ldrb     r3, [r7, #0x25]
003b005c cmp      r3, #0
003b0060 beq      #0x3aff68
003b0064 ldr      r0, [fp, #0x1c]
003b0068 cmp      r0, #0
003b006c beq      #0x3aff68
003b0070 ldr      r3, [pc, #0x13c]
003b0074 ldr      r7, [fp, #0x20]
003b0078 ldr      r3, [r4, r3]
003b007c ldr      sb, [r3]
003b0080 bl       #0x3af6d8
003b0084 ldr      sl, [r7, r0, lsl #2]
003b0088 mov      r0, r6
003b008c bl       #0x3935dc
003b0090 ldr      r6, [r0]
003b0094 ldr      lr, [r0, #4]
003b0098 ldr      r7, [r0, #8]
003b009c mov      ip, #0xbf000000
003b00a0 add      ip, ip, #0x800000
003b00a4 mov      r0, sb
003b00a8 mov      r1, sl
003b00ac mov      r3, r8
003b00b0 add      r2, sp, #0x18
003b00b4 str      r6, [sp, #0x18]
003b00b8 str      lr, [sp, #0x1c]
003b00bc str      r7, [sp, #0x20]
003b00c0 b        #0x3b001c
003b00c4 ldr      r3, [pc, #0xe8]
003b00c8 ldr      r8, [r7, #8]
003b00cc ldr      r3, [r4, r3]
003b00d0 ldr      r3, [r3]
003b00d4 str      r3, [sp, #0x10]
003b00d8 bl       #0x3af6d8
003b00dc ldr      r8, [r8, r0, lsl #2]
003b00e0 mov      r0, r6
003b00e4 bl       #0x3935dc
003b00e8 ldr      r2, [r0, #4]
003b00ec ldr      lr, [r0]
003b00f0 ldr      r3, [sp, #0x10]
003b00f4 str      r2, [sp, #0x14]
003b00f8 ldr      r0, [r0, #8]
003b00fc str      lr, [sp, #0x3c]
003b0100 ldr      lr, [sp, #0x14]
003b0104 mov      ip, #0xbf000000
003b0108 str      r0, [sp, #0x44]
003b010c add      ip, ip, #0x800000
003b0110 mov      r0, r3
003b0114 mov      r1, r8
003b0118 mov      r3, sl
003b011c add      r2, sp, #0x3c
003b0120 str      lr, [sp, #0x40]
003b0124 mov      lr, #1
003b0128 str      lr, [sp]
003b012c str      ip, [sp, #8]
003b0130 str      ip, [sp, #4]
003b0134 bl       #0x36b5d8
003b0138 b        #0x3b0040
003b013c ldr      r3, [pc, #0x70]
003b0140 ldr      sl, [r7, #0x10]
003b0144 ldr      r3, [r4, r3]
003b0148 ldr      r3, [r3]
003b014c str      r3, [sp, #0x10]
003b0150 bl       #0x3af6d8
003b0154 ldr      sl, [sl, r0, lsl #2]
003b0158 mov      r0, r6
003b015c bl       #0x3935dc
003b0160 ldr      r2, [r0, #4]
003b0164 ldr      lr, [r0]
003b0168 ldr      r3, [sp, #0x10]
003b016c str      r2, [sp, #0x14]
003b0170 ldr      r0, [r0, #8]
003b0174 str      lr, [sp, #0x30]
003b0178 ldr      lr, [sp, #0x14]
003b017c mov      ip, #0xbf000000
003b0180 str      r0, [sp, #0x38]
003b0184 add      ip, ip, #0x800000
003b0188 mov      r0, r3
003b018c mov      r1, sl
003b0190 mov      r3, r8
003b0194 add      r2, sp, #0x30
003b0198 str      lr, [sp, #0x34]
003b019c b        #0x3b0124
003b01a0 bl       #0x30e310
003b01a4 subseq   r4, lr, r0, lsr #23
003b01a8 andeq    r4, r0, ip, lsr #1
003b01ac andeq    r0, r0, r4, lsl #17
003b01b0 subseq   r3, r1, r0, lsr #25
003b01b4 andeq    r0, r0, r4, lsr #27
