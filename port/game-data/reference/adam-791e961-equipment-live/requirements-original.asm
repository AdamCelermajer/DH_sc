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
