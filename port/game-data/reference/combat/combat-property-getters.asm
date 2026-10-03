
# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZNK14CharProperties20PROPS_GetBonusDamageEb
003df8ac: push     {r4, r5, r6, r7, r8, lr}
003df8b0: mov      r4, r0
003df8b4: ldr      r0, [r0, #4]
003df8b8: cmp      r1, #0
003df8bc: movne    r1, #2
003df8c0: moveq    r1, #1
003df8c4: add      r0, r0, #0x37c
003df8c8: bl       #0x3ffe3c
003df8cc: cmp      r0, #0
003df8d0: beq      #0x3df938
003df8d4: bl       #0x3f9e08
003df8d8: ldr      r2, [r0, #0x94]
003df8dc: cmn      r2, #1
003df8e0: beq      #0x3df938
003df8e4: add      r5, r4, #0xa90
003df8e8: add      r5, r5, #4
003df8ec: add      r2, r2, #0x53
003df8f0: mov      r1, r5
003df8f4: mov      r0, r4
003df8f8: bl       #0x3dedb4
003df8fc: mov      r7, r0
003df900: ldr      r0, [r4, #4]
003df904: mov      r1, #1
003df908: add      r0, r0, #0x37c
003df90c: bl       #0x4001a0
003df910: cmp      r0, #0
003df914: bne      #0x3df940
003df918: ldr      r3, [r4, #4]
003df91c: add      r6, r0, r7
003df920: add      r0, r3, #0x37c
003df924: bl       #0x40019c
003df928: cmp      r0, #0
003df92c: bne      #0x3df968
003df930: add      r0, r6, r0
003df934: pop      {r4, r5, r6, r7, r8, pc}
003df938: mov      r0, #0
003df93c: pop      {r4, r5, r6, r7, r8, pc}
003df940: mov      r1, r5
003df944: mov      r2, #0x5b
003df948: mov      r0, r4
003df94c: bl       #0x3dedb4
003df950: ldr      r3, [r4, #4]
003df954: add      r6, r0, r7
003df958: add      r0, r3, #0x37c
003df95c: bl       #0x40019c
003df960: cmp      r0, #0
003df964: beq      #0x3df930
003df968: mov      r0, r4
003df96c: mov      r1, r5
003df970: mov      r2, #0x5a
003df974: bl       #0x3dedb4
003df978: add      r0, r6, r0
003df97c: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK14CharProperties12_GetPropertyERKN7Structs19CharacterPropertiesEi
003dedb4: str      lr, [sp, #-4]!
003dedb8: ldr      r3, [pc, #0xf0]
003dedbc: cmp      r2, #0
003dedc0: sub      sp, sp, #0xc
003dedc4: add      r3, pc, r3
003dedc8: blt      #0x3dedfc
003dedcc: cmp      r2, #0xdf
003dedd0: ble      #0x3dee20
003dedd4: ldr      r2, [pc, #0xd8]
003dedd8: ldr      r2, [r3, r2]
003deddc: ldr      r2, [r2]
003dede0: cmp      r2, #2
003dede4: beq      #0x3dee10
003dede8: cmp      r2, #1
003dedec: beq      #0x3dee78
003dedf0: mvn      r0, #0
003dedf4: add      sp, sp, #0xc
003dedf8: ldm      sp!, {pc}
003dedfc: ldr      r2, [pc, #0xb0]
003dee00: ldr      r2, [r3, r2]
003dee04: ldr      r2, [r2]
003dee08: cmp      r2, #2
003dee0c: bne      #0x3dee38
003dee10: mov      r3, #0
003dee14: str      r3, [r3]
003dee18: mvn      r0, #0
003dee1c: b        #0x3dedf4
003dee20: ldr      r0, [pc, #0x90]
003dee24: ldr      r3, [r3, r0]
003dee28: ldr      r3, [r3, r2, lsl #2]
003dee2c: add      r1, r1, r3
003dee30: ldr      r0, [r1, #4]
003dee34: b        #0x3dedf4
003dee38: cmp      r2, #1
003dee3c: bne      #0x3dedf0
003dee40: ldr      r0, [pc, #0x74]
003dee44: ldr      r1, [pc, #0x74]
003dee48: ldr      r2, [pc, #0x74]
003dee4c: ldr      r0, [r3, r0]
003dee50: ldr      r3, [pc, #0x70]
003dee54: movw     ip, #0x103
003dee58: add      r1, pc, r1
003dee5c: add      r0, r0, #0xa8
003dee60: add      r2, pc, r2
003dee64: add      r3, pc, r3
003dee68: str      ip, [sp]
003dee6c: bl       #0x30e004
003dee70: mvn      r0, #0
003dee74: b        #0x3dedf4
003dee78: ldr      r0, [pc, #0x3c]
003dee7c: ldr      r1, [pc, #0x48]
003dee80: ldr      r2, [pc, #0x48]
003dee84: ldr      r0, [r3, r0]
003dee88: ldr      r3, [pc, #0x44]
003dee8c: mov      ip, #0x104
003dee90: add      r1, pc, r1
003dee94: add      r0, r0, #0xa8
003dee98: add      r2, pc, r2
003dee9c: add      r3, pc, r3
003deea0: str      ip, [sp]
003deea4: bl       #0x30e004
003deea8: mvn      r0, #0
003deeac: b        #0x3dedf4
003deeb0: subseq   r5, fp, ip, asr #25
003deeb4: andeq    r3, r0, r0, asr #19
003deeb8: andeq    r2, r0, r8, lsr #5
003deebc: andeq    r1, r0, r0, asr #19
003deec0: subeq    pc, sp, r0, lsl #11
003deec4: strheq   r6, [lr], #-0xe0
003deec8: subeq    r6, lr, ip, asr #28
003deecc: subeq    pc, sp, r8, asr #10
003deed0: subeq    r6, lr, r8, lsl #29
003deed4: subeq    r6, lr, r4, lsl lr
