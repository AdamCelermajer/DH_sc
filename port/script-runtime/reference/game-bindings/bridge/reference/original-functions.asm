
# __aeabi_f2uiz
008be2a0: lsls     r2, r0, #1
008be2a4: bhs      #0x8be2cc
008be2a8: cmp      r2, #0x7f000000
008be2ac: blo      #0x8be2cc
008be2b0: mov      r3, #0x9e
008be2b4: subs     r2, r3, r2, lsr #24
008be2b8: bmi      #0x8be2d4
008be2bc: lsl      r3, r0, #8
008be2c0: orr      r3, r3, #0x80000000
008be2c4: lsr      r0, r3, r2
008be2c8: bx       lr
008be2cc: mov      r0, #0
008be2d0: bx       lr
008be2d4: cmn      r2, #0x61
008be2d8: bne      #0x8be2e4
008be2dc: lsls     r2, r0, #9
008be2e0: bne      #0x8be2ec
008be2e4: mvn      r0, #0
008be2e8: bx       lr
008be2ec: mov      r0, #0
008be2f0: bx       lr

# __fixunssfsi
008be2a0: lsls     r2, r0, #1
008be2a4: bhs      #0x8be2cc
008be2a8: cmp      r2, #0x7f000000
008be2ac: blo      #0x8be2cc
008be2b0: mov      r3, #0x9e
008be2b4: subs     r2, r3, r2, lsr #24
008be2b8: bmi      #0x8be2d4
008be2bc: lsl      r3, r0, #8
008be2c0: orr      r3, r3, #0x80000000
008be2c4: lsr      r0, r3, r2
008be2c8: bx       lr
008be2cc: mov      r0, #0
008be2d0: bx       lr
008be2d4: cmn      r2, #0x61
008be2d8: bne      #0x8be2e4
008be2dc: lsls     r2, r0, #9
008be2e0: bne      #0x8be2ec
008be2e4: mvn      r0, #0
008be2e8: bx       lr
008be2ec: mov      r0, #0
008be2f0: bx       lr

