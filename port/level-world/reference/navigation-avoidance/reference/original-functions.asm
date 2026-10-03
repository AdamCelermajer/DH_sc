
# _ZN7Point3DIfE9normalizeEv
0034d0b0: push     {r4, r5, r6, r7, lr}
0034d0b4: mov      r4, r0
0034d0b8: ldr      r0, [r0]
0034d0bc: sub      sp, sp, #0xc
0034d0c0: ldr      r7, [r4, #4]
0034d0c4: mov      r1, r0
0034d0c8: bl       #0x30ed6c
0034d0cc: mov      r1, r7
0034d0d0: mov      r5, r0
0034d0d4: mov      r0, r7
0034d0d8: bl       #0x30ed6c
0034d0dc: mov      r1, r0
0034d0e0: mov      r0, r5
0034d0e4: bl       #0x30eba4
0034d0e8: ldr      r6, [r4, #8]
0034d0ec: mov      r5, r0
0034d0f0: mov      r1, r6
0034d0f4: mov      r0, r6
0034d0f8: bl       #0x30ed6c
0034d0fc: mov      r1, r0
0034d100: mov      r0, r5
0034d104: bl       #0x30eba4
0034d108: bl       #0x30e124
0034d10c: add      r1, sp, #8
0034d110: str      r0, [r1, #-4]!
0034d114: mov      r0, r4
0034d118: bl       #0x34d04c
0034d11c: add      sp, sp, #0xc
0034d120: pop      {r4, r5, r6, r7, pc}

# _ZN14PhysicalObject15onCollisionTestEP18PhysicalBaseObjectsttstt
0046e6bc: push     {r4, r5, r6, r7}
0046e6c0: ldr      ip, [r0, #8]
0046e6c4: ldr      r1, [r1, #8]
0046e6c8: ldrh     r5, [sp, #0x10]
0046e6cc: cmp      ip, #0
0046e6d0: ldrsh    r4, [sp, #0x14]
0046e6d4: ldrh     r6, [sp, #0x18]
0046e6d8: ldrh     r7, [sp, #0x1c]
0046e6dc: beq      #0x46e6f8
0046e6e0: ldrb     r0, [ip, #0x80]
0046e6e4: cmp      r0, #0
0046e6e8: bne      #0x46e6f8
0046e6ec: mov      r0, #0
0046e6f0: pop      {r4, r5, r6, r7}
0046e6f4: bx       lr
0046e6f8: cmp      r1, #0
0046e6fc: beq      #0x46e70c
0046e700: ldrb     r1, [r1, #0x80]
0046e704: cmp      r1, #0
0046e708: beq      #0x46e6ec
0046e70c: cmp      r2, r4
0046e710: movne    r1, #0
0046e714: moveq    r1, #1
0046e718: cmp      r2, #0
0046e71c: moveq    r1, #0
0046e720: cmp      r1, #0
0046e724: bne      #0x46e740
0046e728: tst      r6, r5
0046e72c: beq      #0x46e6ec
0046e730: tst      r7, r3
0046e734: moveq    r0, #0
0046e738: movne    r0, #1
0046e73c: b        #0x46e6f0
0046e740: cmp      r2, #0
0046e744: movle    r0, #0
0046e748: movgt    r0, #1
0046e74c: b        #0x46e6f0

# _ZN7PFWorld14AvoidObstaclesERK8PFObjectR7Point3DIfE
00527cc4: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00527cc8: ldr      ip, [r1, #0x10]
00527ccc: ldr      r4, [pc, #0x3d4]
00527cd0: sub      sp, sp, #0x44
00527cd4: cmp      ip, #0
00527cd8: mov      r5, r2
00527cdc: add      r4, pc, r4
00527ce0: beq      #0x527da4
00527ce4: ldr      r3, [r1, #4]
00527ce8: ands     ip, r3, #1
00527cec: bne      #0x527da4
00527cf0: tst      r3, #2
00527cf4: beq      #0x527da4
00527cf8: add      r7, sp, #0x28
00527cfc: mov      r6, #0
00527d00: add      r2, sp, #0x34
00527d04: mov      r3, r7
00527d08: str      ip, [sp, #0x30]
00527d0c: str      r6, [sp, #0x34]
00527d10: str      r6, [sp, #0x38]
00527d14: str      r6, [sp, #0x3c]
00527d18: str      ip, [sp, #0x28]
00527d1c: str      ip, [sp, #0x2c]
00527d20: bl       #0x527660
00527d24: cmp      r0, #0
00527d28: str      r0, [sp, #0xc]
00527d2c: str      r6, [sp, #0x1c]
00527d30: str      r6, [sp, #0x20]
00527d34: str      r6, [sp, #0x24]
00527d38: beq      #0x527d9c
00527d3c: ldr      sl, [r5]
00527d40: ldr      r1, [sp, #0x34]
00527d44: ldr      r8, [r5, #4]
00527d48: mov      r0, sl
00527d4c: bl       #0x30ed6c
00527d50: ldr      r1, [sp, #0x38]
00527d54: mov      sb, r0
00527d58: mov      r0, r8
00527d5c: bl       #0x30ed6c
00527d60: mov      r1, r0
00527d64: mov      r0, sb
00527d68: bl       #0x30eba4
00527d6c: ldr      sb, [r5, #8]
00527d70: mov      fp, r0
00527d74: ldr      r1, [sp, #0x3c]
00527d78: mov      r0, sb
00527d7c: bl       #0x30ed6c
00527d80: mov      r1, r0
00527d84: mov      r0, fp
00527d88: bl       #0x30eba4
00527d8c: mov      r1, r6
00527d90: bl       #0x30e70c
00527d94: cmp      r0, #0
00527d98: bne      #0x527dac
00527d9c: mov      r0, r7
00527da0: bl       #0x524c74
00527da4: add      sp, sp, #0x44
00527da8: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
00527dac: ldr      r3, [pc, #0x2f8]
00527db0: mov      r0, r8
00527db4: ldr      r3, [r4, r3]
00527db8: ldr      fp, [r3, #8]
00527dbc: ldr      r6, [r3, #4]
00527dc0: ldr      r4, [r3]
00527dc4: mov      r1, fp
00527dc8: bl       #0x30ed6c
00527dcc: mov      r1, r6
00527dd0: mov      r3, r0
00527dd4: mov      r0, sb
00527dd8: str      r3, [sp, #4]
00527ddc: bl       #0x30ed6c
00527de0: ldr      r3, [sp, #4]
00527de4: mov      r1, r0
00527de8: mov      r0, r3
00527dec: bl       #0x30e3ac
00527df0: mov      r1, r4
00527df4: str      r0, [sp, #0x10]
00527df8: mov      r0, sb
00527dfc: bl       #0x30ed6c
00527e00: mov      r1, fp
00527e04: mov      sb, r0
00527e08: mov      r0, sl
00527e0c: bl       #0x30ed6c
00527e10: mov      r1, r0
00527e14: mov      r0, sb
00527e18: bl       #0x30e3ac
00527e1c: mov      r1, r6
00527e20: str      r0, [sp, #0x14]
00527e24: mov      r0, sl
00527e28: bl       #0x30ed6c
00527e2c: mov      r1, r4
00527e30: mov      r6, r0
00527e34: mov      r0, r8
00527e38: bl       #0x30ed6c
00527e3c: mov      r1, r0
00527e40: mov      r0, r6
00527e44: bl       #0x30e3ac
00527e48: str      r0, [sp, #0x18]
00527e4c: add      r0, sp, #0x10
00527e50: bl       #0x34d0b0
00527e54: ldr      sb, [sp, #0x34]
00527e58: ldr      r8, [r0]
00527e5c: mov      r3, r0
00527e60: ldr      r6, [r0, #4]
00527e64: mov      r1, sb
00527e68: mov      r0, r8
00527e6c: ldr      r4, [r3, #8]
00527e70: bl       #0x30ed6c
00527e74: ldr      sl, [sp, #0x38]
00527e78: mov      fp, r0
00527e7c: mov      r0, r6
00527e80: mov      r1, sl
00527e84: bl       #0x30ed6c
00527e88: mov      r1, r0
00527e8c: mov      r0, fp
00527e90: bl       #0x30eba4
00527e94: ldr      r1, [sp, #0x3c]
00527e98: mov      fp, r0
00527e9c: mov      r0, r4
00527ea0: bl       #0x30ed6c
00527ea4: mov      r1, r0
00527ea8: mov      r0, fp
00527eac: bl       #0x30eba4
00527eb0: mov      r1, sb
00527eb4: mov      fp, r0
00527eb8: mov      r0, sb
00527ebc: bl       #0x30ed6c
00527ec0: mov      r1, sl
00527ec4: mov      sb, r0
00527ec8: mov      r0, sl
00527ecc: bl       #0x30ed6c
00527ed0: mov      r1, r0
00527ed4: mov      r0, sb
00527ed8: bl       #0x30eba4
00527edc: mov      sl, r0
00527ee0: ldr      r0, [sp, #0x3c]
00527ee4: mov      r1, r0
00527ee8: bl       #0x30ed6c
00527eec: mov      r1, r0
00527ef0: mov      r0, sl
00527ef4: bl       #0x30eba4
00527ef8: bl       #0x30e124
00527efc: mov      sl, r0
00527f00: ldr      r0, [r5]
00527f04: mov      r1, r0
00527f08: bl       #0x30ed6c
00527f0c: mov      sb, r0
00527f10: ldr      r0, [r5, #4]
00527f14: mov      r1, r0
00527f18: bl       #0x30ed6c
00527f1c: mov      r1, r0
00527f20: mov      r0, sb
00527f24: bl       #0x30eba4
00527f28: mov      sb, r0
00527f2c: ldr      r0, [r5, #8]
00527f30: mov      r1, r0
00527f34: bl       #0x30ed6c
00527f38: mov      r1, r0
00527f3c: mov      r0, sb
00527f40: bl       #0x30eba4
00527f44: bl       #0x30e124
00527f48: bic      sb, fp, #0x80000000
00527f4c: movw     r1, #0xb717
00527f50: str      r0, [sp, #8]
00527f54: movt     r1, #0x38d1
00527f58: mov      r0, sb
00527f5c: bl       #0x30e70c
00527f60: cmp      r0, #0
00527f64: bne      #0x527fa8
00527f68: mov      r1, sb
00527f6c: mov      r0, fp
00527f70: bl       #0x30ec94
00527f74: mov      sb, r0
00527f78: mov      r1, sb
00527f7c: mov      r0, r8
00527f80: bl       #0x30ed6c
00527f84: mov      r1, sb
00527f88: mov      r8, r0
00527f8c: mov      r0, r6
00527f90: bl       #0x30ed6c
00527f94: mov      r1, sb
00527f98: mov      r6, r0
00527f9c: mov      r0, r4
00527fa0: bl       #0x30ed6c
00527fa4: mov      r4, r0
00527fa8: mov      r1, r8
00527fac: mov      r0, sl
00527fb0: bl       #0x30ed6c
00527fb4: ldr      r1, [r5]
00527fb8: bl       #0x30eba4
00527fbc: mov      r1, r6
00527fc0: mov      sb, r0
00527fc4: mov      r0, sl
00527fc8: bl       #0x30ed6c
00527fcc: ldr      r1, [r5, #4]
00527fd0: bl       #0x30eba4
00527fd4: mov      r1, r4
00527fd8: mov      fp, r0
00527fdc: mov      r0, sl
00527fe0: bl       #0x30ed6c
00527fe4: ldr      r1, [r5, #8]
00527fe8: bl       #0x30eba4
00527fec: ldr      r3, [sp, #0xc]
00527ff0: str      fp, [sp, #0x20]
00527ff4: str      r0, [sp, #0x24]
00527ff8: cmp      r3, #1
00527ffc: str      sb, [sp, #0x1c]
00528000: bls      #0x528090
00528004: mov      r1, r5
00528008: add      r0, sp, #0x1c
0052800c: bl       #0x313058
00528010: movw     r1, #0xb8c2
00528014: movt     r1, #0x3db2
00528018: bl       #0x30e4b4
0052801c: cmp      r0, #0
00528020: ldreq    sb, [sp, #0x1c]
00528024: beq      #0x528090
00528028: movw     r1, #0x2d41
0052802c: ldr      r0, [sp, #8]
00528030: movt     r1, #0x3db3
00528034: bl       #0x30ed6c
00528038: mov      r1, r8
0052803c: mov      sl, r0
00528040: bl       #0x30ed6c
00528044: mov      r1, r0
00528048: ldr      r0, [r5]
0052804c: bl       #0x30eba4
00528050: mov      r1, r6
00528054: str      r0, [r5]
00528058: mov      r0, sl
0052805c: bl       #0x30ed6c
00528060: mov      r1, r0
00528064: ldr      r0, [r5, #4]
00528068: bl       #0x30eba4
0052806c: mov      r1, r4
00528070: str      r0, [r5, #4]
00528074: mov      r0, sl
00528078: bl       #0x30ed6c
0052807c: mov      r1, r0
00528080: ldr      r0, [r5, #8]
00528084: bl       #0x30eba4
00528088: str      r0, [r5, #8]
0052808c: b        #0x527d9c
00528090: ldr      r2, [sp, #0x20]
00528094: ldr      r3, [sp, #0x24]
00528098: str      sb, [r5]
0052809c: str      r2, [r5, #4]
005280a0: str      r3, [r5, #8]
005280a4: b        #0x527d9c
005280a8: strheq   ip, [r6], #-0xd4
005280ac: andeq    r4, r0, r0, asr #6

# _ZN7PFWorld19_CalcObstaclesForceERK8PFObjectR7Point3DIfEPSt6vectorINS_13ObstacleForceESaIS7_EE
00527660: push     {r4, r5, r6, r7, r8, sb, sl, fp, lr}
00527664: add      r0, r0, #0x2c
00527668: sub      sp, sp, #0x5c
0052766c: mov      r6, r1
00527670: add      r1, r1, #0x10
00527674: str      r2, [sp, #0x14]
00527678: str      r3, [sp, #0x28]
0052767c: bl       #0x527560
00527680: ldr      r1, [pc, #0x624]
00527684: ldr      sl, [r0, #0x10]
00527688: ldr      r4, [r0]
0052768c: ldr      r2, [r0, #0xc]
00527690: add      r1, pc, r1
00527694: str      r1, [sp, #0x10]
00527698: ldr      r1, [sp, #0x14]
0052769c: mov      r3, #0
005276a0: str      r2, [sp, #0x18]
005276a4: cmp      r4, sl
005276a8: ldr      r8, [r0, #8]
005276ac: str      r3, [sp, #0x48]
005276b0: str      r3, [sp, #0x4c]
005276b4: str      r3, [sp, #0x50]
005276b8: str      r3, [r1]
005276bc: str      r3, [r1, #4]
005276c0: str      r3, [r1, #8]
005276c4: moveq    r3, #0
005276c8: streq    r3, [sp, #0x30]
005276cc: beq      #0x5278c4
005276d0: ldr      r2, [pc, #0x5d8]
005276d4: ldr      r1, [pc, #0x5d8]
005276d8: mov      r3, #0
005276dc: str      r2, [sp, #0x24]
005276e0: ldr      r2, [pc, #0x5d0]
005276e4: str      r1, [sp, #0x34]
005276e8: add      r7, r6, #0x38
005276ec: add      r2, pc, r2
005276f0: str      r2, [sp, #0x38]
005276f4: ldr      r2, [pc, #0x5c0]
005276f8: str      r3, [sp, #0x30]
005276fc: add      r2, pc, r2
00527700: str      r2, [sp, #0x3c]
00527704: ldr      r2, [pc, #0x5b4]
00527708: add      r2, pc, r2
0052770c: str      r2, [sp, #0x40]
00527710: ldr      r2, [sp, #0x28]
00527714: add      r2, r2, #8
00527718: str      r2, [sp, #0x44]
0052771c: ldr      r5, [r4]
00527720: cmp      r5, #0
00527724: beq      #0x527a90
00527728: ldr      r3, [r5, #4]
0052772c: tst      r3, #4
00527730: beq      #0x5278a4
00527734: cmp      r6, r5
00527738: beq      #0x5278a4
0052773c: tst      r3, #8
00527740: beq      #0x5278a4
00527744: ldr      r0, [r6]
00527748: ldr      r3, [r5]
0052774c: cmp      r0, #0
00527750: ldrne    r0, [r0, #0x2dc]
00527754: cmp      r3, #0
00527758: beq      #0x52776c
0052775c: ldr      r1, [r3, #0x2dc]
00527760: cmp      r0, #0
00527764: cmpne    r1, #0
00527768: bne      #0x527ae0
0052776c: ldr      r1, [r5, #0x18]
00527770: ldr      r0, [r6, #0x18]
00527774: bl       #0x30e3ac
00527778: ldr      r1, [r5, #0x1c]
0052777c: mov      fp, r0
00527780: ldr      r0, [r6, #0x1c]
00527784: bl       #0x30e3ac
00527788: ldr      r1, [r5, #0x20]
0052778c: mov      sb, r0
00527790: ldr      r0, [r6, #0x20]
00527794: bl       #0x30e3ac
00527798: mov      r1, fp
0052779c: str      r0, [sp, #0x50]
005277a0: mov      r0, fp
005277a4: str      fp, [sp, #0x48]
005277a8: str      sb, [sp, #0x4c]
005277ac: bl       #0x30ed6c
005277b0: mov      r1, sb
005277b4: mov      fp, r0
005277b8: mov      r0, sb
005277bc: bl       #0x30ed6c
005277c0: mov      r1, r0
005277c4: mov      r0, fp
005277c8: bl       #0x30eba4
005277cc: ldr      r1, [r5, #8]
005277d0: ldr      r2, [r6, #0x38]
005277d4: ldr      fp, [r6, #8]
005277d8: str      r1, [sp, #0x20]
005277dc: ldr      r3, [r5, #0x30]
005277e0: cmp      r2, r7
005277e4: mov      sb, r0
005277e8: str      r3, [sp, #0x1c]
005277ec: movne    r3, r2
005277f0: beq      #0x5278d0
005277f4: ldr      r3, [r3]
005277f8: cmp      r7, r3
005277fc: bne      #0x5277f4
00527800: ldr      r3, [r2, #8]
00527804: mov      r0, r3
00527808: ldr      r3, [r3]
0052780c: mov      lr, pc
00527810: ldr      pc, [r3, #0x28]
00527814: ldr      r3, [r6, #0x18]
00527818: mov      r2, r0
0052781c: ldr      r0, [r0]
00527820: mov      r1, r3
00527824: str      r3, [sp, #8]
00527828: str      r2, [sp, #0xc]
0052782c: bl       #0x30e3ac
00527830: ldr      r1, [r6, #0x1c]
00527834: ldr      r2, [sp, #0xc]
00527838: mov      ip, r0
0052783c: str      r1, [sp, #0x2c]
00527840: ldr      r0, [r2, #4]
00527844: str      ip, [sp, #0xc]
00527848: bl       #0x30e3ac
0052784c: ldr      ip, [sp, #0xc]
00527850: mov      r2, r0
00527854: str      r2, [sp, #0xc]
00527858: mov      r1, ip
0052785c: mov      r0, ip
00527860: bl       #0x30ed6c
00527864: ldr      r2, [sp, #0xc]
00527868: mov      ip, r0
0052786c: str      ip, [sp, #0xc]
00527870: mov      r1, r2
00527874: mov      r0, r2
00527878: bl       #0x30ed6c
0052787c: ldr      ip, [sp, #0xc]
00527880: mov      r1, r0
00527884: mov      r0, ip
00527888: bl       #0x30eba4
0052788c: mov      r1, r0
00527890: mov      r0, sb
00527894: bl       #0x30e4b4
00527898: cmp      r0, #0
0052789c: ldr      r3, [sp, #8]
005278a0: beq      #0x5278dc
005278a4: add      r4, r4, #4
005278a8: cmp      r8, r4
005278ac: ldreq    r3, [sp, #0x18]
005278b0: ldreq    r4, [r3, #4]!
005278b4: streq    r3, [sp, #0x18]
005278b8: addeq    r8, r4, #0x80
005278bc: cmp      sl, r4
005278c0: bne      #0x52771c
005278c4: ldr      r0, [sp, #0x30]
005278c8: add      sp, sp, #0x5c
005278cc: pop      {r4, r5, r6, r7, r8, sb, sl, fp, pc}
005278d0: ldr      r1, [r6, #0x1c]
005278d4: ldr      r3, [r6, #0x18]
005278d8: str      r1, [sp, #0x2c]
005278dc: ldr      r0, [r6, #0x40]
005278e0: mov      r1, r3
005278e4: bl       #0x30e3ac
005278e8: ldr      r1, [sp, #0x2c]
005278ec: mov      r3, r0
005278f0: ldr      r0, [r6, #0x44]
005278f4: str      r3, [sp, #8]
005278f8: bl       #0x30e3ac
005278fc: ldr      r3, [sp, #8]
00527900: mov      r2, r0
00527904: str      r2, [sp, #0xc]
00527908: mov      r1, r3
0052790c: mov      r0, r3
00527910: bl       #0x30ed6c
00527914: ldr      r2, [sp, #0xc]
00527918: mov      r3, r0
0052791c: str      r3, [sp, #8]
00527920: mov      r1, r2
00527924: mov      r0, r2
00527928: bl       #0x30ed6c
0052792c: ldr      r3, [sp, #8]
00527930: mov      r1, r0
00527934: mov      r0, r3
00527938: bl       #0x30eba4
0052793c: mov      r1, r0
00527940: mov      r0, sb
00527944: bl       #0x30e4b4
00527948: cmp      r0, #0
0052794c: bne      #0x5278a4
00527950: mov      r0, fp
00527954: ldr      r1, [sp, #0x20]
00527958: bl       #0x30eba4
0052795c: ldr      r1, [sp, #0x1c]
00527960: bl       #0x30eba4
00527964: mov      r1, r0
00527968: bl       #0x30ed6c
0052796c: mov      r1, sb
00527970: mov      fp, r0
00527974: bl       #0x30e2f8
00527978: cmp      r0, #0
0052797c: beq      #0x5278a4
00527980: mov      r1, fp
00527984: mov      r0, sb
00527988: bl       #0x30ec94
0052798c: mov      r1, r0
00527990: mov      r0, #0x3f800000
00527994: bl       #0x30e3ac
00527998: mov      r2, #0
0052799c: str      r0, [sp, #0x1c]
005279a0: add      r0, sp, #0x48
005279a4: str      r2, [sp, #0x50]
005279a8: bl       #0x34d0b0
005279ac: ldr      r1, [r5, #0x34]
005279b0: ldr      r0, [sp, #0x1c]
005279b4: bl       #0x30ed6c
005279b8: ldr      r1, [sp, #0x48]
005279bc: mov      fp, r0
005279c0: bl       #0x30ed6c
005279c4: ldr      r1, [sp, #0x4c]
005279c8: mov      sb, r0
005279cc: mov      r0, fp
005279d0: str      sb, [sp, #0x48]
005279d4: bl       #0x30ed6c
005279d8: ldr      r1, [sp, #0x50]
005279dc: mov      r3, r0
005279e0: mov      r0, fp
005279e4: str      r3, [sp, #0x4c]
005279e8: str      r3, [sp, #8]
005279ec: bl       #0x30ed6c
005279f0: ldr      r1, [sp, #0x28]
005279f4: mov      fp, r0
005279f8: str      r0, [sp, #0x50]
005279fc: cmp      r1, #0
00527a00: ldr      r3, [sp, #8]
00527a04: beq      #0x527a3c
00527a08: ldmib    r1, {r2, ip}
00527a0c: cmp      r2, ip
00527a10: beq      #0x527b10
00527a14: str      r5, [r2, #0x10]
00527a18: str      sb, [r2]
00527a1c: str      r3, [r2, #4]
00527a20: str      r0, [r2, #8]
00527a24: ldr      r3, [sp, #0x1c]
00527a28: str      r3, [r2, #0xc]
00527a2c: ldr      r3, [r1, #4]
00527a30: ldr      sb, [sp, #0x48]
00527a34: add      r3, r3, #0x14
00527a38: str      r3, [r1, #4]
00527a3c: ldr      r3, [sp, #0x14]
00527a40: mov      r1, sb
00527a44: ldr      r0, [r3]
00527a48: bl       #0x30eba4
00527a4c: ldr      r1, [sp, #0x14]
00527a50: str      r0, [r1]
00527a54: ldr      r2, [sp, #0x14]
00527a58: ldr      r1, [sp, #0x4c]
00527a5c: ldr      r0, [r2, #4]
00527a60: bl       #0x30eba4
00527a64: ldr      r3, [sp, #0x14]
00527a68: str      r0, [r3, #4]
00527a6c: ldr      r1, [sp, #0x50]
00527a70: ldr      r0, [r3, #8]
00527a74: bl       #0x30eba4
00527a78: ldr      r1, [sp, #0x14]
00527a7c: str      r0, [r1, #8]
00527a80: ldr      r2, [sp, #0x30]
00527a84: add      r2, r2, #1
00527a88: str      r2, [sp, #0x30]
00527a8c: b        #0x5278a4
00527a90: ldr      r2, [sp, #0x10]
00527a94: ldr      r1, [sp, #0x24]
00527a98: ldr      r3, [r2, r1]
00527a9c: ldr      r3, [r3]
00527aa0: cmp      r3, #2
00527aa4: streq    r5, [r5]
00527aa8: beq      #0x527728
00527aac: cmp      r3, #1
00527ab0: bne      #0x527728
00527ab4: ldr      r1, [sp, #0x10]
00527ab8: ldr      r3, [sp, #0x34]
00527abc: mov      ip, #0x3c
00527ac0: ldr      r2, [sp, #0x3c]
00527ac4: ldr      r0, [r1, r3]
00527ac8: ldr      r1, [sp, #0x38]
00527acc: ldr      r3, [sp, #0x40]
00527ad0: add      r0, r0, #0xa8
00527ad4: str      ip, [sp]
00527ad8: bl       #0x30e004
00527adc: b        #0x527728
00527ae0: bl       #0x46e768
00527ae4: cmp      r0, #0
00527ae8: bne      #0x52776c
00527aec: add      r4, r4, #4
00527af0: cmp      r8, r4
00527af4: ldreq    r3, [sp, #0x18]
00527af8: ldreq    r4, [r3, #4]!
00527afc: streq    r3, [sp, #0x18]
00527b00: addeq    r8, r4, #0x80
00527b04: cmp      sl, r4
00527b08: bne      #0x52771c
00527b0c: b        #0x5278c4
00527b10: ldr      r1, [sp, #0x28]
00527b14: movw     r2, #0xcccd
00527b18: movt     r2, #0xcccc
00527b1c: ldr      r0, [r1]
00527b20: rsb      r0, r0, ip
00527b24: asr      r0, r0, #2
00527b28: mul      r0, r2, r0
00527b2c: movw     r2, #0xcccc
00527b30: cmp      r0, #1
00527b34: addhs    r1, r0, r0
00527b38: addlo    r1, r0, #1
00527b3c: orr      r2, r2, r2, lsl #12
00527b40: cmp      r1, r2
00527b44: bhi      #0x527b50
00527b48: cmp      r0, r1
00527b4c: bls      #0x527b58
00527b50: movw     r1, #0xcccc
00527b54: orr      r1, r1, r1, lsl #12
00527b58: add      r2, sp, #0x58
00527b5c: str      r1, [r2, #-4]!
00527b60: ldr      r0, [sp, #0x44]
00527b64: str      r3, [sp, #8]
00527b68: str      ip, [sp, #0xc]
00527b6c: bl       #0x526c90
00527b70: ldr      r1, [sp, #0x28]
00527b74: str      r0, [sp, #0x20]
00527b78: ldr      ip, [sp, #0xc]
00527b7c: ldr      r2, [r1]
00527b80: movw     r0, #0xcccd
00527b84: movt     r0, #0xcccc
00527b88: rsb      r1, r2, ip
00527b8c: asr      r1, r1, #2
00527b90: mul      ip, r0, r1
00527b94: ldr      r3, [sp, #8]
00527b98: cmp      ip, #0
00527b9c: ldrle    r2, [sp, #0x20]
00527ba0: ble      #0x527bf0
00527ba4: ldr      r1, [sp, #0x20]
00527ba8: mov      r0, ip
00527bac: ldr      lr, [r2]
00527bb0: subs     r0, r0, #1
00527bb4: str      lr, [r1]
00527bb8: ldr      lr, [r2, #4]
00527bbc: str      lr, [r1, #4]
00527bc0: ldr      lr, [r2, #8]
00527bc4: str      lr, [r1, #8]
00527bc8: ldr      lr, [r2, #0xc]
00527bcc: str      lr, [r1, #0xc]
00527bd0: ldr      lr, [r2, #0x10]
00527bd4: add      r2, r2, #0x14
00527bd8: str      lr, [r1, #0x10]
00527bdc: add      r1, r1, #0x14
00527be0: bne      #0x527bac
00527be4: ldr      r1, [sp, #0x20]
00527be8: mov      r2, #0x14
00527bec: mla      r2, r2, ip, r1
00527bf0: str      sb, [r2]
00527bf4: str      r3, [r2, #4]
00527bf8: str      fp, [r2, #8]
00527bfc: ldr      r3, [sp, #0x1c]
00527c00: str      r5, [r2, #0x10]
00527c04: add      r5, r2, #0x14
00527c08: str      r3, [r2, #0xc]
00527c0c: ldr      r1, [sp, #0x28]
00527c10: ldm      r1, {r0, r3}
00527c14: cmp      r3, r0
00527c18: beq      #0x527c48
00527c1c: sub      r1, r3, #0x14
00527c20: rsb      r1, r0, r1
00527c24: movw     r2, #0xcccd
00527c28: lsr      r1, r1, #2
00527c2c: movt     r2, #0xccc
00527c30: mul      r2, r2, r1
00527c34: mvn      r1, #0x13
00527c38: bic      r2, r2, #0xc0000000
00527c3c: mul      r2, r1, r2
00527c40: add      r2, r2, r1
00527c44: add      r3, r3, r2
00527c48: ldr      r1, [sp, #0x28]
00527c4c: cmp      r3, #0
00527c50: ldr      r2, [r1, #8]
00527c54: beq      #0x527c80
00527c58: rsb      r3, r3, r2
00527c5c: movw     r2, #0xcccd
00527c60: asr      r3, r3, #2
00527c64: movt     r2, #0xcccc
00527c68: mul      r2, r2, r3
00527c6c: mov      r1, #0x14
00527c70: mul      r1, r1, r2
00527c74: cmp      r1, #0x80
00527c78: bhi      #0x527ca4
00527c7c: bl       #0x708f00
00527c80: ldr      r3, [sp, #0x54]
00527c84: ldr      r1, [sp, #0x20]
00527c88: mov      r2, #0x14
00527c8c: ldr      sb, [sp, #0x48]
00527c90: mla      r3, r2, r3, r1
00527c94: ldr      r2, [sp, #0x28]
00527c98: stm      r2, {r1, r5}
00527c9c: str      r3, [r2, #8]
00527ca0: b        #0x527a3c
00527ca4: bl       #0x310440
00527ca8: b        #0x527c80
00527cac: subeq    sp, r6, r0, lsl #8
00527cb0: andeq    r3, r0, r0, asr #19
00527cb4: andeq    r1, r0, r0, asr #19
00527cb8: eorseq   r6, sb, ip, ror #25
00527cbc: eorseq   r8, sb, r4, lsl fp
00527cc0: eorseq   r5, fp, r0, asr #7

# _ZNK7Point3DIfE5angleERKS0_
00313058: push     {r4, lr}
0031305c: bl       #0x312f40
00313060: pop      {r4, lr}
00313064: b        #0x30e3dc

# _ZNK14PhysicalObject10canCollideEPS_
0046e768: push     {r4, r5, r6, lr}
0046e76c: cmp      r1, #0
0046e770: sub      sp, sp, #0x10
0046e774: mov      ip, r0
0046e778: beq      #0x46e7fc
0046e77c: ldrb     r3, [r0, #0x26]
0046e780: cmp      r3, #0
0046e784: bne      #0x46e7fc
0046e788: ldrb     r3, [r1, #0x26]
0046e78c: cmp      r3, #0
0046e790: bne      #0x46e7fc
0046e794: ldr      r3, [r0, #0x18]
0046e798: cmp      r3, #0
0046e79c: beq      #0x46e7f0
0046e7a0: ldr      r0, [r1, #0x18]
0046e7a4: ldrh     r2, [r3, #0x26]
0046e7a8: ldrh     r4, [r3, #0x24]
0046e7ac: cmp      r0, #0
0046e7b0: ldrh     r3, [r3, #0x22]
0046e7b4: beq      #0x46e808
0046e7b8: ldrh     r6, [r0, #0x26]
0046e7bc: ldrh     lr, [r0, #0x24]
0046e7c0: ldrh     r5, [r0, #0x22]
0046e7c4: sxth     r0, r6
0046e7c8: str      r0, [sp, #4]
0046e7cc: str      r4, [sp]
0046e7d0: str      r5, [sp, #8]
0046e7d4: str      lr, [sp, #0xc]
0046e7d8: mov      r0, ip
0046e7dc: sxth     r2, r2
0046e7e0: ldr      ip, [ip]
0046e7e4: mov      lr, pc
0046e7e8: ldr      pc, [ip, #8]
0046e7ec: b        #0x46e800
0046e7f0: ldr      r3, [r0, #0x1c]
0046e7f4: cmp      r3, #0
0046e7f8: bne      #0x46e7a0
0046e7fc: mov      r0, #0
0046e800: add      sp, sp, #0x10
0046e804: pop      {r4, r5, r6, pc}
0046e808: ldr      r0, [r1, #0x1c]
0046e80c: cmp      r0, #0
0046e810: bne      #0x46e7b8
0046e814: b        #0x46e7fc
