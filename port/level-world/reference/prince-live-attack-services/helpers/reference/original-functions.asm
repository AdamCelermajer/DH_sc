
# _ZNK10GameObject12GetLookAtVecER7Point3DIfE
00393ae4: push     {r4, r5, r6, lr}
00393ae8: ldr      r4, [r0, #0x174]
00393aec: mov      r5, r1
00393af0: mov      r0, r4
00393af4: bl       #0x30eb08
00393af8: mov      r6, r0
00393afc: mov      r0, r4
00393b00: bl       #0x30e754
00393b04: mov      r3, #0
00393b08: add      r0, r0, #0x80000000
00393b0c: str      r6, [r5]
00393b10: str      r3, [r5, #8]
00393b14: str      r0, [r5, #4]
00393b18: pop      {r4, r5, r6, pc}

# _ZN14ObjectSearcher10TargetList6SearchERK7Point3DIfEfS4_fRNS_11IObjectListE
004a2f34: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a2f38: ldr      r6, [pc, #0x458]
004a2f3c: ldr      r7, [pc, #0x458]
004a2f40: ldr      lr, [pc, #0x458]
004a2f44: add      r6, pc, r6
004a2f48: ldr      ip, [r6, r7]
004a2f4c: ldr      r8, [r6, lr]
004a2f50: sub      sp, sp, #0x6c
004a2f54: ldr      ip, [ip]
004a2f58: mov      r5, r0
004a2f5c: mov      r0, r8
004a2f60: str      r3, [sp, #0x1c]
004a2f64: str      ip, [sp, #0x64]
004a2f68: mov      sb, r1
004a2f6c: str      r2, [sp, #0x10]
004a2f70: ldr      r4, [sp, #0x94]
004a2f74: bl       #0x337888
004a2f78: ldr      r1, [pc, #0x424]
004a2f7c: add      sl, sp, #0x4c
004a2f80: add      r2, sp, #0x48
004a2f84: add      r1, pc, r1
004a2f88: mov      r0, sl
004a2f8c: bl       #0x3140ec
004a2f90: mov      r0, r8
004a2f94: mov      r1, sl
004a2f98: bl       #0x337a88
004a2f9c: ldr      r0, [sp, #0x60]
004a2fa0: cmp      r0, sl
004a2fa4: beq      #0x4a2fdc
004a2fa8: cmp      r0, #0
004a2fac: beq      #0x4a2fdc
004a2fb0: ldr      r1, [sp, #0x4c]
004a2fb4: rsb      r1, r0, r1
004a2fb8: cmp      r1, #0x80
004a2fbc: bhi      #0x4a330c
004a2fc0: bl       #0x708f00
004a2fc4: ldr      r2, [r5, #0x10]
004a2fc8: ldr      r3, [r5]
004a2fcc: cmp      r2, r3
004a2fd0: beq      #0x4a2fec
004a2fd4: mov      r0, r5
004a2fd8: bl       #0x38fb18
004a2fdc: ldr      r2, [r5, #0x10]
004a2fe0: ldr      r3, [r5]
004a2fe4: cmp      r2, r3
004a2fe8: bne      #0x4a2fd4
004a2fec: ldr      r0, [sp, #0x10]
004a2ff0: mov      r1, #0
004a2ff4: bl       #0x30e4b4
004a2ff8: cmp      r0, #0
004a2ffc: bne      #0x4a3024
004a3000: ldr      r3, [pc, #0x3a0]
004a3004: ldr      r3, [r6, r3]
004a3008: ldr      r3, [r3]
004a300c: cmp      r3, #2
004a3010: moveq    r3, #0
004a3014: streq    r3, [r3]
004a3018: beq      #0x4a3024
004a301c: cmp      r3, #1
004a3020: beq      #0x4a332c
004a3024: ldr      r0, [sp, #0x90]
004a3028: mov      r1, #0
004a302c: bl       #0x30e4b4
004a3030: cmp      r0, #0
004a3034: bne      #0x4a305c
004a3038: ldr      r3, [pc, #0x368]
004a303c: ldr      r3, [r6, r3]
004a3040: ldr      r3, [r3]
004a3044: cmp      r3, #2
004a3048: moveq    r3, #0
004a304c: streq    r3, [r3]
004a3050: beq      #0x4a305c
004a3054: cmp      r3, #1
004a3058: beq      #0x4a3360
004a305c: ldr      r0, [r5, #0x30]
004a3060: cmp      r0, #0
004a3064: moveq    r3, #0
004a3068: streq    r3, [sp, #0x18]
004a306c: beq      #0x4a307c
004a3070: add      r0, r0, #0x3c8
004a3074: bl       #0x3d4c34
004a3078: str      r0, [sp, #0x18]
004a307c: mov      r8, #0
004a3080: ldr      r3, [r4]
004a3084: mov      r0, r4
004a3088: str      r8, [sp, #0x3c]
004a308c: str      r8, [sp, #0x40]
004a3090: str      r8, [sp, #0x44]
004a3094: mov      lr, pc
004a3098: ldr      pc, [r3, #8]
004a309c: add      r3, sp, #0x3c
004a30a0: str      r3, [sp, #0x20]
004a30a4: add      r3, sp, #0x28
004a30a8: str      r3, [sp, #0x24]
004a30ac: b        #0x4a30c0
004a30b0: ldr      r3, [r4]
004a30b4: mov      r0, r4
004a30b8: mov      lr, pc
004a30bc: ldr      pc, [r3, #0x10]
004a30c0: ldr      r3, [r4]
004a30c4: mov      r0, r4
004a30c8: mov      lr, pc
004a30cc: ldr      pc, [r3, #0xc]
004a30d0: cmp      r0, #0
004a30d4: bne      #0x4a32f0
004a30d8: ldr      r3, [r4]
004a30dc: mov      r0, r4
004a30e0: mov      lr, pc
004a30e4: ldr      pc, [r3, #0x18]
004a30e8: ldr      r3, [r4]
004a30ec: mov      r8, r0
004a30f0: mov      r0, r4
004a30f4: mov      lr, pc
004a30f8: ldr      pc, [r3, #0x1c]
004a30fc: cmp      r8, #0
004a3100: mov      sl, r0
004a3104: beq      #0x4a30b0
004a3108: ldr      r3, [r5, #0x2c]
004a310c: cmp      r8, r3
004a3110: beq      #0x4a30b0
004a3114: ldrb     r3, [r8, #0x8a]
004a3118: cmp      r3, #0
004a311c: beq      #0x4a30b0
004a3120: ldr      r3, [r8]
004a3124: mov      r0, r8
004a3128: mov      lr, pc
004a312c: ldr      pc, [r3, #0xc4]
004a3130: cmp      r0, #0
004a3134: beq      #0x4a3150
004a3138: ldrb     r3, [r8, #0x2ee]
004a313c: cmp      r3, #0
004a3140: beq      #0x4a3150
004a3144: ldrb     r3, [r8, #0x2f0]
004a3148: cmp      r3, #0
004a314c: beq      #0x4a30b0
004a3150: ldr      r3, [r8]
004a3154: mov      r0, r8
004a3158: ldr      r1, [r5, #0x2c]
004a315c: mov      lr, pc
004a3160: ldr      pc, [r3, #0x88]
004a3164: cmp      r0, #0
004a3168: beq      #0x4a30b0
004a316c: cmp      sl, #0
004a3170: beq      #0x4a3314
004a3174: mov      r0, r5
004a3178: mov      r1, sl
004a317c: bl       #0x4a1ab8
004a3180: cmp      r0, #0
004a3184: beq      #0x4a30b0
004a3188: ldr      r3, [r8]
004a318c: mov      r0, r8
004a3190: mov      lr, pc
004a3194: ldr      pc, [r3, #0x94]
004a3198: mov      r2, r0
004a319c: mov      r0, r8
004a31a0: str      r2, [sp, #8]
004a31a4: bl       #0x3935dc
004a31a8: ldr      r1, [sb]
004a31ac: mov      fp, r0
004a31b0: ldr      r0, [r0]
004a31b4: bl       #0x30e3ac
004a31b8: ldr      r1, [sb, #4]
004a31bc: mov      r3, r0
004a31c0: ldr      r0, [fp, #4]
004a31c4: str      r3, [sp, #0xc]
004a31c8: bl       #0x30e3ac
004a31cc: str      r0, [sp, #0x14]
004a31d0: ldr      r0, [fp, #8]
004a31d4: ldr      r1, [sb, #8]
004a31d8: bl       #0x30e3ac
004a31dc: ldr      r3, [sp, #0xc]
004a31e0: mov      fp, r0
004a31e4: str      fp, [sp, #0x44]
004a31e8: mov      r1, r3
004a31ec: mov      r0, r3
004a31f0: str      r3, [sp, #0x3c]
004a31f4: ldr      r3, [sp, #0x14]
004a31f8: str      r3, [sp, #0x40]
004a31fc: bl       #0x30ed6c
004a3200: mov      r3, r0
004a3204: ldr      r0, [sp, #0x14]
004a3208: str      r3, [sp, #0xc]
004a320c: mov      r1, r0
004a3210: bl       #0x30ed6c
004a3214: ldr      r3, [sp, #0xc]
004a3218: mov      r1, r0
004a321c: mov      r0, r3
004a3220: bl       #0x30eba4
004a3224: mov      r1, fp
004a3228: mov      r3, r0
004a322c: mov      r0, fp
004a3230: str      r3, [sp, #0xc]
004a3234: bl       #0x30ed6c
004a3238: ldr      r3, [sp, #0xc]
004a323c: mov      r1, r0
004a3240: mov      r0, r3
004a3244: bl       #0x30eba4
004a3248: bl       #0x30e124
004a324c: ldr      r2, [sp, #8]
004a3250: mov      r1, r2
004a3254: bl       #0x30e3ac
004a3258: ldr      r1, [sp, #0x18]
004a325c: bl       #0x30e3ac
004a3260: ldr      r1, [sp, #0x10]
004a3264: str      r0, [sp, #0x14]
004a3268: bl       #0x30e2f8
004a326c: cmp      r0, #0
004a3270: bne      #0x4a30b0
004a3274: ldr      r1, [sp, #0x1c]
004a3278: ldr      r0, [sp, #0x20]
004a327c: bl       #0x313058
004a3280: movw     r1, #0xfdb
004a3284: bic      r0, r0, #0x80000000
004a3288: mov      fp, r0
004a328c: movt     r1, #0x4049
004a3290: ldr      r0, [sp, #0x90]
004a3294: bl       #0x30e70c
004a3298: cmp      r0, #0
004a329c: beq      #0x4a32b4
004a32a0: ldr      r0, [sp, #0x90]
004a32a4: mov      r1, fp
004a32a8: bl       #0x30e70c
004a32ac: cmp      r0, #0
004a32b0: bne      #0x4a30b0
004a32b4: ldr      r3, [sp, #0x14]
004a32b8: cmp      sl, #0
004a32bc: mov      r0, r5
004a32c0: str      r3, [sp, #0x2c]
004a32c4: mov      r3, #0
004a32c8: str      r3, [sp, #0x34]
004a32cc: mov      r3, #0
004a32d0: str      r3, [sp, #0x38]
004a32d4: ldr      r1, [sp, #0x24]
004a32d8: movne    r3, #1
004a32dc: str      r8, [sp, #0x28]
004a32e0: str      fp, [sp, #0x30]
004a32e4: strne    r3, [sp, #0x34]
004a32e8: bl       #0x4a2440
004a32ec: b        #0x4a30b0
004a32f0: ldr      r3, [r6, r7]
004a32f4: ldr      r2, [sp, #0x64]
004a32f8: ldr      r3, [r3]
004a32fc: cmp      r2, r3
004a3300: bne      #0x4a3394
004a3304: add      sp, sp, #0x6c
004a3308: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a330c: bl       #0x310440
004a3310: b        #0x4a2fdc
004a3314: mov      r0, r5
004a3318: mov      r1, r8
004a331c: bl       #0x4a1950
004a3320: cmp      r0, #0
004a3324: beq      #0x4a30b0
004a3328: b        #0x4a3188
004a332c: ldr      r0, [pc, #0x78]
004a3330: ldr      r1, [pc, #0x78]
004a3334: ldr      r2, [pc, #0x78]
004a3338: ldr      r0, [r6, r0]
004a333c: ldr      r3, [pc, #0x74]
004a3340: mov      ip, #0xcc
004a3344: add      r1, pc, r1
004a3348: add      r2, pc, r2
004a334c: add      r3, pc, r3
004a3350: add      r0, r0, #0xa8
004a3354: str      ip, [sp]
004a3358: bl       #0x30e004
004a335c: b        #0x4a3024
004a3360: ldr      r0, [pc, #0x44]
004a3364: ldr      r1, [pc, #0x50]
004a3368: ldr      r2, [pc, #0x50]
004a336c: ldr      r0, [r6, r0]
004a3370: ldr      r3, [pc, #0x4c]
004a3374: mov      ip, #0xcd
004a3378: add      r1, pc, r1
004a337c: add      r2, pc, r2
004a3380: add      r3, pc, r3
004a3384: add      r0, r0, #0xa8
004a3388: str      ip, [sp]
004a338c: bl       #0x30e004
004a3390: b        #0x4a305c
004a3394: bl       #0x30e310
004a3398: subeq    r1, pc, ip, asr #22
004a339c: andeq    r4, r0, ip, lsr #1
004a33a0: andeq    r0, r0, r4, lsl #17
004a33a4: subeq    r2, r3, r4, lsr #13
004a33a8: andeq    r3, r0, r0, asr #19
004a33ac: andeq    r1, r0, r0, asr #19
004a33b0: umaaleq  fp, r1, r4, r0
004a33b4: strdeq   r2, r3, [r3], #-0x28
004a33b8: subeq    r2, r3, ip, lsl #6
004a33bc: subeq    fp, r1, r0, rrx
004a33c0: subeq    r2, r3, ip, lsr r3
004a33c4: ldrdeq   r2, r3, [r3], #-0x28

# _ZNK10GameObject17GetTargetPositionEv
003935dc: ldr      r3, [r0, #0x180]
003935e0: cmp      r3, #0
003935e4: beq      #0x3935f8
003935e8: ldrb     r3, [r0, #0x80]
003935ec: cmp      r3, #0
003935f0: addne    r0, r0, #0x184
003935f4: bxne     lr
003935f8: add      r0, r0, #0x160
003935fc: bx       lr

# _ZNK9Character18GetCharAIFactionIdEv
003a3180: ldr      r0, [r0, #0xff8]
003a3184: ldr      r3, [pc, #0x24]
003a3188: cmp      r0, #0
003a318c: add      r3, pc, r3
003a3190: blt      #0x3a31a8
003a3194: ldr      r2, [pc, #0x18]
003a3198: ldr      r3, [r3, r2]
003a319c: ldr      r3, [r3]
003a31a0: cmp      r0, r3
003a31a4: bxlt     lr
003a31a8: mov      r0, #0xa
003a31ac: bx       lr
003a31b0: subseq   r1, pc, r4, lsl #18
003a31b4: andeq    r2, r0, r4, asr #4

# _ZNK12ObjectHandle9GetObjectEb
0033ff8c: b        #0x33fdc0

# _ZNK6CharAI17AI_GetMeleeRadiusEv
003d4c34: push     {r4, r5, lr}
003d4c38: mov      r5, r0
003d4c3c: ldr      r0, [r0, #4]
003d4c40: sub      sp, sp, #0xc
003d4c44: add      r1, sp, #8
003d4c48: mov      r3, #0
003d4c4c: str      r3, [r1, #-4]!
003d4c50: add      r0, r0, #0x37c
003d4c54: ldr      r4, [pc, #0x38]
003d4c58: bl       #0x3fff30
003d4c5c: ldr      r3, [pc, #0x34]
003d4c60: add      r4, pc, r4
003d4c64: ldr      r0, [r5, #4]
003d4c68: ldr      r3, [r4, r3]
003d4c6c: ldr      r4, [r3]
003d4c70: bl       #0x3a2fec
003d4c74: mov      r3, #0x44
003d4c78: mla      r4, r3, r0, r4
003d4c7c: ldr      r0, [sp, #4]
003d4c80: bl       #0x30e964
003d4c84: ldr      r1, [r4, #0x20]
003d4c88: bl       #0x30eba4
003d4c8c: add      sp, sp, #0xc
003d4c90: pop      {r4, r5, pc}
003d4c94: subseq   pc, fp, r0, lsr lr
003d4c98: andeq    r0, r0, r8, asr r7

# _ZNK6CharAI23AI_IsInInteractionRangeEPK10GameObject
003d4f98: push     {r4, r5, r6, r7, r8, sl, lr}
003d4f9c: subs     r4, r1, #0
003d4fa0: sub      sp, sp, #0x1c
003d4fa4: mov      r6, r0
003d4fa8: beq      #0x3d50d0
003d4fac: ldr      r0, [r6, #4]
003d4fb0: bl       #0x3935dc
003d4fb4: mov      r1, r4
003d4fb8: mov      r5, r0
003d4fbc: add      r0, sp, #0xc
003d4fc0: bl       #0x38b228
003d4fc4: ldr      r1, [sp, #0xc]
003d4fc8: ldr      r0, [r5]
003d4fcc: bl       #0x30e3ac
003d4fd0: ldr      r1, [sp, #0x10]
003d4fd4: mov      sl, r0
003d4fd8: ldr      r0, [r5, #4]
003d4fdc: bl       #0x30e3ac
003d4fe0: ldr      r1, [sp, #0x14]
003d4fe4: mov      r8, r0
003d4fe8: ldr      r0, [r5, #8]
003d4fec: bl       #0x30e3ac
003d4ff0: mov      r1, sl
003d4ff4: mov      r7, r0
003d4ff8: mov      r0, sl
003d4ffc: bl       #0x30ed6c
003d5000: mov      r1, r8
003d5004: mov      r5, r0
003d5008: mov      r0, r8
003d500c: bl       #0x30ed6c
003d5010: mov      r1, r0
003d5014: mov      r0, r5
003d5018: bl       #0x30eba4
003d501c: mov      r1, r7
003d5020: mov      r5, r0
003d5024: mov      r0, r7
003d5028: bl       #0x30ed6c
003d502c: mov      r1, r0
003d5030: mov      r0, r5
003d5034: bl       #0x30eba4
003d5038: bl       #0x30e124
003d503c: ldr      r3, [r4, #0x2e8]
003d5040: mov      r8, r0
003d5044: cmp      r3, #0
003d5048: beq      #0x3d50e4
003d504c: mov      r5, #0
003d5050: mov      r7, #0x42000000
003d5054: add      r7, r7, #0xa00000
003d5058: mov      r1, r5
003d505c: mov      r0, r8
003d5060: bl       #0x30e3ac
003d5064: mov      r1, r5
003d5068: bl       #0x30e3ac
003d506c: ldr      r1, [r6, #4]
003d5070: mov      r5, r0
003d5074: ldr      r3, [r4]
003d5078: mov      r0, r4
003d507c: mov      lr, pc
003d5080: ldr      pc, [r3, #0x90]
003d5084: cmp      r0, #8
003d5088: beq      #0x3d50b0
003d508c: mov      r0, r5
003d5090: mov      r1, r7
003d5094: bl       #0x30e9ac
003d5098: cmp      r0, #0
003d509c: mov      r0, #0
003d50a0: bne      #0x3d50c8
003d50a4: uxtb     r0, r0
003d50a8: add      sp, sp, #0x1c
003d50ac: pop      {r4, r5, r6, r7, r8, sl, pc}
003d50b0: mov      r0, r5
003d50b4: mov      r1, #0
003d50b8: bl       #0x30e9ac
003d50bc: cmp      r0, #0
003d50c0: mov      r0, #0
003d50c4: beq      #0x3d50a4
003d50c8: mov      r0, #1
003d50cc: b        #0x3d50a4
003d50d0: ldr      r4, [r0, #0x40]
003d50d4: cmp      r4, #0
003d50d8: moveq    r0, r4
003d50dc: beq      #0x3d50a8
003d50e0: b        #0x3d4fac
003d50e4: mov      r0, r6
003d50e8: bl       #0x3d4c34
003d50ec: mov      r1, r0
003d50f0: ldr      r3, [r4]
003d50f4: mov      r0, r4
003d50f8: str      r1, [sp, #4]
003d50fc: mov      lr, pc
003d5100: ldr      pc, [r3, #0x94]
003d5104: mov      r5, r0
003d5108: ldr      r0, [r6, #4]
003d510c: bl       #0x3a3024
003d5110: ldr      r1, [sp, #4]
003d5114: ldr      r7, [r0, #0x18]
003d5118: b        #0x3d505c

# _ZNK13ItemInventory14CanRangeAttackEv
00400014: b        #0x3fffa4
