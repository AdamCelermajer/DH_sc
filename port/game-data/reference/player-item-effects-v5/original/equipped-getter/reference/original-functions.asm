
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

# _ZNK13ItemInventory18GetCurrentEquipSetEi
003fc6a8: cmp      r1, #0
003fc6ac: blt      #0x3fc6c0
003fc6b0: sub      r1, r1, #1
003fc6b4: cmp      r1, #1
003fc6b8: movhi    r0, #0
003fc6bc: bxhi     lr
003fc6c0: ldrsb    r0, [r0, #0x2e]
003fc6c4: bx       lr

# _ZNK13ItemInventory20GetNumEquipmentSlotsEv
003ffd20: ldr      r3, [r0, #0x14]
003ffd24: ldr      r2, [r3]
003ffd28: ldr      r0, [r3, #4]
003ffd2c: rsb      r0, r2, r0
003ffd30: asr      r0, r0, #2
003ffd34: bx       lr
