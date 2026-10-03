
# _ZN6Random9GetRandomEib.clone.1
003ca708: push     {r4, lr}
003ca70c: ldr      r4, [pc, #0x7c]
003ca710: cmp      r0, #0
003ca714: add      r4, pc, r4
003ca718: beq      #0x3ca778
003ca71c: ldr      r2, [pc, #0x70]
003ca720: mov      r1, r0
003ca724: movw     r0, #0xe6ab
003ca728: ldr      r2, [r4, r2]
003ca72c: movw     r3, #0xdb17
003ca730: movt     r3, #0x2b52
003ca734: ldr      lr, [r2]
003ca738: movw     ip, #0xf26b
003ca73c: movt     ip, #0xda
003ca740: mul      r0, r0, lr
003ca744: add      r0, r0, #0x2b000
003ca748: add      r0, r0, #0x3fc
003ca74c: add      r0, r0, #1
003ca750: umull    lr, r3, r3, r0
003ca754: rsb      lr, r3, r0
003ca758: add      r3, r3, lr, lsr #1
003ca75c: lsr      r3, r3, #0x17
003ca760: mls      r3, ip, r3, r0
003ca764: mov      r0, r3
003ca768: str      r3, [r2]
003ca76c: bl       #0x30eb2c
003ca770: eor      r0, r1, r1, asr #31
003ca774: sub      r0, r0, r1, asr #31
003ca778: ldr      r3, [pc, #0x18]
003ca77c: ldr      r3, [r4, r3]
003ca780: ldr      r2, [r3]
003ca784: add      r2, r2, #1
003ca788: str      r2, [r3]
003ca78c: pop      {r4, pc}
003ca790: subseq   sl, ip, ip, ror r3
003ca794: muleq    r0, r4, ip
003ca798: andeq    r1, r0, r8, lsl #1

# _ZN12CharAnimator6UpdateEv
003caf3c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003caf40: ldr      r5, [pc, #0x37c]
003caf44: ldr      r6, [pc, #0x37c]
003caf48: mov      r4, r0
003caf4c: add      r5, pc, r5
003caf50: ldr      r3, [r5, r6]
003caf54: ldr      r0, [pc, #0x370]
003caf58: sub      sp, sp, #0x78
003caf5c: ldr      r3, [r3]
003caf60: add      r0, pc, r0
003caf64: str      r3, [sp, #0x74]
003caf68: bl       #0x3136b4
003caf6c: ldr      r3, [r4, #4]
003caf70: ldr      r3, [r3, #0x520]
003caf74: tst      r3, #0x200
003caf78: bne      #0x3cafb8
003caf7c: ldrb     r3, [r4, #0x5c]
003caf80: cmp      r3, #0
003caf84: bne      #0x3cb128
003caf88: ldr      r0, [pc, #0x340]
003caf8c: mov      r3, #0
003caf90: strb     r3, [r4, #0x5c]
003caf94: add      r0, pc, r0
003caf98: bl       #0x3136b8
003caf9c: ldr      r3, [r5, r6]
003cafa0: ldr      r2, [sp, #0x74]
003cafa4: ldr      r3, [r3]
003cafa8: cmp      r2, r3
003cafac: bne      #0x3cb2c0
003cafb0: add      sp, sp, #0x78
003cafb4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003cafb8: ldrb     r3, [r4, #0x5c]
003cafbc: cmp      r3, #0
003cafc0: beq      #0x3cb11c
003cafc4: ldrb     r3, [r4, #0x4a]
003cafc8: mov      r2, #1
003cafcc: strb     r2, [r4, #0x5c]
003cafd0: cmp      r3, #0
003cafd4: movne    r3, #0
003cafd8: strbne   r3, [r4, #0x4a]
003cafdc: strbne   r2, [r4, #0x49]
003cafe0: beq      #0x3cb134
003cafe4: ldr      sl, [r4, #0x2c]
003cafe8: ldr      r3, [pc, #0x2e4]
003cafec: mov      r8, #0xc
003caff0: mla      r8, r8, sl, r4
003caff4: ldr      r3, [r5, r3]
003caff8: ldr      r2, [r8, #8]
003caffc: mov      r7, #0x14
003cb000: ldr      r3, [r3]
003cb004: ldr      r0, [r4, #4]
003cb008: mov      r1, #0x27
003cb00c: mla      r7, r7, r2, r3
003cb010: mov      r2, #0
003cb014: bl       #0x3a4d5c
003cb018: ldr      r3, [r7, #0x10]
003cb01c: cmp      r3, #1
003cb020: bne      #0x3cb144
003cb024: ldr      r3, [r8, #0x10]
003cb028: ldr      r2, [r7, #8]
003cb02c: add      r3, r3, #1
003cb030: cmp      r3, r2
003cb034: beq      #0x3cb144
003cb038: mov      r8, #0xc
003cb03c: mla      r8, r8, sl, r4
003cb040: str      r3, [r8, #0x10]
003cb044: ldr      r2, [r7, #8]
003cb048: cmp      r2, r3
003cb04c: bhi      #0x3cb1e4
003cb050: mov      r2, #0xc
003cb054: mla      r2, r2, sl, r4
003cb058: ldr      r3, [r2, #0xc]
003cb05c: cmp      r3, #0
003cb060: beq      #0x3cb174
003cb064: subgt    r3, r3, #1
003cb068: strgt    r3, [r2, #0xc]
003cb06c: ldr      r3, [pc, #0x264]
003cb070: add      r7, sp, #0x44
003cb074: ldr      r8, [r5, r3]
003cb078: mov      r0, r8
003cb07c: bl       #0x337888
003cb080: ldr      r1, [pc, #0x254]
003cb084: add      r2, sp, #0xc
003cb088: mov      r0, r7
003cb08c: add      r1, pc, r1
003cb090: bl       #0x3140ec
003cb094: mov      r1, r7
003cb098: mov      r0, r8
003cb09c: bl       #0x337a88
003cb0a0: mov      r0, r7
003cb0a4: bl       #0x3139ac
003cb0a8: mov      r2, #0
003cb0ac: ldr      r0, [r4, #4]
003cb0b0: mov      r1, #0x23
003cb0b4: bl       #0x3a4d5c
003cb0b8: ldr      r2, [r4, #0x50]
003cb0bc: mov      r3, #0xc
003cb0c0: mla      r3, r3, sl, r4
003cb0c4: cmn      r2, #1
003cb0c8: ldr      r7, [r3, #0xc]
003cb0cc: beq      #0x3cb2ac
003cb0d0: mov      r3, #0xc
003cb0d4: mla      sl, r3, sl, r4
003cb0d8: cmp      r7, #0
003cb0dc: mvnlt    r3, #0
003cb0e0: strlt    r3, [sl, #0xc]
003cb0e4: strge    r7, [sl, #0xc]
003cb0e8: mov      r3, #0
003cb0ec: strb     r3, [r4, #0x49]
003cb0f0: ldr      r1, [r4, #0x50]
003cb0f4: cmn      r1, #1
003cb0f8: beq      #0x3cb10c
003cb0fc: mov      r0, r4
003cb100: bl       #0x3cacb0
003cb104: mvn      r3, #0
003cb108: str      r3, [r4, #0x50]
003cb10c: ldr      r0, [pc, #0x1cc]
003cb110: add      r0, pc, r0
003cb114: bl       #0x3136b8
003cb118: b        #0x3caf9c
003cb11c: mov      r0, r4
003cb120: bl       #0x3c9168
003cb124: b        #0x3cafc4
003cb128: mov      r0, r4
003cb12c: bl       #0x3c91c8
003cb130: b        #0x3caf88
003cb134: ldrb     r3, [r4, #0x49]
003cb138: cmp      r3, #0
003cb13c: beq      #0x3cb0f0
003cb140: b        #0x3cafe4
003cb144: ldr      r0, [r4, #4]
003cb148: mov      r1, #0x25
003cb14c: mov      r2, #0
003cb150: bl       #0x3a4d5c
003cb154: ldr      r3, [r7, #0x10]
003cb158: cmp      r3, #1
003cb15c: bne      #0x3cb050
003cb160: add      r3, r3, #0xb
003cb164: mla      r3, r3, sl, r4
003cb168: ldr      r3, [r3, #0x10]
003cb16c: add      r3, r3, #1
003cb170: b        #0x3cb038
003cb174: ldr      r3, [r4, #0x2c]
003cb178: cmp      r3, #0
003cb17c: bne      #0x3cb258
003cb180: ldrb     r7, [r4, #0x48]
003cb184: cmp      r7, #0
003cb188: bne      #0x3cb0e8
003cb18c: ldr      r3, [pc, #0x144]
003cb190: add      r8, sp, #0x14
003cb194: ldr      sl, [r5, r3]
003cb198: mov      r0, sl
003cb19c: bl       #0x337888
003cb1a0: ldr      r1, [pc, #0x13c]
003cb1a4: add      r2, sp, #4
003cb1a8: mov      r0, r8
003cb1ac: add      r1, pc, r1
003cb1b0: bl       #0x3140ec
003cb1b4: mov      r1, r8
003cb1b8: mov      r0, sl
003cb1bc: bl       #0x337a88
003cb1c0: mov      r0, r8
003cb1c4: bl       #0x3139ac
003cb1c8: mov      r3, #1
003cb1cc: strb     r3, [r4, #0x48]
003cb1d0: mov      r2, r7
003cb1d4: ldr      r0, [r4, #4]
003cb1d8: mov      r1, #0x22
003cb1dc: bl       #0x3a4d5c
003cb1e0: b        #0x3cb0e8
003cb1e4: ldr      r3, [pc, #0xec]
003cb1e8: add      sl, sp, #0x5c
003cb1ec: ldr      sb, [r5, r3]
003cb1f0: mov      r0, sb
003cb1f4: bl       #0x337888
003cb1f8: ldr      r1, [pc, #0xe8]
003cb1fc: add      r2, sp, #0x10
003cb200: mov      r0, sl
003cb204: add      r1, pc, r1
003cb208: bl       #0x3140ec
003cb20c: mov      r1, sl
003cb210: mov      r0, sb
003cb214: bl       #0x337a88
003cb218: mov      r0, sl
003cb21c: bl       #0x3139ac
003cb220: mov      r1, #0x23
003cb224: ldr      r0, [r4, #4]
003cb228: mov      r2, #0
003cb22c: bl       #0x3a4d5c
003cb230: ldr      r1, [r8, #0x10]
003cb234: ldr      r3, [r7, #8]
003cb238: cmp      r1, r3
003cb23c: bhs      #0x3cb24c
003cb240: mov      r0, r4
003cb244: bl       #0x3ca79c
003cb248: b        #0x3cb0e8
003cb24c: mov      r0, r4
003cb250: bl       #0x3caf3c
003cb254: b        #0x3cb0e8
003cb258: ldr      r3, [pc, #0x78]
003cb25c: add      r7, sp, #0x2c
003cb260: ldr      r8, [r5, r3]
003cb264: mov      r0, r8
003cb268: bl       #0x337888
003cb26c: ldr      r1, [pc, #0x78]
003cb270: add      r2, sp, #8
003cb274: mov      r0, r7
003cb278: add      r1, pc, r1
003cb27c: bl       #0x3140ec
003cb280: mov      r1, r7
003cb284: mov      r0, r8
003cb288: bl       #0x337a88
003cb28c: mov      r0, r7
003cb290: bl       #0x3139ac
003cb294: ldr      r3, [r4, #0x2c]
003cb298: mov      r0, r4
003cb29c: sub      r3, r3, #1
003cb2a0: str      r3, [r4, #0x2c]
003cb2a4: bl       #0x3caf3c
003cb2a8: b        #0x3cb0e8
003cb2ac: ldr      r1, [r3, #8]
003cb2b0: mov      r0, r4
003cb2b4: ldr      r2, [r4, #0x2c]
003cb2b8: bl       #0x3cab38
003cb2bc: b        #0x3cb0d0
003cb2c0: bl       #0x30e310
003cb2c4: subseq   sb, ip, r4, asr #22
003cb2c8: andeq    r4, r0, ip, lsr #1
003cb2cc: subeq    sl, pc, r0, asr #4
003cb2d0: subeq    sl, pc, ip, lsl #4
003cb2d4: andeq    r3, r0, ip, ror ip
003cb2d8: andeq    r0, r0, r4, lsl #17
003cb2dc: subeq    sb, pc, r4, asr #31
003cb2e0: umaaleq  sl, pc, r0, r0
003cb2e4: subeq    sb, pc, r4, lsr #29
003cb2e8: subeq    sb, pc, ip, asr #28
003cb2ec: ldrdeq   sb, sl, [pc], #-0xd8

# _ZNK12CharAnimator15ANIM_IsSequenceEv
003c92d4: ldrb     r2, [r0, #0x48]
003c92d8: ldr      r3, [pc, #0x44]
003c92dc: cmp      r2, #0
003c92e0: add      r3, pc, r3
003c92e4: movne    r0, #0
003c92e8: bxne     lr
003c92ec: ldr      r2, [r0, #0x2c]
003c92f0: mov      r1, #0xc
003c92f4: mla      r0, r1, r2, r0
003c92f8: ldr      r2, [pc, #0x28]
003c92fc: mov      r1, #0x14
003c9300: ldr      r2, [r3, r2]
003c9304: ldr      r3, [r0, #8]
003c9308: ldr      r2, [r2]
003c930c: mla      r3, r1, r3, r2
003c9310: ldr      r0, [r3, #0x10]
003c9314: cmp      r0, #1
003c9318: movne    r0, #0
003c931c: moveq    r0, #1
003c9320: bx       lr
003c9324: ldrheq   fp, [ip], #-0x70
003c9328: andeq    r3, r0, ip, ror ip

# _ZN12CharAnimator8_SetAnimEij
003cab38: push     {r4, r5, r6, r7, r8, sl, lr}
003cab3c: ldr      r4, [pc, #0x150]
003cab40: ldr      r5, [pc, #0x150]
003cab44: sub      sp, sp, #0x44
003cab48: add      r4, pc, r4
003cab4c: ldr      r3, [r4, r5]
003cab50: cmp      r1, #0
003cab54: mov      r6, r0
003cab58: ldr      r3, [r3]
003cab5c: str      r3, [sp, #0x3c]
003cab60: blt      #0x3cab80
003cab64: ldr      r3, [pc, #0x130]
003cab68: ldr      r3, [r4, r3]
003cab6c: ldr      r3, [r3]
003cab70: cmp      r1, r3
003cab74: bge      #0x3cab80
003cab78: cmp      r2, #2
003cab7c: bls      #0x3cab9c
003cab80: ldr      r3, [r4, r5]
003cab84: ldr      r2, [sp, #0x3c]
003cab88: ldr      r3, [r3]
003cab8c: cmp      r2, r3
003cab90: bne      #0x3cac90
003cab94: add      sp, sp, #0x44
003cab98: pop      {r4, r5, r6, r7, r8, sl, pc}
003cab9c: ldr      r3, [pc, #0xfc]
003caba0: str      r1, [r0, #0x4c]
003caba4: mov      r8, #0x14
003caba8: ldr      r0, [r4, r3]
003cabac: mov      r3, #0xc
003cabb0: mla      r3, r3, r2, r6
003cabb4: ldr      r0, [r0]
003cabb8: str      r2, [r6, #0x2c]
003cabbc: ldr      r2, [pc, #0xe0]
003cabc0: mla      r8, r8, r1, r0
003cabc4: str      r1, [r3, #8]
003cabc8: ldr      sl, [r4, r2]
003cabcc: ldr      r2, [r8, #4]
003cabd0: add      r7, sp, #0x24
003cabd4: mov      r0, sl
003cabd8: str      r2, [r3, #0xc]
003cabdc: bl       #0x337888
003cabe0: ldr      r1, [pc, #0xc0]
003cabe4: add      r2, sp, #8
003cabe8: mov      r0, r7
003cabec: add      r1, pc, r1
003cabf0: bl       #0x3140ec
003cabf4: mov      r1, r7
003cabf8: mov      r0, sl
003cabfc: bl       #0x337a88
003cac00: mov      r0, r7
003cac04: bl       #0x3139ac
003cac08: ldr      r0, [r6, #4]
003cac0c: mov      r1, #0x24
003cac10: mov      r2, #0
003cac14: bl       #0x3a4d5c
003cac18: ldr      r3, [r8, #0x10]
003cac1c: cmp      r3, #2
003cac20: beq      #0x3cac34
003cac24: mov      r0, r6
003cac28: mov      r1, #0
003cac2c: bl       #0x3ca79c
003cac30: b        #0x3cab80
003cac34: mov      r0, sl
003cac38: bl       #0x337888
003cac3c: ldr      r1, [pc, #0x68]
003cac40: add      r7, sp, #0xc
003cac44: add      r2, sp, #4
003cac48: add      r1, pc, r1
003cac4c: mov      r0, r7
003cac50: bl       #0x3140ec
003cac54: mov      r0, sl
003cac58: mov      r1, r7
003cac5c: bl       #0x337a88
003cac60: mov      sl, r0
003cac64: eor      sl, sl, #1
003cac68: mov      r0, r7
003cac6c: bl       #0x3139ac
003cac70: tst      sl, #0xff
003cac74: beq      #0x3cac24
003cac78: ldr      r0, [r8, #8]
003cac7c: bl       #0x3ca708
003cac80: mov      r1, r0
003cac84: mov      r0, r6
003cac88: bl       #0x3ca79c
003cac8c: b        #0x3cab80
003cac90: bl       #0x30e310
003cac94: subseq   sb, ip, r8, asr #30
003cac98: andeq    r4, r0, ip, lsr #1
003cac9c: andeq    r2, r0, r8, asr #20
003caca0: andeq    r3, r0, ip, ror ip
003caca4: andeq    r0, r0, r4, lsl #17
003caca8: subeq    sl, pc, r4, ror #8
003cacac: umaaleq  r8, pc, r8, pc

# _ZNK12CharAnimator13ANIM_IsUniqueEv
003c9228: ldrb     r2, [r0, #0x48]
003c922c: ldr      r3, [pc, #0x40]
003c9230: cmp      r2, #0
003c9234: add      r3, pc, r3
003c9238: movne    r0, #0
003c923c: bxne     lr
003c9240: ldr      r2, [r0, #0x2c]
003c9244: mov      r1, #0xc
003c9248: mla      r0, r1, r2, r0
003c924c: ldr      r2, [pc, #0x24]
003c9250: mov      r1, #0x14
003c9254: ldr      r2, [r3, r2]
003c9258: ldr      r3, [r0, #8]
003c925c: ldr      r2, [r2]
003c9260: mla      r3, r1, r3, r2
003c9264: ldr      r0, [r3, #0x10]
003c9268: rsbs     r0, r0, #1
003c926c: movlo    r0, #0
003c9270: bx       lr
003c9274: subseq   fp, ip, ip, asr r8
003c9278: andeq    r3, r0, ip, ror ip

# _ZN12CharAnimator12_SetAnimStepEj
003ca79c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003ca7a0: ldr      r3, [r0, #0x2c]
003ca7a4: mov      r2, #0xc
003ca7a8: ldr      r5, [pc, #0x374]
003ca7ac: mla      r3, r2, r3, r0
003ca7b0: ldr      r2, [pc, #0x370]
003ca7b4: add      r5, pc, r5
003ca7b8: mov      r4, r0
003ca7bc: ldr      r2, [r5, r2]
003ca7c0: ldr      r0, [r3, #8]
003ca7c4: mov      ip, #0x14
003ca7c8: ldr      r2, [r2]
003ca7cc: sub      sp, sp, #0x24
003ca7d0: mla      r2, ip, r0, r2
003ca7d4: ldr      r0, [r2, #8]
003ca7d8: cmp      r0, r1
003ca7dc: bhi      #0x3ca7e8
003ca7e0: add      sp, sp, #0x24
003ca7e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003ca7e8: ldr      r2, [r2, #0xc]
003ca7ec: mov      r6, #0x38
003ca7f0: str      r1, [r3, #0x10]
003ca7f4: mla      r6, r6, r1, r2
003ca7f8: ldr      r0, [r4, #4]
003ca7fc: mov      r1, #0x26
003ca800: mov      r2, #0
003ca804: bl       #0x3a4d5c
003ca808: ldr      r3, [r6, #0x28]
003ca80c: cmp      r3, #1
003ca810: beq      #0x3caad8
003ca814: ldr      r3, [r6, #0x10]
003ca818: cmn      r3, #1
003ca81c: beq      #0x3ca9d8
003ca820: ldr      r3, [pc, #0x304]
003ca824: ldr      r0, [r5, r3]
003ca828: bl       #0x31f594
003ca82c: cmp      r0, #0
003ca830: beq      #0x3ca870
003ca834: ldr      r7, [r0, #0x128]
003ca838: cmp      r7, #0
003ca83c: beq      #0x3ca870
003ca840: mov      r0, r7
003ca844: ldr      r1, [r4, #4]
003ca848: bl       #0x40f980
003ca84c: cmp      r0, #0
003ca850: beq      #0x3ca870
003ca854: ldr      r1, [r6, #0x10]
003ca858: cmn      r1, #1
003ca85c: beq      #0x3ca9f4
003ca860: mov      r0, r7
003ca864: mov      r2, #0
003ca868: mov      r3, #1
003ca86c: bl       #0x40f904
003ca870: ldrb     r3, [r6, #0x34]
003ca874: strb     r3, [r4, #0x30]
003ca878: ldrb     r3, [r6, #0x34]
003ca87c: cmp      r3, #0
003ca880: moveq    r7, #1
003ca884: bne      #0x3caa10
003ca888: ldr      r3, [pc, #0x2a0]
003ca88c: ldr      r0, [r4, #4]
003ca890: ldr      sb, [r6, #0x2c]
003ca894: ldr      r3, [r5, r3]
003ca898: ldr      fp, [r3]
003ca89c: bl       #0x3935dc
003ca8a0: ldr      lr, [r0]
003ca8a4: ldr      r8, [r0, #4]
003ca8a8: ldr      sl, [r0, #8]
003ca8ac: mov      ip, #0xbf000000
003ca8b0: add      ip, ip, #0x800000
003ca8b4: str      lr, [sp, #0x14]
003ca8b8: mov      r0, fp
003ca8bc: mov      lr, #1
003ca8c0: mov      r1, sb
003ca8c4: add      r2, sp, #0x14
003ca8c8: mov      r3, #0
003ca8cc: str      r8, [sp, #0x18]
003ca8d0: str      sl, [sp, #0x1c]
003ca8d4: str      lr, [sp]
003ca8d8: str      ip, [sp, #8]
003ca8dc: str      ip, [sp, #4]
003ca8e0: bl       #0x36b5d8
003ca8e4: cmp      r7, #0
003ca8e8: beq      #0x3ca91c
003ca8ec: ldr      r8, [r6, #0x18]
003ca8f0: cmn      r8, #1
003ca8f4: beq      #0x3ca91c
003ca8f8: ldrb     r7, [r6, #4]
003ca8fc: cmp      r7, #0
003ca900: beq      #0x3caa94
003ca904: ldr      r3, [pc, #0x228]
003ca908: mov      r1, r8
003ca90c: ldr      r2, [r4, #4]
003ca910: ldr      r0, [r5, r3]
003ca914: mov      r3, #0
003ca918: bl       #0x495f04
003ca91c: ldr      r2, [r6, #0x30]
003ca920: ldr      r3, [r4, #4]
003ca924: str      r2, [r4, #0x34]
003ca928: ldr      r5, [r3, #0x2d8]
003ca92c: cmp      r5, #0
003ca930: beq      #0x3ca9e8
003ca934: ldr      r3, [r6, #8]
003ca938: cmn      r3, #1
003ca93c: beq      #0x3ca9e8
003ca940: mov      r3, #0
003ca944: strb     r3, [r4, #0x48]
003ca948: mov      r0, r4
003ca94c: bl       #0x3c9b7c
003ca950: ldrb     r3, [r4, #0x54]
003ca954: cmp      r3, #0
003ca958: beq      #0x3caa80
003ca95c: ldrb     r1, [r4, #0x49]
003ca960: ldr      r3, [r5, #0x38]
003ca964: ldrb     r2, [r6, #0x1c]
003ca968: cmp      r1, #0
003ca96c: beq      #0x3caac4
003ca970: mov      r1, #0
003ca974: str      r1, [r3, #0x14]
003ca978: mov      r1, #0
003ca97c: str      r1, [r3, #0xc]
003ca980: strb     r2, [r3, #0x10]
003ca984: ldr      r2, [r5, #0x38]
003ca988: ldr      r1, [r6, #8]
003ca98c: mov      r6, #0
003ca990: ldr      r3, [r4, #0x44]
003ca994: ldr      ip, [r2]
003ca998: mov      r0, r2
003ca99c: str      r6, [sp]
003ca9a0: mov      r2, r6
003ca9a4: mov      lr, pc
003ca9a8: ldr      pc, [ip, #0x1c]
003ca9ac: ldr      r1, [r4, #0x34]
003ca9b0: ldr      r0, [r4, #0x40]
003ca9b4: bl       #0x30ed6c
003ca9b8: ldr      r5, [r5, #0x38]
003ca9bc: mov      r1, r0
003ca9c0: mov      r2, r6
003ca9c4: mov      r0, r5
003ca9c8: ldr      r3, [r5]
003ca9cc: mov      lr, pc
003ca9d0: ldr      pc, [r3, #0x28]
003ca9d4: b        #0x3ca7e0
003ca9d8: ldr      r3, [r6, #0x20]
003ca9dc: cmp      r3, #0
003ca9e0: beq      #0x3ca870
003ca9e4: b        #0x3ca820
003ca9e8: mov      r0, r4
003ca9ec: bl       #0x3c9924
003ca9f0: b        #0x3ca7e0
003ca9f4: ldr      r0, [r6, #0x20]
003ca9f8: cmp      r0, #0
003ca9fc: beq      #0x3ca870
003caa00: ldr      r8, [r6, #0x24]
003caa04: bl       #0x3ca708
003caa08: ldr      r1, [r8, r0, lsl #2]
003caa0c: b        #0x3ca860
003caa10: ldr      r0, [r4, #4]
003caa14: mov      r1, #1
003caa18: add      r0, r0, #0x37c
003caa1c: bl       #0x3ffe3c
003caa20: mov      r7, r0
003caa24: ldr      r0, [r4, #4]
003caa28: mov      r1, #2
003caa2c: add      r0, r0, #0x37c
003caa30: bl       #0x3ffe3c
003caa34: mov      r1, r7
003caa38: mov      sl, r0
003caa3c: mov      r0, r4
003caa40: bl       #0x3c94b8
003caa44: mov      r1, r7
003caa48: eor      r8, r0, #1
003caa4c: ldrb     r2, [r6, #4]
003caa50: mov      r0, r4
003caa54: bl       #0x3c955c
003caa58: uxtb     r8, r8
003caa5c: eor      r0, r0, #1
003caa60: cmp      r8, #0
003caa64: uxtb     r7, r0
003caa68: bne      #0x3caaf0
003caa6c: cmp      r7, #0
003caa70: bne      #0x3cab08
003caa74: cmp      r8, #0
003caa78: beq      #0x3ca8e4
003caa7c: b        #0x3ca888
003caa80: ldr      r2, [r5, #0x38]
003caa84: ldrb     r1, [r6, #0x1c]
003caa88: str      r3, [r2, #0xc]
003caa8c: strb     r1, [r2, #0x10]
003caa90: b        #0x3ca984
003caa94: ldr      r0, [r4, #4]
003caa98: bl       #0x3935dc
003caa9c: ldr      r3, [r4, #4]
003caaa0: mov      r2, r0
003caaa4: ldr      r0, [pc, #0x88]
003caaa8: mov      r1, r8
003caaac: add      r3, r3, #0x16c
003caab0: ldr      r0, [r5, r0]
003caab4: str      r7, [sp, #4]
003caab8: str      r7, [sp]
003caabc: bl       #0x495888
003caac0: b        #0x3ca91c
003caac4: ldrb     r1, [r4, #0x4a]
003caac8: cmp      r1, #0
003caacc: ldreq    r1, [r6, #0xc]
003caad0: beq      #0x3ca974
003caad4: b        #0x3ca970
003caad8: ldr      r2, [r4, #0x2c]
003caadc: mov      r0, r4
003caae0: ldr      r1, [r6, #8]
003caae4: add      r2, r2, #1
003caae8: bl       #0x3cab38
003caaec: b        #0x3ca7e0
003caaf0: mov      r0, r4
003caaf4: mov      r1, sl
003caaf8: bl       #0x3c94b8
003caafc: eor      r0, r0, #1
003cab00: uxtb     r8, r0
003cab04: b        #0x3caa6c
003cab08: mov      r1, sl
003cab0c: mov      r0, r4
003cab10: ldrb     r2, [r6, #4]
003cab14: bl       #0x3c955c
003cab18: eor      r0, r0, #1
003cab1c: uxtb     r8, r0
003cab20: b        #0x3caa74
003cab24: ldrsbeq  sl, [ip], #-0x2c
003cab28: andeq    r3, r0, ip, ror ip
003cab2c: strdeq   r3, r4, [r0], -r4
003cab30: andeq    r0, r0, r4, lsr #27
003cab34: andeq    r1, r0, r8, lsl #22

# _ZNK12CharAnimator13ANIM_IsRandomEv
003c927c: ldrb     r2, [r0, #0x48]
003c9280: ldr      r3, [pc, #0x44]
003c9284: cmp      r2, #0
003c9288: add      r3, pc, r3
003c928c: movne    r0, #0
003c9290: bxne     lr
003c9294: ldr      r2, [r0, #0x2c]
003c9298: mov      r1, #0xc
003c929c: mla      r0, r1, r2, r0
003c92a0: ldr      r2, [pc, #0x28]
003c92a4: mov      r1, #0x14
003c92a8: ldr      r2, [r3, r2]
003c92ac: ldr      r3, [r0, #8]
003c92b0: ldr      r2, [r2]
003c92b4: mla      r3, r1, r3, r2
003c92b8: ldr      r0, [r3, #0x10]
003c92bc: cmp      r0, #2
003c92c0: movne    r0, #0
003c92c4: moveq    r0, #1
003c92c8: bx       lr
003c92cc: subseq   fp, ip, r8, lsl #16
003c92d0: andeq    r3, r0, ip, ror ip
