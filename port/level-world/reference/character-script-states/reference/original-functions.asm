
# _ZN12CharAIScriptD1Ev
003d926c: push     {r4, r5, r6, lr}
003d9270: ldr      r3, [pc, #0x54]
003d9274: ldr      r2, [pc, #0x54]
003d9278: ldr      r1, [r0, #0xac]
003d927c: add      r3, pc, r3
003d9280: ldr      r2, [r3, r2]
003d9284: cmp      r1, #0
003d9288: mov      r4, r0
003d928c: add      r2, r2, #8
003d9290: str      r2, [r0]
003d9294: beq      #0x3d92bc
003d9298: add      r5, r0, #0x9c
003d929c: mov      r0, r5
003d92a0: ldr      r1, [r4, #0xa0]
003d92a4: bl       #0x3d91e8
003d92a8: mov      r3, #0
003d92ac: str      r5, [r4, #0xa8]
003d92b0: str      r3, [r4, #0xac]
003d92b4: str      r5, [r4, #0xa4]
003d92b8: str      r3, [r4, #0xa0]
003d92bc: mov      r0, r4
003d92c0: bl       #0x37bec0
003d92c4: mov      r0, r4
003d92c8: pop      {r4, r5, r6, pc}
003d92cc: subseq   fp, fp, r4, lsl r8
003d92d0: muleq    r0, ip, fp

# _ZN12CharAIScript24CharAIScriptBindFunctionEv
003d8ec8: push     {r4, r5, r6, lr}
003d8ecc: ldr      r4, [pc, #0x44]
003d8ed0: ldr      r3, [pc, #0x44]
003d8ed4: ldr      r1, [pc, #0x44]
003d8ed8: mov      r5, r0
003d8edc: add      r4, pc, r4
003d8ee0: add      r6, r0, #0x10
003d8ee4: ldr      r2, [r4, r3]
003d8ee8: mov      r0, r6
003d8eec: mov      r3, r5
003d8ef0: add      r1, pc, r1
003d8ef4: bl       #0x31a4d4
003d8ef8: ldr      r3, [pc, #0x24]
003d8efc: ldr      r1, [pc, #0x24]
003d8f00: mov      r0, r6
003d8f04: ldr      r2, [r4, r3]
003d8f08: add      r1, pc, r1
003d8f0c: mov      r3, r5
003d8f10: pop      {r4, r5, r6, lr}
003d8f14: b        #0x31a4d4
003d8f18: ldrheq   fp, [fp], #-0xb4
003d8f1c: andeq    r4, r0, ip, asr r6
003d8f20: subeq    ip, lr, r8, ror #17
003d8f24: andeq    r1, r0, r0, ror sb
003d8f28: subeq    ip, lr, r0, ror #17

# _ZN12CharAIScriptC1Eb
003d8f44: push     {r4, r5, r6, lr}
003d8f48: ldr      r5, [pc, #0x58]
003d8f4c: mov      r4, r0
003d8f50: mov      r6, r1
003d8f54: bl       #0x37c674
003d8f58: ldr      r1, [pc, #0x4c]
003d8f5c: add      r5, pc, r5
003d8f60: mov      r3, #0
003d8f64: ldr      r1, [r5, r1]
003d8f68: mov      r2, r4
003d8f6c: str      r3, [r4, #0x98]
003d8f70: add      r1, r1, #8
003d8f74: str      r1, [r4]
003d8f78: str      r3, [r4, #0xa0]
003d8f7c: cmp      r6, r3
003d8f80: strb     r3, [r2, #0x9c]!
003d8f84: str      r2, [r4, #0xa8]
003d8f88: str      r3, [r4, #0xb4]
003d8f8c: str      r2, [r4, #0xa4]
003d8f90: str      r3, [r4, #0xac]
003d8f94: bne      #0x3d8fa0
003d8f98: mov      r0, r4
003d8f9c: bl       #0x3d8ec8
003d8fa0: mov      r0, r4
003d8fa4: pop      {r4, r5, r6, pc}
003d8fa8: subseq   fp, fp, r4, lsr fp
003d8fac: muleq    r0, ip, fp

# _ZN12CharAIScript12_ChangeStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003d9710: push     {r4, r5, r6, lr}
003d9714: ldr      r3, [r0, #4]
003d9718: mov      r4, r2
003d971c: sub      sp, sp, #8
003d9720: ldm      r3, {r0, r2}
003d9724: rsb      r3, r0, r2
003d9728: asr      r3, r3, #4
003d972c: add      r2, r3, r3, lsl #3
003d9730: add      r2, r2, r2, lsl #6
003d9734: add      r2, r3, r2, lsl #3
003d9738: add      r2, r2, r2, lsl #15
003d973c: add      r3, r3, r2, lsl #3
003d9740: cmn      r3, #1
003d9744: beq      #0x3d9750
003d9748: add      sp, sp, #8
003d974c: pop      {r4, r5, r6, pc}
003d9750: bl       #0x31c49c
003d9754: add      r5, r4, #0x9c
003d9758: add      r1, sp, #8
003d975c: str      r0, [r1, #-4]!
003d9760: mov      r0, r5
003d9764: bl       #0x3d9358
003d9768: cmp      r0, r5
003d976c: mov      r6, r0
003d9770: beq      #0x3d9748
003d9774: mov      r0, r4
003d9778: add      r6, r6, #0x28
003d977c: bl       #0x3d8e78
003d9780: str      r6, [r4, #0xb4]
003d9784: mov      r0, r4
003d9788: bl       #0x3d8e8c
003d978c: b        #0x3d9748

# _ZN11AISExternal8OnUpdateEv
003dce64: push     {r4, lr}
003dce68: mov      r4, r0
003dce6c: bl       #0x3dc798
003dce70: ldr      r3, [r4, #0xb8]
003dce74: tst      r3, #1
003dce78: beq      #0x3dce8c
003dce7c: ldr      r1, [pc, #0x1c]
003dce80: mov      r0, r4
003dce84: add      r1, pc, r1
003dce88: bl       #0x37c514
003dce8c: mov      r0, r4
003dce90: bl       #0x3d8eb4
003dce94: mov      r0, r4
003dce98: pop      {r4, lr}
003dce9c: b        #0x3d8ea0
003dcea0: subeq    r8, lr, r4, lsl #25

# _ZN12CharAIScript13CallStateInitEv
003d8e8c: ldr      r3, [r0, #0xb4]
003d8e90: cmp      r3, #0
003d8e94: bxeq     lr
003d8e98: ldr      r1, [r3, #0x44]
003d8e9c: b        #0x37c514

# _ZN12CharAIScript19CallStateConditionsEv
003d8ea0: ldr      r3, [r0, #0xb4]
003d8ea4: cmp      r3, #0
003d8ea8: bxeq     lr
003d8eac: ldr      r1, [r3, #0x2c]
003d8eb0: b        #0x37c514

# _ZN12CharAIScript15CallStateUpdateEv
003d8eb4: ldr      r3, [r0, #0xb4]
003d8eb8: cmp      r3, #0
003d8ebc: bxeq     lr
003d8ec0: ldr      r1, [r3, #0x14]
003d8ec4: b        #0x37c514

# _ZN12CharAIScript13CallStatePostEv
003d8e78: ldr      r3, [r0, #0xb4]
003d8e7c: cmp      r3, #0
003d8e80: bxeq     lr
003d8e84: ldr      r1, [r3, #0x5c]
003d8e88: b        #0x37c514

# _ZN12CharAIScript6_StateC1Ev
003d9684: push     {r4, r5, r6, lr}
003d9688: mov      r4, r0
003d968c: str      r0, [r4, #0x10]
003d9690: str      r0, [r4, #0x14]
003d9694: mov      r1, #0x10
003d9698: bl       #0x31167c
003d969c: ldr      r2, [r4, #0x10]
003d96a0: add      r3, r4, #0x18
003d96a4: mov      r5, #0
003d96a8: strb     r5, [r2]
003d96ac: mov      r0, r3
003d96b0: str      r3, [r4, #0x28]
003d96b4: str      r3, [r4, #0x2c]
003d96b8: mov      r1, #0x10
003d96bc: bl       #0x31167c
003d96c0: ldr      r2, [r4, #0x28]
003d96c4: add      r3, r4, #0x30
003d96c8: mov      r0, r3
003d96cc: strb     r5, [r2]
003d96d0: mov      r1, #0x10
003d96d4: str      r3, [r4, #0x40]
003d96d8: str      r3, [r4, #0x44]
003d96dc: bl       #0x31167c
003d96e0: ldr      r2, [r4, #0x40]
003d96e4: add      r3, r4, #0x48
003d96e8: mov      r0, r3
003d96ec: strb     r5, [r2]
003d96f0: mov      r1, #0x10
003d96f4: str      r3, [r4, #0x58]
003d96f8: str      r3, [r4, #0x5c]
003d96fc: bl       #0x31167c
003d9700: ldr      r3, [r4, #0x58]
003d9704: mov      r0, r4
003d9708: strb     r5, [r3]
003d970c: pop      {r4, r5, r6, pc}

# _ZN12CharAIScript14_RegisterStateERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003da144: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003da148: ldr      r4, [r0, #4]
003da14c: mov      r5, r0
003da150: sub      sp, sp, #0x1c
003da154: ldm      r4, {r0, r3}
003da158: rsb      r3, r0, r3
003da15c: asr      r3, r3, #4
003da160: add      r7, r3, r3, lsl #3
003da164: add      r7, r7, r7, lsl #6
003da168: add      r7, r3, r7, lsl #3
003da16c: add      r7, r7, r7, lsl #15
003da170: add      r7, r3, r7, lsl #3
003da174: rsb      r7, r7, #0
003da178: cmp      r7, #1
003da17c: bls      #0x3da1b4
003da180: cmp      r7, #5
003da184: bhi      #0x3da1b4
003da188: mov      r3, #0
003da18c: mov      r1, r3
003da190: b        #0x3da19c
003da194: cmp      r1, r7
003da198: bhs      #0x3da1bc
003da19c: add      ip, r0, r3
003da1a0: ldr      ip, [ip, #4]
003da1a4: add      r1, r1, #1
003da1a8: add      r3, r3, #0x70
003da1ac: cmp      ip, #4
003da1b0: beq      #0x3da194
003da1b4: add      sp, sp, #0x1c
003da1b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003da1bc: cmp      r7, #0
003da1c0: add      r6, r2, #0x9c
003da1c4: beq      #0x3da3a8
003da1c8: bl       #0x31c49c
003da1cc: add      r1, sp, #0x18
003da1d0: str      r0, [r1, #-4]!
003da1d4: mov      r0, r6
003da1d8: bl       #0x3d9fdc
003da1dc: ldr      r3, [r5, #4]
003da1e0: mov      r6, r0
003da1e4: ldm      r3, {r0, r1}
003da1e8: rsb      r1, r0, r1
003da1ec: asr      r1, r1, #4
003da1f0: add      r2, r1, r1, lsl #3
003da1f4: add      r2, r2, r2, lsl #6
003da1f8: add      r2, r1, r2, lsl #3
003da1fc: add      r2, r2, r2, lsl #15
003da200: add      r2, r1, r2, lsl #3
003da204: rsb      r2, r2, #0
003da208: cmp      r2, #1
003da20c: bls      #0x3da1b4
003da210: ldr      r1, [pc, #0x1a4]
003da214: ldr      sb, [pc, #0x1a4]
003da218: ldr      fp, [pc, #0x1a4]
003da21c: add      r1, pc, r1
003da220: str      r1, [sp, #8]
003da224: ldr      r1, [pc, #0x19c]
003da228: add      sb, pc, sb
003da22c: add      fp, pc, fp
003da230: add      r1, pc, r1
003da234: str      r1, [sp, #0xc]
003da238: add      sl, r6, #0x18
003da23c: add      r8, r6, #0x30
003da240: add      r7, r6, #0x48
003da244: mov      r4, #1
003da248: sub      ip, r4, #1
003da24c: cmp      ip, #3
003da250: addls    pc, pc, ip, lsl #2
003da254: b        #0x3da2a8
003da258: b        #0x3da364
003da25c: b        #0x3da320
003da260: b        #0x3da2dc
003da264: b        #0x3da268
003da268: cmp      r2, #4
003da26c: bhi      #0x3da284
003da270: mov      r0, sb
003da274: str      r3, [sp, #4]
003da278: bl       #0x708eb0
003da27c: ldr      r3, [sp, #4]
003da280: ldr      r0, [r3]
003da284: add      r0, r0, #0x1c0
003da288: bl       #0x31c49c
003da28c: str      r0, [sp, #4]
003da290: bl       #0x30de54
003da294: ldr      r1, [sp, #4]
003da298: add      r2, r1, r0
003da29c: mov      r0, r7
003da2a0: bl       #0x3109e0
003da2a4: ldr      r3, [r5, #4]
003da2a8: ldm      r3, {r0, r2}
003da2ac: add      r4, r4, #1
003da2b0: rsb      r2, r0, r2
003da2b4: asr      r2, r2, #4
003da2b8: add      r1, r2, r2, lsl #3
003da2bc: add      r1, r1, r1, lsl #6
003da2c0: add      r1, r2, r1, lsl #3
003da2c4: add      r1, r1, r1, lsl #15
003da2c8: add      r2, r2, r1, lsl #3
003da2cc: rsb      r2, r2, #0
003da2d0: cmp      r4, r2
003da2d4: blo      #0x3da248
003da2d8: b        #0x3da1b4
003da2dc: cmp      r2, #3
003da2e0: bhi      #0x3da2f8
003da2e4: mov      r0, fp
003da2e8: str      r3, [sp, #4]
003da2ec: bl       #0x708eb0
003da2f0: ldr      r3, [sp, #4]
003da2f4: ldr      r0, [r3]
003da2f8: add      r0, r0, #0x150
003da2fc: bl       #0x31c49c
003da300: str      r0, [sp, #4]
003da304: bl       #0x30de54
003da308: ldr      r1, [sp, #4]
003da30c: add      r2, r1, r0
003da310: mov      r0, r8
003da314: bl       #0x3109e0
003da318: ldr      r3, [r5, #4]
003da31c: b        #0x3da2a8
003da320: cmp      r2, #2
003da324: bhi      #0x3da33c
003da328: ldr      r0, [sp, #8]
003da32c: str      r3, [sp, #4]
003da330: bl       #0x708eb0
003da334: ldr      r3, [sp, #4]
003da338: ldr      r0, [r3]
003da33c: add      r0, r0, #0xe0
003da340: bl       #0x31c49c
003da344: str      r0, [sp, #4]
003da348: bl       #0x30de54
003da34c: ldr      r1, [sp, #4]
003da350: add      r2, r1, r0
003da354: mov      r0, sl
003da358: bl       #0x3109e0
003da35c: ldr      r3, [r5, #4]
003da360: b        #0x3da2a8
003da364: cmp      r2, #1
003da368: bhi      #0x3da380
003da36c: ldr      r0, [sp, #0xc]
003da370: str      r3, [sp, #4]
003da374: bl       #0x708eb0
003da378: ldr      r3, [sp, #4]
003da37c: ldr      r0, [r3]
003da380: add      r0, r0, #0x70
003da384: bl       #0x31c49c
003da388: str      r0, [sp, #4]
003da38c: bl       #0x30de54
003da390: ldr      r1, [sp, #4]
003da394: add      r2, r1, r0
003da398: mov      r0, r6
003da39c: bl       #0x3109e0
003da3a0: ldr      r3, [r5, #4]
003da3a4: b        #0x3da2a8
003da3a8: ldr      r0, [pc, #0x1c]
003da3ac: add      r0, pc, r0
003da3b0: bl       #0x708eb0
003da3b4: ldr      r0, [r4]
003da3b8: b        #0x3da1c8
003da3bc: subeq    r4, lr, ip, asr #4
003da3c0: subeq    r4, lr, r0, asr #4
003da3c4: subeq    r4, lr, ip, lsr r2
003da3c8: subeq    r4, lr, r8, lsr r2
003da3cc: strheq   r4, [lr], #-0xc
