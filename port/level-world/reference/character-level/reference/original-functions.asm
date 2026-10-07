
# _ZN9Character9_SetLevelERKN3sfc6script3lua9ArgumentsERNS2_12ReturnValuesEPv
003b73a4: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003b73a8: ldr      r3, [r0, #4]
003b73ac: mov      r6, r2
003b73b0: ldr      r4, [pc, #0xe0]
003b73b4: ldm      r3, {r1, r2}
003b73b8: add      r4, pc, r4
003b73bc: mov      r5, r0
003b73c0: rsb      r3, r1, r2
003b73c4: asr      r3, r3, #4
003b73c8: add      r2, r3, r3, lsl #3
003b73cc: add      r2, r2, r2, lsl #6
003b73d0: add      r2, r3, r2, lsl #3
003b73d4: add      r2, r2, r2, lsl #15
003b73d8: add      r3, r3, r2, lsl #3
003b73dc: cmp      r3, #0
003b73e0: bne      #0x3b73e8
003b73e4: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003b73e8: ldr      r3, [r1, #4]
003b73ec: cmp      r3, #3
003b73f0: bne      #0x3b73e4
003b73f4: mov      r1, #0
003b73f8: bl       #0x37baf8
003b73fc: bl       #0x31bbf0
003b7400: ldr      r3, [pc, #0x94]
003b7404: ldr      r8, [pc, #0x94]
003b7408: ldr      r7, [pc, #0x94]
003b740c: ldr      r4, [r4, r3]
003b7410: add      r8, pc, r8
003b7414: add      r7, pc, r7
003b7418: mov      sl, r0
003b741c: mov      r1, r8
003b7420: mov      r2, r7
003b7424: ldr      r0, [r4, #0x2c]
003b7428: bl       #0x4c4bdc
003b742c: lsl      sb, r0, #8
003b7430: mov      r0, sl
003b7434: bl       #0x30e4cc
003b7438: cmp      sb, r0
003b743c: blt      #0x3b7480
003b7440: mov      r1, #0
003b7444: mov      r0, r5
003b7448: bl       #0x37baf8
003b744c: bl       #0x31bbf0
003b7450: bl       #0x30e4cc
003b7454: str      r0, [r6, #0x5b8]
003b7458: mov      r1, #1
003b745c: add      r0, r6, #0x560
003b7460: bl       #0x3e0810
003b7464: mov      r0, r6
003b7468: mvn      r1, #0
003b746c: bl       #0x3bdca4
003b7470: mov      r0, r6
003b7474: mvn      r1, #0
003b7478: pop      {r4, r5, r6, r7, r8, sb, sl, lr}
003b747c: b        #0x3bdbb8
003b7480: ldr      r0, [r4, #0x2c]
003b7484: mov      r1, r8
003b7488: mov      r2, r7
003b748c: bl       #0x4c4bdc
003b7490: lsl      r0, r0, #8
003b7494: b        #0x3b7454
003b7498: ldrsbeq  sp, [sp], #-0x68
003b749c: strdeq   r3, r4, [r0], -r4
003b74a0: subseq   sl, r0, r0, asr #6
003b74a4: subseq   sp, r0, r4, lsl r1

# _ZN14CharProperties20UpdateBasePropertiesEi
003e087c: push     {r4, r5, r6, lr}
003e0880: mov      r4, r0
003e0884: mov      r5, r1
003e0888: bl       #0x3defbc
003e088c: mov      r0, r4
003e0890: mov      r1, r5
003e0894: bl       #0x3df2a4
003e0898: mov      r0, r4
003e089c: mov      r1, #1
003e08a0: pop      {r4, r5, r6, lr}
003e08a4: b        #0x3e0810

# _ZNK9Character8GetLevelEv
003bd120: add      r0, r0, #0x560
003bd124: mov      r1, #0x13
003bd128: mov      r2, #0
003bd12c: b        #0x3df6e0

# _ZN14CharProperties16RecalcPropertiesEb
003e0810: cmp      r1, #0
003e0814: push     {r4, r5, r6, lr}
003e0818: mov      r4, r0
003e081c: bne      #0x3e0840
003e0820: mov      r5, #0
003e0824: mov      r1, r5
003e0828: mov      r0, r4
003e082c: add      r5, r5, #1
003e0830: bl       #0x3dfe60
003e0834: cmp      r5, #0xe0
003e0838: bne      #0x3e0824
003e083c: pop      {r4, r5, r6, pc}
003e0840: add      r1, r0, #8
003e0844: ldr      r2, [r0, #0x74]
003e0848: mov      r3, #0
003e084c: bl       #0x3e2e20
003e0850: b        #0x3e0820
