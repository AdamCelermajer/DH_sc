
# _ZN15bitmap_info_oglC1EPN6glitch5video12IVideoDriverEiiPh
007d5404: push     {r4, r5, r6, r7, r8, sl, lr}
007d5408: ldr      r6, [pc, #0x104]
007d540c: sub      sp, sp, #0x14
007d5410: mov      r4, r0
007d5414: mov      r7, r2
007d5418: mov      r8, r3
007d541c: mov      sl, r1
007d5420: ldr      r5, [sp, #0x30]
007d5424: bl       #0x759c04
007d5428: ldr      r2, [pc, #0xe8]
007d542c: add      r6, pc, r6
007d5430: mov      r3, #0
007d5434: ldr      r2, [r6, r2]
007d5438: mov      r1, #1
007d543c: str      r3, [r4, #0x1c]
007d5440: add      r2, r2, #8
007d5444: str      r2, [r4]
007d5448: strb     r3, [r4, #0xc]
007d544c: strb     r3, [r4, #0xd]
007d5450: str      r3, [r4, #0x10]
007d5454: str      r3, [r4, #0x14]
007d5458: str      r3, [r4, #0x18]
007d545c: str      r1, [r4, #0x30]
007d5460: str      r7, [r4, #0x20]
007d5464: str      r8, [r4, #0x24]
007d5468: str      sl, [r4, #0x28]
007d546c: str      r1, [r4, #0x2c]
007d5470: mov      r2, #0xc
007d5474: ldr      r1, [sl, #0xe0]
007d5478: add      r0, sp, #0xc
007d547c: add      r3, sp, #4
007d5480: stmib    sp, {r7, r8}
007d5484: bl       #0x5e8788
007d5488: ldr      r3, [sp, #0xc]
007d548c: cmp      r3, #0
007d5490: ldrne    r2, [r3, #4]
007d5494: addne    r2, r2, #1
007d5498: strne    r2, [r3, #4]
007d549c: ldr      r0, [r4, #0x18]
007d54a0: str      r3, [r4, #0x18]
007d54a4: cmp      r0, #0
007d54a8: beq      #0x7d54b0
007d54ac: bl       #0x31d584
007d54b0: ldr      r0, [sp, #0xc]
007d54b4: cmp      r0, #0
007d54b8: beq      #0x7d54c0
007d54bc: bl       #0x31d584
007d54c0: cmp      r5, #0
007d54c4: beq      #0x7d5508
007d54c8: mul      r7, r7, r8
007d54cc: ldr      r3, [r4, #0x18]
007d54d0: cmp      r7, #0
007d54d4: ldr      r3, [r3, #8]
007d54d8: ble      #0x7d5508
007d54dc: mov      r2, #0
007d54e0: mvn      r1, #0
007d54e4: ldrb     r0, [r5, r2]
007d54e8: add      r2, r2, #1
007d54ec: cmp      r2, r7
007d54f0: strb     r0, [r3]
007d54f4: strb     r1, [r3, #1]
007d54f8: strb     r1, [r3, #2]
007d54fc: strb     r1, [r3, #3]
007d5500: add      r3, r3, #4
007d5504: bne      #0x7d54e4
007d5508: mov      r0, r4
007d550c: add      sp, sp, #0x14
007d5510: pop      {r4, r5, r6, r7, r8, sl, pc}
007d5514: andseq   pc, fp, r4, ror #12
007d5518: andeq    r0, r0, r4, ror #11

# _ZNK6glitch2io11CFileSystem15getFileBasenameERKSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEEb
0056c740: push     {r4, r5, r6, r7, lr}
0056c744: mov      r1, #0x2f
0056c748: sub      sp, sp, #0x14
0056c74c: mov      r5, r0
0056c750: mov      r0, r2
0056c754: mov      r4, r2
0056c758: mov      r7, r3
0056c75c: bl       #0x56c640
0056c760: mov      r1, #0x5c
0056c764: mov      r6, r0
0056c768: mov      r0, r4
0056c76c: bl       #0x56c640
0056c770: cmp      r0, r6
0056c774: movge    r6, r0
0056c778: movlt    r6, r6
0056c77c: cmp      r7, #0
0056c780: beq      #0x56c7c8
0056c784: ldr      r3, [r4, #0x14]
0056c788: ldr      r2, [r4, #0x10]
0056c78c: mov      r7, #0
0056c790: rsb      r2, r3, r2
0056c794: cmp      r6, r2
0056c798: blo      #0x56c7fc
0056c79c: cmp      r7, #0
0056c7a0: bne      #0x56c824
0056c7a4: str      r5, [r5, #0x10]
0056c7a8: str      r5, [r5, #0x14]
0056c7ac: ldr      r2, [r4, #0x10]
0056c7b0: mov      r0, r5
0056c7b4: ldr      r1, [r4, #0x14]
0056c7b8: bl       #0x325ff4
0056c7bc: mov      r0, r5
0056c7c0: add      sp, sp, #0x14
0056c7c4: pop      {r4, r5, r6, r7, pc}
0056c7c8: mov      r0, r4
0056c7cc: mov      r1, #0x2e
0056c7d0: bl       #0x56c640
0056c7d4: cmn      r0, #1
0056c7d8: ldrne    r2, [r4, #0x10]
0056c7dc: ldreq    r3, [r4, #0x14]
0056c7e0: ldreq    r2, [r4, #0x10]
0056c7e4: ldrne    r3, [r4, #0x14]
0056c7e8: rsbeq    r2, r3, r2
0056c7ec: rsbne    r2, r3, r2
0056c7f0: rsbne    r7, r0, r2
0056c7f4: cmp      r6, r2
0056c7f8: bhs      #0x56c79c
0056c7fc: mvn      r3, r6
0056c800: add      r2, r3, r2
0056c804: rsb      r3, r7, r2
0056c808: add      ip, sp, #0xc
0056c80c: mov      r1, r4
0056c810: add      r2, r6, #1
0056c814: mov      r0, r5
0056c818: str      ip, [sp]
0056c81c: bl       #0x56c4c4
0056c820: b        #0x56c7bc
0056c824: rsb      r3, r7, r2
0056c828: add      ip, sp, #8
0056c82c: mov      r1, r4
0056c830: mov      r0, r5
0056c834: mov      r2, #0
0056c838: str      ip, [sp]
0056c83c: bl       #0x56c4c4
0056c840: b        #0x56c7bc

# _ZN6glitch2io10CZipReader22deletePathFromFilenameERSbIcSt11char_traitsIcENS_4core10SAllocatorIcLNS_6memory13E_MEMORY_HINTE0EEEE
00577b50: push     {r4, r5, r6, lr}
00577b54: ldr      r2, [r1, #0x14]
00577b58: ldr      r5, [r1, #0x10]
00577b5c: mov      r4, r1
00577b60: rsb      r5, r2, r5
00577b64: ldrsb    r3, [r2, r5]
00577b68: add      r5, r2, r5
00577b6c: cmp      r3, #0x2f
00577b70: cmpne    r3, #0x5c
00577b74: bne      #0x577ba0
00577b78: cmp      r5, r2
00577b7c: beq      #0x577bc0
00577b80: add      r5, r5, #1
00577b84: mov      r0, r5
00577b88: bl       #0x30de54
00577b8c: mov      r1, r5
00577b90: add      r2, r5, r0
00577b94: mov      r0, r4
00577b98: pop      {r4, r5, r6, lr}
00577b9c: b        #0x320b88
00577ba0: cmp      r5, r2
00577ba4: beq      #0x577bc4
00577ba8: ldrsb    r3, [r5, #-1]!
00577bac: cmp      r3, #0x5c
00577bb0: cmpne    r3, #0x2f
00577bb4: beq      #0x577b78
00577bb8: cmp      r5, r2
00577bbc: bne      #0x577ba8
00577bc0: pop      {r4, r5, r6, pc}
00577bc4: pop      {r4, r5, r6, pc}

# _ZN6glitch2io10CZipReaderC1EPNS0_9IReadFileEbb
00577fa4: ldr      ip, [pc, #0x9c]
00577fa8: push     {r4, r5, r6, lr}
00577fac: ldr      r5, [pc, #0x98]
00577fb0: add      ip, pc, ip
00577fb4: mov      r4, r0
00577fb8: ldr      r5, [ip, r5]
00577fbc: mov      r0, #0
00577fc0: mov      r6, #1
00577fc4: add      r5, r5, #8
00577fc8: cmp      r1, #0
00577fcc: stm      r4, {r5, r6}
00577fd0: strb     r2, [r4, #0xc]
00577fd4: strb     r3, [r4, #0xd]
00577fd8: str      r0, [r4, #0x1c]
00577fdc: str      r1, [r4, #8]
00577fe0: str      r0, [r4, #0x10]
00577fe4: str      r0, [r4, #0x14]
00577fe8: str      r0, [r4, #0x18]
00577fec: beq      #0x578040
00577ff0: ldr      r3, [r1, #4]
00577ff4: add      r3, r3, r6
00577ff8: str      r3, [r1, #4]
00577ffc: mov      r0, r4
00578000: bl       #0x577db4
00578004: cmp      r0, #0
00578008: bne      #0x577ffc
0057800c: ldr      r0, [r4, #0x14]
00578010: ldr      r3, [r4, #0x18]
00578014: rsb      r3, r0, r3
00578018: asr      r3, r3, #2
0057801c: add      r1, r3, r3, lsl #3
00578020: add      r3, r3, r1, lsl #1
00578024: lsl      r1, r3, #9
00578028: rsb      r1, r3, r1
0057802c: add      r1, r1, r1, lsl #18
00578030: rsb      r1, r1, #0
00578034: cmp      r1, #1
00578038: bls      #0x578040
0057803c: bl       #0x5777ac
00578040: mov      r0, r4
00578044: pop      {r4, r5, r6, pc}
00578048: subeq    ip, r1, r0, ror #21
0057804c: strheq   r2, [r0], -r0

# _ZN21render_handler_glitch19draw_mesh_primitiveEiPKviPKti
007d94f8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d94fc: ldrb     ip, [r0, #0x3b7]
007d9500: sub      sp, sp, #0x64
007d9504: mov      r4, r0
007d9508: cmp      ip, #0
007d950c: str      r1, [sp, #0x10]
007d9510: mov      r5, r2
007d9514: str      r3, [sp, #0xc]
007d9518: bne      #0x7d9524
007d951c: add      sp, sp, #0x64
007d9520: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d9524: add      r1, r3, #2
007d9528: bl       #0x7d4340
007d952c: ldr      r1, [sp, #0xc]
007d9530: mov      fp, #0x18
007d9534: ldr      r2, [r4, #0x374]
007d9538: mul      fp, fp, r1
007d953c: add      r3, r2, fp
007d9540: cmp      r2, r3
007d9544: beq      #0x7d9574
007d9548: ldr      ip, [r5]
007d954c: ldr      r0, [r5, #4]
007d9550: ldr      r1, [r4, #0x348]
007d9554: str      ip, [r2, #0xc]
007d9558: str      r0, [r2, #0x10]
007d955c: str      r1, [r2, #0x14]
007d9560: add      r2, r2, #0x18
007d9564: cmp      r3, r2
007d9568: add      r5, r5, #8
007d956c: bne      #0x7d9548
007d9570: ldr      r3, [r4, #0x374]
007d9574: ldr      ip, [sp, #0xc]
007d9578: ldr      r1, [r4, #0x10]
007d957c: add      r2, r4, #0x1f0
007d9580: add      r0, r4, #0x3b0
007d9584: str      r2, [sp, #0x14]
007d9588: str      ip, [sp]
007d958c: bl       #0x7d6b54
007d9590: add      r0, sp, #0x1c
007d9594: mov      r1, r4
007d9598: add      r2, r4, #0x30c
007d959c: bl       #0x7d6680
007d95a0: ldr      r5, [r4, #0x374]
007d95a4: add      fp, r5, fp
007d95a8: cmp      r5, fp
007d95ac: beq      #0x7d96c0
007d95b0: ldr      r8, [r5, #0xc]
007d95b4: ldr      r1, [sp, #0x20]
007d95b8: ldr      r7, [r5, #0x10]
007d95bc: mov      r0, r8
007d95c0: bl       #0x30ed6c
007d95c4: ldr      r1, [sp, #0x30]
007d95c8: mov      sl, r0
007d95cc: mov      r0, r7
007d95d0: bl       #0x30ed6c
007d95d4: mov      r1, r0
007d95d8: mov      r0, sl
007d95dc: bl       #0x30eba4
007d95e0: ldr      r6, [r5, #0x14]
007d95e4: mov      sl, r0
007d95e8: ldr      r1, [sp, #0x40]
007d95ec: mov      r0, r6
007d95f0: bl       #0x30ed6c
007d95f4: mov      r1, r0
007d95f8: mov      r0, sl
007d95fc: bl       #0x30eba4
007d9600: ldr      r1, [sp, #0x50]
007d9604: bl       #0x30eba4
007d9608: ldr      r1, [sp, #0x24]
007d960c: mov      sb, r0
007d9610: mov      r0, r8
007d9614: bl       #0x30ed6c
007d9618: ldr      r1, [sp, #0x34]
007d961c: mov      sl, r0
007d9620: mov      r0, r7
007d9624: bl       #0x30ed6c
007d9628: mov      r1, r0
007d962c: mov      r0, sl
007d9630: bl       #0x30eba4
007d9634: ldr      r1, [sp, #0x44]
007d9638: mov      sl, r0
007d963c: mov      r0, r6
007d9640: bl       #0x30ed6c
007d9644: mov      r1, r0
007d9648: mov      r0, sl
007d964c: bl       #0x30eba4
007d9650: ldr      r1, [sp, #0x54]
007d9654: bl       #0x30eba4
007d9658: ldr      r1, [sp, #0x1c]
007d965c: mov      sl, r0
007d9660: mov      r0, r8
007d9664: bl       #0x30ed6c
007d9668: ldr      r1, [sp, #0x2c]
007d966c: mov      r8, r0
007d9670: mov      r0, r7
007d9674: bl       #0x30ed6c
007d9678: mov      r1, r0
007d967c: mov      r0, r8
007d9680: bl       #0x30eba4
007d9684: ldr      r1, [sp, #0x3c]
007d9688: mov      r7, r0
007d968c: mov      r0, r6
007d9690: bl       #0x30ed6c
007d9694: mov      r1, r0
007d9698: mov      r0, r7
007d969c: bl       #0x30eba4
007d96a0: ldr      r1, [sp, #0x4c]
007d96a4: bl       #0x30eba4
007d96a8: str      r0, [r5, #0xc]
007d96ac: str      sb, [r5, #0x10]
007d96b0: str      sl, [r5, #0x14]
007d96b4: add      r5, r5, #0x18
007d96b8: cmp      fp, r5
007d96bc: bne      #0x7d95b0
007d96c0: ldr      r3, [r4, #0x378]
007d96c4: ldr      r2, [sp, #0xc]
007d96c8: mov      r0, r4
007d96cc: str      r2, [r3, #8]
007d96d0: ldr      ip, [sp, #0x8c]
007d96d4: ldr      r1, [r4, #0x374]
007d96d8: ldr      r3, [sp, #0x88]
007d96dc: str      ip, [sp]
007d96e0: ldr      ip, [sp, #0x10]
007d96e4: str      ip, [sp, #4]
007d96e8: bl       #0x7d860c
007d96ec: cmp      r0, #0
007d96f0: bne      #0x7d951c
007d96f4: ldr      r1, [sp, #0x8c]
007d96f8: ldr      r2, [sp, #0x88]
007d96fc: cmp      r2, #0
007d9700: cmpne    r1, #0
007d9704: beq      #0x7d9720
007d9708: ldr      r0, [sp, #0x14]
007d970c: add      r1, r4, #0x378
007d9710: ldr      r2, [sp, #0x88]
007d9714: ldr      r3, [sp, #0x8c]
007d9718: bl       #0x7d7094
007d971c: b        #0x7d951c
007d9720: ldr      r0, [sp, #0x14]
007d9724: add      r1, r4, #0x378
007d9728: ldr      r2, [sp, #0x10]
007d972c: bl       #0x7d6ea0
007d9730: b        #0x7d951c

# _ZN16BufferedRenderer5flushEv
007d6894: push     {r4, r5, r6, lr}
007d6898: ldr      r3, [r0, #0x10]
007d689c: sub      sp, sp, #0x28
007d68a0: mov      r4, r0
007d68a4: ldr      r3, [r3, #8]
007d68a8: cmp      r3, #0
007d68ac: bne      #0x7d68b8
007d68b0: add      sp, sp, #0x28
007d68b4: pop      {r4, r5, r6, pc}
007d68b8: ldr      r0, [r0, #0x38]
007d68bc: bl       #0x7935ec
007d68c0: ldr      r3, [r4, #0x10]
007d68c4: ldr      r2, [r4, #8]
007d68c8: ldr      r6, [r3, #8]
007d68cc: rsb      r2, r2, r6
007d68d0: str      r2, [r3, #8]
007d68d4: ldr      r0, [r4, #0x10]
007d68d8: ldr      r1, [r4, #8]
007d68dc: bl       #0x5a0b34
007d68e0: ldrb     r3, [r4, #4]
007d68e4: cmp      r3, #0
007d68e8: bne      #0x7d6a30
007d68ec: ldr      r3, [r4, #0x10c]
007d68f0: mov      r5, #0xc
007d68f4: mul      r5, r5, r3
007d68f8: add      r3, r4, r5
007d68fc: ldr      r3, [r3, #0x40]
007d6900: cmp      r3, #0
007d6904: addne    r5, r4, r5
007d6908: addeq    r5, r4, #0x3c
007d690c: addne    r5, r5, #0x3c
007d6910: mov      r2, #0
007d6914: add      r3, r4, #0x108
007d6918: ldr      r0, [r5, #4]
007d691c: ldrh     r1, [r5, #8]
007d6920: bl       #0x5cd324
007d6924: ldrh     r2, [r5, #0xa]
007d6928: movw     r3, #0xffff
007d692c: cmp      r2, r3
007d6930: beq      #0x7d698c
007d6934: ldr      r1, [r4, #0x108]
007d6938: cmp      r1, #0
007d693c: beq      #0x7d698c
007d6940: mov      r3, #0
007d6944: mov      r2, #0x3f800000
007d6948: str      r2, [sp, #0x1c]
007d694c: str      r2, [sp, #0x14]
007d6950: str      r2, [sp, #0x18]
007d6954: str      r3, [sp, #0x10]
007d6958: str      r3, [sp, #0x20]
007d695c: str      r3, [sp, #4]
007d6960: str      r3, [sp, #8]
007d6964: str      r3, [sp, #0xc]
007d6968: ldr      r3, [r1, #0x38]
007d696c: ldr      r0, [r5, #4]
007d6970: ldrh     r1, [r5, #0xa]
007d6974: ubfx     r3, r3, #4, #6
007d6978: cmp      r3, #2
007d697c: addne    r3, sp, #4
007d6980: addeq    r3, sp, #0x14
007d6984: mov      r2, #0
007d6988: bl       #0x5cb6e8
007d698c: add      r1, r5, #4
007d6990: ldr      r0, [r4, #0x34]
007d6994: bl       #0x7d4ef8
007d6998: ldr      r3, [r4, #0x10]
007d699c: ldr      r0, [r4, #0x34]
007d69a0: add      r1, sp, #0x24
007d69a4: cmp      r3, #0
007d69a8: str      r3, [sp, #0x24]
007d69ac: ldrne    r2, [r3]
007d69b0: addne    r2, r2, #1
007d69b4: strne    r2, [r3]
007d69b8: add      r2, r4, #0x14
007d69bc: bl       #0x7d4ebc
007d69c0: ldr      r5, [sp, #0x24]
007d69c4: cmp      r5, #0
007d69c8: beq      #0x7d69e0
007d69cc: ldr      r3, [r5]
007d69d0: sub      r3, r3, #1
007d69d4: cmp      r3, #0
007d69d8: str      r3, [r5]
007d69dc: beq      #0x7d6a1c
007d69e0: ldr      r1, [r4, #8]
007d69e4: ldr      r0, [r4, #0x10]
007d69e8: rsb      r1, r1, #0
007d69ec: bl       #0x5a0b34
007d69f0: ldr      r3, [r4]
007d69f4: ldr      r2, [r4, #0x10]
007d69f8: cmp      r3, #1
007d69fc: movne    r6, #0
007d6a00: mov      r3, #0
007d6a04: streq    r6, [r4, #8]
007d6a08: str      r6, [r2, #8]
007d6a0c: str      r3, [r4, #0x24]
007d6a10: str      r3, [r4, #0x1c]
007d6a14: str      r3, [r4, #0x20]
007d6a18: b        #0x7d68b0
007d6a1c: mov      r0, r5
007d6a20: bl       #0x5a0a1c
007d6a24: mov      r0, r5
007d6a28: bl       #0x30e2b0
007d6a2c: b        #0x7d69e0
007d6a30: mov      r0, r4
007d6a34: bl       #0x7d673c
007d6a38: mov      r5, r0
007d6a3c: b        #0x7d6910

# _ZN14FileSystemBase17createAndOpenFileEPKc
0034e688: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034e68c: ldr      r4, [pc, #0x2b0]
0034e690: ldr      sb, [pc, #0x2b0]
0034e694: sub      sp, sp, #0xa4
0034e698: add      r4, pc, r4
0034e69c: ldr      r3, [r4, sb]
0034e6a0: mov      r5, r0
0034e6a4: add      sl, sp, #0x84
0034e6a8: ldr      r3, [r3]
0034e6ac: mov      r2, r1
0034e6b0: add      r6, sp, #0x6c
0034e6b4: str      r3, [sp, #0x9c]
0034e6b8: ldr      r3, [r5]
0034e6bc: mov      r0, sl
0034e6c0: mov      r1, r5
0034e6c4: mov      lr, pc
0034e6c8: ldr      pc, [r3, #0xb8]
0034e6cc: mov      r0, r6
0034e6d0: mov      r1, #0x10
0034e6d4: str      r6, [sp, #0x7c]
0034e6d8: str      r6, [sp, #0x80]
0034e6dc: bl       #0x3209a8
0034e6e0: ldr      r3, [sp, #0x7c]
0034e6e4: mov      r2, #0
0034e6e8: add      r7, sp, #0x54
0034e6ec: strb     r2, [r3]
0034e6f0: ldr      r3, [sp, #0x98]
0034e6f4: ldr      r2, [sp, #0x94]
0034e6f8: mov      r0, r7
0034e6fc: mov      r1, r3
0034e700: rsb      r3, r3, r2
0034e704: cmp      r3, #8
0034e708: addls    r2, r1, r3
0034e70c: addhi    r2, r1, #8
0034e710: str      r7, [sp, #0x64]
0034e714: str      r7, [sp, #0x68]
0034e718: bl       #0x325ff4
0034e71c: ldr      r8, [sp, #0x98]
0034e720: ldr      r1, [pc, #0x224]
0034e724: mov      r0, r8
0034e728: add      r1, pc, r1
0034e72c: bl       #0x30ebd4
0034e730: cmp      r0, #0
0034e734: beq      #0x34e8a0
0034e738: mov      r1, r8
0034e73c: mov      r0, r6
0034e740: ldr      r2, [sp, #0x94]
0034e744: bl       #0x320b88
0034e748: add      r8, sp, #0x24
0034e74c: mov      r0, r8
0034e750: ldr      r1, [sp, #0x80]
0034e754: ldr      r2, [sp, #0x7c]
0034e758: str      r8, [sp, #0x34]
0034e75c: str      r8, [sp, #0x38]
0034e760: bl       #0x325ff4
0034e764: ldr      r3, [pc, #0x1e4]
0034e768: ldr      r1, [sp, #0x38]
0034e76c: ldr      r0, [r4, r3]
0034e770: bl       #0x320678
0034e774: cmp      r0, #0
0034e778: beq      #0x34e808
0034e77c: ldr      r3, [r5]
0034e780: mov      r0, r5
0034e784: mov      lr, pc
0034e788: ldr      pc, [r3, #0x2c]
0034e78c: add      fp, sp, #0xc
0034e790: str      fp, [sp, #0x1c]
0034e794: str      fp, [sp, #0x20]
0034e798: str      r0, [sp]
0034e79c: bl       #0x30de54
0034e7a0: ldr      r1, [sp]
0034e7a4: add      r2, r1, r0
0034e7a8: mov      r0, fp
0034e7ac: bl       #0x3116e8
0034e7b0: ldr      r1, [pc, #0x19c]
0034e7b4: ldr      r3, [r5]
0034e7b8: mov      r0, r5
0034e7bc: add      r1, pc, r1
0034e7c0: mov      lr, pc
0034e7c4: ldr      pc, [r3, #0x30]
0034e7c8: mov      r0, r5
0034e7cc: ldr      r1, [sp, #0x38]
0034e7d0: bl       #0x56d4e0
0034e7d4: cmp      r0, #0
0034e7d8: str      r0, [sp, #4]
0034e7dc: beq      #0x34e7ec
0034e7e0: mov      r0, fp
0034e7e4: bl       #0x3139ac
0034e7e8: b        #0x34e820
0034e7ec: ldr      r3, [r5]
0034e7f0: mov      r0, r5
0034e7f4: ldr      r1, [sp, #0x20]
0034e7f8: mov      lr, pc
0034e7fc: ldr      pc, [r3, #0x30]
0034e800: mov      r0, fp
0034e804: bl       #0x3139ac
0034e808: mov      r0, r5
0034e80c: ldr      r1, [sp, #0x38]
0034e810: bl       #0x56d4e0
0034e814: cmp      r0, #0
0034e818: str      r0, [sp, #4]
0034e81c: beq      #0x34e918
0034e820: ldr      r0, [sp, #0x38]
0034e824: cmp      r0, r8
0034e828: beq      #0x34e838
0034e82c: cmp      r0, #0
0034e830: beq      #0x34e838
0034e834: bl       #0x310450
0034e838: ldr      r0, [sp, #0x68]
0034e83c: cmp      r0, r7
0034e840: beq      #0x34e850
0034e844: cmp      r0, #0
0034e848: beq      #0x34e850
0034e84c: bl       #0x310450
0034e850: ldr      r0, [sp, #0x80]
0034e854: cmp      r0, r6
0034e858: beq      #0x34e868
0034e85c: cmp      r0, #0
0034e860: beq      #0x34e868
0034e864: bl       #0x310450
0034e868: ldr      r0, [sp, #0x98]
0034e86c: cmp      r0, sl
0034e870: beq      #0x34e880
0034e874: cmp      r0, #0
0034e878: beq      #0x34e880
0034e87c: bl       #0x310450
0034e880: ldr      r3, [r4, sb]
0034e884: ldr      r2, [sp, #0x9c]
0034e888: ldr      r0, [sp, #4]
0034e88c: ldr      r3, [r3]
0034e890: cmp      r2, r3
0034e894: bne      #0x34e940
0034e898: add      sp, sp, #0xa4
0034e89c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034e8a0: ldr      r1, [pc, #0xb0]
0034e8a4: mov      r0, r8
0034e8a8: add      r1, pc, r1
0034e8ac: bl       #0x30ebd4
0034e8b0: cmp      r0, #0
0034e8b4: bne      #0x34e738
0034e8b8: ldr      r1, [pc, #0x9c]
0034e8bc: ldr      r0, [sp, #0x68]
0034e8c0: add      r1, pc, r1
0034e8c4: bl       #0x30e31c
0034e8c8: cmp      r0, #0
0034e8cc: beq      #0x34e738
0034e8d0: ldr      r3, [pc, #0x88]
0034e8d4: add      r8, sp, #0x3c
0034e8d8: mov      r2, sl
0034e8dc: ldr      r3, [r4, r3]
0034e8e0: mov      r0, r8
0034e8e4: ldr      r1, [r3]
0034e8e8: bl       #0x34e394
0034e8ec: mov      r0, r6
0034e8f0: ldr      r1, [sp, #0x50]
0034e8f4: ldr      r2, [sp, #0x4c]
0034e8f8: bl       #0x320b88
0034e8fc: ldr      r0, [sp, #0x50]
0034e900: cmp      r0, r8
0034e904: beq      #0x34e748
0034e908: cmp      r0, #0
0034e90c: beq      #0x34e748
0034e910: bl       #0x310450
0034e914: b        #0x34e748
0034e918: ldr      r0, [pc, #0x44]
0034e91c: ldr      r1, [sp, #0x38]
0034e920: add      r0, pc, r0
0034e924: bl       #0x324114
0034e928: ldr      r0, [pc, #0x38]
0034e92c: ldr      r1, [sp, #0x38]
0034e930: mov      r2, #3
0034e934: add      r0, pc, r0
0034e938: bl       #0x60ace8
0034e93c: b        #0x34e820
0034e940: bl       #0x30e310

# _ZN21render_handler_glitch15draw_mesh_stripEPKvi
007d9764: str      lr, [sp, #-4]!
007d9768: mov      ip, #0
007d976c: sub      sp, sp, #0xc
007d9770: mov      r3, r2
007d9774: mov      r2, r1
007d9778: mov      r1, #4
007d977c: str      ip, [sp, #4]
007d9780: str      ip, [sp]
007d9784: bl       #0x7d94f8
007d9788: add      sp, sp, #0xc
007d978c: ldm      sp!, {pc}

# _ZN6glitch2io11CFileSystem29createAndOpenFileFromArchivesEPKc
0056c0f8: push     {r4, r5, r6, lr}
0056c0fc: ldr      r3, [r0, #8]
0056c100: ldr      r2, [r0, #0xc]
0056c104: mov      r5, r0
0056c108: mov      r6, r1
0056c10c: rsb      r2, r3, r2
0056c110: lsrs     r2, r2, #2
0056c114: beq      #0x56c15c
0056c118: mov      r4, #0
0056c11c: b        #0x56c134
0056c120: ldr      r3, [r5, #8]
0056c124: ldr      r2, [r5, #0xc]
0056c128: rsb      r2, r3, r2
0056c12c: cmp      r4, r2, asr #2
0056c130: bhs      #0x56c15c
0056c134: ldr      r3, [r3, r4, lsl #2]
0056c138: mov      r1, r6
0056c13c: add      r4, r4, #1
0056c140: mov      r0, r3
0056c144: ldr      r3, [r3]
0056c148: mov      lr, pc
0056c14c: ldr      pc, [r3, #0xc]
0056c150: cmp      r0, #0
0056c154: beq      #0x56c120
0056c158: pop      {r4, r5, r6, pc}
0056c15c: ldr      r3, [r5, #0x14]
0056c160: ldr      r2, [r5, #0x18]
0056c164: rsb      r2, r3, r2
0056c168: lsrs     r2, r2, #2
0056c16c: beq      #0x56c1b4
0056c170: mov      r4, #0
0056c174: b        #0x56c18c
0056c178: ldr      r3, [r5, #0x14]
0056c17c: ldr      r2, [r5, #0x18]
0056c180: rsb      r2, r3, r2
0056c184: cmp      r4, r2, asr #2
0056c188: bhs      #0x56c1b4
0056c18c: ldr      r3, [r3, r4, lsl #2]
0056c190: mov      r1, r6
0056c194: add      r4, r4, #1
0056c198: mov      r0, r3
0056c19c: ldr      r3, [r3]
0056c1a0: mov      lr, pc
0056c1a4: ldr      pc, [r3, #0xc]
0056c1a8: cmp      r0, #0
0056c1ac: beq      #0x56c178
0056c1b0: b        #0x56c158
0056c1b4: ldr      r3, [r5, #0x20]
0056c1b8: ldr      r2, [r5, #0x24]
0056c1bc: rsb      r2, r3, r2
0056c1c0: lsrs     r2, r2, #2
0056c1c4: beq      #0x56c20c
0056c1c8: mov      r4, #0
0056c1cc: b        #0x56c1e4
0056c1d0: ldr      r3, [r5, #0x20]
0056c1d4: ldr      r2, [r5, #0x24]
0056c1d8: rsb      r2, r3, r2
0056c1dc: cmp      r4, r2, asr #2
0056c1e0: bhs      #0x56c20c
0056c1e4: ldr      r3, [r3, r4, lsl #2]
0056c1e8: mov      r1, r6
0056c1ec: add      r4, r4, #1
0056c1f0: mov      r0, r3
0056c1f4: ldr      r3, [r3]
0056c1f8: mov      lr, pc
0056c1fc: ldr      pc, [r3, #0xc]
0056c200: cmp      r0, #0
0056c204: beq      #0x56c1d0
0056c208: b        #0x56c158
0056c20c: mov      r0, #0
0056c210: pop      {r4, r5, r6, pc}

# _ZNK7gameswf6cxform9transformENS_4rgbaE
00794f8c: push     {r4, r5, r6, r7, r8, lr}
00794f90: mov      r4, r0
00794f94: sub      sp, sp, #0x10
00794f98: uxtb     r0, r1
00794f9c: ubfx     r6, r1, #8, #8
00794fa0: ubfx     r5, r1, #0x10, #8
00794fa4: lsr      r8, r1, #0x18
00794fa8: bl       #0x30e964
00794fac: ldr      r1, [r4]
00794fb0: bl       #0x30ed6c
00794fb4: ldr      r1, [r4, #4]
00794fb8: bl       #0x30eba4
00794fbc: mov      r1, #0x43000000
00794fc0: add      r1, r1, #0x7f0000
00794fc4: mov      r7, r0
00794fc8: bl       #0x30e70c
00794fcc: cmp      r0, #0
00794fd0: moveq    r7, #0xff
00794fd4: beq      #0x794ff0
00794fd8: mov      r0, r7
00794fdc: mov      r1, #0
00794fe0: bl       #0x30e2f8
00794fe4: cmp      r0, #0
00794fe8: moveq    r7, #0
00794fec: bne      #0x7950f0
00794ff0: mov      r0, r6
00794ff4: bl       #0x30e964
00794ff8: ldr      r1, [r4, #8]
00794ffc: bl       #0x30ed6c
00795000: ldr      r1, [r4, #0xc]
00795004: bl       #0x30eba4
00795008: mov      r1, #0x43000000
0079500c: add      r1, r1, #0x7f0000
00795010: mov      r6, r0
00795014: bl       #0x30e70c
00795018: cmp      r0, #0
0079501c: moveq    r6, #0xff
00795020: beq      #0x79503c
00795024: mov      r0, r6
00795028: mov      r1, #0
0079502c: bl       #0x30e2f8
00795030: cmp      r0, #0
00795034: moveq    r6, #0
00795038: bne      #0x795120
0079503c: mov      r0, r5
00795040: bl       #0x30e964
00795044: ldr      r1, [r4, #0x10]
00795048: bl       #0x30ed6c
0079504c: ldr      r1, [r4, #0x14]
00795050: bl       #0x30eba4
00795054: mov      r1, #0x43000000
00795058: add      r1, r1, #0x7f0000
0079505c: mov      r5, r0
00795060: bl       #0x30e70c
00795064: cmp      r0, #0
00795068: moveq    r5, #0xff
0079506c: beq      #0x795088
00795070: mov      r0, r5
00795074: mov      r1, #0
00795078: bl       #0x30e2f8
0079507c: cmp      r0, #0
00795080: moveq    r5, #0
00795084: bne      #0x795110
00795088: mov      r0, r8
0079508c: bl       #0x30e964
00795090: ldr      r1, [r4, #0x18]
00795094: bl       #0x30ed6c
00795098: ldr      r1, [r4, #0x1c]
0079509c: bl       #0x30eba4
007950a0: mov      r1, #0x43000000
007950a4: add      r1, r1, #0x7f0000
007950a8: mov      r4, r0
007950ac: bl       #0x30e70c
007950b0: cmp      r0, #0
007950b4: moveq    r3, #0xff
007950b8: beq      #0x7950d4
007950bc: mov      r0, r4
007950c0: mov      r1, #0
007950c4: bl       #0x30e2f8
007950c8: cmp      r0, #0
007950cc: moveq    r3, #0
007950d0: bne      #0x795100
007950d4: mov      r0, #0
007950d8: bfi      r0, r7, #0, #8
007950dc: bfi      r0, r6, #8, #8
007950e0: bfi      r0, r5, #0x10, #8
007950e4: bfi      r0, r3, #0x18, #8
007950e8: add      sp, sp, #0x10
007950ec: pop      {r4, r5, r6, r7, r8, pc}
007950f0: mov      r0, r7
007950f4: bl       #0x8be2a0
007950f8: uxtb     r7, r0
007950fc: b        #0x794ff0
00795100: mov      r0, r4
00795104: bl       #0x8be2a0
00795108: uxtb     r3, r0
0079510c: b        #0x7950d4
00795110: mov      r0, r5
00795114: bl       #0x8be2a0
00795118: uxtb     r5, r0
0079511c: b        #0x795088
00795120: mov      r0, r6
00795124: bl       #0x8be2a0
00795128: uxtb     r6, r0
0079512c: b        #0x79503c

# _ZN6glitch5video15CTextureManager10getTextureEPKcS3_
005ed210: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005ed214: ldr      r5, [pc, #0x158]
005ed218: ldr      r8, [pc, #0x158]
005ed21c: mov      r4, r0
005ed220: add      r5, pc, r5
005ed224: ldr      r0, [r5, r8]
005ed228: sub      sp, sp, #0x34
005ed22c: mov      ip, #0
005ed230: ldr      r0, [r0]
005ed234: cmp      r3, #0
005ed238: str      ip, [r4]
005ed23c: mov      sl, r1
005ed240: str      r0, [sp, #0x2c]
005ed244: mov      r7, r2
005ed248: beq      #0x5ed2ec
005ed24c: add      r6, sp, #0x14
005ed250: mov      r1, r3
005ed254: mov      r0, r6
005ed258: add      r2, sp, #0x10
005ed25c: bl       #0x32603c
005ed260: ldr      r2, [sp, #0x28]
005ed264: add      r0, sp, #0xc
005ed268: mov      r1, sl
005ed26c: bl       #0x5e8f2c
005ed270: ldr      r3, [sp, #0xc]
005ed274: cmp      r3, #0
005ed278: ldrne    r2, [r3, #4]
005ed27c: addne    r2, r2, #1
005ed280: strne    r2, [r3, #4]
005ed284: ldr      r0, [r4]
005ed288: str      r3, [r4]
005ed28c: cmp      r0, #0
005ed290: beq      #0x5ed298
005ed294: bl       #0x31d584
005ed298: ldr      r0, [sp, #0xc]
005ed29c: cmp      r0, #0
005ed2a0: beq      #0x5ed2a8
005ed2a4: bl       #0x31d584
005ed2a8: ldr      sb, [r4]
005ed2ac: cmp      sb, #0
005ed2b0: beq      #0x5ed2fc
005ed2b4: ldr      r0, [sp, #0x28]
005ed2b8: cmp      r0, r6
005ed2bc: beq      #0x5ed2cc
005ed2c0: cmp      r0, #0
005ed2c4: beq      #0x5ed2cc
005ed2c8: bl       #0x310450
005ed2cc: ldr      r3, [r5, r8]
005ed2d0: ldr      r2, [sp, #0x2c]
005ed2d4: mov      r0, r4
005ed2d8: ldr      r3, [r3]
005ed2dc: cmp      r2, r3
005ed2e0: bne      #0x5ed370
005ed2e4: add      sp, sp, #0x34
005ed2e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005ed2ec: add      r6, sp, #0x14
005ed2f0: mov      r0, r6
005ed2f4: bl       #0x5e9908
005ed2f8: b        #0x5ed260
005ed2fc: ldr      r3, [sl, #0x2c]
005ed300: mov      r1, r7
005ed304: mov      r0, r3
005ed308: ldr      r3, [r3]
005ed30c: mov      lr, pc
005ed310: ldr      pc, [r3, #0xc]
005ed314: subs     fp, r0, #0
005ed318: beq      #0x5ed358
005ed31c: add      r7, sp, #8
005ed320: mov      r2, fp
005ed324: mov      r3, r6
005ed328: mov      r1, sl
005ed32c: mov      r0, r7
005ed330: str      sb, [sp]
005ed334: bl       #0x5ecf34
005ed338: mov      r1, r7
005ed33c: mov      r0, r4
005ed340: bl       #0x384df8
005ed344: mov      r0, r7
005ed348: bl       #0x41742c
005ed34c: mov      r0, fp
005ed350: bl       #0x31d584
005ed354: b        #0x5ed2b4
005ed358: ldr      r0, [pc, #0x1c]
005ed35c: mov      r1, r7
005ed360: mov      r2, #3
005ed364: add      r0, pc, r0
005ed368: bl       #0x60ace8
005ed36c: b        #0x5ed2b4
005ed370: bl       #0x30e310
005ed374: eorseq   r7, sl, r0, ror r8
005ed378: andeq    r4, r0, ip, lsr #1
005ed37c: mlaeq    pc, r4, r2, r6

# _ZN21render_handler_glitch18draw_triangle_listEPKviPKti
007d9734: str      lr, [sp, #-4]!
007d9738: mov      ip, r2
007d973c: sub      sp, sp, #0xc
007d9740: str      r3, [sp]
007d9744: mov      r3, ip
007d9748: ldr      ip, [sp, #0x10]
007d974c: mov      r2, r1
007d9750: mov      r1, #6
007d9754: str      ip, [sp, #4]
007d9758: bl       #0x7d94f8
007d975c: add      sp, sp, #0xc
007d9760: ldm      sp!, {pc}

# _ZNK6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE8CTexture16updateParametersEv
005afd40: push     {r4, r5, r6, r7, lr}
005afd44: mov      r4, r0
005afd48: ldr      r0, [pc, #0x278]
005afd4c: ldr      r2, [r4, #0x38]
005afd50: ldrh     r3, [r4, #0x40]
005afd54: ldr      r1, [pc, #0x270]
005afd58: add      r0, pc, r0
005afd5c: add      r0, r0, #0xa4
005afd60: tst      r3, #4
005afd64: and      ip, r2, #3
005afd68: sub      sp, sp, #0xc
005afd6c: ldr      r5, [r0, ip, lsl #2]
005afd70: add      r1, pc, r1
005afd74: beq      #0x5afda8
005afd78: ldrb     r3, [r4, #0x3f]
005afd7c: tst      r3, #2
005afd80: bne      #0x5afe7c
005afd84: ubfx     r2, r2, #0xc, #3
005afd88: ldr      r3, [pc, #0x240]
005afd8c: mov      r0, r5
005afd90: movw     r1, #0x2801
005afd94: add      r3, pc, r3
005afd98: add      r3, r3, #0xb4
005afd9c: ldr      r2, [r3, r2, lsl #2]
005afda0: bl       #0x30e910
005afda4: ldrh     r3, [r4, #0x40]
005afda8: tst      r3, #8
005afdac: bne      #0x5aff9c
005afdb0: tst      r3, #0x10
005afdb4: bne      #0x5aff70
005afdb8: tst      r3, #0x20
005afdbc: bne      #0x5aff44
005afdc0: tst      r3, #0x40
005afdc4: beq      #0x5afe74
005afdc8: ldr      r2, [r4, #0x34]
005afdcc: ldr      r1, [r2, #0x9c]
005afdd0: tst      r1, #0x80
005afdd4: bne      #0x5afe4c
005afdd8: tst      r3, #0x80
005afddc: beq      #0x5afdec
005afde0: ldr      r1, [r2, #0x9c]
005afde4: tst      r1, #0x20000
005afde8: bne      #0x5afef8
005afdec: ldr      r2, [r2, #0x7ec]
005afdf0: tst      r2, #0x80000
005afdf4: beq      #0x5afe34
005afdf8: tst      r3, #0x400
005afdfc: beq      #0x5afe34
005afe00: ldr      r3, [r4, #0x38]
005afe04: ubfx     r3, r3, #0xc, #3
005afe08: cmp      r3, #3
005afe0c: bgt      #0x5aff30
005afe10: mov      r1, #0x3f000000
005afe14: ldr      r0, [r4, #0x50]
005afe18: bl       #0x30eba4
005afe1c: bl       #0x30e4cc
005afe20: mov      r2, r0
005afe24: mov      r0, r5
005afe28: movw     r1, #0x813d
005afe2c: bl       #0x30e910
005afe30: ldrh     r3, [r4, #0x40]
005afe34: movw     r2, #0xe003
005afe38: movt     r2, #0
005afe3c: and      r2, r3, r2
005afe40: strh     r2, [r4, #0x40]
005afe44: add      sp, sp, #0xc
005afe48: pop      {r4, r5, r6, r7, pc}
005afe4c: ldr      r3, [pc, #0x180]
005afe50: ldr      r2, [r4, #0x38]
005afe54: mov      r0, r5
005afe58: add      r3, pc, r3
005afe5c: add      r3, r3, #0xcc
005afe60: ubfx     r2, r2, #0x15, #3
005afe64: ldr      r2, [r3, r2, lsl #2]
005afe68: movw     r1, #0x2803
005afe6c: bl       #0x30e910
005afe70: ldrh     r3, [r4, #0x40]
005afe74: ldr      r2, [r4, #0x34]
005afe78: b        #0x5afdd8
005afe7c: ldr      r0, [pc, #0x154]
005afe80: ubfx     r3, r2, #4, #6
005afe84: ldr      r1, [r1, r0]
005afe88: mov      r0, #0x28
005afe8c: mul      r3, r0, r3
005afe90: ldr      r3, [r1, r3]
005afe94: tst      r3, #8
005afe98: beq      #0x5afd84
005afe9c: mov      r0, #0
005afea0: ldr      r6, [r4, #0x1c]
005afea4: bl       #0x5fdaa0
005afea8: ldr      r1, [pc, #0x12c]
005afeac: ldr      r3, [pc, #0x12c]
005afeb0: ldr      ip, [r0]
005afeb4: mov      r2, r6
005afeb8: add      r3, pc, r3
005afebc: add      r1, pc, r1
005afec0: mov      r0, #3
005afec4: str      ip, [sp]
005afec8: bl       #0x60b034
005afecc: ldr      r3, [r4, #0x38]
005afed0: ubfx     r2, r3, #0xc, #3
005afed4: cmp      r2, #0
005afed8: beq      #0x5afd88
005afedc: ldrh     r2, [r4, #0x40]
005afee0: bic      r3, r3, #0x7000
005afee4: str      r3, [r4, #0x38]
005afee8: orr      r3, r2, #4
005afeec: strh     r3, [r4, #0x40]
005afef0: mov      r2, #0
005afef4: b        #0x5afd88
005afef8: ldr      r6, [r2, #0x4a8]
005afefc: ldr      r7, [r4, #0x44]
005aff00: mov      r0, r6
005aff04: mov      r1, r7
005aff08: bl       #0x30e70c
005aff0c: cmp      r0, #0
005aff10: moveq    r6, r7
005aff14: mov      r2, r6
005aff18: mov      r0, r5
005aff1c: movw     r1, #0x84fe
005aff20: bl       #0x30e25c
005aff24: ldr      r2, [r4, #0x34]
005aff28: ldrh     r3, [r4, #0x40]
005aff2c: b        #0x5afdec
005aff30: ldr      r0, [r4, #0x50]
005aff34: bl       #0x30e514
005aff38: bl       #0x30e4cc
005aff3c: mov      r2, r0
005aff40: b        #0x5afe24
005aff44: ldr      r3, [pc, #0x98]
005aff48: ldr      r2, [r4, #0x38]
005aff4c: mov      r0, r5
005aff50: add      r3, pc, r3
005aff54: add      r3, r3, #0xcc
005aff58: ubfx     r2, r2, #0x15, #3
005aff5c: ldr      r2, [r3, r2, lsl #2]
005aff60: movw     r1, #0x2803
005aff64: bl       #0x30e910
005aff68: ldrh     r3, [r4, #0x40]
005aff6c: b        #0x5afdc0
005aff70: ldr      r3, [pc, #0x70]
005aff74: ldr      r2, [r4, #0x38]
005aff78: mov      r0, r5
005aff7c: add      r3, pc, r3
005aff80: add      r3, r3, #0xcc
005aff84: ubfx     r2, r2, #0x12, #3
005aff88: ldr      r2, [r3, r2, lsl #2]
005aff8c: movw     r1, #0x2802
005aff90: bl       #0x30e910
005aff94: ldrh     r3, [r4, #0x40]
005aff98: b        #0x5afdb8
005aff9c: ldr      r3, [pc, #0x48]
005affa0: ldr      r2, [r4, #0x38]
005affa4: mov      r0, r5
005affa8: add      r3, pc, r3
005affac: add      r3, r3, #0xb4
005affb0: ubfx     r2, r2, #0xf, #3
005affb4: ldr      r2, [r3, r2, lsl #2]
005affb8: mov      r1, #0x2800
005affbc: bl       #0x30e910
005affc0: ldrh     r3, [r4, #0x40]
005affc4: b        #0x5afdb0
005affc8: ldrsbteq r0, [r3], -ip
005affcc: eorseq   r4, lr, r0, lsr #26
005affd0: eorseq   r0, r3, r0, lsr #5
005affd4: ldrsbteq r0, [r3], -ip
005affd8: andeq    r1, r0, r4, lsr pc
005affdc: eorseq   r0, r3, r4, asr #9
005affe0: eorseq   r0, r3, r8, lsr #10
005affe4: eorseq   r0, r3, r4, ror #1
005affe8: ldrhteq  r0, [r3], -r8
005affec: eorseq   r0, r3, ip, lsl #1

# _ZN6glitch2io10CZipReader8findFileEPKc
00578520: push     {r4, r5, r6, r7, r8, lr}
00578524: ldr      r4, [pc, #0xf0]
00578528: ldr      r7, [pc, #0xf0]
0057852c: sub      sp, sp, #0x70
00578530: add      r4, pc, r4
00578534: ldr      r3, [r4, r7]
00578538: mov      r8, r1
0057853c: mov      r6, r0
00578540: ldr      r3, [r3]
00578544: mov      r0, sp
00578548: mov      r5, sp
0057854c: str      r3, [sp, #0x6c]
00578550: bl       #0x577308
00578554: mov      r0, r8
00578558: bl       #0x30de54
0057855c: mov      r1, r8
00578560: add      r2, r8, r0
00578564: add      r0, sp, #0x18
00578568: bl       #0x320b88
0057856c: ldrb     r3, [r6, #0xc]
00578570: cmp      r3, #0
00578574: beq      #0x5785c8
00578578: ldr      r2, [sp, #0x2c]
0057857c: ldr      r3, [sp, #0x28]
00578580: cmp      r2, r3
00578584: beq      #0x5785c8
00578588: mov      r3, #0
0057858c: ldrb     r1, [r2, r3]
00578590: add      r2, r2, r3
00578594: add      r3, r3, #1
00578598: uxtb     r0, r1
0057859c: sub      ip, r0, #0x41
005785a0: uxtb     ip, ip
005785a4: cmp      ip, #0x19
005785a8: addls    r1, r0, #0x20
005785ac: uxtbls   r1, r1
005785b0: strb     r1, [r2]
005785b4: ldr      r2, [sp, #0x2c]
005785b8: ldr      r1, [sp, #0x28]
005785bc: rsb      r1, r2, r1
005785c0: cmp      r3, r1
005785c4: blo      #0x57858c
005785c8: ldrb     r3, [r6, #0xd]
005785cc: cmp      r3, #0
005785d0: beq      #0x5785e0
005785d4: mov      r0, r6
005785d8: add      r1, r5, #0x18
005785dc: bl       #0x577b50
005785e0: add      r0, r6, #0x14
005785e4: mov      r1, sp
005785e8: bl       #0x5783c4
005785ec: mov      r6, r0
005785f0: mov      r0, sp
005785f4: bl       #0x5773e4
005785f8: ldr      r3, [r4, r7]
005785fc: ldr      r2, [sp, #0x6c]
00578600: mov      r0, r6
00578604: ldr      r3, [r3]
00578608: cmp      r2, r3
0057860c: bne      #0x578618
00578610: add      sp, sp, #0x70
00578614: pop      {r4, r5, r6, r7, r8, pc}
00578618: bl       #0x30e310
0057861c: subeq    ip, r1, r0, ror #10
00578620: andeq    r4, r0, ip, lsr #1

# _ZNK14FileSystemBase18ApplyFilenameHacksEPKc
0034e96c: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0034e970: ldr      r5, [pc, #0x1d4]
0034e974: ldr      r8, [pc, #0x1d4]
0034e978: mov      r4, r0
0034e97c: add      r5, pc, r5
0034e980: ldr      r3, [r5, r8]
0034e984: sub      sp, sp, #0x54
0034e988: mov      sl, r1
0034e98c: ldr      r3, [r3]
0034e990: mov      r1, #0x10
0034e994: str      r0, [r4, #0x10]
0034e998: str      r0, [r4, #0x14]
0034e99c: mov      r6, r2
0034e9a0: str      r3, [sp, #0x4c]
0034e9a4: bl       #0x3209a8
0034e9a8: ldr      r3, [r4, #0x10]
0034e9ac: mov      r2, #0
0034e9b0: mov      r0, sl
0034e9b4: strb     r2, [r3]
0034e9b8: ldr      r3, [sl]
0034e9bc: mov      lr, pc
0034e9c0: ldr      pc, [r3, #0x2c]
0034e9c4: mov      r7, r0
0034e9c8: bl       #0x30de54
0034e9cc: mov      r1, r7
0034e9d0: mov      sb, r0
0034e9d4: mov      r0, r6
0034e9d8: bl       #0x30ebd4
0034e9dc: subs     r7, r0, #0
0034e9e0: beq      #0x34eb2c
0034e9e4: add      fp, sb, #1
0034e9e8: add      fp, r7, fp
0034e9ec: mov      r0, fp
0034e9f0: bl       #0x30de54
0034e9f4: mov      r1, fp
0034e9f8: add      r2, fp, r0
0034e9fc: mov      r0, r4
0034ea00: bl       #0x320b88
0034ea04: ldr      r1, [pc, #0x148]
0034ea08: ldr      r0, [r4, #0x14]
0034ea0c: add      r1, pc, r1
0034ea10: bl       #0x30ebd4
0034ea14: cmp      r0, #0
0034ea18: beq      #0x34ea70
0034ea1c: add      fp, sp, #0x34
0034ea20: mov      r3, #1
0034ea24: mov      r1, sl
0034ea28: mov      r0, fp
0034ea2c: mov      r2, r4
0034ea30: bl       #0x56c740
0034ea34: ldr      r1, [pc, #0x11c]
0034ea38: mov      r0, r4
0034ea3c: add      r1, pc, r1
0034ea40: add      r2, r1, #0x10
0034ea44: bl       #0x320b88
0034ea48: mov      r0, r4
0034ea4c: ldr      r1, [sp, #0x48]
0034ea50: ldr      r2, [sp, #0x44]
0034ea54: bl       #0x320a4c
0034ea58: ldr      r0, [sp, #0x48]
0034ea5c: cmp      r0, fp
0034ea60: beq      #0x34ea70
0034ea64: cmp      r0, #0
0034ea68: beq      #0x34ea70
0034ea6c: bl       #0x310450
0034ea70: mov      r0, r4
0034ea74: mov      r1, #0
0034ea78: mvn      r2, #0
0034ea7c: bl       #0x34e090
0034ea80: cmp      r7, #0
0034ea84: beq      #0x34eb0c
0034ea88: rsb      r3, r6, #1
0034ea8c: add      sb, r3, sb
0034ea90: mov      r1, r6
0034ea94: add      r7, r7, sb
0034ea98: add      r6, sp, #0x1c
0034ea9c: add      r2, r1, r7
0034eaa0: mov      r0, r6
0034eaa4: add      r7, sp, #4
0034eaa8: str      r6, [sp, #0x2c]
0034eaac: str      r6, [sp, #0x30]
0034eab0: bl       #0x325ff4
0034eab4: mov      r0, r7
0034eab8: mov      r1, r6
0034eabc: mov      r2, r4
0034eac0: bl       #0x34e324
0034eac4: cmp      r4, r7
0034eac8: beq      #0x34eadc
0034eacc: mov      r0, r4
0034ead0: ldr      r1, [sp, #0x18]
0034ead4: ldr      r2, [sp, #0x14]
0034ead8: bl       #0x320b88
0034eadc: ldr      r0, [sp, #0x18]
0034eae0: cmp      r0, r7
0034eae4: beq      #0x34eaf4
0034eae8: cmp      r0, #0
0034eaec: beq      #0x34eaf4
0034eaf0: bl       #0x310450
0034eaf4: ldr      r0, [sp, #0x30]
0034eaf8: cmp      r0, r6
0034eafc: beq      #0x34eb0c
0034eb00: cmp      r0, #0
0034eb04: beq      #0x34eb0c
0034eb08: bl       #0x310450
0034eb0c: ldr      r3, [r5, r8]
0034eb10: ldr      r2, [sp, #0x4c]
0034eb14: mov      r0, r4
0034eb18: ldr      r3, [r3]
0034eb1c: cmp      r2, r3
0034eb20: bne      #0x34eb48
0034eb24: add      sp, sp, #0x54
0034eb28: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0034eb2c: mov      r0, r6
0034eb30: bl       #0x30de54
0034eb34: mov      r1, r6
0034eb38: add      r2, r6, r0
0034eb3c: mov      r0, r4
0034eb40: bl       #0x320b88
0034eb44: b        #0x34ea04
0034eb48: bl       #0x30e310
0034eb4c: rsbeq    r6, r4, r4, lsl r1
0034eb50: andeq    r4, r0, ip, lsr #1
0034eb54: subseq   sb, r7, ip, lsr #14
0034eb58: ldrsheq  r1, [r7], #-0xcc

# _ZN7gameswf6cxform5clampEv
00795130: push     {r4, r5, r6, lr}
00795134: ldr      r5, [r0]
00795138: mov      r4, r0
0079513c: mov      r1, #0x3f800000
00795140: mov      r0, r5
00795144: bl       #0x30e70c
00795148: cmp      r0, #0
0079514c: moveq    r5, #0x3f800000
00795150: beq      #0x79516c
00795154: mov      r0, r5
00795158: mov      r1, #0
0079515c: bl       #0x30e2f8
00795160: cmp      r0, #0
00795164: bne      #0x79533c
00795168: mov      r5, #0
0079516c: ldr      r6, [r4, #8]
00795170: str      r5, [r4]
00795174: mov      r1, #0x3f800000
00795178: mov      r0, r6
0079517c: bl       #0x30e70c
00795180: cmp      r0, #0
00795184: moveq    r6, #0x3f800000
00795188: beq      #0x7951a4
0079518c: mov      r0, r6
00795190: mov      r1, #0
00795194: bl       #0x30e2f8
00795198: cmp      r0, #0
0079519c: bne      #0x7954a8
007951a0: mov      r6, #0
007951a4: ldr      r5, [r4, #0x10]
007951a8: str      r6, [r4, #8]
007951ac: mov      r1, #0x3f800000
007951b0: mov      r0, r5
007951b4: bl       #0x30e70c
007951b8: cmp      r0, #0
007951bc: moveq    r5, #0x3f800000
007951c0: beq      #0x7951dc
007951c4: mov      r0, r5
007951c8: mov      r1, #0
007951cc: bl       #0x30e2f8
007951d0: cmp      r0, #0
007951d4: bne      #0x795478
007951d8: mov      r5, #0
007951dc: ldr      r6, [r4, #0x18]
007951e0: str      r5, [r4, #0x10]
007951e4: mov      r1, #0x3f800000
007951e8: mov      r0, r6
007951ec: bl       #0x30e70c
007951f0: cmp      r0, #0
007951f4: moveq    r6, #0x3f800000
007951f8: beq      #0x795214
007951fc: mov      r0, r6
00795200: mov      r1, #0
00795204: bl       #0x30e2f8
00795208: cmp      r0, #0
0079520c: bne      #0x795448
00795210: mov      r6, #0
00795214: ldr      r5, [r4, #4]
00795218: mov      r1, #0x43000000
0079521c: str      r6, [r4, #0x18]
00795220: mov      r0, r5
00795224: add      r1, r1, #0x7f0000
00795228: bl       #0x30e70c
0079522c: cmp      r0, #0
00795230: moveq    r5, #0x43000000
00795234: addeq    r5, r5, #0x7f0000
00795238: beq      #0x79525c
0079523c: mov      r1, #0xc3000000
00795240: mov      r0, r5
00795244: add      r1, r1, #0x7f0000
00795248: bl       #0x30e2f8
0079524c: cmp      r0, #0
00795250: moveq    r5, #0xc3000000
00795254: addeq    r5, r5, #0x7f0000
00795258: bne      #0x795414
0079525c: ldr      r6, [r4, #0xc]
00795260: mov      r1, #0x43000000
00795264: str      r5, [r4, #4]
00795268: mov      r0, r6
0079526c: add      r1, r1, #0x7f0000
00795270: bl       #0x30e70c
00795274: cmp      r0, #0
00795278: moveq    r6, #0x43000000
0079527c: addeq    r6, r6, #0x7f0000
00795280: beq      #0x7952a4
00795284: mov      r1, #0xc3000000
00795288: mov      r0, r6
0079528c: add      r1, r1, #0x7f0000
00795290: bl       #0x30e2f8
00795294: cmp      r0, #0
00795298: moveq    r6, #0xc3000000
0079529c: addeq    r6, r6, #0x7f0000
007952a0: bne      #0x7953e0
007952a4: ldr      r5, [r4, #0x14]
007952a8: mov      r1, #0x43000000
007952ac: str      r6, [r4, #0xc]
007952b0: mov      r0, r5
007952b4: add      r1, r1, #0x7f0000
007952b8: bl       #0x30e70c
007952bc: cmp      r0, #0
007952c0: moveq    r5, #0x43000000
007952c4: addeq    r5, r5, #0x7f0000
007952c8: beq      #0x7952ec
007952cc: mov      r1, #0xc3000000
007952d0: mov      r0, r5
007952d4: add      r1, r1, #0x7f0000
007952d8: bl       #0x30e2f8
007952dc: cmp      r0, #0
007952e0: moveq    r5, #0xc3000000
007952e4: addeq    r5, r5, #0x7f0000
007952e8: bne      #0x7953ac
007952ec: ldr      r6, [r4, #0x1c]
007952f0: mov      r1, #0x43000000
007952f4: str      r5, [r4, #0x14]
007952f8: mov      r0, r6
007952fc: add      r1, r1, #0x7f0000
00795300: bl       #0x30e70c
00795304: cmp      r0, #0
00795308: moveq    r6, #0x43000000
0079530c: addeq    r6, r6, #0x7f0000
00795310: beq      #0x795334
00795314: mov      r1, #0xc3000000
00795318: mov      r0, r6
0079531c: add      r1, r1, #0x7f0000
00795320: bl       #0x30e2f8
00795324: cmp      r0, #0
00795328: moveq    r6, #0xc3000000
0079532c: addeq    r6, r6, #0x7f0000
00795330: bne      #0x79536c
00795334: str      r6, [r4, #0x1c]
00795338: pop      {r4, r5, r6, pc}
0079533c: mov      r0, r5
00795340: mvn      r1, #0x800000
00795344: bl       #0x30e4b4
00795348: cmp      r0, #0
0079534c: beq      #0x795168
00795350: mvn      r1, #0x80000000
00795354: mov      r0, r5
00795358: sub      r1, r1, #0x800000
0079535c: bl       #0x30e9ac
00795360: cmp      r0, #0
00795364: beq      #0x795168
00795368: b        #0x79516c
0079536c: mov      r0, r6
00795370: mvn      r1, #0x800000
00795374: bl       #0x30e4b4
00795378: cmp      r0, #0
0079537c: bne      #0x79538c
00795380: mov      r6, #0
00795384: str      r6, [r4, #0x1c]
00795388: pop      {r4, r5, r6, pc}
0079538c: mvn      r1, #0x80000000
00795390: mov      r0, r6
00795394: sub      r1, r1, #0x800000
00795398: bl       #0x30e9ac
0079539c: cmp      r0, #0
007953a0: bne      #0x795334
007953a4: mov      r6, #0
007953a8: b        #0x795384
007953ac: mov      r0, r5
007953b0: mvn      r1, #0x800000
007953b4: bl       #0x30e4b4
007953b8: cmp      r0, #0
007953bc: moveq    r5, #0
007953c0: beq      #0x7952ec
007953c4: mvn      r1, #0x80000000
007953c8: mov      r0, r5
007953cc: sub      r1, r1, #0x800000
007953d0: bl       #0x30e9ac
007953d4: cmp      r0, #0
007953d8: moveq    r5, #0
007953dc: b        #0x7952ec
007953e0: mov      r0, r6
007953e4: mvn      r1, #0x800000
007953e8: bl       #0x30e4b4
007953ec: cmp      r0, #0
007953f0: moveq    r6, #0
007953f4: beq      #0x7952a4
007953f8: mvn      r1, #0x80000000
007953fc: mov      r0, r6
00795400: sub      r1, r1, #0x800000
00795404: bl       #0x30e9ac
00795408: cmp      r0, #0
0079540c: moveq    r6, #0
00795410: b        #0x7952a4
00795414: mov      r0, r5
00795418: mvn      r1, #0x800000
0079541c: bl       #0x30e4b4
00795420: cmp      r0, #0
00795424: moveq    r5, #0
00795428: beq      #0x79525c
0079542c: mvn      r1, #0x80000000
00795430: mov      r0, r5
00795434: sub      r1, r1, #0x800000
00795438: bl       #0x30e9ac
0079543c: cmp      r0, #0
00795440: moveq    r5, #0
00795444: b        #0x79525c
00795448: mov      r0, r6
0079544c: mvn      r1, #0x800000
00795450: bl       #0x30e4b4
00795454: cmp      r0, #0
00795458: beq      #0x795210
0079545c: mvn      r1, #0x80000000
00795460: mov      r0, r6
00795464: sub      r1, r1, #0x800000
00795468: bl       #0x30e9ac
0079546c: cmp      r0, #0
00795470: beq      #0x795210
00795474: b        #0x795214
00795478: mov      r0, r5
0079547c: mvn      r1, #0x800000
00795480: bl       #0x30e4b4
00795484: cmp      r0, #0
00795488: beq      #0x7951d8
0079548c: mvn      r1, #0x80000000
00795490: mov      r0, r5
00795494: sub      r1, r1, #0x800000
00795498: bl       #0x30e9ac
0079549c: cmp      r0, #0
007954a0: beq      #0x7951d8
007954a4: b        #0x7951dc
007954a8: mov      r0, r6
007954ac: mvn      r1, #0x800000
007954b0: bl       #0x30e4b4
007954b4: cmp      r0, #0
007954b8: beq      #0x7951a0
007954bc: mvn      r1, #0x80000000
007954c0: mov      r0, r6
007954c4: sub      r1, r1, #0x800000
007954c8: bl       #0x30e9ac
007954cc: cmp      r0, #0
007954d0: beq      #0x7951a0
007954d4: b        #0x7951a4

# _ZN6glitch5video6detail10renderpass12SRenderStateC1ERKNS0_12SRenderStateE
005d7a10: push     {r4, r5}
005d7a14: ldrb     ip, [r1, #0x15]
005d7a18: ldrb     r2, [r1, #0x14]
005d7a1c: ldrb     r4, [r1, #0x17]
005d7a20: ldrb     r5, [r1, #0x16]
005d7a24: strb     r2, [r0, #8]
005d7a28: strb     r4, [r0, #0xb]
005d7a2c: strb     r5, [r0, #0xa]
005d7a30: strb     ip, [r0, #9]
005d7a34: mov      r3, r0
005d7a38: ldr      r0, [r1, #0x28]
005d7a3c: mov      r2, #0
005d7a40: str      r0, [r3, #0xc]
005d7a44: ldr      r0, [r1, #0x2c]
005d7a48: str      r0, [r3, #0x10]
005d7a4c: ldr      r0, [r1, #0x38]
005d7a50: str      r2, [r3, #4]
005d7a54: str      r2, [r3]
005d7a58: str      r0, [r3, #0x1c]
005d7a5c: ldr      r2, [r1, #0xc]
005d7a60: tst      r2, #0x80000
005d7a64: movne    r2, #0x10000
005d7a68: strne    r2, [r3, #4]
005d7a6c: ldr      ip, [r1, #8]
005d7a70: ubfx     ip, ip, #0xc, #3
005d7a74: lsl      ip, ip, #0x18
005d7a78: str      ip, [r3]
005d7a7c: ldrb     r0, [r1]
005d7a80: orr      ip, ip, r0
005d7a84: str      ip, [r3]
005d7a88: ldr      r2, [r1, #0xc]
005d7a8c: tst      r2, #0x100000
005d7a90: ldr      r2, [r3, #4]
005d7a94: orrne    r2, r2, #0x20000
005d7a98: biceq    r2, r2, #0x20000
005d7a9c: str      r2, [r3, #4]
005d7aa0: ldr      r0, [r1, #8]
005d7aa4: bic      r2, r2, #0x40000
005d7aa8: and      r0, r0, #0xc0000000
005d7aac: orr      r0, ip, r0
005d7ab0: str      r0, [r3]
005d7ab4: ldr      ip, [r1, #0xc]
005d7ab8: ubfx     ip, ip, #0x15, #1
005d7abc: orr      r2, r2, ip, lsl #18
005d7ac0: str      r2, [r3, #4]
005d7ac4: ldr      ip, [r1, #0xc]
005d7ac8: tst      ip, #0x400000
005d7acc: orrne    r2, r2, #0x80000
005d7ad0: biceq    r2, r2, #0x80000
005d7ad4: str      r2, [r3, #4]
005d7ad8: ldr      r2, [r1, #0xc]
005d7adc: ubfx     r2, r2, #0xc, #3
005d7ae0: orr      r0, r0, r2, lsl #27
005d7ae4: str      r0, [r3]
005d7ae8: ldr      r2, [r1, #0xc]
005d7aec: tst      r2, #0x800000
005d7af0: ldr      r2, [r3, #4]
005d7af4: orrne    r2, r2, #0x100000
005d7af8: biceq    r2, r2, #0x100000
005d7afc: str      r2, [r3, #4]
005d7b00: ldr      r0, [r1, #0xc]
005d7b04: bic      r2, r2, #0x3000
005d7b08: ubfx     r0, r0, #0xf, #2
005d7b0c: orr      r2, r2, r0, lsl #12
005d7b10: str      r2, [r3, #4]
005d7b14: ldr      r0, [r1, #0xc]
005d7b18: bic      r2, r2, #0xc000
005d7b1c: ubfx     r0, r0, #0x11, #2
005d7b20: orr      r2, r2, r0, lsl #14
005d7b24: str      r2, [r3, #4]
005d7b28: ldr      r0, [r1, #0xc]
005d7b2c: tst      r0, #0x2000000
005d7b30: orrne    ip, r2, #0x200000
005d7b34: biceq    ip, r2, #0x200000
005d7b38: str      ip, [r3, #4]
005d7b3c: ldr      r2, [r1, #0xc]
005d7b40: tst      r2, #0x4000000
005d7b44: orrne    ip, ip, #0x400000
005d7b48: biceq    ip, ip, #0x400000
005d7b4c: str      ip, [r3, #4]
005d7b50: ldr      r2, [r1, #0xc]
005d7b54: tst      r2, #0x8000000
005d7b58: orrne    ip, ip, #0x800000
005d7b5c: biceq    ip, ip, #0x800000
005d7b60: str      ip, [r3, #4]
005d7b64: ldr      r2, [r1, #0xc]
005d7b68: tst      r2, #0x10000000
005d7b6c: orrne    ip, ip, #0x1000000
005d7b70: biceq    ip, ip, #0x1000000
005d7b74: str      ip, [r3, #4]
005d7b78: ldr      r2, [r1, #0xc]
005d7b7c: tst      r2, #0x20000000
005d7b80: orrne    ip, ip, #0x2000000
005d7b84: biceq    ip, ip, #0x2000000
005d7b88: str      ip, [r3, #4]
005d7b8c: ldr      r2, [r1, #0xc]
005d7b90: tst      r2, #0x40000000
005d7b94: orrne    ip, ip, #0x4000000
005d7b98: biceq    ip, ip, #0x4000000
005d7b9c: str      ip, [r3, #4]
005d7ba0: ldr      r2, [r1, #0x10]
005d7ba4: tst      r2, #1
005d7ba8: orrne    ip, ip, #0x8000000
005d7bac: biceq    ip, ip, #0x8000000
005d7bb0: str      ip, [r3, #4]
005d7bb4: ldr      r0, [r1, #8]
005d7bb8: bic      ip, ip, #7
005d7bbc: ldr      r2, [r3]
005d7bc0: ubfx     r0, r0, #0x12, #3
005d7bc4: orr      ip, ip, r0
005d7bc8: str      ip, [r3, #4]
005d7bcc: ldrb     r0, [r1, #2]
005d7bd0: bic      r2, r2, #0xff00
005d7bd4: bic      ip, ip, #0x38
005d7bd8: orr      r2, r2, r0, lsl #8
005d7bdc: str      r2, [r3]
005d7be0: ldrb     r4, [r1, #3]
005d7be4: bic      r2, r2, #0xff0000
005d7be8: mov      r0, r3
005d7bec: orr      r2, r2, r4, lsl #16
005d7bf0: str      r2, [r3]
005d7bf4: ldr      r2, [r1, #8]
005d7bf8: ubfx     r2, r2, #0x15, #3
005d7bfc: orr      r2, ip, r2, lsl #3
005d7c00: str      r2, [r3, #4]
005d7c04: ldr      ip, [r1, #8]
005d7c08: bic      r2, r2, #0x1c0
005d7c0c: ubfx     ip, ip, #0x18, #3
005d7c10: orr      r2, r2, ip, lsl #6
005d7c14: str      r2, [r3, #4]
005d7c18: ldr      ip, [r1, #8]
005d7c1c: bic      r2, r2, #0xe00
005d7c20: ubfx     ip, ip, #0x1b, #3
005d7c24: orr      r2, r2, ip, lsl #9
005d7c28: str      r2, [r3, #4]
005d7c2c: ldr      ip, [r1, #0x30]
005d7c30: ldr      r2, [r1, #0x34]
005d7c34: str      ip, [r3, #0x14]
005d7c38: str      r2, [r3, #0x18]
005d7c3c: pop      {r4, r5}
005d7c40: bx       lr

# _Z11ToLowerCaseRSbIcSt11char_traitsIcEN6glitch4core10SAllocatorIcLNS1_6memory13E_MEMORY_HINTE0EEEEii
0034e090: ldr      r3, [r0, #0x14]
0034e094: ldr      ip, [r0, #0x10]
0034e098: cmn      r2, #1
0034e09c: str      r4, [sp, #-4]!
0034e0a0: rsb      ip, r3, ip
0034e0a4: beq      #0x34e0f4
0034e0a8: cmp      r2, ip
0034e0ac: bge      #0x34e0f4
0034e0b0: cmp      r1, r2
0034e0b4: ble      #0x34e0c0
0034e0b8: b        #0x34e0ec
0034e0bc: ldr      r3, [r0, #0x14]
0034e0c0: ldrb     r4, [r3, r1]
0034e0c4: add      r3, r3, r1
0034e0c8: add      r1, r1, #1
0034e0cc: sxtb     ip, r4
0034e0d0: cmp      ip, #0x40
0034e0d4: ble      #0x34e0e4
0034e0d8: cmp      ip, #0x5a
0034e0dc: addle    r4, r4, #0x20
0034e0e0: strble   r4, [r3]
0034e0e4: cmp      r1, r2
0034e0e8: ble      #0x34e0bc
0034e0ec: ldm      sp!, {r4}
0034e0f0: bx       lr
0034e0f4: sub      r2, ip, #1
0034e0f8: b        #0x34e0b0

# _ZN6glitch2io11CFileSystem19getWorkingDirectoryEv
0056c214: ldr      r3, [pc, #0xc]
0056c218: ldr      r2, [pc, #0xc]
0056c21c: add      r3, pc, r3
0056c220: ldr      r0, [r3, r2]
0056c224: bx       lr
0056c228: subeq    r8, r2, r4, ror r8
0056c22c: andeq    r0, r0, r8, ror #28

# _ZN21render_handler_glitch10fill_style10set_bitmapEPN7gameswf11bitmap_infoERKNS1_6matrixENS1_14render_handler16bitmap_wrap_modeERKNS1_6cxformE
007d4430: push     {r4, r5, r6, r7, r8, lr}
007d4434: cmp      r3, #0
007d4438: moveq    r3, #2
007d443c: movne    r3, #3
007d4440: str      r3, [r0]
007d4444: str      r1, [r0, #8]
007d4448: add      lr, r0, #0xc
007d444c: mov      ip, r2
007d4450: mov      r4, r0
007d4454: ldr      r5, [sp, #0x18]
007d4458: ldm      ip!, {r0, r1, r2, r3}
007d445c: stm      lr!, {r0, r1, r2, r3}
007d4460: ldm      ip, {r0, r1}
007d4464: add      ip, r4, #0x24
007d4468: stm      lr, {r0, r1}
007d446c: ldm      r5!, {r0, r1, r2, r3}
007d4470: stm      ip!, {r0, r1, r2, r3}
007d4474: ldm      r5, {r0, r1, r2, r3}
007d4478: stm      ip, {r0, r1, r2, r3}
007d447c: add      r0, r4, #0x24
007d4480: bl       #0x795130
007d4484: mov      r1, #0x43000000
007d4488: add      r1, r1, #0x7f0000
007d448c: ldr      r0, [r4, #0x24]
007d4490: bl       #0x30ed6c
007d4494: bl       #0x8be2a0
007d4498: mov      r1, #0x43000000
007d449c: add      r1, r1, #0x7f0000
007d44a0: uxtb     r5, r0
007d44a4: ldr      r0, [r4, #0x2c]
007d44a8: bl       #0x30ed6c
007d44ac: bl       #0x8be2a0
007d44b0: mov      r1, #0x43000000
007d44b4: add      r1, r1, #0x7f0000
007d44b8: uxtb     r6, r0
007d44bc: ldr      r0, [r4, #0x34]
007d44c0: bl       #0x30ed6c
007d44c4: bl       #0x8be2a0
007d44c8: mov      r1, #0x43000000
007d44cc: add      r1, r1, #0x7f0000
007d44d0: uxtb     r7, r0
007d44d4: ldr      r0, [r4, #0x3c]
007d44d8: bl       #0x30ed6c
007d44dc: bl       #0x8be2a0
007d44e0: strb     r7, [r4, #6]
007d44e4: strb     r0, [r4, #7]
007d44e8: strb     r6, [r4, #5]
007d44ec: strb     r5, [r4, #4]
007d44f0: ldr      r0, [r4, #0x28]
007d44f4: mov      r1, #0x3f800000
007d44f8: bl       #0x30e2f8
007d44fc: cmp      r0, #0
007d4500: beq      #0x7d4510
007d4504: mov      r3, #1
007d4508: strb     r3, [r4, #0x44]
007d450c: pop      {r4, r5, r6, r7, r8, pc}
007d4510: ldr      r0, [r4, #0x30]
007d4514: mov      r1, #0x3f800000
007d4518: bl       #0x30e2f8
007d451c: cmp      r0, #0
007d4520: bne      #0x7d4504
007d4524: ldr      r0, [r4, #0x38]
007d4528: mov      r1, #0x3f800000
007d452c: bl       #0x30e2f8
007d4530: cmp      r0, #0
007d4534: bne      #0x7d4504
007d4538: ldr      r0, [r4, #0x40]
007d453c: mov      r1, #0x3f800000
007d4540: bl       #0x30e2f8
007d4544: cmp      r0, #0
007d4548: bne      #0x7d4504
007d454c: mov      r3, #0
007d4550: strb     r3, [r4, #0x44]
007d4554: pop      {r4, r5, r6, r7, r8, pc}

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

# _ZN6glitch5video15CCommonGLDriverINS0_21CProgrammableGLDriverINS0_18CGLSLShaderHandlerEEENS0_6detail33CProgrammableGLFunctionPointerSetEE21applyRenderStateBlendINS5_10renderpass12SRenderStateEEEvRKT_
005af3f8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005af3fc: ldrb     r3, [r0, #0x1c4]
005af400: sub      sp, sp, #0xc
005af404: mov      r4, r0
005af408: cmp      r3, #0
005af40c: mov      r8, r1
005af410: beq      #0x5af558
005af414: ldr      r2, [r8]
005af418: ldr      r3, [r4, #0x1fc]
005af41c: ubfx     r5, r2, #0x18, #3
005af420: cmp      r5, r3
005af424: beq      #0x5af444
005af428: ldr      r3, [pc, #0x13c]
005af42c: add      r3, pc, r3
005af430: add      r3, r3, #0x3c
005af434: ldr      r0, [r3, r5, lsl #2]
005af438: bl       #0x30df14
005af43c: str      r5, [r4, #0x1fc]
005af440: ldr      r2, [r8]
005af444: and      r3, r2, #0xf
005af448: mov      r5, #0
005af44c: ubfx     r2, r2, #4, #4
005af450: bfi      r5, r3, #0, #8
005af454: ldr      r1, [r4, #0x200]
005af458: bfi      r5, r2, #8, #8
005af45c: bfc      r5, #0x10, #0x10
005af460: cmp      r5, r1
005af464: beq      #0x5af480
005af468: ldr      r0, [pc, #0x100]
005af46c: add      r0, pc, r0
005af470: ldr      r1, [r0, r2, lsl #2]
005af474: ldr      r0, [r0, r3, lsl #2]
005af478: bl       #0x30e418
005af47c: str      r5, [r4, #0x200]
005af480: ldrb     r7, [r8, #8]
005af484: ldrb     r6, [r8, #0xb]
005af488: ldrb     r5, [r8, #0xa]
005af48c: ldrb     r3, [r4, #0x204]
005af490: ldrb     r2, [r4, #0x207]
005af494: ldrb     r8, [r8, #9]
005af498: ldrb     r0, [r4, #0x206]
005af49c: ldrb     r1, [r4, #0x205]
005af4a0: strb     r2, [sp, #3]
005af4a4: strb     r0, [sp, #2]
005af4a8: strb     r1, [sp, #1]
005af4ac: strb     r3, [sp]
005af4b0: strb     r6, [sp, #7]
005af4b4: strb     r5, [sp, #6]
005af4b8: strb     r8, [sp, #5]
005af4bc: strb     r7, [sp, #4]
005af4c0: ldr      r3, [sp]
005af4c4: ldr      r2, [sp, #4]
005af4c8: cmp      r2, r3
005af4cc: beq      #0x5af550
005af4d0: mov      r0, r7
005af4d4: bl       #0x30e964
005af4d8: movw     r1, #0x8081
005af4dc: movt     r1, #0x3b80
005af4e0: bl       #0x30ed6c
005af4e4: mov      sb, r0
005af4e8: mov      r0, r8
005af4ec: bl       #0x30e964
005af4f0: movw     r1, #0x8081
005af4f4: movt     r1, #0x3b80
005af4f8: bl       #0x30ed6c
005af4fc: mov      sl, r0
005af500: mov      r0, r5
005af504: bl       #0x30e964
005af508: movw     r1, #0x8081
005af50c: movt     r1, #0x3b80
005af510: bl       #0x30ed6c
005af514: mov      fp, r0
005af518: mov      r0, r6
005af51c: bl       #0x30e964
005af520: movw     r1, #0x8081
005af524: movt     r1, #0x3b80
005af528: bl       #0x30ed6c
005af52c: mov      r1, sl
005af530: mov      r3, r0
005af534: mov      r2, fp
005af538: mov      r0, sb
005af53c: bl       #0x30e364
005af540: strb     r7, [r4, #0x204]
005af544: strb     r6, [r4, #0x207]
005af548: strb     r5, [r4, #0x206]
005af54c: strb     r8, [r4, #0x205]
005af550: add      sp, sp, #0xc
005af554: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005af558: movw     r0, #0xbe2
005af55c: bl       #0x30e544
005af560: mov      r3, #1
005af564: strb     r3, [r4, #0x1c4]
005af568: b        #0x5af414
005af56c: eorseq   r0, r3, r8, lsl #24
005af570: eorseq   r0, r3, r8, asr #23

# _ZN6glitch5video12pixel_format7convertENS0_14E_PIXEL_FORMATEPKvjS2_Pvjjjb
005f95ac: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
005f95b0: sub      sp, sp, #0x134
005f95b4: ldr      r5, [pc, #0xea0]
005f95b8: mov      r8, r1
005f95bc: ldrb     r1, [sp, #0x168]
005f95c0: cmp      r2, #0
005f95c4: add      r5, pc, r5
005f95c8: str      r2, [sp, #0x5c]
005f95cc: mov      r7, r3
005f95d0: mov      sl, r0
005f95d4: ldr      sb, [sp, #0x15c]
005f95d8: ldr      r4, [sp, #0x160]
005f95dc: str      r1, [sp, #0x38]
005f95e0: beq      #0x5f9738
005f95e4: cmp      sb, #0
005f95e8: beq      #0x5f9724
005f95ec: cmp      sl, r7
005f95f0: beq      #0x5f97a0
005f95f4: ldr      r6, [sp, #0x158]
005f95f8: cmp      r8, r6
005f95fc: beq      #0x5f9808
005f9600: ldr      fp, [pc, #0xe58]
005f9604: mov      r1, #0x28
005f9608: mul      r6, r1, r7
005f960c: ldr      r2, [r5, fp]
005f9610: ldr      r3, [r2, r6]
005f9614: add      r6, r2, r6
005f9618: tst      r3, #8
005f961c: beq      #0x5f9654
005f9620: uxth     r3, r7
005f9624: cmp      r3, #0x27
005f9628: beq      #0x5f9794
005f962c: mov      r0, #0
005f9630: bl       #0x5ed944
005f9634: ldr      r1, [r0, r7, lsl #2]
005f9638: ldr      r0, [pc, #0xe24]
005f963c: mov      r2, #3
005f9640: add      r0, pc, r0
005f9644: bl       #0x60ace8
005f9648: mov      r0, #0
005f964c: add      sp, sp, #0x134
005f9650: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005f9654: mul      r1, r1, sl
005f9658: ldr      r2, [r2, r1]
005f965c: tst      r2, #8
005f9660: bne      #0x5f97d0
005f9664: tst      r3, #4
005f9668: beq      #0x5f96b4
005f966c: tst      r2, #4
005f9670: bne      #0x5f96b4
005f9674: mov      r0, sl
005f9678: str      r3, [sp, #0x28]
005f967c: bl       #0x5ed954
005f9680: ldrb     r2, [r6, #0x14]
005f9684: ldr      r3, [sp, #0x28]
005f9688: orr      r2, r2, r0, lsl #2
005f968c: sub      r2, r2, #4
005f9690: cmp      r2, #5
005f9694: addls    pc, pc, r2, lsl #2
005f9698: b        #0x5f9aa4
005f969c: b        #0x5f99e0
005f96a0: b        #0x5f9abc
005f96a4: b        #0x5f9aa4
005f96a8: b        #0x5f9aa4
005f96ac: b        #0x5f991c
005f96b0: b        #0x5f986c
005f96b4: ldr      r1, [r5, fp]
005f96b8: mov      r0, #0x28
005f96bc: mla      ip, r0, r7, r1
005f96c0: mla      r1, r0, sl, r1
005f96c4: ldrb     r0, [ip, #0x14]
005f96c8: ldrb     r1, [r1, #0x14]
005f96cc: cmp      r1, r0
005f96d0: beq      #0x5f9748
005f96d4: orr      r1, r2, r3
005f96d8: ands     r1, r1, #2
005f96dc: bne      #0x5f9844
005f96e0: sub      r0, sl, #0xa
005f96e4: cmp      r0, #1
005f96e8: bls      #0x5f9ebc
005f96ec: ldr      lr, [sp, #0x158]
005f96f0: str      r4, [sp, #8]
005f96f4: ldr      r5, [sp, #0x38]
005f96f8: ldr      r4, [sp, #0x164]
005f96fc: mov      r0, sl
005f9700: mov      r1, r8
005f9704: ldr      r2, [sp, #0x5c]
005f9708: mov      r3, r7
005f970c: str      lr, [sp]
005f9710: str      sb, [sp, #4]
005f9714: str      r4, [sp, #0xc]
005f9718: str      r5, [sp, #0x10]
005f971c: bl       #0x5f4fe8
005f9720: b        #0x5f964c
005f9724: mov      r0, r7
005f9728: mov      r1, r4
005f972c: bl       #0x5edaec
005f9730: mov      sb, r0
005f9734: b        #0x5f95ec
005f9738: mov      r1, r4
005f973c: bl       #0x5edaec
005f9740: str      r0, [sp, #0x5c]
005f9744: b        #0x5f95e4
005f9748: tst      r2, #0x40
005f974c: bne      #0x5f96d4
005f9750: tst      r3, #0x40
005f9754: bne      #0x5f96d4
005f9758: tst      r3, #1
005f975c: beq      #0x5f9768
005f9760: tst      r2, #1
005f9764: beq      #0x5f96d4
005f9768: cmp      r7, #2
005f976c: cmpne    sl, #2
005f9770: beq      #0x5f96d4
005f9774: cmp      r1, #4
005f9778: addls    pc, pc, r1, lsl #2
005f977c: b        #0x5f9854
005f9780: b        #0x5f9d34
005f9784: b        #0x5f9c4c
005f9788: b        #0x5f9b6c
005f978c: b        #0x5f9c4c
005f9790: b        #0x5f9b6c
005f9794: ldr      r1, [pc, #0xccc]
005f9798: add      r1, pc, r1
005f979c: b        #0x5f9638
005f97a0: str      r4, [sp, #4]
005f97a4: ldr      r5, [sp, #0x38]
005f97a8: ldr      r4, [sp, #0x164]
005f97ac: mov      r0, sl
005f97b0: mov      r1, r8
005f97b4: ldr      r2, [sp, #0x5c]
005f97b8: ldr      r3, [sp, #0x158]
005f97bc: str      sb, [sp]
005f97c0: str      r4, [sp, #8]
005f97c4: str      r5, [sp, #0xc]
005f97c8: bl       #0x5ee40c
005f97cc: b        #0x5f964c
005f97d0: ldr      lr, [sp, #0x158]
005f97d4: str      r4, [sp, #8]
005f97d8: ldr      r5, [sp, #0x38]
005f97dc: ldr      r4, [sp, #0x164]
005f97e0: mov      r0, sl
005f97e4: mov      r1, r8
005f97e8: ldr      r2, [sp, #0x5c]
005f97ec: mov      r3, r7
005f97f0: str      lr, [sp]
005f97f4: str      sb, [sp, #4]
005f97f8: str      r4, [sp, #0xc]
005f97fc: str      r5, [sp, #0x10]
005f9800: bl       #0x5fd894
005f9804: b        #0x5f964c
005f9808: mov      r1, r4
005f980c: mov      r0, sl
005f9810: bl       #0x5edaec
005f9814: mov      r1, r4
005f9818: mov      r6, r0
005f981c: mov      r0, r7
005f9820: bl       #0x5edaec
005f9824: cmp      r6, r0
005f9828: beq      #0x5f985c
005f982c: ldr      r0, [pc, #0xc38]
005f9830: mov      r1, #3
005f9834: add      r0, pc, r0
005f9838: bl       #0x60aca0
005f983c: mov      r0, #0
005f9840: b        #0x5f964c
005f9844: ldr      r0, [pc, #0xc24]
005f9848: mov      r1, #3
005f984c: add      r0, pc, r0
005f9850: bl       #0x60aca0
005f9854: mov      r0, #0
005f9858: b        #0x5f964c
005f985c: ldr      ip, [sp, #0x5c]
005f9860: cmp      ip, sb
005f9864: bne      #0x5f982c
005f9868: b        #0x5f9600
005f986c: ldr      r3, [r5, fp]
005f9870: mov      r2, #0x28
005f9874: movw     fp, #0x999a
005f9878: mla      sl, r2, sl, r3
005f987c: movw     r3, #0xa3d
005f9880: movt     r3, #0x3f17
005f9884: str      r3, [sp, #0xf4]
005f9888: movw     r3, #0x47ae
005f988c: movt     fp, #0x3e99
005f9890: add      ip, sp, #0xb4
005f9894: movt     r3, #0x3de1
005f9898: add      lr, sp, #0xf0
005f989c: str      r3, [sp, #0xf8]
005f98a0: str      fp, [sp, #0xf0]
005f98a4: mov      r3, fp
005f98a8: mov      r6, ip
005f98ac: mov      r7, ip
005f98b0: mov      r5, #0
005f98b4: str      sl, [sp, #0x3c]
005f98b8: str      lr, [sp, #0x40]
005f98bc: mov      fp, ip
005f98c0: ldr      r0, [sp, #0x3c]
005f98c4: add      r2, r0, r5
005f98c8: ldr      r0, [r2, #4]
005f98cc: ldrb     r2, [sl, #0x1c]
005f98d0: add      sl, sl, #1
005f98d4: str      r0, [r6, #0xc]
005f98d8: strb     r2, [r7, #0x18]
005f98dc: lsr      r0, r0, r2
005f98e0: str      r3, [sp, #0x28]
005f98e4: bl       #0x30e2e0
005f98e8: ldr      r3, [sp, #0x28]
005f98ec: mov      r1, r0
005f98f0: add      r6, r6, #4
005f98f4: mov      r0, r3
005f98f8: bl       #0x30ec94
005f98fc: str      r0, [fp, r5]
005f9900: add      r5, r5, #4
005f9904: cmp      r5, #0xc
005f9908: add      r7, r7, #1
005f990c: beq      #0x5fa2dc
005f9910: ldr      r1, [sp, #0x40]
005f9914: ldr      r3, [r1, r5]
005f9918: b        #0x5f98c0
005f991c: ands     r2, r3, #1
005f9920: mov      r6, r8
005f9924: ldr      r7, [sp, #0x158]
005f9928: bne      #0x5fac60
005f992c: ldr      r1, [r5, fp]
005f9930: mov      r0, #0x28
005f9934: movw     r3, #0x999a
005f9938: mla      sl, r0, sl, r1
005f993c: movw     r1, #0xa3d
005f9940: movt     r1, #0x3f17
005f9944: str      r1, [sp, #0xf4]
005f9948: movw     r1, #0x47ae
005f994c: add      ip, sp, #0xb4
005f9950: movt     r3, #0x3e99
005f9954: movt     r1, #0x3de1
005f9958: add      lr, sp, #0xf0
005f995c: str      r7, [sp, #0x4c]
005f9960: str      r1, [sp, #0xf8]
005f9964: str      r3, [sp, #0xf0]
005f9968: mov      r5, r2
005f996c: str      r3, [sp, #0x3c]
005f9970: mov      fp, ip
005f9974: str      sl, [sp, #0x40]
005f9978: str      lr, [sp, #0x44]
005f997c: str      r8, [sp, #0x48]
005f9980: mov      r6, ip
005f9984: mov      r7, ip
005f9988: ldr      r0, [sp, #0x40]
005f998c: add      r3, r0, r5
005f9990: ldr      r0, [r3, #4]
005f9994: ldrb     r3, [sl, #0x1c]
005f9998: add      sl, sl, #1
005f999c: str      r0, [fp, #0xc]
005f99a0: strb     r3, [r6, #0x18]
005f99a4: lsr      r0, r0, r3
005f99a8: bl       #0x30e2e0
005f99ac: mov      r1, r0
005f99b0: ldr      r0, [sp, #0x3c]
005f99b4: bl       #0x30ec94
005f99b8: str      r0, [r7, r5]
005f99bc: add      r5, r5, #4
005f99c0: cmp      r5, #0xc
005f99c4: add      fp, fp, #4
005f99c8: add      r6, r6, #1
005f99cc: beq      #0x5fa5e8
005f99d0: ldr      r1, [sp, #0x44]
005f99d4: ldr      r1, [r1, r5]
005f99d8: str      r1, [sp, #0x3c]
005f99dc: b        #0x5f9988
005f99e0: ldr      r7, [sp, #0x158]
005f99e4: ands     r3, r3, #1
005f99e8: mov      r6, r8
005f99ec: str      r7, [sp, #0x68]
005f99f0: bne      #0x5fadb4
005f99f4: ldr      r1, [r5, fp]
005f99f8: mov      r0, #0x28
005f99fc: movw     r2, #0x999a
005f9a00: mla      sl, r0, sl, r1
005f9a04: movw     r1, #0xa3d
005f9a08: movt     r1, #0x3f17
005f9a0c: str      r1, [sp, #0xf4]
005f9a10: movw     r1, #0x47ae
005f9a14: add      ip, sp, #0xb4
005f9a18: movt     r2, #0x3e99
005f9a1c: movt     r1, #0x3de1
005f9a20: add      lr, sp, #0xf0
005f9a24: str      r1, [sp, #0xf8]
005f9a28: str      r2, [sp, #0xf0]
005f9a2c: mov      r5, r3
005f9a30: mov      r7, ip
005f9a34: mov      fp, ip
005f9a38: str      sl, [sp, #0x3c]
005f9a3c: str      lr, [sp, #0x40]
005f9a40: str      r8, [sp, #0x44]
005f9a44: mov      r6, ip
005f9a48: ldr      r0, [sp, #0x3c]
005f9a4c: add      r3, r0, r5
005f9a50: ldr      r0, [r3, #4]
005f9a54: ldrb     r3, [sl, #0x1c]
005f9a58: add      sl, sl, #1
005f9a5c: str      r0, [r7, #0xc]
005f9a60: strb     r3, [fp, #0x18]
005f9a64: lsr      r0, r0, r3
005f9a68: str      r2, [sp, #0x2c]
005f9a6c: bl       #0x30e2e0
005f9a70: ldr      r2, [sp, #0x2c]
005f9a74: mov      r1, r0
005f9a78: add      r7, r7, #4
005f9a7c: mov      r0, r2
005f9a80: bl       #0x30ec94
005f9a84: str      r0, [r6, r5]
005f9a88: add      r5, r5, #4
005f9a8c: cmp      r5, #0xc
005f9a90: add      fp, fp, #1
005f9a94: beq      #0x5fa478
005f9a98: ldr      r1, [sp, #0x40]
005f9a9c: ldr      r2, [r1, r5]
005f9aa0: b        #0x5f9a48
005f9aa4: ldr      r0, [pc, #0x9c8]
005f9aa8: mov      r1, #3
005f9aac: add      r0, pc, r0
005f9ab0: bl       #0x60aca0
005f9ab4: mov      r0, #0
005f9ab8: b        #0x5f964c
005f9abc: ldr      r3, [r5, fp]
005f9ac0: mov      r2, #0x28
005f9ac4: movw     fp, #0x999a
005f9ac8: mla      sl, r2, sl, r3
005f9acc: movw     r3, #0xa3d
005f9ad0: movt     r3, #0x3f17
005f9ad4: str      r3, [sp, #0xf4]
005f9ad8: movw     r3, #0x47ae
005f9adc: movt     fp, #0x3e99
005f9ae0: add      ip, sp, #0xb4
005f9ae4: movt     r3, #0x3de1
005f9ae8: add      lr, sp, #0xf0
005f9aec: str      r3, [sp, #0xf8]
005f9af0: str      fp, [sp, #0xf0]
005f9af4: mov      r3, fp
005f9af8: mov      r6, ip
005f9afc: mov      r7, ip
005f9b00: mov      r5, #0
005f9b04: str      sl, [sp, #0x3c]
005f9b08: str      lr, [sp, #0x40]
005f9b0c: mov      fp, ip
005f9b10: ldr      r0, [sp, #0x3c]
005f9b14: add      r2, r0, r5
005f9b18: ldr      r0, [r2, #4]
005f9b1c: ldrb     r2, [sl, #0x1c]
005f9b20: add      sl, sl, #1
005f9b24: str      r0, [r7, #0xc]
005f9b28: strb     r2, [r6, #0x18]
005f9b2c: lsr      r0, r0, r2
005f9b30: str      r3, [sp, #0x28]
005f9b34: bl       #0x30e2e0
005f9b38: ldr      r3, [sp, #0x28]
005f9b3c: mov      r1, r0
005f9b40: add      r7, r7, #4
005f9b44: mov      r0, r3
005f9b48: bl       #0x30ec94
005f9b4c: str      r0, [fp, r5]
005f9b50: add      r5, r5, #4
005f9b54: cmp      r5, #0xc
005f9b58: add      r6, r6, #1
005f9b5c: beq      #0x5fa170
005f9b60: ldr      r1, [sp, #0x40]
005f9b64: ldr      r3, [r1, r5]
005f9b68: b        #0x5f9b10
005f9b6c: ldr      ip, [r5, fp]
005f9b70: mov      fp, #0x28
005f9b74: mov      r5, r8
005f9b78: mla      r3, fp, r7, ip
005f9b7c: ldr      r6, [sp, #0x158]
005f9b80: ldrb     r3, [r3, #0x17]
005f9b84: cmp      r3, #3
005f9b88: beq      #0x5faf10
005f9b8c: cmp      r3, #4
005f9b90: beq      #0x5fafcc
005f9b94: cmp      r3, #2
005f9b98: bne      #0x5f9854
005f9b9c: mov      r1, r7
005f9ba0: mov      r0, sl
005f9ba4: add      r2, sp, #0x10c
005f9ba8: str      ip, [sp, #0x30]
005f9bac: bl       #0x5ed9d0
005f9bb0: ldr      ip, [sp, #0x30]
005f9bb4: ldr      r1, [sp, #0x158]
005f9bb8: mla      ip, fp, sl, ip
005f9bbc: cmp      r8, r1
005f9bc0: ldrb     r7, [ip, #0x15]
005f9bc4: beq      #0x5fca70
005f9bc8: ldr      sl, [sp, #0x38]
005f9bcc: ldr      r0, [sp, #0x164]
005f9bd0: mov      r1, sb
005f9bd4: cmp      sl, #0
005f9bd8: ldrne    ip, [sp, #0x164]
005f9bdc: rsbne    r1, sb, #0
005f9be0: subne    r3, ip, #1
005f9be4: mlane    r6, r3, sb, r6
005f9be8: cmp      r0, #0
005f9bec: beq      #0x5f9eb4
005f9bf0: ldr      sl, [sp, #0x5c]
005f9bf4: mov      r3, r6
005f9bf8: mov      ip, r0
005f9bfc: cmp      r4, #0
005f9c00: movne    r2, r4
005f9c04: beq      #0x5f9c30
005f9c08: ldrb     r0, [sp, #0x10c]
005f9c0c: subs     r2, r2, #1
005f9c10: ldr      r0, [r8, r0, lsl #2]
005f9c14: str      r0, [r3]
005f9c18: ldrb     r0, [sp, #0x10d]
005f9c1c: ldr      r0, [r8, r0, lsl #2]
005f9c20: add      r8, r8, r7
005f9c24: str      r0, [r3, #4]
005f9c28: add      r3, r3, #8
005f9c2c: bne      #0x5f9c08
005f9c30: subs     ip, ip, #1
005f9c34: beq      #0x5f9eb4
005f9c38: add      r5, r5, sl
005f9c3c: add      r6, r6, r1
005f9c40: mov      r3, r6
005f9c44: mov      r8, r5
005f9c48: b        #0x5f9bfc
005f9c4c: ldr      ip, [r5, fp]
005f9c50: mov      fp, #0x28
005f9c54: mov      r5, r8
005f9c58: mla      r3, fp, r7, ip
005f9c5c: ldr      r6, [sp, #0x158]
005f9c60: ldrb     r3, [r3, #0x17]
005f9c64: cmp      r3, #3
005f9c68: beq      #0x5fb2f0
005f9c6c: cmp      r3, #4
005f9c70: beq      #0x5fb218
005f9c74: cmp      r3, #2
005f9c78: bne      #0x5f9854
005f9c7c: mov      r1, r7
005f9c80: mov      r0, sl
005f9c84: add      r2, sp, #0x10c
005f9c88: str      ip, [sp, #0x30]
005f9c8c: bl       #0x5ed9d0
005f9c90: ldr      ip, [sp, #0x30]
005f9c94: ldr      r1, [sp, #0x158]
005f9c98: mla      ip, fp, sl, ip
005f9c9c: cmp      r8, r1
005f9ca0: ldrb     r7, [ip, #0x15]
005f9ca4: beq      #0x5fc518
005f9ca8: ldr      sl, [sp, #0x38]
005f9cac: ldr      r0, [sp, #0x164]
005f9cb0: mov      r1, sb
005f9cb4: cmp      sl, #0
005f9cb8: ldrne    ip, [sp, #0x164]
005f9cbc: rsbne    r1, sb, #0
005f9cc0: subne    r3, ip, #1
005f9cc4: mlane    r6, r3, sb, r6
005f9cc8: cmp      r0, #0
005f9ccc: beq      #0x5f9eb4
005f9cd0: ldr      sl, [sp, #0x5c]
005f9cd4: ldr      ip, [sp, #0x164]
005f9cd8: mov      r0, r6
005f9cdc: cmp      r4, #0
005f9ce0: movne    r3, r4
005f9ce4: beq      #0x5f9d18
005f9ce8: ldrb     r2, [sp, #0x10c]
005f9cec: subs     r3, r3, #1
005f9cf0: lsl      r2, r2, #1
005f9cf4: ldrh     r2, [r5, r2]
005f9cf8: strh     r2, [r6]
005f9cfc: ldrb     r2, [sp, #0x10d]
005f9d00: lsl      r2, r2, #1
005f9d04: ldrh     r2, [r5, r2]
005f9d08: add      r5, r5, r7
005f9d0c: strh     r2, [r6, #2]
005f9d10: add      r6, r6, #4
005f9d14: bne      #0x5f9ce8
005f9d18: subs     ip, ip, #1
005f9d1c: beq      #0x5f9eb4
005f9d20: add      r5, r8, sl
005f9d24: add      r0, r0, r1
005f9d28: mov      r6, r0
005f9d2c: mov      r8, r5
005f9d30: b        #0x5f9cdc
005f9d34: ldr      ip, [r5, fp]
005f9d38: mov      fp, #0x28
005f9d3c: mov      r5, r8
005f9d40: mla      r3, fp, r7, ip
005f9d44: ldr      r6, [sp, #0x158]
005f9d48: ldrb     r3, [r3, #0x17]
005f9d4c: cmp      r3, #3
005f9d50: beq      #0x5fb15c
005f9d54: cmp      r3, #4
005f9d58: beq      #0x5fb094
005f9d5c: cmp      r3, #2
005f9d60: bne      #0x5f9854
005f9d64: mov      r1, r7
005f9d68: mov      r0, sl
005f9d6c: add      r2, sp, #0x12c
005f9d70: str      ip, [sp, #0x30]
005f9d74: bl       #0x5ed9d0
005f9d78: ldr      ip, [sp, #0x30]
005f9d7c: ldr      r0, [sp, #0x158]
005f9d80: mla      ip, fp, sl, ip
005f9d84: cmp      r8, r0
005f9d88: ldrb     r7, [ip, #0x15]
005f9d8c: beq      #0x5fc47c
005f9d90: ldr      sl, [sp, #0x38]
005f9d94: ldr      r0, [sp, #0x164]
005f9d98: mov      r1, sb
005f9d9c: cmp      sl, #0
005f9da0: ldrne    ip, [sp, #0x164]
005f9da4: rsbne    r1, sb, #0
005f9da8: subne    r3, ip, #1
005f9dac: mlane    r6, r3, sb, r6
005f9db0: cmp      r0, #0
005f9db4: beq      #0x5f9eb4
005f9db8: ldr      sl, [sp, #0x5c]
005f9dbc: ldr      ip, [sp, #0x164]
005f9dc0: mov      r0, r6
005f9dc4: cmp      r4, #0
005f9dc8: movne    r3, r4
005f9dcc: beq      #0x5f9df8
005f9dd0: ldrb     r2, [sp, #0x12c]
005f9dd4: subs     r3, r3, #1
005f9dd8: ldrb     r2, [r5, r2]
005f9ddc: strb     r2, [r6]
005f9de0: ldrb     r2, [sp, #0x12d]
005f9de4: ldrb     r2, [r5, r2]
005f9de8: add      r5, r5, r7
005f9dec: strb     r2, [r6, #1]
005f9df0: add      r6, r6, #2
005f9df4: bne      #0x5f9dd0
005f9df8: subs     ip, ip, #1
005f9dfc: beq      #0x5f9eb4
005f9e00: add      r5, r8, sl
005f9e04: add      r0, r0, r1
005f9e08: mov      r6, r0
005f9e0c: mov      r8, r5
005f9e10: b        #0x5f9dc4
005f9e14: ldr      r3, [sp, #0x164]
005f9e18: ldr      r8, [sp, #0x158]
005f9e1c: sub      r6, r3, #1
005f9e20: mla      r6, r6, sb, r8
005f9e24: rsb      sb, sb, #0
005f9e28: cmp      r8, r6
005f9e2c: str      sb, [sp, #0x54]
005f9e30: addls    sl, sp, #0x10c
005f9e34: movls    r8, r4
005f9e38: bhi      #0x5f9eb4
005f9e3c: cmp      r8, #0
005f9e40: mov      sb, r6
005f9e44: mov      fp, r5
005f9e48: movne    r4, r8
005f9e4c: beq      #0x5f9e9c
005f9e50: ldrb     r3, [sp, #0x12c]
005f9e54: ldrb     ip, [sp, #0x12d]
005f9e58: mov      r0, r5
005f9e5c: ldrb     lr, [r6, r3]
005f9e60: mov      r1, sl
005f9e64: mov      r2, r7
005f9e68: strb     lr, [sp, #0x10c]
005f9e6c: ldrb     ip, [r6, ip]
005f9e70: strb     ip, [sp, #0x10d]
005f9e74: ldrb     r3, [r5, r3]
005f9e78: strb     r3, [r6]
005f9e7c: ldrb     r3, [sp, #0x12d]
005f9e80: ldrb     r3, [r5, r3]
005f9e84: add      r5, r5, r7
005f9e88: strb     r3, [r6, #1]
005f9e8c: bl       #0x30e868
005f9e90: subs     r4, r4, #1
005f9e94: add      r6, r6, #2
005f9e98: bne      #0x5f9e50
005f9e9c: ldr      ip, [sp, #0x5c]
005f9ea0: ldr      r0, [sp, #0x54]
005f9ea4: add      r5, fp, ip
005f9ea8: add      r6, sb, r0
005f9eac: cmp      r5, r6
005f9eb0: bls      #0x5f9e3c
005f9eb4: mov      r0, #1
005f9eb8: b        #0x5f964c
005f9ebc: mov      r0, r7
005f9ec0: str      r1, [sp, #0x30]
005f9ec4: str      r2, [sp, #0x2c]
005f9ec8: str      r3, [sp, #0x28]
005f9ecc: bl       #0x5ed954
005f9ed0: cmp      r0, #1
005f9ed4: mov      r6, r8
005f9ed8: ldr      r1, [sp, #0x30]
005f9edc: ldr      r2, [sp, #0x2c]
005f9ee0: ldr      r3, [sp, #0x28]
005f9ee4: beq      #0x5fa9d8
005f9ee8: cmp      r0, #2
005f9eec: beq      #0x5fa750
005f9ef0: cmp      r0, #0
005f9ef4: movne    r0, r1
005f9ef8: bne      #0x5f964c
005f9efc: ldr      r1, [sp, #0x158]
005f9f00: ldr      r0, [r5, fp]
005f9f04: str      r1, [sp, #0xa8]
005f9f08: mov      r1, #0x28
005f9f0c: mla      r1, r1, r7, r0
005f9f10: ldrb     r1, [r1, #0x19]
005f9f14: cmp      r1, #8
005f9f18: bhi      #0x5fb794
005f9f1c: tst      r3, #1
005f9f20: bne      #0x5fbf74
005f9f24: mov      r0, #0
005f9f28: str      r0, [sp, #0x3c]
005f9f2c: ldr      r3, [r5, fp]
005f9f30: mov      r0, #0x28
005f9f34: add      ip, sp, #0xb4
005f9f38: mla      r7, r0, r7, r3
005f9f3c: str      sl, [sp, #0x44]
005f9f40: mla      r0, r0, sl, r3
005f9f44: str      sb, [sp, #0x4c]
005f9f48: mov      r1, ip
005f9f4c: mov      r2, #0
005f9f50: str      r6, [sp, #0x40]
005f9f54: str      r8, [sp, #0x48]
005f9f58: str      r4, [sp, #0x50]
005f9f5c: mov      sb, r5
005f9f60: mov      sl, r7
005f9f64: add      r5, sl, r2
005f9f68: ldrb     r3, [r0, #0x18]
005f9f6c: ldrb     r4, [r7, #0x18]
005f9f70: ldr      r8, [r5, #4]
005f9f74: ldrb     r6, [r7, #0x1c]
005f9f78: ldrb     r5, [r0, #0x1c]
005f9f7c: cmp      r3, r4
005f9f80: str      r8, [ip, r2]
005f9f84: strb     r5, [r1, #0x10]
005f9f88: strb     r6, [r1, #0x14]
005f9f8c: bls      #0x5fa448
005f9f90: add      r3, r3, r5
005f9f94: rsb      r3, r4, r3
005f9f98: strb     r3, [r1, #0x10]
005f9f9c: add      r2, r2, #4
005f9fa0: cmp      r2, #0x10
005f9fa4: add      r0, r0, #1
005f9fa8: add      r1, r1, #1
005f9fac: add      r7, r7, #1
005f9fb0: bne      #0x5f9f64
005f9fb4: mov      r5, sb
005f9fb8: ldr      r3, [sp, #0xc0]
005f9fbc: ldr      r1, [r5, fp]
005f9fc0: ldr      sl, [sp, #0x44]
005f9fc4: ldr      r6, [sp, #0x40]
005f9fc8: str      r3, [sp, #0x40]
005f9fcc: mov      r3, #0x28
005f9fd0: mla      r3, r3, sl, r1
005f9fd4: ldr      r7, [sp, #0x3c]
005f9fd8: ldr      r8, [sp, #0x48]
005f9fdc: ldr      r5, [sp, #0x158]
005f9fe0: ldr      sl, [sp, #0x40]
005f9fe4: add      r2, r3, r2
005f9fe8: cmp      r8, r5
005f9fec: and      r7, r7, sl
005f9ff0: ldr      r4, [sp, #0x50]
005f9ff4: ldr      sb, [sp, #0x4c]
005f9ff8: str      r7, [sp, #0x3c]
005f9ffc: ldrb     fp, [r2, #5]
005fa000: beq      #0x5fc314
005fa004: ldr      r0, [sp, #0x38]
005fa008: str      sb, [sp, #0x68]
005fa00c: cmp      r0, #0
005fa010: beq      #0x5fa030
005fa014: ldr      r1, [sp, #0x164]
005fa018: ldr      r2, [sp, #0x158]
005fa01c: sub      r3, r1, #1
005fa020: mla      r3, r3, sb, r2
005fa024: str      r3, [sp, #0xa8]
005fa028: rsb      r3, sb, #0
005fa02c: str      r3, [sp, #0x68]
005fa030: ldr      r5, [sp, #0x164]
005fa034: cmp      r5, #0
005fa038: beq      #0x5f9eb4
005fa03c: ldr      ip, [sp, #0xb8]
005fa040: ldrb     r0, [sp, #0xc6]
005fa044: ldrb     r5, [sp, #0xc9]
005fa048: str      ip, [sp, #0x50]
005fa04c: str      r0, [sp, #0x4c]
005fa050: ldrb     r1, [sp, #0xca]
005fa054: ldr      r0, [sp, #0xa8]
005fa058: ldr      r2, [sp, #0xbc]
005fa05c: ldrb     r3, [sp, #0xc7]
005fa060: ldrb     ip, [sp, #0xcb]
005fa064: str      r8, [sp, #0x64]
005fa068: ldrb     sb, [sp, #0xc4]
005fa06c: ldrb     sl, [sp, #0xc8]
005fa070: ldr      r8, [sp, #0xb4]
005fa074: ldrb     r7, [sp, #0xc5]
005fa078: str      r5, [sp, #0x58]
005fa07c: str      r1, [sp, #0x48]
005fa080: str      r2, [sp, #0x44]
005fa084: str      r3, [sp, #0x38]
005fa088: str      ip, [sp, #0x34]
005fa08c: str      r0, [sp, #0x60]
005fa090: mov      r5, r0
005fa094: cmp      r4, #0
005fa098: movne    r2, #0
005fa09c: strne    r5, [sp, #0x54]
005fa0a0: strne    r4, [sp, #0x6c]
005fa0a4: beq      #0x5fa138
005fa0a8: ldrb     r3, [r6]
005fa0ac: ldr      r4, [sp, #0x4c]
005fa0b0: ldr      r5, [sp, #0x50]
005fa0b4: strb     r3, [sp, #0x110]
005fa0b8: ldrb     r3, [r6, #1]
005fa0bc: strb     r3, [sp, #0x111]
005fa0c0: ldrb     r3, [r6, #2]
005fa0c4: add      r6, fp, r6
005fa0c8: strb     r3, [sp, #0x112]
005fa0cc: ldr      r3, [sp, #0x110]
005fa0d0: lsr      ip, r3, r4
005fa0d4: ldr      r4, [sp, #0x58]
005fa0d8: lsr      r0, r3, r7
005fa0dc: and      r0, r5, r0, lsl r4
005fa0e0: ldr      r5, [sp, #0x38]
005fa0e4: ldr      r4, [sp, #0x44]
005fa0e8: lsr      r1, r3, sb
005fa0ec: lsr      r3, r3, r5
005fa0f0: ldr      r5, [sp, #0x48]
005fa0f4: and      r1, r8, r1, lsl sl
005fa0f8: and      ip, r4, ip, lsl r5
005fa0fc: ldr      r4, [sp, #0x40]
005fa100: ldr      r5, [sp, #0x34]
005fa104: orr      r1, r0, r1
005fa108: orr      ip, r1, ip
005fa10c: and      r3, r4, r3, lsl r5
005fa110: orr      r3, ip, r3
005fa114: ldr      ip, [sp, #0x3c]
005fa118: ldr      r0, [sp, #0x54]
005fa11c: orr      r3, r3, ip
005fa120: strb     r3, [r0, r2]
005fa124: ldr      r1, [sp, #0x6c]
005fa128: add      r2, r2, #1
005fa12c: cmp      r1, r2
005fa130: bne      #0x5fa0a8
005fa134: mov      r4, r1
005fa138: ldr      r2, [sp, #0x164]
005fa13c: subs     r2, r2, #1
005fa140: str      r2, [sp, #0x164]
005fa144: beq      #0x5f9eb4
005fa148: ldr      r3, [sp, #0x64]
005fa14c: ldr      r5, [sp, #0x5c]
005fa150: ldr      ip, [sp, #0x60]
005fa154: ldr      r0, [sp, #0x68]
005fa158: add      r6, r3, r5
005fa15c: str      r6, [sp, #0x64]
005fa160: add      ip, ip, r0
005fa164: str      ip, [sp, #0x60]
005fa168: mov      r5, ip
005fa16c: b        #0x5fa094
005fa170: ldr      r2, [sp, #0x158]
005fa174: ldr      r3, [sp, #0x3c]
005fa178: str      r8, [sp, #0x58]
005fa17c: cmp      r8, r2
005fa180: ldrb     r5, [r3, #0x15]
005fa184: mov      fp, r2
005fa188: beq      #0x5fbf88
005fa18c: ldr      sl, [sp, #0x38]
005fa190: str      sb, [sp, #0x64]
005fa194: cmp      sl, #0
005fa198: beq      #0x5fa1b0
005fa19c: ldr      ip, [sp, #0x164]
005fa1a0: rsb      r0, sb, #0
005fa1a4: str      r0, [sp, #0x64]
005fa1a8: sub      r3, ip, #1
005fa1ac: mla      fp, r3, sb, r2
005fa1b0: ldr      r1, [sp, #0x164]
005fa1b4: cmp      r1, #0
005fa1b8: beq      #0x5f9eb4
005fa1bc: ldrb     r2, [sp, #0xcc]
005fa1c0: ldr      r3, [sp, #0xb4]
005fa1c4: ldr      r6, [sp, #0xc4]
005fa1c8: ldrb     r7, [sp, #0xcd]
005fa1cc: ldr      sl, [sp, #0xb8]
005fa1d0: ldr      ip, [sp, #0xc8]
005fa1d4: ldrb     r0, [sp, #0xce]
005fa1d8: ldr      r1, [sp, #0xbc]
005fa1dc: ldr      sb, [sp, #0xc0]
005fa1e0: str      r2, [sp, #0x50]
005fa1e4: str      r3, [sp, #0x4c]
005fa1e8: str      r6, [sp, #0x48]
005fa1ec: str      r7, [sp, #0x44]
005fa1f0: str      sl, [sp, #0x40]
005fa1f4: str      ip, [sp, #0x3c]
005fa1f8: str      r0, [sp, #0x38]
005fa1fc: str      r1, [sp, #0x34]
005fa200: str      r4, [sp, #0x60]
005fa204: ldr      r7, [sp, #0x60]
005fa208: cmp      r7, #0
005fa20c: beq      #0x5fa2ac
005fa210: ldr      r7, [sp, #0x60]
005fa214: mov      r6, #0
005fa218: ldrh     r4, [r8], r5
005fa21c: ldr      r2, [sp, #0x50]
005fa220: and      r0, r4, sb
005fa224: lsr      r0, r0, r2
005fa228: bl       #0x30e2e0
005fa22c: ldr      r1, [sp, #0x4c]
005fa230: bl       #0x30ed6c
005fa234: ldr      r3, [sp, #0x48]
005fa238: ldr      ip, [sp, #0x44]
005fa23c: mov      sl, r0
005fa240: and      r0, r4, r3
005fa244: lsr      r0, r0, ip
005fa248: bl       #0x30e2e0
005fa24c: ldr      r1, [sp, #0x40]
005fa250: bl       #0x30ed6c
005fa254: mov      r1, r0
005fa258: mov      r0, sl
005fa25c: bl       #0x30eba4
005fa260: ldr      lr, [sp, #0x3c]
005fa264: ldr      r1, [sp, #0x38]
005fa268: mov      sl, r0
005fa26c: and      r0, r4, lr
005fa270: lsr      r0, r0, r1
005fa274: bl       #0x30e2e0
005fa278: ldr      r1, [sp, #0x34]
005fa27c: bl       #0x30ed6c
005fa280: mov      r1, r0
005fa284: mov      r0, sl
005fa288: bl       #0x30eba4
005fa28c: movw     r1, #0xff00
005fa290: movt     r1, #0x477f
005fa294: bl       #0x30ed6c
005fa298: bl       #0x8be2a0
005fa29c: subs     r7, r7, #1
005fa2a0: strh     r0, [fp, r6]
005fa2a4: add      r6, r6, #2
005fa2a8: bne      #0x5fa218
005fa2ac: ldr      r2, [sp, #0x164]
005fa2b0: subs     r2, r2, #1
005fa2b4: str      r2, [sp, #0x164]
005fa2b8: beq      #0x5f9eb4
005fa2bc: ldr      r3, [sp, #0x58]
005fa2c0: ldr      r4, [sp, #0x5c]
005fa2c4: ldr      r6, [sp, #0x64]
005fa2c8: add      r3, r3, r4
005fa2cc: str      r3, [sp, #0x58]
005fa2d0: add      fp, fp, r6
005fa2d4: mov      r8, r3
005fa2d8: b        #0x5fa204
005fa2dc: ldr      r2, [sp, #0x38]
005fa2e0: ldr      r3, [sp, #0x3c]
005fa2e4: ldr      sl, [sp, #0x158]
005fa2e8: cmp      r2, #0
005fa2ec: ldrb     fp, [r3, #0x15]
005fa2f0: str      sb, [sp, #0x68]
005fa2f4: beq      #0x5fa30c
005fa2f8: ldr      r5, [sp, #0x164]
005fa2fc: rsb      r6, sb, #0
005fa300: str      r6, [sp, #0x68]
005fa304: sub      r3, r5, #1
005fa308: mla      sl, r3, sb, sl
005fa30c: ldr      r7, [sp, #0x164]
005fa310: cmp      r7, #0
005fa314: beq      #0x5f9eb4
005fa318: ldrb     ip, [sp, #0xcc]
005fa31c: ldr      r0, [sp, #0xb4]
005fa320: ldr      r1, [sp, #0xc4]
005fa324: ldrb     r2, [sp, #0xcd]
005fa328: ldr      r3, [sp, #0xb8]
005fa32c: ldr      r5, [sp, #0xc8]
005fa330: ldrb     r6, [sp, #0xce]
005fa334: ldr      r7, [sp, #0xbc]
005fa338: ldr      sb, [sp, #0xc0]
005fa33c: str      ip, [sp, #0x50]
005fa340: str      r0, [sp, #0x4c]
005fa344: str      r1, [sp, #0x48]
005fa348: str      r2, [sp, #0x44]
005fa34c: str      r3, [sp, #0x40]
005fa350: str      r5, [sp, #0x3c]
005fa354: str      r6, [sp, #0x38]
005fa358: str      r7, [sp, #0x34]
005fa35c: str      sl, [sp, #0x58]
005fa360: str      r8, [sp, #0x64]
005fa364: str      r4, [sp, #0x60]
005fa368: ldr      ip, [sp, #0x60]
005fa36c: cmp      ip, #0
005fa370: beq      #0x5fa410
005fa374: ldr      r6, [sp, #0x60]
005fa378: mov      r5, #0
005fa37c: ldr      r4, [r8], fp
005fa380: ldr      ip, [sp, #0x50]
005fa384: and      r0, r4, sb
005fa388: lsr      r0, r0, ip
005fa38c: bl       #0x30e2e0
005fa390: ldr      r1, [sp, #0x4c]
005fa394: bl       #0x30ed6c
005fa398: ldr      lr, [sp, #0x48]
005fa39c: ldr      r1, [sp, #0x44]
005fa3a0: mov      r7, r0
005fa3a4: and      r0, r4, lr
005fa3a8: lsr      r0, r0, r1
005fa3ac: bl       #0x30e2e0
005fa3b0: ldr      r1, [sp, #0x40]
005fa3b4: bl       #0x30ed6c
005fa3b8: mov      r1, r0
005fa3bc: mov      r0, r7
005fa3c0: bl       #0x30eba4
005fa3c4: ldr      r2, [sp, #0x3c]
005fa3c8: ldr      r3, [sp, #0x38]
005fa3cc: mov      r7, r0
005fa3d0: and      r0, r4, r2
005fa3d4: lsr      r0, r0, r3
005fa3d8: bl       #0x30e2e0
005fa3dc: ldr      r1, [sp, #0x34]
005fa3e0: bl       #0x30ed6c
005fa3e4: mov      r1, r0
005fa3e8: mov      r0, r7
005fa3ec: bl       #0x30eba4
005fa3f0: movw     r1, #0xff00
005fa3f4: movt     r1, #0x477f
005fa3f8: bl       #0x30ed6c
005fa3fc: bl       #0x8be2a0
005fa400: subs     r6, r6, #1
005fa404: strh     r0, [sl, r5]
005fa408: add      r5, r5, #2
005fa40c: bne      #0x5fa37c
005fa410: ldr      r4, [sp, #0x164]
005fa414: subs     r4, r4, #1
005fa418: str      r4, [sp, #0x164]
005fa41c: beq      #0x5f9eb4
005fa420: ldr      r5, [sp, #0x64]
005fa424: ldr      r7, [sp, #0x58]
005fa428: ldr      sl, [sp, #0x68]
005fa42c: ldr      r6, [sp, #0x5c]
005fa430: add      r7, r7, sl
005fa434: add      r8, r5, r6
005fa438: str      r7, [sp, #0x58]
005fa43c: mov      sl, r7
005fa440: str      r8, [sp, #0x64]
005fa444: b        #0x5fa368
005fa448: cmp      r4, r3, lsl #1
005fa44c: addle    r4, r4, r6
005fa450: rsble    r3, r3, r4
005fa454: strble   r3, [r1, #0x14]
005fa458: b        #0x5f9f9c
005fa45c: eorseq   fp, sb, ip, asr #9
005fa460: andeq    r1, r0, r4, lsr pc
005fa464: eoreq    sl, lr, r0, lsl #16
005fa468: eoreq    ip, ip, r8, asr #25
005fa46c: eoreq    sl, lr, ip, asr #11
005fa470: eoreq    sl, lr, r4, ror #12
005fa474: eoreq    sl, lr, r4, asr #7
005fa478: ldr      r2, [sp, #0x38]
005fa47c: ldr      r3, [sp, #0x3c]
005fa480: ldr      r6, [sp, #0x44]
005fa484: cmp      r2, #0
005fa488: ldrb     sl, [r3, #0x15]
005fa48c: str      sb, [sp, #0x64]
005fa490: beq      #0x5fa4b0
005fa494: ldr      r5, [sp, #0x164]
005fa498: ldr      r7, [sp, #0x158]
005fa49c: rsb      ip, sb, #0
005fa4a0: sub      r3, r5, #1
005fa4a4: mla      r3, r3, sb, r7
005fa4a8: str      ip, [sp, #0x64]
005fa4ac: str      r3, [sp, #0x68]
005fa4b0: ldr      r0, [sp, #0x164]
005fa4b4: cmp      r0, #0
005fa4b8: beq      #0x5f9eb4
005fa4bc: ldrb     sb, [sp, #0xcc]
005fa4c0: ldr      r0, [sp, #0x68]
005fa4c4: str      r8, [sp, #0x60]
005fa4c8: ldr      r1, [sp, #0xb4]
005fa4cc: ldr      r2, [sp, #0xc4]
005fa4d0: ldrb     r3, [sp, #0xcd]
005fa4d4: ldr      r5, [sp, #0xb8]
005fa4d8: ldr      r7, [sp, #0xc8]
005fa4dc: ldrb     r8, [sp, #0xce]
005fa4e0: ldr      ip, [sp, #0xbc]
005fa4e4: ldr      fp, [sp, #0xc0]
005fa4e8: str      sb, [sp, #0x50]
005fa4ec: str      r1, [sp, #0x4c]
005fa4f0: str      r2, [sp, #0x48]
005fa4f4: str      r3, [sp, #0x44]
005fa4f8: str      r5, [sp, #0x40]
005fa4fc: str      r7, [sp, #0x3c]
005fa500: str      r8, [sp, #0x38]
005fa504: str      ip, [sp, #0x34]
005fa508: str      r0, [sp, #0x58]
005fa50c: mov      sb, r0
005fa510: cmp      r4, #0
005fa514: movne    r5, #0
005fa518: beq      #0x5fa5b0
005fa51c: ldrh     r7, [r6], sl
005fa520: ldr      r1, [sp, #0x50]
005fa524: and      r0, r7, fp
005fa528: lsr      r0, r0, r1
005fa52c: bl       #0x30e2e0
005fa530: ldr      r1, [sp, #0x4c]
005fa534: bl       #0x30ed6c
005fa538: ldr      r2, [sp, #0x48]
005fa53c: ldr      r3, [sp, #0x44]
005fa540: mov      r8, r0
005fa544: and      r0, r7, r2
005fa548: lsr      r0, r0, r3
005fa54c: bl       #0x30e2e0
005fa550: ldr      r1, [sp, #0x40]
005fa554: bl       #0x30ed6c
005fa558: mov      r1, r0
005fa55c: mov      r0, r8
005fa560: bl       #0x30eba4
005fa564: ldr      ip, [sp, #0x3c]
005fa568: ldr      lr, [sp, #0x38]
005fa56c: mov      r8, r0
005fa570: and      r0, r7, ip
005fa574: lsr      r0, r0, lr
005fa578: bl       #0x30e2e0
005fa57c: ldr      r1, [sp, #0x34]
005fa580: bl       #0x30ed6c
005fa584: mov      r1, r0
005fa588: mov      r0, r8
005fa58c: bl       #0x30eba4
005fa590: mov      r1, #0x43000000
005fa594: add      r1, r1, #0x7f0000
005fa598: bl       #0x30ed6c
005fa59c: bl       #0x8be2a0
005fa5a0: strb     r0, [sb, r5]
005fa5a4: add      r5, r5, #1
005fa5a8: cmp      r4, r5
005fa5ac: bne      #0x5fa51c
005fa5b0: ldr      r0, [sp, #0x164]
005fa5b4: subs     r0, r0, #1
005fa5b8: str      r0, [sp, #0x164]
005fa5bc: beq      #0x5f9eb4
005fa5c0: ldr      r1, [sp, #0x60]
005fa5c4: ldr      r3, [sp, #0x58]
005fa5c8: ldr      r2, [sp, #0x5c]
005fa5cc: ldr      r5, [sp, #0x64]
005fa5d0: add      r6, r1, r2
005fa5d4: add      r3, r3, r5
005fa5d8: str      r3, [sp, #0x58]
005fa5dc: mov      sb, r3
005fa5e0: str      r6, [sp, #0x60]
005fa5e4: b        #0x5fa510
005fa5e8: ldr      r2, [sp, #0x38]
005fa5ec: ldr      r3, [sp, #0x40]
005fa5f0: ldr      r6, [sp, #0x48]
005fa5f4: cmp      r2, #0
005fa5f8: ldr      r7, [sp, #0x4c]
005fa5fc: ldrb     sl, [r3, #0x15]
005fa600: str      sb, [sp, #0x64]
005fa604: beq      #0x5fa620
005fa608: ldr      r5, [sp, #0x164]
005fa60c: ldr      ip, [sp, #0x158]
005fa610: rsb      r0, sb, #0
005fa614: sub      r7, r5, #1
005fa618: mla      r7, r7, sb, ip
005fa61c: str      r0, [sp, #0x64]
005fa620: ldr      r1, [sp, #0x164]
005fa624: cmp      r1, #0
005fa628: beq      #0x5f9eb4
005fa62c: ldrb     r2, [sp, #0xcc]
005fa630: str      r8, [sp, #0x60]
005fa634: ldr      r3, [sp, #0xb4]
005fa638: str      r2, [sp, #0x50]
005fa63c: ldr      r5, [sp, #0xc4]
005fa640: ldrb     r8, [sp, #0xcd]
005fa644: ldr      ip, [sp, #0xb8]
005fa648: ldr      r0, [sp, #0xc8]
005fa64c: ldrb     r1, [sp, #0xce]
005fa650: ldr      r2, [sp, #0xbc]
005fa654: ldr      fp, [sp, #0xc0]
005fa658: str      r3, [sp, #0x4c]
005fa65c: str      r5, [sp, #0x48]
005fa660: str      r8, [sp, #0x44]
005fa664: str      ip, [sp, #0x40]
005fa668: str      r0, [sp, #0x3c]
005fa66c: str      r1, [sp, #0x38]
005fa670: str      r2, [sp, #0x34]
005fa674: str      r7, [sp, #0x58]
005fa678: cmp      r4, #0
005fa67c: movne    r5, #0
005fa680: beq      #0x5fa718
005fa684: ldr      r8, [r6], sl
005fa688: ldr      r3, [sp, #0x50]
005fa68c: and      r0, r8, fp
005fa690: lsr      r0, r0, r3
005fa694: bl       #0x30e2e0
005fa698: ldr      r1, [sp, #0x4c]
005fa69c: bl       #0x30ed6c
005fa6a0: ldr      ip, [sp, #0x48]
005fa6a4: ldr      lr, [sp, #0x44]
005fa6a8: mov      sb, r0
005fa6ac: and      r0, r8, ip
005fa6b0: lsr      r0, r0, lr
005fa6b4: bl       #0x30e2e0
005fa6b8: ldr      r1, [sp, #0x40]
005fa6bc: bl       #0x30ed6c
005fa6c0: mov      r1, r0
005fa6c4: mov      r0, sb
005fa6c8: bl       #0x30eba4
005fa6cc: ldr      r1, [sp, #0x3c]
005fa6d0: ldr      r2, [sp, #0x38]
005fa6d4: mov      sb, r0
005fa6d8: and      r0, r8, r1
005fa6dc: lsr      r0, r0, r2
005fa6e0: bl       #0x30e2e0
005fa6e4: ldr      r1, [sp, #0x34]
005fa6e8: bl       #0x30ed6c
005fa6ec: mov      r1, r0
005fa6f0: mov      r0, sb
005fa6f4: bl       #0x30eba4
005fa6f8: mov      r1, #0x43000000
005fa6fc: add      r1, r1, #0x7f0000
005fa700: bl       #0x30ed6c
005fa704: bl       #0x8be2a0
005fa708: strb     r0, [r7, r5]
005fa70c: add      r5, r5, #1
005fa710: cmp      r4, r5
005fa714: bne      #0x5fa684
005fa718: ldr      r3, [sp, #0x164]
005fa71c: subs     r3, r3, #1
005fa720: str      r3, [sp, #0x164]
005fa724: beq      #0x5f9eb4
005fa728: ldr      r5, [sp, #0x60]
005fa72c: ldr      r7, [sp, #0x5c]
005fa730: ldr      r8, [sp, #0x58]
005fa734: ldr      ip, [sp, #0x64]
005fa738: add      r6, r5, r7
005fa73c: str      r6, [sp, #0x60]
005fa740: add      r8, r8, ip
005fa744: str      r8, [sp, #0x58]
005fa748: mov      r7, r8
005fa74c: b        #0x5fa678
005fa750: ldr      r1, [sp, #0x158]
005fa754: ldr      r0, [r5, fp]
005fa758: str      r1, [sp, #0xa0]
005fa75c: mov      r1, #0x28
005fa760: mla      r1, r1, r7, r0
005fa764: ldrb     r1, [r1, #0x19]
005fa768: cmp      r1, #8
005fa76c: bhi      #0x5fbb70
005fa770: tst      r3, #1
005fa774: bne      #0x5fbf4c
005fa778: mov      r1, #0
005fa77c: str      r1, [sp, #0x40]
005fa780: ldr      r3, [r5, fp]
005fa784: mov      r0, #0x28
005fa788: add      ip, sp, #0xb4
005fa78c: mla      r7, r0, r7, r3
005fa790: str      sl, [sp, #0x44]
005fa794: mla      r0, r0, sl, r3
005fa798: str      sb, [sp, #0x4c]
005fa79c: mov      r1, ip
005fa7a0: mov      r2, #0
005fa7a4: str      r6, [sp, #0x3c]
005fa7a8: str      r8, [sp, #0x48]
005fa7ac: str      r4, [sp, #0x50]
005fa7b0: mov      sb, r5
005fa7b4: mov      sl, r7
005fa7b8: add      r5, sl, r2
005fa7bc: ldrb     r3, [r0, #0x18]
005fa7c0: ldrb     r4, [r7, #0x18]
005fa7c4: ldr      r8, [r5, #4]
005fa7c8: ldrb     r6, [r7, #0x1c]
005fa7cc: ldrb     r5, [r0, #0x1c]
005fa7d0: cmp      r3, r4
005fa7d4: str      r8, [ip, r2]
005fa7d8: strb     r5, [r1, #0x10]
005fa7dc: strb     r6, [r1, #0x14]
005fa7e0: bls      #0x5fa9c4
005fa7e4: add      r3, r3, r5
005fa7e8: rsb      r3, r4, r3
005fa7ec: strb     r3, [r1, #0x10]
005fa7f0: add      r2, r2, #4
005fa7f4: cmp      r2, #0x10
005fa7f8: add      r0, r0, #1
005fa7fc: add      r1, r1, #1
005fa800: add      r7, r7, #1
005fa804: bne      #0x5fa7b8
005fa808: mov      r5, sb
005fa80c: ldr      r3, [sp, #0xc0]
005fa810: ldr      sl, [sp, #0x44]
005fa814: ldr      r1, [r5, fp]
005fa818: ldr      r6, [sp, #0x3c]
005fa81c: str      r3, [sp, #0x3c]
005fa820: mov      r3, #0x28
005fa824: mla      r3, r3, sl, r1
005fa828: ldr      r5, [sp, #0x38]
005fa82c: ldr      r7, [sp, #0x40]
005fa830: add      r3, r3, r2
005fa834: ldr      sl, [sp, #0x3c]
005fa838: ldr      sb, [sp, #0x4c]
005fa83c: ldrb     r3, [r3, #5]
005fa840: and      r7, r7, sl
005fa844: cmp      r5, #0
005fa848: ldr      r8, [sp, #0x48]
005fa84c: ldr      r4, [sp, #0x50]
005fa850: str      r7, [sp, #0x38]
005fa854: str      r3, [sp, #0x40]
005fa858: str      sb, [sp, #0x6c]
005fa85c: beq      #0x5fa87c
005fa860: ldr      ip, [sp, #0x164]
005fa864: ldr      r0, [sp, #0x158]
005fa868: rsb      r1, sb, #0
005fa86c: sub      r3, ip, #1
005fa870: mla      r3, r3, sb, r0
005fa874: str      r1, [sp, #0x6c]
005fa878: str      r3, [sp, #0xa0]
005fa87c: ldr      r2, [sp, #0x164]
005fa880: cmp      r2, #0
005fa884: beq      #0x5f9eb4
005fa888: ldr      r3, [sp, #0xb8]
005fa88c: ldrb     r5, [sp, #0xc6]
005fa890: ldrb     ip, [sp, #0xca]
005fa894: str      r3, [sp, #0x58]
005fa898: ldr      r0, [sp, #0xbc]
005fa89c: ldr      r3, [sp, #0xa0]
005fa8a0: ldrb     r1, [sp, #0xc7]
005fa8a4: ldrb     r2, [sp, #0xcb]
005fa8a8: str      r8, [sp, #0x68]
005fa8ac: ldrb     sb, [sp, #0xc4]
005fa8b0: ldrb     sl, [sp, #0xc8]
005fa8b4: ldr      r8, [sp, #0xb4]
005fa8b8: ldrb     r7, [sp, #0xc5]
005fa8bc: ldrb     fp, [sp, #0xc9]
005fa8c0: str      r5, [sp, #0x50]
005fa8c4: str      ip, [sp, #0x4c]
005fa8c8: str      r0, [sp, #0x48]
005fa8cc: str      r1, [sp, #0x44]
005fa8d0: str      r2, [sp, #0x34]
005fa8d4: str      r3, [sp, #0x60]
005fa8d8: mov      r5, r3
005fa8dc: str      r4, [sp, #0x64]
005fa8e0: ldr      ip, [sp, #0x64]
005fa8e4: cmp      ip, #0
005fa8e8: beq      #0x5fa98c
005fa8ec: ldr      r1, [sp, #0x64]
005fa8f0: mov      r2, #0
005fa8f4: str      r7, [sp, #0x54]
005fa8f8: str      r5, [sp, #0x70]
005fa8fc: ldrb     r3, [r6]
005fa900: ldr      r4, [sp, #0x40]
005fa904: ldr      r5, [sp, #0x54]
005fa908: strb     r3, [sp, #0x100]
005fa90c: ldrb     r3, [r6, #1]
005fa910: ldr      r7, [sp, #0x50]
005fa914: subs     r1, r1, #1
005fa918: strb     r3, [sp, #0x101]
005fa91c: ldrb     r3, [r6, #2]
005fa920: add      r6, r6, r4
005fa924: strb     r3, [sp, #0x102]
005fa928: ldr      r3, [sp, #0x100]
005fa92c: lsr      ip, r3, r5
005fa930: lsr      r4, r3, r7
005fa934: ldr      r5, [sp, #0x58]
005fa938: ldr      r7, [sp, #0x44]
005fa93c: lsr      r0, r3, sb
005fa940: and      ip, r5, ip, lsl fp
005fa944: lsr      r3, r3, r7
005fa948: ldr      r5, [sp, #0x48]
005fa94c: ldr      r7, [sp, #0x4c]
005fa950: and      r0, r8, r0, lsl sl
005fa954: and      r4, r5, r4, lsl r7
005fa958: ldr      r5, [sp, #0x3c]
005fa95c: ldr      r7, [sp, #0x34]
005fa960: orr      r0, ip, r0
005fa964: ldr      ip, [sp, #0x38]
005fa968: and      r3, r5, r3, lsl r7
005fa96c: orr      r4, r0, r4
005fa970: ldr      r0, [sp, #0x70]
005fa974: orr      r3, r4, r3
005fa978: orr      r3, r3, ip
005fa97c: str      r3, [r0, r2]
005fa980: add      r2, r2, #4
005fa984: bne      #0x5fa8fc
005fa988: ldr      r7, [sp, #0x54]
005fa98c: ldr      r1, [sp, #0x164]
005fa990: subs     r1, r1, #1
005fa994: str      r1, [sp, #0x164]
005fa998: beq      #0x5f9eb4
005fa99c: ldr      r2, [sp, #0x68]
005fa9a0: ldr      r4, [sp, #0x60]
005fa9a4: ldr      r5, [sp, #0x6c]
005fa9a8: ldr      r3, [sp, #0x5c]
005fa9ac: add      r4, r4, r5
005fa9b0: add      r6, r2, r3
005fa9b4: str      r4, [sp, #0x60]
005fa9b8: mov      r5, r4
005fa9bc: str      r6, [sp, #0x68]
005fa9c0: b        #0x5fa8e0
005fa9c4: cmp      r4, r3, lsl #1
005fa9c8: addle    r4, r4, r6
005fa9cc: rsble    r3, r3, r4
005fa9d0: strble   r3, [r1, #0x14]
005fa9d4: b        #0x5fa7f0
005fa9d8: ldr      r1, [sp, #0x158]
005fa9dc: ldr      r0, [r5, fp]
005fa9e0: str      r1, [sp, #0xa0]
005fa9e4: mov      r1, #0x28
005fa9e8: mla      r1, r1, r7, r0
005fa9ec: ldrb     r1, [r1, #0x19]
005fa9f0: cmp      r1, #8
005fa9f4: bhi      #0x5fb3b8
005fa9f8: tst      r3, #1
005fa9fc: bne      #0x5fbf60
005faa00: mov      r1, #0
005faa04: str      r1, [sp, #0x40]
005faa08: ldr      r3, [r5, fp]
005faa0c: mov      r0, #0x28
005faa10: add      ip, sp, #0xb4
005faa14: mla      r7, r0, r7, r3
005faa18: str      sl, [sp, #0x44]
005faa1c: mla      r0, r0, sl, r3
005faa20: str      sb, [sp, #0x4c]
005faa24: mov      r1, ip
005faa28: mov      r2, #0
005faa2c: str      r6, [sp, #0x3c]
005faa30: str      r8, [sp, #0x48]
005faa34: str      r4, [sp, #0x50]
005faa38: mov      sb, r5
005faa3c: mov      sl, r7
005faa40: add      r5, sl, r2
005faa44: ldrb     r3, [r0, #0x18]
005faa48: ldrb     r4, [r7, #0x18]
005faa4c: ldr      r8, [r5, #4]
005faa50: ldrb     r6, [r7, #0x1c]
005faa54: ldrb     r5, [r0, #0x1c]
005faa58: cmp      r3, r4
005faa5c: str      r8, [ip, r2]
005faa60: strb     r5, [r1, #0x10]
005faa64: strb     r6, [r1, #0x14]
005faa68: bls      #0x5fac4c
005faa6c: add      r3, r3, r5
005faa70: rsb      r3, r4, r3
005faa74: strb     r3, [r1, #0x10]
005faa78: add      r2, r2, #4
005faa7c: cmp      r2, #0x10
005faa80: add      r0, r0, #1
005faa84: add      r1, r1, #1
005faa88: add      r7, r7, #1
005faa8c: bne      #0x5faa40
005faa90: mov      r5, sb
005faa94: ldr      r3, [sp, #0xc0]
005faa98: ldr      sl, [sp, #0x44]
005faa9c: ldr      r1, [r5, fp]
005faaa0: ldr      r6, [sp, #0x3c]
005faaa4: str      r3, [sp, #0x3c]
005faaa8: mov      r3, #0x28
005faaac: mla      r3, r3, sl, r1
005faab0: ldr      r5, [sp, #0x38]
005faab4: ldr      r7, [sp, #0x40]
005faab8: add      r3, r3, r2
005faabc: ldr      sl, [sp, #0x3c]
005faac0: ldr      sb, [sp, #0x4c]
005faac4: ldrb     r3, [r3, #5]
005faac8: and      r7, r7, sl
005faacc: cmp      r5, #0
005faad0: ldr      r8, [sp, #0x48]
005faad4: ldr      r4, [sp, #0x50]
005faad8: str      r7, [sp, #0x38]
005faadc: str      r3, [sp, #0x40]
005faae0: str      sb, [sp, #0x6c]
005faae4: beq      #0x5fab04
005faae8: ldr      ip, [sp, #0x164]
005faaec: ldr      r0, [sp, #0x158]
005faaf0: rsb      r1, sb, #0
005faaf4: sub      r3, ip, #1
005faaf8: mla      r3, r3, sb, r0
005faafc: str      r1, [sp, #0x6c]
005fab00: str      r3, [sp, #0xa0]
005fab04: ldr      r2, [sp, #0x164]
005fab08: cmp      r2, #0
005fab0c: beq      #0x5f9eb4
005fab10: ldr      r3, [sp, #0xb8]
005fab14: ldrb     r5, [sp, #0xc6]
005fab18: ldrb     ip, [sp, #0xca]
005fab1c: str      r3, [sp, #0x58]
005fab20: ldr      r0, [sp, #0xbc]
005fab24: ldr      r3, [sp, #0xa0]
005fab28: ldrb     r1, [sp, #0xc7]
005fab2c: ldrb     r2, [sp, #0xcb]
005fab30: str      r8, [sp, #0x68]
005fab34: ldrb     sb, [sp, #0xc4]
005fab38: ldrb     sl, [sp, #0xc8]
005fab3c: ldr      r8, [sp, #0xb4]
005fab40: ldrb     r7, [sp, #0xc5]
005fab44: ldrb     fp, [sp, #0xc9]
005fab48: str      r5, [sp, #0x50]
005fab4c: str      ip, [sp, #0x4c]
005fab50: str      r0, [sp, #0x48]
005fab54: str      r1, [sp, #0x44]
005fab58: str      r2, [sp, #0x34]
005fab5c: str      r3, [sp, #0x60]
005fab60: mov      r5, r3
005fab64: str      r4, [sp, #0x64]
005fab68: ldr      ip, [sp, #0x64]
005fab6c: cmp      ip, #0
005fab70: beq      #0x5fac14
005fab74: ldr      r1, [sp, #0x64]
005fab78: mov      r2, #0
005fab7c: str      r7, [sp, #0x54]
005fab80: str      r5, [sp, #0x70]
005fab84: ldrb     r3, [r6]
005fab88: ldr      r4, [sp, #0x40]
005fab8c: ldr      r5, [sp, #0x54]
005fab90: strb     r3, [sp, #0x108]
005fab94: ldrb     r3, [r6, #1]
005fab98: ldr      r7, [sp, #0x50]
005fab9c: subs     r1, r1, #1
005faba0: strb     r3, [sp, #0x109]
005faba4: ldrb     r3, [r6, #2]
005faba8: add      r6, r6, r4
005fabac: strb     r3, [sp, #0x10a]
005fabb0: ldr      r3, [sp, #0x108]
005fabb4: lsr      ip, r3, r5
005fabb8: lsr      r4, r3, r7
005fabbc: ldr      r5, [sp, #0x58]
005fabc0: ldr      r7, [sp, #0x44]
005fabc4: lsr      r0, r3, sb
005fabc8: and      ip, r5, ip, lsl fp
005fabcc: lsr      r3, r3, r7
005fabd0: ldr      r5, [sp, #0x48]
005fabd4: ldr      r7, [sp, #0x4c]
005fabd8: and      r0, r8, r0, lsl sl
005fabdc: and      r4, r5, r4, lsl r7
005fabe0: ldr      r5, [sp, #0x3c]
005fabe4: ldr      r7, [sp, #0x34]
005fabe8: orr      r0, ip, r0
005fabec: ldr      ip, [sp, #0x38]
005fabf0: and      r3, r5, r3, lsl r7
005fabf4: orr      r4, r0, r4
005fabf8: ldr      r0, [sp, #0x70]
005fabfc: orr      r3, r4, r3
005fac00: orr      r3, r3, ip
005fac04: strh     r3, [r0, r2]
005fac08: add      r2, r2, #2
005fac0c: bne      #0x5fab84
005fac10: ldr      r7, [sp, #0x54]
005fac14: ldr      r1, [sp, #0x164]
005fac18: subs     r1, r1, #1
005fac1c: str      r1, [sp, #0x164]
005fac20: beq      #0x5f9eb4
005fac24: ldr      r2, [sp, #0x68]
005fac28: ldr      r4, [sp, #0x60]
005fac2c: ldr      r5, [sp, #0x6c]
005fac30: ldr      r3, [sp, #0x5c]
005fac34: add      r4, r4, r5
005fac38: add      r6, r2, r3
005fac3c: str      r4, [sp, #0x60]
005fac40: mov      r5, r4
005fac44: str      r6, [sp, #0x68]
005fac48: b        #0x5fab68
005fac4c: cmp      r4, r3, lsl #1
005fac50: addle    r4, r4, r6
005fac54: rsble    r3, r3, r4
005fac58: strble   r3, [r1, #0x14]
005fac5c: b        #0x5faa78
005fac60: mov      r1, sl
005fac64: add      r0, sp, #0xb4
005fac68: bl       #0x5edca4
005fac6c: ldr      r3, [r5, fp]
005fac70: ldr      ip, [sp, #0x38]
005fac74: mov      r2, #0x28
005fac78: mla      sl, r2, sl, r3
005fac7c: cmp      ip, #0
005fac80: str      sb, [sp, #0x38]
005fac84: ldrb     sl, [sl, #0x15]
005fac88: beq      #0x5faca0
005fac8c: ldr      r0, [sp, #0x164]
005fac90: rsb      r1, sb, #0
005fac94: str      r1, [sp, #0x38]
005fac98: sub      r3, r0, #1
005fac9c: mla      r7, r3, sb, r7
005faca0: ldr      r2, [sp, #0x164]
005faca4: cmp      r2, #0
005faca8: strne    r8, [sp, #0x34]
005facac: movne    fp, r7
005facb0: beq      #0x5f9eb4
005facb4: cmp      r4, #0
005facb8: movne    r8, r4
005facbc: beq      #0x5fad84
005facc0: ldr      r5, [r6]
005facc4: ldr      r0, [sp, #0xc0]
005facc8: ldrb     r3, [sp, #0xcc]
005faccc: and      r0, r5, r0
005facd0: lsr      r0, r0, r3
005facd4: bl       #0x30e2e0
005facd8: ldr      r1, [sp, #0xb4]
005facdc: bl       #0x30ed6c
005face0: mov      sb, r0
005face4: ldr      r0, [sp, #0xc4]
005face8: ldrb     r3, [sp, #0xcd]
005facec: and      r0, r5, r0
005facf0: lsr      r0, r0, r3
005facf4: bl       #0x30e2e0
005facf8: ldr      r1, [sp, #0xb8]
005facfc: bl       #0x30ed6c
005fad00: mov      r1, r0
005fad04: mov      r0, sb
005fad08: bl       #0x30eba4
005fad0c: ldr      r2, [sp, #0xc8]
005fad10: ldrb     r3, [sp, #0xce]
005fad14: mov      sb, r0
005fad18: and      r5, r5, r2
005fad1c: lsr      r0, r5, r3
005fad20: bl       #0x30e2e0
005fad24: ldr      r1, [sp, #0xbc]
005fad28: bl       #0x30ed6c
005fad2c: mov      r1, r0
005fad30: mov      r0, sb
005fad34: bl       #0x30eba4
005fad38: mov      r1, #0x43000000
005fad3c: add      r1, r1, #0x7f0000
005fad40: bl       #0x30ed6c
005fad44: bl       #0x8be2a0
005fad48: strb     r0, [r7]
005fad4c: ldr      r3, [sp, #0xd0]
005fad50: ldr      r0, [r6], sl
005fad54: ldrb     r2, [sp, #0xcf]
005fad58: and      r0, r0, r3
005fad5c: ldr      r3, [sp, #0xd8]
005fad60: orr      r0, r3, r0, lsr r2
005fad64: bl       #0x30e2e0
005fad68: ldr      r1, [sp, #0xd4]
005fad6c: bl       #0x30ed6c
005fad70: bl       #0x8be2a0
005fad74: subs     r8, r8, #1
005fad78: strb     r0, [r7, #1]
005fad7c: add      r7, r7, #2
005fad80: bne      #0x5facc0
005fad84: ldr      r3, [sp, #0x164]
005fad88: subs     r3, r3, #1
005fad8c: str      r3, [sp, #0x164]
005fad90: beq      #0x5f9eb4
005fad94: ldr      r5, [sp, #0x34]
005fad98: ldr      r7, [sp, #0x5c]
005fad9c: ldr      r8, [sp, #0x38]
005fada0: add      r6, r5, r7
005fada4: add      fp, fp, r8
005fada8: mov      r7, fp
005fadac: str      r6, [sp, #0x34]
005fadb0: b        #0x5facb4
005fadb4: mov      r1, sl
005fadb8: add      r0, sp, #0xb4
005fadbc: bl       #0x5edca4
005fadc0: ldr      r3, [r5, fp]
005fadc4: ldr      ip, [sp, #0x38]
005fadc8: mov      r2, #0x28
005fadcc: mla      sl, r2, sl, r3
005fadd0: cmp      ip, #0
005fadd4: str      sb, [sp, #0x38]
005fadd8: ldrb     sl, [sl, #0x15]
005faddc: beq      #0x5fadf8
005fade0: ldr      r0, [sp, #0x164]
005fade4: rsb      r1, sb, #0
005fade8: str      r1, [sp, #0x38]
005fadec: sub      r3, r0, #1
005fadf0: mla      r7, r3, sb, r7
005fadf4: str      r7, [sp, #0x68]
005fadf8: ldr      r2, [sp, #0x164]
005fadfc: cmp      r2, #0
005fae00: beq      #0x5f9eb4
005fae04: ldr      r5, [sp, #0x68]
005fae08: str      r8, [sp, #0x34]
005fae0c: mov      fp, r5
005fae10: cmp      r4, #0
005fae14: movne    r8, r4
005fae18: beq      #0x5faee0
005fae1c: ldrh     r7, [r6]
005fae20: ldr      r0, [sp, #0xc0]
005fae24: ldrb     r3, [sp, #0xcc]
005fae28: and      r0, r7, r0
005fae2c: lsr      r0, r0, r3
005fae30: bl       #0x30e2e0
005fae34: ldr      r1, [sp, #0xb4]
005fae38: bl       #0x30ed6c
005fae3c: mov      sb, r0
005fae40: ldr      r0, [sp, #0xc4]
005fae44: ldrb     r3, [sp, #0xcd]
005fae48: and      r0, r7, r0
005fae4c: lsr      r0, r0, r3
005fae50: bl       #0x30e2e0
005fae54: ldr      r1, [sp, #0xb8]
005fae58: bl       #0x30ed6c
005fae5c: mov      r1, r0
005fae60: mov      r0, sb
005fae64: bl       #0x30eba4
005fae68: ldr      r2, [sp, #0xc8]
005fae6c: ldrb     r3, [sp, #0xce]
005fae70: mov      sb, r0
005fae74: and      r7, r7, r2
005fae78: lsr      r0, r7, r3
005fae7c: bl       #0x30e2e0
005fae80: ldr      r1, [sp, #0xbc]
005fae84: bl       #0x30ed6c
005fae88: mov      r1, r0
005fae8c: mov      r0, sb
005fae90: bl       #0x30eba4
005fae94: mov      r1, #0x43000000
005fae98: add      r1, r1, #0x7f0000
005fae9c: bl       #0x30ed6c
005faea0: bl       #0x8be2a0
005faea4: strb     r0, [r5]
005faea8: ldr      r3, [sp, #0xd0]
005faeac: ldrh     r0, [r6], sl
005faeb0: ldrb     r2, [sp, #0xcf]
005faeb4: and      r0, r0, r3
005faeb8: ldr      r3, [sp, #0xd8]
005faebc: orr      r0, r3, r0, lsr r2
005faec0: bl       #0x30e2e0
005faec4: ldr      r1, [sp, #0xd4]
005faec8: bl       #0x30ed6c
005faecc: bl       #0x8be2a0
005faed0: subs     r8, r8, #1
005faed4: strb     r0, [r5, #1]
005faed8: add      r5, r5, #2
005faedc: bne      #0x5fae1c
005faee0: ldr      r3, [sp, #0x164]
005faee4: subs     r3, r3, #1
005faee8: str      r3, [sp, #0x164]
005faeec: beq      #0x5f9eb4
005faef0: ldr      r5, [sp, #0x34]
005faef4: ldr      r7, [sp, #0x5c]
005faef8: ldr      r8, [sp, #0x38]
005faefc: add      r6, r5, r7
005faf00: add      fp, fp, r8
005faf04: str      r6, [sp, #0x34]
005faf08: mov      r5, fp
005faf0c: b        #0x5fae10
005faf10: mov      r1, r7
005faf14: mov      r0, sl
005faf18: add      r2, sp, #0x10c
005faf1c: str      ip, [sp, #0x30]
005faf20: bl       #0x5ed9d0
005faf24: ldr      ip, [sp, #0x30]
005faf28: ldr      r1, [sp, #0x158]
005faf2c: mla      ip, fp, sl, ip
005faf30: cmp      r8, r1
005faf34: ldrb     r7, [ip, #0x15]
005faf38: beq      #0x5fc9d0
005faf3c: ldr      sl, [sp, #0x38]
005faf40: ldr      r0, [sp, #0x164]
005faf44: mov      r1, sb
005faf48: cmp      sl, #0
005faf4c: ldrne    ip, [sp, #0x164]
005faf50: rsbne    r1, sb, #0
005faf54: subne    r3, ip, #1
005faf58: mlane    r6, r3, sb, r6
005faf5c: cmp      r0, #0
005faf60: beq      #0x5f9eb4
005faf64: ldr      sl, [sp, #0x5c]
005faf68: ldr      ip, [sp, #0x164]
005faf6c: mov      r0, r6
005faf70: cmp      r4, #0
005faf74: movne    r3, r4
005faf78: beq      #0x5fafb0
005faf7c: ldrb     r2, [sp, #0x10c]
005faf80: subs     r3, r3, #1
005faf84: ldr      r2, [r5, r2, lsl #2]
005faf88: str      r2, [r6]
005faf8c: ldrb     r2, [sp, #0x10d]
005faf90: ldr      r2, [r5, r2, lsl #2]
005faf94: str      r2, [r6, #4]
005faf98: ldrb     r2, [sp, #0x10e]
005faf9c: ldr      r2, [r5, r2, lsl #2]
005fafa0: add      r5, r5, r7
005fafa4: str      r2, [r6, #8]
005fafa8: add      r6, r6, #0xc
005fafac: bne      #0x5faf7c
005fafb0: subs     ip, ip, #1
005fafb4: beq      #0x5f9eb4
005fafb8: add      r5, r8, sl
005fafbc: add      r0, r0, r1
005fafc0: mov      r6, r0
005fafc4: mov      r8, r5
005fafc8: b        #0x5faf70
005fafcc: mov      r1, r7
005fafd0: mov      r0, sl
005fafd4: add      r2, sp, #0x10c
005fafd8: str      ip, [sp, #0x30]
005fafdc: bl       #0x5ed9d0
005fafe0: ldr      ip, [sp, #0x30]
005fafe4: ldr      r1, [sp, #0x158]
005fafe8: mla      ip, fp, sl, ip
005fafec: cmp      r8, r1
005faff0: ldrb     r7, [ip, #0x15]
005faff4: beq      #0x5fcd3c
005faff8: ldr      sl, [sp, #0x38]
005faffc: ldr      r0, [sp, #0x164]
005fb000: mov      r1, sb
005fb004: cmp      sl, #0
005fb008: ldrne    ip, [sp, #0x164]
005fb00c: rsbne    r1, sb, #0
005fb010: subne    r3, ip, #1
005fb014: mlane    r6, r3, sb, r6
005fb018: cmp      r0, #0
005fb01c: beq      #0x5f9eb4
005fb020: ldr      sl, [sp, #0x5c]
005fb024: ldr      ip, [sp, #0x164]
005fb028: mov      r0, r6
005fb02c: cmp      r4, #0
005fb030: movne    r3, r4
005fb034: beq      #0x5fb078
005fb038: ldrb     r2, [sp, #0x10c]
005fb03c: subs     r3, r3, #1
005fb040: ldr      r2, [r5, r2, lsl #2]
005fb044: str      r2, [r6]
005fb048: ldrb     r2, [sp, #0x10d]
005fb04c: ldr      r2, [r5, r2, lsl #2]
005fb050: str      r2, [r6, #4]
005fb054: ldrb     r2, [sp, #0x10e]
005fb058: ldr      r2, [r5, r2, lsl #2]
005fb05c: str      r2, [r6, #8]
005fb060: ldrb     r2, [sp, #0x10f]
005fb064: ldr      r2, [r5, r2, lsl #2]
005fb068: add      r5, r5, r7
005fb06c: str      r2, [r6, #0xc]
005fb070: add      r6, r6, #0x10
005fb074: bne      #0x5fb038
005fb078: subs     ip, ip, #1
005fb07c: beq      #0x5f9eb4
005fb080: add      r5, r8, sl
005fb084: add      r0, r0, r1
005fb088: mov      r6, r0
005fb08c: mov      r8, r5
005fb090: b        #0x5fb02c
005fb094: mov      r1, r7
005fb098: mov      r0, sl
005fb09c: add      r2, sp, #0x12c
005fb0a0: str      ip, [sp, #0x30]
005fb0a4: bl       #0x5ed9d0
005fb0a8: ldr      ip, [sp, #0x30]
005fb0ac: ldr      r1, [sp, #0x158]
005fb0b0: mla      ip, fp, sl, ip
005fb0b4: cmp      r8, r1
005fb0b8: ldrb     r7, [ip, #0x15]
005fb0bc: beq      #0x5fcc88
005fb0c0: ldr      sl, [sp, #0x38]
005fb0c4: ldr      r0, [sp, #0x164]
005fb0c8: mov      r1, sb
005fb0cc: cmp      sl, #0
005fb0d0: ldrne    ip, [sp, #0x164]
005fb0d4: rsbne    r1, sb, #0
005fb0d8: subne    r3, ip, #1
005fb0dc: mlane    r6, r3, sb, r6
005fb0e0: cmp      r0, #0
005fb0e4: beq      #0x5f9eb4
005fb0e8: ldr      sl, [sp, #0x5c]
005fb0ec: ldr      ip, [sp, #0x164]
005fb0f0: mov      r0, r6
005fb0f4: cmp      r4, #0
005fb0f8: movne    r3, r4
005fb0fc: beq      #0x5fb140
005fb100: ldrb     r2, [sp, #0x12c]
005fb104: subs     r3, r3, #1
005fb108: ldrb     r2, [r5, r2]
005fb10c: strb     r2, [r6]
005fb110: ldrb     r2, [sp, #0x12d]
005fb114: ldrb     r2, [r5, r2]
005fb118: strb     r2, [r6, #1]
005fb11c: ldrb     r2, [sp, #0x12e]
005fb120: ldrb     r2, [r5, r2]
005fb124: strb     r2, [r6, #2]
005fb128: ldrb     r2, [sp, #0x12f]
005fb12c: ldrb     r2, [r5, r2]
005fb130: add      r5, r5, r7
005fb134: strb     r2, [r6, #3]
005fb138: add      r6, r6, #4
005fb13c: bne      #0x5fb100
005fb140: subs     ip, ip, #1
005fb144: beq      #0x5f9eb4
005fb148: add      r5, r8, sl
005fb14c: add      r0, r0, r1
005fb150: mov      r6, r0
005fb154: mov      r8, r5
005fb158: b        #0x5fb0f4
005fb15c: mov      r1, r7
005fb160: mov      r0, sl
005fb164: add      r2, sp, #0x12c
005fb168: str      ip, [sp, #0x30]
005fb16c: bl       #0x5ed9d0
005fb170: ldr      ip, [sp, #0x30]
005fb174: ldr      r1, [sp, #0x158]
005fb178: mla      ip, fp, sl, ip
005fb17c: cmp      r8, r1
005fb180: ldrb     r7, [ip, #0x15]
005fb184: beq      #0x5fcbe0
005fb188: ldr      sl, [sp, #0x38]
005fb18c: ldr      r0, [sp, #0x164]
005fb190: mov      r1, sb
005fb194: cmp      sl, #0
005fb198: ldrne    ip, [sp, #0x164]
005fb19c: rsbne    r1, sb, #0
005fb1a0: subne    r3, ip, #1
005fb1a4: mlane    r6, r3, sb, r6
005fb1a8: cmp      r0, #0
005fb1ac: beq      #0x5f9eb4
005fb1b0: ldr      sl, [sp, #0x5c]
005fb1b4: ldr      ip, [sp, #0x164]
005fb1b8: mov      r0, r6
005fb1bc: cmp      r4, #0
005fb1c0: movne    r3, r4
005fb1c4: beq      #0x5fb1fc
005fb1c8: ldrb     r2, [sp, #0x12c]
005fb1cc: subs     r3, r3, #1
005fb1d0: ldrb     r2, [r5, r2]
005fb1d4: strb     r2, [r6]
005fb1d8: ldrb     r2, [sp, #0x12d]
005fb1dc: ldrb     r2, [r5, r2]
005fb1e0: strb     r2, [r6, #1]
005fb1e4: ldrb     r2, [sp, #0x12e]
005fb1e8: ldrb     r2, [r5, r2]
005fb1ec: add      r5, r5, r7
005fb1f0: strb     r2, [r6, #2]
005fb1f4: add      r6, r6, #3
005fb1f8: bne      #0x5fb1c8
005fb1fc: subs     ip, ip, #1
005fb200: beq      #0x5f9eb4
005fb204: add      r5, r8, sl
005fb208: add      r0, r0, r1
005fb20c: mov      r6, r0
005fb210: mov      r8, r5
005fb214: b        #0x5fb1bc
005fb218: mov      r1, r7
005fb21c: mov      r0, sl
005fb220: add      r2, sp, #0x10c
005fb224: str      ip, [sp, #0x30]
005fb228: bl       #0x5ed9d0
005fb22c: ldr      ip, [sp, #0x30]
005fb230: ldr      r1, [sp, #0x158]
005fb234: mla      ip, fp, sl, ip
005fb238: cmp      r8, r1
005fb23c: ldrb     r7, [ip, #0x15]
005fb240: beq      #0x5fcb14
005fb244: ldr      sl, [sp, #0x38]
005fb248: ldr      r0, [sp, #0x164]
005fb24c: mov      r1, sb
005fb250: cmp      sl, #0
005fb254: ldrne    ip, [sp, #0x164]
005fb258: rsbne    r1, sb, #0
005fb25c: subne    r3, ip, #1
005fb260: mlane    r6, r3, sb, r6
005fb264: cmp      r0, #0
005fb268: beq      #0x5f9eb4
005fb26c: ldr      sl, [sp, #0x5c]
005fb270: mov      r3, r6
005fb274: mov      ip, r0
005fb278: cmp      r4, #0
005fb27c: movne    r2, r4
005fb280: beq      #0x5fb2d4
005fb284: ldrb     r0, [sp, #0x10c]
005fb288: subs     r2, r2, #1
005fb28c: lsl      r0, r0, #1
005fb290: ldrh     r0, [r8, r0]
005fb294: strh     r0, [r3]
005fb298: ldrb     r0, [sp, #0x10d]
005fb29c: lsl      r0, r0, #1
005fb2a0: ldrh     r0, [r8, r0]
005fb2a4: strh     r0, [r3, #2]
005fb2a8: ldrb     r0, [sp, #0x10e]
005fb2ac: lsl      r0, r0, #1
005fb2b0: ldrh     r0, [r8, r0]
005fb2b4: strh     r0, [r3, #4]
005fb2b8: ldrb     r0, [sp, #0x10f]
005fb2bc: lsl      r0, r0, #1
005fb2c0: ldrh     r0, [r8, r0]
005fb2c4: add      r8, r8, r7
005fb2c8: strh     r0, [r3, #6]
005fb2cc: add      r3, r3, #8
005fb2d0: bne      #0x5fb284
005fb2d4: subs     ip, ip, #1
005fb2d8: beq      #0x5f9eb4
005fb2dc: add      r5, r5, sl
005fb2e0: add      r6, r6, r1
005fb2e4: mov      r3, r6
005fb2e8: mov      r8, r5
005fb2ec: b        #0x5fb278
005fb2f0: mov      r1, r7
005fb2f4: mov      r0, sl
005fb2f8: add      r2, sp, #0x10c
005fb2fc: str      ip, [sp, #0x30]
005fb300: bl       #0x5ed9d0
005fb304: ldr      ip, [sp, #0x30]
005fb308: ldr      r1, [sp, #0x158]
005fb30c: mla      ip, fp, sl, ip
005fb310: cmp      r8, r1
005fb314: ldrb     r7, [ip, #0x15]
005fb318: beq      #0x5fc914
005fb31c: ldr      sl, [sp, #0x38]
005fb320: ldr      r0, [sp, #0x164]
005fb324: mov      r1, sb
005fb328: cmp      sl, #0
005fb32c: ldrne    ip, [sp, #0x164]
005fb330: rsbne    r1, sb, #0
005fb334: subne    r3, ip, #1
005fb338: mlane    r6, r3, sb, r6
005fb33c: cmp      r0, #0
005fb340: beq      #0x5f9eb4
005fb344: ldr      sl, [sp, #0x5c]
005fb348: mov      r3, r6
005fb34c: mov      ip, r0
005fb350: cmp      r4, #0
005fb354: movne    r2, r4
005fb358: beq      #0x5fb39c
005fb35c: ldrb     r0, [sp, #0x10c]
005fb360: subs     r2, r2, #1
005fb364: lsl      r0, r0, #1
005fb368: ldrh     r0, [r8, r0]
005fb36c: strh     r0, [r3]
005fb370: ldrb     r0, [sp, #0x10d]
005fb374: lsl      r0, r0, #1
005fb378: ldrh     r0, [r8, r0]
005fb37c: strh     r0, [r3, #2]
005fb380: ldrb     r0, [sp, #0x10e]
005fb384: lsl      r0, r0, #1
005fb388: ldrh     r0, [r8, r0]
005fb38c: add      r8, r8, r7
005fb390: strh     r0, [r3, #4]
005fb394: add      r3, r3, #6
005fb398: bne      #0x5fb35c
005fb39c: subs     ip, ip, #1
005fb3a0: beq      #0x5f9eb4
005fb3a4: add      r5, r5, sl
005fb3a8: add      r6, r6, r1
005fb3ac: mov      r3, r6
005fb3b0: mov      r8, r5
005fb3b4: b        #0x5fb350
005fb3b8: tst      r3, #1
005fb3bc: bne      #0x5fc10c
005fb3c0: mov      r3, #0
005fb3c4: str      r3, [sp, #0x3c]
005fb3c8: ldr      r2, [r5, fp]
005fb3cc: mov      r3, #0x28
005fb3d0: add      ip, sp, #0xb4
005fb3d4: mla      r7, r3, r7, r2
005fb3d8: mla      r2, r3, sl, r2
005fb3dc: mov      r0, r7
005fb3e0: str      r7, [sp, #0x40]
005fb3e4: str      r2, [sp, #0x44]
005fb3e8: mov      r7, r2
005fb3ec: str      sl, [sp, #0x50]
005fb3f0: str      sb, [sp, #0x60]
005fb3f4: str      ip, [sp, #0x48]
005fb3f8: mov      r1, ip
005fb3fc: mov      r2, #0
005fb400: str      r6, [sp, #0x4c]
005fb404: str      r8, [sp, #0x58]
005fb408: str      r4, [sp, #0x64]
005fb40c: mov      sb, r5
005fb410: mov      sl, r0
005fb414: add      r5, sl, r2
005fb418: ldrb     r3, [r7, #0x18]
005fb41c: ldrb     r4, [r0, #0x18]
005fb420: ldr      r8, [r5, #4]
005fb424: ldrb     r6, [r0, #0x1c]
005fb428: ldrb     r5, [r7, #0x1c]
005fb42c: cmp      r3, r4
005fb430: str      r8, [ip, r2]
005fb434: strb     r5, [r1, #0x10]
005fb438: strb     r6, [r1, #0x14]
005fb43c: bls      #0x5fb780
005fb440: add      r3, r3, r5
005fb444: rsb      r3, r4, r3
005fb448: strb     r3, [r1, #0x10]
005fb44c: add      r2, r2, #4
005fb450: cmp      r2, #0x10
005fb454: add      r7, r7, #1
005fb458: add      r1, r1, #1
005fb45c: add      r0, r0, #1
005fb460: bne      #0x5fb414
005fb464: mov      r5, sb
005fb468: ldr      r2, [r5, fp]
005fb46c: ldr      sl, [sp, #0x50]
005fb470: mov      r3, #0x28
005fb474: ldr      r5, [sp, #0x3c]
005fb478: mla      sl, r3, sl, r2
005fb47c: ldr      r3, [sp, #0xc0]
005fb480: ldr      r6, [sp, #0x4c]
005fb484: ldr      r8, [sp, #0x58]
005fb488: ldr      r4, [sp, #0x64]
005fb48c: ldr      sb, [sp, #0x60]
005fb490: ldr      r0, [sp, #0x48]
005fb494: ldr      ip, [sp, #0x44]
005fb498: and      r3, r5, r3
005fb49c: str      r3, [sp, #0x3c]
005fb4a0: ldr      r2, [sp, #0x40]
005fb4a4: mov      r3, #0
005fb4a8: str      r6, [sp, #0x40]
005fb4ac: mov      fp, r8
005fb4b0: str      r4, [sp, #0x34]
005fb4b4: ldrb     r6, [ip, #0x18]
005fb4b8: ldrb     r5, [r2, #0x18]
005fb4bc: add      r1, sl, r3, lsl #2
005fb4c0: ldr      r1, [r1, #4]
005fb4c4: rsb      r5, r5, r6, lsl #1
005fb4c8: uxtb     r5, r5
005fb4cc: and      r7, r1, r1, lsl r5
005fb4d0: add      r6, sp, #0x130
005fb4d4: add      r8, r6, r3, lsl #2
005fb4d8: str      r1, [r8, #-0x60]
005fb4dc: str      r7, [r8, #-0x54]
005fb4e0: ldrb     r7, [r0, #0x10]
005fb4e4: ldrb     r1, [r2, #0x1c]
005fb4e8: add      r8, sp, #0x130
005fb4ec: add      r6, r8, r3
005fb4f0: add      r3, r3, #1
005fb4f4: sub      r6, r6, #0x4c
005fb4f8: add      r5, r5, r7
005fb4fc: cmp      r3, #3
005fb500: strb     r1, [r6, #7]
005fb504: strb     r5, [r6, #4]
005fb508: add      ip, ip, #1
005fb50c: add      r2, r2, #1
005fb510: add      r0, r0, #1
005fb514: bne      #0x5fb4b4
005fb518: ldr      ip, [sp, #0x38]
005fb51c: ldrb     sl, [sl, #0x15]
005fb520: ldr      r6, [sp, #0x40]
005fb524: cmp      ip, #0
005fb528: mov      r8, fp
005fb52c: ldr      r4, [sp, #0x34]
005fb530: str      sl, [sp, #0x38]
005fb534: str      sb, [sp, #0xa8]
005fb538: beq      #0x5fb558
005fb53c: ldr      r0, [sp, #0x164]
005fb540: ldr      r1, [sp, #0x158]
005fb544: rsb      r2, sb, #0
005fb548: sub      r3, r0, #1
005fb54c: mla      r3, r3, sb, r1
005fb550: str      r2, [sp, #0xa8]
005fb554: str      r3, [sp, #0xa0]
005fb558: ldr      r3, [sp, #0x164]
005fb55c: cmp      r3, #0
005fb560: beq      #0x5f9eb4
005fb564: ldrb     r5, [sp, #0xc8]
005fb568: ldr      r7, [sp, #0xdc]
005fb56c: ldrb     sl, [sp, #0xeb]
005fb570: ldr      ip, [sp, #0xb4]
005fb574: ldr      r0, [sp, #0xd4]
005fb578: ldrb     r1, [sp, #0xc5]
005fb57c: str      r8, [sp, #0xa4]
005fb580: ldrb     r8, [sp, #0xe8]
005fb584: ldrb     r2, [sp, #0xc9]
005fb588: ldr      r3, [sp, #0xe0]
005fb58c: str      r5, [sp, #0x98]
005fb590: str      r7, [sp, #0x94]
005fb594: ldrb     r5, [sp, #0xe9]
005fb598: ldrb     r7, [sp, #0xec]
005fb59c: str      r8, [sp, #0x90]
005fb5a0: str      sl, [sp, #0x8c]
005fb5a4: ldr      r8, [sp, #0xb8]
005fb5a8: ldr      sl, [sp, #0xd8]
005fb5ac: str      ip, [sp, #0x40]
005fb5b0: str      r0, [sp, #0x88]
005fb5b4: ldrb     ip, [sp, #0xc6]
005fb5b8: str      r1, [sp, #0x84]
005fb5bc: ldrb     r0, [sp, #0xca]
005fb5c0: ldr      r1, [sp, #0xe4]
005fb5c4: str      r2, [sp, #0x80]
005fb5c8: str      r3, [sp, #0x7c]
005fb5cc: str      r5, [sp, #0x78]
005fb5d0: str      r7, [sp, #0x74]
005fb5d4: str      r8, [sp, #0x34]
005fb5d8: str      sl, [sp, #0x70]
005fb5dc: str      ip, [sp, #0x6c]
005fb5e0: ldr      sb, [sp, #0xd0]
005fb5e4: ldrb     fp, [sp, #0xc4]
005fb5e8: str      r0, [sp, #0x68]
005fb5ec: str      r1, [sp, #0x64]
005fb5f0: ldr      ip, [sp, #0xa0]
005fb5f4: ldr      sl, [sp, #0xc0]
005fb5f8: ldrb     r2, [sp, #0xea]
005fb5fc: ldrb     r3, [sp, #0xed]
005fb600: ldr      r5, [sp, #0xbc]
005fb604: ldrb     r7, [sp, #0xc7]
005fb608: ldrb     r8, [sp, #0xcb]
005fb60c: str      sl, [sp, #0x44]
005fb610: str      r2, [sp, #0x60]
005fb614: str      r3, [sp, #0x58]
005fb618: str      r5, [sp, #0x50]
005fb61c: str      r7, [sp, #0x4c]
005fb620: str      r8, [sp, #0x48]
005fb624: str      ip, [sp, #0x9c]
005fb628: mov      sl, ip
005fb62c: str      r4, [sp, #0xa0]
005fb630: ldr      ip, [sp, #0xa0]
005fb634: cmp      ip, #0
005fb638: beq      #0x5fb748
005fb63c: ldr      r1, [sp, #0xa0]
005fb640: mov      r2, #0
005fb644: str      sl, [sp, #0x54]
005fb648: ldrb     r3, [r6]
005fb64c: ldr      r4, [sp, #0x88]
005fb650: ldr      r5, [sp, #0x84]
005fb654: strb     r3, [sp, #0x10c]
005fb658: ldrb     r3, [r6, #1]
005fb65c: ldr      sl, [sp, #0x98]
005fb660: ldr      r0, [sp, #0x38]
005fb664: strb     r3, [sp, #0x10d]
005fb668: ldrb     r3, [r6, #2]
005fb66c: ldr      r8, [sp, #0x70]
005fb670: add      r6, r0, r6
005fb674: strb     r3, [sp, #0x10e]
005fb678: ldr      r3, [sp, #0x10c]
005fb67c: subs     r1, r1, #1
005fb680: and      r7, sb, r3
005fb684: lsr      r7, r7, fp
005fb688: and      ip, r3, r4
005fb68c: lsr      ip, ip, r5
005fb690: lsl      r7, r7, sl
005fb694: ldr      r5, [sp, #0x80]
005fb698: ldr      r4, [sp, #0x6c]
005fb69c: and      r0, r3, r8
005fb6a0: lsl      ip, ip, r5
005fb6a4: ldr      r8, [sp, #0x68]
005fb6a8: lsr      r0, r0, r4
005fb6ac: lsl      r0, r0, r8
005fb6b0: ldr      sl, [sp, #0x94]
005fb6b4: ldr      r4, [sp, #0x90]
005fb6b8: ldr      r5, [sp, #0x7c]
005fb6bc: and      r8, r3, sl
005fb6c0: ldr      sl, [sp, #0x78]
005fb6c4: lsr      r8, r8, r4
005fb6c8: and      r4, r3, r5
005fb6cc: lsr      r4, r4, sl
005fb6d0: ldr      sl, [sp, #0x64]
005fb6d4: and      r5, r3, sl
005fb6d8: ldr      sl, [sp, #0x8c]
005fb6dc: orr      r7, r7, r8, lsl sl
005fb6e0: ldr      sl, [sp, #0x74]
005fb6e4: ldr      r8, [sp, #0x60]
005fb6e8: orr      ip, ip, r4, lsl sl
005fb6ec: ldr      r4, [sp, #0x58]
005fb6f0: lsr      r5, r5, r8
005fb6f4: ldr      r8, [sp, #0x4c]
005fb6f8: orr      r5, r0, r5, lsl r4
005fb6fc: ldr      sl, [sp, #0x44]
005fb700: ldr      r0, [sp, #0x48]
005fb704: lsr      r3, r3, r8
005fb708: and      r3, sl, r3, lsl r0
005fb70c: ldr      r4, [sp, #0x40]
005fb710: ldr      r8, [sp, #0x3c]
005fb714: ldr      sl, [sp, #0x34]
005fb718: ldr      r0, [sp, #0x50]
005fb71c: and      r7, r7, r4
005fb720: orr      r7, r8, r7
005fb724: and      ip, ip, sl
005fb728: orr      ip, r7, ip
005fb72c: and      r5, r5, r0
005fb730: orr      ip, ip, r5
005fb734: orr      ip, ip, r3
005fb738: ldr      r3, [sp, #0x54]
005fb73c: strh     ip, [r3, r2]
005fb740: add      r2, r2, #2
005fb744: bne      #0x5fb648
005fb748: ldr      r4, [sp, #0x164]
005fb74c: subs     r4, r4, #1
005fb750: str      r4, [sp, #0x164]
005fb754: beq      #0x5f9eb4
005fb758: ldr      r5, [sp, #0xa4]
005fb75c: ldr      r8, [sp, #0x9c]
005fb760: ldr      sl, [sp, #0xa8]
005fb764: ldr      r7, [sp, #0x5c]
005fb768: add      r8, r8, sl
005fb76c: add      r6, r5, r7
005fb770: str      r8, [sp, #0x9c]
005fb774: mov      sl, r8
005fb778: str      r6, [sp, #0xa4]
005fb77c: b        #0x5fb630
005fb780: cmp      r4, r3, lsl #1
005fb784: addle    r4, r4, r6
005fb788: rsble    r3, r3, r4
005fb78c: strble   r3, [r1, #0x14]
005fb790: b        #0x5fb44c
005fb794: tst      r3, #1
005fb798: bne      #0x5fc120
005fb79c: mov      r3, #0
005fb7a0: str      r3, [sp, #0x3c]
005fb7a4: ldr      r2, [r5, fp]
005fb7a8: mov      r3, #0x28
005fb7ac: add      ip, sp, #0xb4
005fb7b0: mla      r7, r3, r7, r2
005fb7b4: mla      r2, r3, sl, r2
005fb7b8: mov      r0, r7
005fb7bc: str      r7, [sp, #0x40]
005fb7c0: str      r2, [sp, #0x44]
005fb7c4: mov      r7, r2
005fb7c8: str      sl, [sp, #0x50]
005fb7cc: str      sb, [sp, #0x60]
005fb7d0: str      ip, [sp, #0x48]
005fb7d4: mov      r1, ip
005fb7d8: mov      r2, #0
005fb7dc: str      r6, [sp, #0x4c]
005fb7e0: str      r8, [sp, #0x58]
005fb7e4: str      r4, [sp, #0x64]
005fb7e8: mov      sb, r5
005fb7ec: mov      sl, r0
005fb7f0: add      r5, sl, r2
005fb7f4: ldrb     r3, [r7, #0x18]
005fb7f8: ldrb     r4, [r0, #0x18]
005fb7fc: ldr      r8, [r5, #4]
005fb800: ldrb     r6, [r0, #0x1c]
005fb804: ldrb     r5, [r7, #0x1c]
005fb808: cmp      r3, r4
005fb80c: str      r8, [ip, r2]
005fb810: strb     r5, [r1, #0x10]
005fb814: strb     r6, [r1, #0x14]
005fb818: bls      #0x5fbb5c
005fb81c: add      r3, r3, r5
005fb820: rsb      r3, r4, r3
005fb824: strb     r3, [r1, #0x10]
005fb828: add      r2, r2, #4
005fb82c: cmp      r2, #0x10
005fb830: add      r7, r7, #1
005fb834: add      r1, r1, #1
005fb838: add      r0, r0, #1
005fb83c: bne      #0x5fb7f0
005fb840: mov      r5, sb
005fb844: ldr      r2, [r5, fp]
005fb848: ldr      sl, [sp, #0x50]
005fb84c: mov      r3, #0x28
005fb850: ldr      r5, [sp, #0x3c]
005fb854: mla      sl, r3, sl, r2
005fb858: ldr      r3, [sp, #0xc0]
005fb85c: ldr      r6, [sp, #0x4c]
005fb860: ldr      r8, [sp, #0x58]
005fb864: ldr      r4, [sp, #0x64]
005fb868: ldr      sb, [sp, #0x60]
005fb86c: ldr      r0, [sp, #0x48]
005fb870: ldr      ip, [sp, #0x44]
005fb874: and      r3, r5, r3
005fb878: str      r3, [sp, #0x3c]
005fb87c: ldr      r2, [sp, #0x40]
005fb880: mov      r3, #0
005fb884: str      r6, [sp, #0x40]
005fb888: mov      fp, r8
005fb88c: str      r4, [sp, #0x34]
005fb890: ldrb     r6, [ip, #0x18]
005fb894: ldrb     r5, [r2, #0x18]
005fb898: add      r1, sl, r3, lsl #2
005fb89c: ldr      r1, [r1, #4]
005fb8a0: rsb      r5, r5, r6, lsl #1
005fb8a4: uxtb     r5, r5
005fb8a8: and      r7, r1, r1, lsl r5
005fb8ac: add      r6, sp, #0x130
005fb8b0: add      r8, r6, r3, lsl #2
005fb8b4: str      r1, [r8, #-0x60]
005fb8b8: str      r7, [r8, #-0x54]
005fb8bc: ldrb     r7, [r0, #0x10]
005fb8c0: ldrb     r1, [r2, #0x1c]
005fb8c4: add      r8, sp, #0x130
005fb8c8: add      r6, r8, r3
005fb8cc: add      r3, r3, #1
005fb8d0: sub      r6, r6, #0x4c
005fb8d4: add      r5, r5, r7
005fb8d8: cmp      r3, #3
005fb8dc: strb     r1, [r6, #7]
005fb8e0: strb     r5, [r6, #4]
005fb8e4: add      ip, ip, #1
005fb8e8: add      r2, r2, #1
005fb8ec: add      r0, r0, #1
005fb8f0: bne      #0x5fb890
005fb8f4: ldr      ip, [sp, #0x158]
005fb8f8: mov      r8, fp
005fb8fc: ldr      r6, [sp, #0x40]
005fb900: cmp      fp, ip
005fb904: ldr      r4, [sp, #0x34]
005fb908: ldrb     fp, [sl, #0x15]
005fb90c: beq      #0x5fc5bc
005fb910: ldr      sl, [sp, #0x38]
005fb914: str      sb, [sp, #0xa4]
005fb918: cmp      sl, #0
005fb91c: beq      #0x5fb93c
005fb920: ldr      ip, [sp, #0x164]
005fb924: ldr      r0, [sp, #0x158]
005fb928: rsb      r1, sb, #0
005fb92c: sub      r3, ip, #1
005fb930: mla      r3, r3, sb, r0
005fb934: str      r1, [sp, #0xa4]
005fb938: str      r3, [sp, #0xa8]
005fb93c: ldr      r2, [sp, #0x164]
005fb940: cmp      r2, #0
005fb944: beq      #0x5f9eb4
005fb948: ldrb     r3, [sp, #0xc8]
005fb94c: ldr      r5, [sp, #0xdc]
005fb950: ldrb     r7, [sp, #0xe8]
005fb954: ldr      ip, [sp, #0xb4]
005fb958: ldr      r0, [sp, #0xd4]
005fb95c: ldrb     r1, [sp, #0xc5]
005fb960: ldrb     r2, [sp, #0xc9]
005fb964: str      r8, [sp, #0xa0]
005fb968: ldrb     r8, [sp, #0xeb]
005fb96c: str      r3, [sp, #0x94]
005fb970: str      r5, [sp, #0x90]
005fb974: ldr      r3, [sp, #0xe0]
005fb978: ldrb     r5, [sp, #0xe9]
005fb97c: str      r7, [sp, #0x8c]
005fb980: str      r8, [sp, #0x88]
005fb984: ldrb     r7, [sp, #0xec]
005fb988: ldr      r8, [sp, #0xb8]
005fb98c: str      ip, [sp, #0x34]
005fb990: str      r0, [sp, #0x84]
005fb994: ldr      ip, [sp, #0xd8]
005fb998: ldrb     r0, [sp, #0xc6]
005fb99c: str      r1, [sp, #0x80]
005fb9a0: str      r2, [sp, #0x7c]
005fb9a4: ldrb     r1, [sp, #0xca]
005fb9a8: ldr      r2, [sp, #0xe4]
005fb9ac: ldrb     sl, [sp, #0xc4]
005fb9b0: str      r3, [sp, #0x78]
005fb9b4: str      r5, [sp, #0x74]
005fb9b8: str      r7, [sp, #0x70]
005fb9bc: str      r8, [sp, #0x38]
005fb9c0: str      ip, [sp, #0x6c]
005fb9c4: str      r0, [sp, #0x68]
005fb9c8: str      r1, [sp, #0x64]
005fb9cc: ldr      sb, [sp, #0xd0]
005fb9d0: str      r2, [sp, #0x60]
005fb9d4: ldr      r1, [sp, #0xa8]
005fb9d8: ldrb     r3, [sp, #0xea]
005fb9dc: ldrb     r5, [sp, #0xed]
005fb9e0: ldr      r7, [sp, #0xbc]
005fb9e4: ldrb     r8, [sp, #0xc7]
005fb9e8: ldrb     ip, [sp, #0xcb]
005fb9ec: ldr      r0, [sp, #0xc0]
005fb9f0: str      sl, [sp, #0x98]
005fb9f4: str      r3, [sp, #0x58]
005fb9f8: str      r5, [sp, #0x50]
005fb9fc: str      r7, [sp, #0x4c]
005fba00: str      r8, [sp, #0x48]
005fba04: str      ip, [sp, #0x44]
005fba08: str      r0, [sp, #0x40]
005fba0c: str      r1, [sp, #0x9c]
005fba10: mov      sl, r1
005fba14: cmp      r4, #0
005fba18: movne    r2, #0
005fba1c: strne    sl, [sp, #0x54]
005fba20: beq      #0x5fbb24
005fba24: ldrb     r3, [r6]
005fba28: ldr      r5, [sp, #0x98]
005fba2c: ldr      r8, [sp, #0x84]
005fba30: strb     r3, [sp, #0x120]
005fba34: ldrb     r3, [r6, #1]
005fba38: ldr      sl, [sp, #0x80]
005fba3c: ldr      ip, [sp, #0x6c]
005fba40: strb     r3, [sp, #0x121]
005fba44: ldrb     r3, [r6, #2]
005fba48: add      r6, fp, r6
005fba4c: strb     r3, [sp, #0x122]
005fba50: ldr      r3, [sp, #0x120]
005fba54: and      r7, sb, r3
005fba58: lsr      r7, r7, r5
005fba5c: ldr      r5, [sp, #0x94]
005fba60: and      r0, r3, r8
005fba64: lsr      r0, r0, sl
005fba68: lsl      r7, r7, r5
005fba6c: ldr      sl, [sp, #0x7c]
005fba70: ldr      r8, [sp, #0x68]
005fba74: and      r1, r3, ip
005fba78: lsl      r0, r0, sl
005fba7c: ldr      ip, [sp, #0x64]
005fba80: lsr      r1, r1, r8
005fba84: lsl      r1, r1, ip
005fba88: ldr      r5, [sp, #0x90]
005fba8c: ldr      sl, [sp, #0x8c]
005fba90: and      r8, r3, r5
005fba94: ldr      r5, [sp, #0x78]
005fba98: lsr      r8, r8, sl
005fba9c: ldr      sl, [sp, #0x74]
005fbaa0: and      ip, r3, r5
005fbaa4: lsr      ip, ip, sl
005fbaa8: ldr      sl, [sp, #0x60]
005fbaac: and      r5, sl, r3
005fbab0: ldr      sl, [sp, #0x88]
005fbab4: orr      r7, r7, r8, lsl sl
005fbab8: ldr      sl, [sp, #0x70]
005fbabc: ldr      r8, [sp, #0x58]
005fbac0: orr      r0, r0, ip, lsl sl
005fbac4: ldr      ip, [sp, #0x50]
005fbac8: lsr      r5, r5, r8
005fbacc: orr      r5, r1, r5, lsl ip
005fbad0: ldr      r1, [sp, #0x48]
005fbad4: ldr      r8, [sp, #0x40]
005fbad8: ldr      sl, [sp, #0x44]
005fbadc: lsr      r3, r3, r1
005fbae0: and      r3, r8, r3, lsl sl
005fbae4: ldr      ip, [sp, #0x34]
005fbae8: ldr      r1, [sp, #0x3c]
005fbaec: ldr      r8, [sp, #0x38]
005fbaf0: ldr      sl, [sp, #0x4c]
005fbaf4: and      r7, r7, ip
005fbaf8: orr      r7, r1, r7
005fbafc: and      r0, r0, r8
005fbb00: orr      r0, r7, r0
005fbb04: and      r5, r5, sl
005fbb08: ldr      ip, [sp, #0x54]
005fbb0c: orr      r0, r0, r5
005fbb10: orr      r0, r0, r3
005fbb14: strb     r0, [ip, r2]
005fbb18: add      r2, r2, #1
005fbb1c: cmp      r4, r2
005fbb20: bne      #0x5fba24
005fbb24: ldr      r0, [sp, #0x164]
005fbb28: subs     r0, r0, #1
005fbb2c: str      r0, [sp, #0x164]
005fbb30: beq      #0x5f9eb4
005fbb34: ldr      r1, [sp, #0xa0]
005fbb38: ldr      r3, [sp, #0x9c]
005fbb3c: ldr      r2, [sp, #0x5c]
005fbb40: ldr      r5, [sp, #0xa4]
005fbb44: add      r6, r1, r2
005fbb48: add      r3, r3, r5
005fbb4c: str      r3, [sp, #0x9c]
005fbb50: mov      sl, r3
005fbb54: str      r6, [sp, #0xa0]
005fbb58: b        #0x5fba14
005fbb5c: cmp      r4, r3, lsl #1
005fbb60: addle    r4, r4, r6
005fbb64: rsble    r3, r3, r4
005fbb68: strble   r3, [r1, #0x14]
005fbb6c: b        #0x5fb828
005fbb70: tst      r3, #1
005fbb74: bne      #0x5fc0f8
005fbb78: mov      r3, #0
005fbb7c: str      r3, [sp, #0x40]
005fbb80: ldr      r2, [r5, fp]
005fbb84: mov      r3, #0x28
005fbb88: add      ip, sp, #0xb4
005fbb8c: mla      r7, r3, r7, r2
005fbb90: mla      r2, r3, sl, r2
005fbb94: mov      r0, r7
005fbb98: str      r7, [sp, #0x3c]
005fbb9c: str      r2, [sp, #0x44]
005fbba0: mov      r7, r2
005fbba4: str      sl, [sp, #0x50]
005fbba8: str      sb, [sp, #0x60]
005fbbac: str      ip, [sp, #0x48]
005fbbb0: mov      r1, ip
005fbbb4: mov      r2, #0
005fbbb8: str      r6, [sp, #0x4c]
005fbbbc: str      r8, [sp, #0x58]
005fbbc0: str      r4, [sp, #0x64]
005fbbc4: mov      sb, r5
005fbbc8: mov      sl, r0
005fbbcc: add      r5, sl, r2
005fbbd0: ldrb     r3, [r7, #0x18]
005fbbd4: ldrb     r4, [r0, #0x18]
005fbbd8: ldr      r8, [r5, #4]
005fbbdc: ldrb     r6, [r0, #0x1c]
005fbbe0: ldrb     r5, [r7, #0x1c]
005fbbe4: cmp      r3, r4
005fbbe8: str      r8, [ip, r2]
005fbbec: strb     r5, [r1, #0x10]
005fbbf0: strb     r6, [r1, #0x14]
005fbbf4: bls      #0x5fbf38
005fbbf8: add      r3, r3, r5
005fbbfc: rsb      r3, r4, r3
005fbc00: strb     r3, [r1, #0x10]
005fbc04: add      r2, r2, #4
005fbc08: cmp      r2, #0x10
005fbc0c: add      r7, r7, #1
005fbc10: add      r1, r1, #1
005fbc14: add      r0, r0, #1
005fbc18: bne      #0x5fbbcc
005fbc1c: mov      r5, sb
005fbc20: ldr      r2, [r5, fp]
005fbc24: ldr      sl, [sp, #0x50]
005fbc28: mov      r3, #0x28
005fbc2c: ldr      r5, [sp, #0x40]
005fbc30: mla      sl, r3, sl, r2
005fbc34: ldr      r3, [sp, #0xc0]
005fbc38: ldr      r6, [sp, #0x4c]
005fbc3c: ldr      r8, [sp, #0x58]
005fbc40: ldr      r4, [sp, #0x64]
005fbc44: ldr      sb, [sp, #0x60]
005fbc48: ldr      r0, [sp, #0x44]
005fbc4c: ldr      ip, [sp, #0x48]
005fbc50: and      r3, r5, r3
005fbc54: str      r3, [sp, #0x40]
005fbc58: ldr      r2, [sp, #0x3c]
005fbc5c: mov      r3, #0
005fbc60: str      r6, [sp, #0x3c]
005fbc64: mov      fp, r8
005fbc68: str      r4, [sp, #0x34]
005fbc6c: ldrb     r6, [r0, #0x18]
005fbc70: ldrb     r5, [r2, #0x18]
005fbc74: add      r1, sl, r3, lsl #2
005fbc78: ldr      r1, [r1, #4]
005fbc7c: rsb      r5, r5, r6, lsl #1
005fbc80: uxtb     r5, r5
005fbc84: and      r7, r1, r1, lsl r5
005fbc88: add      r6, sp, #0x130
005fbc8c: add      r8, r6, r3, lsl #2
005fbc90: str      r1, [r8, #-0x60]
005fbc94: str      r7, [r8, #-0x54]
005fbc98: ldrb     r7, [ip, #0x10]
005fbc9c: ldrb     r1, [r2, #0x1c]
005fbca0: add      r8, sp, #0x130
005fbca4: add      r6, r8, r3
005fbca8: add      r3, r3, #1
005fbcac: sub      r6, r6, #0x4c
005fbcb0: add      r5, r5, r7
005fbcb4: cmp      r3, #3
005fbcb8: strb     r1, [r6, #7]
005fbcbc: strb     r5, [r6, #4]
005fbcc0: add      r0, r0, #1
005fbcc4: add      r2, r2, #1
005fbcc8: add      ip, ip, #1
005fbccc: bne      #0x5fbc6c
005fbcd0: ldr      ip, [sp, #0x38]
005fbcd4: ldrb     sl, [sl, #0x15]
005fbcd8: ldr      r6, [sp, #0x3c]
005fbcdc: cmp      ip, #0
005fbce0: mov      r8, fp
005fbce4: ldr      r4, [sp, #0x34]
005fbce8: str      sl, [sp, #0x38]
005fbcec: str      sb, [sp, #0xa8]
005fbcf0: beq      #0x5fbd10
005fbcf4: ldr      r0, [sp, #0x164]
005fbcf8: ldr      r1, [sp, #0x158]
005fbcfc: rsb      r2, sb, #0
005fbd00: sub      r3, r0, #1
005fbd04: mla      r3, r3, sb, r1
005fbd08: str      r2, [sp, #0xa8]
005fbd0c: str      r3, [sp, #0xa0]
005fbd10: ldr      r3, [sp, #0x164]
005fbd14: cmp      r3, #0
005fbd18: beq      #0x5f9eb4
005fbd1c: ldrb     r5, [sp, #0xc8]
005fbd20: ldr      r7, [sp, #0xdc]
005fbd24: ldrb     sl, [sp, #0xeb]
005fbd28: ldr      ip, [sp, #0xb4]
005fbd2c: ldr      r0, [sp, #0xd4]
005fbd30: ldrb     r1, [sp, #0xc5]
005fbd34: str      r8, [sp, #0xa4]
005fbd38: ldrb     r8, [sp, #0xe8]
005fbd3c: ldrb     r2, [sp, #0xc9]
005fbd40: ldr      r3, [sp, #0xe0]
005fbd44: str      r5, [sp, #0x98]
005fbd48: str      r7, [sp, #0x94]
005fbd4c: ldrb     r5, [sp, #0xe9]
005fbd50: ldrb     r7, [sp, #0xec]
005fbd54: str      r8, [sp, #0x90]
005fbd58: str      sl, [sp, #0x8c]
005fbd5c: ldr      r8, [sp, #0xb8]
005fbd60: ldr      sl, [sp, #0xd8]
005fbd64: str      ip, [sp, #0x34]
005fbd68: str      r0, [sp, #0x88]
005fbd6c: ldrb     ip, [sp, #0xc6]
005fbd70: str      r1, [sp, #0x84]
005fbd74: ldrb     r0, [sp, #0xca]
005fbd78: ldr      r1, [sp, #0xe4]
005fbd7c: str      r2, [sp, #0x80]
005fbd80: str      r3, [sp, #0x7c]
005fbd84: str      r5, [sp, #0x78]
005fbd88: str      r7, [sp, #0x74]
005fbd8c: str      r8, [sp, #0x70]
005fbd90: str      sl, [sp, #0x3c]
005fbd94: str      ip, [sp, #0x6c]
005fbd98: ldr      sb, [sp, #0xd0]
005fbd9c: ldrb     fp, [sp, #0xc4]
005fbda0: str      r0, [sp, #0x68]
005fbda4: str      r1, [sp, #0x64]
005fbda8: ldr      ip, [sp, #0xa0]
005fbdac: ldr      sl, [sp, #0xc0]
005fbdb0: ldrb     r2, [sp, #0xea]
005fbdb4: ldrb     r3, [sp, #0xed]
005fbdb8: ldr      r5, [sp, #0xbc]
005fbdbc: ldrb     r7, [sp, #0xc7]
005fbdc0: ldrb     r8, [sp, #0xcb]
005fbdc4: str      sl, [sp, #0x44]
005fbdc8: str      r2, [sp, #0x60]
005fbdcc: str      r3, [sp, #0x58]
005fbdd0: str      r5, [sp, #0x50]
005fbdd4: str      r7, [sp, #0x4c]
005fbdd8: str      r8, [sp, #0x48]
005fbddc: str      ip, [sp, #0x9c]
005fbde0: mov      sl, ip
005fbde4: str      r4, [sp, #0xa0]
005fbde8: ldr      ip, [sp, #0xa0]
005fbdec: cmp      ip, #0
005fbdf0: beq      #0x5fbf00
005fbdf4: ldr      r1, [sp, #0xa0]
005fbdf8: mov      r2, #0
005fbdfc: str      sl, [sp, #0x54]
005fbe00: ldrb     r3, [r6]
005fbe04: ldr      r4, [sp, #0x88]
005fbe08: ldr      r5, [sp, #0x84]
005fbe0c: strb     r3, [sp, #0x104]
005fbe10: ldrb     r3, [r6, #1]
005fbe14: ldr      sl, [sp, #0x98]
005fbe18: ldr      r0, [sp, #0x38]
005fbe1c: strb     r3, [sp, #0x105]
005fbe20: ldrb     r3, [r6, #2]
005fbe24: ldr      r8, [sp, #0x3c]
005fbe28: add      r6, r6, r0
005fbe2c: strb     r3, [sp, #0x106]
005fbe30: ldr      r3, [sp, #0x104]
005fbe34: subs     r1, r1, #1
005fbe38: and      r7, r3, sb
005fbe3c: lsr      r7, r7, fp
005fbe40: and      ip, r3, r4
005fbe44: lsr      ip, ip, r5
005fbe48: lsl      r7, r7, sl
005fbe4c: ldr      r5, [sp, #0x80]
005fbe50: ldr      r4, [sp, #0x6c]
005fbe54: and      r0, r8, r3
005fbe58: lsl      ip, ip, r5
005fbe5c: ldr      r8, [sp, #0x68]
005fbe60: lsr      r0, r0, r4
005fbe64: lsl      r0, r0, r8
005fbe68: ldr      sl, [sp, #0x94]
005fbe6c: ldr      r4, [sp, #0x90]
005fbe70: ldr      r5, [sp, #0x7c]
005fbe74: and      r8, sl, r3
005fbe78: ldr      sl, [sp, #0x78]
005fbe7c: lsr      r8, r8, r4
005fbe80: and      r4, r3, r5
005fbe84: lsr      r4, r4, sl
005fbe88: ldr      sl, [sp, #0x64]
005fbe8c: and      r5, r3, sl
005fbe90: ldr      sl, [sp, #0x8c]
005fbe94: orr      r7, r7, r8, lsl sl
005fbe98: ldr      sl, [sp, #0x74]
005fbe9c: ldr      r8, [sp, #0x60]
005fbea0: orr      ip, ip, r4, lsl sl
005fbea4: ldr      r4, [sp, #0x58]
005fbea8: lsr      r5, r5, r8
005fbeac: ldr      r8, [sp, #0x4c]
005fbeb0: orr      r5, r0, r5, lsl r4
005fbeb4: ldr      sl, [sp, #0x44]
005fbeb8: ldr      r0, [sp, #0x48]
005fbebc: lsr      r3, r3, r8
005fbec0: and      r3, sl, r3, lsl r0
005fbec4: ldr      r4, [sp, #0x34]
005fbec8: ldr      r8, [sp, #0x40]
005fbecc: ldr      sl, [sp, #0x70]
005fbed0: ldr      r0, [sp, #0x50]
005fbed4: and      r7, r7, r4
005fbed8: orr      r7, r8, r7
005fbedc: and      ip, ip, sl
005fbee0: orr      ip, r7, ip
005fbee4: and      r5, r5, r0
005fbee8: orr      ip, ip, r5
005fbeec: orr      ip, ip, r3
005fbef0: ldr      r3, [sp, #0x54]
005fbef4: str      ip, [r3, r2]
005fbef8: add      r2, r2, #4
005fbefc: bne      #0x5fbe00
005fbf00: ldr      r4, [sp, #0x164]
005fbf04: subs     r4, r4, #1
005fbf08: str      r4, [sp, #0x164]
005fbf0c: beq      #0x5f9eb4
005fbf10: ldr      r5, [sp, #0xa4]
005fbf14: ldr      r8, [sp, #0x9c]
005fbf18: ldr      sl, [sp, #0xa8]
005fbf1c: ldr      r7, [sp, #0x5c]
005fbf20: add      r8, r8, sl
005fbf24: add      r6, r5, r7
005fbf28: str      r8, [sp, #0x9c]
005fbf2c: mov      sl, r8
005fbf30: str      r6, [sp, #0xa4]
005fbf34: b        #0x5fbde8
005fbf38: cmp      r4, r3, lsl #1
005fbf3c: addle    r4, r4, r6
005fbf40: rsble    r3, r3, r4
005fbf44: strble   r3, [r1, #0x14]
005fbf48: b        #0x5fbc04
005fbf4c: tst      r2, #1
005fbf50: mvneq    r0, #0
005fbf54: streq    r0, [sp, #0x40]
005fbf58: beq      #0x5fa780
005fbf5c: b        #0x5fa778
005fbf60: tst      r2, #1
005fbf64: mvneq    r0, #0
005fbf68: streq    r0, [sp, #0x40]
005fbf6c: beq      #0x5faa08
005fbf70: b        #0x5faa00
005fbf74: tst      r2, #1
005fbf78: mvneq    ip, #0
005fbf7c: streq    ip, [sp, #0x3c]
005fbf80: beq      #0x5f9f2c
005fbf84: b        #0x5f9f24
005fbf88: ldr      r6, [sp, #0x38]
005fbf8c: cmp      r6, #0
005fbf90: bne      #0x5fc134
005fbf94: ldr      r7, [sp, #0x164]
005fbf98: cmp      r7, #0
005fbf9c: beq      #0x5f9eb4
005fbfa0: ldr      r8, [sp, #0xc0]
005fbfa4: ldrb     sl, [sp, #0xcc]
005fbfa8: ldrb     r6, [sp, #0xce]
005fbfac: str      r2, [sp, #0x34]
005fbfb0: ldr      ip, [sp, #0xb4]
005fbfb4: ldr      r0, [sp, #0xc4]
005fbfb8: ldrb     r1, [sp, #0xcd]
005fbfbc: ldr      r2, [sp, #0xb8]
005fbfc0: ldr      r3, [sp, #0xc8]
005fbfc4: ldr      r7, [sp, #0xbc]
005fbfc8: str      r8, [sp, #0x64]
005fbfcc: str      sl, [sp, #0x60]
005fbfd0: str      r6, [sp, #0x3c]
005fbfd4: str      ip, [sp, #0x50]
005fbfd8: str      r0, [sp, #0x4c]
005fbfdc: str      r1, [sp, #0x48]
005fbfe0: str      r2, [sp, #0x44]
005fbfe4: str      r3, [sp, #0x40]
005fbfe8: str      r7, [sp, #0x38]
005fbfec: mov      r8, fp
005fbff0: mov      r6, fp
005fbff4: add      sl, sp, #0xf0
005fbff8: str      sb, [sp, #0x6c]
005fbffc: str      r4, [sp, #0x68]
005fc000: ldr      r7, [sp, #0x68]
005fc004: cmp      r7, #0
005fc008: beq      #0x5fc0c0
005fc00c: ldr      r7, [sp, #0x68]
005fc010: mov      r4, #0
005fc014: mov      r2, r5
005fc018: mov      r1, r6
005fc01c: mov      r0, sl
005fc020: bl       #0x30e868
005fc024: ldr      lr, [sp, #0x64]
005fc028: ldrh     sb, [sp, #0xf0]
005fc02c: ldr      r1, [sp, #0x60]
005fc030: add      r6, r6, r5
005fc034: and      r0, sb, lr
005fc038: lsr      r0, r0, r1
005fc03c: bl       #0x30e2e0
005fc040: ldr      r1, [sp, #0x50]
005fc044: bl       #0x30ed6c
005fc048: ldr      r2, [sp, #0x4c]
005fc04c: ldr      r3, [sp, #0x48]
005fc050: mov      fp, r0
005fc054: and      r0, sb, r2
005fc058: lsr      r0, r0, r3
005fc05c: bl       #0x30e2e0
005fc060: ldr      r1, [sp, #0x44]
005fc064: bl       #0x30ed6c
005fc068: mov      r1, r0
005fc06c: mov      r0, fp
005fc070: bl       #0x30eba4
005fc074: ldr      ip, [sp, #0x40]
005fc078: ldr      lr, [sp, #0x3c]
005fc07c: mov      fp, r0
005fc080: and      r0, sb, ip
005fc084: lsr      r0, r0, lr
005fc088: bl       #0x30e2e0
005fc08c: ldr      r1, [sp, #0x38]
005fc090: bl       #0x30ed6c
005fc094: mov      r1, r0
005fc098: mov      r0, fp
005fc09c: bl       #0x30eba4
005fc0a0: movw     r1, #0xff00
005fc0a4: movt     r1, #0x477f
005fc0a8: bl       #0x30ed6c
005fc0ac: bl       #0x8be2a0
005fc0b0: subs     r7, r7, #1
005fc0b4: strh     r0, [r8, r4]
005fc0b8: add      r4, r4, #2
005fc0bc: bne      #0x5fc014
005fc0c0: ldr      r0, [sp, #0x164]
005fc0c4: subs     r0, r0, #1
005fc0c8: str      r0, [sp, #0x164]
005fc0cc: beq      #0x5f9eb4
005fc0d0: ldr      r1, [sp, #0x34]
005fc0d4: ldr      r3, [sp, #0x58]
005fc0d8: ldr      r2, [sp, #0x5c]
005fc0dc: ldr      r4, [sp, #0x6c]
005fc0e0: add      r6, r1, r2
005fc0e4: add      r3, r3, r4
005fc0e8: str      r3, [sp, #0x58]
005fc0ec: str      r6, [sp, #0x34]
005fc0f0: mov      r8, r3
005fc0f4: b        #0x5fc000
005fc0f8: tst      r2, #1
005fc0fc: mvneq    r2, #0
005fc100: streq    r2, [sp, #0x40]
005fc104: beq      #0x5fbb80
005fc108: b        #0x5fbb78
005fc10c: tst      r2, #1
005fc110: mvneq    r2, #0
005fc114: streq    r2, [sp, #0x3c]
005fc118: beq      #0x5fb3c8
005fc11c: b        #0x5fb3c0
005fc120: tst      r2, #1
005fc124: mvneq    r2, #0
005fc128: streq    r2, [sp, #0x3c]
005fc12c: beq      #0x5fb7a4
005fc130: b        #0x5fb79c
005fc134: ldr      r8, [sp, #0x164]
005fc138: ldr      sl, [sp, #0x158]
005fc13c: sub      r6, r8, #1
005fc140: mla      r6, r6, sb, sl
005fc144: rsb      sb, sb, #0
005fc148: cmp      sl, r6
005fc14c: str      sb, [sp, #0x68]
005fc150: bhi      #0x5f9eb4
005fc154: ldrb     ip, [sp, #0xcc]
005fc158: str      sl, [sp, #0x34]
005fc15c: ldr      r0, [sp, #0xb4]
005fc160: ldr      r1, [sp, #0xc4]
005fc164: ldrb     r2, [sp, #0xcd]
005fc168: ldr      r3, [sp, #0xb8]
005fc16c: ldr      r7, [sp, #0xc8]
005fc170: ldrb     r8, [sp, #0xce]
005fc174: ldr      sl, [sp, #0xbc]
005fc178: str      r4, [sp, #0x64]
005fc17c: ldr      fp, [sp, #0xc0]
005fc180: ldr      r4, [sp, #0x58]
005fc184: str      ip, [sp, #0x50]
005fc188: add      ip, sp, #0xf0
005fc18c: str      r0, [sp, #0x4c]
005fc190: str      r1, [sp, #0x48]
005fc194: str      r2, [sp, #0x44]
005fc198: str      r3, [sp, #0x40]
005fc19c: str      r7, [sp, #0x3c]
005fc1a0: str      r8, [sp, #0x38]
005fc1a4: str      sl, [sp, #0x54]
005fc1a8: str      ip, [sp, #0x60]
005fc1ac: ldr      ip, [sp, #0x64]
005fc1b0: cmp      ip, #0
005fc1b4: beq      #0x5fc2f0
005fc1b8: ldr      r8, [sp, #0x64]
005fc1bc: mov      r7, #0
005fc1c0: ldrh     sl, [r6, r7]
005fc1c4: ldr      lr, [sp, #0x50]
005fc1c8: and      r0, sl, fp
005fc1cc: lsr      r0, r0, lr
005fc1d0: bl       #0x30e2e0
005fc1d4: ldr      r1, [sp, #0x4c]
005fc1d8: bl       #0x30ed6c
005fc1dc: ldr      r1, [sp, #0x48]
005fc1e0: ldr      r2, [sp, #0x44]
005fc1e4: mov      sb, r0
005fc1e8: and      r0, sl, r1
005fc1ec: lsr      r0, r0, r2
005fc1f0: bl       #0x30e2e0
005fc1f4: ldr      r1, [sp, #0x40]
005fc1f8: bl       #0x30ed6c
005fc1fc: mov      r1, r0
005fc200: mov      r0, sb
005fc204: bl       #0x30eba4
005fc208: ldr      r3, [sp, #0x3c]
005fc20c: mov      sb, r0
005fc210: and      r0, sl, r3
005fc214: ldr      sl, [sp, #0x38]
005fc218: lsr      r0, r0, sl
005fc21c: bl       #0x30e2e0
005fc220: ldr      r1, [sp, #0x54]
005fc224: bl       #0x30ed6c
005fc228: mov      r1, r0
005fc22c: mov      r0, sb
005fc230: bl       #0x30eba4
005fc234: movw     r1, #0xff00
005fc238: movt     r1, #0x477f
005fc23c: bl       #0x30ed6c
005fc240: bl       #0x8be2a0
005fc244: strh     r0, [sp, #0xf0]
005fc248: ldrh     sl, [r4]
005fc24c: ldr      ip, [sp, #0x50]
005fc250: and      r0, sl, fp
005fc254: lsr      r0, r0, ip
005fc258: bl       #0x30e2e0
005fc25c: ldr      r1, [sp, #0x4c]
005fc260: bl       #0x30ed6c
005fc264: ldr      lr, [sp, #0x48]
005fc268: ldr      r1, [sp, #0x44]
005fc26c: mov      sb, r0
005fc270: and      r0, sl, lr
005fc274: lsr      r0, r0, r1
005fc278: bl       #0x30e2e0
005fc27c: ldr      r1, [sp, #0x40]
005fc280: bl       #0x30ed6c
005fc284: mov      r1, r0
005fc288: mov      r0, sb
005fc28c: bl       #0x30eba4
005fc290: ldr      r2, [sp, #0x3c]
005fc294: ldr      r3, [sp, #0x38]
005fc298: mov      sb, r0
005fc29c: and      r0, sl, r2
005fc2a0: lsr      r0, r0, r3
005fc2a4: bl       #0x30e2e0
005fc2a8: ldr      r1, [sp, #0x54]
005fc2ac: bl       #0x30ed6c
005fc2b0: mov      r1, r0
005fc2b4: mov      r0, sb
005fc2b8: bl       #0x30eba4
005fc2bc: movw     r1, #0xff00
005fc2c0: movt     r1, #0x477f
005fc2c4: bl       #0x30ed6c
005fc2c8: bl       #0x8be2a0
005fc2cc: mov      r2, r5
005fc2d0: strh     r0, [r6, r7]
005fc2d4: mov      r0, r4
005fc2d8: ldr      r1, [sp, #0x60]
005fc2dc: bl       #0x30e868
005fc2e0: subs     r8, r8, #1
005fc2e4: add      r4, r4, r5
005fc2e8: add      r7, r7, #2
005fc2ec: bne      #0x5fc1c0
005fc2f0: ldr      r7, [sp, #0x34]
005fc2f4: ldr      r8, [sp, #0x5c]
005fc2f8: ldr      sl, [sp, #0x68]
005fc2fc: add      r4, r7, r8
005fc300: add      r6, r6, sl
005fc304: cmp      r4, r6
005fc308: bhi      #0x5f9eb4
005fc30c: str      r4, [sp, #0x34]
005fc310: b        #0x5fc1ac
005fc314: ldr      ip, [sp, #0x38]
005fc318: cmp      ip, #0
005fc31c: bne      #0x5fcec0
005fc320: ldr      r0, [sp, #0x164]
005fc324: cmp      r0, #0
005fc328: beq      #0x5f9eb4
005fc32c: ldrb     r3, [sp, #0xc9]
005fc330: ldr      r1, [sp, #0xb4]
005fc334: ldrb     r2, [sp, #0xc5]
005fc338: str      r8, [sp, #0x68]
005fc33c: ldrb     r7, [sp, #0xc6]
005fc340: str      r3, [sp, #0x58]
005fc344: ldr      r3, [sp, #0x68]
005fc348: str      r1, [sp, #0x64]
005fc34c: str      r2, [sp, #0x60]
005fc350: ldr      r5, [sp, #0xb8]
005fc354: ldrb     ip, [sp, #0xca]
005fc358: ldr      r0, [sp, #0xbc]
005fc35c: ldrb     r1, [sp, #0xc7]
005fc360: ldrb     r2, [sp, #0xcb]
005fc364: ldrb     r8, [sp, #0xc4]
005fc368: ldrb     sl, [sp, #0xc8]
005fc36c: str      r7, [sp, #0x4c]
005fc370: str      r3, [sp, #0x6c]
005fc374: mov      r7, r3
005fc378: add      r3, sp, #0x120
005fc37c: str      sb, [sp, #0x70]
005fc380: str      r5, [sp, #0x50]
005fc384: str      ip, [sp, #0x48]
005fc388: str      r0, [sp, #0x44]
005fc38c: str      r1, [sp, #0x38]
005fc390: str      r2, [sp, #0x34]
005fc394: mov      sb, r3
005fc398: cmp      r4, #0
005fc39c: movne    r5, #0
005fc3a0: strne    r6, [sp, #0x54]
005fc3a4: beq      #0x5fc448
005fc3a8: mov      r1, r7
005fc3ac: mov      r2, fp
005fc3b0: mov      r0, sb
005fc3b4: bl       #0x30e868
005fc3b8: ldrb     r1, [sp, #0x120]
005fc3bc: ldrb     r2, [sp, #0x121]
005fc3c0: ldrb     r3, [sp, #0x122]
005fc3c4: strb     r1, [sp, #0x114]
005fc3c8: strb     r2, [sp, #0x115]
005fc3cc: strb     r3, [sp, #0x116]
005fc3d0: ldr      r3, [sp, #0x114]
005fc3d4: ldr      r6, [sp, #0x60]
005fc3d8: ldr      ip, [sp, #0x4c]
005fc3dc: lsr      r2, r3, r8
005fc3e0: lsr      r1, r3, r6
005fc3e4: lsr      r0, r3, ip
005fc3e8: ldr      r6, [sp, #0x50]
005fc3ec: ldr      ip, [sp, #0x58]
005fc3f0: add      r7, r7, fp
005fc3f4: and      r1, r6, r1, lsl ip
005fc3f8: ldr      r6, [sp, #0x64]
005fc3fc: ldr      ip, [sp, #0x38]
005fc400: and      r2, r6, r2, lsl sl
005fc404: lsr      r3, r3, ip
005fc408: ldr      r6, [sp, #0x44]
005fc40c: ldr      ip, [sp, #0x48]
005fc410: orr      r2, r1, r2
005fc414: ldr      r1, [sp, #0x54]
005fc418: and      r0, r6, r0, lsl ip
005fc41c: ldr      r6, [sp, #0x40]
005fc420: ldr      ip, [sp, #0x34]
005fc424: orr      r2, r2, r0
005fc428: ldr      r0, [sp, #0x3c]
005fc42c: and      r3, r6, r3, lsl ip
005fc430: orr      r3, r2, r3
005fc434: orr      r3, r3, r0
005fc438: strb     r3, [r1, r5]
005fc43c: add      r5, r5, #1
005fc440: cmp      r4, r5
005fc444: bne      #0x5fc3a8
005fc448: ldr      r2, [sp, #0x164]
005fc44c: subs     r2, r2, #1
005fc450: str      r2, [sp, #0x164]
005fc454: beq      #0x5f9eb4
005fc458: ldr      r3, [sp, #0x6c]
005fc45c: ldr      r6, [sp, #0x68]
005fc460: ldr      r5, [sp, #0x5c]
005fc464: ldr      ip, [sp, #0x70]
005fc468: add      r7, r3, r5
005fc46c: add      r6, r6, ip
005fc470: str      r6, [sp, #0x68]
005fc474: str      r7, [sp, #0x6c]
005fc478: b        #0x5fc398
005fc47c: ldr      r1, [sp, #0x38]
005fc480: cmp      r1, #0
005fc484: bne      #0x5f9e14
005fc488: ldr      r2, [sp, #0x164]
005fc48c: cmp      r2, #0
005fc490: beq      #0x5f9eb4
005fc494: mov      fp, r0
005fc498: str      r0, [sp, #0x34]
005fc49c: mov      r6, r0
005fc4a0: add      r8, sp, #0x10c
005fc4a4: mov      sl, r4
005fc4a8: cmp      sl, #0
005fc4ac: movne    r4, sl
005fc4b0: beq      #0x5fc4ec
005fc4b4: mov      r1, r6
005fc4b8: mov      r0, r8
005fc4bc: mov      r2, r7
005fc4c0: bl       #0x30e868
005fc4c4: ldrb     r3, [sp, #0x12c]
005fc4c8: subs     r4, r4, #1
005fc4cc: add      r6, r6, r7
005fc4d0: ldrb     r3, [r8, r3]
005fc4d4: strb     r3, [r5]
005fc4d8: ldrb     r3, [sp, #0x12d]
005fc4dc: ldrb     r3, [r8, r3]
005fc4e0: strb     r3, [r5, #1]
005fc4e4: add      r5, r5, #2
005fc4e8: bne      #0x5fc4b4
005fc4ec: ldr      r1, [sp, #0x164]
005fc4f0: subs     r1, r1, #1
005fc4f4: str      r1, [sp, #0x164]
005fc4f8: beq      #0x5f9eb4
005fc4fc: ldr      r2, [sp, #0x34]
005fc500: ldr      r3, [sp, #0x5c]
005fc504: add      fp, fp, sb
005fc508: mov      r5, fp
005fc50c: add      r6, r2, r3
005fc510: str      r6, [sp, #0x34]
005fc514: b        #0x5fc4a8
005fc518: ldr      r2, [sp, #0x38]
005fc51c: cmp      r2, #0
005fc520: bne      #0x5fd7e4
005fc524: ldr      r3, [sp, #0x164]
005fc528: cmp      r3, #0
005fc52c: beq      #0x5f9eb4
005fc530: mov      fp, r1
005fc534: str      r1, [sp, #0x34]
005fc538: mov      r6, r1
005fc53c: add      r8, sp, #0xf0
005fc540: mov      sl, r4
005fc544: cmp      sl, #0
005fc548: movne    r4, sl
005fc54c: beq      #0x5fc590
005fc550: mov      r1, r6
005fc554: mov      r0, r8
005fc558: mov      r2, r7
005fc55c: bl       #0x30e868
005fc560: ldrb     r3, [sp, #0x10c]
005fc564: subs     r4, r4, #1
005fc568: add      r6, r6, r7
005fc56c: lsl      r3, r3, #1
005fc570: ldrh     r3, [r8, r3]
005fc574: strh     r3, [r5]
005fc578: ldrb     r3, [sp, #0x10d]
005fc57c: lsl      r3, r3, #1
005fc580: ldrh     r3, [r8, r3]
005fc584: strh     r3, [r5, #2]
005fc588: add      r5, r5, #4
005fc58c: bne      #0x5fc550
005fc590: ldr      r2, [sp, #0x164]
005fc594: subs     r2, r2, #1
005fc598: str      r2, [sp, #0x164]
005fc59c: beq      #0x5f9eb4
005fc5a0: ldr      r3, [sp, #0x34]
005fc5a4: ldr      r4, [sp, #0x5c]
005fc5a8: add      fp, fp, sb
005fc5ac: mov      r5, fp
005fc5b0: add      r6, r3, r4
005fc5b4: str      r6, [sp, #0x34]
005fc5b8: b        #0x5fc544
005fc5bc: ldr      r0, [sp, #0x38]
005fc5c0: cmp      r0, #0
005fc5c4: beq      #0x5fd5a0
005fc5c8: ldr      r0, [sp, #0x164]
005fc5cc: sub      r3, r0, #1
005fc5d0: mla      r3, r3, sb, r8
005fc5d4: rsb      sb, sb, #0
005fc5d8: cmp      r8, r3
005fc5dc: str      sb, [sp, #0xac]
005fc5e0: bhi      #0x5f9eb4
005fc5e4: ldr      r1, [sp, #0xd0]
005fc5e8: ldrb     r2, [sp, #0xc4]
005fc5ec: ldrb     r5, [sp, #0xc8]
005fc5f0: ldr      r7, [sp, #0xdc]
005fc5f4: ldrb     sl, [sp, #0xeb]
005fc5f8: ldr      ip, [sp, #0xb4]
005fc5fc: ldr      r0, [sp, #0xd4]
005fc600: str      r8, [sp, #0xa4]
005fc604: str      r1, [sp, #0x98]
005fc608: ldrb     r8, [sp, #0xe8]
005fc60c: ldrb     r1, [sp, #0xc5]
005fc610: str      r2, [sp, #0x94]
005fc614: str      r5, [sp, #0x90]
005fc618: ldrb     r2, [sp, #0xc9]
005fc61c: ldr      r5, [sp, #0xe0]
005fc620: str      r7, [sp, #0x8c]
005fc624: str      r8, [sp, #0x88]
005fc628: ldrb     r7, [sp, #0xe9]
005fc62c: ldrb     r8, [sp, #0xec]
005fc630: str      sl, [sp, #0x84]
005fc634: str      ip, [sp, #0x34]
005fc638: ldr      sl, [sp, #0xb8]
005fc63c: ldr      ip, [sp, #0xd8]
005fc640: str      r0, [sp, #0x80]
005fc644: str      r1, [sp, #0x7c]
005fc648: ldrb     r0, [sp, #0xc6]
005fc64c: ldrb     r1, [sp, #0xca]
005fc650: str      r2, [sp, #0x78]
005fc654: str      r5, [sp, #0x74]
005fc658: str      r7, [sp, #0x70]
005fc65c: str      r8, [sp, #0x6c]
005fc660: str      sl, [sp, #0x54]
005fc664: str      ip, [sp, #0x68]
005fc668: str      r0, [sp, #0x64]
005fc66c: str      r1, [sp, #0x60]
005fc670: ldr      r2, [sp, #0xe4]
005fc674: ldrb     r5, [sp, #0xea]
005fc678: ldrb     r7, [sp, #0xed]
005fc67c: ldr      r8, [sp, #0xbc]
005fc680: ldrb     sl, [sp, #0xc7]
005fc684: ldrb     ip, [sp, #0xcb]
005fc688: ldr      r0, [sp, #0xc0]
005fc68c: add      r1, sp, #0x10c
005fc690: str      r4, [sp, #0xa8]
005fc694: str      r2, [sp, #0x58]
005fc698: str      r5, [sp, #0x50]
005fc69c: str      r7, [sp, #0x4c]
005fc6a0: str      r8, [sp, #0x48]
005fc6a4: str      sl, [sp, #0x44]
005fc6a8: str      ip, [sp, #0x40]
005fc6ac: str      r0, [sp, #0x38]
005fc6b0: str      r3, [sp, #0xa0]
005fc6b4: str      r1, [sp, #0x9c]
005fc6b8: mov      r4, r3
005fc6bc: ldr      r3, [sp, #0xa8]
005fc6c0: cmp      r3, #0
005fc6c4: ldrne    sb, [sp, #0xa8]
005fc6c8: beq      #0x5fc8e8
005fc6cc: ldrb     r3, [r4]
005fc6d0: ldr      r5, [sp, #0x98]
005fc6d4: ldr      r8, [sp, #0x94]
005fc6d8: strb     r3, [sp, #0x12c]
005fc6dc: ldrb     r3, [r4, #1]!
005fc6e0: ldr      sl, [sp, #0x80]
005fc6e4: ldr      lr, [sp, #0x7c]
005fc6e8: strb     r3, [sp, #0x12d]
005fc6ec: ldrb     r3, [r4, #1]
005fc6f0: ldr      r1, [sp, #0x9c]
005fc6f4: mov      r0, r6
005fc6f8: strb     r3, [sp, #0x12e]
005fc6fc: ldr      r3, [sp, #0x12c]
005fc700: mov      r2, fp
005fc704: and      r7, r3, r5
005fc708: and      ip, r3, sl
005fc70c: ldr      r5, [sp, #0x68]
005fc710: lsr      r7, r7, r8
005fc714: ldr      r8, [sp, #0x90]
005fc718: lsr      ip, ip, lr
005fc71c: ldr      lr, [sp, #0x64]
005fc720: and      sl, r3, r5
005fc724: lsl      r7, r7, r8
005fc728: ldr      r8, [sp, #0x60]
005fc72c: lsr      sl, sl, lr
005fc730: ldr      r5, [sp, #0x78]
005fc734: lsl      sl, sl, r8
005fc738: lsl      ip, ip, r5
005fc73c: str      sl, [sp, #0x1c]
005fc740: ldr      sl, [sp, #0x8c]
005fc744: ldr      r5, [sp, #0x74]
005fc748: str      ip, [sp, #0x24]
005fc74c: and      r8, r3, sl
005fc750: ldr      ip, [sp, #0x88]
005fc754: ldr      sl, [sp, #0x70]
005fc758: and      lr, r3, r5
005fc75c: lsr      r8, r8, ip
005fc760: lsr      lr, lr, sl
005fc764: ldr      ip, [sp, #0x58]
005fc768: ldr      sl, [sp, #0x84]
005fc76c: and      r5, ip, r3
005fc770: orr      r7, r7, r8, lsl sl
005fc774: ldr      ip, [sp, #0x50]
005fc778: ldr      r8, [sp, #0x24]
005fc77c: ldr      sl, [sp, #0x6c]
005fc780: lsr      r5, r5, ip
005fc784: orr      ip, r8, lr, lsl sl
005fc788: ldr      lr, [sp, #0x1c]
005fc78c: ldr      r8, [sp, #0x4c]
005fc790: ldr      sl, [sp, #0x44]
005fc794: orr      r5, lr, r5, lsl r8
005fc798: ldr      lr, [sp, #0x38]
005fc79c: ldr      r8, [sp, #0x40]
005fc7a0: lsr      r3, r3, sl
005fc7a4: and      r3, lr, r3, lsl r8
005fc7a8: ldr      sl, [sp, #0x34]
005fc7ac: ldr      r8, [sp, #0x54]
005fc7b0: and      lr, r7, sl
005fc7b4: ldr      r7, [sp, #0x3c]
005fc7b8: ldr      sl, [sp, #0x48]
005fc7bc: and      ip, ip, r8
005fc7c0: orr      lr, r7, lr
005fc7c4: and      r5, r5, sl
005fc7c8: orr      ip, lr, ip
005fc7cc: orr      ip, ip, r5
005fc7d0: orr      r3, ip, r3
005fc7d4: strb     r3, [sp, #0x10c]
005fc7d8: ldrb     r3, [r6]
005fc7dc: ldr      ip, [sp, #0x98]
005fc7e0: ldr      lr, [sp, #0x94]
005fc7e4: strb     r3, [sp, #0x128]
005fc7e8: ldrb     r3, [r6, #1]
005fc7ec: ldr      r5, [sp, #0x80]
005fc7f0: ldr      r8, [sp, #0x7c]
005fc7f4: strb     r3, [sp, #0x129]
005fc7f8: ldrb     r3, [r6, #2]
005fc7fc: add      r6, r6, fp
005fc800: strb     r3, [sp, #0x12a]
005fc804: ldr      r3, [sp, #0x128]
005fc808: and      r7, r3, ip
005fc80c: lsr      r7, r7, lr
005fc810: and      ip, r3, r5
005fc814: ldr      lr, [sp, #0x68]
005fc818: ldr      r5, [sp, #0x90]
005fc81c: lsr      ip, ip, r8
005fc820: ldr      r8, [sp, #0x64]
005fc824: and      sl, r3, lr
005fc828: lsl      r7, r7, r5
005fc82c: ldr      r5, [sp, #0x60]
005fc830: lsr      sl, sl, r8
005fc834: ldr      lr, [sp, #0x78]
005fc838: lsl      sl, sl, r5
005fc83c: lsl      ip, ip, lr
005fc840: str      sl, [sp, #0x1c]
005fc844: ldr      sl, [sp, #0x8c]
005fc848: ldr      r5, [sp, #0x74]
005fc84c: str      ip, [sp, #0x24]
005fc850: and      r8, r3, sl
005fc854: ldr      ip, [sp, #0x88]
005fc858: ldr      sl, [sp, #0x70]
005fc85c: and      lr, r3, r5
005fc860: lsr      r8, r8, ip
005fc864: lsr      lr, lr, sl
005fc868: ldr      ip, [sp, #0x58]
005fc86c: ldr      sl, [sp, #0x84]
005fc870: and      r5, ip, r3
005fc874: orr      r7, r7, r8, lsl sl
005fc878: ldr      ip, [sp, #0x50]
005fc87c: ldr      r8, [sp, #0x24]
005fc880: ldr      sl, [sp, #0x6c]
005fc884: lsr      r5, r5, ip
005fc888: orr      ip, r8, lr, lsl sl
005fc88c: ldr      lr, [sp, #0x1c]
005fc890: ldr      r8, [sp, #0x4c]
005fc894: ldr      sl, [sp, #0x44]
005fc898: orr      r5, lr, r5, lsl r8
005fc89c: ldr      lr, [sp, #0x38]
005fc8a0: ldr      r8, [sp, #0x40]
005fc8a4: lsr      r3, r3, sl
005fc8a8: and      r3, lr, r3, lsl r8
005fc8ac: ldr      sl, [sp, #0x34]
005fc8b0: ldr      r8, [sp, #0x54]
005fc8b4: and      lr, r7, sl
005fc8b8: ldr      r7, [sp, #0x3c]
005fc8bc: ldr      sl, [sp, #0x48]
005fc8c0: and      ip, ip, r8
005fc8c4: orr      lr, r7, lr
005fc8c8: orr      ip, lr, ip
005fc8cc: and      r5, r5, sl
005fc8d0: orr      ip, ip, r5
005fc8d4: orr      r3, ip, r3
005fc8d8: strb     r3, [r4, #-1]
005fc8dc: bl       #0x30e868
005fc8e0: subs     sb, sb, #1
005fc8e4: bne      #0x5fc6cc
005fc8e8: ldr      ip, [sp, #0xa4]
005fc8ec: ldr      r0, [sp, #0x5c]
005fc8f0: ldr      r1, [sp, #0xa0]
005fc8f4: ldr      r2, [sp, #0xac]
005fc8f8: add      r6, ip, r0
005fc8fc: add      r4, r1, r2
005fc900: cmp      r4, r6
005fc904: blo      #0x5f9eb4
005fc908: str      r4, [sp, #0xa0]
005fc90c: str      r6, [sp, #0xa4]
005fc910: b        #0x5fc6bc
005fc914: ldr      r2, [sp, #0x38]
005fc918: cmp      r2, #0
005fc91c: bne      #0x5fd4cc
005fc920: ldr      r3, [sp, #0x164]
005fc924: cmp      r3, #0
005fc928: beq      #0x5f9eb4
005fc92c: mov      fp, sb
005fc930: str      r1, [sp, #0x34]
005fc934: mov      r6, r1
005fc938: mov      sl, r1
005fc93c: add      r8, sp, #0xf0
005fc940: mov      sb, r4
005fc944: cmp      sb, #0
005fc948: movne    r4, sb
005fc94c: beq      #0x5fc9a0
005fc950: mov      r1, sl
005fc954: mov      r0, r8
005fc958: mov      r2, r7
005fc95c: bl       #0x30e868
005fc960: ldrb     r3, [sp, #0x10c]
005fc964: subs     r4, r4, #1
005fc968: add      sl, sl, r7
005fc96c: lsl      r3, r3, #1
005fc970: ldrh     r3, [r8, r3]
005fc974: strh     r3, [r6]
005fc978: ldrb     r3, [sp, #0x10d]
005fc97c: lsl      r3, r3, #1
005fc980: ldrh     r3, [r8, r3]
005fc984: strh     r3, [r6, #2]
005fc988: ldrb     r3, [sp, #0x10e]
005fc98c: lsl      r3, r3, #1
005fc990: ldrh     r3, [r8, r3]
005fc994: strh     r3, [r6, #4]
005fc998: add      r6, r6, #6
005fc99c: bne      #0x5fc950
005fc9a0: ldr      r2, [sp, #0x164]
005fc9a4: subs     r2, r2, #1
005fc9a8: str      r2, [sp, #0x164]
005fc9ac: beq      #0x5f9eb4
005fc9b0: ldr      r3, [sp, #0x34]
005fc9b4: ldr      r4, [sp, #0x5c]
005fc9b8: add      r5, r5, fp
005fc9bc: mov      r6, r5
005fc9c0: add      r3, r3, r4
005fc9c4: str      r3, [sp, #0x34]
005fc9c8: mov      sl, r3
005fc9cc: b        #0x5fc944
005fc9d0: ldr      r2, [sp, #0x38]
005fc9d4: cmp      r2, #0
005fc9d8: bne      #0x5fd40c
005fc9dc: ldr      r3, [sp, #0x164]
005fc9e0: cmp      r3, #0
005fc9e4: beq      #0x5f9eb4
005fc9e8: mov      fp, r1
005fc9ec: str      r1, [sp, #0x34]
005fc9f0: mov      r6, r1
005fc9f4: add      r8, sp, #0xf0
005fc9f8: mov      sl, r4
005fc9fc: cmp      sl, #0
005fca00: movne    r4, sl
005fca04: beq      #0x5fca44
005fca08: mov      r1, r5
005fca0c: mov      r2, r7
005fca10: mov      r0, r8
005fca14: bl       #0x30e868
005fca18: ldrb     r1, [sp, #0x10c]
005fca1c: ldrb     r2, [sp, #0x10d]
005fca20: ldrb     r3, [sp, #0x10e]
005fca24: ldr      r1, [r8, r1, lsl #2]
005fca28: ldr      r2, [r8, r2, lsl #2]
005fca2c: ldr      r3, [r8, r3, lsl #2]
005fca30: subs     r4, r4, #1
005fca34: stm      r6, {r1, r2, r3}
005fca38: add      r5, r5, r7
005fca3c: add      r6, r6, #0xc
005fca40: bne      #0x5fca08
005fca44: ldr      r1, [sp, #0x164]
005fca48: subs     r1, r1, #1
005fca4c: str      r1, [sp, #0x164]
005fca50: beq      #0x5f9eb4
005fca54: ldr      r2, [sp, #0x34]
005fca58: ldr      r3, [sp, #0x5c]
005fca5c: add      fp, fp, sb
005fca60: mov      r6, fp
005fca64: add      r5, r2, r3
005fca68: str      r5, [sp, #0x34]
005fca6c: b        #0x5fc9fc
005fca70: ldr      r2, [sp, #0x38]
005fca74: cmp      r2, #0
005fca78: bne      #0x5fd360
005fca7c: ldr      r3, [sp, #0x164]
005fca80: cmp      r3, #0
005fca84: beq      #0x5f9eb4
005fca88: mov      fp, sb
005fca8c: str      r1, [sp, #0x34]
005fca90: mov      r6, r1
005fca94: mov      sl, r1
005fca98: add      r8, sp, #0xf0
005fca9c: mov      sb, r4
005fcaa0: cmp      sb, #0
005fcaa4: movne    r4, sb
005fcaa8: beq      #0x5fcae4
005fcaac: mov      r1, sl
005fcab0: mov      r0, r8
005fcab4: mov      r2, r7
005fcab8: bl       #0x30e868
005fcabc: ldrb     r3, [sp, #0x10c]
005fcac0: subs     r4, r4, #1
005fcac4: add      sl, sl, r7
005fcac8: ldr      r3, [r8, r3, lsl #2]
005fcacc: str      r3, [r6]
005fcad0: ldrb     r3, [sp, #0x10d]
005fcad4: ldr      r3, [r8, r3, lsl #2]
005fcad8: str      r3, [r6, #4]
005fcadc: add      r6, r6, #8
005fcae0: bne      #0x5fcaac
005fcae4: ldr      r1, [sp, #0x164]
005fcae8: subs     r1, r1, #1
005fcaec: str      r1, [sp, #0x164]
005fcaf0: beq      #0x5f9eb4
005fcaf4: ldr      r2, [sp, #0x34]
005fcaf8: ldr      r3, [sp, #0x5c]
005fcafc: add      r5, r5, fp
005fcb00: mov      r6, r5
005fcb04: add      r2, r2, r3
005fcb08: str      r2, [sp, #0x34]
005fcb0c: mov      sl, r2
005fcb10: b        #0x5fcaa0
005fcb14: ldr      r2, [sp, #0x38]
005fcb18: cmp      r2, #0
005fcb1c: bne      #0x5fd268
005fcb20: ldr      r3, [sp, #0x164]
005fcb24: cmp      r3, #0
005fcb28: beq      #0x5f9eb4
005fcb2c: mov      fp, sb
005fcb30: str      r1, [sp, #0x34]
005fcb34: mov      r6, r1
005fcb38: mov      sl, r1
005fcb3c: add      r8, sp, #0xf0
005fcb40: mov      sb, r4
005fcb44: cmp      sb, #0
005fcb48: movne    r4, sb
005fcb4c: beq      #0x5fcbb0
005fcb50: mov      r1, sl
005fcb54: mov      r2, r7
005fcb58: mov      r0, r8
005fcb5c: bl       #0x30e868
005fcb60: ldrb     r0, [sp, #0x10c]
005fcb64: ldrb     r1, [sp, #0x10d]
005fcb68: ldrb     r2, [sp, #0x10e]
005fcb6c: lsl      r0, r0, #1
005fcb70: ldrh     r0, [r8, r0]
005fcb74: ldrb     r3, [sp, #0x10f]
005fcb78: lsl      r1, r1, #1
005fcb7c: strh     r0, [r6]
005fcb80: ldrh     r1, [r8, r1]
005fcb84: lsl      r2, r2, #1
005fcb88: lsl      r3, r3, #1
005fcb8c: strh     r1, [r6, #2]
005fcb90: ldrh     r2, [r8, r2]
005fcb94: subs     r4, r4, #1
005fcb98: add      sl, sl, r7
005fcb9c: strh     r2, [r6, #4]
005fcba0: ldrh     r3, [r8, r3]
005fcba4: strh     r3, [r6, #6]
005fcba8: add      r6, r6, #8
005fcbac: bne      #0x5fcb50
005fcbb0: ldr      r2, [sp, #0x164]
005fcbb4: subs     r2, r2, #1
005fcbb8: str      r2, [sp, #0x164]
005fcbbc: beq      #0x5f9eb4
005fcbc0: ldr      r4, [sp, #0x34]
005fcbc4: ldr      r3, [sp, #0x5c]
005fcbc8: add      r4, r4, fp
005fcbcc: add      r5, r5, r3
005fcbd0: str      r4, [sp, #0x34]
005fcbd4: mov      r6, r4
005fcbd8: mov      sl, r5
005fcbdc: b        #0x5fcb44
005fcbe0: ldr      r2, [sp, #0x38]
005fcbe4: cmp      r2, #0
005fcbe8: bne      #0x5fd1a8
005fcbec: ldr      r3, [sp, #0x164]
005fcbf0: cmp      r3, #0
005fcbf4: beq      #0x5f9eb4
005fcbf8: mov      fp, r1
005fcbfc: str      r1, [sp, #0x34]
005fcc00: mov      r6, r1
005fcc04: add      r8, sp, #0x10c
005fcc08: mov      sl, r4
005fcc0c: cmp      sl, #0
005fcc10: movne    r4, sl
005fcc14: beq      #0x5fcc5c
005fcc18: mov      r1, r5
005fcc1c: mov      r2, r7
005fcc20: mov      r0, r8
005fcc24: bl       #0x30e868
005fcc28: ldrb     r1, [sp, #0x12c]
005fcc2c: ldrb     r2, [sp, #0x12d]
005fcc30: ldrb     r3, [sp, #0x12e]
005fcc34: ldrb     r1, [r8, r1]
005fcc38: ldrb     r2, [r8, r2]
005fcc3c: ldrb     r3, [r8, r3]
005fcc40: subs     r4, r4, #1
005fcc44: strb     r1, [r6]
005fcc48: strb     r2, [r6, #1]
005fcc4c: strb     r3, [r6, #2]
005fcc50: add      r5, r5, r7
005fcc54: add      r6, r6, #3
005fcc58: bne      #0x5fcc18
005fcc5c: ldr      r1, [sp, #0x164]
005fcc60: subs     r1, r1, #1
005fcc64: str      r1, [sp, #0x164]
005fcc68: beq      #0x5f9eb4
005fcc6c: ldr      r2, [sp, #0x34]
005fcc70: ldr      r3, [sp, #0x5c]
005fcc74: add      fp, fp, sb
005fcc78: mov      r6, fp
005fcc7c: add      r5, r2, r3
005fcc80: str      r5, [sp, #0x34]
005fcc84: b        #0x5fcc0c
005fcc88: ldr      r2, [sp, #0x38]
005fcc8c: cmp      r2, #0
005fcc90: bne      #0x5fd0cc
005fcc94: ldr      r3, [sp, #0x164]
005fcc98: cmp      r3, #0
005fcc9c: beq      #0x5f9eb4
005fcca0: mov      fp, r1
005fcca4: str      r1, [sp, #0x34]
005fcca8: mov      r8, r1
005fccac: add      r6, sp, #0x10c
005fccb0: mov      sl, r4
005fccb4: cmp      sl, #0
005fccb8: movne    r4, sl
005fccbc: beq      #0x5fcd10
005fccc0: mov      r1, r8
005fccc4: mov      r0, r6
005fccc8: mov      r2, r7
005fcccc: bl       #0x30e868
005fccd0: ldrb     r3, [sp, #0x12c]
005fccd4: subs     r4, r4, #1
005fccd8: add      r8, r8, r7
005fccdc: ldrb     r3, [r6, r3]
005fcce0: strb     r3, [r5]
005fcce4: ldrb     r3, [sp, #0x12d]
005fcce8: ldrb     r3, [r6, r3]
005fccec: strb     r3, [r5, #1]
005fccf0: ldrb     r3, [sp, #0x12e]
005fccf4: ldrb     r3, [r6, r3]
005fccf8: strb     r3, [r5, #2]
005fccfc: ldrb     r3, [sp, #0x12f]
005fcd00: ldrb     r3, [r6, r3]
005fcd04: strb     r3, [r5, #3]
005fcd08: add      r5, r5, #4
005fcd0c: bne      #0x5fccc0
005fcd10: ldr      r2, [sp, #0x164]
005fcd14: subs     r2, r2, #1
005fcd18: str      r2, [sp, #0x164]
005fcd1c: beq      #0x5f9eb4
005fcd20: ldr      r3, [sp, #0x34]
005fcd24: ldr      r4, [sp, #0x5c]
005fcd28: add      fp, fp, sb
005fcd2c: mov      r5, fp
005fcd30: add      r8, r3, r4
005fcd34: str      r8, [sp, #0x34]
005fcd38: b        #0x5fccb4
005fcd3c: ldr      r2, [sp, #0x38]
005fcd40: cmp      r2, #0
005fcd44: bne      #0x5fcde4
005fcd48: ldr      r3, [sp, #0x164]
005fcd4c: cmp      r3, #0
005fcd50: beq      #0x5f9eb4
005fcd54: mov      fp, r1
005fcd58: str      r1, [sp, #0x34]
005fcd5c: mov      r6, r1
005fcd60: add      r8, sp, #0xf0
005fcd64: mov      sl, r4
005fcd68: cmp      sl, #0
005fcd6c: movne    r4, sl
005fcd70: beq      #0x5fcdb8
005fcd74: mov      r1, r5
005fcd78: mov      r2, r7
005fcd7c: mov      r0, r8
005fcd80: bl       #0x30e868
005fcd84: ldrb     r0, [sp, #0x10c]
005fcd88: ldrb     r1, [sp, #0x10d]
005fcd8c: ldrb     r2, [sp, #0x10e]
005fcd90: ldrb     r3, [sp, #0x10f]
005fcd94: ldr      r0, [r8, r0, lsl #2]
005fcd98: ldr      r1, [r8, r1, lsl #2]
005fcd9c: ldr      r2, [r8, r2, lsl #2]
005fcda0: ldr      r3, [r8, r3, lsl #2]
005fcda4: subs     r4, r4, #1
005fcda8: stm      r6, {r0, r1, r2, r3}
005fcdac: add      r5, r5, r7
005fcdb0: add      r6, r6, #0x10
005fcdb4: bne      #0x5fcd74
005fcdb8: ldr      r2, [sp, #0x164]
005fcdbc: subs     r2, r2, #1
005fcdc0: str      r2, [sp, #0x164]
005fcdc4: beq      #0x5f9eb4
005fcdc8: ldr      r3, [sp, #0x34]
005fcdcc: ldr      r4, [sp, #0x5c]
005fcdd0: add      fp, fp, sb
005fcdd4: mov      r6, fp
005fcdd8: add      r5, r3, r4
005fcddc: str      r5, [sp, #0x34]
005fcde0: b        #0x5fcd68
005fcde4: ldr      r8, [sp, #0x164]
005fcde8: ldr      sl, [sp, #0x158]
005fcdec: sub      r6, r8, #1
005fcdf0: mla      r6, r6, sb, sl
005fcdf4: rsb      sb, sb, #0
005fcdf8: cmp      sl, r6
005fcdfc: str      sb, [sp, #0x38]
005fce00: movls    sl, r4
005fce04: addls    sb, sp, #0xf0
005fce08: bhi      #0x5f9eb4
005fce0c: cmp      sl, #0
005fce10: str      r6, [sp, #0x54]
005fce14: mov      fp, r5
005fce18: movne    r4, sl
005fce1c: beq      #0x5fce9c
005fce20: ldrb     r3, [sp, #0x10c]
005fce24: ldrb     r2, [sp, #0x10d]
005fce28: ldrb     lr, [sp, #0x10e]
005fce2c: ldr      r1, [r6, r3, lsl #2]
005fce30: ldrb     ip, [sp, #0x10f]
005fce34: mov      r0, r5
005fce38: str      r1, [sp, #0xf0]
005fce3c: ldr      r8, [r6, r2, lsl #2]
005fce40: mov      r1, sb
005fce44: mov      r2, r7
005fce48: str      r8, [sp, #0xf4]
005fce4c: ldr      lr, [r6, lr, lsl #2]
005fce50: str      lr, [sp, #0xf8]
005fce54: ldr      ip, [r6, ip, lsl #2]
005fce58: str      ip, [sp, #0xfc]
005fce5c: ldr      r3, [r5, r3, lsl #2]
005fce60: str      r3, [r6]
005fce64: ldrb     r3, [sp, #0x10d]
005fce68: ldr      r3, [r5, r3, lsl #2]
005fce6c: str      r3, [r6, #4]
005fce70: ldrb     r3, [sp, #0x10e]
005fce74: ldr      r3, [r5, r3, lsl #2]
005fce78: str      r3, [r6, #8]
005fce7c: ldrb     r3, [sp, #0x10f]
005fce80: ldr      r3, [r5, r3, lsl #2]
005fce84: add      r5, r5, r7
005fce88: str      r3, [r6, #0xc]
005fce8c: bl       #0x30e868
005fce90: subs     r4, r4, #1
005fce94: add      r6, r6, #0x10
005fce98: bne      #0x5fce20
005fce9c: ldr      ip, [sp, #0x5c]
005fcea0: ldr      r0, [sp, #0x54]
005fcea4: ldr      r1, [sp, #0x38]
005fcea8: add      r5, fp, ip
005fceac: add      r6, r0, r1
005fceb0: cmp      r6, r5
005fceb4: bhs      #0x5fce0c
005fceb8: mov      r0, #1
005fcebc: b        #0x5f964c
005fcec0: ldr      r7, [sp, #0x164]
005fcec4: sub      r5, r7, #1
005fcec8: mla      r5, r5, sb, r8
005fcecc: rsb      sb, sb, #0
005fced0: cmp      r8, r5
005fced4: str      sb, [sp, #0x70]
005fced8: bhi      #0x5f9eb4
005fcedc: ldrb     ip, [sp, #0xc5]
005fcee0: str      r8, [sp, #0x6c]
005fcee4: ldr      r8, [sp, #0xb4]
005fcee8: ldrb     r0, [sp, #0xc9]
005fceec: str      ip, [sp, #0x58]
005fcef0: str      r8, [sp, #0x60]
005fcef4: ldr      r1, [sp, #0xb8]
005fcef8: ldrb     r8, [sp, #0xc7]
005fcefc: ldrb     r2, [sp, #0xc6]
005fcf00: ldrb     r3, [sp, #0xca]
005fcf04: ldr      r7, [sp, #0xbc]
005fcf08: ldrb     ip, [sp, #0xcb]
005fcf0c: ldrb     sb, [sp, #0xc4]
005fcf10: ldrb     sl, [sp, #0xc8]
005fcf14: str      r0, [sp, #0x50]
005fcf18: add      r0, sp, #0x120
005fcf1c: str      r8, [sp, #0x34]
005fcf20: str      r1, [sp, #0x4c]
005fcf24: ldr      r8, [sp, #0x40]
005fcf28: str      r2, [sp, #0x48]
005fcf2c: str      r3, [sp, #0x44]
005fcf30: str      r7, [sp, #0x38]
005fcf34: str      ip, [sp, #0x54]
005fcf38: str      r5, [sp, #0x68]
005fcf3c: str      r0, [sp, #0x64]
005fcf40: str      r4, [sp, #0x40]
005fcf44: ldr      r4, [sp, #0x40]
005fcf48: cmp      r4, #0
005fcf4c: ldrne    r7, [sp, #0x40]
005fcf50: beq      #0x5fd0a0
005fcf54: ldrb     r3, [r5]
005fcf58: ldr      ip, [sp, #0x60]
005fcf5c: ldr      r4, [sp, #0x58]
005fcf60: strb     r3, [sp, #0x11c]
005fcf64: ldrb     r3, [r5, #1]!
005fcf68: ldr      r1, [sp, #0x64]
005fcf6c: mov      r0, r6
005fcf70: strb     r3, [sp, #0x11d]
005fcf74: ldrb     r3, [r5, #1]
005fcf78: mov      r2, fp
005fcf7c: strb     r3, [sp, #0x11e]
005fcf80: ldr      r3, [sp, #0x11c]
005fcf84: lsr      lr, r3, sb
005fcf88: and      lr, ip, lr, lsl sl
005fcf8c: str      lr, [sp, #0x74]
005fcf90: ldr      lr, [sp, #0x48]
005fcf94: ldr      ip, [sp, #0x4c]
005fcf98: lsr      r4, r3, r4
005fcf9c: lsr      lr, r3, lr
005fcfa0: str      lr, [sp, #0x7c]
005fcfa4: ldr      lr, [sp, #0x50]
005fcfa8: and      r4, ip, r4, lsl lr
005fcfac: str      r4, [sp, #0x78]
005fcfb0: ldr      r4, [sp, #0x34]
005fcfb4: ldr      lr, [sp, #0x7c]
005fcfb8: ldr      ip, [sp, #0x38]
005fcfbc: lsr      r3, r3, r4
005fcfc0: ldr      r4, [sp, #0x44]
005fcfc4: and      ip, ip, lr, lsl r4
005fcfc8: str      ip, [sp, #0x80]
005fcfcc: ldr      ip, [sp, #0x54]
005fcfd0: and      r4, r8, r3, lsl ip
005fcfd4: ldr      r3, [sp, #0x3c]
005fcfd8: ldr      ip, [sp, #0x74]
005fcfdc: orr      lr, r3, ip
005fcfe0: ldr      r3, [sp, #0x78]
005fcfe4: ldr      ip, [sp, #0x80]
005fcfe8: orr      lr, lr, r3
005fcfec: orr      lr, lr, ip
005fcff0: orr      r3, lr, r4
005fcff4: strb     r3, [sp, #0x120]
005fcff8: ldrb     r3, [r6]
005fcffc: ldr      ip, [sp, #0x60]
005fd000: ldr      r4, [sp, #0x58]
005fd004: strb     r3, [sp, #0x118]
005fd008: ldrb     r3, [r6, #1]
005fd00c: strb     r3, [sp, #0x119]
005fd010: ldrb     r3, [r6, #2]
005fd014: add      r6, fp, r6
005fd018: strb     r3, [sp, #0x11a]
005fd01c: ldr      r3, [sp, #0x118]
005fd020: lsr      lr, r3, sb
005fd024: and      lr, ip, lr, lsl sl
005fd028: str      lr, [sp, #0x74]
005fd02c: ldr      lr, [sp, #0x48]
005fd030: ldr      ip, [sp, #0x4c]
005fd034: lsr      r4, r3, r4
005fd038: lsr      lr, r3, lr
005fd03c: str      lr, [sp, #0x7c]
005fd040: ldr      lr, [sp, #0x50]
005fd044: and      r4, ip, r4, lsl lr
005fd048: str      r4, [sp, #0x78]
005fd04c: ldr      r4, [sp, #0x34]
005fd050: ldr      lr, [sp, #0x7c]
005fd054: ldr      ip, [sp, #0x38]
005fd058: lsr      r3, r3, r4
005fd05c: ldr      r4, [sp, #0x44]
005fd060: and      ip, ip, lr, lsl r4
005fd064: str      ip, [sp, #0x80]
005fd068: ldr      ip, [sp, #0x54]
005fd06c: and      r4, r8, r3, lsl ip
005fd070: ldr      r3, [sp, #0x3c]
005fd074: ldr      ip, [sp, #0x74]
005fd078: orr      lr, r3, ip
005fd07c: ldr      r3, [sp, #0x78]
005fd080: ldr      ip, [sp, #0x80]
005fd084: orr      lr, lr, r3
005fd088: orr      lr, lr, ip
005fd08c: orr      r3, lr, r4
005fd090: strb     r3, [r5, #-1]
005fd094: bl       #0x30e868
005fd098: subs     r7, r7, #1
005fd09c: bne      #0x5fcf54
005fd0a0: ldr      r0, [sp, #0x6c]
005fd0a4: ldr      r1, [sp, #0x5c]
005fd0a8: ldr      r2, [sp, #0x68]
005fd0ac: ldr      r3, [sp, #0x70]
005fd0b0: add      r6, r0, r1
005fd0b4: add      r5, r2, r3
005fd0b8: cmp      r5, r6
005fd0bc: blo      #0x5f9eb4
005fd0c0: str      r5, [sp, #0x68]
005fd0c4: str      r6, [sp, #0x6c]
005fd0c8: b        #0x5fcf44
005fd0cc: ldr      r8, [sp, #0x164]
005fd0d0: ldr      sl, [sp, #0x158]
005fd0d4: sub      r6, r8, #1
005fd0d8: mla      r6, r6, sb, sl
005fd0dc: rsb      sb, sb, #0
005fd0e0: cmp      sl, r6
005fd0e4: str      sb, [sp, #0x38]
005fd0e8: movls    sl, r4
005fd0ec: addls    sb, sp, #0x10c
005fd0f0: bhi      #0x5f9eb4
005fd0f4: cmp      sl, #0
005fd0f8: str      r6, [sp, #0x54]
005fd0fc: mov      fp, r5
005fd100: movne    r4, sl
005fd104: beq      #0x5fd184
005fd108: ldrb     r3, [sp, #0x12c]
005fd10c: ldrb     r2, [sp, #0x12d]
005fd110: ldrb     lr, [sp, #0x12e]
005fd114: ldrb     r1, [r6, r3]
005fd118: ldrb     ip, [sp, #0x12f]
005fd11c: mov      r0, r5
005fd120: strb     r1, [sp, #0x10c]
005fd124: ldrb     r8, [r6, r2]
005fd128: mov      r1, sb
005fd12c: mov      r2, r7
005fd130: strb     r8, [sp, #0x10d]
005fd134: ldrb     lr, [r6, lr]
005fd138: strb     lr, [sp, #0x10e]
005fd13c: ldrb     ip, [r6, ip]
005fd140: strb     ip, [sp, #0x10f]
005fd144: ldrb     r3, [r5, r3]
005fd148: strb     r3, [r6]
005fd14c: ldrb     r3, [sp, #0x12d]
005fd150: ldrb     r3, [r5, r3]
005fd154: strb     r3, [r6, #1]
005fd158: ldrb     r3, [sp, #0x12e]
005fd15c: ldrb     r3, [r5, r3]
005fd160: strb     r3, [r6, #2]
005fd164: ldrb     r3, [sp, #0x12f]
005fd168: ldrb     r3, [r5, r3]
005fd16c: add      r5, r5, r7
005fd170: strb     r3, [r6, #3]
005fd174: bl       #0x30e868
005fd178: subs     r4, r4, #1
005fd17c: add      r6, r6, #4
005fd180: bne      #0x5fd108
005fd184: ldr      ip, [sp, #0x5c]
005fd188: ldr      r0, [sp, #0x54]
005fd18c: ldr      r1, [sp, #0x38]
005fd190: add      r5, fp, ip
005fd194: add      r6, r0, r1
005fd198: cmp      r5, r6
005fd19c: bls      #0x5fd0f4
005fd1a0: mov      r0, #1
005fd1a4: b        #0x5f964c
005fd1a8: ldr      r8, [sp, #0x164]
005fd1ac: ldr      sl, [sp, #0x158]
005fd1b0: sub      r6, r8, #1
005fd1b4: mla      r6, r6, sb, sl
005fd1b8: rsb      sb, sb, #0
005fd1bc: cmp      sl, r6
005fd1c0: str      sb, [sp, #0x54]
005fd1c4: addls    sl, sp, #0x10c
005fd1c8: movls    r8, r4
005fd1cc: bhi      #0x5f9eb4
005fd1d0: cmp      r8, #0
005fd1d4: mov      fp, r6
005fd1d8: mov      sb, r5
005fd1dc: movne    r4, r8
005fd1e0: beq      #0x5fd248
005fd1e4: ldrb     r3, [sp, #0x12c]
005fd1e8: ldrb     r2, [sp, #0x12d]
005fd1ec: ldrb     ip, [sp, #0x12e]
005fd1f0: ldrb     lr, [r6, r3]
005fd1f4: mov      r0, r5
005fd1f8: mov      r1, sl
005fd1fc: strb     lr, [sp, #0x10c]
005fd200: ldrb     lr, [r6, r2]
005fd204: mov      r2, r7
005fd208: strb     lr, [sp, #0x10d]
005fd20c: ldrb     ip, [r6, ip]
005fd210: strb     ip, [sp, #0x10e]
005fd214: ldrb     r3, [r5, r3]
005fd218: strb     r3, [r6]
005fd21c: ldrb     r3, [sp, #0x12d]
005fd220: ldrb     r3, [r5, r3]
005fd224: strb     r3, [r6, #1]
005fd228: ldrb     r3, [sp, #0x12e]
005fd22c: ldrb     r3, [r5, r3]
005fd230: add      r5, r5, r7
005fd234: strb     r3, [r6, #2]
005fd238: bl       #0x30e868
005fd23c: subs     r4, r4, #1
005fd240: add      r6, r6, #3
005fd244: bne      #0x5fd1e4
005fd248: ldr      ip, [sp, #0x5c]
005fd24c: ldr      r0, [sp, #0x54]
005fd250: add      r5, sb, ip
005fd254: add      r6, fp, r0
005fd258: cmp      r5, r6
005fd25c: bls      #0x5fd1d0
005fd260: mov      r0, #1
005fd264: b        #0x5f964c
005fd268: ldr      r6, [sp, #0x164]
005fd26c: ldr      r8, [sp, #0x158]
005fd270: sub      fp, r6, #1
005fd274: mla      fp, fp, sb, r8
005fd278: rsb      sb, sb, #0
005fd27c: cmp      r8, fp
005fd280: str      sb, [sp, #0x54]
005fd284: movls    r8, r5
005fd288: addls    sb, sp, #0xf0
005fd28c: movls    sl, r4
005fd290: bhi      #0x5f9eb4
005fd294: cmp      sl, #0
005fd298: mov      r4, fp
005fd29c: mov      r5, r8
005fd2a0: movne    r6, sl
005fd2a4: beq      #0x5fd340
005fd2a8: ldrb     r3, [sp, #0x10c]
005fd2ac: ldrb     r0, [sp, #0x10d]
005fd2b0: ldrb     r1, [sp, #0x10e]
005fd2b4: lsl      r3, r3, #1
005fd2b8: ldrh     ip, [r4, r3]
005fd2bc: lsl      r0, r0, #1
005fd2c0: lsl      r1, r1, #1
005fd2c4: strh     ip, [sp, #0xf0]
005fd2c8: ldrh     r0, [r4, r0]
005fd2cc: ldrb     r2, [sp, #0x10f]
005fd2d0: strh     r0, [sp, #0xf2]
005fd2d4: ldrh     r1, [r4, r1]
005fd2d8: lsl      r2, r2, #1
005fd2dc: mov      r0, r5
005fd2e0: strh     r1, [sp, #0xf4]
005fd2e4: ldrh     r2, [r4, r2]
005fd2e8: mov      r1, sb
005fd2ec: strh     r2, [sp, #0xf6]
005fd2f0: ldrh     r3, [r5, r3]
005fd2f4: mov      r2, r7
005fd2f8: strh     r3, [r4]
005fd2fc: ldrb     r3, [sp, #0x10d]
005fd300: lsl      r3, r3, #1
005fd304: ldrh     r3, [r5, r3]
005fd308: strh     r3, [r4, #2]
005fd30c: ldrb     r3, [sp, #0x10e]
005fd310: lsl      r3, r3, #1
005fd314: ldrh     r3, [r5, r3]
005fd318: strh     r3, [r4, #4]
005fd31c: ldrb     r3, [sp, #0x10f]
005fd320: lsl      r3, r3, #1
005fd324: ldrh     r3, [r5, r3]
005fd328: add      r5, r5, r7
005fd32c: strh     r3, [r4, #6]
005fd330: bl       #0x30e868
005fd334: subs     r6, r6, #1
005fd338: add      r4, r4, #8
005fd33c: bne      #0x5fd2a8
005fd340: ldr      r0, [sp, #0x5c]
005fd344: ldr      r1, [sp, #0x54]
005fd348: add      r8, r8, r0
005fd34c: add      fp, fp, r1
005fd350: cmp      r8, fp
005fd354: bls      #0x5fd294
005fd358: mov      r0, #1
005fd35c: b        #0x5f964c
005fd360: ldr      r6, [sp, #0x164]
005fd364: ldr      r8, [sp, #0x158]
005fd368: sub      fp, r6, #1
005fd36c: mla      fp, fp, sb, r8
005fd370: rsb      sb, sb, #0
005fd374: cmp      r8, fp
005fd378: str      sb, [sp, #0x54]
005fd37c: movls    r8, r5
005fd380: addls    sb, sp, #0xf0
005fd384: movls    sl, r4
005fd388: bhi      #0x5f9eb4
005fd38c: cmp      sl, #0
005fd390: mov      r4, fp
005fd394: mov      r5, r8
005fd398: movne    r6, sl
005fd39c: beq      #0x5fd3ec
005fd3a0: ldrb     r3, [sp, #0x10c]
005fd3a4: ldrb     ip, [sp, #0x10d]
005fd3a8: mov      r0, r5
005fd3ac: ldr      lr, [r4, r3, lsl #2]
005fd3b0: mov      r1, sb
005fd3b4: mov      r2, r7
005fd3b8: str      lr, [sp, #0xf0]
005fd3bc: ldr      ip, [r4, ip, lsl #2]
005fd3c0: str      ip, [sp, #0xf4]
005fd3c4: ldr      r3, [r5, r3, lsl #2]
005fd3c8: str      r3, [r4]
005fd3cc: ldrb     r3, [sp, #0x10d]
005fd3d0: ldr      r3, [r5, r3, lsl #2]
005fd3d4: add      r5, r5, r7
005fd3d8: str      r3, [r4, #4]
005fd3dc: bl       #0x30e868
005fd3e0: subs     r6, r6, #1
005fd3e4: add      r4, r4, #8
005fd3e8: bne      #0x5fd3a0
005fd3ec: ldr      ip, [sp, #0x5c]
005fd3f0: ldr      r0, [sp, #0x54]
005fd3f4: add      r8, r8, ip
005fd3f8: add      fp, fp, r0
005fd3fc: cmp      r8, fp
005fd400: bls      #0x5fd38c
005fd404: mov      r0, #1
005fd408: b        #0x5f964c
005fd40c: ldr      r8, [sp, #0x164]
005fd410: ldr      sl, [sp, #0x158]
005fd414: sub      r6, r8, #1
005fd418: mla      r6, r6, sb, sl
005fd41c: rsb      sb, sb, #0
005fd420: cmp      sl, r6
005fd424: str      sb, [sp, #0x54]
005fd428: addls    sl, sp, #0xf0
005fd42c: movls    r8, r4
005fd430: bhi      #0x5f9eb4
005fd434: cmp      r8, #0
005fd438: mov      fp, r6
005fd43c: mov      sb, r5
005fd440: movne    r4, r8
005fd444: beq      #0x5fd4ac
005fd448: ldrb     r3, [sp, #0x10c]
005fd44c: ldrb     r2, [sp, #0x10d]
005fd450: ldrb     ip, [sp, #0x10e]
005fd454: ldr      lr, [r6, r3, lsl #2]
005fd458: mov      r0, r5
005fd45c: mov      r1, sl
005fd460: str      lr, [sp, #0xf0]
005fd464: ldr      lr, [r6, r2, lsl #2]
005fd468: mov      r2, r7
005fd46c: str      lr, [sp, #0xf4]
005fd470: ldr      ip, [r6, ip, lsl #2]
005fd474: str      ip, [sp, #0xf8]
005fd478: ldr      r3, [r5, r3, lsl #2]
005fd47c: str      r3, [r6]
005fd480: ldrb     r3, [sp, #0x10d]
005fd484: ldr      r3, [r5, r3, lsl #2]
005fd488: str      r3, [r6, #4]
005fd48c: ldrb     r3, [sp, #0x10e]
005fd490: ldr      r3, [r5, r3, lsl #2]
005fd494: add      r5, r5, r7
005fd498: str      r3, [r6, #8]
005fd49c: bl       #0x30e868
005fd4a0: subs     r4, r4, #1
005fd4a4: add      r6, r6, #0xc
005fd4a8: bne      #0x5fd448
005fd4ac: ldr      ip, [sp, #0x5c]
005fd4b0: ldr      r0, [sp, #0x54]
005fd4b4: add      r5, sb, ip
005fd4b8: add      r6, fp, r0
005fd4bc: cmp      r6, r5
005fd4c0: bhs      #0x5fd434
005fd4c4: mov      r0, #1
005fd4c8: b        #0x5f964c
005fd4cc: ldr      r8, [sp, #0x164]
005fd4d0: ldr      sl, [sp, #0x158]
005fd4d4: sub      r6, r8, #1
005fd4d8: mla      r6, r6, sb, sl
005fd4dc: cmp      sl, r6
005fd4e0: bhi      #0x5f9eb4
005fd4e4: rsb      sb, sb, #0
005fd4e8: str      sb, [sp, #0x54]
005fd4ec: mov      fp, r6
005fd4f0: mov      sb, sl
005fd4f4: add      sl, sp, #0xf0
005fd4f8: cmp      r4, #0
005fd4fc: movne    r8, r4
005fd500: beq      #0x5fd57c
005fd504: ldrb     r3, [sp, #0x10c]
005fd508: ldrb     r1, [sp, #0x10d]
005fd50c: ldrb     r2, [sp, #0x10e]
005fd510: lsl      r3, r3, #1
005fd514: ldrh     ip, [r6, r3]
005fd518: lsl      r1, r1, #1
005fd51c: lsl      r2, r2, #1
005fd520: strh     ip, [sp, #0xf0]
005fd524: ldrh     r1, [r6, r1]
005fd528: mov      r0, r5
005fd52c: strh     r1, [sp, #0xf2]
005fd530: ldrh     r2, [r6, r2]
005fd534: mov      r1, sl
005fd538: strh     r2, [sp, #0xf4]
005fd53c: ldrh     r3, [r5, r3]
005fd540: mov      r2, r7
005fd544: strh     r3, [r6]
005fd548: ldrb     r3, [sp, #0x10d]
005fd54c: lsl      r3, r3, #1
005fd550: ldrh     r3, [r5, r3]
005fd554: strh     r3, [r6, #2]
005fd558: ldrb     r3, [sp, #0x10e]
005fd55c: lsl      r3, r3, #1
005fd560: ldrh     r3, [r5, r3]
005fd564: add      r5, r5, r7
005fd568: strh     r3, [r6, #4]
005fd56c: bl       #0x30e868
005fd570: subs     r8, r8, #1
005fd574: add      r6, r6, #6
005fd578: bne      #0x5fd504
005fd57c: ldr      r0, [sp, #0x5c]
005fd580: ldr      r1, [sp, #0x54]
005fd584: add      r5, sb, r0
005fd588: add      r6, fp, r1
005fd58c: cmp      r5, r6
005fd590: bhi      #0x5f9eb4
005fd594: mov      fp, r6
005fd598: mov      sb, r5
005fd59c: b        #0x5fd4f8
005fd5a0: ldr      r1, [sp, #0x164]
005fd5a4: cmp      r1, #0
005fd5a8: beq      #0x5f9eb4
005fd5ac: ldr      r2, [sp, #0xd0]
005fd5b0: ldrb     r3, [sp, #0xc4]
005fd5b4: ldrb     r5, [sp, #0xc8]
005fd5b8: ldr      r7, [sp, #0xdc]
005fd5bc: ldrb     sl, [sp, #0xeb]
005fd5c0: ldr      ip, [sp, #0xb4]
005fd5c4: ldr      r0, [sp, #0xd4]
005fd5c8: str      r8, [sp, #0xa0]
005fd5cc: ldrb     r8, [sp, #0xe8]
005fd5d0: ldrb     r1, [sp, #0xc5]
005fd5d4: str      r2, [sp, #0x38]
005fd5d8: str      r3, [sp, #0x9c]
005fd5dc: ldrb     r2, [sp, #0xc9]
005fd5e0: ldr      r3, [sp, #0xe0]
005fd5e4: str      r5, [sp, #0x98]
005fd5e8: str      r7, [sp, #0x94]
005fd5ec: ldrb     r5, [sp, #0xe9]
005fd5f0: ldrb     r7, [sp, #0xec]
005fd5f4: str      r8, [sp, #0x90]
005fd5f8: str      sl, [sp, #0x8c]
005fd5fc: ldr      r8, [sp, #0xb8]
005fd600: ldr      sl, [sp, #0xd8]
005fd604: str      ip, [sp, #0x40]
005fd608: str      r0, [sp, #0x88]
005fd60c: ldrb     ip, [sp, #0xc6]
005fd610: ldrb     r0, [sp, #0xca]
005fd614: str      r1, [sp, #0x84]
005fd618: str      r2, [sp, #0x80]
005fd61c: str      r3, [sp, #0x7c]
005fd620: str      r5, [sp, #0x78]
005fd624: str      r7, [sp, #0x74]
005fd628: str      r8, [sp, #0x70]
005fd62c: str      sl, [sp, #0x34]
005fd630: str      ip, [sp, #0x6c]
005fd634: str      r0, [sp, #0x68]
005fd638: ldrb     r3, [sp, #0xed]
005fd63c: ldr      sl, [sp, #0xc0]
005fd640: ldr      ip, [sp, #0xa0]
005fd644: ldr      r1, [sp, #0xe4]
005fd648: ldrb     r2, [sp, #0xea]
005fd64c: ldr      r5, [sp, #0xbc]
005fd650: ldrb     r7, [sp, #0xc7]
005fd654: ldrb     r8, [sp, #0xcb]
005fd658: str      r3, [sp, #0x58]
005fd65c: add      r3, sp, #0x10c
005fd660: str      sl, [sp, #0x44]
005fd664: str      sb, [sp, #0xa8]
005fd668: str      r1, [sp, #0x64]
005fd66c: str      r2, [sp, #0x60]
005fd670: str      r5, [sp, #0x50]
005fd674: str      r7, [sp, #0x4c]
005fd678: str      r8, [sp, #0x48]
005fd67c: str      ip, [sp, #0xa4]
005fd680: mov      sl, ip
005fd684: mov      sb, r3
005fd688: cmp      r4, #0
005fd68c: movne    r5, #0
005fd690: strne    sl, [sp, #0x54]
005fd694: beq      #0x5fd7ac
005fd698: mov      r1, r6
005fd69c: mov      r2, fp
005fd6a0: mov      r0, sb
005fd6a4: bl       #0x30e868
005fd6a8: ldrb     r1, [sp, #0x10c]
005fd6ac: ldrb     r2, [sp, #0x10d]
005fd6b0: ldrb     r3, [sp, #0x10e]
005fd6b4: strb     r1, [sp, #0x124]
005fd6b8: strb     r2, [sp, #0x125]
005fd6bc: strb     r3, [sp, #0x126]
005fd6c0: ldr      r3, [sp, #0x124]
005fd6c4: ldr      r8, [sp, #0x38]
005fd6c8: ldr      sl, [sp, #0x9c]
005fd6cc: ldr      ip, [sp, #0x88]
005fd6d0: and      r7, r8, r3
005fd6d4: ldr      r0, [sp, #0x84]
005fd6d8: lsr      r7, r7, sl
005fd6dc: ldr      sl, [sp, #0x98]
005fd6e0: and      r1, r3, ip
005fd6e4: lsr      r1, r1, r0
005fd6e8: lsl      r7, r7, sl
005fd6ec: ldr      r0, [sp, #0x80]
005fd6f0: ldr      r8, [sp, #0x34]
005fd6f4: ldr      ip, [sp, #0x6c]
005fd6f8: lsl      r1, r1, r0
005fd6fc: and      r2, r8, r3
005fd700: ldr      r8, [sp, #0x68]
005fd704: lsr      r2, r2, ip
005fd708: lsl      r2, r2, r8
005fd70c: ldr      sl, [sp, #0x94]
005fd710: ldr      ip, [sp, #0x90]
005fd714: add      r6, r6, fp
005fd718: and      r8, r3, sl
005fd71c: ldr      sl, [sp, #0x7c]
005fd720: lsr      r8, r8, ip
005fd724: and      r0, sl, r3
005fd728: ldr      ip, [sp, #0x78]
005fd72c: ldr      sl, [sp, #0x64]
005fd730: lsr      r0, r0, ip
005fd734: and      ip, sl, r3
005fd738: ldr      sl, [sp, #0x8c]
005fd73c: orr      r7, r7, r8, lsl sl
005fd740: ldr      sl, [sp, #0x74]
005fd744: ldr      r8, [sp, #0x60]
005fd748: orr      r1, r1, r0, lsl sl
005fd74c: ldr      r0, [sp, #0x58]
005fd750: lsr      ip, ip, r8
005fd754: ldr      r8, [sp, #0x4c]
005fd758: orr      r2, r2, ip, lsl r0
005fd75c: ldr      sl, [sp, #0x44]
005fd760: ldr      ip, [sp, #0x48]
005fd764: lsr      r3, r3, r8
005fd768: and      r3, sl, r3, lsl ip
005fd76c: ldr      r8, [sp, #0x40]
005fd770: ldr      sl, [sp, #0x3c]
005fd774: ldr      ip, [sp, #0x70]
005fd778: and      r0, r7, r8
005fd77c: orr      r0, sl, r0
005fd780: and      r1, r1, ip
005fd784: orr      r1, r0, r1
005fd788: ldr      r0, [sp, #0x50]
005fd78c: and      r2, r2, r0
005fd790: orr      r2, r1, r2
005fd794: ldr      r1, [sp, #0x54]
005fd798: orr      r3, r2, r3
005fd79c: strb     r3, [r1, r5]
005fd7a0: add      r5, r5, #1
005fd7a4: cmp      r4, r5
005fd7a8: bne      #0x5fd698
005fd7ac: ldr      r2, [sp, #0x164]
005fd7b0: subs     r2, r2, #1
005fd7b4: str      r2, [sp, #0x164]
005fd7b8: beq      #0x5f9eb4
005fd7bc: ldr      r3, [sp, #0xa4]
005fd7c0: ldr      r7, [sp, #0xa0]
005fd7c4: ldr      r5, [sp, #0x5c]
005fd7c8: ldr      r8, [sp, #0xa8]
005fd7cc: add      r6, r3, r5
005fd7d0: add      r7, r7, r8
005fd7d4: str      r7, [sp, #0xa0]
005fd7d8: mov      sl, r7
005fd7dc: str      r6, [sp, #0xa4]
005fd7e0: b        #0x5fd688
005fd7e4: ldr      r8, [sp, #0x164]
005fd7e8: ldr      sl, [sp, #0x158]
005fd7ec: sub      r6, r8, #1
005fd7f0: mla      r6, r6, sb, sl
005fd7f4: rsb      sb, sb, #0
005fd7f8: cmp      sl, r6
005fd7fc: str      sb, [sp, #0x54]
005fd800: addls    sl, sp, #0xf0
005fd804: bhi      #0x5f9eb4
005fd808: cmp      r4, #0
005fd80c: mov      fp, r6
005fd810: mov      sb, r5
005fd814: movne    r8, r4
005fd818: beq      #0x5fd874
005fd81c: ldrb     r3, [sp, #0x10c]
005fd820: ldrb     r2, [sp, #0x10d]
005fd824: mov      r0, r5
005fd828: lsl      r3, r3, #1
005fd82c: ldrh     ip, [r6, r3]
005fd830: lsl      r2, r2, #1
005fd834: mov      r1, sl
005fd838: strh     ip, [sp, #0xf0]
005fd83c: ldrh     r2, [r6, r2]
005fd840: strh     r2, [sp, #0xf2]
005fd844: ldrh     r3, [r5, r3]
005fd848: mov      r2, r7
005fd84c: strh     r3, [r6]
005fd850: ldrb     r3, [sp, #0x10d]
005fd854: lsl      r3, r3, #1
005fd858: ldrh     r3, [r5, r3]
005fd85c: add      r5, r5, r7
005fd860: strh     r3, [r6, #2]
005fd864: bl       #0x30e868
005fd868: subs     r8, r8, #1
005fd86c: add      r6, r6, #4
005fd870: bne      #0x5fd81c
005fd874: ldr      r0, [sp, #0x5c]
005fd878: ldr      r1, [sp, #0x54]
005fd87c: add      r5, sb, r0
005fd880: add      r6, fp, r1
005fd884: cmp      r5, r6
005fd888: bls      #0x5fd808
005fd88c: mov      r0, #1
005fd890: b        #0x5f964c

# _ZN6glitch5video8ITexture7setWrapENS0_15E_TEXTURE_CLAMPE
007d3bb4: ldr      r3, [r0, #0x38]
007d3bb8: str      r4, [sp, #-4]!
007d3bbc: ubfx     r2, r3, #0x12, #3
007d3bc0: cmp      r1, r2
007d3bc4: ldrhne   r2, [r0, #0x40]
007d3bc8: bfine    r3, r1, #0x12, #3
007d3bcc: strne    r3, [r0, #0x38]
007d3bd0: orrne    r2, r2, #0x10
007d3bd4: strhne   r2, [r0, #0x40]
007d3bd8: ubfx     r2, r3, #0x15, #3
007d3bdc: cmp      r1, r2
007d3be0: beq      #0x7d3c1c
007d3be4: and      ip, r1, #7
007d3be8: bic      r3, r3, #0xe00000
007d3bec: orr      r3, r3, ip, lsl #21
007d3bf0: ldrh     r2, [r0, #0x40]
007d3bf4: ubfx     r4, r3, #0x15, #3
007d3bf8: cmp      r1, r4
007d3bfc: orr      r2, r2, #0x20
007d3c00: str      r3, [r0, #0x38]
007d3c04: bicne    r3, r3, #0x7000000
007d3c08: strh     r2, [r0, #0x40]
007d3c0c: orrne    ip, r3, ip, lsl #24
007d3c10: orrne    r2, r2, #0x40
007d3c14: strhne   r2, [r0, #0x40]
007d3c18: strne    ip, [r0, #0x38]
007d3c1c: ldm      sp!, {r4}
007d3c20: bx       lr

# _ZN15bitmap_info_ogl6layoutEv
007d5c20: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d5c24: ldr      r4, [pc, #0x404]
007d5c28: ldr      r6, [pc, #0x404]
007d5c2c: ldr      r8, [r0, #0x10]
007d5c30: add      r4, pc, r4
007d5c34: ldr      r3, [r4, r6]
007d5c38: sub      sp, sp, #0x64
007d5c3c: cmp      r8, #0
007d5c40: ldr      r3, [r3]
007d5c44: mov      r5, r0
007d5c48: str      r3, [sp, #0x5c]
007d5c4c: beq      #0x7d5c6c
007d5c50: ldr      r3, [r4, r6]
007d5c54: ldr      r2, [sp, #0x5c]
007d5c58: ldr      r3, [r3]
007d5c5c: cmp      r2, r3
007d5c60: bne      #0x7d602c
007d5c64: add      sp, sp, #0x64
007d5c68: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d5c6c: ldr      r1, [pc, #0x3c4]
007d5c70: add      fp, sp, #0x1c
007d5c74: mov      r0, fp
007d5c78: add      r1, pc, r1
007d5c7c: mov      r2, r5
007d5c80: bl       #0x30eae4
007d5c84: ldr      r7, [r5, #0x28]
007d5c88: cmp      r7, #0
007d5c8c: beq      #0x7d5c9c
007d5c90: ldr      r3, [r7, #0x88]
007d5c94: tst      r3, #0x10
007d5c98: bne      #0x7d5ec4
007d5c9c: mov      r2, #0
007d5ca0: mov      r3, r7
007d5ca4: str      r2, [sp, #8]
007d5ca8: ldr      r8, [r3, #0xe0]
007d5cac: ldrb     sl, [r5, #0xc]
007d5cb0: cmp      r8, #0
007d5cb4: moveq    sl, r8
007d5cb8: beq      #0x7d5ce0
007d5cbc: ldr      r2, [r8, #0x74]
007d5cc0: ubfx     r3, r2, #4, #1
007d5cc4: cmp      sl, r3
007d5cc8: beq      #0x7d5ce0
007d5ccc: cmp      sl, #0
007d5cd0: orrne    r2, r2, #0x10
007d5cd4: biceq    r2, r2, #0x10
007d5cd8: str      r2, [r8, #0x74]
007d5cdc: mov      sl, r3
007d5ce0: ldr      sb, [r5, #0x18]
007d5ce4: cmp      sb, #0
007d5ce8: beq      #0x7d5eec
007d5cec: ldrb     r3, [r5, #0xc]
007d5cf0: cmp      r3, #0
007d5cf4: ldreq    r3, [r5, #0x28]
007d5cf8: beq      #0x7d5e84
007d5cfc: ldr      r3, [r5, #0x28]
007d5d00: ldr      r2, [r3, #0x9c]
007d5d04: and      r2, r2, #0x6000
007d5d08: cmp      r2, #0x6000
007d5d0c: bne      #0x7d5e84
007d5d10: ldr      r2, [sb, #0x20]
007d5d14: ldr      r1, [pc, #0x320]
007d5d18: mov      r0, #0x28
007d5d1c: mul      r2, r0, r2
007d5d20: ldr      r1, [r4, r1]
007d5d24: ldr      r2, [r1, r2]
007d5d28: tst      r2, #8
007d5d2c: bne      #0x7d5e84
007d5d30: ldrb     ip, [sb, #0x28]
007d5d34: cmp      ip, #0
007d5d38: bne      #0x7d5e84
007d5d3c: add      sb, sp, #0x18
007d5d40: ldr      r1, [r3, #0xe0]
007d5d44: mov      r2, fp
007d5d48: mov      r0, sb
007d5d4c: add      r3, r5, #0x18
007d5d50: str      ip, [sp]
007d5d54: mov      ip, #1
007d5d58: str      ip, [sp, #4]
007d5d5c: bl       #0x5ecaa4
007d5d60: mov      r1, sb
007d5d64: add      r0, r5, #0x10
007d5d68: bl       #0x384df8
007d5d6c: ldr      r0, [sp, #0x18]
007d5d70: cmp      r0, #0
007d5d74: beq      #0x7d5d7c
007d5d78: bl       #0x31d584
007d5d7c: ldr      r3, [r5, #0x10]
007d5d80: ldr      r1, [pc, #0x2b8]
007d5d84: ldr      r0, [r5, #0x2c]
007d5d88: mov      r2, #1
007d5d8c: strb     r2, [r5, #0xd]
007d5d90: add      r1, pc, r1
007d5d94: ldr      r2, [r3, #0x38]
007d5d98: ldr      r1, [r1, r0, lsl #2]
007d5d9c: ubfx     r0, r2, #0xc, #3
007d5da0: cmp      r1, r0
007d5da4: beq      #0x7d5dd8
007d5da8: ldrb     r0, [r3, #0x3e]
007d5dac: cmp      r0, #1
007d5db0: bls      #0x7d6014
007d5db4: ldrh     r0, [r3, #0x40]
007d5db8: and      r1, r1, #7
007d5dbc: bic      r2, r2, #0x7000
007d5dc0: orr      r2, r2, r1, lsl #12
007d5dc4: orr      r1, r0, #4
007d5dc8: str      r2, [r3, #0x38]
007d5dcc: strh     r1, [r3, #0x40]
007d5dd0: ldr      r3, [r5, #0x10]
007d5dd4: ldr      r2, [r3, #0x38]
007d5dd8: ldr      r1, [pc, #0x264]
007d5ddc: ldr      ip, [r5, #0x30]
007d5de0: ubfx     r0, r2, #0xf, #3
007d5de4: add      r1, pc, r1
007d5de8: ldr      r1, [r1, ip, lsl #2]
007d5dec: cmp      r1, r0
007d5df0: beq      #0x7d5e10
007d5df4: ldrh     r0, [r3, #0x40]
007d5df8: and      r1, r1, #7
007d5dfc: bic      r2, r2, #0x38000
007d5e00: orr      r2, r2, r1, lsl #15
007d5e04: orr      r1, r0, #8
007d5e08: strh     r1, [r3, #0x40]
007d5e0c: str      r2, [r3, #0x38]
007d5e10: ldr      r0, [r5, #0x18]
007d5e14: mov      r3, #0
007d5e18: str      r3, [r5, #0x18]
007d5e1c: cmp      r0, r3
007d5e20: beq      #0x7d5e28
007d5e24: bl       #0x31d584
007d5e28: cmp      r8, #0
007d5e2c: beq      #0x7d5e50
007d5e30: ldr      r3, [r8, #0x74]
007d5e34: ubfx     r2, r3, #4, #1
007d5e38: cmp      sl, r2
007d5e3c: beq      #0x7d5e50
007d5e40: cmp      sl, #0
007d5e44: orrne    r3, r3, #0x10
007d5e48: biceq    r3, r3, #0x10
007d5e4c: str      r3, [r8, #0x74]
007d5e50: cmp      r7, #0
007d5e54: beq      #0x7d5c50
007d5e58: ldr      r3, [r7, #0x88]
007d5e5c: ldr      r2, [sp, #8]
007d5e60: ubfx     r3, r3, #4, #1
007d5e64: cmp      r2, r3
007d5e68: beq      #0x7d5c50
007d5e6c: mov      r0, r7
007d5e70: ldr      r3, [r7]
007d5e74: mov      r1, #0x10
007d5e78: mov      lr, pc
007d5e7c: ldr      pc, [r3, #0xa0]
007d5e80: b        #0x7d5c50
007d5e84: add      sb, sp, #0x14
007d5e88: ldr      r1, [r3, #0xe0]
007d5e8c: mov      ip, #0
007d5e90: mov      r2, fp
007d5e94: mov      r0, sb
007d5e98: add      r3, r5, #0x18
007d5e9c: str      ip, [sp, #4]
007d5ea0: str      ip, [sp]
007d5ea4: bl       #0x5ecaa4
007d5ea8: mov      r1, sb
007d5eac: add      r0, r5, #0x10
007d5eb0: bl       #0x384df8
007d5eb4: ldr      r0, [sp, #0x14]
007d5eb8: cmp      r0, #0
007d5ebc: bne      #0x7d5d78
007d5ec0: b        #0x7d5d7c
007d5ec4: mov      r2, r8
007d5ec8: ldr      r3, [r7]
007d5ecc: mov      r0, r7
007d5ed0: mov      r1, #0x10
007d5ed4: mov      lr, pc
007d5ed8: ldr      pc, [r3, #0xa0]
007d5edc: mov      r2, #1
007d5ee0: str      r2, [sp, #8]
007d5ee4: ldr      r3, [r5, #0x28]
007d5ee8: b        #0x7d5ca8
007d5eec: ldr      r0, [r5, #0x1c]
007d5ef0: cmp      r0, #0
007d5ef4: beq      #0x7d5e28
007d5ef8: ldr      r1, [r0]
007d5efc: mov      r2, fp
007d5f00: mov      r3, sb
007d5f04: ldr      r0, [r0, #8]
007d5f08: bl       #0x56f370
007d5f0c: str      r0, [sp, #0xc]
007d5f10: ldr      r2, [r5, #0x28]
007d5f14: add      fp, sp, #0x10
007d5f18: mov      r3, sb
007d5f1c: ldr      r1, [r2, #0xe0]
007d5f20: mov      r0, fp
007d5f24: ldr      r2, [sp, #0xc]
007d5f28: str      sb, [sp]
007d5f2c: bl       #0x5ed0c4
007d5f30: mov      r1, fp
007d5f34: add      r0, r5, #0x10
007d5f38: bl       #0x384df8
007d5f3c: ldr      r0, [sp, #0x10]
007d5f40: cmp      r0, #0
007d5f44: beq      #0x7d5f4c
007d5f48: bl       #0x31d584
007d5f4c: ldr      r3, [r5, #0x10]
007d5f50: ldr      r1, [pc, #0xf0]
007d5f54: ldr      r0, [r5, #0x2c]
007d5f58: ldr      r2, [r3, #0x38]
007d5f5c: add      r1, pc, r1
007d5f60: ldr      r1, [r1, r0, lsl #2]
007d5f64: ubfx     r0, r2, #0xc, #3
007d5f68: cmp      r1, r0
007d5f6c: beq      #0x7d5fa0
007d5f70: ldrb     r0, [r3, #0x3e]
007d5f74: cmp      r0, #1
007d5f78: bls      #0x7d6020
007d5f7c: ldrh     r0, [r3, #0x40]
007d5f80: and      r1, r1, #7
007d5f84: bic      r2, r2, #0x7000
007d5f88: orr      r2, r2, r1, lsl #12
007d5f8c: orr      r1, r0, #4
007d5f90: str      r2, [r3, #0x38]
007d5f94: strh     r1, [r3, #0x40]
007d5f98: ldr      r3, [r5, #0x10]
007d5f9c: ldr      r2, [r3, #0x38]
007d5fa0: ldr      r1, [pc, #0xa4]
007d5fa4: ldr      ip, [r5, #0x30]
007d5fa8: ubfx     r0, r2, #0xf, #3
007d5fac: add      r1, pc, r1
007d5fb0: ldr      r1, [r1, ip, lsl #2]
007d5fb4: cmp      r1, r0
007d5fb8: beq      #0x7d5fd8
007d5fbc: ldrh     r0, [r3, #0x40]
007d5fc0: and      r1, r1, #7
007d5fc4: bic      r2, r2, #0x38000
007d5fc8: orr      r2, r2, r1, lsl #15
007d5fcc: orr      r1, r0, #8
007d5fd0: strh     r1, [r3, #0x40]
007d5fd4: str      r2, [r3, #0x38]
007d5fd8: mov      r3, #1
007d5fdc: strb     r3, [r5, #0xd]
007d5fe0: ldr      r0, [sp, #0xc]
007d5fe4: bl       #0x31d584
007d5fe8: ldr      sb, [r5, #0x1c]
007d5fec: cmp      sb, #0
007d5ff0: beq      #0x7d6008
007d5ff4: mov      r0, sb
007d5ff8: bl       #0x7b6548
007d5ffc: mov      r0, sb
007d6000: mov      r1, #0
007d6004: bl       #0x752b38
007d6008: mov      r3, #0
007d600c: str      r3, [r5, #0x1c]
007d6010: b        #0x7d5e28
007d6014: cmp      r1, #1
007d6018: bgt      #0x7d5dd8
007d601c: b        #0x7d5db4
007d6020: cmp      r1, #1
007d6024: bgt      #0x7d5fa0
007d6028: b        #0x7d5f7c
007d602c: bl       #0x30e310
007d6030: andseq   lr, fp, r0, ror #28
007d6034: andeq    r4, r0, ip, lsr #1
007d6038: andseq   r6, r3, r0, lsr #8
007d603c: andeq    r1, r0, r4, lsr pc
007d6040: andseq   r6, r3, ip, lsr #5
007d6044: andseq   r6, r3, r8, asr r2
007d6048: andseq   r6, r3, r0, ror #1
007d604c: mulseq   r3, r0, r0
