
# _ZN14CharProperties16PROPS_ResetSheetEPN7Structs19CharacterPropertiesE
003def84: ldr      r3, [pc, #0x18]
003def88: cmp      r1, #0
003def8c: add      r3, pc, r3
003def90: beq      #0x3def98
003def94: b        #0x3def34
003def98: ldr      r2, [pc, #8]
003def9c: ldr      r1, [r3, r2]
003defa0: b        #0x3def94
003defa4: subseq   r5, fp, r4, lsl #22
003defa8: andeq    r1, r0, ip, asr #32

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

# _ZN14CharProperties23PROPS_ApplyClassToSheetEiPN7Structs19CharacterPropertiesE
003df314: str      lr, [sp, #-4]!
003df318: ldr      r3, [pc, #0x80]
003df31c: subs     ip, r2, #0
003df320: sub      sp, sp, #0xc
003df324: mov      r2, r1
003df328: add      r3, pc, r3
003df32c: beq      #0x3df344
003df330: mov      r1, ip
003df334: mov      r3, #1
003df338: add      sp, sp, #0xc
003df33c: pop      {lr}
003df340: b        #0x3e2e20
003df344: ldr      r2, [pc, #0x58]
003df348: ldr      r2, [r3, r2]
003df34c: ldr      r2, [r2]
003df350: cmp      r2, #2
003df354: streq    ip, [ip]
003df358: beq      #0x3df364
003df35c: cmp      r2, #1
003df360: beq      #0x3df36c
003df364: add      sp, sp, #0xc
003df368: ldm      sp!, {pc}
003df36c: ldr      r0, [pc, #0x34]
003df370: ldr      r1, [pc, #0x34]
003df374: ldr      r2, [pc, #0x34]
003df378: ldr      r0, [r3, r0]
003df37c: ldr      r3, [pc, #0x30]
003df380: mov      ip, #0x31c
003df384: add      r1, pc, r1
003df388: add      r2, pc, r2
003df38c: add      r3, pc, r3
003df390: add      r0, r0, #0xa8
003df394: str      ip, [sp]
003df398: bl       #0x30e004
003df39c: b        #0x3df364
003df3a0: subseq   r5, fp, r8, ror #14
003df3a4: andeq    r3, r0, r0, asr #19
003df3a8: andeq    r1, r0, r0, asr #19
003df3ac: subeq    pc, sp, r4, asr r0
003df3b0: subeq    r4, lr, r0, asr #22
003df3b4: subeq    r6, lr, r4, lsr #18

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
