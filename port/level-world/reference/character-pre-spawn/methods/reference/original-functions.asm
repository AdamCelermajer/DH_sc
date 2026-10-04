
# _ZN10CSPreSpawn7OnEventEiP9CharacterP16CharStateMachineiPv
003c2810: push     {r4, r5, r6, lr}
003c2814: sub      sp, sp, #0x10
003c2818: ldr      r6, [sp, #0x20]
003c281c: ldr      r4, [pc, #0x108]
003c2820: mov      r3, r1
003c2824: cmp      r6, #9
003c2828: add      r4, pc, r4
003c282c: mov      r5, r2
003c2830: ldr      r0, [sp, #0x24]
003c2834: beq      #0x3c2874
003c2838: cmp      r6, #0x28
003c283c: beq      #0x3c2848
003c2840: add      sp, sp, #0x10
003c2844: pop      {r4, r5, r6, pc}
003c2848: ldr      r1, [pc, #0xe0]
003c284c: add      r1, pc, r1
003c2850: bl       #0x30e31c
003c2854: cmp      r0, #0
003c2858: bne      #0x3c2840
003c285c: ldr      r3, [r5, #0x520]
003c2860: mov      r0, r5
003c2864: orr      r3, r3, #0x2000
003c2868: str      r3, [r5, #0x520]
003c286c: bl       #0x3b4088
003c2870: b        #0x3c2840
003c2874: add      ip, sp, #0x10
003c2878: mov      r2, #1
003c287c: str      r2, [ip, #-4]!
003c2880: mov      r1, r6
003c2884: mov      r2, r0
003c2888: mov      r0, r5
003c288c: str      ip, [sp]
003c2890: bl       #0x3ad2e4
003c2894: cmp      r0, #0
003c2898: beq      #0x3c2840
003c289c: ldr      r1, [sp, #0xc]
003c28a0: cmp      r1, #1
003c28a4: beq      #0x3c2900
003c28a8: ldr      r3, [pc, #0x84]
003c28ac: ldr      r3, [r4, r3]
003c28b0: ldr      r3, [r3]
003c28b4: cmp      r3, #2
003c28b8: moveq    r3, #0
003c28bc: streq    r3, [r3]
003c28c0: beq      #0x3c2840
003c28c4: cmp      r3, #1
003c28c8: bne      #0x3c2840
003c28cc: ldr      r0, [pc, #0x64]
003c28d0: ldr      r1, [pc, #0x64]
003c28d4: ldr      r2, [pc, #0x64]
003c28d8: ldr      r0, [r4, r0]
003c28dc: ldr      r3, [pc, #0x60]
003c28e0: mov      ip, #0x72
003c28e4: add      r1, pc, r1
003c28e8: add      r2, pc, r2
003c28ec: add      r3, pc, r3
003c28f0: add      r0, r0, #0xa8
003c28f4: str      ip, [sp]
003c28f8: bl       #0x30e004
003c28fc: b        #0x3c2840
003c2900: add      r0, r5, #0x4f0
003c2904: add      r0, r0, #0xc
003c2908: mov      r2, #0
003c290c: bl       #0x3c2734
003c2910: ldr      r3, [r5, #0x400]
003c2914: cmp      r3, #3
003c2918: bne      #0x3c2840
003c291c: ldr      r3, [r5, #0x3fc]
003c2920: mov      r2, #0
003c2924: str      r2, [r3, #0x24]
003c2928: b        #0x3c2840
003c292c: subseq   r2, sp, r8, ror #4
003c2930: subseq   r2, r0, ip, lsl r3
003c2934: andeq    r3, r0, r0, asr #19
003c2938: andeq    r1, r0, r0, asr #19
003c293c: strdeq   fp, ip, [pc], #-0xa4
003c2940: subseq   r2, r0, r8, lsr #9
003c2944: ldrsheq  r2, [r0], #-0x4c

# _ZN10CSPreSpawn6OnBlurEiP9CharacterP16CharStateMachinei
003c67d4: ldr      r3, [pc, #0xa0]
003c67d8: ldr      r1, [pc, #0xa0]
003c67dc: push     {r4, r5, r6, r7, lr}
003c67e0: add      r3, pc, r3
003c67e4: ldr      r6, [r3, r1]
003c67e8: ldr      r1, [pc, #0x94]
003c67ec: mov      r4, r2
003c67f0: ldr      r2, [r6]
003c67f4: ldr      r7, [r3, r1]
003c67f8: sub      sp, sp, #0x24
003c67fc: str      r2, [sp, #0x1c]
003c6800: mov      r0, r7
003c6804: bl       #0x337888
003c6808: ldr      r1, [pc, #0x78]
003c680c: add      r5, sp, #4
003c6810: mov      r2, sp
003c6814: add      r1, pc, r1
003c6818: mov      r0, r5
003c681c: bl       #0x3140ec
003c6820: mov      r1, r5
003c6824: mov      r0, r7
003c6828: bl       #0x337a88
003c682c: mov      r0, r5
003c6830: bl       #0x318254
003c6834: ldr      r3, [r4]
003c6838: mov      r1, #1
003c683c: mov      r0, r4
003c6840: mov      lr, pc
003c6844: ldr      pc, [r3, #0x40]
003c6848: mov      r1, #0
003c684c: mov      r2, r1
003c6850: mov      r0, r4
003c6854: bl       #0x3a59ac
003c6858: mov      r0, r4
003c685c: bl       #0x394a3c
003c6860: ldr      r2, [sp, #0x1c]
003c6864: ldr      r3, [r6]
003c6868: cmp      r2, r3
003c686c: bne      #0x3c6878
003c6870: add      sp, sp, #0x24
003c6874: pop      {r4, r5, r6, r7, pc}
003c6878: bl       #0x30e310
003c687c: ldrheq   lr, [ip], #-0x20
003c6880: andeq    r4, r0, ip, lsr #1
003c6884: andeq    r0, r0, r4, lsl #17
003c6888: subeq    lr, pc, ip, lsr r6

# _ZN9Character9CSM_SpawnEiPviRi
003ad2e4: cmp      r3, #0
003ad2e8: push     {r4, lr}
003ad2ec: mov      r4, r0
003ad2f0: bne      #0x3ad2fc
003ad2f4: pop      {r4, lr}
003ad2f8: b        #0x3a5248
003ad2fc: cmp      r3, #0x11
003ad300: beq      #0x3ad30c
003ad304: mov      r0, #1
003ad308: pop      {r4, pc}
003ad30c: ldr      r0, [r0, #0x3fc]
003ad310: cmp      r0, #0
003ad314: beq      #0x3ad328
003ad318: ldr      r1, [r4, #0x3cc]
003ad31c: bl       #0x3d24fc
003ad320: cmp      r0, #0
003ad324: beq      #0x3ad330
003ad328: movw     r3, #0x1430
003ad32c: ldrb     r0, [r4, r3]
003ad330: pop      {r4, pc}

# _ZN10CSPreSpawn7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c688c: push     {r4, r5, r6, r7, r8, sl, lr}
003c6890: ldr      r5, [pc, #0x168]
003c6894: ldr      r7, [pc, #0x168]
003c6898: ldr      r1, [pc, #0x168]
003c689c: add      r5, pc, r5
003c68a0: ldr      r3, [r5, r7]
003c68a4: ldr      r8, [r5, r1]
003c68a8: sub      sp, sp, #0x24
003c68ac: ldr      r3, [r3]
003c68b0: mov      r0, r8
003c68b4: mov      r4, r2
003c68b8: str      r3, [sp, #0x1c]
003c68bc: bl       #0x337888
003c68c0: ldr      r1, [pc, #0x144]
003c68c4: add      r6, sp, #4
003c68c8: mov      r2, sp
003c68cc: mov      r0, r6
003c68d0: add      r1, pc, r1
003c68d4: bl       #0x3140ec
003c68d8: mov      r1, r6
003c68dc: mov      r0, r8
003c68e0: bl       #0x337a88
003c68e4: mov      r0, r6
003c68e8: bl       #0x318254
003c68ec: mov      r3, #0x1300
003c68f0: str      r3, [r4, #0x520]
003c68f4: ldr      r3, [pc, #0x114]
003c68f8: mov      r0, r4
003c68fc: mov      r6, #0xa0
003c6900: ldr      r8, [r5, r3]
003c6904: ldr      sl, [r8]
003c6908: bl       #0x3a3228
003c690c: mla      r0, r6, r0, sl
003c6910: ldr      r3, [r0, #0x64]
003c6914: cmn      r3, #1
003c6918: beq      #0x3c6990
003c691c: mov      r0, r4
003c6920: ldr      r8, [r8]
003c6924: bl       #0x3a3228
003c6928: mla      r6, r6, r0, r8
003c692c: add      r0, r4, #0x490
003c6930: add      r0, r0, #0xc
003c6934: ldr      r1, [r6, #0x64]
003c6938: bl       #0x3cacb0
003c693c: mov      r1, #0
003c6940: mov      r2, r1
003c6944: mov      r0, r4
003c6948: bl       #0x394bf8
003c694c: movw     r3, #0x13e4
003c6950: ldrb     r1, [r4, r3]
003c6954: cmp      r1, #0
003c6958: bne      #0x3c696c
003c695c: ldr      r3, [r4]
003c6960: mov      r0, r4
003c6964: mov      lr, pc
003c6968: ldr      pc, [r3, #0x40]
003c696c: mov      r0, r4
003c6970: bl       #0x3949b0
003c6974: ldr      r3, [r5, r7]
003c6978: ldr      r2, [sp, #0x1c]
003c697c: ldr      r3, [r3]
003c6980: cmp      r2, r3
003c6984: bne      #0x3c69fc
003c6988: add      sp, sp, #0x24
003c698c: pop      {r4, r5, r6, r7, r8, sl, pc}
003c6990: mov      r0, r4
003c6994: ldr      r8, [r8]
003c6998: bl       #0x3a3228
003c699c: ldr      r3, [pc, #0x70]
003c69a0: ldr      r1, [pc, #0x70]
003c69a4: ldr      r2, [r5, r3]
003c69a8: mla      r3, r6, r0, r8
003c69ac: ldr      r0, [r2, #0x2c]
003c69b0: ldr      r2, [pc, #0x64]
003c69b4: add      r1, pc, r1
003c69b8: ldr      r6, [r3, #0x80]
003c69bc: add      r2, pc, r2
003c69c0: bl       #0x4c4bdc
003c69c4: add      r8, r4, #0x490
003c69c8: ands     r0, r0, #1
003c69cc: add      r8, r8, #0xc
003c69d0: bne      #0x3c69f0
003c69d4: add      r1, r0, r6
003c69d8: mov      r0, r8
003c69dc: bl       #0x3cacb0
003c69e0: mov      r0, r8
003c69e4: mov      r1, #0
003c69e8: bl       #0x3c93fc
003c69ec: b        #0x3c693c
003c69f0: mov      r0, r4
003c69f4: bl       #0x3a53e0
003c69f8: b        #0x3c69d4
003c69fc: bl       #0x30e310
003c6a00: ldrsheq  lr, [ip], #-0x14
003c6a04: andeq    r4, r0, ip, lsr #1
003c6a08: andeq    r0, r0, r4, lsl #17
003c6a0c: subeq    lr, pc, r0, lsl #11
003c6a10: andeq    r4, r0, r4, asr #16
003c6a14: strdeq   r3, r4, [r0], -r4
003c6a18: subeq    lr, pc, r4, lsl #4
003c6a1c: subeq    lr, pc, ip, lsl #4

# _ZN16CharStateMachine16SM_SetSpawnStateEbb
003c2734: push     {r4, r5, lr}
003c2738: subs     r3, r1, #0
003c273c: sub      sp, sp, #0xc
003c2740: mov      r4, r0
003c2744: beq      #0x3c27d8
003c2748: ldr      r2, [r0, #4]
003c274c: movw     r1, #0x1434
003c2750: ldr      r3, [r2, r1]
003c2754: cmp      r3, #0
003c2758: movlt    r3, #0
003c275c: strlt    r3, [r2, r1]
003c2760: movw     r1, #0x1438
003c2764: ldr      r0, [r2, r1]
003c2768: cmp      r0, r3
003c276c: movwlt   r0, #0x1434
003c2770: ldrlt    r5, [r2, r0]
003c2774: movge    r5, r3
003c2778: movge    r3, r0
003c277c: strlt    r3, [r2, r1]
003c2780: cmp      r3, r5
003c2784: bne      #0x3c27a8
003c2788: cmp      r3, #0
003c278c: bne      #0x3c27ec
003c2790: mov      r0, r4
003c2794: mov      r1, #1
003c2798: mvn      r2, #0
003c279c: add      sp, sp, #0xc
003c27a0: pop      {r4, r5, lr}
003c27a4: b        #0x3c1938
003c27a8: rsb      r0, r5, r3
003c27ac: bl       #0x3c26a0
003c27b0: ldr      r3, [r4, #4]
003c27b4: mov      ip, #0
003c27b8: add      r1, r0, r5
003c27bc: mov      r2, ip
003c27c0: add      r0, r3, #0x3b4
003c27c4: mov      r3, #0x2d
003c27c8: str      ip, [sp]
003c27cc: bl       #0x3dbe24
003c27d0: add      sp, sp, #0xc
003c27d4: pop      {r4, r5, pc}
003c27d8: mov      r1, #1
003c27dc: mvn      r2, #0
003c27e0: add      sp, sp, #0xc
003c27e4: pop      {r4, r5, lr}
003c27e8: b        #0x3c1938
003c27ec: ldr      r0, [r4, #4]
003c27f0: mov      ip, #0
003c27f4: mov      r1, r3
003c27f8: mov      r2, ip
003c27fc: mov      r3, #0x2d
003c2800: add      r0, r0, #0x3b4
003c2804: str      ip, [sp]
003c2808: bl       #0x3dbe24
003c280c: b        #0x3c27d0

# _ZNK9Character13GetAnimStanceEv
003a53e0: push     {r4, r5, r6, lr}
003a53e4: ldr      r3, [r0]
003a53e8: mov      r4, r0
003a53ec: mov      lr, pc
003a53f0: ldr      pc, [r3, #0x28]
003a53f4: ldr      r5, [pc, #0xa8]
003a53f8: cmp      r0, #0
003a53fc: add      r5, pc, r5
003a5400: bne      #0x3a5438
003a5404: mov      r4, #0
003a5408: ldr      r3, [pc, #0x98]
003a540c: ldr      r1, [pc, #0x98]
003a5410: ldr      r2, [pc, #0x98]
003a5414: ldr      r3, [r5, r3]
003a5418: add      r1, pc, r1
003a541c: add      r2, pc, r2
003a5420: ldr      r0, [r3, #0x2c]
003a5424: bl       #0x4c4bdc
003a5428: cmp      r4, r0
003a542c: movlt    r0, r4
003a5430: movge    r0, #0
003a5434: pop      {r4, r5, r6, pc}
003a5438: add      r4, r4, #0x37c
003a543c: mov      r0, r4
003a5440: bl       #0x4000c8
003a5444: cmp      r0, #0
003a5448: movne    r4, #3
003a544c: bne      #0x3a5408
003a5450: mov      r0, r4
003a5454: bl       #0x400080
003a5458: cmp      r0, #0
003a545c: movne    r4, #4
003a5460: bne      #0x3a5408
003a5464: mov      r0, r4
003a5468: bl       #0x40019c
003a546c: subs     r1, r0, #0
003a5470: movne    r4, #2
003a5474: bne      #0x3a5408
003a5478: mov      r0, r4
003a547c: bl       #0x4001a0
003a5480: cmp      r0, #0
003a5484: movne    r4, #1
003a5488: bne      #0x3a5408
003a548c: mov      r0, r4
003a5490: bl       #0x3ffe8c
003a5494: cmp      r0, #0
003a5498: moveq    r4, #5
003a549c: beq      #0x3a5408
003a54a0: b        #0x3a5404

# _ZNK9Character18GetCharAnimTableIdEv
003a3228: mov      r3, #0x1000
003a322c: ldr      r0, [r0, r3]
003a3230: ldr      r3, [pc, #0x24]
003a3234: cmp      r0, #0
003a3238: add      r3, pc, r3
003a323c: blt      #0x3a3254
003a3240: ldr      r2, [pc, #0x18]
003a3244: ldr      r3, [r3, r2]
003a3248: ldr      r3, [r3]
003a324c: cmp      r0, r3
003a3250: bxlt     lr
003a3254: mov      r0, #0x11
003a3258: bx       lr
003a325c: subseq   r1, pc, r8, asr r8
003a3260: andeq    r2, r0, r0, asr #17

# _ZN10CSPreSpawn8OnUpdateEiP9CharacterP16CharStateMachine
003c0070: bx       lr
