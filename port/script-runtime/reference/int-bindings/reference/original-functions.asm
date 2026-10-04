
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

# _ZN6glitch4core10hashStringEPKc
0037c164: push     {r4, r5, r6, r7, lr}
0037c168: ldr      r4, [pc, #0x118]
0037c16c: ldr      r3, [pc, #0x118]
0037c170: ldr      r7, [pc, #0x118]
0037c174: add      r4, pc, r4
0037c178: ldr      r5, [r4, r3]
0037c17c: ldr      r3, [r4, r7]
0037c180: sub      sp, sp, #0x24
0037c184: ldr      r2, [r5]
0037c188: ldr      r3, [r3]
0037c18c: mov      r6, r0
0037c190: tst      r2, #1
0037c194: str      r3, [sp, #0x1c]
0037c198: beq      #0x37c244
0037c19c: add      r5, sp, #4
0037c1a0: mov      r0, r6
0037c1a4: str      r5, [sp, #0x14]
0037c1a8: str      r5, [sp, #0x18]
0037c1ac: bl       #0x30de54
0037c1b0: mov      r1, r6
0037c1b4: add      r2, r6, r0
0037c1b8: mov      r0, r5
0037c1bc: bl       #0x3116e8
0037c1c0: ldr      r0, [sp, #0x18]
0037c1c4: ldr      ip, [sp, #0x14]
0037c1c8: cmp      r0, ip
0037c1cc: moveq    r6, #0
0037c1d0: beq      #0x37c200
0037c1d4: mov      r2, r0
0037c1d8: mov      r6, #0
0037c1dc: ldrsb    r1, [r2], #1
0037c1e0: movw     r3, #0x79b9
0037c1e4: movt     r3, #0x9e37
0037c1e8: add      r3, r1, r3
0037c1ec: add      r3, r3, r6, lsl #6
0037c1f0: add      r3, r3, r6, lsr #2
0037c1f4: cmp      r2, ip
0037c1f8: eor      r6, r6, r3
0037c1fc: bne      #0x37c1dc
0037c200: cmp      r0, r5
0037c204: beq      #0x37c224
0037c208: cmp      r0, #0
0037c20c: beq      #0x37c224
0037c210: ldr      r1, [sp, #4]
0037c214: rsb      r1, r0, r1
0037c218: cmp      r1, #0x80
0037c21c: bhi      #0x37c27c
0037c220: bl       #0x708f00
0037c224: ldr      r3, [r4, r7]
0037c228: ldr      r2, [sp, #0x1c]
0037c22c: mov      r0, r6
0037c230: ldr      r3, [r3]
0037c234: cmp      r2, r3
0037c238: bne      #0x37c284
0037c23c: add      sp, sp, #0x24
0037c240: pop      {r4, r5, r6, r7, pc}
0037c244: mov      r0, r5
0037c248: bl       #0x30e76c
0037c24c: cmp      r0, #0
0037c250: beq      #0x37c19c
0037c254: mov      r0, r5
0037c258: bl       #0x30ea3c
0037c25c: ldr      r3, [pc, #0x30]
0037c260: ldr      r0, [r4, r3]
0037c264: ldr      r3, [pc, #0x2c]
0037c268: ldr      r1, [r4, r3]
0037c26c: ldr      r3, [pc, #0x28]
0037c270: ldr      r2, [r4, r3]
0037c274: bl       #0x30e304
0037c278: b        #0x37c19c
0037c27c: bl       #0x310440
0037c280: b        #0x37c224
0037c284: bl       #0x30e310
0037c288: rsbeq    r8, r1, ip, lsl sb
0037c28c: muleq    r0, ip, sb
0037c290: andeq    r4, r0, ip, lsr #1
0037c294: andeq    r1, r0, r4, ror #13
0037c298: andeq    r2, r0, r4, lsr #14
0037c29c: muleq    r0, r0, r8

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

# _ZN3sfc6script3lua12ReturnValues11pushIntegerEi
0037cb24: ldr      r3, [pc, #0x58]
0037cb28: ldr      r2, [pc, #0x58]
0037cb2c: push     {r4, r5, r6, lr}
0037cb30: add      r3, pc, r3
0037cb34: ldr      r5, [r3, r2]
0037cb38: sub      sp, sp, #0x78
0037cb3c: add      r4, sp, #4
0037cb40: ldr      r3, [r5]
0037cb44: str      r3, [sp, #0x74]
0037cb48: ldr      r6, [r0, #0x24]
0037cb4c: mov      r0, r4
0037cb50: bl       #0x37ca9c
0037cb54: mov      r0, r6
0037cb58: mov      r1, r4
0037cb5c: bl       #0x3195c0
0037cb60: mov      r0, r4
0037cb64: bl       #0x3193e8
0037cb68: ldr      r2, [sp, #0x74]
0037cb6c: ldr      r3, [r5]
0037cb70: cmp      r2, r3
0037cb74: bne      #0x37cb80
0037cb78: add      sp, sp, #0x78
0037cb7c: pop      {r4, r5, r6, pc}
0037cb80: bl       #0x30e310
0037cb84: rsbeq    r7, r1, r0, ror #30
0037cb88: andeq    r4, r0, ip, lsr #1

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
