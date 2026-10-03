
# _ZN11Application7_UpdateEi
0032c438: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0032c43c: ldr      r4, [pc, #0x818]
0032c440: ldr      r2, [pc, #0x818]
0032c444: mov      r5, r0
0032c448: add      r4, pc, r4
0032c44c: ldr      r3, [r4, r2]
0032c450: ldr      r0, [pc, #0x80c]
0032c454: sub      sp, sp, #0x1bc
0032c458: ldr      r3, [r3]
0032c45c: add      r0, pc, r0
0032c460: str      r2, [sp, #8]
0032c464: mov      r7, r1
0032c468: str      r3, [sp, #0x1b4]
0032c46c: bl       #0x3136b4
0032c470: ldr      r3, [pc, #0x7f0]
0032c474: ldr      r6, [pc, #0x7f0]
0032c478: add      r8, sp, #0x19c
0032c47c: ldr      r0, [r4, r3]
0032c480: bl       #0x52e784
0032c484: ldr      sl, [r4, r6]
0032c488: mov      r0, sl
0032c48c: bl       #0x337888
0032c490: ldr      r1, [pc, #0x7d8]
0032c494: add      r2, sp, #0x48
0032c498: mov      r0, r8
0032c49c: add      r1, pc, r1
0032c4a0: bl       #0x3140ec
0032c4a4: mov      r0, sl
0032c4a8: mov      r1, r8
0032c4ac: mov      r2, #0
0032c4b0: bl       #0x337ddc
0032c4b4: mov      r0, r8
0032c4b8: bl       #0x3139ac
0032c4bc: ldrb     r3, [r5, #0xaa]
0032c4c0: cmp      r3, #0
0032c4c4: bne      #0x32cb5c
0032c4c8: ldr      r3, [pc, #0x7a4]
0032c4cc: ldr      r3, [r4, r3]
0032c4d0: ldrb     r3, [r3]
0032c4d4: cmp      r3, #0
0032c4d8: bne      #0x32c81c
0032c4dc: ldr      r8, [pc, #0x794]
0032c4e0: ldr      fp, [pc, #0x794]
0032c4e4: mov      r0, r5
0032c4e8: bl       #0x31f594
0032c4ec: mov      r1, #1
0032c4f0: mov      sl, r0
0032c4f4: mov      r0, r5
0032c4f8: bl       #0x321000
0032c4fc: cmp      r0, #0
0032c500: bne      #0x32c854
0032c504: ldr      r3, [pc, #0x774]
0032c508: ldr      r3, [r4, r3]
0032c50c: ldrb     r3, [r3]
0032c510: cmp      r3, #0
0032c514: beq      #0x32c878
0032c518: ldr      r3, [r4, r8]
0032c51c: ldr      r3, [r3]
0032c520: cmp      r3, #0x11
0032c524: beq      #0x32cc34
0032c528: ldr      r0, [r5, #4]
0032c52c: ldrb     r3, [r0, #8]
0032c530: cmp      r3, #0
0032c534: bne      #0x32cb74
0032c538: bl       #0x3cb2f0
0032c53c: ldr      r3, [r4, fp]
0032c540: ldr      r0, [r3, #0x40]
0032c544: bl       #0x378fb4
0032c548: mov      r0, r7
0032c54c: bl       #0x30ed30
0032c550: mov      sb, r1
0032c554: mov      r2, r0
0032c558: mov      r3, r1
0032c55c: mov      r8, r0
0032c560: ldr      r0, [r5, #0x14]
0032c564: bl       #0x33900c
0032c568: ldr      r0, [r5, #0x18]
0032c56c: mov      r3, sb
0032c570: mov      r2, r8
0032c574: bl       #0x33a7f4
0032c578: ldr      r3, [pc, #0x704]
0032c57c: ldr      r3, [r4, r3]
0032c580: ldr      r0, [r3]
0032c584: cmp      r0, #0
0032c588: beq      #0x32c590
0032c58c: bl       #0x36934c
0032c590: ldr      r0, [r5, #0x20]
0032c594: cmp      r0, #0
0032c598: beq      #0x32c5a8
0032c59c: mov      r2, r8
0032c5a0: mov      r3, sb
0032c5a4: bl       #0x33b334
0032c5a8: ldr      r0, [r5, #0x24]
0032c5ac: cmp      r0, #0
0032c5b0: beq      #0x32c5c0
0032c5b4: mov      r2, r8
0032c5b8: mov      r3, sb
0032c5bc: bl       #0x33d744
0032c5c0: bl       #0x34dda4
0032c5c4: mov      r8, r0
0032c5c8: mov      r0, r7
0032c5cc: bl       #0x30e964
0032c5d0: ldr      r7, [r8]
0032c5d4: mov      r1, r0
0032c5d8: mov      r0, r8
0032c5dc: mov      lr, pc
0032c5e0: ldr      pc, [r7, #0xc]
0032c5e4: mov      r0, r5
0032c5e8: bl       #0x321164
0032c5ec: ldr      r7, [pc, #0x694]
0032c5f0: ldr      sl, [r4, r6]
0032c5f4: add      r8, sp, #0x184
0032c5f8: add      r7, pc, r7
0032c5fc: mov      r0, sl
0032c600: bl       #0x337888
0032c604: add      r2, sp, #0x44
0032c608: mov      r1, r7
0032c60c: mov      r0, r8
0032c610: bl       #0x3140ec
0032c614: mov      r1, r8
0032c618: mov      r0, sl
0032c61c: bl       #0x337a88
0032c620: mov      sb, r0
0032c624: mov      r0, r8
0032c628: bl       #0x3139ac
0032c62c: cmp      sb, #0
0032c630: bne      #0x32cb20
0032c634: ldr      sl, [r4, r6]
0032c638: ldr      r7, [pc, #0x64c]
0032c63c: add      r8, sp, #0x154
0032c640: mov      r0, sl
0032c644: add      r7, pc, r7
0032c648: bl       #0x337888
0032c64c: add      r2, sp, #0x3c
0032c650: mov      r1, r7
0032c654: mov      r0, r8
0032c658: bl       #0x3140ec
0032c65c: mov      r1, r8
0032c660: mov      r0, sl
0032c664: bl       #0x337a88
0032c668: mov      sb, r0
0032c66c: mov      r0, r8
0032c670: bl       #0x3139ac
0032c674: cmp      sb, #0
0032c678: bne      #0x32cae0
0032c67c: ldr      sl, [r4, r6]
0032c680: ldr      r7, [pc, #0x608]
0032c684: add      r8, sp, #0x124
0032c688: mov      r0, sl
0032c68c: add      r7, pc, r7
0032c690: bl       #0x337888
0032c694: add      r2, sp, #0x34
0032c698: mov      r1, r7
0032c69c: mov      r0, r8
0032c6a0: bl       #0x3140ec
0032c6a4: mov      r1, r8
0032c6a8: mov      r0, sl
0032c6ac: bl       #0x337a88
0032c6b0: mov      sb, r0
0032c6b4: mov      r0, r8
0032c6b8: bl       #0x3139ac
0032c6bc: cmp      sb, #0
0032c6c0: bne      #0x32ca9c
0032c6c4: ldr      sl, [r4, r6]
0032c6c8: ldr      r7, [pc, #0x5c4]
0032c6cc: add      r8, sp, #0xf4
0032c6d0: mov      r0, sl
0032c6d4: add      r7, pc, r7
0032c6d8: bl       #0x337888
0032c6dc: add      r2, sp, #0x2c
0032c6e0: mov      r1, r7
0032c6e4: mov      r0, r8
0032c6e8: bl       #0x3140ec
0032c6ec: mov      r1, r8
0032c6f0: mov      r0, sl
0032c6f4: bl       #0x337a88
0032c6f8: mov      sb, r0
0032c6fc: mov      r0, r8
0032c700: bl       #0x3139ac
0032c704: cmp      sb, #0
0032c708: bne      #0x32ca44
0032c70c: ldr      sl, [r4, r6]
0032c710: ldr      r7, [pc, #0x580]
0032c714: add      r8, sp, #0xc4
0032c718: mov      r0, sl
0032c71c: add      r7, pc, r7
0032c720: bl       #0x337888
0032c724: add      r2, sp, #0x24
0032c728: mov      r1, r7
0032c72c: mov      r0, r8
0032c730: bl       #0x3140ec
0032c734: mov      r1, r8
0032c738: mov      r0, sl
0032c73c: bl       #0x337a88
0032c740: mov      sb, r0
0032c744: mov      r0, r8
0032c748: bl       #0x3139ac
0032c74c: cmp      sb, #0
0032c750: bne      #0x32c9ec
0032c754: ldr      sl, [r4, r6]
0032c758: ldr      r7, [pc, #0x53c]
0032c75c: add      r8, sp, #0x94
0032c760: mov      r0, sl
0032c764: add      r7, pc, r7
0032c768: bl       #0x337888
0032c76c: add      r2, sp, #0x1c
0032c770: mov      r1, r7
0032c774: mov      r0, r8
0032c778: bl       #0x3140ec
0032c77c: mov      r1, r8
0032c780: mov      r0, sl
0032c784: bl       #0x337a88
0032c788: mov      sb, r0
0032c78c: mov      r0, r8
0032c790: bl       #0x3139ac
0032c794: cmp      sb, #0
0032c798: bne      #0x32c99c
0032c79c: ldr      r8, [r4, r6]
0032c7a0: ldr      r6, [pc, #0x4f8]
0032c7a4: add      r7, sp, #0x64
0032c7a8: mov      r0, r8
0032c7ac: add      r6, pc, r6
0032c7b0: bl       #0x337888
0032c7b4: add      r2, sp, #0x14
0032c7b8: mov      r1, r6
0032c7bc: mov      r0, r7
0032c7c0: bl       #0x3140ec
0032c7c4: mov      r1, r7
0032c7c8: mov      r0, r8
0032c7cc: bl       #0x337a88
0032c7d0: mov      sl, r0
0032c7d4: mov      r0, r7
0032c7d8: bl       #0x3139ac
0032c7dc: cmp      sl, #0
0032c7e0: bne      #0x32c92c
0032c7e4: ldr      r3, [r5, #0x74]
0032c7e8: ldr      r0, [pc, #0x4b4]
0032c7ec: add      r3, r3, #1
0032c7f0: str      r3, [r5, #0x74]
0032c7f4: add      r0, pc, r0
0032c7f8: bl       #0x3136b8
0032c7fc: ldr      r2, [sp, #8]
0032c800: ldr      r3, [r4, r2]
0032c804: ldr      r2, [sp, #0x1b4]
0032c808: ldr      r3, [r3]
0032c80c: cmp      r2, r3
0032c810: bne      #0x32cc58
0032c814: add      sp, sp, #0x1bc
0032c818: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0032c81c: ldr      r8, [pc, #0x454]
0032c820: ldr      r3, [r4, r8]
0032c824: ldr      r3, [r3]
0032c828: cmp      r3, #0xa
0032c82c: beq      #0x32cb7c
0032c830: mov      r0, r5
0032c834: bl       #0x31f594
0032c838: mov      r1, #1
0032c83c: mov      sl, r0
0032c840: mov      r0, r5
0032c844: bl       #0x321000
0032c848: cmp      r0, #0
0032c84c: ldr      fp, [pc, #0x428]
0032c850: beq      #0x32c504
0032c854: bl       #0x7fd794
0032c858: ldrb     r3, [r0, #5]
0032c85c: cmp      r3, #0
0032c860: beq      #0x32c518
0032c864: ldr      r3, [pc, #0x414]
0032c868: ldr      r3, [r4, r3]
0032c86c: ldrb     r3, [r3]
0032c870: cmp      r3, #0
0032c874: bne      #0x32c518
0032c878: mov      r0, r5
0032c87c: bl       #0x31f5b4
0032c880: cmp      r0, #0
0032c884: beq      #0x32c518
0032c888: mov      r0, r5
0032c88c: bl       #0x31f5e8
0032c890: cmp      r0, #0
0032c894: bne      #0x32c518
0032c898: cmp      sl, #0
0032c89c: beq      #0x32c8b8
0032c8a0: ldrb     r3, [sl, #0x144]
0032c8a4: cmp      r3, #0
0032c8a8: beq      #0x32c518
0032c8ac: ldrb     r3, [sl, #0x1a8]
0032c8b0: cmp      r3, #0
0032c8b4: bne      #0x32c518
0032c8b8: ldr      r3, [r4, fp]
0032c8bc: ldr      sl, [pc, #0x3e4]
0032c8c0: mov      sb, #1
0032c8c4: ldr      r3, [r3, #0x10]
0032c8c8: add      sl, pc, sl
0032c8cc: mov      r0, sl
0032c8d0: ldr      r3, [r3, #0x1c]
0032c8d4: str      r3, [sp, #0xc]
0032c8d8: bl       #0x3136b4
0032c8dc: bl       #0x35c058
0032c8e0: ldr      r0, [sp, #0xc]
0032c8e4: bl       #0x35c51c
0032c8e8: ldr      r3, [sp, #0xc]
0032c8ec: mov      r0, r7
0032c8f0: strb     sb, [r3, #0x250]
0032c8f4: ldr      r2, [sp, #0xc]
0032c8f8: ldr      r3, [r2]
0032c8fc: str      r3, [sp, #4]
0032c900: bl       #0x30e964
0032c904: mov      r2, #0
0032c908: mov      r1, r0
0032c90c: ldr      r3, [sp, #4]
0032c910: ldr      r0, [sp, #0xc]
0032c914: mov      lr, pc
0032c918: ldr      pc, [r3, #0x60]
0032c91c: strb     sb, [r5, #0xa9]
0032c920: mov      r0, sl
0032c924: bl       #0x3136b8
0032c928: b        #0x32c518
0032c92c: add      r7, sp, #0x4c
0032c930: mov      r0, r8
0032c934: bl       #0x337888
0032c938: mov      r1, r6
0032c93c: add      r2, sp, #0x10
0032c940: mov      r0, r7
0032c944: bl       #0x3140ec
0032c948: mov      r1, r7
0032c94c: mov      r2, #0
0032c950: mov      r0, r8
0032c954: bl       #0x337ddc
0032c958: mov      r0, r7
0032c95c: bl       #0x3139ac
0032c960: mov      r0, r5
0032c964: bl       #0x31f594
0032c968: subs     r6, r0, #0
0032c96c: beq      #0x32c7e4
0032c970: ldr      r3, [r4, fp]
0032c974: ldr      r1, [pc, #0x330]
0032c978: ldr      r2, [pc, #0x330]
0032c97c: ldr      r0, [r3, #0x2c]
0032c980: add      r1, pc, r1
0032c984: add      r2, pc, r2
0032c988: bl       #0x4c4bdc
0032c98c: mov      r1, r0
0032c990: mov      r0, r6
0032c994: bl       #0x3ef728
0032c998: b        #0x32c7e4
0032c99c: add      r8, sp, #0x7c
0032c9a0: mov      r0, sl
0032c9a4: bl       #0x337888
0032c9a8: mov      r1, r7
0032c9ac: add      r2, sp, #0x18
0032c9b0: mov      r0, r8
0032c9b4: bl       #0x3140ec
0032c9b8: mov      r1, r8
0032c9bc: mov      r2, #0
0032c9c0: mov      r0, sl
0032c9c4: bl       #0x337ddc
0032c9c8: mov      r0, r8
0032c9cc: bl       #0x3139ac
0032c9d0: mov      r0, r5
0032c9d4: bl       #0x31f594
0032c9d8: cmp      r0, #0
0032c9dc: beq      #0x32c79c
0032c9e0: mvn      r1, #0
0032c9e4: bl       #0x3ef728
0032c9e8: b        #0x32c79c
0032c9ec: ldr      r3, [r4, fp]
0032c9f0: ldr      r2, [pc, #0x2bc]
0032c9f4: mov      r1, #0
0032c9f8: ldr      r3, [r3, #0x10]
0032c9fc: ldr      r2, [r4, r2]
0032ca00: add      r8, sp, #0xac
0032ca04: ldr      r3, [r3, #0x1c]
0032ca08: ldr      r0, [r3, #4]
0032ca0c: bl       #0x51073c
0032ca10: mov      r0, sl
0032ca14: bl       #0x337888
0032ca18: mov      r1, r7
0032ca1c: add      r2, sp, #0x20
0032ca20: mov      r0, r8
0032ca24: bl       #0x3140ec
0032ca28: mov      r0, sl
0032ca2c: mov      r1, r8
0032ca30: mov      r2, #0
0032ca34: bl       #0x337ddc
0032ca38: mov      r0, r8
0032ca3c: bl       #0x3139ac
0032ca40: b        #0x32c754
0032ca44: ldr      r3, [r4, fp]
0032ca48: ldr      r2, [pc, #0x268]
0032ca4c: mov      r1, #0
0032ca50: ldr      r3, [r3, #0x10]
0032ca54: ldr      r2, [r4, r2]
0032ca58: add      r8, sp, #0xdc
0032ca5c: ldr      r3, [r3, #0x1c]
0032ca60: ldr      r0, [r3, #4]
0032ca64: bl       #0x50e7c4
0032ca68: mov      r0, sl
0032ca6c: bl       #0x337888
0032ca70: mov      r1, r7
0032ca74: add      r2, sp, #0x28
0032ca78: mov      r0, r8
0032ca7c: bl       #0x3140ec
0032ca80: mov      r0, sl
0032ca84: mov      r1, r8
0032ca88: mov      r2, #0
0032ca8c: bl       #0x337ddc
0032ca90: mov      r0, r8
0032ca94: bl       #0x3139ac
0032ca98: b        #0x32c70c
0032ca9c: ldr      r3, [r4, fp]
0032caa0: add      r8, sp, #0x10c
0032caa4: ldr      r0, [r3, #0x38]
0032caa8: bl       #0x340304
0032caac: mov      r0, sl
0032cab0: bl       #0x337888
0032cab4: mov      r1, r7
0032cab8: add      r2, sp, #0x30
0032cabc: mov      r0, r8
0032cac0: bl       #0x3140ec
0032cac4: mov      r0, sl
0032cac8: mov      r1, r8
0032cacc: mov      r2, #0
0032cad0: bl       #0x337ddc
0032cad4: mov      r0, r8
0032cad8: bl       #0x3139ac
0032cadc: b        #0x32c6c4
0032cae0: bl       #0x50e2b8
0032cae4: add      r8, sp, #0x13c
0032cae8: bl       #0x50e228
0032caec: mov      r0, sl
0032caf0: bl       #0x337888
0032caf4: mov      r1, r7
0032caf8: add      r2, sp, #0x38
0032cafc: mov      r0, r8
0032cb00: bl       #0x3140ec
0032cb04: mov      r0, sl
0032cb08: mov      r1, r8
0032cb0c: mov      r2, #0
0032cb10: bl       #0x337ddc
0032cb14: mov      r0, r8
0032cb18: bl       #0x3139ac
0032cb1c: b        #0x32c67c
0032cb20: bl       #0x50e228
0032cb24: add      r8, sp, #0x16c
0032cb28: mov      r0, sl
0032cb2c: bl       #0x337888
0032cb30: mov      r1, r7
0032cb34: add      r2, sp, #0x40
0032cb38: mov      r0, r8
0032cb3c: bl       #0x3140ec
0032cb40: mov      r0, sl
0032cb44: mov      r1, r8
0032cb48: mov      r2, #0
0032cb4c: bl       #0x337ddc
0032cb50: mov      r0, r8
0032cb54: bl       #0x3139ac
0032cb58: b        #0x32c634
0032cb5c: mov      r3, #0
0032cb60: strb     r3, [r5, #0xaa]
0032cb64: mov      r0, r5
0032cb68: mov      r1, #3
0032cb6c: bl       #0x32c1f4
0032cb70: b        #0x32c4c8
0032cb74: bl       #0x317ef8
0032cb78: b        #0x32c538
0032cb7c: mov      r3, #0
0032cb80: mov      r0, #0x10
0032cb84: str      r3, [sp, #4]
0032cb88: bl       #0x310454
0032cb8c: ldr      r3, [sp, #4]
0032cb90: mov      fp, #0x42000000
0032cb94: add      fp, fp, #0xc80000
0032cb98: str      r3, [r0]
0032cb9c: str      r3, [r0, #4]
0032cba0: str      fp, [r0, #8]
0032cba4: str      fp, [r0, #0xc]
0032cba8: mov      sb, r0
0032cbac: mov      r0, #0x10
0032cbb0: str      r3, [sp, #4]
0032cbb4: bl       #0x310454
0032cbb8: ldr      r3, [sp, #4]
0032cbbc: str      fp, [r0, #8]
0032cbc0: str      fp, [r0, #4]
0032cbc4: str      r3, [r0]
0032cbc8: mov      r3, #0x43000000
0032cbcc: add      r3, r3, #0x480000
0032cbd0: str      r3, [r0, #0xc]
0032cbd4: ldr      r3, [r5, #0x20]
0032cbd8: mov      sl, r0
0032cbdc: mov      r1, sb
0032cbe0: mov      r0, r3
0032cbe4: ldr      r3, [r3]
0032cbe8: mov      lr, pc
0032cbec: ldr      pc, [r3, #0x30]
0032cbf0: cmp      r0, #0
0032cbf4: bne      #0x32cc3c
0032cbf8: ldr      r3, [r5, #0x20]
0032cbfc: mov      r1, sl
0032cc00: mov      r0, r3
0032cc04: ldr      r3, [r3]
0032cc08: mov      lr, pc
0032cc0c: ldr      pc, [r3, #0x30]
0032cc10: cmp      r0, #0
0032cc14: beq      #0x32c830
0032cc18: ldr      fp, [pc, #0x5c]
0032cc1c: ldr      r1, [pc, #0x98]
0032cc20: ldr      r3, [r4, fp]
0032cc24: add      r1, pc, r1
0032cc28: ldr      r0, [r3, #0x4c]
0032cc2c: bl       #0x46d578
0032cc30: b        #0x32c4e4
0032cc34: bl       #0x314734
0032cc38: b        #0x32c538
0032cc3c: ldr      fp, [pc, #0x38]
0032cc40: ldr      r1, [pc, #0x78]
0032cc44: ldr      r3, [r4, fp]
0032cc48: add      r1, pc, r1
0032cc4c: ldr      r0, [r3, #0x4c]
0032cc50: bl       #0x46d578
0032cc54: b        #0x32c4e4
0032cc58: bl       #0x30e310
0032cc5c: rsbeq    r8, r6, r8, asr #12
0032cc60: andeq    r4, r0, ip, lsr #1
0032cc64: subseq   r2, sb, r4, asr sp
0032cc68: andeq    r1, r0, r4, ror r4
0032cc6c: andeq    r0, r0, r4, lsl #17
0032cc70: subseq   r2, sb, ip, lsr #26
0032cc74: strheq   r3, [r0], -r0
0032cc78: andeq    r3, r0, r0, asr r8
0032cc7c: strdeq   r3, r4, [r0], -r4
0032cc80: andeq    r2, r0, r0, lsr #31
0032cc84: andeq    r0, r0, r4, lsr #27
0032cc88: subseq   r2, sb, r0, lsl ip
0032cc8c: subseq   r2, sb, r4, ror #23
0032cc90: ldrheq   r2, [sb], #-0xbc
0032cc94: subseq   r2, sb, ip, lsl #23
0032cc98: subseq   r2, sb, r4, ror #22
0032cc9c: subseq   r2, sb, r4, lsr fp
0032cca0: subseq   r2, sb, ip, lsl #22
0032cca4: ldrheq   r2, [sb], #-0x9c
0032cca8: subseq   r2, sb, r8, lsr #18
0032ccac: subseq   r2, sb, r8, asr sb
0032ccb0: subseq   r2, sb, r4, ror #18
0032ccb4: andeq    r2, r0, ip, lsl #28
0032ccb8: andeq    r1, r0, r4, lsr #13
0032ccbc: subseq   r2, sb, r4, asr #11

# _ZN11Application6UpdateEv
0032ccc4: push     {r4, r5, r6, r7, r8, sl, lr}
0032ccc8: ldr      r5, [pc, #0x2d4]
0032cccc: ldr      r6, [pc, #0x2d4]
0032ccd0: ldrb     r2, [r0, #0xa4]
0032ccd4: add      r5, pc, r5
0032ccd8: ldr      r3, [r5, r6]
0032ccdc: sub      sp, sp, #0x2c
0032cce0: cmp      r2, #0
0032cce4: ldr      r3, [r3]
0032cce8: mov      r4, r0
0032ccec: str      r3, [sp, #0x24]
0032ccf0: beq      #0x32cd10
0032ccf4: ldr      r3, [r5, r6]
0032ccf8: ldr      r2, [sp, #0x24]
0032ccfc: ldr      r3, [r3]
0032cd00: cmp      r2, r3
0032cd04: bne      #0x32cfa0
0032cd08: add      sp, sp, #0x2c
0032cd0c: pop      {r4, r5, r6, r7, r8, sl, pc}
0032cd10: bl       #0x7fd794
0032cd14: ldrb     r3, [r0, #5]
0032cd18: cmp      r3, #0
0032cd1c: bne      #0x32cea8
0032cd20: ldrb     r3, [r4, #0x7a]
0032cd24: cmp      r3, #0
0032cd28: movne    r3, #0
0032cd2c: strbne   r3, [r4, #0x7a]
0032cd30: ldr      r3, [r4, #0x10]
0032cd34: ldr      r3, [r3, #0x20]
0032cd38: mov      r0, r3
0032cd3c: ldr      r3, [r3]
0032cd40: mov      lr, pc
0032cd44: ldr      pc, [r3, #0xc]
0032cd48: ldr      r3, [r4, #0x70]
0032cd4c: mov      r7, r0
0032cd50: rsb      r3, r3, r0
0032cd54: cmp      r3, #0x7d0
0032cd58: bhi      #0x32cf00
0032cd5c: ldr      r0, [r4, #0x28]
0032cd60: cmp      r0, #0
0032cd64: beq      #0x32cd80
0032cd68: ldr      r3, [pc, #0x23c]
0032cd6c: add      r3, pc, r3
0032cd70: ldr      r2, [r3, #4]
0032cd74: rsb      r2, r2, r7
0032cd78: cmp      r2, #0xfa0
0032cd7c: bgt      #0x32ce9c
0032cd80: str      r7, [r4, #0x70]
0032cd84: bl       #0x7fd794
0032cd88: ldrb     r3, [r0, #5]
0032cd8c: cmp      r3, #0
0032cd90: bne      #0x32cec0
0032cd94: ldr      r0, [r4, #0x20]
0032cd98: bl       #0x33c568
0032cd9c: mov      r0, r4
0032cda0: bl       #0x320da4
0032cda4: ldr      r0, [r4, #0x8c]
0032cda8: bl       #0x30e2e0
0032cdac: mov      r7, r0
0032cdb0: mov      r1, r0
0032cdb4: mov      r0, #0x44000000
0032cdb8: add      r0, r0, #0x7a0000
0032cdbc: bl       #0x30ec94
0032cdc0: ldr      r3, [pc, #0x1e8]
0032cdc4: mov      r1, r7
0032cdc8: mov      sl, r0
0032cdcc: ldr      r8, [r5, r3]
0032cdd0: add      r7, sp, #0xc
0032cdd4: mov      r0, r8
0032cdd8: bl       #0x31177c
0032cddc: ldr      r1, [r4, #0x8c]
0032cde0: mov      r0, r4
0032cde4: bl       #0x32c438
0032cde8: mov      r0, r4
0032cdec: bl       #0x32ade8
0032cdf0: ldr      r1, [pc, #0x1bc]
0032cdf4: add      r2, sp, #8
0032cdf8: mov      r0, r7
0032cdfc: add      r1, pc, r1
0032ce00: bl       #0x3140ec
0032ce04: mov      ip, #0x42000000
0032ce08: mov      r3, #0
0032ce0c: add      ip, ip, #0x700000
0032ce10: mov      r2, sl
0032ce14: mov      r1, r7
0032ce18: mov      r0, r8
0032ce1c: str      ip, [sp]
0032ce20: bl       #0x312734
0032ce24: mov      r0, r7
0032ce28: bl       #0x3139ac
0032ce2c: bl       #0x7fd794
0032ce30: ldrb     r3, [r0, #5]
0032ce34: cmp      r3, #0
0032ce38: bne      #0x32ceb4
0032ce3c: bl       #0x38174c
0032ce40: cmp      r0, #0
0032ce44: beq      #0x32ccf4
0032ce48: ldr      r3, [pc, #0x168]
0032ce4c: add      r3, pc, r3
0032ce50: ldr      r2, [r3, #8]
0032ce54: ldr      r0, [r3, #0xc]
0032ce58: add      r2, r2, #1
0032ce5c: str      r2, [r3, #8]
0032ce60: ldr      r1, [r4, #0x8c]
0032ce64: cmp      r2, #0xa
0032ce68: add      r2, r0, r1
0032ce6c: str      r2, [r3, #0xc]
0032ce70: beq      #0x32cf10
0032ce74: ldr      r1, [r3, #0x10]
0032ce78: cmp      r1, #0
0032ce7c: ble      #0x32ccf4
0032ce80: ldr      r3, [r4, #0x10]
0032ce84: mov      r2, #0
0032ce88: mov      r0, r3
0032ce8c: ldr      r3, [r3]
0032ce90: mov      lr, pc
0032ce94: ldr      pc, [r3, #0x10]
0032ce98: b        #0x32ccf4
0032ce9c: str      r7, [r3, #4]
0032cea0: bl       #0x339100
0032cea4: b        #0x32cd80
0032cea8: bl       #0x7fd794
0032ceac: bl       #0x7fd914
0032ceb0: b        #0x32cd20
0032ceb4: bl       #0x7fd794
0032ceb8: bl       #0x7fd634
0032cebc: b        #0x32ce3c
0032cec0: ldr      r7, [pc, #0xf4]
0032cec4: add      r7, pc, r7
0032cec8: mov      r0, r7
0032cecc: bl       #0x3136b4
0032ced0: bl       #0x7fd794
0032ced4: mov      r8, r0
0032ced8: ldr      r0, [r4, #0x8c]
0032cedc: bl       #0x30e2e0
0032cee0: mov      r1, r0
0032cee4: mov      r0, r8
0032cee8: bl       #0x824f34
0032ceec: bl       #0x320e98
0032cef0: bl       #0x4a039c
0032cef4: mov      r0, r7
0032cef8: bl       #0x3136b8
0032cefc: b        #0x32cd94
0032cf00: str      r0, [r4, #0x70]
0032cf04: mov      r0, r4
0032cf08: bl       #0x320da4
0032cf0c: b        #0x32ccf4
0032cf10: movw     r1, #0x6667
0032cf14: movt     r1, #0x6666
0032cf18: smull    r0, r1, r1, r2
0032cf1c: ldr      r0, [r3, #0x10]
0032cf20: asr      r2, r2, #0x1f
0032cf24: rsb      r1, r2, r1, asr #2
0032cf28: rsb      r1, r0, r1
0032cf2c: cmp      r1, #0xf
0032cf30: rsble    r1, r1, #0x10
0032cf34: strle    r1, [r3, #0x10]
0032cf38: ble      #0x32cf74
0032cf3c: cmp      r1, #0x20
0032cf40: rsble    r1, r1, #0x21
0032cf44: strle    r1, [r3, #0x10]
0032cf48: ble      #0x32cf74
0032cf4c: cmp      r1, #0x31
0032cf50: rsble    r1, r1, #0x32
0032cf54: strle    r1, [r3, #0x10]
0032cf58: ble      #0x32cf74
0032cf5c: mov      r2, #0
0032cf60: str      r2, [r3, #0xc]
0032cf64: str      r2, [r3, #0x10]
0032cf68: str      r2, [r3, #8]
0032cf6c: mov      r1, #5
0032cf70: b        #0x32cf90
0032cf74: ldr      r3, [pc, #0x44]
0032cf78: mov      r2, #0
0032cf7c: cmp      r1, #4
0032cf80: add      r3, pc, r3
0032cf84: str      r2, [r3, #0xc]
0032cf88: str      r2, [r3, #8]
0032cf8c: ble      #0x32cf6c
0032cf90: ldr      r3, [pc, #0x2c]
0032cf94: add      r3, pc, r3
0032cf98: str      r1, [r3, #0x10]
0032cf9c: b        #0x32ce80
0032cfa0: bl       #0x30e310
0032cfa4: strhteq  r7, [r6], #-0xdc
0032cfa8: andeq    r4, r0, ip, lsr #1
0032cfac: rsbeq    r2, r7, r8, lsl #25
0032cfb0: strdeq   r0, r1, [r0], -ip
0032cfb4: subseq   r2, sb, r4, lsl #10
0032cfb8: rsbeq    r2, r7, r8, lsr #23
0032cfbc: subseq   r2, sb, ip, lsr #8
0032cfc0: rsbeq    r2, r7, r4, ror sl
0032cfc4: rsbeq    r2, r7, r0, ror #20
