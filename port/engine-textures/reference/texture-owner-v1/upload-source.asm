005afff0: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005afff4: ldr r2, [pc, #0x440]
005afff8: ldrb r3, [r0, #0x3f]
005afffc: sub sp, sp, #0x4c
005b0000: add r2, pc, r2
005b0004: tst r3, #2
005b0008: str r2, [sp, #0x30]
005b000c: ldrbeq ip, [r0, #0x3e]
005b0010: movne r3, #1
005b0014: ldrbne sb, [r0, #0x3e]
005b0018: streq ip, [sp, #0x40]
005b001c: strne r3, [sp, #0x40]
005b0020: ldr r2, [r0, #0x2c]
005b0024: ldr r3, [r0, #0x30]
005b0028: ldr r4, [r0, #0x38]
005b002c: movne r8, sb
005b0030: moveq r8, ip
005b0034: moveq sb, #1
005b0038: add r8, r8, #1
005b003c: cmp r2, #0
005b0040: mov r5, r0
005b0044: mov fp, r1
005b0048: add r8, r3, r8, lsl #2
005b004c: ubfx r4, r4, #4, #6
005b0050: ldr sl, [r0, #0x34]
005b0054: beq #0x5b0094
005b0058: mov r0, r4
005b005c: ldr r1, [r5, #0x20]
005b0060: bl #0x5edaec
005b0064: ldr r6, [r5, #0x34]
005b0068: tst r0, #1
005b006c: and r0, r0, #3
005b0070: ldr r3, [r6, #0x26c]
005b0074: movne r7, #1
005b0078: rsbeq r7, r0, #4
005b007c: cmp r7, r3
005b0080: beq #0x5b0094
005b0084: movw r0, #0xcf5
005b0088: mov r1, r7
005b008c: bl #0x30e16c
005b0090: str r7, [r6, #0x26c]
005b0094: bl #0x30e1a8
005b0098: mov r3, #0x14
005b009c: mla r3, r3, r4, sl
005b00a0: mov r1, #0
005b00a4: str r3, [sp, #0x34]
005b00a8: ldr r2, [r5, #0x38]
005b00ac: ldr r3, [pc, #0x38c]
005b00b0: ldr ip, [sp, #0x34]
005b00b4: and r2, r2, #3
005b00b8: cmp r2, #2
005b00bc: add r3, pc, r3
005b00c0: add ip, ip, #0x4b0
005b00c4: moveq r2, #6
005b00c8: movne r2, #1
005b00cc: add ip, ip, #4
005b00d0: add r3, r3, #0xa4
005b00d4: str r1, [sp, #0x20]
005b00d8: str r2, [sp, #0x44]
005b00dc: str ip, [sp, #0x3c]
005b00e0: str r3, [sp, #0x38]
005b00e4: str r1, [sp, #0x24]
005b00e8: mov r4, r1
005b00ec: ldr r2, [sp, #0x40]
005b00f0: cmp r2, #0
005b00f4: beq #0x5b025c
005b00f8: ldr r3, [sp, #0x34]
005b00fc: sub sl, r2, #1
005b0100: ldr ip, [pc, #0x33c]
005b0104: uxtb sl, sl
005b0108: add r3, r3, #0x4b0
005b010c: add sl, sl, #1
005b0110: mov r6, #0
005b0114: add r3, r3, #8
005b0118: lsl sl, sl, #2
005b011c: str r3, [sp, #0x2c]
005b0120: mov r7, r6
005b0124: str ip, [sp, #0x28]
005b0128: ldr r3, [r8]
005b012c: mov r2, #1
005b0130: ands r3, r3, r2, lsl r4
005b0134: beq #0x5b0238
005b0138: ldr lr, [r5, #0x2c]
005b013c: cmp lr, #0
005b0140: beq #0x5b0168
005b0144: ldrb r3, [r5, #0x3f]
005b0148: tst r3, #2
005b014c: beq #0x5b027c
005b0150: ldr r3, [r5, #0x30]
005b0154: ldr r1, [sp, #0x20]
005b0158: ldm r3, {r2, r3}
005b015c: rsb r2, r2, r3
005b0160: mul r2, r2, r1
005b0164: add lr, lr, r2
005b0168: ldr r3, [r5, #0x20]
005b016c: ldr r1, [r5, #0x24]
005b0170: ldr r2, [r5, #0x38]
005b0174: asr r3, r3, r7
005b0178: asr r1, r1, r7
005b017c: and r0, r2, #3
005b0180: cmp r3, #1
005b0184: movlt r3, #1
005b0188: cmp r1, #1
005b018c: movlt r1, #1
005b0190: cmp r0, #1
005b0194: beq #0x5b0224
005b0198: cmp r0, #2
005b019c: ldreq ip, [sp, #0x24]
005b01a0: ldrne ip, [sp, #0x38]
005b01a4: ubfx r2, r2, #4, #6
005b01a8: addeq r0, ip, #0x8500
005b01ac: ldrne r0, [ip, r0, lsl #2]
005b01b0: mov ip, #0x28
005b01b4: mul ip, ip, r2
005b01b8: ldr r2, [sp, #0x28]
005b01bc: str ip, [sp, #0x18]
005b01c0: ldr ip, [sp, #0x30]
005b01c4: addeq r0, r0, #0x15
005b01c8: ldr r2, [ip, r2]
005b01cc: ldr ip, [sp, #0x18]
005b01d0: ldr ip, [r2, ip]
005b01d4: str ip, [sp, #0x1c]
005b01d8: ands ip, ip, #8
005b01dc: beq #0x5b02a0
005b01e0: cmp fp, #0
005b01e4: beq #0x5b02e4
005b01e8: ldr r2, [sp, #0x34]
005b01ec: ldr ip, [r5, #0x30]
005b01f0: ldr r2, [r2, #0x4b0]
005b01f4: str r1, [sp]
005b01f8: mov r1, #0
005b01fc: str r1, [sp, #4]
005b0200: str r2, [sp, #0x1c]
005b0204: add r1, ip, r6
005b0208: ldr r1, [r1, #4]
005b020c: ldr ip, [ip, r6]
005b0210: str lr, [sp, #0xc]
005b0214: rsb ip, ip, r1
005b0218: mov r1, r7
005b021c: str ip, [sp, #8]
005b0220: bl #0x30ed3c
005b0224: bl #0x30e1a8
005b0228: cmp r0, #0
005b022c: ldrbne r3, [r5, #0x3f]
005b0230: orrne r3, r3, #0x10
005b0234: strbne r3, [r5, #0x3f]
005b0238: add r4, r4, sb
005b023c: cmp r4, #0x1f
005b0240: movhi r3, #0
005b0244: add r6, r6, #4
005b0248: strhi r3, [r8], #4
005b024c: subhi r4, r4, #0x20
005b0250: cmp r6, sl
005b0254: add r7, r7, #1
005b0258: bne #0x5b0128
005b025c: ldr ip, [sp, #0x24]
005b0260: ldr r1, [sp, #0x44]
005b0264: add ip, ip, #1
005b0268: cmp ip, r1
005b026c: str ip, [sp, #0x24]
005b0270: bge #0x5b0360
005b0274: str ip, [sp, #0x20]
005b0278: b #0x5b00ec
005b027c: ldr r3, [r5, #0x30]
005b0280: ldrb r1, [r5, #0x3e]
005b0284: ldr ip, [sp, #0x20]
005b0288: ldr r2, [r3, r6]
005b028c: ldr r3, [r3, r1, lsl #2]
005b0290: add r3, r3, #0x7f
005b0294: bic r3, r3, #0x7f
005b0298: mla r2, r3, ip, r2
005b029c: b #0x5b0164
005b02a0: cmp fp, #0
005b02a4: beq #0x5b0328
005b02a8: ldr r2, [sp, #0x34]
005b02ac: ldr r2, [r2, #0x4b0]
005b02b0: str ip, [sp, #4]
005b02b4: ldr ip, [sp, #0x3c]
005b02b8: str r2, [sp, #0x1c]
005b02bc: str r1, [sp]
005b02c0: ldr r1, [ip]
005b02c4: str r1, [sp, #8]
005b02c8: ldr r1, [sp, #0x2c]
005b02cc: ldr ip, [r1]
005b02d0: mov r1, r7
005b02d4: str lr, [sp, #0x10]
005b02d8: str ip, [sp, #0xc]
005b02dc: bl #0x30e010
005b02e0: b #0x5b0224
005b02e4: ldr r2, [sp, #0x34]
005b02e8: str r1, [sp, #4]
005b02ec: str r3, [sp]
005b02f0: ldr r3, [r2, #0x4b0]
005b02f4: ldr r2, [r5, #0x30]
005b02f8: mov r1, r7
005b02fc: str r3, [sp, #8]
005b0300: add r3, r2, r6
005b0304: ldr ip, [r2, r6]
005b0308: ldr r3, [r3, #4]
005b030c: mov r2, fp
005b0310: str lr, [sp, #0x10]
005b0314: rsb ip, ip, r3
005b0318: mov r3, fp
005b031c: str ip, [sp, #0xc]
005b0320: bl #0x30dda0
005b0324: b #0x5b0224
005b0328: ldr r2, [sp, #0x3c]
005b032c: str r1, [sp, #4]
005b0330: str r3, [sp]
005b0334: ldr r3, [r2]
005b0338: mov r1, r7
005b033c: mov r2, fp
005b0340: str r3, [sp, #8]
005b0344: ldr r3, [sp, #0x2c]
005b0348: ldr ip, [r3]
005b034c: mov r3, fp
005b0350: str lr, [sp, #0x10]
005b0354: str ip, [sp, #0xc]
005b0358: bl #0x30eb50
005b035c: b #0x5b0224
005b0360: cmp r4, #0
005b0364: movne r3, #0
005b0368: strne r3, [r8]
005b036c: ldrh r2, [r5, #0x40]
005b0370: ldrb r3, [r5, #0x3f]
005b0374: bic r2, r2, #3
005b0378: tst r3, #0x10
005b037c: strh r2, [r5, #0x40]
005b0380: bne #0x5b03e0
005b0384: ldrb r2, [r5, #0x3e]
005b0388: cmp r2, #1
005b038c: bls #0x5b03e0
005b0390: tst r3, #2
005b0394: beq #0x5b03e0
005b0398: ldr r3, [r5, #0x2c]
005b039c: cmp r3, #0
005b03a0: beq #0x5b0410
005b03a4: ldr r1, [sp, #0x30]
005b03a8: ldr r3, [r5, #0x38]
005b03ac: ldr r2, [pc, #0x90]
005b03b0: ubfx r3, r3, #4, #6
005b03b4: ldr r2, [r1, r2]
005b03b8: mov r1, #0x28
005b03bc: mul r3, r1, r3
005b03c0: ldr r3, [r2, r3]
005b03c4: tst r3, #8
005b03c8: beq #0x5b03ec
005b03cc: ldr r1, [pc, #0x74]
005b03d0: ldr r2, [r5, #0x1c]
005b03d4: mov r0, #2
005b03d8: add r1, pc, r1
005b03dc: bl #0x60b034
005b03e0: mov r0, #1
005b03e4: add sp, sp, #0x4c
005b03e8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005b03ec: ldr r3, [r5, #0x34]
005b03f0: ldr r3, [r3, #0x9c]
005b03f4: tst r3, #4
005b03f8: beq #0x5b03e0
005b03fc: mov r0, r5
005b0400: ldr r3, [r5]
005b0404: mov lr, pc
005b0408: ldr pc, [r3, #0x20]
005b040c: b #0x5b03e0
005b0410: ldr r3, [r5, #0x38]
005b0414: ldr r2, [pc, #0x28]
005b0418: ldr ip, [sp, #0x30]
005b041c: ubfx r3, r3, #4, #6
005b0420: mov r1, #0x28
005b0424: ldr r2, [ip, r2]
005b0428: mul r3, r1, r3
005b042c: ldr r3, [r2, r3]
005b0430: tst r3, #8
005b0434: beq #0x5b03e0
005b0438: b #0x5b03cc
005b043c: mlaseq lr, r0, sl, r4
005b0440: eorseq pc, r2, r8, ror pc
005b0444: andeq r1, r0, r4, lsr pc
005b0448: eorseq r0, r3, r0, lsr #32

005fdae8: ldrb r3, [r0, #0x3f]
005fdaec: push {r4, r5, r6, r7}
005fdaf0: tst r3, #2
005fdaf4: beq #0x5fdb70
005fdaf8: ldr r3, [r0, #0x2c]
005fdafc: cmp r3, #0
005fdb00: beq #0x5fdbd8
005fdb04: ldr r7, [r0, #0x38]
005fdb08: ldrh r3, [r0, #0x40]
005fdb0c: ldrb r1, [r0, #0x3e]
005fdb10: and r7, r7, #3
005fdb14: orr r3, r3, #1
005fdb18: cmp r7, #2
005fdb1c: mov r2, #0
005fdb20: strh r3, [r0, #0x40]
005fdb24: moveq r7, #6
005fdb28: movne r7, #1
005fdb2c: mov r3, r2
005fdb30: mov r6, #1
005fdb34: ldr r4, [r0, #0x30]
005fdb38: add r1, r1, #1
005fdb3c: lsr ip, r3, #5
005fdb40: add r1, r4, r1, lsl #2
005fdb44: ldr r4, [r1, ip, lsl #2]
005fdb48: and r5, r3, #0x1f
005fdb4c: add r2, r2, #1
005fdb50: orr r4, r4, r6, lsl r5
005fdb54: str r4, [r1, ip, lsl #2]
005fdb58: ldrb r1, [r0, #0x3e]
005fdb5c: cmp r2, r7
005fdb60: add r3, r3, r1
005fdb64: blt #0x5fdb34
005fdb68: pop {r4, r5, r6, r7}
005fdb6c: bx lr
005fdb70: ldr r3, [r0, #0x2c]
005fdb74: cmp r3, #0
005fdb78: beq #0x5fdbe4
005fdb7c: ldr r1, [r0, #0x38]
005fdb80: ldrb r2, [r0, #0x3e]
005fdb84: ldr r3, [r0, #0x30]
005fdb88: and r1, r1, #3
005fdb8c: cmp r1, #2
005fdb90: moveq r1, #6
005fdb94: movne r1, #1
005fdb98: mul r1, r2, r1
005fdb9c: ldrh ip, [r0, #0x40]
005fdba0: add r1, r1, #0x1f
005fdba4: add r2, r2, #1
005fdba8: lsr r1, r1, #5
005fdbac: add r3, r3, r2, lsl #2
005fdbb0: add r1, r3, r1, lsl #2
005fdbb4: orr r2, ip, #1
005fdbb8: cmp r1, r3
005fdbbc: strh r2, [r0, #0x40]
005fdbc0: beq #0x5fdb68
005fdbc4: mvn r2, #0
005fdbc8: str r2, [r3], #4
005fdbcc: cmp r1, r3
005fdbd0: bne #0x5fdbc8
005fdbd4: b #0x5fdb68
005fdbd8: cmp r1, #0
005fdbdc: beq #0x5fdb68
005fdbe0: b #0x5fdb04
005fdbe4: cmp r1, #0
005fdbe8: beq #0x5fdb68
005fdbec: b #0x5fdb7c
005fdbf0: push {r4, lr}
005fdbf4: ldr r3, [r0, #0x34]
005fdbf8: mov r0, r3
005fdbfc: ldr r3, [r3]
005fdc00: mov lr, pc
005fdc04: ldr pc, [r3, #0x5c]

005aa5f8: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005aa5fc: ldr r8, [r3]
005aa600: mov r5, r1
005aa604: ldr r1, [r1, #0x9c]
005aa608: add ip, r8, #6
005aa60c: and ip, ip, #0x1f
005aa610: mov lr, #1
005aa614: ands ip, r1, lr, lsl ip
005aa618: ldr r7, [pc, #0x25c]
005aa61c: sub sp, sp, #0x24
005aa620: mov r4, r0
005aa624: add r7, pc, r7
005aa628: bne #0x5aa670
005aa62c: uxth r3, r8
005aa630: cmp r3, #0xff
005aa634: beq #0x5aa6b8
005aa638: mov r0, ip
005aa63c: str r2, [sp, #0x10]
005aa640: bl #0x5fda68
005aa644: ldr r2, [sp, #0x10]
005aa648: ldr r3, [r0, r8, lsl #2]
005aa64c: ldr r1, [pc, #0x22c]
005aa650: mov r0, #3
005aa654: add r1, pc, r1
005aa658: bl #0x60b034
005aa65c: mov r3, #0
005aa660: str r3, [r4]
005aa664: mov r0, r4
005aa668: add sp, sp, #0x24
005aa66c: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005aa670: ldr r0, [r3, #0x10]
005aa674: cmp r0, #0
005aa678: beq #0x5aa6c4
005aa67c: ldr r6, [r3, #0x14]
005aa680: cmp r6, #0
005aa684: ldreq sl, [r3, #0x18]
005aa688: beq #0x5aa6cc
005aa68c: ldr sl, [r3, #0x18]
005aa690: cmp sl, #0
005aa694: beq #0x5aa6cc
005aa698: tst r1, #0x10
005aa69c: bne #0x5aa6f0
005aa6a0: cmp r0, r6
005aa6a4: beq #0x5aa868
005aa6a8: ldr r1, [pc, #0x1d4]
005aa6ac: mov r3, r0
005aa6b0: add r1, pc, r1
005aa6b4: b #0x5aa6d8
005aa6b8: ldr r3, [pc, #0x1c8]
005aa6bc: add r3, pc, r3
005aa6c0: b #0x5aa64c
005aa6c4: ldr sl, [r3, #0x18]
005aa6c8: ldr r6, [r3, #0x14]
005aa6cc: ldr r1, [pc, #0x1b8]
005aa6d0: mov r3, r0
005aa6d4: add r1, pc, r1
005aa6d8: mov r0, #3
005aa6dc: stm sp, {r6, sl}
005aa6e0: bl #0x60b034
005aa6e4: mov r3, #0
005aa6e8: str r3, [r4]
005aa6ec: b #0x5aa664
005aa6f0: cmp r8, #3
005aa6f4: beq #0x5aa73c
005aa6f8: tst r1, #0x20
005aa6fc: bne #0x5aa73c
005aa700: sub r1, r0, #1
005aa704: tst r1, r0
005aa708: beq #0x5aa71c
005aa70c: ldr r1, [pc, #0x17c]
005aa710: mov r3, r0
005aa714: add r1, pc, r1
005aa718: b #0x5aa6d8
005aa71c: sub r1, r6, #1
005aa720: tst r1, r6
005aa724: bne #0x5aa70c
005aa728: cmp r8, #1
005aa72c: bne #0x5aa73c
005aa730: sub r1, sl, #1
005aa734: tst r1, sl
005aa738: bne #0x5aa70c
005aa73c: ldr sb, [pc, #0x150]
005aa740: ldr r8, [r3, #4]
005aa744: mov fp, #0x28
005aa748: ldr r1, [r7, sb]
005aa74c: str r2, [sp, #0x10]
005aa750: str r3, [sp, #0x14]
005aa754: mla fp, fp, r8, r1
005aa758: str r8, [sp, #0x1c]
005aa75c: ldrb ip, [fp, #0x24]
005aa760: mov r1, ip
005aa764: str ip, [sp, #0x18]
005aa768: bl #0x30eb2c
005aa76c: cmp r1, #0
005aa770: ldr r2, [sp, #0x10]
005aa774: ldr r3, [sp, #0x14]
005aa778: beq #0x5aa7d0
005aa77c: uxth r1, r8
005aa780: cmp r1, #0x27
005aa784: bne #0x5aa828
005aa788: ldr r3, [pc, #0x108]
005aa78c: add r3, pc, r3
005aa790: ldr r1, [r7, sb]
005aa794: ldr ip, [sp, #0x1c]
005aa798: mov r0, #0x28
005aa79c: ldr r5, [sp, #0x18]
005aa7a0: mla r0, r0, ip, r1
005aa7a4: ldr r1, [pc, #0xf0]
005aa7a8: ldrb ip, [r0, #0x26]
005aa7ac: ldrb lr, [r0, #0x25]
005aa7b0: add r1, pc, r1
005aa7b4: mov r0, #3
005aa7b8: stm sp, {r5, lr}
005aa7bc: str ip, [sp, #8]
005aa7c0: bl #0x60b034
005aa7c4: mov r3, #0
005aa7c8: str r3, [r4]
005aa7cc: b #0x5aa664
005aa7d0: mov r0, r6
005aa7d4: ldrb r1, [fp, #0x25]
005aa7d8: str r2, [sp, #0x10]
005aa7dc: str r3, [sp, #0x14]
005aa7e0: bl #0x30eb2c
005aa7e4: cmp r1, #0
005aa7e8: ldr r2, [sp, #0x10]
005aa7ec: ldr r3, [sp, #0x14]
005aa7f0: bne #0x5aa77c
005aa7f4: mov r0, sl
005aa7f8: ldrb r1, [fp, #0x26]
005aa7fc: bl #0x30eb2c
005aa800: cmp r1, #0
005aa804: ldr r2, [sp, #0x10]
005aa808: ldr r3, [sp, #0x14]
005aa80c: bne #0x5aa77c
005aa810: mov r1, r5
005aa814: ldr ip, [r5]
005aa818: mov r0, r4
005aa81c: mov lr, pc
005aa820: ldr pc, [ip, #0x204]
005aa824: b #0x5aa664
005aa828: mov r0, #0
005aa82c: str r2, [sp, #0x10]
005aa830: str r3, [sp, #0x14]
005aa834: bl #0x5ed944
005aa838: ldr r3, [sp, #0x14]
005aa83c: ldr r1, [r7, sb]
005aa840: ldr r2, [sp, #0x10]
005aa844: ldr r3, [r3, #4]
005aa848: str r3, [sp, #0x1c]
005aa84c: ldr r5, [sp, #0x1c]
005aa850: ldr r3, [r0, r8, lsl #2]
005aa854: mov r0, #0x28
005aa858: mla r1, r0, r5, r1
005aa85c: ldrb r1, [r1, #0x24]
005aa860: str r1, [sp, #0x18]
005aa864: b #0x5aa790
005aa868: cmp r8, #1
005aa86c: bne #0x5aa6f0
005aa870: cmp sl, r0
005aa874: bne #0x5aa6a8
005aa878: b #0x5aa6f8
005aa87c: eorseq sl, lr, ip, ror #8
005aa880: eorseq r5, r3, r4, asr r7
005aa884: eorseq r5, r3, r0, ror r7
005aa888: eorseq fp, r1, r4, lsr #27
005aa88c: eorseq r5, r3, r4, lsl #14
005aa890: eorseq r5, r3, ip, asr r7
005aa894: andeq r1, r0, r4, lsr pc
005aa898: ldrsbteq fp, [r1], -r4
005aa89c: eorseq r5, r3, r8, lsl r7
005aa8a0: push {r4, r5, r6, r7, lr}
005aa8a4: ldr r4, [pc, #0x88]

006dd6dc: push {r4, r5, r6, lr}
006dd6e0: mov r6, r0
006dd6e4: mov r0, #0
006dd6e8: bl #0x6de848
006dd6ec: ldr r1, [r0]
006dd6f0: mov r5, r0
006dd6f4: cmp r1, #0
006dd6f8: beq #0x6dd72c
006dd6fc: mov r4, #1
006dd700: b #0x6dd714
006dd704: ldr r1, [r5, r4, lsl #2]
006dd708: add r4, r4, #1
006dd70c: cmp r1, #0
006dd710: beq #0x6dd72c
006dd714: mov r0, r6
006dd718: bl #0x30e31c
006dd71c: cmp r0, #0
006dd720: bne #0x6dd704
006dd724: sub r0, r4, #1
006dd728: pop {r4, r5, r6, pc}
006dd72c: movw r0, #0xffff
006dd730: pop {r4, r5, r6, pc}

005f95ac: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005f95b0: sub sp, sp, #0x134
005f95b4: ldr r5, [pc, #0xea0]
005f95b8: mov r8, r1
005f95bc: ldrb r1, [sp, #0x168]
005f95c0: cmp r2, #0
005f95c4: add r5, pc, r5
005f95c8: str r2, [sp, #0x5c]
005f95cc: mov r7, r3
005f95d0: mov sl, r0
005f95d4: ldr sb, [sp, #0x15c]
005f95d8: ldr r4, [sp, #0x160]
005f95dc: str r1, [sp, #0x38]
005f95e0: beq #0x5f9738
005f95e4: cmp sb, #0
005f95e8: beq #0x5f9724
005f95ec: cmp sl, r7
005f95f0: beq #0x5f97a0
005f95f4: ldr r6, [sp, #0x158]
005f95f8: cmp r8, r6
005f95fc: beq #0x5f9808
005f9600: ldr fp, [pc, #0xe58]
005f9604: mov r1, #0x28
005f9608: mul r6, r1, r7
005f960c: ldr r2, [r5, fp]
005f9610: ldr r3, [r2, r6]
005f9614: add r6, r2, r6
005f9618: tst r3, #8
005f961c: beq #0x5f9654
005f9620: uxth r3, r7
005f9624: cmp r3, #0x27
005f9628: beq #0x5f9794
005f962c: mov r0, #0
005f9630: bl #0x5ed944
005f9634: ldr r1, [r0, r7, lsl #2]
005f9638: ldr r0, [pc, #0xe24]
005f963c: mov r2, #3
005f9640: add r0, pc, r0
005f9644: bl #0x60ace8
005f9648: mov r0, #0
005f964c: add sp, sp, #0x134
005f9650: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005f9654: mul r1, r1, sl
005f9658: ldr r2, [r2, r1]
005f965c: tst r2, #8
005f9660: bne #0x5f97d0
005f9664: tst r3, #4
005f9668: beq #0x5f96b4
005f966c: tst r2, #4
005f9670: bne #0x5f96b4
005f9674: mov r0, sl
005f9678: str r3, [sp, #0x28]
005f967c: bl #0x5ed954
005f9680: ldrb r2, [r6, #0x14]
005f9684: ldr r3, [sp, #0x28]
005f9688: orr r2, r2, r0, lsl #2
005f968c: sub r2, r2, #4
005f9690: cmp r2, #5
005f9694: addls pc, pc, r2, lsl #2
005f9698: b #0x5f9aa4
005f969c: b #0x5f99e0
005f96a0: b #0x5f9abc
005f96a4: b #0x5f9aa4
005f96a8: b #0x5f9aa4
005f96ac: b #0x5f991c
005f96b0: b #0x5f986c
005f96b4: ldr r1, [r5, fp]
005f96b8: mov r0, #0x28
005f96bc: mla ip, r0, r7, r1
005f96c0: mla r1, r0, sl, r1
005f96c4: ldrb r0, [ip, #0x14]
005f96c8: ldrb r1, [r1, #0x14]
005f96cc: cmp r1, r0
005f96d0: beq #0x5f9748
005f96d4: orr r1, r2, r3
005f96d8: ands r1, r1, #2
005f96dc: bne #0x5f9844
005f96e0: sub r0, sl, #0xa
005f96e4: cmp r0, #1
005f96e8: bls #0x5f9ebc
005f96ec: ldr lr, [sp, #0x158]
005f96f0: str r4, [sp, #8]
005f96f4: ldr r5, [sp, #0x38]
005f96f8: ldr r4, [sp, #0x164]
005f96fc: mov r0, sl
005f9700: mov r1, r8
005f9704: ldr r2, [sp, #0x5c]
005f9708: mov r3, r7
005f970c: str lr, [sp]
005f9710: str sb, [sp, #4]
005f9714: str r4, [sp, #0xc]
005f9718: str r5, [sp, #0x10]
005f971c: bl #0x5f4fe8
005f9720: b #0x5f964c
005f9724: mov r0, r7
005f9728: mov r1, r4
005f972c: bl #0x5edaec
005f9730: mov sb, r0
005f9734: b #0x5f95ec
005f9738: mov r1, r4

005edbec: ldrb ip, [sp, #4]
005edbf0: cmp ip, #0
005edbf4: ldrb ip, [sp]
005edbf8: bne #0x5edc04
005edbfc: lsrs r1, r1, ip
005edc00: moveq r1, #1
005edc04: lsrs r2, r2, ip
005edc08: moveq r2, #1
005edc0c: lsrs r3, r3, ip
005edc10: moveq r3, #1
005edc14: b #0x5edbb8
005edc18: push {r4, r5, r6, r7, r8, sb, sl, lr}
005edc1c: sub sp, sp, #8
005edc20: ldrb r4, [sp, #0x28]
005edc24: mov sb, r0
005edc28: mov sl, r1
005edc2c: cmp r4, #0
005edc30: mov r8, r2
005edc34: mov r6, r3
005edc38: ldrb r7, [sp, #0x2c]
005edc3c: moveq r5, r4
005edc40: beq #0x5edc74
005edc44: mov r5, #0
005edc48: sub r4, r4, #1
005edc4c: uxtb r4, r4
005edc50: mov r0, sb
005edc54: mov r1, sl
005edc58: mov r2, r8
005edc5c: mov r3, r6
005edc60: stm sp, {r4, r7}
005edc64: bl #0x5edbec
005edc68: cmp r4, #0
005edc6c: add r5, r5, r0
005edc70: bne #0x5edc48
005edc74: mov r0, r5
005edc78: add sp, sp, #8
005edc7c: pop {r4, r5, r6, r7, r8, sb, sl, pc}
005edc80: str lr, [sp, #-4]!
005edc84: sub sp, sp, #0xc
005edc88: ldrb ip, [sp, #0x10]
005edc8c: str r3, [sp]
005edc90: mov r3, #1
005edc94: str ip, [sp, #4]
005edc98: bl #0x5edc18
005edc9c: add sp, sp, #0xc
005edca0: ldm sp!, {pc}
005edca4: ldr r3, [pc, #0xe8]
005edca8: ldr r2, [pc, #0xe8]
005edcac: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005edcb0: add r3, pc, r3
005edcb4: ldr r2, [r3, r2]
005edcb8: mov r6, #0x28
005edcbc: movw r3, #0xa3d
005edcc0: sub sp, sp, #0x14
005edcc4: movt r3, #0x3f17
005edcc8: mla r6, r6, r1, r2
005edccc: movw fp, #0x999a
005edcd0: str r3, [sp, #8]
005edcd4: movw r3, #0x47ae
005edcd8: movt fp, #0x3e99
005edcdc: movt r3, #0x3de1
005edce0: mov r5, r0
005edce4: str r3, [sp, #0xc]
005edce8: str fp, [sp, #4]
005edcec: mov r8, r0
005edcf0: mov r7, r0
005edcf4: mov r4, #0
005edcf8: mov sl, r6
005edcfc: add sb, sp, #4
005edd00: add r3, sl, r4
005edd04: ldr r0, [r3, #4]
005edd08: ldrb r3, [r6, #0x1c]
005edd0c: add r6, r6, #1
005edd10: str r0, [r7, #0xc]
005edd14: strb r3, [r8, #0x18]
005edd18: lsr r0, r0, r3
005edd1c: bl #0x30e2e0
005edd20: mov r1, r0
005edd24: mov r0, fp
005edd28: bl #0x30ec94
005edd2c: str r0, [r5, r4]
005edd30: add r4, r4, #4
005edd34: cmp r4, #0xc
005edd38: add r7, r7, #4
005edd3c: add r8, r8, #1
005edd40: ldrne fp, [sb, r4]
005edd44: bne #0x5edd00
005edd48: ldr r3, [sl, #0x10]
005edd4c: ldrb r4, [sl, #0x1f]
005edd50: str r3, [r5, #0x1c]
005edd54: strb r4, [r5, #0x1b]
005edd58: lsr r4, r3, r4
005edd5c: mov r0, r4
005edd60: bl #0x30e2e0
005edd64: mov r1, r0
005edd68: mov r0, #0x43000000

005edaec: ldr r3, [pc, #0x4c]
005edaf0: ldr r2, [pc, #0x4c]
005edaf4: push {r4, lr}
005edaf8: add r3, pc, r3
005edafc: ldr r2, [r3, r2]
005edb00: mov r4, #0x28
005edb04: mla r4, r4, r0, r2
005edb08: ldrb r3, [r4, #0x24]
005edb0c: cmp r3, #1
005edb10: bls #0x5edb30
005edb14: sub r0, r3, #1
005edb18: add r0, r0, r1
005edb1c: mov r1, r3
005edb20: bl #0x30ec4c
005edb24: ldrb r1, [r4, #0x15]
005edb28: mul r0, r1, r0
005edb2c: pop {r4, pc}
005edb30: ldrb r0, [r4, #0x16]
005edb34: mul r1, r0, r1
005edb38: lsr r0, r1, #3
005edb3c: pop {r4, pc}
005edb40: mlaseq sl, r8, pc, r6
005edb44: andeq r1, r0, r4, lsr pc
005edb48: push {r4, r5, r6, r7, r8, lr}
005edb4c: mov r8, r2
005edb50: mov r6, r0
005edb54: bl #0x5edaec
005edb58: ldr r4, [pc, #0x50]
005edb5c: ldr r5, [pc, #0x50]
005edb60: mov r2, #0x28
005edb64: add r4, pc, r4
005edb68: ldr r3, [r4, r5]
005edb6c: mov r7, r0
005edb70: mla r3, r2, r6, r3
005edb74: ldrb r1, [r3, #0x25]
005edb78: cmp r1, #1
005edb7c: mulls r0, r8, r0
005edb80: bls #0x5edb94
005edb84: sub r0, r1, #1
005edb88: add r0, r0, r8
005edb8c: bl #0x30ec4c
005edb90: mul r0, r0, r7
005edb94: ldr r3, [r4, r5]
005edb98: mov r2, #0x28
005edb9c: mla r6, r2, r6, r3
005edba0: ldrb r3, [r6, #0x27]
005edba4: cmp r0, r3
005edba8: movlo r0, r3
005edbac: pop {r4, r5, r6, r7, r8, pc}
005edbb0: eorseq r6, sl, ip, lsr #30
005edbb4: andeq r1, r0, r4, lsr pc
005edbb8: push {r4, lr}
005edbbc: mov r4, r3
005edbc0: bl #0x5edb48
005edbc4: mul r0, r4, r0
005edbc8: pop {r4, pc}
005edbcc: ldrb ip, [sp]
005edbd0: cmp ip, #0
005edbd4: bne #0x5edbe0
005edbd8: lsrs r1, r1, r3
005edbdc: moveq r1, #1
005edbe0: lsrs r2, r2, r3
005edbe4: moveq r2, #1
005edbe8: b #0x5edb48

006dcd90: mov r2, #0
006dcd94: mov r1, #4
006dcd98: str r2, [r0, #0x10]
006dcd9c: str r1, [r0, #0x18]
006dcda0: str r2, [r0, #0x14]
006dcda4: str r2, [r0, #0x1c]
006dcda8: str r2, [r0, #0x20]
006dcdac: str r2, [r0, #0x24]
006dcdb0: str r2, [r0, #0x28]
006dcdb4: str r2, [r0, #0x2c]
006dcdb8: str r2, [r0]
006dcdbc: str r2, [r0, #4]
006dcdc0: str r2, [r0, #8]
006dcdc4: str r2, [r0, #0xc]
006dcdc8: bx lr
006dcdcc: ldr r0, [r0, #0x284]
006dcdd0: bx lr
006dcdd4: mov r3, #0x44
006dcdd8: mul r3, r3, r1
006dcddc: add r3, r3, #0x288
006dcde0: add r0, r0, r3
006dcde4: bx lr
006dcde8: ldrb r0, [r0, #0x1c4]
006dcdec: bx lr
006dcdf0: ldrb r2, [r0, #0x204]
006dcdf4: ldrb ip, [r0, #0x205]
006dcdf8: ldrb r1, [r0, #0x206]
006dcdfc: mov r3, #0
006dce00: bfi r3, r2, #0, #8
006dce04: ldrb r2, [r0, #0x207]
006dce08: bfi r3, ip, #8, #8
006dce0c: bfi r3, r1, #0x10, #8
