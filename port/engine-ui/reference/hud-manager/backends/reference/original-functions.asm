
# _ZNK6CharAI16AI_IsSkillUsableEj
003d8358: push     {r4, r5, r6, r7, lr}
003d835c: mov      r4, r0
003d8360: ldr      r0, [r0, #4]
003d8364: sub      sp, sp, #0xc
003d8368: mov      r5, r1
003d836c: add      r0, r0, #0x4f0
003d8370: add      r0, r0, #0xc
003d8374: bl       #0x3c02e8
003d8378: ldr      r6, [pc, #0xdc]
003d837c: cmp      r0, #0
003d8380: ldreq    r0, [r4, #4]
003d8384: add      r6, pc, r6
003d8388: beq      #0x3d83a8
003d838c: ldr      r0, [r4, #4]
003d8390: ldr      r3, [r0, #0x520]
003d8394: tst      r3, #0x8000
003d8398: bne      #0x3d83a8
003d839c: mov      r0, #0
003d83a0: add      sp, sp, #0xc
003d83a4: pop      {r4, r5, r6, r7, pc}
003d83a8: add      r0, r0, #0x4f0
003d83ac: add      r0, r0, #0xc
003d83b0: bl       #0x3c0334
003d83b4: subs     r7, r0, #0
003d83b8: bne      #0x3d839c
003d83bc: mov      r0, r4
003d83c0: bl       #0x3cb458
003d83c4: cmp      r0, #0
003d83c8: beq      #0x3d839c
003d83cc: ldr      r3, [r4, #0xb4]
003d83d0: ldr      r2, [r4, #0xb8]
003d83d4: rsb      r2, r3, r2
003d83d8: cmp      r5, r2, asr #2
003d83dc: blo      #0x3d8444
003d83e0: ldr      r3, [pc, #0x78]
003d83e4: ldr      r3, [r6, r3]
003d83e8: ldr      r3, [r3]
003d83ec: cmp      r3, #2
003d83f0: streq    r7, [r7]
003d83f4: beq      #0x3d839c
003d83f8: cmp      r3, #1
003d83fc: bne      #0x3d839c
003d8400: ldr      r0, [pc, #0x5c]
003d8404: ldr      r1, [pc, #0x5c]
003d8408: ldr      r2, [pc, #0x5c]
003d840c: ldr      r0, [r6, r0]
003d8410: ldr      r3, [pc, #0x58]
003d8414: add      r2, pc, r2
003d8418: mov      ip, #0xb5
003d841c: add      r3, pc, r3
003d8420: add      r1, pc, r1
003d8424: add      r0, r0, #0xa8
003d8428: str      ip, [sp]
003d842c: bl       #0x30e004
003d8430: ldr      r2, [r4, #0xb8]
003d8434: ldr      r3, [r4, #0xb4]
003d8438: rsb      r2, r3, r2
003d843c: cmp      r5, r2, asr #2
003d8440: bhs      #0x3d839c
003d8444: ldr      r0, [r3, r5, lsl #2]
003d8448: cmp      r0, #0
003d844c: beq      #0x3d839c
003d8450: add      sp, sp, #0xc
003d8454: pop      {r4, r5, r6, r7, lr}
003d8458: b        #0x3da9dc
003d845c: subseq   ip, fp, ip, lsl #14
003d8460: andeq    r3, r0, r0, asr #19
003d8464: andeq    r1, r0, r0, asr #19
003d8468: strheq   r5, [lr], #-0xf8
003d846c: subeq    sp, lr, ip, ror #6
003d8470: subeq    sp, lr, ip, lsl #6

# _ZNK6CharAI16AI_IsSpellUsableEv
003d80b4: push     {r4, r5, r6, r7, lr}
003d80b8: mov      r4, r0
003d80bc: ldr      r0, [r0, #4]
003d80c0: sub      sp, sp, #0xc
003d80c4: ldr      r6, [pc, #0xdc]
003d80c8: add      r0, r0, #0x4f0
003d80cc: add      r0, r0, #0xc
003d80d0: bl       #0x3c02e8
003d80d4: cmp      r0, #0
003d80d8: add      r6, pc, r6
003d80dc: beq      #0x3d80ec
003d80e0: mov      r0, #0
003d80e4: add      sp, sp, #0xc
003d80e8: pop      {r4, r5, r6, r7, pc}
003d80ec: ldr      r0, [r4, #4]
003d80f0: add      r0, r0, #0x4f0
003d80f4: add      r0, r0, #0xc
003d80f8: bl       #0x3c0334
003d80fc: subs     r7, r0, #0
003d8100: bne      #0x3d80e0
003d8104: mov      r0, r4
003d8108: bl       #0x3cb458
003d810c: cmp      r0, #0
003d8110: beq      #0x3d80e0
003d8114: ldr      r0, [r4, #4]
003d8118: mvn      r1, #0
003d811c: bl       #0x3bb98c
003d8120: ldr      r2, [r4, #0xc0]
003d8124: ldr      r3, [r4, #0xc4]
003d8128: mov      r5, r0
003d812c: rsb      r3, r2, r3
003d8130: cmp      r0, r3, asr #2
003d8134: blt      #0x3d8158
003d8138: ldr      r3, [pc, #0x6c]
003d813c: ldr      r3, [r6, r3]
003d8140: ldr      r3, [r3]
003d8144: cmp      r3, #2
003d8148: streq    r7, [r7]
003d814c: beq      #0x3d8158
003d8150: cmp      r3, #1
003d8154: beq      #0x3d8170
003d8158: ldr      r0, [r2, r5, lsl #2]
003d815c: cmp      r0, #0
003d8160: beq      #0x3d80e0
003d8164: add      sp, sp, #0xc
003d8168: pop      {r4, r5, r6, r7, lr}
003d816c: b        #0x3da9dc
003d8170: ldr      r0, [pc, #0x38]
003d8174: ldr      r1, [pc, #0x38]
003d8178: ldr      r2, [pc, #0x38]
003d817c: ldr      r0, [r6, r0]
003d8180: ldr      r3, [pc, #0x34]
003d8184: add      r2, pc, r2
003d8188: movw     ip, #0x141
003d818c: add      r1, pc, r1
003d8190: add      r0, r0, #0xa8
003d8194: add      r3, pc, r3
003d8198: str      ip, [sp]
003d819c: bl       #0x30e004
003d81a0: ldr      r2, [r4, #0xc0]
003d81a4: b        #0x3d8158
003d81a8: ldrheq   ip, [fp], #-0x98
003d81ac: andeq    r3, r0, r0, asr #19
003d81b0: andeq    r1, r0, r0, asr #19
003d81b4: subeq    r6, lr, ip, asr #4
003d81b8: subeq    sp, lr, ip, lsl r6
003d81bc: umaaleq  sp, lr, r4, r5

# _ZNK6CharAI12AI_SkillInfoEjjRf
003d7e88: push     {r4, r5, r6, r7, lr}
003d7e8c: mov      r4, r0
003d7e90: ldr      r6, [r4, #0xb8]
003d7e94: ldr      r0, [r0, #0xb4]
003d7e98: ldr      ip, [pc, #0xa8]
003d7e9c: sub      sp, sp, #0xc
003d7ea0: rsb      r6, r0, r6
003d7ea4: cmp      r1, r6, asr #2
003d7ea8: add      ip, pc, ip
003d7eac: mov      r5, r1
003d7eb0: mov      r7, r2
003d7eb4: mov      r6, r3
003d7eb8: blo      #0x3d7ee0
003d7ebc: ldr      r3, [pc, #0x88]
003d7ec0: ldr      r3, [ip, r3]
003d7ec4: ldr      r3, [r3]
003d7ec8: cmp      r3, #2
003d7ecc: moveq    r3, #0
003d7ed0: streq    r3, [r3]
003d7ed4: beq      #0x3d7ee0
003d7ed8: cmp      r3, #1
003d7edc: beq      #0x3d7f10
003d7ee0: ldr      r0, [r0, r5, lsl #2]
003d7ee4: cmp      r0, #0
003d7ee8: beq      #0x3d7f00
003d7eec: mov      r1, r7
003d7ef0: mov      r2, r6
003d7ef4: add      sp, sp, #0xc
003d7ef8: pop      {r4, r5, r6, r7, lr}
003d7efc: b        #0x3daca8
003d7f00: mov      r3, #0
003d7f04: str      r3, [r6]
003d7f08: add      sp, sp, #0xc
003d7f0c: pop      {r4, r5, r6, r7, pc}
003d7f10: ldr      r0, [pc, #0x38]
003d7f14: ldr      r1, [pc, #0x38]
003d7f18: ldr      r2, [pc, #0x38]
003d7f1c: ldr      r0, [ip, r0]
003d7f20: ldr      r3, [pc, #0x34]
003d7f24: movw     ip, #0x1b7
003d7f28: add      r1, pc, r1
003d7f2c: add      r0, r0, #0xa8
003d7f30: add      r2, pc, r2
003d7f34: add      r3, pc, r3
003d7f38: str      ip, [sp]
003d7f3c: bl       #0x30e004
003d7f40: ldr      r0, [r4, #0xb4]
003d7f44: b        #0x3d7ee0
003d7f48: subseq   ip, fp, r8, ror #23
003d7f4c: andeq    r3, r0, r0, asr #19
003d7f50: andeq    r1, r0, r0, asr #19
003d7f54: strheq   r6, [lr], #-0x40
003d7f58: subeq    sp, lr, r0, asr r8
003d7f5c: strdeq   sp, lr, [lr], #-0x74

# _ZNK14PlayerSavegame16SG_GetSkillLevelEj
004668dc: push     {r4, r5, r6, lr}
004668e0: ldr      r3, [r0, #0x84]
004668e4: ldr      r4, [pc, #0xf8]
004668e8: sub      sp, sp, #8
004668ec: cmp      r3, r1
004668f0: mov      r5, r0
004668f4: mov      r6, r1
004668f8: add      r4, pc, r4
004668fc: bhi      #0x466924
00466900: ldr      r3, [pc, #0xe0]
00466904: ldr      r3, [r4, r3]
00466908: ldr      r3, [r3]
0046690c: cmp      r3, #2
00466910: moveq    r3, #0
00466914: streq    r3, [r3]
00466918: beq      #0x466924
0046691c: cmp      r3, #1
00466920: beq      #0x46696c
00466924: ldr      r3, [r5, #0x80]
00466928: cmp      r3, #0
0046692c: beq      #0x466940
00466930: add      r6, r3, r6, lsl #3
00466934: ldrh     r0, [r6, #4]
00466938: add      sp, sp, #8
0046693c: pop      {r4, r5, r6, pc}
00466940: ldr      r2, [pc, #0xa0]
00466944: ldr      r2, [r4, r2]
00466948: ldr      r2, [r2]
0046694c: cmp      r2, #2
00466950: streq    r3, [r3]
00466954: mvneq    r0, #0
00466958: beq      #0x466938
0046695c: cmp      r2, #1
00466960: beq      #0x4669a0
00466964: mvn      r0, #0
00466968: b        #0x466938
0046696c: ldr      r0, [pc, #0x78]
00466970: ldr      r1, [pc, #0x78]
00466974: ldr      r2, [pc, #0x78]
00466978: ldr      r0, [r4, r0]
0046697c: ldr      r3, [pc, #0x74]
00466980: mov      ip, #0xa5
00466984: add      r1, pc, r1
00466988: add      r2, pc, r2
0046698c: add      r3, pc, r3
00466990: add      r0, r0, #0xa8
00466994: str      ip, [sp]
00466998: bl       #0x30e004
0046699c: b        #0x466924
004669a0: ldr      r0, [pc, #0x44]
004669a4: ldr      r1, [pc, #0x50]
004669a8: ldr      r2, [pc, #0x50]
004669ac: ldr      r0, [r4, r0]
004669b0: ldr      r3, [pc, #0x4c]
004669b4: mov      ip, #0xa6
004669b8: add      r1, pc, r1
004669bc: add      r3, pc, r3
004669c0: add      r0, r0, #0xa8
004669c4: add      r2, pc, r2
004669c8: str      ip, [sp]
004669cc: bl       #0x30e004
004669d0: ldr      r3, [r5, #0x80]
004669d4: cmp      r3, #0
004669d8: bne      #0x466930
004669dc: mvn      r0, #0
004669e0: b        #0x466938

# _ZN17CharAISkillScript11GetCooldownEv
003da3d0: push     {r4, lr}
003da3d4: ldr      r1, [r0, #0x18]
003da3d8: sub      sp, sp, #8
003da3dc: cmn      r1, #1
003da3e0: beq      #0x3da430
003da3e4: ldr      r0, [r0, #4]
003da3e8: add      r2, sp, #4
003da3ec: mov      r3, sp
003da3f0: add      r0, r0, #0x3b4
003da3f4: bl       #0x3db344
003da3f8: cmp      r0, #0
003da3fc: beq      #0x3da430
003da400: ldr      r0, [sp, #4]
003da404: bl       #0x30e2e0
003da408: mov      r4, r0
003da40c: ldr      r0, [sp]
003da410: bl       #0x30e2e0
003da414: mov      r1, r0
003da418: mov      r0, r4
003da41c: bl       #0x30ec94
003da420: mov      r1, r0
003da424: mov      r0, #0x3f800000
003da428: bl       #0x30e3ac
003da42c: b        #0x3da434
003da430: mov      r0, #0
003da434: add      sp, sp, #8
003da438: pop      {r4, pc}

# _ZNK14PlayerSavegame17SG_GetSkillInSlotEi
00467488: push     {r4, r5, r6, lr}
0046748c: mov      r6, r0
00467490: ldr      r0, [r0, #0x10]
00467494: mov      r5, r1
00467498: mvn      r1, #0
0046749c: add      r0, r0, #0x37c
004674a0: bl       #0x3fc6a0
004674a4: ldr      r3, [r6, #0x88]
004674a8: mov      r2, #0x18
004674ac: mla      r3, r2, r0, r3
004674b0: ldr      r4, [r3, #4]
004674b4: cmp      r4, #0
004674b8: beq      #0x4674fc
004674bc: mov      ip, r3
004674c0: b        #0x4674c8
004674c4: mov      r4, r1
004674c8: ldr      r1, [r4, #0x10]
004674cc: cmp      r5, r1
004674d0: ldrgt    r1, [r4, #0xc]
004674d4: ldrle    r1, [r4, #8]
004674d8: movgt    r4, ip
004674dc: mov      ip, r4
004674e0: cmp      r1, #0
004674e4: bne      #0x4674c4
004674e8: cmp      r3, r4
004674ec: beq      #0x467500
004674f0: ldr      r2, [r4, #0x10]
004674f4: cmp      r5, r2
004674f8: bge      #0x467500
004674fc: mov      r4, r3
00467500: ldr      r0, [r6, #0x10]
00467504: mvn      r1, #0
00467508: add      r0, r0, #0x37c
0046750c: bl       #0x3fc6a0
00467510: ldr      r3, [r6, #0x88]
00467514: mov      r2, #0x18
00467518: mla      r3, r2, r0, r3
0046751c: cmp      r4, r3
00467520: mvneq    r0, #0
00467524: ldrne    r0, [r4, #0x14]
00467528: pop      {r4, r5, r6, pc}

# _ZNK10CharTimers12TMR_TimeLeftEjRjS0_
003db344: ldr      ip, [r0, #0xc]
003db348: ldr      r0, [r0, #8]
003db34c: rsb      ip, r0, ip
003db350: cmp      r1, ip, asr #5
003db354: bhs      #0x3db380
003db358: add      r1, r0, r1, lsl #5
003db35c: ldrb     r0, [r1, #0x14]
003db360: cmp      r0, #0
003db364: beq      #0x3db380
003db368: ldr      ip, [r1, #0x10]
003db36c: mov      r0, #1
003db370: str      ip, [r2]
003db374: ldr      r2, [r1, #0xc]
003db378: str      r2, [r3]
003db37c: bx       lr
003db380: mov      r0, #0
003db384: bx       lr

# _ZNK6CharAI12AI_SpellInfoERf
003d7da8: push     {r4, r5, r6, lr}
003d7dac: mov      r4, r0
003d7db0: sub      sp, sp, #8
003d7db4: mov      r6, r1
003d7db8: ldr      r0, [r0, #4]
003d7dbc: mvn      r1, #0
003d7dc0: bl       #0x3bb98c
003d7dc4: ldr      r2, [r4, #0xc0]
003d7dc8: ldr      r1, [r4, #0xc4]
003d7dcc: ldr      r3, [pc, #0x9c]
003d7dd0: mov      r5, r0
003d7dd4: rsb      r1, r2, r1
003d7dd8: cmp      r0, r1, asr #2
003d7ddc: add      r3, pc, r3
003d7de0: blo      #0x3d7e08
003d7de4: ldr      r1, [pc, #0x88]
003d7de8: ldr      r1, [r3, r1]
003d7dec: ldr      r1, [r1]
003d7df0: cmp      r1, #2
003d7df4: moveq    r3, #0
003d7df8: streq    r3, [r3]
003d7dfc: beq      #0x3d7e08
003d7e00: cmp      r1, #1
003d7e04: beq      #0x3d7e38
003d7e08: ldr      r0, [r2, r5, lsl #2]
003d7e0c: cmp      r0, #0
003d7e10: beq      #0x3d7e28
003d7e14: mov      r2, r6
003d7e18: mov      r1, #0
003d7e1c: add      sp, sp, #8
003d7e20: pop      {r4, r5, r6, lr}
003d7e24: b        #0x3daca8
003d7e28: mov      r3, #0
003d7e2c: str      r3, [r6]
003d7e30: add      sp, sp, #8
003d7e34: pop      {r4, r5, r6, pc}
003d7e38: ldr      r0, [pc, #0x38]
003d7e3c: ldr      r1, [pc, #0x38]
003d7e40: ldr      r2, [pc, #0x38]
003d7e44: ldr      r0, [r3, r0]
003d7e48: ldr      r3, [pc, #0x34]
003d7e4c: add      r2, pc, r2
003d7e50: movw     ip, #0x1c9
003d7e54: add      r1, pc, r1
003d7e58: add      r0, r0, #0xa8
003d7e5c: add      r3, pc, r3
003d7e60: str      ip, [sp]
003d7e64: bl       #0x30e004
003d7e68: ldr      r2, [r4, #0xc0]
003d7e6c: b        #0x3d7e08
003d7e70: ldrheq   ip, [fp], #-0xc4
003d7e74: andeq    r3, r0, r0, asr #19
003d7e78: andeq    r1, r0, r0, asr #19
003d7e7c: subeq    r6, lr, r4, lsl #11
003d7e80: strheq   sp, [lr], #-0x8c
003d7e84: subeq    sp, lr, ip, asr #17

# _ZNK13ItemInventory13GetNumPotionsEv
003fc690: ldr      r0, [r0, #0x24]
003fc694: cmp      r0, #0
003fc698: ldrshne  r0, [r0, #0x50]
003fc69c: bx       lr
