
# _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKNS0_8SChannelEPPv
0061c6bc: str      lr, [sp, #-4]!
0061c6c0: mov      ip, r1
0061c6c4: ldr      lr, [ip, #8]
0061c6c8: sub      sp, sp, #0xc
0061c6cc: ldr      r1, [r1, #4]
0061c6d0: mov      r3, r2
0061c6d4: add      ip, ip, #0xc
0061c6d8: mov      r2, lr
0061c6dc: str      ip, [sp]
0061c6e0: bl       #0x61c2f8
0061c6e4: add      sp, sp, #0xc
0061c6e8: ldm      sp!, {pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_25CSceneNodeQuaternionMixinIfEEfLi4ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
00613294: push     {r4, r5, r6, lr}
00613298: mov      r4, r1
0061329c: sub      sp, sp, #8
006132a0: mov      r1, #0
006132a4: mov      r5, r3
006132a8: bl       #0x669e24
006132ac: mov      r1, r5
006132b0: mov      r6, r0
006132b4: mov      r0, #0x3f800000
006132b8: bl       #0x30e3ac
006132bc: str      r5, [sp, #4]
006132c0: str      r0, [sp]
006132c4: ldr      r0, [r6, #4]
006132c8: ldr      r3, [sp, #0x18]
006132cc: mov      r1, sp
006132d0: add      r0, r0, r4, lsl #4
006132d4: mov      r2, #2
006132d8: bl       #0x6130d4
006132dc: add      sp, sp, #8
006132e0: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada18SAnimationAccessor8getValueEiPvRib
0066a1a8: push     {r4, r5, r6, r7, r8, lr}
0066a1ac: sub      sp, sp, #8
0066a1b0: mov      r7, r1
0066a1b4: mov      r6, r2
0066a1b8: mov      r5, r3
0066a1bc: mov      r8, r0
0066a1c0: ldrb     r4, [sp, #0x20]
0066a1c4: bl       #0x66a004
0066a1c8: mov      r1, r8
0066a1cc: ldr      ip, [r0]
0066a1d0: mov      r2, r7
0066a1d4: mov      r3, r6
0066a1d8: str      r5, [sp]
0066a1dc: str      r4, [sp, #4]
0066a1e0: mov      lr, pc
0066a1e4: ldr      pc, [ip, #0x68]
0066a1e8: add      sp, sp, #8
0066a1ec: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_20CSceneNodeScaleMixinIfEEfLi3ENS1_15SUseDefaultLerpIfEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
00628850: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00628854: mov      r7, r1
00628858: sub      sp, sp, #8
0062885c: mov      r1, #0
00628860: mov      r4, r3
00628864: ldr      r6, [sp, #0x28]
00628868: bl       #0x669e24
0062886c: mov      r1, r4
00628870: mov      r8, r0
00628874: mov      r0, #0x3f800000
00628878: bl       #0x30e3ac
0062887c: str      r4, [sp, #4]
00628880: str      r0, [sp]
00628884: mov      r3, #0xc
00628888: mul      r7, r3, r7
0062888c: ldr      r3, [r8, #4]
00628890: mov      r5, r0
00628894: ldr      r1, [r3, r7]
00628898: add      r7, r3, r7
0062889c: bl       #0x30ed6c
006288a0: mov      r1, #0
006288a4: bl       #0x30eba4
006288a8: ldr      r1, [r7, #4]
006288ac: mov      r8, r0
006288b0: mov      r0, r5
006288b4: bl       #0x30ed6c
006288b8: mov      r1, #0
006288bc: bl       #0x30eba4
006288c0: add      r7, r7, #4
006288c4: ldr      r1, [r7, #4]
006288c8: mov      sl, r0
006288cc: mov      r0, r5
006288d0: bl       #0x30ed6c
006288d4: mov      r1, #0
006288d8: bl       #0x30eba4
006288dc: add      r5, r7, #4
006288e0: add      sb, r5, #4
006288e4: ldr      r1, [sb, #4]
006288e8: mov      r7, r0
006288ec: mov      r0, r4
006288f0: bl       #0x30ed6c
006288f4: mov      r1, r0
006288f8: mov      r0, sl
006288fc: bl       #0x30eba4
00628900: add      sb, sb, #4
00628904: ldr      r1, [sb, #4]
00628908: mov      sl, r0
0062890c: mov      r0, r4
00628910: bl       #0x30ed6c
00628914: mov      r1, r7
00628918: bl       #0x30eba4
0062891c: ldr      r1, [r5, #4]
00628920: mov      r7, r0
00628924: mov      r0, r4
00628928: bl       #0x30ed6c
0062892c: mov      r1, r0
00628930: mov      r0, r8
00628934: bl       #0x30eba4
00628938: mov      r3, r6
0062893c: str      r0, [r3], #4
00628940: str      sl, [r6, #4]
00628944: str      r7, [r3, #4]
00628948: add      sp, sp, #8
0062894c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIiLi1000EEEbRKNS_3res6vectorIiEEiRii
0066a368: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a36c: mov      r0, r2
0066a370: sub      sp, sp, #0xc
0066a374: mov      r4, r1
0066a378: mov      fp, r3
0066a37c: bl       #0x30e964
0066a380: ldr      r5, [r4]
0066a384: ldr      r8, [sp, #0x30]
0066a388: ldr      r7, [r4, #4]
0066a38c: sub      r5, r5, #1
0066a390: bic      r8, r8, r8, asr #31
0066a394: cmp      r8, r5
0066a398: movge    r8, r5
0066a39c: mov      r6, r0
0066a3a0: ldr      r0, [r7, r8, lsl #2]
0066a3a4: bl       #0x30e964
0066a3a8: mov      r1, r6
0066a3ac: bl       #0x30e2f8
0066a3b0: cmp      r0, #0
0066a3b4: mov      sl, #0
0066a3b8: movne    sl, #1
0066a3bc: uxtb     sl, sl
0066a3c0: cmp      sl, #0
0066a3c4: lsl      r2, r8, #2
0066a3c8: beq      #0x66a488
0066a3cc: cmp      r8, #0
0066a3d0: subgt    r8, r8, #1
0066a3d4: ble      #0x66a488
0066a3d8: cmp      r8, r5
0066a3dc: bge      #0x66a58c
0066a3e0: ldr      r0, [r7, r8, lsl #2]
0066a3e4: bl       #0x30e964
0066a3e8: mov      r1, r0
0066a3ec: mov      r0, r6
0066a3f0: bl       #0x30e70c
0066a3f4: cmp      r0, #0
0066a3f8: mov      sl, #0
0066a3fc: movne    sl, #1
0066a400: lsl      r3, r8, #2
0066a404: uxtb     sl, sl
0066a408: mov      sb, r8
0066a40c: cmp      sl, #0
0066a410: beq      #0x66a564
0066a414: cmp      r5, #0
0066a418: ble      #0x66a450
0066a41c: mov      sl, #1
0066a420: add      r8, sl, r5
0066a424: asr      r8, r8, #1
0066a428: ldr      r0, [r7, r8, lsl #2]
0066a42c: bl       #0x30e964
0066a430: mov      r1, r0
0066a434: mov      r0, r6
0066a438: bl       #0x30e70c
0066a43c: cmp      r0, #0
0066a440: subne    r5, r8, #1
0066a444: addeq    sl, r8, #1
0066a448: cmp      sl, r5
0066a44c: ble      #0x66a420
0066a450: str      r5, [fp]
0066a454: ldr      r3, [r4, #4]
0066a458: ldr      r0, [r3, r5, lsl #2]
0066a45c: bl       #0x30e964
0066a460: mov      r1, r0
0066a464: mov      r0, r6
0066a468: bl       #0x30df8c
0066a46c: cmp      r0, #0
0066a470: bne      #0x66a55c
0066a474: ldr      r0, [r4]
0066a478: sub      r0, r0, #1
0066a47c: subs     r0, r0, r5
0066a480: movne    r0, #1
0066a484: b        #0x66a554
0066a488: cmp      r5, r8
0066a48c: ble      #0x66a510
0066a490: add      sb, r8, #1
0066a494: ldr      r0, [r7, sb, lsl #2]
0066a498: str      r2, [sp]
0066a49c: bl       #0x30e964
0066a4a0: mov      r1, r6
0066a4a4: str      r0, [sp, #4]
0066a4a8: bl       #0x30e70c
0066a4ac: ldr      r2, [sp]
0066a4b0: lsl      r3, sb, #2
0066a4b4: cmp      r0, #0
0066a4b8: mov      r1, r3
0066a4bc: moveq    sb, r8
0066a4c0: moveq    r3, r2
0066a4c4: beq      #0x66a40c
0066a4c8: cmp      r5, sb
0066a4cc: ble      #0x66a518
0066a4d0: add      r8, sb, #1
0066a4d4: ldr      r0, [r7, r8, lsl #2]
0066a4d8: str      r3, [sp]
0066a4dc: bl       #0x30e964
0066a4e0: mov      r1, r6
0066a4e4: bl       #0x30e70c
0066a4e8: subs     sl, r0, #0
0066a4ec: bne      #0x66a3d8
0066a4f0: ldr      r1, [sp, #4]
0066a4f4: mov      r0, r6
0066a4f8: bl       #0x30e70c
0066a4fc: cmp      r0, #0
0066a500: movne    sl, #1
0066a504: ldr      r3, [sp]
0066a508: uxtb     sl, sl
0066a50c: b        #0x66a40c
0066a510: mov      sb, r8
0066a514: lsl      r1, r8, #2
0066a518: mov      r3, r1
0066a51c: mov      r8, sb
0066a520: str      r8, [fp]
0066a524: ldr      r2, [r4, #4]
0066a528: ldr      r0, [r2, r3]
0066a52c: bl       #0x30e964
0066a530: mov      r1, r0
0066a534: mov      r0, r6
0066a538: bl       #0x30df8c
0066a53c: cmp      r0, #0
0066a540: bne      #0x66a55c
0066a544: ldr      r0, [r4]
0066a548: sub      r0, r0, #1
0066a54c: subs     r0, r0, r8
0066a550: movne    r0, #1
0066a554: add      sp, sp, #0xc
0066a558: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a55c: mov      r0, #0
0066a560: b        #0x66a554
0066a564: add      r2, sb, #1
0066a568: ldr      r0, [r7, r2, lsl #2]
0066a56c: str      r3, [sp]
0066a570: bl       #0x30e964
0066a574: mov      r1, r6
0066a578: bl       #0x30e70c
0066a57c: cmp      r0, #0
0066a580: ldr      r3, [sp]
0066a584: bne      #0x66a414
0066a588: b        #0x66a51c
0066a58c: lsl      r3, r8, #2
0066a590: b        #0x66a520

# _ZNK6glitch7collada16CColladaDatabase14getVisualSceneEi
0060e54c: ldr      r3, [r0]
0060e550: ldr      r3, [r3, #0x24]
0060e554: ldr      r3, [r3, #0x20]
0060e558: ldr      r2, [r3, #0x98]
0060e55c: cmp      r2, #0
0060e560: ldrgt    r0, [r3, #0x9c]
0060e564: movle    r0, #0
0060e568: addgt    r0, r0, r1, lsl #4
0060e56c: bx       lr

# _ZN6glitch7collada21CSceneNodeAnimatorSet17getAnimationValueEiiPv
0065f7b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065f7b8: mov      r4, r0
0065f7bc: mov      sb, r2
0065f7c0: ldr      r2, [r4, #0xc]
0065f7c4: sub      sp, sp, #0x34
0065f7c8: mov      r5, r1
0065f7cc: ldr      r0, [r0, #0x24]
0065f7d0: ldr      r1, [r4, #0x50]
0065f7d4: mov      sl, r3
0065f7d8: str      r2, [sp, #0xc]
0065f7dc: bl       #0x65f0b4
0065f7e0: ldr      r6, [r4, #0x4c]
0065f7e4: ldr      r3, [r4, #0x24]
0065f7e8: mov      r1, #0xc
0065f7ec: ldr      r2, [r0]
0065f7f0: add      r6, r5, r6
0065f7f4: mul      r6, r1, r6
0065f7f8: ldr      r7, [r3, #0x30]
0065f7fc: ldr      r2, [r2, #0x24]
0065f800: add      r8, r7, r6
0065f804: ldr      r1, [r8, #4]
0065f808: ldr      r2, [r2, #0x20]
0065f80c: cmp      r1, #0
0065f810: ldr      fp, [r2, #0x14]
0065f814: beq      #0x65f844
0065f818: ldr      r3, [r3, #0x18]
0065f81c: ldr      r3, [r3, r5, lsl #2]
0065f820: mov      r0, r3
0065f824: ldr      r3, [r3]
0065f828: str      r1, [sp, #8]
0065f82c: mov      lr, pc
0065f830: ldr      pc, [r3, #8]
0065f834: ldr      r1, [sp, #8]
0065f838: mov      r2, r0
0065f83c: mov      r0, sl
0065f840: bl       #0x30e868
0065f844: ldr      r3, [r7, r6]
0065f848: cmp      r3, #2
0065f84c: bne      #0x65f8a4
0065f850: mov      r3, #0
0065f854: mov      r1, sb
0065f858: mov      r0, r4
0065f85c: strb     r3, [sp, #0x21]
0065f860: bl       #0x65f364
0065f864: ldr      r3, [r8, #8]
0065f868: str      r0, [sp, #0x28]
0065f86c: ldr      r2, [sp, #0xc]
0065f870: str      r3, [sp, #0x24]
0065f874: add      r3, sp, #0x14
0065f878: str      r3, [sp, #0x2c]
0065f87c: ldr      r3, [r4, #0x40]
0065f880: cmp      fp, #0
0065f884: mov      r1, sb
0065f888: addeq    r3, r3, r5, lsl #2
0065f88c: add      r0, sp, #0x24
0065f890: subs     ip, r2, #1
0065f894: movne    ip, #1
0065f898: mov      r2, sl
0065f89c: str      ip, [sp]
0065f8a0: bl       #0x66a1a8
0065f8a4: add      sp, sp, #0x34
0065f8a8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZNK6glitch7collada16CColladaDatabase21getBlendableAnimationEPKNS0_8SChannelE
0061c1e0: subs     r2, r1, #0
0061c1e4: beq      #0x61c1f4
0061c1e8: ldrb     r3, [r2, #0xc]
0061c1ec: ldmib    r2, {r1, r2}
0061c1f0: b        #0x61c0c8
0061c1f4: mov      r0, r2
0061c1f8: bx       lr

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoItLi30EEEbRKNS_3res6vectorIiEEiRii
0066b130: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b134: sub      sp, sp, #0x24
0066b138: str      r0, [sp, #0x18]
0066b13c: mov      r0, r2
0066b140: str      r2, [sp, #0xc]
0066b144: mov      r4, r1
0066b148: str      r3, [sp, #0x10]
0066b14c: bl       #0x30e964
0066b150: movw     r1, #0x5555
0066b154: movt     r1, #0x4205
0066b158: str      r0, [sp, #0x14]
0066b15c: bl       #0x30ec94
0066b160: ldr      r6, [r4]
0066b164: ldr      r5, [sp, #0x48]
0066b168: ldr      r8, [r4, #4]
0066b16c: sub      r6, r6, #1
0066b170: bic      r5, r5, r5, asr #31
0066b174: cmp      r5, r6
0066b178: movge    r5, r6
0066b17c: lsl      r3, r5, #1
0066b180: str      r3, [sp, #8]
0066b184: mov      r7, r0
0066b188: ldrh     r0, [r8, r3]
0066b18c: bl       #0x30e964
0066b190: mov      r1, r7
0066b194: bl       #0x30e2f8
0066b198: cmp      r0, #0
0066b19c: mov      sl, #0
0066b1a0: movne    sl, #1
0066b1a4: uxtb     sl, sl
0066b1a8: cmp      sl, #0
0066b1ac: beq      #0x66b218
0066b1b0: cmp      r5, #0
0066b1b4: subgt    r5, r5, #1
0066b1b8: lslgt    r3, r5, #1
0066b1bc: ble      #0x66b218
0066b1c0: cmp      r5, r6
0066b1c4: bge      #0x66b320
0066b1c8: ldrh     r0, [r8, r3]
0066b1cc: mov      sb, r3
0066b1d0: bl       #0x30e964
0066b1d4: mov      r1, r0
0066b1d8: mov      r0, r7
0066b1dc: bl       #0x30e70c
0066b1e0: cmp      r0, #0
0066b1e4: mov      sl, #0
0066b1e8: movne    sl, #1
0066b1ec: uxtb     sl, sl
0066b1f0: mov      fp, r5
0066b1f4: cmp      sl, #0
0066b1f8: beq      #0x66b2f4
0066b1fc: ldr      r0, [sp, #0x18]
0066b200: ldr      r2, [sp, #0xc]
0066b204: ldr      r3, [sp, #0x10]
0066b208: mov      r1, r4
0066b20c: add      sp, sp, #0x24
0066b210: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b214: b        #0x66abb8
0066b218: cmp      r6, r5
0066b21c: ble      #0x66b294
0066b220: add      fp, r5, #1
0066b224: lsl      sb, fp, #1
0066b228: ldrh     r0, [r8, sb]
0066b22c: bl       #0x30e964
0066b230: mov      r1, r7
0066b234: str      r0, [sp, #0x1c]
0066b238: bl       #0x30e70c
0066b23c: cmp      r0, #0
0066b240: mov      r3, sb
0066b244: beq      #0x66b314
0066b248: cmp      r6, fp
0066b24c: ble      #0x66b29c
0066b250: add      r5, fp, #1
0066b254: lsl      r3, r5, #1
0066b258: ldrh     r0, [r8, r3]
0066b25c: str      r3, [sp, #4]
0066b260: bl       #0x30e964
0066b264: mov      r1, r7
0066b268: bl       #0x30e70c
0066b26c: subs     sl, r0, #0
0066b270: ldr      r3, [sp, #4]
0066b274: bne      #0x66b1c0
0066b278: ldr      r1, [sp, #0x1c]
0066b27c: mov      r0, r7
0066b280: bl       #0x30e70c
0066b284: cmp      r0, #0
0066b288: movne    sl, #1
0066b28c: uxtb     sl, sl
0066b290: b        #0x66b1f4
0066b294: mov      fp, r5
0066b298: lsl      r3, r5, #1
0066b29c: mov      sb, r3
0066b2a0: mov      r5, fp
0066b2a4: ldr      r3, [sp, #0x10]
0066b2a8: str      r5, [r3]
0066b2ac: ldr      r3, [r4, #4]
0066b2b0: ldrh     r0, [r3, sb]
0066b2b4: bl       #0x30e964
0066b2b8: movw     r1, #0x5555
0066b2bc: movt     r1, #0x4205
0066b2c0: bl       #0x30ed6c
0066b2c4: mov      r1, r0
0066b2c8: ldr      r0, [sp, #0x14]
0066b2cc: bl       #0x30df8c
0066b2d0: cmp      r0, #0
0066b2d4: movne    r0, #0
0066b2d8: bne      #0x66b2ec
0066b2dc: ldr      r0, [r4]
0066b2e0: sub      r0, r0, #1
0066b2e4: subs     r0, r5, r0
0066b2e8: movne    r0, #1
0066b2ec: add      sp, sp, #0x24
0066b2f0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066b2f4: add      r8, r8, fp, lsl #1
0066b2f8: ldrh     r0, [r8, #2]
0066b2fc: bl       #0x30e964
0066b300: mov      r1, r7
0066b304: bl       #0x30e70c
0066b308: cmp      r0, #0
0066b30c: bne      #0x66b1fc
0066b310: b        #0x66b2a0
0066b314: ldr      sb, [sp, #8]
0066b318: mov      fp, r5
0066b31c: b        #0x66b1f4
0066b320: mov      sb, r3
0066b324: b        #0x66b2a4

# _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIiLi1000EEEbiRKNS_3res6vectorIiEEiRiRfi
0066a5e0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a5e4: sub      sp, sp, #0x14
0066a5e8: ldr      r8, [sp, #0x38]
0066a5ec: ldr      ip, [sp, #0x40]
0066a5f0: mov      r5, r2
0066a5f4: mov      r6, r1
0066a5f8: mov      r2, r3
0066a5fc: mov      r1, r5
0066a600: mov      r4, r3
0066a604: mov      r3, r8
0066a608: mov      r7, r0
0066a60c: str      ip, [sp]
0066a610: ldr      sl, [sp, #0x3c]
0066a614: bl       #0x66a368
0066a618: mov      r1, r6
0066a61c: mov      sb, r0
0066a620: mov      r0, r7
0066a624: bl       #0x669e9c
0066a628: cmp      r0, #0
0066a62c: moveq    r6, #0
0066a630: andne    r6, sb, #1
0066a634: cmp      r6, #0
0066a638: beq      #0x66a6ac
0066a63c: ldr      fp, [r8]
0066a640: ldr      sb, [r5, #4]
0066a644: mov      r5, #0
0066a648: mov      r7, #0x3f800000
0066a64c: ldr      r0, [sb, fp, lsl #2]
0066a650: bl       #0x30e964
0066a654: bl       #0x30e4cc
0066a658: mov      r8, r0
0066a65c: rsb      r0, r0, r4
0066a660: bl       #0x30e964
0066a664: add      fp, fp, #1
0066a668: mov      r4, r0
0066a66c: ldr      r0, [sb, fp, lsl #2]
0066a670: bl       #0x30e964
0066a674: bl       #0x30e4cc
0066a678: rsb      r0, r8, r0
0066a67c: bl       #0x30e964
0066a680: mov      r1, r0
0066a684: mov      r0, r4
0066a688: bl       #0x30ec94
0066a68c: mov      r1, r5
0066a690: str      r0, [sl]
0066a694: mov      r4, r0
0066a698: bl       #0x30e70c
0066a69c: cmp      r0, #0
0066a6a0: movne    r4, r5
0066a6a4: beq      #0x66a6b8
0066a6a8: str      r4, [sl]
0066a6ac: mov      r0, r6
0066a6b0: add      sp, sp, #0x14
0066a6b4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a6b8: mov      r0, r4
0066a6bc: mov      r1, r7
0066a6c0: bl       #0x30e70c
0066a6c4: cmp      r0, #0
0066a6c8: moveq    r4, r7
0066a6cc: b        #0x66a6a8

# _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExIhLi30EEEbiRKNS_3res6vectorIiEEiRiRfi
0066aab4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0066aab8: sub      sp, sp, #0x10
0066aabc: ldr      r8, [sp, #0x30]
0066aac0: ldr      ip, [sp, #0x38]
0066aac4: mov      r5, r2
0066aac8: mov      r6, r1
0066aacc: mov      r2, r3
0066aad0: mov      r1, r5
0066aad4: mov      r4, r3
0066aad8: mov      r3, r8
0066aadc: mov      r7, r0
0066aae0: str      ip, [sp]
0066aae4: ldr      sl, [sp, #0x34]
0066aae8: bl       #0x66a894
0066aaec: mov      r1, r6
0066aaf0: mov      sb, r0
0066aaf4: mov      r0, r7
0066aaf8: bl       #0x669e9c
0066aafc: cmp      r0, #0
0066ab00: moveq    r6, #0
0066ab04: andne    r6, sb, #1
0066ab08: cmp      r6, #0
0066ab0c: beq      #0x66ab94
0066ab10: ldr      r3, [r8]
0066ab14: ldr      r5, [r5, #4]
0066ab18: mov      r8, #0
0066ab1c: mov      sb, #0x3f800000
0066ab20: ldrb     r0, [r5, r3]!
0066ab24: bl       #0x30e964
0066ab28: movw     r1, #0x5555
0066ab2c: movt     r1, #0x4205
0066ab30: bl       #0x30ed6c
0066ab34: bl       #0x30e4cc
0066ab38: mov      r7, r0
0066ab3c: rsb      r0, r0, r4
0066ab40: bl       #0x30e964
0066ab44: mov      r4, r0
0066ab48: ldrb     r0, [r5, #1]
0066ab4c: bl       #0x30e964
0066ab50: movw     r1, #0x5555
0066ab54: movt     r1, #0x4205
0066ab58: bl       #0x30ed6c
0066ab5c: bl       #0x30e4cc
0066ab60: rsb      r0, r7, r0
0066ab64: bl       #0x30e964
0066ab68: mov      r1, r0
0066ab6c: mov      r0, r4
0066ab70: bl       #0x30ec94
0066ab74: mov      r1, r8
0066ab78: str      r0, [sl]
0066ab7c: mov      r4, r0
0066ab80: bl       #0x30e70c
0066ab84: cmp      r0, #0
0066ab88: movne    r4, r8
0066ab8c: beq      #0x66aba0
0066ab90: str      r4, [sl]
0066ab94: mov      r0, r6
0066ab98: add      sp, sp, #0x10
0066ab9c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0066aba0: mov      r0, r4
0066aba4: mov      r1, sb
0066aba8: bl       #0x30e70c
0066abac: cmp      r0, #0
0066abb0: moveq    r4, sb
0066abb4: b        #0x66ab90

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiiRiRfi
0066b814: push     {r4, r5, r6, r7, lr}
0066b818: sub      sp, sp, #0x14
0066b81c: mov      r5, r2
0066b820: mov      r4, r3
0066b824: mov      r6, r0
0066b828: mov      r7, r1
0066b82c: bl       #0x669eec
0066b830: ldr      ip, [sp, #0x28]
0066b834: mov      r2, r0
0066b838: mov      r1, r7
0066b83c: str      ip, [sp, #4]
0066b840: ldr      ip, [sp, #0x2c]
0066b844: mov      r0, r6
0066b848: mov      r3, r5
0066b84c: str      r4, [sp]
0066b850: str      ip, [sp, #8]
0066b854: bl       #0x66b65c
0066b858: add      sp, sp, #0x14
0066b85c: pop      {r4, r5, r6, r7, pc}

# _ZNK6glitch7collada18SAnimationAccessor16findKeyFrameNoExItLi30EEEbiRKNS_3res6vectorIiEEiRiRfi
0066b550: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0066b554: sub      sp, sp, #0x10
0066b558: ldr      r8, [sp, #0x30]
0066b55c: ldr      ip, [sp, #0x38]
0066b560: mov      r5, r2
0066b564: mov      r6, r1
0066b568: mov      r2, r3
0066b56c: mov      r1, r5
0066b570: mov      r4, r3
0066b574: mov      r3, r8
0066b578: mov      r7, r0
0066b57c: str      ip, [sp]
0066b580: ldr      sl, [sp, #0x34]
0066b584: bl       #0x66b130
0066b588: mov      r1, r6
0066b58c: mov      sb, r0
0066b590: mov      r0, r7
0066b594: bl       #0x669e9c
0066b598: cmp      r0, #0
0066b59c: moveq    r6, #0
0066b5a0: andne    r6, sb, #1
0066b5a4: cmp      r6, #0
0066b5a8: beq      #0x66b638
0066b5ac: ldr      r8, [r8]
0066b5b0: ldr      r7, [r5, #4]
0066b5b4: mov      sb, #0
0066b5b8: lsl      r3, r8, #1
0066b5bc: ldrh     r0, [r7, r3]
0066b5c0: bl       #0x30e964
0066b5c4: movw     r1, #0x5555
0066b5c8: movt     r1, #0x4205
0066b5cc: bl       #0x30ed6c
0066b5d0: bl       #0x30e4cc
0066b5d4: mov      r5, r0
0066b5d8: rsb      r0, r0, r4
0066b5dc: bl       #0x30e964
0066b5e0: add      r7, r7, r8, lsl #1
0066b5e4: mov      r4, r0
0066b5e8: ldrh     r0, [r7, #2]
0066b5ec: bl       #0x30e964
0066b5f0: movw     r1, #0x5555
0066b5f4: movt     r1, #0x4205
0066b5f8: bl       #0x30ed6c
0066b5fc: bl       #0x30e4cc
0066b600: rsb      r0, r5, r0
0066b604: bl       #0x30e964
0066b608: mov      r1, r0
0066b60c: mov      r0, r4
0066b610: bl       #0x30ec94
0066b614: mov      r1, sb
0066b618: str      r0, [sl]
0066b61c: mov      r4, r0
0066b620: bl       #0x30e70c
0066b624: cmp      r0, #0
0066b628: mov      r5, #0x3f800000
0066b62c: movne    r4, sb
0066b630: beq      #0x66b644
0066b634: str      r4, [sl]
0066b638: mov      r0, r6
0066b63c: add      sp, sp, #0x10
0066b640: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0066b644: mov      r0, r4
0066b648: mov      r1, r5
0066b64c: bl       #0x30e70c
0066b650: cmp      r0, #0
0066b654: moveq    r4, r5
0066b658: b        #0x66b634

# _ZN6glitch7collada21IAnimationSetTemplate10setUnAddedEv
00667320: ldmib    r0, {r2, r3}
00667324: rsb      r3, r2, r3
00667328: lsrs     r3, r3, #2
0066732c: bxeq     lr
00667330: mov      r3, #0
00667334: mov      r1, r3
00667338: ldr      r2, [r2, r3, lsl #2]
0066733c: strb     r1, [r2]
00667340: ldr      r2, [r0, #4]
00667344: ldr      r2, [r2, r3, lsl #2]
00667348: add      r3, r3, #1
0066734c: str      r1, [r2, #0xc]
00667350: ldmib    r0, {r2, ip}
00667354: rsb      ip, r2, ip
00667358: cmp      r3, ip, asr #2
0066735c: blo      #0x667338
00667360: bx       lr

# _ZN6glitch7collada21IAnimationSetTemplate11addChannelsEPSt6vectorIPKNS0_8SChannelENS_4core10SAllocatorIS5_LNS_6memory13E_MEMORY_HINTE0EEEEPS2_IPKNS0_17CAnimationTrackExENS7_ISF_LS9_0EEEE
00667384: push     {r4, r5, r6, r7, r8, lr}
00667388: mov      r5, r0
0066738c: ldr      r3, [r0, #4]
00667390: ldr      r0, [r0, #8]
00667394: rsb      r2, r3, r0
00667398: lsrs     r2, r2, #2
0066739c: beq      #0x667488
006673a0: mov      r4, #0
006673a4: mov      r7, #1
006673a8: b        #0x6673bc
006673ac: add      r4, r4, #1
006673b0: rsb      r2, r3, r0
006673b4: cmp      r4, r2, asr #2
006673b8: bhs      #0x667488
006673bc: ldr      r2, [r3, r4, lsl #2]
006673c0: lsl      r6, r4, #2
006673c4: ldrb     r1, [r2]
006673c8: cmp      r1, #0
006673cc: bne      #0x6673ac
006673d0: mov      r0, #0x10
006673d4: bl       #0x5341ac
006673d8: ldr      r3, [r5, #4]
006673dc: mov      r8, r0
006673e0: ldr      r3, [r3, r4, lsl #2]
006673e4: ldr      r3, [r3, #4]
006673e8: str      r3, [r0, #8]
006673ec: ldr      r3, [r5, #4]
006673f0: ldr      r3, [r3, r4, lsl #2]
006673f4: ldr      r3, [r3, #8]
006673f8: mov      r0, r3
006673fc: ldr      r3, [r3]
00667400: mov      lr, pc
00667404: ldr      pc, [r3, #0x54]
00667408: str      r0, [r8, #4]
0066740c: ldr      r3, [r5, #4]
00667410: ldr      r3, [r3, r4, lsl #2]
00667414: str      r8, [r3, #0xc]
00667418: ldr      r3, [r8, #8]
0066741c: sub      r3, r3, #1
00667420: cmp      r3, #0xc
00667424: addls    pc, pc, r3, lsl #2
00667428: b        #0x667464
0066742c: b        #0x66756c
00667430: b        #0x667550
00667434: b        #0x667534
00667438: b        #0x667518
0066743c: b        #0x6674fc
00667440: b        #0x6674e0
00667444: b        #0x6674e0
00667448: b        #0x6674e0
0066744c: b        #0x6674e0
00667450: b        #0x6674c4
00667454: b        #0x6674a8
00667458: b        #0x66748c
0066745c: b        #0x667460
00667460: bl       #0x610ebc
00667464: ldr      r3, [r5, #4]
00667468: ldr      r3, [r3, r6]
0066746c: strb     r7, [r3]
00667470: ldr      r3, [r5, #4]
00667474: ldr      r0, [r5, #8]
00667478: add      r4, r4, #1
0066747c: rsb      r2, r3, r0
00667480: cmp      r4, r2, asr #2
00667484: blo      #0x6673bc
00667488: pop      {r4, r5, r6, r7, r8, pc}
0066748c: bl       #0x610d00
00667490: ldr      r3, [r5, #4]
00667494: ldr      r3, [r3, r6]
00667498: strb     r7, [r3]
0066749c: ldr      r3, [r5, #4]
006674a0: ldr      r0, [r5, #8]
006674a4: b        #0x667478
006674a8: bl       #0x610b44
006674ac: ldr      r3, [r5, #4]
006674b0: ldr      r3, [r3, r6]
006674b4: strb     r7, [r3]
006674b8: ldr      r3, [r5, #4]
006674bc: ldr      r0, [r5, #8]
006674c0: b        #0x667478
006674c4: bl       #0x610988
006674c8: ldr      r3, [r5, #4]
006674cc: ldr      r3, [r3, r6]
006674d0: strb     r7, [r3]
006674d4: ldr      r3, [r5, #4]
006674d8: ldr      r0, [r5, #8]
006674dc: b        #0x667478
006674e0: bl       #0x6100dc
006674e4: ldr      r3, [r5, #4]
006674e8: ldr      r3, [r3, r6]
006674ec: strb     r7, [r3]
006674f0: ldr      r3, [r5, #4]
006674f4: ldr      r0, [r5, #8]
006674f8: b        #0x667478
006674fc: bl       #0x60ff20
00667500: ldr      r3, [r5, #4]
00667504: ldr      r3, [r3, r6]
00667508: strb     r7, [r3]
0066750c: ldr      r3, [r5, #4]
00667510: ldr      r0, [r5, #8]
00667514: b        #0x667478
00667518: bl       #0x6107cc
0066751c: ldr      r3, [r5, #4]
00667520: ldr      r3, [r3, r6]
00667524: strb     r7, [r3]
00667528: ldr      r3, [r5, #4]
0066752c: ldr      r0, [r5, #8]
00667530: b        #0x667478
00667534: bl       #0x610610
00667538: ldr      r3, [r5, #4]
0066753c: ldr      r3, [r3, r6]
00667540: strb     r7, [r3]
00667544: ldr      r3, [r5, #4]
00667548: ldr      r0, [r5, #8]
0066754c: b        #0x667478
00667550: bl       #0x610454
00667554: ldr      r3, [r5, #4]
00667558: ldr      r3, [r3, r6]
0066755c: strb     r7, [r3]
00667560: ldr      r3, [r5, #4]
00667564: ldr      r0, [r5, #8]
00667568: b        #0x667478
0066756c: bl       #0x610298
00667570: ldr      r3, [r5, #4]
00667574: ldr      r3, [r3, r6]
00667578: strb     r7, [r3]
0066757c: ldr      r3, [r5, #4]
00667580: ldr      r0, [r5, #8]
00667584: b        #0x667478

# _ZNK6glitch7collada35CAnimationSetTransformationTemplate15getDefaultValueEPKNS0_8SChannelEPPv
006e23d0: push     {r4, r5, r6, r7, r8, lr}
006e23d4: ldr      r4, [r0, #4]
006e23d8: ldr      r3, [r0, #8]
006e23dc: mov      r5, r0
006e23e0: mov      r7, r1
006e23e4: cmp      r4, r3
006e23e8: mov      r6, r2
006e23ec: bne      #0x6e2400
006e23f0: b        #0x6e242c
006e23f4: ldr      r3, [r5, #8]
006e23f8: cmp      r4, r3
006e23fc: beq      #0x6e242c
006e2400: ldr      r3, [r4]
006e2404: mov      r1, r7
006e2408: mov      r2, r6
006e240c: ldr      r0, [r3, #8]
006e2410: add      r4, r4, #4
006e2414: add      r0, r0, #0x14c
006e2418: bl       #0x61c6bc
006e241c: cmp      r0, #0
006e2420: beq      #0x6e23f4
006e2424: mov      r0, #1
006e2428: pop      {r4, r5, r6, r7, r8, pc}
006e242c: mov      r0, #0
006e2430: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada6detail27constructCompatibilityTableEv
00670a60: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
00670a64: ldr      r4, [pc, #0x288]
00670a68: add      r4, pc, r4
00670a6c: ldr      r5, [r4]
00670a70: ands     r5, r5, #1
00670a74: beq      #0x670cb0
00670a78: ldr      r6, [pc, #0x278]
00670a7c: mov      r5, #1
00670a80: mov      r7, #0
00670a84: add      r6, pc, r6
00670a88: mov      r0, r6
00670a8c: mov      r8, r5
00670a90: mov      r3, #0
00670a94: add      r4, r7, r7, lsl #1
00670a98: add      r2, r4, r3, lsr #5
00670a9c: and      ip, r3, #0x1f
00670aa0: add      r2, r6, r2, lsl #2
00670aa4: ldr      r1, [r2, #4]
00670aa8: add      r3, r3, #1
00670aac: cmp      r3, #0x5c
00670ab0: bic      r1, r1, r5, lsl ip
00670ab4: str      r1, [r2, #4]
00670ab8: bne      #0x670a98
00670abc: add      r3, r4, r7, lsr #5
00670ac0: and      r1, r7, #0x1f
00670ac4: add      r3, r0, r3, lsl #2
00670ac8: ldr      r2, [r3, #4]
00670acc: add      r7, r7, #1
00670ad0: cmp      r7, #0x5c
00670ad4: orr      r2, r2, r8, lsl r1
00670ad8: str      r2, [r3, #4]
00670adc: bne      #0x670a90
00670ae0: ldr      r7, [r0, #0x450]
00670ae4: ldr      r2, [r0, #0x3d8]
00670ae8: ldr      r6, [r0, #0x420]
00670aec: orr      r7, r7, #0x7800000
00670af0: str      r7, [r0, #0x450]
00670af4: ldr      r7, [r0, #0x10c]
00670af8: orr      r2, r2, #0x3c0000
00670afc: ldr      sb, [r0, #0x42c]
00670b00: orr      r3, r7, #0x3a00000
00670b04: ldr      r7, [r0, #0x118]
00670b08: ldr      r8, [r0, #0x438]
00670b0c: ldr      sl, [r0, #0x444]
00670b10: orr      r7, r7, #0x3600000
00670b14: str      r7, [r0, #0x118]
00670b18: ldr      r7, [r0, #0x130]
00670b1c: ldr      r5, [r0, #0x3e4]
00670b20: ldr      ip, [r0, #0x3f0]
00670b24: orr      r7, r7, #0x1e00000
00670b28: str      r7, [r0, #0x130]
00670b2c: ldr      r7, [r0, #0x100]
00670b30: ldr      r4, [r0, #0x3fc]
00670b34: ldr      r1, [r0, #0x408]
00670b38: orr      r7, r7, #0x3c00000
00670b3c: ldr      fp, [r0, #0x124]
00670b40: str      r7, [r0, #0x100]
00670b44: str      r2, [r0, #0x3d8]
00670b48: ldr      r7, [r0, #0x3a8]
00670b4c: ldr      r2, [r0, #0x7c]
00670b50: orr      sb, sb, #0xe800000
00670b54: orr      sl, sl, #0xb800000
00670b58: orr      r8, r8, #0xd800000
00670b5c: orr      r6, r6, #0xf000000
00670b60: orr      r5, r5, #0x3a0000
00670b64: orr      r4, r4, #0x2e0000
00670b68: orr      ip, ip, #0x360000
00670b6c: orr      r1, r1, #0x1e0000
00670b70: orr      fp, fp, #0x2e00000
00670b74: orr      r7, r7, #0x1d000
00670b78: orr      r2, r2, #0x3800
00670b7c: str      r7, [r0, #0x3a8]
00670b80: str      sb, [r0, #0x42c]
00670b84: str      sl, [r0, #0x444]
00670b88: str      r8, [r0, #0x438]
00670b8c: str      r6, [r0, #0x420]
00670b90: str      r5, [r0, #0x3e4]
00670b94: str      r4, [r0, #0x3fc]
00670b98: str      ip, [r0, #0x3f0]
00670b9c: str      r1, [r0, #0x408]
00670ba0: str      r3, [r0, #0x10c]
00670ba4: str      fp, [r0, #0x124]
00670ba8: ldr      r7, [r0, #0x39c]
00670bac: ldr      sl, [r0, #0x3b4]
00670bb0: ldr      sb, [r0, #0x3c0]
00670bb4: ldr      r8, [r0, #0x3cc]
00670bb8: ldr      r1, [r0, #0x360]
00670bbc: ldr      r6, [r0, #0x36c]
00670bc0: ldr      r4, [r0, #0x378]
00670bc4: ldr      r5, [r0, #0x384]
00670bc8: ldr      ip, [r0, #0x390]
00670bcc: ldr      fp, [r0, #0x88]
00670bd0: str      r2, [r0, #0x7c]
00670bd4: ldr      r2, [r0, #0xa0]
00670bd8: ldr      r3, [r0, #0x94]
00670bdc: orr      sb, sb, #0x17000
00670be0: orr      r2, r2, #0x1c00
00670be4: str      r2, [r0, #0xa0]
00670be8: ldr      r2, [r0, #0x10]
00670bec: orr      r3, r3, #0x2c00
00670bf0: str      r3, [r0, #0x94]
00670bf4: orr      r2, r2, #0x1c
00670bf8: ldr      r3, [r0, #0x1c]
00670bfc: str      r2, [r0, #0x10]
00670c00: ldr      r2, [r0, #0x34]
00670c04: orr      r3, r3, #0x1a
00670c08: str      r3, [r0, #0x1c]
00670c0c: orr      r2, r2, #0xe
00670c10: ldr      r3, [r0, #0x28]
00670c14: str      r2, [r0, #0x34]
00670c18: ldr      r2, [r0, #0x4c]
00670c1c: orr      r3, r3, #0x16
00670c20: str      r3, [r0, #0x28]
00670c24: orr      r2, r2, #0x3a0
00670c28: ldr      r3, [r0, #0x64]
00670c2c: str      r2, [r0, #0x4c]
00670c30: ldr      r2, [r0, #0x58]
00670c34: orr      r3, r3, #0x2e0
00670c38: str      r3, [r0, #0x64]
00670c3c: orr      r2, r2, #0x360
00670c40: ldr      r3, [r0, #0x70]
00670c44: str      r2, [r0, #0x58]
00670c48: ldr      r2, [r0, #0x40]
00670c4c: orr      r3, r3, #0x1e0
00670c50: str      r3, [r0, #0x70]
00670c54: orr      sl, sl, #0x1b000
00670c58: orr      r8, r8, #0xf000
00670c5c: orr      r7, r7, #0x1e000
00670c60: orr      r6, r6, #0xe80
00670c64: orr      r5, r5, #0xb80
00670c68: orr      r4, r4, #0xd80
00670c6c: orr      ip, ip, #0x780
00670c70: orr      r1, r1, #0xf00
00670c74: orr      fp, fp, #0x3400
00670c78: orr      r3, r2, #0x3c0
00670c7c: str      sb, [r0, #0x3c0]
00670c80: str      sl, [r0, #0x3b4]
00670c84: str      r8, [r0, #0x3cc]
00670c88: str      r7, [r0, #0x39c]
00670c8c: str      r6, [r0, #0x36c]
00670c90: str      r5, [r0, #0x384]
00670c94: str      r4, [r0, #0x378]
00670c98: str      ip, [r0, #0x390]
00670c9c: str      r1, [r0, #0x360]
00670ca0: str      fp, [r0, #0x88]
00670ca4: str      r3, [r0, #0x40]
00670ca8: add      r0, r0, #4
00670cac: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
00670cb0: mov      r0, r4
00670cb4: bl       #0x30e76c
00670cb8: cmp      r0, #0
00670cbc: beq      #0x670a78
00670cc0: add      r4, r4, #4
00670cc4: add      r2, r4, #0x450
00670cc8: mov      r3, r4
00670ccc: str      r5, [r3], #4
00670cd0: str      r5, [r4, #4]
00670cd4: add      r4, r4, #0xc
00670cd8: cmp      r4, r2
00670cdc: str      r5, [r3, #4]
00670ce0: bne      #0x670cc8
00670ce4: ldr      r0, [pc, #0x10]
00670ce8: add      r0, pc, r0
00670cec: bl       #0x30ea3c
00670cf0: b        #0x670a78
00670cf4: eorseq   r6, r8, r0, lsr #13
00670cf8: eorseq   r6, r8, r4, lsl #13
00670cfc: eorseq   r6, r8, r0, lsr #8

# _ZNK6glitch7collada16CColladaDatabase7getNodeEPKcRNS0_5SNodeE
0061c214: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c218: mov      r7, r0
0061c21c: ldr      r0, [r2]
0061c220: mov      r4, r2
0061c224: mov      r8, r1
0061c228: bl       #0x30e31c
0061c22c: cmp      r0, #0
0061c230: bne      #0x61c23c
0061c234: mov      r0, r4
0061c238: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c23c: ldr      sl, [r4, #0x38]
0061c240: cmp      sl, #0
0061c244: ble      #0x61c288
0061c248: mov      r5, #0
0061c24c: mov      r6, r5
0061c250: b        #0x61c260
0061c254: cmp      r6, sl
0061c258: add      r5, r5, #0x50
0061c25c: beq      #0x61c288
0061c260: ldr      r2, [r4, #0x3c]
0061c264: mov      r0, r7
0061c268: mov      r1, r8
0061c26c: add      r2, r2, r5
0061c270: bl       #0x61c214
0061c274: cmp      r0, #0
0061c278: add      r6, r6, #1
0061c27c: beq      #0x61c254
0061c280: mov      r4, r0
0061c284: b        #0x61c234
0061c288: mov      r4, #0
0061c28c: b        #0x61c234

# _ZNK6glitch7collada17CAnimationTrackEx8getValueERKNS0_18SAnimationAccessorEiPvRib
006e2c40: push     {r4, r5, r6, r7, r8, lr}
006e2c44: sub      sp, sp, #0x10
006e2c48: ldr      r4, [sp, #0x28]
006e2c4c: mov      lr, #0
006e2c50: add      ip, sp, #0x10
006e2c54: ldr      r7, [r4]
006e2c58: mov      r6, r1
006e2c5c: str      lr, [ip, #-4]!
006e2c60: mov      r5, r0
006e2c64: mov      r8, r3
006e2c68: mov      r1, lr
006e2c6c: mov      r3, ip
006e2c70: mov      r0, r6
006e2c74: add      ip, sp, #8
006e2c78: str      r7, [sp, #4]
006e2c7c: str      ip, [sp]
006e2c80: ldrb     r7, [sp, #0x2c]
006e2c84: bl       #0x66b814
006e2c88: tst      r0, r7
006e2c8c: bne      #0x6e2cbc
006e2c90: mov      r0, r5
006e2c94: mov      r1, r6
006e2c98: mov      r3, r8
006e2c9c: ldr      ip, [r5]
006e2ca0: ldr      r2, [sp, #0xc]
006e2ca4: mov      lr, pc
006e2ca8: ldr      pc, [ip, #0x28]
006e2cac: ldr      r3, [sp, #0xc]
006e2cb0: str      r3, [r4]
006e2cb4: add      sp, sp, #0x10
006e2cb8: pop      {r4, r5, r6, r7, r8, pc}
006e2cbc: ldr      r3, [sp, #8]
006e2cc0: ldr      r2, [sp, #0xc]
006e2cc4: str      r8, [sp, #4]
006e2cc8: str      r3, [sp]
006e2ccc: mov      r0, r5
006e2cd0: mov      r1, r6
006e2cd4: ldr      ip, [r5]
006e2cd8: add      r3, r2, #1
006e2cdc: mov      lr, pc
006e2ce0: ldr      pc, [ip, #0x20]
006e2ce4: b        #0x6e2cac

# _ZN6glitch7collada13CAnimationSet12addAnimationEPKNS0_10SAnimationE
006601fc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00660200: ldr      r3, [r0, #0xc]
00660204: ldr      fp, [r0, #0x10]
00660208: ldr      r2, [pc, #0x28c]
0066020c: sub      sp, sp, #0x14
00660210: rsb      fp, r3, fp
00660214: str      r1, [sp, #0xc]
00660218: asrs     fp, fp, #2
0066021c: add      r2, pc, r2
00660220: mov      r5, r0
00660224: ldr      r4, [r1, #0x10]
00660228: beq      #0x660330
0066022c: ldr      r1, [pc, #0x26c]
00660230: mov      r6, #0
00660234: mov      sl, #0xc
00660238: ldr      sb, [r2, r1]
0066023c: ldr      r2, [pc, #0x260]
00660240: mov      r8, #1
00660244: lsl      r7, r6, #2
00660248: add      r2, pc, r2
0066024c: str      r2, [sp, #8]
00660250: ldr      r1, [r3, r6, lsl #2]
00660254: ldr      r3, [r4, #8]
00660258: ldr      r2, [sb]
0066025c: ldr      r1, [r1, #8]
00660260: cmp      r3, #0x5b
00660264: mla      r2, sl, r1, r2
00660268: bhi      #0x660300
0066026c: lsr      r1, r3, #5
00660270: ldr      r2, [r2, r1, lsl #2]
00660274: and      r3, r3, #0x1f
00660278: ands     r2, r2, r8, lsl r3
0066027c: beq      #0x6602d0
00660280: ldr      r3, [r5, #0xc]
00660284: ldr      r1, [r4, #4]
00660288: ldr      r7, [r3, r7]
0066028c: ldr      r0, [r7, #4]
00660290: bl       #0x30e31c
00660294: cmp      r0, #0
00660298: bne      #0x6602d0
0066029c: ldr      r3, [r4, #8]
006602a0: cmp      r3, #0xe
006602a4: beq      #0x66031c
006602a8: cmp      r3, #0x56
006602ac: beq      #0x6602bc
006602b0: mov      r0, r6
006602b4: add      sp, sp, #0x14
006602b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006602bc: ldr      r0, [r7, #0xc]
006602c0: ldr      r1, [r4, #0xc]
006602c4: bl       #0x30e31c
006602c8: cmp      r0, #0
006602cc: beq      #0x6602b0
006602d0: add      r6, r6, #1
006602d4: cmp      r6, fp
006602d8: beq      #0x660330
006602dc: ldr      r3, [r5, #0xc]
006602e0: ldr      r2, [sb]
006602e4: lsl      r7, r6, #2
006602e8: ldr      r1, [r3, r6, lsl #2]
006602ec: ldr      r3, [r4, #8]
006602f0: ldr      r1, [r1, #8]
006602f4: cmp      r3, #0x5b
006602f8: mla      r2, sl, r1, r2
006602fc: bls      #0x66026c
00660300: ldr      r0, [sp, #8]
00660304: str      r2, [sp, #4]
00660308: str      r3, [sp]
0066030c: bl       #0x708eb0
00660310: ldr      r3, [sp]
00660314: ldr      r2, [sp, #4]
00660318: b        #0x66026c
0066031c: ldrb     r2, [r7, #0xc]
00660320: ldrb     r3, [r4, #0xc]
00660324: cmp      r2, r3
00660328: bne      #0x6602d0
0066032c: b        #0x6602b0
00660330: ldr      r0, [sp, #0xc]
00660334: bl       #0x611ae0
00660338: subs     r6, r0, #0
0066033c: mvneq    r0, #0
00660340: beq      #0x6602b4
00660344: ldr      sl, [r5, #0x10]
00660348: ldr      r3, [r5, #0x14]
0066034c: cmp      sl, r3
00660350: beq      #0x66039c
00660354: str      r4, [sl]
00660358: ldr      r3, [r5, #0x10]
0066035c: add      r3, r3, #4
00660360: str      r3, [r5, #0x10]
00660364: ldr      r8, [r5, #0x1c]
00660368: ldr      r3, [r5, #0x20]
0066036c: cmp      r8, r3
00660370: beq      #0x66041c
00660374: str      r6, [r8]
00660378: ldr      r3, [r5, #0x1c]
0066037c: add      r3, r3, #4
00660380: str      r3, [r5, #0x1c]
00660384: ldr      r3, [r5, #0xc]
00660388: ldr      r0, [r5, #0x10]
0066038c: rsb      r0, r3, r0
00660390: asr      r0, r0, #2
00660394: sub      r0, r0, #1
00660398: b        #0x6602b4
0066039c: ldr      r3, [r5, #0xc]
006603a0: rsb      r3, r3, sl
006603a4: asr      r3, r3, #2
006603a8: cmp      r3, #1
006603ac: addhs    r8, r3, r3
006603b0: addlo    r8, r3, #1
006603b4: cmn      r8, #0xc0000001
006603b8: bhi      #0x660414
006603bc: cmp      r3, r8
006603c0: bhi      #0x660414
006603c4: lsl      r8, r8, #2
006603c8: mov      r1, #0
006603cc: mov      r0, r8
006603d0: bl       #0x310568
006603d4: ldr      r1, [r5, #0xc]
006603d8: mov      r7, r0
006603dc: subs     sl, sl, r1
006603e0: moveq    sl, r0
006603e4: beq      #0x6603f4
006603e8: mov      r2, sl
006603ec: bl       #0x30df38
006603f0: add      sl, r0, sl
006603f4: str      r4, [sl], #4
006603f8: ldr      r0, [r5, #0xc]
006603fc: add      r8, r7, r8
00660400: bl       #0x310450
00660404: str      sl, [r5, #0x10]
00660408: str      r8, [r5, #0x14]
0066040c: str      r7, [r5, #0xc]
00660410: b        #0x660364
00660414: mvn      r8, #0xc0000000
00660418: b        #0x6603c4
0066041c: ldr      r3, [r5, #0x18]
00660420: rsb      r3, r3, r8
00660424: asr      r3, r3, #2
00660428: cmp      r3, #1
0066042c: addhs    r7, r3, r3
00660430: addlo    r7, r3, #1
00660434: cmn      r7, #0xc0000001
00660438: bhi      #0x660494
0066043c: cmp      r3, r7
00660440: bhi      #0x660494
00660444: lsl      r7, r7, #2
00660448: mov      r1, #0
0066044c: mov      r0, r7
00660450: bl       #0x310568
00660454: ldr      r1, [r5, #0x18]
00660458: mov      r4, r0
0066045c: subs     r8, r8, r1
00660460: moveq    r8, r0
00660464: beq      #0x660474
00660468: mov      r2, r8
0066046c: bl       #0x30df38
00660470: add      r8, r0, r8
00660474: str      r6, [r8], #4
00660478: ldr      r0, [r5, #0x18]
0066047c: add      r7, r4, r7
00660480: bl       #0x310450
00660484: str      r8, [r5, #0x1c]
00660488: str      r7, [r5, #0x20]
0066048c: str      r4, [r5, #0x18]
00660490: b        #0x660384
00660494: mvn      r7, #0xc0000000
00660498: b        #0x660444
0066049c: eorseq   r4, r3, r4, ror r8
006604a0: andeq    r4, r0, ip, asr #10
006604a4: eoreq    r1, r6, r0, lsl #21

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoIhLi30EEEbRKNS_3res6vectorIiEEiRii
0066a894: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a898: sub      sp, sp, #0x1c
0066a89c: str      r0, [sp, #0x10]
0066a8a0: mov      r0, r2
0066a8a4: str      r2, [sp, #4]
0066a8a8: mov      r4, r1
0066a8ac: str      r3, [sp, #8]
0066a8b0: bl       #0x30e964
0066a8b4: movw     r1, #0x5555
0066a8b8: movt     r1, #0x4205
0066a8bc: str      r0, [sp, #0xc]
0066a8c0: bl       #0x30ec94
0066a8c4: ldr      r6, [r4]
0066a8c8: ldr      r5, [sp, #0x40]
0066a8cc: ldr      r8, [r4, #4]
0066a8d0: sub      r6, r6, #1
0066a8d4: bic      r5, r5, r5, asr #31
0066a8d8: cmp      r5, r6
0066a8dc: movge    r5, r6
0066a8e0: mov      r7, r0
0066a8e4: ldrb     r0, [r8, r5]
0066a8e8: bl       #0x30e964
0066a8ec: mov      r1, r7
0066a8f0: bl       #0x30e2f8
0066a8f4: cmp      r0, #0
0066a8f8: mov      sl, #0
0066a8fc: movne    sl, #1
0066a900: uxtb     sl, sl
0066a904: cmp      sl, #0
0066a908: beq      #0x66a96c
0066a90c: cmp      r5, #0
0066a910: subgt    r5, r5, #1
0066a914: ble      #0x66a96c
0066a918: cmp      r5, r6
0066a91c: bge      #0x66aa60
0066a920: ldrb     r0, [r8, r5]
0066a924: bl       #0x30e964
0066a928: mov      r1, r0
0066a92c: mov      r0, r7
0066a930: bl       #0x30e70c
0066a934: cmp      r0, #0
0066a938: mov      sl, #0
0066a93c: movne    sl, #1
0066a940: mov      fp, r5
0066a944: uxtb     sl, sl
0066a948: mov      sb, r5
0066a94c: cmp      sl, #0
0066a950: beq      #0x66aa3c
0066a954: ldr      r0, [sp, #0x10]
0066a958: ldmib    sp, {r2, r3}
0066a95c: mov      r1, r4
0066a960: add      sp, sp, #0x1c
0066a964: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066a968: b        #0x66a6d0
0066a96c: cmp      r6, r5
0066a970: bgt      #0x66a9d0
0066a974: mov      sb, r5
0066a978: mov      fp, sb
0066a97c: mov      r5, sb
0066a980: ldr      r3, [sp, #8]
0066a984: str      r5, [r3]
0066a988: ldr      r3, [r4, #4]
0066a98c: ldrb     r0, [r3, fp]
0066a990: bl       #0x30e964
0066a994: movw     r1, #0x5555
0066a998: movt     r1, #0x4205
0066a99c: bl       #0x30ed6c
0066a9a0: mov      r1, r0
0066a9a4: ldr      r0, [sp, #0xc]
0066a9a8: bl       #0x30df8c
0066a9ac: cmp      r0, #0
0066a9b0: movne    r0, #0
0066a9b4: bne      #0x66a9c8
0066a9b8: ldr      r0, [r4]
0066a9bc: sub      r0, r0, #1
0066a9c0: subs     r0, r5, r0
0066a9c4: movne    r0, #1
0066a9c8: add      sp, sp, #0x1c
0066a9cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066a9d0: add      sb, r5, #1
0066a9d4: ldrb     r0, [r8, sb]
0066a9d8: bl       #0x30e964
0066a9dc: mov      r1, r7
0066a9e0: str      r0, [sp, #0x14]
0066a9e4: bl       #0x30e70c
0066a9e8: cmp      r0, #0
0066a9ec: mov      fp, sb
0066a9f0: moveq    fp, r5
0066a9f4: moveq    sb, fp
0066a9f8: beq      #0x66a94c
0066a9fc: cmp      r6, sb
0066aa00: ble      #0x66a978
0066aa04: add      r5, sb, #1
0066aa08: ldrb     r0, [r8, r5]
0066aa0c: bl       #0x30e964
0066aa10: mov      r1, r7
0066aa14: bl       #0x30e70c
0066aa18: subs     sl, r0, #0
0066aa1c: bne      #0x66a918
0066aa20: ldr      r1, [sp, #0x14]
0066aa24: mov      r0, r7
0066aa28: bl       #0x30e70c
0066aa2c: cmp      r0, #0
0066aa30: movne    sl, #1
0066aa34: uxtb     sl, sl
0066aa38: b        #0x66a94c
0066aa3c: add      r8, r8, sb
0066aa40: ldrb     r0, [r8, #1]
0066aa44: bl       #0x30e964
0066aa48: mov      r1, r7
0066aa4c: bl       #0x30e70c
0066aa50: cmp      r0, #0
0066aa54: moveq    r5, sb
0066aa58: beq      #0x66a980
0066aa5c: b        #0x66a954
0066aa60: mov      fp, r5
0066aa64: b        #0x66a980

# _ZNK6glitch7collada16CColladaDatabase7getNodeEPKc
0061c290: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c294: mov      r8, r1
0061c298: mov      r1, #0
0061c29c: mov      r7, r0
0061c2a0: bl       #0x60e54c
0061c2a4: subs     r6, r0, #0
0061c2a8: bne      #0x61c2b4
0061c2ac: mov      r0, #0
0061c2b0: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c2b4: ldr      sl, [r6, #8]
0061c2b8: cmp      sl, #0
0061c2bc: ble      #0x61c2ac
0061c2c0: mov      r4, #0
0061c2c4: mov      r5, r4
0061c2c8: ldr      r2, [r6, #0xc]
0061c2cc: mov      r0, r7
0061c2d0: mov      r1, r8
0061c2d4: add      r2, r2, r4
0061c2d8: bl       #0x61c214
0061c2dc: cmp      r0, #0
0061c2e0: add      r5, r5, #1
0061c2e4: bne      #0x61c2b0
0061c2e8: cmp      r5, sl
0061c2ec: add      r4, r4, #0x50
0061c2f0: bne      #0x61c2c8
0061c2f4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada18SAnimationAccessor14findKeyFrameNoEiRKNS_3res6vectorIiEEiRiRfi
0066b65c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b660: ldr      ip, [r0, #8]
0066b664: sub      sp, sp, #0x14
0066b668: mov      r4, r0
0066b66c: ldrb     r8, [ip, #0xd]
0066b670: mov      sb, r1
0066b674: mov      sl, r2
0066b678: cmp      r8, #0
0066b67c: mov      r7, r3
0066b680: ldr      r6, [sp, #0x38]
0066b684: ldr      r5, [sp, #0x3c]
0066b688: ldr      fp, [sp, #0x40]
0066b68c: beq      #0x66b6e8
0066b690: ldr      r3, [ip, #4]
0066b694: cmp      r3, r7
0066b698: beq      #0x66b6c4
0066b69c: str      r7, [ip, #4]
0066b6a0: mov      r1, #0
0066b6a4: bl       #0x669e84
0066b6a8: cmp      r0, #3
0066b6ac: beq      #0x66b7bc
0066b6b0: cmp      r0, #4
0066b6b4: beq      #0x66b78c
0066b6b8: cmp      r0, #1
0066b6bc: beq      #0x66b734
0066b6c0: ldr      ip, [r4, #8]
0066b6c4: ldr      r3, [ip]
0066b6c8: str      r3, [r5]
0066b6cc: ldr      r3, [r4, #8]
0066b6d0: ldr      r3, [r3, #8]
0066b6d4: str      r3, [r6]
0066b6d8: ldr      r3, [r4, #8]
0066b6dc: ldrb     r0, [r3, #0xc]
0066b6e0: add      sp, sp, #0x14
0066b6e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066b6e8: mov      r1, r8
0066b6ec: bl       #0x669e84
0066b6f0: cmp      r0, #3
0066b6f4: beq      #0x66b7ec
0066b6f8: cmp      r0, #4
0066b6fc: beq      #0x66b764
0066b700: cmp      r0, #1
0066b704: movne    r0, r8
0066b708: bne      #0x66b6e0
0066b70c: mov      r0, r4
0066b710: mov      r1, sb
0066b714: mov      r2, sl
0066b718: mov      r3, r7
0066b71c: str      r6, [sp, #0x38]
0066b720: str      r5, [sp, #0x3c]
0066b724: str      fp, [sp, #0x40]
0066b728: add      sp, sp, #0x14
0066b72c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b730: b        #0x66aab4
0066b734: ldr      r8, [r4, #8]
0066b738: mov      r1, sb
0066b73c: mov      r2, sl
0066b740: add      ip, r8, #8
0066b744: mov      r3, r7
0066b748: mov      r0, r4
0066b74c: str      ip, [sp]
0066b750: stmib    sp, {r8, fp}
0066b754: bl       #0x66aab4
0066b758: strb     r0, [r8, #0xc]
0066b75c: ldr      ip, [r4, #8]
0066b760: b        #0x66b6c4
0066b764: mov      r0, r4
0066b768: mov      r1, sb
0066b76c: mov      r2, sl
0066b770: mov      r3, r7
0066b774: str      r6, [sp, #0x38]
0066b778: str      r5, [sp, #0x3c]
0066b77c: str      fp, [sp, #0x40]
0066b780: add      sp, sp, #0x14
0066b784: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b788: b        #0x66a5e0
0066b78c: ldr      r8, [r4, #8]
0066b790: mov      r1, sb
0066b794: mov      r2, sl
0066b798: add      ip, r8, #8
0066b79c: mov      r3, r7
0066b7a0: mov      r0, r4
0066b7a4: str      ip, [sp]
0066b7a8: stmib    sp, {r8, fp}
0066b7ac: bl       #0x66a5e0
0066b7b0: strb     r0, [r8, #0xc]
0066b7b4: ldr      ip, [r4, #8]
0066b7b8: b        #0x66b6c4
0066b7bc: ldr      r8, [r4, #8]
0066b7c0: mov      r1, sb
0066b7c4: mov      r2, sl
0066b7c8: add      ip, r8, #8
0066b7cc: mov      r3, r7
0066b7d0: mov      r0, r4
0066b7d4: str      ip, [sp]
0066b7d8: stmib    sp, {r8, fp}
0066b7dc: bl       #0x66b550
0066b7e0: strb     r0, [r8, #0xc]
0066b7e4: ldr      ip, [r4, #8]
0066b7e8: b        #0x66b6c4
0066b7ec: mov      r0, r4
0066b7f0: mov      r1, sb
0066b7f4: mov      r2, sl
0066b7f8: mov      r3, r7
0066b7fc: str      r6, [sp, #0x38]
0066b800: str      r5, [sp, #0x3c]
0066b804: str      fp, [sp, #0x40]
0066b808: add      sp, sp, #0x14
0066b80c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066b810: b        #0x66b550

# _ZNK6glitch7collada16CColladaDatabase15getDefaultValueEPKcNS0_8SChannel4TypeEPPvS6_
0061c2f8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0061c2fc: ldr      r4, [pc, #0x3a4]
0061c300: ldr      r5, [pc, #0x3a4]
0061c304: mov      r7, r3
0061c308: add      r4, pc, r4
0061c30c: ldr      ip, [r4, r5]
0061c310: sub      sp, sp, #0x7c
0061c314: mov      r6, r1
0061c318: ldr      r3, [ip]
0061c31c: ldr      r8, [sp, #0xa0]
0061c320: str      r3, [sp, #0x74]
0061c324: cmp      r2, #0x5b
0061c328: addls    pc, pc, r2, lsl #2
0061c32c: b        #0x61c5d4
0061c330: b        #0x61c5e0
0061c334: b        #0x61c660
0061c338: b        #0x61c660
0061c33c: b        #0x61c660
0061c340: b        #0x61c660
0061c344: b        #0x61c670
0061c348: b        #0x61c670
0061c34c: b        #0x61c670
0061c350: b        #0x61c670
0061c354: b        #0x61c670
0061c358: b        #0x61c548
0061c35c: b        #0x61c548
0061c360: b        #0x61c5e8
0061c364: b        #0x61c600
0061c368: b        #0x61c61c
0061c36c: b        #0x61c524
0061c370: b        #0x61c564
0061c374: b        #0x61c524
0061c378: b        #0x61c524
0061c37c: b        #0x61c524
0061c380: b        #0x61c644
0061c384: b        #0x61c524
0061c388: b        #0x61c524
0061c38c: b        #0x61c524
0061c390: b        #0x61c524
0061c394: b        #0x61c524
0061c398: b        #0x61c524
0061c39c: b        #0x61c5d4
0061c3a0: b        #0x61c5d4
0061c3a4: b        #0x61c5d4
0061c3a8: b        #0x61c5d4
0061c3ac: b        #0x61c5d4
0061c3b0: b        #0x61c5d4
0061c3b4: b        #0x61c5d4
0061c3b8: b        #0x61c5d4
0061c3bc: b        #0x61c5d4
0061c3c0: b        #0x61c5d4
0061c3c4: b        #0x61c5d4
0061c3c8: b        #0x61c5d4
0061c3cc: b        #0x61c5d4
0061c3d0: b        #0x61c5d4
0061c3d4: b        #0x61c5d4
0061c3d8: b        #0x61c5d4
0061c3dc: b        #0x61c5d4
0061c3e0: b        #0x61c5d4
0061c3e4: b        #0x61c5d4
0061c3e8: b        #0x61c5d4
0061c3ec: b        #0x61c5d4
0061c3f0: b        #0x61c5d4
0061c3f4: b        #0x61c5d4
0061c3f8: b        #0x61c5d4
0061c3fc: b        #0x61c5d4
0061c400: b        #0x61c5d4
0061c404: b        #0x61c5d4
0061c408: b        #0x61c5d4
0061c40c: b        #0x61c5d4
0061c410: b        #0x61c5d4
0061c414: b        #0x61c5d4
0061c418: b        #0x61c5d4
0061c41c: b        #0x61c5d4
0061c420: b        #0x61c5d4
0061c424: b        #0x61c5d4
0061c428: b        #0x61c5d4
0061c42c: b        #0x61c5d4
0061c430: b        #0x61c5d4
0061c434: b        #0x61c5d4
0061c438: b        #0x61c5d4
0061c43c: b        #0x61c5d4
0061c440: b        #0x61c5d4
0061c444: b        #0x61c5d4
0061c448: b        #0x61c5d4
0061c44c: b        #0x61c524
0061c450: b        #0x61c524
0061c454: b        #0x61c524
0061c458: b        #0x61c524
0061c45c: b        #0x61c524
0061c460: b        #0x61c524
0061c464: b        #0x61c524
0061c468: b        #0x61c524
0061c46c: b        #0x61c524
0061c470: b        #0x61c524
0061c474: b        #0x61c524
0061c478: b        #0x61c524
0061c47c: b        #0x61c524
0061c480: b        #0x61c524
0061c484: b        #0x61c524
0061c488: b        #0x61c580
0061c48c: b        #0x61c524
0061c490: b        #0x61c524
0061c494: b        #0x61c524
0061c498: b        #0x61c524
0061c49c: b        #0x61c524
0061c4a0: ldr      r1, [pc, #0x208]
0061c4a4: add      sb, sp, #0x5c
0061c4a8: add      r8, sp, #0x44
0061c4ac: add      r1, pc, r1
0061c4b0: add      r2, sp, #0x10
0061c4b4: mov      r0, sb
0061c4b8: bl       #0x32603c
0061c4bc: mov      r2, r6
0061c4c0: mov      r0, r8
0061c4c4: mov      r1, sb
0061c4c8: bl       #0x56d8b0
0061c4cc: ldr      r1, [pc, #0x1e0]
0061c4d0: add      r6, sp, #0x2c
0061c4d4: add      r2, sp, #0xc
0061c4d8: add      r1, pc, r1
0061c4dc: add      sl, sp, #0x14
0061c4e0: mov      r0, r6
0061c4e4: bl       #0x32603c
0061c4e8: mov      r2, r6
0061c4ec: mov      r0, sl
0061c4f0: mov      r1, r8
0061c4f4: bl       #0x34e324
0061c4f8: mov      r1, #2
0061c4fc: ldr      r0, [sp, #0x28]
0061c500: bl       #0x60aca0
0061c504: mov      r0, sl
0061c508: bl       #0x60f494
0061c50c: mov      r0, r6
0061c510: bl       #0x60f494
0061c514: mov      r0, r8
0061c518: bl       #0x60f494
0061c51c: mov      r0, sb
0061c520: bl       #0x60f494
0061c524: mov      r0, #0
0061c528: str      r0, [r7]
0061c52c: ldr      r3, [r4, r5]
0061c530: ldr      r2, [sp, #0x74]
0061c534: ldr      r3, [r3]
0061c538: cmp      r2, r3
0061c53c: bne      #0x61c6a4
0061c540: add      sp, sp, #0x7c
0061c544: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0061c548: bl       #0x61c290
0061c54c: cmp      r0, #0
0061c550: addne    r0, r0, #0x28
0061c554: strne    r0, [r7]
0061c558: movne    r0, #1
0061c55c: bne      #0x61c52c
0061c560: b        #0x61c524
0061c564: bl       #0x61b1ec
0061c568: cmp      r0, #0
0061c56c: beq      #0x61c524
0061c570: add      r0, r0, #0xc
0061c574: str      r0, [r7]
0061c578: mov      r0, #1
0061c57c: b        #0x61c52c
0061c580: bl       #0x61ac28
0061c584: subs     sb, r0, #0
0061c588: beq      #0x61c4a0
0061c58c: ldr      fp, [sb, #0x10]
0061c590: cmp      fp, #0
0061c594: ble      #0x61c524
0061c598: ldr      r3, [pc, #0x118]
0061c59c: mov      r6, #0
0061c5a0: mov      sl, r6
0061c5a4: add      r3, pc, r3
0061c5a8: ldr      r2, [sb, #0x14]
0061c5ac: ldr      r1, [r8]
0061c5b0: add      r2, r2, r6
0061c5b4: ldr      r2, [r2, #4]
0061c5b8: cmp      r1, r2
0061c5bc: beq      #0x61c68c
0061c5c0: add      sl, sl, #1
0061c5c4: cmp      sl, fp
0061c5c8: add      r6, r6, #0x18
0061c5cc: bne      #0x61c5a8
0061c5d0: b        #0x61c524
0061c5d4: mov      r3, #0
0061c5d8: str      r3, [r7]
0061c5dc: b        #0x61c524
0061c5e0: bl       #0x61c290
0061c5e4: b        #0x61c524
0061c5e8: bl       #0x61c290
0061c5ec: cmp      r0, #0
0061c5f0: addne    r0, r0, #0x2c
0061c5f4: strne    r0, [r7]
0061c5f8: movne    r0, #1
0061c5fc: b        #0x61c52c
0061c600: bl       #0x61c290
0061c604: cmp      r0, #0
0061c608: addne    r0, r0, #0x30
0061c60c: strne    r0, [r7]
0061c610: movne    r0, #1
0061c614: bne      #0x61c52c
0061c618: b        #0x61c524
0061c61c: bl       #0x61a9a0
0061c620: cmp      r0, #0
0061c624: beq      #0x61c524
0061c628: ldr      r3, [r0, #8]
0061c62c: ldrb     r2, [r8]
0061c630: mov      r0, #1
0061c634: ldr      r3, [r3, #0x1c]
0061c638: add      r3, r3, r2, lsl #2
0061c63c: str      r3, [r7]
0061c640: b        #0x61c52c
0061c644: bl       #0x61c290
0061c648: cmp      r0, #0
0061c64c: addne    r0, r0, #0x34
0061c650: strne    r0, [r7]
0061c654: movne    r0, #1
0061c658: bne      #0x61c52c
0061c65c: b        #0x61c524
0061c660: bl       #0x61c290
0061c664: cmp      r0, #0
0061c668: bne      #0x61c570
0061c66c: b        #0x61c524
0061c670: bl       #0x61c290
0061c674: cmp      r0, #0
0061c678: addne    r0, r0, #0x18
0061c67c: strne    r0, [r7]
0061c680: movne    r0, #1
0061c684: bne      #0x61c52c
0061c688: b        #0x61c524
0061c68c: mov      r0, r3
0061c690: mov      r1, #1
0061c694: str      r3, [sp, #4]
0061c698: bl       #0x60aca0
0061c69c: ldr      r3, [sp, #4]
0061c6a0: b        #0x61c5c0
0061c6a4: bl       #0x30e310
0061c6a8: eorseq   r8, r7, r8, lsl #15
0061c6ac: andeq    r4, r0, ip, lsr #1
0061c6b0: eoreq    r8, ip, r4, lsl #18
0061c6b4: eoreq    r8, ip, r8, ror #17
0061c6b8: eoreq    r8, ip, ip, lsr #16

# _ZNK6glitch7collada16CColladaDatabase12getAnimationEPKcNS0_8SChannel4TypeEh
0061c0c8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061c0cc: mov      r4, r0
0061c0d0: ldr      r0, [r0]
0061c0d4: mov      r6, r2
0061c0d8: mov      sb, r3
0061c0dc: ldr      r2, [r0, #0x24]
0061c0e0: mov      r5, r1
0061c0e4: ldr      r3, [r2, #0x20]
0061c0e8: ldr      sl, [r3, #0x24]
0061c0ec: cmp      sl, #0
0061c0f0: ble      #0x61c1ac
0061c0f4: mov      r7, #0
0061c0f8: b        #0x61c12c
0061c0fc: cmp      r6, #1
0061c100: blo      #0x61c114
0061c104: cmp      r6, #4
0061c108: bls      #0x61c190
0061c10c: cmp      r6, #5
0061c110: beq      #0x61c1b8
0061c114: ldr      r2, [r3, #8]
0061c118: cmp      r2, r6
0061c11c: beq      #0x61c1d0
0061c120: add      r7, r7, #1
0061c124: cmp      r7, sl
0061c128: beq      #0x61c1ac
0061c12c: mov      r0, r4
0061c130: mov      r1, r7
0061c134: bl       #0x60e35c
0061c138: cmp      r6, #9
0061c13c: mov      r8, r0
0061c140: ldr      r3, [r0, #0x10]
0061c144: beq      #0x61c1b8
0061c148: bls      #0x61c0fc
0061c14c: cmp      r6, #0x57
0061c150: blo      #0x61c114
0061c154: cmp      r6, #0x5b
0061c158: bls      #0x61c164
0061c15c: cmp      r6, #0x100
0061c160: bne      #0x61c114
0061c164: ldr      r2, [r3, #8]
0061c168: sub      r2, r2, #0x57
0061c16c: cmp      r2, #4
0061c170: bhi      #0x61c120
0061c174: ldr      r0, [r3, #4]
0061c178: mov      r1, r5
0061c17c: bl       #0x30e31c
0061c180: cmp      r0, #0
0061c184: bne      #0x61c120
0061c188: mov      r0, r8
0061c18c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c190: ldr      r2, [r3, #8]
0061c194: sub      r2, r2, #1
0061c198: cmp      r2, #3
0061c19c: bls      #0x61c174
0061c1a0: add      r7, r7, #1
0061c1a4: cmp      r7, sl
0061c1a8: bne      #0x61c12c
0061c1ac: mov      r8, #0
0061c1b0: mov      r0, r8
0061c1b4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061c1b8: ldr      r2, [r3, #8]
0061c1bc: cmp      r2, #5
0061c1c0: beq      #0x61c174
0061c1c4: cmp      r2, #9
0061c1c8: bne      #0x61c120
0061c1cc: b        #0x61c174
0061c1d0: ldrb     r2, [r3, #0xc]
0061c1d4: cmp      r2, sb
0061c1d8: bne      #0x61c120
0061c1dc: b        #0x61c174

# _ZN6glitch7collada35CAnimationSetTransformationTemplate16isAnimationExistEPKNS0_8SChannelE
006e22cc: push     {r4, r5, r6, r7, r8, lr}
006e22d0: ldr      r3, [r0, #4]
006e22d4: ldr      r2, [r0, #8]
006e22d8: mov      r5, r0
006e22dc: mov      r6, r1
006e22e0: rsb      r2, r3, r2
006e22e4: lsrs     r2, r2, #2
006e22e8: beq      #0x6e2394
006e22ec: mov      r4, #0
006e22f0: mov      r8, #1
006e22f4: b        #0x6e234c
006e22f8: ldr      r3, [r6, #8]
006e22fc: cmp      r3, #0xd
006e2300: bhi      #0x6e237c
006e2304: lsl      r3, r8, r3
006e2308: tst      r3, #0x3c00
006e230c: mov      r0, #1
006e2310: bne      #0x6e23b8
006e2314: tst      r3, #0x3e0
006e2318: bne      #0x6e239c
006e231c: tst      r3, #0x1e
006e2320: beq      #0x6e237c
006e2324: ldr      r3, [r5, #4]
006e2328: ldr      r2, [r3, r7]
006e232c: ldr      r0, [r2, #4]
006e2330: cmp      r0, #1
006e2334: beq      #0x6e23b0
006e2338: ldr      r2, [r5, #8]
006e233c: add      r4, r4, #1
006e2340: rsb      r2, r3, r2
006e2344: cmp      r4, r2, asr #2
006e2348: bhs      #0x6e2394
006e234c: ldr      r3, [r3, r4, lsl #2]
006e2350: lsl      r7, r4, #2
006e2354: ldr      r3, [r3, #8]
006e2358: mov      r0, r3
006e235c: ldr      r3, [r3]
006e2360: mov      lr, pc
006e2364: ldr      pc, [r3, #0x54]
006e2368: mov      r1, r0
006e236c: ldr      r0, [r6, #4]
006e2370: bl       #0x30e31c
006e2374: cmp      r0, #0
006e2378: beq      #0x6e22f8
006e237c: ldr      r3, [r5, #4]
006e2380: ldr      r2, [r5, #8]
006e2384: add      r4, r4, #1
006e2388: rsb      r2, r3, r2
006e238c: cmp      r4, r2, asr #2
006e2390: blo      #0x6e234c
006e2394: mov      r0, #0
006e2398: pop      {r4, r5, r6, r7, r8, pc}
006e239c: ldr      r3, [r5, #4]
006e23a0: ldr      r2, [r3, r7]
006e23a4: ldr      r1, [r2, #4]
006e23a8: cmp      r1, #5
006e23ac: bne      #0x6e2338
006e23b0: strb     r0, [r2]
006e23b4: pop      {r4, r5, r6, r7, r8, pc}
006e23b8: ldr      r3, [r5, #4]
006e23bc: ldr      r2, [r3, r7]
006e23c0: ldr      r1, [r2, #4]
006e23c4: cmp      r1, #0xa
006e23c8: bne      #0x6e2338
006e23cc: b        #0x6e23b0
