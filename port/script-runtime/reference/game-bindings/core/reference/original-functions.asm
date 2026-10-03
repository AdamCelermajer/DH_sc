
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

# _ZNK3sfc6script3lua5Value9getStringEv
0031c49c: push     {r4, r5, r6, r7, r8, lr}
0031c4a0: ldr      r4, [pc, #0x16c]
0031c4a4: ldr      r6, [pc, #0x16c]
0031c4a8: ldr      r3, [r0, #4]
0031c4ac: add      r4, pc, r4
0031c4b0: ldr      r2, [r4, r6]
0031c4b4: sub      sp, sp, #0x28
0031c4b8: cmp      r3, #0
0031c4bc: ldr      r2, [r2]
0031c4c0: mov      r5, r0
0031c4c4: str      r2, [sp, #0x24]
0031c4c8: beq      #0x31c54c
0031c4cc: cmp      r3, #1
0031c4d0: beq      #0x31c5ec
0031c4d4: cmp      r3, #4
0031c4d8: beq      #0x31c5e4
0031c4dc: cmp      r3, #3
0031c4e0: bne      #0x31c59c
0031c4e4: ldr      r7, [r0, #8]
0031c4e8: mov      r0, r7
0031c4ec: bl       #0x30ecb8
0031c4f0: mov      r1, r0
0031c4f4: mov      r0, r7
0031c4f8: bl       #0x30df8c
0031c4fc: cmp      r0, #0
0031c500: bne      #0x31c570
0031c504: mov      r0, r7
0031c508: bl       #0x30e8a4
0031c50c: ldr      r8, [pc, #0x108]
0031c510: add      r7, sp, #4
0031c514: mov      r2, r0
0031c518: add      r8, pc, r8
0031c51c: mov      r3, r1
0031c520: mov      r0, r7
0031c524: mov      r1, r8
0031c528: bl       #0x30eae4
0031c52c: mov      r0, r7
0031c530: bl       #0x30de54
0031c534: mov      r1, r7
0031c538: add      r2, r7, r0
0031c53c: add      r0, r5, #0xc
0031c540: bl       #0x3109e0
0031c544: ldr      r0, [r5, #0x20]
0031c548: b        #0x31c554
0031c54c: ldr      r0, [pc, #0xcc]
0031c550: add      r0, pc, r0
0031c554: ldr      r3, [r4, r6]
0031c558: ldr      r2, [sp, #0x24]
0031c55c: ldr      r3, [r3]
0031c560: cmp      r2, r3
0031c564: bne      #0x31c610
0031c568: add      sp, sp, #0x28
0031c56c: pop      {r4, r5, r6, r7, r8, pc}
0031c570: mov      r0, r5
0031c574: bl       #0x31bbf0
0031c578: bl       #0x30e4cc
0031c57c: ldr      r8, [pc, #0xa0]
0031c580: add      r7, sp, #4
0031c584: mov      r2, r0
0031c588: add      r8, pc, r8
0031c58c: mov      r0, r7
0031c590: mov      r1, r8
0031c594: bl       #0x30eae4
0031c598: b        #0x31c52c
0031c59c: cmp      r3, #2
0031c5a0: beq      #0x31c5b0
0031c5a4: cmp      r3, #7
0031c5a8: movne    r0, #0
0031c5ac: bne      #0x31c554
0031c5b0: ldr      r1, [pc, #0x70]
0031c5b4: add      r7, sp, #4
0031c5b8: mov      r2, #8
0031c5bc: add      r1, pc, r1
0031c5c0: ldr      r3, [r5, #0x6c]
0031c5c4: mov      r0, r7
0031c5c8: bl       #0x30eae4
0031c5cc: mov      r0, r7
0031c5d0: bl       #0x30de54
0031c5d4: mov      r1, r7
0031c5d8: add      r2, r7, r0
0031c5dc: add      r0, r5, #0xc
0031c5e0: bl       #0x3109e0
0031c5e4: ldr      r0, [r5, #0x20]
0031c5e8: b        #0x31c554
0031c5ec: bl       #0x31bc80
0031c5f0: cmp      r0, #0
0031c5f4: bne      #0x31c604
0031c5f8: ldr      r0, [pc, #0x2c]
0031c5fc: add      r0, pc, r0
0031c600: b        #0x31c554
0031c604: ldr      r0, [pc, #0x24]
0031c608: add      r0, pc, r0
0031c60c: b        #0x31c554
0031c610: bl       #0x30e310
0031c614: rsbeq    r8, r7, r4, ror #11
0031c618: andeq    r4, r0, ip, lsr #1
0031c61c: subseq   r1, sl, r8, asr #28

# _ZN3sfc6script3lua6Binder17__smethodCallbackEP9lua_State
00319fd0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00319fd4: ldr      r4, [pc, #0x238]
00319fd8: ldr      sb, [pc, #0x238]
00319fdc: sub      sp, sp, #0x4c
00319fe0: add      r4, pc, r4
00319fe4: ldr      r3, [r4, sb]
00319fe8: mov      r1, #1
00319fec: mov      r5, r0
00319ff0: ldr      r3, [r3]
00319ff4: str      r3, [sp, #0x44]
00319ff8: bl       #0x84b264
00319ffc: cmp      r0, #5
0031a000: beq      #0x31a028
0031a004: ldr      r3, [pc, #0x210]
0031a008: ldr      r3, [r4, r3]
0031a00c: ldr      r3, [r3]
0031a010: cmp      r3, #2
0031a014: moveq    r3, #0
0031a018: streq    r3, [r3]
0031a01c: beq      #0x31a028
0031a020: cmp      r3, #1
0031a024: beq      #0x31a188
0031a028: ldr      r2, [pc, #0x1f0]
0031a02c: mov      r1, #1
0031a030: mov      r0, r5
0031a034: add      r2, pc, r2
0031a038: bl       #0x84c1ec
0031a03c: mvn      r1, #0
0031a040: mov      r0, r5
0031a044: bl       #0x84b390
0031a048: add      r7, sp, #0x14
0031a04c: mvn      r1, #1
0031a050: mov      r8, r0
0031a054: mov      r0, r5
0031a058: bl       #0x84b140
0031a05c: add      sl, sp, #0xc
0031a060: mov      r1, r5
0031a064: mvn      r2, #0
0031a068: mov      r0, r7
0031a06c: bl       #0x3196ec
0031a070: add      r6, sp, #0x1c
0031a074: mov      r2, #1
0031a078: mov      r1, r5
0031a07c: mov      r0, sl
0031a080: bl       #0x3196ec
0031a084: mov      r0, r6
0031a088: bl       #0x31b434
0031a08c: ldr      fp, [sp, #0x10]
0031a090: ldm      fp, {r0, r3}
0031a094: rsb      r3, r0, r3
0031a098: asr      r3, r3, #4
0031a09c: add      r2, r3, r3, lsl #3
0031a0a0: add      r2, r2, r2, lsl #6
0031a0a4: add      r2, r3, r2, lsl #3
0031a0a8: add      r2, r2, r2, lsl #15
0031a0ac: add      r3, r3, r2, lsl #3
0031a0b0: cmp      r3, #0
0031a0b4: bne      #0x31a0c8
0031a0b8: ldr      r0, [pc, #0x164]
0031a0bc: add      r0, pc, r0
0031a0c0: bl       #0x708eb0
0031a0c4: ldr      r0, [fp]
0031a0c8: bl       #0x31b580
0031a0cc: subs     fp, r0, #0
0031a0d0: beq      #0x31a1bc
0031a0d4: cmp      r8, #0
0031a0d8: beq      #0x31a134
0031a0dc: mov      r2, r8
0031a0e0: mov      r0, r7
0031a0e4: mov      r1, r6
0031a0e8: blx      fp
0031a0ec: mov      r1, r5
0031a0f0: mov      r0, r6
0031a0f4: bl       #0x31b308
0031a0f8: mov      r5, r0
0031a0fc: mov      r0, r6
0031a100: bl       #0x31b398
0031a104: mov      r0, sl
0031a108: bl       #0x319228
0031a10c: mov      r0, r7
0031a110: bl       #0x319228
0031a114: ldr      r3, [r4, sb]
0031a118: ldr      r2, [sp, #0x44]
0031a11c: mov      r0, r5
0031a120: ldr      r3, [r3]
0031a124: cmp      r2, r3
0031a128: bne      #0x31a210
0031a12c: add      sp, sp, #0x4c
0031a130: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0031a134: ldr      r3, [pc, #0xe0]
0031a138: ldr      r3, [r4, r3]
0031a13c: ldr      r3, [r3]
0031a140: cmp      r3, #2
0031a144: streq    r8, [r8]
0031a148: beq      #0x31a0dc
0031a14c: cmp      r3, #1
0031a150: bne      #0x31a0dc
0031a154: ldr      r0, [pc, #0xcc]
0031a158: ldr      r1, [pc, #0xcc]
0031a15c: ldr      r2, [pc, #0xcc]
0031a160: ldr      r0, [r4, r0]
0031a164: ldr      r3, [pc, #0xc8]
0031a168: mov      ip, #0x3a
0031a16c: add      r1, pc, r1
0031a170: add      r2, pc, r2
0031a174: add      r3, pc, r3
0031a178: add      r0, r0, #0xa8
0031a17c: str      ip, [sp]
0031a180: bl       #0x30e004
0031a184: b        #0x31a0dc
0031a188: ldr      r0, [pc, #0x98]
0031a18c: ldr      r1, [pc, #0xa4]
0031a190: ldr      r2, [pc, #0xa4]
0031a194: ldr      r0, [r4, r0]
0031a198: ldr      r3, [pc, #0xa0]
0031a19c: mov      ip, #0x2e
0031a1a0: add      r1, pc, r1
0031a1a4: add      r2, pc, r2
0031a1a8: add      r3, pc, r3
0031a1ac: add      r0, r0, #0xa8
0031a1b0: str      ip, [sp]
0031a1b4: bl       #0x30e004
0031a1b8: b        #0x31a028
0031a1bc: ldr      r3, [pc, #0x58]
0031a1c0: ldr      r3, [r4, r3]
0031a1c4: ldr      r3, [r3]
0031a1c8: cmp      r3, #2
0031a1cc: streq    fp, [fp]
0031a1d0: beq      #0x31a0d4
0031a1d4: cmp      r3, #1
0031a1d8: bne      #0x31a0d4
0031a1dc: ldr      r0, [pc, #0x44]
0031a1e0: ldr      r1, [pc, #0x5c]
0031a1e4: ldr      r2, [pc, #0x5c]
0031a1e8: ldr      r0, [r4, r0]
0031a1ec: ldr      r3, [pc, #0x58]
0031a1f0: mov      ip, #0x39
0031a1f4: add      r1, pc, r1
0031a1f8: add      r2, pc, r2
0031a1fc: add      r3, pc, r3
0031a200: add      r0, r0, #0xa8
0031a204: str      ip, [sp]
0031a208: bl       #0x30e004
0031a20c: b        #0x31a0d4
0031a210: bl       #0x30e310
0031a214: strhteq  sl, [r7], #-0xa0
0031a218: andeq    r4, r0, ip, lsr #1
0031a21c: andeq    r3, r0, r0, asr #19
0031a220: subseq   r4, sl, ip, lsl r8
0031a224: subseq   r4, sl, ip, lsr #7
0031a228: andeq    r1, r0, r0, asr #19
0031a22c: subseq   r4, sl, ip, ror #4
0031a230: subseq   r4, sl, r0, ror #13
0031a234: subseq   r4, sl, r4, ror #12
0031a238: subseq   r4, sl, r8, lsr r2
0031a23c: subseq   r4, sl, ip, lsl #13
0031a240: subseq   r4, sl, r0, lsr r6
0031a244: subseq   r4, sl, r4, ror #3
0031a248: subseq   r4, sl, r8, lsr #12
0031a24c: ldrsbeq  r4, [sl], #-0x5c

# _ZN3sfc6script3lua6Binder12bindFunctionEPKcPFvRKNS1_9ArgumentsERNS1_12ReturnValuesEPvESA_
0031a4d4: push     {r4, r5, r6, r7, r8, sl, lr}
0031a4d8: mov      r6, r0
0031a4dc: ldr      r0, [r0, #4]
0031a4e0: ldr      r5, [pc, #0x11c]
0031a4e4: sub      sp, sp, #0x14
0031a4e8: cmp      r0, #0
0031a4ec: add      r5, pc, r5
0031a4f0: mov      r8, r1
0031a4f4: mov      r7, r2
0031a4f8: mov      sl, r3
0031a4fc: beq      #0x31a554
0031a500: cmp      r1, #0
0031a504: beq      #0x31a55c
0031a508: cmp      r7, #0
0031a50c: beq      #0x31a5b0
0031a510: add      r4, sp, #8
0031a514: mov      r0, r4
0031a518: bl       #0x3192b4
0031a51c: mov      r0, r4
0031a520: mov      r1, r7
0031a524: bl       #0x31a46c
0031a528: mov      r0, r4
0031a52c: mov      r1, sl
0031a530: bl       #0x31a46c
0031a534: ldr      r3, [pc, #0xcc]
0031a538: ldr      r0, [r6, #4]
0031a53c: mov      r1, r8
0031a540: ldr      r2, [r5, r3]
0031a544: mov      r3, r4
0031a548: bl       #0x31af08
0031a54c: mov      r0, r4
0031a550: bl       #0x319228
0031a554: add      sp, sp, #0x14
0031a558: pop      {r4, r5, r6, r7, r8, sl, pc}
0031a55c: ldr      r3, [pc, #0xa8]
0031a560: ldr      r3, [r5, r3]
0031a564: ldr      r3, [r3]
0031a568: cmp      r3, #2
0031a56c: streq    r1, [r1]
0031a570: beq      #0x31a508
0031a574: cmp      r3, #1
0031a578: bne      #0x31a508
0031a57c: ldr      r0, [pc, #0x8c]
0031a580: ldr      r1, [pc, #0x8c]
0031a584: ldr      r2, [pc, #0x8c]
0031a588: ldr      r0, [r5, r0]
0031a58c: ldr      r3, [pc, #0x88]
0031a590: mov      ip, #0x79
0031a594: add      r1, pc, r1
0031a598: add      r2, pc, r2
0031a59c: add      r3, pc, r3
0031a5a0: add      r0, r0, #0xa8
0031a5a4: str      ip, [sp]
0031a5a8: bl       #0x30e004
0031a5ac: b        #0x31a508
0031a5b0: ldr      r3, [pc, #0x54]
0031a5b4: ldr      r3, [r5, r3]
0031a5b8: ldr      r3, [r3]
0031a5bc: cmp      r3, #2
0031a5c0: streq    r7, [r7]
0031a5c4: beq      #0x31a510
0031a5c8: cmp      r3, #1
0031a5cc: bne      #0x31a510
0031a5d0: ldr      r0, [pc, #0x38]
0031a5d4: ldr      r1, [pc, #0x44]
0031a5d8: ldr      r2, [pc, #0x44]
0031a5dc: ldr      r0, [r5, r0]
0031a5e0: ldr      r3, [pc, #0x40]
0031a5e4: mov      ip, #0x7a
0031a5e8: add      r1, pc, r1
0031a5ec: add      r2, pc, r2
0031a5f0: add      r3, pc, r3
0031a5f4: add      r0, r0, #0xa8
0031a5f8: str      ip, [sp]
0031a5fc: bl       #0x30e004
0031a600: b        #0x31a510
0031a604: rsbeq    sl, r7, r4, lsr #11
0031a608: andeq    r2, r0, r0, asr r4
0031a60c: andeq    r3, r0, r0, asr #19
0031a610: andeq    r1, r0, r0, asr #19
0031a614: subseq   r3, sl, r4, asr #28
0031a618: subseq   r4, sl, r0, lsr r2
0031a61c: subseq   r4, sl, ip, lsr r2
0031a620: ldrsheq  r3, [sl], #-0xd0
0031a624: subseq   r4, sl, r4, lsr r2
0031a628: subseq   r4, sl, r8, ror #3

# _ZN9Character10_StopTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b7064: str      lr, [sp, #-4]!
003b7068: ldr      r3, [r0, #4]
003b706c: sub      sp, sp, #0xc
003b7070: ldr      r1, [r3, #4]
003b7074: ldr      ip, [r3]
003b7078: rsb      r3, ip, r1
003b707c: asr      r3, r3, #4
003b7080: add      r1, r3, r3, lsl #3
003b7084: add      r1, r1, r1, lsl #6
003b7088: add      r1, r3, r1, lsl #3
003b708c: add      r1, r1, r1, lsl #15
003b7090: add      r3, r3, r1, lsl #3
003b7094: cmp      r3, #0
003b7098: bne      #0x3b70a4
003b709c: add      sp, sp, #0xc
003b70a0: ldm      sp!, {pc}
003b70a4: ldr      r3, [ip, #4]
003b70a8: cmp      r3, #3
003b70ac: bne      #0x3b709c
003b70b0: mov      r1, #0
003b70b4: str      r2, [sp, #4]
003b70b8: bl       #0x37baf8
003b70bc: bl       #0x38d798
003b70c0: ldr      r2, [sp, #4]
003b70c4: mov      r1, r0
003b70c8: add      r0, r2, #0x3b4
003b70cc: add      sp, sp, #0xc
003b70d0: pop      {lr}
003b70d4: b        #0x3db2d8

# _ZN9Character11_StartTimerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b7590: push     {r4, r5, r6, lr}
003b7594: ldr      r3, [r0, #4]
003b7598: mov      r4, r1
003b759c: mov      r6, r2
003b75a0: ldm      r3, {r1, r2}
003b75a4: sub      sp, sp, #8
003b75a8: mov      r5, r0
003b75ac: rsb      r3, r1, r2
003b75b0: asr      r3, r3, #4
003b75b4: add      r2, r3, r3, lsl #3
003b75b8: add      r2, r2, r2, lsl #6
003b75bc: add      r2, r3, r2, lsl #3
003b75c0: add      r2, r2, r2, lsl #15
003b75c4: add      r3, r3, r2, lsl #3
003b75c8: rsb      r3, r3, #0
003b75cc: cmp      r3, #0
003b75d0: bne      #0x3b75dc
003b75d4: add      sp, sp, #8
003b75d8: pop      {r4, r5, r6, pc}
003b75dc: ldr      r2, [r1, #4]
003b75e0: cmp      r2, #3
003b75e4: bne      #0x3b75d4
003b75e8: cmp      r3, #1
003b75ec: bls      #0x3b7604
003b75f0: mov      r1, #1
003b75f4: bl       #0x37baf8
003b75f8: bl       #0x31bc80
003b75fc: cmp      r0, #0
003b7600: bne      #0x3b764c
003b7604: mov      r1, #0
003b7608: mov      r0, r5
003b760c: bl       #0x37baf8
003b7610: bl       #0x38d798
003b7614: mov      ip, #0
003b7618: mov      r1, r0
003b761c: mov      r2, ip
003b7620: add      r0, r6, #0x3b4
003b7624: mov      r3, #0x35
003b7628: str      ip, [sp]
003b762c: bl       #0x3dbe24
003b7630: mov      r1, r0
003b7634: cmn      r1, #1
003b7638: beq      #0x3b75d4
003b763c: mov      r0, r4
003b7640: add      sp, sp, #8
003b7644: pop      {r4, r5, r6, lr}
003b7648: b        #0x37cb24
003b764c: mov      r1, #0
003b7650: mov      r0, r5
003b7654: bl       #0x37baf8
003b7658: bl       #0x38d798
003b765c: mov      ip, #0
003b7660: mov      r1, r0
003b7664: mvn      r2, #0
003b7668: add      r0, r6, #0x3b4
003b766c: mov      r3, #0x35
003b7670: str      ip, [sp]
003b7674: bl       #0x3dbe24
003b7678: mov      r1, r0
003b767c: b        #0x3b7634
