
# _ZN9Character8_SetPropERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b7eac: push     {r4, r5, r6, r7, r8, lr}
003b7eb0: ldr      r5, [r0, #4]
003b7eb4: mov      r6, r2
003b7eb8: mov      r4, r0
003b7ebc: ldm      r5, {r1, r3}
003b7ec0: rsb      r3, r1, r3
003b7ec4: asr      r3, r3, #4
003b7ec8: add      r2, r3, r3, lsl #3
003b7ecc: add      r2, r2, r2, lsl #6
003b7ed0: add      r2, r3, r2, lsl #3
003b7ed4: add      r2, r2, r2, lsl #15
003b7ed8: add      r3, r3, r2, lsl #3
003b7edc: rsb      r3, r3, #0
003b7ee0: cmp      r3, #1
003b7ee4: bls      #0x3b7efc
003b7ee8: cmp      r3, #0
003b7eec: beq      #0x3b7f00
003b7ef0: ldr      r3, [r1, #4]
003b7ef4: cmp      r3, #3
003b7ef8: beq      #0x3b7f14
003b7efc: pop      {r4, r5, r6, r7, r8, pc}
003b7f00: ldr      r0, [pc, #0x188]
003b7f04: add      r0, pc, r0
003b7f08: bl       #0x708eb0
003b7f0c: ldr      r1, [r5]
003b7f10: b        #0x3b7ef0
003b7f14: mov      r1, #0
003b7f18: mov      r0, r4
003b7f1c: bl       #0x37baf8
003b7f20: bl       #0x38d798
003b7f24: mov      r1, #0
003b7f28: mov      r0, r4
003b7f2c: bl       #0x37baf8
003b7f30: bl       #0x38d798
003b7f34: cmp      r0, #0xdf
003b7f38: bhi      #0x3b7efc
003b7f3c: ldr      r5, [r4, #4]
003b7f40: ldr      r3, [r5]
003b7f44: ldr      r2, [r5, #4]
003b7f48: rsb      r2, r3, r2
003b7f4c: asr      r2, r2, #4
003b7f50: add      r1, r2, r2, lsl #3
003b7f54: add      r1, r1, r1, lsl #6
003b7f58: add      r1, r2, r1, lsl #3
003b7f5c: add      r1, r1, r1, lsl #15
003b7f60: add      r2, r2, r1, lsl #3
003b7f64: rsb      r2, r2, #0
003b7f68: cmp      r2, #1
003b7f6c: bhi      #0x3b7f80
003b7f70: ldr      r0, [pc, #0x11c]
003b7f74: add      r0, pc, r0
003b7f78: bl       #0x708eb0
003b7f7c: ldr      r3, [r5]
003b7f80: ldr      r3, [r3, #0x74]
003b7f84: cmp      r3, #3
003b7f88: bne      #0x3b7efc
003b7f8c: ldr      r2, [r4, #4]
003b7f90: ldr      r3, [r2]
003b7f94: ldr      r2, [r2, #4]
003b7f98: rsb      r3, r3, r2
003b7f9c: asr      r3, r3, #4
003b7fa0: add      r2, r3, r3, lsl #3
003b7fa4: add      r2, r2, r2, lsl #6
003b7fa8: add      r2, r3, r2, lsl #3
003b7fac: add      r2, r2, r2, lsl #15
003b7fb0: add      r3, r3, r2, lsl #3
003b7fb4: rsb      r3, r3, #0
003b7fb8: cmp      r3, #2
003b7fbc: bhi      #0x3b8000
003b7fc0: mov      r1, #0
003b7fc4: mov      r0, r4
003b7fc8: bl       #0x37baf8
003b7fcc: bl       #0x38d798
003b7fd0: mov      r1, #1
003b7fd4: mov      r5, r0
003b7fd8: mov      r0, r4
003b7fdc: bl       #0x37baf8
003b7fe0: bl       #0x31bbf0
003b7fe4: bl       #0x30e4cc
003b7fe8: add      r6, r6, #0x560
003b7fec: mov      r2, r0
003b7ff0: mov      r1, r5
003b7ff4: mov      r0, r6
003b7ff8: pop      {r4, r5, r6, r7, r8, lr}
003b7ffc: b        #0x3e07a0
003b8000: mov      r0, r4
003b8004: mov      r1, #2
003b8008: bl       #0x37baf8
003b800c: ldr      r5, [r0, #4]
003b8010: cmp      r5, #2
003b8014: bne      #0x3b7fc0
003b8018: mov      r1, r5
003b801c: mov      r0, r4
003b8020: bl       #0x37baf8
003b8024: bl       #0x31b580
003b8028: cmp      r0, #0
003b802c: beq      #0x3b7efc
003b8030: mov      r1, #0
003b8034: mov      r0, r4
003b8038: bl       #0x37baf8
003b803c: bl       #0x38d798
003b8040: mov      r1, #1
003b8044: mov      r7, r0
003b8048: mov      r0, r4
003b804c: bl       #0x37baf8
003b8050: bl       #0x31bbf0
003b8054: mov      r1, r5
003b8058: mov      r8, r0
003b805c: mov      r0, r4
003b8060: bl       #0x37baf8
003b8064: bl       #0x31b580
003b8068: mov      r4, r0
003b806c: mov      r0, r8
003b8070: bl       #0x30e4cc
003b8074: add      r6, r6, #0x560
003b8078: mov      r2, r0
003b807c: mov      r1, r7
003b8080: mov      r0, r6
003b8084: mov      r3, r4
003b8088: pop      {r4, r5, r6, r7, r8, lr}
003b808c: b        #0x3e0614
003b8090: subseq   r6, r0, r4, ror #10
003b8094: ldrsheq  r6, [r0], #-0x44

# _ZN9Character11_ClearPropsERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b82e4: push     {r4, r5, lr}
003b82e8: ldr      r3, [r0, #4]
003b82ec: sub      sp, sp, #0xc
003b82f0: mov      r4, r0
003b82f4: ldr      r1, [r3, #4]
003b82f8: ldr      ip, [r3]
003b82fc: rsb      r3, ip, r1
003b8300: asr      r3, r3, #4
003b8304: add      r1, r3, r3, lsl #3
003b8308: add      r1, r1, r1, lsl #6
003b830c: add      r1, r3, r1, lsl #3
003b8310: add      r1, r1, r1, lsl #15
003b8314: add      r3, r3, r1, lsl #3
003b8318: cmp      r3, #0
003b831c: bne      #0x3b8328
003b8320: add      sp, sp, #0xc
003b8324: pop      {r4, r5, pc}
003b8328: ldr      r3, [ip, #4]
003b832c: cmp      r3, #2
003b8330: beq      #0x3b8370
003b8334: cmp      r3, #1
003b8338: bne      #0x3b8320
003b833c: mov      r1, #0
003b8340: mov      r0, r4
003b8344: str      r2, [sp, #4]
003b8348: bl       #0x37baf8
003b834c: bl       #0x31bc80
003b8350: cmp      r0, #0
003b8354: ldr      r2, [sp, #4]
003b8358: beq      #0x3b8320
003b835c: add      r0, r2, #0x560
003b8360: mov      r1, #0
003b8364: add      sp, sp, #0xc
003b8368: pop      {r4, r5, lr}
003b836c: b        #0x3def84
003b8370: mov      r1, #0
003b8374: str      r2, [sp, #4]
003b8378: bl       #0x37baf8
003b837c: bl       #0x31b580
003b8380: cmp      r0, #0
003b8384: ldr      r2, [sp, #4]
003b8388: beq      #0x3b83f0
003b838c: ldr      r5, [r4, #4]
003b8390: add      r4, r2, #0x560
003b8394: ldm      r5, {r0, r3}
003b8398: rsb      r3, r0, r3
003b839c: asr      r3, r3, #4
003b83a0: add      r2, r3, r3, lsl #3
003b83a4: add      r2, r2, r2, lsl #6
003b83a8: add      r2, r3, r2, lsl #3
003b83ac: add      r2, r2, r2, lsl #15
003b83b0: add      r3, r3, r2, lsl #3
003b83b4: cmp      r3, #0
003b83b8: bne      #0x3b83cc
003b83bc: ldr      r0, [pc, #0x64]
003b83c0: add      r0, pc, r0
003b83c4: bl       #0x708eb0
003b83c8: ldr      r0, [r5]
003b83cc: bl       #0x31b580
003b83d0: mov      r1, r0
003b83d4: mov      r0, r4
003b83d8: bl       #0x3def84
003b83dc: mov      r0, r4
003b83e0: mov      r1, #1
003b83e4: add      sp, sp, #0xc
003b83e8: pop      {r4, r5, lr}
003b83ec: b        #0x3e0810
003b83f0: ldr      r3, [r4, #4]
003b83f4: ldr      r1, [r3, #4]
003b83f8: ldr      ip, [r3]
003b83fc: rsb      r3, ip, r1
003b8400: asr      r3, r3, #4
003b8404: add      r1, r3, r3, lsl #3
003b8408: add      r1, r1, r1, lsl #6
003b840c: add      r1, r3, r1, lsl #3
003b8410: add      r1, r1, r1, lsl #15
003b8414: add      r3, r3, r1, lsl #3
003b8418: cmp      r3, #0
003b841c: ldrne    r3, [ip, #4]
003b8420: bne      #0x3b8334
003b8424: b        #0x3b8320
003b8428: subseq   r6, r0, r8, lsr #1

# _ZNK10CharTimers12TMR_TimeLeftEjRjS0_
003db344: ldr      ip, [r0, #0xc]
003db348: ldr      r0, [r0, #8]
003db34c: rsb      ip, r0, ip
003db350: cmp      r1, ip, asr #5
003db354: bhs      #0x3db380
003db358: add      r1, r0, r1, lsl #5
003db35c: ldrb     r0, [r1, #0x14]
003db360: cmp      r0, #0
003db364: beq      #0x3db380
003db368: ldr      ip, [r1, #0x10]
003db36c: mov      r0, #1
003db370: str      ip, [r2]
003db374: ldr      r2, [r1, #0xc]
003db378: str      r2, [r3]
003db37c: bx       lr
003db380: mov      r0, #0
003db384: bx       lr

# _ZN9Character15_ApplyPropClassERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003babdc: push     {r4, r5, r6, r7, r8, lr}
003babe0: ldr      r3, [r0, #4]
003babe4: mov      r6, r2
003babe8: ldr      r4, [pc, #0x1ac]
003babec: ldm      r3, {r1, r2}
003babf0: add      r4, pc, r4
003babf4: mov      r5, r0
003babf8: rsb      r3, r1, r2
003babfc: asr      r3, r3, #4
003bac00: add      r2, r3, r3, lsl #3
003bac04: add      r2, r2, r2, lsl #6
003bac08: add      r2, r3, r2, lsl #3
003bac0c: add      r2, r2, r2, lsl #15
003bac10: add      r3, r3, r2, lsl #3
003bac14: cmp      r3, #0
003bac18: bne      #0x3bac20
003bac1c: pop      {r4, r5, r6, r7, r8, pc}
003bac20: ldr      r3, [r1, #4]
003bac24: cmp      r3, #3
003bac28: bne      #0x3bac1c
003bac2c: mov      r1, #0
003bac30: bl       #0x37baf8
003bac34: bl       #0x38d798
003bac38: ldr      r3, [pc, #0x160]
003bac3c: ldr      r3, [r4, r3]
003bac40: ldr      r3, [r3]
003bac44: cmp      r0, r3
003bac48: bhi      #0x3bac1c
003bac4c: ldr      r7, [r5, #4]
003bac50: ldm      r7, {r0, r3}
003bac54: rsb      r3, r0, r3
003bac58: asr      r3, r3, #4
003bac5c: add      r2, r3, r3, lsl #3
003bac60: add      r2, r2, r2, lsl #6
003bac64: add      r2, r3, r2, lsl #3
003bac68: add      r2, r2, r2, lsl #15
003bac6c: add      r3, r3, r2, lsl #3
003bac70: rsb      r3, r3, #0
003bac74: cmp      r3, #1
003bac78: bls      #0x3bacc8
003bac7c: ldr      r3, [r0, #0x74]
003bac80: cmp      r3, #2
003bac84: beq      #0x3bad04
003bac88: mov      r0, r5
003bac8c: mov      r1, #1
003bac90: bl       #0x37baf8
003bac94: ldr      r4, [r0, #4]
003bac98: cmp      r4, #1
003bac9c: beq      #0x3bad64
003baca0: ldr      r7, [r5, #4]
003baca4: ldm      r7, {r0, r3}
003baca8: rsb      r3, r0, r3
003bacac: asr      r3, r3, #4
003bacb0: add      r2, r3, r3, lsl #3
003bacb4: add      r2, r2, r2, lsl #6
003bacb8: add      r2, r3, r2, lsl #3
003bacbc: add      r2, r2, r2, lsl #15
003bacc0: add      r3, r3, r2, lsl #3
003bacc4: rsb      r3, r3, #0
003bacc8: cmp      r3, #0
003baccc: add      r6, r6, #0x560
003bacd0: beq      #0x3bacf0
003bacd4: bl       #0x31bbf0
003bacd8: bl       #0x8be2a0
003bacdc: mov      r2, #0
003bace0: mov      r1, r0
003bace4: mov      r0, r6
003bace8: pop      {r4, r5, r6, r7, r8, lr}
003bacec: b        #0x3df3b8
003bacf0: ldr      r0, [pc, #0xac]
003bacf4: add      r0, pc, r0
003bacf8: bl       #0x708eb0
003bacfc: ldr      r0, [r7]
003bad00: b        #0x3bacd4
003bad04: mov      r1, #1
003bad08: mov      r0, r5
003bad0c: bl       #0x37baf8
003bad10: bl       #0x31b580
003bad14: cmp      r0, #0
003bad18: beq      #0x3bac1c
003bad1c: mov      r1, #0
003bad20: mov      r0, r5
003bad24: bl       #0x37baf8
003bad28: bl       #0x38d798
003bad2c: mov      r1, #1
003bad30: mov      r7, r0
003bad34: mov      r0, r5
003bad38: bl       #0x37baf8
003bad3c: bl       #0x31b580
003bad40: add      r4, r6, #0x560
003bad44: mov      r2, r0
003bad48: mov      r1, r7
003bad4c: mov      r0, r4
003bad50: bl       #0x3df314
003bad54: mov      r0, r4
003bad58: mov      r1, #1
003bad5c: pop      {r4, r5, r6, r7, r8, lr}
003bad60: b        #0x3e0810
003bad64: mov      r1, #0
003bad68: mov      r0, r5
003bad6c: bl       #0x37baf8
003bad70: bl       #0x38d798
003bad74: mov      r1, r4
003bad78: mov      r7, r0
003bad7c: mov      r0, r5
003bad80: bl       #0x37baf8
003bad84: bl       #0x31bc80
003bad88: mov      r1, r7
003bad8c: mov      r2, r0
003bad90: add      r0, r6, #0x560
003bad94: pop      {r4, r5, r6, r7, r8, lr}
003bad98: b        #0x3df3b8
003bad9c: subseq   sb, sp, r0, lsr #29
003bada0: andeq    r3, r0, r8, ror #10
003bada4: subseq   r3, r0, r4, ror r7
