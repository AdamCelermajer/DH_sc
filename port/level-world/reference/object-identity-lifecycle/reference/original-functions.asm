
# _ZNSaINSt4priv10_List_nodeIP10ObjectBaseEEE8allocateEjPKv.clone.20
00343168: str      lr, [sp, #-4]!
0034316c: sub      sp, sp, #0xc
00343170: add      r0, sp, #8
00343174: mov      r3, #0xc
00343178: str      r3, [r0, #-4]!
0034317c: bl       #0x708ec0
00343180: add      sp, sp, #0xc
00343184: ldm      sp!, {pc}

# _ZN10ObjectBase9GetHandleEv
0033dd2c: ldr      r3, [pc, #0x34]
0033dd30: ldr      r2, [pc, #0x34]
0033dd34: push     {r4, lr}
0033dd38: add      r3, pc, r3
0033dd3c: ldr      r2, [r3, r2]
0033dd40: ldr      ip, [r1, #0x2c]
0033dd44: mov      r4, r0
0033dd48: ldr      lr, [r2, #0x38]
0033dd4c: mov      r2, #0xc
0033dd50: ldr      r3, [lr, #0x78]
0033dd54: str      r3, [ip, #8]
0033dd58: ldr      r1, [r1, #0x2c]
0033dd5c: bl       #0x30df38
0033dd60: mov      r0, r4
0033dd64: pop      {r4, pc}
0033dd68: rsbeq    r6, r5, r8, asr sp
0033dd6c: strdeq   r3, r4, [r0], -r4

# _ZN10ObjectBase11setUpdatingEb
0033dcf0: strb     r1, [r0, #0x85]
0033dcf4: bx       lr

# _ZN10ObjectBaseD0Ev
0033e97c: push     {r4, lr}
0033e980: mov      r4, r0
0033e984: bl       #0x33e840
0033e988: mov      r0, r4
0033e98c: bl       #0x310440
0033e990: mov      r0, r4
0033e994: pop      {r4, pc}

# _ZN10GameObject8_GetNameERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
0038ebf0: mov      r0, r1
0038ebf4: ldr      r1, [r2, #0x44]
0038ebf8: b        #0x37c8cc

# _ZN12ObjectHandlecvP10GameObjectEv
0033fee4: push     {r4, lr}
0033fee8: mov      r1, #0
0033feec: bl       #0x33fdc0
0033fef0: subs     r4, r0, #0
0033fef4: bne      #0x33ff00
0033fef8: mov      r0, #0
0033fefc: pop      {r4, pc}
0033ff00: ldr      r3, [r4]
0033ff04: mov      lr, pc
0033ff08: ldr      pc, [r3, #0x20]
0033ff0c: cmp      r0, #0
0033ff10: beq      #0x33fef8
0033ff14: mov      r0, r4
0033ff18: pop      {r4, pc}

# _ZN13ObjectManager10FakeRemoveE12ObjectHandle
00349240: push     {r4, r5, r6, r7, r8, lr}
00349244: sub      sp, sp, #0x18
00349248: add      r4, sp, #4
0034924c: mov      r5, r0
00349250: mov      r0, r4
00349254: stm      r4, {r1, r2, r3}
00349258: bl       #0x33fee4
0034925c: mov      r6, r0
00349260: ldr      r0, [r0, #0x2f4]
00349264: mov      r3, #1
00349268: strb     r3, [r6, #0x81]
0034926c: cmp      r0, #0
00349270: beq      #0x34927c
00349274: mov      r1, r6
00349278: bl       #0x3968cc
0034927c: mov      r0, r5
00349280: mov      r1, r6
00349284: bl       #0x3462b8
00349288: mov      r2, r5
0034928c: ldr      r0, [r2, #0x90]!
00349290: cmp      r0, r2
00349294: beq      #0x3492b4
00349298: ldr      r3, [r0, #8]
0034929c: cmp      r6, r3
003492a0: beq      #0x3492b4
003492a4: ldr      r0, [r0]
003492a8: cmp      r2, r0
003492ac: bne      #0x349298
003492b0: mov      r0, r2
003492b4: cmp      r2, r0
003492b8: beq      #0x3492d4
003492bc: ldr      r3, [r0]
003492c0: ldr      r2, [r0, #4]
003492c4: mov      r1, #0xc
003492c8: str      r3, [r2]
003492cc: str      r2, [r3, #4]
003492d0: bl       #0x708f00
003492d4: mov      r0, r4
003492d8: mov      r1, #0
003492dc: bl       #0x33fdc0
003492e0: mov      r7, r5
003492e4: mov      r8, r0
003492e8: ldr      r0, [r7, #0x2c]!
003492ec: cmp      r7, r0
003492f0: beq      #0x349310
003492f4: ldr      r3, [r0, #8]
003492f8: ldr      r6, [r0]
003492fc: cmp      r8, r3
00349300: beq      #0x349468
00349304: mov      r0, r6
00349308: cmp      r7, r0
0034930c: bne      #0x3492f4
00349310: mov      r0, r4
00349314: bl       #0x33ff54
00349318: mov      r7, r5
0034931c: mov      r8, r0
00349320: ldr      r0, [r7, #0x70]!
00349324: cmp      r7, r0
00349328: beq      #0x349348
0034932c: ldr      r3, [r0, #8]
00349330: ldr      r6, [r0]
00349334: cmp      r8, r3
00349338: beq      #0x349484
0034933c: mov      r0, r6
00349340: cmp      r7, r0
00349344: bne      #0x34932c
00349348: mov      r0, r4
0034934c: bl       #0x33ff54
00349350: subs     r8, r0, #0
00349354: beq      #0x349394
00349358: mov      r7, r5
0034935c: ldr      r0, [r7, #0x60]!
00349360: cmp      r7, r0
00349364: beq      #0x349384
00349368: ldr      r3, [r0, #8]
0034936c: ldr      r6, [r0]
00349370: cmp      r8, r3
00349374: beq      #0x3494a0
00349378: mov      r0, r6
0034937c: cmp      r7, r0
00349380: bne      #0x349368
00349384: add      r0, r8, #0x3c8
00349388: bl       #0x3d2ff8
0034938c: mov      r0, r8
00349390: bl       #0x3a66f8
00349394: mov      r0, r4
00349398: mov      r1, #0
0034939c: bl       #0x33fdc0
003493a0: subs     r7, r0, #0
003493a4: beq      #0x3493b4
003493a8: ldr      r3, [r7, #0xf4]
003493ac: cmp      r3, #5
003493b0: beq      #0x3494ec
003493b4: mov      r0, r4
003493b8: mov      r1, #0
003493bc: bl       #0x33fdc0
003493c0: mov      r7, r5
003493c4: mov      r8, r0
003493c8: ldr      r0, [r7, #0x100]!
003493cc: cmp      r7, r0
003493d0: beq      #0x3493f0
003493d4: ldr      r3, [r0, #8]
003493d8: ldr      r6, [r0]
003493dc: cmp      r8, r3
003493e0: beq      #0x3494bc
003493e4: mov      r0, r6
003493e8: cmp      r7, r0
003493ec: bne      #0x3493d4
003493f0: ldr      r3, [r5, #0x10]
003493f4: ldr      r0, [sp, #4]
003493f8: cmp      r3, #0
003493fc: addeq    r6, r5, #0xc
00349400: beq      #0x349448
00349404: add      r6, r5, #0xc
00349408: mov      r1, r6
0034940c: b        #0x349414
00349410: mov      r3, r2
00349414: ldr      r2, [r3, #0x10]
00349418: cmp      r0, r2
0034941c: ldrgt    r2, [r3, #0xc]
00349420: ldrle    r2, [r3, #8]
00349424: movgt    r3, r1
00349428: mov      r1, r3
0034942c: cmp      r2, #0
00349430: bne      #0x349410
00349434: cmp      r6, r3
00349438: beq      #0x349448
0034943c: ldr      r2, [r3, #0x10]
00349440: cmp      r0, r2
00349444: bge      #0x3494d8
00349448: mov      r1, r4
0034944c: mov      r0, r6
00349450: bl       #0x33fc88
00349454: ldr      r1, [r0, #0x18]
00349458: mov      r0, r5
0034945c: bl       #0x343188
00349460: add      sp, sp, #0x18
00349464: pop      {r4, r5, r6, r7, r8, pc}
00349468: ldr      r3, [r0, #4]
0034946c: mov      r1, #0xc
00349470: str      r6, [r3]
00349474: str      r3, [r6, #4]
00349478: bl       #0x708f00
0034947c: mov      r0, r6
00349480: b        #0x349308
00349484: ldr      r3, [r0, #4]
00349488: mov      r1, #0xc
0034948c: str      r6, [r3]
00349490: str      r3, [r6, #4]
00349494: bl       #0x708f00
00349498: mov      r0, r6
0034949c: b        #0x349340
003494a0: ldr      r3, [r0, #4]
003494a4: mov      r1, #0xc
003494a8: str      r6, [r3]
003494ac: str      r3, [r6, #4]
003494b0: bl       #0x708f00
003494b4: mov      r0, r6
003494b8: b        #0x34937c
003494bc: ldr      r3, [r0, #4]
003494c0: mov      r1, #0xc
003494c4: str      r6, [r3]
003494c8: str      r3, [r6, #4]
003494cc: bl       #0x708f00
003494d0: mov      r0, r6
003494d4: b        #0x3493e8
003494d8: add      r1, sp, #0x18
003494dc: str      r3, [r1, #-4]!
003494e0: mov      r0, r6
003494e4: bl       #0x347f58
003494e8: b        #0x349448
003494ec: mov      r8, r5
003494f0: ldr      r0, [r8, #0x68]!
003494f4: cmp      r8, r0
003494f8: beq      #0x3493b4
003494fc: ldr      r3, [r0, #8]
00349500: ldr      r6, [r0]
00349504: cmp      r7, r3
00349508: movne    r0, r6
0034950c: bne      #0x3494f4
00349510: ldr      r3, [r0, #4]
00349514: mov      r1, #0xc
00349518: str      r6, [r3]
0034951c: str      r3, [r6, #4]
00349520: bl       #0x708f00
00349524: mov      r0, r6
00349528: b        #0x3494f4

# _ZN12ObjectHandlecvP9CharacterEv
0033ff54: push     {r4, lr}
0033ff58: mov      r1, #0
0033ff5c: bl       #0x33fdc0
0033ff60: subs     r4, r0, #0
0033ff64: bne      #0x33ff70
0033ff68: mov      r0, #0
0033ff6c: pop      {r4, pc}
0033ff70: ldr      r3, [r4]
0033ff74: mov      lr, pc
0033ff78: ldr      pc, [r3, #0x24]
0033ff7c: cmp      r0, #0
0033ff80: beq      #0x33ff68
0033ff84: mov      r0, r4
0033ff88: pop      {r4, pc}

# _ZN13ObjectManager15MarkForDeletionEP10ObjectBase
003432f8: push     {r4, r5, lr}
003432fc: mov      r4, r0
00343300: ldr      r3, [r4, #0x3c]!
00343304: sub      sp, sp, #0xc
00343308: mov      r5, r0
0034330c: cmp      r3, r4
00343310: beq      #0x343330
00343314: ldr      r2, [r3, #8]
00343318: cmp      r2, r1
0034331c: beq      #0x343330
00343320: ldr      r3, [r3]
00343324: cmp      r4, r3
00343328: bne      #0x343314
0034332c: mov      r3, r4
00343330: cmp      r4, r3
00343334: beq      #0x343340
00343338: add      sp, sp, #0xc
0034333c: pop      {r4, r5, pc}
00343340: mov      r0, r4
00343344: str      r1, [sp, #4]
00343348: bl       #0x343168
0034334c: ldr      r1, [sp, #4]
00343350: str      r1, [r0, #8]
00343354: ldr      r3, [r5, #0x40]
00343358: str      r4, [r0]
0034335c: str      r3, [r0, #4]
00343360: str      r0, [r3]
00343364: str      r0, [r5, #0x40]
00343368: b        #0x343338

# _ZN10ObjectBase7SetNameEPKc
0034ac18: push     {r4, r5, r6, lr}
0034ac1c: mov      r6, r0
0034ac20: mov      r0, r1
0034ac24: mov      r4, r1
0034ac28: bl       #0x30de54
0034ac2c: ldr      r5, [pc, #0x60]
0034ac30: add      r2, r4, r0
0034ac34: mov      r1, r4
0034ac38: add      r0, r6, #0x30
0034ac3c: bl       #0x3109e0
0034ac40: ldr      r3, [pc, #0x50]
0034ac44: add      r5, pc, r5
0034ac48: ldr      r1, [r6, #0x2c]
0034ac4c: ldr      r3, [r5, r3]
0034ac50: ldr      r0, [r3, #0x38]
0034ac54: add      r0, r0, #0xc
0034ac58: bl       #0x33fc88
0034ac5c: cmp      r4, #0
0034ac60: mov      r5, r0
0034ac64: beq      #0x34ac84
0034ac68: mov      r0, r4
0034ac6c: bl       #0x30de54
0034ac70: add      r2, r4, r0
0034ac74: mov      r0, r5
0034ac78: mov      r1, r4
0034ac7c: pop      {r4, r5, r6, lr}
0034ac80: b        #0x3109e0
0034ac84: ldr      r2, [pc, #0x10]
0034ac88: add      r2, pc, r2
0034ac8c: mov      r4, r2
0034ac90: b        #0x34ac74
0034ac94: rsbeq    sb, r4, ip, asr #28
0034ac98: strdeq   r3, r4, [r0], -r4
0034ac9c: subseq   r0, r8, r0, lsl #23

# _ZN13ObjectManager6RemoveE12ObjectHandle
00348ea4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00348ea8: sub      sp, sp, #0x20
00348eac: add      r4, sp, #4
00348eb0: mov      r5, r0
00348eb4: mov      r0, r4
00348eb8: stm      r4, {r1, r2, r3}
00348ebc: bl       #0x33fee4
00348ec0: subs     r7, r0, #0
00348ec4: beq      #0x348f34
00348ec8: ldr      r0, [r7, #0x2f4]
00348ecc: cmp      r0, #0
00348ed0: beq      #0x348edc
00348ed4: mov      r1, r7
00348ed8: bl       #0x3968cc
00348edc: mov      r0, r5
00348ee0: mov      r1, r7
00348ee4: bl       #0x3462b8
00348ee8: mov      r2, r5
00348eec: ldr      r0, [r2, #0x90]!
00348ef0: cmp      r0, r2
00348ef4: beq      #0x348f14
00348ef8: ldr      r3, [r0, #8]
00348efc: cmp      r7, r3
00348f00: beq      #0x348f14
00348f04: ldr      r0, [r0]
00348f08: cmp      r2, r0
00348f0c: bne      #0x348ef8
00348f10: mov      r0, r2
00348f14: cmp      r2, r0
00348f18: beq      #0x348f34
00348f1c: ldr      r3, [r0]
00348f20: ldr      r2, [r0, #4]
00348f24: mov      r1, #0xc
00348f28: str      r3, [r2]
00348f2c: str      r2, [r3, #4]
00348f30: bl       #0x708f00
00348f34: mov      r0, r4
00348f38: mov      r1, #0
00348f3c: bl       #0x33fdc0
00348f40: mov      r8, r5
00348f44: mov      sl, r0
00348f48: ldr      r0, [r8, #0x2c]!
00348f4c: cmp      r8, r0
00348f50: beq      #0x348f70
00348f54: ldr      r3, [r0, #8]
00348f58: ldr      r6, [r0]
00348f5c: cmp      sl, r3
00348f60: beq      #0x3490ec
00348f64: mov      r0, r6
00348f68: cmp      r8, r0
00348f6c: bne      #0x348f54
00348f70: mov      r0, r4
00348f74: bl       #0x33ff54
00348f78: mov      r8, r5
00348f7c: mov      sl, r0
00348f80: ldr      r0, [r8, #0x70]!
00348f84: cmp      r8, r0
00348f88: beq      #0x348fa8
00348f8c: ldr      r3, [r0, #8]
00348f90: ldr      r6, [r0]
00348f94: cmp      sl, r3
00348f98: beq      #0x349108
00348f9c: mov      r0, r6
00348fa0: cmp      r8, r0
00348fa4: bne      #0x348f8c
00348fa8: mov      r0, r4
00348fac: bl       #0x33ff54
00348fb0: subs     sl, r0, #0
00348fb4: beq      #0x348fec
00348fb8: mov      r8, r5
00348fbc: ldr      r0, [r8, #0x60]!
00348fc0: cmp      r8, r0
00348fc4: beq      #0x348fe4
00348fc8: ldr      r3, [r0, #8]
00348fcc: ldr      r6, [r0]
00348fd0: cmp      sl, r3
00348fd4: beq      #0x349124
00348fd8: mov      r0, r6
00348fdc: cmp      r8, r0
00348fe0: bne      #0x348fc8
00348fe4: add      r0, sl, #0x3c8
00348fe8: bl       #0x3d2ff8
00348fec: mov      r0, r4
00348ff0: mov      r1, #0
00348ff4: bl       #0x33fdc0
00348ff8: subs     r8, r0, #0
00348ffc: beq      #0x34900c
00349000: ldr      r3, [r8, #0xf4]
00349004: cmp      r3, #5
00349008: beq      #0x349200
0034900c: bl       #0x7fd794
00349010: ldrb     r3, [r0, #5]
00349014: cmp      r3, #0
00349018: addeq    sl, r5, #0xc
0034901c: beq      #0x349058
00349020: mov      sb, r5
00349024: ldr      r6, [sb, #0x100]!
00349028: add      sl, r5, #0xc
0034902c: b        #0x349048
00349030: ldr      r8, [r6, #8]
00349034: bl       #0x33fc88
00349038: ldr      r3, [r0, #0x18]
0034903c: cmp      r8, r3
00349040: beq      #0x349180
00349044: ldr      r6, [r6]
00349048: cmp      r6, sb
0034904c: mov      r0, sl
00349050: mov      r1, r4
00349054: bne      #0x349030
00349058: ldr      r3, [r5, #0x50]
0034905c: sub      r3, r3, #1
00349060: str      r3, [r5, #0x50]
00349064: ldrb     r3, [r7, #0x2fc]
00349068: cmp      r3, #0
0034906c: beq      #0x349140
00349070: mov      r1, r4
00349074: mov      r0, sl
00349078: bl       #0x33fc88
0034907c: ldr      r1, [r0, #0x18]
00349080: mov      r0, r5
00349084: bl       #0x343188
00349088: ldr      r3, [r5, #0x10]
0034908c: ldr      r0, [sp, #4]
00349090: cmp      r3, #0
00349094: beq      #0x3490d8
00349098: mov      r1, sl
0034909c: b        #0x3490a4
003490a0: mov      r3, r2
003490a4: ldr      r2, [r3, #0x10]
003490a8: cmp      r0, r2
003490ac: ldrgt    r2, [r3, #0xc]
003490b0: ldrle    r2, [r3, #8]
003490b4: movgt    r3, r1
003490b8: mov      r1, r3
003490bc: cmp      r2, #0
003490c0: bne      #0x3490a0
003490c4: cmp      sl, r3
003490c8: beq      #0x3490d8
003490cc: ldr      r2, [r3, #0x10]
003490d0: cmp      r0, r2
003490d4: bge      #0x34916c
003490d8: ldr      r3, [r5, #0x78]
003490dc: add      r3, r3, #1
003490e0: str      r3, [r5, #0x78]
003490e4: add      sp, sp, #0x20
003490e8: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003490ec: ldr      r3, [r0, #4]
003490f0: mov      r1, #0xc
003490f4: str      r6, [r3]
003490f8: str      r3, [r6, #4]
003490fc: bl       #0x708f00
00349100: mov      r0, r6
00349104: b        #0x348f68
00349108: ldr      r3, [r0, #4]
0034910c: mov      r1, #0xc
00349110: str      r6, [r3]
00349114: str      r3, [r6, #4]
00349118: bl       #0x708f00
0034911c: mov      r0, r6
00349120: b        #0x348fa0
00349124: ldr      r3, [r0, #4]
00349128: mov      r1, #0xc
0034912c: str      r6, [r3]
00349130: str      r3, [r6, #4]
00349134: bl       #0x708f00
00349138: mov      r0, r6
0034913c: b        #0x348fdc
00349140: mov      r1, r4
00349144: mov      r0, sl
00349148: bl       #0x33fc88
0034914c: ldr      r3, [r0, #0x18]
00349150: cmp      r3, #0
00349154: beq      #0x349088
00349158: mov      r0, r3
0034915c: ldr      r3, [r3]
00349160: mov      lr, pc
00349164: ldr      pc, [r3, #4]
00349168: b        #0x349088
0034916c: add      r1, sp, #0x20
00349170: str      r3, [r1, #-4]!
00349174: mov      r0, sl
00349178: bl       #0x347f58
0034917c: b        #0x3490d8
00349180: add      sb, sp, #0x10
00349184: mov      r1, r8
00349188: mov      r0, sb
0034918c: bl       #0x33dd2c
00349190: mov      r0, sb
00349194: bl       #0x33ff54
00349198: subs     r3, r0, #0
0034919c: beq      #0x3491b4
003491a0: ldr      r3, [r3]
003491a4: mov      lr, pc
003491a8: ldr      pc, [r3, #0x28]
003491ac: cmp      r0, #0
003491b0: bne      #0x3491e0
003491b4: ldr      sb, [r8, #0x108]
003491b8: add      r8, r5, #0x120
003491bc: mov      r0, r8
003491c0: bl       #0x343148
003491c4: uxth     sb, sb
003491c8: strh     sb, [r0, #8]
003491cc: ldr      r3, [r5, #0x124]
003491d0: str      r8, [r0]
003491d4: str      r3, [r0, #4]
003491d8: str      r0, [r3]
003491dc: str      r0, [r5, #0x124]
003491e0: ldr      r3, [r6]
003491e4: ldr      r2, [r6, #4]
003491e8: mov      r0, r6
003491ec: mov      r1, #0xc
003491f0: str      r3, [r2]
003491f4: str      r2, [r3, #4]
003491f8: bl       #0x708f00
003491fc: b        #0x349058
00349200: mov      sl, r5
00349204: ldr      r0, [sl, #0x68]!
00349208: cmp      sl, r0
0034920c: beq      #0x34900c
00349210: ldr      r3, [r0, #8]
00349214: ldr      r6, [r0]
00349218: cmp      r8, r3
0034921c: movne    r0, r6
00349220: bne      #0x349208
00349224: ldr      r3, [r0, #4]
00349228: mov      r1, #0xc
0034922c: str      r6, [r3]
00349230: str      r3, [r6, #4]
00349234: bl       #0x708f00
00349238: mov      r0, r6
0034923c: b        #0x349208

# _ZN13ObjectManager14GetObjectByPtrEP10ObjectBase
00340c54: push     {r4, r5, r6, lr}
00340c58: mov      r6, r2
00340c5c: sub      sp, sp, #0x10
00340c60: mov      r4, r0
00340c64: bl       #0x33f50c
00340c68: cmp      r6, #0
00340c6c: beq      #0x340c9c
00340c70: mov      r1, r6
00340c74: mov      r0, sp
00340c78: bl       #0x33dd2c
00340c7c: ldr      r0, [sp]
00340c80: ldr      r1, [sp, #8]
00340c84: ldr      r2, [sp, #4]
00340c88: mov      r3, r4
00340c8c: str      r0, [r3], #4
00340c90: mov      r5, sp
00340c94: str      r1, [r3, #4]
00340c98: str      r2, [r4, #4]
00340c9c: mov      r0, r4
00340ca0: add      sp, sp, #0x10
00340ca4: pop      {r4, r5, r6, pc}

# _ZN13ObjectManager3AddEP10ObjectBasePKcS3_ib
0034b270: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034b274: ldr      r4, [pc, #0x27c]
0034b278: sub      sp, sp, #0x2c
0034b27c: subs     r6, r2, #0
0034b280: add      r4, pc, r4
0034b284: mov      r5, r0
0034b288: mov      r8, r1
0034b28c: mov      r7, r3
0034b290: ldr      fp, [sp, #0x50]
0034b294: ldr      sl, [sp, #0x54]
0034b298: ldrb     sb, [sp, #0x58]
0034b29c: beq      #0x34b304
0034b2a0: cmp      r7, #0
0034b2a4: beq      #0x34b358
0034b2a8: mov      r4, #0
0034b2ac: mov      ip, #1
0034b2b0: mov      r2, r7
0034b2b4: mov      r3, sl
0034b2b8: mov      r0, r5
0034b2bc: mov      r1, r8
0034b2c0: str      ip, [sp]
0034b2c4: str      r4, [sp, #4]
0034b2c8: bl       #0x34aca0
0034b2cc: mov      r0, r5
0034b2d0: mov      r1, r4
0034b2d4: bl       #0x33fdc0
0034b2d8: cmp      r0, r4
0034b2dc: beq      #0x34b3ac
0034b2e0: cmp      r6, r4
0034b2e4: beq      #0x34b2f8
0034b2e8: mov      r0, r6
0034b2ec: ldr      r3, [r6]
0034b2f0: mov      lr, pc
0034b2f4: ldr      pc, [r3, #4]
0034b2f8: mov      r0, r5
0034b2fc: add      sp, sp, #0x2c
0034b300: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034b304: ldr      r3, [pc, #0x1f0]
0034b308: ldr      r3, [r4, r3]
0034b30c: ldr      r3, [r3]
0034b310: cmp      r3, #2
0034b314: streq    r6, [r6]
0034b318: beq      #0x34b2a0
0034b31c: cmp      r3, #1
0034b320: bne      #0x34b2a0
0034b324: ldr      r0, [pc, #0x1d4]
0034b328: ldr      r1, [pc, #0x1d4]
0034b32c: ldr      r2, [pc, #0x1d4]
0034b330: ldr      r0, [r4, r0]
0034b334: ldr      r3, [pc, #0x1d0]
0034b338: movw     ip, #0x428
0034b33c: add      r1, pc, r1
0034b340: add      r2, pc, r2
0034b344: add      r3, pc, r3
0034b348: add      r0, r0, #0xa8
0034b34c: str      ip, [sp]
0034b350: bl       #0x30e004
0034b354: b        #0x34b2a0
0034b358: ldr      r3, [pc, #0x19c]
0034b35c: ldr      r3, [r4, r3]
0034b360: ldr      r3, [r3]
0034b364: cmp      r3, #2
0034b368: streq    r7, [r7]
0034b36c: beq      #0x34b2a8
0034b370: cmp      r3, #1
0034b374: bne      #0x34b2a8
0034b378: ldr      r0, [pc, #0x180]
0034b37c: ldr      r1, [pc, #0x18c]
0034b380: ldr      r2, [pc, #0x18c]
0034b384: ldr      r0, [r4, r0]
0034b388: ldr      r3, [pc, #0x188]
0034b38c: movw     ip, #0x429
0034b390: add      r1, pc, r1
0034b394: add      r2, pc, r2
0034b398: add      r3, pc, r3
0034b39c: add      r0, r0, #0xa8
0034b3a0: str      ip, [sp]
0034b3a4: bl       #0x30e004
0034b3a8: b        #0x34b2a8
0034b3ac: mov      r1, r5
0034b3b0: add      r0, r8, #0xc
0034b3b4: bl       #0x33fc88
0034b3b8: str      r6, [r0, #0x18]
0034b3bc: ldr      r3, [r8, #0x50]
0034b3c0: mov      r2, r5
0034b3c4: mov      r1, r7
0034b3c8: add      r3, r3, #1
0034b3cc: str      r3, [r8, #0x50]
0034b3d0: str      r6, [r5, #4]
0034b3d4: ldr      ip, [r6, #0x2c]
0034b3d8: ldr      lr, [r2], #4
0034b3dc: mov      r0, r6
0034b3e0: mov      r3, ip
0034b3e4: str      lr, [r3], #4
0034b3e8: ldr      lr, [r5, #4]
0034b3ec: add      r4, sp, #0x18
0034b3f0: str      lr, [ip, #4]
0034b3f4: ldr      r2, [r2, #4]
0034b3f8: str      r2, [r3, #4]
0034b3fc: bl       #0x34ac18
0034b400: mov      r0, fp
0034b404: bl       #0x30de54
0034b408: mov      r1, fp
0034b40c: add      r2, fp, r0
0034b410: add      r0, r6, #0x48
0034b414: bl       #0x3109e0
0034b418: mov      r1, r6
0034b41c: mov      r0, r4
0034b420: str      sl, [r6, #0x64]
0034b424: bl       #0x33dd2c
0034b428: mov      r0, r4
0034b42c: bl       #0x33ff54
0034b430: subs     r7, r0, #0
0034b434: beq      #0x34b45c
0034b438: add      r4, r8, #0x60
0034b43c: mov      r0, r4
0034b440: bl       #0x342690
0034b444: str      r7, [r0, #8]
0034b448: ldr      r3, [r8, #0x64]
0034b44c: str      r4, [r0]
0034b450: str      r3, [r0, #4]
0034b454: str      r0, [r3]
0034b458: str      r0, [r8, #0x64]
0034b45c: add      r4, sp, #0xc
0034b460: mov      r0, r4
0034b464: mov      r1, r6
0034b468: bl       #0x33dd2c
0034b46c: mov      r0, r4
0034b470: mov      r1, #0
0034b474: bl       #0x33fdc0
0034b478: subs     r4, r0, #0
0034b47c: beq      #0x34b48c
0034b480: ldr      r3, [r4, #0xf4]
0034b484: cmp      r3, #5
0034b488: beq      #0x34b4a4
0034b48c: cmp      sb, #0
0034b490: beq      #0x34b2f8
0034b494: mov      r0, r8
0034b498: mov      r1, r6
0034b49c: bl       #0x3431c0
0034b4a0: b        #0x34b2f8
0034b4a4: ldr      r0, [r6, #0x5c]
0034b4a8: ldr      r2, [r6, #0x58]
0034b4ac: rsb      r2, r0, r2
0034b4b0: cmp      r2, #6
0034b4b4: bne      #0x34b48c
0034b4b8: ldr      r1, [pc, #0x5c]
0034b4bc: add      r1, pc, r1
0034b4c0: bl       #0x30e5e0
0034b4c4: cmp      r0, #0
0034b4c8: bne      #0x34b48c
0034b4cc: mov      r3, #0xc
0034b4d0: add      r0, sp, #0x28
0034b4d4: str      r3, [r0, #-4]!
0034b4d8: bl       #0x708ec0
0034b4dc: str      r4, [r0, #8]
0034b4e0: ldr      r3, [r8, #0x6c]
0034b4e4: add      r2, r8, #0x68
0034b4e8: stm      r0, {r2, r3}
0034b4ec: str      r0, [r3]
0034b4f0: str      r0, [r8, #0x6c]
0034b4f4: b        #0x34b48c
0034b4f8: rsbeq    sb, r4, r0, lsl r8
0034b4fc: andeq    r3, r0, r0, asr #19
0034b500: andeq    r1, r0, r0, asr #19

# _ZN10ObjectBaseD1Ev
0033e840: push     {r4, r5, lr}
0033e844: ldr      r5, [pc, #0x104]
0033e848: ldr      r3, [pc, #0x104]
0033e84c: ldrb     r1, [r0, #0x29]
0033e850: add      r5, pc, r5
0033e854: ldr      r3, [r5, r3]
0033e858: cmp      r1, #0
0033e85c: sub      sp, sp, #0xc
0033e860: add      r2, r3, #0x74
0033e864: add      r1, r3, #8
0033e868: add      r3, r3, #0x68
0033e86c: mov      r4, r0
0033e870: stm      r0, {r1, r3}
0033e874: str      r2, [r0, #0x24]
0033e878: beq      #0x33e8a0
0033e87c: ldr      r3, [pc, #0xd4]
0033e880: ldr      r3, [r5, r3]
0033e884: ldr      r3, [r3]
0033e888: cmp      r3, #2
0033e88c: moveq    r3, #0
0033e890: streq    r3, [r3]
0033e894: beq      #0x33e8a0
0033e898: cmp      r3, #1
0033e89c: beq      #0x33e91c
0033e8a0: ldr      r0, [r4, #0x2c]
0033e8a4: cmp      r0, #0
0033e8a8: beq      #0x33e8b8
0033e8ac: bl       #0x310440
0033e8b0: mov      r3, #0
0033e8b4: str      r3, [r4, #0x2c]
0033e8b8: add      r0, r4, #0xd4
0033e8bc: bl       #0x3139ac
0033e8c0: add      r0, r4, #0xb0
0033e8c4: bl       #0x33e7f8
0033e8c8: add      r0, r4, #0x8c
0033e8cc: bl       #0x33e7f8
0033e8d0: add      r0, r4, #0x68
0033e8d4: bl       #0x3139ac
0033e8d8: add      r0, r4, #0x48
0033e8dc: bl       #0x3139ac
0033e8e0: add      r0, r4, #0x30
0033e8e4: bl       #0x3139ac
0033e8e8: ldr      r2, [pc, #0x6c]
0033e8ec: ldr      r3, [pc, #0x6c]
0033e8f0: add      r0, r4, #8
0033e8f4: ldr      r2, [r5, r2]
0033e8f8: ldr      r3, [r5, r3]
0033e8fc: add      r2, r2, #8
0033e900: add      r3, r3, #8
0033e904: str      r2, [r4, #0x24]
0033e908: str      r3, [r4, #4]
0033e90c: bl       #0x3139ac
0033e910: mov      r0, r4
0033e914: add      sp, sp, #0xc
0033e918: pop      {r4, r5, pc}
0033e91c: ldr      r0, [pc, #0x40]
0033e920: ldr      r1, [pc, #0x40]
0033e924: ldr      r2, [pc, #0x40]
0033e928: ldr      r0, [r5, r0]
0033e92c: ldr      r3, [pc, #0x3c]
0033e930: mov      ip, #0x71
0033e934: add      r1, pc, r1
0033e938: add      r2, pc, r2
0033e93c: add      r3, pc, r3
0033e940: add      r0, r0, #0xa8
0033e944: str      ip, [sp]
0033e948: bl       #0x30e004
0033e94c: b        #0x33e8a0
0033e950: rsbeq    r6, r5, r0, asr #4
0033e954: andeq    r3, r0, r4, lsl #23
0033e958: andeq    r3, r0, r0, asr #19
0033e95c: andeq    r1, r0, r0, asr #8
0033e960: ldrdeq   r3, r4, [r0], -ip
0033e964: andeq    r1, r0, r0, asr #19
0033e968: subseq   pc, r7, r4, lsr #21
0033e96c: ldrsbeq  r1, [r8], #-0x70
0033e970: ldrsbeq  r1, [r8], #-0x7c

# _ZN12ObjectHandleC1EP10ObjectBase
0033f524: push     {r4, r5, lr}
0033f528: mov      r3, #0
0033f52c: mvn      r2, #0
0033f530: cmp      r1, #0
0033f534: sub      sp, sp, #0x14
0033f538: mov      r4, r0
0033f53c: str      r3, [r0, #4]
0033f540: str      r2, [r0, #8]
0033f544: str      r3, [r0]
0033f548: beq      #0x33f56c
0033f54c: mov      r0, sp
0033f550: bl       #0x33dd2c
0033f554: ldm      sp, {r0, r1, r2}
0033f558: mov      r3, r4
0033f55c: str      r0, [r3], #4
0033f560: mov      r5, sp
0033f564: str      r1, [r4, #4]
0033f568: str      r2, [r3, #4]
0033f56c: mov      r0, r4
0033f570: add      sp, sp, #0x14
0033f574: pop      {r4, r5, pc}

# _ZN10ObjectBase6DeleteEv
0033ddb4: mov      r3, #2
0033ddb8: strb     r3, [r0, #0x82]
0033ddbc: mov      r3, #1
0033ddc0: strb     r3, [r0, #0x81]
0033ddc4: bx       lr

# _ZN13ObjectManager6UpdateEf
0034a620: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034a624: ldr      r7, [pc, #0x5c4]
0034a628: ldr      sl, [pc, #0x5c4]
0034a62c: mov      r5, r0
0034a630: add      r7, pc, r7
0034a634: ldr      r3, [r7, sl]
0034a638: ldr      r0, [pc, #0x5b8]
0034a63c: sub      sp, sp, #0xb4
0034a640: ldr      r3, [r3]
0034a644: add      r0, pc, r0
0034a648: mov      r4, r1
0034a64c: str      r3, [sp, #0xac]
0034a650: bl       #0x3136b4
0034a654: bl       #0x7fd794
0034a658: ldrb     r3, [r0, #5]
0034a65c: cmp      r3, #0
0034a660: movne    r3, #1
0034a664: strbne   r3, [r5, #0xfd]
0034a668: strbne   r3, [r5, #0xfc]
0034a66c: ldr      r3, [pc, #0x588]
0034a670: ldr      r0, [r7, r3]
0034a674: bl       #0x31f594
0034a678: cmp      r0, #0
0034a67c: beq      #0x34a68c
0034a680: ldrb     r3, [r0, #0x144]
0034a684: cmp      r3, #0
0034a688: bne      #0x34aa8c
0034a68c: mov      r0, r5
0034a690: bl       #0x34163c
0034a694: mov      r1, r4
0034a698: mov      r0, r5
0034a69c: bl       #0x340274
0034a6a0: mov      r4, r5
0034a6a4: mov      r0, r5
0034a6a8: bl       #0x3460cc
0034a6ac: ldr      ip, [r4, #0x34]!
0034a6b0: cmp      ip, r4
0034a6b4: addeq    r8, r5, #0x2c
0034a6b8: beq      #0x34a700
0034a6bc: mov      r3, ip
0034a6c0: ldr      r3, [r3]
0034a6c4: cmp      r4, r3
0034a6c8: bne      #0x34a6c0
0034a6cc: add      r8, r5, #0x2c
0034a6d0: mov      r0, r8
0034a6d4: str      ip, [sp, #0x68]
0034a6d8: add      r1, sp, #0x64
0034a6dc: add      ip, sp, #0x70
0034a6e0: add      r2, sp, #0x68
0034a6e4: add      r3, sp, #0x6c
0034a6e8: str      ip, [sp]
0034a6ec: str      r8, [sp, #0x64]
0034a6f0: str      r4, [sp, #0x6c]
0034a6f4: bl       #0x345428
0034a6f8: mov      r0, r4
0034a6fc: bl       #0x34526c
0034a700: mov      r6, r5
0034a704: ldr      r4, [r6, #0x3c]!
0034a708: cmp      r4, r6
0034a70c: beq      #0x34a760
0034a710: mov      r3, r4
0034a714: ldr      r3, [r3]
0034a718: cmp      r6, r3
0034a71c: bne      #0x34a714
0034a720: add      r2, sp, #0x54
0034a724: cmp      r4, r6
0034a728: add      sb, sp, #0x48
0034a72c: str      r2, [sp, #0xc]
0034a730: beq      #0x34a760
0034a734: ldr      r1, [r4, #8]
0034a738: ldrb     r3, [r1, #0x29]
0034a73c: cmp      r3, #0
0034a740: beq      #0x34ab14
0034a744: ldrb     r3, [r1, #0x82]
0034a748: cmp      r3, #0
0034a74c: bne      #0x34ab20
0034a750: ldr      fp, [r4]
0034a754: mov      r4, fp
0034a758: cmp      r4, r6
0034a75c: bne      #0x34a734
0034a760: mov      sb, r5
0034a764: ldr      r6, [sb, #0x44]!
0034a768: cmp      sb, r6
0034a76c: beq      #0x34a7b0
0034a770: ldr      r4, [r6, #8]
0034a774: ldrb     r3, [r4, #0xac]
0034a778: cmp      r3, #0
0034a77c: bne      #0x34a9c0
0034a780: ldr      r3, [r4, #0xa8]
0034a784: cmp      r3, #0
0034a788: beq      #0x34a9c0
0034a78c: mov      r1, #1
0034a790: mov      r0, r4
0034a794: bl       #0x33e6d4
0034a798: mov      r0, r4
0034a79c: mov      r1, #1
0034a7a0: bl       #0x33e61c
0034a7a4: ldr      r6, [r6]
0034a7a8: cmp      sb, r6
0034a7ac: bne      #0x34a770
0034a7b0: mov      r3, #0
0034a7b4: str      r3, [r5, #0x58]
0034a7b8: str      r3, [r5, #0x5c]
0034a7bc: ldr      r4, [r5, #0x2c]
0034a7c0: add      r3, sp, #0x3c
0034a7c4: str      r3, [sp, #0xc]
0034a7c8: ldr      r3, [pc, #0x430]
0034a7cc: add      ip, sp, #0x24
0034a7d0: str      ip, [sp, #0x10]
0034a7d4: add      r2, sp, #0x30
0034a7d8: add      ip, sp, #0x60
0034a7dc: cmp      r8, r4
0034a7e0: add      r6, r5, #0x70
0034a7e4: str      r2, [sp, #0x14]
0034a7e8: str      r3, [sp, #0x18]
0034a7ec: str      ip, [sp, #0x1c]
0034a7f0: beq      #0x34a8dc
0034a7f4: ldr      sb, [r4, #8]
0034a7f8: cmp      sb, #0
0034a7fc: beq      #0x34a8cc
0034a800: ldr      r3, [sb]
0034a804: mov      r0, sb
0034a808: mov      lr, pc
0034a80c: ldr      pc, [r3, #0x24]
0034a810: cmp      r0, #0
0034a814: bne      #0x34aa64
0034a818: ldrb     r3, [sb, #0x85]
0034a81c: cmp      r3, #0
0034a820: beq      #0x34aa10
0034a824: ldrb     r3, [sb, #0x8a]
0034a828: cmp      r3, #0
0034a82c: beq      #0x34aa10
0034a830: ldrb     fp, [sb, #0x81]
0034a834: cmp      fp, #0
0034a838: bne      #0x34a9e0
0034a83c: ldr      r3, [sb]
0034a840: mov      r0, sb
0034a844: strb     fp, [sb, #0x88]
0034a848: mov      lr, pc
0034a84c: ldr      pc, [r3, #0x2c]
0034a850: ldr      r0, [sp, #0xc]
0034a854: mov      r1, sb
0034a858: bl       #0x33dd2c
0034a85c: ldr      r0, [sp, #0xc]
0034a860: mov      r1, fp
0034a864: bl       #0x33fdc0
0034a868: cmp      r0, #0
0034a86c: beq      #0x34a8cc
0034a870: ldrb     r3, [sb, #0x88]
0034a874: ldrb     r2, [sb, #0x89]
0034a878: cmp      r2, r3
0034a87c: beq      #0x34a8cc
0034a880: cmp      r3, #0
0034a884: strb     r3, [sb, #0x89]
0034a888: bne      #0x34aad4
0034a88c: mov      r1, sb
0034a890: ldr      r0, [sp, #0x10]
0034a894: bl       #0x33dd2c
0034a898: ldr      r0, [sp, #0x10]
0034a89c: bl       #0x33ff54
0034a8a0: mov      fp, r0
0034a8a4: ldr      r0, [r5, #0x70]
0034a8a8: cmp      r6, r0
0034a8ac: beq      #0x34a8cc
0034a8b0: ldr      r3, [r0, #8]
0034a8b4: ldr      sb, [r0]
0034a8b8: cmp      fp, r3
0034a8bc: beq      #0x34aab8
0034a8c0: mov      r0, sb
0034a8c4: cmp      r6, r0
0034a8c8: bne      #0x34a8b0
0034a8cc: ldr      sb, [r4]
0034a8d0: mov      r4, sb
0034a8d4: cmp      r8, r4
0034a8d8: bne      #0x34a7f4
0034a8dc: ldr      r5, [pc, #0x320]
0034a8e0: add      r4, sp, #0x94
0034a8e4: ldr      r6, [r7, r5]
0034a8e8: mov      r0, r6
0034a8ec: bl       #0x337888
0034a8f0: ldr      r1, [pc, #0x310]
0034a8f4: add      r2, sp, #0x78
0034a8f8: mov      r0, r4
0034a8fc: add      r1, pc, r1
0034a900: bl       #0x3140ec
0034a904: mov      r0, r6
0034a908: mov      r1, r4
0034a90c: mov      r2, #0
0034a910: bl       #0x337ddc
0034a914: ldr      r0, [sp, #0xa8]
0034a918: cmp      r0, r4
0034a91c: beq      #0x34a93c
0034a920: cmp      r0, #0
0034a924: beq      #0x34a93c
0034a928: ldr      r1, [sp, #0x94]
0034a92c: rsb      r1, r0, r1
0034a930: cmp      r1, #0x80
0034a934: bhi      #0x34abdc
0034a938: bl       #0x708f00
0034a93c: ldr      r5, [r7, r5]
0034a940: add      r4, sp, #0x7c
0034a944: mov      r0, r5
0034a948: bl       #0x337888
0034a94c: ldr      r1, [pc, #0x2b8]
0034a950: add      r2, sp, #0x74
0034a954: mov      r0, r4
0034a958: add      r1, pc, r1
0034a95c: bl       #0x3140ec
0034a960: mov      r0, r5
0034a964: mov      r1, r4
0034a968: mov      r2, #0
0034a96c: bl       #0x337ddc
0034a970: ldr      r0, [sp, #0x90]
0034a974: cmp      r0, r4
0034a978: beq      #0x34a998
0034a97c: cmp      r0, #0
0034a980: beq      #0x34a998
0034a984: ldr      r1, [sp, #0x7c]
0034a988: rsb      r1, r0, r1
0034a98c: cmp      r1, #0x80
0034a990: bhi      #0x34abe4
0034a994: bl       #0x708f00
0034a998: ldr      r0, [pc, #0x270]
0034a99c: add      r0, pc, r0
0034a9a0: bl       #0x3136b8
0034a9a4: ldr      r3, [r7, sl]
0034a9a8: ldr      r2, [sp, #0xac]
0034a9ac: ldr      r3, [r3]
0034a9b0: cmp      r2, r3
0034a9b4: bne      #0x34abec
0034a9b8: add      sp, sp, #0xb4
0034a9bc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034a9c0: ldrb     r3, [r4, #0xd0]
0034a9c4: cmp      r3, #0
0034a9c8: bne      #0x34a7a4
0034a9cc: ldr      r3, [r4, #0xcc]
0034a9d0: cmp      r3, #0
0034a9d4: bne      #0x34a78c
0034a9d8: ldr      r6, [r6]
0034a9dc: b        #0x34a7a8
0034a9e0: mov      r1, sb
0034a9e4: mov      r0, r5
0034a9e8: bl       #0x3432f8
0034a9ec: ldr      sb, [r4]
0034a9f0: ldr      r3, [r4, #4]
0034a9f4: mov      r0, r4
0034a9f8: mov      r1, #0xc
0034a9fc: str      sb, [r3]
0034aa00: str      r3, [sb, #4]
0034aa04: bl       #0x708f00
0034aa08: mov      r4, sb
0034aa0c: b        #0x34a8d4
0034aa10: bl       #0x7fd794
0034aa14: ldrb     r3, [r0, #5]
0034aa18: cmp      r3, #0
0034aa1c: bne      #0x34aa70
0034aa20: mov      r2, #0
0034aa24: strb     r2, [sb, #0x86]
0034aa28: ldr      r3, [sb]
0034aa2c: mov      r0, sb
0034aa30: mov      lr, pc
0034aa34: ldr      pc, [r3, #0x24]
0034aa38: cmp      r0, #0
0034aa3c: beq      #0x34a8cc
0034aa40: ldr      ip, [sp, #0x18]
0034aa44: mov      r0, sb
0034aa48: ldr      r1, [sp, #0x1c]
0034aa4c: ldr      r3, [r7, ip]
0034aa50: mov      r2, #0
0034aa54: str      r3, [sp, #0x60]
0034aa58: bl       #0x3a7b24
0034aa5c: ldr      sb, [r4]
0034aa60: b        #0x34a8d0
0034aa64: mov      r0, sb
0034aa68: bl       #0x3a4344
0034aa6c: b        #0x34a818
0034aa70: ldr      r3, [sb]
0034aa74: mov      r0, sb
0034aa78: mov      lr, pc
0034aa7c: ldr      pc, [r3, #0x54]
0034aa80: cmp      r0, #0
0034aa84: beq      #0x34aa20
0034aa88: b        #0x34a830
0034aa8c: ldrb     r3, [r0, #0x198]
0034aa90: cmp      r3, #0
0034aa94: bne      #0x34a68c
0034aa98: bl       #0x7fd794
0034aa9c: ldrb     r3, [r0, #5]
0034aaa0: cmp      r3, #0
0034aaa4: bne      #0x34a68c
0034aaa8: ldr      r0, [pc, #0x164]
0034aaac: add      r0, pc, r0
0034aab0: bl       #0x3136b8
0034aab4: b        #0x34a9a4
0034aab8: ldr      r3, [r0, #4]
0034aabc: mov      r1, #0xc
0034aac0: str      sb, [r3]
0034aac4: str      r3, [sb, #4]
0034aac8: bl       #0x708f00
0034aacc: mov      r0, sb
0034aad0: b        #0x34a8c4
0034aad4: mov      r1, sb
0034aad8: ldr      r0, [sp, #0x14]
0034aadc: bl       #0x33dd2c
0034aae0: ldr      r0, [sp, #0x14]
0034aae4: bl       #0x33ff54
0034aae8: mov      sb, r0
0034aaec: mov      r0, r6
0034aaf0: bl       #0x342690
0034aaf4: str      sb, [r0, #8]
0034aaf8: ldr      r3, [r5, #0x74]
0034aafc: str      r6, [r0]
0034ab00: str      r3, [r0, #4]
0034ab04: str      r0, [r3]
0034ab08: str      r0, [r5, #0x74]
0034ab0c: ldr      sb, [r4]
0034ab10: b        #0x34a8d0
0034ab14: ldrb     r3, [r1, #0x82]
0034ab18: cmp      r3, #0
0034ab1c: beq      #0x34ab30
0034ab20: sub      r3, r3, #1
0034ab24: strb     r3, [r1, #0x82]
0034ab28: ldr      fp, [r4]
0034ab2c: b        #0x34a754
0034ab30: mov      r0, r5
0034ab34: bl       #0x347dec
0034ab38: cmp      r0, #0
0034ab3c: bne      #0x34ab98
0034ab40: ldr      r3, [r4, #8]
0034ab44: mov      r0, r3
0034ab48: ldr      r3, [r3]
0034ab4c: mov      lr, pc
0034ab50: ldr      pc, [r3, #0x24]
0034ab54: cmp      r0, #0
0034ab58: bne      #0x34aba0
0034ab5c: ldr      r1, [r4, #8]
0034ab60: mov      r0, sb
0034ab64: bl       #0x33f524
0034ab68: ldm      sb, {r1, r2, r3}
0034ab6c: mov      r0, r5
0034ab70: bl       #0x348ea4
0034ab74: ldr      fp, [r4]
0034ab78: ldr      r3, [r4, #4]
0034ab7c: mov      r0, r4
0034ab80: mov      r1, #0xc
0034ab84: str      fp, [r3]
0034ab88: str      r3, [fp, #4]
0034ab8c: bl       #0x708f00
0034ab90: mov      r4, fp
0034ab94: b        #0x34a758
0034ab98: ldr      r1, [r4, #8]
0034ab9c: b        #0x34a744
0034aba0: ldr      r3, [r4, #8]
0034aba4: mov      r0, r3
0034aba8: ldr      r3, [r3]
0034abac: mov      lr, pc
0034abb0: ldr      pc, [r3, #0x28]
0034abb4: cmp      r0, #0
0034abb8: beq      #0x34ab5c
0034abbc: ldr      r1, [r4, #8]
0034abc0: ldr      r0, [sp, #0xc]
0034abc4: bl       #0x33f524
0034abc8: ldr      ip, [sp, #0xc]
0034abcc: mov      r0, r5
0034abd0: ldm      ip, {r1, r2, r3}
0034abd4: bl       #0x349240
0034abd8: b        #0x34ab74
0034abdc: bl       #0x310440
0034abe0: b        #0x34a93c
0034abe4: bl       #0x310440
0034abe8: b        #0x34a998
0034abec: bl       #0x30e310
0034abf0: rsbeq    sl, r4, r0, ror #8
0034abf4: andeq    r4, r0, ip, lsr #1
0034abf8: subseq   r5, r7, ip, lsr sp
0034abfc: strdeq   r3, r4, [r0], -r4
0034ac00: andeq    r1, r0, r4, lsr r1
0034ac04: andeq    r0, r0, r4, lsl #17

# _ZN12ObjectHandle9GetObjectEb
0033fdc0: push     {r4, r5, r6, r7, r8, lr}
0033fdc4: ldr      r4, [r0]
0033fdc8: ldr      r5, [pc, #0xc0]
0033fdcc: sub      sp, sp, #8
0033fdd0: cmp      r4, #0
0033fdd4: mov      r6, r0
0033fdd8: mov      r7, r1
0033fddc: add      r5, pc, r5
0033fde0: beq      #0x33fe20
0033fde4: ldr      r3, [pc, #0xa8]
0033fde8: ldr      r4, [r0, #4]
0033fdec: ldr      r3, [r5, r3]
0033fdf0: cmp      r4, #0
0033fdf4: ldr      r0, [r3, #0x38]
0033fdf8: ldr      r8, [r0, #0x78]
0033fdfc: beq      #0x33fe0c
0033fe00: ldr      r3, [r6, #8]
0033fe04: cmp      r3, r8
0033fe08: beq      #0x33fe20
0033fe0c: add      r0, r0, #0xc
0033fe10: mov      r1, r6
0033fe14: bl       #0x33fc88
0033fe18: ldr      r4, [r0, #0x18]
0033fe1c: stmib    r6, {r4, r8}
0033fe20: cmp      r7, #0
0033fe24: beq      #0x33fe30
0033fe28: cmp      r4, #0
0033fe2c: beq      #0x33fe3c
0033fe30: mov      r0, r4
0033fe34: add      sp, sp, #8
0033fe38: pop      {r4, r5, r6, r7, r8, pc}
0033fe3c: ldr      r3, [pc, #0x54]
0033fe40: ldr      r3, [r5, r3]
0033fe44: ldr      r3, [r3]
0033fe48: cmp      r3, #2
0033fe4c: streq    r4, [r4]
0033fe50: beq      #0x33fe30
0033fe54: cmp      r3, #1
0033fe58: bne      #0x33fe30
0033fe5c: ldr      r0, [pc, #0x38]
0033fe60: ldr      r1, [pc, #0x38]
0033fe64: ldr      r2, [pc, #0x38]
0033fe68: ldr      r0, [r5, r0]
0033fe6c: ldr      r3, [pc, #0x34]
0033fe70: mov      ip, #0x31
0033fe74: add      r1, pc, r1
0033fe78: add      r2, pc, r2
0033fe7c: add      r3, pc, r3
0033fe80: add      r0, r0, #0xa8
0033fe84: str      ip, [sp]
0033fe88: bl       #0x30e004
0033fe8c: b        #0x33fe30
0033fe90: strhteq  r4, [r5], #-0xc4
0033fe94: strdeq   r3, r4, [r0], -r4
0033fe98: andeq    r3, r0, r0, asr #19
0033fe9c: andeq    r1, r0, r0, asr #19
0033fea0: subseq   lr, r7, r4, ror #10

# _ZN10ObjectBaseC1ENS_6GO_IDSE
0033f15c: push     {r4, r5, r6, r7, r8, lr}
0033f160: ldr      r6, [pc, #0x198]
0033f164: ldr      r2, [pc, #0x198]
0033f168: ldr      r3, [pc, #0x198]
0033f16c: add      r6, pc, r6
0033f170: ldr      r2, [r6, r2]
0033f174: ldr      r3, [r6, r3]
0033f178: mov      r4, r0
0033f17c: add      r2, r2, #8
0033f180: add      r0, r3, #8
0033f184: add      r3, r4, #8
0033f188: str      r2, [r4]
0033f18c: str      r0, [r4, #4]
0033f190: mov      r7, r1
0033f194: mov      r0, r3
0033f198: str      r3, [r4, #0x18]
0033f19c: str      r3, [r4, #0x1c]
0033f1a0: mov      r1, #0x10
0033f1a4: bl       #0x31167c
0033f1a8: ldr      r2, [pc, #0x15c]
0033f1ac: ldr      r1, [r4, #0x18]
0033f1b0: mov      r5, #0
0033f1b4: ldr      r2, [r6, r2]
0033f1b8: strb     r5, [r1]
0033f1bc: add      r3, r4, #0x30
0033f1c0: add      r1, r2, #0x74
0033f1c4: add      r0, r2, #8
0033f1c8: add      r2, r2, #0x68
0033f1cc: stm      r4, {r0, r2}
0033f1d0: str      r1, [r4, #0x24]
0033f1d4: mov      r0, r3
0033f1d8: str      r5, [r4, #0x20]
0033f1dc: strb     r5, [r4, #0x28]
0033f1e0: strb     r5, [r4, #0x29]
0033f1e4: str      r5, [r4, #0x2c]
0033f1e8: str      r3, [r4, #0x40]
0033f1ec: str      r3, [r4, #0x44]
0033f1f0: mov      r1, #0x10
0033f1f4: bl       #0x31167c
0033f1f8: ldr      r2, [r4, #0x40]
0033f1fc: add      r3, r4, #0x48
0033f200: mov      r0, r3
0033f204: strb     r5, [r2]
0033f208: mov      r1, #0x10
0033f20c: str      r3, [r4, #0x58]
0033f210: str      r3, [r4, #0x5c]
0033f214: bl       #0x31167c
0033f218: ldr      r2, [r4, #0x58]
0033f21c: add      r3, r4, #0x68
0033f220: mvn      r6, #0
0033f224: strb     r5, [r2]
0033f228: mov      r1, #0x10
0033f22c: mov      r0, r3
0033f230: strb     r5, [r4, #0x60]
0033f234: str      r3, [r4, #0x78]
0033f238: str      r3, [r4, #0x7c]
0033f23c: str      r6, [r4, #0x64]
0033f240: bl       #0x31167c
0033f244: ldr      r3, [r4, #0x78]
0033f248: add      r0, r4, #0x8c
0033f24c: strb     r5, [r3]
0033f250: mov      r3, #1
0033f254: strb     r3, [r4, #0x8a]
0033f258: strb     r5, [r4, #0x81]
0033f25c: strb     r5, [r4, #0x84]
0033f260: strb     r5, [r4, #0x85]
0033f264: strb     r5, [r4, #0x86]
0033f268: strb     r5, [r4, #0x88]
0033f26c: strb     r5, [r4, #0x89]
0033f270: bl       #0x33ed7c
0033f274: add      r0, r4, #0xb0
0033f278: bl       #0x33ed7c
0033f27c: add      r3, r4, #0xd4
0033f280: mov      r0, r3
0033f284: str      r3, [r4, #0xe4]
0033f288: str      r3, [r4, #0xe8]
0033f28c: mov      r1, #0x10
0033f290: bl       #0x31167c
0033f294: ldr      r3, [r4, #0xe4]
0033f298: mov      r1, r5
0033f29c: mov      r0, #0xc
0033f2a0: strb     r5, [r3]
0033f2a4: mov      r3, #0
0033f2a8: str      r3, [r4, #0x114]
0033f2ac: strb     r5, [r4, #0xf0]
0033f2b0: strb     r5, [r4, #0xf1]
0033f2b4: strb     r5, [r4, #0xf8]
0033f2b8: str      r5, [r4, #0xfc]
0033f2bc: str      r5, [r4, #0x100]
0033f2c0: str      r5, [r4, #0x104]
0033f2c4: strb     r5, [r4, #0x10c]
0033f2c8: strb     r5, [r4, #0x118]
0033f2cc: strb     r5, [r4, #0x119]
0033f2d0: str      r5, [r4, #0x11c]
0033f2d4: str      r7, [r4, #0xf4]
0033f2d8: str      r6, [r4, #0x110]
0033f2dc: str      r6, [r4, #0xec]
0033f2e0: str      r6, [r4, #0x108]
0033f2e4: bl       #0x310570
0033f2e8: mov      r5, r0
0033f2ec: bl       #0x33f50c
0033f2f0: str      r5, [r4, #0x2c]
0033f2f4: mov      r0, r4
0033f2f8: str      r4, [r5, #4]
0033f2fc: pop      {r4, r5, r6, r7, r8, pc}
0033f300: rsbeq    r5, r5, r4, lsr #18
0033f304: andeq    r1, r0, ip, lsl #1
0033f308: ldrdeq   r3, r4, [r0], -ip
0033f30c: andeq    r3, r0, r4, lsl #23

# _ZN12ObjectHandleC1Ev
0033f50c: mov      r2, #0
0033f510: mvn      r1, #0
0033f514: str      r1, [r0, #8]
0033f518: str      r2, [r0, #4]
0033f51c: str      r2, [r0]
0033f520: bx       lr

# _ZN13ObjectManager29AddOrphanRenderObjectToDeleteEP10ObjectBase
00343188: push     {r4, r5, r6, lr}
0034318c: subs     r6, r1, #0
00343190: mov      r4, r0
00343194: beq      #0x3431bc
00343198: add      r5, r0, #4
0034319c: mov      r0, r5
003431a0: bl       #0x343168
003431a4: str      r6, [r0, #8]
003431a8: ldr      r3, [r4, #8]
003431ac: str      r5, [r0]
003431b0: str      r3, [r0, #4]
003431b4: str      r0, [r3]
003431b8: str      r0, [r4, #8]
003431bc: pop      {r4, r5, r6, pc}
