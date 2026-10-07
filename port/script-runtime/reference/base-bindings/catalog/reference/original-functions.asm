
# _ZN9LuaScript7_BitAndERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e9ec: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037e9f0: ldr      r4, [r0, #4]
0037e9f4: mov      sl, r1
0037e9f8: sub      sp, sp, #4
0037e9fc: ldm      r4, {r2, r3}
0037ea00: mov      r5, r0
0037ea04: rsb      r3, r2, r3
0037ea08: asr      r1, r3, #4
0037ea0c: add      r8, r1, r1, lsl #3
0037ea10: add      r8, r8, r8, lsl #6
0037ea14: add      r8, r1, r8, lsl #3
0037ea18: add      r8, r8, r8, lsl #15
0037ea1c: add      r8, r1, r8, lsl #3
0037ea20: rsb      r8, r8, #0
0037ea24: cmp      r8, #1
0037ea28: bls      #0x37ebb0
0037ea2c: ldr      sb, [pc, #0x184]
0037ea30: mov      r7, #0
0037ea34: mov      r6, r7
0037ea38: add      sb, pc, sb
0037ea3c: b        #0x37ea4c
0037ea40: ldr      r4, [r5, #4]
0037ea44: ldm      r4, {r2, r3}
0037ea48: rsb      r3, r2, r3
0037ea4c: asr      r3, r3, #4
0037ea50: add      r1, r3, r3, lsl #3
0037ea54: add      r1, r1, r1, lsl #6
0037ea58: add      r1, r3, r1, lsl #3
0037ea5c: add      r1, r1, r1, lsl #15
0037ea60: add      r3, r3, r1, lsl #3
0037ea64: rsb      r3, r3, #0
0037ea68: cmp      r6, r3
0037ea6c: add      r6, r6, #1
0037ea70: blo      #0x37ea80
0037ea74: mov      r0, sb
0037ea78: bl       #0x708eb0
0037ea7c: ldr      r2, [r4]
0037ea80: add      r2, r2, r7
0037ea84: ldr      r3, [r2, #4]
0037ea88: add      r7, r7, #0x70
0037ea8c: cmp      r3, #3
0037ea90: bne      #0x37ebb0
0037ea94: cmp      r6, r8
0037ea98: bne      #0x37ea40
0037ea9c: ldr      r4, [r5, #4]
0037eaa0: ldm      r4, {r0, r3}
0037eaa4: rsb      r3, r0, r3
0037eaa8: asr      r3, r3, #4
0037eaac: add      r2, r3, r3, lsl #3
0037eab0: add      r2, r2, r2, lsl #6
0037eab4: add      r2, r3, r2, lsl #3
0037eab8: add      r2, r2, r2, lsl #15
0037eabc: add      r3, r3, r2, lsl #3
0037eac0: cmp      r3, #0
0037eac4: bne      #0x37ead8
0037eac8: ldr      r0, [pc, #0xec]
0037eacc: add      r0, pc, r0
0037ead0: bl       #0x708eb0
0037ead4: ldr      r0, [r4]
0037ead8: bl       #0x31bbf0
0037eadc: bl       #0x30e4cc
0037eae0: ldr      r2, [r5, #4]
0037eae4: mov      r8, r0
0037eae8: ldm      r2, {r2, r3}
0037eaec: rsb      r3, r2, r3
0037eaf0: asr      r3, r3, #4
0037eaf4: add      sb, r3, r3, lsl #3
0037eaf8: add      sb, sb, sb, lsl #6
0037eafc: add      sb, r3, sb, lsl #3
0037eb00: add      sb, sb, sb, lsl #15
0037eb04: add      sb, r3, sb, lsl #3
0037eb08: rsb      sb, sb, #0
0037eb0c: cmp      sb, #1
0037eb10: bls      #0x37eb9c
0037eb14: ldr      fp, [pc, #0xa4]
0037eb18: mov      r7, #0x70
0037eb1c: mov      r4, #1
0037eb20: add      fp, pc, fp
0037eb24: add      r0, r2, r7
0037eb28: bl       #0x31bbf0
0037eb2c: bl       #0x30e4cc
0037eb30: add      r4, r4, #1
0037eb34: cmp      r4, sb
0037eb38: and      r8, r8, r0
0037eb3c: beq      #0x37eb9c
0037eb40: ldr      r6, [r5, #4]
0037eb44: mov      r0, fp
0037eb48: add      r7, r7, #0x70
0037eb4c: ldm      r6, {r2, r3}
0037eb50: rsb      r3, r2, r3
0037eb54: asr      r3, r3, #4
0037eb58: add      r1, r3, r3, lsl #3
0037eb5c: add      r1, r1, r1, lsl #6
0037eb60: add      r1, r3, r1, lsl #3
0037eb64: add      r1, r1, r1, lsl #15
0037eb68: add      r3, r3, r1, lsl #3
0037eb6c: rsb      r3, r3, #0
0037eb70: cmp      r4, r3
0037eb74: blo      #0x37eb24
0037eb78: bl       #0x708eb0
0037eb7c: ldr      r2, [r6]
0037eb80: add      r4, r4, #1
0037eb84: add      r0, r2, r7
0037eb88: bl       #0x31bbf0
0037eb8c: bl       #0x30e4cc
0037eb90: cmp      r4, sb
0037eb94: and      r8, r8, r0
0037eb98: bne      #0x37eb40
0037eb9c: mov      r0, sl
0037eba0: mov      r1, r8
0037eba4: add      sp, sp, #4
0037eba8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037ebac: b        #0x37cb24
0037ebb0: add      sp, sp, #4
0037ebb4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037ebb8: subseq   pc, r3, r0, lsr sl

# _ZN9LuaScript9_MulFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e1a4: push     {r4, r5, r6, lr}
0037e1a8: ldr      r5, [r0, #4]
0037e1ac: mov      r6, r1
0037e1b0: mov      r4, r0
0037e1b4: ldm      r5, {r1, r3}
0037e1b8: rsb      r3, r1, r3
0037e1bc: asr      r3, r3, #4
0037e1c0: add      r2, r3, r3, lsl #3
0037e1c4: add      r2, r2, r2, lsl #6
0037e1c8: add      r2, r3, r2, lsl #3
0037e1cc: add      r2, r2, r2, lsl #15
0037e1d0: add      r3, r3, r2, lsl #3
0037e1d4: rsb      r3, r3, #0
0037e1d8: cmp      r3, #1
0037e1dc: bls      #0x37e2b8
0037e1e0: cmp      r3, #0
0037e1e4: beq      #0x37e2a4
0037e1e8: ldr      r3, [r1, #4]
0037e1ec: cmp      r3, #3
0037e1f0: beq      #0x37e2bc
0037e1f4: ldr      r5, [r4, #4]
0037e1f8: ldm      r5, {r0, r3}
0037e1fc: rsb      r3, r0, r3
0037e200: asr      r3, r3, #4
0037e204: add      r2, r3, r3, lsl #3
0037e208: add      r2, r2, r2, lsl #6
0037e20c: add      r2, r3, r2, lsl #3
0037e210: add      r2, r2, r2, lsl #15
0037e214: add      r3, r3, r2, lsl #3
0037e218: cmp      r3, #0
0037e21c: beq      #0x37e290
0037e220: bl       #0x31bbf0
0037e224: bl       #0x30e4cc
0037e228: ldr      r4, [r4, #4]
0037e22c: mov      r5, r0
0037e230: ldm      r4, {r0, r3}
0037e234: rsb      r3, r0, r3
0037e238: asr      r3, r3, #4
0037e23c: add      r2, r3, r3, lsl #3
0037e240: add      r2, r2, r2, lsl #6
0037e244: add      r2, r3, r2, lsl #3
0037e248: add      r2, r2, r2, lsl #15
0037e24c: add      r3, r3, r2, lsl #3
0037e250: rsb      r3, r3, #0
0037e254: cmp      r3, #1
0037e258: bls      #0x37e27c
0037e25c: add      r0, r0, #0x70
0037e260: bl       #0x31bbf0
0037e264: bl       #0x30e4cc
0037e268: mul      r1, r5, r0
0037e26c: mov      r0, r6
0037e270: asr      r1, r1, #8
0037e274: pop      {r4, r5, r6, lr}
0037e278: b        #0x37cb24
0037e27c: ldr      r0, [pc, #0x48]
0037e280: add      r0, pc, r0
0037e284: bl       #0x708eb0
0037e288: ldr      r0, [r4]
0037e28c: b        #0x37e25c
0037e290: ldr      r0, [pc, #0x38]
0037e294: add      r0, pc, r0
0037e298: bl       #0x708eb0
0037e29c: ldr      r0, [r5]
0037e2a0: b        #0x37e220
0037e2a4: ldr      r0, [pc, #0x28]
0037e2a8: add      r0, pc, r0
0037e2ac: bl       #0x708eb0
0037e2b0: ldr      r1, [r5]
0037e2b4: b        #0x37e1e8
0037e2b8: pop      {r4, r5, r6, pc}
0037e2bc: mov      r0, r4
0037e2c0: mov      r1, #1
0037e2c4: bl       #0x37baf8
0037e2c8: b        #0x37e1f4
0037e2cc: subseq   r0, r4, r8, ror #3
0037e2d0: ldrsbeq  r0, [r4], #-0x14
0037e2d4: subseq   r0, r4, r0, asr #3

# _ZN9LuaScript18_IsPlayerCharacterERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ddc8: ldr      r3, [pc, #0x80]
0037ddcc: ldr      r2, [pc, #0x80]
0037ddd0: push     {r4, r5, r6, lr}
0037ddd4: add      r3, pc, r3
0037ddd8: mov      r5, r0
0037dddc: ldr      r0, [r3, r2]
0037dde0: mov      r4, r1
0037dde4: mov      r2, #1
0037dde8: ldr      r0, [r0, #0x40]
0037ddec: mov      r1, #0
0037ddf0: bl       #0x36e478
0037ddf4: ldr      r5, [r5, #4]
0037ddf8: ldr      r6, [r0, #0x660]
0037ddfc: ldm      r5, {r0, r3}
0037de00: rsb      r3, r0, r3
0037de04: asr      r3, r3, #4
0037de08: add      r2, r3, r3, lsl #3
0037de0c: add      r2, r2, r2, lsl #6
0037de10: add      r2, r3, r2, lsl #3
0037de14: add      r2, r2, r2, lsl #15
0037de18: add      r3, r3, r2, lsl #3
0037de1c: cmp      r3, #0
0037de20: bne      #0x37de34
0037de24: ldr      r0, [pc, #0x2c]
0037de28: add      r0, pc, r0
0037de2c: bl       #0x708eb0
0037de30: ldr      r0, [r5]
0037de34: bl       #0x31b580
0037de38: cmp      r6, r0
0037de3c: movne    r1, #0
0037de40: moveq    r1, #1
0037de44: mov      r0, r4
0037de48: pop      {r4, r5, r6, lr}
0037de4c: b        #0x37c7e4
0037de50: strhteq  r6, [r1], #-0xcc
0037de54: strdeq   r3, r4, [r0], -r4
0037de58: subseq   r0, r4, r0, asr #12

# _ZN9LuaScript12_PushVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037be00: ldr      r3, [r2, #0x5c]
0037be04: push     {r4, r5, r6, lr}
0037be08: cmp      r3, #0
0037be0c: mov      r4, r2
0037be10: beq      #0x37be38
0037be14: add      r5, r2, #0x4c
0037be18: mov      r0, r5
0037be1c: ldr      r1, [r2, #0x50]
0037be20: bl       #0x37bd7c
0037be24: mov      r3, #0
0037be28: str      r5, [r4, #0x58]
0037be2c: str      r3, [r4, #0x5c]
0037be30: str      r5, [r4, #0x54]
0037be34: str      r3, [r4, #0x50]
0037be38: mov      r3, #1
0037be3c: strb     r3, [r4, #0x64]
0037be40: pop      {r4, r5, r6, pc}

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

# _ZN9LuaScript9_DivFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e068: push     {r4, r5, r6, lr}
0037e06c: ldr      r5, [r0, #4]
0037e070: mov      r6, r1
0037e074: mov      r4, r0
0037e078: ldm      r5, {r1, r3}
0037e07c: rsb      r3, r1, r3
0037e080: asr      r3, r3, #4
0037e084: add      r2, r3, r3, lsl #3
0037e088: add      r2, r2, r2, lsl #6
0037e08c: add      r2, r3, r2, lsl #3
0037e090: add      r2, r2, r2, lsl #15
0037e094: add      r3, r3, r2, lsl #3
0037e098: rsb      r3, r3, #0
0037e09c: cmp      r3, #1
0037e0a0: bls      #0x37e184
0037e0a4: cmp      r3, #0
0037e0a8: beq      #0x37e170
0037e0ac: ldr      r3, [r1, #4]
0037e0b0: cmp      r3, #3
0037e0b4: beq      #0x37e188
0037e0b8: ldr      r5, [r4, #4]
0037e0bc: ldm      r5, {r0, r3}
0037e0c0: rsb      r3, r0, r3
0037e0c4: asr      r3, r3, #4
0037e0c8: add      r2, r3, r3, lsl #3
0037e0cc: add      r2, r2, r2, lsl #6
0037e0d0: add      r2, r3, r2, lsl #3
0037e0d4: add      r2, r2, r2, lsl #15
0037e0d8: add      r3, r3, r2, lsl #3
0037e0dc: cmp      r3, #0
0037e0e0: beq      #0x37e15c
0037e0e4: bl       #0x31bbf0
0037e0e8: bl       #0x30e4cc
0037e0ec: ldr      r4, [r4, #4]
0037e0f0: mov      r5, r0
0037e0f4: ldm      r4, {r0, r3}
0037e0f8: rsb      r3, r0, r3
0037e0fc: asr      r3, r3, #4
0037e100: add      r2, r3, r3, lsl #3
0037e104: add      r2, r2, r2, lsl #6
0037e108: add      r2, r3, r2, lsl #3
0037e10c: add      r2, r2, r2, lsl #15
0037e110: add      r3, r3, r2, lsl #3
0037e114: rsb      r3, r3, #0
0037e118: cmp      r3, #1
0037e11c: bls      #0x37e148
0037e120: add      r0, r0, #0x70
0037e124: bl       #0x31bbf0
0037e128: bl       #0x30e4cc
0037e12c: asr      r1, r0, #8
0037e130: mov      r0, r5
0037e134: bl       #0x30e2a4
0037e138: mov      r1, r0
0037e13c: mov      r0, r6
0037e140: pop      {r4, r5, r6, lr}
0037e144: b        #0x37cb24
0037e148: ldr      r0, [pc, #0x48]
0037e14c: add      r0, pc, r0
0037e150: bl       #0x708eb0
0037e154: ldr      r0, [r4]
0037e158: b        #0x37e120
0037e15c: ldr      r0, [pc, #0x38]
0037e160: add      r0, pc, r0
0037e164: bl       #0x708eb0
0037e168: ldr      r0, [r5]
0037e16c: b        #0x37e0e4
0037e170: ldr      r0, [pc, #0x28]
0037e174: add      r0, pc, r0
0037e178: bl       #0x708eb0
0037e17c: ldr      r1, [r5]
0037e180: b        #0x37e0ac
0037e184: pop      {r4, r5, r6, pc}
0037e188: mov      r0, r4
0037e18c: mov      r1, #1
0037e190: bl       #0x37baf8
0037e194: b        #0x37e0b8
0037e198: subseq   r0, r4, ip, lsl r3
0037e19c: subseq   r0, r4, r8, lsl #6
0037e1a0: ldrsheq  r0, [r4], #-0x24

# _ZN9LuaScript12_GetPyStructERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f4a8: push     {r4, r5, r6, r7, r8, lr}
0037f4ac: ldr      r6, [r0, #4]
0037f4b0: mov      r4, r1
0037f4b4: ldr      r5, [pc, #0x12c]
0037f4b8: ldm      r6, {r1, r3}
0037f4bc: add      r5, pc, r5
0037f4c0: mov      r7, r0
0037f4c4: rsb      r3, r1, r3
0037f4c8: asr      r3, r3, #4
0037f4cc: add      r2, r3, r3, lsl #3
0037f4d0: add      r2, r2, r2, lsl #6
0037f4d4: add      r2, r3, r2, lsl #3
0037f4d8: add      r2, r2, r2, lsl #15
0037f4dc: add      r3, r3, r2, lsl #3
0037f4e0: rsb      r3, r3, #0
0037f4e4: cmp      r3, #1
0037f4e8: bls      #0x37f500
0037f4ec: cmp      r3, #0
0037f4f0: beq      #0x37f504
0037f4f4: ldr      r3, [r1, #4]
0037f4f8: cmp      r3, #4
0037f4fc: beq      #0x37f518
0037f500: pop      {r4, r5, r6, r7, r8, pc}
0037f504: ldr      r0, [pc, #0xe0]
0037f508: add      r0, pc, r0
0037f50c: bl       #0x708eb0
0037f510: ldr      r1, [r6]
0037f514: b        #0x37f4f4
0037f518: mov      r0, r7
0037f51c: mov      r1, #1
0037f520: bl       #0x37baf8
0037f524: ldr      r3, [r0, #4]
0037f528: cmp      r3, #4
0037f52c: bne      #0x37f500
0037f530: ldr      r6, [r7, #4]
0037f534: ldr      r2, [pc, #0xb4]
0037f538: ldm      r6, {r0, r3}
0037f53c: ldr      r2, [r5, r2]
0037f540: rsb      r3, r0, r3
0037f544: asr      r3, r3, #4
0037f548: ldr      r8, [r2, #0x30]
0037f54c: add      r2, r3, r3, lsl #3
0037f550: add      r2, r2, r2, lsl #6
0037f554: add      r2, r3, r2, lsl #3
0037f558: add      r2, r2, r2, lsl #15
0037f55c: add      r3, r3, r2, lsl #3
0037f560: cmp      r3, #0
0037f564: bne      #0x37f578
0037f568: ldr      r0, [pc, #0x84]
0037f56c: add      r0, pc, r0
0037f570: bl       #0x708eb0
0037f574: ldr      r0, [r6]
0037f578: bl       #0x31c49c
0037f57c: ldr      r5, [r7, #4]
0037f580: mov      r6, r0
0037f584: ldm      r5, {r0, r3}
0037f588: rsb      r3, r0, r3
0037f58c: asr      r3, r3, #4
0037f590: add      r2, r3, r3, lsl #3
0037f594: add      r2, r2, r2, lsl #6
0037f598: add      r2, r3, r2, lsl #3
0037f59c: add      r2, r2, r2, lsl #15
0037f5a0: add      r3, r3, r2, lsl #3
0037f5a4: rsb      r3, r3, #0
0037f5a8: cmp      r3, #1
0037f5ac: bhi      #0x37f5c0
0037f5b0: ldr      r0, [pc, #0x40]
0037f5b4: add      r0, pc, r0
0037f5b8: bl       #0x708eb0
0037f5bc: ldr      r0, [r5]
0037f5c0: add      r0, r0, #0x70
0037f5c4: bl       #0x31c49c
0037f5c8: mov      r1, r6
0037f5cc: mov      r2, r0
0037f5d0: mov      r0, r8
0037f5d4: bl       #0x4bd640
0037f5d8: mov      r1, r0
0037f5dc: mov      r0, r4
0037f5e0: pop      {r4, r5, r6, r7, r8, lr}
0037f5e4: b        #0x37cb24

# _ZN9LuaScript11_PopVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037dc44: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0037dc48: ldr      sb, [pc, #0x174]
0037dc4c: ldr      r4, [r2, #0x54]
0037dc50: sub      sp, sp, #8
0037dc54: mov      r8, r2
0037dc58: add      sb, pc, sb
0037dc5c: add      r7, r2, #0x4c
0037dc60: add      r5, r2, #0x34
0037dc64: add      sl, sp, #4
0037dc68: cmp      r7, r4
0037dc6c: beq      #0x37dcd4
0037dc70: ldr      r0, [r4, #0x28]
0037dc74: ldr      r6, [r4, #0x24]
0037dc78: rsb      r6, r0, r6
0037dc7c: cmp      r6, #0
0037dc80: ble      #0x37dd44
0037dc84: mov      r0, r5
0037dc88: add      r1, r4, #0x10
0037dc8c: bl       #0x37dac4
0037dc90: add      r3, r4, #0x14
0037dc94: cmp      r3, r0
0037dc98: beq      #0x37dca8
0037dc9c: ldr      r1, [r4, #0x28]
0037dca0: ldr      r2, [r4, #0x24]
0037dca4: bl       #0x3109e0
0037dca8: ldr      r2, [r4, #0xc]
0037dcac: cmp      r2, #0
0037dcb0: bne      #0x37dcbc
0037dcb4: b        #0x37dd10
0037dcb8: mov      r2, r3
0037dcbc: ldr      r3, [r2, #8]
0037dcc0: cmp      r3, #0
0037dcc4: bne      #0x37dcb8
0037dcc8: mov      r4, r2
0037dccc: cmp      r7, r4
0037dcd0: bne      #0x37dc70
0037dcd4: ldr      r3, [r8, #0x5c]
0037dcd8: cmp      r3, #0
0037dcdc: beq      #0x37dd00
0037dce0: mov      r0, r7
0037dce4: ldr      r1, [r8, #0x50]
0037dce8: bl       #0x37bd7c
0037dcec: mov      r3, #0
0037dcf0: str      r7, [r8, #0x58]
0037dcf4: str      r3, [r8, #0x5c]
0037dcf8: str      r7, [r8, #0x54]
0037dcfc: str      r3, [r8, #0x50]
0037dd00: mov      r3, #0
0037dd04: strb     r3, [r8, #0x64]
0037dd08: add      sp, sp, #8
0037dd0c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0037dd10: ldr      r3, [r4, #4]
0037dd14: ldr      r1, [r3, #0xc]
0037dd18: cmp      r4, r1
0037dd1c: bne      #0x37dd38
0037dd20: mov      r4, r3
0037dd24: ldr      r3, [r3, #4]
0037dd28: ldr      r2, [r3, #0xc]
0037dd2c: cmp      r2, r4
0037dd30: beq      #0x37dd20
0037dd34: ldr      r2, [r4, #0xc]
0037dd38: cmp      r2, r3
0037dd3c: movne    r4, r3
0037dd40: b        #0x37dc68
0037dd44: mov      r1, sb
0037dd48: mov      r2, r6
0037dd4c: bl       #0x30e5e0
0037dd50: cmp      r0, #0
0037dd54: bne      #0x37dc84
0037dd58: cmp      r6, #0
0037dd5c: bne      #0x37dc84
0037dd60: ldr      r3, [r8, #0x38]
0037dd64: cmp      r3, #0
0037dd68: ldrne    r0, [r4, #0x10]
0037dd6c: movne    r1, r5
0037dd70: bne      #0x37dd7c
0037dd74: b        #0x37dca8
0037dd78: mov      r3, r2
0037dd7c: ldr      r2, [r3, #0x10]
0037dd80: cmp      r2, r0
0037dd84: ldrlo    r2, [r3, #0xc]
0037dd88: ldrhs    r2, [r3, #8]
0037dd8c: movlo    r3, r1
0037dd90: mov      r1, r3
0037dd94: cmp      r2, #0
0037dd98: bne      #0x37dd78
0037dd9c: cmp      r5, r3
0037dda0: beq      #0x37dca8
0037dda4: ldr      r2, [r3, #0x10]
0037dda8: cmp      r2, r0
0037ddac: bhi      #0x37dca8
0037ddb0: mov      r0, r5
0037ddb4: mov      r1, sl
0037ddb8: str      r3, [sp, #4]
0037ddbc: bl       #0x37be48
0037ddc0: b        #0x37dca8
0037ddc4: ldrheq   sp, [r4], #-0xb0

# _ZN9LuaScript9_GetPyCstERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f354: push     {r4, r5, r6, r7, r8, lr}
0037f358: ldr      r6, [r0, #4]
0037f35c: mov      r4, r1
0037f360: ldr      r5, [pc, #0x12c]
0037f364: ldm      r6, {r1, r3}
0037f368: add      r5, pc, r5
0037f36c: mov      r7, r0
0037f370: rsb      r3, r1, r3
0037f374: asr      r3, r3, #4
0037f378: add      r2, r3, r3, lsl #3
0037f37c: add      r2, r2, r2, lsl #6
0037f380: add      r2, r3, r2, lsl #3
0037f384: add      r2, r2, r2, lsl #15
0037f388: add      r3, r3, r2, lsl #3
0037f38c: rsb      r3, r3, #0
0037f390: cmp      r3, #1
0037f394: bls      #0x37f3ac
0037f398: cmp      r3, #0
0037f39c: beq      #0x37f3b0
0037f3a0: ldr      r3, [r1, #4]
0037f3a4: cmp      r3, #4
0037f3a8: beq      #0x37f3c4
0037f3ac: pop      {r4, r5, r6, r7, r8, pc}
0037f3b0: ldr      r0, [pc, #0xe0]
0037f3b4: add      r0, pc, r0
0037f3b8: bl       #0x708eb0
0037f3bc: ldr      r1, [r6]
0037f3c0: b        #0x37f3a0
0037f3c4: mov      r0, r7
0037f3c8: mov      r1, #1
0037f3cc: bl       #0x37baf8
0037f3d0: ldr      r3, [r0, #4]
0037f3d4: cmp      r3, #4
0037f3d8: bne      #0x37f3ac
0037f3dc: ldr      r6, [r7, #4]
0037f3e0: ldr      r2, [pc, #0xb4]
0037f3e4: ldm      r6, {r0, r3}
0037f3e8: ldr      r2, [r5, r2]
0037f3ec: rsb      r3, r0, r3
0037f3f0: asr      r3, r3, #4
0037f3f4: ldr      r8, [r2, #0x2c]
0037f3f8: add      r2, r3, r3, lsl #3
0037f3fc: add      r2, r2, r2, lsl #6
0037f400: add      r2, r3, r2, lsl #3
0037f404: add      r2, r2, r2, lsl #15
0037f408: add      r3, r3, r2, lsl #3
0037f40c: cmp      r3, #0
0037f410: bne      #0x37f424
0037f414: ldr      r0, [pc, #0x84]
0037f418: add      r0, pc, r0
0037f41c: bl       #0x708eb0
0037f420: ldr      r0, [r6]
0037f424: bl       #0x31c49c
0037f428: ldr      r5, [r7, #4]
0037f42c: mov      r6, r0
0037f430: ldm      r5, {r0, r3}
0037f434: rsb      r3, r0, r3
0037f438: asr      r3, r3, #4
0037f43c: add      r2, r3, r3, lsl #3
0037f440: add      r2, r2, r2, lsl #6
0037f444: add      r2, r3, r2, lsl #3
0037f448: add      r2, r2, r2, lsl #15
0037f44c: add      r3, r3, r2, lsl #3
0037f450: rsb      r3, r3, #0
0037f454: cmp      r3, #1
0037f458: bhi      #0x37f46c
0037f45c: ldr      r0, [pc, #0x40]
0037f460: add      r0, pc, r0
0037f464: bl       #0x708eb0
0037f468: ldr      r0, [r5]
0037f46c: add      r0, r0, #0x70
0037f470: bl       #0x31c49c
0037f474: mov      r1, r6
0037f478: mov      r2, r0
0037f47c: mov      r0, r8
0037f480: bl       #0x4c4bdc
0037f484: mov      r1, r0
0037f488: mov      r0, r4
0037f48c: pop      {r4, r5, r6, r7, r8, lr}
0037f490: b        #0x37cb24
0037f494: rsbeq    r5, r1, r8, lsr #14
0037f498: ldrheq   pc, [r3], #-4
0037f49c: strdeq   r3, r4, [r0], -r4
0037f4a0: subseq   pc, r3, r0, asr r0
0037f4a4: subseq   pc, r3, r8

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

# _ZN9LuaScript14_GetGameScriptERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037c934: ldr      r3, [pc, #0x34]
0037c938: ldr      r2, [pc, #0x34]
0037c93c: push     {r4, lr}
0037c940: add      r3, pc, r3
0037c944: ldr      r0, [r3, r2]
0037c948: mov      r4, r1
0037c94c: bl       #0x31f594
0037c950: cmp      r0, #0
0037c954: beq      #0x37c96c
0037c958: bl       #0x3f1394
0037c95c: mov      r1, r0
0037c960: mov      r0, r4
0037c964: pop      {r4, lr}
0037c968: b        #0x37c8cc
0037c96c: pop      {r4, pc}
0037c970: rsbeq    r8, r1, r0, asr r1
0037c974: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScript19_GetHostPlayerLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037cc00: ldr      r3, [pc, #0x2c]
0037cc04: ldr      r2, [pc, #0x2c]
0037cc08: push     {r4, lr}
0037cc0c: add      r3, pc, r3
0037cc10: ldr      r2, [r3, r2]
0037cc14: mov      r4, r1
0037cc18: ldr      r0, [r2, #0x40]
0037cc1c: bl       #0x36e09c
0037cc20: ldr      r3, [r0, #0x330]
0037cc24: mov      r0, r4
0037cc28: mov      r1, r3
0037cc2c: pop      {r4, lr}
0037cc30: b        #0x37cb24
0037cc34: rsbeq    r7, r1, r4, lsl #29
0037cc38: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScript14_GetHostPlayerERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ca60: ldr      r3, [pc, #0x2c]
0037ca64: ldr      r2, [pc, #0x2c]
0037ca68: push     {r4, lr}
0037ca6c: add      r3, pc, r3
0037ca70: ldr      r2, [r3, r2]
0037ca74: mov      r4, r1
0037ca78: ldr      r0, [r2, #0x40]
0037ca7c: bl       #0x36e09c
0037ca80: ldr      r3, [r0, #0x660]
0037ca84: mov      r0, r4
0037ca88: mov      r1, r3
0037ca8c: pop      {r4, lr}
0037ca90: b        #0x37c9f8
0037ca94: rsbeq    r8, r1, r4, lsr #32
0037ca98: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScript13_CallPyScriptERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ef3c: push     {r4, r5, r6, lr}
0037ef40: ldr      r2, [r0, #4]
0037ef44: ldr      r4, [pc, #0x90]
0037ef48: ldm      r2, {r1, r3}
0037ef4c: add      r4, pc, r4
0037ef50: rsb      r3, r1, r3
0037ef54: asr      r3, r3, #4
0037ef58: add      r2, r3, r3, lsl #3
0037ef5c: add      r2, r2, r2, lsl #6
0037ef60: add      r2, r3, r2, lsl #3
0037ef64: add      r2, r2, r2, lsl #15
0037ef68: add      r3, r3, r2, lsl #3
0037ef6c: cmp      r3, #0
0037ef70: bne      #0x37ef78
0037ef74: pop      {r4, r5, r6, pc}
0037ef78: ldr      r3, [r1, #4]
0037ef7c: cmp      r3, #4
0037ef80: bne      #0x37ef74
0037ef84: mov      r1, #0
0037ef88: bl       #0x37baf8
0037ef8c: bl       #0x31c49c
0037ef90: ldr      r3, [pc, #0x48]
0037ef94: mov      r1, r0
0037ef98: mov      r2, #1
0037ef9c: ldr      r4, [r4, r3]
0037efa0: mov      r0, r4
0037efa4: bl       #0x4591f0
0037efa8: cmn      r0, #1
0037efac: mov      r5, r0
0037efb0: beq      #0x37ef74
0037efb4: mov      r0, r4
0037efb8: mov      r1, r5
0037efbc: bl       #0x455bec
0037efc0: subs     r3, r0, #0
0037efc4: bne      #0x37ef74
0037efc8: mov      r0, r4
0037efcc: mov      r1, r5
0037efd0: mvn      r2, #0
0037efd4: pop      {r4, r5, r6, lr}
0037efd8: b        #0x4605c0
0037efdc: rsbeq    r5, r1, r4, asr #22
0037efe0: andeq    r1, r0, r0, lsr #20

# _ZN9LuaScript12_SetGameTypeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f878: push     {r4, r5, r6, lr}
0037f87c: ldr      r2, [r0, #4]
0037f880: ldr      r6, [pc, #0x124]
0037f884: sub      sp, sp, #8
0037f888: ldm      r2, {r1, r3}
0037f88c: add      r6, pc, r6
0037f890: mov      r4, r0
0037f894: rsb      r3, r1, r3
0037f898: asr      r3, r3, #4
0037f89c: add      r2, r3, r3, lsl #3
0037f8a0: add      r2, r2, r2, lsl #6
0037f8a4: add      r2, r3, r2, lsl #3
0037f8a8: add      r2, r2, r2, lsl #15
0037f8ac: add      r3, r3, r2, lsl #3
0037f8b0: cmp      r3, #0
0037f8b4: bne      #0x37f8c0
0037f8b8: add      sp, sp, #8
0037f8bc: pop      {r4, r5, r6, pc}
0037f8c0: ldr      r3, [r1, #4]
0037f8c4: cmp      r3, #3
0037f8c8: bne      #0x37f8b8
0037f8cc: mov      r1, #0
0037f8d0: bl       #0x37baf8
0037f8d4: bl       #0x31bbf0
0037f8d8: bl       #0x8be2a0
0037f8dc: cmp      r0, #3
0037f8e0: bhi      #0x37f8b8
0037f8e4: ldr      r3, [pc, #0xc4]
0037f8e8: ldr      r0, [r6, r3]
0037f8ec: bl       #0x31f594
0037f8f0: subs     r5, r0, #0
0037f8f4: beq      #0x37f8b8
0037f8f8: ldr      r4, [r4, #4]
0037f8fc: ldm      r4, {r0, r3}
0037f900: rsb      r3, r0, r3
0037f904: asr      r3, r3, #4
0037f908: add      r2, r3, r3, lsl #3
0037f90c: add      r2, r2, r2, lsl #6
0037f910: add      r2, r3, r2, lsl #3
0037f914: add      r2, r2, r2, lsl #15
0037f918: add      r3, r3, r2, lsl #3
0037f91c: cmp      r3, #0
0037f920: bne      #0x37f934
0037f924: ldr      r0, [pc, #0x88]
0037f928: add      r0, pc, r0
0037f92c: bl       #0x708eb0
0037f930: ldr      r0, [r4]
0037f934: bl       #0x31bbf0
0037f938: bl       #0x8be2a0
0037f93c: cmp      r0, #3
0037f940: mov      r4, r0
0037f944: bls      #0x37f96c
0037f948: ldr      r3, [pc, #0x68]
0037f94c: ldr      r3, [r6, r3]
0037f950: ldr      r3, [r3]
0037f954: cmp      r3, #2
0037f958: moveq    r3, #0
0037f95c: streq    r3, [r3]
0037f960: beq      #0x37f96c
0037f964: cmp      r3, #1
0037f968: beq      #0x37f974
0037f96c: str      r4, [r5, #0x150]
0037f970: b        #0x37f8b8
0037f974: ldr      r0, [pc, #0x40]
0037f978: ldr      r1, [pc, #0x40]
0037f97c: ldr      r2, [pc, #0x40]
0037f980: ldr      r0, [r6, r0]
0037f984: ldr      r3, [pc, #0x3c]
0037f988: movw     ip, #0x1ee
0037f98c: add      r1, pc, r1
0037f990: add      r0, r0, #0xa8
0037f994: add      r2, pc, r2
0037f998: add      r3, pc, r3
0037f99c: str      ip, [sp]
0037f9a0: bl       #0x30e004
0037f9a4: str      r4, [r5, #0x150]
0037f9a8: b        #0x37f8b8
0037f9ac: rsbeq    r5, r1, r4, lsl #4
0037f9b0: strdeq   r3, r4, [r0], -r4
0037f9b4: subseq   lr, r3, r0, asr #22
0037f9b8: andeq    r3, r0, r0, asr #19
0037f9bc: andeq    r1, r0, r0, asr #19
0037f9c0: subseq   lr, r3, ip, asr #20

# _ZN9LuaScript10_PlaySoundERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e578: push     {r4, r5, r6, r7, r8, lr}
0037e57c: ldr      r6, [r0, #4]
0037e580: mov      r5, r0
0037e584: ldr      r4, [pc, #0x18c]
0037e588: ldm      r6, {r0, r3}
0037e58c: add      r4, pc, r4
0037e590: sub      sp, sp, #8
0037e594: rsb      r3, r0, r3
0037e598: asr      r3, r3, #4
0037e59c: add      r2, r3, r3, lsl #3
0037e5a0: add      r2, r2, r2, lsl #6
0037e5a4: add      r2, r3, r2, lsl #3
0037e5a8: add      r2, r2, r2, lsl #15
0037e5ac: add      r3, r3, r2, lsl #3
0037e5b0: rsb      r3, r3, #0
0037e5b4: cmp      r3, #3
0037e5b8: bhi      #0x37e5cc
0037e5bc: ldr      r0, [pc, #0x158]
0037e5c0: add      r0, pc, r0
0037e5c4: bl       #0x708eb0
0037e5c8: ldr      r0, [r6]
0037e5cc: add      r0, r0, #0x150
0037e5d0: bl       #0x31bc80
0037e5d4: cmp      r0, #0
0037e5d8: bne      #0x37e700
0037e5dc: ldr      r6, [r5, #4]
0037e5e0: ldm      r6, {r0, r3}
0037e5e4: rsb      r3, r0, r3
0037e5e8: asr      r3, r3, #4
0037e5ec: add      r2, r3, r3, lsl #3
0037e5f0: add      r2, r2, r2, lsl #6
0037e5f4: add      r2, r3, r2, lsl #3
0037e5f8: add      r2, r2, r2, lsl #15
0037e5fc: add      r3, r3, r2, lsl #3
0037e600: cmp      r3, #0
0037e604: bne      #0x37e618
0037e608: ldr      r0, [pc, #0x110]
0037e60c: add      r0, pc, r0
0037e610: bl       #0x708eb0
0037e614: ldr      r0, [r6]
0037e618: bl       #0x31c49c
0037e61c: bl       #0x37ba84
0037e620: cmn      r0, #1
0037e624: mov      r7, r0
0037e628: beq      #0x37e6d0
0037e62c: ldr      r8, [r5, #4]
0037e630: ldr      r2, [pc, #0xec]
0037e634: ldm      r8, {r0, r3}
0037e638: ldr      r2, [r4, r2]
0037e63c: rsb      r3, r0, r3
0037e640: asr      r3, r3, #4
0037e644: ldr      r6, [r2]
0037e648: add      r2, r3, r3, lsl #3
0037e64c: add      r2, r2, r2, lsl #6
0037e650: add      r2, r3, r2, lsl #3
0037e654: add      r2, r2, r2, lsl #15
0037e658: add      r3, r3, r2, lsl #3
0037e65c: rsb      r3, r3, #0
0037e660: cmp      r3, #1
0037e664: bls      #0x37e6ec
0037e668: add      r0, r0, #0x70
0037e66c: bl       #0x31bc80
0037e670: ldr      r5, [r5, #4]
0037e674: mov      r4, r0
0037e678: ldm      r5, {r0, r3}
0037e67c: rsb      r3, r0, r3
0037e680: asr      r3, r3, #4
0037e684: add      r2, r3, r3, lsl #3
0037e688: add      r2, r2, r2, lsl #6
0037e68c: add      r2, r3, r2, lsl #3
0037e690: add      r2, r2, r2, lsl #15
0037e694: add      r3, r3, r2, lsl #3
0037e698: rsb      r3, r3, #0
0037e69c: cmp      r3, #2
0037e6a0: bls      #0x37e6d8
0037e6a4: add      r0, r0, #0xe0
0037e6a8: bl       #0x31bbf0
0037e6ac: bl       #0x30e4cc
0037e6b0: mov      ip, #0
0037e6b4: mov      r3, r0
0037e6b8: mov      r1, r7
0037e6bc: mov      r0, r6
0037e6c0: mov      r2, r4
0037e6c4: str      ip, [sp, #4]
0037e6c8: str      ip, [sp]
0037e6cc: bl       #0x36b80c
0037e6d0: add      sp, sp, #8
0037e6d4: pop      {r4, r5, r6, r7, r8, pc}
0037e6d8: ldr      r0, [pc, #0x48]
0037e6dc: add      r0, pc, r0
0037e6e0: bl       #0x708eb0
0037e6e4: ldr      r0, [r5]
0037e6e8: b        #0x37e6a4
0037e6ec: ldr      r0, [pc, #0x38]
0037e6f0: add      r0, pc, r0
0037e6f4: bl       #0x708eb0
0037e6f8: ldr      r0, [r8]
0037e6fc: b        #0x37e668
0037e700: ldr      r3, [pc, #0x1c]
0037e704: mov      r1, #0
0037e708: ldr      r3, [r4, r3]
0037e70c: ldr      r0, [r3]
0037e710: bl       #0x36a1a0
0037e714: b        #0x37e5dc
0037e718: rsbeq    r6, r1, r4, lsl #10
0037e71c: subseq   pc, r3, r8, lsr #29
0037e720: subseq   pc, r3, ip, asr lr
0037e724: andeq    r0, r0, r4, lsr #27
0037e728: subseq   pc, r3, ip, lsl #27
0037e72c: subseq   pc, r3, r8, ror sp

# _ZN9LuaScript13_AddToVFTableERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ec70: push     {r4, r5, r6, r7, lr}
0037ec74: ldr      r5, [r0, #4]
0037ec78: mov      r6, r2
0037ec7c: sub      sp, sp, #0xc
0037ec80: ldm      r5, {r1, r3}
0037ec84: mov      r4, r0
0037ec88: rsb      r3, r1, r3
0037ec8c: asr      r3, r3, #4
0037ec90: add      r2, r3, r3, lsl #3
0037ec94: add      r2, r2, r2, lsl #6
0037ec98: add      r2, r3, r2, lsl #3
0037ec9c: add      r2, r2, r2, lsl #15
0037eca0: add      r3, r3, r2, lsl #3
0037eca4: rsb      r3, r3, #0
0037eca8: cmp      r3, #1
0037ecac: bls      #0x37ecc4
0037ecb0: cmp      r3, #0
0037ecb4: beq      #0x37eccc
0037ecb8: ldr      r3, [r1, #4]
0037ecbc: cmp      r3, #4
0037ecc0: beq      #0x37ece8
0037ecc4: add      sp, sp, #0xc
0037ecc8: pop      {r4, r5, r6, r7, pc}
0037eccc: ldr      r0, [pc, #0x1a0]
0037ecd0: add      r0, pc, r0
0037ecd4: bl       #0x708eb0
0037ecd8: ldr      r1, [r5]
0037ecdc: ldr      r3, [r1, #4]
0037ece0: cmp      r3, #4
0037ece4: bne      #0x37ecc4
0037ece8: mov      r0, r4
0037ecec: mov      r1, #1
0037ecf0: bl       #0x37baf8
0037ecf4: ldr      r3, [r0, #4]
0037ecf8: cmp      r3, #4
0037ecfc: bne      #0x37ecc4
0037ed00: ldr      r5, [r4, #4]
0037ed04: ldm      r5, {r0, r3}
0037ed08: rsb      r3, r0, r3
0037ed0c: asr      r3, r3, #4
0037ed10: add      r2, r3, r3, lsl #3
0037ed14: add      r2, r2, r2, lsl #6
0037ed18: add      r2, r3, r2, lsl #3
0037ed1c: add      r2, r2, r2, lsl #15
0037ed20: add      r3, r3, r2, lsl #3
0037ed24: cmp      r3, #0
0037ed28: bne      #0x37ed3c
0037ed2c: ldr      r0, [pc, #0x144]
0037ed30: add      r0, pc, r0
0037ed34: bl       #0x708eb0
0037ed38: ldr      r0, [r5]
0037ed3c: bl       #0x31c49c
0037ed40: bl       #0x37c164
0037ed44: ldrb     r3, [r6, #0x64]
0037ed48: str      r0, [sp, #4]
0037ed4c: cmp      r3, #0
0037ed50: beq      #0x37edac
0037ed54: ldr      r3, [r6, #0x50]
0037ed58: add      ip, r6, #0x4c
0037ed5c: cmp      r3, #0
0037ed60: beq      #0x37ee28
0037ed64: mov      r1, ip
0037ed68: b        #0x37ed70
0037ed6c: mov      r3, r2
0037ed70: ldr      r2, [r3, #0x10]
0037ed74: cmp      r0, r2
0037ed78: ldrhi    r2, [r3, #0xc]
0037ed7c: ldrls    r2, [r3, #8]
0037ed80: movhi    r3, r1
0037ed84: mov      r1, r3
0037ed88: cmp      r2, #0
0037ed8c: bne      #0x37ed6c
0037ed90: cmp      ip, r3
0037ed94: beq      #0x37ee30
0037ed98: ldr      r2, [r3, #0x10]
0037ed9c: cmp      r0, r2
0037eda0: blo      #0x37ee28
0037eda4: cmp      ip, r3
0037eda8: beq      #0x37ee30
0037edac: add      r6, r6, #0x34
0037edb0: add      r5, sp, #4
0037edb4: mov      r1, r5
0037edb8: mov      r0, r6
0037edbc: bl       #0x37dac4
0037edc0: ldr      r4, [r4, #4]
0037edc4: mov      r5, r0
0037edc8: ldm      r4, {r0, r3}
0037edcc: rsb      r3, r0, r3
0037edd0: asr      r3, r3, #4
0037edd4: add      r2, r3, r3, lsl #3
0037edd8: add      r2, r2, r2, lsl #6
0037eddc: add      r2, r3, r2, lsl #3
0037ede0: add      r2, r2, r2, lsl #15
0037ede4: add      r3, r3, r2, lsl #3
0037ede8: rsb      r3, r3, #0
0037edec: cmp      r3, #1
0037edf0: bhi      #0x37ee04
0037edf4: ldr      r0, [pc, #0x80]
0037edf8: add      r0, pc, r0
0037edfc: bl       #0x708eb0
0037ee00: ldr      r0, [r4]
0037ee04: add      r0, r0, #0x70
0037ee08: bl       #0x31c49c
0037ee0c: mov      r4, r0
0037ee10: bl       #0x30de54
0037ee14: mov      r1, r4
0037ee18: add      r2, r4, r0
0037ee1c: mov      r0, r5
0037ee20: bl       #0x3109e0
0037ee24: b        #0x37ecc4
0037ee28: mov      r3, ip
0037ee2c: b        #0x37eda4
0037ee30: add      r5, sp, #4
0037ee34: mov      r0, ip
0037ee38: mov      r1, r5
0037ee3c: bl       #0x37dac4
0037ee40: add      r6, r6, #0x34
0037ee44: mov      r7, r0
0037ee48: mov      r1, r5
0037ee4c: mov      r0, r6
0037ee50: bl       #0x37dac4
0037ee54: cmp      r7, r0
0037ee58: mov      r3, r0
0037ee5c: beq      #0x37edb4
0037ee60: mov      r0, r7
0037ee64: ldr      r2, [r3, #0x10]
0037ee68: ldr      r1, [r3, #0x14]
0037ee6c: bl       #0x3109e0
0037ee70: b        #0x37edb4

# _ZN9LuaScript9_GetPyOIDERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f5fc: push     {r4, r5, r6, r7, r8, lr}
0037f600: ldr      r6, [r0, #4]
0037f604: mov      r4, r1
0037f608: ldr      r5, [pc, #0x12c]
0037f60c: ldm      r6, {r1, r3}
0037f610: add      r5, pc, r5
0037f614: mov      r7, r0
0037f618: rsb      r3, r1, r3
0037f61c: asr      r3, r3, #4
0037f620: add      r2, r3, r3, lsl #3
0037f624: add      r2, r2, r2, lsl #6
0037f628: add      r2, r3, r2, lsl #3
0037f62c: add      r2, r2, r2, lsl #15
0037f630: add      r3, r3, r2, lsl #3
0037f634: rsb      r3, r3, #0
0037f638: cmp      r3, #1
0037f63c: bls      #0x37f654
0037f640: cmp      r3, #0
0037f644: beq      #0x37f658
0037f648: ldr      r3, [r1, #4]
0037f64c: cmp      r3, #4
0037f650: beq      #0x37f66c
0037f654: pop      {r4, r5, r6, r7, r8, pc}
0037f658: ldr      r0, [pc, #0xe0]
0037f65c: add      r0, pc, r0
0037f660: bl       #0x708eb0
0037f664: ldr      r1, [r6]
0037f668: b        #0x37f648
0037f66c: mov      r0, r7
0037f670: mov      r1, #1
0037f674: bl       #0x37baf8
0037f678: ldr      r3, [r0, #4]
0037f67c: cmp      r3, #4
0037f680: bne      #0x37f654
0037f684: ldr      r6, [r7, #4]
0037f688: ldr      r2, [pc, #0xb4]
0037f68c: ldm      r6, {r0, r3}
0037f690: ldr      r2, [r5, r2]
0037f694: rsb      r3, r0, r3
0037f698: asr      r3, r3, #4
0037f69c: ldr      r8, [r2, #0x30]
0037f6a0: add      r2, r3, r3, lsl #3
0037f6a4: add      r2, r2, r2, lsl #6
0037f6a8: add      r2, r3, r2, lsl #3
0037f6ac: add      r2, r2, r2, lsl #15
0037f6b0: add      r3, r3, r2, lsl #3
0037f6b4: cmp      r3, #0
0037f6b8: bne      #0x37f6cc
0037f6bc: ldr      r0, [pc, #0x84]
0037f6c0: add      r0, pc, r0
0037f6c4: bl       #0x708eb0
0037f6c8: ldr      r0, [r6]
0037f6cc: bl       #0x31c49c
0037f6d0: ldr      r5, [r7, #4]
0037f6d4: mov      r6, r0
0037f6d8: ldm      r5, {r0, r3}
0037f6dc: rsb      r3, r0, r3
0037f6e0: asr      r3, r3, #4
0037f6e4: add      r2, r3, r3, lsl #3
0037f6e8: add      r2, r2, r2, lsl #6
0037f6ec: add      r2, r3, r2, lsl #3
0037f6f0: add      r2, r2, r2, lsl #15
0037f6f4: add      r3, r3, r2, lsl #3
0037f6f8: rsb      r3, r3, #0
0037f6fc: cmp      r3, #1
0037f700: bhi      #0x37f714
0037f704: ldr      r0, [pc, #0x40]
0037f708: add      r0, pc, r0
0037f70c: bl       #0x708eb0
0037f710: ldr      r0, [r5]
0037f714: add      r0, r0, #0x70
0037f718: bl       #0x31c49c
0037f71c: mov      r1, r6
0037f720: mov      r2, r0
0037f724: mov      r0, r8
0037f728: bl       #0x4bd640
0037f72c: mov      r1, r0
0037f730: mov      r0, r4
0037f734: pop      {r4, r5, r6, r7, r8, lr}
0037f738: b        #0x37cb24
0037f73c: rsbeq    r5, r1, r0, lsl #9
0037f740: subseq   lr, r3, ip, lsl #28
0037f744: strdeq   r3, r4, [r0], -r4
0037f748: subseq   lr, r3, r8, lsr #27
0037f74c: subseq   lr, r3, r0, ror #26

# _ZN9LuaScript10_StopSoundERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e730: push     {r4, r5, r6, lr}
0037e734: ldr      r5, [r0, #4]
0037e738: mov      r6, r0
0037e73c: ldr      r4, [pc, #0xc0]
0037e740: ldm      r5, {r0, r3}
0037e744: add      r4, pc, r4
0037e748: rsb      r3, r0, r3
0037e74c: asr      r3, r3, #4
0037e750: add      r2, r3, r3, lsl #3
0037e754: add      r2, r2, r2, lsl #6
0037e758: add      r2, r3, r2, lsl #3
0037e75c: add      r2, r2, r2, lsl #15
0037e760: add      r3, r3, r2, lsl #3
0037e764: cmp      r3, #0
0037e768: bne      #0x37e77c
0037e76c: ldr      r0, [pc, #0x94]
0037e770: add      r0, pc, r0
0037e774: bl       #0x708eb0
0037e778: ldr      r0, [r5]
0037e77c: bl       #0x31c49c
0037e780: bl       #0x37ba84
0037e784: cmn      r0, #1
0037e788: mov      r5, r0
0037e78c: beq      #0x37e800
0037e790: ldr      r6, [r6, #4]
0037e794: ldr      r2, [pc, #0x70]
0037e798: ldm      r6, {r0, r3}
0037e79c: ldr      r2, [r4, r2]
0037e7a0: rsb      r3, r0, r3
0037e7a4: asr      r3, r3, #4
0037e7a8: ldr      r4, [r2]
0037e7ac: add      r2, r3, r3, lsl #3
0037e7b0: add      r2, r2, r2, lsl #6
0037e7b4: add      r2, r3, r2, lsl #3
0037e7b8: add      r2, r2, r2, lsl #15
0037e7bc: add      r3, r3, r2, lsl #3
0037e7c0: rsb      r3, r3, #0
0037e7c4: cmp      r3, #1
0037e7c8: bls      #0x37e7ec
0037e7cc: add      r0, r0, #0x70
0037e7d0: bl       #0x31bbf0
0037e7d4: bl       #0x30e4cc
0037e7d8: mov      r1, r5
0037e7dc: mov      r2, r0
0037e7e0: mov      r0, r4
0037e7e4: pop      {r4, r5, r6, lr}
0037e7e8: b        #0x369fec
0037e7ec: ldr      r0, [pc, #0x1c]
0037e7f0: add      r0, pc, r0
0037e7f4: bl       #0x708eb0
0037e7f8: ldr      r0, [r6]
0037e7fc: b        #0x37e7cc
0037e800: pop      {r4, r5, r6, pc}
0037e804: rsbeq    r6, r1, ip, asr #6
0037e808: ldrsheq  pc, [r3], #-0xc8
0037e80c: andeq    r0, r0, r4, lsr #27
0037e810: subseq   pc, r3, r8, ror ip

# _ZN9LuaScript7_BitXOrERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f750: push     {r4, r5, r6, lr}
0037f754: ldr      r3, [r0, #4]
0037f758: mov      r4, r0
0037f75c: mov      r5, r1
0037f760: ldm      r3, {r0, r2}
0037f764: rsb      r3, r0, r2
0037f768: asr      r3, r3, #4
0037f76c: add      r2, r3, r3, lsl #3
0037f770: add      r2, r2, r2, lsl #6
0037f774: add      r2, r3, r2, lsl #3
0037f778: add      r2, r2, r2, lsl #15
0037f77c: add      r3, r3, r2, lsl #3
0037f780: cmn      r3, #2
0037f784: beq      #0x37f78c
0037f788: pop      {r4, r5, r6, pc}
0037f78c: ldr      r3, [r0, #4]
0037f790: cmp      r3, #3
0037f794: bne      #0x37f788
0037f798: ldr      r3, [r0, #0x74]
0037f79c: cmp      r3, #3
0037f7a0: bne      #0x37f788
0037f7a4: bl       #0x31bbf0
0037f7a8: bl       #0x30e4cc
0037f7ac: ldr      r4, [r4, #4]
0037f7b0: mov      r6, r0
0037f7b4: ldm      r4, {r0, r3}
0037f7b8: rsb      r3, r0, r3
0037f7bc: asr      r3, r3, #4
0037f7c0: add      r2, r3, r3, lsl #3
0037f7c4: add      r2, r2, r2, lsl #6
0037f7c8: add      r2, r3, r2, lsl #3
0037f7cc: add      r2, r2, r2, lsl #15
0037f7d0: add      r3, r3, r2, lsl #3
0037f7d4: rsb      r3, r3, #0
0037f7d8: cmp      r3, #1
0037f7dc: bls      #0x37f7fc
0037f7e0: add      r0, r0, #0x70
0037f7e4: bl       #0x31bbf0
0037f7e8: bl       #0x30e4cc
0037f7ec: eor      r1, r0, r6
0037f7f0: mov      r0, r5
0037f7f4: pop      {r4, r5, r6, lr}
0037f7f8: b        #0x37cb24
0037f7fc: ldr      r0, [pc, #0xc]
0037f800: add      r0, pc, r0
0037f804: bl       #0x708eb0
0037f808: ldr      r0, [r4]
0037f80c: b        #0x37f7e0
0037f810: subseq   lr, r3, r8, ror #24

# _ZN9LuaScript7_BitNotERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f814: push     {r4, lr}
0037f818: ldr      r3, [r0, #4]
0037f81c: mov      r4, r1
0037f820: ldm      r3, {r1, r2}
0037f824: rsb      r3, r1, r2
0037f828: asr      r3, r3, #4
0037f82c: add      r2, r3, r3, lsl #3
0037f830: add      r2, r2, r2, lsl #6
0037f834: add      r2, r3, r2, lsl #3
0037f838: add      r2, r2, r2, lsl #15
0037f83c: add      r3, r3, r2, lsl #3
0037f840: cmn      r3, #1
0037f844: beq      #0x37f84c
0037f848: pop      {r4, pc}
0037f84c: ldr      r3, [r1, #4]
0037f850: cmp      r3, #3
0037f854: bne      #0x37f848
0037f858: mov      r1, #0
0037f85c: bl       #0x37baf8
0037f860: bl       #0x31bbf0
0037f864: bl       #0x30e4cc
0037f868: mvn      r1, r0
0037f86c: mov      r0, r4
0037f870: pop      {r4, lr}
0037f874: b        #0x37cb24

# _ZN9LuaScript24_GetHostPlayerDifficultyERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037cb8c: ldr      r3, [pc, #0x3c]
0037cb90: ldr      r2, [pc, #0x3c]
0037cb94: push     {r4, lr}
0037cb98: add      r3, pc, r3
0037cb9c: ldr      r0, [r3, r2]
0037cba0: mov      r4, r1
0037cba4: bl       #0x31f594
0037cba8: subs     r3, r0, #0
0037cbac: beq      #0x37cbc0
0037cbb0: ldr      r1, [r3, #0x118]
0037cbb4: mov      r0, r4
0037cbb8: pop      {r4, lr}
0037cbbc: b        #0x37cb24
0037cbc0: mov      r0, r4
0037cbc4: mov      r1, r3
0037cbc8: pop      {r4, lr}
0037cbcc: b        #0x37cb24

# _ZN9LuaScript8_ToFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ebc4: push     {r4, lr}
0037ebc8: ldr      r3, [r0, #4]
0037ebcc: mov      r4, r1
0037ebd0: ldm      r3, {r0, r2}
0037ebd4: rsb      r3, r0, r2
0037ebd8: asr      r3, r3, #4
0037ebdc: add      r2, r3, r3, lsl #3
0037ebe0: add      r2, r2, r2, lsl #6
0037ebe4: add      r2, r3, r2, lsl #3
0037ebe8: add      r2, r2, r2, lsl #15
0037ebec: add      r3, r3, r2, lsl #3
0037ebf0: cmp      r3, #0
0037ebf4: bne      #0x37ebfc
0037ebf8: pop      {r4, pc}
0037ebfc: bl       #0x31bbf0
0037ec00: bl       #0x30e4cc
0037ec04: lsl      r1, r0, #8
0037ec08: mov      r0, r4
0037ec0c: pop      {r4, lr}
0037ec10: b        #0x37cb24

# _ZN9LuaScript6_TraceERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ee80: bx       lr

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

# _ZN9LuaScript10_PlayMusicERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e420: push     {r4, r5, r6, r7, lr}
0037e424: ldr      r5, [r0, #4]
0037e428: mov      r6, r0
0037e42c: ldr      r4, [pc, #0x128]
0037e430: ldm      r5, {r0, r3}
0037e434: add      r4, pc, r4
0037e438: sub      sp, sp, #0xc
0037e43c: rsb      r3, r0, r3
0037e440: asr      r3, r3, #4
0037e444: add      r2, r3, r3, lsl #3
0037e448: add      r2, r2, r2, lsl #6
0037e44c: add      r2, r3, r2, lsl #3
0037e450: add      r2, r2, r2, lsl #15
0037e454: add      r3, r3, r2, lsl #3
0037e458: cmp      r3, #0
0037e45c: bne      #0x37e470
0037e460: ldr      r0, [pc, #0xf8]
0037e464: add      r0, pc, r0
0037e468: bl       #0x708eb0
0037e46c: ldr      r0, [r5]
0037e470: bl       #0x31c49c
0037e474: bl       #0x37ba84
0037e478: cmn      r0, #1
0037e47c: mov      r5, r0
0037e480: beq      #0x37e504
0037e484: ldr      r7, [r6, #4]
0037e488: ldr      r2, [pc, #0xd4]
0037e48c: ldm      r7, {r0, r3}
0037e490: ldr      r2, [r4, r2]
0037e494: rsb      r3, r0, r3
0037e498: asr      r3, r3, #4
0037e49c: ldr      r6, [r2]
0037e4a0: add      r2, r3, r3, lsl #3
0037e4a4: add      r2, r2, r2, lsl #6
0037e4a8: add      r2, r3, r2, lsl #3
0037e4ac: add      r2, r2, r2, lsl #15
0037e4b0: add      r3, r3, r2, lsl #3
0037e4b4: rsb      r3, r3, #0
0037e4b8: cmp      r3, #1
0037e4bc: bls      #0x37e50c
0037e4c0: add      r0, r0, #0x70
0037e4c4: bl       #0x31bbf0
0037e4c8: bl       #0x30e4cc
0037e4cc: mov      r1, r5
0037e4d0: str      r0, [sp]
0037e4d4: mov      r2, #1
0037e4d8: mov      r0, r6
0037e4dc: mov      r3, #0
0037e4e0: bl       #0x36bd78
0037e4e4: ldr      r3, [pc, #0x7c]
0037e4e8: ldr      r0, [r4, r3]
0037e4ec: bl       #0x31f594
0037e4f0: cmp      r0, #0
0037e4f4: beq      #0x37e504
0037e4f8: ldr      r3, [r0, #0x11c]
0037e4fc: cmp      r5, r3
0037e500: beq      #0x37e520
0037e504: add      sp, sp, #0xc
0037e508: pop      {r4, r5, r6, r7, pc}
0037e50c: ldr      r0, [pc, #0x58]
0037e510: add      r0, pc, r0
0037e514: bl       #0x708eb0
0037e518: ldr      r0, [r7]
0037e51c: b        #0x37e4c0
0037e520: ldrb     r3, [r6, #0x31]
0037e524: cmp      r3, #0
0037e528: bne      #0x37e544
0037e52c: ldr      r1, [pc, #0x3c]
0037e530: mov      r0, r6
0037e534: add      r1, pc, r1
0037e538: add      sp, sp, #0xc
0037e53c: pop      {r4, r5, r6, r7, lr}
0037e540: b        #0x369514
0037e544: ldr      r1, [pc, #0x28]
0037e548: mov      r0, r6
0037e54c: add      r1, pc, r1
0037e550: add      sp, sp, #0xc
0037e554: pop      {r4, r5, r6, r7, lr}
0037e558: b        #0x369514
0037e55c: rsbeq    r6, r1, ip, asr r6
0037e560: subseq   r0, r4, r4
0037e564: andeq    r0, r0, r4, lsr #27
0037e568: strdeq   r3, r4, [r0], -r4
0037e56c: subseq   pc, r3, r8, asr pc
0037e570: ldrsheq  r3, [r4], #-0x64
0037e574: ldrsbeq  r3, [r4], #-0x64

# _ZN9LuaScript21_GetGameObjectsByTypeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f03c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037f040: ldr      r3, [r0, #4]
0037f044: sub      sp, sp, #0x1c
0037f048: str      r1, [sp, #4]
0037f04c: ldr      r2, [r3, #4]
0037f050: mov      r5, r0
0037f054: ldr      r0, [r3]
0037f058: ldr      r4, [pc, #0x188]
0037f05c: rsb      r3, r0, r2
0037f060: asr      r3, r3, #4
0037f064: add      r4, pc, r4
0037f068: add      r2, r3, r3, lsl #3
0037f06c: add      r2, r2, r2, lsl #6
0037f070: add      r2, r3, r2, lsl #3
0037f074: add      r2, r2, r2, lsl #15
0037f078: add      r3, r3, r2, lsl #3
0037f07c: rsb      r3, r3, #0
0037f080: cmp      r3, #1
0037f084: bls      #0x37f098
0037f088: add      r2, r0, #0x70
0037f08c: ldr      r1, [r2, #4]
0037f090: cmp      r1, #3
0037f094: beq      #0x37f1ac
0037f098: mov      fp, #0
0037f09c: cmp      r3, #0
0037f0a0: bne      #0x37f0ac
0037f0a4: add      sp, sp, #0x1c
0037f0a8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037f0ac: ldr      r3, [r0, #4]
0037f0b0: cmp      r3, #4
0037f0b4: bne      #0x37f0a4
0037f0b8: bl       #0x31c49c
0037f0bc: ldr      r3, [pc, #0x128]
0037f0c0: mov      sb, r0
0037f0c4: ldr      r3, [r4, r3]
0037f0c8: ldr      sl, [r3, #0x38]
0037f0cc: ldr      r4, [sl, #0x14]
0037f0d0: add      sl, sl, #0xc
0037f0d4: cmp      r4, sl
0037f0d8: beq      #0x37f0a4
0037f0dc: mov      r8, #0
0037f0e0: mov      r7, r8
0037f0e4: add      r6, sp, #0xc
0037f0e8: ldr      r1, [r4, #0x2c]
0037f0ec: cmp      r1, #0
0037f0f0: beq      #0x37f140
0037f0f4: mov      r0, r6
0037f0f8: bl       #0x33dd2c
0037f0fc: mov      r0, r6
0037f100: bl       #0x33fee4
0037f104: subs     r5, r0, #0
0037f108: beq      #0x37f140
0037f10c: add      r0, r5, #4
0037f110: bl       #0x510b4c
0037f114: mov      r1, sb
0037f118: bl       #0x30e31c
0037f11c: cmp      r0, #0
0037f120: bne      #0x37f140
0037f124: cmp      fp, r8
0037f128: addhi    r8, r8, #1
0037f12c: bhi      #0x37f140
0037f130: mov      r1, r5
0037f134: ldr      r0, [sp, #4]
0037f138: bl       #0x37c9f8
0037f13c: add      r7, r7, #1
0037f140: ldr      r2, [r4, #0xc]
0037f144: cmp      r2, #0
0037f148: bne      #0x37f154
0037f14c: b        #0x37f178
0037f150: mov      r2, r3
0037f154: ldr      r3, [r2, #8]
0037f158: cmp      r3, #0
0037f15c: bne      #0x37f150
0037f160: mov      r4, r2
0037f164: cmp      sl, r4
0037f168: beq      #0x37f0a4
0037f16c: cmp      r7, #0xe
0037f170: bhi      #0x37f0a4
0037f174: b        #0x37f0e8
0037f178: ldr      r3, [r4, #4]
0037f17c: ldr      r1, [r3, #0xc]
0037f180: cmp      r4, r1
0037f184: bne      #0x37f1a0
0037f188: mov      r4, r3
0037f18c: ldr      r3, [r3, #4]
0037f190: ldr      r2, [r3, #0xc]
0037f194: cmp      r2, r4
0037f198: beq      #0x37f188
0037f19c: ldr      r2, [r4, #0xc]
0037f1a0: cmp      r2, r3
0037f1a4: movne    r4, r3
0037f1a8: b        #0x37f164
0037f1ac: mov      r0, r2
0037f1b0: bl       #0x31bbf0
0037f1b4: bl       #0x8be2a0
0037f1b8: ldr      r3, [r5, #4]
0037f1bc: mov      fp, r0
0037f1c0: ldm      r3, {r0, r2}
0037f1c4: rsb      r3, r0, r2
0037f1c8: asr      r3, r3, #4
0037f1cc: add      r2, r3, r3, lsl #3
0037f1d0: add      r2, r2, r2, lsl #6
0037f1d4: add      r2, r3, r2, lsl #3
0037f1d8: add      r2, r2, r2, lsl #15
0037f1dc: add      r3, r3, r2, lsl #3
0037f1e0: rsb      r3, r3, #0
0037f1e4: b        #0x37f09c
0037f1e8: rsbeq    r5, r1, ip, lsr #20
0037f1ec: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScript21_GetCurrentLevelRangeERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037f1f0: push     {r4, r5, r6, r7, r8, lr}
0037f1f4: ldr      r4, [pc, #0x14c]
0037f1f8: ldr      r3, [pc, #0x14c]
0037f1fc: mov      r7, r0
0037f200: add      r4, pc, r4
0037f204: ldr      r0, [r4, r3]
0037f208: mov      r6, r1
0037f20c: bl       #0x31f594
0037f210: ldr      r5, [r0, #0x3c]
0037f214: cmn      r5, #1
0037f218: beq      #0x37f32c
0037f21c: ldr      r2, [r7, #4]
0037f220: ldm      r2, {r0, r3}
0037f224: rsb      r3, r0, r3
0037f228: asr      r3, r3, #4
0037f22c: add      r2, r3, r3, lsl #3
0037f230: add      r2, r2, r2, lsl #6
0037f234: add      r2, r3, r2, lsl #3
0037f238: add      r2, r2, r2, lsl #15
0037f23c: add      r3, r3, r2, lsl #3
0037f240: cmp      r3, #0
0037f244: bne      #0x37f284
0037f248: mov      r3, #0x48
0037f24c: mul      r5, r3, r5
0037f250: ldr      r3, [pc, #0xf8]
0037f254: mov      r0, r6
0037f258: ldr      r4, [r4, r3]
0037f25c: ldr      r3, [r4]
0037f260: add      r3, r3, r5
0037f264: ldr      r1, [r3, #0x3c]
0037f268: bl       #0x37cb24
0037f26c: ldr      r3, [r4]
0037f270: mov      r0, r6
0037f274: add      r5, r3, r5
0037f278: ldr      r1, [r5, #0x30]
0037f27c: pop      {r4, r5, r6, r7, r8, lr}
0037f280: b        #0x37cb24
0037f284: ldr      r3, [r0, #4]
0037f288: cmp      r3, #3
0037f28c: bne      #0x37f248
0037f290: bl       #0x31bbf0
0037f294: bl       #0x30e4cc
0037f298: cmp      r0, #1
0037f29c: beq      #0x37f2b4
0037f2a0: cmp      r0, #2
0037f2a4: beq      #0x37f2f0
0037f2a8: cmp      r0, #0
0037f2ac: beq      #0x37f248
0037f2b0: pop      {r4, r5, r6, r7, r8, pc}
0037f2b4: mov      r3, #0x48
0037f2b8: mul      r5, r3, r5
0037f2bc: ldr      r3, [pc, #0x8c]
0037f2c0: mov      r0, r6
0037f2c4: ldr      r4, [r4, r3]
0037f2c8: ldr      r3, [r4]
0037f2cc: add      r3, r3, r5
0037f2d0: ldr      r1, [r3, #0x40]
0037f2d4: bl       #0x37cb24
0037f2d8: ldr      r3, [r4]
0037f2dc: mov      r0, r6
0037f2e0: add      r5, r3, r5
0037f2e4: ldr      r1, [r5, #0x34]
0037f2e8: pop      {r4, r5, r6, r7, r8, lr}
0037f2ec: b        #0x37cb24
0037f2f0: mov      r3, #0x48
0037f2f4: mul      r5, r3, r5
0037f2f8: ldr      r3, [pc, #0x50]
0037f2fc: mov      r0, r6
0037f300: ldr      r4, [r4, r3]
0037f304: ldr      r3, [r4]
0037f308: add      r3, r3, r5
0037f30c: ldr      r1, [r3, #0x44]
0037f310: bl       #0x37cb24
0037f314: ldr      r3, [r4]
0037f318: mov      r0, r6
0037f31c: add      r5, r3, r5
0037f320: ldr      r1, [r5, #0x38]
0037f324: pop      {r4, r5, r6, r7, r8, lr}
0037f328: b        #0x37cb24
0037f32c: mov      r0, r6
0037f330: mov      r1, r5
0037f334: bl       #0x37cb24
0037f338: mov      r0, r6
0037f33c: mov      r1, r5
0037f340: pop      {r4, r5, r6, r7, r8, lr}
0037f344: b        #0x37cb24
0037f348: mlseq    r1, r0, r8, r5
0037f34c: strdeq   r3, r4, [r0], -r4
0037f350: andeq    r0, r0, r4, ror r8

# _ZN9LuaScript5_RandERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037df30: push     {r4, r5, r6, lr}
0037df34: ldr      r5, [r0, #4]
0037df38: mov      r6, r1
0037df3c: mov      r4, r0
0037df40: ldm      r5, {r1, r3}
0037df44: rsb      r3, r1, r3
0037df48: asr      r3, r3, #4
0037df4c: add      r2, r3, r3, lsl #3
0037df50: add      r2, r2, r2, lsl #6
0037df54: add      r2, r3, r2, lsl #3
0037df58: add      r2, r2, r2, lsl #15
0037df5c: add      r3, r3, r2, lsl #3
0037df60: rsb      r3, r3, #0
0037df64: cmp      r3, #1
0037df68: bls      #0x37e048
0037df6c: cmp      r3, #0
0037df70: beq      #0x37e034
0037df74: ldr      r3, [r1, #4]
0037df78: cmp      r3, #3
0037df7c: beq      #0x37e04c
0037df80: ldr      r5, [r4, #4]
0037df84: ldm      r5, {r0, r3}
0037df88: rsb      r3, r0, r3
0037df8c: asr      r3, r3, #4
0037df90: add      r2, r3, r3, lsl #3
0037df94: add      r2, r2, r2, lsl #6
0037df98: add      r2, r3, r2, lsl #3
0037df9c: add      r2, r2, r2, lsl #15
0037dfa0: add      r3, r3, r2, lsl #3
0037dfa4: cmp      r3, #0
0037dfa8: beq      #0x37e020
0037dfac: bl       #0x31bbf0
0037dfb0: bl       #0x30e4cc
0037dfb4: ldr      r4, [r4, #4]
0037dfb8: mov      r5, r0
0037dfbc: ldm      r4, {r0, r3}
0037dfc0: rsb      r3, r0, r3
0037dfc4: asr      r3, r3, #4
0037dfc8: add      r2, r3, r3, lsl #3
0037dfcc: add      r2, r2, r2, lsl #6
0037dfd0: add      r2, r3, r2, lsl #3
0037dfd4: add      r2, r2, r2, lsl #15
0037dfd8: add      r3, r3, r2, lsl #3
0037dfdc: rsb      r3, r3, #0
0037dfe0: cmp      r3, #1
0037dfe4: bls      #0x37e00c
0037dfe8: add      r0, r0, #0x70
0037dfec: bl       #0x31bbf0
0037dff0: bl       #0x30e4cc
0037dff4: rsb      r0, r5, r0
0037dff8: bl       #0x37bc2c
0037dffc: add      r1, r0, r5
0037e000: mov      r0, r6
0037e004: pop      {r4, r5, r6, lr}
0037e008: b        #0x37cb24
0037e00c: ldr      r0, [pc, #0x48]
0037e010: add      r0, pc, r0
0037e014: bl       #0x708eb0
0037e018: ldr      r0, [r4]
0037e01c: b        #0x37dfe8
0037e020: ldr      r0, [pc, #0x38]
0037e024: add      r0, pc, r0
0037e028: bl       #0x708eb0
0037e02c: ldr      r0, [r5]
0037e030: b        #0x37dfac
0037e034: ldr      r0, [pc, #0x28]
0037e038: add      r0, pc, r0
0037e03c: bl       #0x708eb0
0037e040: ldr      r1, [r5]
0037e044: b        #0x37df74
0037e048: pop      {r4, r5, r6, pc}
0037e04c: mov      r0, r4
0037e050: mov      r1, #1
0037e054: bl       #0x37baf8
0037e058: b        #0x37df80
0037e05c: subseq   r0, r4, r8, asr r4
0037e060: subseq   r0, r4, r4, asr #8
0037e064: subseq   r0, r4, r0, lsr r4

# _ZN9LuaScript6_RandFERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e2d8: push     {r4, r5, r6, lr}
0037e2dc: ldr      r5, [r0, #4]
0037e2e0: mov      r6, r1
0037e2e4: mov      r4, r0
0037e2e8: ldm      r5, {r1, r3}
0037e2ec: rsb      r3, r1, r3
0037e2f0: asr      r3, r3, #4
0037e2f4: add      r2, r3, r3, lsl #3
0037e2f8: add      r2, r2, r2, lsl #6
0037e2fc: add      r2, r3, r2, lsl #3
0037e300: add      r2, r2, r2, lsl #15
0037e304: add      r3, r3, r2, lsl #3
0037e308: rsb      r3, r3, #0
0037e30c: cmp      r3, #1
0037e310: bls      #0x37e400
0037e314: cmp      r3, #0
0037e318: beq      #0x37e3ec
0037e31c: ldr      r3, [r1, #4]
0037e320: cmp      r3, #3
0037e324: beq      #0x37e404
0037e328: ldr      r5, [r4, #4]
0037e32c: ldm      r5, {r0, r3}
0037e330: rsb      r3, r0, r3
0037e334: asr      r3, r3, #4
0037e338: add      r2, r3, r3, lsl #3
0037e33c: add      r2, r2, r2, lsl #6
0037e340: add      r2, r3, r2, lsl #3
0037e344: add      r2, r2, r2, lsl #15
0037e348: add      r3, r3, r2, lsl #3
0037e34c: cmp      r3, #0
0037e350: beq      #0x37e3d8
0037e354: bl       #0x31bbf0
0037e358: ldr      r4, [r4, #4]
0037e35c: mov      r5, r0
0037e360: ldm      r4, {r0, r3}
0037e364: rsb      r3, r0, r3
0037e368: asr      r3, r3, #4
0037e36c: add      r2, r3, r3, lsl #3
0037e370: add      r2, r2, r2, lsl #6
0037e374: add      r2, r3, r2, lsl #3
0037e378: add      r2, r2, r2, lsl #15
0037e37c: add      r3, r3, r2, lsl #3
0037e380: rsb      r3, r3, #0
0037e384: cmp      r3, #1
0037e388: bls      #0x37e3c4
0037e38c: add      r0, r0, #0x70
0037e390: bl       #0x31bbf0
0037e394: mov      r1, r5
0037e398: bl       #0x30e3ac
0037e39c: bl       #0x30e4cc
0037e3a0: bl       #0x37bc2c
0037e3a4: bl       #0x30e964
0037e3a8: mov      r1, r0
0037e3ac: mov      r0, r5
0037e3b0: bl       #0x30eba4
0037e3b4: mov      r1, r0
0037e3b8: mov      r0, r6
0037e3bc: pop      {r4, r5, r6, lr}
0037e3c0: b        #0x37ccbc
0037e3c4: ldr      r0, [pc, #0x48]
0037e3c8: add      r0, pc, r0
0037e3cc: bl       #0x708eb0
0037e3d0: ldr      r0, [r4]
0037e3d4: b        #0x37e38c
0037e3d8: ldr      r0, [pc, #0x38]
0037e3dc: add      r0, pc, r0
0037e3e0: bl       #0x708eb0
0037e3e4: ldr      r0, [r5]
0037e3e8: b        #0x37e354
0037e3ec: ldr      r0, [pc, #0x28]
0037e3f0: add      r0, pc, r0
0037e3f4: bl       #0x708eb0
0037e3f8: ldr      r1, [r5]
0037e3fc: b        #0x37e31c
0037e400: pop      {r4, r5, r6, pc}
0037e404: mov      r0, r4
0037e408: mov      r1, #1
0037e40c: bl       #0x37baf8
0037e410: b        #0x37e328
0037e414: subseq   r0, r4, r0, lsr #1
0037e418: subseq   r0, r4, ip, lsl #1
0037e41c: subseq   r0, r4, r8, ror r0

# _ZN9LuaScript6_BitOrERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037e814: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037e818: ldr      r4, [r0, #4]
0037e81c: mov      sl, r1
0037e820: sub      sp, sp, #4
0037e824: ldm      r4, {r2, r3}
0037e828: mov      r5, r0
0037e82c: rsb      r3, r2, r3
0037e830: asr      r1, r3, #4
0037e834: add      r8, r1, r1, lsl #3
0037e838: add      r8, r8, r8, lsl #6
0037e83c: add      r8, r1, r8, lsl #3
0037e840: add      r8, r8, r8, lsl #15
0037e844: add      r8, r1, r8, lsl #3
0037e848: rsb      r8, r8, #0
0037e84c: cmp      r8, #1
0037e850: bls      #0x37e9d8
0037e854: ldr      sb, [pc, #0x184]
0037e858: mov      r7, #0
0037e85c: mov      r6, r7
0037e860: add      sb, pc, sb
0037e864: b        #0x37e874
0037e868: ldr      r4, [r5, #4]
0037e86c: ldm      r4, {r2, r3}
0037e870: rsb      r3, r2, r3
0037e874: asr      r3, r3, #4
0037e878: add      r1, r3, r3, lsl #3
0037e87c: add      r1, r1, r1, lsl #6
0037e880: add      r1, r3, r1, lsl #3
0037e884: add      r1, r1, r1, lsl #15
0037e888: add      r3, r3, r1, lsl #3
0037e88c: rsb      r3, r3, #0
0037e890: cmp      r6, r3
0037e894: add      r6, r6, #1
0037e898: blo      #0x37e8a8
0037e89c: mov      r0, sb
0037e8a0: bl       #0x708eb0
0037e8a4: ldr      r2, [r4]
0037e8a8: add      r2, r2, r7
0037e8ac: ldr      r3, [r2, #4]
0037e8b0: add      r7, r7, #0x70
0037e8b4: cmp      r3, #3
0037e8b8: bne      #0x37e9d8
0037e8bc: cmp      r6, r8
0037e8c0: bne      #0x37e868
0037e8c4: ldr      r4, [r5, #4]
0037e8c8: ldm      r4, {r0, r3}
0037e8cc: rsb      r3, r0, r3
0037e8d0: asr      r3, r3, #4
0037e8d4: add      r2, r3, r3, lsl #3
0037e8d8: add      r2, r2, r2, lsl #6
0037e8dc: add      r2, r3, r2, lsl #3
0037e8e0: add      r2, r2, r2, lsl #15
0037e8e4: add      r3, r3, r2, lsl #3
0037e8e8: cmp      r3, #0
0037e8ec: bne      #0x37e900
0037e8f0: ldr      r0, [pc, #0xec]
0037e8f4: add      r0, pc, r0
0037e8f8: bl       #0x708eb0
0037e8fc: ldr      r0, [r4]
0037e900: bl       #0x31bbf0
0037e904: bl       #0x30e4cc
0037e908: ldr      r2, [r5, #4]
0037e90c: mov      r8, r0
0037e910: ldm      r2, {r2, r3}
0037e914: rsb      r3, r2, r3
0037e918: asr      r3, r3, #4
0037e91c: add      sb, r3, r3, lsl #3
0037e920: add      sb, sb, sb, lsl #6
0037e924: add      sb, r3, sb, lsl #3
0037e928: add      sb, sb, sb, lsl #15
0037e92c: add      sb, r3, sb, lsl #3
0037e930: rsb      sb, sb, #0
0037e934: cmp      sb, #1
0037e938: bls      #0x37e9c4
0037e93c: ldr      fp, [pc, #0xa4]
0037e940: mov      r7, #0x70
0037e944: mov      r4, #1
0037e948: add      fp, pc, fp
0037e94c: add      r0, r2, r7
0037e950: bl       #0x31bbf0
0037e954: bl       #0x30e4cc
0037e958: add      r4, r4, #1
0037e95c: cmp      r4, sb
0037e960: orr      r8, r8, r0
0037e964: beq      #0x37e9c4
0037e968: ldr      r6, [r5, #4]
0037e96c: mov      r0, fp
0037e970: add      r7, r7, #0x70
0037e974: ldm      r6, {r2, r3}
0037e978: rsb      r3, r2, r3
0037e97c: asr      r3, r3, #4
0037e980: add      r1, r3, r3, lsl #3
0037e984: add      r1, r1, r1, lsl #6
0037e988: add      r1, r3, r1, lsl #3
0037e98c: add      r1, r1, r1, lsl #15
0037e990: add      r3, r3, r1, lsl #3
0037e994: rsb      r3, r3, #0
0037e998: cmp      r4, r3
0037e99c: blo      #0x37e94c
0037e9a0: bl       #0x708eb0
0037e9a4: ldr      r2, [r6]
0037e9a8: add      r4, r4, #1
0037e9ac: add      r0, r2, r7
0037e9b0: bl       #0x31bbf0
0037e9b4: bl       #0x30e4cc
0037e9b8: cmp      r4, sb
0037e9bc: orr      r8, r8, r0
0037e9c0: bne      #0x37e968
0037e9c4: mov      r0, sl
0037e9c8: mov      r1, r8
0037e9cc: add      sp, sp, #4
0037e9d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0037e9d4: b        #0x37cb24
0037e9d8: add      sp, sp, #4
0037e9dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0037e9e0: subseq   pc, r3, r8, lsl #24
0037e9e4: subseq   pc, r3, r4, ror fp
0037e9e8: subseq   pc, r3, r0, lsr #22

# _ZN9LuaScript14_GetNumPlayersERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037cbd8: ldr      r3, [pc, #0x18]
0037cbdc: ldr      r2, [pc, #0x18]
0037cbe0: mov      r0, r1
0037cbe4: add      r3, pc, r3
0037cbe8: ldr      r2, [r3, r2]
0037cbec: ldr      r3, [r2, #0x40]
0037cbf0: ldr      r1, [r3, #0x6c4]
0037cbf4: b        #0x37cb24
0037cbf8: rsbeq    r7, r1, ip, lsr #29
0037cbfc: strdeq   r3, r4, [r0], -r4

# _ZN9LuaScript10_FromFixedERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0037ee84: push     {r4, r5, r6, lr}
0037ee88: ldr      r3, [r0, #4]
0037ee8c: mov      r4, r0
0037ee90: mov      r5, r1
0037ee94: ldm      r3, {r0, r2}
0037ee98: rsb      r3, r0, r2
0037ee9c: asr      r3, r3, #4
0037eea0: add      r2, r3, r3, lsl #3
0037eea4: add      r2, r2, r2, lsl #6
0037eea8: add      r2, r3, r2, lsl #3
0037eeac: add      r2, r2, r2, lsl #15
0037eeb0: add      r3, r3, r2, lsl #3
0037eeb4: cmp      r3, #0
0037eeb8: bne      #0x37eec0
0037eebc: pop      {r4, r5, r6, pc}
0037eec0: bl       #0x31bbf0
0037eec4: bl       #0x30e4cc
0037eec8: asr      r1, r0, #8
0037eecc: mov      r0, r5
0037eed0: bl       #0x37cb24
0037eed4: ldr      r4, [r4, #4]
0037eed8: ldm      r4, {r0, r3}
0037eedc: rsb      r3, r0, r3
0037eee0: asr      r3, r3, #4
0037eee4: add      r2, r3, r3, lsl #3
0037eee8: add      r2, r2, r2, lsl #6
0037eeec: add      r2, r3, r2, lsl #3
0037eef0: add      r2, r2, r2, lsl #15
0037eef4: add      r3, r3, r2, lsl #3
0037eef8: cmp      r3, #0
0037eefc: beq      #0x37ef24
0037ef00: bl       #0x31bbf0
0037ef04: bl       #0x30e4cc
0037ef08: bl       #0x30e964
0037ef0c: mov      r1, #0x3b800000
0037ef10: bl       #0x30ed6c
0037ef14: mov      r1, r0
0037ef18: mov      r0, r5
0037ef1c: pop      {r4, r5, r6, lr}
0037ef20: b        #0x37ccbc
0037ef24: ldr      r0, [pc, #0xc]
0037ef28: add      r0, pc, r0
0037ef2c: bl       #0x708eb0
0037ef30: ldr      r0, [r4]
0037ef34: b        #0x37ef00
0037ef38: subseq   pc, r3, r0, asr #10
