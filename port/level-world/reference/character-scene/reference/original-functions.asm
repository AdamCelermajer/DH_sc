
# _ZNK6glitch4core8CMatrix4IfE12transformBoxERNS0_8aabbox3dIfEE
00587398: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
0058739c: ldrb     r3, [r0, #0x40]
005873a0: mov      r4, r0
005873a4: mov      r5, r1
005873a8: cmp      r3, #0
005873ac: bne      #0x587600
005873b0: ldr      sl, [r1]
005873b4: ldr      r7, [r1, #4]
005873b8: ldr      r1, [r0]
005873bc: mov      r0, sl
005873c0: bl       #0x30ed6c
005873c4: ldr      r1, [r4, #0x10]
005873c8: mov      r8, r0
005873cc: mov      r0, r7
005873d0: bl       #0x30ed6c
005873d4: mov      r1, r0
005873d8: mov      r0, r8
005873dc: bl       #0x30eba4
005873e0: ldr      r6, [r5, #8]
005873e4: ldr      r1, [r4, #0x20]
005873e8: mov      r8, r0
005873ec: mov      r0, r6
005873f0: bl       #0x30ed6c
005873f4: mov      r1, r0
005873f8: mov      r0, r8
005873fc: bl       #0x30eba4
00587400: ldr      r1, [r4, #0x30]
00587404: bl       #0x30eba4
00587408: ldr      r1, [r4, #4]
0058740c: mov      sb, r0
00587410: mov      r0, sl
00587414: bl       #0x30ed6c
00587418: ldr      r1, [r4, #0x14]
0058741c: mov      r8, r0
00587420: mov      r0, r7
00587424: bl       #0x30ed6c
00587428: mov      r1, r0
0058742c: mov      r0, r8
00587430: bl       #0x30eba4
00587434: ldr      r1, [r4, #0x24]
00587438: mov      r8, r0
0058743c: mov      r0, r6
00587440: bl       #0x30ed6c
00587444: mov      r1, r0
00587448: mov      r0, r8
0058744c: bl       #0x30eba4
00587450: ldr      r1, [r4, #0x34]
00587454: bl       #0x30eba4
00587458: ldr      r1, [r4, #8]
0058745c: mov      r8, r0
00587460: mov      r0, sl
00587464: bl       #0x30ed6c
00587468: ldr      r1, [r4, #0x18]
0058746c: mov      sl, r0
00587470: mov      r0, r7
00587474: bl       #0x30ed6c
00587478: mov      r1, r0
0058747c: mov      r0, sl
00587480: bl       #0x30eba4
00587484: ldr      r1, [r4, #0x28]
00587488: mov      r7, r0
0058748c: mov      r0, r6
00587490: bl       #0x30ed6c
00587494: mov      r1, r0
00587498: mov      r0, r7
0058749c: bl       #0x30eba4
005874a0: ldr      r1, [r4, #0x38]
005874a4: bl       #0x30eba4
005874a8: str      sb, [r5]
005874ac: ldr      r6, [r5, #0xc]
005874b0: str      r0, [r5, #8]
005874b4: str      r8, [r5, #4]
005874b8: ldr      r1, [r4]
005874bc: mov      r7, r0
005874c0: mov      r0, r6
005874c4: bl       #0x30ed6c
005874c8: ldr      r1, [r4, #0x10]
005874cc: mov      sl, r0
005874d0: ldr      r0, [r5, #0x10]
005874d4: bl       #0x30ed6c
005874d8: mov      r1, r0
005874dc: mov      r0, sl
005874e0: bl       #0x30eba4
005874e4: ldr      r1, [r4, #0x20]
005874e8: mov      sl, r0
005874ec: ldr      r0, [r5, #0x14]
005874f0: bl       #0x30ed6c
005874f4: mov      r1, r0
005874f8: mov      r0, sl
005874fc: bl       #0x30eba4
00587500: ldr      r1, [r4, #0x30]
00587504: bl       #0x30eba4
00587508: ldr      r1, [r4, #4]
0058750c: mov      fp, r0
00587510: mov      r0, r6
00587514: bl       #0x30ed6c
00587518: ldr      r1, [r4, #0x14]
0058751c: mov      sl, r0
00587520: ldr      r0, [r5, #0x10]
00587524: bl       #0x30ed6c
00587528: mov      r1, r0
0058752c: mov      r0, sl
00587530: bl       #0x30eba4
00587534: ldr      r1, [r4, #0x24]
00587538: mov      sl, r0
0058753c: ldr      r0, [r5, #0x14]
00587540: bl       #0x30ed6c
00587544: mov      r1, r0
00587548: mov      r0, sl
0058754c: bl       #0x30eba4
00587550: ldr      r1, [r4, #0x34]
00587554: bl       #0x30eba4
00587558: ldr      r1, [r4, #8]
0058755c: mov      sl, r0
00587560: mov      r0, r6
00587564: bl       #0x30ed6c
00587568: ldr      r1, [r4, #0x18]
0058756c: mov      r6, r0
00587570: ldr      r0, [r5, #0x10]
00587574: bl       #0x30ed6c
00587578: mov      r1, r0
0058757c: mov      r0, r6
00587580: bl       #0x30eba4
00587584: ldr      r1, [r4, #0x28]
00587588: mov      r6, r0
0058758c: ldr      r0, [r5, #0x14]
00587590: bl       #0x30ed6c
00587594: mov      r1, r0
00587598: mov      r0, r6
0058759c: bl       #0x30eba4
005875a0: ldr      r1, [r4, #0x38]
005875a4: bl       #0x30eba4
005875a8: str      fp, [r5, #0xc]
005875ac: mov      r4, r0
005875b0: str      r0, [r5, #0x14]
005875b4: mov      r1, fp
005875b8: str      sl, [r5, #0x10]
005875bc: mov      r0, sb
005875c0: bl       #0x30e2f8
005875c4: cmp      r0, #0
005875c8: strne    fp, [r5]
005875cc: strne    sb, [r5, #0xc]
005875d0: mov      r1, sl
005875d4: mov      r0, r8
005875d8: bl       #0x30e2f8
005875dc: cmp      r0, #0
005875e0: strne    sl, [r5, #4]
005875e4: strne    r8, [r5, #0x10]
005875e8: mov      r0, r7
005875ec: mov      r1, r4
005875f0: bl       #0x30e2f8
005875f4: cmp      r0, #0
005875f8: strne    r7, [r5, #0x14]
005875fc: strne    r4, [r5, #8]
00587600: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch7collada6detail33CColladaHardwareQuatSkinTechnique18computeBoundingBoxEv
0066e384: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066e388: mov      r4, r0
0066e38c: mov      sl, r1
0066e390: mov      r0, r1
0066e394: sub      sp, sp, #0x24
0066e398: bl       #0x66df88
0066e39c: ldr      r2, [sl, #0x10]
0066e3a0: mvn      r3, #0x80000000
0066e3a4: sub      r3, r3, #0x800000
0066e3a8: mvn      r1, #0x800000
0066e3ac: ldr      r0, [r2, #0x10]
0066e3b0: ldr      sb, [r2, #0x14]
0066e3b4: str      r3, [r4, #8]
0066e3b8: str      r1, [r4, #0xc]
0066e3bc: str      r1, [r4, #0x10]
0066e3c0: str      r1, [r4, #0x14]
0066e3c4: str      r3, [r4]
0066e3c8: str      r3, [r4, #4]
0066e3cc: ldr      r3, [sl, #0xc]
0066e3d0: rsb      sb, r0, sb
0066e3d4: ubfx     sb, sb, #2, #8
0066e3d8: ldr      r6, [r3, #0x8c]
0066e3dc: cmp      r6, #0
0066e3e0: bne      #0x66e4a8
0066e3e4: cmp      sb, #0
0066e3e8: bne      #0x66e40c
0066e3ec: ldr      r3, [sl, #0x10]
0066e3f0: mov      r0, r4
0066e3f4: ldr      r2, [r3]
0066e3f8: bic      r2, r2, #8
0066e3fc: str      r2, [r3]
0066e400: add      sp, sp, #0x24
0066e404: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066e408: ldr      r1, [r4, #0xc]
0066e40c: ldr      r3, [sl, #0x10]
0066e410: ldr      r3, [r3, #0x10]
0066e414: ldr      r3, [r3, r6, lsl #2]
0066e418: add      r6, r6, #1
0066e41c: ldr      r8, [r3, #0x30]
0066e420: ldr      r5, [r3, #0x38]
0066e424: ldr      r7, [r3, #0x34]
0066e428: mov      r0, r8
0066e42c: bl       #0x30e2f8
0066e430: cmp      r0, #0
0066e434: ldr      r1, [r4, #0x10]
0066e438: strne    r8, [r4, #0xc]
0066e43c: mov      r0, r7
0066e440: bl       #0x30e2f8
0066e444: cmp      r0, #0
0066e448: ldr      r1, [r4, #0x14]
0066e44c: strne    r7, [r4, #0x10]
0066e450: mov      r0, r5
0066e454: bl       #0x30e2f8
0066e458: cmp      r0, #0
0066e45c: ldr      r1, [r4]
0066e460: strne    r5, [r4, #0x14]
0066e464: mov      r0, r8
0066e468: bl       #0x30e70c
0066e46c: cmp      r0, #0
0066e470: ldr      r1, [r4, #4]
0066e474: strne    r8, [r4]
0066e478: mov      r0, r7
0066e47c: bl       #0x30e70c
0066e480: cmp      r0, #0
0066e484: strne    r7, [r4, #4]
0066e488: ldr      r1, [r4, #8]
0066e48c: mov      r0, r5
0066e490: bl       #0x30e70c
0066e494: cmp      r0, #0
0066e498: strne    r5, [r4, #8]
0066e49c: cmp      r6, sb
0066e4a0: blt      #0x66e408
0066e4a4: b        #0x66e3ec
0066e4a8: cmp      sb, #0
0066e4ac: beq      #0x66e3ec
0066e4b0: mov      r5, #0
0066e4b4: add      r2, sp, #8
0066e4b8: mov      r7, r5
0066e4bc: str      r2, [sp, #4]
0066e4c0: b        #0x66e4c8
0066e4c4: ldr      r3, [sl, #0xc]
0066e4c8: ldr      r3, [r3, #0x90]
0066e4cc: ldr      r0, [sl, #0x10]
0066e4d0: ldr      r1, [sp, #4]
0066e4d4: ldr      ip, [r3, r5]
0066e4d8: add      r3, r3, r5
0066e4dc: add      r2, r3, #0xc
0066e4e0: str      ip, [sp, #8]
0066e4e4: ldr      ip, [r3, #4]
0066e4e8: add      r5, r5, #0x18
0066e4ec: str      ip, [sp, #0xc]
0066e4f0: ldr      ip, [r3, #8]
0066e4f4: str      ip, [sp, #0x10]
0066e4f8: ldr      r3, [r3, #0xc]
0066e4fc: str      r3, [sp, #0x14]
0066e500: ldr      r3, [r2, #4]
0066e504: str      r3, [sp, #0x18]
0066e508: ldr      r3, [r2, #8]
0066e50c: str      r3, [sp, #0x1c]
0066e510: ldr      r3, [r0, #0x10]
0066e514: ldr      r0, [r3, r7, lsl #2]
0066e518: bl       #0x587398
0066e51c: ldr      fp, [sp, #0x14]
0066e520: ldr      r1, [r4, #0xc]
0066e524: ldr      r8, [sp, #0x18]
0066e528: mov      r0, fp
0066e52c: bl       #0x30e2f8
0066e530: cmp      r0, #0
0066e534: ldr      r6, [sp, #0x1c]
0066e538: ldr      r1, [r4, #0x10]
0066e53c: strne    fp, [r4, #0xc]
0066e540: mov      r0, r8
0066e544: bl       #0x30e2f8
0066e548: cmp      r0, #0
0066e54c: ldr      r1, [r4, #0x14]
0066e550: strne    r8, [r4, #0x10]
0066e554: mov      r0, r6
0066e558: bl       #0x30e2f8
0066e55c: cmp      r0, #0
0066e560: ldr      r1, [r4]
0066e564: strne    r6, [r4, #0x14]
0066e568: mov      r0, fp
0066e56c: bl       #0x30e70c
0066e570: cmp      r0, #0
0066e574: ldr      r1, [r4, #4]
0066e578: strne    fp, [r4]
0066e57c: mov      r0, r8
0066e580: bl       #0x30e70c
0066e584: cmp      r0, #0
0066e588: ldr      r1, [r4, #8]
0066e58c: strne    r8, [r4, #4]
0066e590: mov      r0, r6
0066e594: bl       #0x30e70c
0066e598: cmp      r0, #0
0066e59c: strne    r6, [r4, #8]
0066e5a0: ldr      fp, [sp, #8]
0066e5a4: ldr      r1, [r4, #0xc]
0066e5a8: ldr      r8, [sp, #0xc]
0066e5ac: mov      r0, fp
0066e5b0: bl       #0x30e2f8
0066e5b4: cmp      r0, #0
0066e5b8: ldr      r6, [sp, #0x10]
0066e5bc: ldr      r1, [r4, #0x10]
0066e5c0: strne    fp, [r4, #0xc]
0066e5c4: mov      r0, r8
0066e5c8: bl       #0x30e2f8
0066e5cc: cmp      r0, #0
0066e5d0: ldr      r1, [r4, #0x14]
0066e5d4: strne    r8, [r4, #0x10]
0066e5d8: mov      r0, r6
0066e5dc: bl       #0x30e2f8
0066e5e0: cmp      r0, #0
0066e5e4: ldr      r1, [r4]
0066e5e8: strne    r6, [r4, #0x14]
0066e5ec: mov      r0, fp
0066e5f0: bl       #0x30e70c
0066e5f4: cmp      r0, #0
0066e5f8: ldr      r1, [r4, #4]
0066e5fc: strne    fp, [r4]
0066e600: mov      r0, r8
0066e604: bl       #0x30e70c
0066e608: cmp      r0, #0
0066e60c: strne    r8, [r4, #4]
0066e610: ldr      r1, [r4, #8]
0066e614: mov      r0, r6
0066e618: bl       #0x30e70c
0066e61c: add      r7, r7, #1
0066e620: cmp      r0, #0
0066e624: strne    r6, [r4, #8]
0066e628: cmp      r7, sb
0066e62c: blt      #0x66e4c4
0066e630: b        #0x66e3ec

# _ZN10GameObject11SetPositionERK7Point3DIfEb
00393db4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00393db8: ldr      r5, [r0, #0x2e0]
00393dbc: mov      r4, r0
00393dc0: mov      r6, r1
00393dc4: cmp      r5, #0
00393dc8: mov      r7, r2
00393dcc: beq      #0x393e2c
00393dd0: ldr      r1, [r0, #0x164]
00393dd4: ldr      r0, [r6, #4]
00393dd8: bl       #0x30e3ac
00393ddc: ldr      r1, [r4, #0x168]
00393de0: mov      sl, r0
00393de4: ldr      r0, [r6, #8]
00393de8: bl       #0x30e3ac
00393dec: ldr      r1, [r4, #0x160]
00393df0: mov      r8, r0
00393df4: ldr      r0, [r6]
00393df8: bl       #0x30e3ac
00393dfc: mov      r1, r0
00393e00: ldr      r0, [r5, #0xc]
00393e04: bl       #0x30eba4
00393e08: mov      r1, sl
00393e0c: str      r0, [r5, #0xc]
00393e10: ldr      r0, [r5, #0x10]
00393e14: bl       #0x30eba4
00393e18: mov      r1, r8
00393e1c: str      r0, [r5, #0x10]
00393e20: ldr      r0, [r5, #0x14]
00393e24: bl       #0x30eba4
00393e28: str      r0, [r5, #0x14]
00393e2c: ldr      r3, [r6]
00393e30: mov      r0, r4
00393e34: str      r3, [r4, #0x160]
00393e38: ldr      r3, [r6, #4]
00393e3c: str      r3, [r4, #0x164]
00393e40: ldr      r3, [r6, #8]
00393e44: str      r3, [r4, #0x168]
00393e48: bl       #0x38aac8
00393e4c: ldr      r0, [r4, #0x2dc]
00393e50: cmp      r0, #0
00393e54: beq      #0x393e64
00393e58: ldr      r1, [r4, #0x160]
00393e5c: ldr      r2, [r4, #0x164]
00393e60: bl       #0x46ea80
00393e64: ldr      r0, [r4, #0x2d8]
00393e68: cmp      r0, #0
00393e6c: beq      #0x393e74
00393e70: bl       #0x470cb8
00393e74: cmp      r7, #0
00393e78: bne      #0x393e80
00393e7c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00393e80: mov      r0, r4
00393e84: mov      r1, r6
00393e88: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
00393e8c: b        #0x393600

# _ZN6glitch7collada6detail35CColladaHardwareMatrixSkinTechnique18computeBoundingBoxEv
0066c5f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066c5f8: mov      r4, r0
0066c5fc: mov      sl, r1
0066c600: mov      r0, r1
0066c604: sub      sp, sp, #0x24
0066c608: bl       #0x66c544
0066c60c: ldr      r2, [sl, #0x10]
0066c610: mvn      r3, #0x80000000
0066c614: sub      r3, r3, #0x800000
0066c618: mvn      r1, #0x800000
0066c61c: ldr      r0, [r2, #0x10]
0066c620: ldr      sb, [r2, #0x14]
0066c624: str      r3, [r4, #8]
0066c628: str      r1, [r4, #0xc]
0066c62c: str      r1, [r4, #0x10]
0066c630: str      r1, [r4, #0x14]
0066c634: str      r3, [r4]
0066c638: str      r3, [r4, #4]
0066c63c: ldr      r3, [sl, #0xc]
0066c640: rsb      sb, r0, sb
0066c644: ubfx     sb, sb, #2, #8
0066c648: ldr      r6, [r3, #0x8c]
0066c64c: cmp      r6, #0
0066c650: bne      #0x66c718
0066c654: cmp      sb, #0
0066c658: bne      #0x66c67c
0066c65c: ldr      r3, [sl, #0x10]
0066c660: mov      r0, r4
0066c664: ldr      r2, [r3]
0066c668: bic      r2, r2, #8
0066c66c: str      r2, [r3]
0066c670: add      sp, sp, #0x24
0066c674: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066c678: ldr      r1, [r4, #0xc]
0066c67c: ldr      r3, [sl, #0x10]
0066c680: ldr      r3, [r3, #0x10]
0066c684: ldr      r3, [r3, r6, lsl #2]
0066c688: add      r6, r6, #1
0066c68c: ldr      r8, [r3, #0x30]
0066c690: ldr      r5, [r3, #0x38]
0066c694: ldr      r7, [r3, #0x34]
0066c698: mov      r0, r8
0066c69c: bl       #0x30e2f8
0066c6a0: cmp      r0, #0
0066c6a4: ldr      r1, [r4, #0x10]
0066c6a8: strne    r8, [r4, #0xc]
0066c6ac: mov      r0, r7
0066c6b0: bl       #0x30e2f8
0066c6b4: cmp      r0, #0
0066c6b8: ldr      r1, [r4, #0x14]
0066c6bc: strne    r7, [r4, #0x10]
0066c6c0: mov      r0, r5
0066c6c4: bl       #0x30e2f8
0066c6c8: cmp      r0, #0
0066c6cc: ldr      r1, [r4]
0066c6d0: strne    r5, [r4, #0x14]
0066c6d4: mov      r0, r8
0066c6d8: bl       #0x30e70c
0066c6dc: cmp      r0, #0
0066c6e0: ldr      r1, [r4, #4]
0066c6e4: strne    r8, [r4]
0066c6e8: mov      r0, r7
0066c6ec: bl       #0x30e70c
0066c6f0: cmp      r0, #0
0066c6f4: strne    r7, [r4, #4]
0066c6f8: ldr      r1, [r4, #8]
0066c6fc: mov      r0, r5
0066c700: bl       #0x30e70c
0066c704: cmp      r0, #0
0066c708: strne    r5, [r4, #8]
0066c70c: cmp      r6, sb
0066c710: blt      #0x66c678
0066c714: b        #0x66c65c
0066c718: cmp      sb, #0
0066c71c: beq      #0x66c65c
0066c720: mov      r5, #0
0066c724: add      r2, sp, #8
0066c728: mov      r7, r5
0066c72c: str      r2, [sp, #4]
0066c730: b        #0x66c738
0066c734: ldr      r3, [sl, #0xc]
0066c738: ldr      r3, [r3, #0x90]
0066c73c: ldr      r0, [sl, #0x10]
0066c740: ldr      r1, [sp, #4]
0066c744: ldr      ip, [r3, r5]
0066c748: add      r3, r3, r5
0066c74c: add      r2, r3, #0xc
0066c750: str      ip, [sp, #8]
0066c754: ldr      ip, [r3, #4]
0066c758: add      r5, r5, #0x18
0066c75c: str      ip, [sp, #0xc]
0066c760: ldr      ip, [r3, #8]
0066c764: str      ip, [sp, #0x10]
0066c768: ldr      r3, [r3, #0xc]
0066c76c: str      r3, [sp, #0x14]
0066c770: ldr      r3, [r2, #4]
0066c774: str      r3, [sp, #0x18]
0066c778: ldr      r3, [r2, #8]
0066c77c: str      r3, [sp, #0x1c]
0066c780: ldr      r3, [r0, #0x10]
0066c784: ldr      r0, [r3, r7, lsl #2]
0066c788: bl       #0x587398
0066c78c: ldr      fp, [sp, #0x14]
0066c790: ldr      r1, [r4, #0xc]
0066c794: ldr      r8, [sp, #0x18]
0066c798: mov      r0, fp
0066c79c: bl       #0x30e2f8
0066c7a0: cmp      r0, #0
0066c7a4: ldr      r6, [sp, #0x1c]
0066c7a8: ldr      r1, [r4, #0x10]
0066c7ac: strne    fp, [r4, #0xc]
0066c7b0: mov      r0, r8
0066c7b4: bl       #0x30e2f8
0066c7b8: cmp      r0, #0
0066c7bc: ldr      r1, [r4, #0x14]
0066c7c0: strne    r8, [r4, #0x10]
0066c7c4: mov      r0, r6
0066c7c8: bl       #0x30e2f8
0066c7cc: cmp      r0, #0
0066c7d0: ldr      r1, [r4]
0066c7d4: strne    r6, [r4, #0x14]
0066c7d8: mov      r0, fp
0066c7dc: bl       #0x30e70c
0066c7e0: cmp      r0, #0
0066c7e4: ldr      r1, [r4, #4]
0066c7e8: strne    fp, [r4]
0066c7ec: mov      r0, r8
0066c7f0: bl       #0x30e70c
0066c7f4: cmp      r0, #0
0066c7f8: ldr      r1, [r4, #8]
0066c7fc: strne    r8, [r4, #4]
0066c800: mov      r0, r6
0066c804: bl       #0x30e70c
0066c808: cmp      r0, #0
0066c80c: strne    r6, [r4, #8]
0066c810: ldr      fp, [sp, #8]
0066c814: ldr      r1, [r4, #0xc]
0066c818: ldr      r8, [sp, #0xc]
0066c81c: mov      r0, fp
0066c820: bl       #0x30e2f8
0066c824: cmp      r0, #0
0066c828: ldr      r6, [sp, #0x10]
0066c82c: ldr      r1, [r4, #0x10]
0066c830: strne    fp, [r4, #0xc]
0066c834: mov      r0, r8
0066c838: bl       #0x30e2f8
0066c83c: cmp      r0, #0
0066c840: ldr      r1, [r4, #0x14]
0066c844: strne    r8, [r4, #0x10]
0066c848: mov      r0, r6
0066c84c: bl       #0x30e2f8
0066c850: cmp      r0, #0
0066c854: ldr      r1, [r4]
0066c858: strne    r6, [r4, #0x14]
0066c85c: mov      r0, fp
0066c860: bl       #0x30e70c
0066c864: cmp      r0, #0
0066c868: ldr      r1, [r4, #4]
0066c86c: strne    fp, [r4]
0066c870: mov      r0, r8
0066c874: bl       #0x30e70c
0066c878: cmp      r0, #0
0066c87c: strne    r8, [r4, #4]
0066c880: ldr      r1, [r4, #8]
0066c884: mov      r0, r6
0066c888: bl       #0x30e70c
0066c88c: add      r7, r7, #1
0066c890: cmp      r0, #0
0066c894: strne    r6, [r4, #8]
0066c898: cmp      r7, sb
0066c89c: blt      #0x66c734
0066c8a0: b        #0x66c65c

# _ZN9Character8InitPostEv
003b4d60: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003b4d64: ldr      r5, [pc, #0x828]
003b4d68: ldr      r6, [pc, #0x828]
003b4d6c: movw     r3, #0x1394
003b4d70: add      r5, pc, r5
003b4d74: ldr      r2, [r5, r6]
003b4d78: ldrb     r7, [r0, r3]
003b4d7c: sub      sp, sp, #0xfc
003b4d80: ldr      r2, [r2]
003b4d84: cmp      r7, #0
003b4d88: mov      r4, r0
003b4d8c: str      r2, [sp, #0xf4]
003b4d90: beq      #0x3b4db0
003b4d94: ldr      r3, [r5, r6]
003b4d98: ldr      r2, [sp, #0xf4]
003b4d9c: ldr      r3, [r3]
003b4da0: cmp      r2, r3
003b4da4: bne      #0x3b5590
003b4da8: add      sp, sp, #0xfc
003b4dac: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
003b4db0: mov      r2, #1
003b4db4: strb     r2, [r0, r3]
003b4db8: bl       #0x38bd64
003b4dbc: ldr      r3, [r4, #0x274]
003b4dc0: cmp      r0, r3
003b4dc4: bge      #0x3b4d94
003b4dc8: ldr      fp, [pc, #0x7cc]
003b4dcc: add      r8, sp, #0xdc
003b4dd0: ldr      sl, [r5, fp]
003b4dd4: mov      r0, sl
003b4dd8: bl       #0x337888
003b4ddc: ldr      r1, [pc, #0x7bc]
003b4de0: add      r2, sp, #0x48
003b4de4: mov      r0, r8
003b4de8: add      r1, pc, r1
003b4dec: bl       #0x3140ec
003b4df0: mov      r1, r8
003b4df4: mov      r0, sl
003b4df8: bl       #0x337a88
003b4dfc: mov      r0, r8
003b4e00: bl       #0x318254
003b4e04: movw     r3, #0x13fc
003b4e08: ldr      r2, [r4, r3]
003b4e0c: movw     r3, #0x13f8
003b4e10: ldr      r3, [r4, r3]
003b4e14: cmp      r3, r2
003b4e18: beq      #0x3b4e6c
003b4e1c: ldr      r1, [pc, #0x780]
003b4e20: add      r8, sp, #0x24
003b4e24: ldr      r3, [r4, #0x64]
003b4e28: ldr      r1, [r5, r1]
003b4e2c: mov      r0, r8
003b4e30: ldr      r1, [r1, #0x38]
003b4e34: str      r7, [sp]
003b4e38: str      r7, [sp, #4]
003b4e3c: bl       #0x34aca0
003b4e40: mov      r0, r8
003b4e44: mov      r1, r7
003b4e48: bl       #0x33fdc0
003b4e4c: cmp      r0, #0
003b4e50: beq      #0x3b4e6c
003b4e54: mov      r0, r8
003b4e58: bl       #0x33fee4
003b4e5c: subs     r1, r0, #0
003b4e60: beq      #0x3b4e6c
003b4e64: ldr      r0, [r4, #0x378]
003b4e68: bl       #0x405540
003b4e6c: mov      r0, r4
003b4e70: movw     r7, #0x13c8
003b4e74: bl       #0x3b3d38
003b4e78: ldrsh    r3, [r4, r7]
003b4e7c: cmn      r3, #1
003b4e80: movweq   r1, #0xffff
003b4e84: beq      #0x3b4ec4
003b4e88: ldr      sl, [r5, fp]
003b4e8c: add      r8, sp, #0xc4
003b4e90: mov      r0, sl
003b4e94: bl       #0x337888
003b4e98: ldr      r1, [pc, #0x708]
003b4e9c: add      r2, sp, #0x44
003b4ea0: mov      r0, r8
003b4ea4: add      r1, pc, r1
003b4ea8: bl       #0x3140ec
003b4eac: mov      r1, r8
003b4eb0: mov      r0, sl
003b4eb4: bl       #0x337a88
003b4eb8: mov      r0, r8
003b4ebc: bl       #0x318254
003b4ec0: ldrh     r1, [r4, r7]
003b4ec4: add      r7, r4, #0x560
003b4ec8: sxth     r1, r1
003b4ecc: mov      r0, r7
003b4ed0: bl       #0x3df2a4
003b4ed4: mov      r0, r7
003b4ed8: mov      r1, #1
003b4edc: bl       #0x3e0810
003b4ee0: mov      r0, r4
003b4ee4: bl       #0x3a54d4
003b4ee8: subs     r8, r0, #0
003b4eec: beq      #0x3b4f04
003b4ef0: bl       #0x30de54
003b4ef4: mov      r1, r8
003b4ef8: add      r2, r8, r0
003b4efc: add      r0, r4, #0x290
003b4f00: bl       #0x3109e0
003b4f04: ldr      sl, [r5, fp]
003b4f08: add      r8, sp, #0xac
003b4f0c: mov      r0, sl
003b4f10: bl       #0x337888
003b4f14: ldr      r1, [pc, #0x690]
003b4f18: add      r2, sp, #0x40
003b4f1c: mov      r0, r8
003b4f20: add      r1, pc, r1
003b4f24: bl       #0x3140ec
003b4f28: mov      r1, r8
003b4f2c: mov      r0, sl
003b4f30: bl       #0x337a88
003b4f34: mov      r0, r8
003b4f38: bl       #0x318254
003b4f3c: ldr      r0, [r4, #0x59c]
003b4f40: bl       #0x30e964
003b4f44: movw     r1, #0x74bc
003b4f48: movt     r1, #0x3c13
003b4f4c: bl       #0x30ed6c
003b4f50: str      r0, [r4, #0x120]
003b4f54: ldr      r0, [r4, #0x5a0]
003b4f58: bl       #0x30e964
003b4f5c: movw     r1, #0x74bc
003b4f60: movt     r1, #0x3c13
003b4f64: bl       #0x30ed6c
003b4f68: str      r0, [r4, #0x124]
003b4f6c: ldr      r0, [r4, #0x5a4]
003b4f70: bl       #0x30e964
003b4f74: movw     r1, #0xd70a
003b4f78: movt     r1, #0x3c23
003b4f7c: bl       #0x30ed6c
003b4f80: str      r0, [r4, #0x128]
003b4f84: mov      r0, r4
003b4f88: bl       #0x38be5c
003b4f8c: mov      r0, r4
003b4f90: bl       #0x38ab60
003b4f94: subs     r1, r0, #0
003b4f98: beq      #0x3b5488
003b4f9c: mov      r1, #2
003b4fa0: mov      r0, r4
003b4fa4: bl       #0x3bc4d0
003b4fa8: ldr      r3, [pc, #0x600]
003b4fac: mov      r0, r4
003b4fb0: ldr      r3, [r5, r3]
003b4fb4: ldr      r8, [r3]
003b4fb8: bl       #0x3a2fec
003b4fbc: mov      r3, #0x44
003b4fc0: mla      r8, r3, r0, r8
003b4fc4: ldr      r3, [r4]
003b4fc8: mov      r0, r4
003b4fcc: mov      lr, pc
003b4fd0: ldr      pc, [r3, #0x28]
003b4fd4: cmp      r0, #0
003b4fd8: bne      #0x3b4ff4
003b4fdc: ldrb     r3, [r8, #0x10]
003b4fe0: cmp      r3, #0
003b4fe4: beq      #0x3b4ff4
003b4fe8: mov      r3, #1
003b4fec: strb     r3, [r4, #0x3ec]
003b4ff0: b        #0x3b5014
003b4ff4: ldr      r3, [r4]
003b4ff8: mov      r0, r4
003b4ffc: mov      lr, pc
003b5000: ldr      pc, [r3, #0x28]
003b5004: cmp      r0, #0
003b5008: bne      #0x3b54a4
003b500c: add      r0, r4, #0x3c8
003b5010: bl       #0x3cf1f0
003b5014: movw     r3, #0x1488
003b5018: ldr      r2, [pc, #0x594]
003b501c: ldr      r1, [r4, r3]
003b5020: ldr      r3, [r8, #0x30]
003b5024: str      r2, [sp, #0xc]
003b5028: mov      r2, r4
003b502c: add      r1, r1, r3
003b5030: ldr      r3, [sp, #0xc]
003b5034: ldr      sb, [pc, #0x57c]
003b5038: add      sl, sp, #0x94
003b503c: ldr      r0, [r5, r3]
003b5040: bl       #0x495430
003b5044: movw     r3, #0x1484
003b5048: str      r0, [r4, r3]
003b504c: mov      r0, r4
003b5050: bl       #0x3b4738
003b5054: ldr      r8, [r5, fp]
003b5058: add      sb, pc, sb
003b505c: mov      r0, r8
003b5060: bl       #0x337888
003b5064: add      r2, sp, #0x3c
003b5068: mov      r0, sl
003b506c: mov      r1, sb
003b5070: bl       #0x3140ec
003b5074: mov      r1, sl
003b5078: mov      r0, r8
003b507c: bl       #0x337a88
003b5080: mov      r0, sl
003b5084: bl       #0x318254
003b5088: add      r0, r4, #0x490
003b508c: add      r0, r0, #0xc
003b5090: bl       #0x3c9f4c
003b5094: add      sl, sp, #0x7c
003b5098: mov      r0, r8
003b509c: bl       #0x337888
003b50a0: add      r2, sp, #0x38
003b50a4: mov      r0, sl
003b50a8: mov      r1, sb
003b50ac: bl       #0x3140ec
003b50b0: mov      r1, sl
003b50b4: mov      r0, r8
003b50b8: bl       #0x337a88
003b50bc: mov      r0, sl
003b50c0: bl       #0x318254
003b50c4: mov      r0, r4
003b50c8: bl       #0x3b3b00
003b50cc: add      sl, sp, #0x64
003b50d0: mov      r0, r8
003b50d4: bl       #0x337888
003b50d8: add      r2, sp, #0x34
003b50dc: mov      r1, sb
003b50e0: mov      r0, sl
003b50e4: bl       #0x3140ec
003b50e8: mov      r1, sl
003b50ec: mov      r0, r8
003b50f0: bl       #0x337a88
003b50f4: mov      r0, sl
003b50f8: bl       #0x318254
003b50fc: ldr      r3, [r4]
003b5100: mov      r0, r4
003b5104: mov      lr, pc
003b5108: ldr      pc, [r3, #0x28]
003b510c: cmp      r0, #0
003b5110: beq      #0x3b538c
003b5114: ldr      r3, [r4, #0x2d8]
003b5118: cmp      r3, #0
003b511c: beq      #0x3b512c
003b5120: ldr      r0, [r3, #8]
003b5124: mov      r1, #0
003b5128: bl       #0x59719c
003b512c: ldr      r2, [pc, #0x470]
003b5130: mov      r0, r4
003b5134: mov      r1, #4
003b5138: str      r2, [sp, #0x10]
003b513c: bl       #0x3bc4d0
003b5140: ldr      r3, [sp, #0x10]
003b5144: ldr      r0, [r5, r3]
003b5148: bl       #0x31f594
003b514c: cmp      r0, #0
003b5150: beq      #0x3b5160
003b5154: ldr      r1, [r0, #0x118]
003b5158: mov      r0, r4
003b515c: bl       #0x3bb950
003b5160: ldr      r2, [sp, #0x10]
003b5164: mov      r1, r4
003b5168: ldr      r3, [r5, r2]
003b516c: ldr      r0, [r3, #0x40]
003b5170: bl       #0x36effc
003b5174: cmp      r0, #0
003b5178: bne      #0x3b54dc
003b517c: movw     r3, #0x13c8
003b5180: ldrsh    r1, [r4, r3]
003b5184: mov      r0, r7
003b5188: bl       #0x3df2a4
003b518c: mov      r0, r7
003b5190: bl       #0x3df480
003b5194: mov      r0, r7
003b5198: mov      r1, #1
003b519c: bl       #0x3e0810
003b51a0: ldr      r2, [sp, #0x10]
003b51a4: mov      r1, r4
003b51a8: ldr      r3, [r5, r2]
003b51ac: ldr      r0, [r3, #0x40]
003b51b0: bl       #0x36effc
003b51b4: cmp      r0, #0
003b51b8: bne      #0x3b54d0
003b51bc: movw     r3, #0xc9ff
003b51c0: movt     r3, #0x3b9a
003b51c4: str      r3, [r4, #0x3a4]
003b51c8: mov      r2, #0
003b51cc: mov      r0, r7
003b51d0: mov      r1, #0xc2
003b51d4: bl       #0x3df6e0
003b51d8: ldr      r2, [sp, #0xc]
003b51dc: bic      r0, r0, r0, asr #31
003b51e0: strb     r0, [r4, #0x3a8]
003b51e4: ldr      r3, [r5, r2]
003b51e8: ldr      r2, [r3, #0x1c]
003b51ec: ldr      r3, [r3, #0x20]
003b51f0: rsb      r3, r2, r3
003b51f4: asr      r3, r3, #3
003b51f8: add      r2, r3, r3, lsl #2
003b51fc: add      r2, r2, r2, lsl #4
003b5200: add      r2, r2, r2, lsl #8
003b5204: add      r2, r2, r2, lsl #16
003b5208: add      r3, r3, r2, lsl #1
003b520c: cmp      r3, #0
003b5210: beq      #0x3b538c
003b5214: mov      r0, #0x24
003b5218: mov      r1, #0
003b521c: bl       #0x31056c
003b5220: movw     r3, #0x1494
003b5224: str      r0, [r4, r3]
003b5228: ldr      r3, [pc, #0x38c]
003b522c: mov      sb, r0
003b5230: ldr      r3, [r5, r3]
003b5234: ldr      sl, [r3]
003b5238: cmp      sl, #0
003b523c: beq      #0x3b550c
003b5240: ldr      r3, [pc, #0x378]
003b5244: ldr      r2, [pc, #0x378]
003b5248: str      r7, [sp, #0x18]
003b524c: ldr      r3, [r5, r3]
003b5250: add      r2, pc, r2
003b5254: mov      r8, #0
003b5258: ldr      r3, [r3]
003b525c: str      r0, [sp, #0x14]
003b5260: mov      sb, r2
003b5264: mov      r7, r3
003b5268: b        #0x3b5278
003b526c: add      r8, r8, #1
003b5270: cmp      r8, sl
003b5274: beq      #0x3b5504
003b5278: mov      r0, sb
003b527c: ldr      r1, [r7, r8, lsl #2]
003b5280: bl       #0x30e31c
003b5284: cmp      r0, #0
003b5288: bne      #0x3b526c
003b528c: ldr      sb, [sp, #0x14]
003b5290: ldr      r7, [sp, #0x18]
003b5294: mov      r1, r8
003b5298: ldr      r3, [sp, #0xc]
003b529c: str      r7, [sp, #0x14]
003b52a0: str      fp, [sp, #0x18]
003b52a4: ldr      r2, [r5, r3]
003b52a8: ldr      r3, [pc, #0x318]
003b52ac: str      r6, [sp, #0x1c]
003b52b0: mov      r8, #0
003b52b4: movw     sl, #0x1494
003b52b8: mov      r7, r1
003b52bc: mov      r6, r2
003b52c0: mov      fp, r3
003b52c4: b        #0x3b52cc
003b52c8: ldr      sb, [r4, sl]
003b52cc: mov      r0, r6
003b52d0: add      r1, r7, r8
003b52d4: mov      r2, #0
003b52d8: bl       #0x495430
003b52dc: str      r0, [sb, r8, lsl #2]
003b52e0: ldr      r3, [r4, sl]
003b52e4: ldr      r3, [r3, r8, lsl #2]
003b52e8: cmp      r3, #0
003b52ec: beq      #0x3b5350
003b52f0: ldr      r2, [r5, fp]
003b52f4: mov      r0, r3
003b52f8: mov      r1, #0
003b52fc: ldr      lr, [r2]
003b5300: ldr      ip, [r2, #4]
003b5304: ldr      r2, [r2, #8]
003b5308: str      lr, [r3, #0x34]
003b530c: str      ip, [r3, #0x38]
003b5310: str      r2, [r3, #0x3c]
003b5314: bl       #0x492aa0
003b5318: ldr      r3, [r4, sl]
003b531c: mov      r1, #0
003b5320: ldr      r0, [r3, r8, lsl #2]
003b5324: bl       #0x492ef0
003b5328: ldr      r3, [r4, sl]
003b532c: ldr      r0, [r3, r8, lsl #2]
003b5330: bl       #0x49267c
003b5334: ldr      r3, [r0]
003b5338: mov      lr, pc
003b533c: ldr      pc, [r3, #0x44]
003b5340: mov      r1, #1
003b5344: ldr      r3, [r0]
003b5348: mov      lr, pc
003b534c: ldr      pc, [r3, #0x40]
003b5350: add      r8, r8, #1
003b5354: cmp      r8, #9
003b5358: bne      #0x3b52c8
003b535c: ldr      r2, [sp, #0x10]
003b5360: mov      r1, r4
003b5364: ldr      r7, [sp, #0x14]
003b5368: ldr      r3, [r5, r2]
003b536c: ldr      fp, [sp, #0x18]
003b5370: ldr      r6, [sp, #0x1c]
003b5374: ldr      r0, [r3, #0x40]
003b5378: bl       #0x36effc
003b537c: cmp      r0, #0
003b5380: bne      #0x3b5514
003b5384: mov      r0, r4
003b5388: bl       #0x3a41a0
003b538c: add      r8, r4, #0xff0
003b5390: add      r8, r8, #4
003b5394: mov      r1, r8
003b5398: mov      r2, #0xd2
003b539c: mov      r0, r7
003b53a0: bl       #0x3dedb4
003b53a4: bl       #0x30e964
003b53a8: mov      r3, #0x1440
003b53ac: str      r0, [r4, r3]
003b53b0: mov      r2, #0xd3
003b53b4: mov      r1, r8
003b53b8: mov      r0, r7
003b53bc: bl       #0x3dedb4
003b53c0: bl       #0x30e964
003b53c4: movw     r3, #0x1444
003b53c8: str      r0, [r4, r3]
003b53cc: add      r1, r4, #0x160
003b53d0: mov      r0, r4
003b53d4: bl       #0x3a58f4
003b53d8: add      r1, r4, #0x1440
003b53dc: mov      r0, r4
003b53e0: add      r1, r1, #0x10
003b53e4: mov      r2, #1
003b53e8: bl       #0x393db4
003b53ec: ldr      ip, [r4, #0x16c]
003b53f0: ldr      r0, [r4, #0x2d8]
003b53f4: ldr      r1, [r4, #0x170]
003b53f8: ldr      r2, [r4, #0x174]
003b53fc: movw     r3, #0x145c
003b5400: str      ip, [r4, r3]
003b5404: movw     r3, #0x1460
003b5408: str      r1, [r4, r3]
003b540c: cmp      r0, #0
003b5410: movw     r3, #0x1464
003b5414: str      r2, [r4, r3]
003b5418: beq      #0x3b5420
003b541c: bl       #0x470a54
003b5420: mov      r1, #0
003b5424: mov      r2, #1
003b5428: mov      r0, r4
003b542c: bl       #0x3a59ac
003b5430: mov      r0, r4
003b5434: bl       #0x3b3a70
003b5438: ldrb     r1, [r4, #0x3ec]
003b543c: cmp      r1, #0
003b5440: beq      #0x3b54c4
003b5444: mov      r0, r4
003b5448: bl       #0x3d37d0
003b544c: ldr      r7, [r5, fp]
003b5450: add      r4, sp, #0x4c
003b5454: mov      r0, r7
003b5458: bl       #0x337888
003b545c: ldr      r1, [pc, #0x168]
003b5460: add      r2, sp, #0x30
003b5464: mov      r0, r4
003b5468: add      r1, pc, r1
003b546c: bl       #0x3140ec
003b5470: mov      r0, r7
003b5474: mov      r1, r4
003b5478: bl       #0x337a88
003b547c: mov      r0, r4
003b5480: bl       #0x318254
003b5484: b        #0x3b4d94
003b5488: mov      r0, r4
003b548c: ldr      r3, [r4]
003b5490: mov      lr, pc
003b5494: ldr      pc, [r3, #0x40]
003b5498: mov      r0, r4
003b549c: bl       #0x33ddb4
003b54a0: b        #0x3b4d94
003b54a4: ldr      r3, [pc, #0xf8]
003b54a8: mov      r1, r4
003b54ac: ldr      r3, [r5, r3]
003b54b0: ldr      r0, [r3, #0x40]
003b54b4: bl       #0x36effc
003b54b8: cmp      r0, #0
003b54bc: bne      #0x3b500c
003b54c0: b        #0x3b4fe8
003b54c4: add      r0, r4, #0x3c8
003b54c8: bl       #0x3ce7c0
003b54cc: b        #0x3b5444
003b54d0: mov      r0, r4
003b54d4: bl       #0x3b3a90
003b54d8: b        #0x3b51bc
003b54dc: mov      r0, r4
003b54e0: bl       #0x3b395c
003b54e4: mov      r0, r7
003b54e8: bl       #0x3defac
003b54ec: movw     r3, #0x14e8
003b54f0: ldr      r0, [r4, r3]
003b54f4: cmp      r0, #0
003b54f8: beq      #0x3b517c
003b54fc: bl       #0x4679e8
003b5500: b        #0x3b517c
003b5504: ldr      sb, [sp, #0x14]
003b5508: ldr      r7, [sp, #0x18]
003b550c: mvn      r1, #0
003b5510: b        #0x3b5298
003b5514: ldr      r3, [sp, #0xc]
003b5518: mov      r2, #0
003b551c: movw     r8, #0x149c
003b5520: ldr      r0, [r5, r3]
003b5524: ldr      r3, [pc, #0xa4]
003b5528: ldr      r3, [r5, r3]
003b552c: ldr      r3, [r3]
003b5530: ldr      r1, [r3, #0x88]
003b5534: bl       #0x495430
003b5538: cmp      r0, #0
003b553c: str      r0, [r4, r8]
003b5540: beq      #0x3b5384
003b5544: mov      r2, #0
003b5548: str      r2, [r0, #0x3c]
003b554c: str      r2, [r0, #0x34]
003b5550: str      r2, [r0, #0x38]
003b5554: mov      r1, #0
003b5558: bl       #0x492aa0
003b555c: mov      r1, #0
003b5560: ldr      r0, [r4, r8]
003b5564: bl       #0x492ef0
003b5568: ldr      r0, [r4, r8]
003b556c: bl       #0x49267c
003b5570: ldr      r3, [r0]
003b5574: mov      lr, pc
003b5578: ldr      pc, [r3, #0x44]
003b557c: mov      r1, #1
003b5580: ldr      r3, [r0]
003b5584: mov      lr, pc
003b5588: ldr      pc, [r3, #0x40]
003b558c: b        #0x3b5384
003b5590: bl       #0x30e310
003b5594: subseq   pc, sp, r0, lsr #26
003b5598: andeq    r4, r0, ip, lsr #1
003b559c: andeq    r0, r0, r4, lsl #17
003b55a0: subseq   pc, r0, r8, lsr #32
003b55a4: strdeq   r3, r4, [r0], -r4
003b55a8: subseq   lr, r0, ip, ror #30
003b55ac: ldrsheq  lr, [r0], #-0xe0
003b55b0: andeq    r0, r0, r8, asr r7
003b55b4: andeq    r1, r0, r8, lsl #22
003b55b8: ldrheq   lr, [r0], #-0xd8
003b55bc: andeq    r0, r0, r4, asr #13
003b55c0: muleq    r0, r4, r2
003b55c4: subseq   lr, r0, r0, ror #24
003b55c8: andeq    r3, r0, ip, lsr #30
003b55cc: subseq   lr, r0, r8, lsr #19
003b55d0: andeq    r3, r0, r8, asr #5

# _ZN9Character6ReviveEP10GameObjectb
003a59ac: push     {r4, r5, r6, r7, lr}
003a59b0: movw     r3, #0x1449
003a59b4: ldrb     r3, [r0, r3]
003a59b8: ldr      r5, [pc, #0x11c]
003a59bc: sub      sp, sp, #0x24
003a59c0: cmp      r3, #0
003a59c4: mov      r4, r0
003a59c8: mov      r6, r2
003a59cc: add      r5, pc, r5
003a59d0: bne      #0x3a5a44
003a59d4: mov      r2, #1
003a59d8: movw     r3, #0x1448
003a59dc: strb     r2, [r4, r3]
003a59e0: mov      r7, #0
003a59e4: movw     r3, #0x1449
003a59e8: strb     r7, [r4, r3]
003a59ec: mov      r0, r4
003a59f0: bl       #0x3b3a70
003a59f4: strb     r7, [r4, #0x118]
003a59f8: bl       #0x7fd794
003a59fc: ldrb     r3, [r0, #5]
003a5a00: cmp      r3, r7
003a5a04: mvnne    r3, #0
003a5a08: strne    r3, [r4, #0x110]
003a5a0c: movne    r3, #0
003a5a10: strne    r3, [r4, #0x114]
003a5a14: cmp      r6, #0
003a5a18: bne      #0x3a5ad0
003a5a1c: ldr      r3, [r4]
003a5a20: mov      r0, r4
003a5a24: mov      lr, pc
003a5a28: ldr      pc, [r3, #0x28]
003a5a2c: cmp      r0, #0
003a5a30: bne      #0x3a5a54
003a5a34: add      r0, r4, #0x3c8
003a5a38: bl       #0x3d8894
003a5a3c: add      sp, sp, #0x24
003a5a40: pop      {r4, r5, r6, r7, pc}
003a5a44: mov      r1, #3
003a5a48: mov      r2, #0
003a5a4c: bl       #0x3a4d5c
003a5a50: b        #0x3a59d4
003a5a54: bl       #0x7fd794
003a5a58: ldrb     ip, [r0, #5]
003a5a5c: cmp      ip, #0
003a5a60: bne      #0x3a5a34
003a5a64: ldr      r3, [pc, #0x74]
003a5a68: add      r2, sp, #0x1c
003a5a6c: ldr      r0, [r5, r3]
003a5a70: movw     r3, #0x147c
003a5a74: ldr      lr, [r4, r3]
003a5a78: movw     r3, #0x1474
003a5a7c: ldr      r7, [r4, r3]
003a5a80: movw     r3, #0x1478
003a5a84: ldr      r6, [r4, r3]
003a5a88: add      r5, sp, #0x10
003a5a8c: mov      r3, ip
003a5a90: mov      r1, r5
003a5a94: str      r7, [sp, #0x10]
003a5a98: str      r6, [sp, #0x14]
003a5a9c: str      lr, [sp, #0x1c]
003a5aa0: str      lr, [sp, #0x18]
003a5aa4: str      ip, [sp]
003a5aa8: str      ip, [sp, #4]
003a5aac: str      ip, [sp, #8]
003a5ab0: bl       #0x525508
003a5ab4: ldr      r3, [sp, #0x1c]
003a5ab8: mov      r1, r5
003a5abc: mov      r0, r4
003a5ac0: mov      r2, #1
003a5ac4: str      r3, [sp, #0x18]
003a5ac8: bl       #0x393db4
003a5acc: b        #0x3a5a34
003a5ad0: mov      r0, r4
003a5ad4: bl       #0x3b4088
003a5ad8: b        #0x3a5a1c
003a5adc: subseq   pc, lr, r4, asr #1
003a5ae0: andeq    r1, r0, r4, lsl #4

# _ZN12VisualObject11SetPositionERK7Point3DIfE
00470c24: push     {r4, lr}
00470c28: mov      r4, r0
00470c2c: ldr      r0, [r0, #8]
00470c30: sub      sp, sp, #0x10
00470c34: cmp      r0, #0
00470c38: beq      #0x470c7c
00470c3c: ldr      r3, [r0]
00470c40: ldr      lr, [r1]
00470c44: ldr      ip, [r1, #4]
00470c48: ldr      r2, [r1, #8]
00470c4c: ldr      r3, [r3, #0xa4]
00470c50: add      r1, sp, #4
00470c54: str      lr, [sp, #4]
00470c58: str      ip, [sp, #8]
00470c5c: str      r2, [sp, #0xc]
00470c60: blx      r3
00470c64: ldr      r3, [r4, #8]
00470c68: mov      r1, #0
00470c6c: mov      r0, r3
00470c70: ldr      r3, [r3]
00470c74: mov      lr, pc
00470c78: ldr      pc, [r3, #0xb8]
00470c7c: add      sp, sp, #0x10
00470c80: pop      {r4, pc}

# _ZN6glitch7collada12CSkinnedMesh18computeBoundingBoxEv
00663648: push     {r4, r5, lr}
0066364c: ldr      r3, [r0, #0x3c]
00663650: sub      sp, sp, #0x1c
00663654: mov      r4, r0
00663658: mov      r1, r3
0066365c: mov      r0, sp
00663660: ldr      r3, [r3]
00663664: mov      lr, pc
00663668: ldr      pc, [r3, #8]
0066366c: ldr      ip, [sp, #4]
00663670: ldr      r1, [sp, #8]
00663674: ldr      r2, [sp, #0xc]
00663678: ldr      r3, [sp, #0x10]
0066367c: ldr      r0, [sp, #0x14]
00663680: ldr      r5, [sp]
00663684: str      ip, [r4, #0x28]
00663688: str      r0, [r4, #0x38]
0066368c: str      r5, [r4, #0x24]
00663690: str      r1, [r4, #0x2c]
00663694: str      r2, [r4, #0x30]
00663698: str      r3, [r4, #0x34]
0066369c: add      sp, sp, #0x1c
006636a0: pop      {r4, r5, pc}

# _ZN9Character15SetRelativeAABBERK4aabbIfEb
003a4398: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a439c: ldr      r5, [r1]
003a43a0: cmp      r2, #0
003a43a4: sub      sp, sp, #0xc
003a43a8: str      r5, [r0, #0x144]
003a43ac: ldr      fp, [r1, #4]
003a43b0: mov      r4, r0
003a43b4: str      fp, [r0, #0x148]
003a43b8: ldr      sb, [r1, #8]
003a43bc: str      sb, [r0, #0x14c]
003a43c0: ldr      sl, [r1, #0xc]
003a43c4: str      sl, [r0, #0x150]
003a43c8: ldr      r8, [r1, #0x10]
003a43cc: str      r8, [r0, #0x154]
003a43d0: ldr      r7, [r1, #0x14]
003a43d4: str      r7, [r0, #0x158]
003a43d8: bne      #0x3a445c
003a43dc: movw     r3, #0x1038
003a43e0: ldr      r0, [r0, r3]
003a43e4: str      r2, [sp, #4]
003a43e8: bl       #0x30e964
003a43ec: movw     r1, #0xd70a
003a43f0: movt     r1, #0x3c23
003a43f4: bl       #0x30ed6c
003a43f8: mov      r1, r5
003a43fc: mov      r6, r0
003a4400: bl       #0x30ed6c
003a4404: mov      r1, fp
003a4408: str      r0, [r4, #0x144]
003a440c: mov      r0, r6
003a4410: bl       #0x30ed6c
003a4414: mov      r1, sb
003a4418: str      r0, [r4, #0x148]
003a441c: mov      r0, r6
003a4420: bl       #0x30ed6c
003a4424: mov      r1, sl
003a4428: str      r0, [r4, #0x14c]
003a442c: mov      r0, r6
003a4430: bl       #0x30ed6c
003a4434: mov      r1, r8
003a4438: str      r0, [r4, #0x150]
003a443c: mov      r0, r6
003a4440: bl       #0x30ed6c
003a4444: mov      r1, r7
003a4448: str      r0, [r4, #0x154]
003a444c: mov      r0, r6
003a4450: bl       #0x30ed6c
003a4454: str      r0, [r4, #0x158]
003a4458: ldr      r2, [sp, #4]
003a445c: mov      r0, r4
003a4460: add      r1, r4, #0x144
003a4464: add      sp, sp, #0xc
003a4468: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
003a446c: b        #0x38b110

# _ZN6glitch7collada6detail36CColladaHardwareTextureSkinTechnique18computeBoundingBoxEv
0066ef44: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066ef48: mov      r4, r0
0066ef4c: mov      sl, r1
0066ef50: mov      r0, r1
0066ef54: sub      sp, sp, #0x24
0066ef58: bl       #0x66ee94
0066ef5c: ldr      r2, [sl, #0x10]
0066ef60: mvn      r3, #0x80000000
0066ef64: sub      r3, r3, #0x800000
0066ef68: mvn      r1, #0x800000
0066ef6c: ldr      r0, [r2, #0x10]
0066ef70: ldr      sb, [r2, #0x14]
0066ef74: str      r3, [r4, #8]
0066ef78: str      r1, [r4, #0xc]
0066ef7c: str      r1, [r4, #0x10]
0066ef80: str      r1, [r4, #0x14]
0066ef84: str      r3, [r4]
0066ef88: str      r3, [r4, #4]
0066ef8c: ldr      r3, [sl, #0xc]
0066ef90: rsb      sb, r0, sb
0066ef94: ubfx     sb, sb, #2, #8
0066ef98: ldr      r6, [r3, #0x8c]
0066ef9c: cmp      r6, #0
0066efa0: bne      #0x66f068
0066efa4: cmp      sb, #0
0066efa8: bne      #0x66efcc
0066efac: ldr      r3, [sl, #0x10]
0066efb0: mov      r0, r4
0066efb4: ldr      r2, [r3]
0066efb8: bic      r2, r2, #8
0066efbc: str      r2, [r3]
0066efc0: add      sp, sp, #0x24
0066efc4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066efc8: ldr      r1, [r4, #0xc]
0066efcc: ldr      r3, [sl, #0x10]
0066efd0: ldr      r3, [r3, #0x10]
0066efd4: ldr      r3, [r3, r6, lsl #2]
0066efd8: add      r6, r6, #1
0066efdc: ldr      r8, [r3, #0x30]
0066efe0: ldr      r5, [r3, #0x38]
0066efe4: ldr      r7, [r3, #0x34]
0066efe8: mov      r0, r8
0066efec: bl       #0x30e2f8
0066eff0: cmp      r0, #0
0066eff4: ldr      r1, [r4, #0x10]
0066eff8: strne    r8, [r4, #0xc]
0066effc: mov      r0, r7
0066f000: bl       #0x30e2f8
0066f004: cmp      r0, #0
0066f008: ldr      r1, [r4, #0x14]
0066f00c: strne    r7, [r4, #0x10]
0066f010: mov      r0, r5
0066f014: bl       #0x30e2f8
0066f018: cmp      r0, #0
0066f01c: ldr      r1, [r4]
0066f020: strne    r5, [r4, #0x14]
0066f024: mov      r0, r8
0066f028: bl       #0x30e70c
0066f02c: cmp      r0, #0
0066f030: ldr      r1, [r4, #4]
0066f034: strne    r8, [r4]
0066f038: mov      r0, r7
0066f03c: bl       #0x30e70c
0066f040: cmp      r0, #0
0066f044: strne    r7, [r4, #4]
0066f048: ldr      r1, [r4, #8]
0066f04c: mov      r0, r5
0066f050: bl       #0x30e70c
0066f054: cmp      r0, #0
0066f058: strne    r5, [r4, #8]
0066f05c: cmp      r6, sb
0066f060: blt      #0x66efc8
0066f064: b        #0x66efac
0066f068: cmp      sb, #0
0066f06c: beq      #0x66efac
0066f070: mov      r5, #0
0066f074: add      r2, sp, #8
0066f078: mov      r7, r5
0066f07c: str      r2, [sp, #4]
0066f080: b        #0x66f088
0066f084: ldr      r3, [sl, #0xc]
0066f088: ldr      r3, [r3, #0x90]
0066f08c: ldr      r0, [sl, #0x10]
0066f090: ldr      r1, [sp, #4]
0066f094: ldr      ip, [r3, r5]
0066f098: add      r3, r3, r5
0066f09c: add      r2, r3, #0xc
0066f0a0: str      ip, [sp, #8]
0066f0a4: ldr      ip, [r3, #4]
0066f0a8: add      r5, r5, #0x18
0066f0ac: str      ip, [sp, #0xc]
0066f0b0: ldr      ip, [r3, #8]
0066f0b4: str      ip, [sp, #0x10]
0066f0b8: ldr      r3, [r3, #0xc]
0066f0bc: str      r3, [sp, #0x14]
0066f0c0: ldr      r3, [r2, #4]
0066f0c4: str      r3, [sp, #0x18]
0066f0c8: ldr      r3, [r2, #8]
0066f0cc: str      r3, [sp, #0x1c]
0066f0d0: ldr      r3, [r0, #0x10]
0066f0d4: ldr      r0, [r3, r7, lsl #2]
0066f0d8: bl       #0x587398
0066f0dc: ldr      fp, [sp, #0x14]
0066f0e0: ldr      r1, [r4, #0xc]
0066f0e4: ldr      r8, [sp, #0x18]
0066f0e8: mov      r0, fp
0066f0ec: bl       #0x30e2f8
0066f0f0: cmp      r0, #0
0066f0f4: ldr      r6, [sp, #0x1c]
0066f0f8: ldr      r1, [r4, #0x10]
0066f0fc: strne    fp, [r4, #0xc]
0066f100: mov      r0, r8
0066f104: bl       #0x30e2f8
0066f108: cmp      r0, #0
0066f10c: ldr      r1, [r4, #0x14]
0066f110: strne    r8, [r4, #0x10]
0066f114: mov      r0, r6
0066f118: bl       #0x30e2f8
0066f11c: cmp      r0, #0
0066f120: ldr      r1, [r4]
0066f124: strne    r6, [r4, #0x14]
0066f128: mov      r0, fp
0066f12c: bl       #0x30e70c
0066f130: cmp      r0, #0
0066f134: ldr      r1, [r4, #4]
0066f138: strne    fp, [r4]
0066f13c: mov      r0, r8
0066f140: bl       #0x30e70c
0066f144: cmp      r0, #0
0066f148: ldr      r1, [r4, #8]
0066f14c: strne    r8, [r4, #4]
0066f150: mov      r0, r6
0066f154: bl       #0x30e70c
0066f158: cmp      r0, #0
0066f15c: strne    r6, [r4, #8]
0066f160: ldr      fp, [sp, #8]
0066f164: ldr      r1, [r4, #0xc]
0066f168: ldr      r8, [sp, #0xc]
0066f16c: mov      r0, fp
0066f170: bl       #0x30e2f8
0066f174: cmp      r0, #0
0066f178: ldr      r6, [sp, #0x10]
0066f17c: ldr      r1, [r4, #0x10]
0066f180: strne    fp, [r4, #0xc]
0066f184: mov      r0, r8
0066f188: bl       #0x30e2f8
0066f18c: cmp      r0, #0
0066f190: ldr      r1, [r4, #0x14]
0066f194: strne    r8, [r4, #0x10]
0066f198: mov      r0, r6
0066f19c: bl       #0x30e2f8
0066f1a0: cmp      r0, #0
0066f1a4: ldr      r1, [r4]
0066f1a8: strne    r6, [r4, #0x14]
0066f1ac: mov      r0, fp
0066f1b0: bl       #0x30e70c
0066f1b4: cmp      r0, #0
0066f1b8: ldr      r1, [r4, #4]
0066f1bc: strne    fp, [r4]
0066f1c0: mov      r0, r8
0066f1c4: bl       #0x30e70c
0066f1c8: cmp      r0, #0
0066f1cc: strne    r8, [r4, #4]
0066f1d0: ldr      r1, [r4, #8]
0066f1d4: mov      r0, r6
0066f1d8: bl       #0x30e70c
0066f1dc: add      r7, r7, #1
0066f1e0: cmp      r0, #0
0066f1e4: strne    r6, [r4, #8]
0066f1e8: cmp      r7, sb
0066f1ec: blt      #0x66f084
0066f1f0: b        #0x66efac

# _ZN12VisualObject11CalcMeshBoxEv
0047211c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00472120: ldr      r2, [pc, #0x5d4]
00472124: ldr      r3, [r0, #0xc]
00472128: sub      sp, sp, #0x64
0047212c: add      r2, pc, r2
00472130: cmp      r3, #0
00472134: str      r2, [sp, #8]
00472138: mov      r4, r0
0047213c: beq      #0x472408
00472140: mov      r0, r3
00472144: ldr      r3, [r3]
00472148: mov      lr, pc
0047214c: ldr      pc, [r3, #0x30]
00472150: ldr      r1, [r0]
00472154: mov      r3, r0
00472158: ldr      r2, [r4, #0xc]
0047215c: str      r1, [r4, #0x10]
00472160: ldr      r1, [r0, #4]
00472164: mov      r0, r2
00472168: str      r1, [r4, #0x14]
0047216c: ldr      r3, [r3, #8]
00472170: str      r3, [r4, #0x18]
00472174: ldr      r3, [r2]
00472178: mov      lr, pc
0047217c: ldr      pc, [r3, #0x30]
00472180: ldr      r2, [r0, #0xc]
00472184: mov      r3, r0
00472188: ldr      r0, [r4, #0xc]
0047218c: str      r2, [r4, #0x1c]
00472190: ldr      r2, [r3, #0x10]
00472194: str      r2, [r4, #0x20]
00472198: ldr      r3, [r3, #0x14]
0047219c: str      r3, [r4, #0x24]
004721a0: bl       #0x597290
004721a4: ldr      r3, [r0]
004721a8: mov      lr, pc
004721ac: ldr      pc, [r3, #0x90]
004721b0: mov      r3, r0
004721b4: ldr      r1, [r0]
004721b8: ldr      r0, [r4, #0x10]
004721bc: ldr      r6, [r3, #4]
004721c0: ldr      r5, [r3, #8]
004721c4: bl       #0x30ed6c
004721c8: mov      r1, r6
004721cc: str      r0, [r4, #0x10]
004721d0: ldr      r0, [r4, #0x14]
004721d4: bl       #0x30ed6c
004721d8: mov      r1, r5
004721dc: str      r0, [r4, #0x14]
004721e0: ldr      r0, [r4, #0x18]
004721e4: bl       #0x30ed6c
004721e8: str      r0, [r4, #0x18]
004721ec: ldr      r0, [r4, #0xc]
004721f0: bl       #0x597290
004721f4: ldr      r3, [r0]
004721f8: mov      lr, pc
004721fc: ldr      pc, [r3, #0x90]
00472200: mov      r3, r0
00472204: ldr      r1, [r0]
00472208: ldr      r0, [r4, #0x1c]
0047220c: ldr      r6, [r3, #4]
00472210: ldr      r5, [r3, #8]
00472214: bl       #0x30ed6c
00472218: mov      r1, r6
0047221c: str      r0, [r4, #0x1c]
00472220: ldr      r0, [r4, #0x20]
00472224: bl       #0x30ed6c
00472228: mov      r1, r5
0047222c: str      r0, [r4, #0x20]
00472230: ldr      r0, [r4, #0x24]
00472234: bl       #0x30ed6c
00472238: str      r0, [r4, #0x24]
0047223c: ldr      r3, [r4, #8]
00472240: add      r5, sp, #0x10
00472244: mov      r6, #0
00472248: mov      r0, r3
0047224c: ldr      r3, [r3]
00472250: mov      lr, pc
00472254: ldr      pc, [r3, #0x40]
00472258: mov      r2, #0x41
0047225c: mov      r1, r0
00472260: mov      r0, r5
00472264: strb     r6, [sp, #0x50]
00472268: bl       #0x30e868
0047226c: mov      r3, #0
00472270: mov      r1, r5
00472274: add      r0, r4, #0x10
00472278: str      r3, [sp, #0x48]
0047227c: str      r3, [sp, #0x40]
00472280: str      r3, [sp, #0x44]
00472284: strb     r6, [sp, #0x50]
00472288: bl       #0x312da8
0047228c: mov      r1, r5
00472290: add      r0, r4, #0x1c
00472294: bl       #0x312da8
00472298: ldr      r7, [r4, #0x1c]
0047229c: ldr      r5, [r4, #0x10]
004722a0: mov      r0, r7
004722a4: mov      r1, r5
004722a8: bl       #0x30e70c
004722ac: mov      r1, r5
004722b0: cmp      r0, r6
004722b4: mov      r0, r7
004722b8: movne    fp, r7
004722bc: moveq    fp, r5
004722c0: bl       #0x30e2f8
004722c4: cmp      r0, #0
004722c8: ldr      r6, [r4, #0x20]
004722cc: moveq    r7, r5
004722d0: ldr      r5, [r4, #0x14]
004722d4: mov      r0, r6
004722d8: str      r7, [r4, #0x1c]
004722dc: mov      r1, r5
004722e0: str      fp, [r4, #0x10]
004722e4: bl       #0x30e70c
004722e8: mov      r1, r5
004722ec: cmp      r0, #0
004722f0: mov      r0, r6
004722f4: movne    sb, r6
004722f8: moveq    sb, r5
004722fc: bl       #0x30e2f8
00472300: cmp      r0, #0
00472304: ldr      r8, [r4, #0x18]
00472308: moveq    r6, r5
0047230c: ldr      r5, [r4, #0x24]
00472310: mov      r1, r8
00472314: str      r6, [r4, #0x20]
00472318: mov      r0, r5
0047231c: str      sb, [r4, #0x14]
00472320: bl       #0x30e70c
00472324: mov      r1, r8
00472328: cmp      r0, #0
0047232c: mov      r0, r5
00472330: movne    sl, r5
00472334: moveq    sl, r8
00472338: bl       #0x30e2f8
0047233c: cmp      r0, #0
00472340: moveq    r5, r8
00472344: mov      r1, fp
00472348: str      r5, [r4, #0x24]
0047234c: mov      r0, r7
00472350: str      sl, [r4, #0x18]
00472354: bl       #0x30e3ac
00472358: mov      r1, #0x3f000000
0047235c: bl       #0x30ed6c
00472360: mov      r1, sb
00472364: mov      r7, r0
00472368: mov      r0, r6
0047236c: bl       #0x30e3ac
00472370: mov      r1, #0x3f000000
00472374: bl       #0x30ed6c
00472378: mov      r1, sl
0047237c: mov      r8, r0
00472380: mov      r0, r5
00472384: bl       #0x30e3ac
00472388: mov      r1, #0x3f000000
0047238c: bl       #0x30ed6c
00472390: ldr      ip, [sp, #8]
00472394: ldr      r3, [pc, #0x364]
00472398: mov      r6, r0
0047239c: mov      r1, r7
004723a0: ldr      r5, [ip, r3]
004723a4: ldr      r0, [r5]
004723a8: bl       #0x30e3ac
004723ac: str      r0, [r4, #0x10]
004723b0: ldr      r1, [r5]
004723b4: mov      r0, r7
004723b8: bl       #0x30eba4
004723bc: str      r0, [r4, #0x1c]
004723c0: ldr      r0, [r5, #4]
004723c4: mov      r1, r8
004723c8: bl       #0x30e3ac
004723cc: str      r0, [r4, #0x14]
004723d0: ldr      r1, [r5, #4]
004723d4: mov      r0, r8
004723d8: bl       #0x30eba4
004723dc: str      r0, [r4, #0x20]
004723e0: ldr      r0, [r5, #8]
004723e4: mov      r1, r6
004723e8: bl       #0x30e3ac
004723ec: str      r0, [r4, #0x18]
004723f0: ldr      r1, [r5, #8]
004723f4: mov      r0, r6
004723f8: bl       #0x30eba4
004723fc: str      r0, [r4, #0x24]
00472400: add      sp, sp, #0x64
00472404: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00472408: ldr      ip, [sp, #8]
0047240c: ldr      r0, [pc, #0x2f0]
00472410: mvn      r1, #0x80000000
00472414: sub      r1, r1, #0x800000
00472418: ldr      r5, [ip, r0]
0047241c: mvn      r2, #0x800000
00472420: str      r1, [r4, #0x18]
00472424: str      r1, [r4, #0x10]
00472428: str      r1, [r4, #0x14]
0047242c: str      r2, [r4, #0x24]
00472430: str      r2, [r4, #0x1c]
00472434: str      r2, [r4, #0x20]
00472438: ldr      r2, [r5, #0x10]
0047243c: str      r3, [sp, #0x5c]
00472440: str      r3, [sp, #0x54]
00472444: str      r3, [sp, #0x58]
00472448: ldr      r3, [r2, #0x1c]
0047244c: add      r6, sp, #0x54
00472450: movw     r1, #0x6164
00472454: mov      r0, r3
00472458: ldr      ip, [r3]
0047245c: movt     r1, #0x7365
00472460: ldr      r3, [r4, #8]
00472464: mov      r2, r6
00472468: mov      lr, pc
0047246c: ldr      pc, [ip, #0x20]
00472470: ldr      r0, [sp, #0x54]
00472474: ldr      r3, [sp, #0x58]
00472478: rsb      r3, r0, r3
0047247c: asrs     r3, r3, #2
00472480: str      r3, [sp, #0xc]
00472484: beq      #0x472690
00472488: ldr      r2, [sp, #0xc]
0047248c: cmp      r2, #0
00472490: beq      #0x472680
00472494: mov      r5, #0
00472498: b        #0x4724a0
0047249c: ldr      r0, [sp, #0x54]
004724a0: ldr      r3, [r0, r5, lsl #2]
004724a4: mov      r0, r3
004724a8: ldr      r3, [r3]
004724ac: mov      lr, pc
004724b0: ldr      pc, [r3, #0x30]
004724b4: ldr      r3, [sp, #0x54]
004724b8: ldr      sl, [r0, #8]
004724bc: ldr      fp, [r0]
004724c0: ldr      r3, [r3, r5, lsl #2]
004724c4: ldr      sb, [r0, #4]
004724c8: mov      r0, r3
004724cc: ldr      r3, [r3]
004724d0: mov      lr, pc
004724d4: ldr      pc, [r3, #0x30]
004724d8: ldr      r2, [sp, #0x54]
004724dc: mov      r3, r0
004724e0: ldr      r6, [r0, #0x14]
004724e4: ldr      r8, [r0, #0xc]
004724e8: ldr      r0, [r2, r5, lsl #2]
004724ec: ldr      r7, [r3, #0x10]
004724f0: bl       #0x597290
004724f4: cmp      r0, #0
004724f8: beq      #0x4725bc
004724fc: ldr      r3, [sp, #0x54]
00472500: ldr      r0, [r3, r5, lsl #2]
00472504: bl       #0x597290
00472508: ldr      r3, [r0]
0047250c: mov      lr, pc
00472510: ldr      pc, [r3, #0x90]
00472514: mov      r2, r0
00472518: ldr      r3, [r2, #4]
0047251c: ldr      r2, [r2, #8]
00472520: ldr      r1, [r0]
00472524: mov      r0, fp
00472528: stm      sp, {r2, r3}
0047252c: bl       #0x30ed6c
00472530: ldr      r3, [sp, #4]
00472534: mov      fp, r0
00472538: mov      r0, sb
0047253c: mov      r1, r3
00472540: bl       #0x30ed6c
00472544: ldr      r2, [sp]
00472548: mov      sb, r0
0047254c: mov      r0, sl
00472550: mov      r1, r2
00472554: bl       #0x30ed6c
00472558: ldr      r3, [sp, #0x54]
0047255c: mov      sl, r0
00472560: ldr      r0, [r3, r5, lsl #2]
00472564: bl       #0x597290
00472568: ldr      r3, [r0]
0047256c: mov      lr, pc
00472570: ldr      pc, [r3, #0x90]
00472574: mov      r2, r0
00472578: ldr      r3, [r2, #4]
0047257c: ldr      r2, [r2, #8]
00472580: ldr      r1, [r0]
00472584: mov      r0, r8
00472588: stm      sp, {r2, r3}
0047258c: bl       #0x30ed6c
00472590: ldr      r3, [sp, #4]
00472594: mov      r8, r0
00472598: mov      r0, r7
0047259c: mov      r1, r3
004725a0: bl       #0x30ed6c
004725a4: ldr      r2, [sp]
004725a8: mov      r7, r0
004725ac: mov      r0, r6
004725b0: mov      r1, r2
004725b4: bl       #0x30ed6c
004725b8: mov      r6, r0
004725bc: ldr      r3, [r4, #0x10]
004725c0: mov      r1, fp
004725c4: add      r5, r5, #1
004725c8: mov      r0, r3
004725cc: str      r3, [sp, #4]
004725d0: bl       #0x30e2f8
004725d4: cmp      r0, #0
004725d8: ldr      r3, [sp, #4]
004725dc: movne    r3, fp
004725e0: ldr      fp, [r4, #0x14]
004725e4: str      r3, [r4, #0x10]
004725e8: mov      r1, sb
004725ec: mov      r0, fp
004725f0: bl       #0x30e2f8
004725f4: cmp      r0, #0
004725f8: movne    fp, sb
004725fc: ldr      sb, [r4, #0x18]
00472600: mov      r1, sl
00472604: str      fp, [r4, #0x14]
00472608: mov      r0, sb
0047260c: bl       #0x30e2f8
00472610: cmp      r0, #0
00472614: movne    sb, sl
00472618: ldr      sl, [r4, #0x1c]
0047261c: mov      r1, r8
00472620: str      sb, [r4, #0x18]
00472624: mov      r0, sl
00472628: bl       #0x30e70c
0047262c: cmp      r0, #0
00472630: movne    sl, r8
00472634: ldr      r8, [r4, #0x20]
00472638: mov      r1, r7
0047263c: str      sl, [r4, #0x1c]
00472640: mov      r0, r8
00472644: bl       #0x30e70c
00472648: cmp      r0, #0
0047264c: movne    r8, r7
00472650: ldr      r7, [r4, #0x24]
00472654: str      r8, [r4, #0x20]
00472658: mov      r1, r6
0047265c: mov      r0, r7
00472660: bl       #0x30e70c
00472664: ldr      r3, [sp, #0xc]
00472668: cmp      r0, #0
0047266c: movne    r7, r6
00472670: cmp      r5, r3
00472674: str      r7, [r4, #0x24]
00472678: bne      #0x47249c
0047267c: ldr      r0, [sp, #0x54]
00472680: cmp      r0, #0
00472684: beq      #0x47223c
00472688: bl       #0x310450
0047268c: b        #0x47223c
00472690: ldr      r3, [r5, #0x10]
00472694: movw     r1, #0x6164
00472698: movt     r1, #0x6d65
0047269c: ldr      ip, [r3, #0x1c]
004726a0: mov      r2, r6
004726a4: ldr      r3, [r4, #8]
004726a8: mov      r0, ip
004726ac: ldr      ip, [ip]
004726b0: mov      lr, pc
004726b4: ldr      pc, [ip, #0x20]
004726b8: ldr      r0, [sp, #0x54]
004726bc: ldr      r3, [sp, #0x58]
004726c0: rsb      r3, r0, r3
004726c4: asrs     r3, r3, #2
004726c8: str      r3, [sp, #0xc]
004726cc: bne      #0x472488
004726d0: mov      r3, #0
004726d4: cmp      r0, #0
004726d8: str      r3, [r4, #0x24]
004726dc: str      r3, [r4, #0x10]
004726e0: str      r3, [r4, #0x14]
004726e4: str      r3, [r4, #0x18]
004726e8: str      r3, [r4, #0x1c]
004726ec: str      r3, [r4, #0x20]
004726f0: beq      #0x472400
004726f4: bl       #0x310450
004726f8: b        #0x472400
004726fc: subseq   r2, r2, r4, ror #18
00472700: andeq    r3, r0, ip, lsr #30
00472704: strdeq   r3, r4, [r0], -r4

# _ZN6glitch5scene10ISceneNode22updateAbsolutePositionEb
00597c60: push     {r4, r5, r6, lr}
00597c64: ldr      r3, [r0, #0xec]
00597c68: mov      r4, r0
00597c6c: mov      r5, r1
00597c70: cmp      r3, #0
00597c74: beq      #0x597d18
00597c78: ldr      r2, [r3, #0x11c]
00597c7c: tst      r2, #0x20
00597c80: bne      #0x597cd0
00597c84: ldr      r2, [r0, #0x11c]
00597c88: tst      r2, #0x5e
00597c8c: bne      #0x597cd0
00597c90: cmp      r5, #0
00597c94: ldrne    r5, [r4, #0xf4]!
00597c98: bne      #0x597cc4
00597c9c: b        #0x597ccc
00597ca0: cmp      r5, #0
00597ca4: moveq    r3, r5
00597ca8: subne    r3, r5, #4
00597cac: mov      r0, r3
00597cb0: mov      r1, #1
00597cb4: ldr      r3, [r3]
00597cb8: mov      lr, pc
00597cbc: ldr      pc, [r3, #0xb8]
00597cc0: ldr      r5, [r5]
00597cc4: cmp      r4, r5
00597cc8: bne      #0x597ca0
00597ccc: pop      {r4, r5, r6, pc}
00597cd0: mov      r0, r3
00597cd4: ldr      r3, [r3]
00597cd8: mov      lr, pc
00597cdc: ldr      pc, [r3, #0x38]
00597ce0: ldr      r3, [r4]
00597ce4: mov      r6, r0
00597ce8: mov      r0, r4
00597cec: mov      lr, pc
00597cf0: ldr      pc, [r3, #0x40]
00597cf4: add      r2, r4, #0x24
00597cf8: mov      r1, r0
00597cfc: mov      r0, r6
00597d00: bl       #0x597884
00597d04: ldr      r3, [r4, #0x11c]
00597d08: orr      r3, r3, #0x120
00597d0c: bic      r3, r3, #0x50
00597d10: str      r3, [r4, #0x11c]
00597d14: b        #0x597c90
00597d18: ldr      r3, [r0, #0x11c]
00597d1c: tst      r3, #0x5e
00597d20: beq      #0x597c90
00597d24: mov      r6, r0
00597d28: ldr      r3, [r6], #0x24
00597d2c: mov      lr, pc
00597d30: ldr      pc, [r3, #0x40]
00597d34: mov      r2, #0x41
00597d38: mov      r1, r0
00597d3c: mov      r0, r6
00597d40: bl       #0x30e868
00597d44: ldr      r3, [r4, #0x11c]
00597d48: orr      r3, r3, #0x120
00597d4c: bic      r3, r3, #0x50
00597d50: str      r3, [r4, #0x11c]
00597d54: b        #0x597c90

# _ZN6glitch7collada19CModularSkinnedMesh18computeBoundingBoxEv
00646c1c: push     {r4, r5, r6, r7, r8, lr}
00646c20: ldr      r4, [r0, #0x24]
00646c24: ldr      r5, [r0, #0x28]
00646c28: mov      r7, r0
00646c2c: cmp      r4, r5
00646c30: beq      #0x646c80
00646c34: ldr      r3, [r4, #4]
00646c38: cmp      r3, #0
00646c3c: beq      #0x646ccc
00646c40: mov      r0, r3
00646c44: ldr      r3, [r3]
00646c48: mov      lr, pc
00646c4c: ldr      pc, [r3, #0x24]
00646c50: ldr      r3, [r0]
00646c54: str      r3, [r7, #0x40]
00646c58: ldr      r3, [r0, #4]
00646c5c: str      r3, [r7, #0x44]
00646c60: ldr      r3, [r0, #8]
00646c64: str      r3, [r7, #0x48]
00646c68: ldr      r3, [r0, #0xc]
00646c6c: str      r3, [r7, #0x4c]
00646c70: ldr      r3, [r0, #0x10]
00646c74: str      r3, [r7, #0x50]
00646c78: ldr      r3, [r0, #0x14]
00646c7c: str      r3, [r7, #0x54]
00646c80: add      r4, r4, #8
00646c84: cmp      r4, r5
00646c88: beq      #0x646cc0
00646c8c: add      r6, r7, #0x40
00646c90: ldr      r3, [r4, #4]
00646c94: add      r4, r4, #8
00646c98: subs     r0, r3, #0
00646c9c: beq      #0x646cb8
00646ca0: ldr      r3, [r3]
00646ca4: mov      lr, pc
00646ca8: ldr      pc, [r3, #0x24]
00646cac: mov      r1, r0
00646cb0: mov      r0, r6
00646cb4: bl       #0x35c150
00646cb8: cmp      r4, r5
00646cbc: bne      #0x646c90
00646cc0: mov      r3, #0
00646cc4: strb     r3, [r7, #0x59]
00646cc8: pop      {r4, r5, r6, r7, r8, pc}
00646ccc: add      r4, r4, #8
00646cd0: cmp      r4, r5
00646cd4: bne      #0x646c34
00646cd8: b        #0x646c80

# _ZN12VisualObject12SyncPositionEv
00470cb8: ldr      r1, [r0, #4]
00470cbc: cmp      r1, #0
00470cc0: bxeq     lr
00470cc4: add      r1, r1, #0x160
00470cc8: b        #0x470c24

# _ZN6glitch7collada6detail29CColladaSoftwareSkinTechnique18computeBoundingBoxEv
0066fb84: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0066fb88: mov      r4, r0
0066fb8c: mov      sl, r1
0066fb90: mov      r0, r1
0066fb94: sub      sp, sp, #0x24
0066fb98: bl       #0x66fab4
0066fb9c: ldr      r2, [sl, #0x10]
0066fba0: mvn      r3, #0x80000000
0066fba4: sub      r3, r3, #0x800000
0066fba8: mvn      r1, #0x800000
0066fbac: ldr      r0, [r2, #0x10]
0066fbb0: ldr      sb, [r2, #0x14]
0066fbb4: str      r3, [r4, #8]
0066fbb8: str      r1, [r4, #0xc]
0066fbbc: str      r1, [r4, #0x10]
0066fbc0: str      r1, [r4, #0x14]
0066fbc4: str      r3, [r4]
0066fbc8: str      r3, [r4, #4]
0066fbcc: ldr      r3, [sl, #0xc]
0066fbd0: rsb      sb, r0, sb
0066fbd4: ubfx     sb, sb, #2, #8
0066fbd8: ldr      r6, [r3, #0x8c]
0066fbdc: cmp      r6, #0
0066fbe0: bne      #0x66fca8
0066fbe4: cmp      sb, #0
0066fbe8: bne      #0x66fc0c
0066fbec: ldr      r3, [sl, #0x10]
0066fbf0: mov      r0, r4
0066fbf4: ldr      r2, [r3]
0066fbf8: bic      r2, r2, #8
0066fbfc: str      r2, [r3]
0066fc00: add      sp, sp, #0x24
0066fc04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0066fc08: ldr      r1, [r4, #0xc]
0066fc0c: ldr      r3, [sl, #0x10]
0066fc10: ldr      r3, [r3, #0x10]
0066fc14: ldr      r3, [r3, r6, lsl #2]
0066fc18: add      r6, r6, #1
0066fc1c: ldr      r8, [r3, #0x30]
0066fc20: ldr      r5, [r3, #0x38]
0066fc24: ldr      r7, [r3, #0x34]
0066fc28: mov      r0, r8
0066fc2c: bl       #0x30e2f8
0066fc30: cmp      r0, #0
0066fc34: ldr      r1, [r4, #0x10]
0066fc38: strne    r8, [r4, #0xc]
0066fc3c: mov      r0, r7
0066fc40: bl       #0x30e2f8
0066fc44: cmp      r0, #0
0066fc48: ldr      r1, [r4, #0x14]
0066fc4c: strne    r7, [r4, #0x10]
0066fc50: mov      r0, r5
0066fc54: bl       #0x30e2f8
0066fc58: cmp      r0, #0
0066fc5c: ldr      r1, [r4]
0066fc60: strne    r5, [r4, #0x14]
0066fc64: mov      r0, r8
0066fc68: bl       #0x30e70c
0066fc6c: cmp      r0, #0
0066fc70: ldr      r1, [r4, #4]
0066fc74: strne    r8, [r4]
0066fc78: mov      r0, r7
0066fc7c: bl       #0x30e70c
0066fc80: cmp      r0, #0
0066fc84: strne    r7, [r4, #4]
0066fc88: ldr      r1, [r4, #8]
0066fc8c: mov      r0, r5
0066fc90: bl       #0x30e70c
0066fc94: cmp      r0, #0
0066fc98: strne    r5, [r4, #8]
0066fc9c: cmp      r6, sb
0066fca0: blt      #0x66fc08
0066fca4: b        #0x66fbec
0066fca8: cmp      sb, #0
0066fcac: beq      #0x66fbec
0066fcb0: mov      r5, #0
0066fcb4: add      r2, sp, #8
0066fcb8: mov      r7, r5
0066fcbc: str      r2, [sp, #4]
0066fcc0: b        #0x66fcc8
0066fcc4: ldr      r3, [sl, #0xc]
0066fcc8: ldr      r3, [r3, #0x90]
0066fccc: ldr      r0, [sl, #0x10]
0066fcd0: ldr      r1, [sp, #4]
0066fcd4: ldr      ip, [r3, r5]
0066fcd8: add      r3, r3, r5
0066fcdc: add      r2, r3, #0xc
0066fce0: str      ip, [sp, #8]
0066fce4: ldr      ip, [r3, #4]
0066fce8: add      r5, r5, #0x18
0066fcec: str      ip, [sp, #0xc]
0066fcf0: ldr      ip, [r3, #8]
0066fcf4: str      ip, [sp, #0x10]
0066fcf8: ldr      r3, [r3, #0xc]
0066fcfc: str      r3, [sp, #0x14]
0066fd00: ldr      r3, [r2, #4]
0066fd04: str      r3, [sp, #0x18]
0066fd08: ldr      r3, [r2, #8]
0066fd0c: str      r3, [sp, #0x1c]
0066fd10: ldr      r3, [r0, #0x10]
0066fd14: ldr      r0, [r3, r7, lsl #2]
0066fd18: bl       #0x587398
0066fd1c: ldr      fp, [sp, #0x14]
0066fd20: ldr      r1, [r4, #0xc]
0066fd24: ldr      r8, [sp, #0x18]
0066fd28: mov      r0, fp
0066fd2c: bl       #0x30e2f8
0066fd30: cmp      r0, #0
0066fd34: ldr      r6, [sp, #0x1c]
0066fd38: ldr      r1, [r4, #0x10]
0066fd3c: strne    fp, [r4, #0xc]
0066fd40: mov      r0, r8
0066fd44: bl       #0x30e2f8
0066fd48: cmp      r0, #0
0066fd4c: ldr      r1, [r4, #0x14]
0066fd50: strne    r8, [r4, #0x10]
0066fd54: mov      r0, r6
0066fd58: bl       #0x30e2f8
0066fd5c: cmp      r0, #0
0066fd60: ldr      r1, [r4]
0066fd64: strne    r6, [r4, #0x14]
0066fd68: mov      r0, fp
0066fd6c: bl       #0x30e70c
0066fd70: cmp      r0, #0
0066fd74: ldr      r1, [r4, #4]
0066fd78: strne    fp, [r4]
0066fd7c: mov      r0, r8
0066fd80: bl       #0x30e70c
0066fd84: cmp      r0, #0
0066fd88: ldr      r1, [r4, #8]
0066fd8c: strne    r8, [r4, #4]
0066fd90: mov      r0, r6
0066fd94: bl       #0x30e70c
0066fd98: cmp      r0, #0
0066fd9c: strne    r6, [r4, #8]
0066fda0: ldr      fp, [sp, #8]
0066fda4: ldr      r1, [r4, #0xc]
0066fda8: ldr      r8, [sp, #0xc]
0066fdac: mov      r0, fp
0066fdb0: bl       #0x30e2f8
0066fdb4: cmp      r0, #0
0066fdb8: ldr      r6, [sp, #0x10]
0066fdbc: ldr      r1, [r4, #0x10]
0066fdc0: strne    fp, [r4, #0xc]
0066fdc4: mov      r0, r8
0066fdc8: bl       #0x30e2f8
0066fdcc: cmp      r0, #0
0066fdd0: ldr      r1, [r4, #0x14]
0066fdd4: strne    r8, [r4, #0x10]
0066fdd8: mov      r0, r6
0066fddc: bl       #0x30e2f8
0066fde0: cmp      r0, #0
0066fde4: ldr      r1, [r4]
0066fde8: strne    r6, [r4, #0x14]
0066fdec: mov      r0, fp
0066fdf0: bl       #0x30e70c
0066fdf4: cmp      r0, #0
0066fdf8: ldr      r1, [r4, #4]
0066fdfc: strne    fp, [r4]
0066fe00: mov      r0, r8
0066fe04: bl       #0x30e70c
0066fe08: cmp      r0, #0
0066fe0c: strne    r8, [r4, #4]
0066fe10: ldr      r1, [r4, #8]
0066fe14: mov      r0, r6
0066fe18: bl       #0x30e70c
0066fe1c: add      r7, r7, #1
0066fe20: cmp      r0, #0
0066fe24: strne    r6, [r4, #8]
0066fe28: cmp      r7, sb
0066fe2c: blt      #0x66fcc4
0066fe30: b        #0x66fbec

# _ZN10GameObject15SetRelativeAABBERK4aabbIfEb
0038b110: push     {r4, r5, r6, r7, r8, lr}
0038b114: ldr      r5, [r1]
0038b118: mov      r3, r1
0038b11c: mov      r4, r0
0038b120: str      r5, [r0, #0x144]
0038b124: ldr      r7, [r1, #4]
0038b128: mov      r1, r5
0038b12c: str      r7, [r0, #0x148]
0038b130: ldr      r2, [r3, #8]
0038b134: str      r2, [r0, #0x14c]
0038b138: ldr      r0, [r3, #0xc]
0038b13c: str      r0, [r4, #0x150]
0038b140: ldr      r6, [r3, #0x10]
0038b144: str      r6, [r4, #0x154]
0038b148: ldr      r3, [r3, #0x14]
0038b14c: str      r3, [r4, #0x158]
0038b150: bl       #0x30e3ac
0038b154: mov      r1, #0
0038b158: mov      r8, r0
0038b15c: bl       #0x30df8c
0038b160: cmp      r0, #0
0038b164: beq      #0x38b188
0038b168: mov      r1, r7
0038b16c: mov      r0, r6
0038b170: bl       #0x30e3ac
0038b174: mov      r1, #0
0038b178: bl       #0x30df8c
0038b17c: cmp      r0, #0
0038b180: movne    r3, #1
0038b184: strbne   r3, [r4, #0x2f9]
0038b188: mov      r1, #0x41000000
0038b18c: mov      r0, r8
0038b190: add      r1, r1, #0x200000
0038b194: bl       #0x30e70c
0038b198: cmp      r0, #0
0038b19c: beq      #0x38b1c8
0038b1a0: mov      r1, #0x40000000
0038b1a4: add      r1, r1, #0xa00000
0038b1a8: mov      r0, r5
0038b1ac: bl       #0x30e3ac
0038b1b0: mov      r1, #0x40000000
0038b1b4: str      r0, [r4, #0x144]
0038b1b8: add      r1, r1, #0xa00000
0038b1bc: ldr      r0, [r4, #0x150]
0038b1c0: bl       #0x30eba4
0038b1c4: str      r0, [r4, #0x150]
0038b1c8: ldr      r5, [r4, #0x148]
0038b1cc: ldr      r0, [r4, #0x154]
0038b1d0: mov      r1, r5
0038b1d4: bl       #0x30e3ac
0038b1d8: mov      r1, #0x41000000
0038b1dc: add      r1, r1, #0x200000
0038b1e0: bl       #0x30e70c
0038b1e4: cmp      r0, #0
0038b1e8: beq      #0x38b214
0038b1ec: mov      r1, #0x40000000
0038b1f0: add      r1, r1, #0xa00000
0038b1f4: mov      r0, r5
0038b1f8: bl       #0x30e3ac
0038b1fc: mov      r1, #0x40000000
0038b200: str      r0, [r4, #0x148]
0038b204: add      r1, r1, #0xa00000
0038b208: ldr      r0, [r4, #0x154]
0038b20c: bl       #0x30eba4
0038b210: str      r0, [r4, #0x154]
0038b214: mov      r0, r4
0038b218: bl       #0x38aac8
0038b21c: mov      r0, r4
0038b220: pop      {r4, r5, r6, r7, r8, lr}
0038b224: b        #0x393ea0

# _ZN10GameObject18UpdateAbsoluteAABBEv
0038aac8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0038aacc: mov      r4, r0
0038aad0: ldr      sl, [r4, #0x148]
0038aad4: ldr      r0, [r0, #0x144]
0038aad8: ldr      r8, [r4, #0x14c]
0038aadc: ldr      r7, [r4, #0x150]
0038aae0: ldr      r6, [r4, #0x154]
0038aae4: ldr      r5, [r4, #0x158]
0038aae8: ldr      r1, [r4, #0x160]
0038aaec: str      r0, [r4, #0x12c]
0038aaf0: str      sl, [r4, #0x130]
0038aaf4: str      r8, [r4, #0x134]
0038aaf8: str      r7, [r4, #0x138]
0038aafc: str      r6, [r4, #0x13c]
0038ab00: str      r5, [r4, #0x140]
0038ab04: bl       #0x30eba4
0038ab08: ldr      r1, [r4, #0x164]
0038ab0c: str      r0, [r4, #0x12c]
0038ab10: mov      r0, sl
0038ab14: bl       #0x30eba4
0038ab18: ldr      r1, [r4, #0x168]
0038ab1c: str      r0, [r4, #0x130]
0038ab20: mov      r0, r8
0038ab24: bl       #0x30eba4
0038ab28: ldr      r1, [r4, #0x160]
0038ab2c: str      r0, [r4, #0x134]
0038ab30: mov      r0, r7
0038ab34: bl       #0x30eba4
0038ab38: ldr      r1, [r4, #0x164]
0038ab3c: str      r0, [r4, #0x138]
0038ab40: mov      r0, r6
0038ab44: bl       #0x30eba4
0038ab48: ldr      r1, [r4, #0x168]
0038ab4c: str      r0, [r4, #0x13c]
0038ab50: mov      r0, r5
0038ab54: bl       #0x30eba4
0038ab58: str      r0, [r4, #0x140]
0038ab5c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
