
# _ZN7gameswf12get_fontfileEPKcRNS_9tu_stringEbb
007d0fe4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d0fe8: ldr      r4, [pc, #0x12c]
007d0fec: ldr      r5, [pc, #0x12c]
007d0ff0: subs     r8, r0, #0
007d0ff4: add      r4, pc, r4
007d0ff8: ldr      r0, [r4, r5]
007d0ffc: mov      fp, r2
007d1000: sub      sp, sp, #0x114
007d1004: ldr      r2, [r0]
007d1008: mov      r7, r1
007d100c: mov      sb, r3
007d1010: str      r2, [sp, #0x10c]
007d1014: beq      #0x7d109c
007d1018: add      r6, sp, #0xc
007d101c: mov      sl, #0x100
007d1020: mov      r1, #0
007d1024: mov      r2, sl
007d1028: mov      r0, r6
007d102c: bl       #0x30e460
007d1030: mov      r0, r8
007d1034: mov      r1, fp
007d1038: mov      r2, sb
007d103c: mov      r3, r6
007d1040: str      sl, [sp]
007d1044: bl       #0x42b38c
007d1048: cmp      r0, #0
007d104c: bne      #0x7d10f0
007d1050: ldr      r1, [pc, #0xcc]
007d1054: mov      r0, r8
007d1058: add      r1, pc, r1
007d105c: bl       #0x30ebd4
007d1060: cmp      r0, #0
007d1064: beq      #0x7d109c
007d1068: ldr      r1, [pc, #0xb8]
007d106c: mov      r0, r7
007d1070: add      r1, pc, r1
007d1074: bl       #0x76c818
007d1078: cmp      fp, #0
007d107c: beq      #0x7d10bc
007d1080: cmp      sb, #0
007d1084: bne      #0x7d10dc
007d1088: ldr      r1, [pc, #0x9c]
007d108c: mov      r0, r7
007d1090: add      r1, pc, r1
007d1094: bl       #0x7521cc
007d1098: b        #0x7d10c4
007d109c: mov      r0, #0
007d10a0: ldr      r3, [r4, r5]
007d10a4: ldr      r2, [sp, #0x10c]
007d10a8: ldr      r3, [r3]
007d10ac: cmp      r2, r3
007d10b0: bne      #0x7d1118
007d10b4: add      sp, sp, #0x114
007d10b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d10bc: cmp      sb, #0
007d10c0: bne      #0x7d1104
007d10c4: ldr      r1, [pc, #0x64]
007d10c8: mov      r0, r7
007d10cc: add      r1, pc, r1
007d10d0: bl       #0x7521cc
007d10d4: mov      r0, #1
007d10d8: b        #0x7d10a0
007d10dc: ldr      r1, [pc, #0x50]
007d10e0: mov      r0, r7
007d10e4: add      r1, pc, r1
007d10e8: bl       #0x7521cc
007d10ec: b        #0x7d10c4
007d10f0: mov      r0, r7
007d10f4: mov      r1, r6
007d10f8: bl       #0x76c818
007d10fc: mov      r0, #1
007d1100: b        #0x7d10a0
007d1104: ldr      r1, [pc, #0x2c]
007d1108: mov      r0, r7
007d110c: add      r1, pc, r1
007d1110: bl       #0x7521cc
007d1114: b        #0x7d10c4
007d1118: bl       #0x30e310
007d111c: mulseq   ip, ip, sl
007d1120: andeq    r4, r0, ip, lsr #1
007d1124: andseq   sl, r3, r0, lsr #30
007d1128: andseq   sl, r3, r0, asr #30
007d112c: andseq   sl, r3, r8, lsl #16
007d1130: ldrdeq   r8, sb, [pc], -ip
007d1134: andseq   sl, r3, ip, ror #29
007d1138: andseq   sl, r3, ip, lsl #15

# _ZN8RenderFX7SetTextEPN7gameswf9characterEPKcb
007a92e0: push     {r4, r5, r6, r7, r8, sl, lr}
007a92e4: ldr      r4, [pc, #0x9c]
007a92e8: ldr      r5, [pc, #0x9c]
007a92ec: subs     r6, r1, #0
007a92f0: add      r4, pc, r4
007a92f4: ldr      r1, [r4, r5]
007a92f8: mov      r8, r3
007a92fc: sub      sp, sp, #0x1c
007a9300: ldr      r3, [r1]
007a9304: mov      r7, r2
007a9308: str      r3, [sp, #0x14]
007a930c: beq      #0x7a932c
007a9310: ldr      ip, [r6]
007a9314: mov      r0, r6
007a9318: mov      r1, #0x20
007a931c: mov      lr, pc
007a9320: ldr      pc, [ip, #8]
007a9324: cmp      r0, #0
007a9328: bne      #0x7a9348
007a932c: ldr      r3, [r4, r5]
007a9330: ldr      r2, [sp, #0x14]
007a9334: ldr      r3, [r3]
007a9338: cmp      r2, r3
007a933c: bne      #0x7a9384
007a9340: add      sp, sp, #0x1c
007a9344: pop      {r4, r5, r6, r7, r8, sl, pc}
007a9348: mov      r1, r7
007a934c: mov      r0, sp
007a9350: bl       #0x413a7c
007a9354: mov      r0, r6
007a9358: mov      r1, sp
007a935c: mov      r2, r8
007a9360: bl       #0x790ab0
007a9364: ldrsb    r3, [sp]
007a9368: mov      sl, sp
007a936c: cmn      r3, #1
007a9370: bne      #0x7a932c
007a9374: ldr      r0, [sp, #0xc]
007a9378: ldr      r1, [sp, #8]
007a937c: bl       #0x752b38
007a9380: b        #0x7a932c
007a9384: bl       #0x30e310
007a9388: andseq   fp, lr, r0, lsr #15
007a938c: andeq    r4, r0, ip, lsr #1

# _Z12get_fontfilePKcbbPci
0042b38c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0042b390: ldr      r4, [pc, #0x3fc]
0042b394: ldr      r8, [pc, #0x3fc]
0042b398: ldr      lr, [pc, #0x3fc]
0042b39c: add      r4, pc, r4
0042b3a0: ldr      ip, [r4, r8]
0042b3a4: ldr      r7, [r4, lr]
0042b3a8: sub      sp, sp, #0x8c
0042b3ac: ldr      ip, [ip]
0042b3b0: add      r5, sp, #0x6c
0042b3b4: mov      r6, r0
0042b3b8: mov      r0, r7
0042b3bc: str      ip, [sp, #0x84]
0042b3c0: mov      sl, r3
0042b3c4: mov      sb, r2
0042b3c8: mov      fp, r1
0042b3cc: bl       #0x337888
0042b3d0: mov      r0, r5
0042b3d4: mov      r1, #0x10
0042b3d8: str      r5, [sp, #0x7c]
0042b3dc: str      r5, [sp, #0x80]
0042b3e0: bl       #0x31167c
0042b3e4: ldr      r1, [pc, #0x3b4]
0042b3e8: mov      r2, #0xf
0042b3ec: ldr      r0, [sp, #0x80]
0042b3f0: add      r1, pc, r1
0042b3f4: bl       #0x30e868
0042b3f8: add      r3, r0, #0xf
0042b3fc: str      r3, [sp, #0x7c]
0042b400: mov      r3, #0
0042b404: strb     r3, [r0, #0xf]
0042b408: mov      r1, r5
0042b40c: mov      r0, r7
0042b410: bl       #0x337a88
0042b414: mov      r0, r5
0042b418: bl       #0x3139ac
0042b41c: ldr      r1, [pc, #0x380]
0042b420: mov      r0, r6
0042b424: add      r1, pc, r1
0042b428: bl       #0x30e31c
0042b42c: cmp      r0, #0
0042b430: beq      #0x42b5a4
0042b434: ldr      r1, [pc, #0x36c]
0042b438: mov      r0, r6
0042b43c: add      r1, pc, r1
0042b440: bl       #0x30e31c
0042b444: cmp      r0, #0
0042b448: beq      #0x42b644
0042b44c: ldr      r1, [pc, #0x358]
0042b450: mov      r0, r6
0042b454: add      r1, pc, r1
0042b458: bl       #0x30e31c
0042b45c: cmp      r0, #0
0042b460: beq      #0x42b554
0042b464: ldr      r1, [pc, #0x344]
0042b468: mov      r0, r6
0042b46c: add      r1, pc, r1
0042b470: bl       #0x30e31c
0042b474: cmp      r0, #0
0042b478: beq      #0x42b660
0042b47c: ldr      r1, [pc, #0x330]
0042b480: mov      r0, r6
0042b484: add      r1, pc, r1
0042b488: bl       #0x30e31c
0042b48c: cmp      r0, #0
0042b490: bne      #0x42b5f8
0042b494: ldr      r2, [pc, #0x31c]
0042b498: ldr      r1, [pc, #0x31c]
0042b49c: mov      r3, r6
0042b4a0: ldr      r2, [r4, r2]
0042b4a4: add      r1, pc, r1
0042b4a8: mov      r0, sl
0042b4ac: ldr      r2, [r2]
0042b4b0: bl       #0x30eae4
0042b4b4: ldr      r5, [pc, #0x304]
0042b4b8: ldr      r3, [r4, r5]
0042b4bc: add      r5, sp, #0xc
0042b4c0: mov      r1, sl
0042b4c4: ldr      r3, [r3, #0x10]
0042b4c8: add      r2, sp, #4
0042b4cc: mov      r0, r5
0042b4d0: ldr      r6, [r3, #0x34]
0042b4d4: add      sb, sp, #0x24
0042b4d8: ldr      r3, [r6]
0042b4dc: ldr      r7, [r3, #0x34]
0042b4e0: bl       #0x32603c
0042b4e4: mov      r0, sb
0042b4e8: mov      r1, r6
0042b4ec: mov      r2, r5
0042b4f0: blx      r7
0042b4f4: ldr      r0, [sp, #0x20]
0042b4f8: cmp      r0, r5
0042b4fc: beq      #0x42b50c
0042b500: cmp      r0, #0
0042b504: beq      #0x42b50c
0042b508: bl       #0x310450
0042b50c: ldr      r1, [sp, #0x38]
0042b510: mov      r0, sl
0042b514: bl       #0x30e520
0042b518: ldr      r1, [pc, #0x2a4]
0042b51c: mov      r0, sl
0042b520: add      r1, pc, r1
0042b524: bl       #0x30e508
0042b528: subs     r5, r0, #0
0042b52c: beq      #0x42b538
0042b530: bl       #0x30eb14
0042b534: mov      r5, #1
0042b538: ldr      r0, [sp, #0x38]
0042b53c: cmp      r0, sb
0042b540: beq      #0x42b5bc
0042b544: cmp      r0, #0
0042b548: beq      #0x42b5bc
0042b54c: bl       #0x310450
0042b550: b        #0x42b5bc
0042b554: cmp      fp, #0
0042b558: beq      #0x42b580
0042b55c: cmp      sb, #0
0042b560: bne      #0x42b724
0042b564: ldr      r1, [pc, #0x25c]
0042b568: mov      r0, sl
0042b56c: mov      r2, #0x1c
0042b570: add      r1, pc, r1
0042b574: bl       #0x30e868
0042b578: mov      r5, #1
0042b57c: b        #0x42b5bc
0042b580: cmp      sb, #0
0042b584: beq      #0x42b5dc
0042b588: ldr      r1, [pc, #0x23c]
0042b58c: mov      r0, sl
0042b590: mov      r2, #0x1f
0042b594: add      r1, pc, r1
0042b598: bl       #0x30e868
0042b59c: mov      r5, #1
0042b5a0: b        #0x42b5bc
0042b5a4: ldr      r1, [pc, #0x224]
0042b5a8: mov      r0, sl
0042b5ac: mov      r2, #0x1d
0042b5b0: add      r1, pc, r1
0042b5b4: bl       #0x30e868
0042b5b8: mov      r5, #1
0042b5bc: ldr      r3, [r4, r8]
0042b5c0: ldr      r2, [sp, #0x84]
0042b5c4: mov      r0, r5
0042b5c8: ldr      r3, [r3]
0042b5cc: cmp      r2, r3
0042b5d0: bne      #0x42b790
0042b5d4: add      sp, sp, #0x8c
0042b5d8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0042b5dc: ldr      r1, [pc, #0x1f0]
0042b5e0: mov      r0, sl
0042b5e4: mov      r2, #0x17
0042b5e8: add      r1, pc, r1
0042b5ec: bl       #0x30e868
0042b5f0: mov      r5, #1
0042b5f4: b        #0x42b5bc
0042b5f8: ldr      r5, [pc, #0x1c0]
0042b5fc: ldr      r3, [r4, r5]
0042b600: ldr      r0, [r3, #0x4c]
0042b604: bl       #0x46d514
0042b608: cmp      r0, #5
0042b60c: beq      #0x42b764
0042b610: cmp      r0, #6
0042b614: beq      #0x42b740
0042b618: cmp      r0, #4
0042b61c: beq      #0x42b700
0042b620: ldr      r2, [pc, #0x190]
0042b624: ldr      r1, [pc, #0x1ac]
0042b628: mov      r3, r6
0042b62c: ldr      r2, [r4, r2]
0042b630: add      r1, pc, r1
0042b634: mov      r0, sl
0042b638: ldr      r2, [r2]
0042b63c: bl       #0x30eae4
0042b640: b        #0x42b4b8
0042b644: ldr      r1, [pc, #0x190]
0042b648: mov      r0, sl
0042b64c: mov      r2, #0x1d
0042b650: add      r1, pc, r1
0042b654: bl       #0x30e868
0042b658: mov      r5, #1
0042b65c: b        #0x42b5bc
0042b660: ldr      r3, [pc, #0x150]
0042b664: ldr      r1, [pc, #0x174]
0042b668: mov      r0, sl
0042b66c: ldr      r3, [r4, r3]
0042b670: add      r1, pc, r1
0042b674: add      r5, sp, #0x3c
0042b678: ldr      r2, [r3]
0042b67c: bl       #0x30eae4
0042b680: ldr      r3, [pc, #0x138]
0042b684: mov      r1, sl
0042b688: add      r2, sp, #8
0042b68c: ldr      r3, [r4, r3]
0042b690: mov      r0, r5
0042b694: add      sb, sp, #0x54
0042b698: ldr      r3, [r3, #0x10]
0042b69c: ldr      r6, [r3, #0x34]
0042b6a0: ldr      r3, [r6]
0042b6a4: ldr      r7, [r3, #0x34]
0042b6a8: bl       #0x32603c
0042b6ac: mov      r0, sb
0042b6b0: mov      r1, r6
0042b6b4: mov      r2, r5
0042b6b8: blx      r7
0042b6bc: ldr      r0, [sp, #0x50]
0042b6c0: cmp      r0, r5
0042b6c4: beq      #0x42b6d4
0042b6c8: cmp      r0, #0
0042b6cc: beq      #0x42b6d4
0042b6d0: bl       #0x310450
0042b6d4: mov      r0, sl
0042b6d8: ldr      r1, [sp, #0x68]
0042b6dc: bl       #0x30e520
0042b6e0: ldr      r0, [sp, #0x68]
0042b6e4: cmp      r0, sb
0042b6e8: beq      #0x42b788
0042b6ec: cmp      r0, #0
0042b6f0: beq      #0x42b788
0042b6f4: bl       #0x310450
0042b6f8: mov      r5, #1
0042b6fc: b        #0x42b5bc
0042b700: ldr      r2, [pc, #0xb0]
0042b704: ldr      r1, [pc, #0xd8]
0042b708: mov      r3, r6
0042b70c: ldr      r2, [r4, r2]
0042b710: add      r1, pc, r1
0042b714: mov      r0, sl
0042b718: ldr      r2, [r2]
0042b71c: bl       #0x30eae4
0042b720: b        #0x42b4b8
0042b724: ldr      r1, [pc, #0xbc]
0042b728: mov      r0, sl
0042b72c: mov      r2, #0x23
0042b730: add      r1, pc, r1
0042b734: bl       #0x30e868
0042b738: mov      r5, #1
0042b73c: b        #0x42b5bc
0042b740: ldr      r2, [pc, #0x70]
0042b744: ldr      r1, [pc, #0xa0]
0042b748: mov      r3, r6
0042b74c: ldr      r2, [r4, r2]
0042b750: add      r1, pc, r1
0042b754: mov      r0, sl
0042b758: ldr      r2, [r2]
0042b75c: bl       #0x30eae4
0042b760: b        #0x42b4b8
0042b764: ldr      r2, [pc, #0x4c]
0042b768: ldr      r1, [pc, #0x80]
0042b76c: mov      r3, r6
0042b770: ldr      r2, [r4, r2]
0042b774: add      r1, pc, r1
0042b778: mov      r0, sl
0042b77c: ldr      r2, [r2]
0042b780: bl       #0x30eae4
0042b784: b        #0x42b4b8
0042b788: mov      r5, #1
0042b78c: b        #0x42b5bc
0042b790: bl       #0x30e310
0042b794: ldrsheq  sb, [r6], #-0x64
0042b798: andeq    r4, r0, ip, lsr #1
0042b79c: andeq    r0, r0, r4, lsl #17
0042b7a0: umaaleq  lr, sb, r0, r7
0042b7a4: subeq    r4, sb, r4, lsr #6
0042b7a8: subeq    lr, sb, r4, ror r7
0042b7ac: subeq    lr, sb, r4, ror #14
0042b7b0: ldrdeq   lr, pc, [sb], #-0x7c
0042b7b4: subeq    lr, sb, ip, ror #15
0042b7b8: andeq    r0, r0, r0, lsl #12
0042b7bc: ldrdeq   lr, pc, [sb], #-0x74
0042b7c0: strdeq   r3, r4, [r0], -r4
0042b7c4: subeq    r5, sb, r0, lsl #5
0042b7c8: subeq    lr, sb, r0, lsl #13
0042b7cc: subeq    lr, sb, ip, ror r6
0042b7d0: subeq    lr, sb, r0, ror #11
0042b7d4: subeq    lr, sb, r8, asr #12
0042b7d8: umaaleq  lr, sb, r0, r6
0042b7dc: subeq    lr, sb, r0, asr #10
0042b7e0: subeq    lr, sb, r0, ror #11
0042b7e4: subeq    lr, sb, r0, lsl #11
0042b7e8: umaaleq  lr, sb, r8, r4
0042b7ec: subeq    lr, sb, r8, lsr #10
0042b7f0: subeq    lr, sb, r4, lsr r5
