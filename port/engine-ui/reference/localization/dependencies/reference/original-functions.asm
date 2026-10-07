
# _ZN13StringManager5parseERSsPKcz
00508ef4: push     {r2, r3}
00508ef8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508efc: ldr      r5, [pc, #0xab4]
00508f00: ldr      r2, [pc, #0xab4]
00508f04: sub      sp, sp, #0xac
00508f08: add      r5, pc, r5
00508f0c: ldr      r3, [r5, r2]
00508f10: ldr      r4, [sp, #0xd0]
00508f14: str      r2, [sp, #0x14]
00508f18: ldr      r3, [r3]
00508f1c: add      r2, sp, #0xd4
00508f20: cmp      r4, #0
00508f24: str      r2, [sp, #0x54]
00508f28: str      r0, [sp, #0x18]
00508f2c: str      r3, [sp, #0xa4]
00508f30: mov      r7, r1
00508f34: beq      #0x508f44
00508f38: ldrsb    r3, [r4]
00508f3c: cmp      r3, #0
00508f40: bne      #0x508f74
00508f44: mov      r8, #0
00508f48: ldr      r2, [sp, #0x14]
00508f4c: mov      r0, r8
00508f50: ldr      r3, [r5, r2]
00508f54: ldr      r2, [sp, #0xa4]
00508f58: ldr      r3, [r3]
00508f5c: cmp      r2, r3
00508f60: bne      #0x5099b4
00508f64: add      sp, sp, #0xac
00508f68: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508f6c: add      sp, sp, #8
00508f70: bx       lr
00508f74: ldr      r3, [pc, #0xa44]
00508f78: ldr      r6, [pc, #0xa44]
00508f7c: ldr      r2, [pc, #0xa44]
00508f80: ldr      r8, [r5, r3]
00508f84: add      r6, pc, r6
00508f88: add      r2, pc, r2
00508f8c: ldr      r0, [r8, #0x2c]
00508f90: mov      r1, r6
00508f94: str      r3, [sp, #0x24]
00508f98: bl       #0x4c4bdc
00508f9c: mov      r1, r0
00508fa0: ldr      r0, [sp, #0x18]
00508fa4: bl       #0x508edc
00508fa8: ldr      r2, [pc, #0xa1c]
00508fac: str      r0, [sp, #0x40]
00508fb0: mov      r1, r6
00508fb4: add      r2, pc, r2
00508fb8: ldr      r0, [r8, #0x2c]
00508fbc: bl       #0x4c4bdc
00508fc0: mov      r1, r0
00508fc4: ldr      r0, [sp, #0x18]
00508fc8: bl       #0x508edc
00508fcc: ldr      r2, [pc, #0x9fc]
00508fd0: str      r0, [sp, #0x38]
00508fd4: mov      r1, r6
00508fd8: add      r2, pc, r2
00508fdc: ldr      r0, [r8, #0x2c]
00508fe0: bl       #0x4c4bdc
00508fe4: mov      r1, r0
00508fe8: ldr      r0, [sp, #0x18]
00508fec: bl       #0x508edc
00508ff0: bl       #0x30e094
00508ff4: str      r0, [sp, #0x2c]
00508ff8: ldrb     r3, [r4]
00508ffc: cmp      r3, #0
00509000: moveq    r8, r3
00509004: beq      #0x509248
00509008: ldr      r2, [pc, #0x9c4]
0050900c: mov      r8, #0
00509010: add      r4, r4, #1
00509014: add      r2, pc, r2
00509018: str      r2, [sp, #0x34]
0050901c: ldr      r2, [pc, #0x9b4]
00509020: ldr      ip, [sp, #0x34]
00509024: mov      r6, r8
00509028: add      r2, pc, r2
0050902c: str      r2, [sp, #0x3c]
00509030: ldr      r2, [pc, #0x9a4]
00509034: add      ip, ip, #6
00509038: str      ip, [sp, #0x48]
0050903c: add      r2, pc, r2
00509040: str      r2, [sp, #0x4c]
00509044: ldr      r2, [pc, #0x994]
00509048: str      r5, [sp, #0x28]
0050904c: add      r2, pc, r2
00509050: str      r2, [sp, #0x44]
00509054: b        #0x509088
00509058: sxtb     r3, r3
0050905c: cmp      r3, #0x5e
00509060: moveq    r6, #1
00509064: beq      #0x50907c
00509068: cmp      r3, #0x7c
0050906c: beq      #0x5092b0
00509070: mov      r0, r7
00509074: mov      r2, r4
00509078: bl       #0x310804
0050907c: ldrb     r3, [r4], #1
00509080: cmp      r3, #0
00509084: beq      #0x509244
00509088: cmp      r6, #0
0050908c: sub      r1, r4, #1
00509090: beq      #0x509058
00509094: sxtb     r3, r3
00509098: sub      r2, r3, #0x23
0050909c: cmp      r2, #0x53
005090a0: addls    pc, pc, r2, lsl #2
005090a4: b        #0x509230
005090a8: b        #0x5096f4
005090ac: b        #0x5095fc
005090b0: b        #0x509230
005090b4: b        #0x509230
005090b8: b        #0x509230
005090bc: b        #0x509230
005090c0: b        #0x509230
005090c4: b        #0x5096f4
005090c8: b        #0x509230
005090cc: b        #0x509230
005090d0: b        #0x509230
005090d4: b        #0x509230
005090d8: b        #0x509230
005090dc: b        #0x509230
005090e0: b        #0x509230
005090e4: b        #0x509230
005090e8: b        #0x509230
005090ec: b        #0x509230
005090f0: b        #0x509230
005090f4: b        #0x509230
005090f8: b        #0x509230
005090fc: b        #0x509230
00509100: b        #0x509230
00509104: b        #0x509230
00509108: b        #0x509230
0050910c: b        #0x509230
00509110: b        #0x509230
00509114: b        #0x509230
00509118: b        #0x509230
0050911c: b        #0x509230
00509120: b        #0x509230
00509124: b        #0x509230
00509128: b        #0x509230
0050912c: b        #0x509230
00509130: b        #0x509230
00509134: b        #0x509230
00509138: b        #0x509230
0050913c: b        #0x509230
00509140: b        #0x509230
00509144: b        #0x509230
00509148: b        #0x509230
0050914c: b        #0x509230
00509150: b        #0x509230
00509154: b        #0x509230
00509158: b        #0x509230
0050915c: b        #0x509230
00509160: b        #0x509230
00509164: b        #0x509230
00509168: b        #0x509230
0050916c: b        #0x509230
00509170: b        #0x509230
00509174: b        #0x509230
00509178: b        #0x509230
0050917c: b        #0x509230
00509180: b        #0x509230
00509184: b        #0x509230
00509188: b        #0x509230
0050918c: b        #0x509230
00509190: b        #0x509230
00509194: b        #0x5096f4
00509198: b        #0x509230
0050919c: b        #0x509230
005091a0: b        #0x509230
005091a4: b        #0x509230
005091a8: b        #0x509230
005091ac: b        #0x509540
005091b0: b        #0x509230
005091b4: b        #0x509388
005091b8: b        #0x509388
005091bc: b        #0x509388
005091c0: b        #0x509388
005091c4: b        #0x509230
005091c8: b        #0x509540
005091cc: b        #0x509230
005091d0: b        #0x509388
005091d4: b        #0x509350
005091d8: b        #0x509230
005091dc: b        #0x509540
005091e0: b        #0x509230
005091e4: b        #0x509230
005091e8: b        #0x50932c
005091ec: b        #0x5092ec
005091f0: b        #0x509230
005091f4: b        #0x5091f8
005091f8: ldr      ip, [sp, #0x28]
005091fc: ldr      lr, [sp, #0x24]
00509200: add      r5, sp, #0x98
00509204: mov      r1, r5
00509208: mov      r2, #0xa
0050920c: mov      r3, #1
00509210: ldr      r0, [ip, lr]
00509214: bl       #0x31f6b0
00509218: mov      r0, r5
0050921c: bl       #0x30de54
00509220: add      r2, r5, r0
00509224: mov      r1, r5
00509228: mov      r0, r7
0050922c: bl       #0x310804
00509230: mov      r8, #1
00509234: mov      r6, #0
00509238: ldrb     r3, [r4], #1
0050923c: cmp      r3, #0
00509240: bne      #0x509088
00509244: ldr      r5, [sp, #0x28]
00509248: ldr      r3, [r7, #0x14]
0050924c: ldr      r0, [r7, #0x10]
00509250: mov      r1, #0
00509254: rsb      r0, r3, r0
00509258: add      r0, r0, #0x80
0050925c: bl       #0x31056c
00509260: mov      r4, r0
00509264: ldr      r0, [sp, #0x18]
00509268: ldr      r6, [r7, #0x14]
0050926c: bl       #0x50750c
00509270: mov      r1, r4
00509274: mov      r3, r0
00509278: mvn      r2, #0
0050927c: mov      r0, r6
00509280: bl       #0x752a18
00509284: mov      r0, r4
00509288: bl       #0x30de54
0050928c: mov      r1, r4
00509290: add      r2, r4, r0
00509294: mov      r0, r7
00509298: bl       #0x3109e0
0050929c: cmp      r4, #0
005092a0: beq      #0x508f48
005092a4: mov      r0, r4
005092a8: bl       #0x310440
005092ac: b        #0x508f48
005092b0: ldr      r2, [pc, #0x72c]
005092b4: add      r5, sp, #0x78
005092b8: mov      r1, #0x20
005092bc: add      r2, pc, r2
005092c0: mov      r3, #0x11
005092c4: mov      r0, r5
005092c8: bl       #0x30e244
005092cc: mov      r0, r5
005092d0: bl       #0x30de54
005092d4: mov      r1, r5
005092d8: add      r2, r5, r0
005092dc: mov      r0, r7
005092e0: bl       #0x310804
005092e4: mov      r8, #1
005092e8: b        #0x50907c
005092ec: ldr      r3, [sp, #0x28]
005092f0: ldr      lr, [sp, #0x24]
005092f4: add      r5, sp, #0x58
005092f8: mov      r1, r5
005092fc: ldr      r0, [r3, lr]
00509300: mov      r2, #0x20
00509304: bl       #0x3206d0
00509308: mov      r0, r5
0050930c: bl       #0x30de54
00509310: mov      r1, r5
00509314: add      r2, r5, r0
00509318: mov      r0, r7
0050931c: bl       #0x310804
00509320: mov      r8, #1
00509324: mov      r6, #0
00509328: b        #0x50907c
0050932c: ldr      r3, [sp, #0x54]
00509330: add      r2, r3, #4
00509334: str      r2, [sp, #0x54]
00509338: ldr      r5, [r3]
0050933c: cmp      r5, #0
00509340: bne      #0x509218
00509344: ldr      r2, [sp, #0x48]
00509348: ldr      r5, [sp, #0x34]
0050934c: b        #0x509224
00509350: add      r5, sp, #0x78
00509354: mov      r1, #0x20
00509358: ldr      r2, [sp, #0x3c]
0050935c: mov      r0, r5
00509360: bl       #0x30e244
00509364: mov      r0, r5
00509368: bl       #0x30de54
0050936c: mov      r1, r5
00509370: add      r2, r5, r0
00509374: mov      r0, r7
00509378: bl       #0x310804
0050937c: mov      r8, #1
00509380: mov      r6, #0
00509384: b        #0x509238
00509388: cmp      r3, #0x66
0050938c: beq      #0x509790
00509390: cmp      r3, #0x67
00509394: beq      #0x509904
00509398: cmp      r3, #0x68
0050939c: beq      #0x509954
005093a0: cmp      r3, #0x69
005093a4: beq      #0x509984
005093a8: cmp      r3, #0x6d
005093ac: beq      #0x509810
005093b0: ldrsb    r3, [r4, #-1]
005093b4: cmp      r3, #0x6d
005093b8: beq      #0x5097cc
005093bc: mov      r1, #0
005093c0: ldr      r0, [sp, #0x20]
005093c4: bl       #0x30e70c
005093c8: cmp      r0, #0
005093cc: movweq   r1, #0xd70a
005093d0: movwne   r1, #0xd70a
005093d4: movteq   r1, #0x3ba3
005093d8: movtne   r1, #0xbba3
005093dc: ldr      r0, [sp, #0x20]
005093e0: bl       #0x30eba4
005093e4: str      r0, [sp, #0x20]
005093e8: add      r1, sp, #0x50
005093ec: ldr      r0, [sp, #0x20]
005093f0: bl       #0x30ea30
005093f4: mov      r1, #0
005093f8: mov      r5, r0
005093fc: ldr      r0, [sp, #0x20]
00509400: bl       #0x30e70c
00509404: cmp      r0, #0
00509408: movweq   r1, #0xd70a
0050940c: movwne   r1, #0xd70a
00509410: movteq   r1, #0x3ba3
00509414: movtne   r1, #0xbba3
00509418: mov      r0, r5
0050941c: bl       #0x30e3ac
00509420: mov      r6, r0
00509424: ldr      r0, [sp, #0x50]
00509428: bl       #0x30e4cc
0050942c: ldr      r1, [sp, #0x2c]
00509430: str      r0, [sp, #0x1c]
00509434: cmp      r1, r0
00509438: bgt      #0x509770
0050943c: ldr      r2, [sp, #0x1c]
00509440: movw     r3, #0xde83
00509444: ldr      ip, [sp, #0x1c]
00509448: movt     r3, #0x431b
0050944c: smull    r2, r3, r3, r2
00509450: asr      r1, ip, #0x1f
00509454: mov      r2, #0xf4000
00509458: rsb      r3, r1, r3, asr #18
0050945c: add      r2, r2, #0x240
00509460: mls      r2, r2, r3, ip
00509464: movw     r0, #0x4dd3
00509468: mov      lr, ip
0050946c: movt     r0, #0x1062
00509470: smull    lr, ip, r0, lr
00509474: smull    lr, r0, r0, r2
00509478: asr      r2, r2, #0x1f
0050947c: rsb      lr, r2, r0, asr #6
00509480: ldr      r0, [sp, #0x1c]
00509484: rsb      ip, r1, ip, asr #6
00509488: mov      r2, #0x3e8
0050948c: cmp      r3, #0
00509490: mls      ip, r2, ip, r0
00509494: bne      #0x5098ac
00509498: cmp      lr, #0
0050949c: beq      #0x509840
005094a0: ldr      r2, [pc, #0x540]
005094a4: mov      r3, lr
005094a8: ldr      lr, [sp, #0x38]
005094ac: add      r5, sp, #0x78
005094b0: add      r2, pc, r2
005094b4: mov      r0, r5
005094b8: mov      r1, #0x20
005094bc: str      ip, [sp, #4]
005094c0: str      lr, [sp]
005094c4: bl       #0x30e244
005094c8: mov      r0, r5
005094cc: bl       #0x30de54
005094d0: mov      r1, r5
005094d4: add      r2, r5, r0
005094d8: mov      r0, r7
005094dc: bl       #0x310804
005094e0: bic      r6, r6, #0x80000000
005094e4: movw     r1, #0xb717
005094e8: mov      r0, r6
005094ec: movt     r1, #0x38d1
005094f0: bl       #0x30e70c
005094f4: cmp      r0, #0
005094f8: bne      #0x509230
005094fc: mov      r0, r7
00509500: ldr      r1, [sp, #0x40]
00509504: bl       #0x3f1b80
00509508: ldrsb    r3, [r4, #-1]
0050950c: cmp      r3, #0x6d
00509510: beq      #0x5097f0
00509514: mov      r0, r6
00509518: bl       #0x30e8a4
0050951c: ldr      r2, [sp, #0x44]
00509520: strd     r0, r1, [sp]
00509524: mov      r0, r5
00509528: mov      r1, #0x10
0050952c: bl       #0x30e244
00509530: add      r1, r5, #2
00509534: mov      r0, r7
00509538: bl       #0x3f1b80
0050953c: b        #0x509230
00509540: cmp      r3, #0x64
00509544: beq      #0x5097b4
00509548: cmp      r3, #0x6b
0050954c: beq      #0x5098d8
00509550: cmp      r3, #0x70
00509554: beq      #0x509934
00509558: ldr      lr, [sp, #0x1c]
0050955c: ldr      r0, [sp, #0x2c]
00509560: cmp      lr, r0
00509564: blt      #0x509750
00509568: ldr      r1, [sp, #0x1c]
0050956c: movw     r3, #0xde83
00509570: ldr      r2, [sp, #0x1c]
00509574: movt     r3, #0x431b
00509578: smull    r1, r3, r3, r1
0050957c: ldr      ip, [sp, #0x1c]
00509580: asr      r1, r2, #0x1f
00509584: mov      r2, #0xf4000
00509588: rsb      r3, r1, r3, asr #18
0050958c: add      r2, r2, #0x240
00509590: mls      r2, r2, r3, ip
00509594: movw     r0, #0x4dd3
00509598: mov      lr, ip
0050959c: movt     r0, #0x1062
005095a0: smull    lr, ip, r0, lr
005095a4: smull    lr, r0, r0, r2
005095a8: asr      r2, r2, #0x1f
005095ac: rsb      lr, r2, r0, asr #6
005095b0: ldr      r0, [sp, #0x1c]
005095b4: rsb      ip, r1, ip, asr #6
005095b8: mov      r2, #0x3e8
005095bc: cmp      r3, #0
005095c0: mls      ip, r2, ip, r0
005095c4: bne      #0x509880
005095c8: cmp      lr, #0
005095cc: beq      #0x509860
005095d0: ldr      r2, [pc, #0x414]
005095d4: mov      r3, lr
005095d8: ldr      lr, [sp, #0x38]
005095dc: add      r5, sp, #0x78
005095e0: add      r2, pc, r2
005095e4: mov      r0, r5
005095e8: mov      r1, #0x20
005095ec: str      ip, [sp, #4]
005095f0: str      lr, [sp]
005095f4: bl       #0x30e244
005095f8: b        #0x509364
005095fc: ldr      r2, [sp, #0x54]
00509600: add      r3, r2, #4
00509604: str      r3, [sp, #0x54]
00509608: ldr      r8, [r2]
0050960c: add      r3, r3, #4
00509610: str      r3, [sp, #0x54]
00509614: ldrb     r3, [r8]
00509618: ldr      sb, [r2, #4]
0050961c: cmp      r3, #0
00509620: beq      #0x5096e8
00509624: add      r5, sp, #0x78
00509628: mov      r6, #0
0050962c: add      r0, r5, #1
00509630: add      r8, r8, #1
00509634: mov      fp, r5
00509638: mov      sl, r6
0050963c: str      r0, [sp, #0x30]
00509640: b        #0x5096ac
00509644: cmp      r6, #0
00509648: bne      #0x50970c
0050964c: ldrb     r2, [sb]
00509650: cmp      r2, #0
00509654: beq      #0x509668
00509658: cmp      r2, r3
0050965c: strbeq   r3, [fp], #1
00509660: addeq    sb, sb, #1
00509664: beq      #0x509694
00509668: mov      r1, #0
0050966c: strb     r3, [fp]
00509670: strb     r1, [fp, #1]
00509674: mov      r0, r5
00509678: bl       #0x30de54
0050967c: mov      r1, r5
00509680: add      r2, r5, r0
00509684: mov      r0, r7
00509688: bl       #0x310804
0050968c: mov      fp, r5
00509690: mov      sl, #0
00509694: ldrsb    r3, [sb]
00509698: cmp      r3, #0
0050969c: beq      #0x50971c
005096a0: ldrb     r3, [r8], #1
005096a4: cmp      r3, #0
005096a8: beq      #0x5096e8
005096ac: cmp      sl, #0
005096b0: sub      r1, r8, #1
005096b4: bne      #0x509644
005096b8: sxtb     r3, r3
005096bc: cmp      r3, #0x24
005096c0: bne      #0x50970c
005096c4: strb     r3, [sp, #0x78]
005096c8: ldrsb    r3, [sb]
005096cc: ldr      fp, [sp, #0x30]
005096d0: mov      sl, #1
005096d4: cmp      r3, #0x24
005096d8: ldrb     r3, [r8], #1
005096dc: addeq    sb, sb, #1
005096e0: cmp      r3, #0
005096e4: bne      #0x5096ac
005096e8: mov      r6, r3
005096ec: mov      r8, #1
005096f0: b        #0x50907c
005096f4: mov      r0, r7
005096f8: mov      r2, r4
005096fc: bl       #0x310804
00509700: mov      r8, #1
00509704: mov      r6, #0
00509708: b        #0x509238
0050970c: mov      r0, r7
00509710: mov      r2, r8
00509714: bl       #0x310804
00509718: b        #0x5096a0
0050971c: ldr      r3, [sp, #0x54]
00509720: mov      r6, #1
00509724: add      r2, r3, #4
00509728: str      r2, [sp, #0x54]
0050972c: ldr      r1, [r3]
00509730: mov      r0, r1
00509734: str      r1, [sp, #0x10]
00509738: bl       #0x30de54
0050973c: ldr      r1, [sp, #0x10]
00509740: add      r2, r1, r0
00509744: mov      r0, r7
00509748: bl       #0x310804
0050974c: b        #0x5096a0
00509750: ldr      r2, [pc, #0x298]
00509754: add      r5, sp, #0x78
00509758: mov      r0, r5
0050975c: add      r2, pc, r2
00509760: mov      r1, #0x20
00509764: mov      r3, lr
00509768: bl       #0x30e244
0050976c: b        #0x509364
00509770: ldr      r2, [pc, #0x27c]
00509774: add      r5, sp, #0x78
00509778: mov      r0, r5
0050977c: add      r2, pc, r2
00509780: mov      r1, #0x20
00509784: ldr      r3, [sp, #0x1c]
00509788: bl       #0x30e244
0050978c: b        #0x5094c8
00509790: ldr      r3, [sp, #0x54]
00509794: add      r3, r3, #7
00509798: bic      r3, r3, #7
0050979c: add      r2, r3, #8
005097a0: str      r2, [sp, #0x54]
005097a4: ldrd     r0, r1, [r3]
005097a8: bl       #0x30e6a0
005097ac: str      r0, [sp, #0x20]
005097b0: b        #0x5093b0
005097b4: ldr      r3, [sp, #0x54]
005097b8: add      r2, r3, #4
005097bc: str      r2, [sp, #0x54]
005097c0: ldr      r3, [r3]
005097c4: str      r3, [sp, #0x1c]
005097c8: b        #0x509558
005097cc: mov      r1, #0
005097d0: ldr      r0, [sp, #0x20]
005097d4: bl       #0x30e70c
005097d8: cmp      r0, #0
005097dc: movweq   r1, #0xcccd
005097e0: movwne   r1, #0xcccd
005097e4: movteq   r1, #0x3d4c
005097e8: movtne   r1, #0xbd4c
005097ec: b        #0x5093dc
005097f0: mov      r0, r6
005097f4: bl       #0x30e8a4
005097f8: ldr      r2, [sp, #0x4c]
005097fc: strd     r0, r1, [sp]
00509800: mov      r0, r5
00509804: mov      r1, #0x10
00509808: bl       #0x30e244
0050980c: b        #0x509530
00509810: ldr      r3, [sp, #0x54]
00509814: add      r3, r3, #7
00509818: bic      r3, r3, #7
0050981c: add      r2, r3, #8
00509820: str      r2, [sp, #0x54]
00509824: ldrd     r0, r1, [r3]
00509828: bl       #0x30e6a0
0050982c: mov      r1, #0x42000000
00509830: add      r1, r1, #0xc80000
00509834: bl       #0x30ec94
00509838: str      r0, [sp, #0x20]
0050983c: b        #0x5093b0
00509840: ldr      r2, [pc, #0x1b0]
00509844: add      r5, sp, #0x78
00509848: mov      r3, ip
0050984c: add      r2, pc, r2
00509850: mov      r0, r5
00509854: mov      r1, #0x20
00509858: bl       #0x30e244
0050985c: b        #0x5094c8
00509860: ldr      r2, [pc, #0x194]
00509864: add      r5, sp, #0x78
00509868: mov      r3, ip
0050986c: add      r2, pc, r2
00509870: mov      r0, r5
00509874: mov      r1, #0x20
00509878: bl       #0x30e244
0050987c: b        #0x509364
00509880: ldr      r2, [pc, #0x178]
00509884: str      ip, [sp, #0xc]
00509888: ldr      ip, [sp, #0x38]
0050988c: add      r5, sp, #0x78
00509890: add      r2, pc, r2
00509894: mov      r0, r5
00509898: mov      r1, #0x20
0050989c: stm      sp, {ip, lr}
005098a0: str      ip, [sp, #8]
005098a4: bl       #0x30e244
005098a8: b        #0x509364
005098ac: ldr      r2, [pc, #0x150]
005098b0: str      ip, [sp, #0xc]
005098b4: ldr      ip, [sp, #0x38]
005098b8: add      r5, sp, #0x78
005098bc: add      r2, pc, r2
005098c0: mov      r0, r5
005098c4: mov      r1, #0x20
005098c8: stm      sp, {ip, lr}
005098cc: str      ip, [sp, #8]
005098d0: bl       #0x30e244
005098d4: b        #0x5094c8
005098d8: ldr      r2, [sp, #0x54]
005098dc: movw     r3, #0x4dd3
005098e0: movt     r3, #0x1062
005098e4: add      r1, r2, #4
005098e8: str      r1, [sp, #0x54]
005098ec: ldr      r2, [r2]
005098f0: smull    ip, r3, r3, r2
005098f4: asr      r2, r2, #0x1f
005098f8: rsb      r2, r2, r3, asr #6
005098fc: str      r2, [sp, #0x1c]
00509900: b        #0x509558
00509904: ldr      r3, [sp, #0x54]
00509908: add      r3, r3, #7
0050990c: bic      r3, r3, #7
00509910: add      r2, r3, #8
00509914: str      r2, [sp, #0x54]
00509918: ldrd     r0, r1, [r3]
0050991c: bl       #0x30e6a0
00509920: mov      r1, #0x44000000
00509924: add      r1, r1, #0x7a0000
00509928: bl       #0x30ec94
0050992c: str      r0, [sp, #0x20]
00509930: b        #0x5093b0
00509934: ldr      r3, [sp, #0x54]
00509938: add      r2, r3, #4
0050993c: str      r2, [sp, #0x54]
00509940: ldr      r3, [r3]
00509944: mov      r2, #0x64
00509948: mul      r2, r2, r3
0050994c: str      r2, [sp, #0x1c]
00509950: b        #0x509558
00509954: ldr      r3, [sp, #0x54]
00509958: add      r3, r3, #7
0050995c: bic      r3, r3, #7
00509960: add      r2, r3, #8
00509964: str      r2, [sp, #0x54]
00509968: ldrd     r0, r1, [r3]
0050996c: bl       #0x30e6a0
00509970: mov      r1, #0x42000000
00509974: add      r1, r1, #0xc80000
00509978: bl       #0x30ed6c
0050997c: str      r0, [sp, #0x20]
00509980: b        #0x5093b0
00509984: ldr      r3, [sp, #0x54]
00509988: add      r3, r3, #7
0050998c: bic      r3, r3, #7
00509990: add      r2, r3, #8
00509994: str      r2, [sp, #0x54]
00509998: ldrd     r0, r1, [r3]
0050999c: bl       #0x30e6a0
005099a0: mov      r1, #0x40000000
005099a4: add      r1, r1, #0xa00000
005099a8: bl       #0x30ec94
005099ac: str      r0, [sp, #0x20]
005099b0: b        #0x5093b0
005099b4: bl       #0x30e310
005099b8: subeq    fp, r8, r8, lsl #23
005099bc: andeq    r4, r0, ip, lsr #1
005099c0: strdeq   r3, r4, [r0], -r4
005099c4: eorseq   r5, fp, r4, lsr #25
005099c8: eorseq   r2, sp, r8, lsl lr
005099cc: eorseq   r2, sp, ip, lsl #28
005099d0: eorseq   r2, sp, r8, lsl #28
005099d4: eorseq   r2, sp, ip, ror #27
005099d8: eorseq   r2, ip, r8, asr #19
005099dc: ldrshteq r2, [sp], -r4
005099e0: ldrsbteq r2, [sp], -ip
005099e4: eorseq   r2, sp, ip, asr sb
005099e8: eorseq   r2, sp, r8, ror #18
005099ec: eorseq   r2, sp, r8, lsr r8
005099f0: eorseq   r8, fp, r4, asr r7
005099f4: eorseq   r8, fp, r4, lsr r7
005099f8: eorseq   r8, fp, r4, ror #12
005099fc: eorseq   r8, fp, r4, asr #12
00509a00: eorseq   r2, sp, r8, ror r5
00509a04: eorseq   r2, sp, ip, asr #10

# _ZNK9Character16SG_GetPlayerNameEv
003bb7e8: movw     r3, #0x14e8
003bb7ec: ldr      r0, [r0, r3]
003bb7f0: cmp      r0, #0
003bb7f4: ldrne    r0, [r0, #0x2c]
003bb7f8: bx       lr

# _ZNK13StringManager9getStringEij
00508e1c: ldr      r3, [pc, #0x9c]
00508e20: ldr      ip, [pc, #0x9c]
00508e24: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508e28: add      r3, pc, r3
00508e2c: ldr      r4, [pc, #0x94]
00508e30: ldr      r5, [r3, ip]
00508e34: mov      fp, r2
00508e38: ldr      r2, [pc, #0x8c]
00508e3c: add      r4, pc, r4
00508e40: sub      sp, sp, #4
00508e44: mov      r6, r1
00508e48: mov      sb, r0
00508e4c: mov      r1, r4
00508e50: add      r2, pc, r2
00508e54: ldr      r0, [r5, #0x2c]
00508e58: bl       #0x4c4bdc
00508e5c: ldr      r2, [pc, #0x6c]
00508e60: mov      r8, r0
00508e64: mov      r1, r4
00508e68: ldr      r0, [r5, #0x2c]
00508e6c: add      r2, pc, r2
00508e70: bl       #0x4c4bdc
00508e74: ldr      r2, [pc, #0x58]
00508e78: mov      r7, r0
00508e7c: mov      r1, r4
00508e80: ldr      r0, [r5, #0x2c]
00508e84: add      r2, pc, r2
00508e88: bl       #0x4c4bdc
00508e8c: ldr      r2, [pc, #0x44]
00508e90: mov      sl, r0
00508e94: mov      r1, r4
00508e98: ldr      r0, [r5, #0x2c]
00508e9c: add      r2, pc, r2
00508ea0: bl       #0x4c4bdc
00508ea4: and      r1, r7, r6, asr r8
00508ea8: and      r2, r0, r6, asr sl
00508eac: mov      r3, fp
00508eb0: mov      r0, sb
00508eb4: add      sp, sp, #4
00508eb8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00508ebc: b        #0x5088c4
00508ec0: subeq    fp, r8, r8, ror #24
00508ec4: strdeq   r3, r4, [r0], -r4
00508ec8: eorseq   r2, sp, r4, lsl pc
00508ecc: eorseq   r2, sp, r0, lsl pc
00508ed0: eorseq   r2, sp, r4, lsl #30
00508ed4: ldrshteq r2, [sp], -ip
00508ed8: ldrshteq r2, [sp], -r4

# _ZN13StringManager11parseColorsERSsPKc
00507ea4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00507ea8: ldr      r6, [pc, #0x5d8]
00507eac: ldr      sb, [pc, #0x5d8]
00507eb0: sub      sp, sp, #0x44
00507eb4: add      r6, pc, r6
00507eb8: ldr      r3, [r6, sb]
00507ebc: cmp      r2, #0
00507ec0: mov      fp, r0
00507ec4: ldr      r3, [r3]
00507ec8: mov      r7, r1
00507ecc: str      r3, [sp, #0x3c]
00507ed0: beq      #0x5080d0
00507ed4: ldrb     r1, [r2]
00507ed8: cmp      r1, #0
00507edc: beq      #0x5080d0
00507ee0: ldr      r3, [pc, #0x5a8]
00507ee4: mov      r8, #0
00507ee8: add      r4, r2, #1
00507eec: add      r3, pc, r3
00507ef0: str      r3, [sp, #4]
00507ef4: ldr      r3, [pc, #0x598]
00507ef8: mov      r5, r8
00507efc: add      r3, pc, r3
00507f00: str      r3, [sp, #0xc]
00507f04: ldr      r3, [pc, #0x58c]
00507f08: add      r3, pc, r3
00507f0c: str      r3, [sp, #0x10]
00507f10: ldr      r3, [pc, #0x584]
00507f14: add      r3, pc, r3
00507f18: str      r3, [sp, #0x14]
00507f1c: ldr      r3, [sp, #4]
00507f20: add      r3, r3, #1
00507f24: str      r3, [sp, #8]
00507f28: b        #0x507f60
00507f2c: sxtb     r1, r1
00507f30: cmp      r1, #0x5e
00507f34: moveq    r5, #1
00507f38: beq      #0x507f54
00507f3c: cmp      r1, #0x7c
00507f40: beq      #0x50817c
00507f44: mov      r1, sl
00507f48: mov      r0, r7
00507f4c: mov      r2, r4
00507f50: bl       #0x310804
00507f54: ldrb     r1, [r4], #1
00507f58: cmp      r1, #0
00507f5c: beq      #0x5080f4
00507f60: cmp      r5, #0
00507f64: sub      sl, r4, #1
00507f68: beq      #0x507f2c
00507f6c: sxtb     r1, r1
00507f70: sub      r1, r1, #0x23
00507f74: cmp      r1, #0x53
00507f78: addls    pc, pc, r1, lsl #2
00507f7c: b        #0x508174
00507f80: b        #0x508158
00507f84: b        #0x508174
00507f88: b        #0x508174
00507f8c: b        #0x508174
00507f90: b        #0x508174
00507f94: b        #0x508174
00507f98: b        #0x508174
00507f9c: b        #0x508158
00507fa0: b        #0x508174
00507fa4: b        #0x508174
00507fa8: b        #0x508174
00507fac: b        #0x508174
00507fb0: b        #0x508174
00507fb4: b        #0x5081b8
00507fb8: b        #0x508384
00507fbc: b        #0x5083c4
00507fc0: b        #0x508404
00507fc4: b        #0x508444
00507fc8: b        #0x508218
00507fcc: b        #0x508258
00507fd0: b        #0x508298
00507fd4: b        #0x5082d8
00507fd8: b        #0x508318
00507fdc: b        #0x508174
00507fe0: b        #0x508174
00507fe4: b        #0x508174
00507fe8: b        #0x508174
00507fec: b        #0x508174
00507ff0: b        #0x508174
00507ff4: b        #0x508174
00507ff8: b        #0x508174
00507ffc: b        #0x508174
00508000: b        #0x508174
00508004: b        #0x508174
00508008: b        #0x508174
0050800c: b        #0x508174
00508010: b        #0x508174
00508014: b        #0x508174
00508018: b        #0x508174
0050801c: b        #0x508174
00508020: b        #0x508174
00508024: b        #0x508174
00508028: b        #0x508174
0050802c: b        #0x508174
00508030: b        #0x508174
00508034: b        #0x508174
00508038: b        #0x508174
0050803c: b        #0x508174
00508040: b        #0x508174
00508044: b        #0x508174
00508048: b        #0x508174
0050804c: b        #0x508174
00508050: b        #0x508174
00508054: b        #0x508174
00508058: b        #0x508174
0050805c: b        #0x508174
00508060: b        #0x508174
00508064: b        #0x508174
00508068: b        #0x508174
0050806c: b        #0x508158
00508070: b        #0x508174
00508074: b        #0x508174
00508078: b        #0x508174
0050807c: b        #0x508174
00508080: b        #0x508174
00508084: b        #0x508158
00508088: b        #0x508174
0050808c: b        #0x508158
00508090: b        #0x508158
00508094: b        #0x508158
00508098: b        #0x508158
0050809c: b        #0x508174
005080a0: b        #0x508158
005080a4: b        #0x508174
005080a8: b        #0x508174
005080ac: b        #0x508354
005080b0: b        #0x508174
005080b4: b        #0x508158
005080b8: b        #0x508174
005080bc: b        #0x50836c
005080c0: b        #0x508158
005080c4: b        #0x508158
005080c8: b        #0x508174
005080cc: b        #0x508158
005080d0: mov      r8, #0
005080d4: ldr      r3, [r6, sb]
005080d8: ldr      r2, [sp, #0x3c]
005080dc: mov      r0, r8
005080e0: ldr      r3, [r3]
005080e4: cmp      r2, r3
005080e8: bne      #0x508484
005080ec: add      sp, sp, #0x44
005080f0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005080f4: ldr      r3, [r7, #0x14]
005080f8: ldr      r0, [r7, #0x10]
005080fc: rsb      r0, r3, r0
00508100: add      r0, r0, #0x80
00508104: bl       #0x31056c
00508108: mov      r4, r0
0050810c: mov      r0, fp
00508110: ldr      r5, [r7, #0x14]
00508114: bl       #0x50750c
00508118: mov      r1, r4
0050811c: mov      r3, r0
00508120: mvn      r2, #0
00508124: mov      r0, r5
00508128: bl       #0x752a18
0050812c: mov      r0, r4
00508130: bl       #0x30de54
00508134: mov      r1, r4
00508138: add      r2, r4, r0
0050813c: mov      r0, r7
00508140: bl       #0x3109e0
00508144: cmp      r4, #0
00508148: beq      #0x5080d4
0050814c: mov      r0, r4
00508150: bl       #0x310440
00508154: b        #0x5080d4
00508158: ldmib    sp, {r1, r2}
0050815c: mov      r0, r7
00508160: bl       #0x310804
00508164: mov      r0, r7
00508168: mov      r1, sl
0050816c: mov      r2, r4
00508170: bl       #0x310804
00508174: mov      r5, #0
00508178: b        #0x507f54
0050817c: ldr      r2, [pc, #0x31c]
00508180: add      r8, sp, #0x1c
00508184: mov      r1, #0x20
00508188: add      r2, pc, r2
0050818c: mov      r3, #0x11
00508190: mov      r0, r8
00508194: bl       #0x30e244
00508198: mov      r0, r8
0050819c: bl       #0x30de54
005081a0: mov      r1, r8
005081a4: add      r2, r8, r0
005081a8: mov      r0, r7
005081ac: bl       #0x310804
005081b0: mov      r8, #1
005081b4: b        #0x507f54
005081b8: ldr      r3, [pc, #0x2e4]
005081bc: ldr      r1, [pc, #0x2e4]
005081c0: ldr      r2, [pc, #0x2e4]
005081c4: ldr      r3, [r6, r3]
005081c8: add      r1, pc, r1
005081cc: add      r2, pc, r2
005081d0: ldr      r0, [r3, #0x2c]
005081d4: bl       #0x4c4bdc
005081d8: ldr      r2, [pc, #0x2d0]
005081dc: add      r5, sp, #0x1c
005081e0: bic      r3, r0, #0xff000000
005081e4: add      r2, pc, r2
005081e8: mov      r1, #0x20
005081ec: mov      r0, r5
005081f0: bl       #0x30e244
005081f4: mov      r0, r5
005081f8: bl       #0x30de54
005081fc: mov      r1, r5
00508200: add      r2, r5, r0
00508204: mov      r0, r7
00508208: bl       #0x310804
0050820c: mov      r8, #1
00508210: mov      r5, #0
00508214: b        #0x507f54
00508218: ldr      r3, [pc, #0x284]
0050821c: ldr      r1, [pc, #0x290]
00508220: ldr      r2, [pc, #0x290]
00508224: ldr      r3, [r6, r3]
00508228: add      r1, pc, r1
0050822c: add      r2, pc, r2
00508230: ldr      r0, [r3, #0x2c]
00508234: bl       #0x4c4bdc
00508238: ldr      r2, [pc, #0x27c]
0050823c: add      r5, sp, #0x1c
00508240: bic      r3, r0, #0xff000000
00508244: add      r2, pc, r2
00508248: mov      r1, #0x20
0050824c: mov      r0, r5
00508250: bl       #0x30e244
00508254: b        #0x5081f4
00508258: ldr      r3, [pc, #0x244]
0050825c: ldr      r1, [pc, #0x25c]
00508260: ldr      r2, [pc, #0x25c]
00508264: ldr      r3, [r6, r3]
00508268: add      r1, pc, r1
0050826c: add      r2, pc, r2
00508270: ldr      r0, [r3, #0x2c]
00508274: bl       #0x4c4bdc
00508278: ldr      r2, [pc, #0x248]
0050827c: add      r5, sp, #0x1c
00508280: bic      r3, r0, #0xff000000
00508284: add      r2, pc, r2
00508288: mov      r1, #0x20
0050828c: mov      r0, r5
00508290: bl       #0x30e244
00508294: b        #0x5081f4
00508298: ldr      r3, [pc, #0x204]
0050829c: ldr      r1, [pc, #0x228]
005082a0: ldr      r2, [pc, #0x228]
005082a4: ldr      r3, [r6, r3]
005082a8: add      r1, pc, r1
005082ac: add      r2, pc, r2
005082b0: ldr      r0, [r3, #0x2c]
005082b4: bl       #0x4c4bdc
005082b8: ldr      r2, [pc, #0x214]
005082bc: add      r5, sp, #0x1c
005082c0: bic      r3, r0, #0xff000000
005082c4: add      r2, pc, r2
005082c8: mov      r1, #0x20
005082cc: mov      r0, r5
005082d0: bl       #0x30e244
005082d4: b        #0x5081f4
005082d8: ldr      r3, [pc, #0x1c4]
005082dc: ldr      r1, [pc, #0x1f4]
005082e0: ldr      r2, [pc, #0x1f4]
005082e4: ldr      r3, [r6, r3]
005082e8: add      r1, pc, r1
005082ec: add      r2, pc, r2
005082f0: ldr      r0, [r3, #0x2c]
005082f4: bl       #0x4c4bdc
005082f8: ldr      r2, [pc, #0x1e0]
005082fc: add      r5, sp, #0x1c
00508300: bic      r3, r0, #0xff000000
00508304: add      r2, pc, r2
00508308: mov      r1, #0x20
0050830c: mov      r0, r5
00508310: bl       #0x30e244
00508314: b        #0x5081f4
00508318: ldr      r3, [pc, #0x184]
0050831c: ldr      r2, [pc, #0x1c0]
00508320: ldr      r1, [sp, #0x14]
00508324: ldr      r3, [r6, r3]
00508328: add      r2, pc, r2
0050832c: add      r5, sp, #0x1c
00508330: ldr      r0, [r3, #0x2c]
00508334: bl       #0x4c4bdc
00508338: ldr      r2, [pc, #0x1a8]
0050833c: bic      r3, r0, #0xff000000
00508340: mov      r1, #0x20
00508344: add      r2, pc, r2
00508348: mov      r0, r5
0050834c: bl       #0x30e244
00508350: b        #0x5081f4
00508354: add      r5, sp, #0x1c
00508358: mov      r1, #0x20
0050835c: ldr      r2, [sp, #0xc]
00508360: mov      r0, r5
00508364: bl       #0x30e244
00508368: b        #0x5081f4
0050836c: add      r5, sp, #0x1c
00508370: mov      r1, #0x20
00508374: ldr      r2, [sp, #0x10]
00508378: mov      r0, r5
0050837c: bl       #0x30e244
00508380: b        #0x5081f4
00508384: ldr      r3, [pc, #0x118]
00508388: ldr      r1, [pc, #0x15c]
0050838c: ldr      r2, [pc, #0x15c]
00508390: ldr      r3, [r6, r3]
00508394: add      r1, pc, r1
00508398: add      r2, pc, r2
0050839c: ldr      r0, [r3, #0x2c]
005083a0: bl       #0x4c4bdc
005083a4: ldr      r2, [pc, #0x148]
005083a8: add      r5, sp, #0x1c
005083ac: bic      r3, r0, #0xff000000
005083b0: add      r2, pc, r2
005083b4: mov      r1, #0x20
005083b8: mov      r0, r5
005083bc: bl       #0x30e244
005083c0: b        #0x5081f4
005083c4: ldr      r3, [pc, #0xd8]
005083c8: ldr      r1, [pc, #0x128]
005083cc: ldr      r2, [pc, #0x128]
005083d0: ldr      r3, [r6, r3]
005083d4: add      r1, pc, r1
005083d8: add      r2, pc, r2
005083dc: ldr      r0, [r3, #0x2c]
005083e0: bl       #0x4c4bdc
005083e4: ldr      r2, [pc, #0x114]
005083e8: add      r5, sp, #0x1c
005083ec: bic      r3, r0, #0xff000000
005083f0: add      r2, pc, r2
005083f4: mov      r1, #0x20
005083f8: mov      r0, r5
005083fc: bl       #0x30e244
00508400: b        #0x5081f4
00508404: ldr      r3, [pc, #0x98]
00508408: ldr      r1, [pc, #0xf4]
0050840c: ldr      r2, [pc, #0xf4]
00508410: ldr      r3, [r6, r3]
00508414: add      r1, pc, r1
00508418: add      r2, pc, r2
0050841c: ldr      r0, [r3, #0x2c]
00508420: bl       #0x4c4bdc
00508424: ldr      r2, [pc, #0xe0]
00508428: add      r5, sp, #0x1c
0050842c: bic      r3, r0, #0xff000000
00508430: add      r2, pc, r2
00508434: mov      r1, #0x20
00508438: mov      r0, r5
0050843c: bl       #0x30e244
00508440: b        #0x5081f4
00508444: ldr      r3, [pc, #0x58]
00508448: ldr      r1, [pc, #0xc0]
0050844c: ldr      r2, [pc, #0xc0]
00508450: ldr      r3, [r6, r3]
00508454: add      r1, pc, r1
00508458: add      r2, pc, r2
0050845c: ldr      r0, [r3, #0x2c]
00508460: bl       #0x4c4bdc
00508464: ldr      r2, [pc, #0xac]
00508468: add      r5, sp, #0x1c
0050846c: bic      r3, r0, #0xff000000
00508470: add      r2, pc, r2
00508474: mov      r1, #0x20
00508478: mov      r0, r5
0050847c: bl       #0x30e244
00508480: b        #0x5081f4
00508484: bl       #0x30e310
00508488: ldrdeq   ip, sp, [r8], #-0xbc
0050848c: andeq    r4, r0, ip, lsr #1
00508490: eorseq   r3, sp, r4, ror sp
00508494: ldrshteq r3, [ip], -r4
00508498: eorseq   lr, fp, r0, asr #8
0050849c: eorseq   r1, ip, r4, lsl #2
005084a0: mlaseq   sp, r0, sl, r3
005084a4: strdeq   r3, r4, [r0], -r4
005084a8: eorseq   r0, ip, r0, asr lr
005084ac: ldrhteq  lr, [fp], -ip
005084b0: eorseq   r3, sp, ip, lsr sl
005084b4: ldrshteq r0, [ip], -r0
005084b8: eorseq   r3, sp, ip, lsl #20
005084bc: ldrsbteq r3, [sp], -ip
005084c0: ldrhteq  r0, [ip], -r0
005084c4: ldrsbteq r3, [sp], -r4
005084c8: mlaseq   sp, ip, sb, r3
005084cc: eorseq   r0, ip, r0, ror sp
005084d0: mlaseq   sp, ip, sb, r3
005084d4: eorseq   r3, sp, ip, asr sb
005084d8: eorseq   r0, ip, r0, lsr sp
005084dc: eorseq   r3, sp, r4, ror #18
005084e0: eorseq   r3, sp, ip, lsl sb
005084e4: eorseq   r3, sp, r0, lsr sb
005084e8: ldrsbteq r3, [sp], -ip
005084ec: eorseq   r0, ip, r4, lsl #25
005084f0: eorseq   r8, fp, r8, ror r1
005084f4: eorseq   r3, sp, r0, ror r8
005084f8: eorseq   r0, ip, r4, asr #24
005084fc: ldrhteq  lr, [fp], -r8
00508500: eorseq   r3, sp, r0, lsr r8
00508504: eorseq   r0, ip, r4, lsl #24
00508508: eorseq   lr, fp, r0, lsl #25
0050850c: ldrshteq r3, [sp], -r0
00508510: eorseq   r0, ip, r4, asr #23
00508514: eorseq   lr, fp, r8, asr #24
00508518: ldrhteq  r3, [sp], -r0

# _ZN7Structs13LangSheetList4readEP11IStreamBase
004dc2ac: push     {r4, r5, r6, r7, r8, lr}
004dc2b0: mov      r5, r0
004dc2b4: sub      sp, sp, #8
004dc2b8: mov      r0, r1
004dc2bc: mov      r7, r1
004dc2c0: ldr      r6, [pc, #0x154]
004dc2c4: add      r1, r5, #4
004dc2c8: bl       #0x3df1a0
004dc2cc: mov      r3, #1
004dc2d0: cmp      r3, #0
004dc2d4: str      r3, [sp, #4]
004dc2d8: add      r6, pc, r6
004dc2dc: bne      #0x4dc320
004dc2e0: add      r3, r5, #5
004dc2e4: add      r2, r5, #6
004dc2e8: ldrb     r0, [r2, #1]
004dc2ec: ldrb     r1, [r3, #-1]
004dc2f0: cmp      r3, r2
004dc2f4: eor      r1, r0, r1
004dc2f8: strb     r1, [r3, #-1]
004dc2fc: ldrb     r0, [r2, #1]
004dc300: eor      r1, r1, r0
004dc304: strb     r1, [r2, #1]
004dc308: ldrb     r0, [r3, #-1]
004dc30c: sub      r2, r2, #1
004dc310: eor      r1, r1, r0
004dc314: strb     r1, [r3, #-1]
004dc318: add      r3, r3, #1
004dc31c: blo      #0x4dc2e8
004dc320: ldr      r3, [r5, #8]
004dc324: cmp      r3, #0
004dc328: beq      #0x4dc370
004dc32c: ldr      r2, [r3, #-4]
004dc330: mov      r0, #0x14
004dc334: mla      r0, r0, r2, r3
004dc338: cmp      r3, r0
004dc33c: bne      #0x4dc348
004dc340: b        #0x4dc368
004dc344: mov      r0, r4
004dc348: sub      r4, r0, #0x14
004dc34c: ldr      r3, [r0, #-0x14]
004dc350: mov      r0, r4
004dc354: mov      lr, pc
004dc358: ldr      pc, [r3]
004dc35c: ldr      r0, [r5, #8]
004dc360: cmp      r0, r4
004dc364: bne      #0x4dc344
004dc368: sub      r0, r0, #8
004dc36c: bl       #0x310440
004dc370: ldr      r4, [r5, #4]
004dc374: mov      r8, #0x14
004dc378: mov      r1, #1
004dc37c: mul      r0, r8, r4
004dc380: add      r0, r0, #8
004dc384: bl       #0x31056c
004dc388: cmp      r4, #0
004dc38c: str      r8, [r0]
004dc390: str      r4, [r0, #4]
004dc394: add      r3, r0, #8
004dc398: beq      #0x4dc3cc
004dc39c: ldr      r1, [pc, #0x7c]
004dc3a0: mov      r2, #0
004dc3a4: ldr      ip, [r6, r1]
004dc3a8: mov      r1, r2
004dc3ac: add      ip, ip, #8
004dc3b0: add      r2, r2, #1
004dc3b4: cmp      r2, r4
004dc3b8: str      ip, [r0, #8]
004dc3bc: str      r1, [r0, #0x10]
004dc3c0: str      r1, [r0, #0x18]
004dc3c4: add      r0, r0, #0x14
004dc3c8: bne      #0x4dc3b0
004dc3cc: ldr      r2, [r5, #4]
004dc3d0: str      r3, [r5, #8]
004dc3d4: cmp      r2, #0
004dc3d8: beq      #0x4dc414
004dc3dc: mov      r4, #0
004dc3e0: mov      r6, r4
004dc3e4: b        #0x4dc3ec
004dc3e8: ldr      r3, [r5, #8]
004dc3ec: add      r0, r3, r4
004dc3f0: mov      r1, r7
004dc3f4: ldr      r3, [r3, r4]
004dc3f8: mov      lr, pc
004dc3fc: ldr      pc, [r3, #0xc]
004dc400: ldr      r3, [r5, #4]
004dc404: add      r6, r6, #1
004dc408: add      r4, r4, #0x14
004dc40c: cmp      r3, r6
004dc410: bhi      #0x4dc3e8
004dc414: add      sp, sp, #8
004dc418: pop      {r4, r5, r6, r7, r8, pc}
004dc41c: strheq   r8, [fp], #-0x78
004dc420: andeq    r3, r0, r8, lsr #15

# _ZN7gameswf15format_utf_textEPKcPcib
00752a18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00752a1c: add      r7, r0, #1
00752a20: mov      r8, r3
00752a24: ldrb     r3, [r7, #-1]
00752a28: sub      sp, sp, #0xc
00752a2c: mov      r5, #0
00752a30: cmp      r3, #0
00752a34: mov      r4, r1
00752a38: mov      r6, r2
00752a3c: mov      sl, r5
00752a40: add      fp, sp, #4
00752a44: mov      sb, #0x11
00752a48: beq      #0x752a94
00752a4c: sxtb     r2, r3
00752a50: cmp      r2, #0x20
00752a54: beq      #0x752ac8
00752a58: cmp      r2, r6
00752a5c: strbeq   sb, [r4], #1
00752a60: beq      #0x752a84
00752a64: cmp      r2, #0x21
00752a68: cmpne    r2, #0x3f
00752a6c: moveq    r5, #1
00752a70: cmp      r8, #0
00752a74: strb     r3, [r4], #1
00752a78: beq      #0x752a84
00752a7c: cmp      r5, #0
00752a80: bne      #0x752aa0
00752a84: add      r7, r7, #1
00752a88: ldrb     r3, [r7, #-1]
00752a8c: cmp      r3, #0
00752a90: bne      #0x752a4c
00752a94: strb     r3, [r4]
00752a98: add      sp, sp, #0xc
00752a9c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00752aa0: mov      r0, r4
00752aa4: mov      r1, fp
00752aa8: mov      r2, #0x20
00752aac: str      sl, [sp, #4]
00752ab0: bl       #0x7527e4
00752ab4: ldr      r3, [sp, #4]
00752ab8: mov      r5, sl
00752abc: add      r7, r7, #1
00752ac0: add      r4, r4, r3
00752ac4: b        #0x752a88
00752ac8: str      sl, [sp, #4]
00752acc: ldrb     r3, [r7]
00752ad0: sub      r3, r3, #0x21
00752ad4: uxtb     r2, r3
00752ad8: cmp      r2, #0x1e
00752adc: bhi      #0x752b20
00752ae0: sxtb     r3, r3
00752ae4: mov      r1, #1
00752ae8: lsl      r2, r1, r3
00752aec: movw     r3, #0x2001
00752af0: movt     r3, #0x4600
00752af4: and      r3, r2, r3
00752af8: cmp      r3, #0
00752afc: beq      #0x752b20
00752b00: mov      r0, r4
00752b04: mov      r1, fp
00752b08: mov      r2, #0xa0
00752b0c: bl       #0x7527e4
00752b10: ldr      r3, [sp, #4]
00752b14: add      r7, r7, #1
00752b18: add      r4, r4, r3
00752b1c: b        #0x752a88
00752b20: ldrb     r3, [r7, #-1]
00752b24: sxtb     r2, r3
00752b28: b        #0x752a64

# _ZN13StringManager8addSpaceEv
0050750c: ldr      r3, [r0, #4]
00507510: cmp      r3, #7
00507514: movhi    r0, #0
00507518: bxhi     lr
0050751c: ldr      r2, [pc, #0xc]
00507520: add      r2, pc, r2
00507524: add      r3, r2, r3
00507528: ldrb     r0, [r3, #0x40]
0050752c: bx       lr
00507530: eorseq   r4, sp, r0, lsr #11

# _Z15ParsePlayerNameRKSs
00433d00: push     {r4, r5, r6, r7, r8, lr}
00433d04: mov      r4, r0
00433d08: sub      sp, sp, #8
00433d0c: str      r0, [r4, #0x10]
00433d10: str      r0, [r4, #0x14]
00433d14: mov      r8, r1
00433d18: ldr      r5, [pc, #0xa8]
00433d1c: mov      r1, #0x10
00433d20: bl       #0x31167c
00433d24: ldr      r2, [pc, #0xa0]
00433d28: ldr      r3, [r4, #0x10]
00433d2c: add      r5, pc, r5
00433d30: ldr      r7, [r5, r2]
00433d34: mov      r6, #0
00433d38: strb     r6, [r3]
00433d3c: ldr      r0, [r7, #0x40]
00433d40: mov      r1, r6
00433d44: mov      r2, #1
00433d48: bl       #0x36e478
00433d4c: ldr      r3, [r0, #0x660]
00433d50: cmp      r3, r6
00433d54: beq      #0x433dac
00433d58: mov      r1, r6
00433d5c: mov      r2, #1
00433d60: ldr      r0, [r7, #0x40]
00433d64: bl       #0x36e478
00433d68: ldr      r0, [r0, #0x660]
00433d6c: ldr      r6, [r7, #0x34]
00433d70: ldr      r5, [r8, #0x14]
00433d74: bl       #0x3bb7e8
00433d78: ldr      ip, [pc, #0x50]
00433d7c: ldr      r2, [pc, #0x50]
00433d80: str      r0, [sp, #4]
00433d84: add      ip, pc, ip
00433d88: add      r2, pc, r2
00433d8c: mov      r0, r6
00433d90: mov      r3, r5
00433d94: mov      r1, r4
00433d98: str      ip, [sp]
00433d9c: bl       #0x508ef4
00433da0: mov      r0, r4
00433da4: add      sp, sp, #8
00433da8: pop      {r4, r5, r6, r7, r8, pc}
00433dac: cmp      r4, r8
00433db0: beq      #0x433da0
00433db4: ldr      r2, [r8, #0x10]
00433db8: mov      r0, r4
00433dbc: ldr      r1, [r8, #0x14]
00433dc0: bl       #0x3109e0
00433dc4: b        #0x433da0
00433dc8: subseq   r0, r6, r4, ror #26
00433dcc: strdeq   r3, r4, [r0], -r4
00433dd0: subeq    r7, sb, ip, asr #15
00433dd4: subeq    r7, sb, r0, asr #15

# _ZN13StringManagerC1Ev
00507d10: ldr      r3, [pc, #0x4c]
00507d14: ldr      r2, [pc, #0x4c]
00507d18: mvn      r1, #0
00507d1c: add      r3, pc, r3
00507d20: ldr      r2, [r3, r2]
00507d24: push     {r4, lr}
00507d28: add      r2, r2, #8
00507d2c: mov      r4, r0
00507d30: str      r1, [r0, #4]
00507d34: str      r2, [r0]
00507d38: mov      r1, #0
00507d3c: movw     r2, #0x534
00507d40: add      r0, r0, #8
00507d44: bl       #0x30e460
00507d48: add      r0, r4, #0x530
00507d4c: mov      r1, #0
00507d50: movw     r2, #0x29a
00507d54: add      r0, r0, #0xc
00507d58: bl       #0x30e460
00507d5c: mov      r0, r4
00507d60: pop      {r4, pc}
00507d64: subeq    ip, r8, r4, ror sp
00507d68: andeq    r0, r0, r8, lsl #21

# _ZNK13StringManager9getStringEi
00508edc: cmp      r1, #0
00508ee0: blt      #0x508eec
00508ee4: ldr      r2, [r0, #4]
00508ee8: b        #0x508e1c
00508eec: mov      r0, #0
00508ef0: bx       lr

# _ZNK13StringManager12getStringIdxEiij
005088c4: push     {r4, r5, r6, r7, r8, sl, lr}
005088c8: ldr      r6, [pc, #0x33c]
005088cc: cmn      r3, #1
005088d0: mov      r4, r3
005088d4: add      r6, pc, r6
005088d8: sub      sp, sp, #0xc
005088dc: mov      r7, r0
005088e0: mov      r5, r1
005088e4: mov      r8, r2
005088e8: moveq    r4, #0
005088ec: beq      #0x50891c
005088f0: cmp      r4, #8
005088f4: bls      #0x50891c
005088f8: ldr      r3, [pc, #0x310]
005088fc: ldr      r3, [r6, r3]
00508900: ldr      r3, [r3]
00508904: cmp      r3, #2
00508908: moveq    r3, #0
0050890c: streq    r3, [r3]
00508910: beq      #0x50891c
00508914: cmp      r3, #1
00508918: beq      #0x508b6c
0050891c: cmp      r5, #0
00508920: blt      #0x508a6c
00508924: cmp      r5, #0x24
00508928: ble      #0x508948
0050892c: ldr      r3, [pc, #0x2dc]
00508930: ldr      r3, [r6, r3]
00508934: ldr      r3, [r3]
00508938: cmp      r3, #2
0050893c: beq      #0x508a80
00508940: cmp      r3, #1
00508944: beq      #0x508bd8
00508948: mov      r0, r7
0050894c: mov      r1, r4
00508950: mov      r2, r5
00508954: bl       #0x507668
00508958: subs     r3, r0, #0
0050895c: beq      #0x508aa0
00508960: mov      r3, #0x25
00508964: mla      r3, r3, r4, r5
00508968: add      r3, r3, #2
0050896c: ldr      r3, [r7, r3, lsl #2]
00508970: cmp      r3, #0
00508974: beq      #0x508ac8
00508978: mov      sl, #0x25
0050897c: mla      sl, sl, r4, r5
00508980: add      sl, sl, #0x29c
00508984: add      sl, r7, sl, lsl #1
00508988: ldrh     r3, [sl, #4]
0050898c: cmp      r3, #0
00508990: bne      #0x5089b4
00508994: ldr      r2, [pc, #0x274]
00508998: ldr      r2, [r6, r2]
0050899c: ldr      r2, [r2]
005089a0: cmp      r2, #2
005089a4: streq    r3, [r3]
005089a8: beq      #0x5089b4
005089ac: cmp      r2, #1
005089b0: beq      #0x508ba0
005089b4: sxth     r3, r3
005089b8: cmp      r3, r8
005089bc: bgt      #0x508a38
005089c0: ldr      r3, [pc, #0x248]
005089c4: ldr      r3, [r6, r3]
005089c8: ldr      r3, [r3]
005089cc: cmp      r3, #2
005089d0: beq      #0x508b58
005089d4: cmp      r3, #1
005089d8: beq      #0x5089ec
005089dc: ldr      r0, [pc, #0x230]
005089e0: add      r0, pc, r0
005089e4: add      sp, sp, #0xc
005089e8: pop      {r4, r5, r6, r7, r8, sl, pc}
005089ec: ldr      r0, [pc, #0x224]
005089f0: ldr      r1, [pc, #0x224]
005089f4: ldr      r2, [pc, #0x224]
005089f8: ldr      r0, [r6, r0]
005089fc: ldr      r3, [pc, #0x220]
00508a00: mov      ip, #0x1f8
00508a04: add      r1, pc, r1
00508a08: add      r3, pc, r3
00508a0c: add      r0, r0, #0xa8
00508a10: add      r2, pc, r2
00508a14: str      ip, [sp]
00508a18: bl       #0x30e004
00508a1c: mov      r3, #0x25
00508a20: mla      r3, r3, r4, r5
00508a24: add      r3, r3, #0x29c
00508a28: add      r3, r7, r3, lsl #1
00508a2c: ldrsh    r3, [r3, #4]
00508a30: cmp      r8, r3
00508a34: bge      #0x5089dc
00508a38: mov      r3, #0x25
00508a3c: mla      r4, r3, r4, r5
00508a40: add      r4, r4, #2
00508a44: ldr      r3, [r7, r4, lsl #2]
00508a48: ldr      r0, [r3, r8, lsl #2]
00508a4c: cmp      r0, #0
00508a50: beq      #0x508a60
00508a54: ldrsb    r3, [r0]
00508a58: cmp      r3, #0
00508a5c: bne      #0x5089e4
00508a60: ldr      r0, [pc, #0x1c0]
00508a64: add      r0, pc, r0
00508a68: b        #0x5089e4
00508a6c: ldr      r3, [pc, #0x19c]
00508a70: ldr      r3, [r6, r3]
00508a74: ldr      r3, [r3]
00508a78: cmp      r3, #2
00508a7c: bne      #0x508b1c
00508a80: mov      r3, #0
00508a84: str      r3, [r3]
00508a88: mov      r0, r7
00508a8c: mov      r1, r4
00508a90: mov      r2, r5
00508a94: bl       #0x507668
00508a98: subs     r3, r0, #0
00508a9c: bne      #0x508960
00508aa0: mov      r0, r7
00508aa4: mov      r1, r4
00508aa8: mov      r2, r5
00508aac: bl       #0x50851c
00508ab0: mov      r3, #0x25
00508ab4: mla      r3, r3, r4, r5
00508ab8: add      r3, r3, #2
00508abc: ldr      r3, [r7, r3, lsl #2]
00508ac0: cmp      r3, #0
00508ac4: bne      #0x508978
00508ac8: ldr      r2, [pc, #0x140]
00508acc: ldr      r2, [r6, r2]
00508ad0: ldr      r2, [r2]
00508ad4: cmp      r2, #2
00508ad8: streq    r3, [r3]
00508adc: beq      #0x508978
00508ae0: cmp      r2, #1
00508ae4: bne      #0x508978
00508ae8: ldr      r0, [pc, #0x128]
00508aec: ldr      r1, [pc, #0x138]
00508af0: ldr      r2, [pc, #0x138]
00508af4: ldr      r0, [r6, r0]
00508af8: ldr      r3, [pc, #0x134]
00508afc: movw     ip, #0x1f5
00508b00: add      r1, pc, r1
00508b04: add      r2, pc, r2
00508b08: add      r3, pc, r3
00508b0c: add      r0, r0, #0xa8
00508b10: str      ip, [sp]
00508b14: bl       #0x30e004
00508b18: b        #0x508978
00508b1c: cmp      r3, #1
00508b20: bne      #0x508948
00508b24: ldr      r0, [pc, #0xec]
00508b28: ldr      r1, [pc, #0x108]
00508b2c: ldr      r2, [pc, #0x108]
00508b30: ldr      r0, [r6, r0]
00508b34: ldr      r3, [pc, #0x104]
00508b38: movw     ip, #0x1ea
00508b3c: add      r1, pc, r1
00508b40: add      r2, pc, r2
00508b44: add      r3, pc, r3
00508b48: add      r0, r0, #0xa8
00508b4c: str      ip, [sp]
00508b50: bl       #0x30e004
00508b54: b        #0x508948
00508b58: ldr      r0, [pc, #0xe4]
00508b5c: mov      r3, #0
00508b60: str      r3, [r3]
00508b64: add      r0, pc, r0
00508b68: b        #0x5089e4
00508b6c: ldr      r0, [pc, #0xa4]
00508b70: ldr      r1, [pc, #0xd0]
00508b74: ldr      r2, [pc, #0xd0]
00508b78: ldr      r0, [r6, r0]
00508b7c: ldr      r3, [pc, #0xcc]
00508b80: movw     ip, #0x1e9
00508b84: add      r1, pc, r1
00508b88: add      r2, pc, r2
00508b8c: add      r3, pc, r3
00508b90: add      r0, r0, #0xa8
00508b94: str      ip, [sp]
00508b98: bl       #0x30e004
00508b9c: b        #0x50891c
00508ba0: ldr      r0, [pc, #0x70]
00508ba4: ldr      r1, [pc, #0xa8]
00508ba8: ldr      r2, [pc, #0xa8]
00508bac: ldr      r0, [r6, r0]
00508bb0: ldr      r3, [pc, #0xa4]
00508bb4: movw     ip, #0x1f7
00508bb8: add      r1, pc, r1
00508bbc: add      r3, pc, r3
00508bc0: add      r0, r0, #0xa8
00508bc4: add      r2, pc, r2
00508bc8: str      ip, [sp]
00508bcc: bl       #0x30e004
00508bd0: ldrh     r3, [sl, #4]
00508bd4: b        #0x5089b4
00508bd8: ldr      r0, [pc, #0x38]
00508bdc: ldr      r1, [pc, #0x7c]
00508be0: ldr      r2, [pc, #0x7c]
00508be4: ldr      r0, [r6, r0]
00508be8: ldr      r3, [pc, #0x78]
00508bec: movw     ip, #0x1eb
00508bf0: add      r1, pc, r1
00508bf4: add      r2, pc, r2
00508bf8: add      r3, pc, r3
00508bfc: add      r0, r0, #0xa8
00508c00: str      ip, [sp]
00508c04: bl       #0x30e004
00508c08: b        #0x508948
00508c0c: strheq   ip, [r8], #-0x1c
00508c10: andeq    r3, r0, r0, asr #19
00508c14: eorseq   r3, sp, r0, lsr r3
00508c18: andeq    r1, r0, r0, asr #19
00508c1c: ldrsbteq r5, [fp], -r4
00508c20: eorseq   r3, sp, r8, lsl #6
00508c24: eorseq   r3, sp, r0, asr #2
00508c28: eorseq   r3, sp, r4, ror #5
00508c2c: ldrsbteq r5, [fp], -r8
00508c30: eorseq   r3, sp, ip, asr #3
00508c34: eorseq   r3, sp, r0, asr #32
00508c38: mlaseq   fp, ip, r8, r5
00508c3c: eorseq   r3, sp, r0, ror #2
00508c40: eorseq   r3, sp, r4
00508c44: eorseq   r3, sp, ip, lsr #3
00508c48: eorseq   r5, fp, r4, asr r8
00508c4c: ldrshteq r3, [sp], -r8
00508c50: ldrhteq  r2, [sp], -ip
00508c54: eorseq   r5, fp, r0, lsr #16
00508c58: eorseq   r3, sp, ip, lsr #2
00508c5c: eorseq   r2, sp, ip, lsl #31
00508c60: eorseq   r5, fp, r8, ror #15
00508c64: ldrhteq  r3, [sp], -ip
00508c68: eorseq   r2, sp, r0, asr pc

# _ZNK13StringManager13getSymbolPackEv
005075b0: mov      r0, #8
005075b4: bx       lr

# _ZN6Arrays15StrID_Languages4readEP11IStreamBase
004b53f8: push     {r4, r5, r6, r7, r8, sl, lr}
004b53fc: sub      sp, sp, #0xc
004b5400: mov      sl, r0
004b5404: bl       #0x313a90
004b5408: ldr      r6, [pc, #0x124]
004b540c: mov      r3, #1
004b5410: cmp      r3, #0
004b5414: str      r0, [sp, #4]
004b5418: str      r3, [sp]
004b541c: add      r6, pc, r6
004b5420: bne      #0x4b5468
004b5424: add      r3, sp, #4
004b5428: add      r2, r3, #2
004b542c: add      r3, r3, #1
004b5430: ldrb     r0, [r2, #1]
004b5434: ldrb     r1, [r3, #-1]
004b5438: cmp      r2, r3
004b543c: eor      r1, r0, r1
004b5440: strb     r1, [r3, #-1]
004b5444: ldrb     r0, [r2, #1]
004b5448: eor      r1, r1, r0
004b544c: strb     r1, [r2, #1]
004b5450: ldrb     r0, [r3, #-1]
004b5454: sub      r2, r2, #1
004b5458: eor      r1, r1, r0
004b545c: strb     r1, [r3, #-1]
004b5460: add      r3, r3, #1
004b5464: bhi      #0x4b5430
004b5468: bl       #0x4a3a24
004b546c: ldr      r7, [pc, #0xc4]
004b5470: ldr      r4, [sp, #4]
004b5474: mov      r5, #0xc
004b5478: ldr      r3, [r6, r7]
004b547c: mul      r0, r5, r4
004b5480: str      r4, [r3]
004b5484: add      r0, r0, #8
004b5488: mov      r1, #1
004b548c: bl       #0x31056c
004b5490: cmp      r4, #0
004b5494: str      r5, [r0]
004b5498: str      r4, [r0, #4]
004b549c: add      r3, r0, #8
004b54a0: beq      #0x4b54d0
004b54a4: ldr      r1, [pc, #0x90]
004b54a8: mov      r2, #0
004b54ac: mov      ip, r2
004b54b0: ldr      r1, [r6, r1]
004b54b4: add      r1, r1, #8
004b54b8: add      r2, r2, #1
004b54bc: cmp      r2, r4
004b54c0: str      r1, [r0, #8]
004b54c4: str      ip, [r0, #0x10]
004b54c8: add      r0, r0, #0xc
004b54cc: bne      #0x4b54b8
004b54d0: ldr      r2, [r6, r7]
004b54d4: ldr      r8, [pc, #0x64]
004b54d8: ldr      r1, [r2]
004b54dc: ldr      r2, [r6, r8]
004b54e0: cmp      r1, #0
004b54e4: str      r3, [r2]
004b54e8: beq      #0x4b552c
004b54ec: mov      r4, #0
004b54f0: mov      r5, r4
004b54f4: b        #0x4b5500
004b54f8: ldr      r3, [r6, r8]
004b54fc: ldr      r3, [r3]
004b5500: add      r0, r3, r4
004b5504: mov      r1, sl
004b5508: ldr      r3, [r3, r4]
004b550c: mov      lr, pc
004b5510: ldr      pc, [r3, #0xc]
004b5514: ldr      r3, [r6, r7]
004b5518: add      r5, r5, #1
004b551c: add      r4, r4, #0xc
004b5520: ldr      r3, [r3]
004b5524: cmp      r3, r5
004b5528: bhi      #0x4b54f8
004b552c: add      sp, sp, #0xc
004b5530: pop      {r4, r5, r6, r7, r8, sl, pc}
004b5534: subeq    pc, sp, r4, ror r6
004b5538: andeq    r4, r0, r0, ror #6
004b553c: andeq    r1, r0, r8, lsr #4
004b5540: muleq    r0, r0, pc

# _ZNK13StringManager12getSheetNameEj
00507568: ldr      r3, [pc, #0x38]
0050756c: ldr      r2, [r0, #4]
00507570: ldr      r0, [pc, #0x34]
00507574: add      r3, pc, r3
00507578: cmn      r2, #1
0050757c: ldr      r0, [r3, r0]
00507580: moveq    r2, #0
00507584: ldr      r3, [r0]
00507588: movne    r0, #0xc
0050758c: mulne    r2, r0, r2
00507590: add      r2, r3, r2
00507594: ldr      r3, [r2, #8]
00507598: mov      r2, #0x14
0050759c: mla      r1, r2, r1, r3
005075a0: ldr      r0, [r1, #0x10]
005075a4: bx       lr
005075a8: subeq    sp, r8, ip, lsl r5
005075ac: muleq    r0, r0, pc

# _ZNK13StringManager18getNumberOfStringsEii
00507550: mov      r3, #0x25
00507554: mla      r3, r3, r2, r1
00507558: add      r3, r3, #0x29c
0050755c: add      r3, r0, r3, lsl #1
00507560: ldrsh    r0, [r3, #4]
00507564: bx       lr

# _ZN13StringManager10switchPackEjb
00507a94: cmp      r2, #0
00507a98: push     {r4, r5, r6, lr}
00507a9c: mov      r4, r1
00507aa0: mov      r5, r0
00507aa4: beq      #0x507ac0
00507aa8: ldr      r1, [r0, #4]
00507aac: cmp      r1, r4
00507ab0: beq      #0x507ad8
00507ab4: cmn      r1, #1
00507ab8: beq      #0x507ac4
00507abc: bl       #0x5079c0
00507ac0: ldr      r1, [r5, #4]
00507ac4: cmp      r4, r1
00507ac8: beq      #0x507ad8
00507acc: str      r4, [r5, #4]
00507ad0: mov      r0, #1
00507ad4: pop      {r4, r5, r6, pc}
00507ad8: mov      r0, #0
00507adc: pop      {r4, r5, r6, pc}

# _ZN13PlayerManager14GetLocalPlayerEib
0036e478: push     {r4, lr}
0036e47c: mov      r4, r0
0036e480: bl       #0x36e2cc
0036e484: mov      r2, #0
0036e488: mov      r1, r0
0036e48c: mov      r0, r4
0036e490: pop      {r4, lr}
0036e494: b        #0x36dfb0
