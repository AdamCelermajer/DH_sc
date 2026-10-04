
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
