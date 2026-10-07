
# _ZN6CharAI10AI_SetDeadEv
003d6cdc: mov      r1, #0
003d6ce0: push     {r4, lr}
003d6ce4: mov      r2, r1
003d6ce8: mov      r4, r0
003d6cec: bl       #0x3d6890
003d6cf0: mov      r0, r4
003d6cf4: bl       #0x3d49c4
003d6cf8: ldr      r0, [r4, #4]
003d6cfc: mov      r1, #0
003d6d00: mov      r2, r1
003d6d04: add      r0, r0, #0x4f0
003d6d08: mov      r3, #1
003d6d0c: add      r0, r0, #0xc
003d6d10: bl       #0x3c58c8
003d6d14: ldr      r0, [r4, #4]
003d6d18: ldr      r1, [r4, #0x10]
003d6d1c: add      r0, r0, #0x3b4
003d6d20: bl       #0x3db2d8
003d6d24: ldr      r0, [r4, #4]
003d6d28: ldr      r1, [r4, #0x14]
003d6d2c: add      r0, r0, #0x3b4
003d6d30: bl       #0x3db2d8
003d6d34: mvn      r3, #0
003d6d38: str      r3, [r4, #0x14]
003d6d3c: str      r3, [r4, #0x10]
003d6d40: mov      r0, r4
003d6d44: bl       #0x3d5fa8
003d6d48: mov      r0, r4
003d6d4c: mov      r1, #0
003d6d50: bl       #0x3d6abc
003d6d54: mov      r0, r4
003d6d58: bl       #0x3d8ae0
003d6d5c: mov      r0, r4
003d6d60: pop      {r4, lr}
003d6d64: b        #0x3d8a98

# _ZN6CharAI17AI_SyncLastTargetEv
003d49c4: ldr      r3, [r0, #0x40]
003d49c8: str      r3, [r0, #0x44]
003d49cc: bx       lr

# _ZN6CharAI6OnDiedEP10GameObject
003d1000: push     {r4, r5, r6, lr}
003d1004: mov      r4, r0
003d1008: ldr      r0, [r0, #0x34]
003d100c: mov      r5, r1
003d1010: cmp      r0, #0
003d1014: beq      #0x3d1024
003d1018: ldr      r1, [r4, #4]
003d101c: mov      r2, r5
003d1020: bl       #0x3d2628
003d1024: ldr      r3, [r4, #0x1c]
003d1028: cmp      r3, #0
003d102c: beq      #0x3d1044
003d1030: mov      r0, r3
003d1034: mov      r1, r5
003d1038: ldr      r3, [r3]
003d103c: mov      lr, pc
003d1040: ldr      pc, [r3, #0x24]
003d1044: mov      r0, r4
003d1048: pop      {r4, r5, r6, lr}
003d104c: b        #0x3d6cdc

# _ZN10CharTimers8TMR_StopEj
003db2d8: ldr      r3, [r0, #8]
003db2dc: ldr      r2, [r0, #0xc]
003db2e0: rsb      r2, r3, r2
003db2e4: cmp      r1, r2, asr #5
003db2e8: addlo    r3, r3, r1, lsl #5
003db2ec: movlo    r2, #0
003db2f0: strblo   r2, [r3, #0x14]
003db2f4: bx       lr

# _ZN6CharAI16AI_ClearAllAggroEv
003d5fa8: push     {r4, r5, r6, r7, r8, sl, lr}
003d5fac: mov      r3, #0
003d5fb0: sub      sp, sp, #0x14
003d5fb4: mov      r5, r0
003d5fb8: ldr      r1, [r5, #0x8c]
003d5fbc: mov      r0, sp
003d5fc0: str      r3, [sp, #8]
003d5fc4: str      r3, [sp]
003d5fc8: str      r3, [sp, #4]
003d5fcc: ldr      r4, [r5, #0x84]
003d5fd0: bl       #0x3d5e14
003d5fd4: mov      sl, sp
003d5fd8: add      r6, r5, #0x7c
003d5fdc: add      r7, r5, #4
003d5fe0: add      r8, sp, #0xc
003d5fe4: cmp      r4, r6
003d5fe8: beq      #0x3d60a4
003d5fec: ldr      r0, [r4, #0x10]
003d5ff0: ldr      r3, [r0, #0x460]
003d5ff4: cmp      r3, #0
003d5ff8: beq      #0x3d6058
003d5ffc: add      r0, r0, #0x450
003d6000: add      r0, r0, #0xc
003d6004: ldr      ip, [r7]
003d6008: mov      r1, r0
003d600c: b        #0x3d6014
003d6010: mov      r3, r2
003d6014: ldr      r2, [r3, #0x10]
003d6018: cmp      r2, ip
003d601c: ldrlo    r2, [r3, #0xc]
003d6020: ldrhs    r2, [r3, #8]
003d6024: movlo    r3, r1
003d6028: mov      r1, r3
003d602c: cmp      r2, #0
003d6030: bne      #0x3d6010
003d6034: cmp      r0, r3
003d6038: beq      #0x3d6058
003d603c: ldr      r1, [r5, #4]
003d6040: ldr      r2, [r3, #0x10]
003d6044: cmp      r1, r2
003d6048: blo      #0x3d6058
003d604c: mov      r1, r8
003d6050: str      r3, [sp, #0xc]
003d6054: bl       #0x3d5d9c
003d6058: ldmib    sp, {r1, r3}
003d605c: cmp      r1, r3
003d6060: beq      #0x3d614c
003d6064: ldr      r3, [r4, #0x10]
003d6068: str      r3, [r1]
003d606c: ldr      r3, [sp, #4]
003d6070: add      r3, r3, #4
003d6074: str      r3, [sp, #4]
003d6078: ldr      r2, [r4, #0xc]
003d607c: cmp      r2, #0
003d6080: bne      #0x3d608c
003d6084: b        #0x3d6118
003d6088: mov      r2, r3
003d608c: ldr      r3, [r2, #8]
003d6090: cmp      r3, #0
003d6094: bne      #0x3d6088
003d6098: mov      r4, r2
003d609c: cmp      r4, r6
003d60a0: bne      #0x3d5fec
003d60a4: ldr      r3, [r5, #0x8c]
003d60a8: cmp      r3, #0
003d60ac: bne      #0x3d615c
003d60b0: ldm      sp, {r0, r3}
003d60b4: rsb      r3, r0, r3
003d60b8: lsrs     r3, r3, #2
003d60bc: beq      #0x3d60f0
003d60c0: mov      r4, #0
003d60c4: ldr      r3, [r0, r4, lsl #2]
003d60c8: ldr      r1, [r5, #4]
003d60cc: add      r4, r4, #1
003d60d0: add      r0, r3, #0x3c8
003d60d4: ldr      r3, [r3, #0x3c8]
003d60d8: mov      lr, pc
003d60dc: ldr      pc, [r3, #0x3c]
003d60e0: ldm      sp, {r0, r3}
003d60e4: rsb      r3, r0, r3
003d60e8: cmp      r4, r3, asr #2
003d60ec: blo      #0x3d60c4
003d60f0: cmp      r0, #0
003d60f4: beq      #0x3d6110
003d60f8: ldr      r1, [sp, #8]
003d60fc: rsb      r1, r0, r1
003d6100: bic      r1, r1, #3
003d6104: cmp      r1, #0x80
003d6108: bhi      #0x3d6180
003d610c: bl       #0x708f00
003d6110: add      sp, sp, #0x14
003d6114: pop      {r4, r5, r6, r7, r8, sl, pc}
003d6118: ldr      r3, [r4, #4]
003d611c: ldr      r1, [r3, #0xc]
003d6120: cmp      r4, r1
003d6124: bne      #0x3d6140
003d6128: mov      r4, r3
003d612c: ldr      r3, [r3, #4]
003d6130: ldr      r2, [r3, #0xc]
003d6134: cmp      r2, r4
003d6138: beq      #0x3d6128
003d613c: ldr      r2, [r4, #0xc]
003d6140: cmp      r3, r2
003d6144: movne    r4, r3
003d6148: b        #0x3d5fe4
003d614c: mov      r0, sp
003d6150: add      r2, r4, #0x10
003d6154: bl       #0x3d5ee0
003d6158: b        #0x3d6078
003d615c: mov      r0, r4
003d6160: ldr      r1, [r5, #0x80]
003d6164: bl       #0x3cd34c
003d6168: mov      r3, #0
003d616c: str      r4, [r5, #0x88]
003d6170: str      r3, [r5, #0x8c]
003d6174: str      r4, [r5, #0x84]
003d6178: str      r3, [r5, #0x80]
003d617c: b        #0x3d60b0
003d6180: bl       #0x310440
003d6184: b        #0x3d6110

# _ZN6CharAI12AI_SetTargetEP10GameObjectb
003d6890: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003d6894: ldr      r5, [pc, #0x204]
003d6898: ldr      r7, [pc, #0x204]
003d689c: sub      sp, sp, #0x78
003d68a0: add      r5, pc, r5
003d68a4: ldr      r3, [r5, r7]
003d68a8: mov      r4, r0
003d68ac: cmp      r2, #0
003d68b0: ldr      r3, [r3]
003d68b4: mov      r6, r1
003d68b8: str      r1, [r4, #0x3c]
003d68bc: str      r3, [sp, #0x74]
003d68c0: bne      #0x3d6a20
003d68c4: ldr      r3, [r0, #0x40]
003d68c8: ldr      sb, [pc, #0x1d8]
003d68cc: add      r8, sp, #0x5c
003d68d0: cmp      r3, r1
003d68d4: ldrne    r1, [r0, #4]
003d68d8: ldr      sl, [r5, sb]
003d68dc: movwne   r3, #0x14d0
003d68e0: strhne   r2, [r1, r3]
003d68e4: mov      r0, sl
003d68e8: bl       #0x337888
003d68ec: ldr      r1, [pc, #0x1b8]
003d68f0: add      r2, sp, #0x10
003d68f4: mov      r0, r8
003d68f8: add      r1, pc, r1
003d68fc: bl       #0x3140ec
003d6900: mov      r0, sl
003d6904: mov      r1, r8
003d6908: bl       #0x337a88
003d690c: mov      sl, r0
003d6910: ldr      r0, [sp, #0x70]
003d6914: cmp      r0, r8
003d6918: beq      #0x3d6938
003d691c: cmp      r0, #0
003d6920: beq      #0x3d6938
003d6924: ldr      r1, [sp, #0x5c]
003d6928: rsb      r1, r0, r1
003d692c: cmp      r1, #0x80
003d6930: bhi      #0x3d6a4c
003d6934: bl       #0x708f00
003d6938: cmp      sl, #0
003d693c: beq      #0x3d69a4
003d6940: ldr      r3, [r4, #0x40]
003d6944: cmp      r3, r6
003d6948: beq      #0x3d69a4
003d694c: subs     r3, r3, #0
003d6950: movne    r3, #1
003d6954: subs     r2, r6, #0
003d6958: movne    r2, #1
003d695c: tst      r2, r3
003d6960: bne      #0x3d6a28
003d6964: cmp      r3, #0
003d6968: beq      #0x3d6a18
003d696c: ldr      sl, [r5, sb]
003d6970: add      r8, sp, #0x2c
003d6974: mov      r0, sl
003d6978: bl       #0x337888
003d697c: ldr      r1, [pc, #0x12c]
003d6980: add      r2, sp, #8
003d6984: mov      r0, r8
003d6988: add      r1, pc, r1
003d698c: bl       #0x3140ec
003d6990: mov      r0, sl
003d6994: mov      r1, r8
003d6998: bl       #0x337a88
003d699c: mov      r0, r8
003d69a0: bl       #0x318254
003d69a4: cmp      r6, #0
003d69a8: str      r6, [r4, #0x40]
003d69ac: beq      #0x3d69fc
003d69b0: ldr      r0, [r4, #4]
003d69b4: bl       #0x3a2fec
003d69b8: ldr      r2, [r4, #0x44]
003d69bc: ldr      r3, [r4, #0x40]
003d69c0: cmp      r3, r2
003d69c4: movne    r2, #0
003d69c8: strbne   r2, [r4, #0x4c]
003d69cc: movne    r2, r3
003d69d0: str      r2, [r4, #0x44]
003d69d4: mov      r0, r3
003d69d8: ldr      r3, [r3]
003d69dc: mov      lr, pc
003d69e0: ldr      pc, [r3, #0x34]
003d69e4: eor      r0, r0, #1
003d69e8: strb     r0, [r4, #0x48]
003d69ec: ldr      r1, [r4, #0x40]
003d69f0: mov      r0, r4
003d69f4: bl       #0x3d4ed8
003d69f8: strb     r0, [r4, #0x49]
003d69fc: ldr      r3, [r5, r7]
003d6a00: ldr      r2, [sp, #0x74]
003d6a04: ldr      r3, [r3]
003d6a08: cmp      r2, r3
003d6a0c: bne      #0x3d6a9c
003d6a10: add      sp, sp, #0x78
003d6a14: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003d6a18: cmp      r2, #0
003d6a1c: bne      #0x3d6a5c
003d6a20: str      r6, [r4, #0x40]
003d6a24: b        #0x3d69fc
003d6a28: ldr      sl, [r5, sb]
003d6a2c: add      r8, sp, #0x44
003d6a30: mov      r0, sl
003d6a34: bl       #0x337888
003d6a38: ldr      r1, [pc, #0x74]
003d6a3c: add      r2, sp, #0xc
003d6a40: mov      r0, r8
003d6a44: add      r1, pc, r1
003d6a48: b        #0x3d698c
003d6a4c: bl       #0x310440
003d6a50: cmp      sl, #0
003d6a54: beq      #0x3d69a4
003d6a58: b        #0x3d6940
003d6a5c: ldr      sl, [r5, sb]
003d6a60: add      r8, sp, #0x14
003d6a64: mov      r0, sl
003d6a68: bl       #0x337888
003d6a6c: ldr      r1, [pc, #0x44]
003d6a70: add      r2, sp, #4
003d6a74: mov      r0, r8
003d6a78: add      r1, pc, r1
003d6a7c: bl       #0x3140ec
003d6a80: mov      r1, r8
003d6a84: mov      r0, sl
003d6a88: bl       #0x337a88
003d6a8c: mov      r0, r8
003d6a90: bl       #0x318254
003d6a94: str      r6, [r4, #0x40]
003d6a98: b        #0x3d69b0
003d6a9c: bl       #0x30e310
003d6aa0: ldrsheq  lr, [fp], #-0x10
003d6aa4: andeq    r4, r0, ip, lsr #1
003d6aa8: andeq    r0, r0, r4, lsl #17
003d6aac: subeq    lr, lr, r0, ror #27
003d6ab0: subeq    lr, lr, r8, ror #26
003d6ab4: subeq    lr, lr, ip, lsr #25
003d6ab8: subeq    lr, lr, r8, ror ip
