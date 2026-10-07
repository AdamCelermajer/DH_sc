
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

# _ZN14CharProperties12PROPS_AddDotEiii
003e2720: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e2724: ldr      r4, [pc, #0x2c4]
003e2728: sub      sp, sp, #0x1c
003e272c: cmp      r1, #0
003e2730: add      r4, pc, r4
003e2734: str      r1, [sp, #0x14]
003e2738: mov      r7, r0
003e273c: mov      r6, r2
003e2740: mov      r5, r3
003e2744: ble      #0x3e288c
003e2748: cmp      r6, #0
003e274c: blt      #0x3e2834
003e2750: ldr      r3, [pc, #0x29c]
003e2754: ldr      r3, [r4, r3]
003e2758: ldr      sl, [r3]
003e275c: cmp      sl, #0
003e2760: beq      #0x3e282c
003e2764: ldr      r3, [pc, #0x28c]
003e2768: ldr      fp, [pc, #0x28c]
003e276c: mov      r8, #0
003e2770: ldr      r3, [r4, r3]
003e2774: add      fp, pc, fp
003e2778: ldr      sb, [r3]
003e277c: b        #0x3e278c
003e2780: add      r8, r8, #1
003e2784: cmp      r8, sl
003e2788: beq      #0x3e282c
003e278c: ldr      r1, [sb, r8, lsl #2]
003e2790: mov      r0, fp
003e2794: bl       #0x30e31c
003e2798: cmp      r0, #0
003e279c: bne      #0x3e2780
003e27a0: ldr      r3, [pc, #0x258]
003e27a4: add      r8, r8, r5
003e27a8: str      r8, [sp, #0x10]
003e27ac: ldr      r3, [r4, r3]
003e27b0: ldr      sl, [r3]
003e27b4: cmp      sl, #0
003e27b8: beq      #0x3e2824
003e27bc: ldr      r3, [pc, #0x240]
003e27c0: ldr      fp, [pc, #0x240]
003e27c4: mov      r8, #0
003e27c8: ldr      r3, [r4, r3]
003e27cc: add      fp, pc, fp
003e27d0: ldr      sb, [r3]
003e27d4: b        #0x3e27e4
003e27d8: add      r8, r8, #1
003e27dc: cmp      r8, sl
003e27e0: beq      #0x3e2824
003e27e4: ldr      r1, [sb, r8, lsl #2]
003e27e8: mov      r0, fp
003e27ec: bl       #0x30e31c
003e27f0: cmp      r0, #0
003e27f4: bne      #0x3e27d8
003e27f8: add      r3, r5, #1
003e27fc: add      r8, r8, r5
003e2800: cmp      r3, #5
003e2804: addls    pc, pc, r3, lsl #2
003e2808: b        #0x3e28e4
003e280c: b        #0x3e2950
003e2810: b        #0x3e295c
003e2814: b        #0x3e2968
003e2818: b        #0x3e2974
003e281c: b        #0x3e2980
003e2820: b        #0x3e298c
003e2824: mvn      r8, #0
003e2828: b        #0x3e27f8
003e282c: mvn      r8, #0
003e2830: b        #0x3e27a0
003e2834: ldr      r3, [pc, #0x1d0]
003e2838: ldr      r3, [r4, r3]
003e283c: ldr      r3, [r3]
003e2840: cmp      r3, #2
003e2844: moveq    r3, #0
003e2848: streq    r3, [r3]
003e284c: beq      #0x3e2750
003e2850: cmp      r3, #1
003e2854: bne      #0x3e2750
003e2858: ldr      r0, [pc, #0x1b0]
003e285c: ldr      r1, [pc, #0x1b0]
003e2860: ldr      r2, [pc, #0x1b0]
003e2864: ldr      r0, [r4, r0]
003e2868: ldr      r3, [pc, #0x1ac]
003e286c: movw     ip, #0x436
003e2870: add      r1, pc, r1
003e2874: add      r2, pc, r2
003e2878: add      r3, pc, r3
003e287c: add      r0, r0, #0xa8
003e2880: str      ip, [sp]
003e2884: bl       #0x30e004
003e2888: b        #0x3e2750
003e288c: ldr      r3, [pc, #0x178]
003e2890: ldr      r3, [r4, r3]
003e2894: ldr      r3, [r3]
003e2898: cmp      r3, #2
003e289c: moveq    r3, #0
003e28a0: streq    r3, [r3]
003e28a4: beq      #0x3e2748
003e28a8: cmp      r3, #1
003e28ac: bne      #0x3e2748
003e28b0: ldr      r0, [pc, #0x158]
003e28b4: ldr      r1, [pc, #0x164]
003e28b8: ldr      r2, [pc, #0x164]
003e28bc: ldr      r0, [r4, r0]
003e28c0: ldr      r3, [pc, #0x160]
003e28c4: movw     ip, #0x435
003e28c8: add      r1, pc, r1
003e28cc: add      r2, pc, r2
003e28d0: add      r3, pc, r3
003e28d4: add      r0, r0, #0xa8
003e28d8: str      ip, [sp]
003e28dc: bl       #0x30e004
003e28e0: b        #0x3e2748
003e28e4: ldr      r3, [pc, #0x120]
003e28e8: ldr      r3, [r4, r3]
003e28ec: ldr      r3, [r3]
003e28f0: cmp      r3, #2
003e28f4: beq      #0x3e29a0
003e28f8: cmp      r3, #1
003e28fc: beq      #0x3e29b4
003e2900: ldr      ip, [pc, #0x124]
003e2904: add      ip, pc, ip
003e2908: ldr      r1, [sp, #0x10]
003e290c: ldr      r2, [sp, #0x14]
003e2910: mov      r0, r7
003e2914: mov      r3, #1
003e2918: stm      sp, {r6, r8, ip}
003e291c: bl       #0x3e232c
003e2920: subs     r1, r0, #0
003e2924: beq      #0x3e2998
003e2928: add      r5, r5, #0x7f
003e292c: mov      r0, r7
003e2930: mov      r3, r6
003e2934: mov      r2, r5
003e2938: bl       #0x3deca0
003e293c: mov      r0, r7
003e2940: mov      r1, r5
003e2944: add      sp, sp, #0x1c
003e2948: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e294c: b        #0x3dfe60
003e2950: ldr      ip, [pc, #0xd8]
003e2954: add      ip, pc, ip
003e2958: b        #0x3e2908
003e295c: ldr      ip, [pc, #0xd0]
003e2960: add      ip, pc, ip
003e2964: b        #0x3e2908
003e2968: ldr      ip, [pc, #0xc8]
003e296c: add      ip, pc, ip
003e2970: b        #0x3e2908
003e2974: ldr      ip, [pc, #0xc0]
003e2978: add      ip, pc, ip
003e297c: b        #0x3e2908
003e2980: ldr      ip, [pc, #0xb8]
003e2984: add      ip, pc, ip
003e2988: b        #0x3e2908
003e298c: ldr      ip, [pc, #0xb0]
003e2990: add      ip, pc, ip
003e2994: b        #0x3e2908
003e2998: add      sp, sp, #0x1c
003e299c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e29a0: ldr      ip, [pc, #0xa0]
003e29a4: mov      r3, #0
003e29a8: str      r3, [r3]
003e29ac: add      ip, pc, ip
003e29b0: b        #0x3e2908
003e29b4: ldr      r0, [pc, #0x54]
003e29b8: ldr      r1, [pc, #0x8c]
003e29bc: ldr      r2, [pc, #0x8c]
003e29c0: ldr      r0, [r4, r0]
003e29c4: ldr      r3, [pc, #0x88]
003e29c8: movw     ip, #0x445
003e29cc: add      r1, pc, r1
003e29d0: add      r0, r0, #0xa8
003e29d4: add      r2, pc, r2
003e29d8: add      r3, pc, r3
003e29dc: str      ip, [sp]
003e29e0: bl       #0x30e004
003e29e4: ldr      ip, [pc, #0x6c]
003e29e8: add      ip, pc, ip
003e29ec: b        #0x3e2908
003e29f0: subseq   r2, fp, r0, ror #6
003e29f4: andeq    r3, r0, r8, ror #10
003e29f8: muleq    r0, r0, sl
003e29fc: subeq    r3, lr, r4, asr r6
003e2a00: andeq    r0, r0, r4, asr #13
003e2a04: muleq    r0, r4, r2
003e2a08: strdeq   r3, r4, [lr], #-0x5c
003e2a0c: andeq    r3, r0, r0, asr #19
003e2a10: andeq    r1, r0, r0, asr #19
003e2a14: subeq    fp, sp, r8, ror #22
003e2a18: subeq    r3, lr, r4, asr #10
003e2a1c: subeq    r3, lr, r8, lsr r4
003e2a20: subeq    fp, sp, r0, lsl fp
003e2a24: ldrdeq   r3, r4, [lr], #-0x4c
003e2a28: subeq    r3, lr, r0, ror #7
003e2a2c: subeq    r3, lr, r4, lsr r5
003e2a30: umaaleq  r3, lr, ip, r4
003e2a34: subeq    r3, lr, r0, lsl #9
003e2a38: umaaleq  r3, lr, r4, r4
003e2a3c: umaaleq  r3, lr, r8, r4
003e2a40: umaaleq  r3, lr, ip, r4
003e2a44: subeq    r3, lr, r0, lsr #9
003e2a48: subeq    r3, lr, ip, lsl #9
003e2a4c: subeq    fp, sp, ip, lsl #20
003e2a50: subeq    r3, lr, r4, ror r4
003e2a54: ldrdeq   r3, r4, [lr], #-0x28
003e2a58: subeq    r3, lr, r0, asr r4

# _ZNSt3mapIiN14CharProperties8BuffDeclESt4lessIiESaISt4pairIKiS1_EEEixIiEERS1_RKT_
003e209c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003e20a0: ldr      r5, [pc, #0xf8]
003e20a4: ldr      r6, [pc, #0xf8]
003e20a8: ldr      r4, [r0, #4]
003e20ac: add      r5, pc, r5
003e20b0: ldr      r3, [r5, r6]
003e20b4: sub      sp, sp, #0xa0
003e20b8: cmp      r4, #0
003e20bc: ldr      r3, [r3]
003e20c0: mov      sl, r0
003e20c4: mov      r8, r1
003e20c8: str      r3, [sp, #0x9c]
003e20cc: moveq    r4, r0
003e20d0: beq      #0x3e2104
003e20d4: ldr      r1, [r1]
003e20d8: mov      r2, r0
003e20dc: b        #0x3e20e8
003e20e0: mov      r2, r4
003e20e4: mov      r4, r3
003e20e8: ldr      r3, [r4, #0x10]
003e20ec: cmp      r1, r3
003e20f0: ldrgt    r3, [r4, #0xc]
003e20f4: ldrle    r3, [r4, #8]
003e20f8: movgt    r4, r2
003e20fc: cmp      r3, #0
003e2100: bne      #0x3e20e0
003e2104: cmp      sl, r4
003e2108: beq      #0x3e2140
003e210c: ldr      r2, [r8]
003e2110: ldr      r3, [r4, #0x10]
003e2114: mov      r0, r4
003e2118: cmp      r2, r3
003e211c: blt      #0x3e2140
003e2120: ldr      r3, [r5, r6]
003e2124: ldr      r2, [sp, #0x9c]
003e2128: add      r0, r0, #0x14
003e212c: ldr      r3, [r3]
003e2130: cmp      r2, r3
003e2134: bne      #0x3e219c
003e2138: add      sp, sp, #0xa0
003e213c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003e2140: add      r7, sp, #0x54
003e2144: mov      r0, r7
003e2148: bl       #0x3e1a08
003e214c: ldr      r3, [r8]
003e2150: add      sb, sp, #0xa0
003e2154: mov      r1, r7
003e2158: str      r3, [sb, #-0x98]!
003e215c: add      r8, sb, #4
003e2160: mov      r0, r8
003e2164: bl       #0x3e19c0
003e2168: mov      r1, sl
003e216c: mov      r3, sb
003e2170: mov      r2, sp
003e2174: add      r0, sp, #4
003e2178: str      r4, [sp]
003e217c: bl       #0x3e1d28
003e2180: ldr      r4, [sp, #4]
003e2184: mov      r0, r8
003e2188: bl       #0x3e0a68
003e218c: mov      r0, r7
003e2190: bl       #0x3e0a68
003e2194: mov      r0, r4
003e2198: b        #0x3e2120
003e219c: bl       #0x30e310
003e21a0: subseq   r2, fp, r4, ror #19
003e21a4: andeq    r4, r0, ip, lsr #1

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

# _ZNSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE10_M_advanceEi
003de8b4: ldr      r3, [r0]
003de8b8: ldr      r2, [r0, #4]
003de8bc: str      r4, [sp, #-4]!
003de8c0: rsb      r2, r2, r3
003de8c4: add      r2, r1, r2, asr #2
003de8c8: mvn      ip, r2
003de8cc: lsr      r4, ip, #0x1f
003de8d0: cmp      r2, #0x1f
003de8d4: movgt    r4, #0
003de8d8: andle    r4, r4, #1
003de8dc: cmp      r4, #0
003de8e0: addne    r3, r3, r1, lsl #2
003de8e4: strne    r3, [r0]
003de8e8: bne      #0x3de920
003de8ec: ldr      r1, [r0, #0xc]
003de8f0: cmp      r2, #0
003de8f4: lsrgt    r3, r2, #5
003de8f8: mvnle    r3, ip, lsr #5
003de8fc: add      ip, r1, r3, lsl #2
003de900: str      ip, [r0, #0xc]
003de904: sub      r2, r2, r3, lsl #5
003de908: ldr      r3, [r1, r3, lsl #2]
003de90c: add      r2, r3, r2, lsl #2
003de910: add      r1, r3, #0x80
003de914: str      r2, [r0]
003de918: str      r1, [r0, #8]
003de91c: str      r3, [r0, #4]
003de920: ldm      sp!, {r4}
003de924: bx       lr

# _ZNKSt4priv20_Deque_iterator_baseIPN14CharProperties8BuffInstEE11_M_subtractERKS4_
003de870: push     {r4, r5}
003de874: ldr      ip, [r1, #0xc]
003de878: ldr      r5, [r0]
003de87c: ldr      r2, [r0, #4]
003de880: ldr      r4, [r0, #0xc]
003de884: ldr      r3, [r1, #8]
003de888: ldr      r1, [r1]
003de88c: rsb      r2, r2, r5
003de890: rsb      r0, ip, r4
003de894: rsb      r3, r1, r3
003de898: asr      r2, r2, #2
003de89c: asr      r0, r0, #2
003de8a0: add      r3, r2, r3, asr #2
003de8a4: sub      r0, r0, #1
003de8a8: add      r0, r3, r0, lsl #5
003de8ac: pop      {r4, r5}
003de8b0: bx       lr

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

# _ZN14CharPropertiesD1Ev
003e0c6c: ldr      r2, [pc, #0x190]
003e0c70: ldr      r3, [pc, #0x190]
003e0c74: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003e0c78: add      r2, pc, r2
003e0c7c: ldr      r3, [r2, r3]
003e0c80: sub      sp, sp, #0x34
003e0c84: str      r2, [sp, #8]
003e0c88: add      r3, r3, #8
003e0c8c: str      r3, [r0]
003e0c90: ldr      r3, [pc, #0x174]
003e0c94: add      sb, r0, #0xe10
003e0c98: ldr      r7, [r0, #0xe20]
003e0c9c: mov      fp, r0
003e0ca0: add      sb, sb, #8
003e0ca4: add      sl, sp, #0x20
003e0ca8: add      r4, sp, #0x10
003e0cac: str      r3, [sp, #0xc]
003e0cb0: cmp      sb, r7
003e0cb4: beq      #0x3e0d74
003e0cb8: add      r5, r7, #0x34
003e0cbc: ldm      r5, {r0, r1, r2, r3}
003e0cc0: stm      sl, {r0, r1, r2, r3}
003e0cc4: add      r0, r7, #0x44
003e0cc8: mov      r1, sl
003e0ccc: bl       #0x3de870
003e0cd0: subs     r8, r0, #0
003e0cd4: beq      #0x3e0d20
003e0cd8: mov      r6, #0
003e0cdc: ldm      r5, {r0, r1, r2, r3}
003e0ce0: stm      r4, {r0, r1, r2, r3}
003e0ce4: mov      r1, r6
003e0ce8: mov      r0, r4
003e0cec: bl       #0x3de8b4
003e0cf0: ldr      r3, [sp, #0x10]
003e0cf4: add      r6, r6, #1
003e0cf8: ldr      r3, [r3]
003e0cfc: subs     r0, r3, #0
003e0d00: beq      #0x3e0d18
003e0d04: str      r3, [sp, #4]
003e0d08: bl       #0x4c5740
003e0d0c: ldr      r3, [sp, #4]
003e0d10: mov      r0, r3
003e0d14: bl       #0x310440
003e0d18: cmp      r6, r8
003e0d1c: bne      #0x3e0cdc
003e0d20: mov      r0, r5
003e0d24: bl       #0x3e092c
003e0d28: ldr      r3, [r7, #0x18]
003e0d2c: cmp      r3, #0
003e0d30: beq      #0x3e0d48
003e0d34: ldr      r3, [sp, #8]
003e0d38: ldr      r2, [sp, #0xc]
003e0d3c: add      r1, r7, #0x18
003e0d40: ldr      r0, [r3, r2]
003e0d44: bl       #0x494978
003e0d48: ldr      r2, [r7, #0xc]
003e0d4c: cmp      r2, #0
003e0d50: bne      #0x3e0d5c
003e0d54: b        #0x3e0dd0
003e0d58: mov      r2, r3
003e0d5c: ldr      r3, [r2, #8]
003e0d60: cmp      r3, #0
003e0d64: bne      #0x3e0d58
003e0d68: mov      r7, r2
003e0d6c: cmp      sb, r7
003e0d70: bne      #0x3e0cb8
003e0d74: ldr      r3, [fp, #0xe28]
003e0d78: cmp      r3, #0
003e0d7c: beq      #0x3e0da0
003e0d80: mov      r0, sb
003e0d84: ldr      r1, [fp, #0xe1c]
003e0d88: bl       #0x3e0ab8
003e0d8c: mov      r3, #0
003e0d90: str      sb, [fp, #0xe24]
003e0d94: str      r3, [fp, #0xe28]
003e0d98: str      sb, [fp, #0xe20]
003e0d9c: str      r3, [fp, #0xe1c]
003e0da0: add      r0, fp, #0xa90
003e0da4: add      r0, r0, #4
003e0da8: bl       #0x4c5740
003e0dac: add      r0, fp, #0x710
003e0db0: bl       #0x4c5740
003e0db4: add      r0, fp, #0x38c
003e0db8: bl       #0x4c5740
003e0dbc: add      r0, fp, #8
003e0dc0: bl       #0x4c5740
003e0dc4: mov      r0, fp
003e0dc8: add      sp, sp, #0x34
003e0dcc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e0dd0: ldr      r3, [r7, #4]
003e0dd4: ldr      r1, [r3, #0xc]
003e0dd8: cmp      r7, r1
003e0ddc: bne      #0x3e0df8
003e0de0: mov      r7, r3
003e0de4: ldr      r3, [r3, #4]
003e0de8: ldr      r2, [r3, #0xc]
003e0dec: cmp      r2, r7
003e0df0: beq      #0x3e0de0
003e0df4: ldr      r2, [r7, #0xc]
003e0df8: cmp      r3, r2
003e0dfc: movne    r7, r3
003e0e00: b        #0x3e0cb0
003e0e04: subseq   r3, fp, r8, lsl lr
003e0e08: ldrdeq   r3, r4, [r0], -r0
003e0e0c: andeq    r1, r0, r8, lsl #22

# _ZN14CharProperties8BuffDeclC1Ev
003e1a08: push     {r4, r5, r6, lr}
003e1a0c: add      r3, r0, #8
003e1a10: mov      r4, r0
003e1a14: mov      r5, #0
003e1a18: mvn      r2, #0
003e1a1c: str      r2, [r0]
003e1a20: mov      r1, #0x10
003e1a24: mov      r0, r3
003e1a28: str      r3, [r4, #0x18]
003e1a2c: str      r3, [r4, #0x1c]
003e1a30: str      r5, [r4, #4]
003e1a34: bl       #0x31167c
003e1a38: ldr      r3, [r4, #0x18]
003e1a3c: add      r0, r4, #0x20
003e1a40: mov      r1, r5
003e1a44: strb     r5, [r3]
003e1a48: str      r5, [r4, #0x20]
003e1a4c: str      r5, [r4, #0x24]
003e1a50: str      r5, [r4, #0x28]
003e1a54: str      r5, [r4, #0x2c]
003e1a58: str      r5, [r4, #0x30]
003e1a5c: str      r5, [r4, #0x34]
003e1a60: str      r5, [r4, #0x38]
003e1a64: str      r5, [r4, #0x3c]
003e1a68: str      r5, [r4, #0x40]
003e1a6c: str      r5, [r4, #0x44]
003e1a70: bl       #0x3e1818
003e1a74: mov      r0, r4
003e1a78: pop      {r4, r5, r6, pc}

# _ZN14CharProperties12_SetPropertyERN7Structs19CharacterPropertiesEii
003deca0: str      lr, [sp, #-4]!
003deca4: ldr      ip, [pc, #0xe0]
003deca8: cmp      r2, #0
003decac: sub      sp, sp, #0xc
003decb0: add      ip, pc, ip
003decb4: blt      #0x3dece4
003decb8: cmp      r2, #0xdf
003decbc: ble      #0x3ded04
003decc0: ldr      r3, [pc, #0xc8]
003decc4: ldr      r3, [ip, r3]
003decc8: ldr      r3, [r3]
003deccc: cmp      r3, #2
003decd0: beq      #0x3decf8
003decd4: cmp      r3, #1
003decd8: beq      #0x3ded58
003decdc: add      sp, sp, #0xc
003dece0: ldm      sp!, {pc}
003dece4: ldr      r3, [pc, #0xa4]
003dece8: ldr      r3, [ip, r3]
003decec: ldr      r3, [r3]
003decf0: cmp      r3, #2
003decf4: bne      #0x3ded1c
003decf8: mov      r3, #0
003decfc: str      r3, [r3]
003ded00: b        #0x3decdc
003ded04: ldr      r0, [pc, #0x88]
003ded08: ldr      r0, [ip, r0]
003ded0c: ldr      r2, [r0, r2, lsl #2]
003ded10: add      r1, r1, r2
003ded14: str      r3, [r1, #4]
003ded18: b        #0x3decdc
003ded1c: cmp      r3, #1
003ded20: bne      #0x3decdc
003ded24: ldr      r0, [pc, #0x6c]
003ded28: ldr      r1, [pc, #0x6c]
003ded2c: ldr      r2, [pc, #0x6c]
003ded30: ldr      r0, [ip, r0]
003ded34: ldr      r3, [pc, #0x68]
003ded38: movw     ip, #0x113
003ded3c: add      r1, pc, r1
003ded40: add      r2, pc, r2
003ded44: add      r3, pc, r3
003ded48: add      r0, r0, #0xa8
003ded4c: str      ip, [sp]
003ded50: bl       #0x30e004
003ded54: b        #0x3decdc
003ded58: ldr      r0, [pc, #0x38]
003ded5c: ldr      r1, [pc, #0x44]
003ded60: ldr      r2, [pc, #0x44]
003ded64: ldr      r0, [ip, r0]
003ded68: ldr      r3, [pc, #0x40]
003ded6c: mov      ip, #0x114
003ded70: add      r1, pc, r1
003ded74: add      r2, pc, r2
003ded78: add      r3, pc, r3
003ded7c: add      r0, r0, #0xa8
003ded80: str      ip, [sp]
003ded84: bl       #0x30e004
003ded88: b        #0x3decdc
003ded8c: subseq   r5, fp, r0, ror #27
003ded90: andeq    r3, r0, r0, asr #19
003ded94: andeq    r2, r0, r8, lsr #5
003ded98: andeq    r1, r0, r0, asr #19
003ded9c: umaaleq  pc, sp, ip, r6
003deda0: ldrdeq   r6, r7, [lr], #-0xf0
003deda4: subeq    r6, lr, ip, ror #30
003deda8: subeq    pc, sp, r8, ror #12
003dedac: subeq    r6, lr, ip, lsr #31
003dedb0: subeq    r6, lr, r8, lsr pc

# _ZN7Structs19CharacterPropertiesD1Ev
004c5740: bx       lr

# _ZN14CharProperties16RecalcPropertiesEb
003e0810: cmp      r1, #0
003e0814: push     {r4, r5, r6, lr}
003e0818: mov      r4, r0
003e081c: bne      #0x3e0840
003e0820: mov      r5, #0
003e0824: mov      r1, r5
003e0828: mov      r0, r4
003e082c: add      r5, r5, #1
003e0830: bl       #0x3dfe60
003e0834: cmp      r5, #0xe0
003e0838: bne      #0x3e0824
003e083c: pop      {r4, r5, r6, pc}
003e0840: add      r1, r0, #8
003e0844: ldr      r2, [r0, #0x74]
003e0848: mov      r3, #0
003e084c: bl       #0x3e2e20
003e0850: b        #0x3e0820

# _ZN14CharProperties16_ResetPropertiesERN7Structs19CharacterPropertiesE
003def34: ldr      r3, [pc, #0x40]
003def38: ldr      r2, [pc, #0x40]
003def3c: push     {r4, r5, r6, r7, r8, lr}
003def40: add      r3, pc, r3
003def44: mov      r8, r0
003def48: ldr      r7, [r3, r2]
003def4c: mov      r6, r1
003def50: mov      r4, #0
003def54: mov      r1, r4
003def58: mov      r0, r8
003def5c: ldr      r5, [r7, r4, lsl #2]
003def60: bl       #0x3def10
003def64: add      r4, r4, #1
003def68: add      r5, r5, #4
003def6c: cmp      r4, #0xe0
003def70: str      r0, [r6, r5]
003def74: bne      #0x3def54
003def78: pop      {r4, r5, r6, r7, r8, pc}
003def7c: subseq   r5, fp, r0, asr fp
003def80: andeq    r2, r0, r8, lsr #5

# _ZN14CharProperties8BuffDeclD1Ev
003e0a68: push     {r4, lr}
003e0a6c: mov      r4, r0
003e0a70: add      r0, r0, #0x20
003e0a74: bl       #0x3e09a4
003e0a78: add      r3, r4, #8
003e0a7c: ldr      r0, [r3, #0x14]
003e0a80: cmp      r0, r3
003e0a84: beq      #0x3e0aa4
003e0a88: cmp      r0, #0
003e0a8c: beq      #0x3e0aa4
003e0a90: ldr      r1, [r4, #8]
003e0a94: rsb      r1, r0, r1
003e0a98: cmp      r1, #0x80
003e0a9c: bhi      #0x3e0aac
003e0aa0: bl       #0x708f00
003e0aa4: mov      r0, r4
003e0aa8: pop      {r4, pc}
003e0aac: bl       #0x310440
003e0ab0: mov      r0, r4
003e0ab4: pop      {r4, pc}

# _ZN14CharProperties14RecalcPropertyEi
003dfe60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003dfe64: sub      sp, sp, #0x54
003dfe68: mov      r6, r0
003dfe6c: mov      r7, r1
003dfe70: bl       #0x3deed8
003dfe74: tst      r0, #4
003dfe78: bne      #0x3e0110
003dfe7c: tst      r0, #2
003dfe80: bne      #0x3e02bc
003dfe84: tst      r0, #1
003dfe88: beq      #0x3e0080
003dfe8c: add      r2, r6, #0xa90
003dfe90: add      r2, r2, #4
003dfe94: str      r2, [sp, #4]
003dfe98: ldr      r3, [r6, #0xe20]
003dfe9c: add      sb, r6, #0xe10
003dfea0: add      sb, sb, #8
003dfea4: str      r3, [sp, #0xc]
003dfea8: ldr      r2, [sp, #0xc]
003dfeac: add      ip, sp, #0x20
003dfeb0: str      ip, [sp, #8]
003dfeb4: cmp      r2, sb
003dfeb8: add      r4, sp, #0x10
003dfebc: mov      r8, r6
003dfec0: beq      #0x3e0018
003dfec4: ldrb     r3, [sb]
003dfec8: cmp      r3, #0
003dfecc: bne      #0x3dfee4
003dfed0: ldr      r3, [sb, #4]
003dfed4: ldr      r3, [r3, #4]
003dfed8: cmp      r3, sb
003dfedc: ldreq    ip, [sb, #0xc]
003dfee0: beq      #0x3dff04
003dfee4: ldr      ip, [sb, #8]
003dfee8: cmp      ip, #0
003dfeec: bne      #0x3dfef8
003dfef0: b        #0x3e00b4
003dfef4: mov      ip, r3
003dfef8: ldr      r3, [ip, #0xc]
003dfefc: cmp      r3, #0
003dff00: bne      #0x3dfef4
003dff04: ldr      lr, [sp, #8]
003dff08: add      r5, ip, #0x34
003dff0c: ldm      r5, {r0, r1, r2, r3}
003dff10: stm      lr, {r0, r1, r2, r3}
003dff14: add      r0, ip, #0x44
003dff18: ldr      r1, [sp, #8]
003dff1c: bl       #0x3de870
003dff20: subs     sl, r0, #0
003dff24: beq      #0x3dffc8
003dff28: mov      fp, #0
003dff2c: mov      r6, fp
003dff30: ldm      r5, {r0, r1, r2, r3}
003dff34: stm      r4, {r0, r1, r2, r3}
003dff38: mov      r1, r6
003dff3c: mov      r0, r4
003dff40: bl       #0x3de8b4
003dff44: ldm      r5, {r0, r1, r2, r3}
003dff48: stm      r4, {r0, r1, r2, r3}
003dff4c: mov      r1, r6
003dff50: mov      r0, r4
003dff54: bl       #0x3de8b4
003dff58: ldr      r3, [sp, #0x10]
003dff5c: mov      r2, r7
003dff60: mov      r0, r8
003dff64: ldr      r1, [r3]
003dff68: bl       #0x3df114
003dff6c: cmp      r0, #0
003dff70: beq      #0x3dffb4
003dff74: ldm      r5, {r0, r1, r2, r3}
003dff78: stm      r4, {r0, r1, r2, r3}
003dff7c: mov      r1, r6
003dff80: mov      r0, r4
003dff84: bl       #0x3de8b4
003dff88: ldr      r3, [sp, #0x10]
003dff8c: mov      r2, r7
003dff90: mov      r0, r8
003dff94: ldr      r1, [r3]
003dff98: bl       #0x3dedb4
003dff9c: ldr      r1, [sp, #4]
003dffa0: mov      r3, r0
003dffa4: mov      r2, r7
003dffa8: mov      r0, r8
003dffac: bl       #0x3deca0
003dffb0: mov      fp, #1
003dffb4: add      r6, r6, #1
003dffb8: cmp      r6, sl
003dffbc: bne      #0x3dff30
003dffc0: cmp      fp, #0
003dffc4: bne      #0x3e0474
003dffc8: ldrb     r3, [sb]
003dffcc: cmp      r3, #0
003dffd0: bne      #0x3dffe8
003dffd4: ldr      r3, [sb, #4]
003dffd8: ldr      r3, [r3, #4]
003dffdc: cmp      r3, sb
003dffe0: ldreq    r3, [sb, #0xc]
003dffe4: beq      #0x3e0008
003dffe8: ldr      r3, [sb, #8]
003dffec: cmp      r3, #0
003dfff0: bne      #0x3dfffc
003dfff4: b        #0x3e00e4
003dfff8: mov      r3, r2
003dfffc: ldr      r2, [r3, #0xc]
003e0000: cmp      r2, #0
003e0004: bne      #0x3dfff8
003e0008: mov      sb, r3
003e000c: ldr      r2, [sp, #0xc]
003e0010: cmp      r2, sb
003e0014: bne      #0x3dfec4
003e0018: add      r4, r8, #0x710
003e001c: mov      r0, r8
003e0020: mov      r1, r4
003e0024: mov      r2, r7
003e0028: bl       #0x3df114
003e002c: cmp      r0, #0
003e0030: mov      r6, r8
003e0034: bne      #0x3e047c
003e0038: add      r4, r6, #0x38c
003e003c: mov      r0, r6
003e0040: mov      r1, r4
003e0044: mov      r2, r7
003e0048: bl       #0x3df114
003e004c: cmp      r0, #0
003e0050: bne      #0x3e047c
003e0054: add      r4, r6, #8
003e0058: mov      r0, r6
003e005c: mov      r1, r4
003e0060: mov      r2, r7
003e0064: bl       #0x3df114
003e0068: cmp      r0, #0
003e006c: bne      #0x3e047c
003e0070: mov      r0, r6
003e0074: mov      r1, r7
003e0078: bl       #0x3def10
003e007c: b        #0x3e048c
003e0080: tst      r0, #0x20
003e0084: bne      #0x3e0550
003e0088: tst      r0, #0x10
003e008c: addeq    ip, r6, #0xa90
003e0090: addeq    ip, ip, #4
003e0094: streq    ip, [sp, #4]
003e0098: bne      #0x3e0438
003e009c: mov      r0, r6
003e00a0: ldr      r1, [sp, #4]
003e00a4: mov      r2, r7
003e00a8: bl       #0x3dedb4
003e00ac: add      sp, sp, #0x54
003e00b0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003e00b4: ldr      ip, [sb, #4]
003e00b8: ldr      r3, [ip, #8]
003e00bc: cmp      r3, sb
003e00c0: beq      #0x3e00cc
003e00c4: b        #0x3dff04
003e00c8: mov      ip, r3
003e00cc: ldr      r3, [ip, #4]
003e00d0: ldr      r2, [r3, #8]
003e00d4: cmp      r2, ip
003e00d8: beq      #0x3e00c8
003e00dc: mov      ip, r3
003e00e0: b        #0x3dff04
003e00e4: ldr      r3, [sb, #4]
003e00e8: ldr      r2, [r3, #8]
003e00ec: cmp      sb, r2
003e00f0: bne      #0x3e0008
003e00f4: mov      r2, r3
003e00f8: ldr      r3, [r3, #4]
003e00fc: ldr      r1, [r3, #8]
003e0100: cmp      r1, r2
003e0104: beq      #0x3e00f4
003e0108: mov      sb, r3
003e010c: b        #0x3e000c
003e0110: add      r2, r6, #0xa90
003e0114: add      r2, r2, #4
003e0118: mov      r1, r7
003e011c: mov      r0, r6
003e0120: str      r2, [sp, #4]
003e0124: bl       #0x3def10
003e0128: add      r4, r6, #8
003e012c: mov      r3, r0
003e0130: ldr      r1, [sp, #4]
003e0134: mov      r0, r6
003e0138: mov      r2, r7
003e013c: bl       #0x3deca0
003e0140: mov      r0, r6
003e0144: mov      r1, r4
003e0148: mov      r2, r7
003e014c: bl       #0x3df114
003e0150: cmp      r0, #0
003e0154: bne      #0x3e0528
003e0158: add      r4, r6, #0x38c
003e015c: mov      r0, r6
003e0160: mov      r1, r4
003e0164: mov      r2, r7
003e0168: bl       #0x3df114
003e016c: cmp      r0, #0
003e0170: bne      #0x3e0500
003e0174: add      r4, r6, #0x710
003e0178: mov      r0, r6
003e017c: mov      r1, r4
003e0180: mov      r2, r7
003e0184: bl       #0x3df114
003e0188: cmp      r0, #0
003e018c: bne      #0x3e04d8
003e0190: add      r3, r6, #0xe10
003e0194: add      r3, r3, #8
003e0198: str      r3, [sp, #8]
003e019c: ldr      sb, [r6, #0xe20]
003e01a0: add      fp, sp, #0x40
003e01a4: add      r4, sp, #0x10
003e01a8: ldr      ip, [sp, #8]
003e01ac: cmp      sb, ip
003e01b0: beq      #0x3e009c
003e01b4: add      r8, sb, #0x34
003e01b8: ldm      r8, {r0, r1, r2, r3}
003e01bc: stm      fp, {r0, r1, r2, r3}
003e01c0: add      r0, sb, #0x44
003e01c4: mov      r1, fp
003e01c8: bl       #0x3de870
003e01cc: subs     sl, r0, #0
003e01d0: beq      #0x3e0260
003e01d4: mov      r5, #0
003e01d8: b        #0x3e01e8
003e01dc: add      r5, r5, #1
003e01e0: cmp      r5, sl
003e01e4: beq      #0x3e0260
003e01e8: ldm      r8, {r0, r1, r2, r3}
003e01ec: stm      r4, {r0, r1, r2, r3}
003e01f0: mov      r1, r5
003e01f4: mov      r0, r4
003e01f8: bl       #0x3de8b4
003e01fc: ldr      r3, [sp, #0x10]
003e0200: mov      r2, r7
003e0204: mov      r0, r6
003e0208: ldr      r1, [r3]
003e020c: bl       #0x3df114
003e0210: cmp      r0, #0
003e0214: beq      #0x3e01dc
003e0218: ldm      r8, {r0, r1, r2, r3}
003e021c: stm      r4, {r0, r1, r2, r3}
003e0220: mov      r1, r5
003e0224: mov      r0, r4
003e0228: bl       #0x3de8b4
003e022c: ldr      r3, [sp, #0x10]
003e0230: mov      r2, r7
003e0234: mov      r0, r6
003e0238: ldr      r1, [r3]
003e023c: bl       #0x3dedb4
003e0240: add      r5, r5, #1
003e0244: mov      r3, r0
003e0248: ldr      r1, [sp, #4]
003e024c: mov      r0, r6
003e0250: mov      r2, r7
003e0254: bl       #0x3df140
003e0258: cmp      r5, sl
003e025c: bne      #0x3e01e8
003e0260: ldr      r2, [sb, #0xc]
003e0264: cmp      r2, #0
003e0268: bne      #0x3e0274
003e026c: b        #0x3e0288
003e0270: mov      r2, r3
003e0274: ldr      r3, [r2, #8]
003e0278: cmp      r3, #0
003e027c: bne      #0x3e0270
003e0280: mov      sb, r2
003e0284: b        #0x3e01a8
003e0288: ldr      r3, [sb, #4]
003e028c: ldr      r1, [r3, #0xc]
003e0290: cmp      sb, r1
003e0294: bne      #0x3e02b0
003e0298: mov      sb, r3
003e029c: ldr      r3, [r3, #4]
003e02a0: ldr      r2, [r3, #0xc]
003e02a4: cmp      sb, r2
003e02a8: beq      #0x3e0298
003e02ac: ldr      r2, [sb, #0xc]
003e02b0: cmp      r3, r2
003e02b4: movne    sb, r3
003e02b8: b        #0x3e01a8
003e02bc: add      r4, r6, #8
003e02c0: mov      r1, r4
003e02c4: mov      r0, r6
003e02c8: mov      r2, r7
003e02cc: bl       #0x3df114
003e02d0: cmp      r0, #0
003e02d4: movne    r1, r4
003e02d8: bne      #0x3e043c
003e02dc: add      r4, r6, #0x38c
003e02e0: mov      r0, r6
003e02e4: mov      r1, r4
003e02e8: mov      r2, r7
003e02ec: bl       #0x3df114
003e02f0: cmp      r0, #0
003e02f4: bne      #0x3e05a8
003e02f8: add      r4, r6, #0x710
003e02fc: mov      r0, r6
003e0300: mov      r1, r4
003e0304: mov      r2, r7
003e0308: bl       #0x3df114
003e030c: cmp      r0, #0
003e0310: bne      #0x3e05f8
003e0314: add      lr, r6, #0xe10
003e0318: add      r2, r6, #0xa90
003e031c: add      lr, lr, #8
003e0320: add      r2, r2, #4
003e0324: str      lr, [sp, #0xc]
003e0328: str      r2, [sp, #4]
003e032c: add      r3, sp, #0x30
003e0330: ldr      sl, [r6, #0xe20]
003e0334: add      r4, sp, #0x10
003e0338: str      r3, [sp, #8]
003e033c: mov      r8, r6
003e0340: ldr      lr, [sp, #0xc]
003e0344: cmp      lr, sl
003e0348: beq      #0x3e05e4
003e034c: ldr      ip, [sp, #8]
003e0350: add      r5, sl, #0x34
003e0354: ldm      r5, {r0, r1, r2, r3}
003e0358: stm      ip, {r0, r1, r2, r3}
003e035c: add      r0, sl, #0x44
003e0360: ldr      r1, [sp, #8]
003e0364: bl       #0x3de870
003e0368: subs     sb, r0, #0
003e036c: beq      #0x3e0410
003e0370: mov      r6, #0
003e0374: mov      fp, r6
003e0378: ldm      r5, {r0, r1, r2, r3}
003e037c: stm      r4, {r0, r1, r2, r3}
003e0380: mov      r1, r6
003e0384: mov      r0, r4
003e0388: bl       #0x3de8b4
003e038c: ldm      r5, {r0, r1, r2, r3}
003e0390: stm      r4, {r0, r1, r2, r3}
003e0394: mov      r1, r6
003e0398: mov      r0, r4
003e039c: bl       #0x3de8b4
003e03a0: ldr      r3, [sp, #0x10]
003e03a4: mov      r2, r7
003e03a8: mov      r0, r8
003e03ac: ldr      r1, [r3]
003e03b0: bl       #0x3df114
003e03b4: cmp      r0, #0
003e03b8: beq      #0x3e03fc
003e03bc: ldm      r5, {r0, r1, r2, r3}
003e03c0: stm      r4, {r0, r1, r2, r3}
003e03c4: mov      r1, r6
003e03c8: mov      r0, r4
003e03cc: bl       #0x3de8b4
003e03d0: ldr      r3, [sp, #0x10]
003e03d4: mov      r2, r7
003e03d8: mov      r0, r8
003e03dc: ldr      r1, [r3]
003e03e0: bl       #0x3dedb4
003e03e4: ldr      r1, [sp, #4]
003e03e8: mov      r3, r0
003e03ec: mov      r2, r7
003e03f0: mov      r0, r8
003e03f4: bl       #0x3deca0
003e03f8: mov      fp, #1
003e03fc: add      r6, r6, #1
003e0400: cmp      r6, sb
003e0404: bne      #0x3e0378
003e0408: cmp      fp, #0
003e040c: bne      #0x3e0474
003e0410: ldr      r2, [sl, #0xc]
003e0414: cmp      r2, #0
003e0418: beq      #0x3e04a4
003e041c: mov      sl, r2
003e0420: b        #0x3e0428
003e0424: mov      sl, r3
003e0428: ldr      r3, [sl, #8]
003e042c: cmp      r3, #0
003e0430: bne      #0x3e0424
003e0434: b        #0x3e0340
003e0438: add      r1, r6, #8
003e043c: mov      r2, r7
003e0440: add      lr, r6, #0xa90
003e0444: mov      r0, r6
003e0448: str      lr, [sp, #4]
003e044c: bl       #0x3dedb4
003e0450: ldr      r2, [sp, #4]
003e0454: mov      r3, r0
003e0458: add      r2, r2, #4
003e045c: str      r2, [sp, #4]
003e0460: mov      r1, r2
003e0464: mov      r0, r6
003e0468: mov      r2, r7
003e046c: bl       #0x3deca0
003e0470: b        #0x3e009c
003e0474: mov      r6, r8
003e0478: b        #0x3e009c
003e047c: mov      r1, r4
003e0480: mov      r0, r6
003e0484: mov      r2, r7
003e0488: bl       #0x3dedb4
003e048c: mov      r3, r0
003e0490: ldr      r1, [sp, #4]
003e0494: mov      r0, r6
003e0498: mov      r2, r7
003e049c: bl       #0x3deca0
003e04a0: b        #0x3e009c
003e04a4: ldr      r3, [sl, #4]
003e04a8: ldr      r1, [r3, #0xc]
003e04ac: cmp      sl, r1
003e04b0: bne      #0x3e04cc
003e04b4: mov      sl, r3
003e04b8: ldr      r3, [r3, #4]
003e04bc: ldr      r2, [r3, #0xc]
003e04c0: cmp      r2, sl
003e04c4: beq      #0x3e04b4
003e04c8: ldr      r2, [sl, #0xc]
003e04cc: cmp      r3, r2
003e04d0: movne    sl, r3
003e04d4: b        #0x3e0340
003e04d8: mov      r1, r4
003e04dc: mov      r2, r7
003e04e0: mov      r0, r6
003e04e4: bl       #0x3dedb4
003e04e8: ldr      r1, [sp, #4]
003e04ec: mov      r3, r0
003e04f0: mov      r2, r7
003e04f4: mov      r0, r6
003e04f8: bl       #0x3df140
003e04fc: b        #0x3e0190
003e0500: mov      r1, r4
003e0504: mov      r2, r7
003e0508: mov      r0, r6
003e050c: bl       #0x3dedb4
003e0510: ldr      r1, [sp, #4]
003e0514: mov      r3, r0
003e0518: mov      r2, r7
003e051c: mov      r0, r6
003e0520: bl       #0x3df140
003e0524: b        #0x3e0174
003e0528: mov      r1, r4
003e052c: mov      r2, r7
003e0530: mov      r0, r6
003e0534: bl       #0x3dedb4
003e0538: ldr      r1, [sp, #4]
003e053c: mov      r3, r0
003e0540: mov      r2, r7
003e0544: mov      r0, r6
003e0548: bl       #0x3df140
003e054c: b        #0x3e0158
003e0550: add      r3, r6, #0xa90
003e0554: add      r3, r3, #4
003e0558: add      r1, r6, #8
003e055c: mov      r2, r7
003e0560: mov      r0, r6
003e0564: str      r3, [sp, #4]
003e0568: bl       #0x3dedb4
003e056c: ldr      r1, [sp, #4]
003e0570: mov      r3, r0
003e0574: mov      r2, r7
003e0578: mov      r0, r6
003e057c: bl       #0x3deca0
003e0580: add      r1, r6, #0x38c
003e0584: mov      r2, r7
003e0588: mov      r0, r6
003e058c: bl       #0x3dedb4
003e0590: ldr      r1, [sp, #4]
003e0594: mov      r3, r0
003e0598: mov      r2, r7
003e059c: mov      r0, r6
003e05a0: bl       #0x3df140
003e05a4: b        #0x3e009c
003e05a8: add      r3, r6, #0xa90
003e05ac: mov      r1, r4
003e05b0: mov      r2, r7
003e05b4: mov      r0, r6
003e05b8: str      r3, [sp, #4]
003e05bc: bl       #0x3dedb4
003e05c0: ldr      ip, [sp, #4]
003e05c4: mov      r3, r0
003e05c8: mov      r2, r7
003e05cc: add      ip, ip, #4
003e05d0: mov      r0, r6
003e05d4: mov      r1, ip
003e05d8: str      ip, [sp, #4]
003e05dc: bl       #0x3deca0
003e05e0: b        #0x3e009c
003e05e4: mov      r0, r8
003e05e8: mov      r1, r7
003e05ec: mov      r6, r8
003e05f0: bl       #0x3def10
003e05f4: b        #0x3e048c
003e05f8: mov      r2, r7
003e05fc: mov      r1, r4
003e0600: mov      r0, r6
003e0604: bl       #0x3dedb4
003e0608: add      r2, r6, #0xa90
003e060c: mov      r3, r0
003e0610: b        #0x3e0458