# _ZN3sfc6script3lua5ValueC1Ei
0037ca9c: ldr      r2, [pc, #0x78]
0037caa0: ldr      ip, [pc, #0x78]
0037caa4: mov      r3, r0
0037caa8: add      r2, pc, r2
0037caac: ldr      ip, [r2, ip]
0037cab0: push     {r4, r5, r6, lr}
0037cab4: add      ip, ip, #8
0037cab8: mov      r4, r0
0037cabc: str      ip, [r3], #0xc
0037cac0: mov      r5, r1
0037cac4: mov      r0, r3
0037cac8: mov      r1, #0x10
0037cacc: str      r3, [r4, #0x1c]
0037cad0: str      r3, [r4, #0x20]
0037cad4: bl       #0x31167c
0037cad8: ldr      r2, [r4, #0x1c]
0037cadc: add      r3, r4, #0x24
0037cae0: mov      r6, #0
0037cae4: strb     r6, [r2]
0037cae8: mov      r0, r3
0037caec: str      r3, [r4, #0x64]
0037caf0: str      r3, [r4, #0x68]
0037caf4: bl       #0x37be44
0037caf8: ldr      r3, [r4, #0x64]
0037cafc: mov      r0, r5
0037cb00: str      r6, [r3]
0037cb04: bl       #0x30e964
0037cb08: mov      r1, r0
0037cb0c: mov      r0, r4
0037cb10: bl       #0x31b5e8
0037cb14: mov      r0, r4
0037cb18: pop      {r4, r5, r6, pc}
0037cb1c: rsbeq    r7, r1, r8, ror #31
0037cb20: muleq    r0, r8, r7

# _ZN3sfc6script3lua6Binder18__functionCallbackEP9lua_State
0031a250: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0031a254: ldr      r4, [pc, #0x194]
0031a258: ldr      sb, [pc, #0x194]
0031a25c: sub      sp, sp, #0x4c
0031a260: add      r4, pc, r4
0031a264: ldr      r3, [r4, sb]
0031a268: add      r6, sp, #0x14
0031a26c: mov      r1, r0
0031a270: ldr      r3, [r3]
0031a274: mov      r7, r0
0031a278: mov      r2, #0
0031a27c: mov      r0, r6
0031a280: add      sl, sp, #0xc
0031a284: str      r3, [sp, #0x44]
0031a288: add      r5, sp, #0x1c
0031a28c: bl       #0x3196ec
0031a290: mov      r2, #2
0031a294: mov      r1, r7
0031a298: mov      r0, sl
0031a29c: bl       #0x3196ec
0031a2a0: mov      r0, r5
0031a2a4: bl       #0x31b434
0031a2a8: ldr      r8, [sp, #0x10]
0031a2ac: ldm      r8, {r0, r3}
0031a2b0: rsb      r3, r0, r3
0031a2b4: asr      r3, r3, #4
0031a2b8: add      r2, r3, r3, lsl #3
0031a2bc: add      r2, r2, r2, lsl #6
0031a2c0: add      r2, r3, r2, lsl #3
0031a2c4: add      r2, r2, r2, lsl #15
0031a2c8: add      r3, r3, r2, lsl #3
0031a2cc: cmp      r3, #0
0031a2d0: bne      #0x31a2e4
0031a2d4: ldr      r0, [pc, #0x11c]
0031a2d8: add      r0, pc, r0
0031a2dc: bl       #0x708eb0
0031a2e0: ldr      r0, [r8]
0031a2e4: bl       #0x31b580
0031a2e8: ldr      fp, [sp, #0x10]
0031a2ec: mov      r8, r0
0031a2f0: ldm      fp, {r0, r3}
0031a2f4: rsb      r3, r0, r3
0031a2f8: asr      r3, r3, #4
0031a2fc: add      r2, r3, r3, lsl #3
0031a300: add      r2, r2, r2, lsl #6
0031a304: add      r2, r3, r2, lsl #3
0031a308: add      r2, r2, r2, lsl #15
0031a30c: add      r3, r3, r2, lsl #3
0031a310: rsb      r3, r3, #0
0031a314: cmp      r3, #1
0031a318: bhi      #0x31a32c
0031a31c: ldr      r0, [pc, #0xd8]
0031a320: add      r0, pc, r0
0031a324: bl       #0x708eb0
0031a328: ldr      r0, [fp]
0031a32c: add      r0, r0, #0x70
0031a330: bl       #0x31b580
0031a334: cmp      r8, #0
0031a338: mov      fp, r0
0031a33c: beq      #0x31a398
0031a340: mov      r2, fp
0031a344: mov      r0, r6
0031a348: mov      r1, r5
0031a34c: blx      r8
0031a350: mov      r1, r7
0031a354: mov      r0, r5
0031a358: bl       #0x31b308
0031a35c: mov      r7, r0
0031a360: mov      r0, r5
0031a364: bl       #0x31b398
0031a368: mov      r0, sl
0031a36c: bl       #0x319228
0031a370: mov      r0, r6
0031a374: bl       #0x319228
0031a378: ldr      r3, [r4, sb]
0031a37c: ldr      r2, [sp, #0x44]
0031a380: mov      r0, r7
0031a384: ldr      r3, [r3]
0031a388: cmp      r2, r3
0031a38c: bne      #0x31a3ec
0031a390: add      sp, sp, #0x4c
0031a394: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031a398: ldr      r3, [pc, #0x60]
0031a39c: ldr      r3, [r4, r3]
0031a3a0: ldr      r3, [r3]
0031a3a4: cmp      r3, #2
0031a3a8: streq    r8, [r8]
0031a3ac: beq      #0x31a340
0031a3b0: cmp      r3, #1
0031a3b4: bne      #0x31a340
0031a3b8: ldr      r0, [pc, #0x44]
0031a3bc: ldr      r1, [pc, #0x44]
0031a3c0: ldr      r2, [pc, #0x44]
0031a3c4: ldr      r0, [r4, r0]
0031a3c8: ldr      r3, [pc, #0x40]
0031a3cc: mov      ip, #0x20
0031a3d0: add      r1, pc, r1
0031a3d4: add      r2, pc, r2
0031a3d8: add      r3, pc, r3
0031a3dc: add      r0, r0, #0xa8
0031a3e0: str      ip, [sp]
0031a3e4: bl       #0x30e004
0031a3e8: b        #0x31a340
0031a3ec: bl       #0x30e310
0031a3f0: rsbeq    sl, r7, r0, lsr r8
0031a3f4: andeq    r4, r0, ip, lsr #1

# _ZN3sfc6script3lua9ArgumentsC1EP9lua_Statei
003196ec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003196f0: ldr      r3, [pc, #0x1d4]
003196f4: sub      sp, sp, #0xfc
003196f8: ldr      sb, [pc, #0x1d0]
003196fc: str      r3, [sp, #0xc]
00319700: ldr      lr, [sp, #0xc]
00319704: ldr      r3, [pc, #0x1c8]
00319708: add      sb, pc, sb
0031970c: ldr      ip, [sb, lr]
00319710: ldr      r3, [sb, r3]
00319714: mov      r6, r0
00319718: ldr      r0, [ip]
0031971c: add      r3, r3, #8
00319720: str      r3, [r6]
00319724: mov      r8, r2
00319728: str      r0, [sp, #0xf4]
0031972c: mov      r7, r1
00319730: bl       #0x31ce84
00319734: cmp      r8, #0
00319738: mov      fp, r0
0031973c: str      r0, [r6, #4]
00319740: ble      #0x31980c
00319744: ldr      r3, [pc, #0x18c]
00319748: mov      r4, #1
0031974c: add      r5, sp, #0x84
00319750: add      r3, pc, r3
00319754: str      r3, [sp, #8]
00319758: mov      sl, #0x70
0031975c: b        #0x319764
00319760: ldr      fp, [r6, #4]
00319764: mov      r0, r5
00319768: bl       #0x3194e0
0031976c: mov      r0, fp
00319770: mov      r1, r5
00319774: bl       #0x3195c0
00319778: mov      r0, r5
0031977c: bl       #0x3193e8
00319780: ldr      r3, [r6, #4]
00319784: ldm      r3, {r0, r2}
00319788: rsb      r2, r0, r2
0031978c: asr      r2, r2, #4
00319790: add      fp, r2, r2, lsl #3
00319794: add      fp, fp, fp, lsl #6
00319798: add      fp, r2, fp, lsl #3
0031979c: add      fp, fp, fp, lsl #15
003197a0: add      fp, r2, fp, lsl #3
003197a4: rsb      fp, fp, #0
003197a8: subs     fp, fp, #1
003197ac: bhs      #0x3197c4
003197b0: ldr      r0, [sp, #8]
003197b4: str      r3, [sp, #4]
003197b8: bl       #0x708eb0
003197bc: ldr      r3, [sp, #4]
003197c0: ldr      r0, [r3]
003197c4: movw     r2, #0xd8ee
003197c8: movt     r2, #0xffff
003197cc: rsb      r2, r4, r2
003197d0: mla      r0, sl, fp, r0
003197d4: add      r4, r4, #1
003197d8: mov      r1, r7
003197dc: bl       #0x31c9c8
003197e0: cmp      r8, r4
003197e4: bge      #0x319760
003197e8: ldr      r2, [sp, #0xc]
003197ec: mov      r0, r6
003197f0: ldr      r3, [sb, r2]
003197f4: ldr      r2, [sp, #0xf4]
003197f8: ldr      r3, [r3]
003197fc: cmp      r2, r3
00319800: bne      #0x3198c8
00319804: add      sp, sp, #0xfc
00319808: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031980c: mov      r0, r7
00319810: bl       #0x84b12c
00319814: rsb      r8, r8, #1
00319818: cmp      r0, r8
0031981c: mov      r5, r0
00319820: blt      #0x3198b8
00319824: ldr      r3, [pc, #0xb0]
00319828: add      r4, sp, #0x14
0031982c: mov      sl, #0x70
00319830: add      r3, pc, r3
00319834: str      r3, [sp, #8]
00319838: ldr      fp, [r6, #4]
0031983c: mov      r0, r4
00319840: bl       #0x3194e0
00319844: mov      r0, fp
00319848: mov      r1, r4
0031984c: bl       #0x3195c0
00319850: mov      r0, r4
00319854: bl       #0x3193e8
00319858: ldr      fp, [r6, #4]
0031985c: ldm      fp, {r0, r2}
00319860: rsb      r2, r0, r2
00319864: asr      r2, r2, #4
00319868: add      r3, r2, r2, lsl #3
0031986c: add      r3, r3, r3, lsl #6
00319870: add      r3, r2, r3, lsl #3
00319874: add      r3, r3, r3, lsl #15
00319878: add      r3, r2, r3, lsl #3
0031987c: rsb      r3, r3, #0
00319880: subs     r3, r3, #1
00319884: bhs      #0x31989c
00319888: ldr      r0, [sp, #8]
0031988c: str      r3, [sp, #4]
00319890: bl       #0x708eb0
00319894: ldr      r0, [fp]
00319898: ldr      r3, [sp, #4]
0031989c: mov      r2, r8
003198a0: mla      r0, sl, r3, r0
003198a4: add      r8, r8, #1
003198a8: mov      r1, r7
003198ac: bl       #0x31c9c8
003198b0: cmp      r5, r8
003198b4: bge      #0x319838
003198b8: mov      r0, r7
003198bc: mvn      r1, r5
003198c0: bl       #0x84b140
003198c4: b        #0x3197e8
003198c8: bl       #0x30e310
003198cc: andeq    r4, r0, ip, lsr #1
003198d0: rsbeq    fp, r7, r8, lsl #7
003198d4: andeq    r2, r0, r0, ror #6
003198d8: subseq   r4, sl, r8, lsl sp
003198dc: subseq   r4, sl, r8, lsr ip

# _ZN6CharAI7OnTimerEPv
003d0c10: push     {r4, lr}
003d0c14: ldr      r3, [r0, #0x1c]
003d0c18: cmp      r3, #0
003d0c1c: beq      #0x3d0c30
003d0c20: mov      r0, r3
003d0c24: ldr      r3, [r3]
003d0c28: mov      lr, pc
003d0c2c: ldr      pc, [r3, #0x80]
003d0c30: pop      {r4, pc}
