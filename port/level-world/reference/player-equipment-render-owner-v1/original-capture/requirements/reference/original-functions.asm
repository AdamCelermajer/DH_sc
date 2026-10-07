
# _ZNK13ItemInventory17HasMainHandWeaponEv
003ffe8c: push     {r4, lr}
003ffe90: mov      r1, #1
003ffe94: mov      r4, r0
003ffe98: bl       #0x3fc6a8
003ffe9c: mov      r3, #0xc
003ffea0: mul      r3, r3, r0
003ffea4: ldr      r2, [r4, #0x14]
003ffea8: ldr      r3, [r2, r3]
003ffeac: ldr      r0, [r3, #4]
003ffeb0: subs     r0, r0, #0
003ffeb4: movne    r0, #1
003ffeb8: pop      {r4, pc}

# _ZN13ItemInventory15GetEquippedItemEj
003ffe3c: push     {r4, r5, r6, lr}
003ffe40: ldr      r3, [r0, #0x14]
003ffe44: mov      r5, r0
003ffe48: mov      r4, r1
003ffe4c: ldm      r3, {r2, r3}
003ffe50: rsb      r3, r2, r3
003ffe54: cmp      r1, r3, asr #2
003ffe58: blo      #0x3ffe64
003ffe5c: mov      r0, #0
003ffe60: pop      {r4, r5, r6, pc}
003ffe64: bl       #0x3fc6a8
003ffe68: mov      r3, #0xc
003ffe6c: mul      r3, r3, r0
003ffe70: ldr      r2, [r5, #0x14]
003ffe74: ldr      r3, [r2, r3]
003ffe78: ldr      r3, [r3, r4, lsl #2]
003ffe7c: cmp      r3, #0
003ffe80: beq      #0x3ffe5c
003ffe84: ldr      r0, [r3]
003ffe88: pop      {r4, r5, r6, pc}

# _ZNK13ItemInventory14IsDualWieldingEv
0040019c: b        #0x400158

# _ZNK13ItemInventory12HasTwoHanderEb
004001a0: push     {r4, r5, r6, lr}
004001a4: mov      r5, r1
004001a8: mov      r1, #1
004001ac: mov      r4, r0
004001b0: bl       #0x3fc6a8
004001b4: mov      r3, #0xc
004001b8: mul      r3, r3, r0
004001bc: ldr      r2, [r4, #0x14]
004001c0: ldr      r3, [r2, r3]
004001c4: ldr      r0, [r3, #4]
004001c8: cmp      r0, #0
004001cc: beq      #0x400218
004001d0: ldr      r0, [r0]
004001d4: bl       #0x3f9e08
004001d8: ldr      r3, [r0, #0x58]
004001dc: ldr      r2, [r4, #4]
004001e0: ldr      r0, [r0, #0x68]
004001e4: sub      r3, r3, #4
004001e8: cmp      r3, #1
004001ec: bls      #0x40021c
004001f0: cmp      r5, #0
004001f4: bne      #0x40021c
004001f8: cmn      r0, #4
004001fc: beq      #0x400208
00400200: mov      r0, r5
00400204: pop      {r4, r5, r6, pc}
00400208: movw     r3, #0x1324
0040020c: ldr      r0, [r2, r3]
00400210: rsbs     r0, r0, #1
00400214: movlo    r0, #0
00400218: pop      {r4, r5, r6, pc}
0040021c: cmn      r0, #4
00400220: movne    r0, #0
00400224: moveq    r0, #1
00400228: pop      {r4, r5, r6, pc}

# _ZN9Character24INV_DoesMeetRequirementsEP12ItemInstance
003a4930: push     {r4, r5, r6, lr}
003a4934: mov      r4, r0
003a4938: mov      r5, r1
003a493c: bl       #0x7fd794
003a4940: ldrb     r3, [r0, #5]
003a4944: cmp      r3, #0
003a4948: bne      #0x3a4978
003a494c: cmp      r5, #0
003a4950: beq      #0x3a4990
003a4954: mov      r0, r5
003a4958: bl       #0x3f9e08
003a495c: movw     r3, #0x1044
003a4960: ldr      r2, [r4, r3]
003a4964: ldr      r3, [r0, #0x74]
003a4968: cmp      r3, r2, asr #8
003a496c: ble      #0x3a4998
003a4970: mov      r0, #0
003a4974: pop      {r4, r5, r6, pc}
003a4978: ldr      r3, [r4]
003a497c: mov      r0, r4
003a4980: mov      lr, pc
003a4984: ldr      pc, [r3, #0x54]
003a4988: cmp      r0, #0
003a498c: beq      #0x3a494c
003a4990: mov      r0, #1
003a4994: pop      {r4, r5, r6, pc}
003a4998: movw     r3, #0x124c
003a499c: ldr      r2, [r4, r3]
003a49a0: ldr      r3, [r0, #0x78]
003a49a4: cmp      r3, r2, asr #8
003a49a8: bgt      #0x3a4970
003a49ac: movw     r3, #0x1250
003a49b0: ldr      r2, [r4, r3]
003a49b4: ldr      r3, [r0, #0x7c]
003a49b8: cmp      r3, r2, asr #8
003a49bc: bgt      #0x3a4970
003a49c0: movw     r3, #0x1254
003a49c4: ldr      r2, [r4, r3]
003a49c8: ldr      r3, [r0, #0x80]
003a49cc: cmp      r3, r2, asr #8
003a49d0: bgt      #0x3a4970
003a49d4: movw     r3, #0x1258
003a49d8: ldr      r2, [r4, r3]
003a49dc: ldr      r3, [r0, #0x84]
003a49e0: cmp      r3, r2, asr #8
003a49e4: movgt    r0, #0
003a49e8: movle    r0, #1
003a49ec: pop      {r4, r5, r6, pc}

# _ZNK13ItemInventory20GetNumEquipmentSlotsEv
003ffd20: ldr      r3, [r0, #0x14]
003ffd24: ldr      r2, [r3]
003ffd28: ldr      r0, [r3, #4]
003ffd2c: rsb      r0, r2, r0
003ffd30: asr      r0, r0, #2
003ffd34: bx       lr
