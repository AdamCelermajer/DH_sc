
# _ZNK14CharProperties11_GetDefaultEi
003def10: ldr      r3, [pc, #0x14]
003def14: mov      r2, r1
003def18: ldr      r1, [pc, #0x10]
003def1c: add      r3, pc, r3
003def20: ldr      ip, [r3, r1]
003def24: ldr      r1, [ip]
003def28: b        #0x3dedb4
003def2c: subseq   r5, fp, r4, ror fp
003def30: andeq    r2, r0, r0, asr fp

# _ZN14CharProperties12_AddPropertyERN7Structs19CharacterPropertiesEii
003df140: push     {r4, r5, r6, r7, r8, lr}
003df144: mov      r7, r3
003df148: mov      r6, r0
003df14c: mov      r5, r1
003df150: mov      r4, r2
003df154: bl       #0x3df114
003df158: cmp      r0, #0
003df15c: bne      #0x3df178
003df160: mov      r0, r6
003df164: mov      r1, r5
003df168: mov      r2, r4
003df16c: mov      r3, r7
003df170: pop      {r4, r5, r6, r7, r8, lr}
003df174: b        #0x3deca0
003df178: mov      r1, r5
003df17c: mov      r2, r4
003df180: mov      r0, r6
003df184: bl       #0x3dedb4
003df188: mov      r1, r5
003df18c: add      r3, r0, r7
003df190: mov      r2, r4
003df194: mov      r0, r6
003df198: pop      {r4, r5, r6, r7, r8, lr}
003df19c: b        #0x3deca0
