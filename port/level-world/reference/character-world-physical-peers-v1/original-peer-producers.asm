# _ZN14PhysicalObject15onCollisionEndsEP18PhysicalBaseObjectRK7Point2DIfEb
003883cc: bx lr
# _ZN5Decor17DeclarePropertiesEv
003899d0: push {r4, r5, r6, r7, lr}
003899d4: sub sp, sp, #0xc
003899d8: mov r7, r0
003899dc: bl #0x38cee8
003899e0: mov r1, #0
003899e4: mov r0, #0x24
003899e8: bl #0x310570
003899ec: ldr r5, [pc, #0x68]
003899f0: ldr r3, [pc, #0x68]
003899f4: ldr r6, [pc, #0x68]
003899f8: add r5, pc, r5
003899fc: ldr r3, [r5, r3]
00389a00: add r6, pc, r6
00389a04: mov r4, r0
00389a08: add r3, r3, #8
00389a0c: mov r1, r6
00389a10: add r2, sp, #4
00389a14: str r3, [r0], #8
00389a18: bl #0x3140ec
00389a1c: ldr r3, [pc, #0x44]
00389a20: add r2, r7, #0x374
00389a24: add r0, r7, #4
00389a28: ldr r3, [r5, r3]
00389a2c: add r2, r2, #2
00389a30: rsb r2, r0, r2
00389a34: add r3, r3, #8
00389a38: str r3, [r4]
00389a3c: mov r3, #1
00389a40: str r2, [r4, #4]
00389a44: strb r3, [r4, #0x20]
00389a48: mov r1, r6
00389a4c: mov r2, r4
00389a50: bl #0x513ce4
00389a54: add sp, sp, #0xc
00389a58: pop {r4, r5, r6, r7, pc}
00389a5c: mlseq r0, r8, r0, fp
00389a60: andeq r2, r0, r0, lsr r3
00389a64: ldrsheq r8, [r3], #-0x80
00389a68: andeq r3, r0, ip, asr #28
# _ZN14PhysicalObject15onCollisionTestEP18PhysicalBaseObjectsttstt
0046e6bc: push {r4, r5, r6, r7}
0046e6c0: ldr ip, [r0, #8]
0046e6c4: ldr r1, [r1, #8]
0046e6c8: ldrh r5, [sp, #0x10]
0046e6cc: cmp ip, #0
0046e6d0: ldrsh r4, [sp, #0x14]
0046e6d4: ldrh r6, [sp, #0x18]
0046e6d8: ldrh r7, [sp, #0x1c]
0046e6dc: beq #0x46e6f8
0046e6e0: ldrb r0, [ip, #0x80]
0046e6e4: cmp r0, #0
0046e6e8: bne #0x46e6f8
0046e6ec: mov r0, #0
0046e6f0: pop {r4, r5, r6, r7}
0046e6f4: bx lr
0046e6f8: cmp r1, #0
0046e6fc: beq #0x46e70c
0046e700: ldrb r1, [r1, #0x80]
0046e704: cmp r1, #0
0046e708: beq #0x46e6ec
0046e70c: cmp r2, r4
0046e710: movne r1, #0
0046e714: moveq r1, #1
0046e718: cmp r2, #0
0046e71c: moveq r1, #0
0046e720: cmp r1, #0
0046e724: bne #0x46e740
0046e728: tst r6, r5
0046e72c: beq #0x46e6ec
0046e730: tst r7, r3
0046e734: moveq r0, #0
0046e738: movne r0, #1
0046e73c: b #0x46e6f0
0046e740: cmp r2, #0
0046e744: movle r0, #0
0046e748: movgt r0, #1
0046e74c: b #0x46e6f0
# _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_
0033e4ac: push {r4, r5, r6, r7, r8, sl, lr}
0033e4b0: mov r6, r0
0033e4b4: sub sp, sp, #0xc
0033e4b8: mov r5, r1
0033e4bc: mov r0, #0x24
0033e4c0: mov r1, #0
0033e4c4: mov sl, r3
0033e4c8: mov r7, r2
0033e4cc: bl #0x310570
0033e4d0: ldr r4, [pc, #0x54]
0033e4d4: ldr r3, [pc, #0x54]
0033e4d8: mov r8, r0
0033e4dc: add r4, pc, r4
0033e4e0: ldr r3, [r4, r3]
0033e4e4: mov r1, r5
0033e4e8: add r2, sp, #4
0033e4ec: add r3, r3, #8
0033e4f0: str r3, [r0], #8
0033e4f4: bl #0x3140ec
0033e4f8: ldr r3, [pc, #0x34]
0033e4fc: rsb r7, r6, r7
0033e500: str r7, [r8, #4]
0033e504: ldr r3, [r4, r3]
0033e508: strb sl, [r8, #0x20]
0033e50c: mov r0, r6
0033e510: add r3, r3, #8
0033e514: str r3, [r8]
0033e518: mov r1, r5
0033e51c: mov r2, r8
0033e520: bl #0x513ce4
0033e524: add sp, sp, #0xc
0033e528: pop {r4, r5, r6, r7, r8, sl, pc}
0033e52c: strhteq r6, [r5], #-0x54
0033e530: andeq r2, r0, r0, lsr r3
0033e534: andeq r3, r0, ip, asr #28
# _ZN18SimpleTypePropertyIbE17SetToDefaultValueEPv
0033de80: ldrb r2, [r0, #0x20]
0033de84: ldr r3, [r0, #4]
0033de88: strb r2, [r1, r3]
0033de8c: bx lr
# _ZN13AnimatedDecor17DeclarePropertiesEv
00389dd4: push {r4, lr}
00389dd8: mov r4, r0
00389ddc: bl #0x3899d0
00389de0: ldr r1, [pc, #0x10]
00389de4: add r2, r4, #0x37c
00389de8: add r0, r4, #4
00389dec: add r1, pc, r1
00389df0: pop {r4, lr}
00389df4: b #0x33ef7c
00389df8: subseq r8, r3, r4, lsl r5
# _Z14GetNewInstanceI13AnimatedDecorEP10ObjectBasev
00342600: push {r4, r5, r6, lr}
00342604: mov r1, #0
00342608: mov r0, #0x394
0034260c: bl #0x310570
00342610: ldr r6, [pc, #0x70]
00342614: mov r1, #0x14
00342618: mov r4, r0
0034261c: bl #0x38c398
00342620: ldr r3, [pc, #0x64]
00342624: add r6, pc, r6
00342628: add r2, r4, #0x37c
0034262c: ldr r3, [r6, r3]
00342630: mov r5, #1
00342634: mov r0, r2
00342638: add ip, r3, #8
0034263c: add r1, r3, #0xe4
00342640: add r3, r3, #0xd8
00342644: str r3, [r4, #4]
00342648: str r1, [r4, #0x24]
0034264c: str r2, [r4, #0x38c]
00342650: str r2, [r4, #0x390]
00342654: str ip, [r4]
00342658: strb r5, [r4, #0x375]
0034265c: strb r5, [r4, #0x376]
00342660: strb r5, [r4, #0x84]
00342664: mov r1, #0x10
00342668: bl #0x31167c
0034266c: ldr r2, [r4, #0x38c]
00342670: mov r3, #0
00342674: mov r0, r4
00342678: strb r3, [r2]
0034267c: strb r5, [r4, #0x84]
00342680: strb r3, [r4, #0x375]
00342684: pop {r4, r5, r6, pc}
00342688: rsbeq r2, r5, ip, ror #8
0034268c: andeq r1, r0, r8, lsr sp
# _ZN11POCharacter15onCollisionEndsEP18PhysicalBaseObjectRK7Point2DIfEb
0046fd28: push {r4, r5, r6, r7, r8, sb, sl, lr}
0046fd2c: ldr r4, [pc, #0xe8]
0046fd30: ldr r5, [pc, #0xe8]
0046fd34: sub sp, sp, #0x28
0046fd38: add r4, pc, r4
0046fd3c: ldr r2, [r4, r5]
0046fd40: mov sl, r3
0046fd44: ldr r2, [r2]
0046fd48: str r2, [sp, #0x24]
0046fd4c: ldr r6, [r0, #8]
0046fd50: ldr r7, [r1, #8]
0046fd54: cmp r7, #0
0046fd58: cmpne r6, #0
0046fd5c: bne #0x46fd7c
0046fd60: ldr r3, [r4, r5]
0046fd64: ldr r2, [sp, #0x24]
0046fd68: ldr r3, [r3]
0046fd6c: cmp r2, r3
0046fd70: bne #0x46fe18
0046fd74: add sp, sp, #0x28
0046fd78: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0046fd7c: ldr r3, [pc, #0xa0]
0046fd80: add r8, sp, #0xc
0046fd84: ldr sb, [r4, r3]
0046fd88: mov r0, sb
0046fd8c: bl #0x337888
0046fd90: ldr r1, [pc, #0x90]
0046fd94: mov r0, r8
0046fd98: str r8, [sp, #0x1c]
0046fd9c: add r1, pc, r1
0046fda0: add r1, r1, #0x19
0046fda4: str r8, [sp, #0x20]
0046fda8: bl #0x46fcd8
0046fdac: mov r0, sb
0046fdb0: mov r1, r8
0046fdb4: bl #0x337a88
0046fdb8: mov sb, r0
0046fdbc: mov r0, r8
0046fdc0: bl #0x3139ac
0046fdc4: cmp sb, #0
0046fdc8: bne #0x46fe04
0046fdcc: mov r1, r6
0046fdd0: mov r0, sp
0046fdd4: bl #0x33dd2c
0046fdd8: mov r0, sp
0046fddc: bl #0x33ff54
0046fde0: cmp r0, #0
0046fde4: mov r8, sp
0046fde8: beq #0x46fd60
0046fdec: cmp sl, #0
0046fdf0: movne r1, #0x3b
0046fdf4: moveq r1, #0x3c
0046fdf8: mov r2, r7
0046fdfc: bl #0x3a4d5c
0046fe00: b #0x46fd60
0046fe04: ldr r3, [r6]
0046fe08: mov r0, r6
0046fe0c: mov lr, pc
0046fe10: ldr pc, [r3, #0x28]
0046fe14: b #0x46fdcc
0046fe18: bl #0x30e310
0046fe1c: subseq r4, r2, r8, asr sp
0046fe20: andeq r4, r0, ip, lsr #1
0046fe24: andeq r0, r0, r4, lsl #17
0046fe28: subeq sp, r5, ip, ror r8
# _ZN11POCharacter18onCollisionResultsEP18PhysicalBaseObjectRK7Point2DIfEb
0046fb14: bx lr
# _ZN10ObjectBase17DeclarePropertiesEv
0033f014: push {r4, r5, r6, lr}
0033f018: ldr r1, [pc, #0x10c]
0033f01c: mov r4, r0
0033f020: add r5, r0, #4
0033f024: mov r0, r5
0033f028: add r2, r4, #0x84
0033f02c: ldrb r3, [r4, #0x84]
0033f030: add r1, pc, r1
0033f034: bl #0x33e4ac
0033f038: ldr r1, [pc, #0xf0]
0033f03c: mov r3, #1
0033f040: mov r0, r5
0033f044: add r2, r4, #0x80
0033f048: add r1, pc, r1
0033f04c: bl #0x33e4ac
0033f050: ldr r1, [pc, #0xdc]
0033f054: mov r0, r5
0033f058: add r2, r4, #0x30
0033f05c: add r1, pc, r1
0033f060: bl #0x33ef7c
0033f064: ldr r1, [pc, #0xcc]
0033f068: mov r0, r5
0033f06c: add r2, r4, #0x48
0033f070: add r1, pc, r1
0033f074: bl #0x33ef7c
0033f078: ldr r1, [pc, #0xbc]
0033f07c: mov r0, r5
0033f080: add r2, r4, #0x68
0033f084: add r1, pc, r1
0033f088: bl #0x33ef7c
0033f08c: ldr r1, [pc, #0xac]
0033f090: mov r0, r5
0033f094: add r2, r4, #0x83
0033f098: add r1, pc, r1
0033f09c: mov r3, #0
0033f0a0: bl #0x33e4ac
0033f0a4: ldr r1, [pc, #0x98]
0033f0a8: mov r3, #0
0033f0ac: mov r0, r5
0033f0b0: add r2, r4, #0x87
0033f0b4: add r1, pc, r1
0033f0b8: bl #0x33e4ac
0033f0bc: ldr r1, [pc, #0x84]
0033f0c0: mov r0, r5
0033f0c4: add r2, r4, #0x90
0033f0c8: add r1, pc, r1
0033f0cc: bl #0x33ef7c
0033f0d0: ldr r1, [pc, #0x74]
0033f0d4: mov r0, r5
0033f0d8: add r2, r4, #0xb4
0033f0dc: add r1, pc, r1
0033f0e0: bl #0x33ef7c
0033f0e4: ldr r1, [pc, #0x64]
0033f0e8: mov r0, r5
0033f0ec: add r2, r4, #0xd4
0033f0f0: add r1, pc, r1
0033f0f4: bl #0x33ef7c
0033f0f8: ldr r1, [pc, #0x54]
0033f0fc: mov r0, r5
0033f100: add r2, r4, #0xf0
0033f104: add r1, pc, r1
0033f108: mov r3, #0
0033f10c: bl #0x33e4ac
0033f110: ldr r1, [pc, #0x40]
0033f114: mov r0, r5
0033f118: add r2, r4, #0xf1
0033f11c: add r1, pc, r1
0033f120: mov r3, #0
0033f124: pop {r4, r5, r6, lr}
0033f128: b #0x33e4ac
0033f12c: subseq r1, r8, r8, lsr r1
0033f130: subseq r1, r8, r8, lsr #2
0033f134: subseq r2, sl, ip, lsl #1
0033f138: subseq r1, r8, r8, lsl #2
0033f13c: subseq r1, r8, r4, lsl #2
0033f140: subseq r1, r8, r0, lsl #2
0033f144: ldrsheq r1, [r8], #-4
0033f148: ldrsheq r1, [r8], #-0
0033f14c: subseq r1, r8, ip, ror #1
0033f150: subseq r1, r8, r8, ror #1
0033f154: subseq r1, r8, r4, ror #1
0033f158: subseq r1, r8, r4, ror #1
# _ZN11POCharacter19onCollisionPersistsEP18PhysicalBaseObjectRK7Point2DIfEb
0046ff6c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046ff70: ldr r4, [pc, #0xec]
0046ff74: ldr r5, [pc, #0xec]
0046ff78: sub sp, sp, #0x2c
0046ff7c: add r4, pc, r4
0046ff80: ldr r2, [r4, r5]
0046ff84: mov sl, r3
0046ff88: ldr r2, [r2]
0046ff8c: str r2, [sp, #0x24]
0046ff90: ldr r6, [r0, #8]
0046ff94: ldr r7, [r1, #8]
0046ff98: cmp r7, #0
0046ff9c: cmpne r6, #0
0046ffa0: bne #0x46ffc0
0046ffa4: ldr r3, [r4, r5]
0046ffa8: ldr r2, [sp, #0x24]
0046ffac: ldr r3, [r3]
0046ffb0: cmp r2, r3
0046ffb4: bne #0x470060
0046ffb8: add sp, sp, #0x2c
0046ffbc: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046ffc0: mov r1, r6
0046ffc4: mov r0, sp
0046ffc8: bl #0x33dd2c
0046ffcc: mov r0, sp
0046ffd0: bl #0x33ff54
0046ffd4: ldr r3, [pc, #0x90]
0046ffd8: mov fp, r0
0046ffdc: add r8, sp, #0xc
0046ffe0: ldr sb, [r4, r3]
0046ffe4: mov r0, sb
0046ffe8: bl #0x337888
0046ffec: ldr r1, [pc, #0x7c]
0046fff0: mov r0, r8
0046fff4: str r8, [sp, #0x1c]
0046fff8: add r1, pc, r1
0046fffc: add r1, r1, #0x19
00470000: str r8, [sp, #0x20]
00470004: bl #0x46fcd8
00470008: mov r0, sb
0047000c: mov r1, r8
00470010: bl #0x337a88
00470014: mov sb, r0
00470018: mov r0, r8
0047001c: bl #0x3139ac
00470020: cmp sb, #0
00470024: bne #0x47004c
00470028: cmp fp, #0
0047002c: beq #0x46ffa4
00470030: cmp sl, #0
00470034: movne r1, #0x39
00470038: moveq r1, #0x3a
0047003c: mov r0, fp
00470040: mov r2, r7
00470044: bl #0x3a4d5c
00470048: b #0x46ffa4
0047004c: mov r0, r6
00470050: ldr r3, [r6]
00470054: mov lr, pc
00470058: ldr pc, [r3, #0x28]
0047005c: b #0x470028
00470060: bl #0x30e310
00470064: subseq r4, r2, r4, lsl fp
00470068: andeq r4, r0, ip, lsr #1
0047006c: andeq r0, r0, r4, lsl #17
00470070: subeq sp, r5, r0, lsr #12
# _ZN11POCharacter17onCollisionBeginsEP18PhysicalBaseObjectRK7Point2DIfEb
0046fe68: push {r4, r5, r6, r7, r8, sb, sl, lr}
0046fe6c: ldr r4, [pc, #0xe8]
0046fe70: ldr r5, [pc, #0xe8]
0046fe74: sub sp, sp, #0x28
0046fe78: add r4, pc, r4
0046fe7c: ldr r2, [r4, r5]
0046fe80: mov sl, r3
0046fe84: ldr r2, [r2]
0046fe88: str r2, [sp, #0x24]
0046fe8c: ldr r6, [r0, #8]
0046fe90: ldr r7, [r1, #8]
0046fe94: cmp r7, #0
0046fe98: cmpne r6, #0
0046fe9c: bne #0x46febc
0046fea0: ldr r3, [r4, r5]
0046fea4: ldr r2, [sp, #0x24]
0046fea8: ldr r3, [r3]
0046feac: cmp r2, r3
0046feb0: bne #0x46ff58
0046feb4: add sp, sp, #0x28
0046feb8: pop {r4, r5, r6, r7, r8, sb, sl, pc}
0046febc: ldr r3, [pc, #0xa0]
0046fec0: add r8, sp, #0xc
0046fec4: ldr sb, [r4, r3]
0046fec8: mov r0, sb
0046fecc: bl #0x337888
0046fed0: ldr r1, [pc, #0x90]
0046fed4: mov r0, r8
0046fed8: str r8, [sp, #0x1c]
0046fedc: add r1, pc, r1
0046fee0: add r1, r1, #0x19
0046fee4: str r8, [sp, #0x20]
0046fee8: bl #0x46fcd8
0046feec: mov r0, sb
0046fef0: mov r1, r8
0046fef4: bl #0x337a88
0046fef8: mov sb, r0
0046fefc: mov r0, r8
0046ff00: bl #0x3139ac
0046ff04: cmp sb, #0
0046ff08: bne #0x46ff44
0046ff0c: mov r1, r6
0046ff10: mov r0, sp
0046ff14: bl #0x33dd2c
0046ff18: mov r0, sp
0046ff1c: bl #0x33ff54
0046ff20: cmp r0, #0
0046ff24: mov r8, sp
0046ff28: beq #0x46fea0
0046ff2c: cmp sl, #0
0046ff30: movne r1, #0x37
0046ff34: moveq r1, #0x38
0046ff38: mov r2, r7
0046ff3c: bl #0x3a4d5c
0046ff40: b #0x46fea0
0046ff44: ldr r3, [r6]
0046ff48: mov r0, r6
0046ff4c: mov lr, pc
0046ff50: ldr pc, [r3, #0x28]
0046ff54: b #0x46ff0c
0046ff58: bl #0x30e310
0046ff5c: subseq r4, r2, r8, lsl ip
0046ff60: andeq r4, r0, ip, lsr #1
0046ff64: andeq r0, r0, r4, lsl #17
0046ff68: subeq sp, r5, ip, lsr r7
# _ZN14PhysicalObject18onCollisionResultsEP18PhysicalBaseObjectb
003883d0: bx lr
# _ZN11POCharacter15onCollisionTestEP18PhysicalBaseObjectsttstt
0046fb4c: push {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046fb50: sub sp, sp, #0x2c
0046fb54: mov r8, r3
0046fb58: ldrh r3, [sp, #0x5c]
0046fb5c: mov r5, r0
0046fb60: add r4, sp, #0x1c
0046fb64: mov r0, r4
0046fb68: mov r7, r1
0046fb6c: ldr r1, [r5, #8]
0046fb70: mov sl, r2
0046fb74: str r3, [sp, #0x14]
0046fb78: ldrh sb, [sp, #0x50]
0046fb7c: ldrsh fp, [sp, #0x54]
0046fb80: ldrh r6, [sp, #0x58]
0046fb84: bl #0x33dd2c
0046fb88: mov r0, r4
0046fb8c: bl #0x33ff54
0046fb90: cmp r0, #0
0046fb94: beq #0x46fbd4
0046fb98: add r4, r0, #0x4f0
0046fb9c: add r4, r4, #0xc
0046fba0: mov r0, r4
0046fba4: bl #0x3c01c0
0046fba8: cmp r0, #0
0046fbac: beq #0x46fbbc
0046fbb0: mov r0, #0
0046fbb4: add sp, sp, #0x2c
0046fbb8: pop {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046fbbc: mov r0, r4
0046fbc0: bl #0x3c03a4
0046fbc4: cmp r0, #0
0046fbc8: beq #0x46fbd4
0046fbcc: tst r6, #3
0046fbd0: beq #0x46fbb0
0046fbd4: ldr ip, [sp, #0x14]
0046fbd8: mov r0, r5
0046fbdc: mov r1, r7
0046fbe0: mov r2, sl
0046fbe4: mov r3, r8
0046fbe8: stm sp, {sb, fp}
0046fbec: str r6, [sp, #8]
0046fbf0: str ip, [sp, #0xc]
0046fbf4: bl #0x46e6bc
0046fbf8: b #0x46fbb4
# _ZNK13AnimatedDecor9IsZonableEv
003884f8: b #0x38ab60
# _ZN14PhysicalObject17onCollisionBeginsEP18PhysicalBaseObjectRK7Point2DIfEb
003883c4: bx lr
# _ZN10ObjectBaseC2ENS_6GO_IDSE
0033f310: push {r4, r5, r6, r7, r8, lr}
0033f314: ldr r6, [pc, #0x198]
0033f318: ldr r2, [pc, #0x198]
0033f31c: ldr r3, [pc, #0x198]
0033f320: add r6, pc, r6
0033f324: ldr r2, [r6, r2]
0033f328: ldr r3, [r6, r3]
0033f32c: mov r4, r0
0033f330: add r2, r2, #8
0033f334: add r0, r3, #8
0033f338: add r3, r4, #8
0033f33c: str r2, [r4]
0033f340: str r0, [r4, #4]
0033f344: mov r7, r1
0033f348: mov r0, r3
0033f34c: str r3, [r4, #0x18]
0033f350: str r3, [r4, #0x1c]
0033f354: mov r1, #0x10
0033f358: bl #0x31167c
0033f35c: ldr r2, [pc, #0x15c]
0033f360: ldr r1, [r4, #0x18]
0033f364: mov r5, #0
0033f368: ldr r2, [r6, r2]
0033f36c: strb r5, [r1]
0033f370: add r3, r4, #0x30
0033f374: add r1, r2, #0x74
0033f378: add r0, r2, #8
0033f37c: add r2, r2, #0x68
0033f380: stm r4, {r0, r2}
0033f384: str r1, [r4, #0x24]
0033f388: mov r0, r3
0033f38c: str r5, [r4, #0x20]
0033f390: strb r5, [r4, #0x28]
0033f394: strb r5, [r4, #0x29]
0033f398: str r5, [r4, #0x2c]
0033f39c: str r3, [r4, #0x40]
0033f3a0: str r3, [r4, #0x44]
0033f3a4: mov r1, #0x10
0033f3a8: bl #0x31167c
0033f3ac: ldr r2, [r4, #0x40]
0033f3b0: add r3, r4, #0x48
0033f3b4: mov r0, r3
0033f3b8: strb r5, [r2]
0033f3bc: mov r1, #0x10
0033f3c0: str r3, [r4, #0x58]
0033f3c4: str r3, [r4, #0x5c]
0033f3c8: bl #0x31167c
0033f3cc: ldr r2, [r4, #0x58]
0033f3d0: add r3, r4, #0x68
0033f3d4: mvn r6, #0
0033f3d8: strb r5, [r2]
0033f3dc: mov r1, #0x10
0033f3e0: mov r0, r3
0033f3e4: strb r5, [r4, #0x60]
0033f3e8: str r3, [r4, #0x78]
0033f3ec: str r3, [r4, #0x7c]
0033f3f0: str r6, [r4, #0x64]
0033f3f4: bl #0x31167c
0033f3f8: ldr r3, [r4, #0x78]
0033f3fc: add r0, r4, #0x8c
0033f400: strb r5, [r3]
0033f404: mov r3, #1
0033f408: strb r3, [r4, #0x8a]
0033f40c: strb r5, [r4, #0x81]
0033f410: strb r5, [r4, #0x84]
0033f414: strb r5, [r4, #0x85]
0033f418: strb r5, [r4, #0x86]
0033f41c: strb r5, [r4, #0x88]
0033f420: strb r5, [r4, #0x89]
0033f424: bl #0x33ed7c
0033f428: add r0, r4, #0xb0
0033f42c: bl #0x33ed7c
0033f430: add r3, r4, #0xd4
0033f434: mov r0, r3
0033f438: str r3, [r4, #0xe4]
0033f43c: str r3, [r4, #0xe8]
0033f440: mov r1, #0x10
0033f444: bl #0x31167c
0033f448: ldr r3, [r4, #0xe4]
0033f44c: mov r1, r5
0033f450: mov r0, #0xc
0033f454: strb r5, [r3]
0033f458: mov r3, #0
0033f45c: str r3, [r4, #0x114]
0033f460: strb r5, [r4, #0xf0]
0033f464: strb r5, [r4, #0xf1]
0033f468: strb r5, [r4, #0xf8]
0033f46c: str r5, [r4, #0xfc]
0033f470: str r5, [r4, #0x100]
0033f474: str r5, [r4, #0x104]
0033f478: strb r5, [r4, #0x10c]
0033f47c: strb r5, [r4, #0x118]
0033f480: strb r5, [r4, #0x119]
0033f484: str r5, [r4, #0x11c]
0033f488: str r7, [r4, #0xf4]
0033f48c: str r6, [r4, #0x110]
0033f490: str r6, [r4, #0xec]
0033f494: str r6, [r4, #0x108]
0033f498: bl #0x310570
0033f49c: mov r5, r0
0033f4a0: bl #0x33f50c
0033f4a4: str r5, [r4, #0x2c]
0033f4a8: mov r0, r4
0033f4ac: str r4, [r5, #4]
0033f4b0: pop {r4, r5, r6, r7, r8, pc}
0033f4b4: rsbeq r5, r5, r0, ror r7
0033f4b8: andeq r1, r0, ip, lsl #1
0033f4bc: ldrdeq r3, r4, [r0], -ip
0033f4c0: andeq r3, r0, r4, lsl #23
# _ZN14PhysicalObject19onCollisionPersistsEP18PhysicalBaseObjectRK7Point2DIfEb
003883c8: bx lr
