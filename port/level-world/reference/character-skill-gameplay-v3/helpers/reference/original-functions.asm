
# _ZNK3sfc6script3lua5Value7getBoolEv
0031bc80: push     {r4, r5, r6, lr}
0031bc84: ldr      r3, [r0, #4]
0031bc88: mov      r5, r0
0031bc8c: cmp      r3, #0
0031bc90: beq      #0x31bcbc
0031bc94: cmp      r3, #1
0031bc98: beq      #0x31bcc8
0031bc9c: cmp      r3, #3
0031bca0: beq      #0x31bcc8
0031bca4: cmp      r3, #2
0031bca8: beq      #0x31bcec
0031bcac: cmp      r3, #7
0031bcb0: beq      #0x31bcec
0031bcb4: cmp      r3, #4
0031bcb8: beq      #0x31bcfc
0031bcbc: mov      r5, #0
0031bcc0: mov      r0, r5
0031bcc4: pop      {r4, r5, r6, pc}
0031bcc8: ldr      r0, [r5, #8]
0031bccc: mov      r1, #0
0031bcd0: bl       #0x30df8c
0031bcd4: cmp      r0, #0
0031bcd8: mov      r5, #0
0031bcdc: moveq    r5, #1
0031bce0: uxtb     r5, r5
0031bce4: mov      r0, r5
0031bce8: pop      {r4, r5, r6, pc}
0031bcec: ldr      r5, [r5, #0x6c]
0031bcf0: subs     r5, r5, #0
0031bcf4: movne    r5, #1
0031bcf8: b        #0x31bcc0
0031bcfc: bl       #0x84c7e0
0031bd00: ldr      r1, [r5, #0x20]
0031bd04: mov      r4, r0
0031bd08: bl       #0x84c04c
0031bd0c: mov      r0, r4
0031bd10: mvn      r1, #0
0031bd14: bl       #0x84b320
0031bd18: subs     r5, r0, #0
0031bd1c: movne    r5, #1
0031bd20: mov      r0, r4
0031bd24: bl       #0x85797c
0031bd28: b        #0x31bcc0

# _ZNK6CharAI21IsScriptProcessLoadedEv
003cb458: ldr      r0, [r0, #0x28]
003cb45c: cmp      r0, #6
003cb460: movle    r0, #0
003cb464: movgt    r0, #1
003cb468: bx       lr

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

# _ZN9Character7UseManaEi
003bdef4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003bdef8: ldr      r4, [pc, #0x1a4]
003bdefc: ldr      r5, [pc, #0x1a4]
003bdf00: sub      sp, sp, #0x48
003bdf04: add      r4, pc, r4
003bdf08: ldr      r3, [r4, r5]
003bdf0c: mov      r6, r0
003bdf10: mov      r7, r1
003bdf14: ldr      r3, [r3]
003bdf18: str      r3, [sp, #0x44]
003bdf1c: bl       #0x7fd794
003bdf20: ldrb     r3, [r0, #5]
003bdf24: cmp      r3, #0
003bdf28: bne      #0x3bdf74
003bdf2c: cmp      r7, #0
003bdf30: blt      #0x3bdf94
003bdf34: ldr      sb, [pc, #0x170]
003bdf38: ldr      r3, [pc, #0x170]
003bdf3c: add      sb, pc, sb
003bdf40: ldr      r0, [r4, r3]
003bdf44: mov      r1, sb
003bdf48: bl       #0x320e14
003bdf4c: cmp      r0, #0
003bdf50: beq      #0x3bdfec
003bdf54: mov      r0, #1
003bdf58: ldr      r3, [r4, r5]
003bdf5c: ldr      r2, [sp, #0x44]
003bdf60: ldr      r3, [r3]
003bdf64: cmp      r2, r3
003bdf68: bne      #0x3be0a0
003bdf6c: add      sp, sp, #0x48
003bdf70: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003bdf74: ldr      r3, [r6]
003bdf78: mov      r0, r6
003bdf7c: mov      lr, pc
003bdf80: ldr      pc, [r3, #0x54]
003bdf84: cmp      r0, #0
003bdf88: bne      #0x3bdf54
003bdf8c: cmp      r7, #0
003bdf90: bge      #0x3bdf34
003bdf94: ldr      r3, [pc, #0x118]
003bdf98: ldr      r3, [r4, r3]
003bdf9c: ldr      r3, [r3]
003bdfa0: cmp      r3, #2
003bdfa4: moveq    r3, #0
003bdfa8: streq    r3, [r3]
003bdfac: beq      #0x3bdf34
003bdfb0: cmp      r3, #1
003bdfb4: bne      #0x3bdf34
003bdfb8: ldr      r0, [pc, #0xf8]
003bdfbc: ldr      r1, [pc, #0xf8]
003bdfc0: ldr      r2, [pc, #0xf8]
003bdfc4: ldr      r0, [r4, r0]
003bdfc8: ldr      r3, [pc, #0xf4]
003bdfcc: mov      ip, #0x9f
003bdfd0: add      r1, pc, r1
003bdfd4: add      r2, pc, r2
003bdfd8: add      r3, pc, r3
003bdfdc: add      r0, r0, #0xa8
003bdfe0: str      ip, [sp]
003bdfe4: bl       #0x30e004
003bdfe8: b        #0x3bdf34
003bdfec: ldr      r3, [pc, #0xd4]
003bdff0: add      r8, sp, #0x2c
003bdff4: ldr      sl, [r4, r3]
003bdff8: mov      r0, sl
003bdffc: bl       #0x337888
003be000: mov      r1, sb
003be004: add      r2, sp, #0x10
003be008: mov      r0, r8
003be00c: bl       #0x3140ec
003be010: mov      r1, r8
003be014: mov      r0, sl
003be018: bl       #0x337a88
003be01c: mov      sb, r0
003be020: mov      r0, r8
003be024: bl       #0x318254
003be028: cmp      sb, #0
003be02c: bne      #0x3bdf54
003be030: movw     r3, #0x14f0
003be034: ldrb     r3, [r6, r3]
003be038: cmp      r3, #0
003be03c: bne      #0x3bdf54
003be040: mov      r0, r6
003be044: mov      r1, r7
003be048: bl       #0x3bd40c
003be04c: cmp      r0, #0
003be050: beq      #0x3bdf58
003be054: rsb      r2, r7, #0
003be058: add      r0, r6, #0x560
003be05c: mov      r1, #0x29
003be060: bl       #0x3e0708
003be064: mov      r0, sl
003be068: bl       #0x337888
003be06c: ldr      r1, [pc, #0x58]
003be070: add      r6, sp, #0x14
003be074: add      r2, sp, #0xc
003be078: add      r1, pc, r1
003be07c: mov      r0, r6
003be080: bl       #0x3140ec
003be084: mov      r1, r6
003be088: mov      r0, sl
003be08c: bl       #0x337a88
003be090: mov      r0, r6
003be094: bl       #0x318254
003be098: mov      r0, #1
003be09c: b        #0x3bdf58
003be0a0: bl       #0x30e310
003be0a4: subseq   r6, sp, ip, lsl #23
003be0a8: andeq    r4, r0, ip, lsr #1
003be0ac: subseq   r6, r0, r4, lsr sl
003be0b0: strdeq   r3, r4, [r0], -r4
003be0b4: andeq    r3, r0, r0, asr #19
003be0b8: andeq    r1, r0, r0, asr #19
003be0bc: subseq   r0, r0, r8, lsl #8
003be0c0: subseq   r6, r0, ip, lsl sb
003be0c4: subseq   r6, r0, r8, lsr #18
003be0c8: andeq    r0, r0, r4, lsl #17
003be0cc: subseq   r6, r0, r8, lsl r8

# _ZNK3sfc6script3lua12ReturnValuesixEj
003da43c: push     {r4, r5, r6, lr}
003da440: ldr      r4, [r0, #0x24]
003da444: mov      r5, r1
003da448: ldm      r4, {r2, r3}
003da44c: rsb      r3, r2, r3
003da450: asr      r3, r3, #4
003da454: add      r1, r3, r3, lsl #3
003da458: add      r1, r1, r1, lsl #6
003da45c: add      r1, r3, r1, lsl #3
003da460: add      r1, r1, r1, lsl #15
003da464: add      r3, r3, r1, lsl #3
003da468: rsb      r3, r3, #0
003da46c: cmp      r5, r3
003da470: blo      #0x3da484
003da474: ldr      r0, [pc, #0x14]
003da478: add      r0, pc, r0
003da47c: bl       #0x708eb0
003da480: ldr      r2, [r4]
003da484: mov      r0, #0x70
003da488: mla      r0, r0, r5, r2
003da48c: pop      {r4, r5, r6, pc}
003da490: strdeq   r3, r4, [lr], #-0xf0

# _ZN17CharAISkillScript11GetCooldownEv
003da3d0: push     {r4, lr}
003da3d4: ldr      r1, [r0, #0x18]
003da3d8: sub      sp, sp, #8
003da3dc: cmn      r1, #1
003da3e0: beq      #0x3da430
003da3e4: ldr      r0, [r0, #4]
003da3e8: add      r2, sp, #4
003da3ec: mov      r3, sp
003da3f0: add      r0, r0, #0x3b4
003da3f4: bl       #0x3db344
003da3f8: cmp      r0, #0
003da3fc: beq      #0x3da430
003da400: ldr      r0, [sp, #4]
003da404: bl       #0x30e2e0
003da408: mov      r4, r0
003da40c: ldr      r0, [sp]
003da410: bl       #0x30e2e0
003da414: mov      r1, r0
003da418: mov      r0, r4
003da41c: bl       #0x30ec94
003da420: mov      r1, r0
003da424: mov      r0, #0x3f800000
003da428: bl       #0x30e3ac
003da42c: b        #0x3da434
003da430: mov      r0, #0
003da434: add      sp, sp, #8
003da438: pop      {r4, pc}

# _ZNK16CharStateMachine12SM_IsCastingEv
003c0334: push     {r4, lr}
003c0338: bl       #0x3c01ac
003c033c: cmp      r0, #7
003c0340: movne    r0, #0
003c0344: moveq    r0, #1
003c0348: pop      {r4, pc}

# _ZN17CharAISkillScript7OnSkillEv
003da794: push     {r4, r5, r6, r7, lr}
003da798: ldr      r4, [pc, #0x108]
003da79c: ldr      r7, [pc, #0x108]
003da7a0: sub      sp, sp, #0x34
003da7a4: add      r4, pc, r4
003da7a8: ldr      r3, [r4, r7]
003da7ac: add      r5, sp, #4
003da7b0: mov      r6, r0
003da7b4: ldr      r3, [r3]
003da7b8: mov      r0, r5
003da7bc: str      r3, [sp, #0x2c]
003da7c0: bl       #0x31b434
003da7c4: ldr      r3, [r6, #4]
003da7c8: ldr      r0, [r3, #0x3e4]
003da7cc: cmp      r0, #0
003da7d0: beq      #0x3da7f4
003da7d4: ldr      r1, [pc, #0xd4]
003da7d8: mov      r3, r5
003da7dc: add      r2, r6, #0xc
003da7e0: add      r1, pc, r1
003da7e4: bl       #0x37c390
003da7e8: ldr      r3, [sp, #0xc]
003da7ec: cmp      r3, #0
003da7f0: beq      #0x3da820
003da7f4: mov      r6, #0
003da7f8: mov      r0, r5
003da7fc: bl       #0x31b398
003da800: ldr      r3, [r4, r7]
003da804: ldr      r2, [sp, #0x2c]
003da808: mov      r0, r6
003da80c: ldr      r3, [r3]
003da810: cmp      r2, r3
003da814: bne      #0x3da8a4
003da818: add      sp, sp, #0x34
003da81c: pop      {r4, r5, r6, r7, pc}
003da820: ldr      r0, [sp, #0x28]
003da824: ldm      r0, {r1, r2}
003da828: cmp      r1, r2
003da82c: beq      #0x3da838
003da830: mov      r3, sp
003da834: bl       #0x31c3cc
003da838: ldr      r3, [r6, #4]
003da83c: ldr      r1, [pc, #0x70]
003da840: mov      r2, r5
003da844: ldr      r0, [r3, #0x3e4]
003da848: add      r1, pc, r1
003da84c: bl       #0x37c494
003da850: ldr      r1, [sp, #0xc]
003da854: cmp      r1, #0
003da858: bne      #0x3da7f4
003da85c: ldr      r2, [sp, #0x28]
003da860: ldr      r3, [r2]
003da864: ldr      r2, [r2, #4]
003da868: rsb      r3, r3, r2
003da86c: asr      r3, r3, #4
003da870: add      r2, r3, r3, lsl #3
003da874: add      r2, r2, r2, lsl #6
003da878: add      r2, r3, r2, lsl #3
003da87c: add      r2, r2, r2, lsl #15
003da880: add      r3, r3, r2, lsl #3
003da884: cmp      r3, #0
003da888: moveq    r6, #1
003da88c: beq      #0x3da7f8
003da890: mov      r0, r5
003da894: bl       #0x3da43c
003da898: bl       #0x31bc80
003da89c: mov      r6, r0
003da8a0: b        #0x3da7f8
003da8a4: bl       #0x30e310
003da8a8: subseq   sl, fp, ip, ror #5
003da8ac: andeq    r4, r0, ip, lsr #1
003da8b0: subeq    fp, lr, r0, ror r0
003da8b4: subeq    fp, lr, r8, lsr #32

# _ZN3sfc6script3lua12ReturnValues11pushPointerEPv
0038eb00: ldr      r3, [pc, #0x58]
0038eb04: ldr      r2, [pc, #0x58]
0038eb08: push     {r4, r5, r6, lr}
0038eb0c: add      r3, pc, r3
0038eb10: ldr      r5, [r3, r2]
0038eb14: sub      sp, sp, #0x78
0038eb18: add      r4, sp, #4
0038eb1c: ldr      r3, [r5]
0038eb20: str      r3, [sp, #0x74]
0038eb24: ldr      r6, [r0, #0x24]
0038eb28: mov      r0, r4
0038eb2c: bl       #0x31a414
0038eb30: mov      r0, r6
0038eb34: mov      r1, r4
0038eb38: bl       #0x3195c0
0038eb3c: mov      r0, r4
0038eb40: bl       #0x3193e8
0038eb44: ldr      r2, [sp, #0x74]
0038eb48: ldr      r3, [r5]
0038eb4c: cmp      r2, r3
0038eb50: bne      #0x38eb5c
0038eb54: add      sp, sp, #0x78
0038eb58: pop      {r4, r5, r6, pc}
0038eb5c: bl       #0x30e310
0038eb60: rsbeq    r5, r0, r4, lsl #31
0038eb64: andeq    r4, r0, ip, lsr #1

# _ZNK16CharStateMachine15SM_IsUsingSkillEv
003c02e8: push     {r4, lr}
003c02ec: bl       #0x3c01ac
003c02f0: cmp      r0, #6
003c02f4: movne    r0, #0
003c02f8: moveq    r0, #1
003c02fc: pop      {r4, pc}
