
# _ZN14ObjectSearcher10TargetList11SetSortTypeEi.clone.1
003d015c: push     {r4, r5, r6, lr}
003d0160: ldr      r2, [r0, #0x10]
003d0164: ldr      r3, [r0]
003d0168: ldr      r5, [pc, #0x34]
003d016c: mov      r4, r0
003d0170: cmp      r2, r3
003d0174: add      r5, pc, r5
003d0178: beq      #0x3d0194
003d017c: mov      r0, r4
003d0180: bl       #0x38fb18
003d0184: ldr      r2, [r4, #0x10]
003d0188: ldr      r3, [r4]
003d018c: cmp      r2, r3
003d0190: bne      #0x3d017c
003d0194: ldr      r3, [pc, #0xc]
003d0198: ldr      r3, [r5, r3]
003d019c: str      r3, [r4, #0x28]
003d01a0: pop      {r4, r5, r6, pc}
003d01a4: subseq   r4, ip, ip, lsl sb
003d01a8: ldrdeq   r1, r2, [r0], -r4

# _ZNSaIN14ObjectSearcher10TargetInfoEE8allocateEjPKv.clone.7
004a1e0c: str      lr, [sp, #-4]!
004a1e10: sub      sp, sp, #0xc
004a1e14: add      r0, sp, #8
004a1e18: mov      r3, #0x78
004a1e1c: str      r3, [r0, #-4]!
004a1e20: bl       #0x708ec0
004a1e24: add      sp, sp, #0xc
004a1e28: ldm      sp!, {pc}

# _ZNSt4priv11_Deque_baseIN14ObjectSearcher10TargetInfoESaIS2_EE17_M_initialize_mapEj.clone.4
004a2240: mov      r3, #8
004a2244: push     {r4, r5, r6, lr}
004a2248: mov      r1, r3
004a224c: mov      r4, r0
004a2250: str      r3, [r0, #0x24]
004a2254: mov      r2, #0
004a2258: add      r0, r0, #0x20
004a225c: bl       #0x4a21e0
004a2260: mov      r5, r0
004a2264: str      r0, [r4, #0x20]
004a2268: mov      r0, r4
004a226c: ldr      r6, [r0, #0x24]!
004a2270: bl       #0x4a1e0c
004a2274: sub      r6, r6, #1
004a2278: lsr      r6, r6, #1
004a227c: str      r0, [r5, r6, lsl #2]
004a2280: add      r3, r5, r6, lsl #2
004a2284: str      r3, [r4, #0xc]
004a2288: ldr      r2, [r5, r6, lsl #2]
004a228c: str      r3, [r4, #0x1c]
004a2290: add      r3, r2, #0x78
004a2294: stmib    r4, {r2, r3}
004a2298: ldr      r3, [r5, r6, lsl #2]
004a229c: str      r2, [r4]
004a22a0: add      r2, r3, #0x78
004a22a4: str      r3, [r4, #0x10]
004a22a8: str      r2, [r4, #0x18]
004a22ac: str      r3, [r4, #0x14]
004a22b0: pop      {r4, r5, r6, pc}

# _ZN14ObjectSearcher10TargetList6SearchEffRNS_11IObjectListE
004a3428: push     {r4, r5, r6, r7, r8, lr}
004a342c: sub      sp, sp, #0x18
004a3430: add      r5, sp, #0xc
004a3434: mov      ip, #0
004a3438: mov      r4, r0
004a343c: mov      r7, r1
004a3440: ldr      r0, [r0, #0x2c]
004a3444: mov      r1, r5
004a3448: mov      r6, r2
004a344c: mov      r8, r3
004a3450: str      ip, [sp, #0x14]
004a3454: str      ip, [sp, #0xc]
004a3458: str      ip, [sp, #0x10]
004a345c: bl       #0x393ae4
004a3460: ldr      r0, [r4, #0x2c]
004a3464: bl       #0x3935dc
004a3468: mov      r2, r7
004a346c: mov      r1, r0
004a3470: mov      r3, r5
004a3474: mov      r0, r4
004a3478: str      r6, [sp]
004a347c: str      r8, [sp, #4]
004a3480: bl       #0x4a2f34
004a3484: add      sp, sp, #0x18
004a3488: pop      {r4, r5, r6, r7, r8, pc}

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

# _ZN14ObjectSearcher14RoomObjectList3GetEv
004a1840: ldr      r3, [r0, #0x10]
004a1844: ldr      r0, [r3, #8]
004a1848: bx       lr

# _ZN14ObjectSearcher10TargetList18_IsGameObjectValidEP10GameObject
004a1950: push     {r4, lr}
004a1954: mov      r3, r0
004a1958: ldr      r0, [r0, #0x38]
004a195c: mov      r2, r1
004a1960: cmp      r0, #0
004a1964: beq      #0x4a1980
004a1968: cmp      r0, #1
004a196c: beq      #0x4a19a4
004a1970: cmp      r0, #3
004a1974: movne    r0, #0
004a1978: moveq    r0, #1
004a197c: pop      {r4, pc}
004a1980: mov      r0, r1
004a1984: ldr      r1, [r3, #0x2c]
004a1988: ldr      r3, [r2]
004a198c: mov      lr, pc
004a1990: ldr      pc, [r3, #0x90]
004a1994: cmp      r0, #8
004a1998: movne    r0, #0
004a199c: moveq    r0, #1
004a19a0: pop      {r4, pc}
004a19a4: mov      r0, r1
004a19a8: ldr      r1, [r3, #0x2c]
004a19ac: ldr      r3, [r2]
004a19b0: mov      lr, pc
004a19b4: ldr      pc, [r3, #0x90]
004a19b8: adds     r0, r0, #1
004a19bc: movne    r0, #1
004a19c0: pop      {r4, pc}

# _ZN14ObjectSearcher14RoomObjectList5AtEndEv
004a17fc: ldr      r3, [r0, #8]
004a1800: ldr      r0, [r0, #0xc]
004a1804: cmp      r0, r3
004a1808: movne    r0, #0
004a180c: moveq    r0, #1
004a1810: bx       lr

# _ZSt15__push_heap_auxINSt4priv15_Deque_iteratorIN14ObjectSearcher10TargetInfoESt16_Nonconst_traitsIS3_EEENS2_12TargetSorterEiS3_EvT_S8_T0_PT1_PT2_
004a1f30: push     {r4, r5, r6, r7, lr}
004a1f34: sub      sp, sp, #0x64
004a1f38: add      r4, sp, #0x40
004a1f3c: mov      r5, r1
004a1f40: mov      ip, r0
004a1f44: mov      r7, r2
004a1f48: ldm      r0, {r0, r1, r2, r3}
004a1f4c: stm      r4, {r0, r1, r2, r3}
004a1f50: add      lr, sp, #0x50
004a1f54: ldm      ip, {r0, r1, r2, r3}
004a1f58: stm      lr, {r0, r1, r2, r3}
004a1f5c: mov      r1, lr
004a1f60: mov      r0, r5
004a1f64: bl       #0x38d610
004a1f68: add      ip, sp, #0x30
004a1f6c: sub      r6, r0, #1
004a1f70: ldm      r5, {r0, r1, r2, r3}
004a1f74: stm      ip, {r0, r1, r2, r3}
004a1f78: mov      r0, ip
004a1f7c: mvn      r1, #0
004a1f80: bl       #0x38d684
004a1f84: ldr      r5, [sp, #0x30]
004a1f88: add      ip, sp, #0x1c
004a1f8c: add      lr, sp, #0x20
004a1f90: ldm      r5!, {r0, r1, r2, r3}
004a1f94: stm      ip!, {r0, r1, r2, r3}
004a1f98: ldr      r3, [r5]
004a1f9c: str      r7, [sp, #0x10]
004a1fa0: str      r3, [ip]
004a1fa4: mov      ip, sp
004a1fa8: ldm      lr, {r0, r1, r2, r3}
004a1fac: stm      ip, {r0, r1, r2, r3}
004a1fb0: mov      r0, r4
004a1fb4: mov      r1, r6
004a1fb8: ldr      r3, [sp, #0x1c]
004a1fbc: mov      r2, #0
004a1fc0: bl       #0x4a1e2c
004a1fc4: add      sp, sp, #0x64
004a1fc8: pop      {r4, r5, r6, r7, pc}

# _ZNSaIPN14ObjectSearcher10TargetInfoEE8allocateEjPKv
004a21e0: str      lr, [sp, #-4]!
004a21e4: cmn      r1, #0xc0000001
004a21e8: sub      sp, sp, #0xc
004a21ec: bhi      #0x4a2228
004a21f0: cmp      r1, #0
004a21f4: moveq    r0, r1
004a21f8: bne      #0x4a2204
004a21fc: add      sp, sp, #0xc
004a2200: ldm      sp!, {pc}
004a2204: lsl      r0, r1, #2
004a2208: cmp      r0, #0x80
004a220c: str      r0, [sp, #4]
004a2210: bhi      #0x4a2220
004a2214: add      r0, sp, #4
004a2218: bl       #0x708ec0
004a221c: b        #0x4a21fc
004a2220: bl       #0x310454
004a2224: b        #0x4a21fc
004a2228: ldr      r0, [pc, #0xc]
004a222c: add      r0, pc, r0
004a2230: bl       #0x30e0c4
004a2234: mov      r0, #1
004a2238: bl       #0x30de48
004a223c: subeq    ip, r1, r4, asr #4

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

# _ZSt14__pop_heap_auxINSt4priv15_Deque_iteratorIN14ObjectSearcher10TargetInfoESt16_Nonconst_traitsIS3_EEES3_NS2_12TargetSorterEEvT_S8_PT0_T1_
0038fa54: push     {r4, r5, r6, r7, r8, sl, lr}
0038fa58: sub      sp, sp, #0x74
0038fa5c: add      r7, sp, #0x40
0038fa60: mov      r5, r1
0038fa64: mov      sl, r3
0038fa68: ldm      r0, {r0, r1, r2, r3}
0038fa6c: stm      r7, {r0, r1, r2, r3}
0038fa70: add      r4, sp, #0x30
0038fa74: ldm      r5, {r0, r1, r2, r3}
0038fa78: add      r6, sp, #0x60
0038fa7c: stm      r4, {r0, r1, r2, r3}
0038fa80: mov      r0, r4
0038fa84: mvn      r1, #0
0038fa88: bl       #0x38d684
0038fa8c: ldm      r4, {r0, r1, r2, r3}
0038fa90: stm      r6, {r0, r1, r2, r3}
0038fa94: add      r8, sp, #0x50
0038fa98: ldm      r5, {r0, r1, r2, r3}
0038fa9c: stm      r4, {r0, r1, r2, r3}
0038faa0: mov      r0, r4
0038faa4: mvn      r1, #0
0038faa8: bl       #0x38d684
0038faac: ldm      r4, {r0, r1, r2, r3}
0038fab0: stm      r8, {r0, r1, r2, r3}
0038fab4: ldm      r5, {r0, r1, r2, r3}
0038fab8: stm      r4, {r0, r1, r2, r3}
0038fabc: mov      r0, r4
0038fac0: mvn      r1, #0
0038fac4: bl       #0x38d684
0038fac8: ldr      r4, [sp, #0x30]
0038facc: add      ip, sp, #0x1c
0038fad0: add      lr, sp, #0x20
0038fad4: ldm      r4!, {r0, r1, r2, r3}
0038fad8: stm      ip!, {r0, r1, r2, r3}
0038fadc: ldr      r3, [r4]
0038fae0: str      sl, [sp, #0x10]
0038fae4: str      r3, [ip]
0038fae8: mov      ip, sp
0038faec: ldm      lr, {r0, r1, r2, r3}
0038faf0: stm      ip, {r0, r1, r2, r3}
0038faf4: mov      ip, #0
0038faf8: mov      r0, r7
0038fafc: mov      r1, r6
0038fb00: mov      r2, r8
0038fb04: ldr      r3, [sp, #0x1c]
0038fb08: str      ip, [sp, #0x14]
0038fb0c: bl       #0x38f7cc
0038fb10: add      sp, sp, #0x74
0038fb14: pop      {r4, r5, r6, r7, r8, sl, pc}

# _ZN14ObjectSearcher10TargetList6SearchEff
003d0020: ldr      ip, [pc, #0x4c]
003d0024: push     {r4, lr}
003d0028: ldr      lr, [pc, #0x48]
003d002c: add      ip, pc, ip
003d0030: ldr      r3, [pc, #0x44]
003d0034: ldr      lr, [ip, lr]
003d0038: sub      sp, sp, #0x18
003d003c: ldr      r3, [ip, r3]
003d0040: ldr      r4, [lr, #0x38]
003d0044: add      r3, r3, #8
003d0048: add      lr, r4, #0x80
003d004c: stmib    sp, {r3, lr}
003d0050: ldr      r4, [r4, #0x80]
003d0054: add      r3, sp, #4
003d0058: str      lr, [sp, #0x10]
003d005c: mov      lr, #0
003d0060: str      r4, [sp, #0xc]
003d0064: str      lr, [sp, #0x14]
003d0068: bl       #0x4a3428
003d006c: add      sp, sp, #0x18
003d0070: pop      {r4, pc}
003d0074: subseq   r4, ip, r4, ror #20
003d0078: strdeq   r3, r4, [r0], -r4
003d007c: andeq    r3, r0, r4, ror #10

# _ZN14ObjectSearcher14RoomObjectList4NextEv
004a18ec: ldr      r2, [r0, #0x10]
004a18f0: ldr      r2, [r2]
004a18f4: str      r2, [r0, #0x10]
004a18f8: b        #0x4a18a8

# _ZN14ObjectSearcher12TargetSorter12_sortFrontalERKNS_10TargetInfoES3_
0038d5b4: push     {r4, lr}
0038d5b8: mov      r3, r0
0038d5bc: ldr      r2, [r3, #0xc]
0038d5c0: ldr      r0, [r1, #0xc]
0038d5c4: and      r2, r2, #1
0038d5c8: and      r0, r0, #1
0038d5cc: cmp      r2, r0
0038d5d0: beq      #0x38d5d8
0038d5d4: pop      {r4, pc}
0038d5d8: ldr      r0, [r3, #8]
0038d5dc: ldr      r1, [r1, #8]
0038d5e0: bl       #0x30e2f8
0038d5e4: cmp      r0, #0
0038d5e8: mov      r0, #0
0038d5ec: movne    r0, #1
0038d5f0: uxtb     r0, r0
0038d5f4: pop      {r4, pc}

# _ZN14ObjectSearcher14RoomObjectList5ResetEv
004a18fc: ldr      r1, [r0, #4]
004a1900: ldr      r2, [r1]
004a1904: str      r1, [r0, #0xc]
004a1908: str      r2, [r0, #8]
004a190c: ldr      r2, [r2, #8]
004a1910: ldr      r2, [r2]
004a1914: str      r2, [r0, #0x10]
004a1918: b        #0x4a18a8

# _ZN14ObjectSearcher14RoomObjectList16_ValidateCurrentEv
004a18a8: ldr      r3, [r0, #8]
004a18ac: ldr      ip, [r0, #0xc]
004a18b0: cmp      r3, ip
004a18b4: beq      #0x4a18e8
004a18b8: ldr      r1, [r3, #8]
004a18bc: ldr      r2, [r0, #0x10]
004a18c0: cmp      r1, r2
004a18c4: bxne     lr
004a18c8: ldr      r3, [r3]
004a18cc: cmp      ip, r3
004a18d0: str      r3, [r0, #8]
004a18d4: bxeq     lr
004a18d8: ldr      r2, [r3, #8]
004a18dc: ldr      r2, [r2]
004a18e0: str      r2, [r0, #0x10]
004a18e4: b        #0x4a18b0
004a18e8: bx       lr

# _ZN14ObjectSearcher14RoomObjectList7GetCharEv
004a1cfc: push     {r4, lr}
004a1d00: sub      sp, sp, #0x10
004a1d04: ldr      r3, [r0]
004a1d08: mov      lr, pc
004a1d0c: ldr      pc, [r3, #0x18]
004a1d10: add      r4, sp, #4
004a1d14: mov      r1, r0
004a1d18: mov      r0, r4
004a1d1c: bl       #0x33dd2c
004a1d20: mov      r0, r4
004a1d24: bl       #0x33ff54
004a1d28: add      sp, sp, #0x10
004a1d2c: pop      {r4, pc}

# _ZNSt14priority_queueIN14ObjectSearcher10TargetInfoESt5dequeIS1_SaIS1_EENS0_12TargetSorterEE4pushERKS1_
004a2440: push     {r4, r5, r6, r7, r8, sb, sl, lr}
004a2444: ldr      r3, [r0, #0x18]
004a2448: ldr      ip, [r0, #0x10]
004a244c: sub      sp, sp, #0x28
004a2450: sub      r3, r3, #0x14
004a2454: cmp      ip, r3
004a2458: mov      r4, r0
004a245c: mov      lr, r1
004a2460: beq      #0x4a24d8
004a2464: ldm      lr!, {r0, r1, r2, r3}
004a2468: stm      ip!, {r0, r1, r2, r3}
004a246c: ldr      r2, [lr]
004a2470: str      r2, [ip]
004a2474: ldr      ip, [r4, #0x10]
004a2478: add      ip, ip, #0x14
004a247c: str      ip, [r4, #0x10]
004a2480: ldmib    r4, {r5, r6, r7}
004a2484: ldr      lr, [r4]
004a2488: ldr      r8, [r4, #0x1c]
004a248c: ldr      sl, [r4, #0x18]
004a2490: ldr      sb, [r4, #0x14]
004a2494: ldr      r2, [r4, #0x28]
004a2498: mov      r4, #0
004a249c: mov      r3, r4
004a24a0: add      r0, sp, #8
004a24a4: add      r1, sp, #0x18
004a24a8: str      r7, [sp, #0x14]
004a24ac: str      r6, [sp, #0x10]
004a24b0: str      r5, [sp, #0xc]
004a24b4: str      lr, [sp, #8]
004a24b8: str      r8, [sp, #0x24]
004a24bc: str      sl, [sp, #0x20]
004a24c0: str      sb, [sp, #0x1c]
004a24c4: str      ip, [sp, #0x18]
004a24c8: str      r4, [sp]
004a24cc: bl       #0x4a1f30
004a24d0: add      sp, sp, #0x28
004a24d4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004a24d8: bl       #0x4a22b4
004a24dc: ldr      ip, [r4, #0x10]
004a24e0: b        #0x4a2480

# _ZN14ObjectSearcher12TargetSorter11_sortNoSortERKNS_10TargetInfoES3_
0038d568: mov      r0, #0
0038d56c: bx       lr

# _ZSt10__pop_heapINSt4priv15_Deque_iteratorIN14ObjectSearcher10TargetInfoESt16_Nonconst_traitsIS3_EEES3_NS2_12TargetSorterEiEvT_S8_S8_T0_T1_PT2_
0038f7cc: sub      sp, sp, #8
0038f7d0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038f7d4: sub      sp, sp, #0x74
0038f7d8: str      r3, [sp, #0x9c]
0038f7dc: ldr      lr, [r2]
0038f7e0: ldr      r5, [r0]
0038f7e4: mov      ip, r0
0038f7e8: ldr      fp, [sp, #0xb0]
0038f7ec: mov      r4, r1
0038f7f0: ldm      r5!, {r0, r1, r2, r3}
0038f7f4: stm      lr!, {r0, r1, r2, r3}
0038f7f8: ldr      r2, [r5]
0038f7fc: mov      r3, lr
0038f800: add      r5, sp, #0x50
0038f804: str      r2, [r3]
0038f808: ldm      ip, {r0, r1, r2, r3}
0038f80c: stm      r5, {r0, r1, r2, r3}
0038f810: add      lr, sp, #0x60
0038f814: ldm      ip, {r0, r1, r2, r3}
0038f818: stm      lr, {r0, r1, r2, r3}
0038f81c: mov      r1, lr
0038f820: mov      r0, r4
0038f824: bl       #0x38d610
0038f828: add      r3, sp, #0x1c
0038f82c: str      r3, [sp, #4]
0038f830: ldr      lr, [sp, #4]
0038f834: add      ip, sp, #0x9c
0038f838: mov      sb, r0
0038f83c: ldm      ip!, {r0, r1, r2, r3}
0038f840: stm      lr!, {r0, r1, r2, r3}
0038f844: ldr      r2, [ip]
0038f848: cmp      sb, #2
0038f84c: str      r2, [lr]
0038f850: ble      #0x38fa44
0038f854: mov      r8, #0
0038f858: mov      r6, #2
0038f85c: add      r4, sp, #0x30
0038f860: b        #0x38f868
0038f864: mov      r6, ip
0038f868: ldm      r5, {r0, r1, r2, r3}
0038f86c: stm      r4, {r0, r1, r2, r3}
0038f870: mov      r1, r6
0038f874: mov      r0, r4
0038f878: bl       #0x38d684
0038f87c: ldm      r5, {r0, r1, r2, r3}
0038f880: ldr      sl, [sp, #0x30]
0038f884: sub      r7, r6, #1
0038f888: stm      r4, {r0, r1, r2, r3}
0038f88c: mov      r1, r7
0038f890: mov      r0, r4
0038f894: bl       #0x38d684
0038f898: ldr      r1, [sp, #0x30]
0038f89c: mov      r0, sl
0038f8a0: blx      fp
0038f8a4: cmp      r0, #0
0038f8a8: movne    r6, r7
0038f8ac: ldm      r5, {r0, r1, r2, r3}
0038f8b0: stm      r4, {r0, r1, r2, r3}
0038f8b4: mov      r1, r8
0038f8b8: mov      r0, r4
0038f8bc: bl       #0x38d684
0038f8c0: ldm      r5, {r0, r1, r2, r3}
0038f8c4: ldr      r7, [sp, #0x30]
0038f8c8: stm      r4, {r0, r1, r2, r3}
0038f8cc: mov      r0, r4
0038f8d0: mov      r1, r6
0038f8d4: bl       #0x38d684
0038f8d8: ldr      lr, [sp, #0x30]
0038f8dc: add      ip, r6, #1
0038f8e0: lsl      ip, ip, #1
0038f8e4: ldm      lr!, {r0, r1, r2, r3}
0038f8e8: stm      r7!, {r0, r1, r2, r3}
0038f8ec: ldr      r2, [lr]
0038f8f0: cmp      sb, ip
0038f8f4: mov      r8, r6
0038f8f8: str      r2, [r7]
0038f8fc: bgt      #0x38f864
0038f900: cmp      sb, ip
0038f904: beq      #0x38f9fc
0038f908: add      r7, sp, #0x40
0038f90c: ldm      r5, {r0, r1, r2, r3}
0038f910: stm      r7, {r0, r1, r2, r3}
0038f914: ldr      ip, [sp, #4]
0038f918: add      r8, sp, #8
0038f91c: mov      lr, r8
0038f920: ldm      ip!, {r0, r1, r2, r3}
0038f924: stm      lr!, {r0, r1, r2, r3}
0038f928: ldr      r2, [ip]
0038f92c: cmp      r6, #0
0038f930: subgt    r5, r6, #1
0038f934: str      r2, [lr]
0038f938: asrgt    r5, r5, #1
0038f93c: bgt      #0x38f984
0038f940: ldm      r7, {r0, r1, r2, r3}
0038f944: stm      r4, {r0, r1, r2, r3}
0038f948: mov      r0, r4
0038f94c: mov      r1, r6
0038f950: bl       #0x38d684
0038f954: ldm      r8!, {r0, r1, r2, r3}
0038f958: ldr      ip, [sp, #0x30]
0038f95c: stm      ip!, {r0, r1, r2, r3}
0038f960: ldr      r2, [r8]
0038f964: str      r2, [ip]
0038f968: add      sp, sp, #0x74
0038f96c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0038f970: add      sp, sp, #8
0038f974: bx       lr
0038f978: sub      r3, r5, #1
0038f97c: mov      r6, r5
0038f980: asr      r5, r3, #1
0038f984: ldm      r7, {r0, r1, r2, r3}
0038f988: stm      r4, {r0, r1, r2, r3}
0038f98c: mov      r1, r5
0038f990: mov      r0, r4
0038f994: bl       #0x38d684
0038f998: mov      r1, r8
0038f99c: ldr      r0, [sp, #0x30]
0038f9a0: blx      fp
0038f9a4: cmp      r0, #0
0038f9a8: beq      #0x38f940
0038f9ac: ldm      r7, {r0, r1, r2, r3}
0038f9b0: stm      r4, {r0, r1, r2, r3}
0038f9b4: mov      r1, r6
0038f9b8: mov      r0, r4
0038f9bc: bl       #0x38d684
0038f9c0: ldm      r7, {r0, r1, r2, r3}
0038f9c4: ldr      r6, [sp, #0x30]
0038f9c8: stm      r4, {r0, r1, r2, r3}
0038f9cc: mov      r1, r5
0038f9d0: mov      r0, r4
0038f9d4: bl       #0x38d684
0038f9d8: ldr      ip, [sp, #0x30]
0038f9dc: cmp      r5, #0
0038f9e0: ldm      ip!, {r0, r1, r2, r3}
0038f9e4: stm      r6!, {r0, r1, r2, r3}
0038f9e8: ldr      r2, [ip]
0038f9ec: str      r2, [r6]
0038f9f0: bne      #0x38f978
0038f9f4: mov      r6, r5
0038f9f8: b        #0x38f940
0038f9fc: ldm      r5, {r0, r1, r2, r3}
0038fa00: stm      r4, {r0, r1, r2, r3}
0038fa04: mov      r1, r6
0038fa08: mov      r0, r4
0038fa0c: sub      r6, sb, #1
0038fa10: bl       #0x38d684
0038fa14: ldm      r5, {r0, r1, r2, r3}
0038fa18: ldr      r7, [sp, #0x30]
0038fa1c: stm      r4, {r0, r1, r2, r3}
0038fa20: mov      r0, r4
0038fa24: mov      r1, r6
0038fa28: bl       #0x38d684
0038fa2c: ldr      ip, [sp, #0x30]
0038fa30: ldm      ip!, {r0, r1, r2, r3}
0038fa34: stm      r7!, {r0, r1, r2, r3}
0038fa38: ldr      r2, [ip]
0038fa3c: str      r2, [r7]
0038fa40: b        #0x38f908
0038fa44: mov      r6, #0
0038fa48: mov      ip, #2
0038fa4c: add      r4, sp, #0x30
0038fa50: b        #0x38f900

# _ZSt13__adjust_heapINSt4priv15_Deque_iteratorIN14ObjectSearcher10TargetInfoESt16_Nonconst_traitsIS3_EEEiS3_NS2_12TargetSorterEEvT_T0_S9_T1_T2_
004a24e4: sub      sp, sp, #8
004a24e8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a24ec: add      r7, r1, #1
004a24f0: lsl      r7, r7, #1
004a24f4: sub      sp, sp, #0x44
004a24f8: cmp      r7, r2
004a24fc: str      r1, [sp, #0x1c]
004a2500: mov      sb, r2
004a2504: str      r3, [sp, #0x6c]
004a2508: mov      r5, r0
004a250c: ldr      fp, [sp, #0x80]
004a2510: movge    r8, r1
004a2514: bge      #0x4a25bc
004a2518: ldr      r8, [sp, #0x1c]
004a251c: add      r4, sp, #0x20
004a2520: ldm      r5, {r0, r1, r2, r3}
004a2524: stm      r4, {r0, r1, r2, r3}
004a2528: mov      r1, r7
004a252c: mov      r0, r4
004a2530: bl       #0x38d684
004a2534: ldm      r5, {r0, r1, r2, r3}
004a2538: ldr      sl, [sp, #0x20]
004a253c: sub      r6, r7, #1
004a2540: stm      r4, {r0, r1, r2, r3}
004a2544: mov      r1, r6
004a2548: mov      r0, r4
004a254c: bl       #0x38d684
004a2550: ldr      r1, [sp, #0x20]
004a2554: mov      r0, sl
004a2558: blx      fp
004a255c: cmp      r0, #0
004a2560: moveq    r6, r7
004a2564: ldm      r5, {r0, r1, r2, r3}
004a2568: stm      r4, {r0, r1, r2, r3}
004a256c: mov      r1, r8
004a2570: mov      r0, r4
004a2574: bl       #0x38d684
004a2578: ldm      r5, {r0, r1, r2, r3}
004a257c: ldr      r8, [sp, #0x20]
004a2580: stm      r4, {r0, r1, r2, r3}
004a2584: mov      r0, r4
004a2588: mov      r1, r6
004a258c: bl       #0x38d684
004a2590: ldr      ip, [sp, #0x20]
004a2594: add      r7, r6, #1
004a2598: lsl      r7, r7, #1
004a259c: ldm      ip!, {r0, r1, r2, r3}
004a25a0: stm      r8!, {r0, r1, r2, r3}
004a25a4: ldr      r2, [ip]
004a25a8: mov      r3, r8
004a25ac: cmp      sb, r7
004a25b0: mov      r8, r6
004a25b4: str      r2, [r3]
004a25b8: bgt      #0x4a2520
004a25bc: cmp      r7, sb
004a25c0: beq      #0x4a2608
004a25c4: add      ip, sp, #0x30
004a25c8: ldm      r5, {r0, r1, r2, r3}
004a25cc: stm      ip, {r0, r1, r2, r3}
004a25d0: add      lr, sp, #0x70
004a25d4: ldm      lr, {r0, r1, r2, r3}
004a25d8: mov      lr, sp
004a25dc: stm      lr, {r0, r1, r2, r3}
004a25e0: mov      r0, ip
004a25e4: mov      r1, r8
004a25e8: ldr      r2, [sp, #0x1c]
004a25ec: ldr      r3, [sp, #0x6c]
004a25f0: str      fp, [sp, #0x10]
004a25f4: bl       #0x4a1e2c
004a25f8: add      sp, sp, #0x44
004a25fc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a2600: add      sp, sp, #8
004a2604: bx       lr
004a2608: add      r4, sp, #0x20
004a260c: ldm      r5, {r0, r1, r2, r3}
004a2610: stm      r4, {r0, r1, r2, r3}
004a2614: mov      r1, r8
004a2618: mov      r0, r4
004a261c: sub      r8, r7, #1
004a2620: bl       #0x38d684
004a2624: ldm      r5, {r0, r1, r2, r3}
004a2628: ldr      r6, [sp, #0x20]
004a262c: stm      r4, {r0, r1, r2, r3}
004a2630: mov      r0, r4
004a2634: mov      r1, r8
004a2638: bl       #0x38d684
004a263c: ldr      ip, [sp, #0x20]
004a2640: ldm      ip!, {r0, r1, r2, r3}
004a2644: stm      r6!, {r0, r1, r2, r3}
004a2648: ldr      r2, [ip]
004a264c: str      r2, [r6]
004a2650: b        #0x4a25c4

# _ZNK7Point3DIfE8angleCosERKS0_
00312f40: push     {r4, r5, r6, r7, r8, lr}
00312f44: ldr      r7, [r0]
00312f48: mov      r3, r0
00312f4c: mov      r4, r1
00312f50: ldr      r6, [r0, #4]
00312f54: ldr      r1, [r1]
00312f58: mov      r0, r7
00312f5c: ldr      r5, [r3, #8]
00312f60: bl       #0x30ed6c
00312f64: ldr      r1, [r4, #4]
00312f68: mov      r8, r0
00312f6c: mov      r0, r6
00312f70: bl       #0x30ed6c
00312f74: mov      r1, r0
00312f78: mov      r0, r8
00312f7c: bl       #0x30eba4
00312f80: ldr      r1, [r4, #8]
00312f84: mov      r8, r0
00312f88: mov      r0, r5
00312f8c: bl       #0x30ed6c
00312f90: mov      r1, r0
00312f94: mov      r0, r8
00312f98: bl       #0x30eba4
00312f9c: mov      r1, r7
00312fa0: mov      r8, r0
00312fa4: mov      r0, r7
00312fa8: bl       #0x30ed6c
00312fac: mov      r1, r6
00312fb0: mov      r7, r0
00312fb4: mov      r0, r6
00312fb8: bl       #0x30ed6c
00312fbc: mov      r1, r0
00312fc0: mov      r0, r7
00312fc4: bl       #0x30eba4
00312fc8: mov      r1, r5
00312fcc: mov      r6, r0
00312fd0: mov      r0, r5
00312fd4: bl       #0x30ed6c
00312fd8: mov      r1, r0
00312fdc: mov      r0, r6
00312fe0: bl       #0x30eba4
00312fe4: bl       #0x30e124
00312fe8: mov      r5, r0
00312fec: ldr      r0, [r4]
00312ff0: ldr      r7, [r4, #4]
00312ff4: ldr      r6, [r4, #8]
00312ff8: mov      r1, r0
00312ffc: bl       #0x30ed6c
00313000: mov      r1, r7
00313004: mov      r4, r0
00313008: mov      r0, r7
0031300c: bl       #0x30ed6c
00313010: mov      r1, r0
00313014: mov      r0, r4
00313018: bl       #0x30eba4
0031301c: mov      r1, r6
00313020: mov      r4, r0
00313024: mov      r0, r6
00313028: bl       #0x30ed6c
0031302c: mov      r1, r0
00313030: mov      r0, r4
00313034: bl       #0x30eba4
00313038: bl       #0x30e124
0031303c: mov      r1, r0
00313040: mov      r0, r5
00313044: bl       #0x30ed6c
00313048: mov      r1, r0
0031304c: mov      r0, r8
00313050: bl       #0x30ec94
00313054: pop      {r4, r5, r6, r7, r8, pc}

# _ZNSt14priority_queueIN14ObjectSearcher10TargetInfoESt5dequeIS1_SaIS1_EENS0_12TargetSorterEE3popEv
0038fb18: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0038fb1c: ldm      r0, {r5, r6, r7}
0038fb20: add      r8, r0, #0x14
0038fb24: ldm      r8, {r8, ip, lr}
0038fb28: ldr      sl, [r0, #0x10]
0038fb2c: ldr      sb, [r0, #0xc]
0038fb30: sub      sp, sp, #0x20
0038fb34: ldr      r3, [r0, #0x28]
0038fb38: mov      r4, r0
0038fb3c: add      r1, sp, #0x10
0038fb40: mov      r0, sp
0038fb44: mov      r2, #0
0038fb48: stm      sp, {r5, r6, r7, sb}
0038fb4c: str      lr, [sp, #0x1c]
0038fb50: str      ip, [sp, #0x18]
0038fb54: str      r8, [sp, #0x14]
0038fb58: str      sl, [sp, #0x10]
0038fb5c: bl       #0x38fa54
0038fb60: ldr      r0, [r4, #0x10]
0038fb64: ldr      r3, [r4, #0x14]
0038fb68: cmp      r0, r3
0038fb6c: subne    r0, r0, #0x14
0038fb70: strne    r0, [r4, #0x10]
0038fb74: beq      #0x38fb80
0038fb78: add      sp, sp, #0x20
0038fb7c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0038fb80: cmp      r0, #0
0038fb84: beq      #0x38fb90
0038fb88: mov      r1, #0x78
0038fb8c: bl       #0x708f00
0038fb90: ldr      r3, [r4, #0x1c]
0038fb94: sub      r2, r3, #4
0038fb98: str      r2, [r4, #0x1c]
0038fb9c: ldr      r3, [r3, #-4]
0038fba0: add      r1, r3, #0x64
0038fba4: add      r2, r3, #0x78
0038fba8: str      r1, [r4, #0x10]
0038fbac: str      r2, [r4, #0x18]
0038fbb0: str      r3, [r4, #0x14]
0038fbb4: b        #0x38fb78

# _ZN14ObjectSearcher10TargetListC1EP10GameObjectiii
004a2730: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
004a2734: ldr      r6, [pc, #0x10c]
004a2738: ldr      r7, [pc, #0x10c]
004a273c: mov      r5, #0
004a2740: add      r6, pc, r6
004a2744: str      r5, [r0]
004a2748: str      r5, [r0, #4]
004a274c: str      r5, [r0, #8]
004a2750: str      r5, [r0, #0xc]
004a2754: str      r5, [r0, #0x10]
004a2758: str      r5, [r0, #0x14]
004a275c: str      r5, [r0, #0x18]
004a2760: str      r5, [r0, #0x1c]
004a2764: str      r5, [r0, #0x20]
004a2768: str      r5, [r0, #0x24]
004a276c: mov      r4, r0
004a2770: mov      r8, r2
004a2774: mov      sl, r3
004a2778: mov      fp, r1
004a277c: ldr      sb, [sp, #0x28]
004a2780: bl       #0x4a2240
004a2784: ldr      r2, [r6, r7]
004a2788: mov      r3, r4
004a278c: str      r8, [r4, #0x34]
004a2790: str      r2, [r4, #0x28]
004a2794: str      sl, [r4, #0x38]
004a2798: str      r5, [r4, #0x2c]
004a279c: str      r5, [r4, #0x30]
004a27a0: str      r5, [r4, #0x40]
004a27a4: strb     r5, [r3, #0x3c]!
004a27a8: ldr      r1, [r4, #0x10]
004a27ac: ldr      r2, [r4]
004a27b0: str      r3, [r4, #0x48]
004a27b4: str      r5, [r4, #0x4c]
004a27b8: cmp      r1, r2
004a27bc: str      r3, [r4, #0x44]
004a27c0: beq      #0x4a27dc
004a27c4: mov      r0, r4
004a27c8: bl       #0x38fb18
004a27cc: ldr      r2, [r4, #0x10]
004a27d0: ldr      r3, [r4]
004a27d4: cmp      r2, r3
004a27d8: bne      #0x4a27c4
004a27dc: cmp      sb, #1
004a27e0: beq      #0x4a2808
004a27e4: cmp      sb, #2
004a27e8: beq      #0x4a2828
004a27ec: ldr      r3, [r6, r7]
004a27f0: mov      r0, r4
004a27f4: mov      r1, fp
004a27f8: str      r3, [r4, #0x28]
004a27fc: bl       #0x4a191c
004a2800: mov      r0, r4
004a2804: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2808: ldr      r3, [pc, #0x40]
004a280c: mov      r0, r4
004a2810: mov      r1, fp
004a2814: ldr      r3, [r6, r3]
004a2818: str      r3, [r4, #0x28]
004a281c: bl       #0x4a191c
004a2820: mov      r0, r4
004a2824: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2828: ldr      r3, [pc, #0x24]
004a282c: mov      r0, r4
004a2830: mov      r1, fp
004a2834: ldr      r3, [r6, r3]
004a2838: str      r3, [r4, #0x28]
004a283c: bl       #0x4a191c
004a2840: mov      r0, r4
004a2844: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
004a2848: subeq    r2, pc, r0, asr r3
004a284c: andeq    r2, r0, r4, ror ip
004a2850: andeq    r4, r0, r4, asr #21
004a2854: ldrdeq   r1, r2, [r0], -r4

# _ZNK7Point3DIfE5angleERKS0_
00313058: push     {r4, lr}
0031305c: bl       #0x312f40
00313060: pop      {r4, lr}
00313064: b        #0x30e3dc

# _ZNSt5dequeIN14ObjectSearcher10TargetInfoESaIS1_EE18_M_push_back_aux_vERKS1_
004a22b4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
004a22b8: ldr      sl, [r0, #0x1c]
004a22bc: ldr      r2, [r0, #0x20]
004a22c0: ldr      r3, [r0, #0x24]
004a22c4: mov      r5, r1
004a22c8: rsb      r1, r2, sl
004a22cc: sub      r1, r3, r1, asr #2
004a22d0: cmp      r1, #1
004a22d4: mov      r4, r0
004a22d8: bls      #0x4a2320
004a22dc: add      r0, r4, #0x24
004a22e0: bl       #0x4a1e0c
004a22e4: str      r0, [sl, #4]
004a22e8: ldr      ip, [r4, #0x10]
004a22ec: ldm      r5!, {r0, r1, r2, r3}
004a22f0: stm      ip!, {r0, r1, r2, r3}
004a22f4: ldr      r2, [r5]
004a22f8: str      r2, [ip]
004a22fc: ldr      r3, [r4, #0x1c]
004a2300: add      r2, r3, #4
004a2304: str      r2, [r4, #0x1c]
004a2308: ldr      r3, [r3, #4]
004a230c: add      r2, r3, #0x78
004a2310: str      r3, [r4, #0x10]
004a2314: str      r2, [r4, #0x18]
004a2318: str      r3, [r4, #0x14]
004a231c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
004a2320: ldr      r1, [r0, #0xc]
004a2324: rsb      r7, r1, sl
004a2328: asr      r7, r7, #2
004a232c: add      r7, r7, #1
004a2330: add      sb, r7, #1
004a2334: cmp      r3, sb, lsl #1
004a2338: bls      #0x4a2368
004a233c: rsb      r6, sb, r3
004a2340: lsr      r6, r6, #1
004a2344: add      r6, r2, r6, lsl #2
004a2348: cmp      r1, r6
004a234c: bls      #0x4a240c
004a2350: add      r2, sl, #4
004a2354: subs     r2, r2, r1
004a2358: beq      #0x4a23d8
004a235c: mov      r0, r6
004a2360: bl       #0x30df38
004a2364: b        #0x4a23d8
004a2368: cmp      r3, #0
004a236c: movne    r2, r3
004a2370: moveq    r2, #1
004a2374: add      r8, r3, #2
004a2378: add      r8, r8, r2
004a237c: mov      r1, r8
004a2380: mov      r2, #0
004a2384: add      r0, r0, #0x20
004a2388: bl       #0x4a21e0
004a238c: ldr      r2, [r4, #0x1c]
004a2390: ldr      r1, [r4, #0xc]
004a2394: rsb      r6, sb, r8
004a2398: lsr      r6, r6, #1
004a239c: add      r2, r2, #4
004a23a0: subs     r2, r2, r1
004a23a4: mov      sl, r0
004a23a8: add      r6, r0, r6, lsl #2
004a23ac: bne      #0x4a2434
004a23b0: ldr      r0, [r4, #0x20]
004a23b4: ldr      r1, [r4, #0x24]
004a23b8: cmp      r0, #0
004a23bc: beq      #0x4a23d0
004a23c0: lsl      r1, r1, #2
004a23c4: cmp      r1, #0x80
004a23c8: bhi      #0x4a242c
004a23cc: bl       #0x708f00
004a23d0: str      sl, [r4, #0x20]
004a23d4: str      r8, [r4, #0x24]
004a23d8: str      r6, [r4, #0xc]
004a23dc: ldr      r3, [r6]
004a23e0: sub      r7, r7, #1
004a23e4: add      sl, r6, r7, lsl #2
004a23e8: add      r2, r3, #0x78
004a23ec: str      r2, [r4, #8]
004a23f0: str      r3, [r4, #4]
004a23f4: str      sl, [r4, #0x1c]
004a23f8: ldr      r3, [r6, r7, lsl #2]
004a23fc: add      r2, r3, #0x78
004a2400: str      r2, [r4, #0x18]
004a2404: str      r3, [r4, #0x14]
004a2408: b        #0x4a22dc
004a240c: add      r2, sl, #4
004a2410: rsb      r2, r1, r2
004a2414: cmp      r2, #0
004a2418: ble      #0x4a23d8
004a241c: add      r0, r6, r7, lsl #2
004a2420: rsb      r0, r2, r0
004a2424: bl       #0x30df38
004a2428: b        #0x4a23d8
004a242c: bl       #0x310440
004a2430: b        #0x4a23d0
004a2434: mov      r0, r6
004a2438: bl       #0x30df38
004a243c: b        #0x4a23b0

# _ZN14ObjectSearcher12TargetSorter12_sortClosestERKNS_10TargetInfoES3_
0038d570: push     {r4, lr}
0038d574: mov      r3, r0
0038d578: ldr      r2, [r3, #0xc]
0038d57c: ldr      r0, [r1, #0xc]
0038d580: and      r2, r2, #1
0038d584: and      r0, r0, #1
0038d588: cmp      r2, r0
0038d58c: beq      #0x38d594
0038d590: pop      {r4, pc}
0038d594: ldr      r0, [r3, #4]
0038d598: ldr      r1, [r1, #4]
0038d59c: bl       #0x30e2f8
0038d5a0: cmp      r0, #0
0038d5a4: mov      r0, #0
0038d5a8: movne    r0, #1
0038d5ac: uxtb     r0, r0
0038d5b0: pop      {r4, pc}

# _ZN14ObjectSearcher10TargetList17_IsCharacterValidEP9Character
004a1ab8: push     {r4, r5, r6, lr}
004a1abc: ldr      r3, [r0, #0x30]
004a1ac0: mov      r4, r0
004a1ac4: mov      r5, r1
004a1ac8: cmp      r3, #0
004a1acc: beq      #0x4a1c0c
004a1ad0: movw     r2, #0x1314
004a1ad4: ldr      r2, [r3, r2]
004a1ad8: movw     r3, #0x1310
004a1adc: ldr      r3, [r1, r3]
004a1ae0: cmp      r2, r3
004a1ae4: bge      #0x4a1af0
004a1ae8: mov      r0, #0
004a1aec: pop      {r4, r5, r6, pc}
004a1af0: ldr      r3, [r0, #0x34]
004a1af4: cmn      r3, #0x80000001
004a1af8: beq      #0x4a1c0c
004a1afc: tst      r3, #0x80
004a1b00: bne      #0x4a1c14
004a1b04: tst      r3, #0x10
004a1b08: bne      #0x4a1c34
004a1b0c: tst      r3, #0x20
004a1b10: bne      #0x4a1c4c
004a1b14: tst      r3, #0x40
004a1b18: beq      #0x4a1b28
004a1b1c: ldrb     r2, [r5, #0x2fa]
004a1b20: cmp      r2, #0
004a1b24: bne      #0x4a1c0c
004a1b28: tst      r3, #1
004a1b2c: bne      #0x4a1ba8
004a1b30: tst      r3, #2
004a1b34: bne      #0x4a1c64
004a1b38: tst      r3, #4
004a1b3c: bne      #0x4a1ca8
004a1b40: tst      r3, #8
004a1b44: beq      #0x4a1ae8
004a1b48: ldr      r3, [r5]
004a1b4c: mov      r0, r5
004a1b50: mov      lr, pc
004a1b54: ldr      pc, [r3, #0x34]
004a1b58: cmp      r0, #0
004a1b5c: beq      #0x4a1ae8
004a1b60: ldr      r0, [r4, #0x30]
004a1b64: mov      r1, r5
004a1b68: add      r0, r0, #0x3c8
004a1b6c: bl       #0x3d511c
004a1b70: cmp      r0, #0
004a1b74: beq      #0x4a1ae8
004a1b78: ldr      r0, [r4, #0x30]
004a1b7c: add      r0, r0, #0x4f0
004a1b80: add      r0, r0, #0xc
004a1b84: bl       #0x3c01ac
004a1b88: cmp      r0, #0xf
004a1b8c: beq      #0x4a1ae8
004a1b90: add      r0, r5, #0x4f0
004a1b94: add      r0, r0, #0xc
004a1b98: bl       #0x3c01ac
004a1b9c: subs     r0, r0, #0x10
004a1ba0: movne    r0, #1
004a1ba4: pop      {r4, r5, r6, pc}
004a1ba8: ldr      r3, [r5]
004a1bac: mov      r0, r5
004a1bb0: mov      lr, pc
004a1bb4: ldr      pc, [r3, #0x34]
004a1bb8: cmp      r0, #0
004a1bbc: bne      #0x4a1ca0
004a1bc0: ldr      r0, [r4, #0x30]
004a1bc4: mov      r1, r5
004a1bc8: add      r0, r0, #0x3c8
004a1bcc: bl       #0x3d574c
004a1bd0: cmp      r0, #0
004a1bd4: beq      #0x4a1ca0
004a1bd8: ldr      r3, [r5]
004a1bdc: mov      r0, r5
004a1be0: mov      lr, pc
004a1be4: ldr      pc, [r3, #0x28]
004a1be8: cmp      r0, #0
004a1bec: beq      #0x4a1c0c
004a1bf0: ldr      r3, [r4, #0x30]
004a1bf4: mov      r0, r3
004a1bf8: ldr      r3, [r3]
004a1bfc: mov      lr, pc
004a1c00: ldr      pc, [r3, #0x28]
004a1c04: cmp      r0, #0
004a1c08: bne      #0x4a1ca0
004a1c0c: mov      r0, #1
004a1c10: pop      {r4, r5, r6, pc}
004a1c14: ldr      r3, [r1]
004a1c18: mov      r0, r1
004a1c1c: mov      lr, pc
004a1c20: ldr      pc, [r3, #0x28]
004a1c24: cmp      r0, #0
004a1c28: ldreq    r3, [r4, #0x34]
004a1c2c: beq      #0x4a1b04
004a1c30: b        #0x4a1c0c
004a1c34: mov      r0, r5
004a1c38: bl       #0x3a30c4
004a1c3c: cmp      r0, #0
004a1c40: ldreq    r3, [r4, #0x34]
004a1c44: beq      #0x4a1b0c
004a1c48: b        #0x4a1c0c
004a1c4c: mov      r0, r5
004a1c50: bl       #0x3a30f4
004a1c54: cmp      r0, #0
004a1c58: ldreq    r3, [r4, #0x34]
004a1c5c: beq      #0x4a1b14
004a1c60: b        #0x4a1c0c
004a1c64: ldr      r3, [r5]
004a1c68: mov      r0, r5
004a1c6c: mov      lr, pc
004a1c70: ldr      pc, [r3, #0x34]
004a1c74: cmp      r0, #0
004a1c78: beq      #0x4a1c84
004a1c7c: ldr      r3, [r4, #0x34]
004a1c80: b        #0x4a1b38
004a1c84: ldr      r0, [r4, #0x30]
004a1c88: mov      r1, r5
004a1c8c: add      r0, r0, #0x3c8
004a1c90: bl       #0x3d5a98
004a1c94: cmp      r0, #0
004a1c98: bne      #0x4a1c0c
004a1c9c: b        #0x4a1c7c
004a1ca0: ldr      r3, [r4, #0x34]
004a1ca4: b        #0x4a1b30
004a1ca8: ldr      r0, [r4, #0x30]
004a1cac: mov      r1, r5
004a1cb0: add      r0, r0, #0x3c8
004a1cb4: bl       #0x3d511c
004a1cb8: cmp      r0, #0
004a1cbc: ldreq    r3, [r4, #0x34]
004a1cc0: beq      #0x4a1b40
004a1cc4: b        #0x4a1c0c

# _ZSt11__push_heapINSt4priv15_Deque_iteratorIN14ObjectSearcher10TargetInfoESt16_Nonconst_traitsIS3_EEEiS3_NS2_12TargetSorterEEvT_T0_S9_T1_T2_
004a1e2c: sub      sp, sp, #8
004a1e30: push     {r4, r5, r6, r7, r8, sb, sl, lr}
004a1e34: cmp      r1, r2
004a1e38: sub      sp, sp, #0x10
004a1e3c: mov      r6, r1
004a1e40: mov      sl, r2
004a1e44: str      r3, [sp, #0x34]
004a1e48: mov      r5, r0
004a1e4c: ldr      sb, [sp, #0x48]
004a1e50: movle    r4, sp
004a1e54: addle    r8, sp, #0x34
004a1e58: bgt      #0x4a1e94
004a1e5c: ldm      r5, {r0, r1, r2, r3}
004a1e60: stm      r4, {r0, r1, r2, r3}
004a1e64: mov      r0, sp
004a1e68: mov      r1, r6
004a1e6c: bl       #0x38d684
004a1e70: ldm      r8!, {r0, r1, r2, r3}
004a1e74: ldr      ip, [sp]
004a1e78: stm      ip!, {r0, r1, r2, r3}
004a1e7c: ldr      r2, [r8]
004a1e80: str      r2, [ip]
004a1e84: add      sp, sp, #0x10
004a1e88: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
004a1e8c: add      sp, sp, #8
004a1e90: bx       lr
004a1e94: sub      r7, r1, #1
004a1e98: add      r7, r7, r7, lsr #31
004a1e9c: mov      r4, sp
004a1ea0: asr      r7, r7, #1
004a1ea4: add      r8, sp, #0x34
004a1ea8: ldm      r5, {r0, r1, r2, r3}
004a1eac: stm      r4, {r0, r1, r2, r3}
004a1eb0: mov      r1, r7
004a1eb4: mov      r0, sp
004a1eb8: bl       #0x38d684
004a1ebc: mov      r1, r8
004a1ec0: ldr      r0, [sp]
004a1ec4: blx      sb
004a1ec8: cmp      r0, #0
004a1ecc: beq      #0x4a1e5c
004a1ed0: ldm      r5, {r0, r1, r2, r3}
004a1ed4: stm      r4, {r0, r1, r2, r3}
004a1ed8: mov      r1, r6
004a1edc: mov      r0, sp
004a1ee0: bl       #0x38d684
004a1ee4: ldm      r5, {r0, r1, r2, r3}
004a1ee8: ldr      r6, [sp]
004a1eec: stm      r4, {r0, r1, r2, r3}
004a1ef0: mov      r1, r7
004a1ef4: mov      r0, sp
004a1ef8: bl       #0x38d684
004a1efc: ldr      ip, [sp]
004a1f00: cmp      sl, r7
004a1f04: ldm      ip!, {r0, r1, r2, r3}
004a1f08: stm      r6!, {r0, r1, r2, r3}
004a1f0c: ldr      r2, [ip]
004a1f10: str      r2, [r6]
004a1f14: movge    r6, r7
004a1f18: bge      #0x4a1e5c
004a1f1c: sub      r3, r7, #1
004a1f20: add      r3, r3, r3, lsr #31
004a1f24: mov      r6, r7
004a1f28: asr      r7, r3, #1
004a1f2c: b        #0x4a1ea8
