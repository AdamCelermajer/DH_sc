
# _ZN21render_handler_glitch22begin_display_callbackEv
007d6a40: add      r0, r0, #0x1f0
007d6a44: b        #0x7d6894

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

# _ZN21render_handler_glitch11end_displayEv
007d6de4: push     {r4, lr}
007d6de8: mov      r4, r0
007d6dec: add      r0, r0, #0x1f0
007d6df0: bl       #0x7d6894
007d6df4: ldr      r3, [r4, #0x10]
007d6df8: add      r2, r4, #0x38
007d6dfc: mov      r1, #2
007d6e00: mov      r0, r3
007d6e04: ldr      r3, [r3]
007d6e08: mov      lr, pc
007d6e0c: ldr      pc, [r3, #0x6c]
007d6e10: ldr      r3, [r4, #0x10]
007d6e14: add      r2, r4, #0x7c
007d6e18: mov      r1, #0
007d6e1c: mov      r0, r3
007d6e20: ldr      r3, [r3]
007d6e24: mov      lr, pc
007d6e28: ldr      pc, [r3, #0x6c]
007d6e2c: ldr      r3, [r4, #0x10]
007d6e30: add      r2, r4, #0xc0
007d6e34: mov      r1, #1
007d6e38: mov      r0, r3
007d6e3c: ldr      r3, [r3]
007d6e40: mov      lr, pc
007d6e44: ldr      pc, [r3, #0x6c]
007d6e48: ldr      r3, [r4, #0x10]
007d6e4c: add      r1, r4, #0x104
007d6e50: ldr      r3, [r3, #0xcc]
007d6e54: ldr      r3, [r3, #-4]
007d6e58: mov      r0, r3
007d6e5c: ldr      r3, [r3]
007d6e60: mov      lr, pc
007d6e64: ldr      pc, [r3, #0xc]
007d6e68: ldr      r3, [r4, #0x10]
007d6e6c: mov      r1, #0x100
007d6e70: ldrb     r2, [r4, #0x301]
007d6e74: mov      r0, r3
007d6e78: ldr      r3, [r3]
007d6e7c: mov      lr, pc
007d6e80: ldr      pc, [r3, #0xa0]
007d6e84: ldr      r3, [r4, #0x10]
007d6e88: add      r1, r4, #0x14
007d6e8c: mov      r0, r3
007d6e90: ldr      r3, [r3]
007d6e94: mov      lr, pc
007d6e98: ldr      pc, [r3, #0x1e4]
007d6e9c: pop      {r4, pc}

# _ZN8RenderFX12BeginDisplayEv
007a9afc: push     {r4, lr}
007a9b00: ldr      r0, [r0, #0x38]
007a9b04: bl       #0x76d5b4
007a9b08: subs     r4, r0, #0
007a9b0c: beq      #0x7a9b28
007a9b10: bl       #0x759c64
007a9b14: mov      r0, r4
007a9b18: bl       #0x7751bc
007a9b1c: mov      r0, r4
007a9b20: pop      {r4, lr}
007a9b24: b        #0x75a240
007a9b28: pop      {r4, lr}
007a9b2c: b        #0x7751bc
