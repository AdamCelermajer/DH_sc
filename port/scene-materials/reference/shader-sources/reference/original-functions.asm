
# _ZN6glitch4core18allocProcessBufferEi
005345f4: push     {r4, lr}
005345f8: ldr      r3, [pc, #0x80]
005345fc: mov      r4, r0
00534600: add      r3, pc, r3
00534604: ldr      r2, [r3]
00534608: cmp      r2, #0
0053460c: beq      #0x534660
00534610: ldr      r3, [pc, #0x6c]
00534614: add      r2, r4, #3
00534618: lsr      r2, r2, #2
0053461c: add      r3, pc, r3
00534620: ldr      r0, [r3, #8]
00534624: ldr      r1, [r3, #4]
00534628: add      r2, r2, #2
0053462c: rsb      r1, r0, r1
00534630: cmp      r2, r1, asr #2
00534634: bgt      #0x534650
00534638: str      r2, [r0], #4
0053463c: ldr      r1, [r3, #8]
00534640: add      r1, r1, r2, lsl #2
00534644: str      r1, [r3, #8]
00534648: str      r2, [r1, #-4]
0053464c: pop      {r4, pc}
00534650: ldrb     r0, [r3, #0x10]
00534654: cmp      r0, #0
00534658: bne      #0x534670
0053465c: pop      {r4, pc}
00534660: mov      r0, r3
00534664: ldr      r1, [r3, #0xc]
00534668: bl       #0x53427c
0053466c: b        #0x534610
00534670: mov      r0, r4
00534674: mov      r1, #0
00534678: pop      {r4, lr}
0053467c: b        #0x5341a8
00534680: subeq    r1, ip, ip, lsr #31
00534684: umaaleq  r1, ip, r0, pc

# _ZN6glitch5video18ICodeShaderManager20initAdditionalConfigEPKc
006e0a3c: push     {r4, r5, r6, r7, r8, lr}
006e0a40: ldr      r3, [r0, #0x80]
006e0a44: mov      r4, r0
006e0a48: mov      r7, r1
006e0a4c: cmn      r3, #1
006e0a50: beq      #0x6e0a58
006e0a54: pop      {r4, r5, r6, r7, r8, pc}
006e0a58: ldr      r3, [r0, #0x2c]
006e0a5c: ldr      r3, [r3, #0xd4]
006e0a60: ldr      r5, [r3, #0x34]
006e0a64: cmp      r5, #0
006e0a68: ldrne    r3, [r5, #4]
006e0a6c: mov      r0, r5
006e0a70: addne    r3, r3, #1
006e0a74: strne    r3, [r5, #4]
006e0a78: ldr      r3, [r5]
006e0a7c: mov      lr, pc
006e0a80: ldr      pc, [r3, #0xc]
006e0a84: subs     r6, r0, #0
006e0a88: beq      #0x6e0b2c
006e0a8c: ldr      r3, [r6]
006e0a90: mov      lr, pc
006e0a94: ldr      pc, [r3, #0x20]
006e0a98: mov      r1, #0
006e0a9c: str      r0, [r4, #0x80]
006e0aa0: add      r0, r0, #1
006e0aa4: bl       #0x5341a8
006e0aa8: mov      r1, r0
006e0aac: ldr      r0, [r4, #0x7c]
006e0ab0: str      r1, [r4, #0x7c]
006e0ab4: cmp      r0, #0
006e0ab8: beq      #0x6e0ac4
006e0abc: bl       #0x30e0b8
006e0ac0: ldr      r1, [r4, #0x7c]
006e0ac4: ldr      r2, [r4, #0x80]
006e0ac8: ldr      r3, [r6]
006e0acc: mov      r0, r6
006e0ad0: mov      lr, pc
006e0ad4: ldr      pc, [r3, #0xc]
006e0ad8: mov      r0, r6
006e0adc: bl       #0x31d584
006e0ae0: ldr      r3, [r4, #0x80]
006e0ae4: ldr      r2, [r4, #0x7c]
006e0ae8: mov      r1, #0
006e0aec: strb     r1, [r2, r3]
006e0af0: ldr      r3, [r4, #0x7c]
006e0af4: ldr      r1, [r4, #0x80]
006e0af8: add      r1, r3, r1
006e0afc: cmp      r1, r3
006e0b00: beq      #0x6e0b20
006e0b04: mov      r0, #0xa
006e0b08: ldrsb    r2, [r3]
006e0b0c: cmp      r2, #0x5e
006e0b10: strbeq   r0, [r3]
006e0b14: add      r3, r3, #1
006e0b18: cmp      r3, r1
006e0b1c: bne      #0x6e0b08
006e0b20: mov      r0, r5
006e0b24: pop      {r4, r5, r6, r7, r8, lr}
006e0b28: b        #0x31d584
006e0b2c: ldr      r4, [pc, #0x28]
006e0b30: add      r4, pc, r4
006e0b34: ldrb     r3, [r4]
006e0b38: cmp      r3, #0
006e0b3c: beq      #0x6e0b20
006e0b40: ldr      r1, [pc, #0x18]
006e0b44: mov      r2, r7
006e0b48: mov      r0, #2
006e0b4c: add      r1, pc, r1
006e0b50: bl       #0x60b034
006e0b54: strb     r6, [r4]
006e0b58: b        #0x6e0b20
006e0b5c: eoreq    ip, fp, r0, ror #31

# _ZN6glitch5video15CGLSLShaderCodeC1EPKcPS3_NS0_13E_SHADER_TYPEEPNS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEEb
006df738: push     {r3, r4, r5, r6, r7, r8, sb, sl, fp, lr}
006df73c: ldr      r6, [pc, #0x12c]
006df740: mov      r4, r0
006df744: mov      r5, r2
006df748: mov      r8, r3
006df74c: ldrb     sb, [sp, #0x2c]
006df750: bl       #0x6e2178
006df754: ldr      r3, [pc, #0x118]
006df758: add      r6, pc, r6
006df75c: ldr      r1, [sp, #0x28]
006df760: ldr      r3, [r6, r3]
006df764: mov      r2, #0
006df768: str      r1, [r4, #0x2c]
006df76c: add      r3, r3, #8
006df770: strb     r2, [r4, #0x34]
006df774: str      r3, [r4]
006df778: str      r2, [r4, #0x30]
006df77c: ldr      r0, [r5]
006df780: cmp      r0, r2
006df784: moveq    sl, r0
006df788: beq      #0x6df7a8
006df78c: mov      sl, r5
006df790: ldr      r3, [sl, #4]!
006df794: cmp      r3, #0
006df798: bne      #0x6df790
006df79c: rsb      sl, r5, sl
006df7a0: asr      sl, sl, #2
006df7a4: lsl      r0, sl, #2
006df7a8: cmp      r8, #4
006df7ac: movw     r2, #0x8b31
006df7b0: movw     r3, #0x8b30
006df7b4: moveq    r3, r2
006df7b8: str      r3, [r4, #0x28]
006df7bc: str      sl, [r4, #0x24]
006df7c0: mov      r1, #0
006df7c4: bl       #0x5341a8
006df7c8: ldr      r3, [r4, #0x24]
006df7cc: mov      fp, r0
006df7d0: str      r0, [r4, #0x20]
006df7d4: cmp      r3, #0
006df7d8: ble      #0x6df838
006df7dc: mov      r6, #0
006df7e0: mov      r7, r6
006df7e4: b        #0x6df7ec
006df7e8: ldr      fp, [r4, #0x20]
006df7ec: ldr      r0, [r5, r6]
006df7f0: bl       #0x30de54
006df7f4: mov      r1, #0
006df7f8: add      r0, r0, #1
006df7fc: bl       #0x5341a8
006df800: str      r0, [fp, r6]
006df804: ldr      fp, [r5, r6]
006df808: add      r7, r7, #1
006df80c: mov      r0, fp
006df810: bl       #0x30de54
006df814: ldr      r3, [r4, #0x20]
006df818: add      r2, r0, #1
006df81c: mov      r1, fp
006df820: ldr      r0, [r3, r6]
006df824: bl       #0x30e868
006df828: ldr      r3, [r4, #0x24]
006df82c: add      r6, r6, #4
006df830: cmp      r3, r7
006df834: bgt      #0x6df7e8
006df838: cmp      r8, #4
006df83c: movw     r3, #0x8b30
006df840: movw     r1, #0x8b31
006df844: mov      r2, r5
006df848: movne    r1, r3
006df84c: mov      r0, r4
006df850: mov      r3, sl
006df854: bl       #0x6df560
006df858: cmp      sb, #0
006df85c: beq      #0x6df868
006df860: mov      r0, r4
006df864: bl       #0x6df3c0
006df868: mov      r0, r4
006df86c: pop      {r3, r4, r5, r6, r7, r8, sb, sl, fp, pc}
006df870: eoreq    r5, fp, r8, lsr r3
006df874: andeq    r2, r0, ip, lsl #18

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

# _ZN6glitch4core32isProcessBufferHeapExcessEnabledEv
00534254: ldr      r3, [pc, #8]
00534258: add      r3, pc, r3
0053425c: ldrb     r0, [r3, #0x10]
00534260: bx       lr
00534264: subeq    r2, ip, r4, asr r3

# _ZN6glitch5video18CGLSLShaderManager12createShaderEPKcS3_S3_S3_S3_PNS_2io9IReadFileES6_
006e01b4: push     {r4, r5, r6, r7, r8, lr}
006e01b8: mov      r4, r1
006e01bc: sub      sp, sp, #0x18
006e01c0: mov      r5, r0
006e01c4: add      r0, r1, #4
006e01c8: mov      r1, r2
006e01cc: mov      r8, r3
006e01d0: mov      r6, r2
006e01d4: bl       #0x5d85d0
006e01d8: ldr      r7, [pc, #0x160]
006e01dc: movw     r3, #0xffff
006e01e0: cmp      r0, r3
006e01e4: add      r7, pc, r7
006e01e8: beq      #0x6e0238
006e01ec: ldr      r3, [r4, #0x1c]
006e01f0: ldr      r2, [r4, #0x20]
006e01f4: rsb      r2, r3, r2
006e01f8: cmp      r0, r2, asr #3
006e01fc: addlo    r0, r3, r0, lsl #3
006e0200: bhs      #0x6e022c
006e0204: ldr      r3, [r0]
006e0208: cmp      r3, #0
006e020c: str      r3, [r5]
006e0210: beq      #0x6e0220
006e0214: ldr      r2, [r3, #4]
006e0218: add      r2, r2, #1
006e021c: str      r2, [r3, #4]
006e0220: mov      r0, r5
006e0224: add      sp, sp, #0x18
006e0228: pop      {r4, r5, r6, r7, r8, pc}
006e022c: ldr      r3, [pc, #0x110]
006e0230: ldr      r0, [r7, r3]
006e0234: b        #0x6e0204
006e0238: ldr      ip, [sp, #0x30]
006e023c: add      r0, sp, #0x14
006e0240: mov      r2, r8
006e0244: str      ip, [sp]
006e0248: ldr      ip, [sp, #0x3c]
006e024c: mov      r1, r4
006e0250: mov      r3, #4
006e0254: str      ip, [sp, #4]
006e0258: bl       #0x6dfe68
006e025c: ldr      r0, [sp, #0x14]
006e0260: cmp      r0, #0
006e0264: streq    r0, [r5]
006e0268: beq      #0x6e0220
006e026c: ldr      ip, [sp, #0x38]
006e0270: mov      r3, #0xe
006e0274: ldr      r2, [sp, #0x34]
006e0278: str      ip, [sp]
006e027c: ldr      ip, [sp, #0x40]
006e0280: add      r0, sp, #0x10
006e0284: mov      r1, r4
006e0288: str      ip, [sp, #4]
006e028c: bl       #0x6dfe68
006e0290: ldr      r3, [sp, #0x10]
006e0294: cmp      r3, #0
006e0298: streq    r3, [r5]
006e029c: beq      #0x6e0324
006e02a0: ldr      r2, [sp, #0x14]
006e02a4: cmp      r2, #0
006e02a8: str      r2, [sp, #0xc]
006e02ac: beq      #0x6e0338
006e02b0: ldr      r3, [r2, #4]
006e02b4: add      r3, r3, #1
006e02b8: str      r3, [r2, #4]
006e02bc: ldr      r3, [sp, #0x10]
006e02c0: cmp      r3, #0
006e02c4: str      r3, [sp, #8]
006e02c8: beq      #0x6e02d8
006e02cc: ldr      r2, [r3, #4]
006e02d0: add      r2, r2, #1
006e02d4: str      r2, [r3, #4]
006e02d8: mov      r0, r5
006e02dc: add      ip, sp, #8
006e02e0: mov      r1, r4
006e02e4: mov      r2, r6
006e02e8: add      r3, sp, #0xc
006e02ec: str      ip, [sp]
006e02f0: bl       #0x6dfd8c
006e02f4: ldr      r0, [sp, #8]
006e02f8: cmp      r0, #0
006e02fc: beq      #0x6e0304
006e0300: bl       #0x31d584
006e0304: ldr      r0, [sp, #0xc]
006e0308: cmp      r0, #0
006e030c: beq      #0x6e0314
006e0310: bl       #0x31d584
006e0314: ldr      r0, [sp, #0x10]
006e0318: cmp      r0, #0
006e031c: beq      #0x6e0324
006e0320: bl       #0x31d584
006e0324: ldr      r0, [sp, #0x14]
006e0328: cmp      r0, #0
006e032c: beq      #0x6e0220
006e0330: bl       #0x31d584
006e0334: b        #0x6e0220
006e0338: str      r3, [sp, #8]
006e033c: b        #0x6e02cc
006e0340: eoreq    r4, fp, ip, lsr #17
006e0344: strdeq   r4, r5, [r0], -ip

# _ZN6glitch5video14IShaderManager10loadShaderEPKc
005e72bc: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005e72c0: mov      r8, r0
005e72c4: sub      sp, sp, #0x2c
005e72c8: mov      r0, r1
005e72cc: mov      sl, r1
005e72d0: bl       #0x30de54
005e72d4: str      r0, [sp, #0xc]
005e72d8: bl       #0x534254
005e72dc: str      r0, [sp, #8]
005e72e0: mov      r0, #1
005e72e4: bl       #0x534268
005e72e8: mov      r0, #0xfa
005e72ec: bl       #0x5345f4
005e72f0: ldr      r3, [r8, #0x2c]
005e72f4: mov      r7, r0
005e72f8: ldr      r3, [r3, #0xd4]
005e72fc: ldr      r5, [r3, #0x34]
005e7300: cmp      r5, #0
005e7304: ldrne    r3, [r5, #4]
005e7308: addne    r3, r3, #1
005e730c: strne    r3, [r5, #4]
005e7310: ldr      r3, [r8, #0x30]
005e7314: ldr      r2, [r8, #0x34]
005e7318: rsb      r2, r3, r2
005e731c: asr      r2, r2, #3
005e7320: add      sb, r2, r2, lsl #2
005e7324: add      sb, sb, sb, lsl #4
005e7328: add      sb, sb, sb, lsl #8
005e732c: add      sb, sb, sb, lsl #16
005e7330: adds     sb, r2, sb, lsl #1
005e7334: beq      #0x5e73e8
005e7338: ldr      fp, [pc, #0x1c8]
005e733c: mov      r4, #0
005e7340: str      r4, [sp, #4]
005e7344: add      fp, pc, fp
005e7348: mov      r6, r4
005e734c: b        #0x5e7360
005e7350: cmp      r6, sb
005e7354: add      r4, r4, #0x18
005e7358: beq      #0x5e73bc
005e735c: ldr      r3, [r8, #0x30]
005e7360: add      r3, r3, r4
005e7364: ldr      r2, [r3, #0x14]
005e7368: mov      r1, fp
005e736c: mov      r3, sl
005e7370: mov      r0, r7
005e7374: bl       #0x30eae4
005e7378: mov      r1, r7
005e737c: ldr      r3, [r5]
005e7380: mov      r0, r5
005e7384: mov      lr, pc
005e7388: ldr      pc, [r3, #0x44]
005e738c: cmp      r0, #0
005e7390: add      r6, r6, #1
005e7394: beq      #0x5e7350
005e7398: ldr      r3, [r5]
005e739c: mov      r0, r5
005e73a0: mov      r1, r7
005e73a4: mov      lr, pc
005e73a8: ldr      pc, [r3, #0xc]
005e73ac: cmp      r6, sb
005e73b0: str      r0, [sp, #4]
005e73b4: add      r4, r4, #0x18
005e73b8: bne      #0x5e735c
005e73bc: ldr      r3, [sp, #4]
005e73c0: cmp      r3, #0
005e73c4: beq      #0x5e73e8
005e73c8: ldr      r3, [sp, #0xc]
005e73cc: ldr      r1, [pc, #0x138]
005e73d0: sub      r0, r3, #4
005e73d4: add      r0, sl, r0
005e73d8: add      r1, pc, r1
005e73dc: bl       #0x30e31c
005e73e0: subs     r4, r0, #0
005e73e4: beq      #0x5e7420
005e73e8: mov      sl, #0
005e73ec: cmp      r5, #0
005e73f0: beq      #0x5e73fc
005e73f4: mov      r0, r5
005e73f8: bl       #0x31d584
005e73fc: cmp      r7, #0
005e7400: beq      #0x5e740c
005e7404: mov      r0, r7
005e7408: bl       #0x534688
005e740c: ldr      r0, [sp, #8]
005e7410: bl       #0x534268
005e7414: mov      r0, sl
005e7418: add      sp, sp, #0x2c
005e741c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005e7420: add      r6, sp, #0x24
005e7424: ldr      r3, [r8]
005e7428: mov      r2, sl
005e742c: mov      r0, r6
005e7430: mov      r1, r8
005e7434: mov      lr, pc
005e7438: ldr      pc, [r3, #0x14]
005e743c: ldr      r3, [sp, #0x24]
005e7440: cmp      r3, #0
005e7444: beq      #0x5e73e8
005e7448: ldr      r1, [sp, #4]
005e744c: ldr      r3, [r5]
005e7450: mov      r0, r5
005e7454: mov      lr, pc
005e7458: ldr      pc, [r3, #0x50]
005e745c: subs     sl, r0, #0
005e7460: beq      #0x5e74e4
005e7464: ldr      r3, [r5]
005e7468: mov      r0, r5
005e746c: ldr      r1, [r8, #0x2c]
005e7470: mov      lr, pc
005e7474: ldr      pc, [r3, #0x64]
005e7478: subs     sb, r0, #0
005e747c: beq      #0x5e74f8
005e7480: mov      r2, r4
005e7484: add      r4, sp, #0x14
005e7488: mov      r3, r2
005e748c: mov      r1, sl
005e7490: mov      r0, r4
005e7494: bl       #0x570d18
005e7498: mov      r0, sl
005e749c: bl       #0x31d584
005e74a0: mov      r1, sb
005e74a4: mov      r0, r4
005e74a8: bl       #0x571920
005e74ac: ldr      r3, [sp, #0x24]
005e74b0: mov      r1, sb
005e74b4: mov      sl, #1
005e74b8: mov      r0, r3
005e74bc: ldr      r3, [r3]
005e74c0: mov      lr, pc
005e74c4: ldr      pc, [r3, #0x10]
005e74c8: mov      r1, r6
005e74cc: mov      r0, r8
005e74d0: bl       #0x5e7298
005e74d4: mov      r0, sb
005e74d8: bl       #0x31d584
005e74dc: mov      r0, r4
005e74e0: bl       #0x570d98
005e74e4: ldr      r0, [sp, #0x24]
005e74e8: cmp      r0, #0
005e74ec: beq      #0x5e73ec
005e74f0: bl       #0x31d584
005e74f4: b        #0x5e73ec
005e74f8: mov      r0, sl
005e74fc: mov      sl, sb
005e7500: bl       #0x31d584
005e7504: b        #0x5e74e4
005e7508: eoreq    fp, pc, r4, lsr #17

# _ZNK6glitch5video15CGLSLShaderCode13compileShaderEv
006df3c0: push     {r4, r5, lr}
006df3c4: ldrb     r5, [r0, #0x34]
006df3c8: sub      sp, sp, #0x14
006df3cc: mov      r4, r0
006df3d0: cmp      r5, #0
006df3d4: beq      #0x6df3e4
006df3d8: mov      r0, #0
006df3dc: add      sp, sp, #0x14
006df3e0: pop      {r4, r5, pc}
006df3e4: ldr      r0, [r0, #0x30]
006df3e8: bl       #0x30e46c
006df3ec: add      r2, sp, #0x10
006df3f0: str      r5, [r2, #-4]!
006df3f4: ldr      r0, [r4, #0x30]
006df3f8: movw     r1, #0x8b81
006df3fc: bl       #0x30ed54
006df400: add      r2, sp, #0x10
006df404: str      r5, [r2, #-8]!
006df408: ldr      r0, [r4, #0x30]
006df40c: movw     r1, #0x8b84
006df410: bl       #0x30ed54
006df414: ldr      r3, [sp, #0xc]
006df418: cmp      r3, #0
006df41c: beq      #0x6df4b8
006df420: ldr      r0, [sp, #8]
006df424: cmp      r0, #1
006df428: ble      #0x6df4ac
006df42c: bl       #0x5345f4
006df430: ldr      r1, [sp, #8]
006df434: mov      r3, r0
006df438: mov      r5, r0
006df43c: add      r2, sp, #4
006df440: ldr      r0, [r4, #0x30]
006df444: bl       #0x30e214
006df448: ldr      r0, [r4, #0x30]
006df44c: movw     r1, #0x8b4f
006df450: mov      r2, sp
006df454: bl       #0x30ed54
006df458: ldr      r1, [pc, #0xe4]
006df45c: mov      r0, r5
006df460: add      r1, pc, r1
006df464: bl       #0x30ebd4
006df468: cmp      r0, #0
006df46c: beq      #0x6df49c
006df470: ldr      r2, [sp]
006df474: movw     r3, #0x8b31
006df478: cmp      r2, r3
006df47c: beq      #0x6df52c
006df480: ldr      r2, [pc, #0xc0]
006df484: add      r2, pc, r2
006df488: ldr      r1, [pc, #0xbc]
006df48c: mov      r0, #2
006df490: mov      r3, r5
006df494: add      r1, pc, r1
006df498: bl       #0x60b034
006df49c: cmp      r5, #0
006df4a0: beq      #0x6df4ac
006df4a4: mov      r0, r5
006df4a8: bl       #0x534688
006df4ac: mov      r0, #1
006df4b0: strb     r0, [r4, #0x34]
006df4b4: b        #0x6df3dc
006df4b8: ldr      r0, [sp, #8]
006df4bc: bl       #0x5345f4
006df4c0: mov      r5, r0
006df4c4: mov      r3, r5
006df4c8: ldr      r1, [sp, #8]
006df4cc: ldr      r0, [r4, #0x30]
006df4d0: mov      r2, sp
006df4d4: bl       #0x30dfbc
006df4d8: add      r2, sp, #4
006df4dc: ldr      r0, [r4, #0x30]
006df4e0: movw     r1, #0x8b4f
006df4e4: bl       #0x30ed54
006df4e8: ldr      r2, [sp, #4]
006df4ec: movw     r3, #0x8b31
006df4f0: cmp      r2, r3
006df4f4: beq      #0x6df538
006df4f8: ldr      r2, [pc, #0x50]
006df4fc: add      r2, pc, r2
006df500: ldr      r1, [pc, #0x4c]
006df504: mov      r0, #3
006df508: mov      r3, r5
006df50c: add      r1, pc, r1
006df510: bl       #0x60b034
006df514: cmp      r5, #0
006df518: beq      #0x6df3d8
006df51c: mov      r0, r5
006df520: bl       #0x534688
006df524: mov      r0, #0
006df528: b        #0x6df3dc
006df52c: ldr      r2, [pc, #0x24]
006df530: add      r2, pc, r2
006df534: b        #0x6df488
006df538: ldr      r2, [pc, #0x1c]
006df53c: add      r2, pc, r2
006df540: b        #0x6df500
006df544: eoreq    pc, r0, r0, lsr #20
006df548: eoreq    pc, r0, r4, asr #19

# _ZN6glitch7collada19SProfileGLES2Traits12createShaderEPNS_5video14IShaderManagerERNS0_5SPassINS0_25SRenderStatesProgrammableEEE
00634b30: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00634b34: ldr      r5, [pc, #0x120]
00634b38: ldr      r7, [pc, #0x120]
00634b3c: sub      sp, sp, #0x3c
00634b40: add      r5, pc, r5
00634b44: ldr      r3, [r5, r7]
00634b48: add      r4, sp, #0x1c
00634b4c: mov      r8, r0
00634b50: ldr      r3, [r3]
00634b54: mov      sb, r1
00634b58: mov      r0, r4
00634b5c: mov      r1, #0x10
00634b60: mov      sl, r2
00634b64: str      r3, [sp, #0x34]
00634b68: str      r4, [sp, #0x2c]
00634b6c: str      r4, [sp, #0x30]
00634b70: bl       #0x3209a8
00634b74: ldr      r3, [sp, #0x2c]
00634b78: mov      r6, #0
00634b7c: strb     r6, [r3]
00634b80: ldr      fp, [sl, #4]
00634b84: mov      r0, fp
00634b88: bl       #0x30de54
00634b8c: mov      r1, fp
00634b90: add      r2, fp, r0
00634b94: mov      r0, r4
00634b98: bl       #0x320a4c
00634b9c: ldr      fp, [sl, #0xc]
00634ba0: mov      r0, fp
00634ba4: bl       #0x30de54
00634ba8: mov      r1, fp
00634bac: add      r2, fp, r0
00634bb0: mov      r0, r4
00634bb4: bl       #0x320a4c
00634bb8: ldr      fp, [sl, #0x10]
00634bbc: mov      r0, fp
00634bc0: bl       #0x30de54
00634bc4: mov      r1, fp
00634bc8: add      r2, fp, r0
00634bcc: mov      r0, r4
00634bd0: bl       #0x320a4c
00634bd4: ldr      fp, [sl, #0x18]
00634bd8: mov      r0, fp
00634bdc: bl       #0x30de54
00634be0: mov      r1, fp
00634be4: add      r2, fp, r0
00634be8: mov      r0, r4
00634bec: bl       #0x320a4c
00634bf0: ldr      r3, [sl, #4]
00634bf4: ldr      lr, [sl, #0x18]
00634bf8: ldr      ip, [sl, #0xc]
00634bfc: ldr      sl, [sl, #0x10]
00634c00: mov      r0, r8
00634c04: mov      r1, sb
00634c08: ldr      r2, [sp, #0x30]
00634c0c: str      ip, [sp]
00634c10: stmib    sp, {sl, lr}
00634c14: str      r6, [sp, #0x10]
00634c18: str      r6, [sp, #0xc]
00634c1c: bl       #0x6e01b4
00634c20: ldr      r0, [sp, #0x30]
00634c24: cmp      r0, r4
00634c28: beq      #0x634c38
00634c2c: cmp      r0, r6
00634c30: beq      #0x634c38
00634c34: bl       #0x310450
00634c38: ldr      r3, [r5, r7]
00634c3c: ldr      r2, [sp, #0x34]
00634c40: mov      r0, r8
00634c44: ldr      r3, [r3]
00634c48: cmp      r2, r3
00634c4c: bne      #0x634c58
00634c50: add      sp, sp, #0x3c
00634c54: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00634c58: bl       #0x30e310
00634c5c: eorseq   pc, r5, r0, asr pc
00634c60: andeq    r4, r0, ip, lsr #1

# _ZN16BufferedRenderer23createBlendModeMaterialEN5boost13intrusive_ptrIN6glitch5video17CMaterialRendererEEEiPKc
007d4ab0: push     {r4, r5, r6, r7, r8, lr}
007d4ab4: sub      sp, sp, #8
007d4ab8: add      r4, sp, #4
007d4abc: mov      r7, r2
007d4ac0: mov      r2, #0
007d4ac4: mov      r5, r0
007d4ac8: mov      r6, r3
007d4acc: mov      r0, r4
007d4ad0: mov      r3, r2
007d4ad4: mov      r8, r1
007d4ad8: bl       #0x5cc0a0
007d4adc: mov      r3, #0xc
007d4ae0: mul      r7, r3, r7
007d4ae4: mov      r1, r4
007d4ae8: add      r0, r5, r7
007d4aec: add      r0, r0, #0x3c
007d4af0: bl       #0x7d4a08
007d4af4: mov      r0, r4
007d4af8: bl       #0x310be8
007d4afc: ldr      r0, [r8]
007d4b00: mov      r1, r6
007d4b04: bl       #0x5d4714
007d4b08: cmp      r0, #0xff
007d4b0c: beq      #0x7d4b24
007d4b10: add      r7, r5, r7
007d4b14: ldr      r3, [r7, #0x40]
007d4b18: strb     r0, [r3, #8]
007d4b1c: add      sp, sp, #8
007d4b20: pop      {r4, r5, r6, r7, r8, pc}
007d4b24: ldr      r0, [pc, #0xc]
007d4b28: mov      r1, r6
007d4b2c: add      r0, pc, r0
007d4b30: bl       #0x7611f0
007d4b34: b        #0x7d4b1c
007d4b38: andseq   r7, r3, r4, lsr r5

# _ZN6glitch5video18ICodeShaderManager18makeShaderCodeNameEPKcjS3_jS3_jPj
006e0b64: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006e0b68: sub      sp, sp, #0xc
006e0b6c: mov      r5, r2
006e0b70: mov      r4, r0
006e0b74: mov      fp, r1
006e0b78: mov      sb, r3
006e0b7c: bl       #0x534254
006e0b80: str      r0, [sp, #4]
006e0b84: mov      r0, #1
006e0b88: bl       #0x534268
006e0b8c: ldr      r3, [r4, #0x7c]
006e0b90: ldr      r7, [sp, #0x30]
006e0b94: ldr      sl, [sp, #0x38]
006e0b98: cmp      r3, #0
006e0b9c: ldrne    r8, [r4, #0x80]
006e0ba0: add      r7, r7, r5
006e0ba4: add      sl, sl, r7
006e0ba8: moveq    r8, sl
006e0bac: addne    r8, sl, r8
006e0bb0: add      r0, r8, #1
006e0bb4: bl       #0x5345f4
006e0bb8: mov      r1, fp
006e0bbc: mov      r6, r0
006e0bc0: bl       #0x30e520
006e0bc4: mov      r1, sb
006e0bc8: add      r0, r6, r5
006e0bcc: bl       #0x30e520
006e0bd0: ldr      r1, [sp, #0x34]
006e0bd4: add      r0, r6, r7
006e0bd8: bl       #0x30e520
006e0bdc: ldr      r1, [r4, #0x7c]
006e0be0: cmp      r1, #0
006e0be4: beq      #0x6e0bf0
006e0be8: add      r0, r6, sl
006e0bec: bl       #0x30e520
006e0bf0: ldr      r3, [sp, #0x3c]
006e0bf4: cmp      r3, #0
006e0bf8: strne    r8, [r3]
006e0bfc: ldr      r0, [sp, #4]
006e0c00: bl       #0x534268
006e0c04: mov      r0, r6
006e0c08: add      sp, sp, #0xc
006e0c0c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}

# _ZN6glitch4core33setProcessBufferHeapExcessEnabledEb
00534268: ldr      r3, [pc, #8]
0053426c: add      r3, pc, r3
00534270: strb     r0, [r3, #0x10]
00534274: bx       lr
00534278: subeq    r2, ip, r0, asr #6

# _ZNK21render_handler_glitch10fill_style5applyEPN6glitch5video12IVideoDriverER16BufferedRendererP6Vertexi
007d6b54: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d6b58: ldr      ip, [r0]
007d6b5c: sub      sp, sp, #0x1c
007d6b60: mov      r4, r0
007d6b64: cmp      ip, #1
007d6b68: mov      r7, r2
007d6b6c: mov      sb, r3
007d6b70: ldr      r5, [sp, #0x40]
007d6b74: ldr      r6, [r0, #8]
007d6b78: beq      #0x7d6bcc
007d6b7c: sub      ip, ip, #2
007d6b80: cmp      ip, #1
007d6b84: bls      #0x7d6c2c
007d6b88: cmp      r5, #0
007d6b8c: ldrb     r2, [r0, #6]
007d6b90: ldrb     ip, [r0, #7]
007d6b94: ldrb     r1, [r4, #5]
007d6b98: ldrb     r0, [r0, #4]
007d6b9c: ble      #0x7d6bc4
007d6ba0: mov      r3, #0
007d6ba4: add      r3, r3, #1
007d6ba8: cmp      r5, r3
007d6bac: strb     ip, [sb, #0xb]
007d6bb0: strb     r2, [sb, #0xa]
007d6bb4: strb     r1, [sb, #9]
007d6bb8: strb     r0, [sb, #8]
007d6bbc: add      sb, sb, #0x18
007d6bc0: bgt      #0x7d6ba4
007d6bc4: add      sp, sp, #0x1c
007d6bc8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d6bcc: ldr      r3, [r6]
007d6bd0: mov      r0, r6
007d6bd4: mov      lr, pc
007d6bd8: ldr      pc, [r3, #8]
007d6bdc: mov      r0, r7
007d6be0: add      r1, r6, #0x10
007d6be4: bl       #0x7d6a48
007d6be8: cmp      r5, #0
007d6bec: ble      #0x7d6bc4
007d6bf0: mov      r1, #0
007d6bf4: mov      r0, #0
007d6bf8: mov      r2, r1
007d6bfc: mov      r3, sb
007d6c00: add      r2, r2, #1
007d6c04: str      r0, [r3, r1]!
007d6c08: cmp      r2, r5
007d6c0c: str      r0, [r3, #4]
007d6c10: add      r1, r1, #0x18
007d6c14: bne      #0x7d6bfc
007d6c18: ldrb     r2, [r4, #6]
007d6c1c: ldrb     ip, [r4, #7]
007d6c20: ldrb     r0, [r4, #4]
007d6c24: ldrb     r1, [r4, #5]
007d6c28: b        #0x7d6ba0
007d6c2c: ldr      r3, [r6]
007d6c30: mov      r0, r6
007d6c34: mov      lr, pc
007d6c38: ldr      pc, [r3, #8]
007d6c3c: mov      r0, r7
007d6c40: add      r1, r6, #0x10
007d6c44: bl       #0x7d6a48
007d6c48: ldr      r0, [r6, #0x10]
007d6c4c: cmp      r0, #0
007d6c50: beq      #0x7d6c68
007d6c54: ldr      r1, [r4]
007d6c58: cmp      r1, #2
007d6c5c: movne    r1, #2
007d6c60: moveq    r1, #0
007d6c64: bl       #0x7d3bb4
007d6c68: ldr      r3, [r4, #8]
007d6c6c: mov      r0, r3
007d6c70: ldr      r3, [r3]
007d6c74: mov      lr, pc
007d6c78: ldr      pc, [r3, #0x24]
007d6c7c: bl       #0x30e964
007d6c80: mov      r1, r0
007d6c84: mov      r0, #0x3f800000
007d6c88: bl       #0x30ec94
007d6c8c: ldr      r3, [r4, #8]
007d6c90: mov      r7, r0
007d6c94: mov      r0, r3
007d6c98: ldr      r3, [r3]
007d6c9c: mov      lr, pc
007d6ca0: ldr      pc, [r3, #0x28]
007d6ca4: bl       #0x30e964
007d6ca8: mov      r1, r0
007d6cac: mov      r0, #0x3f800000
007d6cb0: bl       #0x30ec94
007d6cb4: ldr      r1, [r4, #0xc]
007d6cb8: mov      r6, r0
007d6cbc: mov      r0, r7
007d6cc0: bl       #0x30ed6c
007d6cc4: ldr      r1, [r4, #0x10]
007d6cc8: mov      sl, r0
007d6ccc: mov      r0, r7
007d6cd0: bl       #0x30ed6c
007d6cd4: ldr      r1, [r4, #0x14]
007d6cd8: mov      r8, r0
007d6cdc: mov      r0, r7
007d6ce0: bl       #0x30ed6c
007d6ce4: str      r0, [sp, #0xc]
007d6ce8: ldr      r1, [r4, #0x18]
007d6cec: mov      r0, r6
007d6cf0: bl       #0x30ed6c
007d6cf4: str      r0, [sp, #8]
007d6cf8: ldr      r1, [r4, #0x1c]
007d6cfc: mov      r0, r6
007d6d00: bl       #0x30ed6c
007d6d04: str      r0, [sp, #4]
007d6d08: ldr      r1, [r4, #0x20]
007d6d0c: mov      r0, r6
007d6d10: bl       #0x30ed6c
007d6d14: cmp      r5, #0
007d6d18: str      r0, [sp]
007d6d1c: ble      #0x7d6bc4
007d6d20: mov      r6, sb
007d6d24: str      sb, [sp, #0x14]
007d6d28: mov      r7, #0
007d6d2c: mov      fp, sl
007d6d30: str      r4, [sp, #0x10]
007d6d34: mov      sb, r8
007d6d38: ldr      r8, [r6, #0xc]
007d6d3c: mov      r0, fp
007d6d40: ldr      r4, [r6, #0x10]
007d6d44: mov      r1, r8
007d6d48: bl       #0x30ed6c
007d6d4c: mov      r1, r4
007d6d50: mov      sl, r0
007d6d54: mov      r0, sb
007d6d58: bl       #0x30ed6c
007d6d5c: mov      r1, r0
007d6d60: mov      r0, sl
007d6d64: bl       #0x30eba4
007d6d68: mov      r1, r0
007d6d6c: ldr      r0, [sp, #0xc]
007d6d70: bl       #0x30eba4
007d6d74: str      r0, [r6]
007d6d78: mov      r1, r8
007d6d7c: ldr      r0, [sp, #8]
007d6d80: bl       #0x30ed6c
007d6d84: mov      r1, r4
007d6d88: mov      r8, r0
007d6d8c: ldr      r0, [sp, #4]
007d6d90: bl       #0x30ed6c
007d6d94: mov      r1, r0
007d6d98: mov      r0, r8
007d6d9c: bl       #0x30eba4
007d6da0: mov      r1, r0
007d6da4: ldr      r0, [sp]
007d6da8: bl       #0x30eba4
007d6dac: add      r7, r7, #1
007d6db0: cmp      r7, r5
007d6db4: str      r0, [r6, #4]
007d6db8: add      r6, r6, #0x18
007d6dbc: bne      #0x7d6d38
007d6dc0: ldr      r4, [sp, #0x10]
007d6dc4: ldr      sb, [sp, #0x14]
007d6dc8: ldrb     r2, [r4, #6]
007d6dcc: ldrb     ip, [r4, #7]
007d6dd0: ldrb     r0, [r4, #4]
007d6dd4: ldrb     r1, [r4, #5]
007d6dd8: b        #0x7d6ba0

# _ZN6glitch5video18CGLSLShaderManager16createShaderCodeEPKcNS0_13E_SHADER_TYPEES3_PNS_2io9IReadFileE
006dfe68: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
006dfe6c: mov      r4, r1
006dfe70: ldr      r1, [r1, #0x80]
006dfe74: sub      sp, sp, #0x3c
006dfe78: mov      r6, r0
006dfe7c: cmn      r1, #1
006dfe80: mov      r7, r2
006dfe84: mov      fp, r3
006dfe88: ldr      sb, [sp, #0x60]
006dfe8c: beq      #0x6e010c
006dfe90: mov      ip, #0
006dfe94: mov      r2, ip
006dfe98: mov      r3, sb
006dfe9c: mov      r1, r7
006dfea0: mov      r0, r4
006dfea4: str      ip, [sp]
006dfea8: bl       #0x6e0c10
006dfeac: mov      r5, r0
006dfeb0: mov      r1, r4
006dfeb4: add      r0, sp, #0x34
006dfeb8: mov      r2, r5
006dfebc: bl       #0x6e1b08
006dfec0: ldr      r3, [sp, #0x34]
006dfec4: cmp      r3, #0
006dfec8: beq      #0x6dff08
006dfecc: str      r3, [r6]
006dfed0: ldr      r2, [r3, #4]
006dfed4: add      r2, r2, #1
006dfed8: str      r2, [r3, #4]
006dfedc: ldr      r0, [sp, #0x34]
006dfee0: cmp      r0, #0
006dfee4: beq      #0x6dfeec
006dfee8: bl       #0x31d584
006dfeec: cmp      r5, #0
006dfef0: beq      #0x6dfefc
006dfef4: mov      r0, r5
006dfef8: bl       #0x534688
006dfefc: mov      r0, r6
006dff00: add      sp, sp, #0x3c
006dff04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
006dff08: ldr      r3, [r4, #0x2c]
006dff0c: ldr      r3, [r3, #0xd4]
006dff10: ldr      sl, [r3, #0x34]
006dff14: cmp      sl, #0
006dff18: ldrne    r3, [sl, #4]
006dff1c: addne    r3, r3, #1
006dff20: strne    r3, [sl, #4]
006dff24: ldr      r1, [sp, #0x64]
006dff28: cmp      r1, #0
006dff2c: ldrne    r8, [sp, #0x64]
006dff30: beq      #0x6e0144
006dff34: ldr      r3, [r8]
006dff38: mov      r0, r8
006dff3c: mov      lr, pc
006dff40: ldr      pc, [r3, #0x20]
006dff44: str      r0, [sp, #0xc]
006dff48: ldr      r3, [r8]
006dff4c: mov      r0, r8
006dff50: mov      lr, pc
006dff54: ldr      pc, [r3, #0x20]
006dff58: add      r0, r0, #1
006dff5c: bl       #0x5345f4
006dff60: str      r0, [sp, #8]
006dff64: ldr      r3, [r8]
006dff68: mov      r0, r8
006dff6c: ldr      r1, [sp, #8]
006dff70: ldr      r2, [sp, #0xc]
006dff74: mov      lr, pc
006dff78: ldr      pc, [r3, #0xc]
006dff7c: ldr      r3, [sp, #0xc]
006dff80: cmp      r3, r0
006dff84: beq      #0x6dffc8
006dff88: ldr      r1, [pc, #0x1f4]
006dff8c: mov      r2, r7
006dff90: mov      r0, #3
006dff94: add      r1, pc, r1
006dff98: bl       #0x60b034
006dff9c: mov      r3, #0
006dffa0: str      r3, [r6]
006dffa4: ldr      r3, [sp, #8]
006dffa8: cmp      r3, #0
006dffac: beq      #0x6dffb8
006dffb0: mov      r0, r3
006dffb4: bl       #0x534688
006dffb8: cmp      sl, #0
006dffbc: beq      #0x6dfeec
006dffc0: mov      r0, sl
006dffc4: b        #0x6dfee8
006dffc8: ldr      r1, [sp, #0x64]
006dffcc: ldr      r2, [sp, #0xc]
006dffd0: mov      r3, #0
006dffd4: cmp      r8, r1
006dffd8: ldr      r1, [sp, #8]
006dffdc: strb     r3, [r1, r2]
006dffe0: beq      #0x6dffec
006dffe4: mov      r0, r8
006dffe8: bl       #0x31d584
006dffec: add      r8, sp, #0x10
006dfff0: mov      r2, #0
006dfff4: add      r3, r8, #4
006dfff8: str      r2, [r3], #4
006dfffc: str      r2, [r3], #4
006e0000: str      r2, [r3], #4
006e0004: str      r2, [r3], #4
006e0008: str      r2, [r3], #4
006e000c: ldr      ip, [pc, #0x174]
006e0010: ldr      r0, [pc, #0x174]
006e0014: str      r2, [r3], #4
006e0018: ldr      r1, [r4, #0x2c]
006e001c: add      ip, pc, ip
006e0020: str      r2, [r3], #4
006e0024: add      r0, pc, r0
006e0028: str      r2, [r3]
006e002c: str      ip, [sp, #0x1c]
006e0030: str      r0, [sp, #0x28]
006e0034: str      r2, [sp, #0x10]
006e0038: ldr      r3, [r1, #0x88]
006e003c: tst      r3, #0x400
006e0040: bne      #0x6e0120
006e0044: ldr      r3, [pc, #0x144]
006e0048: add      r3, pc, r3
006e004c: str      r3, [sp, #0x10]
006e0050: ldr      r3, [r1, #0x88]
006e0054: tst      r3, #0x800
006e0058: bne      #0x6e0138
006e005c: ldr      r3, [pc, #0x130]
006e0060: add      r3, pc, r3
006e0064: str      r3, [sp, #0x14]
006e0068: ldr      r3, [r1, #0x88]
006e006c: tst      r3, #0x1000
006e0070: bne      #0x6e012c
006e0074: ldr      r2, [pc, #0x11c]
006e0078: add      r2, pc, r2
006e007c: ldr      r3, [r4, #0x7c]
006e0080: str      r2, [sp, #0x18]
006e0084: cmp      r3, #0
006e0088: beq      #0x6e0178
006e008c: cmp      sb, #0
006e0090: str      r3, [sp, #0x20]
006e0094: beq      #0x6e016c
006e0098: ldr      r2, [sp, #8]
006e009c: mov      r1, #0
006e00a0: mov      r0, #0x38
006e00a4: str      r2, [sp, #0x2c]
006e00a8: str      sb, [sp, #0x24]
006e00ac: bl       #0x5341ac
006e00b0: ldr      ip, [r4, #0x2c]
006e00b4: mov      r3, fp
006e00b8: mov      r2, r8
006e00bc: str      ip, [sp]
006e00c0: mov      r1, r5
006e00c4: mov      ip, #1
006e00c8: mov      r7, r0
006e00cc: str      ip, [sp, #4]
006e00d0: bl       #0x6df738
006e00d4: cmp      r7, #0
006e00d8: ldrne    r3, [r7, #4]
006e00dc: mov      r0, r7
006e00e0: addne    r3, r3, #1
006e00e4: strne    r3, [r7, #4]
006e00e8: ldrb     r3, [r7, #0x34]
006e00ec: cmp      r3, #0
006e00f0: strne    r7, [r6]
006e00f4: ldrne    r3, [r7, #4]
006e00f8: streq    r3, [r6]
006e00fc: addne    r3, r3, #1
006e0100: strne    r3, [r7, #4]
006e0104: bl       #0x31d584
006e0108: b        #0x6dffa4
006e010c: ldr      r1, [pc, #0x88]
006e0110: mov      r0, r4
006e0114: add      r1, pc, r1
006e0118: bl       #0x6e0a3c
006e011c: b        #0x6dfe90
006e0120: ldr      r3, [pc, #0x78]
006e0124: add      r3, pc, r3
006e0128: b        #0x6e004c
006e012c: ldr      r2, [pc, #0x70]
006e0130: add      r2, pc, r2
006e0134: b        #0x6e007c
006e0138: ldr      r3, [pc, #0x68]
006e013c: add      r3, pc, r3
006e0140: b        #0x6e0064
006e0144: ldr      r3, [sl]
006e0148: mov      r0, sl
006e014c: mov      r1, r7
006e0150: mov      lr, pc
006e0154: ldr      pc, [r3, #0xc]
006e0158: subs     r8, r0, #0
006e015c: bne      #0x6dff34
006e0160: ldr      r2, [sp, #0x64]
006e0164: str      r2, [r6]
006e0168: b        #0x6dffb8
006e016c: ldr      sb, [pc, #0x38]
006e0170: add      sb, pc, sb
006e0174: b        #0x6e0098
006e0178: ldr      r3, [pc, #0x30]
006e017c: add      r3, pc, r3
006e0180: b        #0x6e008c

# _ZN6glitch5video18ICodeShaderManager18makeShaderCodeNameEPKcS3_S3_Pj
006e0c10: push     {r4, r5, r6, r7, r8, sl, lr}
006e0c14: subs     r5, r2, #0
006e0c18: sub      sp, sp, #0x14
006e0c1c: mov      r8, r0
006e0c20: mov      sl, r1
006e0c24: mov      r4, r3
006e0c28: beq      #0x6e0c8c
006e0c2c: mov      r0, r5
006e0c30: bl       #0x30de54
006e0c34: mov      r6, r0
006e0c38: cmp      r4, #0
006e0c3c: beq      #0x6e0c7c
006e0c40: mov      r0, r4
006e0c44: bl       #0x30de54
006e0c48: mov      r7, r0
006e0c4c: mov      r0, sl
006e0c50: bl       #0x30de54
006e0c54: ldr      ip, [sp, #0x30]
006e0c58: mov      r2, r0
006e0c5c: mov      r1, sl
006e0c60: mov      r0, r8
006e0c64: mov      r3, r5
006e0c68: str      r6, [sp]
006e0c6c: stmib    sp, {r4, r7, ip}
006e0c70: bl       #0x6e0b64
006e0c74: add      sp, sp, #0x14
006e0c78: pop      {r4, r5, r6, r7, r8, sl, pc}
006e0c7c: mov      r7, r4
006e0c80: ldr      r4, [pc, #0x14]
006e0c84: add      r4, pc, r4
006e0c88: b        #0x6e0c4c
006e0c8c: mov      r6, r5
006e0c90: ldr      r5, [pc, #8]
006e0c94: add      r5, pc, r5
006e0c98: b        #0x6e0c38
006e0c9c: andseq   sl, lr, r4, lsl #23
006e0ca0: andseq   sl, lr, r4, ror fp

# _ZN21render_handler_glitch11draw_bitmapERKN7gameswf6matrixEPNS0_11bitmap_infoERKNS0_4rectES8_NS0_4rgbaE
007d8df4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d8df8: sub      sp, sp, #0x54
007d8dfc: ldrb     r6, [sp, #0x7f]
007d8e00: mov      r4, r0
007d8e04: mov      r5, r1
007d8e08: mov      r0, r6
007d8e0c: str      r2, [sp, #0x1c]
007d8e10: mov      sb, r3
007d8e14: bl       #0x30e964
007d8e18: mov      r1, #0
007d8e1c: bl       #0x30df8c
007d8e20: cmp      r0, #0
007d8e24: ldrb     r0, [sp, #0x7c]
007d8e28: ldr      r7, [sp, #0x78]
007d8e2c: ldrb     sl, [sp, #0x7d]
007d8e30: str      r0, [sp, #0x2c]
007d8e34: ldrb     r8, [sp, #0x7e]
007d8e38: bne      #0x7d934c
007d8e3c: ldr      r3, [r5]
007d8e40: ldr      r0, [sb]
007d8e44: mov      r1, r3
007d8e48: str      r3, [sp, #8]
007d8e4c: bl       #0x30ed6c
007d8e50: str      r0, [sp, #0x28]
007d8e54: ldr      r1, [r5, #4]
007d8e58: str      r1, [sp, #0x20]
007d8e5c: ldr      r0, [sb, #8]
007d8e60: bl       #0x30ed6c
007d8e64: ldr      fp, [r5, #8]
007d8e68: mov      r2, r0
007d8e6c: mov      r1, r2
007d8e70: ldr      r0, [sp, #0x28]
007d8e74: str      r2, [sp, #0x10]
007d8e78: bl       #0x30eba4
007d8e7c: mov      r1, fp
007d8e80: bl       #0x30eba4
007d8e84: str      r0, [sp, #0x30]
007d8e88: ldr      ip, [r5, #0xc]
007d8e8c: ldr      r0, [sb]
007d8e90: mov      r1, ip
007d8e94: str      ip, [sp, #0xc]
007d8e98: bl       #0x30ed6c
007d8e9c: str      r0, [sp, #0x38]
007d8ea0: ldr      r0, [r5, #0x10]
007d8ea4: str      r0, [sp, #0x24]
007d8ea8: ldr      r0, [sb, #8]
007d8eac: ldr      r1, [sp, #0x24]
007d8eb0: bl       #0x30ed6c
007d8eb4: str      r0, [sp, #0x3c]
007d8eb8: ldr      r5, [r5, #0x14]
007d8ebc: ldr      r1, [sp, #0x3c]
007d8ec0: ldr      r0, [sp, #0x38]
007d8ec4: bl       #0x30eba4
007d8ec8: mov      r1, r5
007d8ecc: bl       #0x30eba4
007d8ed0: ldr      r3, [sp, #8]
007d8ed4: str      r0, [sp, #0x34]
007d8ed8: ldr      r0, [sb, #4]
007d8edc: mov      r1, r3
007d8ee0: bl       #0x30ed6c
007d8ee4: ldr      r2, [sp, #0x10]
007d8ee8: mov      r1, r0
007d8eec: mov      r0, r2
007d8ef0: bl       #0x30eba4
007d8ef4: mov      r1, r0
007d8ef8: mov      r0, fp
007d8efc: bl       #0x30eba4
007d8f00: ldr      ip, [sp, #0xc]
007d8f04: str      r0, [sp, #0x14]
007d8f08: ldr      r0, [sb, #4]
007d8f0c: mov      r1, ip
007d8f10: bl       #0x30ed6c
007d8f14: mov      r1, r0
007d8f18: ldr      r0, [sp, #0x3c]
007d8f1c: bl       #0x30eba4
007d8f20: mov      r1, r0
007d8f24: mov      r0, r5
007d8f28: bl       #0x30eba4
007d8f2c: str      r0, [sp, #0x18]
007d8f30: ldr      r3, [sb, #0xc]
007d8f34: ldr      r1, [sp, #0x20]
007d8f38: mov      r0, r3
007d8f3c: str      r3, [sp, #8]
007d8f40: bl       #0x30ed6c
007d8f44: mov      r1, r0
007d8f48: ldr      r0, [sp, #0x28]
007d8f4c: bl       #0x30eba4
007d8f50: mov      r1, r0
007d8f54: mov      r0, fp
007d8f58: bl       #0x30eba4
007d8f5c: ldr      r3, [sp, #8]
007d8f60: ldr      r1, [sp, #0x24]
007d8f64: mov      sb, r0
007d8f68: mov      r0, r3
007d8f6c: bl       #0x30ed6c
007d8f70: mov      r1, r0
007d8f74: ldr      r0, [sp, #0x38]
007d8f78: bl       #0x30eba4
007d8f7c: mov      r1, r0
007d8f80: mov      r0, r5
007d8f84: bl       #0x30eba4
007d8f88: mov      r1, sb
007d8f8c: mov      r5, r0
007d8f90: ldr      r0, [sp, #0x14]
007d8f94: bl       #0x30eba4
007d8f98: ldr      r1, [sp, #0x30]
007d8f9c: bl       #0x30e3ac
007d8fa0: mov      r1, r5
007d8fa4: str      r0, [sp, #0x28]
007d8fa8: ldr      r0, [sp, #0x18]
007d8fac: bl       #0x30eba4
007d8fb0: ldr      r1, [sp, #0x34]
007d8fb4: bl       #0x30e3ac
007d8fb8: ldr      r1, [sp, #0x1c]
007d8fbc: str      r0, [sp, #0x24]
007d8fc0: ldr      r3, [r1]
007d8fc4: mov      r0, r1
007d8fc8: mov      lr, pc
007d8fcc: ldr      pc, [r3, #8]
007d8fd0: ldr      r2, [sp, #0x1c]
007d8fd4: ldr      r0, [r2, #0x10]
007d8fd8: cmp      r0, #0
007d8fdc: beq      #0x7d8fe8
007d8fe0: mov      r1, #1
007d8fe4: bl       #0x7d3bb4
007d8fe8: ldr      ip, [sp, #0x1c]
007d8fec: add      r3, r4, #0x1f0
007d8ff0: mov      r0, r3
007d8ff4: add      r1, ip, #0x10
007d8ff8: str      r3, [sp, #0x20]
007d8ffc: bl       #0x7d6a48
007d9000: ldr      r3, [r4, #0x374]
007d9004: ldr      r2, [r4, #0x348]
007d9008: ldr      r0, [sp, #0x30]
007d900c: movw     fp, #0x6667
007d9010: str      r2, [r3, #0x14]
007d9014: str      r0, [r3, #0xc]
007d9018: ldr      r1, [sp, #0x34]
007d901c: movt     fp, #0x6666
007d9020: str      r1, [r3, #0x10]
007d9024: ldr      r3, [r4, #0x374]
007d9028: ldr      r2, [r4, #0x348]
007d902c: add      r3, r3, #0x18
007d9030: str      r2, [r3, #0x14]
007d9034: ldr      r2, [sp, #0x14]
007d9038: str      r2, [r3, #0xc]
007d903c: ldr      ip, [sp, #0x18]
007d9040: str      ip, [r3, #0x10]
007d9044: ldr      r2, [r4, #0x374]
007d9048: ldr      r1, [r4, #0x348]
007d904c: mov      r3, #0
007d9050: add      r2, r2, #0x30
007d9054: str      r5, [r2, #0x10]
007d9058: str      r1, [r2, #0x14]
007d905c: str      sb, [r2, #0xc]
007d9060: ldr      r2, [r4, #0x374]
007d9064: ldr      r1, [r4, #0x348]
007d9068: mov      r5, #0x14
007d906c: add      r2, r2, #0x48
007d9070: str      r1, [r2, #0x14]
007d9074: ldr      r0, [sp, #0x28]
007d9078: str      r0, [r2, #0xc]
007d907c: ldr      r1, [sp, #0x24]
007d9080: str      r1, [r2, #0x10]
007d9084: ldr      r0, [r7]
007d9088: ldr      r1, [r7, #8]
007d908c: ldr      r2, [r4, #0x374]
007d9090: str      r0, [r2]
007d9094: str      r1, [r2, #4]
007d9098: ldr      r1, [r7, #8]
007d909c: ldr      r2, [r4, #0x374]
007d90a0: ldr      r0, [r7, #4]
007d90a4: str      r0, [r2, #0x18]
007d90a8: str      r1, [r2, #0x1c]
007d90ac: ldr      r1, [r7, #0xc]
007d90b0: ldr      r0, [r7]
007d90b4: ldr      r2, [r4, #0x374]
007d90b8: str      r0, [r2, #0x30]
007d90bc: str      r1, [r2, #0x34]
007d90c0: ldr      r0, [r7, #0xc]
007d90c4: ldr      r1, [r7, #4]
007d90c8: ldr      r2, [r4, #0x374]
007d90cc: mov      r7, r3
007d90d0: str      r0, [r2, #0x4c]
007d90d4: str      r1, [r2, #0x48]
007d90d8: str      fp, [sp, #0x14]
007d90dc: mov      fp, r6
007d90e0: ldr      r6, [sp, #0x2c]
007d90e4: ldr      r3, [r4, #0x374]
007d90e8: add      r3, r3, r7
007d90ec: strb     r6, [r3, #8]
007d90f0: strb     fp, [r3, #0xb]
007d90f4: strb     r8, [r3, #0xa]
007d90f8: strb     sl, [r3, #9]
007d90fc: ldrb     r3, [r4, #4]
007d9100: cmp      r3, #0
007d9104: beq      #0x7d9168
007d9108: ldr      sb, [r4, #0x374]
007d910c: add      sb, sb, r7
007d9110: ldr      r0, [sb, #0xc]
007d9114: bl       #0x30e4cc
007d9118: ldr      ip, [sp, #0x14]
007d911c: add      r0, r0, #0xa
007d9120: smull    ip, r3, ip, r0
007d9124: asr      r0, r0, #0x1f
007d9128: rsb      r0, r0, r3, asr #3
007d912c: mul      r0, r5, r0
007d9130: bl       #0x30e964
007d9134: str      r0, [sb, #0xc]
007d9138: ldr      sb, [r4, #0x374]
007d913c: add      sb, sb, r7
007d9140: ldr      r0, [sb, #0x10]
007d9144: bl       #0x30e4cc
007d9148: ldr      r1, [sp, #0x14]
007d914c: add      r0, r0, #0xa
007d9150: smull    r1, r3, r1, r0
007d9154: asr      r0, r0, #0x1f
007d9158: rsb      r0, r0, r3, asr #3
007d915c: mul      r0, r5, r0
007d9160: bl       #0x30e964
007d9164: str      r0, [sb, #0x10]
007d9168: add      r7, r7, #0x18
007d916c: cmp      r7, #0x60
007d9170: bne      #0x7d90e4
007d9174: ldr      r3, [pc, #0x22c]
007d9178: ldr      r1, [r4, #0x378]
007d917c: mov      r2, #4
007d9180: add      r3, pc, r3
007d9184: ldr      ip, [r3, #0x18]
007d9188: ldr      r0, [r3, #0x1c]
007d918c: str      r2, [r1, #8]
007d9190: ldr      lr, [r3, #0x20]
007d9194: add      r5, sp, #0x50
007d9198: ldr      r1, [r4, #0x374]
007d919c: str      ip, [r5, #-0xc]!
007d91a0: add      ip, sp, #0x48
007d91a4: str      r0, [ip], #4
007d91a8: str      lr, [ip]
007d91ac: mov      r6, #6
007d91b0: mov      r0, r4
007d91b4: mov      r3, r5
007d91b8: str      r6, [sp]
007d91bc: str      r6, [sp, #4]
007d91c0: bl       #0x7d860c
007d91c4: cmp      r0, #0
007d91c8: beq      #0x7d9354
007d91cc: ldr      r6, [r4, #0xc]
007d91d0: cmp      r6, #0
007d91d4: beq      #0x7d934c
007d91d8: ldr      r2, [r6, #0x44]
007d91dc: str      r2, [sp, #0x14]
007d91e0: ldr      r7, [r4, #0x374]
007d91e4: adds     r8, r2, #6
007d91e8: ldr      fp, [r6, #0x24]
007d91ec: add      r4, r7, #0xc
007d91f0: beq      #0x7d9200
007d91f4: ldr      r3, [r6, #0x48]
007d91f8: cmp      r8, r3
007d91fc: bgt      #0x7d9378
007d9200: ldr      ip, [sp, #0x14]
007d9204: mov      r2, #0
007d9208: lsl      r3, ip, #1
007d920c: ldr      r0, [r6, #0x40]
007d9210: add      r1, r3, r2
007d9214: add      r2, r2, #2
007d9218: mov      ip, #0
007d921c: cmp      r2, #0xc
007d9220: strh     ip, [r0, r1]
007d9224: bne      #0x7d920c
007d9228: ldr      r0, [r6, #0x40]
007d922c: mov      r1, r5
007d9230: str      r8, [r6, #0x44]
007d9234: add      r0, r0, r3
007d9238: bl       #0x30e868
007d923c: ldr      r5, [r6, #0x24]
007d9240: adds     r5, r5, #4
007d9244: beq      #0x7d9254
007d9248: ldr      r3, [r6, #0x28]
007d924c: cmp      r5, r3
007d9250: bgt      #0x7d9398
007d9254: ldr      sl, [r6, #0x34]
007d9258: str      r5, [r6, #0x24]
007d925c: adds     sl, sl, #4
007d9260: beq      #0x7d9270
007d9264: ldr      r3, [r6, #0x38]
007d9268: cmp      sl, r3
007d926c: bgt      #0x7d9388
007d9270: ldr      r3, [r6, #0x20]
007d9274: ldr      sb, [r6, #0x30]
007d9278: mov      r8, #0xc
007d927c: mla      r8, r8, fp, r3
007d9280: mov      r5, #0
007d9284: str      sl, [r6, #0x34]
007d9288: add      sb, sb, fp, lsl #3
007d928c: mov      r1, r5
007d9290: mov      r2, r5
007d9294: ldr      r3, [r4, r2]
007d9298: add      r0, r4, r2
007d929c: add      r0, r0, #4
007d92a0: str      r3, [r8, r1]
007d92a4: ldr      ip, [r0], #4
007d92a8: add      r3, r8, r1
007d92ac: add      r3, r3, #4
007d92b0: str      ip, [r3], #4
007d92b4: ldr      sl, [r0]
007d92b8: mov      ip, r7
007d92bc: mov      r0, sb
007d92c0: str      sl, [r3]
007d92c4: ldr      r3, [ip, r2]!
007d92c8: add      r2, r2, #0x18
007d92cc: cmp      r2, #0x480
007d92d0: str      r3, [r0, r5]!
007d92d4: ldr      r3, [ip, #4]
007d92d8: add      r1, r1, #0xc
007d92dc: add      r5, r5, #8
007d92e0: str      r3, [r0, #4]
007d92e4: bne      #0x7d9294
007d92e8: ldr      r3, [r6, #0x14]
007d92ec: ldr      r2, [r6, #0x18]
007d92f0: add      r4, r3, #1
007d92f4: cmp      r4, r2
007d92f8: ble      #0x7d930c
007d92fc: add      r0, r6, #0x10
007d9300: add      r1, r4, r4, asr #1
007d9304: bl       #0x78a6f8
007d9308: ldr      r3, [r6, #0x14]
007d930c: mov      r2, #0x18
007d9310: ldr      r1, [r6, #0x10]
007d9314: mul      r2, r2, r3
007d9318: ldr      r0, [sp, #0x2c]
007d931c: add      r3, r1, r2
007d9320: str      r0, [r3, #4]
007d9324: ldr      ip, [sp, #0x1c]
007d9328: str      ip, [r1, r2]
007d932c: mov      r2, #6
007d9330: str      r2, [r3, #0x14]
007d9334: str      fp, [r3, #8]
007d9338: ldr      r0, [sp, #0x14]
007d933c: mov      r2, #4
007d9340: str      r2, [r3, #0xc]
007d9344: str      r0, [r3, #0x10]
007d9348: str      r4, [r6, #0x14]
007d934c: add      sp, sp, #0x54
007d9350: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d9354: mov      r3, r6
007d9358: ldr      r0, [sp, #0x20]
007d935c: add      r1, r4, #0x378
007d9360: mov      r2, r5
007d9364: bl       #0x7d7094
007d9368: ldr      r6, [r4, #0xc]
007d936c: cmp      r6, #0
007d9370: bne      #0x7d91d8
007d9374: b        #0x7d934c
007d9378: add      r0, r6, #0x40
007d937c: add      r1, r8, r8, asr #1
007d9380: bl       #0x779e7c
007d9384: b        #0x7d9200
007d9388: add      r0, r6, #0x30
007d938c: add      r1, sl, sl, asr #1
007d9390: bl       #0x7d4208
007d9394: b        #0x7d9270
007d9398: add      r0, r6, #0x20
007d939c: add      r1, r5, r5, asr #1
007d93a0: bl       #0x7d4180
007d93a4: b        #0x7d9254
007d93a8: ldrheq   r2, [r3], -ip

# _ZN6glitch5video15CGLSLShaderCode12createShaderEiPPKci
006df560: push     {r4, r5, lr}
006df564: mov      r4, r0
006df568: ldr      r0, [r0, #0x30]
006df56c: sub      sp, sp, #0xc
006df570: mov      r5, r3
006df574: cmp      r0, #0
006df578: bne      #0x6df590
006df57c: mov      r0, r1
006df580: str      r2, [sp, #4]
006df584: bl       #0x30e538
006df588: str      r0, [r4, #0x30]
006df58c: ldr      r2, [sp, #4]
006df590: mov      r1, r5
006df594: mov      r3, #0
006df598: add      sp, sp, #0xc
006df59c: pop      {r4, r5, lr}
006df5a0: b        #0x30ed84

# _ZN21render_handler_glitch14set_blend_modeEN7gameswf10blend_mode2idE
007d81e4: push     {r4, r5, r6, lr}
007d81e8: ldr      r3, [r0, #0x2fc]
007d81ec: mov      r4, r0
007d81f0: mov      r5, r1
007d81f4: cmp      r3, #0xf
007d81f8: beq      #0x7d8258
007d81fc: ldr      r2, [r0, #0x344]
007d8200: cmp      r2, #0
007d8204: addle    r6, r0, #0x1f0
007d8208: ble      #0x7d8228
007d820c: add      r6, r0, #0x1f0
007d8210: mov      r0, r6
007d8214: bl       #0x7d6894
007d8218: mov      r0, r6
007d821c: mov      r1, #0
007d8220: bl       #0x7d7f28
007d8224: ldr      r3, [r4, #0x2fc]
007d8228: cmp      r3, r5
007d822c: beq      #0x7d8238
007d8230: mov      r0, r6
007d8234: bl       #0x7d6894
007d8238: ldr      r3, [r4, #0x344]
007d823c: str      r5, [r4, #0x2fc]
007d8240: cmp      r3, #0
007d8244: ble      #0x7d8258
007d8248: mov      r0, r6
007d824c: mov      r1, #2
007d8250: pop      {r4, r5, r6, lr}
007d8254: b        #0x7d7f28
007d8258: pop      {r4, r5, r6, pc}

# _ZN21render_handler_glitchC1EPN6glitch5video12IVideoDriverE
007d60f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d60f8: ldr      sl, [pc, #0x524]
007d60fc: ldr      r2, [pc, #0x524]
007d6100: mov      r3, #0x1f0000
007d6104: add      sl, pc, sl
007d6108: ldr      r2, [sl, r2]
007d610c: mov      r4, r0
007d6110: mov      r5, #0
007d6114: mov      r7, #0x3f800000
007d6118: mov      r6, #0
007d611c: add      r3, r3, #0xff
007d6120: add      r2, r2, #8
007d6124: mov      fp, #1
007d6128: str      r3, [r0, #0x14]
007d612c: str      r2, [r0]
007d6130: sub      sp, sp, #0x74
007d6134: strb     fp, [r4, #4]
007d6138: str      r5, [r0, #8]
007d613c: str      r5, [r0, #0xc]
007d6140: str      r1, [r4, #0x10]
007d6144: strb     r5, [r0, #0x18]
007d6148: strb     r5, [r0, #0x19]
007d614c: strb     r5, [r0, #0x1a]
007d6150: strb     r5, [r0, #0x1b]
007d6154: str      r7, [r0, #0x1c]
007d6158: str      r6, [r0, #0x20]
007d615c: str      r7, [r0, #0x24]
007d6160: str      r5, [r0, #0x28]
007d6164: str      r5, [r0, #0x2c]
007d6168: str      r5, [r0, #0x30]
007d616c: str      r5, [r0, #0x34]
007d6170: add      r0, r0, #0x38
007d6174: mov      sb, r1
007d6178: add      r8, r4, #0x1f0
007d617c: bl       #0x7d6050
007d6180: add      r0, r4, #0x114
007d6184: bl       #0x7d6050
007d6188: mov      r1, sb
007d618c: mov      r0, r8
007d6190: bl       #0x7d4768
007d6194: strb     r5, [r4, #0x300]
007d6198: strb     r5, [r4, #0x301]
007d619c: str      r6, [r4, #0x304]
007d61a0: str      r6, [r4, #0x308]
007d61a4: str      r5, [r4, #0x310]
007d61a8: str      r5, [r4, #0x314]
007d61ac: str      r5, [r4, #0x318]
007d61b0: str      r5, [r4, #0x320]
007d61b4: str      r7, [r4, #0x30c]
007d61b8: str      r7, [r4, #0x31c]
007d61bc: str      r7, [r4, #0x324]
007d61c0: str      r7, [r4, #0x32c]
007d61c4: str      r7, [r4, #0x334]
007d61c8: str      r7, [r4, #0x33c]
007d61cc: str      r6, [r4, #0x328]
007d61d0: str      r6, [r4, #0x330]
007d61d4: str      r6, [r4, #0x338]
007d61d8: str      r6, [r4, #0x340]
007d61dc: str      r5, [r4, #0x344]
007d61e0: str      r6, [r4, #0x348]
007d61e4: str      r5, [r4, #0x34c]
007d61e8: str      r5, [r4, #0x350]
007d61ec: str      r5, [r4, #0x354]
007d61f0: strb     r5, [r4, #0x358]
007d61f4: str      r5, [r4, #0x35c]
007d61f8: str      r5, [r4, #0x360]
007d61fc: str      r5, [r4, #0x364]
007d6200: strb     r5, [r4, #0x368]
007d6204: add      r0, r4, #0x36c
007d6208: bl       #0x785b08
007d620c: mov      r1, fp
007d6210: add      r0, r4, #0x378
007d6214: mov      r2, #0x40000
007d6218: str      r5, [r4, #0x370]
007d621c: str      r5, [r4, #0x374]
007d6220: bl       #0x5a1404
007d6224: ldr      ip, [pc, #0x400]
007d6228: mov      r2, #4
007d622c: mov      r3, r2
007d6230: mov      r1, sb
007d6234: add      ip, pc, ip
007d6238: add      r0, r4, #0x37c
007d623c: str      ip, [sp]
007d6240: bl       #0x7d5404
007d6244: add      r3, r4, #0x3b0
007d6248: add      r1, r3, #0xe4
007d624c: mvn      r2, #0
007d6250: str      r5, [r3]
007d6254: strb     r2, [r3, #4]
007d6258: strb     r2, [r3, #5]
007d625c: strb     r2, [r3, #6]
007d6260: strb     r2, [r3, #7]
007d6264: str      r5, [r3, #0x10]
007d6268: str      r5, [r3, #0x14]
007d626c: str      r5, [r3, #0x18]
007d6270: str      r5, [r3, #0x20]
007d6274: str      r7, [r3, #0xc]
007d6278: str      r7, [r3, #0x1c]
007d627c: str      r7, [r3, #0x24]
007d6280: str      r7, [r3, #0x2c]
007d6284: str      r7, [r3, #0x34]
007d6288: str      r7, [r3, #0x3c]
007d628c: str      r6, [r3, #0x28]
007d6290: str      r6, [r3, #0x30]
007d6294: str      r6, [r3, #0x38]
007d6298: str      r6, [r3, #0x40]
007d629c: strb     r5, [r3, #0x44]
007d62a0: add      r3, r3, #0x4c
007d62a4: cmp      r3, r1
007d62a8: bne      #0x7d6250
007d62ac: ldr      r3, [r4, #0x10]
007d62b0: cmp      r3, #0
007d62b4: beq      #0x7d6418
007d62b8: ldr      r1, [r3, #4]
007d62bc: mov      lr, #1
007d62c0: mov      r2, r5
007d62c4: add      r1, r1, #1
007d62c8: str      r1, [r3, #4]
007d62cc: ldr      ip, [r4, #0x10]
007d62d0: add      r0, sp, #0x6c
007d62d4: mov      r3, #4
007d62d8: mov      r1, ip
007d62dc: ldr      ip, [ip]
007d62e0: str      lr, [sp, #8]
007d62e4: str      r5, [sp]
007d62e8: str      r5, [sp, #4]
007d62ec: mov      lr, pc
007d62f0: ldr      pc, [ip, #0x78]
007d62f4: ldr      r3, [sp, #0x6c]
007d62f8: ldr      r0, [r4, #0x378]
007d62fc: mov      ip, #0xc
007d6300: cmp      r3, #0
007d6304: str      r3, [sp, #0x34]
007d6308: ldrne    r2, [r3, #4]
007d630c: add      r1, r0, #0x14
007d6310: addne    r2, r2, #1
007d6314: strne    r2, [r3, #4]
007d6318: str      ip, [sp, #0x38]
007d631c: mov      ip, #6
007d6320: str      ip, [sp, #0x3c]
007d6324: mov      ip, #3
007d6328: strh     ip, [sp, #0x40]
007d632c: add      r2, sp, #0x34
007d6330: mov      ip, #0x18
007d6334: mov      r3, #1
007d6338: strh     ip, [sp, #0x42]
007d633c: bl       #0x7d46fc
007d6340: ldr      r0, [sp, #0x34]
007d6344: cmp      r0, #0
007d6348: beq      #0x7d6350
007d634c: bl       #0x31d584
007d6350: ldr      r3, [sp, #0x6c]
007d6354: ldr      r0, [r4, #0x378]
007d6358: mov      ip, #0
007d635c: cmp      r3, #0
007d6360: str      r3, [sp, #0x24]
007d6364: ldrne    r2, [r3, #4]
007d6368: add      r1, r0, #0x24
007d636c: addne    r2, r2, #1
007d6370: strne    r2, [r3, #4]
007d6374: str      ip, [sp, #0x28]
007d6378: mov      ip, #6
007d637c: str      ip, [sp, #0x2c]
007d6380: mov      ip, #2
007d6384: strh     ip, [sp, #0x30]
007d6388: add      r2, sp, #0x24
007d638c: mov      ip, #0x18
007d6390: mov      r3, #1
007d6394: strh     ip, [sp, #0x32]
007d6398: bl       #0x7d46fc
007d639c: ldr      r0, [sp, #0x24]
007d63a0: cmp      r0, #0
007d63a4: beq      #0x7d63ac
007d63a8: bl       #0x31d584
007d63ac: ldr      r3, [sp, #0x6c]
007d63b0: ldr      r0, [r4, #0x378]
007d63b4: mov      ip, #8
007d63b8: cmp      r3, #0
007d63bc: str      r3, [sp, #0x14]
007d63c0: ldrne    r2, [r3, #4]
007d63c4: add      r1, r0, #0x34
007d63c8: addne    r2, r2, #1
007d63cc: strne    r2, [r3, #4]
007d63d0: str      ip, [sp, #0x18]
007d63d4: mov      ip, #1
007d63d8: str      ip, [sp, #0x1c]
007d63dc: mov      ip, #4
007d63e0: strh     ip, [sp, #0x20]
007d63e4: add      r2, sp, #0x14
007d63e8: mov      ip, #0x18
007d63ec: mov      r3, #0
007d63f0: strh     ip, [sp, #0x22]
007d63f4: bl       #0x7d46fc
007d63f8: ldr      r0, [sp, #0x14]
007d63fc: cmp      r0, #0
007d6400: beq      #0x7d6408
007d6404: bl       #0x31d584
007d6408: ldr      r0, [sp, #0x6c]
007d640c: cmp      r0, #0
007d6410: beq      #0x7d6418
007d6414: bl       #0x31d584
007d6418: mov      r0, r4
007d641c: mov      r1, #0x100
007d6420: bl       #0x7d4340
007d6424: ldr      r3, [pc, #0x204]
007d6428: ldr      r1, [pc, #0x204]
007d642c: add      r5, sp, #0x44
007d6430: ldr      r2, [sl, r3]
007d6434: add      r1, pc, r1
007d6438: mov      r0, r5
007d643c: bl       #0x60f25c
007d6440: ldr      r3, [pc, #0x1f0]
007d6444: add      r7, sp, #0x68
007d6448: mov      ip, #0
007d644c: ldr      r2, [r4, #0x10]
007d6450: mov      r0, r7
007d6454: mov      r1, r5
007d6458: add      r3, pc, r3
007d645c: str      ip, [sp]
007d6460: bl       #0x61b10c
007d6464: ldr      r3, [sp, #0x68]
007d6468: add      r6, sp, #0x64
007d646c: mov      r1, r6
007d6470: cmp      r3, #0
007d6474: str      r3, [sp, #0x64]
007d6478: ldrne    r2, [r3]
007d647c: mov      r0, r8
007d6480: addne    r2, r2, #1
007d6484: strne    r2, [r3]
007d6488: ldr      r3, [pc, #0x1ac]
007d648c: mov      r2, #0
007d6490: add      r3, pc, r3
007d6494: bl       #0x7d4ab0
007d6498: mov      r0, r6
007d649c: bl       #0x3522b8
007d64a0: ldr      r3, [sp, #0x68]
007d64a4: add      r6, sp, #0x60
007d64a8: mov      r1, r6
007d64ac: cmp      r3, #0
007d64b0: str      r3, [sp, #0x60]
007d64b4: ldrne    r2, [r3]
007d64b8: mov      r0, r8
007d64bc: addne    r2, r2, #1
007d64c0: strne    r2, [r3]
007d64c4: ldr      r3, [pc, #0x174]
007d64c8: mov      r2, #1
007d64cc: add      r3, pc, r3
007d64d0: bl       #0x7d4ab0
007d64d4: mov      r0, r6
007d64d8: bl       #0x3522b8
007d64dc: ldr      r3, [sp, #0x68]
007d64e0: add      r6, sp, #0x5c
007d64e4: mov      r1, r6
007d64e8: cmp      r3, #0
007d64ec: str      r3, [sp, #0x5c]
007d64f0: ldrne    r2, [r3]
007d64f4: mov      r0, r8
007d64f8: addne    r2, r2, #1
007d64fc: strne    r2, [r3]
007d6500: ldr      r3, [pc, #0x13c]
007d6504: mov      r2, #3
007d6508: add      r3, pc, r3
007d650c: bl       #0x7d4ab0
007d6510: mov      r0, r6
007d6514: bl       #0x3522b8
007d6518: ldr      r3, [sp, #0x68]
007d651c: add      r6, sp, #0x58
007d6520: mov      r1, r6
007d6524: cmp      r3, #0
007d6528: str      r3, [sp, #0x58]
007d652c: ldrne    r2, [r3]
007d6530: mov      r0, r8
007d6534: addne    r2, r2, #1
007d6538: strne    r2, [r3]
007d653c: ldr      r3, [pc, #0x104]
007d6540: mov      r2, #4
007d6544: add      r3, pc, r3
007d6548: bl       #0x7d4ab0
007d654c: mov      r0, r6
007d6550: bl       #0x3522b8
007d6554: ldr      r3, [sp, #0x68]
007d6558: add      r6, sp, #0x54
007d655c: mov      r1, r6
007d6560: cmp      r3, #0
007d6564: str      r3, [sp, #0x54]
007d6568: ldrne    r2, [r3]
007d656c: mov      r0, r8
007d6570: addne    r2, r2, #1
007d6574: strne    r2, [r3]
007d6578: ldr      r3, [pc, #0xcc]
007d657c: mov      r2, #0xd
007d6580: add      r3, pc, r3
007d6584: bl       #0x7d4ab0
007d6588: mov      r0, r6
007d658c: bl       #0x3522b8
007d6590: ldr      r3, [sp, #0x68]
007d6594: add      r6, sp, #0x50
007d6598: mov      r1, r6
007d659c: cmp      r3, #0
007d65a0: str      r3, [sp, #0x50]
007d65a4: ldrne    r2, [r3]
007d65a8: mov      r0, r8
007d65ac: addne    r2, r2, #1
007d65b0: strne    r2, [r3]
007d65b4: ldr      r3, [pc, #0x94]
007d65b8: mov      r2, #0xf
007d65bc: add      r3, pc, r3
007d65c0: bl       #0x7d4ab0
007d65c4: mov      r0, r6
007d65c8: bl       #0x3522b8
007d65cc: ldr      r3, [sp, #0x68]
007d65d0: add      r6, sp, #0x4c
007d65d4: mov      r1, r6
007d65d8: cmp      r3, #0
007d65dc: str      r3, [sp, #0x4c]
007d65e0: ldrne    r2, [r3]
007d65e4: mov      r0, r8
007d65e8: addne    r2, r2, #1
007d65ec: strne    r2, [r3]
007d65f0: ldr      r3, [pc, #0x5c]
007d65f4: mov      r2, #0x10
007d65f8: add      r3, pc, r3
007d65fc: bl       #0x7d4ab0
007d6600: mov      r0, r6
007d6604: bl       #0x3522b8
007d6608: mov      r0, r7
007d660c: bl       #0x3522b8
007d6610: mov      r0, r5
007d6614: bl       #0x619474
007d6618: mov      r0, r4
007d661c: add      sp, sp, #0x74
007d6620: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d6624: andseq   lr, fp, ip, lsl #19
007d6628: andeq    r4, r0, ip, lsl #4
007d662c: andseq   r7, ip, r4, lsr #31
007d6630: andeq    r4, r0, r0, lsl r7
007d6634: andseq   r5, r3, ip, ror ip
007d6638: andseq   r5, r3, r0, ror ip
007d663c: andeq    sl, lr, r0, lsr r4
007d6640: strdeq   sl, fp, [lr], -r4
007d6644: andseq   r5, r3, r8, asr r4
007d6648: mulseq   r3, r4, fp
007d664c: andseq   r5, r3, r0, ror #22
007d6650: andeq    sl, lr, r4, lsl #6
007d6654: andeq    sl, lr, r8, asr #5

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
