
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

# _ZN12CharAnimator10__CallbackEPN6glitch5scene19ITimelineControllerEPv
003c90ec: mov      r3, #1
003c90f0: strb     r3, [r1, #0x49]
003c90f4: bx       lr

# _ZN12CharAnimator17ANIM_SkipNextStepEv
003c9464: ldr      r1, [r0, #0x2c]
003c9468: b        #0x3c9444

# _ZN12CharAnimator13ANIM_StopLoopEb
003c948c: ldrb     r3, [r0, #0x48]
003c9490: cmp      r3, #0
003c9494: bxne     lr
003c9498: ldr      r2, [r0, #0x2c]
003c949c: cmp      r1, #0
003c94a0: mov      r1, #0xc
003c94a4: mla      r2, r1, r2, r0
003c94a8: str      r3, [r2, #0xc]
003c94ac: movne    r3, #1
003c94b0: strbne   r3, [r0, #0x4a]
003c94b4: bx       lr

# _ZN12CharAnimator8ANIM_SetEi
003cacb0: ldrb     r2, [r0, #0x49]
003cacb4: cmp      r2, #0
003cacb8: strne    r1, [r0, #0x50]
003cacbc: bxne     lr
003cacc0: mov      ip, #0x3f800000
003cacc4: str      ip, [r0, #0x40]
003cacc8: b        #0x3cab38
