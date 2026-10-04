
# _ZN6glitch5video6detail19IMaterialParametersINS0_9CMaterialENS_24ISharedMemoryBlockHeaderIS3_EEE15setParameterCvtINS0_6SColorEEEN5boost9enable_ifINS1_36SIsValidSetMaterialParamaterOverloadIT_EEbE4typeEtjRKSC_
005cad38: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005cad3c: ldr      r5, [r0, #4]
005cad40: ldr      ip, [pc, #0x1c0]
005cad44: sub      sp, sp, #0x14
005cad48: ldrh     r6, [r5, #0xe]
005cad4c: mov      r4, r0
005cad50: add      ip, pc, ip
005cad54: cmp      r6, r1
005cad58: bls      #0x5cadb8
005cad5c: ldr      r5, [r5, #0x20]
005cad60: adds     r1, r5, r1, lsl #4
005cad64: beq      #0x5cadb8
005cad68: ldr      r5, [pc, #0x19c]
005cad6c: ldrb     r8, [r1, #6]
005cad70: ldr      ip, [ip, r5]
005cad74: ldr      ip, [ip, r8, lsl #2]
005cad78: tst      ip, #0x10000
005cad7c: beq      #0x5cadb8
005cad80: ldr      ip, [r1, #8]
005cad84: cmp      r2, ip
005cad88: bhs      #0x5cadb8
005cad8c: ldr      r6, [r1, #0xc]
005cad90: add      r7, r0, #0x20
005cad94: cmp      r8, #0x10
005cad98: add      r5, r7, r6
005cad9c: beq      #0x5cadc4
005cada0: cmp      r8, #0x11
005cada4: beq      #0x5caec8
005cada8: cmp      r8, #8
005cadac: beq      #0x5cadf4
005cadb0: mov      r0, #1
005cadb4: b        #0x5cadbc
005cadb8: mov      r0, #0
005cadbc: add      sp, sp, #0x14
005cadc0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005cadc4: ldr      r2, [r3]
005cadc8: ldr      r1, [r7, r6]
005cadcc: cmp      r1, r2
005cadd0: mvnne    r2, #0
005cadd4: strne    r2, [r0, #0xc]
005cadd8: strne    r2, [r0, #0x10]
005caddc: mov      r1, r3
005cade0: mov      r0, r5
005cade4: mov      r2, #4
005cade8: bl       #0x30e868
005cadec: mov      r0, #1
005cadf0: b        #0x5cadbc
005cadf4: ldrb     r0, [r3]
005cadf8: ldrb     sb, [r3, #1]
005cadfc: ldrb     sl, [r3, #2]
005cae00: ldrb     fp, [r3, #3]
005cae04: bl       #0x30e964
005cae08: movw     r1, #0x8081
005cae0c: movt     r1, #0x3b80
005cae10: bl       #0x30ed6c
005cae14: mov      r8, r0
005cae18: mov      r0, sb
005cae1c: str      r8, [sp]
005cae20: bl       #0x30e964
005cae24: movw     r1, #0x8081
005cae28: movt     r1, #0x3b80
005cae2c: bl       #0x30ed6c
005cae30: mov      sb, r0
005cae34: mov      r0, sl
005cae38: str      sb, [sp, #4]
005cae3c: bl       #0x30e964
005cae40: movw     r1, #0x8081
005cae44: movt     r1, #0x3b80
005cae48: bl       #0x30ed6c
005cae4c: str      r0, [sp, #8]
005cae50: mov      r0, fp
005cae54: bl       #0x30e964
005cae58: movw     r1, #0x8081
005cae5c: movt     r1, #0x3b80
005cae60: bl       #0x30ed6c
005cae64: str      r0, [sp, #0xc]
005cae68: mov      r1, r8
005cae6c: ldr      r0, [r7, r6]
005cae70: bl       #0x30df8c
005cae74: cmp      r0, #0
005cae78: mov      sl, sp
005cae7c: beq      #0x5cae94
005cae80: mov      r1, sb
005cae84: ldr      r0, [r5, #4]
005cae88: bl       #0x30df8c
005cae8c: cmp      r0, #0
005cae90: bne      #0x5caedc
005cae94: mvn      r3, #0
005cae98: ldr      r8, [sl]
005cae9c: str      r3, [r4, #0xc]
005caea0: str      r3, [r4, #0x10]
005caea4: ldr      r1, [sl, #0xc]
005caea8: ldr      r2, [sl, #4]
005caeac: ldr      r3, [sl, #8]
005caeb0: mov      r0, #1
005caeb4: str      r8, [r7, r6]
005caeb8: str      r1, [r5, #0xc]
005caebc: str      r2, [r5, #4]
005caec0: str      r3, [r5, #8]
005caec4: b        #0x5cadbc
005caec8: mov      r1, r5
005caecc: mov      r2, r3
005caed0: bl       #0x5cac8c
005caed4: mov      r0, #1
005caed8: b        #0x5cadbc
005caedc: ldr      r0, [r5, #8]
005caee0: ldr      r1, [sp, #8]
005caee4: bl       #0x30df8c
005caee8: cmp      r0, #0
005caeec: beq      #0x5cae94
005caef0: ldr      r0, [r5, #0xc]
005caef4: ldr      r1, [sp, #0xc]
005caef8: bl       #0x30df8c
005caefc: cmp      r0, #0
005caf00: bne      #0x5caea4
005caf04: b        #0x5cae94
005caf08: eorseq   sb, ip, r0, asr #26
005caf0c: andeq    r2, r0, r4, lsr #25

# _ZNK6glitch7collada14CMeshSceneNode16getMaterialCountEv
006461cc: push     {r4, lr}
006461d0: ldr      r3, [r0, #0x134]
006461d4: mov      r0, r3
006461d8: ldr      r3, [r3]
006461dc: mov      lr, pc
006461e0: ldr      pc, [r3, #0x10]
006461e4: pop      {r4, pc}

# __aeabi_f2uiz
008be2a0: lsls     r2, r0, #1
008be2a4: bhs      #0x8be2cc
008be2a8: cmp      r2, #0x7f000000
008be2ac: blo      #0x8be2cc
008be2b0: mov      r3, #0x9e
008be2b4: subs     r2, r3, r2, lsr #24
008be2b8: bmi      #0x8be2d4
008be2bc: lsl      r3, r0, #8
008be2c0: orr      r3, r3, #0x80000000
008be2c4: lsr      r0, r3, r2
008be2c8: bx       lr
008be2cc: mov      r0, #0
008be2d0: bx       lr
008be2d4: cmn      r2, #0x61
008be2d8: bne      #0x8be2e4
008be2dc: lsls     r2, r0, #9
008be2e0: bne      #0x8be2ec
008be2e4: mvn      r0, #0
008be2e8: bx       lr
008be2ec: mov      r0, #0
008be2f0: bx       lr

# __fixunssfsi
008be2a0: lsls     r2, r0, #1
008be2a4: bhs      #0x8be2cc
008be2a8: cmp      r2, #0x7f000000
008be2ac: blo      #0x8be2cc
008be2b0: mov      r3, #0x9e
008be2b4: subs     r2, r3, r2, lsr #24
008be2b8: bmi      #0x8be2d4
008be2bc: lsl      r3, r0, #8
008be2c0: orr      r3, r3, #0x80000000
008be2c4: lsr      r0, r3, r2
008be2c8: bx       lr
008be2cc: mov      r0, #0
008be2d0: bx       lr
008be2d4: cmn      r2, #0x61
008be2d8: bne      #0x8be2e4
008be2dc: lsls     r2, r0, #9
008be2e0: bne      #0x8be2ec
008be2e4: mvn      r0, #0
008be2e8: bx       lr
008be2ec: mov      r0, #0
008be2f0: bx       lr

# _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiPv
0061a3fc: push     {r4, r5, r6, r7, r8, lr}
0061a400: mov      r4, r1
0061a404: mov      r1, #0
0061a408: mov      r6, r2
0061a40c: mov      r5, r0
0061a410: bl       #0x669e24
0061a414: ldr      r7, [r0, #4]
0061a418: mov      r0, r5
0061a41c: bl       #0x669e54
0061a420: cmp      r0, #0
0061a424: bne      #0x61a434
0061a428: ldrb     r3, [r7, r4]
0061a42c: strb     r3, [r6]
0061a430: pop      {r4, r5, r6, r7, r8, pc}
0061a434: mov      r0, r5
0061a438: bl       #0x669e68
0061a43c: cmp      r0, #0
0061a440: beq      #0x61a428
0061a444: mov      r0, r5
0061a448: bl       #0x669e68
0061a44c: ldrb     r2, [r0]
0061a450: mov      r3, r6
0061a454: strb     r2, [r3], #1
0061a458: ldrb     r2, [r0, #1]
0061a45c: strb     r2, [r6, #1]
0061a460: ldrb     r2, [r0, #2]
0061a464: strb     r2, [r3, #1]
0061a468: ldrb     r3, [r7, r4]
0061a46c: strb     r3, [r6, #3]
0061a470: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch5video24CMaterialRendererManager22createMaterialInstanceENS0_15E_MATERIAL_TYPEE
005d9af0: push     {r4, r5, r6, lr}
005d9af4: mov      r3, #0
005d9af8: mov      r5, r1
005d9afc: str      r3, [r0]
005d9b00: mov      r1, r2
005d9b04: sub      sp, sp, #0x10
005d9b08: mov      r4, r0
005d9b0c: mov      r0, r5
005d9b10: bl       #0x5d8b28
005d9b14: ldr      r2, [r5, #0x18]
005d9b18: ldr      r1, [r5, #0x1c]
005d9b1c: ldr      r3, [pc, #0xac]
005d9b20: rsb      r1, r2, r1
005d9b24: cmp      r0, r1, asr #3
005d9b28: add      r3, pc, r3
005d9b2c: addlo    r2, r2, r0, lsl #3
005d9b30: ldrhs    r2, [pc, #0x9c]
005d9b34: ldrhs    r2, [r3, r2]
005d9b38: ldr      r3, [r2]
005d9b3c: cmp      r3, #0
005d9b40: str      r3, [sp, #0xc]
005d9b44: beq      #0x5d9bc8
005d9b48: ldr      r2, [r3]
005d9b4c: add      r2, r2, #1
005d9b50: str      r2, [r3]
005d9b54: ldr      r3, [sp, #0xc]
005d9b58: cmp      r3, #0
005d9b5c: beq      #0x5d9bc8
005d9b60: mov      r2, #0
005d9b64: add      r6, sp, #8
005d9b68: add      r5, sp, #0xc
005d9b6c: mov      r3, r2
005d9b70: mov      r0, r6
005d9b74: mov      r1, r5
005d9b78: bl       #0x5cc0a0
005d9b7c: ldr      r3, [sp, #8]
005d9b80: add      r0, sp, #0x10
005d9b84: str      r3, [sp, #4]
005d9b88: cmp      r3, #0
005d9b8c: ldrne    r2, [r3]
005d9b90: addne    r2, r2, #1
005d9b94: strne    r2, [r3]
005d9b98: ldrne    r3, [sp, #4]
005d9b9c: ldr      r2, [r4]
005d9ba0: str      r3, [r4]
005d9ba4: str      r2, [r0, #-0xc]!
005d9ba8: bl       #0x310be8
005d9bac: mov      r0, r6
005d9bb0: bl       #0x310be8
005d9bb4: mov      r0, r5
005d9bb8: bl       #0x3522b8
005d9bbc: mov      r0, r4
005d9bc0: add      sp, sp, #0x10
005d9bc4: pop      {r4, r5, r6, pc}
005d9bc8: add      r5, sp, #0xc
005d9bcc: b        #0x5d9bb4
005d9bd0: eorseq   sl, fp, r8, ror #30
005d9bd4: ldrdeq   r3, r4, [r0], -ip

# _ZNK6glitch7collada14CRootSceneNode11hasMaterialEPKc
0065b538: push     {r4, r5, r6, r7, r8, lr}
0065b53c: mov      r5, r1
0065b540: ldr      r4, [r5, #0x178]!
0065b544: mov      r6, r0
0065b548: mov      r7, r2
0065b54c: cmp      r4, r5
0065b550: bne      #0x65b564
0065b554: b        #0x65b5a0
0065b558: ldr      r4, [r4]
0065b55c: cmp      r5, r4
0065b560: beq      #0x65b5a0
0065b564: ldr      r3, [r4, #8]
0065b568: mov      r1, r7
0065b56c: ldr      r0, [r3]
0065b570: bl       #0x30e31c
0065b574: cmp      r0, #0
0065b578: bne      #0x65b558
0065b57c: ldr      r3, [r4, #0xc]
0065b580: cmp      r3, #0
0065b584: str      r3, [r6]
0065b588: beq      #0x65b5a8
0065b58c: ldr      r2, [r3]
0065b590: mov      r0, r6
0065b594: add      r2, r2, #1
0065b598: str      r2, [r3]
0065b59c: pop      {r4, r5, r6, r7, r8, pc}
0065b5a0: mov      r3, #0
0065b5a4: str      r3, [r6]
0065b5a8: mov      r0, r6
0065b5ac: pop      {r4, r5, r6, r7, r8, pc}

# _ZN6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE11getInstanceEv
00611924: push     {r4, r5, r6, lr}
00611928: ldr      r4, [pc, #0x70]
0061192c: ldr      r3, [pc, #0x70]
00611930: add      r4, pc, r4
00611934: ldr      r6, [r4, r3]
00611938: ldr      r3, [r6]
0061193c: tst      r3, #1
00611940: beq      #0x611950
00611944: ldr      r5, [pc, #0x5c]
00611948: ldr      r0, [r4, r5]
0061194c: pop      {r4, r5, r6, pc}
00611950: mov      r0, r6
00611954: bl       #0x30e76c
00611958: cmp      r0, #0
0061195c: beq      #0x611944
00611960: ldr      r3, [pc, #0x44]
00611964: ldr      r5, [pc, #0x3c]
00611968: mov      r0, r6
0061196c: ldr      r3, [r4, r3]
00611970: ldr      r6, [r4, r5]
00611974: add      r3, r3, #8
00611978: str      r3, [r6]
0061197c: bl       #0x30ea3c
00611980: ldr      r3, [pc, #0x28]
00611984: mov      r0, r6
00611988: ldr      r1, [r4, r3]
0061198c: ldr      r3, [pc, #0x20]
00611990: ldr      r2, [r4, r3]
00611994: bl       #0x30e304
00611998: ldr      r0, [r4, r5]
0061199c: pop      {r4, r5, r6, pc}
006119a0: eorseq   r3, r8, r0, ror #2
006119a4: andeq    r2, r0, r4, asr r5
006119a8: strdeq   r4, r5, [r0], -r4
006119ac: andeq    r3, r0, r4, lsl #28
006119b0: strdeq   r2, r3, [r0], -r4
006119b4: muleq    r0, r0, r8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE17applyBlendedValueEPvPfiSF_PNS1_15CApplicatorInfoE
00625a28: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00625a2c: cmp      r3, #1
00625a30: sub      sp, sp, #0x24
00625a34: mov      r4, r3
00625a38: stm      sp, {r1, r2}
00625a3c: beq      #0x625b2c
00625a40: mov      r7, #0
00625a44: cmp      r3, #0
00625a48: str      r7, [sp, #0xc]
00625a4c: str      r7, [sp, #0x10]
00625a50: str      r7, [sp, #0x14]
00625a54: str      r7, [sp, #0x18]
00625a58: movne    fp, #0
00625a5c: addne    r8, sp, #0xc
00625a60: beq      #0x625b5c
00625a64: ldr      r2, [sp, #4]
00625a68: ldr      r3, [sp]
00625a6c: mov      r5, #0
00625a70: ldr      sl, [r2, fp]
00625a74: add      sb, r3, fp
00625a78: mov      r6, r5
00625a7c: ldrb     r0, [sb, r6]
00625a80: bl       #0x30e964
00625a84: mov      r1, sl
00625a88: bl       #0x30ed6c
00625a8c: mov      r1, r7
00625a90: bl       #0x30eba4
00625a94: str      r0, [r8, r5]
00625a98: add      r5, r5, #4
00625a9c: cmp      r5, #0x10
00625aa0: add      r6, r6, #1
00625aa4: ldrne    r7, [r8, r5]
00625aa8: bne      #0x625a7c
00625aac: subs     r4, r4, #1
00625ab0: add      fp, fp, #4
00625ab4: ldrne    r7, [sp, #0xc]
00625ab8: bne      #0x625a64
00625abc: ldr      r0, [sp, #0xc]
00625ac0: bl       #0x8be2a0
00625ac4: strb     r0, [sp, #0x1c]
00625ac8: ldr      r0, [sp, #0x10]
00625acc: bl       #0x8be2a0
00625ad0: strb     r0, [sp, #0x1d]
00625ad4: ldr      r0, [sp, #0x14]
00625ad8: bl       #0x8be2a0
00625adc: strb     r0, [sp, #0x1e]
00625ae0: ldr      r0, [sp, #0x18]
00625ae4: bl       #0x8be2a0
00625ae8: strb     r0, [sp, #0x1f]
00625aec: ldr      r3, [sp, #0x4c]
00625af0: ldrb     r5, [sp, #0x1f]
00625af4: ldrb     r4, [sp, #0x1c]
00625af8: ldrb     lr, [sp, #0x1d]
00625afc: ldrb     ip, [sp, #0x1e]
00625b00: ldrh     r1, [r3, #8]
00625b04: ldr      r0, [sp, #0x48]
00625b08: mov      r3, r8
00625b0c: mov      r2, #0
00625b10: strb     r5, [sp, #0xf]
00625b14: strb     r4, [sp, #0xc]
00625b18: strb     lr, [sp, #0xd]
00625b1c: strb     ip, [sp, #0xe]
00625b20: bl       #0x5cad38
00625b24: add      sp, sp, #0x24
00625b28: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00625b2c: ldr      r3, [sp]
00625b30: ldr      r2, [sp]
00625b34: add      r8, sp, #0xc
00625b38: ldrb     r0, [r3], #1
00625b3c: ldrb     r1, [r2, #1]
00625b40: ldrb     r2, [r3, #2]
00625b44: ldrb     r3, [r3, #1]
00625b48: strb     r0, [sp, #0x1c]
00625b4c: strb     r1, [sp, #0x1d]
00625b50: strb     r3, [sp, #0x1e]
00625b54: strb     r2, [sp, #0x1f]
00625b58: b        #0x625aec
00625b5c: mov      r0, r7
00625b60: add      r8, sp, #0xc
00625b64: b        #0x625ac0

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE10applyValueEPvSF_PNS1_15CApplicatorInfoE
006229d0: str      lr, [sp, #-4]!
006229d4: ldrb     r3, [r1, #2]
006229d8: ldrb     ip, [r1, #3]
006229dc: ldrb     r0, [r1]
006229e0: ldrb     r1, [r1, #1]
006229e4: sub      sp, sp, #0xc
006229e8: strb     r0, [sp, #4]
006229ec: strb     r3, [sp, #6]
006229f0: strb     ip, [sp, #7]
006229f4: mov      r3, #0
006229f8: strb     r1, [sp, #5]
006229fc: ldrh     r1, [r3, #8]
00622a00: mov      r0, r2
00622a04: mov      r2, r3
00622a08: add      r3, sp, #4
00622a0c: bl       #0x5cad38
00622a10: add      sp, sp, #0xc
00622a14: ldm      sp!, {pc}

# _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiifPv
0061a484: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061a488: mov      r4, r1
0061a48c: mov      r1, #0
0061a490: mov      r5, r2
0061a494: mov      r8, r3
0061a498: mov      r7, r0
0061a49c: ldr      r6, [sp, #0x20]
0061a4a0: bl       #0x669e24
0061a4a4: ldr      sb, [r0, #4]
0061a4a8: mov      r0, r7
0061a4ac: bl       #0x669e54
0061a4b0: cmp      r0, #0
0061a4b4: beq      #0x61a518
0061a4b8: mov      sl, #0
0061a4bc: mov      r0, r7
0061a4c0: bl       #0x669e68
0061a4c4: ldrb     r3, [r0, sl]
0061a4c8: strb     r3, [r6, sl]
0061a4cc: add      sl, sl, #1
0061a4d0: cmp      sl, #3
0061a4d4: bne      #0x61a4bc
0061a4d8: ldrb     r7, [sb, r4]
0061a4dc: mov      r0, r7
0061a4e0: bl       #0x30e964
0061a4e4: mov      r4, r0
0061a4e8: ldrb     r0, [sb, r5]
0061a4ec: rsb      r0, r7, r0
0061a4f0: bl       #0x30e964
0061a4f4: mov      r1, r0
0061a4f8: mov      r0, r8
0061a4fc: bl       #0x30ed6c
0061a500: mov      r1, r0
0061a504: mov      r0, r4
0061a508: bl       #0x30eba4
0061a50c: bl       #0x8be2a0
0061a510: strb     r0, [r6, #3]
0061a514: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0061a518: ldrb     r7, [sb, r4]
0061a51c: mov      r0, r7
0061a520: bl       #0x30e964
0061a524: mov      r4, r0
0061a528: ldrb     r0, [sb, r5]
0061a52c: rsb      r0, r7, r0
0061a530: bl       #0x30e964
0061a534: mov      r1, r0
0061a538: mov      r0, r8
0061a53c: bl       #0x30ed6c
0061a540: mov      r1, r0
0061a544: mov      r0, r4
0061a548: bl       #0x30eba4
0061a54c: bl       #0x8be2a0
0061a550: strb     r0, [r6]
0061a554: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _Z24SetNodeToSelfIlluminatedPN6glitch5scene10ISceneNodeE
0050e398: push     {r4, r5, r6, lr}
0050e39c: mov      r5, r0
0050e3a0: ldr      r3, [r0]
0050e3a4: mov      lr, pc
0050e3a8: ldr      pc, [r3, #0x88]
0050e3ac: ldr      r4, [r5, #0xf4]!
0050e3b0: cmp      r4, r5
0050e3b4: beq      #0x50e3d4
0050e3b8: cmp      r4, #0
0050e3bc: moveq    r0, r4
0050e3c0: subne    r0, r4, #4
0050e3c4: bl       #0x50e398
0050e3c8: ldr      r4, [r4]
0050e3cc: cmp      r5, r4
0050e3d0: bne      #0x50e3b8
0050e3d4: pop      {r4, r5, r6, pc}

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE18applyKeyBasedValueERKNS0_18SAnimationAccessorEiPvPNS1_15CApplicatorInfoE
00622a18: push     {r4, r5, r6, lr}
00622a1c: sub      sp, sp, #8
00622a20: mov      r0, r1
00622a24: mov      r1, r2
00622a28: add      r2, sp, #4
00622a2c: mov      r6, r3
00622a30: bl       #0x61a3fc
00622a34: ldr      r3, [sp, #0x18]
00622a38: ldrb     r5, [sp, #7]
00622a3c: ldrb     r4, [sp, #4]
00622a40: ldrb     lr, [sp, #5]
00622a44: ldrb     ip, [sp, #6]
00622a48: ldrh     r1, [r3, #8]
00622a4c: mov      r0, r6
00622a50: mov      r2, #0
00622a54: mov      r3, sp
00622a58: strb     r5, [sp, #3]
00622a5c: strb     r4, [sp]
00622a60: strb     lr, [sp, #1]
00622a64: strb     ip, [sp, #2]
00622a68: bl       #0x5cad38
00622a6c: add      sp, sp, #8
00622a70: pop      {r4, r5, r6, pc}

# _ZN6glitch7collada21CSceneNodeAnimatorSet20applyAnimationValuesEj
0065f418: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0065f41c: ldr      r3, [r0, #0x24]
0065f420: sub      sp, sp, #0x44
0065f424: mov      r4, r0
0065f428: ldr      r3, [r3, #0x3c]
0065f42c: mov      r5, r1
0065f430: cmp      r3, #0
0065f434: bne      #0x65f444
0065f438: ldr      r3, [r0, #0x18]
0065f43c: cmp      r3, #0
0065f440: beq      #0x65f5c0
0065f444: mov      r0, r4
0065f448: mov      r1, r5
0065f44c: bl       #0x667c48
0065f450: ldr      r3, [r4]
0065f454: mov      r0, r4
0065f458: mov      lr, pc
0065f45c: ldr      pc, [r3, #0x44]
0065f460: cmp      r0, #0
0065f464: beq      #0x65f5c8
0065f468: ldr      r0, [r0, #4]
0065f46c: str      r0, [sp, #0xc]
0065f470: ldr      r3, [r4, #0xc]
0065f474: ldr      r1, [r4, #0x50]
0065f478: ldr      r0, [r4, #0x24]
0065f47c: subs     r3, r3, #1
0065f480: movne    r3, #1
0065f484: str      r3, [sp, #0x14]
0065f488: bl       #0x65f0b4
0065f48c: ldr      r3, [r0]
0065f490: ldr      r1, [sp, #0xc]
0065f494: mov      r0, r4
0065f498: ldr      r3, [r3, #0x24]
0065f49c: ldr      r3, [r3, #0x20]
0065f4a0: ldr      r5, [r3, #0x14]
0065f4a4: bl       #0x65f364
0065f4a8: str      r0, [sp, #0x10]
0065f4ac: ldr      r3, [r4, #0x24]
0065f4b0: subs     r5, r5, #0
0065f4b4: movne    r5, #1
0065f4b8: strb     r5, [sp, #0x31]
0065f4bc: ldr      fp, [r3, #0x3c]
0065f4c0: cmp      fp, #0
0065f4c4: beq      #0x65f5c0
0065f4c8: add      r1, sp, #0x24
0065f4cc: add      r2, sp, #0x34
0065f4d0: mov      r5, #0
0065f4d4: str      r1, [sp, #0x18]
0065f4d8: str      r2, [sp, #0x1c]
0065f4dc: b        #0x65f4ec
0065f4e0: add      r5, r5, #1
0065f4e4: cmp      r5, fp
0065f4e8: beq      #0x65f5c0
0065f4ec: mov      r1, r5
0065f4f0: ldr      r3, [r4]
0065f4f4: mov      r0, r4
0065f4f8: mov      lr, pc
0065f4fc: ldr      pc, [r3, #0x80]
0065f500: cmp      r0, #0
0065f504: beq      #0x65f4e0
0065f508: ldr      r3, [r4, #0x28]
0065f50c: lsl      r8, r5, #2
0065f510: ldr      r6, [r3, r5, lsl #2]
0065f514: cmp      r6, #0
0065f518: mov      r2, r6
0065f51c: beq      #0x65f4e0
0065f520: ldr      sb, [r4, #0x4c]
0065f524: ldr      r3, [r4, #0x24]
0065f528: mov      ip, #0xc
0065f52c: add      sb, r5, sb
0065f530: mul      sb, ip, sb
0065f534: ldr      sl, [r3, #0x30]
0065f538: add      r7, sl, sb
0065f53c: ldr      r1, [r7, #4]
0065f540: cmp      r1, #0
0065f544: beq      #0x65f568
0065f548: ldr      r0, [r3, #0x18]
0065f54c: ldr      r3, [r4, #0x34]
0065f550: ldr      ip, [r0, r8]
0065f554: ldr      r3, [r3, r8]
0065f558: mov      r0, ip
0065f55c: ldr      ip, [ip]
0065f560: mov      lr, pc
0065f564: ldr      pc, [ip, #0x70]
0065f568: ldr      r3, [sl, sb]
0065f56c: cmp      r3, #2
0065f570: bne      #0x65f4e0
0065f574: ldr      r2, [r7, #8]
0065f578: ldr      r3, [r4, #0x34]
0065f57c: ldr      r1, [sp, #0x10]
0065f580: str      r2, [sp, #0x34]
0065f584: ldr      r2, [sp, #0x18]
0065f588: str      r1, [sp, #0x38]
0065f58c: ldr      ip, [sp, #0x14]
0065f590: str      r2, [sp, #0x3c]
0065f594: ldr      r1, [r4, #0x40]
0065f598: ldr      r3, [r3, r8]
0065f59c: mov      r2, r6
0065f5a0: add      r8, r1, r8
0065f5a4: ldr      r0, [sp, #0x1c]
0065f5a8: ldr      r1, [sp, #0xc]
0065f5ac: add      r5, r5, #1
0065f5b0: stm      sp, {r8, ip}
0065f5b4: bl       #0x66a0c0
0065f5b8: cmp      r5, fp
0065f5bc: bne      #0x65f4ec
0065f5c0: add      sp, sp, #0x44
0065f5c4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0065f5c8: ldr      r1, [r4, #0x14]
0065f5cc: mov      r0, r5
0065f5d0: bl       #0x30eb2c
0065f5d4: str      r1, [sp, #0xc]
0065f5d8: b        #0x65f470

# _ZN6glitch7collada16CColladaDatabase19getAnimationTrackExEPKNS0_10SAnimationE
00611ae0: ldr      r3, [pc, #0x61c]
00611ae4: push     {r4, r5, r6, lr}
00611ae8: subs     r4, r0, #0
00611aec: add      r3, pc, r3
00611af0: beq      #0x611c80
00611af4: ldr      r2, [r4, #0x10]
00611af8: ldr      r2, [r2, #8]
00611afc: sub      r2, r2, #1
00611b00: cmp      r2, #0x5a
00611b04: addls    pc, pc, r2, lsl #2
00611b08: b        #0x611c80
00611b0c: b        #0x611cc4
00611b10: b        #0x611ce8
00611b14: b        #0x611d0c
00611b18: b        #0x611d30
00611b1c: b        #0x611d54
00611b20: b        #0x611d78
00611b24: b        #0x611d78
00611b28: b        #0x611d78
00611b2c: b        #0x611d78
00611b30: b        #0x611d9c
00611b34: b        #0x611dc0
00611b38: b        #0x611de4
00611b3c: b        #0x611e08
00611b40: b        #0x611e2c
00611b44: b        #0x611c80
00611b48: b        #0x611e44
00611b4c: b        #0x611c80
00611b50: b        #0x611c80
00611b54: b        #0x611c80
00611b58: b        #0x611e4c
00611b5c: b        #0x611c80
00611b60: b        #0x611c80
00611b64: b        #0x611c80
00611b68: b        #0x611c80
00611b6c: b        #0x611c80
00611b70: b        #0x611c80
00611b74: b        #0x611c80
00611b78: b        #0x611e58
00611b7c: b        #0x611e58
00611b80: b        #0x611e58
00611b84: b        #0x611e58
00611b88: b        #0x611e58
00611b8c: b        #0x611e64
00611b90: b        #0x611e58
00611b94: b        #0x611e58
00611b98: b        #0x611e58
00611b9c: b        #0x611e58
00611ba0: b        #0x611e58
00611ba4: b        #0x611e64
00611ba8: b        #0x611e58
00611bac: b        #0x611e58
00611bb0: b        #0x611e58
00611bb4: b        #0x611e58
00611bb8: b        #0x611e58
00611bbc: b        #0x611e58
00611bc0: b        #0x611e58
00611bc4: b        #0x611e58
00611bc8: b        #0x611e58
00611bcc: b        #0x611e58
00611bd0: b        #0x611e58
00611bd4: b        #0x611e58
00611bd8: b        #0x611e58
00611bdc: b        #0x611e58
00611be0: b        #0x611e58
00611be4: b        #0x611e58
00611be8: b        #0x611e58
00611bec: b        #0x611e64
00611bf0: b        #0x611e64
00611bf4: b        #0x611e58
00611bf8: b        #0x611e58
00611bfc: b        #0x611e58
00611c00: b        #0x611e58
00611c04: b        #0x611e58
00611c08: b        #0x611e58
00611c0c: b        #0x611e58
00611c10: b        #0x611e58
00611c14: b        #0x611e64
00611c18: b        #0x611e58
00611c1c: b        #0x611e58
00611c20: b        #0x611e58
00611c24: b        #0x611c80
00611c28: b        #0x611c80
00611c2c: b        #0x611c80
00611c30: b        #0x611c80
00611c34: b        #0x611c80
00611c38: b        #0x611c80
00611c3c: b        #0x611c80
00611c40: b        #0x611c80
00611c44: b        #0x611c80
00611c48: b        #0x611c80
00611c4c: b        #0x611c80
00611c50: b        #0x611c80
00611c54: b        #0x611c80
00611c58: b        #0x611c80
00611c5c: b        #0x611c80
00611c60: b        #0x611c88
00611c64: b        #0x611e38
00611c68: b        #0x611e38
00611c6c: b        #0x611e38
00611c70: b        #0x611e38
00611c74: b        #0x611e38
00611c78: cmp      r3, #2
00611c7c: beq      #0x611f6c
00611c80: mov      r0, #0
00611c84: pop      {r4, r5, r6, pc}
00611c88: ldr      r2, [r4, #8]
00611c8c: ldr      r3, [r2, #0x10]
00611c90: cmp      r3, #1
00611c94: beq      #0x611e70
00611c98: cmp      r3, #6
00611c9c: bne      #0x611c80
00611ca0: ldr      r3, [r2, #0x14]
00611ca4: sub      r3, r3, #1
00611ca8: cmp      r3, #3
00611cac: addls    pc, pc, r3, lsl #2
00611cb0: b        #0x611c80
00611cb4: b        #0x611f9c
00611cb8: b        #0x611f94
00611cbc: b        #0x611f8c
00611cc0: b        #0x611f84
00611cc4: ldr      r3, [r4, #0x1c]
00611cc8: cmp      r3, #0
00611ccc: beq      #0x611ef4
00611cd0: ldr      r3, [r3]
00611cd4: cmp      r3, #1
00611cd8: beq      #0x612034
00611cdc: bhs      #0x611eec
00611ce0: pop      {r4, r5, r6, lr}
00611ce4: b        #0x6103c0
00611ce8: ldr      r3, [r4, #0x1c]
00611cec: cmp      r3, #0
00611cf0: beq      #0x611f54
00611cf4: ldr      r3, [r3]
00611cf8: cmp      r3, #1
00611cfc: beq      #0x612024
00611d00: bhs      #0x611f4c
00611d04: pop      {r4, r5, r6, lr}
00611d08: b        #0x61057c
00611d0c: ldr      r3, [r4, #0x1c]
00611d10: cmp      r3, #0
00611d14: beq      #0x611f04
00611d18: ldr      r3, [r3]
00611d1c: cmp      r3, #1
00611d20: beq      #0x61201c
00611d24: bhs      #0x611efc
00611d28: pop      {r4, r5, r6, lr}
00611d2c: b        #0x610738
00611d30: ldr      r3, [r4, #0x1c]
00611d34: cmp      r3, #0
00611d38: beq      #0x611f64
00611d3c: ldr      r3, [r3]
00611d40: cmp      r3, #1
00611d44: beq      #0x61204c
00611d48: bhs      #0x611f5c
00611d4c: pop      {r4, r5, r6, lr}
00611d50: b        #0x6108f4
00611d54: ldr      r3, [r4, #0x1c]
00611d58: cmp      r3, #0
00611d5c: beq      #0x611f6c
00611d60: ldr      r3, [r3]
00611d64: cmp      r3, #1
00611d68: beq      #0x612064
00611d6c: bhs      #0x611c78
00611d70: pop      {r4, r5, r6, lr}
00611d74: b        #0x610048
00611d78: ldr      r3, [r4, #0x1c]
00611d7c: cmp      r3, #0
00611d80: beq      #0x611f44
00611d84: ldr      r3, [r3]
00611d88: cmp      r3, #1
00611d8c: beq      #0x61205c
00611d90: bhs      #0x611f3c
00611d94: pop      {r4, r5, r6, lr}
00611d98: b        #0x610204
00611d9c: ldr      r3, [r4, #0x1c]
00611da0: cmp      r3, #0
00611da4: beq      #0x611f34
00611da8: ldr      r3, [r3]
00611dac: cmp      r3, #1
00611db0: beq      #0x612054
00611db4: bhs      #0x611f2c
00611db8: pop      {r4, r5, r6, lr}
00611dbc: b        #0x610ab0
00611dc0: ldr      r3, [r4, #0x1c]
00611dc4: cmp      r3, #0
00611dc8: beq      #0x611f24
00611dcc: ldr      r3, [r3]
00611dd0: cmp      r3, #1
00611dd4: beq      #0x61202c
00611dd8: bhs      #0x611f1c
00611ddc: pop      {r4, r5, r6, lr}
00611de0: b        #0x610c6c
00611de4: ldr      r3, [r4, #0x1c]
00611de8: cmp      r3, #0
00611dec: beq      #0x611f14
00611df0: ldr      r3, [r3]
00611df4: cmp      r3, #1
00611df8: beq      #0x612044
00611dfc: bhs      #0x611f0c
00611e00: pop      {r4, r5, r6, lr}
00611e04: b        #0x610e28
00611e08: ldr      r3, [r4, #0x1c]
00611e0c: cmp      r3, #0
00611e10: beq      #0x611f7c
00611e14: ldr      r3, [r3]
00611e18: cmp      r3, #1
00611e1c: beq      #0x61203c
00611e20: bhs      #0x611f74
00611e24: pop      {r4, r5, r6, lr}
00611e28: b        #0x610fe4
00611e2c: ldr      r2, [pc, #0x2d4]
00611e30: ldr      r0, [r3, r2]
00611e34: pop      {r4, r5, r6, pc}
00611e38: ldr      r2, [pc, #0x2cc]
00611e3c: ldr      r0, [r3, r2]
00611e40: pop      {r4, r5, r6, pc}
00611e44: pop      {r4, r5, r6, lr}
00611e48: b        #0x611078
00611e4c: ldr      r2, [pc, #0x2bc]
00611e50: ldr      r0, [r3, r2]
00611e54: pop      {r4, r5, r6, pc}
00611e58: ldr      r2, [pc, #0x2b4]
00611e5c: ldr      r0, [r3, r2]
00611e60: pop      {r4, r5, r6, pc}
00611e64: ldr      r2, [pc, #0x2ac]
00611e68: ldr      r0, [r3, r2]
00611e6c: pop      {r4, r5, r6, pc}
00611e70: ldr      r3, [r2, #0x14]
00611e74: cmp      r3, #3
00611e78: beq      #0x61206c
00611e7c: cmp      r3, #4
00611e80: beq      #0x611ff4
00611e84: cmp      r3, #1
00611e88: bne      #0x611c80
00611e8c: ldr      r3, [r4, #0x18]
00611e90: ldr      r2, [r3, #4]
00611e94: cmp      r2, #1
00611e98: ble      #0x611c80
00611e9c: ldr      r3, [r3]
00611ea0: sub      r3, r3, #1
00611ea4: cmp      r3, #0xe
00611ea8: addls    pc, pc, r3, lsl #2
00611eac: b        #0x611c80
00611eb0: b        #0x612004
00611eb4: b        #0x611ffc
00611eb8: b        #0x611c80
00611ebc: b        #0x612014
00611ec0: b        #0x611c80
00611ec4: b        #0x611c80
00611ec8: b        #0x611c80
00611ecc: b        #0x61200c
00611ed0: b        #0x611c80
00611ed4: b        #0x611c80
00611ed8: b        #0x611c80
00611edc: b        #0x611c80
00611ee0: b        #0x611c80
00611ee4: b        #0x611c80
00611ee8: b        #0x611ff4
00611eec: cmp      r3, #2
00611ef0: bne      #0x611c80
00611ef4: pop      {r4, r5, r6, lr}
00611ef8: b        #0x610298
00611efc: cmp      r3, #2
00611f00: bne      #0x611c80
00611f04: pop      {r4, r5, r6, lr}
00611f08: b        #0x610610
00611f0c: cmp      r3, #2
00611f10: bne      #0x611c80
00611f14: pop      {r4, r5, r6, lr}
00611f18: b        #0x610d00
00611f1c: cmp      r3, #2
00611f20: bne      #0x611c80
00611f24: pop      {r4, r5, r6, lr}
00611f28: b        #0x610b44
00611f2c: cmp      r3, #2
00611f30: bne      #0x611c80
00611f34: pop      {r4, r5, r6, lr}
00611f38: b        #0x610988
00611f3c: cmp      r3, #2
00611f40: bne      #0x611c80
00611f44: pop      {r4, r5, r6, lr}
00611f48: b        #0x6100dc
00611f4c: cmp      r3, #2
00611f50: bne      #0x611c80
00611f54: pop      {r4, r5, r6, lr}
00611f58: b        #0x610454
00611f5c: cmp      r3, #2
00611f60: bne      #0x611c80
00611f64: pop      {r4, r5, r6, lr}
00611f68: b        #0x6107cc
00611f6c: pop      {r4, r5, r6, lr}
00611f70: b        #0x60ff20
00611f74: cmp      r3, #2
00611f78: bne      #0x611c80
00611f7c: pop      {r4, r5, r6, lr}
00611f80: b        #0x610ebc
00611f84: pop      {r4, r5, r6, lr}
00611f88: b        #0x6116d4
00611f8c: pop      {r4, r5, r6, lr}
00611f90: b        #0x611640
00611f94: pop      {r4, r5, r6, lr}
00611f98: b        #0x6115ac
00611f9c: ldr      r5, [pc, #0x178]
00611fa0: add      r5, pc, r5
00611fa4: ldr      r3, [r5, #0xc]
00611fa8: tst      r3, #1
00611fac: beq      #0x612074
00611fb0: ldr      r3, [r4, #0x18]
00611fb4: ldm      r3, {r1, r2}
00611fb8: sub      r3, r1, #1
00611fbc: cmp      r3, #7
00611fc0: movhi    r1, #0
00611fc4: bhi      #0x611fd4
00611fc8: ldr      r1, [pc, #0x150]
00611fcc: add      r1, pc, r1
00611fd0: ldr      r1, [r1, r3, lsl #2]
00611fd4: ldr      r3, [pc, #0x148]
00611fd8: sub      r2, r2, #1
00611fdc: add      r2, r2, r2, lsl #2
00611fe0: add      r2, r2, r1
00611fe4: add      r3, pc, r3
00611fe8: add      r3, r3, r2, lsl #2
00611fec: ldr      r0, [r3, #0x10]
00611ff0: pop      {r4, r5, r6, pc}
00611ff4: pop      {r4, r5, r6, lr}
00611ff8: b        #0x611a4c
00611ffc: pop      {r4, r5, r6, lr}
00612000: b        #0x6117fc
00612004: pop      {r4, r5, r6, lr}
00612008: b        #0x611768
0061200c: pop      {r4, r5, r6, lr}
00612010: b        #0x611924
00612014: pop      {r4, r5, r6, lr}
00612018: b        #0x611890
0061201c: pop      {r4, r5, r6, lr}
00612020: b        #0x6106a4
00612024: pop      {r4, r5, r6, lr}
00612028: b        #0x6104e8
0061202c: pop      {r4, r5, r6, lr}
00612030: b        #0x610bd8
00612034: pop      {r4, r5, r6, lr}
00612038: b        #0x61032c
0061203c: pop      {r4, r5, r6, lr}
00612040: b        #0x610f50
00612044: pop      {r4, r5, r6, lr}
00612048: b        #0x610d94
0061204c: pop      {r4, r5, r6, lr}
00612050: b        #0x610860
00612054: pop      {r4, r5, r6, lr}
00612058: b        #0x610a1c
0061205c: pop      {r4, r5, r6, lr}
00612060: b        #0x610170
00612064: pop      {r4, r5, r6, lr}
00612068: b        #0x60ffb4
0061206c: pop      {r4, r5, r6, lr}
00612070: b        #0x6119b8
00612074: add      r6, r5, #0xc
00612078: mov      r0, r6
0061207c: bl       #0x30e76c
00612080: cmp      r0, #0
00612084: beq      #0x611fb0
00612088: bl       #0x61110c
0061208c: str      r0, [r5, #0x10]
00612090: bl       #0x61110c
00612094: str      r0, [r5, #0x14]
00612098: bl       #0x6115ac
0061209c: str      r0, [r5, #0x24]
006120a0: bl       #0x6111a0
006120a4: str      r0, [r5, #0x28]
006120a8: bl       #0x611234
006120ac: str      r0, [r5, #0x2c]
006120b0: bl       #0x611640
006120b4: str      r0, [r5, #0x38]
006120b8: bl       #0x6112c8
006120bc: str      r0, [r5, #0x3c]
006120c0: bl       #0x6112c8
006120c4: str      r0, [r5, #0x40]
006120c8: bl       #0x6112c8
006120cc: str      r0, [r5, #0x44]
006120d0: bl       #0x6116d4
006120d4: str      r0, [r5, #0x4c]
006120d8: bl       #0x61135c
006120dc: str      r0, [r5, #0x50]
006120e0: bl       #0x6113f0
006120e4: str      r0, [r5, #0x54]
006120e8: bl       #0x611484
006120ec: str      r0, [r5, #0x58]
006120f0: bl       #0x611518
006120f4: str      r0, [r5, #0x5c]
006120f8: mov      r0, r6
006120fc: bl       #0x30ea3c
00612100: b        #0x611fb0
00612104: eorseq   r2, r8, r4, lsr #31
00612108: andeq    r0, r0, r4, lsr #30
0061210c: andeq    r2, r0, r4, lsl #27
00612110: andeq    r4, r0, r0, lsl #1
00612114: strheq   r2, [r0], -r0
00612118: ldrdeq   r4, r5, [r0], -r0
0061211c: eorseq   r4, lr, r4, ror sp
00612120: mlaeq    sp, r8, sp, r2
00612124: eorseq   r4, lr, r0, lsr sp

# _ZN6glitch7collada15CColladaFactory14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverEPNS0_9SMaterialEPNS0_14CRootSceneNodeE
006323d0: push     {r4, r5, r6, r7, r8, sb, sl, lr}
006323d4: sub      sp, sp, #0x20
006323d8: ldr      r4, [sp, #0x44]
006323dc: mov      r7, r1
006323e0: mov      r8, r2
006323e4: cmp      r4, #0
006323e8: mov      sl, r3
006323ec: mov      r5, r0
006323f0: ldr      r6, [sp, #0x40]
006323f4: beq      #0x63241c
006323f8: mov      r1, r4
006323fc: ldr      r2, [r6]
00632400: bl       #0x65b538
00632404: ldr      r3, [r5]
00632408: cmp      r3, #0
0063240c: beq      #0x632420
00632410: mov      r0, r5
00632414: add      sp, sp, #0x20
00632418: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0063241c: str      r4, [r0]
00632420: ldr      r2, [r6, #0xc]
00632424: ldr      r1, [r6, #0x18]
00632428: ldr      r3, [r6, #8]
0063242c: add      r2, r2, #1
00632430: stm      sp, {r1, r2, r3, r4}
00632434: add      sb, sp, #0x1c
00632438: mov      r3, sl
0063243c: mov      r1, r7
00632440: ldr      ip, [r7]
00632444: mov      r0, sb
00632448: mov      r2, r8
0063244c: mov      lr, pc
00632450: ldr      pc, [ip, #0x1c]
00632454: ldr      r3, [sp, #0x1c]
00632458: cmp      r3, #0
0063245c: beq      #0x6324b8
00632460: add      r7, sp, #0x18
00632464: mov      r2, sl
00632468: mov      r1, r8
0063246c: mov      r0, r7
00632470: mov      r3, sb
00632474: str      r6, [sp]
00632478: str      r4, [sp, #4]
0063247c: bl       #0x631ce8
00632480: ldr      r3, [sp, #0x18]
00632484: add      r0, sp, #0x20
00632488: str      r3, [sp, #0x14]
0063248c: cmp      r3, #0
00632490: ldrne    r2, [r3]
00632494: addne    r2, r2, #1
00632498: strne    r2, [r3]
0063249c: ldrne    r3, [sp, #0x14]
006324a0: ldr      r2, [r5]
006324a4: str      r3, [r5]
006324a8: str      r2, [r0, #-0xc]!
006324ac: bl       #0x310be8
006324b0: mov      r0, r7
006324b4: bl       #0x310be8
006324b8: mov      r0, sb
006324bc: bl       #0x3522b8
006324c0: b        #0x632410

# _ZNK6glitch7collada18SAnimationAccessor9getOutputEi
00669e24: ldm      r0, {r2, r3}
00669e28: mov      r0, #0x1c
00669e2c: ldr      r2, [r2, #8]
00669e30: mla      r2, r0, r1, r2
00669e34: ldr      r2, [r2, #0x18]
00669e38: add      r3, r3, r2, lsl #3
00669e3c: add      r0, r3, #4
00669e40: bx       lr

# _ZNK6glitch7collada15animation_track23CMaterialApplicatorInfo5cloneEv
00667ed0: push     {r4, r5, r6, lr}
00667ed4: mov      r1, #0
00667ed8: mov      r5, r0
00667edc: mov      r0, #0xc
00667ee0: bl       #0x5341ac
00667ee4: ldr      r4, [pc, #0x24]
00667ee8: ldr      r2, [pc, #0x24]
00667eec: add      r4, pc, r4
00667ef0: ldr      r2, [r4, r2]
00667ef4: add      r2, r2, #8
00667ef8: str      r2, [r0]
00667efc: ldr      r2, [r5, #4]
00667f00: str      r2, [r0, #4]
00667f04: ldr      r2, [r5, #8]
00667f08: str      r2, [r0, #8]
00667f0c: pop      {r4, r5, r6, pc}
00667f10: eorseq   ip, r2, r4, lsr #23
00667f14: andeq    r0, r0, r0, lsl #20

# _ZN6glitch7collada15animation_track13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS3_NS_5video6SColorEEEEELi3EhEEE20applyKeyBasedValueExERKNS0_18SAnimationAccessorEiifPvPNS1_15CApplicatorInfoE
00622a74: push     {r4, r5, lr}
00622a78: sub      sp, sp, #0x14
00622a7c: add      ip, sp, #0xc
00622a80: str      ip, [sp]
00622a84: bl       #0x61a484
00622a88: ldr      r3, [sp, #0x24]
00622a8c: ldrb     r5, [sp, #0xf]
00622a90: ldrb     r4, [sp, #0xc]
00622a94: ldrb     lr, [sp, #0xd]
00622a98: ldrb     ip, [sp, #0xe]
00622a9c: ldrh     r1, [r3, #8]
00622aa0: ldr      r0, [sp, #0x20]
00622aa4: mov      r2, #0
00622aa8: add      r3, sp, #8
00622aac: strb     r5, [sp, #0xb]
00622ab0: strb     r4, [sp, #8]
00622ab4: strb     lr, [sp, #9]
00622ab8: strb     ip, [sp, #0xa]
00622abc: bl       #0x5cad38
00622ac0: add      sp, sp, #0x14
00622ac4: pop      {r4, r5, pc}

# _ZN6glitch5scene10ISceneNodeC1EiRKNS_4core8vector3dIfEERKNS2_10quaternionES6_
00599268: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0059926c: ldr      r6, [pc, #0x1c0]
00599270: ldr      lr, [pc, #0x1c0]
00599274: ldr      ip, [pc, #0x1c0]
00599278: add      r6, pc, r6
0059927c: ldr      lr, [r6, lr]
00599280: ldr      ip, [r6, ip]
00599284: mov      sl, #1
00599288: ldr      r5, [lr, #0xc]
0059928c: add      ip, ip, #8
00599290: str      sl, [r0, #0x134]
00599294: str      r5, [r0]
00599298: str      ip, [r0, #0x130]
0059929c: ldr      ip, [r5, #-0xc]
005992a0: ldr      lr, [lr, #0x10]
005992a4: mov      r5, #0
005992a8: sub      sp, sp, #0xc
005992ac: str      lr, [r0, ip]
005992b0: str      r5, [r0, #4]
005992b4: str      r5, [r0, #8]
005992b8: mov      r4, r0
005992bc: mov      r7, r2
005992c0: str      r3, [sp]
005992c4: ldr      r8, [sp, #0x30]
005992c8: str      r1, [sp, #4]
005992cc: bl       #0x6a118c
005992d0: ldr      r2, [pc, #0x168]
005992d4: add      r1, r4, #0xc
005992d8: mov      r0, r1
005992dc: ldr      r2, [r6, r2]
005992e0: str      r1, [r4, #0x1c]
005992e4: str      r1, [r4, #0x20]
005992e8: add      r1, r2, #0x120
005992ec: add      r2, r2, #0x1c
005992f0: str      r2, [r4]
005992f4: str      r1, [r4, #0x130]
005992f8: bl       #0x598ee0
005992fc: ldr      r2, [r4, #0x1c]
00599300: mov      sb, #0x40
00599304: mov      r6, #0x3f800000
00599308: strb     r5, [r2]
0059930c: mov      r1, r5
00599310: mov      r2, sb
00599314: strb     r5, [r4, #0x64]
00599318: add      r0, r4, #0x24
0059931c: bl       #0x30e460
00599320: mov      r2, sb
00599324: mov      r1, r5
00599328: str      r6, [r4, #0x24]
0059932c: str      r6, [r4, #0x38]
00599330: str      r6, [r4, #0x4c]
00599334: str      r6, [r4, #0x60]
00599338: strb     sl, [r4, #0x64]
0059933c: strb     r5, [r4, #0xa8]
00599340: add      r0, r4, #0x68
00599344: bl       #0x30e460
00599348: str      r6, [r4, #0x68]
0059934c: str      r6, [r4, #0x7c]
00599350: str      r6, [r4, #0x90]
00599354: str      r6, [r4, #0xa4]
00599358: strb     sl, [r4, #0xa8]
0059935c: ldr      r2, [r7]
00599360: add      fp, r4, #0xb8
00599364: mov      ip, #0xbf000000
00599368: str      r2, [r4, #0xac]
0059936c: ldr      r2, [r7, #4]
00599370: add      ip, ip, #0x800000
00599374: add      lr, r4, #0xfc
00599378: str      r2, [r4, #0xb0]
0059937c: ldr      r2, [r7, #8]
00599380: add      sb, r4, #0xf4
00599384: add      r7, r4, #0x104
00599388: str      r2, [r4, #0xb4]
0059938c: ldr      r3, [sp]
00599390: ldm      r3, {r0, r1, r2, r3}
00599394: stm      fp, {r0, r1, r2, r3}
00599398: ldr      r3, [r8]
0059939c: mov      r0, r4
005993a0: mov      r1, r5
005993a4: str      r3, [r4, #0xc8]
005993a8: ldr      r3, [r8, #4]
005993ac: str      r3, [r4, #0xcc]
005993b0: ldr      r3, [r8, #8]
005993b4: str      ip, [r4, #0xdc]
005993b8: str      ip, [r4, #0xd4]
005993bc: str      r3, [r4, #0xd0]
005993c0: str      ip, [r4, #0xd8]
005993c4: str      r6, [r4, #0xe8]
005993c8: str      r6, [r4, #0xe0]
005993cc: str      r6, [r4, #0xe4]
005993d0: str      r5, [r4, #0xec]
005993d4: str      r5, [r4, #0xf0]
005993d8: str      sb, [r4, #0xf4]
005993dc: str      sb, [r4, #0xf8]
005993e0: str      lr, [r4, #0x100]
005993e4: str      r7, [r4, #0x108]
005993e8: ldr      r3, [sp, #4]
005993ec: strb     sl, [r4, #0x121]
005993f0: str      lr, [r4, #0xfc]
005993f4: str      r3, [r4, #0x10c]
005993f8: movw     r3, #0x60f
005993fc: str      r3, [r4, #0x11c]
00599400: mov      r3, #0
00599404: str      r3, [r4, #0x128]
00599408: str      r7, [r4, #0x104]
0059940c: str      r5, [r4, #0x110]
00599410: str      r5, [r4, #0x114]
00599414: str      r5, [r4, #0x118]
00599418: strb     sl, [r4, #0x120]
0059941c: str      r5, [r4, #0x124]
00599420: str      r5, [r4, #0x12c]
00599424: bl       #0x597c60
00599428: mov      r0, r4
0059942c: add      sp, sp, #0xc
00599430: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00599434: eorseq   fp, pc, r8, lsl r8
00599438: andeq    r4, r0, r4, lsr #23
0059943c: andeq    r2, r0, r4, asr #22
00599440: andeq    r2, r0, r0, lsl #3

# _ZN14ColladaFactory14createMaterialERKN6glitch7collada16CColladaDatabaseEPNS0_5video12IVideoDriverEPNS1_9SMaterialEPNS1_14CRootSceneNodeE
003506e0: push     {r4, lr}
003506e4: sub      sp, sp, #8
003506e8: ldr      ip, [sp, #0x10]
003506ec: mov      r4, r0
003506f0: str      ip, [sp]
003506f4: ldr      ip, [sp, #0x14]
003506f8: str      ip, [sp, #4]
003506fc: bl       #0x6323d0
00350700: mov      r0, r4
00350704: add      sp, sp, #8
00350708: pop      {r4, pc}

# _ZN6glitch7collada14CRootSceneNode11getMaterialEPKcPNS_5video12IVideoDriverE
0065ca30: push     {r4, r5, r6, r7, lr}
0065ca34: mov      r4, r0
0065ca38: sub      sp, sp, #0xc
0065ca3c: mov      r5, r3
0065ca40: mov      r7, r1
0065ca44: mov      r6, r2
0065ca48: bl       #0x65b538
0065ca4c: ldr      r3, [r4]
0065ca50: cmp      r3, #0
0065ca54: beq      #0x65ca64
0065ca58: mov      r0, r4
0065ca5c: add      sp, sp, #0xc
0065ca60: pop      {r4, r5, r6, r7, pc}
0065ca64: cmp      r5, #0
0065ca68: beq      #0x65ca58
0065ca6c: mov      r2, r6
0065ca70: mov      r3, r5
0065ca74: mov      r1, r7
0065ca78: add      r0, sp, #4
0065ca7c: bl       #0x65c808
0065ca80: ldr      r3, [sp, #4]
0065ca84: cmp      r3, #0
0065ca88: ldrne    r2, [r3]
0065ca8c: addne    r2, r2, #1
0065ca90: strne    r2, [r3]
0065ca94: ldr      r5, [r4]
0065ca98: str      r3, [r4]
0065ca9c: cmp      r5, #0
0065caa0: beq      #0x65cac8
0065caa4: ldr      r3, [r5]
0065caa8: sub      r3, r3, #1
0065caac: cmp      r3, #0
0065cab0: str      r3, [r5]
0065cab4: bne      #0x65cac8
0065cab8: mov      r0, r5
0065cabc: bl       #0x5cbf78
0065cac0: mov      r0, r5
0065cac4: bl       #0x30e2b0
0065cac8: ldr      r5, [sp, #4]
0065cacc: cmp      r5, #0
0065cad0: beq      #0x65ca58
0065cad4: ldr      r3, [r5]
0065cad8: sub      r3, r3, #1
0065cadc: cmp      r3, #0
0065cae0: str      r3, [r5]
0065cae4: bne      #0x65ca58
0065cae8: mov      r0, r5
0065caec: bl       #0x5cbf78
0065caf0: mov      r0, r5
0065caf4: bl       #0x30e2b0
0065caf8: b        #0x65ca58

# _ZN6glitch7collada15animation_track12CInterpreterINS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIA4_hNS_5video6SColorEEEEELi3EhEEhLi4ENS1_17SUseDefaultValuesILi3EhEEE18getKeyBasedValueExERKNS0_18SAnimationAccessorEiiifPv
00619978: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0061997c: mov      r4, r1
00619980: mov      r1, #0
00619984: mov      r5, r2
00619988: mov      sl, r3
0061998c: mov      r7, r0
00619990: ldr      r8, [sp, #0x20]
00619994: ldr      r6, [sp, #0x24]
00619998: bl       #0x669e24
0061999c: ldr      r3, [r0, #4]
006199a0: mov      r0, r7
006199a4: ldrb     r2, [r3, r4]
006199a8: ldrb     sl, [r3, sl]
006199ac: ldrb     r5, [r3, r5]
006199b0: rsb      sl, r2, sl
006199b4: rsb      r5, r2, r5
006199b8: bl       #0x669e54
006199bc: cmp      r0, #0
006199c0: uxtb     r5, r5
006199c4: uxtb     sl, sl
006199c8: bne      #0x619a04
006199cc: mov      r0, r5
006199d0: bl       #0x30e964
006199d4: mov      r4, r0
006199d8: rsb      r0, r5, sl
006199dc: bl       #0x30e964
006199e0: mov      r1, r0
006199e4: mov      r0, r8
006199e8: bl       #0x30ed6c
006199ec: mov      r1, r0
006199f0: mov      r0, r4
006199f4: bl       #0x30eba4
006199f8: bl       #0x8be2a0
006199fc: strb     r0, [r6]
00619a00: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00619a04: mov      r0, r7
00619a08: bl       #0x669e68
00619a0c: ldrb     r1, [r0]
00619a10: mov      r3, r6
00619a14: mov      r2, r0
00619a18: strb     r1, [r3], #1
00619a1c: ldrb     r1, [r2, #1]
00619a20: mov      r0, r5
00619a24: strb     r1, [r6, #1]
00619a28: ldrb     r2, [r2, #2]
00619a2c: strb     r2, [r3, #1]
00619a30: bl       #0x30e964
00619a34: mov      r4, r0
00619a38: rsb      r0, r5, sl
00619a3c: bl       #0x30e964
00619a40: mov      r1, r0
00619a44: mov      r0, r8
00619a48: bl       #0x30ed6c
00619a4c: mov      r1, r0
00619a50: mov      r0, r4
00619a54: bl       #0x30eba4
00619a58: bl       #0x8be2a0
00619a5c: strb     r0, [r6, #3]
00619a60: pop      {r4, r5, r6, r7, r8, sb, sl, pc}

# _ZNK6glitch7collada18SAnimationAccessor15getDefaultValueEv
00669e68: ldr      r3, [r0]
00669e6c: ldr      r3, [r3, #0x18]
00669e70: ldr      r0, [r3, #8]
00669e74: bx       lr

# _ZN6glitch7collada18ISceneNodeAnimator9forceBindEv
00667f18: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00667f1c: ldr      r6, [r0, #0x10]
00667f20: ldr      sl, [pc, #0xeec]
00667f24: sub      sp, sp, #0x24
00667f28: cmp      r6, #0
00667f2c: mov      r4, r0
00667f30: add      sl, pc, sl
00667f34: beq      #0x6681e4
00667f38: ldr      r3, [r0]
00667f3c: mov      lr, pc
00667f40: ldr      pc, [r3, #0x70]
00667f44: subs     r7, r0, #0
00667f48: ble      #0x6681e4
00667f4c: ldr      r3, [pc, #0xec4]
00667f50: ldr      fp, [pc, #0xec4]
00667f54: mov      r5, #0
00667f58: add      r3, pc, r3
00667f5c: str      r3, [sp]
00667f60: ldr      r3, [pc, #0xeb8]
00667f64: add      r3, pc, r3
00667f68: str      r3, [sp, #4]
00667f6c: ldr      r3, [pc, #0xeb0]
00667f70: add      r3, pc, r3
00667f74: str      r3, [sp, #8]
00667f78: ldr      r3, [pc, #0xea8]
00667f7c: add      r3, pc, r3
00667f80: str      r3, [sp, #0xc]
00667f84: mov      r1, r5
00667f88: ldr      r3, [r4]
00667f8c: mov      r0, r4
00667f90: mov      lr, pc
00667f94: ldr      pc, [r3, #0x6c]
00667f98: ldr      r3, [r4]
00667f9c: mov      sb, r0
00667fa0: mov      r1, r5
00667fa4: mov      r0, r4
00667fa8: mov      lr, pc
00667fac: ldr      pc, [r3, #0x54]
00667fb0: ldr      r3, [r0, #8]
00667fb4: sub      r3, r3, #1
00667fb8: cmp      r3, #0x5a
00667fbc: addls    pc, pc, r3, lsl #2
00667fc0: b        #0x6681ec
00667fc4: b        #0x66820c
00667fc8: b        #0x66820c
00667fcc: b        #0x66820c
00667fd0: b        #0x66820c
00667fd4: b        #0x66820c
00667fd8: b        #0x6681ec
00667fdc: b        #0x6681ec
00667fe0: b        #0x6681ec
00667fe4: b        #0x66820c
00667fe8: b        #0x66820c
00667fec: b        #0x66820c
00667ff0: b        #0x66820c
00667ff4: b        #0x66820c
00667ff8: b        #0x668258
00667ffc: b        #0x6682ac
00668000: b        #0x6682cc
00668004: b        #0x6681ec
00668008: b        #0x6681ec
0066800c: b        #0x6681ec
00668010: b        #0x66820c
00668014: b        #0x6681ec
00668018: b        #0x6681ec
0066801c: b        #0x6681ec
00668020: b        #0x6681ec
00668024: b        #0x6681ec
00668028: b        #0x66830c
0066802c: b        #0x6681ec
00668030: b        #0x668354
00668034: b        #0x66839c
00668038: b        #0x6683e4
0066803c: b        #0x66842c
00668040: b        #0x668474
00668044: b        #0x6684bc
00668048: b        #0x668504
0066804c: b        #0x66854c
00668050: b        #0x668594
00668054: b        #0x6685dc
00668058: b        #0x668624
0066805c: b        #0x66866c
00668060: b        #0x6686b4
00668064: b        #0x6686fc
00668068: b        #0x668744
0066806c: b        #0x66878c
00668070: b        #0x6687d4
00668074: b        #0x66881c
00668078: b        #0x668864
0066807c: b        #0x6688ac
00668080: b        #0x6688f4
00668084: b        #0x66893c
00668088: b        #0x668984
0066808c: b        #0x6689cc
00668090: b        #0x668a14
00668094: b        #0x668a5c
00668098: b        #0x668aac
0066809c: b        #0x668af4
006680a0: b        #0x668b3c
006680a4: b        #0x668b8c
006680a8: b        #0x668bd4
006680ac: b        #0x668c1c
006680b0: b        #0x668c64
006680b4: b        #0x668cac
006680b8: b        #0x668cf4
006680bc: b        #0x668d3c
006680c0: b        #0x668d84
006680c4: b        #0x668dcc
006680c8: b        #0x668ecc
006680cc: b        #0x668f1c
006680d0: b        #0x668f64
006680d4: b        #0x668fac
006680d8: b        #0x668ff0
006680dc: b        #0x6681ec
006680e0: b        #0x6681ec
006680e4: b        #0x6681ec
006680e8: b        #0x6681ec
006680ec: b        #0x6681ec
006680f0: b        #0x6681ec
006680f4: b        #0x6681ec
006680f8: b        #0x6681ec
006680fc: b        #0x6681ec
00668100: b        #0x6681ec
00668104: b        #0x6681ec
00668108: b        #0x6681ec
0066810c: b        #0x6681ec
00668110: b        #0x6681ec
00668114: b        #0x6681ec
00668118: b        #0x668130
0066811c: b        #0x668130
00668120: b        #0x668130
00668124: b        #0x668130
00668128: b        #0x668130
0066812c: b        #0x668130
00668130: add      r8, sp, #0x1c
00668134: mov      r2, sb
00668138: mov      r0, r8
0066813c: mov      r1, r6
00668140: mov      r3, #0
00668144: bl       #0x65ca30
00668148: ldr      r2, [sp, #0x1c]
0066814c: cmp      r2, #0
00668150: beq      #0x6694a4
00668154: mov      r1, r5
00668158: ldr      r3, [r4]
0066815c: mov      r0, r4
00668160: mov      lr, pc
00668164: ldr      pc, [r3, #0x54]
00668168: ldr      r3, [pc, #0xcbc]
0066816c: mov      r2, #0
00668170: mvn      r1, #0
00668174: ldr      r3, [sl, r3]
00668178: str      r2, [sp, #0x14]
0066817c: str      r1, [sp, #0x18]
00668180: add      r3, r3, #8
00668184: str      r3, [sp, #0x10]
00668188: ldr      r3, [r0, #0xc]
0066818c: str      r3, [sp, #0x14]
00668190: ldr      r3, [sp, #0x1c]
00668194: ldr      r1, [r0, #0xc]
00668198: ldr      r0, [r3, #4]
0066819c: bl       #0x5d308c
006681a0: str      r0, [sp, #0x18]
006681a4: add      r3, sp, #0x10
006681a8: mov      r0, r4
006681ac: ldr      ip, [r4]
006681b0: mov      r1, r5
006681b4: ldr      r2, [sp, #0x1c]
006681b8: mov      lr, pc
006681bc: ldr      pc, [ip, #0x68]
006681c0: ldr      r3, [pc, #0xc68]
006681c4: mov      r0, r8
006681c8: ldr      r3, [sl, r3]
006681cc: add      r3, r3, #8
006681d0: str      r3, [sp, #0x10]
006681d4: bl       #0x310be8
006681d8: add      r5, r5, #1
006681dc: cmp      r5, r7
006681e0: bne      #0x667f84
006681e4: add      sp, sp, #0x24
006681e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006681ec: mov      r2, #0
006681f0: ldr      ip, [r4]
006681f4: mov      r0, r4
006681f8: mov      r1, r5
006681fc: mov      r3, r2
00668200: mov      lr, pc
00668204: ldr      pc, [ip, #0x68]
00668208: b        #0x6681d8
0066820c: mov      r1, sb
00668210: mov      r0, r6
00668214: bl       #0x5985e4
00668218: mov      r8, r0
0066821c: ldr      ip, [r4]
00668220: mov      r0, r4
00668224: mov      r1, r5
00668228: mov      r2, r8
0066822c: mov      r3, #0
00668230: mov      lr, pc
00668234: ldr      pc, [ip, #0x68]
00668238: cmp      r8, #0
0066823c: beq      #0x6681d8
00668240: mov      r0, r8
00668244: ldr      r3, [r8]
00668248: mov      r1, r4
0066824c: mov      lr, pc
00668250: ldr      pc, [r3, #0x78]
00668254: b        #0x6681d8
00668258: mov      r1, sb
0066825c: mov      r0, r6
00668260: bl       #0x65b5b0
00668264: subs     r8, r0, #0
00668268: beq      #0x669484
0066826c: mov      r1, r5
00668270: ldr      r3, [r4]
00668274: mov      r0, r4
00668278: mov      lr, pc
0066827c: ldr      pc, [r3, #0x54]
00668280: ldr      r3, [r8, #0x24]
00668284: ldrb     r2, [r0, #0xc]
00668288: ldr      ip, [r4]
0066828c: mov      r0, r4
00668290: add      r2, r3, r2, lsl #3
00668294: add      r2, r2, #0xc
00668298: mov      r1, r5
0066829c: mov      r3, #0
006682a0: mov      lr, pc
006682a4: ldr      pc, [ip, #0x68]
006682a8: b        #0x6681d8
006682ac: ldr      ip, [r4]
006682b0: mov      r0, r4
006682b4: mov      r1, r5
006682b8: mov      r2, r6
006682bc: mov      r3, #0
006682c0: mov      lr, pc
006682c4: ldr      pc, [ip, #0x68]
006682c8: b        #0x6681d8
006682cc: mov      r1, sb
006682d0: mov      r0, r6
006682d4: bl       #0x65b5f4
006682d8: subs     r2, r0, #0
006682dc: ldreq    ip, [r4]
006682e0: moveq    r0, r4
006682e4: moveq    r1, r5
006682e8: moveq    r3, r2
006682ec: ldrne    r2, [r2, #0x134]
006682f0: ldrne    ip, [r4]
006682f4: movne    r0, r4
006682f8: movne    r1, r5
006682fc: movne    r3, #0
00668300: mov      lr, pc
00668304: ldr      pc, [ip, #0x68]
00668308: b        #0x6681d8
0066830c: add      r8, sp, #0x1c
00668310: mov      r3, #0
00668314: mov      r2, sb
00668318: mov      r0, r8
0066831c: mov      r1, r6
00668320: bl       #0x65ca30
00668324: ldr      r2, [sp, #0x1c]
00668328: mov      r0, r4
0066832c: ldr      ip, [r4]
00668330: cmp      r2, #0
00668334: mov      r1, r5
00668338: moveq    r3, r2
0066833c: movne    r3, #0
00668340: mov      lr, pc
00668344: ldr      pc, [ip, #0x68]
00668348: mov      r0, r8
0066834c: bl       #0x310be8
00668350: b        #0x6681d8
00668354: mov      r1, sb
00668358: mov      r0, r6
0066835c: bl       #0x65b4e4
00668360: subs     r2, r0, #0
00668364: beq      #0x669468
00668368: ldr      r1, [pc, #0xac4]
0066836c: ldr      ip, [r4]
00668370: ldr      r3, [r2]
00668374: add      r1, pc, r1
00668378: ldr      r8, [ip, #0x68]
0066837c: mov      lr, pc
00668380: ldr      pc, [r3, #0xfc]
00668384: mov      r1, r5
00668388: mov      r2, r0
0066838c: mov      r3, #0
00668390: mov      r0, r4
00668394: blx      r8
00668398: b        #0x6681d8
0066839c: mov      r1, sb
006683a0: mov      r0, r6
006683a4: bl       #0x65b4e4
006683a8: subs     r2, r0, #0
006683ac: beq      #0x66944c
006683b0: ldr      r1, [pc, #0xa80]
006683b4: ldr      ip, [r4]
006683b8: ldr      r3, [r2]
006683bc: add      r1, pc, r1
006683c0: ldr      r8, [ip, #0x68]
006683c4: mov      lr, pc
006683c8: ldr      pc, [r3, #0xfc]
006683cc: mov      r1, r5
006683d0: mov      r2, r0
006683d4: mov      r3, #0
006683d8: mov      r0, r4
006683dc: blx      r8
006683e0: b        #0x6681d8
006683e4: mov      r1, sb
006683e8: mov      r0, r6
006683ec: bl       #0x65b4e4
006683f0: subs     r2, r0, #0
006683f4: beq      #0x669430
006683f8: ldr      r1, [pc, #0xa3c]
006683fc: ldr      ip, [r4]
00668400: ldr      r3, [r2]
00668404: add      r1, pc, r1
00668408: ldr      r8, [ip, #0x68]
0066840c: mov      lr, pc
00668410: ldr      pc, [r3, #0xfc]
00668414: mov      r1, r5
00668418: mov      r2, r0
0066841c: mov      r3, #0
00668420: mov      r0, r4
00668424: blx      r8
00668428: b        #0x6681d8
0066842c: mov      r1, sb
00668430: mov      r0, r6
00668434: bl       #0x65b4e4
00668438: subs     r2, r0, #0
0066843c: beq      #0x669414
00668440: ldr      r1, [pc, #0x9f8]
00668444: ldr      ip, [r4]
00668448: ldr      r3, [r2]
0066844c: add      r1, pc, r1
00668450: ldr      r8, [ip, #0x68]
00668454: mov      lr, pc
00668458: ldr      pc, [r3, #0xfc]
0066845c: mov      r1, r5
00668460: mov      r2, r0
00668464: mov      r3, #0
00668468: mov      r0, r4
0066846c: blx      r8
00668470: b        #0x6681d8
00668474: mov      r1, sb
00668478: mov      r0, r6
0066847c: bl       #0x65b4e4
00668480: subs     r2, r0, #0
00668484: beq      #0x6693f8
00668488: ldr      r1, [pc, #0x9b4]
0066848c: ldr      ip, [r4]
00668490: ldr      r3, [r2]
00668494: add      r1, pc, r1
00668498: ldr      r8, [ip, #0x68]
0066849c: mov      lr, pc
006684a0: ldr      pc, [r3, #0xfc]
006684a4: mov      r1, r5
006684a8: mov      r2, r0
006684ac: mov      r3, #0
006684b0: mov      r0, r4
006684b4: blx      r8
006684b8: b        #0x6681d8
006684bc: mov      r1, sb
006684c0: mov      r0, r6
006684c4: bl       #0x65b4e4
006684c8: subs     r2, r0, #0
006684cc: beq      #0x6693dc
006684d0: ldr      r1, [pc, #0x970]
006684d4: ldr      ip, [r4]
006684d8: ldr      r3, [r2]
006684dc: add      r1, pc, r1
006684e0: ldr      r8, [ip, #0x68]
006684e4: mov      lr, pc
006684e8: ldr      pc, [r3, #0xfc]
006684ec: mov      r1, r5
006684f0: mov      r2, r0
006684f4: mov      r3, #0
006684f8: mov      r0, r4
006684fc: blx      r8
00668500: b        #0x6681d8
00668504: mov      r1, sb
00668508: mov      r0, r6
0066850c: bl       #0x65b4e4
00668510: subs     r2, r0, #0
00668514: beq      #0x6693c0
00668518: ldr      r1, [pc, #0x92c]
0066851c: ldr      ip, [r4]
00668520: ldr      r3, [r2]
00668524: add      r1, pc, r1
00668528: ldr      r8, [ip, #0x68]
0066852c: mov      lr, pc
00668530: ldr      pc, [r3, #0xfc]
00668534: mov      r1, r5
00668538: mov      r2, r0
0066853c: mov      r3, #0
00668540: mov      r0, r4
00668544: blx      r8
00668548: b        #0x6681d8
0066854c: mov      r1, sb
00668550: mov      r0, r6
00668554: bl       #0x65b4e4
00668558: subs     r2, r0, #0
0066855c: beq      #0x6693a4
00668560: ldr      r1, [pc, #0x8e8]
00668564: ldr      ip, [r4]
00668568: ldr      r3, [r2]
0066856c: add      r1, pc, r1
00668570: ldr      r8, [ip, #0x68]
00668574: mov      lr, pc
00668578: ldr      pc, [r3, #0xfc]
0066857c: mov      r1, r5
00668580: mov      r2, r0
00668584: mov      r3, #0
00668588: mov      r0, r4
0066858c: blx      r8
00668590: b        #0x6681d8
00668594: mov      r1, sb
00668598: mov      r0, r6
0066859c: bl       #0x65b4e4
006685a0: subs     r2, r0, #0
006685a4: beq      #0x669388
006685a8: ldr      r1, [pc, #0x8a4]
006685ac: ldr      ip, [r4]
006685b0: ldr      r3, [r2]
006685b4: add      r1, pc, r1
006685b8: ldr      r8, [ip, #0x68]
006685bc: mov      lr, pc
006685c0: ldr      pc, [r3, #0xfc]
006685c4: mov      r1, r5
006685c8: mov      r2, r0
006685cc: mov      r3, #0
006685d0: mov      r0, r4
006685d4: blx      r8
006685d8: b        #0x6681d8
006685dc: mov      r1, sb
006685e0: mov      r0, r6
006685e4: bl       #0x65b4e4
006685e8: subs     r2, r0, #0
006685ec: beq      #0x66936c
006685f0: ldr      r1, [pc, #0x860]
006685f4: ldr      ip, [r4]
006685f8: ldr      r3, [r2]
006685fc: add      r1, pc, r1
00668600: ldr      r8, [ip, #0x68]
00668604: mov      lr, pc
00668608: ldr      pc, [r3, #0xfc]
0066860c: mov      r1, r5
00668610: mov      r2, r0
00668614: mov      r3, #0
00668618: mov      r0, r4
0066861c: blx      r8
00668620: b        #0x6681d8
00668624: mov      r1, sb
00668628: mov      r0, r6
0066862c: bl       #0x65b4e4
00668630: subs     r2, r0, #0
00668634: beq      #0x669350
00668638: ldr      r1, [pc, #0x81c]
0066863c: ldr      ip, [r4]
00668640: ldr      r3, [r2]
00668644: add      r1, pc, r1
00668648: ldr      r8, [ip, #0x68]
0066864c: mov      lr, pc
00668650: ldr      pc, [r3, #0xfc]
00668654: mov      r1, r5
00668658: mov      r2, r0
0066865c: mov      r3, #0
00668660: mov      r0, r4
00668664: blx      r8
00668668: b        #0x6681d8
0066866c: mov      r1, sb
00668670: mov      r0, r6
00668674: bl       #0x65b4e4
00668678: subs     r2, r0, #0
0066867c: beq      #0x669334
00668680: ldr      r1, [pc, #0x7d8]
00668684: ldr      ip, [r4]
00668688: ldr      r3, [r2]
0066868c: add      r1, pc, r1
00668690: ldr      r8, [ip, #0x68]
00668694: mov      lr, pc
00668698: ldr      pc, [r3, #0xfc]
0066869c: mov      r1, r5
006686a0: mov      r2, r0
006686a4: mov      r3, #0
006686a8: mov      r0, r4
006686ac: blx      r8
006686b0: b        #0x6681d8
006686b4: mov      r1, sb
006686b8: mov      r0, r6
006686bc: bl       #0x65b4e4
006686c0: subs     r2, r0, #0
006686c4: beq      #0x669318
006686c8: ldr      r1, [pc, #0x794]
006686cc: ldr      ip, [r4]
006686d0: ldr      r3, [r2]
006686d4: add      r1, pc, r1
006686d8: ldr      r8, [ip, #0x68]
006686dc: mov      lr, pc
006686e0: ldr      pc, [r3, #0xfc]
006686e4: mov      r1, r5
006686e8: mov      r2, r0
006686ec: mov      r3, #0
006686f0: mov      r0, r4
006686f4: blx      r8
006686f8: b        #0x6681d8
006686fc: mov      r1, sb
00668700: mov      r0, r6
00668704: bl       #0x65b4e4
00668708: subs     r2, r0, #0
0066870c: beq      #0x6692fc
00668710: ldr      r1, [pc, #0x750]
00668714: ldr      ip, [r4]
00668718: ldr      r3, [r2]
0066871c: add      r1, pc, r1
00668720: ldr      r8, [ip, #0x68]
00668724: mov      lr, pc
00668728: ldr      pc, [r3, #0xfc]
0066872c: mov      r1, r5
00668730: mov      r2, r0
00668734: mov      r3, #0
00668738: mov      r0, r4
0066873c: blx      r8
00668740: b        #0x6681d8
00668744: mov      r1, sb
00668748: mov      r0, r6
0066874c: bl       #0x65b4e4
00668750: subs     r2, r0, #0
00668754: beq      #0x6692e0
00668758: ldr      r1, [pc, #0x70c]
0066875c: ldr      ip, [r4]
00668760: ldr      r3, [r2]
00668764: add      r1, pc, r1
00668768: ldr      r8, [ip, #0x68]
0066876c: mov      lr, pc
00668770: ldr      pc, [r3, #0xfc]
00668774: mov      r1, r5
00668778: mov      r2, r0
0066877c: mov      r3, #0
00668780: mov      r0, r4
00668784: blx      r8
00668788: b        #0x6681d8
0066878c: mov      r1, sb
00668790: mov      r0, r6
00668794: bl       #0x65b4e4
00668798: subs     r2, r0, #0
0066879c: beq      #0x6692c4
006687a0: ldr      r1, [pc, #0x6c8]
006687a4: ldr      ip, [r4]
006687a8: ldr      r3, [r2]
006687ac: add      r1, pc, r1
006687b0: ldr      r8, [ip, #0x68]
006687b4: mov      lr, pc
006687b8: ldr      pc, [r3, #0xfc]
006687bc: mov      r1, r5
006687c0: mov      r2, r0
006687c4: mov      r3, #0
006687c8: mov      r0, r4
006687cc: blx      r8
006687d0: b        #0x6681d8
006687d4: mov      r1, sb
006687d8: mov      r0, r6
006687dc: bl       #0x65b4e4
006687e0: subs     r2, r0, #0
006687e4: beq      #0x6692a8
006687e8: ldr      r1, [pc, #0x684]
006687ec: ldr      ip, [r4]
006687f0: ldr      r3, [r2]
006687f4: add      r1, pc, r1
006687f8: ldr      r8, [ip, #0x68]
006687fc: mov      lr, pc
00668800: ldr      pc, [r3, #0xfc]
00668804: mov      r1, r5
00668808: mov      r2, r0
0066880c: mov      r3, #0
00668810: mov      r0, r4
00668814: blx      r8
00668818: b        #0x6681d8
0066881c: mov      r1, sb
00668820: mov      r0, r6
00668824: bl       #0x65b4e4
00668828: subs     r2, r0, #0
0066882c: beq      #0x66928c
00668830: ldr      r1, [pc, #0x640]
00668834: ldr      ip, [r4]
00668838: ldr      r3, [r2]
0066883c: add      r1, pc, r1
00668840: ldr      r8, [ip, #0x68]
00668844: mov      lr, pc
00668848: ldr      pc, [r3, #0xfc]
0066884c: mov      r1, r5
00668850: mov      r2, r0
00668854: mov      r3, #0
00668858: mov      r0, r4
0066885c: blx      r8
00668860: b        #0x6681d8
00668864: mov      r1, sb
00668868: mov      r0, r6
0066886c: bl       #0x65b4e4
00668870: subs     r2, r0, #0
00668874: beq      #0x669270
00668878: ldr      r1, [pc, #0x5fc]
0066887c: ldr      ip, [r4]
00668880: ldr      r3, [r2]
00668884: add      r1, pc, r1
00668888: ldr      r8, [ip, #0x68]
0066888c: mov      lr, pc
00668890: ldr      pc, [r3, #0xfc]
00668894: mov      r1, r5
00668898: mov      r2, r0
0066889c: mov      r3, #0
006688a0: mov      r0, r4
006688a4: blx      r8
006688a8: b        #0x6681d8
006688ac: mov      r1, sb
006688b0: mov      r0, r6
006688b4: bl       #0x65b4e4
006688b8: subs     r2, r0, #0
006688bc: beq      #0x669254
006688c0: ldr      r1, [pc, #0x5b8]
006688c4: ldr      ip, [r4]
006688c8: ldr      r3, [r2]
006688cc: add      r1, pc, r1
006688d0: ldr      r8, [ip, #0x68]
006688d4: mov      lr, pc
006688d8: ldr      pc, [r3, #0xfc]
006688dc: mov      r1, r5
006688e0: mov      r2, r0
006688e4: mov      r3, #0
006688e8: mov      r0, r4
006688ec: blx      r8
006688f0: b        #0x6681d8
006688f4: mov      r1, sb
006688f8: mov      r0, r6
006688fc: bl       #0x65b4e4
00668900: subs     r2, r0, #0
00668904: beq      #0x669238
00668908: ldr      r1, [pc, #0x574]
0066890c: ldr      ip, [r4]
00668910: ldr      r3, [r2]
00668914: add      r1, pc, r1
00668918: ldr      r8, [ip, #0x68]
0066891c: mov      lr, pc
00668920: ldr      pc, [r3, #0xfc]
00668924: mov      r1, r5
00668928: mov      r2, r0
0066892c: mov      r3, #0
00668930: mov      r0, r4
00668934: blx      r8
00668938: b        #0x6681d8
0066893c: mov      r1, sb
00668940: mov      r0, r6
00668944: bl       #0x65b4e4
00668948: subs     r2, r0, #0
0066894c: beq      #0x66921c
00668950: ldr      r1, [pc, #0x530]
00668954: ldr      ip, [r4]
00668958: ldr      r3, [r2]
0066895c: add      r1, pc, r1
00668960: ldr      r8, [ip, #0x68]
00668964: mov      lr, pc
00668968: ldr      pc, [r3, #0xfc]
0066896c: mov      r1, r5
00668970: mov      r2, r0
00668974: mov      r3, #0
00668978: mov      r0, r4
0066897c: blx      r8
00668980: b        #0x6681d8
00668984: mov      r1, sb
00668988: mov      r0, r6
0066898c: bl       #0x65b4e4
00668990: subs     r2, r0, #0
00668994: beq      #0x669200
00668998: ldr      r1, [pc, #0x4ec]
0066899c: ldr      ip, [r4]
006689a0: ldr      r3, [r2]
006689a4: add      r1, pc, r1
006689a8: ldr      r8, [ip, #0x68]
006689ac: mov      lr, pc
006689b0: ldr      pc, [r3, #0xfc]
006689b4: mov      r1, r5
006689b8: mov      r2, r0
006689bc: mov      r3, #0
006689c0: mov      r0, r4
006689c4: blx      r8
006689c8: b        #0x6681d8
006689cc: mov      r1, sb
006689d0: mov      r0, r6
006689d4: bl       #0x65b4e4
006689d8: subs     r2, r0, #0
006689dc: beq      #0x6691e4
006689e0: ldr      r1, [pc, #0x4a8]
006689e4: ldr      ip, [r4]
006689e8: ldr      r3, [r2]
006689ec: add      r1, pc, r1
006689f0: ldr      r8, [ip, #0x68]
006689f4: mov      lr, pc
006689f8: ldr      pc, [r3, #0xfc]
006689fc: mov      r1, r5
00668a00: mov      r2, r0
00668a04: mov      r3, #0
00668a08: mov      r0, r4
00668a0c: blx      r8
00668a10: b        #0x6681d8
00668a14: mov      r1, sb
00668a18: mov      r0, r6
00668a1c: bl       #0x65b4e4
00668a20: subs     r2, r0, #0
00668a24: beq      #0x6691c8
00668a28: ldr      r1, [pc, #0x464]
00668a2c: ldr      ip, [r4]
00668a30: ldr      r3, [r2]
00668a34: add      r1, pc, r1
00668a38: ldr      r8, [ip, #0x68]
00668a3c: mov      lr, pc
00668a40: ldr      pc, [r3, #0xfc]
00668a44: mov      r1, r5
00668a48: mov      r2, r0
00668a4c: mov      r3, #0
00668a50: mov      r0, r4
00668a54: blx      r8
00668a58: b        #0x6681d8
00668a5c: mov      r1, sb
00668a60: mov      r0, r6
00668a64: bl       #0x65b4e4
00668a68: subs     r2, r0, #0
00668a6c: beq      #0x66951c
00668a70: ldrb     r8, [r2, #0x13c]
00668a74: cmp      r8, #0
00668a78: bne      #0x6681d8
00668a7c: ldr      ip, [r4]
00668a80: ldr      r3, [r2]
00668a84: ldr      r1, [sp, #0xc]
00668a88: ldr      sb, [ip, #0x68]
00668a8c: mov      lr, pc
00668a90: ldr      pc, [r3, #0xfc]
00668a94: mov      r1, r5
00668a98: mov      r2, r0
00668a9c: mov      r3, r8
00668aa0: mov      r0, r4
00668aa4: blx      sb
00668aa8: b        #0x6681d8
00668aac: mov      r1, sb
00668ab0: mov      r0, r6
00668ab4: bl       #0x65b4e4
00668ab8: subs     r2, r0, #0
00668abc: beq      #0x669040
00668ac0: ldr      r1, [pc, #0x3d0]
00668ac4: ldr      ip, [r4]
00668ac8: ldr      r3, [r2]
00668acc: add      r1, pc, r1
00668ad0: ldr      r8, [ip, #0x68]
00668ad4: mov      lr, pc
00668ad8: ldr      pc, [r3, #0xfc]
00668adc: mov      r1, r5
00668ae0: mov      r2, r0
00668ae4: mov      r3, #0
00668ae8: mov      r0, r4
00668aec: blx      r8
00668af0: b        #0x6681d8
00668af4: mov      r1, sb
00668af8: mov      r0, r6
00668afc: bl       #0x65b4e4
00668b00: subs     r2, r0, #0
00668b04: beq      #0x6691ac
00668b08: ldr      r1, [pc, #0x38c]
00668b0c: ldr      ip, [r4]
00668b10: ldr      r3, [r2]
00668b14: add      r1, pc, r1
00668b18: ldr      r8, [ip, #0x68]
00668b1c: mov      lr, pc
00668b20: ldr      pc, [r3, #0xfc]
00668b24: mov      r1, r5
00668b28: mov      r2, r0
00668b2c: mov      r3, #0
00668b30: mov      r0, r4
00668b34: blx      r8
00668b38: b        #0x6681d8
00668b3c: mov      r1, sb
00668b40: mov      r0, r6
00668b44: bl       #0x65b4e4
00668b48: subs     r2, r0, #0
00668b4c: beq      #0x669500
00668b50: ldrb     r8, [r2, #0x13d]
00668b54: cmp      r8, #0
00668b58: bne      #0x6681d8
00668b5c: ldr      ip, [r4]
00668b60: ldr      r3, [r2]
00668b64: ldr      r1, [sp, #8]
00668b68: ldr      sb, [ip, #0x68]
00668b6c: mov      lr, pc
00668b70: ldr      pc, [r3, #0xfc]
00668b74: mov      r1, r5
00668b78: mov      r2, r0
00668b7c: mov      r3, r8
00668b80: mov      r0, r4
00668b84: blx      sb
00668b88: b        #0x6681d8
00668b8c: mov      r1, sb
00668b90: mov      r0, r6
00668b94: bl       #0x65b4e4
00668b98: subs     r2, r0, #0
00668b9c: beq      #0x6690b0
00668ba0: ldr      r1, [pc, #0x2f8]
00668ba4: ldr      ip, [r4]
00668ba8: ldr      r3, [r2]
00668bac: add      r1, pc, r1
00668bb0: ldr      r8, [ip, #0x68]
00668bb4: mov      lr, pc
00668bb8: ldr      pc, [r3, #0xfc]
00668bbc: mov      r1, r5
00668bc0: mov      r2, r0
00668bc4: mov      r3, #0
00668bc8: mov      r0, r4
00668bcc: blx      r8
00668bd0: b        #0x6681d8
00668bd4: mov      r1, sb
00668bd8: mov      r0, r6
00668bdc: bl       #0x65b4e4
00668be0: subs     r2, r0, #0
00668be4: beq      #0x6690e8
00668be8: ldr      r1, [pc, #0x2b4]
00668bec: ldr      ip, [r4]
00668bf0: ldr      r3, [r2]
00668bf4: add      r1, pc, r1
00668bf8: ldr      r8, [ip, #0x68]
00668bfc: mov      lr, pc
00668c00: ldr      pc, [r3, #0xfc]
00668c04: mov      r1, r5
00668c08: mov      r2, r0
00668c0c: mov      r3, #0
00668c10: mov      r0, r4
00668c14: blx      r8
00668c18: b        #0x6681d8
00668c1c: mov      r1, sb
00668c20: mov      r0, r6
00668c24: bl       #0x65b4e4
00668c28: subs     r2, r0, #0
00668c2c: beq      #0x6690cc
00668c30: ldr      r1, [pc, #0x270]
00668c34: ldr      ip, [r4]
00668c38: ldr      r3, [r2]
00668c3c: add      r1, pc, r1
00668c40: ldr      r8, [ip, #0x68]
00668c44: mov      lr, pc
00668c48: ldr      pc, [r3, #0xfc]
00668c4c: mov      r1, r5
00668c50: mov      r2, r0
00668c54: mov      r3, #0
00668c58: mov      r0, r4
00668c5c: blx      r8
00668c60: b        #0x6681d8
00668c64: mov      r1, sb
00668c68: mov      r0, r6
00668c6c: bl       #0x65b4e4
00668c70: subs     r2, r0, #0
00668c74: beq      #0x669158
00668c78: ldr      r1, [pc, #0x22c]
00668c7c: ldr      ip, [r4]
00668c80: ldr      r3, [r2]
00668c84: add      r1, pc, r1
00668c88: ldr      r8, [ip, #0x68]
00668c8c: mov      lr, pc
00668c90: ldr      pc, [r3, #0xfc]
00668c94: mov      r1, r5
00668c98: mov      r2, r0
00668c9c: mov      r3, #0
00668ca0: mov      r0, r4
00668ca4: blx      r8
00668ca8: b        #0x6681d8
00668cac: mov      r1, sb
00668cb0: mov      r0, r6
00668cb4: bl       #0x65b4e4
00668cb8: subs     r2, r0, #0
00668cbc: beq      #0x66913c
00668cc0: ldr      r1, [pc, #0x1e8]
00668cc4: ldr      ip, [r4]
00668cc8: ldr      r3, [r2]
00668ccc: add      r1, pc, r1
00668cd0: ldr      r8, [ip, #0x68]
00668cd4: mov      lr, pc
00668cd8: ldr      pc, [r3, #0xfc]
00668cdc: mov      r1, r5
00668ce0: mov      r2, r0
00668ce4: mov      r3, #0
00668ce8: mov      r0, r4
00668cec: blx      r8
00668cf0: b        #0x6681d8
00668cf4: mov      r1, sb
00668cf8: mov      r0, r6
00668cfc: bl       #0x65b4e4
00668d00: subs     r2, r0, #0
00668d04: beq      #0x669120
00668d08: ldr      r1, [pc, #0x1a4]
00668d0c: ldr      ip, [r4]
00668d10: ldr      r3, [r2]
00668d14: add      r1, pc, r1
00668d18: ldr      r8, [ip, #0x68]
00668d1c: mov      lr, pc
00668d20: ldr      pc, [r3, #0xfc]
00668d24: mov      r1, r5
00668d28: mov      r2, r0
00668d2c: mov      r3, #0
00668d30: mov      r0, r4
00668d34: blx      r8
00668d38: b        #0x6681d8
00668d3c: mov      r1, sb
00668d40: mov      r0, r6
00668d44: bl       #0x65b4e4
00668d48: subs     r2, r0, #0
00668d4c: beq      #0x669104
00668d50: ldr      r1, [pc, #0x160]
00668d54: ldr      ip, [r4]
00668d58: ldr      r3, [r2]
00668d5c: add      r1, pc, r1
00668d60: ldr      r8, [ip, #0x68]
00668d64: mov      lr, pc
00668d68: ldr      pc, [r3, #0xfc]
00668d6c: mov      r1, r5
00668d70: mov      r2, r0
00668d74: mov      r3, #0
00668d78: mov      r0, r4
00668d7c: blx      r8
00668d80: b        #0x6681d8
00668d84: mov      r1, sb
00668d88: mov      r0, r6
00668d8c: bl       #0x65b4e4
00668d90: subs     r2, r0, #0
00668d94: beq      #0x669190
00668d98: ldr      r1, [pc, #0x11c]
00668d9c: ldr      ip, [r4]
00668da0: ldr      r3, [r2]
00668da4: add      r1, pc, r1
00668da8: ldr      r8, [ip, #0x68]
00668dac: mov      lr, pc
00668db0: ldr      pc, [r3, #0xfc]
00668db4: mov      r1, r5
00668db8: mov      r2, r0
00668dbc: mov      r3, #0
00668dc0: mov      r0, r4
00668dc4: blx      r8
00668dc8: b        #0x6681d8
00668dcc: mov      r1, sb
00668dd0: mov      r0, r6
00668dd4: bl       #0x65b4e4
00668dd8: subs     r2, r0, #0
00668ddc: beq      #0x669174
00668de0: ldr      r1, [pc, #0xd8]
00668de4: ldr      ip, [r4]
00668de8: ldr      r3, [r2]
00668dec: add      r1, pc, r1
00668df0: ldr      r8, [ip, #0x68]
00668df4: mov      lr, pc
00668df8: ldr      pc, [r3, #0xfc]
00668dfc: mov      r1, r5
00668e00: mov      r2, r0
00668e04: mov      r3, #0
00668e08: mov      r0, r4
00668e0c: blx      r8
00668e10: b        #0x6681d8
00668e14: eorseq   ip, r2, r0, ror #22
00668e18: eoreq    sp, r7, r8, ror r2
00668e1c: mlaeq    r7, r8, r2, ip
00668e20: eoreq    sp, r7, r4, asr #4
00668e24: eoreq    sp, r7, r8, lsl r2
00668e28: eoreq    sp, r7, r4, ror #3
00668e2c: andeq    r0, r0, r0, lsl #20
00668e30: andeq    r1, r0, ip, asr r2
00668e34: eoreq    sp, r7, r4, asr #32
00668e38: eoreq    sp, r7, ip, lsr r0
00668e3c: eoreq    sp, r7, r4
00668e40: eoreq    r0, r7, r4, lsr #31
00668e44: eoreq    ip, r7, ip, lsl lr
00668e48: eoreq    ip, r7, ip, asr #26
00668e4c: eoreq    ip, r7, r4, lsl sp
00668e50: eoreq    ip, r7, r4, asr sp
00668e54: eoreq    ip, r7, ip, lsl sp
00668e58: eoreq    ip, r7, r4, ror #25
00668e5c: eoreq    ip, r7, ip, lsr #25
00668e60: eoreq    ip, r7, ip, ror ip
00668e64: eoreq    ip, r7, r4, asr #24
00668e68: eoreq    ip, r7, ip, lsr #26

# _ZNK6glitch5video17CMaterialRenderer14getParameterIDEPKct
005d308c: push     {r4, r5, r6, lr}
005d3090: mov      r4, r0
005d3094: mov      r0, r1
005d3098: mov      r1, #0
005d309c: mov      r5, r2
005d30a0: bl       #0x6a5074
005d30a4: cmp      r0, #0
005d30a8: movweq   r5, #0xffff
005d30ac: beq      #0x5d3108
005d30b0: ldr      r6, [r0]
005d30b4: mov      r1, r0
005d30b8: add      r6, r6, #1
005d30bc: str      r6, [r1], #4
005d30c0: ldrh     ip, [r4, #0xe]
005d30c4: cmp      r5, ip
005d30c8: bhs      #0x5d311c
005d30cc: ldr      r4, [r4, #0x20]
005d30d0: b        #0x5d30e0
005d30d4: uxth     r5, r2
005d30d8: cmp      r5, ip
005d30dc: bhs      #0x5d311c
005d30e0: ldr      r3, [r4, r5, lsl #4]
005d30e4: add      r2, r5, #1
005d30e8: cmp      r3, #0
005d30ec: addne    r3, r3, #4
005d30f0: cmp      r1, r3
005d30f4: bne      #0x5d30d4
005d30f8: sub      r6, r6, #1
005d30fc: cmp      r6, #0
005d3100: str      r6, [r0]
005d3104: beq      #0x5d3110
005d3108: mov      r0, r5
005d310c: pop      {r4, r5, r6, pc}
005d3110: bl       #0x6a4d9c
005d3114: mov      r0, r5
005d3118: pop      {r4, r5, r6, pc}
005d311c: movw     r5, #0xffff
005d3120: b        #0x5d30f8

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE12getValueSizeEv
0060edec: mov      r0, #4
0060edf0: bx       lr

# _ZNK6glitch7collada15animation_track10CVirtualExINS1_13CApplyValueExIA4_hNS1_6CMixinIhLi4ENS1_17SMaterialSetParamINS1_15SAnimationTypesIS4_NS_5video6SColorEEEEELi3EhEEEEE15getBlendedValueEPvPfiSF_
00626124: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00626128: cmp      r3, #1
0062612c: sub      sp, sp, #0x1c
00626130: mov      r4, r3
00626134: stm      sp, {r1, r2}
00626138: beq      #0x6261fc
0062613c: mov      r7, #0
00626140: cmp      r3, #0
00626144: str      r7, [sp, #8]
00626148: str      r7, [sp, #0xc]
0062614c: str      r7, [sp, #0x10]
00626150: str      r7, [sp, #0x14]
00626154: movne    fp, #0
00626158: addne    r8, sp, #8
0062615c: beq      #0x626230
00626160: ldr      r0, [sp, #4]
00626164: ldr      r3, [sp]
00626168: mov      r5, #0
0062616c: ldr      sl, [r0, fp]
00626170: add      sb, r3, fp
00626174: mov      r6, r5
00626178: ldrb     r0, [sb, r6]
0062617c: bl       #0x30e964
00626180: mov      r1, sl
00626184: bl       #0x30ed6c
00626188: mov      r1, r7
0062618c: bl       #0x30eba4
00626190: str      r0, [r8, r5]
00626194: add      r5, r5, #4
00626198: cmp      r5, #0x10
0062619c: add      r6, r6, #1
006261a0: ldrne    r7, [r8, r5]
006261a4: bne      #0x626178
006261a8: subs     r4, r4, #1
006261ac: add      fp, fp, #4
006261b0: ldrne    r7, [sp, #8]
006261b4: bne      #0x626160
006261b8: ldr      r0, [sp, #8]
006261bc: bl       #0x8be2a0
006261c0: ldr      r4, [sp, #0x40]
006261c4: strb     r0, [r4], #1
006261c8: ldr      r0, [sp, #0xc]
006261cc: bl       #0x8be2a0
006261d0: ldr      r3, [sp, #0x40]
006261d4: strb     r0, [r3, #1]
006261d8: ldr      r0, [sp, #0x10]
006261dc: bl       #0x8be2a0
006261e0: strb     r0, [r4, #1]
006261e4: ldr      r0, [sp, #0x14]
006261e8: bl       #0x8be2a0
006261ec: add      r4, r4, #1
006261f0: strb     r0, [r4, #1]
006261f4: add      sp, sp, #0x1c
006261f8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006261fc: ldr      r2, [sp]
00626200: ldr      r3, [sp, #0x40]
00626204: ldrb     r1, [r2], #1
00626208: strb     r1, [r3], #1
0062620c: ldr      r0, [sp]
00626210: ldrb     r1, [r0, #1]
00626214: ldr      r0, [sp, #0x40]
00626218: strb     r1, [r0, #1]
0062621c: ldrb     r1, [r2, #1]
00626220: strb     r1, [r3, #1]
00626224: ldrb     r2, [r2, #2]
00626228: strb     r2, [r3, #2]
0062622c: b        #0x6261f4
00626230: mov      r0, r7
00626234: b        #0x6261bc

# _ZN6glitch7collada21CSceneNodeAnimatorSet11animateNodeEPNS_5scene10ISceneNodeEj
0065f270: push     {r4, lr}
0065f274: mov      r1, r2
0065f278: ldr      r3, [r0]
0065f27c: mov      lr, pc
0065f280: ldr      pc, [r3, #0x50]
0065f284: pop      {r4, pc}

# _ZN14AnimControllerC1EP13RootSceneNodeb
00474d30: push     {r4, r5, r6, lr}
00474d34: ldr      r4, [pc, #0xe4]
00474d38: ldr      r3, [pc, #0xe4]
00474d3c: cmp      r1, #0
00474d40: add      r4, pc, r4
00474d44: ldr      r3, [r4, r3]
00474d48: sub      sp, sp, #8
00474d4c: mov      r5, r0
00474d50: add      r3, r3, #8
00474d54: str      r3, [r0]
00474d58: mov      r6, r2
00474d5c: str      r1, [r0, #4]
00474d60: beq      #0x474dc8
00474d64: ldr      r3, [r1]
00474d68: cmp      r6, #0
00474d6c: ldr      r3, [r3, #-0xc]
00474d70: add      r1, r1, r3
00474d74: ldr      r3, [r1, #4]
00474d78: add      r3, r3, #1
00474d7c: str      r3, [r1, #4]
00474d80: bne      #0x474db0
00474d84: ldr      r3, [pc, #0x9c]
00474d88: mov      r0, r5
00474d8c: mov      r2, r5
00474d90: ldr      r1, [r4, r3]
00474d94: ldr      r3, [pc, #0x90]
00474d98: str      r5, [sp]
00474d9c: ldr      r3, [r4, r3]
00474da0: bl       #0x474cac
00474da4: mov      r0, r5
00474da8: add      sp, sp, #8
00474dac: pop      {r4, r5, r6, pc}
00474db0: ldr      r3, [r5, #4]
00474db4: mov      r0, r3
00474db8: ldr      r3, [r3]
00474dbc: mov      lr, pc
00474dc0: ldr      pc, [r3, #0x74]
00474dc4: b        #0x474da4
00474dc8: ldr      r3, [pc, #0x60]
00474dcc: ldr      r3, [r4, r3]
00474dd0: ldr      r3, [r3]
00474dd4: cmp      r3, #2
00474dd8: streq    r1, [r1]
00474ddc: beq      #0x474d64
00474de0: cmp      r3, #1
00474de4: bne      #0x474d64
00474de8: ldr      r0, [pc, #0x44]
00474dec: ldr      r1, [pc, #0x44]
00474df0: ldr      r2, [pc, #0x44]
00474df4: ldr      r0, [r4, r0]
00474df8: ldr      r3, [pc, #0x40]
00474dfc: add      r1, pc, r1
00474e00: mov      ip, #0x1c
00474e04: add      r0, r0, #0xa8
00474e08: add      r2, pc, r2
00474e0c: add      r3, pc, r3
00474e10: str      ip, [sp]
00474e14: bl       #0x30e004
00474e18: ldr      r1, [r5, #4]
00474e1c: b        #0x474d64
00474e20: subseq   pc, r1, r0, asr sp
00474e24: andeq    r4, r0, r0, lsl r8
00474e28: andeq    r3, r0, r0, ror r7
00474e2c: andeq    r2, r0, r0, lsl #18
00474e30: andeq    r3, r0, r0, asr #19
00474e34: andeq    r1, r0, r0, asr #19
00474e38: ldrdeq   sb, sl, [r4], #-0x5c
00474e3c: subeq    r8, r5, r8, lsr r8
00474e40: subeq    r8, r5, ip, asr sb

# _ZN14AnimController12SetCallbacksEPN6glitch7collada18ISceneNodeAnimatorEPFvPNS0_5scene19ITimelineControllerEPvES7_PFvRKNS1_15STriggeredEventES7_ES7_
00474c78: str      r4, [sp, #-4]!
00474c7c: cmp      r1, #0
00474c80: ldmib    sp, {r4, ip}
00474c84: bne      #0x474c90
00474c88: ldm      sp!, {r4}
00474c8c: bx       lr
00474c90: mov      r0, r1
00474c94: mov      r1, r2
00474c98: mov      r2, r3
00474c9c: mov      r3, r4
00474ca0: str      ip, [sp, #4]
00474ca4: ldm      sp!, {r4}
00474ca8: b        #0x366330

# _ZNK6glitch7collada18SAnimationAccessor15hasDefaultValueEv
00669e54: ldr      r3, [r0]
00669e58: ldr      r0, [r3, #0x18]
00669e5c: subs     r0, r0, #0
00669e60: movne    r0, #1
00669e64: bx       lr

# _ZNK6glitch7collada18SAnimationAccessor10applyValueEiPvPNS0_15animation_track15CApplicatorInfoERib
0066a0c0: push     {r4, r5, r6, r7, r8, lr}
0066a0c4: sub      sp, sp, #0x10
0066a0c8: mov      r7, r1
0066a0cc: mov      r6, r2
0066a0d0: mov      r5, r3
0066a0d4: mov      r8, r0
0066a0d8: ldrb     r4, [sp, #0x2c]
0066a0dc: bl       #0x66a004
0066a0e0: ldr      lr, [sp, #0x28]
0066a0e4: ldr      ip, [r0]
0066a0e8: mov      r1, r8
0066a0ec: mov      r2, r7
0066a0f0: mov      r3, r6
0066a0f4: stm      sp, {r5, lr}
0066a0f8: str      r4, [sp, #8]
0066a0fc: mov      lr, pc
0066a100: ldr      pc, [ip, #0x6c]
0066a104: add      sp, sp, #0x10
0066a108: pop      {r4, r5, r6, r7, r8, pc}

# _ZNK6glitch5scene10ISceneNode16getMaterialCountEv
005970b0: mov      r0, #0
005970b4: bx       lr

# _ZN6glitch7collada14createMaterialERKNS0_16CColladaDatabaseEPNS_5video12IVideoDriverERKN5boost13intrusive_ptrINS4_17CMaterialRendererEEERNS0_9SMaterialEPNS0_14CRootSceneNodeE
00631ce8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00631cec: mov      r2, #0
00631cf0: sub      sp, sp, #0xa4
00631cf4: str      r0, [sp, #0x1c]
00631cf8: str      r3, [sp, #0x18]
00631cfc: str      r2, [r0]
00631d00: ldr      r0, [sp, #0x18]
00631d04: ldr      r1, [pc, #0x69c]
00631d08: ldr      r8, [sp, #0xc8]
00631d0c: ldr      r3, [r0]
00631d10: add      r1, pc, r1
00631d14: str      r1, [sp, #0x28]
00631d18: cmp      r3, r2
00631d1c: beq      #0x631ea8
00631d20: add      r4, sp, #0x9c
00631d24: mov      r3, r2
00631d28: ldr      r1, [sp, #0x18]
00631d2c: ldr      r2, [r8]
00631d30: mov      r0, r4
00631d34: bl       #0x5cc0a0
00631d38: ldr      r3, [sp, #0x9c]
00631d3c: add      r0, sp, #0xa0
00631d40: str      r3, [sp, #0x98]
00631d44: cmp      r3, #0
00631d48: ldrne    r2, [r3]
00631d4c: addne    r2, r2, #1
00631d50: strne    r2, [r3]
00631d54: ldr      sb, [sp, #0x1c]
00631d58: ldrne    r3, [sp, #0x98]
00631d5c: ldr      r2, [sb]
00631d60: str      r3, [sb]
00631d64: str      r2, [r0, #-8]!
00631d68: bl       #0x310be8
00631d6c: mov      r0, r4
00631d70: bl       #0x310be8
00631d74: ldr      sl, [r8, #0x10]
00631d78: cmp      sl, #0
00631d7c: str      sl, [sp, #0x20]
00631d80: ble      #0x631ea8
00631d84: ldr      r3, [pc, #0x620]
00631d88: ldr      r2, [pc, #0x620]
00631d8c: mov      r4, #0
00631d90: add      r3, pc, r3
00631d94: add      r3, r3, #0x4c
00631d98: str      r3, [sp, #0x3c]
00631d9c: ldr      r3, [pc, #0x610]
00631da0: add      r2, pc, r2
00631da4: str      r2, [sp, #0x2c]
00631da8: add      r3, pc, r3
00631dac: str      r3, [sp, #0x34]
00631db0: ldr      r3, [pc, #0x600]
00631db4: mov      r6, r4
00631db8: add      r3, pc, r3
00631dbc: str      r3, [sp, #0x38]
00631dc0: b        #0x631e38
00631dc4: ldr      r1, [sp, #0x1c]
00631dc8: ldr      r0, [r1]
00631dcc: ldr      r1, [r7, #0x10]
00631dd0: ldr      r3, [r0, #4]
00631dd4: ldrh     r2, [r3, #0xe]
00631dd8: cmp      r5, r2
00631ddc: ldrlo    sb, [r3, #0x20]
00631de0: movhs    sb, #0
00631de4: addlo    sb, sb, r5, lsl #4
00631de8: ldr      sl, [sb, #8]
00631dec: str      sl, [sp, #0xc]
00631df0: ldr      r1, [r1]
00631df4: cmp      sl, r1
00631df8: bls      #0x631eb4
00631dfc: ldr      r2, [r0, #0x1c]
00631e00: ldr      r3, [sb]
00631e04: ldr      r1, [pc, #0x5b0]
00631e08: cmp      r2, #0
00631e0c: addne    r2, r2, #4
00631e10: cmp      r3, #0
00631e14: addne    r3, r3, #4
00631e18: add      r1, pc, r1
00631e1c: mov      r0, #3
00631e20: bl       #0x60b034
00631e24: ldr      r3, [sp, #0x20]
00631e28: add      r6, r6, #1
00631e2c: add      r4, r4, #0x18
00631e30: cmp      r6, r3
00631e34: beq      #0x631ea8
00631e38: ldr      r7, [r8, #0x14]
00631e3c: ldr      ip, [sp, #0x18]
00631e40: mov      r2, #0
00631e44: ldr      r1, [r7, r4]
00631e48: ldr      r0, [ip]
00631e4c: bl       #0x5d308c
00631e50: movw     r3, #0xffff
00631e54: cmp      r0, r3
00631e58: add      r7, r7, r4
00631e5c: mov      r5, r0
00631e60: bne      #0x631dc4
00631e64: ldr      r3, [r7, #8]
00631e68: cmp      r3, #0x14
00631e6c: bne      #0x631e24
00631e70: ldr      r3, [r7, #0x14]
00631e74: ldr      r1, [sp, #0x18]
00631e78: add      r6, r6, #1
00631e7c: add      r4, r4, #0x18
00631e80: ldr      r0, [r1]
00631e84: ldr      r1, [r3, #4]
00631e88: bl       #0x5d4714
00631e8c: cmp      r0, #0xff
00631e90: ldrne    r2, [sp, #0x1c]
00631e94: ldrne    r3, [r2]
00631e98: strbne   r0, [r3, #8]
00631e9c: ldr      r3, [sp, #0x20]
00631ea0: cmp      r6, r3
00631ea4: bne      #0x631e38
00631ea8: ldr      r0, [sp, #0x1c]
00631eac: add      sp, sp, #0xa4
00631eb0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00631eb4: ldrb     fp, [sb, #6]
00631eb8: ldr      r1, [sp, #0x2c]
00631ebc: ldr      ip, [r7, #8]
00631ec0: mov      sl, #1
00631ec4: ldr      r1, [r1, fp, lsl #2]
00631ec8: str      ip, [sp, #0x30]
00631ecc: str      ip, [sp, #0x24]
00631ed0: ands     r1, r1, sl, lsl ip
00631ed4: bne      #0x631f4c
00631ed8: ldr      r3, [r0, #0x1c]
00631edc: cmp      r3, #0
00631ee0: addne    r3, r3, #4
00631ee4: str      r3, [sp, #0x30]
00631ee8: ldr      r5, [sb]
00631eec: cmp      r5, #0
00631ef0: addne    r5, r5, #4
00631ef4: cmp      fp, #0xff
00631ef8: beq      #0x631f84
00631efc: mov      r0, #0
00631f00: bl       #0x5e80b4
00631f04: ldr      r7, [r7, #8]
00631f08: ldr      sl, [r0, fp, lsl #2]
00631f0c: str      r7, [sp, #0x24]
00631f10: ldr      r1, [sp, #0x34]
00631f14: add      r0, sp, #0x40
00631f18: mov      r2, #0x58
00631f1c: bl       #0x30e868
00631f20: ldr      r0, [sp, #0x24]
00631f24: add      ip, sp, #0xa0
00631f28: ldr      r2, [sp, #0x30]
00631f2c: add      r3, ip, r0, lsl #2
00631f30: ldr      ip, [r3, #-0x60]
00631f34: mov      r0, #3
00631f38: mov      r3, r5
00631f3c: ldr      r1, [sp, #0x38]
00631f40: stm      sp, {sl, ip}
00631f44: bl       #0x60b034
00631f48: b        #0x631e24
00631f4c: sub      fp, fp, #9
00631f50: cmp      fp, #9
00631f54: addls    pc, pc, fp, lsl #2
00631f58: b        #0x632024
00631f5c: b        #0x631e24
00631f60: b        #0x631e24
00631f64: b        #0x632080
00631f68: b        #0x63215c
00631f6c: b        #0x6321ec
00631f70: b        #0x63227c
00631f74: b        #0x63230c
00631f78: b        #0x632024
00631f7c: b        #0x632024
00631f80: b        #0x631f90
00631f84: ldr      sl, [pc, #0x434]
00631f88: add      sl, pc, sl
00631f8c: b        #0x631f10
00631f90: ldr      r1, [sp, #0xc]
00631f94: cmp      r1, #0
00631f98: beq      #0x631e24
00631f9c: add      sl, sp, #0x40
00631fa0: mov      fp, #0
00631fa4: str      sl, [sp, #0x24]
00631fa8: mov      sb, fp
00631fac: ldr      sl, [sp, #0xc]
00631fb0: b        #0x631fec
00631fb4: ldr      ip, [sp, #0xcc]
00631fb8: cmp      ip, #0
00631fbc: beq      #0x631fdc
00631fc0: ldr      ip, [sp, #0x24]
00631fc4: ldr      r0, [sp, #0xcc]
00631fc8: ldr      r1, [sp, #0x1c]
00631fcc: mov      r2, r5
00631fd0: mov      r3, sb
00631fd4: str      ip, [sp]
00631fd8: bl       #0x65b0fc
00631fdc: add      sb, sb, #1
00631fe0: cmp      sb, sl
00631fe4: add      fp, fp, #4
00631fe8: beq      #0x631e24
00631fec: ldr      r3, [r7, #0x14]
00631ff0: add      r3, r3, fp
00631ff4: ldr      r3, [r3]
00631ff8: str      r3, [sp, #0x40]
00631ffc: ldr      r2, [r3, #-4]
00632000: cmp      r2, #0
00632004: beq      #0x631e24
00632008: ldrsb    r2, [r3]
0063200c: cmp      r2, #0x23
00632010: bne      #0x631fb4
00632014: ldrsb    r3, [r3, #1]
00632018: cmp      r3, #0
0063201c: beq      #0x631e24
00632020: b        #0x631fb4
00632024: ldr      r1, [sp, #0x28]
00632028: ldr      r3, [pc, #0x394]
0063202c: ldr      sb, [sp, #0x30]
00632030: ldr      sl, [sp, #0x28]
00632034: ldr      r2, [r1, r3]
00632038: ldr      r1, [pc, #0x388]
0063203c: add      r3, sb, #1
00632040: ldr      ip, [sp, #0x28]
00632044: ldr      r1, [sl, r1]
00632048: ldr      sl, [r2, r3, lsl #2]
0063204c: ldr      r2, [pc, #0x378]
00632050: ldrb     r1, [r1, sl]
00632054: ldr      r2, [ip, r2]
00632058: ldr      ip, [sp, #0x3c]
0063205c: ldr      lr, [ip, sb, lsl #2]
00632060: ldrb     ip, [r2, r3]
00632064: ldr      r3, [r7, #0x14]
00632068: mov      r2, lr
0063206c: mul      ip, ip, r1
00632070: mov      r1, r5
00632074: str      ip, [sp]
00632078: bl       #0x5ccf40
0063207c: b        #0x631e24
00632080: add      fp, sp, #0x40
00632084: mov      r0, fp
00632088: bl       #0x631c14
0063208c: ldr      sl, [sp, #0x28]
00632090: ldr      r3, [pc, #0x32c]
00632094: ldr      r2, [r7, #8]
00632098: ldr      ip, [sb, #8]
0063209c: ldr      r1, [sl, r3]
006320a0: ldr      r3, [pc, #0x320]
006320a4: add      r2, r2, #1
006320a8: ldr      r1, [r1, r2, lsl #2]
006320ac: ldr      r0, [sl, r3]
006320b0: ldr      r3, [pc, #0x314]
006320b4: cmp      ip, #0
006320b8: ldrb     r1, [r0, r1]
006320bc: ldr      r3, [sl, r3]
006320c0: ldrb     r3, [r3, r2]
006320c4: mul      r3, r3, r1
006320c8: beq      #0x631e24
006320cc: mov      sb, #0
006320d0: str      r4, [sp, #0x24]
006320d4: str      r6, [sp, #0x30]
006320d8: mov      sl, sb
006320dc: mov      r6, r5
006320e0: mov      r4, r3
006320e4: mov      r5, ip
006320e8: b        #0x6320fc
006320ec: add      sl, sl, #1
006320f0: cmp      sl, r5
006320f4: add      sb, sb, r4
006320f8: beq      #0x63239c
006320fc: ldr      lr, [r7, #0x14]
00632100: mov      ip, #0
00632104: strb     ip, [sp, #0x80]
00632108: add      lr, lr, sb
0063210c: mov      ip, fp
00632110: ldm      lr!, {r0, r1, r2, r3}
00632114: stm      ip!, {r0, r1, r2, r3}
00632118: ldm      lr!, {r0, r1, r2, r3}
0063211c: stm      ip!, {r0, r1, r2, r3}
00632120: ldm      lr!, {r0, r1, r2, r3}
00632124: stm      ip!, {r0, r1, r2, r3}
00632128: ldm      lr, {r0, r1, r2, r3}
0063212c: stm      ip, {r0, r1, r2, r3}
00632130: mov      r0, fp
00632134: bl       #0x5ba19c
00632138: cmp      r0, #0
0063213c: bne      #0x6320ec
00632140: ldr      r3, [sp, #0x1c]
00632144: mov      r2, sl
00632148: mov      r1, r6
0063214c: ldr      r0, [r3]
00632150: mov      r3, fp
00632154: bl       #0x5cb4dc
00632158: b        #0x6320ec
0063215c: cmp      r5, r2
00632160: ldrlo    r3, [r3, #0x20]
00632164: movhs    r3, #0
00632168: ldr      sl, [r7, #0x14]
0063216c: addlo    r3, r3, r5, lsl #4
00632170: ldr      sb, [r3, #8]
00632174: cmp      sb, #0
00632178: beq      #0x631e24
0063217c: str      r4, [sp, #0x24]
00632180: ldr      r4, [sp, #0x1c]
00632184: mov      r7, #0
00632188: add      fp, sp, #0x40
0063218c: ldr      r0, [sl, r7, lsl #2]
00632190: mov      r2, r7
00632194: mov      r1, r5
00632198: ldr      r0, [r0]
0063219c: mov      r3, fp
006321a0: add      r7, r7, #1
006321a4: cmp      r0, #0
006321a8: beq      #0x6321dc
006321ac: ldr      r0, [r0, #0x10]
006321b0: cmp      r0, #0
006321b4: str      r0, [sp, #0x40]
006321b8: ldrne    ip, [r0, #4]
006321bc: addne    ip, ip, #1
006321c0: strne    ip, [r0, #4]
006321c4: ldr      r0, [r4]
006321c8: bl       #0x5cd324
006321cc: ldr      r0, [sp, #0x40]
006321d0: cmp      r0, #0
006321d4: beq      #0x6321dc
006321d8: bl       #0x31d584
006321dc: cmp      r7, sb
006321e0: bne      #0x63218c
006321e4: ldr      r4, [sp, #0x24]
006321e8: b        #0x631e24
006321ec: cmp      r5, r2
006321f0: ldrlo    r3, [r3, #0x20]
006321f4: movhs    r3, #0
006321f8: ldr      sl, [r7, #0x14]
006321fc: addlo    r3, r3, r5, lsl #4
00632200: ldr      sb, [r3, #8]
00632204: cmp      sb, #0
00632208: beq      #0x631e24
0063220c: str      r4, [sp, #0x24]
00632210: ldr      r4, [sp, #0x1c]
00632214: mov      r7, #0
00632218: add      fp, sp, #0x40
0063221c: ldr      r0, [sl, r7, lsl #2]
00632220: mov      r2, r7
00632224: mov      r1, r5
00632228: ldr      r0, [r0]
0063222c: mov      r3, fp
00632230: add      r7, r7, #1
00632234: cmp      r0, #0
00632238: beq      #0x63226c
0063223c: ldr      r0, [r0, #0x10]
00632240: cmp      r0, #0
00632244: str      r0, [sp, #0x40]
00632248: ldrne    ip, [r0, #4]
0063224c: addne    ip, ip, #1
00632250: strne    ip, [r0, #4]
00632254: ldr      r0, [r4]
00632258: bl       #0x5cd324
0063225c: ldr      r0, [sp, #0x40]
00632260: cmp      r0, #0
00632264: beq      #0x63226c
00632268: bl       #0x31d584
0063226c: cmp      r7, sb
00632270: bne      #0x63221c
00632274: ldr      r4, [sp, #0x24]
00632278: b        #0x631e24
0063227c: cmp      r5, r2
00632280: ldrlo    r3, [r3, #0x20]
00632284: movhs    r3, #0
00632288: ldr      sl, [r7, #0x14]
0063228c: addlo    r3, r3, r5, lsl #4
00632290: ldr      sb, [r3, #8]
00632294: cmp      sb, #0
00632298: beq      #0x631e24
0063229c: str      r4, [sp, #0x24]
006322a0: ldr      r4, [sp, #0x1c]
006322a4: mov      r7, #0
006322a8: add      fp, sp, #0x40
006322ac: ldr      r0, [sl, r7, lsl #2]
006322b0: mov      r2, r7
006322b4: mov      r1, r5
006322b8: ldr      r0, [r0]
006322bc: mov      r3, fp
006322c0: add      r7, r7, #1
006322c4: cmp      r0, #0
006322c8: beq      #0x6322fc
006322cc: ldr      r0, [r0, #0x10]
006322d0: cmp      r0, #0
006322d4: str      r0, [sp, #0x40]
006322d8: ldrne    ip, [r0, #4]
006322dc: addne    ip, ip, #1
006322e0: strne    ip, [r0, #4]
006322e4: ldr      r0, [r4]
006322e8: bl       #0x5cd324
006322ec: ldr      r0, [sp, #0x40]
006322f0: cmp      r0, #0
006322f4: beq      #0x6322fc
006322f8: bl       #0x31d584
006322fc: cmp      r7, sb
00632300: bne      #0x6322ac
00632304: ldr      r4, [sp, #0x24]
00632308: b        #0x631e24
0063230c: cmp      r5, r2
00632310: ldrlo    r3, [r3, #0x20]
00632314: movhs    r3, #0
00632318: ldr      sl, [r7, #0x14]
0063231c: addlo    r3, r3, r5, lsl #4
00632320: ldr      sb, [r3, #8]
00632324: cmp      sb, #0
00632328: beq      #0x631e24
0063232c: str      r4, [sp, #0x24]
00632330: ldr      r4, [sp, #0x1c]
00632334: mov      r7, #0
00632338: add      fp, sp, #0x40
0063233c: ldr      r0, [sl, r7, lsl #2]
00632340: mov      r2, r7
00632344: mov      r1, r5
00632348: ldr      r0, [r0]
0063234c: mov      r3, fp
00632350: add      r7, r7, #1
00632354: cmp      r0, #0
00632358: beq      #0x63238c
0063235c: ldr      r0, [r0, #0x10]
00632360: cmp      r0, #0
00632364: str      r0, [sp, #0x40]
00632368: ldrne    ip, [r0, #4]
0063236c: addne    ip, ip, #1
00632370: strne    ip, [r0, #4]
00632374: ldr      r0, [r4]
00632378: bl       #0x5cd324
0063237c: ldr      r0, [sp, #0x40]
00632380: cmp      r0, #0
00632384: beq      #0x63238c
00632388: bl       #0x31d584
0063238c: cmp      r7, sb
00632390: bne      #0x63233c
00632394: ldr      r4, [sp, #0x24]
00632398: b        #0x631e24
0063239c: ldr      r4, [sp, #0x24]
006323a0: ldr      r6, [sp, #0x30]
006323a4: b        #0x631e24
006323a8: eorseq   r2, r6, r0, lsl #27
006323ac: strhteq  r3, [fp], -ip
006323b0: eoreq    r3, fp, ip, lsr #1
006323b4: eorseq   r5, r2, r0, asr #15
006323b8: eoreq    r3, fp, r0, ror #3
006323bc: eoreq    r3, fp, r0, asr r1
