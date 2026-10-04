
# _ZN12StreamBufferC1EP11IStreamBase
003172d8: push     {r4, r5, r6, r7, lr}
003172dc: ldr      r5, [pc, #0xf8]
003172e0: ldr      r3, [pc, #0xf8]
003172e4: mov      r6, #0
003172e8: add      r5, pc, r5
003172ec: ldr      r3, [r5, r3]
003172f0: mov      r7, #0
003172f4: strd     r6, r7, [r0, #0x10]
003172f8: strd     r6, r7, [r0, #8]
003172fc: add      r3, r3, #8
00317300: mov      r2, #0
00317304: str      r3, [r0]
00317308: mov      r3, #0x800
0031730c: strb     r2, [r0, #0x2c]
00317310: str      r2, [r0, #0x1c]
00317314: str      r2, [r0, #0x20]
00317318: str      r2, [r0, #0x24]
0031731c: str      r2, [r0, #0x28]
00317320: str      r3, [r0, #0x18]
00317324: mov      r4, r0
00317328: sub      sp, sp, #0xc
0031732c: ldr      r3, [r1]
00317330: mov      r0, r1
00317334: mov      r6, r1
00317338: mov      lr, pc
0031733c: ldr      pc, [r3, #8]
00317340: mov      r2, r0
00317344: mov      r3, r1
00317348: mov      r0, r4
0031734c: bl       #0x317180
00317350: ldrb     r3, [r4, #0x2c]
00317354: ldr      r2, [r6]
00317358: cmp      r3, #0
0031735c: ldr      r7, [r2, #0x18]
00317360: bne      #0x317384
00317364: ldr      r2, [pc, #0x78]
00317368: ldr      r2, [r5, r2]
0031736c: ldr      r2, [r2]
00317370: cmp      r2, #2
00317374: streq    r3, [r3]
00317378: beq      #0x317384
0031737c: cmp      r2, #1
00317380: beq      #0x3173a8
00317384: ldr      r3, [r4, #0x1c]
00317388: mov      r0, r6
0031738c: ldr      r2, [r4, #0x28]
00317390: ldr      r1, [r3]
00317394: mov      r3, #0
00317398: blx      r7
0031739c: mov      r0, r4
003173a0: add      sp, sp, #0xc
003173a4: pop      {r4, r5, r6, r7, pc}
003173a8: ldr      r0, [pc, #0x38]
003173ac: ldr      r1, [pc, #0x38]
003173b0: ldr      r2, [pc, #0x38]
003173b4: ldr      r0, [r5, r0]
003173b8: ldr      r3, [pc, #0x34]
003173bc: mov      ip, #0x82
003173c0: add      r1, pc, r1
003173c4: add      r2, pc, r2
003173c8: add      r3, pc, r3
003173cc: add      r0, r0, #0xa8
003173d0: str      ip, [sp]
003173d4: bl       #0x30e004
003173d8: b        #0x317384
003173dc: rsbeq    sp, r7, r8, lsr #15
003173e0: andeq    r0, r0, r4, ror fp
003173e4: andeq    r3, r0, r0, asr #19
003173e8: andeq    r1, r0, r0, asr #19
003173ec: subseq   r7, sl, r8, lsl r0
003173f0: subseq   r7, sl, r4, lsl #4
003173f4: subseq   r7, sl, r8, ror r2

# _ZN3sfc6script3lua8Instance12_chunkReaderEP9lua_StatePvPj
0031b018: push     {r4, r5, r6, r7, lr}
0031b01c: ldr      r3, [r1, #4]
0031b020: mov      r4, r1
0031b024: ldr      r1, [pc, #0x13c]
0031b028: cmp      r3, #0
0031b02c: sub      sp, sp, #0xc
0031b030: mov      r6, r2
0031b034: add      r1, pc, r1
0031b038: bne      #0x31b078
0031b03c: ldr      r5, [r4, #8]
0031b040: cmp      r5, #0
0031b044: bne      #0x31b0e8
0031b048: ldr      r3, [pc, #0x11c]
0031b04c: ldr      r3, [r1, r3]
0031b050: ldr      r3, [r3]
0031b054: cmp      r3, #2
0031b058: streq    r5, [r5]
0031b05c: moveq    r0, r5
0031b060: beq      #0x31b070
0031b064: cmp      r3, #1
0031b068: beq      #0x31b130
0031b06c: mov      r0, #0
0031b070: add      sp, sp, #0xc
0031b074: pop      {r4, r5, r6, r7, pc}
0031b078: mov      r0, r3
0031b07c: ldr      r3, [r3]
0031b080: mov      lr, pc
0031b084: ldr      pc, [r3, #0x24]
0031b088: ldr      r3, [r4, #4]
0031b08c: mov      r5, r0
0031b090: mov      r7, r1
0031b094: mov      r0, r3
0031b098: ldr      r3, [r3]
0031b09c: mov      lr, pc
0031b0a0: ldr      pc, [r3, #8]
0031b0a4: cmp      r5, r0
0031b0a8: beq      #0x31b0d8
0031b0ac: ldr      r3, [r4, #4]
0031b0b0: mov      r0, r3
0031b0b4: ldr      ip, [r3]
0031b0b8: ldr      r1, [r4, #0xc]
0031b0bc: ldr      r2, [r4, #0x10]
0031b0c0: mov      r3, #0
0031b0c4: mov      lr, pc
0031b0c8: ldr      pc, [ip, #0x18]
0031b0cc: str      r0, [r6]
0031b0d0: ldr      r0, [r4, #0xc]
0031b0d4: b        #0x31b070
0031b0d8: cmp      r7, r1
0031b0dc: bne      #0x31b0ac
0031b0e0: mov      r0, #0
0031b0e4: b        #0x31b070
0031b0e8: ldr      r3, [r5]
0031b0ec: mov      r0, r5
0031b0f0: mov      lr, pc
0031b0f4: ldr      pc, [r3, #0x24]
0031b0f8: ldr      r3, [r4, #8]
0031b0fc: mov      r5, r0
0031b100: mov      r7, r1
0031b104: mov      r0, r3
0031b108: ldr      r3, [r3]
0031b10c: mov      lr, pc
0031b110: ldr      pc, [r3, #8]
0031b114: cmp      r5, r0
0031b118: ldrne    r3, [r4, #8]
0031b11c: bne      #0x31b0b0
0031b120: cmp      r7, r1
0031b124: beq      #0x31b06c
0031b128: ldr      r3, [r4, #8]
0031b12c: b        #0x31b0b0
0031b130: ldr      r0, [pc, #0x38]
0031b134: ldr      r2, [pc, #0x38]
0031b138: ldr      r3, [pc, #0x38]
0031b13c: ldr      r0, [r1, r0]
0031b140: ldr      r1, [pc, #0x34]
0031b144: mov      ip, #0x5e
0031b148: add      r0, r0, #0xa8
0031b14c: add      r1, pc, r1
0031b150: add      r2, pc, r2
0031b154: add      r3, pc, r3
0031b158: str      ip, [sp]
0031b15c: bl       #0x30e004
0031b160: mov      r0, r5
0031b164: b        #0x31b070
0031b168: rsbeq    sb, r7, ip, asr sl
0031b16c: andeq    r3, r0, r0, asr #19
0031b170: andeq    r1, r0, r0, asr #19
0031b174: subseq   r3, sl, r8, lsr #14
0031b178: subseq   r3, sl, r4, asr #14
0031b17c: subseq   r3, sl, ip, lsl #5
