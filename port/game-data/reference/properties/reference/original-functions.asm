
# _GLOBAL__I_.._.._sources_Game_Objects_Characters_Properties_CharProperties.cpp
003df548: push     {r4, r5, r6, r7, r8, lr}
003df54c: ldr      r4, [pc, #0x148]
003df550: ldr      r6, [pc, #0x148]
003df554: ldr      r3, [pc, #0x148]
003df558: add      r4, pc, r4
003df55c: ldr      r2, [pc, #0x144]
003df560: ldr      r3, [r4, r3]
003df564: ldr      r5, [r4, r6]
003df568: ldr      r1, [pc, #0x13c]
003df56c: mov      ip, #0
003df570: mov      r0, #0x3f000000
003df574: add      r2, pc, r2
003df578: str      r0, [r2, #8]
003df57c: str      r0, [r2]
003df580: str      r0, [r2, #4]
003df584: str      ip, [r3, #8]
003df588: str      ip, [r3]
003df58c: str      ip, [r3, #4]
003df590: ldr      r1, [r4, r1]
003df594: mov      r0, r3
003df598: mov      r2, r5
003df59c: bl       #0x30e304
003df5a0: ldr      ip, [pc, #0x108]
003df5a4: ldr      r3, [pc, #0x108]
003df5a8: ldr      r2, [pc, #0x108]
003df5ac: ldr      ip, [r4, ip]
003df5b0: ldr      r3, [r4, r3]
003df5b4: ldr      r1, [r4, r2]
003df5b8: add      ip, ip, #8
003df5bc: mov      r0, r3
003df5c0: mov      r2, r5
003df5c4: str      ip, [r3]
003df5c8: bl       #0x30e304
003df5cc: ldr      r3, [pc, #0xe8]
003df5d0: ldr      r3, [r4, r3]
003df5d4: ldr      r2, [r3]
003df5d8: tst      r2, #1
003df5dc: beq      #0x3df66c
003df5e0: ldr      r3, [pc, #0xd8]
003df5e4: ldr      r3, [r4, r3]
003df5e8: ldr      r2, [r3]
003df5ec: tst      r2, #1
003df5f0: beq      #0x3df63c
003df5f4: ldr      r3, [pc, #0xc8]
003df5f8: ldr      r3, [r4, r3]
003df5fc: ldr      r2, [r3]
003df600: tst      r2, #1
003df604: beq      #0x3df60c
003df608: pop      {r4, r5, r6, r7, r8, pc}
003df60c: mov      r2, #1
003df610: str      r2, [r3]
003df614: ldr      r3, [pc, #0xac]
003df618: ldr      r5, [r4, r3]
003df61c: mov      r0, r5
003df620: bl       #0x4932c4
003df624: ldr      r3, [pc, #0xa0]
003df628: ldr      r2, [r4, r6]
003df62c: mov      r0, r5
003df630: ldr      r1, [r4, r3]
003df634: pop      {r4, r5, r6, r7, r8, lr}
003df638: b        #0x30e304
003df63c: mov      r2, #1
003df640: str      r2, [r3]
003df644: ldr      r3, [pc, #0x84]
003df648: ldr      r5, [r4, r3]
003df64c: mov      r0, r5
003df650: bl       #0x32d79c
003df654: ldr      r3, [pc, #0x78]
003df658: mov      r0, r5
003df65c: ldr      r2, [r4, r6]
003df660: ldr      r1, [r4, r3]
003df664: bl       #0x30e304
003df668: b        #0x3df5f4
003df66c: mov      r2, #1
003df670: str      r2, [r3]
003df674: ldr      r3, [pc, #0x5c]
003df678: ldr      r7, [r4, r3]
003df67c: mov      r0, r7
003df680: bl       #0x3790a8
003df684: ldr      r3, [pc, #0x50]
003df688: mov      r0, r7
003df68c: mov      r2, r5
003df690: ldr      r1, [r4, r3]
003df694: bl       #0x30e304
003df698: b        #0x3df5e0
003df69c: subseq   r5, fp, r8, lsr r5
003df6a0: muleq    r0, r0, r8
003df6a4: andeq    r1, r0, r0, lsl #3
003df6a8: subseq   r3, ip, r4, ror #13
003df6ac: andeq    r4, r0, ip, lsl r2
003df6b0: andeq    r2, r0, ip, lsl #19
003df6b4: andeq    r1, r0, ip, asr #32
003df6b8: andeq    r3, r0, r8, ror ip
003df6bc: strdeq   r0, r1, [r0], -r4
003df6c0: andeq    r0, r0, ip, lsr #31
003df6c4: strheq   r4, [r0], -r4
003df6c8: andeq    r1, r0, r8, lsl #22
003df6cc: andeq    r4, r0, r0, ror r1
003df6d0: strdeq   r3, r4, [r0], -r4
003df6d4: andeq    r0, r0, r0, asr #17
003df6d8: andeq    r2, r0, r4, lsl r7
003df6dc: muleq    r0, ip, r5

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

# _ZN14CharProperties18_LoadFromCharTableERN7Structs19CharacterPropertiesEi
003df250: ldr      r3, [pc, #0x40]
003df254: subs     ip, r2, #0
003df258: add      r3, pc, r3
003df25c: bxlt     lr
003df260: ldr      r2, [pc, #0x34]
003df264: ldr      r2, [r3, r2]
003df268: ldr      r2, [r2]
003df26c: cmp      ip, r2
003df270: bxge     lr
003df274: ldr      r2, [pc, #0x24]
003df278: add      r0, r1, #4
003df27c: mov      r1, #0x384
003df280: ldr      r3, [r3, r2]
003df284: mov      r2, #0x380
003df288: ldr      r3, [r3]
003df28c: mla      ip, r1, ip, r3
003df290: add      r1, ip, #4
003df294: b        #0x30e868
003df298: subseq   r5, fp, r8, lsr r8
003df29c: andeq    r4, r0, r4, lsl #4
003df2a0: andeq    r2, r0, r0, asr fp

# _ZN14CharProperties16PROPS_SetToSheetEiiPN7Structs19CharacterPropertiesE
003e0614: push     {r4, r5, r6, r7, lr}
003e0618: ldr      ip, [pc, #0xd0]
003e061c: subs     r7, r3, #0
003e0620: sub      sp, sp, #0xc
003e0624: add      ip, pc, ip
003e0628: mov      r6, r2
003e062c: mov      r5, r0
003e0630: mov      r4, r1
003e0634: beq      #0x3e069c
003e0638: bl       #0x3deed8
003e063c: tst      r0, #4
003e0640: bne      #0x3e0674
003e0644: tst      r0, #8
003e0648: bne      #0x3e0654
003e064c: add      sp, sp, #0xc
003e0650: pop      {r4, r5, r6, r7, pc}
003e0654: add      r1, r5, #0xa90
003e0658: mov      r0, r5
003e065c: add      r1, r1, #4
003e0660: mov      r2, r4
003e0664: mov      r3, r6
003e0668: add      sp, sp, #0xc
003e066c: pop      {r4, r5, r6, r7, lr}
003e0670: b        #0x3deca0
003e0674: mov      r1, r7
003e0678: mov      r0, r5
003e067c: mov      r3, r6
003e0680: mov      r2, r4
003e0684: bl       #0x3deca0
003e0688: mov      r0, r5
003e068c: mov      r1, r4
003e0690: add      sp, sp, #0xc
003e0694: pop      {r4, r5, r6, r7, lr}
003e0698: b        #0x3dfe60
003e069c: ldr      r3, [pc, #0x50]
003e06a0: ldr      r3, [ip, r3]
003e06a4: ldr      r3, [r3]
003e06a8: cmp      r3, #2
003e06ac: streq    r7, [r7]
003e06b0: beq      #0x3e064c
003e06b4: cmp      r3, #1
003e06b8: bne      #0x3e064c
003e06bc: ldr      r0, [pc, #0x34]
003e06c0: ldr      r1, [pc, #0x34]
003e06c4: ldr      r2, [pc, #0x34]
003e06c8: ldr      r0, [ip, r0]
003e06cc: ldr      r3, [pc, #0x30]
003e06d0: movw     ip, #0x326
003e06d4: add      r1, pc, r1
003e06d8: add      r2, pc, r2
003e06dc: add      r3, pc, r3
003e06e0: add      r0, r0, #0xa8
003e06e4: str      ip, [sp]
003e06e8: bl       #0x30e004
003e06ec: b        #0x3e064c
003e06f0: subseq   r4, fp, ip, ror #8
003e06f4: andeq    r3, r0, r0, asr #19
003e06f8: andeq    r1, r0, r0, asr #19
003e06fc: subeq    sp, sp, r4, lsl #26
003e0700: strdeq   r3, r4, [lr], #-0x70
003e0704: ldrdeq   r5, r6, [lr], #-0x54

# _ZNSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE10_M_advanceEi
003de8b4: ldr      r3, [r0]
003de8b8: ldr      r2, [r0, #4]
003de8bc: str      r4, [sp, #-4]!
003de8c0: rsb      r2, r2, r3
003de8c4: add      r2, r1, r2, asr #2
003de8c8: mvn      ip, r2
003de8cc: lsr      r4, ip, #0x1f
003de8d0: cmp      r2, #0x1f
003de8d4: movgt    r4, #0
003de8d8: andle    r4, r4, #1
003de8dc: cmp      r4, #0
003de8e0: addne    r3, r3, r1, lsl #2
003de8e4: strne    r3, [r0]
003de8e8: bne      #0x3de920
003de8ec: ldr      r1, [r0, #0xc]
003de8f0: cmp      r2, #0
003de8f4: lsrgt    r3, r2, #5
003de8f8: mvnle    r3, ip, lsr #5
003de8fc: add      ip, r1, r3, lsl #2
003de900: str      ip, [r0, #0xc]
003de904: sub      r2, r2, r3, lsl #5
003de908: ldr      r3, [r1, r3, lsl #2]
003de90c: add      r2, r3, r2, lsl #2
003de910: add      r1, r3, #0x80
003de914: str      r2, [r0]
003de918: str      r1, [r0, #8]
003de91c: str      r3, [r0, #4]
003de920: ldm      sp!, {r4}
003de924: bx       lr

# _ZN14CharProperties9PROPS_SetEii
003e07a0: push     {r4, r5, r6, lr}
003e07a4: mov      r6, r2
003e07a8: mov      r4, r0
003e07ac: mov      r5, r1
003e07b0: bl       #0x3deed8
003e07b4: tst      r0, #0x20
003e07b8: bne      #0x3e07e4
003e07bc: tst      r0, #8
003e07c0: bne      #0x3e07c8
003e07c4: pop      {r4, r5, r6, pc}
003e07c8: add      r1, r4, #0xa90
003e07cc: mov      r0, r4
003e07d0: add      r1, r1, #4
003e07d4: mov      r2, r5
003e07d8: mov      r3, r6
003e07dc: pop      {r4, r5, r6, lr}
003e07e0: b        #0x3deca0
003e07e4: mov      r0, r4
003e07e8: add      r1, r4, #0x38c
003e07ec: mov      r3, r6
003e07f0: mov      r2, r5
003e07f4: bl       #0x3deca0
003e07f8: mov      r0, r4
003e07fc: mov      r1, r5
003e0800: pop      {r4, r5, r6, lr}
003e0804: b        #0x3dfe60

# _ZNK14CharProperties8_GetTypeEi
003deed8: ldr      r3, [pc, #0x28]
003deedc: mov      r2, r1
003deee0: ldr      r1, [pc, #0x24]
003deee4: push     {r4, lr}
003deee8: add      r3, pc, r3
003deeec: ldr      ip, [r3, r1]
003deef0: ldr      r1, [ip]
003deef4: add      r1, r1, #0x384
003deef8: bl       #0x3dedb4
003deefc: cmn      r0, #1
003def00: moveq    r0, #0x10
003def04: pop      {r4, pc}
003def08: subseq   r5, fp, r8, lsr #23
003def0c: andeq    r2, r0, r0, asr fp

# _ZNKSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE11_M_subtractERKS4_
003de870: push     {r4, r5}
003de874: ldr      ip, [r1, #0xc]
003de878: ldr      r5, [r0]
003de87c: ldr      r2, [r0, #4]
003de880: ldr      r4, [r0, #0xc]
003de884: ldr      r3, [r1, #8]
003de888: ldr      r1, [r1]
003de88c: rsb      r2, r2, r5
003de890: rsb      r0, ip, r4
003de894: rsb      r3, r1, r3
003de898: asr      r2, r2, #2
003de89c: asr      r0, r0, #2
003de8a0: add      r3, r2, r3, asr #2
003de8a4: sub      r0, r0, #1
003de8a8: add      r0, r3, r0, lsl #5
003de8ac: pop      {r4, r5}
003de8b0: bx       lr

# _ZN14CharProperties12PROPS_SetIntEii
003e0808: lsl      r2, r2, #8
003e080c: b        #0x3e07a0

# _ZN14CharProperties20ResetGearsPropertiesEv
003defac: add      r1, r0, #0x710
003defb0: b        #0x3def34

# _ZN14CharPropertiesC2Ev
003df084: push     {r4, r5, r6, lr}
003df088: ldr      r5, [pc, #0x74]
003df08c: ldr      r3, [pc, #0x74]
003df090: ldr      r1, [pc, #0x74]
003df094: add      r5, pc, r5
003df098: ldr      r3, [r5, r3]
003df09c: ldr      r1, [r5, r1]
003df0a0: add      r2, r0, #0xe10
003df0a4: add      r2, r2, #8
003df0a8: add      r1, r1, #8
003df0ac: add      ip, r3, #8
003df0b0: mov      r3, #0
003df0b4: str      ip, [r0]
003df0b8: str      r2, [r0, #0xe24]
003df0bc: str      r2, [r0, #0xe20]
003df0c0: str      r1, [r0, #0xa94]
003df0c4: str      r3, [r0, #0xe30]
003df0c8: str      r3, [r0, #4]
003df0cc: str      r1, [r0, #8]
003df0d0: str      r1, [r0, #0x38c]
003df0d4: str      r1, [r0, #0x710]
003df0d8: str      r3, [r0, #0xe1c]
003df0dc: strb     r3, [r0, #0xe18]
003df0e0: str      r3, [r0, #0xe28]
003df0e4: mov      r4, r0
003df0e8: bl       #0x3defc4
003df0ec: ldr      r3, [pc, #0x1c]
003df0f0: mov      r0, r4
003df0f4: ldr      r1, [r5, r3]
003df0f8: bl       #0x3def34
003df0fc: mov      r0, r4
003df100: pop      {r4, r5, r6, pc}
003df104: ldrsheq  r5, [fp], #-0x9c
003df108: ldrdeq   r3, r4, [r0], -r0
003df10c: andeq    r2, r0, ip, lsl #19
003df110: andeq    r1, r0, ip, asr #32

# _ZN14CharProperties20ResetSavedPropertiesEv
003defb4: add      r1, r0, #0x38c
003defb8: b        #0x3def34

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

# _ZN14CharProperties16PROPS_ApplyClassEib
003df3b8: ldr      ip, [pc, #0x28]
003df3bc: subs     r3, r2, #0
003df3c0: mov      r2, r1
003df3c4: add      ip, pc, ip
003df3c8: bne      #0x3df3d8
003df3cc: add      r1, r0, #0xa90
003df3d0: add      r1, r1, #4
003df3d4: b        #0x3e2e20
003df3d8: ldr      r1, [pc, #0xc]
003df3dc: mov      r3, #1
003df3e0: ldr      r1, [ip, r1]
003df3e4: b        #0x3e2e20
003df3e8: subseq   r5, fp, ip, asr #13
003df3ec: andeq    r1, r0, ip, asr #32

# _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
003def34: ldr      r3, [pc, #0x40]
003def38: ldr      r2, [pc, #0x40]
003def3c: push     {r4, r5, r6, r7, r8, lr}
003def40: add      r3, pc, r3
003def44: mov      r8, r0
003def48: ldr      r7, [r3, r2]
003def4c: mov      r6, r1
003def50: mov      r4, #0
003def54: mov      r1, r4
003def58: mov      r0, r8
003def5c: ldr      r5, [r7, r4, lsl #2]
003def60: bl       #0x3def10
003def64: add      r4, r4, #1
003def68: add      r5, r5, #4
003def6c: cmp      r4, #0xe0
003def70: str      r0, [r6, r5]
003def74: bne      #0x3def54
003def78: pop      {r4, r5, r6, r7, r8, pc}
003def7c: subseq   r5, fp, r0, asr fp
003def80: andeq    r2, r0, r8, lsr #5

# _ZN14CharProperties9PROPS_AddEii
003e0708: push     {r4, r5, r6, r7, r8, lr}
003e070c: mov      r7, r2
003e0710: mov      r4, r0
003e0714: mov      r5, r1
003e0718: bl       #0x3deed8
003e071c: tst      r0, #0x20
003e0720: bne      #0x3e0760
003e0724: tst      r0, #8
003e0728: bne      #0x3e0730
003e072c: pop      {r4, r5, r6, r7, r8, pc}
003e0730: add      r6, r4, #0xa90
003e0734: add      r6, r6, #4
003e0738: mov      r1, r6
003e073c: mov      r2, r5
003e0740: mov      r0, r4
003e0744: bl       #0x3dedb4
003e0748: mov      r1, r6
003e074c: add      r3, r0, r7
003e0750: mov      r2, r5
003e0754: mov      r0, r4
003e0758: pop      {r4, r5, r6, r7, r8, lr}
003e075c: b        #0x3deca0
003e0760: add      r6, r4, #0x38c
003e0764: mov      r1, r6
003e0768: mov      r2, r5
003e076c: mov      r0, r4
003e0770: bl       #0x3dedb4
003e0774: mov      r1, r6
003e0778: add      r3, r0, r7
003e077c: mov      r2, r5
003e0780: mov      r0, r4
003e0784: bl       #0x3deca0
003e0788: mov      r0, r4
003e078c: mov      r1, r5
003e0790: pop      {r4, r5, r6, r7, r8, lr}
003e0794: b        #0x3dfe60

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

# _ZN14CharProperties19ResetBasePropertiesEv
003defbc: add      r1, r0, #8
003defc0: b        #0x3def34

# _ZN14CharProperties18LoadBasePropertiesEi
003df2a4: mov      r2, r1
003df2a8: add      r1, r0, #8
003df2ac: b        #0x3df250

# _ZN14CharProperties14RecalcPropertyEi
003dfe60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dfe64: sub      sp, sp, #0x54
003dfe68: mov      r6, r0
003dfe6c: mov      r7, r1
003dfe70: bl       #0x3deed8
003dfe74: tst      r0, #4
003dfe78: bne      #0x3e0110
003dfe7c: tst      r0, #2
003dfe80: bne      #0x3e02bc
003dfe84: tst      r0, #1
003dfe88: beq      #0x3e0080
003dfe8c: add      r2, r6, #0xa90
003dfe90: add      r2, r2, #4
003dfe94: str      r2, [sp, #4]
003dfe98: ldr      r3, [r6, #0xe20]
003dfe9c: add      sb, r6, #0xe10
003dfea0: add      sb, sb, #8
003dfea4: str      r3, [sp, #0xc]
003dfea8: ldr      r2, [sp, #0xc]
003dfeac: add      ip, sp, #0x20
003dfeb0: str      ip, [sp, #8]
003dfeb4: cmp      r2, sb
003dfeb8: add      r4, sp, #0x10
003dfebc: mov      r8, r6
003dfec0: beq      #0x3e0018
003dfec4: ldrb     r3, [sb]
003dfec8: cmp      r3, #0
003dfecc: bne      #0x3dfee4
003dfed0: ldr      r3, [sb, #4]
003dfed4: ldr      r3, [r3, #4]
003dfed8: cmp      r3, sb
003dfedc: ldreq    ip, [sb, #0xc]
003dfee0: beq      #0x3dff04
003dfee4: ldr      ip, [sb, #8]
003dfee8: cmp      ip, #0
003dfeec: bne      #0x3dfef8
003dfef0: b        #0x3e00b4
003dfef4: mov      ip, r3
003dfef8: ldr      r3, [ip, #0xc]
003dfefc: cmp      r3, #0
003dff00: bne      #0x3dfef4
003dff04: ldr      lr, [sp, #8]
003dff08: add      r5, ip, #0x34
003dff0c: ldm      r5, {r0, r1, r2, r3}
003dff10: stm      lr, {r0, r1, r2, r3}
003dff14: add      r0, ip, #0x44
003dff18: ldr      r1, [sp, #8]
003dff1c: bl       #0x3de870
003dff20: subs     sl, r0, #0
003dff24: beq      #0x3dffc8
003dff28: mov      fp, #0
003dff2c: mov      r6, fp
003dff30: ldm      r5, {r0, r1, r2, r3}
003dff34: stm      r4, {r0, r1, r2, r3}
003dff38: mov      r1, r6
003dff3c: mov      r0, r4
003dff40: bl       #0x3de8b4
003dff44: ldm      r5, {r0, r1, r2, r3}
003dff48: stm      r4, {r0, r1, r2, r3}
003dff4c: mov      r1, r6
003dff50: mov      r0, r4
003dff54: bl       #0x3de8b4
003dff58: ldr      r3, [sp, #0x10]
003dff5c: mov      r2, r7
003dff60: mov      r0, r8
003dff64: ldr      r1, [r3]
003dff68: bl       #0x3df114
003dff6c: cmp      r0, #0
003dff70: beq      #0x3dffb4
003dff74: ldm      r5, {r0, r1, r2, r3}
003dff78: stm      r4, {r0, r1, r2, r3}
003dff7c: mov      r1, r6
003dff80: mov      r0, r4
003dff84: bl       #0x3de8b4
003dff88: ldr      r3, [sp, #0x10]
003dff8c: mov      r2, r7
003dff90: mov      r0, r8
003dff94: ldr      r1, [r3]
003dff98: bl       #0x3dedb4
003dff9c: ldr      r1, [sp, #4]
003dffa0: mov      r3, r0
003dffa4: mov      r2, r7
003dffa8: mov      r0, r8
003dffac: bl       #0x3deca0
003dffb0: mov      fp, #1
003dffb4: add      r6, r6, #1
003dffb8: cmp      r6, sl
003dffbc: bne      #0x3dff30
003dffc0: cmp      fp, #0
003dffc4: bne      #0x3e0474
003dffc8: ldrb     r3, [sb]
003dffcc: cmp      r3, #0
003dffd0: bne      #0x3dffe8
003dffd4: ldr      r3, [sb, #4]
003dffd8: ldr      r3, [r3, #4]
003dffdc: cmp      r3, sb
003dffe0: ldreq    r3, [sb, #0xc]
003dffe4: beq      #0x3e0008
003dffe8: ldr      r3, [sb, #8]
003dffec: cmp      r3, #0
003dfff0: bne      #0x3dfffc
003dfff4: b        #0x3e00e4
003dfff8: mov      r3, r2
003dfffc: ldr      r2, [r3, #0xc]
003e0000: cmp      r2, #0
003e0004: bne      #0x3dfff8
003e0008: mov      sb, r3
003e000c: ldr      r2, [sp, #0xc]
003e0010: cmp      r2, sb
003e0014: bne      #0x3dfec4
003e0018: add      r4, r8, #0x710
003e001c: mov      r0, r8
003e0020: mov      r1, r4
003e0024: mov      r2, r7
003e0028: bl       #0x3df114
003e002c: cmp      r0, #0
003e0030: mov      r6, r8
003e0034: bne      #0x3e047c
003e0038: add      r4, r6, #0x38c
003e003c: mov      r0, r6
003e0040: mov      r1, r4
003e0044: mov      r2, r7
003e0048: bl       #0x3df114
003e004c: cmp      r0, #0
003e0050: bne      #0x3e047c
003e0054: add      r4, r6, #8
003e0058: mov      r0, r6
003e005c: mov      r1, r4
003e0060: mov      r2, r7
003e0064: bl       #0x3df114
003e0068: cmp      r0, #0
003e006c: bne      #0x3e047c
003e0070: mov      r0, r6
003e0074: mov      r1, r7
003e0078: bl       #0x3def10
003e007c: b        #0x3e048c
003e0080: tst      r0, #0x20
003e0084: bne      #0x3e0550
003e0088: tst      r0, #0x10
003e008c: addeq    ip, r6, #0xa90
003e0090: addeq    ip, ip, #4
003e0094: streq    ip, [sp, #4]
003e0098: bne      #0x3e0438
003e009c: mov      r0, r6
003e00a0: ldr      r1, [sp, #4]
003e00a4: mov      r2, r7
003e00a8: bl       #0x3dedb4
003e00ac: add      sp, sp, #0x54
003e00b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e00b4: ldr      ip, [sb, #4]
003e00b8: ldr      r3, [ip, #8]
003e00bc: cmp      r3, sb
003e00c0: beq      #0x3e00cc
003e00c4: b        #0x3dff04
003e00c8: mov      ip, r3
003e00cc: ldr      r3, [ip, #4]
003e00d0: ldr      r2, [r3, #8]
003e00d4: cmp      r2, ip
003e00d8: beq      #0x3e00c8
003e00dc: mov      ip, r3
003e00e0: b        #0x3dff04
003e00e4: ldr      r3, [sb, #4]
003e00e8: ldr      r2, [r3, #8]
003e00ec: cmp      sb, r2
003e00f0: bne      #0x3e0008
003e00f4: mov      r2, r3
003e00f8: ldr      r3, [r3, #4]
003e00fc: ldr      r1, [r3, #8]
003e0100: cmp      r1, r2
003e0104: beq      #0x3e00f4
003e0108: mov      sb, r3
003e010c: b        #0x3e000c
003e0110: add      r2, r6, #0xa90
003e0114: add      r2, r2, #4
003e0118: mov      r1, r7
003e011c: mov      r0, r6
003e0120: str      r2, [sp, #4]
003e0124: bl       #0x3def10
003e0128: add      r4, r6, #8
003e012c: mov      r3, r0
003e0130: ldr      r1, [sp, #4]
003e0134: mov      r0, r6
003e0138: mov      r2, r7
003e013c: bl       #0x3deca0
003e0140: mov      r0, r6
003e0144: mov      r1, r4
003e0148: mov      r2, r7
003e014c: bl       #0x3df114
003e0150: cmp      r0, #0
003e0154: bne      #0x3e0528
003e0158: add      r4, r6, #0x38c
003e015c: mov      r0, r6
003e0160: mov      r1, r4
003e0164: mov      r2, r7
003e0168: bl       #0x3df114
003e016c: cmp      r0, #0
003e0170: bne      #0x3e0500
003e0174: add      r4, r6, #0x710
003e0178: mov      r0, r6
003e017c: mov      r1, r4
003e0180: mov      r2, r7
003e0184: bl       #0x3df114
003e0188: cmp      r0, #0
003e018c: bne      #0x3e04d8
003e0190: add      r3, r6, #0xe10
003e0194: add      r3, r3, #8
003e0198: str      r3, [sp, #8]
003e019c: ldr      sb, [r6, #0xe20]
003e01a0: add      fp, sp, #0x40
003e01a4: add      r4, sp, #0x10
003e01a8: ldr      ip, [sp, #8]
003e01ac: cmp      sb, ip
003e01b0: beq      #0x3e009c
003e01b4: add      r8, sb, #0x34
003e01b8: ldm      r8, {r0, r1, r2, r3}
003e01bc: stm      fp, {r0, r1, r2, r3}
003e01c0: add      r0, sb, #0x44
003e01c4: mov      r1, fp
003e01c8: bl       #0x3de870
003e01cc: subs     sl, r0, #0
003e01d0: beq      #0x3e0260
003e01d4: mov      r5, #0
003e01d8: b        #0x3e01e8
003e01dc: add      r5, r5, #1
003e01e0: cmp      r5, sl
003e01e4: beq      #0x3e0260
003e01e8: ldm      r8, {r0, r1, r2, r3}
003e01ec: stm      r4, {r0, r1, r2, r3}
003e01f0: mov      r1, r5
003e01f4: mov      r0, r4
003e01f8: bl       #0x3de8b4
003e01fc: ldr      r3, [sp, #0x10]
003e0200: mov      r2, r7
003e0204: mov      r0, r6
003e0208: ldr      r1, [r3]
003e020c: bl       #0x3df114
003e0210: cmp      r0, #0
003e0214: beq      #0x3e01dc
003e0218: ldm      r8, {r0, r1, r2, r3}
003e021c: stm      r4, {r0, r1, r2, r3}
003e0220: mov      r1, r5
003e0224: mov      r0, r4
003e0228: bl       #0x3de8b4
003e022c: ldr      r3, [sp, #0x10]
003e0230: mov      r2, r7
003e0234: mov      r0, r6
003e0238: ldr      r1, [r3]
003e023c: bl       #0x3dedb4
003e0240: add      r5, r5, #1
003e0244: mov      r3, r0
003e0248: ldr      r1, [sp, #4]
003e024c: mov      r0, r6
003e0250: mov      r2, r7
003e0254: bl       #0x3df140
003e0258: cmp      r5, sl
003e025c: bne      #0x3e01e8
003e0260: ldr      r2, [sb, #0xc]
003e0264: cmp      r2, #0
003e0268: bne      #0x3e0274
003e026c: b        #0x3e0288
003e0270: mov      r2, r3
003e0274: ldr      r3, [r2, #8]
003e0278: cmp      r3, #0
003e027c: bne      #0x3e0270
003e0280: mov      sb, r2
003e0284: b        #0x3e01a8
003e0288: ldr      r3, [sb, #4]
003e028c: ldr      r1, [r3, #0xc]
003e0290: cmp      sb, r1
003e0294: bne      #0x3e02b0
003e0298: mov      sb, r3
003e029c: ldr      r3, [r3, #4]
003e02a0: ldr      r2, [r3, #0xc]
003e02a4: cmp      sb, r2
003e02a8: beq      #0x3e0298
003e02ac: ldr      r2, [sb, #0xc]
003e02b0: cmp      r3, r2
003e02b4: movne    sb, r3
003e02b8: b        #0x3e01a8
003e02bc: add      r4, r6, #8
003e02c0: mov      r1, r4
003e02c4: mov      r0, r6
003e02c8: mov      r2, r7
003e02cc: bl       #0x3df114
003e02d0: cmp      r0, #0
003e02d4: movne    r1, r4
003e02d8: bne      #0x3e043c
003e02dc: add      r4, r6, #0x38c
003e02e0: mov      r0, r6
003e02e4: mov      r1, r4
003e02e8: mov      r2, r7
003e02ec: bl       #0x3df114
003e02f0: cmp      r0, #0
003e02f4: bne      #0x3e05a8
003e02f8: add      r4, r6, #0x710
003e02fc: mov      r0, r6
003e0300: mov      r1, r4
003e0304: mov      r2, r7
003e0308: bl       #0x3df114
003e030c: cmp      r0, #0
003e0310: bne      #0x3e05f8
003e0314: add      lr, r6, #0xe10
003e0318: add      r2, r6, #0xa90
003e031c: add      lr, lr, #8
003e0320: add      r2, r2, #4
003e0324: str      lr, [sp, #0xc]
003e0328: str      r2, [sp, #4]
003e032c: add      r3, sp, #0x30
003e0330: ldr      sl, [r6, #0xe20]
003e0334: add      r4, sp, #0x10
003e0338: str      r3, [sp, #8]
003e033c: mov      r8, r6
003e0340: ldr      lr, [sp, #0xc]
003e0344: cmp      lr, sl
003e0348: beq      #0x3e05e4
003e034c: ldr      ip, [sp, #8]
003e0350: add      r5, sl, #0x34
003e0354: ldm      r5, {r0, r1, r2, r3}
003e0358: stm      ip, {r0, r1, r2, r3}
003e035c: add      r0, sl, #0x44
003e0360: ldr      r1, [sp, #8]
003e0364: bl       #0x3de870
003e0368: subs     sb, r0, #0
003e036c: beq      #0x3e0410
003e0370: mov      r6, #0
003e0374: mov      fp, r6
003e0378: ldm      r5, {r0, r1, r2, r3}
003e037c: stm      r4, {r0, r1, r2, r3}
003e0380: mov      r1, r6
003e0384: mov      r0, r4
003e0388: bl       #0x3de8b4
003e038c: ldm      r5, {r0, r1, r2, r3}
003e0390: stm      r4, {r0, r1, r2, r3}
003e0394: mov      r1, r6
003e0398: mov      r0, r4
003e039c: bl       #0x3de8b4
003e03a0: ldr      r3, [sp, #0x10]
003e03a4: mov      r2, r7
003e03a8: mov      r0, r8
003e03ac: ldr      r1, [r3]
003e03b0: bl       #0x3df114
003e03b4: cmp      r0, #0
003e03b8: beq      #0x3e03fc
003e03bc: ldm      r5, {r0, r1, r2, r3}
003e03c0: stm      r4, {r0, r1, r2, r3}
003e03c4: mov      r1, r6
003e03c8: mov      r0, r4
003e03cc: bl       #0x3de8b4
003e03d0: ldr      r3, [sp, #0x10]
003e03d4: mov      r2, r7
003e03d8: mov      r0, r8
003e03dc: ldr      r1, [r3]
003e03e0: bl       #0x3dedb4
003e03e4: ldr      r1, [sp, #4]
003e03e8: mov      r3, r0
003e03ec: mov      r2, r7
003e03f0: mov      r0, r8
003e03f4: bl       #0x3deca0
003e03f8: mov      fp, #1
003e03fc: add      r6, r6, #1
003e0400: cmp      r6, sb
003e0404: bne      #0x3e0378
003e0408: cmp      fp, #0
003e040c: bne      #0x3e0474
003e0410: ldr      r2, [sl, #0xc]
003e0414: cmp      r2, #0
003e0418: beq      #0x3e04a4
003e041c: mov      sl, r2
003e0420: b        #0x3e0428
003e0424: mov      sl, r3
003e0428: ldr      r3, [sl, #8]
003e042c: cmp      r3, #0
003e0430: bne      #0x3e0424
003e0434: b        #0x3e0340
003e0438: add      r1, r6, #8
003e043c: mov      r2, r7
003e0440: add      lr, r6, #0xa90
003e0444: mov      r0, r6
003e0448: str      lr, [sp, #4]
003e044c: bl       #0x3dedb4
003e0450: ldr      r2, [sp, #4]
003e0454: mov      r3, r0
003e0458: add      r2, r2, #4
003e045c: str      r2, [sp, #4]
003e0460: mov      r1, r2
003e0464: mov      r0, r6
003e0468: mov      r2, r7
003e046c: bl       #0x3deca0
003e0470: b        #0x3e009c
003e0474: mov      r6, r8
003e0478: b        #0x3e009c
003e047c: mov      r1, r4
003e0480: mov      r0, r6
003e0484: mov      r2, r7
003e0488: bl       #0x3dedb4
003e048c: mov      r3, r0
003e0490: ldr      r1, [sp, #4]
003e0494: mov      r0, r6
003e0498: mov      r2, r7
003e049c: bl       #0x3deca0
003e04a0: b        #0x3e009c
003e04a4: ldr      r3, [sl, #4]
003e04a8: ldr      r1, [r3, #0xc]
003e04ac: cmp      sl, r1
003e04b0: bne      #0x3e04cc
003e04b4: mov      sl, r3
003e04b8: ldr      r3, [r3, #4]
003e04bc: ldr      r2, [r3, #0xc]
003e04c0: cmp      r2, sl
003e04c4: beq      #0x3e04b4
003e04c8: ldr      r2, [sl, #0xc]
003e04cc: cmp      r3, r2
003e04d0: movne    sl, r3
003e04d4: b        #0x3e0340
003e04d8: mov      r1, r4
003e04dc: mov      r2, r7
003e04e0: mov      r0, r6
003e04e4: bl       #0x3dedb4
003e04e8: ldr      r1, [sp, #4]
003e04ec: mov      r3, r0
003e04f0: mov      r2, r7
003e04f4: mov      r0, r6
003e04f8: bl       #0x3df140
003e04fc: b        #0x3e0190
003e0500: mov      r1, r4
003e0504: mov      r2, r7
003e0508: mov      r0, r6
003e050c: bl       #0x3dedb4
003e0510: ldr      r1, [sp, #4]
003e0514: mov      r3, r0
003e0518: mov      r2, r7
003e051c: mov      r0, r6
003e0520: bl       #0x3df140
003e0524: b        #0x3e0174
003e0528: mov      r1, r4
003e052c: mov      r2, r7
003e0530: mov      r0, r6
003e0534: bl       #0x3dedb4
003e0538: ldr      r1, [sp, #4]
003e053c: mov      r3, r0
003e0540: mov      r2, r7
003e0544: mov      r0, r6
003e0548: bl       #0x3df140
003e054c: b        #0x3e0158
003e0550: add      r3, r6, #0xa90
003e0554: add      r3, r3, #4
003e0558: add      r1, r6, #8
003e055c: mov      r2, r7
003e0560: mov      r0, r6
003e0564: str      r3, [sp, #4]
003e0568: bl       #0x3dedb4
003e056c: ldr      r1, [sp, #4]
003e0570: mov      r3, r0
003e0574: mov      r2, r7
003e0578: mov      r0, r6
003e057c: bl       #0x3deca0
003e0580: add      r1, r6, #0x38c
003e0584: mov      r2, r7
003e0588: mov      r0, r6
003e058c: bl       #0x3dedb4
003e0590: ldr      r1, [sp, #4]
003e0594: mov      r3, r0
003e0598: mov      r2, r7
003e059c: mov      r0, r6
003e05a0: bl       #0x3df140
003e05a4: b        #0x3e009c
003e05a8: add      r3, r6, #0xa90
003e05ac: mov      r1, r4
003e05b0: mov      r2, r7
003e05b4: mov      r0, r6
003e05b8: str      r3, [sp, #4]
003e05bc: bl       #0x3dedb4
003e05c0: ldr      ip, [sp, #4]
003e05c4: mov      r3, r0
003e05c8: mov      r2, r7
003e05cc: add      ip, ip, #4
003e05d0: mov      r0, r6
003e05d4: mov      r1, ip
003e05d8: str      ip, [sp, #4]
003e05dc: bl       #0x3deca0
003e05e0: b        #0x3e009c
003e05e4: mov      r0, r8
003e05e8: mov      r1, r7
003e05ec: mov      r6, r8
003e05f0: bl       #0x3def10
003e05f4: b        #0x3e048c
003e05f8: mov      r2, r7
003e05fc: mov      r1, r4
003e0600: mov      r0, r6
003e0604: bl       #0x3dedb4
003e0608: add      r2, r6, #0xa90
003e060c: mov      r3, r0
003e0610: b        #0x3e0458
