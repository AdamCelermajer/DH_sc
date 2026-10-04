
# _ZN17MenuFlash2DCamera12SetLimitClipEPN7gameswf9characterE
0042cf30: push     {r4, r5, lr}
0042cf34: cmp      r1, #0
0042cf38: mov      r4, r0
0042cf3c: sub      sp, sp, #0x14
0042cf40: str      r1, [r4, #8]
0042cf44: beq      #0x42cf60
0042cf48: mov      r0, sp
0042cf4c: bl       #0x416a7c
0042cf50: mov      r5, sp
0042cf54: add      ip, r4, #0xc
0042cf58: ldm      r5, {r0, r1, r2, r3}
0042cf5c: stm      ip, {r0, r1, r2, r3}
0042cf60: add      sp, sp, #0x14
0042cf64: pop      {r4, r5, pc}

# _ZN17MenuFlash2DCamera6UpdateEv
0042cd84: push     {r4, r5, r6, r7, r8, lr}
0042cd88: ldr      r2, [r0, #0x24]
0042cd8c: ldr      r1, [r0, #0x2c]
0042cd90: ldr      r3, [pc, #0x190]
0042cd94: sub      sp, sp, #8
0042cd98: cmp      r2, r1
0042cd9c: mov      r4, r0
0042cda0: add      r3, pc, r3
0042cda4: bge      #0x42cefc
0042cda8: movw     r0, #0x6667
0042cdac: rsb      r1, r2, r1
0042cdb0: movt     r0, #0x6666
0042cdb4: smull    ip, r0, r0, r1
0042cdb8: asr      r1, r1, #0x1f
0042cdbc: rsb      r1, r1, r0, asr #2
0042cdc0: add      r2, r2, #1
0042cdc4: add      r2, r2, r1
0042cdc8: str      r2, [r4, #0x24]
0042cdcc: ldr      r2, [r4, #0x28]
0042cdd0: ldr      r1, [r4, #0x30]
0042cdd4: cmp      r2, r1
0042cdd8: bge      #0x42ced0
0042cddc: movw     r0, #0x6667
0042cde0: rsb      r1, r2, r1
0042cde4: movt     r0, #0x6666
0042cde8: smull    ip, r0, r0, r1
0042cdec: asr      r1, r1, #0x1f
0042cdf0: rsb      r1, r1, r0, asr #2
0042cdf4: add      r2, r2, #1
0042cdf8: add      r2, r2, r1
0042cdfc: str      r2, [r4, #0x28]
0042ce00: ldr      r1, [pc, #0x124]
0042ce04: ldr      r2, [r4, #8]
0042ce08: ldr      r3, [r3, r1]
0042ce0c: cmp      r2, #0
0042ce10: ldr      r3, [r3, #0x10]
0042ce14: ldr      r3, [r3, #0x10]
0042ce18: ldr      r3, [r3, #0xcc]
0042ce1c: ldr      r3, [r3, #-4]
0042ce20: ldr      r5, [r3, #0x10]
0042ce24: ldr      r6, [r3, #0xc]
0042ce28: beq      #0x42ce94
0042ce2c: ldr      r0, [r4, #0xc]
0042ce30: bl       #0x30e4cc
0042ce34: ldr      r8, [r4, #0x24]
0042ce38: add      r3, r0, r8
0042ce3c: cmp      r3, #0
0042ce40: rsbgt    r8, r0, #0
0042ce44: strgt    r8, [r4, #0x24]
0042ce48: ldr      r0, [r4, #0x14]
0042ce4c: bl       #0x30e4cc
0042ce50: ldr      r7, [r4, #0x28]
0042ce54: add      r3, r0, r7
0042ce58: cmp      r3, #0
0042ce5c: rsbgt    r7, r0, #0
0042ce60: strgt    r7, [r4, #0x28]
0042ce64: ldr      r0, [r4, #0x10]
0042ce68: bl       #0x30e4cc
0042ce6c: add      r8, r0, r8
0042ce70: cmp      r6, r8
0042ce74: rsbgt    r0, r0, r6
0042ce78: strgt    r0, [r4, #0x24]
0042ce7c: ldr      r0, [r4, #0x18]
0042ce80: bl       #0x30e4cc
0042ce84: add      r7, r0, r7
0042ce88: cmp      r5, r7
0042ce8c: rsbgt    r0, r0, r5
0042ce90: strgt    r0, [r4, #0x28]
0042ce94: mov      r1, #0
0042ce98: ldr      r0, [r4, #4]
0042ce9c: mov      r2, r1
0042cea0: mov      r3, r6
0042cea4: str      r5, [sp]
0042cea8: bl       #0x7a9bac
0042ceac: ldr      r2, [r4, #0x28]
0042ceb0: ldr      r0, [r4, #4]
0042ceb4: ldr      r1, [r4, #0x24]
0042ceb8: mov      ip, #0
0042cebc: mov      r3, r6
0042cec0: stm      sp, {r5, ip}
0042cec4: bl       #0x7a9b30
0042cec8: add      sp, sp, #8
0042cecc: pop      {r4, r5, r6, r7, r8, pc}
0042ced0: ble      #0x42ce00
0042ced4: movw     r0, #0x6667
0042ced8: rsb      r1, r1, r2
0042cedc: movt     r0, #0x6666
0042cee0: smull    ip, r0, r0, r1
0042cee4: asr      r1, r1, #0x1f
0042cee8: rsb      r1, r1, r0, asr #2
0042ceec: mvn      r1, r1
0042cef0: add      r2, r1, r2
0042cef4: str      r2, [r4, #0x28]
0042cef8: b        #0x42ce00
0042cefc: ble      #0x42cdcc
0042cf00: movw     r0, #0x6667
0042cf04: rsb      r1, r1, r2
0042cf08: movt     r0, #0x6666
0042cf0c: smull    ip, r0, r0, r1
0042cf10: asr      r1, r1, #0x1f
0042cf14: rsb      r1, r1, r0, asr #2
0042cf18: mvn      r1, r1
0042cf1c: add      r2, r1, r2
0042cf20: str      r2, [r4, #0x24]
0042cf24: b        #0x42cdcc
0042cf28: ldrsheq  r7, [r6], #-0xc0
0042cf2c: strdeq   r3, r4, [r0], -r4

# _ZN7gameswf4root20set_display_viewportEiiii
00775d38: push     {r4, lr}
00775d3c: ldr      ip, [r0, #0x14]
00775d40: sub      sp, sp, #8
00775d44: cmp      ip, r1
00775d48: ldr      ip, [sp, #0x10]
00775d4c: beq      #0x775d78
00775d50: str      ip, [r0, #0x20]
00775d54: str      ip, [sp]
00775d58: str      r1, [r0, #0x14]
00775d5c: mov      ip, #0
00775d60: str      r2, [r0, #0x18]
00775d64: str      r3, [r0, #0x1c]
00775d68: str      ip, [sp, #4]
00775d6c: bl       #0x7755f4
00775d70: add      sp, sp, #8
00775d74: pop      {r4, pc}
00775d78: ldr      r4, [r0, #0x18]
00775d7c: cmp      r4, r2
00775d80: bne      #0x775d50
00775d84: ldr      r4, [r0, #0x1c]
00775d88: cmp      r4, r3
00775d8c: bne      #0x775d50
00775d90: ldr      r4, [r0, #0x20]
00775d94: cmp      r4, ip
00775d98: bne      #0x775d50
00775d9c: b        #0x775d70

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

# _ZN8RenderFX9SetBoundsEiiiiN7gameswf10scale_modeE
007a9b30: push     {r4, r5, r6, r7, r8, sl, lr}
007a9b34: sub      sp, sp, #0xc
007a9b38: ldr      r0, [r0, #0x38]
007a9b3c: mov      sl, r1
007a9b40: mov      r8, r2
007a9b44: mov      r7, r3
007a9b48: ldr      r6, [sp, #0x28]
007a9b4c: ldr      r5, [sp, #0x2c]
007a9b50: bl       #0x76d5b4
007a9b54: subs     r4, r0, #0
007a9b58: beq      #0x7a9b8c
007a9b5c: bl       #0x759c64
007a9b60: mov      r0, r4
007a9b64: mov      r1, sl
007a9b68: mov      r2, r8
007a9b6c: mov      r3, r7
007a9b70: str      r6, [sp]
007a9b74: str      r5, [sp, #4]
007a9b78: bl       #0x7755f4
007a9b7c: mov      r0, r4
007a9b80: add      sp, sp, #0xc
007a9b84: pop      {r4, r5, r6, r7, r8, sl, lr}
007a9b88: b        #0x75a240
007a9b8c: mov      r1, sl
007a9b90: mov      r2, r8
007a9b94: mov      r3, r7
007a9b98: str      r6, [sp, #0x28]
007a9b9c: str      r5, [sp, #0x2c]
007a9ba0: add      sp, sp, #0xc
007a9ba4: pop      {r4, r5, r6, r7, r8, sl, lr}
007a9ba8: b        #0x7755f4

# _ZN17MenuFlash2DCameraC1EP6MenuFX
0042ccd0: ldr      r2, [pc, #0x68]
0042ccd4: ldr      ip, [pc, #0x68]
0042ccd8: ldr      r3, [pc, #0x68]
0042ccdc: add      r2, pc, r2
0042cce0: push     {r4, r5}
0042cce4: ldr      ip, [r2, ip]
0042cce8: ldr      r4, [r2, r3]
0042ccec: str      r1, [r0, #4]
0042ccf0: add      r5, ip, #8
0042ccf4: mov      ip, #0
0042ccf8: str      r5, [r0]
0042ccfc: str      ip, [r0, #8]
0042cd00: ldr      r1, [r4]
0042cd04: ldr      r4, [pc, #0x40]
0042cd08: add      r1, r1, r1, lsr #31
0042cd0c: ldr      r2, [r2, r4]
0042cd10: asr      r1, r1, #1
0042cd14: str      r1, [r0, #0x1c]
0042cd18: ldr      r2, [r2]
0042cd1c: str      ip, [r0, #0x30]
0042cd20: str      ip, [r0, #0x24]
0042cd24: add      r2, r2, r2, lsr #31
0042cd28: str      ip, [r0, #0x28]
0042cd2c: asr      r2, r2, #1
0042cd30: str      r2, [r0, #0x20]
0042cd34: str      ip, [r0, #0x2c]
0042cd38: pop      {r4, r5}
0042cd3c: bx       lr
0042cd40: ldrheq   r7, [r6], #-0xd4
0042cd44: andeq    r0, r0, r4, ror #28
0042cd48: andeq    r2, r0, r4, asr #11
0042cd4c: strdeq   r2, r3, [r0], -r8

# _ZN7gameswf4root18set_display_boundsEiiiiNS_10scale_modeE
007755f4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
007755f8: ldr      r5, [pc, #0x718]
007755fc: ldr      r6, [pc, #0x718]
00775600: ldr      lr, [pc, #0x718]
00775604: add      r5, pc, r5
00775608: ldr      ip, [r5, r6]
0077560c: ldr      r8, [r5, lr]
00775610: mov      r4, r0
00775614: ldr      r0, [ip]
00775618: ldr      ip, [r8]
0077561c: sub      sp, sp, #0xd4
00775620: str      r0, [sp, #0xcc]
00775624: mov      r7, r1
00775628: mov      r0, ip
0077562c: ldr      r1, [ip]
00775630: mov      sl, r2
00775634: mov      sb, r3
00775638: ldr      fp, [sp, #0xf8]
0077563c: mov      lr, pc
00775640: ldr      pc, [r1, #0xac]
00775644: cmp      r0, #0
00775648: bne      #0x775ba4
0077564c: mov      r0, sb
00775650: bl       #0x30e964
00775654: ldr      r8, [r4, #0xc]
00775658: mov      r3, r0
0077565c: ldr      r1, [r8, #0xb4]
00775660: ldr      r0, [r8, #0xb8]
00775664: str      r3, [sp]
00775668: bl       #0x30e3ac
0077566c: mov      r1, #0x41000000
00775670: add      r1, r1, #0xa00000
00775674: bl       #0x30ec94
00775678: ldr      r3, [sp]
0077567c: mov      r1, r0
00775680: mov      r0, r3
00775684: bl       #0x30ec94
00775688: mov      r3, r0
0077568c: mov      r0, fp
00775690: str      r3, [sp]
00775694: bl       #0x30e964
00775698: ldr      r1, [r8, #0xbc]
0077569c: mov      r2, r0
007756a0: ldr      r0, [r8, #0xc0]
007756a4: str      r2, [sp, #4]
007756a8: bl       #0x30e3ac
007756ac: mov      r1, #0x41000000
007756b0: add      r1, r1, #0xa00000
007756b4: bl       #0x30ec94
007756b8: ldr      r2, [sp, #4]
007756bc: mov      r1, r0
007756c0: mov      r0, r2
007756c4: bl       #0x30ec94
007756c8: ldr      r3, [sp]
007756cc: mov      r2, #1
007756d0: str      r2, [sp, #0xc]
007756d4: mov      r1, r3
007756d8: bl       #0x30ec94
007756dc: ldr      r3, [sp, #0xfc]
007756e0: str      r0, [sp, #8]
007756e4: cmp      r3, #1
007756e8: beq      #0x775b6c
007756ec: cmp      r3, #2
007756f0: beq      #0x775b04
007756f4: ldr      r3, [r4, #0x24]
007756f8: cmp      r3, r7
007756fc: beq      #0x775b44
00775700: ldr      r2, [sp, #0xc]
00775704: str      r7, [r4, #0x24]
00775708: str      sl, [r4, #0x28]
0077570c: cmp      r2, #0
00775710: str      sb, [r4, #0x2c]
00775714: str      fp, [r4, #0x30]
00775718: bne      #0x775ab8
0077571c: mov      r0, sb
00775720: bl       #0x30e964
00775724: ldr      r1, [r8, #0xbc]
00775728: mov      r7, r0
0077572c: ldr      r0, [r8, #0xc0]
00775730: bl       #0x30e3ac
00775734: mov      r1, #0x41000000
00775738: add      r1, r1, #0xa00000
0077573c: bl       #0x30ec94
00775740: mov      r1, r0
00775744: mov      r0, r7
00775748: bl       #0x30ec94
0077574c: mov      sl, r0
00775750: mov      r0, fp
00775754: bl       #0x30e964
00775758: ldr      r1, [r8, #0xb4]
0077575c: mov      r7, r0
00775760: ldr      r0, [r8, #0xb8]
00775764: bl       #0x30e3ac
00775768: mov      r1, #0x41000000
0077576c: add      r1, r1, #0xa00000
00775770: bl       #0x30ec94
00775774: mov      r1, r0
00775778: mov      r0, r7
0077577c: bl       #0x30ec94
00775780: mov      r7, r0
00775784: mov      r1, r7
00775788: mov      r0, sl
0077578c: bl       #0x30e70c
00775790: ldr      r3, [r4, #0xcc]
00775794: cmp      r0, #0
00775798: moveq    r7, sl
0077579c: cmp      r3, #0
007757a0: str      r7, [r4, #0x34]
007757a4: beq      #0x775a9c
007757a8: ldr      r0, [r4, #0xc8]
007757ac: ldrb     r3, [r0, #4]
007757b0: cmp      r3, #0
007757b4: beq      #0x775c74
007757b8: mov      r3, #0
007757bc: ldr      r0, [r4, #0x18]
007757c0: str      r3, [sp, #0x64]
007757c4: str      r3, [sp, #0x60]
007757c8: bl       #0x30e964
007757cc: mov      r7, r0
007757d0: ldr      r0, [r4, #0x20]
007757d4: bl       #0x30e964
007757d8: mov      r1, r0
007757dc: mov      r0, r7
007757e0: bl       #0x30eba4
007757e4: mov      r7, r0
007757e8: ldr      r0, [r4, #0x14]
007757ec: bl       #0x30e964
007757f0: mov      r8, r0
007757f4: ldr      r0, [r4, #0x1c]
007757f8: bl       #0x30e964
007757fc: mov      r1, r0
00775800: mov      r0, r8
00775804: bl       #0x30eba4
00775808: add      r1, sp, #0x60
0077580c: str      r0, [sp, #0x58]
00775810: mov      r0, r4
00775814: str      r7, [sp, #0x5c]
00775818: bl       #0x773dc0
0077581c: mov      r0, r4
00775820: add      r1, sp, #0x58
00775824: bl       #0x773dc0
00775828: ldr      r8, [r4, #0xcc]
0077582c: cmp      r8, #0
00775830: beq      #0x775844
00775834: ldr      r0, [r4, #0xc8]
00775838: ldrb     r3, [r0, #4]
0077583c: cmp      r3, #0
00775840: beq      #0x775cec
00775844: mov      r1, #0
00775848: mov      r0, #0x38
0077584c: bl       #0x752ba8
00775850: mov      r1, r8
00775854: mov      r7, r0
00775858: bl       #0x76b820
0077585c: ldr      r1, [pc, #0x4c0]
00775860: ldr      r3, [r7]
00775864: add      sb, sp, #0xb8
00775868: add      r1, pc, r1
0077586c: mov      r0, sb
00775870: ldr      sl, [r3, #0x1c]
00775874: bl       #0x413a7c
00775878: mov      r3, #0
0077587c: ldr      r0, [sp, #0x60]
00775880: strb     r3, [sp, #0x44]
00775884: mov      r3, #2
00775888: strb     r3, [sp, #0x45]
0077588c: bl       #0x30e8a4
00775890: strd     r0, r1, [sp, #0x50]
00775894: ldr      r3, [sp, #0x50]
00775898: add      r8, sp, #0x44
0077589c: mov      r1, sb
007758a0: str      r3, [sp, #0x48]
007758a4: ldr      r3, [sp, #0x54]
007758a8: mov      r2, r8
007758ac: mov      r0, r7
007758b0: str      r3, [r8, #8]
007758b4: blx      sl
007758b8: mov      r0, r8
007758bc: bl       #0x797124
007758c0: ldrsb    r3, [sp, #0xb8]
007758c4: cmn      r3, #1
007758c8: beq      #0x775cac
007758cc: ldr      r1, [pc, #0x454]
007758d0: ldr      r3, [r7]
007758d4: add      sb, sp, #0xa4
007758d8: add      r1, pc, r1
007758dc: mov      r0, sb
007758e0: ldr      sl, [r3, #0x1c]
007758e4: bl       #0x413a7c
007758e8: mov      r3, #0
007758ec: ldr      r0, [sp, #0x64]
007758f0: strb     r3, [sp, #0x38]
007758f4: mov      r3, #2
007758f8: strb     r3, [sp, #0x39]
007758fc: bl       #0x30e8a4
00775900: strd     r0, r1, [sp, #0x50]
00775904: ldr      r3, [sp, #0x50]
00775908: add      r8, sp, #0x38
0077590c: mov      r1, sb
00775910: str      r3, [sp, #0x3c]
00775914: ldr      r3, [sp, #0x54]
00775918: mov      r2, r8
0077591c: mov      r0, r7
00775920: str      r3, [r8, #8]
00775924: blx      sl
00775928: mov      r0, r8
0077592c: bl       #0x797124
00775930: ldrsb    r3, [sp, #0xa4]
00775934: cmn      r3, #1
00775938: beq      #0x775c9c
0077593c: ldr      r1, [pc, #0x3e8]
00775940: ldr      r3, [r7]
00775944: add      sb, sp, #0x90
00775948: add      r1, pc, r1
0077594c: mov      r0, sb
00775950: ldr      sl, [r3, #0x1c]
00775954: bl       #0x413a7c
00775958: mov      r3, #0
0077595c: ldr      r0, [sp, #0x58]
00775960: strb     r3, [sp, #0x2c]
00775964: mov      r3, #2
00775968: strb     r3, [sp, #0x2d]
0077596c: bl       #0x30e8a4
00775970: strd     r0, r1, [sp, #0x50]
00775974: ldr      r3, [sp, #0x50]
00775978: add      r8, sp, #0x2c
0077597c: mov      r1, sb
00775980: str      r3, [sp, #0x30]
00775984: ldr      r3, [sp, #0x54]
00775988: mov      r2, r8
0077598c: mov      r0, r7
00775990: str      r3, [r8, #8]
00775994: blx      sl
00775998: mov      r0, r8
0077599c: bl       #0x797124
007759a0: ldrsb    r3, [sp, #0x90]
007759a4: cmn      r3, #1
007759a8: beq      #0x775cdc
007759ac: ldr      r1, [pc, #0x37c]
007759b0: ldr      r3, [r7]
007759b4: add      sb, sp, #0x7c
007759b8: add      r1, pc, r1
007759bc: mov      r0, sb
007759c0: ldr      sl, [r3, #0x1c]
007759c4: bl       #0x413a7c
007759c8: mov      r3, #0
007759cc: ldr      r0, [sp, #0x5c]
007759d0: strb     r3, [sp, #0x20]
007759d4: mov      r3, #2
007759d8: strb     r3, [sp, #0x21]
007759dc: bl       #0x30e8a4
007759e0: strd     r0, r1, [sp, #0x50]
007759e4: ldr      r3, [sp, #0x50]
007759e8: add      r8, sp, #0x20
007759ec: mov      r1, sb
007759f0: str      r3, [sp, #0x24]
007759f4: ldr      r3, [sp, #0x54]
007759f8: mov      r2, r8
007759fc: mov      r0, r7
00775a00: str      r3, [r8, #8]
00775a04: blx      sl
00775a08: mov      r0, r8
00775a0c: bl       #0x797124
00775a10: ldrsb    r3, [sp, #0x7c]
00775a14: cmn      r3, #1
00775a18: beq      #0x775ccc
00775a1c: mov      r3, #0
00775a20: strb     r3, [sp, #0x14]
00775a24: mov      r0, r7
00775a28: mov      r3, #5
00775a2c: strb     r3, [sp, #0x15]
00775a30: str      r7, [sp, #0x18]
00775a34: bl       #0x759c64
00775a38: ldr      r3, [r4, #0xcc]
00775a3c: cmp      r3, #0
00775a40: beq      #0x775a54
00775a44: ldr      r0, [r4, #0xc8]
00775a48: ldrb     r2, [r0, #4]
00775a4c: cmp      r2, #0
00775a50: beq      #0x775c4c
00775a54: ldr      sl, [r3, #0x34]
00775a58: ldr      r1, [pc, #0x2d4]
00775a5c: add      r8, sp, #0x68
00775a60: ldr      r3, [sl]
00775a64: add      r1, pc, r1
00775a68: mov      r0, r8
00775a6c: add      r4, sp, #0x14
00775a70: ldr      r7, [r3, #0x1c]
00775a74: bl       #0x413a7c
00775a78: mov      r0, sl
00775a7c: mov      r1, r8
00775a80: mov      r2, r4
00775a84: blx      r7
00775a88: ldrsb    r3, [sp, #0x68]
00775a8c: cmn      r3, #1
00775a90: beq      #0x775cbc
00775a94: mov      r0, r4
00775a98: bl       #0x797124
00775a9c: ldr      r3, [r5, r6]
00775aa0: ldr      r2, [sp, #0xcc]
00775aa4: ldr      r3, [r3]
00775aa8: cmp      r2, r3
00775aac: bne      #0x775d14
00775ab0: add      sp, sp, #0xd4
00775ab4: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00775ab8: mov      r0, sb
00775abc: bl       #0x30e964
00775ac0: ldr      r1, [r8, #0xb4]
00775ac4: mov      r7, r0
00775ac8: ldr      r0, [r8, #0xb8]
00775acc: bl       #0x30e3ac
00775ad0: mov      r1, #0x41000000
00775ad4: add      r1, r1, #0xa00000
00775ad8: bl       #0x30ec94
00775adc: mov      r1, r0
00775ae0: mov      r0, r7
00775ae4: bl       #0x30ec94
00775ae8: mov      sl, r0
00775aec: mov      r0, fp
00775af0: bl       #0x30e964
00775af4: ldr      r1, [r8, #0xbc]
00775af8: mov      r7, r0
00775afc: ldr      r0, [r8, #0xc0]
00775b00: b        #0x775764
00775b04: mov      r1, #0x3f800000
00775b08: bl       #0x30e4b4
00775b0c: cmp      r0, #0
00775b10: beq      #0x775b7c
00775b14: mov      r0, fp
00775b18: bl       #0x30e964
00775b1c: ldr      r1, [sp, #8]
00775b20: bl       #0x30ec94
00775b24: bl       #0x30e4cc
00775b28: rsb      r0, fp, r0
00775b2c: add      r3, r0, r0, lsr #31
00775b30: add      fp, fp, r0
00775b34: sub      sl, sl, r3, asr #1
00775b38: ldr      r3, [r4, #0x24]
00775b3c: cmp      r3, r7
00775b40: bne      #0x775700
00775b44: ldr      r3, [r4, #0x28]
00775b48: cmp      r3, sl
00775b4c: bne      #0x775700
00775b50: ldr      r3, [r4, #0x2c]
00775b54: cmp      r3, sb
00775b58: bne      #0x775700
00775b5c: ldr      r3, [r4, #0x30]
00775b60: cmp      r3, fp
00775b64: bne      #0x775700
00775b68: b        #0x775a9c
00775b6c: mov      r1, #0x3f800000
00775b70: bl       #0x30e4b4
00775b74: cmp      r0, #0
00775b78: beq      #0x775b14
00775b7c: mov      r0, sb
00775b80: bl       #0x30e964
00775b84: ldr      r1, [sp, #8]
00775b88: bl       #0x30ed6c
00775b8c: bl       #0x30e4cc
00775b90: rsb      r0, sb, r0
00775b94: add      r3, r0, r0, lsr #31
00775b98: add      sb, sb, r0
00775b9c: sub      r7, r7, r3, asr #1
00775ba0: b        #0x7756f4
00775ba4: ldr      r3, [r8]
00775ba8: mov      r0, r3
00775bac: ldr      r3, [r3]
00775bb0: mov      lr, pc
00775bb4: ldr      pc, [r3, #0xac]
00775bb8: cmp      r0, #2
00775bbc: beq      #0x77564c
00775bc0: mov      r0, sb
00775bc4: bl       #0x30e964
00775bc8: ldr      r8, [r4, #0xc]
00775bcc: mov      r3, r0
00775bd0: ldr      r1, [r8, #0xbc]
00775bd4: ldr      r0, [r8, #0xc0]
00775bd8: str      r3, [sp]
00775bdc: bl       #0x30e3ac
00775be0: mov      r1, #0x41000000
00775be4: add      r1, r1, #0xa00000
00775be8: bl       #0x30ec94
00775bec: ldr      r3, [sp]
00775bf0: mov      r1, r0
00775bf4: mov      r0, r3
00775bf8: bl       #0x30ec94
00775bfc: mov      r3, r0
00775c00: mov      r0, fp
00775c04: str      r3, [sp]
00775c08: bl       #0x30e964
00775c0c: ldr      r1, [r8, #0xb4]
00775c10: mov      r2, r0
00775c14: ldr      r0, [r8, #0xb8]
00775c18: str      r2, [sp, #4]
00775c1c: bl       #0x30e3ac
00775c20: mov      r1, #0x41000000
00775c24: add      r1, r1, #0xa00000
00775c28: bl       #0x30ec94
00775c2c: ldr      r2, [sp, #4]
00775c30: mov      r1, r0
00775c34: mov      r0, r2
00775c38: bl       #0x30ec94
00775c3c: mov      r2, #0
00775c40: str      r2, [sp, #0xc]
00775c44: ldr      r3, [sp]
00775c48: b        #0x7756d4
00775c4c: ldr      r1, [r0]
00775c50: sub      r1, r1, #1
00775c54: cmp      r1, #0
00775c58: str      r1, [r0]
00775c5c: bne      #0x775c64
00775c60: bl       #0x752b38
00775c64: mov      r3, #0
00775c68: str      r3, [r4, #0xcc]
00775c6c: str      r3, [r4, #0xc8]
00775c70: b        #0x775a54
00775c74: ldr      r1, [r0]
00775c78: sub      r1, r1, #1
00775c7c: cmp      r1, #0
00775c80: str      r1, [r0]
00775c84: bne      #0x775c8c
00775c88: bl       #0x752b38
00775c8c: mov      r3, #0
00775c90: str      r3, [r4, #0xcc]
00775c94: str      r3, [r4, #0xc8]
00775c98: b        #0x775a9c
00775c9c: ldr      r0, [sp, #0xb0]
00775ca0: ldr      r1, [sp, #0xac]
00775ca4: bl       #0x752b38
00775ca8: b        #0x77593c
00775cac: ldr      r0, [sp, #0xc4]
00775cb0: ldr      r1, [sp, #0xc0]
00775cb4: bl       #0x752b38
00775cb8: b        #0x7758cc
00775cbc: ldr      r0, [sp, #0x74]
00775cc0: ldr      r1, [sp, #0x70]
00775cc4: bl       #0x752b38
00775cc8: b        #0x775a94
00775ccc: ldr      r0, [sp, #0x88]
00775cd0: ldr      r1, [sp, #0x84]
00775cd4: bl       #0x752b38
00775cd8: b        #0x775a1c
00775cdc: ldr      r0, [sp, #0x9c]
00775ce0: ldr      r1, [sp, #0x98]
00775ce4: bl       #0x752b38
00775ce8: b        #0x7759ac
00775cec: ldr      r1, [r0]
00775cf0: sub      r1, r1, #1
00775cf4: cmp      r1, #0
00775cf8: str      r1, [r0]
00775cfc: bne      #0x775d04
00775d00: bl       #0x752b38
00775d04: mov      r8, #0
00775d08: str      r8, [r4, #0xc8]
00775d0c: str      r8, [r4, #0xcc]
00775d10: b        #0x775844
00775d14: bl       #0x30e310
00775d18: eoreq    pc, r1, ip, lsl #9
00775d1c: andeq    r4, r0, ip, lsr #1
00775d20: strheq   r3, [r0], -r4
00775d24: andseq   r4, sb, r0, lsl #1
00775d28: andseq   r4, sb, r8, lsl r0
00775d2c: ldrheq   r3, [sb], -r0
00775d30: andseq   r3, sb, r8, asr #30
00775d34: andseq   sp, r6, ip, lsr #16

# _ZN7gameswf4root17logical_to_screenERNS_5pointE
00773f50: ldr      r3, [pc, #0x1c8]
00773f54: ldr      r2, [pc, #0x1c8]
00773f58: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00773f5c: add      r3, pc, r3
00773f60: ldr      r2, [r3, r2]
00773f64: sub      sp, sp, #0xc
00773f68: mov      r4, r0
00773f6c: ldr      r3, [r2]
00773f70: mov      r5, r1
00773f74: mov      r0, r3
00773f78: ldr      r3, [r3]
00773f7c: mov      lr, pc
00773f80: ldr      pc, [r3, #0xac]
00773f84: cmp      r0, #0
00773f88: cmpne    r0, #2
00773f8c: movne    r8, #0
00773f90: moveq    r8, #1
00773f94: bne      #0x7740c4
00773f98: ldr      r6, [r4, #0xc]
00773f9c: ldr      r1, [r6, #0xb4]
00773fa0: ldr      r0, [r6, #0xb8]
00773fa4: bl       #0x30e3ac
00773fa8: ldr      r1, [r6, #0xbc]
00773fac: mov      fp, r0
00773fb0: ldr      r0, [r6, #0xc0]
00773fb4: bl       #0x30e3ac
00773fb8: mov      sl, r0
00773fbc: ldr      r0, [r4, #0x2c]
00773fc0: bl       #0x30e964
00773fc4: mov      r7, r0
00773fc8: ldr      r0, [r4, #0x1c]
00773fcc: bl       #0x30e964
00773fd0: mov      r1, r7
00773fd4: bl       #0x30ec94
00773fd8: str      r0, [sp]
00773fdc: ldr      r0, [r4, #0x30]
00773fe0: bl       #0x30e964
00773fe4: mov      r6, r0
00773fe8: ldr      r0, [r4, #0x20]
00773fec: bl       #0x30e964
00773ff0: mov      r1, r6
00773ff4: bl       #0x30ec94
00773ff8: str      r0, [sp, #4]
00773ffc: ldr      r0, [r4, #0x24]
00774000: bl       #0x30e964
00774004: mov      r1, #0x41000000
00774008: add      r1, r1, #0xa00000
0077400c: bl       #0x30ed6c
00774010: mov      r1, #0x41000000
00774014: mov      sb, r0
00774018: add      r1, r1, #0xa00000
0077401c: mov      r0, fp
00774020: bl       #0x30ec94
00774024: mov      r1, r0
00774028: mov      r0, r7
0077402c: bl       #0x30ec94
00774030: mov      r1, r0
00774034: mov      r0, sb
00774038: bl       #0x30ec94
0077403c: mov      r7, r0
00774040: ldr      r0, [r4, #0x28]
00774044: bl       #0x30e964
00774048: mov      r1, #0x41000000
0077404c: add      r1, r1, #0xa00000
00774050: bl       #0x30ed6c
00774054: mov      r1, #0x41000000
00774058: mov      r4, r0
0077405c: add      r1, r1, #0xa00000
00774060: mov      r0, sl
00774064: bl       #0x30ec94
00774068: mov      r1, r0
0077406c: mov      r0, r6
00774070: bl       #0x30ec94
00774074: mov      r1, r0
00774078: mov      r0, r4
0077407c: bl       #0x30ec94
00774080: cmp      r8, #0
00774084: mov      r4, r0
00774088: bne      #0x7740ec
0077408c: ldr      r0, [sp, #4]
00774090: ldr      r1, [r5]
00774094: bl       #0x30ed6c
00774098: mov      r1, r4
0077409c: bl       #0x30e3ac
007740a0: str      r0, [r5]
007740a4: ldr      r1, [r5, #4]
007740a8: ldr      r0, [sp]
007740ac: bl       #0x30ed6c
007740b0: mov      r1, r7
007740b4: bl       #0x30e3ac
007740b8: str      r0, [r5, #4]
007740bc: add      sp, sp, #0xc
007740c0: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
007740c4: ldr      r6, [r4, #0xc]
007740c8: ldr      r1, [r6, #0xbc]
007740cc: ldr      r0, [r6, #0xc0]
007740d0: bl       #0x30e3ac
007740d4: ldr      r1, [r6, #0xb4]
007740d8: mov      fp, r0
007740dc: ldr      r0, [r6, #0xb8]
007740e0: bl       #0x30e3ac
007740e4: mov      sl, r0
007740e8: b        #0x773fbc
007740ec: ldr      r0, [sp]
007740f0: ldr      r1, [r5]
007740f4: bl       #0x30ed6c
007740f8: mov      r1, r7
007740fc: bl       #0x30e3ac
00774100: str      r0, [r5]
00774104: ldr      r1, [r5, #4]
00774108: ldr      r0, [sp, #4]
0077410c: bl       #0x30ed6c
00774110: mov      r1, r4
00774114: bl       #0x30e3ac
00774118: str      r0, [r5, #4]
0077411c: b        #0x7740bc
00774120: eoreq    r0, r2, r4, lsr fp
00774124: strheq   r3, [r0], -r4

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
