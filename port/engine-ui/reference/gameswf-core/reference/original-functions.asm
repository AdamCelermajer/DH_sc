
# _ZN7gameswf6matrix4readEPNS_6streamE
007965d4: push     {r4, r5, r6, r7, r8, lr}
007965d8: mov      r5, r0
007965dc: mov      r0, r1
007965e0: mov      r4, r1
007965e4: bl       #0x783b18
007965e8: mov      r2, #0
007965ec: add      r3, r5, #8
007965f0: str      r2, [r5, #4]
007965f4: str      r2, [r3], #4
007965f8: str      r2, [r3], #4
007965fc: str      r2, [r3], #4
00796600: mov      r1, #0x3f800000
00796604: str      r2, [r3]
00796608: mov      r0, r4
0079660c: str      r1, [r5, #0x10]
00796610: str      r1, [r5]
00796614: mov      r1, #1
00796618: bl       #0x7839a4
0079661c: cmp      r0, #0
00796620: bne      #0x79677c
00796624: mov      r0, r4
00796628: mov      r1, #1
0079662c: bl       #0x7839a4
00796630: cmp      r0, #0
00796634: bne      #0x7966e4
00796638: mov      r0, r4
0079663c: mov      r1, #5
00796640: bl       #0x7839a4
00796644: subs     r6, r0, #0
00796648: ble      #0x7966b8
0079664c: mov      r1, r6
00796650: mov      r0, r4
00796654: bl       #0x783a68
00796658: bl       #0x30e964
0079665c: mvn      r1, #0x800000
00796660: mov      r7, r0
00796664: bl       #0x30e4b4
00796668: cmp      r0, #0
0079666c: bne      #0x7966bc
00796670: mov      r7, #0
00796674: mov      r1, r6
00796678: mov      r0, r4
0079667c: str      r7, [r5, #8]
00796680: bl       #0x783a68
00796684: bl       #0x30e964
00796688: mvn      r1, #0x800000
0079668c: mov      r4, r0
00796690: bl       #0x30e4b4
00796694: cmp      r0, #0
00796698: beq      #0x7966d8
0079669c: mvn      r1, #0x80000000
007966a0: mov      r0, r4
007966a4: sub      r1, r1, #0x800000
007966a8: bl       #0x30e9ac
007966ac: cmp      r0, #0
007966b0: beq      #0x7966d8
007966b4: str      r4, [r5, #0x14]
007966b8: pop      {r4, r5, r6, r7, r8, pc}
007966bc: mvn      r1, #0x80000000
007966c0: mov      r0, r7
007966c4: sub      r1, r1, #0x800000
007966c8: bl       #0x30e9ac
007966cc: cmp      r0, #0
007966d0: bne      #0x796674
007966d4: b        #0x796670
007966d8: mov      r4, #0
007966dc: str      r4, [r5, #0x14]
007966e0: b        #0x7966b8
007966e4: mov      r1, #5
007966e8: mov      r0, r4
007966ec: bl       #0x7839a4
007966f0: mov      r7, r0
007966f4: mov      r1, r7
007966f8: mov      r0, r4
007966fc: bl       #0x783a68
00796700: bl       #0x30e964
00796704: mov      r1, #0x37800000
00796708: bl       #0x30ed6c
0079670c: mvn      r1, #0x800000
00796710: mov      r6, r0
00796714: bl       #0x30e4b4
00796718: cmp      r0, #0
0079671c: bne      #0x796830
00796720: mov      r6, #0
00796724: mov      r1, r7
00796728: str      r6, [r5, #0xc]
0079672c: mov      r0, r4
00796730: bl       #0x783a68
00796734: bl       #0x30e964
00796738: mov      r1, #0x37800000
0079673c: bl       #0x30ed6c
00796740: mvn      r1, #0x800000
00796744: mov      r6, r0
00796748: bl       #0x30e4b4
0079674c: cmp      r0, #0
00796750: beq      #0x796770
00796754: mvn      r1, #0x80000000
00796758: mov      r0, r6
0079675c: sub      r1, r1, #0x800000
00796760: bl       #0x30e9ac
00796764: cmp      r0, #0
00796768: strne    r6, [r5, #4]
0079676c: bne      #0x796638
00796770: mov      r6, #0
00796774: str      r6, [r5, #4]
00796778: b        #0x796638
0079677c: mov      r1, #5
00796780: mov      r0, r4
00796784: bl       #0x7839a4
00796788: mov      r7, r0
0079678c: mov      r1, r7
00796790: mov      r0, r4
00796794: bl       #0x783a68
00796798: bl       #0x30e964
0079679c: mov      r1, #0x37800000
007967a0: bl       #0x30ed6c
007967a4: mvn      r1, #0x800000
007967a8: mov      r6, r0
007967ac: bl       #0x30e4b4
007967b0: cmp      r0, #0
007967b4: bne      #0x796814
007967b8: mov      r6, #0
007967bc: mov      r1, r7
007967c0: str      r6, [r5]
007967c4: mov      r0, r4
007967c8: bl       #0x783a68
007967cc: bl       #0x30e964
007967d0: mov      r1, #0x37800000
007967d4: bl       #0x30ed6c
007967d8: mvn      r1, #0x800000
007967dc: mov      r6, r0
007967e0: bl       #0x30e4b4
007967e4: cmp      r0, #0
007967e8: beq      #0x796808
007967ec: mvn      r1, #0x80000000
007967f0: mov      r0, r6
007967f4: sub      r1, r1, #0x800000
007967f8: bl       #0x30e9ac
007967fc: cmp      r0, #0
00796800: strne    r6, [r5, #0x10]
00796804: bne      #0x796624
00796808: mov      r6, #0
0079680c: str      r6, [r5, #0x10]
00796810: b        #0x796624
00796814: mvn      r1, #0x80000000
00796818: mov      r0, r6
0079681c: sub      r1, r1, #0x800000
00796820: bl       #0x30e9ac
00796824: cmp      r0, #0
00796828: bne      #0x7967bc
0079682c: b        #0x7967b8
00796830: mvn      r1, #0x80000000
00796834: mov      r0, r6
00796838: sub      r1, r1, #0x800000
0079683c: bl       #0x30e9ac
00796840: cmp      r0, #0
00796844: bne      #0x796724
00796848: b        #0x796720

# _ZN7gameswf27substitute_bitmap_characterERNS_9tu_stringEPNS_20bitmap_character_defEPNS_20movie_definition_subE
00759cfc: ldr      r3, [pc, #0x90]
00759d00: ldr      r2, [pc, #0x90]
00759d04: push     {r4, r5, r6, r7, r8, lr}
00759d08: add      r3, pc, r3
00759d0c: ldr      r5, [r3, r2]
00759d10: mov      r6, r0
00759d14: ldr      r3, [r5]
00759d18: cmp      r3, #0
00759d1c: beq      #0x759d90
00759d20: ldr      r3, [r1]
00759d24: mov      r0, r1
00759d28: mov      lr, pc
00759d2c: ldr      pc, [r3, #0x2c]
00759d30: ldrsb    r3, [r6]
00759d34: mov      r4, r0
00759d38: ldr      r5, [r5]
00759d3c: cmn      r3, #1
00759d40: ldr      r3, [r0]
00759d44: addne    r6, r6, #1
00759d48: ldreq    r6, [r6, #0xc]
00759d4c: mov      lr, pc
00759d50: ldr      pc, [r3, #0x24]
00759d54: ldr      r3, [r4]
00759d58: mov      r7, r0
00759d5c: mov      r0, r4
00759d60: mov      lr, pc
00759d64: ldr      pc, [r3, #0x28]
00759d68: mov      r1, r7
00759d6c: mov      r2, r0
00759d70: mov      r0, r6
00759d74: blx      r5
00759d78: subs     r1, r0, #0
00759d7c: beq      #0x759d90
00759d80: mov      r0, r4
00759d84: ldr      r3, [r4]
00759d88: mov      lr, pc
00759d8c: ldr      pc, [r3, #0x20]
00759d90: pop      {r4, r5, r6, r7, r8, pc}
00759d94: eoreq    sl, r3, r8, lsl #27
00759d98: andeq    r2, r0, r8, ror r7
