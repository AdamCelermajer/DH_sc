
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

# _ZN12ItemInstance11_UpdateNameEv
003fb754: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003fb758: ldr      r4, [pc, #0x4bc]
003fb75c: ldr      r3, [pc, #0x4bc]
003fb760: ldr      r6, [pc, #0x4bc]
003fb764: add      r4, pc, r4
003fb768: ldr      r3, [r4, r3]
003fb76c: ldr      r2, [r4, r6]
003fb770: mov      r7, r0
003fb774: ldr      r3, [r3]
003fb778: ldr      r0, [r0, #4]
003fb77c: ldr      r1, [pc, #0x4a4]
003fb780: mov      r5, #0xa4
003fb784: mla      r5, r5, r0, r3
003fb788: ldr      r3, [r2]
003fb78c: add      r1, pc, r1
003fb790: add      sl, r7, #8
003fb794: sub      sp, sp, #0xf4
003fb798: mov      r2, r1
003fb79c: mov      r0, sl
003fb7a0: str      r3, [sp, #0xec]
003fb7a4: bl       #0x3109e0
003fb7a8: ldr      r1, [r5, #0x44]
003fb7ac: cmn      r1, #1
003fb7b0: beq      #0x3fbacc
003fb7b4: ldr      r2, [pc, #0x470]
003fb7b8: ldr      r3, [r4, r2]
003fb7bc: str      r2, [sp, #4]
003fb7c0: ldr      r0, [r3, #0x34]
003fb7c4: bl       #0x508edc
003fb7c8: ldr      r3, [pc, #0x460]
003fb7cc: str      r0, [sp, #0x14]
003fb7d0: ldr      r3, [r4, r3]
003fb7d4: ldr      r1, [r3]
003fb7d8: bl       #0x30ebd4
003fb7dc: cmp      r0, #0
003fb7e0: str      r0, [sp, #8]
003fb7e4: movne    sb, #0
003fb7e8: movne    fp, #1
003fb7ec: beq      #0x3fbbc0
003fb7f0: ldr      r1, [r5, #0x48]
003fb7f4: cmn      r1, #1
003fb7f8: moveq    r5, #0
003fb7fc: beq      #0x3fb814
003fb800: ldr      r2, [sp, #4]
003fb804: ldr      r3, [r4, r2]
003fb808: ldr      r0, [r3, #0x34]
003fb80c: bl       #0x508edc
003fb810: mov      r5, r0
003fb814: ldr      r1, [pc, #0x418]
003fb818: add      r3, sp, #0xd4
003fb81c: mov      r0, r3
003fb820: add      r1, pc, r1
003fb824: add      r2, sp, #0x40
003fb828: str      r3, [sp]
003fb82c: bl       #0x3140ec
003fb830: cmp      r5, #0
003fb834: streq    r5, [sp, #0xc]
003fb838: beq      #0x3fb988
003fb83c: ldr      r3, [pc, #0x3f4]
003fb840: mov      r0, r5
003fb844: ldr      r3, [r4, r3]
003fb848: ldr      r1, [r3]
003fb84c: bl       #0x30ebd4
003fb850: cmp      r0, #0
003fb854: streq    r0, [sp, #0xc]
003fb858: moveq    r8, r0
003fb85c: beq      #0x3fb870
003fb860: subs     r1, r5, r0
003fb864: movne    r1, #1
003fb868: str      r1, [sp, #0xc]
003fb86c: rsb      r8, r5, r0
003fb870: cmp      sb, #0
003fb874: movne    sb, #2
003fb878: ldr      r3, [pc, #0x3bc]
003fb87c: cmp      fp, #0
003fb880: movne    r2, sb
003fb884: addne    r2, r2, #1
003fb888: mov      r0, r5
003fb88c: str      sb, [sp, #0x10]
003fb890: str      r3, [sp, #0x18]
003fb894: strne    r2, [sp, #0x10]
003fb898: bl       #0x30de54
003fb89c: mvn      r2, #0
003fb8a0: add      sb, sp, #0xf0
003fb8a4: mov      fp, r0
003fb8a8: add      r0, r2, #1
003fb8ac: str      r2, [sb, #-0xd0]!
003fb8b0: cmp      r2, #0
003fb8b4: cmpne    r0, fp
003fb8b8: add      r3, sb, #8
003fb8bc: movge    r1, #0
003fb8c0: str      r8, [sp, #0x1c]
003fb8c4: strge    r1, [sb, #4]
003fb8c8: mov      r8, r3
003fb8cc: bge      #0x3fb8f0
003fb8d0: ldr      r2, [sp, #0x18]
003fb8d4: add      r0, r5, r0
003fb8d8: ldr      r3, [r4, r2]
003fb8dc: ldr      r1, [r3]
003fb8e0: bl       #0x30ebd4
003fb8e4: cmp      r0, #0
003fb8e8: rsbne    r0, r5, r0
003fb8ec: str      r0, [sb, #4]
003fb8f0: cmp      sb, r8
003fb8f4: beq      #0x3fb91c
003fb8f8: ldr      r2, [sb, #4]!
003fb8fc: add      r0, r2, #1
003fb900: cmp      r2, #0
003fb904: cmpne    r0, fp
003fb908: movge    r1, #0
003fb90c: strge    r1, [sb, #4]
003fb910: blt      #0x3fb8d0
003fb914: cmp      sb, r8
003fb918: bne      #0x3fb8f8
003fb91c: ldr      r3, [sp, #0xc]
003fb920: ldr      r8, [sp, #0x1c]
003fb924: ldr      r2, [sp, #0x10]
003fb928: cmp      r3, #0
003fb92c: moveq    r8, fp
003fb930: add      r1, sp, #0xf0
003fb934: str      r8, [sp, #0x30]
003fb938: add      r3, r1, r2, lsl #2
003fb93c: ldr      fp, [r3, #-0xd0]
003fb940: cmp      fp, #0
003fb944: beq      #0x3fb954
003fb948: ldr      sb, [r3, #-0xcc]
003fb94c: cmp      sb, #0
003fb950: bne      #0x3fbb88
003fb954: add      sb, sp, #0xbc
003fb958: mov      r1, r5
003fb95c: add      r2, sp, #0x3c
003fb960: mov      r0, sb
003fb964: bl       #0x3140ec
003fb968: ldr      r2, [sp, #0x20]
003fb96c: mov      r3, r8
003fb970: ldr      r0, [sp]
003fb974: add      r2, r2, #1
003fb978: mov      r1, sb
003fb97c: bl       #0x3fa720
003fb980: mov      r0, sb
003fb984: bl       #0x3139ac
003fb988: add      r8, sp, #0x8c
003fb98c: mov      r0, r8
003fb990: mov      r1, #0x10
003fb994: str      r8, [sp, #0x9c]
003fb998: str      r8, [sp, #0xa0]
003fb99c: bl       #0x31167c
003fb9a0: ldr      r3, [sp, #0x9c]
003fb9a4: ldr      r1, [pc, #0x294]
003fb9a8: add      sb, sp, #0x74
003fb9ac: mov      r2, #0
003fb9b0: strb     r2, [r3]
003fb9b4: add      r1, pc, r1
003fb9b8: add      r2, sp, #0x34
003fb9bc: mov      r0, sb
003fb9c0: bl       #0x3140ec
003fb9c4: ldr      r3, [sp, #4]
003fb9c8: mov      r1, sb
003fb9cc: ldr      r2, [r4, r3]
003fb9d0: ldr      r3, [r7, #0x54]
003fb9d4: ldr      r0, [r2, #0x34]
003fb9d8: ldr      r2, [sp, #0x14]
003fb9dc: bl       #0x508ef4
003fb9e0: ldr      r1, [sp, #8]
003fb9e4: mov      r0, r8
003fb9e8: cmp      r1, #0
003fb9ec: ldreq    r2, [sp, #0x84]
003fb9f0: ldrne    r2, [sp, #8]
003fb9f4: ldrne    r1, [sp, #0x14]
003fb9f8: ldreq    r3, [sp, #0x88]
003fb9fc: rsbne    r3, r1, r2
003fba00: rsbeq    r3, r3, r2
003fba04: mov      r1, sb
003fba08: mov      r2, #0
003fba0c: bl       #0x3fa720
003fba10: ldr      r1, [pc, #0x22c]
003fba14: mov      r0, sl
003fba18: add      r1, pc, r1
003fba1c: mov      r2, r1
003fba20: bl       #0x3109e0
003fba24: ldr      r2, [sp, #0xc]
003fba28: cmp      r2, #0
003fba2c: bne      #0x3fbae8
003fba30: cmp      r5, #0
003fba34: beq      #0x3fbaa4
003fba38: ldr      r3, [sp, #0xe8]
003fba3c: ldr      r1, [sp, #0xe4]
003fba40: add      r5, sp, #0x5c
003fba44: mov      r0, r5
003fba48: rsb      r1, r3, r1
003fba4c: add      r1, r1, #2
003fba50: str      r5, [sp, #0x6c]
003fba54: str      r5, [sp, #0x70]
003fba58: bl       #0x31167c
003fba5c: ldr      r3, [sp, #0x6c]
003fba60: ldr      r1, [sp, #0xc]
003fba64: mov      r0, r5
003fba68: strb     r1, [r3]
003fba6c: ldr      r2, [sp, #0xe4]
003fba70: ldr      r1, [sp, #0xe8]
003fba74: bl       #0x310804
003fba78: ldr      r1, [pc, #0x1c8]
003fba7c: mov      r0, r5
003fba80: add      r1, pc, r1
003fba84: add      r1, r1, #1
003fba88: bl       #0x3fa868
003fba8c: mov      r0, sl
003fba90: ldr      r1, [sp, #0x70]
003fba94: ldr      r2, [sp, #0x6c]
003fba98: bl       #0x310804
003fba9c: mov      r0, r5
003fbaa0: bl       #0x3139ac
003fbaa4: ldr      r1, [sp, #0xa0]
003fbaa8: ldr      r2, [sp, #0x9c]
003fbaac: mov      r0, sl
003fbab0: bl       #0x310804
003fbab4: mov      r0, sb
003fbab8: bl       #0x3139ac
003fbabc: mov      r0, r8
003fbac0: bl       #0x3139ac
003fbac4: ldr      r0, [sp]
003fbac8: bl       #0x3139ac
003fbacc: ldr      r3, [r4, r6]
003fbad0: ldr      r2, [sp, #0xec]
003fbad4: ldr      r3, [r3]
003fbad8: cmp      r2, r3
003fbadc: bne      #0x3fbc18
003fbae0: add      sp, sp, #0xf4
003fbae4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003fbae8: mov      r0, sl
003fbaec: ldr      r1, [sp, #0xa0]
003fbaf0: ldr      r2, [sp, #0x9c]
003fbaf4: bl       #0x310804
003fbaf8: cmp      r5, #0
003fbafc: beq      #0x3fbb6c
003fbb00: ldr      r3, [sp, #0xe8]
003fbb04: ldr      r1, [sp, #0xe4]
003fbb08: add      r5, sp, #0x44
003fbb0c: mov      r0, r5
003fbb10: rsb      r1, r3, r1
003fbb14: add      r1, r1, #2
003fbb18: str      r5, [sp, #0x54]
003fbb1c: str      r5, [sp, #0x58]
003fbb20: bl       #0x31167c
003fbb24: ldr      r1, [pc, #0x120]
003fbb28: ldr      r3, [sp, #0x54]
003fbb2c: mov      r2, #0
003fbb30: add      r1, pc, r1
003fbb34: strb     r2, [r3]
003fbb38: add      r1, r1, #1
003fbb3c: mov      r0, r5
003fbb40: bl       #0x3fa868
003fbb44: ldr      r1, [sp, #0xe8]
003fbb48: ldr      r2, [sp, #0xe4]
003fbb4c: mov      r0, r5
003fbb50: bl       #0x310804
003fbb54: mov      r0, sl
003fbb58: ldr      r1, [sp, #0x58]
003fbb5c: ldr      r2, [sp, #0x54]
003fbb60: bl       #0x310804
003fbb64: mov      r0, r5
003fbb68: bl       #0x3139ac
003fbb6c: mov      r0, sb
003fbb70: bl       #0x3139ac
003fbb74: mov      r0, r8
003fbb78: bl       #0x3139ac
003fbb7c: ldr      r0, [sp]
003fbb80: bl       #0x3139ac
003fbb84: b        #0x3fbacc
003fbb88: add      r8, sp, #0xa4
003fbb8c: mov      r1, r5
003fbb90: add      r2, sp, #0x38
003fbb94: mov      r0, r8
003fbb98: bl       #0x3140ec
003fbb9c: rsb      r3, fp, sb
003fbba0: add      r2, fp, #1
003fbba4: sub      r3, r3, #1
003fbba8: ldr      r0, [sp]
003fbbac: mov      r1, r8
003fbbb0: bl       #0x3fa720
003fbbb4: mov      r0, r8
003fbbb8: bl       #0x3139ac
003fbbbc: b        #0x3fb988
003fbbc0: ldr      r3, [pc, #0x88]
003fbbc4: ldr      r0, [sp, #0x14]
003fbbc8: ldr      r3, [r4, r3]
003fbbcc: ldr      r1, [r3]
003fbbd0: bl       #0x30ebd4
003fbbd4: cmp      r0, #0
003fbbd8: movne    sb, #1
003fbbdc: mov      fp, r0
003fbbe0: str      r0, [sp, #8]
003fbbe4: movne    fp, sb
003fbbe8: bne      #0x3fb7f0
003fbbec: ldr      r3, [pc, #0x60]
003fbbf0: ldr      r0, [sp, #0x14]
003fbbf4: ldr      r3, [r4, r3]
003fbbf8: ldr      r1, [r3]
003fbbfc: bl       #0x30ebd4
003fbc00: cmp      r0, #0
003fbc04: str      r0, [sp, #8]
003fbc08: ldreq    sb, [sp, #8]
003fbc0c: movne    sb, #1
003fbc10: moveq    fp, sb
003fbc14: b        #0x3fb7f0
003fbc18: bl       #0x30e310
003fbc1c: subseq   sb, sb, ip, lsr #6
003fbc20: andeq    r2, r0, ip, ror #16
003fbc24: andeq    r4, r0, ip, lsr #1
003fbc28: subeq    r0, sp, ip, ror r0
003fbc2c: strdeq   r3, r4, [r0], -r4
003fbc30: strdeq   r3, r4, [r0], -r4
003fbc34: subeq    pc, ip, r8, ror #31
003fbc38: andeq    r2, r0, r4, asr #6
003fbc3c: strdeq   r1, r2, [r0], -ip
003fbc40: subeq    pc, ip, r4, asr lr

# _ZN12ItemInstance8SetValueEi
003fbc58: str      r1, [r0, #0x54]
003fbc5c: b        #0x3fb754
