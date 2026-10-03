
# _ZN3sfc6script3luaL5panicEP9lua_State
0031a9e8: mov      r0, #0
0031a9ec: bx       lr

# _ZN10AISDefault6OnInitEv
003dbe78: bx       lr

# _ZN6CharAI6OnInitEv
003d12b0: push     {r4, r5, r6, r7, lr}
003d12b4: ldr      r3, [r0, #4]
003d12b8: sub      sp, sp, #0xc
003d12bc: mov      r4, r0
003d12c0: mov      r0, r3
003d12c4: ldr      r3, [r3]
003d12c8: mov      lr, pc
003d12cc: ldr      pc, [r3, #0x34]
003d12d0: ldr      r5, [pc, #0xe8]
003d12d4: cmp      r0, #0
003d12d8: add      r5, pc, r5
003d12dc: bne      #0x3d139c
003d12e0: ldr      r1, [r4, #0x10]
003d12e4: cmn      r1, #1
003d12e8: beq      #0x3d12f8
003d12ec: ldr      r0, [r4, #4]
003d12f0: add      r0, r0, #0x3b4
003d12f4: bl       #0x3db2d8
003d12f8: ldr      r6, [pc, #0xc4]
003d12fc: ldr      r1, [pc, #0xc4]
003d1300: ldr      r2, [pc, #0xc4]
003d1304: ldr      r3, [r5, r6]
003d1308: add      r1, pc, r1
003d130c: add      r2, pc, r2
003d1310: ldr      r0, [r3, #0x2c]
003d1314: ldr      r7, [r4, #4]
003d1318: bl       #0x4c4bdc
003d131c: add      r7, r7, #0x3b4
003d1320: mov      r1, r0
003d1324: mov      ip, #0
003d1328: mov      r0, r7
003d132c: mvn      r2, #0
003d1330: mov      r3, #0x33
003d1334: str      ip, [sp]
003d1338: bl       #0x3dbe24
003d133c: ldr      r1, [r4, #0x14]
003d1340: str      r0, [r4, #0x10]
003d1344: cmn      r1, #1
003d1348: beq      #0x3d1358
003d134c: ldr      r0, [r4, #4]
003d1350: add      r0, r0, #0x3b4
003d1354: bl       #0x3db2d8
003d1358: ldr      r3, [r5, r6]
003d135c: ldr      r1, [pc, #0x6c]
003d1360: ldr      r2, [pc, #0x6c]
003d1364: ldr      r0, [r3, #0x2c]
003d1368: add      r1, pc, r1
003d136c: add      r2, pc, r2
003d1370: ldr      r5, [r4, #4]
003d1374: bl       #0x4c4bdc
003d1378: add      r5, r5, #0x3b4
003d137c: mov      r1, r0
003d1380: mov      ip, #0
003d1384: mov      r0, r5
003d1388: mvn      r2, #0
003d138c: mov      r3, #0x34
003d1390: str      ip, [sp]
003d1394: bl       #0x3dbe24
003d1398: str      r0, [r4, #0x14]
003d139c: ldr      r3, [r4, #0x20]
003d13a0: cmp      r3, #0
003d13a4: beq      #0x3d13b8
003d13a8: mov      r0, r3
003d13ac: ldr      r3, [r3]
003d13b0: mov      lr, pc
003d13b4: ldr      pc, [r3, #8]
003d13b8: add      sp, sp, #0xc
003d13bc: pop      {r4, r5, r6, r7, pc}
003d13c0: ldrheq   r3, [ip], #-0x78
003d13c4: strdeq   r3, r4, [r0], -r4
003d13c8: subeq    r0, pc, r8, asr #8
003d13cc: subeq    r4, pc, ip, ror #2
003d13d0: subeq    r0, pc, r8, ror #7
003d13d4: subeq    r4, pc, r4, lsl r1

# lua_newstate
008579f8: push     {r4, r5, r6, r7, r8, lr}
008579fc: mov      r4, r1
00857a00: mov      r1, #0
00857a04: mov      r5, r0
00857a08: mov      r2, r1
00857a0c: mov      r0, r4
00857a10: mov      r3, #0x14c
00857a14: blx      r5
00857a18: subs     r6, r0, #0
00857a1c: moveq    r5, r6
00857a20: beq      #0x857b4c
00857a24: mov      r8, #8
00857a28: strb     r8, [r6, #4]
00857a2c: mov      r8, #0x21
00857a30: mov      r3, #0
00857a34: strb     r8, [r6, #0x7c]
00857a38: mov      r8, #0x61
00857a3c: strb     r8, [r6, #5]
00857a40: mov      r2, r3
00857a44: add      ip, r6, #0x68
00857a48: add      lr, r6, #0xd0
00857a4c: mov      r1, #0xc8
00857a50: add      r7, r6, #0x84
00857a54: mov      r8, #1
00857a58: mov      r0, #0x14c
00857a5c: str      r5, [r6, #0x74]
00857a60: str      r4, [r6, #0x78]
00857a64: str      r3, [r6]
00857a68: str      r3, [r6, #0x20]
00857a6c: str      r3, [r6, #0x2c]
00857a70: str      r3, [r6, #0x60]
00857a74: str      r3, [r6, #0x44]
00857a78: strb     r3, [r6, #0x38]
00857a7c: str      r3, [r6, #0x3c]
00857a80: str      r3, [r6, #0x40]
00857a84: str      r3, [r6, #0x58]
00857a88: str      r3, [r6, #0x30]
00857a8c: strh     r3, [r6, #0x36]
00857a90: strh     r3, [r6, #0x34]
00857a94: strb     r3, [r6, #6]
00857a98: str      r3, [r6, #0x14]
00857a9c: str      r3, [r6, #0x28]
00857aa0: str      r3, [r6, #0x18]
00857aa4: str      r3, [r6, #0x64]
00857aa8: str      r3, [r6, #0x4c]
00857aac: str      r3, [r6, #0xa8]
00857ab0: str      r3, [r6, #0x70]
00857ab4: str      r3, [r6, #0x6c]
00857ab8: str      r3, [r6, #0x68]
00857abc: strb     r8, [r6, #0x39]
00857ac0: str      lr, [r6, #0xe0]
00857ac4: str      ip, [r6, #0x10]
00857ac8: str      r6, [r6, #0xcc]
00857acc: str      lr, [r6, #0xdc]
00857ad0: mov      r5, r6
00857ad4: str      r3, [ip, #0x60]
00857ad8: mov      r4, r2
00857adc: str      r3, [r6, #0x9c]
00857ae0: strb     r3, [r6, #0x7d]
00857ae4: str      r3, [r6, #0xa4]
00857ae8: str      r3, [r6, #0xc0]
00857aec: str      r3, [r6, #0x80]
00857af0: str      r3, [r6, #0x8c]
00857af4: str      r3, [r6, #0x90]
00857af8: str      r3, [r6, #0x94]
00857afc: str      r3, [r6, #0x98]
00857b00: str      r3, [r6, #0xb4]
00857b04: str      r7, [r6, #0x88]
00857b08: str      r0, [r6, #0xac]
00857b0c: str      r1, [r6, #0xbc]
00857b10: str      r6, [r6, #0x84]
00857b14: str      r1, [r6, #0xb8]
00857b18: mov      r3, r6
00857b1c: add      r2, r2, #1
00857b20: cmp      r2, #9
00857b24: str      r4, [r3, #0xe4]
00857b28: add      r3, r3, #4
00857b2c: bne      #0x857b1c
00857b30: ldr      r1, [pc, #0x30]
00857b34: mov      r0, r6
00857b38: mov      r2, r4
00857b3c: add      r1, pc, r1
00857b40: bl       #0x8517fc
00857b44: cmp      r0, #0
00857b48: bne      #0x857b54
00857b4c: mov      r0, r5
00857b50: pop      {r4, r5, r6, r7, r8, pc}
00857b54: mov      r0, r6
00857b58: mov      r5, r4
00857b5c: bl       #0x8578fc
00857b60: mov      r0, r5
00857b64: pop      {r4, r5, r6, r7, r8, pc}
00857b68: andeq    r0, r0, r8, lsl #2

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

# _ZN6CharAI17LoadScriptProcessEv
003cf1f0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003cf1f4: ldr      r3, [r0, #0x28]
003cf1f8: ldr      r6, [pc, #0x18c]
003cf1fc: sub      sp, sp, #0xc
003cf200: cmp      r3, #6
003cf204: mov      r4, r0
003cf208: add      r6, pc, r6
003cf20c: bgt      #0x3cf2a8
003cf210: ldrb     r2, [r0, #0x24]
003cf214: cmp      r2, #0
003cf218: bne      #0x3cf348
003cf21c: mov      r5, #6
003cf220: ldr      r8, [pc, #0x168]
003cf224: ldr      sl, [pc, #0x168]
003cf228: ldr      sb, [pc, #0x168]
003cf22c: ldr      r7, [pc, #0x168]
003cf230: ldr      fp, [pc, #0x168]
003cf234: add      r8, pc, r8
003cf238: add      sl, pc, sl
003cf23c: add      sb, pc, sb
003cf240: cmp      r3, #6
003cf244: addls    pc, pc, r3, lsl #2
003cf248: b        #0x3cf300
003cf24c: b        #0x3cf2f0
003cf250: b        #0x3cf2e0
003cf254: b        #0x3cf2d0
003cf258: b        #0x3cf2c0
003cf25c: b        #0x3cf2b0
003cf260: b        #0x3cf28c
003cf264: b        #0x3cf268
003cf268: ldr      r2, [r4, #0x20]
003cf26c: ldr      r3, [r4, #0x28]
003cf270: str      r2, [r4, #0x1c]
003cf274: add      r3, r3, #1
003cf278: cmp      r5, #0
003cf27c: str      r3, [r4, #0x28]
003cf280: beq      #0x3cf2a8
003cf284: sub      r5, r5, #1
003cf288: b        #0x3cf240
003cf28c: mov      r0, r4
003cf290: bl       #0x3cb314
003cf294: ldr      r3, [r4, #0x28]
003cf298: cmp      r5, #0
003cf29c: add      r3, r3, #1
003cf2a0: str      r3, [r4, #0x28]
003cf2a4: bne      #0x3cf284
003cf2a8: add      sp, sp, #0xc
003cf2ac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003cf2b0: mov      r0, r4
003cf2b4: bl       #0x3cdf7c
003cf2b8: ldr      r3, [r4, #0x28]
003cf2bc: b        #0x3cf274
003cf2c0: mov      r0, r4
003cf2c4: bl       #0x3cc218
003cf2c8: ldr      r3, [r4, #0x28]
003cf2cc: b        #0x3cf274
003cf2d0: mov      r0, r4
003cf2d4: bl       #0x3cc26c
003cf2d8: ldr      r3, [r4, #0x28]
003cf2dc: b        #0x3cf274
003cf2e0: mov      r0, r4
003cf2e4: bl       #0x3cc278
003cf2e8: ldr      r3, [r4, #0x28]
003cf2ec: b        #0x3cf274
003cf2f0: mov      r0, r4
003cf2f4: bl       #0x3cf04c
003cf2f8: ldr      r3, [r4, #0x28]
003cf2fc: b        #0x3cf274
003cf300: ldr      r2, [r6, r7]
003cf304: ldr      r2, [r2]
003cf308: cmp      r2, #2
003cf30c: moveq    r2, #0
003cf310: streq    r2, [r2]
003cf314: beq      #0x3cf274
003cf318: cmp      r2, #1
003cf31c: bne      #0x3cf274
003cf320: ldr      r0, [r6, fp]
003cf324: mov      r3, sb
003cf328: movw     ip, #0x22b
003cf32c: mov      r1, r8
003cf330: mov      r2, sl
003cf334: add      r0, r0, #0xa8
003cf338: str      ip, [sp]
003cf33c: bl       #0x30e004
003cf340: ldr      r3, [r4, #0x28]
003cf344: b        #0x3cf274
003cf348: ldr      r3, [r0, #4]
003cf34c: mov      r0, r3
003cf350: ldr      r3, [r3]
003cf354: mov      lr, pc
003cf358: ldr      pc, [r3, #0x28]
003cf35c: cmp      r0, #0
003cf360: ldrne    r3, [r4, #0x28]
003cf364: bne      #0x3cf21c
003cf368: ldr      r1, [r4, #0x28]
003cf36c: mov      r0, r4
003cf370: rsb      r1, r1, #7
003cf374: bl       #0x3cb854
003cf378: cmp      r0, #0
003cf37c: ble      #0x3cf2a8
003cf380: sub      r5, r0, #1
003cf384: ldr      r3, [r4, #0x28]
003cf388: b        #0x3cf220
003cf38c: subseq   r5, ip, r8, lsl #17
003cf390: subeq    pc, lr, r4, lsr #3
003cf394: subeq    pc, lr, r0, lsr r3
003cf398: subeq    r5, pc, ip, ror pc
003cf39c: andeq    r3, r0, r0, asr #19
003cf3a0: andeq    r1, r0, r0, asr #19

# _ZN6CharAI15SetScriptByNameEPKc
003ceeb0: push     {r4, r5, r6, r7, r8, sl, lr}
003ceeb4: ldr      r4, [pc, #0x16c]
003ceeb8: ldr      r5, [pc, #0x16c]
003ceebc: mov      r6, r1
003ceec0: add      r4, pc, r4
003ceec4: ldr      r3, [r4, r5]
003ceec8: ldr      r1, [pc, #0x160]
003ceecc: sub      sp, sp, #0x24
003ceed0: ldr      r3, [r3]
003ceed4: mov      r7, r0
003ceed8: add      r1, pc, r1
003ceedc: mov      r0, r6
003ceee0: mov      r2, #2
003ceee4: str      r3, [sp, #0x1c]
003ceee8: bl       #0x30ec7c
003ceeec: subs     r8, r0, #0
003ceef0: bne      #0x3cef64
003ceef4: ldr      r1, [pc, #0x138]
003ceef8: add      r6, r6, #2
003ceefc: mov      r0, r6
003cef00: add      r1, pc, r1
003cef04: bl       #0x30e31c
003cef08: cmp      r0, #0
003cef0c: beq      #0x3cf00c
003cef10: ldr      r1, [pc, #0x120]
003cef14: mov      r0, r6
003cef18: add      r1, pc, r1
003cef1c: bl       #0x30e31c
003cef20: cmp      r0, #0
003cef24: beq      #0x3ceff8
003cef28: ldr      r1, [pc, #0x10c]
003cef2c: mov      r0, r6
003cef30: add      r1, pc, r1
003cef34: bl       #0x30e31c
003cef38: cmp      r0, #0
003cef3c: beq      #0x3cf018
003cef40: ldr      r1, [pc, #0xf8]
003cef44: mov      r0, r6
003cef48: add      r1, pc, r1
003cef4c: bl       #0x30e31c
003cef50: cmp      r0, #0
003cef54: bne      #0x3cefe8
003cef58: mov      r0, r7
003cef5c: bl       #0x3cce14
003cef60: b        #0x3cefcc
003cef64: mov      r0, r7
003cef68: bl       #0x3ccaf4
003cef6c: ldr      r3, [pc, #0xd0]
003cef70: add      r8, sp, #4
003cef74: ldr      sl, [r4, r3]
003cef78: mov      r0, sl
003cef7c: bl       #0x337888
003cef80: ldr      r1, [pc, #0xc0]
003cef84: mov      r2, sp
003cef88: mov      r0, r8
003cef8c: add      r1, pc, r1
003cef90: bl       #0x3140ec
003cef94: mov      r0, sl
003cef98: mov      r1, r8
003cef9c: bl       #0x337a88
003cefa0: ldr      r0, [sp, #0x18]
003cefa4: cmp      r0, r8
003cefa8: beq      #0x3cefc8
003cefac: cmp      r0, #0
003cefb0: beq      #0x3cefc8
003cefb4: ldr      r1, [sp, #4]
003cefb8: rsb      r1, r0, r1
003cefbc: cmp      r1, #0x80
003cefc0: bhi      #0x3cf004
003cefc4: bl       #0x708f00
003cefc8: str      r6, [r7, #0x30]
003cefcc: ldr      r3, [r4, r5]
003cefd0: ldr      r2, [sp, #0x1c]
003cefd4: ldr      r3, [r3]
003cefd8: cmp      r2, r3
003cefdc: bne      #0x3cf024
003cefe0: add      sp, sp, #0x24
003cefe4: pop      {r4, r5, r6, r7, r8, sl, pc}
003cefe8: mov      r0, r7
003cefec: bl       #0x3cce14
003ceff0: str      r8, [r7, #0x30]
003ceff4: b        #0x3cefcc
003ceff8: mov      r0, r7
003ceffc: bl       #0x3ccfe4
003cf000: b        #0x3cefcc
003cf004: bl       #0x310440
003cf008: b        #0x3cefc8
003cf00c: mov      r0, r7
003cf010: bl       #0x3ccbe4
003cf014: b        #0x3cefcc
003cf018: mov      r0, r7
003cf01c: bl       #0x3cccf8
003cf020: b        #0x3cefcc
003cf024: bl       #0x30e310
003cf028: ldrsbeq  r5, [ip], #-0xb0
003cf02c: andeq    r4, r0, ip, lsr #1
003cf030: strheq   r6, [pc], #-0x48
003cf034: umaaleq  r6, pc, r8, r4
003cf038: umaaleq  r6, pc, r0, r4
003cf03c: subeq    r6, pc, r8, lsl #9
003cf040: subeq    r6, pc, r8, ror r4
003cf044: andeq    r0, r0, r4, lsl #17
003cf048: subeq    r6, pc, ip, asr #7

# _ZN3sfc6script3lua8Instance8loadFileER12StreamBuffer
0031acf4: push     {r4, r5, r6, r7, r8, sl, lr}
0031acf8: ldr      r4, [pc, #0xd8]
0031acfc: ldr      r8, [pc, #0xd8]
0031ad00: sub      sp, sp, #0x410
0031ad04: add      r4, pc, r4
0031ad08: ldr      r3, [r4, r8]
0031ad0c: sub      sp, sp, #0xc
0031ad10: mov      r6, r1
0031ad14: ldr      r3, [r3]
0031ad18: mov      sl, r2
0031ad1c: mov      r5, r0
0031ad20: str      r3, [sp, #0x414]
0031ad24: bl       #0x31a804
0031ad28: ldr      r2, [pc, #0xb0]
0031ad2c: ldr      r7, [r6, #4]
0031ad30: mov      ip, #0
0031ad34: ldr      r3, [pc, #0xa8]
0031ad38: str      ip, [sp, #4]
0031ad3c: add      ip, sp, #0x18
0031ad40: ldr      r1, [r4, r2]
0031ad44: sub      ip, ip, #4
0031ad48: add      r2, sp, #8
0031ad4c: add      r3, pc, r3
0031ad50: sub      r2, r2, #8
0031ad54: str      ip, [sp, #0xc]
0031ad58: mov      r0, r7
0031ad5c: mov      ip, #0x400
0031ad60: str      ip, [sp, #0x10]
0031ad64: str      sl, [sp, #8]
0031ad68: str      r6, [sp]
0031ad6c: bl       #0x84bbbc
0031ad70: mov      r1, r7
0031ad74: mov      r2, r0
0031ad78: mov      r0, r5
0031ad7c: bl       #0x31a8ac
0031ad80: ldr      r1, [r5, #4]
0031ad84: cmp      r1, #0
0031ad88: bne      #0x31adb0
0031ad8c: ldr      r6, [r6, #4]
0031ad90: mov      r2, r1
0031ad94: mov      r3, r1
0031ad98: mov      r0, r6
0031ad9c: bl       #0x84bc50
0031ada0: mov      r1, r6
0031ada4: mov      r2, r0
0031ada8: mov      r0, r5
0031adac: bl       #0x31a8ac
0031adb0: ldr      r3, [r4, r8]
0031adb4: ldr      r2, [sp, #0x414]
0031adb8: mov      r0, r5
0031adbc: ldr      r3, [r3]
0031adc0: cmp      r2, r3
0031adc4: bne      #0x31add4
0031adc8: add      sp, sp, #0x1c
0031adcc: add      sp, sp, #0x400
0031add0: pop      {r4, r5, r6, r7, r8, sl, pc}
0031add4: bl       #0x30e310
0031add8: rsbeq    sb, r7, ip, lsl #27
0031addc: andeq    r4, r0, ip, lsr #1
0031ade0: andeq    r0, r0, r4, asr sb
0031ade4: subseq   r3, sl, ip, lsl fp

# _Z12LuaAllocatorPvS_jj
00310500: push     {r4, r5, r6, lr}
00310504: subs     r5, r3, #0
00310508: mov      r4, r1
0031050c: mov      r6, r2
00310510: beq      #0x310548
00310514: cmp      r1, #0
00310518: beq      #0x310558
0031051c: cmp      r5, r2
00310520: movls    r5, r1
00310524: bhi      #0x310530
00310528: mov      r0, r5
0031052c: pop      {r4, r5, r6, pc}
00310530: mov      r0, r5
00310534: bl       #0x3104fc
00310538: mov      r1, r4
0031053c: mov      r2, r6
00310540: mov      r5, r0
00310544: bl       #0x30e868
00310548: mov      r0, r4
0031054c: bl       #0x310448
00310550: mov      r0, r5
00310554: pop      {r4, r5, r6, pc}
00310558: mov      r0, r5
0031055c: pop      {r4, r5, r6, lr}
00310560: b        #0x3104fc

# _ZN3sfc6script3lua8Instance13includeStringEv
0031b008: ldr      r0, [r0, #4]
0031b00c: b        #0x85a0dc

# _ZN3sfc6script3lua8Instance11includeMathEv
0031b000: ldr      r0, [r0, #4]
0031b004: b        #0x853e38

# _ZN9LuaScript12BindFunctionEv
0037b5a0: push     {r4, r5, r6, lr}
0037b5a4: add      r4, r0, #4
0037b5a8: mov      r5, r0
0037b5ac: mov      r0, r4
0037b5b0: bl       #0x31b010
0037b5b4: mov      r0, r4
0037b5b8: bl       #0x31b000
0037b5bc: mov      r0, r4
0037b5c0: bl       #0x31aff8
0037b5c4: mov      r0, r4
0037b5c8: ldr      r4, [pc, #0x3a8]
0037b5cc: bl       #0x31b008
0037b5d0: ldr      r3, [pc, #0x3a4]
0037b5d4: ldr      r1, [pc, #0x3a4]
0037b5d8: add      r4, pc, r4
0037b5dc: add      r6, r5, #0x10
0037b5e0: ldr      r2, [r4, r3]
0037b5e4: mov      r0, r6
0037b5e8: mov      r3, r5
0037b5ec: add      r1, pc, r1
0037b5f0: bl       #0x31a4d4
0037b5f4: ldr      r3, [pc, #0x388]
0037b5f8: ldr      r1, [pc, #0x388]
0037b5fc: mov      r0, r6
0037b600: ldr      r2, [r4, r3]
0037b604: add      r1, pc, r1
0037b608: mov      r3, r5
0037b60c: bl       #0x31a4d4
0037b610: ldr      r3, [pc, #0x374]
0037b614: ldr      r1, [pc, #0x374]
0037b618: mov      r0, r6
0037b61c: ldr      r2, [r4, r3]
0037b620: add      r1, pc, r1
0037b624: mov      r3, r5
0037b628: bl       #0x31a4d4
0037b62c: ldr      r3, [pc, #0x360]
0037b630: ldr      r1, [pc, #0x360]
0037b634: mov      r0, r6
0037b638: ldr      r2, [r4, r3]
0037b63c: add      r1, pc, r1
0037b640: mov      r3, r5
0037b644: bl       #0x31a4d4
0037b648: ldr      r3, [pc, #0x34c]
0037b64c: ldr      r1, [pc, #0x34c]
0037b650: mov      r0, r6
0037b654: ldr      r2, [r4, r3]
0037b658: add      r1, pc, r1
0037b65c: mov      r3, r5
0037b660: bl       #0x31a4d4
0037b664: ldr      r3, [pc, #0x338]
0037b668: ldr      r1, [pc, #0x338]
0037b66c: mov      r0, r6
0037b670: ldr      r2, [r4, r3]
0037b674: add      r1, pc, r1
0037b678: mov      r3, r5
0037b67c: bl       #0x31a4d4
0037b680: ldr      r3, [pc, #0x324]
0037b684: ldr      r1, [pc, #0x324]
0037b688: mov      r0, r6
0037b68c: ldr      r2, [r4, r3]
0037b690: add      r1, pc, r1
0037b694: mov      r3, r5
0037b698: bl       #0x31a4d4
0037b69c: ldr      r3, [pc, #0x310]
0037b6a0: ldr      r1, [pc, #0x310]
0037b6a4: mov      r0, r6
0037b6a8: ldr      r2, [r4, r3]
0037b6ac: add      r1, pc, r1
0037b6b0: mov      r3, r5
0037b6b4: bl       #0x31a4d4
0037b6b8: ldr      r3, [pc, #0x2fc]
0037b6bc: ldr      r1, [pc, #0x2fc]
0037b6c0: mov      r0, r6
0037b6c4: ldr      r2, [r4, r3]
0037b6c8: add      r1, pc, r1
0037b6cc: mov      r3, r5
0037b6d0: bl       #0x31a4d4
0037b6d4: ldr      r3, [pc, #0x2e8]
0037b6d8: ldr      r1, [pc, #0x2e8]
0037b6dc: mov      r0, r6
0037b6e0: ldr      r2, [r4, r3]
0037b6e4: add      r1, pc, r1
0037b6e8: mov      r3, r5
0037b6ec: bl       #0x31a4d4
0037b6f0: ldr      r3, [pc, #0x2d4]
0037b6f4: ldr      r1, [pc, #0x2d4]
0037b6f8: mov      r0, r6
0037b6fc: ldr      r2, [r4, r3]
0037b700: add      r1, pc, r1
0037b704: mov      r3, r5
0037b708: bl       #0x31a4d4
0037b70c: ldr      r3, [pc, #0x2c0]
0037b710: ldr      r1, [pc, #0x2c0]
0037b714: mov      r0, r6
0037b718: ldr      r2, [r4, r3]
0037b71c: add      r1, pc, r1
0037b720: mov      r3, r5
0037b724: bl       #0x31a4d4
0037b728: ldr      r3, [pc, #0x2ac]
0037b72c: ldr      r1, [pc, #0x2ac]
0037b730: mov      r0, r6
0037b734: ldr      r2, [r4, r3]
0037b738: add      r1, pc, r1
0037b73c: mov      r3, r5
0037b740: bl       #0x31a4d4
0037b744: ldr      r3, [pc, #0x298]
0037b748: ldr      r1, [pc, #0x298]
0037b74c: mov      r0, r6
0037b750: ldr      r2, [r4, r3]
0037b754: add      r1, pc, r1
0037b758: mov      r3, r5
0037b75c: bl       #0x31a4d4
0037b760: ldr      r3, [pc, #0x284]
0037b764: ldr      r1, [pc, #0x284]
0037b768: mov      r0, r6
0037b76c: ldr      r2, [r4, r3]
0037b770: add      r1, pc, r1
0037b774: mov      r3, r5
0037b778: bl       #0x31a4d4
0037b77c: ldr      r3, [pc, #0x270]
0037b780: ldr      r1, [pc, #0x270]
0037b784: mov      r0, r6
0037b788: ldr      r2, [r4, r3]
0037b78c: add      r1, pc, r1
0037b790: mov      r3, r5
0037b794: bl       #0x31a4d4
0037b798: ldr      r3, [pc, #0x25c]
0037b79c: ldr      r1, [pc, #0x25c]
0037b7a0: mov      r0, r6
0037b7a4: ldr      r2, [r4, r3]
0037b7a8: add      r1, pc, r1
0037b7ac: mov      r3, r5
0037b7b0: bl       #0x31a4d4
0037b7b4: ldr      r3, [pc, #0x248]
0037b7b8: ldr      r1, [pc, #0x248]
0037b7bc: mov      r0, r6
0037b7c0: ldr      r2, [r4, r3]
0037b7c4: add      r1, pc, r1
0037b7c8: mov      r3, r5
0037b7cc: bl       #0x31a4d4
0037b7d0: ldr      r3, [pc, #0x234]
0037b7d4: ldr      r1, [pc, #0x234]
0037b7d8: mov      r0, r6
0037b7dc: ldr      r2, [r4, r3]
0037b7e0: add      r1, pc, r1
0037b7e4: mov      r3, r5
0037b7e8: bl       #0x31a4d4
0037b7ec: ldr      r3, [pc, #0x220]
0037b7f0: ldr      r1, [pc, #0x220]
0037b7f4: mov      r0, r6
0037b7f8: ldr      r2, [r4, r3]
0037b7fc: add      r1, pc, r1
0037b800: mov      r3, r5
0037b804: bl       #0x31a4d4
0037b808: ldr      r3, [pc, #0x20c]
0037b80c: ldr      r1, [pc, #0x20c]
0037b810: mov      r0, r6
0037b814: ldr      r2, [r4, r3]
0037b818: add      r1, pc, r1
0037b81c: mov      r3, r5
0037b820: bl       #0x31a4d4
0037b824: ldr      r3, [pc, #0x1f8]
0037b828: ldr      r1, [pc, #0x1f8]
0037b82c: mov      r0, r6
0037b830: ldr      r2, [r4, r3]
0037b834: add      r1, pc, r1
0037b838: mov      r3, r5
0037b83c: bl       #0x31a4d4
0037b840: ldr      r3, [pc, #0x1e4]
0037b844: ldr      r1, [pc, #0x1e4]
0037b848: mov      r0, r6
0037b84c: ldr      r2, [r4, r3]
0037b850: add      r1, pc, r1
0037b854: mov      r3, r5
0037b858: bl       #0x31a4d4
0037b85c: ldr      r3, [pc, #0x1d0]
0037b860: ldr      r1, [pc, #0x1d0]
0037b864: mov      r0, r6
0037b868: ldr      r2, [r4, r3]
0037b86c: add      r1, pc, r1
0037b870: mov      r3, r5
0037b874: bl       #0x31a4d4
0037b878: ldr      r3, [pc, #0x1bc]
0037b87c: ldr      r1, [pc, #0x1bc]
0037b880: mov      r0, r6
0037b884: ldr      r2, [r4, r3]
0037b888: add      r1, pc, r1
0037b88c: mov      r3, r5
0037b890: bl       #0x31a4d4
0037b894: ldr      r3, [pc, #0x1a8]
0037b898: ldr      r1, [pc, #0x1a8]
0037b89c: mov      r0, r6
0037b8a0: ldr      r2, [r4, r3]
0037b8a4: add      r1, pc, r1
0037b8a8: mov      r3, r5
0037b8ac: bl       #0x31a4d4
0037b8b0: ldr      r3, [pc, #0x194]
0037b8b4: ldr      r1, [pc, #0x194]
0037b8b8: mov      r0, r6
0037b8bc: ldr      r2, [r4, r3]
0037b8c0: add      r1, pc, r1
0037b8c4: mov      r3, r5
0037b8c8: bl       #0x31a4d4
0037b8cc: ldr      r3, [pc, #0x180]
0037b8d0: ldr      r1, [pc, #0x180]
0037b8d4: mov      r0, r6
0037b8d8: ldr      r2, [r4, r3]
0037b8dc: add      r1, pc, r1
0037b8e0: mov      r3, r5
0037b8e4: bl       #0x31a4d4
0037b8e8: ldr      r3, [pc, #0x16c]
0037b8ec: ldr      r1, [pc, #0x16c]
0037b8f0: mov      r0, r6
0037b8f4: ldr      r2, [r4, r3]
0037b8f8: add      r1, pc, r1
0037b8fc: mov      r3, r5
0037b900: bl       #0x31a4d4
0037b904: ldr      r3, [pc, #0x158]
0037b908: ldr      r1, [pc, #0x158]
0037b90c: mov      r0, r6
0037b910: ldr      r2, [r4, r3]
0037b914: add      r1, pc, r1
0037b918: mov      r3, r5
0037b91c: bl       #0x31a4d4
0037b920: ldr      r3, [pc, #0x144]
0037b924: ldr      r1, [pc, #0x144]
0037b928: mov      r0, r6
0037b92c: ldr      r2, [r4, r3]
0037b930: add      r1, pc, r1
0037b934: mov      r3, r5
0037b938: bl       #0x31a4d4
0037b93c: ldr      r3, [pc, #0x130]
0037b940: ldr      r1, [pc, #0x130]
0037b944: mov      r0, r6
0037b948: ldr      r2, [r4, r3]
0037b94c: add      r1, pc, r1
0037b950: mov      r3, r5
0037b954: bl       #0x31a4d4
0037b958: ldr      r3, [pc, #0x11c]
0037b95c: ldr      r1, [pc, #0x11c]
0037b960: mov      r0, r6
0037b964: ldr      r2, [r4, r3]
0037b968: add      r1, pc, r1
0037b96c: mov      r3, r5
0037b970: pop      {r4, r5, r6, lr}
0037b974: b        #0x31a4d4
0037b978: strhteq  sb, [r1], #-0x48
0037b97c: andeq    r3, r0, r0, asr ip
0037b980: subseq   r6, r4, ip, asr r4
0037b984: andeq    r3, r0, r0, asr #20
0037b988: subseq   r6, r4, ip, asr #8
0037b98c: andeq    r1, r0, r4, lsr r7
0037b990: subseq   r6, r4, r8, lsr r4
0037b994: andeq    r0, r0, r0, ror #29
0037b998: subseq   r6, r4, r4, lsr #8
0037b99c: muleq    r0, ip, fp
0037b9a0: subseq   r6, r4, r0, lsl r4
0037b9a4: muleq    r0, r8, ip
0037b9a8: subseq   r6, r4, r4, lsl #8
0037b9ac: muleq    r0, ip, r5
0037b9b0: ldrsheq  r6, [r4], #-0x38
0037b9b4: andeq    r3, r0, r8, ror #15
0037b9b8: subseq   r6, r4, ip, ror #7
0037b9bc: andeq    r4, r0, r4, asr #23
0037b9c0: ldrsbeq  r6, [r4], #-0x38
0037b9c4: muleq    r0, r0, r8
0037b9c8: subseq   r6, r4, ip, asr #7
0037b9cc: andeq    r1, r0, r4, ror #7
0037b9d0: subseq   r6, r4, r0, asr #7
0037b9d4: strdeq   r4, r5, [r0], -r4
0037b9d8: ldrheq   r6, [r4], #-0x34
0037b9dc: andeq    r4, r0, r0, lsl #6
0037b9e0: subseq   r6, r4, r0, lsr #7
0037b9e4: andeq    r3, r0, ip, asr #10
0037b9e8: subseq   r6, r4, ip, lsl #7
0037b9ec: andeq    r0, r0, r8, lsr r7
0037b9f0: subseq   r6, r4, r8, ror r3
0037b9f4: andeq    r4, r0, r4, lsr r6
0037b9f8: subseq   r6, r4, r4, ror #6
0037b9fc: muleq    r0, r0, r0
0037ba00: subseq   r6, r4, r0, asr r3
0037ba04: andeq    r1, r0, ip, lsl #13
0037ba08: subseq   r6, r4, ip, lsr r3
0037ba0c: andeq    r1, r0, ip, lsl #9
0037ba10: subseq   r6, r4, r0, lsr r3
0037ba14: andeq    r2, r0, ip, lsr #29
0037ba18: subseq   r6, r4, r4, lsr #6
0037ba1c: andeq    r2, r0, ip, lsl #5
0037ba20: subseq   r6, r4, r8, lsl r3
0037ba24: ldrdeq   r4, r5, [r0], -ip
0037ba28: subseq   r6, r4, ip, lsl #6
0037ba2c: muleq    r0, r8, pc
0037ba30: subseq   r6, r4, r0, lsl #6
0037ba34: andeq    r1, r0, r0, asr #21
0037ba38: ldrsheq  r6, [r4], #-0x24
0037ba3c: andeq    r3, r0, r8, lsr #11
0037ba40: ldrsheq  r6, [r4], #-0x20
0037ba44: andeq    r4, r0, r0, asr r0
0037ba48: subseq   r6, r4, ip, ror #5
0037ba4c: strdeq   r3, r4, [r0], -r4
0037ba50: subseq   r6, r4, r8, ror #5
0037ba54: andeq    r2, r0, ip, lsl #8
0037ba58: subseq   r6, r4, r4, ror #5
0037ba5c: andeq    r1, r0, ip, ror #15
0037ba60: ldrsbeq  r6, [r4], #-0x28
0037ba64: muleq    r0, r8, sb
0037ba68: subseq   r6, r4, ip, asr #5
0037ba6c: andeq    r3, r0, r8, ror pc
0037ba70: subseq   r6, r4, r0, asr #5
0037ba74: andeq    r2, r0, ip, lsr #13
0037ba78: ldrheq   r6, [r4], #-0x24
0037ba7c: andeq    r2, r0, r8, lsl #30
0037ba80: subseq   r6, r4, r8, lsr #5

# _ZN3sfc6script3lua8Instance12includeTableEv
0031aff8: ldr      r0, [r0, #4]
0031affc: b        #0x85b174

# _ZN9Character14createBindingsERN3sfc6script3lua6BinderE
003b56bc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b56c0: ldr      r6, [pc, #0xff8]
003b56c4: sub      sp, sp, #0xc
003b56c8: mov      r4, r1
003b56cc: mov      r5, r0
003b56d0: bl       #0x38d7ec
003b56d4: ldr      r3, [pc, #0xfe8]
003b56d8: add      r6, pc, r6
003b56dc: ldr      r7, [pc, #0xfe4]
003b56e0: ldr      r8, [r6, r3]
003b56e4: mov      r0, r4
003b56e8: add      r7, pc, r7
003b56ec: mov      r3, r5
003b56f0: mov      r1, r7
003b56f4: mov      r2, r8
003b56f8: bl       #0x31a4d4
003b56fc: mov      r0, r4
003b5700: mov      r1, r7
003b5704: mov      r2, r8
003b5708: bl       #0x319af4
003b570c: ldr      r2, [pc, #0xfb8]
003b5710: ldr      r7, [pc, #0xfb8]
003b5714: mov      r3, r5
003b5718: ldr      r8, [r6, r2]
003b571c: add      r7, pc, r7
003b5720: mov      r0, r4
003b5724: mov      r1, r7
003b5728: mov      r2, r8
003b572c: bl       #0x31a4d4
003b5730: mov      r0, r4
003b5734: mov      r1, r7
003b5738: mov      r2, r8
003b573c: bl       #0x319af4
003b5740: ldr      r2, [pc, #0xf8c]
003b5744: ldr      r7, [pc, #0xf8c]
003b5748: mov      r3, r5
003b574c: ldr      r8, [r6, r2]
003b5750: add      r7, pc, r7
003b5754: mov      r0, r4
003b5758: mov      r1, r7
003b575c: mov      r2, r8
003b5760: bl       #0x31a4d4
003b5764: mov      r0, r4
003b5768: mov      r1, r7
003b576c: mov      r2, r8
003b5770: bl       #0x319af4
003b5774: ldr      r2, [pc, #0xf60]
003b5778: ldr      r7, [pc, #0xf60]
003b577c: mov      r3, r5
003b5780: ldr      r8, [r6, r2]
003b5784: add      r7, pc, r7
003b5788: mov      r0, r4
003b578c: mov      r1, r7
003b5790: mov      r2, r8
003b5794: bl       #0x31a4d4
003b5798: mov      r0, r4
003b579c: mov      r1, r7
003b57a0: mov      r2, r8
003b57a4: bl       #0x319af4
003b57a8: ldr      r2, [pc, #0xf34]
003b57ac: ldr      r7, [pc, #0xf34]
003b57b0: mov      r3, r5
003b57b4: ldr      r8, [r6, r2]
003b57b8: add      r7, pc, r7
003b57bc: mov      r0, r4
003b57c0: mov      r1, r7
003b57c4: mov      r2, r8
003b57c8: bl       #0x31a4d4
003b57cc: mov      r0, r4
003b57d0: mov      r1, r7
003b57d4: mov      r2, r8
003b57d8: bl       #0x319af4
003b57dc: ldr      r2, [pc, #0xf08]
003b57e0: ldr      r7, [pc, #0xf08]
003b57e4: mov      r3, r5
003b57e8: ldr      r8, [r6, r2]
003b57ec: add      r7, pc, r7
003b57f0: mov      r0, r4
003b57f4: mov      r1, r7
003b57f8: mov      r2, r8
003b57fc: bl       #0x31a4d4
003b5800: mov      r0, r4
003b5804: mov      r1, r7
003b5808: mov      r2, r8
003b580c: bl       #0x319af4
003b5810: ldr      r2, [pc, #0xedc]
003b5814: ldr      r7, [pc, #0xedc]
003b5818: mov      r3, r5
003b581c: ldr      r8, [r6, r2]
003b5820: add      r7, pc, r7
003b5824: mov      r0, r4
003b5828: mov      r1, r7
003b582c: mov      r2, r8
003b5830: bl       #0x31a4d4
003b5834: mov      r0, r4
003b5838: mov      r1, r7
003b583c: mov      r2, r8
003b5840: bl       #0x319af4
003b5844: ldr      r2, [pc, #0xeb0]
003b5848: ldr      r7, [pc, #0xeb0]
003b584c: mov      r3, r5
003b5850: ldr      r8, [r6, r2]
003b5854: add      r7, pc, r7
003b5858: mov      r0, r4
003b585c: mov      r1, r7
003b5860: mov      r2, r8
003b5864: bl       #0x31a4d4
003b5868: mov      r0, r4
003b586c: mov      r1, r7
003b5870: mov      r2, r8
003b5874: bl       #0x319af4
003b5878: ldr      r2, [pc, #0xe84]
003b587c: ldr      r7, [pc, #0xe84]
003b5880: mov      r3, r5
003b5884: ldr      r8, [r6, r2]
003b5888: add      r7, pc, r7
003b588c: mov      r0, r4
003b5890: mov      r1, r7
003b5894: mov      r2, r8
003b5898: bl       #0x31a4d4
003b589c: mov      r0, r4
003b58a0: mov      r1, r7
003b58a4: mov      r2, r8
003b58a8: bl       #0x319af4
003b58ac: ldr      r2, [pc, #0xe58]
003b58b0: ldr      r7, [pc, #0xe58]
003b58b4: mov      r3, r5
003b58b8: ldr      r8, [r6, r2]
003b58bc: add      r7, pc, r7
003b58c0: mov      r0, r4
003b58c4: mov      r1, r7
003b58c8: mov      r2, r8
003b58cc: bl       #0x31a4d4
003b58d0: mov      r0, r4
003b58d4: mov      r1, r7
003b58d8: mov      r2, r8
003b58dc: bl       #0x319af4
003b58e0: ldr      r2, [pc, #0xe2c]
003b58e4: ldr      r7, [pc, #0xe2c]
003b58e8: mov      r3, r5
003b58ec: ldr      r8, [r6, r2]
003b58f0: add      r7, pc, r7
003b58f4: mov      r0, r4
003b58f8: mov      r1, r7
003b58fc: mov      r2, r8
003b5900: bl       #0x31a4d4
003b5904: mov      r0, r4
003b5908: mov      r1, r7
003b590c: mov      r2, r8
003b5910: bl       #0x319af4
003b5914: ldr      r2, [pc, #0xe00]
003b5918: ldr      r7, [pc, #0xe00]
003b591c: mov      r3, r5
003b5920: ldr      r8, [r6, r2]
003b5924: add      r7, pc, r7
003b5928: mov      r0, r4
003b592c: mov      r1, r7
003b5930: mov      r2, r8
003b5934: bl       #0x31a4d4
003b5938: mov      r0, r4
003b593c: mov      r1, r7
003b5940: mov      r2, r8
003b5944: bl       #0x319af4
003b5948: ldr      r2, [pc, #0xdd4]
003b594c: ldr      r7, [pc, #0xdd4]
003b5950: mov      r3, r5
003b5954: ldr      r8, [r6, r2]
003b5958: add      r7, pc, r7
003b595c: mov      r0, r4
003b5960: mov      r1, r7
003b5964: mov      r2, r8
003b5968: bl       #0x31a4d4
003b596c: mov      r0, r4
003b5970: mov      r1, r7
003b5974: mov      r2, r8
003b5978: bl       #0x319af4
003b597c: ldr      r2, [pc, #0xda8]
003b5980: ldr      r7, [pc, #0xda8]
003b5984: mov      r3, r5
003b5988: ldr      r8, [r6, r2]
003b598c: add      r7, pc, r7
003b5990: mov      r0, r4
003b5994: mov      r1, r7
003b5998: mov      r2, r8
003b599c: bl       #0x31a4d4
003b59a0: mov      r0, r4
003b59a4: mov      r1, r7
003b59a8: mov      r2, r8
003b59ac: bl       #0x319af4
003b59b0: ldr      r2, [pc, #0xd7c]
003b59b4: ldr      r7, [pc, #0xd7c]
003b59b8: mov      r3, r5
003b59bc: ldr      r8, [r6, r2]
003b59c0: add      r7, pc, r7
003b59c4: mov      r0, r4
003b59c8: mov      r1, r7
003b59cc: mov      r2, r8
003b59d0: bl       #0x31a4d4
003b59d4: mov      r0, r4
003b59d8: mov      r1, r7
003b59dc: mov      r2, r8
003b59e0: bl       #0x319af4
003b59e4: ldr      r2, [pc, #0xd50]
003b59e8: ldr      r7, [pc, #0xd50]
003b59ec: mov      r3, r5
003b59f0: ldr      r8, [r6, r2]
003b59f4: add      r7, pc, r7
003b59f8: mov      r0, r4
003b59fc: mov      r1, r7
003b5a00: mov      r2, r8
003b5a04: bl       #0x31a4d4
003b5a08: mov      r0, r4
003b5a0c: mov      r1, r7
003b5a10: mov      r2, r8
003b5a14: bl       #0x319af4
003b5a18: ldr      r2, [pc, #0xd24]
003b5a1c: ldr      r7, [pc, #0xd24]
003b5a20: mov      r3, r5
003b5a24: ldr      r8, [r6, r2]
003b5a28: add      r7, pc, r7
003b5a2c: mov      r0, r4
003b5a30: mov      r1, r7
003b5a34: mov      r2, r8
003b5a38: bl       #0x31a4d4
003b5a3c: mov      r0, r4
003b5a40: mov      r1, r7
003b5a44: mov      r2, r8
003b5a48: bl       #0x319af4
003b5a4c: ldr      r2, [pc, #0xcf8]
003b5a50: ldr      r7, [pc, #0xcf8]
003b5a54: mov      r3, r5
003b5a58: ldr      r8, [r6, r2]
003b5a5c: add      r7, pc, r7
003b5a60: mov      r0, r4
003b5a64: mov      r1, r7
003b5a68: mov      r2, r8
003b5a6c: bl       #0x31a4d4
003b5a70: mov      r0, r4
003b5a74: mov      r1, r7
003b5a78: mov      r2, r8
003b5a7c: bl       #0x319af4
003b5a80: ldr      r2, [pc, #0xccc]
003b5a84: ldr      r7, [pc, #0xccc]
003b5a88: mov      r3, r5
003b5a8c: ldr      r8, [r6, r2]
003b5a90: add      r7, pc, r7
003b5a94: mov      r0, r4
003b5a98: mov      r1, r7
003b5a9c: mov      r2, r8
003b5aa0: bl       #0x31a4d4
003b5aa4: mov      r0, r4
003b5aa8: mov      r1, r7
003b5aac: mov      r2, r8
003b5ab0: bl       #0x319af4
003b5ab4: ldr      r2, [pc, #0xca0]
003b5ab8: ldr      r7, [pc, #0xca0]
003b5abc: mov      r3, r5
003b5ac0: ldr      r8, [r6, r2]
003b5ac4: add      r7, pc, r7
003b5ac8: mov      r0, r4
003b5acc: mov      r1, r7
003b5ad0: mov      r2, r8
003b5ad4: bl       #0x31a4d4
003b5ad8: mov      r0, r4
003b5adc: mov      r1, r7
003b5ae0: mov      r2, r8
003b5ae4: bl       #0x319af4
003b5ae8: ldr      r2, [pc, #0xc74]
003b5aec: ldr      r7, [pc, #0xc74]
003b5af0: mov      r3, r5
003b5af4: ldr      r8, [r6, r2]
003b5af8: add      r7, pc, r7
003b5afc: mov      r0, r4
003b5b00: mov      r1, r7
003b5b04: mov      r2, r8
003b5b08: bl       #0x31a4d4
003b5b0c: mov      r0, r4
003b5b10: mov      r1, r7
003b5b14: mov      r2, r8
003b5b18: bl       #0x319af4
003b5b1c: ldr      r2, [pc, #0xc48]
003b5b20: ldr      r7, [pc, #0xc48]
003b5b24: mov      r3, r5
003b5b28: ldr      r8, [r6, r2]
003b5b2c: add      r7, pc, r7
003b5b30: mov      r0, r4
003b5b34: mov      r1, r7
003b5b38: mov      r2, r8
003b5b3c: bl       #0x31a4d4
003b5b40: mov      r0, r4
003b5b44: mov      r1, r7
003b5b48: mov      r2, r8
003b5b4c: bl       #0x319af4
003b5b50: ldr      r3, [pc, #0xc1c]
003b5b54: ldr      r7, [pc, #0xc1c]
003b5b58: ldr      r8, [pc, #0xc1c]
003b5b5c: ldr      sl, [r6, r3]
003b5b60: add      r7, pc, r7
003b5b64: mov      r3, r5
003b5b68: mov      r0, r4
003b5b6c: mov      r1, r7
003b5b70: mov      r2, sl
003b5b74: bl       #0x31a4d4
003b5b78: add      r8, pc, r8
003b5b7c: mov      r0, r4
003b5b80: mov      r1, r7
003b5b84: mov      r2, sl
003b5b88: bl       #0x319af4
003b5b8c: mov      r3, r5
003b5b90: mov      r0, r4
003b5b94: mov      r1, r8
003b5b98: mov      r2, sl
003b5b9c: bl       #0x31a4d4
003b5ba0: mov      r0, r4
003b5ba4: mov      r1, r8
003b5ba8: mov      r2, sl
003b5bac: bl       #0x319af4
003b5bb0: ldr      r2, [pc, #0xbc8]
003b5bb4: ldr      r7, [pc, #0xbc8]
003b5bb8: mov      r3, r5
003b5bbc: ldr      r8, [r6, r2]
003b5bc0: add      r7, pc, r7
003b5bc4: mov      r0, r4
003b5bc8: mov      r1, r7
003b5bcc: mov      r2, r8
003b5bd0: bl       #0x31a4d4
003b5bd4: mov      r0, r4
003b5bd8: mov      r1, r7
003b5bdc: mov      r2, r8
003b5be0: bl       #0x319af4
003b5be4: ldr      r2, [pc, #0xb9c]
003b5be8: ldr      r7, [pc, #0xb9c]
003b5bec: mov      r3, r5
003b5bf0: ldr      r8, [r6, r2]
003b5bf4: add      r7, pc, r7
003b5bf8: mov      r0, r4
003b5bfc: mov      r1, r7
003b5c00: mov      r2, r8
003b5c04: bl       #0x31a4d4
003b5c08: mov      r0, r4
003b5c0c: mov      r1, r7
003b5c10: mov      r2, r8
003b5c14: bl       #0x319af4
003b5c18: ldr      r2, [pc, #0xb70]
003b5c1c: ldr      r7, [pc, #0xb70]
003b5c20: mov      r3, r5
003b5c24: ldr      r8, [r6, r2]
003b5c28: add      r7, pc, r7
003b5c2c: mov      r0, r4
003b5c30: mov      r1, r7
003b5c34: mov      r2, r8
003b5c38: bl       #0x31a4d4
003b5c3c: mov      r0, r4
003b5c40: mov      r1, r7
003b5c44: mov      r2, r8
003b5c48: bl       #0x319af4
003b5c4c: ldr      r2, [pc, #0xb44]
003b5c50: ldr      r7, [pc, #0xb44]
003b5c54: mov      r3, r5
003b5c58: ldr      r8, [r6, r2]
003b5c5c: add      r7, pc, r7
003b5c60: mov      r0, r4
003b5c64: mov      r1, r7
003b5c68: mov      r2, r8
003b5c6c: bl       #0x31a4d4
003b5c70: mov      r0, r4
003b5c74: mov      r1, r7
003b5c78: mov      r2, r8
003b5c7c: bl       #0x319af4
003b5c80: ldr      r2, [pc, #0xb18]
003b5c84: ldr      r7, [pc, #0xb18]
003b5c88: mov      r3, r5
003b5c8c: ldr      r8, [r6, r2]
003b5c90: add      r7, pc, r7
003b5c94: mov      r0, r4
003b5c98: mov      r1, r7
003b5c9c: mov      r2, r8
003b5ca0: bl       #0x31a4d4
003b5ca4: mov      r0, r4
003b5ca8: mov      r1, r7
003b5cac: mov      r2, r8
003b5cb0: bl       #0x319af4
003b5cb4: ldr      r2, [pc, #0xaec]
003b5cb8: ldr      r7, [pc, #0xaec]
003b5cbc: mov      r3, r5
003b5cc0: ldr      r8, [r6, r2]
003b5cc4: add      r7, pc, r7
003b5cc8: mov      r0, r4
003b5ccc: mov      r1, r7
003b5cd0: mov      r2, r8
003b5cd4: bl       #0x31a4d4
003b5cd8: mov      r0, r4
003b5cdc: mov      r1, r7
003b5ce0: mov      r2, r8
003b5ce4: bl       #0x319af4
003b5ce8: ldr      r2, [pc, #0xac0]
003b5cec: ldr      r7, [pc, #0xac0]
003b5cf0: mov      r3, r5
003b5cf4: ldr      r8, [r6, r2]
003b5cf8: add      r7, pc, r7
003b5cfc: mov      r0, r4
003b5d00: mov      r1, r7
003b5d04: mov      r2, r8
003b5d08: bl       #0x31a4d4
003b5d0c: mov      r0, r4
003b5d10: mov      r1, r7
003b5d14: mov      r2, r8
003b5d18: bl       #0x319af4
003b5d1c: ldr      r2, [pc, #0xa94]
003b5d20: ldr      r7, [pc, #0xa94]
003b5d24: mov      r3, r5
003b5d28: ldr      r8, [r6, r2]
003b5d2c: add      r7, pc, r7
003b5d30: mov      r0, r4
003b5d34: mov      r1, r7
003b5d38: mov      r2, r8
003b5d3c: bl       #0x31a4d4
003b5d40: mov      r0, r4
003b5d44: mov      r1, r7
003b5d48: mov      r2, r8
003b5d4c: bl       #0x319af4
003b5d50: ldr      r2, [pc, #0xa68]
003b5d54: ldr      r7, [pc, #0xa68]
003b5d58: mov      r3, r5
003b5d5c: ldr      r8, [r6, r2]
003b5d60: add      r7, pc, r7
003b5d64: mov      r0, r4
003b5d68: mov      r1, r7
003b5d6c: mov      r2, r8
003b5d70: bl       #0x31a4d4
003b5d74: mov      r0, r4
003b5d78: mov      r1, r7
003b5d7c: mov      r2, r8
003b5d80: bl       #0x319af4
003b5d84: ldr      r2, [pc, #0xa3c]
003b5d88: ldr      r7, [pc, #0xa3c]
003b5d8c: mov      r3, r5
003b5d90: ldr      r8, [r6, r2]
003b5d94: add      r7, pc, r7
003b5d98: mov      r0, r4
003b5d9c: mov      r1, r7
003b5da0: mov      r2, r8
003b5da4: bl       #0x31a4d4
003b5da8: mov      r0, r4
003b5dac: mov      r1, r7
003b5db0: mov      r2, r8
003b5db4: bl       #0x319af4
003b5db8: ldr      r2, [pc, #0xa10]
003b5dbc: ldr      r7, [pc, #0xa10]
003b5dc0: mov      r3, r5
003b5dc4: ldr      r8, [r6, r2]
003b5dc8: add      r7, pc, r7
003b5dcc: mov      r0, r4
003b5dd0: mov      r1, r7
003b5dd4: mov      r2, r8
003b5dd8: bl       #0x31a4d4
003b5ddc: mov      r0, r4
003b5de0: mov      r1, r7
003b5de4: mov      r2, r8
003b5de8: bl       #0x319af4
003b5dec: ldr      r2, [pc, #0x9e4]
003b5df0: ldr      r7, [pc, #0x9e4]
003b5df4: mov      r3, r5
003b5df8: ldr      r8, [r6, r2]
003b5dfc: add      r7, pc, r7
003b5e00: mov      r0, r4
003b5e04: mov      r1, r7
003b5e08: mov      r2, r8
003b5e0c: bl       #0x31a4d4
003b5e10: mov      r0, r4
003b5e14: mov      r1, r7
003b5e18: mov      r2, r8
003b5e1c: bl       #0x319af4
003b5e20: ldr      r2, [pc, #0x9b8]
003b5e24: ldr      r7, [pc, #0x9b8]
003b5e28: mov      r3, r5
003b5e2c: ldr      r8, [r6, r2]
003b5e30: add      r7, pc, r7
003b5e34: mov      r0, r4
003b5e38: mov      r1, r7
003b5e3c: mov      r2, r8
003b5e40: bl       #0x31a4d4
003b5e44: mov      r0, r4
003b5e48: mov      r1, r7
003b5e4c: mov      r2, r8
003b5e50: bl       #0x319af4
003b5e54: ldr      r2, [pc, #0x98c]
003b5e58: ldr      r7, [pc, #0x98c]
003b5e5c: mov      r3, r5
003b5e60: ldr      r8, [r6, r2]
003b5e64: add      r7, pc, r7
003b5e68: mov      r0, r4
003b5e6c: mov      r1, r7
003b5e70: mov      r2, r8
003b5e74: bl       #0x31a4d4
003b5e78: mov      r0, r4
003b5e7c: mov      r1, r7
003b5e80: mov      r2, r8
003b5e84: bl       #0x319af4
003b5e88: ldr      r3, [pc, #0x960]
003b5e8c: ldr      r1, [pc, #0x960]
003b5e90: mov      r0, r4
003b5e94: ldr      r2, [r6, r3]
003b5e98: add      r1, pc, r1
003b5e9c: mov      r3, #0
003b5ea0: bl       #0x31a4d4
003b5ea4: ldr      r2, [pc, #0x94c]
003b5ea8: ldr      r7, [pc, #0x94c]
003b5eac: mov      r3, r5
003b5eb0: ldr      r8, [r6, r2]
003b5eb4: add      r7, pc, r7
003b5eb8: mov      r0, r4
003b5ebc: mov      r1, r7
003b5ec0: mov      r2, r8
003b5ec4: bl       #0x31a4d4
003b5ec8: mov      r0, r4
003b5ecc: mov      r1, r7
003b5ed0: mov      r2, r8
003b5ed4: bl       #0x319af4
003b5ed8: ldr      r2, [pc, #0x920]
003b5edc: ldr      r7, [pc, #0x920]
003b5ee0: mov      r3, r5
003b5ee4: ldr      r8, [r6, r2]
003b5ee8: add      r7, pc, r7
003b5eec: mov      r0, r4
003b5ef0: mov      r1, r7
003b5ef4: mov      r2, r8
003b5ef8: bl       #0x31a4d4
003b5efc: mov      r0, r4
003b5f00: mov      r1, r7
003b5f04: mov      r2, r8
003b5f08: bl       #0x319af4
003b5f0c: ldr      r2, [pc, #0x8f4]
003b5f10: ldr      r7, [pc, #0x8f4]
003b5f14: mov      r3, r5
003b5f18: ldr      r8, [r6, r2]
003b5f1c: add      r7, pc, r7
003b5f20: mov      r0, r4
003b5f24: mov      r1, r7
003b5f28: mov      r2, r8
003b5f2c: bl       #0x31a4d4
003b5f30: mov      r0, r4
003b5f34: mov      r1, r7
003b5f38: mov      r2, r8
003b5f3c: bl       #0x319af4
003b5f40: ldr      r2, [pc, #0x8c8]
003b5f44: ldr      r7, [pc, #0x8c8]
003b5f48: mov      r3, r5
003b5f4c: ldr      r8, [r6, r2]
003b5f50: add      r7, pc, r7
003b5f54: mov      r0, r4
003b5f58: mov      r1, r7
003b5f5c: mov      r2, r8
003b5f60: bl       #0x31a4d4
003b5f64: mov      r0, r4
003b5f68: mov      r1, r7
003b5f6c: mov      r2, r8
003b5f70: bl       #0x319af4
003b5f74: ldr      r2, [pc, #0x89c]
003b5f78: ldr      r7, [pc, #0x89c]
003b5f7c: mov      r3, r5
003b5f80: ldr      r8, [r6, r2]
003b5f84: add      r7, pc, r7
003b5f88: mov      r0, r4
003b5f8c: mov      r1, r7
003b5f90: mov      r2, r8
003b5f94: bl       #0x31a4d4
003b5f98: mov      r0, r4
003b5f9c: mov      r1, r7
003b5fa0: mov      r2, r8
003b5fa4: bl       #0x319af4
003b5fa8: ldr      r3, [pc, #0x870]
003b5fac: ldr      r7, [pc, #0x870]
003b5fb0: ldr      r8, [pc, #0x870]
003b5fb4: ldr      sl, [r6, r3]
003b5fb8: add      r7, pc, r7
003b5fbc: mov      r3, r5
003b5fc0: mov      r0, r4
003b5fc4: mov      r1, r7
003b5fc8: mov      r2, sl
003b5fcc: bl       #0x31a4d4
003b5fd0: add      r8, pc, r8
003b5fd4: mov      r0, r4
003b5fd8: mov      r1, r7
003b5fdc: mov      r2, sl
003b5fe0: bl       #0x319af4
003b5fe4: mov      r3, r5
003b5fe8: mov      r0, r4
003b5fec: mov      r1, r8
003b5ff0: mov      r2, sl
003b5ff4: bl       #0x31a4d4
003b5ff8: mov      r0, r4
003b5ffc: mov      r1, r8
003b6000: mov      r2, sl
003b6004: bl       #0x319af4
003b6008: ldr      r3, [pc, #0x81c]
003b600c: ldr      r7, [pc, #0x81c]
003b6010: ldr      r8, [pc, #0x81c]
003b6014: ldr      sl, [r6, r3]
003b6018: add      r7, pc, r7
003b601c: mov      r3, r5
003b6020: mov      r0, r4
003b6024: mov      r1, r7
003b6028: mov      r2, sl
003b602c: bl       #0x31a4d4
003b6030: add      r8, pc, r8
003b6034: mov      r0, r4
003b6038: mov      r1, r7
003b603c: mov      r2, sl
003b6040: bl       #0x319af4
003b6044: mov      r3, r5
003b6048: mov      r0, r4
003b604c: mov      r1, r8
003b6050: mov      r2, sl
003b6054: bl       #0x31a4d4
003b6058: mov      r0, r4
003b605c: mov      r1, r8
003b6060: mov      r2, sl
003b6064: bl       #0x319af4
003b6068: ldr      r2, [pc, #0x7c8]
003b606c: ldr      r7, [pc, #0x7c8]
003b6070: mov      r3, r5
003b6074: ldr      r8, [r6, r2]
003b6078: add      r7, pc, r7
003b607c: mov      r0, r4
003b6080: mov      r1, r7
003b6084: mov      r2, r8
003b6088: bl       #0x31a4d4
003b608c: mov      r0, r4
003b6090: mov      r1, r7
003b6094: mov      r2, r8
003b6098: bl       #0x319af4
003b609c: ldr      r2, [pc, #0x79c]
003b60a0: ldr      r7, [pc, #0x79c]
003b60a4: mov      r3, r5
003b60a8: ldr      r8, [r6, r2]
003b60ac: add      r7, pc, r7
003b60b0: mov      r0, r4
003b60b4: mov      r1, r7
003b60b8: mov      r2, r8
003b60bc: bl       #0x31a4d4
003b60c0: mov      r0, r4
003b60c4: mov      r1, r7
003b60c8: mov      r2, r8
003b60cc: bl       #0x319af4
003b60d0: ldr      r2, [pc, #0x770]
003b60d4: ldr      r7, [pc, #0x770]
003b60d8: mov      r3, r5
003b60dc: ldr      r8, [r6, r2]
003b60e0: add      r7, pc, r7
003b60e4: mov      r0, r4
003b60e8: mov      r1, r7
003b60ec: mov      r2, r8
003b60f0: bl       #0x31a4d4
003b60f4: mov      r0, r4
003b60f8: mov      r1, r7
003b60fc: mov      r2, r8
003b6100: bl       #0x319af4
003b6104: ldr      r2, [pc, #0x744]
003b6108: ldr      r7, [pc, #0x744]
003b610c: mov      r3, r5
003b6110: ldr      r8, [r6, r2]
003b6114: add      r7, pc, r7
003b6118: mov      r0, r4
003b611c: mov      r1, r7
003b6120: mov      r2, r8
003b6124: bl       #0x31a4d4
003b6128: mov      r0, r4
003b612c: mov      r1, r7
003b6130: mov      r2, r8
003b6134: bl       #0x319af4
003b6138: ldr      r2, [pc, #0x718]
003b613c: ldr      r7, [pc, #0x718]
003b6140: mov      r3, r5
003b6144: ldr      r8, [r6, r2]
003b6148: add      r7, pc, r7
003b614c: mov      r0, r4
003b6150: mov      r1, r7
003b6154: mov      r2, r8
003b6158: bl       #0x31a4d4
003b615c: mov      r0, r4
003b6160: mov      r1, r7
003b6164: mov      r2, r8
003b6168: bl       #0x319af4
003b616c: ldr      r3, [pc, #0x6ec]
003b6170: ldr      r7, [pc, #0x6ec]
003b6174: ldr      r8, [pc, #0x6ec]
003b6178: ldr      sl, [r6, r3]
003b617c: add      r7, pc, r7
003b6180: mov      r3, r5
003b6184: mov      r0, r4
003b6188: mov      r1, r7
003b618c: mov      r2, sl
003b6190: bl       #0x31a4d4
003b6194: add      r8, pc, r8
003b6198: mov      r0, r4
003b619c: mov      r1, r7
003b61a0: mov      r2, sl
003b61a4: bl       #0x319af4
003b61a8: mov      r3, r5
003b61ac: mov      r0, r4
003b61b0: mov      r1, r8
003b61b4: mov      r2, sl
003b61b8: bl       #0x31a4d4
003b61bc: mov      r0, r4
003b61c0: mov      r1, r8
003b61c4: mov      r2, sl
003b61c8: bl       #0x319af4
003b61cc: ldr      r2, [pc, #0x698]
003b61d0: ldr      r7, [pc, #0x698]
003b61d4: mov      r3, r5
003b61d8: ldr      r8, [r6, r2]
003b61dc: add      r7, pc, r7
003b61e0: mov      r0, r4
003b61e4: mov      r1, r7
003b61e8: mov      r2, r8
003b61ec: bl       #0x31a4d4
003b61f0: mov      r0, r4
003b61f4: mov      r1, r7
003b61f8: mov      r2, r8
003b61fc: bl       #0x319af4
003b6200: ldr      r2, [pc, #0x66c]
003b6204: ldr      r7, [pc, #0x66c]
003b6208: mov      r3, r5
003b620c: ldr      r8, [r6, r2]
003b6210: add      r7, pc, r7
003b6214: mov      r0, r4
003b6218: mov      r1, r7
003b621c: mov      r2, r8
003b6220: bl       #0x31a4d4
003b6224: mov      r0, r4
003b6228: mov      r1, r7
003b622c: mov      r2, r8
003b6230: bl       #0x319af4
003b6234: ldr      r2, [pc, #0x640]
003b6238: ldr      r7, [pc, #0x640]
003b623c: mov      r3, r5
003b6240: ldr      r8, [r6, r2]
003b6244: add      r7, pc, r7
003b6248: mov      r0, r4
003b624c: mov      r1, r7
003b6250: mov      r2, r8
003b6254: bl       #0x31a4d4
003b6258: mov      r0, r4
003b625c: mov      r1, r7
003b6260: mov      r2, r8
003b6264: bl       #0x319af4
003b6268: ldr      r2, [pc, #0x614]
003b626c: ldr      r7, [pc, #0x614]
003b6270: mov      r3, r5
003b6274: ldr      r8, [r6, r2]
003b6278: add      r7, pc, r7
003b627c: mov      r0, r4
003b6280: mov      r1, r7
003b6284: mov      r2, r8
003b6288: bl       #0x31a4d4
003b628c: mov      r0, r4
003b6290: mov      r1, r7
003b6294: mov      r2, r8
003b6298: bl       #0x319af4
003b629c: ldr      r2, [pc, #0x5e8]
003b62a0: ldr      r7, [pc, #0x5e8]
003b62a4: mov      r3, r5
003b62a8: ldr      r8, [r6, r2]
003b62ac: add      r7, pc, r7
003b62b0: mov      r0, r4
003b62b4: mov      r1, r7
003b62b8: mov      r2, r8
003b62bc: bl       #0x31a4d4
003b62c0: mov      r0, r4
003b62c4: mov      r1, r7
003b62c8: mov      r2, r8
003b62cc: bl       #0x319af4
003b62d0: ldr      r2, [pc, #0x5bc]
003b62d4: ldr      r7, [pc, #0x5bc]
003b62d8: mov      r3, r5
003b62dc: ldr      r8, [r6, r2]
003b62e0: add      r7, pc, r7
003b62e4: mov      r0, r4
003b62e8: mov      r1, r7
003b62ec: mov      r2, r8
003b62f0: bl       #0x31a4d4
003b62f4: mov      r0, r4
003b62f8: mov      r1, r7
003b62fc: mov      r2, r8
003b6300: bl       #0x319af4
003b6304: ldr      r2, [pc, #0x590]
003b6308: ldr      r7, [pc, #0x590]
003b630c: mov      r3, r5
003b6310: ldr      r8, [r6, r2]
003b6314: add      r7, pc, r7
003b6318: mov      r0, r4
003b631c: mov      r1, r7
003b6320: mov      r2, r8
003b6324: bl       #0x31a4d4
003b6328: mov      r0, r4
003b632c: mov      r1, r7
003b6330: mov      r2, r8
003b6334: bl       #0x319af4
003b6338: ldr      r2, [pc, #0x564]
003b633c: ldr      r7, [pc, #0x564]
003b6340: mov      r3, r5
003b6344: ldr      r8, [r6, r2]
003b6348: add      r7, pc, r7
003b634c: mov      r0, r4
003b6350: mov      r1, r7
003b6354: mov      r2, r8
003b6358: bl       #0x31a4d4
003b635c: mov      r0, r4
003b6360: mov      r1, r7
003b6364: mov      r2, r8
003b6368: bl       #0x319af4
003b636c: ldr      r2, [pc, #0x538]
003b6370: ldr      r7, [pc, #0x538]
003b6374: mov      r3, r5
003b6378: ldr      r2, [r6, r2]
003b637c: add      r7, pc, r7
003b6380: mov      r0, r4
003b6384: mov      r1, r7
003b6388: str      r2, [sp, #4]
003b638c: bl       #0x31a4d4
003b6390: mov      r0, r4
003b6394: mov      r1, r7
003b6398: ldr      r2, [sp, #4]
003b639c: bl       #0x319af4
003b63a0: ldr      r2, [pc, #0x50c]
003b63a4: ldr      r7, [pc, #0x50c]
003b63a8: mov      r3, r5
003b63ac: ldr      fp, [r6, r2]
003b63b0: add      r7, pc, r7
003b63b4: mov      r0, r4
003b63b8: mov      r1, r7
003b63bc: mov      r2, fp
003b63c0: bl       #0x31a4d4
003b63c4: mov      r0, r4
003b63c8: mov      r1, r7
003b63cc: mov      r2, fp
003b63d0: bl       #0x319af4
003b63d4: ldr      r2, [pc, #0x4e0]
003b63d8: ldr      r7, [pc, #0x4e0]
003b63dc: mov      r3, r5
003b63e0: ldr      sb, [r6, r2]
003b63e4: add      r7, pc, r7
003b63e8: mov      r0, r4
003b63ec: mov      r1, r7
003b63f0: mov      r2, sb
003b63f4: bl       #0x31a4d4
003b63f8: mov      r0, r4
003b63fc: mov      r1, r7
003b6400: mov      r2, sb
003b6404: bl       #0x319af4
003b6408: ldr      r2, [pc, #0x4b4]
003b640c: ldr      r7, [pc, #0x4b4]
003b6410: mov      r3, r5
003b6414: ldr      r8, [r6, r2]
003b6418: add      r7, pc, r7
003b641c: mov      r0, r4
003b6420: mov      r1, r7
003b6424: mov      r2, r8
003b6428: bl       #0x31a4d4
003b642c: mov      r0, r4
003b6430: mov      r1, r7
003b6434: mov      r2, r8
003b6438: bl       #0x319af4
003b643c: ldr      r2, [pc, #0x488]
003b6440: ldr      r7, [pc, #0x488]
003b6444: mov      r3, r5
003b6448: ldr      r8, [r6, r2]
003b644c: add      r7, pc, r7
003b6450: mov      r0, r4
003b6454: mov      r1, r7
003b6458: mov      r2, r8
003b645c: bl       #0x31a4d4
003b6460: mov      r0, r4
003b6464: mov      r1, r7
003b6468: mov      r2, r8
003b646c: bl       #0x319af4
003b6470: ldr      r3, [pc, #0x45c]
003b6474: ldr      r7, [pc, #0x45c]
003b6478: mov      r0, r4
003b647c: ldr      ip, [r6, r3]
003b6480: add      r7, pc, r7
003b6484: mov      r3, r5
003b6488: mov      r1, r7
003b648c: mov      r2, ip
003b6490: str      ip, [sp]
003b6494: ldr      r8, [pc, #0x440]
003b6498: bl       #0x31a4d4
003b649c: ldr      ip, [sp]
003b64a0: ldr      sl, [pc, #0x438]
003b64a4: mov      r0, r4
003b64a8: mov      r2, ip
003b64ac: mov      r1, r7
003b64b0: add      r8, pc, r8
003b64b4: bl       #0x319af4
003b64b8: ldr      r7, [pc, #0x424]
003b64bc: mov      r3, r5
003b64c0: mov      r0, r4
003b64c4: mov      r1, r8
003b64c8: ldr      r2, [sp, #4]
003b64cc: bl       #0x31a4d4
003b64d0: add      sl, pc, sl
003b64d4: mov      r0, r4
003b64d8: mov      r1, r8
003b64dc: ldr      r2, [sp, #4]
003b64e0: bl       #0x319af4
003b64e4: mov      r3, r5
003b64e8: mov      r0, r4
003b64ec: mov      r1, sl
003b64f0: mov      r2, fp
003b64f4: bl       #0x31a4d4
003b64f8: add      r7, pc, r7
003b64fc: mov      r0, r4
003b6500: mov      r1, sl
003b6504: mov      r2, fp
003b6508: bl       #0x319af4
003b650c: mov      r3, r5
003b6510: mov      r0, r4
003b6514: mov      r1, r7
003b6518: mov      r2, sb
003b651c: bl       #0x31a4d4
003b6520: mov      r0, r4
003b6524: mov      r1, r7
003b6528: mov      r2, sb
003b652c: bl       #0x319af4
003b6530: ldr      r3, [pc, #0x3b0]
003b6534: ldr      r7, [pc, #0x3b0]
003b6538: ldr      r8, [pc, #0x3b0]
003b653c: ldr      sl, [r6, r3]
003b6540: add      r7, pc, r7
003b6544: mov      r3, r5
003b6548: mov      r0, r4
003b654c: mov      r1, r7
003b6550: mov      r2, sl
003b6554: bl       #0x31a4d4
003b6558: add      r8, pc, r8
003b655c: mov      r0, r4
003b6560: mov      r1, r7
003b6564: mov      r2, sl
003b6568: bl       #0x319af4
003b656c: mov      r3, r5
003b6570: mov      r0, r4
003b6574: mov      r1, r8
003b6578: mov      r2, sl
003b657c: bl       #0x31a4d4
003b6580: mov      r0, r4
003b6584: mov      r1, r8
003b6588: mov      r2, sl
003b658c: bl       #0x319af4
003b6590: ldr      r2, [pc, #0x35c]
003b6594: ldr      r7, [pc, #0x35c]
003b6598: mov      r3, r5
003b659c: ldr      r8, [r6, r2]
003b65a0: add      r7, pc, r7
003b65a4: mov      r0, r4
003b65a8: mov      r1, r7
003b65ac: mov      r2, r8
003b65b0: bl       #0x31a4d4
003b65b4: mov      r0, r4
003b65b8: mov      r1, r7
003b65bc: mov      r2, r8
003b65c0: bl       #0x319af4
003b65c4: ldr      r2, [pc, #0x330]
003b65c8: ldr      r7, [pc, #0x330]
003b65cc: mov      r3, r5
003b65d0: ldr      r8, [r6, r2]
003b65d4: add      r7, pc, r7
003b65d8: mov      r0, r4
003b65dc: mov      r1, r7
003b65e0: mov      r2, r8
003b65e4: bl       #0x31a4d4
003b65e8: mov      r0, r4
003b65ec: mov      r1, r7
003b65f0: mov      r2, r8
003b65f4: bl       #0x319af4
003b65f8: ldr      r2, [pc, #0x304]
003b65fc: ldr      r7, [pc, #0x304]
003b6600: mov      r3, r5
003b6604: ldr      r8, [r6, r2]
003b6608: add      r7, pc, r7
003b660c: mov      r0, r4
003b6610: mov      r1, r7
003b6614: mov      r2, r8
003b6618: bl       #0x31a4d4
003b661c: mov      r0, r4
003b6620: mov      r1, r7
003b6624: mov      r2, r8
003b6628: bl       #0x319af4
003b662c: ldr      r2, [pc, #0x2d8]
003b6630: ldr      r7, [pc, #0x2d8]
003b6634: mov      r3, r5
003b6638: ldr      r8, [r6, r2]
003b663c: add      r7, pc, r7
003b6640: mov      r0, r4
003b6644: mov      r1, r7
003b6648: mov      r2, r8
003b664c: bl       #0x31a4d4
003b6650: mov      r0, r4
003b6654: mov      r1, r7
003b6658: mov      r2, r8
003b665c: bl       #0x319af4
003b6660: ldr      r2, [pc, #0x2ac]
003b6664: ldr      r7, [pc, #0x2ac]
003b6668: mov      r3, r5
003b666c: ldr      r8, [r6, r2]
003b6670: add      r7, pc, r7
003b6674: mov      r0, r4
003b6678: mov      r1, r7
003b667c: mov      r2, r8
003b6680: bl       #0x31a4d4
003b6684: mov      r0, r4
003b6688: mov      r1, r7
003b668c: mov      r2, r8
003b6690: bl       #0x319af4
003b6694: ldr      r2, [pc, #0x280]
003b6698: ldr      r7, [pc, #0x280]
003b669c: mov      r3, r5
003b66a0: ldr      r8, [r6, r2]
003b66a4: add      r7, pc, r7
003b66a8: mov      r0, r4
003b66ac: mov      r1, r7
003b66b0: mov      r2, r8
003b66b4: bl       #0x31a4d4
003b66b8: mov      r0, r4
003b66bc: b        #0x3b6974
003b66c0: ldrheq   pc, [sp], #-0x38
003b66c4: andeq    r2, r0, r8, ror r6
003b66c8: subseq   lr, r0, r0, asr r8
003b66cc: andeq    r3, r0, r8, asr #6
003b66d0: subseq   lr, r0, r4, lsr r8
003b66d4: strheq   r2, [r0], -r8
003b66d8: subseq   lr, r0, r0, lsl r8
003b66dc: strheq   r2, [r0], -ip
003b66e0: subseq   fp, r0, r4, lsr #29
003b66e4: andeq    r2, r0, r8, lsr #17
003b66e8: ldrheq   lr, [r0], #-0x70
003b66ec: andeq    r4, r0, ip, lsl #24
003b66f0: subseq   lr, r0, r4, lsl #15
003b66f4: andeq    r1, r0, r0, ror #22
003b66f8: subseq   lr, r0, r8, asr r7
003b66fc: andeq    r3, r0, r8, asr #21
003b6700: subseq   lr, r0, ip, lsr #14
003b6704: andeq    r1, r0, r0, lsl #26
003b6708: subseq   lr, r0, r8, lsl #14
003b670c: ldrdeq   r0, r1, [r0], -ip
003b6710: ldrsbeq  lr, [r0], #-0x6c
003b6714: strheq   r2, [r0], -r8
003b6718: ldrheq   lr, [r0], #-0x60
003b671c: strdeq   r1, r2, [r0], -r0
003b6720: subseq   lr, r0, r4, lsl #13
003b6724: strheq   r4, [r0], -r4
003b6728: subseq   lr, r0, r0, ror #12
003b672c: strheq   r2, [r0], -r8
003b6730: subseq   lr, r0, ip, lsr r6
003b6734: andeq    r1, r0, r4, lsr #9
003b6738: subseq   lr, r0, r0, lsl r6
003b673c: strdeq   r4, r5, [r0], -r8
003b6740: subseq   lr, r0, ip, ror #11
003b6744: andeq    r4, r0, r0, ror #14
003b6748: subseq   lr, r0, r8, asr #11
003b674c: strdeq   r0, r1, [r0], -r8
003b6750: subseq   lr, r0, r4, lsr #11
003b6754: andeq    r2, r0, r4, asr #9
003b6758: subseq   lr, r0, r0, lsl #11
003b675c: andeq    r4, r0, r8, lsr r1
003b6760: subseq   lr, r0, ip, asr r5
003b6764: andeq    r1, r0, r4, lsl #16
003b6768: subseq   lr, r0, r8, lsr r5
003b676c: andeq    r0, r0, r0, lsl lr
003b6770: subseq   lr, r0, r4, lsl r5
003b6774: strdeq   r1, r2, [r0], -r8
003b6778: ldrsheq  lr, [r0], #-0x40
003b677c: ldrsheq  lr, [r0], #-0x40
003b6780: muleq    r0, r0, r7
003b6784: ldrheq   lr, [r0], #-0x48
003b6788: ldrdeq   r0, r1, [r0], -ip

# _ZN3sfc6script3lua8Instance11includeBaseEv
0031b010: ldr      r0, [r0, #4]
0031b014: b        #0x84dbfc

# lua_atpanic
0084b11c: ldr      r3, [r0, #0x10]
0084b120: ldr      r0, [r3, #0x58]
0084b124: str      r1, [r3, #0x58]
0084b128: bx       lr
