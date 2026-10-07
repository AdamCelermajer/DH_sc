
# _ZN13ObjectManager11LoadFromXMLEP12TiXmlElementPKcRK7Point3DIfEi
0034b868: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b86c: ldr      r4, [pc, #0x338]
0034b870: ldr      r5, [pc, #0x338]
0034b874: subs     r7, r1, #0
0034b878: add      r4, pc, r4
0034b87c: ldr      r1, [r4, r5]
0034b880: mov      sl, r2
0034b884: sub      sp, sp, #0x25c
0034b888: ldr      r2, [r1]
0034b88c: mov      fp, r0
0034b890: str      r3, [sp, #0xc]
0034b894: str      r2, [sp, #0x254]
0034b898: beq      #0x34bb30
0034b89c: ldr      r1, [pc, #0x310]
0034b8a0: mov      r0, r7
0034b8a4: add      r1, pc, r1
0034b8a8: bl       #0x514c70
0034b8ac: ldr      r1, [pc, #0x304]
0034b8b0: mov      r8, r0
0034b8b4: mov      r0, r7
0034b8b8: add      r1, pc, r1
0034b8bc: bl       #0x514c70
0034b8c0: subs     sb, r0, #0
0034b8c4: beq      #0x34badc
0034b8c8: cmp      r8, #0
0034b8cc: beq      #0x34badc
0034b8d0: add      r6, sp, #0x2c
0034b8d4: mov      r0, r6
0034b8d8: bl       #0x33f50c
0034b8dc: cmp      sl, #0
0034b8e0: beq      #0x34b8f8
0034b8e4: mov      r0, sl
0034b8e8: mov      r1, r8
0034b8ec: bl       #0x30e31c
0034b8f0: cmp      r0, #0
0034b8f4: bne      #0x34badc
0034b8f8: ldr      r1, [pc, #0x2bc]
0034b8fc: mov      r0, sb
0034b900: add      r1, pc, r1
0034b904: bl       #0x30e31c
0034b908: cmp      r0, #0
0034b90c: beq      #0x34baf8
0034b910: add      sl, sp, #0x13c
0034b914: mov      r1, sb
0034b918: mov      r0, sl
0034b91c: bl       #0x30eae4
0034b920: ldr      r1, [pc, #0x298]
0034b924: mov      r2, sl
0034b928: add      r0, sp, #0x3c
0034b92c: add      r1, pc, r1
0034b930: mov      r3, #0x53
0034b934: bl       #0x30eae4
0034b938: ldr      ip, [sp, #0x280]
0034b93c: add      sl, sp, #0x10
0034b940: mov      r3, sb
0034b944: mov      r1, fp
0034b948: mov      r0, sl
0034b94c: mov      r2, r8
0034b950: mov      sb, #1
0034b954: str      ip, [sp]
0034b958: str      sb, [sp, #4]
0034b95c: bl       #0x34b520
0034b960: ldr      r1, [sp, #0x14]
0034b964: add      r3, r6, #4
0034b968: ldr      r2, [sp, #0x10]
0034b96c: str      r1, [r3], #4
0034b970: ldr      ip, [sl, #8]
0034b974: add      r6, sp, #0x2c
0034b978: mov      r0, r6
0034b97c: mov      r1, #0
0034b980: str      ip, [r3]
0034b984: str      r2, [sp, #0x2c]
0034b988: bl       #0x33fdc0
0034b98c: cmp      r0, #0
0034b990: beq      #0x34badc
0034b994: mov      r1, sb
0034b998: mov      r0, r6
0034b99c: bl       #0x33fdc0
0034b9a0: add      r0, r0, #4
0034b9a4: bl       #0x513d78
0034b9a8: ldr      r1, [pc, #0x214]
0034b9ac: mov      r0, r7
0034b9b0: add      r1, pc, r1
0034b9b4: bl       #0x514c70
0034b9b8: subs     fp, r0, #0
0034b9bc: beq      #0x34ba18
0034b9c0: mov      r1, sb
0034b9c4: mov      r0, r6
0034b9c8: bl       #0x33fdc0
0034b9cc: add      sb, sp, #0x23c
0034b9d0: add      sl, r0, #4
0034b9d4: mov      r1, fp
0034b9d8: add      r2, sp, #0x38
0034b9dc: mov      r0, sb
0034b9e0: bl       #0x3140ec
0034b9e4: mov      r0, sl
0034b9e8: mov      r1, sb
0034b9ec: bl       #0x513fec
0034b9f0: ldr      r0, [sp, #0x250]
0034b9f4: cmp      r0, sb
0034b9f8: beq      #0x34ba18
0034b9fc: cmp      r0, #0
0034ba00: beq      #0x34ba18
0034ba04: ldr      r1, [sp, #0x23c]
0034ba08: rsb      r1, r0, r1
0034ba0c: cmp      r1, #0x80
0034ba10: bhi      #0x34bba0
0034ba14: bl       #0x708f00
0034ba18: mov      r1, #1
0034ba1c: mov      r0, r6
0034ba20: bl       #0x33fdc0
0034ba24: add      r0, r0, #4
0034ba28: bl       #0x5136ec
0034ba2c: mov      r1, #1
0034ba30: mov      r0, r6
0034ba34: bl       #0x33fdc0
0034ba38: mov      r1, r7
0034ba3c: add      r0, r0, #4
0034ba40: bl       #0x513a00
0034ba44: ldr      r1, [pc, #0x17c]
0034ba48: mov      r0, r8
0034ba4c: add      r1, pc, r1
0034ba50: bl       #0x30e31c
0034ba54: cmp      r0, #0
0034ba58: beq      #0x34bb84
0034ba5c: mov      r1, #1
0034ba60: mov      r0, r6
0034ba64: bl       #0x33fdc0
0034ba68: ldr      r3, [r0]
0034ba6c: mov      lr, pc
0034ba70: ldr      pc, [r3, #0x20]
0034ba74: cmp      r0, #0
0034ba78: beq      #0x34badc
0034ba7c: mov      r0, r6
0034ba80: bl       #0x33fee4
0034ba84: ldr      r3, [sp, #0xc]
0034ba88: mov      r8, r0
0034ba8c: ldr      r0, [r0, #0x164]
0034ba90: ldr      r1, [r3, #4]
0034ba94: bl       #0x30eba4
0034ba98: ldr      ip, [sp, #0xc]
0034ba9c: mov      r7, r0
0034baa0: ldr      r0, [r8, #0x168]
0034baa4: ldr      r1, [ip, #8]
0034baa8: bl       #0x30eba4
0034baac: ldr      r3, [sp, #0xc]
0034bab0: mov      r6, r0
0034bab4: ldr      r0, [r8, #0x160]
0034bab8: ldr      r1, [r3]
0034babc: bl       #0x30eba4
0034bac0: add      r1, sp, #0x20
0034bac4: str      r0, [sp, #0x20]
0034bac8: mov      r2, #1
0034bacc: mov      r0, r8
0034bad0: str      r7, [sp, #0x24]
0034bad4: str      r6, [sp, #0x28]
0034bad8: bl       #0x393db4
0034badc: ldr      r3, [r4, r5]
0034bae0: ldr      r2, [sp, #0x254]
0034bae4: ldr      r3, [r3]
0034bae8: cmp      r2, r3
0034baec: bne      #0x34bba8
0034baf0: add      sp, sp, #0x25c
0034baf4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034baf8: add      sl, sp, #0x13c
0034bafc: mov      r1, sb
0034bb00: mov      r0, sl
0034bb04: bl       #0x30eae4
0034bb08: ldr      r1, [pc, #0xbc]
0034bb0c: add      sb, sp, #0x3c
0034bb10: mov      r2, sl
0034bb14: add      r1, pc, r1
0034bb18: mov      r0, sb
0034bb1c: mov      r3, #0x53
0034bb20: bl       #0x30eae4
0034bb24: mvn      ip, #0
0034bb28: str      ip, [sp, #0x280]
0034bb2c: b        #0x34b938
0034bb30: ldr      r3, [pc, #0x98]
0034bb34: ldr      r3, [r4, r3]
0034bb38: ldr      r3, [r3]
0034bb3c: cmp      r3, #2
0034bb40: streq    r7, [r7]
0034bb44: beq      #0x34badc
0034bb48: cmp      r3, #1
0034bb4c: bne      #0x34badc
0034bb50: ldr      r0, [pc, #0x7c]
0034bb54: ldr      r1, [pc, #0x7c]
0034bb58: ldr      r2, [pc, #0x7c]
0034bb5c: ldr      r0, [r4, r0]
0034bb60: ldr      r3, [pc, #0x78]
0034bb64: mov      ip, #0x20c
0034bb68: add      r1, pc, r1
0034bb6c: add      r2, pc, r2
0034bb70: add      r3, pc, r3
0034bb74: add      r0, r0, #0xa8
0034bb78: str      ip, [sp]
0034bb7c: bl       #0x30e004
0034bb80: b        #0x34badc
0034bb84: mov      r0, r6
0034bb88: mov      r1, #1
0034bb8c: bl       #0x33fdc0
0034bb90: ldr      r3, [r0]
0034bb94: mov      lr, pc
0034bb98: ldr      pc, [r3, #0x1c]
0034bb9c: b        #0x34ba5c
0034bba0: bl       #0x310440
0034bba4: b        #0x34ba18
0034bba8: bl       #0x30e310
0034bbac: rsbeq    sb, r4, r8, lsl r2
0034bbb0: andeq    r4, r0, ip, lsr #1
0034bbb4: ldrsbeq  r4, [r7], #-0x84
0034bbb8: subseq   r5, sb, r0, lsr r8
0034bbbc: subseq   r4, r7, r0, asr #22
0034bbc0: subseq   r4, r7, ip, lsr #22
0034bbc4: ldrheq   r4, [r7], #-0xa0
0034bbc8: subseq   r4, r7, r4, lsr #20
0034bbcc: subseq   r4, r7, r4, asr #18
0034bbd0: andeq    r3, r0, r0, asr #19
0034bbd4: andeq    r1, r0, r0, asr #19
0034bbd8: subseq   r2, r7, r0, ror r8
0034bbdc: subseq   r4, r7, ip, asr #17
0034bbe0: subseq   r4, r7, r8, lsr #14

# _ZN11PropertyMap14InitPropertiesEv
00513d78: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00513d7c: ldr      fp, [pc, #0x250]
00513d80: ldr      r2, [pc, #0x250]
00513d84: ldr      r3, [pc, #0x250]
00513d88: sub      sp, sp, #0x84
00513d8c: add      fp, pc, fp
00513d90: str      r3, [sp, #4]
00513d94: ldr      r3, [fp, r2]
00513d98: str      r2, [sp, #8]
00513d9c: str      r0, [sp, #0xc]
00513da0: ldr      r3, [r3]
00513da4: str      r3, [sp, #0x7c]
00513da8: bl       #0x510b4c
00513dac: ldr      r2, [sp, #4]
00513db0: mov      sl, r0
00513db4: ldr      r8, [fp, r2]
00513db8: ldr      r4, [r8, #4]
00513dbc: cmp      r4, #0
00513dc0: beq      #0x513ee0
00513dc4: add      r7, sp, #0x4c
00513dc8: add      sb, sp, #0x18
00513dcc: b        #0x513dd8
00513dd0: mov      r8, r4
00513dd4: mov      r4, r3
00513dd8: mov      r1, sl
00513ddc: mov      r2, sb
00513de0: mov      r0, r7
00513de4: bl       #0x3140ec
00513de8: ldr      r3, [r4, #0x24]
00513dec: ldr      r1, [sp, #0x60]
00513df0: ldr      r6, [r4, #0x20]
00513df4: ldr      r5, [sp, #0x5c]
00513df8: mov      r0, r3
00513dfc: rsb      r6, r3, r6
00513e00: rsb      r5, r1, r5
00513e04: cmp      r5, r6
00513e08: movlt    r2, r5
00513e0c: movge    r2, r6
00513e10: bl       #0x30e5e0
00513e14: subs     r3, r0, #0
00513e18: bne      #0x513e30
00513e1c: cmp      r6, r5
00513e20: mvnlt    r3, #0
00513e24: blt      #0x513e30
00513e28: movle    r3, #0
00513e2c: movgt    r3, #1
00513e30: mov      r0, r7
00513e34: str      r3, [sp]
00513e38: bl       #0x318254
00513e3c: ldr      r3, [sp]
00513e40: cmp      r3, #0
00513e44: ldrlt    r3, [r4, #0xc]
00513e48: ldrge    r3, [r4, #8]
00513e4c: movlt    r4, r8
00513e50: cmp      r3, #0
00513e54: bne      #0x513dd0
00513e58: ldr      r2, [sp, #4]
00513e5c: mov      r8, r4
00513e60: ldr      r3, [fp, r2]
00513e64: cmp      r4, r3
00513e68: beq      #0x513ee0
00513e6c: add      r5, sp, #0x34
00513e70: mov      r1, sl
00513e74: add      r2, sp, #0x14
00513e78: mov      r0, r5
00513e7c: bl       #0x3140ec
00513e80: ldr      r3, [sp, #0x48]
00513e84: ldr      r1, [r4, #0x24]
00513e88: ldr      r6, [r4, #0x20]
00513e8c: ldr      r7, [sp, #0x44]
00513e90: mov      r0, r3
00513e94: rsb      r6, r1, r6
00513e98: rsb      r7, r3, r7
00513e9c: cmp      r6, r7
00513ea0: movlt    r2, r6
00513ea4: movge    r2, r7
00513ea8: bl       #0x30e5e0
00513eac: subs     r8, r0, #0
00513eb0: bne      #0x513ec8
00513eb4: cmp      r7, r6
00513eb8: mvnlt    r8, #0
00513ebc: blt      #0x513ec8
00513ec0: movle    r8, #0
00513ec4: movgt    r8, #1
00513ec8: mov      r0, r5
00513ecc: bl       #0x318254
00513ed0: cmp      r8, #0
00513ed4: ldrlt    r3, [sp, #4]
00513ed8: movge    r8, r4
00513edc: ldrlt    r8, [fp, r3]
00513ee0: ldr      r2, [sp, #4]
00513ee4: ldr      r3, [fp, r2]
00513ee8: cmp      r8, r3
00513eec: beq      #0x513f10
00513ef0: ldr      r2, [sp, #8]
00513ef4: ldr      r3, [fp, r2]
00513ef8: ldr      r2, [sp, #0x7c]
00513efc: ldr      r3, [r3]
00513f00: cmp      r2, r3
00513f04: bne      #0x513fd0
00513f08: add      sp, sp, #0x84
00513f0c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00513f10: add      r4, sp, #0x64
00513f14: mov      r0, r4
00513f18: mov      r1, #0x10
00513f1c: str      r4, [sp, #0x74]
00513f20: str      r4, [sp, #0x78]
00513f24: bl       #0x31167c
00513f28: ldr      r3, [sp, #0x74]
00513f2c: add      r7, sp, #0x1c
00513f30: mov      r5, #0
00513f34: strb     r5, [r3]
00513f38: mov      r1, r4
00513f3c: mov      r0, r7
00513f40: bl       #0x32b918
00513f44: mov      r1, r5
00513f48: mov      r0, #0x38
00513f4c: bl       #0x310570
00513f50: ldr      r3, [pc, #0x88]
00513f54: ldr      r6, [pc, #0x88]
00513f58: mov      r5, r0
00513f5c: ldr      r3, [fp, r3]
00513f60: add      r6, pc, r6
00513f64: mov      r1, r6
00513f68: add      r3, r3, #8
00513f6c: add      r2, sp, #0x10
00513f70: str      r3, [r0], #8
00513f74: bl       #0x3140ec
00513f78: ldr      r3, [pc, #0x68]
00513f7c: mov      r0, r5
00513f80: mov      r2, #4
00513f84: ldr      r3, [fp, r3]
00513f88: str      r2, [r5, #4]
00513f8c: mov      r1, r7
00513f90: add      r3, r3, #8
00513f94: str      r3, [r0], #0x20
00513f98: bl       #0x32b918
00513f9c: mov      r1, r6
00513fa0: mov      r2, r5
00513fa4: ldr      r0, [sp, #0xc]
00513fa8: bl       #0x513ce4
00513fac: mov      r0, r7
00513fb0: bl       #0x318254
00513fb4: mov      r0, r4
00513fb8: bl       #0x318254
00513fbc: ldr      r0, [sp, #0xc]
00513fc0: ldr      r3, [r0]
00513fc4: mov      lr, pc
00513fc8: ldr      pc, [r3]
00513fcc: b        #0x513ef0
00513fd0: bl       #0x30e310
00513fd4: subeq    r0, r8, r4, lsl #26
00513fd8: andeq    r4, r0, ip, lsr #1
00513fdc: andeq    r2, r0, r8, ror #20
00513fe0: andeq    r2, r0, r0, lsr r3
00513fe4: eorseq   ip, sl, r0, lsl #10
00513fe8: muleq    r0, r4, r4

# _ZN11PropertyMap11SetTemplateERKSs
00513fec: push     {r4, r5, r6, r7, r8, sl, lr}
00513ff0: ldr      r3, [r1, #0x14]
00513ff4: ldr      r2, [r1, #0x10]
00513ff8: ldr      r7, [pc, #0x164]
00513ffc: sub      sp, sp, #0x14
00514000: cmp      r3, r2
00514004: mov      r4, r0
00514008: add      r7, pc, r7
0051400c: beq      #0x514040
00514010: add      r0, r0, #4
00514014: cmp      r1, r0
00514018: beq      #0x514024
0051401c: mov      r1, r3
00514020: bl       #0x3109e0
00514024: mov      r0, r4
00514028: bl       #0x5134c0
0051402c: ldr      r8, [r0, #0x10]
00514030: cmp      r8, #0
00514034: beq      #0x514048
00514038: mov      r0, r4
0051403c: bl       #0x51419c
00514040: add      sp, sp, #0x14
00514044: pop      {r4, r5, r6, r7, r8, sl, pc}
00514048: mov      r0, r4
0051404c: bl       #0x510b4c
00514050: add      r3, sp, #0x10
00514054: str      r0, [r3, #-4]!
00514058: mov      r0, r3
0051405c: bl       #0x5124d4
00514060: bl       #0x513aa8
00514064: mov      r5, r0
00514068: mov      r0, r4
0051406c: bl       #0x5134c0
00514070: ldr      r3, [r0, #0x10]
00514074: mov      r6, r0
00514078: cmp      r3, #0
0051407c: bne      #0x514110
00514080: ldr      r7, [r5, #8]
00514084: cmp      r5, r7
00514088: beq      #0x514038
0051408c: add      r1, r7, #0x10
00514090: mov      r0, r6
00514094: ldr      r8, [r7, #0x28]
00514098: bl       #0x512b74
0051409c: ldr      r3, [r8]
005140a0: mov      sl, r0
005140a4: mov      r0, r8
005140a8: mov      lr, pc
005140ac: ldr      pc, [r3, #0x14]
005140b0: str      r0, [sl]
005140b4: ldr      r2, [r7, #0xc]
005140b8: cmp      r2, #0
005140bc: bne      #0x5140c8
005140c0: b        #0x5140dc
005140c4: mov      r2, r3
005140c8: ldr      r3, [r2, #8]
005140cc: cmp      r3, #0
005140d0: bne      #0x5140c4
005140d4: mov      r7, r2
005140d8: b        #0x514084
005140dc: ldr      r3, [r7, #4]
005140e0: ldr      r1, [r3, #0xc]
005140e4: cmp      r7, r1
005140e8: bne      #0x514104
005140ec: mov      r7, r3
005140f0: ldr      r3, [r3, #4]
005140f4: ldr      r2, [r3, #0xc]
005140f8: cmp      r2, r7
005140fc: beq      #0x5140ec
00514100: ldr      r2, [r7, #0xc]
00514104: cmp      r2, r3
00514108: movne    r7, r3
0051410c: b        #0x514084
00514110: ldr      r3, [pc, #0x50]
00514114: ldr      r3, [r7, r3]
00514118: ldr      r3, [r3]
0051411c: cmp      r3, #2
00514120: streq    r8, [r8]
00514124: beq      #0x514080
00514128: cmp      r3, #1
0051412c: bne      #0x514080
00514130: ldr      r0, [pc, #0x34]
00514134: ldr      r1, [pc, #0x34]
00514138: ldr      r2, [pc, #0x34]
0051413c: ldr      r0, [r7, r0]
00514140: ldr      r3, [pc, #0x30]
00514144: mov      ip, #0x70
00514148: add      r1, pc, r1
0051414c: add      r2, pc, r2
00514150: add      r3, pc, r3
00514154: add      r0, r0, #0xa8
00514158: str      ip, [sp]
0051415c: bl       #0x30e004
00514160: b        #0x514080
00514164: subeq    r0, r8, r8, lsl #21
00514168: andeq    r3, r0, r0, asr #19
0051416c: andeq    r1, r0, r0, asr #19
00514170: mlaseq   sl, r0, r2, sl
00514174: ldrshteq r7, [ip], -ip
00514178: eorseq   r7, ip, r8, lsr #29

# _ZN11PropertyMap12LoadTemplateEv
0051419c: ldr      r3, [pc, #0x68]
005141a0: ldr      r2, [pc, #0x68]
005141a4: str      lr, [sp, #-4]!
005141a8: add      r3, pc, r3
005141ac: ldr      r2, [r3, r2]
005141b0: sub      sp, sp, #0xc
005141b4: ldr      r2, [r2]
005141b8: cmp      r2, #2
005141bc: moveq    r3, #0
005141c0: streq    r3, [r3]
005141c4: beq      #0x5141d0
005141c8: cmp      r2, #1
005141cc: beq      #0x5141d8
005141d0: add      sp, sp, #0xc
005141d4: ldm      sp!, {pc}
005141d8: ldr      r0, [pc, #0x34]
005141dc: ldr      r1, [pc, #0x34]
005141e0: ldr      r2, [pc, #0x34]
005141e4: ldr      r0, [r3, r0]
005141e8: ldr      r3, [pc, #0x30]
005141ec: mov      ip, #9
005141f0: add      r1, pc, r1
005141f4: add      r2, pc, r2
005141f8: add      r3, pc, r3
005141fc: add      r0, r0, #0xa8
00514200: str      ip, [sp]
00514204: bl       #0x30e004
00514208: b        #0x5141d0
0051420c: subeq    r0, r8, r8, ror #17
00514210: andeq    r3, r0, r0, asr #19
00514214: andeq    r1, r0, r0, asr #19
00514218: eorseq   sl, sl, r8, ror #3
0051421c: eorseq   sl, sl, r4, ror r3
00514220: eorseq   r7, ip, r8, ror #28

# _ZN11PropertyMap21LoadDefaultPropertiesEv
005136ec: push     {r4, r5, r6, r7, r8, sb, sl, lr}
005136f0: ldr      r4, [pc, #0x108]
005136f4: ldr      r6, [pc, #0x108]
005136f8: sub      sp, sp, #0x20
005136fc: add      r4, pc, r4
00513700: ldr      r3, [r4, r6]
00513704: add      sb, r0, #4
00513708: mov      r8, r0
0051370c: ldr      r3, [r3]
00513710: add      r5, sp, #4
00513714: str      r3, [sp, #0x1c]
00513718: bl       #0x5134c0
0051371c: mov      r1, sb
00513720: mov      sl, r0
00513724: mov      r0, r5
00513728: bl       #0x32b918
0051372c: ldr      r7, [sl, #8]
00513730: cmp      sl, r7
00513734: beq      #0x51378c
00513738: ldr      r3, [r7, #0x28]
0051373c: cmp      r3, #0
00513740: beq      #0x513760
00513744: cmp      r8, #0
00513748: beq      #0x513760
0051374c: mov      r0, r3
00513750: mov      r1, r8
00513754: ldr      r3, [r3]
00513758: mov      lr, pc
0051375c: ldr      pc, [r3, #0xc]
00513760: ldr      r2, [r7, #0xc]
00513764: cmp      r2, #0
00513768: bne      #0x513774
0051376c: b        #0x5137c8
00513770: mov      r2, r3
00513774: ldr      r3, [r2, #8]
00513778: cmp      r3, #0
0051377c: bne      #0x513770
00513780: mov      r7, r2
00513784: cmp      sl, r7
00513788: bne      #0x513738
0051378c: cmp      sb, r5
00513790: beq      #0x5137a4
00513794: mov      r0, sb
00513798: ldr      r1, [sp, #0x18]
0051379c: ldr      r2, [sp, #0x14]
005137a0: bl       #0x3109e0
005137a4: mov      r0, r5
005137a8: bl       #0x318254
005137ac: ldr      r3, [r4, r6]
005137b0: ldr      r2, [sp, #0x1c]
005137b4: ldr      r3, [r3]
005137b8: cmp      r2, r3
005137bc: bne      #0x5137fc
005137c0: add      sp, sp, #0x20
005137c4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
005137c8: ldr      r3, [r7, #4]
005137cc: ldr      r1, [r3, #0xc]
005137d0: cmp      r7, r1
005137d4: bne      #0x5137f0
005137d8: mov      r7, r3
005137dc: ldr      r3, [r3, #4]
005137e0: ldr      r2, [r3, #0xc]
005137e4: cmp      r2, r7
005137e8: beq      #0x5137d8
005137ec: ldr      r2, [r7, #0xc]
005137f0: cmp      r2, r3
005137f4: movne    r7, r3
005137f8: b        #0x513730
005137fc: bl       #0x30e310
00513800: umaaleq  r1, r8, r4, r3
00513804: andeq    r4, r0, ip, lsr #1

# _ZN11PropertyMap20LoadOverridesFromXMLEP12TiXmlElement
00513a00: push     {r4, r5, r6, r7, r8, lr}
00513a04: subs     r7, r1, #0
00513a08: mov      r6, r0
00513a0c: beq      #0x513a70
00513a10: bl       #0x5134c0
00513a14: ldr      r4, [r0, #8]
00513a18: mov      r5, r0
00513a1c: cmp      r5, r4
00513a20: beq      #0x513a70
00513a24: ldr      r8, [r4, #0x24]
00513a28: mov      r0, r7
00513a2c: mov      r1, r8
00513a30: bl       #0x514c70
00513a34: mov      r1, r8
00513a38: mov      r2, r0
00513a3c: mov      r0, r6
00513a40: bl       #0x51387c
00513a44: ldr      r2, [r4, #0xc]
00513a48: cmp      r2, #0
00513a4c: bne      #0x513a58
00513a50: b        #0x513a74
00513a54: mov      r2, r3
00513a58: ldr      r3, [r2, #8]
00513a5c: cmp      r3, #0
00513a60: bne      #0x513a54
00513a64: mov      r4, r2
00513a68: cmp      r5, r4
00513a6c: bne      #0x513a24
00513a70: pop      {r4, r5, r6, r7, r8, pc}
00513a74: ldr      r3, [r4, #4]
00513a78: ldr      r1, [r3, #0xc]
00513a7c: cmp      r4, r1
00513a80: bne      #0x513a9c
00513a84: mov      r4, r3
00513a88: ldr      r3, [r3, #4]
00513a8c: ldr      r2, [r3, #0xc]
00513a90: cmp      r2, r4
00513a94: beq      #0x513a84
00513a98: ldr      r2, [r4, #0xc]
00513a9c: cmp      r2, r3
00513aa0: movne    r4, r3
00513aa4: b        #0x513a1c

# _ZN11PropertyMap11SetPropertyEPKcS1_
0051387c: push     {r4, lr}
00513880: mov      r3, r1
00513884: sub      sp, sp, #8
00513888: mov      r1, r0
0051388c: mov      r4, r2
00513890: mov      r0, sp
00513894: mov      r2, r3
00513898: bl       #0x513858
0051389c: cmp      r4, #0
005138a0: ldr      r3, [sp]
005138a4: ldr      r1, [sp, #4]
005138a8: beq      #0x5138d8
005138ac: cmp      r1, #0
005138b0: beq      #0x5138d0
005138b4: cmp      r3, #0
005138b8: beq      #0x5138d0
005138bc: mov      r0, r3
005138c0: mov      r2, r4
005138c4: ldr      r3, [r3]
005138c8: mov      lr, pc
005138cc: ldr      pc, [r3, #4]
005138d0: add      sp, sp, #8
005138d4: pop      {r4, pc}
005138d8: cmp      r1, #0
005138dc: beq      #0x5138d0
005138e0: cmp      r3, #0
005138e4: beq      #0x5138d0
005138e8: mov      r0, r3
005138ec: ldr      r3, [r3]
005138f0: mov      lr, pc
005138f4: ldr      pc, [r3, #0xc]
005138f8: b        #0x5138d0

# _ZN9Character17DeclarePropertiesEv
003a9fe4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003a9fe8: sub      sp, sp, #8
003a9fec: mov      r7, r0
003a9ff0: bl       #0x38cee8
003a9ff4: ldr      r1, [pc, #0x180]
003a9ff8: add      r4, r7, #4
003a9ffc: add      r5, r7, #0x1380
003aa000: mov      r0, r4
003aa004: add      r2, r5, #0x30
003aa008: add      r1, pc, r1
003aa00c: bl       #0x33ef7c
003aa010: ldr      r1, [pc, #0x168]
003aa014: add      r2, r5, #0x18
003aa018: mov      r0, r4
003aa01c: add      r1, pc, r1
003aa020: bl       #0x33ef7c
003aa024: ldr      r1, [pc, #0x158]
003aa028: add      r5, r7, #0x13c0
003aa02c: mov      r0, r4
003aa030: add      r2, r5, #0xc
003aa034: add      r1, pc, r1
003aa038: bl       #0x33ef7c
003aa03c: ldr      r1, [pc, #0x144]
003aa040: mov      r0, r4
003aa044: add      r2, r5, #0x24
003aa048: add      r1, pc, r1
003aa04c: bl       #0x3a92b4
003aa050: ldr      r1, [pc, #0x134]
003aa054: add      r2, r5, #0x28
003aa058: mov      r0, r4
003aa05c: add      r1, pc, r1
003aa060: bl       #0x33ef7c
003aa064: ldr      r1, [pc, #0x124]
003aa068: add      r7, r7, #0x1400
003aa06c: mov      r0, r4
003aa070: mov      r2, r7
003aa074: add      r1, pc, r1
003aa078: bl       #0x33ef7c
003aa07c: ldr      r1, [pc, #0x110]
003aa080: mov      r0, r4
003aa084: add      r2, r7, #0x18
003aa088: add      r1, pc, r1
003aa08c: bl       #0x33ef7c
003aa090: ldr      r1, [pc, #0x100]
003aa094: add      r2, r7, #0x30
003aa098: mov      r0, r4
003aa09c: add      r1, pc, r1
003aa0a0: bl       #0x3a92b4
003aa0a4: mov      r1, #0
003aa0a8: mov      r0, #0x28
003aa0ac: bl       #0x310570
003aa0b0: ldr      r5, [pc, #0xe4]
003aa0b4: ldr      sb, [pc, #0xe4]
003aa0b8: ldr      r8, [pc, #0xe4]
003aa0bc: add      r5, pc, r5
003aa0c0: ldr      sb, [r5, sb]
003aa0c4: add      r8, pc, r8
003aa0c8: mov      r6, r0
003aa0cc: add      sb, sb, #8
003aa0d0: mov      r1, r8
003aa0d4: add      r2, sp, #4
003aa0d8: str      sb, [r0], #8
003aa0dc: bl       #0x3140ec
003aa0e0: ldr      r3, [pc, #0xc0]
003aa0e4: add      r2, r7, #0x34
003aa0e8: mov      sl, #0
003aa0ec: ldr      r3, [r5, r3]
003aa0f0: rsb      r2, r4, r2
003aa0f4: str      r2, [r6, #4]
003aa0f8: add      r3, r3, #8
003aa0fc: str      r3, [r6]
003aa100: mov      r2, r6
003aa104: mov      r1, r8
003aa108: str      sl, [r6, #0x20]
003aa10c: str      sl, [r6, #0x24]
003aa110: mov      r0, r4
003aa114: bl       #0x513ce4
003aa118: mov      r1, sl
003aa11c: mov      r0, #0x24
003aa120: bl       #0x310570
003aa124: ldr      r8, [pc, #0x80]
003aa128: mov      r6, r0
003aa12c: mov      r2, sp
003aa130: add      r8, pc, r8
003aa134: mov      r1, r8
003aa138: str      sb, [r0], #8
003aa13c: bl       #0x3140ec
003aa140: ldr      r3, [pc, #0x68]
003aa144: add      r7, r7, #0x3c
003aa148: rsb      r7, r4, r7
003aa14c: ldr      r3, [r5, r3]
003aa150: str      r7, [r6, #4]
003aa154: mov      r0, r4
003aa158: add      r3, r3, #8
003aa15c: str      r3, [r6]
003aa160: mov      r3, #0
003aa164: str      r3, [r6, #0x20]
003aa168: mov      r1, r8
003aa16c: mov      r2, r6
003aa170: bl       #0x513ce4
003aa174: add      sp, sp, #8
003aa178: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZN11PropertyMap11AddPropertyISsEEvPKcRT_
0033ef7c: ldr      r3, [pc, #0x80]
0033ef80: ldr      ip, [pc, #0x80]
0033ef84: push     {r4, r5, r6, r7, r8, lr}
0033ef88: add      r3, pc, r3
0033ef8c: ldr      r5, [r3, ip]
0033ef90: sub      sp, sp, #0x20
0033ef94: add      r4, sp, #4
0033ef98: ldr      ip, [r5]
0033ef9c: mov      r6, r0
0033efa0: mov      r7, r1
0033efa4: mov      r0, r4
0033efa8: mov      r1, #0x10
0033efac: str      ip, [sp, #0x1c]
0033efb0: mov      r8, r2
0033efb4: str      r4, [sp, #0x14]
0033efb8: str      r4, [sp, #0x18]
0033efbc: bl       #0x31167c
0033efc0: ldr      r3, [sp, #0x14]
0033efc4: mov      r2, #0
0033efc8: mov      r1, r7
0033efcc: strb     r2, [r3]
0033efd0: mov      r0, r6
0033efd4: mov      r2, r8
0033efd8: mov      r3, r4
0033efdc: bl       #0x33e404
0033efe0: mov      r0, r4
0033efe4: bl       #0x3139ac
0033efe8: ldr      r2, [sp, #0x1c]
0033efec: ldr      r3, [r5]
0033eff0: cmp      r2, r3
0033eff4: bne      #0x33f000
0033eff8: add      sp, sp, #0x20
0033effc: pop      {r4, r5, r6, r7, r8, pc}
0033f000: bl       #0x30e310
0033f004: rsbeq    r5, r5, r8, lsl #22
0033f008: andeq    r4, r0, ip, lsr #1

# _ZN11PropertyMap11AddPropertyISsEEvPKcRT_S3_
0033e404: ldr      ip, [pc, #0x98]
0033e408: push     {r4, r5, r6, r7, r8, sl, lr}
0033e40c: ldr      lr, [pc, #0x94]
0033e410: add      ip, pc, ip
0033e414: sub      sp, sp, #0x2c
0033e418: ldr      r5, [ip, lr]
0033e41c: add      r4, sp, #0xc
0033e420: mov      r7, r0
0033e424: ldr      lr, [r5]
0033e428: mov      r6, r1
0033e42c: mov      sl, r2
0033e430: ldr      r1, [r3, #0x14]
0033e434: ldr      r2, [r3, #0x10]
0033e438: mov      r0, r4
0033e43c: str      lr, [sp, #0x24]
0033e440: str      r4, [sp, #0x1c]
0033e444: str      r4, [sp, #0x20]
0033e448: bl       #0x3116e8
0033e44c: mov      r1, #0
0033e450: mov      r0, #0x38
0033e454: bl       #0x310570
0033e458: mov      r3, sl
0033e45c: mov      r8, r0
0033e460: mov      r1, r7
0033e464: mov      r2, r6
0033e468: str      r4, [sp]
0033e46c: bl       #0x33e380
0033e470: mov      r2, r8
0033e474: mov      r0, r7
0033e478: mov      r1, r6
0033e47c: bl       #0x513ce4
0033e480: mov      r0, r4
0033e484: bl       #0x3139ac
0033e488: ldr      r2, [sp, #0x24]
0033e48c: ldr      r3, [r5]
0033e490: cmp      r2, r3
0033e494: bne      #0x33e4a0
0033e498: add      sp, sp, #0x2c
0033e49c: pop      {r4, r5, r6, r7, r8, sl, pc}
0033e4a0: bl       #0x30e310
0033e4a4: rsbeq    r6, r5, r0, lsl #13
0033e4a8: andeq    r4, r0, ip, lsr #1

# _ZN18SimpleTypePropertyISsE17SetToDefaultValueEPv
0033e298: mov      r3, r0
0033e29c: ldr      r0, [r0, #4]
0033e2a0: add      r2, r3, #0x20
0033e2a4: add      r0, r1, r0
0033e2a8: cmp      r0, r2
0033e2ac: bxeq     lr
0033e2b0: ldr      r2, [r3, #0x30]
0033e2b4: ldr      r1, [r3, #0x34]
0033e2b8: b        #0x3109e0

# _ZN11PropertyMap11AddPropertyIbEEvPKcRT_S3_.clone.21
003a92b4: push     {r4, r5, r6, r7, r8, lr}
003a92b8: mov      r7, r0
003a92bc: sub      sp, sp, #8
003a92c0: mov      r6, r1
003a92c4: mov      r0, #0x24
003a92c8: mov      r1, #0
003a92cc: mov      r8, r2
003a92d0: bl       #0x310570
003a92d4: ldr      r5, [pc, #0x58]
003a92d8: ldr      r3, [pc, #0x58]
003a92dc: mov      r4, r0
003a92e0: add      r5, pc, r5
003a92e4: ldr      r3, [r5, r3]
003a92e8: mov      r1, r6
003a92ec: add      r2, sp, #4
003a92f0: add      r3, r3, #8
003a92f4: str      r3, [r0], #8
003a92f8: bl       #0x3140ec
003a92fc: ldr      r3, [pc, #0x38]
003a9300: rsb      r8, r7, r8
003a9304: mov      r2, #1
003a9308: ldr      r3, [r5, r3]
003a930c: strb     r2, [r4, #0x20]
003a9310: str      r8, [r4, #4]
003a9314: add      r3, r3, #8
003a9318: str      r3, [r4]
003a931c: mov      r0, r7
003a9320: mov      r1, r6
003a9324: mov      r2, r4
003a9328: bl       #0x513ce4
003a932c: add      sp, sp, #8
003a9330: pop      {r4, r5, r6, r7, r8, pc}
003a9334: ldrheq   fp, [lr], #-0x70
003a9338: andeq    r2, r0, r0, lsr r3
003a933c: andeq    r3, r0, ip, asr #28

# _ZN18SimpleTypePropertyIbE17SetToDefaultValueEPv
0033de80: ldrb     r2, [r0, #0x20]
0033de84: ldr      r3, [r0, #4]
0033de88: strb     r2, [r1, r3]
0033de8c: bx       lr

# _ZN18SimpleTypePropertyI7Point2DIiEE17SetToDefaultValueEPv
003a35f0: ldr      r3, [r0, #4]
003a35f4: ldr      r2, [r0, #0x20]
003a35f8: add      ip, r1, r3
003a35fc: str      r2, [r1, r3]
003a3600: ldr      r3, [r0, #0x24]
003a3604: str      r3, [ip, #4]
003a3608: bx       lr

# _ZN18SimpleTypePropertyIfE17SetToDefaultValueEPv
00394f00: ldr      r2, [r0, #0x20]
00394f04: ldr      r3, [r0, #4]
00394f08: str      r2, [r1, r3]
00394f0c: bx       lr

# _ZNK9Character16GetPreSetAIStateEv
003a5784: push     {r4, lr}
003a5788: movw     r3, #0x13dc
003a578c: ldr      r2, [r0, r3]
003a5790: movw     r3, #0x13e0
003a5794: ldr      r3, [r0, r3]
003a5798: cmp      r2, r3
003a579c: beq      #0x3a57e4
003a57a0: ldr      r1, [pc, #0x44]
003a57a4: add      r4, r0, #0x13c0
003a57a8: add      r4, r4, #0xc
003a57ac: add      r1, pc, r1
003a57b0: mov      r0, r4
003a57b4: bl       #0x3a5720
003a57b8: cmp      r0, #0
003a57bc: bne      #0x3a57c4
003a57c0: pop      {r4, pc}
003a57c4: ldr      r1, [pc, #0x24]
003a57c8: mov      r0, r4
003a57cc: add      r1, pc, r1
003a57d0: bl       #0x3a5720
003a57d4: cmp      r0, #0
003a57d8: bne      #0x3a57e4
003a57dc: mov      r0, #0x11
003a57e0: pop      {r4, pc}
003a57e4: mov      r0, #3
003a57e8: pop      {r4, pc}
003a57ec: subseq   sp, r1, r4, lsl #22
003a57f0: subseq   sp, r1, ip, ror #21

# _ZN9CharacterC1EN10ObjectBase6GO_IDSE
003aa1b4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003aa1b8: add      ip, r0, #0x374
003aa1bc: sub      sp, sp, #0x3c
003aa1c0: mov      r4, r0
003aa1c4: str      ip, [sp, #0xc]
003aa1c8: bl       #0x38c398
003aa1cc: ldr      ip, [sp, #0xc]
003aa1d0: add      r5, r4, #0x4f0
003aa1d4: add      r5, r5, #0xc
003aa1d8: mov      r0, ip
003aa1dc: bl       #0x404db8
003aa1e0: add      r0, r4, #0x3b4
003aa1e4: str      r0, [sp, #0x20]
003aa1e8: add      r0, r4, #0x37c
003aa1ec: bl       #0x3ff330
003aa1f0: add      r2, r4, #0x490
003aa1f4: add      r1, r4, #0x3c8
003aa1f8: add      r2, r2, #0xc
003aa1fc: ldr      r0, [sp, #0x20]
003aa200: str      r1, [sp, #0x1c]
003aa204: str      r2, [sp, #0x14]
003aa208: bl       #0x3dbb0c
003aa20c: ldr      r0, [sp, #0x1c]
003aa210: bl       #0x3cebf0
003aa214: ldr      r0, [sp, #0x14]
003aa218: bl       #0x3c8ff4
003aa21c: add      r3, r4, #0x560
003aa220: mov      r0, r5
003aa224: str      r3, [sp, #0x18]
003aa228: ldr      sb, [pc, #0x50c]
003aa22c: bl       #0x3c1b58
003aa230: ldr      r0, [sp, #0x18]
003aa234: bl       #0x3df084
003aa238: ldr      lr, [pc, #0x500]
003aa23c: add      sb, pc, sb
003aa240: mov      r8, #0
003aa244: ldr      lr, [sb, lr]
003aa248: mov      fp, #1
003aa24c: mvn      r6, #0
003aa250: add      sl, lr, #0x324
003aa254: str      sl, [sp, #0x34]
003aa258: add      sl, lr, #0x180
003aa25c: str      sl, [sp, #0x10]
003aa260: add      sl, lr, #0x1f4
003aa264: str      sl, [sp, #0x24]
003aa268: add      sl, lr, #0x220
003aa26c: str      sl, [sp, #0x28]
003aa270: add      sl, lr, #0x230
003aa274: str      sl, [sp, #0x2c]
003aa278: add      r0, lr, #8
003aa27c: add      r1, lr, #0x15c
003aa280: add      r2, lr, #0x168
003aa284: add      sl, lr, #0x304
003aa288: str      sl, [sp, #0x30]
003aa28c: stm      r4, {r0, r1}
003aa290: str      r2, [r4, #0x24]
003aa294: ldr      r0, [sp, #0x10]
003aa298: add      lr, lr, #0x314
003aa29c: add      r7, r4, #0x1380
003aa2a0: str      r0, [r4, #0x374]
003aa2a4: ldr      r1, [sp, #0x24]
003aa2a8: add      r3, r7, #0x18
003aa2ac: movw     sl, #0x13a8
003aa2b0: str      r1, [r4, #0x37c]
003aa2b4: ldr      r2, [sp, #0x28]
003aa2b8: add      r7, r7, #0x30
003aa2bc: str      r2, [r4, #0x3b4]
003aa2c0: ldr      r0, [sp, #0x2c]
003aa2c4: str      r0, [r4, #0x3c8]
003aa2c8: ldr      r1, [sp, #0x30]
003aa2cc: str      lr, [r4, #0x4fc]
003aa2d0: mov      r0, r3
003aa2d4: str      r1, [r4, #0x49c]
003aa2d8: ldr      r2, [sp, #0x34]
003aa2dc: mov      r1, #0x10
003aa2e0: str      r2, [r4, #0x560]
003aa2e4: movw     r2, #0x1394
003aa2e8: strb     r8, [r4, r2]
003aa2ec: movw     r2, #0x1395
003aa2f0: strb     r8, [r4, r2]
003aa2f4: movw     r2, #0x1396
003aa2f8: strb     fp, [r4, r2]
003aa2fc: movw     r2, #0x1397
003aa300: strb     r6, [r4, r2]
003aa304: movw     r2, #0x13ac
003aa308: str      r3, [r4, r2]
003aa30c: str      r3, [r4, sl]
003aa310: bl       #0x31167c
003aa314: ldr      r3, [r4, sl]
003aa318: mov      sl, #0x13c0
003aa31c: mov      r0, r7
003aa320: strb     r8, [r3]
003aa324: movw     r3, #0x13c4
003aa328: str      r7, [r4, r3]
003aa32c: mov      r1, #0x10
003aa330: str      r7, [r4, sl]
003aa334: bl       #0x31167c
003aa338: ldr      r2, [r4, sl]
003aa33c: add      r7, r4, sl
003aa340: add      r3, r7, #0xc
003aa344: strb     r8, [r2]
003aa348: movw     r2, #0x13c8
003aa34c: strh     r6, [r4, r2]
003aa350: movw     r2, #0x13ca
003aa354: strh     r6, [r4, r2]
003aa358: movw     sl, #0x13dc
003aa35c: movw     r2, #0x13e0
003aa360: str      r3, [r4, r2]
003aa364: mov      r0, r3
003aa368: str      r3, [r4, sl]
003aa36c: mov      r1, #0x10
003aa370: bl       #0x31167c
003aa374: ldr      r3, [r4, sl]
003aa378: add      r7, r7, #0x28
003aa37c: movw     sl, #0x13f8
003aa380: strb     r8, [r3]
003aa384: movw     r3, #0x13e4
003aa388: strb     fp, [r4, r3]
003aa38c: movw     r3, #0x13fc
003aa390: str      r7, [r4, r3]
003aa394: mov      r0, r7
003aa398: str      r7, [r4, sl]
003aa39c: mov      r1, #0x10
003aa3a0: bl       #0x31167c
003aa3a4: ldr      r3, [r4, sl]
003aa3a8: add      r7, r4, #0x1400
003aa3ac: movw     sl, #0x1410
003aa3b0: strb     r8, [r3]
003aa3b4: movw     r3, #0x1414
003aa3b8: str      r7, [r4, r3]
003aa3bc: mov      r0, r7
003aa3c0: str      r7, [r4, sl]
003aa3c4: mov      r1, #0x10
003aa3c8: bl       #0x31167c
003aa3cc: ldr      r3, [r4, sl]
003aa3d0: add      r7, r7, #0x18
003aa3d4: movw     sl, #0x1428
003aa3d8: strb     r8, [r3]
003aa3dc: movw     r3, #0x142c
003aa3e0: str      r7, [r4, r3]
003aa3e4: mov      r0, r7
003aa3e8: str      r7, [r4, sl]
003aa3ec: mov      r1, #0x10
003aa3f0: bl       #0x31167c
003aa3f4: ldr      r2, [r4, sl]
003aa3f8: mov      r3, #0
003aa3fc: mov      r1, #0xbf000000
003aa400: strb     r8, [r2]
003aa404: movw     r2, #0x14a8
003aa408: strb     r6, [r4, r2]
003aa40c: movw     r2, #0x1430
003aa410: strb     fp, [r4, r2]
003aa414: movw     r2, #0x1434
003aa418: str      r8, [r4, r2]
003aa41c: movw     r2, #0x1438
003aa420: str      r8, [r4, r2]
003aa424: movw     r2, #0x1448
003aa428: strb     fp, [r4, r2]
003aa42c: movw     r2, #0x1449
003aa430: strb     r8, [r4, r2]
003aa434: movw     r2, #0x144c
003aa438: str      r8, [r4, r2]
003aa43c: movw     r2, #0x1450
003aa440: str      r3, [r4, r2]
003aa444: movw     r2, #0x1454
003aa448: str      r3, [r4, r2]
003aa44c: movw     r2, #0x1458
003aa450: str      r3, [r4, r2]
003aa454: movw     r2, #0x145c
003aa458: str      r3, [r4, r2]
003aa45c: movw     r2, #0x1460
003aa460: str      r3, [r4, r2]
003aa464: movw     r2, #0x1464
003aa468: str      r3, [r4, r2]
003aa46c: movw     r2, #0x1468
003aa470: str      r3, [r4, r2]
003aa474: movw     r2, #0x146c
003aa478: str      r3, [r4, r2]
003aa47c: movw     r2, #0x1470
003aa480: str      r3, [r4, r2]
003aa484: movw     r2, #0x1474
003aa488: str      r3, [r4, r2]
003aa48c: movw     r2, #0x1478
003aa490: str      r3, [r4, r2]
003aa494: movw     r2, #0x147c
003aa498: str      r3, [r4, r2]
003aa49c: mov      r2, #0x1480
003aa4a0: strb     r8, [r4, r2]
003aa4a4: movw     r2, #0x1481
003aa4a8: strb     r8, [r4, r2]
003aa4ac: movw     r2, #0x1484
003aa4b0: str      r8, [r4, r2]
003aa4b4: movw     r2, #0x1488
003aa4b8: str      r8, [r4, r2]
003aa4bc: movw     r2, #0x148c
003aa4c0: str      r8, [r4, r2]
003aa4c4: movw     r2, #0x1490
003aa4c8: str      r8, [r4, r2]
003aa4cc: movw     r2, #0x1494
003aa4d0: str      r8, [r4, r2]
003aa4d4: movw     r2, #0x1498
003aa4d8: str      r6, [r4, r2]
003aa4dc: movw     r2, #0x149c
003aa4e0: str      r8, [r4, r2]
003aa4e4: movw     r2, #0x14a0
003aa4e8: str      r8, [r4, r2]
003aa4ec: movw     r2, #0x14a4
003aa4f0: str      r8, [r4, r2]
003aa4f4: movw     r2, #0x14aa
003aa4f8: strh     r8, [r4, r2]
003aa4fc: movw     r2, #0x14ac
003aa500: strb     r8, [r4, r2]
003aa504: movw     r2, #0x14d8
003aa508: str      r3, [r4, r2]
003aa50c: add      r1, r1, #0x800000
003aa510: movw     r2, #0x14fc
003aa514: str      r1, [r4, r2]
003aa518: movw     r2, #0x1504
003aa51c: str      r6, [r4, r2]
003aa520: movw     r2, #0x14ad
003aa524: strb     r8, [r4, r2]
003aa528: movw     r2, #0x14b0
003aa52c: str      r3, [r4, r2]
003aa530: movw     r2, #0x14b4
003aa534: str      r3, [r4, r2]
003aa538: movw     r2, #0x14b8
003aa53c: str      r3, [r4, r2]
003aa540: movw     r2, #0x14bc
003aa544: str      r3, [r4, r2]
003aa548: mov      r2, #0x14c0
003aa54c: str      r3, [r4, r2]
003aa550: movw     r2, #0x14c4
003aa554: str      r3, [r4, r2]
003aa558: movw     r3, #0x14c8
003aa55c: strb     r8, [r4, r3]
003aa560: movw     r3, #0x14ca
003aa564: strh     r6, [r4, r3]
003aa568: movw     r3, #0x14cc
003aa56c: str      r8, [r4, r3]
003aa570: movw     r3, #0x14d0
003aa574: strh     r8, [r4, r3]
003aa578: movw     r3, #0x14d4
003aa57c: str      r8, [r4, r3]
003aa580: movw     r3, #0x14dc
003aa584: strb     r8, [r4, r3]
003aa588: movw     r3, #0x14e4
003aa58c: strb     r8, [r4, r3]
003aa590: movw     r3, #0x14e5
003aa594: strb     r8, [r4, r3]
003aa598: movw     r3, #0x14e8
003aa59c: str      r8, [r4, r3]
003aa5a0: movw     r3, #0x14ec
003aa5a4: str      r8, [r4, r3]
003aa5a8: add      r7, r4, #0x1500
003aa5ac: movw     r3, #0x14f0
003aa5b0: add      r0, r4, #0x1a40
003aa5b4: strb     r8, [r4, r3]
003aa5b8: add      r0, r0, #8
003aa5bc: mov      r3, #0x1500
003aa5c0: add      r7, r7, #8
003aa5c4: str      r6, [r4, r3]
003aa5c8: str      r0, [sp, #0x10]
003aa5cc: mov      r0, r7
003aa5d0: bl       #0x3a6a24
003aa5d4: ldr      r0, [sp, #0x10]
003aa5d8: bl       #0x3a6a24
003aa5dc: add      r0, r4, #0x304
003aa5e0: mov      r1, r4
003aa5e4: strb     fp, [r4, #0x28]
003aa5e8: bl       #0x4a191c
003aa5ec: strb     fp, [r4, #0x1c4]
003aa5f0: strb     fp, [r4, #0x85]
003aa5f4: mov      r0, #0x10
003aa5f8: mov      r1, r8
003aa5fc: bl       #0x310570
003aa600: ldr      r3, [pc, #0x13c]
003aa604: ldr      ip, [sp, #0xc]
003aa608: mov      r6, r0
003aa60c: ldr      r3, [sb, r3]
003aa610: cmp      ip, r8
003aa614: strb     r8, [r6, #0xa]
003aa618: add      r3, r3, #8
003aa61c: str      r8, [r0, #0xc]
003aa620: stm      r0, {r3, ip}
003aa624: strb     r8, [r6, #8]
003aa628: strb     r8, [r6, #9]
003aa62c: beq      #0x3aa6e0
003aa630: mov      r0, ip
003aa634: mov      r1, r6
003aa638: bl       #0x404e10
003aa63c: ldr      r3, [r4, #0x378]
003aa640: ldr      r0, [sp, #0x20]
003aa644: mov      r1, r4
003aa648: str      r4, [r3, #0xc]
003aa64c: bl       #0x3db480
003aa650: ldr      r0, [sp, #0x1c]
003aa654: mov      r1, r4
003aa658: bl       #0x3cb7c0
003aa65c: ldr      r0, [sp, #0x14]
003aa660: mov      r1, r4
003aa664: bl       #0x3c9890
003aa668: mov      r0, r5
003aa66c: mov      r1, r4
003aa670: bl       #0x3c1600
003aa674: ldr      r0, [sp, #0x18]
003aa678: mov      r1, r4
003aa67c: bl       #0x3dec0c
003aa680: mov      r6, #0
003aa684: str      r4, [r4, #0x380]
003aa688: mov      r1, r6
003aa68c: mov      r0, r5
003aa690: add      r6, r6, #1
003aa694: bl       #0x3c7318
003aa698: cmp      r6, #0x14
003aa69c: bne      #0x3aa688
003aa6a0: mov      r1, #0
003aa6a4: movw     r2, #0x14e0
003aa6a8: str      r1, [r4, r2]
003aa6ac: mvn      r3, #0
003aa6b0: movw     r2, #0x14f4
003aa6b4: str      r3, [r4, r2]
003aa6b8: str      r7, [r4, #0x100]
003aa6bc: ldr      sl, [sp, #0x10]
003aa6c0: movw     r2, #0x14f8
003aa6c4: mov      r0, r4
003aa6c8: str      sl, [r4, #0x104]
003aa6cc: str      r3, [r4, r2]
003aa6d0: mov      r3, #1
003aa6d4: strb     r3, [r4, #0xf8]
003aa6d8: add      sp, sp, #0x3c
003aa6dc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003aa6e0: ldr      r3, [pc, #0x60]
003aa6e4: ldr      r3, [sb, r3]
003aa6e8: ldr      r3, [r3]
003aa6ec: cmp      r3, #2
003aa6f0: streq    ip, [r4, #0x374]
003aa6f4: beq      #0x3aa630
003aa6f8: cmp      r3, #1
003aa6fc: bne      #0x3aa630
003aa700: ldr      r0, [pc, #0x44]
003aa704: ldr      r1, [pc, #0x44]
003aa708: ldr      r2, [pc, #0x44]
003aa70c: ldr      r0, [sb, r0]
003aa710: ldr      r3, [pc, #0x40]
003aa714: mov      lr, #0x44
003aa718: add      r1, pc, r1
003aa71c: add      r0, r0, #0xa8
003aa720: add      r2, pc, r2
003aa724: add      r3, pc, r3
003aa728: str      ip, [sp, #0xc]
003aa72c: str      lr, [sp]
003aa730: bl       #0x30e004
003aa734: ldr      ip, [sp, #0xc]
003aa738: b        #0x3aa630
003aa73c: subseq   sl, lr, r4, asr r8
003aa740: andeq    r2, r0, r8, lsl #28
003aa744: andeq    r2, r0, r4, lsr #21
003aa748: andeq    r3, r0, r0, asr #19
003aa74c: andeq    r1, r0, r0, asr #19
003aa750: subseq   r3, r1, r0, asr #25
003aa754: subseq   r8, r1, r0, lsr #27
003aa758: subseq   r8, r1, ip, lsr #27
