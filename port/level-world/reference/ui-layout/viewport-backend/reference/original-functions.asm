
# _ZN7gameswf10scene_node15get_world_mouseERiS1_
007775c4: push     {r4, r5, r6, lr}
007775c8: mov      r4, r0
007775cc: mov      r5, r1
007775d0: mov      r6, r2
007775d4: bl       #0x777350
007775d8: ldr      r0, [r4, #0x260]
007775dc: bl       #0x30e4cc
007775e0: str      r0, [r5]
007775e4: ldr      r0, [r4, #0x264]
007775e8: bl       #0x30e4cc
007775ec: str      r0, [r6]
007775f0: pop      {r4, r5, r6, pc}

# _ZN7gameswf13export_loaderEPNS_6streamEiPNS_20movie_definition_subE
0075c5f8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0075c5fc: ldr      r1, [pc, #0x1c4]
0075c600: ldr      r3, [pc, #0x1c4]
0075c604: sub      sp, sp, #0x34
0075c608: add      r1, pc, r1
0075c60c: str      r3, [sp, #0xc]
0075c610: ldr      r3, [r1, r3]
0075c614: str      r1, [sp, #8]
0075c618: mov      r4, r2
0075c61c: ldr      r3, [r3]
0075c620: mov      r7, r0
0075c624: str      r3, [sp, #0x2c]
0075c628: bl       #0x783c14
0075c62c: subs     r8, r0, #0
0075c630: beq      #0x75c6fc
0075c634: ldr      r3, [pc, #0x194]
0075c638: add      r6, sp, #0x18
0075c63c: mov      sl, #1
0075c640: mov      r5, #0
0075c644: add      r3, pc, r3
0075c648: add      r1, r6, sl
0075c64c: str      r3, [sp, #0x10]
0075c650: mov      fp, r5
0075c654: str      r1, [sp, #0x14]
0075c658: mov      sb, r8
0075c65c: b        #0x75c66c
0075c660: add      r5, r5, #1
0075c664: cmp      sb, r5
0075c668: ble      #0x75c6fc
0075c66c: mov      r0, r7
0075c670: bl       #0x783c14
0075c674: ldr      r3, [sp, #0x28]
0075c678: mvn      r2, #0
0075c67c: mov      r8, r0
0075c680: bfi      r3, r2, #0, #0x18
0075c684: lsr      r2, r3, #0x18
0075c688: bfc      r2, #0, #1
0075c68c: mov      r0, r7
0075c690: mov      r1, r6
0075c694: str      r3, [sp, #0x28]
0075c698: strb     sl, [sp, #0x18]
0075c69c: strb     r2, [sp, #0x2b]
0075c6a0: strb     fp, [sp, #0x19]
0075c6a4: bl       #0x7841c8
0075c6a8: ldr      r3, [r4]
0075c6ac: mov      r0, r4
0075c6b0: mov      r1, r8
0075c6b4: mov      lr, pc
0075c6b8: ldr      pc, [r3, #0x7c]
0075c6bc: subs     r2, r0, #0
0075c6c0: beq      #0x75c720
0075c6c4: ldr      r3, [r4]
0075c6c8: mov      r0, r4
0075c6cc: mov      r1, r6
0075c6d0: mov      lr, pc
0075c6d4: ldr      pc, [r3, #0xa8]
0075c6d8: ldrsb    r3, [sp, #0x18]
0075c6dc: cmn      r3, #1
0075c6e0: bne      #0x75c660
0075c6e4: ldr      r0, [sp, #0x24]
0075c6e8: ldr      r1, [sp, #0x20]
0075c6ec: add      r5, r5, #1
0075c6f0: bl       #0x752b38
0075c6f4: cmp      sb, r5
0075c6f8: bgt      #0x75c66c
0075c6fc: ldr      r2, [sp, #8]
0075c700: ldr      r1, [sp, #0xc]
0075c704: ldr      r3, [r2, r1]
0075c708: ldr      r2, [sp, #0x2c]
0075c70c: ldr      r3, [r3]
0075c710: cmp      r2, r3
0075c714: bne      #0x75c7c4
0075c718: add      sp, sp, #0x34
0075c71c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0075c720: ldr      r3, [r4]
0075c724: mov      r0, r4
0075c728: mov      r1, r8
0075c72c: mov      lr, pc
0075c730: ldr      pc, [r3, #0x5c]
0075c734: subs     r2, r0, #0
0075c738: bne      #0x75c6c4
0075c73c: ldr      r3, [r4]
0075c740: mov      r0, r4
0075c744: mov      r1, r8
0075c748: mov      lr, pc
0075c74c: ldr      pc, [r3, #0x98]
0075c750: subs     ip, r0, #0
0075c754: beq      #0x75c78c
0075c758: ldr      r3, [r4]
0075c75c: mov      r0, r4
0075c760: mov      r1, r6
0075c764: mov      r2, ip
0075c768: str      ip, [sp, #4]
0075c76c: mov      lr, pc
0075c770: ldr      pc, [r3, #0xa8]
0075c774: ldr      ip, [sp, #4]
0075c778: mov      r0, r6
0075c77c: mov      r2, r4
0075c780: mov      r1, ip
0075c784: bl       #0x759cfc
0075c788: b        #0x75c6d8
0075c78c: mov      r1, r8
0075c790: ldr      r3, [r4]
0075c794: mov      r0, r4
0075c798: mov      lr, pc
0075c79c: ldr      pc, [r3, #0xa0]
0075c7a0: subs     r2, r0, #0
0075c7a4: bne      #0x75c6c4
0075c7a8: ldrsb    r3, [sp, #0x18]
0075c7ac: ldr      r0, [sp, #0x10]
0075c7b0: cmn      r3, #1
0075c7b4: ldrne    r1, [sp, #0x14]
0075c7b8: ldreq    r1, [sp, #0x24]
0075c7bc: bl       #0x761184
0075c7c0: b        #0x75c6d8
0075c7c4: bl       #0x30e310
0075c7c8: eoreq    r8, r3, r8, lsl #9
0075c7cc: andeq    r4, r0, ip, lsr #1
0075c7d0: andseq   ip, sl, ip, lsl #11

# _ZN7gameswf4root17screen_to_logicalERNS_5pointE
00773dc0: ldr      r3, [pc, #0x180]
00773dc4: ldr      r2, [pc, #0x180]
00773dc8: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00773dcc: add      r3, pc, r3
00773dd0: ldr      r2, [r3, r2]
00773dd4: mov      r4, r0
00773dd8: mov      r5, r1
00773ddc: ldr      r3, [r2]
00773de0: mov      r0, r3
00773de4: ldr      r3, [r3]
00773de8: mov      lr, pc
00773dec: ldr      pc, [r3, #0xac]
00773df0: cmp      r0, #0
00773df4: cmpne    r0, #2
00773df8: bne      #0x773eb0
00773dfc: ldr      r0, [r4, #0x30]
00773e00: bl       #0x30e964
00773e04: ldr      r6, [r4, #0xc]
00773e08: mov      r7, r0
00773e0c: ldr      r1, [r6, #0xbc]
00773e10: ldr      r0, [r6, #0xc0]
00773e14: bl       #0x30e3ac
00773e18: mov      r1, #0x41000000
00773e1c: add      r1, r1, #0xa00000
00773e20: bl       #0x30ec94
00773e24: mov      r1, r0
00773e28: mov      r0, r7
00773e2c: bl       #0x30ec94
00773e30: mov      r7, r0
00773e34: ldr      r0, [r4, #0x24]
00773e38: bl       #0x30e964
00773e3c: mov      r1, r0
00773e40: ldr      r0, [r5]
00773e44: bl       #0x30e3ac
00773e48: mov      r8, r0
00773e4c: ldr      r0, [r4, #0x2c]
00773e50: bl       #0x30e964
00773e54: ldr      r1, [r6, #0xb4]
00773e58: mov      sl, r0
00773e5c: ldr      r0, [r6, #0xb8]
00773e60: bl       #0x30e3ac
00773e64: mov      r1, #0x41000000
00773e68: add      r1, r1, #0xa00000
00773e6c: bl       #0x30ec94
00773e70: mov      r1, r0
00773e74: mov      r0, sl
00773e78: bl       #0x30ec94
00773e7c: mov      r1, r0
00773e80: mov      r0, r8
00773e84: bl       #0x30ec94
00773e88: str      r0, [r5]
00773e8c: ldr      r0, [r4, #0x28]
00773e90: bl       #0x30e964
00773e94: mov      r1, r0
00773e98: ldr      r0, [r5, #4]
00773e9c: bl       #0x30e3ac
00773ea0: mov      r1, r7
00773ea4: bl       #0x30ec94
00773ea8: str      r0, [r5, #4]
00773eac: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00773eb0: ldr      r0, [r4, #0x2c]
00773eb4: bl       #0x30e964
00773eb8: ldr      r6, [r4, #0xc]
00773ebc: mov      r7, r0
00773ec0: ldr      r1, [r6, #0xbc]
00773ec4: ldr      r0, [r6, #0xc0]
00773ec8: bl       #0x30e3ac
00773ecc: mov      r1, #0x41000000
00773ed0: add      r1, r1, #0xa00000
00773ed4: bl       #0x30ec94
00773ed8: mov      r1, r0
00773edc: mov      r0, r7
00773ee0: bl       #0x30ec94
00773ee4: mov      r7, r0
00773ee8: ldr      r0, [r4, #0x28]
00773eec: bl       #0x30e964
00773ef0: mov      r1, r0
00773ef4: ldr      r0, [r5]
00773ef8: bl       #0x30e3ac
00773efc: mov      r8, r0
00773f00: ldr      r0, [r4, #0x30]
00773f04: bl       #0x30e964
00773f08: ldr      r1, [r6, #0xb4]
00773f0c: mov      sl, r0
00773f10: ldr      r0, [r6, #0xb8]
00773f14: bl       #0x30e3ac
00773f18: mov      r1, #0x41000000
00773f1c: add      r1, r1, #0xa00000
00773f20: bl       #0x30ec94
00773f24: mov      r1, r0
00773f28: mov      r0, sl
00773f2c: bl       #0x30ec94
00773f30: mov      r1, r0
00773f34: mov      r0, r8
00773f38: bl       #0x30ec94
00773f3c: str      r0, [r5]
00773f40: ldr      r0, [r4, #0x24]
00773f44: b        #0x773e90
00773f48: eoreq    r0, r2, r4, asr #25
00773f4c: strheq   r3, [r0], -r4

# _ZN21render_handler_glitch13begin_displayEN7gameswf4rgbaEiiiiffff
007d73d0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007d73d4: sub      sp, sp, #0xac
007d73d8: mov      r4, r0
007d73dc: str      r1, [sp, #0xc]
007d73e0: ldr      r0, [sp, #0xdc]
007d73e4: ldr      r1, [sp, #0xd8]
007d73e8: mov      r5, r3
007d73ec: mov      r6, r2
007d73f0: bl       #0x30e3ac
007d73f4: bic      r3, r0, #0x80000000
007d73f8: str      r3, [r4, #0x304]
007d73fc: ldr      r1, [sp, #0xe0]
007d7400: mov      r8, r0
007d7404: ldr      r0, [sp, #0xe4]
007d7408: bl       #0x30e3ac
007d740c: ldr      r3, [r4, #0x10]
007d7410: bic      r2, r0, #0x80000000
007d7414: str      r2, [r4, #0x308]
007d7418: mov      r1, #2
007d741c: mov      sl, r0
007d7420: mov      r0, r3
007d7424: ldr      r3, [r3]
007d7428: mov      lr, pc
007d742c: ldr      pc, [r3, #0x70]
007d7430: mov      r2, #0x41
007d7434: mov      r1, r0
007d7438: add      r0, r4, #0x38
007d743c: bl       #0x30e868
007d7440: ldr      r3, [r4, #0x10]
007d7444: mov      r1, #0
007d7448: ldr      r7, [pc, #0x358]
007d744c: mov      r0, r3
007d7450: ldr      r3, [r3]
007d7454: mov      lr, pc
007d7458: ldr      pc, [r3, #0x70]
007d745c: mov      r2, #0x41
007d7460: mov      r1, r0
007d7464: add      r0, r4, #0x7c
007d7468: bl       #0x30e868
007d746c: ldr      r3, [r4, #0x10]
007d7470: mov      r1, #1
007d7474: add      r7, pc, r7
007d7478: mov      r0, r3
007d747c: ldr      r3, [r3]
007d7480: mov      lr, pc
007d7484: ldr      pc, [r3, #0x70]
007d7488: mov      r1, r0
007d748c: mov      r2, #0x41
007d7490: add      r0, r4, #0xc0
007d7494: bl       #0x30e868
007d7498: ldr      r3, [r4, #0x10]
007d749c: add      r1, r4, #0x14
007d74a0: ldr      r2, [r3, #0xcc]
007d74a4: mov      r0, r3
007d74a8: ldr      r2, [r2, #-4]
007d74ac: ldr      ip, [r2, #0x14]
007d74b0: str      ip, [r4, #0x104]
007d74b4: ldr      ip, [r2, #0x18]
007d74b8: str      ip, [r4, #0x108]
007d74bc: ldr      ip, [r2, #0x1c]
007d74c0: str      ip, [r4, #0x10c]
007d74c4: ldr      r2, [r2, #0x20]
007d74c8: str      r2, [r4, #0x110]
007d74cc: ldr      r2, [r3, #0x88]
007d74d0: ubfx     r2, r2, #8, #1
007d74d4: strb     r2, [r4, #0x301]
007d74d8: ldr      r3, [r3]
007d74dc: mov      lr, pc
007d74e0: ldr      pc, [r3, #0x1e0]
007d74e4: ldr      r1, [sp, #0xd0]
007d74e8: ldr      r2, [sp, #0xd4]
007d74ec: ldr      r3, [r4, #0x10]
007d74f0: add      r1, r1, r6
007d74f4: add      r2, r2, r5
007d74f8: str      r2, [sp, #0xa4]
007d74fc: str      r5, [sp, #0x9c]
007d7500: str      r1, [sp, #0xa0]
007d7504: str      r6, [sp, #0x98]
007d7508: ldr      r3, [r3, #0xcc]
007d750c: add      r1, sp, #0x98
007d7510: mov      r5, #0
007d7514: ldr      r3, [r3, #-4]
007d7518: mov      r0, r3
007d751c: ldr      r3, [r3]
007d7520: mov      lr, pc
007d7524: ldr      pc, [r3, #0xc]
007d7528: add      r0, r4, #0x1f0
007d752c: bl       #0x7d72b4
007d7530: ldr      r3, [r4, #0x10]
007d7534: mov      r1, #0x100
007d7538: mov      r2, #0
007d753c: mov      r0, r3
007d7540: ldr      r3, [r3]
007d7544: mov      lr, pc
007d7548: ldr      pc, [r3, #0xa0]
007d754c: ldr      r3, [r4, #0x10]
007d7550: mov      r1, #2
007d7554: mov      r0, r3
007d7558: ldr      r3, [r3]
007d755c: mov      lr, pc
007d7560: ldr      pc, [r3, #0xa8]
007d7564: ldr      r3, [r4, #0x350]
007d7568: mov      r0, #0
007d756c: str      r5, [r4, #0x344]
007d7570: cmp      r3, r5
007d7574: str      r0, [r4, #0x348]
007d7578: ble      #0x7d75e4
007d757c: mov      r6, r5
007d7580: b        #0x7d7598
007d7584: str      r6, [lr, #4]
007d7588: ldr      r3, [r4, #0x350]
007d758c: add      r5, r5, #1
007d7590: cmp      r5, r3
007d7594: bge      #0x7d75e4
007d7598: ldr      lr, [r4, #0x34c]
007d759c: add      lr, lr, r5, lsl #4
007d75a0: ldr      r2, [lr, #4]
007d75a4: cmp      r2, #0
007d75a8: bgt      #0x7d7584
007d75ac: bge      #0x7d7584
007d75b0: lsl      r3, r2, #3
007d75b4: ldr      r1, [lr]
007d75b8: adds     r2, r2, #1
007d75bc: add      ip, r1, r3
007d75c0: str      r0, [r1, r3]
007d75c4: str      r0, [ip, #4]
007d75c8: add      r3, r3, #8
007d75cc: bne      #0x7d75b4
007d75d0: str      r6, [lr, #4]
007d75d4: ldr      r3, [r4, #0x350]
007d75d8: add      r5, r5, #1
007d75dc: cmp      r5, r3
007d75e0: blt      #0x7d7598
007d75e4: add      sb, r4, #0x114
007d75e8: mov      ip, #0xbf000000
007d75ec: mov      r6, #0
007d75f0: add      ip, ip, #0x800000
007d75f4: mov      r5, #0
007d75f8: mov      fp, #0x3f800000
007d75fc: mov      r3, #0x3f000000
007d7600: add      r1, sp, #0x14
007d7604: mov      r2, #0x41
007d7608: mov      r0, sb
007d760c: str      ip, [sp, #0x28]
007d7610: str      ip, [sp, #4]
007d7614: str      r3, [sp, #0x4c]
007d7618: str      r6, [sp, #0x18]
007d761c: str      r6, [sp, #0x1c]
007d7620: str      r6, [sp, #0x20]
007d7624: str      r6, [sp, #0x24]
007d7628: str      r6, [sp, #0x2c]
007d762c: str      r6, [sp, #0x30]
007d7630: str      r6, [sp, #0x34]
007d7634: str      r6, [sp, #0x38]
007d7638: str      r3, [sp, #0x3c]
007d763c: str      r6, [sp, #0x40]
007d7640: str      r6, [sp, #0x44]
007d7644: str      r6, [sp, #0x48]
007d7648: strb     r5, [sp, #0x54]
007d764c: str      fp, [sp, #0x14]
007d7650: str      fp, [sp, #0x50]
007d7654: bl       #0x30e868
007d7658: ldr      r3, [r4, #0x10]
007d765c: mov      r2, sb
007d7660: mov      r1, #2
007d7664: mov      r0, r3
007d7668: ldr      r3, [r3]
007d766c: mov      lr, pc
007d7670: ldr      pc, [r3, #0x6c]
007d7674: ldr      r3, [pc, #0x130]
007d7678: add      sb, r4, #0x158
007d767c: add      r1, r4, #0x19c
007d7680: ldr      r7, [r7, r3]
007d7684: mov      r2, #0x41
007d7688: str      r1, [sp, #8]
007d768c: mov      r0, sb
007d7690: mov      r1, r7
007d7694: bl       #0x30e868
007d7698: mov      r1, r8
007d769c: mov      r0, #0x40000000
007d76a0: bl       #0x30ec94
007d76a4: mov      r1, sl
007d76a8: str      r0, [sp, #0x58]
007d76ac: mov      r0, #0x40000000
007d76b0: str      r6, [sp, #0x5c]
007d76b4: str      r6, [sp, #0x60]
007d76b8: str      r6, [sp, #0x64]
007d76bc: str      r6, [sp, #0x68]
007d76c0: bl       #0x30ec94
007d76c4: ldr      ip, [sp, #4]
007d76c8: ldr      r1, [sp, #0xd8]
007d76cc: str      r0, [sp, #0x6c]
007d76d0: ldr      r0, [sp, #0xdc]
007d76d4: str      ip, [sp, #0x80]
007d76d8: str      r6, [sp, #0x84]
007d76dc: str      r6, [sp, #0x70]
007d76e0: str      r6, [sp, #0x74]
007d76e4: str      r6, [sp, #0x78]
007d76e8: str      r6, [sp, #0x7c]
007d76ec: bl       #0x30eba4
007d76f0: mov      r1, r8
007d76f4: add      r0, r0, #0x80000000
007d76f8: bl       #0x30ec94
007d76fc: ldr      r1, [sp, #0xe0]
007d7700: str      r0, [sp, #0x88]
007d7704: ldr      r0, [sp, #0xe4]
007d7708: bl       #0x30eba4
007d770c: mov      r1, sl
007d7710: add      r0, r0, #0x80000000
007d7714: bl       #0x30ec94
007d7718: add      lr, sp, #0x58
007d771c: str      r0, [sp, #0x8c]
007d7720: mov      ip, sb
007d7724: strb     r5, [r4, #0x198]
007d7728: ldm      lr!, {r0, r1, r2, r3}
007d772c: stm      ip!, {r0, r1, r2, r3}
007d7730: ldm      lr!, {r0, r1, r2, r3}
007d7734: stm      ip!, {r0, r1, r2, r3}
007d7738: mov      r6, #0x80000000
007d773c: ldm      lr!, {r0, r1, r2, r3}
007d7740: str      fp, [sp, #0x94]
007d7744: str      r6, [sp, #0x90]
007d7748: stm      ip!, {r0, r1, r2, r3}
007d774c: ldm      lr, {r0, r1, r2, r3}
007d7750: stm      ip, {r0, r1, r2, r3}
007d7754: ldr      r3, [r4, #0x10]
007d7758: strb     r5, [r4, #0x198]
007d775c: mov      r1, r5
007d7760: mov      r0, r3
007d7764: mov      r2, sb
007d7768: ldr      r3, [r3]
007d776c: mov      lr, pc
007d7770: ldr      pc, [r3, #0x6c]
007d7774: mov      r1, r7
007d7778: mov      r2, #0x41
007d777c: ldr      r0, [sp, #8]
007d7780: bl       #0x30e868
007d7784: ldr      r3, [r4, #0x10]
007d7788: ldr      r2, [sp, #8]
007d778c: mov      r1, #1
007d7790: mov      r0, r3
007d7794: ldr      r3, [r3]
007d7798: mov      lr, pc
007d779c: ldr      pc, [r3, #0x6c]
007d77a0: add      sp, sp, #0xac
007d77a4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007d77a8: andseq   sp, fp, ip, lsl r6
007d77ac: andeq    r2, r0, r0, lsr r8

# _ZN21render_handler_glitch17fill_style_bitmapEiPN7gameswf11bitmap_infoERKNS0_6matrixENS0_14render_handler16bitmap_wrap_modeE
007d4558: mov      ip, #0x4c
007d455c: mul      r1, ip, r1
007d4560: mov      ip, r0
007d4564: add      r1, r1, #0x3b0
007d4568: add      r0, r0, r1
007d456c: mov      r1, r2
007d4570: mov      r2, r3
007d4574: ldr      r3, [sp]
007d4578: add      ip, ip, #0x324
007d457c: str      ip, [sp]
007d4580: b        #0x7d4430

# _ZN8RenderFX11SetViewportEiiii
007a9bac: push     {r4, r5, r6, r7, r8, lr}
007a9bb0: sub      sp, sp, #8
007a9bb4: ldr      r0, [r0, #0x38]
007a9bb8: mov      r8, r1
007a9bbc: mov      r7, r2
007a9bc0: mov      r6, r3
007a9bc4: ldr      r5, [sp, #0x20]
007a9bc8: bl       #0x76d5b4
007a9bcc: subs     r4, r0, #0
007a9bd0: beq      #0x7a9c00
007a9bd4: bl       #0x759c64
007a9bd8: mov      r0, r4
007a9bdc: mov      r1, r8
007a9be0: mov      r2, r7
007a9be4: mov      r3, r6
007a9be8: str      r5, [sp]
007a9bec: bl       #0x775d38
007a9bf0: mov      r0, r4
007a9bf4: add      sp, sp, #8
007a9bf8: pop      {r4, r5, r6, r7, r8, lr}
007a9bfc: b        #0x75a240
007a9c00: mov      r1, r8
007a9c04: mov      r2, r7
007a9c08: mov      r3, r6
007a9c0c: str      r5, [sp, #0x20]
007a9c10: add      sp, sp, #8
007a9c14: pop      {r4, r5, r6, r7, r8, lr}
007a9c18: b        #0x775d38

# _ZNK7gameswf6matrix20transform_by_inverseEPNS_5pointERKS1_
00753d7c: push     {r4, r5, r6, r7, lr}
00753d80: sub      sp, sp, #0x1c
00753d84: mov      ip, #0
00753d88: add      r3, sp, #8
00753d8c: str      ip, [r3], #4
00753d90: str      ip, [r3], #4
00753d94: str      ip, [r3], #4
00753d98: mov      r7, r0
00753d9c: mov      r4, r2
00753da0: mov      lr, #0x3f800000
00753da4: str      ip, [r3]
00753da8: mov      r5, r1
00753dac: mov      r0, sp
00753db0: mov      r1, r7
00753db4: str      lr, [sp, #0x10]
00753db8: str      ip, [sp, #4]
00753dbc: str      lr, [sp]
00753dc0: bl       #0x795adc
00753dc4: ldr      r1, [r4]
00753dc8: ldr      r0, [sp]
00753dcc: bl       #0x30ed6c
00753dd0: ldr      r1, [r4, #4]
00753dd4: mov      r6, r0
00753dd8: ldr      r0, [sp, #4]
00753ddc: bl       #0x30ed6c
00753de0: mov      r1, r0
00753de4: mov      r0, r6
00753de8: bl       #0x30eba4
00753dec: ldr      r1, [sp, #8]
00753df0: bl       #0x30eba4
00753df4: str      r0, [r5]
00753df8: ldr      r1, [r4]
00753dfc: ldr      r0, [sp, #0xc]
00753e00: bl       #0x30ed6c
00753e04: ldr      r1, [r4, #4]
00753e08: mov      r6, r0
00753e0c: ldr      r0, [sp, #0x10]
00753e10: bl       #0x30ed6c
00753e14: mov      r1, r0
00753e18: mov      r0, r6
00753e1c: bl       #0x30eba4
00753e20: ldr      r1, [sp, #0x14]
00753e24: bl       #0x30eba4
00753e28: str      r0, [r5, #4]
00753e2c: add      sp, sp, #0x1c
00753e30: pop      {r4, r5, r6, r7, pc}

# _ZN7gameswf4root7advanceEfb
00775304: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00775308: mov      r5, r1
0077530c: sub      sp, sp, #0xc
00775310: add      r6, r0, #0xb8
00775314: mov      r4, r0
00775318: mov      r7, r2
0077531c: bl       #0x773d38
00775320: mov      r1, r5
00775324: mov      r0, r6
00775328: bl       #0x760d58
0077532c: ldr      r1, [r4, #0x8c]
00775330: mov      r0, r5
00775334: bl       #0x30eba4
00775338: mov      r1, r5
0077533c: mov      r8, r0
00775340: str      r0, [r4, #0x8c]
00775344: ldr      r0, [r4, #0x94]
00775348: bl       #0x30e3ac
0077534c: ldr      r1, [r4, #0x90]
00775350: str      r0, [r4, #0x94]
00775354: mov      r0, r8
00775358: bl       #0x30e4b4
0077535c: cmp      r0, #0
00775360: bne      #0x775370
00775364: bl       #0x773d38
00775368: add      sp, sp, #0xc
0077536c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00775370: bl       #0x7b7898
00775374: ldrb     r3, [r4, #0x84]
00775378: cmp      r3, #0
0077537c: beq      #0x77548c
00775380: ldr      r1, [r4, #0x8c]
00775384: mov      sb, #1
00775388: mov      sl, #0xa
0077538c: mov      fp, sp
00775390: ldr      r0, [r4, #0x90]
00775394: bl       #0x30e9ac
00775398: cmp      r0, #0
0077539c: beq      #0x77545c
007753a0: ldrb     r3, [r4, #0x84]
007753a4: cmp      r3, #0
007753a8: bne      #0x7753f0
007753ac: ldr      r8, [r4, #0x10]
007753b0: cmp      r8, #0
007753b4: beq      #0x775484
007753b8: ldr      r3, [r8]
007753bc: mov      r0, r8
007753c0: mov      r1, #2
007753c4: mov      lr, pc
007753c8: ldr      pc, [r3, #8]
007753cc: cmp      r0, #0
007753d0: movne    r0, r8
007753d4: beq      #0x775484
007753d8: bl       #0x78069c
007753dc: ldr      r3, [r4, #0x10]
007753e0: mov      r0, r3
007753e4: ldr      r3, [r3]
007753e8: mov      lr, pc
007753ec: ldr      pc, [r3, #0x148]
007753f0: ldr      r0, [r4, #0x10]
007753f4: cmp      r7, #0
007753f8: moveq    r1, r5
007753fc: ldr      r3, [r0]
00775400: ldrne    r1, [r4, #0x90]
00775404: ldr      r3, [r3, #0x5c]
00775408: blx      r3
0077540c: ldrb     r2, [r4, #0x84]
00775410: cmp      r2, #0
00775414: bne      #0x775440
00775418: ldr      r0, [r4, #0x10]
0077541c: strb     sb, [r4, #0x84]
00775420: mov      r1, sp
00775424: ldr      r3, [r0]
00775428: ldr      r3, [r3, #0x2c]
0077542c: str      r2, [sp, #4]
00775430: strb     sl, [sp]
00775434: strb     r2, [sp, #1]
00775438: strh     r2, [sp, #2]
0077543c: blx      r3
00775440: ldr      r0, [r4, #0x8c]
00775444: ldr      r1, [r4, #0x90]
00775448: bl       #0x30e3ac
0077544c: cmp      r7, #0
00775450: mov      r1, r0
00775454: str      r0, [r4, #0x8c]
00775458: bne      #0x775390
0077545c: ldr      r0, [r4, #0x94]
00775460: mov      r1, #0
00775464: bl       #0x30e9ac
00775468: cmp      r0, #0
0077546c: bne      #0x7754cc
00775470: ldr      r0, [r4, #0x8c]
00775474: ldr      r1, [r4, #0x90]
00775478: bl       #0x30e7f0
0077547c: str      r0, [r4, #0x8c]
00775480: b        #0x775364
00775484: mov      r0, #0
00775488: b        #0x7753d8
0077548c: ldr      r1, [r4, #0xcc]
00775490: cmp      r1, #0
00775494: beq      #0x7754bc
00775498: ldr      r3, [r4, #0xc8]
0077549c: ldrb     r8, [r3, #4]
007754a0: cmp      r8, #0
007754a4: bne      #0x7754bc
007754a8: mov      r1, r8
007754ac: add      r0, r4, #0xc8
007754b0: bl       #0x41fe84
007754b4: str      r8, [r4, #0xcc]
007754b8: mov      r1, r8
007754bc: add      r1, r1, #0x68
007754c0: mov      r0, r4
007754c4: bl       #0x774660
007754c8: b        #0x775380
007754cc: ldr      r0, [r4, #0xcc]
007754d0: cmp      r0, #0
007754d4: beq      #0x7754e8
007754d8: ldr      r3, [r4, #0xc8]
007754dc: ldrb     r2, [r3, #4]
007754e0: cmp      r2, #0
007754e4: beq      #0x775548
007754e8: bl       #0x76c808
007754ec: mov      r0, r6
007754f0: bl       #0x760940
007754f4: ldr      r3, [r4, #0x10]
007754f8: mov      r0, r3
007754fc: ldr      r3, [r3]
00775500: mov      lr, pc
00775504: ldr      pc, [r3, #0x44]
00775508: ldr      r0, [r4, #0xcc]
0077550c: cmp      r0, #0
00775510: beq      #0x775538
00775514: ldr      r3, [r4, #0xc8]
00775518: ldrb     r5, [r3, #4]
0077551c: cmp      r5, #0
00775520: bne      #0x775538
00775524: add      r0, r4, #0xc8
00775528: mov      r1, r5
0077552c: bl       #0x41fe84
00775530: str      r5, [r4, #0xcc]
00775534: mov      r0, r5
00775538: bl       #0x76d2f8
0077553c: mov      r3, #0x40000000
00775540: str      r3, [r4, #0x94]
00775544: b        #0x775470
00775548: ldr      r1, [r3]
0077554c: sub      r1, r1, #1
00775550: cmp      r1, #0
00775554: str      r1, [r3]
00775558: bne      #0x775564
0077555c: mov      r0, r3
00775560: bl       #0x752b38
00775564: mov      r0, #0
00775568: str      r0, [r4, #0xc8]
0077556c: str      r0, [r4, #0xcc]
00775570: b        #0x7754e8

# _ZN7gameswf4root13begin_displayEv
007751bc: push     {r4, r5, r6, r7, r8, sl, lr}
007751c0: ldr      r3, [r0, #0xc]
007751c4: sub      sp, sp, #0x34
007751c8: ldr      r5, [pc, #0x12c]
007751cc: ldr      ip, [r3, #0xbc]
007751d0: ldr      r2, [r3, #0xb4]
007751d4: ldr      r6, [pc, #0x124]
007751d8: str      ip, [sp, #0x28]
007751dc: str      r2, [sp, #0x24]
007751e0: ldr      r2, [r3, #0xb8]
007751e4: ldr      r3, [r3, #0xc0]
007751e8: mov      r4, r0
007751ec: add      r1, sp, #0x24
007751f0: str      r3, [sp, #0x20]
007751f4: add      r5, pc, r5
007751f8: str      r2, [sp, #0x1c]
007751fc: bl       #0x773f50
00775200: mov      r0, r4
00775204: add      r1, sp, #0x1c
00775208: bl       #0x773f50
0077520c: ldr      r3, [r5, r6]
00775210: ldr      r3, [r3]
00775214: cmp      r3, #0
00775218: beq      #0x775230
0077521c: mov      r0, r3
00775220: mov      r1, #0
00775224: ldr      r3, [r3]
00775228: mov      lr, pc
0077522c: ldr      pc, [r3, #0x30]
00775230: ldr      r3, [r4, #0xcc]
00775234: cmp      r3, #0
00775238: beq      #0x77524c
0077523c: ldr      r0, [r4, #0xc8]
00775240: ldrb     r2, [r0, #4]
00775244: cmp      r2, #0
00775248: beq      #0x7752d4
0077524c: ldr      r5, [r5, r6]
00775250: ldr      r1, [r3, #0xac]
00775254: ldr      r3, [r5]
00775258: cmp      r3, #0
0077525c: beq      #0x7752cc
00775260: mov      r0, r3
00775264: ldr      r3, [r3]
00775268: mov      lr, pc
0077526c: ldr      pc, [r3, #0x48]
00775270: ldr      ip, [r5]
00775274: ldr      r1, [r4, #0x38]
00775278: ldr      r2, [r4, #0x14]
0077527c: cmp      ip, #0
00775280: str      r1, [sp, #0x2c]
00775284: ldr      r3, [r4, #0x18]
00775288: ldr      r6, [r4, #0x1c]
0077528c: ldr      r5, [r4, #0x20]
00775290: ldr      r7, [sp, #0x1c]
00775294: ldr      r4, [sp, #0x24]
00775298: ldr      r8, [sp, #0x28]
0077529c: ldr      sl, [sp, #0x20]
007752a0: beq      #0x7752cc
007752a4: mov      r0, ip
007752a8: ldr      ip, [ip]
007752ac: str      r6, [sp]
007752b0: str      r5, [sp, #4]
007752b4: str      r4, [sp, #8]
007752b8: str      r7, [sp, #0xc]
007752bc: str      r8, [sp, #0x10]
007752c0: str      sl, [sp, #0x14]
007752c4: mov      lr, pc
007752c8: ldr      pc, [ip, #0x28]
007752cc: add      sp, sp, #0x34
007752d0: pop      {r4, r5, r6, r7, r8, sl, pc}
007752d4: ldr      r1, [r0]
007752d8: sub      r1, r1, #1
007752dc: cmp      r1, #0
007752e0: str      r1, [r0]
007752e4: bne      #0x7752ec
007752e8: bl       #0x752b38
007752ec: mov      r3, #0
007752f0: str      r3, [r4, #0xc8]
007752f4: str      r3, [r4, #0xcc]
007752f8: b        #0x77524c
007752fc: mlaeq    r1, ip, r8, pc
00775300: strheq   r3, [r0], -r4

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
