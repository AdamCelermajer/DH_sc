
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

# _ZNK14CharProperties12PROPS_GetIntEib
003df6e0: ldr      r3, [pc, #0x28]
003df6e4: cmp      r2, #0
003df6e8: mov      r2, r1
003df6ec: addeq    r1, r0, #0xa90
003df6f0: add      r3, pc, r3
003df6f4: push     {r4, lr}
003df6f8: addeq    r1, r1, #4
003df6fc: ldrne    r1, [pc, #0x10]
003df700: ldrne    r1, [r3, r1]
003df704: bl       #0x3dedb4
003df708: asr      r0, r0, #8
003df70c: pop      {r4, pc}
003df710: subseq   r5, fp, r0, lsr #7
003df714: andeq    r1, r0, ip, asr #32

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

# _ZN14CharProperties16RecalcPropertiesEb
003e0810: cmp      r1, #0
003e0814: push     {r4, r5, r6, lr}
003e0818: mov      r4, r0
003e081c: bne      #0x3e0840
003e0820: mov      r5, #0
003e0824: mov      r1, r5
003e0828: mov      r0, r4
003e082c: add      r5, r5, #1
003e0830: bl       #0x3dfe60
003e0834: cmp      r5, #0xe0
003e0838: bne      #0x3e0824
003e083c: pop      {r4, r5, r6, pc}
003e0840: add      r1, r0, #8
003e0844: ldr      r2, [r0, #0x74]
003e0848: mov      r3, #0
003e084c: bl       #0x3e2e20
003e0850: b        #0x3e0820
