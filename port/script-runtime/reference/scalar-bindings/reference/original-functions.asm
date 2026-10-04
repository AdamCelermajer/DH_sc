
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

# _ZNK3sfc6script3lua9ArgumentsixEj
0037baf8: push     {r4, r5, r6, lr}
0037bafc: ldr      r4, [r0, #4]
0037bb00: mov      r5, r1
0037bb04: ldm      r4, {r2, r3}
0037bb08: rsb      r3, r2, r3
0037bb0c: asr      r3, r3, #4
0037bb10: add      r1, r3, r3, lsl #3
0037bb14: add      r1, r1, r1, lsl #6
0037bb18: add      r1, r3, r1, lsl #3
0037bb1c: add      r1, r1, r1, lsl #15
0037bb20: add      r3, r3, r1, lsl #3
0037bb24: rsb      r3, r3, #0
0037bb28: cmp      r5, r3
0037bb2c: blo      #0x37bb40
0037bb30: ldr      r0, [pc, #0x14]
0037bb34: add      r0, pc, r0
0037bb38: bl       #0x708eb0
0037bb3c: ldr      r2, [r4]
0037bb40: mov      r0, #0x70
0037bb44: mla      r0, r0, r5, r2
0037bb48: pop      {r4, r5, r6, pc}
0037bb4c: subseq   r2, r4, r4, lsr sb

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

# _ZN3sfc6script3lua12ReturnValues10pushNumberEf
0037ccbc: ldr      r3, [pc, #0x58]
0037ccc0: ldr      r2, [pc, #0x58]
0037ccc4: push     {r4, r5, r6, lr}
0037ccc8: add      r3, pc, r3
0037cccc: ldr      r5, [r3, r2]
0037ccd0: sub      sp, sp, #0x78
0037ccd4: add      r4, sp, #4
0037ccd8: ldr      r3, [r5]
0037ccdc: str      r3, [sp, #0x74]
0037cce0: ldr      r6, [r0, #0x24]
0037cce4: mov      r0, r4
0037cce8: bl       #0x37cc3c
0037ccec: mov      r0, r6
0037ccf0: mov      r1, r4
0037ccf4: bl       #0x3195c0
0037ccf8: mov      r0, r4
0037ccfc: bl       #0x3193e8
0037cd00: ldr      r2, [sp, #0x74]
0037cd04: ldr      r3, [r5]
0037cd08: cmp      r2, r3
0037cd0c: bne      #0x37cd18
0037cd10: add      sp, sp, #0x78
0037cd14: pop      {r4, r5, r6, pc}
0037cd18: bl       #0x30e310
0037cd1c: rsbeq    r7, r1, r8, asr #27
0037cd20: andeq    r4, r0, ip, lsr #1

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
