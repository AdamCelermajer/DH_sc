
# _ZNK14CharProperties14_IsPropertySetERKN7Structs19CharacterPropertiesEi
003df114: push     {r4, r5, r6, lr}
003df118: mov      r4, r2
003df11c: mov      r5, r0
003df120: bl       #0x3dedb4
003df124: mov      r1, r4
003df128: mov      r6, r0
003df12c: mov      r0, r5
003df130: bl       #0x3def10
003df134: subs     r0, r6, r0
003df138: movne    r0, #1
003df13c: pop      {r4, r5, r6, pc}

# _ZN14CharProperties21UpdateGearsPropertiesEv
003e08a8: push     {r4, lr}
003e08ac: mov      r4, r0
003e08b0: bl       #0x3defac
003e08b4: mov      r0, r4
003e08b8: bl       #0x3df480
003e08bc: mov      r0, r4
003e08c0: mov      r1, #1
003e08c4: pop      {r4, lr}
003e08c8: b        #0x3e0810

# _ZN14CharProperties12_SetPropertyERN7Structs19CharacterPropertiesEii
003deca0: str      lr, [sp, #-4]!
003deca4: ldr      ip, [pc, #0xe0]
003deca8: cmp      r2, #0
003decac: sub      sp, sp, #0xc
003decb0: add      ip, pc, ip
003decb4: blt      #0x3dece4
003decb8: cmp      r2, #0xdf
003decbc: ble      #0x3ded04
003decc0: ldr      r3, [pc, #0xc8]
003decc4: ldr      r3, [ip, r3]
003decc8: ldr      r3, [r3]
003deccc: cmp      r3, #2
003decd0: beq      #0x3decf8
003decd4: cmp      r3, #1
003decd8: beq      #0x3ded58
003decdc: add      sp, sp, #0xc
003dece0: ldm      sp!, {pc}
003dece4: ldr      r3, [pc, #0xa4]
003dece8: ldr      r3, [ip, r3]
003decec: ldr      r3, [r3]
003decf0: cmp      r3, #2
003decf4: bne      #0x3ded1c
003decf8: mov      r3, #0
003decfc: str      r3, [r3]
003ded00: b        #0x3decdc
003ded04: ldr      r0, [pc, #0x88]
003ded08: ldr      r0, [ip, r0]
003ded0c: ldr      r2, [r0, r2, lsl #2]
003ded10: add      r1, r1, r2
003ded14: str      r3, [r1, #4]
003ded18: b        #0x3decdc
003ded1c: cmp      r3, #1
003ded20: bne      #0x3decdc
003ded24: ldr      r0, [pc, #0x6c]
003ded28: ldr      r1, [pc, #0x6c]
003ded2c: ldr      r2, [pc, #0x6c]
003ded30: ldr      r0, [ip, r0]
003ded34: ldr      r3, [pc, #0x68]
003ded38: movw     ip, #0x113
003ded3c: add      r1, pc, r1
003ded40: add      r2, pc, r2
003ded44: add      r3, pc, r3
003ded48: add      r0, r0, #0xa8
003ded4c: str      ip, [sp]
003ded50: bl       #0x30e004
003ded54: b        #0x3decdc
003ded58: ldr      r0, [pc, #0x38]
003ded5c: ldr      r1, [pc, #0x44]
003ded60: ldr      r2, [pc, #0x44]
003ded64: ldr      r0, [ip, r0]
003ded68: ldr      r3, [pc, #0x40]
003ded6c: mov      ip, #0x114
003ded70: add      r1, pc, r1
003ded74: add      r2, pc, r2
003ded78: add      r3, pc, r3
003ded7c: add      r0, r0, #0xa8
003ded80: str      ip, [sp]
003ded84: bl       #0x30e004
003ded88: b        #0x3decdc
003ded8c: subseq   r5, fp, r0, ror #27
003ded90: andeq    r3, r0, r0, asr #19
003ded94: andeq    r2, r0, r8, lsr #5
003ded98: andeq    r1, r0, r0, asr #19
003ded9c: umaaleq  pc, sp, ip, r6
003deda0: ldrdeq   r6, r7, [lr], #-0xf0
003deda4: subeq    r6, lr, ip, ror #30
003deda8: subeq    pc, sp, r8, ror #12
003dedac: subeq    r6, lr, ip, lsr #31
003dedb0: subeq    r6, lr, r8, lsr pc
