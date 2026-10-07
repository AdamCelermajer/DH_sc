
# _ZN14PlayerSavegame5_LoadEi
00464f4c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00464f50: ldr      r5, [pc, #0x40c]
00464f54: ldr      r7, [pc, #0x40c]
00464f58: ldr      r8, [r0, #8]
00464f5c: add      r5, pc, r5
00464f60: ldr      r3, [r5, r7]
00464f64: sub      sp, sp, #0x2c
00464f68: cmp      r8, #0
00464f6c: ldr      r3, [r3]
00464f70: mov      r4, r0
00464f74: mov      r6, r1
00464f78: str      r3, [sp, #0x24]
00464f7c: beq      #0x4652f0
00464f80: tst      r6, #1
00464f84: beq      #0x46508c
00464f88: ldr      r0, [r4, #8]
00464f8c: cmp      r0, #0
00464f90: beq      #0x46508c
00464f94: ldr      r3, [pc, #0x3d0]
00464f98: ldr      r1, [pc, #0x3d0]
00464f9c: str      r4, [sp]
00464fa0: ldr      r2, [r5, r3]
00464fa4: ldr      r3, [pc, #0x3c8]
00464fa8: add      r1, pc, r1
00464fac: ldr      r3, [r5, r3]
00464fb0: bl       #0x315848
00464fb4: ldr      r3, [pc, #0x3bc]
00464fb8: ldr      r1, [pc, #0x3bc]
00464fbc: ldr      r0, [r4, #8]
00464fc0: ldr      r2, [r5, r3]
00464fc4: ldr      r3, [pc, #0x3b4]
00464fc8: add      r1, pc, r1
00464fcc: str      r4, [sp]
00464fd0: ldr      r3, [r5, r3]
00464fd4: bl       #0x315848
00464fd8: ldr      r3, [pc, #0x3a4]
00464fdc: ldr      r1, [pc, #0x3a4]
00464fe0: ldr      r0, [r4, #8]
00464fe4: ldr      r2, [r5, r3]
00464fe8: ldr      r3, [pc, #0x39c]
00464fec: add      r1, pc, r1
00464ff0: str      r4, [sp]
00464ff4: ldr      r3, [r5, r3]
00464ff8: bl       #0x315848
00464ffc: ldr      r3, [pc, #0x38c]
00465000: ldr      r1, [pc, #0x38c]
00465004: ldr      r0, [r4, #8]
00465008: ldr      r2, [r5, r3]
0046500c: ldr      r3, [pc, #0x384]
00465010: add      r1, pc, r1
00465014: str      r4, [sp]
00465018: ldr      r3, [r5, r3]
0046501c: bl       #0x315848
00465020: ldr      r3, [pc, #0x374]
00465024: ldr      r1, [pc, #0x374]
00465028: ldr      r0, [r4, #8]
0046502c: ldr      r2, [r5, r3]
00465030: ldr      r3, [pc, #0x36c]
00465034: add      r1, pc, r1
00465038: str      r4, [sp]
0046503c: ldr      r3, [r5, r3]
00465040: bl       #0x315848
00465044: ldr      r3, [pc, #0x35c]
00465048: ldr      r1, [pc, #0x35c]
0046504c: ldr      r0, [r4, #8]
00465050: ldr      r2, [r5, r3]
00465054: ldr      r3, [pc, #0x354]
00465058: add      r1, pc, r1
0046505c: str      r4, [sp]
00465060: ldr      r3, [r5, r3]
00465064: bl       #0x315848
00465068: ldr      r3, [pc, #0x344]
0046506c: ldr      r1, [pc, #0x344]
00465070: ldr      r0, [r4, #8]
00465074: ldr      r2, [r5, r3]
00465078: ldr      r3, [pc, #0x33c]
0046507c: add      r1, pc, r1
00465080: str      r4, [sp]
00465084: ldr      r3, [r5, r3]
00465088: bl       #0x315848
0046508c: tst      r6, #2
00465090: bne      #0x4652c4
00465094: tst      r6, #4
00465098: beq      #0x4651f8
0046509c: ldr      r0, [r4, #8]
004650a0: cmp      r0, #0
004650a4: beq      #0x4651f8
004650a8: ldr      r3, [pc, #0x310]
004650ac: ldr      r1, [pc, #0x310]
004650b0: str      r4, [sp]
004650b4: ldr      r2, [r5, r3]
004650b8: ldr      r3, [pc, #0x308]
004650bc: add      r1, pc, r1
004650c0: ldr      r3, [r5, r3]
004650c4: bl       #0x315848
004650c8: ldr      r3, [pc, #0x2fc]
004650cc: ldr      r1, [pc, #0x2fc]
004650d0: ldr      r0, [r4, #8]
004650d4: ldr      r2, [r5, r3]
004650d8: ldr      r3, [pc, #0x2f4]
004650dc: add      r1, pc, r1
004650e0: str      r4, [sp]
004650e4: ldr      r3, [r5, r3]
004650e8: bl       #0x315848
004650ec: ldr      r3, [pc, #0x2e4]
004650f0: ldr      r1, [pc, #0x2e4]
004650f4: ldr      r0, [r4, #8]
004650f8: ldr      r2, [r5, r3]
004650fc: ldr      r3, [pc, #0x2dc]
00465100: add      r1, pc, r1
00465104: str      r4, [sp]
00465108: ldr      r3, [r5, r3]
0046510c: bl       #0x315848
00465110: ldr      r8, [r4, #8]
00465114: bl       #0x7fd794
00465118: ldrb     r3, [r0, #5]
0046511c: cmp      r3, #0
00465120: beq      #0x465148
00465124: ldr      r3, [pc, #0x2b8]
00465128: ldr      r3, [r5, r3]
0046512c: ldr      r3, [r3, #0x40]
00465130: ldrb     r3, [r3, #0x71b]
00465134: cmp      r3, #0
00465138: bne      #0x465148
0046513c: ldr      r3, [pc, #0x2a4]
00465140: ldr      r2, [r5, r3]
00465144: b        #0x46514c
00465148: mov      r2, #0
0046514c: ldr      r3, [pc, #0x298]
00465150: ldr      r1, [pc, #0x298]
00465154: mov      r0, r8
00465158: ldr      r3, [r5, r3]
0046515c: add      r1, pc, r1
00465160: str      r4, [sp]
00465164: bl       #0x315848
00465168: ldr      r3, [pc, #0x284]
0046516c: ldr      r1, [pc, #0x284]
00465170: ldr      r0, [r4, #8]
00465174: ldr      r2, [r5, r3]
00465178: ldr      r3, [pc, #0x27c]
0046517c: add      r1, pc, r1
00465180: str      r4, [sp]
00465184: ldr      r3, [r5, r3]
00465188: bl       #0x315848
0046518c: ldr      r3, [pc, #0x26c]
00465190: ldr      r1, [pc, #0x26c]
00465194: ldr      r0, [r4, #8]
00465198: ldr      r2, [r5, r3]
0046519c: ldr      r3, [pc, #0x264]
004651a0: add      r1, pc, r1
004651a4: str      r4, [sp]
004651a8: ldr      r3, [r5, r3]
004651ac: bl       #0x315848
004651b0: ldr      r3, [pc, #0x254]
004651b4: ldr      r1, [pc, #0x254]
004651b8: ldr      r0, [r4, #8]
004651bc: ldr      r2, [r5, r3]
004651c0: ldr      r3, [pc, #0x24c]
004651c4: add      r1, pc, r1
004651c8: str      r4, [sp]
004651cc: ldr      r3, [r5, r3]
004651d0: bl       #0x315848
004651d4: ldr      r3, [pc, #0x23c]
004651d8: ldr      r1, [pc, #0x23c]
004651dc: ldr      r0, [r4, #8]
004651e0: ldr      r2, [r5, r3]
004651e4: ldr      r3, [pc, #0x234]
004651e8: add      r1, pc, r1
004651ec: str      r4, [sp]
004651f0: ldr      r3, [r5, r3]
004651f4: bl       #0x315848
004651f8: tst      r6, #8
004651fc: beq      #0x46522c
00465200: ldr      r0, [r4, #8]
00465204: cmp      r0, #0
00465208: beq      #0x46522c
0046520c: ldr      r3, [pc, #0x1b8]
00465210: ldr      r1, [pc, #0x20c]
00465214: str      r4, [sp]
00465218: ldr      r2, [r5, r3]
0046521c: ldr      r3, [pc, #0x1b0]
00465220: add      r1, pc, r1
00465224: ldr      r3, [r5, r3]
00465228: bl       #0x315848
0046522c: tst      r6, #0x20
00465230: beq      #0x465260
00465234: ldr      r0, [r4, #8]
00465238: cmp      r0, #0
0046523c: beq      #0x465260
00465240: ldr      r3, [pc, #0x1b8]
00465244: ldr      r1, [pc, #0x1dc]
00465248: str      r4, [sp]
0046524c: ldr      r2, [r5, r3]
00465250: ldr      r3, [pc, #0x1b0]
00465254: add      r1, pc, r1
00465258: ldr      r3, [r5, r3]
0046525c: bl       #0x315848
00465260: tst      r6, #0x10
00465264: beq      #0x4652a8
00465268: ldr      r3, [r4, #8]
0046526c: cmp      r3, #0
00465270: beq      #0x4652a8
00465274: add      r0, r4, #0xb8
00465278: bl       #0x46c1a8
0046527c: add      r0, r4, #0x118
00465280: bl       #0x46c1a8
00465284: ldr      r3, [pc, #0x168]
00465288: ldr      r1, [pc, #0x19c]
0046528c: ldr      r0, [r4, #8]
00465290: ldr      r2, [r5, r3]
00465294: ldr      r3, [pc, #0x160]
00465298: add      r1, pc, r1
0046529c: str      r4, [sp]
004652a0: ldr      r3, [r5, r3]
004652a4: bl       #0x315848
004652a8: ldr      r3, [r5, r7]
004652ac: ldr      r2, [sp, #0x24]
004652b0: ldr      r3, [r3]
004652b4: cmp      r2, r3
004652b8: bne      #0x465360
004652bc: add      sp, sp, #0x2c
004652c0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004652c4: mov      r0, r4
004652c8: bl       #0x46954c
004652cc: mov      r0, r4
004652d0: bl       #0x469764
004652d4: mov      r0, r4
004652d8: bl       #0x4694c8
004652dc: add      r0, r4, #0xb8
004652e0: bl       #0x46c1a8
004652e4: add      r0, r4, #0x118
004652e8: bl       #0x46c1a8
004652ec: b        #0x465094
004652f0: ldr      r3, [r0, #4]
004652f4: cmn      r3, #1
004652f8: beq      #0x464f80
004652fc: add      sl, sp, #0xc
00465300: mov      r0, sl
00465304: mov      r1, #0x10
00465308: str      sl, [sp, #0x1c]
0046530c: str      sl, [sp, #0x20]
00465310: bl       #0x31167c
00465314: ldr      r2, [sp, #0x1c]
00465318: mov      r3, r8
0046531c: mov      r1, sl
00465320: strb     r8, [r2]
00465324: ldr      r0, [r4, #4]
00465328: mov      r2, r8
0046532c: bl       #0x463c84
00465330: mov      r1, r8
00465334: mov      r0, #0x3c
00465338: ldr      fp, [sp, #0x20]
0046533c: bl       #0x310570
00465340: mov      r1, fp
00465344: mov      sb, r0
00465348: mov      r2, r8
0046534c: bl       #0x315ed8
00465350: str      sb, [r4, #8]
00465354: mov      r0, sl
00465358: bl       #0x3139ac
0046535c: b        #0x464f80
00465360: bl       #0x30e310
00465364: subseq   pc, r2, r4, lsr fp
00465368: andeq    r4, r0, ip, lsr #1
0046536c: andeq    r1, r0, r8, lsl #10
00465370: subeq    r8, r6, r0, asr #4
00465374: strdeq   r1, r2, [r0], -r4
00465378: andeq    r1, r0, r8, ror #30
0046537c: subeq    r8, r6, r8, lsr #4
00465380: strdeq   r1, r2, [r0], -r0
00465384: strheq   r2, [r0], -ip
00465388: subeq    r8, r6, ip, lsl #4
0046538c: andeq    r2, r0, r8, asr #29
00465390: andeq    r0, r0, r8, lsr #26
00465394: strdeq   r8, sb, [r6], #-0x10
00465398: strheq   r1, [r0], -r8
0046539c: andeq    r3, r0, r4, lsr #12
004653a0: ldrdeq   r8, sb, [r6], #-0x14
004653a4: andeq    r3, r0, r8, ror r7
004653a8: andeq    r3, r0, ip, asr #26
004653ac: strheq   r8, [r6], #-0x18
004653b0: strheq   r2, [r0], -r0
004653b4: andeq    r4, r0, r0, lsr r0
004653b8: umaaleq  r8, r6, ip, r1
004653bc: andeq    r2, r0, ip, ror #12
004653c0: andeq    r3, r0, r0, lsl #30
004653c4: subeq    r8, r6, r4, ror #2
004653c8: andeq    r4, r0, ip, ror r6
004653cc: muleq    r0, r4, r8
004653d0: subeq    r8, r6, ip, asr #2
004653d4: andeq    r1, r0, r4, lsr fp
004653d8: andeq    r1, r0, ip, lsr r3
004653dc: subeq    r8, r6, r0, lsr r1
004653e0: andeq    r0, r0, r4, lsr #14
004653e4: strdeq   r3, r4, [r0], -r4
004653e8: andeq    r2, r0, ip, asr #21
004653ec: strdeq   r2, r3, [r0], -r4
004653f0: ldrdeq   r8, sb, [r6], #-0xc
004653f4: andeq    r2, r0, r8, asr #12
004653f8: subeq    r8, r6, r4, asr #1
004653fc: andeq    r3, r0, r8, lsl r4
00465400: andeq    r0, r0, r4, asr #14
00465404: subeq    r8, r6, r8, lsr #1
00465408: andeq    r4, r0, ip, ror #23
0046540c: andeq    r2, r0, ip, lsr r4
00465410: subeq    r8, r6, ip, lsl #1
00465414: andeq    r4, r0, r8, lsl #5
00465418: andeq    r0, r0, ip, lsl #29
0046541c: subeq    r8, r6, r0, ror r0
00465420: andeq    r1, r0, r0, ror #24
00465424: subeq    r8, r6, r8
00465428: strdeq   r7, r8, [r6], #-0xf4
0046542c: subeq    r7, r6, r8, lsr #31

# _ZN14PlayerSavegame11_InitSkillsEv
00469764: push     {r4, r5, r6, r7, r8, lr}
00469768: ldr      r5, [r0, #0x80]
0046976c: mov      r4, r0
00469770: cmp      r5, #0
00469774: beq      #0x46977c
00469778: pop      {r4, r5, r6, r7, r8, pc}
0046977c: ldr      r0, [r0, #0x10]
00469780: bl       #0x3bc5fc
00469784: mov      r6, r0
00469788: ldr      r0, [r0, #4]
0046978c: mov      r1, r5
00469790: str      r0, [r4, #0x84]
00469794: lsl      r0, r0, #3
00469798: bl       #0x31056c
0046979c: ldr      r3, [r4, #0x84]
004697a0: str      r0, [r4, #0x80]
004697a4: cmp      r3, #0
004697a8: beq      #0x4697e4
004697ac: mov      r1, r5
004697b0: b        #0x4697b8
004697b4: ldr      r0, [r4, #0x80]
004697b8: ldr      r2, [r6, #8]
004697bc: add      r3, r0, r5, lsl #3
004697c0: ldr      r2, [r2, r5, lsl #2]
004697c4: str      r2, [r0, r5, lsl #3]
004697c8: mov      r2, #0
004697cc: strb     r1, [r3, #6]
004697d0: strh     r2, [r3, #4]
004697d4: ldr      r3, [r4, #0x84]
004697d8: add      r5, r5, #1
004697dc: cmp      r3, r5
004697e0: bhi      #0x4697b4
004697e4: ldr      r1, [r4, #0x88]
004697e8: ldr      r0, [r4, #0x8c]
004697ec: rsb      r3, r1, r0
004697f0: asr      r3, r3, #3
004697f4: add      r2, r3, r3, lsl #2
004697f8: add      r2, r2, r2, lsl #4
004697fc: add      r2, r2, r2, lsl #8
00469800: add      r2, r2, r2, lsl #16
00469804: add      r3, r3, r2, lsl #1
00469808: cmp      r3, #0
0046980c: beq      #0x469778
00469810: mov      r6, #0
00469814: mov      r7, r6
00469818: mov      r8, r6
0046981c: b        #0x469844
00469820: rsb      r3, r1, r0
00469824: asr      r3, r3, #3
00469828: add      r2, r3, r3, lsl #2
0046982c: add      r2, r2, r2, lsl #4
00469830: add      r2, r2, r2, lsl #8
00469834: add      r2, r2, r2, lsl #16
00469838: add      r3, r3, r2, lsl #1
0046983c: cmp      r7, r3
00469840: bhs      #0x469778
00469844: add      r5, r1, r6
00469848: ldr      r3, [r5, #0x10]
0046984c: add      r7, r7, #1
00469850: add      r6, r6, #0x18
00469854: cmp      r3, #0
00469858: beq      #0x469820
0046985c: mov      r0, r5
00469860: ldr      r1, [r5, #4]
00469864: bl       #0x345c94
00469868: str      r8, [r5, #0x10]
0046986c: str      r5, [r5, #8]
00469870: str      r8, [r5, #4]
00469874: str      r5, [r5, #0xc]
00469878: ldr      r1, [r4, #0x88]
0046987c: ldr      r0, [r4, #0x8c]
00469880: b        #0x469820

# _ZN13ItemInventory26UpdateLocalizationForItemsEv
003fdfa0: push     {r4, r5, r6, lr}
003fdfa4: mov      r6, r0
003fdfa8: bl       #0x3fc608
003fdfac: subs     r5, r0, #0
003fdfb0: ble      #0x3fdfd4
003fdfb4: mov      r4, #0
003fdfb8: mov      r1, r4
003fdfbc: mov      r0, r6
003fdfc0: bl       #0x3fc61c
003fdfc4: add      r4, r4, #1
003fdfc8: bl       #0x3fc1b4
003fdfcc: cmp      r5, r4
003fdfd0: bne      #0x3fdfb8
003fdfd4: pop      {r4, r5, r6, pc}

# _ZN14PlayerSavegame17SG_SetSkillInSlotEij
004680a8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
004680ac: ldr      r3, [r0, #0x84]
004680b0: mov      r5, r2
004680b4: ldr      r7, [pc, #0x384]
004680b8: cmp      r3, r2
004680bc: movhi    r2, #0
004680c0: movls    r2, #1
004680c4: cmn      r5, #1
004680c8: moveq    r2, #0
004680cc: cmp      r2, #0
004680d0: sub      sp, sp, #0x20
004680d4: mov      r6, r0
004680d8: add      r7, pc, r7
004680dc: mov      r4, r1
004680e0: beq      #0x468108
004680e4: ldr      r3, [pc, #0x358]
004680e8: ldr      r3, [r7, r3]
004680ec: ldr      r3, [r3]
004680f0: cmp      r3, #2
004680f4: moveq    r3, #0
004680f8: streq    r3, [r3]
004680fc: beq      #0x468108
00468100: cmp      r3, #1
00468104: beq      #0x4683b8
00468108: cmp      r4, #0
0046810c: blt      #0x468360
00468110: ldr      r3, [r6, #0x80]
00468114: cmp      r3, #0
00468118: beq      #0x4683ec
0046811c: ldr      r0, [r6, #0x10]
00468120: mvn      r1, #0
00468124: add      r0, r0, #0x37c
00468128: bl       #0x3fc6a0
0046812c: mov      r1, r4
00468130: mov      r7, r0
00468134: mov      r0, r6
00468138: bl       #0x467488
0046813c: cmn      r5, #1
00468140: beq      #0x4682f8
00468144: mov      sl, #0x18
00468148: mul      sl, sl, r7
0046814c: ldr      r0, [r6, #0x88]
00468150: add      sb, sp, #0x18
00468154: add      r8, r0, sl
00468158: ldr      r3, [r8, #8]
0046815c: cmp      r8, r3
00468160: beq      #0x46819c
00468164: ldr      r2, [r3, #0x14]
00468168: cmp      r5, r2
0046816c: beq      #0x468214
00468170: ldr      r2, [r3, #0xc]
00468174: cmp      r2, #0
00468178: bne      #0x468184
0046817c: b        #0x468250
00468180: mov      r2, r3
00468184: ldr      r3, [r2, #8]
00468188: cmp      r3, #0
0046818c: bne      #0x468180
00468190: mov      r3, r2
00468194: cmp      r8, r3
00468198: bne      #0x468164
0046819c: add      r1, r0, sl
004681a0: ldr      ip, [r1, #4]
004681a4: cmp      ip, #0
004681a8: moveq    ip, r1
004681ac: beq      #0x4681dc
004681b0: mov      r2, r1
004681b4: b        #0x4681bc
004681b8: mov      ip, r3
004681bc: ldr      r3, [ip, #0x10]
004681c0: cmp      r4, r3
004681c4: ldrgt    r3, [ip, #0xc]
004681c8: ldrle    r3, [ip, #8]
004681cc: movgt    ip, r2
004681d0: mov      r2, ip
004681d4: cmp      r3, #0
004681d8: bne      #0x4681b8
004681dc: cmp      r1, ip
004681e0: beq      #0x468284
004681e4: ldr      r2, [ip, #0x10]
004681e8: mov      r3, ip
004681ec: cmp      r4, r2
004681f0: blt      #0x468284
004681f4: str      r5, [r3, #0x14]
004681f8: ldr      r0, [r6, #0x10]
004681fc: cmp      r0, #0
00468200: beq      #0x46820c
00468204: add      r0, r0, #0x3c8
00468208: bl       #0x3d8a04
0046820c: add      sp, sp, #0x20
00468210: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00468214: ldr      r7, [r3, #0xc]
00468218: add      r0, r0, sl
0046821c: cmp      r7, #0
00468220: bne      #0x46822c
00468224: b        #0x4682ac
00468228: mov      r7, r2
0046822c: ldr      r2, [r7, #8]
00468230: cmp      r2, #0
00468234: bne      #0x468228
00468238: mov      r1, sb
0046823c: str      r3, [sp, #0x18]
00468240: bl       #0x467c18
00468244: mov      r3, r7
00468248: ldr      r0, [r6, #0x88]
0046824c: b        #0x46815c
00468250: ldr      r1, [r3, #4]
00468254: ldr      ip, [r1, #0xc]
00468258: cmp      r3, ip
0046825c: bne      #0x468278
00468260: mov      r3, r1
00468264: ldr      r1, [r1, #4]
00468268: ldr      r2, [r1, #0xc]
0046826c: cmp      r2, r3
00468270: beq      #0x468260
00468274: ldr      r2, [r3, #0xc]
00468278: cmp      r2, r1
0046827c: movne    r3, r1
00468280: b        #0x46815c
00468284: add      r3, sp, #8
00468288: mov      lr, #0
0046828c: add      r0, sp, #0x10
00468290: add      r2, sp, #0x14
00468294: str      r4, [sp, #8]
00468298: str      lr, [sp, #0xc]
0046829c: str      ip, [sp, #0x14]
004682a0: bl       #0x342bbc
004682a4: ldr      r3, [sp, #0x10]
004682a8: b        #0x4681f4
004682ac: ldr      r2, [r3, #4]
004682b0: ldr      r1, [r2, #0xc]
004682b4: cmp      r3, r1
004682b8: movne    r7, r3
004682bc: bne      #0x4682d4
004682c0: mov      r7, r2
004682c4: ldr      r2, [r2, #4]
004682c8: ldr      r1, [r2, #0xc]
004682cc: cmp      r1, r7
004682d0: beq      #0x4682c0
004682d4: ldr      r1, [r7, #0xc]
004682d8: str      r3, [sp, #0x18]
004682dc: cmp      r2, r1
004682e0: movne    r7, r2
004682e4: mov      r1, sb
004682e8: bl       #0x467c18
004682ec: mov      r3, r7
004682f0: ldr      r0, [r6, #0x88]
004682f4: b        #0x46815c
004682f8: ldr      r3, [r6, #0x88]
004682fc: mov      r0, #0x18
00468300: mla      r0, r0, r7, r3
00468304: ldr      r3, [r0, #4]
00468308: cmp      r3, #0
0046830c: beq      #0x46820c
00468310: mov      r1, r0
00468314: b        #0x46831c
00468318: mov      r3, r2
0046831c: ldr      r2, [r3, #0x10]
00468320: cmp      r4, r2
00468324: ldrgt    r2, [r3, #0xc]
00468328: ldrle    r2, [r3, #8]
0046832c: movgt    r3, r1
00468330: mov      r1, r3
00468334: cmp      r2, #0
00468338: bne      #0x468318
0046833c: cmp      r0, r3
00468340: beq      #0x46820c
00468344: ldr      r2, [r3, #0x10]
00468348: cmp      r4, r2
0046834c: blt      #0x46820c
00468350: add      r1, sp, #0x20
00468354: str      r3, [r1, #-4]!
00468358: bl       #0x467c18
0046835c: b        #0x46820c
00468360: ldr      r3, [pc, #0xdc]
00468364: ldr      r3, [r7, r3]
00468368: ldr      r3, [r3]
0046836c: cmp      r3, #2
00468370: moveq    r3, #0
00468374: streq    r3, [r3]
00468378: beq      #0x468110
0046837c: cmp      r3, #1
00468380: bne      #0x468110
00468384: ldr      r0, [pc, #0xbc]
00468388: ldr      r1, [pc, #0xbc]
0046838c: ldr      r2, [pc, #0xbc]
00468390: ldr      r0, [r7, r0]
00468394: ldr      r3, [pc, #0xb8]
00468398: mov      ip, #0xe2
0046839c: add      r1, pc, r1
004683a0: add      r2, pc, r2
004683a4: add      r3, pc, r3
004683a8: add      r0, r0, #0xa8
004683ac: str      ip, [sp]
004683b0: bl       #0x30e004
004683b4: b        #0x468110
004683b8: ldr      r0, [pc, #0x88]
004683bc: ldr      r1, [pc, #0x94]
004683c0: ldr      r2, [pc, #0x94]
004683c4: ldr      r0, [r7, r0]
004683c8: ldr      r3, [pc, #0x90]
004683cc: mov      ip, #0xe1
004683d0: add      r1, pc, r1
004683d4: add      r2, pc, r2
004683d8: add      r3, pc, r3
004683dc: add      r0, r0, #0xa8
004683e0: str      ip, [sp]
004683e4: bl       #0x30e004
004683e8: b        #0x468108
004683ec: ldr      r2, [pc, #0x50]
004683f0: ldr      r2, [r7, r2]
004683f4: ldr      r2, [r2]
004683f8: cmp      r2, #2
004683fc: streq    r3, [r3]
00468400: beq      #0x46811c
00468404: cmp      r2, #1
00468408: bne      #0x46811c
0046840c: ldr      r0, [pc, #0x34]
00468410: ldr      r1, [pc, #0x4c]
00468414: ldr      r2, [pc, #0x4c]
00468418: ldr      r0, [r7, r0]
0046841c: ldr      r3, [pc, #0x48]
00468420: mov      ip, #0xe3
00468424: add      r1, pc, r1
00468428: add      r2, pc, r2
0046842c: add      r3, pc, r3
00468430: add      r0, r0, #0xa8
00468434: str      ip, [sp]
00468438: bl       #0x30e004
0046843c: b        #0x46811c
00468440: ldrheq   ip, [r2], #-0x98
00468444: andeq    r3, r0, r0, asr #19
00468448: andeq    r1, r0, r0, asr #19
0046844c: subeq    r6, r5, ip, lsr r0
00468450: subeq    r5, r6, r8, ror #1
00468454: ldrdeq   r4, r5, [r6], #-0xec
00468458: subeq    r6, r5, r8
0046845c: subeq    r5, r6, ip, ror r0
00468460: subeq    r4, r6, r8, lsr #29
00468464: strheq   r5, [r5], #-0xf4
00468468: subeq    r4, r6, r0, asr #29
0046846c: subeq    r4, r6, r4, asr lr

# _ZN12ItemInstanceC2Eij
003fc494: push     {r4, r5, r6, r7, r8, lr}
003fc498: ldr      r5, [pc, #0x148]
003fc49c: ldr      r3, [pc, #0x148]
003fc4a0: mov      r4, r0
003fc4a4: add      r5, pc, r5
003fc4a8: ldr      r3, [r5, r3]
003fc4ac: mov      r6, r1
003fc4b0: add      r1, r0, #8
003fc4b4: add      r3, r3, #8
003fc4b8: str      r3, [r0]
003fc4bc: sub      sp, sp, #8
003fc4c0: mov      r0, r1
003fc4c4: str      r1, [r4, #0x18]
003fc4c8: str      r1, [r4, #0x1c]
003fc4cc: str      r6, [r4, #4]
003fc4d0: mov      r1, #0x10
003fc4d4: mov      r8, r2
003fc4d8: bl       #0x31167c
003fc4dc: ldr      r2, [r4, #0x18]
003fc4e0: mov      r7, #0
003fc4e4: add      r3, r4, #0x20
003fc4e8: strb     r7, [r2]
003fc4ec: mov      r0, r3
003fc4f0: str      r3, [r4, #0x30]
003fc4f4: str      r3, [r4, #0x34]
003fc4f8: mov      r1, #0x10
003fc4fc: bl       #0x31167c
003fc500: ldr      r2, [r4, #0x30]
003fc504: add      r3, r4, #0x38
003fc508: mov      r0, r3
003fc50c: strb     r7, [r2]
003fc510: mov      r1, #0x10
003fc514: str      r3, [r4, #0x48]
003fc518: str      r3, [r4, #0x4c]
003fc51c: bl       #0x31167c
003fc520: ldr      r3, [r4, #0x48]
003fc524: cmp      r6, r7
003fc528: strb     r7, [r3]
003fc52c: mov      r3, #1
003fc530: strb     r3, [r4, #0x68]
003fc534: mvn      r3, #0
003fc538: strh     r8, [r4, #0x50]
003fc53c: strb     r7, [r4, #0x69]
003fc540: str      r7, [r4, #0x54]
003fc544: strh     r3, [r4, #0x58]
003fc548: str      r7, [r4, #0x5c]
003fc54c: str      r7, [r4, #0x60]
003fc550: str      r7, [r4, #0x64]
003fc554: blt      #0x3fc56c
003fc558: ldr      r3, [pc, #0x90]
003fc55c: ldr      r3, [r5, r3]
003fc560: ldr      r3, [r3]
003fc564: cmp      r3, r7
003fc568: bne      #0x3fc590
003fc56c: ldr      r3, [pc, #0x80]
003fc570: ldr      r3, [r5, r3]
003fc574: ldr      r3, [r3]
003fc578: cmp      r3, #2
003fc57c: moveq    r3, #0
003fc580: streq    r3, [r3]
003fc584: beq      #0x3fc590
003fc588: cmp      r3, #1
003fc58c: beq      #0x3fc5b4
003fc590: mov      r0, r4
003fc594: bl       #0x3fb754
003fc598: mov      r0, r4
003fc59c: bl       #0x3fb290
003fc5a0: mov      r0, r4
003fc5a4: bl       #0x3facdc
003fc5a8: mov      r0, r4
003fc5ac: add      sp, sp, #8
003fc5b0: pop      {r4, r5, r6, r7, r8, pc}
003fc5b4: ldr      r0, [pc, #0x3c]
003fc5b8: ldr      r1, [pc, #0x3c]
003fc5bc: ldr      r2, [pc, #0x3c]
003fc5c0: ldr      r0, [r5, r0]
003fc5c4: ldr      r3, [pc, #0x38]
003fc5c8: mov      ip, #0x54
003fc5cc: add      r1, pc, r1
003fc5d0: add      r2, pc, r2
003fc5d4: add      r3, pc, r3
003fc5d8: add      r0, r0, #0xa8
003fc5dc: str      ip, [sp]
003fc5e0: bl       #0x30e004
003fc5e4: b        #0x3fc590
003fc5e8: subseq   r8, sb, ip, ror #11
003fc5ec: andeq    r4, r0, r0, lsl #16
003fc5f0: andeq    r0, r0, r0, ror #26
003fc5f4: andeq    r3, r0, r0, asr #19
003fc5f8: andeq    r1, r0, r0, asr #19
003fc5fc: subeq    r1, ip, ip, lsl #28
003fc600: subeq    sl, ip, r8, ror #24
003fc604: subeq    sl, ip, r4, ror sb

# _ZN14PlayerSavegameC1Ejib
004655ac: ldr      r3, [pc, #0x100]
004655b0: push     {r4, r5, r6, lr}
004655b4: ldr      lr, [pc, #0xfc]
004655b8: add      r3, pc, r3
004655bc: mov      r4, r0
004655c0: ldr      lr, [r3, lr]
004655c4: mov      r5, #0
004655c8: add      ip, r0, #0x18
004655cc: add      lr, lr, #8
004655d0: str      lr, [r0]
004655d4: str      r1, [r0, #4]
004655d8: mov      r0, ip
004655dc: str      ip, [r4, #0x28]
004655e0: str      ip, [r4, #0x2c]
004655e4: mov      r1, #0x10
004655e8: str      r5, [r4, #8]
004655ec: strb     r5, [r4, #0xc]
004655f0: str      r5, [r4, #0x10]
004655f4: strb     r5, [r4, #0x14]
004655f8: mov      r6, r2
004655fc: bl       #0x31167c
00465600: ldr      r3, [r4, #0x28]
00465604: add      r0, r4, #0xb8
00465608: strb     r5, [r3]
0046560c: mov      r3, #1
00465610: str      r3, [r4, #0x30]
00465614: mvn      r3, #0
00465618: str      r3, [r4, #0x34]
0046561c: str      r5, [r4, #0x3c]
00465620: str      r5, [r4, #0x80]
00465624: str      r5, [r4, #0x84]
00465628: str      r5, [r4, #0x88]
0046562c: str      r5, [r4, #0x8c]
00465630: str      r5, [r4, #0x90]
00465634: bl       #0x46b0a4
00465638: add      r0, r4, #0x118
0046563c: bl       #0x46b0a4
00465640: add      r2, r4, #0x17c
00465644: mov      r3, r5
00465648: str      r3, [r2, r5]
0046564c: add      r1, r2, r5
00465650: add      r5, r5, #8
00465654: cmp      r5, #0x18
00465658: str      r3, [r1, #4]
0046565c: bne      #0x465648
00465660: mov      r1, r3
00465664: strb     r3, [r4, #0x194]
00465668: mov      r2, r1
0046566c: mov      r3, r4
00465670: add      r1, r1, #1
00465674: cmp      r1, #3
00465678: str      r2, [r3, #0x94]
0046567c: str      r2, [r3, #0xa0]
00465680: str      r2, [r3, #0xac]
00465684: str      r2, [r3, #0x40]
00465688: str      r2, [r3, #0x68]
0046568c: str      r2, [r3, #0x74]
00465690: str      r2, [r3, #0x5c]
00465694: str      r2, [r3, #0x50]
00465698: add      r3, r3, #4
0046569c: bne      #0x465670
004656a0: mov      r0, r4
004656a4: mov      r1, r6
004656a8: bl       #0x465430
004656ac: mov      r0, r4
004656b0: pop      {r4, r5, r6, pc}
004656b4: ldrsbeq  pc, [r2], #-0x48
004656b8: strheq   r4, [r0], -ip

# _ZN14PlayerSavegame16SG_SetSkillLevelEji
004667c4: push     {r4, r5, r6, r7, lr}
004667c8: ldr      r3, [r0, #0x84]
004667cc: ldr      r4, [pc, #0xe4]
004667d0: sub      sp, sp, #0xc
004667d4: cmp      r3, r1
004667d8: mov      r5, r0
004667dc: mov      r6, r1
004667e0: add      r4, pc, r4
004667e4: mov      r7, r2
004667e8: bhi      #0x466810
004667ec: ldr      r3, [pc, #0xc8]
004667f0: ldr      r3, [r4, r3]
004667f4: ldr      r3, [r3]
004667f8: cmp      r3, #2
004667fc: moveq    r3, #0
00466800: streq    r3, [r3]
00466804: beq      #0x466810
00466808: cmp      r3, #1
0046680c: beq      #0x466884
00466810: ldr      r3, [r5, #0x80]
00466814: cmp      r3, #0
00466818: beq      #0x46682c
0046681c: add      r6, r3, r6, lsl #3
00466820: strh     r7, [r6, #4]
00466824: add      sp, sp, #0xc
00466828: pop      {r4, r5, r6, r7, pc}
0046682c: ldr      r2, [pc, #0x88]
00466830: ldr      r2, [r4, r2]
00466834: ldr      r2, [r2]
00466838: cmp      r2, #2
0046683c: streq    r3, [r3]
00466840: beq      #0x46681c
00466844: cmp      r2, #1
00466848: bne      #0x46681c
0046684c: ldr      r0, [pc, #0x6c]
00466850: ldr      r1, [pc, #0x6c]
00466854: ldr      r2, [pc, #0x6c]
00466858: ldr      r0, [r4, r0]
0046685c: ldr      r3, [pc, #0x68]
00466860: mov      ip, #0xb2
00466864: add      r1, pc, r1
00466868: add      r3, pc, r3
0046686c: add      r0, r0, #0xa8
00466870: add      r2, pc, r2
00466874: str      ip, [sp]
00466878: bl       #0x30e004
0046687c: ldr      r3, [r5, #0x80]
00466880: b        #0x46681c
00466884: ldr      r0, [pc, #0x34]
00466888: ldr      r1, [pc, #0x40]
0046688c: ldr      r2, [pc, #0x40]
00466890: ldr      r0, [r4, r0]
00466894: ldr      r3, [pc, #0x3c]
00466898: mov      ip, #0xb1
0046689c: add      r1, pc, r1
004668a0: add      r2, pc, r2
004668a4: add      r3, pc, r3
004668a8: add      r0, r0, #0xa8
004668ac: str      ip, [sp]
004668b0: bl       #0x30e004
004668b4: b        #0x466810
004668b8: ldrheq   lr, [r2], #-0x20
004668bc: andeq    r3, r0, r0, asr #19
004668c0: andeq    r1, r0, r0, asr #19
004668c4: subeq    r7, r5, r4, ror fp
004668c8: subeq    r6, r6, r8, ror sl
004668cc: subeq    r6, r6, r8, lsl sl
004668d0: subeq    r7, r5, ip, lsr fp
004668d4: subeq    r6, r6, r0, lsr sl
004668d8: ldrdeq   r6, r7, [r6], #-0x9c

# _ZN14PlayerSavegame16__LoadPlayerNameEP11IStreamBasePv
004698dc: add      r1, r1, #0x18
004698e0: b        #0x461da8

# _ZN13ItemInventory12SetPotionQtyEi
003ffc40: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003ffc44: ldr      r5, [r0, #0x24]
003ffc48: ldr      r3, [pc, #0xbc]
003ffc4c: mov      r4, r0
003ffc50: cmp      r5, #0
003ffc54: mov      r6, r1
003ffc58: add      r3, pc, r3
003ffc5c: beq      #0x3ffc80
003ffc60: cmp      r1, #0
003ffc64: bne      #0x3ffc74
003ffc68: mov      r1, r5
003ffc6c: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003ffc70: b        #0x3fe7d8
003ffc74: mov      r0, r5
003ffc78: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003ffc7c: b        #0x3fa0e4
003ffc80: ldr      r2, [pc, #0x88]
003ffc84: ldr      r2, [r3, r2]
003ffc88: ldr      r7, [r2]
003ffc8c: cmp      r7, #0
003ffc90: beq      #0x3ffd04
003ffc94: ldr      r2, [pc, #0x78]
003ffc98: ldr      sl, [pc, #0x78]
003ffc9c: ldr      r3, [r3, r2]
003ffca0: add      sl, pc, sl
003ffca4: ldr      r8, [r3]
003ffca8: b        #0x3ffcb8
003ffcac: add      r5, r5, #1
003ffcb0: cmp      r5, r7
003ffcb4: beq      #0x3ffd04
003ffcb8: ldr      r1, [r8, r5, lsl #2]
003ffcbc: mov      r0, sl
003ffcc0: bl       #0x30e31c
003ffcc4: cmp      r0, #0
003ffcc8: bne      #0x3ffcac
003ffccc: mov      r7, r5
003ffcd0: mov      r1, #0
003ffcd4: mov      r0, #0x6c
003ffcd8: bl       #0x310570
003ffcdc: mov      r1, r7
003ffce0: mov      r2, r6
003ffce4: mov      r5, r0
003ffce8: bl       #0x3fc26c
003ffcec: mov      r0, r4
003ffcf0: mov      r1, r5
003ffcf4: mov      r2, #1
003ffcf8: mov      r3, #0
003ffcfc: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003ffd00: b        #0x3ff5d4
003ffd04: mvn      r7, #0
003ffd08: b        #0x3ffcd0
003ffd0c: subseq   r4, sb, r8, lsr lr
003ffd10: andeq    r0, r0, r0, ror #26
003ffd14: andeq    r1, r0, r4, asr ip
003ffd18: subeq    r7, ip, r0, ror #15

# _ZN12ItemInstance6SetQtyEi
003fa0e4: push     {r4, r5, lr}
003fa0e8: ldr      r3, [pc, #0x74]
003fa0ec: subs     r5, r1, #0
003fa0f0: sub      sp, sp, #0xc
003fa0f4: mov      r4, r0
003fa0f8: add      r3, pc, r3
003fa0fc: blt      #0x3fa10c
003fa100: strh     r5, [r4, #0x50]
003fa104: add      sp, sp, #0xc
003fa108: pop      {r4, r5, pc}
003fa10c: ldr      r2, [pc, #0x54]
003fa110: ldr      r2, [r3, r2]
003fa114: ldr      r2, [r2]
003fa118: cmp      r2, #2
003fa11c: moveq    r3, #0
003fa120: streq    r3, [r3]
003fa124: beq      #0x3fa100
003fa128: cmp      r2, #1
003fa12c: bne      #0x3fa100
003fa130: ldr      r0, [pc, #0x34]
003fa134: ldr      r1, [pc, #0x34]
003fa138: ldr      r2, [pc, #0x34]
003fa13c: ldr      r0, [r3, r0]
003fa140: ldr      r3, [pc, #0x30]
003fa144: movw     ip, #0x223
003fa148: add      r1, pc, r1
003fa14c: add      r2, pc, r2
003fa150: add      r3, pc, r3
003fa154: add      r0, r0, #0xa8
003fa158: str      ip, [sp]
003fa15c: bl       #0x30e004
003fa160: b        #0x3fa100

# _ZN14PlayerSavegame17__LoadPlayerLevelEP11IStreamBasePv
004689a0: add      r1, r1, #0x30
004689a4: b        #0x38b758

# _ZNK14PlayerSavegame16SG_GetSkillLevelEj
004668dc: push     {r4, r5, r6, lr}
004668e0: ldr      r3, [r0, #0x84]
004668e4: ldr      r4, [pc, #0xf8]
004668e8: sub      sp, sp, #8
004668ec: cmp      r3, r1
004668f0: mov      r5, r0
004668f4: mov      r6, r1
004668f8: add      r4, pc, r4
004668fc: bhi      #0x466924
00466900: ldr      r3, [pc, #0xe0]
00466904: ldr      r3, [r4, r3]
00466908: ldr      r3, [r3]
0046690c: cmp      r3, #2
00466910: moveq    r3, #0
00466914: streq    r3, [r3]
00466918: beq      #0x466924
0046691c: cmp      r3, #1
00466920: beq      #0x46696c
00466924: ldr      r3, [r5, #0x80]
00466928: cmp      r3, #0
0046692c: beq      #0x466940
00466930: add      r6, r3, r6, lsl #3
00466934: ldrh     r0, [r6, #4]
00466938: add      sp, sp, #8
0046693c: pop      {r4, r5, r6, pc}
00466940: ldr      r2, [pc, #0xa0]
00466944: ldr      r2, [r4, r2]
00466948: ldr      r2, [r2]
0046694c: cmp      r2, #2
00466950: streq    r3, [r3]
00466954: mvneq    r0, #0
00466958: beq      #0x466938
0046695c: cmp      r2, #1
00466960: beq      #0x4669a0
00466964: mvn      r0, #0
00466968: b        #0x466938
0046696c: ldr      r0, [pc, #0x78]
00466970: ldr      r1, [pc, #0x78]
00466974: ldr      r2, [pc, #0x78]
00466978: ldr      r0, [r4, r0]
0046697c: ldr      r3, [pc, #0x74]
00466980: mov      ip, #0xa5
00466984: add      r1, pc, r1
00466988: add      r2, pc, r2
0046698c: add      r3, pc, r3
00466990: add      r0, r0, #0xa8
00466994: str      ip, [sp]
00466998: bl       #0x30e004
0046699c: b        #0x466924
004669a0: ldr      r0, [pc, #0x44]
004669a4: ldr      r1, [pc, #0x50]
004669a8: ldr      r2, [pc, #0x50]
004669ac: ldr      r0, [r4, r0]
004669b0: ldr      r3, [pc, #0x4c]
004669b4: mov      ip, #0xa6
004669b8: add      r1, pc, r1
004669bc: add      r3, pc, r3
004669c0: add      r0, r0, #0xa8
004669c4: add      r2, pc, r2
004669c8: str      ip, [sp]
004669cc: bl       #0x30e004
004669d0: ldr      r3, [r5, #0x80]
004669d4: cmp      r3, #0
004669d8: bne      #0x466930
004669dc: mvn      r0, #0
004669e0: b        #0x466938

# _ZN14PlayerSavegame15__LoadInventoryEP11IStreamBasePv
0046a3a0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046a3a4: ldr      r4, [pc, #0x3c8]
0046a3a8: ldr      r2, [pc, #0x3c8]
0046a3ac: sub      sp, sp, #0x84
0046a3b0: add      r4, pc, r4
0046a3b4: str      r2, [sp, #0x3c]
0046a3b8: ldr      r2, [r4, r2]
0046a3bc: str      r1, [sp, #0x14]
0046a3c0: ldr      r3, [r1, #0x10]
0046a3c4: ldr      r2, [r2]
0046a3c8: mov      r5, r0
0046a3cc: cmp      r3, #0
0046a3d0: str      r2, [sp, #0x7c]
0046a3d4: beq      #0x46a70c
0046a3d8: add      r1, sp, #0x64
0046a3dc: mov      r0, r1
0046a3e0: str      r1, [sp, #8]
0046a3e4: mov      r1, #0x10
0046a3e8: str      r0, [sp, #0x74]
0046a3ec: str      r0, [sp, #0x78]
0046a3f0: bl       #0x31167c
0046a3f4: ldr      r3, [sp, #0x74]
0046a3f8: mov      r6, #0
0046a3fc: add      r1, sp, #0x5c
0046a400: strb     r6, [r3]
0046a404: mov      r0, r5
0046a408: bl       #0x313b48
0046a40c: mov      r0, r5
0046a410: add      r1, sp, #0x58
0046a414: bl       #0x313b48
0046a418: mov      r0, r5
0046a41c: add      r1, sp, #0x54
0046a420: bl       #0x313b48
0046a424: ldr      r3, [sp, #0x14]
0046a428: ldr      r1, [sp, #0x5c]
0046a42c: ldr      r0, [r3, #0x10]
0046a430: add      r0, r0, #0x37c
0046a434: bl       #0x3fdfd8
0046a438: ldr      r0, [sp, #0x14]
0046a43c: ldr      r2, [sp, #0x58]
0046a440: ldr      r3, [r0, #0x10]
0046a444: strb     r2, [r3, #0x3aa]
0046a448: ldr      r3, [sp, #0x54]
0046a44c: cmp      r3, r6
0046a450: beq      #0x46a6d4
0046a454: ldr      r1, [pc, #0x320]
0046a458: ldr      r2, [pc, #0x320]
0046a45c: ldr      r3, [pc, #0x320]
0046a460: ldr      r0, [pc, #0x320]
0046a464: str      r1, [sp, #0x20]
0046a468: str      r2, [sp, #0x38]
0046a46c: add      r1, sp, #0x50
0046a470: add      r2, sp, #0x4c
0046a474: str      r3, [sp, #0xc]
0046a478: str      r0, [sp, #0x10]
0046a47c: str      r1, [sp, #0x1c]
0046a480: str      r2, [sp, #0x34]
0046a484: add      r3, sp, #0x48
0046a488: add      r0, sp, #0x44
0046a48c: add      r1, sp, #0x63
0046a490: add      r2, sp, #0x40
0046a494: str      r6, [sp, #0x18]
0046a498: str      r3, [sp, #0x30]
0046a49c: str      r0, [sp, #0x2c]
0046a4a0: str      r1, [sp, #0x24]
0046a4a4: str      r2, [sp, #0x28]
0046a4a8: mov      r0, r5
0046a4ac: ldr      r1, [sp, #8]
0046a4b0: bl       #0x461da8
0046a4b4: ldr      r0, [sp, #0x20]
0046a4b8: ldr      sl, [sp, #0x78]
0046a4bc: ldr      r3, [r4, r0]
0046a4c0: ldr      r7, [r3]
0046a4c4: cmp      r7, #0
0046a4c8: beq      #0x46a704
0046a4cc: ldr      r1, [sp, #0x38]
0046a4d0: mov      r6, #0
0046a4d4: ldr      r3, [r4, r1]
0046a4d8: ldr      r8, [r3]
0046a4dc: b        #0x46a4ec
0046a4e0: add      r6, r6, #1
0046a4e4: cmp      r6, r7
0046a4e8: beq      #0x46a704
0046a4ec: mov      r0, sl
0046a4f0: ldr      r1, [r8, r6, lsl #2]
0046a4f4: bl       #0x30e31c
0046a4f8: cmp      r0, #0
0046a4fc: bne      #0x46a4e0
0046a500: mov      r0, r5
0046a504: ldr      r1, [sp, #0x1c]
0046a508: bl       #0x38b758
0046a50c: mov      r0, r5
0046a510: ldr      r1, [sp, #0x34]
0046a514: bl       #0x38b758
0046a518: mov      r0, r5
0046a51c: ldr      r1, [sp, #0x30]
0046a520: bl       #0x38b758
0046a524: mov      r0, r5
0046a528: ldr      r1, [sp, #0x2c]
0046a52c: bl       #0x38b758
0046a530: mov      r0, r5
0046a534: ldr      r1, [sp, #0x24]
0046a538: bl       #0x39f638
0046a53c: mov      r0, r5
0046a540: ldr      r1, [sp, #0x28]
0046a544: bl       #0x313b48
0046a548: mov      r1, #0
0046a54c: mov      r0, #0x6c
0046a550: bl       #0x310570
0046a554: mov      r1, r6
0046a558: mov      r7, r0
0046a55c: ldr      r2, [sp, #0x48]
0046a560: bl       #0x3fc26c
0046a564: mov      r0, r7
0046a568: ldr      r1, [sp, #0x44]
0046a56c: bl       #0x3fbc58
0046a570: ldrb     r3, [sp, #0x63]
0046a574: subs     r3, r3, #0
0046a578: movne    r3, #1
0046a57c: strb     r3, [r7, #0x68]
0046a580: ldr      r3, [sp, #0x40]
0046a584: cmp      r3, #0
0046a588: beq      #0x46a608
0046a58c: mov      r6, #0
0046a590: mov      r0, r5
0046a594: ldr      r1, [sp, #8]
0046a598: bl       #0x461da8
0046a59c: ldr      r2, [sp, #0xc]
0046a5a0: ldr      sb, [sp, #0x78]
0046a5a4: ldr      r3, [r4, r2]
0046a5a8: ldr      sl, [r3]
0046a5ac: cmp      sl, #0
0046a5b0: beq      #0x46a6fc
0046a5b4: ldr      r0, [sp, #0x10]
0046a5b8: mov      r8, #0
0046a5bc: ldr      r3, [r4, r0]
0046a5c0: ldr      fp, [r3]
0046a5c4: b        #0x46a5d4
0046a5c8: add      r8, r8, #1
0046a5cc: cmp      r8, sl
0046a5d0: beq      #0x46a6fc
0046a5d4: mov      r0, sb
0046a5d8: ldr      r1, [fp, r8, lsl #2]
0046a5dc: bl       #0x30e31c
0046a5e0: cmp      r0, #0
0046a5e4: bne      #0x46a5c8
0046a5e8: mov      r1, r8
0046a5ec: mov      r0, r7
0046a5f0: mvn      r2, #0
0046a5f4: bl       #0x3fbc60
0046a5f8: ldr      r3, [sp, #0x40]
0046a5fc: add      r6, r6, #1
0046a600: cmp      r3, r6
0046a604: bhi      #0x46a590
0046a608: ldr      r1, [sp, #0x14]
0046a60c: mov      r2, #1
0046a610: mov      r3, r2
0046a614: ldr      r0, [r1, #0x10]
0046a618: mov      r1, r7
0046a61c: add      r0, r0, #0x37c
0046a620: bl       #0x3ff5d4
0046a624: ldr      r3, [sp, #0x50]
0046a628: mov      r6, r0
0046a62c: cmn      r3, #1
0046a630: beq      #0x46a670
0046a634: ldr      r2, [sp, #0x14]
0046a638: mov      r3, #1
0046a63c: ldr      r1, [r2, #0x10]
0046a640: mov      r2, r0
0046a644: mov      r0, #0
0046a648: ldrb     r7, [r1, #0x3aa]
0046a64c: strb     r0, [r1, #0x3aa]
0046a650: ldr      r1, [sp, #0x14]
0046a654: ldr      r0, [r1, #0x10]
0046a658: ldr      r1, [sp, #0x50]
0046a65c: add      r0, r0, #0x37c
0046a660: bl       #0x400634
0046a664: ldr      r2, [sp, #0x14]
0046a668: ldr      r3, [r2, #0x10]
0046a66c: strb     r7, [r3, #0x3aa]
0046a670: ldr      r3, [sp, #0x4c]
0046a674: cmn      r3, #1
0046a678: beq      #0x46a6b8
0046a67c: ldr      r3, [sp, #0x14]
0046a680: mov      r0, #1
0046a684: mov      r2, r6
0046a688: ldr      r1, [r3, #0x10]
0046a68c: mov      r3, #1
0046a690: ldrb     r6, [r1, #0x3aa]
0046a694: strb     r0, [r1, #0x3aa]
0046a698: ldr      r1, [sp, #0x14]
0046a69c: ldr      r0, [r1, #0x10]
0046a6a0: ldr      r1, [sp, #0x4c]
0046a6a4: add      r0, r0, #0x37c
0046a6a8: bl       #0x400634
0046a6ac: ldr      r2, [sp, #0x14]
0046a6b0: ldr      r3, [r2, #0x10]
0046a6b4: strb     r6, [r3, #0x3aa]
0046a6b8: ldr      r3, [sp, #0x18]
0046a6bc: add      r3, r3, #1
0046a6c0: str      r3, [sp, #0x18]
0046a6c4: ldr      r0, [sp, #0x18]
0046a6c8: ldr      r3, [sp, #0x54]
0046a6cc: cmp      r3, r0
0046a6d0: bhi      #0x46a4a8
0046a6d4: ldr      r0, [sp, #8]
0046a6d8: bl       #0x3139ac
0046a6dc: ldr      r1, [sp, #0x3c]
0046a6e0: ldr      r2, [sp, #0x7c]
0046a6e4: ldr      r3, [r4, r1]
0046a6e8: ldr      r3, [r3]
0046a6ec: cmp      r2, r3
0046a6f0: bne      #0x46a770
0046a6f4: add      sp, sp, #0x84
0046a6f8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046a6fc: mvn      r1, #0
0046a700: b        #0x46a5ec
0046a704: mvn      r6, #0
0046a708: b        #0x46a500
0046a70c: ldr      r2, [pc, #0x78]
0046a710: ldr      r2, [r4, r2]
0046a714: ldr      r2, [r2]
0046a718: cmp      r2, #2
0046a71c: streq    r3, [r3]
0046a720: beq      #0x46a6dc
0046a724: cmp      r2, #1
0046a728: bne      #0x46a6dc
0046a72c: ldr      r0, [pc, #0x5c]
0046a730: ldr      r1, [pc, #0x5c]
0046a734: ldr      r2, [pc, #0x5c]
0046a738: ldr      r0, [r4, r0]
0046a73c: ldr      r3, [pc, #0x58]
0046a740: movw     ip, #0x28f
0046a744: add      r1, pc, r1
0046a748: add      r3, pc, r3
0046a74c: add      r0, r0, #0xa8
0046a750: add      r2, pc, r2
0046a754: str      ip, [sp]
0046a758: bl       #0x30e004
0046a75c: ldr      r0, [sp, #0x14]
0046a760: ldr      r3, [r0, #0x10]
0046a764: cmp      r3, #0
0046a768: beq      #0x46a6dc
0046a76c: b        #0x46a3d8
0046a770: bl       #0x30e310
0046a774: subseq   sl, r2, r0, ror #13
0046a778: andeq    r4, r0, ip, lsr #1
0046a77c: andeq    r0, r0, r0, ror #26
0046a780: andeq    r1, r0, r4, asr ip
0046a784: andeq    r1, r0, r8, lsl #3
0046a788: andeq    r1, r0, ip, asr #5
0046a78c: andeq    r3, r0, r0, asr #19
0046a790: andeq    r1, r0, r0, asr #19
0046a794: umaaleq  r3, r5, r4, ip
0046a798: subeq    r2, r6, r8, asr #26
0046a79c: subeq    r2, r6, r0, ror #26

# _ZN13ItemInventoryC1Ev
003ff200: ldr      r3, [pc, #0x120]
003ff204: ldr      r2, [pc, #0x120]
003ff208: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003ff20c: add      r3, pc, r3
003ff210: ldr      r2, [r3, r2]
003ff214: mov      r6, r0
003ff218: mov      r1, #0
003ff21c: add      r2, r2, #8
003ff220: str      r2, [r6]
003ff224: mvn      r2, #0x80000000
003ff228: sub      sp, sp, #0x10
003ff22c: add      r0, r0, #0x30
003ff230: str      r2, [r6, #0x28]
003ff234: mvn      r2, #0
003ff238: mov      r7, r1
003ff23c: strb     r2, [r6, #0x2c]
003ff240: str      r0, [r6, #0x34]
003ff244: str      r1, [r6, #4]
003ff248: str      r1, [r6, #8]
003ff24c: str      r1, [r6, #0xc]
003ff250: str      r1, [r6, #0x10]
003ff254: str      r1, [r6, #0x14]
003ff258: str      r1, [r6, #0x18]
003ff25c: str      r1, [r6, #0x1c]
003ff260: str      r1, [r6, #0x20]
003ff264: str      r1, [r6, #0x24]
003ff268: strb     r1, [r6, #0x2d]
003ff26c: strb     r1, [r6, #0x2e]
003ff270: strb     r1, [r6, #0x2f]
003ff274: str      r0, [r6, #0x30]
003ff278: add      sl, r6, #0x14
003ff27c: mov      r8, sp
003ff280: mov      r5, r1
003ff284: add      sb, sp, #0xc
003ff288: mov      r0, sl
003ff28c: mov      r1, sp
003ff290: str      r5, [sp]
003ff294: str      r5, [sp, #4]
003ff298: str      r5, [sp, #8]
003ff29c: bl       #0x3ff178
003ff2a0: ldr      r0, [sp]
003ff2a4: cmp      r0, #0
003ff2a8: beq      #0x3ff2c4
003ff2ac: ldr      r1, [sp, #8]
003ff2b0: rsb      r1, r0, r1
003ff2b4: bic      r1, r1, #3
003ff2b8: cmp      r1, #0x80
003ff2bc: bhi      #0x3ff320
003ff2c0: bl       #0x708f00
003ff2c4: mov      r4, #0
003ff2c8: ldr      r0, [r6, #0x14]
003ff2cc: str      r5, [sp, #0xc]
003ff2d0: add      r0, r0, r7
003ff2d4: ldmib    r0, {r1, r3}
003ff2d8: cmp      r1, r3
003ff2dc: beq      #0x3ff314
003ff2e0: str      r5, [r1]
003ff2e4: ldr      r3, [r0, #4]
003ff2e8: add      r3, r3, #4
003ff2ec: str      r3, [r0, #4]
003ff2f0: add      r4, r4, #1
003ff2f4: cmp      r4, #9
003ff2f8: bne      #0x3ff2c8
003ff2fc: add      r7, r7, #0xc
003ff300: cmp      r7, #0x18
003ff304: bne      #0x3ff288
003ff308: mov      r0, r6
003ff30c: add      sp, sp, #0x10
003ff310: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003ff314: mov      r2, sb
003ff318: bl       #0x3fef4c
003ff31c: b        #0x3ff2f0
003ff320: bl       #0x310440
003ff324: b        #0x3ff2c4
003ff328: subseq   r5, sb, r4, lsl #17
003ff32c: strdeq   r2, r3, [r0], -ip

# _ZN14PlayerSavegameC2Ev
00465c40: ldr      r3, [pc, #0x150]
00465c44: ldr      r1, [pc, #0x150]
00465c48: push     {r4, r5, r6, r7, r8, lr}
00465c4c: add      r3, pc, r3
00465c50: ldr      r1, [r3, r1]
00465c54: mov      r4, r0
00465c58: mov      r5, #0
00465c5c: add      r2, r0, #0x18
00465c60: add      r1, r1, #8
00465c64: mvn      r6, #0
00465c68: str      r1, [r0]
00465c6c: sub      sp, sp, #0x18
00465c70: mov      r0, r2
00465c74: str      r2, [r4, #0x28]
00465c78: str      r2, [r4, #0x2c]
00465c7c: mov      r1, #0x10
00465c80: str      r6, [r4, #4]
00465c84: str      r5, [r4, #8]
00465c88: strb     r5, [r4, #0xc]
00465c8c: str      r5, [r4, #0x10]
00465c90: strb     r5, [r4, #0x14]
00465c94: bl       #0x31167c
00465c98: ldr      r3, [r4, #0x28]
00465c9c: add      r0, r4, #0xb8
00465ca0: strb     r5, [r3]
00465ca4: str      r6, [r4, #0x34]
00465ca8: str      r5, [r4, #0x30]
00465cac: str      r5, [r4, #0x3c]
00465cb0: str      r5, [r4, #0x80]
00465cb4: str      r5, [r4, #0x84]
00465cb8: str      r5, [r4, #0x88]
00465cbc: str      r5, [r4, #0x8c]
00465cc0: str      r5, [r4, #0x90]
00465cc4: bl       #0x46b0a4
00465cc8: add      r0, r4, #0x118
00465ccc: bl       #0x46b0a4
00465cd0: mov      r3, r5
00465cd4: str      r5, [r4, #0x178]
00465cd8: add      r1, r4, #0x17c
00465cdc: mov      r2, r5
00465ce0: str      r2, [r1, r3]
00465ce4: add      r0, r1, r3
00465ce8: add      r3, r3, #8
00465cec: cmp      r3, #0x18
00465cf0: str      r2, [r0, #4]
00465cf4: bne      #0x465ce0
00465cf8: mov      r5, r2
00465cfc: strb     r2, [r4, #0x194]
00465d00: add      r8, r4, #0x88
00465d04: mov      r6, sp
00465d08: mov      r7, r2
00465d0c: mov      r0, r8
00465d10: mov      r1, sp
00465d14: str      r7, [sp, #4]
00465d18: strb     r7, [sp]
00465d1c: str      r6, [sp, #8]
00465d20: str      r6, [sp, #0xc]
00465d24: str      r7, [sp, #0x10]
00465d28: bl       #0x465a48
00465d2c: ldr      r3, [sp, #0x10]
00465d30: add      r5, r5, #1
00465d34: cmp      r3, #0
00465d38: beq      #0x465d48
00465d3c: mov      r0, sp
00465d40: ldr      r1, [sp, #4]
00465d44: bl       #0x345c94
00465d48: cmp      r5, #2
00465d4c: bne      #0x465d0c
00465d50: mov      r1, #0
00465d54: mov      r3, r4
00465d58: mov      r2, r1
00465d5c: add      r1, r1, #1
00465d60: cmp      r1, #3
00465d64: str      r2, [r3, #0x94]
00465d68: str      r2, [r3, #0xa0]
00465d6c: str      r2, [r3, #0xac]
00465d70: str      r2, [r3, #0x40]
00465d74: str      r2, [r3, #0x68]
00465d78: str      r2, [r3, #0x74]
00465d7c: str      r2, [r3, #0x5c]
00465d80: str      r2, [r3, #0x50]
00465d84: add      r3, r3, #4
00465d88: bne      #0x465d5c
00465d8c: mov      r0, r4
00465d90: add      sp, sp, #0x18
00465d94: pop      {r4, r5, r6, r7, r8, pc}
00465d98: subseq   lr, r2, r4, asr #28
00465d9c: strheq   r4, [r0], -ip

# _ZN14PlayerSavegame12__LoadSkillsEP11IStreamBasePv
0046ac74: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0046ac78: ldr      r7, [pc, #0x354]
0046ac7c: ldr      r2, [pc, #0x354]
0046ac80: sub      sp, sp, #0x64
0046ac84: add      r7, pc, r7
0046ac88: str      r2, [sp, #0x1c]
0046ac8c: ldr      r2, [r7, r2]
0046ac90: ldr      r3, [r1, #0x80]
0046ac94: mov      r6, r1
0046ac98: ldr      r2, [r2]
0046ac9c: cmp      r3, #0
0046aca0: mov      r4, r0
0046aca4: str      r2, [sp, #0x5c]
0046aca8: beq      #0x46af28
0046acac: ldr      r3, [r6, #0x10]
0046acb0: cmp      r3, #0
0046acb4: beq      #0x46af7c
0046acb8: ldr      r3, [r6, #0x80]
0046acbc: cmp      r3, #0
0046acc0: beq      #0x46aee4
0046acc4: ldr      r3, [r6, #0x10]
0046acc8: cmp      r3, #0
0046accc: beq      #0x46aee4
0046acd0: add      r3, sp, #0x44
0046acd4: mov      r0, r3
0046acd8: mov      r1, #0x10
0046acdc: str      r3, [sp, #0xc]
0046ace0: str      r3, [sp, #0x54]
0046ace4: str      r3, [sp, #0x58]
0046ace8: bl       #0x31167c
0046acec: ldr      r3, [sp, #0x54]
0046acf0: mov      fp, #0
0046acf4: mov      r0, r4
0046acf8: strb     fp, [r3]
0046acfc: add      r1, sp, #0x3c
0046ad00: bl       #0x38b758
0046ad04: ldr      r3, [sp, #0x3c]
0046ad08: cmp      r3, fp
0046ad0c: ble      #0x46add8
0046ad10: ldr      r2, [pc, #0x2c4]
0046ad14: ldr      r3, [pc, #0x2c4]
0046ad18: add      ip, sp, #0x42
0046ad1c: str      r2, [sp, #0x10]
0046ad20: str      r3, [sp, #0x14]
0046ad24: str      ip, [sp, #0x18]
0046ad28: mov      r0, r4
0046ad2c: ldr      r1, [sp, #0xc]
0046ad30: bl       #0x461da8
0046ad34: ldr      r2, [sp, #0x10]
0046ad38: ldr      sl, [sp, #0x58]
0046ad3c: ldr      r3, [r7, r2]
0046ad40: ldr      r8, [r3]
0046ad44: cmp      r8, #0
0046ad48: beq      #0x46af04
0046ad4c: ldr      ip, [sp, #0x14]
0046ad50: mov      r5, #0
0046ad54: ldr      r3, [r7, ip]
0046ad58: ldr      sb, [r3]
0046ad5c: b        #0x46ad6c
0046ad60: add      r5, r5, #1
0046ad64: cmp      r5, r8
0046ad68: beq      #0x46af04
0046ad6c: mov      r0, sl
0046ad70: ldr      r1, [sb, r5, lsl #2]
0046ad74: bl       #0x30e31c
0046ad78: cmp      r0, #0
0046ad7c: bne      #0x46ad60
0046ad80: ldr      r0, [r6, #0x84]
0046ad84: cmp      r0, #0
0046ad88: beq      #0x46adbc
0046ad8c: ldr      r2, [r6, #0x80]
0046ad90: ldr      r3, [r2]
0046ad94: cmp      r3, r5
0046ad98: movne    r3, #0
0046ad9c: bne      #0x46adb0
0046ada0: b        #0x46af0c
0046ada4: ldr      r1, [r2, #8]!
0046ada8: cmp      r1, r5
0046adac: beq      #0x46af0c
0046adb0: add      r3, r3, #1
0046adb4: cmp      r3, r0
0046adb8: bne      #0x46ada4
0046adbc: mov      r0, r4
0046adc0: ldr      r1, [sp, #0x18]
0046adc4: bl       #0x469070
0046adc8: ldr      r3, [sp, #0x3c]
0046adcc: add      fp, fp, #1
0046add0: cmp      r3, fp
0046add4: bgt      #0x46ad28
0046add8: mov      r3, #0
0046addc: mov      r8, r3
0046ade0: str      r3, [sp, #0x38]
0046ade4: str      r3, [sp, #0x34]
0046ade8: add      r2, sp, #0x38
0046adec: add      r3, sp, #0x24
0046adf0: str      r7, [sp, #0x14]
0046adf4: str      r2, [sp, #0x10]
0046adf8: add      sl, sp, #0x34
0046adfc: add      sb, sp, #0x2c
0046ae00: add      fp, sp, #0x30
0046ae04: mov      r7, r3
0046ae08: mov      r0, r4
0046ae0c: ldr      r1, [sp, #0x10]
0046ae10: bl       #0x38b758
0046ae14: ldr      r3, [sp, #0x38]
0046ae18: cmp      r3, #0
0046ae1c: ble      #0x46aecc
0046ae20: mov      r5, #0
0046ae24: mov      r1, sl
0046ae28: mov      r0, r4
0046ae2c: bl       #0x38b758
0046ae30: ldr      r1, [r6, #0x88]
0046ae34: add      r1, r1, r8
0046ae38: ldr      ip, [r1, #4]
0046ae3c: cmp      ip, #0
0046ae40: beq      #0x46af1c
0046ae44: ldr      lr, [sp, #0x34]
0046ae48: mov      r2, r1
0046ae4c: b        #0x46ae58
0046ae50: mov      r2, ip
0046ae54: mov      ip, r3
0046ae58: ldr      r3, [ip, #0x10]
0046ae5c: cmp      r3, lr
0046ae60: ldrlt    r3, [ip, #0xc]
0046ae64: ldrge    r3, [ip, #8]
0046ae68: movlt    ip, r2
0046ae6c: cmp      r3, #0
0046ae70: bne      #0x46ae50
0046ae74: cmp      r1, ip
0046ae78: beq      #0x46ae8c
0046ae7c: ldr      r2, [ip, #0x10]
0046ae80: mov      r3, ip
0046ae84: cmp      r2, lr
0046ae88: ble      #0x46aeb0
0046ae8c: mov      r3, r7
0046ae90: str      ip, [sp, #0x30]
0046ae94: mov      r0, sb
0046ae98: mov      ip, #0
0046ae9c: mov      r2, fp
0046aea0: str      lr, [sp, #0x24]
0046aea4: str      ip, [sp, #0x28]
0046aea8: bl       #0x342bbc
0046aeac: ldr      r3, [sp, #0x2c]
0046aeb0: add      r1, r3, #0x14
0046aeb4: mov      r0, r4
0046aeb8: bl       #0x38b758
0046aebc: ldr      r3, [sp, #0x38]
0046aec0: add      r5, r5, #1
0046aec4: cmp      r3, r5
0046aec8: bgt      #0x46ae24
0046aecc: add      r8, r8, #0x18
0046aed0: cmp      r8, #0x30
0046aed4: bne      #0x46ae08
0046aed8: ldr      r0, [sp, #0xc]
0046aedc: ldr      r7, [sp, #0x14]
0046aee0: bl       #0x3139ac
0046aee4: ldr      r2, [sp, #0x1c]
0046aee8: ldr      r3, [r7, r2]
0046aeec: ldr      r2, [sp, #0x5c]
0046aef0: ldr      r3, [r3]
0046aef4: cmp      r2, r3
0046aef8: bne      #0x46afd0
0046aefc: add      sp, sp, #0x64
0046af00: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0046af04: mvn      r5, #0
0046af08: b        #0x46ad80
0046af0c: add      r1, r2, #4
0046af10: mov      r0, r4
0046af14: bl       #0x469070
0046af18: b        #0x46adc8
0046af1c: ldr      lr, [sp, #0x34]
0046af20: mov      ip, r1
0046af24: b        #0x46ae74
0046af28: ldr      r2, [pc, #0xb4]
0046af2c: ldr      r2, [r7, r2]
0046af30: ldr      r2, [r2]
0046af34: cmp      r2, #2
0046af38: streq    r3, [r3]
0046af3c: beq      #0x46acac
0046af40: cmp      r2, #1
0046af44: bne      #0x46acac
0046af48: ldr      r0, [pc, #0x98]
0046af4c: ldr      r1, [pc, #0x98]
0046af50: ldr      r2, [pc, #0x98]
0046af54: ldr      r0, [r7, r0]
0046af58: ldr      r3, [pc, #0x94]
0046af5c: movw     ip, #0x192
0046af60: add      r1, pc, r1
0046af64: add      r2, pc, r2
0046af68: add      r3, pc, r3
0046af6c: add      r0, r0, #0xa8
0046af70: str      ip, [sp]
0046af74: bl       #0x30e004
0046af78: b        #0x46acac
0046af7c: ldr      r2, [pc, #0x60]
0046af80: ldr      r2, [r7, r2]
0046af84: ldr      r2, [r2]
0046af88: cmp      r2, #2
0046af8c: streq    r3, [r3]
0046af90: beq      #0x46acb8
0046af94: cmp      r2, #1
0046af98: bne      #0x46acb8
0046af9c: ldr      r0, [pc, #0x44]
0046afa0: ldr      r1, [pc, #0x50]
0046afa4: ldr      r2, [pc, #0x50]
0046afa8: ldr      r0, [r7, r0]
0046afac: ldr      r3, [pc, #0x4c]
0046afb0: movw     ip, #0x193
0046afb4: add      r1, pc, r1
0046afb8: add      r2, pc, r2
0046afbc: add      r3, pc, r3
0046afc0: add      r0, r0, #0xa8
0046afc4: str      ip, [sp]
0046afc8: bl       #0x30e004
0046afcc: b        #0x46acb8
0046afd0: bl       #0x30e310
0046afd4: subseq   sb, r2, ip, lsl #28
0046afd8: andeq    r4, r0, ip, lsr #1
0046afdc: andeq    r2, r0, r8, lsr #14
0046afe0: ldrdeq   r3, r4, [r0], -r8
0046afe4: andeq    r3, r0, r0, asr #19
0046afe8: andeq    r1, r0, r0, asr #19
0046afec: subeq    r3, r5, r8, ror r4
0046aff0: strheq   r2, [r6], #-0x54
0046aff4: subeq    r2, r6, r0, asr #10
0046aff8: subeq    r3, r5, r4, lsr #8
0046affc: subeq    r2, r6, r0, ror #9
0046b000: subeq    r2, r6, ip, ror #9

# _ZNK14PlayerSavegame17SG_GetSkillInSlotEi
00467488: push     {r4, r5, r6, lr}
0046748c: mov      r6, r0
00467490: ldr      r0, [r0, #0x10]
00467494: mov      r5, r1
00467498: mvn      r1, #0
0046749c: add      r0, r0, #0x37c
004674a0: bl       #0x3fc6a0
004674a4: ldr      r3, [r6, #0x88]
004674a8: mov      r2, #0x18
004674ac: mla      r3, r2, r0, r3
004674b0: ldr      r4, [r3, #4]
004674b4: cmp      r4, #0
004674b8: beq      #0x4674fc
004674bc: mov      ip, r3
004674c0: b        #0x4674c8
004674c4: mov      r4, r1
004674c8: ldr      r1, [r4, #0x10]
004674cc: cmp      r5, r1
004674d0: ldrgt    r1, [r4, #0xc]
004674d4: ldrle    r1, [r4, #8]
004674d8: movgt    r4, ip
004674dc: mov      ip, r4
004674e0: cmp      r1, #0
004674e4: bne      #0x4674c4
004674e8: cmp      r3, r4
004674ec: beq      #0x467500
004674f0: ldr      r2, [r4, #0x10]
004674f4: cmp      r5, r2
004674f8: bge      #0x467500
004674fc: mov      r4, r3
00467500: ldr      r0, [r6, #0x10]
00467504: mvn      r1, #0
00467508: add      r0, r0, #0x37c
0046750c: bl       #0x3fc6a0
00467510: ldr      r3, [r6, #0x88]
00467514: mov      r2, #0x18
00467518: mla      r3, r2, r0, r3
0046751c: cmp      r4, r3
00467520: mvneq    r0, #0
00467524: ldrne    r0, [r4, #0x14]
00467528: pop      {r4, r5, r6, pc}

# _ZN13ItemInventory18GetCurrentSkillSetEi
003fc6a0: mov      r0, #0
003fc6a4: bx       lr

# _ZN13ItemInventory16_AddItemInstanceEP12ItemInstancebb
003ff5d4: push     {r4, r5, r6, r7, r8, sl, lr}
003ff5d8: mov      r4, r0
003ff5dc: ldrsb    r0, [r0, #0x2c]
003ff5e0: ldr      r6, [pc, #0x260]
003ff5e4: sub      sp, sp, #0xc
003ff5e8: cmp      r0, #0
003ff5ec: add      r6, pc, r6
003ff5f0: mov      r5, r1
003ff5f4: mov      r8, r2
003ff5f8: mov      r7, r3
003ff5fc: beq      #0x3ff6f0
003ff600: ldr      r3, [r4, #0x24]
003ff604: cmp      r3, #0
003ff608: beq      #0x3ff81c
003ff60c: cmp      r7, #0
003ff610: bne      #0x3ff724
003ff614: mov      r0, r5
003ff618: bl       #0x3f9e58
003ff61c: cmp      r0, #0
003ff620: beq      #0x3ff674
003ff624: cmp      r8, #0
003ff628: bne      #0x3ff674
003ff62c: ldr      r7, [r4, #8]
003ff630: ldr      r2, [r4, #0xc]
003ff634: cmp      r7, r2
003ff638: beq      #0x3ff674
003ff63c: ldr      sl, [r7]
003ff640: mov      r1, r8
003ff644: mov      r0, r4
003ff648: ldr      r3, [sl]
003ff64c: cmp      r3, #0
003ff650: beq      #0x3ff664
003ff654: bl       #0x3fdaf0
003ff658: cmp      r0, #0
003ff65c: beq      #0x3ff75c
003ff660: ldr      r2, [r4, #0xc]
003ff664: add      r7, r7, #4
003ff668: cmp      r7, r2
003ff66c: addne    r8, r8, #1
003ff670: bne      #0x3ff63c
003ff674: mov      r1, #0
003ff678: mov      r0, #8
003ff67c: bl       #0x310570
003ff680: mvn      r3, #0
003ff684: str      r5, [r0]
003ff688: strb     r3, [r0, #5]
003ff68c: strb     r3, [r0, #4]
003ff690: ldr      r1, [r4, #0xc]
003ff694: ldr      r3, [r4, #0x10]
003ff698: str      r0, [sp, #4]
003ff69c: cmp      r1, r3
003ff6a0: beq      #0x3ff838
003ff6a4: str      r0, [r1]
003ff6a8: ldr      r3, [r4, #0xc]
003ff6ac: add      r3, r3, #4
003ff6b0: str      r3, [r4, #0xc]
003ff6b4: ldr      r3, [pc, #0x190]
003ff6b8: mov      r0, r4
003ff6bc: ldr      r3, [r6, r3]
003ff6c0: ldr      r5, [r3]
003ff6c4: bl       #0x3fe330
003ff6c8: cmp      r0, #0
003ff6cc: bne      #0x3ff7a8
003ff6d0: ldr      r3, [r4, #8]
003ff6d4: ldr      r8, [r4, #0xc]
003ff6d8: rsb      r8, r3, r8
003ff6dc: asr      r8, r8, #2
003ff6e0: sub      r8, r8, #1
003ff6e4: mov      r0, r8
003ff6e8: add      sp, sp, #0xc
003ff6ec: pop      {r4, r5, r6, r7, r8, sl, pc}
003ff6f0: mov      r0, r1
003ff6f4: bl       #0x3f9e08
003ff6f8: ldr      r3, [r0, #0x58]
003ff6fc: cmp      r3, #0xe
003ff700: bne      #0x3ff600
003ff704: cmp      r5, #0
003ff708: beq      #0x3ff754
003ff70c: mov      r0, r5
003ff710: ldr      r3, [r5]
003ff714: mov      lr, pc
003ff718: ldr      pc, [r3, #4]
003ff71c: mvn      r8, #0
003ff720: b        #0x3ff6e4
003ff724: mov      r0, r5
003ff728: bl       #0x3f9e08
003ff72c: ldr      r3, [r0, #0x58]
003ff730: cmp      r3, #0xd
003ff734: bne      #0x3ff614
003ff738: mov      r0, r4
003ff73c: ldr      r1, [r5, #0x54]
003ff740: bl       #0x3fe164
003ff744: mov      r0, r5
003ff748: ldr      r3, [r5]
003ff74c: mov      lr, pc
003ff750: ldr      pc, [r3, #4]
003ff754: mvn      r8, #0
003ff758: b        #0x3ff6e4
003ff75c: ldr      r0, [sl]
003ff760: mov      r1, r5
003ff764: bl       #0x3f9d78
003ff768: cmp      r0, #0
003ff76c: beq      #0x3ff660
003ff770: mov      r0, r5
003ff774: ldrsh    r6, [r5, #0x50]
003ff778: bl       #0x3f9e08
003ff77c: ldr      r3, [r0, #0x58]
003ff780: cmp      r3, #0xe
003ff784: beq      #0x3ff7fc
003ff788: ldr      r0, [sl]
003ff78c: mov      r1, r6
003ff790: bl       #0x3fa17c
003ff794: mov      r0, r5
003ff798: ldr      r3, [r5]
003ff79c: mov      lr, pc
003ff7a0: ldr      pc, [r3, #4]
003ff7a4: b        #0x3ff6e4
003ff7a8: ldr      r3, [r4, #4]
003ff7ac: mov      r0, r3
003ff7b0: ldr      r3, [r3]
003ff7b4: mov      lr, pc
003ff7b8: ldr      pc, [r3, #0x28]
003ff7bc: cmp      r0, #0
003ff7c0: beq      #0x3ff6d0
003ff7c4: ldr      r3, [pc, #0x84]
003ff7c8: ldr      r1, [r4, #4]
003ff7cc: ldr      r3, [r6, r3]
003ff7d0: ldr      r0, [r3, #0x40]
003ff7d4: bl       #0x36effc
003ff7d8: cmp      r0, #0
003ff7dc: beq      #0x3ff6d0
003ff7e0: ldr      r0, [pc, #0x6c]
003ff7e4: add      r0, pc, r0
003ff7e8: bl       #0x3a3f70
003ff7ec: mov      r1, r0
003ff7f0: mov      r0, r5
003ff7f4: bl       #0x3813b8
003ff7f8: b        #0x3ff6d0
003ff7fc: ldr      r0, [sl]
003ff800: ldrsb    r1, [r4, #0x2c]
003ff804: ldrsh    r3, [r0, #0x50]
003ff808: rsb      r1, r3, r1
003ff80c: cmp      r1, r6
003ff810: movge    r1, r6
003ff814: biclt    r1, r1, r1, asr #31
003ff818: b        #0x3ff790
003ff81c: mov      r0, r5
003ff820: bl       #0x3f9e08
003ff824: ldr      r3, [r0, #0x58]
003ff828: cmp      r3, #0xe
003ff82c: streq    r5, [r4, #0x24]
003ff830: bne      #0x3ff60c
003ff834: b        #0x3ff614
003ff838: add      r0, r4, #8
003ff83c: add      r2, sp, #4
003ff840: bl       #0x3fef4c
003ff844: b        #0x3ff6b4
003ff848: subseq   r5, sb, r4, lsr #9
003ff84c: andeq    r1, r0, r0, ror sp
003ff850: strdeq   r3, r4, [r0], -r4
003ff854: subeq    r7, ip, ip, ror ip

# _ZN14PlayerSavegame7SG_LoadEi
00465430: push     {r4, r5, r6, lr}
00465434: mov      r5, r0
00465438: mov      r4, r1
0046543c: bl       #0x464f4c
00465440: mov      r0, r5
00465444: mov      r1, r4
00465448: pop      {r4, r5, r6, lr}
0046544c: b        #0x468574

# _ZN14PlayerSavegame19_SetupSavedSectionsEbb
00468630: push     {r4, r5, r6, lr}
00468634: ldr      r4, [pc, #0x210]
00468638: cmp      r1, #0
0046863c: sub      sp, sp, #8
00468640: mov      r5, r0
00468644: add      r4, pc, r4
00468648: mov      r6, r2
0046864c: beq      #0x468660
00468650: cmp      r2, #0
00468654: beq      #0x468768
00468658: add      sp, sp, #8
0046865c: pop      {r4, r5, r6, pc}
00468660: cmp      r2, #0
00468664: bne      #0x468658
00468668: ldr      r3, [pc, #0x1e0]
0046866c: ldr      r1, [pc, #0x1e0]
00468670: ldr      r0, [r0, #8]
00468674: ldr      r2, [r4, r3]
00468678: ldr      r3, [pc, #0x1d8]
0046867c: add      r1, pc, r1
00468680: str      r5, [sp]
00468684: ldr      r3, [r4, r3]
00468688: bl       #0x315904
0046868c: ldr      r3, [pc, #0x1c8]
00468690: ldr      r1, [pc, #0x1c8]
00468694: ldr      r0, [r5, #8]
00468698: ldr      r2, [r4, r3]
0046869c: ldr      r3, [pc, #0x1c0]
004686a0: add      r1, pc, r1
004686a4: str      r5, [sp]
004686a8: ldr      r3, [r4, r3]
004686ac: bl       #0x315904
004686b0: ldr      r3, [pc, #0x1b0]
004686b4: ldr      r1, [pc, #0x1b0]
004686b8: ldr      r0, [r5, #8]
004686bc: ldr      r2, [r4, r3]
004686c0: ldr      r3, [pc, #0x1a8]
004686c4: add      r1, pc, r1
004686c8: str      r5, [sp]
004686cc: ldr      r3, [r4, r3]
004686d0: bl       #0x315904
004686d4: ldr      r3, [pc, #0x198]
004686d8: ldr      r1, [pc, #0x198]
004686dc: ldr      r0, [r5, #8]
004686e0: ldr      r2, [r4, r3]
004686e4: ldr      r3, [pc, #0x190]
004686e8: add      r1, pc, r1
004686ec: str      r5, [sp]
004686f0: ldr      r3, [r4, r3]
004686f4: bl       #0x315904
004686f8: ldr      r3, [pc, #0x180]
004686fc: ldr      r1, [pc, #0x180]
00468700: ldr      r0, [r5, #8]
00468704: ldr      r2, [r4, r3]
00468708: ldr      r3, [pc, #0x178]
0046870c: add      r1, pc, r1
00468710: str      r5, [sp]
00468714: ldr      r3, [r4, r3]
00468718: bl       #0x315904
0046871c: ldr      r3, [pc, #0x168]
00468720: ldr      r1, [pc, #0x168]
00468724: ldr      r0, [r5, #8]
00468728: ldr      r2, [r4, r3]
0046872c: ldr      r3, [pc, #0x160]
00468730: add      r1, pc, r1
00468734: str      r5, [sp]
00468738: ldr      r3, [r4, r3]
0046873c: bl       #0x315904
00468740: ldr      r3, [pc, #0x150]
00468744: ldr      r1, [pc, #0x150]
00468748: ldr      r0, [r5, #8]
0046874c: ldr      r2, [r4, r3]
00468750: ldr      r3, [pc, #0x148]
00468754: add      r1, pc, r1
00468758: str      r5, [sp]
0046875c: ldr      r3, [r4, r3]
00468760: bl       #0x315904
00468764: b        #0x468658
00468768: ldr      r3, [pc, #0xe0]
0046876c: ldr      r1, [pc, #0x130]
00468770: ldr      r0, [r0, #8]
00468774: ldr      r2, [r4, r3]
00468778: add      r1, pc, r1
0046877c: mov      r3, r6
00468780: str      r5, [sp]
00468784: bl       #0x315904
00468788: ldr      r3, [pc, #0xcc]
0046878c: ldr      r1, [pc, #0x114]
00468790: ldr      r0, [r5, #8]
00468794: ldr      r2, [r4, r3]
00468798: add      r1, pc, r1
0046879c: mov      r3, r6
004687a0: str      r5, [sp]
004687a4: bl       #0x315904
004687a8: ldr      r3, [pc, #0xb8]
004687ac: ldr      r1, [pc, #0xf8]
004687b0: ldr      r0, [r5, #8]
004687b4: ldr      r2, [r4, r3]
004687b8: add      r1, pc, r1
004687bc: mov      r3, r6
004687c0: str      r5, [sp]
004687c4: bl       #0x315904
004687c8: ldr      r3, [pc, #0xa4]
004687cc: ldr      r1, [pc, #0xdc]
004687d0: ldr      r0, [r5, #8]
004687d4: ldr      r2, [r4, r3]
004687d8: add      r1, pc, r1
004687dc: mov      r3, r6
004687e0: str      r5, [sp]
004687e4: bl       #0x315904
004687e8: ldr      r3, [pc, #0x90]
004687ec: ldr      r1, [pc, #0xc0]
004687f0: ldr      r0, [r5, #8]
004687f4: ldr      r2, [r4, r3]
004687f8: add      r1, pc, r1
004687fc: mov      r3, r6
00468800: str      r5, [sp]
00468804: bl       #0x315904
00468808: ldr      r3, [pc, #0x7c]
0046880c: ldr      r1, [pc, #0xa4]
00468810: ldr      r0, [r5, #8]
00468814: ldr      r2, [r4, r3]
00468818: add      r1, pc, r1
0046881c: mov      r3, r6
00468820: str      r5, [sp]
00468824: bl       #0x315904
00468828: ldr      r3, [pc, #0x68]
0046882c: ldr      r1, [pc, #0x88]
00468830: ldr      r0, [r5, #8]
00468834: ldr      r2, [r4, r3]
00468838: add      r1, pc, r1
0046883c: mov      r3, r6
00468840: str      r5, [sp]
00468844: bl       #0x315904
00468848: b        #0x468658
0046884c: subseq   ip, r2, ip, asr #8
00468850: andeq    r0, r0, r8, lsr #26
00468854: subeq    r4, r6, r4, lsl #23
00468858: strheq   r1, [r0], -r8
0046885c: andeq    r3, r0, r4, lsr #12
00468860: subeq    r4, r6, r8, ror #22
00468864: andeq    r3, r0, r8, ror r7
00468868: andeq    r3, r0, ip, asr #26
0046886c: subeq    r4, r6, ip, asr #22
00468870: strheq   r2, [r0], -r0
00468874: andeq    r4, r0, r0, lsr r0
00468878: subeq    r4, r6, r0, lsr fp
0046887c: andeq    r2, r0, ip, ror #12
00468880: andeq    r3, r0, r0, lsl #30
00468884: subeq    r4, r6, r4, lsl fp
00468888: andeq    r4, r0, ip, ror r6
0046888c: andeq    r1, r0, ip, lsr r3
00468890: subeq    r4, r6, r0, lsl #22
00468894: andeq    r0, r0, r4, lsr #14
00468898: andeq    r0, r0, ip, lsl #29
0046889c: subeq    r4, r6, r4, lsl #22
004688a0: andeq    r1, r0, r0, ror #24
004688a4: subeq    r4, r6, r8, lsl #21
004688a8: subeq    r4, r6, r0, ror sl
004688ac: subeq    r4, r6, r8, asr sl
004688b0: subeq    r4, r6, r0, asr #20
004688b4: subeq    r4, r6, r8, lsr #20
004688b8: subeq    r4, r6, r8, lsl sl
004688bc: subeq    r4, r6, r0, lsr #20
