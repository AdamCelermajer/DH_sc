
# _ZN7gameswf9character9constructEv
00752d84: bx       lr

# _ZN7gameswf9character13do_mouse_dragEv
0075e3a8: push     {r4, r5, r6, r7, r8, sl, lr}
0075e3ac: mov      r3, #0
0075e3b0: sub      sp, sp, #0x94
0075e3b4: mov      r2, #0
0075e3b8: mov      r1, #0x3f800000
0075e3bc: str      r1, [sp, #0x18]
0075e3c0: str      r1, [sp, #0x14]
0075e3c4: strb     r2, [sp, #0xa]
0075e3c8: str      r3, [sp, #0x20]
0075e3cc: str      r2, [sp, #4]
0075e3d0: strb     r2, [sp, #8]
0075e3d4: strb     r2, [sp, #9]
0075e3d8: str      r3, [sp, #0xc]
0075e3dc: str      r3, [sp, #0x10]
0075e3e0: str      r3, [sp, #0x1c]
0075e3e4: add      r6, sp, #4
0075e3e8: ldr      r3, [r0]
0075e3ec: mov      r1, r6
0075e3f0: mov      r4, r0
0075e3f4: mov      lr, pc
0075e3f8: ldr      pc, [r3, #0xdc]
0075e3fc: ldr      r5, [sp, #4]
0075e400: cmp      r5, r4
0075e404: beq      #0x75e410
0075e408: add      sp, sp, #0x94
0075e40c: pop      {r4, r5, r6, r7, r8, sl, pc}
0075e410: mov      r3, #1
0075e414: strb     r3, [r5, #0x9d]
0075e418: mov      r0, r5
0075e41c: ldr      r3, [r5]
0075e420: mov      lr, pc
0075e424: ldr      pc, [r3, #0x134]
0075e428: add      r7, sp, #0x8c
0075e42c: add      r8, sp, #0x88
0075e430: add      r3, sp, #0x84
0075e434: ldr      ip, [r0]
0075e438: mov      r1, r7
0075e43c: mov      r2, r8
0075e440: mov      sl, r5
0075e444: mov      lr, pc
0075e448: ldr      pc, [ip, #0x70]
0075e44c: ldr      r3, [sl, #0x54]
0075e450: cmp      r3, #0
0075e454: beq      #0x75e464
0075e458: ldr      r0, [r3, #0x68]
0075e45c: cmp      r0, #0
0075e460: bne      #0x75e710
0075e464: ldr      r3, [sl, #0x40]
0075e468: cmp      r3, #0
0075e46c: beq      #0x75e4b8
0075e470: ldr      r0, [sl, #0x3c]
0075e474: ldrb     r2, [r0, #4]
0075e478: cmp      r2, #0
0075e47c: beq      #0x75e494
0075e480: mov      sl, r3
0075e484: ldr      r3, [sl, #0x54]
0075e488: cmp      r3, #0
0075e48c: bne      #0x75e458
0075e490: b        #0x75e464
0075e494: ldr      r1, [r0]
0075e498: sub      r1, r1, #1
0075e49c: cmp      r1, #0
0075e4a0: str      r1, [r0]
0075e4a4: bne      #0x75e4ac
0075e4a8: bl       #0x752b38
0075e4ac: mov      r3, #0
0075e4b0: str      r3, [sl, #0x40]
0075e4b4: str      r3, [sl, #0x3c]
0075e4b8: ldr      r0, [sp, #0x88]
0075e4bc: bl       #0x30e964
0075e4c0: mov      r1, #0x41000000
0075e4c4: add      r1, r1, #0xa00000
0075e4c8: bl       #0x30ed6c
0075e4cc: mov      r7, r0
0075e4d0: ldr      r0, [sp, #0x8c]
0075e4d4: bl       #0x30e964
0075e4d8: mov      r1, #0x41000000
0075e4dc: add      r1, r1, #0xa00000
0075e4e0: bl       #0x30ed6c
0075e4e4: str      r0, [sp, #0x7c]
0075e4e8: mov      r0, r5
0075e4ec: str      r7, [sp, #0x80]
0075e4f0: bl       #0x753f74
0075e4f4: add      ip, sp, #0x54
0075e4f8: mov      lr, r0
0075e4fc: ldm      lr!, {r0, r1, r2, r3}
0075e500: stm      ip!, {r0, r1, r2, r3}
0075e504: ldm      lr, {r0, r1}
0075e508: add      r7, sp, #0x7c
0075e50c: mov      r3, #0
0075e510: stm      ip, {r0, r1}
0075e514: add      r1, sp, #0x74
0075e518: add      r0, sp, #0x54
0075e51c: mov      r2, r7
0075e520: str      r3, [sp, #0x78]
0075e524: str      r3, [sp, #0x74]
0075e528: bl       #0x753d7c
0075e52c: ldrb     r3, [sp, #9]
0075e530: cmp      r3, #0
0075e534: bne      #0x75e720
0075e538: ldrb     r3, [sp, #8]
0075e53c: cmp      r3, #0
0075e540: beq      #0x75e950
0075e544: ldr      ip, [r5, #0x4c]
0075e548: add      r4, sp, #0x24
0075e54c: mov      lr, r4
0075e550: ldm      ip!, {r0, r1, r2, r3}
0075e554: stm      lr!, {r0, r1, r2, r3}
0075e558: ldm      ip, {r0, r1}
0075e55c: stm      lr, {r0, r1}
0075e560: ldr      r1, [sp, #0x1c]
0075e564: ldr      r0, [sp, #0x7c]
0075e568: bl       #0x30e3ac
0075e56c: mvn      r1, #0x800000
0075e570: mov      r7, r0
0075e574: bl       #0x30e4b4
0075e578: cmp      r0, #0
0075e57c: bne      #0x75e6e0
0075e580: mov      r7, #0
0075e584: ldr      r1, [sp, #0x20]
0075e588: ldr      r0, [sp, #0x80]
0075e58c: str      r7, [sp, #0x2c]
0075e590: bl       #0x30e3ac
0075e594: mvn      r1, #0x800000
0075e598: mov      r6, r0
0075e59c: bl       #0x30e4b4
0075e5a0: cmp      r0, #0
0075e5a4: bne      #0x75e6c4
0075e5a8: mov      r6, #0
0075e5ac: ldrb     r3, [sp, #0xa]
0075e5b0: str      r6, [sp, #0x38]
0075e5b4: cmp      r3, #0
0075e5b8: beq      #0x75e6b4
0075e5bc: mov      r1, #0x41000000
0075e5c0: add      r1, r1, #0xa00000
0075e5c4: ldr      r0, [sp, #0xc]
0075e5c8: bl       #0x30ed6c
0075e5cc: mov      r1, #0x41000000
0075e5d0: mov      r8, r0
0075e5d4: add      r1, r1, #0xa00000
0075e5d8: ldr      r0, [sp, #0x14]
0075e5dc: bl       #0x30ed6c
0075e5e0: mov      r1, r7
0075e5e4: mov      sl, r0
0075e5e8: bl       #0x30e2f8
0075e5ec: cmp      r0, #0
0075e5f0: moveq    r7, sl
0075e5f4: mov      r1, r7
0075e5f8: mov      r0, r8
0075e5fc: bl       #0x30e70c
0075e600: cmp      r0, #0
0075e604: moveq    r7, r8
0075e608: mov      r0, r7
0075e60c: mvn      r1, #0x800000
0075e610: bl       #0x30e4b4
0075e614: cmp      r0, #0
0075e618: beq      #0x75e708
0075e61c: mvn      r1, #0x80000000
0075e620: mov      r0, r7
0075e624: sub      r1, r1, #0x800000
0075e628: bl       #0x30e9ac
0075e62c: cmp      r0, #0
0075e630: beq      #0x75e708
0075e634: mov      r1, #0x41000000
0075e638: add      r1, r1, #0xa00000
0075e63c: ldr      r0, [sp, #0x10]
0075e640: str      r7, [sp, #0x2c]
0075e644: bl       #0x30ed6c
0075e648: mov      r1, #0x41000000
0075e64c: mov      r7, r0
0075e650: add      r1, r1, #0xa00000
0075e654: ldr      r0, [sp, #0x18]
0075e658: bl       #0x30ed6c
0075e65c: mov      r1, r6
0075e660: mov      r8, r0
0075e664: bl       #0x30e2f8
0075e668: cmp      r0, #0
0075e66c: moveq    r6, r8
0075e670: mov      r1, r6
0075e674: mov      r0, r7
0075e678: bl       #0x30e70c
0075e67c: cmp      r0, #0
0075e680: moveq    r6, r7
0075e684: mov      r0, r6
0075e688: mvn      r1, #0x800000
0075e68c: bl       #0x30e4b4
0075e690: cmp      r0, #0
0075e694: beq      #0x75e6fc
0075e698: mvn      r1, #0x80000000
0075e69c: mov      r0, r6
0075e6a0: sub      r1, r1, #0x800000
0075e6a4: bl       #0x30e9ac
0075e6a8: cmp      r0, #0
0075e6ac: beq      #0x75e6fc
0075e6b0: str      r6, [sp, #0x38]
0075e6b4: mov      r0, r5
0075e6b8: mov      r1, r4
0075e6bc: bl       #0x4121f8
0075e6c0: b        #0x75e408
0075e6c4: mvn      r1, #0x80000000
0075e6c8: mov      r0, r6
0075e6cc: sub      r1, r1, #0x800000
0075e6d0: bl       #0x30e9ac
0075e6d4: cmp      r0, #0
0075e6d8: bne      #0x75e5ac
0075e6dc: b        #0x75e5a8
0075e6e0: mvn      r1, #0x80000000
0075e6e4: mov      r0, r7
0075e6e8: sub      r1, r1, #0x800000
0075e6ec: bl       #0x30e9ac
0075e6f0: cmp      r0, #0
0075e6f4: bne      #0x75e584
0075e6f8: b        #0x75e580
0075e6fc: mov      r6, #0
0075e700: str      r6, [sp, #0x38]
0075e704: b        #0x75e6b4
0075e708: mov      r7, #0
0075e70c: b        #0x75e634
0075e710: mov      r1, r7
0075e714: mov      r2, r8
0075e718: bl       #0x7775c4
0075e71c: b        #0x75e4b8
0075e720: add      r4, sp, #0x3c
0075e724: mov      r2, #0
0075e728: add      r3, r4, #8
0075e72c: str      r2, [r3], #4
0075e730: str      r2, [r3], #4
0075e734: str      r2, [r3], #4
0075e738: mov      r1, #0x3f800000
0075e73c: str      r2, [r3]
0075e740: str      r2, [sp, #0x40]
0075e744: str      r1, [sp, #0x4c]
0075e748: str      r1, [sp, #0x3c]
0075e74c: ldr      r0, [r5, #0x40]
0075e750: cmp      r0, r2
0075e754: beq      #0x75e784
0075e758: ldr      r3, [r5, #0x3c]
0075e75c: ldrb     r6, [r3, #4]
0075e760: cmp      r6, r2
0075e764: beq      #0x75e998
0075e768: bl       #0x753f74
0075e76c: mov      lr, r4
0075e770: mov      ip, r0
0075e774: ldm      ip!, {r0, r1, r2, r3}
0075e778: stm      lr!, {r0, r1, r2, r3}
0075e77c: ldm      ip, {r0, r1}
0075e780: stm      lr, {r0, r1}
0075e784: mov      r3, #0
0075e788: mov      r0, r4
0075e78c: mov      r2, r7
0075e790: add      r1, sp, #0x6c
0075e794: str      r3, [sp, #0x70]
0075e798: str      r3, [sp, #0x6c]
0075e79c: bl       #0x753d7c
0075e7a0: ldr      ip, [r5, #0x4c]
0075e7a4: add      r4, sp, #0x24
0075e7a8: mov      lr, r4
0075e7ac: ldm      ip!, {r0, r1, r2, r3}
0075e7b0: stm      lr!, {r0, r1, r2, r3}
0075e7b4: ldr      r6, [sp, #0x6c]
0075e7b8: ldm      ip, {r0, r1}
0075e7bc: stm      lr, {r0, r1}
0075e7c0: mvn      r1, #0x800000
0075e7c4: mov      r0, r6
0075e7c8: bl       #0x30e4b4
0075e7cc: cmp      r0, #0
0075e7d0: beq      #0x75e934
0075e7d4: mvn      r1, #0x80000000
0075e7d8: mov      r0, r6
0075e7dc: sub      r1, r1, #0x800000
0075e7e0: bl       #0x30e9ac
0075e7e4: cmp      r0, #0
0075e7e8: beq      #0x75e934
0075e7ec: ldr      r7, [sp, #0x70]
0075e7f0: mvn      r1, #0x800000
0075e7f4: str      r6, [sp, #0x2c]
0075e7f8: mov      r0, r7
0075e7fc: bl       #0x30e4b4
0075e800: cmp      r0, #0
0075e804: beq      #0x75e92c
0075e808: mvn      r1, #0x80000000
0075e80c: mov      r0, r7
0075e810: sub      r1, r1, #0x800000
0075e814: bl       #0x30e9ac
0075e818: cmp      r0, #0
0075e81c: beq      #0x75e92c
0075e820: ldrb     r3, [sp, #0xa]
0075e824: str      r7, [sp, #0x38]
0075e828: cmp      r3, #0
0075e82c: beq      #0x75e6b4
0075e830: mov      r1, #0x41000000
0075e834: add      r1, r1, #0xa00000
0075e838: ldr      r0, [sp, #0xc]
0075e83c: bl       #0x30ed6c
0075e840: mov      r1, #0x41000000
0075e844: mov      r8, r0
0075e848: add      r1, r1, #0xa00000
0075e84c: ldr      r0, [sp, #0x14]
0075e850: bl       #0x30ed6c
0075e854: mov      r1, r6
0075e858: mov      sl, r0
0075e85c: bl       #0x30e2f8
0075e860: cmp      r0, #0
0075e864: moveq    r6, sl
0075e868: mov      r1, r6
0075e86c: mov      r0, r8
0075e870: bl       #0x30e70c
0075e874: cmp      r0, #0
0075e878: moveq    r6, r8
0075e87c: mov      r0, r6
0075e880: mvn      r1, #0x800000
0075e884: bl       #0x30e4b4
0075e888: cmp      r0, #0
0075e88c: beq      #0x75e948
0075e890: mvn      r1, #0x80000000
0075e894: mov      r0, r6
0075e898: sub      r1, r1, #0x800000
0075e89c: bl       #0x30e9ac
0075e8a0: cmp      r0, #0
0075e8a4: beq      #0x75e948
0075e8a8: mov      r1, #0x41000000
0075e8ac: add      r1, r1, #0xa00000
0075e8b0: ldr      r0, [sp, #0x10]
0075e8b4: str      r6, [sp, #0x2c]
0075e8b8: bl       #0x30ed6c
0075e8bc: mov      r1, #0x41000000
0075e8c0: mov      r6, r0
0075e8c4: add      r1, r1, #0xa00000
0075e8c8: ldr      r0, [sp, #0x18]
0075e8cc: bl       #0x30ed6c
0075e8d0: mov      r1, r7
0075e8d4: mov      r8, r0
0075e8d8: bl       #0x30e2f8
0075e8dc: cmp      r0, #0
0075e8e0: moveq    r7, r8
0075e8e4: mov      r1, r7
0075e8e8: mov      r0, r6
0075e8ec: bl       #0x30e70c
0075e8f0: cmp      r0, #0
0075e8f4: moveq    r7, r6
0075e8f8: mov      r0, r7
0075e8fc: mvn      r1, #0x800000
0075e900: bl       #0x30e4b4
0075e904: cmp      r0, #0
0075e908: beq      #0x75e93c
0075e90c: mvn      r1, #0x80000000
0075e910: mov      r0, r7
0075e914: sub      r1, r1, #0x800000
0075e918: bl       #0x30e9ac
0075e91c: cmp      r0, #0
0075e920: beq      #0x75e93c
0075e924: str      r7, [sp, #0x38]
0075e928: b        #0x75e6b4
0075e92c: mov      r7, #0
0075e930: b        #0x75e820
0075e934: mov      r6, #0
0075e938: b        #0x75e7ec
0075e93c: mov      r7, #0
0075e940: str      r7, [sp, #0x38]
0075e944: b        #0x75e6b4
0075e948: mov      r6, #0
0075e94c: b        #0x75e8a8
0075e950: ldr      r3, [r5, #0x4c]
0075e954: ldr      r0, [sp, #0x7c]
0075e958: ldr      r1, [r3, #8]
0075e95c: bl       #0x30e3ac
0075e960: str      r0, [sp, #0x1c]
0075e964: ldr      r3, [r5, #0x4c]
0075e968: ldr      r0, [sp, #0x80]
0075e96c: ldr      r1, [r3, #0x14]
0075e970: bl       #0x30e3ac
0075e974: mov      r3, #1
0075e978: str      r0, [sp, #0x20]
0075e97c: strb     r3, [sp, #8]
0075e980: mov      r0, r4
0075e984: mov      r1, r6
0075e988: ldr      r3, [r4]
0075e98c: mov      lr, pc
0075e990: ldr      pc, [r3, #0xe0]
0075e994: b        #0x75e544
0075e998: add      r0, r5, #0x3c
0075e99c: mov      r1, r6
0075e9a0: bl       #0x41fe84
0075e9a4: str      r6, [r5, #0x40]
0075e9a8: b        #0x75e784
