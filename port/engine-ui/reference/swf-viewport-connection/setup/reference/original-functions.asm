
# _ZN8RenderFX10InitializeERNS_24InitializationParametersE
007a9814: push     {r4, r5, r6, r7, r8, lr}
007a9818: ldr      r4, [pc, #0xbc]
007a981c: ldr      r5, [pc, #0xbc]
007a9820: mov      r6, r0
007a9824: add      r4, pc, r4
007a9828: ldr      r3, [r4, r5]
007a982c: ldr      r3, [r3]
007a9830: cmp      r3, #0
007a9834: beq      #0x7a983c
007a9838: pop      {r4, r5, r6, r7, r8, pc}
007a983c: ldr      r0, [pc, #0xa0]
007a9840: add      r0, pc, r0
007a9844: bl       #0x76c740
007a9848: ldr      r0, [pc, #0x98]
007a984c: add      r0, pc, r0
007a9850: bl       #0x76c7c4
007a9854: bl       #0x759b24
007a9858: cmp      r0, #0
007a985c: bne      #0x7a98cc
007a9860: ldr      r0, [pc, #0x84]
007a9864: add      r0, pc, r0
007a9868: bl       #0x76c7c4
007a986c: ldr      r0, [r6]
007a9870: bl       #0x7d6658
007a9874: ldr      r3, [pc, #0x74]
007a9878: mov      r7, r0
007a987c: ldr      r3, [r4, r3]
007a9880: str      r0, [r3]
007a9884: ldr      r3, [r0]
007a9888: mov      lr, pc
007a988c: ldr      pc, [r3, #0xa0]
007a9890: mov      r0, r7
007a9894: ldr      r3, [r7]
007a9898: mov      r1, #1
007a989c: mov      lr, pc
007a98a0: ldr      pc, [r3, #0x84]
007a98a4: ldr      r0, [r6, #4]
007a98a8: cmp      r0, #0
007a98ac: beq      #0x7a98b4
007a98b0: bl       #0x759cdc
007a98b4: bl       #0x76f5c0
007a98b8: mov      r0, r6
007a98bc: bl       #0x7a9708
007a98c0: ldr      r3, [r4, r5]
007a98c4: str      r0, [r3]
007a98c8: pop      {r4, r5, r6, r7, r8, pc}
007a98cc: ldr      r0, [pc, #0x20]
007a98d0: add      r0, pc, r0
007a98d4: bl       #0x761170
007a98d8: b        #0x7a9860
007a98dc: andseq   fp, lr, ip, ror #4
007a98e0: andeq    r4, r0, r0, lsr sb
007a98e4: andeq    r0, r0, ip, lsl #2

# _ZN8RenderFX13CreateContextERNS_24InitializationParametersE
007a9708: push     {r4, r5, r6, r7, lr}
007a970c: mov      r1, #0
007a9710: mov      r4, r0
007a9714: sub      sp, sp, #0xc
007a9718: mov      r0, #0x2c
007a971c: bl       #0x752ba8
007a9720: mov      r5, r0
007a9724: bl       #0x76ce38
007a9728: mov      r1, #0
007a972c: mov      r0, #0x2c
007a9730: bl       #0x752ba8
007a9734: ldr      ip, [r4, #0x20]
007a9738: ldr      r2, [r4, #0x10]
007a973c: ldrb     r3, [r4, #0x1c]
007a9740: ldr      r1, [r4, #0xc]
007a9744: mov      r6, r0
007a9748: str      ip, [sp]
007a974c: bl       #0x7d0dfc
007a9750: str      r6, [r5, #0xc]
007a9754: mov      r1, #0
007a9758: mov      r0, #0x10
007a975c: bl       #0x752ba8
007a9760: ldr      r6, [pc, #0x3c]
007a9764: ldrb     r3, [r4, #0x1c]
007a9768: ldr      r1, [r4, #0x14]
007a976c: ldr      r2, [r4, #0x18]
007a9770: mov      r7, r0
007a9774: bl       #0x7a9698
007a9778: ldr      r3, [pc, #0x28]
007a977c: add      r6, pc, r6
007a9780: mov      r0, r5
007a9784: ldr      r3, [r6, r3]
007a9788: add      r3, r3, #8
007a978c: str      r3, [r7]
007a9790: str      r7, [r5, #0x10]
007a9794: ldr      r3, [r4]
007a9798: str      r3, [r5, #0x28]
007a979c: add      sp, sp, #0xc
007a97a0: pop      {r4, r5, r6, r7, pc}
007a97a4: andseq   fp, lr, r4, lsl r3
007a97a8: andeq    r2, r0, r4, ror r5

# _ZN8RenderFXC1Ev
007a850c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
007a8510: ldr      r3, [pc, #0xd8]
007a8514: ldr      r1, [pc, #0xd8]
007a8518: ldr      r2, [r0, #0x54]
007a851c: add      r3, pc, r3
007a8520: mov      r5, r0
007a8524: ldr      r1, [r3, r1]
007a8528: mvn      r0, #0
007a852c: bfi      r2, r0, #0, #0x18
007a8530: mov      r6, #0
007a8534: lsr      r0, r2, #0x18
007a8538: add      r1, r1, #8
007a853c: bfi      r0, r6, #0, #1
007a8540: mov      sl, #1
007a8544: str      r2, [r5, #0x54]
007a8548: mov      r7, #0
007a854c: str      r1, [r5]
007a8550: strb     r0, [r5, #0x57]
007a8554: str      r6, [r5, #4]
007a8558: str      r6, [r5, #8]
007a855c: str      r6, [r5, #0xc]
007a8560: strb     r6, [r5, #0x10]
007a8564: str      r6, [r5, #0x14]
007a8568: str      r6, [r5, #0x18]
007a856c: str      r6, [r5, #0x1c]
007a8570: str      r6, [r5, #0x20]
007a8574: strb     r6, [r5, #0x24]
007a8578: str      r6, [r5, #0x28]
007a857c: str      r6, [r5, #0x2c]
007a8580: str      r6, [r5, #0x30]
007a8584: strb     r6, [r5, #0x34]
007a8588: str      r6, [r5, #0x38]
007a858c: str      r6, [r5, #0x3c]
007a8590: str      r6, [r5, #0x40]
007a8594: strb     sl, [r5, #0x44]
007a8598: strb     r6, [r5, #0x45]
007a859c: add      r4, r5, #0x58
007a85a0: add      r8, r5, #0xf8
007a85a4: str      r7, [r4, #4]
007a85a8: str      r7, [r4]
007a85ac: str      r7, [r4, #8]
007a85b0: str      r6, [r4, #0xc]
007a85b4: str      r6, [r4, #0x10]
007a85b8: str      r6, [r4, #0x14]
007a85bc: str      r6, [r4, #0x18]
007a85c0: str      r6, [r4, #0x1c]
007a85c4: str      r6, [r4, #0x20]
007a85c8: strb     sl, [r4, #0x24]
007a85cc: mov      r0, r4
007a85d0: add      r4, r4, #0x28
007a85d4: bl       #0x7a84c4
007a85d8: cmp      r4, r8
007a85dc: bne      #0x7a85a4
007a85e0: str      r6, [r5, #0xfc]
007a85e4: str      r6, [r5, #0xf8]
007a85e8: mov      r0, r5
007a85ec: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
007a85f0: andseq   ip, lr, r4, ror r5
007a85f4: andeq    r4, r0, r0, ror #18

# _ZN7gameswf4root20set_background_alphaEf
007741f4: push     {r4, lr}
007741f8: mov      r4, r0
007741fc: mov      r0, r1
00774200: mov      r1, #0x43000000
00774204: add      r1, r1, #0x7f0000
00774208: bl       #0x30ed6c
0077420c: mov      r1, #0x3f000000
00774210: bl       #0x30eba4
00774214: bl       #0x30e4cc
00774218: cmp      r0, #0xfe
0077421c: movgt    r0, #0xff
00774220: bgt      #0x774238
00774224: cmp      r0, #0
00774228: movle    r0, #0
0077422c: bgt      #0x774238
00774230: strb     r0, [r4, #0x3b]
00774234: pop      {r4, pc}
00774238: uxtb     r0, r0
0077423c: strb     r0, [r4, #0x3b]
00774240: pop      {r4, pc}

# _ZN6MenuFXC1Ev
007a86e4: push     {r4, r5, r6, lr}
007a86e8: ldr      r5, [pc, #0x58]
007a86ec: mov      r4, r0
007a86f0: bl       #0x7a85f8
007a86f4: ldr      r2, [pc, #0x50]
007a86f8: add      r5, pc, r5
007a86fc: mov      r3, #0
007a8700: ldr      r2, [r5, r2]
007a8704: mov      r0, r4
007a8708: strb     r3, [r4, #0x120]
007a870c: add      r1, r2, #0x44
007a8710: add      r2, r2, #8
007a8714: str      r1, [r4, #0x100]
007a8718: str      r3, [r4, #0x104]
007a871c: str      r2, [r4]
007a8720: str      r3, [r4, #0x108]
007a8724: str      r3, [r4, #0x10c]
007a8728: strb     r3, [r4, #0x110]
007a872c: str      r3, [r4, #0x114]
007a8730: str      r3, [r4, #0x118]
007a8734: str      r3, [r4, #0x11c]
007a8738: add      r1, r4, #0x100
007a873c: bl       #0x7a7c6c
007a8740: mov      r0, r4
007a8744: pop      {r4, r5, r6, pc}
007a8748: mulseq   lr, r8, r3
007a874c: andeq    r2, r0, r8, lsl r7

# _ZN17MenuFlash2DCameraC1EP6MenuFX
0042ccd0: ldr      r2, [pc, #0x68]
0042ccd4: ldr      ip, [pc, #0x68]
0042ccd8: ldr      r3, [pc, #0x68]
0042ccdc: add      r2, pc, r2
0042cce0: push     {r4, r5}
0042cce4: ldr      ip, [r2, ip]
0042cce8: ldr      r4, [r2, r3]
0042ccec: str      r1, [r0, #4]
0042ccf0: add      r5, ip, #8
0042ccf4: mov      ip, #0
0042ccf8: str      r5, [r0]
0042ccfc: str      ip, [r0, #8]
0042cd00: ldr      r1, [r4]
0042cd04: ldr      r4, [pc, #0x40]
0042cd08: add      r1, r1, r1, lsr #31
0042cd0c: ldr      r2, [r2, r4]
0042cd10: asr      r1, r1, #1
0042cd14: str      r1, [r0, #0x1c]
0042cd18: ldr      r2, [r2]
0042cd1c: str      ip, [r0, #0x30]
0042cd20: str      ip, [r0, #0x24]
0042cd24: add      r2, r2, r2, lsr #31
0042cd28: str      ip, [r0, #0x28]
0042cd2c: asr      r2, r2, #1
0042cd30: str      r2, [r0, #0x20]
0042cd34: str      ip, [r0, #0x2c]
0042cd38: pop      {r4, r5}
0042cd3c: bx       lr
0042cd40: ldrheq   r7, [r6], #-0xd4
0042cd44: andeq    r0, r0, r4, ror #28
0042cd48: andeq    r2, r0, r4, asr #11
0042cd4c: strdeq   r2, r3, [r0], -r8

# _ZN8RenderFX4LoadEPKcPN7gameswf14player_contextE
007ab784: push     {r4, r5, r6, r7, r8, sl, lr}
007ab788: ldr      r6, [pc, #0x188]
007ab78c: ldr      r7, [pc, #0x188]
007ab790: sub      sp, sp, #0x34
007ab794: add      r6, pc, r6
007ab798: ldr      r3, [r6, r7]
007ab79c: subs     r8, r2, #0
007ab7a0: mov      r5, r0
007ab7a4: ldr      r3, [r3]
007ab7a8: mov      r4, r1
007ab7ac: str      r3, [sp, #0x2c]
007ab7b0: beq      #0x7ab8f4
007ab7b4: add      r0, r5, #0x44
007ab7b8: mov      r1, r4
007ab7bc: bl       #0x76c818
007ab7c0: mov      r1, #0
007ab7c4: mov      r0, #0xe0
007ab7c8: bl       #0x752ba8
007ab7cc: mov      r1, r8
007ab7d0: mov      sl, r0
007ab7d4: bl       #0x76f180
007ab7d8: mov      r1, sl
007ab7dc: add      r0, r5, #0x38
007ab7e0: bl       #0x7a87bc
007ab7e4: ldr      r3, [r5, #0x38]
007ab7e8: mov      r0, r4
007ab7ec: str      r5, [r3, #0x94]
007ab7f0: mov      r3, #1
007ab7f4: strb     r3, [sp, #0x18]
007ab7f8: mov      r3, #0
007ab7fc: strb     r3, [sp, #0x19]
007ab800: bl       #0x30de54
007ab804: adds     r3, r4, r0
007ab808: bhs      #0x7ab82c
007ab80c: ldrsb    r2, [r4, r0]
007ab810: cmp      r2, #0x2f
007ab814: beq      #0x7ab82c
007ab818: cmp      r2, #0x5c
007ab81c: beq      #0x7ab82c
007ab820: sub      r3, r3, #1
007ab824: cmp      r4, r3
007ab828: bls      #0x7ab8d4
007ab82c: rsb      r2, r4, #1
007ab830: add      r2, r3, r2
007ab834: cmp      r2, #0
007ab838: ble      #0x7ab870
007ab83c: add      r8, sp, #4
007ab840: mov      r1, r4
007ab844: mov      r0, r8
007ab848: bl       #0x751eb4
007ab84c: ldrsb    r3, [sp, #4]
007ab850: ldr      r0, [r5, #0x38]
007ab854: cmn      r3, #1
007ab858: addne    r1, r8, #1
007ab85c: ldreq    r1, [sp, #0x10]
007ab860: bl       #0x76c868
007ab864: ldrsb    r3, [sp, #4]
007ab868: cmn      r3, #1
007ab86c: beq      #0x7ab904
007ab870: ldrsb    r3, [sp, #0x18]
007ab874: cmn      r3, #1
007ab878: beq      #0x7ab8e4
007ab87c: mov      r2, r4
007ab880: mov      r0, sp
007ab884: ldr      r1, [r5, #0x38]
007ab888: bl       #0x7736f4
007ab88c: add      r0, r5, #0x3c
007ab890: ldr      r1, [sp]
007ab894: bl       #0x75a258
007ab898: ldr      r0, [sp]
007ab89c: cmp      r0, #0
007ab8a0: beq      #0x7ab8a8
007ab8a4: bl       #0x75a240
007ab8a8: ldr      r3, [r5, #0x3c]
007ab8ac: mov      r0, r5
007ab8b0: ldr      r1, [r3, #0x10]
007ab8b4: bl       #0x7a7ee8
007ab8b8: ldr      r3, [r6, r7]
007ab8bc: ldr      r2, [sp, #0x2c]
007ab8c0: ldr      r3, [r3]
007ab8c4: cmp      r2, r3
007ab8c8: bne      #0x7ab914
007ab8cc: add      sp, sp, #0x34
007ab8d0: pop      {r4, r5, r6, r7, r8, sl, pc}
007ab8d4: ldrsb    r2, [r3]
007ab8d8: cmp      r2, #0x2f
007ab8dc: bne      #0x7ab818
007ab8e0: b        #0x7ab82c
007ab8e4: ldr      r0, [sp, #0x24]
007ab8e8: ldr      r1, [sp, #0x20]
007ab8ec: bl       #0x752b38
007ab8f0: b        #0x7ab87c
007ab8f4: ldr      r3, [pc, #0x24]
007ab8f8: ldr      r3, [r6, r3]
007ab8fc: ldr      r8, [r3]
007ab900: b        #0x7ab7b4
007ab904: ldr      r0, [sp, #0x10]
007ab908: ldr      r1, [sp, #0xc]
007ab90c: bl       #0x752b38
007ab910: b        #0x7ab870
007ab914: bl       #0x30e310
007ab918: ldrsheq  sb, [lr], -ip
007ab91c: andeq    r4, r0, ip, lsr #1
007ab920: andeq    r4, r0, r0, lsr sb

# _ZN8RenderFX9PreRenderEv
007a9c1c: push     {r4, r5, r6, lr}
007a9c20: ldr      r0, [r0, #0x38]
007a9c24: bl       #0x76d5b4
007a9c28: ldr      r5, [pc, #0x48]
007a9c2c: subs     r4, r0, #0
007a9c30: add      r5, pc, r5
007a9c34: beq      #0x7a9c3c
007a9c38: bl       #0x759c64
007a9c3c: ldr      r3, [pc, #0x38]
007a9c40: ldr      r3, [r5, r3]
007a9c44: ldr      r0, [r3]
007a9c48: cmp      r0, #0
007a9c4c: beq      #0x7a9c6c
007a9c50: mov      r1, r4
007a9c54: bl       #0x75918c
007a9c58: cmp      r4, #0
007a9c5c: beq      #0x7a9c74
007a9c60: mov      r0, r4
007a9c64: pop      {r4, r5, r6, lr}
007a9c68: b        #0x75a240
007a9c6c: cmp      r4, #0
007a9c70: bne      #0x7a9c60
007a9c74: pop      {r4, r5, r6, pc}
007a9c78: andseq   sl, lr, r0, ror #28
007a9c7c: strheq   r3, [r0], -r8

# _ZN8RenderFX10InitializeEPN6glitch5video12IVideoDriverE
007a98f8: push     {r4, lr}
007a98fc: mov      r4, r0
007a9900: ldr      r0, [pc, #0x48]
007a9904: sub      sp, sp, #0x28
007a9908: add      r0, pc, r0
007a990c: bl       #0x7611f0
007a9910: add      r0, sp, #0x28
007a9914: mov      r2, #1
007a9918: mov      r3, #0
007a991c: strb     r2, [sp, #0x20]
007a9920: str      r4, [r0, #-0x24]!
007a9924: mov      r2, #0x3f800000
007a9928: str      r3, [sp, #0x1c]
007a992c: str      r2, [sp, #0x24]
007a9930: str      r3, [sp, #8]
007a9934: str      r3, [sp, #0xc]
007a9938: str      r3, [sp, #0x10]
007a993c: str      r3, [sp, #0x14]
007a9940: str      r3, [sp, #0x18]
007a9944: bl       #0x7a9814
007a9948: add      sp, sp, #0x28
007a994c: pop      {r4, pc}
007a9950: andseq   r0, r6, r0, lsr pc

# _ZN8RenderFX14SetOrientationEN7gameswf16orientation_modeE
007a7eb4: ldr      r3, [pc, #0x24]
007a7eb8: ldr      r2, [pc, #0x24]
007a7ebc: push     {r4, lr}
007a7ec0: add      r3, pc, r3
007a7ec4: ldr      r2, [r3, r2]
007a7ec8: ldr      r3, [r2]
007a7ecc: mov      r0, r3
007a7ed0: ldr      r3, [r3]
007a7ed4: mov      lr, pc
007a7ed8: ldr      pc, [r3, #0xa8]
007a7edc: pop      {r4, pc}
007a7ee0: ldrsbeq  ip, [lr], -r0
007a7ee4: strheq   r3, [r0], -r4

# _ZN7gameswf4root20set_background_colorERKNS_4rgbaE
007741e0: push     {r4, lr}
007741e4: mov      r2, #4
007741e8: add      r0, r0, #0x38
007741ec: bl       #0x30e868
007741f0: pop      {r4, pc}
