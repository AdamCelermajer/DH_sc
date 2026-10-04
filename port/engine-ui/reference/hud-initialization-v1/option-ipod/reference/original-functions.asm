
# _Z19NativeUseIpodPlayerRKN7gameswf7fn_callE
0043a974: push     {r4, lr}
0043a978: mov      r4, r0
0043a97c: bl       #0x533570
0043a980: cmp      r0, #1
0043a984: mov      r1, r0
0043a988: beq      #0x43a99c
0043a98c: ldr      r0, [r4]
0043a990: mov      r1, #0
0043a994: pop      {r4, lr}
0043a998: b        #0x797230
0043a99c: ldr      r0, [r4]
0043a9a0: pop      {r4, lr}
0043a9a4: b        #0x797230

# _Z25NativeGetOptionParametersRKN7gameswf7fn_callE
0044a298: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0044a29c: ldr      r4, [pc, #0x2f4]
0044a2a0: ldr      sb, [pc, #0x2f4]
0044a2a4: ldr      r3, [r0, #0xc]
0044a2a8: add      r4, pc, r4
0044a2ac: ldr      r2, [r4, sb]
0044a2b0: sub      sp, sp, #0x84
0044a2b4: mov      r6, r0
0044a2b8: ldr      r2, [r2]
0044a2bc: ldr      r0, [r0, #0x14]
0044a2c0: mov      r5, #0xc
0044a2c4: str      r2, [sp, #0x7c]
0044a2c8: ldr      r3, [r3]
0044a2cc: mla      r0, r5, r0, r3
0044a2d0: bl       #0x796fb4
0044a2d4: ldr      r3, [r6, #0xc]
0044a2d8: ldr      r2, [r6, #0x14]
0044a2dc: mov      r7, r0
0044a2e0: ldr      r3, [r3]
0044a2e4: sub      r2, r2, #1
0044a2e8: mla      r5, r5, r2, r3
0044a2ec: ldrsb    r3, [r5, #1]
0044a2f0: cmp      r3, #5
0044a2f4: ldreq    r0, [r5, #4]
0044a2f8: movne    r0, #0
0044a2fc: bl       #0x439cb4
0044a300: ldr      r3, [pc, #0x298]
0044a304: mov      r1, r7
0044a308: mov      r5, r0
0044a30c: ldr      fp, [r4, r3]
0044a310: ldr      r0, [fp, #0x4c]
0044a314: bl       #0x46d474
0044a318: mov      r1, r7
0044a31c: mov      sl, r0
0044a320: ldr      r0, [fp, #0x4c]
0044a324: bl       #0x46d330
0044a328: mov      r1, r7
0044a32c: mov      r8, r0
0044a330: ldr      r0, [fp, #0x4c]
0044a334: bl       #0x46d2b8
0044a338: rsb      r3, sl, r0
0044a33c: cmn      r3, #1
0044a340: mov      r1, r0
0044a344: beq      #0x44a544
0044a348: ldr      r0, [fp, #0x34]
0044a34c: bl       #0x508edc
0044a350: cmp      r5, #0
0044a354: str      r0, [sp, #0xc]
0044a358: beq      #0x44a558
0044a35c: ldr      r3, [pc, #0x240]
0044a360: str      r3, [sp, #8]
0044a364: ldr      r3, [r4, r3]
0044a368: ldrb     r3, [r3]
0044a36c: cmp      r3, #0
0044a370: bne      #0x44a528
0044a374: ldr      r3, [r5]
0044a378: ldr      r1, [pc, #0x228]
0044a37c: add      fp, sp, #0x68
0044a380: ldr      r3, [r3, #0x1c]
0044a384: add      r1, pc, r1
0044a388: mov      r0, fp
0044a38c: str      r3, [sp, #4]
0044a390: bl       #0x413a7c
0044a394: mov      r2, #0
0044a398: mov      r0, r8
0044a39c: strb     r2, [sp, #0x2c]
0044a3a0: mov      r2, #2
0044a3a4: strb     r2, [sp, #0x2d]
0044a3a8: bl       #0x30ed30
0044a3ac: strd     r0, r1, [sp, #0x38]
0044a3b0: ldr      r2, [sp, #0x38]
0044a3b4: add      r8, sp, #0x2c
0044a3b8: mov      r1, fp
0044a3bc: str      r2, [sp, #0x30]
0044a3c0: ldr      r2, [sp, #0x3c]
0044a3c4: mov      r0, r5
0044a3c8: str      r2, [r8, #8]
0044a3cc: ldr      r3, [sp, #4]
0044a3d0: mov      r2, r8
0044a3d4: blx      r3
0044a3d8: mov      r0, r8
0044a3dc: bl       #0x797124
0044a3e0: ldrsb    r3, [sp, #0x68]
0044a3e4: cmn      r3, #1
0044a3e8: beq      #0x44a574
0044a3ec: ldr      r3, [r5]
0044a3f0: ldr      r1, [pc, #0x1b4]
0044a3f4: add      fp, sp, #0x54
0044a3f8: ldr      r3, [r3, #0x1c]
0044a3fc: add      r1, pc, r1
0044a400: mov      r0, fp
0044a404: str      r3, [sp, #4]
0044a408: bl       #0x413a7c
0044a40c: mov      r2, #0
0044a410: strb     r2, [sp, #0x20]
0044a414: mov      r0, sl
0044a418: mov      r2, #2
0044a41c: strb     r2, [sp, #0x21]
0044a420: bl       #0x30ed30
0044a424: strd     r0, r1, [sp, #0x38]
0044a428: ldr      r2, [sp, #0x38]
0044a42c: add      r8, sp, #0x20
0044a430: mov      r1, fp
0044a434: str      r2, [sp, #0x24]
0044a438: ldr      r2, [sp, #0x3c]
0044a43c: mov      r0, r5
0044a440: str      r2, [r8, #8]
0044a444: ldr      r3, [sp, #4]
0044a448: mov      r2, r8
0044a44c: blx      r3
0044a450: mov      r0, r8
0044a454: bl       #0x797124
0044a458: ldrsb    r3, [sp, #0x54]
0044a45c: cmn      r3, #1
0044a460: beq      #0x44a584
0044a464: ldr      r3, [r5]
0044a468: ldr      r1, [pc, #0x140]
0044a46c: add      fp, sp, #0x40
0044a470: ldr      ip, [r3, #0x1c]
0044a474: add      r8, sp, #0x14
0044a478: add      r1, pc, r1
0044a47c: mov      r0, fp
0044a480: str      ip, [sp, #4]
0044a484: bl       #0x413a7c
0044a488: mov      r3, #0
0044a48c: ldr      r1, [sp, #0xc]
0044a490: mov      r0, r8
0044a494: strb     r3, [sp, #0x15]
0044a498: strb     r3, [sp, #0x14]
0044a49c: bl       #0x797350
0044a4a0: mov      r1, fp
0044a4a4: mov      r2, r8
0044a4a8: ldr      ip, [sp, #4]
0044a4ac: mov      r0, r5
0044a4b0: blx      ip
0044a4b4: mov      r0, r8
0044a4b8: bl       #0x797124
0044a4bc: ldrsb    r3, [sp, #0x40]
0044a4c0: cmn      r3, #1
0044a4c4: beq      #0x44a564
0044a4c8: ldr      r0, [r6]
0044a4cc: mov      r1, r5
0044a4d0: bl       #0x797250
0044a4d4: ldr      r2, [sp, #8]
0044a4d8: ldr      r3, [r4, r2]
0044a4dc: ldrb     r3, [r3]
0044a4e0: cmp      r3, #0
0044a4e4: beq      #0x44a50c
0044a4e8: ldr      r0, [pc, #0xc4]
0044a4ec: mov      r1, r7
0044a4f0: add      r0, pc, r0
0044a4f4: bl       #0x30e31c
0044a4f8: cmp      r0, #0
0044a4fc: bne      #0x44a50c
0044a500: ldr      r3, [pc, #0xb0]
0044a504: ldr      r3, [r4, r3]
0044a508: str      sl, [r3]
0044a50c: ldr      r3, [r4, sb]
0044a510: ldr      r2, [sp, #0x7c]
0044a514: ldr      r3, [r3]
0044a518: cmp      r2, r3
0044a51c: bne      #0x44a594
0044a520: add      sp, sp, #0x84
0044a524: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0044a528: ldr      r0, [pc, #0x8c]
0044a52c: mov      r1, r7
0044a530: add      r0, pc, r0
0044a534: bl       #0x30e31c
0044a538: cmp      r0, #0
0044a53c: moveq    r8, #5
0044a540: b        #0x44a374
0044a544: ldr      r3, [pc, #0x74]
0044a548: cmp      r5, #0
0044a54c: add      r3, pc, r3
0044a550: str      r3, [sp, #0xc]
0044a554: bne      #0x44a35c
0044a558: ldr      r2, [pc, #0x44]
0044a55c: str      r2, [sp, #8]
0044a560: b        #0x44a4d4
0044a564: ldr      r0, [sp, #0x4c]
0044a568: ldr      r1, [sp, #0x48]
0044a56c: bl       #0x752b38
0044a570: b        #0x44a4c8
0044a574: ldr      r0, [sp, #0x74]
0044a578: ldr      r1, [sp, #0x70]
0044a57c: bl       #0x752b38
0044a580: b        #0x44a3ec
0044a584: ldr      r0, [sp, #0x60]
0044a588: ldr      r1, [sp, #0x5c]
0044a58c: bl       #0x752b38
0044a590: b        #0x44a464
0044a594: bl       #0x30e310
0044a598: subseq   sl, r4, r8, ror #15
0044a59c: andeq    r4, r0, ip, lsr #1
0044a5a0: strdeq   r3, r4, [r0], -r4
0044a5a4: andeq    r3, r0, ip, lsr #31
0044a5a8: subeq    r2, r8, ip, lsl r4
0044a5ac: subeq    r2, r8, r4, lsl #7
0044a5b0: subeq    r2, r8, r8, lsl r3
0044a5b4: subeq    r1, r8, r8, ror #12
0044a5b8: andeq    r2, r0, r0, ror #17
0044a5bc: subeq    r1, r8, r8, lsr #12
0044a5c0: strheq   r1, [r8], #-0x2c
