
# FT_Load_Glyph
007091e0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007091e4: subs     r6, r0, #0
007091e8: sub      sp, sp, #0xc
007091ec: mov      sb, r1
007091f0: mov      r5, r2
007091f4: beq      #0x709318
007091f8: ldr      r3, [r6, #0x58]
007091fc: cmp      r3, #0
00709200: beq      #0x709318
00709204: ldr      r4, [r6, #0x54]
00709208: cmp      r4, #0
0070920c: beq      #0x709318
00709210: mov      r0, r4
00709214: bl       #0x706e08
00709218: mov      r3, #0
0070921c: str      r3, [r4, #0x94]
00709220: str      r3, [r4, #0x18]
00709224: str      r3, [r4, #0x1c]
00709228: str      r3, [r4, #0x20]
0070922c: str      r3, [r4, #0x24]
00709230: str      r3, [r4, #0x28]
00709234: str      r3, [r4, #0x2c]
00709238: str      r3, [r4, #0x30]
0070923c: str      r3, [r4, #0x34]
00709240: str      r3, [r4, #0x6c]
00709244: str      r3, [r4, #0x70]
00709248: str      r3, [r4, #0x74]
0070924c: str      r3, [r4, #0x78]
00709250: str      r3, [r4, #0x7c]
00709254: str      r3, [r4, #0x50]
00709258: str      r3, [r4, #0x4c]
0070925c: str      r3, [r4, #0x54]
00709260: strb     r3, [r4, #0x5e]
00709264: str      r3, [r4, #0x64]
00709268: str      r3, [r4, #0x68]
0070926c: str      r3, [r4, #0x80]
00709270: str      r3, [r4, #0x84]
00709274: str      r3, [r4, #0x88]
00709278: str      r3, [r4, #0x8c]
0070927c: str      r3, [r4, #0x98]
00709280: str      r3, [r4, #0x48]
00709284: str      r3, [r4, #0x38]
00709288: str      r3, [r4, #0x3c]
0070928c: str      r3, [r4, #0x90]
00709290: ldr      r2, [r6, #0x60]
00709294: tst      r5, #0x400
00709298: orrne    r5, r5, #0x800
0070929c: ldr      r3, [r2, #4]
007092a0: orrne    r5, r5, #1
007092a4: tst      r5, #1
007092a8: ldr      r7, [r3, #0xa8]
007092ac: orrne    r5, r5, #0xa
007092b0: bicne    r5, r5, #4
007092b4: cmp      r7, #0
007092b8: add      fp, r4, #0x6c
007092bc: beq      #0x7092e8
007092c0: movw     r3, #0x8002
007092c4: movt     r3, #0
007092c8: and      r3, r5, r3
007092cc: cmp      r3, #0
007092d0: bne      #0x7092e8
007092d4: ldr      r3, [r2]
007092d8: ldr      r3, [r3]
007092dc: and      r1, r3, #0x300
007092e0: cmp      r1, #0x100
007092e4: beq      #0x709480
007092e8: ldr      ip, [r2, #0x14]
007092ec: mov      r0, r4
007092f0: mov      r2, sb
007092f4: ldr      r1, [r6, #0x58]
007092f8: mov      r3, r5
007092fc: mov      lr, pc
00709300: ldr      pc, [ip, #0x50]
00709304: subs     sl, r0, #0
00709308: beq      #0x709320
0070930c: mov      r0, sl
00709310: add      sp, sp, #0xc
00709314: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00709318: mov      sl, #0x23
0070931c: b        #0x70930c
00709320: ldr      r2, [r4, #0x48]
00709324: movw     r3, #0x746c
00709328: movt     r3, #0x6f75
0070932c: cmp      r2, r3
00709330: beq      #0x709514
00709334: and      r3, r5, #0x10
00709338: cmp      r3, #0
0070933c: ldrne    r3, [r4, #0x34]
00709340: ldreq    r2, [r4, #0x28]
00709344: movne    r2, #0
00709348: strne    r2, [r4, #0x40]
0070934c: strne    r3, [r4, #0x44]
00709350: streq    r3, [r4, #0x44]
00709354: streq    r2, [r4, #0x40]
00709358: tst      r5, #0x2000
0070935c: bne      #0x70936c
00709360: ldr      r3, [r6, #8]
00709364: tst      r3, #1
00709368: bne      #0x709450
0070936c: tst      r5, #0x800
00709370: bne      #0x7093f4
00709374: ldr      r7, [r6, #0x80]
00709378: ldr      r3, [r7, #0x1c]
0070937c: cmp      r3, #0
00709380: beq      #0x7093f4
00709384: ldr      r3, [r4, #4]
00709388: ldr      r3, [r3, #0x60]
0070938c: ldr      r0, [r3, #4]
00709390: ldr      r2, [r0, #0xa4]
00709394: cmp      r2, #0
00709398: ldreq    r1, [r4, #0x48]
0070939c: beq      #0x7093b0
007093a0: ldr      r1, [r4, #0x48]
007093a4: ldr      r3, [r2, #0x18]
007093a8: cmp      r3, r1
007093ac: beq      #0x7093c4
007093b0: mov      r2, #0
007093b4: bl       #0x705708
007093b8: subs     r2, r0, #0
007093bc: addeq    r6, r7, #4
007093c0: beq      #0x7093e8
007093c4: add      r6, r7, #4
007093c8: mov      r0, r2
007093cc: ldr      ip, [r2, #0x14]
007093d0: add      r3, r7, #0x14
007093d4: mov      r1, r4
007093d8: mov      r2, r6
007093dc: mov      lr, pc
007093e0: ldr      pc, [ip, #0x2c]
007093e4: mov      sl, r0
007093e8: mov      r1, r6
007093ec: add      r0, r4, #0x40
007093f0: bl       #0x705fa0
007093f4: cmp      sl, #0
007093f8: bne      #0x70930c
007093fc: ldr      r2, [r4, #0x48]
00709400: movw     r3, #0x7473
00709404: movt     r3, #0x6269
00709408: cmp      r2, r3
0070940c: beq      #0x70930c
00709410: movw     r3, #0x6d70
00709414: movt     r3, #0x636f
00709418: cmp      r2, r3
0070941c: beq      #0x70930c
00709420: tst      r5, #4
00709424: beq      #0x70930c
00709428: ubfx     r1, r5, #0x10, #4
0070942c: cmp      r1, #0
00709430: bne      #0x709440
00709434: tst      r5, #0x1000
00709438: moveq    r1, sl
0070943c: movne    r1, #2
00709440: mov      r0, r4
00709444: add      sp, sp, #0xc
00709448: pop      {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0070944c: b        #0x70747c
00709450: ldr      r7, [r6, #0x58]
00709454: mov      r2, #0x40
00709458: ldr      r0, [r4, #0x38]
0070945c: ldr      r1, [r7, #0x10]
00709460: bl       #0x704250
00709464: str      r0, [r4, #0x38]
00709468: ldr      r1, [r7, #0x14]
0070946c: ldr      r0, [r4, #0x3c]
00709470: mov      r2, #0x40
00709474: bl       #0x704250
00709478: str      r0, [r4, #0x3c]
0070947c: b        #0x70936c
00709480: ldr      r8, [r6, #0x80]
00709484: ldr      r1, [r8, #0x10]
00709488: cmp      r1, #0
0070948c: ble      #0x7092e8
00709490: ldr      r1, [r8, #0xc]
00709494: cmp      r1, #0
00709498: bne      #0x7092e8
0070949c: tst      r5, #0x20
007094a0: bne      #0x7094c4
007094a4: tst      r3, #0x400
007094a8: beq      #0x7094c4
007094ac: ubfx     r3, r5, #0x10, #4
007094b0: cmp      r3, #1
007094b4: beq      #0x7094c4
007094b8: ldrb     r3, [r8, #0x34]
007094bc: cmp      r3, #0
007094c0: beq      #0x7092e8
007094c4: ldr      r3, [r6, #8]
007094c8: tst      r3, #2
007094cc: beq      #0x7094d8
007094d0: tst      r5, #8
007094d4: beq      #0x7095b8
007094d8: mov      r3, #0
007094dc: ldr      fp, [r8, #0x1c]
007094e0: str      r3, [r8, #0x1c]
007094e4: ldr      r3, [r7]
007094e8: ldr      r2, [r6, #0x58]
007094ec: mov      r0, r7
007094f0: ldr      ip, [r3, #0x14]
007094f4: mov      r1, r4
007094f8: str      r5, [sp]
007094fc: mov      r3, sb
00709500: mov      lr, pc
00709504: ldr      pc, [ip, #0xc]
00709508: str      fp, [r8, #0x1c]
0070950c: mov      sl, r0
00709510: b        #0x709334
00709514: mov      r0, fp
00709518: bl       #0x705d3c
0070951c: subs     sl, r0, #0
00709520: bne      #0x70930c
00709524: tst      r5, #2
00709528: bne      #0x709334
0070952c: ands     r3, r5, #0x10
00709530: bne      #0x7095f8
00709534: ldr      r1, [r4, #0x20]
00709538: ldr      r8, [r4, #0x18]
0070953c: ldr      r2, [r4, #0x24]
00709540: ldr      r7, [r4, #0x1c]
00709544: add      r8, r1, r8
00709548: ldr      ip, [r4, #0x2c]
0070954c: ldr      r0, [r4, #0x30]
00709550: rsb      r7, r7, r2
00709554: add      r8, r8, #0x3f
00709558: add      r2, r2, #0x3f
0070955c: bic      r1, r1, #0x3f
00709560: bic      r2, r2, #0x3f
00709564: bic      r8, r8, #0x3f
00709568: bic      r7, r7, #0x3f
0070956c: rsb      r8, r1, r8
00709570: rsb      r7, r7, r2
00709574: bic      ip, ip, #0x3f
00709578: bic      r0, r0, #0x3f
0070957c: str      ip, [r4, #0x2c]
00709580: str      r0, [r4, #0x30]
00709584: str      r8, [r4, #0x18]
00709588: str      r7, [r4, #0x1c]
0070958c: str      r1, [r4, #0x20]
00709590: str      r2, [r4, #0x24]
00709594: ldr      r1, [r4, #0x28]
00709598: ldr      r2, [r4, #0x34]
0070959c: add      r1, r1, #0x20
007095a0: add      r2, r2, #0x20
007095a4: bic      r1, r1, #0x3f
007095a8: bic      r2, r2, #0x3f
007095ac: str      r1, [r4, #0x28]
007095b0: str      r2, [r4, #0x34]
007095b4: b        #0x709338
007095b8: ldr      ip, [r2, #0x14]
007095bc: mov      r0, r4
007095c0: ldr      r1, [r6, #0x58]
007095c4: mov      r2, sb
007095c8: orr      r3, r5, #0x4000
007095cc: mov      lr, pc
007095d0: ldr      pc, [ip, #0x50]
007095d4: subs     sl, r0, #0
007095d8: bne      #0x7095f0
007095dc: ldr      r2, [r4, #0x48]
007095e0: movw     r3, #0x7473
007095e4: movt     r3, #0x6269
007095e8: cmp      r2, r3
007095ec: beq      #0x709334
007095f0: ldr      r8, [r6, #0x80]
007095f4: b        #0x7094d8
007095f8: ldr      r1, [r4, #0x2c]
007095fc: ldr      r2, [r4, #0x30]
00709600: ldr      r7, [r4, #0x18]
00709604: ldr      ip, [r4, #0x1c]
00709608: ldr      r8, [r4, #0x24]
0070960c: add      r7, r1, r7
00709610: add      ip, r2, ip
00709614: ldr      r0, [r4, #0x20]
00709618: add      r7, r7, #0x3f
0070961c: add      ip, ip, #0x3f
00709620: bic      r1, r1, #0x3f
00709624: bic      r2, r2, #0x3f
00709628: add      r8, r8, #0x3f
0070962c: bic      r7, r7, #0x3f
00709630: bic      ip, ip, #0x3f
00709634: rsb      r7, r1, r7
00709638: rsb      ip, r2, ip
0070963c: bic      r0, r0, #0x3f
00709640: bic      r8, r8, #0x3f
00709644: str      r0, [r4, #0x20]
00709648: str      r8, [r4, #0x24]
0070964c: str      r7, [r4, #0x18]
00709650: str      ip, [r4, #0x1c]
00709654: str      r1, [r4, #0x2c]
00709658: str      r2, [r4, #0x30]
0070965c: b        #0x709594

# FT_Get_Kerning
00704f6c: push     {r4, r5, r6, r7, r8, lr}
00704f70: subs     r5, r0, #0
00704f74: mov      r6, r3
00704f78: ldr      r4, [sp, #0x18]
00704f7c: moveq    r7, #0x23
00704f80: beq      #0x705038
00704f84: cmp      r4, #0
00704f88: moveq    r7, #6
00704f8c: beq      #0x705038
00704f90: ldr      ip, [r5, #0x60]
00704f94: mov      r3, #0
00704f98: str      r3, [r4, #4]
00704f9c: str      r3, [r4]
00704fa0: ldr      r3, [ip, #0x14]
00704fa4: ldr      ip, [r3, #0x54]
00704fa8: cmp      ip, #0
00704fac: moveq    r7, ip
00704fb0: beq      #0x705038
00704fb4: mov      r3, r4
00704fb8: blx      ip
00704fbc: subs     r7, r0, #0
00704fc0: bne      #0x705038
00704fc4: cmp      r6, #2
00704fc8: beq      #0x705038
00704fcc: ldr      r3, [r5, #0x58]
00704fd0: ldr      r0, [r4]
00704fd4: ldr      r1, [r3, #0x10]
00704fd8: bl       #0x704468
00704fdc: str      r0, [r4]
00704fe0: ldr      r3, [r5, #0x58]
00704fe4: ldr      r0, [r4, #4]
00704fe8: ldr      r1, [r3, #0x14]
00704fec: bl       #0x704468
00704ff0: cmp      r6, #1
00704ff4: str      r0, [r4, #4]
00704ff8: beq      #0x705038
00704ffc: ldr      r3, [r5, #0x58]
00705000: ldrh     r1, [r3, #0xc]
00705004: cmp      r1, #0x18
00705008: bls      #0x705040
0070500c: ldrh     r1, [r3, #0xe]
00705010: cmp      r1, #0x18
00705014: bls      #0x705058
00705018: ldr      r3, [r4]
0070501c: ldr      r2, [r4, #4]
00705020: add      r3, r3, #0x20
00705024: add      r2, r2, #0x20
00705028: bic      r3, r3, #0x3f
0070502c: bic      r2, r2, #0x3f
00705030: str      r2, [r4, #4]
00705034: str      r3, [r4]
00705038: mov      r0, r7
0070503c: pop      {r4, r5, r6, r7, r8, pc}
00705040: ldr      r0, [r4]
00705044: mov      r2, #0x19
00705048: bl       #0x704250
0070504c: str      r0, [r4]
00705050: ldr      r3, [r5, #0x58]
00705054: b        #0x70500c
00705058: ldr      r0, [r4, #4]
0070505c: mov      r2, #0x19
00705060: bl       #0x704250
00705064: str      r0, [r4, #4]
00705068: b        #0x705018

# _ZN7gameswf14glyph_providerC1Eiibf
007d0dfc: push     {r4, r5, r6, r7, r8, lr}
007d0e00: ldr      lr, [r0, #0x1c]
007d0e04: sub      sp, sp, #8
007d0e08: mvn      ip, #0
007d0e0c: ldr      r7, [sp, #0x20]
007d0e10: bfi      lr, ip, #0, #0x18
007d0e14: lsr      r5, lr, #0x18
007d0e18: mov      ip, #0
007d0e1c: bfi      r5, ip, #0, #1
007d0e20: mov      r6, #1
007d0e24: str      r7, [r0, #4]
007d0e28: strb     r3, [r0, #8]
007d0e2c: str      lr, [r0, #0x1c]
007d0e30: str      ip, [r0, #0x28]
007d0e34: strb     r5, [r0, #0x1f]
007d0e38: str      ip, [r0]
007d0e3c: strb     r6, [r0, #0xc]
007d0e40: strb     ip, [r0, #0xd]
007d0e44: str      ip, [r0, #0x20]
007d0e48: str      ip, [r0, #0x24]
007d0e4c: mov      r4, r0
007d0e50: mov      r7, r1
007d0e54: mov      r8, r2
007d0e58: bl       #0x70d708
007d0e5c: ldr      r3, [pc, #0x80]
007d0e60: subs     r5, r0, #0
007d0e64: add      r3, pc, r3
007d0e68: bne      #0x7d0ec0
007d0e6c: cmp      r8, #0
007d0e70: cmpgt    r7, #0
007d0e74: ble      #0x7d0eb4
007d0e78: mov      r1, r5
007d0e7c: mov      r0, #0x58
007d0e80: bl       #0x752ba8
007d0e84: mov      r1, r7
007d0e88: mov      r6, r0
007d0e8c: mov      r2, r8
007d0e90: mov      r3, #4
007d0e94: str      r5, [sp]
007d0e98: bl       #0x7941c0
007d0e9c: strb     r5, [r6, #0x4c]
007d0ea0: str      r5, [r6, #0x40]
007d0ea4: str      r5, [r6, #0x44]
007d0ea8: str      r5, [r6, #0x48]
007d0eac: str      r4, [r6, #0x50]
007d0eb0: str      r6, [r4, #0x28]
007d0eb4: mov      r0, r4
007d0eb8: add      sp, sp, #8
007d0ebc: pop      {r4, r5, r6, r7, r8, pc}
007d0ec0: ldr      r0, [pc, #0x20]
007d0ec4: ldr      r1, [pc, #0x20]
007d0ec8: mov      r2, r5
007d0ecc: ldr      r0, [r3, r0]
007d0ed0: add      r1, pc, r1
007d0ed4: add      r0, r0, #0xa8
007d0ed8: bl       #0x30e004
007d0edc: mov      r0, r6
007d0ee0: bl       #0x30de48
007d0ee4: andseq   r3, ip, ip, lsr #24
007d0ee8: andeq    r1, r0, r0, asr #19
007d0eec: ldrheq   fp, [r3], -r8

# FT_Init_FreeType
0070d708: push     {r4, r5, r6, lr}
0070d70c: mov      r4, r0
0070d710: bl       #0x70d8a4
0070d714: subs     r5, r0, #0
0070d718: beq      #0x70d770
0070d71c: mov      r1, r4
0070d720: bl       #0x70aee8
0070d724: subs     r6, r0, #0
0070d728: bne      #0x70d760
0070d72c: ldr      r3, [r4]
0070d730: mov      r2, #2
0070d734: str      r2, [r3, #0xc]
0070d738: ldr      r3, [r4]
0070d73c: mov      r2, #3
0070d740: str      r2, [r3, #0x10]
0070d744: ldr      r3, [r4]
0070d748: mov      r2, #7
0070d74c: str      r2, [r3, #0x14]
0070d750: ldr      r0, [r4]
0070d754: bl       #0x70d690
0070d758: mov      r0, r6
0070d75c: pop      {r4, r5, r6, pc}
0070d760: mov      r0, r5
0070d764: bl       #0x70d890
0070d768: mov      r0, r6
0070d76c: pop      {r4, r5, r6, pc}
0070d770: ldr      r0, [pc, #0x10]
0070d774: mov      r6, #7
0070d778: add      r0, pc, r0
0070d77c: bl       #0x70d610
0070d780: mov      r0, r6
0070d784: pop      {r4, r5, r6, pc}
0070d788: andseq   r5, lr, r8, lsr #1

# FT_Library_Version
0070576c: str      r4, [sp, #-4]!
00705770: cmp      r0, #0
00705774: ldrne    r4, [r0, #0xc]
00705778: ldrne    ip, [r0, #0x14]
0070577c: moveq    ip, r0
00705780: ldrne    r0, [r0, #0x10]
00705784: moveq    r4, ip
00705788: cmp      r1, #0
0070578c: strne    r4, [r1]
00705790: cmp      r2, #0
00705794: strne    r0, [r2]
00705798: cmp      r3, #0
0070579c: strne    ip, [r3]
007057a0: ldm      sp!, {r4}
007057a4: bx       lr

# FT_New_Library
0070aee8: push     {r4, r5, r6, r7, r8, lr}
0070aeec: subs     r5, r0, #0
0070aef0: sub      sp, sp, #8
0070aef4: mov      r8, r1
0070aef8: moveq    r4, #6
0070aefc: beq      #0x70af28
0070af00: add      r7, sp, #4
0070af04: bl       #0x70d438
0070af08: mov      r0, r5
0070af0c: mov      r1, #0xc4
0070af10: mov      r2, r7
0070af14: bl       #0x708364
0070af18: ldr      r4, [sp, #4]
0070af1c: mov      r6, r0
0070af20: cmp      r4, #0
0070af24: beq      #0x70af34
0070af28: mov      r0, r4
0070af2c: add      sp, sp, #8
0070af30: pop      {r4, r5, r6, r7, r8, pc}
0070af34: mov      r1, #0x4000
0070af38: str      r5, [r0]
0070af3c: str      r1, [r0, #0xb0]
0070af40: mov      r2, r7
0070af44: mov      r0, r5
0070af48: bl       #0x708364
0070af4c: str      r0, [r6, #0xac]
0070af50: ldr      r3, [sp, #4]
0070af54: cmp      r3, #0
0070af58: streq    r6, [r8]
0070af5c: beq      #0x70af28
0070af60: mov      r0, r5
0070af64: mov      r1, r6
0070af68: bl       #0x706c04
0070af6c: ldr      r4, [sp, #4]
0070af70: b        #0x70af28
