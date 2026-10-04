
# _ZN9LuaScript4CallEPKcRN3sfc6script3lua12ReturnValuesE
0037c494: push     {r4, r5, r6, r7, lr}
0037c498: ldr      r3, [r2, #0x24]
0037c49c: mov      r5, r2
0037c4a0: ldr      r4, [pc, #0x64]
0037c4a4: ldr      ip, [r3]
0037c4a8: ldr      r2, [r3, #4]
0037c4ac: sub      sp, sp, #0xc
0037c4b0: mov      r6, r0
0037c4b4: cmp      ip, r2
0037c4b8: add      r4, pc, r4
0037c4bc: mov      r7, r1
0037c4c0: beq      #0x37c4d4
0037c4c4: mov      r0, r3
0037c4c8: mov      r1, ip
0037c4cc: add      r3, sp, #4
0037c4d0: bl       #0x31c3cc
0037c4d4: mov      r1, r7
0037c4d8: mov      r0, r6
0037c4dc: bl       #0x37c314
0037c4e0: mov      r2, r5
0037c4e4: mov      r1, r0
0037c4e8: add      r0, r6, #4
0037c4ec: bl       #0x31ab14
0037c4f0: ldr      r3, [pc, #0x18]
0037c4f4: ldr      r3, [r4, r3]
0037c4f8: ldr      r2, [r3]
0037c4fc: add      r2, r2, #1
0037c500: str      r2, [r3]
0037c504: add      sp, sp, #0xc
0037c508: pop      {r4, r5, r6, r7, pc}

# _ZN13DebugSwitches4loadEv
00337888: push     {r4, r5, r6, r7, r8, sb, sl, lr}
0033788c: ldr      r4, [pc, #0x1c8]
00337890: ldr      r3, [pc, #0x1c8]
00337894: ldr      r6, [pc, #0x1c8]
00337898: add      r4, pc, r4
0033789c: ldr      r3, [r4, r3]
003378a0: ldr      r1, [r4, r6]
003378a4: sub      sp, sp, #0x98
003378a8: ldrb     r2, [r3]
003378ac: ldr      r1, [r1]
003378b0: mov      r7, r0
003378b4: cmp      r2, #0
003378b8: str      r1, [sp, #0x94]
003378bc: bne      #0x337a3c
003378c0: mov      r1, #1
003378c4: strb     r1, [r3]
003378c8: ldr      r3, [pc, #0x198]
003378cc: ldr      r3, [r4, r3]
003378d0: ldr      r3, [r3, #0x10]
003378d4: ldr      r5, [r3, #0x34]
003378d8: cmp      r5, #0
003378dc: beq      #0x337924
003378e0: ldr      r1, [pc, #0x184]
003378e4: ldr      r3, [r5]
003378e8: mov      r0, r5
003378ec: add      r1, pc, r1
003378f0: mov      lr, pc
003378f4: ldr      pc, [r3, #0x94]
003378f8: cmp      r0, #0
003378fc: mov      r1, r0
00337900: str      r0, [sp, #4]
00337904: beq      #0x337924
00337908: mov      r0, r7
0033790c: bl       #0x3374e4
00337910: mov      r0, r5
00337914: ldr      r3, [r5]
00337918: add      r1, sp, #4
0033791c: mov      lr, pc
00337920: ldr      pc, [r3, #0x78]
00337924: ldr      r3, [pc, #0x144]
00337928: add      sl, sp, #0x7c
0033792c: add      r8, sp, #0x64
00337930: ldr      r5, [r4, r3]
00337934: add      r7, sp, #0x4c
00337938: add      sb, sp, #0x34
0033793c: mov      r0, r5
00337940: bl       #0x337888
00337944: ldr      r1, [pc, #0x128]
00337948: add      r2, sp, #0x18
0033794c: mov      r0, sl
00337950: add      r1, pc, r1
00337954: bl       #0x3140ec
00337958: mov      r1, sl
0033795c: mov      r2, #0
00337960: mov      r0, r5
00337964: bl       #0x337ddc
00337968: mov      r0, sl
0033796c: bl       #0x318254
00337970: mov      r0, r5
00337974: bl       #0x337888
00337978: ldr      r1, [pc, #0xf8]
0033797c: add      r2, sp, #0x14
00337980: mov      r0, r8
00337984: add      r1, pc, r1
00337988: bl       #0x3140ec
0033798c: mov      r1, r8
00337990: mov      r2, #0
00337994: mov      r0, r5
00337998: bl       #0x337ddc
0033799c: mov      r0, r8
003379a0: bl       #0x318254
003379a4: mov      r0, r5
003379a8: bl       #0x337888
003379ac: ldr      r1, [pc, #0xc8]
003379b0: add      r2, sp, #0x10
003379b4: mov      r0, r7
003379b8: add      r1, pc, r1
003379bc: bl       #0x3140ec
003379c0: mov      r1, r7
003379c4: mov      r2, #0
003379c8: mov      r0, r5
003379cc: bl       #0x337ddc
003379d0: mov      r0, r7
003379d4: bl       #0x318254
003379d8: mov      r0, r5
003379dc: bl       #0x337888
003379e0: ldr      r1, [pc, #0x98]
003379e4: add      r2, sp, #0xc
003379e8: mov      r0, sb
003379ec: add      r1, pc, r1
003379f0: bl       #0x3140ec
003379f4: mov      r1, sb
003379f8: mov      r0, r5
003379fc: bl       #0x337a88
00337a00: mov      r0, sb
00337a04: bl       #0x318254
00337a08: mov      r0, r5
00337a0c: bl       #0x337888
00337a10: ldr      r1, [pc, #0x6c]
00337a14: add      r7, sp, #0x1c
00337a18: add      r2, sp, #8
00337a1c: add      r1, pc, r1
00337a20: mov      r0, r7
00337a24: bl       #0x3140ec
00337a28: mov      r0, r5
00337a2c: mov      r1, r7
00337a30: bl       #0x337a88
00337a34: mov      r0, r7
00337a38: bl       #0x318254
00337a3c: ldr      r3, [r4, r6]
00337a40: ldr      r2, [sp, #0x94]
00337a44: ldr      r3, [r3]
00337a48: cmp      r2, r3
00337a4c: bne      #0x337a58
00337a50: add      sp, sp, #0x98
00337a54: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
00337a58: bl       #0x30e310

# _ZN16CharStateMachine9_SetStateEiiPv
003c1938: push     {r4, r5, r6, r7, r8, sl, lr}
003c193c: ldr      ip, [r0, #0x20]
003c1940: sub      sp, sp, #0x14
003c1944: mov      r4, r0
003c1948: cmp      ip, #0
003c194c: mvneq    r7, #0
003c1950: mov      r5, r1
003c1954: mov      r8, r2
003c1958: mov      sl, r3
003c195c: moveq    r6, r7
003c1960: beq      #0x3c1990
003c1964: ldr      r3, [ip, #4]
003c1968: ldr      r6, [ip]
003c196c: ldr      r2, [r0, #4]
003c1970: ldr      ip, [r3]
003c1974: mov      r0, r3
003c1978: str      r1, [sp]
003c197c: mov      r3, r4
003c1980: mov      r1, r6
003c1984: mov      lr, pc
003c1988: ldr      pc, [ip, #0x10]
003c198c: mov      r7, r6
003c1990: mov      r0, r4
003c1994: mov      r1, r5
003c1998: bl       #0x3c0084
003c199c: cmp      r0, #0
003c19a0: streq    r0, [r4, #0x20]
003c19a4: bne      #0x3c19c0
003c19a8: ldr      r0, [r4, #4]
003c19ac: mov      r2, r7
003c19b0: mov      r1, #0x1d
003c19b4: add      sp, sp, #0x14
003c19b8: pop      {r4, r5, r6, r7, r8, sl, lr}
003c19bc: b        #0x3a4d5c
003c19c0: mov      r1, r5
003c19c4: mov      r0, r4
003c19c8: bl       #0x3c184c
003c19cc: cmp      r6, r5
003c19d0: movne    r3, #0
003c19d4: str      r0, [r4, #0x20]
003c19d8: strne    r3, [r4, #0x60]
003c19dc: ldm      r0, {r1, r3}
003c19e0: ldr      r2, [r4, #4]
003c19e4: ldr      ip, [r3]
003c19e8: mov      r0, r3
003c19ec: stm      sp, {r6, r8, sl}
003c19f0: mov      r3, r4
003c19f4: mov      lr, pc
003c19f8: ldr      pc, [ip, #0xc]
003c19fc: b        #0x3c19a8

# _ZNSsD1Ev
00318254: push     {r4, lr}
00318258: mov      r4, r0
0031825c: ldr      r0, [r0, #0x14]
00318260: cmp      r0, r4
00318264: beq      #0x318284
00318268: cmp      r0, #0
0031826c: beq      #0x318284
00318270: ldr      r1, [r4]
00318274: rsb      r1, r0, r1
00318278: cmp      r1, #0x80
0031827c: bhi      #0x31828c
00318280: bl       #0x708f00
00318284: mov      r0, r4
00318288: pop      {r4, pc}
0031828c: bl       #0x310440
00318290: mov      r0, r4
00318294: pop      {r4, pc}

# _ZN16CharStateMachine15RaiseStateEventEiPv
003c5684: push     {r4, r5, r6, r7, r8, sb, sl, lr}
003c5688: ldr      r5, [pc, #0x1e0]
003c568c: ldr      r7, [pc, #0x1e0]
003c5690: mov      r6, r1
003c5694: add      r5, pc, r5
003c5698: ldr      r1, [r5, r7]
003c569c: sub      sp, sp, #0x30
003c56a0: sub      r3, r6, #0x2a
003c56a4: ldr      r1, [r1]
003c56a8: mov      r4, r0
003c56ac: mov      r8, r2
003c56b0: str      r1, [sp, #0x2c]
003c56b4: cmp      r3, #6
003c56b8: addls    pc, pc, r3, lsl #2
003c56bc: b        #0x3c5700
003c56c0: b        #0x3c584c
003c56c4: b        #0x3c583c
003c56c8: b        #0x3c582c
003c56cc: b        #0x3c5700
003c56d0: b        #0x3c5700
003c56d4: b        #0x3c5700
003c56d8: b        #0x3c56dc
003c56dc: mov      r1, #0
003c56e0: bl       #0x3c0260
003c56e4: cmp      r0, #0
003c56e8: beq      #0x3c5700
003c56ec: ldr      r3, [r4, #4]
003c56f0: ldr      r0, [r3, #0x2dc]
003c56f4: cmp      r0, #0
003c56f8: beq      #0x3c5700
003c56fc: bl       #0x46eb20
003c5700: ldr      r3, [r4, #0x20]
003c5704: cmp      r3, #0
003c5708: beq      #0x3c574c
003c570c: ldm      r3, {r1, r3}
003c5710: ldr      r2, [r4, #4]
003c5714: ldr      ip, [r3]
003c5718: mov      r0, r3
003c571c: str      r6, [sp]
003c5720: mov      r3, r4
003c5724: str      r8, [sp, #4]
003c5728: mov      lr, pc
003c572c: ldr      pc, [ip, #0x18]
003c5730: ldr      r3, [r4, #0x20]
003c5734: mov      r0, r4
003c5738: mov      r2, r6
003c573c: ldr      r1, [r3]
003c5740: bl       #0x3c00e0
003c5744: cmp      r0, #0
003c5748: bne      #0x3c5768
003c574c: ldr      r3, [r5, r7]
003c5750: ldr      r2, [sp, #0x2c]
003c5754: ldr      r3, [r3]
003c5758: cmp      r2, r3
003c575c: bne      #0x3c586c
003c5760: add      sp, sp, #0x30
003c5764: pop      {r4, r5, r6, r7, r8, sb, sl, pc}
003c5768: ldr      r3, [pc, #0x108]
003c576c: add      sl, sp, #0x14
003c5770: ldr      sb, [r5, r3]
003c5774: mov      r0, sb
003c5778: bl       #0x337888
003c577c: ldr      r1, [pc, #0xf8]
003c5780: add      r2, sp, #0x10
003c5784: mov      r0, sl
003c5788: add      r1, pc, r1
003c578c: bl       #0x3140ec
003c5790: mov      r1, sl
003c5794: mov      r0, sb
003c5798: bl       #0x337a88
003c579c: mov      r0, sl
003c57a0: bl       #0x318254
003c57a4: ldr      r3, [r4, #0x20]
003c57a8: mov      r0, r4
003c57ac: mov      r2, r6
003c57b0: ldr      r1, [r3]
003c57b4: bl       #0x3c1694
003c57b8: ldr      r1, [r0, #8]
003c57bc: str      r1, [sp, #0xc]
003c57c0: ldr      r3, [r0]
003c57c4: cmp      r3, #0
003c57c8: beq      #0x3c585c
003c57cc: ldr      r3, [r0, #4]
003c57d0: ldr      r2, [r4, #4]
003c57d4: tst      r3, #1
003c57d8: ldrne    r1, [r0]
003c57dc: ldrne    ip, [r2, r3, asr #1]
003c57e0: addne    r0, r2, r3, asr #1
003c57e4: ldreq    ip, [r0]
003c57e8: addeq    r0, r2, r3, asr #1
003c57ec: ldr      r3, [r4, #0x20]
003c57f0: add      r2, sp, #0xc
003c57f4: ldrne    ip, [ip, r1]
003c57f8: ldr      r3, [r3]
003c57fc: mov      r1, r6
003c5800: str      r2, [sp]
003c5804: mov      r2, r8
003c5808: blx      ip
003c580c: cmp      r0, #0
003c5810: beq      #0x3c574c
003c5814: ldr      r1, [sp, #0xc]
003c5818: mov      r0, r4
003c581c: mov      r2, r6
003c5820: mov      r3, r8
003c5824: bl       #0x3c1938
003c5828: b        #0x3c574c
003c582c: ldr      r3, [r0, #0x2c]
003c5830: bic      r3, r3, #4
003c5834: str      r3, [r0, #0x2c]
003c5838: b        #0x3c5700
003c583c: ldr      r3, [r0, #0x2c]
003c5840: bic      r3, r3, #2
003c5844: str      r3, [r0, #0x2c]
003c5848: b        #0x3c5700
003c584c: ldr      r3, [r0, #0x2c]
003c5850: bic      r3, r3, #1
003c5854: str      r3, [r0, #0x2c]
003c5858: b        #0x3c5700
003c585c: ldr      r3, [r0, #4]
003c5860: tst      r3, #1
003c5864: beq      #0x3c5818
003c5868: b        #0x3c57cc
003c586c: bl       #0x30e310
003c5870: ldrsheq  pc, [ip], #-0x3c
003c5874: andeq    r4, r0, ip, lsr #1
003c5878: andeq    r0, r0, r4, lsl #17
003c587c: subeq    pc, pc, r8, lsr #15

# _ZN9LuaScript4CallEPKc
0037c514: ldr      r3, [pc, #0x60]
0037c518: ldr      r2, [pc, #0x60]
0037c51c: push     {r4, r5, r6, r7, lr}
0037c520: add      r3, pc, r3
0037c524: ldr      r5, [r3, r2]
0037c528: sub      sp, sp, #0x34
0037c52c: add      r4, sp, #4
0037c530: ldr      r2, [r5]
0037c534: mov      r6, r0
0037c538: mov      r7, r1
0037c53c: mov      r0, r4
0037c540: str      r2, [sp, #0x2c]
0037c544: bl       #0x31b434
0037c548: mov      r2, r4
0037c54c: mov      r0, r6
0037c550: mov      r1, r7
0037c554: bl       #0x37c494
0037c558: mov      r0, r4
0037c55c: bl       #0x31b398
0037c560: ldr      r2, [sp, #0x2c]
0037c564: ldr      r3, [r5]
0037c568: cmp      r2, r3
0037c56c: bne      #0x37c578
0037c570: add      sp, sp, #0x34
0037c574: pop      {r4, r5, r6, r7, pc}
0037c578: bl       #0x30e310
0037c57c: rsbeq    r8, r1, r0, ror r5
0037c580: andeq    r4, r0, ip, lsr #1

# _ZN6CSIdle6OnBlurEiP9CharacterP16CharStateMachinei
003c2d3c: ldr      r3, [pc, #0x7c]
003c2d40: ldr      r1, [pc, #0x7c]
003c2d44: push     {r4, r5, r6, r7, lr}
003c2d48: add      r3, pc, r3
003c2d4c: ldr      r5, [r3, r1]
003c2d50: ldr      r1, [pc, #0x70]
003c2d54: mov      r7, r2
003c2d58: ldr      r2, [r5]
003c2d5c: ldr      r6, [r3, r1]
003c2d60: sub      sp, sp, #0x24
003c2d64: str      r2, [sp, #0x1c]
003c2d68: mov      r0, r6
003c2d6c: bl       #0x337888
003c2d70: ldr      r1, [pc, #0x54]
003c2d74: add      r4, sp, #4
003c2d78: mov      r2, sp
003c2d7c: add      r1, pc, r1
003c2d80: mov      r0, r4
003c2d84: bl       #0x3140ec
003c2d88: mov      r1, r4
003c2d8c: mov      r0, r6
003c2d90: bl       #0x337a88
003c2d94: mov      r0, r4
003c2d98: bl       #0x318254
003c2d9c: mov      r3, #0
003c2da0: strb     r3, [r7, #0x538]
003c2da4: ldr      r2, [sp, #0x1c]
003c2da8: ldr      r3, [r5]
003c2dac: cmp      r2, r3
003c2db0: bne      #0x3c2dbc
003c2db4: add      sp, sp, #0x24
003c2db8: pop      {r4, r5, r6, r7, pc}
003c2dbc: bl       #0x30e310
003c2dc0: subseq   r1, sp, r8, asr #26
003c2dc4: andeq    r4, r0, ip, lsr #1
003c2dc8: andeq    r0, r0, r4, lsl #17
003c2dcc: ldrsbeq  r2, [r0], #-4

# _ZN10AISDefault11OnEndOfAnimEv
003dbeec: bx       lr

# _ZN9Character10RaiseEventEiPv
003a4d5c: cmp      r1, #0x36
003a4d60: beq      #0x3a4d6c
003a4d64: add      r0, r0, #0x3c8
003a4d68: b        #0x3cbb34
003a4d6c: add      r0, r0, #0x560
003a4d70: mov      r1, r2
003a4d74: b        #0x3e123c

# _ZN6CSIdle7OnEventEiP9CharacterP16CharStateMachineiPv
003c0004: bx       lr

# _ZN6CharAI18_OnAnimSequenceEndEv
003d3d30: push     {r4, lr}
003d3d34: ldr      r0, [r0, #4]
003d3d38: add      r0, r0, #0x4f0
003d3d3c: add      r0, r0, #0xc
003d3d40: bl       #0x3c01ac
003d3d44: mov      r0, #1
003d3d48: pop      {r4, pc}

# _ZN6CharAI14OnStateChangedEii
003d0bec: push     {r4, lr}
003d0bf0: ldr      r3, [r0, #0x1c]
003d0bf4: cmp      r3, #0
003d0bf8: beq      #0x3d0c0c
003d0bfc: mov      r0, r3
003d0c00: ldr      r3, [r3]
003d0c04: mov      lr, pc
003d0c08: ldr      pc, [r3, #0x20]
003d0c0c: pop      {r4, pc}

# _ZN6CharAI14_OnAnimStepEndEv
003d3ff8: push     {r4, lr}
003d3ffc: mov      r4, r0
003d4000: ldr      r0, [r0, #4]
003d4004: add      r0, r0, #0x4f0
003d4008: add      r0, r0, #0xc
003d400c: bl       #0x3c01ac
003d4010: cmp      r0, #5
003d4014: beq      #0x3d4038
003d4018: bge      #0x3d4024
003d401c: mov      r0, #1
003d4020: pop      {r4, pc}
003d4024: cmp      r0, #7
003d4028: bgt      #0x3d401c
003d402c: mov      r0, r4
003d4030: pop      {r4, lr}
003d4034: b        #0x3d3d68
003d4038: mov      r0, r4
003d403c: pop      {r4, lr}
003d4040: b        #0x3d3e44

# _ZN10AISDefault14OnStateChangedEii
003dbe8c: bx       lr

# _ZNK16CharStateMachine11SM_GetStateEv
003c01ac: ldr      r3, [r0, #0x20]
003c01b0: cmp      r3, #0
003c01b4: mvneq    r0, #0
003c01b8: ldrne    r0, [r3]
003c01bc: bx       lr

# _ZN13DebugSwitches9GetSwitchERKSs
00337a88: push     {r4, r5, r6, r7, r8, sl, lr}
00337a8c: ldr      r4, [pc, #0xa8]
00337a90: ldr      r5, [pc, #0xa8]
00337a94: sub      sp, sp, #0x24
00337a98: add      r4, pc, r4
00337a9c: ldr      r3, [r4, r5]
00337aa0: mov      r6, r0
00337aa4: mov      r7, r1
00337aa8: ldr      r3, [r3]
00337aac: str      r3, [sp, #0x1c]
00337ab0: bl       #0x3369a8
00337ab4: cmp      r6, r0
00337ab8: beq      #0x337ae8
00337abc: mov      r0, r6
00337ac0: mov      r1, r7
00337ac4: bl       #0x337288
00337ac8: ldr      r3, [r4, r5]
00337acc: ldr      r2, [sp, #0x1c]
00337ad0: ldrb     r0, [r0]
00337ad4: ldr      r3, [r3]
00337ad8: cmp      r2, r3
00337adc: bne      #0x337b38
00337ae0: add      sp, sp, #0x24
00337ae4: pop      {r4, r5, r6, r7, r8, sl, pc}
00337ae8: mov      r1, r7
00337aec: bl       #0x337288
00337af0: mov      r3, #0
00337af4: strb     r3, [r0]
00337af8: ldr      r3, [pc, #0x44]
00337afc: add      r8, sp, #4
00337b00: ldr      sl, [r4, r3]
00337b04: mov      r0, sl
00337b08: bl       #0x337888
00337b0c: ldr      r1, [pc, #0x34]
00337b10: mov      r2, sp
00337b14: mov      r0, r8
00337b18: add      r1, pc, r1
00337b1c: bl       #0x3140ec
00337b20: mov      r0, sl
00337b24: mov      r1, r8
00337b28: bl       #0x337a88
00337b2c: mov      r0, r8
00337b30: bl       #0x318254
00337b34: b        #0x337abc
00337b38: bl       #0x30e310

# _ZN11AISExternal11OnEndOfAnimEv
003dccd0: push     {r4, lr}
003dccd4: mov      r4, r0
003dccd8: bl       #0x3dbeec
003dccdc: ldr      r1, [pc, #0xc]
003dcce0: mov      r0, r4
003dcce4: add      r1, pc, r1
003dcce8: pop      {r4, lr}
003dccec: b        #0x37c514
003dccf0: subeq    r8, lr, r4, asr #25

# _ZN6CharAI16_OnAnimStepBeginEv
003d4204: push     {r4, lr}
003d4208: mov      r4, r0
003d420c: ldr      r0, [r0, #4]
003d4210: add      r0, r0, #0x4f0
003d4214: add      r0, r0, #0xc
003d4218: bl       #0x3c01ac
003d421c: sub      r0, r0, #4
003d4220: cmp      r0, #3
003d4224: addls    pc, pc, r0, lsl #2
003d4228: b        #0x3d4260
003d422c: b        #0x3d4254
003d4230: b        #0x3d4248
003d4234: b        #0x3d423c
003d4238: b        #0x3d423c
003d423c: mov      r0, r4
003d4240: pop      {r4, lr}
003d4244: b        #0x3d3dd4
003d4248: mov      r0, r4
003d424c: pop      {r4, lr}
003d4250: b        #0x3d4044
003d4254: mov      r0, r4
003d4258: pop      {r4, lr}
003d425c: b        #0x3d4120
003d4260: mov      r0, #1
003d4264: pop      {r4, pc}

# _ZN6CharAI11OnEndOfAnimEv
003d0ce8: push     {r4, lr}
003d0cec: ldr      r3, [r0, #0x1c]
003d0cf0: cmp      r3, #0
003d0cf4: beq      #0x3d0d08
003d0cf8: mov      r0, r3
003d0cfc: ldr      r3, [r3]
003d0d00: mov      lr, pc
003d0d04: ldr      pc, [r3, #0x98]
003d0d08: pop      {r4, pc}

# _ZN6CharAI12RaiseAIEventEiPv
003cbb34: ldr      r3, [pc, #0x6d4]
003cbb38: push     {r4, r5, r6, r7, r8, lr}
003cbb3c: add      r3, pc, r3
003cbb40: mov      r4, r1
003cbb44: mov      r5, r0
003cbb48: mov      r6, r2
003cbb4c: cmp      r1, #0x3f
003cbb50: addls    pc, pc, r1, lsl #2
003cbb54: b        #0x3cbcfc
003cbb58: b        #0x3cbc58
003cbb5c: b        #0x3cbe90
003cbb60: b        #0x3cbe98
003cbb64: b        #0x3cbe2c
003cbb68: b        #0x3cbcfc
003cbb6c: b        #0x3cbcfc
003cbb70: b        #0x3cbcfc
003cbb74: b        #0x3cbcfc
003cbb78: b        #0x3cbcfc
003cbb7c: b        #0x3cbcfc
003cbb80: b        #0x3cbcfc
003cbb84: b        #0x3cbcfc
003cbb88: b        #0x3cbcfc
003cbb8c: b        #0x3cbcfc
003cbb90: b        #0x3cbcfc
003cbb94: b        #0x3cbcfc
003cbb98: b        #0x3cbcfc
003cbb9c: b        #0x3cbcfc
003cbba0: b        #0x3cbcfc
003cbba4: b        #0x3cbcfc
003cbba8: b        #0x3cbcfc
003cbbac: b        #0x3cbcfc
003cbbb0: b        #0x3cbcfc
003cbbb4: b        #0x3cbcfc
003cbbb8: b        #0x3cbcfc
003cbbbc: b        #0x3cbcfc
003cbbc0: b        #0x3cbcfc
003cbbc4: b        #0x3cbcfc
003cbbc8: b        #0x3cbcfc
003cbbcc: b        #0x3cbcfc
003cbbd0: b        #0x3cbcfc
003cbbd4: b        #0x3cbcfc
003cbbd8: b        #0x3cbcfc
003cbbdc: b        #0x3cbcfc
003cbbe0: b        #0x3cbe40
003cbbe4: b        #0x3cbe64
003cbbe8: b        #0x3cbcfc
003cbbec: b        #0x3cbcfc
003cbbf0: b        #0x3cbcfc
003cbbf4: b        #0x3cbcfc
003cbbf8: b        #0x3cbe80
003cbbfc: b        #0x3cbc78
003cbc00: b        #0x3cbcfc
003cbc04: b        #0x3cbcfc
003cbc08: b        #0x3cbcfc
003cbc0c: b        #0x3cbcfc
003cbc10: b        #0x3cbcfc
003cbc14: b        #0x3cbcfc
003cbc18: b        #0x3cbc5c
003cbc1c: b        #0x3cbc8c
003cbc20: b        #0x3cbc98
003cbc24: b        #0x3cbca4
003cbc28: b        #0x3cbcac
003cbc2c: b        #0x3cbcbc
003cbc30: b        #0x3cbcfc
003cbc34: b        #0x3cbcfc
003cbc38: b        #0x3cbcfc
003cbc3c: b        #0x3cbcfc
003cbc40: b        #0x3cbcfc
003cbc44: b        #0x3cbcfc
003cbc48: b        #0x3cbcfc
003cbc4c: b        #0x3cbcfc
003cbc50: b        #0x3cbcfc
003cbc54: b        #0x3cbcf0
003cbc58: movw     r4, #0xc351
003cbc5c: ldr      r0, [r5, #4]
003cbc60: add      r0, r0, #0x4f0
003cbc64: add      r0, r0, #0xc
003cbc68: mov      r1, r4
003cbc6c: mov      r2, r6
003cbc70: pop      {r4, r5, r6, r7, r8, lr}
003cbc74: b        #0x3c5684
003cbc78: ldr      r3, [r0]
003cbc7c: mov      r1, r2
003cbc80: mov      lr, pc
003cbc84: ldr      pc, [r3, #0x80]
003cbc88: b        #0x3cbc5c
003cbc8c: mov      r3, #0
003cbc90: strb     r3, [r0, #0x18]
003cbc94: pop      {r4, r5, r6, r7, r8, pc}
003cbc98: mov      r3, #1
003cbc9c: strb     r3, [r0, #0x4a]
003cbca0: pop      {r4, r5, r6, r7, r8, pc}
003cbca4: bl       #0x3cb77c
003cbca8: pop      {r4, r5, r6, r7, r8, pc}
003cbcac: ldr      r0, [r0, #4]
003cbcb0: add      r0, r0, #0x560
003cbcb4: bl       #0x3df3f0
003cbcb8: pop      {r4, r5, r6, r7, r8, pc}
003cbcbc: ldr      r3, [r0]
003cbcc0: cmp      r2, #0
003cbcc4: mvneq    r1, #0
003cbcc8: ldr      r4, [r3, #0x90]
003cbccc: beq      #0x3cbce4
003cbcd0: mov      r0, r2
003cbcd4: ldr      r3, [r2]
003cbcd8: mov      lr, pc
003cbcdc: ldr      pc, [r3]
003cbce0: mov      r1, r0
003cbce4: mov      r0, r5
003cbce8: blx      r4
003cbcec: pop      {r4, r5, r6, r7, r8, pc}
003cbcf0: ldr      r0, [r0, #4]
003cbcf4: bl       #0x394a3c
003cbcf8: b        #0x3cbc5c
003cbcfc: ldr      r0, [r0, #4]
003cbd00: ldr      r1, [r0, #0x378]
003cbd04: ldrb     r2, [r1, #9]
003cbd08: cmp      r2, #0
003cbd0c: bne      #0x3cbd30
003cbd10: ldr      r2, [pc, #0x4fc]
003cbd14: ldr      r3, [r3, r2]
003cbd18: ldrb     r3, [r3]
003cbd1c: cmp      r3, #0
003cbd20: bne      #0x3cbc5c
003cbd24: ldrb     r3, [r1, #8]
003cbd28: cmp      r3, #0
003cbd2c: bne      #0x3cbc5c
003cbd30: sub      r3, r4, #4
003cbd34: cmp      r3, #0x3a
003cbd38: addls    pc, pc, r3, lsl #2
003cbd3c: b        #0x3cbc60
003cbd40: b        #0x3cc1f8
003cbd44: b        #0x3cbc60
003cbd48: b        #0x3cbc60
003cbd4c: b        #0x3cc1e0
003cbd50: b        #0x3cc1c8
003cbd54: b        #0x3cc1ac
003cbd58: b        #0x3cc198
003cbd5c: b        #0x3cc184
003cbd60: b        #0x3cc170
003cbd64: b        #0x3cc15c
003cbd68: b        #0x3cc148
003cbd6c: b        #0x3cc134
003cbd70: b        #0x3cc120
003cbd74: b        #0x3cc10c
003cbd78: b        #0x3cc0f8
003cbd7c: b        #0x3cc0e4
003cbd80: b        #0x3cc0d0
003cbd84: b        #0x3cc0bc
003cbd88: b        #0x3cc0a8
003cbd8c: b        #0x3cc094
003cbd90: b        #0x3cc080
003cbd94: b        #0x3cc06c
003cbd98: b        #0x3cbc60
003cbd9c: b        #0x3cbc60
003cbda0: b        #0x3cbc60
003cbda4: b        #0x3cc044
003cbda8: b        #0x3cc038
003cbdac: b        #0x3cc02c
003cbdb0: b        #0x3cc020
003cbdb4: b        #0x3cc014
003cbdb8: b        #0x3cbc60
003cbdbc: b        #0x3cbc60
003cbdc0: b        #0x3cc004
003cbdc4: b        #0x3cbff4
003cbdc8: b        #0x3cbfe4
003cbdcc: b        #0x3cbfd4
003cbdd0: b        #0x3cbc60
003cbdd4: b        #0x3cbc60
003cbdd8: b        #0x3cbfbc
003cbddc: b        #0x3cbfa4
003cbde0: b        #0x3cbf8c
003cbde4: b        #0x3cbc60
003cbde8: b        #0x3cbc60
003cbdec: b        #0x3cbc60
003cbdf0: b        #0x3cbc60
003cbdf4: b        #0x3cbc60
003cbdf8: b        #0x3cbc60
003cbdfc: b        #0x3cbc60
003cbe00: b        #0x3cbc60
003cbe04: b        #0x3cbc60
003cbe08: b        #0x3cbc60
003cbe0c: b        #0x3cbf70
003cbe10: b        #0x3cbf54
003cbe14: b        #0x3cbf38
003cbe18: b        #0x3cbf1c
003cbe1c: b        #0x3cbf00
003cbe20: b        #0x3cbee4
003cbe24: b        #0x3cbec8
003cbe28: b        #0x3cbeac
003cbe2c: mov      r1, r2
003cbe30: ldr      r3, [r5]
003cbe34: mov      lr, pc
003cbe38: ldr      pc, [r3, #0x28]
003cbe3c: pop      {r4, r5, r6, r7, r8, pc}
003cbe40: bl       #0x3d3aec
003cbe44: ldr      r3, [r5]
003cbe48: mov      r7, r0
003cbe4c: mov      r0, r5
003cbe50: mov      lr, pc
003cbe54: ldr      pc, [r3, #0x98]
003cbe58: cmp      r7, #0
003cbe5c: bne      #0x3cbc5c
003cbe60: pop      {r4, r5, r6, r7, r8, pc}
003cbe64: bl       #0x3d3ae4
003cbe68: ldr      r3, [r5]
003cbe6c: mov      r7, r0
003cbe70: mov      r0, r5
003cbe74: mov      lr, pc
003cbe78: ldr      pc, [r3, #0x98]
003cbe7c: b        #0x3cbe58
003cbe80: mov      r1, r2
003cbe84: bl       #0x3d4434
003cbe88: mov      r7, r0
003cbe8c: b        #0x3cbe58
003cbe90: movw     r4, #0xc352
003cbe94: b        #0x3cbc5c
003cbe98: ldr      r3, [r0]
003cbe9c: mov      r1, r2
003cbea0: mov      lr, pc
003cbea4: ldr      pc, [r3, #0x24]
003cbea8: b        #0x3cbc5c
003cbeac: mov      r0, r5
003cbeb0: mov      r1, r6
003cbeb4: ldr      r3, [r5]
003cbeb8: mov      r2, #0
003cbebc: mov      lr, pc
003cbec0: ldr      pc, [r3, #0xc8]
003cbec4: pop      {r4, r5, r6, r7, r8, pc}
003cbec8: mov      r0, r5
003cbecc: mov      r1, r6
003cbed0: ldr      r3, [r5]
003cbed4: mov      r2, #1
003cbed8: mov      lr, pc
003cbedc: ldr      pc, [r3, #0xc8]
003cbee0: pop      {r4, r5, r6, r7, r8, pc}
003cbee4: mov      r0, r5
003cbee8: mov      r1, r6
003cbeec: ldr      r3, [r5]
003cbef0: mov      r2, #0
003cbef4: mov      lr, pc
003cbef8: ldr      pc, [r3, #0xc4]
003cbefc: pop      {r4, r5, r6, r7, r8, pc}
003cbf00: mov      r0, r5
003cbf04: mov      r1, r6
003cbf08: ldr      r3, [r5]
003cbf0c: mov      r2, #1
003cbf10: mov      lr, pc
003cbf14: ldr      pc, [r3, #0xc4]
003cbf18: pop      {r4, r5, r6, r7, r8, pc}
003cbf1c: mov      r0, r5
003cbf20: mov      r1, r6
003cbf24: ldr      r3, [r5]
003cbf28: mov      r2, #0
003cbf2c: mov      lr, pc
003cbf30: ldr      pc, [r3, #0xc0]
003cbf34: pop      {r4, r5, r6, r7, r8, pc}
003cbf38: mov      r0, r5
003cbf3c: mov      r1, r6
003cbf40: ldr      r3, [r5]
003cbf44: mov      r2, #1
003cbf48: mov      lr, pc
003cbf4c: ldr      pc, [r3, #0xc0]
003cbf50: pop      {r4, r5, r6, r7, r8, pc}
003cbf54: mov      r0, r5
003cbf58: mov      r1, r6
003cbf5c: ldr      r3, [r5]
003cbf60: mov      r2, #0
003cbf64: mov      lr, pc
003cbf68: ldr      pc, [r3, #0xbc]
003cbf6c: pop      {r4, r5, r6, r7, r8, pc}
003cbf70: mov      r0, r5
003cbf74: mov      r1, r6
003cbf78: ldr      r3, [r5]
003cbf7c: mov      r2, #1
003cbf80: mov      lr, pc
003cbf84: ldr      pc, [r3, #0xbc]
003cbf88: pop      {r4, r5, r6, r7, r8, pc}
003cbf8c: mov      r0, r5
003cbf90: ldr      r3, [r5]
003cbf94: mov      lr, pc
003cbf98: ldr      pc, [r3, #0x88]
003cbf9c: ldr      r0, [r5, #4]
003cbfa0: b        #0x3cbc60
003cbfa4: mov      r0, r5
003cbfa8: ldr      r3, [r5]
003cbfac: mov      lr, pc
003cbfb0: ldr      pc, [r3, #0x84]
003cbfb4: ldr      r0, [r5, #4]
003cbfb8: b        #0x3cbc60
003cbfbc: mov      r0, r5
003cbfc0: ldr      r3, [r5]
003cbfc4: mov      lr, pc
003cbfc8: ldr      pc, [r3, #0x8c]
003cbfcc: ldr      r0, [r5, #4]
003cbfd0: b        #0x3cbc60
003cbfd4: mov      r0, r5
003cbfd8: bl       #0x3d3ff8
003cbfdc: mov      r7, r0
003cbfe0: b        #0x3cbe58
003cbfe4: mov      r0, r5
003cbfe8: bl       #0x3d4204
003cbfec: mov      r7, r0
003cbff0: b        #0x3cbe58
003cbff4: mov      r0, r5
003cbff8: bl       #0x3d3d30
003cbffc: mov      r7, r0
003cc000: b        #0x3cbe58
003cc004: mov      r0, r5
003cc008: bl       #0x3d3d4c
003cc00c: mov      r7, r0
003cc010: b        #0x3cbe58
003cc014: mov      r0, r5
003cc018: pop      {r4, r5, r6, r7, r8, lr}
003cc01c: b        #0x3d8b28
003cc020: mov      r0, r5
003cc024: pop      {r4, r5, r6, r7, r8, lr}
003cc028: b        #0x3d8038
003cc02c: mov      r0, r5
003cc030: pop      {r4, r5, r6, r7, r8, lr}
003cc034: b        #0x3d8b7c
003cc038: mov      r0, r5
003cc03c: pop      {r4, r5, r6, r7, r8, lr}
003cc040: b        #0x3d808c
003cc044: ldr      r3, [r5]
003cc048: add      r0, r0, #0x4f0
003cc04c: add      r0, r0, #0xc
003cc050: ldr      r4, [r3, #0x20]
003cc054: bl       #0x3c01ac
003cc058: mov      r1, r6
003cc05c: mov      r2, r0
003cc060: mov      r0, r5
003cc064: blx      r4
003cc068: pop      {r4, r5, r6, r7, r8, pc}
003cc06c: mov      r0, r5
003cc070: ldr      r3, [r5]
003cc074: mov      lr, pc
003cc078: ldr      pc, [r3, #0x7c]
003cc07c: pop      {r4, r5, r6, r7, r8, pc}
003cc080: mov      r0, r5
003cc084: ldr      r3, [r5]
003cc088: mov      lr, pc
003cc08c: ldr      pc, [r3, #0x78]
003cc090: pop      {r4, r5, r6, r7, r8, pc}
003cc094: mov      r0, r5
003cc098: ldr      r3, [r5]
003cc09c: mov      lr, pc
003cc0a0: ldr      pc, [r3, #0x74]
003cc0a4: pop      {r4, r5, r6, r7, r8, pc}
003cc0a8: mov      r0, r5
003cc0ac: ldr      r3, [r5]
003cc0b0: mov      lr, pc
003cc0b4: ldr      pc, [r3, #0x70]
003cc0b8: pop      {r4, r5, r6, r7, r8, pc}
003cc0bc: mov      r0, r5
003cc0c0: ldr      r3, [r5]
003cc0c4: mov      lr, pc
003cc0c8: ldr      pc, [r3, #0x6c]
003cc0cc: pop      {r4, r5, r6, r7, r8, pc}
003cc0d0: mov      r0, r5
003cc0d4: ldr      r3, [r5]
003cc0d8: mov      lr, pc
003cc0dc: ldr      pc, [r3, #0x68]
003cc0e0: pop      {r4, r5, r6, r7, r8, pc}
003cc0e4: mov      r0, r5
003cc0e8: ldr      r3, [r5]
003cc0ec: mov      lr, pc
003cc0f0: ldr      pc, [r3, #0x64]
003cc0f4: pop      {r4, r5, r6, r7, r8, pc}
003cc0f8: mov      r0, r5
003cc0fc: ldr      r3, [r5]
003cc100: mov      lr, pc
003cc104: ldr      pc, [r3, #0x60]
003cc108: pop      {r4, r5, r6, r7, r8, pc}
003cc10c: mov      r0, r5
003cc110: ldr      r3, [r5]
003cc114: mov      lr, pc
003cc118: ldr      pc, [r3, #0x5c]
003cc11c: pop      {r4, r5, r6, r7, r8, pc}
003cc120: mov      r0, r5
003cc124: ldr      r3, [r5]
003cc128: mov      lr, pc
003cc12c: ldr      pc, [r3, #0x58]
003cc130: pop      {r4, r5, r6, r7, r8, pc}
003cc134: mov      r0, r5
003cc138: ldr      r3, [r5]
003cc13c: mov      lr, pc
003cc140: ldr      pc, [r3, #0x54]
003cc144: pop      {r4, r5, r6, r7, r8, pc}
003cc148: mov      r0, r5
003cc14c: ldr      r3, [r5]
003cc150: mov      lr, pc
003cc154: ldr      pc, [r3, #0x50]
003cc158: pop      {r4, r5, r6, r7, r8, pc}
003cc15c: mov      r0, r5
003cc160: ldr      r3, [r5]
003cc164: mov      lr, pc
003cc168: ldr      pc, [r3, #0x4c]
003cc16c: pop      {r4, r5, r6, r7, r8, pc}
003cc170: mov      r0, r5
003cc174: ldr      r3, [r5]
003cc178: mov      lr, pc
003cc17c: ldr      pc, [r3, #0x48]
003cc180: pop      {r4, r5, r6, r7, r8, pc}
003cc184: mov      r0, r5
003cc188: ldr      r3, [r5]
003cc18c: mov      lr, pc
003cc190: ldr      pc, [r3, #0x44]
003cc194: pop      {r4, r5, r6, r7, r8, pc}
003cc198: mov      r0, r5
003cc19c: ldr      r3, [r5]
003cc1a0: mov      lr, pc
003cc1a4: ldr      pc, [r3, #0x40]
003cc1a8: pop      {r4, r5, r6, r7, r8, pc}
003cc1ac: mov      r0, r5
003cc1b0: ldr      r3, [r5]
003cc1b4: mov      r1, r6
003cc1b8: mov      lr, pc
003cc1bc: ldr      pc, [r3, #0x34]
003cc1c0: ldr      r0, [r5, #4]
003cc1c4: b        #0x3cbc60
003cc1c8: mov      r0, r5
003cc1cc: mov      r1, r6
003cc1d0: ldr      r3, [r5]
003cc1d4: mov      lr, pc
003cc1d8: ldr      pc, [r3, #0x30]
003cc1dc: pop      {r4, r5, r6, r7, r8, pc}
003cc1e0: mov      r0, r5
003cc1e4: mov      r1, r6
003cc1e8: ldr      r3, [r5]
003cc1ec: mov      lr, pc
003cc1f0: ldr      pc, [r3, #0x2c]
003cc1f4: pop      {r4, r5, r6, r7, r8, pc}
003cc1f8: mov      r0, r5
003cc1fc: mov      r1, r6
003cc200: ldr      r3, [r5]
003cc204: mov      lr, pc
003cc208: ldr      pc, [r3, #0xb0]
003cc20c: pop      {r4, r5, r6, r7, r8, pc}
003cc210: subseq   r8, ip, r4, asr pc
003cc214: andeq    r3, r0, r0, asr r6

# _ZN6CharAI20_OnAnimSequenceBeginEv
003d3d4c: push     {r4, lr}
003d3d50: ldr      r0, [r0, #4]
003d3d54: add      r0, r0, #0x4f0
003d3d58: add      r0, r0, #0xc
003d3d5c: bl       #0x3c01ac
003d3d60: mov      r0, #1
003d3d64: pop      {r4, pc}

# _ZN6CSIdle7OnFocusEiP9CharacterP16CharStateMachineiiPv
003c3020: push     {r4, r5, r6, r7, r8, lr}
003c3024: ldr      r4, [pc, #0xf4]
003c3028: ldr      r7, [pc, #0xf4]
003c302c: ldr      r1, [pc, #0xf4]
003c3030: add      r4, pc, r4
003c3034: ldr      r3, [r4, r7]
003c3038: ldr      r8, [r4, r1]
003c303c: sub      sp, sp, #0x20
003c3040: ldr      r3, [r3]
003c3044: mov      r0, r8
003c3048: mov      r6, r2
003c304c: str      r3, [sp, #0x1c]
003c3050: bl       #0x337888
003c3054: ldr      r1, [pc, #0xd0]
003c3058: add      r5, sp, #4
003c305c: mov      r2, sp
003c3060: add      r1, pc, r1
003c3064: mov      r0, r5
003c3068: bl       #0x3140ec
003c306c: mov      r1, r5
003c3070: mov      r0, r8
003c3074: bl       #0x337a88
003c3078: mov      r0, r5
003c307c: bl       #0x318254
003c3080: ldrb     r3, [r6, #0x538]
003c3084: cmp      r3, #0
003c3088: beq      #0x3c30a8
003c308c: ldr      r3, [r4, r7]
003c3090: ldr      r2, [sp, #0x1c]
003c3094: ldr      r3, [r3]
003c3098: cmp      r2, r3
003c309c: bne      #0x3c311c
003c30a0: add      sp, sp, #0x20
003c30a4: pop      {r4, r5, r6, r7, r8, pc}
003c30a8: mov      r3, #0x2380
003c30ac: str      r3, [r6, #0x520]
003c30b0: ldr      r3, [pc, #0x78]
003c30b4: mov      r0, r6
003c30b8: add      r5, r6, #0x490
003c30bc: ldr      r3, [r4, r3]
003c30c0: add      r5, r5, #0xc
003c30c4: ldr      r8, [r3]
003c30c8: bl       #0x3a3228
003c30cc: ldr      r3, [pc, #0x60]
003c30d0: ldr      r1, [pc, #0x60]
003c30d4: ldr      r2, [r4, r3]
003c30d8: mov      r3, #0xa0
003c30dc: mla      r3, r3, r0, r8
003c30e0: ldr      r0, [r2, #0x2c]
003c30e4: ldr      r2, [pc, #0x50]
003c30e8: add      r1, pc, r1
003c30ec: ldr      r8, [r3, #0x28]
003c30f0: add      r2, pc, r2
003c30f4: bl       #0x4c4bdc
003c30f8: ands     r0, r0, #2
003c30fc: bne      #0x3c3110
003c3100: add      r1, r0, r8
003c3104: mov      r0, r5
003c3108: bl       #0x3cacb0
003c310c: b        #0x3c308c
003c3110: mov      r0, r6
003c3114: bl       #0x3a53e0
003c3118: b        #0x3c3100
003c311c: bl       #0x30e310
003c3120: subseq   r1, sp, r0, ror #20
003c3124: andeq    r4, r0, ip, lsr #1
003c3128: andeq    r0, r0, r4, lsl #17
003c312c: ldrsheq  r1, [r0], #-0xd0
003c3130: andeq    r4, r0, r4, asr #16
003c3134: strdeq   r3, r4, [r0], -r4
003c3138: ldrsbeq  r1, [r0], #-0xa0
003c313c: ldrsbeq  r1, [r0], #-0xa8

# _ZNSsC1EPKcRKSaIcE
003140ec: push     {r4, r5, r6, lr}
003140f0: mov      r4, r0
003140f4: str      r0, [r4, #0x10]
003140f8: str      r0, [r4, #0x14]
003140fc: mov      r0, r1
00314100: mov      r5, r1
00314104: bl       #0x30de54
00314108: mov      r1, r5
0031410c: add      r2, r5, r0
00314110: mov      r0, r4
00314114: bl       #0x3116e8
00314118: mov      r0, r4
0031411c: pop      {r4, r5, r6, pc}
