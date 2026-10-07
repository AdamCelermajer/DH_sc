
# _ZN14CharProperties11BuffExpiredEPN10CharTimers5TimerE
003e123c: push     {r4, r5, r6, r7, lr}
003e1240: ldr      r4, [pc, #0x108]
003e1244: subs     r5, r1, #0
003e1248: sub      sp, sp, #0xc
003e124c: mov      r7, r0
003e1250: add      r4, pc, r4
003e1254: beq      #0x3e12c8
003e1258: ldr      r3, [r5]
003e125c: mov      r0, r5
003e1260: mov      lr, pc
003e1264: ldr      pc, [r3, #4]
003e1268: ldr      r3, [r5]
003e126c: mov      r6, r0
003e1270: mov      r0, r5
003e1274: ldr      r5, [r6, #0x388]
003e1278: mov      lr, pc
003e127c: ldr      pc, [r3]
003e1280: cmp      r5, r0
003e1284: beq      #0x3e12ac
003e1288: ldr      r3, [pc, #0xc4]
003e128c: ldr      r3, [r4, r3]
003e1290: ldr      r3, [r3]
003e1294: cmp      r3, #2
003e1298: moveq    r3, #0
003e129c: streq    r3, [r3]
003e12a0: beq      #0x3e12ac
003e12a4: cmp      r3, #1
003e12a8: beq      #0x3e131c
003e12ac: ldr      r3, [r6, #0x390]
003e12b0: mov      r0, r7
003e12b4: mov      r2, r6
003e12b8: ldr      r1, [r3]
003e12bc: add      sp, sp, #0xc
003e12c0: pop      {r4, r5, r6, r7, lr}
003e12c4: b        #0x3e101c
003e12c8: ldr      r3, [pc, #0x84]
003e12cc: ldr      r3, [r4, r3]
003e12d0: ldr      r3, [r3]
003e12d4: cmp      r3, #2
003e12d8: streq    r5, [r5]
003e12dc: beq      #0x3e1258
003e12e0: cmp      r3, #1
003e12e4: bne      #0x3e1258
003e12e8: ldr      r0, [pc, #0x68]
003e12ec: ldr      r1, [pc, #0x68]
003e12f0: ldr      r2, [pc, #0x68]
003e12f4: ldr      r0, [r4, r0]
003e12f8: ldr      r3, [pc, #0x64]
003e12fc: mov      ip, #0x340
003e1300: add      r1, pc, r1
003e1304: add      r2, pc, r2
003e1308: add      r3, pc, r3
003e130c: add      r0, r0, #0xa8
003e1310: str      ip, [sp]
003e1314: bl       #0x30e004
003e1318: b        #0x3e1258
003e131c: ldr      r0, [pc, #0x34]
003e1320: ldr      r1, [pc, #0x40]
003e1324: ldr      r2, [pc, #0x40]
003e1328: ldr      r0, [r4, r0]
003e132c: ldr      r3, [pc, #0x3c]
003e1330: mov      ip, #0x344
003e1334: add      r1, pc, r1
003e1338: add      r2, pc, r2
003e133c: add      r3, pc, r3
003e1340: add      r0, r0, #0xa8
003e1344: str      ip, [sp]
003e1348: bl       #0x30e004
003e134c: b        #0x3e12ac
003e1350: subseq   r3, fp, r0, asr #16
003e1354: andeq    r3, r0, r0, asr #19
003e1358: andeq    r1, r0, r0, asr #19
003e135c: ldrdeq   sp, lr, [sp], #-8
003e1360: subeq    r4, lr, ip, lsr #20
003e1364: subeq    r4, lr, r8, lsr #19
003e1368: subeq    sp, sp, r4, lsr #1
003e136c: subeq    r4, lr, r0, lsl #20
003e1370: subeq    r4, lr, r4, ror sb

# _ZN14CharProperties13PROPS_DelBuffEiPN7Structs19CharacterPropertiesE
003e101c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e1020: ldr      r4, [r0, #0xe1c]
003e1024: ldr      r7, [pc, #0x208]
003e1028: add      r5, r0, #0xe10
003e102c: cmp      r4, #0
003e1030: add      r7, pc, r7
003e1034: sub      sp, sp, #0x54
003e1038: mov      sb, r0
003e103c: mov      r6, r2
003e1040: add      r5, r5, #8
003e1044: beq      #0x3e1128
003e1048: mov      r2, r5
003e104c: b        #0x3e1054
003e1050: mov      r4, r3
003e1054: ldr      r3, [r4, #0x10]
003e1058: cmp      r3, r1
003e105c: ldrlt    r3, [r4, #0xc]
003e1060: ldrge    r3, [r4, #8]
003e1064: movlt    r4, r2
003e1068: mov      r2, r4
003e106c: cmp      r3, #0
003e1070: bne      #0x3e1050
003e1074: cmp      r5, r4
003e1078: beq      #0x3e1134
003e107c: ldr      r3, [r4, #0x10]
003e1080: cmp      r3, r1
003e1084: bgt      #0x3e1128
003e1088: cmp      r5, r4
003e108c: beq      #0x3e1134
003e1090: add      r3, r4, #0x34
003e1094: add      ip, sp, #0x18
003e1098: str      r3, [sp, #4]
003e109c: ldm      r3, {r0, r1, r2, r3}
003e10a0: stm      ip, {r0, r1, r2, r3}
003e10a4: mov      r1, ip
003e10a8: add      r0, r4, #0x44
003e10ac: bl       #0x3de870
003e10b0: cmp      r0, #1
003e10b4: beq      #0x3e1194
003e10b8: cmp      r6, #0
003e10bc: beq      #0x3e1134
003e10c0: ldr      sl, [r4, #0x40]
003e10c4: ldr      r8, [r4, #0x38]
003e10c8: ldr      r5, [r4, #0x3c]
003e10cc: ldr      r7, [r4, #0x34]
003e10d0: ldr      r3, [r4, #0x44]
003e10d4: cmp      r3, r7
003e10d8: beq      #0x3e1134
003e10dc: ldr      r4, [r7]
003e10e0: cmp      r4, r6
003e10e4: mov      fp, r4
003e10e8: beq      #0x3e1140
003e10ec: add      r7, r7, #4
003e10f0: cmp      r7, r5
003e10f4: beq      #0x3e1118
003e10f8: cmp      r3, r7
003e10fc: beq      #0x3e1134
003e1100: ldr      r4, [r7]
003e1104: cmp      r4, r6
003e1108: beq      #0x3e113c
003e110c: add      r7, r7, #4
003e1110: cmp      r5, r7
003e1114: bne      #0x3e10f8
003e1118: ldr      r8, [sl, #4]!
003e111c: add      r5, r8, #0x80
003e1120: mov      r7, r8
003e1124: b        #0x3e10d4
003e1128: mov      r4, r5
003e112c: cmp      r5, r4
003e1130: bne      #0x3e1090
003e1134: add      sp, sp, #0x54
003e1138: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e113c: mov      fp, r6
003e1140: ldr      r0, [sb, #4]
003e1144: ldr      r1, [r4, #0x388]
003e1148: add      r0, r0, #0x3b4
003e114c: bl       #0x3db2d8
003e1150: mov      r0, fp
003e1154: bl       #0x4c5740
003e1158: mov      r0, r4
003e115c: bl       #0x310440
003e1160: ldr      r1, [sp, #4]
003e1164: add      r0, sp, #0x38
003e1168: add      r2, sp, #0x28
003e116c: add      r3, sp, #0x4c
003e1170: str      sl, [sp, #0x34]
003e1174: str      r5, [sp, #0x30]
003e1178: str      r8, [sp, #0x2c]
003e117c: str      r7, [sp, #0x28]
003e1180: bl       #0x3dfa9c
003e1184: mov      r0, sb
003e1188: mov      r1, #1
003e118c: bl       #0x3e0810
003e1190: b        #0x3e1134
003e1194: ldr      lr, [sp, #4]
003e1198: ldr      ip, [sb, #4]
003e119c: add      r6, sp, #8
003e11a0: ldm      lr, {r0, r1, r2, r3}
003e11a4: stm      r6, {r0, r1, r2, r3}
003e11a8: mov      r0, r6
003e11ac: mov      r1, #0
003e11b0: add      r8, ip, #0x3b4
003e11b4: bl       #0x3de8b4
003e11b8: ldr      r3, [sp, #8]
003e11bc: mov      r0, r8
003e11c0: ldr      r3, [r3]
003e11c4: ldr      r1, [r3, #0x388]
003e11c8: bl       #0x3db2d8
003e11cc: ldr      ip, [sp, #4]
003e11d0: ldm      ip, {r0, r1, r2, r3}
003e11d4: stm      r6, {r0, r1, r2, r3}
003e11d8: mov      r0, r6
003e11dc: mov      r1, #0
003e11e0: bl       #0x3de8b4
003e11e4: ldr      r3, [sp, #8]
003e11e8: ldr      r6, [r3]
003e11ec: cmp      r6, #0
003e11f0: beq      #0x3e1204
003e11f4: mov      r0, r6
003e11f8: bl       #0x4c5740
003e11fc: mov      r0, r6
003e1200: bl       #0x310440
003e1204: ldr      r3, [pc, #0x2c]
003e1208: add      r1, r4, #0x18
003e120c: ldr      r0, [r7, r3]
003e1210: bl       #0x494978
003e1214: add      r1, sp, #0x50
003e1218: str      r4, [r1, #-8]!
003e121c: mov      r0, r5
003e1220: bl       #0x3e0fd0
003e1224: mov      r0, sb
003e1228: mov      r1, #1
003e122c: bl       #0x3e0810
003e1230: b        #0x3e1134
003e1234: subseq   r3, fp, r0, ror #20
003e1238: andeq    r1, r0, r8, lsl #22

# _ZN14CharProperties13PROPS_AddBuffEijijiPKc
003e232c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2330: mov      r8, r0
003e2334: ldr      ip, [r0, #0xe1c]
003e2338: ldr      r0, [pc, #0x3d4]
003e233c: sub      sp, sp, #0x94
003e2340: add      fp, r8, #0xe10
003e2344: add      r0, pc, r0
003e2348: cmp      ip, #0
003e234c: str      r0, [sp, #0xc]
003e2350: str      r1, [sp, #0x24]
003e2354: str      r2, [sp, #0x10]
003e2358: mov      r7, r3
003e235c: add      fp, fp, #8
003e2360: ldr      sl, [sp, #0xb8]
003e2364: beq      #0x3e24d0
003e2368: mov      r2, fp
003e236c: b        #0x3e2374
003e2370: mov      ip, r3
003e2374: ldr      r3, [ip, #0x10]
003e2378: cmp      r1, r3
003e237c: ldrgt    r3, [ip, #0xc]
003e2380: ldrle    r3, [ip, #8]
003e2384: movgt    ip, r2
003e2388: mov      r2, ip
003e238c: cmp      r3, #0
003e2390: bne      #0x3e2370
003e2394: cmp      fp, ip
003e2398: beq      #0x3e23a8
003e239c: ldr      r3, [ip, #0x10]
003e23a0: cmp      r1, r3
003e23a4: blt      #0x3e24d0
003e23a8: cmp      r7, #0
003e23ac: movle    r7, #0x80
003e23b0: mov      r5, #0
003e23b4: cmp      fp, ip
003e23b8: str      r5, [sp, #0x8c]
003e23bc: beq      #0x3e24ec
003e23c0: add      lr, sp, #0x6c
003e23c4: add      sb, ip, #0x34
003e23c8: ldm      sb, {r0, r1, r2, r3}
003e23cc: stm      lr, {r0, r1, r2, r3}
003e23d0: add      r0, ip, #0x44
003e23d4: mov      r1, lr
003e23d8: bl       #0x3de870
003e23dc: cmp      r7, r0
003e23e0: beq      #0x3e261c
003e23e4: ldr      r3, [sp, #0x8c]
003e23e8: cmp      r3, #0
003e23ec: beq      #0x3e24ec
003e23f0: ldr      r0, [r8, #4]
003e23f4: ldr      r1, [r3, #0x388]
003e23f8: add      r0, r0, #0x3b4
003e23fc: bl       #0x3db2d8
003e2400: ldr      r3, [sp, #0x8c]
003e2404: mvn      r2, #0
003e2408: str      r2, [r3, #0x388]
003e240c: ldr      r2, [sp, #0x10]
003e2410: cmp      r2, #0
003e2414: ldreq    r1, [sp, #0x8c]
003e2418: bne      #0x3e25d0
003e241c: ldr      lr, [r1, #0x390]
003e2420: ldr      r3, [lr, #4]
003e2424: cmp      r3, #0
003e2428: beq      #0x3e24ac
003e242c: add      ip, sp, #0x3c
003e2430: add      r3, lr, #0x20
003e2434: ldm      r3, {r0, r1, r2, r3}
003e2438: stm      ip, {r0, r1, r2, r3}
003e243c: add      r0, lr, #0x30
003e2440: mov      r1, ip
003e2444: bl       #0x3de870
003e2448: cmp      r0, #1
003e244c: bls      #0x3e24a8
003e2450: ldr      r3, [sp, #0x8c]
003e2454: ldr      r3, [r3, #0x390]
003e2458: ldr      r0, [r3, #4]
003e245c: bl       #0x492550
003e2460: ldr      r2, [sp, #0x8c]
003e2464: ldr      r3, [r0]
003e2468: add      ip, sp, #0x2c
003e246c: ldr      lr, [r2, #0x390]
003e2470: ldr      r5, [r3, #0x1c]
003e2474: mov      r4, r0
003e2478: add      r3, lr, #0x20
003e247c: ldm      r3, {r0, r1, r2, r3}
003e2480: stm      ip, {r0, r1, r2, r3}
003e2484: mov      r1, ip
003e2488: add      r0, lr, #0x30
003e248c: bl       #0x3de870
003e2490: mov      r3, #0
003e2494: sub      r1, r0, #1
003e2498: str      r3, [sp]
003e249c: mov      r0, r4
003e24a0: mov      r2, #1
003e24a4: blx      r5
003e24a8: ldr      r1, [sp, #0x8c]
003e24ac: mov      r0, r8
003e24b0: bl       #0x3def84
003e24b4: ldr      r3, [sp, #0x8c]
003e24b8: lsl      sl, sl, #8
003e24bc: str      sl, [r3, #0x2b4]
003e24c0: ldr      r0, [sp, #0x8c]
003e24c4: str      sl, [r8, #0xd48]
003e24c8: add      sp, sp, #0x94
003e24cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e24d0: cmp      r7, #0
003e24d4: mov      ip, fp
003e24d8: movle    r7, #0x80
003e24dc: mov      r5, #0
003e24e0: cmp      fp, ip
003e24e4: str      r5, [sp, #0x8c]
003e24e8: bne      #0x3e23c0
003e24ec: add      r1, sp, #0x24
003e24f0: mov      r0, fp
003e24f4: bl       #0x3e209c
003e24f8: ldr      r3, [sp, #0x24]
003e24fc: mov      r5, r0
003e2500: mov      r4, r0
003e2504: ldr      r0, [sp, #0xc0]
003e2508: str      r3, [r5], #8
003e250c: bl       #0x30de54
003e2510: ldr      r1, [sp, #0xc0]
003e2514: add      r2, r1, r0
003e2518: mov      r0, r5
003e251c: bl       #0x3109e0
003e2520: ldr      r2, [r4, #4]
003e2524: ldr      ip, [sp, #0xbc]
003e2528: rsbs     r3, r2, #1
003e252c: movlo    r3, #0
003e2530: cmn      ip, #1
003e2534: moveq    r3, #0
003e2538: cmp      r3, #0
003e253c: bne      #0x3e26e4
003e2540: add      ip, sp, #0x4c
003e2544: add      r5, r4, #0x20
003e2548: ldm      r5, {r0, r1, r2, r3}
003e254c: stm      ip, {r0, r1, r2, r3}
003e2550: mov      r1, ip
003e2554: add      r0, r4, #0x30
003e2558: bl       #0x3de870
003e255c: cmp      r7, r0
003e2560: bls      #0x3e2614
003e2564: mov      r1, #0
003e2568: mov      r0, #0x394
003e256c: bl       #0x310570
003e2570: str      r4, [r0, #0x390]
003e2574: ldr      r1, [sp, #0xc]
003e2578: ldr      r3, [pc, #0x198]
003e257c: mvn      r2, #0
003e2580: ldr      r3, [r1, r3]
003e2584: add      r3, r3, #8
003e2588: str      r3, [r0]
003e258c: str      r0, [sp, #0x8c]
003e2590: str      r8, [r0, #0x38c]
003e2594: ldr      r3, [sp, #0x8c]
003e2598: str      sl, [r3, #0x384]
003e259c: ldr      r3, [sp, #0x8c]
003e25a0: str      r2, [r3, #0x388]
003e25a4: ldr      r2, [r4, #0x38]
003e25a8: ldr      r3, [r4, #0x30]
003e25ac: sub      r2, r2, #4
003e25b0: cmp      r3, r2
003e25b4: beq      #0x3e2704
003e25b8: ldr      r2, [sp, #0x8c]
003e25bc: str      r2, [r3]
003e25c0: ldr      r3, [r4, #0x30]
003e25c4: add      r3, r3, #4
003e25c8: str      r3, [r4, #0x30]
003e25cc: b        #0x3e240c
003e25d0: ldr      r0, [r8, #4]
003e25d4: ldr      r4, [sp, #0x8c]
003e25d8: mov      r1, r2
003e25dc: mov      r3, #0x36
003e25e0: add      r0, r0, #0x3b4
003e25e4: mov      r2, #0
003e25e8: str      r4, [sp]
003e25ec: bl       #0x3dbe24
003e25f0: str      r0, [r4, #0x388]
003e25f4: ldr      r1, [sp, #0x8c]
003e25f8: ldr      r3, [r1, #0x388]
003e25fc: cmn      r3, #1
003e2600: bne      #0x3e241c
003e2604: mov      r2, r1
003e2608: mov      r0, r8
003e260c: ldr      r1, [sp, #0x24]
003e2610: bl       #0x3e101c
003e2614: mov      r0, #0
003e2618: b        #0x3e24c8
003e261c: add      r1, sp, #0x88
003e2620: add      r2, sp, #0x84
003e2624: add      r3, sp, #0x80
003e2628: add      ip, sp, #0x7c
003e262c: add      r4, sp, #0x5c
003e2630: str      r1, [sp, #0x14]
003e2634: str      r2, [sp, #0x18]
003e2638: str      r3, [sp, #0x1c]
003e263c: str      ip, [sp, #0x20]
003e2640: b        #0x3e2650
003e2644: add      r5, r5, #1
003e2648: cmp      r7, r5
003e264c: ble      #0x3e23e4
003e2650: ldm      sb, {r0, r1, r2, r3}
003e2654: stm      r4, {r0, r1, r2, r3}
003e2658: mov      r1, r5
003e265c: mov      r0, r4
003e2660: bl       #0x3de8b4
003e2664: ldr      r3, [sp, #0x5c]
003e2668: ldr      r6, [r3]
003e266c: ldr      r3, [r6, #0x384]
003e2670: cmp      r3, sl
003e2674: strlo    r6, [sp, #0x8c]
003e2678: strlo    sl, [r6, #0x384]
003e267c: blo      #0x3e2644
003e2680: bne      #0x3e2644
003e2684: ldr      r1, [r6, #0x388]
003e2688: ldr      r2, [sp, #0x14]
003e268c: ldr      r3, [sp, #0x18]
003e2690: cmn      r1, #1
003e2694: beq      #0x3e26dc
003e2698: ldr      r1, [sp, #0x8c]
003e269c: cmp      r1, #0
003e26a0: beq      #0x3e26dc
003e26a4: ldr      r0, [r8, #4]
003e26a8: ldr      r1, [r1, #0x388]
003e26ac: add      r0, r0, #0x3b4
003e26b0: bl       #0x3db344
003e26b4: ldr      r0, [r8, #4]
003e26b8: ldr      r2, [sp, #0x1c]
003e26bc: ldr      r3, [sp, #0x20]
003e26c0: ldr      r1, [r6, #0x388]
003e26c4: add      r0, r0, #0x3b4
003e26c8: bl       #0x3db344
003e26cc: ldr      r3, [sp, #0x80]
003e26d0: ldr      r2, [sp, #0x88]
003e26d4: cmp      r2, r3
003e26d8: bhs      #0x3e2644
003e26dc: str      r6, [sp, #0x8c]
003e26e0: b        #0x3e2644
003e26e4: mov      r1, ip
003e26e8: ldr      r3, [pc, #0x2c]
003e26ec: ldr      ip, [sp, #0xc]
003e26f0: ldr      r2, [r8, #4]
003e26f4: ldr      r0, [ip, r3]
003e26f8: bl       #0x495430
003e26fc: str      r0, [r4, #4]
003e2700: b        #0x3e2540
003e2704: mov      r0, r5
003e2708: add      r1, sp, #0x8c
003e270c: bl       #0x3e21a8
003e2710: b        #0x3e240c
003e2714: subseq   r2, fp, ip, asr #14
003e2718: andeq    r2, r0, ip, lsl #19
003e271c: andeq    r1, r0, r8, lsl #22

# _ZN14CharProperties20PROPS_RemoveAllBuffsEv
003e0af8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e0afc: ldr      r2, [pc, #0x160]
003e0b00: sub      sp, sp, #0x34
003e0b04: add      r3, r0, #0xe10
003e0b08: add      r2, pc, r2
003e0b0c: str      r2, [sp, #8]
003e0b10: ldr      r2, [pc, #0x150]
003e0b14: add      r3, r3, #8
003e0b18: str      r3, [sp, #4]
003e0b1c: ldr      sl, [r0, #0xe20]
003e0b20: mov      r7, r0
003e0b24: add      sb, sp, #0x20
003e0b28: add      r4, sp, #0x10
003e0b2c: str      r2, [sp, #0xc]
003e0b30: ldr      r3, [sp, #4]
003e0b34: cmp      r3, sl
003e0b38: beq      #0x3e0bec
003e0b3c: add      r5, sl, #0x34
003e0b40: ldm      r5, {r0, r1, r2, r3}
003e0b44: stm      sb, {r0, r1, r2, r3}
003e0b48: add      r0, sl, #0x44
003e0b4c: mov      r1, sb
003e0b50: bl       #0x3de870
003e0b54: subs     r8, r0, #0
003e0b58: beq      #0x3e0ba8
003e0b5c: mov      r6, #0
003e0b60: ldm      r5, {r0, r1, r2, r3}
003e0b64: stm      r4, {r0, r1, r2, r3}
003e0b68: mov      r1, r6
003e0b6c: mov      r0, r4
003e0b70: bl       #0x3de8b4
003e0b74: ldr      r3, [sp, #0x10]
003e0b78: ldr      r0, [r7, #4]
003e0b7c: add      r6, r6, #1
003e0b80: ldr      fp, [r3]
003e0b84: add      r0, r0, #0x3b4
003e0b88: ldr      r1, [fp, #0x388]
003e0b8c: bl       #0x3db2d8
003e0b90: mov      r0, fp
003e0b94: bl       #0x4c5740
003e0b98: mov      r0, fp
003e0b9c: bl       #0x310440
003e0ba0: cmp      r6, r8
003e0ba4: bne      #0x3e0b60
003e0ba8: ldr      r2, [sp, #8]
003e0bac: ldr      r3, [sp, #0xc]
003e0bb0: add      r1, sl, #0x18
003e0bb4: ldr      r0, [r2, r3]
003e0bb8: bl       #0x494978
003e0bbc: ldr      r2, [sl, #0xc]
003e0bc0: cmp      r2, #0
003e0bc4: bne      #0x3e0bd0
003e0bc8: b        #0x3e0c30
003e0bcc: mov      r2, r3
003e0bd0: ldr      r3, [r2, #8]
003e0bd4: cmp      r3, #0
003e0bd8: bne      #0x3e0bcc
003e0bdc: ldr      r3, [sp, #4]
003e0be0: mov      sl, r2
003e0be4: cmp      r3, sl
003e0be8: bne      #0x3e0b3c
003e0bec: ldr      r3, [r7, #0xe28]
003e0bf0: cmp      r3, #0
003e0bf4: beq      #0x3e0c1c
003e0bf8: ldr      r0, [sp, #4]
003e0bfc: ldr      r1, [r7, #0xe1c]
003e0c00: bl       #0x3e0ab8
003e0c04: ldr      r2, [sp, #4]
003e0c08: mov      r3, #0
003e0c0c: str      r3, [r7, #0xe28]
003e0c10: str      r2, [r7, #0xe24]
003e0c14: str      r2, [r7, #0xe20]
003e0c18: str      r3, [r7, #0xe1c]
003e0c1c: mov      r0, r7
003e0c20: mov      r1, #1
003e0c24: bl       #0x3e0810
003e0c28: add      sp, sp, #0x34
003e0c2c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e0c30: ldr      r3, [sl, #4]
003e0c34: ldr      r1, [r3, #0xc]
003e0c38: cmp      r1, sl
003e0c3c: bne      #0x3e0c58
003e0c40: mov      sl, r3
003e0c44: ldr      r3, [r3, #4]
003e0c48: ldr      r2, [r3, #0xc]
003e0c4c: cmp      r2, sl
003e0c50: beq      #0x3e0c40
003e0c54: ldr      r2, [sl, #0xc]
003e0c58: cmp      r3, r2
003e0c5c: movne    sl, r3
003e0c60: b        #0x3e0b30
003e0c64: subseq   r3, fp, r8, lsl #31
003e0c68: andeq    r1, r0, r8, lsl #22
