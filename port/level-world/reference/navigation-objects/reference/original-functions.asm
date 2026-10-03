
# _GLOBAL__I_.._.._sources_Utils_Point3D.cpp
00312e00: ldr      r2, [pc, #0x7c]
00312e04: ldr      r3, [pc, #0x7c]
00312e08: push     {r4, r5, r6, r7}
00312e0c: add      r2, pc, r2
00312e10: ldr      r0, [r2, r3]
00312e14: ldr      r3, [pc, #0x70]
00312e18: ldr      r1, [pc, #0x70]
00312e1c: mov      r7, #0x3f000000
00312e20: ldr      r6, [r2, r3]
00312e24: ldr      r3, [pc, #0x68]
00312e28: mov      r4, #0x3f800000
00312e2c: add      r1, pc, r1
00312e30: ldr      r5, [r2, r3]
00312e34: ldr      r3, [pc, #0x5c]
00312e38: str      r7, [r1, #8]
00312e3c: str      r4, [r0, #8]
00312e40: ldr      ip, [r2, r3]
00312e44: mov      r3, #0
00312e48: str      r3, [r6, #8]
00312e4c: str      r3, [r5, #8]
00312e50: str      r3, [ip, #8]
00312e54: str      r3, [r0, #4]
00312e58: str      r7, [r1]
00312e5c: str      r7, [r1, #4]
00312e60: str      r3, [r6]
00312e64: str      r3, [r6, #4]
00312e68: str      r4, [r5]
00312e6c: str      r3, [r5, #4]
00312e70: str      r3, [ip]
00312e74: str      r4, [ip, #4]
00312e78: str      r3, [r0]
00312e7c: pop      {r4, r5, r6, r7}
00312e80: bx       lr
00312e84: rsbeq    r1, r8, r4, lsl #25
00312e88: andeq    r4, r0, r0, asr #6
00312e8c: andeq    r3, r0, ip, lsr #30
00312e90: rsbeq    ip, r8, r4, lsl sl
00312e94: andeq    r3, r0, r8, lsr r2
00312e98: ldrdeq   r4, r5, [r0], -r8

# _ZN7PFWorld10InitObjectER8PFObjectbRK7Point3DIfEfPv
00526b18: push     {r4, r5, r6, r7, r8, lr}
00526b1c: sub      sp, sp, #0x10
00526b20: subs     r6, r2, #0
00526b24: ldr      r2, [sp, #0x2c]
00526b28: mov      r7, r3
00526b2c: ldr      r3, [r1, #4]
00526b30: str      r2, [r1]
00526b34: ldr      r8, [sp, #0x28]
00526b38: orrne    r3, r3, #1
00526b3c: biceq    r3, r3, #1
00526b40: mov      r4, r1
00526b44: mov      r5, r0
00526b48: str      r3, [r1, #4]
00526b4c: mov      r0, r8
00526b50: mov      r1, #0x3f800000
00526b54: bl       #0x30e70c
00526b58: cmp      r0, #0
00526b5c: movne    r8, #0x3f800000
00526b60: str      r8, [r4, #8]
00526b64: ldr      r3, [r7]
00526b68: cmp      r6, #0
00526b6c: add      ip, r4, #0xc
00526b70: str      r3, [r4, #0x18]
00526b74: ldr      r3, [r7, #4]
00526b78: add      r1, r4, #0x18
00526b7c: addeq    r2, r4, #0x20
00526b80: str      r3, [r4, #0x1c]
00526b84: ldr      r3, [r7, #8]
00526b88: movne    r2, #0
00526b8c: mov      r0, r5
00526b90: str      r3, [r4, #0x20]
00526b94: add      r3, r4, #0x24
00526b98: str      ip, [sp]
00526b9c: add      r4, r4, #0x10
00526ba0: mov      ip, #0
00526ba4: stmib    sp, {r4, ip}
00526ba8: bl       #0x525508
00526bac: add      sp, sp, #0x10
00526bb0: pop      {r4, r5, r6, r7, r8, pc}

# _ZN8PFObject9SetFlyingEb
005241f4: ldr      r3, [r0, #0x14]
005241f8: cmp      r1, #0
005241fc: orrne    r3, r3, #1
00524200: biceq    r3, r3, #1
00524204: str      r3, [r0, #0x14]
00524208: bx       lr

# _ZN8PFObject11SetSwimmingEb
00524218: ldr      r3, [r0, #0x14]
0052421c: cmp      r1, #0
00524220: orrne    r3, r3, #2
00524224: biceq    r3, r3, #2
00524228: str      r3, [r0, #0x14]
0052422c: bx       lr

# _ZN7PFWorld25_ChangeObstacleParentListERK8PFObjectP7PFFloor
00528484: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00528488: ldr      r3, [r1, #4]
0052848c: sub      sp, sp, #0x7c
00528490: mov      r5, r1
00528494: tst      r3, #4
00528498: str      r2, [sp, #0x14]
0052849c: beq      #0x52859c
005284a0: ldr      r3, [r1, #0x10]
005284a4: cmp      r3, r2
005284a8: beq      #0x52859c
005284ac: add      r6, r0, #0x2c
005284b0: add      r1, r1, #0x10
005284b4: mov      r0, r6
005284b8: bl       #0x527560
005284bc: ldr      r3, [r0]
005284c0: ldr      r7, [r0, #8]
005284c4: ldr      lr, [r0, #4]
005284c8: mov      r4, r0
005284cc: str      r3, [sp, #0xc]
005284d0: ldr      ip, [r4, #0xc]
005284d4: ldr      r8, [r0, #0x1c]
005284d8: ldr      sl, [r0, #0x18]
005284dc: ldr      sb, [r0, #0x14]
005284e0: ldr      fp, [r0, #0x10]
005284e4: str      ip, [sp, #0x44]
005284e8: ldr      ip, [sp, #0xc]
005284ec: add      r3, sp, #0x6c
005284f0: add      r0, sp, #0x58
005284f4: str      ip, [sp, #0x38]
005284f8: add      r1, sp, #0x38
005284fc: add      ip, sp, #0x74
00528500: add      r2, sp, #0x48
00528504: str      ip, [sp]
00528508: str      r7, [sp, #0x40]
0052850c: str      lr, [sp, #0x3c]
00528510: str      r8, [sp, #0x54]
00528514: str      sl, [sp, #0x50]
00528518: str      sb, [sp, #0x4c]
0052851c: str      fp, [sp, #0x48]
00528520: str      r5, [sp, #0x6c]
00528524: bl       #0x5262c4
00528528: ldr      ip, [sp, #0x58]
0052852c: ldr      r3, [r4, #0x10]
00528530: cmp      r3, ip
00528534: beq      #0x52859c
00528538: ldr      lr, [sp, #0x64]
0052853c: add      r2, sp, #0x18
00528540: mov      r1, r4
00528544: str      lr, [sp, #0x24]
00528548: ldr      lr, [sp, #0x60]
0052854c: add      r3, sp, #0x70
00528550: add      r0, sp, #0x28
00528554: str      lr, [sp, #0x20]
00528558: ldr      lr, [sp, #0x5c]
0052855c: str      ip, [sp, #0x18]
00528560: str      lr, [sp, #0x1c]
00528564: bl       #0x52689c
00528568: add      r1, sp, #0x14
0052856c: mov      r0, r6
00528570: bl       #0x527560
00528574: str      r5, [sp, #0x68]
00528578: ldr      r1, [r0, #0x18]
0052857c: ldr      r2, [r0, #0x10]
00528580: sub      r1, r1, #4
00528584: cmp      r2, r1
00528588: beq      #0x5285a4
0052858c: str      r5, [r2]
00528590: ldr      r2, [r0, #0x10]
00528594: add      r2, r2, #4
00528598: str      r2, [r0, #0x10]
0052859c: add      sp, sp, #0x7c
005285a0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005285a4: add      r1, sp, #0x68
005285a8: bl       #0x5280b0
005285ac: b        #0x52859c

# _ZN7PFWorld12InitObstacleER8PFObjectbff
00528234: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00528238: mov      r7, r0
0052823c: sub      sp, sp, #0x74
00528240: mov      r0, r3
00528244: mov      r4, r1
00528248: mov      r1, #0
0052824c: mov      r5, r3
00528250: mov      r6, r2
00528254: bl       #0x30e4b4
00528258: ldr      r3, [pc, #0x20c]
0052825c: cmp      r0, #0
00528260: ldr      r8, [sp, #0x98]
00528264: add      r3, pc, r3
00528268: bne      #0x528290
0052826c: ldr      r2, [pc, #0x1fc]
00528270: ldr      r2, [r3, r2]
00528274: ldr      r2, [r2]
00528278: cmp      r2, #2
0052827c: moveq    r3, #0
00528280: streq    r3, [r3]
00528284: beq      #0x528290
00528288: cmp      r2, #1
0052828c: beq      #0x5283ec
00528290: cmp      r6, #0
00528294: beq      #0x5282ac
00528298: mov      r0, r8
0052829c: mov      r1, #0
005282a0: bl       #0x30df8c
005282a4: cmp      r0, #0
005282a8: beq      #0x5282d4
005282ac: ldr      r3, [r4, #4]
005282b0: tst      r3, #4
005282b4: bne      #0x52833c
005282b8: mov      r2, #0
005282bc: bic      r3, r3, #4
005282c0: str      r3, [r4, #4]
005282c4: str      r2, [r4, #0x34]
005282c8: str      r2, [r4, #0x30]
005282cc: add      sp, sp, #0x74
005282d0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005282d4: ldr      r3, [r4, #4]
005282d8: tst      r3, #4
005282dc: bne      #0x528328
005282e0: ldr      r2, [r4, #0x10]
005282e4: cmp      r2, #0
005282e8: addne    r6, r4, #0x10
005282ec: beq      #0x528420
005282f0: mov      r1, r6
005282f4: add      r0, r7, #0x2c
005282f8: bl       #0x527560
005282fc: str      r4, [sp, #0x64]
00528300: ldr      r1, [r0, #0x18]
00528304: ldr      r2, [r0, #0x10]
00528308: sub      r1, r1, #4
0052830c: cmp      r2, r1
00528310: beq      #0x52845c
00528314: str      r4, [r2]
00528318: ldr      r2, [r0, #0x10]
0052831c: add      r2, r2, #4
00528320: str      r2, [r0, #0x10]
00528324: ldr      r3, [r4, #4]
00528328: orr      r3, r3, #4
0052832c: str      r3, [r4, #4]
00528330: str      r5, [r4, #0x30]
00528334: str      r8, [r4, #0x34]
00528338: b        #0x5282cc
0052833c: add      r0, r7, #0x2c
00528340: add      r1, r4, #0x10
00528344: bl       #0x527560
00528348: ldr      ip, [r0, #0x1c]
0052834c: ldmib    r0, {r6, r7, fp}
00528350: ldr      lr, [r0]
00528354: ldr      r8, [r0, #0x18]
00528358: ldr      sl, [r0, #0x14]
0052835c: ldr      sb, [r0, #0x10]
00528360: mov      r5, r0
00528364: add      r3, sp, #0x60
00528368: str      ip, [sp, #0x4c]
0052836c: add      r0, sp, #0x50
00528370: add      ip, sp, #0x6c
00528374: add      r1, sp, #0x30
00528378: add      r2, sp, #0x40
0052837c: str      ip, [sp]
00528380: str      fp, [sp, #0x3c]
00528384: str      r7, [sp, #0x38]
00528388: str      r6, [sp, #0x34]
0052838c: str      lr, [sp, #0x30]
00528390: str      r8, [sp, #0x48]
00528394: str      sl, [sp, #0x44]
00528398: str      sb, [sp, #0x40]
0052839c: str      r4, [sp, #0x60]
005283a0: bl       #0x526044
005283a4: ldr      ip, [sp, #0x50]
005283a8: ldr      r3, [r5, #0x10]
005283ac: cmp      r3, ip
005283b0: beq      #0x5283e4
005283b4: ldr      lr, [sp, #0x5c]
005283b8: mov      r1, r5
005283bc: add      r0, sp, #0x20
005283c0: str      lr, [sp, #0x1c]
005283c4: ldr      lr, [sp, #0x58]
005283c8: add      r2, sp, #0x10
005283cc: add      r3, sp, #0x68
005283d0: str      lr, [sp, #0x18]
005283d4: ldr      lr, [sp, #0x54]
005283d8: str      ip, [sp, #0x10]
005283dc: str      lr, [sp, #0x14]
005283e0: bl       #0x52689c
005283e4: ldr      r3, [r4, #4]
005283e8: b        #0x5282b8
005283ec: ldr      r0, [pc, #0x80]
005283f0: ldr      r1, [pc, #0x80]
005283f4: ldr      r2, [pc, #0x80]
005283f8: ldr      r0, [r3, r0]
005283fc: ldr      r3, [pc, #0x7c]
00528400: mov      ip, #0x9c
00528404: add      r1, pc, r1
00528408: add      r2, pc, r2
0052840c: add      r3, pc, r3
00528410: add      r0, r0, #0xa8
00528414: str      ip, [sp]
00528418: bl       #0x30e004
0052841c: b        #0x528290
00528420: tst      r3, #1
00528424: add      ip, r4, #0xc
00528428: addeq    r2, r4, #0x20
0052842c: add      r3, r4, #0x24
00528430: str      ip, [sp]
00528434: add      r1, r4, #0x18
00528438: mov      ip, #0
0052843c: add      r6, r4, #0x10
00528440: mov      r0, r7
00528444: stmib    sp, {r6, ip}
00528448: bl       #0x525508
0052844c: ldr      r3, [r4, #0x10]
00528450: cmp      r3, #0
00528454: beq      #0x5282cc
00528458: b        #0x5282f0
0052845c: add      r1, sp, #0x64
00528460: bl       #0x5280b0
00528464: ldr      r3, [r4, #4]
00528468: b        #0x528328
0052846c: subeq    ip, r6, ip, lsr #16
00528470: andeq    r3, r0, r0, asr #19
00528474: andeq    r1, r0, r0, asr #19
00528478: ldrsbteq r5, [sb], -r4
0052847c: eorseq   r4, fp, r0, lsl r7
00528480: ldrhteq  r4, [fp], -ip

# _ZNK8PFObject8IsFlyingEv
005241e8: ldr      r0, [r0, #0x14]
005241ec: and      r0, r0, #1
005241f0: bx       lr

# _ZN8PFObjectC1Ev
00524644: ldr      r3, [pc, #0xf4]
00524648: ldr      r1, [pc, #0xf4]
0052464c: push     {r4, r5, r6, r7, r8, lr}
00524650: add      r3, pc, r3
00524654: ldr      lr, [r3, r1]
00524658: mov      r1, #8
0052465c: str      r1, [r0, #4]
00524660: mov      r2, #0
00524664: mov      ip, #0
00524668: mov      r5, #0x3f800000
0052466c: mov      r1, #2
00524670: str      ip, [r0]
00524674: str      ip, [r0, #0xc]
00524678: str      ip, [r0, #0x10]
0052467c: str      r2, [r0, #0x18]
00524680: str      r2, [r0, #0x1c]
00524684: str      r2, [r0, #0x20]
00524688: str      r1, [r0, #0x14]
0052468c: str      r5, [r0, #8]
00524690: ldr      r6, [lr]
00524694: mov      r4, r0
00524698: ldr      r0, [pc, #0xa8]
0052469c: str      r6, [r4, #0x24]
005246a0: ldr      r6, [lr, #4]
005246a4: ldr      r0, [r3, r0]
005246a8: ldr      r1, [pc, #0x9c]
005246ac: str      r6, [r4, #0x28]
005246b0: ldr      r7, [lr, #8]
005246b4: add      r6, r4, #0x38
005246b8: add      lr, r4, #0x8c
005246bc: add      r0, r0, #8
005246c0: add      r1, pc, r1
005246c4: str      r0, [r4, #0x4c]
005246c8: str      r2, [r4, #0x34]
005246cc: str      r2, [r4, #0x40]
005246d0: str      r2, [r4, #0x44]
005246d4: str      r2, [r4, #0x48]
005246d8: str      ip, [r4, #0x50]
005246dc: str      ip, [r4, #0x54]
005246e0: str      r2, [r4, #0x5c]
005246e4: str      r2, [r4, #0x60]
005246e8: str      r2, [r4, #0x64]
005246ec: str      r2, [r4, #0x68]
005246f0: mov      r0, lr
005246f4: str      r7, [r4, #0x2c]
005246f8: str      r6, [r4, #0x3c]
005246fc: str      r5, [r4, #0x58]
00524700: str      r5, [r4, #0x30]
00524704: str      r6, [r4, #0x38]
00524708: add      r1, r1, #1
0052470c: str      r2, [r4, #0x6c]
00524710: str      r2, [r4, #0x70]
00524714: str      ip, [r4, #0x7c]
00524718: str      r2, [r4, #0x88]
0052471c: str      r2, [r4, #0x74]
00524720: str      r2, [r4, #0x78]
00524724: str      r2, [r4, #0x80]
00524728: str      r2, [r4, #0x84]
0052472c: str      lr, [r4, #0x9c]
00524730: str      lr, [r4, #0xa0]
00524734: bl       #0x5244e8
00524738: mov      r0, r4
0052473c: pop      {r4, r5, r6, r7, r8, pc}
00524740: subeq    r0, r7, r0, asr #8
00524744: andeq    r4, r0, r0, asr #6
00524748: andeq    r1, r0, r8, ror r2
0052474c: eorseq   lr, ip, r0, lsr #6

# _ZNK8PFObject10IsSwimmingEv
0052420c: ldr      r0, [r0, #0x14]
00524210: ubfx     r0, r0, #1, #1
00524214: bx       lr

# _ZN7PFWorldC1Ev
00522e2c: ldr      r2, [pc, #0xc4]
00522e30: ldr      r1, [pc, #0xc4]
00522e34: push     {r4, r5, r6, lr}
00522e38: add      r2, pc, r2
00522e3c: ldr      r1, [r2, r1]
00522e40: mov      r4, r0
00522e44: mov      r5, #0
00522e48: mov      r3, #0
00522e4c: add      r1, r1, #8
00522e50: str      r3, [r4, #0x28]
00522e54: str      r3, [r4, #0x14]
00522e58: str      r3, [r4, #0x18]
00522e5c: str      r3, [r4, #0x1c]
00522e60: str      r3, [r4, #0x20]
00522e64: str      r3, [r4, #0x24]
00522e68: stm      r4, {r1, r5}
00522e6c: str      r5, [r4, #8]
00522e70: str      r5, [r4, #0xc]
00522e74: str      r5, [r4, #0x10]
00522e78: str      r5, [r4, #0x30]
00522e7c: strb     r5, [r0, #0x2c]!
00522e80: str      r0, [r4, #0x38]
00522e84: str      r0, [r4, #0x34]
00522e88: str      r5, [r4, #0x3c]
00522e8c: add      r0, r4, #0x4c
00522e90: str      r5, [r4, #0x44]
00522e94: str      r5, [r4, #0x48]
00522e98: str      r5, [r4, #0x4c]
00522e9c: str      r5, [r4, #0x50]
00522ea0: str      r5, [r4, #0x54]
00522ea4: str      r5, [r4, #0x58]
00522ea8: str      r5, [r4, #0x5c]
00522eac: str      r5, [r4, #0x60]
00522eb0: str      r5, [r4, #0x64]
00522eb4: str      r5, [r4, #0x68]
00522eb8: str      r5, [r4, #0x6c]
00522ebc: str      r5, [r4, #0x70]
00522ec0: bl       #0x522cd4
00522ec4: mov      r3, #0x42000000
00522ec8: add      r3, r3, #0xc80000
00522ecc: strb     r5, [r4, #0x94]
00522ed0: str      r5, [r4, #0x74]
00522ed4: str      r3, [r4, #0x90]
00522ed8: str      r5, [r4, #0x78]
00522edc: str      r5, [r4, #0x7c]
00522ee0: str      r5, [r4, #0x80]
00522ee4: str      r5, [r4, #0x84]
00522ee8: str      r5, [r4, #0x88]
00522eec: str      r5, [r4, #0x8c]
00522ef0: mov      r0, r4
00522ef4: pop      {r4, r5, r6, pc}
00522ef8: subeq    r1, r7, r8, asr ip
00522efc: andeq    r2, r0, r4, ror #12
