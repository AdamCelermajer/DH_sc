
# _ZN10AnimatedFX13ForceMaterialEi
004928c8: ldr      r3, [pc, #0x68]
004928cc: ldr      r2, [pc, #0x68]
004928d0: str      lr, [sp, #-4]!
004928d4: add      r3, pc, r3
004928d8: ldr      r2, [r3, r2]
004928dc: sub      sp, sp, #0xc
004928e0: ldr      r2, [r2]
004928e4: cmp      r2, #2
004928e8: moveq    r3, #0
004928ec: streq    r3, [r3]
004928f0: beq      #0x4928fc
004928f4: cmp      r2, #1
004928f8: beq      #0x492904
004928fc: add      sp, sp, #0xc
00492900: ldm      sp!, {pc}
00492904: ldr      r0, [pc, #0x34]
00492908: ldr      r1, [pc, #0x34]
0049290c: ldr      r2, [pc, #0x34]
00492910: ldr      r0, [r3, r0]
00492914: ldr      r3, [pc, #0x30]
00492918: mov      ip, #0x11c
0049291c: add      r1, pc, r1
00492920: add      r2, pc, r2
00492924: add      r3, pc, r3
00492928: add      r0, r0, #0xa8
0049292c: str      ip, [sp]
00492930: bl       #0x30e004
00492934: b        #0x4928fc
00492938: ldrheq   r2, [r0], #-0x1c
0049293c: andeq    r3, r0, r0, asr #19
00492940: andeq    r1, r0, r0, asr #19
00492944: strheq   fp, [r2], #-0xac
00492948: subeq    fp, r2, r8, asr #24
0049294c: subeq    r2, r4, r4, asr #13

# _ZN10AnimatedFXD2Ev
00492408: push     {r4, lr}
0049240c: ldr      r3, [pc, #0x40]
00492410: ldr      r2, [pc, #0x40]
00492414: ldr      r1, [r0, #0x2c]
00492418: add      r3, pc, r3
0049241c: ldr      r2, [r3, r2]
00492420: cmp      r1, #0
00492424: mov      r4, r0
00492428: add      r2, r2, #8
0049242c: str      r2, [r0]
00492430: beq      #0x49244c
00492434: ldr      r3, [r1]
00492438: mov      r0, r1
0049243c: mov      lr, pc
00492440: ldr      pc, [r3, #4]
00492444: mov      r3, #0
00492448: str      r3, [r4, #0x2c]
0049244c: mov      r0, r4
00492450: pop      {r4, pc}
00492454: subseq   r2, r0, r8, ror r6
00492458: andeq    r3, r0, ip, lsl r1

# _ZN15VisualFXManager10_GetAnimFXEi
00494ad4: push     {r4, r5, r6, r7, r8, sl, lr}
00494ad8: ldr      sl, [pc, #0x118]
00494adc: subs     r7, r1, #0
00494ae0: sub      sp, sp, #0xc
00494ae4: add      sl, pc, sl
00494ae8: bge      #0x494afc
00494aec: mov      r5, #0
00494af0: mov      r0, r5
00494af4: add      sp, sp, #0xc
00494af8: pop      {r4, r5, r6, r7, r8, sl, pc}
00494afc: ldr      r3, [pc, #0xf8]
00494b00: ldr      r3, [sl, r3]
00494b04: ldr      r3, [r3]
00494b08: cmp      r7, r3
00494b0c: bge      #0x494aec
00494b10: ldr      r3, [r0, #0x28]
00494b14: mov      r8, #0x18
00494b18: mla      r8, r8, r7, r3
00494b1c: ldr      r2, [r8, #8]
00494b20: ldr      r3, [r8, #4]
00494b24: rsb      r3, r3, r2
00494b28: asrs     r3, r3, #2
00494b2c: bne      #0x494bd0
00494b30: mov      r6, r8
00494b34: ldr      r4, [r6, #0x10]!
00494b38: cmp      r4, r6
00494b3c: beq      #0x494b58
00494b40: ldr      r4, [r4]
00494b44: add      r3, r3, #1
00494b48: cmp      r6, r4
00494b4c: bne      #0x494b40
00494b50: cmp      r3, #5
00494b54: bhi      #0x494aec
00494b58: mov      r1, #0
00494b5c: mov      r0, #0x54
00494b60: bl       #0x310570
00494b64: mov      r1, r7
00494b68: mov      r5, r0
00494b6c: bl       #0x492374
00494b70: ldr      r3, [pc, #0x88]
00494b74: mov      r0, #0xc
00494b78: ldr      r2, [pc, #0x84]
00494b7c: ldr      r1, [sl, r3]
00494b80: mvn      ip, #0
00494b84: mov      r3, #0x3f800000
00494b88: ldr      r1, [r1]
00494b8c: add      r2, pc, r2
00494b90: mla      r7, r0, r7, r1
00494b94: mov      r0, r5
00494b98: ldr      r1, [r7, #8]
00494b9c: str      ip, [sp]
00494ba0: mov      ip, #1
00494ba4: str      ip, [sp, #4]
00494ba8: bl       #0x49296c
00494bac: mov      r0, r6
00494bb0: bl       #0x494ab4
00494bb4: str      r5, [r0, #8]
00494bb8: ldr      r3, [r8, #0x14]
00494bbc: str      r4, [r0]
00494bc0: str      r3, [r0, #4]
00494bc4: str      r0, [r3]
00494bc8: str      r0, [r8, #0x14]
00494bcc: b        #0x494af0
00494bd0: ldr      r5, [r2, #-4]
00494bd4: mov      r1, #0x3f800000
00494bd8: add      r4, r8, #0x10
00494bdc: ldr      r0, [r5, #0x2c]
00494be0: bl       #0x472708
00494be4: ldr      r3, [r8, #8]
00494be8: mov      r0, r4
00494bec: sub      r3, r3, #4
00494bf0: str      r3, [r8, #8]
00494bf4: b        #0x494bb0
00494bf8: subeq    pc, pc, ip, lsr #31
00494bfc: andeq    r0, r0, r8, lsl #23
00494c00: strheq   r1, [r0], -r8
00494c04: subeq    r6, r3, ip, ror ip

# _ZN15VisualFXManager14DropAnimatedFXERP10AnimatedFX
00494978: push     {r4, r5, r6, r7, r8, lr}
0049497c: ldr      r3, [r1]
00494980: ldr      r6, [pc, #0x124]
00494984: mov      r5, r1
00494988: cmp      r3, #0
0049498c: add      r6, pc, r6
00494990: beq      #0x4949e4
00494994: ldrb     r2, [r0, #4]
00494998: cmp      r2, #0
0049499c: beq      #0x4949e8
004949a0: ldr      r3, [r3, #8]
004949a4: cmp      r3, #0
004949a8: blt      #0x4949d8
004949ac: ldr      r2, [r0, #0x2c]
004949b0: ldr      r0, [r0, #0x28]
004949b4: rsb      r2, r0, r2
004949b8: asr      r2, r2, #3
004949bc: add      r1, r2, r2, lsl #2
004949c0: add      r1, r1, r1, lsl #4
004949c4: add      r1, r1, r1, lsl #8
004949c8: add      r1, r1, r1, lsl #16
004949cc: add      r2, r2, r1, lsl #1
004949d0: cmp      r3, r2
004949d4: blo      #0x4949f0
004949d8: mov      r3, #0
004949dc: str      r3, [r5]
004949e0: pop      {r4, r5, r6, r7, r8, pc}
004949e4: pop      {r4, r5, r6, r7, r8, pc}
004949e8: str      r2, [r1]
004949ec: pop      {r4, r5, r6, r7, r8, pc}
004949f0: mov      r8, #0x18
004949f4: mla      r8, r8, r3, r0
004949f8: mov      r7, r8
004949fc: ldr      r0, [r7, #0x10]!
00494a00: cmp      r7, r0
00494a04: beq      #0x494a28
00494a08: ldr      r3, [r0, #8]
00494a0c: ldr      r2, [r5]
00494a10: ldr      r4, [r0]
00494a14: cmp      r2, r3
00494a18: beq      #0x494a90
00494a1c: mov      r0, r4
00494a20: cmp      r7, r0
00494a24: bne      #0x494a08
00494a28: add      r0, r8, #4
00494a2c: mov      r1, r5
00494a30: bl       #0x494550
00494a34: ldr      r3, [r5]
00494a38: mov      r4, #0
00494a3c: mov      r1, #1
00494a40: mov      r0, r3
00494a44: str      r4, [r3, #0x28]
00494a48: bl       #0x492aa0
00494a4c: ldr      r2, [pc, #0x5c]
00494a50: ldr      r3, [r5]
00494a54: mov      r1, r4
00494a58: ldr      r2, [r6, r2]
00494a5c: mov      r0, r3
00494a60: ldr      lr, [r2]
00494a64: ldr      ip, [r2, #4]
00494a68: ldr      r2, [r2, #8]
00494a6c: str      lr, [r3, #0x34]
00494a70: str      ip, [r3, #0x38]
00494a74: str      r2, [r3, #0x3c]
00494a78: bl       #0x492aa0
00494a7c: ldr      r0, [r5]
00494a80: mov      r1, r4
00494a84: bl       #0x492ef0
00494a88: str      r4, [r5]
00494a8c: pop      {r4, r5, r6, r7, r8, pc}
00494a90: ldr      r3, [r0, #4]
00494a94: mov      r1, #0xc
00494a98: str      r4, [r3]
00494a9c: str      r3, [r4, #4]
00494aa0: bl       #0x708f00
00494aa4: mov      r0, r4
00494aa8: b        #0x494a20
00494aac: subseq   r0, r0, r4, lsl #2
00494ab0: andeq    r3, r0, ip, lsr #30

# _ZN10AnimatedFXC2Ei
004922e0: push     {r4, r5}
004922e4: ldr      r4, [pc, #0x80]
004922e8: ldr      r2, [pc, #0x80]
004922ec: str      r1, [r0, #8]
004922f0: add      r4, pc, r4
004922f4: ldr      r2, [r4, r2]
004922f8: mvn      r1, #0
004922fc: str      r1, [r0, #0x1c]
00492300: mov      r1, #0x3f800000
00492304: mov      ip, #0
00492308: add      r5, r2, #8
0049230c: str      r1, [r0, #0x20]
00492310: mov      r2, #0
00492314: mov      r1, #1
00492318: str      r2, [r0, #0x50]
0049231c: str      r5, [r0]
00492320: strb     r1, [r0, #0x24]
00492324: str      ip, [r0, #0x48]
00492328: str      r2, [r0, #0xc]
0049232c: str      r2, [r0, #0x10]
00492330: str      r2, [r0, #0x14]
00492334: strb     r2, [r0, #0x18]
00492338: str      r2, [r0, #0x28]
0049233c: str      r2, [r0, #0x2c]
00492340: strb     r2, [r0, #0x30]
00492344: strb     r2, [r0, #0x31]
00492348: strb     r2, [r0, #0x32]
0049234c: str      ip, [r0, #0x34]
00492350: str      ip, [r0, #0x38]
00492354: str      ip, [r0, #0x3c]
00492358: str      ip, [r0, #0x40]
0049235c: str      ip, [r0, #0x44]
00492360: strb     r2, [r0, #0x4c]
00492364: pop      {r4, r5}
00492368: bx       lr
0049236c: subseq   r2, r0, r0, lsr #15
00492370: andeq    r3, r0, ip, lsl r1

# _ZN10AnimatedFX8SetScaleERK7Point3DIfE
004928ac: ldr      r3, [r0, #0x2c]
004928b0: cmp      r3, #0
004928b4: bxeq     lr
004928b8: mov      r2, #0
004928bc: strb     r2, [r0, #0x32]
004928c0: mov      r0, r3
004928c4: b        #0x4727ac

# _ZN15VisualFXManager21DropAnimatedFXSetByIDEi
00494660: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00494664: ldr      r4, [pc, #0x180]
00494668: subs     r6, r1, #0
0049466c: sub      sp, sp, #0x6c
00494670: mov      r5, r0
00494674: add      r4, pc, r4
00494678: blt      #0x4947e4
0049467c: ldr      r3, [pc, #0x16c]
00494680: ldr      r3, [r4, r3]
00494684: ldr      r3, [r3]
00494688: cmp      r6, r3
0049468c: bge      #0x4947e4
00494690: ldr      fp, [pc, #0x15c]
00494694: mov      sb, #0x18
00494698: ldr      r7, [r0, #0x1c]
0049469c: ldr      lr, [r4, fp]
004946a0: mul      r6, sb, r6
004946a4: ldr      r8, [lr, #8]
004946a8: ldr      r2, [r7, r6]
004946ac: mov      ip, #0
004946b0: mov      r3, ip
004946b4: ldr      r2, [r2, #8]
004946b8: str      r8, [sp, #0x60]
004946bc: ldr      r8, [lr]
004946c0: add      sl, sp, #0x34
004946c4: str      r8, [sp, #0x58]
004946c8: ldr      r8, [lr, #4]
004946cc: add      lr, r7, r6
004946d0: str      lr, [sp, #0x14]
004946d4: ldr      lr, [sp, #0x58]
004946d8: str      ip, [sp]
004946dc: str      ip, [sp, #4]
004946e0: str      lr, [sp, #0x4c]
004946e4: ldr      lr, [sp, #0x60]
004946e8: str      r8, [sp, #0x50]
004946ec: str      r8, [sp, #0x5c]
004946f0: str      lr, [sp, #0x54]
004946f4: add      lr, sp, #0x58
004946f8: str      lr, [sp, #8]
004946fc: add      lr, sp, #0x4c
00494700: str      lr, [sp, #0xc]
00494704: bl       #0x4935b8
00494708: ldr      r1, [sp, #0x14]
0049470c: mov      r8, r0
00494710: mov      r0, sl
00494714: bl       #0x493924
00494718: mov      r1, r5
0049471c: mov      r2, sl
00494720: mov      r3, r8
00494724: add      r0, sp, sb
00494728: bl       #0x4933e4
0049472c: mov      r0, sl
00494730: bl       #0x4940d8
00494734: ldr      r3, [r7, r6]
00494738: ldr      r2, [r8, #4]
0049473c: mov      ip, #0x30
00494740: ldr      r1, [r3, #0x10]
00494744: ldr      r3, [r5, #0x28]
00494748: mov      r0, r8
0049474c: mla      r2, ip, r2, r1
00494750: ldr      r2, [r2, #4]
00494754: mla      sb, sb, r2, r3
00494758: bl       #0x310440
0049475c: mov      r5, sb
00494760: ldr      r3, [r5, #0x10]!
00494764: cmp      r3, r5
00494768: beq      #0x4947e4
0049476c: mov      r2, r3
00494770: ldr      r2, [r2]
00494774: cmp      r5, r2
00494778: bne      #0x494770
0049477c: ldr      r3, [r3, #8]
00494780: add      r1, sp, #0x68
00494784: add      r0, sb, #4
00494788: str      r3, [r1, #-4]!
0049478c: bl       #0x494550
00494790: mov      r0, r5
00494794: bl       #0x493c04
00494798: ldr      r3, [r4, fp]
0049479c: ldr      r0, [sp, #0x64]
004947a0: mov      r1, #0
004947a4: ldr      ip, [r3]
004947a8: ldr      r2, [r3, #4]
004947ac: ldr      r3, [r3, #8]
004947b0: str      ip, [r0, #0x34]
004947b4: str      r2, [r0, #0x38]
004947b8: str      r3, [r0, #0x3c]
004947bc: bl       #0x492aa0
004947c0: ldr      r3, [sp, #0x64]
004947c4: mov      r4, #0
004947c8: mov      r1, #1
004947cc: mov      r0, r3
004947d0: str      r4, [r3, #0x28]
004947d4: bl       #0x492aa0
004947d8: mov      r1, r4
004947dc: ldr      r0, [sp, #0x64]
004947e0: bl       #0x492ef0
004947e4: add      sp, sp, #0x6c
004947e8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004947ec: subseq   r0, r0, ip, lsl r4
004947f0: andeq    r0, r0, r4, asr #13
004947f4: andeq    r3, r0, ip, lsr #30

# _ZN15VisualFXManager13GetAnimFXDataENS_13AnimFXSetInfoEPNS_13AnimFXSetDataE
004933e4: str      r4, [sp, #-4]!
004933e8: ldr      r1, [r2]
004933ec: ldr      ip, [r3, #4]
004933f0: mov      r4, #0x30
004933f4: ldr      r1, [r1, #0x10]
004933f8: mla      r1, r4, ip, r1
004933fc: ldrb     ip, [r1, #0x11]
00493400: strb     ip, [r0]
00493404: ldrb     ip, [r1, #0x10]
00493408: strb     ip, [r0, #1]
0049340c: ldrb     ip, [r1, #0x20]
00493410: strb     ip, [r0, #2]
00493414: ldr      ip, [r1, #0x24]
00493418: str      r3, [r0, #0x10]
0049341c: str      ip, [r0, #4]
00493420: ldr      ip, [r1, #0x14]
00493424: str      ip, [r0, #0xc]
00493428: ldr      r2, [r2]
0049342c: ldr      r2, [r2, #0x14]
00493430: cmp      r2, #1
00493434: beq      #0x49347c
00493438: ldr      r2, [r1, #0xc]
0049343c: cmn      r2, #1
00493440: beq      #0x493470
00493444: ldr      r3, [r3, #0xc]
00493448: cmn      r3, #1
0049344c: beq      #0x493470
00493450: cmp      r2, #0
00493454: streq    r3, [r0, #8]
00493458: beq      #0x493468
0049345c: cmp      r3, #0
00493460: mulne    r2, r2, r3
00493464: str      r2, [r0, #8]
00493468: ldm      sp!, {r4}
0049346c: bx       lr
00493470: mvn      r3, #0
00493474: str      r3, [r0, #8]
00493478: b        #0x493468
0049347c: ldr      r3, [r1, #0xc]
00493480: str      r3, [r0, #8]
00493484: b        #0x493468

# _ZN15VisualFXManager13PlayAnimFXSetEiRK7Point3DIfEPK10GameObjectPNS_13AnimFXSetDataE
00495d14: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495d18: ldr      r7, [pc, #0x1d8]
00495d1c: cmp      r1, #0
00495d20: sub      sp, sp, #0x74
00495d24: add      r7, pc, r7
00495d28: mov      r5, r0
00495d2c: mov      r4, r2
00495d30: mov      fp, r3
00495d34: blt      #0x495e98
00495d38: ldr      r3, [pc, #0x1bc]
00495d3c: ldr      r3, [r7, r3]
00495d40: ldr      r3, [r3]
00495d44: cmp      r1, r3
00495d48: bge      #0x495e98
00495d4c: mov      sb, #0x18
00495d50: mul      sb, sb, r1
00495d54: ldr      sl, [r0, #0x1c]
00495d58: ldr      r2, [sl, sb]
00495d5c: add      r6, sl, sb
00495d60: ldr      r3, [r2, #0x14]
00495d64: cmp      r3, #2
00495d68: beq      #0x495ecc
00495d6c: mov      r0, #0
00495d70: str      r0, [sp, #0x18]
00495d74: str      r0, [sp, #0x24]
00495d78: mov      r3, r0
00495d7c: ldr      r0, [pc, #0x17c]
00495d80: ldr      ip, [r4, #4]
00495d84: ldr      r2, [r2, #8]
00495d88: ldr      r0, [r7, r0]
00495d8c: ldr      r7, [r4, #8]
00495d90: ldr      lr, [r0, #8]
00495d94: ldr      r8, [r0]
00495d98: ldr      r0, [r0, #4]
00495d9c: str      r7, [sp, #0x14]
00495da0: str      lr, [sp, #0x20]
00495da4: str      r0, [sp, #0x1c]
00495da8: ldr      lr, [r4]
00495dac: str      ip, [sp, #0x68]
00495db0: ldr      ip, [sp, #0x14]
00495db4: str      lr, [sp, #0x64]
00495db8: ldr      lr, [sp, #0x1c]
00495dbc: str      ip, [sp, #0x6c]
00495dc0: ldr      ip, [sp, #0x20]
00495dc4: mov      r0, r5
00495dc8: str      r8, [sp, #0x58]
00495dcc: str      ip, [sp, #0x60]
00495dd0: ldr      ip, [sp, #0x98]
00495dd4: str      lr, [sp, #0x5c]
00495dd8: stm      sp, {fp, ip}
00495ddc: add      ip, sp, #0x64
00495de0: str      ip, [sp, #8]
00495de4: add      ip, sp, #0x58
00495de8: str      ip, [sp, #0xc]
00495dec: bl       #0x4935b8
00495df0: add      r7, sp, #0x2c
00495df4: add      lr, sp, #0x44
00495df8: mov      r8, r0
00495dfc: mov      r1, r6
00495e00: mov      r0, r7
00495e04: str      lr, [sp, #0x14]
00495e08: bl       #0x493924
00495e0c: mov      r2, r7
00495e10: mov      r3, r8
00495e14: mov      r1, r5
00495e18: ldr      r0, [sp, #0x14]
00495e1c: bl       #0x4933e4
00495e20: mov      r0, r7
00495e24: add      r7, r6, #0x10
00495e28: bl       #0x4940d8
00495e2c: mov      r0, r7
00495e30: bl       #0x493814
00495e34: str      r8, [r0, #8]
00495e38: ldr      r3, [r6, #0x14]
00495e3c: str      r7, [r0]
00495e40: str      r3, [r0, #4]
00495e44: str      r0, [r3]
00495e48: str      r0, [r6, #0x14]
00495e4c: ldr      r3, [sl, sb]
00495e50: ldr      r0, [sp, #0x18]
00495e54: ldr      r2, [r3, #0x10]
00495e58: add      r3, r2, r0
00495e5c: ldr      r3, [r3, #4]
00495e60: cmn      r3, #1
00495e64: beq      #0x495ea0
00495e68: ldr      r1, [sp, #0x24]
00495e6c: ldr      r3, [r6, #4]
00495e70: ldr      r3, [r3, r1, lsl #2]
00495e74: ldrb     r1, [r3]
00495e78: cmp      r1, #0
00495e7c: beq      #0x495ea0
00495e80: ldr      r1, [r3, #4]
00495e84: mov      r0, r5
00495e88: mov      r2, r4
00495e8c: mov      r3, fp
00495e90: str      r8, [sp]
00495e94: bl       #0x495d14
00495e98: add      sp, sp, #0x74
00495e9c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495ea0: ldr      r3, [r8, #4]
00495ea4: mov      r1, #0x30
00495ea8: ldr      r7, [sp, #0x14]
00495eac: mla      r3, r1, r3, r2
00495eb0: mov      r0, r5
00495eb4: ldr      r1, [r3, #4]
00495eb8: mov      r2, r4
00495ebc: mov      r3, fp
00495ec0: str      r7, [sp]
00495ec4: bl       #0x495b54
00495ec8: b        #0x495e98
00495ecc: ldr      r0, [r2, #0xc]
00495ed0: str      r1, [sp, #0x10]
00495ed4: bl       #0x493780
00495ed8: mov      r2, #0x30
00495edc: mul      r2, r2, r0
00495ee0: mov      r3, r0
00495ee4: str      r2, [sp, #0x18]
00495ee8: ldr      r2, [sl, sb]
00495eec: ldr      r1, [sp, #0x10]
00495ef0: str      r0, [sp, #0x24]
00495ef4: b        #0x495d7c
00495ef8: subeq    lr, pc, ip, ror #26
00495efc: andeq    r0, r0, r4, asr #13
00495f00: andeq    r3, r0, ip, lsr #30

# _ZN15VisualFXManager10GrabAnimFXEiP10GameObject
00495430: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495434: ldr      r4, [pc, #0x260]
00495438: ldr      r5, [pc, #0x260]
0049543c: ldr      ip, [pc, #0x260]
00495440: add      r4, pc, r4
00495444: ldr      r3, [r4, r5]
00495448: ldr      sl, [r4, ip]
0049544c: sub      sp, sp, #0x84
00495450: ldr      r3, [r3]
00495454: mov      r8, r0
00495458: mov      r0, sl
0049545c: str      r3, [sp, #0x7c]
00495460: mov      r7, r1
00495464: mov      sb, r2
00495468: bl       #0x337888
0049546c: ldr      r1, [pc, #0x234]
00495470: add      r6, sp, #0x64
00495474: add      r2, sp, #0x60
00495478: add      r1, pc, r1
0049547c: mov      r0, r6
00495480: bl       #0x3140ec
00495484: mov      r0, sl
00495488: mov      r1, r6
0049548c: bl       #0x337ec8
00495490: mov      sl, r0
00495494: ldr      r0, [sp, #0x78]
00495498: cmp      r0, r6
0049549c: beq      #0x4954bc
004954a0: cmp      r0, #0
004954a4: beq      #0x4954bc
004954a8: ldr      r1, [sp, #0x64]
004954ac: rsb      r1, r0, r1
004954b0: cmp      r1, #0x80
004954b4: bhi      #0x4954e8
004954b8: bl       #0x708f00
004954bc: cmp      sl, #0
004954c0: bne      #0x4954f4
004954c4: mov      r6, #0
004954c8: ldr      r3, [r4, r5]
004954cc: ldr      r2, [sp, #0x7c]
004954d0: mov      r0, r6
004954d4: ldr      r3, [r3]
004954d8: cmp      r2, r3
004954dc: bne      #0x495698
004954e0: add      sp, sp, #0x84
004954e4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004954e8: bl       #0x310440
004954ec: cmp      sl, #0
004954f0: beq      #0x4954c4
004954f4: cmp      r7, #0
004954f8: blt      #0x4954c4
004954fc: ldr      r3, [pc, #0x1a8]
00495500: ldr      r3, [r4, r3]
00495504: ldr      r3, [r3]
00495508: cmp      r7, r3
0049550c: bge      #0x4954c4
00495510: mov      sl, #0x18
00495514: ldr      fp, [r8, #0x1c]
00495518: mul      sl, sl, r7
0049551c: mov      r0, r8
00495520: ldr      r3, [fp, sl]
00495524: add      r1, fp, sl
00495528: str      r1, [sp, #0x14]
0049552c: ldr      r3, [r3, #0x10]
00495530: ldr      r1, [r3, #4]
00495534: bl       #0x494ad4
00495538: subs     r6, r0, #0
0049553c: beq      #0x4954c8
00495540: ldr      r2, [fp, sl]
00495544: ldr      r3, [r2, #0x14]
00495548: cmp      r3, #2
0049554c: movne    r3, #0
00495550: beq      #0x495684
00495554: ldr      r1, [pc, #0x154]
00495558: mov      ip, #0
0049555c: ldr      r2, [r2, #8]
00495560: ldr      r0, [r4, r1]
00495564: mov      r1, r7
00495568: add      r7, sp, #0x1c
0049556c: ldr      lr, [r0, #4]
00495570: ldr      fp, [r0, #8]
00495574: ldr      sl, [r0]
00495578: str      ip, [sp, #4]
0049557c: add      ip, sp, #0x54
00495580: str      ip, [sp, #8]
00495584: mov      r0, r8
00495588: add      ip, sp, #0x48
0049558c: str      lr, [sp, #0x4c]
00495590: str      ip, [sp, #0xc]
00495594: str      lr, [sp, #0x58]
00495598: str      sl, [sp, #0x48]
0049559c: str      sl, [sp, #0x54]
004955a0: str      fp, [sp, #0x50]
004955a4: str      fp, [sp, #0x5c]
004955a8: str      sb, [sp]
004955ac: bl       #0x4935b8
004955b0: ldr      r1, [sp, #0x14]
004955b4: mov      sl, r0
004955b8: mov      r0, r7
004955bc: bl       #0x493924
004955c0: mov      r1, r8
004955c4: mov      r3, sl
004955c8: mov      r2, r7
004955cc: add      r0, sp, #0x34
004955d0: bl       #0x4933e4
004955d4: ldr      r2, [sp, #0x14]
004955d8: mov      r0, r7
004955dc: add      r7, r2, #0x10
004955e0: bl       #0x4940d8
004955e4: mov      r0, r7
004955e8: bl       #0x493814
004955ec: str      sl, [r0, #8]
004955f0: ldr      r1, [sp, #0x14]
004955f4: mov      r3, r0
004955f8: ldr      r2, [r1, #0x14]
004955fc: str      r7, [r0]
00495600: mov      r0, r6
00495604: str      r2, [r3, #4]
00495608: str      r3, [r2]
0049560c: str      r3, [r1, #0x14]
00495610: str      sb, [r6, #0x28]
00495614: mov      r1, #1
00495618: bl       #0x492aa0
0049561c: mov      r0, r6
00495620: mov      r1, #1
00495624: bl       #0x492aa0
00495628: mov      r0, r6
0049562c: mov      r1, #1
00495630: bl       #0x492694
00495634: mov      r0, r6
00495638: bl       #0x492744
0049563c: ldr      lr, [sp, #0x38]
00495640: ldr      r0, [pc, #0x6c]
00495644: ldrb     r1, [sp, #0x34]
00495648: str      lr, [sp]
0049564c: mvn      lr, #0
00495650: ldr      ip, [r4, r0]
00495654: str      lr, [sp, #4]
00495658: ldr      lr, [sp, #0x44]
0049565c: mov      r0, r6
00495660: ldrb     r2, [sp, #0x35]
00495664: ldrb     r3, [sp, #0x36]
00495668: str      lr, [sp, #8]
0049566c: str      ip, [sp, #0xc]
00495670: bl       #0x492e8c
00495674: mov      r0, r6
00495678: mov      r1, #1
0049567c: bl       #0x492ef0
00495680: b        #0x4954c8
00495684: ldr      r0, [r2, #0xc]
00495688: bl       #0x493780
0049568c: ldr      r2, [fp, sl]
00495690: mov      r3, r0
00495694: b        #0x495554
00495698: bl       #0x30e310
0049569c: subeq    pc, pc, r0, asr r6
004956a0: andeq    r4, r0, ip, lsr #1
004956a4: andeq    r0, r0, r4, lsl #17

# _ZN10AnimatedFX8SetStartEv
00492744: push     {r4, r5, r6, lr}
00492748: ldr      r3, [r0, #0x2c]
0049274c: mov      r4, r0
00492750: cmp      r3, #0
00492754: beq      #0x4927a0
00492758: bl       #0x49267c
0049275c: cmp      r0, #0
00492760: beq      #0x4927a0
00492764: mov      r0, r4
00492768: bl       #0x49267c
0049276c: ldr      r3, [r0]
00492770: mov      lr, pc
00492774: ldr      pc, [r3, #0x44]
00492778: ldr      r5, [r0, #0x10]
0049277c: mov      r0, r4
00492780: bl       #0x49267c
00492784: ldr      r3, [r0]
00492788: mov      lr, pc
0049278c: ldr      pc, [r3, #0x44]
00492790: mov      r1, r5
00492794: ldr      r3, [r0]
00492798: mov      lr, pc
0049279c: ldr      pc, [r3, #0xc]
004927a0: pop      {r4, r5, r6, pc}

# _ZN15VisualFXManager13PlayAnimFXSetEiPK10GameObjectPNS_13AnimFXSetDataE
00495f04: str      lr, [sp, #-4]!
00495f08: ldr      ip, [pc, #0x24]
00495f0c: mov      lr, r2
00495f10: ldr      r2, [pc, #0x20]
00495f14: sub      sp, sp, #0xc
00495f18: add      ip, pc, ip
00495f1c: str      r3, [sp]
00495f20: ldr      r2, [ip, r2]
00495f24: mov      r3, lr
00495f28: bl       #0x495d14
00495f2c: add      sp, sp, #0xc
00495f30: ldm      sp!, {pc}
00495f34: subeq    lr, pc, r8, ror fp
00495f38: andeq    r3, r0, ip, lsr #30

# _ZN15VisualFXManager10PlayAnimFXEiPK10GameObjectPNS_10AnimFXDataE
00495f3c: str      lr, [sp, #-4]!
00495f40: ldr      ip, [pc, #0x24]
00495f44: mov      lr, r2
00495f48: ldr      r2, [pc, #0x20]
00495f4c: sub      sp, sp, #0xc
00495f50: add      ip, pc, ip
00495f54: str      r3, [sp]
00495f58: ldr      r2, [ip, r2]
00495f5c: mov      r3, lr
00495f60: bl       #0x495b54
00495f64: add      sp, sp, #0xc
00495f68: ldm      sp!, {pc}
00495f6c: subeq    lr, pc, r0, asr #22
00495f70: andeq    r3, r0, ip, lsr #30

# _ZN15VisualFXManager18_HandleEndOfLoopCBEP10AnimatedFXPNS_13AnimFXSetDataE
00496364: push     {r4, r5, r6}
00496368: ldr      r5, [r2, #8]
0049636c: mov      r4, #0x18
00496370: ldr      ip, [r0, #0x1c]
00496374: mul      r5, r4, r5
00496378: ldr      r5, [ip, r5]
0049637c: ldr      r5, [r5, #0x14]
00496380: cmp      r5, #1
00496384: beq      #0x4963c4
00496388: ldr      r3, [r2, #0x2c]
0049638c: cmp      r3, #0
00496390: beq      #0x4963b4
00496394: ldr      r5, [r3, #8]
00496398: mov      r6, #1
0049639c: strb     r6, [r3]
004963a0: mul      r4, r4, r5
004963a4: ldr      r3, [ip, r4]
004963a8: ldr      r3, [r3, #0x14]
004963ac: cmp      r3, r6
004963b0: beq      #0x4963bc
004963b4: pop      {r4, r5, r6}
004963b8: bx       lr
004963bc: pop      {r4, r5, r6}
004963c0: b        #0x496144
004963c4: mov      r3, #0
004963c8: pop      {r4, r5, r6}
004963cc: b        #0x496144

# _ZN10AnimatedFX11SetEndPointERK7Point3DIfE
00492560: push     {r4, r5, r6, r7, r8, lr}
00492564: mov      r4, r1
00492568: movw     r1, #0x6164
0049256c: mov      r6, r0
00492570: movt     r1, #0x7065
00492574: ldr      r0, [r0, #0x2c]
00492578: bl       #0x4709dc
0049257c: subs     r5, r0, #0
00492580: beq      #0x492678
00492584: ldr      r0, [r6, #0x28]
00492588: cmp      r0, #0
0049258c: beq      #0x492678
00492590: bl       #0x3935dc
00492594: mov      r6, r0
00492598: ldr      r1, [r0]
0049259c: ldr      r0, [r4]
004925a0: bl       #0x30e3ac
004925a4: ldr      r1, [r6, #4]
004925a8: mov      r8, r0
004925ac: ldr      r0, [r4, #4]
004925b0: bl       #0x30e3ac
004925b4: ldr      r1, [r6, #8]
004925b8: mov      r7, r0
004925bc: ldr      r0, [r4, #8]
004925c0: bl       #0x30e3ac
004925c4: mov      r1, r8
004925c8: mov      r6, r0
004925cc: mov      r0, r8
004925d0: bl       #0x30ed6c
004925d4: mov      r1, r7
004925d8: mov      r4, r0
004925dc: mov      r0, r7
004925e0: bl       #0x30ed6c
004925e4: mov      r1, r0
004925e8: mov      r0, r4
004925ec: bl       #0x30eba4
004925f0: mov      r1, r6
004925f4: mov      r4, r0
004925f8: mov      r0, r6
004925fc: bl       #0x30ed6c
00492600: mov      r1, r0
00492604: mov      r0, r4
00492608: bl       #0x30eba4
0049260c: bl       #0x30e124
00492610: ldr      r4, [r5, #0x178]
00492614: cmp      r4, #0
00492618: addne    r4, r4, #0x60
0049261c: ldr      r3, [r4, #8]
00492620: cmp      r3, #0
00492624: bne      #0x492678
00492628: movw     r1, #0x999a
0049262c: movt     r1, #0x3e99
00492630: bl       #0x30ed6c
00492634: ldr      r3, [r4, #4]
00492638: mov      r6, r0
0049263c: mov      r0, r5
00492640: str      r6, [r3, #0x2c]
00492644: ldr      r3, [r5]
00492648: mov      lr, pc
0049264c: ldr      pc, [r3, #0xa0]
00492650: mov      r1, #0xbf000000
00492654: mov      r4, r0
00492658: mov      r0, r6
0049265c: bl       #0x30ed6c
00492660: ldr      r3, [r4, #8]
00492664: ldr      r2, [r4, #4]
00492668: mov      r1, r0
0049266c: mov      r0, r5
00492670: pop      {r4, r5, r6, r7, r8, lr}
00492674: b        #0x597154
00492678: pop      {r4, r5, r6, r7, r8, pc}

# _ZN10AnimatedFX10SetVisibleEb
00492ef0: push     {r4, lr}
00492ef4: ldrb     r2, [r0, #0x24]
00492ef8: mov      r4, r0
00492efc: cmp      r2, r1
00492f00: beq      #0x492f38
00492f04: ldr      r0, [r0, #0x2c]
00492f08: strb     r1, [r4, #0x24]
00492f0c: cmp      r0, #0
00492f10: beq      #0x492f28
00492f14: bl       #0x471368
00492f18: ldr      r3, [r4, #0x2c]
00492f1c: ldrb     r2, [r4, #0x24]
00492f20: ldr      r3, [r3, #8]
00492f24: strb     r2, [r3, #0x200]
00492f28: mov      r0, r4
00492f2c: mov      r1, #0
00492f30: pop      {r4, lr}
00492f34: b        #0x492aa0
00492f38: pop      {r4, pc}

# _ZN10AnimatedFX8SetSpeedEf
004924b0: push     {r4, lr}
004924b4: ldr      r3, [r0, #0x2c]
004924b8: str      r1, [r0, #0x20]
004924bc: cmp      r3, #0
004924c0: beq      #0x4924dc
004924c4: ldr      r3, [r3, #0x38]
004924c8: mov      r2, #0
004924cc: mov      r0, r3
004924d0: ldr      r3, [r3]
004924d4: mov      lr, pc
004924d8: ldr      pc, [r3, #0x28]
004924dc: pop      {r4, pc}

# _ZN10AnimatedFX6UpdateEv
00492f68: push     {r4, r5, r6, r7, r8, sl, lr}
00492f6c: ldr      r5, [pc, #0x1a4]
00492f70: ldr      r6, [pc, #0x1a4]
00492f74: ldr      r3, [r0, #0x28]
00492f78: add      r5, pc, r5
00492f7c: ldr      r2, [r5, r6]
00492f80: sub      sp, sp, #0x44
00492f84: cmp      r3, #0
00492f88: ldr      r2, [r2]
00492f8c: mov      r4, r0
00492f90: str      r2, [sp, #0x3c]
00492f94: beq      #0x492fb8
00492f98: mov      r0, r3
00492f9c: ldr      r3, [r3]
00492fa0: mov      lr, pc
00492fa4: ldr      pc, [r3, #0x34]
00492fa8: cmp      r0, #0
00492fac: beq      #0x4930d4
00492fb0: mov      r3, #0
00492fb4: str      r3, [r4, #0x28]
00492fb8: ldr      r7, [r4, #0x1c]
00492fbc: cmp      r7, #0
00492fc0: blt      #0x492fe4
00492fc4: ldr      r3, [pc, #0x154]
00492fc8: ldr      r0, [r5, r3]
00492fcc: bl       #0x31f66c
00492fd0: rsb      r0, r0, r7
00492fd4: cmp      r0, #0
00492fd8: movle    r3, #0
00492fdc: str      r0, [r4, #0x1c]
00492fe0: strle    r3, [r4, #0x14]
00492fe4: ldr      r3, [r4, #0x28]
00492fe8: cmp      r3, #0
00492fec: beq      #0x492ffc
00492ff0: ldrb     r1, [r3, #0x84]
00492ff4: cmp      r1, #0
00492ff8: beq      #0x4930c8
00492ffc: ldrb     r3, [r4, #0x24]
00493000: cmp      r3, #0
00493004: beq      #0x493088
00493008: ldr      r3, [r4, #0x50]
0049300c: cmp      r3, #0
00493010: beq      #0x4930ec
00493014: ldr      r8, [pc, #0x108]
00493018: add      r7, sp, #0x24
0049301c: ldr      sl, [r5, r8]
00493020: mov      r0, sl
00493024: bl       #0x337888
00493028: ldr      r1, [pc, #0xf8]
0049302c: add      r2, sp, #8
00493030: mov      r0, r7
00493034: add      r1, pc, r1
00493038: bl       #0x3140ec
0049303c: mov      r0, sl
00493040: mov      r1, r7
00493044: bl       #0x337a88
00493048: mov      r0, r7
0049304c: bl       #0x3139ac
00493050: ldr      r8, [r5, r8]
00493054: add      r7, sp, #0xc
00493058: mov      r0, r8
0049305c: bl       #0x337888
00493060: ldr      r1, [pc, #0xc4]
00493064: add      r2, sp, #4
00493068: mov      r0, r7
0049306c: add      r1, pc, r1
00493070: bl       #0x3140ec
00493074: mov      r0, r8
00493078: mov      r1, r7
0049307c: bl       #0x337a88
00493080: mov      r0, r7
00493084: bl       #0x3139ac
00493088: mov      r0, r4
0049308c: ldr      r1, [r4, #0x20]
00493090: bl       #0x4924b0
00493094: ldrb     r7, [r4, #0x18]
00493098: cmp      r7, #0
0049309c: bne      #0x4930ac
004930a0: ldrb     r3, [r4, #0x24]
004930a4: cmp      r3, #0
004930a8: bne      #0x4930f4
004930ac: ldr      r3, [r5, r6]
004930b0: ldr      r2, [sp, #0x3c]
004930b4: ldr      r3, [r3]
004930b8: cmp      r2, r3
004930bc: bne      #0x493114
004930c0: add      sp, sp, #0x44
004930c4: pop      {r4, r5, r6, r7, r8, sl, pc}
004930c8: mov      r0, r4
004930cc: bl       #0x492aa0
004930d0: b        #0x492ffc
004930d4: ldr      r3, [r4, #0x28]
004930d8: ldrb     r3, [r3, #0x81]
004930dc: cmp      r3, #0
004930e0: movne    r3, #0
004930e4: strne    r3, [r4, #0x28]
004930e8: b        #0x492fb8
004930ec: ldr      r8, [pc, #0x30]
004930f0: b        #0x493050
004930f4: mov      r0, r4
004930f8: bl       #0x4924e0
004930fc: cmp      r0, #0
00493100: beq      #0x4930ac
00493104: mov      r0, r4
00493108: mov      r1, r7
0049310c: bl       #0x492ef0
00493110: b        #0x4930ac
00493114: bl       #0x30e310
00493118: subseq   r1, r0, r8, lsl fp
0049311c: andeq    r4, r0, ip, lsr #1
00493120: strdeq   r3, r4, [r0], -r4
00493124: andeq    r0, r0, r4, lsl #17
00493128: strdeq   r1, r2, [r4], #-0xfc
0049312c: subeq    r1, r4, r4, asr #31

# _ZN15VisualFXManager10PlayAnimFXEiRK7Point3DIfES3_PK10GameObjectPNS_10AnimFXDataE
004956b8: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
004956bc: ldr      r4, [pc, #0x1b0]
004956c0: ldr      r7, [pc, #0x1b0]
004956c4: ldr      lr, [pc, #0x1b0]
004956c8: add      r4, pc, r4
004956cc: ldr      ip, [r4, r7]
004956d0: ldr      r5, [r4, lr]
004956d4: sub      sp, sp, #0x3c
004956d8: ldr      ip, [ip]
004956dc: str      r0, [sp, #0x10]
004956e0: mov      r0, r5
004956e4: str      r3, [sp, #0x14]
004956e8: str      ip, [sp, #0x34]
004956ec: mov      sb, r1
004956f0: mov      r8, r2
004956f4: ldr      fp, [sp, #0x60]
004956f8: ldr      r6, [sp, #0x64]
004956fc: bl       #0x337888
00495700: ldr      r1, [pc, #0x178]
00495704: add      sl, sp, #0x1c
00495708: add      r2, sp, #0x18
0049570c: add      r1, pc, r1
00495710: mov      r0, sl
00495714: bl       #0x3140ec
00495718: mov      r0, r5
0049571c: mov      r1, sl
00495720: bl       #0x337ec8
00495724: mov      r5, r0
00495728: ldr      r0, [sp, #0x30]
0049572c: cmp      r0, sl
00495730: beq      #0x495750
00495734: cmp      r0, #0
00495738: beq      #0x495750
0049573c: ldr      r1, [sp, #0x1c]
00495740: rsb      r1, r0, r1
00495744: cmp      r1, #0x80
00495748: bhi      #0x495834
0049574c: bl       #0x708f00
00495750: cmp      r5, #0
00495754: bne      #0x495774
00495758: ldr      r3, [r4, r7]
0049575c: ldr      r2, [sp, #0x34]
00495760: ldr      r3, [r3]
00495764: cmp      r2, r3
00495768: bne      #0x495870
0049576c: add      sp, sp, #0x3c
00495770: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495774: ldr      r0, [sp, #0x10]
00495778: mov      r1, sb
0049577c: bl       #0x494ad4
00495780: subs     r5, r0, #0
00495784: beq      #0x495758
00495788: ldr      r2, [r8, #4]
0049578c: ldr      r3, [r8, #8]
00495790: ldr      r1, [r8]
00495794: str      r2, [r5, #0x38]
00495798: str      r3, [r5, #0x3c]
0049579c: str      r1, [r5, #0x34]
004957a0: mov      r1, #0
004957a4: bl       #0x492aa0
004957a8: mov      r0, r5
004957ac: ldr      r1, [sp, #0x14]
004957b0: bl       #0x492f3c
004957b4: mov      r0, r5
004957b8: mov      r1, #1
004957bc: bl       #0x492694
004957c0: mov      r0, r5
004957c4: bl       #0x492744
004957c8: cmp      r6, #0
004957cc: beq      #0x49583c
004957d0: ldr      r0, [pc, #0xac]
004957d4: ldr      sl, [r6, #4]
004957d8: ldr      ip, [r6, #8]
004957dc: ldr      lr, [r6, #0x10]
004957e0: ldr      r8, [r4, r0]
004957e4: ldrb     r3, [r6, #2]
004957e8: ldrb     r1, [r6]
004957ec: ldrb     r2, [r6, #1]
004957f0: mov      r0, r5
004957f4: str      sl, [sp]
004957f8: stmib    sp, {ip, lr}
004957fc: str      r8, [sp, #0xc]
00495800: bl       #0x492e8c
00495804: ldr      r3, [r6, #0xc]
00495808: str      r3, [r5, #0x1c]
0049580c: cmp      fp, #0
00495810: beq      #0x495824
00495814: str      fp, [r5, #0x28]
00495818: mov      r0, r5
0049581c: mov      r1, #1
00495820: bl       #0x492aa0
00495824: mov      r0, r5
00495828: mov      r1, #1
0049582c: bl       #0x492ef0
00495830: b        #0x495758
00495834: bl       #0x310440
00495838: b        #0x495750
0049583c: ldr      r3, [pc, #0x40]
00495840: mov      r1, #1
00495844: mov      ip, #0x3f800000
00495848: ldr      lr, [r4, r3]
0049584c: mov      r2, r6
00495850: mov      r0, r5
00495854: mov      r3, r1
00495858: str      ip, [sp]
0049585c: str      lr, [sp, #0xc]
00495860: str      r6, [sp, #4]
00495864: str      r6, [sp, #8]
00495868: bl       #0x492e8c
0049586c: b        #0x49580c
00495870: bl       #0x30e310
00495874: subeq    pc, pc, r8, asr #7
00495878: andeq    r4, r0, ip, lsr #1
0049587c: andeq    r0, r0, r4, lsl #17
00495880: subeq    pc, r3, ip, lsr sb
00495884: andeq    r4, r0, r8, lsr ip

# _ZN10AnimatedFX14_HandleLoopEndEv
004927a4: push     {r4, r5, r6, lr}
004927a8: mov      r1, #0
004927ac: mov      r4, r0
004927b0: ldr      r0, [r0, #0x20]
004927b4: bl       #0x30e9ac
004927b8: cmp      r0, #0
004927bc: beq      #0x4927cc
004927c0: ldr      r3, [r4, #4]
004927c4: cmp      r3, #0
004927c8: beq      #0x49288c
004927cc: ldr      r1, [r4, #0x14]
004927d0: cmp      r1, #0
004927d4: blt      #0x49288c
004927d8: bne      #0x492890
004927dc: mov      r0, r4
004927e0: bl       #0x492694
004927e4: ldr      r3, [r4, #0x50]
004927e8: cmp      r3, #0
004927ec: movne    r2, #1
004927f0: strbne   r2, [r3]
004927f4: ldr      r3, [r4, #4]
004927f8: cmp      r3, #0
004927fc: beq      #0x49288c
00492800: ldr      r1, [r4, #0x50]
00492804: mov      r0, r4
00492808: blx      r3
0049280c: mov      r3, #0
00492810: str      r3, [r4, #4]
00492814: mov      r0, r4
00492818: bl       #0x49267c
0049281c: ldr      r3, [r0]
00492820: mov      lr, pc
00492824: ldr      pc, [r3, #0x44]
00492828: ldr      r3, [r4, #0x2c]
0049282c: mov      r0, r4
00492830: ldr      r6, [r3, #8]
00492834: bl       #0x49267c
00492838: ldr      r3, [r0]
0049283c: mov      lr, pc
00492840: ldr      pc, [r3, #0x44]
00492844: ldr      r5, [r0, #0x14]
00492848: mov      r0, r4
0049284c: bl       #0x49267c
00492850: ldr      r3, [r0]
00492854: mov      lr, pc
00492858: ldr      pc, [r3, #0x44]
0049285c: ldr      r3, [r0, #4]
00492860: cmp      r5, r3
00492864: beq      #0x492870
00492868: mov      r0, r4
0049286c: bl       #0x4926e4
00492870: mov      r0, r4
00492874: bl       #0x49267c
00492878: mov      r1, r6
0049287c: mov      r2, r5
00492880: ldr      r3, [r0]
00492884: mov      lr, pc
00492888: ldr      pc, [r3, #0x10]
0049288c: pop      {r4, r5, r6, pc}
00492890: sub      r1, r1, #1
00492894: mov      r0, r4
00492898: str      r1, [r4, #0x14]
0049289c: pop      {r4, r5, r6, lr}
004928a0: b        #0x492744

# _ZN10AnimatedFX11SetRotationERK7Point3DIfE
00492f3c: mov      r2, r1
00492f40: ldr      r1, [r1]
00492f44: str      r1, [r0, #0x40]
00492f48: ldr      ip, [r2, #4]
00492f4c: mov      r1, #0
00492f50: str      ip, [r0, #0x44]
00492f54: ldr      r2, [r2, #8]
00492f58: mov      ip, #1
00492f5c: strb     ip, [r0, #0x4c]
00492f60: str      r2, [r0, #0x48]
00492f64: b        #0x492aa0

# _ZN10GameObject7_PlayFXERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003923cc: push     {r4, r5, r6, lr}
003923d0: ldr      r3, [r0, #4]
003923d4: ldr      r4, [pc, #0x230]
003923d8: sub      sp, sp, #0x20
003923dc: ldr      r1, [r3, #4]
003923e0: ldr      ip, [r3]
003923e4: add      r4, pc, r4
003923e8: mov      r5, r0
003923ec: rsb      r3, ip, r1
003923f0: asr      r3, r3, #4
003923f4: add      r1, r3, r3, lsl #3
003923f8: add      r1, r1, r1, lsl #6
003923fc: add      r1, r3, r1, lsl #3
00392400: add      r1, r1, r1, lsl #15
00392404: add      r3, r3, r1, lsl #3
00392408: cmp      r3, #0
0039240c: bne      #0x392418
00392410: add      sp, sp, #0x20
00392414: pop      {r4, r5, r6, pc}
00392418: ldr      r3, [ip, #4]
0039241c: cmp      r3, #3
00392420: bne      #0x392410
00392424: mov      r1, #0
00392428: str      r2, [sp, #0xc]
0039242c: bl       #0x37baf8
00392430: bl       #0x38d798
00392434: ldr      r3, [pc, #0x1d4]
00392438: ldr      r2, [sp, #0xc]
0039243c: ldr      r3, [r4, r3]
00392440: ldr      r3, [r3]
00392444: cmp      r0, r3
00392448: bhs      #0x392410
0039244c: ldr      r6, [r5, #4]
00392450: ldr      r1, [r6, #4]
00392454: ldr      r3, [r6]
00392458: rsb      r3, r3, r1
0039245c: asr      r3, r3, #4
00392460: add      r1, r3, r3, lsl #3
00392464: add      r1, r1, r1, lsl #6
00392468: add      r1, r3, r1, lsl #3
0039246c: add      r1, r1, r1, lsl #15
00392470: add      r3, r3, r1, lsl #3
00392474: rsb      r3, r3, #0
00392478: cmp      r3, #3
0039247c: bls      #0x392564
00392480: mov      r3, #0
00392484: str      r3, [sp, #0x1c]
00392488: str      r3, [sp, #0x14]
0039248c: str      r3, [sp, #0x18]
00392490: ldm      r6, {r1, r3}
00392494: rsb      r3, r1, r3
00392498: asr      r3, r3, #4
0039249c: add      r0, r3, r3, lsl #3
003924a0: add      r0, r0, r0, lsl #6
003924a4: add      r0, r3, r0, lsl #3
003924a8: add      r0, r0, r0, lsl #15
003924ac: add      r3, r3, r0, lsl #3
003924b0: rsb      r3, r3, #0
003924b4: cmp      r3, #1
003924b8: bhi      #0x3924d4
003924bc: ldr      r0, [pc, #0x150]
003924c0: str      r2, [sp, #0xc]
003924c4: add      r0, pc, r0
003924c8: bl       #0x708eb0
003924cc: ldr      r1, [r6]
003924d0: ldr      r2, [sp, #0xc]
003924d4: ldr      r3, [r1, #0x74]
003924d8: cmp      r3, #3
003924dc: beq      #0x392594
003924e0: ldr      r0, [r2, #0x168]
003924e4: ldr      r1, [r2, #0x160]
003924e8: ldr      r3, [r2, #0x164]
003924ec: str      r0, [sp, #0x1c]
003924f0: str      r1, [sp, #0x14]
003924f4: str      r3, [sp, #0x18]
003924f8: ldr      r5, [r5, #4]
003924fc: ldm      r5, {r0, r3}
00392500: rsb      r3, r0, r3
00392504: asr      r3, r3, #4
00392508: add      r2, r3, r3, lsl #3
0039250c: add      r2, r2, r2, lsl #6
00392510: add      r2, r3, r2, lsl #3
00392514: add      r2, r2, r2, lsl #15
00392518: add      r3, r3, r2, lsl #3
0039251c: cmp      r3, #0
00392520: bne      #0x392534
00392524: ldr      r0, [pc, #0xec]
00392528: add      r0, pc, r0
0039252c: bl       #0x708eb0
00392530: ldr      r0, [r5]
00392534: bl       #0x31bbf0
00392538: ldr      r3, [pc, #0xdc]
0039253c: ldr      r4, [r4, r3]
00392540: bl       #0x8be2a0
00392544: mov      ip, #0
00392548: mov      r1, r0
0039254c: mov      r3, ip
00392550: mov      r0, r4
00392554: add      r2, sp, #0x14
00392558: str      ip, [sp]
0039255c: bl       #0x495d14
00392560: b        #0x392410
00392564: mov      r1, #0
00392568: mov      r0, r5
0039256c: str      r2, [sp, #0xc]
00392570: bl       #0x37baf8
00392574: bl       #0x38d798
00392578: ldr      r3, [pc, #0x9c]
0039257c: mov      r1, r0
00392580: ldr      r2, [sp, #0xc]
00392584: ldr      r0, [r4, r3]
00392588: mov      r3, #0
0039258c: bl       #0x495f04
00392590: b        #0x392410
00392594: mov      r1, #2
00392598: mov      r0, r5
0039259c: str      r2, [sp, #0xc]
003925a0: bl       #0x37baf8
003925a4: ldr      r1, [r0, #4]
003925a8: ldr      r2, [sp, #0xc]
003925ac: cmp      r1, #3
003925b0: bne      #0x3924e0
003925b4: mov      r0, r5
003925b8: bl       #0x37baf8
003925bc: ldr      r6, [r0, #4]
003925c0: ldr      r2, [sp, #0xc]
003925c4: cmp      r6, #3
003925c8: bne      #0x3924e0
003925cc: mov      r1, #1
003925d0: mov      r0, r5
003925d4: bl       #0x37baf8
003925d8: bl       #0x31bbf0
003925dc: mov      r1, #2
003925e0: str      r0, [sp, #0x14]
003925e4: mov      r0, r5
003925e8: bl       #0x37baf8
003925ec: bl       #0x31bbf0
003925f0: mov      r1, r6
003925f4: str      r0, [sp, #0x18]
003925f8: mov      r0, r5
003925fc: bl       #0x37baf8
00392600: bl       #0x31bbf0
00392604: str      r0, [sp, #0x1c]
00392608: b        #0x3924f8
0039260c: rsbeq    r2, r0, ip, lsr #13
00392610: andeq    r0, r0, r4, asr #13
00392614: subseq   fp, r2, r4, lsr #31
00392618: subseq   fp, r2, r0, asr #30
0039261c: andeq    r1, r0, r8, lsl #22

# _ZN15VisualFXManager16GetAnimFXSetDataEiiiPK10GameObjectPNS_13AnimFXSetDataE7Point3DIfES6_
004935b8: push     {r4, r5, r6, r7, r8, lr}
004935bc: mov      r0, #0x30
004935c0: mov      r4, r1
004935c4: mov      r1, #0
004935c8: ldr      r7, [sp, #0x20]
004935cc: ldr      r6, [sp, #0x24]
004935d0: mov      r5, r2
004935d4: mov      r8, r3
004935d8: bl       #0x310570
004935dc: mov      r2, #0
004935e0: mov      r1, #0
004935e4: str      r2, [r0, #0x24]
004935e8: strb     r1, [r0]
004935ec: str      r8, [r0, #4]
004935f0: str      r4, [r0, #8]
004935f4: str      r5, [r0, #0xc]
004935f8: ldr      r1, [sp, #0x18]
004935fc: str      r2, [r0, #0x10]
00493600: str      r2, [r0, #0x14]
00493604: str      r2, [r0, #0x18]
00493608: str      r2, [r0, #0x1c]
0049360c: str      r2, [r0, #0x20]
00493610: str      r1, [r0, #0x28]
00493614: ldr      r2, [r7]
00493618: str      r2, [r0, #0x10]
0049361c: ldr      r2, [r7, #4]
00493620: str      r2, [r0, #0x14]
00493624: ldr      r2, [r7, #8]
00493628: str      r2, [r0, #0x18]
0049362c: ldr      r2, [r6]
00493630: str      r2, [r0, #0x1c]
00493634: ldr      r2, [r6, #4]
00493638: str      r2, [r0, #0x20]
0049363c: ldr      r2, [r6, #8]
00493640: ldr      r1, [sp, #0x1c]
00493644: str      r2, [r0, #0x24]
00493648: str      r1, [r0, #0x2c]
0049364c: pop      {r4, r5, r6, r7, r8, pc}

# _ZN10AnimatedFX10SetLoopingEb
00492694: push     {r4, r5, r6, lr}
00492698: ldr      r3, [r0, #0x2c]
0049269c: mov      r4, r0
004926a0: mov      r5, r1
004926a4: cmp      r3, #0
004926a8: beq      #0x4926dc
004926ac: bl       #0x49267c
004926b0: cmp      r0, #0
004926b4: beq      #0x4926dc
004926b8: mov      r0, r4
004926bc: bl       #0x49267c
004926c0: ldr      r3, [r0]
004926c4: mov      lr, pc
004926c8: ldr      pc, [r3, #0x44]
004926cc: mov      r1, r5
004926d0: ldr      r3, [r0]
004926d4: mov      lr, pc
004926d8: ldr      pc, [r3, #0x40]
004926dc: strb     r5, [r4, #0x18]
004926e0: pop      {r4, r5, r6, pc}

# _ZN15VisualFXManager14PlayAnimFXStepEPNS_13AnimFXSetDataENS_10AnimFXDataE
00495f74: sub      sp, sp, #8
00495f78: push     {r4, r5, r6, r7, r8, lr}
00495f7c: mov      r5, r0
00495f80: mov      r4, r1
00495f84: ldr      r0, [r1, #8]
00495f88: ldr      r1, [r5, #0x1c]
00495f8c: mov      r6, #0x18
00495f90: sub      sp, sp, #8
00495f94: mla      r6, r6, r0, r1
00495f98: str      r3, [sp, #0x24]
00495f9c: str      r2, [sp, #0x20]
00495fa0: ldr      r1, [r4, #4]
00495fa4: ldr      r2, [r6, #4]
00495fa8: ldr      r3, [pc, #0x18c]
00495fac: ldr      r2, [r2, r1, lsl #2]
00495fb0: add      r3, pc, r3
00495fb4: ldrb     r2, [r2]
00495fb8: cmp      r2, #0
00495fbc: beq      #0x496020
00495fc0: ldr      r2, [pc, #0x178]
00495fc4: add      r7, r4, #0x1c
00495fc8: mov      r0, r7
00495fcc: ldr      r8, [r3, r2]
00495fd0: mov      r1, r8
00495fd4: bl       #0x312b6c
00495fd8: cmp      r0, #0
00495fdc: beq      #0x4960bc
00495fe0: add      r7, r4, #0x10
00495fe4: mov      r1, r8
00495fe8: mov      r0, r7
00495fec: bl       #0x312b6c
00495ff0: cmp      r0, #0
00495ff4: bne      #0x4960ec
00495ff8: ldr      r2, [r6, #4]
00495ffc: ldr      r1, [r4, #4]
00496000: ldr      r3, [r4, #0x28]
00496004: mov      r0, r5
00496008: ldr      r1, [r2, r1, lsl #2]
0049600c: mov      r2, r7
00496010: ldr      r1, [r1, #4]
00496014: str      r4, [sp]
00496018: bl       #0x495d14
0049601c: b        #0x4960ac
00496020: ldr      r2, [pc, #0x118]
00496024: add      r7, r4, #0x1c
00496028: mov      r0, r7
0049602c: ldr      r8, [r3, r2]
00496030: mov      r1, r8
00496034: bl       #0x312b6c
00496038: cmp      r0, #0
0049603c: beq      #0x49607c
00496040: add      r7, r4, #0x10
00496044: mov      r1, r8
00496048: mov      r0, r7
0049604c: bl       #0x312b6c
00496050: cmp      r0, #0
00496054: beq      #0x496110
00496058: ldr      r3, [r6, #4]
0049605c: ldr      r1, [r4, #4]
00496060: mov      r0, r5
00496064: ldr      r2, [r4, #0x28]
00496068: ldr      r1, [r3, r1, lsl #2]
0049606c: add      r3, sp, #0x20
00496070: ldr      r1, [r1, #4]
00496074: bl       #0x495f3c
00496078: b        #0x4960ac
0049607c: ldr      r3, [r6, #4]
00496080: ldr      r2, [r4, #4]
00496084: ldr      ip, [r4, #0x28]
00496088: mov      r0, r5
0049608c: ldr      r1, [r3, r2, lsl #2]
00496090: add      r2, r4, #0x10
00496094: mov      r3, r7
00496098: ldr      r1, [r1, #4]
0049609c: str      ip, [sp]
004960a0: add      ip, sp, #0x20
004960a4: str      ip, [sp, #4]
004960a8: bl       #0x4956b8
004960ac: add      sp, sp, #8
004960b0: pop      {r4, r5, r6, r7, r8, lr}
004960b4: add      sp, sp, #8
004960b8: bx       lr
004960bc: ldr      r3, [r6, #4]
004960c0: ldr      r2, [r4, #4]
004960c4: ldr      ip, [r4, #0x28]
004960c8: mov      r0, r5
004960cc: ldr      r1, [r3, r2, lsl #2]
004960d0: add      r2, r4, #0x10
004960d4: mov      r3, r7
004960d8: ldr      r1, [r1, #4]
004960dc: str      ip, [sp]
004960e0: str      r4, [sp, #4]
004960e4: bl       #0x495888
004960e8: b        #0x4960ac
004960ec: ldr      r2, [r6, #4]
004960f0: ldr      r1, [r4, #4]
004960f4: mov      r0, r5
004960f8: mov      r3, r4
004960fc: ldr      r1, [r2, r1, lsl #2]
00496100: ldr      r2, [r4, #0x28]
00496104: ldr      r1, [r1, #4]
00496108: bl       #0x495f04
0049610c: b        #0x4960ac
00496110: ldr      r2, [r6, #4]
00496114: ldr      r1, [r4, #4]
00496118: ldr      r3, [r4, #0x28]
0049611c: add      ip, sp, #0x20
00496120: ldr      r1, [r2, r1, lsl #2]
00496124: mov      r0, r5
00496128: mov      r2, r7
0049612c: ldr      r1, [r1, #4]
00496130: str      ip, [sp]
00496134: bl       #0x495b54
00496138: b        #0x4960ac
0049613c: subeq    lr, pc, r0, ror #21
00496140: andeq    r3, r0, ip, lsr #30

# _ZN15VisualFXManager13PlayAnimFXSetEiRK7Point3DIfES3_PK10GameObjectPNS_13AnimFXSetDataE
00495888: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
0049588c: ldr      ip, [pc, #0x1ec]
00495890: cmp      r1, #0
00495894: sub      sp, sp, #0x74
00495898: add      ip, pc, ip
0049589c: mov      r6, r0
004958a0: mov      r5, r2
004958a4: mov      r4, r3
004958a8: blt      #0x495a18
004958ac: ldr      r3, [pc, #0x1d0]
004958b0: ldr      r3, [ip, r3]
004958b4: ldr      r3, [r3]
004958b8: cmp      r1, r3
004958bc: bge      #0x495a18
004958c0: mov      fp, #0x18
004958c4: mul      fp, fp, r1
004958c8: ldr      sb, [r0, #0x1c]
004958cc: ldr      r2, [sb, fp]
004958d0: add      r7, sb, fp
004958d4: ldr      r3, [r2, #0x14]
004958d8: cmp      r3, #2
004958dc: beq      #0x495a54
004958e0: mov      r0, #0
004958e4: str      r0, [sp, #0x18]
004958e8: str      r0, [sp, #0x24]
004958ec: mov      r3, r0
004958f0: ldr      ip, [r5, #4]
004958f4: ldr      r2, [r2, #8]
004958f8: add      r8, sp, #0x2c
004958fc: str      ip, [sp, #0x14]
00495900: ldr      r0, [r4, #4]
00495904: ldr      lr, [r5, #8]
00495908: ldr      sl, [r4]
0049590c: str      r0, [sp, #0x1c]
00495910: ldr      ip, [r4, #8]
00495914: mov      r0, r6
00495918: str      ip, [sp, #0x20]
0049591c: ldr      ip, [r5]
00495920: str      lr, [sp, #0x6c]
00495924: str      sl, [sp, #0x58]
00495928: str      ip, [sp, #0x64]
0049592c: ldr      ip, [sp, #0x14]
00495930: str      ip, [sp, #0x68]
00495934: ldr      ip, [sp, #0x1c]
00495938: str      ip, [sp, #0x5c]
0049593c: ldr      ip, [sp, #0x20]
00495940: str      ip, [sp, #0x60]
00495944: add      ip, sp, #0x64
00495948: str      ip, [sp, #8]
0049594c: add      ip, sp, #0x58
00495950: str      ip, [sp, #0xc]
00495954: ldr      ip, [sp, #0x98]
00495958: str      ip, [sp]
0049595c: ldr      ip, [sp, #0x9c]
00495960: str      ip, [sp, #4]
00495964: bl       #0x4935b8
00495968: add      r3, sp, #0x44
0049596c: mov      sl, r0
00495970: mov      r1, r7
00495974: mov      r0, r8
00495978: str      r3, [sp, #0x14]
0049597c: bl       #0x493924
00495980: mov      r2, r8
00495984: mov      r3, sl
00495988: mov      r1, r6
0049598c: ldr      r0, [sp, #0x14]
00495990: bl       #0x4933e4
00495994: mov      r0, r8
00495998: add      r8, r7, #0x10
0049599c: bl       #0x4940d8
004959a0: mov      r0, r8
004959a4: bl       #0x493814
004959a8: str      sl, [r0, #8]
004959ac: ldr      r3, [r7, #0x14]
004959b0: str      r8, [r0]
004959b4: str      r3, [r0, #4]
004959b8: str      r0, [r3]
004959bc: str      r0, [r7, #0x14]
004959c0: ldr      r3, [sb, fp]
004959c4: ldr      ip, [sp, #0x18]
004959c8: ldr      r2, [r3, #0x10]
004959cc: add      r3, r2, ip
004959d0: ldr      r3, [r3, #4]
004959d4: cmn      r3, #1
004959d8: beq      #0x495a20
004959dc: ldr      r3, [r7, #4]
004959e0: ldr      r0, [sp, #0x24]
004959e4: ldr      r3, [r3, r0, lsl #2]
004959e8: ldrb     r1, [r3]
004959ec: cmp      r1, #0
004959f0: beq      #0x495a20
004959f4: ldr      ip, [sp, #0x98]
004959f8: ldr      r1, [r3, #4]
004959fc: mov      r0, r6
00495a00: str      ip, [sp]
00495a04: ldr      ip, [sp, #0x9c]
00495a08: mov      r2, r5
00495a0c: mov      r3, r4
00495a10: str      ip, [sp, #4]
00495a14: bl       #0x495888
00495a18: add      sp, sp, #0x74
00495a1c: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495a20: ldr      r3, [sl, #4]
00495a24: mov      r1, #0x30
00495a28: ldr      ip, [sp, #0x98]
00495a2c: mla      r3, r1, r3, r2
00495a30: mov      r0, r6
00495a34: ldr      r1, [r3, #4]
00495a38: str      ip, [sp]
00495a3c: ldr      ip, [sp, #0x14]
00495a40: mov      r2, r5
00495a44: mov      r3, r4
00495a48: str      ip, [sp, #4]
00495a4c: bl       #0x4956b8
00495a50: b        #0x495a18
00495a54: ldr      r0, [r2, #0xc]
00495a58: str      r1, [sp, #0x10]
00495a5c: bl       #0x493780
00495a60: mov      r2, #0x30
00495a64: mul      r2, r2, r0
00495a68: mov      r3, r0
00495a6c: str      r2, [sp, #0x18]
00495a70: ldr      r2, [sb, fp]
00495a74: ldr      r1, [sp, #0x10]
00495a78: str      r0, [sp, #0x24]
00495a7c: b        #0x4958f0

# _ZN10AnimatedFX11GetAnimatorEv
0049267c: ldr      r0, [r0, #0x2c]
00492680: cmp      r0, #0
00492684: bxeq     lr
00492688: ldr      r0, [r0, #0x38]
0049268c: mov      r1, #0
00492690: b        #0x4748b8

# _ZN10AnimatedFX6SetEndEv
004926e4: push     {r4, r5, r6, lr}
004926e8: ldr      r3, [r0, #0x2c]
004926ec: mov      r4, r0
004926f0: cmp      r3, #0
004926f4: beq      #0x492740
004926f8: bl       #0x49267c
004926fc: cmp      r0, #0
00492700: beq      #0x492740
00492704: mov      r0, r4
00492708: bl       #0x49267c
0049270c: ldr      r3, [r0]
00492710: mov      lr, pc
00492714: ldr      pc, [r3, #0x44]
00492718: ldr      r5, [r0, #0x14]
0049271c: mov      r0, r4
00492720: bl       #0x49267c
00492724: ldr      r3, [r0]
00492728: mov      lr, pc
0049272c: ldr      pc, [r3, #0x44]
00492730: mov      r1, r5
00492734: ldr      r3, [r0]
00492738: mov      lr, pc
0049273c: ldr      pc, [r3, #0xc]
00492740: pop      {r4, r5, r6, pc}

# _ZN15VisualFXManager6UpdateEv
00496594: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00496598: ldr      r6, [pc, #0x230]
0049659c: ldr      r8, [pc, #0x230]
004965a0: ldr      r2, [pc, #0x230]
004965a4: add      r6, pc, r6
004965a8: ldr      r3, [r6, r8]
004965ac: ldr      r7, [r6, r2]
004965b0: sub      sp, sp, #0x2c
004965b4: ldr      r3, [r3]
004965b8: mov      r5, r0
004965bc: mov      r0, r7
004965c0: str      r3, [sp, #0x24]
004965c4: bl       #0x337888
004965c8: ldr      r1, [pc, #0x20c]
004965cc: add      r4, sp, #0xc
004965d0: add      r2, sp, #8
004965d4: add      r1, pc, r1
004965d8: mov      r0, r4
004965dc: bl       #0x3140ec
004965e0: mov      r0, r7
004965e4: mov      r1, r4
004965e8: bl       #0x337ec8
004965ec: mov      r7, r0
004965f0: ldr      r0, [sp, #0x20]
004965f4: cmp      r0, r4
004965f8: beq      #0x496618
004965fc: cmp      r0, #0
00496600: beq      #0x496618
00496604: ldr      r1, [sp, #0xc]
00496608: rsb      r1, r0, r1
0049660c: cmp      r1, #0x80
00496610: bhi      #0x4967c4
00496614: bl       #0x708f00
00496618: cmp      r7, #0
0049661c: bne      #0x49663c
00496620: ldr      r3, [r6, r8]
00496624: ldr      r2, [sp, #0x24]
00496628: ldr      r3, [r3]
0049662c: cmp      r2, r3
00496630: bne      #0x4967cc
00496634: add      sp, sp, #0x2c
00496638: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
0049663c: ldr      r0, [pc, #0x19c]
00496640: add      r0, pc, r0
00496644: bl       #0x3136b4
00496648: ldr      r7, [r5, #0x28]
0049664c: ldr      r3, [r5, #0x2c]
00496650: rsb      r3, r7, r3
00496654: asr      r3, r3, #3
00496658: add      r2, r3, r3, lsl #2
0049665c: add      r2, r2, r2, lsl #4
00496660: add      r2, r2, r2, lsl #8
00496664: add      r2, r2, r2, lsl #16
00496668: add      r3, r3, r2, lsl #1
0049666c: cmp      r3, #0
00496670: beq      #0x4966d0
00496674: mov      sl, #0
00496678: mov      sb, sl
0049667c: add      r7, r7, sl
00496680: ldr      r4, [r7, #0x10]!
00496684: b        #0x496694
00496688: ldr      r0, [r4, #8]
0049668c: bl       #0x492f68
00496690: ldr      r4, [r4]
00496694: cmp      r7, r4
00496698: bne      #0x496688
0049669c: ldr      r7, [r5, #0x28]
004966a0: ldr      r3, [r5, #0x2c]
004966a4: add      sb, sb, #1
004966a8: add      sl, sl, #0x18
004966ac: rsb      r3, r7, r3
004966b0: asr      r3, r3, #3
004966b4: add      r2, r3, r3, lsl #2
004966b8: add      r2, r2, r2, lsl #4
004966bc: add      r2, r2, r2, lsl #8
004966c0: add      r2, r2, r2, lsl #16
004966c4: add      r3, r3, r2, lsl #1
004966c8: cmp      sb, r3
004966cc: blo      #0x49667c
004966d0: mov      r7, r5
004966d4: ldr      r4, [r7, #8]!
004966d8: cmp      r4, r7
004966dc: addne    sl, sp, #4
004966e0: beq      #0x4967b4
004966e4: cmp      r7, r4
004966e8: beq      #0x4967b4
004966ec: ldr      r3, [r4, #8]
004966f0: mov      r0, r3
004966f4: str      r3, [sp, #4]
004966f8: bl       #0x49267c
004966fc: ldr      r3, [r0]
00496700: mov      lr, pc
00496704: ldr      pc, [r3, #0x44]
00496708: ldr      r0, [sp, #4]
0049670c: ldr      r3, [r0, #0x2c]
00496710: ldr      fp, [r3, #8]
00496714: bl       #0x49267c
00496718: ldr      r3, [r0]
0049671c: mov      lr, pc
00496720: ldr      pc, [r3, #0x44]
00496724: ldr      sb, [r0, #0x14]
00496728: ldr      r0, [sp, #4]
0049672c: bl       #0x49267c
00496730: ldr      r3, [r0]
00496734: mov      lr, pc
00496738: ldr      pc, [r3, #0x44]
0049673c: ldr      r3, [r0, #4]
00496740: cmp      sb, r3
00496744: beq      #0x496750
00496748: ldr      r0, [sp, #4]
0049674c: bl       #0x4926e4
00496750: ldr      r0, [sp, #4]
00496754: bl       #0x49267c
00496758: mov      r2, sb
0049675c: ldr      r3, [r0]
00496760: mov      r1, fp
00496764: mov      lr, pc
00496768: ldr      pc, [r3, #0x10]
0049676c: ldr      r3, [sp, #4]
00496770: ldrb     r3, [r3, #0x24]
00496774: cmp      r3, #0
00496778: ldrne    sb, [r4]
0049677c: bne      #0x4967a8
00496780: ldr      sb, [r4]
00496784: ldr      r3, [r4, #4]
00496788: mov      r0, r4
0049678c: mov      r1, #0xc
00496790: str      sb, [r3]
00496794: str      r3, [sb, #4]
00496798: bl       #0x708f00
0049679c: mov      r0, r5
004967a0: mov      r1, sl
004967a4: bl       #0x494978
004967a8: mov      r4, sb
004967ac: cmp      r7, r4
004967b0: bne      #0x4966ec
004967b4: ldr      r0, [pc, #0x28]
004967b8: add      r0, pc, r0
004967bc: bl       #0x3136b8
004967c0: b        #0x496620
004967c4: bl       #0x310440
004967c8: b        #0x496618
004967cc: bl       #0x30e310
004967d0: subeq    lr, pc, ip, ror #9
004967d4: andeq    r4, r0, ip, lsr #1
004967d8: andeq    r0, r0, r4, lsl #17
004967dc: subeq    lr, r3, r4, ror sl
004967e0: subeq    lr, r3, r0, lsr sl
004967e4: strheq   lr, [r3], #-0x88

# _ZN15VisualFXManager10PlayAnimFXEiRK7Point3DIfEPK10GameObjectPNS_10AnimFXDataE
00495b54: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00495b58: ldr      r4, [pc, #0x1a0]
00495b5c: ldr      r7, [pc, #0x1a0]
00495b60: ldr      lr, [pc, #0x1a0]
00495b64: add      r4, pc, r4
00495b68: ldr      ip, [r4, r7]
00495b6c: ldr      r6, [r4, lr]
00495b70: sub      sp, sp, #0x3c
00495b74: ldr      ip, [ip]
00495b78: str      r0, [sp, #0x14]
00495b7c: mov      r0, r6
00495b80: mov      fp, r3
00495b84: str      ip, [sp, #0x34]
00495b88: mov      sb, r1
00495b8c: mov      r8, r2
00495b90: ldr      r5, [sp, #0x60]
00495b94: bl       #0x337888
00495b98: ldr      r1, [pc, #0x16c]
00495b9c: add      sl, sp, #0x1c
00495ba0: add      r2, sp, #0x18
00495ba4: add      r1, pc, r1
00495ba8: mov      r0, sl
00495bac: bl       #0x3140ec
00495bb0: mov      r0, r6
00495bb4: mov      r1, sl
00495bb8: bl       #0x337ec8
00495bbc: mov      r6, r0
00495bc0: ldr      r0, [sp, #0x30]
00495bc4: cmp      r0, sl
00495bc8: beq      #0x495be8
00495bcc: cmp      r0, #0
00495bd0: beq      #0x495be8
00495bd4: ldr      r1, [sp, #0x1c]
00495bd8: rsb      r1, r0, r1
00495bdc: cmp      r1, #0x80
00495be0: bhi      #0x495cc0
00495be4: bl       #0x708f00
00495be8: cmp      r6, #0
00495bec: bne      #0x495c0c
00495bf0: ldr      r3, [r4, r7]
00495bf4: ldr      r2, [sp, #0x34]
00495bf8: ldr      r3, [r3]
00495bfc: cmp      r2, r3
00495c00: bne      #0x495cfc
00495c04: add      sp, sp, #0x3c
00495c08: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00495c0c: ldr      r0, [sp, #0x14]
00495c10: mov      r1, sb
00495c14: bl       #0x494ad4
00495c18: subs     r6, r0, #0
00495c1c: beq      #0x495bf0
00495c20: ldr      r2, [r8, #4]
00495c24: ldr      r3, [r8, #8]
00495c28: ldr      r1, [r8]
00495c2c: str      r2, [r6, #0x38]
00495c30: str      r3, [r6, #0x3c]
00495c34: str      r1, [r6, #0x34]
00495c38: mov      r1, #0
00495c3c: bl       #0x492aa0
00495c40: mov      r0, r6
00495c44: mov      r1, #1
00495c48: bl       #0x492694
00495c4c: mov      r0, r6
00495c50: bl       #0x492744
00495c54: cmp      r5, #0
00495c58: beq      #0x495cc8
00495c5c: ldr      r0, [pc, #0xac]
00495c60: ldr      sl, [r5, #4]
00495c64: ldr      ip, [r5, #8]
00495c68: ldr      lr, [r5, #0x10]
00495c6c: ldr      r8, [r4, r0]
00495c70: ldrb     r3, [r5, #2]
00495c74: ldrb     r1, [r5]
00495c78: ldrb     r2, [r5, #1]
00495c7c: mov      r0, r6
00495c80: str      sl, [sp]
00495c84: stmib    sp, {ip, lr}
00495c88: str      r8, [sp, #0xc]
00495c8c: bl       #0x492e8c
00495c90: ldr      r3, [r5, #0xc]
00495c94: str      r3, [r6, #0x1c]
00495c98: cmp      fp, #0
00495c9c: beq      #0x495cb0
00495ca0: str      fp, [r6, #0x28]
00495ca4: mov      r0, r6
00495ca8: mov      r1, #1
00495cac: bl       #0x492aa0
00495cb0: mov      r0, r6
00495cb4: mov      r1, #1
00495cb8: bl       #0x492ef0
00495cbc: b        #0x495bf0
00495cc0: bl       #0x310440
00495cc4: b        #0x495be8
00495cc8: ldr      r3, [pc, #0x40]
00495ccc: mov      r1, #1
00495cd0: mov      ip, #0x3f800000
00495cd4: ldr      lr, [r4, r3]
00495cd8: mov      r2, r5
00495cdc: mov      r0, r6
00495ce0: mov      r3, r1
00495ce4: str      ip, [sp]
00495ce8: str      lr, [sp, #0xc]
00495cec: str      r5, [sp, #4]
00495cf0: str      r5, [sp, #8]
00495cf4: bl       #0x492e8c
00495cf8: b        #0x495c98
00495cfc: bl       #0x30e310
00495d00: subeq    lr, pc, ip, lsr #30
00495d04: andeq    r4, r0, ip, lsr #1
00495d08: andeq    r0, r0, r4, lsl #17
00495d0c: subeq    pc, r3, r4, lsr #9
00495d10: andeq    r4, r0, r8, lsr ip

# _ZN10AnimatedFX4LoadEPKcS1_fib
0049296c: push     {r4, r5, r6, r7, r8, sb, sl, lr}
00492970: ldr      r5, [pc, #0x11c]
00492974: ldr      sl, [pc, #0x11c]
00492978: sub      sp, sp, #0x48
0049297c: add      r5, pc, r5
00492980: ldr      ip, [r5, sl]
00492984: mov      r4, r0
00492988: add      r6, sp, #0x2c
0049298c: ldr      ip, [ip]
00492990: str      r3, [r0, #0x20]
00492994: ldr      r3, [sp, #0x68]
00492998: mov      r8, r2
0049299c: str      r1, [r0, #0xc]
004929a0: str      r3, [r0, #0x14]
004929a4: add      r7, sp, #0x14
004929a8: str      r2, [r4, #0x10]
004929ac: mov      r0, r6
004929b0: add      r2, sp, #0x10
004929b4: str      ip, [sp, #0x44]
004929b8: ldrb     sb, [sp, #0x6c]
004929bc: bl       #0x3140ec
004929c0: mov      r1, r8
004929c4: add      r2, sp, #0xc
004929c8: mov      r0, r7
004929cc: bl       #0x3140ec
004929d0: mov      r1, #0
004929d4: mov      r0, #0xac
004929d8: bl       #0x310570
004929dc: mov      r3, r7
004929e0: mov      r1, #0
004929e4: mov      r2, r6
004929e8: mov      r8, r0
004929ec: bl       #0x472a0c
004929f0: mov      r0, r7
004929f4: str      r8, [r4, #0x2c]
004929f8: bl       #0x3139ac
004929fc: mov      r0, r6
00492a00: bl       #0x3139ac
00492a04: ldr      r3, [r4, #0x2c]
00492a08: cmp      r3, #0
00492a0c: beq      #0x492a64
00492a10: ldr      r3, [r3, #0x38]
00492a14: ldr      r2, [pc, #0x80]
00492a18: mov      r6, #0
00492a1c: ldr      ip, [r3]
00492a20: ldr      r1, [r5, r2]
00492a24: mov      r0, r3
00492a28: mov      r2, r4
00492a2c: mov      r3, r6
00492a30: str      r6, [sp]
00492a34: mov      lr, pc
00492a38: ldr      pc, [ip, #0x2c]
00492a3c: ldr      r3, [r4, #0x2c]
00492a40: mov      r2, r6
00492a44: ldr      r1, [r4, #0x20]
00492a48: ldr      r3, [r3, #0x38]
00492a4c: mov      r0, r3
00492a50: ldr      r3, [r3]
00492a54: mov      lr, pc
00492a58: ldr      pc, [r3, #0x28]
00492a5c: cmp      sb, r6
00492a60: bne      #0x492a80
00492a64: ldr      r3, [r5, sl]
00492a68: ldr      r2, [sp, #0x44]
00492a6c: ldr      r3, [r3]
00492a70: cmp      r2, r3
00492a74: bne      #0x492a90
00492a78: add      sp, sp, #0x48
00492a7c: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00492a80: ldr      r3, [r4, #0x2c]
00492a84: ldr      r0, [r3, #8]
00492a88: bl       #0x50e398
00492a8c: b        #0x492a64
00492a90: bl       #0x30e310
00492a94: subseq   r2, r0, r4, lsl r1
00492a98: andeq    r4, r0, ip, lsr #1
00492a9c: andeq    r2, r0, ip, lsr sp

# _ZN10AnimatedFX11SyncIrrDataEb
00492aa0: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00492aa4: ldr      r3, [r0, #0x2c]
00492aa8: ldr      r8, [pc, #0x3d0]
00492aac: sub      sp, sp, #0x74
00492ab0: cmp      r3, #0
00492ab4: mov      r4, r0
00492ab8: add      r8, pc, r8
00492abc: beq      #0x492c00
00492ac0: ldrb     r3, [r0, #0x30]
00492ac4: ldr      r7, [r0, #0x34]
00492ac8: ldr      r6, [r0, #0x38]
00492acc: cmp      r3, #0
00492ad0: ldr      r5, [r0, #0x3c]
00492ad4: movne    sb, #1
00492ad8: beq      #0x492c08
00492adc: ldr      r2, [r4, #0x50]
00492ae0: cmp      r2, #0
00492ae4: beq      #0x492af0
00492ae8: cmp      r1, #0
00492aec: bne      #0x492c18
00492af0: ldr      r0, [r4, #0x28]
00492af4: cmp      r0, #0
00492af8: beq      #0x492bc8
00492afc: bl       #0x3935dc
00492b00: mov      sl, r0
00492b04: ldr      r1, [sl]
00492b08: mov      r0, r7
00492b0c: bl       #0x30eba4
00492b10: ldr      r1, [sl, #4]
00492b14: mov      r7, r0
00492b18: mov      r0, r6
00492b1c: bl       #0x30eba4
00492b20: ldr      r1, [sl, #8]
00492b24: mov      r6, r0
00492b28: mov      r0, r5
00492b2c: bl       #0x30eba4
00492b30: cmp      sb, #0
00492b34: mov      r5, r0
00492b38: beq      #0x492bbc
00492b3c: ldr      r3, [r4, #0x28]
00492b40: ldr      r2, [r3, #0x2d8]
00492b44: cmp      r2, #0
00492b48: beq      #0x492e30
00492b4c: ldr      r3, [r2, #8]
00492b50: mov      r0, r3
00492b54: ldr      r3, [r3]
00492b58: mov      lr, pc
00492b5c: ldr      pc, [r3, #0x38]
00492b60: mov      r1, r0
00492b64: add      r0, sp, #0x64
00492b68: bl       #0x432bbc
00492b6c: movw     r1, #0xfa35
00492b70: ldr      r0, [sp, #0x68]
00492b74: movt     r1, #0x3c8e
00492b78: bl       #0x30ed6c
00492b7c: movw     r1, #0xfa35
00492b80: mov      fp, r0
00492b84: movt     r1, #0x3c8e
00492b88: ldr      r0, [sp, #0x6c]
00492b8c: bl       #0x30ed6c
00492b90: movw     r1, #0xfa35
00492b94: mov      sl, r0
00492b98: movt     r1, #0x3c8e
00492b9c: ldr      r0, [sp, #0x64]
00492ba0: bl       #0x30ed6c
00492ba4: str      fp, [r4, #0x44]
00492ba8: str      r0, [r4, #0x40]
00492bac: str      sl, [r4, #0x48]
00492bb0: ldr      r0, [r4, #0x2c]
00492bb4: add      r1, r4, #0x40
00492bb8: bl       #0x472874
00492bbc: ldrb     r3, [r4, #0x32]
00492bc0: cmp      r3, #0
00492bc4: bne      #0x492c50
00492bc8: ldrb     r3, [r4, #0x4c]
00492bcc: cmp      r3, #0
00492bd0: bne      #0x492cd4
00492bd4: ldr      ip, [r4, #0x28]
00492bd8: cmp      ip, #0
00492bdc: beq      #0x492cf0
00492be0: cmp      sb, #0
00492be4: beq      #0x492ce4
00492be8: ldr      r0, [r4, #0x2c]
00492bec: add      r1, sp, #0x1c
00492bf0: str      r7, [sp, #0x1c]
00492bf4: str      r6, [sp, #0x20]
00492bf8: str      r5, [sp, #0x24]
00492bfc: bl       #0x470c24
00492c00: add      sp, sp, #0x74
00492c04: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00492c08: cmp      r1, #0
00492c0c: moveq    sb, r1
00492c10: ldrbne   sb, [r0, #0x31]
00492c14: b        #0x492adc
00492c18: ldrb     r3, [r4, #0x31]
00492c1c: cmp      r3, #0
00492c20: beq      #0x492af0
00492c24: ldr      r3, [r2, #0x2c]
00492c28: cmp      r3, #0
00492c2c: beq      #0x492c40
00492c30: mov      r2, r3
00492c34: ldr      r3, [r2, #0x2c]
00492c38: cmp      r3, #0
00492c3c: bne      #0x492c30
00492c40: ldr      r3, [r2, #4]
00492c44: cmp      r3, #0
00492c48: movgt    sb, #0
00492c4c: b        #0x492af0
00492c50: ldr      r3, [pc, #0x22c]
00492c54: ldr      r2, [r4, #0x28]
00492c58: ldr      r3, [r8, r3]
00492c5c: mov      r0, r2
00492c60: ldr      ip, [r3, #8]
00492c64: ldr      r1, [r3]
00492c68: ldr      r3, [r3, #4]
00492c6c: str      ip, [sp, #0x60]
00492c70: str      r1, [sp, #0x58]
00492c74: str      r3, [sp, #0x5c]
00492c78: ldr      r3, [r2]
00492c7c: mov      lr, pc
00492c80: ldr      pc, [r3, #0xa4]
00492c84: cmn      r0, #1
00492c88: beq      #0x492e10
00492c8c: ldr      r3, [r4, #0x28]
00492c90: add      r0, sp, #0x4c
00492c94: mov      r1, r3
00492c98: ldr      r3, [r3]
00492c9c: mov      lr, pc
00492ca0: ldr      pc, [r3, #0xa0]
00492ca4: ldr      r3, [sp, #0x4c]
00492ca8: str      r3, [sp, #0x58]
00492cac: ldr      r3, [sp, #0x50]
00492cb0: str      r3, [sp, #0x5c]
00492cb4: ldr      r3, [sp, #0x54]
00492cb8: str      r3, [sp, #0x60]
00492cbc: ldr      r0, [r4, #0x2c]
00492cc0: add      r1, sp, #0x58
00492cc4: bl       #0x4727ac
00492cc8: ldrb     r3, [r4, #0x4c]
00492ccc: cmp      r3, #0
00492cd0: beq      #0x492bd4
00492cd4: ldr      r0, [r4, #0x2c]
00492cd8: add      r1, r4, #0x40
00492cdc: bl       #0x472874
00492ce0: b        #0x492be8
00492ce4: ldrb     r3, [r4, #0x31]
00492ce8: cmp      r3, #0
00492cec: bne      #0x492be8
00492cf0: mov      sl, #0
00492cf4: cmp      ip, #0
00492cf8: str      sl, [sp, #0x40]
00492cfc: str      sl, [sp, #0x44]
00492d00: str      sl, [sp, #0x48]
00492d04: beq      #0x492e4c
00492d08: ldr      r0, [ip, #0x1ec]
00492d0c: str      r0, [sp, #0x40]
00492d10: ldr      sb, [ip, #0x1f0]
00492d14: mov      r1, r0
00492d18: str      sb, [sp, #0x44]
00492d1c: ldr      fp, [ip, #0x1f4]
00492d20: str      fp, [sp, #0x48]
00492d24: bl       #0x30ed6c
00492d28: mov      r1, sb
00492d2c: mov      r3, r0
00492d30: mov      r0, sb
00492d34: str      r3, [sp, #0x14]
00492d38: bl       #0x30ed6c
00492d3c: ldr      r3, [sp, #0x14]
00492d40: mov      r1, r0
00492d44: mov      r0, r3
00492d48: bl       #0x30eba4
00492d4c: mov      r1, fp
00492d50: mov      sb, r0
00492d54: mov      r0, fp
00492d58: bl       #0x30ed6c
00492d5c: mov      r1, r0
00492d60: mov      r0, sb
00492d64: bl       #0x30eba4
00492d68: mov      r1, sl
00492d6c: bl       #0x30df8c
00492d70: cmp      r0, #0
00492d74: beq      #0x492be8
00492d78: ldr      r3, [pc, #0x108]
00492d7c: mov      ip, #0
00492d80: mov      r2, ip
00492d84: ldr      r0, [r8, r3]
00492d88: add      r1, sp, #0x28
00492d8c: add      r3, sp, #0x40
00492d90: str      ip, [sp]
00492d94: str      ip, [sp, #4]
00492d98: str      ip, [sp, #8]
00492d9c: str      r7, [sp, #0x28]
00492da0: str      r6, [sp, #0x2c]
00492da4: str      r5, [sp, #0x30]
00492da8: bl       #0x525508
00492dac: ldr      r0, [sp, #0x40]
00492db0: mov      r1, r0
00492db4: bl       #0x30ed6c
00492db8: mov      r8, r0
00492dbc: ldr      r0, [sp, #0x44]
00492dc0: mov      r1, r0
00492dc4: bl       #0x30ed6c
00492dc8: mov      r1, r0
00492dcc: mov      r0, r8
00492dd0: bl       #0x30eba4
00492dd4: mov      r8, r0
00492dd8: ldr      r0, [sp, #0x48]
00492ddc: mov      r1, r0
00492de0: bl       #0x30ed6c
00492de4: mov      r1, r0
00492de8: mov      r0, r8
00492dec: bl       #0x30eba4
00492df0: mov      r1, sl
00492df4: bl       #0x30df8c
00492df8: cmp      r0, #0
00492dfc: movne    r3, #0x3f800000
00492e00: strne    sl, [sp, #0x44]
00492e04: strne    sl, [sp, #0x40]
00492e08: strne    r3, [sp, #0x48]
00492e0c: b        #0x492be8
00492e10: ldr      r3, [r4, #0x28]
00492e14: ldr      r2, [r3, #0x120]
00492e18: str      r2, [sp, #0x58]
00492e1c: ldr      r2, [r3, #0x124]
00492e20: str      r2, [sp, #0x5c]
00492e24: ldr      r3, [r3, #0x128]
00492e28: str      r3, [sp, #0x60]
00492e2c: b        #0x492cbc
00492e30: ldr      r2, [r3, #0x16c]
00492e34: str      r2, [r4, #0x40]
00492e38: ldr      r2, [r3, #0x170]
00492e3c: str      r2, [r4, #0x44]
00492e40: ldr      r3, [r3, #0x174]
00492e44: str      r3, [r4, #0x48]
00492e48: b        #0x492bb0
00492e4c: ldr      r3, [pc, #0x34]
00492e50: mov      r2, ip
00492e54: add      r1, sp, #0x34
00492e58: ldr      r0, [r8, r3]
00492e5c: add      r3, sp, #0x40
00492e60: str      r7, [sp, #0x34]
00492e64: str      r6, [sp, #0x38]
00492e68: str      r5, [sp, #0x3c]
00492e6c: str      ip, [sp]
00492e70: str      ip, [sp, #4]
00492e74: str      ip, [sp, #8]
00492e78: bl       #0x525508
00492e7c: b        #0x492be8
00492e80: ldrsbeq  r1, [r0], #-0xf8
00492e84: andeq    r3, r0, ip, lsr #30
00492e88: andeq    r1, r0, r4, lsl #4

# _ZN10AnimatedFX9SetAnimFXEbbbfiPN15VisualFXManager13AnimFXSetDataEPFvPS_S2_E
00492e8c: push     {r4, r5, r6, lr}
00492e90: mov      r4, r0
00492e94: mov      r5, r2
00492e98: strb     r1, [r0, #0x30]
00492e9c: mov      r1, #0
00492ea0: mov      r6, r3
00492ea4: bl       #0x492aa0
00492ea8: mov      r0, r4
00492eac: mov      r1, #1
00492eb0: strb     r5, [r4, #0x31]
00492eb4: bl       #0x492aa0
00492eb8: mov      r0, r4
00492ebc: mov      r1, #0
00492ec0: strb     r6, [r4, #0x32]
00492ec4: bl       #0x492aa0
00492ec8: mov      r0, r4
00492ecc: ldr      r1, [sp, #0x10]
00492ed0: bl       #0x4924b0
00492ed4: ldr      r3, [sp, #0x18]
00492ed8: str      r3, [r4, #0x50]
00492edc: ldr      r3, [sp, #0x14]
00492ee0: str      r3, [r4, #0x14]
00492ee4: ldr      r3, [sp, #0x1c]
00492ee8: str      r3, [r4, #4]
00492eec: pop      {r4, r5, r6, pc}

# _ZN10AnimatedFX17GetAnimControllerEv
00492550: ldr      r0, [r0, #0x2c]
00492554: cmp      r0, #0
00492558: ldrne    r0, [r0, #0x38]
0049255c: bx       lr

# _ZN15VisualFXManager15_HandleSequenceEP10AnimatedFXPNS_13AnimFXSetDataEb
00496144: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00496148: sub      sp, sp, #0x7c
0049614c: str      r1, [sp, #0x14]
00496150: cmp      r3, #0
00496154: mov      r8, r2
00496158: ldrne    r2, [r2, #0x2c]
0049615c: ldreq    r7, [r8, #8]
00496160: mov      r6, r0
00496164: ldrne    r7, [r2, #8]
00496168: mov      r2, #0x18
0049616c: mul      r7, r2, r7
00496170: ldr      r2, [r0, #0x1c]
00496174: ldr      r1, [r2, r7]
00496178: add      r7, r2, r7
0049617c: ldr      r2, [r1, #0x14]
00496180: cmp      r2, #1
00496184: beq      #0x4961e0
00496188: cmp      r3, #0
0049618c: beq      #0x4961bc
00496190: ldr      r3, [r8, #0x2c]
00496194: ldr      r2, [r3, #0x2c]
00496198: cmp      r2, #0
0049619c: beq      #0x4961b4
004961a0: mov      r3, #1
004961a4: strb     r3, [r2]
004961a8: ldr      r1, [sp, #0x14]
004961ac: ldr      r2, [r8, #0x2c]
004961b0: bl       #0x496144
004961b4: add      sp, sp, #0x7c
004961b8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
004961bc: ldr      r2, [r8, #0x2c]
004961c0: cmp      r2, #0
004961c4: beq      #0x4961b4
004961c8: mov      r3, #1
004961cc: strb     r3, [r2]
004961d0: ldr      r1, [sp, #0x14]
004961d4: mov      r2, r8
004961d8: bl       #0x496144
004961dc: b        #0x4961b4
004961e0: add      fp, sp, #0x6c
004961e4: add      r2, sp, #4
004961e8: add      r3, fp, #4
004961ec: str      r2, [sp, #0x1c]
004961f0: str      r3, [sp, #0x20]
004961f4: add      ip, sp, #0x34
004961f8: mov      sl, r7
004961fc: ldr      r5, [sl, #0x10]!
00496200: add      r2, r2, #4
00496204: str      ip, [sp, #0x18]
00496208: add      r3, r3, #4
0049620c: add      ip, sp, #0x4c
00496210: str      r8, [sp, #0x2c]
00496214: add      sb, sp, #0x64
00496218: str      r2, [sp, #0x28]
0049621c: str      ip, [sp, #0x24]
00496220: mov      r8, r3
00496224: cmp      r5, sl
00496228: beq      #0x4961b4
0049622c: ldr      r4, [r5, #8]
00496230: cmp      r4, #0
00496234: beq      #0x4962f8
00496238: ldrb     r3, [r4]
0049623c: cmp      r3, #0
00496240: beq      #0x4962f8
00496244: mov      r2, #0
00496248: strb     r2, [r4]
0049624c: ldr      r2, [r7]
00496250: ldr      r3, [r4, #4]
00496254: ldr      r2, [r2, #0xc]
00496258: add      r3, r3, #1
0049625c: cmp      r3, r2
00496260: blt      #0x496328
00496264: ldr      r3, [r4, #0xc]
00496268: cmp      r3, #0
0049626c: movle    r2, #0
00496270: movgt    r2, #1
00496274: cmn      r3, #1
00496278: movne    r1, #0
0049627c: moveq    r1, #1
00496280: orrs     r1, r2, r1
00496284: beq      #0x496300
00496288: cmp      r2, #0
0049628c: subne    r3, r3, #1
00496290: mov      lr, #0
00496294: strne    r3, [r4, #0xc]
00496298: str      lr, [r4, #4]
0049629c: mov      r1, r7
004962a0: ldr      r0, [sp, #0x18]
004962a4: bl       #0x493924
004962a8: ldr      r2, [sp, #0x18]
004962ac: mov      r0, sb
004962b0: mov      r1, r6
004962b4: mov      r3, r4
004962b8: bl       #0x4933e4
004962bc: ldr      r0, [sp, #0x18]
004962c0: bl       #0x4940d8
004962c4: ldr      r2, [sp, #0x20]
004962c8: ldr      ip, [fp]
004962cc: ldr      lr, [r2]
004962d0: ldm      sb, {r2, r3}
004962d4: str      ip, [sp]
004962d8: ldr      ip, [sp, #0x1c]
004962dc: mov      r1, r4
004962e0: mov      r0, r6
004962e4: str      lr, [ip]
004962e8: ldr      lr, [r8]
004962ec: ldr      ip, [sp, #0x28]
004962f0: str      lr, [ip]
004962f4: bl       #0x495f74
004962f8: ldr      r5, [r5]
004962fc: b        #0x496224
00496300: ldr      r3, [r4, #0x2c]
00496304: cmp      r3, #0
00496308: beq      #0x4962f8
0049630c: mov      r0, r6
00496310: ldr      r1, [sp, #0x14]
00496314: ldr      r2, [sp, #0x2c]
00496318: mov      r3, #1
0049631c: bl       #0x496144
00496320: ldr      r5, [r5]
00496324: b        #0x496224
00496328: str      r3, [r4, #4]
0049632c: mov      r1, r7
00496330: ldr      r0, [sp, #0x24]
00496334: bl       #0x493924
00496338: mov      r3, r4
0049633c: mov      r0, sb
00496340: mov      r1, r6
00496344: ldr      r2, [sp, #0x24]
00496348: bl       #0x4933e4
0049634c: ldr      r0, [sp, #0x24]
00496350: bl       #0x4940d8
00496354: ldr      r3, [sp, #0x20]
00496358: ldr      ip, [fp]
0049635c: ldr      lr, [r3]
00496360: b        #0x4962d0

# _ZN15VisualFXManager16__Anim_EndOfLoopEP10AnimatedFXPNS_13AnimFXSetDataE
004963d0: push     {r4, r5, r6, lr}
004963d4: ldr      r4, [pc, #0x50]
004963d8: subs     r2, r1, #0
004963dc: mov      r5, r0
004963e0: add      r4, pc, r4
004963e4: beq      #0x496424
004963e8: ldr      r6, [pc, #0x40]
004963ec: mov      r1, r0
004963f0: ldr      r0, [r4, r6]
004963f4: bl       #0x496364
004963f8: ldr      r4, [r4, r6]
004963fc: add      r6, r4, #8
00496400: mov      r0, r6
00496404: bl       #0x494ab4
00496408: str      r5, [r0, #8]
0049640c: ldr      r3, [r4, #0xc]
00496410: str      r6, [r0]
00496414: str      r3, [r0, #4]
00496418: str      r0, [r3]
0049641c: str      r0, [r4, #0xc]
00496420: pop      {r4, r5, r6, pc}
00496424: ldr      r6, [pc, #4]
00496428: b        #0x4963f8
0049642c: strheq   lr, [pc], #-0x60
00496430: andeq    r1, r0, r8, lsl #22
