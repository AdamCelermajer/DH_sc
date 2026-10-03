
# _ZN13PhysicalWorld3AddEPK14b2ContactPoint
0034c6c4: push     {r4, r5, r6, r7, r8, sl, lr}
0034c6c8: ldr      r4, [pc, #0x10c]
0034c6cc: ldr      r8, [pc, #0x10c]
0034c6d0: ldr      r2, [pc, #0x10c]
0034c6d4: add      r4, pc, r4
0034c6d8: ldr      r3, [r4, r8]
0034c6dc: ldr      r7, [r4, r2]
0034c6e0: sub      sp, sp, #0x34
0034c6e4: ldr      r3, [r3]
0034c6e8: mov      sl, r0
0034c6ec: mov      r0, r7
0034c6f0: str      r3, [sp, #0x2c]
0034c6f4: mov      r5, r1
0034c6f8: bl       #0x337888
0034c6fc: ldr      r1, [pc, #0xe4]
0034c700: add      r6, sp, #0x14
0034c704: mov      r0, r6
0034c708: add      r1, pc, r1
0034c70c: add      r1, r1, #0x16
0034c710: str      r6, [sp, #0x24]
0034c714: str      r6, [sp, #0x28]
0034c718: bl       #0x34c3c0
0034c71c: mov      r1, r6
0034c720: mov      r0, r7
0034c724: bl       #0x337a88
0034c728: mov      r0, r6
0034c72c: bl       #0x3139ac
0034c730: ldm      r5, {r2, r3}
0034c734: ldr      r7, [r2, #0x2c]
0034c738: ldr      r6, [r3, #0x2c]
0034c73c: cmp      r6, #0
0034c740: cmpne    r7, #0
0034c744: beq      #0x34c7bc
0034c748: mov      r0, sl
0034c74c: mov      r1, r5
0034c750: mov      r2, r7
0034c754: mov      r3, r6
0034c758: bl       #0x34c1e4
0034c75c: ldr      r1, [r7]
0034c760: ldr      r2, [r5, #8]
0034c764: ldr      r3, [r5, #0xc]
0034c768: mov      sl, r0
0034c76c: ldr      ip, [r1, #0xc]
0034c770: mov      r0, r7
0034c774: str      r2, [sp, #0xc]
0034c778: str      r3, [sp, #0x10]
0034c77c: mov      r1, r6
0034c780: add      r2, sp, #0xc
0034c784: mov      r3, sl
0034c788: blx      ip
0034c78c: ldr      r0, [r6]
0034c790: ldr      r2, [r5, #0xc]
0034c794: ldr      r1, [r5, #8]
0034c798: eor      r3, sl, #1
0034c79c: ldr      ip, [r0, #0xc]
0034c7a0: uxtb     r3, r3
0034c7a4: str      r1, [sp, #4]
0034c7a8: str      r2, [sp, #8]
0034c7ac: mov      r0, r6
0034c7b0: mov      r1, r7
0034c7b4: add      r2, sp, #4
0034c7b8: blx      ip
0034c7bc: ldr      r3, [r4, r8]
0034c7c0: ldr      r2, [sp, #0x2c]
0034c7c4: ldr      r3, [r3]
0034c7c8: cmp      r2, r3
0034c7cc: bne      #0x34c7d8
0034c7d0: add      sp, sp, #0x34
0034c7d4: pop      {r4, r5, r6, r7, r8, sl, pc}
0034c7d8: bl       #0x30e310
0034c7dc: strhteq  r8, [r4], #-0x3c
0034c7e0: andeq    r4, r0, ip, lsr #1
0034c7e4: andeq    r0, r0, r4, lsl #17
0034c7e8: subseq   r3, r7, r8, lsr #30

# _ZN13PhysicalWorld13ShouldCollideEP7b2ShapeS1_
0034c304: push     {r4, r5, r6, r7, r8, sl, lr}
0034c308: ldr      r6, [r1, #0x2c]
0034c30c: ldr      r7, [r2, #0x2c]
0034c310: sub      sp, sp, #0x14
0034c314: mov      r4, r1
0034c318: cmp      r7, #0
0034c31c: cmpne    r6, #0
0034c320: mov      r5, r2
0034c324: beq      #0x34c3b0
0034c328: ldrsh    r2, [r1, #0x26]
0034c32c: ldrh     r3, [r1, #0x22]
0034c330: ldrh     r8, [r1, #0x24]
0034c334: ldrsh    lr, [r5, #0x26]
0034c338: ldrh     r0, [r5, #0x22]
0034c33c: ldrh     r1, [r5, #0x24]
0034c340: ldr      ip, [r6]
0034c344: stm      sp, {r8, lr}
0034c348: str      r0, [sp, #8]
0034c34c: str      r1, [sp, #0xc]
0034c350: mov      r0, r6
0034c354: mov      r1, r7
0034c358: mov      lr, pc
0034c35c: ldr      pc, [ip, #8]
0034c360: ldrh     lr, [r5, #0x24]
0034c364: ldrh     r8, [r4, #0x24]
0034c368: ldrsh    r2, [r5, #0x26]
0034c36c: ldrh     r3, [r5, #0x22]
0034c370: ldrsh    r5, [r4, #0x26]
0034c374: ldrh     r4, [r4, #0x22]
0034c378: ldr      ip, [r7]
0034c37c: mov      sl, r0
0034c380: mov      r1, r6
0034c384: mov      r0, r7
0034c388: str      lr, [sp]
0034c38c: str      r5, [sp, #4]
0034c390: str      r4, [sp, #8]
0034c394: str      r8, [sp, #0xc]
0034c398: mov      lr, pc
0034c39c: ldr      pc, [ip, #8]
0034c3a0: cmp      sl, #0
0034c3a4: moveq    r0, #0
0034c3a8: add      sp, sp, #0x14
0034c3ac: pop      {r4, r5, r6, r7, r8, sl, pc}
0034c3b0: add      r0, r0, #4
0034c3b4: add      sp, sp, #0x14
0034c3b8: pop      {r4, r5, r6, r7, r8, sl, lr}
0034c3bc: b        #0x7e8c48

# _ZN13PhysicalWorld4loadEffff
0034c048: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0034c04c: ldr      r5, [pc, #0xf8]
0034c050: mov      sb, r3
0034c054: ldr      r3, [pc, #0xf4]
0034c058: add      r5, pc, r5
0034c05c: sub      sp, sp, #0x38
0034c060: ldr      r6, [r5, r3]
0034c064: mov      r4, r0
0034c068: mov      sl, r1
0034c06c: ldr      r3, [r6]
0034c070: mov      r8, r2
0034c074: add      r7, sp, #0x1c
0034c078: str      r3, [sp, #0x34]
0034c07c: bl       #0x34be0c
0034c080: ldr      r3, [pc, #0xcc]
0034c084: ldr      r5, [r5, r3]
0034c088: mov      r0, r5
0034c08c: bl       #0x337888
0034c090: ldr      r1, [pc, #0xc0]
0034c094: add      r2, sp, #0x18
0034c098: mov      r0, r7
0034c09c: add      r1, pc, r1
0034c0a0: bl       #0x3140ec
0034c0a4: mov      r1, r7
0034c0a8: mov      r0, r5
0034c0ac: bl       #0x337a88
0034c0b0: mov      r0, r7
0034c0b4: bl       #0x3139ac
0034c0b8: ldr      r2, [sp, #0x58]
0034c0bc: mov      r0, #0x19000
0034c0c0: mov      r3, #0
0034c0c4: mov      r1, #0
0034c0c8: add      r0, r0, #0x278
0034c0cc: str      r2, [sp, #0xc]
0034c0d0: str      r3, [sp, #0x14]
0034c0d4: str      r3, [sp, #0x10]
0034c0d8: str      sl, [sp]
0034c0dc: str      r8, [sp, #4]
0034c0e0: str      sb, [sp, #8]
0034c0e4: bl       #0x310570
0034c0e8: add      r2, sp, #0x10
0034c0ec: mov      r3, #1
0034c0f0: mov      r5, r0
0034c0f4: mov      r1, sp
0034c0f8: bl       #0x7e80b8
0034c0fc: mov      r0, r5
0034c100: mov      r1, r4
0034c104: str      r5, [r4, #0x10]
0034c108: bl       #0x7e65e4
0034c10c: ldr      r0, [r4, #0x10]
0034c110: add      r1, r4, #4
0034c114: bl       #0x7e65f4
0034c118: ldr      r0, [r4, #0x10]
0034c11c: add      r1, r4, #8
0034c120: bl       #0x7e6604
0034c124: add      r1, r4, #0xc
0034c128: ldr      r0, [r4, #0x10]
0034c12c: bl       #0x7e65d4
0034c130: ldr      r2, [sp, #0x34]
0034c134: ldr      r3, [r6]
0034c138: cmp      r2, r3
0034c13c: bne      #0x34c148
0034c140: add      sp, sp, #0x38
0034c144: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
0034c148: bl       #0x30e310
0034c14c: rsbeq    r8, r4, r8, lsr sl
0034c150: andeq    r4, r0, ip, lsr #1
0034c154: andeq    r0, r0, r4, lsl #17

# _ZN13PhysicalWorld7PersistEPK14b2ContactPoint
0034c7f4: push     {r4, r5, r6, r7, r8, sl, lr}
0034c7f8: ldr      r4, [pc, #0x10c]
0034c7fc: ldr      r8, [pc, #0x10c]
0034c800: ldr      r2, [pc, #0x10c]
0034c804: add      r4, pc, r4
0034c808: ldr      r3, [r4, r8]
0034c80c: ldr      r7, [r4, r2]
0034c810: sub      sp, sp, #0x34
0034c814: ldr      r3, [r3]
0034c818: mov      sl, r0
0034c81c: mov      r0, r7
0034c820: str      r3, [sp, #0x2c]
0034c824: mov      r5, r1
0034c828: bl       #0x337888
0034c82c: ldr      r1, [pc, #0xe4]
0034c830: add      r6, sp, #0x14
0034c834: mov      r0, r6
0034c838: add      r1, pc, r1
0034c83c: add      r1, r1, #0x16
0034c840: str      r6, [sp, #0x24]
0034c844: str      r6, [sp, #0x28]
0034c848: bl       #0x34c3c0
0034c84c: mov      r1, r6
0034c850: mov      r0, r7
0034c854: bl       #0x337a88
0034c858: mov      r0, r6
0034c85c: bl       #0x3139ac
0034c860: ldm      r5, {r2, r3}
0034c864: ldr      r7, [r2, #0x2c]
0034c868: ldr      r6, [r3, #0x2c]
0034c86c: cmp      r6, #0
0034c870: cmpne    r7, #0
0034c874: beq      #0x34c8ec
0034c878: mov      r0, sl
0034c87c: mov      r1, r5
0034c880: mov      r2, r7
0034c884: mov      r3, r6
0034c888: bl       #0x34c1e4
0034c88c: ldr      r1, [r7]
0034c890: ldr      r2, [r5, #8]
0034c894: ldr      r3, [r5, #0xc]
0034c898: mov      sl, r0
0034c89c: ldr      ip, [r1, #0x10]
0034c8a0: mov      r0, r7
0034c8a4: str      r2, [sp, #0xc]
0034c8a8: str      r3, [sp, #0x10]
0034c8ac: mov      r1, r6
0034c8b0: add      r2, sp, #0xc
0034c8b4: mov      r3, sl
0034c8b8: blx      ip
0034c8bc: ldr      r0, [r6]
0034c8c0: ldr      r2, [r5, #0xc]
0034c8c4: ldr      r1, [r5, #8]
0034c8c8: eor      r3, sl, #1
0034c8cc: ldr      ip, [r0, #0x10]
0034c8d0: uxtb     r3, r3
0034c8d4: str      r1, [sp, #4]
0034c8d8: str      r2, [sp, #8]
0034c8dc: mov      r0, r6
0034c8e0: mov      r1, r7
0034c8e4: add      r2, sp, #4
0034c8e8: blx      ip
0034c8ec: ldr      r3, [r4, r8]
0034c8f0: ldr      r2, [sp, #0x2c]
0034c8f4: ldr      r3, [r3]
0034c8f8: cmp      r2, r3
0034c8fc: bne      #0x34c908
0034c900: add      sp, sp, #0x34
0034c904: pop      {r4, r5, r6, r7, r8, sl, pc}
0034c908: bl       #0x30e310
0034c90c: rsbeq    r8, r4, ip, lsl #5
0034c910: andeq    r4, r0, ip, lsr #1
0034c914: andeq    r0, r0, r4, lsl #17
0034c918: ldrsheq  r3, [r7], #-0xd8

# _ZN13PhysicalWorld11destroyBodyERP6b2Body
0034bcc8: push     {r4, lr}
0034bccc: mov      r4, r1
0034bcd0: ldr      r1, [r1]
0034bcd4: cmp      r1, #0
0034bcd8: beq      #0x34bce4
0034bcdc: ldr      r0, [r0, #0x10]
0034bce0: bl       #0x7e7db0
0034bce4: mov      r3, #0
0034bce8: str      r3, [r4]
0034bcec: pop      {r4, pc}

# _ZN13PhysicalWorld10createBodyEP9b2BodyDef
0034bcf0: subs     r3, r1, #0
0034bcf4: beq      #0x34bd00
0034bcf8: ldr      r0, [r0, #0x10]
0034bcfc: b        #0x7e7ef8
0034bd00: mov      r0, r3
0034bd04: bx       lr

# _ZN13PhysicalWorld6updateEv
0034bd08: push     {r4, r5, r6, lr}
0034bd0c: ldr      r4, [pc, #0x50]
0034bd10: mov      r6, r0
0034bd14: ldr      r5, [pc, #0x4c]
0034bd18: add      r4, pc, r4
0034bd1c: mov      r0, r4
0034bd20: bl       #0x3136b4
0034bd24: ldr      r3, [pc, #0x40]
0034bd28: add      r5, pc, r5
0034bd2c: ldr      r0, [r5, r3]
0034bd30: bl       #0x31f66c
0034bd34: bl       #0x30e2e0
0034bd38: movw     r1, #0x126f
0034bd3c: movt     r1, #0x3a83
0034bd40: bl       #0x30ed6c
0034bd44: ldr      r5, [r6, #0x10]
0034bd48: mov      r1, r0
0034bd4c: mov      r2, #0xa
0034bd50: mov      r0, r5
0034bd54: bl       #0x7e8b1c
0034bd58: mov      r0, r4
0034bd5c: pop      {r4, r5, r6, lr}
0034bd60: b        #0x3136b8
0034bd64: subseq   r4, r7, r0, lsl #18
0034bd68: rsbeq    r8, r4, r8, ror #26
0034bd6c: strdeq   r3, r4, [r0], -r4

# _ZN13PhysicalWorld6ResultEPK15b2ContactResult
0034c548: push     {r4, r5, r6, r7, r8, lr}
0034c54c: ldr      r4, [pc, #0xc8]
0034c550: ldr      r7, [pc, #0xc8]
0034c554: ldr      r2, [pc, #0xc8]
0034c558: add      r4, pc, r4
0034c55c: ldr      r3, [r4, r7]
0034c560: ldr      r8, [r4, r2]
0034c564: sub      sp, sp, #0x20
0034c568: ldr      r3, [r3]
0034c56c: mov      r0, r8
0034c570: mov      r6, r1
0034c574: str      r3, [sp, #0x1c]
0034c578: bl       #0x337888
0034c57c: ldr      r1, [pc, #0xa4]
0034c580: add      r5, sp, #4
0034c584: mov      r0, r5
0034c588: add      r1, pc, r1
0034c58c: add      r1, r1, #0x16
0034c590: str      r5, [sp, #0x14]
0034c594: str      r5, [sp, #0x18]
0034c598: bl       #0x34c3c0
0034c59c: mov      r1, r5
0034c5a0: mov      r0, r8
0034c5a4: bl       #0x337a88
0034c5a8: mov      r0, r5
0034c5ac: bl       #0x3139ac
0034c5b0: ldr      r2, [r6, #4]
0034c5b4: ldr      r3, [r6]
0034c5b8: ldr      r5, [r2, #0x2c]
0034c5bc: ldr      r6, [r3, #0x2c]
0034c5c0: cmp      r5, #0
0034c5c4: cmpne    r6, #0
0034c5c8: beq      #0x34c5fc
0034c5cc: mov      r0, r6
0034c5d0: mov      r1, r5
0034c5d4: mov      r2, #1
0034c5d8: ldr      r3, [r6]
0034c5dc: mov      lr, pc
0034c5e0: ldr      pc, [r3, #0x18]
0034c5e4: mov      r0, r5
0034c5e8: mov      r1, r6
0034c5ec: ldr      r3, [r5]
0034c5f0: mov      r2, #0
0034c5f4: mov      lr, pc
0034c5f8: ldr      pc, [r3, #0x18]
0034c5fc: ldr      r3, [r4, r7]
0034c600: ldr      r2, [sp, #0x1c]
0034c604: ldr      r3, [r3]
0034c608: cmp      r2, r3
0034c60c: bne      #0x34c618
0034c610: add      sp, sp, #0x20
0034c614: pop      {r4, r5, r6, r7, r8, pc}
0034c618: bl       #0x30e310
0034c61c: rsbeq    r8, r4, r8, lsr r5
0034c620: andeq    r4, r0, ip, lsr #1
0034c624: andeq    r0, r0, r4, lsl #17
0034c628: subseq   r4, r7, r8, lsr #1

# _ZN13PhysicalWorld19_IsShape1InstigatorEPK14b2ContactPointP18PhysicalBaseObjectS4_
0034c1e4: push     {r4, r5, r6, r7, r8, lr}
0034c1e8: ldr      r3, [r1]
0034c1ec: mov      r4, #0
0034c1f0: sub      sp, sp, #8
0034c1f4: ldr      r6, [r3, #0xc]
0034c1f8: mov      r5, r1
0034c1fc: mov      r1, r4
0034c200: ldr      r0, [r6, #0x74]
0034c204: mov      r7, r2
0034c208: bl       #0x30e2f8
0034c20c: cmp      r0, #0
0034c210: beq      #0x34c298
0034c214: ldr      r1, [r6, #4]
0034c218: ldr      r0, [r5, #8]
0034c21c: bl       #0x30e3ac
0034c220: ldr      r1, [r6, #8]
0034c224: mov      r8, r0
0034c228: ldr      r0, [r5, #0xc]
0034c22c: bl       #0x30e3ac
0034c230: str      r4, [sp]
0034c234: str      r4, [sp, #4]
0034c238: ldr      r3, [r7]
0034c23c: mov      r6, r0
0034c240: mov      r1, sp
0034c244: mov      r0, r7
0034c248: mov      lr, pc
0034c24c: ldr      pc, [r3, #0x1c]
0034c250: ldr      r1, [sp]
0034c254: mov      r0, r8
0034c258: bl       #0x30ed6c
0034c25c: ldr      r1, [sp, #4]
0034c260: mov      r5, r0
0034c264: mov      r0, r6
0034c268: bl       #0x30ed6c
0034c26c: mov      r1, r0
0034c270: mov      r0, r5
0034c274: bl       #0x30eba4
0034c278: mov      r1, r4
0034c27c: bl       #0x30e2f8
0034c280: cmp      r0, #0
0034c284: mov      r0, #0
0034c288: movne    r0, #1
0034c28c: uxtb     r0, r0
0034c290: add      sp, sp, #8
0034c294: pop      {r4, r5, r6, r7, r8, pc}
0034c298: ldr      r3, [r5, #4]
0034c29c: mov      r1, r4
0034c2a0: mov      r4, #0
0034c2a4: ldr      r3, [r3, #0xc]
0034c2a8: ldr      r0, [r3, #0x74]
0034c2ac: bl       #0x30e2f8
0034c2b0: cmp      r0, #0
0034c2b4: movne    r4, #1
0034c2b8: eor      r4, r4, #1
0034c2bc: uxtb     r0, r4
0034c2c0: b        #0x34c290

# _ZN13PhysicalWorld6RemoveEPK14b2ContactPoint
0034c418: push     {r4, r5, r6, r7, r8, sl, lr}
0034c41c: ldr      r4, [pc, #0x10c]
0034c420: ldr      r8, [pc, #0x10c]
0034c424: ldr      r2, [pc, #0x10c]
0034c428: add      r4, pc, r4
0034c42c: ldr      r3, [r4, r8]
0034c430: ldr      r7, [r4, r2]
0034c434: sub      sp, sp, #0x34
0034c438: ldr      r3, [r3]
0034c43c: mov      sl, r0
0034c440: mov      r0, r7
0034c444: str      r3, [sp, #0x2c]
0034c448: mov      r5, r1
0034c44c: bl       #0x337888
0034c450: ldr      r1, [pc, #0xe4]
0034c454: add      r6, sp, #0x14
0034c458: mov      r0, r6
0034c45c: add      r1, pc, r1
0034c460: add      r1, r1, #0x16
0034c464: str      r6, [sp, #0x24]
0034c468: str      r6, [sp, #0x28]
0034c46c: bl       #0x34c3c0
0034c470: mov      r1, r6
0034c474: mov      r0, r7
0034c478: bl       #0x337a88
0034c47c: mov      r0, r6
0034c480: bl       #0x3139ac
0034c484: ldm      r5, {r2, r3}
0034c488: ldr      r7, [r2, #0x2c]
0034c48c: ldr      r6, [r3, #0x2c]
0034c490: cmp      r6, #0
0034c494: cmpne    r7, #0
0034c498: beq      #0x34c510
0034c49c: mov      r0, sl
0034c4a0: mov      r1, r5
0034c4a4: mov      r2, r7
0034c4a8: mov      r3, r6
0034c4ac: bl       #0x34c1e4
0034c4b0: ldr      r1, [r7]
0034c4b4: ldr      r2, [r5, #8]
0034c4b8: ldr      r3, [r5, #0xc]
0034c4bc: mov      sl, r0
0034c4c0: ldr      ip, [r1, #0x14]
0034c4c4: mov      r0, r7
0034c4c8: str      r2, [sp, #0xc]
0034c4cc: str      r3, [sp, #0x10]
0034c4d0: mov      r1, r6
0034c4d4: add      r2, sp, #0xc
0034c4d8: mov      r3, sl
0034c4dc: blx      ip
0034c4e0: ldr      r0, [r6]
0034c4e4: ldr      r2, [r5, #0xc]
0034c4e8: ldr      r1, [r5, #8]
0034c4ec: eor      r3, sl, #1
0034c4f0: ldr      ip, [r0, #0x14]
0034c4f4: uxtb     r3, r3
0034c4f8: str      r1, [sp, #4]
0034c4fc: str      r2, [sp, #8]
0034c500: mov      r0, r6
0034c504: mov      r1, r7
0034c508: add      r2, sp, #4
0034c50c: blx      ip
0034c510: ldr      r3, [r4, r8]
0034c514: ldr      r2, [sp, #0x2c]
0034c518: ldr      r3, [r3]
0034c51c: cmp      r2, r3
0034c520: bne      #0x34c52c
0034c524: add      sp, sp, #0x34
0034c528: pop      {r4, r5, r6, r7, r8, sl, pc}
0034c52c: bl       #0x30e310
0034c530: rsbeq    r8, r4, r8, ror #12
0034c534: andeq    r4, r0, ip, lsr #1
0034c538: andeq    r0, r0, r4, lsl #17
0034c53c: ldrsbeq  r4, [r7], #-0x14
