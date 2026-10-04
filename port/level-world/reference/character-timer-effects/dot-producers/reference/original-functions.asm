
# _Z16CF_CalcDotDamageP9CharacterS0_b
003b09c4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b09c8: add      r4, r1, #0xff0
003b09cc: subs     r7, r3, #0
003b09d0: add      r4, r4, #4
003b09d4: add      r6, r1, #0x560
003b09d8: mov      sl, r2
003b09dc: mov      r1, r4
003b09e0: movne    r2, #0xb5
003b09e4: moveq    r2, #0x7d
003b09e8: mov      r5, r0
003b09ec: mov      r0, r6
003b09f0: bl       #0x3dedb4
003b09f4: cmp      r0, #0
003b09f8: mov      r8, r0
003b09fc: mvnle    r4, #0
003b0a00: movle    r7, #0
003b0a04: ble      #0x3b0a78
003b0a08: cmp      r7, #0
003b0a0c: beq      #0x3b0a8c
003b0a10: mov      r1, r4
003b0a14: mov      r2, #0xb3
003b0a18: mov      r0, r6
003b0a1c: bl       #0x3dedb4
003b0a20: mov      r1, r4
003b0a24: mov      r7, r0
003b0a28: mov      r2, #0xb4
003b0a2c: mov      r0, r6
003b0a30: bl       #0x3dedb4
003b0a34: rsb      r0, r7, r0
003b0a38: bl       #0x3af6d8
003b0a3c: mov      r1, r4
003b0a40: add      r7, r0, r7
003b0a44: mov      r2, #0xb2
003b0a48: mov      r0, r6
003b0a4c: bl       #0x3dedb4
003b0a50: asr      r4, r0, #8
003b0a54: cmn      r4, #1
003b0a58: beq      #0x3b0a78
003b0a5c: add      r1, sl, #0xff0
003b0a60: add      r1, r1, #4
003b0a64: add      r0, sl, #0x560
003b0a68: add      r2, r4, #0x4a
003b0a6c: bl       #0x3dedb4
003b0a70: rsb      r7, r0, r7
003b0a74: bic      r7, r7, r7, asr #31
003b0a78: str      r8, [r5]
003b0a7c: str      r7, [r5, #4]
003b0a80: str      r4, [r5, #8]
003b0a84: mov      r0, r5
003b0a88: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b0a8c: mov      r1, r4
003b0a90: mov      r2, #0x7b
003b0a94: mov      r0, r6
003b0a98: bl       #0x3dedb4
003b0a9c: mov      r1, r4
003b0aa0: mov      r7, r0
003b0aa4: mov      r2, #0x7c
003b0aa8: mov      r0, r6
003b0aac: bl       #0x3dedb4
003b0ab0: rsb      r0, r7, r0
003b0ab4: bl       #0x3af6d8
003b0ab8: mvn      r4, #0
003b0abc: add      r7, r7, r0
003b0ac0: str      r8, [r5]
003b0ac4: str      r7, [r5, #4]
003b0ac8: str      r4, [r5, #8]
003b0acc: mov      r0, r5
003b0ad0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN14CharProperties12PROPS_AddDotEiii
003e2720: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2724: ldr      r4, [pc, #0x2c4]
003e2728: sub      sp, sp, #0x1c
003e272c: cmp      r1, #0
003e2730: add      r4, pc, r4
003e2734: str      r1, [sp, #0x14]
003e2738: mov      r7, r0
003e273c: mov      r6, r2
003e2740: mov      r5, r3
003e2744: ble      #0x3e288c
003e2748: cmp      r6, #0
003e274c: blt      #0x3e2834
003e2750: ldr      r3, [pc, #0x29c]
003e2754: ldr      r3, [r4, r3]
003e2758: ldr      sl, [r3]
003e275c: cmp      sl, #0
003e2760: beq      #0x3e282c
003e2764: ldr      r3, [pc, #0x28c]
003e2768: ldr      fp, [pc, #0x28c]
003e276c: mov      r8, #0
003e2770: ldr      r3, [r4, r3]
003e2774: add      fp, pc, fp
003e2778: ldr      sb, [r3]
003e277c: b        #0x3e278c
003e2780: add      r8, r8, #1
003e2784: cmp      r8, sl
003e2788: beq      #0x3e282c
003e278c: ldr      r1, [sb, r8, lsl #2]
003e2790: mov      r0, fp
003e2794: bl       #0x30e31c
003e2798: cmp      r0, #0
003e279c: bne      #0x3e2780
003e27a0: ldr      r3, [pc, #0x258]
003e27a4: add      r8, r8, r5
003e27a8: str      r8, [sp, #0x10]
003e27ac: ldr      r3, [r4, r3]
003e27b0: ldr      sl, [r3]
003e27b4: cmp      sl, #0
003e27b8: beq      #0x3e2824
003e27bc: ldr      r3, [pc, #0x240]
003e27c0: ldr      fp, [pc, #0x240]
003e27c4: mov      r8, #0
003e27c8: ldr      r3, [r4, r3]
003e27cc: add      fp, pc, fp
003e27d0: ldr      sb, [r3]
003e27d4: b        #0x3e27e4
003e27d8: add      r8, r8, #1
003e27dc: cmp      r8, sl
003e27e0: beq      #0x3e2824
003e27e4: ldr      r1, [sb, r8, lsl #2]
003e27e8: mov      r0, fp
003e27ec: bl       #0x30e31c
003e27f0: cmp      r0, #0
003e27f4: bne      #0x3e27d8
003e27f8: add      r3, r5, #1
003e27fc: add      r8, r8, r5
003e2800: cmp      r3, #5
003e2804: addls    pc, pc, r3, lsl #2
003e2808: b        #0x3e28e4
003e280c: b        #0x3e2950
003e2810: b        #0x3e295c
003e2814: b        #0x3e2968
003e2818: b        #0x3e2974
003e281c: b        #0x3e2980
003e2820: b        #0x3e298c
003e2824: mvn      r8, #0
003e2828: b        #0x3e27f8
003e282c: mvn      r8, #0
003e2830: b        #0x3e27a0
003e2834: ldr      r3, [pc, #0x1d0]
003e2838: ldr      r3, [r4, r3]
003e283c: ldr      r3, [r3]
003e2840: cmp      r3, #2
003e2844: moveq    r3, #0
003e2848: streq    r3, [r3]
003e284c: beq      #0x3e2750
003e2850: cmp      r3, #1
003e2854: bne      #0x3e2750
003e2858: ldr      r0, [pc, #0x1b0]
003e285c: ldr      r1, [pc, #0x1b0]
003e2860: ldr      r2, [pc, #0x1b0]
003e2864: ldr      r0, [r4, r0]
003e2868: ldr      r3, [pc, #0x1ac]
003e286c: movw     ip, #0x436
003e2870: add      r1, pc, r1
003e2874: add      r2, pc, r2
003e2878: add      r3, pc, r3
003e287c: add      r0, r0, #0xa8
003e2880: str      ip, [sp]
003e2884: bl       #0x30e004
003e2888: b        #0x3e2750
003e288c: ldr      r3, [pc, #0x178]
003e2890: ldr      r3, [r4, r3]
003e2894: ldr      r3, [r3]
003e2898: cmp      r3, #2
003e289c: moveq    r3, #0
003e28a0: streq    r3, [r3]
003e28a4: beq      #0x3e2748
003e28a8: cmp      r3, #1
003e28ac: bne      #0x3e2748
003e28b0: ldr      r0, [pc, #0x158]
003e28b4: ldr      r1, [pc, #0x164]
003e28b8: ldr      r2, [pc, #0x164]
003e28bc: ldr      r0, [r4, r0]
003e28c0: ldr      r3, [pc, #0x160]
003e28c4: movw     ip, #0x435
003e28c8: add      r1, pc, r1
003e28cc: add      r2, pc, r2
003e28d0: add      r3, pc, r3
003e28d4: add      r0, r0, #0xa8
003e28d8: str      ip, [sp]
003e28dc: bl       #0x30e004
003e28e0: b        #0x3e2748
003e28e4: ldr      r3, [pc, #0x120]
003e28e8: ldr      r3, [r4, r3]
003e28ec: ldr      r3, [r3]
003e28f0: cmp      r3, #2
003e28f4: beq      #0x3e29a0
003e28f8: cmp      r3, #1
003e28fc: beq      #0x3e29b4
003e2900: ldr      ip, [pc, #0x124]
003e2904: add      ip, pc, ip
003e2908: ldr      r1, [sp, #0x10]
003e290c: ldr      r2, [sp, #0x14]
003e2910: mov      r0, r7
003e2914: mov      r3, #1
003e2918: stm      sp, {r6, r8, ip}
003e291c: bl       #0x3e232c
003e2920: subs     r1, r0, #0
003e2924: beq      #0x3e2998
003e2928: add      r5, r5, #0x7f
003e292c: mov      r0, r7
003e2930: mov      r3, r6
003e2934: mov      r2, r5
003e2938: bl       #0x3deca0
003e293c: mov      r0, r7
003e2940: mov      r1, r5
003e2944: add      sp, sp, #0x1c
003e2948: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e294c: b        #0x3dfe60
003e2950: ldr      ip, [pc, #0xd8]
003e2954: add      ip, pc, ip
003e2958: b        #0x3e2908
003e295c: ldr      ip, [pc, #0xd0]
003e2960: add      ip, pc, ip
003e2964: b        #0x3e2908
003e2968: ldr      ip, [pc, #0xc8]
003e296c: add      ip, pc, ip
003e2970: b        #0x3e2908
003e2974: ldr      ip, [pc, #0xc0]
003e2978: add      ip, pc, ip
003e297c: b        #0x3e2908
003e2980: ldr      ip, [pc, #0xb8]
003e2984: add      ip, pc, ip
003e2988: b        #0x3e2908
003e298c: ldr      ip, [pc, #0xb0]
003e2990: add      ip, pc, ip
003e2994: b        #0x3e2908
003e2998: add      sp, sp, #0x1c
003e299c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e29a0: ldr      ip, [pc, #0xa0]
003e29a4: mov      r3, #0
003e29a8: str      r3, [r3]
003e29ac: add      ip, pc, ip
003e29b0: b        #0x3e2908
003e29b4: ldr      r0, [pc, #0x54]
003e29b8: ldr      r1, [pc, #0x8c]
003e29bc: ldr      r2, [pc, #0x8c]
003e29c0: ldr      r0, [r4, r0]
003e29c4: ldr      r3, [pc, #0x88]
003e29c8: movw     ip, #0x445
003e29cc: add      r1, pc, r1
003e29d0: add      r0, r0, #0xa8
003e29d4: add      r2, pc, r2
003e29d8: add      r3, pc, r3
003e29dc: str      ip, [sp]
003e29e0: bl       #0x30e004
003e29e4: ldr      ip, [pc, #0x6c]
003e29e8: add      ip, pc, ip
003e29ec: b        #0x3e2908
003e29f0: subseq   r2, fp, r0, ror #6
003e29f4: andeq    r3, r0, r8, ror #10
003e29f8: muleq    r0, r0, sl
003e29fc: subeq    r3, lr, r4, asr r6
003e2a00: andeq    r0, r0, r4, asr #13
003e2a04: muleq    r0, r4, r2
003e2a08: strdeq   r3, r4, [lr], #-0x5c
003e2a0c: andeq    r3, r0, r0, asr #19
003e2a10: andeq    r1, r0, r0, asr #19
003e2a14: subeq    fp, sp, r8, ror #22
003e2a18: subeq    r3, lr, r4, asr #10
003e2a1c: subeq    r3, lr, r8, lsr r4
003e2a20: subeq    fp, sp, r0, lsl fp
003e2a24: ldrdeq   r3, r4, [lr], #-0x4c
003e2a28: subeq    r3, lr, r0, ror #7
003e2a2c: subeq    r3, lr, r4, lsr r5
003e2a30: umaaleq  r3, lr, ip, r4
003e2a34: subeq    r3, lr, r0, lsl #9
003e2a38: umaaleq  r3, lr, r4, r4
003e2a3c: umaaleq  r3, lr, r8, r4
003e2a40: umaaleq  r3, lr, ip, r4
003e2a44: subeq    r3, lr, r0, lsr #9
003e2a48: subeq    r3, lr, ip, lsl #9
003e2a4c: subeq    fp, sp, ip, lsl #20
003e2a50: subeq    r3, lr, r4, ror r4
003e2a54: ldrdeq   r3, r4, [lr], #-0x28
003e2a58: subeq    r3, lr, r0, asr r4

# _ZNK6CharAI13AI_IsInCombatEv
003d4bc4: push     {r4, lr}
003d4bc8: mov      r4, r0
003d4bcc: bl       #0x3d49f0
003d4bd0: cmp      r0, #0
003d4bd4: beq      #0x3d4be0
003d4bd8: mov      r0, #1
003d4bdc: pop      {r4, pc}
003d4be0: mov      r0, r4
003d4be4: bl       #0x3d4a00
003d4be8: cmp      r0, #0
003d4bec: bne      #0x3d4bd8
003d4bf0: ldr      r0, [r4, #4]
003d4bf4: add      r0, r0, #0x4f0
003d4bf8: add      r0, r0, #0xc
003d4bfc: bl       #0x3c02d0
003d4c00: cmp      r0, #0
003d4c04: bne      #0x3d4bd8
003d4c08: ldr      r0, [r4, #4]
003d4c0c: add      r0, r0, #0x4f0
003d4c10: add      r0, r0, #0xc
003d4c14: bl       #0x3c02e8
003d4c18: cmp      r0, #0
003d4c1c: bne      #0x3d4bd8
003d4c20: ldr      r0, [r4, #4]
003d4c24: add      r0, r0, #0x4f0
003d4c28: add      r0, r0, #0xc
003d4c2c: pop      {r4, lr}
003d4c30: b        #0x3c0334

# _ZN9Character7RegenMPEi
003bdbb8: push     {r4, r5, r6, r7, r8, sl, lr}
003bdbbc: ldr      r5, [pc, #0xd0]
003bdbc0: ldr      r8, [pc, #0xd0]
003bdbc4: add      r6, r0, #0xff0
003bdbc8: add      r5, pc, r5
003bdbcc: ldr      r3, [r5, r8]
003bdbd0: add      r6, r6, #4
003bdbd4: add      r7, r0, #0x560
003bdbd8: ldr      r3, [r3]
003bdbdc: mov      r4, r1
003bdbe0: sub      sp, sp, #0x24
003bdbe4: mov      r2, #0x29
003bdbe8: mov      r1, r6
003bdbec: mov      r0, r7
003bdbf0: str      r3, [sp, #0x1c]
003bdbf4: bl       #0x3dedb4
003bdbf8: mov      sl, r0
003bdbfc: mov      r1, r6
003bdc00: mov      r0, r7
003bdc04: mov      r2, #0x2b
003bdc08: bl       #0x3dedb4
003bdc0c: cmp      r4, #0
003bdc10: movlt    r4, r0
003bdc14: add      r3, r4, sl
003bdc18: cmp      r3, r0
003bdc1c: rsbgt    r4, sl, r0
003bdc20: cmp      r4, #0
003bdc24: ble      #0x3bdc74
003bdc28: ldr      r3, [pc, #0x6c]
003bdc2c: add      r6, sp, #4
003bdc30: ldr      sl, [r5, r3]
003bdc34: mov      r0, sl
003bdc38: bl       #0x337888
003bdc3c: ldr      r1, [pc, #0x5c]
003bdc40: mov      r2, sp
003bdc44: mov      r0, r6
003bdc48: add      r1, pc, r1
003bdc4c: bl       #0x3140ec
003bdc50: mov      r1, r6
003bdc54: mov      r0, sl
003bdc58: bl       #0x337a88
003bdc5c: mov      r0, r6
003bdc60: bl       #0x318254
003bdc64: mov      r0, r7
003bdc68: mov      r2, r4
003bdc6c: mov      r1, #0x29
003bdc70: bl       #0x3e0708
003bdc74: ldr      r3, [r5, r8]
003bdc78: ldr      r2, [sp, #0x1c]
003bdc7c: ldr      r3, [r3]
003bdc80: cmp      r2, r3
003bdc84: bne      #0x3bdc90
003bdc88: add      sp, sp, #0x24
003bdc8c: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdc90: bl       #0x30e310
003bdc94: subseq   r6, sp, r8, asr #29
003bdc98: andeq    r4, r0, ip, lsr #1
003bdc9c: andeq    r0, r0, r4, lsl #17
003bdca0: subseq   r6, r0, r8, asr #24

# _ZN14CharProperties15PROPS_RemoveDotEi
003de83c: bx       lr

# _ZN9Character7RegenHPEi
003bdca4: push     {r4, r5, r6, r7, r8, sl, lr}
003bdca8: ldr      r5, [pc, #0xd0]
003bdcac: ldr      r8, [pc, #0xd0]
003bdcb0: add      r6, r0, #0xff0
003bdcb4: add      r5, pc, r5
003bdcb8: ldr      r3, [r5, r8]
003bdcbc: add      r6, r6, #4
003bdcc0: add      r7, r0, #0x560
003bdcc4: ldr      r3, [r3]
003bdcc8: mov      r4, r1
003bdccc: sub      sp, sp, #0x24
003bdcd0: mov      r2, #0x24
003bdcd4: mov      r1, r6
003bdcd8: mov      r0, r7
003bdcdc: str      r3, [sp, #0x1c]
003bdce0: bl       #0x3dedb4
003bdce4: mov      sl, r0
003bdce8: mov      r1, r6
003bdcec: mov      r0, r7
003bdcf0: mov      r2, #0x26
003bdcf4: bl       #0x3dedb4
003bdcf8: cmp      r4, #0
003bdcfc: movlt    r4, r0
003bdd00: add      r3, r4, sl
003bdd04: cmp      r3, r0
003bdd08: rsbgt    r4, sl, r0
003bdd0c: cmp      r4, #0
003bdd10: ble      #0x3bdd60
003bdd14: ldr      r3, [pc, #0x6c]
003bdd18: add      r6, sp, #4
003bdd1c: ldr      sl, [r5, r3]
003bdd20: mov      r0, sl
003bdd24: bl       #0x337888
003bdd28: ldr      r1, [pc, #0x5c]
003bdd2c: mov      r2, sp
003bdd30: mov      r0, r6
003bdd34: add      r1, pc, r1
003bdd38: bl       #0x3140ec
003bdd3c: mov      r1, r6
003bdd40: mov      r0, sl
003bdd44: bl       #0x337a88
003bdd48: mov      r0, r6
003bdd4c: bl       #0x318254
003bdd50: mov      r0, r7
003bdd54: mov      r2, r4
003bdd58: mov      r1, #0x24
003bdd5c: bl       #0x3e0708
003bdd60: ldr      r3, [r5, r8]
003bdd64: ldr      r2, [sp, #0x1c]
003bdd68: ldr      r3, [r3]
003bdd6c: cmp      r2, r3
003bdd70: bne      #0x3bdd7c
003bdd74: add      sp, sp, #0x24
003bdd78: pop      {r4, r5, r6, r7, r8, sl, pc}
003bdd7c: bl       #0x30e310
003bdd80: ldrsbeq  r6, [sp], #-0xdc
003bdd84: andeq    r4, r0, ip, lsr #1
003bdd88: andeq    r0, r0, r4, lsl #17
003bdd8c: subseq   r6, r0, ip, asr fp
