
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

# _ZN9LuaScript7_GetIntERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ec14: push     {r4, r5, r6, lr}
0037ec18: ldr      r3, [r0, #4]
0037ec1c: mov      r5, r2
0037ec20: mov      r4, r1
0037ec24: ldm      r3, {r0, r2}
0037ec28: rsb      r3, r0, r2
0037ec2c: asr      r3, r3, #4
0037ec30: add      r2, r3, r3, lsl #3
0037ec34: add      r2, r2, r2, lsl #6
0037ec38: add      r2, r3, r2, lsl #3
0037ec3c: add      r2, r2, r2, lsl #15
0037ec40: add      r3, r3, r2, lsl #3
0037ec44: cmp      r3, #0
0037ec48: bne      #0x37ec50
0037ec4c: pop      {r4, r5, r6, pc}
0037ec50: bl       #0x31c49c
0037ec54: mov      r1, r0
0037ec58: mov      r0, r5
0037ec5c: bl       #0x37da30
0037ec60: mov      r1, r0
0037ec64: mov      r0, r4
0037ec68: pop      {r4, r5, r6, lr}
0037ec6c: b        #0x37cb24

# _ZNK9LuaScript6GetIntEPKc
0037da30: push     {r4, r5, r6, lr}
0037da34: mov      r4, r0
0037da38: mov      r0, r1
0037da3c: mov      r5, r1
0037da40: bl       #0x37c164
0037da44: ldr      r3, [r4, #0x20]
0037da48: add      ip, r4, #0x1c
0037da4c: cmp      r3, #0
0037da50: beq      #0x37daa4
0037da54: mov      r1, ip
0037da58: b        #0x37da60
0037da5c: mov      r3, r2
0037da60: ldr      r2, [r3, #0x10]
0037da64: cmp      r0, r2
0037da68: ldrhi    r2, [r3, #0xc]
0037da6c: ldrls    r2, [r3, #8]
0037da70: movhi    r3, r1
0037da74: mov      r1, r3
0037da78: cmp      r2, #0
0037da7c: bne      #0x37da5c
0037da80: cmp      ip, r3
0037da84: beq      #0x37daac
0037da88: ldr      r2, [r3, #0x10]
0037da8c: cmp      r0, r2
0037da90: blo      #0x37daa4
0037da94: cmp      ip, r3
0037da98: beq      #0x37daac
0037da9c: ldr      r0, [r3, #0x14]
0037daa0: pop      {r4, r5, r6, pc}
0037daa4: mov      r3, ip
0037daa8: b        #0x37da94
0037daac: mov      r0, r4
0037dab0: mov      r1, r5
0037dab4: mov      r2, #0
0037dab8: bl       #0x37d990
0037dabc: mov      r0, #0
0037dac0: pop      {r4, r5, r6, pc}

# _ZN9LuaScript7_SetIntERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037de5c: push     {r4, r5, r6, r7, r8, lr}
0037de60: ldr      r4, [r0, #4]
0037de64: mov      r5, r0
0037de68: mov      r6, r2
0037de6c: ldm      r4, {r0, r3}
0037de70: rsb      r3, r0, r3
0037de74: asr      r3, r3, #4
0037de78: add      r2, r3, r3, lsl #3
0037de7c: add      r2, r2, r2, lsl #6
0037de80: add      r2, r3, r2, lsl #3
0037de84: add      r2, r2, r2, lsl #15
0037de88: add      r3, r3, r2, lsl #3
0037de8c: rsb      r3, r3, #0
0037de90: cmp      r3, #1
0037de94: bls      #0x37df24
0037de98: cmp      r3, #0
0037de9c: beq      #0x37df10
0037dea0: bl       #0x31c49c
0037dea4: ldr      r4, [r5, #4]
0037dea8: mov      r7, r0
0037deac: ldm      r4, {r0, r3}
0037deb0: rsb      r3, r0, r3
0037deb4: asr      r3, r3, #4
0037deb8: add      r2, r3, r3, lsl #3
0037debc: add      r2, r2, r2, lsl #6
0037dec0: add      r2, r3, r2, lsl #3
0037dec4: add      r2, r2, r2, lsl #15
0037dec8: add      r3, r3, r2, lsl #3
0037decc: rsb      r3, r3, #0
0037ded0: cmp      r3, #1
0037ded4: bls      #0x37defc
0037ded8: add      r0, r0, #0x70
0037dedc: bl       #0x31bbf0
0037dee0: bl       #0x30e4cc
0037dee4: mov      r3, r0
0037dee8: mov      r1, r7
0037deec: mov      r0, r6
0037def0: mov      r2, r3
0037def4: pop      {r4, r5, r6, r7, r8, lr}
0037def8: b        #0x37d990
0037defc: ldr      r0, [pc, #0x24]
0037df00: add      r0, pc, r0
0037df04: bl       #0x708eb0
0037df08: ldr      r0, [r4]
0037df0c: b        #0x37ded8
0037df10: ldr      r0, [pc, #0x14]
0037df14: add      r0, pc, r0
0037df18: bl       #0x708eb0
0037df1c: ldr      r0, [r4]
0037df20: b        #0x37dea0
0037df24: pop      {r4, r5, r6, r7, r8, pc}
0037df28: subseq   r0, r4, r8, ror #10
0037df2c: subseq   r0, r4, r4, asr r5

# _ZNK3sfc6script3lua5Value9getNumberEv
0031bbf0: push     {r4, r5, r6, lr}
0031bbf4: ldr      r3, [r0, #4]
0031bbf8: mov      r5, r0
0031bbfc: cmp      r3, #0
0031bc00: beq      #0x31bc2c
0031bc04: cmp      r3, #1
0031bc08: beq      #0x31bc38
0031bc0c: cmp      r3, #3
0031bc10: beq      #0x31bc38
0031bc14: cmp      r3, #2
0031bc18: beq      #0x31bc44
0031bc1c: cmp      r3, #7
0031bc20: beq      #0x31bc44
0031bc24: cmp      r3, #4
0031bc28: beq      #0x31bc54
0031bc2c: mov      r5, #0
0031bc30: mov      r0, r5
0031bc34: pop      {r4, r5, r6, pc}
0031bc38: ldr      r5, [r5, #8]
0031bc3c: mov      r0, r5
0031bc40: pop      {r4, r5, r6, pc}
0031bc44: ldr      r0, [r5, #0x6c]
0031bc48: bl       #0x30e2e0
0031bc4c: mov      r5, r0
0031bc50: b        #0x31bc30
0031bc54: bl       #0x84c7e0
0031bc58: ldr      r1, [r5, #0x20]
0031bc5c: mov      r4, r0
0031bc60: bl       #0x84c04c
0031bc64: mov      r0, r4
0031bc68: mvn      r1, #0
0031bc6c: bl       #0x84c450
0031bc70: mov      r5, r0
0031bc74: mov      r0, r4
0031bc78: bl       #0x85797c
0031bc7c: b        #0x31bc30

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

# _ZN3sfc6script3lua5ErrorD1Ev
0031a68c: ldr      r3, [pc, #0x24]
0031a690: ldr      r2, [pc, #0x24]
0031a694: push     {r4, lr}
0031a698: add      r3, pc, r3
0031a69c: ldr      r2, [r3, r2]
0031a6a0: mov      r4, r0
0031a6a4: add      r2, r2, #8
0031a6a8: str      r2, [r0], #8
0031a6ac: bl       #0x3139ac
0031a6b0: mov      r0, r4
0031a6b4: pop      {r4, pc}

# _ZN10LuaManager7AddFileEP9LuaScriptPKc
0037b23c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037b240: ldr      r4, [pc, #0x2e0]
0037b244: ldr      r6, [pc, #0x2e0]
0037b248: sub      sp, sp, #0x7c
0037b24c: add      r4, pc, r4
0037b250: ldr      r3, [r4, r6]
0037b254: subs     r7, r1, #0
0037b258: mov      sl, r0
0037b25c: ldr      r3, [r3]
0037b260: mov      r5, r2
0037b264: str      r3, [sp, #0x74]
0037b268: beq      #0x37b318
0037b26c: cmp      r5, #0
0037b270: beq      #0x37b280
0037b274: ldrsb    r3, [r5]
0037b278: cmp      r3, #0
0037b27c: bne      #0x37b2a4
0037b280: mov      r5, #0
0037b284: ldr      r3, [r4, r6]
0037b288: ldr      r2, [sp, #0x74]
0037b28c: mov      r0, r5
0037b290: ldr      r3, [r3]
0037b294: cmp      r2, r3
0037b298: bne      #0x37b524
0037b29c: add      sp, sp, #0x7c
0037b2a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037b2a4: add      r8, sp, #0x5c
0037b2a8: mov      r0, r8
0037b2ac: add      r1, r7, #0x68
0037b2b0: mov      r2, r5
0037b2b4: bl       #0x3338cc
0037b2b8: ldr      r1, [pc, #0x270]
0037b2bc: mov      r0, r5
0037b2c0: add      r1, pc, r1
0037b2c4: bl       #0x30ebd4
0037b2c8: cmp      r0, #0
0037b2cc: beq      #0x37b3f8
0037b2d0: ldr      r1, [pc, #0x25c]
0037b2d4: mov      r2, #5
0037b2d8: add      r1, pc, r1
0037b2dc: bl       #0x30ec7c
0037b2e0: cmp      r0, #0
0037b2e4: bne      #0x37b36c
0037b2e8: ldr      r3, [sp, #0x70]
0037b2ec: add      r1, sp, #0x78
0037b2f0: add      r5, r7, #0x80
0037b2f4: str      r3, [r1, #-0x5c]!
0037b2f8: mov      r0, r5
0037b2fc: bl       #0x37a190
0037b300: cmp      r5, r0
0037b304: beq      #0x37b380
0037b308: mov      r5, #1
0037b30c: mov      r0, r8
0037b310: bl       #0x3139ac
0037b314: b        #0x37b284
0037b318: ldr      r3, [pc, #0x218]
0037b31c: ldr      r3, [r4, r3]
0037b320: ldr      r3, [r3]
0037b324: cmp      r3, #2
0037b328: streq    r7, [r7]
0037b32c: beq      #0x37b26c
0037b330: cmp      r3, #1
0037b334: bne      #0x37b26c
0037b338: ldr      r0, [pc, #0x1fc]
0037b33c: ldr      r1, [pc, #0x1fc]
0037b340: ldr      r2, [pc, #0x1fc]
0037b344: ldr      r0, [r4, r0]
0037b348: ldr      r3, [pc, #0x1f8]
0037b34c: mov      ip, #0x23
0037b350: add      r1, pc, r1
0037b354: add      r2, pc, r2
0037b358: add      r3, pc, r3
0037b35c: add      r0, r0, #0xa8
0037b360: str      ip, [sp]
0037b364: bl       #0x30e004
0037b368: b        #0x37b26c
0037b36c: ldr      r1, [pc, #0x1d8]
0037b370: mov      r0, r8
0037b374: add      r1, pc, r1
0037b378: bl       #0x379ef8
0037b37c: b        #0x37b2e8
0037b380: ldr      r3, [sp, #0x70]
0037b384: add      r1, sp, #0x78
0037b388: add      sl, sl, #4
0037b38c: str      r3, [r1, #-0x60]!
0037b390: mov      r0, sl
0037b394: bl       #0x37a300
0037b398: cmp      r0, sl
0037b39c: mov      sb, r0
0037b3a0: beq      #0x37b444
0037b3a4: ldr      sl, [r0, #0x28]
0037b3a8: mov      r2, #0
0037b3ac: mov      r3, #0
0037b3b0: ldr      r1, [sl]
0037b3b4: mov      r0, sl
0037b3b8: mov      lr, pc
0037b3bc: ldr      pc, [r1, #0x20]
0037b3c0: cmp      sl, #0
0037b3c4: beq      #0x37b4d0
0037b3c8: add      sb, sp, #0x24
0037b3cc: add      r1, r7, #4
0037b3d0: mov      r2, sl
0037b3d4: mov      r0, sb
0037b3d8: bl       #0x31acf4
0037b3dc: ldr      r3, [sp, #0x28]
0037b3e0: cmp      r3, #0
0037b3e4: beq      #0x37b40c
0037b3e8: mov      r0, sb
0037b3ec: bl       #0x31a68c
0037b3f0: mov      r5, #0
0037b3f4: b        #0x37b30c
0037b3f8: ldr      r1, [pc, #0x150]
0037b3fc: mov      r0, r8
0037b400: add      r1, pc, r1
0037b404: bl       #0x379ef8
0037b408: b        #0x37b2e8
0037b40c: add      r7, sp, #0x44
0037b410: mov      r0, sb
0037b414: bl       #0x31a68c
0037b418: ldr      r1, [sp, #0x70]
0037b41c: add      r2, sp, #0x20
0037b420: mov      r0, r7
0037b424: bl       #0x3140ec
0037b428: add      r0, sp, #8
0037b42c: mov      r1, r5
0037b430: mov      r2, r7
0037b434: bl       #0x37a9e8
0037b438: mov      r0, r7
0037b43c: bl       #0x3139ac
0037b440: b        #0x37b308
0037b444: ldr      r3, [pc, #0x108]
0037b448: mov      r2, #0
0037b44c: ldr      r1, [sp, #0x70]
0037b450: ldr      fp, [r4, r3]
0037b454: mov      r3, r2
0037b458: ldr      r0, [fp, #0x10]
0037b45c: ldr      ip, [r0, #0x34]
0037b460: mov      r0, ip
0037b464: ldr      ip, [ip]
0037b468: mov      lr, pc
0037b46c: ldr      pc, [ip, #0x88]
0037b470: cmp      r0, #0
0037b474: str      r0, [sp, #0x14]
0037b478: moveq    r5, r0
0037b47c: beq      #0x37b30c
0037b480: mov      r1, #0
0037b484: mov      r0, #0x30
0037b488: bl       #0x310570
0037b48c: ldr      r1, [sp, #0x14]
0037b490: mov      sl, r0
0037b494: bl       #0x3172d8
0037b498: ldr      r3, [sp, #0x70]
0037b49c: add      r1, sp, #0x78
0037b4a0: mov      r0, sb
0037b4a4: str      r3, [r1, #-0x68]!
0037b4a8: bl       #0x37b0fc
0037b4ac: str      sl, [r0]
0037b4b0: ldr      r3, [fp, #0x10]
0037b4b4: add      r1, sp, #0x14
0037b4b8: ldr      r3, [r3, #0x34]
0037b4bc: mov      r0, r3
0037b4c0: ldr      r3, [r3]
0037b4c4: mov      lr, pc
0037b4c8: ldr      pc, [r3, #0x78]
0037b4cc: b        #0x37b3c0
0037b4d0: ldr      r3, [pc, #0x60]
0037b4d4: ldr      r3, [r4, r3]
0037b4d8: ldr      r3, [r3]
0037b4dc: cmp      r3, #2
0037b4e0: streq    sl, [sl]
0037b4e4: beq      #0x37b3c8
0037b4e8: cmp      r3, #1
0037b4ec: bne      #0x37b3c8
0037b4f0: ldr      r0, [pc, #0x44]
0037b4f4: ldr      r1, [pc, #0x5c]
0037b4f8: ldr      r2, [pc, #0x5c]
0037b4fc: ldr      r0, [r4, r0]
0037b500: ldr      r3, [pc, #0x58]
0037b504: mov      ip, #0x61
0037b508: add      r1, pc, r1
0037b50c: add      r2, pc, r2
0037b510: add      r3, pc, r3
0037b514: add      r0, r0, #0xa8
0037b518: str      ip, [sp]
0037b51c: bl       #0x30e004
0037b520: b        #0x37b3c8
0037b524: bl       #0x30e310
0037b528: rsbeq    sb, r1, r4, asr #16
0037b52c: andeq    r4, r0, ip, lsr #1
0037b530: subseq   r6, r4, r0, ror r7
0037b534: subseq   r6, r4, r0, ror #14
0037b538: andeq    r3, r0, r0, asr #19
0037b53c: andeq    r1, r0, r0, asr #19
0037b540: subseq   r3, r4, r8, lsl #1
0037b544: subseq   r6, r4, r4, lsl #13
0037b548: subseq   r6, r4, r8, lsl #13
0037b54c: subseq   r6, r7, r4, lsl sp
0037b550: subseq   r6, r4, r8, lsr r6
0037b554: strdeq   r3, r4, [r0], -r4
0037b558: ldrsbeq  r2, [r4], #-0xe0
0037b55c: subseq   r6, r4, r4, lsr r5
0037b560: ldrsbeq  r6, [r4], #-0x40

# _ZN9LuaScript8_IncludeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037efe4: push     {r4, lr}
0037efe8: ldr      r3, [r0, #4]
0037efec: mov      r4, r2
0037eff0: ldm      r3, {r0, r2}
0037eff4: rsb      r3, r0, r2
0037eff8: asr      r3, r3, #4
0037effc: add      r2, r3, r3, lsl #3
0037f000: add      r2, r2, r2, lsl #6
0037f004: add      r2, r3, r2, lsl #3
0037f008: add      r2, r2, r2, lsl #15
0037f00c: add      r3, r3, r2, lsl #3
0037f010: cmp      r3, #0
0037f014: bne      #0x37f01c
0037f018: pop      {r4, pc}
0037f01c: ldr      r3, [r0, #4]
0037f020: cmp      r3, #4
0037f024: bne      #0x37f018
0037f028: bl       #0x31c49c
0037f02c: mov      r1, r0
0037f030: mov      r0, r4
0037f034: pop      {r4, lr}
0037f038: b        #0x37b574

# _ZN9LuaScript4LoadEPKc
0037b574: ldr      r3, [pc, #0x1c]
0037b578: ldr      r2, [pc, #0x1c]
0037b57c: mov      ip, r0
0037b580: add      r3, pc, r3
0037b584: ldr      r0, [r3, r2]
0037b588: mov      r2, r1
0037b58c: mov      r1, ip
0037b590: ldr      r0, [r0, #0x3c]
0037b594: b        #0x37b23c
0037b598: rsbeq    sb, r1, r0, lsl r5
0037b59c: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScript6SetIntEPKci
0037d990: push     {r4, r5, r6, lr}
0037d994: mov      r6, r0
0037d998: sub      sp, sp, #0x10
0037d99c: mov      r0, r1
0037d9a0: mov      r5, r2
0037d9a4: bl       #0x37c164
0037d9a8: ldr      ip, [r6, #0x20]
0037d9ac: add      r1, r6, #0x1c
0037d9b0: mov      r4, r0
0037d9b4: cmp      ip, #0
0037d9b8: moveq    ip, r1
0037d9bc: beq      #0x37d9ec
0037d9c0: mov      r2, r1
0037d9c4: b        #0x37d9cc
0037d9c8: mov      ip, r3
0037d9cc: ldr      r3, [ip, #0x10]
0037d9d0: cmp      r4, r3
0037d9d4: ldrhi    r3, [ip, #0xc]
0037d9d8: ldrls    r3, [ip, #8]
0037d9dc: movhi    ip, r2
0037d9e0: mov      r2, ip
0037d9e4: cmp      r3, #0
0037d9e8: bne      #0x37d9c8
0037d9ec: cmp      r1, ip
0037d9f0: beq      #0x37da04
0037d9f4: ldr      r2, [ip, #0x10]
0037d9f8: mov      r3, ip
0037d9fc: cmp      r4, r2
0037da00: bhs      #0x37da24
0037da04: mov      r3, sp
0037da08: mov      lr, #0
0037da0c: add      r0, sp, #8
0037da10: add      r2, sp, #0xc
0037da14: stm      sp, {r4, lr}
0037da18: str      ip, [sp, #0xc]
0037da1c: bl       #0x37d61c
0037da20: ldr      r3, [sp, #8]
0037da24: str      r5, [r3, #0x14]
0037da28: add      sp, sp, #0x10
0037da2c: pop      {r4, r5, r6, pc}

# _ZN3sfc6script3lua5ErrorC1Ev
0031a804: ldr      r2, [pc, #0x44]
0031a808: ldr      r1, [pc, #0x44]
0031a80c: mov      r3, r0
0031a810: add      r2, pc, r2
0031a814: ldr      r1, [r2, r1]
0031a818: push     {r4, lr}
0031a81c: add      r1, r1, #8
0031a820: mov      r4, r0
0031a824: str      r1, [r3], #8
0031a828: mov      r0, r3
0031a82c: str      r3, [r4, #0x18]
0031a830: str      r3, [r4, #0x1c]
0031a834: bl       #0x31a710
0031a838: ldr      r2, [r4, #0x18]
0031a83c: mov      r3, #0
0031a840: mov      r0, r4
0031a844: strb     r3, [r2]
0031a848: str      r3, [r4, #4]
0031a84c: pop      {r4, pc}
0031a850: rsbeq    sl, r7, r0, lsl #5
0031a854: muleq    r0, r8, r4

# _ZN3sfc6script3lua5Error8setErrorEP9lua_Statei
0031a8ac: cmp      r2, #0
0031a8b0: push     {r4, r5, r6, lr}
0031a8b4: mov      r4, r0
0031a8b8: mov      r5, r1
0031a8bc: str      r2, [r0, #4]
0031a8c0: bne      #0x31a8dc
0031a8c4: ldr      r1, [pc, #0x48]
0031a8c8: add      r0, r0, #8
0031a8cc: add      r1, pc, r1
0031a8d0: mov      r2, r1
0031a8d4: pop      {r4, r5, r6, lr}
0031a8d8: b        #0x3109e0
0031a8dc: mvn      r1, #0
0031a8e0: mov      r2, #0
0031a8e4: mov      r0, r5
0031a8e8: bl       #0x84c384
0031a8ec: mov      r6, r0
0031a8f0: bl       #0x30de54
0031a8f4: mov      r1, r6
0031a8f8: add      r2, r6, r0
0031a8fc: add      r0, r4, #8
0031a900: bl       #0x3109e0
0031a904: mov      r0, r5
0031a908: mvn      r1, #1
0031a90c: pop      {r4, r5, r6, lr}
0031a910: b        #0x84b140
0031a914: subseq   r0, fp, ip, lsr pc
